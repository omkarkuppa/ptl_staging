/** @file
  Dump functions for USB4 capabilities and path entries.

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

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/CmUtilsLib.h>
#include <Library/Usb4ConfigSpace.h>
#include <Library/Usb4Capabilities.h>

/**
  Dump adapter TMU capabilities.

  @param[in] TmuCap - Pointer to TMU capabilities.
**/
VOID
DumpLaneAdapterTmuCap (
  IN USB4_ADP_CAP_TMU    *TmuCap
  )
{
  if (TmuCap != NULL) {
    DEBUG ((CAP_VERBOSE, "    LANE TMU Cap CS0 = 0x%08X\n", TmuCap->CapCs0));
    DEBUG ((CAP_VERBOSE, "    LANE TMU Cap CS1 = 0x%08X\n", TmuCap->CapCs1));
    DEBUG ((CAP_VERBOSE, "    LANE TMU Cap CS2 = 0x%08X\n", TmuCap->CapCs2));
    DEBUG ((CAP_VERBOSE, "    LANE TMU Cap CS3 = 0x%08X\n", TmuCap->CapCs3));
    DEBUG ((CAP_VERBOSE, "    LANE TMU Cap CS4 = 0x%08X\n", TmuCap->CapCs4));
    DEBUG ((CAP_VERBOSE, "    LANE TMU Cap CS5 = 0x%08X\n", TmuCap->CapCs5));
    DEBUG ((CAP_VERBOSE, "    LANE TMU Cap CS6 = 0x%08X\n", TmuCap->CapCs6));
    DEBUG ((CAP_VERBOSE, "    LANE TMU Cap CS7 = 0x%08X\n", TmuCap->CapCs7));
  }
}

/**
  Dump Lane adapter capabilities.

  @param[in] AdpCap - Pointer to Lane adapter capabilities.
**/
VOID
DumpLaneCap (
  IN USB4_ADP_CAP_LANE    *AdpCap
  )
{
  if (AdpCap != NULL) {
    DEBUG ((DEBUG_INFO, "    LANE Adapter Cap CS0 = 0x%08X\n", AdpCap->CapCs0));
    DEBUG ((DEBUG_INFO, "        Supported Link Speed = 0x%x\n", AdpCap->CapCs0.Fields.SupportedLinkSpeeds));
    DEBUG ((DEBUG_INFO, "        Supported Link Width = 0x%x\n", AdpCap->CapCs0.Fields.SupportedLinkWidths));
    DEBUG ((DEBUG_INFO, "        CL0s Support = %d\n", AdpCap->CapCs0.Fields.Cl0sSupport));
    DEBUG ((DEBUG_INFO, "        CL1 Support = %d\n", AdpCap->CapCs0.Fields.Cl1Support));
    DEBUG ((DEBUG_INFO, "        CL2 Support = %d\n", AdpCap->CapCs0.Fields.Cl2Support));
    DEBUG ((DEBUG_INFO, "    LANE Adapter Cap CS1 = 0x%08X\n", AdpCap->CapCs1));
    DEBUG ((DEBUG_INFO, "        Target Link Speed = 0x%x\n", AdpCap->CapCs1.Fields.LinkSpeedTarget));
    DEBUG ((DEBUG_INFO, "        Target Link Widths = 0x%x\n", AdpCap->CapCs1.Fields.LinkWidthTarget));
    DEBUG ((CAP_VERBOSE, "        CL0s Enable = %d\n", AdpCap->CapCs1.Fields.Cl0sEnable));
    DEBUG ((CAP_VERBOSE, "        CL1 Enable = %d\n", AdpCap->CapCs1.Fields.Cl1Enable));
    DEBUG ((CAP_VERBOSE, "        CL2 Enable = %d\n", AdpCap->CapCs1.Fields.Cl2Enable));
    DEBUG ((DEBUG_INFO, "        Lane Disable = %d\n", AdpCap->CapCs1.Fields.LaneDisable));
    DEBUG ((DEBUG_INFO, "        Lane Bonding = %d\n", AdpCap->CapCs1.Fields.LaneBonding));
    DEBUG ((DEBUG_INFO, "        Current Link Speed = 0x%x\n", AdpCap->CapCs1.Fields.CurLinkSpeed));
    DEBUG ((DEBUG_INFO, "        Negotiated Link Width = 0x%x\n", AdpCap->CapCs1.Fields.CurLinkWidth));
    DEBUG ((DEBUG_INFO, "        Adapter State = 0x%0x\n", AdpCap->CapCs1.Fields.AdapterState));
    DEBUG ((CAP_VERBOSE, "        PM Secondary = %d\n", AdpCap->CapCs1.Fields.PmSecondary));
    DEBUG ((DEBUG_INFO, "    LANE Adapter Cap CS2 = 0x%08X\n", AdpCap->CapCs2));
    DEBUG ((CAP_VERBOSE, "        Logical Layer Errors = 0x%0x\n", AdpCap->CapCs2.Fields.LogicalLayerErr));
    DEBUG ((CAP_VERBOSE, "        Logical Layer Errors Enable = 0x%0x\n", AdpCap->CapCs2.Fields.LogicalLayerErrEn));
  }
}

