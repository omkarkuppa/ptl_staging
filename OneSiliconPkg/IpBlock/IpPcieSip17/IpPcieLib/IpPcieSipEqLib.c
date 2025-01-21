/** @file
  This file contains common equalization code for PCIe SIP controller

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

#include <IpWrapper.h>
#include <IpPcieRegs.h>
#include <IpPcieDefines.h>
#include "IpPcieSipInitLib.h"

/**
  Reads out internal signal state from the PCIe RP hardware.
  This is accomplished with monitor mux register in RP.

  @param[in] pInst        *pInst
  @param[in] SignalIndex  Index of the signal to read

  @return Value of the signal state.
**/
static
UINT32
SipGetInternalSignalState (
  IP_PCIE_INST          *pInst,
  UINT32                MonitorCategory,
  UINT32                MonitorSegment,
  UINT32                MonitorSignal
  )
{
  MM_PCIE_CFG_STRUCT  Mmux;
  CDM_PCIE_CFG_STRUCT  Cdm;

  if (MonitorCategory > 0xF ||
      MonitorSegment >= 0xF ||
      MonitorSignal > 0xFF) {
    PRINT_LEVEL1 ("Failed to read monitor signal, index out of range C:%X S:%X S:%X\n",
                           MonitorCategory, MonitorSegment, MonitorSignal);
    return 0xFFFFFFFF;
  }
  Cdm.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, CDM_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Cdm.Bits.mcs = MonitorCategory;
  Cdm.Bits.mss = MonitorSegment;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, CDM_PCIE_CFG_REG, Cdm.Data, IpWrRegFlagSize32Bits);
  Mmux.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, MM_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Mmux.Bits.mss = MonitorSignal;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, MM_PCIE_CFG_REG, Mmux.Data, IpWrRegFlagSize32Bits);

  Mmux.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, MM_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  return Mmux.Bits.msst;
}

/**
  Dumps best EQ coefficients values that were selected during EQ.

  @param[in]  pInst        *pInst
  @param[in]  RpLaneIndex  Lane index of the RP
**/
static
void
SipDumpBestEqCoefficients (
  IP_PCIE_INST          *pInst,
  UINT32                RpLaneIndex
  )
{
  UINT32         SignalState;
  IP_PCIE_SPEED  MaxRpSpeed;

  MaxRpSpeed = (IP_PCIE_SPEED) SipGetMaxLinkSpeed (pInst);


  PRINT_LEVEL1 ("Lane %d:\n", RpLaneIndex);
  if (MaxRpSpeed >= IpPcieGen3) {
    SignalState = SipGetInternalSignalState (
                    pInst,
                    R_SIP_MONITOR_CAT2,
                    RpLaneIndex,
                    R_SIP_MONITOR_CAT2_BEST4COEFF_GEN3);
    PRINT_LEVEL1 ("GEN3: %X\n", SignalState);
  }

  if (MaxRpSpeed >= IpPcieGen4) {
    SignalState = SipGetInternalSignalState (
                    pInst,
                    R_SIP_MONITOR_CAT2,
                    RpLaneIndex,
                    R_SIP_MONITOR_CAT2_BEST4COEFF_GEN4);
    PRINT_LEVEL1 ("GEN4: %X\n", SignalState);
  }

  if (MaxRpSpeed >= IpPcieGen5) {
    SignalState = SipGetInternalSignalState (
                    pInst,
                    R_SIP_MONITOR_CAT2,
                    RpLaneIndex,
                    R_SIP_MONITOR_CAT2_BEST4COEFF_GEN5);
    PRINT_LEVEL1 ("GEN5: %X\n", SignalState);
  }

}

/**
  Dump Figure of merit scoreboard(FOMS) for each of the root port lanes.

  @param[in] pInst        *pInst
  @param[in] RpLaneIndex  Index of the root port lane
**/
static
void
SipDumpFoms (
  IP_PCIE_INST          *pInst,
  UINT32                RpLaneIndex
  )
{
  UINT32    ListEntryIndex;
  UINT32    Foms;
  FOMS_PCIE_CFG_STRUCT    PcieFoms;

  PRINT_LEVEL1 ("%d.%d FOM Scoreboard:\n", SipGetPortNum (pInst), RpLaneIndex);
  for (ListEntryIndex = 0; ListEntryIndex < 10; ListEntryIndex += 3) {
    //
    // Program Figure Of Merit Status register 0x0464
    // FOMV field holds FOM values for 3 list entires. To select correct triplet
    // we use Index field.
    //
    PcieFoms.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, FOMS_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    PcieFoms.Bits.idx = ListEntryIndex/3;
    PcieFoms.Bits.ln  = RpLaneIndex;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, FOMS_PCIE_CFG_REG, PcieFoms.Data, IpWrRegFlagSize32Bits);
    PRINT_LEVEL1 ("%d:%03d  ", ListEntryIndex, PcieFoms.Bits.fomsv);

    Foms = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, FOMS_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    PRINT_LEVEL1 ("%d:%03d  ", ListEntryIndex,     (Foms & B_PCIE_CFG_FOMS_FOMSV0) >> N_PCIE_CFG_FOMS_FOMSV0);
    PRINT_LEVEL1 ("%d:%03d  ", ListEntryIndex + 1, (Foms & B_PCIE_CFG_FOMS_FOMSV1) >> N_PCIE_CFG_FOMS_FOMSV1);
    PRINT_LEVEL1 ("%d:%03d\n", ListEntryIndex + 2, (Foms & B_PCIE_CFG_FOMS_FOMSV2) >> N_PCIE_CFG_FOMS_FOMSV2);
  }
}

/**
  Reset and enable Recovery Entry and Idle Framing Error Count

  @param[in] pInst  *pInst
**/
void
SipResetErrorCounts (
  IP_PCIE_INST      *pInst
  )
{
 EQCFG1_PCIE_CFG_STRUCT     EqCfg1;

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  /// Program Equalization Configuration 1 register 0x0450
  EqCfg1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, EQCFG1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  EqCfg1.Bits.reifece = 0;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, EQCFG1_PCIE_CFG_REG, EqCfg1.Data, IpWrRegFlagSize32Bits);

  EqCfg1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, EQCFG1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  EqCfg1.Bits.reifece = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, EQCFG1_PCIE_CFG_REG, EqCfg1.Data, IpWrRegFlagSize32Bits);

  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);
}

/**
  Dump PCIe HW EQ results. For internal use only.

  @param[in] pInst  *pInst
**/
void
SipDumpHwEqResults (
  IP_PCIE_INST      *pInst
  )
{
  UINT32    RpLaneIndex;
  UINT32    MaxLinkWidth;
  UINT32    TimeoutUs;
  UINT8     PcieSpeed;
  LSTS_PCIE_CFG_STRUCT        Lsts;

  MaxLinkWidth = SipGetMaxLinkWidth (pInst);
  /// Program Link Status register 0x0052
  Lsts.Data = (UINT16)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LSTS_PCIE_CFG_REG, IpWrRegFlagSize16Bits);

  if (Lsts.Bits.la == 0) {
    PRINT_ERROR ( "No link after EQ!\n");
    return;
  }

  //
  // Wait for link training to complete
  //
  for (TimeoutUs = 0; TimeoutUs < 5000000; TimeoutUs += 10) {
    Lsts.Data = (UINT16)IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LSTS_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
    if (Lsts.Bits.lt == 0) {
      break;
    }
    IpWrDelayUs (pInst->TimeCntxt, 10);
  }
  PRINT_LEVEL1 ("HW EQ finished, results:\n");
  PRINT_LEVEL1 ("EQCFG1: 0x%08x\n", IpWrRegRead (pInst->RegCntxt_Cfg_Pri, EQCFG1_PCIE_CFG_REG, IpWrRegFlagSize32Bits));
  PRINT_LEVEL1 ("LSTS2:  0x%04x\n", IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LSTS2_PCIE_CFG_REG, IpWrRegFlagSize16Bits));
  PRINT_LEVEL1 ("Training time = %dus\n", TimeoutUs);

  PcieSpeed = pInst->PcieRpCommonConfig.PcieSpeed;
  if (PcieSpeed >= IpPcieGen3) {
    for (RpLaneIndex = 0; RpLaneIndex < MaxLinkWidth; RpLaneIndex++) {
      SipDumpFoms (pInst, RpLaneIndex);
      SipDumpBestEqCoefficients (pInst, RpLaneIndex);
    }
  }
}

/**
  Set the preset/coefficient list entry to the desired value.

  @param[in] pInst             *pInst
  @param[in] ListEntryIndex    Index of the entry
  @param[in] ListEntryValue    Value of the entry
  @param[in] TargetPcieSpeed   PCIe speed to train to
**/
static
void
SipSetPresetCoefficientListEntry (
  IP_PCIE_INST          *pInst,
  UINT32                ListEntryIndex,
  UINT32                ListEntryValue,
  IP_PCIE_SPEED         TargetPcieSpeed
  )
{
  UINT32            ListEntryRegisterIndex;
  UINT32            ListEntryFieldIndex;
  UINT32            Data32And;
  UINT32            Data32Or;
  UINT32            Data32;

  if ((ListEntryValue & ~0x3F)) {
     IpWrError (pInst->ErrorAssert, IpWrErrorMajorError, __LINE__);
  }

  //
  // Preset/Coefficient list is kept in hardware in a set
  // of continous registers each of which contains 5 list entries.
  //
  ListEntryRegisterIndex = ListEntryIndex / 5;
  ListEntryFieldIndex = ListEntryIndex % 5;

  switch (TargetPcieSpeed) {
    case IpPcieGen5:
      /// Program 32 GT/s Remote Transmitter Preset Coefficient List 1 register 0x1e08
      Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PX32GRTPCL1_PCIE_MEM_RCRB_REG + (4 * ListEntryRegisterIndex), IpWrRegFlagSize32Bits);
      Data32And = (UINT32) ~(0x3F << (6 * ListEntryFieldIndex));
      Data32Or = (ListEntryValue << (6 * ListEntryFieldIndex));
      Data32 &= Data32And;
      Data32 |= Data32Or;

      IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PX32GRTPCL1_PCIE_MEM_RCRB_REG + (4 * ListEntryRegisterIndex), Data32, IpWrRegFlagSize32Bits);
      break;
    case IpPcieGen4:
      /// Program 16 GT/s Remote Transmitter Preset Coefficient List 1 register 0x04DC
      Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PX16GRTPCL1_PCIE_CFG_REG + (4 * ListEntryRegisterIndex), IpWrRegFlagSize32Bits);
      Data32And = (UINT32) ~(0x3F << (6 * ListEntryFieldIndex));
      Data32Or = (ListEntryValue << (6 * ListEntryFieldIndex));
      Data32 &= Data32And;
      Data32 |= Data32Or;
      IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PX16GRTPCL1_PCIE_CFG_REG + (4 * ListEntryRegisterIndex), Data32, IpWrRegFlagSize32Bits);
      break;
    case IpPcieGen3:
      /// Program Remote Transmitter Preset Coefficient List 1 register 0x0454
      Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, RTPCL1_PCIE_CFG_REG + (4 * ListEntryRegisterIndex), IpWrRegFlagSize32Bits);
      Data32And = (UINT32) ~(0x3F << (6 * ListEntryFieldIndex));
      Data32Or = (ListEntryValue << (6 * ListEntryFieldIndex));
      Data32 &= Data32And;
      Data32 |= Data32Or;
      IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, RTPCL1_PCIE_CFG_REG + (4 * ListEntryRegisterIndex), Data32, IpWrRegFlagSize32Bits);
      break;
    default:
      break;
  }
}

