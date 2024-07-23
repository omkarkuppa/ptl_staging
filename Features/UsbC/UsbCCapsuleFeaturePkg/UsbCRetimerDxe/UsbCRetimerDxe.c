/** @file
  Publish USBC_RETIMER_PROTOCOL.

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
  - UEFI Specification 2.6, Section 22.3

**/

#include <Protocol/UsbCRetimerProtocol.h>
#include <EcCommon.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UsbcCapsuleDebugLib.h>
#include <UsbCCapsuleDebug/UsbCCapsuleDebugProtocol.h>
#include <UsbCCapsuleDebug/UsbCCapsuleLogEvents.h>

// Global Variable Defination
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN    mUsbCRetimerSupported     = FALSE;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8      gTotalCountOfPdController = 0; // For storing Total number of PD controller available on Board.
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN    mInitDoneFlag             = FALSE;
GLOBAL_REMOVE_IF_UNREFERENCED USBC_RETIMER_PROTOCOL             mUsbCRetimerProtocol;

/**
  Change Tbt Retimer PD controller Fw Update Mode.

  @param[in]  PdControllerMode    Mode of PD Controller to SET.
  @param[in]  PdControllerNumber  Number or Index of PD Controller.

  @retval  EFI_SUCCESS            Successfully Mode is Changed to ALT TBT Mode.
  @retval  EFI_INVALID_PARAMETER  Invalid GUID from ESRT Table is Passed.
  @retval  EFI_UNSUPPORTED        This driver does not support.
  @retval  EFI_DEVICE_ERROR       This driver cannot be started due to device Error.
  @retval  EFI_ALREADY_STARTED    This driver has been started. This is applicable for PD FwUpdate Mode Entry.
  @retval  EFI_NOT_STARTED        This driver has not been started. This is applicable for PD FwUpdate Mode Exit.
  @retval  EFI_TIMEOUT            Mode Change Command timeout Happen.
**/
EFI_STATUS
TbtPdRetimerFwUpdateModeChange (
  IN  UINT8  PdControllerMode
  );

/**
  Get PD controller count and Force Power GPIO count from Retimer Platform Specific Data table.

  @retval  EFI_SUCCESS    Successfully get data from Retimer data table.
  @retval  EFI_NOT_READY  No Retimer data table found.
**/
EFI_STATUS
GetValidateData (
  )
{
  gTotalCountOfPdController = PcdGet8 (PcdUsbCPdNumber);
  DEBUG ((DEBUG_INFO, "Total count of PD Controller number = %d\n", gTotalCountOfPdController));
  mInitDoneFlag = TRUE;

  return EFI_SUCCESS;
}

