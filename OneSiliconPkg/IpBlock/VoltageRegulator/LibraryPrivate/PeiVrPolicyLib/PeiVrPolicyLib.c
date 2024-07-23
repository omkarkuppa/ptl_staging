/** @file
  This file load VR deafults and config block.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PeiVrLib.h>
#include <Library/PeiVrDomainLib.h>
#include <CpuRegs.h>

/**
  Print CPU_POWER_MGMT_VR_CONFIG and serial out.

  @param[in] SiPolicyPpi  Instance of SI_POLICY_PPI
**/
VOID
EFIAPI
CpuPowerMgmtVrConfigPrint (
  IN  SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi
  )
{
  EFI_STATUS  Status;
  CPU_POWER_MGMT_VR_CONFIG  *CpuPowerMgmtVrConfig;
  UINT16                            MaxNumVrs;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuPowerMgmtVrConfigGuid, (VOID *) &CpuPowerMgmtVrConfig);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Define Maximum Number of Voltage Regulator Domains.
  ///
  MaxNumVrs = GetMaxNumVrs ();

  UINT32 Index = 0;
  DEBUG ((DEBUG_INFO, "------------------ CPU Power Mgmt VR Config ------------------\n"));

  for (Index = 0; Index < MaxNumVrs && Index < MAX_NUM_VRS; Index++) {
    DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : VrConfigEnable[%X] : 0x%X\n", Index, CpuPowerMgmtVrConfig->VrConfigEnable[Index]));
    if (CpuPowerMgmtVrConfig->VrConfigEnable[Index] == 1) {
      DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : TdcCurrentLimit[%X] : 0x%X\n", Index, CpuPowerMgmtVrConfig->TdcCurrentLimit[Index]));
      DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : AcLoadline[%X] : 0x%X\n", Index, CpuPowerMgmtVrConfig->AcLoadline[Index]));
      DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : DcLoadline[%X] : 0x%X\n", Index, CpuPowerMgmtVrConfig->DcLoadline[Index]));
      DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : Ps1Threshold[%X] : 0x%X\n", Index, CpuPowerMgmtVrConfig->Ps1Threshold[Index]));
      DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : Ps2Threshold[%X] : 0x%X\n", Index, CpuPowerMgmtVrConfig->Ps2Threshold[Index]));
      DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : Ps3Threshold[%X] : 0x%X\n", Index, CpuPowerMgmtVrConfig->Ps3Threshold[Index]));
      DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : Ps3Enable[%X] : 0x%X\n", Index, CpuPowerMgmtVrConfig->Ps3Enable[Index]));
      DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : Ps4Enable[%X] : 0x%X\n", Index, CpuPowerMgmtVrConfig->Ps4Enable[Index]));
      DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : ImonSlope[%X] : 0x%X\n", Index, CpuPowerMgmtVrConfig->ImonSlope[Index]));
      DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : ImonOffset[%X] : 0x%X\n", Index, CpuPowerMgmtVrConfig->ImonOffset[Index]));
      DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : IccMax[%X] : 0x%X\n", Index, CpuPowerMgmtVrConfig->IccMax[Index]));
      DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : EnableFastVmode[%X] : 0x%X\n", Index, CpuPowerMgmtVrConfig->EnableFastVmode[Index]));
      DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : IccLimit[%X] : 0x%X\n", Index, CpuPowerMgmtVrConfig->IccLimit[Index]));
      DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : VrVoltageLimit[%X] : 0x%X\n", Index, CpuPowerMgmtVrConfig->VrVoltageLimit[Index]));
      DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : TdcEnable[%X] : 0x%X\n", Index, CpuPowerMgmtVrConfig->TdcEnable[Index]));
      DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : TdcTimeWindow[%X] : 0x%X\n", Index, CpuPowerMgmtVrConfig->TdcTimeWindow[Index]));
      DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : TdcLock[%X] : 0x%X\n", Index, CpuPowerMgmtVrConfig->TdcLock[Index]));
      DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG:: FastPkgCRampDisable for Index = %d : 0x%X\n", Index, CpuPowerMgmtVrConfig->FastPkgCRampDisable[Index]));
      DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG:: SlowSlewRate for Index = %d : 0x%X\n", Index, CpuPowerMgmtVrConfig->SlowSlewRate[Index]));
      DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : TdcMode[%X] : 0x%X\n", Index, CpuPowerMgmtVrConfig->TdcMode[Index]));
      DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : VccInDemotionEnable[%X] : 0x%X\n", Index, CpuPowerMgmtVrConfig->VccInDemotionEnable[Index]));
      DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : VccInDemotionCapacitanceInUf[%X] : 0x%X\n", Index, CpuPowerMgmtVrConfig->VccInDemotionCapacitanceInUf[Index]));
      DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : VccInDemotionQuiescentPowerInMw[%X] : 0x%X\n", Index, CpuPowerMgmtVrConfig->VccInDemotionQuiescentPowerInMw[Index]));
    }
  }
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : PsysSlope : 0x%X\n", CpuPowerMgmtVrConfig->PsysSlope));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : PsysOffset : 0x%X\n", CpuPowerMgmtVrConfig->PsysOffset));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : PsysPmax : 0x%X\n", CpuPowerMgmtVrConfig->PsysPmax));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : AcousticNoiseMitigation : 0x%X\n", CpuPowerMgmtVrConfig->AcousticNoiseMitigation));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : PreWake : 0x%X\n", CpuPowerMgmtVrConfig->PreWake));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : RampUp : 0x%X\n", CpuPowerMgmtVrConfig->RampUp));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : RampDown : 0x%X\n", CpuPowerMgmtVrConfig->RampDown));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : DlvrSpreadSpectrumPercentage : 0x%X\n", CpuPowerMgmtVrConfig->DlvrSpreadSpectrumPercentage));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : DlvrRfiFrequency : 0x%X\n", CpuPowerMgmtVrConfig->DlvrRfiFrequency));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : DlvrPhaseSsc : 0x%X\n", CpuPowerMgmtVrConfig->DlvrPhaseSsc));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : DlvrRfiEnable : 0x%X\n", CpuPowerMgmtVrConfig->DlvrRfiEnable));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : RfiMitigation : 0x%X\n", CpuPowerMgmtVrConfig->RfiMitigation));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : VrPowerDeliveryDesign : 0x%X\n", CpuPowerMgmtVrConfig->VrPowerDeliveryDesign));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : EnableVsysCritical= 0x%X\n", CpuPowerMgmtVrConfig->EnableVsysCritical));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : PsysFullScale= 0x%X\n", CpuPowerMgmtVrConfig->PsysFullScale));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : PsysCriticalThreshold= 0x%X\n", CpuPowerMgmtVrConfig->PsysCriticalThreshold));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : VsysFullScale= 0x%X\n", CpuPowerMgmtVrConfig->VsysFullScale));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : VsysCriticalThreshold= 0x%X\n", CpuPowerMgmtVrConfig->VsysCriticalThreshold));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : VsysAssertionDeglitchMantissa= 0x%X\n", CpuPowerMgmtVrConfig->VsysAssertionDeglitchMantissa));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : VsysAssertionDeglitchExponent= 0x%X\n", CpuPowerMgmtVrConfig->VsysAssertionDeglitchExponent));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : VsysDeassertionDeglitchMantissa= 0x%X\n", CpuPowerMgmtVrConfig->VsysDeassertionDeglitchMantissa));
  DEBUG ((DEBUG_INFO, " CPU_POWER_MGMT_VR_CONFIG : VsysDeassertionDeglitchExponent= 0x%X\n", CpuPowerMgmtVrConfig->VsysDeassertionDeglitchExponent));
}
/**
  Load Config block default

  @param[IN,OUT] ConfigBlockPointer         Pointer to config block

**/
VOID
LoadCpuPowerMgmtVrConfigDefault (
  IN OUT VOID          *ConfigBlockPointer
  )
{
  UINTN                           Index;
  CPU_POWER_MGMT_VR_CONFIG        *CpuPowerMgmtVrConfig;
  UINT16                          MaxNumVrs;

  CpuPowerMgmtVrConfig = ConfigBlockPointer;
  Index                = 0;

  DEBUG ((DEBUG_INFO, "CpuPowerMgmtVrConfig->Header.GuidHob.Name = %g\n", &CpuPowerMgmtVrConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "CpuPowerMgmtVrConfig->Header.GuidHob.Header.HobLength = 0x%x\n", CpuPowerMgmtVrConfig->Header.GuidHob.Header.HobLength));
  ///
  /// Define Maximum Number of Voltage Regulator Domains.
  ///
  MaxNumVrs = GetMaxNumVrs ();

  /*************************************
    CPU VOLTAGE REGULATOR configuration
  **************************************/
  for (Index = 0; Index < MaxNumVrs && Index < MAX_NUM_VRS; Index++) {
    CpuPowerMgmtVrConfig->VrConfigEnable[Index] = CPU_FEATURE_ENABLE;
    CpuPowerMgmtVrConfig->Ps1Threshold[Index] = 0;
    CpuPowerMgmtVrConfig->Ps2Threshold[Index] = 0;
    CpuPowerMgmtVrConfig->Ps3Threshold[Index] = 0;
    CpuPowerMgmtVrConfig->Ps3Enable[Index] = CPU_FEATURE_ENABLE;
    CpuPowerMgmtVrConfig->Ps4Enable[Index] = CPU_FEATURE_ENABLE;
    CpuPowerMgmtVrConfig->PS1toPS0DynamicCutoffEnable[Index] = CPU_FEATURE_DISABLE;
    CpuPowerMgmtVrConfig->PS2toPS1DynamicCutoffEnable[Index] = CPU_FEATURE_DISABLE;
    CpuPowerMgmtVrConfig->PS1toPS0MCoef[Index] = 0x64;
    CpuPowerMgmtVrConfig->PS1toPS0CCoef[Index] = 0x7D0;
    CpuPowerMgmtVrConfig->PS2toPS1MCoef[Index] = 0x64;
    CpuPowerMgmtVrConfig->PS2toPS1CCoef[Index] = 0x1F4;
    //
    // 0: Disable, 1: Enable, 2: User Config
    //
    CpuPowerMgmtVrConfig->VccInDemotionEnable[Index] = 1;
  }
  ///
  /// Load CPU power management Vr Config block default for specific generation.
  ///
  PeiCpuLoadPowerMgmtVrConfigDefaultFru (CpuPowerMgmtVrConfig);
}

STATIC COMPONENT_BLOCK_ENTRY mVrIpBlock = {
  &gCpuPowerMgmtVrConfigGuid,
  sizeof (CPU_POWER_MGMT_VR_CONFIG),
  CPU_POWER_MGMT_VR_CONFIG_REVISION,
  LoadCpuPowerMgmtVrConfigDefault
};

/**
  Get VR config block total size.

  @retval Size of config block
**/
UINT16
EFIAPI
GetCpuPowerMgmtVrConfigBlockTotalSize (
  VOID
  )
{
  return mVrIpBlock.Size;
}

/**
  Add VR ConfigBlock.

  @param[in] ConfigBlockTableAddress The pointer to config block table

  @retval EFI_SUCCESS                The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES       Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
AddCpuPowerMgmtVrConfigBlock (
  IN VOID *ConfigBlockTableAddress
  )
{
  EFI_STATUS Status;

  Status = AddComponentConfigBlocks (ConfigBlockTableAddress, &mVrIpBlock, 1);
  return Status;
}