/**
  Dump DP-IN adapter capability.

  @param[in] DpInCap - Pointer to DP-IN adapter capability.
**/
VOID
DumpAdpDpInCap (
  IN USB4_ADP_CAP_DP_IN    *DpInCap
  )
{
  if (DpInCap != NULL) {
    DEBUG ((DEBUG_INFO, "    DP IN Adapter Cap CS0 = 0x%08X\n", DpInCap->CapCs0));
    DEBUG ((DEBUG_INFO, "        Video HopID = %d\n", DpInCap->CapCs0.Fields.VideoHopId));
    DEBUG ((DEBUG_INFO, "    DP IN Adapter Cap CS1 = 0x%08X\n", DpInCap->CapCs1));
    DEBUG ((DEBUG_INFO, "        AUX Tx HopId = %d\n", DpInCap->CapCs1.Fields.AuxTxHopId));
    DEBUG ((DEBUG_INFO, "        AUX Rx HopId = %d\n", DpInCap->CapCs1.Fields.AuxRxHopId));
    DEBUG ((DEBUG_INFO, "    DP IN Adapter Cap CS2 = 0x%08X\n", DpInCap->CapCs2));
    DEBUG ((DEBUG_INFO, "        HPD = %d\n", DpInCap->CapCs2.Fields.Hpd));
    DEBUG ((DEBUG_INFO, "    DP IN Adapter Cap CS3 = 0x%08X\n", DpInCap->CapCs3));
    DEBUG ((DEBUG_INFO, "    DP IN Adapter Cap CS4 = 0x%08X (LOCAL_CAP)\n", DpInCap->CapCs4));
    DEBUG ((DEBUG_INFO, "        Protocol Adapter version = %d\n", DpInCap->CapCs4.Fields.ProtocolAdpRev));
    DEBUG ((DEBUG_INFO, "        Maximal DPCD Rev = %d\n", DpInCap->CapCs4.Fields.MaxDpcdRev));
    DEBUG ((DEBUG_INFO, "        Maximal Link Rate = %d\n", DpInCap->CapCs4.Fields.MaxLinkRate));
    DEBUG ((DEBUG_INFO, "        Maximal Lane Count = %d\n", DpInCap->CapCs4.Fields.MaxLaneCnt));
    DEBUG ((DEBUG_INFO, "        MST Capability support = %d\n", DpInCap->CapCs4.Fields.MstCapSupport));
    DEBUG ((DEBUG_INFO, "        TPS3 support = %d\n", DpInCap->CapCs4.Fields.Tps3CapSupport));
    DEBUG ((DEBUG_INFO, "        TPS4 support = %d\n", DpInCap->CapCs4.Fields.Tps4CapSupport));
    DEBUG ((DEBUG_INFO, "        FEC not support = %d\n", DpInCap->CapCs4.Fields.FecNotSupport));
    DEBUG ((DEBUG_INFO, "        Secondary split support = %d\n", DpInCap->CapCs4.Fields.SecSplitCapSupport));
    DEBUG ((DEBUG_INFO, "        LTTPR not support = %d\n", DpInCap->CapCs4.Fields.LttprNotSupport));
    DEBUG ((DEBUG_INFO, "        DSC not support = %d\n", DpInCap->CapCs4.Fields.DscNotSupport));
    DEBUG ((DEBUG_INFO, "    DP IN Adapter Cap CS5 = 0x%08X (REMOTE_CAP)\n", DpInCap->CapCs5));
    DEBUG ((DEBUG_INFO, "        Protocol Adapter version = %d\n", DpInCap->CapCs5.Fields.ProtocolAdpRev));
    DEBUG ((DEBUG_INFO, "        Maximal DPCD Rev = %d\n", DpInCap->CapCs5.Fields.MaxDpcdRev));
    DEBUG ((DEBUG_INFO, "        Maximal Link Rate = %d\n", DpInCap->CapCs5.Fields.MaxLinkRate));
    DEBUG ((DEBUG_INFO, "        Maximal Lane Count = %d\n", DpInCap->CapCs5.Fields.MaxLaneCnt));
    DEBUG ((DEBUG_INFO, "        MST Capability support = %d\n", DpInCap->CapCs5.Fields.MstCapSupport));
    DEBUG ((DEBUG_INFO, "        TPS3 support = %d\n", DpInCap->CapCs5.Fields.Tps3CapSupport));
    DEBUG ((DEBUG_INFO, "        TPS4 support = %d\n", DpInCap->CapCs5.Fields.Tps4CapSupport));
    DEBUG ((DEBUG_INFO, "        FEC not support = %d\n", DpInCap->CapCs5.Fields.FecNotSupport));
    DEBUG ((DEBUG_INFO, "        Secondary split support = %d\n", DpInCap->CapCs5.Fields.SecSplitCapSupport));
    DEBUG ((DEBUG_INFO, "        LTTPR not support = %d\n", DpInCap->CapCs5.Fields.LttprNotSupport));
    DEBUG ((DEBUG_INFO, "        DSC not support = %d\n", DpInCap->CapCs5.Fields.DscNotSupport));
    DEBUG ((DEBUG_INFO, "    DP IN Adapter Cap CS6 = 0x%08X (DP_STATUS)\n", DpInCap->CapCs6));
    DEBUG ((DEBUG_INFO, "        Lane count = %d\n", DpInCap->CapCs6.Fields.LaneCnt));
    DEBUG ((DEBUG_INFO, "        Link rate = %d\n", DpInCap->CapCs6.Fields.LinkRate));
    DEBUG ((DEBUG_INFO, "    DP IN Adapter Cap CS7 = 0x%08X\n", DpInCap->CapCs7));
    DEBUG ((DEBUG_INFO, "        Protocol Adapter version = %d\n", DpInCap->CapCs7.Fields.ProtocolAdpRev));
    DEBUG ((DEBUG_INFO, "        Maximal DPCD Rev = %d\n", DpInCap->CapCs7.Fields.MaxDpcdRev));
    DEBUG ((DEBUG_INFO, "        Maximal Link Rate = %d\n", DpInCap->CapCs7.Fields.MaxLinkRate));
    DEBUG ((DEBUG_INFO, "        Maximal Lane Count = %d\n", DpInCap->CapCs7.Fields.MaxLaneCnt));
    DEBUG ((DEBUG_INFO, "        MST Capability support = %d\n", DpInCap->CapCs7.Fields.MstCapSupport));
    DEBUG ((DEBUG_INFO, "        TPS3 support = %d\n", DpInCap->CapCs7.Fields.Tps3CapSupport));
    DEBUG ((DEBUG_INFO, "        TPS4 support = %d\n", DpInCap->CapCs7.Fields.Tps4CapSupport));
    DEBUG ((DEBUG_INFO, "        FEC not support = %d\n", DpInCap->CapCs7.Fields.FecNotSupport));
    DEBUG ((DEBUG_INFO, "        Secondary split support = %d\n", DpInCap->CapCs7.Fields.SecSplitCapSupport));
    DEBUG ((DEBUG_INFO, "        LTTPR not support = %d\n", DpInCap->CapCs7.Fields.LttprNotSupport));
    DEBUG ((DEBUG_INFO, "        DSC not support = %d\n", DpInCap->CapCs7.Fields.DscNotSupport));
    DEBUG ((DEBUG_INFO, "        DPRX Capabilities Read Done = %d\n", DpInCap->CapCs7.Fields.DprxCapReadDone));
  }
}

