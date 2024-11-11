/** @file
  Publish USBC_RETIMER_PROTOCOL.

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
  - UEFI Specification 2.6, Section 22.3

**/

#include <Protocol/UsbCRetimerProtocol.h>
#include <Protocol/UsbCProgressCodeProtocol.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/GpioV2WrapperLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <PlatformBoardConfig.h>
#include <IntelRcStatusCode.h>
#include <Library/EcTcssLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED USBC_PROGRESS_CODE_PROTOCOL  mUsbCProgressCodeProtocol;
GLOBAL_REMOVE_IF_UNREFERENCED USBC_RETIMER_PROTOCOL        mUsbCRetimerProtocol;

/**
  Show UsbC Capsule Progress Code
**/
VOID
UsbCCapsuleShowProgressCode (
  IN UINT32 ProgressCode
  )
{
  if (PcdGetBool (VpdPcdUsbCCapsuleProgressCodeEnable) == TRUE) {
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, ProgressCode);
  }
}

/**
  Get UsbC Retimer Controller info and Capability.

  @param[in]   This               The UsbC RETIMER PROTOCOL Instance.
  @param[in]   RetimerGuid        GUID from ESRT ACPI Table.

  @retval  EFI_SUCCESS            Successfully Get Info of Retimer Controller.
  @retval  EFI_INVALID_PARAMETER  Invalid GUID from ESRT Table is Passed.
  @retval  EFI_NOT_READY          Board Retimer PCD is not ready or Not Available.
  @retval  EFI_UNSUPPORTED        This driver does not support.

**/
EFI_STATUS
EFIAPI
GetRetimerInfo (
  IN USBC_RETIMER_PROTOCOL  *This,
  IN EFI_GUID               RetimerGuid  ///< GUID from ESRT ACPI Table
  )
{
  return EFI_UNSUPPORTED;
}

/**
  This event will be registered on Exit Boot Services Event to remove
  Protocol Service to make sure this protocol will not be used by any
  UEFI Application running in EDK Shell.

  @param[in]  Event    The Event that is being processed.
  @param[in]  Context  The Event Context.

**/
VOID
EFIAPI
UsbCRetimerSupportExitBootServiceEvent (
  IN  EFI_EVENT  Event,
  IN  VOID       *Context
  )
{
  USBC_RETIMER_PROTOCOL_CAPABILITY  RetimerProtocolCapability;
  RetimerProtocolCapability.RetimerSupport = FALSE;

  gRT->SetVariable (
         USBC_RETIMER_PROTOCOL_CAPABILITY_NAME,
         &gUsbCRetimerProtocolCapabilityGuid,
         EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
         sizeof (USBC_RETIMER_PROTOCOL_CAPABILITY),
         &RetimerProtocolCapability
         );

  //
  // Only one successful install
  //
  gBS->CloseEvent (Event);
}

/**
  Control Retimer FP GPIO by given output state

  @param[in] OutputState          RETIMER_FORCE_POWER_GPIO_LOW
                                  RETIMER_FORCE_POWER_GPIO_HIGH

  @retval  EFI_SUCCESS            Successfully assert/de-assert retimer FP GPIO
  @retval  EFI_INVALID_PARAMETER  Invalid group or pad number

**/
EFI_STATUS
EFIAPI
ControlRetimerForcePowerGpio (
  IN  UINT8  OutputState
  )
{
  GPIOV2_PAD    GpioPad;
  VPD_GPIO_PAD  *VpdGpio;
  EFI_STATUS    Status;

  VpdGpio = PcdGetPtr (VpdPcdBoardRetimerForcePwrGpio);
  GpioPad = VpdGpio->GpioPad;
  if (GpioPad == GPIOV2_PAD_NONE) {
    Status = EFI_DEVICE_ERROR;
    DEBUG ((DEBUG_ERROR, "\nUSBC_RETIMER_PROTOCOL: Retimer GPIO is not support = %r\n", Status));
    return Status;
  }

  if (OutputState == RETIMER_FORCE_POWER_GPIO_LOW) {
    Status = GpioV2SetTx (GpioPad, GpioV2StateLow);
  } else if (OutputState == RETIMER_FORCE_POWER_GPIO_HIGH) {
    Status = GpioV2SetTx (GpioPad, GpioV2StateHigh);
  } else {
    Status = EFI_INVALID_PARAMETER;
    DEBUG ((DEBUG_ERROR, "\nUSBC_RETIMER_PROTOCOL: OutputState is not correct = %r\n", Status));
  }

  return Status;
}

/**
  UsbCRetimerSupport DXE module entry point.

  @param[in]  ImageHandle  Not used.
  @param[in]  SystemTable  Global system service table.

  @retval  EFI_SUCCESS           Initialization complete.
  @retval  EFI_UNSUPPORTED       The chipset is unsupported by this driver.
  @retval  EFI_OUT_OF_RESOURCES  Do not have enough resources to initialize the driver.
  @retval  EFI_DEVICE_ERROR      Device error, driver exits abnormally.

**/
EFI_STATUS
EFIAPI
UsbCRetimerSupportEntryPoint (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS             Status;
  EFI_EVENT              Event;

  DEBUG ((DEBUG_INFO, "%a: Start\n", __FUNCTION__));

  //
  // Initializing USBC_RETIMER_PROTOCOL
  //
  mUsbCRetimerProtocol.GetPdControllerMode = GetPdControllerMode;
  mUsbCRetimerProtocol.SetPdControllerMode = SetPdControllerMode;
  mUsbCRetimerProtocol.RetimerFP           = ControlRetimerForcePowerGpio;

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
  // Register the ExitBootService event to remove Protocol Service.
  // This additional functionality to make sure this protocol will not be used by
  // any UEFI Application running in EDK Shell.
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  UsbCRetimerSupportExitBootServiceEvent,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &Event
                  );
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, "%a: UsbCRetimerSupportCloseEvent is Register with Status =%r\n", __FUNCTION__, Status));

  mUsbCProgressCodeProtocol.ShowProgressCode = UsbCCapsuleShowProgressCode;
  //
  // Install UsbC Capsule Debug Progress Code Protocol
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gUsbCCapsuleDebugProgressCodeProtocolGuid,
                  &mUsbCProgressCodeProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to Install UsbC Capsule Debug Progress Code Protocol with Status = %r\n", __FUNCTION__, Status));
    return Status;
  }
  DEBUG ((DEBUG_INFO, "%a: UsbC Capsule Debug Progress Code Protocol is installed Successfully.\n", __FUNCTION__));

  DEBUG ((DEBUG_INFO, "%a: End\n", __FUNCTION__));
  return Status;
}