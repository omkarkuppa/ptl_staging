/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#include <PiPei.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BiosIdLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/PciLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiLib.h>
#include <Library/MmioInitLib.h>
#include <Library/SiliconInitLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PmcLib.h>
#include <Library/PeiBootModeLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/WakeupEventLib.h>
#include <Library/BoardConfigLib.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/EcMiscLib.h>
#include <PlatformBoardConfig.h>
#include <Register/PmcRegs.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <PlatformBoardId.h>
#include <Library/PcdInfoLib.h>
#include <Protocol/IntelDieInfo.h>
#include <Library/PeiPlatformLib.h>
#include <SetupVariable.h>
#include <Library/NemMapLib.h>
#include <Guid/MigratedFvInfo.h>
#include <Library/HobLib.h>
#include <Library/PcdGpioNativeLib.h>
#include <Register/GpioAcpiDefines.h>

EFI_STATUS
EFIAPI
SiPreMemPolicyPpiNotifyCallback(
  IN CONST EFI_PEI_SERVICES** PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR* NotifyDescriptor,
  IN VOID* Ppi
  );

static EFI_PEI_NOTIFY_DESCRIPTOR mSiPreMemPolicyNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gSiPreMemPolicyReadyPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT)SiPreMemPolicyPpiNotifyCallback
};

/**
  Configure EC for specific devices

  @param[in] PchLan       - The PchLan of PCH_SETUP variable.
  @param[in] BootMode     - The current boot mode.
**/
VOID
EcInit(
  IN UINT8                PchLan,
  IN EFI_BOOT_MODE        BootMode
)
{
  //
  // Clear 10sec PB Override mode in EC so that Power button is always available during POST.
  //
  ClearTenSecPwrButtonMode();
}

