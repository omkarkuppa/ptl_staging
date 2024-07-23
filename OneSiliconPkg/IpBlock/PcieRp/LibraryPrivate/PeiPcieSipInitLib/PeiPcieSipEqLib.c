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

#include <Register/PcieSipRegs.h>
#include <Library/PeiPcieSipInitLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/PerformanceLib.h>


/**
  Reads out internal signal state from the PCIe RP hardware.
  This is accomplished with monitor mux register in RP.

  @param[in] RpDev        Pointer to the root port device
  @param[in] SignalIndex  Index of the signal to read

  @return Value of the signal state.
**/
STATIC
UINT32
PcieSipGetInternalSignalState (
  IN PCIE_ROOT_PORT_DEV    *RpDev,
  IN UINT32                MonitorCategory,
  IN UINT32                MonitorSegment,
  IN UINT32                MonitorSignal
  )
{
  if (MonitorCategory > 0xF ||
      MonitorSegment >= 0xF ||
      MonitorSignal > 0xFF) {
    DEBUG ((DEBUG_ERROR, "Failed to read monitor signal, index out of range C:%X S:%X S:%X\n",
                           MonitorCategory, MonitorSegment, MonitorSignal));
    return 0xFFFFFFFF;
  }

  RpDev->PciCfgAccess->Write32 (
                         RpDev->PciCfgAccess,
                         R_PCIE_CFG_CDM,
                         MonitorCategory << N_PCIE_CFG_CDM_MCS ||
                         MonitorSegment << N_PCIE_CFG_CDM_MSS);
  RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_MM, MonitorSignal);
  return RpDev->PciCfgAccess->Read32 (RpDev->PciCfgAccess, R_PCIE_CFG_MM) >> N_PCIE_CFG_MM_MSST;
}

/**
  Dumps best EQ coefficients values that were selected during EQ.

  @param[in]  RpDev        Pointer to the root port device
  @param[in]  RpLaneIndex  Lane index of the RP
**/
STATIC
VOID
PcieSipDumpBestEqCoefficients (
  IN PCIE_ROOT_PORT_DEV    *RpDev,
  IN UINT32                RpLaneIndex
  )
{
  UINT32      SignalState;
  PCIE_SPEED  MaxRpSpeed;

  MaxRpSpeed = PcieSipGetMaxLinkSpeed (RpDev);


  DEBUG ((DEBUG_INFO, "Lane %d:\n", RpLaneIndex));
  if (MaxRpSpeed >= PcieGen3) {
    SignalState = PcieSipGetInternalSignalState (
                    RpDev,
                    R_PCIE_SIP_MONITOR_CAT2,
                    RpLaneIndex,
                    R_PCIE_SIP_MONITOR_CAT2_BEST4COEFF_GEN3);
    DEBUG ((DEBUG_INFO, "GEN3: %X\n", SignalState));
  }

  if (MaxRpSpeed >= PcieGen4) {
    SignalState = PcieSipGetInternalSignalState (
                    RpDev,
                    R_PCIE_SIP_MONITOR_CAT2,
                    RpLaneIndex,
                    R_PCIE_SIP_MONITOR_CAT2_BEST4COEFF_GEN4);
    DEBUG ((DEBUG_INFO, "GEN4: %X\n", SignalState));
  }

  if (MaxRpSpeed >= PcieGen5) {
    SignalState = PcieSipGetInternalSignalState (
                    RpDev,
                    R_PCIE_SIP_MONITOR_CAT2,
                    RpLaneIndex,
                    R_PCIE_SIP_MONITOR_CAT2_BEST4COEFF_GEN5);
    DEBUG ((DEBUG_INFO, "GEN5: %X\n", SignalState));
  }
}

/**
  Dump Figure of merit scoreboard(FOMS) for each of the root port lanes.

  @param[in] RpDev        Pointer to the root port device
  @param[in] RpLaneIndex  Index of the root port lane
**/
STATIC
VOID
PcieSipDumpFoms (
  IN PCIE_ROOT_PORT_DEV    *RpDev,
  IN UINT32                RpLaneIndex
  )
{
  UINT32    ListEntryIndex;
  UINT32    Foms;

  DEBUG ((DEBUG_INFO, "%d.%d FOM Scoreboard:\n", PcieSipGetPortNum (RpDev), RpLaneIndex));
  for (ListEntryIndex = 0; ListEntryIndex < 10; ListEntryIndex += 3) {
    //
    // FOMV field holds FOM values for 3 list entires. To select correct triplet
    // we use Index field.
    //
    RpDev->PciCfgAccess->AndThenOr32 (
      RpDev->PciCfgAccess,
      R_PCIE_CFG_FOMS,
      (UINT32) ~(B_PCIE_CFG_FOMS_IDX | B_PCIE_CFG_FOMS_LN),
      ((ListEntryIndex/3)  << N_PCIE_CFG_FOMS_IDX) | (RpLaneIndex << N_PCIE_CFG_FOMS_LN)
      );
    Foms = RpDev->PciCfgAccess->Read32 (RpDev->PciCfgAccess, R_PCIE_CFG_FOMS);
    DEBUG ((DEBUG_INFO, "%d:%03d  ", ListEntryIndex,     (Foms & B_PCIE_CFG_FOMS_FOMSV0) >> N_PCIE_CFG_FOMS_FOMSV0));
    DEBUG ((DEBUG_INFO, "%d:%03d  ", ListEntryIndex + 1, (Foms & B_PCIE_CFG_FOMS_FOMSV1) >> N_PCIE_CFG_FOMS_FOMSV1));
    DEBUG ((DEBUG_INFO, "%d:%03d\n", ListEntryIndex + 2, (Foms & B_PCIE_CFG_FOMS_FOMSV2) >> N_PCIE_CFG_FOMS_FOMSV2));
  }
}

/**
  Reset and enable Recovery Entry and Idle Framing Error Count

  @param[in] RpDev    Pointer to the root port device
**/
VOID
PcieSipResetErrorCounts (
  PCIE_ROOT_PORT_DEV    *RpDev
  )
{
  RpDev->PciCfgAccess->And32 (RpDev->PciCfgAccess, R_PCIE_CFG_EQCFG1, (UINT32) ~B_PCIE_CFG_EQCFG1_REIFECE);
  RpDev->PciCfgAccess->Or32 (RpDev->PciCfgAccess, R_PCIE_CFG_EQCFG1, B_PCIE_CFG_EQCFG1_REIFECE);
}

/**
  Dump PCIe HW EQ results. For internal use only.

  @param[in] RpDev     Pointer to the root port device
**/
VOID
PcieSipDumpHwEqResults (
  IN PCIE_ROOT_PORT_DEV    *RpDev
  )
{
  UINT32    RpLaneIndex;
  UINT32    MaxLinkWidth;
  UINT32    TimeoutUs;
  UINT8     PcieSpeed;

  MaxLinkWidth = PcieSipGetMaxLinkWidth (RpDev);

 if ((RpDev->PciCfgAccess->Read16 (RpDev->PciCfgAccess,R_PCIE_CFG_LSTS) & B_PCIE_LSTS_LA) == 0) {
    DEBUG ((DEBUG_ERROR, "No link after EQ!\n"));
    return;
  }

  //
  // Wait for link training to complete
  //
  for (TimeoutUs = 0; TimeoutUs < 5000000; TimeoutUs += 10) {
    if ((RpDev->PciCfgAccess->Read16 (RpDev->PciCfgAccess, R_PCIE_CFG_LSTS) & B_PCIE_LSTS_LT) == 0) {
      break;
    }
    MicroSecondDelay (10);
  }
  DEBUG ((DEBUG_INFO, "HW EQ finished, results:\n"));
  DEBUG ((DEBUG_INFO, "EQCFG1: 0x%08x\n", RpDev->PciCfgAccess->Read32 (RpDev->PciCfgAccess, R_PCIE_CFG_EQCFG1)));
  DEBUG ((DEBUG_INFO, "LSTS2:  0x%04x\n", RpDev->PciCfgAccess->Read16 (RpDev->PciCfgAccess, R_PCIE_CFG_LSTS2)));
  DEBUG ((DEBUG_INFO, "Training time = %dus\n", TimeoutUs));

  PcieSpeed = RpDev->PcieRootPortConfig->PcieRpCommonConfig.PcieSpeed;
  if (PcieSpeed >= PcieGen3) {
    for (RpLaneIndex = 0; RpLaneIndex < MaxLinkWidth; RpLaneIndex++) {
      PcieSipDumpFoms (RpDev, RpLaneIndex);
      PcieSipDumpBestEqCoefficients (RpDev, RpLaneIndex);
    }
  }
}

