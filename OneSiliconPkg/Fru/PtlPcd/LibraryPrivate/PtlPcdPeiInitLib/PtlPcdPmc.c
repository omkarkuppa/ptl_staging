/** @file
  PTL PCD PMC initialization

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

#include "PtlPcdInitPei.h"

#include <Library/ConfigBlockLib.h>
#include <ConfigBlock.h>
#include <Library/PeiItssLib.h>
#include <Register/PmcRegsFivr.h>
#include <Fru/PtlPcd/IncludePrivate/Register/PmcRegsPtlPcd.h>
#include <Library/EspiPrivateLib.h>
#include <Library/DciPrivateLib.h>
#include <Library/PchFiaLib.h>
#include <Library/PeiPmcPrivateLib2.h>
#include <Library/PmcSocLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/PcdInfoLib.h>
#include <P2SbController.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Register/PchRegs.h>
#include <PmcHandle.h>
#include <Register/GpioAcpiDefines.h>
#include <Library/PcdGpioNativeLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdFiaSocLib.h>
#include <Library/P2SbSocLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdFiaSocLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPsfSocLib.h>
#include <PcdSbPortIds.h>
#include <Library/Ptl/PcdInfoLib/PtlPcdInfoLib.h>


#define R_PTL_SOC_PMC_PWRM_DISABLE_DTS_IN_S0IX                      0x1580
#define B_PTL_SOC_PMC_PWRM_DISABLE_DTS_IN_S0IX_D_DTS                BIT0      ///< DISABLE_DTS_IN_S0IX
#define B_PTL_SOC_PMC_PWRM_DISABLE_DTS_IN_S0IX_E_DTS_TH_XING        BIT1      ///< ENABLE_DTS_THRESHOLD_XING_CHECK
#define B_PTL_SOC_PMC_PWRM_DISABLE_DTS_IN_S0IX_S0IXLOCK             BIT31
#define R_PTL_SOC_PMC_PWRM_FAB_PG_CTL_PSF_MASK                      0x17F0
#define R_PTL_SOC_PMC_PWRM_FAB_PG_CTL_SBR_MASK                      0x17F4

#define R_PTL_SOC_PMC_PWRM_VR_MISC_CTL                              0x1900

/**
  Configures which PM_SYNC messages are allowed to be
  sent over PM_SYNC interface.

  @param[in] PmcHandle  Pointer to PMC handle
**/
VOID
PtlPcdPmSyncInitMessages (
  IN PMC_HANDLE  *PmcHandle
  )
{
  ///
  /// PWRMBASE + 0x1814 [24,18,16,15,14,11,10,9,7,6,5,3,1,0] = all 1s = 0x0105CEEB
  ///
  MmioOr32 (
    PmcHandle->PwrmBase + R_PMC_PWRM_PMC_PWR_CTL,
    BIT24 | BIT18 | BIT16 | BIT15 | BIT14 | BIT11 | BIT10 | BIT9 | BIT7 | BIT6 | BIT5 | BIT3 | BIT1 | BIT0
  );
}

/**
  Configure CPPM acording to BWG

  @param[in] PmcHandle  Pointer to PMC handle
**/
VOID
PtlPcdPmcConfigureCppm (
  IN PMC_HANDLE  *PmcHandle
  )
{
  MmioAndThenOr32 (PmcHandle->PwrmBase + R_PMC_PWRM_CPPMFALIGNCTL_2, (UINT32) ~B_PMC_PWRM_CPPMFALIGNCTL_2_MASK, 0x00002EFB);

  MmioAndThenOr32 (PmcHandle->PwrmBase + R_PMC_PWRM_CPPMFALIGNCTL_1, (UINT32) ~B_PMC_PWRM_CPPMFALIGNCTL_1_MASK, 0xC80A8000);
}

