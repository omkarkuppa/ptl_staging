/** @file
  Source code file for the Boot Mode Library

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

#include <Base.h>
#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/Capsule.h>
#include <SetupVariable.h>
#include <Library/PmcLib.h>
#include <Register/TcoRegs.h>
#include <FastBootDataDef.h>
#include <FastBootExceptionInfoHob.h>
#include <CmosMap.h>
#include <Library/CmosAccessLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/GpioV2WrapperLib.h>
#include <PlatformBoardConfig.h>
#include <Library/PeiBootModeLib.h>
#include "PeiBootModeLibInternal.h"
#include <PlatformBoardId.h>
#include <PlatformBoardType.h>
#include <PlatformNvRamHookLib.h>
#include <Library/SetupInitLib.h>
#include <BootStateLib.h>
#include <Include/ChassisIntrDetHob.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Guid/CapsuleVendor.h>

#define FAST_BOOT_OR_MASK       0x01

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_PPI_DESCRIPTOR mPpiListNonS3BootMode = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiBootInNonS3ModePpiGuid,
  NULL
};

///
/// Priority of our boot modes, highest priority first
///
GLOBAL_REMOVE_IF_UNREFERENCED EFI_BOOT_MODE mBootModePriority[] = {
  BOOT_IN_RECOVERY_MODE,
  BOOT_WITH_DEFAULT_SETTINGS,
  BOOT_ON_FLASH_UPDATE,
  BOOT_ON_S3_RESUME,
  BOOT_ON_S4_RESUME,
  BOOT_WITH_MINIMAL_CONFIGURATION,
  BOOT_ASSUMING_NO_CONFIGURATION_CHANGES,
  BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS,
  BOOT_WITH_FULL_CONFIGURATION,
  BOOT_ON_S5_RESUME,
  BOOT_SPECIAL_MASK
};

EFI_STATUS
EFIAPI
PreMemUpdateBootMode (
  IN OUT EFI_BOOT_MODE          *BootMode
  );

/**
  Determine if we're in capsule update mode

  @retval EFI_SUCCESS   if we have a capsule available
  @retval EFI_NOT_FOUND when no capsule detected

**/
EFI_STATUS
EFIAPI
CheckCapsuleUpdate (
  VOID
  )
{
  EFI_STATUS                       Status;
  UINTN                            Size;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *PpiVariableServices;
  EFI_PHYSICAL_ADDRESS             CapsuleDataPtr64;

  CapsuleDataPtr64 = 0;

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **)&PpiVariableServices
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to find ReadOnlyVariable2PPI\n"));
    return EFI_NOT_FOUND;
  }

  ///
  /// Check Update Capsule by directly reading variable, since PeiBootModeLib has
  /// no dependency with CapsulePpi and may fail to locate CapsulePpi.
  ///
  Size   = sizeof (CapsuleDataPtr64);
  Status = PpiVariableServices->GetVariable (
                                  PpiVariableServices,
                                  EFI_CAPSULE_VARIABLE_NAME,
                                  &gEfiCapsuleVendorGuid,
                                  NULL,
                                  &Size,
                                  (VOID *)&CapsuleDataPtr64
                                  );

  if (!EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

/**
  Update boot mode if it is in capsule update boot mode.

  @param[in,out] BootMode         Current System BootMode.

  @retval EFI_SUCCESS             Boot Mode is updated.
  @retval Others                  Some issues occur in updating Boot Mode.

**/
EFI_STATUS
EFIAPI
PreMemUpdateCapsuleBootMode (
  IN OUT EFI_BOOT_MODE          *BootMode
  )
{
  EFI_STATUS                    Status;

  DEBUG((DEBUG_INFO, "PeiBootModeLib PreMemUpdateCapsuleBootMode PeiServicesGetBootMode = 0x%x\n", *BootMode));

  ///
  /// Determine if we're in capsule update mode
  ///
  Status = CheckCapsuleUpdate ();
  if (Status == EFI_SUCCESS) {
    PrioritizeBootMode(BootMode, BOOT_ON_FLASH_UPDATE);
  }

  if (*BootMode != BOOT_ON_FLASH_UPDATE) {
    ///
    /// Skip other Boot Mode update during capsule update
    ///
    Status = PreMemUpdateBootMode (BootMode);
  }

  DEBUG((DEBUG_INFO, "PeiBootModeLib PreMemUpdateCapsuleBootMode PeiServicesSetBootMode = 0x%x\n", *BootMode));
  return Status;
}

/**
  Update boot progress bit for TxtClean reset.
**/
EFI_STATUS
EFIAPI
UpdateFastBootFlagStatusCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

/**
  Notification on reset for TxtClean.
**/
static EFI_PEI_NOTIFY_DESCRIPTOR mPeiTxtCleanResetNotificationNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiTxtCleanResetNotificationPpiGuid,
  UpdateFastBootFlagStatusCallback
};

