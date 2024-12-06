/** @file
  PCIe RP SoC library.

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
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPcieRpLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <PcieRegs.h>
#include <Register/PchRegs.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <PchPcieRpInfo.h>
#include <Register/PchPcieRpRegs.h>
#include <Register/PcieSipRegs.h>
#include <IpCpcie.h>
#include <Library/PeiPchPcieClocksLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/PcdLib.h>
#include <Library/P2SbSocLib.h>
#include <PcdSbPortIds.h>

typedef struct {
  UINT8            DevNum;
  P2SB_PORT_16_ID  Pid;
  UINTN            RpNumBase;
  UINT8            NumOfRp;
  UINT8            NumOfLanes;
  BOOLEAN          IocDecoded;
} PTL_SOC_PCIE_CONTROLLER_INFO;

GLOBAL_REMOVE_IF_UNREFERENCED PTL_SOC_PCIE_CONTROLLER_INFO  mPtlPcdPPcieControllerInfo[] = {
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_1, .Pid.Pid16bit = PTL_SID_F2_PID_PXPA,  0, 4, 4, FALSE },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_5, .Pid.Pid16bit = PTL_SID_F2_PID_PXPB,  4, 4, 4, FALSE },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_9, .Pid.Pid16bit = PTL_P_H_SID_F2_PID_PXPC,  8, 2, 4, TRUE }
};

GLOBAL_REMOVE_IF_UNREFERENCED PTL_SOC_PCIE_CONTROLLER_INFO  mPtlPcdHPcieControllerInfo[] = {
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_1, .Pid.Pid16bit = PTL_SID_F2_PID_PXPA, 0, 4, 4, FALSE },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_5, .Pid.Pid16bit = PTL_SID_F2_PID_PXPB, 4, 4, 4, FALSE },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_9, .Pid.Pid16bit = PTL_P_H_SID_F2_PID_PXPC, 8, 2, 4, TRUE },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_11, .Pid.Pid16bit = PTL_H_SID_F2_PID_PXPD, 10, 1, 8, TRUE },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_12, .Pid.Pid16bit = PTL_H_SID_F2_PID_PXPE, 11, 1, 4, TRUE }
};



/**
  Translate RpIndex to controller index.

  @param[in] RpIndex  Root port index

  @return ControllerIndex
  @retval 0xFF Failed to lookup controller index.
**/
UINT32
PtlPcdRpIndexToControllerIndex(
  IN UINTN  RpIndex
  )
{
  if (PtlIsPcdP ()) {
    if (RpIndex < 4) {
      return 0;
    } else if (RpIndex < 8) {
      return 1;
    } else if (RpIndex < 10) {
      return 2;
    }
  } else {
    if (RpIndex < 4) {
      return 0;
    } else if (RpIndex < 8) {
      return 1;
    } else if (RpIndex < 10) {
      return 2;
    } else if (RpIndex < 11) {
      return 3;
    } else if (RpIndex < 12) {
      return 4;
    }
  }
  return 0xFF;
}

/**
  Get the FID number of the PCIe root port.

  @param[in] RpIndex  Index of the root port

  @return FID number to use when sending SBI msg
**/
UINT16
PtlPcdPcieRpGetSbiFid(
  IN UINTN  RpIndex
  )
{
      if (RpIndex < 8) {
        return (UINT16)(RpIndex % PCH_PCIE_CONTROLLER_PORTS);
      } else if (RpIndex < 10) {
        return (UINT16)(RpIndex % PEG_PCIE_CONTROLLER_PORTS);
      } else {
        return 0;
      }
}

/**
  Get P2SB instance for PCIe root port

  @param[in]      RpIndex              Root port physical number. (0-based)
  @param[in, out] P2SbController       P2SB controller pointer
  @param[in, out] Pid                  P2SB 16bit port ID

**/
VOID
PtlPcdGetPcieP2SbController (
  IN UINT32                      RpIndex,
  IN OUT P2SB_CONTROLLER         *P2SbController
  )
{
  PTL_SOC_PCIE_CONTROLLER_INFO   *PcieInfo;
  UINT32                         Index;

  Index    = PtlPcdRpIndexToControllerIndex ((UINTN)RpIndex);

  if (PtlIsPcdP ()) {
    if (Index >= (sizeof(mPtlPcdPPcieControllerInfo)/sizeof(PTL_SOC_PCIE_CONTROLLER_INFO))) {
      return;
    }
    PcieInfo = mPtlPcdPPcieControllerInfo;
  } else {
     if (Index >= (sizeof(mPtlPcdHPcieControllerInfo)/sizeof(PTL_SOC_PCIE_CONTROLLER_INFO))) {
      return;
     }
    PcieInfo = mPtlPcdHPcieControllerInfo;
  }

  PtlPcdGetP2SbController (P2SbController, PcieInfo[Index].Pid);
  return;
}
/**
  Get PTL SoC Pcie Root Port Device and Function Number by Root Port physical Number

  @param[in]  RpNumber              Root port physical number. (0-based)
  @param[out] RpDev                 Return corresponding root port device number.
  @param[out] RpFun                 Return corresponding root port function number.

  @retval     EFI_SUCCESS           Root port device and function is retrieved
  @retval     EFI_INVALID_PARAMETER RpNumber is invalid
**/
EFI_STATUS
PtlPcdGetPcieRpDevFun (
  IN  UINTN    RpNumber,
  OUT UINT16   *RpDev,
  OUT UINT16   *RpFun
  )
{
  UINT32                         Index;
  UINTN                          FuncIndex;
  UINT32                         PciePcd;
  PTL_SOC_PCIE_CONTROLLER_INFO   *PcieInfo;
  P2SB_SIDEBAND_REGISTER_ACCESS  RpAccess;
  P2SB_CONTROLLER                P2SbController;

  Index    = PtlPcdRpIndexToControllerIndex (RpNumber);

  if (PtlIsPcdP ()) {
    if (Index >= ARRAY_SIZE (mPtlPcdPPcieControllerInfo)) {
      return EFI_NOT_FOUND;
    }
    PcieInfo = mPtlPcdPPcieControllerInfo;
  } else {
     if (Index >= ARRAY_SIZE (mPtlPcdHPcieControllerInfo)) {
      return EFI_NOT_FOUND;
     }
    PcieInfo = mPtlPcdHPcieControllerInfo;
  }

  *RpDev   = 0xFF;
  *RpFun   = 0xFF;

  if (Index == 0xFF  || PcieInfo == NULL) {
    return EFI_NOT_FOUND;
  }

  FuncIndex = RpNumber - PcieInfo[Index].RpNumBase;
  *RpDev = PcieInfo[Index].DevNum;
  PtlPcdGetP2SbController (&P2SbController, PcieInfo[Index].Pid);
  BuildP2SbSidebandAccess (
    &P2SbController,
    PcieInfo[Index].Pid.PortId.LocalPid,
    PtlPcdPcieRpGetSbiFid (RpNumber),
    P2SbPrivateConfig,
    P2SbMmioAccess,
    FALSE,
    &RpAccess
    );
  PciePcd = RpAccess.Access.Read32 (&RpAccess.Access, R_SPX_SIP16_PCR_PCD);
  if (PciePcd != 0xFFFFFFFF) {
    *RpFun = (PciePcd >> (FuncIndex * S_SPX_PCR_PCD_RP_FIELD)) & B_SPX_PCR_PCD_RP1FN;
  }
  return EFI_SUCCESS;
}

