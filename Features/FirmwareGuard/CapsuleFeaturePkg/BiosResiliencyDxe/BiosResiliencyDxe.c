/** @file
  Chasm Falls Resiliency init Dxe driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PlatformFlashAccessLib.h>
#include <Library/ComponentUpdateLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/SeamlessRecoverySupportLib.h>
#include <Library/PlatformWdtLib.h>
#include <Library/PrintLib.h>
#include <Library/CapsuleUpdateResetLib.h>
#include <Library/TopSwapSupportLib.h>
#include <Library/PlatformBootStateHookLib.h>
#include <Library/VariablePolicyHelperLib.h>
#include <Library/HashSupportLib.h>
#include <Library/PayloadResiliencySupportLib.h>

#include <Guid/SystemResourceTable.h>
#include <Guid/CapsuleReport.h>
#include <Guid/SysFwUpdateProgress.h>

//
// A duplication of FmpDxe VariableSupport.h.
// @todo: remove FMP private data dependency in future platforms.
//
#define VARNAME_FMPSTATE                L"FmpState"

typedef struct {
  BOOLEAN  VersionValid;
  BOOLEAN  LsvValid;
  BOOLEAN  LastAttemptStatusValid;
  BOOLEAN  LastAttemptVersionValid;
  UINT32   Version;
  UINT32   Lsv;
  UINT32   LastAttemptStatus;
  UINT32   LastAttemptVersion;
} FMP_CONTROLLER_STATE;

extern EFI_HANDLE                        mBackUpFileSystemHandle;
SYSTEM_FIRMWARE_UPDATE_PROGRESS          mPreviousProgress = {0};
BOOLEAN                                  mBiosGuardEnabled = FALSE;

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
  );

/**
  Display message to indicate resiliency actions.

  @param[in] PopUpString1     The first line message
  @param[in] PopUpString2     The second line message
  @param[in] Status           If Status is not NULL, the value would be appended to PopUpString2.

**/

VOID
PopUpMsg (
  IN CHAR16         *PopUpString1,
  IN CHAR16         *PopUpString2,
  IN EFI_STATUS     *Status
  )
{
  CHAR16                            StringWithStatus[200];
  UINTN                             MsgAttribute;
  UINTN                             MsgRelay;

  gST->ConOut->ClearScreen(gST->ConOut);

  MsgRelay     = 3000000; // Three seconds delay after poping up message

  if (IsTopSwapEnabled ()) {
    MsgAttribute = EFI_DARKGRAY | EFI_BACKGROUND_MAGENTA;
  } else {
    MsgAttribute = EFI_DARKGRAY | EFI_BACKGROUND_CYAN;
  }

  if (Status != NULL) {
    ZeroMem (StringWithStatus, sizeof (CHAR16) * 200);
    UnicodeSPrint (StringWithStatus, sizeof (StringWithStatus), L"%s (%r)", PopUpString2, *Status);
    PopUpString2 = StringWithStatus;

    if (EFI_ERROR (*Status)) {
      MsgAttribute = EFI_LIGHTGRAY | EFI_BACKGROUND_RED;
      MsgRelay     = MsgRelay + 5000000; // Five more seconds delay for error status case.
    }
  }

  CreatePopUp (
    MsgAttribute,
    NULL,
    PopUpString1,
    PopUpString2,
    NULL
    );

  gBS->Stall (MsgRelay);
}

/**
  This function either flash IbbR to Ibb (restore) or Ibb to IbbR (back up)
  according to input parameter Restore.

  If Restore = TRUE, it means Ibb is corrupted and system boot from IbbR.
  In this case Ibb needs to be restored from IbbR.

  If Restore = FALSE, it means system boots fine from Ibb.
  In this case Ibb needs to be backed up to IbbR.

  @param[in] Restore            If Restore = TRUE,  IbbR -> Ibb
                                If Restore = FALSE, Ibb  -> IbbR

  @retval Status                Return the stauts of IbbRestoreOrBackUp function

**/
EFI_STATUS
IbbRestoreOrBackUp (
  BOOLEAN       Restore
  )
{
  EFI_STATUS                   Status;
  VOID                         *IbbSourceBuffer;
  UINTN                        IbbSourceSize;
  EFI_PHYSICAL_ADDRESS         TargetFlashAddress;
  VOID                         *BgupBuffer;
  UINTN                        BgupSize;
  FLASH_ACCESS_REQUEST         FlashReq;

  DEBUG ((DEBUG_INFO, "IbbRestoreOrBackUp (%d) - begin\n", Restore));

  IbbSourceBuffer = NULL;
  IbbSourceSize   = 0;
  BgupBuffer      = NULL;
  BgupSize        = 0;

  //
  // 1. Get Ibb source buffer and determine destination flash address
  //
  // When TopSwap is disabled, Ibb region is mapped to PcdFlashIbbBase.
  // When TopSwap is enabled, IbbR region is mapped to PcdFlashIbbBase.
  // So copy Ibb Source buffer from PcdFlashIbbBase MMIO anyway.
  //
  IbbSourceSize = FixedPcdGet32 (PcdFlashIbbSize);
  IbbSourceBuffer = AllocateCopyPool (
                      IbbSourceSize,
                      (VOID *) (UINTN) FixedPcdGet32 (PcdFlashIbbBase)
                      );

  if (IbbSourceBuffer == NULL) {
    DEBUG ((DEBUG_ERROR, "Falied to allocate IbbSourceBuffer with size 0x%x\n", IbbSourceSize));
    Status = EFI_OUT_OF_RESOURCES;
    goto IbbSyncUpExit;
  }

  if (Restore) {
    //
    // System boot from Ibb Recovery region (TopSwap enabled).
    // Flash Ibb to main Ibb region
    //
    TargetFlashAddress = FixedPcdGet32 (PcdFlashIbbOffset);
  } else {
    //
    // System boot from Ibb main region (TopSwap disabled).
    // Flash Ibb to Ibb recovery region
    //
    TargetFlashAddress = FixedPcdGet32 (PcdFlashIbbROffset);
  }

#if FixedPcdGetBool (PcdBiosExtenedRegionEnable) == 1
  // If it's extended bios, recalculate IBB offset.
  if (TargetFlashAddress > FixedPcdGet32 (PcdFlashAllExtendedRegionSize)) {
    TargetFlashAddress = TargetFlashAddress - FixedPcdGet32 (PcdFlashAllExtendedRegionSize) + FixedPcdGet32(PcdFlashExtendSize);
  }
#endif

  //
  // 2. Sync up IBB
  //
  //
  // Read Ibb/IbbR sync up BGUP from FV if BiosGuard is enabled.
  // The script in BGUP would detect TopSwap status and decide to flash Ibb -> IbbR or IbbR -> Ibb
  //
  if (mBiosGuardEnabled) {
    Status = GetSectionFromAnyFv (
               &gIbbBgslFileGuid,
               EFI_SECTION_RAW,
               0,
               (VOID **) &BgupBuffer,
               &BgupSize
               );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Cannot find IbbRBgsl section in any FV (%r)\n", Status));
      goto IbbSyncUpExit;
    }
  }

  //
  // Flash Ibb or IbbR region accordingly.
  //
  ZeroMem (&FlashReq, sizeof (FlashReq));
  FlashReq.FlashRegionType = FlashRegionBios;
  FlashReq.FlashAddress = TargetFlashAddress;
  FlashReq.Data = IbbSourceBuffer;
  FlashReq.DataSize = IbbSourceSize;
  FlashReq.BgupImage = BgupBuffer;
  FlashReq.BgupImageSize = BgupSize;
  Status = UpdateBiosFirmware (&FlashReq, NULL, 0, 0);