/**
  Called after CPU C10 gate is configured on PMC side to
  finish up feature configuration on SoC side.

  @param[in] PmcHandle  Pointer to PMC handle
**/
VOID
PtlPcdPmcConfigurePowerButtonDebounce (
  IN PMC_HANDLE  *PmcHandle
  )
{
  EFI_STATUS      Status;
  GPIOV2_SERVICES *GpioServices;

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: retrieving GpioServices failed (Status: %d)\n", __FUNCTION__, Status));
    return;
  }

  Status = PtlPcdGpioSetDebounceTimerforPwrBtn (GpioServices, PmcHandle->PmConfig->PowerButtonDebounce);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: Setting pin failed (Status: %d)\n", __FUNCTION__, Status));
  }
}

/**
  Called after CPU C10 gate is configured on PMC side to
  finish up feature configuration on SoC side.

  @param[in] PmcHandle  Pointer to PMC handle
**/
VOID
PtlPcdPmcConfigureCpuC10Gate (
  IN PMC_HANDLE  *PmcHandle
  )
{
  EFI_STATUS      Status;
  GPIOV2_SERVICES *GpioServices;
    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: retrieving GpioServices failed (Status: %d)\n", __FUNCTION__, Status));
    return;
  }

  Status = PtlPcdGpioEnableCpuC10GatePin (GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: Setting pin failed (Status: %d)\n", __FUNCTION__, Status));
  }
}

/**
  Called after PMC is done configuring VR alert to finish up
  init on SoC side.

  @param[in] PmcHandle  Pointer to PMC handle
**/
VOID
PtlPcdPmcConfigureVrAlert (
  IN PMC_HANDLE  *PmcHandle
  )
{
  EFI_STATUS      Status;
  GPIOV2_SERVICES *GpioServices;

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: retrieving GpioServices failed (Status: %d)\n", __FUNCTION__, Status));
    return;
  }

  Status = PtlPcdGpioEnableVrAlert (GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: Setting pin failed (Status: %d)\n", __FUNCTION__, Status));
  }
}

/**
  Get the PMC SOC configuration.

  @param[out] PmcSocConfig  Configuration of the PMC
**/
VOID
PtlPcdPmcGetSocConfig (
  IN SI_POLICY_PPI    *SiPolicy,
  OUT PMC_SOC_CONFIG  *PmcSocConfig
  )
{
  UINT32                          PortNum;
  UINT32                          MaxUsb3PortNum;
  P2SB_CONTROLLER                 P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS   EspiPcrAccess;
  P2SB_SIDEBAND_REGISTER_ACCESS   DciPcrAccess;
  P2SB_PORT_16_ID                 P2SBPid;

  if (PmcSocConfig == NULL) {
    return;
  }

  P2SBPid.Pid16bit = PTL_SID_F2_PID_SPBC;
  PtlPcdGetP2SbController (&P2SbController, P2SBPid);
  BuildP2SbSidebandAccess (&P2SbController, P2SBPid.PortId.LocalPid, 0, P2SbPrivateConfig, P2SbMmioAccess, FALSE, &EspiPcrAccess);
  P2SBPid.Pid16bit = PTL_SID_F2_PID_EXI;
  PtlPcdGetP2SbController (&P2SbController, P2SBPid);
  BuildP2SbSidebandAccess (&P2SbController, P2SBPid.PortId.LocalPid, 0, P2SbPrivateConfig, P2SbMmioAccess, FALSE, &DciPcrAccess);

  ZeroMem (PmcSocConfig, sizeof (PMC_SOC_CONFIG));

  PmcSocConfig->CppmCgInterfaceVersion = 2;
  PmcSocConfig->LpmSupported = TRUE;
  PmcSocConfig->LpmInterfaceVersion = 3;
  PmcSocConfig->PmSyncMiscCfgInterfaceVersion = 2;
  PmcSocConfig->LpmPriorityConfigurable = FALSE;
  PmcSocConfig->LpmPriVal = 0;
  PmcSocConfig->OsIdleSupported = FALSE;
#if (FixedPcdGet8(PcdEmbeddedEnable) == 0x1)
  PmcSocConfig->TimedGpioSupported = TRUE;
#endif
  PmcSocConfig->CpuIovrRampTime = 0;
  PmcSocConfig->PsOnSupported = FALSE;
  PmcSocConfig->ModPhySusPgSupported = FALSE;
  PmcSocConfig->SciIrq = ItssGetPmcSciDevIntConfig (SiPolicy);
  PmcSocConfig->UsbDbcConnected = IsPchUsbDbcConnected ((REGISTER_ACCESS*) &DciPcrAccess);
  PmcSocConfig->AllSbrIdleQualifierEnable = TRUE;
  PmcSocConfig->ObffConfigurable = FALSE;
  PmcSocConfig->FabricPowerGatingConfigurable = FALSE;
  PmcSocConfig->LockFivr = TRUE;
  PmcSocConfig->LockCnviLdoCnf = TRUE;
  PmcSocConfig->BreakCxConfigurable = FALSE;
  PmcSocConfig->PwrCycDurLegacyOffset = FALSE;
  PmcSocConfig->AllPsfIdleOverride = FALSE;
  PmcSocConfig->PmcWdtCmdSupported = TRUE;

  MaxUsb3PortNum = GetPchXhciMaxUsb3PortNum ();
  for (PortNum = 0; PortNum < MaxUsb3PortNum; PortNum++) {
    if (PtlPcdFiaIsUsb3PortConnected (PortNum)) {
      PmcSocConfig->Usb3LanesConnectedBitmask |= (1 << PortNum);
    }
  }
}