/**
  Sets the Gen3/Gen4 Preset EQ mode that will be used by root port.

  @param[in] pInst             *pInst
  @param[in] TargetPcieSpeed   PCIe speed to train to
**/
static
void
SipSetPresetEqMode (
  IP_PCIE_INST          *pInst,
  IP_PCIE_SPEED         TargetPcieSpeed
  )
{
  RTPCL1_PCIE_CFG_STRUCT                RtPcl1;
  PX32GRTPCL1_PCIE_MEM_RCRB_STRUCT      Px32grtpcl1;
  PX16GRTPCL1_PCIE_CFG_STRUCT           Px16GrtPcl1;

  switch (TargetPcieSpeed) {
    case IpPcieGen5:
    /// Program 32 GT/s Remote Transmitter Preset Coefficient List 1 registe 0x1E08
      Px32grtpcl1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PX32GRTPCL1_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
      Px32grtpcl1.Bits.pcm = 0;
      IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PX32GRTPCL1_PCIE_MEM_RCRB_REG, Px32grtpcl1.Data, IpWrRegFlagSize32Bits);
      break;
    case IpPcieGen4:
      /// Program 16 GT/s Remote Transmitter Preset Coefficient List 1 register 0x04DC
      Px16GrtPcl1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PX16GRTPCL1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
      Px16GrtPcl1.Bits.pcm = 0;
      IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PX16GRTPCL1_PCIE_CFG_REG, Px16GrtPcl1.Data, IpWrRegFlagSize32Bits);
      break;
    case IpPcieGen3:
      /// Remote Transmitter Preset Coefficient List 1 register 0x0454
      RtPcl1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, RTPCL1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
      RtPcl1.Bits.pcm = 0;
      IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, RTPCL1_PCIE_CFG_REG, RtPcl1.Data, IpWrRegFlagSize32Bits);
      break;
    default:
      PRINT_ERROR ( "Incorrect speed for Preset PCIe link EQ mode\n");
      break;
  }
}

/**
  Sets the Gen3/Gen4 Coefficient EQ mode that will be used by root port.

  @param[in] pInst             *pInst
  @param[in] TargetPcieSpeed   PCIe speed to train to
**/
static
void
SipSetCoefficientEqMode (
  IP_PCIE_INST          *pInst,
  IP_PCIE_SPEED         TargetPcieSpeed
  )
{
  PX16GRTPCL1_PCIE_CFG_STRUCT      Px16GrtPcl1;
  RTPCL1_PCIE_CFG_STRUCT           RtPcl1;
  PX32GRTPCL1_PCIE_MEM_RCRB_STRUCT Px32Grtpcl1;

  switch (TargetPcieSpeed) {
    case IpPcieGen5:
      /// Program 32 GT/s Remote Transmitter Preset Coefficient List 1 register 0x1e08
      Px32Grtpcl1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PX32GRTPCL1_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
      Px32Grtpcl1.Bits.pcm = 1;
      IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PX32GRTPCL1_PCIE_MEM_RCRB_REG, Px32Grtpcl1.Data, IpWrRegFlagSize32Bits);
      break;
    case IpPcieGen4:
      /// Program 16 GT/s Remote Transmitter Preset Coefficient List 1 register 0x04DC
      Px16GrtPcl1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PX16GRTPCL1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
      Px16GrtPcl1.Bits.pcm = 1;
      IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PX16GRTPCL1_PCIE_CFG_REG, Px16GrtPcl1.Data, IpWrRegFlagSize32Bits);
      break;
    case IpPcieGen3:
      /// Remote Transmitter Preset Coefficient List 1 register 0x0454
      RtPcl1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, RTPCL1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
      RtPcl1.Bits.pcm = 1;
      IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, RTPCL1_PCIE_CFG_REG, RtPcl1.Data, IpWrRegFlagSize32Bits);
      break;
    default:
      PRINT_ERROR ( "Incorrect speed for Coefficient PCIe link EQ mode\n");
      break;
  }
}

/**
  Sets transmitter lane presets for both upstream and downstream port for Gen3.

  @param[in] IP_PCIE_INST          *pInst
  @param[in] LaneIndex             Index of the transmitter lane
  @param[in] DownstreamPortPreset  Preset value for the downstream port
  @param[in] UpstreamPortPreset    Preset value for the upstream port
**/
static
void
SipSetGen3TransmitterLanePreset (
  IP_PCIE_INST          *pInst,
  UINT32                LaneIndex,
  UINT32                DownstreamPortPreset,
  UINT32                UpstreamPortPreset
  )
{
  UINT32    PresetRegisterIndex;
  UINT32    DownstreamPortPresetFieldIndex;
  UINT32    UpstreamPortPresetFieldIndex;
  UINT32    Data32;
  UINT32    Data32And;
  UINT32    Data32Or;

  //
  // Each register holds preset values for 2 lanes.
  //
  PresetRegisterIndex = LaneIndex / 2;

  //
  // Each of the lanes configuration spaces contains fields
  // for downstream and upstream preset. To select correct preset
  // we calculate the lane index inside the register by doing %2 operation
  // on global lane index and then multiply it by 2 to get downstream preset
  // index field. The layout of the register is as follows:
  // -Downstream port lane 0 preset configration - preset config 0
  // -Upstream port lane 0 preset configuration - preset config 1
  // -Downstream port lane 1 preset configuration - preset config 2
  // -Upstream port lane 1 preste configuration - preset config 3
  //
  DownstreamPortPresetFieldIndex = (LaneIndex % 2) * 2;
  UpstreamPortPresetFieldIndex = DownstreamPortPresetFieldIndex + 1;

  /// Program Lane 0 And Lane 1 Equalization Control register 0x0A3C
  Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, L01EC_PCIE_CFG_REG + (4 * PresetRegisterIndex), IpWrRegFlagSize32Bits);
  Data32And = ~(UINT32)((B_EX_L01EC_TP << (DownstreamPortPresetFieldIndex * S_EX_L01EC_TP)) |
              (B_EX_L01EC_TP << (UpstreamPortPresetFieldIndex * S_EX_L01EC_TP)));
  Data32Or = (DownstreamPortPreset << (DownstreamPortPresetFieldIndex * S_EX_L01EC_TP)) |
             (UpstreamPortPreset << (UpstreamPortPresetFieldIndex * S_EX_L01EC_TP));

  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, L01EC_PCIE_CFG_REG + (4 * PresetRegisterIndex), Data32, IpWrRegFlagSize32Bits);
}

/**
  Sets transmitter lane presets for both upstream and downstream port for Gen4.

  @param[in] IP_PCIE_INST          *pInst
  @param[in] LaneIndex             Index of the transmitter lane
  @param[in] DownstreamPortPreset  Preset value for the downstream port
  @param[in] UpstreamPortPreset    Preset value for the upstream port
**/
static
void
SipSetGen4TransmitterLanePreset (
  IP_PCIE_INST          *pInst,
  UINT32                LaneIndex,
  UINT32                DownstreamPortPreset,
  UINT32                UpstreamPortPreset
  )
{
  UINT32    PresetRegisterIndex;
  UINT32    DownstreamPortPresetFieldIndex;
  UINT32    UpstreamPortPresetFieldIndex;
  UINT16    Data16;
  UINT16    Data16And;
  UINT16    Data16Or;

  //
  // Each register holds preset values for 2 lanes.
  //
  PresetRegisterIndex = LaneIndex / 2;

  //
  // Each of the lanes configuration spaces contains fields
  // for downstream and upstream preset. To select correct preset
  // we calculate the lane index inside the register by doing %2 operation
  // on global lane index and then multiply it by 2 to get downstream preset
  // index field. The layout of the register is as follows:
  // -Downstream port lane 0 preset configration - preset config 0
  // -Upstream port lane 0 preset configuration - preset config 1
  // -Downstream port lane 1 preset configuration - preset config 2
  // -Upstream port lane 1 preste configuration - preset config 3
  //
  DownstreamPortPresetFieldIndex = (LaneIndex % 2) * 2;
  UpstreamPortPresetFieldIndex = DownstreamPortPresetFieldIndex + 1;

  /// Program Physical Layer 16.0 GT/s Lane 01 Equalization Control Register register 0x0ABC
  Data16 = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PL16L01EC_PCIE_CFG_REG + (2 * PresetRegisterIndex), IpWrRegFlagSize16Bits);
  Data16And = ~(UINT16)((B_EX_L01EC_TP << (DownstreamPortPresetFieldIndex * S_EX_PL16L01EC_TP)) |
              (B_EX_L01EC_TP << (UpstreamPortPresetFieldIndex * S_EX_PL16L01EC_TP)));
  Data16Or = (UINT16)((DownstreamPortPreset << (DownstreamPortPresetFieldIndex * S_EX_PL16L01EC_TP)) |
             (UpstreamPortPreset << (UpstreamPortPresetFieldIndex * S_EX_PL16L01EC_TP)));

  Data16 &= Data16And;
  Data16 |= Data16Or;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PL16L01EC_PCIE_CFG_REG + (2 * PresetRegisterIndex), Data16, IpWrRegFlagSize16Bits);
}

/**
  Sets transmitter lane presets for both upstream and downstream port for Gen5.

  @param[in] IP_PCIE_INST          *pInst
  @param[in] LaneIndex             Index of the transmitter lane
  @param[in] DownstreamPortPreset  Preset value for the downstream port
  @param[in] UpstreamPortPreset    Preset value for the upstream port
**/
static
void
SipSetGen5TransmitterLanePreset (
  IP_PCIE_INST          *pInst,
  UINT32                LaneIndex,
  UINT32                DownstreamPortPreset,
  UINT32                UpstreamPortPreset
  )
{
  UINT32    PresetRegisterIndex;
  UINT32    DownstreamPortPresetFieldIndex;
  UINT32    UpstreamPortPresetFieldIndex;
  UINT32    Data32;
  UINT32    Data32And;
  UINT32    Data32Or;
  //
  // Each register holds preset values for 4 lanes.
  //
  PresetRegisterIndex = LaneIndex / 4;

  //
  // Each of the lanes configuration spaces contains fields
  // for downstream and upstream preset. To select correct preset
  // we calculate the lane index inside the register by doing %2 operation
  // on global lane index and then multiply it by 2 to get downstream preset
  // index field. The layout of the register is as follows:
  // -Downstream port lane 0 preset configration - preset config 0
  // -Upstream port lane 0 preset configuration - preset config 1
  // -Downstream port lane 1 preset configuration - preset config 2
  // -Upstream port lane 1 preste configuration - preset config 3
  // -Downstream port lane 2 preset configration - preset config 4
  // -Upstream port lane 2 preset configuration - preset config 5
  // -Downstream port lane 3 preset configuration - preset config 6
  // -Upstream port lane 3 preste configuration - preset config 7
  //
  DownstreamPortPresetFieldIndex = (LaneIndex % 4) * 2;
  UpstreamPortPresetFieldIndex = DownstreamPortPresetFieldIndex + 1;

  /// Program 32.0 GT/s Lane 0123 Equalization Control Register registers 0x0AFC
  Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, G5LANEEQCTL_0_PCIE_CFG_REG + (4 * PresetRegisterIndex), IpWrRegFlagSize32Bits);
  Data32And = ~(UINT32)((B_EX_L01EC_TP << (DownstreamPortPresetFieldIndex * S_EX_PL16L01EC_TP)) |
              (B_EX_L01EC_TP << (UpstreamPortPresetFieldIndex * S_EX_PL16L01EC_TP)));
  Data32Or = (UINT32)((DownstreamPortPreset << (DownstreamPortPresetFieldIndex * S_EX_PL16L01EC_TP)) |
             (UpstreamPortPreset << (UpstreamPortPresetFieldIndex * S_EX_PL16L01EC_TP)));

  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, G5LANEEQCTL_0_PCIE_CFG_REG + (4 * PresetRegisterIndex), Data32, IpWrRegFlagSize32Bits);
}

