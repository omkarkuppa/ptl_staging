/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

**/
#include <Library/DebugLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SataSocLib.h>
#include <Library/ChipsetInitLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/PcdPolicyLib.h>
#include <LpcConfig.h>
#include <HsioPcieConfig.h>
#include <PchPcieRpConfig.h>
#include <ConfigBlock/PchGeneralConfig.h>
#include <HsioConfig.h>
#include <FlashProtectionConfig.h>
#include <LockDownConfig.h>

/**
  Print HSIO_CONFIG.

  @param[in] HsioConfig         Pointer to a HSIO_CONFIG that provides the eSPI setting

**/
VOID
PrintHsioConfig (
  IN CONST HSIO_CONFIG   *HsioConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ HSIO Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " ChipsetInit Binary Pointer = %x\n", HsioConfig->ChipsetInitBinPtr));
  DEBUG ((DEBUG_INFO, " ChipsetInit Binary Length  = %x\n", HsioConfig->ChipsetInitBinLen));
  DEBUG ((DEBUG_INFO, " SynpsPhy Binary Pointer    = %x\n", HsioConfig->SynpsPhyBinPtr));
  DEBUG ((DEBUG_INFO, " SynpsPhy Binary Length     = %x\n", HsioConfig->SynpsPhyBinLen));
  DEBUG ((DEBUG_INFO, " Nphy Binary Pointer        = %x\n", HsioConfig->NphyBinPtr));
  DEBUG ((DEBUG_INFO, " Nphy Binary Length         = %x\n", HsioConfig->NphyBinLen));
}