/**
  Dump DP-OUT adapter capability.

  @param[in] DpOutCap - Pointer to DP-OUT adapter capability.
**/
VOID
DumpAdpDpOutCap (
  IN USB4_ADP_CAP_DP_OUT    *DpOutCap
  )
{
  if (DpOutCap != NULL) {
    DEBUG ((DEBUG_INFO, "    DP OUT Adapter Cap CS0 = 0x%08X\n", DpOutCap->CapCs0));
    DEBUG ((DEBUG_INFO, "        Video HopID = %d\n", DpOutCap->CapCs0.Fields.VideoHopId));
    DEBUG ((DEBUG_INFO, "    DP OUT Adapter Cap CS1 = 0x%08X\n", DpOutCap->CapCs1));
    DEBUG ((DEBUG_INFO, "        AUX Tx HopId = %d\n", DpOutCap->CapCs1.Fields.AuxTxHopId));
    DEBUG ((DEBUG_INFO, "        AUX Rx HopId = %d\n", DpOutCap->CapCs1.Fields.AuxRxHopId));
    DEBUG ((DEBUG_INFO, "    DP OUT Adapter Cap CS2 = 0x%08X\n", DpOutCap->CapCs2));
    DEBUG ((DEBUG_INFO, "        HPD = %d\n", DpOutCap->CapCs2.Fields.Hpd));
    DEBUG ((DEBUG_INFO, "        Max Acc Cycles = %d\n", DpOutCap->CapCs2.Fields.MaxAccCycles));
    DEBUG ((DEBUG_INFO, "    DP OUT Adapter Cap CS3 = 0x%08X\n", DpOutCap->CapCs3));
    DEBUG ((DEBUG_INFO, "    DP OUT Adapter Cap CS4 = 0x%08X\n", DpOutCap->CapCs4));
    DEBUG ((DEBUG_INFO, "        Protocol Adapter version = %d\n", DpOutCap->CapCs4.Fields.ProtocolAdpRev));
    DEBUG ((DEBUG_INFO, "        Maximal DPCD Rev = %d\n", DpOutCap->CapCs4.Fields.MaxDpcdRev));
    DEBUG ((DEBUG_INFO, "        Maximal Link Rate = %d\n", DpOutCap->CapCs4.Fields.MaxLinkRate));
    DEBUG ((DEBUG_INFO, "        Maximal Lane Count = %d\n", DpOutCap->CapCs4.Fields.MaxLaneCnt));
    DEBUG ((DEBUG_INFO, "        MST Capability support = %d\n", DpOutCap->CapCs4.Fields.MstCapSupport));
    DEBUG ((DEBUG_INFO, "        TPS3 support = %d\n", DpOutCap->CapCs4.Fields.Tps3CapSupport));
    DEBUG ((DEBUG_INFO, "        TPS4 support = %d\n", DpOutCap->CapCs4.Fields.Tps4CapSupport));
    DEBUG ((DEBUG_INFO, "        FEC not support = %d\n", DpOutCap->CapCs4.Fields.FecNotSupport));
    DEBUG ((DEBUG_INFO, "        Secondary split support = %d\n", DpOutCap->CapCs4.Fields.SecSplitCapSupport));
    DEBUG ((DEBUG_INFO, "        LTTPR not support = %d\n", DpOutCap->CapCs4.Fields.LttprNotSupport));
    DEBUG ((DEBUG_INFO, "        DSC not support = %d\n", DpOutCap->CapCs4.Fields.DscNotSupport));
    DEBUG ((DEBUG_INFO, "    DP OUT Adapter Cap CS5 = 0x%08X\n", DpOutCap->CapCs5));
    DEBUG ((DEBUG_INFO, "        Protocol Adapter version = %d\n", DpOutCap->CapCs5.Fields.ProtocolAdpRev));
    DEBUG ((DEBUG_INFO, "        Maximal DPCD Rev = %d\n", DpOutCap->CapCs5.Fields.MaxDpcdRev));
    DEBUG ((DEBUG_INFO, "        Maximal Link Rate = %d\n", DpOutCap->CapCs5.Fields.MaxLinkRate));
    DEBUG ((DEBUG_INFO, "        Maximal Lane Count = %d\n", DpOutCap->CapCs5.Fields.MaxLaneCnt));
    DEBUG ((DEBUG_INFO, "        MST Capability support = %d\n", DpOutCap->CapCs5.Fields.MstCapSupport));
    DEBUG ((DEBUG_INFO, "        TPS3 support = %d\n", DpOutCap->CapCs5.Fields.Tps3CapSupport));
    DEBUG ((DEBUG_INFO, "        TPS4 support = %d\n", DpOutCap->CapCs5.Fields.Tps4CapSupport));
    DEBUG ((DEBUG_INFO, "        FEC not support = %d\n", DpOutCap->CapCs5.Fields.FecNotSupport));
    DEBUG ((DEBUG_INFO, "        Secondary split support = %d\n", DpOutCap->CapCs5.Fields.SecSplitCapSupport));
    DEBUG ((DEBUG_INFO, "        LTTPR not support = %d\n", DpOutCap->CapCs5.Fields.LttprNotSupport));
    DEBUG ((DEBUG_INFO, "        DSC not support = %d\n", DpOutCap->CapCs5.Fields.DscNotSupport));
    DEBUG ((DEBUG_INFO, "    DP OUT Adapter Cap CS6 = 0x%08X\n", DpOutCap->CapCs6));
    DEBUG ((DEBUG_INFO, "        Lane count = %d\n", DpOutCap->CapCs6.Fields.LaneCnt));
    DEBUG ((DEBUG_INFO, "        Link rate = %d\n", DpOutCap->CapCs6.Fields.LinkRate));
    DEBUG ((DEBUG_INFO, "        CM Handshake (CMHS) = %d\n", DpOutCap->CapCs6.Fields.CmHandshake));
    DEBUG ((DEBUG_INFO, "        DP-IN Adapter USB4 Flag = %d\n", DpOutCap->CapCs6.Fields.DpInUsb4Flag));
    DEBUG ((DEBUG_INFO, "    DP OUT Adapter Cap CS7 = 0x%08X\n", DpOutCap->CapCs7));
    DEBUG ((DEBUG_INFO, "        Protocol Adapter version = %d\n", DpOutCap->CapCs7.Fields.ProtocolAdpRev));
    DEBUG ((DEBUG_INFO, "        Maximal DPCD Rev = %d\n", DpOutCap->CapCs7.Fields.MaxDpcdRev));
    DEBUG ((DEBUG_INFO, "        Maximal Link Rate = %d\n", DpOutCap->CapCs7.Fields.MaxLinkRate));
    DEBUG ((DEBUG_INFO, "        Maximal Lane Count = %d\n", DpOutCap->CapCs7.Fields.MaxLaneCnt));
    DEBUG ((DEBUG_INFO, "        MST Capability support = %d\n", DpOutCap->CapCs7.Fields.MstCapSupport));
    DEBUG ((DEBUG_INFO, "        TPS3 support = %d\n", DpOutCap->CapCs7.Fields.Tps3CapSupport));
    DEBUG ((DEBUG_INFO, "        TPS4 support = %d\n", DpOutCap->CapCs7.Fields.Tps4CapSupport));
    DEBUG ((DEBUG_INFO, "        FEC not support = %d\n", DpOutCap->CapCs7.Fields.FecNotSupport));
    DEBUG ((DEBUG_INFO, "        Secondary split support = %d\n", DpOutCap->CapCs7.Fields.SecSplitCapSupport));
    DEBUG ((DEBUG_INFO, "        LTTPR not support = %d\n", DpOutCap->CapCs7.Fields.LttprNotSupport));
    DEBUG ((DEBUG_INFO, "        DSC not support = %d\n", DpOutCap->CapCs7.Fields.DscNotSupport));
  }
}