/**
  Update boot progress bit for TxtClean reset.
  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The gPeiTxtCleanResetNotificationPpiGuid PPI. Not used.
**/
EFI_STATUS
EFIAPI
UpdateFastBootFlagStatusCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  DEBUG ((DEBUG_INFO, "FastBootCallback Entry.\n"));
  // Clear Boot Progress bit [bit0]
  UpdateFastBootFlagStatus (GetFastBootFlagStatus () & ~BIT0);

  return EFI_SUCCESS;
}

/**
  Check fast boot is enabled or not

  @retval  FastBootEnabledStatus  TRUE means fast boot is enabled
                                  FALSE means fast boot is disabled
**/
BOOLEAN
EFIAPI
IsFastBootEnabled (
  VOID
  )
{
  EFI_STATUS                      Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *PeiReadOnlyVarPpi;
  UINTN                           VarSize;
  SETUP_DATA                      SystemConfiguration;
  BOOLEAN                         FastBootEnabledStatus;

  FastBootEnabledStatus = FALSE;
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &PeiReadOnlyVarPpi
             );
  if (Status == EFI_SUCCESS) {
    VarSize = sizeof (SETUP_DATA);
    Status = PeiReadOnlyVarPpi->GetVariable (
                                  PeiReadOnlyVarPpi,
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
  }

  return FastBootEnabledStatus;
}

/**
  Update boot mode

  @param[in,out] BootMode             Current System BootMode.

  @retval EFI_SUCCESS                 The boot mode was returned successfully.

**/

EFI_STATUS
EFIAPI
PreMemUpdateBootMode (
  IN OUT EFI_BOOT_MODE          *BootMode
  )
{
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *VariableServices;
  UINTN                             VarSize;
  BOOLEAN                           IsFastBootEnable;
  EFI_STATUS                        Status;
  UINT8                             FastBootRegData;
  FAST_BOOT_EXCEPTION_INFO_HOB      *FastBootExceptionInfoHob;

  DEBUG ((DEBUG_INFO, "PeiBootModeLib PreMemUpdateBootMode Entry\n"));
  FastBootRegData = 0;

  IsFastBootEnable = IsFastBootEnabled ();
  if (IsFastBootEnable) {
    if (IsPreviousFastBootCompleted () && !IsTimeOutOccurred () && !IsPowerButtonOverride ()) {
        PrioritizeBootMode (BootMode, BOOT_WITH_MINIMAL_CONFIGURATION);
    } else {
      Status = PeiServicesCreateHob (
                 EFI_HOB_TYPE_GUID_EXTENSION,
                 sizeof (FAST_BOOT_EXCEPTION_INFO_HOB),
                 (VOID **) &FastBootExceptionInfoHob
                 );
      if (!EFI_ERROR (Status)) {
        FastBootExceptionInfoHob->Header.Name = gFastBootExceptionInfoHobGuid;
        FastBootExceptionInfoHob->FbExceptionType = ExceptionType2;
        FastBootExceptionInfoHob->FbExceptionCategory = BootFailure;
      }
      DEBUG ((DEBUG_INFO, "PeiBootModeLib Exception has occurred. Prioritizing Boot mode to BOOT_WITH_FULL_CONFIGURATION\n"));
      PrioritizeBootMode (BootMode, BOOT_WITH_FULL_CONFIGURATION);
    }
  } else {
    if ((*BootMode != BOOT_ON_S5_RESUME) && IsBootWithNoChange ()) {
      PrioritizeBootMode (BootMode, BOOT_ASSUMING_NO_CONFIGURATION_CHANGES);
    } else {
      PrioritizeBootMode (BootMode, BOOT_WITH_FULL_CONFIGURATION);
    }
  }

  ///
  /// FastBoot - set boot progress bit to indicate boot is in progress
  /// bit will be cleared before booting to OS/Shell/BIOS Setup or ResetSystem() been triggered
  ///
  if (IsFastBootEnable) {
    if (*BootMode != BOOT_ON_S3_RESUME) {
      FastBootRegData = GetFastBootFlagStatus ();
      FastBootRegData |= FAST_BOOT_OR_MASK;
      UpdateFastBootFlagStatus (FastBootRegData);
      PeiServicesNotifyPpi(&mPeiTxtCleanResetNotificationNotifyList);
    }
  }

  if (*BootMode != BOOT_ON_S3_RESUME) {
    ///
    /// If not in S3 mode, signal it.
    ///
    Status = PeiServicesInstallPpi (&mPpiListNonS3BootMode);
    ASSERT_EFI_ERROR (Status);
  }

  if (IsFastBootEnable) {
    Status = PeiServicesLocatePpi (
               &gEfiPeiReadOnlyVariable2PpiGuid,
               0,
               NULL,
               (VOID **) &VariableServices
               );
    VarSize = 0;
    if (!RETURN_ERROR (Status)) {
      Status = VariableServices->GetVariable (
                                   VariableServices,
                                   L"ConOut",
                                   &gEfiGlobalVariableGuid,
                                   NULL,
                                   &VarSize,
                                   NULL
                                   );
      if (Status == EFI_NOT_FOUND) {
        PrioritizeBootMode (BootMode, BOOT_WITH_FULL_CONFIGURATION);
      }
    }
  }

  DEBUG ((DEBUG_INFO, "PeiBootModeLib PreMemUpdateBootMode Exit "));
  return EFI_SUCCESS;
}

