/** @file
  Fast Boot Related HOB Creation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/PmcLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Guid/GlobalVariable.h>
#include <SetupVariable.h>
#include <FastBootFunctionEnabledHob.h>
#include <FastBootExceptionInfoHob.h>
#include <Ppi/EndOfPeiPhase.h>
#include <AttemptUsbFirst.h>

/**
  Check to see the overclocking is enabled or not

  @param[in]  VariableServices        A pointer to this instance of the EFI_PEI_READ_ONLY_VARIABLE2_PPI.

  @retval  OverclockingEnabledStatus  TRUE means overclocking is enabled
                                      FALSE means overclocking is disabled
**/
BOOLEAN
EFIAPI
IsOverclockingEnabled (
  IN EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices
  )
{
  BOOLEAN                         OverclockingEnabledStatus;

  OverclockingEnabledStatus = FALSE;
#if FixedPcdGetBool(PcdOverclockEnable) == 1
  EFI_STATUS                      Status;
  UINTN                           VarSize;
  OC_SETUP                        OcSetup;

  VarSize = sizeof (OC_SETUP);
  Status = VariableServices->GetVariable (
                                VariableServices,
                                L"OcSetup",
                                &gOcSetupVariableGuid,
                                NULL,
                                &VarSize,
                                &OcSetup
                                );
  if (Status == EFI_SUCCESS) {
    if (OcSetup.OverclockingSupport != 0) {
      OverclockingEnabledStatus = TRUE;
    }
  }
#endif
  return OverclockingEnabledStatus;
}

/**
  Check fast boot is enabled or not

  @param[in]  VariableServices    A pointer to this instance of the EFI_PEI_READ_ONLY_VARIABLE2_PPI.

  @retval  FastBootEnabledStatus  TRUE means fast boot is enabled
                                  FALSE means fast boot is disabled
**/
BOOLEAN
EFIAPI
IsFastBootEnabled (
  IN EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices
  )
{
  EFI_STATUS                      Status;
  UINTN                           VarSize;
  SETUP_DATA                      SystemConfiguration;
  BOOLEAN                         FastBootEnabledStatus;

  FastBootEnabledStatus = FALSE;

  VarSize = sizeof (SETUP_DATA);
  Status = VariableServices->GetVariable (
                                VariableServices,
                                L"Setup",
                                &gSetupVariableGuid,
                                NULL,
                                &VarSize,
                                &SystemConfiguration
                                );
  if (Status == EFI_SUCCESS) {
    if (SystemConfiguration.FastBoot != 0) {
      FastBootEnabledStatus = TRUE;
    }
  }

  return FastBootEnabledStatus;
}