IbbSyncUpExit:
  if (IbbSourceBuffer != NULL) {
    FreePool (IbbSourceBuffer);
  }

  if (BgupBuffer != NULL) {
    FreePool (BgupBuffer);
  }

  DEBUG ((DEBUG_INFO, "IbbRestoreOrBackUp - end (%r)\n", Status));
  return Status;
}

/**
  This function used to check whether the input buffer include valid Obb content

  @param[in] ObbSourceBuffer    This is point to the buffer stored for Obb.
  @param[in] ObbSourceSize      This is the obb buffer size.

  @retval Status                Return EFI_SUCCESS means Obb is valid, otherwise, invalid Obb.

**/
EFI_STATUS
IsObbValid (
  IN VOID   *ObbSourceBuffer,
  IN UINTN  ObbSourceSize
  )
{
  EFI_STATUS  Status;
  VOID        *ObbDigestBuffer;
  UINTN       ObbDigestSize;

  if ((ObbSourceBuffer == NULL) || (ObbSourceSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Read Obb digest from FV to examine the integrity of ObbSourceBuffer.
  //
  Status = GetSectionFromAnyFv (
             &gObbSha384HashFileGuid,
             EFI_SECTION_RAW,
             0,
             (VOID **) &ObbDigestBuffer,
             &ObbDigestSize
             );

  DEBUG ((DEBUG_INFO, "Read Obb digest from FV (%r) with size 0x%X\n", Status, ObbDigestSize));

  if (EFI_ERROR (Status) || (ObbDigestSize != BACKUP_FILE_HASH_DIGEST_SIZE)) {
    DEBUG ((DEBUG_ERROR, "Cannot find expected Obb digest section in any FV.\n"));
    return EFI_NOT_FOUND;
  }

  //
  // Examine OBB file digest
  //
  Status = ExamineHashShaDigest (
             (UINT8 *)ObbSourceBuffer,
             ObbSourceSize,
             (UINT8 *)ObbDigestBuffer,
             BACKUP_FILE_HASH_DIGEST_SIZE
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "OBB digest does not match to the one saved in FV.\n"));
  } else {
    DEBUG ((DEBUG_INFO, "OBB digest check passed.\n"));
  }

  return Status;
}