/**
  Build a PMC handle for PTL SoC

  @param[in]  SiPolicy      SiPolicyPpi instance
  @param[in]  PmcSocConfig  Pointer to PMC SoC configuration
  @param[in]  PmcCallbacks  Pointer to PMC SoC callbacks
  @param[in]  PmConfig      Pointer to PMC platform configuration
  @param[out] PmcHandle     On output pointer to initialized PMC handle
**/
VOID
PtlPcdPmcGetHandle (
  IN  SI_POLICY_PPI   *SiPolicy,
  IN  PMC_SOC_CONFIG  *PmcSocConfig,
  IN  PMC_CALLBACK    *PmcCallback,
  IN  PCH_PM_CONFIG   **PmConfig,
  OUT PMC_HANDLE      *PmcHandle
  )
{
  EFI_STATUS  Status;

  PtlPcdPmcGetSocConfig (SiPolicy, PmcSocConfig);

  Status = GetConfigBlock ((VOID *) SiPolicy, &gPmConfigGuid, (VOID *) PmConfig);
  ASSERT_EFI_ERROR (Status);

  PmcCallback->PmcConfigureSlpS0 = NULL;
  PmcCallback->PmcConfigureCppm = PtlPcdPmcConfigureCppm;
  PmcCallback->PmcConfigurePowerButtonDebounce = PtlPcdPmcConfigurePowerButtonDebounce;
  PmcCallback->PmcConfigureCpuC10Gate = PtlPcdPmcConfigureCpuC10Gate;
  PmcCallback->PmcConfigureVrAlert = PtlPcdPmcConfigureVrAlert;

  PmcHandle->PwrmBase = PmcGetPwrmBase ();
  PmcHandle->PmcSocConfig = PmcSocConfig;
  PmcHandle->PmConfig = *PmConfig;
  PmcHandle->PmcCallback = PmcCallback;
}

/**
  Clear GPE1 TBT LSX Status Bit
**/
VOID
PtlGpe1LsxStatusClear (
  )
{
  UINT16  ABase;
  UINT32  Gpe1Status;

  ABase = PmcGetAcpiBase ();
  Gpe1Status = IoRead32 (ABase + R_ACPI_IO_GPE1_STS_31_0);
  if ((Gpe1Status & B_ACPI_IO_GPE1_STS_31_0_TBTLSX_PME_B0_STS) != 0) {
    //
    // The GPE Status bit is write clear. The request is clear TBT_LSX Bit, only.
    // So that write the B_ACPI_IO_GPE1_STS_31_0_TBTLSX_PME_B0_STS.
    //
    DEBUG ((DEBUG_INFO, "Gpe1Status value = 0x%x\n", Gpe1Status));
    IoWrite32 (ABase + R_ACPI_IO_GPE1_STS_31_0, B_ACPI_IO_GPE1_STS_31_0_TBTLSX_PME_B0_STS);
  }
  return;
}

