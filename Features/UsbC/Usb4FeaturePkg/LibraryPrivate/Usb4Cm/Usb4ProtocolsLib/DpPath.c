/** @file
  USB4 path configuration for DisplayPort tunneling.

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
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/CmUtilsLib.h>
#include <Library/Usb4CsLib.h>
#include <Library/Usb4DomainLib.h>
#include <Library/Usb4RouterLib.h>
#include <Library/Usb4Common.h>
#include <Library/Usb4RouterDefs.h>
#include <Library/Usb4Capabilities.h>
#include "ProtocolConfig.h"
#include "Usb4Protocols.h"

/**
  Config DP path entry according to path type

  @param[out] PathEntry   - Pointer to path entry.
  @param[in]  Egress      - Egress adapter number of packet routing.
  @param[in]  EgressHopId - Egress hop id of packet routing.
  @param[in]  DpPathType  - DP path type.
**/
VOID
ConfigureDpPathEntry (
  OUT PUSB4_PATH_CS_ENTRY    PathEntry,
  IN UINT16                  Egress,
  IN UINT16                  EgressHopId,
  IN DP_PATH_TYPE            DpPathType
  )
{
  if (PathEntry == NULL) {
    return;
  }

  PathEntry->PathCs0.Fields.OutputAdapter    = (UINT32) Egress;
  PathEntry->PathCs0.Fields.OutputHopId      = (UINT32) EgressHopId;
  PathEntry->PathCs0.Fields.Valid            = 1;

  //
  // Based on the Connection Manger guide, flow control mechanism is applied to lane adapter only
  // IFC/ISE are set if the ingress adapter is Lane adapter
  // EFC/ESE are set if the egress adapter is Lane Adapter
  //
  // Dedicated flow control is applied to AUX path
  // - IFC/EFC = 1, ISE/ESE = 0
  // - Buffer size = CreditAllocated in Path CS0
  // Disabled flow control is applied to MAIN path
  // - IFC/EFC = 0, ISE/ESE = 0
  // - Buffer size = NFC buffers in Router CS4
  //
  // Looks like flow control/credit allocated/flow control settings are still required for DP-IN adapter
  // DP Tunneling doesn't work if flow control settings are set for DP-IN adapter
  //
  switch (DpPathType) {
    case DPIN_TO_LANE_MAIN:
      DEBUG ((PATH_VERBOSE, "Configure Main Link Path DP-IN to Lane\n"));
      PathEntry->PathCs1.Fields.Priority         = DPIN_TO_LANE_PATH_MAIN_PRIORITY;
      PathEntry->PathCs1.Fields.Weight           = DPIN_TO_LANE_PATH_MAIN_WEIGHT;
      PathEntry->PathCs1.Fields.Efc              = DPIN_TO_LANE_PATH_MAIN_EFC;
      PathEntry->PathCs1.Fields.Ese              = DPIN_TO_LANE_PATH_MAIN_ESE;
      //
      // The following fields should be N/A for DP-IN to Lane based on Connection Manager Guide
      //
      PathEntry->PathCs0.Fields.CreditsAllocated = DPIN_TO_LANE_PATH_MAIN_CREDITS_ALLOCATED;
      PathEntry->PathCs1.Fields.Ifc              = DPIN_TO_LANE_PATH_MAIN_IFC;
      PathEntry->PathCs1.Fields.Ise              = DPIN_TO_LANE_PATH_MAIN_ISE;
      break;
    case DPIN_TO_LANE_AUX:
      DEBUG ((PATH_VERBOSE, "Configure Aux Path DP-IN to Lane\n"));
      PathEntry->PathCs1.Fields.Priority         = DPIN_TO_LANE_PATH_AUX_PRIORITY;
      PathEntry->PathCs1.Fields.Weight           = DPIN_TO_LANE_PATH_AUX_WEIGHT;
      PathEntry->PathCs1.Fields.Efc              = DPIN_TO_LANE_PATH_AUX_EFC;
      PathEntry->PathCs1.Fields.Ese              = DPIN_TO_LANE_PATH_AUX_ESE;
      //
      // The following fields should be N/A for DP-IN to Lane
      //
      PathEntry->PathCs0.Fields.CreditsAllocated = DPIN_TO_LANE_PATH_AUX_CREDITS_ALLOCATED;
      PathEntry->PathCs1.Fields.Ifc              = DPIN_TO_LANE_PATH_AUX_IFC;
      PathEntry->PathCs1.Fields.Ise              = DPIN_TO_LANE_PATH_AUX_ISE;
      break;
    case LANE_TO_DPIN_AUX:
      DEBUG ((PATH_VERBOSE, "Configure Aux Path Lane to DP-IN\n"));
      PathEntry->PathCs1.Fields.Priority         = LANE_TO_DPIN_PATH_AUX_PRIORITY;
      PathEntry->PathCs1.Fields.Weight           = LANE_TO_DPIN_PATH_AUX_WEIGHT;
      PathEntry->PathCs0.Fields.CreditsAllocated = LANE_TO_DPIN_PATH_AUX_CREDITS_ALLOCATED;
      PathEntry->PathCs1.Fields.Ifc              = LANE_TO_DPIN_PATH_AUX_IFC;
      PathEntry->PathCs1.Fields.Ise              = LANE_TO_DPIN_PATH_AUX_ISE;
      //
      // The following fields should be N/A for Lane to DP-IN
      //
      PathEntry->PathCs1.Fields.Efc              = LANE_TO_DPIN_PATH_AUX_EFC;
      PathEntry->PathCs1.Fields.Ese              = LANE_TO_DPIN_PATH_AUX_ESE;
      break;
    case LANE_TO_DPOUT_MAIN:
      DEBUG ((PATH_VERBOSE, "Configure Main Link Path Lane to DP-OUT\n"));
      PathEntry->PathCs1.Fields.Priority         = LANE_TO_DPOUT_PATH_MAIN_PRIORITY;
      PathEntry->PathCs1.Fields.Weight           = LANE_TO_DPOUT_PATH_MAIN_WEIGHT;
      PathEntry->PathCs0.Fields.CreditsAllocated = LANE_TO_DPOUT_PATH_MAIN_CREDITS_ALLOCATED;
      PathEntry->PathCs1.Fields.Ifc              = LANE_TO_DPOUT_PATH_MAIN_IFC;
      PathEntry->PathCs1.Fields.Ise              = LANE_TO_DPOUT_PATH_MAIN_ISE;
      //
      // The following fields should be N/A for Lane to DP-OUT
      //
      PathEntry->PathCs1.Fields.Efc              = LANE_TO_DPOUT_PATH_MAIN_EFC;
      PathEntry->PathCs1.Fields.Ese              = LANE_TO_DPOUT_PATH_MAIN_ESE;
      break;
    case LANE_TO_DPOUT_AUX:
      DEBUG ((PATH_VERBOSE, "Configure Aux Path Lane to DP-OUT\n"));
      PathEntry->PathCs1.Fields.Priority         = LANE_TO_DPOUT_PATH_AUX_PRIORITY;
      PathEntry->PathCs1.Fields.Weight           = LANE_TO_DPOUT_PATH_AUX_WEIGHT;
      PathEntry->PathCs0.Fields.CreditsAllocated = LANE_TO_DPOUT_PATH_AUX_CREDITS_ALLOCATED;
      PathEntry->PathCs1.Fields.Ifc              = LANE_TO_DPOUT_PATH_AUX_IFC;
      PathEntry->PathCs1.Fields.Ise              = LANE_TO_DPOUT_PATH_AUX_ISE;
      //
      // The following fields should be N/A for Lane to DP-OUT
      //
      PathEntry->PathCs1.Fields.Efc              = LANE_TO_DPOUT_PATH_AUX_EFC;
      PathEntry->PathCs1.Fields.Ese              = LANE_TO_DPOUT_PATH_AUX_ESE;
      break;
    case DPOUT_TO_LANE_AUX:
      DEBUG ((PATH_VERBOSE, "Configure Aux Path DP-OUT to Lane\n"));
      PathEntry->PathCs1.Fields.Priority         = DPOUT_TO_LANE_PATH_AUX_PRIORITY;
      PathEntry->PathCs1.Fields.Weight           = DPOUT_TO_LANE_PATH_AUX_WEIGHT;
      PathEntry->PathCs1.Fields.Efc              = DPOUT_TO_LANE_PATH_AUX_EFC;
      PathEntry->PathCs1.Fields.Ese              = DPOUT_TO_LANE_PATH_AUX_ESE;
      //
      // The following fields should be N/A for DP-IN to Lane
      //
      PathEntry->PathCs0.Fields.CreditsAllocated = DPOUT_TO_LANE_PATH_AUX_CREDITS_ALLOCATED;
      PathEntry->PathCs1.Fields.Ifc              = DPOUT_TO_LANE_PATH_AUX_IFC;
      PathEntry->PathCs1.Fields.Ise              = DPOUT_TO_LANE_PATH_AUX_ISE;
      break;
    case DP_LANE_TO_LANE_MAIN:
      DEBUG ((PATH_VERBOSE, "Configure Main Link Path Lane to Lane\n"));
      PathEntry->PathCs1.Fields.Priority         = LANE_TO_LANE_PATH_MAIN_PRIORITY;
      PathEntry->PathCs1.Fields.Weight           = LANE_TO_LANE_PATH_MAIN_WEIGHT;
      PathEntry->PathCs0.Fields.CreditsAllocated = LANE_TO_LANE_PATH_MAIN_CREDITS_ALLOCATED;
      PathEntry->PathCs1.Fields.Ifc              = LANE_TO_LANE_PATH_MAIN_IFC;
      PathEntry->PathCs1.Fields.Ise              = LANE_TO_LANE_PATH_MAIN_ISE;
      PathEntry->PathCs1.Fields.Efc              = LANE_TO_LANE_PATH_MAIN_EFC;
      PathEntry->PathCs1.Fields.Ese              = LANE_TO_LANE_PATH_MAIN_ESE;
      break;
    case DP_LANE_TO_LANE_AUX:
      DEBUG ((PATH_VERBOSE, "Configure Aux Path Lane to Lane\n"));
      PathEntry->PathCs1.Fields.Priority         = LANE_TO_LANE_PATH_AUX_PRIORITY;
      PathEntry->PathCs1.Fields.Weight           = LANE_TO_LANE_PATH_AUX_WEIGHT;
      PathEntry->PathCs0.Fields.CreditsAllocated = LANE_TO_LANE_PATH_AUX_CREDITS_ALLOCATED;
      PathEntry->PathCs1.Fields.Ifc              = LANE_TO_LANE_PATH_AUX_IFC;
      PathEntry->PathCs1.Fields.Ise              = LANE_TO_LANE_PATH_AUX_ISE;
      PathEntry->PathCs1.Fields.Efc              = LANE_TO_LANE_PATH_AUX_EFC;
      PathEntry->PathCs1.Fields.Ese              = LANE_TO_LANE_PATH_AUX_ESE;
      break;
  }

  DEBUG ((PATH_VERBOSE, "Output Adapter = %d\n", PathEntry->PathCs0.Fields.OutputAdapter));
  DEBUG ((PATH_VERBOSE, "Output HopId   = %d\n", PathEntry->PathCs0.Fields.OutputHopId));
  DEBUG ((PATH_VERBOSE, "Priority = %d\n", PathEntry->PathCs1.Fields.Priority));
  DEBUG ((PATH_VERBOSE, "Weight   = %d\n", PathEntry->PathCs1.Fields.Weight));
  DEBUG ((PATH_VERBOSE, "Credits  = %d\n", PathEntry->PathCs0.Fields.CreditsAllocated));
  DEBUG ((PATH_VERBOSE, "Ifc      = %d\n", PathEntry->PathCs1.Fields.Ifc));
  DEBUG ((PATH_VERBOSE, "Ise      = %d\n", PathEntry->PathCs1.Fields.Ise));
  DEBUG ((PATH_VERBOSE, "Efc      = %d\n", PathEntry->PathCs1.Fields.Efc));
  DEBUG ((PATH_VERBOSE, "Ese      = %d\n", PathEntry->PathCs1.Fields.Ese));
  //
  // N/A for USB4 spec, need to clarify with TBT team why this should be done
  //
  DEBUG ((DEBUG_INFO, "DP Path BlockLow (Bit29) = %d\n", PathEntry->PathCs1.Fields.BlockLow));
  PathEntry->PathCs1.Fields.BlockLow         = 0;
}