/**
  This function returns PID according to PCIe controller index

  @param[in] ControllerIndex     PCIe controller index

  @retval P2SB_PID    Returns PID for SBI Access
**/
P2SB_PID
PtlPcdGetPcieControllerSbiPid (
  IN UINT32  ControllerIndex
  )
{
  if (PtlIsPcdP ()) {
    if (ControllerIndex < ARRAY_SIZE (mPtlPcdPPcieControllerInfo)) {
      return mPtlPcdPPcieControllerInfo[ControllerIndex].Pid.PortId.LocalPid;
    }
  } else {
    if (ControllerIndex < ARRAY_SIZE (mPtlPcdHPcieControllerInfo)) {
      return mPtlPcdHPcieControllerInfo[ControllerIndex].Pid.PortId.LocalPid;
    }
  }
  return 0xFF;
}

/**
  Return number of Root Ports for given PCIe Controller number

  @param[in] ControllerIndex     PCIe controller index

  @retval NumOfRp    Number of Root Ports for PCIe Controller
**/
UINT8
PtlPcdGetPcieControllerNumOfRootPorts (
  IN UINT32  ControllerIndex
  )
{
  if (PtlIsPcdP ()) {
    if (ControllerIndex < ARRAY_SIZE (mPtlPcdPPcieControllerInfo)) {
      return mPtlPcdPPcieControllerInfo[ControllerIndex].NumOfRp;
    }
  } else {
    if (ControllerIndex < ARRAY_SIZE (mPtlPcdHPcieControllerInfo)) {
      return mPtlPcdHPcieControllerInfo[ControllerIndex].NumOfRp;
    }
  }
  return 0xFF;
}

/**
  Return number of lanes for given PCIe Controller number

  @param[in] ControllerIndex     PCIe controller index

  @retval NumOfLanes    Number of lanes for PCIe Controller
**/
UINT8
PtlPcdGetPcieControllerNumOfLanes (
  IN UINT32  ControllerIndex
  )
{
  if (PtlIsPcdP ()) {
    if (ControllerIndex < ARRAY_SIZE (mPtlPcdPPcieControllerInfo)) {
      return mPtlPcdPPcieControllerInfo[ControllerIndex].NumOfLanes;
    }
  } else {
    if (ControllerIndex < ARRAY_SIZE (mPtlPcdHPcieControllerInfo)) {
      return mPtlPcdHPcieControllerInfo[ControllerIndex].NumOfLanes;
    }
  }
  return 0xFF;
}