/**
  Do platform specific programming. For example, EC init, Chipset programming

  @retval  EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
PlatformSpecificInitPreMem(
  VOID
  )
{
  EFI_STATUS                      Status;
  EFI_BOOT_MODE                   BootMode;
  SA_SETUP                        SaSetup;
  PCH_SETUP                       PchSetup;
  SETUP_DATA                      SystemConfiguration;
  UINTN                           VarSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI* VariableServices;

  Status = PeiServicesGetBootMode(&BootMode);
  ASSERT_EFI_ERROR(Status);

  //
  // Locate Setup variables
  //
  Status = PeiServicesLocatePpi(
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID**)&VariableServices
             );
  ASSERT_EFI_ERROR(Status);

  VarSize = sizeof(PCH_SETUP);
  Status = VariableServices->GetVariable(
                               VariableServices,
                               L"PchSetup",
                               &gPchSetupVariableGuid,
                               NULL,
                               &VarSize,
                               &PchSetup
                               );

#if FixedPcdGetBool (PcdEcEnable) == 1
  if (!EFI_ERROR(Status)) {
    if (PcdGetBool (PcdEcPresent) == TRUE) {
      EcInit(PchSetup.PchLan, BootMode);
    }
  }
#endif

  VarSize = sizeof(SETUP_DATA);
  Status = VariableServices->GetVariable(
                               VariableServices,
                               L"Setup",
                               &gSetupVariableGuid,
                               NULL,
                               &VarSize,
                               &SystemConfiguration
                               );
  if (!EFI_ERROR(Status)) {
    //
    // Selecting charging method
    //
#if FixedPcdGetBool (PcdEcEnable) == 1
    if (PcdGetBool (PcdEcPresent) == TRUE) {
      if (SystemConfiguration.EcChargingMethod == 1) {
        //
        // Fast charging
        //
        DfctFastChargingMode(TRUE);
      } else {
        //
        // Normal charging
        //
        DfctFastChargingMode(FALSE);
      }
    }
#endif

    VarSize = sizeof(SA_SETUP);
    Status = VariableServices->GetVariable(
                                 VariableServices,
                                 L"SaSetup",
                                 &gSaSetupVariableGuid,
                                 NULL,
                                 &VarSize,
                                 &SaSetup
                                 );
    if (!EFI_ERROR(Status)) {
      if (PcdGet8(PcdPlatformType) == TypeUltUlx) {
        // Force Valid Sensor Settings depending on the Option selected in Sensor Hub Type Setup Option.
        if (SystemConfiguration.SensorHubType == 0) {
          SystemConfiguration.UsbSensorHub = 0;
        } else if (SystemConfiguration.SensorHubType == 1) {
          SaSetup.AlsEnable = 0;
          SystemConfiguration.UsbSensorHub = 0;
        } else {
          SaSetup.AlsEnable = 0;
          SystemConfiguration.UsbSensorHub = 1;
        }
      }
    }
  }
  return EFI_SUCCESS;
}

/**
  Si PreMem Policy Ppi NotifyCallback.

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
SiPreMemPolicyPpiNotifyCallback(
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS Status;

  ///
  /// Platform specific programming. It may have Chipset, EC or some platform specific
  /// programming here.
  ///
  Status = PlatformSpecificInitPreMem();
  ASSERT_EFI_ERROR(Status);

  return Status;
}

/**
  Set the state to go after G3

  @dot
    digraph G {
      subgraph cluster_c0 {
        node [shape = box];
          b1[label="EcForceResetAfterAcRemoval ()" fontsize=12 style=filled color=lightblue];
          b2[label="Force S5" fontsize=12 style=filled color=lightblue];
          b3[label="Force S0" fontsize=12 style=filled color=lightblue];

        node [shape = ellipse];
          e1[label="Start" fontsize=12 style=filled color=lightblue];
          e2[label="End" fontsize=12 style=filled color=lightblue];

        node [shape = diamond,style=filled,color=lightblue];
          d1[label="GetVariable\nFroceResetAfterAcRemoval" fontsize=12];
          d2[label="EcPresent" fontsize=12];
          d3[label="ForceResetAfterAcRemoval" fontsize=12];
          d4[label="GetVariable\nStateAfterG3" fontsize=12];
          d5[label="StateAfterG3" fontsize=12];

        label = "SetTheStateToGoAfterG3 Flow"; fontsize=15; fontcolor=black; color=lightblue;
        e1 -> d1
        d1 -> d2 [label="Success" fontsize=9]
        d1 -> d4 [label="Fail" fontsize=9]
        d2 -> b1 [label="Yes" fontsize=9]
        b1 -> d3
        d2 -> d3 [label="No" fontsize=9]
        d3 -> b3 [label="Yes" fontsize=9]
        d3 -> d4 [label="No" fontsize=9]
        d4 -> d5 [label="Success" fontsize=9]
        d4 -> b3 [label="Fail" fontsize=9]
        d5 -> b2 [label="Set" fontsize=9]
        d5 -> b3 [label="Clear" fontsize=9]
        b3 -> e2
      }
    }
  @enddot

  @param[in]  VariableServices  A pointer to this instance of the EFI_PEI_READ_ONLY_VARIABLE2_PPI.
**/
VOID
EFIAPI
SetTheStateToGoAfterG3(
  IN EFI_PEI_READ_ONLY_VARIABLE2_PPI  *VariableServices
  )
{
  EFI_STATUS            Status;
  UINTN                 VariableSize;
  UINT8                 ForceResetAfterAcRemovalVar;
  UINT8                 StateAfterG3;
  PCH_SETUP             PchSetup;

  ForceResetAfterAcRemovalVar = 0;
  StateAfterG3 = 0;

  VariableSize = sizeof(UINT8);
  Status = VariableServices->GetVariable(
                               VariableServices,
                               L"ForceResetAfterAcRemoval",
                               &gOsVendorGlobalVariableGuid,
                               NULL,
                               &VariableSize,
                               &ForceResetAfterAcRemovalVar
                               );
  if (!EFI_ERROR(Status)) {
#if FixedPcdGetBool (PcdEcEnable) == 1
    if (PcdGetBool (PcdEcPresent) == TRUE) {
      ///
      /// Set AC Removal reset in EC if necessary.
      /// ForceResetAfterAcRemovalVar =  0 : Clear AC Removal reset bit in EC
      ///                                1 : Set AC Removal reset bit in EC
      ///
      EcForceResetAfterAcRemoval(ForceResetAfterAcRemovalVar);
    }
#endif
  }

  VariableSize = sizeof(PCH_SETUP);
  Status = VariableServices->GetVariable(
                               VariableServices,
                               L"PchSetup",
                               &gPchSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &PchSetup
                               );
  if (!EFI_ERROR(Status)) {
    StateAfterG3 = PchSetup.StateAfterG3;
  }

  if ((StateAfterG3 == 1) && (ForceResetAfterAcRemovalVar == 0)) {
    PmcSetPlatformStateAfterPowerFailure(1); // AfterG3 = S5
  } else {
    PmcSetPlatformStateAfterPowerFailure(0); // AfterG3 = S0
  }
}

