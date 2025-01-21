/** @file
  Provides platform policy services used during a capsule update.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include <PiDxe.h>
#include <Library/CapsuleUpdatePolicyLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/EcMiscLib.h>
#include <SetupVariable.h>

/**
  Check platform battery status.

  @param[in]    RequiredCapacity       The required battery capacity.
  @param[out]   IsBatteryLow           TRUE:  Current battery capacity meets the criteria.
                                       FALSE: Current battery capacity is insufficient.
**/
VOID
EFIAPI
PlatformCheckBattery (
  IN  UINT8                  RequiredCapacity,
  OUT BOOLEAN                *IsBatteryLow
  )
{
  EFI_STATUS                           Status;
  BOOLEAN                              Bat1Present;
  BOOLEAN                              Bat2Present;
  UINT8                                Bat1Capacity;
  UINT8                                Bat2Capacity;

  DEBUG ((DEBUG_INFO, "PerformCheckBattery : RequiredCapacity is %d\n", RequiredCapacity));

  Bat1Present  = FALSE;
  Bat2Present  = FALSE;
  Bat1Capacity = 0;
  Bat2Capacity = 0;

  //
  // Battery 1 present check and read remaining percent
  //
  Status = GetBatInfo (1, &Bat1Present, &Bat1Capacity, NULL, NULL);
  DEBUG ((DEBUG_INFO, "GetBatInfo 1:%r, Present:%d, Capacity:%d\n", Status, Bat1Present, Bat1Capacity));
  if (EFI_ERROR (Status)) {
    Bat1Present = FALSE;
  }

  //
  // Battery 2 present check and read remaining percent
  //
  Status = GetBatInfo (2, &Bat2Present, &Bat2Capacity, NULL, NULL);
  DEBUG ((DEBUG_INFO, "GetBatInfo 2:%r, Present:%d, Capacity:%d\n", Status, Bat2Present, Bat2Capacity));
  if (EFI_ERROR (Status)) {
    Bat2Present = FALSE;
  }

  //
  // Batteries capacity check
  //
  if (Bat1Present && Bat2Present) {
    // Both batteries exist
    if ((Bat1Capacity < RequiredCapacity) && (Bat2Capacity < RequiredCapacity)) {
      *IsBatteryLow = TRUE;
    } else {
      *IsBatteryLow = FALSE;
    }
  } else if (Bat1Present) {
    // Battery 1 only
    *IsBatteryLow = (Bat1Capacity < RequiredCapacity) ? TRUE : FALSE;
  } else if (Bat2Present) {
    // Battery 2 only
    *IsBatteryLow = (Bat2Capacity < RequiredCapacity) ? TRUE : FALSE;
  } else {
    // No Battery
    *IsBatteryLow = TRUE;
  }

  return;
}

/**
  Determine if the system power state supports a capsule update.

  @param[out] Good  Returns TRUE if system power state supports a capsule
                    update.  Returns FALSE if system power state does not
                    support a capsule update.  Return value is only valid if
                    return status is EFI_SUCCESS.

  @retval EFI_SUCCESS            Good parameter has been updated with result.
  @retval EFI_INVALID_PARAMETER  Good is NULL.
  @retval EFI_DEVICE_ERROR       System power state can not be determined.

**/
EFI_STATUS
EFIAPI
CheckSystemPower (
  OUT BOOLEAN  *Good
  )
{
  EFI_STATUS                             Status;
  SETUP_DATA                             SetupData;
  UINTN                                  VariableSize;
  BOOLEAN                                IsAcPluggedIn;
  BOOLEAN                                IsBatteryLow;

  *Good = TRUE;

  VariableSize = sizeof (SETUP_DATA);
  Status = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  NULL,
                  &VariableSize,
                  &SetupData
                  );

  if (!EFI_ERROR (Status)) {
   if (SetupData.SysFwUpdateSkipPowerCheck) {
     return EFI_SUCCESS;
   }
  }

  EcAcPresent (&IsAcPluggedIn);
  PlatformCheckBattery (FixedPcdGet8 (PcdLowBatteryCheck), &IsBatteryLow);

  DEBUG ((DEBUG_INFO, "IsAcPluggedIn = 0x%x\n", IsAcPluggedIn));
  DEBUG ((DEBUG_INFO, "IsBatteryLow  = 0x%x\n", IsBatteryLow));

  if ((!IsAcPluggedIn) && (IsBatteryLow)) {
    DEBUG ((DEBUG_ERROR, "Perform SysFwUpdate PowerCheck failure.\n"));
    *Good = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  Determines if the system thermal state supports a capsule update.

  @param[out] Good  Returns TRUE if system thermal state supports a capsule
                    update.  Returns FALSE if system thermal state does not
                    support a capsule update.  Return value is only valid if
                    return status is EFI_SUCCESS.

  @retval EFI_SUCCESS            Good parameter has been updated with result.
  @retval EFI_INVALID_PARAMETER  Good is NULL.
  @retval EFI_DEVICE_ERROR       System thermal state can not be determined.

**/
EFI_STATUS
EFIAPI
CheckSystemThermal (
  OUT BOOLEAN   *Good
  )
{
  *Good = TRUE;
  return EFI_SUCCESS;
}

/**
  Determines if the system environment state supports a capsule update.

  @param[out] Good  Returns TRUE if system environment state supports a capsule
                    update.  Returns FALSE if system environment state does not
                    support a capsule update.  Return value is only valid if
                    return status is EFI_SUCCESS.

  @retval EFI_SUCCESS            Good parameter has been updated with result.
  @retval EFI_INVALID_PARAMETER  Good is NULL.
  @retval EFI_DEVICE_ERROR       System environment state can not be determined.

**/
EFI_STATUS
EFIAPI
CheckSystemEnvironment (
  OUT BOOLEAN   *Good
  )
{
  *Good = TRUE;
  return EFI_SUCCESS;
}

/**
  Determines if the Lowest Supported Version checks should be performed.  The
  expected result from this function is TRUE.  A platform can choose to return
  FALSE (e.g. during manufacturing or servicing) to allow a capsule update to a
  version below the current Lowest Supported Version.

  @retval TRUE   The lowest supported version check is required.
  @retval FALSE  Do not perform lowest support version check.

**/
BOOLEAN
EFIAPI
IsLowestSupportedVersionCheckRequired (
  VOID
  )
{
  return TRUE;
}

/**
  Determines if the FMP device should be locked when the event specified by
  PcdFmpDeviceLockEventGuid is signaled. The expected result from this function
  is TRUE so the FMP device is always locked.  A platform can choose to return
  FALSE (e.g. during manufacturing) to allow FMP devices to remain unlocked.

  @retval TRUE   The FMP device lock action is required at lock event guid.
  @retval FALSE  Do not perform FMP device lock at lock event guid.

**/
BOOLEAN
EFIAPI
IsLockFmpDeviceAtLockEventGuidRequired (
  VOID
  )
{
  return TRUE;
}