/**
  Sets required transmitter presets for the root port.
  Those presets will be used during phase 1 of the equalization process
  to establish initial communication at higher speed.

  @param[in] IP_PCIE_INST            *pInst
  @param[in] PcieGen3LinkEqSettings  PCIe link EQ settings
  @param[in] PcieGen4LinkEqSettings  PCIe link EQ settings
  @param[in] PcieGen5LinkEqSettings  PCIe link EQ settings
  @param[in] PcieSpeed               PCIe speed to be trained to
**/
void
SipSetTransmitterPresets (
  IP_PCIE_INST                *pInst,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen3LinkEqSettings,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen4LinkEqSettings,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen5LinkEqSettings,
  IP_PCIE_SPEED               PcieSpeed
  )
{
  UINT32    LaneIndex;

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  // This code only supports PCIe root ports with up
  // to 4 PCIe lanes.
  //
  for (LaneIndex = 0; LaneIndex < SipGetMaxLinkWidth (pInst); LaneIndex++) {
    switch (PcieSpeed) {
      case IpPcieGen5:
        SipSetGen5TransmitterLanePreset (
          pInst,
          LaneIndex,
          PcieGen5LinkEqSettings->PlatformSettings.Ph1DownstreamPortTransmitterPreset,
          PcieGen5LinkEqSettings->PlatformSettings.Ph1UpstreamPortTransmitterPreset
          );
      case IpPcieGen4:
        SipSetGen4TransmitterLanePreset (
          pInst,
          LaneIndex,
          PcieGen4LinkEqSettings->PlatformSettings.Ph1DownstreamPortTransmitterPreset,
          PcieGen4LinkEqSettings->PlatformSettings.Ph1UpstreamPortTransmitterPreset
          );
      case IpPcieGen3:
        SipSetGen3TransmitterLanePreset (
          pInst,
          LaneIndex,
          PcieGen3LinkEqSettings->PlatformSettings.Ph1DownstreamPortTransmitterPreset,
          PcieGen3LinkEqSettings->PlatformSettings.Ph1UpstreamPortTransmitterPreset
          );
        break;
      default:
        PRINT_ERROR ( "Lane presets not available at this speed\n");
        break;
    }
  }
  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);
}

/**
  Start Equalization process

  @param[in] IP_PCIE_INST         *pInst
  @param[in] TargetPcieSpeed      PCIe speed to be trained to
**/
void
SipStartEqualization (
  IP_PCIE_INST          *pInst,
  IP_PCIE_SPEED         TargetPcieSpeed
  )
{
  LCTL_PCIE_CFG_STRUCT        Lctl;
  LCTL2_PCIE_CFG_STRUCT       Lctl2;
  LCTL3_PCIE_CFG_STRUCT       Lctl3;

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  //
  // Program Link Control 2 register 0x0070
  // Set target link speed
  //
  Lctl2.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCTL2_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
  Lctl2.Bits.tls = 0;
  if (TargetPcieSpeed >= IpPcieGen5) {
    Lctl2.Data |= IpPcieGen5;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LCTL2_PCIE_CFG_REG, Lctl2.Data, IpWrRegFlagSize16Bits);
  } else if (TargetPcieSpeed >= IpPcieGen4) {
    Lctl2.Data |= IpPcieGen4;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LCTL2_PCIE_CFG_REG, Lctl2.Data, IpWrRegFlagSize16Bits);
  } else if (TargetPcieSpeed >= IpPcieGen3) {
    Lctl2.Data |= IpPcieGen3;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LCTL2_PCIE_CFG_REG, Lctl2.Data, IpWrRegFlagSize16Bits);
  }
  //
  // Program Link Control 3 register 0x0A34
  // Set the perform equalization bit and start the link retrain.
  //
  if (pInst->PcieRpCommonConfig.EqPhBypass == 0) {
  Lctl3.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCTL3_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Lctl3.Bits.pe = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LCTL3_PCIE_CFG_REG, Lctl3.Data, IpWrRegFlagSize32Bits);
  }

  // Program PCI Express Additional Link Control register 0x0050
  Lctl.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LCTL_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
  Lctl.Bits.rl = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, LCTL_PCIE_CFG_REG, Lctl.Data, IpWrRegFlagSize16Bits);

  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);
}

/**
  Initializes preset list that will be used during EQ

  @param[in] pInst             *pInst
  @param[in] PresetList        Pointer to the preset list
  @param[in] PresetCount       Number of presets in the list
  @param[in] TargetPcieSpeed   PCIe speed to train to
**/
static
void
SipInitializePresetList (
  IP_PCIE_INST           *pInst,
  UINT32                 *PresetList,
  UINT32                 PresetCount,
  IP_PCIE_SPEED          TargetPcieSpeed
  )
{
  UINT32    PresetIndex;

 if (PresetCount > 11) {
    PRINT_ERROR ( "PCIe EQ: Maximum number of presets exceeded, failed with preset count = %d\n", PresetCount);
    IpWrError (pInst->ErrorAssert, IpWrErrorMajorError, __LINE__);
    return;
  }

  for (PresetIndex = 0; PresetIndex < PresetCount; PresetIndex++) {
    PRINT_LEVEL1 ("PCIe EQ: Setting preset %d, preset value = %d\n", PresetIndex, PresetList[PresetIndex]);

    SipSetPresetCoefficientListEntry (pInst, PresetIndex, PresetList[PresetIndex], TargetPcieSpeed);
  }
}

/**
  Initializes coefficient list that will be used during EQ.

  @param[in] pInst             *pInst
  @param[in] CoefficientsList  Pointer on the coefficients list
  @param[in] CoefficientCount  Number of coefficients in the list
  @param[in] TargetPcieSpeed   PCIe speed to train to
**/
static
void
SipInitializeCoefficientsList (
  IP_PCIE_INST                    *pInst,
  IP_PCIE_LINK_EQ_COEFFICIENTS    *CoefficientsList,
  UINT32                          CoefficientCount,
  IP_PCIE_SPEED                      TargetPcieSpeed
  )
{
  UINT32    CoefficientIndex;
  UINT32    PreCursorListIndex;
  UINT32    PostCursorListIndex;

  if (CoefficientCount >= 10) {
    PRINT_ERROR ( "PCIe EQ: Maximum number of coefficients exceeded, failed with coefficient count = %d\n", CoefficientCount);
    IpWrError (pInst->ErrorAssert, IpWrErrorMajorError, __LINE__);
    return;
  }

  for (CoefficientIndex = 0; CoefficientIndex < CoefficientCount; CoefficientIndex++) {
    PRINT_LEVEL1 ("PCIe EQ: Setting coefficient %d, pre-cursor = %d, post-cursor = %d\n",
            CoefficientIndex,
            CoefficientsList[CoefficientIndex].PreCursor,
            CoefficientsList[CoefficientIndex].PostCursor);
    //
    // Coefficient list in the hardware is kept as a list of alternating pre and post cursor coefficients.
    // The layout of the hardware list is as follows:
    // pre-cursor coefficient 0 - index 0
    // post-cursor coefficient 0 - index 1
    // pre-cursor coefficient 1 - index 2
    // ...
    //
    PreCursorListIndex = CoefficientIndex * 2;
    PostCursorListIndex = PreCursorListIndex + 1;

    SipSetPresetCoefficientListEntry (pInst, PreCursorListIndex, CoefficientsList[CoefficientIndex].PreCursor, TargetPcieSpeed);
    SipSetPresetCoefficientListEntry (pInst, PostCursorListIndex, CoefficientsList[CoefficientIndex].PostCursor, TargetPcieSpeed);
  }
}

/**
  Sets up the hardware for Gen3 autonomus equalization.

  @param[in] pInst               Pointer to the IP_PCIE_INST
  @param[in] PcieLinkEqSettings  PCIe link EQ settings
**/
static
void
SipSetupGen3HardwareEq (
  IP_PCIE_INST             *pInst,
  IP_PCIE_LINK_EQ_SETTINGS *PcieLinkEqSettings
  )
{
  EQCFG1_PCIE_CFG_STRUCT   EqCfg1;
  EQCFG2_PCIE_CFG_STRUCT   EqCfg2;
  EQCFG4_PCIE_CFG_STRUCT   EqCfg4;
  HAEQ_PCIE_CFG_STRUCT     Haeq;

  switch (PcieLinkEqSettings->PlatformSettings.PcieLinkEqMode) {
    case IpPcieLinkEqPresetMode:
      SipSetPresetEqMode (pInst, IpPcieGen3);
      SipInitializePresetList (
        pInst,
        PcieLinkEqSettings->PlatformSettings.Ph3PresetList,
        PcieLinkEqSettings->PlatformSettings.Ph3NumberOfPresetsOrCoefficients,
        IpPcieGen3
        );
      break;
    case IpPcieLinkEqCoefficientMode:
      SipSetCoefficientEqMode (pInst, IpPcieGen3);
      SipInitializeCoefficientsList (
        pInst,
        PcieLinkEqSettings->PlatformSettings.Ph3CoefficientsList,
        PcieLinkEqSettings->PlatformSettings.Ph3NumberOfPresetsOrCoefficients,
        IpPcieGen3
        );
      break;
    default:
     PRINT_ERROR ( "Incorrect Gen3 equalization mode = %d\n", PcieLinkEqSettings->PlatformSettings.PcieLinkEqMode);
  }

  if (PcieLinkEqSettings->PlatformSettings.EqPh23Bypass) {
    /// Program Equalization Configuration 1 register 0x0450
    EqCfg1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, EQCFG1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    EqCfg1.Bits.lep23b = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, EQCFG1_PCIE_CFG_REG, EqCfg1.Data, IpWrRegFlagSize32Bits);
  }
  if (PcieLinkEqSettings->PlatformSettings.EqPh3Bypass) {
    /// Program Equalization Configuration 1 register 0x0450
    EqCfg1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, EQCFG1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    EqCfg1.Bits.lep3b = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, EQCFG1_PCIE_CFG_REG, EqCfg1.Data, IpWrRegFlagSize32Bits);
  }

  //
  // Program Equalization Configuration 2 register 0x047C
  // This programing tells hardware to perform preset/coefficient search only up to the given preset/coefficient. We want hardware to search
  // all presets/coefficients that were passed in the list to BIOS. This register is 0 based, meaning that value of 0 will cause hardware to
  // terminate search after preset/coefficient 0.
  //
  EqCfg2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, EQCFG2_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  EqCfg2.Bits.hapcsb = (PcieLinkEqSettings->PlatformSettings.Ph3NumberOfPresetsOrCoefficients - 1);
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, EQCFG2_PCIE_CFG_REG, EqCfg2.Data, IpWrRegFlagSize32Bits);

  // Program Equalization Configuration 4 register 0x048C
  EqCfg4.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, EQCFG4_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  EqCfg4.Bits.px8gtswlpce = PcieLinkEqSettings->PlatformSettings.TsLockTimer;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, EQCFG4_PCIE_CFG_REG, EqCfg4.Data, IpWrRegFlagSize32Bits);

  //
  // Program Equalization Configuration 2 register 0x047C
  // Program the preset/coefficient evaluation time depending on the number of presets/coefficients used in EQ.
  // HW EQ has to be finished within 24ms so the more coefficient/presets we have to evaluate the less time we can
  // spend on each.
  // NOTE: Encoding of the register is not 1:1 with time in ms. Please see register description for details.
  //
  EqCfg2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, EQCFG2_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  EqCfg2.Bits.pcet = PcieLinkEqSettings->PlatformSettings.PCETTimer;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, EQCFG2_PCIE_CFG_REG, EqCfg2.Data, IpWrRegFlagSize32Bits);

  //
  // Program Equalization Configuration 1 register 0x0450
  // Set Receiver wait time for EQ evaluation to 1us.
  //
  EqCfg1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, EQCFG1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  EqCfg1.Bits.rwtneve = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, EQCFG1_PCIE_CFG_REG, EqCfg1.Data, IpWrRegFlagSize32Bits);

  //
  // Program Equalization Configuration 2 register 0x047C
  // Set the receiver eye width margin error threshold to 2 errors.
  // The value of the error threshold is calculated in hardware by
  // multiplying value from REWMET with multiplier from REWMETM. Value
  // of 0 in REWMETM means no multiplier.
  //
  EqCfg2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, EQCFG2_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  EqCfg2.Bits.rewmet = 2;
  EqCfg2.Bits.rewmetm = 0;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, EQCFG2_PCIE_CFG_REG, EqCfg2.Data, IpWrRegFlagSize32Bits);

  //
  // Program Hardware Autonomous Equalization Control register 0x0468
  // Set HAEQ.DL to 0xE
  //
  Haeq.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, HAEQ_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  Haeq.Bits.dl = 0xE;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, HAEQ_PCIE_CFG_REG, Haeq.Data, IpWrRegFlagSize32Bits);
}

