/** @file
  USB4 Hot plug event handler implementation.

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
#include <Library/Usb4DomainLib.h>
#include <Library/Usb4RouterLib.h>
#include <Library/Usb4HiCoreLib.h>
#include <Library/Tbt3SupportLib.h>

/**
  USB4 port hot plug event handler.

  @param[in] Router  - Pointer to Router instance.
  @param[in] AdpNum  - Adapter number of hot plug event.
  @param[in] Unplug  - Indicate unplug event or plug event.

  @retval EFI_SUCCESS           - USB4 port hot plug process success.
  @retval EFI_UNSUPPORTED       - USB4 port hot plug process failure.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
CmUsb4HotPlugHandler (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum,
  IN UINT8           Unplug
  )
{
  EFI_STATUS       Status;
  PUSB4_ROUTER     DsRouter;
  PUSB4_ADAPTER    LaneAdp;

  Status = EFI_SUCCESS;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "CmUsb4HotPlugHandler: Null Router!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "CmUsb4HotPlugHandler: Invalid AdpNum %d!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "CmUsb4HotPlugHandler - Rt 0x%016llX Adp %d Unplug %d\n", Router->TopologyId, AdpNum, Unplug));

  LaneAdp = &(Router->Adapter[AdpNum]);
  if (LaneAdp->Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_INFO, "CmUsb4HotPlugHandler: Rt 0x%016llX Adp %d is not Lane adp!\n", Router->TopologyId, AdpNum));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Ignore hot plug event if the adapter is upstream lane adapter or is not a Lane0 adapter.
  //
  if ((LaneAdp->AdpDep.Lane.Status.Lane0 == 0) || (AdpNum == Router->LaneUpAdpNum)) {
    DEBUG ((DEBUG_INFO, "Rt 0x%016llX Adp %d is not Lane0 Adp or is Upstream Adp, ignore hotplug event\n", Router->TopologyId, AdpNum));
    goto Exit;
  }

  DsRouter = Router->Adapter[AdpNum].AdpDep.Lane.DsRouter;

  if (Unplug == 0) {
    //
    // Hot plug event
    //
    if (DsRouter == NULL) {
      DEBUG ((DEBUG_INFO, "Enumerate new Router at Rt 0x%016llX Adp %d\n", Router->TopologyId, AdpNum));
      //
      // Enumerate and create downstream Router and its downstream Routers.
      //
      Status = Usb4RtEnumDsRouter (Router, AdpNum);
      if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Enum Rt 0x%016llX Ds Rt tree at adp %d error, %r\n", Router->TopologyId, AdpNum, Status));
          goto Exit;
      }
    } else {
      DEBUG ((DEBUG_INFO, "Duplicated hot plug event for Rt 0x%016llX Adp %d\n", Router->TopologyId, AdpNum));
      //
      // Duplicated hot plug event, just ignore it
      //
      goto Exit;
    }
  } else {
    //
    // Unplug Event
    //
    if (DsRouter != NULL) {
      DEBUG ((DEBUG_INFO, "Remove a Router from Rt 0x%016llX Adp %d\n", Router->TopologyId, AdpNum));
      //
      // Remove and destroy Router and its downstream Routers from Domain.
      //
      Status = Usb4RtDsDisconnect (Router, AdpNum);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Remove Rt 0x%016llX from Domain failure, %r\n", Router->TopologyId, Status));
        goto Exit;
      }
    } else {
      //
      // Duplicated unplug event, just ignore it
      //
      DEBUG ((DEBUG_INFO, "No Router connecting to Rt 0x%016llX Adp %d in Domain!\n", Router->TopologyId, AdpNum));
      goto Exit;
    }
  }
Exit:
  return Status;
}

/**
  DP adapter hot plug event handler.

  @param[in] Router  - Pointer to Router instance.
  @param[in] AdpNum  - Adapter number of hot plug event.
  @param[in] Unplug  - Indicate unplug event or plug event.

  @retval EFI_SUCCESS           - DP hotplug process success.
  @retval EFI_UNSUPPORTED       - DP hotplug process failure.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/

EFI_STATUS
CmDpHotPlugHandler (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum,
  IN UINT8           Unplug
  )
{

  EFI_STATUS       Status;
  PUSB4_ADAPTER    DpAdp;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "CmDpHotPlugHandler: Null Router!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "CmDpHotPlugHandler: Invalid AdpNum %d!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DpAdp = &(Router->Adapter[AdpNum]);
  if ((DpAdp->Type != USB4_ADP_TYPE_DP_IN) && (DpAdp->Type != USB4_ADP_TYPE_DP_OUT)) {
    DEBUG ((DEBUG_ERROR, "CmDpHotPlugHandler: Rt 0x%016llX Adp %d is not DP adapter\n", Router->TopologyId, AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "CmDpHotPlugHandler - Rt 0x%016llX Adp %d Unplug %d\n", Router->TopologyId, AdpNum, Unplug));

  if (Unplug == 0) {
    if (DpAdp->Type == USB4_ADP_TYPE_DP_IN) {
      Status = Usb4DomainDpSrcAdpAdd (Router->DomainContext, Router, AdpNum);
    } else {
      Status = Usb4DomainDpSinkAdpAdd (Router->DomainContext, Router, AdpNum);
    }
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "CmDpHotPlugHandler: Add DP source/sink failure, %r\n", Status));
      goto Exit;
    }
  } else {
    if (DpAdp->Type == USB4_ADP_TYPE_DP_IN) {
      Status = Usb4DomainDpSrcAdpRemove (Router->DomainContext, Router, AdpNum);
    } else {
      Status = Usb4DomainDpSinkAdpRemove (Router->DomainContext, Router, AdpNum);
    }
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "CmDpHotPlugHandler: Remove DP source/sink failure, %r\n", Status));
      goto Exit;
    }
  }

Exit:
  return Status;
}

/**
  Hot plug event handler

  @param[in] Usb4Hr   - Pointer to USB4 host router instance.
  @param[in] RxEvent  - Pointer to Rx event.

  @retval EFI_SUCCESS           - Hot plug process success.
  @retval EFI_UNSUPPORTED       - Hot plug process failure.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
CmHotPlugHandler (
  IN USB4_HR_INSTANCE    *Usb4Hr,
  IN PRX_EVENT           RxEvent
  )
{
  EFI_STATUS       Status;
  PUSB4_ROUTER     Router;
  PUSB4_ADAPTER    Adapter;
  UINT8            AdpNum;
  BOOLEAN          ConnectXhc;

  if ((Usb4Hr == NULL) || (RxEvent == NULL)) {
    DEBUG ((DEBUG_ERROR, "CmHotPlugHandler: Invalid parameter (%p, %p)\n", Usb4Hr, RxEvent));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (RxEvent->Pdf != PDF_HOT_PLUG) {
    DEBUG ((DEBUG_ERROR, "CmHotPlugHandler: Not a hot plug event, pdf = %d\n", RxEvent->Pdf));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Status = Usb4DomainGetRouter ((USB4_DOMAIN_CONTEXT) Usb4Hr->Usb4Domain, &(RxEvent->TopologyId), &Router);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "CmHotPlugHandler: Fail to get Rt 0x%016llX in domain, %r\n", RxEvent->TopologyId, Status));
    goto Exit;
  }

  AdpNum = RxEvent->AdpNum;
  if ((AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "CmHotPlugHandler: ApdNum %d > Max Adapter number %d\n", AdpNum, Router->MaxAdapter));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // <TBT3>
  // Refer to 4.1.2 xHC enablement/4.1.2.1 AR TBT3 Router of CM Thunderbolt 3 Device Support rev 0.4
  //
  if (AdpNum == 0) {
    if (Router->RouterType == USB4_ROUTER_ALPINE_RIDGE) {
      ConnectXhc = (RxEvent->Unplug) ? FALSE : TRUE;
      Status = Tbt3ArConnectXhc (Router, ConnectXhc);
    } else {
      DEBUG ((DEBUG_ERROR, "CmHotPlugHandler: AdpNum 0 hot plug event is used by AlpineRidge only\n"));
      Status = EFI_UNSUPPORTED;
    }
  } else {
    Adapter = &(Router->Adapter[AdpNum]);
    if (Adapter->Type == USB4_ADP_TYPE_LANE) {
      Status = CmUsb4HotPlugHandler (Router, AdpNum, RxEvent->Unplug);
    } else if ((Adapter->Type == USB4_ADP_TYPE_DP_IN) || (Adapter->Type == USB4_ADP_TYPE_DP_OUT)) {
      Status = CmDpHotPlugHandler (Router, AdpNum, RxEvent->Unplug);
    } else {
      DEBUG ((DEBUG_ERROR, "CmHotPlugHandler: Hot plug is not supported for adapter type 0x%08x\n", Adapter->Type));
      Status = EFI_UNSUPPORTED;
    }
  }

Exit:
  return Status;
}
