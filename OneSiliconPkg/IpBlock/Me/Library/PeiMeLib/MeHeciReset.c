/** @file
  ME HECI reset PEI lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#include <Base.h>
#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/ResetSystemLib.h>
#include <Library/MeUtilsLib.h>
#include <Ppi/PlatformSpecificResetHandler.h>
#include <Ppi/PlatformSpecificResetNotification.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiMeLib.h>
#include <Library/HobLib.h>
#include <Library/PmcPrivateLib.h>
#include <Register/EspiRegs.h>
#include <PchConfigHobCommon.h>
#include <PchPolicyCommon.h>

STATIC EFI_GUID mResetCallbackPtrGuid = { 0x08e0ae2a, 0x29a3, 0x4c79, { 0xbc, 0x2b, 0xcd, 0x16, 0xdf, 0xa6, 0x52, 0x90 }};
STATIC EFI_GUID mResetNotificationCallbackPtrGuid = { 0x1c9057fb, 0x1f2c, 0x4219, { 0xac, 0xf8, 0xbe, 0xa3, 0x57, 0xe4, 0x7f, 0xdf }};

typedef struct {
  EFI_RESET_SYSTEM  ResetFunction;
} RESET_CALLBACK_PTR_HOB;

/**
  Print reset message for debug build readability
**/
VOID
EFIAPI
PrintResetMessage (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "******************************\n"));
  DEBUG ((DEBUG_INFO, "**    SYSTEM REBOOT !!!     **\n"));
  DEBUG ((DEBUG_INFO, "******************************\n"));
}

/**
  Wait until CSME completes unconfiguration process.

  @retval EFI_SUCCESS             Unconfiguration not in progress
  @retval EFI_UNSUPPORTED         Unconfiguration error
  @retval EFI_TIMEOUT             Timeout exceeded
**/
EFI_STATUS
WaitForUnconfigFinish (
  VOID
  )
{
  EFI_STATUS Status;
  UINT8      UnconfigStatus;
  UINT8      Timeout;

  Timeout = ME_UNCONFIG_TIMEOUT;

  do {
    Status = PeiHeciGetUnconfigureStatus (&UnconfigStatus);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    switch (UnconfigStatus) {
      case ME_UNCONFIG_IN_PROGRESS:
        MicroSecondDelay (1000000);
        break;

      case ME_UNCONFIG_NOT_IN_PROGRESS:
      case ME_UNCONFIG_FINISHED:
        return EFI_SUCCESS;

      default:
        return EFI_UNSUPPORTED;
    }
  } while (Timeout--);

  return EFI_TIMEOUT;
}

/**
  Sent HECI command to ME for GlobalReset

  @param[in]  ResetType         The type of reset to perform.
  @param[in]  ResetStatus       The status code for the reset.
  @param[in]  DataSize          The size, in bytes, of ResetData.
  @param[in]  ResetData         For a ResetType of EfiResetCold, EfiResetWarm, or
                                EfiResetShutdown the data buffer starts with a Null-terminated
                                string, optionally followed by additional binary data.
                                The string is a description that the caller may use to further
                                indicate the reason for the system reset.
                                For a ResetType of EfiResetPlatformSpecific the data buffer
                                also starts with a Null-terminated string that is followed
                                by an EFI_GUID that describes the specific type of reset to perform.
**/
VOID
EFIAPI
PeiMeResetHandlerCallback (
  IN EFI_RESET_TYPE           ResetType,
  IN EFI_STATUS               ResetStatus,
  IN UINTN                    DataSize,
  IN VOID                     *ResetData OPTIONAL
  )
{
  EFI_GUID                *GuidPtr;
  EFI_PEI_HOB_POINTERS    HobPtr;
  EFI_STATUS              Status;
  PCH_GENERAL_CONFIG_HOB  *PchGeneralHob;
  EFI_PEI_HOB_POINTERS    HobList;

  // Get PCH GENERAL HOB.
  HobList.Guid = GetFirstGuidHob (&gPchGeneralConfigHobGuid);
  if (HobList.Guid != NULL) {
    PchGeneralHob = (PCH_GENERAL_CONFIG_HOB *) GET_GUID_HOB_DATA (HobList.Guid);

    if (PchGeneralHob->DisableResets == FORCE_ENABLE) {
      DEBUG ((DEBUG_INFO, "DisableResets is enabled. Reset skipped.\n"));
      return;
    }
  }

  PrintResetMessage ();
  if (ResetType != EfiResetPlatformSpecific || (ResetData == NULL)) {
    return;
  }

  GuidPtr = (EFI_GUID *) ((UINT8 *) ResetData + DataSize - sizeof (EFI_GUID));
  if (!CompareGuid (GuidPtr, &gPchGlobalResetGuid)) {
    return;
  }

  //
  // PCH BIOS Spec Section 4.6 GPIO Reset Requirement
  //
  DEBUG ((DEBUG_INFO, "Enable CF9 Global Reset - PEI\n"));
  PmcEnableCf9GlobalReset ();

  if (MeSkuIsClient ()) {
    HobPtr.Guid = GetFirstGuidHob (&gMeDidSentHobGuid);
    if (HobPtr.Guid != NULL) {
      //
      // After sending DRAM Init Done to ME FW, please do the global reset through HECI.
      //
      Status = PeiHeciSendCbmResetRequest (CBM_RR_REQ_ORIGIN_BIOS_POST, CBM_HRR_GLOBAL_RESET);
      if (EFI_ERROR (Status)) {
        //
        // Check if ME is undergoing unconfiguration. If yes, then wait for the queued message to be executed.
        // Platform should reset during function execution. If not, attempt to do CF9 reset
        //
        WaitForUnconfigFinish ();
      }
      //
      // Allow 1 second delay so CSME can perform the global reset within this period.
      //
      MicroSecondDelay (1000000);
    }
  }
  //
  // In case reset through HECI fails, continue with CF9 reset
  //
  IoWrite8 (R_PCH_IO_RST_CNT, V_PCH_IO_RST_CNT_FULLRESET);
  CpuDeadLoop ();
}