/**
  Sets up the hardware for Gen4 autonomus equalization.

  @param[in] pInst               Pointer to the IP_PCIE_INST
  @param[in] PcieLinkEqSettings  PCIe link EQ settings
**/
static
void
SipSetupGen4HardwareEq (
  IP_PCIE_INST             *pInst,
  IP_PCIE_LINK_EQ_SETTINGS *PcieLinkEqSettings
  )
{
  EQCFG4_PCIE_CFG_STRUCT   EqCfg4;
  EQCFG5_PCIE_CFG_STRUCT   EqCfg5;

  switch (PcieLinkEqSettings->PlatformSettings.PcieLinkEqMode) {
    case IpPcieLinkEqPresetMode:
      SipSetPresetEqMode (pInst, IpPcieGen4);
      SipInitializePresetList (
        pInst,
        PcieLinkEqSettings->PlatformSettings.Ph3PresetList,
        PcieLinkEqSettings->PlatformSettings.Ph3NumberOfPresetsOrCoefficients,
        IpPcieGen4
        );
      break;
    case IpPcieLinkEqCoefficientMode:
      SipSetCoefficientEqMode (pInst, IpPcieGen4);
      SipInitializeCoefficientsList (pInst,
        PcieLinkEqSettings->PlatformSettings.Ph3CoefficientsList,
        PcieLinkEqSettings->PlatformSettings.Ph3NumberOfPresetsOrCoefficients,
        IpPcieGen4
        );
      break;
    default:
     PRINT_ERROR ( "Incorrect Gen4 equalization mode\n");
  }

  if (PcieLinkEqSettings->PlatformSettings.EqPh23Bypass) {
    // Program Equalization Configuration 4 register 0x048C
    EqCfg4.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, EQCFG4_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    EqCfg4.Bits.px16glep23b = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, EQCFG4_PCIE_CFG_REG, EqCfg4.Data, IpWrRegFlagSize32Bits);
  }

  if (PcieLinkEqSettings->PlatformSettings.EqPh3Bypass) {
    // Program Equalization Configuration 4 register 0x048C
    EqCfg4.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, EQCFG4_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    EqCfg4.Bits.px16glep3b = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, EQCFG4_PCIE_CFG_REG, EqCfg4.Data, IpWrRegFlagSize32Bits);
  }
  //
  // Program Equalization Configuration 5 register 0x04F8
  // This programing tells hardware to perform preset/coefficient search only up to the given preset/coefficient. We want hardware to search
  // all presets/coefficients that were passed in the list to BIOS. This register is 0 based, meaning that value of 0 will cause hardware to
  // terminate search after preset/coefficient 0.
  //
  EqCfg5.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, EQCFG5_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  EqCfg5.Bits.hapcsb = PcieLinkEqSettings->PlatformSettings.Ph3NumberOfPresetsOrCoefficients - 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, EQCFG5_PCIE_CFG_REG, EqCfg5.Data, IpWrRegFlagSize32Bits);

  // Program Equalization Configuration 4 register 0x048C
  EqCfg4.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, EQCFG4_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  EqCfg4.Bits.px8gtswlpce = PcieLinkEqSettings->PlatformSettings.TsLockTimer;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, EQCFG4_PCIE_CFG_REG, EqCfg4.Data, IpWrRegFlagSize32Bits);

  //
  // Program the preset/coefficient evaluation time depending on the number of presets/coefficients used in EQ.
  // HW EQ has to be finished within 24ms so the more coefficient/presets we have to evaluate the less time we can
  // spend on each.
  // Program Equalization Configuration 5 register 0x04F8
  // NOTE: Encoding of the register is not 1:1 with time in ms. Please see register description for details.
  //
  EqCfg5.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, EQCFG5_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  EqCfg5.Bits.pcet = PcieLinkEqSettings->PlatformSettings.PCETTimer;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, EQCFG5_PCIE_CFG_REG, EqCfg5.Data, IpWrRegFlagSize32Bits);

  //
  // Program Equalization Configuration 4 register 0x048C
  // Set Receiver wait time for EQ evaluation to 3us.
  //
  EqCfg4.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, EQCFG4_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  EqCfg4.Bits.px16ghapccpi = 0x0;
  EqCfg4.Bits.px16grwtneve = 0x1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, EQCFG4_PCIE_CFG_REG, EqCfg4.Data, IpWrRegFlagSize32Bits);
}

/**
  Program registers to Setup Gen5 Hardware Equalization

  @param[in] pInst               *pInst
  @param[in] PcieLinkEqSettings  PCIe link EQ settings
**/
void
SipSetupGen5HardwareEq (
  IP_PCIE_INST                *pInst,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieLinkEqSettings
  )
{
  PX32EQCFG1_PCIE_MEM_RCRB_STRUCT Px32EqCfg1;
  PX32EQCFG2_PCIE_MEM_RCRB_STRUCT Px32EqCfg2;

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  switch (PcieLinkEqSettings->PlatformSettings.PcieLinkEqMode) {
    case IpPcieLinkEqPresetMode:
      SipSetPresetEqMode (pInst, IpPcieGen5);
      SipInitializePresetList (
        pInst,
        PcieLinkEqSettings->PlatformSettings.Ph3PresetList,
        PcieLinkEqSettings->PlatformSettings.Ph3NumberOfPresetsOrCoefficients,
        IpPcieGen5
        );
      break;
    case IpPcieLinkEqCoefficientMode:
      SipSetCoefficientEqMode (pInst, IpPcieGen5);
      SipInitializeCoefficientsList (pInst,
      PcieLinkEqSettings->PlatformSettings.Ph3CoefficientsList,
      PcieLinkEqSettings->PlatformSettings.Ph3NumberOfPresetsOrCoefficients,
      IpPcieGen5
      );
      break;
    default:
     PRINT_ERROR ( "Incorrect Gen5 equalization mode\n");
  }

  Px32EqCfg1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PX32EQCFG1_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  Px32EqCfg1.Bits.px32glep23b = 0;
  Px32EqCfg1.Bits.px32glep3b = 0;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PX32EQCFG1_PCIE_MEM_RCRB_REG, Px32EqCfg1.Data, IpWrRegFlagSize32Bits);

  if (PcieLinkEqSettings->PlatformSettings.EqPh23Bypass) {
    /// Program 32GT/s Equalization Configuration 1 register 0x1e04
    Px32EqCfg1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PX32EQCFG1_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
    Px32EqCfg1.Bits.px32glep23b = 1;
    IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PX32EQCFG1_PCIE_MEM_RCRB_REG, Px32EqCfg1.Data, IpWrRegFlagSize32Bits);
  }
  if (PcieLinkEqSettings->PlatformSettings.EqPh3Bypass) {
    /// Program 32GT/s Equalization Configuration 1 register 0x1e04
    Px32EqCfg1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PX32EQCFG1_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
    Px32EqCfg1.Bits.px32glep3b = 1;
    IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PX32EQCFG1_PCIE_MEM_RCRB_REG, Px32EqCfg1.Data, IpWrRegFlagSize32Bits);
  }

  //
  // This programing tells hardware to perform preset/coefficient search only up to the given preset/coefficient. We want hardware to search
  // all presets/coefficients that were passed in the list to BIOS. This register is 0 based, meaning that value of 0 will cause hardware to
  // terminate search after preset/coefficient 0.
  // Program 32GT/s Equalization Configuration 2 register 0x1e24
  //
  Px32EqCfg2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PX32EQCFG2_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  Px32EqCfg2.Bits.hapcsb = PcieLinkEqSettings->PlatformSettings.Ph3NumberOfPresetsOrCoefficients - 1;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PX32EQCFG2_PCIE_MEM_RCRB_REG, Px32EqCfg2.Data, IpWrRegFlagSize32Bits);

  /// Program 32GT/s Equalization Configuration 1 register 0x1e04
  Px32EqCfg1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PX32EQCFG1_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  Px32EqCfg1.Bits.px32gtswlpce = PcieLinkEqSettings->PlatformSettings.TsLockTimer;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PX32EQCFG1_PCIE_MEM_RCRB_REG, Px32EqCfg1.Data, IpWrRegFlagSize32Bits);

  //
  // Program the preset/coefficient evaluation time depending on the number of presets/coefficients used in EQ.
  // HW EQ has to be finished within 24ms so the more coefficient/presets we have to evaluate the less time we can
  // spend on each.
  // NOTE: Encoding of the register is not 1:1 with time in ms. Please see register description for details.
  // Program 32GT/s Equalization Configuration 2 register 0x1e24
  //
  Px32EqCfg2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PX32EQCFG2_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  Px32EqCfg2.Bits.pcet = PcieLinkEqSettings->PlatformSettings.PCETTimer;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PX32EQCFG2_PCIE_MEM_RCRB_REG, Px32EqCfg2.Data, IpWrRegFlagSize32Bits);

  //
  // Set Receiver wait time for EQ evaluation to 3us.
  // @todo Need to define test menu for Ph23Bypass and Ph3Bypass. program LEP23B and LEP3B accordingly
  // @todo Need to define TS lock timer test menu and program TSWLPCE accordingly
  // Program 32GT/s Equalization Configuration 1 register 0x1e04
  //
  Px32EqCfg1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PX32EQCFG1_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  Px32EqCfg1.Bits.px32grwtneve = 1;
  Px32EqCfg1.Bits.px32ghapccpi = PcieLinkEqSettings->PlatformSettings.Ph3NumberOfPresetsOrCoefficients - 1;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PX32EQCFG1_PCIE_MEM_RCRB_REG, Px32EqCfg1.Data, IpWrRegFlagSize32Bits);

  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);
}

/**
  Sets up hardware for fixed EQ method.

  @param[in] pInst               Pointer to the IP_PCIE_INST
  @param[in] PcieLinkEqSettings  PCIe link EQ settings
  @param[in] RootPortPcieSpeed   PCIe speed of the Root Port
**/
static
void
SipSetupFixedEq (
  IP_PCIE_INST                *pInst,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieLinkEqSettings,
  IP_PCIE_SPEED               RootPortPcieSpeed
  )
{
  UINT32                            LinkWidth;
  EQCFG1_PCIE_CFG_STRUCT            EqCfg1;
  EQCFG4_PCIE_CFG_STRUCT            EqCfg4;
  PX32EQCFG1_PCIE_MEM_RCRB_STRUCT   Px32EqCfg1;

  LinkWidth = SipGetMaxLinkWidth (pInst);
  if (PcieLinkEqSettings->PlatformSettings.Ph3NumberOfPresetsOrCoefficients < LinkWidth) {
    PRINT_ERROR ( "Fixed EQ requested with insufficient number of presets or coefficients\n");
    IpWrError (pInst->ErrorAssert, IpWrErrorMajorError, __LINE__);
    return;
  }

  //
  // Set preset/coefficient evaluation bypass and override enable. This will cause the root port
  // to unconditionally accept remote transmiter values.
  //
  switch (RootPortPcieSpeed) {
    case IpPcieGen5:
      Px32EqCfg1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PX32EQCFG1_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
      Px32EqCfg1.Bits.px32grtpcoe   = 1;
      Px32EqCfg1.Bits.px32grtlepceb = 1;
      IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PX32EQCFG1_PCIE_MEM_RCRB_REG, Px32EqCfg1.Data, IpWrRegFlagSize32Bits);
      break;
    case IpPcieGen4:
      // Program Equalization Configuration 4 register 0x048C
      EqCfg4.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, EQCFG4_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
      EqCfg4.Bits.px16grtpcoe   = 1;
      EqCfg4.Bits.px16grtlepceb = 1;
      IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, EQCFG4_PCIE_CFG_REG, EqCfg4.Data, IpWrRegFlagSize32Bits);
      break;
    case IpPcieGen3:
      // Program Equalization Configuration 1 register 0x0450
      EqCfg1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, EQCFG1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
      EqCfg1.Bits.rtpcoe   = 1;
      EqCfg1.Bits.rtlepceb = 1;
      IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, EQCFG1_PCIE_CFG_REG, EqCfg1.Data, IpWrRegFlagSize32Bits);
      break;
    default:
      PRINT_ERROR ( "Equalization not possible at this speed\n");
      break;
  }

  //
  // Fixed equalization algorithm performs remote transmitter override instead of searching for best values.
  // In remote transmitter override mode the meaning of preset list and coefficient list is changed and instead
  // of containing the list of values that should be searched it contains fixed values of the presets or coefficients
  // for each PCIe lane that will be used to override.
  //
  switch (PcieLinkEqSettings->PlatformSettings.PcieLinkEqMode) {
    case IpPcieLinkEqPresetMode:
      SipSetPresetEqMode (pInst, RootPortPcieSpeed);
      SipInitializePresetList (pInst, PcieLinkEqSettings->PlatformSettings.Ph3PresetList, LinkWidth, RootPortPcieSpeed);
      break;
    case IpPcieLinkEqCoefficientMode:
      SipSetCoefficientEqMode (pInst, RootPortPcieSpeed);
      SipInitializeCoefficientsList (pInst, PcieLinkEqSettings->PlatformSettings.Ph3CoefficientsList, LinkWidth, RootPortPcieSpeed);
      break;
    default:
     PRINT_ERROR ( "Incorrect equalization mode\n");
  }
}

