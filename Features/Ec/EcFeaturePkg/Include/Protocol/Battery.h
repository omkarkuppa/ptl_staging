/** @file
  This file defines the Battery Protocol.

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

#ifndef _BATTERY_H_
#define _BATTERY_H_

#include <Uefi.h>

#define EFI_BATTERY_PROTOCOL_GUID \
  { \
    0xa94e3f49, 0x2e6b, 0x49c2, {0xbe, 0xf6, 0x00, 0x72, 0x75, 0x16, 0x1e, 0x8a } \
  }
extern EFI_GUID    gBatteryProtocolGuid;

typedef struct _EFI_BATTERY_PROTOCOL EFI_BATTERY_PROTOCOL;
typedef struct _BATTERY_INFORMATION  BATTERY_INFORMATION;
typedef struct _BATTERY_STATUS       BATTERY_STATUS;

#define EFI_BATTERY_PROTOCOL_REVISION       0x00010000

/**
  Return static information about a battery.
  Recommended for manufacturing and recovery UEFI applications to obtain static information about the battery.

  The GetBatteryInformation() function gets battery info thus EC.

  @param[in]  This                A pointer to the EFI_BATTERY_PROTOCOL instance.
  @param[out] BatteryInformation  Return the data of BATTERY_INFORMATION structures.

  @retval EFI_SUCCESS             The function returned successfully.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_DEVICE_ERROR        The physical device reported an error.
  @retval EFI_NOT_READY           The physical device is busy or not ready to process this request.
  @retval Others                  Failed - EFI_UNSUPPORTED, EFI_BUFFER_TOO_SMALL, etc.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_BATTERY_GET_BATTERY_INFORMATION) (
  IN  EFI_BATTERY_PROTOCOL    *This,
  OUT BATTERY_INFORMATION     *BatteryInformation
  );

typedef struct _BATTERY_INFORMATION {
  //
  // Battery's design capacity - the nominal capacity of a new battery - expressed as milliamp-hour [mAh].
  // - 0x00000000 - 0x7ffffffff in [mAh]
  // - 0xFFFFFFFF - Unknown design capacity.
  //
  UINT32  DesignedCapacity;
  //
  // Predicted battery capacity when fully charged, expressed as milliamp-hour [mAh].
  // - 0x00000000 - 0x7ffffffff in [mAh]
  // - 0xFFFFFFFF - Unknown design capacity.
  //
  UINT32  LastFullChargeCapacity;
  //
  // 0x00000000 - Primary, 0x00000001 - Secondary.
  //
  UINT32  BatteryTechnology;
  //
  // Nominal voltage of a new battery.
  // - 0x00000000 - 0x7ffffffff in [mV]
  // - 0xFFFFFFFF - Unknown design voltage.
  //
  UINT32  DesignVoltage;
  //
  // OEM-designed battery warning capacity.
  // - 0x00000000 - 0x7ffffffff in [mAh]
  //
  UINT32  DesignCapacityOfWarning;
  //
  // OEM-designed low battery capacity.
  // - 0x00000000 - 0x7ffffffff in [mAh]
  //
  UINT32  DesignCapacityOfLow;
  //
  // A pointer to OEM-specific Control Method Battery model number in ASCIIZ.
  //
  CHAR8   *ModelNumber;
  //
  // A pointer to OEM-specific Control Method Battery serial number in ASCIIZ.
  //
  CHAR8   *SerialNumber;
  //
  // A pointer to OEM-specific Control Method Battery type in ASCIIZ.
  //
  CHAR8   *BatteryType;
  //
  // A pointer to OEM-specific information for the battery. If it is not supported, this field should be 0.
  //
  CHAR8   *OEMInformation;
} BATTERY_INFORMATION;

/**
  Returns the current battery status.
  Recommended for manufacturing and recovery UEFI applications to obtain the battery current status.

  The GetBatteryStatus() function gets battery status thus EC.

  @param[in]  This                A pointer to the EFI_BATTERY_PROTOCOL instance.
  @param[out] BatteryStatus       Return the data of BATTERY_STATUS structures.

  @retval EFI_SUCCESS             The function returned successfully.
  @retval EFI_INVALID_PARAMETER   Invalid parameter.
  @retval EFI_DEVICE_ERROR        The physical device reported an error.
  @retval EFI_NOT_READY           The physical device is busy or not ready to process this request.
  @retval Others                  Failed - EFI_UNSUPPORTED, EFI_BUFFER_TOO_SMALL, etc.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_BATTERY_GET_BATTERY_STATUS) (
  IN  EFI_BATTERY_PROTOCOL *This,
  OUT BATTERY_STATUS       *BatteryStatus
  );

typedef struct _BATTERY_STATUS {
  //
  // Bit values. The Charging and the Discharging bit are mutually exclusive and must not both be set at the same time.
  // - Bit [0] - 1 indicates the battery is discharging.
  // - Bit [1] - 1 indicates the battery is charging.
  // - Bit [2] - 1 indicates the battery is in the critical energy state.
  // - Bit [3:31] - Reserved.
  //
  UINT32  BatteryState;
  //
  // The current being supplied or accepted through the battery's terminals (direction depends on the Battery State value).
  // - 0x00000000 - 0x7ffffffff in [mA]
  // - 0xFFFFFFFF - Unknown rate.
  //
  UINT32  BatteryPresentRate;
  //
  // The estimated remaining battery capacity expressed as current [mAh]. Batteries that are rechargeable are required to return a valid Battery Remaining Capacity value.
  // - 0x00000000 - 0x7ffffffff in[mAh]
  // - 0xFFFFFFFF - Unknown capacity.
  //
  UINT32  BatteryRemainingCapacity;
  //
  // The voltage across the battery's terminals. Batteries that are rechargeable must report Battery Present Voltage. Only a primary battery can report unknown voltage.
  // - 0x00000000 - 0x7ffffffff in[mV]
  // - 0xFFFFFFFF - Unknown voltage.
  //
  UINT32  BatteryPresentVoltage;
} BATTERY_STATUS;

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
typedef
EFI_STATUS
(EFIAPI *EFI_BATTERY_ENABLE_CHARGING) (
  IN EFI_BATTERY_PROTOCOL  *This,
  IN BOOLEAN               EnableCharging
  );

//
//  EFI_BATTERY_PROTOCOL provides control over a single battery subsystem, or an aggregate view of the primary/secondary battery subsystems.
//
struct _EFI_BATTERY_PROTOCOL {
  //
  // Return the protocol revision.
  //
  UINT32                                Revision;
  //
  // Return static information about a battery.
  //
  EFI_BATTERY_GET_BATTERY_INFORMATION   GetBatteryInformation;
  //
  // Returns the current battery status.
  //
  EFI_BATTERY_GET_BATTERY_STATUS        GetBatteryStatus;
  //
  // Enable or disable charging. Effective only for the current session.
  //
  EFI_BATTERY_ENABLE_CHARGING           EnableCharging;
};

#endif
