/** @file
  Protocol tunneling implementation for USB4 routers.

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
#include <Library/Usb4RouterLib.h>
#include <Library/Tbt3SupportLib.h>
#include <Library/Usb4RouterDefs.h>
#include "Usb4Protocols.h"
#include "ProtocolConfig.h"

/**
  Enable Protocol Tunneling for the downstream Router.

  @param[in] Router - Pointer to Router instance.
  @param[in] AdpNum - Downstream Lane adapter number.

  @retval EFI_SUCCESS           - Enable Protocol Tunneling successfully.
  @retval EFI_UNSUPPORTED       - Fail to enable protocol tunneling.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4RtEnableDsProtocols (
  PUSB4_ROUTER    Router,
  UINT8           AdpNum
  )
{
  EFI_STATUS       Status;
  PUSB4_ROUTER     DsRouter;
  PUSB4_ADAPTER    LaneAdp;

  DEBUG ((DEBUG_INFO, "Usb4RtEnableDsProtocols entry\n"));

  Status = EFI_SUCCESS;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4RtEnableDsProtocols: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "Usb4RtEnableDsProtocols: Invalid AdpNum %d!\n", AdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  LaneAdp = &Router->Adapter[AdpNum];
  if (LaneAdp->Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "Usb4RtEnableDsProtocols: Adp %d is not Lane Adapter, Adapter type = %d\n",AdpNum, LaneAdp->Type));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  DsRouter = LaneAdp->AdpDep.Lane.DsRouter;
  if (DsRouter == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4RtEnableDsProtocols: Null DsRouter at Adp %d!\n", AdpNum));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  DEBUG ((CM_DEBUG, "Enable protocol tunneling for downstream router 0x%016llX\n", DsRouter->TopologyId));
  // <TBT3>
  // TBT3 doesn't define router CS 5/6.
  // PCIe tunneling is enabled by default.
  //
  if (DsRouter->State.Tbt3 == 0) {
    //
    // Enable USB Tunneling of downstream Router if the following conditions are true:
    // 1. USB3 Tunneling is enabled in parent Router.
    // 2. USB3 upstream adapter is present in downstream Router.
    // 3. The downstream Lane adapter is paired with an USB3 adapter
    //
    if ((Router->State.UtOn == 1) && (DsRouter->Usb3UpAdpNum != 0) &&
        (LaneAdp->AdpDep.Lane.PairedUsbAdp != 0)) {
      DEBUG ((DEBUG_INFO, "USB3 UP adapter = %d, enable USB3 Tunneling\n", DsRouter->Usb3UpAdpNum));
      Status = Usb4RtSetAttributes (DsRouter, RT_CS5_UTO, TRUE);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Rt 0x%016llX - Enable USB Tunneling failure, %r\n", DsRouter->TopologyId, Status));
        goto Exit;
      }
      DsRouter->State.UtOn = 1;
    }

    //
    // Enable PCIe Tunneling only if the following conditions are true:
    // 1. PCIe Tunneling is enabled in parent Router
    // 2. PCIe Upstream Adapter is present in downstream Router
    // 3. The downstream Lane adapter is paied with a PCIe adapter
    //
    if ((Router->State.PtOn == 1) && (DsRouter->PcieUpAdpNum != 0) &&
        (LaneAdp->AdpDep.Lane.PairedPcieAdp != 0)) {
      DEBUG ((DEBUG_INFO, "PCIe UP adapter = %d, enable PCIe Tunneling\n", DsRouter->PcieUpAdpNum));
      Status = Usb4RtSetAttributes (DsRouter, RT_CS5_PTO, TRUE);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Rt 0x%016llX - Enable PCIe Tunneling failure, %r\n", Router->TopologyId, Status));
        goto Exit;
      }
      DsRouter->State.PtOn = 1;

      //
      // Enable internal USB Host Controller of downstream Router if the following conditions are true:
      // 1. Internal xHCI is implemented in the downstream Router it is present.
      // 2. USB3 Tunneling of downstream Router is not enabled.
      // 3. PCIe Tunneling of downstream Router is enabled.
      //
      if ((DsRouter->State.Usb3HcImpl) && (DsRouter->State.UtOn == 0)) {
        DEBUG ((DEBUG_INFO, "Internal USB HC implemented, enable USB Host Controller\n"));
        Status = Usb4RtSetAttributes (DsRouter, RT_CS5_HCO, TRUE);
        if (Status == EFI_SUCCESS) {
          DsRouter->State.Usb3HcOn = 1;
        } else {
          DEBUG ((DEBUG_ERROR, "Rt 0x%016llX - Enable internal USB HC failure, %r\n", Router->TopologyId, Status));
        }
      }
    }

    //
    // Set Configuration Valid bit to let Router knows PTC, HCO, UTO bit are valid now.
    //
    DEBUG ((DEBUG_INFO, "Rt 0x%016llX - Write Configuration Valid bit\n", DsRouter->TopologyId));
    Status = Usb4RtSetAttributes (DsRouter, RT_CS5_CV, TRUE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Rt 0x%016llX - Write Configuration Valid failure, %r\n", DsRouter->TopologyId, Status));
      goto Exit;
    }

    //
    // Wait for Configuration Ready bit to indicate Router is ready for Protocol Tunneling.
    //
    DEBUG ((DEBUG_INFO, "Rt 0x%016llX - Wait for Configuration Ready bit\n", DsRouter->TopologyId));
    Status = Usb4RtWaitForStatus (DsRouter, RT_CS6_CR);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Rt 0x%016llX - Wait for Configuration Ready failure, %r\n", DsRouter->TopologyId, Status));
      goto Exit;
    }
  } else {
    //
    // <TBT3>
    // TBB3 router supports PCIe tunneling by default, but doesn't support native USB3 tunneling.
    // USB3 is supported by the internal xHCI controller.
    //
    if ((Router->State.PtOn == 1) && (DsRouter->PcieUpAdpNum != 0) && (LaneAdp->AdpDep.Lane.PairedPcieAdp != 0)) {
      DsRouter->State.PtOn     = 1;
      DsRouter->State.Usb3HcOn = 1;
    }
    DsRouter->State.UtOn     = 0;
  }

  //
  // Setup path for PCIe Tunneling if the paired PCIe downstream adapter and PCIe upstream adapter are present.
  //
  if (DsRouter->State.PtOn) {
    DEBUG ((DEBUG_INFO, "Set PCIe path for Rt 0x%016llX <=> 0x%016llX\n", Router->TopologyId, DsRouter->TopologyId));
    Status = Usb4RtConfigDsPciePath (Router, AdpNum);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "<U4CmErr> Rt 0x%016llX adp %d - Set PCIe path failure %r\n", Router->TopologyId, AdpNum, Status));
      //
      // Clear PCIe tunneling support in router state.
      // Don't go to Exit since PCIe tunneling failure should not break USB3 tunneling.
      //
      DsRouter->State.PtOn = 0;
    } else {
      //
      // <TBT3>
      // Set xHC connect bit of router VSEC6 port regions for TitanRidge xHC enablement.
      // Refer to CM TBT3 Device Support 4.1.2 xHC enablement.
      //
      if (DsRouter->RouterType == USB4_ROUTER_TITAN_RIDGE) {
        Status = Tbt3TrConnectXhc (DsRouter, TRUE);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "<U4CmErr> Fail to connect xHC for TitanRidge, %r\n", Status));
          //
          // Clear PCIe tunneling support in router state.
          // Don't go to Exit since PCIe tunneling failure should not break USB3 tunneling.
          //
          DsRouter->State.PtOn = 0;
        }
      } // if (DsRouter->RouterType == USB4_ROUTER_TITAN_RIDGE)
    }
  } // if (DsRouter->State.PtOn)

  //
  // Setup path for USB3 tunneling if USB3 tunneling is enabled.
  //
  if (DsRouter->State.UtOn) {
    DEBUG ((DEBUG_INFO, "Set USB3 path for Rt 0x%016llX <=> 0x%016llX\n", Router->TopologyId, DsRouter->TopologyId));
    Status = Usb4RtConfigDsUsb3Path (Router, AdpNum);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "<U4CmErr> Rt 0x%016llX adp %d - Set USB3 path failure %r\n", Router->TopologyId, AdpNum, Status));
      //
      // Clear USB3 tunneling support in router state
      //
      DsRouter->State.UtOn = 0;
    }
  }

  //
  // Return enable protocol success if either PCIe or USB3 tunneling is enabled successfully.
  //
  Status = (DsRouter->State.PtOn || DsRouter->State.UtOn) ? EFI_SUCCESS : EFI_UNSUPPORTED;

Exit:
  DEBUG ((DEBUG_INFO, "Usb4RtEnableDsProtocols exit - %r\n", Status));
  return Status;
}

/**
  Remove Protocol Tunneling settings that are configured for the connection between Router and its downstream Routers.

  @param[in] Router - Pointer to Router instance.
  @param[in] AdpNum - Downstream Adapter number.

  @retval EFI_SUCCESS           - Remove Protocol Tunneling settings success.
  @retval EFI_UNSUPPORTED       - Unexpected protocol tunneling removal failure.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4RtRemoveDsProtocols (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum
  )
{
  EFI_STATUS      Status;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4RtRemoveDsProtocols: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Usb4RtRemoveDsProtocols entry - Rt 0x%016llX AdpNum %d\n", Router->TopologyId, AdpNum));

  Status = Usb4RtRemoveDsDp (Router, AdpNum);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Rt 0x%016llX AdpNum %d DP tear down failure, %r\n", Router->TopologyId, AdpNum, Status));
  }

  Status = Usb4RtRemoveDsPcie (Router, AdpNum);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Rt 0x%016llX AdpNum %d PCIe tear down failure, %r\n", Router->TopologyId, AdpNum, Status));
  }

  Status = Usb4RtRemoveDsUsb3 (Router, AdpNum);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Rt 0x%016llX AdpNum %d USB3 tear down failure, %r\n", Router->TopologyId, AdpNum, Status));
  }

Exit:
  DEBUG ((DEBUG_INFO, "Usb4RtRemoveDsProtocols exit\n"));
  return Status;
}

/**
  Get available HopID between current router's egress lane adapter and next router's ingress lane adapter.

  @param[in]  CurRt     - Pointer to the router instance.
  @param[in]  OutAdpNum - Egress lane adapter number.
  @param[in]  NextRt    - Pointer to the next router instance.
  @param[in]  InAdpNum  - Ingress lane adapter number.
  @param[out] NewHopId  - Pointer to the returned HopID.

  @retval EFI_SUCCESS           - Get available HopID successfully.
  @retval EFI_NOT_FOUND         - No HopID available for use.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
GetAvailableHopId (
  IN USB4_ROUTER    *CurRt,
  IN UINT8          OutAdpNum,
  IN USB4_ROUTER    *NextRt,
  IN UINT8          InAdpNum,
  OUT UINT16        *NewHopId
  )
{
  EFI_STATUS    Status;
  UINT16        HopId;
  UINT16        HopIdMax;
  UINT32        HopIdMask;

  Status = EFI_SUCCESS;

  if ((CurRt == NULL) || (NextRt == NULL) || (NewHopId == NULL)) {
    DEBUG ((DEBUG_ERROR, "GetAvailableHopId: Invalid parameter, (%p, %p, %p)\n", CurRt, NextRt, NewHopId));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  if ((OutAdpNum == 0) || (OutAdpNum > CurRt->MaxAdapter) || (OutAdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX invalid adp %d\n", CurRt->TopologyId, OutAdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  if (CurRt->Adapter[OutAdpNum].Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX adp %d is not Lane adapter\n", CurRt->TopologyId, OutAdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  if ((InAdpNum == 0) || (InAdpNum > NextRt->MaxAdapter) || (InAdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX invalid adp %d\n", NextRt->TopologyId, InAdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  if (NextRt->Adapter[InAdpNum].Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "Rt 0x%016llX adp %d is not Lane adapter\n", NextRt->TopologyId, InAdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Look for the available HopID by checking HopIdMask of current router's egress lane adapter.
  // Each bit of HopIdMask maps to one HopID.
  // Bit 0  - DYNAMIC_HOPID_MIN (10)
  // Bit 31 - DYNAMIC_HOPID_MAX (41)
  //
  HopIdMask = CurRt->Adapter[OutAdpNum].AdpDep.Lane.HopIdMask;
  HopIdMax = MIN (CurRt->Adapter[OutAdpNum].MaxOutputHopId, NextRt->Adapter[InAdpNum].MaxInputHopId);
  HopIdMax = MIN (HopIdMax, DYNAMIC_HOPID_MAX);
  for (HopId = DYNAMIC_HOPID_MIN; HopId <= HopIdMax; HopId++) {
    if ((HopIdMask & (1 << (HopId - DYNAMIC_HOPID_MIN))) == 0) {
      break;
    }
  }
  //
  // Check if a new HopID is found
  //
  if (HopId <= HopIdMax) {
    //
    // Set the bit of the mapped HopID in HopIdMask.
    //
    HopIdMask |= (1 << (HopId - DYNAMIC_HOPID_MIN));
    DEBUG ((DEBUG_INFO, "Rt 0x%016llX adp %d: New HopId = %d, HopIdMask = 0x%08x\n", CurRt->TopologyId, OutAdpNum, HopId, HopIdMask));
    CurRt->Adapter[OutAdpNum].AdpDep.Lane.HopIdMask = HopIdMask;
    *NewHopId = HopId;
  } else {
    DEBUG ((DEBUG_ERROR, "No HopID available in Rt %016llX Adp %d, HopIdMask = 0x%0X\n", CurRt->TopologyId, OutAdpNum, HopIdMask));
    Status = EFI_NOT_FOUND;
  }

Exit:
  return Status;
}

/**
  Free HopID when path is teared down

  @param[in] CurRt     - Pointer to the router instance.
  @param[in] OutAdpNum - Egress lane adapter number.
  @param[in] HopId     - HopID to be released.

  @retval EFI_SUCCESS           - Free HopID successfully.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
FreeHopId (
  IN USB4_ROUTER    *CurRt,
  IN UINT8          OutAdpNum,
  IN UINT16         HopId
  )
{
  EFI_STATUS    Status;
  UINT16        HopIdMax;
  UINT32        HopIdMask;


  Status = EFI_SUCCESS;

  if (CurRt == NULL) {
    DEBUG ((DEBUG_ERROR, "FreeHopId: Null router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  if ((OutAdpNum == 0) || (OutAdpNum > CurRt->MaxAdapter) || (OutAdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "FreeHopId: Rt 0x%016llX invalid adp %d\n", CurRt->TopologyId, OutAdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  if (CurRt->Adapter[OutAdpNum].Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "FreeHopId: Rt 0x%016llX adp %d is not Lane adapter\n", CurRt->TopologyId, OutAdpNum));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  HopIdMax = MIN (CurRt->Adapter[OutAdpNum].MaxOutputHopId, DYNAMIC_HOPID_MAX);
  if (HopId > HopIdMax) {
    DEBUG ((DEBUG_ERROR, "FreeHopId: HopId %d is not valid\n", HopId));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Clear the bit of the mapped HopID from HopIdMask.
  // Each bit of HopIdMask maps to one HopID.
  // Bit 0  - DYNAMIC_HOPID_MIN (10)
  // Bit 31 - DYNAMIC_HOPID_MAX (41)
  //
  CurRt->Adapter[OutAdpNum].AdpDep.Lane.HopIdMask &= ~(1 << (HopId - DYNAMIC_HOPID_MIN));
  HopIdMask = CurRt->Adapter[OutAdpNum].AdpDep.Lane.HopIdMask;
  DEBUG ((DEBUG_INFO, "Rt 0x%016llX adp %d: Free HopId = %d, HopIdMask = 0x%08x\n", CurRt->TopologyId, OutAdpNum, HopId, HopIdMask));

Exit:
  return Status;
}