/** @file
  PCIe tunneling implementation.

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
#include <Library/Usb4CsLib.h>
#include <Library/Usb4RouterLib.h>
#include <Library/Usb4Capabilities.h>
#include "ProtocolConfig.h"

/**
  Set Path segment starting from PCIe adapter to Lane adapter.

  @param[in] Router  - Pointer to Router instance.
  @param[in] Ingress - Ingress adapter number.
  @param[in] Egress  - Egress adapter number.

  @retval EFI_SUCCESS           - Set path segment successfully.
  @retval EFI_UNSUPPORTED       - Fail to set path segment.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
SetPathPcieToLane (
  IN PUSB4_ROUTER    Router,
  IN UINT8           Ingress,
  IN UINT8           Egress
  )
{
  EFI_STATUS            Status;
  PUSB4_ADAPTER         PcieAdp;
  PUSB4_ADAPTER         LaneAdp;
  USB4_PATH_CS_ENTRY    PathEntry;

  DEBUG ((DEBUG_INFO, "SetPathPcieToLane entry\n"));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "SetPathPcieToLane: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((Ingress == 0) || (Ingress > Router->MaxAdapter) || (Ingress >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "SetPathPcieToLane: Invalid ingress adp %d!\n", Ingress));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  PcieAdp = &(Router->Adapter[Ingress]);
  if ((PcieAdp->Type != USB4_ADP_TYPE_PCIE_US) && (PcieAdp->Type != USB4_ADP_TYPE_PCIE_DS)) {
    DEBUG ((DEBUG_ERROR, "SetPathPcieToLane: Ingress adapter is not PCIe adapter\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if ((Egress == 0) || (Egress > Router->MaxAdapter) || (Egress >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "SetPathPcieToLane: Invalid egress adp %d\n", Egress));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  LaneAdp = &Router->Adapter[Egress];

  if (LaneAdp->Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "SetPathPcieToLane: Egress adapter is not Lane adapter\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Router 0x%016llX PCIe = %d, Lane = %d\n", Router->TopologyId, Ingress, Egress));

  Status = Usb4CsPathRead (Router, Ingress, PCIE_ADAPTER_HOPID, &PathEntry);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Read path cs (Adp %d, HopId %d) failure, %r\n", Ingress, PCIE_ADAPTER_HOPID, Status));
    goto Exit;
  }

  //
  // Check the original path entry values - only for initial development phase
  //
  DEBUG ((DEBUG_INFO, "Original path entry\n"));
  DumpPathEntry (&PathEntry);

  PathEntry.PathCs0.Fields.OutputAdapter = (UINT32) Egress;
  PathEntry.PathCs0.Fields.OutputHopId   = LANE_ADAPTER_PCIE_HOPID;
  PathEntry.PathCs0.Fields.Valid         = 1;
  PathEntry.PathCs1.Fields.Priority      = PCIE_TO_LANE_PATH_PRIORITY;
  PathEntry.PathCs1.Fields.Weight        = PCIE_TO_LANE_PATH_WEIGHT;
  PathEntry.PathCs1.Fields.Efc           = PCIE_TO_LANE_PATH_EFC;
  PathEntry.PathCs1.Fields.Ese           = PCIE_TO_LANE_PATH_ESE;
  DEBUG ((DEBUG_INFO, "Path BlockLow (Bit29) = %d\n", PathEntry.PathCs1.Fields.BlockLow));
  PathEntry.PathCs1.Fields.BlockLow      = 0;

  //
  // Required for TBT3 Router, N/A for USB4 Router
  //
  if (Router->State.Tbt3) {
    PathEntry.PathCs1.Fields.Ifc = PCIE_TO_LANE_PATH_IFC;
    PathEntry.PathCs1.Fields.Ise = PCIE_TO_LANE_PATH_ISE;
  }

  Status = Usb4CsPathWrite (Router, Ingress, PCIE_ADAPTER_HOPID, &PathEntry);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Write path cs (Adp %d, HopId %d) failure, %r\n", Ingress, PCIE_ADAPTER_HOPID, Status));
    goto Exit;
  }

  //
  // only for initial development phase
  //
  Status = Usb4CsPathRead (Router, Ingress, PCIE_ADAPTER_HOPID, &PathEntry);
  DEBUG ((DEBUG_INFO, "Updated path entry\n"));
  DumpPathEntry (&PathEntry);
Exit:
  DEBUG ((DEBUG_INFO, "SetPathPcieToLane exit\n"));
  return Status;
}

/**
  Credit number should be allocated for Lane to PCIe path segment

  @param[in] Router     - Pointer to Router instance.
  @param[in] Ingress    - Ingress adapter number.

  @retval Credit number should be allocated.
  @retval 0  - N/A for adapter other than Lane adapter
**/
UINT32
CreditsForLaneToPciePath (
  IN PUSB4_ROUTER    Router,
  IN UINT8           Ingress
  )
{
  EFI_STATUS                Status;
  PUSB4_ADAPTER             LaneAdp;
  USB4_ADP_CAP_LANE_CS_1    AdpCapCs1;
  UINT32                    Credits;
  UINT32                    LinkWidth;
  UINT32                    LinkSpeed;

  Credits = 0;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "CreditsForLaneToPciePath: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((Ingress == 0) || (Ingress > Router->MaxAdapter) || (Ingress >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "CreditsForLaneToPciePath: Invalid ingress adp %d!\n", Ingress));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  LaneAdp = &(Router->Adapter[Ingress]);
  if ((LaneAdp->Type != USB4_ADP_TYPE_LANE) || (LaneAdp->AdpCapReg == 0)) {
    DEBUG ((DEBUG_ERROR, "Adp %d is not Lane adapter or Lane adapter capability is NULL.\n", Ingress));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Status = Usb4CsAdpRead (Router, Ingress, LaneAdp->AdpCapReg + USB4_CS_1, 1, (UINT32 *) &AdpCapCs1);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Query Lane Cap failure: Adapter %d at 0x%0x, %r\n", Ingress, LaneAdp->AdpCapReg, Status));
    goto Exit;
  }

  LinkWidth = AdpCapCs1.Fields.CurLinkWidth;
  LinkSpeed = AdpCapCs1.Fields.CurLinkSpeed;
  Credits = LANE_TO_PCIE_PATH_CREDITS_LEGACY;

  if (Router->RouterType == USB4_ROUTER_MAPLE_RIDGE) {
    Credits = (LinkWidth == LANE_LINK_WIDTH_x2) ? LANE_TO_PCIE_PATH_CREDITS_DTBT_X2 : LANE_TO_PCIE_PATH_CREDITS_DTBT_X1;
  } else {
    if (LinkSpeed == LANE_LINK_SPEED_GEN3) {
      Credits = (LinkWidth == LANE_LINK_WIDTH_x2) ? LANE_TO_PCIE_PATH_CREDITS_40G : LANE_TO_PCIE_PATH_CREDITS_20G;
    } else if (LinkSpeed == LANE_LINK_SPEED_GEN2) {
      Credits = (LinkWidth == LANE_LINK_WIDTH_x2) ? LANE_TO_PCIE_PATH_CREDITS_20G : LANE_TO_PCIE_PATH_CREDITS_10G;
    }
  }
  DEBUG ((DEBUG_INFO, "LinkWidth = 0x%0x, LinkSpeed = 0x%0x, PCIe Credits = %d\n", LinkWidth, LinkSpeed, Credits));
Exit:
  return Credits;
}