/**
  Get UsbC Retimer Controller info and Capability.

  @param[in]   This         The UsbC RETIMER PROTOCOL Instance.
  @param[in]   RetimerGuid  GUID from ESRT ACPI Table.

  @retval  EFI_SUCCESS            Successfully Get Info of Retimer Controller.
  @retval  EFI_INVALID_PARAMETER  Invalid GUID from ESRT Table is Passed.
  @retval  EFI_NOT_READY          Board Retimer PCD is not ready or Not Available.
  @retval  EFI_UNSUPPORTED        This driver does not support.
**/
EFI_STATUS
EFIAPI
GetRetimerInfo (
  IN USBC_RETIMER_PROTOCOL              *This,
  IN EFI_GUID                           RetimerGuid ///< GUID from ESRT ACPI Table
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Drive UsbC Retimer Controller into FW Update Mode.

  @param[in]  This         The UsbC RETIMER PROTOCOL Instance.
  @param[in]  RetimerGuid  GUID from ESRT ACPI Table.

  @retval  EFI_SUCCESS            Successfully Mode is Changed to FW Update Mode.
  @retval  EFI_INVALID_PARAMETER  Invalid GUID from ESRT Table is Passed.
  @retval  EFI_UNSUPPORTED        This driver does not support.
  @retval  EFI_DEVICE_ERROR       This driver cannot be started due to device Error.
  @retval  EFI_ALREADY_STARTED    This driver has been started.
  @retval  EFI_TIMEOUT            DriveToFwUpdateMode Command timeout Happen.
  @retval  EFI_NOT_READY          Board Retimer PCD is not ready or Not Available.
**/
EFI_STATUS
EFIAPI
DriveToFwUpdateMode (
  IN  USBC_RETIMER_PROTOCOL  *This,
  IN  EFI_GUID               RetimerGuid ///< GUID from ESRT ACPI Table
  )
{
  EFI_STATUS  Status;
  BOOLEAN     ValidGuidFound;

  ValidGuidFound     = NO_VALID_GUID_FOUND;
  Status             = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "\nUSBC_RETIMER_PROTOCOL.Drive: Start\n"));
  DEBUG ((DEBUG_INFO, "  RetimerGuid = %g\n", RetimerGuid));
  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_PD_DRIVE_START, 0, 0);

  if (mInitDoneFlag == FALSE) {
    Status = GetValidateData();
    DEBUG ((DEBUG_ERROR, "\nUSBC_RETIMER_PROTOCOL.Drive: Return with Status = %r\n", Status));

    if (EFI_ERROR(Status)) {
      return EFI_NOT_STARTED;
    }
  }

  // Check if Valid Guid is passed or Not for TBT Retimer Device.
  if (CompareGuid (&RetimerGuid, &gAllTbtRetimerDeviceGuid)) {
    ValidGuidFound = TBT_RETIMER_VALID_GUID_FOUND;
  }

  if (ValidGuidFound == NO_VALID_GUID_FOUND) {
    Status = EFI_INVALID_PARAMETER;
    DEBUG ((DEBUG_ERROR, "\nUSBC_RETIMER_PROTOCOL.Drive: Invalid Retimer Guid Passed for UsbCRetimer . Returning with Status = %r\n", Status));
    return Status;
  }

  // Check if Usb Type-C Retimer is not supported
  if (mUsbCRetimerSupported) {
    Status = EFI_UNSUPPORTED;
    DEBUG ((DEBUG_ERROR, "\nUSBC_RETIMER_PROTOCOL.Drive: Usb Type-C Retimer is not supported. Returning with Status = %r\n", Status));
    return Status;
  }

  if ((ValidGuidFound & TBT_RETIMER_VALID_GUID_FOUND) == TBT_RETIMER_VALID_GUID_FOUND) {
    // Change the PD Controller Mode
    Status = TbtPdRetimerFwUpdateModeChange (RetimerFirmWareUpdateEnableMode);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "\nUSBC_RETIMER_PROTOCOL.Drive: Failed to Enable Retimer FW Update Mode with Status = %r\n", Status));
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_CHANGE_MODE_FAIL, (UINT32)Status, 0);
      return Status;
    }
  }

  DEBUG ((DEBUG_INFO, "\nUSBC_RETIMER_PROTOCOL.Drive: End\n"));
  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_PD_DRIVE_END, 0, 0);
  return Status;
}