/**
  This function either flash Obb backup file to Obb SPI region (restore) or
  save Obb content on SPI to ESP (back up) according to input parameter Restore.

  If Restore = TRUE, it means Ibb/Obb is corrupted and system boot from IbbR + Obb backup file.
  In this case Obb SPI region needs to be restored from Obb recovery file.

  If Restore = FALSE, it means system boots fine from Ibb + Obb.
  In this case Obb needs to be backed up to ESP.

  @param[in] Restore            If Restore = TRUE,  Obb on ESP -> Obb on SPI
                                If Restore = FALSE, Obb on SPI -> Obb on ESP

  @retval Status                Return the stauts of ObbRestoreOrBackUp function

**/
EFI_STATUS
ObbRestoreOrBackUp (
  BOOLEAN       Restore
  )
{
  EFI_STATUS                   Status;
  VOID                         *ObbSourceBuffer;
  UINTN                        ObbSourceSize;
  VOID                         *BgupBuffer;
  UINTN                        BgupSize;
  FLASH_ACCESS_REQUEST         FlashReq;
  EFI_PHYSICAL_ADDRESS         TargetFlashAddress;

  DEBUG ((DEBUG_INFO, "ObbRestoreOrBackUp (%d) - begin\n", Restore));

  ObbSourceBuffer = NULL;
  ObbSourceSize   = 0;
  BgupBuffer      = NULL;
  BgupSize        = 0;

  //
  // 1. Get Obb source buffer either from ESP or SPI.
  //
  if (Restore) {
    //
    // Get Obb file buffer from ESP.
    //
    Status = ReadBackupFile (
               mBackUpFileSystemHandle,
               SYSFW_UPDATE_CURRENT_OBB_BACKUP_FILE_NAME,
               (VOID **) &ObbSourceBuffer,
               &ObbSourceSize
               );

    DEBUG ((DEBUG_INFO, "Read Obb file from ESP (%r) with size 0x%X\n", Status, ObbSourceSize));

    if (EFI_ERROR (Status) || (ObbSourceSize != FixedPcdGet32 (PcdFlashObbSize))) {
      DEBUG ((DEBUG_ERROR, "Cannot find expected Obb backup file\n"));
      Status = EFI_NOT_FOUND;
      goto ObbSyncUpExit;
    }
  } else {
    //
    // Copy Obb source buffer from SPI.
    //
    ObbSourceSize = FixedPcdGet32 (PcdFlashObbSize);
    ObbSourceBuffer = AllocateCopyPool (
                        ObbSourceSize,
                        (VOID *) (UINTN) FixedPcdGet32 (PcdFlashObbBase)
                        );

    if (ObbSourceBuffer == NULL) {
      DEBUG ((DEBUG_ERROR, "Falied to allocate ObbSourceBuffer with size 0x%x\n", ObbSourceSize));
      Status = EFI_OUT_OF_RESOURCES;
      goto ObbSyncUpExit;
    }
  }

  //
  // 2. Check whether Obb is valid source buffer either from SPI FV or from backup Obb in ESP
  //
  Status = IsObbValid (ObbSourceBuffer, ObbSourceSize);
  if (EFI_ERROR (Status))
    goto ObbSyncUpExit;

  //
  // 3. After checking the integraty of Obb buffer, restore or save Obb accordingly.
  //
  if (Restore) {
    //
    // Read Obb restore BGUP from FV if BiosGuard is enabled.
    // The script in BGUP would flash ObbBuffer to Obb region on SPI.
    //
    if (mBiosGuardEnabled) {
      Status = GetSectionFromAnyFv (
                 &gObbRBgslFileGuid,
                 EFI_SECTION_RAW,
                 0,
                 (VOID **) &BgupBuffer,
                 &BgupSize
                 );

      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Cannot find ObbRBgsl section in any FV (%r)\n", Status));
        goto ObbSyncUpExit;
      }
    }
    TargetFlashAddress = FixedPcdGet32 (PcdFlashObbOffset);

#if FixedPcdGetBool (PcdBiosExtenedRegionEnable) == 1
    // If it's extended bios, recalculate OBB offset.
    if (TargetFlashAddress > FixedPcdGet32 (PcdFlashAllExtendedRegionSize)) {
      TargetFlashAddress = TargetFlashAddress - FixedPcdGet32 (PcdFlashAllExtendedRegionSize) + FixedPcdGet32(PcdFlashExtendSize);
    }
#endif

    //
    // Restore Obb to SPI
    //
    ZeroMem (&FlashReq, sizeof (FlashReq));
    FlashReq.FlashRegionType = FlashRegionBios;
    FlashReq.FlashAddress = TargetFlashAddress;
    FlashReq.Data = ObbSourceBuffer;
    FlashReq.DataSize = ObbSourceSize;
    FlashReq.BgupImage = BgupBuffer;
    FlashReq.BgupImageSize = BgupSize;
    Status = UpdateBiosFirmware (&FlashReq, NULL, 0, 0);
  } else {
    //
    // Save Obb to disk
    //
    DeleteFile (mBackUpFileSystemHandle, SYSFW_UPDATE_CURRENT_OBB_BACKUP_FILE_NAME);
    Status = WriteBackupFile (
               mBackUpFileSystemHandle,
               SYSFW_UPDATE_CURRENT_OBB_BACKUP_FILE_NAME,
               (UINT8 *) (UINTN) ObbSourceBuffer,
               ObbSourceSize
               );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to save Obb to disk (%r)\n", Status));
    }
  }

ObbSyncUpExit:
  if (ObbSourceBuffer != NULL) {
    FreePool (ObbSourceBuffer);
  }

  if (BgupBuffer != NULL) {
    FreePool (BgupBuffer);
  }

  DEBUG ((DEBUG_INFO, "ObbRestoreOrBackUp - end (%r)\n", Status));
  return Status;
}

/**
  Check if it's required to save BIOS Obb to ESP.

  @retval TRUE           Obb backup is required.
  @retval FALSE          Obb backup is not required.

**/
BOOLEAN
IsObbBackUpRequired (
  VOID
  )
{
  EFI_STATUS                          Status;
  VOID                                *ObbSourceBuffer;
  UINTN                               ObbSourceSize;

  //
  // Check if this is the first boot
  //
  if (IsFirstBoot ()) {
    DEBUG ((DEBUG_INFO, "First Boot, BIOS Obb BackUp is required.\n"));
    return TRUE;
  }

  //
  // Check if OBB Backup file exists
  //
  Status = ReadBackupFile (mBackUpFileSystemHandle, SYSFW_UPDATE_CURRENT_OBB_BACKUP_FILE_NAME, (VOID **) &ObbSourceBuffer, &ObbSourceSize);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OBB Backup file does not exist, BIOS Obb BackUp is required.\n"));
    return TRUE;
  } else {

    //
    // Check whether Obb is valid in source buffer
    //
    Status = IsObbValid (ObbSourceBuffer, ObbSourceSize);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OBB Backup file is not valid, BIOS Obb BackUp is required.\n"));
      return TRUE;
    }
  }

  if (!IsTopSwapEnabled () && (mPreviousProgress.Component == UpdatingResiliency)) {
    DEBUG ((DEBUG_INFO, "Post-BIOS update phase and BIOS is healthy, BIOS Backup is required.\n"));
    return TRUE;
  }

  return FALSE;
}

