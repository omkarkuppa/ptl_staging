/** @file
  PCH private PEI PMC Library for all PCH generations.

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

#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/SiScheduleResetLib.h>
#include <Register/PmcRegsFivr.h>
#include <PmcHandle.h>
#include <IndustryStandard/Pci30.h>
#include <PchPolicyCommon.h> // To import FORCE_DISABLE/FORCE_ENABLE definitions

/**
  This function locks FIVR Configuration

  @param[in] PchPwrmBase  PMC base address
**/
VOID
PmcLockFivrConfig (
  IN UINTN  PchPwrmBase
  )
{
  //
  // GEN_PMCON_B[17] set after configuring FIVR external VR register
  //
  MmioOr32 (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_B, B_PMC_PWRM_GEN_PMCON_B_VR_CONFIG_LOCK);
}

/**
  Clear MSE and BM bits in PMC Pci config space and clear PMC BARs.

  @param[in] PmcPciCfgBase             Pmc pci config base address
**/
VOID
PmcDisableMmio (
  IN UINT64  PmcPciCfgBase
  )
{
  UINT8   Index;
  //
  // Clear MSE and Bus Master bits in PMC Pci config space.
  //
  PciSegmentAnd8 (
    PmcPciCfgBase + PCI_COMMAND_OFFSET,
    (UINT8) ~(EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER)
    );

  //
  // Zero BAR, BAR1 and BAR2 (64-bit addresses) in PMC PCI config space
  //
  for (Index = 0; Index < 3; Index++) {
    PciSegmentWrite32 (PmcPciCfgBase + PCI_BASE_ADDRESSREG_OFFSET + (Index * 8), 0);       // BAR 32 lower bits
    PciSegmentWrite32 (PmcPciCfgBase + PCI_BASE_ADDRESSREG_OFFSET + (Index * 8) + 4, 0);   // BAR 32 upper bits
  }
}

/**
  Configure PM Timer.
  Once PM Timer is disabled, TCO timer stops counting.
  This must be executed after uCode PM timer emulation.

  @param[in] SiPolicy                  The SI Policy PPI instance
**/
VOID
PmcConfigurePmTimer (
  IN PMC_HANDLE  *PmcHandle
  )
{
  if (PmcHandle->PmConfig->EnableTcoTimer) {
    return;
  }

  DEBUG ((DEBUG_INFO, "PmcConfigurePmTimer - Disable ACPI timer.\n"));
  //
  // For CNL: Disable ACPI timer (set PwrmBase + 0x18FC[1] = 1), and the TCO timer will also stop.
  //
  MmioWrite32 (PmcHandle->PwrmBase + R_PMC_PWRM_ACPI_TMR_CTL, B_PMC_PWRM_ACPI_TMR_CTL_ACPI_TIM_DIS);
}

/**
  Lock down PMC settings

  @param[in] SiPolicy      The SI Policy PPI instance
  @param[in] PmcSocConfig  PMC SoC config
**/
VOID
PmcLockSettings (
  IN PMC_HANDLE  *PmcHandle,
  IN PMC_SOC_CONFIG  *PmcSocConfig
  )
{
  UINT32                     RegisterOffset;
  ///
  /// Set PWRMBASE Offset 0x1048 [24]
  ///
  MmioOr32 (PmcHandle->PwrmBase + R_PMC_PWRM_ETR3, BIT24);

  ///
  /// PM_SYNC_LOCK
  /// Set PWRMBASE Offset 0x18C8 [15]
  ///
  switch (PmcSocConfig->PmSyncMiscCfgInterfaceVersion) {
  case 2:
    RegisterOffset = R_PMC_PWRM_PM_SYNC_MISC_CFG_VER2;
    break;
  case 1:
  default:
    RegisterOffset = R_PMC_PWRM_PM_SYNC_MISC_CFG;
    break;
  }

  MmioOr32 (PmcHandle->PwrmBase + RegisterOffset, B_PMC_PWRM_PM_SYNC_MISC_CFG_PM_SYNC_LOCK);
}