/**
  Detect boot mode

  @retval EFI_BOOT_MODE  Current System BootMode.
**/

EFI_BOOT_MODE
EFIAPI
DetectBootMode (
  VOID
  )
{
  EFI_STATUS                      Status;
  EFI_BOOT_MODE                   BootMode;

  PMC_SLEEP_STATE                 SleepType;

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, "PeiBootModeLib DetectBootMode PeiServicesGetBootMode BootMode= 0x%x \n", BootMode));

  ///
  /// Check boot mode is already recovery
  ///
  if (BootMode == BOOT_IN_RECOVERY_MODE) {
    return BootMode;
  }

  Status = IsRecoveryMode ();
  if (Status == EFI_SUCCESS) {
    BootMode = BOOT_IN_RECOVERY_MODE;
    DEBUG((DEBUG_INFO, "PeiBootModeLib IsRecoveryMode BOOT_IN_RECOVERY_MODE"));
    //mPpiListRecoveryBootMode is installed by MinPlatform
  }

  SleepType = PmcGetSleepTypeAfterWake ();
  DEBUG((DEBUG_INFO, "PeiBootModeLib PmcGetSleepTypeAfterWake SleepType= %d \n", SleepType));
  if (SleepType != PmcNotASleepState) {
    if (!PmcIsPowerFailureDetected () && PmcIsRtcBatteryGood()) {
      switch (SleepType) {
        case PmcS3SleepState:
          PrioritizeBootMode (&BootMode, BOOT_ON_S3_RESUME);
          DEBUG((DEBUG_INFO, "PeiBootModeLib PmcS3SleepState BootMode= 0x%x \n", BootMode));
          break;
        case PmcS4SleepState:
          PrioritizeBootMode (&BootMode, BOOT_ON_S4_RESUME);
          DEBUG((DEBUG_INFO, "PeiBootModeLib PmcS4SleepState BootMode= 0x%x \n", BootMode));
          break;
        case PmcS5SleepState:
          PrioritizeBootMode (&BootMode, BOOT_ON_S5_RESUME);
          DEBUG((DEBUG_INFO, "PeiBootModeLib PmcS5SleepState BootMode= 0x%x \n", BootMode));
          break;
        default:
          break;
      }
    }
  } else {
    ///
    /// Clear Wake Status and Sleep Type
    ///
    PmcClearWakeStatus ();
    PmcSetSleepState (PmcInS0State);
  }

  if ( IsCoinlessModeDetect () == TRUE ) {
    DEBUG ((DEBUG_INFO, "In Coinless mode\n"));
  } else {
    DEBUG ((DEBUG_INFO, "In Coin mode or Coinless Mode Gpio is LOW\n"));
    if (!PmcIsRtcBatteryGood ()) {
      DEBUG ((DEBUG_ERROR, "RTC Power failure !! Could be due to a weak or missing battery, BootMode BOOT_WITH_DEFAULT_SETTINGS.\n"));
      BootMode = BOOT_WITH_DEFAULT_SETTINGS;
    }
  }


  ///
  /// Check whether Setup Variable does exist to know the first boot or not.
  /// Note: Any modification to the below code will impact RPE feature : "Revert to BIOS Golden Config ".
  ///