/**
  Update Config block Context for PCIE

  @param[in]  *pInst          Pointer to IP inst
  @param[in]  *PcieConfig     Rootport configuration
**/
VOID
PcieUpdateConfig (
  IN  IP_PCIE_INST                  *pInst,
  IN  PCH_PCIE_CONFIG               *PcieConfig
  )
{
  UINT16                             Index;

  if (pInst == NULL) {
    return;
  }

  //
  // Update PcieRpCommonConfig
  //
  pInst->PcieRpCommonConfig.HotPlug                       = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.HotPlug;
  pInst->PcieRpCommonConfig.PmSci                         = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PmSci;
  pInst->PcieRpCommonConfig.TransmitterHalfSwing          = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.TransmitterHalfSwing;
  pInst->PcieRpCommonConfig.AcsEnabled                    = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.AcsEnabled;
  pInst->PcieRpCommonConfig.AdvancedErrorReporting        = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.AdvancedErrorReporting;
  pInst->PcieRpCommonConfig.UnsupportedRequestReport      = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.UnsupportedRequestReport;
  pInst->PcieRpCommonConfig.FatalErrorReport              = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.FatalErrorReport;
  pInst->PcieRpCommonConfig.NoFatalErrorReport            = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.NoFatalErrorReport;
  pInst->PcieRpCommonConfig.CorrectableErrorReport        = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.CorrectableErrorReport;
  pInst->PcieRpCommonConfig.SystemErrorOnFatalError       = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.SystemErrorOnFatalError;
  pInst->PcieRpCommonConfig.SystemErrorOnNonFatalError    = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.SystemErrorOnNonFatalError;
  pInst->PcieRpCommonConfig.SystemErrorOnCorrectableError = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.SystemErrorOnCorrectableError;
  pInst->PcieRpCommonConfig.MaxPayload                    = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.MaxPayload;
  pInst->PcieRpCommonConfig.SlotImplemented               = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.SlotImplemented;
  pInst->PcieRpCommonConfig.PtmEnabled                    = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PtmEnabled;
  pInst->PcieRpCommonConfig.SlotPowerLimitScale           = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.SlotPowerLimitScale;
  pInst->PcieRpCommonConfig.SlotPowerLimitValue           = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.SlotPowerLimitValue;
  pInst->PcieRpCommonConfig.ClkReqDetect                  = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.ClkReqDetect;
  pInst->PcieRpCommonConfig.MrlSensorPresent              = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.MrlSensorPresent;
  pInst->PcieRpCommonConfig.RelaxedOrder                  = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.RelaxedOrder;
  pInst->PcieRpCommonConfig.NoSnoop                       = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.NoSnoop;
  pInst->PcieRpCommonConfig.EnablePeerMemoryWrite         = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.EnablePeerMemoryWrite;
  pInst->PcieRpCommonConfig.ClockGating                   = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.ClockGating;
  pInst->PcieRpCommonConfig.PowerGating                   = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PowerGating;
  pInst->PcieRpCommonConfig.AutoPowerGating               = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.AutoPowerGating;
  pInst->PcieRpCommonConfig.VisaClockGating               = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.VisaClockGating;
  pInst->PcieRpCommonConfig.PcieFomsCp                    = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieFomsCp;
  pInst->PcieRpCommonConfig.LedManagement                 = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.LedManagement;
  pInst->PcieRpCommonConfig.PhysicalSlotNumber            = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PhysicalSlotNumber;
  pInst->PcieRpCommonConfig.Gen3EqPh3Method               = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.Gen3EqPh3Method;
  pInst->PcieRpCommonConfig.CompletionTimeout             = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.CompletionTimeout;
  pInst->PcieRpCommonConfig.Aspm                          = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.Aspm;
  pInst->PcieRpCommonConfig.L1Substates                   = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.L1Substates;
  pInst->PcieRpCommonConfig.LtrEnable                     = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.LtrEnable;
  pInst->PcieRpCommonConfig.EnableCpm                     = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.EnableCpm;
  pInst->PcieRpCommonConfig.PcieSpeed                     = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieSpeed;
  pInst->PcieRpCommonConfig.DetectTimeoutMs               = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.DetectTimeoutMs;
  pInst->PcieRpCommonConfig.FormFactor                    = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.FormFactor;
  pInst->PcieRpCommonConfig.L1Low                         = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.L1Low;
  pInst->PcieRpCommonConfig.LinkDownGpios                 = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.LinkDownGpios;
  pInst->PcieRpCommonConfig.OverrideEqualizationDefaults  = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.OverrideEqualizationDefaults;
  pInst->PcieRpCommonConfig.PcieCfgDump                   = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieCfgDump;
  pInst->PcieRpCommonConfig.EqPhBypass                    = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.EqPhBypass;

  pInst->PcieRpCommonConfig.PcieRpLtrConfig.ForceLtrOverride                  = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieRpLtrConfig.ForceLtrOverride;
  pInst->PcieRpCommonConfig.PcieRpLtrConfig.LtrConfigLock                     = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieRpLtrConfig.LtrConfigLock;
  pInst->PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxNoSnoopLatency              = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxNoSnoopLatency;
  pInst->PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxSnoopLatency                = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxSnoopLatency;
  pInst->PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideMode       = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideMode;
  pInst->PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideMultiplier = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideMultiplier;
  pInst->PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideValue      = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieRpLtrConfig.NonSnoopLatencyOverrideValue;
  pInst->PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideMode          = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideMode;
  pInst->PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideMultiplier    = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideMultiplier;
  pInst->PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideValue         = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieRpLtrConfig.SnoopLatencyOverrideValue;

  pInst->PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.PcieLinkEqMethod                   = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.PcieLinkEqMethod;
  pInst->PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.PcieLinkEqMethod                   = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.PcieLinkEqMethod;
  pInst->PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.PcieLinkEqMethod                   = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.PcieLinkEqMethod;
  pInst->PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.PcieLinkEqMode                     = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.PcieLinkEqMode;
  pInst->PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.PcieLinkEqMode                     = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.PcieLinkEqMode;
  pInst->PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.PcieLinkEqMode                     = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.PcieLinkEqMode;
  pInst->PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.LocalTransmitterOverrideEnable     = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.LocalTxOverrideEn;
  pInst->PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.LocalTransmitterOverrideEnable     = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.LocalTxOverrideEn;
  pInst->PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.LocalTransmitterOverrideEnable     = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.LocalTxOverrideEn;
  pInst->PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3NumberOfPresetsOrCoefficients   = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3NoOfPresetOrCoeff;
  pInst->PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3NumberOfPresetsOrCoefficients   = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3NoOfPresetOrCoeff;
  pInst->PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3NumberOfPresetsOrCoefficients   = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3NoOfPresetOrCoeff;
  pInst->PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph1DownstreamPortTransmitterPreset = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph1DpTxPreset;
  pInst->PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph1DownstreamPortTransmitterPreset = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph1DpTxPreset;
  pInst->PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph1DownstreamPortTransmitterPreset = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph1DpTxPreset;
  pInst->PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph1UpstreamPortTransmitterPreset   = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph1UpTxPreset;
  pInst->PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph1UpstreamPortTransmitterPreset   = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph1UpTxPreset;
  pInst->PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph1UpstreamPortTransmitterPreset   = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph1UpTxPreset;
  pInst->PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph2LocalTransmitterOverridePreset  = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph2LocalTxOverridePreset;
  pInst->PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph2LocalTransmitterOverridePreset  = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph2LocalTxOverridePreset;
  pInst->PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph2LocalTransmitterOverridePreset  = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph2LocalTxOverridePreset;
  pInst->PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.PCETTimer                          = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.PCETTimer;
  pInst->PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.PCETTimer                          = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.PCETTimer;
  pInst->PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.PCETTimer                          = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.PCETTimer;
  pInst->PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.RemotePresetCoeffoverride          = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.RemotePresetCoeffoverride;
  pInst->PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.RemotePresetCoeffoverride          = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.RemotePresetCoeffoverride;
  pInst->PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.RemotePresetCoeffoverride          = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.RemotePresetCoeffoverride;
  pInst->PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.EqPh3Bypass                        = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.EqPh3Bypass;
  pInst->PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.EqPh3Bypass                        = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.EqPh3Bypass;
  pInst->PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.EqPh3Bypass                        = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.EqPh3Bypass;
  pInst->PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.EqPh23Bypass                       = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.EqPh23Bypass;
  pInst->PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.EqPh23Bypass                       = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.EqPh23Bypass;
  pInst->PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.EqPh23Bypass                       = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.EqPh23Bypass;
  pInst->PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.TsLockTimer                        = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.TsLockTimer;
  pInst->PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.TsLockTimer                        = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.TsLockTimer;
  pInst->PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.TsLockTimer                        = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.TsLockTimer;
  for (Index = 0; Index < IP_PCIE_LINK_EQ_COEFFICIENTS_MAX; Index++) {
    pInst->PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[Index].PreCursor  = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[Index].PreCursor;
    pInst->PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[Index].PostCursor = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3CoefficientsList[Index].PostCursor;
    pInst->PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[Index].PreCursor  = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[Index].PreCursor;
    pInst->PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[Index].PostCursor = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3CoefficientsList[Index].PostCursor;
    pInst->PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[Index].PreCursor  = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[Index].PreCursor;
    pInst->PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[Index].PostCursor = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3CoefficientsList[Index].PostCursor;
  }
  for (Index = 0; Index < IP_PCIE_LINK_EQ_PRESETS_MAX; Index++) {
    pInst->PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[Index] = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen3LinkEqPlatformSettings.Ph3PresetList[Index];
    pInst->PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[Index] = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen4LinkEqPlatformSettings.Ph3PresetList[Index];
    pInst->PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[Index] = PcieConfig->RootPort[pInst->RpIndex].PcieRpCommonConfig.PcieGen5LinkEqPlatformSettings.Ph3PresetList[Index];
  }
  pInst->PcieRpCommonConfig.RpFunctionSwap      = PcieConfig->PcieCommonConfig.RpFunctionSwap;
  pInst->PcieRpCommonConfig.ComplianceTestMode  = PcieConfig->PcieCommonConfig.ComplianceTestMode;
  pInst->PcieRpCommonConfig.EnablePort8xhDecode = PcieConfig->PcieCommonConfig.EnablePort8xhDecode;
  pInst->PcieRpCommonConfig.EnableDtr           = 0;
}