/**
  Dump USB3 adapter capabilities

  @param[in] AdpCap - Pointer to USB3 adapter capabilities.
**/
VOID
DumpAdpUsb3Cap (
  IN USB4_ADP_CAP_USB3    *AdpCap
  )
{
  if (AdpCap != NULL) {
    DEBUG ((CAP_VERBOSE, "    USB3 Adapter Cap CS0 = 0x%08X\n", AdpCap->CapCs0));
    DEBUG ((CAP_VERBOSE, "    USB3 Adapter Cap CS1 = 0x%08X\n", AdpCap->CapCs1));
    DEBUG ((CAP_VERBOSE, "    USB3 Adapter Cap CS2 = 0x%08X\n", AdpCap->CapCs2));
    //
    // Comment out due to ITBT doesn't support it, not sure if new iTBT FW supports it
    //
    /*
    DEBUG ((CAP_VERBOSE, "    USB3 Adapter Cap CS3 = 0x%08X\n", AdpCap->CapCs3));
    DEBUG ((CAP_VERBOSE, "    USB3 Adapter Cap CS4 = 0x%08X\n", AdpCap->CapCs4));
    */
  }
}

/**
  Dump PCIe adapter capabilities

  @param[in] AdpCap - Pointer to PCIe adapter capabilities.
**/
VOID
DumpAdpPcieCap (
  IN USB4_ADP_CAP_PCIE    *AdpCap
  )
{
  if (AdpCap != NULL) {
    DEBUG ((DEBUG_INFO, "    PCI-E Adapter Cap CS0 = 0x%08X\n", AdpCap->CapCs0.Value));
    DEBUG ((DEBUG_INFO, "        Link = %d\n", AdpCap->CapCs0.Fields.Link));
    DEBUG ((DEBUG_INFO, "        TX Electrical Idle = %d\n", AdpCap->CapCs0.Fields.TxIdle));
    DEBUG ((DEBUG_INFO, "        RX Electrical Idle = %d\n", AdpCap->CapCs0.Fields.RxIdle));
    DEBUG ((DEBUG_INFO, "        RST = %d\n", AdpCap->CapCs0.Fields.Reset));
    DEBUG ((DEBUG_INFO, "        LTSSM = 0x%x\n", AdpCap->CapCs0.Fields.LtssmState));
    DEBUG ((DEBUG_INFO, "        Path Enable = %d\n", AdpCap->CapCs0.Fields.PathEnable));
  }
}