#if FixedPcdGetBool (PcdSetupEnable) == 1
  if (!IsSetupStructuresUnChanged ()) {
    DEBUG((DEBUG_INFO, "Setup changed in DetectBootMode\n"));
    BootMode = BOOT_WITH_DEFAULT_SETTINGS;
  }
  DEBUG((DEBUG_ERROR, "PeiBootModeLib after Setup Variables check, BootMode = 0x%x\n", BootMode));
#endif

#if FixedPcdGetBool (PcdCapsuleEnable) == 1
  ///
  /// Update BootMode
  ///
  Status = PreMemUpdateCapsuleBootMode(&BootMode);
#else
  Status = PreMemUpdateBootMode (&BootMode);
#endif
  DEBUG((DEBUG_INFO, "PeiBootModeLib Exit of DetectBootModeLib, BootMode =0x%x\n", BootMode));
  return BootMode;
}

/**
  Check Boot Flag Status to determine if previous boot was successful
  Boot Progress bit will be cleared before booting to OS/Shell/BIOS Setup
  or ResetSystem() been triggered while Fastboot enabled.

  @retval  TRUE   Previous Fast Boot was success.
  @retval  FALSE  Previous Fast Boot wasn't success.
**/

BOOLEAN
EFIAPI
IsPreviousFastBootCompleted (
  VOID
  )
{
  UINT8                           PreviousFastBootCompletionFlag;
  ///
  /// Get last Boot State Variable to confirm that it is not a first boot.
  ///
  if (!IsBootStatePresent()) {
    return FALSE;
  }
  PreviousFastBootCompletionFlag = 0;
  PreviousFastBootCompletionFlag = GetFastBootFlagStatus ();
  PreviousFastBootCompletionFlag &= FAST_BOOT_OR_MASK;

  if(PreviousFastBootCompletionFlag){
    DEBUG((DEBUG_INFO, "IsPreviousFastBootCompleted() returns FALSE as previous boot is still in progress \n"));
    return FALSE;
  }
  DEBUG ((DEBUG_INFO, "IsPreviousFastBootCompleted() return TRUE as previoius boot is successfully completed.\n"));
  return TRUE;
}

/**
  Query PCH to determine if Intrusion Detection set

  @retval  TRUE   No Change.
  @retavl  FALSE  Change.
**/
BOOLEAN
EFIAPI
IsBootWithNoChange (
  VOID
  )
{
  EFI_STATUS                      Status;
  UINT16                          TcoBase;
  UINT16                          Tco2Status;
  BOOLEAN                         BoxOpen;
  CHASSIS_INTR_DET_HOB            *ChassisIntrDetHob;

  DEBUG((DEBUG_INFO, "PeiBootModeLib IsBootWithNoChange Entry \n"));

  ChassisIntrDetHob = GetFirstGuidHob(&gChassisIntrudeDetHobGuid);
  if (ChassisIntrDetHob == NULL) {
    ///
    /// Create HOB for Chassis Intrusion Detection Status
    ///
    Status = PeiServicesCreateHob(
      EFI_HOB_TYPE_GUID_EXTENSION,
      sizeof(CHASSIS_INTR_DET_HOB),
      (VOID**)&ChassisIntrDetHob
    );
    ASSERT_EFI_ERROR(Status);
    if (EFI_ERROR(Status)) {
      return FALSE;
    }
    DEBUG((DEBUG_INFO, "Chassis Intrusion Detection Status HOB installed\n"));

    //
    // Initialize default HOB data
    //
    ZeroMem(&(ChassisIntrDetHob->Revision), (sizeof(CHASSIS_INTR_DET_HOB) - sizeof(EFI_HOB_GUID_TYPE)));
    ChassisIntrDetHob->EfiHobGuidType.Name = gChassisIntrudeDetHobGuid;
    ChassisIntrDetHob->Revision = 1;
  }

  DEBUG((DEBUG_INFO, "Chassis Intrusion Detection HOB data updated\n"));

  ///
  /// Read the TCO registers
  ///
  TcoBase     = (UINT16)PcdGet16 (PcdTcoBaseAddress);
  Tco2Status  = IoRead16 (TcoBase + R_TCO_IO_TSTS2);

  ///
  /// This is the state of the hardware
  ///
  BoxOpen = (BOOLEAN) (Tco2Status & B_TCO_IO_TSTS2_INTRD_DET);
  if (BoxOpen) {
    /// Record Chassis Intrusion Status in a Hob right before clearing it.
    ChassisIntrDetHob->Status = 1;
    ///
    /// Clear the bit for next boot.
    ///
    Tco2Status |= B_TCO_IO_TSTS2_INTRD_DET;
    IoWrite16 (TcoBase + R_TCO_IO_TSTS2, Tco2Status);

    ///
    /// Since it was OPEN, return that it cannot be in "no config change boot"
    ///
    DEBUG ((DEBUG_INFO, "PeiBootModeLib BoxOpen \n"));
    return FALSE;
  } else {
    /// Record Chassis Intrusion Status in a Hob right before clearing it.
    ChassisIntrDetHob->Status = 0;
    ///
    /// Since it was CLOSED, return that can be "no config change boot"
    /// Need variable to know that memory was tested AT LEAST one time!
    ///
    if (IsBootStatePresent()) {
      DEBUG ((DEBUG_INFO, "PeiBootModeLib BootState variable is present\n"));
      return TRUE;
    } else {
      DEBUG ((DEBUG_INFO, "PeiBootModeLib BootState variable is not present\n"));
      return FALSE;
    }
  }
}