/**
  Clears Gen3 Gen4 EQ related registers to protect against EQ reruns and bad defaults.

  @param[in]   @param[in] pInst          *pInst
**/
void
SipClearEqRegisters (
  IP_PCIE_INST        *pInst
  )
{

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, EQCFG1_PCIE_CFG_REG, 0, IpWrRegFlagSize32Bits);
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, EQCFG2_PCIE_CFG_REG, 0, IpWrRegFlagSize32Bits);
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, EQCFG4_PCIE_CFG_REG, 0, IpWrRegFlagSize32Bits);
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, EQCFG5_PCIE_CFG_REG, 0, IpWrRegFlagSize32Bits);
  IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, HAEQ_PCIE_CFG_REG, 0, IpWrRegFlagSize32Bits);

  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);
}

typedef struct {
  UINT32  CursorRegisterOffset;
  UINT32  CursorFieldOffset;
  UINT32  PreCursorRegisterOffset;
  UINT32  PreCursorFieldOffset;
  UINT32  PostCursorRegisterOffset;
  UINT32  PostCursorFieldOffset;
} PRESET_TO_COEFFICIENT_REGISTER_LAYOUT;

/**
  Get the preset registers and fields offsets.

  @param[in]  PresetIndex      Index of the preset
  @param[out] RegisterLayout   Returns a structure that describe preset layout in memory.
**/
void
IpGetSipPresetToCoefficientRegisterLayout (
  UINT32                                  PresetIndex,
  PRESET_TO_COEFFICIENT_REGISTER_LAYOUT   *RegisterLayout
  )
{
  //
  // Each dword register contains 5 fields, each preset uses 3 fields
  // where:
  // 1st field is cursor value
  // 2nd field is pre cursor value
  // 3rd field is post cursor value
  // Each field is 6-bit wide
  //
  RegisterLayout->CursorRegisterOffset = ((PresetIndex * 3) / 5) * 4;
  RegisterLayout->PreCursorRegisterOffset = (((PresetIndex * 3) + 1) / 5) * 4;
  RegisterLayout->PostCursorRegisterOffset = (((PresetIndex * 3) + 2) / 5) * 4;
  RegisterLayout->CursorFieldOffset = ((PresetIndex * 3) % 5) * 6;
  RegisterLayout->PreCursorFieldOffset = (((PresetIndex * 3) + 1) % 5) * 6;
  RegisterLayout->PostCursorFieldOffset = (((PresetIndex * 3) + 2) % 5) * 6;
}

/**
  Set preset coefficients.

  @param[in] pInst                *pInst
  @param[in] LaneIndex            PCIe lane for which to set coefficients
  @param[in] PresetIndex          Preset index for the given lane
  @param[in] LowFrequencyValue    Low frequency value for the given lane
  @param[in] FullSwingValue       Full swing value for the given lane
  @param[in] PresetToCoefficient  Structure with information about coefficients for the preset
**/
static
void
SetSipGen3PresetToCoefficientMapping (
  IP_PCIE_INST                             *pInst,
  UINT32                                   LaneIndex,
  UINT32                                   PresetIndex,
  UINT32                                   LowFrequencyValue,
  UINT32                                   FullSwingValue,
  IP_PCIE_PRESET_TO_COEFFICIENT_MAPPING    PresetToCoefficient
  )
{
  UINT32                                   LaneBase;
  PRESET_TO_COEFFICIENT_REGISTER_LAYOUT    RegisterLayout;
  UINT32                                   Data32;
  UINT32                                   Data32And;
  UINT32                                   Data32Or;

  /// Program Lane 0 P0 And P1 Preset-Coefficient Mapping register 0x0500
  LaneBase = L0P0P1PCM_PCIE_CFG_REG + (LaneIndex * S_SINGLE_LANE_PRESETS);
  IpGetSipPresetToCoefficientRegisterLayout (PresetIndex, &RegisterLayout);
  Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, LaneBase + RegisterLayout.CursorRegisterOffset, IpWrRegFlagSize32Bits);
  Data32And = ~(0x3F << RegisterLayout.CursorFieldOffset);
  Data32Or = (PresetToCoefficient.CursorCoefficient & 0x3F) << RegisterLayout.CursorFieldOffset;

  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, LaneBase + RegisterLayout.CursorRegisterOffset, Data32, IpWrRegFlagSize32Bits);
  Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, LaneBase + RegisterLayout.PreCursorRegisterOffset, IpWrRegFlagSize32Bits);
  Data32And = ~(0x3F << RegisterLayout.PreCursorFieldOffset);
  Data32Or = (PresetToCoefficient.PreCursorCoefficient & 0x3F) << RegisterLayout.PreCursorFieldOffset;

  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, LaneBase + RegisterLayout.PreCursorRegisterOffset, Data32, IpWrRegFlagSize32Bits);

  Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, LaneBase + RegisterLayout.PostCursorRegisterOffset, IpWrRegFlagSize32Bits);
  Data32And = ~(0x3F << RegisterLayout.PostCursorFieldOffset);
  Data32Or = (PresetToCoefficient.PostCursorCoefficient & 0x3F) << RegisterLayout.PostCursorFieldOffset;

  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, LaneBase + RegisterLayout.PostCursorRegisterOffset, Data32, IpWrRegFlagSize32Bits);

  Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, LaneBase + R_LFFS_OFFSET_FROM_LANE_BASE, IpWrRegFlagSize32Bits);
  Data32And = ~(UINT32)((0x3F << N_LFFS_FS) | 0x3F);
  Data32Or = ((FullSwingValue & 0x3F) << N_LFFS_FS) | (LowFrequencyValue & 0x3F);

  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, LaneBase + R_LFFS_OFFSET_FROM_LANE_BASE, Data32, IpWrRegFlagSize32Bits);
}

/**
  Set Gen4 preset coefficients.

  @param[in] pInst                *pInst
  @param[in] LaneIndex            PCIe lane for which to set coefficients
  @param[in] PresetIndex          Preset index for the given lane
  @param[in] LowFrequencyValue    Low frequency value for the given lane
  @param[in] FullSwingValue       Full swing value for the given lane
  @param[in] PresetToCoefficient  Structure with information about coefficients for the preset
**/
static
void
SetSipGen4PresetToCoefficientMapping (
  IP_PCIE_INST                             *pInst,
  UINT32                                   PresetIndex,
  UINT32                                   LowFrequencyValue,
  UINT32                                   FullSwingValue,
  IP_PCIE_PRESET_TO_COEFFICIENT_MAPPING    PresetToCoefficient
  )
{
  PRESET_TO_COEFFICIENT_REGISTER_LAYOUT    RegisterLayout;
  UINT32                                   Data32;
  UINT32                                   Data32And;
  UINT32                                   Data32Or;

  IpGetSipPresetToCoefficientRegisterLayout (PresetIndex, &RegisterLayout);

  /// Program 16.0 GT/s P0 And P1 Preset-Coefficient Mapping register 0x0520
  Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PX16GP0P1PCM_PCIE_CFG_REG + RegisterLayout.CursorRegisterOffset, IpWrRegFlagSize32Bits);
  Data32And = ~(0x3F << RegisterLayout.CursorFieldOffset);
  Data32Or = (PresetToCoefficient.CursorCoefficient & 0x3F) << RegisterLayout.CursorFieldOffset;

  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PX16GP0P1PCM_PCIE_CFG_REG + RegisterLayout.CursorRegisterOffset, Data32, IpWrRegFlagSize32Bits);

  Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PX16GP0P1PCM_PCIE_CFG_REG + RegisterLayout.PreCursorRegisterOffset, IpWrRegFlagSize32Bits);
  Data32And = ~(0x3F << RegisterLayout.PreCursorFieldOffset);
  Data32Or = (PresetToCoefficient.PreCursorCoefficient & 0x3F) << RegisterLayout.PreCursorFieldOffset;

  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PX16GP0P1PCM_PCIE_CFG_REG + RegisterLayout.PreCursorRegisterOffset, Data32, IpWrRegFlagSize32Bits);

  Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PX16GP0P1PCM_PCIE_CFG_REG + RegisterLayout.PostCursorRegisterOffset, IpWrRegFlagSize32Bits);
  Data32And = ~(0x3F << RegisterLayout.PostCursorFieldOffset);
  Data32Or = (PresetToCoefficient.PostCursorCoefficient & 0x3F) << RegisterLayout.PostCursorFieldOffset;

  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PX16GP0P1PCM_PCIE_CFG_REG + RegisterLayout.PostCursorRegisterOffset, Data32, IpWrRegFlagSize32Bits);

  Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PX16GP0P1PCM_PCIE_CFG_REG + R_LFFS_OFFSET_FROM_LANE_BASE, IpWrRegFlagSize32Bits);
  Data32And = ~(UINT32)((0x3F << N_LFFS_FS) | 0x3F);
  Data32Or = ((FullSwingValue & 0x3F) << N_LFFS_FS) | (LowFrequencyValue & 0x3F);

  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PX16GP0P1PCM_PCIE_CFG_REG + R_LFFS_OFFSET_FROM_LANE_BASE, Data32, IpWrRegFlagSize32Bits);
}