/**
  Dump Lane adapter capabilities CS DWORD1

  @param[in] AdpCap - Pointer to Lane adapter capabilities CS DWORD1.
**/
VOID
DumpLaneCapCs1 (
  IN USB4_ADP_CAP_LANE_CS_1    *AdpCapCs1
  )
{
  if (AdpCapCs1 != NULL) {
    DEBUG ((DEBUG_INFO, "    LANE Adapter Cap CS1 = 0x%08X\n", AdpCapCs1->Value));
    DEBUG ((DEBUG_INFO, "        Target Link Speed = 0x%x\n", AdpCapCs1->Fields.LinkSpeedTarget));
    DEBUG ((DEBUG_INFO, "        Target Link Widths = 0x%x\n", AdpCapCs1->Fields.LinkWidthTarget));
    DEBUG ((CAP_VERBOSE, "        CL0s Enable = %d\n", AdpCapCs1->Fields.Cl0sEnable));
    DEBUG ((CAP_VERBOSE, "        CL1 Enable = %d\n", AdpCapCs1->Fields.Cl1Enable));
    DEBUG ((CAP_VERBOSE, "        CL2 Enable = %d\n", AdpCapCs1->Fields.Cl2Enable));
    DEBUG ((DEBUG_INFO, "        Lane Disable = %d\n", AdpCapCs1->Fields.LaneDisable));
    DEBUG ((DEBUG_INFO, "        Lane Bonding = %d\n", AdpCapCs1->Fields.LaneBonding));
    DEBUG ((DEBUG_INFO, "        Current Link Speed = 0x%x\n", AdpCapCs1->Fields.CurLinkSpeed));
    DEBUG ((DEBUG_INFO, "        Negotiated Link Width = 0x%x\n", AdpCapCs1->Fields.CurLinkWidth));
    DEBUG ((DEBUG_INFO, "        Adapter State = 0x%0x\n", AdpCapCs1->Fields.AdapterState));
    DEBUG ((CAP_VERBOSE, "        PM Secondary = %d\n", AdpCapCs1->Fields.PmSecondary));
  }
}

