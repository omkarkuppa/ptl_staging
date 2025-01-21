/**@file
  Battery DXE driver to initialize and implement battery protocol.

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

@par Specification
**/
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DxeBatteryLib.h>
#include <Protocol/Battery.h>

/**
  Return static information about a battery.
  Recommended for manufacturing and recovery UEFI applications to obtain static information about the battery

  @param[in]  This                A pointer to the EFI_BATTERY_PROTOCOL instance.
  @param[out] BatteryInformation  Return the data of BATTERY_INFORMATION structures.

  @retval EFI_SUCCESS             The function returned successfully.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_DEVICE_ERROR        The physical device reported an error.
  @retval EFI_NOT_READY           The physical device is busy or not ready to process this request.
  @retval Others                  Failed - EFI_UNSUPPORTED, EFI_BUFFER_TOO_SMALL, etc.
**/
EFI_STATUS
EFIAPI
GetBatteryInformation (
  IN  EFI_BATTERY_PROTOCOL  *This,
  OUT BATTERY_INFORMATION   *BatteryInformation
  )
{
  EFI_STATUS                Status;

  if ((This == NULL) || (BatteryInformation == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get Designed Capacity (mAh)
  //
  Status = GetDesignCapacity (&BatteryInformation->DesignedCapacity);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Get Last FullCharge Capacity (mAh)
  //
  Status = GetLastFullChargeCapacity (&BatteryInformation->LastFullChargeCapacity);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Get Battery Technology
  //
  Status = GetBatteryTechnology (&BatteryInformation->BatteryTechnology);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Get Design Voltage (mVh)
  //
  Status = GetDesignVoltage (&BatteryInformation->DesignVoltage);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Get Design Capacity Of Warning (mAh)
  //
  Status = GetDesignCapacityOfWarning (&BatteryInformation->DesignCapacityOfWarning);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Get Design Capacity Of Low (mAh)
  //
  Status = GetDesignCapacityOfLow (&BatteryInformation->DesignCapacityOfLow);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Get Model Number
  //
  BatteryInformation->ModelNumber = GetModelNumber ();
  if (BatteryInformation->ModelNumber == NULL) {
    return EFI_DEVICE_ERROR;
  }
  //
  // Get Serial Number
  //
  BatteryInformation->SerialNumber = GetSerialNumber ();
  if (BatteryInformation->SerialNumber == NULL) {
    return EFI_DEVICE_ERROR;
  }
  //
  // Get Battery ype
  //
  BatteryInformation->BatteryType = GetBatteryType ();
  if (BatteryInformation->BatteryType == NULL) {
    return EFI_DEVICE_ERROR;
  }
  //
  // Get OEM Information
  //
  BatteryInformation->OEMInformation = GetOEMInformation ();
  if (BatteryInformation->OEMInformation == NULL) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Returns the current battery status.
  Recommended for manufacturing and recovery UEFI applications to obtain the battery current status.

  @param[in]  This                A pointer to the EFI_BATTERY_PROTOCOL instance.
  @param[out] BatteryStatus       Return the data of BATTERY_STATUS structures.

  @retval EFI_SUCCESS             The function returned successfully.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_DEVICE_ERROR        The physical device reported an error.
  @retval EFI_NOT_READY           The physical device is busy or not ready to process this request.
  @retval Others                  Failed - EFI_UNSUPPORTED, EFI_BUFFER_TOO_SMALL, etc.
**/
EFI_STATUS
EFIAPI
GetBatteryStatus (
  IN  EFI_BATTERY_PROTOCOL  *This,
  OUT BATTERY_STATUS        *BatteryStatus
  )
{
  EFI_STATUS                Status;

  if ((This == NULL) || (BatteryStatus == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Get Battery State
  //
  Status = GetBatteryState (&BatteryStatus->BatteryState);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Get Battery PresentRate (mAh)
  //
  Status = GetBatteryPresentRate (&BatteryStatus->BatteryPresentRate);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Get Battery Remaining Capacity (mAh)
  //
  Status = GetBatteryRemainingCapacity (&BatteryStatus->BatteryRemainingCapacity);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Get Battery Present Voltage (mVh)
  //
  Status = GetBatteryPresentVoltage (&BatteryStatus->BatteryPresentVoltage);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Enable or disable charging. Effective only for the current session.
  Optional for manufacturing and recovery UEFI applications to enable battery charging if it is not already enabled by the system firmware.

  @param[in] This                 A pointer to the EFI_BATTERY_PROTOCOL instance.
  @param[in] EnableCharging       True - Enable charging. False - Disable charging.

  @retval EFI_SUCCESS             The function returned successfully.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_DEVICE_ERROR        The physical device reported an error.
  @retval EFI_UNSUPPORTED         If Platform does not support controlling Charging.
**/
EFI_STATUS
EFIAPI
BatteryEnableCharging (
  IN EFI_BATTERY_PROTOCOL  *This,
  IN BOOLEAN               EnableCharging
  )
{
  EFI_STATUS               Status;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = SetBatteryEnableCharging(EnableCharging);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Entry point of the battery protocol support code and install EFI Battery protocol.

  @param[in] ImageHandle          The firmware allocated handle for the EFI image.
  @param[in] SystemTable          A pointer to the EFI System Table.

  @retval EFI_SUCCESS             Install EFI Battery protocol is executed successfully.
  @retval EFI_UNSUPPORTED         Unsupported battery.
  @retval Others                  Failed - EFI_TIMEOUT, EFI_INVALID_PARAMETER, EFI_UNSUPPORTED,
                                          EFI_BUFFER_TOO_SMALL, EFI_DEVICE_ERROR, etc.
**/
EFI_STATUS
EFIAPI
InstallBatteryProtocol (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_HANDLE             Handle;
  EFI_STATUS             Status;
  EFI_BATTERY_PROTOCOL   *BatteryProtocol;
  UINT8                  BatSupported;

  if (ImageHandle == NULL || SystemTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Handle       = NULL;
  BatSupported = PcdGet8 (PcdBatterySupport);

  if (BatSupported != 0 ) {
    BatteryProtocol = AllocateZeroPool (sizeof (EFI_BATTERY_PROTOCOL));
    if (BatteryProtocol == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
    BatteryProtocol->Revision               = EFI_BATTERY_PROTOCOL_REVISION;
    BatteryProtocol->GetBatteryInformation  = GetBatteryInformation;
    BatteryProtocol->GetBatteryStatus       = GetBatteryStatus;
    BatteryProtocol->EnableCharging         = BatteryEnableCharging;

    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gBatteryProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    BatteryProtocol
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  } else {
    Status = EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}