/**
  Get project specific Gen3 preset configuration.

  @param[in]     pInst               Pointer to IP inst
  @param[in/out] PlatformSettings    Pointer to caller allocated project equalization configuration
**/
VOID
PcieGetGen3PresetConfig (
  IN  IP_PCIE_INST                       *pInst,
  OUT IP_PCIE_LINK_EQ_PLATFORM_SETTINGS  *PlatformSettings
  )
{
  if ((pInst->SipPhyVersion == IP_SC16)) {
    PlatformSettings->Ph3PresetList[0]                 = 4;
    PlatformSettings->Ph3PresetList[1]                 = 5;
    PlatformSettings->Ph3PresetList[2]                 = 6;
    PlatformSettings->Ph3PresetList[3]                 = 7;
    PlatformSettings->Ph3PresetList[4]                 = 8;
    PlatformSettings->Ph3PresetList[5]                 = 0;
    PlatformSettings->Ph3PresetList[6]                 = 0;
    PlatformSettings->Ph3PresetList[7]                 = 0;
    PlatformSettings->Ph3PresetList[8]                 = 0;
    PlatformSettings->Ph3PresetList[9]                 = 0;
    PlatformSettings->Ph3PresetList[10]                = 0;
    PlatformSettings->Ph3NumberOfPresetsOrCoefficients = 5;
  } else if ((pInst->SipPhyVersion == IP_SE32)) {
    PlatformSettings->Ph3PresetList[0]                 = 4;
    PlatformSettings->Ph3PresetList[1]                 = 5;
    PlatformSettings->Ph3PresetList[2]                 = 6;
    PlatformSettings->Ph3PresetList[3]                 = 8;
    PlatformSettings->Ph3PresetList[4]                 = 9;
    PlatformSettings->Ph3PresetList[5]                 = 0;
    PlatformSettings->Ph3PresetList[6]                 = 0;
    PlatformSettings->Ph3PresetList[7]                 = 0;
    PlatformSettings->Ph3PresetList[8]                 = 0;
    PlatformSettings->Ph3PresetList[9]                 = 0;
    PlatformSettings->Ph3PresetList[10]                = 0;
    PlatformSettings->Ph3NumberOfPresetsOrCoefficients = 5;
  }
}

/**
  Get project specific Gen4 preset configuration.

  @param[in]     pInst               Pointer to IP inst
  @param[in/out] PlatformSettings    Pointer to caller allocated project equalization configuration
**/
VOID
PcieGetGen4PresetConfig (
  IN  IP_PCIE_INST                       *pInst,
  OUT IP_PCIE_LINK_EQ_PLATFORM_SETTINGS  *PlatformSettings
  )
{
  if ((pInst->SipPhyVersion == IP_SC16)) {
    PlatformSettings->Ph3PresetList[0]                 = 4;
    PlatformSettings->Ph3PresetList[1]                 = 5;
    PlatformSettings->Ph3PresetList[2]                 = 6;
    PlatformSettings->Ph3PresetList[3]                 = 0;
    PlatformSettings->Ph3PresetList[4]                 = 0;
    PlatformSettings->Ph3PresetList[5]                 = 0;
    PlatformSettings->Ph3PresetList[6]                 = 0;
    PlatformSettings->Ph3PresetList[7]                 = 0;
    PlatformSettings->Ph3PresetList[8]                 = 0;
    PlatformSettings->Ph3PresetList[9]                 = 0;
    PlatformSettings->Ph3PresetList[10]                = 0;
    PlatformSettings->Ph3NumberOfPresetsOrCoefficients = 3;
  } else if ((pInst->SipPhyVersion == IP_SE32)) {
    PlatformSettings->Ph3PresetList[0]                 = 4;
    PlatformSettings->Ph3PresetList[1]                 = 5;
    PlatformSettings->Ph3PresetList[2]                 = 6;
    PlatformSettings->Ph3PresetList[3]                 = 8;
    PlatformSettings->Ph3PresetList[4]                 = 9;
    PlatformSettings->Ph3PresetList[5]                 = 0;
    PlatformSettings->Ph3PresetList[6]                 = 0;
    PlatformSettings->Ph3PresetList[7]                 = 0;
    PlatformSettings->Ph3PresetList[8]                 = 0;
    PlatformSettings->Ph3PresetList[9]                 = 0;
    PlatformSettings->Ph3PresetList[10]                = 0;
    PlatformSettings->Ph3NumberOfPresetsOrCoefficients = 5;
  }
}

/**
  Get project specific Gen5 preset configuration.

  @param[in]     pInst               Pointer to IP inst
  @param[in/out] PlatformSettings    Pointer to caller allocated project equalization configuration
**/
VOID
PcieGetGen5PresetConfig (
  IN  IP_PCIE_INST                       *pInst,
  OUT IP_PCIE_LINK_EQ_PLATFORM_SETTINGS  *PlatformSettings
  )
{
  PlatformSettings->Ph3PresetList[0]                 = 4;
  PlatformSettings->Ph3PresetList[1]                 = 5;
  PlatformSettings->Ph3PresetList[2]                 = 7;
  PlatformSettings->Ph3PresetList[3]                 = 8;
  PlatformSettings->Ph3PresetList[4]                 = 0;
  PlatformSettings->Ph3PresetList[5]                 = 0;
  PlatformSettings->Ph3PresetList[6]                 = 0;
  PlatformSettings->Ph3PresetList[7]                 = 0;
  PlatformSettings->Ph3PresetList[8]                 = 0;
  PlatformSettings->Ph3PresetList[9]                 = 0;
  PlatformSettings->Ph3PresetList[10]                = 0;
  PlatformSettings->Ph3NumberOfPresetsOrCoefficients = 4;
}