#if FixedPcdGetBool (PcdMultiIbbFeatureEnable) == 1
#if FixedPcdGetBool (PcdSignedFspEnable) == 1
/**
  Build hob for FSP M reabse.

**/
VOID
BuildMigrateHob (
  VOID
  )
{
  UINTN                           Size;
  EDKII_MIGRATION_INFO           *MigrationInfo;
  TO_MIGRATE_FV_INFO             *ToMigrateFvInfo;

  DEBUG ((DEBUG_INFO, "BuildMigrateHob Start\n"));
  Size = sizeof (EDKII_MIGRATION_INFO) + sizeof (TO_MIGRATE_FV_INFO);
  MigrationInfo = AllocateZeroPool (Size);
  if (MigrationInfo == NULL) {
    return;
  }

  MigrationInfo->ToMigrateFvCount     = 1;
  ToMigrateFvInfo                     = (TO_MIGRATE_FV_INFO *) (MigrationInfo + 1);
  ToMigrateFvInfo->FvOrgBaseOnTempRam = (UINT32) FixedPcdGet32 (PcdFlashFvFspMBase);
  ToMigrateFvInfo->FvMigrationFlags   = 0;
  MigrationInfo->MigrateAll           = FALSE;
  BuildGuidDataHob (&gEdkiiMigrationInfoGuid, MigrationInfo, Size);
  FreePool (MigrationInfo);
  DEBUG ((DEBUG_INFO, "BuildMigrateHob End\n"));
}
#endif
#endif

/**
  Boards configuration init function for PEI pre-memory phase.

  @retval EFI_SUCCESS             The function completed successfully.
**/
EFI_STATUS
EFIAPI
PtlInitPreMem (
  VOID
  )
{
  EFI_STATUS                        Status;
  UINTN                             VariableSize;
  VOID                              *MemorySavedData;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *VariableServices;
  UINTN                             TempRamBase;
  UINT32                            TempRamSize;

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  ASSERT_EFI_ERROR (Status);
  //
  // Initialize S3 Data variable (S3DataPtr). It may be used for warm and fast boot paths.
  //
  VariableSize = 0;
  MemorySavedData = NULL;
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"MemoryConfig",
                               &gFspNonVolatileStorageHobGuid,
                               NULL,
                               &VariableSize,
                               MemorySavedData
                               );
  if (Status == EFI_BUFFER_TOO_SMALL) {
    //
    // Set the DISB bit
    // after memory Data is saved to NVRAM.
    //
    PmcSetDramInitScratchpad ();
  }

  GetTempRamBaseAndSize (&TempRamBase, &TempRamSize);
  PcdSet32S (PcdStackBase, (UINT32) TempRamBase + TempRamSize - (PcdGet32 (PcdFspTemporaryRamSize) + PcdGet32 (PcdFspReservedBufferSize)));
  PcdSet32S (PcdStackSize, PcdGet32 (PcdFspTemporaryRamSize));

  PcdSet8S (PcdCpuRatio, PcdGet8 (VpdPcdCpuRatio));
  PcdSet8S (PcdBiosGuard, PcdGet8 (VpdPcdBiosGuard));

  #if FixedPcdGetBool (PcdFspModeSelection) == 1
    Status = PtlPcdInstallDieInfo();
    ASSERT_EFI_ERROR(Status);
  #endif // FixedPcdGetBool (PcdFspModeSelection) == 1

  ///
  /// Set what state (S0/S5) to go to when power is re-applied after a power failure (G3 state)
  ///
  SetTheStateToGoAfterG3(VariableServices);

#if FixedPcdGetBool (PcdMultiIbbFeatureEnable) == 1
#if FixedPcdGetBool (PcdSignedFspEnable) == 1
  BuildMigrateHob ();
#endif
#endif

  return EFI_SUCCESS;
}

/**
  Updates the wakeupType.
**/
VOID
PtlWakeUpTypeUpdate (
  VOID
  )
{
  UINT8   WakeupType;
  //
  // Updates the wakeupType which will be used to update the same in Smbios table 01
  //
  GetWakeupEvent (&WakeupType);
  PcdSet8S (PcdWakeupType, WakeupType);
}

VOID
PtlMrcConfigInit (
  VOID
  );

VOID
PtlSaMiscConfigInit (
  VOID
  );

VOID
PtlSaGpioConfigInit (
  VOID
  );

VOID
PtlSaDisplayConfigInit (
  VOID
  );

VOID
PtlSaUsbConfigInit (
  VOID
  );