/**
  Set the preset/coefficient list entry to the desired value.

  @param[in] RpDev             Pointer to the root port device
  @param[in] ListEntryIndex    Index of the entry
  @param[in] ListEntryValue    Value of the entry
  @param[in] TargetPcieSpeed   PCIe speed to train to
**/
STATIC
VOID
PcieSipSetPresetCoefficientListEntry (
  IN PCIE_ROOT_PORT_DEV    *RpDev,
  IN UINT32                ListEntryIndex,
  IN UINT32                ListEntryValue,
  IN PCIE_SPEED            TargetPcieSpeed
  )
{
  UINT32    ListEntryRegisterIndex;
  UINT32    ListEntryFieldIndex;

  ASSERT (ListEntryIndex < 20);
  ASSERT ((ListEntryValue & ~0x3F) == 0);

  //
  // Preset/Coefficient list is kept in hardware in a set
  // of continous registers each of which contains 5 list entries.
  //
  ListEntryRegisterIndex = ListEntryIndex / 5;
  ListEntryFieldIndex = ListEntryIndex % 5;

  switch (TargetPcieSpeed) {
    case PcieGen5:
      RpDev->PciSbiMsgMemAccess->AndThenOr32 (
        RpDev->PciSbiMsgMemAccess,
        R_PCIE_MEM_PX32GRTPCL1 + (4 * ListEntryRegisterIndex),
        (UINT32) ~(0x3F << (6 * ListEntryFieldIndex)),
        (ListEntryValue << (6 * ListEntryFieldIndex))
        );
      break;
    case PcieGen4:
      RpDev->PciSbiMsgCfgAccess->AndThenOr32 (
        RpDev->PciSbiMsgCfgAccess,
        R_PCIE_CFG_PX16GRTPCL1 + (4 * ListEntryRegisterIndex),
        (UINT32) ~(0x3F << (6 * ListEntryFieldIndex)),
        (ListEntryValue << (6 * ListEntryFieldIndex))
        );
      break;
    case PcieGen3:
      RpDev->PciSbiMsgCfgAccess->AndThenOr32 (
        RpDev->PciSbiMsgCfgAccess,
        R_PCIE_CFG_RTPCL1 + (4 * ListEntryRegisterIndex),
        (UINT32) ~(0x3F << (6 * ListEntryFieldIndex)),
        (ListEntryValue << (6 * ListEntryFieldIndex))
        );
      break;
    default:
      break;
  }
}

/**
  Sets the Gen3/Gen4 Preset EQ mode that will be used by root port.

  @param[in] RpDev             Pointer to the root port device
  @param[in] TargetPcieSpeed   PCIe speed to train to
**/
STATIC
VOID
PcieSipSetPresetEqMode (
  IN PCIE_ROOT_PORT_DEV    *RpDev,
  IN PCIE_SPEED            TargetPcieSpeed
  )
{
  switch (TargetPcieSpeed) {
    case PcieGen5:
      RpDev->PciSbiMsgMemAccess->And32 (RpDev->PciSbiMsgMemAccess, R_PCIE_MEM_PX32GRTPCL1, (UINT32) ~B_PCIE_MEM_PX32GRTPCL1_PCM);
      break;
    case PcieGen4:
      RpDev->PciSbiMsgCfgAccess->And32 (RpDev->PciSbiMsgCfgAccess, R_PCIE_CFG_PX16GRTPCL1, (UINT32) ~B_PCIE_CFG_PX16GRTPCL1_PCM);
      break;
    case PcieGen3:
      RpDev->PciSbiMsgCfgAccess->And32 (RpDev->PciSbiMsgCfgAccess, R_PCIE_CFG_RTPCL1, (UINT32) ~B_PCIE_CFG_RTPCL1_PCM);
      break;
    default:
      DEBUG ((DEBUG_ERROR, "Incorrect speed for Preset PCIe link EQ mode\n"));
      break;
  }
}

/**
  Sets the Gen3/Gen4 Coefficient EQ mode that will be used by root port.

  @param[in] RpDev             Pointer to the root port device
  @param[in] TargetPcieSpeed   PCIe speed to train to
**/
STATIC
VOID
PcieSipSetCoefficientEqMode (
  IN PCIE_ROOT_PORT_DEV    *RpDev,
  IN PCIE_SPEED            TargetPcieSpeed
  )
{
  switch (TargetPcieSpeed) {
    case PcieGen5:
      RpDev->PciSbiMsgMemAccess->Or32 (RpDev->PciSbiMsgMemAccess, R_PCIE_MEM_PX32GRTPCL1, B_PCIE_MEM_PX32GRTPCL1_PCM);
      break;
    case PcieGen4:
      RpDev->PciSbiMsgCfgAccess->Or32 (RpDev->PciSbiMsgCfgAccess, R_PCIE_CFG_PX16GRTPCL1, B_PCIE_CFG_PX16GRTPCL1_PCM);
      break;
    case PcieGen3:
      RpDev->PciSbiMsgCfgAccess->Or32 (RpDev->PciSbiMsgCfgAccess, R_PCIE_CFG_RTPCL1, B_PCIE_CFG_RTPCL1_PCM);
      break;
    default:
      DEBUG ((DEBUG_ERROR, "Incorrect speed for Coefficient PCIe link EQ mode\n"));
      break;
  }
}

/**
  Sets transmitter lane presets for both upstream and downstream port for Gen3.

  @param[in] RpDev                 Pointer to the root port device
  @param[in] LaneIndex             Index of the transmitter lane
  @param[in] DownstreamPortPreset  Preset value for the downstream port
  @param[in] UpstreamPortPreset    Preset value for the upstream port
**/
VOID
PcieSipSetGen3TransmitterLanePreset (
  IN PCIE_ROOT_PORT_DEV    *RpDev,
  IN UINT32                LaneIndex,
  IN UINT32                DownstreamPortPreset,
  IN UINT32                UpstreamPortPreset
  )
{
  UINT32    PresetRegisterIndex;
  UINT32    DownstreamPortPresetFieldIndex;
  UINT32    UpstreamPortPresetFieldIndex;

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

  RpDev->PciSbiMsgCfgAccess->AndThenOr32 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_L01EC + (4 * PresetRegisterIndex),
    ~(UINT32)((B_PCIE_CFG_L01EC_TP << (DownstreamPortPresetFieldIndex * S_PCIE_CFG_L01EC_TP)) |
              (B_PCIE_CFG_L01EC_TP << (UpstreamPortPresetFieldIndex * S_PCIE_CFG_L01EC_TP))),
    (DownstreamPortPreset << (DownstreamPortPresetFieldIndex * S_PCIE_CFG_L01EC_TP)) |
    (UpstreamPortPreset << (UpstreamPortPresetFieldIndex * S_PCIE_CFG_L01EC_TP))
    );
}

/**
  Sets transmitter lane presets for both upstream and downstream port for Gen4.

  @param[in] RpBase                PCI base of the RP
  @param[in] LaneIndex             Index of the transmitter lane
  @param[in] DownstreamPortPreset  Preset value for the downstream port
  @param[in] UpstreamPortPreset    Preset value for the upstream port
**/
VOID
PcieSipSetGen4TransmitterLanePreset (
  IN PCIE_ROOT_PORT_DEV    *RpDev,
  IN UINT32                LaneIndex,
  IN UINT32                DownstreamPortPreset,
  IN UINT32                UpstreamPortPreset
  )
{
  UINT32    PresetRegisterIndex;
  UINT32    DownstreamPortPresetFieldIndex;
  UINT32    UpstreamPortPresetFieldIndex;

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

  RpDev->PciSbiMsgCfgAccess->AndThenOr16 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_PL16L01EC + (2 * PresetRegisterIndex),
    ~(UINT16)((B_PCIE_CFG_L01EC_TP << (DownstreamPortPresetFieldIndex * S_PCIE_CFG_PL16L01EC_TP)) |
              (B_PCIE_CFG_L01EC_TP << (UpstreamPortPresetFieldIndex * S_PCIE_CFG_PL16L01EC_TP))),
    (UINT16)((DownstreamPortPreset << (DownstreamPortPresetFieldIndex * S_PCIE_CFG_PL16L01EC_TP)) |
    (UpstreamPortPreset << (UpstreamPortPresetFieldIndex * S_PCIE_CFG_PL16L01EC_TP)))
    );
}

/**
  Starts the EQ process.

  @param[in] RpDev               Pointer to the root port device
  @param[in] TargetPcieSpeed     PCIe speed to be trained to
**/
STATIC
VOID
PcieSipStartEqualization (
  IN PCIE_ROOT_PORT_DEV    *RpDev,
  IN PCIE_SPEED            TargetPcieSpeed
  )
{

  //
  // Set target link speed
  //
  if (TargetPcieSpeed >= PcieGen5) {
    RpDev->PciCfgAccess->AndThenOr16 (RpDev->PciCfgAccess, R_PCIE_CFG_LCTL2, (UINT16) ~B_PCIE_LCTL2_TLS, PcieGen5);
  } else if (TargetPcieSpeed >= PcieGen4) {
    RpDev->PciCfgAccess->AndThenOr16 (RpDev->PciCfgAccess, R_PCIE_CFG_LCTL2, (UINT16) ~B_PCIE_LCTL2_TLS, PcieGen4);
  } else if (TargetPcieSpeed >= PcieGen3) {
    RpDev->PciCfgAccess->AndThenOr16 (RpDev->PciCfgAccess, R_PCIE_CFG_LCTL2, (UINT16) ~B_PCIE_LCTL2_TLS, PcieGen3);
  }
  //
  // Set the perform equalization bit and start the link retrain.
  //
  RpDev->PciCfgAccess->Or32 (RpDev->PciCfgAccess, R_PCIE_CFG_LCTL3, B_PCIE_CFG_LCTL3_PE);
  RpDev->PciCfgAccess->Or32 (RpDev->PciCfgAccess, R_PCIE_CFG_LCTL, B_PCIE_CFG_LCTL_RL);
}