/**
  Configure deep Sx settings

  @param[in] PchPwrmBase  PMC MMIO
  @param[in] PmConfig     Pointer to PM config

  @retval EFI_SUCCESS                   The function completed successfully
**/
STATIC
VOID
ConfigureDeepSxSettings (
  IN UINTN          PchPwrmBase,
  IN PCH_PM_CONFIG  *PmConfig
  )
{
  UINT32                                S3Data32;
  UINT32                                S4Data32;
  UINT32                                S5Data32;

  ///
  /// PCH BIOS Spec Section 19.11  Deep Sx Power Policies
  /// The System BIOS can perform the following register programming guidelines to enable system
  /// enter Deep S3, Deep S4 or Deep S5.
  ///
  ///                            DPS3_EN_DC         DPS3_EN_AC         DPS4_EN_DC         DPS4_EN_AC         DPS5_EN_DC          DPS5_EN_AC
  ///                         S3_PWRGATE_POL[1]  S3_PWRGATE_POL[0]  S4_PWRGATE_POL[1]  S4_PWRGATE_POL[0]  S5_PWRGATE_POL[15]  S5_PWRGATE_POL[14]
  /// Deep Sx disabled                 0                0                  0                  0                  0                   0
  ///
  /// Enabled in S5                    0                0                  0                  0                  1                   1
  ///
  /// Enabled in S4 and S5             0                0                  1                  1                  1                   1
  ///
  /// Enabled in S3, S4 and S5         1                1                  1                  1                  1                   1
  ///
  /// Configuration supported by MOBILE:
  /// Enabled in S5                    0                0                  0                  0                  1                   0
  /// (Battery mode)
  /// Enabled in S4 and S5             0                0                  1                  0                  1                   0
  /// (Battery Mode)
  /// Enabled in S3, S4 and S5         1                0                  1                  0                  1                   0
  /// (Battery Mode)
  ///
  /// NOTE: Mobile platforms support Deep S3/S4/S5 in DC ONLY,
  /// Desktop and Intel C206 Chipset (LPC Dev ID 0x1C56) platforms support Deep S3/S4/S5 in AC ONLY,
  /// Intel C204 Chipset (LPC Dev ID 0x1C54) and Intel C202 Chipset (LPC Dev ID 0x1C52) platforms DO NOT support Deep S4/S5.
  ///
  /// Deep Sx disabled                 0                0                  0                  0                  0                   0
  ///
  switch (PmConfig->PchDeepSxPol) {
    case PchDpS5AlwaysEn:
      ///
      /// Configuration 2: Enabled in S5/AC-DC
      /// DEEP_S3_POL.DPS3_EN_DC = 0; DEEP_S3_POL.DPS3_EN_AC = 0;
      /// DEEP_S4_POL.DPS4_EN_DC = 0; DEEP_S4_POL.DPS4_EN_AC = 0;
      /// DEEP_S5_POL.DPS5_EN_DC = 1; DEEP_S5_POL.DPS5_EN_AC = 1;
      ///
      S3Data32  = 0;
      S4Data32  = 0;
      S5Data32  = (UINT32) (B_PMC_PWRM_S5_PWRGATE_POL_S5AC_GATE_SUS | B_PMC_PWRM_S5_PWRGATE_POL_S5DC_GATE_SUS);
      break;

    case PchDpS4S5AlwaysEn:
      ///
      /// Configuration 4: Enabled only in S4-S5
      /// DEEP_S3_POL.DPS3_EN_DC = 0; DEEP_S3_POL.DPS3_EN_AC = 0;
      /// DEEP_S4_POL.DPS4_EN_DC = 1; DEEP_S4_POL.DPS4_EN_AC = 1;
      /// DEEP_S5_POL.DPS5_EN_DC = 1; DEEP_S5_POL.DPS5_EN_AC = 1;
      ///
      S3Data32  = 0;
      S4Data32  = (UINT32) (B_PMC_PWRM_S4_PWRGATE_POL_S4AC_GATE_SUS | B_PMC_PWRM_S4_PWRGATE_POL_S4DC_GATE_SUS);
      S5Data32  = (UINT32) (B_PMC_PWRM_S5_PWRGATE_POL_S5AC_GATE_SUS | B_PMC_PWRM_S5_PWRGATE_POL_S5DC_GATE_SUS);
      break;

    case PchDpS3S4S5AlwaysEn:
      ///
      /// Configuration 6: Enabled only in S3-S4-S5
      /// DEEP_S3_POL.DPS3_EN_DC = 1; DEEP_S3_POL.DPS3_EN_AC = 1;
      /// DEEP_S4_POL.DPS4_EN_DC = 1; DEEP_S4_POL.DPS4_EN_AC = 1;
      /// DEEP_S5_POL.DPS5_EN_DC = 1; DEEP_S5_POL.DPS5_EN_AC = 1;
      ///
      S3Data32  = (UINT32) (B_PMC_PWRM_S3_PWRGATE_POL_S3AC_GATE_SUS | B_PMC_PWRM_S3_PWRGATE_POL_S3DC_GATE_SUS);
      S4Data32  = (UINT32) (B_PMC_PWRM_S4_PWRGATE_POL_S4AC_GATE_SUS | B_PMC_PWRM_S4_PWRGATE_POL_S4DC_GATE_SUS);
      S5Data32  = (UINT32) (B_PMC_PWRM_S5_PWRGATE_POL_S5AC_GATE_SUS | B_PMC_PWRM_S5_PWRGATE_POL_S5DC_GATE_SUS);
      break;

    case PchDpS5BatteryEn:
      ///
      /// Configuration 1: Enabled in S5/Battery only
      /// DEEP_S3_POL.DPS3_EN_DC = 0; DEEP_S3_POL.DPS3_EN_AC = 0;
      /// DEEP_S4_POL.DPS4_EN_DC = 0; DEEP_S4_POL.DPS4_EN_AC = 0;
      /// DEEP_S5_POL.DPS5_EN_DC = 1; DEEP_S5_POL.DPS5_EN_AC = 0;
      ///
      S3Data32  = 0;
      S4Data32  = 0;
      S5Data32  = (UINT32) (B_PMC_PWRM_S5_PWRGATE_POL_S5DC_GATE_SUS);
      break;

    case PchDpS4S5BatteryEn:
      ///
      /// Configuration 3: Enabled only in S4-S5/Battery Mode
      /// DEEP_S3_POL.DPS3_EN_DC = 0; DEEP_S3_POL.DPS3_EN_AC = 0;
      /// DEEP_S4_POL.DPS4_EN_DC = 1; DEEP_S4_POL.DPS4_EN_AC = 0;
      /// DEEP_S5_POL.DPS5_EN_DC = 1; DEEP_S5_POL.DPS5_EN_AC = 0;
      ///
      S3Data32  = 0;
      S4Data32  = (UINT32) (B_PMC_PWRM_S4_PWRGATE_POL_S4DC_GATE_SUS);
      S5Data32  = (UINT32) (B_PMC_PWRM_S5_PWRGATE_POL_S5DC_GATE_SUS);
      break;

    case PchDpS3S4S5BatteryEn:
      ///
      /// Configuration 5: Enabled only in S4-S5/Battery Mode
      /// DEEP_S3_POL.DPS3_EN_DC = 1; DEEP_S3_POL.DPS3_EN_AC = 0;
      /// DEEP_S4_POL.DPS4_EN_DC = 1; DEEP_S4_POL.DPS4_EN_AC = 0;
      /// DEEP_S5_POL.DPS5_EN_DC = 1; DEEP_S5_POL.DPS5_EN_AC = 0;
      ///
      S3Data32  = (UINT32) (B_PMC_PWRM_S3_PWRGATE_POL_S3DC_GATE_SUS);
      S4Data32  = (UINT32) (B_PMC_PWRM_S4_PWRGATE_POL_S4DC_GATE_SUS);
      S5Data32  = (UINT32) (B_PMC_PWRM_S5_PWRGATE_POL_S5DC_GATE_SUS);
      break;

    case PchDeepSxPolDisable:
    default:
      ///
      /// Configuration 5: DeepSx Disabled
      /// DEEP_S3_POL.DPS3_EN_DC = 0; DEEP_S3_POL.DPS3_EN_AC = 0;
      /// DEEP_S4_POL.DPS4_EN_DC = 0; DEEP_S4_POL.DPS4_EN_AC = 0;
      /// DEEP_S5_POL.DPS5_EN_DC = 0; DEEP_S5_POL.DPS5_EN_AC = 0;
      ///
      S3Data32  = 0;
      S4Data32  = 0;
      S5Data32  = 0;
      break;
  }
  MmioWrite32 ((PchPwrmBase + R_PMC_PWRM_S3_PWRGATE_POL), S3Data32);
  MmioWrite32 ((PchPwrmBase + R_PMC_PWRM_S4_PWRGATE_POL), S4Data32);
  MmioWrite32 ((PchPwrmBase + R_PMC_PWRM_S5_PWRGATE_POL), S5Data32);
}