/**
  Get project specific Gen3 preset to coefficient mapping configuration.

  @param[in]     pInst              Pointer to IP inst
  @param[in/out] ProjectSettings    Pointer to caller allocated project equalization configuration
**/
VOID
PcieGen3PresetToCoefficientConfig (
  IN  IP_PCIE_INST                      *pInst,
  OUT IP_PCIE_LINK_EQ_PROJECT_SETTINGS  *ProjectSettings
  )
{
  if ((pInst->SipPhyVersion == IP_SC16)) {
    ProjectSettings->PresetToCoefficient[0].CursorCoefficient = 36;
    ProjectSettings->PresetToCoefficient[0].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[0].PostCursorCoefficient = 12;
    ProjectSettings->PresetToCoefficient[1].CursorCoefficient = 40;
    ProjectSettings->PresetToCoefficient[1].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[1].PostCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[2].CursorCoefficient = 38;
    ProjectSettings->PresetToCoefficient[2].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[2].PostCursorCoefficient = 10;
    ProjectSettings->PresetToCoefficient[3].CursorCoefficient = 42;
    ProjectSettings->PresetToCoefficient[3].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[3].PostCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[4].CursorCoefficient = 48;
    ProjectSettings->PresetToCoefficient[4].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[4].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[5].CursorCoefficient = 44;
    ProjectSettings->PresetToCoefficient[5].PreCursorCoefficient = 4;
    ProjectSettings->PresetToCoefficient[5].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[6].CursorCoefficient = 42;
    ProjectSettings->PresetToCoefficient[6].PreCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[6].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[7].CursorCoefficient = 34;
    ProjectSettings->PresetToCoefficient[7].PreCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[7].PostCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[8].CursorCoefficient = 36;
    ProjectSettings->PresetToCoefficient[8].PreCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[8].PostCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[9].CursorCoefficient = 40;
    ProjectSettings->PresetToCoefficient[9].PreCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[9].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[10].CursorCoefficient = 32;
    ProjectSettings->PresetToCoefficient[10].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[10].PostCursorCoefficient = 16;
  } else if ((pInst->SipPhyVersion == IP_SE32)) {
    ProjectSettings->PresetToCoefficient[0].CursorCoefficient = 34;
    ProjectSettings->PresetToCoefficient[0].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[0].PostCursorCoefficient = 14;
    ProjectSettings->PresetToCoefficient[1].CursorCoefficient = 38;
    ProjectSettings->PresetToCoefficient[1].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[1].PostCursorCoefficient = 10;
    ProjectSettings->PresetToCoefficient[2].CursorCoefficient = 36;
    ProjectSettings->PresetToCoefficient[2].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[2].PostCursorCoefficient = 12;
    ProjectSettings->PresetToCoefficient[3].CursorCoefficient = 40;
    ProjectSettings->PresetToCoefficient[3].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[3].PostCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[4].CursorCoefficient = 48;
    ProjectSettings->PresetToCoefficient[4].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[4].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[5].CursorCoefficient = 42;
    ProjectSettings->PresetToCoefficient[5].PreCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[5].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[6].CursorCoefficient = 40;
    ProjectSettings->PresetToCoefficient[6].PreCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[6].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[7].CursorCoefficient = 32;
    ProjectSettings->PresetToCoefficient[7].PreCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[7].PostCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[8].CursorCoefficient = 34;
    ProjectSettings->PresetToCoefficient[8].PreCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[8].PostCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[9].CursorCoefficient = 38;
    ProjectSettings->PresetToCoefficient[9].PreCursorCoefficient = 10;
    ProjectSettings->PresetToCoefficient[9].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[10].CursorCoefficient = 32;
    ProjectSettings->PresetToCoefficient[10].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[10].PostCursorCoefficient = 16;
  }
}

/**
  Get project specific Gen4 preset to coefficient mapping configuration.

  @param[in]     pInst              Pointer to IP inst
  @param[in/out] ProjectSettings    Pointer to caller allocated project equalization configuration
**/
VOID
PcieGen4PresetToCoefficientConfig (
  IN  IP_PCIE_INST                      *pInst,
  OUT IP_PCIE_LINK_EQ_PROJECT_SETTINGS  *ProjectSettings
  )
{
 if ((pInst->SipPhyVersion == IP_SC16)) {
    ProjectSettings->PresetToCoefficient[0].CursorCoefficient = 36;
    ProjectSettings->PresetToCoefficient[0].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[0].PostCursorCoefficient = 12;
    ProjectSettings->PresetToCoefficient[1].CursorCoefficient = 40;
    ProjectSettings->PresetToCoefficient[1].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[1].PostCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[2].CursorCoefficient = 38;
    ProjectSettings->PresetToCoefficient[2].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[2].PostCursorCoefficient = 10;
    ProjectSettings->PresetToCoefficient[3].CursorCoefficient = 42;
    ProjectSettings->PresetToCoefficient[3].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[3].PostCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[4].CursorCoefficient = 48;
    ProjectSettings->PresetToCoefficient[4].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[4].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[5].CursorCoefficient = 44;
    ProjectSettings->PresetToCoefficient[5].PreCursorCoefficient = 4;
    ProjectSettings->PresetToCoefficient[5].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[6].CursorCoefficient = 42;
    ProjectSettings->PresetToCoefficient[6].PreCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[6].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[7].CursorCoefficient = 34;
    ProjectSettings->PresetToCoefficient[7].PreCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[7].PostCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[8].CursorCoefficient = 36;
    ProjectSettings->PresetToCoefficient[8].PreCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[8].PostCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[9].CursorCoefficient = 40;
    ProjectSettings->PresetToCoefficient[9].PreCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[9].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[10].CursorCoefficient = 32;
    ProjectSettings->PresetToCoefficient[10].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[10].PostCursorCoefficient = 16;
  } else if ((pInst->SipPhyVersion == IP_SE32)) {
    ProjectSettings->PresetToCoefficient[0].CursorCoefficient = 34;
    ProjectSettings->PresetToCoefficient[0].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[0].PostCursorCoefficient = 14;
    ProjectSettings->PresetToCoefficient[1].CursorCoefficient = 38;
    ProjectSettings->PresetToCoefficient[1].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[1].PostCursorCoefficient = 10;
    ProjectSettings->PresetToCoefficient[2].CursorCoefficient = 36;
    ProjectSettings->PresetToCoefficient[2].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[2].PostCursorCoefficient = 12;
    ProjectSettings->PresetToCoefficient[3].CursorCoefficient = 40;
    ProjectSettings->PresetToCoefficient[3].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[3].PostCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[4].CursorCoefficient = 48;
    ProjectSettings->PresetToCoefficient[4].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[4].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[5].CursorCoefficient = 42;
    ProjectSettings->PresetToCoefficient[5].PreCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[5].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[6].CursorCoefficient = 40;
    ProjectSettings->PresetToCoefficient[6].PreCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[6].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[7].CursorCoefficient = 32;
    ProjectSettings->PresetToCoefficient[7].PreCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[7].PostCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[8].CursorCoefficient = 34;
    ProjectSettings->PresetToCoefficient[8].PreCursorCoefficient = 6;
    ProjectSettings->PresetToCoefficient[8].PostCursorCoefficient = 8;
    ProjectSettings->PresetToCoefficient[9].CursorCoefficient = 38;
    ProjectSettings->PresetToCoefficient[9].PreCursorCoefficient = 10;
    ProjectSettings->PresetToCoefficient[9].PostCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[10].CursorCoefficient = 32;
    ProjectSettings->PresetToCoefficient[10].PreCursorCoefficient = 0;
    ProjectSettings->PresetToCoefficient[10].PostCursorCoefficient = 16;
  }
}