/**
  Initializes preset list that will be used during EQ

  @param[in] RpDev             Pointer to the root port device
  @param[in] PresetList        Pointer to the preset list
  @param[in] PresetCount       Number of presets in the list
  @param[in] TargetPcieSpeed   PCIe speed to train to
**/
STATIC
VOID
PcieSipInitializePresetList (
  IN PCIE_ROOT_PORT_DEV     *RpDev,
  IN UINT32                 *PresetList,
  IN UINT32                 PresetCount,
  IN PCIE_SPEED             TargetPcieSpeed
  )
{
  UINT32    PresetIndex;

 if (PresetCount > 11) {
    DEBUG ((DEBUG_ERROR, "PCIe EQ: Maximum number of presets exceeded, failed with preset count = %d\n", PresetCount));
    ASSERT (FALSE);
    return;
  }

  for (PresetIndex = 0; PresetIndex < PresetCount; PresetIndex++) {
    DEBUG ((DEBUG_INFO, "PCIe EQ: Setting preset %d, preset value = %d\n", PresetIndex, PresetList[PresetIndex]));

    PcieSipSetPresetCoefficientListEntry (RpDev, PresetIndex, PresetList[PresetIndex], TargetPcieSpeed);
  }
}

/**
  Initializes coefficient list that will be used during EQ.

  @param[in] RpDev             Pointer to the root port device
  @param[in] CoefficientsList  Pointer on the coefficients list
  @param[in] CoefficientCount  Number of coefficients in the list
  @param[in] TargetPcieSpeed   PCIe speed to train to
**/
STATIC
VOID
PcieSipInitializeCoefficientsList (
  IN PCIE_ROOT_PORT_DEV           *RpDev,
  IN PCIE_LINK_EQ_COEFFICIENTS    *CoefficientsList,
  IN UINT32                       CoefficientCount,
  IN PCIE_SPEED                   TargetPcieSpeed
  )
{
  UINT32    CoefficientIndex;
  UINT32    PreCursorListIndex;
  UINT32    PostCursorListIndex;

  if (CoefficientCount >= 10) {
    DEBUG ((DEBUG_ERROR, "PCIe EQ: Maximum number of coefficients exceeded, failed with coefficient count = %d\n", CoefficientCount));
    ASSERT (FALSE);
    return;
  }

  for (CoefficientIndex = 0; CoefficientIndex < CoefficientCount; CoefficientIndex++) {
    DEBUG ((DEBUG_INFO, "PCIe EQ: Setting coefficient %d, pre-cursor = %d, post-cursor = %d\n",
            CoefficientIndex,
            CoefficientsList[CoefficientIndex].PreCursor,
            CoefficientsList[CoefficientIndex].PostCursor));
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

    PcieSipSetPresetCoefficientListEntry (RpDev, PreCursorListIndex, CoefficientsList[CoefficientIndex].PreCursor, TargetPcieSpeed);
    PcieSipSetPresetCoefficientListEntry (RpDev, PostCursorListIndex, CoefficientsList[CoefficientIndex].PostCursor, TargetPcieSpeed);
  }
}

/**
  Sets up the hardware for Gen3 autonomus equalization.

  @param[in] RpDev               Pointer to the root port device
  @param[in] PcieLinkEqSettings  PCIe link EQ settings
**/
STATIC
VOID
PcieSipSetupGen3HardwareEq (
  IN PCIE_ROOT_PORT_DEV       *RpDev,
  IN PCIE_LINK_EQ_SETTINGS    *PcieLinkEqSettings
  )
{
  switch (PcieLinkEqSettings->PlatformSettings.PcieLinkEqMode) {
    case PcieLinkEqPresetMode:
      PcieSipSetPresetEqMode (RpDev, PcieGen3);
      PcieSipInitializePresetList (
        RpDev,
        PcieLinkEqSettings->PlatformSettings.Ph3PresetList,
        PcieLinkEqSettings->PlatformSettings.Ph3NoOfPresetOrCoeff,
        PcieGen3
        );
      break;
    case PcieLinkEqCoefficientMode:
      PcieSipSetCoefficientEqMode (RpDev, PcieGen3);
      PcieSipInitializeCoefficientsList (
        RpDev,
        PcieLinkEqSettings->PlatformSettings.Ph3CoefficientsList,
        PcieLinkEqSettings->PlatformSettings.Ph3NoOfPresetOrCoeff,
        PcieGen3
        );
      break;
    default:
     DEBUG ((DEBUG_ERROR, "Incorrect Gen3 equalization mode = %d\n", PcieLinkEqSettings->PlatformSettings.PcieLinkEqMode));
  }

  if (PcieLinkEqSettings->PlatformSettings.EqPh23Bypass) {
    RpDev->PciSbiMsgCfgAccess->Or32 (
      RpDev->PciSbiMsgCfgAccess,
      R_PCIE_CFG_EQCFG1,
      (UINT32) B_PCIE_CFG_EQCFG1_LEP23B
    );
  }
  if (PcieLinkEqSettings->PlatformSettings.EqPh3Bypass) {
    RpDev->PciSbiMsgCfgAccess->Or32 (
      RpDev->PciSbiMsgCfgAccess,
      R_PCIE_CFG_EQCFG1,
      (UINT32) B_PCIE_CFG_EQCFG1_LEP3B
    );
  }
  RpDev->PciSbiMsgCfgAccess->Or32 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_EQCFG1,
    (UINT32) N_PCIE_CFG_EQCFG1_LEB
  );

  //
  // This programing tells hardware to perform preset/coefficient search only up to the given preset/coefficient. We want hardware to search
  // all presets/coefficients that were passed in the list to BIOS. This register is 0 based, meaning that value of 0 will cause hardware to
  // terminate search after preset/coefficient 0.
  //
  RpDev->PciSbiMsgCfgAccess->AndThenOr32 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_EQCFG2,
    (UINT32) ~(B_PCIE_CFG_EQCFG2_HAPCSB),
    (UINT32) ((PcieLinkEqSettings->PlatformSettings.Ph3NoOfPresetOrCoeff - 1) << N_PCIE_CFG_EQCFG2_HAPCSB)
    );

  RpDev->PciSbiMsgCfgAccess->AndThenOr32 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_EQCFG4,
    (UINT32) ~(B_PCIE_CFG_EQCFG4_PX8GTSWLPCE),
    PcieLinkEqSettings->PlatformSettings.TsLockTimer << N_PCIE_CFG_EQCFG4_PX8GTSWLPCE
    );

  //
  // Program the preset/coefficient evaluation time depending on the number of presets/coefficients used in EQ.
  // HW EQ has to be finished within 24ms so the more coefficient/presets we have to evaluate the less time we can
  // spend on each.
  // NOTE: Encoding of the register is not 1:1 with time in ms. Please see register description for details.
  //
  RpDev->PciSbiMsgCfgAccess->AndThenOr32 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_EQCFG2,
    (UINT32) ~B_PCIE_CFG_EQCFG2_PCET,
    PcieLinkEqSettings->PlatformSettings.PCETTimer << N_PCIE_CFG_EQCFG2_PCET
    );

  //
  // Set Receiver wait time for EQ evaluation to 1us.
  //
  RpDev->PciSbiMsgCfgAccess->AndThenOr32 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_EQCFG1,
    (UINT32) ~(0xF << N_PCIE_CFG_EQCFG1_RWTNEVE),
    1 << N_PCIE_CFG_EQCFG1_RWTNEVE
    );

  //
  // Set the receiver eye width margin error threshold to 2 errors.
  // The value of the error threshold is calculated in hardware by
  // multiplying value from REWMET with multiplier from REWMETM. Value
  // of 0 in REWMETM means no multiplier.
  //
  RpDev->PciSbiMsgCfgAccess->AndThenOr32 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_EQCFG2,
    (UINT32) ~(B_PCIE_CFG_EQCFG2_REWMET | B_PCIE_CFG_EQCFG2_REWMETM),
    2
    );

  //
  // Set HAEQ.DL to 0xE
  //
  RpDev->PciSbiMsgCfgAccess->Or32 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_HAEQ,
    0xE << N_PCIE_CFG_HAEQ_DL
    );
}