/**
  Set Gen5 preset coefficients.

  @param[in] pInst                *pInst
  @param[in] LaneIndex            PCIe lane for which to set coefficients
  @param[in] PresetIndex          Preset index for the given lane
  @param[in] LowFrequencyValue    Low frequency value for the given lane
  @param[in] FullSwingValue       Full swing value for the given lane
  @param[in] PresetToCoefficient  Structure with information about coefficients for the preset
**/
static
void
SetSipGen5PresetToCoefficientMapping (
  IP_PCIE_INST                             *pInst,
  UINT32                                   PresetIndex,
  UINT32                                   LowFrequencyValue,
  UINT32                                   FullSwingValue,
  IP_PCIE_PRESET_TO_COEFFICIENT_MAPPING    PresetToCoefficient
  )
{
  PRESET_TO_COEFFICIENT_REGISTER_LAYOUT    RegisterLayout;
  UINT32                                   Data32;
  UINT32                                   Data32And;
  UINT32                                   Data32Or;

  IpGetSipPresetToCoefficientRegisterLayout (PresetIndex, &RegisterLayout);

  /// Program 32.0 GT/s P0 And P1 Preset-Coefficient Mapping register 0x1e28
  Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PX32GP0P1PCM_PCIE_MEM_RCRB_REG + RegisterLayout.CursorRegisterOffset, IpWrRegFlagSize32Bits);
  Data32And = ~(0x3F << RegisterLayout.CursorFieldOffset);
  Data32Or = (PresetToCoefficient.CursorCoefficient & 0x3F) << RegisterLayout.CursorFieldOffset;

  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PX32GP0P1PCM_PCIE_MEM_RCRB_REG + RegisterLayout.CursorRegisterOffset, Data32, IpWrRegFlagSize32Bits);

  Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PX32GP0P1PCM_PCIE_MEM_RCRB_REG + RegisterLayout.PreCursorRegisterOffset, IpWrRegFlagSize32Bits);
  Data32And = ~(0x3F << RegisterLayout.PreCursorFieldOffset);
  Data32Or = (PresetToCoefficient.PreCursorCoefficient & 0x3F) << RegisterLayout.PreCursorFieldOffset;

  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PX32GP0P1PCM_PCIE_MEM_RCRB_REG + RegisterLayout.PreCursorRegisterOffset, Data32, IpWrRegFlagSize32Bits);

  Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PX32GP0P1PCM_PCIE_MEM_RCRB_REG + RegisterLayout.PostCursorRegisterOffset, IpWrRegFlagSize32Bits);
  Data32And = ~(0x3F << RegisterLayout.PostCursorFieldOffset);
  Data32Or = (PresetToCoefficient.PostCursorCoefficient & 0x3F) << RegisterLayout.PostCursorFieldOffset;

  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PX32GP0P1PCM_PCIE_MEM_RCRB_REG + RegisterLayout.PostCursorRegisterOffset, Data32, IpWrRegFlagSize32Bits);

  Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PX32GP0P1PCM_PCIE_MEM_RCRB_REG + R_LFFS_OFFSET_FROM_LANE_BASE, IpWrRegFlagSize32Bits);
  Data32And = ~(UINT32)((0x3F << N_LFFS_FS) | 0x3F);
  Data32Or = ((FullSwingValue & 0x3F) << N_LFFS_FS) | (LowFrequencyValue & 0x3F);

  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PX32GP0P1PCM_PCIE_MEM_RCRB_REG + R_LFFS_OFFSET_FROM_LANE_BASE, Data32, IpWrRegFlagSize32Bits);
}

/**
  Get coefficients values corresponding to given preset.
  This function will return valid values only after
  SipConfigurePresetToCoefficientMapping has been called.

  @param[in]  IP_PCIE_INST       *pInst
  @param[in]  LaneIndex          PCIe lane for which to get coefficients
  @param[in]  PresetIndex        Preset index for given lane
  @param[out] Coefficients       Value of the coefficients extracted from RP HW
**/
static
void
GetSipGen3PresetToCoefficientMapping (
  IP_PCIE_INST                    *pInst,
  UINT32                          LaneIndex,
  UINT32                          PresetIndex,
  IP_PCIE_LINK_EQ_COEFFICIENTS    *Coefficients
  )
{
  UINT32                                   LaneBase;
  PRESET_TO_COEFFICIENT_REGISTER_LAYOUT    RegisterLayout;

  /// Program Lane 0 P0 And P1 Preset-Coefficient Mapping register 0x0500
  LaneBase = L0P0P1PCM_PCIE_CFG_REG + (LaneIndex * S_SINGLE_LANE_PRESETS);
  IpGetSipPresetToCoefficientRegisterLayout (PresetIndex, &RegisterLayout);

  Coefficients->PreCursor = ((UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, LaneBase + RegisterLayout.PreCursorRegisterOffset, IpWrRegFlagSize32Bits) >> RegisterLayout.PreCursorFieldOffset) & 0x3F;
  Coefficients->PostCursor = ((UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, LaneBase + RegisterLayout.PostCursorRegisterOffset, IpWrRegFlagSize32Bits) >> RegisterLayout.PostCursorFieldOffset) & 0x3F;
}

/**
  Get coefficients values corresponding to given preset.
  This function will return valid values only after
  SipConfigurePresetToCoefficientMapping has been called.

  @param[in]  IP_PCIE_INST       *pInst
  @param[in]  LaneIndex          PCIe lane for which to get coefficients
  @param[in]  PresetIndex        Preset index for given lane
  @param[out] Coefficients       Value of the coefficients extracted from RP HW
**/
static
void
GetSipGen4PresetToCoefficientMapping (
  IP_PCIE_INST                    *pInst,
  UINT32                          LaneIndex,
  UINT32                          PresetIndex,
  IP_PCIE_LINK_EQ_COEFFICIENTS    *Coefficients
  )
{
  UINT32                                   LaneBase;
  PRESET_TO_COEFFICIENT_REGISTER_LAYOUT    RegisterLayout;

  /// Program 16.0 GT/s P0 And P1 Preset-Coefficient Mapping register 0x0520
  LaneBase = PX16GP0P1PCM_PCIE_CFG_REG + (LaneIndex * S_SINGLE_LANE_PRESETS);
  IpGetSipPresetToCoefficientRegisterLayout (PresetIndex, &RegisterLayout);

  Coefficients->PreCursor = ((UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, LaneBase + RegisterLayout.PreCursorRegisterOffset, IpWrRegFlagSize32Bits) >> RegisterLayout.PreCursorFieldOffset) & 0x3F;
  Coefficients->PostCursor = ((UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, LaneBase + RegisterLayout.PostCursorRegisterOffset, IpWrRegFlagSize32Bits) >> RegisterLayout.PostCursorFieldOffset) & 0x3F;
}

/**
  Get coefficients values corresponding to given preset.
  This function will return valid values only after
  SipConfigurePresetToCoefficientMapping has been called.

  @param[in]  IP_PCIE_INST       *pInst
  @param[in]  LaneIndex          PCIe lane for which to get coefficients
  @param[in]  PresetIndex        Preset index for given lane
  @param[out] Coefficients       Value of the coefficients extracted from RP HW
**/
static
void
GetSipGen5PresetToCoefficientMapping (
  IP_PCIE_INST                    *pInst,
  UINT32                          LaneIndex,
  UINT32                          PresetIndex,
  IP_PCIE_LINK_EQ_COEFFICIENTS    *Coefficients
  )
{
  UINT32                                   LaneBase;
  PRESET_TO_COEFFICIENT_REGISTER_LAYOUT    RegisterLayout;

  LaneBase = PX32GP0P1PCM_PCIE_MEM_RCRB_REG + (LaneIndex * S_SINGLE_LANE_PRESETS);
  IpGetSipPresetToCoefficientRegisterLayout (PresetIndex, &RegisterLayout);

  Coefficients->PreCursor = ((UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, LaneBase + RegisterLayout.PreCursorRegisterOffset, IpWrRegFlagSize32Bits) >> RegisterLayout.PreCursorFieldOffset) & 0x3F;
  Coefficients->PostCursor = ((UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, LaneBase + RegisterLayout.PostCursorRegisterOffset, IpWrRegFlagSize32Bits) >> RegisterLayout.PostCursorFieldOffset) & 0x3F;
}

/**
  Programs presets-to-coefficients mapping for hardware equalization.
  It should not be performed for ports without Gen3 capability

  @param[in] IP_PCIE_INST             *pInst
  @param[in] PcieGen3LinkEqSettings   Pointer to PCIe link EQ settings
  @param[in] PcieGen4LinkEqSettings   Pointer to PCIe link EQ settings
  @param[in] PcieGen5LinkEqSettings   Pointer to PCIe link EQ settings
  @param[in] TargetPcieSpeed          PCIe speed to train to
**/
void
SipConfigurePresetToCoefficientMapping (
  IP_PCIE_INST                *pInst,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen3LinkEqSettings,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen4LinkEqSettings,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen5LinkEqSettings,
  IP_PCIE_SPEED                  TargetPcieSpeed
  )
{
  UINT32    LaneIndex;
  UINT32    MaxLinkWidth;
  UINT32    PresetIndex;
  UINT32    TimeoutUs;
  BOOLEAN   Gen3QueryToPipeSupport;
  BOOLEAN   Gen4QueryToPipeSupport;
  BOOLEAN   Gen5QueryToPipeSupport;
  EQCFG1_PCIE_CFG_STRUCT   EqCfg1;

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  Gen3QueryToPipeSupport = PcieGen3LinkEqSettings->ProjectSettings.PipePresetToCoefficientQuerySupported;
  Gen4QueryToPipeSupport = PcieGen4LinkEqSettings->ProjectSettings.PipePresetToCoefficientQuerySupported;
  Gen5QueryToPipeSupport = PcieGen5LinkEqSettings->ProjectSettings.PipePresetToCoefficientQuerySupported;
  //
  // Pcie SIP > 14 has space for only one lane PCM
  // configuration. This configuration is propagated
  // to all lanes.
  //
  MaxLinkWidth = 1;

  if (Gen3QueryToPipeSupport || Gen4QueryToPipeSupport || Gen5QueryToPipeSupport) {
    //
    // Program Equalization Configuration 1 register 0x0450
    // If supported send a query to PIPE
    //
    EqCfg1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, EQCFG1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    EqCfg1.Bits.hpcmqe = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Pri, EQCFG1_PCIE_CFG_REG, EqCfg1.Data, IpWrRegFlagSize32Bits);

    TimeoutUs = 1000;
    while (TimeoutUs-- > 0) {
      // Program Equalization Configuration 1 register 0x0450
      EqCfg1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, EQCFG1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
      if (EqCfg1.Bits.hpcmqe == 0) {
        return;
      }
      IpWrDelayUs (pInst->TimeCntxt, 1);
    }
    IpWrError (pInst->ErrorAssert, IpWrErrorMajorError, __LINE__);
  } else {
    //
    // If query to PIPE is unsupported we need to program project's recommendation from IP
    //
   for (LaneIndex = 0; LaneIndex < MaxLinkWidth; LaneIndex++) {
      for (PresetIndex = 0; PresetIndex < ARRAY_SIZE (PcieGen4LinkEqSettings->ProjectSettings.PresetToCoefficient); PresetIndex++) {
        switch (TargetPcieSpeed) {
          case IpPcieGen5:
            if (!Gen5QueryToPipeSupport) {
              SetSipGen5PresetToCoefficientMapping (
                pInst,
                PresetIndex,
                PcieGen5LinkEqSettings->ProjectSettings.LowFrequencyValue,
                PcieGen5LinkEqSettings->ProjectSettings.FullSwingValue,
                PcieGen5LinkEqSettings->ProjectSettings.PresetToCoefficient[PresetIndex]
              );
            }
          case IpPcieGen4:
            if (!Gen4QueryToPipeSupport) {
              SetSipGen4PresetToCoefficientMapping (
                pInst,
                PresetIndex,
                PcieGen4LinkEqSettings->ProjectSettings.LowFrequencyValue,
                PcieGen4LinkEqSettings->ProjectSettings.FullSwingValue,
                PcieGen4LinkEqSettings->ProjectSettings.PresetToCoefficient[PresetIndex]
              );
            }
          case IpPcieGen3:
            if (!Gen3QueryToPipeSupport) {
              SetSipGen3PresetToCoefficientMapping (
                pInst,
                LaneIndex,
                PresetIndex,
                PcieGen3LinkEqSettings->ProjectSettings.LowFrequencyValue,
                PcieGen3LinkEqSettings->ProjectSettings.FullSwingValue,
                PcieGen3LinkEqSettings->ProjectSettings.PresetToCoefficient[PresetIndex]
              );
            }
            break;
          default:
            PRINT_ERROR ( "Preset to coefficient mapping not available at this speed\n");
            break;
        }
      }
    }
  }
  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);
}

