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

@par Specification
**/
#include <Library/DebugLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SataSocLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/PcdPolicyLib.h>
#include <ConfigBlock/PchGeneralConfig.h>
#include <Library/PchPcieRpLib.h>
#include <FlashProtectionConfig.h>
#include <LockDownConfig.h>
#include <HsioConfig.h>
#include <PchPcieRpConfig.h>

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPchGeneralConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_GENERAL_CONFIG  *PchGeneralConfig;
  PchGeneralConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "PchGeneralConfig->Header.GuidHob.Name = %g\n", &PchGeneralConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "PchGeneralConfig->Header.GuidHob.Header.HobLength = 0x%x\n", PchGeneralConfig->Header.GuidHob.Header.HobLength));

  /********************************
    PCH general configuration
  ********************************/
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPcieRpConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  UINTN            Index;
  PCH_PCIE_CONFIG  *PchPcieConfig;

  PchPcieConfig = ConfigBlockPointer;

  /********************************
    PCI Express related settings
  ********************************/

  for (Index = 0; Index < GetPchMaxPciePortNum (); Index++) {
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.Aspm                   = PchPcieAspmAutoConfig;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PmSci                  = FALSE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.AcsEnabled             = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PtmEnabled             = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.DpcEnabled             = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.RpDpcExtensionsEnabled = FALSE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.MaxPayload             = PchPcieMaxPayload256;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SlotImplemented        = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PhysicalSlotNumber     = (UINT16) Index;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.L1Substates            = PchPcieL1SubstatesL1_1_2;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.L1Low                  = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.EnableCpm              = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.RelaxedOrder           = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.NoSnoop                = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.LedManagement          = FALSE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.ClockGating            = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PowerGating            = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.VisaClockGating        = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.AutoPowerGating        = TRUE;
    //
    // PCIe LTR Configuration.
    //
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.LtrEnable              = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideMode          = 2;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideMultiplier    = 2;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideValue         = 0x41;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideMode       = 2;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideMultiplier = 2;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideValue      = 0x41;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.LtrOverrideSpecCompliant          = 0;

    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.AspmOptionalityCompliance                         = 0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieCfgDump                                       = 0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.EqPhBypass                                        = 0;

    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.AdvancedErrorReporting                            = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.UnsupportedRequestReport                          = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.FatalErrorReport                                  = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.NoFatalErrorReport                                = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.CorrectableErrorReport                            = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.EnablePeerMemoryWrite                             = FALSE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.LinkDownGpios                                     = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.AutoPowerGating                                   = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.VisaClockGating                                   = TRUE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.HostL0sTxDis                                      = FALSE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.TransmitterHalfSwing                              = FALSE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.LtrConfigLock                     = FALSE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieRpLtrConfig.ForceLtrOverride                  = 0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieFomsCp                                        = 0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SystemErrorOnFatalError                           = 0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SystemErrorOnNonFatalError                        = 0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.SystemErrorOnCorrectableError                     = 0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.HotPlug                                           = FALSE;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieSpeed                                         = PcieAuto;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.DetectTimeoutMs                                   = 0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.CompletionTimeout                                 = 0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PresetToCoeffConfig                               = FALSE;

    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph1DpTxPreset = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph1UpTxPreset = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3NoOfPresetOrCoeff = 0x1;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[0].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[0].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[1].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[1].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[2].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[2].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[3].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[3].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[4].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[4].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[5].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[5].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[6].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[6].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[7].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[7].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[8].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[8].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[9].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[9].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[0] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[1] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[2] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[3] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[4] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[5] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[5] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[6] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[7] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[8] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[9] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[10] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.PCETTimer = 0x0;

    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph1DpTxPreset = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph1UpTxPreset = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3NoOfPresetOrCoeff = 0x1;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[0].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[0].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[1].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[1].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[2].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[2].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[3].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[3].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[4].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[4].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[5].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[5].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[6].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[6].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[7].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[7].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[8].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[8].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[9].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[9].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[0] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[1] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[2] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[3] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[4] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[5] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[6] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[7] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[8] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[9] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[10] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.PCETTimer = 0x0;

    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph1DpTxPreset = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph1UpTxPreset = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3NoOfPresetOrCoeff = 0x1;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[0].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[0].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[1].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[1].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[2].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[2].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[3].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[3].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[4].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[4].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[5].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[5].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[6].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[6].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[7].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[7].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[8].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[8].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[9].PreCursor  = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[9].PostCursor = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[0] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[1] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[2] = 0x7;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[3] = 0x8;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[4] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[5] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[6] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[7] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[8] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[9] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[10] = 0x0;
    PchPcieConfig->RootPort[Index].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.PCETTimer = 0x0;
  }
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPcieCommonConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_PCIE_CONFIG  *PchPcieConfig;

  PchPcieConfig = ConfigBlockPointer;

  PchPcieConfig->PcieCommonConfig.RpFunctionSwap     = TRUE;
  PchPcieConfig->PcieCommonConfig.PhyAutoPowerGating = TRUE;

  PchPcieConfig->PchPciePort8xhDecodePortIndex = 1;
  PchPcieConfig->GlobalPcieAer = 1;
  PchPcieConfig->PcieTbtPerfBoost = 0;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadPchPcieConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_PCIE_CONFIG  *PchPcieConfig;

  PchPcieConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "PchPcieConfig->Header.GuidHob.Name = %g\n", &PchPcieConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "PchPcieConfig->Header.GuidHob.Header.HobLength = 0x%x\n", PchPcieConfig->Header.GuidHob.Header.HobLength));

  LoadPcieCommonConfigDefault (PchPcieConfig);
  LoadPcieRpConfigDefault (PchPcieConfig);
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadFlashProtectionConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_FLASH_PROTECTION_CONFIG  *FlashProtectionConfig;
  FlashProtectionConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "FlashProtectionConfig->Header.GuidHob.Name = %g\n", &FlashProtectionConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "FlashProtectionConfig->Header.GuidHob.Header.HobLength = 0x%x\n", FlashProtectionConfig->Header.GuidHob.Header.HobLength));
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadLockDownConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_LOCK_DOWN_CONFIG  *LockDownConfig;
  LockDownConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "LockDownConfig->Header.GuidHob.Name = %g\n", &LockDownConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "LockDownConfig->Header.GuidHob.Header.HobLength = 0x%x\n", LockDownConfig->Header.GuidHob.Header.HobLength));

  /********************************
    Lockdown configuration
  ********************************/
  LockDownConfig->GlobalSmi       = TRUE;
  LockDownConfig->BiosInterface   = TRUE;
  LockDownConfig->BiosLock        = TRUE;
  LockDownConfig->SpiEiss         = TRUE;
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadHsioConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  HSIO_CONFIG  *HsioConfig;
  HsioConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "HsioConfig->Header.GuidHob.Name = %g\n", &HsioConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "HsioConfig->Header.GuidHob.Header.HobLength = 0x%x\n", HsioConfig->Header.GuidHob.Header.HobLength));
}

