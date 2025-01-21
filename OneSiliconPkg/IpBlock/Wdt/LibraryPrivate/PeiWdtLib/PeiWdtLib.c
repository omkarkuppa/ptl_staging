/** @file
  Implementation file for Watchdog Timer functionality

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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
#include <PiPei.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/HobLib.h>
#include <Library/WdtCommonLib.h>
#include <Ppi/SiPolicy.h>
#include <Ppi/PlatformSpecificResetNotification.h>
#include <WatchDogConfig.h>
#include <Register/PmcRegs.h>

STATIC EFI_GUID mResetCallbackPtrGuid = { 0x1c9057fb, 0x1f2c, 0x4219, { 0xac, 0xf8, 0xbe, 0xa3, 0x57, 0xe4, 0x7f, 0xdf }};

typedef struct {
  EFI_RESET_SYSTEM  ResetFunction;
} RESET_CALLBACK_PTR_HOB;

VOID
EFIAPI
WdtResetNotificationCallback (
  IN EFI_RESET_TYPE     ResetType,
  IN EFI_STATUS         ResetStatus,
  IN UINTN              DataSize,
  IN VOID               *ResetData OPTIONAL
  );

/**
  This function register reset notify ppi with wdt in PEI

  @param[in]  PeiServices      Pointer to PEI Services Table.
  @param[in]  NotifyDescriptor Pointer to the descriptor for the Notification event that
                               caused this function to execute.
  @param[in]  Interface        Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others

**/
EFI_STATUS
EFIAPI
ResetNotificationReadyCallBack (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Interface
  )
{
  EFI_STATUS                      Status;
  EFI_RESET_NOTIFICATION_PROTOCOL *ResetNotify;
  RESET_CALLBACK_PTR_HOB          *PtrHob;

  Status = PeiServicesLocatePpi (
            &gEdkiiPlatformSpecificResetNotificationPpiGuid,
            0,
            NULL,
            (VOID **) &ResetNotify
            );

  if (!EFI_ERROR(Status)) {
    PtrHob = BuildGuidHob (&mResetCallbackPtrGuid, sizeof (RESET_CALLBACK_PTR_HOB));
    if (PtrHob == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    PtrHob->ResetFunction = WdtResetNotificationCallback;
    ResetNotify->RegisterResetNotify (ResetNotify, WdtResetNotificationCallback);
  }
  return EFI_SUCCESS;
}

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_NOTIFY_DESCRIPTOR  mResetNotificationReady = {
  EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gEdkiiPlatformSpecificResetNotificationPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) ResetNotificationReadyCallBack
};

/**
  This runtine clears status bits and disable watchdog, then lock the
  WDT registers.
  while WDT is designed to be disabled and locked by policy,
  bios should not enable WDT by WDT PPI. In such case, bios shows the
  warning message but not disable and lock WDT register to make sure
  WDT event trigger correctly.

  @param[in] SiPreMemPolicyPpi         The Silicon PreMem Policy PPI instance
**/
VOID
WdtDisableAndLock (
  IN  SI_PREMEM_POLICY_PPI    *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                  Status;
  PCH_WDT_PREMEM_CONFIG       *WdtPreMemConfig;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gWatchDogPreMemConfigGuid, (VOID *) &WdtPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  //
  // If WDT is enabled and DisableAndLock is set, then proceed with the lock
  //
  if (WdtPreMemConfig->DisableAndLock) {
    DEBUG ((DEBUG_INFO, "(WDT) Disable and Lock by policy request.\n"));
    //
    // clear status bits and disable watchdog, then lock the register
    //
    WdtDisable ();
    IoWrite32 (WdtGetAddress (), (B_ACPI_IO_OC_WDT_CTL_OC_WDT_ICCSURV_STS | B_ACPI_IO_OC_WDT_CTL_OC_WDT_NO_ICCSURV_STS));
    IoWrite32 (WdtGetAddress (), B_ACPI_IO_OC_WDT_CTL_OC_WDT_CTL_LCK);
  }
}

/**
  Disable WDT when S3 resume before OS.
**/
VOID
WdtDisableBeforeS3Resume (
  VOID
  )
{
  EFI_STATUS    Status;
  EFI_BOOT_MODE BootMode;

  DEBUG ((DEBUG_INFO, "(WDT) WdtDisableBeforeS3Resume\n"));

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  //
  // We need to disable WDT on S3 resume because we cannot assume the OS WDT driver is
  // available. Also, if the system is resuming from S3, it is assumed the previous boot was stable.
  //
  if (BootMode == BOOT_ON_S3_RESUME) {
    WdtDisable ();
  }
}

/**
  WDT call back function for Reset notification.

  @param[in] ResetType            UEFI defined reset type.
  @param[in] ResetStatus          The status code for the reset.
  @param[in] DataSize             The size of ResetData in bytes.
  @param[in] ResetData            Optional element used to introduce a platform specific reset.
                                  The exact type of the reset is defined by the EFI_GUID that follows
                                  the Null-terminated Unicode string.
**/
VOID
EFIAPI
WdtResetNotificationCallback (
  IN EFI_RESET_TYPE     ResetType,
  IN EFI_STATUS         ResetStatus,
  IN UINTN              DataSize,
  IN VOID               *ResetData OPTIONAL
  )
{
  WdtAllowKnownReset ();
}

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
ResetHandlerRegisterAfterMemoryDiscovered (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Interface
  )
{
  EFI_STATUS                      Status;
  EFI_RESET_NOTIFICATION_PROTOCOL *ResetNotify;
  EFI_HOB_GUID_TYPE               *GuidHob;
  RESET_CALLBACK_PTR_HOB          *PtrHob;

  Status = PeiServicesLocatePpi (
            &gEdkiiPlatformSpecificResetNotificationPpiGuid,
            0,
            NULL,
            (VOID **) &ResetNotify
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
    ResetNotify->UnregisterResetNotify (ResetNotify, PtrHob->ResetFunction);
    ResetNotify->RegisterResetNotify (ResetNotify, WdtResetNotificationCallback);
  }
  return EFI_SUCCESS;
}

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_NOTIFY_DESCRIPTOR  mResetNotificationReadyAfterMemoryDiscovered = {
  EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gEfiPeiMemoryDiscoveredPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) ResetHandlerRegisterAfterMemoryDiscovered
};

/**
  Install PchResetCallback for WDT.
**/
VOID
WdtInstallResetCallback (
  VOID
  )
{
  UINT32      Readback;

  Readback = IoRead32 (WdtGetAddress ());

  DEBUG ((DEBUG_INFO, "(WDT) Readback = 0x%08x\n", Readback));

  PeiServicesNotifyPpi (&mResetNotificationReady);

}
