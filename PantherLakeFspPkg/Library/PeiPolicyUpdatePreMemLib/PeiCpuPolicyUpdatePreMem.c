/** @file
  This file is SampleCode of the library for Intel CPU PEI Policy initialization.

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
**/

#include <Library/DebugLib.h>
#include <Ppi/SiPolicy.h>
#include <FspmUpd.h>
#include <Library/ConfigBlockLib.h>
#include <Library/CpuPlatformLib.h>
#include <Register/Cpuid.h>
#include <Library/PeiVrDomainLib.h>
/**
  This function performs CPU PEI Policy initialization in Pre-memory.

  @param[in] SiPreMemPolicyPpi     The SI Pre-Mem Policy PPI instance
  @param[in] FspmUpd               The pointer of FspmUpd

  @retval EFI_SUCCESS              The PPI is installed and initialized.
  @retval EFI ERRORS               The PPI is not successfully installed.
**/
EFI_STATUS
EFIAPI
FspUpdatePeiCpuPolicyPreMem (
  IN OUT SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi,
  IN     FSPM_UPD                 *FspmUpd
  )
{
  CPU_INIT_PREMEM_CONFIG                       *CpuInitPreMemConfig;
  CPU_POWER_MGMT_VR_CONFIG                     *CpuPowerMgmtVrConfig;
  CPU_POWER_DELIVERY_CONFIG                    *CpuPowerDeliveryConfig;
  UINT8                                        Index;
  EFI_STATUS                                   Status;
  UINT8                                        AllCoreCount;
  UINT8                                        AllSmallCoreCount;
  UINT8                                        AllLpAtomCores;
  UINT16                                       MaxNumVrs;

  AllCoreCount         = 0;
  AllSmallCoreCount    = 0;
  AllLpAtomCores       = 0;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuInitPreMemConfigGuid, (VOID *) &CpuInitPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuPowerMgmtVrConfigGuid, (VOID *) &CpuPowerMgmtVrConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuPowerDeliveryConfigGuid, (VOID *) &CpuPowerDeliveryConfig);
  ASSERT_EFI_ERROR (Status);
  //
  //  Update CpuConfigLibPreMem Config Block data
  //
  CpuInitPreMemConfig->BootMaxFrequency         = FspmUpd->FspmConfig.BootMaxFrequency;

  //
  // Converting 0xFF to the actual total supported core count.
  //
  if (FspmUpd->FspmConfig.ActiveCoreCount == 0xFF) {
    FspmUpd->FspmConfig.ActiveCoreCount = AllCoreCount;
  }
  if (FspmUpd->FspmConfig.ActiveSmallCoreCount == 0xFF) {
    FspmUpd->FspmConfig.ActiveSmallCoreCount = AllSmallCoreCount;
  }
  if (FspmUpd->FspmConfig.ActiveCoreCount == 0 && FspmUpd->FspmConfig.ActiveSmallCoreCount == 0) {
    FspmUpd->FspmConfig.ActiveCoreCount = AllCoreCount;
    FspmUpd->FspmConfig.ActiveSmallCoreCount = AllSmallCoreCount;
  }

  if (FspmUpd->FspmConfig.ActiveLpAtomCoreCount == 0xFF) {
    FspmUpd->FspmConfig.ActiveLpAtomCoreCount = AllLpAtomCores;
  }

  CpuInitPreMemConfig->ActiveCoreCount             = FspmUpd->FspmConfig.ActiveCoreCount;
  CpuInitPreMemConfig->ActiveSmallCoreCount        = FspmUpd->FspmConfig.ActiveSmallCoreCount;
  CpuInitPreMemConfig->ActiveLpAtomCoreCount       = FspmUpd->FspmConfig.ActiveLpAtomCoreCount;
  CpuInitPreMemConfig->FClkFrequency               = FspmUpd->FspmConfig.FClkFrequency;
  CpuInitPreMemConfig->BistOnReset                 = FspmUpd->FspmConfig.BistOnReset;
  CpuInitPreMemConfig->VmxEnable                   = FspmUpd->FspmConfig.VmxEnable;
  CpuInitPreMemConfig->CpuRatio                    = FspmUpd->FspmConfig.CpuRatio;
  CpuInitPreMemConfig->TmeEnable                   = FspmUpd->FspmConfig.TmeEnable;
  CpuInitPreMemConfig->CrashLogEnable              = FspmUpd->FspmConfig.CpuCrashLogEnable;
  CpuInitPreMemConfig->DebugInterfaceEnable        = FspmUpd->FspmConfig.DebugInterfaceEnable;
  CpuInitPreMemConfig->DebugInterfaceLockEnable    = FspmUpd->FspmConfig.DebugInterfaceLockEnable;
  CpuInitPreMemConfig->DfdEnable                   = FspmUpd->FspmConfig.DfdEnable;
  CpuInitPreMemConfig->PrmrrSize                   = FspmUpd->FspmConfig.PrmrrSize;
  CpuInitPreMemConfig->CrashLogGprs                = FspmUpd->FspmConfig.CrashLogGprs;
  CpuInitPreMemConfig->OcLock                      = FspmUpd->FspmConfig.OcLock;
  CpuInitPreMemConfig->TsegSize                    = FspmUpd->FspmConfig.TsegSize;
  CpuInitPreMemConfig->SmmRelocationEnable         = FspmUpd->FspmConfig.SmmRelocationEnable;
  CpuInitPreMemConfig->ReduceXecores               = FspmUpd->FspmConfig.ReduceXecores;
  ///
  /// Define Maximum Number of Voltage Regulator Domains.
  ///
  MaxNumVrs = GetMaxNumVrs ();

  for (Index = 0; ((Index < MaxNumVrs) && (Index < MAX_NUM_VRS)); Index++) {
    CpuPowerMgmtVrConfig->ImonSlope[Index]           = FspmUpd->FspmConfig.ImonSlope[Index];
    CpuPowerMgmtVrConfig->ImonOffset[Index]          = FspmUpd->FspmConfig.ImonOffset[Index];
    CpuPowerMgmtVrConfig->Ps1Threshold[Index]        = FspmUpd->FspmConfig.Ps1Threshold[Index];
    CpuPowerMgmtVrConfig->Ps2Threshold[Index]        = FspmUpd->FspmConfig.Ps2Threshold[Index];
    CpuPowerMgmtVrConfig->Ps3Threshold[Index]        = FspmUpd->FspmConfig.Ps3Threshold[Index];
    CpuPowerMgmtVrConfig->VrConfigEnable[Index]      = FspmUpd->FspmConfig.VrConfigEnable[Index];
    CpuPowerMgmtVrConfig->Ps4Enable[Index]           = FspmUpd->FspmConfig.Ps4Enable[Index];
    CpuPowerMgmtVrConfig->Ps3Enable[Index]           = FspmUpd->FspmConfig.Ps3Enable[Index];
    CpuPowerMgmtVrConfig->TdcEnable[Index]           = FspmUpd->FspmConfig.TdcEnable[Index];
    CpuPowerMgmtVrConfig->TdcTimeWindow[Index]       = FspmUpd->FspmConfig.TdcTimeWindow[Index];
    CpuPowerMgmtVrConfig->TdcLock[Index]             = FspmUpd->FspmConfig.TdcLock[Index];
    CpuPowerMgmtVrConfig->TdcMode[Index]             = FspmUpd->FspmConfig.TdcMode[Index];
    CpuPowerMgmtVrConfig->TdcCurrentLimit[Index]     = FspmUpd->FspmConfig.TdcCurrentLimit[Index];
    CpuPowerMgmtVrConfig->AcLoadline[Index]          = FspmUpd->FspmConfig.AcLoadline[Index];
    CpuPowerMgmtVrConfig->DcLoadline[Index]          = FspmUpd->FspmConfig.DcLoadline[Index];
    CpuPowerMgmtVrConfig->IccMax[Index]              = FspmUpd->FspmConfig.IccMax[Index];
    CpuPowerMgmtVrConfig->EnableFastVmode[Index]     = FspmUpd->FspmConfig.EnableFastVmode[Index];
    if (FspmUpd->FspmConfig.EnableFastVmode[Index] == 1) {
      CpuPowerMgmtVrConfig->IccLimit[Index]            = FspmUpd->FspmConfig.IccLimit[Index];
    }
    CpuPowerMgmtVrConfig->VrVoltageLimit[Index]      = FspmUpd->FspmConfig.VrVoltageLimit[Index];
    CpuPowerMgmtVrConfig->FastPkgCRampDisable[Index] = FspmUpd->FspmConfig.FastPkgCRampDisable[Index];
    CpuPowerMgmtVrConfig->SlowSlewRate[Index]        = FspmUpd->FspmConfig.SlowSlewRate[Index];
  }

  CpuPowerMgmtVrConfig->RfiMitigation                = FspmUpd->FspmConfig.RfiMitigation;
  CpuPowerMgmtVrConfig->PsysSlope                    = FspmUpd->FspmConfig.PsysSlope;
  CpuPowerMgmtVrConfig->PsysOffset                   = FspmUpd->FspmConfig.PsysOffset;
  CpuPowerMgmtVrConfig->PsysPmax                     = FspmUpd->FspmConfig.PsysPmax;
  CpuPowerMgmtVrConfig->AcousticNoiseMitigation      = FspmUpd->FspmConfig.AcousticNoiseMitigation;
  CpuPowerMgmtVrConfig->PcoreHysteresisWindow        = FspmUpd->FspmConfig.PcoreHysteresisWindow;
  CpuPowerMgmtVrConfig->EcoreHysteresisWindow        = FspmUpd->FspmConfig.EcoreHysteresisWindow;
  CpuPowerMgmtVrConfig->DlvrSpreadSpectrumPercentage = FspmUpd->FspmConfig.DlvrSpreadSpectrumPercentage;
  CpuPowerMgmtVrConfig->DlvrRfiFrequency             = FspmUpd->FspmConfig.DlvrRfiFrequency;
  CpuPowerMgmtVrConfig->DlvrRfiEnable                = FspmUpd->FspmConfig.DlvrRfiEnable;
  CpuPowerMgmtVrConfig->DlvrPhaseSsc                 = FspmUpd->FspmConfig.DlvrPhaseSsc;
  CpuPowerMgmtVrConfig->VrPowerDeliveryDesign        = FspmUpd->FspmConfig.VrPowerDeliveryDesign;
  CpuPowerMgmtVrConfig->FivrSpectrumEnable           = FspmUpd->FspmConfig.FivrSpectrumEnable;

  CpuPowerMgmtVrConfig->EnableVsysCritical              = FspmUpd->FspmConfig.EnableVsysCritical;
  CpuPowerMgmtVrConfig->VsysFullScale                   = FspmUpd->FspmConfig.VsysFullScale;
  CpuPowerMgmtVrConfig->VsysCriticalThreshold           = FspmUpd->FspmConfig.VsysCriticalThreshold;
  CpuPowerMgmtVrConfig->PsysFullScale                   = FspmUpd->FspmConfig.PsysFullScale;
  CpuPowerMgmtVrConfig->PsysCriticalThreshold           = FspmUpd->FspmConfig.PsysCriticalThreshold;
  CpuPowerMgmtVrConfig->VsysAssertionDeglitchMantissa   = FspmUpd->FspmConfig.VsysAssertionDeglitchMantissa;
  CpuPowerMgmtVrConfig->VsysAssertionDeglitchExponent   = FspmUpd->FspmConfig.VsysAssertionDeglitchExponent;
  CpuPowerMgmtVrConfig->VsysDeassertionDeglitchMantissa = FspmUpd->FspmConfig.VsysDeassertionDeglitchMantissa;
  CpuPowerMgmtVrConfig->VsysDeassertionDeglitchExponent = FspmUpd->FspmConfig.VsysDeassertionDeglitchExponent;

  //
  // Cpu Power Delivery policy update
  //
  for (Index = 0; Index < SKIN_TEMP_CONTROL_SENSOR; Index++) {
    CpuPowerDeliveryConfig->SkinTargetTemp[Index]          = FspmUpd->FspmConfig.SkinTargetTemp[Index];
    CpuPowerDeliveryConfig->SkinTempControlEnable[Index]   = FspmUpd->FspmConfig.SkinTempControlEnable[Index];
    CpuPowerDeliveryConfig->SkinControlLoopGain[Index]     = FspmUpd->FspmConfig.SkinControlLoopGain[Index];
    CpuPowerDeliveryConfig->SkinTempOverrideEnable[Index]  = FspmUpd->FspmConfig.SkinTempOverrideEnable[Index];
    CpuPowerDeliveryConfig->SkinMinPerformanceLevel[Index] = FspmUpd->FspmConfig.SkinMinPerformanceLevel[Index];
    CpuPowerDeliveryConfig->SkinTempOverride[Index]        = FspmUpd->FspmConfig.SkinTempOverride[Index];
  }
  CpuPowerDeliveryConfig->PowerLimit1Time          = FspmUpd->FspmConfig.PowerLimit1Time;
  CpuPowerDeliveryConfig->PowerLimit2              = FspmUpd->FspmConfig.PowerLimit2;
  CpuPowerDeliveryConfig->CustomPowerLimit1        = FspmUpd->FspmConfig.CustomPowerLimit1;
  CpuPowerDeliveryConfig->CustomPowerLimit2        = FspmUpd->FspmConfig.CustomPowerLimit2;
  CpuPowerDeliveryConfig->PowerLimit3Time          = FspmUpd->FspmConfig.PowerLimit3Time;
  CpuPowerDeliveryConfig->PowerLimit3DutyCycle     = FspmUpd->FspmConfig.PowerLimit3DutyCycle;
  CpuPowerDeliveryConfig->PowerLimit3Lock          = FspmUpd->FspmConfig.PowerLimit3Lock;
  CpuPowerDeliveryConfig->PowerLimit4Lock          = FspmUpd->FspmConfig.PowerLimit4Lock;
  CpuPowerDeliveryConfig->TccActivationOffset      = FspmUpd->FspmConfig.TccActivationOffset;
  CpuPowerDeliveryConfig->TccOffsetLock            = FspmUpd->FspmConfig.TccOffsetLock;
  CpuPowerDeliveryConfig->PowerLimit1              = (UINT16) (FspmUpd->FspmConfig.PowerLimit1);
  CpuPowerDeliveryConfig->PowerLimit2Power         = (UINT16) (FspmUpd->FspmConfig.PowerLimit2Power);
  CpuPowerDeliveryConfig->PowerLimit3              = (UINT16) (FspmUpd->FspmConfig.PowerLimit3);
  CpuPowerDeliveryConfig->PowerLimit4              = (UINT16) (FspmUpd->FspmConfig.PowerLimit4);
  CpuPowerDeliveryConfig->PowerLimit4Boost         = FspmUpd->FspmConfig.PowerLimit4Boost;
  CpuPowerDeliveryConfig->ApplyConfigTdp           = FspmUpd->FspmConfig.ApplyConfigTdp;
  CpuPowerDeliveryConfig->DualTauBoost             = FspmUpd->FspmConfig.DualTauBoost;
  CpuPowerDeliveryConfig->ConfigTdpLock            = FspmUpd->FspmConfig.ConfigTdpLock;
  CpuPowerDeliveryConfig->ConfigTdpBios            = FspmUpd->FspmConfig.ConfigTdpBios;
  CpuPowerDeliveryConfig->CustomPowerLimit1          = (UINT16) (FspmUpd->FspmConfig.Custom1PowerLimit1);
  CpuPowerDeliveryConfig->CustomPowerLimit2          = (UINT16) (FspmUpd->FspmConfig.Custom1PowerLimit2);
  CpuPowerDeliveryConfig->PsysPowerLimit1          = FspmUpd->FspmConfig.PsysPowerLimit1;
  CpuPowerDeliveryConfig->PsysPowerLimit1Time      = FspmUpd->FspmConfig.PsysPowerLimit1Time;
  CpuPowerDeliveryConfig->PsysPowerLimit2          = FspmUpd->FspmConfig.PsysPowerLimit2;
  CpuPowerDeliveryConfig->PsysPowerLimit1Power     = (UINT16) (FspmUpd->FspmConfig.PsysPowerLimit1Power);
  CpuPowerDeliveryConfig->PsysPowerLimit2Power     = (UINT16) (FspmUpd->FspmConfig.PsysPowerLimit2Power);
  CpuPowerDeliveryConfig->PlatformAtxTelemetryUnit = FspmUpd->FspmConfig.PlatformAtxTelemetryUnit;
  CpuPowerDeliveryConfig->DisableVrThermalAlert    = FspmUpd->FspmConfig.DisableVrThermalAlert;
  CpuPowerDeliveryConfig->ThermalMonitor           = FspmUpd->FspmConfig.ThermalMonitor;
  CpuPowerDeliveryConfig->ConfigTdpLevel           = FspmUpd->FspmConfig.ConfigTdpLevel;
  CpuPowerDeliveryConfig->VsysMax                  = FspmUpd->FspmConfig.VsysMax;
  CpuPowerDeliveryConfig->ThETAIbattEnable         = FspmUpd->FspmConfig.ThETAIbattEnable;
  CpuPowerDeliveryConfig->IsysCurrentLimitL1       = FspmUpd->FspmConfig.IsysCurrentLimitL1;
  CpuPowerDeliveryConfig->IsysCurrentL1Tau         = FspmUpd->FspmConfig.IsysCurrentL1Tau;
  CpuPowerDeliveryConfig->IsysCurrentLimitL2Enable = FspmUpd->FspmConfig.IsysCurrentLimitL2Enable;
  CpuPowerDeliveryConfig->ResponseMode             = FspmUpd->FspmConfig.ResponseMode;
  CpuPowerDeliveryConfig->AcDcPowerState           = FspmUpd->FspmConfig.AcDcPowerState;
  CpuPowerDeliveryConfig->IsysCurrentLimitL1Enable = FspmUpd->FspmConfig.IsysCurrentLimitL1Enable;

  return EFI_SUCCESS;
}