/**
  Dump path entry

  @param[in] PathEntry - Pointer to the path entry
**/
VOID
DumpPathEntry (
  IN USB4_PATH_CS_ENTRY    *PathEntry
  )
{
  DEBUG ((PATH_VERBOSE, "Path CS0 = 0x%08X\n", PathEntry->PathCs0.Value));
  DEBUG ((PATH_VERBOSE, "   Output HopId = %d\n", PathEntry->PathCs0.Fields.OutputHopId));
  DEBUG ((PATH_VERBOSE, "   Output Adapter = %d\n", PathEntry->PathCs0.Fields.OutputAdapter));
  DEBUG ((PATH_VERBOSE, "   Credits Allocated = %d\n", PathEntry->PathCs0.Fields.CreditsAllocated));
  DEBUG ((PATH_VERBOSE, "   Valid = %d\n", PathEntry->PathCs0.Fields.Valid));
  DEBUG ((PATH_VERBOSE, "Path CS1 = 0x%08X\n", PathEntry->PathCs1.Value));
  DEBUG ((PATH_VERBOSE, "   Weight = %d\n", PathEntry->PathCs1.Fields.Weight));
  DEBUG ((PATH_VERBOSE, "   Priority = %d\n", PathEntry->PathCs1.Fields.Priority));
  DEBUG ((PATH_VERBOSE, "   Counter ID = %d\n", PathEntry->PathCs1.Fields.CounterId));
  DEBUG ((PATH_VERBOSE, "   Counter Enable = %d\n", PathEntry->PathCs1.Fields.CounterEn));
  DEBUG ((PATH_VERBOSE, "   Ingress Flow Control (Ifc) = %d\n", PathEntry->PathCs1.Fields.Ifc));
  DEBUG ((PATH_VERBOSE, "   Egress Flow Control (Efc) = %d\n", PathEntry->PathCs1.Fields.Efc));
  DEBUG ((PATH_VERBOSE, "   Ingress Shared Buffering Enable (Ise) = %d\n", PathEntry->PathCs1.Fields.Ise));
  DEBUG ((PATH_VERBOSE, "   Egress Shared Buffering Enable (Ese) = %d\n", PathEntry->PathCs1.Fields.Ese));
  DEBUG ((PATH_VERBOSE, "   Pending Packets = %d\n", PathEntry->PathCs1.Fields.PendingPkts));
  DEBUG ((PATH_VERBOSE, "   Path BlockLow = %d\n", PathEntry->PathCs1.Fields.BlockLow));
}