/**
  Create Fast Boot HOB

  @param   PeiServices          An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation
  @param   NotifyDescriptor     Address of the notification descriptor data structure.
  @param   Ppi                  Address of the PPI that was installed.

  @retval  EFI_SUCCESS          Successfully update the Boot records.
  @retval  EFI_NOT_FOUND        The PPI descriptor is not found in the database.
**/
EFI_STATUS
CreateFastBootHobPreMem (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                       Status;
  UINTN                            VarSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *VariableServices;
  FAST_BOOT_FUNCTION_ENABLED_HOB   *FastBootFunctionEnabledHob;
  FAST_BOOT_EXCEPTION_INFO_HOB     *FastBootExceptionInfoHob;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (IsFastBootEnabled (VariableServices)) {
    ///
    /// If Fast Boot is enabled, create the FAST_BOOT_FUNCTION_ENABLED_HOB for other modules' reference.
    ///
    Status = PeiServicesCreateHob (
               EFI_HOB_TYPE_GUID_EXTENSION,
               sizeof (FAST_BOOT_FUNCTION_ENABLED_HOB),
               (VOID **) &FastBootFunctionEnabledHob
               );
    if (!EFI_ERROR (Status)) {
      FastBootFunctionEnabledHob->Header.Name     = gFastBootFunctionEnabledHobGuid;
      FastBootFunctionEnabledHob->FastBootEnabled = TRUE;
    } else {
      return Status;
    }
    ///
    /// When RTC battery is drained (RTC power loss) or Secondary NvRam content is cleared (via jumper short), this bit will get set.
    /// This is the Fast Boot Exception Type 2.
    ///
    if (!PmcIsRtcBatteryGood ()) {
      Status = PeiServicesCreateHob (
                 EFI_HOB_TYPE_GUID_EXTENSION,
                 sizeof (FAST_BOOT_EXCEPTION_INFO_HOB),
                 (VOID **) &FastBootExceptionInfoHob
                 );
      if (!EFI_ERROR (Status)) {
        FastBootExceptionInfoHob->Header.Name         = gFastBootExceptionInfoHobGuid;
        FastBootExceptionInfoHob->FbExceptionType     = ExceptionType2;
        FastBootExceptionInfoHob->FbExceptionCategory = ContentLost;
      } else {
        return Status;
      }
    }
    ///
    /// Check the EFI Globally-Defined variable which is nonexistent right after updating BIOS, to determine if BIOS was just updated.
    ///
    Status = VariableServices->GetVariable (
                                 VariableServices,
                                 L"ConOut",
                                 &gEfiGlobalVariableGuid,
                                 NULL,
                                 &VarSize,
                                 NULL
                                 );
    if (Status == EFI_NOT_FOUND) {
      Status = PeiServicesCreateHob (
                 EFI_HOB_TYPE_GUID_EXTENSION,
                 sizeof (FAST_BOOT_EXCEPTION_INFO_HOB),
                 (VOID **) &FastBootExceptionInfoHob
                 );
      if (!EFI_ERROR (Status)) {
        FastBootExceptionInfoHob->Header.Name         = gFastBootExceptionInfoHobGuid;
        FastBootExceptionInfoHob->FbExceptionType     = ExceptionType2;
        FastBootExceptionInfoHob->FbExceptionCategory = FirmwareUpdate;
      } else {
        return Status;
      }
    }
    ///
    /// If overclocking is enabled, then BIOS shall switch back to Full Boot mode
    ///
    if (IsOverclockingEnabled (VariableServices)) {
      Status = PeiServicesCreateHob (
                 EFI_HOB_TYPE_GUID_EXTENSION,
                 sizeof (FAST_BOOT_EXCEPTION_INFO_HOB),
                 (VOID **) &FastBootExceptionInfoHob
                 );
      if (!EFI_ERROR (Status)) {
        FastBootExceptionInfoHob->Header.Name         = gFastBootExceptionInfoHobGuid;
        FastBootExceptionInfoHob->FbExceptionType     = ExceptionType2;
        FastBootExceptionInfoHob->FbExceptionCategory = SpecialBoot;
      } else {
        return Status;
      }
    }
  }
  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  Create USB First Hotkey Information HOB

  @param PeiServices       An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation
  @param NotifyDescriptor  Address of the notification descriptor data structure.
  @param Ppi               Address of the PPI that was installed.

  @retval EFI_SUCCESS      Successfully update the Boot records.
**/
EFI_STATUS
CreateUsbFirstHotkeyInfoEndOfPei (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                     Status;
  ATTEMPT_USB_FIRST_HOTKEY_INFO  AttemptUsbFirstHotkeyInfo;

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  ZeroMem (
    &AttemptUsbFirstHotkeyInfo,
    sizeof (AttemptUsbFirstHotkeyInfo)
    );

  AttemptUsbFirstHotkeyInfo.RevisonId = 0;
  AttemptUsbFirstHotkeyInfo.HotkeyTriggered = FALSE;

  ///
  /// Build HOB for Attempt USB First feature
  ///
  BuildGuidDataHob (
    &gAttemptUsbFirstHotkeyInfoHobGuid,
    &(AttemptUsbFirstHotkeyInfo),
    sizeof (ATTEMPT_USB_FIRST_HOTKEY_INFO)
    );

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));

  return Status;
}

static EFI_PEI_NOTIFY_DESCRIPTOR  mSiPreMemPolicyNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gSiPreMemPolicyReadyPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) CreateFastBootHobPreMem
};

static EFI_PEI_NOTIFY_DESCRIPTOR  mEndOfPeiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) CreateUsbFirstHotkeyInfoEndOfPei
};

/**
  Main Entry Of Creation Fast Boot Related HOB

  @param[in]  FileHandle   Handle of the file being invoked.
  @param[in]  PeiServices  Pointer to PEI Services table.

  @retval EFI_SUCCESS Install function successfully.

**/
EFI_STATUS
EFIAPI
FastBootPeiEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS     Status;

  //
  // Performing SiPreMemPolicyNotify
  //
  Status = PeiServicesNotifyPpi (&mSiPreMemPolicyNotifyList);

  //
  // Performing PlatformInitEndOfPei after EndOfPei PPI produced
  //
  Status = PeiServicesNotifyPpi (&mEndOfPeiNotifyList);

  return Status;
}
