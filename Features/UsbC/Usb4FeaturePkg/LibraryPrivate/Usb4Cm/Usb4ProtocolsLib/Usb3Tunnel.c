/** @file
  USB3 tunneling implementation.

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
#include <Library/Usb4RouterLib.h>
#include <Library/Usb4DomainLib.h>
#include <Library/Usb4Capabilities.h>
#include "ProtocolConfig.h"

/**
  Set Path segment starting from USB3 adapter to Lane adapter.

  @param[in] Router  - Pointer to Router instance.
  @param[in] Ingress - Ingress adapter number.
  @param[in] Egress  - Egress adapter number.

  @retval EFI_SUCCESS           - Set path segment successfully.
  @retval EFI_UNSUPPORTED       - Fail to set path segment.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
SetPathUsb3ToLane (
  IN PUSB4_ROUTER    Router,
  IN UINT8           Ingress,
  IN UINT8           Egress
  )
{
  EFI_STATUS            Status;
  PUSB4_ADAPTER         Usb3Adp;
  PUSB4_ADAPTER         LaneAdp;
  USB4_PATH_CS_ENTRY    PathEntry;

  DEBUG ((DEBUG_INFO, "SetPathUsb3ToLane entry\n"));
  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "SetPathUsb3ToLane: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((Ingress == 0) || (Ingress > Router->MaxAdapter) || (Ingress >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "SetPathUsb3ToLane: Invalid ingress adp %d!\n", Ingress));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Usb3Adp = &(Router->Adapter[Ingress]);
  if ((Usb3Adp->Type != USB4_ADP_TYPE_USB3_US) && (Usb3Adp->Type != USB4_ADP_TYPE_USB3_DS)) {
    DEBUG ((DEBUG_ERROR, "SetPathUsb3ToLane: Ingress adapter is not USB3 adapter\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if ((Egress == 0) || (Egress > Router->MaxAdapter) || (Egress >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "SetPathUsb3ToLane: Invalid egress adp %d\n", Egress));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  LaneAdp = &Router->Adapter[Egress];

  if (LaneAdp->Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "SetPathUsb3ToLane: Egress adapter is not Lane adapter\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Router 0x%016llX USB3 = %d, Lane = %d\n", Router->TopologyId, Ingress, Egress));
  Status = Usb4CsPathRead (Router, Ingress, USB3_ADAPTER_HOPID, &PathEntry);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Read path cs (Adp %d, HopId %d) failure, %r\n", Ingress, USB3_ADAPTER_HOPID, Status));
    goto Exit;
  }

  //
  // Check the original path entry values - only for initial development phase
  //
  DEBUG ((DEBUG_INFO, "Original path entry\n"));
  DumpPathEntry (&PathEntry);

  PathEntry.PathCs0.Fields.OutputAdapter = (UINT32) Egress;
  PathEntry.PathCs0.Fields.OutputHopId   = LANE_ADAPTER_USB3_HOPID;
  PathEntry.PathCs0.Fields.Valid         = 1;
  PathEntry.PathCs1.Fields.Priority      = USB3_TO_LANE_PATH_PRIORITY;
  PathEntry.PathCs1.Fields.Weight        = USB3_TO_LANE_PATH_WEIGHT;
  PathEntry.PathCs1.Fields.Efc           = USB3_TO_LANE_PATH_EFC;
  PathEntry.PathCs1.Fields.Ese           = USB3_TO_LANE_PATH_ESE;
  DEBUG ((DEBUG_INFO, "Path BlockLow (Bit29) = %d\n", PathEntry.PathCs1.Fields.BlockLow));
  PathEntry.PathCs1.Fields.BlockLow      = 0;

  //
  // Required for TBT3 Router, N/A for USB4 Router
  //
  if (Router->State.Tbt3) {
    PathEntry.PathCs1.Fields.Ifc = USB3_TO_LANE_PATH_IFC;
    PathEntry.PathCs1.Fields.Ise = USB3_TO_LANE_PATH_ISE;
  } else {
    //
    // TBT team reference codes set this for USB4 Router though it's N/A
    // Need to clarify further in the future
    //
    PathEntry.PathCs1.Fields.Ifc = USB3_TO_LANE_PATH_IFC;
  }

  //
  // Write path entry for USB3 to Lane path segment
  //
  Status = Usb4CsPathWrite (Router, Ingress, USB3_ADAPTER_HOPID, &PathEntry);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Write path cs (Adapter=%d, HopId=%d) failure, %r\n", Ingress, USB3_ADAPTER_HOPID, Status));
    goto Exit;
  }

  //
  // only for initial development phase
  //
  Status = Usb4CsPathRead (Router, Ingress, USB3_ADAPTER_HOPID, &PathEntry);
  DEBUG ((DEBUG_INFO, "Updated path entry\n"));
  DumpPathEntry (&PathEntry);
Exit:
  DEBUG ((DEBUG_INFO, "SetPathUsb3ToLane exit\n"));
  return Status;
}

/**
  Credit number should be allocated for Lane to USB3 path segment

  @param[in] Router     - Pointer to Router instance.
  @param[in] Ingress    - Ingress adapter number.

  @retval Credit number should be allocated.
  @retval 0  - N/A for adapter other than Lane adapter
**/
UINT32
CreditsAllocatedForUsb3Path (
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
    DEBUG ((DEBUG_ERROR, "CreditsAllocatedForUsb3Path: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((Ingress == 0) || (Ingress > Router->MaxAdapter) || (Ingress >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "CreditsAllocatedForUsb3Path: Invalid ingress adp %d!\n", Ingress));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  LaneAdp = &(Router->Adapter[Ingress]);
  if ((LaneAdp->Type != USB4_ADP_TYPE_LANE) || (LaneAdp->AdpCapReg == 0)) {
    DEBUG ((DEBUG_ERROR, "Adp %d is not Lane adapter or Lane adapter capability is NULL.\n", Ingress));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Status = Usb4CsAdpRead (Router, Ingress, LaneAdp->AdpCapReg + USB4_CS_1, 1, (UINT32 *)&AdpCapCs1);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Query Lane Cap failure: Adapter %d at 0x%0x, %r\n", Ingress, LaneAdp->AdpCapReg, Status));
    goto Exit;
  }

  LinkWidth = AdpCapCs1.Fields.CurLinkWidth;
  LinkSpeed = AdpCapCs1.Fields.CurLinkSpeed;
  Credits = LANE_TO_USB3_PATH_CREDITS_LEGACY;

  if (Router->RouterType == USB4_ROUTER_MAPLE_RIDGE) {
    Credits = (LinkWidth == LANE_LINK_WIDTH_x2) ? LANE_TO_USB3_PATH_CREDITS_DTBT_X2 : LANE_TO_USB3_PATH_CREDITS_DTBT_X1;
  } else {
    if (LinkSpeed == LANE_LINK_SPEED_GEN3) {
      Credits = (LinkWidth == LANE_LINK_WIDTH_x2) ? LANE_TO_USB3_PATH_CREDITS_40G : LANE_TO_USB3_PATH_CREDITS_20G;
    } else if (LinkSpeed == LANE_LINK_SPEED_GEN2) {
      Credits = (LinkWidth == LANE_LINK_WIDTH_x2) ? LANE_TO_USB3_PATH_CREDITS_20G : LANE_TO_USB3_PATH_CREDITS_10G;
    }
  }
  DEBUG ((DEBUG_INFO, "LinkWidth = 0x%x, LinkSpeed = 0x%x, USB3 Credits = %d\n", LinkWidth, LinkSpeed, Credits));
Exit:
  return Credits;
}

/**
  Set Path segment starting from Lane adapter to USB3 adapter.

  @param[in] Router  - Pointer to Router instance.
  @param[in] Ingress - Ingress adapter number.
  @param[in] Egress  - Egress adapter number.

  @retval EFI_SUCCESS           - Set path segment successfully.
  @retval EFI_UNSUPPORTED       - Fail to set path segment.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
SetPathLaneToUsb3 (
  IN PUSB4_ROUTER    Router,
  IN UINT8           Ingress,
  IN UINT8           Egress
  )
{
  EFI_STATUS            Status;
  PUSB4_ADAPTER         Usb3Adp;
  PUSB4_ADAPTER         LaneAdp;
  USB4_PATH_CS_ENTRY    PathEntry;

  DEBUG ((DEBUG_INFO, "SetPathLaneToUsb3 entry\n"));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "SetPathLaneToUsb3: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((Ingress == 0) || (Ingress > Router->MaxAdapter) || (Ingress >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "SetPathLaneToUsb3: Invalid ingress adp %d!\n", Ingress));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  LaneAdp = &(Router->Adapter[Ingress]);
  if (LaneAdp->Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "SetPathLaneToUsb3: Ingress adapter is not Lane adapter\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if ((Egress == 0) || (Egress > Router->MaxAdapter) || (Egress >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "SetPathLaneToUsb3: Invalid egress adp %d\n", Egress));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Usb3Adp = &Router->Adapter[Egress];
  if ((Usb3Adp->Type != USB4_ADP_TYPE_USB3_US) && (Usb3Adp->Type != USB4_ADP_TYPE_USB3_DS)) {
    DEBUG ((DEBUG_ERROR, "SetPathLaneToUsb3: Egress adapter is not USB3 adapter\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if (Router->Adapter[Ingress].MaxInputHopId < LANE_ADAPTER_USB3_HOPID) {
    DEBUG ((DEBUG_ERROR, "Adapter MaxInputHopId %d is less than Lane adapter USB3 HopId\n", Router->Adapter[Ingress].MaxInputHopId));
    Status = EFI_UNSUPPORTED;
    ASSERT (0);
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Router 0x%016llX - Lane = %d, USB3 = %d, Input HopId = %d\n", Router->TopologyId, Ingress, Egress, LANE_ADAPTER_USB3_HOPID));

  Status = Usb4CsPathRead (Router, Ingress, LANE_ADAPTER_USB3_HOPID, &PathEntry);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Read path cs (Adp %d, HopId %d) failure, %r\n", Ingress, LANE_ADAPTER_USB3_HOPID, Status));
    goto Exit;
  }

  //
  // only for initial development phase
  //
  DEBUG ((DEBUG_INFO, "Original path entry\n"));
  DumpPathEntry (&PathEntry);

  PathEntry.PathCs0.Fields.OutputAdapter    = (UINT32) Egress;
  PathEntry.PathCs0.Fields.OutputHopId      = USB3_ADAPTER_HOPID;
  PathEntry.PathCs0.Fields.Valid            = 1;
  PathEntry.PathCs1.Fields.Priority         = LANE_TO_USB3_PATH_PRIORITY;
  PathEntry.PathCs1.Fields.Weight           = LANE_TO_USB3_PATH_WEIGHT;
  PathEntry.PathCs1.Fields.Ifc              = LANE_TO_USB3_PATH_IFC;
  PathEntry.PathCs1.Fields.Ise              = LANE_TO_USB3_PATH_ISE;
  PathEntry.PathCs0.Fields.CreditsAllocated = CreditsAllocatedForUsb3Path (Router, Ingress);
  DEBUG ((DEBUG_INFO, "Path BlockLow (Bit29) = %d\n", PathEntry.PathCs1.Fields.BlockLow));
  PathEntry.PathCs1.Fields.BlockLow         = 0;

  //
  // Required for TBT3 Router, N/A for USB4 Router
  //
  if (Router->State.Tbt3) {
    PathEntry.PathCs1.Fields.Efc = LANE_TO_USB3_PATH_EFC;
    PathEntry.PathCs1.Fields.Ese = LANE_TO_USB3_PATH_ESE;
  }

  Status = Usb4CsPathWrite (Router, Ingress, LANE_ADAPTER_USB3_HOPID, &PathEntry);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Write path cs (Adapter=%d, HopId=%d) failure, %r\n", Ingress, LANE_ADAPTER_USB3_HOPID, Status));
    goto Exit;
  }

  //
  // only for initial development phase
  //
  Status = Usb4CsPathRead (Router, Ingress, LANE_ADAPTER_USB3_HOPID, &PathEntry);
  DEBUG ((DEBUG_INFO, "Updated path entry\n"));
  DumpPathEntry (&PathEntry);
Exit:
  DEBUG ((DEBUG_INFO, "SetPathLaneToUsb3 exit\n"));
  return Status;
}

/**
  Enable USB3 path on USB3 adapter

  @param[in] Router        - Pointer to Router instance.
  @param[in] Usb3AdpNum    - USB3 adapter number.
  @param[in] PathEnable    - TRUE = Enable path, FALSE = Disable path.

  @retval EFI_SUCCESS           - Enable USB3 path success.
  @retval EFI_UNSUPPORTED       - Fail to enable path segment.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb3AdpEnablePath (
  IN PUSB4_ROUTER    Router,
  IN UINT8           Usb3AdpNum,
  IN BOOLEAN         PathEnable
  )
{
  EFI_STATUS           Status;
  PUSB4_ADAPTER        Usb3Adapter;
  USB4_ADP_CAP_USB3    Usb3CapCs;

  DEBUG ((DEBUG_INFO, "Usb3AdpEnablePath entry\n"));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb3AdpEnablePath: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((Usb3AdpNum == 0) || (Usb3AdpNum > Router->MaxAdapter) || (Usb3AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "Usb3AdpEnablePath: Invalid AdpNum %d!\n", Usb3AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Router 0x%016llX USB3 adapter %d Enable = %d\n", Router->TopologyId, Usb3AdpNum, PathEnable));

  Usb3Adapter = &Router->Adapter[Usb3AdpNum];
  if ((Usb3Adapter->Type != USB4_ADP_TYPE_USB3_US) && (Usb3Adapter->Type != USB4_ADP_TYPE_USB3_DS)) {
    DEBUG ((DEBUG_ERROR, "Adp %d is not USB3 adapter, adapter type = 0x%08x\n", Usb3AdpNum, Usb3Adapter->Type));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if (Usb3Adapter->AdpCapReg == 0) {
    DEBUG ((DEBUG_ERROR, "Usb3AdpEnablePath: USB3 Adapter Capability is not present\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Status = Usb4CsAdpRead (Router, Usb3AdpNum, Usb3Adapter->AdpCapReg, sizeofdw (USB4_ADP_CAP_USB3), (UINT32 *) &Usb3CapCs);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Read USB3 adapter capability at 0x%0X failure , %r\n", Usb3Adapter->AdpCapReg, Status));
    goto Exit;
  }
  Usb3CapCs.CapCs0.Fields.PathEnable = (PathEnable) ? 1 : 0;
  Usb3CapCs.CapCs0.Fields.Valid = 1;
  Status = Usb4CsAdpWrite (Router, Usb3AdpNum, Usb3Adapter->AdpCapReg, sizeofdw (USB4_ADP_CAP_USB3), (UINT32 *) &Usb3CapCs);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Write USB3 adapter capability at 0x%0X failure , %r\n", Usb3Adapter->AdpCapReg, Status));
    goto Exit;
  }

  //
  // Read back the value for checking - initial development phase only
  //
  Status = Usb4CsAdpWrite (Router, Usb3AdpNum, Usb3Adapter->AdpCapReg, sizeofdw (USB4_ADP_CAP_USB3), (UINT32 *) &Usb3CapCs);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Read USB3 adapter capability at 0x%0X failure , %r\n", Usb3Adapter->AdpCapReg, Status));
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "USB3 Path Enable bit = %d, Valid bit = %d\n", Usb3CapCs.CapCs0.Fields.PathEnable, Usb3CapCs.CapCs0.Fields.Valid));

  Usb3Adapter->AdpDep.AdpUsb3.Status.PathEnable = (PathEnable) ? 1 : 0;

Exit:
  DEBUG ((DEBUG_INFO, "Usb3AdpEnablePath exit\n"));
  return Status;
}


/**
  Set the allocated bandwidth for the USB3 traffic.
  Refer to USB4 spec 9.1.2 for more details.

  @param[in] Router       - Pointer to the Host Router instance.
  @param[in] AdpNum       - USB3 adapter number for bandwidth allocation.
  @param[in] UsBandwidth  - The amount of allocated upstream bandwidth. (in 512 * 2Scale bytes)
  @param[in] DsBandwidth  - The amount of allocated downstream bandwidth. (in 512 * 2Scale bytes)

  @retval EFI_SUCCESS           - Set the allocated bandwidth successfully.
  @retval EFI_UNSUPPORTED       - Fail to set the allocated bandwidth.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
HrUsb3BandwidthUpdate (
  IN USB4_ROUTER     *Router,
  IN UINT8           AdpNum,
  IN UINT32          UsBandwidth,
  IN UINT32          DsBandwidth
  )
{
  EFI_STATUS           Status;
  UINT16               AdpCapReg;
  UINT32               RetryCount;
  USB4_ADP_CAP_USB3    Usb3Cap;

  DEBUG ((DEBUG_INFO, "HrUsb3BandwidthUpdate entry\n"));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "HrUsb3BandwidthUpdate: Null router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  //
  // This only applies to the Host Router
  //
  if (Router->TopologyId.Value != 0) {
    DEBUG ((DEBUG_ERROR, "HrUsb3BandwidthUpdate: Rt 0x%016llX is not host router\n", Router->TopologyId));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "HrUsb3BandwidthUpdate: Invalid adapter number %d\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  if (Router->Adapter[AdpNum].Type != USB4_ADP_TYPE_USB3_DS) {
    DEBUG ((DEBUG_ERROR, "HrUsb3BandwidthUpdate: Adp %d is not USB3 downstream adapter\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Set the allocated bandwidth for each USB3 downstream adapter.
  //
  AdpCapReg = Router->Adapter[AdpNum].AdpCapReg;
  if (AdpCapReg == 0) {
    DEBUG ((DEBUG_ERROR, "HrUsb3BandwidthUpdate: Adp %d null AdpCapReg!\n", AdpNum));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Status = Usb4CsAdpRead (Router, AdpNum, AdpCapReg, sizeofdw (Usb3Cap), (UINT32 *) &Usb3Cap);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Read USB3 adapter %d capability at 0x%0X failure , %r\n", AdpNum, AdpCapReg, Status));
    goto Exit;
  }
  DEBUG ((DEBUG_INFO, "USB3 Adp %d Cap - Original Valid = %d, PathEnable = %d, HcAck = %d, Cmr = %d, UsBW = 0x%x, DsBW = 0x%x\n",
          AdpNum,
          Usb3Cap.CapCs0.Fields.Valid,
          Usb3Cap.CapCs0.Fields.PathEnable,
          Usb3Cap.CapCs1.Fields.HcAck,
          Usb3Cap.CapCs2.Fields.CmRequest,
          Usb3Cap.CapCs2.Fields.UsBwAllocated,
          Usb3Cap.CapCs2.Fields.DsBwAllocated
        ));
  //
  // Set the allocated bandwidth only for the USB3 adapter that enable USB3 tunneling path.
  //
  if ((Usb3Cap.CapCs0.Fields.Valid == 0) || (Usb3Cap.CapCs0.Fields.PathEnable == 0)) {
    DEBUG ((DEBUG_ERROR, "HrUsb3BandwidthUpdate: HR Adp %d USB3 cap valid bit or path enable is not set\n", AdpNum));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if (Usb3Cap.CapCs1.Fields.HcAck == 1) {
    //
    // This should not happen before CM sets CMR to 1
    //
    DEBUG ((DEBUG_ERROR, "HcAck has been set to 1 before setting CMR to 1!\n"));
    ASSERT (1);
  }

  //
  // Both the internal Host Controller and Connection Manager read/write the consumed/allocated bandwidth.
  // CMR and HcAck are used as mutex mechanism for mutual access to the consumed/allocated bandwidth
  // When HcAck == 0, the internal Host Controller owns the access permission.
  // When HcAck == 1, Connection Manager owns the access permission.
  // HcAck is set to 1 by setting CMR to 1.
  //
  DEBUG ((DEBUG_INFO, "Set USB3 Adp %d CMR to 1\n", AdpNum));

  //
  // Write CMR to 1 and wait for HcAck being set to 1 by the internal Host Controller
  //
  Usb3Cap.CapCs2.Fields.CmRequest = 1;
  Status = Usb4CsAdpWrite (Router, AdpNum, AdpCapReg + USB4_CS_2, 1, (UINT32 *)&(Usb3Cap.CapCs2));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Write USB3 Adp %d Cap CS2 at Cap 0x%0X failure , %r\n", AdpNum, AdpCapReg, Status));
    goto Exit;
  }

  //
  // Wait for HcAck before setting the allocated bandwidth
  //
  for (RetryCount = 0; RetryCount < CM_STATUS_WAITS_MAX; RetryCount++) {
    Status = Usb4CsAdpRead (Router, AdpNum, AdpCapReg + USB4_CS_1, 1, (UINT32 *)&(Usb3Cap.CapCs1));
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Read USB3 Adp %d Cap CS1 at Cap 0x%0X failure , %r\n", AdpNum, AdpCapReg, Status));
      goto Exit;
    }
    if (Usb3Cap.CapCs1.Fields.HcAck) {
      DEBUG ((DEBUG_INFO, "HR USB3 Adp %d received HcAck! Retry = %d\n", AdpNum, RetryCount));
      break;
    }
    gBS->Stall (CM_STATUS_POLL_US);
  }

  if (Usb3Cap.CapCs1.Fields.HcAck == 0) {
    DEBUG ((DEBUG_ERROR, "HR USB3 Adp %d waits for HcAck timeout!\n", AdpNum));
    Status = EFI_TIMEOUT;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Set USB3 Adp %d UsBwAllocated = %d, DsBandwidth = %d\n", AdpNum, UsBandwidth, DsBandwidth));
  //
  // Set allocated bandwidth for USB3 isochronous traffic.
  //
  Usb3Cap.CapCs2.Fields.UsBwAllocated = UsBandwidth;
  Usb3Cap.CapCs2.Fields.DsBwAllocated = DsBandwidth;
  Status = Usb4CsAdpWrite (Router, AdpNum, AdpCapReg + USB4_CS_2, 1, (UINT32 *)&(Usb3Cap.CapCs2));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Write USB3 Adp %d Cap CS2 at Cap 0x%0X failure , %r\n", AdpNum, AdpCapReg, Status));
    goto Exit;
  }

  //
  // Set Connection Manager Request to 0 after update complete
  //
  Usb3Cap.CapCs2.Fields.CmRequest = 0;
  Status = Usb4CsAdpWrite (Router, AdpNum, AdpCapReg + USB4_CS_2, 1, (UINT32 *)&(Usb3Cap.CapCs2));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Write USB3 Adp %d Cap CS2 at Cap 0x%0X failure , %r\n", AdpNum, AdpCapReg, Status));
    goto Exit;
  }

  //
  // Initial development phase only
  // Read back the USB3 Capability
  //
  Status = Usb4CsAdpRead (Router, AdpNum, AdpCapReg, sizeofdw(Usb3Cap), (UINT32 *)&Usb3Cap);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Read USB3 adapter %d capability at 0x%0X failure , %r\n", AdpNum, AdpCapReg, Status));
    goto Exit;
  }
  DEBUG ((DEBUG_INFO, "USB3 Adp %d Cap - Valid = %d, HcAck = %d, UsBW(C) = %d, DsBW(C) = %d, Cmr = %d, UsBW(A) = 0x%x, DsBW(A) = 0x%x\n",
          AdpNum,
          Usb3Cap.CapCs0.Fields.Valid,
          Usb3Cap.CapCs1.Fields.HcAck,
          Usb3Cap.CapCs1.Fields.UsBwConsumed,
          Usb3Cap.CapCs1.Fields.DsBwConsumed,
          Usb3Cap.CapCs2.Fields.CmRequest,
          Usb3Cap.CapCs2.Fields.UsBwAllocated,
          Usb3Cap.CapCs2.Fields.DsBwAllocated
        ));

Exit:
  DEBUG ((DEBUG_INFO, "HrUsb3BandwidthUpdate exit\n"));
  return Status;
}

/**
  Update USB3 bandwidth allocation for new added path

  @param[in] UpperRt     - Pointer to upper router instance of the path.
  @param[in] UpperAdpNum - Downstream USB3 adapter number of upper router in the path.
  @param[in] LowerRt     - Pointer to lower router instance of the path.
  @param[in] LowerAdpNum - Upstream USB3 adapter number of lower router in the path.

  @retval EFI_SUCCESS           - Update USB3 bandwidth allocation successfully.
  @retval EFI_UNSUPPORTED       - Fail to update USB3 bandwidth allocation.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
*/
EFI_STATUS
Usb3BandwidthUpdateForPath (
  IN USB4_ROUTER    *UpperRt,
  IN UINT8          UpperAdpNum,
  IN USB4_ROUTER    *LowerRt,
  IN UINT8          LowerAdpNum
)
{
  EFI_STATUS      Status;
  USB4_ADAPTER    *UpperUsb3Adp;
  USB4_ADAPTER    *LowerUsb3Adp;
  USB4_ROUTER     *RootRt;
  TOPOLOGY_ID     TopologyId;
  UINT8           HrDsUsb3AdpNum;
  UINT8           HrDsLaneAdpNum;

  DEBUG ((DEBUG_INFO, "Usb3BandwidthUpdateForPath entry\n"));
  if ((UpperRt == NULL) || (LowerRt == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb3BandwidthUpdateForPath: Invalid parameter (%p, %p)\n", UpperRt, LowerRt));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((UpperAdpNum > UpperRt->MaxAdapter) || (UpperAdpNum >= USB4_ADAPTER_SUPPORT_MAX) ||
      (LowerAdpNum > LowerRt->MaxAdapter) || (LowerAdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "Usb3BandwidthUpdateForPath: Invalid adapter number (%d, %d)\n", UpperAdpNum, LowerAdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  UpperUsb3Adp = &(UpperRt->Adapter[UpperAdpNum]);
  LowerUsb3Adp = &(LowerRt->Adapter[LowerAdpNum]);
  if ((UpperUsb3Adp->Type != USB4_ADP_TYPE_USB3_DS) || (LowerUsb3Adp->Type != USB4_ADP_TYPE_USB3_US)) {
    DEBUG ((DEBUG_ERROR, "Usb3BandwidthUpdateForPath: Invalid adapter type for bandwidth allocation update\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Update USB3 bandwidth for Rt 0x%016llX Adp %d <-> Rt 0x%016llX Adp %d\n", UpperRt->TopologyId, UpperAdpNum, LowerRt->TopologyId, LowerAdpNum));

  //
  // Find the host router's downstream USB3 adapter which this path belongs to.
  //
  if (UpperRt->Depth == 0) {
    RootRt = UpperRt;
    HrDsUsb3AdpNum = UpperAdpNum;
  } else {
    //
    // Get host router instance from domain.
    //
    TopologyId.Value = 0;
    Status = Usb4DomainGetRouter (UpperRt->DomainContext, &TopologyId, &RootRt);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Usb3BandwidthUpdateForPath: Fail to get host router instance in domain, %r\n", Status));
      goto Exit;
    }
    //
    // Get the downstream lane adapter of host router which this path belongs to.
    //
    HrDsLaneAdpNum = (UINT8) LowerRt->TopologyId.Bits.Level0Adapter;
    if ((HrDsLaneAdpNum == 0) || (HrDsLaneAdpNum > RootRt->MaxAdapter) || (HrDsLaneAdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
      DEBUG ((DEBUG_ERROR, "Usb3BandwidthUpdateForPath: Invalid adapter number %d for host router\n", HrDsLaneAdpNum));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
    if (RootRt->Adapter[HrDsLaneAdpNum].Type != USB4_ADP_TYPE_LANE) {
      DEBUG ((DEBUG_ERROR, "Usb3BandwidthUpdateForPath: Adp %d is not lane adapter in HR\n", HrDsLaneAdpNum));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
    //
    // Get the paired USB3 adapter of the host router downstream lane adapter.
    //
    HrDsUsb3AdpNum = UpperRt->Adapter[HrDsLaneAdpNum].AdpDep.Lane.PairedUsbAdp;
    if (HrDsLaneAdpNum > RootRt->MaxAdapter) {
      DEBUG ((DEBUG_ERROR, "Usb3BandwidthUpdateForPath: Invalid adapter number %d for host router\n", HrDsLaneAdpNum));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
    if (RootRt->Adapter[HrDsLaneAdpNum].Type != USB4_ADP_TYPE_LANE) {
      DEBUG ((DEBUG_ERROR, "Usb3BandwidthUpdateForPath: Adp %d is not lane adapter in HR\n", HrDsLaneAdpNum));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
    DEBUG ((DEBUG_INFO, "USB3 path is under HR Lane Adp %d USB3 Adp %d\n", HrDsLaneAdpNum, HrDsUsb3AdpNum));
  }

  //
  // Update USB3 bandwidth allocation in host router's downstream USB3 adapter.
  //
  Status = HrUsb3BandwidthUpdate (RootRt, HrDsUsb3AdpNum, USB3_ALLOCATED_BW, USB3_ALLOCATED_BW);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb3BandwidthUpdateForPath: Fail to update USB3 bandwidth allocation, %r\n", Status));
    goto Exit;
  }

Exit:
  DEBUG ((DEBUG_INFO, "Usb3BandwidthUpdateForPath exit\n"));
  return Status;
}

/**
  Setup USB3 path between router's USB3 downstream adapter and downstream router's USB3 upstream adapter.

  @param[in] Router    - Pointer to Router instance.
  @param[in] AdpNum    - Downstream Lane adapter number that the downstream Router is connecting.

  @retval EFI_SUCCESS           - Config USB3 path successfully.
  @retval EFI_UNSUPPORTED       - Fail to config USB3 path.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4RtConfigDsUsb3Path (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum
  )
{
  EFI_STATUS       Status;
  PUSB4_ROUTER     DsRouter;
  PUSB4_ADAPTER    LaneAdp;
  UINT8            Usb3AdpNum;

  DEBUG ((DEBUG_INFO, "Usb4RtConfigDsUsb3Path entry\n"));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4RtConfigDsUsb3Path: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "Usb4RtConfigDsUsb3Path: Invalid AdpNum %d!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  LaneAdp = &Router->Adapter[AdpNum];
  if (LaneAdp->Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "Usb4RtConfigDsUsb3Path: Adp %d is not Lane Adapter, Adapter type = %d\n",AdpNum, LaneAdp->Type));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  DsRouter = LaneAdp->AdpDep.Lane.DsRouter;
  if (DsRouter == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4RtConfigDsUsb3Path: Null DsRouter at Adp %d!\n", AdpNum));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Paired USB3 downstream adapter of the downstream Lane adapter.
  //
  Usb3AdpNum = LaneAdp->AdpDep.Lane.PairedUsbAdp;

  //
  // Setup Outbond path
  //
  DEBUG ((DEBUG_INFO, "Set outbond path : Router 0x%016llX to Router 0x%016llX\n", Router->TopologyId, DsRouter->TopologyId));
  Status = SetPathUsb3ToLane (Router, Usb3AdpNum, AdpNum);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }
  Status = SetPathLaneToUsb3 (DsRouter, DsRouter->LaneUpAdpNum, DsRouter->Usb3UpAdpNum);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  //
  // Setup Inbond path
  //
  DEBUG ((DEBUG_INFO, "Set inbond path : Router 0x%016llX to Router 0x%016llX\n", DsRouter->TopologyId, Router->TopologyId));
  Status = SetPathUsb3ToLane (DsRouter, DsRouter->Usb3UpAdpNum, DsRouter->LaneUpAdpNum);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }
  Status = SetPathLaneToUsb3 (Router, AdpNum, Usb3AdpNum);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  //
  // Enable USB3 Path on USB3 adapter so that USB3 adapter can send USB3 Tunneled packets
  //
  Status = Usb3AdpEnablePath (Router, Usb3AdpNum, TRUE);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }
  Status = Usb3AdpEnablePath (DsRouter, DsRouter->Usb3UpAdpNum, TRUE);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  //
  // Update USB3 bandwidth allocation
  //
  Status = Usb3BandwidthUpdateForPath (Router, Usb3AdpNum, DsRouter, DsRouter->Usb3UpAdpNum);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }
Exit:
  DEBUG ((DEBUG_INFO, "Usb4RtConfigDsUsb3Path exit\n"));
  return Status;
}

/**
  Remove USB3 Tunneling that are configured for the connection between the Router and the downstream Router.
  Note that the downstream Router may have been disconnected.
  Don't try to access config space of downstream Router.

  @param[in] Router    - Pointer to Router instance.
  @param[in] AdpNum    - Adapter number of the Lane adapter to remove USB3 tunneling.

  @retval EFI_SUCCESS           - Remove USB3 Tunneling successfully.
  @retval EFI_UNSUPPORTED       - Remove USB3 Tunneling failure.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4RtRemoveDsUsb3 (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum
  )
{
  EFI_STATUS       Status;
  PUSB4_ROUTER     DsRouter;
  PUSB4_ADAPTER    LaneAdp;
  UINT8            Usb3AdpNum;

  DEBUG ((DEBUG_INFO, "Usb4RtRemoveDsUsb3 entry - Router %p, AdpNum %d\n", Router, AdpNum));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4RtRemoveDsUsb3: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "Usb4RtRemoveDsUsb3: Invalid AdpNum %d!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  LaneAdp = &Router->Adapter[AdpNum];
  if (LaneAdp->Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "Usb4RtRemoveDsUsb3: Adp %d is not Lane Adapter, Adapter type = %d\n",AdpNum, LaneAdp->Type));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  DsRouter = LaneAdp->AdpDep.Lane.DsRouter;
  if (DsRouter == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4RtRemoveDsUsb3: Null DsRouter at Adp %d!\n", AdpNum));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Skip actions if USB3 tunneling is not enabled with downstream Router
  //
  if (DsRouter->State.UtOn == 0) {
    DEBUG ((DEBUG_INFO, "USB3 tunneling is not enabled with Rt 0x%016llX, skip actions\n", DsRouter->TopologyId));
    Status = EFI_SUCCESS;
    goto Exit;
  }

  Usb3AdpNum = LaneAdp->AdpDep.Lane.PairedUsbAdp;

  //
  // Disable USB3 Path on USB3 adapter so that USB3 adapter stops sending USB3 Tunneled packets
  //
  Status = Usb3AdpEnablePath (Router, Usb3AdpNum, FALSE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Adp %d disable USB3 path failure, %r\n", Router->TopologyId, AdpNum, Status));
    goto Exit;
  }

  //
  // Tear down USB3 path between the Lane adapter reporting Router removal and its paired USB3 adapter.
  //
  Status = Usb4CsPathEnable (Router, Usb3AdpNum, USB3_ADAPTER_HOPID, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Adp %d disable path entry failure, %r\n", Router->TopologyId, Usb3AdpNum, Status));
    goto Exit;
  }
  Status = Usb4CsPathEnable (Router, AdpNum, LANE_ADAPTER_USB3_HOPID, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX Adp %d disable path entry failure, %r\n", Router->TopologyId, AdpNum, Status));
    goto Exit;
  }

  //
  // Clear USB3 Tunneling flag of downstream Router
  //
  DsRouter->State.UtOn = 0;

Exit:
  DEBUG ((DEBUG_INFO, "Usb4RtRemoveDsUsb3 exit\n"));
  return Status;
}