/**
  Get project specific Gen5 preset to coefficient mapping configuration.

  @param[in]     pInst              Pointer to IP inst
  @param[in/out] ProjectSettings    Pointer to caller allocated project equalization configuration
**/
VOID
PcieGen5PresetToCoefficientConfig (
  IN  IP_PCIE_INST                      *pInst,
  OUT IP_PCIE_LINK_EQ_PROJECT_SETTINGS  *ProjectSettings
  )
{
  ProjectSettings->PresetToCoefficient[0].CursorCoefficient      = 34;
  ProjectSettings->PresetToCoefficient[0].PreCursorCoefficient   = 0;
  ProjectSettings->PresetToCoefficient[0].PostCursorCoefficient  = 14;
  ProjectSettings->PresetToCoefficient[1].CursorCoefficient      = 38;
  ProjectSettings->PresetToCoefficient[1].PreCursorCoefficient   = 0;
  ProjectSettings->PresetToCoefficient[1].PostCursorCoefficient  = 10;
  ProjectSettings->PresetToCoefficient[2].CursorCoefficient      = 36;
  ProjectSettings->PresetToCoefficient[2].PreCursorCoefficient   = 0;
  ProjectSettings->PresetToCoefficient[2].PostCursorCoefficient  = 12;
  ProjectSettings->PresetToCoefficient[3].CursorCoefficient      = 40;
  ProjectSettings->PresetToCoefficient[3].PreCursorCoefficient   = 0;
  ProjectSettings->PresetToCoefficient[3].PostCursorCoefficient  = 8;
  ProjectSettings->PresetToCoefficient[4].CursorCoefficient      = 48;
  ProjectSettings->PresetToCoefficient[4].PreCursorCoefficient   = 0;
  ProjectSettings->PresetToCoefficient[4].PostCursorCoefficient  = 0;
  ProjectSettings->PresetToCoefficient[5].CursorCoefficient      = 42;
  ProjectSettings->PresetToCoefficient[5].PreCursorCoefficient   = 6;
  ProjectSettings->PresetToCoefficient[5].PostCursorCoefficient  = 0;
  ProjectSettings->PresetToCoefficient[6].CursorCoefficient      = 40;
  ProjectSettings->PresetToCoefficient[6].PreCursorCoefficient   = 8;
  ProjectSettings->PresetToCoefficient[6].PostCursorCoefficient  = 0;
  ProjectSettings->PresetToCoefficient[7].CursorCoefficient      = 32;
  ProjectSettings->PresetToCoefficient[7].PreCursorCoefficient   = 5;
  ProjectSettings->PresetToCoefficient[7].PostCursorCoefficient  = 11;
  ProjectSettings->PresetToCoefficient[8].CursorCoefficient      = 34;
  ProjectSettings->PresetToCoefficient[8].PreCursorCoefficient   = 6;
  ProjectSettings->PresetToCoefficient[8].PostCursorCoefficient  = 8;
  ProjectSettings->PresetToCoefficient[9].CursorCoefficient      = 38;
  ProjectSettings->PresetToCoefficient[9].PreCursorCoefficient   = 10;
  ProjectSettings->PresetToCoefficient[9].PostCursorCoefficient  = 0;
  ProjectSettings->PresetToCoefficient[10].CursorCoefficient     = 30;
  ProjectSettings->PresetToCoefficient[10].PreCursorCoefficient  = 2;
  ProjectSettings->PresetToCoefficient[10].PostCursorCoefficient = 16;
}

/**
  Get the project recommended equalization settings

  @param[in]  pInst                          Pointer to IP inst
  @param[out] PcieGen3LinkEqSettings         Pointer to the equalization config
  @param[out] PcieGen4LinkEqSettings         Pointer to the equalization config
  @param[out] PcieGen5LinkEqSettings         Pointer to the equalization config
**/
VOID
PcieGetProjectDefaultEqConfiguration (
  IN  IP_PCIE_INST              *pInst,
  OUT IP_PCIE_LINK_EQ_SETTINGS  *PcieGen3LinkEqSettings,
  OUT IP_PCIE_LINK_EQ_SETTINGS  *PcieGen4LinkEqSettings,
  OUT IP_PCIE_LINK_EQ_SETTINGS  *PcieGen5LinkEqSettings
  )
{
  PcieGetGen3PresetConfig (pInst, &PcieGen3LinkEqSettings->PlatformSettings);
  PcieGen3PresetToCoefficientConfig (pInst, &PcieGen3LinkEqSettings->ProjectSettings);
  PcieGen3LinkEqSettings->PlatformSettings.PcieLinkEqMethod                     = PcieLinkHardwareEq;
  PcieGen3LinkEqSettings->PlatformSettings.PcieLinkEqMode                       = PcieLinkEqPresetMode;
  PcieGen3LinkEqSettings->PlatformSettings.Ph1DownstreamPortTransmitterPreset   = 7;
  if ((pInst->SipPhyVersion == IP_SE32)) {
    PcieGen3LinkEqSettings->PlatformSettings.Ph1UpstreamPortTransmitterPreset   = 7;
  } else {
    PcieGen3LinkEqSettings->PlatformSettings.Ph1UpstreamPortTransmitterPreset   = 4;
  }
  PcieGen3LinkEqSettings->ProjectSettings.PipePresetToCoefficientQuerySupported = FALSE;
  PcieGen3LinkEqSettings->PlatformSettings.PCETTimer                            = 2;
  PcieGen3LinkEqSettings->ProjectSettings.FullSwingValue                        = 0x30;
  PcieGen3LinkEqSettings->ProjectSettings.LowFrequencyValue                     = 0x10;
  PcieGen3LinkEqSettings->PlatformSettings.EqPh3Bypass                          = 0;
  PcieGen3LinkEqSettings->PlatformSettings.EqPh23Bypass                         = 0;
  PcieGen3LinkEqSettings->PlatformSettings.TsLockTimer                          = 0;

  PcieGetGen4PresetConfig (pInst, &PcieGen4LinkEqSettings->PlatformSettings);
  PcieGen4PresetToCoefficientConfig (pInst, &PcieGen4LinkEqSettings->ProjectSettings);
  PcieGen4LinkEqSettings->PlatformSettings.PcieLinkEqMethod                     = PcieLinkHardwareEq;
  PcieGen4LinkEqSettings->PlatformSettings.PcieLinkEqMode                       = PcieLinkEqPresetMode;
  PcieGen4LinkEqSettings->PlatformSettings.Ph1DownstreamPortTransmitterPreset   = 7;
  if ((pInst->SipPhyVersion == IP_SE32)) {
    PcieGen4LinkEqSettings->PlatformSettings.Ph1UpstreamPortTransmitterPreset   = 7;
  } else {
    PcieGen4LinkEqSettings->PlatformSettings.Ph1UpstreamPortTransmitterPreset   = 4;
  }
  PcieGen4LinkEqSettings->ProjectSettings.PipePresetToCoefficientQuerySupported = FALSE;
  PcieGen4LinkEqSettings->PlatformSettings.PCETTimer                            = 2;
  PcieGen4LinkEqSettings->ProjectSettings.FullSwingValue                        = 0x30;
  PcieGen4LinkEqSettings->ProjectSettings.LowFrequencyValue                     = 0x10;
  PcieGen4LinkEqSettings->PlatformSettings.EqPh3Bypass                          = 0;
  PcieGen4LinkEqSettings->PlatformSettings.EqPh23Bypass                         = 0;
  PcieGen4LinkEqSettings->PlatformSettings.TsLockTimer                          = 0;

  PcieGetGen5PresetConfig (pInst, &PcieGen5LinkEqSettings->PlatformSettings);
  PcieGen5PresetToCoefficientConfig (pInst, &PcieGen5LinkEqSettings->ProjectSettings);
  PcieGen5LinkEqSettings->PlatformSettings.PcieLinkEqMethod                     = PcieLinkHardwareEq;
  PcieGen5LinkEqSettings->PlatformSettings.PcieLinkEqMode                       = PcieLinkEqPresetMode;
  PcieGen5LinkEqSettings->PlatformSettings.Ph1DownstreamPortTransmitterPreset   = 5;
  PcieGen5LinkEqSettings->PlatformSettings.Ph1UpstreamPortTransmitterPreset     = 5;
  PcieGen5LinkEqSettings->ProjectSettings.PipePresetToCoefficientQuerySupported = FALSE;
  PcieGen5LinkEqSettings->PlatformSettings.PCETTimer                            = 2;
  PcieGen5LinkEqSettings->ProjectSettings.FullSwingValue                        = 0x30;
  PcieGen5LinkEqSettings->ProjectSettings.LowFrequencyValue                     = 0x10;
  PcieGen5LinkEqSettings->PlatformSettings.EqPh3Bypass                          = 0;
  PcieGen5LinkEqSettings->PlatformSettings.EqPh23Bypass                         = 0;
  PcieGen5LinkEqSettings->PlatformSettings.TsLockTimer                          = 1;
}