/**
  Configure PCH to CPU energy report

  @param[in] PwrmBase      PwrmBase
  @param[in] PmConfig      PM config
  @param[in] PmcSocConfig  PMC SoC configuration
**/
STATIC
VOID
ConfigureEnergyReport (
  IN UINTN           PwrmBase,
  IN PCH_PM_CONFIG   *PmConfig,
  IN PMC_SOC_CONFIG  *PmcSocConfig
  )
{
  if (PmConfig->DisableEnergyReport) {
    //
    // Enable Energy Reporting
    // PM_CFG (0x1818h[2]) = 0b
    //
    MmioAnd32 (PwrmBase + R_PMC_PWRM_PM_CFG, (UINT32)(~B_PMC_PWRM_PM_CFG_ER_EN));
  } else {
    //
    // Enable Energy Reporting
    // PM_CFG (0x1818h[2]) = 1b
    //
    MmioOr32 (PwrmBase + R_PMC_PWRM_PM_CFG, B_PMC_PWRM_PM_CFG_ER_EN);
  }

  if (PmConfig->PmErDebugMode) {
    MmioOr32 (PwrmBase + R_PMC_PWRM_ETR3, B_PMC_PWRM_ETR3_ER_DEBUG_EN);
  } else {
    MmioAnd32 (PwrmBase + R_PMC_PWRM_ETR3, (UINT32) (~B_PMC_PWRM_ETR3_ER_DEBUG_EN));
  }

  //
  // Set Lock bit
  //
  MmioOr32 (PwrmBase + R_PMC_PWRM_PM_CFG, B_PMC_PWRM_PM_CFG_ER_LOCK);
}

/**
  This function sets IRQ number used for SCI interrupt

  @param[in] PwrmBase         PMC MMIO
  @param[in] Irq              Irq number (9, 10, 11, 20, 21, 22, 23)
**/
VOID
PmcSetSciIrq (
  IN UINTN   PwrmBase,
  IN UINT8   Irq
  )
{
  UINT32  Data32Or;
  //
  // IRQx    Value to set in register
  // IRQ9    000b
  // IRQ10   001b
  // IRQ11   010b
  // IRQ20   100b
  // IRQ21   101b
  // IRQ22   110b
  // IRQ23   111b
  //
  switch (Irq) {
    case 9:  Data32Or = V_PMC_PWRM_ACPI_SCIS_IRQ9;  break;
    case 10: Data32Or = V_PMC_PWRM_ACPI_SCIS_IRQ10; break;
    case 11: Data32Or = V_PMC_PWRM_ACPI_SCIS_IRQ11; break;
    case 20: Data32Or = V_PMC_PWRM_ACPI_SCIS_IRQ20; break;
    case 21: Data32Or = V_PMC_PWRM_ACPI_SCIS_IRQ21; break;
    case 22: Data32Or = V_PMC_PWRM_ACPI_SCIS_IRQ21; break;
    case 23: Data32Or = V_PMC_PWRM_ACPI_SCIS_IRQ23; break;
    default:
      //
      // Unsupported SCI IRQ
      //
      return;
  }

  MmioAndThenOr32 (
    PwrmBase + R_PMC_PWRM_ACTL,
    ~(UINT32)B_PMC_PWRM_ACTL_SCIS,
    Data32Or
    );
}

/**
  Configure Dirty Warm Reset feature
  Global Reset converted to Host Reset feature configuration

  @param[in] PmcHandle      PMC handle

**/
STATIC
VOID
PmcConfigureDirtyWarmReset (
  IN PMC_HANDLE  *PmcHandle
  )
{
  if (PmcHandle->PmConfig->DirtyWarmResetEnable == FALSE) {
    return;
  }
  //
  // Enable Globa2Host feature
  //
  MmioOr32 (PmcHandle->PwrmBase + R_PMC_PWRM_GBL2HOST_EN, B_PMC_PWRM_GBL2HOST_EN_HOST_RESET_TIMEOUT);

  //
  // Enable Sys Reset# Suppression
  //   Suppress (ignore) SYS_RESET# input during a Dirty Warm Reset.
  //   SYS_RESET# triggered Resets will be ignored if this bit is set.
  //
  MmioOr32(PmcHandle->PwrmBase + R_PMC_PWRM_PM_CFG3, B_PMC_PWRM_PM_CFG3_SYS_RST_SUP);
}

/**
  Configures fabric power gating settings

  @param[in] PwrmBase      PMC MMIO
  @param[in] PmcSocConfig  Pointer to PMC SOC configuration.
**/
STATIC
VOID
PmcConfigureFabricPowerGating (
  IN UINTN           PwrmBase,
  IN PMC_SOC_CONFIG  *PmcSocConfig
  )
{
  UINT32  Data32Or;
  UINT32  Data32And;

  if (PmcSocConfig->AllPsfIdleOverride) {
    Data32Or = 2;
    Data32And = (UINT32) ~(BIT31 | BIT29 | (0x1F << 6) | 0xF);
  } else if (PmcSocConfig->FabricPowerGatingCppmQualificationEnable) {
    Data32Or = BIT31 | BIT30 | (2 << 6) | 2;
    Data32And = (UINT32) ~(BIT29 | (0x1F << 6) | 0xF);
  } else {
    Data32Or = BIT31 | 2;
    Data32And = (UINT32) ~(BIT29 | (0x1F << 6) | 0xF);
  }
  MmioAndThenOr32 (
    PwrmBase + R_PMC_PWRM_FAB_PG_CTL_PSF,
    Data32And,
    Data32Or
    );

  Data32Or = (B_PMC_PWRM_FAB_PG_CTL_SBR_CX_QUAL_EN_SBR | (2 << 6) | 2);
  if (PmcSocConfig->AllSbrIdleQualifierEnable) {
    Data32Or |= B_PMC_PWRM_FAB_PG_CTL_SBR_ALL_SBR_IDLE_QUAL_EN_SBR;
  }
  MmioAndThenOr32 (
    PwrmBase + R_PMC_PWRM_FAB_PG_CTL_SBR,
    (UINT32) ~((0x1F << 6) | 0xF),
    Data32Or
    );
}