#if FixedPcdGetBool (PcdBiosExtenedRegionEnable) == 1
/**
  This function used to check whether the input buffer include valid Extended Bios content

  @param[in] ExtendedBiosSourceBuffer    This is point to the buffer stored for Extended Bios.
  @param[in] ExtendedBiosSourceSize      This is the obb buffer size.

  @retval Status                Return EFI_SUCCESS means Extended Bios is valid, otherwise, invalid Extended Bios.

**/
EFI_STATUS
IsExtendedValid (
  IN VOID   *ExtendedBiosSourceBuffer,
  IN UINTN  ExtendedBiosSourceSize
  )
{
  EFI_STATUS  Status;
  VOID        *ExtendedBiosDigestBuffer;
  UINTN       ExtendedBiosDigestSize;

  if ((ExtendedBiosSourceBuffer == NULL) || (ExtendedBiosSourceSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Read Obb digest from FV to examine the integrity of ExtendedBiosSourceBuffer.
  //
  Status = GetSectionFromAnyFv (
             &gExtendedSha384HashFileGuid,
             EFI_SECTION_RAW,
             0,
             (VOID **) &ExtendedBiosDigestBuffer,
             &ExtendedBiosDigestSize
             );

  DEBUG ((DEBUG_INFO, "Read ExtendedBios digest from FV (%r) with size 0x%X\n", Status, ExtendedBiosDigestSize));

  if (EFI_ERROR (Status) || (ExtendedBiosDigestSize != BACKUP_FILE_HASH_DIGEST_SIZE)) {
    DEBUG ((DEBUG_ERROR, "Cannot find expected ExtendedBios digest section in any FV.\n"));
    return EFI_NOT_FOUND;
  }

  //
  // Examine extended BIOS file digest
  //
  Status = ExamineHashShaDigest (
             (UINT8 *)((UINTN)ExtendedBiosSourceBuffer),
             ExtendedBiosSourceSize,
             (UINT8 *)((UINTN)ExtendedBiosDigestBuffer),
             BACKUP_FILE_HASH_DIGEST_SIZE
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Extended BIOS region digest does not match to the one saved in FV.\n"));
  } else {
    DEBUG ((DEBUG_INFO, "Extended BIOS region digest check passed.\n"));
  }

  return Status;
}
/**
  This function either flash Extended bios backup file to Extended bios SPI region (restore) or
  save Extended bios content on SPI to ESP (back up) according to input parameter Restore.

  If Restore = TRUE, it means Ibb/Obb/Extended is corrupted and system boot from IbbR + Obb + Extended backup file.
  In this case Extended bios SPI region needs to be restored from Extended bios recovery file.

  If Restore = FALSE, it means system boots fine from Ibb + Obb + Extended bios.
  In this case Extended bios needs to be backed up to ESP.

  @param[in] Restore            If Restore = TRUE,  Extended on ESP -> Extended on SPI
                                If Restore = FALSE, Extended on SPI -> Extended on ESP

  @retval Status                Return the stauts of ExtendedBiosRestoreOrBackUp function

**/
EFI_STATUS
ExtendedBiosRestoreOrBackUp (
  BOOLEAN       Restore
  )
{
  EFI_STATUS                   Status;
  UINT8                         *ExtendedBiosSourceBuffer;
  UINTN                        ExtendedBiosSourceSize;
  VOID                         *BgupBuffer;
  UINTN                        BgupSize;
  FLASH_ACCESS_REQUEST         FlashReq;

  DEBUG ((DEBUG_INFO, "ExtendedBiosRestoreOrBackUp (%d) - begin\n", Restore));

  ExtendedBiosSourceBuffer = NULL;
  ExtendedBiosSourceSize   = 0;
  BgupBuffer      = NULL;
  BgupSize        = 0;

  //
  // 1. Get extended bios source buffer either from ESP or SPI.
  //
  if (Restore) {
    //
    // Get extended bios file buffer from ESP.
    //
    Status = ReadBackupFile (
               mBackUpFileSystemHandle,
               SYSFW_UPDATE_CURRENT_EXTENDED_BIOS_BACKUP_FILE_NAME,
               (VOID **) &ExtendedBiosSourceBuffer,
               &ExtendedBiosSourceSize
               );

    DEBUG ((DEBUG_INFO, "Read Extended bios file from ESP (%r) with size 0x%X\n", Status, ExtendedBiosSourceSize));

    if (EFI_ERROR (Status) || (ExtendedBiosSourceSize != FixedPcdGet32 (PcdFlashExtendSize))) {
      DEBUG ((DEBUG_ERROR, "Cannot find expected Extended bios backup file\n"));
      Status = EFI_NOT_FOUND;
      goto ExtendedBiosSyncUpExit;
    }
  } else {
    //
    // Copy extended bios source buffer from SPI.
    //
    ExtendedBiosSourceSize = FixedPcdGet32 (PcdFlashExtendSize);
    ExtendedBiosSourceBuffer = AllocateCopyPool (
                        ExtendedBiosSourceSize,
                        (VOID *) (UINTN) (FixedPcdGet32 (PcdFlashExtendedRegionBase) + FixedPcdGet32 (PcdFlashExtendOffset))
                        );

    if (ExtendedBiosSourceBuffer == NULL) {
      DEBUG ((DEBUG_ERROR, "Falied to allocate ExtendedBiosSourceBuffer with size 0x%x\n", ExtendedBiosSourceSize));
      Status = EFI_OUT_OF_RESOURCES;
      goto ExtendedBiosSyncUpExit;
    }
  }

  //
  // 2. Check whether Extended BIOS is valid source buffer either from SPI FV or from backup Extended BIOS in ESP
  //
  Status = IsExtendedValid (ExtendedBiosSourceBuffer, ExtendedBiosSourceSize);
  if (EFI_ERROR (Status))
    goto ExtendedBiosSyncUpExit;

  //
  // 3. After checking the integraty of Extended BIOS buffer, restore or save Extended BIOS accordingly.
  //
  if (Restore) {
    //
    // Read Extended BIOS restore BGUP from FV if BiosGuard is enabled.
    // The script in BGUP would flash Extended BIOSBuffer to Extended BIOS region on SPI.
    //
    if (mBiosGuardEnabled) {
      Status = GetSectionFromAnyFv (
                 &gExtendedBgslFileGuid,
                 EFI_SECTION_RAW,
                 0,
                 (VOID **) &BgupBuffer,
                 &BgupSize
                 );

      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Cannot find ObbRBgsl section in any FV (%r)\n", Status));
        goto ExtendedBiosSyncUpExit;
      }
    }

    //
    // Restore Extended BIOS to SPI
    //
    ZeroMem (&FlashReq, sizeof (FlashReq));
    FlashReq.FlashRegionType = FlashRegionBios;
    FlashReq.FlashAddress = 0;
    FlashReq.Data = ExtendedBiosSourceBuffer;
    FlashReq.DataSize = ExtendedBiosSourceSize;
    FlashReq.BgupImage = BgupBuffer;
    FlashReq.BgupImageSize = BgupSize;
    Status = UpdateBiosFirmware (&FlashReq, NULL, 0, 0);
  } else {
    //
    // Save Extended BIOS to disk
    //
    DeleteFile (mBackUpFileSystemHandle, SYSFW_UPDATE_CURRENT_EXTENDED_BIOS_BACKUP_FILE_NAME);
    Status = WriteBackupFile (
               mBackUpFileSystemHandle,
               SYSFW_UPDATE_CURRENT_EXTENDED_BIOS_BACKUP_FILE_NAME,
               (UINT8 *) (UINTN) ExtendedBiosSourceBuffer,
               ExtendedBiosSourceSize
               );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to save Extended BIOS to disk (%r)\n", Status));
    }
  }

ExtendedBiosSyncUpExit:
  if (ExtendedBiosSourceBuffer != NULL) {
    FreePool (ExtendedBiosSourceBuffer);
  }

  if (BgupBuffer != NULL) {
    FreePool (BgupBuffer);
  }

  DEBUG ((DEBUG_INFO, "ExtendedBiosRestoreOrBackUp - end (%r)\n", Status));
  return Status;
}
#endif