/**
  Sets local transmitter override coefficients for a
  given lane for Gen3.

  @param[in] IP_PCIE_INST      *pInst
  @param[in] LaneIndex         Index of the PCI lane
  @param[in] LaneCoefficients  Coefficients to be programed
**/
static
void
PcieSipSetGen3LocalTransmitterOverrideCoefficients (
  IP_PCIE_INST                    *pInst,
  UINT32                          LaneIndex,
  IP_PCIE_LINK_EQ_COEFFICIENTS    *LaneCoefficients
  )
{
  UINT32    LaneRegisterIndex;
  UINT32    LaneIndexInsideRegister;
  UINT32    Data32;
  UINT32    Data32And;
  UINT32    Data32Or;

  //
  // Each of the local transmitter override registers
  // holds configuration fields for 2 PCIe lanes.
  // The layout of the register is as follows
  // lower lane pre-cursor value
  // lower lane post-cursor value
  // upper lane pre-cursor value
  // upper lane post-cursor value
  // lower lane local transmitter override enable
  // upper lane local transmitter override enable
  // Cursor values fields are 6 bits wide.
  //
  LaneRegisterIndex = LaneIndex / 2;
  LaneIndexInsideRegister = LaneIndex % 2;

  //
  // We program the LTCXX register in 2 writes since according to PCIe BWG
  // the pre and post cursor values must be valid at the time of setting the
  // override enable bits.
  // Program Local Transmitter Coefficient Override 1 register 0x0470
  //
  Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, LTCO1_PCIE_CFG_REG + (4 * LaneRegisterIndex), IpWrRegFlagSize32Bits);
  Data32And = (UINT32) ~((0x3F << (6 * LaneIndexInsideRegister)) | (0x3F << (12 * LaneIndexInsideRegister)));
  Data32Or = (LaneCoefficients->PreCursor << (6 * LaneIndexInsideRegister) | (LaneCoefficients->PostCursor << (12 * LaneIndexInsideRegister)));

  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, LTCO1_PCIE_CFG_REG + (4 * LaneRegisterIndex), Data32, IpWrRegFlagSize32Bits);

  Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, LTCO1_PCIE_CFG_REG + (4 * LaneRegisterIndex), IpWrRegFlagSize32Bits);
  Data32Or = B_LTCO1_L0TCOE << LaneIndexInsideRegister;
  Data32 |= Data32Or;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, LTCO1_PCIE_CFG_REG + (4 * LaneRegisterIndex), Data32, IpWrRegFlagSize32Bits);
}

/**
  Sets local transmitter override coefficients for a
  given lane for Gen4.

  @param[in] IP_PCIE_INST      *pInst
  @param[in] LaneIndex         Index of the PCI lane
  @param[in] LaneCoefficients  Coefficients to be programed
**/
static
void
PcieSipSetGen4LocalTransmitterOverrideCoefficients (
  IP_PCIE_INST                 *pInst,
  UINT32                       LaneIndex,
  IP_PCIE_LINK_EQ_COEFFICIENTS    *LaneCoefficients
  )
{
  UINT32    LaneRegisterIndex;
  UINT32    LaneIndexInsideRegister;
  UINT32    Data32;
  UINT32    Data32And;
  UINT32    Data32Or;

  //
  // Each of the local transmitter override registers
  // holds configuration fields for 2 PCIe lanes.
  // The layout of the register is as follows
  // lower lane pre-cursor value
  // lower lane post-cursor value
  // upper lane pre-cursor value
  // upper lane post-cursor value
  // lower lane local transmitter override enable
  // upper lane local transmitter override enable
  // Cursor values fields are 6 bits wide.
  //
  LaneRegisterIndex = LaneIndex / 2;
  LaneIndexInsideRegister = LaneIndex % 2;

  //
  // We program the LTCXX register in 2 writes since according to PCIe BWG
  // the pre and post cursor values must be valid at the time of setting the
  // override enable bits.
  //Program 16 GT/s Local Transmitter Coefficient Override 1 register 0x0600
  //
  Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PX16GLTCO1_PCIE_CFG_REG + (4 * LaneRegisterIndex), IpWrRegFlagSize32Bits);
  Data32And = (UINT32) ~((0x3F << (6 * LaneIndexInsideRegister)) | (0x3F << (12 * LaneIndexInsideRegister)));
  Data32Or = (LaneCoefficients->PreCursor << (6 * LaneIndexInsideRegister) | (LaneCoefficients->PostCursor << (12 * LaneIndexInsideRegister)));

  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PX16GLTCO1_PCIE_CFG_REG + (4 * LaneRegisterIndex), Data32, IpWrRegFlagSize32Bits);

  Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, PX16GLTCO1_PCIE_CFG_REG + (4 * LaneRegisterIndex), IpWrRegFlagSize32Bits);
  Data32Or = B_PX16GLTCO1_L0TCOE << LaneIndexInsideRegister;
  Data32 |= Data32Or;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, PX16GLTCO1_PCIE_CFG_REG + (4 * LaneRegisterIndex), Data32, IpWrRegFlagSize32Bits);
}

/**
  Sets local transmitter override coefficients for a
  given lane for Gen5.

  @param[in] IP_PCIE_INST      *pInst
  @param[in] LaneIndex         Index of the PCI lane
  @param[in] LaneCoefficients  Coefficients to be programed
**/
static
void
PcieSipSetGen5LocalTransmitterOverrideCoefficients (
  IP_PCIE_INST                 *pInst,
  UINT32                       LaneIndex,
  IP_PCIE_LINK_EQ_COEFFICIENTS    *LaneCoefficients
  )
{
  UINT32            LaneRegisterIndex;
  UINT32            LaneIndexInsideRegister;
  UINT32            Data32;
  UINT32            Data32And;
  UINT32            Data32Or;

  //
  // Each of the local transmitter override registers
  // holds configuration fields for 2 PCIe lanes.
  // The layout of the register is as follows
  // lower lane pre-cursor value
  // lower lane post-cursor value
  // upper lane pre-cursor value
  // upper lane post-cursor value
  // lower lane local transmitter override enable
  // upper lane local transmitter override enable
  // Cursor values fields are 6 bits wide.
  //
  LaneRegisterIndex = LaneIndex / 2;
  LaneIndexInsideRegister = LaneIndex % 2;

  //
  // We program the LTCXX register in 2 writes since according to PCIe BWG
  // the pre and post cursor values must be valid at the time of setting the
  // override enable bits.
  // Program 32 GT/s Local Transmitter Coefficient Override 1 register 0x1e48
  //
  Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PX32GLTCO1_PCIE_MEM_RCRB_REG + (4 * LaneRegisterIndex), IpWrRegFlagSize32Bits);
  Data32And = (UINT32) ~((0x3F << (6 * LaneIndexInsideRegister)) | (0x3F << (12 * LaneIndexInsideRegister)));
  Data32Or = (LaneCoefficients->PreCursor << (6 * LaneIndexInsideRegister) | (LaneCoefficients->PostCursor << (12 * LaneIndexInsideRegister)));

  Data32 &= Data32And;
  Data32 |= Data32Or;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PX32GLTCO1_PCIE_MEM_RCRB_REG + (4 * LaneRegisterIndex), Data32, IpWrRegFlagSize32Bits);

  Data32 = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PX32GLTCO1_PCIE_MEM_RCRB_REG + (4 * LaneRegisterIndex), IpWrRegFlagSize32Bits);
  Data32Or = B_PX32GLTCO1_L0TCOE << LaneIndexInsideRegister;
  Data32 |= Data32Or;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PX32GLTCO1_PCIE_MEM_RCRB_REG + (4 * LaneRegisterIndex), Data32, IpWrRegFlagSize32Bits);
}

/**
  If local transmitter override is enabled this function will
  program all of the required coefficients overrides.

  @param[in] IP_PCIE_INST           *pInst
  @param[in] PcieGen3LinkEqSettings  PCIe Gen3 link EQ settings
  @param[in] PcieGen4LinkEqSettings  PCIe Gen4 link EQ settings
  @param[in] PcieGen5LinkEqSettings  PCIe Gen5 link EQ settings
  @param[in] TargetPcieSpeed         PCIe speed to train to
**/
void
SipSetupEqPh2 (
  IP_PCIE_INST                *pInst,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen3LinkEqSettings,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen4LinkEqSettings,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen5LinkEqSettings,
  IP_PCIE_SPEED               TargetPcieSpeed
  )
{
  UINT32                          LaneIndex;
  UINT8                           LinkWidth;
  IP_PCIE_LINK_EQ_COEFFICIENTS    Gen5LaneCoefficients;
  IP_PCIE_LINK_EQ_COEFFICIENTS    Gen4LaneCoefficients;
  IP_PCIE_LINK_EQ_COEFFICIENTS    Gen3LaneCoefficients;
  LSTS_PCIE_CFG_STRUCT            Lsts;

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  if ((!PcieGen5LinkEqSettings->PlatformSettings.LocalTransmitterOverrideEnable) && (!PcieGen4LinkEqSettings->PlatformSettings.LocalTransmitterOverrideEnable) && (!PcieGen3LinkEqSettings->PlatformSettings.LocalTransmitterOverrideEnable)) {
    return;
  }

  // Link Status register 0x0052
  Lsts.Data = (UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, LSTS_PCIE_CFG_REG, IpWrRegFlagSize16Bits);
  LinkWidth = (UINT8) Lsts.Bits.nlw;
  for (LaneIndex = 0; LaneIndex < LinkWidth; LaneIndex++) {
    //
    // Even though hardware supports override only
    // with coefficients due to user convenience BIOS API
    // allows to choose override preset instead. We translate the
    // preset to the appropriate coefficient values as set in
    // preset to coefficient mapping register and use those coefficient
    // values during override. This is done so that user can choose from well
    // defined presets instead of the phy specific coefficients.
    //
    switch (TargetPcieSpeed) {
      case IpPcieGen5:
        GetSipGen5PresetToCoefficientMapping (
          pInst,
          LaneIndex,
          PcieGen5LinkEqSettings->PlatformSettings.Ph2LocalTransmitterOverridePreset,
          &Gen5LaneCoefficients
          );
        PcieSipSetGen5LocalTransmitterOverrideCoefficients (
          pInst,
          LaneIndex,
          &Gen5LaneCoefficients
          );
      case IpPcieGen4:
        GetSipGen4PresetToCoefficientMapping (
          pInst,
          LaneIndex,
          PcieGen4LinkEqSettings->PlatformSettings.Ph2LocalTransmitterOverridePreset,
          &Gen4LaneCoefficients
          );
        PcieSipSetGen4LocalTransmitterOverrideCoefficients (
          pInst,
          LaneIndex,
          &Gen4LaneCoefficients
          );
      case IpPcieGen3:
        GetSipGen3PresetToCoefficientMapping (
          pInst,
          LaneIndex,
          PcieGen3LinkEqSettings->PlatformSettings.Ph2LocalTransmitterOverridePreset,
          &Gen3LaneCoefficients
          );
        PcieSipSetGen3LocalTransmitterOverrideCoefficients (
          pInst,
          LaneIndex,
          &Gen3LaneCoefficients
          );
        break;
      default:
        PRINT_ERROR ( "Unable to proceed with equalization phase 2 at this speed\n");
        break;
    }
  }
  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);
}