EFI_STATUS
PtlRootPortClkInfoInit (
  VOID
  );

EFI_STATUS
PtlUsbConfigInit (
  VOID
  );

VOID
PtlGpioTablePreMemInit(
  VOID
  );

VOID
PtlBoardGroupTierInit (
  VOID
  );

/**
  Board Misc init function for PEI pre-memory phase.
**/
VOID
PtlBoardMiscInitPreMem (
  VOID
  )
{
  EFI_STATUS Status;
  PCD64_BLOB *VpdPcdData;

  //
  // RecoveryMode GPIO
  //
  VpdPcdData = PcdGetPtr (VpdPcdRecoveryModeGpio);
  PcdSet64S(PcdRecoveryModeGpio, VpdPcdData->Blob);
  //
  // OddPower Init
  //
  PcdSetBoolS (PcdOddPowerInitEnable, PcdGetBool (VpdPcdOddPowerInitEnable));
  //
  // Smbus Alert function Init.
  //
  PcdSetBoolS (PcdSmbusAlertEnable, PcdGetBool (VpdPcdSmbusAlertEnable));

  // EC FailSafe Cpu Temp and Fan Speed Setting
  // Update this setting based on heat sink design
  PcdSet8S (PcdEcFailSafeActionCpuTemp, PcdGet8 (VpdPcdEcFailSafeActionCpuTemp));
  PcdSet8S (PcdEcFailSafeActionFanPwm, PcdGet8 (VpdPcdEcFailSafeActionFanPwm));

  // Connectivity GPIO Init
  CnvPreMemPcdInitBeforeSiliconInit ();

  //
  // Set FailSafe Critical Temp & Fan Speed setting
  //
#if FixedPcdGetBool (PcdEcEnable) == 1
  if (PcdGetBool (PcdEcPresent) == TRUE) {
    Status = SetFailSafeFanCtrl(
                PcdGet8(PcdEcFailSafeActionCpuTemp),
                PcdGet8(PcdEcFailSafeActionFanPwm)
    );
    DEBUG((DEBUG_INFO, " EC FailSafe (CpuTemp,Fan) set to (%d,%d), Status = %r\n", \
           PcdGet8(PcdEcFailSafeActionCpuTemp), PcdGet8(PcdEcFailSafeActionFanPwm), Status));
  }
#endif

  Status = PeiServicesNotifyPpi(&mSiPreMemPolicyNotifyList);
  ASSERT_EFI_ERROR(Status);

}

/**
  Notify Ec To Pd firmware for enabling or disabling Pcie.
**/
VOID
PtlNotifyEcToPdForPcieTunnel (
  VOID
  )
{
  EFI_STATUS                        Status;
  EFI_BOOT_MODE                     BootMode;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *VariableServices;
  SETUP_DATA                        SetupData;
  UINTN                             VariableSize;

  PeiServicesGetBootMode (&BootMode);

  //
  // PD will keep original contents during Sx state.
  // It doesn't need to do anything during these states.
  //
  if ((BootMode == BOOT_ON_S3_RESUME) ||
      (BootMode == BOOT_ON_S4_RESUME) ||
      (BootMode == BOOT_ON_S5_RESUME)) {
    return;
  }

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (SETUP_DATA);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"Setup",
                               &gSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               (VOID *) &SetupData
                               );

  ASSERT_EFI_ERROR (Status);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Get Setup variable fail Status = %r\n", Status));
    return;
  }

  //
  // PCIE Tunnel En/Dis function. PD is set enable PCIE Tunnel by default.
  //
  NotifyEcToPdToUpdatePcieTunnel (SetupData.EnablePcieTunnelingOverUsb4);

  return;
}