/**
  Sets up the hardware for Gen4 autonomus equalization.

  @param[in] RpDev               Pointer to the root port device
  @param[in] PcieLinkEqSettings  PCIe link EQ settings
**/
STATIC
VOID
PcieSipSetupGen4HardwareEq (
  IN PCIE_ROOT_PORT_DEV       *RpDev,
  IN PCIE_LINK_EQ_SETTINGS    *PcieLinkEqSettings
  )
{
  switch (PcieLinkEqSettings->PlatformSettings.PcieLinkEqMode) {
    case PcieLinkEqPresetMode:
      PcieSipSetPresetEqMode (RpDev, PcieGen4);
      PcieSipInitializePresetList (
        RpDev,
        PcieLinkEqSettings->PlatformSettings.Ph3PresetList,
        PcieLinkEqSettings->PlatformSettings.Ph3NoOfPresetOrCoeff,
        PcieGen4
        );
      break;
    case PcieLinkEqCoefficientMode:
      PcieSipSetCoefficientEqMode (RpDev, PcieGen4);
      PcieSipInitializeCoefficientsList (RpDev,
      PcieLinkEqSettings->PlatformSettings.Ph3CoefficientsList,
      PcieLinkEqSettings->PlatformSettings.Ph3NoOfPresetOrCoeff,
      PcieGen4
      );
      break;
    default:
     DEBUG ((DEBUG_ERROR, "Incorrect Gen4 equalization mode\n"));
  }

  if (PcieLinkEqSettings->PlatformSettings.EqPh23Bypass) {
    RpDev->PciSbiMsgCfgAccess->Or32 (
      RpDev->PciSbiMsgCfgAccess,
      R_PCIE_CFG_EQCFG4,
      (UINT32) B_PCIE_CFG_EQCFG4_PX16GLEP23B
    );
  }

  if (PcieLinkEqSettings->PlatformSettings.EqPh3Bypass) {
    RpDev->PciSbiMsgCfgAccess->Or32 (
      RpDev->PciSbiMsgCfgAccess,
      R_PCIE_CFG_EQCFG4,
      (UINT32) B_PCIE_CFG_EQCFG4_PX16GLEP3B
    );
  }

  RpDev->PciSbiMsgCfgAccess->Or32 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_EQCFG4,
    (UINT32) BIT19
  );
  //
  // This programing tells hardware to perform preset/coefficient search only up to the given preset/coefficient. We want hardware to search
  // all presets/coefficients that were passed in the list to BIOS. This register is 0 based, meaning that value of 0 will cause hardware to
  // terminate search after preset/coefficient 0.
  //
  RpDev->PciSbiMsgCfgAccess->AndThenOr32 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_EQCFG5,
    (UINT32) ~B_PCIE_CFG_EQCFG5_HAPCSB,
    (PcieLinkEqSettings->PlatformSettings.Ph3NoOfPresetOrCoeff - 1) << N_PCIE_CFG_EQCFG5_HAPCSB
    );

  RpDev->PciSbiMsgCfgAccess->AndThenOr32 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_EQCFG4,
    (UINT32) ~(B_PCIE_CFG_EQCFG4_PX16GTSWLPCE),
    PcieLinkEqSettings->PlatformSettings.TsLockTimer << N_PCIE_CFG_EQCFG4_PX16GTSWLPCE
    );

  //
  // Program the preset/coefficient evaluation time depending on the number of presets/coefficients used in EQ.
  // HW EQ has to be finished within 24ms so the more coefficient/presets we have to evaluate the less time we can
  // spend on each.
  // NOTE: Encoding of the register is not 1:1 with time in ms. Please see register description for details.
  //
  RpDev->PciSbiMsgCfgAccess->AndThenOr32 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_EQCFG5,
    (UINT32) ~B_PCIE_CFG_EQCFG5_PCET,
    PcieLinkEqSettings->PlatformSettings.PCETTimer << N_PCIE_CFG_EQCFG5_PCET
    );

  //
  // Set Receiver wait time for EQ evaluation to 3us.
  //
  RpDev->PciSbiMsgCfgAccess->AndThenOr32 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_EQCFG4,
    (UINT32) ~(B_PCIE_CFG_EQCFG4_PX16GRWTNEVE | B_PCIE_CFG_EQCFG4_PX16GHAPCCPI),
    (0x3 << N_PCIE_CFG_EQCFG4_PX16GRWTNEVE) |
    ((PcieLinkEqSettings->PlatformSettings.Ph3NoOfPresetOrCoeff - 1) << N_PCIE_CFG_EQCFG4_PX16GHAPCCPI)
    );
}

/**
  Sets up the hardware for Gen5 autonomus equalization.

  @param[in] RpDev               Pointer to the root port device
  @param[in] PcieLinkEqSettings  PCIe link EQ settings
**/
STATIC
VOID
PcieSipSetupGen5HardwareEq (
  IN PCIE_ROOT_PORT_DEV       *RpDev,
  IN PCIE_LINK_EQ_SETTINGS    *PcieLinkEqSettings
  )
{
  switch (PcieLinkEqSettings->PlatformSettings.PcieLinkEqMode) {
    case PcieLinkEqPresetMode:
      PcieSipSetPresetEqMode (RpDev, PcieGen5);
      PcieSipInitializePresetList (
        RpDev,
        PcieLinkEqSettings->PlatformSettings.Ph3PresetList,
        PcieLinkEqSettings->PlatformSettings.Ph3NoOfPresetOrCoeff,
        PcieGen5
        );
      break;
    case PcieLinkEqCoefficientMode:
      PcieSipSetCoefficientEqMode (RpDev, PcieGen5);
      PcieSipInitializeCoefficientsList (RpDev,
      PcieLinkEqSettings->PlatformSettings.Ph3CoefficientsList,
      PcieLinkEqSettings->PlatformSettings.Ph3NoOfPresetOrCoeff,
      PcieGen5
      );
      break;
    default:
     DEBUG ((DEBUG_ERROR, "Incorrect Gen5 equalization mode\n"));
  }

  if (PcieLinkEqSettings->PlatformSettings.EqPh23Bypass) {
    RpDev->PciSbiMsgMemAccess->Or32 (
      RpDev->PciSbiMsgMemAccess,
      R_PCIE_MEM_PX32EQCFG1,
      (UINT32) B_PCIE_MEM_PX32EQCFG1_PX32GLEP23B
    );
  }
  if (PcieLinkEqSettings->PlatformSettings.EqPh3Bypass) {
    RpDev->PciSbiMsgMemAccess->Or32 (
      RpDev->PciSbiMsgMemAccess,
      R_PCIE_MEM_PX32EQCFG1,
      (UINT32) B_PCIE_MEM_PX32EQCFG1_PX32GLEP3B
    );
  }
  RpDev->PciSbiMsgMemAccess->Or32 (
    RpDev->PciSbiMsgMemAccess,
    R_PCIE_MEM_PX32EQCFG1,
    (UINT32) BIT19
  );

  //
  // This programing tells hardware to perform preset/coefficient search only up to the given preset/coefficient. We want hardware to search
  // all presets/coefficients that were passed in the list to BIOS. This register is 0 based, meaning that value of 0 will cause hardware to
  // terminate search after preset/coefficient 0.
  //
  RpDev->PciSbiMsgMemAccess->AndThenOr32 (
    RpDev->PciSbiMsgMemAccess,
    R_PCIE_MEM_PX32EQCFG2,
    (UINT32) ~B_PCIE_MEM_PX32EQCFG2_HAPCSB,
    (PcieLinkEqSettings->PlatformSettings.Ph3NoOfPresetOrCoeff - 1) << N_PCIE_MEM_PX32EQCFG2_HAPCSB
    );

  RpDev->PciSbiMsgMemAccess->AndThenOr32 (
    RpDev->PciSbiMsgMemAccess,
    R_PCIE_MEM_PX32EQCFG1,
    (UINT32) ~(B_PCIE_MEM_PX32EQCFG1_PX32GTSWLPCE),
    PcieLinkEqSettings->PlatformSettings.TsLockTimer << N_PCIE_MEM_PX32EQCFG1_PX32GTSWLPCE
    );

  //
  // Program the preset/coefficient evaluation time depending on the number of presets/coefficients used in EQ.
  // HW EQ has to be finished within 24ms so the more coefficient/presets we have to evaluate the less time we can
  // spend on each.
  // NOTE: Encoding of the register is not 1:1 with time in ms. Please see register description for details.
  //
  RpDev->PciSbiMsgMemAccess->AndThenOr32 (
    RpDev->PciSbiMsgMemAccess,
    R_PCIE_MEM_PX32EQCFG2,
    (UINT32) ~B_PCIE_MEM_PX32EQCFG2_PCET,
    PcieLinkEqSettings->PlatformSettings.PCETTimer << N_PCIE_MEM_PX32EQCFG2_PCET
    );

  //
  // Set Receiver wait time for EQ evaluation to 3us.
  // @todo Need to define test menu for Ph23Bypass and Ph3Bypass. program LEP23B and LEP3B accordingly
  // @todo Need to define TS lock timer test menu and program TSWLPCE accordingly
  //
  RpDev->PciSbiMsgMemAccess->AndThenOr32 (
    RpDev->PciSbiMsgMemAccess,
    R_PCIE_MEM_PX32EQCFG1,
    (UINT32) ~(B_PCIE_MEM_PX32EQCFG1_PX32GLEP23B | B_PCIE_MEM_PX32EQCFG1_PX32GLEP3B |
               B_PCIE_MEM_PX32EQCFG1_PX32GRWTNEVE | B_PCIE_MEM_PX32EQCFG1_PX32GHAPCCPI),
    (0x1 << N_PCIE_MEM_PX32EQCFG1_PX32GTSWLPCE) | (0x1 << N_PCIE_MEM_PX32EQCFG1_PX32GRWTNEVE) |
    ((PcieLinkEqSettings->PlatformSettings.Ph3NoOfPresetOrCoeff - 1) << N_PCIE_MEM_PX32EQCFG1_PX32GHAPCCPI)
    );
}