/**
  This function register reset handler ppi for ME in PEI

  @param[in]  PeiServices      Pointer to PEI Services Table.
  @param[in]  NotifyDescriptor Pointer to the descriptor for the Notification event that
                               caused this function to execute.
  @param[in]  Interface        Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others

**/
EFI_STATUS
EFIAPI
MeResetHandlerCallback (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Interface
  )
{
  EFI_STATUS                                Status;
  EDKII_PLATFORM_SPECIFIC_RESET_HANDLER_PPI *ResetHandlerPpi;
  RESET_CALLBACK_PTR_HOB                    *PtrHob;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  Status = PeiServicesLocatePpi (
             &gEdkiiPlatformSpecificResetHandlerPpiGuid,
             0,
             NULL,
             (VOID **) &ResetHandlerPpi
             );
  if (!EFI_ERROR (Status)) {
    PtrHob = BuildGuidHob (&mResetCallbackPtrGuid, sizeof (RESET_CALLBACK_PTR_HOB));
    if (PtrHob == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    PtrHob->ResetFunction = PeiMeResetHandlerCallback;
    ResetHandlerPpi->RegisterResetNotify (ResetHandlerPpi, PeiMeResetHandlerCallback);
  }

  return EFI_SUCCESS;
}

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_NOTIFY_DESCRIPTOR  mResetHandlerReady = {
  EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gEdkiiPlatformSpecificResetHandlerPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) MeResetHandlerCallback
};

/**
  This function register reset notify ppi with wdt in PEI after memory is discovered
  @param[in]  PeiServices      Pointer to PEI Services Table.
  @param[in]  NotifyDescriptor Pointer to the descriptor for the Notification event that
                               caused this function to execute.
  @param[in]  Interface        Pointer to the PPI data associated with this function.
  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/
STATIC
EFI_STATUS
EFIAPI
ResetHandlerReadyAfterPeiMigration (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Interface
  )
{
  EFI_STATUS                                      Status;
  EDKII_PLATFORM_SPECIFIC_RESET_HANDLER_PPI       *ResetHandlerPpi;
  EDKII_PLATFORM_SPECIFIC_RESET_NOTIFICATION_PPI  *ResetNotificationPpi;
  EFI_HOB_GUID_TYPE                               *GuidHob;
  RESET_CALLBACK_PTR_HOB                          *PtrHob;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  Status = PeiServicesLocatePpi (
             &gEdkiiPlatformSpecificResetHandlerPpiGuid,
             0,
             NULL,
             (VOID **) &ResetHandlerPpi
             );

  if (!EFI_ERROR(Status)) {
    //
    // After memory is discovered, we need to fixup pointer to the callback function.
    //
    GuidHob = GetFirstGuidHob (&mResetCallbackPtrGuid);
    if (GuidHob == NULL) {
      ASSERT (FALSE);
      return EFI_NOT_FOUND;
    }
    PtrHob = (RESET_CALLBACK_PTR_HOB *) GET_GUID_HOB_DATA (GuidHob);
    ResetHandlerPpi->UnregisterResetNotify (ResetHandlerPpi, PtrHob->ResetFunction);
    ResetHandlerPpi->RegisterResetNotify (ResetHandlerPpi, PeiMeResetHandlerCallback);
  }

  Status = PeiServicesLocatePpi (
             &gEdkiiPlatformSpecificResetNotificationPpiGuid,
             0,
             NULL,
             (VOID **) &ResetNotificationPpi
             );

  if (!EFI_ERROR(Status)) {
    //
    // After memory is discovered, we need to fixup pointer to the callback function.
    //
    GuidHob = GetFirstGuidHob (&mResetNotificationCallbackPtrGuid);
    if (GuidHob != NULL) {
      PtrHob = (RESET_CALLBACK_PTR_HOB *) GET_GUID_HOB_DATA (GuidHob);
      ResetNotificationPpi->UnregisterResetNotify (ResetNotificationPpi, PtrHob->ResetFunction);
      ResetNotificationPpi->RegisterResetNotify (ResetNotificationPpi, PeiMeResetHandlerCallback);
    }
  }

  return EFI_SUCCESS;
}

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_NOTIFY_DESCRIPTOR  mResetHandlerReadyAfterPeiMigration = {
  EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gEdkiiPeiMigrateTempRamPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) ResetHandlerReadyAfterPeiMigration
};

/**
  Install ME call back function for reset2 ppi.

**/
VOID
RegisterMeReset (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "%a () Start\n", __FUNCTION__));
  PeiServicesNotifyPpi (&mResetHandlerReady);
  PeiServicesNotifyPpi (&mResetHandlerReadyAfterPeiMigration);
}