/**
  Load Config block default for FIA

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
LoadFiaConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  FIA_CONFIG  *FiaConfig;
  FiaConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "FiaConfig->Header.GuidHob.Name = %g\n", &FiaConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "FiaConfig->Header.GuidHob.Header.HobLength = 0x%x\n", FiaConfig->Header.GuidHob.Header.HobLength));
  FiaConfig->FiaProgramming        = TRUE;
}

/**
  Config blocks associated with gRstConfigGuid have to be placed one after another due to our access method.
**/
GLOBAL_REMOVE_IF_UNREFERENCED COMPONENT_BLOCK_ENTRY  mPchIpBlocks [] = {
  {&gPchGeneralConfigGuid,       sizeof (PCH_GENERAL_CONFIG),           PCH_GENERAL_CONFIG_REVISION,           LoadPchGeneralConfigDefault},
  {&gPchPcieConfigGuid,          sizeof (PCH_PCIE_CONFIG),              PCH_PCIE_CONFIG_REVISION,              LoadPchPcieConfigDefault},
  {&gFlashProtectionConfigGuid,  sizeof (PCH_FLASH_PROTECTION_CONFIG),  PCH_FLASH_PROTECTION_CONFIG_REVISION,  LoadFlashProtectionConfigDefault},
  {&gLockDownConfigGuid,         sizeof (PCH_LOCK_DOWN_CONFIG),         PCH_LOCK_DOWN_CONFIG_REVISION,         LoadLockDownConfigDefault},
  {&gHsioConfigGuid,             sizeof (HSIO_CONFIG),                  HSIO_CONFIG_REVISION,                  LoadHsioConfigDefault},
  {&gFiaConfigGuid,              sizeof (FIA_CONFIG),                   FIA_CONFIG_REVISION,                   LoadFiaConfigDefault}
};

GLOBAL_REMOVE_IF_UNREFERENCED COMPONENT_BLOCK_ENTRY  mSocSIpBlocks [] = {
  {&gPchGeneralConfigGuid,       sizeof (PCH_GENERAL_CONFIG),           PCH_GENERAL_CONFIG_REVISION,           LoadPchGeneralConfigDefault},
  {&gPchPcieConfigGuid,          sizeof (PCH_PCIE_CONFIG),              PCH_PCIE_CONFIG_REVISION,              LoadPchPcieConfigDefault},
  {&gFlashProtectionConfigGuid,  sizeof (PCH_FLASH_PROTECTION_CONFIG),  PCH_FLASH_PROTECTION_CONFIG_REVISION,  LoadFlashProtectionConfigDefault},
  {&gLockDownConfigGuid,         sizeof (PCH_LOCK_DOWN_CONFIG),         PCH_LOCK_DOWN_CONFIG_REVISION,         LoadLockDownConfigDefault},
  {&gHsioConfigGuid,             sizeof (HSIO_CONFIG),                  HSIO_CONFIG_REVISION,                  LoadHsioConfigDefault},
  {&gFiaConfigGuid,              sizeof (FIA_CONFIG),                   FIA_CONFIG_REVISION,                   LoadFiaConfigDefault}
};

/**
  Get PCH config block table total size.

  @retval                               Size of PCH config block table
**/
UINT16
EFIAPI
PtlPcdGetConfigBlockTotalSize (
  VOID
  )
{
  return GetComponentConfigBlockTotalSize (&mSocSIpBlocks[0], sizeof (mSocSIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}

/**
  PchAddConfigBlocks add all PCH config blocks.

  @param[in] ConfigBlockTableAddress    The pointer to add PCH config blocks

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
PtlPcdAddConfigBlocks (
  IN     VOID      *ConfigBlockTableAddress
  )
{
  DEBUG ((DEBUG_INFO, "PTL PCD AddConfigBlocks\n"));

  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mSocSIpBlocks[0], sizeof (mSocSIpBlocks) / sizeof (COMPONENT_BLOCK_ENTRY));
}
