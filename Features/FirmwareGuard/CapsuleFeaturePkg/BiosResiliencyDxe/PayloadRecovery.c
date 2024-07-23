/**  @file
  Payloads Management recovery driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

  @par Specification Reference:
**/

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/ComponentUpdateLib.h>
#include <Library/HashSupportLib.h>
#include <IndustryStandard/FirmwareInterfaceTable.h>
#include <Library/SeamlessRecoverySupportLib.h>
#include <Library/PayloadResiliencySupportLib.h>
#include <Library/SpiAccessLib.h>
#include <Guid/SysFwUpdateProgress.h>

extern EFI_HANDLE                        mBackUpFileSystemHandle;

/**
  This function get the block address which contains the Fit table and copy it to the memory

  @param[out] FitEntry          Pointer to the Fit table in memory
  @param[out] FitBlockBuffer    Pointer to the block address in memory which contains the Fit table

  @retval Status                Return the status of GetFitEntry function

**/
EFI_STATUS
GetFitEntry (
  IN  BOOLEAN                             Restore,
  OUT FIRMWARE_INTERFACE_TABLE_ENTRY      **FitEntry,
  OUT UINTN                               *FitEntrySize
  )
{
  UINT64                            FitEntryInFlash;
  UINT64                            FitTableBackupOffset;
  UINT64                            FitTableBackupSize;
  UINT32                            EntryNum;

  FitTableBackupOffset = FIT_TABLE_BACKUP_OFFSET;
  FitTableBackupSize   = FIT_TABLE_BACKUP_SIZE;

  if (Restore) {
    FitEntryInFlash = *(UINT64 *) (UINTN) (FIT_POINTER_ADDRESS) + FitTableBackupOffset;
  } else {
    FitEntryInFlash = *(UINT64 *) (UINTN) (FIT_POINTER_ADDRESS - FixedPcdGet32 (PcdFlashIbbSize)) \
                        - FixedPcdGet32 (PcdFlashIbbSize) + FitTableBackupOffset + FitTableBackupSize;
  }

  *FitEntry = (FIRMWARE_INTERFACE_TABLE_ENTRY *)FitEntryInFlash;
  EntryNum = (((UINT32)(*FitEntry)->Size[2] << 16) | ((UINT32)(*FitEntry)->Size[1] << 8) | (UINT32)(*FitEntry)->Size[0]) & 0xFFFFFF;
  *FitEntrySize = EntryNum * sizeof (FIRMWARE_INTERFACE_TABLE_ENTRY);

  if (((*FitEntry)->Type != FIT_TYPE_00_HEADER) || ((*FitEntry)->Address != FIT_TYPE_00_SIGNATURE)) {
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  This function either flash FitPayloadR to FitPayload (restore) or FitPayload to FitPayloadR (back up)
  according to input parameter Restore.

  If Restore = TRUE, it means FitPayload is corrupted and system boot from FitPayloadR.
  In this case FitPayload needs to be restored from FitPayloadR.

  If Restore = FALSE, it means system boots fine from FitPayload.
  In this case FitPayload needs to be backed up to FitPayloadR.

  @param[in] Restore            If Restore = TRUE,  FitPayloadR -> FitPayload
                                If Restore = FALSE, FitPayload  -> FitPayloadR

  @retval Status                Return the status of FitPayloadRestoreOrBackUp function

**/
EFI_STATUS
FitPayloadRestoreOrBackUp (
  BOOLEAN                      Restore
  )
{
  EFI_STATUS                     Status;
  VOID                           *FitPayloadSourceBuffer;
  UINTN                          FitPayloadSourceSize;
  VOID                           *FitTableSourceBuffer;
  FIRMWARE_INTERFACE_TABLE_ENTRY *FitTableSourcePointer;
  UINTN                          FitTableSourceSize;
  EFI_PHYSICAL_ADDRESS           FitPayloadTargetFlashAddress;
  EFI_PHYSICAL_ADDRESS           FitTableTargetFlashAddress;
  VOID                           *FitPayloadBgupBuffer;
  UINTN                          FitPayloadBgupSize;
  VOID                           *FitTableBgupBuffer;
  UINTN                          FitTableBgupSize;
  FLASH_ACCESS_REQUEST           FlashReq;

  DEBUG ((DEBUG_INFO, "FitPayloadRestoreOrBackUp (%d) - begin\n", Restore));

  FitPayloadSourceBuffer = NULL;
  FitTableSourcePointer  = NULL;
  FitPayloadSourceSize   = 0;
  FitTableSourceBuffer   = NULL;
  FitTableSourceSize     = 0;
  FitPayloadBgupBuffer   = NULL;
  FitPayloadBgupSize     = 0;
  FitTableBgupBuffer     = NULL;
  FitTableBgupSize       = 0;

  //
  // 1. Get FitPayload source buffer and determine destination flash address. Currently only support Microcode
  //
  FitPayloadSourceSize = FixedPcdGet32 (PcdFlashFvMicrocodeSize);
  DEBUG ((DEBUG_ERROR, "FitPayloadSourceSize = 0x%x\n", FitPayloadSourceSize));
  DEBUG ((DEBUG_ERROR, "PcdBiosAreaBaseAddress = 0x%x\n", FixedPcdGet32 (PcdBiosAreaBaseAddress)));

  Status = GetFitEntry (Restore, &FitTableSourcePointer, &FitTableSourceSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error: Invalid FIT-R header\n"));
    goto FitPayloadSyncUpExit;
  }

  if (Restore) {
    //
    // Flash FitPayloadR to main FitPayload region
    //
    FitPayloadSourceBuffer = AllocateCopyPool (
                            FitPayloadSourceSize,
                            (VOID *) (UINTN) FixedPcdGet32 (PcdFlashFvMicrocodeRBase)
                            );
    if (FitPayloadSourceBuffer == NULL) {
      DEBUG ((DEBUG_ERROR, "Failed to allocate FitPayloadSourceBuffer with size 0x%x\n", FitPayloadSourceSize));
      Status = EFI_OUT_OF_RESOURCES;
      goto FitPayloadSyncUpExit;
    }
    FitPayloadTargetFlashAddress = (UINT64) FixedPcdGet32 (PcdFlashFvMicrocodeOffset);

    FitTableSourceBuffer = AllocateCopyPool (FitTableSourceSize, (VOID *)FitTableSourcePointer);
    if (FitTableSourceBuffer == NULL) {
      DEBUG ((DEBUG_ERROR, "Failed to allocate FitTableSourceBuffer with size 0x%x\n", FitTableSourceSize));
      Status = EFI_OUT_OF_RESOURCES;
      goto FitPayloadSyncUpExit;
    }
    FitTableTargetFlashAddress = *(EFI_PHYSICAL_ADDRESS *) (UINTN) (FIT_POINTER_ADDRESS) & 0xFFFFFF;
  } else {
    //
    // Flash FitPayload to FitPayloadR region
    //
    FitPayloadSourceBuffer = AllocateCopyPool (
                          FitPayloadSourceSize,
                          (VOID *) (UINTN) FixedPcdGet32 (PcdFlashFvMicrocodeBase)
                          );
    if (FitPayloadSourceBuffer == NULL) {
      DEBUG ((DEBUG_ERROR, "Failed to allocate FitPayloadSourceBuffer with size 0x%x\n", FitPayloadSourceSize));
      Status = EFI_OUT_OF_RESOURCES;
      goto FitPayloadSyncUpExit;
    }
    FitPayloadTargetFlashAddress = (UINT64) FixedPcdGet32 (PcdFlashFvMicrocodeROffset);

    FitTableSourceBuffer = AllocateCopyPool (FitTableSourceSize, (VOID *)FitTableSourcePointer);
    if (FitTableSourceBuffer == NULL) {
      DEBUG ((DEBUG_ERROR, "Failed to allocate FitTableSourceBuffer with size 0x%x\n", FitTableSourceSize));
      Status = EFI_OUT_OF_RESOURCES;
      goto FitPayloadSyncUpExit;
    }
    FitTableTargetFlashAddress = (*(EFI_PHYSICAL_ADDRESS *) (UINTN) (FIT_POINTER_ADDRESS) - FixedPcdGet32 (PcdFlashIbbSize)) & 0xFFFFFF;
  }

  //
  // 3. Sync up FitPayload and FIT table
  //
  // Read FitPayload/FitPayloadR sync up BGUP from FV if BiosGuard is enabled.
  // The script in BGUP would detect TopSwap status and decide to flash FitPayload -> FitPayloadR or FitPayloadR -> FitPayload
  //
  if (IsBiosGuardEnabled ()) {
    Status = GetSectionFromAnyFv (
               &gUcodeBgslFileGuid,
               EFI_SECTION_RAW,
               0,
               (VOID **) &FitPayloadBgupBuffer,
               &FitPayloadBgupSize
               );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Cannot find Microcode Bgsl section in any FV (%r)\n", Status));
      goto FitPayloadSyncUpExit;
    }

    Status = GetSectionFromAnyFv (
               &gFitTableSyncBgslFileGuid,
               EFI_SECTION_RAW,
               0,
               (VOID **) &FitTableBgupBuffer,
               &FitTableBgupSize
               );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Cannot find Fit Table Bgsl section in any FV (%r)\n", Status));
      goto FitPayloadSyncUpExit;
    }
  }

  //
  // 4. Flash FitPayload or FitPayloadR region accordingly.
  //
  ZeroMem (&FlashReq, sizeof (FlashReq));
  FlashReq.FlashRegionType = FlashRegionBios;
  FlashReq.FlashAddress = FitPayloadTargetFlashAddress;
  FlashReq.Data = FitPayloadSourceBuffer;
  FlashReq.DataSize = FitPayloadSourceSize;
  FlashReq.BgupImage = FitPayloadBgupBuffer;
  FlashReq.BgupImageSize = FitPayloadBgupSize;
  Status = UpdateBiosFirmware (&FlashReq, NULL, 0, 0);

  if (EFI_ERROR (Status)) {
    goto FitPayloadSyncUpExit;
  }

  //
  // 5. Flash FIT Table region.
  //
  ZeroMem (&FlashReq, sizeof (FlashReq));
  FlashReq.FlashRegionType = FlashRegionBios;
  FlashReq.FlashAddress = FitTableTargetFlashAddress;
  FlashReq.Data = FitTableSourceBuffer;
  FlashReq.DataSize = FitTableSourceSize;
  FlashReq.BgupImage = FitTableBgupBuffer;
  FlashReq.BgupImageSize = FitTableBgupSize;
  Status = UpdateBiosFirmware (&FlashReq, NULL, 0, 0);

  if (EFI_ERROR (Status)) {
    goto FitPayloadSyncUpExit;
  }

FitPayloadSyncUpExit:
  if (FitPayloadSourceBuffer != NULL) {
    FreePool (FitPayloadSourceBuffer);
  }

  if (FitPayloadBgupBuffer != NULL) {
    FreePool (FitPayloadBgupBuffer);
  }

  if (FitTableSourceBuffer != NULL) {
    FreePool (FitTableSourceBuffer);
  }

  if (FitTableBgupBuffer != NULL) {
    FreePool (FitTableBgupBuffer);
  }

  DEBUG ((DEBUG_INFO, "FitPayloadRestoreOrBackUp - end (%r)\n", Status));
  return Status;
}

