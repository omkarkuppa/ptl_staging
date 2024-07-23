/** @file
  The implementation about ME ReadyToBootEvent and EndOfFirmwareEvent for FSP API support.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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

#include <Guid/EventGroup.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/FspErrorInfoLib.h>
#include <Library/HobLib.h>
#include <Library/HeciInitLib.h>
#include <Library/MeChipsetLib.h>
#include <Library/MeInitLib.h>
#include <Library/PeiMeLib.h>
#include <Library/PmcPrivateLib.h>
#include <Ppi/Wdt.h>
#include <MeState.h>
#include <PchResetPlatformSpecific.h>

EFI_STATUS
EFIAPI
MeReadyToBootEventApi (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  );

EFI_STATUS
EFIAPI
MeEndOfFirmwareEventApi (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  );

STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mMeReadyToBootNotifyList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK  | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiEventReadyToBootGuid,
    MeReadyToBootEventApi
  }
};

STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mMeEndOfFirmwareNotifyList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK  | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gFspEventEndOfFirmwareGuid,
    MeEndOfFirmwareEventApi
  }
};

/**
  The notification registered as a callback to perform ME event prior to boot for API mode.

  @param[in]  PeiServices         An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation
  @param[in]  NotifyDescriptor    Address of the notification descriptor data structure.
  @param[in]  Ppi                 Address of the PPI that was installed.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    No enough buffer to allocate
**/
EFI_STATUS
EFIAPI
MeReadyToBootEventApi (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  )
{
  EFI_STATUS                      Status;
  WDT_PPI                         *Wdt;
  UINT32                          RequestedActions;
  PCH_RESET_DATA                  ResetData;
  ME_BIOS_BOOT_PATH               MeBiosPath;
  BOOLEAN                         MeDisable;
  ME_HANDLE                       *MeHandle;

  DEBUG ((DEBUG_INFO, "%a () Start\n", __FUNCTION__));

  ZeroMem (&ResetData, sizeof (PCH_RESET_DATA));
  Wdt        = NULL;
  MeHandle   = NULL;
  MeDisable  = FALSE;
  MeBiosPath = CheckMeBootPath ();
  switch (MeBiosPath) {
    case SecoverJmprPath:
    case SecoverMeiMsgPath:
    case SwTempDisablePath:
    case DebugModePath:
      ///
      /// If ME is running in those ME-BIOS paths - hide HECI1 prior to boot
      ///
      MeDisable = TRUE;
      break;

    case NormalPath:
    case RecoveryPath:
    case ErrorPath:
      ///
      /// Send EoP message - this is crucial for platform security
      ///
      if (GetBootModeHob () == BOOT_ON_S3_RESUME) {
        break;
      }
      if (MeEndOfPostEnabled ()) {
        RequestedActions = HeciEopStatusSuccess;
        Status = MeEndOfPostEvent (&RequestedActions);
        MeSetEndOfPostDone ();
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "MeEndOfPostEvent failed with Status = %r\n", Status));
          SendFspErrorInfo (gMeFspErrorTypeCallerId, gMeFspErrorTypeEop, EfiStatusToUint32 (Status));
          MeDisable = TRUE;
        } else if (RequestedActions == HeciEopPerformGlobalReset) {
          ///
          /// Locate WDT PPI for access to Wdt->AllowKnownReset()
          ///
          Status = PeiServicesLocatePpi (&gWdtPpiGuid, 0, NULL, (VOID **) &Wdt);
          if (!EFI_ERROR (Status)) {
            Wdt->AllowKnownReset ();
          }
          DEBUG ((DEBUG_INFO, "%a - Global Reset requested by FW EOP ACK %r\n", __FUNCTION__, Status));
          CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
          StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
          (*GetPeiServicesTablePointer ())->ResetSystem2 (EfiResetPlatformSpecific, EFI_SUCCESS, sizeof (PCH_RESET_DATA), &ResetData);
        }
      }
      break;
    default:
      ///
      /// In other ME-BIOS paths HECI1 has already been disabled in PEI Post Mem
      ///
      break;
  }

  ///
  /// PMIR Configuration & FDSW Lockdown
  ///
  LockConfig ();

  if (((MeBiosPath == SwTempDisablePath) || (MeBiosPath == SecoverMeiMsgPath)) &&
      MeIsHeci1ForceEnabled ()) {
    MeDisable = FALSE;
  }

  /// If ME policy is set to disabled, then hide MEI1 prior to boot
  ///
  if (MeDisable) {
    SetD0I3Bit (HECI1);
  }

  Status = GetMeHandle (&MeHandle);
  if (!EFI_ERROR (Status)) {
    PmcDisableMeDevices (MeHandle->PmcPwrmBase, MeDisable);
  }

  return EFI_SUCCESS;
}

/**
  The notification registered as a callback to perform ME EndOfFirmware event prior to boot for API mode.

  @param[in]  PeiServices         An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation
  @param[in]  NotifyDescriptor    Address of the notification descriptor data structure.
  @param[in]  Ppi                 Address of the PPI that was installed.

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
EFIAPI
MeEndOfFirmwareEventApi (
  IN  EFI_PEI_SERVICES            **PeiServices,
  IN  EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN  VOID                        *Ppi
  )
{
  UINT8       MeDevFunction;

  DEBUG ((DEBUG_INFO, "%a () Start\n", __FUNCTION__));

  ///
  /// Don't set D0I3 if disabled by policy
  ///
  if (MeHeciD0I3Enabled () == FALSE) {
    DEBUG ((DEBUG_INFO, "%a () - SKIP setting D0I3 bits for HECI devices on boot time\n", __FUNCTION__));
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "%a () - setting D0I3 bits for HECI devices on boot time\n", __FUNCTION__));

  //
  // Set D0i3 bit
  //
  for (MeDevFunction = HECI1; MeDevFunction <= HECI4; MeDevFunction++) {
    if (!IsHeciDeviceFunction (MeDevFunction)) {
      continue;
    }
    SetD0I3Bit (MeDevFunction);
  }

  return EFI_SUCCESS;
}

/**
  HeciInit Module Entry Point for signal ReadyToBoot and EndOfFirmware FSP API events.

  @param[in] FileHandle           PEIM's file handle
  @param[in] PeiServices          An indirect pointer to the EFI_PEI_SERVICES table published by
                                  the PEI Foundation

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
HeciInitApiEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "%a () Start\n", __FUNCTION__));

  Status = PeiServicesNotifyPpi (mMeReadyToBootNotifyList);
  ASSERT_EFI_ERROR (Status);

  Status = PeiServicesNotifyPpi (mMeEndOfFirmwareNotifyList);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "%a () End\n", __FUNCTION__));
  return Status;
}