/**
  This function intends to update the LastAttempStatus of the latest Capsule update result.
  The latest Capsule udpate result is identified via CapsuleLast variable.

  There are some hard assumptions about FMP controller state variable here:
    1. HardwareInstance is 0
    2. UpdateImageTypeId is identical to gEfiCallerIdGuid in the FMP instance.

  This function accesses the variable created by FmpDxe driver which might be improved in the future.

  @param[in] LastAttemptStatus    The new LastAttempStatus value.

**/
VOID
PatchCapsuleLastAttempStatus (
  UINT32              LastAttemptStatus
  )
{
  EFI_STATUS                          Status;
  UINTN                               Size;
  CHAR16                              CapsuleLastStr[sizeof("Capsule####")];
  EFI_CAPSULE_RESULT_VARIABLE_HEADER  *CapsuleResult;
  UINTN                               CapsuleResultSize;
  EFI_CAPSULE_RESULT_VARIABLE_FMP     *CapsuleResultFmp;
  FMP_CONTROLLER_STATE                *FmpControllerState;

  CapsuleResult      = NULL;
  CapsuleResultSize  = 0;
  CapsuleResultFmp   = NULL;
  FmpControllerState = NULL;

  //
  // Get CapsuleLast varilabe
  //
  ZeroMem (CapsuleLastStr, sizeof (CapsuleLastStr));
  Size = sizeof(L"Capsule####") - sizeof(CHAR16); // no zero terminator
  Status = gRT->GetVariable (
                  L"CapsuleLast",
                  &gEfiCapsuleReportGuid,
                  NULL,
                  &Size,
                  CapsuleLastStr
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get CapsuleLast variable (%r)\n", Status));
    goto Exit;
  }

  //
  // L"CapsuleLast" is got, check if data (CapsuleResult) is there.
  //
  Status = GetVariable2 (
             CapsuleLastStr,
             &gEfiCapsuleReportGuid,
             (VOID **) &CapsuleResult,
             &CapsuleResultSize
             );
  if ((EFI_ERROR (Status)) || (CapsuleResult == NULL)) {
    DEBUG ((DEBUG_ERROR, "Failed to get %s variable (%r)\n", CapsuleLastStr, Status));
    goto Exit;
  } else {
    DEBUG ((DEBUG_INFO, "CapsuleResult variable %s with size 0x%x is got\n", CapsuleLastStr, CapsuleResultSize));
  }

  //
  // CapsuleResult is got. Further parse the required information.
  //
  if ((CapsuleResult->VariableTotalSize != CapsuleResultSize) || \
      (CapsuleResult->VariableTotalSize < sizeof (EFI_CAPSULE_RESULT_VARIABLE_HEADER) + sizeof (EFI_CAPSULE_RESULT_VARIABLE_FMP) + sizeof (CHAR16) * 2) || \
      (!CompareGuid (&CapsuleResult->CapsuleGuid, &gEfiFmpCapsuleGuid))) {
    DEBUG ((DEBUG_ERROR, "Not a FMP CapsuleResult variable.\n"));
    goto Exit;
  }

  CapsuleResultFmp = (EFI_CAPSULE_RESULT_VARIABLE_FMP *)(CapsuleResult + 1);

  DEBUG ((DEBUG_INFO, "Found FMP CapsuleResult with ImageTypeId %g.\n", &CapsuleResultFmp->UpdateImageTypeId));

  //
  // Get corresponding controller state variable in order to update LastAttemptStatus.
  // There are hard assumptions that:
  //   1. HardwareInstance is 0
  //   2. UpdateImageTypeId is identical to gEfiCallerIdGuid in the FMP instance.
  //
  FmpControllerState = NULL;
  Size               = 0;

  Status = GetVariable2 (
             VARNAME_FMPSTATE,
             &CapsuleResultFmp->UpdateImageTypeId,
             (VOID **)&FmpControllerState,
             &Size
             );

  if (EFI_ERROR (Status) || (FmpControllerState == NULL) || (Size != sizeof (*FmpControllerState))) {
    DEBUG ((DEBUG_ERROR, "Failed to get controller state variable (%r). Size is 0x%x\n", Status, Size));
    goto Exit;
  }

  //
  // Patch LastAttempStatus in controller state variable and set it back.
  //
  FmpControllerState->LastAttemptStatus = LastAttemptStatus;
  Status = gRT->SetVariable (
                  VARNAME_FMPSTATE,
                  &CapsuleResultFmp->UpdateImageTypeId,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof (*FmpControllerState),
                  FmpControllerState
                  );

  DEBUG ((DEBUG_INFO, "Patch controller state variable (%g) with LastAttemptStatus 0x%x: %r\n",
    &CapsuleResultFmp->UpdateImageTypeId,
    LastAttemptStatus,
    Status
    ));

Exit:
  if (CapsuleResult != NULL) {
    FreePool (CapsuleResult);
  }

  if (FmpControllerState != NULL) {
    FreePool (FmpControllerState);
  }

}