/**
  Sets up hardware for fixed EQ method.

  @param[in] RpDev               Pointer to the root port device
  @param[in] PcieLinkEqSettings  PCIe link EQ settings
  @param[in] RootPortPcieSpeed   PCIe speed of the Root Port
**/
STATIC
VOID
PcieSipSetupFixedEq (
  IN PCIE_ROOT_PORT_DEV       *RpDev,
  IN PCIE_LINK_EQ_SETTINGS    *PcieLinkEqSettings,
  IN PCIE_SPEED               RootPortPcieSpeed
  )
{
  UINT32    LinkWidth;
  UINT32    Data32Or;
  Data32Or = 0;

  LinkWidth = PcieSipGetMaxLinkWidth (RpDev);
  if (PcieLinkEqSettings->PlatformSettings.Ph3NoOfPresetOrCoeff < LinkWidth) {
    DEBUG ((DEBUG_ERROR, "Fixed EQ requested with insufficient number of presets or coefficients\n"));
    ASSERT (FALSE);
    return;
  }

  //
  // Set preset/coefficient evaluation bypass and override enable. This will cause the root port
  // to unconditionally accept remote transmiter values.
  //
  switch (RootPortPcieSpeed) {
    case PcieGen5:
      Data32Or = B_PCIE_MEM_PX32EQCFG1_PX32GRTPCOE | B_PCIE_MEM_PX32EQCFG1_PX32GRTLEPCEB;
      RpDev->PciSbiMsgMemAccess->Or32 (RpDev->PciSbiMsgMemAccess, R_PCIE_MEM_PX32EQCFG1, Data32Or);
      break;
    case PcieGen4:
      Data32Or = B_PCIE_CFG_EQCFG4_PX16GRTPCOE | B_PCIE_CFG_EQCFG4_PX16GRTLEPCEB;
      RpDev->PciSbiMsgCfgAccess->Or32 (RpDev->PciSbiMsgCfgAccess, R_PCIE_CFG_EQCFG4, Data32Or);
      break;
    case PcieGen3:
      Data32Or = B_PCIE_CFG_EQCFG1_RTPCOE | B_PCIE_CFG_EQCFG1_RTLEPCEB;
      RpDev->PciSbiMsgCfgAccess->Or32 (RpDev->PciSbiMsgCfgAccess, R_PCIE_CFG_EQCFG1, Data32Or);
      break;
    default:
      DEBUG ((DEBUG_ERROR, "Equalization not possible at this speed\n"));
      break;
  }

  //
  // Fixed equalization algorithm performs remote transmitter override instead of searching for best values.
  // In remote transmitter override mode the meaning of preset list and coefficient list is changed and instead
  // of containing the list of values that should be searched it contains fixed values of the presets or coefficients
  // for each PCIe lane that will be used to override.
  //
  switch (PcieLinkEqSettings->PlatformSettings.PcieLinkEqMode) {
    case PcieLinkEqPresetMode:
      PcieSipSetPresetEqMode (RpDev, RootPortPcieSpeed);
      PcieSipInitializePresetList (RpDev, PcieLinkEqSettings->PlatformSettings.Ph3PresetList, LinkWidth, RootPortPcieSpeed);
      break;
    case PcieLinkEqCoefficientMode:
      PcieSipSetCoefficientEqMode (RpDev, RootPortPcieSpeed);
      PcieSipInitializeCoefficientsList (RpDev, PcieLinkEqSettings->PlatformSettings.Ph3CoefficientsList, LinkWidth, RootPortPcieSpeed);
      break;
    default:
     DEBUG ((DEBUG_ERROR, "Incorrect equalization mode\n"));
  }
}

/**
  Clears Gen3 Gen4 EQ related registers to protect against EQ reruns and bad defaults.

  @param[in] RpDev     Pointer to the root port device
**/
STATIC
VOID
PcieSipClearEqRegisters (
  IN PCIE_ROOT_PORT_DEV    *RpDev
  )
{
  RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_EQCFG1, 0);
  RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_EQCFG2, 0);
  RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_EQCFG4, 0);
  RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_EQCFG5, 0);
  RpDev->PciCfgAccess->Write32 (RpDev->PciCfgAccess, R_PCIE_CFG_HAEQ, 0);
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
VOID
GetSipPresetToCoefficientRegisterLayout (
  IN  UINT32                                  PresetIndex,
  OUT PRESET_TO_COEFFICIENT_REGISTER_LAYOUT   *RegisterLayout
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

  @param[in] RpDev                Pointer to the root port device
  @param[in] LaneIndex            PCIe lane for which to set coefficients
  @param[in] PresetIndex          Preset index for the given lane
  @param[in] LowFrequencyValue    Low frequency value for the given lane
  @param[in] FullSwingValue       Full swing value for the given lane
  @param[in] PresetToCoefficient  Structure with information about coefficients for the preset
**/
VOID
SetSipGen3PresetToCoefficientMapping (
  IN PCIE_ROOT_PORT_DEV                    *RpDev,
  IN UINT32                                LaneIndex,
  IN UINT32                                PresetIndex,
  IN UINT32                                LowFrequencyValue,
  IN UINT32                                FullSwingValue,
  IN PCIE_PRESET_TO_COEFFICIENT_MAPPING    PresetToCoefficient
  )
{
  UINT32                                   LaneBase;
  PRESET_TO_COEFFICIENT_REGISTER_LAYOUT    RegisterLayout;

  LaneBase = R_PCIE_CFG_L0P0P1PCM + (LaneIndex * S_PCIE_CFG_SINGLE_LANE_PRESETS);
  GetSipPresetToCoefficientRegisterLayout (PresetIndex, &RegisterLayout);

  RpDev->PciSbiMsgCfgAccess->AndThenOr32 (
    RpDev->PciSbiMsgCfgAccess,
    LaneBase + RegisterLayout.CursorRegisterOffset,
    ~(0x3F << RegisterLayout.CursorFieldOffset),
    (PresetToCoefficient.CursorCoefficient & 0x3F) << RegisterLayout.CursorFieldOffset
    );
  RpDev->PciSbiMsgCfgAccess->AndThenOr32 (
    RpDev->PciSbiMsgCfgAccess,
    LaneBase + RegisterLayout.PreCursorRegisterOffset,
    ~(0x3F << RegisterLayout.PreCursorFieldOffset),
    (PresetToCoefficient.PreCursorCoefficient & 0x3F) << RegisterLayout.PreCursorFieldOffset
    );
  RpDev->PciSbiMsgCfgAccess->AndThenOr32 (
    RpDev->PciSbiMsgCfgAccess,
    LaneBase + RegisterLayout.PostCursorRegisterOffset,
    ~(0x3F << RegisterLayout.PostCursorFieldOffset),
    (PresetToCoefficient.PostCursorCoefficient & 0x3F) << RegisterLayout.PostCursorFieldOffset
    );
  RpDev->PciSbiMsgCfgAccess->AndThenOr32 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_L0P0P1PCM  + R_PCIE_CFG_LFFS_OFFSET_FROM_LANE_BASE,
    ~(UINT32)((0x3F << N_PCIE_CFG_LFFS_FS) | 0x3F),
    ((FullSwingValue & 0x3F) << N_PCIE_CFG_LFFS_FS) | (LowFrequencyValue & 0x3F)
    );
}

