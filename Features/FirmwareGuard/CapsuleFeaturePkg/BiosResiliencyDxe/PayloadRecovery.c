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
#include <Library/SeamlessRecoverySupportLib.h>
#include <Library/PayloadResiliencySupportLib.h>
#include <Guid/SysFwUpdateProgress.h>

extern EFI_HANDLE                        mBackUpFileSystemHandle;

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