/**
  This function restore CapsuleLast variable in post-update phase

**/
VOID
RestoreCapsuleLast (
  VOID
  )
{
  EFI_STATUS                       Status;
  UINTN                            Size;
  CHAR16                           CapsuleLastStr[sizeof("Capsule####")];

  DEBUG ((DEBUG_INFO, "RestoreCapsuleLast START\n"));

  //
  // Restore CapsuleLast variable since it is deleted in DxeCapsuleLibConstructor when BootMode is BOOT_ON_FLASH_UPDATE.
  //
  if (mPreviousProgress.Component == UpdatingResiliency) {
    ZeroMem (CapsuleLastStr, sizeof (CapsuleLastStr));
    Size = sizeof(L"Capsule####") - sizeof(CHAR16); // no zero terminator
    Status = gRT->GetVariable (
                    CAPSULE_LAST_BACKUP_VARIABLE_NAME,
                    &gSysFwUpdateProgressGuid,
                    NULL,
                    &Size,
                    CapsuleLastStr
                    );
    if (!EFI_ERROR (Status)) {
      Status = gRT->SetVariable(
                      L"CapsuleLast",
                      &gEfiCapsuleReportGuid,
                      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                      Size,
                      CapsuleLastStr
                      );
      DEBUG ((DEBUG_INFO, "Restore CapsuleLast variable %s (%r)\n", CapsuleLastStr, Status));
      ASSERT_EFI_ERROR (Status);
    }
  }
  gRT->SetVariable (
         CAPSULE_LAST_BACKUP_VARIABLE_NAME,
         &gSysFwUpdateProgressGuid,
         EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
         0,
         NULL
         );
}

/**
  This is a callback function for VariableWriteArchProtocol protocol install event.
  The purpose is to backup CapsuleLast variable in post-update phase (Progress is UpdatingResiliency)
  as earily as possible since it will be deleted in DxeCapsuleLibConstructor
  when BootMode is BOOT_ON_FLASH_UPDATE.

  @param Event           the event that is signaled.
  @param Context         not used here.

**/
VOID
EFIAPI
BackUpCapsuleLast (
  IN EFI_EVENT     Event,
  IN VOID          *Context
  )
{
  VOID                             *VariableWrite;
  EFI_STATUS                       Status;
  UINTN                            Size;
  CHAR16                           CapsuleLastStr[sizeof("Capsule####")];
  EDKII_VARIABLE_POLICY_PROTOCOL   *VariablePolicy = NULL;

  //
  // Make sure VariableWriteArchProtocol is installed.
  //
  Status = gBS->LocateProtocol (
                  &gEfiVariableWriteArchProtocolGuid,
                  NULL,
                  &VariableWrite
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  DEBUG ((DEBUG_INFO, "BackUpCapsuleLast START\n"));

  //
  // Backup CapsuleLast variable since it will be deleted in DxeCapsuleLibConstructor when BootMode is BOOT_ON_FLASH_UPDATE.
  //
  if (mPreviousProgress.Component == UpdatingResiliency) {
    ZeroMem (CapsuleLastStr, sizeof (CapsuleLastStr));
    Size = sizeof(L"Capsule####") - sizeof(CHAR16); // no zero terminator
    Status = gRT->GetVariable (
                    L"CapsuleLast",
                    &gEfiCapsuleReportGuid,
                    NULL,
                    &Size,
                    CapsuleLastStr
                    );
    if (!EFI_ERROR (Status)) {
      Status = gRT->SetVariable(
                      CAPSULE_LAST_BACKUP_VARIABLE_NAME,
                      &gSysFwUpdateProgressGuid,
                      EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                      Size,
                      CapsuleLastStr
                      );
      if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_INFO, "SetVariable fail\n"));
      }
      DEBUG ((DEBUG_INFO, "Back up CapsuleLast variable %s (%r)\n", CapsuleLastStr, Status));
    }
  }

  // Lock CapsuleLastBackUp after EndOfDxe.
  Status = gBS->LocateProtocol (&gEdkiiVariablePolicyProtocolGuid, NULL, (VOID**)&VariablePolicy);
  DEBUG ((DEBUG_INFO, "Locate Variable Policy protocol - %r\n", Status));
  if (!EFI_ERROR (Status)) {
    Status = RegisterBasicVariablePolicy (VariablePolicy,
               &gSysFwUpdateProgressGuid,
               CAPSULE_LAST_BACKUP_VARIABLE_NAME,
               VARIABLE_POLICY_NO_MIN_SIZE,
               VARIABLE_POLICY_NO_MAX_SIZE,
               VARIABLE_POLICY_NO_MUST_ATTR,
               VARIABLE_POLICY_NO_CANT_ATTR,
               VARIABLE_POLICY_TYPE_LOCK_NOW
               );
    ASSERT_EFI_ERROR (Status);
  }

  gBS->CloseEvent (Event);
}