/**
  Set Gen4 preset coefficients.

  @param[in] RpDev                Pointer to the root port device
  @param[in] LaneIndex            PCIe lane for which to set coefficients
  @param[in] PresetIndex          Preset index for the given lane
  @param[in] LowFrequencyValue    Low frequency value for the given lane
  @param[in] FullSwingValue       Full swing value for the given lane
  @param[in] PresetToCoefficient  Structure with information about coefficients for the preset
**/
VOID
SetSipGen4PresetToCoefficientMapping (
  IN PCIE_ROOT_PORT_DEV                    *RpDev,
  IN UINT32                                PresetIndex,
  IN UINT32                                LowFrequencyValue,
  IN UINT32                                FullSwingValue,
  IN PCIE_PRESET_TO_COEFFICIENT_MAPPING    PresetToCoefficient
  )
{
  PRESET_TO_COEFFICIENT_REGISTER_LAYOUT    RegisterLayout;

  GetSipPresetToCoefficientRegisterLayout (PresetIndex, &RegisterLayout);

  RpDev->PciSbiMsgCfgAccess->AndThenOr32 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_PX16GP0P1PCM + RegisterLayout.CursorRegisterOffset,
    ~(0x3F << RegisterLayout.CursorFieldOffset),
    (PresetToCoefficient.CursorCoefficient & 0x3F) << RegisterLayout.CursorFieldOffset
    );
  RpDev->PciSbiMsgCfgAccess->AndThenOr32 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_PX16GP0P1PCM + RegisterLayout.PreCursorRegisterOffset,
    ~(0x3F << RegisterLayout.PreCursorFieldOffset),
    (PresetToCoefficient.PreCursorCoefficient & 0x3F) << RegisterLayout.PreCursorFieldOffset
    );
  RpDev->PciSbiMsgCfgAccess->AndThenOr32 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_PX16GP0P1PCM + RegisterLayout.PostCursorRegisterOffset,
    ~(0x3F << RegisterLayout.PostCursorFieldOffset),
    (PresetToCoefficient.PostCursorCoefficient & 0x3F) << RegisterLayout.PostCursorFieldOffset
    );
  RpDev->PciSbiMsgCfgAccess->AndThenOr32 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_PX16GP0P1PCM + R_PCIE_CFG_LFFS_OFFSET_FROM_LANE_BASE,
    ~(UINT32)((0x3F << N_PCIE_CFG_LFFS_FS) | 0x3F),
    ((FullSwingValue & 0x3F) << N_PCIE_CFG_LFFS_FS) | (LowFrequencyValue & 0x3F)
    );
}

/**
  Set Gen5 preset coefficients.

  @param[in] RpDev                Pointer to the root port device
  @param[in] LaneIndex            PCIe lane for which to set coefficients
  @param[in] PresetIndex          Preset index for the given lane
  @param[in] LowFrequencyValue    Low frequency value for the given lane
  @param[in] FullSwingValue       Full swing value for the given lane
  @param[in] PresetToCoefficient  Structure with information about coefficients for the preset
**/
STATIC
VOID
SetSipGen5PresetToCoefficientMapping (
  IN PCIE_ROOT_PORT_DEV                    *RpDev,
  IN UINT32                                PresetIndex,
  IN UINT32                                LowFrequencyValue,
  IN UINT32                                FullSwingValue,
  IN PCIE_PRESET_TO_COEFFICIENT_MAPPING    PresetToCoefficient
  )
{
  PRESET_TO_COEFFICIENT_REGISTER_LAYOUT    RegisterLayout;

  GetSipPresetToCoefficientRegisterLayout (PresetIndex, &RegisterLayout);

  RpDev->PciSbiMsgMemAccess->AndThenOr32 (
    RpDev->PciSbiMsgMemAccess,
    R_PCIE_MEM_PX32GP0P1PCM + RegisterLayout.CursorRegisterOffset,
    ~(0x3F << RegisterLayout.CursorFieldOffset),
    (PresetToCoefficient.CursorCoefficient & 0x3F) << RegisterLayout.CursorFieldOffset
    );
  RpDev->PciSbiMsgMemAccess->AndThenOr32 (
    RpDev->PciSbiMsgMemAccess,
    R_PCIE_MEM_PX32GP0P1PCM + RegisterLayout.PreCursorRegisterOffset,
    ~(0x3F << RegisterLayout.PreCursorFieldOffset),
    (PresetToCoefficient.PreCursorCoefficient & 0x3F) << RegisterLayout.PreCursorFieldOffset
    );
  RpDev->PciSbiMsgMemAccess->AndThenOr32 (
    RpDev->PciSbiMsgMemAccess,
    R_PCIE_MEM_PX32GP0P1PCM + RegisterLayout.PostCursorRegisterOffset,
    ~(0x3F << RegisterLayout.PostCursorFieldOffset),
    (PresetToCoefficient.PostCursorCoefficient & 0x3F) << RegisterLayout.PostCursorFieldOffset
    );
  RpDev->PciSbiMsgMemAccess->AndThenOr32 (
    RpDev->PciSbiMsgMemAccess,
    R_PCIE_MEM_PX32GP0P1PCM + R_PCIE_CFG_LFFS_OFFSET_FROM_LANE_BASE,
    ~(UINT32)((0x3F << N_PCIE_CFG_LFFS_FS) | 0x3F),
    ((FullSwingValue & 0x3F) << N_PCIE_CFG_LFFS_FS) | (LowFrequencyValue & 0x3F)
    );
}

/**
  Get coefficients values corresponding to given preset.
  This function will return valid values only after
  PcieSipConfigurePresetToCoefficientMapping has been called.

  @param[in]  RpDev         Pointer to the root port device
  @param[in]  LaneIndex     PCIe lane for which to get coefficients
  @param[in]  PresetIndex   Preset index for given lane
  @param[out] Coefficients  Value of the coefficients extracted from RP HW
**/
VOID
GetSipGen3PresetToCoefficientMapping (
  IN  PCIE_ROOT_PORT_DEV           *RpDev,
  IN  UINT32                       LaneIndex,
  IN  UINT32                       PresetIndex,
  OUT PCIE_LINK_EQ_COEFFICIENTS    *Coefficients
  )
{
  UINT32                                   LaneBase;
  PRESET_TO_COEFFICIENT_REGISTER_LAYOUT    RegisterLayout;

  LaneBase = R_PCIE_CFG_L0P0P1PCM + (LaneIndex * S_PCIE_CFG_SINGLE_LANE_PRESETS);
  GetSipPresetToCoefficientRegisterLayout (PresetIndex, &RegisterLayout);

  Coefficients->PreCursor = (RpDev->PciSbiMsgCfgAccess->Read32 (RpDev->PciCfgAccess, LaneBase + RegisterLayout.PreCursorRegisterOffset) >> RegisterLayout.PreCursorFieldOffset) & 0x3F;
  Coefficients->PostCursor = (RpDev->PciSbiMsgCfgAccess->Read32 (RpDev->PciCfgAccess, LaneBase + RegisterLayout.PostCursorRegisterOffset) >> RegisterLayout.PostCursorFieldOffset) & 0x3F;
}

/**
  Get coefficients values corresponding to given preset.
  This function will return valid values only after
  PcieSipConfigurePresetToCoefficientMapping has been called.

  @param[in]  RpDev         Pointer to the root port device
  @param[in]  LaneIndex     PCIe lane for which to get coefficients
  @param[in]  PresetIndex   Preset index for given lane
  @param[out] Coefficients  Value of the coefficients extracted from RP HW
**/
VOID
GetSipGen4PresetToCoefficientMapping (
  IN  PCIE_ROOT_PORT_DEV           *RpDev,
  IN  UINT32                       LaneIndex,
  IN  UINT32                       PresetIndex,
  OUT PCIE_LINK_EQ_COEFFICIENTS    *Coefficients
  )
{
  UINT32                                   LaneBase;
  PRESET_TO_COEFFICIENT_REGISTER_LAYOUT    RegisterLayout;

  LaneBase = R_PCIE_CFG_PX16GP0P1PCM + (LaneIndex * S_PCIE_CFG_SINGLE_LANE_PRESETS);
  GetSipPresetToCoefficientRegisterLayout (PresetIndex, &RegisterLayout);

  Coefficients->PreCursor = (RpDev->PciSbiMsgCfgAccess->Read32 (RpDev->PciSbiMsgCfgAccess, LaneBase + RegisterLayout.PreCursorRegisterOffset) >> RegisterLayout.PreCursorFieldOffset) & 0x3F;
  Coefficients->PostCursor = (RpDev->PciSbiMsgCfgAccess->Read32 (RpDev->PciSbiMsgCfgAccess, LaneBase + RegisterLayout.PostCursorRegisterOffset) >> RegisterLayout.PostCursorFieldOffset) & 0x3F;
}