/**
  Update Private Config for Ip Instance

  @param[in]     pInst              Pointer to IP inst
**/

VOID
PcieUpdatePrivateConfig (
  IN IP_PCIE_INST       *pInst,
  IN PCH_PCIE_CONFIG    *PcieConfig
)
{
  pInst->PrivateConfig.VnnRemovalSupported = FALSE;
  pInst->PrivateConfig.BusMin              = PcdGet8 (PcdSiliconInitTempPciBusMin);
  pInst->PrivateConfig.BusMax              = PcdGet8 (PcdSiliconInitTempPciBusMax);
  pInst->PrivateConfig.TPowerOn            = 0x5;
  if ((pInst->SipPhyVersion == IP_SC16)) {
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[0]  = 0xFD5AFF3C;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[1]  = 0xA028EF54;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[2]  = 0x86249024;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[3]  = 0x4717471B;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[4]  = 0x4215;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[5]  = 0x0;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[6]  = 0x0;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[7]  = 0x0;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[8]  = 0x2F0D300F;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[9]  = 0x00002B0C;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[10] = 0x0;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[11] = 0x0;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[12] = 0x0;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[13] = 0x0;
  }
  if ((pInst->SipPhyVersion == IP_SE32)) {
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[0]  = 0xFF5EFF40;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[1]  = 0xAA2FEF60;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[2]  = 0x862C912C;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[3]  = 0x481D4A21;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[4]  = 0x431C;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[5]  = 0X0;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[6]  = 0X0;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[7]  = 0X0;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[8]  = 0x2E0F3111;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[9]  = 0x2C0E;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[10] = 0x0;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[11] = 0x2F0D290E;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[12] = 0x2C0F;
    pInst->PrivateConfig.PtmConfig.PtmPipeStageDelay[13] = 0x0;
  }
  pInst->PrivateConfig.PtmConfig.PtmConfig             = 0x40452;
  pInst->PrivateConfig.L1StandardConfig.NoSnoopEnable  = 0x1;
  pInst->PrivateConfig.L1StandardConfig.NoSnoopScale   = 0x2;
  pInst->PrivateConfig.L1StandardConfig.NoSnoopValue   = 0x41;
  pInst->PrivateConfig.L1StandardConfig.SnoopEnable    = 0x1;
  pInst->PrivateConfig.L1StandardConfig.SnoopScale     = 0x2;
  pInst->PrivateConfig.L1StandardConfig.SnoopValue     = 0x41;
  pInst->PrivateConfig.L1p1Config.NoSnoopEnable        = 0x1;
  pInst->PrivateConfig.L1p1Config.NoSnoopScale         = 0x2;
  pInst->PrivateConfig.L1p1Config.NoSnoopValue         = 0x41;
  pInst->PrivateConfig.L1p1Config.SnoopEnable          = 0x1;
  pInst->PrivateConfig.L1p1Config.SnoopScale           = 0x2;
  pInst->PrivateConfig.L1p1Config.SnoopValue           = 0x41;
  pInst->PrivateConfig.L1p2Config.NoSnoopEnable        = 0x1;
  pInst->PrivateConfig.L1p2Config.NoSnoopScale         = 0x2;
  pInst->PrivateConfig.L1p2Config.NoSnoopValue         = 0x82;
  pInst->PrivateConfig.L1p2Config.SnoopEnable          = 0x1;
  pInst->PrivateConfig.L1p2Config.SnoopScale           = 0x2;
  pInst->PrivateConfig.L1p2Config.SnoopValue           = 0x82;
  pInst->PrivateConfig.LtrSubL11Npg.NoSnoopEnable      = 0x1;
  pInst->PrivateConfig.LtrSubL11Npg.NoSnoopScale       = 0x2;
  pInst->PrivateConfig.LtrSubL11Npg.NoSnoopValue       = 0x2D;
  pInst->PrivateConfig.LtrSubL11Npg.SnoopEnable        = 0x1;
  pInst->PrivateConfig.LtrSubL11Npg.SnoopScale         = 0x2;
  pInst->PrivateConfig.LtrSubL11Npg.SnoopValue         = 0x2D;

  //
  // RpIndex is 0 based whereas PchPciePort8xhDecodePortIndex is 1 based indexing.
  //
  if (PcieConfig->PcieCommonConfig.EnablePort8xhDecode && PcieConfig->PchPciePort8xhDecodePortIndex == ((pInst->RpIndex) + 1)) {
    pInst->PrivateConfig.EnablePort80Decode = TRUE;
  }

  pInst->PrivateConfig.MultiVcEnabled                 = TRUE;
  pInst->PrivateConfig.NumOfVcs                       = 1;
  pInst->PrivateConfig.AtomicOpEnabled                = TRUE;

  pInst->PrivateConfig.PayLoadConfig.Uprs             = 0x7;
  pInst->PrivateConfig.PayLoadConfig.Imps             = 0x7;
  pInst->PrivateConfig.PayLoadConfig.Unrs             = 0x1;
  pInst->PrivateConfig.PayLoadConfig.Imrs             = 0x1;
  pInst->PrivateConfig.BlockChainTimer                = V_PGCTRL_PMREQBLKRSPT_25US;

  if (PtlIsPcdH () && (pInst->RpIndex == 10 || pInst->RpIndex == 11)){
    pInst->PrivateConfig.ClockDcgEnable = FALSE;
  } else {
    pInst->PrivateConfig.ClockDcgEnable = TRUE;
  }

  if (pInst->RpIndex <= 10) {
    pInst->PrivateConfig.ChainTimersValue               = V_PCIE_CFG_COCTL_CT_14NS;
  } else {
    pInst->PrivateConfig.ChainTimersValue               = V_PCIE_CFG_COCTL_CT_3NS;
  }

  PcieGetProjectDefaultEqConfiguration (pInst, &pInst->PrivateConfig.Gen3EqSettings, &pInst->PrivateConfig.Gen4EqSettings, &pInst->PrivateConfig.Gen5EqSettings);
}
/**
  Update Private Config for PCIE IP Instance

  @param[in]  pInst    *pInst
**/
VOID
PcieUpdateControllerConfig (
  IN IP_PCIE_INST       *pInst
  )
{
  if (PtlIsPcdP ()) {
    if (pInst->RpIndex < 8) {
      pInst->PrivateConfig.RootPortIndexInController =  pInst->RpIndex % PCH_PCIE_CONTROLLER_PORTS;
      pInst->PrivateConfig.RootPortsInController     = 4;
      pInst->PrivateConfig.LanesPerRootPort          = 1;
    } else if (pInst->RpIndex < 10) {
      pInst->PrivateConfig.RootPortIndexInController = pInst->RpIndex % PEG_PCIE_CONTROLLER_PORTS;
      pInst->PrivateConfig.RootPortsInController     = 2;
      pInst->PrivateConfig.LanesPerRootPort          = 2;
    }
  } else {
    if (pInst->RpIndex < 8) {
      pInst->PrivateConfig.RootPortIndexInController =  pInst->RpIndex % PCH_PCIE_CONTROLLER_PORTS;
      pInst->PrivateConfig.RootPortsInController     = 4;
      pInst->PrivateConfig.LanesPerRootPort          = 1;
    } else if (pInst->RpIndex < 10) {
      pInst->PrivateConfig.RootPortIndexInController = pInst->RpIndex % PEG_PCIE_CONTROLLER_PORTS;
      pInst->PrivateConfig.RootPortsInController     = 2;
      pInst->PrivateConfig.LanesPerRootPort          = 2;
    } else if (pInst->RpIndex < 11) {
      pInst->PrivateConfig.RootPortIndexInController = 0;
      pInst->PrivateConfig.RootPortsInController     = 1;
      pInst->PrivateConfig.LanesPerRootPort          = 8;
    } else if (pInst->RpIndex < 12) {
      pInst->PrivateConfig.RootPortIndexInController = 0;
      pInst->PrivateConfig.RootPortsInController     = 1;
      pInst->PrivateConfig.LanesPerRootPort          = 4;
    }
  }
}