/**
  Configure Low Power Mode S0ix sub-state support

  @param[in] PwrmBase      Power management base.
  @param[in] PmConfig      Pointer to PM config.
  @param[in] PmcSocConfig  Pointer to PMC SOC configuration.
**/
VOID
PmcConfigureLpmS0ixSupport (
  IN UINTN           PwrmBase,
  IN PCH_PM_CONFIG   *PmConfig,
  IN PMC_SOC_CONFIG  *PmcSocConfig
  )
{
  PMC_LPM_S0IX_SUB_STATE_EN  LpmS0ixSubStateEnable;
  UINT32                     RegisterOffset;

  //
  // LPM_EN[0] - LPM0 - s0i2.0
  // LPM_EN[1] - LPM1 - s0i2.1
  // LPM_EN[2] - LPM2 - s0i2.2. Requires control of v1p05-PHY FET using EXT_PWR_GATE# pin
  // LPM_EN[3] - LPM3 - s0i3.0
  // LPM_EN[4] - LPM4 - s0i3.1
  // LPM_EN[5] - LPM5 - s0i3.2
  // LPM_EN[6] - LPM6 - s0i3.3. Requires control of v1p05-PHY FET using EXT_PWR_GATE# pin
  // LPM_EN[7] - LPM7 - s0i3.4. Requires control of v1p05-IS FET using EXT_PWR_GATE2# pin
  //
  LpmS0ixSubStateEnable.Val = PmConfig->LpmS0ixSubStateEnable.Val;

  //
  // Respective S0ix sub-states need to be disabled if
  // related v1p05-PHY and v1p05-IS FETs are not supported by platform
  //
  if (!PmConfig->V1p05PhyExtFetControlEn) {
    LpmS0ixSubStateEnable.Field.S0i2p2En = 0;
    LpmS0ixSubStateEnable.Field.S0i3p3En = 0;
  }
  if (!PmConfig->V1p05IsExtFetControlEn) {
    LpmS0ixSubStateEnable.Field.S0i3p4En = 0;
  }

  switch (PmcSocConfig->LpmInterfaceVersion) {
  case 4:
    RegisterOffset = R_PMC_PWRM_LPM_EN_V4;
    break;
  case 3:
    RegisterOffset = R_PMC_PWRM_LPM_EN_V3;
    break;
  case 2:
    RegisterOffset = R_PMC_PWRM_LPM_EN_V2;
    break;
  case 1:
  default:
    RegisterOffset = R_PMC_PWRM_LPM_EN;
    break;
  }
  MmioAndThenOr32 (PwrmBase + RegisterOffset, (UINT32) ~V_PMC_PWRM_LPM_EN, LpmS0ixSubStateEnable.Val);

  //
  // LPM_PRI[3:0]   - LPM0 - s0i2.0
  // LPM_PRI[7:4]   - LPM1 - s0i2.1
  // LPM_PRI[11:8]  - LPM2 - s0i2.2
  // LPM_PRI[15:12] - LPM3 - s0i3.0
  // LPM_PRI[19:16] - LPM4 - s0i3.1
  // LPM_PRI[23:20] - LPM5 - s0i3.2
  // LPM_PRI[27:24] - LPM6 - s0i3.3
  // LPM_PRI[31:28] - LPM7 - s0i3.4
  //
  if (PmcSocConfig->LpmPriorityConfigurable) {
    switch (PmcSocConfig->LpmInterfaceVersion) {
    case 4:
      RegisterOffset = R_PMC_PWRM_LPM_PRI_V4;
      break;
    case 3:
      RegisterOffset = R_PMC_PWRM_LPM_PRI_V3;
      break;
    case 2:
      RegisterOffset = R_PMC_PWRM_LPM_PRI_V2;
      break;
    case 1:
    default:
      RegisterOffset = R_PMC_PWRM_LPM_PRI;
      break;
    }
    MmioAndThenOr32 (PwrmBase + RegisterOffset, ~(UINT32)(V_PMC_PWRM_LPM_PRI_MASK), PmcSocConfig->LpmPriVal);
  }
}

/**
  Programs PMC LPM

  @param[in] PwrmBase      Power management base.
  @param[in] PmConfig      Pointer to PM config.
  @param[in] PmcSocConfig  Pointer to PMC SOC configuration.
**/
VOID
PmcConfigureLpm (
  IN UINTN           PwrmBase,
  IN PCH_PM_CONFIG   *PmConfig,
  IN PMC_SOC_CONFIG  *PmcSocConfig
  )
{
  UINT32  RegisterOffset;

  if (!PmcSocConfig->LpmSupported) {
    return;
  }

  if (PmConfig->S0ixAutoDemotion) {
    switch (PmcSocConfig->LpmInterfaceVersion) {
    case 4:
    case 3:
      RegisterOffset = R_PMC_PWRM_LPM_ADEM_HOST_CTRL_V3;
      break;
    case 2:
      RegisterOffset = R_PMC_PWRM_LPM_ADEM_HOST_CTRL_V2;
      break;
    case 1:
    default:
      RegisterOffset = R_PMC_PWRM_LPM_ADEM_HOST_CTRL;
      break;
    }
    MmioOr32 (
      PwrmBase + RegisterOffset,
      B_PMC_PWRM_LPM_ADEM_HOST_CTRL_LPM_ADEM_HOST_ENABLE
      );
  }

  PmcConfigureLpmS0ixSupport (PwrmBase, PmConfig, PmcSocConfig);
}

/**
  Enable OS IDLE Mode

  @param[in] PwrmBase  PMC MMIO
**/
VOID
PmcEnableOsIdleMode (
  IN UINTN  PwrmBase
  )
{
  MmioOr32 (PwrmBase + R_PMC_PWRM_WINIDLE_BIOS_ENABLE, B_PMC_PWRM_WINIDLE_BIOS_ENABLE_WI_ENABLE);
}

/**
  Lock OS IDLE Mode register

  @param[in] PwrmBase  PMC MMIO
**/
VOID
PmcLockOsIdleMode (
  IN UINTN  PwrmBase
  )
{
  MmioOr32 (PwrmBase + R_PMC_PWRM_WINIDLE_BIOS_ENABLE, B_PMC_PWRM_WINIDLE_BIOS_ENABLE_WI_LOCK);
}

