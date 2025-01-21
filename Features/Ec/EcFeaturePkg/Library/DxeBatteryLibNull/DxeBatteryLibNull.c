/** @file
  Null Dxe Battery library instance with empty functions.

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
#include <Library/DxeBatteryLib.h>

/**
  Check if the real battery is present.

  @param[out]   IsRealBat                 Return the real battery present status.

  @retval       EFI_SUCCESS               Check real battery status thru EC successfully.
  @retval       Others                    Failed - EFI_TIMEOUT, EFI_INVALID_PARAMETER, EFI_UNSUPPORTED,
                                                   EFI_BUFFER_TOO_SMALL, EFI_DEVICE_ERROR, etc.
**/
EFI_STATUS
EFIAPI
CheckBatPresent (
  OUT BOOLEAN       *IsRealBat
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Read 16bit data and format them to 32bit from EC Memory from location pointed by EC offset to match the battery 32bit data requirement.

  @param[in]    EcOffset                  EcOffset use to communicate with EC RAM.
  @param[out]   Data32                    Return the data with 32 bits.

  @retval       EFI_SUCCESS               Read Data from EC Memory thru EC successfully.
  @retval       Others                    Failed - EFI_TIMEOUT, EFI_INVALID_PARAMETER, EFI_UNSUPPORTED,
                                                  EFI_BUFFER_TOO_SMALL, EFI_DEVICE_ERROR, etc.
**/
EFI_STATUS
ReadEcBatData32 (
  IN  UINT8           EcOffset,
  OUT UINT32          *Data32
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Get batter designed capacity value.

  @param[out]   DesignedCapacity          Return battery designed capacity value.

  @retval       EFI_SUCCESS               Get battery designed capacity value thru EC successfully.
  @retval       Others                    Failed - EFI_TIMEOUT, EFI_INVALID_PARAMETER, EFI_UNSUPPORTED,
                                                   EFI_BUFFER_TOO_SMALL, EFI_DEVICE_ERROR, etc.
**/
EFI_STATUS
EFIAPI
GetDesignCapacity (
  OUT UINT32          *DesignedCapacity
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Get battery last full charge capacity value.

  @param[out]   LastFullChargeCapacity    Return battery last full charge capacity value.

  @retval       EFI_SUCCESS               Get battery last full charge capacity value thru EC successfully.
  @retval       Others                    Failed - EFI_TIMEOUT, EFI_INVALID_PARAMETER, EFI_UNSUPPORTED,
                                                   EFI_BUFFER_TOO_SMALL, EFI_DEVICE_ERROR, etc.
**/
EFI_STATUS
EFIAPI
GetLastFullChargeCapacity (
  OUT UINT32          *LastFullChargeCapacity
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Get battery technology value.

  @param[out]   BatteryTechnology         Return battery technology value.

  @retval       EFI_SUCCESS               Get battery technology value successfully.
  @retval       Others                    Failed - EFI_TIMEOUT, EFI_INVALID_PARAMETER, EFI_UNSUPPORTED,
                                                   EFI_BUFFER_TOO_SMALL, EFI_DEVICE_ERROR, etc.
**/
EFI_STATUS
EFIAPI
GetBatteryTechnology (
  OUT UINT32          *BatteryTechnology
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Get design voltage value.

  @param[out]   DesignVoltage             Return battery design voltage value.

  @retval       EFI_SUCCESS               Get battery design voltage value thru EC successfully.
  @retval       Others                    Failed - EFI_TIMEOUT, EFI_INVALID_PARAMETER, EFI_UNSUPPORTED,
                                                   EFI_BUFFER_TOO_SMALL, EFI_DEVICE_ERROR, etc.
**/
EFI_STATUS
EFIAPI
GetDesignVoltage (
  OUT UINT32          *DesignVoltage
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Get design capacity of warning value.

  @param[out]   DesignCapacityOfWarning   Return design capacity of warning value.

  @retval       EFI_SUCCESS               Get design capacity of warning value successfully.
  @retval       Others                    Failed - EFI_TIMEOUT, EFI_INVALID_PARAMETER, EFI_UNSUPPORTED,
                                                   EFI_BUFFER_TOO_SMALL, EFI_DEVICE_ERROR, etc.
**/
EFI_STATUS
EFIAPI
GetDesignCapacityOfWarning (
  OUT UINT32          *DesignCapacityOfWarning
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Get design capacity of low value.

  @param[out]   DesignCapacityOfLow       Return design capacity of low value.

  @retval       EFI_SUCCESS               Get design capacity of low value successfully.
  @retval       Others                    Failed - EFI_TIMEOUT, EFI_INVALID_PARAMETER, EFI_UNSUPPORTED,
                                                   EFI_BUFFER_TOO_SMALL, EFI_DEVICE_ERROR, etc.
**/
EFI_STATUS
EFIAPI
GetDesignCapacityOfLow (
  OUT UINT32          *DesignCapacityOfLow
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Get model number value.

  @retval       ModelNumber               Address: Get model number value successfully.
                                          NULL: Failed to get model number.
**/
CHAR8 *
EFIAPI
GetModelNumber (
  VOID
  )
{
  return NULL;
}

/**
  Get serial number value.

  @retval       SerialNumber              Address: Get serial number value successfully.
                                          NULL: Failed to get serial number.
**/
CHAR8 *
EFIAPI
GetSerialNumber (
  VOID
  )
{
  return NULL;
}

/**
  Get battery type value.

  @retval       BatteryType               Address: Get battery type value successfully.
                                          NULL: Failed to get battery type.
**/
CHAR8 *
EFIAPI
GetBatteryType (
  VOID
  )
{
  return NULL;
}

/**
  Get OEM information value.

  @retval       OEMInformation            Address: Get OEM information value successfully.
                                          NULL: Failed to get OEM information value.
**/
CHAR8 *
EFIAPI
GetOEMInformation (
  VOID
  )
{
  return NULL;
}

/**
  Get battery state value.

  @param[out]        BatState             Return battery state value.
  @retval            EFI_SUCCESS          Get battery state value thru EC successfully.
  @retval            Others               Failed - EFI_TIMEOUT, EFI_INVALID_PARAMETER, EFI_UNSUPPORTED,
                                                   EFI_BUFFER_TOO_SMALL, EFI_DEVICE_ERROR, etc.
**/
EFI_STATUS
EFIAPI
GetBatteryState (
  OUT  UINT32        *BatState
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Get battery present rate value.

  @param[out]   BatteryPresentRate        Return battery present rate value.

  @retval       EFI_SUCCESS               Get battery present rate value thru EC successfully.
  @retval       Others                    Failed - EFI_TIMEOUT, EFI_INVALID_PARAMETER, EFI_UNSUPPORTED,
                                                   EFI_BUFFER_TOO_SMALL, EFI_DEVICE_ERROR, etc.
**/
EFI_STATUS
EFIAPI
GetBatteryPresentRate (
  OUT UINT32          *BatteryPresentRate
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Get battery remaining capacity value.

  @param[out]   BatteryRemainingCapacity  Return battery remaining capacity value.

  @retval       EFI_SUCCESS               Get battery remaining capacity value thru EC successfully.
  @retval       Others                    Failed - EFI_TIMEOUT, EFI_INVALID_PARAMETER, EFI_UNSUPPORTED,
                                                   EFI_BUFFER_TOO_SMALL, EFI_DEVICE_ERROR, etc.
**/
EFI_STATUS
EFIAPI
GetBatteryRemainingCapacity (
  OUT UINT32          *BatteryRemainingCapacity
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Get battery present voltage value.

  @param[out]   BatteryPresentVoltage     Return battery present voltage value.

  @retval       EFI_SUCCESS               Get battery present voltage value thru EC successfully.
  @retval       Others                    Failed - EFI_TIMEOUT, EFI_INVALID_PARAMETER, EFI_UNSUPPORTED,
                                                   EFI_BUFFER_TOO_SMALL, EFI_DEVICE_ERROR, etc.
**/
EFI_STATUS
EFIAPI
GetBatteryPresentVoltage (
  OUT UINT32          *BatteryPresentVoltage
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Set battery enable or disable charging.

  @param[in]   EnableCharging            True - Enable charging.
                                          False - Disable charging.

  @retval       EFI_SUCCESS               Set battery enable or disable charging successfully.
  @retval       Others                    Failed - EFI_TIMEOUT, EFI_INVALID_PARAMETER, EFI_UNSUPPORTED,
                                                   EFI_BUFFER_TOO_SMALL, EFI_DEVICE_ERROR, etc.
**/
EFI_STATUS
EFIAPI
SetBatteryEnableCharging (
  IN BOOLEAN          EnableCharging
  )
{
  return EFI_UNSUPPORTED;
}