/**
  Given the current boot mode, and a proposed new boot mode, determine
  which has priority. If the new boot mode has higher priority, then
  make it the current boot mode.

  @param[in][out] CurrentBootMode Pointer to current planned boot mode
  @param[in][out] NewBootMode     Proposed boot mode.

  @retval         EFI_NOT_FOUND   If either boot mode is not recognized.
  @retval         EFI_SUCCESS     If both boot mode values were
                                  recognized and were processed.
**/
EFI_STATUS
EFIAPI
PrioritizeBootMode (
  IN OUT EFI_BOOT_MODE    *CurrentBootMode,
  IN EFI_BOOT_MODE        NewBootMode
  )
{
  UINT32 CurrentIndex;
  UINT32 NewIndex;

  DEBUG((DEBUG_INFO, "PeiBootModeLib PrioritizeBootMode Entry \n"));
  ///
  /// Find the position of the current boot mode in our priority array
  ///
  for ( CurrentIndex = 0;
        CurrentIndex < sizeof (mBootModePriority) / sizeof (mBootModePriority[0]);
        CurrentIndex++) {
    if (mBootModePriority[CurrentIndex] == *CurrentBootMode) {
      break;
    }
  }
  if (CurrentIndex >= sizeof (mBootModePriority) / sizeof (mBootModePriority[0])) {
    return EFI_NOT_FOUND;
  }
  ///
  /// Find the position of the new boot mode in our priority array
  ///
  for ( NewIndex = 0;
        NewIndex < sizeof (mBootModePriority) / sizeof (mBootModePriority[0]);
        NewIndex++) {
    if (mBootModePriority[NewIndex] == NewBootMode) {
      ///
      /// If this new boot mode occurs before the current boot mode in the
      /// priority array, then take it.
      ///
      if (NewIndex < CurrentIndex) {
        *CurrentBootMode = NewBootMode;
        DEBUG((DEBUG_INFO, "PeiBootModeLib NewIndex < CurrentIndex, Update  PrioritizeBootMode CurrentBootMode: 0x%x \n", *CurrentBootMode));
    }
    else {
       DEBUG((DEBUG_INFO, "PeiBootModeLib NewIndex > CurrentIndex Exit PrioritizeBootMode CurrentBootMode 0x%x \n", *CurrentBootMode));
    }
      return EFI_SUCCESS;
    }
  }
  DEBUG((DEBUG_INFO, "PeiBootModeLib PrioritizeBootMode Exit\n"));
  return EFI_NOT_FOUND;
}

/**
  Check if this system boot is due to 4sec power button override has occurred

  @retval  TRUE   Power Button Override occurred in last system boot
  @retval  FALSE  Power Button Override didnt occur
**/
BOOLEAN
EFIAPI
IsPowerButtonOverride (
  VOID
  )
{
  if (PmcIsPowerButtonOverrideDetected ()) {
    DEBUG ((DEBUG_INFO, "IsPowerButtonOverride TRUE\n"));
    return TRUE;
  }
  return FALSE;
}