/**
  Set DP Main Link Path segment starting from DP-IN adapter to Lane adapter.

  @param[in] Router           - Pointer to Router instance.
  @param[in] Ingress          - Ingress adapter number.
  @param[in] Egress           - Egress adapter number.
  @param[in] DpPathType       - DP path type.
  @param[in] IngressHopId     - Ingress HopId used in the path.
  @param[in] EgressHopId      - Egress HopId used in the path.

  @retval EFI_SUCCESS           - Set path segment success.
  @retval EFI_UNSUPPORTED       - Fail to set path segment.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
SetDpPathSeg (
  IN PUSB4_ROUTER    Router,
  IN UINT8           Ingress,
  IN UINT8           Egress,
  IN DP_PATH_TYPE    DpPathType,
  IN UINT16          IngressHopId,
  IN UINT16          EgressHopId
  )
{
  EFI_STATUS            Status;
  USB4_PATH_CS_ENTRY    PathEntry;

  DEBUG ((PATH_VERBOSE, "SetDpPathSeg entry - Type=%d\n", DpPathType));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "SetDpPathSeg: Null router!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((Ingress == 0) || (Ingress > Router->MaxAdapter)) {
    DEBUG ((DEBUG_ERROR, "SetDpPathSeg: Invalid Ingress %d\n", Ingress));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((Egress == 0) || (Egress > Router->MaxAdapter)) {
    DEBUG ((DEBUG_ERROR, "SetDpPathSeg: Invalid Egress %d\n", Egress));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DEBUG ((PATH_VERBOSE, "RT 0x%016llX In=%d, Out=%d, InHopId=%d, OutHopId=%d\n", Router->TopologyId, Ingress, Egress, IngressHopId, EgressHopId));

  //
  // Query the path entry to be updated.
  //
  Status = Usb4CsPathRead (Router, Ingress, IngressHopId, &PathEntry);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Read path cs (Adapter=%d, HopId=%d) failure, %r\n", Ingress, IngressHopId, Status));
    goto Exit;
  }

  //
  // Check the original path entry values - only for initial development phase
  //
  DEBUG ((PATH_VERBOSE, "Original path entry\n"));
  DumpPathEntry (&PathEntry);

  //
  // Configure the path entry based on the path type
  //
  ConfigureDpPathEntry (&PathEntry, Egress, EgressHopId, DpPathType);

  //
  // Update the path entry configuration space
  //
  Status = Usb4CsPathWrite (Router, Ingress, IngressHopId, &PathEntry);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Write path cs (Adapter=%d, HopId=%d) failure, %r\n", Ingress, IngressHopId, Status));
    goto Exit;
  }

  //
  // only for initial development phase
  //
  Status = Usb4CsPathRead (Router, Ingress, IngressHopId, &PathEntry);
  DEBUG ((PATH_VERBOSE, "Updated path entry\n"));
  DumpPathEntry (&PathEntry);
Exit:
  DEBUG ((PATH_VERBOSE, "SetDpPathSeg exit\n"));
  return Status;
}

/**
  Set DP MAIN Link path and AUX path from DP Source to DP Sink

  @param[in] DpSrcRt      - Pointer to DP Source Router.
  @param[in] DpSrcAdpNum  - DP-IN adapter number of DP source.
  @param[in] DpSinkRt     - Pointer to DP Sink Router.
  @param[in] DpSinkAdpNum - DP_OUT adapter number of DP sink.
  @param[in] StreamId     - DP stream ID for identifying DP path.

  @retval EFI_SUCCESS           - Setup DP outbond path successfully.
  @retval EFI_UNSUPPORTED       - Fail to setup DP outbond path.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
SetDpOutbondPath (
  IN PUSB4_ROUTER    DpSrcRt,
  IN UINT8           DpSrcAdpNum,
  IN PUSB4_ROUTER    DpSinkRt,
  IN UINT8           DpSinkAdpNum,
  IN UINT32          StreamId
  )
{
  EFI_STATUS      Status;
  PUSB4_ROUTER    CurRt;
  PUSB4_ROUTER    NextRt;
  UINT8           Depth;
  UINT8           DepthMax;
  UINT8           DsAdpNum;
  UINT16          EgressMainHopId;
  UINT16          EgressAuxHopId;
  UINT16          IngressMainHopId;
  UINT16          IngressAuxHopId;

  DEBUG ((PATH_VERBOSE, "SetDpOutbondPath entry - StreamId = %d\n", StreamId));

  if ((DpSrcRt == NULL) || (DpSinkRt == NULL) || (StreamId >= USB4_DP_STREAM_MAX)) {
    DEBUG ((DEBUG_ERROR, "SetDpOutbondPath: Invalid parameter (%p, %p, %d)\n", DpSrcRt, DpSinkRt, StreamId));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Status = EFI_SUCCESS;

  //
  // Set Main Link path and Aux Out path.
  //
  CurRt = DpSrcRt;
  IngressMainHopId = DP_ADAPTER_MAIN_HOPID;
  IngressAuxHopId  = DP_ADAPTER_AUX_HOPID;

  //
  // Depth in for loop traverses from Router connecting DP source to the parent Router of Router connecting DP sink
  // Depth should be in valid range for TopologyId.Adapter array access
  //
  DepthMax = MIN (DpSinkRt->Depth - 1, mUsb4TopologyDepth - 1);
  DepthMax = MIN (DepthMax, TOPOLOGY_ID_ADAPTER_SIZE - 1);

  for (Depth = DpSrcRt->Depth; Depth <= DepthMax; Depth++) {
    //
    // Get the downstream Lane adapter number of each depth from Topology ID of DP Sink Router
    //
    DsAdpNum = DpSinkRt->TopologyId.Adapter[Depth];
    if ((DsAdpNum == 0) || (DsAdpNum > CurRt->MaxAdapter) || (DsAdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
      DEBUG ((DEBUG_ERROR, "Rt 0x%016llX invalid adp %d\n", CurRt->TopologyId, DsAdpNum));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
    if (CurRt->Adapter[DsAdpNum].Type != USB4_ADP_TYPE_LANE) {
      DEBUG ((DEBUG_ERROR, "Rt 0x%016llX adp %d is not Lane adapter\n", CurRt->TopologyId, DsAdpNum));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
    NextRt = CurRt->Adapter[DsAdpNum].AdpDep.Lane.DsRouter;
    if (NextRt == NULL) {
      DEBUG ((DEBUG_ERROR, "SetDpOutbondPath: Null NextRt at depth %d\n", Depth));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
    if ((NextRt->LaneUpAdpNum == 0) || (NextRt->LaneUpAdpNum > NextRt->MaxAdapter) || (NextRt->LaneUpAdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
      DEBUG ((DEBUG_ERROR, "Rt 0x%016llx LaneUpAdpNum %d is invalid\n", NextRt->TopologyId, NextRt->LaneUpAdpNum));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
    if (NextRt->Adapter[NextRt->LaneUpAdpNum].Type != USB4_ADP_TYPE_LANE) {
      DEBUG ((DEBUG_ERROR, "Rt 0x%016llX adp %d is not Lane adapter\n", NextRt->TopologyId, NextRt->LaneUpAdpNum));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }

    //
    // Set Main Link path and Aux Out path for the current Router.
    //
    if (CurRt == DpSrcRt) {
      //
      // Get a new HopID used for AUX-OUT path between current router's downstream Lane adapter and downstream router's upstream Lane adapter.
      //
      DEBUG ((PATH_VERBOSE, "Rt 0x%016llX DPIN %d to Ln %d (AUX-OUT)\n", CurRt->TopologyId, DpSrcAdpNum, DsAdpNum));
      Status = GetAvailableHopId (CurRt, DsAdpNum, NextRt, NextRt->LaneUpAdpNum, &EgressAuxHopId);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Ln %d failed to get new HopID (AUX-OUT), %r\n", CurRt->TopologyId, DsAdpNum, Status));
        goto Exit;
      }
      //
      // Set DP Aux-Out path
      //
      DEBUG ((PATH_VERBOSE, "Ingress HopId = %d, Egress HopId = %d (AUX-OUT)\n", IngressAuxHopId, EgressAuxHopId));
      Status = SetDpPathSeg (CurRt, DpSrcAdpNum, DsAdpNum, DPIN_TO_LANE_AUX, IngressAuxHopId, EgressAuxHopId);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Rt 0x%016llX DPIN %d to Ln %d (AUX-OUT) failed, %r\n", CurRt->TopologyId, DpSrcAdpNum, DsAdpNum, Status));
        goto Exit;
      }
      CurRt->Adapter[DsAdpNum].AdpDep.Lane.EgressDpAuxHopId[StreamId] = EgressAuxHopId;

      //
      // Get a new HopID used for Main Link path between current router's downstream Lane adapter and downstream router's upstream Lane adapter.
      //
      DEBUG ((PATH_VERBOSE, "Rt 0x%016llX DPIN %d to Ln %d (MAIN)\n", CurRt->TopologyId, DpSrcAdpNum, DsAdpNum));
      Status = GetAvailableHopId (CurRt, DsAdpNum, NextRt, NextRt->LaneUpAdpNum, &EgressMainHopId);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Ln %d failed to get new HopID (MAIN), %r\n", CurRt->TopologyId, DsAdpNum, Status));
        goto Exit;
      }
      //
      // Set DP Main link path
      //
      DEBUG ((PATH_VERBOSE, "Ingress HopId = %d, Egress HopId = %d (MAIN)\n", IngressMainHopId, EgressMainHopId));
      Status = SetDpPathSeg (CurRt, DpSrcAdpNum, DsAdpNum, DPIN_TO_LANE_MAIN, IngressMainHopId, EgressMainHopId);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Rt 0x%016llX DPIN %d to Ln %d (MAIN) failed, %r\n", CurRt->TopologyId, DpSrcAdpNum, DsAdpNum, Status));
        goto Exit;
      }
      CurRt->Adapter[DsAdpNum].AdpDep.Lane.EgressDpMainHopId[StreamId] = EgressMainHopId;
    } else {
      if ((CurRt->LaneUpAdpNum == 0) || (CurRt->LaneUpAdpNum > CurRt->MaxAdapter) || (CurRt->LaneUpAdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
        DEBUG ((DEBUG_ERROR, "Rt 0x%016llX invalid adp %d\n", CurRt->TopologyId, CurRt->LaneUpAdpNum));
        Status = EFI_UNSUPPORTED;
        goto Exit;
      }
      //
      // Get a new HopID used for AUX-OUT path between current router's downstream Lane adapter and downstream router's upstream Lane adapter.
      //
      DEBUG ((PATH_VERBOSE, "Rt 0x%016llX Ln %d to Ln %d (AUX-OUT)\n", CurRt->TopologyId, CurRt->LaneUpAdpNum, DsAdpNum));
      IngressAuxHopId = EgressAuxHopId;
      Status = GetAvailableHopId (CurRt, DsAdpNum, NextRt, NextRt->LaneUpAdpNum, &EgressAuxHopId);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Ln %d failed to get new HopID (AUX-OUT), %r\n", CurRt->TopologyId, DsAdpNum, Status));
        goto Exit;
      }
      //
      // Set DP Aux-Out path
      //
      DEBUG ((PATH_VERBOSE, "Ingress HopId = %d, Egress HopId = %d (AUX-OUT)\n", IngressAuxHopId, EgressAuxHopId));
      Status = SetDpPathSeg (CurRt, CurRt->LaneUpAdpNum, DsAdpNum, DP_LANE_TO_LANE_AUX, IngressAuxHopId, EgressAuxHopId);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Ln %d to Ln %d (AUX-OUT) failed, %r\n", CurRt->TopologyId, CurRt->LaneUpAdpNum, DsAdpNum, Status));
        goto Exit;
      }
      CurRt->Adapter[CurRt->LaneUpAdpNum].AdpDep.Lane.IngressDpAuxHopId[StreamId] = IngressAuxHopId;
      CurRt->Adapter[DsAdpNum].AdpDep.Lane.EgressDpAuxHopId[StreamId] = EgressAuxHopId;

      //
      // Get a new HopID used for Main Link path between current router's downstream Lane adapter and downstream router's upstream Lane adapter.
      //
      DEBUG ((PATH_VERBOSE, "Rt 0x%016llX Ln %d to Ln %d (MAIN)\n", CurRt->TopologyId, CurRt->LaneUpAdpNum, DsAdpNum));
      IngressMainHopId = EgressMainHopId;
      Status = GetAvailableHopId (CurRt, DsAdpNum, NextRt, NextRt->LaneUpAdpNum, &EgressMainHopId);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Ln %d failed to get new HopID (MAIN), %r\n", CurRt->TopologyId, DsAdpNum, Status));
        goto Exit;
      }
      //
      // Set DP Main link path
      //
      DEBUG ((PATH_VERBOSE, "Ingress HopId = %d, Egress HopId = %d (MAIN)\n", IngressMainHopId, EgressMainHopId));
      Status = SetDpPathSeg (CurRt, CurRt->LaneUpAdpNum, DsAdpNum, DP_LANE_TO_LANE_MAIN, IngressMainHopId, EgressMainHopId);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Ln %d to Ln %d (MAIN) failed, %r\n", CurRt->TopologyId, CurRt->LaneUpAdpNum, DsAdpNum, Status));
        goto Exit;
      }
      CurRt->Adapter[CurRt->LaneUpAdpNum].AdpDep.Lane.IngressDpMainHopId[StreamId] = IngressMainHopId;
      CurRt->Adapter[DsAdpNum].AdpDep.Lane.EgressDpMainHopId[StreamId] = EgressMainHopId;
    }

    //
    // Set Main Link path and Aux out path for the next Router if the next Router is DP sink Router.
    //
    if (NextRt == DpSinkRt) {
      //
      // Set DP Aux-Out path
      //
      DEBUG ((PATH_VERBOSE, "Rt 0x%016llX Ln %d to DPOUT %d (AUX-OUT)\n", NextRt->TopologyId, NextRt->LaneUpAdpNum, DpSinkAdpNum));
      IngressAuxHopId = EgressAuxHopId;
      EgressAuxHopId  = DP_ADAPTER_AUX_HOPID;
      DEBUG ((PATH_VERBOSE, "Ingress HopId = %d, Egress HopId = %d (AUX-OUT)\n", IngressAuxHopId, EgressAuxHopId));
      Status = SetDpPathSeg (NextRt, NextRt->LaneUpAdpNum, DpSinkAdpNum, LANE_TO_DPOUT_AUX, IngressAuxHopId, EgressAuxHopId);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Ln %d to DPOUT %d (AUX-OUT) failed, %r\n", NextRt->TopologyId, NextRt->LaneUpAdpNum, DpSinkAdpNum, Status));
        goto Exit;
      }
      NextRt->Adapter[NextRt->LaneUpAdpNum].AdpDep.Lane.IngressDpAuxHopId[StreamId] = IngressAuxHopId;

      //
      // Set DP Main link path
      //
      DEBUG ((PATH_VERBOSE, "Rt 0x%016llX Ln %d to DPOUT %d (MAIN)\n", NextRt->TopologyId, NextRt->LaneUpAdpNum, DpSinkAdpNum));
      IngressMainHopId = EgressMainHopId;
      EgressMainHopId  = DP_ADAPTER_MAIN_HOPID;
      DEBUG ((PATH_VERBOSE, "Ingress HopId = %d, Egress HopId = %d (MAIN)\n", IngressMainHopId, EgressMainHopId));
      Status = SetDpPathSeg (NextRt, NextRt->LaneUpAdpNum, DpSinkAdpNum, LANE_TO_DPOUT_MAIN, IngressMainHopId, EgressMainHopId);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Ln %d to DPOUT %d (MAIN) failed, %r\n", NextRt->TopologyId, NextRt->LaneUpAdpNum, DpSinkAdpNum, Status));
        goto Exit;
      }
      NextRt->Adapter[NextRt->LaneUpAdpNum].AdpDep.Lane.IngressDpMainHopId[StreamId] = IngressMainHopId;

      break;
    }

    CurRt = NextRt;
  }
Exit:
  DEBUG ((PATH_VERBOSE, "SetDpOutbondPath exit\n"));
  return Status;
}

/**
  Set DP AUX path from DP Sink to DP Source

  @param[in] DpSrcRt      - Pointer to DP Source Router.
  @param[in] DpSrcAdpNum  - DP-IN adapter number of DP source.
  @param[in] DpSinkRt     - Pointer to DP Sink Router.
  @param[in] DpSinkAdpNum - DP_OUT adapter number of DP sink.
  @param[in] StreamId     - DP stream ID for identifying DP path.

  @retval EFI_SUCCESS           - Setup DP inbond path successfully.
  @retval EFI_UNSUPPORTED       - Setup DP inbond path failure.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
SetDpInbondPath (
  IN PUSB4_ROUTER    DpSrcRt,
  IN UINT8           DpSrcAdpNum,
  IN PUSB4_ROUTER    DpSinkRt,
  IN UINT8           DpSinkAdpNum,
  IN UINT32          StreamId
  )
{
  EFI_STATUS      Status;
  PUSB4_ROUTER    CurRt;
  PUSB4_ROUTER    NextRt;
  UINT8           Depth;
  UINT8           UpAdpNum;
  UINT8           DsAdpNum;
  UINT8           NextAdpNum;
  UINT16          EgressAuxHopId;
  UINT16          IngressAuxHopId;

  DEBUG ((PATH_VERBOSE, "SetDpInbondPath entry - StreamId = %d\n", StreamId));

  if ((DpSrcRt == NULL) || (DpSinkRt == NULL) || (StreamId >= USB4_DP_STREAM_MAX)) {
    DEBUG ((DEBUG_ERROR, "SetDpInbondPath: Invalid parameter (%p, %p, %d)\n", DpSrcRt, DpSinkRt, StreamId));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Status = EFI_SUCCESS;

  //
  // Set Aux In path from DP sink router to DP source router.
  //
  CurRt = DpSinkRt;
  DsAdpNum = DpSinkAdpNum;
  for (Depth = DpSinkRt->Depth; Depth > DpSrcRt->Depth; Depth--) {
    if (CurRt->UpperRtLane == NULL) {
      DEBUG ((DEBUG_ERROR, "RT 0x%016llX NULL upper RT connected lane adapter\n", CurRt->TopologyId));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
    UpAdpNum   = CurRt->LaneUpAdpNum;
    NextAdpNum = CurRt->UpperRtLane->AdpNum;
    NextRt = CurRt->UpperRtLane->Router;
    if (NextRt == NULL) {
      DEBUG ((DEBUG_ERROR, "Null upper router at Rt 0x%016llX\n", CurRt->TopologyId));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
    if ((UpAdpNum == 0) || (UpAdpNum > CurRt->MaxAdapter) || (UpAdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
      DEBUG ((DEBUG_ERROR, "Rt 0x%016llX invalid adp %d\n", CurRt->TopologyId, UpAdpNum));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
    if ((DsAdpNum == 0) || (DsAdpNum > CurRt->MaxAdapter) || (DsAdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
      DEBUG ((DEBUG_ERROR, "Rt 0x%016llX invalid adp %d\n", CurRt->TopologyId, DsAdpNum));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
    if ((NextAdpNum == 0) || (NextAdpNum > NextRt->MaxAdapter) || (NextAdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
      DEBUG ((DEBUG_ERROR, "Rt 0x%016llX invalid adp %d\n", NextRt->TopologyId, NextAdpNum));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
    //
    // Set Aux In path for the current Router.
    //
    if (CurRt == DpSinkRt) {
      //
      // Get a new HopID used for AUX-IN path between current router's upstream Lane adapter and upstream router's downstream Lane adapter.
      //
      DEBUG ((PATH_VERBOSE, "Rt 0x%016llX DPOUT %d to Ln %d (AUX-IN)\n", CurRt->TopologyId, DpSinkAdpNum, UpAdpNum));
      IngressAuxHopId = DP_ADAPTER_AUX_HOPID;
      Status = GetAvailableHopId (CurRt, UpAdpNum, NextRt, NextAdpNum, &EgressAuxHopId);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Ln %d failed to get new HopID (AUX-IN), %r\n", CurRt->TopologyId, UpAdpNum, Status));
        goto Exit;
      }
      //
      // Set DP Aux-In path
      //
      DEBUG ((PATH_VERBOSE, "Ingress HopId = %d, Egress HopId = %d (AUX-IN)\n", IngressAuxHopId, EgressAuxHopId));
      Status = SetDpPathSeg (CurRt, DpSinkAdpNum, UpAdpNum, DPOUT_TO_LANE_AUX, IngressAuxHopId, EgressAuxHopId);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Rt 0x%016llX DPOUT %d to Ln %d (AUX-IN) failed, %r\n", CurRt->TopologyId, DpSinkAdpNum, UpAdpNum, Status));
        goto Exit;
      }
      CurRt->Adapter[UpAdpNum].AdpDep.Lane.EgressDpAuxHopId[StreamId] = EgressAuxHopId;
    } else {
      //
      // Get a new HopID used for AUX-IN path between current router's upstream Lane adapter and upstream router's downstream Lane adapter.
      //
      DEBUG ((PATH_VERBOSE, "Rt 0x%016llX Ln %d to Ln %d (AUX-IN)\n", NextRt->TopologyId, DsAdpNum, UpAdpNum));
      IngressAuxHopId = EgressAuxHopId;
      Status = GetAvailableHopId (CurRt, UpAdpNum, NextRt, NextAdpNum, &EgressAuxHopId);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Ln %d failed to get new HopID (AUX-IN), %r\n", CurRt->TopologyId, UpAdpNum, Status));
        goto Exit;
      }
      //
      // Set DP Aux-In path
      //
      DEBUG ((PATH_VERBOSE, "Ingress HopId = %d, Egress HopId = %d (AUX-IN)\n", IngressAuxHopId, EgressAuxHopId));
      Status = SetDpPathSeg (CurRt, DsAdpNum, UpAdpNum, DP_LANE_TO_LANE_AUX, IngressAuxHopId, EgressAuxHopId);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Ln %d to Ln %d (AUX-IN) failed, %r\n", CurRt->TopologyId, DsAdpNum, UpAdpNum, Status));
        goto Exit;
      }
      CurRt->Adapter[DsAdpNum].AdpDep.Lane.IngressDpAuxHopId[StreamId] = IngressAuxHopId;
      CurRt->Adapter[UpAdpNum].AdpDep.Lane.EgressDpAuxHopId[StreamId] = EgressAuxHopId;
    }

    //
    // Set Aux In path for the next Router if the next Router is DP Source Router.
    //
    if (NextRt == DpSrcRt) {
      DEBUG ((PATH_VERBOSE, "Rt 0x%016llX Ln %d to DPIN %d (AUX-IN)\n", NextRt->TopologyId, NextAdpNum, DpSrcAdpNum));
      IngressAuxHopId = EgressAuxHopId;
      EgressAuxHopId  = DP_ADAPTER_AUX_HOPID;
      //
      // Set DP Aux-In path
      //
      DEBUG ((PATH_VERBOSE, "Ingress HopId = %d, Egress HopId = %d (AUX-IN)\n", IngressAuxHopId, EgressAuxHopId));
      Status = SetDpPathSeg (NextRt, NextAdpNum, DpSrcAdpNum, LANE_TO_DPIN_AUX, IngressAuxHopId, EgressAuxHopId);
      if (EFI_ERROR (Status)) {
        DEBUG ((PATH_VERBOSE, "Rt 0x%016llX Ln %d to DPIN %d (AUX-IN) failed, %r\n", NextRt->TopologyId, NextAdpNum, DpSrcAdpNum, Status));
        goto Exit;
      }
      NextRt->Adapter[NextAdpNum].AdpDep.Lane.IngressDpAuxHopId[StreamId] = IngressAuxHopId;
      break;
    }

    CurRt = NextRt;
    DsAdpNum = NextAdpNum;
  }
Exit:
  DEBUG ((PATH_VERBOSE, "SetDpInbondPath exit\n"));
  return Status;
}

/**
  Enable/Disable Video/Aux path of DP adapter.

  @param[in] Router  - Pointer to Router.
  @param[in] AdpNum  - DP adapter number.
  @param[in] Enable  - Indicate the path should be enabled or disabled.

  @retval EFI_SUCCESS           - Enable Video/Aux path of DP adapter success.
  @retval EFI_UNSUPPORTED       - Enable Video/Aux path of DP adapter failure.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
DpAdpEnablePath (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum,
  IN BOOLEAN         Enable
  )
{
  EFI_STATUS              Status;
  PUSB4_ADAPTER           DpAdp;
  USB4_ADP_CAP_DP_CS_0    DpCapCs0;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "DpAdpEnablePath: Null Router!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "DpAdpEnablePath: Invalid AdpNum %d!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "DpAdpEnablePath - Rt 0x%016llX Adp %d Enable=%d\n", Router->TopologyId, AdpNum, Enable));
  DpAdp = &(Router->Adapter[AdpNum]);
  if ((DpAdp->Type != USB4_ADP_TYPE_DP_IN) && (DpAdp->Type != USB4_ADP_TYPE_DP_OUT)) {
    DEBUG ((DEBUG_ERROR, "DpAdpEnablePath: Rt 0x%016llX Adp %d is not DP adapter\n", Router->TopologyId, AdpNum));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Status = Usb4CsAdpRead (Router, AdpNum, DpAdp->AdpCapReg + USB4_CS_0, 1, (UINT32 *) &DpCapCs0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX adp %d read DP Cap CS0 failure, %r\n", Router->TopologyId, AdpNum, Status));
    goto Exit;
  }
  DEBUG ((PATH_VERBOSE, "DP Rt 0x%016llX adp %d cap 0x%x - Original DpCapCs0 0x%08X\n", Router->TopologyId, AdpNum, DpAdp->AdpCapReg, DpCapCs0));
  if (Enable) {
    DpCapCs0.Fields.VideoEnable = 1;
    DpCapCs0.Fields.AuxEnable   = 1;
  } else {
    DpCapCs0.Fields.VideoEnable = 0;
    DpCapCs0.Fields.AuxEnable   = 0;
  }
  Status = Usb4CsAdpWrite (Router, AdpNum, DpAdp->AdpCapReg + USB4_CS_0, 1, (UINT32 *) &DpCapCs0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX adp %d write DP Cap CS0 failure, %r\n", Router->TopologyId, AdpNum, Status));
    goto Exit;
  }

  //
  // Read back the value for checking - For initial development phase only
  //
  Status = Usb4CsAdpRead (Router, AdpNum, DpAdp->AdpCapReg + USB4_CS_0, 1, (UINT32 *) &DpCapCs0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX adp %d read DP Cap CS0 failure, %r\n", Router->TopologyId, AdpNum, Status));
    goto Exit;
  }
  DEBUG ((PATH_VERBOSE, "DP Rt 0x%016llX adp %d cap 0x%x - Updated DpCapCs0 0x%08X\n", Router->TopologyId, AdpNum, DpAdp->AdpCapReg, DpCapCs0));

Exit:
  return Status;
}

/**
  Enable DP path between DP source and DP sink.

  @param[in] DpSrcRt      - Pointer to DP Source Router.
  @param[in] DpSrcAdpNum  - DP-IN adapter number of DP source.
  @param[in] DpSinkRt     - Pointer to DP Sink Router.
  @param[in] DpSinkAdpNum - DP_OUT adapter number of DP sink.

  @retval EFI_SUCCESS           - Enable DP path successfully.
  @retval EFI_UNSUPPORTED       - Fail to enable DP path failure.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
DpPathEnable (
  IN PUSB4_ROUTER    DpSrcRt,
  IN UINT8           DpSrcAdpNum,
  IN PUSB4_ROUTER    DpSinkRt,
  IN UINT8           DpSinkAdpNum
  )
{
  EFI_STATUS       Status;

  DEBUG ((PATH_VERBOSE, "DpPathEnable entry\n"));

  if ((DpSrcRt == NULL) || (DpSinkRt == NULL)) {
    DEBUG ((DEBUG_ERROR, "DpPathEnable: Null router (%p, %p)\n", DpSrcRt, DpSinkRt));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((DpSrcAdpNum == 0) || (DpSrcAdpNum > DpSrcRt->MaxAdapter)) {
    DEBUG ((DEBUG_ERROR, "DpPathEnable: DpSrcRt invalid DpSrcAdpNum %d\n", DpSrcAdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((DpSinkAdpNum == 0) || (DpSinkAdpNum > DpSinkRt->MaxAdapter)) {
    DEBUG ((DEBUG_ERROR, "DpPathEnable: DpSinkRt Invalid DpSinkAdpNum %d\n", DpSinkAdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Enable DP-IN adapter to send packets on Main Link path and send/receive packets on Aux path.
  //
  DEBUG ((CM_DEBUG, "Rt 0x%016llX Adp %d: Enable DP path of DP IN adapter\n", DpSrcRt->TopologyId, DpSrcAdpNum));
  Status = DpAdpEnablePath (DpSrcRt, DpSrcAdpNum, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "DP IN path enable failure - Rt 0x%016llX Adp %d, %r\n", DpSrcRt->TopologyId, DpSrcAdpNum, Status));
    goto Exit;
  }

  //
  // Enable DP-OUT adapter to receive packets on Main Link path and send/receive packets on Aux path.
  //
  DEBUG ((CM_DEBUG, "Rt 0x%016llX Adp %d: Enable DP path of DP OUT adapter\n", DpSinkRt->TopologyId, DpSinkAdpNum));
  Status = DpAdpEnablePath (DpSinkRt, DpSinkAdpNum, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "DP OUT path enable failure - Rt 0x%016llX Adp %d, %r\n", DpSinkRt->TopologyId, DpSinkAdpNum, Status));
    goto Exit;
  }

  DEBUG ((CM_DEBUG, "Rt 0x%016llX Adp %d : Wait for DPRX Capabilities Read Done\n", DpSrcRt->TopologyId, DpSrcAdpNum));

Exit:
  DEBUG ((PATH_VERBOSE, "DpPathEnable exit\n"));
  return Status;
}

/**
  Update Video/Aux HopId in DP adapter capability for TBT3 compatibility

  @param[in] DpSrcRt   - Pointer to DP source Router.
  @param[in] DpSinkRt  - Pointer to DP sink Router.

  @retval EFI_SUCCESS  - Setup DP path successfully.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval EFI_UNSUPPORTED       - Not DP adapter.
  @retval Errors in USB4 config space access.
**/
EFI_STATUS
UpdateDpHopId (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum
  )
{
  EFI_STATUS               Status;
  PUSB4_ADAPTER            DpAdp;
  PUSB4_ADP_CAP_DP_CS_0    DpCapCs0;
  PUSB4_ADP_CAP_DP_CS_1    DpCapCs1;
  UINT32                   DpCapCs[2];

  DEBUG ((PATH_VERBOSE, "UpdateDpHopId entry\n"));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "UpdateDpHopId: Null Router!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "UpdateDpHopId: Invalid AdpNum %d!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DpAdp = &Router->Adapter[AdpNum];

  if ((DpAdp->Type != USB4_ADP_TYPE_DP_IN) && (DpAdp->Type != USB4_ADP_TYPE_DP_OUT)) {
    DEBUG ((DEBUG_ERROR, "UpdateDpHopId: Rt 0x%016llX Adp %d is not DP adapter!\n", Router->TopologyId, AdpNum));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  DpCapCs0 = (PUSB4_ADP_CAP_DP_CS_0) &DpCapCs[0];
  DpCapCs1 = (PUSB4_ADP_CAP_DP_CS_1) &DpCapCs[1];

  Status = Usb4CsAdpRead (Router, AdpNum, DpAdp->AdpCapReg, 2, (UINT32 *) &DpCapCs);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX adp %d read DP Cap CS failure, %r\n", Router->TopologyId, AdpNum, Status));
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Rt 0x%016llX Adp %d DP Cap - Video HopId = %d, AuxRxHopId = %d, AuxTxHopId = %d\n",
          Router->TopologyId,
          AdpNum,
          DpCapCs0->Fields.VideoHopId,
          DpCapCs1->Fields.AuxRxHopId,
          DpCapCs1->Fields.AuxTxHopId
          ));

  if ((DpCapCs0->Fields.VideoHopId == 0) || (DpCapCs1->Fields.AuxRxHopId == 0) || (DpCapCs1->Fields.AuxTxHopId == 0)) {
    DEBUG ((DEBUG_INFO, "Rt 0x%016llX Adp %d - Update DP HopId\n", Router->TopologyId, AdpNum));
    DpCapCs0->Fields.VideoHopId = DP_ADAPTER_MAIN_HOPID;
    DpCapCs1->Fields.AuxRxHopId = DP_ADAPTER_AUX_HOPID;
    DpCapCs1->Fields.AuxTxHopId = DP_ADAPTER_AUX_HOPID;
    Status = Usb4CsAdpWrite (Router, AdpNum, DpAdp->AdpCapReg, 2, (UINT32 *)DpCapCs);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Adp %d write DP Cap CS failure, %r\n", Router->TopologyId, AdpNum, Status));
      goto Exit;
    }

    //
    // Read back the Video/Aux HopID for checking, only for initial development phase
    //
    Status = Usb4CsAdpRead (Router, AdpNum, DpAdp->AdpCapReg, 2, (UINT32 *)&DpCapCs);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Adp %d read DP Cap CS failure, %r\n", Router->TopologyId, AdpNum, Status));
      goto Exit;
    }
    DEBUG ((DEBUG_INFO, "Rt 0x%016llX Adp %d DP Cap - Updated Video HopId = %d, AuxRxHopId = %d, AuxTxHopId = %d\n",
            Router->TopologyId,
            AdpNum,
            DpCapCs0->Fields.VideoHopId,
            DpCapCs1->Fields.AuxRxHopId,
            DpCapCs1->Fields.AuxTxHopId
            ));
  }