/**
  Initialize PMC controller.

  @param[in] SiPolicy  The Silicon Policy PPI instance
**/
VOID
PtlPcdPmcInit (
  IN SI_POLICY_PPI  *SiPolicy
  )
{
  PMC_SOC_CONFIG       PmcSocConfig;
  PMC_HANDLE           PmcHandle;
  PMC_CALLBACK         PmcCallback;
  PCH_PM_CONFIG        *PmConfig;
  EFI_STATUS           Status;
  GPIOV2_SERVICES      *GpioServices;

  PtlPcdPmcGetSocConfig (SiPolicy, &PmcSocConfig);

  PtlPcdPmcGetHandle (SiPolicy, &PmcSocConfig, &PmcCallback, &PmConfig, &PmcHandle);
  PmcInit (&PmcHandle);

  PtlPcdPmSyncInitMessages(&PmcHandle);

  if (PmcSocConfig.TimedGpioSupported) {
      Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: retrieving GpioServices(%a, %d) failed (Status: %d)\n", __FUNCTION__, GPIO_HID_PTL_PCD_P, 0, Status));
      ASSERT (FALSE);
      return;
    }

    if (PmConfig->EnableTimedGpio0) {
      Status = PtlPcdGpioEnableTimeSync (GpioServices, 0);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: enabling GPIO V2 Time Sync %d failed (Status: %d)\n", __FUNCTION__, 0, Status));
        ASSERT (FALSE);
        return;
      }
    }
    if (PmConfig->EnableTimedGpio1) {
      Status = PtlPcdGpioEnableTimeSync (GpioServices, 1);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: enabling GPIO V2 Time Sync %d failed (Status: %d)\n", __FUNCTION__, 1, Status));
        ASSERT (FALSE);
        return;
      }
    }
  }
  MmioOr32 (PmcHandle.PwrmBase + R_PMC_PWRM_VR_MISC_CTL, 0x00100000);
  MmioOr32 (PmcHandle.PwrmBase + R_PMC_PWRM_PM_CFG, 0x234F0000);
  MmioAnd32 (PmcHandle.PwrmBase + R_PMC_PWRM_PM_CFG4, (UINT32)~B_PMC_PWRM_PM_CFG4_VCCST_TMR);
  PmcLockFivrConfig (PmcHandle.PwrmBase);
  PtlPcdPsfHidePmc ();

  // Write clear GPE1 TBT LSX Bit
  // HSDES Article: https://hsdes.intel.com/appstore/article/#/15013925040
  //
  PtlGpe1LsxStatusClear ();
}