/**
  Programs presets-to-coefficients mapping for hardware equalization.
  It should not be performed for ports without Gen3 capability

  @param[in] RpDev                    Pointer to the root port device
  @param[in] PcieGen3LinkEqSettings   Pointer to PCIe link EQ settings
  @param[in] PcieGen4LinkEqSettings   Pointer to PCIe link EQ settings
  @param[in] PcieGen5LinkEqSettings   Pointer to PCIe link EQ settings
  @param[in] TargetPcieSpeed          PCIe speed to train to
**/
STATIC
VOID
PcieSipConfigurePresetToCoefficientMapping (
  IN PCIE_ROOT_PORT_DEV       *RpDev,
  IN PCIE_LINK_EQ_SETTINGS    *PcieGen3LinkEqSettings,
  IN PCIE_LINK_EQ_SETTINGS    *PcieGen4LinkEqSettings,
  IN PCIE_LINK_EQ_SETTINGS    *PcieGen5LinkEqSettings,
  IN PCIE_SPEED               TargetPcieSpeed
  )
{
  UINT32    LaneIndex;
  UINT32    MaxLinkWidth;
  UINT32    PresetIndex;
  UINT32    TimeoutUs;
  BOOLEAN   Gen3QueryToPipeSupport;
  BOOLEAN   Gen4QueryToPipeSupport;
  BOOLEAN   Gen5QueryToPipeSupport;

  Gen3QueryToPipeSupport = PcieGen3LinkEqSettings->ProjectSettings.PipePresetToCoefficientQuerySupported;
  Gen4QueryToPipeSupport = PcieGen4LinkEqSettings->ProjectSettings.PipePresetToCoefficientQuerySupported;
  Gen5QueryToPipeSupport = PcieGen5LinkEqSettings->ProjectSettings.PipePresetToCoefficientQuerySupported;
  if (RpDev->SipVersion <= PcieSip14) {
    MaxLinkWidth = PcieSipGetMaxLinkWidth (RpDev);
  } else {
  //
  // Pcie SIP > 14 has space for only one lane PCM
  // configuration. This configuration is propagated
  // to all lanes.
  //
    MaxLinkWidth = 1;
  }

  if (Gen3QueryToPipeSupport || Gen4QueryToPipeSupport || Gen5QueryToPipeSupport) {
    //
    // If supported send a query to PIPE
    //
    RpDev->PciCfgAccess->Or32 (RpDev->PciCfgAccess, R_PCIE_CFG_EQCFG1, B_PCIE_CFG_EQCFG1_HPCMQE);
    TimeoutUs = 1000;
    while (TimeoutUs-- > 0) {
      if ((RpDev->PciCfgAccess->Read32 (RpDev->PciCfgAccess, R_PCIE_CFG_EQCFG1) & B_PCIE_CFG_EQCFG1_HPCMQE) == 0) {
        return;
      }
      MicroSecondDelay (1);
    }
    ASSERT (FALSE);
  } else {
    //
    // If query to PIPE is unsupported we need to program project's recommendation from IP
    //
    PERF_INMODULE_BEGIN ("CoeffMapping");
    for (LaneIndex = 0; LaneIndex < MaxLinkWidth; LaneIndex++) {
      for (PresetIndex = 0; PresetIndex < ARRAY_SIZE (PcieGen4LinkEqSettings->ProjectSettings.PresetToCoefficient); PresetIndex++) {
        switch (TargetPcieSpeed) {
          case PcieGen5:
            if (!Gen5QueryToPipeSupport) {
              SetSipGen5PresetToCoefficientMapping (
                RpDev,
                PresetIndex,
                PcieGen5LinkEqSettings->ProjectSettings.LowFrequencyValue,
                PcieGen5LinkEqSettings->ProjectSettings.FullSwingValue,
                PcieGen5LinkEqSettings->ProjectSettings.PresetToCoefficient[PresetIndex]
              );
            }
          case PcieGen4:
            if (!Gen4QueryToPipeSupport) {
              SetSipGen4PresetToCoefficientMapping (
                RpDev,
                PresetIndex,
                PcieGen4LinkEqSettings->ProjectSettings.LowFrequencyValue,
                PcieGen4LinkEqSettings->ProjectSettings.FullSwingValue,
                PcieGen4LinkEqSettings->ProjectSettings.PresetToCoefficient[PresetIndex]
              );
            }
          case PcieGen3:
            if (!Gen3QueryToPipeSupport) {
              SetSipGen3PresetToCoefficientMapping (
                RpDev,
                LaneIndex,
                PresetIndex,
                PcieGen3LinkEqSettings->ProjectSettings.LowFrequencyValue,
                PcieGen3LinkEqSettings->ProjectSettings.FullSwingValue,
                PcieGen3LinkEqSettings->ProjectSettings.PresetToCoefficient[PresetIndex]
              );
            }
            break;
          default:
            DEBUG ((DEBUG_ERROR, "Preset to coefficient mapping not available at this speed\n"));
            break;
        }
      }
    }
    PERF_INMODULE_END ("CoeffMapping");
  }
}

/**
  Sets local transmitter override coefficients for a
  given lane for Gen3.

  @param[in] RpDev             Pointer to the root port device
  @param[in] LaneIndex         Index of the PCI lane
  @param[in] LaneCoefficients  Coefficients to be programed
**/
VOID
PcieSipSetGen3LocalTransmitterOverrideCoefficients (
  IN PCIE_ROOT_PORT_DEV           *RpDev,
  IN UINT32                       LaneIndex,
  IN PCIE_LINK_EQ_COEFFICIENTS    *LaneCoefficients
  )
{
  UINT32    LaneRegisterIndex;
  UINT32    LaneIndexInsideRegister;

  ASSERT ((LaneCoefficients->PreCursor & ~0x3F) == 0);
  ASSERT ((LaneCoefficients->PostCursor & ~0x3F) == 0);

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
  //
  RpDev->PciSbiMsgCfgAccess->AndThenOr32 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_LTCO1 + (4 * LaneRegisterIndex),
    (UINT32) ~((0x3F << (6 * LaneIndexInsideRegister)) | (0x3F << (12 * LaneIndexInsideRegister))),
    (LaneCoefficients->PreCursor << (6 * LaneIndexInsideRegister) | (LaneCoefficients->PostCursor << (12 * LaneIndexInsideRegister)))
    );

  RpDev->PciSbiMsgCfgAccess->Or32 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_LTCO1 + (4 * LaneRegisterIndex),
    B_PCIE_CFG_LTCO1_L0TCOE << LaneIndexInsideRegister
    );
}

/**
  Sets local transmitter override coefficients for a
  given lane for Gen4.

  @param[in] RpDev             Pointer to the root port device
  @param[in] LaneIndex         Index of the PCI lane
  @param[in] LaneCoefficients  Coefficients to be programed
**/
VOID
PcieSipSetGen4LocalTransmitterOverrideCoefficients (
  IN PCIE_ROOT_PORT_DEV           *RpDev,
  IN UINT32                       LaneIndex,
  IN PCIE_LINK_EQ_COEFFICIENTS    *LaneCoefficients
  )
{
  UINT32    LaneRegisterIndex;
  UINT32    LaneIndexInsideRegister;

  ASSERT ((LaneCoefficients->PreCursor & ~0x3F) == 0);
  ASSERT ((LaneCoefficients->PostCursor & ~0x3F) == 0);

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
  //
  RpDev->PciSbiMsgCfgAccess->AndThenOr32 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_PX16GLTCO1 + (4 * LaneRegisterIndex),
    (UINT32) ~((0x3F << (6 * LaneIndexInsideRegister)) | (0x3F << (12 * LaneIndexInsideRegister))),
    (LaneCoefficients->PreCursor << (6 * LaneIndexInsideRegister) | (LaneCoefficients->PostCursor << (12 * LaneIndexInsideRegister)))
    );

  RpDev->PciSbiMsgCfgAccess->Or32 (
    RpDev->PciSbiMsgCfgAccess,
    R_PCIE_CFG_PX16GLTCO1 + (4 * LaneRegisterIndex),
    B_PCIE_CFG_PX16GLTCO1_L0TCOE << LaneIndexInsideRegister
    );
}