/**
  Prints PCIe equalization settings

  @param[in] PcieLinkEqSettings  Pointer to PCIe link EQ settings
**/
static
void
PcieSipPrintEqConfig (
  IP_PCIE_INST                *pInst,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieLinkEqSettings
  )
{
  UINT8    Index;

  PRINT_LEVEL1 ("Printing PCIe equalization settings\n");
  PRINT_LEVEL1 ("PcieLinkEqMethod = %d\n", PcieLinkEqSettings->PlatformSettings.PcieLinkEqMethod);
  PRINT_LEVEL1 ("PcieLinkEqMode = %d\n", PcieLinkEqSettings->PlatformSettings.PcieLinkEqMode);
  PRINT_LEVEL1 ("Ph1DownstreamPortTransmitterPreset = %d\n", PcieLinkEqSettings->PlatformSettings.Ph1DownstreamPortTransmitterPreset);
  PRINT_LEVEL1 ("Ph1UpstreamPortTransmitterPreset = %d\n", PcieLinkEqSettings->PlatformSettings.Ph1UpstreamPortTransmitterPreset);
  PRINT_LEVEL1 ("Ph3NumberOfPresetsOrCoefficients = %d\n", PcieLinkEqSettings->PlatformSettings.Ph3NumberOfPresetsOrCoefficients);
  if (PcieLinkEqSettings->PlatformSettings.PcieLinkEqMode == IpPcieLinkEqPresetMode) {
    for (Index = 0; Index < PcieLinkEqSettings->PlatformSettings.Ph3NumberOfPresetsOrCoefficients && Index < IP_PCIE_LINK_EQ_PRESETS_MAX; Index++) {
      PRINT_LEVEL1 ("Phase 3 Preset %d = %d\n", Index, PcieLinkEqSettings->PlatformSettings.Ph3PresetList[Index]);
    }
  } else {
    for (Index = 0; Index < PcieLinkEqSettings->PlatformSettings.Ph3NumberOfPresetsOrCoefficients && Index < IP_PCIE_LINK_EQ_COEFFICIENTS_MAX; Index++) {
      PRINT_LEVEL1 ("Phase 3 Coefficient %d, pre-cursor = %d, post-cursor = %d\n",
                            Index,
                            PcieLinkEqSettings->PlatformSettings.Ph3CoefficientsList[Index].PreCursor,
                            PcieLinkEqSettings->PlatformSettings.Ph3CoefficientsList[Index].PostCursor);
    }
  }
  PRINT_LEVEL1 ("Phase 2 local transmitter override enable = %d\n", PcieLinkEqSettings->PlatformSettings.LocalTransmitterOverrideEnable);
  PRINT_LEVEL1 ("Phase 2 local transmitter override preset = %d\n", PcieLinkEqSettings->PlatformSettings.Ph2LocalTransmitterOverridePreset);
  PRINT_LEVEL1 ("PipePresetToCoefficientQuerySupported = %d\n", PcieLinkEqSettings->ProjectSettings.PipePresetToCoefficientQuerySupported);
  if (!PcieLinkEqSettings->ProjectSettings.PipePresetToCoefficientQuerySupported) {
    PRINT_LEVEL1 ("LowFrequencyValue = %d\n", PcieLinkEqSettings->ProjectSettings.LowFrequencyValue);
    PRINT_LEVEL1 ("FullSwingValue = %d\n", PcieLinkEqSettings->ProjectSettings.FullSwingValue);
    for (Index = 0; Index < IP_PCIE_LINK_EQ_PRESETS_MAX; Index++) {
      PRINT_LEVEL1 ("Preset %d, pre-cursror = %d, cursor = %d, post-cursor = %d\n",
                            Index,
                            PcieLinkEqSettings->ProjectSettings.PresetToCoefficient[Index].PreCursorCoefficient,
                            PcieLinkEqSettings->ProjectSettings.PresetToCoefficient[Index].CursorCoefficient,
                            PcieLinkEqSettings->ProjectSettings.PresetToCoefficient[Index].PostCursorCoefficient);
    }
  }
}

/**
  Program registers to Setup EQTS2 recovery

  @param[in] pInst                *pInst
**/
void
SipSetupEQTS2Recovery (
  IP_PCIE_INST  *pInst
  )
{
  EQCFG1_PCIE_CFG_STRUCT                EqCfg1;
  EQCFG4_PCIE_CFG_STRUCT                EqCfg4;
  PX32EQCFG1_PCIE_MEM_RCRB_STRUCT       Px32EqCfg1;
  //
  // Set EQTS2IRRC, PX16GEQTS2IRRC and PX32GEQTS2IRRC for Gen3, Gen4 and Gen5
  // so that EQTS2 will be sent during Redo EQ in Gen5 Capable Controller
  //

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  /// Program Equalization Configuration 1 register 0x0450
  EqCfg1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, EQCFG1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  EqCfg1.Bits.eqts2irrc = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, EQCFG1_PCIE_CFG_REG, EqCfg1.Data, IpWrRegFlagSize32Bits);

  /// Program Equalization Configuration 4 register 0x048C
  EqCfg4.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, EQCFG4_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  EqCfg4.Bits.px16geqts2irrc = 1;
  IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, EQCFG4_PCIE_CFG_REG, EqCfg4.Data, IpWrRegFlagSize32Bits);

  /// Program 32GT/s Equalization Configuration 1 register 0x1e04
  Px32EqCfg1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Mem_Rcrb, PX32EQCFG1_PCIE_MEM_RCRB_REG, IpWrRegFlagSize32Bits);
  Px32EqCfg1.Bits.px32geqts2irrc = 1;
  IpWrRegWrite (pInst->RegCntxt_Mem_Rcrb, PX32EQCFG1_PCIE_MEM_RCRB_REG, Px32EqCfg1.Data, IpWrRegFlagSize32Bits);

  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);
}
/**
  Performs PCIe link equalization according to platform settings.
  Please make sure that link is in gen1 or gen2 before calling this
  procedure.

  @param[in] pInst                   Pointer to the IP_PCIE_INST
  @param[in] PcieGen3LinkEqSettings  PCIe Gen3 link equalization settings to be used during EQ
  @param[in] PcieGen4LinkEqSettings  PCIe Gen4 link equalization settings to be used during EQ
  @param[in] PcieGen5LinkEqSettings  PCIe Gen5 link equalization settings to be used during EQ
  @param[in] RootPortPcieSpeed       PCIe speed of the Root Port
**/
void
SipLinkEqualize (
  IP_PCIE_INST             *pInst,
  IP_PCIE_LINK_EQ_SETTINGS *PcieGen3LinkEqSettings,
  IP_PCIE_LINK_EQ_SETTINGS *PcieGen4LinkEqSettings,
  IP_PCIE_LINK_EQ_SETTINGS *PcieGen5LinkEqSettings,
  IP_PCIE_SPEED            RootPortPcieSpeed
  )
{
  EQCFG1_PCIE_CFG_STRUCT   EqCfg1;
  EQCFG2_PCIE_CFG_STRUCT   EqCfg2;
  HAEQ_PCIE_CFG_STRUCT     Haeq;
  LCAP_PCIE_CFG_STRUCT     Lcap;

  PRINT_LEVEL1 ("%s Start\n", __FUNCTION__);

  SipSetTransmitterPresets (pInst, PcieGen3LinkEqSettings, PcieGen4LinkEqSettings, PcieGen5LinkEqSettings, RootPortPcieSpeed);
  SipSetupEqPh2 (pInst, PcieGen3LinkEqSettings, PcieGen4LinkEqSettings, PcieGen5LinkEqSettings, RootPortPcieSpeed);
  SipSetupEQTS2Recovery (pInst);
  switch (RootPortPcieSpeed) {
    case IpPcieGen5:
      switch (PcieGen5LinkEqSettings->PlatformSettings.PcieLinkEqMethod) {
        case IpPcieLinkHardwareEq:
          SipSetupGen5HardwareEq (pInst, PcieGen5LinkEqSettings);
          break;
        case IpPcieLinkFixedEq:
          SipSetupFixedEq (pInst, PcieGen5LinkEqSettings, RootPortPcieSpeed);
          break;
        default:
          PRINT_ERROR ( "Invalid PCIe link EQ method\n");
          IpWrError (pInst->ErrorAssert, IpWrErrorMajorError, __LINE__);
          break;
      }
    case IpPcieGen4:
      switch (PcieGen4LinkEqSettings->PlatformSettings.PcieLinkEqMethod) {
        case IpPcieLinkHardwareEq:
          SipSetupGen4HardwareEq (pInst, PcieGen4LinkEqSettings);
          break;
        case IpPcieLinkFixedEq:
          SipSetupFixedEq (pInst, PcieGen4LinkEqSettings, RootPortPcieSpeed);
          break;
        default:
          PRINT_ERROR ( "Invalid PCIe link EQ method\n");
          IpWrError (pInst->ErrorAssert, IpWrErrorMajorError, __LINE__);
          break;
      }
    case IpPcieGen3:
      switch (PcieGen3LinkEqSettings->PlatformSettings.PcieLinkEqMethod) {
        case IpPcieLinkHardwareEq:
          SipSetupGen3HardwareEq (pInst, PcieGen3LinkEqSettings);
          break;
        case IpPcieLinkFixedEq:
          SipSetupFixedEq (pInst, PcieGen3LinkEqSettings, RootPortPcieSpeed);
          break;
        default:
          PRINT_ERROR ( "Invalid PCIe link EQ method\n");
          IpWrError (pInst->ErrorAssert, IpWrErrorMajorError, __LINE__);
          break;
      }
      break;
    default:
      PRINT_ERROR ( "Equalization not possible at this speed\n");
      break;
  }
  if (pInst->PcieRpCommonConfig.EqPhBypass) {
    /// Program Equalization Configuration 1 register 0x0450
    EqCfg1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Sb, EQCFG1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
    EqCfg1.Bits.leb = 1;
    IpWrRegWrite (pInst->RegCntxt_Cfg_Sb, EQCFG1_PCIE_CFG_REG, EqCfg1.Data, IpWrRegFlagSize32Bits);
  }
  /// Program Equalization Configuration 1 register 0x0450
  EqCfg1.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, EQCFG1_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  PRINT_LEVEL1 ("EqCfg1: 0x%08x\n", EqCfg1.Data);
  /// Program Equalization Configuration 2 register 0x047C
  EqCfg2.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, EQCFG2_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  PRINT_LEVEL1 ("EqCfg2: 0x%08x\n", EqCfg2.Data);
  /// Program Hardware Autonomous Equalization Control register 0x0468
  Haeq.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, HAEQ_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  PRINT_LEVEL1 ("HAEQ:   0x%08x\n", Haeq.Data);

  Lcap.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri,  LCAP_PCIE_CFG_REG, IpWrRegFlagSize32Bits);

  if ((pInst->PcieRpCommonConfig.ComplianceTestMode == TRUE) && (Lcap.Bits.mls >= IpPcieGen3)) {
    SipStartEqualization (pInst, (IP_PCIE_SPEED) Lcap.Bits.mls);
  }

  PRINT_LEVEL1 ("%s End\n", __FUNCTION__);
  return;
}

/**
  Performs the initialization for hardware equalization to Gen3.
  Please make sure this function is called before programming any
  power management register in PCIe Root Port

  @param[in] pInst                   *pInst
  @param[in] PcieGen3LinkEqSettings  PCIe Gen3 link equalization settings to be used during EQ
  @param[in] PcieGen4LinkEqSettings  PCIe Gen4 link equalization settings to be used during EQ
  @param[in] PcieGen5LinkEqSettings  PCIe Gen5 link equalization settings to be used during EQ
**/
void
SipLinkEqualizeInit (
  IP_PCIE_INST                *pInst,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen3LinkEqSettings,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen4LinkEqSettings,
  IP_PCIE_LINK_EQ_SETTINGS    *PcieGen5LinkEqSettings
  )
{
  IP_PCIE_SPEED               PcieSpeed;
  LCAP_PCIE_CFG_STRUCT        Lcap;

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  /// Program Link Capabilities register 0x004C
  Lcap.Data = (UINT32) IpWrRegRead (pInst->RegCntxt_Cfg_Pri,  LCAP_PCIE_CFG_REG, IpWrRegFlagSize32Bits);
  PcieSpeed = (IP_PCIE_SPEED) Lcap.Bits.mls;

  if (!pInst->PrivateConfig.RootPortDisable) {
    PRINT_LEVEL1 ("For Gen3\n");
    PcieSipPrintEqConfig (pInst, PcieGen3LinkEqSettings);
    if (PcieSpeed >= IpPcieGen4) {
      PRINT_LEVEL1 ("For Gen4\n");
      PcieSipPrintEqConfig (pInst, PcieGen4LinkEqSettings);
    }
    if (PcieSpeed >= IpPcieGen5) {
      PRINT_LEVEL1 ("For Gen5\n");
      PcieSipPrintEqConfig (pInst, PcieGen5LinkEqSettings);
    }
  }

  SipClearEqRegisters (pInst);
  SipConfigurePresetToCoefficientMapping (pInst, PcieGen3LinkEqSettings, PcieGen4LinkEqSettings, PcieGen5LinkEqSettings, PcieSpeed);
  PRINT_LEVEL1 ("%s End \n", __FUNCTION__);
}