/**
  Conditionally enables VISA debug GPIO pins if the VisaDebug option is set in Setup.

  @param[in] Hid         The GPIO HID to use for VISA debug GPIO initialization.
**/
VOID
VisaDebugGpioInit (
  CHAR8 *Hid
  )
{
  EFI_STATUS                        Status;
  GPIOV2_SERVICES                   *GpioServices;
  PCH_SETUP                         PchSetup;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *VariableServices;
  UINTN                             VarSize;

  // Locate VariableServices
  Status = PeiServicesLocatePpi(
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID**)&VariableServices
           );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "VisaDebugGpioInit: Failed to locate VariableServices: %r\n", Status));
    return;
  }

  // Get PchSetup variable
  VarSize = sizeof(PCH_SETUP);
  Status = VariableServices->GetVariable(
                               VariableServices,
                               L"PchSetup",
                               &gPchSetupVariableGuid,
                               NULL,
                               &VarSize,
                               &PchSetup
                             );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "VisaDebugGpioInit: Failed to get PchSetup variable: %r\n", Status));
    return;
  }

  if (PchSetup.VisaDebug) {
    Status = GpioV2GetAccess(Hid, 0, &GpioServices); // GPIO_HID_PTL_PCD_P
    if (EFI_ERROR(Status)) {
      DEBUG((DEBUG_ERROR, "VisaDebugGpioInit: GpioV2GetAccess failed: %r\n", Status));
      return;
    }
    PtlPcdGpioEnableVisaPins(GpioServices);
    DEBUG((DEBUG_INFO, "VisaDebugGpioInit: VISA debug GPIO pins enabled.\n"));
  } else {
    DEBUG((DEBUG_INFO, "VisaDebugGpioInit: VISA debug not enabled.\n"));
  }
}

/**
  A hook for board-specific initialization prior to memory initialization.

  @retval EFI_SUCCESS   The board initialization was successful.
**/
EFI_STATUS
EFIAPI
PtlBoardInitBeforeMemoryInit (
  VOID
  )
{
  EFI_STATUS               Status;
  DEBUG ((DEBUG_INFO, "PtlBoardInitBeforeMemoryInit\n"));

  PtlInitPreMem ();

  PtlWakeUpTypeUpdate ();
  PtlBoardMiscInitPreMem ();

  ///
  /// Do basic PCH init
  ///
  SiliconInit ();
  PtlBoardGroupTierInit ();
  PtlGpioTablePreMemInit ();
  VisaDebugGpioInit (GPIO_HID_PTL_PCD_P); // Initialize VISA debug GPIO pins if enabled

  GpioInit (PcdGetPtr(PcdBoardGpioTableEarlyPreMem));
  //
  // According to PCIe SPEC., the delay is reuqired to ensure the delta time
  // between dTBT CIO Power Enable and PERST of PCIe slot is greater than 10ms
  // during power on. The VpdPcd can be zero for boards don't support dTBT.
  //
  if (PcdGet32 (VpdPcdMiniDeltaTimeFromEarlyPrememToPrememGpio) != 0) {
    MicroSecondDelay (PcdGet32 (VpdPcdMiniDeltaTimeFromEarlyPrememToPrememGpio));
  }
  PtlMrcConfigInit ();
  PtlSaGpioConfigInit ();
  PtlSaMiscConfigInit ();
  Status = PtlRootPortClkInfoInit ();
  Status = PtlUsbConfigInit ();
  PtlSaDisplayConfigInit ();
  PtlSaUsbConfigInit ();
  // Configure GPIO Before Memory
  GpioInit (PcdGetPtr (PcdBoardGpioTablePreMem));

  // Configure GPIO group GPE tier
  GpioGroupTierInit ();


  // Notify Ec To Pd firmware for enabling or disabling Pcie.
#if FixedPcdGetBool (PcdEcEnable) == 1
  if (PcdGetBool (PcdEcPresent) == TRUE) {
    PtlNotifyEcToPdForPcieTunnel ();
  }
#endif

  ASSERT(Status == EFI_SUCCESS);
  return EFI_SUCCESS;
}

/**
  A hook for board-specific initialization after memory initialization.

  @retval EFI_SUCCESS   The board initialization was successful.
**/
EFI_STATUS
EFIAPI
PtlBoardInitAfterMemoryInit(
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "PtlBoardInitAfterMemoryInit\n"));
  //
  // Update Pch Type in BIOS ID string
  //
  UpdatePchTypeInBoardId ();

  return MmioInit ();
}

/**
  This board service initializes board-specific debug devices.

  @retval EFI_SUCCESS   Board-specific debug initialization was successful.
**/
EFI_STATUS
EFIAPI
PtlBoardDebugInit (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "PtlBoardDebugInit\n"));
  return EFI_SUCCESS;
}

/**
  This board service detects the boot mode.

  @retval EFI_BOOT_MODE The boot mode.
**/
EFI_BOOT_MODE
EFIAPI
PtlBoardBootModeDetect (
  VOID
  )
{
  EFI_BOOT_MODE                             BootMode;

  DEBUG ((DEBUG_INFO, "PtlBoardBootModeDetect\n"));
  BootMode = DetectBootMode ();
  DEBUG ((DEBUG_INFO, "BootMode: 0x%02X\n", BootMode));
  return BootMode;
}