/**
  Perform PCH PMC initialization

  @param[in] PmcHandle  Handle to PMC controller
**/
VOID
PmcInit (
  IN PMC_HANDLE  *PmcHandle
  )
{
  UINTN                     PchPwrmBase;
  UINT32                    Data32;
  UINT32                    Data32And;
  UINT32                    Data32Or;
  PCH_PM_CONFIG             *PmConfig;
  PMC_SOC_CONFIG            *PmcSocConfig;
  UINT32                    PmConA;
  UINT32                    PmConB;

  DEBUG ((DEBUG_INFO, "PmcInit start\n"));

  PmcSocConfig = PmcHandle->PmcSocConfig;
  PmConfig = PmcHandle->PmConfig;
  if (PmcSocConfig == NULL || PmConfig == NULL) {
    DEBUG ((DEBUG_ERROR, "PM policy can't be NULL\n"));
    return;
  }

  PchPwrmBase = PmcHandle->PwrmBase;
  //
  // Perform PM recommendation settings
  //

  ///
  /// GEN_PMCON_A[22] = 1, Allow OPI PLL Shutdown in C0 (ALLOW_OPI_PLL_SD_INC0)
  /// GEN_PMCON_A[20] = 1, Allow SPXB Clock Gating in C0 (ALLOW_SPXB_CG_INC0)
  /// GEN_PMCON_A[19] = 1, Allow L1.LOW Entry during C0 (ALLOW_L1LOW_C0)
  /// GEN_PMCON_A[17] = 1, Allow L1.LOW Entry with OPI Voltage ON (ALLOW_L1LOW_OPI_ON)
  /// GEN_PMCON_A[13] = 1, Allow L1.LOW Entry with CPU BCLK REQ Asserted (ALLOW_L1LOW_BCLKREQ_ON)
  ///
  //
  // Do not clear those RW/1C bits by accident.
  //
  MmioAndThenOr32 (
    PchPwrmBase + R_PMC_PWRM_GEN_PMCON_A,
    (UINT32) ~(B_PMC_PWRM_GEN_PMCON_A_GBL_RST_STS |
               B_PMC_PWRM_GEN_PMCON_A_MS4V |
               B_PMC_PWRM_GEN_PMCON_A_SUS_PWR_FLR |
               B_PMC_PWRM_GEN_PMCON_A_PF |
               B_PMC_PWRM_GEN_PMCON_A_HOST_RST_STS),
    B_PMC_PWRM_GEN_PMCON_A_ALLOW_OPI_PLL_SD_INC0 |
    B_PMC_PWRM_GEN_PMCON_A_ALLOW_SPXB_CG_INC0 |
    B_PMC_PWRM_GEN_PMCON_A_ALLOW_L1LOW_C0 |
    B_PMC_PWRM_GEN_PMCON_A_ALLOW_L1LOW_OPI_ON |
    B_PMC_PWRM_GEN_PMCON_A_ALLOW_L1LOW_BCLKREQ_ON
    );

  ///
  /// PWRMBASE + BM_CX_CNF[14, 10] to all 1
  ///
  if (PmcSocConfig->BreakCxConfigurable == TRUE) {
    MmioOr32 (
      PchPwrmBase + R_PMC_PWRM_BM_CX_CNF,
      B_PMC_PWRM_BM_CX_CNF_BM_STS_ZERO_EN
    );
  }

  if (PmcSocConfig->DisableIosfSbClockGating) {
    MmioAnd32 (
      PchPwrmBase + R_PMC_PWRM_PMC_PWR_CTL,
      (UINT32)~BIT7
      );
  }

  ///
  /// PWRMBASE + PM_CFG2[31:29]
  /// For PCH power button override period, 0=4sec, 011b=10sec
  ///
  Data32Or = (PmConfig->PwrBtnOverridePeriod) << N_PMC_PWRM_PM_CFG2_PBOP;
  ///
  /// PWRMBASE + PM_CFG2[28] for power button native mode disable
  ///
  if (PmConfig->DisableNativePowerButton) {
    Data32Or |= B_PMC_PWRM_PM_CFG2_PB_DIS;
  }
  //
  // Set debounce timer for pin that supports PWRBTN
  // Do this even if PWRBTN is not used in case debounce feature
  // of this pin is to be used for a different purposes
  //
  if (PmConfig->PowerButtonDebounce > 0 && PmcHandle->PmcCallback->PmcConfigurePowerButtonDebounce) {
    PmcHandle->PmcCallback->PmcConfigurePowerButtonDebounce (PmcHandle);
  }
  if (PmcHandle->PmcSocConfig->PwrCycDurLegacyOffset == TRUE) {
    Data32And = (UINT32)~(B_PMC_PWRM_PM_CFG2_PBOP | B_PMC_PWRM_PM_CFG2_PB_DIS);
  } else {
    Data32And = (UINT32)~(B_PMC_PWRM_PM_CFG2_PBOP | B_PMC_PWRM_PM_CFG2_PB_DIS | B_PMC_PWRM_PM_CFG2_PWR_CYC_DUR);
    Data32Or |= (UINT32) (PmConfig->PchPwrCycDur << N_PMC_PWRM_PM_CFG2_PWR_CYC_DUR);
  }
  MmioAndThenOr32 (PchPwrmBase + R_PMC_PWRM_PM_CFG2, Data32And, Data32Or);


  //
  // If CPU_C10_GATE# pin is used, PWRMBASE + 0x18E0[2] = 1
  // For wake on WLAN from S3/S4/S5, set PWRM_CFG3 [BIT17], HOST_WLAN_PP_EN = 1
  // For wake on WLAN from DeepSx S3/S4/S5, set PWRM_CFG3 [BIT16], DSX_WLAN_PP_EN = 1
  //
  Data32Or = 0x01040001;

  if (PmConfig->CpuC10GatePinEnable) {
    Data32Or |= B_PMC_PWRM_PM_CFG3_HOST_MISC_CORE_CFG_CPU_VCC_MAP;
  }

  if (PmConfig->WakeConfig.WoWlanEnable == TRUE) {
    Data32Or |= B_PMC_PWRM_PM_CFG3_HOST_WLAN_PP_EN;
    if (PmConfig->WakeConfig.WoWlanDeepSxEnable == TRUE) {
      Data32Or |= B_PMC_PWRM_PM_CFG3_DSX_WLAN_PP_EN;
    }
  }

  Data32And = (UINT32) B_PMC_PWRM_CFG3_BIOS_FIVR_DYN_EN;

  MmioAndThenOr32(
    PchPwrmBase + R_PMC_PWRM_PM_CFG3,
    Data32And,
    Data32Or
  );

  if (PmConfig->CpuC10GatePinEnable && PmcHandle->PmcCallback->PmcConfigureCpuC10Gate) {
    PmcHandle->PmcCallback->PmcConfigureCpuC10Gate (PmcHandle);
  }

  if (PmcSocConfig->LockCnviLdoCnf) {
    MmioOr32 (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_B, B_PMC_PWRM_GEN_PMCON_B_CNVLDOCFGLOCK);
  }

  ///
  /// For PCH-LP and USB2 PHY SUS Well Power Gating enabled:
  ///    PM_CFG4 (0x18E8h[30])  = 1, USB2 PHY SUS Well Power Gating Enable (U2_PHY_PG_EN)
  ///

  //
  // Set the CPU IOVR ramp duration according to SoC recommendation. The register
  // is in the unit of 10us.
  //
  Data32And = (UINT32) ~B_PMC_PWRM_PM_CFG4_CPU_IOVR_RAMP_DUR;
  Data32Or  = (PmcSocConfig->CpuIovrRampTime << N_PMC_PWRM_PM_CFG4_CPU_IOVR_RAMP_DUR);

  ///
  /// USB2 PHY SUS Well Power Gating must be done after USB2 PHY programming
  ///
  if (PmConfig->Usb2PhySusPgEnable) {
    Data32Or |= B_PMC_PWRM_PM_CFG4_U2_PHY_PG_EN;
  }

  MmioAndThenOr32 (
    PchPwrmBase + R_PMC_PWRM_PM_CFG4,
    Data32And,
    Data32Or
    );

  ///
  /// PWRMBASE + 0x1B14 = 0x1E0A4616
  ///
  if (PmcSocConfig->ObffConfigurable == TRUE) {
    MmioAndThenOr32 (PchPwrmBase + R_PMC_PWRM_OBFF_CFG, ~((UINT32)V_PMC_PWRM_OBFF_CFG_MASK), 0x1E0A4616);
  }

  if (PmcHandle->PmcCallback->PmcConfigureSlpS0) {
    PmcHandle->PmcCallback->PmcConfigureSlpS0 (PmcHandle);
  }

  PmcConfigureLpm (PchPwrmBase, PmConfig, PmcSocConfig);

  if (PmcSocConfig->FabricPowerGatingConfigurable == TRUE) {
    PmcConfigureFabricPowerGating (PchPwrmBase, PmcSocConfig);
  }

  if (PmcHandle->PmcCallback->PmcConfigureCppm) {
    PmcHandle->PmcCallback->PmcConfigureCppm (PmcHandle);
  }

  if (PmcSocConfig->SkipModPhyGatingPolicy) {
    ///
    /// CPPM_MPG_POL1A (0x10E0h[30])  = 1,    CPPM Shutdown Qualifier Enable for ModPHY (CPPM_MODPHY_QUAL)
    /// CPPM_MPG_POL1A (0x10E0h[29])  = 1,    ASLT/PLT Selection for ModPHY (LT_MODPHY_SEL)
    /// CPPM_MPG_POL1A (0x10E0h[8:0]) = 069h, LTR Treshold for ModPHY (LTR_MODPHY_THRESH)
    ///
    MmioAndThenOr32 (
      PchPwrmBase + R_PMC_PWRM_CPPM_MPG_POL1A,
      (UINT32) ~B_PMC_PWRM_CPPM_MPG_POL1A_LTR_MPHY_THRESH,
      B_PMC_PWRM_CPPM_MPG_POL1A_CPPM_MPHY_QUAL |
      B_PMC_PWRM_CPPM_MPG_POL1A_LT_MPHY_SEL |
      0x69
      );
  }

  if (PmConfig->LatchEventsC10Exit) {
    MmioOr32 (PchPwrmBase + R_PMC_PWRM_ETR3, B_PMC_PWRM_ETR3_LATCH_EVENTS_C10_EXIT);
  } else {
    MmioAnd32 (PchPwrmBase + R_PMC_PWRM_ETR3, (UINT32)~B_PMC_PWRM_ETR3_LATCH_EVENTS_C10_EXIT);
  }

  ///
  /// Handling Status Registers
  /// System BIOS must set 1b to clear the following registers during power-on
  /// and resuming from Sx sleep state.
  /// - PWRMBASE + PRSTS[0] = 1b
  /// - PWRMBASE + PRSTS[4] = 1b
  /// - PWRMBASE + PRSTS[5] = 1b
  ///
  Data32 = B_PMC_PWRM_PRSTS_PMC_HOST_WAKE_STS;
  ///
  /// Clear power / reset status bits on PCH Corporate
  ///
  if (PmConfig->MeWakeSts) {
    Data32 |= B_PMC_PWRM_PRSTS_ME_HOST_WAKE_STS;
  }
  if (PmConfig->WolOvrWkSts) {
    Data32 |= B_PMC_PWRM_PRSTS_WOL_OVR_WK_STS;
  }

  MmioOr32 (PchPwrmBase + R_PMC_PWRM_PRSTS, Data32);

  PmcConfigureDirtyWarmReset (PmcHandle);

  if (PmcHandle->PmcSocConfig->SleEnabled) {
    return;
  }

  ///
  /// We need to enable LAN_WAKE_PIN_DSX_EN for Wake from both SX and DSX
  ///
  Data32 = MmioRead32 (PchPwrmBase + R_PMC_PWRM_DSX_CFG);
  if (PmConfig->WakeConfig.LanWakeFromDeepSx == TRUE) {
    Data32 |= B_PMC_PWRM_DSX_CFG_LANWAKE_PIN_DSX_EN;
  } else {
    Data32 &= ~B_PMC_PWRM_DSX_CFG_LANWAKE_PIN_DSX_EN;
  }
  //
  // Disable PCH internal AC PRESENT pulldown
  //
  if (PmConfig->DisableDsxAcPresentPulldown) {
    Data32 |= B_PMC_PWRM_DSX_CFG_ACPRES_PD_DSX_DIS;
  } else {
    Data32 &= ~B_PMC_PWRM_DSX_CFG_ACPRES_PD_DSX_DIS;
  }
  ///
  /// Enable WAKE_PIN__DSX_EN for Wake
  ///
  if (PmConfig->WakeConfig.PcieWakeFromDeepSx) {
    Data32 |= B_PMC_PWRM_DSX_CFG_WAKE_PIN_DSX_EN;
  } else {
    Data32 &= ~B_PMC_PWRM_DSX_CFG_WAKE_PIN_DSX_EN;
  }
  MmioWrite32 (PchPwrmBase + R_PMC_PWRM_DSX_CFG, Data32);

  ///
  /// Handle wake policy
  ///
  PmConA = MmioRead32 (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_A);
  PmConB = MmioRead32 (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_B);

  //
  // Don't clear B_PMC_PWRM_GEN_PMCON_A_GBL_RST_STS, B_PMC_PWRM_GEN_PMCON_A_MS4V, B_PMC_PWRM_GEN_PMCON_A_SUS_PWR_FLR
  // B_PMC_PWRM_GEN_PMCON_A_PF, B_PMC_PWRM_GEN_PMCON_A_HOST_RST_STS by accident since those are RW/1C.
  //
  PmConA &= (UINT32) ~(B_PMC_PWRM_GEN_PMCON_A_GBL_RST_STS | B_PMC_PWRM_GEN_PMCON_A_MS4V | B_PMC_PWRM_GEN_PMCON_A_SUS_PWR_FLR | B_PMC_PWRM_GEN_PMCON_A_PF | B_PMC_PWRM_GEN_PMCON_A_HOST_RST_STS);

  PmConA &= (UINT32) ~(B_PMC_PWRM_GEN_PMCON_A_PME_B0_S5_DIS);
  PmConB &= (UINT32) ~(B_PMC_PWRM_GEN_PMCON_B_WOL_EN_OVRD);

  if (PmConfig->WakeConfig.PmeB0S5Dis) {
    PmConA |= B_PMC_PWRM_GEN_PMCON_A_PME_B0_S5_DIS;
  }

  if (PmConfig->WakeConfig.WolEnableOverride) {
    ///
    /// Wake-On-LAN (WOL) Implementation
    /// Step 1
    /// Clear PWRMBASE + GEN_PMCON_A[30] = 0b to ensure the LAN PHY will be powered for WOL
    /// when the power source is either the AC or the DC battery.
    ///
    PmConA &= (UINT32) ~B_PMC_PWRM_GEN_PMCON_A_DC_PP_DIS;

    ///
    /// Step 2
    /// Clear PWRMBASE + GEN_PMCON_A[29] = 0b to ensure the LAN PHY will be powered for WOL in Deep Sx.
    ///
    PmConA &= (UINT32) ~B_PMC_PWRM_GEN_PMCON_A_DSX_PP_DIS;

    ///
    /// Step 3
    /// Set PWRMBASE + GEN_PMCON_A[28] = 1b to ensure the LAN PHY will be powered for WOL after a G3 transition.
    ///
    PmConA |= (UINT32) B_PMC_PWRM_GEN_PMCON_A_AG3_PP_EN;

    ///
    /// Step 4
    /// Set PWRMBASE + GEN_PMCON_A[27] = 1b to ensure the LAN PHY will be powered for WOL from Sx.
    ///
    PmConA |= (UINT32) B_PMC_PWRM_GEN_PMCON_A_SX_PP_EN;

    ///
    /// Step 5
    /// "PME_B0_EN", ABASE + Offset 28h[13], bit must be programmed to enable wakes
    /// from S1-S4 at the Power Management Controller
    /// Done in ASL code(_PRW)
    ///
    ///
    /// Step 6
    /// Set "WOL Enable Override", PWRMBASE + GEN_PMCON_B[13], bit to 1b to guarantee the
    /// LAN-Wakes are enabled at the Power Management Controller, even in surprise
    /// S5 cases such as power loss/return and Power Button Override
    ///
    PmConB |= B_PMC_PWRM_GEN_PMCON_B_WOL_EN_OVRD;

    ///
    /// Step 7
    /// Moreover, system BIOS also require to enables in the LAN device by performing
    /// the WOL configuration requirements in the GbE region of the SPI flash.
    /// Done in PchSmmSxGoToSleep() SMM handler.
    ///
  } else {
    ///
    /// PWRMBASE + GEN_PMCON_A[30:27] and PWRMBASE + GEN_PMCON_B[13] are all in RTC or DSW well, so BIOS also
    /// needs to program them while WOL setup option is disabled.
    ///
    PmConA &= (UINT32) ~(B_PMC_PWRM_GEN_PMCON_A_AG3_PP_EN | B_PMC_PWRM_GEN_PMCON_A_SX_PP_EN);
    PmConA |= (B_PMC_PWRM_GEN_PMCON_A_DC_PP_DIS | B_PMC_PWRM_GEN_PMCON_A_DSX_PP_DIS);

    PmConB &= (UINT32) ~(B_PMC_PWRM_GEN_PMCON_B_WOL_EN_OVRD);
  }

  ///
  /// Configure On DC PHY Power Disable according to policy SlpLanLowDc.
  /// When this bit is set, SLP_LAN# will be driven low when ACPRESENT is low.
  /// This indicates that LAN PHY should be powered off on battery mode.
  /// This will override the DC_PP_DIS setting by WolEnableOverride.
  ///
  if (PmConfig->SlpLanLowDc) {
    PmConA |= (UINT32) (B_PMC_PWRM_GEN_PMCON_A_DC_PP_DIS);
  } else {
    PmConA &= (UINT32) ~(B_PMC_PWRM_GEN_PMCON_A_DC_PP_DIS);
  }

  ///
  /// Enabling SLP_S3# and SLP_S4# Stretch
  /// PWRMBASE + GEN_PMCON_A[12:10]
  /// PWRMBASE + GEN_PMCON_A[5:3]
  ///
  PmConA &= (UINT32) ~(B_PMC_PWRM_GEN_PMCON_A_SLP_S3_MIN_ASST_WDTH | B_PMC_PWRM_GEN_PMCON_A_S4MAW);

  switch (PmConfig->PchSlpS3MinAssert) {
    case PchSlpS360us:
      PmConA |= V_PMC_PWRM_GEN_PMCON_A_SLP_S3_MAW_60US;
      break;

    case PchSlpS31ms:
      PmConA |= V_PMC_PWRM_GEN_PMCON_A_SLP_S3_MAW_1MS;
      break;

    case PchSlpS350ms:
    default:
      PmConA |= V_PMC_PWRM_GEN_PMCON_A_SLP_S3_MAW_50MS;
      break;

    case PchSlpS32s:
      PmConA |= V_PMC_PWRM_GEN_PMCON_A_SLP_S3_MAW_2S;
      break;
  }

  switch (PmConfig->PchSlpS4MinAssert) {
    case PchSlpS4PchTime:
      PmConA &= (UINT32) (~B_PMC_PWRM_GEN_PMCON_A_S4ASE);
      break;

    case PchSlpS41s:
      PmConA |= V_PMC_PWRM_GEN_PMCON_A_SLP_S4_MAW_1S | B_PMC_PWRM_GEN_PMCON_A_S4ASE;
      break;

    case PchSlpS42s:
      PmConA |= V_PMC_PWRM_GEN_PMCON_A_SLP_S4_MAW_2S | B_PMC_PWRM_GEN_PMCON_A_S4ASE;
      break;

    case PchSlpS43s:
      PmConA |= V_PMC_PWRM_GEN_PMCON_A_SLP_S4_MAW_3S | B_PMC_PWRM_GEN_PMCON_A_S4ASE;
      break;

    case PchSlpS44s:
    default:
      PmConA |= V_PMC_PWRM_GEN_PMCON_A_SLP_S4_MAW_4S | B_PMC_PWRM_GEN_PMCON_A_S4ASE;
      break;
  }

  if (PmConfig->SlpStrchSusUp == FALSE) {
    PmConA |= B_PMC_PWRM_GEN_PMCON_A_DIS_SLP_X_STRCH_SUS_UP;
  } else {
    PmConA &= (UINT32)~B_PMC_PWRM_GEN_PMCON_A_DIS_SLP_X_STRCH_SUS_UP;
  }
  MmioWrite32 (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_A, PmConA);
  MmioWrite32 (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_B, PmConB);

  ///
  /// Enable OS Idle Mode   : PWRMBASE + 0x1400[0]  = 1b
  /// Lock WinIdle Register : PWRMBASE + 0x1400[31] = 1b
  ///
  ///
  if (PmcSocConfig->OsIdleSupported) {
    if (PmConfig->OsIdleEnable) {
      PmcEnableOsIdleMode (PchPwrmBase);
    }
    PmcLockOsIdleMode (PchPwrmBase);
  }

  ///
  /// Set CF9LOCK (PWRMBASE + ETR3[31]  = 1b)
  /// Done in Intel Management Engine Framework Reference Code
  ///
  ///
  /// Note: PWRMBASE + PM_CFG[19:16] are platform dependent settings (0Fh provides longest assertion),
  /// please consult with your board design engineers for correct values to be programmed to.
  ///
  /// For PWRMBASE + PM_CFG[9:8] Reset Power Cycle Duration could be customized, please refer to EDS
  /// and make sure the setting correct, which never less than the following register.
  /// - GEN_PMCON_3.SLP_S3_MIN_ASST_WDTH
  /// - GEN_PMCON_3.SLP_S4_MIN_ASST_WDTH
  /// - PM_CFG.SLP_A_MIN_ASST_WDTH
  /// - PM_CFG.SLP_LAN_MIN_ASST_WDTH
  ///
  Data32 = MmioRead32 (PchPwrmBase + R_PMC_PWRM_PM_CFG);
  if (PmcSocConfig->PwrCycDurLegacyOffset == FALSE) {
    Data32 &= (UINT32) ~(B_PMC_PWRM_PM_CFG_SLP_SUS_MIN_ASST_WDTH | B_PMC_PWRM_PM_CFG_SLP_A_MIN_ASST_WDTH);
  } else {
    Data32 &= (UINT32) ~(B_PMC_PWRM_PM_CFG_SLP_SUS_MIN_ASST_WDTH | B_PMC_PWRM_PM_CFG_SLP_A_MIN_ASST_WDTH | B_PMC_PWRM_PM_CFG_PWR_CYC_DUR_LEGACY);
    switch (PmConfig->PchPwrCycDur) {
      case 0:  // treat as PCH default
        Data32 |= V_PMC_PWRM_PM_CFG_RPCD_4S;
        break;
      case 1:
        Data32 |= V_PMC_PWRM_PM_CFG_RPCD_1S;
        break;
      case 2:
        Data32 |= V_PMC_PWRM_PM_CFG_RPCD_2S;
        break;
      case 3:
        Data32 |= V_PMC_PWRM_PM_CFG_RPCD_3S;
        break;
      case 4:
        Data32 |= V_PMC_PWRM_PM_CFG_RPCD_4S;
        break;
      default:
        Data32 |= V_PMC_PWRM_PM_CFG_RPCD_4S;
        DEBUG ((DEBUG_WARN, "Invalid value for PchPwrCycDur. Using 4Sec as the default value.\n"));
        break;
    }
  }
  switch (PmConfig->PchSlpSusMinAssert) {
    case PchSlpSus0ms:
      Data32 |= V_PMC_PWRM_PM_CFG_SSMAW_0S;
      break;

    case PchSlpSus500ms:
      Data32 |= V_PMC_PWRM_PM_CFG_SSMAW_0_5S;
      break;

    case PchSlpSus1s:
      Data32 |= V_PMC_PWRM_PM_CFG_SSMAW_1S;
      break;

    case PchSlpSus4s:
    default:
      Data32 |= V_PMC_PWRM_PM_CFG_SSMAW_4S;
      break;
  }
  switch (PmConfig->PchSlpAMinAssert) {
    case PchSlpA0ms:
      Data32 |= V_PMC_PWRM_PM_CFG_SAMAW_0S;
      break;

    case PchSlpA4s:
      Data32 |= V_PMC_PWRM_PM_CFG_SAMAW_4S;
      break;

    case PchSlpA98ms:
      Data32 |= V_PMC_PWRM_PM_CFG_SAMAW_98ms;
      break;

    case PchSlpA2s:
    default:
      Data32 |= V_PMC_PWRM_PM_CFG_SAMAW_2S;
      break;
  }

  //
  // Disable Global Reset on uncorrectable error on PMC SRAM interface. PM_CFG[23] = 0 for TGP+
  // Allow 24MHz Crystal Oscillator Shutdown, Please note: below programming must be done after USB2 GLOBAL PORT 2 programming
  // Allow USB2 PHY Core Power Gating
  // PMC reads disabling is moved to PchOnPciEnumCompleteCommon()
  //
  Data32 |= (B_PMC_PWRM_PM_CFG_ALLOW_USB2_CORE_PG | B_PMC_PWRM_PM_CFG_ALLOW_24_OSC_SD);

  //
  // Clear CPU OC Strap
  //
  Data32 &= (UINT32) ~(B_PMC_PWRM_PM_CFG_COCS);
  MmioWrite32 (PchPwrmBase + R_PMC_PWRM_PM_CFG, Data32);

  if ((PmcSocConfig->PowerOptimizerEnable == TRUE) && (PmcSocConfig->ObffConfigurable == TRUE)) {
    Data32 = MmioRead32 (PchPwrmBase + R_PMC_PWRM_OBFF_CFG);
    if (PmcSocConfig->ClientObffEn == FORCE_DISABLE) {
      Data32 &= (UINT32) ~(BIT4);
    }
    if (PmcSocConfig->CxObffEntryDelay == FORCE_ENABLE) {
      Data32 |= (UINT32) (BIT3);
    }
    if (PmcSocConfig->MemCloseStateEn == FORCE_DISABLE) {
      Data32 &= (UINT32) ~(BIT2);
    }
    if (PmcSocConfig->InternalObffEn == FORCE_DISABLE) {
      Data32 &= (UINT32) ~(BIT1);
    }
    if (PmcSocConfig->ExternalObffEn == FORCE_ENABLE) {
      Data32 |= BIT0;
    }
    MmioWrite32 (PchPwrmBase + R_PMC_PWRM_OBFF_CFG, Data32);
  }

  //
  // Configure VrAlert GPIO pin and lock
  //
  if (PmConfig->VrAlert) {
    MmioOr32 (PchPwrmBase + R_PMC_PWRM_PMC_THROT_1, B_PMC_PWRM_PMC_THROT_1_VRALERT_EN);
    if (PmcHandle->PmcCallback->PmcConfigureVrAlert) {
      PmcHandle->PmcCallback->PmcConfigureVrAlert (PmcHandle);
    }
  } else {
    MmioAnd32 (PchPwrmBase + R_PMC_PWRM_PMC_THROT_1, ~(UINT32)B_PMC_PWRM_PMC_THROT_1_VRALERT_EN);
  }
  MmioOr32 (PchPwrmBase + R_PMC_PWRM_PMC_THROT_1, B_PMC_PWRM_PMC_THROT_1_PMC_THROT_LOCK);

  //
  // Configure Energy Report
  //
  ConfigureEnergyReport (PchPwrmBase, PmConfig, PmcSocConfig);

  //
  // Deep Sx Enabling
  //
  ConfigureDeepSxSettings (PchPwrmBase, PmConfig);

  //
  // Configure SCI interrupt
  //
  PmcSetSciIrq (PchPwrmBase, PmcSocConfig->SciIrq);

  //
  // Configure WDT setting
  //
  if (PmcSocConfig->PmcWdtCmdSupported) {
    if (PmConfig->PmcWdtTimerEn) {
      PmcEnableWdt (PchPwrmBase);
    } else {
      PmcDisableWdt (PchPwrmBase);
    }
  }
}