/**
  Set Path segment starting from Lane adapter to PCIe adapter.

  @param[in] Router  - Pointer to Router instance.
  @param[in] Ingress - Ingress adapter number.
  @param[in] Egress  - Egress adapter number.

  @retval EFI_SUCCESS           - Set path segment successfully.
  @retval EFI_UNSUPPORTED       - Fail to set path segment.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
SetPathLaneToPcie (
  IN PUSB4_ROUTER    Router,
  IN UINT8           Ingress,
  IN UINT8           Egress
  )
{
  EFI_STATUS            Status;
  PUSB4_ADAPTER         LaneAdp;
  PUSB4_ADAPTER         PcieAdp;
  USB4_PATH_CS_ENTRY    PathEntry;

  DEBUG ((DEBUG_INFO, "SetPathLaneToPcie entry\n"));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "SetPathLaneToPcie: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((Ingress == 0) || (Ingress > Router->MaxAdapter) || (Ingress >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "SetPathLaneToPcie: Invalid ingress adp %d!\n", Ingress));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  LaneAdp = &(Router->Adapter[Ingress]);
  if (LaneAdp->Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "SetPathLaneToPcie: Ingress adapter is not Lane adapter\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if ((Egress == 0) || (Egress > Router->MaxAdapter) || (Egress >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "SetPathLaneToPcie: Invalid egress adp %d\n", Egress));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  PcieAdp = &Router->Adapter[Egress];
  if ((PcieAdp->Type != USB4_ADP_TYPE_PCIE_US) && (PcieAdp->Type != USB4_ADP_TYPE_PCIE_DS)) {
    DEBUG ((DEBUG_ERROR, "SetPathLaneToPcie: Egress adapter is not PCIe adapter\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if (Router->Adapter[Ingress].MaxInputHopId < LANE_ADAPTER_PCIE_HOPID) {
    DEBUG ((DEBUG_ERROR, "Adapter MaxInputHopId %d is less than Lane adapter PCIe HopId\n", Router->Adapter[Ingress].MaxInputHopId));
    Status = EFI_UNSUPPORTED;
    ASSERT (0);
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Rt 0x%016llX - Lane = %d, PCIe = %d, Input HopId = %d\n", Router->TopologyId, Ingress, Egress, LANE_ADAPTER_PCIE_HOPID));

  Status = Usb4CsPathRead (Router, Ingress, LANE_ADAPTER_PCIE_HOPID, &PathEntry);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Read path cs (Adp %d, HopId %d) failure, %r\n", Ingress, LANE_ADAPTER_PCIE_HOPID, Status));
    goto Exit;
  }

  //
  // only for initial development phase
  //
  DEBUG ((DEBUG_INFO, "Original path entry\n"));
  DumpPathEntry (&PathEntry);

  PathEntry.PathCs0.Fields.OutputAdapter    = (UINT32) Egress;
  PathEntry.PathCs0.Fields.OutputHopId      = (UINT32) PCIE_ADAPTER_HOPID;
  PathEntry.PathCs0.Fields.Valid            = 1;
  PathEntry.PathCs1.Fields.Priority         = LANE_TO_PCIE_PATH_PRIORITY;
  PathEntry.PathCs1.Fields.Weight           = LANE_TO_PCIE_PATH_WEIGHT;
  PathEntry.PathCs1.Fields.Ifc              = LANE_TO_PCIE_PATH_IFC;
  PathEntry.PathCs1.Fields.Ise              = LANE_TO_PCIE_PATH_ISE;
  PathEntry.PathCs0.Fields.CreditsAllocated = CreditsForLaneToPciePath (Router, Ingress);
  DEBUG ((DEBUG_INFO, "Path BlockLow (Bit29) = %d\n", PathEntry.PathCs1.Fields.BlockLow));
  PathEntry.PathCs1.Fields.BlockLow         = 0;

  //
  // Required for TBT3 Router, N/A for USB4 Router
  //
  if (Router->State.Tbt3) {
    PathEntry.PathCs1.Fields.Efc              = LANE_TO_PCIE_PATH_EFC;
    PathEntry.PathCs1.Fields.Ese              = LANE_TO_PCIE_PATH_ESE;
  }

  Status = Usb4CsPathWrite (Router, Ingress, LANE_ADAPTER_PCIE_HOPID, &PathEntry);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Write path cs (Adapter=%d, HopId=%d) failure, %r\n", Ingress, LANE_ADAPTER_PCIE_HOPID, Status));
    goto Exit;
  }

  //
  // only for initial development phase
  //
  Status = Usb4CsPathRead (Router, Ingress, LANE_ADAPTER_PCIE_HOPID, &PathEntry);
  DEBUG ((DEBUG_INFO, "Updated path entry\n"));
  DumpPathEntry (&PathEntry);
Exit:
  DEBUG ((DEBUG_INFO, "SetPathLaneToPcie exit\n"));
  return Status;
}

/**
  Enable/Disable Extended Encapsulation on PCIe adapter

  @param[in] Router        - Pointer to Router instance.
  @param[in] PcieAdpNum    - PCIe adapter number.
  @param[in] Enable        - TRUE = Enable encapsulation , FALSE = Disable encapsulation.

  @retval EFI_SUCCESS           - Enable PCIe encapsulation success.
  @retval EFI_UNSUPPORTED       - Fail to enable PCIe encapsulation.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
PcieAdpEnableExtendedEncapsulation (
  IN PUSB4_ROUTER    Router,
  IN UINT8           PcieAdpNum,
  IN BOOLEAN         Enable
  )
{
  EFI_STATUS                Status;
  PUSB4_ADAPTER             PcieAdapter;
  USB4_ADP_CAP_PCIE_CS_1    PcieCapCs1;

  DEBUG ((DEBUG_INFO, "PcieAdpEnableExtendedEncapsulation entry\n"));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "PcieAdpEnableExtendedEncapsulation: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Router->Usb4Version != RT_USB4_VERSION_2) {
    DEBUG ((DEBUG_ERROR, "PcieAdpEnableExtendedEncapsulation: Router doesn't support USB4_VERSION_2\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if ((PcieAdpNum == 0) || (PcieAdpNum > Router->MaxAdapter) || (PcieAdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "PcieAdpEnableExtendedEncapsulation: Invalid AdpNum %d!\n", PcieAdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Rt 0x%016llX Adp %d Encapsulation = %d\n", Router->TopologyId, PcieAdpNum, Enable));

  PcieAdapter = &Router->Adapter[PcieAdpNum];
  if ((PcieAdapter->Type != USB4_ADP_TYPE_PCIE_US) && (PcieAdapter->Type != USB4_ADP_TYPE_PCIE_DS)) {
    DEBUG ((DEBUG_ERROR, "Adp %d is not PCIe adapter, adapter type = 0x%08x\n", PcieAdpNum, PcieAdapter->Type));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if (PcieAdapter->AdpCapReg == 0) {
    DEBUG ((DEBUG_ERROR, "PcieAdpEnableExtendedEncapsulation: PCIe Adapter Capability is not present\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Set Extended Encapsulation bit of PCIe adapter capability
  //
  Status = Usb4CsAdpRead (Router, PcieAdpNum, PcieAdapter->AdpCapReg + USB4_CS_1, sizeofdw(USB4_ADP_CAP_PCIE_CS_1), (UINT32 *) &PcieCapCs1);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Read PCIe adapter capability at 0x%0X failure , %r\n", PcieAdapter->AdpCapReg + USB4_CS_1, Status));
    goto Exit;
  }

  PcieCapCs1.Fields.ExtendedEncapsulation = (Enable) ? 1 : 0;

  Status = Usb4CsAdpWrite (Router, PcieAdpNum, PcieAdapter->AdpCapReg + USB4_CS_1, sizeofdw(USB4_ADP_CAP_PCIE_CS_1), (UINT32 *) &PcieCapCs1);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Write PCIe adapter capability at 0x%0X failure , %r\n", PcieAdapter->AdpCapReg, Status));
    goto Exit;
  }

Exit:
  DEBUG ((DEBUG_INFO, "PcieAdpEnableExtendedEncapsulation exit\n"));
  return Status;
}

/**
  Enable/Disable PCIe path on PCIe adapter
  This controls whether PCIe tunneled packets can be sent by PCIe adapter or not.

  @param[in] Router        - Pointer to Router instance.
  @param[in] PcieAdpNum    - PCIe adapter number.
  @param[in] PathEnable    - TRUE = Enable path, FALSE = Disable path.

  @retval EFI_SUCCESS           - Enable PCIe path success.
  @retval EFI_UNSUPPORTED       - Fail to enable path segment.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
PcieAdpEnablePath (
  IN PUSB4_ROUTER    Router,
  IN UINT8           PcieAdpNum,
  IN BOOLEAN         PathEnable
  )
{
  EFI_STATUS           Status;
  PUSB4_ADAPTER        PcieAdapter;
  USB4_ADP_CAP_PCIE    PcieCapCs;

  DEBUG ((DEBUG_INFO, "PcieAdpEnablePath entry\n"));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "PcieAdpEnablePath: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((PcieAdpNum == 0) || (PcieAdpNum > Router->MaxAdapter) || (PcieAdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "PcieAdpEnablePath: Invalid AdpNum %d!\n", PcieAdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Router 0x%016llX PCIe adapter %d Enable = %d\n", Router->TopologyId, PcieAdpNum, PathEnable));

  PcieAdapter = &Router->Adapter[PcieAdpNum];
  if ((PcieAdapter->Type != USB4_ADP_TYPE_PCIE_US) && (PcieAdapter->Type != USB4_ADP_TYPE_PCIE_DS)) {
    DEBUG ((DEBUG_ERROR, "Adp %d is not PCIe adapter, adapter type = 0x%08x\n", PcieAdpNum, PcieAdapter->Type));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if (PcieAdapter->AdpCapReg == 0) {
    DEBUG ((DEBUG_ERROR, "PcieAdpEnablePath: PCIe Adapter Capability is not present\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Set Path Enable bit of PCIe adapter capability
  //
  Status = Usb4CsAdpRead (Router, PcieAdpNum, PcieAdapter->AdpCapReg, sizeofdw (USB4_ADP_CAP_PCIE), (UINT32 *) &PcieCapCs);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Read PCIe adapter capability at 0x%0X failure , %r\n", PcieAdapter->AdpCapReg, Status));
    goto Exit;
  }
  PcieCapCs.CapCs0.Fields.PathEnable = (PathEnable) ? 1 : 0;
  Status = Usb4CsAdpWrite (Router, PcieAdpNum, PcieAdapter->AdpCapReg, sizeofdw (USB4_ADP_CAP_PCIE), (UINT32 *) &PcieCapCs);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Write PCIe adapter capability at 0x%0X failure , %r\n", PcieAdapter->AdpCapReg, Status));
    goto Exit;
  }

  //
  // Read back the value for checking - initial development phase only
  //
  Status = Usb4CsAdpRead (Router, PcieAdpNum, PcieAdapter->AdpCapReg, sizeofdw (USB4_ADP_CAP_PCIE), (UINT32 *) &PcieCapCs);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Read PCIe adapter capability at 0x%0X failure , %r\n", PcieAdapter->AdpCapReg, Status));
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "PCIe Path Enable bit = %d\n", PcieCapCs.CapCs0.Fields.PathEnable));
  DEBUG ((DEBUG_INFO, "PCIe LTSSM state = 0x%x\n", PcieCapCs.CapCs0.Fields.LtssmState));

  PcieAdapter->AdpDep.AdpPcie.Status.PathEnable = (PathEnable) ? 1 : 0;

Exit:
  DEBUG ((DEBUG_INFO, "PcieAdpEnablePath exit\n"));
  return Status;
}

/**
  Setup PCIe path between router's PCIe downstream adapter and downstream router's PCIe upstream adapter.

  @param[in] Router - Pointer to Router instance.
  @param[in] AdpNum - Adapter number of Lane adapter to setup PCIe path.

  @retval EFI_SUCCESS           - Config PCIe path successfully.
  @retval EFI_UNSUPPORTED       - Fail to config PCIe path.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4RtConfigDsPciePath (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum
  )
{
  EFI_STATUS       Status;
  PUSB4_ROUTER     DsRouter;
  PUSB4_ADAPTER    LaneAdp;
  UINT8            PcieAdpNum;

  DEBUG ((DEBUG_INFO, "Usb4RtConfigDsPciePath entry\n"));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4RtConfigDsPciePath: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "Usb4RtConfigDsPciePath: Invalid AdpNum %d!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  LaneAdp = &Router->Adapter[AdpNum];
  if (LaneAdp->Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "Usb4RtConfigDsPciePath: Adp %d is not Lane Adapter, Adapter type = %d\n",AdpNum, LaneAdp->Type));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  DsRouter = LaneAdp->AdpDep.Lane.DsRouter;
  if (DsRouter == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4RtConfigDsPciePath: Null DsRouter at Adp %d!\n", AdpNum));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Paired PCIe downstream adapter of the downstream Lane adapter.
  //
  PcieAdpNum = LaneAdp->AdpDep.Lane.PairedPcieAdp;

  //
  // Setup Outbond path
  //
  DEBUG ((DEBUG_INFO, "Set outbond path : Router 0x%016llX to Router 0x%016llX\n", Router->TopologyId, DsRouter->TopologyId));
  Status = SetPathPcieToLane (Router, PcieAdpNum, AdpNum);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }
  Status = SetPathLaneToPcie (DsRouter, DsRouter->LaneUpAdpNum, DsRouter->PcieUpAdpNum);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  //
  // Setup Inbond path
  //
  DEBUG ((DEBUG_INFO, "Set inbond path : Router 0x%016llX to Router 0x%016llX\n", DsRouter->TopologyId, Router->TopologyId));
  Status = SetPathPcieToLane (DsRouter, DsRouter->PcieUpAdpNum, DsRouter->LaneUpAdpNum);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }
  Status = SetPathLaneToPcie (Router, AdpNum, PcieAdpNum);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  //
  // Enable Extended Encapsulation on PCIe adapter if the Routers on both ends of a USB4 Link support Version 2.0.
  //
  if ((Router->Usb4Version == RT_USB4_VERSION_2) && (DsRouter->Usb4Version == RT_USB4_VERSION_2)) {
    Status = PcieAdpEnableExtendedEncapsulation (Router, PcieAdpNum, TRUE);
    if (EFI_ERROR (Status)) {
      goto Exit;
    }

    Status = PcieAdpEnableExtendedEncapsulation (DsRouter, DsRouter->PcieUpAdpNum, TRUE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "DsRt 0x%016llX Adp %d enable Pcie EE failure, %r\n", DsRouter->TopologyId, DsRouter->PcieUpAdpNum, Status));
      Status = PcieAdpEnableExtendedEncapsulation (Router, PcieAdpNum, FALSE);
      DEBUG ((DEBUG_INFO, "Clean Rt 0x%016llX Adp %d Pcie EE (%r)\n", Router->TopologyId, PcieAdpNum, Status));
      goto Exit;
    }

    DEBUG ((DEBUG_INFO, "Enable PCIe Extended Encapsulation successfully\n"));
  }

  //
  // Enable PCIe Path on PCIe adapter so that PCIe adapter can send PCIe Tunneled packets
  //
  Status = PcieAdpEnablePath (Router, PcieAdpNum, TRUE);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }
  Status = PcieAdpEnablePath (DsRouter, DsRouter->PcieUpAdpNum, TRUE);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

Exit:
  DEBUG ((DEBUG_INFO, "Usb4RtConfigDsPciePath exit\n"));
  return Status;
}

/**
  Remove PCIe Tunneling that are configured for the connection between the Router and the downstream Router.
  Note that the downstream Router may have been disconnected.
  Don't try to access config space of downstream Router.

  @param[in] Router    - Pointer to Router instance.
  @param[in] AdpNum    - Adapter number of the Lane adapter to remove PCIe tunneling.

  @retval EFI_SUCCESS           - Remove PCIe Tunneling successfully.
  @retval EFI_UNSUPPORTED       - Remove PCIe Tunneling failure.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4RtRemoveDsPcie (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum
  )
{
  EFI_STATUS       Status;
  PUSB4_ROUTER     DsRouter;
  PUSB4_ADAPTER    LaneAdp;
  UINT8            PcieAdpNum;

  DEBUG ((DEBUG_INFO, "Usb4RtRemoveDsPcie entry - Router %p, AdpNum %d\n", Router, AdpNum));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4RtRemoveDsPcie: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "Usb4RtRemoveDsPcie: Invalid AdpNum %d!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  LaneAdp = &Router->Adapter[AdpNum];
  if (LaneAdp->Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "Usb4RtRemoveDsPcie: Adp %d is not Lane Adapter, Adapter type = %d\n", AdpNum, LaneAdp->Type));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  DsRouter = LaneAdp->AdpDep.Lane.DsRouter;
  if (DsRouter == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4RtRemoveDsPcie: Null DsRouter at Adp %d!\n", AdpNum));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Skip actions if PCIe tunneling is not enabled with downstream Router
  //
  if (DsRouter->State.PtOn == 0) {
    DEBUG ((DEBUG_INFO, "PCIe tunneling is not enabled with Rt 0x%016llX, skip actions\n", DsRouter->TopologyId));
    Status = EFI_SUCCESS;
    goto Exit;
  }

  PcieAdpNum = LaneAdp->AdpDep.Lane.PairedPcieAdp;

  //
  // Disable PCIe Path on PCIe adapter so that PCIe adapter stops sending PCIe Tunneled packets
  //
  Status = PcieAdpEnablePath (Router, PcieAdpNum, FALSE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Adp %d disable PCIe path failure, %r\n", Router->TopologyId, AdpNum, Status));
    goto Exit;
  }

  //
  // Tear down PCIe path between the Lane adapter reporting Router removal and its paired PCIe adapter.
  //
  Status = Usb4CsPathEnable (Router, PcieAdpNum, PCIE_ADAPTER_HOPID, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Adp %d disable path entry failure, %r\n", Router->TopologyId, PcieAdpNum, Status));
    goto Exit;
  }
  Status = Usb4CsPathEnable (Router, AdpNum, LANE_ADAPTER_PCIE_HOPID, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Adp %d disable path entry failure, %r\n", Router->TopologyId, AdpNum, Status));
    goto Exit;
  }

  //
  // Disable Extended Encapsulation on PCIe adapter
  //
  if (Router->Usb4Version == RT_USB4_VERSION_2) {
    Status = PcieAdpEnableExtendedEncapsulation (Router, PcieAdpNum, FALSE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Adp %d disable Pcie EE failure, %r\n", Router->TopologyId, AdpNum, Status));
      goto Exit;
    }
  }

  //
  // Clear PCIe Tunneling flag of downstream Router
  //
  DsRouter->State.PtOn = 0;

Exit:
  DEBUG ((DEBUG_INFO, "Usb4RtRemoveDsPcie exit\n"));
  return Status;
}