/**
  Locks down some extra registers in PMC that
  are not connected to any BIOS functionalities.

  @param[in] PmcHandle  PMC handle
**/
VOID
PtlPcdPmcExtraLock (
  IN PMC_HANDLE  *PmcHandle
  )
{
  MmioOr32 (PmcHandle->PwrmBase + R_PTL_PCD_P_PMC_PWRM_PMCR, B_PTL_PCD_P_PMC_PWRM_PMCR_PGD_LOCK);
  MmioOr32 (PmcHandle->PwrmBase + R_PTL_PCD_P_PMC_PMSYNC_TPR_CFG, B_PTL_PCD_P_PMC_PMSYNC_TPR_CFG_PCH2CPU_TPR_CFG_LOCK);
  MmioOr32 (PmcHandle->PwrmBase + R_PTL_PCD_P_PMC_PWRM_TSS0, B_PTL_PCD_P_PMC_PWRM_TSS0_TSS0LOCK);
  MmioOr32 (PmcHandle->PwrmBase + R_PTL_PCD_P_PMC_PWRM_TSS1, B_PTL_PCD_P_PMC_PWRM_TSS1_TSS1LOCK);
  MmioOr32 (PmcHandle->PwrmBase + R_PTL_PCD_P_PMC_PWRM_TSS2, B_PTL_PCD_P_PMC_PWRM_TSS2_TSS2LOCK);
  MmioOr32 (PmcHandle->PwrmBase + R_PTL_PCD_P_PMC_PWRM_TSS3, B_PTL_PCD_P_PMC_PWRM_TSS3_TSS3LOCK);
  MmioOr32 (PmcHandle->PwrmBase + R_PTL_PCD_P_PMC_PWRM_TSS4, B_PTL_PCD_P_PMC_PWRM_TSS4_TSS4LOCK);
  MmioOr32 (PmcHandle->PwrmBase + R_PTL_PCD_P_PMC_PWRM_TSS5, B_PTL_PCD_P_PMC_PWRM_TSS5_TSS5LOCK);
  MmioOr32 (PmcHandle->PwrmBase + R_PTL_PCD_P_PMC_PWRM_TSS6, B_PTL_PCD_P_PMC_PWRM_TSS6_TSS6LOCK);
  MmioOr32 (PmcHandle->PwrmBase + R_PTL_PCD_P_PMC_PWRM_TSS7, B_PTL_PCD_P_PMC_PWRM_TSS7_TSS7LOCK);
  MmioOr32 (PmcHandle->PwrmBase + R_PTL_PCD_P_PMC_PWRM_TTF, B_PTL_PCD_P_PMC_PWRM_TTF_TTFL);
}

/**
  Init PMC at end of PEI.

  @param[in] SiPolicy  Pointer to SiPolicy
**/
VOID
PtlPcdPmcInitEndOfPei (
  IN SI_POLICY_PPI  *SiPolicy
  )
{
  PMC_SOC_CONFIG       PmcSocConfig;
  PMC_HANDLE           PmcHandle;
  PMC_CALLBACK         PmcCallback;
  PCH_PM_CONFIG        *PmConfig;

  PtlPcdPmcGetHandle (SiPolicy, &PmcSocConfig, &PmcCallback, &PmConfig, &PmcHandle);

  //
  // Configure Pm timer
  // This must be done after uCode PM timer emulation.
  //
  PmcConfigurePmTimer (&PmcHandle);
  PmcLockSettings (&PmcHandle, &PmcSocConfig);

  //
  // Lock PMC Static function disable configuration fields. It is assumed
  // that at the end of PEI all static disable configuration is finished.
  //
  MmioOr32 (PmcHandle.PwrmBase + R_PMC_PWRM_GEN_PMCON_B, BIT21);

  //
  // Lock V1p05 PHY FET Ramp Time settings
  //
  MmioOr32 (PmcHandle.PwrmBase + R_PMC_PWRM_EXT_FET_RAMP_CONFIG, B_PMC_PWRM_EXT_FET_RAMP_CONFIG_V105_PHY_FRT_LOCK | B_PMC_PWRM_EXT_FET_RAMP_CONFIG_V105_PHY_IS_FRT_LOCK);

  MmioOr32 (PmcHandle.PwrmBase + R_PMC_PWRM_GEN_PMCON_B, B_PMC_PWRM_GEN_PMCON_B_CPU_EPOC_LOCK);

  MmioAndThenOr32 (
    PmcHandle.PwrmBase + R_PTL_SOC_PMC_PWRM_DISABLE_DTS_IN_S0IX,
    ~(UINT32)B_PTL_SOC_PMC_PWRM_DISABLE_DTS_IN_S0IX_E_DTS_TH_XING,
    B_PTL_SOC_PMC_PWRM_DISABLE_DTS_IN_S0IX_D_DTS
  );
  MmioOr32 (PmcHandle.PwrmBase + R_PTL_SOC_PMC_PWRM_DISABLE_DTS_IN_S0IX, B_PTL_SOC_PMC_PWRM_DISABLE_DTS_IN_S0IX_S0IXLOCK);

  PtlPcdPmcExtraLock (&PmcHandle);
}