/**
  This function used to check whether the input buffer include valid NonFitPayload content

  @param[in] NonFitPayloadSourceBuffer    This is point to the buffer stored for NonFitPayload.
  @param[in] NonFitPayloadSourceSize      This is the NonFitPayload buffer size.

  @retval Status                          Return EFI_SUCCESS means NonFitPayload is valid, otherwise, invalid NonFitPayload.

**/
EFI_STATUS
IsNonFitPayloadValid (
  IN VOID   *NonFitPayloadSourceBuffer,
  IN UINTN  NonFitPayloadSourceSize
  )
{
  EFI_STATUS  Status;
  VOID        *NonFitPayloadDigestBuffer;
  UINTN       NonFitPayloadDigestSize;

  if ((NonFitPayloadSourceBuffer == NULL) || (NonFitPayloadSourceSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Read NonFitPayload digest from FV to examine the integrity of NonFitPayloadSourceBuffer.
  //
  Status = GetSectionFromAnyFv (
             &gPayloadSha384HashFileGuid,
             EFI_SECTION_RAW,
             0,
             (VOID **) &NonFitPayloadDigestBuffer,
             &NonFitPayloadDigestSize
             );

  DEBUG ((DEBUG_INFO, "Read NonFitPayload digest from FV (%r) with size 0x%X\n", Status, NonFitPayloadDigestSize));

  if (EFI_ERROR (Status) || (NonFitPayloadDigestSize != BACKUP_FILE_HASH_DIGEST_SIZE)) {
    DEBUG ((DEBUG_ERROR, "Cannot find expected NonFitPayload digest section in any FV.\n"));
    return EFI_NOT_FOUND;
  }

  //
  // Examine NonFitPayload file digest
  //
  Status = ExamineHashShaDigest (
             (UINT8 *)NonFitPayloadSourceBuffer,
             NonFitPayloadSourceSize,
             (UINT8 *)NonFitPayloadDigestBuffer,
             BACKUP_FILE_HASH_DIGEST_SIZE
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "NonFitPayload digest does not match to the one saved in FV.\n"));
  } else {
    DEBUG ((DEBUG_INFO, "NonFitPayload digest check passed.\n"));
  }

  return Status;
}