/**
  Check Fast Boot 30-second WDT (TCO Timer) timeout has occurred

  @retval  TRUE   Timeout has occurred
  @retval  FALSE  Timeout didnt occur
**/
BOOLEAN
IsTimeOutOccurred (
  VOID
  )
{
  if (IoRead8 (PcdGet16 (PcdTcoBaseAddress) + R_TCO_IO_TWDS) == FAST_BOOT_WDT_RESET_SIGNATURE) {
    IoWrite8 (PcdGet16 (PcdTcoBaseAddress) + R_TCO_IO_TWDS, 0);
    DEBUG ((DEBUG_INFO, "IsTimeOutOccurred TRUE\n"));
    return TRUE;
  }
  return FALSE;
}

/**
  Check if the value is of ODD parity.

  @param[in]  Value  The 8 bits value

  @retval     TRUE   The value is of ODD parity
  @retval     FALSE  The value is not of ODD parity
**/
BOOLEAN
CheckOddParity (
  IN  UINT8 Value
  )
{
  UINT8                           OddNum;
  UINT8                           Index;

  OddNum = 0;
  for (Index = 0; Index < 8; Index++) {
    if (Value & (1 << Index)) {
      OddNum++;
    }
  }
  return (BOOLEAN) (OddNum % 2 != 0);
}

/**
  Detect recovery mode

  Detect Jumper to determine if recovery mode is set

  @retval  EFI_SUCCESS       System in Recovery Mode
  @retval  EFI_UNSUPPORTED   System doesn't support Recovery Mode
  @retval  EFI_NOT_FOUND     System is not in Recovery Mode
**/

EFI_STATUS
EFIAPI
EFIAPI
IsRecoveryMode (
  VOID
  )
{
  EFI_STATUS            Status;
  GPIOV2_PAD_STATE      RecoveryModeGpioState;
  PCD64_BLOB            Pcd64;

  if (PcdGet8 (PcdBoardType) == BoardTypeSv ||
      PcdGet8 (PcdBoardType) == BoardTypeRvpErb ||
      PcdGet8 (PcdPlatformFlavor) == FlavorUpServer) {
    return RETURN_UNSUPPORTED; /* No Recovery Jumper*/
  }

  RecoveryModeGpioState = GpioV2StateHigh;
  Pcd64.Blob = PcdGet64 (PcdRecoveryModeGpio);
  if (Pcd64.Blob != 0 && Pcd64.BoardGpioConfig.Type == BoardGpioTypePch) {
    Status = GpioV2GetRx (Pcd64.BoardGpioConfig.u.Pin, &RecoveryModeGpioState);
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  } else {
    DEBUG ((DEBUG_ERROR, "Invalid Recovery GPIO, PcdRecoveryModeGpio = 0x%x\n", PcdGet64 (PcdRecoveryModeGpio)));
    return EFI_NOT_FOUND;
  }

  if (RecoveryModeGpioState == GpioV2StateLow) {
    DEBUG ((DEBUG_INFO, "RecJumperFlag SET\n"));
    return EFI_SUCCESS;
  } else {
    DEBUG ((DEBUG_INFO, "RecJumperFlag NOT SET\n"));
    return EFI_NOT_FOUND;
  }
}

/**
  Detect coinless Mode.

  @retval  TRUE   Execute coinless Mode.
  @retval  FALSE  Doesn't execute coinless Mode.
**/
BOOLEAN
EFIAPI
IsCoinlessModeDetect (
  VOID
  )
{
  GPIOV2_PAD_STATE  GpioPadState;
  EFI_STATUS        Status;

  GpioPadState = GpioV2StateLow;
  if (PcdGet32 (PcdCoinlessModeDetectGpio) == 0) {
    DEBUG ((DEBUG_INFO, "Coinless mode GPIO not present\n"));
    return FALSE;
  }

  Status = GpioV2GetRx (PcdGet32 (PcdCoinlessModeDetectGpio), &GpioPadState);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Invalid Coinless Mode GPIO: %x\n", PcdGet32 (PcdCoinlessModeDetectGpio)));
    return FALSE;
  }

  if (GpioPadState == GpioV2StateHigh) {
    return TRUE;
  } else {
    return FALSE;
  }
}