/**
  Print PCH_PCIE_CONFIG and serial out.

  @param[in] PchPcieConfig      Pointer to a PCH_PCIE_CONFIG that provides the platform settings
**/
VOID
PchPrintPcieConfig (
  IN CONST PCH_PCIE_CONFIG   *PchPcieConfig
  )
{
  UINT32 Index;
  UINT32 RpIndex;

  DEBUG ((DEBUG_INFO, "------------------ PCH PCIE Config ------------------\n"));
  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    DEBUG ((DEBUG_INFO, " RootPort[%d] HotPlug= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.HotPlug));
    DEBUG ((DEBUG_INFO, " RootPort[%d] PmSci= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PmSci));
    DEBUG ((DEBUG_INFO, " RootPort[%d] ClkReqDetect= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.ClkReqDetect));
    DEBUG ((DEBUG_INFO, " RootPort[%d] MrlSensorPresent = %d\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.MrlSensorPresent));
    DEBUG ((DEBUG_INFO, " RootPort[%d] UnsupportedRequestReport= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.UnsupportedRequestReport));
    DEBUG ((DEBUG_INFO, " RootPort[%d] FatalErrorReport= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.FatalErrorReport));
    DEBUG ((DEBUG_INFO, " RootPort[%d] NoFatalErrorReport= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.NoFatalErrorReport));
    DEBUG ((DEBUG_INFO, " RootPort[%d] CorrectableErrorReport= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.CorrectableErrorReport));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SystemErrorOnFatalError= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SystemErrorOnFatalError));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SystemErrorOnNonFatalError= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SystemErrorOnNonFatalError));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SystemErrorOnCorrectableError= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SystemErrorOnCorrectableError));
    DEBUG ((DEBUG_INFO, " RootPort[%d] MaxPayload= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.MaxPayload));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SlotImplemented= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SlotImplemented));
    DEBUG ((DEBUG_INFO, " RootPort[%d] AcsEnabled= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.AcsEnabled));
    DEBUG ((DEBUG_INFO, " RootPort[%d] PtmEnabled= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PtmEnabled));
    DEBUG ((DEBUG_INFO, " RootPort[%d] AdvancedErrorReporting= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.AdvancedErrorReporting));
    DEBUG ((DEBUG_INFO, " RootPort[%d] TransmitterHalfSwing= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.TransmitterHalfSwing));
    DEBUG ((DEBUG_INFO, " RootPort[%d] PcieSpeed= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieSpeed));
    DEBUG ((DEBUG_INFO, " RootPort[%d] PhysicalSlotNumber= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PhysicalSlotNumber));
    DEBUG ((DEBUG_INFO, " RootPort[%d] CompletionTimeout= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.CompletionTimeout));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Aspm= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.Aspm));
    DEBUG ((DEBUG_INFO, " RootPort[%d] L1Substates= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.L1Substates));
    DEBUG ((DEBUG_INFO, " RootPort[%d] LtrEnable= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.LtrEnable));
    DEBUG ((DEBUG_INFO, " RootPort[%d] LtrConfigLock= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.LtrConfigLock));
    DEBUG ((DEBUG_INFO, " RootPort[%d] LtrMaxSnoopLatency= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxSnoopLatency));
    DEBUG ((DEBUG_INFO, " RootPort[%d] LtrMaxNoSnoopLatency= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxNoSnoopLatency));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SnoopLatencyOverrideMode= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideMode));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SnoopLatencyOverrideMultiplier= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideMultiplier));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SnoopLatencyOverrideValue= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideValue));
    DEBUG ((DEBUG_INFO, " RootPort[%d] NonSnoopLatencyOverrideMode= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideMode));
    DEBUG ((DEBUG_INFO, " RootPort[%d] NonSnoopLatencyOverrideMultiplier= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideMultiplier));
    DEBUG ((DEBUG_INFO, " RootPort[%d] NonSnoopLatencyOverrideValue= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideValue));
    DEBUG ((DEBUG_INFO, " RootPort[%d] ForceLtrOverride= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.ForceLtrOverride));
    DEBUG ((DEBUG_INFO, " RootPort[%d] LtrOverrideSpecCompliant= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.LtrOverrideSpecCompliant));
    DEBUG ((DEBUG_INFO, " RootPort[%d] DetectTimeoutMs= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.DetectTimeoutMs));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SlotPowerLimitScale= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SlotPowerLimitScale));
    DEBUG ((DEBUG_INFO, " RootPort[%d] SlotPowerLimitValue= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SlotPowerLimitValue));
    DEBUG ((DEBUG_INFO, " RootPort[%d] EnableCpm= %x\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.EnableCpm));
    DEBUG ((DEBUG_INFO, " RootPort[%d] FormFactor = %d\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.FormFactor));
    DEBUG ((DEBUG_INFO, " RootPort[%d] EnablePeerMemoryWrite = %d\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.EnablePeerMemoryWrite));
    DEBUG ((DEBUG_INFO, " RootPort[%d] ClockGating = %d\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.ClockGating));
    DEBUG ((DEBUG_INFO, " RootPort[%d] PowerGating = %d\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PowerGating));
    DEBUG ((DEBUG_INFO, " RootPort[%d] AutoPowerGating = %d\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.AutoPowerGating));
    DEBUG ((DEBUG_INFO, " RootPort[%d] VisaClockGating = %d\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.VisaClockGating));
    DEBUG ((DEBUG_INFO, " RootPort[%d] LinkDownGpios = %d\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.LinkDownGpios));
    DEBUG ((DEBUG_INFO, " RootPort[%d] PcieFomsCp = %d\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieFomsCp));
    DEBUG ((DEBUG_INFO, " RootPort[%d] EqPhBypass: %d\n", Index, PchPcieConfig->RootPort[Index].PcieRpCommonConfig.EqPhBypass));
  }

  for (RpIndex = 0; RpIndex < GetPchMaxPciePortNum(); RpIndex++) {
    DEBUG ((DEBUG_INFO, " RootPort[%d] AspmOptionalityCompliance: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.AspmOptionalityCompliance));
    DEBUG ((DEBUG_INFO, " RootPort[%d] OverrideEqualizationDefaults: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.OverrideEqualizationDefaults));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen3 PcieLinkEqMethod: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.PcieLinkEqMethod));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen4 PcieLinkEqMethod: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.PcieLinkEqMethod));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen5 PcieLinkEqMethod: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.PcieLinkEqMethod));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen3 PcieLinkEqMode: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.PcieLinkEqMode));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen4 PcieLinkEqMode: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.PcieLinkEqMode));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen5 PcieLinkEqMode: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.PcieLinkEqMode));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen3 Ph1DpTxPreset: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph1DpTxPreset));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen4 Ph1DpTxPreset: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph1DpTxPreset));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen5 Ph1DpTxPreset: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph1DpTxPreset));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen3 Ph1UpTxPreset: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph1UpTxPreset));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen4 Ph1UpTxPreset: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph1UpTxPreset));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen5 Ph1UpTxPreset: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph1UpTxPreset));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen3 Ph3NoOfPresetOrCoeff: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3NoOfPresetOrCoeff));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen4 Ph3NoOfPresetOrCoeff: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3NoOfPresetOrCoeff));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen5 Ph3NoOfPresetOrCoeff: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3NoOfPresetOrCoeff));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen3 EqPh3Bypass: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.EqPh3Bypass));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen4 EqPh3Bypass: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.EqPh3Bypass));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen5 EqPh3Bypass: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.EqPh3Bypass));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen3 EqPh23Bypass: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.EqPh23Bypass));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen4 EqPh23Bypass: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.EqPh23Bypass));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen5 EqPh23Bypass: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.EqPh23Bypass));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen3 PcetTimer: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.PCETTimer));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen4 PcetTimer: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.PCETTimer));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen5 PcetTimer: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.PCETTimer));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen3 TsLockTimer: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.TsLockTimer));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen4 TsLockTimer: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.TsLockTimer));
    DEBUG ((DEBUG_INFO, " RootPort[%d] Gen5 TsLockTimer: %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.TsLockTimer));
    for (Index = 0; Index < PCIE_LINK_EQ_COEFFICIENTS_MAX; Index++) {
      DEBUG ((DEBUG_INFO,
              "RootPort[%d] Gen3 Phase 3 Coefficient %d, pre-cursor = %d, post-cursor = %d\n",
              RpIndex,
              Index,
              PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[Index].PreCursor,
              PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[Index].PostCursor));
      DEBUG ((DEBUG_INFO,
              "RootPort[%d] Gen4 Phase 3 Coefficient %d, pre-cursor = %d, post-cursor = %d\n",
              RpIndex,
              Index,
              PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[Index].PreCursor,
              PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[Index].PostCursor));
      DEBUG ((DEBUG_INFO,
              "RootPort[%d] Gen5 Phase 3 Coefficient %d, pre-cursor = %d, post-cursor = %d\n",
              RpIndex,
              Index,
              PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[Index].PreCursor,
              PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[Index].PostCursor));
    }
    for (Index = 0; Index < PCIE_LINK_EQ_PRESETS_MAX; Index++) {
      DEBUG ((DEBUG_INFO, "RootPort[%d] Gen3 Phase 3 Preset %d = %d\n", RpIndex, Index, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[Index]));
      DEBUG ((DEBUG_INFO, "RootPort[%d] Gen4 Phase 3 Preset %d = %d\n", RpIndex, Index, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[Index]));
      DEBUG ((DEBUG_INFO, "RootPort[%d] Gen5 Phase 3 Preset %d = %d\n", RpIndex, Index, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[Index]));
    }
    DEBUG ((DEBUG_INFO, "RootPort[%d] Gen3 LocalTxOverrideEn = %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.LocalTxOverrideEn));
    DEBUG ((DEBUG_INFO, "RootPort[%d] Gen4 LocalTxOverrideEn = %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.LocalTxOverrideEn));
    DEBUG ((DEBUG_INFO, "RootPort[%d] Gen5 LocalTxOverrideEn = %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.LocalTxOverrideEn));
    DEBUG ((DEBUG_INFO, "RootPort[%d] Gen3 Ph2LocalTxOverridePreset = %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph2LocalTxOverridePreset));
    DEBUG ((DEBUG_INFO, "RootPort[%d] Gen4 Ph2LocalTxOverridePreset = %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph2LocalTxOverridePreset));
    DEBUG ((DEBUG_INFO, "RootPort[%d] Gen5 Ph2LocalTxOverridePreset = %d\n", RpIndex, PchPcieConfig->RootPort[RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph2LocalTxOverridePreset));
  }
  DEBUG ((DEBUG_INFO, " EnablePort8xhDecode= %x\n", PchPcieConfig->PcieCommonConfig.EnablePort8xhDecode));
  DEBUG ((DEBUG_INFO, " PchPciePort8xhDecodePortIndex= %x\n", PchPcieConfig->PchPciePort8xhDecodePortIndex));
  DEBUG ((DEBUG_INFO, " PcieClockGating= %x\n", PchPcieConfig->PcieClockGating));
  DEBUG ((DEBUG_INFO, " ComplianceTestMode= %x\n", PchPcieConfig->PcieCommonConfig.ComplianceTestMode));
  DEBUG ((DEBUG_INFO, " RpFunctionSwap= %x\n", PchPcieConfig->PcieCommonConfig.RpFunctionSwap));
  DEBUG ((DEBUG_INFO, " PhyAutoPowerGating= %x\n", PchPcieConfig->PcieCommonConfig.PhyAutoPowerGating));
  DEBUG ((DEBUG_INFO, " EnableDtr= %x\n", PchPcieConfig->PcieCommonConfig.EnableDtr));

}

/**
  Print PCH_LOCK_DOWN_CONFIG and serial out.

  @param[in] LockDownConfig         Pointer to a PCH_LOCK_DOWN_CONFIG that provides the platform setting
**/
VOID
PchPrintLockDownConfig (
  IN CONST PCH_LOCK_DOWN_CONFIG   *LockDownConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH Lock Down Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " GlobalSmi= %x\n", LockDownConfig->GlobalSmi));
  DEBUG ((DEBUG_INFO, " BiosInterface= %x\n", LockDownConfig->BiosInterface));
  DEBUG ((DEBUG_INFO, " BiosLock= %x\n", LockDownConfig->BiosLock));
  DEBUG ((DEBUG_INFO, " SpiEiss= %x\n", LockDownConfig->SpiEiss));
  DEBUG ((DEBUG_INFO, " UnlockGpioPads= %x\n", LockDownConfig->UnlockGpioPads));
}

/**
  Print PCH_GENERAL_CONFIG and serial out.

  @param[in] PchGeneralConfig   Pointer to a PCH_GENERAL_CONFIG that provides the platform setting
**/
VOID
PchPrintGeneralConfig (
  IN CONST PCH_GENERAL_CONFIG   *PchGeneralConfig
  )
{
  DEBUG ((DEBUG_INFO, "------------------ PCH General Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " Crid= %x\n", PchGeneralConfig->Crid));
  DEBUG ((DEBUG_INFO, " LegacyIoLowLatency = %x\n", PchGeneralConfig->LegacyIoLowLatency));
}

/**
  Print PCH_FLASH_PROTECTION_CONFIG and serial out.

  @param[in] FlashProtectConfig  Pointer to a PCH_FLASH_PROTECTION_CONFIG that provides the platform setting
**/
VOID
PchPrintFlashProtectionConfig (
  IN CONST PCH_FLASH_PROTECTION_CONFIG   *FlashProtectConfig
  )
{
  UINT32 Index;

  DEBUG ((DEBUG_INFO, "------------------ PCH Flash Protection Config ------------------\n"));
  for (Index = 0; Index < PCH_FLASH_PROTECTED_RANGES; ++Index) {
    DEBUG ((DEBUG_INFO, " WriteProtectionEnable[%d] = %x\n", Index, FlashProtectConfig->ProtectRange[Index].WriteProtectionEnable));
    DEBUG ((DEBUG_INFO, " ReadProtectionEnable[%d]  = %x\n", Index, FlashProtectConfig->ProtectRange[Index].ReadProtectionEnable));
    DEBUG ((DEBUG_INFO, " ProtectedRangeLimit[%d]   = %x\n", Index, FlashProtectConfig->ProtectRange[Index].ProtectedRangeLimit));
    DEBUG ((DEBUG_INFO, " ProtectedRangeBase[%d]    = %x\n", Index, FlashProtectConfig->ProtectRange[Index].ProtectedRangeBase));
  }
}

/**
  Print whole PCH config blocks and serial out.

  @param[in] SiPolicyPpi    The RC Policy PPI instance
**/
VOID
EFIAPI
PtlPcdPrintPolicyPpi (
  IN  SI_POLICY_PPI     *SiPolicyPpi
  )
{
DEBUG_CODE_BEGIN();
  EFI_STATUS                      Status;
  PCH_GENERAL_CONFIG              *PchGeneralConfig;
  PCH_PCIE_CONFIG                 *PchPcieConfig;
  PCH_FLASH_PROTECTION_CONFIG     *FlashProtectionConfig;
  PCH_LOCK_DOWN_CONFIG            *LockDownConfig;
  HSIO_CONFIG                     *HsioConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPchGeneralConfigGuid, (VOID *) &PchGeneralConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gPchPcieConfigGuid, (VOID *) &PchPcieConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gFlashProtectionConfigGuid, (VOID *) &FlashProtectionConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gLockDownConfigGuid, (VOID *) &LockDownConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gHsioConfigGuid, (VOID *) &HsioConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------------ PCH Print Policy Start ------------------------\n"));
  DEBUG ((DEBUG_INFO, " Revision= %x\n", SiPolicyPpi->TableHeader.Header.Revision));

  PchPrintGeneralConfig (PchGeneralConfig);
  PchPrintPcieConfig (PchPcieConfig);

  PchPrintLockDownConfig (LockDownConfig);
  PchPrintFlashProtectionConfig (FlashProtectionConfig);
  PrintHsioConfig (HsioConfig);

  DEBUG ((DEBUG_INFO, "------------------------ PCH Print Platform Protocol End --------------------------\n"));
DEBUG_CODE_END();
}