/**
  This function either flash NonFitPayload backup file to NonFitPayload SPI region (restore) or
  save NonFitPayload content on SPI to ESP (back up) according to input parameter Restore.

  If Restore = TRUE, it means Ibb/NonFitPayload is corrupted and system boot from IbbR + NonFitPayload backup file.
  In this case NonFitPayload SPI region needs to be restored from NonFitPayload recovery file.

  If Restore = FALSE, it means system boots fine from Ibb + NonFitPayload.
  In this case NonFitPayload needs to be backed up to ESP.

  @param[in] Restore            If Restore = TRUE,  NonFitPayload on ESP -> NonFitPayload on SPI
                                If Restore = FALSE, NonFitPayload on SPI -> NonFitPayload on ESP

  @retval Status                Return the stauts of NonFitPayloadRestoreOrBackUp function

**/
EFI_STATUS
NonFitPayloadRestoreOrBackUp (
  BOOLEAN       Restore
  )
{
  EFI_STATUS                   Status;
  VOID                         *NonFitPayloadSourceBuffer;
  UINTN                        NonFitPayloadSourceSize;
  VOID                         *BgupBuffer;
  UINTN                        BgupSize;
  FLASH_ACCESS_REQUEST         FlashReq;
  EFI_PHYSICAL_ADDRESS         TargetFlashAddress;

  DEBUG ((DEBUG_INFO, "NonFitPayloadRestoreOrBackUp (%d) - begin\n", Restore));

  NonFitPayloadSourceBuffer = NULL;
  NonFitPayloadSourceSize   = 0;
  BgupBuffer                = NULL;
  BgupSize                  = 0;

  //
  // 1. Get NonFitPayload source buffer either from ESP or SPI.
  //
  if (Restore) {
    //
    // Get NonFitPayload file buffer from ESP.
    //
    Status = ReadBackupFile (
               mBackUpFileSystemHandle,
               SYSFW_UPDATE_CURRENT_PAYLOAD_BACKUP_FILE_NAME,
               (VOID **) &NonFitPayloadSourceBuffer,
               &NonFitPayloadSourceSize
               );

    DEBUG ((DEBUG_INFO, "Read NonFitPayload file from ESP (%r) with size 0x%X\n", Status, NonFitPayloadSourceSize));

    if (EFI_ERROR (Status) || (NonFitPayloadSourceSize != FixedPcdGet32 (PcdFlashNonFitPayloadSize))) {
      DEBUG ((DEBUG_ERROR, "Cannot find expected NonFitPayload backup file\n"));
      Status = EFI_NOT_FOUND;
      goto NonFitPayloadSyncUpExit;
    }
  } else {
    //
    // Copy NonFitPayload source buffer from SPI.
    //
    NonFitPayloadSourceSize = FixedPcdGet32 (PcdFlashNonFitPayloadSize);
    NonFitPayloadSourceBuffer = AllocateCopyPool (
                        NonFitPayloadSourceSize,
                        (VOID *) (UINTN) FixedPcdGet32 (PcdFlashNonFitPayloadBase)
                        );

    if (NonFitPayloadSourceBuffer == NULL) {
      DEBUG ((DEBUG_ERROR, "Falied to allocate NonFitPayloadSourceBuffer with size 0x%x\n", NonFitPayloadSourceSize));
      Status = EFI_OUT_OF_RESOURCES;
      goto NonFitPayloadSyncUpExit;
    }
  }

  //
  // 2. Check whether NonFitPayload is valid source buffer either from SPI FV or from backup NonFitPayload in ESP
  //
  Status = IsNonFitPayloadValid (NonFitPayloadSourceBuffer, NonFitPayloadSourceSize);
  if (EFI_ERROR (Status))
    goto NonFitPayloadSyncUpExit;

  //
  // 3. After checking the integrity of NonFitPayload buffer, restore or save NonFitPayload accordingly.
  //
  if (Restore) {
    //
    // Read NonFitPayload restore BGUP from FV if BiosGuard is enabled.
    // The script in BGUP would flash NonFitPayloadBuffer to NonFitPayload region on SPI.
    //
    if (IsBiosGuardEnabled ()) {
      Status = GetSectionFromAnyFv (
                 &gNonFitPayloadBgslFileGuid,
                 EFI_SECTION_RAW,
                 0,
                 (VOID **) &BgupBuffer,
                 &BgupSize
                 );

      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Cannot find NonFitPayloadBgsl section in any FV (%r)\n", Status));
        goto NonFitPayloadSyncUpExit;
      }
    }
    TargetFlashAddress = FixedPcdGet32 (PcdFlashNonFitPayloadOffset);

#if FixedPcdGetBool (PcdBiosExtenedRegionEnable) == 1
    // If it's extended bios, recalculate PcdFlashNonFitPayloadOffset offset.
    if (TargetFlashAddress > FixedPcdGet32 (PcdFlashAllExtendedRegionSize)) {
      TargetFlashAddress = TargetFlashAddress - FixedPcdGet32 (PcdFlashAllExtendedRegionSize) + FixedPcdGet32(PcdFlashExtendSize);
    }
#endif

    //
    // Restore PcdFlashNonFitPayloadOffset to SPI
    //
    ZeroMem (&FlashReq, sizeof (FlashReq));
    FlashReq.FlashRegionType = FlashRegionBios;
    FlashReq.FlashAddress = TargetFlashAddress;
    FlashReq.Data = NonFitPayloadSourceBuffer;
    FlashReq.DataSize = NonFitPayloadSourceSize;
    FlashReq.BgupImage = BgupBuffer;
    FlashReq.BgupImageSize = BgupSize;
    Status = UpdateBiosFirmware (&FlashReq, NULL, 0, 0);
  } else {
    //
    // Save PcdFlashNonFitPayloadOffset to disk
    //
    DeleteFile (mBackUpFileSystemHandle, SYSFW_UPDATE_CURRENT_PAYLOAD_BACKUP_FILE_NAME);
    Status = WriteBackupFile (
               mBackUpFileSystemHandle,
               SYSFW_UPDATE_CURRENT_PAYLOAD_BACKUP_FILE_NAME,
               (UINT8 *) (UINTN) NonFitPayloadSourceBuffer,
               NonFitPayloadSourceSize
               );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to save NonFitPayload to disk (%r)\n", Status));
    }
  }

NonFitPayloadSyncUpExit:
  if (NonFitPayloadSourceBuffer != NULL) {
    FreePool (NonFitPayloadSourceBuffer);
  }

  if (BgupBuffer != NULL) {
    FreePool (BgupBuffer);
  }

  DEBUG ((DEBUG_INFO, "NonFitPayloadRestoreOrBackUp - end (%r)\n", Status));
  return Status;
}