/**
  Restore TBT PD Controller into original mode.

  @param[in]  This         The UsbC RETIMER PROTOCOL Instance.
  @param[in]  RetimerGuid  GUID from ESRT ACPI Table.

  @retval  EFI_SUCCESS            Successfully Mode is Restore.
  @retval  EFI_INVALID_PARAMETER  Invalid GUID from ESRT Table is Passed.
  @retval  EFI_UNSUPPORTED        This driver does not support.
  @retval  EFI_DEVICE_ERROR       This driver cannot be started due to device Error.
  @retval  EFI_NOT_STARTED        This driver has not been started.
  @retval  EFI_TIMEOUT            RestoreToOriginalMode Command timeout Happen.
  @retval  EFI_NOT_READY          Board Retimer PCD is not ready or Not Available.
**/
EFI_STATUS
EFIAPI
RestoreToOriginalMode (
  IN USBC_RETIMER_PROTOCOL  *This,
  IN EFI_GUID               RetimerGuid ///< GUID from ESRT ACPI Table
  )
{
  EFI_STATUS  Status;
  UINT8       ValidGuidFound;

  ValidGuidFound     = NO_VALID_GUID_FOUND;
  Status             = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "\nUSBC_RETIMER_PROTOCOL.Restore: Start\n"));
  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_PD_RESTORE_START, 0, 0);

  if (mInitDoneFlag == FALSE) {
    Status = GetValidateData();
    DEBUG ((DEBUG_ERROR, "\nUSBC_RETIMER_PROTOCOL.Restore: Return with Status = %r\n", Status));
    if (EFI_ERROR(Status)) {
      return EFI_NOT_STARTED;
    }
  }

  // Check if Valid Guid is passed or Not.
  if (CompareGuid (&RetimerGuid, &gAllTbtRetimerDeviceGuid)) {
    ValidGuidFound      = TBT_RETIMER_VALID_GUID_FOUND;
  }

  if (ValidGuidFound == NO_VALID_GUID_FOUND) {
    Status = EFI_INVALID_PARAMETER;
    DEBUG ((DEBUG_ERROR, "\nUSBC_RETIMER_PROTOCOL.Restore: Invalid Retimer Guid Passed for UsbCRetimer. Returning with Status = %r\n", Status));
    return Status;
  }

  // Check if UsbCRetimer is not supported
  if (mUsbCRetimerSupported) {
    Status = EFI_UNSUPPORTED;
    DEBUG ((DEBUG_ERROR, "\nUSBC_RETIMER_PROTOCOL.Restore: Usb Type-C Retimer is not supported. Returning with Status = %r\n", Status));
    return Status;
  }

  if ((ValidGuidFound & TBT_RETIMER_VALID_GUID_FOUND) == TBT_RETIMER_VALID_GUID_FOUND) {

    // Change the PD Controller Mode
    Status = TbtPdRetimerFwUpdateModeChange (RetimerFirmWareUpdateDisableMode);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "\nUSBC_RETIMER_PROTOCOL.Restore: Failed to Exit Retimer FW Update Mode For PD Controller with Status = %r\n", Status));
      CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, USBC_RETIMER_CAPSULE_EVT_CODE_RESTORE_PD_CHANGE_MODE_FAIL, (UINT32)Status, 0);
      return Status;
    }
  }

  DEBUG ((DEBUG_INFO, "\nUSBC_RETIMER_PROTOCOL.Restore: End\n"));
  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_PD_RESTORE_END, 0, 0);
  return Status;
}

/**
  Notify function for event group EFI_EVENT_GROUP_EXIT_BOOT_SERVICES. This is used to
  remove Protocol Service support to make sure This protocol will not be used by
  any UEFI Application running in EDK Shell.

  @param[in]  Event    The Event that is being processed.
  @param[in]  Context  The Event Context.
**/
VOID
EFIAPI
UsbCRetimerSupportCloseEventNotify (
  IN  EFI_EVENT  Event,
  IN  VOID       *Context
  )
{
  mUsbCRetimerSupported = TRUE;
  //
  // Only one successful install
  //
  gBS->CloseEvent(Event);
}

/**
  UsbCRetimer DXE module entry point.

  @param[in]  ImageHandle  Not used.
  @param[in]  SystemTable  Global system service table.

  @retval  EFI_SUCCESS           Initialization complete.
  @retval  EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  @retval  EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval  EFI_DEVICE_ERROR      Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
UsbCRetimerDxeEntryPoint (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   Event;

  DEBUG ((DEBUG_INFO, "%a: Start\n", __FUNCTION__));

  InstallCapsuleDebugProtocol ();

  GetValidateData ();

  // Initializing USBC_RETIMER_PROTOCOL
  mUsbCRetimerProtocol.Drive          = DriveToFwUpdateMode;
  mUsbCRetimerProtocol.Restore        = RestoreToOriginalMode;
  mUsbCRetimerProtocol.RetimerFP      = NULL;

  //
  // Install UsbC Retimer Protocol
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gUsbCRetimerProtocolGuid,
                  &mUsbCRetimerProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to Install UsbC Retimer Protocol with Status = %r\n", __FUNCTION__, Status));
    return Status;
  }
  DEBUG ((DEBUG_INFO, "%a: UsbCRetimerProtocol is installed Successfully.\n", __FUNCTION__));
  //
  // Register notify function to Remove Protocol Service on Exit Boot Services Event.
  // This additional functionality to make sure This protocol will not be used by
  // any UEFI Application running in EDK Shell.
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  UsbCRetimerSupportCloseEventNotify,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &Event
                  );
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, "%a: UsbCRetimerSupportCloseEvent is Register with Status =%r\n", __FUNCTION__, Status));

  DEBUG ((DEBUG_INFO, "%a: End\n", __FUNCTION__));
  return Status;
}