Exit:
  DEBUG ((PATH_VERBOSE, "UpdateDpHopId exit\n"));
  return Status;
}

/**
  Traverse and configure the path between DP source adapter and DP sink adapter.

  @param[in] DpSrcRt      - Pointer to DP Source Router.
  @param[in] DpSrcAdpNum  - DP-IN adapter number of DP source.
  @param[in] DpSinkRt     - Pointer to DP Sink Router.
  @param[in] DpSinkAdpNum - DP_OUT adapter number of DP sink.
  @param[in] StreamId     - DP stream ID for identifying DP path.

  @retval EFI_SUCCESS           - Setup DP path successfully.
  @retval EFI_UNSUPPORTED       - Fail to setup DP path.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
DpPathSetup (
  IN PUSB4_ROUTER    DpSrcRt,
  IN UINT8           DpSrcAdpNum,
  IN PUSB4_ROUTER    DpSinkRt,
  IN UINT8           DpSinkAdpNum,
  IN UINT32          StreamId
  )
{
  EFI_STATUS    Status;

  DEBUG ((PATH_VERBOSE, "DpPathSetup entry - StreamId = %d\n", StreamId));

  if ((DpSrcRt == NULL) || (DpSinkRt == NULL) || (StreamId >= USB4_DP_STREAM_MAX)) {
    DEBUG ((DEBUG_ERROR, "DpPathSetup: Null router (%p, %p, %d)\n", DpSrcRt, DpSinkRt, StreamId));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "DP Src  : Rt 0x%016llX DP-IN %d\n", DpSrcRt->TopologyId, DpSrcAdpNum));
  DEBUG ((DEBUG_INFO, "DP Sink : Rt 0x%016llX DP-OUT %d\n", DpSinkRt->TopologyId, DpSinkAdpNum));

  // DpSrcRt should be ancestor of DpSinkRt in the topology tree
  if ((DpSinkRt->TopologyId.Value & DpSrcRt->TopologyId.Value) != DpSrcRt->TopologyId.Value) {
    DEBUG ((DEBUG_ERROR, "DP source router is not ancestor of DP sink router!"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Update Video/Aux HopId in DP adapter capability for TBT3 Router
  //
  if (DpSrcRt->State.Tbt3) {
    DEBUG ((DEBUG_INFO, "Update Video/Aux HopId for DP-IN Router (TBT3)\n"));
    Status = UpdateDpHopId (DpSrcRt, DpSrcAdpNum);
  }

  if (DpSinkRt->State.Tbt3) {
    DEBUG ((DEBUG_INFO, "Update Video/Aux HopId for DP-OUT Router (TBT3)\n"));
    Status = UpdateDpHopId (DpSinkRt, DpSinkAdpNum);
  }

  //
  // Set DP MAIN Link path and AUX out path
  //
  Status = SetDpOutbondPath (DpSrcRt, DpSrcAdpNum, DpSinkRt, DpSinkAdpNum, StreamId);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Set DP outbond path failure, %r\n", Status));
    goto Exit;
  }

  //
  // Set DP Aux In path
  //
  Status = SetDpInbondPath (DpSrcRt, DpSrcAdpNum, DpSinkRt, DpSinkAdpNum, StreamId);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Set DP inbond path failure, %r\n", Status));
    goto Exit;
  }

  //
  // Enable DP path
  //
  Status = DpPathEnable (DpSrcRt, DpSrcAdpNum, DpSinkRt, DpSinkAdpNum);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Enable DP path failure, %r\n", Status));
    goto Exit;
  }

Exit:
  DEBUG ((PATH_VERBOSE, "DpPathSetup exit\n"));
  return Status;
}

/**
  Tear down the DP path between the DP source and DP sink.
  If RemovedRt is specified, tear down DP path from DP source to RemovedRt's upper level router.

  @param[in] DpSrcRt      - Pointer to DP Source Router.
  @param[in] DpSrcAdpNum  - DP-IN adapter number of DP source.
  @param[in] DpSinkRt     - Pointer to DP Sink Router.
  @param[in] DpSinkAdpNum - DP_OUT adapter number of DP sink.
  @param[in] StreamId     - DP stream ID for identifying DP path.
  @param[in] RemovedRt    - Removed router in the DP path (optional)

  @retval EFI_SUCCESS           - DP path tear down success.
  @retval EFI_UNSUPPORTED       - Fail to tear down DP path.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
DpPathTearDown (
  IN PUSB4_ROUTER    DpSrcRt,
  IN UINT8           DpSrcAdpNum,
  IN PUSB4_ROUTER    DpSinkRt,
  IN UINT8           DpSinkAdpNum,
  IN UINT32          StreamId,
  IN PUSB4_ROUTER    RemovedRt
  )
{
  EFI_STATUS            Status;
  PUSB4_ROUTER          CurRt;
  UINT16                EgressMainHopId;
  UINT16                EgressAuxOutHopId;
  UINT16                EgressAuxInHopId;
  UINT16                IngressMainHopId;
  UINT16                IngressAuxOutHopId;
  UINT16                IngressAuxInHopId;
  UINT8                 InAdpNum;
  UINT8                 OutAdpNum;

  DEBUG ((DEBUG_INFO, "DpPathTearDown entry - StreamId = %d\n", StreamId));

  Status = EFI_SUCCESS;

  if ((DpSrcRt == NULL) || (DpSinkRt == NULL) || (StreamId >= USB4_DP_STREAM_MAX)) {
    DEBUG ((DEBUG_ERROR, "DpPathTearDown: NULL DP Sink/Src - DpSrcRt %p, DpSinkRt %p, StreamId %d\n", DpSrcRt, DpSinkRt, StreamId));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Tear down the DP path from DP source to DP sink
  // Stop tearing down the path if RemovedRt is specified and the current router is the removed router.
  // When RemovedRt is specified, it means the router and its downstream routers have been removed.
  // Their USB4 config space can't be accessed.
  //
  // InAdpNum  : DP-IN adapter number or Upstream Lane adapter number.
  // OutAdpNum : DP-OUT adapter number or Downstream Lane adapter number.
  // Each Lane adapter in the DP path records the ingress/egress HopID used for the DP stream.
  // Egress HopID of Lane adapter in DP path is dynamic and controlled by HopIdMask of each Lane adapter.
  // When the DP path is disabled, the egress HopID should be released so that the HopID can be re-used in new DP path setup.
  //
  // For each router in the DP path, do the following operations for path tear down:
  // 1. Disable DP Main path segment that starts from DP-IN/Upstream Lane adapter to Downstream Lane adapter/DP-OUT.
  // 2. Disable DP AUX-OUT path segment that starts from DP-IN/Upstream Lane adapter to Downstream Lane adapter/DP-OUT.
  // 3. Disable DP AUX-IN path segment that start from DP-OUT/Downstream Lane adapter to Upstream Lane adapter/DP-IN.
  // 4. For Upstream Lane adapter, free the assigned egress HopIDs and clear ingress/egress HopIDs of DP stream.
  // 5. For Downstream Lane adapter, free the assigned egress HopIDs and clear ingress/egress HopIDs of DP stream.
  //
  for (CurRt = DpSrcRt; CurRt != NULL && CurRt != RemovedRt;) {
    if ((CurRt->Depth > mUsb4TopologyDepth) || (CurRt->Depth >= TOPOLOGY_ID_ADAPTER_SIZE)) {
      DEBUG ((DEBUG_ERROR, "DpPathTearDown: Invalid router depth %d\n", CurRt->Depth));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }

    InAdpNum = (CurRt == DpSrcRt) ? DpSrcAdpNum : CurRt->LaneUpAdpNum;
    OutAdpNum = (CurRt == DpSinkRt) ? DpSinkAdpNum : DpSinkRt->TopologyId.Adapter[CurRt->Depth];
    DEBUG ((DEBUG_INFO, "Rt 0x%016llX InAdp %d OutAdp %d\n", CurRt->TopologyId, InAdpNum, OutAdpNum));

    //
    // Make sure InAdpNum and OutAdpNum are in valid range for array access
    //
    if ((InAdpNum == 0) || (InAdpNum > CurRt->MaxAdapter) || (InAdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
      DEBUG ((DEBUG_ERROR, "DpPathTearDown: Invalid InAdpNum %d\n", InAdpNum));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
    if ((OutAdpNum == 0) || (OutAdpNum > CurRt->MaxAdapter) || (OutAdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
      DEBUG ((DEBUG_ERROR, "DpPathTearDown: Invalid OutAdpNum %d\n", OutAdpNum));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }

    IngressMainHopId   = (CurRt == DpSrcRt) ? DP_ADAPTER_MAIN_HOPID : CurRt->Adapter[InAdpNum].AdpDep.Lane.IngressDpMainHopId[StreamId];
    IngressAuxOutHopId = (CurRt == DpSrcRt) ? DP_ADAPTER_AUX_HOPID : CurRt->Adapter[InAdpNum].AdpDep.Lane.IngressDpAuxHopId[StreamId];
    EgressMainHopId    = (CurRt == DpSinkRt) ? DP_ADAPTER_MAIN_HOPID : CurRt->Adapter[OutAdpNum].AdpDep.Lane.EgressDpMainHopId[StreamId];
    EgressAuxOutHopId  = (CurRt == DpSinkRt) ? DP_ADAPTER_AUX_HOPID : CurRt->Adapter[OutAdpNum].AdpDep.Lane.EgressDpAuxHopId[StreamId];
    IngressAuxInHopId  = (CurRt == DpSinkRt) ? DP_ADAPTER_AUX_HOPID : CurRt->Adapter[OutAdpNum].AdpDep.Lane.IngressDpAuxHopId[StreamId];
    EgressAuxInHopId   = (CurRt == DpSrcRt) ? DP_ADAPTER_AUX_HOPID : CurRt->Adapter[InAdpNum].AdpDep.Lane.EgressDpAuxHopId[StreamId];
    DEBUG ((DEBUG_INFO, "InAdp: IngressHopId(Main) = %d, IngressHopId(Aux-Out) = %d, EgressHopId(Aux-In) = %d\n", IngressMainHopId, IngressAuxOutHopId, EgressAuxInHopId));
    DEBUG ((DEBUG_INFO, "OutAdp: EgressHopId(Main) = %d, EgressHopId(Aux-Out) = %d, IngressHopId(Aux-In) = %d\n", EgressMainHopId, EgressAuxOutHopId, IngressAuxInHopId));

    //
    // Disable DP Main link path entry of current router
    //
    DEBUG ((DEBUG_INFO, "Disable Rt 0x%016llX Adp %d path %d (MAIN)\n", CurRt->TopologyId, InAdpNum, IngressMainHopId));
    Status = Usb4CsPathEnable (CurRt, InAdpNum, IngressMainHopId, FALSE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Disable Rt 0x%016llX Adp %d path %d failure (MAIN), %r\n", CurRt->TopologyId, InAdpNum, IngressMainHopId, Status));
      goto Exit;
    }

    //
    // Disable DP Aux-Out path entry of current router
    //
    DEBUG ((DEBUG_INFO, "Disable Rt 0x%016llX Adp %d path %d (AUX-OUT)\n", CurRt->TopologyId, InAdpNum, IngressAuxOutHopId));
    Status = Usb4CsPathEnable (CurRt, InAdpNum, IngressAuxOutHopId, FALSE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Disable Rt 0x%016llX Adp %d path %d failure (AUX-OUT), %r\n", CurRt->TopologyId, InAdpNum, IngressAuxOutHopId, Status));
      goto Exit;
    }

    //
    // Disable DP Aux-In path entry of current router
    //
    DEBUG ((DEBUG_ERROR, "Disable Rt 0x%016llX Adp %d path %d (AUX-IN)\n", CurRt->TopologyId, OutAdpNum, IngressAuxInHopId));
    Status = Usb4CsPathEnable (CurRt, OutAdpNum, IngressAuxInHopId, FALSE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Disable Rt 0x%016llX Adp %d path %d failure (AUX-IN), %r\n", CurRt->TopologyId, OutAdpNum, IngressAuxInHopId, Status));
      goto Exit;
    }

    //
    // Free the dynamic egress HopID used in Lane adapter and clear HopID used for DP stream
    //
    if (CurRt->Adapter[InAdpNum].Type == USB4_ADP_TYPE_LANE) {
      //
      // Free HopID used in egress Lane adapter of DP Aux-In path
      //
      Status = FreeHopId (CurRt, InAdpNum, EgressAuxInHopId);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "DpPathTearDown: Fail to free Rt 0x%016llX Adp %d HopID %d, %r\n", CurRt->TopologyId, InAdpNum, EgressAuxInHopId, Status));
        goto Exit;
      }

      CurRt->Adapter[InAdpNum].AdpDep.Lane.IngressDpMainHopId[StreamId] = 0;
      CurRt->Adapter[InAdpNum].AdpDep.Lane.IngressDpAuxHopId[StreamId]  = 0;
      CurRt->Adapter[InAdpNum].AdpDep.Lane.EgressDpAuxHopId[StreamId]   = 0;
    }

    //
    // Free the dynamic egress HopID used in Lane adapter and clear HopID used for DP stream
    //
    if (CurRt->Adapter[OutAdpNum].Type == USB4_ADP_TYPE_LANE) {
      //
      // Free HopID used in egress Lane adapter of DP Main link path
      //
      Status = FreeHopId (CurRt, OutAdpNum, EgressMainHopId);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "DpPathTearDown: Fail to free Rt 0x%016llX Adp %d HopID %d, %r\n", CurRt->TopologyId, OutAdpNum, EgressMainHopId, Status));
        goto Exit;
      }
      //
      // Free HopID used in egress Lane adapter of DP Aux-Out path
      //
      Status = FreeHopId (CurRt, OutAdpNum, EgressAuxOutHopId);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "DpPathTearDown: Fail to free Rt 0x%016llX Adp %d HopID %d, %r\n", CurRt->TopologyId, OutAdpNum, EgressAuxOutHopId, Status));
        goto Exit;
      }

      CurRt->Adapter[OutAdpNum].AdpDep.Lane.EgressDpMainHopId[StreamId] = 0;
      CurRt->Adapter[OutAdpNum].AdpDep.Lane.EgressDpAuxHopId[StreamId]  = 0;
      CurRt->Adapter[OutAdpNum].AdpDep.Lane.IngressDpAuxHopId[StreamId] = 0;
    }

    //
    // Reach end of DP path
    //
    if (CurRt == DpSinkRt) {
      break;
    }

    //
    // Go to next level router in DP path.
    //
    CurRt = CurRt->Adapter[OutAdpNum].AdpDep.Lane.DsRouter;
  }

Exit:
  DEBUG ((DEBUG_INFO, "DpPathTearDown exit\n"));
  return Status;
}