/**
  ResiliencyCallBackFunction

  Chasm Falls Resiliency Feature to determine BIOS Image health and perform
  BIOS restore (rollback) or save (backup) actions accordingly.

  If BIOS is healthy, backup BIOS to IbbR SPI region + ESP
  If BIOS is unhealthy, restore BIOS from IbbR SPI region + ESP

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.

**/
VOID
EFIAPI
ResiliencyCallBackFunction (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                          Status;
  EFI_STATUS                          ResetRegisterStatus;
  BOOLEAN                             ResetRequired;
  SYSTEM_FIRMWARE_UPDATE_PROGRESS     CurrentProgress;

  DEBUG ((DEBUG_INFO, "ResiliencyCallBackFunction START\n"));

  //
  // Disarm WDT first.
  //
  Status = DisarmPlatformWdt ();
  DEBUG ((DEBUG_INFO, "DisArmPlatformWdt: %r\n", Status));

  ResetRequired = FALSE;

  DEBUG ((DEBUG_INFO, "PreviousProgress component = 0x%x.\n", mPreviousProgress.Component));
  DEBUG ((DEBUG_INFO, "PreviousProgress progress  = 0x%x.\n", mPreviousProgress.Progress));

  if ((mPreviousProgress.Component != NotStarted) &&
      (mPreviousProgress.Component != UpdatingResiliency)) {
    //
    // Other Component types indicate an interrupted update or MeRecovery which should be addressed
    // in FmpDeviceLib instances invoked earlier than this callback function.
    //
    ASSERT (FALSE);
    DEBUG ((DEBUG_ERROR, "Unexpected PreviousProgress Component\n"));
    return;
  }

  GetCurrentUpdateProgress (&CurrentProgress);

  //
  // Restore CapsuleLast if required.
  //
  RestoreCapsuleLast ();

  Status = InitializeBackupFileSystem ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Abort this function since ESP is required for Resiliency feature.\n"));
    //
    // Delete progress variable if no ESP, then no resiliency
    //
    ClearUpdateProgress ();
    return;
  }

  if (IsTopSwapEnabled ()) {
    //
    // TopSwap is enabled. Restore BIOS
    // System boot from IbbR region + Obb file loaded from ESP.
    //
    // 1. Flash IbbR content to Ibb SPI region
    //
    PopUpMsg (L"Resiliency rollback (TopSwap is enabled)", L"Restore Ibb from IbbR - Begin...", NULL);
    Status = IbbRestoreOrBackUp (TRUE);
    ASSERT_EFI_ERROR (Status);
    PopUpMsg (L"Resiliency rollback (TopSwap is enabled)", L"Restore Ibb from IbbR - End", &Status);

    //
    // 2. Flash Obb file from ESP to Obb SPI region
    //
    PopUpMsg (L"Resiliency rollback (TopSwap is enabled)", L"Restore Obb from ESP - Begin...", NULL);
    Status = ObbRestoreOrBackUp (TRUE);
    ASSERT_EFI_ERROR (Status);
//  @Todo: Temporarily disabled due to other depedency.
//         Need to revert when enabling NonFitPayload update/recovery
//    if (IsPayloadBackupEnabled ()) {
//      Status = NonFitPayloadRestoreOrBackUp (TRUE);
//      ASSERT_EFI_ERROR (Status);
//    }
    PopUpMsg (L"Resiliency rollback (TopSwap is enabled)", L"Restore Obb from ESP - End", &Status);

#if FixedPcdGetBool (PcdBiosExtenedRegionEnable) == 1
    //
    // Flash Extended BIOS file from ESP to Extended BIOS SPI region
    //
    PopUpMsg (L"Resiliency rollback (TopSwap is enabled)", L"Restore Extended BIOS from ESP - Begin...", NULL);
    Status = ExtendedBiosRestoreOrBackUp (TRUE);
    ASSERT_EFI_ERROR (Status);
    PopUpMsg (L"Resiliency rollback (TopSwap is enabled)", L"Restore Extended BIOS from ESP - End", &Status);
#endif

    //
    // 3. Disable TopSwap, clear progress variable and restart the system
    //
    TopSwapControl (FALSE);

    //
    // Keep update progress if there is MeResiliency required.
    //
    if (CurrentProgress.Component != UpdatingMeResiliency) {
      ClearUpdateProgress ();
    }

    //
    // 4. Report an error in LastAttempStatus to indicate update failure if this is a post-update phase
    //
    if (mPreviousProgress.Component == UpdatingResiliency) {
      PatchCapsuleLastAttempStatus (LAST_ATTEMPT_STATUS_ERROR_INVALID_FORMAT);
    }

    DEBUG ((DEBUG_INFO, "BIOS restore finished.\n"));
    DEBUG ((DEBUG_INFO, "TopSwap bit is: %d\n", IsTopSwapEnabled ()));
    PopUpMsg (L"BIOS restore finished", NULL, NULL);
    ResetRequired = TRUE;
  } else if (IsObbBackUpRequired ()) {
    //
    // TopSwap is disabled. Back up BIOS
    // System boot from Ibb + Obb SPI region
    //
    // 1. Flash Ibb content to IbbR SPI region if this is post-BIOS update.
    //
    if (mPreviousProgress.Component == UpdatingResiliency) {
      PopUpMsg (L"Resiliency backup", L"Backup Ibb to IbbR - Begin...", NULL);
      Status = IbbRestoreOrBackUp (FALSE);
      ASSERT_EFI_ERROR (Status);
      PopUpMsg (L"Resiliency backup", L"Backup Ibb to IbbR - End", &Status);
      ResetRequired = TRUE;
    }

    //
    // 2. Save Obb content to a file on ESP
    //
    PopUpMsg (L"Resiliency backup", L"Backup Obb to Esp - Begin...", NULL);
    Status = ObbRestoreOrBackUp (FALSE);
    ASSERT_EFI_ERROR (Status);
    if (IsPayloadBackupEnabled ()) {
      Status = NonFitPayloadRestoreOrBackUp (FALSE);
      ASSERT_EFI_ERROR (Status);
    }
    PopUpMsg (L"Resiliency backup", L"Backup Obb to Esp - End", &Status);

#if FixedPcdGetBool (PcdBiosExtenedRegionEnable) == 1
    //
    // Save ExtendedBios content to a file on ESP
    //
    PopUpMsg (L"Resiliency backup", L"Backup ExtendedBios to Esp - Begin...", NULL);
    Status = ExtendedBiosRestoreOrBackUp (FALSE);
    ASSERT_EFI_ERROR (Status);
    PopUpMsg (L"Resiliency backup", L"Backup ExtendedBios to Esp - End", &Status);
#endif

    //
    // 3. Clear progress variable
    //
    //
    // Keep update progress if there is MeResiliency required.
    //
    if (CurrentProgress.Component != UpdatingMeResiliency) {
      ClearUpdateProgress ();
    }

    DEBUG ((DEBUG_INFO, "BIOS backup finished.\n"));
    PopUpMsg (L"BIOS backup finished", NULL, NULL);
    gST->ConOut->ClearScreen(gST->ConOut);
  }

  if ((mPreviousProgress.Component == UpdatingResiliency) && (mPreviousProgress.Progress & SYSFW_UPDATE_RESILIENCY_PENDING_ACTION_EC_RESET)) {
    //
    // Register specific reset handler to perform EC reset
    //
    ResetRegisterStatus = CapsuleUpdateRegisterResetHandler (CAPSULE_UPDATE_RESET_TYPE_EC_RESET);
    DEBUG ((DEBUG_INFO, "RegisterResetHandler (ec reset) - %r\n", ResetRegisterStatus));
    if (ResetRegisterStatus == EFI_SUCCESS) {
      //
      // Change reset type to EC reset. The assumption is that EC reset is deeper than PCH global reset.
      //
      ResetRegisterStatus = CapsuleUpdateRegisterResetFilter (CAPSULE_UPDATE_RESET_TYPE_EC_RESET);
      DEBUG ((DEBUG_INFO, "RegisterResetFilter (%x) - %r\n", CAPSULE_UPDATE_RESET_TYPE_EC_RESET, ResetRegisterStatus));
    }
    ResetRequired = TRUE;
  }

  if (ResetRequired) {
    DEBUG ((DEBUG_INFO, "End of resiliency process. Restart the system...\n"));
    PopUpMsg (L"End of Resiliency process.", L"Restart the system...", NULL);
    gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
  }

  DEBUG ((DEBUG_INFO, "ResiliencyCallBackFunction END\n"));
}