/**
  Prints PCIe equalization settings

  @param[in] PcieLinkEqSettings  Pointer to PCIe link EQ settings
**/
STATIC
VOID
PcieSipPrintEqConfig (
  IN PCIE_LINK_EQ_SETTINGS    *PcieLinkEqSettings
  )
{
  UINT8    Index;

  DEBUG ((DEBUG_INFO, "Printing PCIe equalization settings\n"));
  DEBUG ((DEBUG_INFO, "PcieLinkEqMethod = %d\n", PcieLinkEqSettings->PlatformSettings.PcieLinkEqMethod));
  DEBUG ((DEBUG_INFO, "PcieLinkEqMode = %d\n", PcieLinkEqSettings->PlatformSettings.PcieLinkEqMode));
  DEBUG ((DEBUG_INFO, "Ph1DpTxPreset = %d\n", PcieLinkEqSettings->PlatformSettings.Ph1DpTxPreset));
  DEBUG ((DEBUG_INFO, "Ph1UpTxPreset = %d\n", PcieLinkEqSettings->PlatformSettings.Ph1UpTxPreset));
  DEBUG ((DEBUG_INFO, "Ph3NoOfPresetOrCoeff = %d\n", PcieLinkEqSettings->PlatformSettings.Ph3NoOfPresetOrCoeff));
  if (PcieLinkEqSettings->PlatformSettings.PcieLinkEqMode == PcieLinkEqPresetMode) {
    for (Index = 0; Index < PcieLinkEqSettings->PlatformSettings.Ph3NoOfPresetOrCoeff && Index < PCIE_LINK_EQ_PRESETS_MAX; Index++) {
      DEBUG ((DEBUG_INFO, "Phase 3 Preset %d = %d\n", Index, PcieLinkEqSettings->PlatformSettings.Ph3PresetList[Index]));
    }
  } else {
    for (Index = 0; Index < PcieLinkEqSettings->PlatformSettings.Ph3NoOfPresetOrCoeff && Index < PCIE_LINK_EQ_COEFFICIENTS_MAX; Index++) {
      DEBUG ((DEBUG_INFO, "Phase 3 Coefficient %d, pre-cursor = %d, post-cursor = %d\n",
                            Index,
                            PcieLinkEqSettings->PlatformSettings.Ph3CoefficientsList[Index].PreCursor,
                            PcieLinkEqSettings->PlatformSettings.Ph3CoefficientsList[Index].PostCursor));
    }
  }
  DEBUG ((DEBUG_INFO, "Phase 2 local transmitter override enable = %d\n", PcieLinkEqSettings->PlatformSettings.LocalTxOverrideEn));
  DEBUG ((DEBUG_INFO, "Phase 2 local transmitter override preset = %d\n", PcieLinkEqSettings->PlatformSettings.Ph2LocalTxOverridePreset));
  DEBUG ((DEBUG_INFO, "PipePresetToCoefficientQuerySupported = %d\n", PcieLinkEqSettings->ProjectSettings.PipePresetToCoefficientQuerySupported));
  if (!PcieLinkEqSettings->ProjectSettings.PipePresetToCoefficientQuerySupported) {
    DEBUG ((DEBUG_INFO, "LowFrequencyValue = %d\n", PcieLinkEqSettings->ProjectSettings.LowFrequencyValue));
    DEBUG ((DEBUG_INFO, "FullSwingValue = %d\n", PcieLinkEqSettings->ProjectSettings.FullSwingValue));
    for (Index = 0; Index < PCIE_LINK_EQ_PRESETS_MAX; Index++) {
      DEBUG ((DEBUG_INFO, "Preset %d, pre-cursror = %d, cursor = %d, post-cursor = %d\n",
                            Index,
                            PcieLinkEqSettings->ProjectSettings.PresetToCoefficient[Index].PreCursorCoefficient,
                            PcieLinkEqSettings->ProjectSettings.PresetToCoefficient[Index].CursorCoefficient,
                            PcieLinkEqSettings->ProjectSettings.PresetToCoefficient[Index].PostCursorCoefficient));
    }
  }
}

/**
  Performs PCIe link equalization according to platform settings.
  Please make sure that link is in gen1 or gen2 before calling this
  procedure.

  @param[in] RpDev                   Pointer to the root port device
  @param[in] EndPointAttached        Is End Point device connected to the Root Port
  @param[in] PcieGen3LinkEqSettings  PCIe Gen3 link equalization settings to be used during EQ
  @param[in] PcieGen4LinkEqSettings  PCIe Gen4 link equalization settings to be used during EQ
  @param[in] PcieGen5LinkEqSettings  PCIe Gen5 link equalization settings to be used during EQ
  @param[in] RootPortPcieSpeed       PCIe speed of the Root Port
  @param[in] EndPointPcieSpeed       PCIe speed of the End Point
**/
VOID
PcieSipLinkEqualize (
  IN PCIE_ROOT_PORT_DEV       *RpDev,
  IN BOOLEAN                  EndPointAttached,
  IN PCIE_LINK_EQ_SETTINGS    *PcieGen3LinkEqSettings,
  IN PCIE_LINK_EQ_SETTINGS    *PcieGen4LinkEqSettings,
  IN PCIE_LINK_EQ_SETTINGS    *PcieGen5LinkEqSettings,
  IN PCIE_SPEED               RootPortPcieSpeed,
  IN PCIE_SPEED               EndpointPcieSpeed
  )
{
  switch (RootPortPcieSpeed) {
    case PcieGen5:
      switch (PcieGen5LinkEqSettings->PlatformSettings.PcieLinkEqMethod) {
        case PcieLinkHardwareEq:
          PcieSipSetupGen5HardwareEq (RpDev, PcieGen5LinkEqSettings);
          break;
        case PcieLinkFixedEq:
          PcieSipSetupFixedEq (RpDev, PcieGen5LinkEqSettings, RootPortPcieSpeed);
          break;
        default:
          DEBUG ((DEBUG_ERROR, "Invalid PCIe link EQ method\n"));
          ASSERT (FALSE);
          break;
      }
    case PcieGen4:
      switch (PcieGen4LinkEqSettings->PlatformSettings.PcieLinkEqMethod) {
        case PcieLinkHardwareEq:
          PcieSipSetupGen4HardwareEq (RpDev, PcieGen4LinkEqSettings);
          break;
        case PcieLinkFixedEq:
          PcieSipSetupFixedEq (RpDev, PcieGen4LinkEqSettings, RootPortPcieSpeed);
          break;
        default:
          DEBUG ((DEBUG_ERROR, "Invalid PCIe link EQ method\n"));
          ASSERT (FALSE);
          break;
      }
    case PcieGen3:
      switch (PcieGen3LinkEqSettings->PlatformSettings.PcieLinkEqMethod) {
        case PcieLinkHardwareEq:
          PcieSipSetupGen3HardwareEq (RpDev, PcieGen3LinkEqSettings);
          break;
        case PcieLinkFixedEq:
          PcieSipSetupFixedEq (RpDev, PcieGen3LinkEqSettings, RootPortPcieSpeed);
          break;
        default:
          DEBUG ((DEBUG_ERROR, "Invalid PCIe link EQ method\n"));
          ASSERT (FALSE);
          break;
      }
      break;
    default:
      DEBUG ((DEBUG_ERROR, "Equalization not possible at this speed\n"));
      break;
  }
  DEBUG ((DEBUG_INFO, "EQCFG1: 0x%08x\n", RpDev->PciCfgAccess->Read32(RpDev->PciCfgAccess, R_PCIE_CFG_EQCFG1)));
  DEBUG ((DEBUG_INFO, "EQCFG2: 0x%08x\n", RpDev->PciCfgAccess->Read32(RpDev->PciCfgAccess, R_PCIE_CFG_EQCFG2)));
  DEBUG ((DEBUG_INFO, "HAEQ:   0x%08x\n", RpDev->PciCfgAccess->Read32(RpDev->PciCfgAccess, R_PCIE_CFG_HAEQ)));

  if ((RpDev->PcieConfig->PcieCommonConfig.ComplianceTestMode == TRUE) && (RootPortPcieSpeed >= PcieGen3)) {
    PcieSipStartEqualization (RpDev, RootPortPcieSpeed);
  }
  return;
}

/**
  Performs the initialization for hardware equalization to Gen3.
  Please make sure this function is called before programming any
  power management register in PCIe Root Port

  @param[in] RpDev                   Pointer to the root port device
  @param[in] PcieGen3LinkEqSettings  PCIe Gen3 link equalization settings to be used during EQ
  @param[in] PcieGen4LinkEqSettings  PCIe Gen4 link equalization settings to be used during EQ
  @param[in] PcieGen5LinkEqSettings  PCIe Gen5 link equalization settings to be used during EQ
**/
VOID
PcieSipLinkEqualizeInit (
  IN PCIE_ROOT_PORT_DEV       *RpDev,
  IN PCIE_LINK_EQ_SETTINGS    *PcieGen3LinkEqSettings,
  IN PCIE_LINK_EQ_SETTINGS    *PcieGen4LinkEqSettings,
  IN PCIE_LINK_EQ_SETTINGS    *PcieGen5LinkEqSettings
  )
{
  PCIE_SPEED    PcieSpeed;

  PcieSpeed = (PCIE_SPEED)(RpDev->PciCfgAccess->Read32 (RpDev->PciCfgAccess, R_PCIE_CFG_LCAP) & B_PCIE_LCAP_MLS);
  DEBUG ((DEBUG_INFO, "For Gen3\n"));
  PcieSipPrintEqConfig (PcieGen3LinkEqSettings);
  if (PcieSpeed >= PcieGen4) {
    DEBUG ((DEBUG_INFO, "For Gen4\n"));
    PcieSipPrintEqConfig (PcieGen4LinkEqSettings);
  }
  if (PcieSpeed >= PcieGen5) {
    DEBUG ((DEBUG_INFO, "For Gen5\n"));
    PcieSipPrintEqConfig (PcieGen5LinkEqSettings);
  }
  PcieSipClearEqRegisters (RpDev);

  PERF_INMODULE_BEGIN ("PcieConfigPresetToCoeff");
  PcieSipConfigurePresetToCoefficientMapping (RpDev, PcieGen3LinkEqSettings, PcieGen4LinkEqSettings, PcieGen5LinkEqSettings, PcieSpeed);
  PERF_INMODULE_END ("PcieConfigPresetToCoeff");
}