/**
  This function returns PID according to Root Port Number

  @param[in] RpIndex     Root Port Index (0-based)

  @retval P2SB_PID   Returns PID for SBI Access
**/
P2SB_PID
PtlPcdGetRpSbiPid (
  IN UINTN  RpIndex
  )
{
  UINT32   ControllerIndex;

  ControllerIndex = PtlPcdRpIndexToControllerIndex(RpIndex);
  return PtlPcdGetPcieControllerSbiPid (ControllerIndex);
}

BOOLEAN
PtlPcdIsRpIocDecoded (
  IN UINT32  RpIndex
  )
{
  UINT32  ControllerIndex;

  ControllerIndex = PtlPcdRpIndexToControllerIndex ((UINTN)RpIndex);
  if (PtlIsPcdP ()) {
    if (ControllerIndex < ARRAY_SIZE (mPtlPcdPPcieControllerInfo)) {
      return mPtlPcdPPcieControllerInfo[ControllerIndex].IocDecoded;
    }
  } else {
    if (ControllerIndex < ARRAY_SIZE (mPtlPcdHPcieControllerInfo)) {
      return mPtlPcdHPcieControllerInfo[ControllerIndex].IocDecoded;
    }
  }

  return FALSE;
}

/**
  Get the FID number of the PCIe root port.

  @param[in] RpIndex  Index of the root port

  @return FID number to use when sending SBI msg
**/
UINT16
GetRpSbiFid (
  IN UINTN  RpIndex
  )
{
  return PtlPcdPcieRpGetSbiFid (RpIndex);
}

/**
  Checks if root port is decoded by IOC.

  @param[in] RpIndex  Root port index.

  @return TRUE if root port is IOC decoded.
**/
BOOLEAN
IsRpIocDecoded (
  IN UINT32  RpIndex
  )
{
  return PtlPcdIsRpIocDecoded (RpIndex);
}


/**
  Checks whether RpIndex is Controller.

  @param[in] RpIndex  Root port index.

  @return TRUE if RpIndex is Controller.
**/
BOOLEAN
IsController (
  IN UINT32                      RpIndex
  )
{
  if (PtlIsPcdP ()) {
    if (RpIndex == 0 || RpIndex == 4 || RpIndex == 8) {
      return TRUE;
    }
  } else {
    if (RpIndex == 0 || RpIndex == 4 || RpIndex == 8 || RpIndex == 10 || RpIndex == 11) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Get Pch Pcie Root Port Device and Function Number by Root Port physical Number

  @param[in]  RpNumber              Root port physical number. (0-based)
  @param[out] RpDev                 Return corresponding root port device number.
  @param[out] RpFun                 Return corresponding root port function number.

  @retval     EFI_SUCCESS           Root port device and function is retrieved
  @retval     EFI_INVALID_PARAMETER RpNumber is invalid
**/
EFI_STATUS
EFIAPI
GetPchPcieRpDevFun (
  IN  UINTN   RpNumber,
  OUT UINTN   *RpDev,
  OUT UINTN   *RpFun
  )
{
  if ((RpDev == NULL) || (RpFun == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *RpDev = 0;
  *RpFun = 0;
  return PtlPcdGetPcieRpDevFun ((UINTN) RpNumber, (UINT16 *) RpDev, (UINT16 *) RpFun);
}

/**
  This function returns PID according to Root Port Number

  @param[in] RpIndex     Root Port Index (0-based)

  @retval P2SB_PID       Returns PID for SBI Access
**/
P2SB_PID
GetRootPortSbiPid (
  IN UINTN  RpIndex
  )
{
  return PtlPcdGetRpSbiPid ((UINTN) RpIndex);
}

/**
  Returns the PHY version of the PCIe root port.

  @param[in] RpIndex  Index of the root port in PCH

  @return Version of the PHY
**/
IP_PCIE_SIP_PHY_VERSION
PcieRpGetSipPhyVer (
  IN UINT32  RpIndex
  )
{
  if (PtlIsPcdP ()) {
    if ((RpIndex == 8) || (RpIndex == 9)) {
      return IP_SE32;
    }
  } else {
    if ((RpIndex == 8) || (RpIndex == 9) || (RpIndex == 10) || (RpIndex == 11)) {
      return IP_SE32;
    }
  }
  return IP_SC16;
}