/**
  Initialize Resiliency Support for Chasm Falls

  @retval EFI_SUCCESS    Resiliency Support is initialized successfully
  @retval EFI_NOT_FOUND  Resiliency Support is not initialized successfully
**/

EFI_STATUS
EFIAPI
BiosResiliencyDxeEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  EFI_STATUS                Status;
  VOID                      *Registration;
  EFI_EVENT                 BeforeEndOfDxeEvent;

  DEBUG ((DEBUG_INFO, "ResiliencyDxeEntryPoint START\n"));

  Status                    = EFI_SUCCESS;
  BeforeEndOfDxeEvent       = NULL;
  mBiosGuardEnabled         = IsBiosGuardEnabled ();

  IsPreviousUpdateUnfinished (&mPreviousProgress);

  EfiCreateProtocolNotifyEvent (
    &gEfiVariableWriteArchProtocolGuid,
    TPL_CALLBACK,
    BackUpCapsuleLast,
    NULL,
    &Registration
    );

  DEBUG ((DEBUG_INFO, "Register an Before EndOfDxe Callback Function for Resiliency Health Check \n"));
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  ResiliencyCallBackFunction,
                  NULL,
                  &gPlatformBeforeEndOfDxeEventGroupGuid,
                  &BeforeEndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "ResiliencyDxeEntryPoint END\n"));
  return Status;
}
