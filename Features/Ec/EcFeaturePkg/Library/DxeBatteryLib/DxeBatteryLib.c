/** @file
  DXE Battery library implementation.

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
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/EcMiscLib.h>
#include <Library/EcPrivateLib.h>
#include <Library/DxeBatteryLib.h>
#include <Protocol/EcNvsArea.h>
#include <EcCommands.h>
#include <EcRamOffset.h>
#include <BatteryData.h>

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
  EFI_STATUS        Status;
  UINT8             BatStatus[1];

  if (IsRealBat == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  BatStatus[0]   = BATTERY_1_STATUS;
  *IsRealBat     = FALSE;

  Status = ReadEcRam (EcId0Ch0, BatStatus, sizeof (BatStatus));
  if (EFI_ERROR (Status)) {
    if (Status == EFI_TIMEOUT) {
      Status = EFI_NOT_READY;
    }
    return Status;
  }
  *IsRealBat = (BatStatus[0] & BIT3) ? TRUE : FALSE;

  return EFI_SUCCESS;
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
  EFI_STATUS          Status;
  UINT8               Data8[1];

  if (Data32 == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Data8[0]  = EcOffset + 1;

  Status = ReadEcRam (EcId0Ch0, Data8, sizeof (Data8));
  *Data32 |= (UINT32)Data8[0];
  *Data32 <<= 8;
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Data8[0]  = EcOffset;
  Status = ReadEcRam (EcId0Ch0, Data8, sizeof (Data8));
  if (EFI_ERROR (Status)) {
    return Status;
  }
  *Data32 |= (UINT32)Data8[0];

  return EFI_SUCCESS;
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
  EFI_STATUS          Status;

  Status = ReadEcBatData32 (BATTERY_1_DESIGN_CAPACITY, DesignedCapacity);
  if (EFI_ERROR (Status)) {
    if (Status == EFI_TIMEOUT) {
      Status = EFI_NOT_READY;
    }
    return Status;
  }
  return EFI_SUCCESS;
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
  EFI_STATUS          Status;

  Status = ReadEcBatData32 (BATTERY_1_FULL_CHARGE_CAPACITY, LastFullChargeCapacity);
  if (EFI_ERROR (Status)) {
    if (Status == EFI_TIMEOUT) {
      Status = EFI_NOT_READY;
    }
    return Status;
  }
  return EFI_SUCCESS;
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
  EFI_STATUS          Status;
  BOOLEAN             IsRealBat;

  if (BatteryTechnology == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  IsRealBat           = FALSE;

  Status = CheckBatPresent (&IsRealBat);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (IsRealBat) {
    *BatteryTechnology = PHYSICAL_BATTERY_TECHNOLOGY;
  } else {
    *BatteryTechnology = VIRTUAL_BATTERY_TECHNOLOGY;
  }
  return EFI_SUCCESS;
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
  EFI_STATUS          Status;

  Status = ReadEcBatData32 (BATTERY_1_DESIGN_VOLTAGE, DesignVoltage);
  if (EFI_ERROR (Status)) {
    if (Status == EFI_TIMEOUT) {
      Status = EFI_NOT_READY;
    }
    return Status;
  }
  return EFI_SUCCESS;
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
  EFI_STATUS          Status;
  BOOLEAN             IsRealBat;
  UINT32              LastFullChargeCapacity;

  if (DesignCapacityOfWarning == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  IsRealBat               = FALSE;
  LastFullChargeCapacity  = 0;

  Status = CheckBatPresent (&IsRealBat);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (IsRealBat) {
    Status = GetLastFullChargeCapacity (&LastFullChargeCapacity);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    *DesignCapacityOfWarning = LastFullChargeCapacity / 10;
  } else {
    *DesignCapacityOfWarning = VIRTUAL_DESIGN_CAPACITY_OF_WARNING;
  }

  return EFI_SUCCESS;
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
  EFI_STATUS          Status;
  BOOLEAN             IsRealBat;
  UINT32              LastFullChargeCapacity;

  if (DesignCapacityOfLow == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  IsRealBat               = FALSE;
  LastFullChargeCapacity  = 0;

  Status = CheckBatPresent (&IsRealBat);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (IsRealBat) {
    Status = GetLastFullChargeCapacity (&LastFullChargeCapacity);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    *DesignCapacityOfLow = LastFullChargeCapacity / 25;
  } else {
    *DesignCapacityOfLow = VIRTUAL_DESIGN_CAPACITY_OF_LOW;
  }

  return EFI_SUCCESS;
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
  EFI_STATUS          Status;
  BOOLEAN             IsRealBat;

  IsRealBat           = FALSE;

  Status = CheckBatPresent (&IsRealBat);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  if (IsRealBat) {
    return (CHAR8 *) AllocateCopyPool (sizeof (PHYSICAL_MODEL_NUMBER), PHYSICAL_MODEL_NUMBER);
  } else {
    return (CHAR8 *) AllocateCopyPool (sizeof (VIRTUAL_MODEL_NUMBER), VIRTUAL_MODEL_NUMBER);
  }
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
  EFI_STATUS          Status;
  BOOLEAN             IsRealBat;

  IsRealBat           = FALSE;

  Status = CheckBatPresent (&IsRealBat);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  if (IsRealBat) {
    return (CHAR8 *) AllocateCopyPool (sizeof (PHYSICAL_SERIAL_NUMBER), PHYSICAL_SERIAL_NUMBER);
  } else {
    return (CHAR8 *) AllocateCopyPool (sizeof (VIRTUAL_SERIAL_NUMBER), VIRTUAL_SERIAL_NUMBER);
  }
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
  EFI_STATUS          Status;
  BOOLEAN             IsRealBat;

  IsRealBat           = FALSE;

  Status = CheckBatPresent (&IsRealBat);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  if (IsRealBat) {
    return (CHAR8 *) AllocateCopyPool (sizeof (PHYSICAL_BATTERY_TYPE), PHYSICAL_BATTERY_TYPE);
  } else {
    return (CHAR8 *) AllocateCopyPool (sizeof (VIRTUAL_BATTERY_TYPE), VIRTUAL_BATTERY_TYPE);
  }
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
  EFI_STATUS          Status;
  BOOLEAN             IsRealBat;

  IsRealBat           = FALSE;

  Status = CheckBatPresent (&IsRealBat);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  if (IsRealBat) {
    return (CHAR8 *) AllocateCopyPool (sizeof (PHYSICAL_OEM_INFORMATION), PHYSICAL_OEM_INFORMATION);
  } else {
    return (CHAR8 *) AllocateCopyPool (sizeof (VIRTUAL_OEM_INFORMATION), VIRTUAL_OEM_INFORMATION);
  }
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
  OUT UINT32         *BatState
  )
{
  EFI_STATUS         Status;
  BOOLEAN            IsRealBat;
  BOOLEAN            IsVirtualAcPower;
  UINT8              BatStatus[1];

  if (BatState == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  IsRealBat          = FALSE;
  IsVirtualAcPower   = FALSE;
  BatStatus[0]       = 0;

  Status = CheckBatPresent (&IsRealBat);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (IsRealBat) {
    //
    // Get Battery State
    //
    BatStatus[0] = BATTERY_1_STATUS;
    Status = ReadEcRam (EcId0Ch0, BatStatus, sizeof (BatStatus));
    if (EFI_ERROR (Status)) {
      return Status;
    }
    *BatState = BatStatus[0] & BATTERY_STATE;
  } else {
    //
    // Get power charging state from EC.
    //
    BatStatus[0] = DEVICE_STATE_02;
    Status = ReadEcRam (EcId0Ch0, BatStatus, sizeof (BatStatus));
    if (Status == EFI_TIMEOUT) {
      Status = EFI_NOT_READY;
      return Status;
    }
    IsVirtualAcPower = (BatStatus[0] & BIT4) ? TRUE : FALSE;
    //
    // Battery State
    // Bit [0] 1 indicates the battery is discharging.
    // Bit [1] 1 indicates the battery is charging.
    //
    if (IsVirtualAcPower) {
      *BatState = 0x2;
    } else {
      *BatState = 0x1;
    }
  }

  return EFI_SUCCESS;
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
  EFI_STATUS          Status;

  BOOLEAN             IsRealBat;
  BOOLEAN             IsDisCharging;
  UINT8               BatStatus[1];
  UINT8               BatPreRateCmd;

  if (BatteryPresentRate == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  IsRealBat           = FALSE;
  IsDisCharging       = FALSE;
  BatStatus[0]        = 0;
  BatPreRateCmd       = 0;

  Status = CheckBatPresent (&IsRealBat);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (IsRealBat) {
    BatStatus[0] = BATTERY_1_STATUS;
    Status = ReadEcRam (EcId0Ch0, BatStatus, sizeof (BatStatus));
    if (EFI_ERROR (Status)) {
      return Status;
    }
    IsDisCharging = (BatStatus[0] & BIT0) ? TRUE : FALSE;
    //
    // Get Battery PresentRate (mAh)
    //
    if (IsDisCharging) {
      //
      // Get Battery PresentRate (mAh) when DisCharge
      //
      BatPreRateCmd = BATTERY_1_DISCHARGE_CURRENT;
    } else {
      //
      // Get Battery PresentRate (mAh) when Charge
      //
      BatPreRateCmd = BATTERY_1_CHARGE_CURRENT;
    }
    Status = ReadEcBatData32 (BatPreRateCmd, BatteryPresentRate);
    if (EFI_ERROR (Status)) {
      if (Status == EFI_TIMEOUT) {
        Status = EFI_NOT_READY;
      }
      return Status;
    }
  } else {
    *BatteryPresentRate = VIRTUAL_BATTERY_PRESENT_RATE;
  }

  return EFI_SUCCESS;
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
  EFI_STATUS          Status;
  BOOLEAN             IsRealBat;

  if (BatteryRemainingCapacity == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  IsRealBat           = FALSE;

  Status = CheckBatPresent (&IsRealBat);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (IsRealBat) {
    Status = ReadEcBatData32 (BATTERY_1_REMAINING_CAPACITY, BatteryRemainingCapacity);
    if (EFI_ERROR (Status)) {
      if (Status == EFI_TIMEOUT) {
        Status = EFI_NOT_READY;
      }
      return Status;
    }
  } else {
    *BatteryRemainingCapacity = VIRTUAL_BATTERY_REMAINING_CAPACITY;
  }

  return EFI_SUCCESS;
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
  EFI_STATUS          Status;
  BOOLEAN             IsRealBat;

  if (BatteryPresentVoltage == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  IsRealBat           = FALSE;

  Status = CheckBatPresent (&IsRealBat);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (IsRealBat) {
    Status = ReadEcBatData32 (BATTERY_1_FULL_RESOLUTION_VOLTAGE, BatteryPresentVoltage);
    if (EFI_ERROR (Status)) {
      if (Status == EFI_TIMEOUT) {
        Status = EFI_NOT_READY;
      }
      return Status;
    }
  } else {
    *BatteryPresentVoltage = VIRTUAL_BATTERY_PRESENT_VOLTAGE;
  }

  return EFI_SUCCESS;
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
