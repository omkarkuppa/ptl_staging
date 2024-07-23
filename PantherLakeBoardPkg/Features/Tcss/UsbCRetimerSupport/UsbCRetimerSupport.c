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
#include <Library/UefiBootServicesTableLib.h>
#include <Library/GpioV2WrapperLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <PlatformBoardConfig.h>
#include <IntelRcStatusCode.h>

GLOBAL_REMOVE_IF_UNREFERENCED USBC_PROGRESS_CODE_PROTOCOL  mUsbCProgressCodeProtocol;

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
  USBC_RETIMER_PROTOCOL  *UsbCRetimerProtocol;

  DEBUG ((DEBUG_INFO, "%a: Start\n", __FUNCTION__));

  Status = gBS->LocateProtocol (&gUsbCRetimerProtocolGuid, NULL, (VOID**) &UsbCRetimerProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to locate UsbCRetimerProtocol (%r).\n", __FUNCTION__, Status));
    return Status;
  }

  UsbCRetimerProtocol->RetimerFP = ControlRetimerForcePowerGpio;

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