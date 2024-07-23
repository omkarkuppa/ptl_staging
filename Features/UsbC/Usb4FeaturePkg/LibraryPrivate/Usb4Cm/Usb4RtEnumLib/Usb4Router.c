/** @file
  USB4 router specific implementation.

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
#include <Library/Usb4ProtocolsLib.h>
#include <Library/Tbt3SupportLib.h>
#include <Library/Usb4Common.h>
#include <Library/Usb4ConfigLayer.h>
#include <Library/Usb4ConfigSpace.h>
#include <Library/Usb4Capabilities.h>
#include <Library/Usb4CsIo.h>
#include <Library/Usb4RouterDefs.h>
#include <Library/Tbt3ConfigSpace.h>
#include <Library/Usb4DebugLib.h>
#include "Usb4Adapter.h"

extern EFI_USB4_DEBUG_PROTOCOL   *mUsb4Log;

const UINT16 mAlpineRidgeIdTable[] = {
  0x1575, 0x1576, 0x1577, 0x1578, 0x15DD, 0x15BF, 0x15C0, 0x15DC, 0x15D9, 0x15DA,
  0x15D2, 0x15D3, 0x15DE, 0x15B5, 0x15C1, 0x15D4, 0x15DB
};

const UINT16 mTitanRidgeIdTable[] = {
  0x15EC, 0x15E9, 0x15E7, 0x15E8, 0x15EA, 0x15EB, 0x15EF
};

const UINT16 mYosemiteFallsIdTable[] = {
  0x8A17, 0x8A0D, 0x8AA3, 0x8AA0, 0x8AB3, 0x8AB0, 0x463E, 0x466D, 0x464E, 0x467D,
  0x465E, 0x468D
};

const UINT16 mMapleRidgeIdTable[] = {
  0x1138, 0x1137, 0x1136, 0x1135, 0x1134, 0x1133
};

const UINT16 mTglItbtIdTable[] = {
  0x9A1B, 0x9A1D, 0x9A1F, 0x9A21
};

/**
  Check if a Router is in a Router tree starting from the specified root.

  @param[in] Router - Pointer to the downstream Router.
  @param[in] Root   - Pointer to the tree root

  @retval TRUE  - Router is in the tree.
  @retval FALSE - Router is not in the tree.
**/
BOOLEAN
Usb4RtInRtTree (
  IN PUSB4_ROUTER     Router,
  IN PUSB4_ROUTER     Root
  )
{
  UINT64      Mask;
  BOOLEAN     InTree;

  InTree = FALSE;

  if ((Router == NULL) || (Root == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4RtInRtTree: Null router (%p, %p)\n", Router, Root));
    goto Exit;
  }

  if (Router->Depth >= Root->Depth) {
    Mask = (((UINT64) 1) << (Root->Depth << 3)) - 1;
    InTree = ((Router->TopologyId.Value & Mask) == Root->TopologyId.Value) ? TRUE : FALSE;
  }

  DEBUG ((DEBUG_INFO, "Usb4RtInRtTree: Rt 0x%016llX, Root 0x%016llX, Ret = %d\n", Router->TopologyId, Root->TopologyId, InTree));
Exit:
  return InTree;
}

/**
  Set Router attributes in Router CS 5

  @param[in] Router       - Pointer to Router instance.
  @param[in] Attrib       - Router attribute to be set.
  @param[in] Enable       - Indicate attribute enable/disable.

  @retval EFI_SUCCESS           - Set Router attributes successfully.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval Errors in config space access.
**/
EFI_STATUS
Usb4RtSetAttributes (
  IN PUSB4_ROUTER    Router,
  IN UINT32          Attrib,
  IN BOOLEAN         Enable
  )
{
  EFI_STATUS      Status;
  USB4_RT_CS_5    RtCs5;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4RtSetAttributes: Null Router!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DEBUG ((CM_VERBOSE, "Set Router 0x%016llX attribs 0x%0X enable = %d\n", Router->TopologyId, Attrib, Enable));
  Status = Usb4CsRtRead (Router, USB4_CS_5, 1, (UINT32 *) &RtCs5);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Router 0x%016llX - Query Router CS5 failure, %r\n", Router->TopologyId, Status));
    goto Exit;
  }

  if (Enable) {
    RtCs5.Value |= (Attrib & RT_CS5_BIT_MASK);
  } else {
    RtCs5.Value &= ~(Attrib & RT_CS5_BIT_MASK);
  }

  DEBUG ((CM_VERBOSE, "Write Router 0x%016llX attribs 0x%0X\n", Router->TopologyId, RtCs5));
  Status = Usb4CsRtWrite (Router, USB4_CS_5, 1, (UINT32 *) &RtCs5);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Router 0x%016llX - Write Router CS5 failure, %r\n", Router->TopologyId, Status));
    goto Exit;
  }

Exit:
  return Status;
}

/**
  Wait for Router status bits of Router CS 6

  @param[in] Router     - Pointer to Router instance.
  @param[in] WaitFlag   - Router status flags for waiting.

  @retval EFI_SUCCESS   - All specified status bits are set.
  @retval EFI_TIMEOUT   - Not all specified status bits are set after a timeout period.
  @retval Errors in config space access.
**/
EFI_STATUS
Usb4RtWaitForStatus (
  IN PUSB4_ROUTER    Router,
  IN UINT32          WaitFlags
  )
{
  EFI_STATUS      Status;
  UINT32          RetryCount;
  USB4_RT_CS_6    RtCs6;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4RtWaitForStatus: Null Router!\n\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  for (RetryCount = 0; RetryCount < CM_STATUS_WAITS_MAX; RetryCount++) {
    Status = Usb4CsRtRead (Router, USB4_CS_6, 1, (UINT32 *) &RtCs6);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Usb4RtWaitForStatus: Read Router CS6 error, %r\n", Status));
      goto Exit;
    }

    if ((RtCs6.Value & WaitFlags) == WaitFlags) {
      break;
    }
    gBS->Stall (CM_STATUS_POLL_US);
  }

  if (RetryCount == CM_STATUS_WAITS_MAX) {
    DEBUG ((DEBUG_ERROR, "Usb4RtWaitForStatus timeout, Router CS6 = 0x%08X!\n", RtCs6));
    Status = EFI_TIMEOUT;
    goto Exit;
  }
Exit:
  return Status;
}

/**
  Look up device ID table for router type

  @param[in] VendorId - Vendor ID of Router
  @param[in] DeviceId - Device ID of Router

  @retval Router type
**/
USB4_ROUTER_TYPE
Usb4DeviceIdToRouterType (
  UINT16    VendorId,
  UINT16    DeviceId
  )
{
  USB4_ROUTER_TYPE    RouterType;
  UINT32              Index;

  RouterType = USB4_ROUTER_USB4;

  if ((VendorId == USB_VENDOR_ID_INTEL_1) || (VendorId == USB_VENDOR_ID_INTEL_2)) {
    for (Index = 0; Index < sizeof(mMapleRidgeIdTable)/sizeof(UINT16); Index++) {
      if (mMapleRidgeIdTable[Index] == DeviceId) {
        RouterType = USB4_ROUTER_MAPLE_RIDGE;
        goto Exit;
      }
    }
    for (Index = 0; Index < sizeof(mTitanRidgeIdTable)/sizeof(UINT16); Index++) {
      if (mTitanRidgeIdTable[Index] == DeviceId) {
        RouterType = USB4_ROUTER_TITAN_RIDGE;
        goto Exit;
      }
    }

    for (Index = 0; Index < sizeof(mAlpineRidgeIdTable)/sizeof(UINT16); Index++) {
      if (mAlpineRidgeIdTable[Index] == DeviceId) {
        RouterType = USB4_ROUTER_ALPINE_RIDGE;
        goto Exit;
      }
    }
  }

Exit:
  return RouterType;
}

/**
  Detect device router type based on Device ID.
  Set TTB3 flag in router instance if TBT3 router is detected.

  @param[in,out] Router  - Pointer to router instance

  @retval EFI_SUCCESS           - Identify the router type successfully.
  @retval EFI_UNSUPPORTED       - The router type can't be identified.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval Errors in config space access.
**/
EFI_STATUS
Usb4RtDetect (
  IN OUT PUSB4_ROUTER    Router
  )
{
  EFI_STATUS             Status;
  USB4_RT_CS_0           RtCs0;
  UINT16                 VendorId;
  UINT16                 DeviceId;

  DEBUG ((DEBUG_INFO, "Usb4RtDetect entry\n"));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4RtDetect: Invalid parameter\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Query Device ID from USB4 Router configuration space
  //
  Status = Usb4CsRtRead (Router, USB4_CS_0, 1, (UINT32 *) &RtCs0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4RtDetect: Read USB4 Router config space failure, %r!\n", Status));
    goto Exit;
  }

  VendorId = (UINT16)(RtCs0.Fields.VendorId);
  DeviceId = (UINT16)(RtCs0.Fields.DeviceId);
  //
  // May need to find a general way to identify USB4 and TBT3 router instead of using VID & PID
  //
  Router->RouterType = Usb4DeviceIdToRouterType (VendorId, DeviceId);
  DEBUG ((DEBUG_INFO, "Vendor ID = 0x%0X, Device ID = 0x%0X, Router type = %d\n", VendorId, DeviceId, Router->RouterType));

  switch (Router->RouterType) {
    case USB4_ROUTER_TITAN_RIDGE:
      DEBUG ((DEBUG_INFO, "TitanRidge is detected!\n"));
      Router->State.Tbt3 = 1;
      break;
    case USB4_ROUTER_ALPINE_RIDGE:
      DEBUG ((DEBUG_INFO, "AlpineRidge is detected!\n"));
      Router->State.Tbt3 = 1;
      break;
    case USB4_ROUTER_MAPLE_RIDGE:
      DEBUG ((DEBUG_INFO, "MapleRidge is detected!\n"));
      break;
    case USB4_ROUTER_USB4:
      DEBUG ((DEBUG_INFO, "USB4 compliant Router is detected!\n"));
      break;
    default:
      DEBUG ((DEBUG_ERROR, "Usb4RtDetect: Unsupported router type 0x%0x\n", Router->RouterType));
      Status = EFI_UNSUPPORTED;
      break;
  }

Exit:
  DEBUG ((DEBUG_INFO, "Usb4RtDetect exit\n"));
  return Status;
}

/**
  Scan router adapters and find the connectivity between Lane adapter and PCIe/USB3 adapter.

  @param[in] Router  - Pointer to Router instance.

  @retval EFI_SUCCESS           - Scan router adapters successfully.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval Errors in config space access.
**/
EFI_STATUS
Usb4RtScanAdapters (
  IN OUT PUSB4_ROUTER    Router
  )
{
  EFI_STATUS           Status;
  UINT8                Index;
  UINT8                PcieIndex;
  UINT8                Usb3Index;
  UINT8                NextPcie;
  UINT8                NextUsb3;
  BOOLEAN              Lane0;
  USB4_ADAPTER_TYPE    AdapterType;

  DEBUG ((DEBUG_INFO, "Usb4RtScanAdapters entry\n"));

  Status = EFI_SUCCESS;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4RtScanAdapters: Invalid parameter\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Lane0 = TRUE;

  //
  // Scan adapters from adapter 1.
  // Adapter 0 is control adapter and has no adapter configuration space.
  //
  for (Index = 1; (Index <= Router->MaxAdapter) && (Index < USB4_ADAPTER_SUPPORT_MAX); Index++) {
    Status = Usb4AdpEnum (Router, Index, Lane0);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Usb4RtScanAdapters: Enumerate adapter %d failure, %r\n", Index, Status));
      goto Exit;
    }

    AdapterType = Router->Adapter[Index].Type;
    if (AdapterType == USB4_ADP_TYPE_UNSUPPORTED) {
      continue;
    }

    //
    // The next Lane adapter should be Lane1 if the current adapter is Lane0 adapter.
    // The next Lane adapter should be Lane0 if the current adapter is Lane1 adapter.
    //
    Lane0 = (AdapterType == USB4_ADP_TYPE_LANE) ? !Lane0 : TRUE;

    if (AdapterType == USB4_ADP_TYPE_PCIE_US) {
      Router->PcieUpAdpNum = Index;
    } else if (AdapterType == USB4_ADP_TYPE_USB3_US) {
      Router->Usb3UpAdpNum = Index;
    }

    //
    // Set Pcie flag to indicate Router supports PCIe if PCIe adapter is detected.
    // Set Usb3 flag to indicate Router supports USB3 if USB3 adapter is detected.
    // Set DpIn flag to indicate Router supports DP-IN if DP-IN adapter is detected.
    // Set DpOut flag to indicate Router supports DP-OUT if DP-OUT adapter is detected.
    //
    switch (AdapterType) {
      case USB4_ADP_TYPE_PCIE_US:
        Router->PcieUpAdpNum = Index;
        Router->State.Pcie = 1;
        break;
      case USB4_ADP_TYPE_PCIE_DS:
        Router->State.Pcie = 1;
        break;
      case USB4_ADP_TYPE_USB3_US:
        Router->Usb3UpAdpNum = Index;
        Router->State.Usb3 = 1;
        break;
      case USB4_ADP_TYPE_USB3_DS:
        Router->State.Usb3 = 1;
        break;
      case USB4_ADP_TYPE_DP_IN:
        Router->State.DpIn = 1;
        break;
      case USB4_ADP_TYPE_DP_OUT:
        Router->State.DpOut = 1;
        //
        // <TBT3>
        // (TitanRidge only, refer to CM TBT3 Device support 5.1 TMU configuration)
        //
        if (Router->RouterType == USB4_ROUTER_TITAN_RIDGE) {
          Status = Tbt3TrSetTmuStable (Router, Index);
          if (EFI_ERROR (Status)) {
            DEBUG ((DEBUG_ERROR, "Fail to set DP OUT TMU Stable bit, %r\n", Status));
            goto Exit;
          }
        }
        break;
      case USB4_ADP_TYPE_HOST_INTERFACE:
        Router->HiAdpNum = Index;
        break;
      default:
        break;
    }
  }

  if (Router->State.Tbt3 == 0) {
    //
    // Find the pair relationship between Lane adapter, PCIe adapter and USB3 adapter.
    // Lane adapter and PCIe adapter are paired in order of increasing adapter numbers.
    // Lane adapter and USB3 adapter are paired in order of increasing adapter numbers.
    // A Lane adapter can be paired with a PCIe adapter and a USB3 adapter simultaneously.
    // Example: USB4 Ports are (1,2) (3,4) (5,6), PCIe adapters are (7, 8, 9), USB3 adapters are (10, 11, 12)
    // The connectivity that a Router shall have is the following:
    // {(1, 7), (1, 10)}, {(3, 8), (3, 11)}, {(5, 9), (5, 12)}
    //
    NextPcie = 1;
    NextUsb3 = 1;
    for (Index = 1; (Index <= Router->MaxAdapter) && (Index < USB4_ADAPTER_SUPPORT_MAX); Index++) {
      if (Router->Adapter[Index].Type != USB4_ADP_TYPE_LANE) {
        continue;
      }
      if (Router->Adapter[Index].AdpDep.Lane.Status.Lane0 == FALSE) {
        continue;
      }

      if (Router->State.Pcie == 1) {
        for (PcieIndex = NextPcie; (PcieIndex <= Router->MaxAdapter) && (PcieIndex < USB4_ADAPTER_SUPPORT_MAX); PcieIndex++) {
          AdapterType = Router->Adapter[PcieIndex].Type;
          if ((AdapterType == USB4_ADP_TYPE_PCIE_US) || (AdapterType == USB4_ADP_TYPE_PCIE_DS)) {
            DEBUG ((DEBUG_INFO, "Lane-PCIe adapter pair : Lane = %d, PCIe = %d)\n", Index, PcieIndex));
            Router->Adapter[Index].AdpDep.Lane.PairedPcieAdp = PcieIndex;
            Router->Adapter[PcieIndex].AdpDep.AdpPcie.PairedLaneAdp = Index;
            NextPcie = PcieIndex + 1;
            break;
          }
        }
      }

      if (Router->State.Usb3 == 1) {
        for (Usb3Index = NextUsb3; (Usb3Index <= Router->MaxAdapter) && (Usb3Index < USB4_ADAPTER_SUPPORT_MAX); Usb3Index++) {
          AdapterType = Router->Adapter[Usb3Index].Type;
          if ((AdapterType == USB4_ADP_TYPE_USB3_US) || (AdapterType == USB4_ADP_TYPE_USB3_DS)) {
            DEBUG ((DEBUG_INFO, "Lane-USB adapter pair : Lane = %d, USB = %d)\n", Index, Usb3Index));
            Router->Adapter[Index].AdpDep.Lane.PairedUsbAdp = Usb3Index;
            Router->Adapter[Usb3Index].AdpDep.AdpUsb3.PairedLaneAdp = Index;
            NextUsb3 = Usb3Index + 1;
            break;
          }
        }
      }
    }
  } else {
    //
    // <TBT3>
    // TBT3 upstream lane adapter number is not always the lowest number of lane adapter.
    // It depends on which port is connected to the host router.
    // The routing pair rule is different to USB4.
    // Pair the upstream adapter for Lane/PCIe/USB3 first.
    //
    if ((Router->LaneUpAdpNum != 0) && (Router->LaneUpAdpNum <= Router->MaxAdapter) && (Router->LaneUpAdpNum < USB4_ADAPTER_SUPPORT_MAX)) {
      if ((Router->PcieUpAdpNum != 0) && (Router->PcieUpAdpNum <= Router->MaxAdapter) && (Router->PcieUpAdpNum < USB4_ADAPTER_SUPPORT_MAX)) {
        DEBUG ((DEBUG_INFO, "Lane-PCIe adapter pair : Lane = %d, PCIe = %d)\n", Router->LaneUpAdpNum, Router->PcieUpAdpNum));
        Router->Adapter[Router->LaneUpAdpNum].AdpDep.Lane.PairedPcieAdp    = Router->PcieUpAdpNum;
        Router->Adapter[Router->PcieUpAdpNum].AdpDep.AdpPcie.PairedLaneAdp = Router->LaneUpAdpNum;
      }
      if ((Router->Usb3UpAdpNum != 0) && (Router->Usb3UpAdpNum <= Router->MaxAdapter) && (Router->Usb3UpAdpNum < USB4_ADAPTER_SUPPORT_MAX)) {
        DEBUG ((DEBUG_INFO, "Lane-USB adapter pair : Lane = %d, USB = %d)\n", Router->LaneUpAdpNum, Router->Usb3UpAdpNum));
        Router->Adapter[Router->LaneUpAdpNum].AdpDep.Lane.PairedUsbAdp     = Router->Usb3UpAdpNum;
        Router->Adapter[Router->Usb3UpAdpNum].AdpDep.AdpUsb3.PairedLaneAdp = Router->LaneUpAdpNum;
      }
    }

    //
    // Pair the downstream adapters for Lane/PCIe/USB3
    //
    NextPcie = 1;
    NextUsb3 = 1;
    for (Index = 1; (Index <= Router->MaxAdapter) && (Index < USB4_ADAPTER_SUPPORT_MAX); Index++) {
      if (Router->Adapter[Index].Type != USB4_ADP_TYPE_LANE) {
        continue;
      }
      if (Router->Adapter[Index].AdpDep.Lane.Status.Lane0 == FALSE) {
        continue;
      }
      if (Index == Router->LaneUpAdpNum) {
        continue;
      }

      if (Router->State.Pcie == 1) {
        for (PcieIndex = NextPcie; (PcieIndex <= Router->MaxAdapter) && (PcieIndex < USB4_ADAPTER_SUPPORT_MAX); PcieIndex++) {
          AdapterType = Router->Adapter[PcieIndex].Type;
          if (AdapterType == USB4_ADP_TYPE_PCIE_DS) {
            DEBUG ((DEBUG_INFO, "Lane-PCIe adapter pair : Lane = %d, PCIe = %d)\n", Index, PcieIndex));
            Router->Adapter[Index].AdpDep.Lane.PairedPcieAdp = PcieIndex;
            Router->Adapter[PcieIndex].AdpDep.AdpPcie.PairedLaneAdp = Index;
            NextPcie = PcieIndex + 1;
            break;
          }
        }
      }

      if (Router->State.Usb3 == 1) {
        for (Usb3Index = NextUsb3; (Usb3Index <= Router->MaxAdapter) && (Usb3Index < USB4_ADAPTER_SUPPORT_MAX); Usb3Index++) {
          AdapterType = Router->Adapter[Usb3Index].Type;
          if (AdapterType == USB4_ADP_TYPE_USB3_DS) {
            DEBUG ((DEBUG_INFO, "Lane-USB adapter pair : Lane = %d, USB = %d)\n", Index, Usb3Index));
            Router->Adapter[Index].AdpDep.Lane.PairedUsbAdp = Usb3Index;
            Router->Adapter[Usb3Index].AdpDep.AdpUsb3.PairedLaneAdp = Index;
            NextUsb3 = Usb3Index + 1;
            break;
          }
        }
      }
    }
  }

Exit:
  DEBUG ((DEBUG_INFO, "Usb4RtScanAdapters exit\n"));
  return Status;
}

/**
  Scan Router Capabilities

  @param[in] Router  - Pointer to Router instance.
  @param[in] Offset  - Offset of next Capability.

  @retval EFI_SUCCESS           - Scan Router Capabilities success.
  @retval EFI_INVALID_PARAMETER - Scan Router Capabilities failure.
  @retval Errors in config space access.
**/
EFI_STATUS
Usb4RtScanCapabilities (
  IN PUSB4_ROUTER     Router,
  IN UINT16           Offset
  )
{
  EFI_STATUS               Status;
  USB4_CAP_HEADER          CapHead;
  UINT16                   CapReg;
  UINT16                   VsecNextCap;
  UINT8                    CapId;
  USB4_RT_CAP_VSC          *VscCap;
  USB4_RT_CAP_VSEC_CS_1    VsecCs1;

  DEBUG ((DEBUG_INFO, "Usb4RtScanCapabilities entry\n"));

  Status = EFI_SUCCESS;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4RtScanCapabilities: Invalid parameter\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Router 0x%016llX - Scan Router Capabilities\n", Router->TopologyId));
  CapReg = Offset;
  while (CapReg != 0) {
    VsecNextCap = 0;
    Status = Usb4CsRtRead (Router, CapReg, sizeofdw (USB4_CAP_HEADER), (UINT32 *) &CapHead);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Rt 0x%016llx read RT CS at 0x%x failure, %r\n", Router->TopologyId, CapReg, Status));
      goto Exit;
    }

    CapId = (UINT8) CapHead.CapId;
    DEBUG ((DEBUG_INFO, "Router CapId = %d, CapReg = 0x%0x\n", CapId, CapReg));
    switch (CapId) {
      case USB4_RT_CAP_ID_TMU:
        Router->TmuCapReg = CapReg;
        break;
      case USB4_RT_CAP_ID_VENDOR:
        VscCap = (USB4_RT_CAP_VSC *) &CapHead;
        if (VscCap->CapCs0.Fields.VscLength == 0) {
          //
          // VSEC, read CS1 to get next capability offset.
          //
          Status = Usb4CsRtRead (Router, CapReg + 1, 1, (UINT32 *) &VsecCs1);
          if (EFI_ERROR (Status)) {
            DEBUG ((DEBUG_ERROR, "Read Rt CapReg 0x%0x VSEC CS1 failure, %r\n", CapReg, Status));
            goto Exit;
          }
          VsecNextCap = (UINT16) VsecCs1.Fields.NextCap;

          DEBUG ((DEBUG_INFO, "    VSEC: VsecId = 0x%0x, VsecLen = 0x%0x\n",
                  VscCap->CapCs0.Fields.VscId,
                  VsecCs1.Fields.VsecLength
                  ));
          //
          // <TBT3>
          // TBT3 Vendor Specific Extended Capability 6
          //
          if (VscCap->CapCs0.Fields.VscId == VSEC_ID_6) {
            Router->Vsec6Reg = CapReg;
            if (Router->State.Tbt3 == 1) {
              Status = Tbt3QueryRtVsec6 (Router);
              if (EFI_ERROR (Status)) {
                DEBUG ((DEBUG_ERROR, "Query Rt VSEC6 failure, %r\n", Status));
              }
            }
          }
        } else {
          DEBUG ((DEBUG_INFO, "    VSC: VscId = 0x%0x, VscLen = 0x%0x\n",
                  VscCap->CapCs0.Fields.VscId,
                  VscCap->CapCs0.Fields.VscLength
                  ));
          //
          // <TBT3>
          // TBT3 Vendor Specific Capability 1
          //
          if (VscCap->CapCs0.Fields.VscId == VSC_ID_1) {
            Router->Vsc1Reg = CapReg;
          }
        }
    }
    //
    // VSEC Next Cap is at different location.
    //
    CapReg = (VsecNextCap) ? VsecNextCap : (UINT16) CapHead.NextCap;
  }

Exit:
  DEBUG ((DEBUG_INFO, "Usb4RtScanCapabilities exit\n"));
  return Status;
}

/**
  Disable router TMU

  @param[in] Router - Pointer to router instance.

  @retval EFI_SUCCESS - Disable router TMU successfully.
  @retval EFI_UNSUPPORTED - Fail to disable router TMU.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval Errors in config space access.
**/
EFI_STATUS
DisableRtTmu (
  IN PUSB4_ROUTER    Router
  )
{
  EFI_STATUS              Status;
  USB4_RT_CAP_TMU_CS_3    TmuCs3;

  DEBUG ((DEBUG_INFO, "DisableRtTmu entry\n"));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "DisableRtTmu: Null router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }
  if (Router->TmuCapReg == 0) {
    DEBUG ((DEBUG_ERROR, "DisableRtTmu: Null TMU capability\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Status = Usb4CsRtRead (Router, Router->TmuCapReg, 1, (UINT32 *) &TmuCs3);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "DisableRtTmu: Read TMU CS_3 failure, %r\n", Status));
    goto Exit;
  }
  DEBUG ((DEBUG_INFO, "Original TS packet interval = %d\n", TmuCs3.Fields.TsPktInterval));
  TmuCs3.Fields.TsPktInterval = 0;
  Status = Usb4CsRtWrite (Router, Router->TmuCapReg, 1, (UINT32 *) &TmuCs3);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "DisableRtTmu: Write TMU CS_3 failure, %r\n", Status));
    goto Exit;
  }

Exit:
  DEBUG ((DEBUG_INFO, "DisableRtTmu exit\n"));
  return Status;
}

/**
  Enumerate the Router.
  Write Topology ID and Topology ID Valid bit to Router Configuration Space.
  Router will enter enumerated state.

  @param[in] Router - Pointer to Router instance.

  @retval EFI_SUCCESS           - Write Topology ID and valid bit successfully.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval Errors in config space access
**/
EFI_STATUS
Usb4RtEnumerate (
  IN PUSB4_ROUTER  Router
  )
{
  EFI_STATUS         Status;
  USB4_RT_CS_INFO    RtCsInfo;
  UINT8              LaneUpAdpNum;

  DEBUG ((DEBUG_INFO, "Usb4RtEnumerate entry\n"));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4RtEnumerate: Null Router!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  LaneUpAdpNum = 0;

  //
  // Read the basic router information
  //
  Status = Usb4CsRtRead (Router, USB4_CS_0, sizeofdw (USB4_RT_CS_INFO), (UINT32 *)&RtCsInfo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Read router config space of 0x%016llx failure, %r\n", Router->TopologyId, Status));
    goto Exit;
  }

  //
  // Save the basic Router information
  //
  Router->VendorId = (UINT16)RtCsInfo.RouterCs0.Fields.VendorId;
  Router->DeviceId = (UINT16)RtCsInfo.RouterCs0.Fields.DeviceId;
  Router->MaxAdapter = (UINT8)RtCsInfo.RouterCs1.Fields.MaxAdapter;
  Router->Revision = RtCsInfo.RouterCs1.Fields.RevisionNumber;
  Router->NotifyTimeout = RtCsInfo.RouterCs4.Fields.NotificationTimeout;
  Router->Usb4Version = RtCsInfo.RouterCs4.Fields.Usb4Version;
  Router->State.Usb3HcImpl = RtCsInfo.RouterCs6.Fields.UsbHcImplemented;

  Status = Usb4RtScanCapabilities (Router, (UINT16)RtCsInfo.RouterCs1.Fields.NextCapabilityPointer);
  if (EFI_ERROR (Status)) {
    //
    // Capabilities scan failure should not stop router enumeration
    //
    DEBUG ((DEBUG_ERROR, "Router 0x%016llX scan router capability failure, %r\n", Router->TopologyId, Status));
  }

  //
  // Update Router CS data for USB4 and TBT3
  //
  if (Router->State.Tbt3 == 1) {
    //
    // <TBT3>
    // For TBT3 Router, Update Upstream Adapter field of Router CS1 by the response Adapter value of Router CS read.
    // Default Upstream Adapter of TBT3 Router CS1 doesn't indicate real Upstream Adapter number.
    // This is not required for the USB4 compliant Router.
    //
    Status = Usb4CsRtQueryUpAdapter (Router, &LaneUpAdpNum);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Query TBT3 Router 0x%016llX upstream adapter failure, %r\n", Router->TopologyId, Status));
      goto Exit;
    }
    DEBUG ((DEBUG_INFO, "TBT3 Router Upstream Adapter = %d, default = %d\n", LaneUpAdpNum, RtCsInfo.RouterCs1.Fields.UpstreamAdapter));
    Router->LaneUpAdpNum = LaneUpAdpNum;
    DEBUG ((DEBUG_INFO, "Change notification timeout from %d ms to %d ms\n", RtCsInfo.RouterCs4.Fields.NotificationTimeout, RT_NOTIFICATION_TIMEOUT_IN_MS));
    Router->NotifyTimeout = RT_NOTIFICATION_TIMEOUT_IN_MS;
    //
    // Disable router TMU according to CM TBT3 Device Support 3.2.1 Router enumeration and configuration.
    //
    Status = DisableRtTmu (Router);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Disable Rt TMU failure, %r\n", Status));
      goto Exit;
    }
  } else {
    DEBUG ((DEBUG_INFO, "Change notification timeout from %d ms to %d ms\n", RtCsInfo.RouterCs4.Fields.NotificationTimeout, RT_NOTIFICATION_TIMEOUT_IN_MS));
    Router->NotifyTimeout = RT_NOTIFICATION_TIMEOUT_IN_MS;
    Router->LaneUpAdpNum = (UINT8)RtCsInfo.RouterCs1.Fields.UpstreamAdapter;
    RtCsInfo.RouterCs4.Fields.CmUsb4VerMajor = 1;
    RtCsInfo.RouterCs4.Fields.CmUsb4VerMinor = 0;
    if (Router->CmVer == USB4_CM_VERSION_2) {
      RtCsInfo.RouterCs4.Fields.CmUsb4VerMajor = 2;
    }
  }

  //
  // Set Topology ID, Topology Valid bit, Depth for Router enumeration
  //
  RtCsInfo.RouterCs1.Fields.Depth               = (UINT32)Router->Depth;
  RtCsInfo.RouterCs1.Fields.UpstreamAdapter     = (UINT32)Router->LaneUpAdpNum;
  RtCsInfo.RouterCs2.Fields.ToplogyIdLow        = Router->TopologyId.Id.Low;
  RtCsInfo.RouterCs3.Fields.ToplogyIdHigh       = Router->TopologyId.Id.High;
  RtCsInfo.RouterCs3.Fields.ToplogyIdValid      = 1;
  RtCsInfo.RouterCs4.Fields.NotificationTimeout = Router->NotifyTimeout;

  DEBUG ((DEBUG_INFO, "Set Router 0x%016llx TopologyId Valid\n", Router->TopologyId.Value));

  //
  // Write Router CS1 ~ CS4 to set Topology ID, Topology Valid and Depth
  //  - up until now no matter the Route string the access was only to the router
  //  - Control adapter will start sending plug packets
  //  - Control adapter will allow access to Configuration spaces other than "Device space"
  //  - Control adapter will allow access to switches beyond the router
  //
  Status = Usb4CsRtWrite (Router, USB4_CS_1, 4, (UINT32 *)&(RtCsInfo.RouterCs1));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Set Router 0x%016llx TopologyID Valid failure, %r\n", Router->TopologyId, Status));
    goto Exit;
  }

  //
  // Read back the router information (only for the initial development phase)
  //
  Status = Usb4CsRtRead (Router, USB4_CS_1, 4, (UINT32 *)&RtCsInfo.RouterCs1);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Read router config space of 0x%016llx failure, %r\n", Router->TopologyId, Status));
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Router Vendor ID         = 0x%04X\n", Router->VendorId));
  DEBUG ((DEBUG_INFO, "Router Device ID         = 0x%04X\n", Router->DeviceId));
  DEBUG ((DEBUG_INFO, "Router Upstream Adapter  = %d\n", Router->LaneUpAdpNum));
  DEBUG ((DEBUG_INFO, "Router Max Adapter       = %d\n", Router->MaxAdapter));
  DEBUG ((DEBUG_INFO, "Router Depth             = %d\n", RtCsInfo.RouterCs1.Fields.Depth));
  DEBUG ((DEBUG_INFO, "Router Revision Number   = 0x%0X\n", Router->Revision));
  DEBUG ((DEBUG_INFO, "Router Topology ID Low   = 0x%08X\n", RtCsInfo.RouterCs2.Fields.ToplogyIdLow));
  DEBUG ((DEBUG_INFO, "Router Topology ID High  = 0x%08X\n", RtCsInfo.RouterCs3.Fields.ToplogyIdHigh));
  DEBUG ((DEBUG_INFO, "Router Topology Valid    = %d\n", RtCsInfo.RouterCs3.Fields.ToplogyIdValid));
  DEBUG ((DEBUG_INFO, "Router CM USB Version    = %d.%d\n", RtCsInfo.RouterCs4.Fields.CmUsb4VerMajor, RtCsInfo.RouterCs4.Fields.CmUsb4VerMinor));
  DEBUG ((DEBUG_INFO, "Router Notify Timeout    = %d\n", RtCsInfo.RouterCs4.Fields.NotificationTimeout));
  DEBUG ((DEBUG_INFO, "Router USB4 Version      = 0x%0X\n", RtCsInfo.RouterCs4.Fields.Usb4Version));
  DEBUG ((DEBUG_INFO, "Router CS4 = 0x%08X\n", RtCsInfo.RouterCs4));
  DEBUG ((DEBUG_INFO, "Router CS5 = 0x%08X\n", RtCsInfo.RouterCs5));
  DEBUG ((DEBUG_INFO, "Router CS6 = 0x%08X\n", RtCsInfo.RouterCs6));
  DEBUG ((DEBUG_INFO, "Router CS7 = 0x%08X\n", RtCsInfo.RouterCs7));
  DEBUG ((DEBUG_INFO, "Router CS8 = 0x%08X\n", RtCsInfo.RouterCs8));

  DEBUG ((DEBUG_INFO, "Set Router 0x%016llx TopologyId Valid successfully\n", Router->TopologyId));
Exit:
  DEBUG ((DEBUG_INFO, "Usb4RtEnumerate exit\n"));
  return Status;
}

/**
  Enumerate USB4 device router and initialize the router instance based on Topology ID.

  @param[in,out] Router  - Pointer to Router instance.

  @retval EFI_SUCCESS           - Router initialization success.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval Errors in config space access
**/
EFI_STATUS
Usb4DevRtInit (
  IN OUT PUSB4_ROUTER    Router
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "Usb4DevRtInit entry\n"));
  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_DEVRT_INIT_ENTRY, 0, 0);

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4DevRtInit: Null Router!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_RT_TOPOLOGY, Router->TopologyId.Id.High, Router->TopologyId.Id.Low);

  //
  // Detect the host router type and set TBT3 flag for TBT3 host router.
  // CM needs to handle USB4 Router and TBT3 Router differently for some operations
  // Now only Using Tbt3 flag in the same function for TBT3 and USB4 difference since no much difference so far.
  // Router interface is not really required now.
  //
  Status = Usb4RtDetect (Router);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4DevRtInit: Fail to identify router, %r\n", Status));
    goto Exit;
  }

  //
  // Set Topology ID and Topology valid bit to enumerate the Router.
  // This must be done before any access to the router other than router config space.
  //
  Status = Usb4RtEnumerate (Router);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4DevRtInit: Enumerate Router failure, %r\n", Status));
    goto Exit;
  }

  //
  // <TBT3>
  // Set Link configured for TBT3 router
  // This is required if new router or the parent router is TBT3 router.
  // (refer to TBT3 CM guide 3.2.2)
  //
  if ((Router->ParentRt != NULL) && ((Router->ParentRt->State.Tbt3 == 1) || (Router->State.Tbt3 == 1))) {
    Status = Tbt3RtLinkConfigured (Router);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Usb4DevRtInit: Fail to set link configured\n"));
      goto Exit;
    }
  }

  //
  // <TBT3>
  // Set TBT3 VSEC6 CM bit and SLI bit for TBT3 router.
  // (Refer to TBT3 CM guide 3.2.2)
  //
  if (Router->State.Tbt3 == 1) {
    Status = Tbt3SetCmSli (Router);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Usb4DevRtInit: Fail to set TBT3 LC_SX_CTRL CM bit and SLI bit, %r\n", Status));
      goto Exit;
    }
  }

  //
  // <TBT3>
  // Enable AlpineRidge USB hot plug events
  // (Refer to CM TBT3 Device Support 4.1.2 xHC enablement)
  //
  if (Router->RouterType == USB4_ROUTER_ALPINE_RIDGE) {
    Status = Tbt3ArEnableUsbPlugEvents (Router);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Usb4DevRtInit: Fail to enable USB hot plug events for AlpineRidge\n"));
      goto Exit;
    }
  }

  //
  // Scan router adapters and adapter capabilities
  //
  Status = Usb4RtScanAdapters (Router);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4DevRtInit: Enumerate router adapters failure, %r\n", Status));
    goto Exit;
  }

Exit:
  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_DEVRT_INIT_EXIT, (UINT32) Status, 0);
  DEBUG ((DEBUG_INFO, "Usb4DevRtInit exit\n"));
  return Status;
}

/**
  Generic USB4 router destroy.

  @param[in] Router - Pointer to USB4 Router instance.

  @retval EFI_SUCCESS     - USB4 Router destroy successfully.
  @retval EFI_UNSUPPORTED - Downstream router is still present.
**/
EFI_STATUS
Usb4RtDestroy (
  IN PUSB4_ROUTER   Router
  )
{
  EFI_STATUS    Status;

  DEBUG ((DEBUG_INFO, "Usb4RtDestroy entry\n"));

  if (Router != NULL) {
    DEBUG ((DEBUG_INFO, "Destroy Router 0x%016llX Depth %d\n", Router->TopologyId, Router->Depth));

    if (Router->DsRouterCount != 0) {
      DEBUG ((DEBUG_ERROR, "Downstream Routers should be removed before Router is destroyed!"));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }

    //
    // Remove the connection between Router and its Upstream Router/connected Adapter.
    //
    if (Router->UpperRtLane != NULL) {
      Router->UpperRtLane->AdpDep.Lane.DsRouter = NULL;
      if (Router->UpperRtLane->Router != NULL) {
        if (Router->UpperRtLane->Router->DsRouterCount == 0) {
          DEBUG ((DEBUG_ERROR, "Usb4RtDestroy: Parent router's downstream router count is 0\n"));
          ASSERT (0);
        } else {
          Router->UpperRtLane->Router->DsRouterCount--;
        }
      } else {
        DEBUG ((DEBUG_ERROR, "Usb4RtDestroy: Null Router in UpperRtLane! Rt 0x%016llX\n", Router->TopologyId));
        ASSERT (0);
      }
      Router->UpperRtLane = NULL;
    }

    Usb4DomainFreeRouter (Router);
  }
  Status = EFI_SUCCESS;

Exit:
  DEBUG ((DEBUG_INFO, "Usb4RtDestroy exit\n"));
  return Status;
}

/**
  Generic USB4 router instance creation

  @param[in]  TopologyId     - Pointer to Router Topology ID.
  @param[in]  Depth          - Router depth.
  @param[in]  DomainContext  - USB4 domain context.
  @param[out] RouterInstance - Pointer of pointer to the created USB4 Router instance.

  @retval EFI_SUCCESS           - Create USB4 router instance successfully.
  @retval EFI_OUT_OF_RESOURCES  - Insufficient resources to create USB4 router instance.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4RtCreate (
  IN  PTOPOLOGY_ID          TopologyId,
  IN  UINT8                 Depth,
  IN  USB4_DOMAIN_CONTEXT   DomainContext,
  OUT USB4_ROUTER           **RouterInstance
  )
{
  EFI_STATUS     Status;
  USB4_ROUTER    *Router;

  DEBUG ((DEBUG_INFO, "Usb4RtCreate entry\n"));

  Router = NULL;

  if ((TopologyId == NULL) || (RouterInstance == NULL)) {
    DEBUG ((DEBUG_ERROR, "Usb4RtCreate: Invalid parameter (%p,%p)\n", TopologyId, RouterInstance));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Depth > mUsb4TopologyDepth) {
    DEBUG ((DEBUG_ERROR, "Usb4RtCreate: Depth=%d, router can only support depth up to 5\n", Depth));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Get a free router instance from domain.
  //
  Status = Usb4DomainGetFreeRouter (DomainContext, &Router);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Usb4RtCreate: Fail to get free router instance from domain\n"));
    goto Exit;
  }

  //
  // Set the required information for Router enumeration.
  //
  DEBUG ((DEBUG_INFO, "Usb4RtCreate: Depth = %d, TopologyID = 0x%016llX\n", Depth, TopologyId->Value));
  Router->TopologyId.Value = TopologyId->Value;
  Router->Depth            = Depth;

  *RouterInstance = Router;
  Status = EFI_SUCCESS;
Exit:
  if ((EFI_ERROR (Status))) {
    if (Router != NULL) {
      Usb4LogWrite (mUsb4Log, USB4_DBG_ERROR, USB4_EVT_CODE_RT_DESTROY_ERROR, (UINT32) Status, 0);
      Usb4RtDestroy (Router);
    }
  }

  DEBUG ((DEBUG_INFO, "Usb4RtCreate exit\n"));
  return Status;
}

/**
  Create the downstream router instance if a router connects to the downstream Lane adapter.

  @param[in]  Router     - Pointer to Router instance.
  @param[in]  AdpNum     - Downstream Adapter number.
  @param[out] DsRouter   - Pointer of pointer to Downstream Router.

  @retval EFI_SUCCESS           - Enumerate the connected Downstream Router success.
  @retval EFI_NOT_FOUND         - No Downstream Router is connected.
  @retval EFI_UNSUPPORTED       - Not a Lane adapter or depth exceeds the maximum.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
CreateDsRouter (
  IN  PUSB4_ROUTER    Router,
  IN  UINT8           AdpNum,
  OUT USB4_ROUTER     **DsRouter
  )
{
  EFI_STATUS           Status;
  TOPOLOGY_ID          TopologyId;
  LANE_ADAPTER_STATE   LaneAdapterState;
  PUSB4_ROUTER         NewRouter;
  UINT32               Retry;

  DEBUG ((CM_VERBOSE, "CreateDsRouter entry - Adapter = %d\n", AdpNum));

  if ((Router == NULL) || (DsRouter == NULL)) {
    DEBUG ((DEBUG_ERROR, "CreateDsRouter: Invalid parameter (%p,%p)\n", Router, DsRouter));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "CreateDsRouter: Invalid AdpNum %d, max = %d\n", AdpNum, Router->MaxAdapter));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Router->Adapter[AdpNum].Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "CreateDsRouter: Invalid adapter type 0x%08X\n", Router->Adapter[AdpNum].Type));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  if (Router->Depth >= mUsb4TopologyDepth) {
    DEBUG ((DEBUG_ERROR, "CreateDsRouter: Can't enumerate downstream Router at depth %d\n", Router->Depth));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Query Lane Adapter State
  // Enumerate the Downstream Router only if the adapter state is CL0
  //
  Status = QueryLaneAdapterState (Router, AdpNum, &LaneAdapterState);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "CreateDsRouter: Query Lane Adapter State failure, %r\n", Status));
    goto Exit;
  }
  DEBUG ((CM_DEBUG, "Router 0x%016llX adapter %d state = %d\n", Router->TopologyId, AdpNum, LaneAdapterState));

  //
  // Lane adapter state may be in transition to CL0
  //
  if (LaneAdapterState == LANE_STATE_TRAINING_BONDING) {
    for (Retry = 0; Retry < USB4_TRAINING_WAITS; Retry++) {
      gBS->Stall (CM_STATUS_POLL_US);
      Status = QueryLaneAdapterState (Router, AdpNum, &LaneAdapterState);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "CreateDsRouter: Query Lane Adapter State failure, %r\n", Status));
        goto Exit;
      }
      if (LaneAdapterState != LANE_STATE_TRAINING_BONDING) {
        break;
      }
    }
    DEBUG ((CM_DEBUG, "Router 0x%016llX adapter %d state = %d, Retry = %d\n", Router->TopologyId, AdpNum, LaneAdapterState, Retry));
  }

  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_ADP_STATE, (UINT32) AdpNum, (UINT32) LaneAdapterState);

  if (LaneAdapterState != LANE_STATE_CL0) {
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  DEBUG ((CM_DEBUG, "Router 0x%016llX adapter %d is in CL0\n", Router->TopologyId, AdpNum));

  //
  // Unlock the downstream Lane adapter so that the control packets can be forwarded to the downstream facing adapter.
  //
  Status = LaneAdapterUnlock (Router, AdpNum);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unlock Router 0x%016llX adapter %d failure, %r\n", Router->TopologyId, AdpNum, Status));
    goto Exit;
  }

  //
  // Set Topology ID of the downstream Router based on downstream adapter number and depth
  //
  TopologyId.Value = Router->TopologyId.Value;
  TopologyId.Adapter[Router->Depth] = AdpNum;

  //
  // Create Router instance for the downstream Router
  //
  Status = Usb4RtCreate (&TopologyId, Router->Depth + 1, Router->DomainContext, &NewRouter);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "CreateDsRouter: Can't create instance for Router 0x%016llX\n", TopologyId.Value));
    goto Exit;
  }

  //
  // Set parent router information of new router
  //
  NewRouter->ParentRt     = Router;
  NewRouter->ParentAdpNum = AdpNum;
  NewRouter->UpperRtLane  = &Router->Adapter[AdpNum];

  //
  // Set downstream information
  //
  Router->Adapter[AdpNum].AdpDep.Lane.DsRouter = NewRouter;
  Router->DsRouterCount++;

  //
  // Enumerate the router and initialize the Router instance.
  //
  Status = Usb4DevRtInit (NewRouter);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "CreateDsRouter: Usb4DevRtInit failure, %r\n", Status));
    Usb4RtDestroy (NewRouter);
    goto Exit;
  }

  *DsRouter = NewRouter;

Exit:
  DEBUG ((CM_VERBOSE, "CreateDsRouter exit\n"));
  return Status;
}

/**
  Enumerate the downstream Router and all of its downstream Routers at the specified Lane adapter.

  @param[in] Router      - Pointer to a Router instance that needs downstream enumeration
  @param[in] AdpNum      - Lane Adapter number for enumerating the downstream Router

  @retval EFI_SUCCESS           - Enumerate and add downstream Routers success.
  @retval EFI_UNSUPPORTED       - Downstream router enumeration can't be supported.
  @retval EFI_NOT_FOUND         - Downstream router is not found at the specified adapter.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval Errors in config space access.
**/
EFI_STATUS
Usb4RtEnumDsRouter (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum
  )
{
  EFI_STATUS        Status;
  PUSB4_ROUTER      DsRouter;

  DEBUG ((DEBUG_INFO, "Usb4RtEnumDsRouter entry\n"));

  DsRouter = NULL;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4RtEnumDsRouter: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter)) {
    DEBUG ((DEBUG_ERROR, "Usb4RtEnumDsRouter: Invalid AdpNum %d, max = %d\n", AdpNum, Router->MaxAdapter));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Router->Depth >= mUsb4TopologyDepth) {
    DEBUG ((DEBUG_ERROR, "Usb4RtEnumDsRouter: Can't enumerate downstream adapters at depth %d\n", Router->Depth));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Create the downstream router if the downstream router is present.
  //
  Status = CreateDsRouter (Router, AdpNum, &DsRouter);
  if (EFI_ERROR (Status)) {
    if (Status != EFI_NOT_FOUND) {
      DEBUG ((DEBUG_ERROR, "<U4CmErr> Fail to create the downstream router, %r\n", Status));
    }
    goto Exit;
  }

  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_DSRT_FOUND, (UINT32) AdpNum, 0);

  //
  // Perform lane bonding between the router and the downstream router for dual lane support
  // Single lane is performed if lane bonding is not successful
  //
  Status = AdapterLaneBonding (Router, AdpNum);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "<U4CmErr> Rt 0x%016llX Adp %d lane bonding failure, %r\n", Router->TopologyId, AdpNum, Status));
    DEBUG ((DEBUG_WARN, "Single Lane is performed in adapter due to Lane Bonding failure\n"));
  }

  Status = LinkConfigTmuMode (Router, AdpNum, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "<U4CmErr> Rt 0x%016llX Adp %d set TMU failure, %r\n", Router->TopologyId, AdpNum, Status));
    goto Exit;
  }

  //
  // Enable PCIe/USB3 protocol tunneling between router and downstream router
  // DP tunneling setup will be handled by DP hot plug events
  //
  Status = Usb4RtEnableDsProtocols (Router, AdpNum);
  if (EFI_ERROR (Status)) {
    //
    // PCIe/USB3 tunneling setup failure should not break the router enumeration.
    // DP tunneling may still work.
    //
    DEBUG ((DEBUG_ERROR, "<U4CmErr> PCIe/USB3 tunneling setup failure - Rt 0x%016llX Adp %d, %r\n", Router->TopologyId, AdpNum, Status));
  }

  //
  // Add the new enumerated Router to Domain Topology
  //
  Status = Usb4DomainAddRouter (DsRouter);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Add downstream Router 0x%016llX failure, %r\n", DsRouter->TopologyId.Value, Status));
    goto Exit;
  }

  //
  // Enumerate all downstream Routers of new enumerated Router
  //
  if (DsRouter->Depth < mUsb4TopologyDepth) {
    Status = Usb4RtEnumAllDsRouters (DsRouter);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "<U4CmErr> Enum downstream routers of Rt 0x%016llX failure %r\n", DsRouter->TopologyId.Value, Status));
      //
      // DsRouter is enumerated and added to domain successfully though the failure happens while enumerating its downstream routers.
      //
      Status = EFI_SUCCESS;
    }
  }
Exit:
  if (EFI_ERROR (Status) && (DsRouter != NULL)) {
    Usb4RtDestroy (DsRouter);
  }

  DEBUG ((DEBUG_INFO, "Usb4RtEnumDsRouter exit\n"));
  return Status;
}

/**
  Enumerate downstream Routers of a specific Router and add to Domain Topology

  @param[in] Router      - Pointer to a Router instance that needs downstream enumeration

  @retval EFI_SUCCESS           - Enumerate and add downstream Routers success
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval Errors in config space access.
**/
EFI_STATUS
Usb4RtEnumAllDsRouters (
  IN USB4_ROUTER     *Router
  )
{
  EFI_STATUS    Status;
  UINT8         Index;

  DEBUG ((DEBUG_INFO, "Usb4RtEnumAllDsRouters entry\n"));
  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_ENUM_ALLDSRT_ENTRY, 0, 0);

  Status = EFI_SUCCESS;

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4RtEnumAllDsRouters: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Router->Depth >= mUsb4TopologyDepth) {
    DEBUG ((DEBUG_ERROR, "Usb4RtEnumAllDsRouters: Can't enumerate downstream adapters at depth %d\n", Router->Depth));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_RT_TOPOLOGY, Router->TopologyId.Id.High, Router->TopologyId.Id.Low);

  //
  // Enumerate all downstream Lane0 adapters to find downstream Routers
  //
  for (Index = 1; (Index <= Router->MaxAdapter) && (Index < USB4_ADAPTER_SUPPORT_MAX); Index++) {
    if ((Router->Adapter[Index].Type != USB4_ADP_TYPE_LANE) || (Router->Adapter[Index].AdpDep.Lane.Status.Lane0 == FALSE)) {
      continue;
    }

    //
    // Skip upstream Lane adapter.
    //
    if (Index == (UINT32)Router->LaneUpAdpNum) {
      continue;
    }

    //
    // Enumerate and create downstream Router and all of its downstream Routers
    //
    Status = Usb4RtEnumDsRouter (Router, Index);
    if (EFI_ERROR (Status)) {
      //
      // EFI_NOT_FOUND is expected and not a failure if no device router connects to the downstream Lane adapter.
      // Keep enumerating the next downstream Lane adapter if enumeration failure happens at current Lane adapter.
      //
      if (Status != EFI_NOT_FOUND) {
        DEBUG ((DEBUG_ERROR, "<U4CmErr> Enum Rt 0x%016llX  Rt tree at adp %d error, %r\n", Router->TopologyId.Value, Index, Status));
      }
      Status = EFI_SUCCESS;
      continue;
    }
  }

Exit:
  Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_ENUM_ALLDSRT_EXIT, (UINT32) Status, 0);
  DEBUG ((DEBUG_INFO, "Usb4RtEnumAllDsRouters exit\n"));
  return Status;
}

/**
  Disconnect the downstream Routers connected at the given adapter from domain.

  @param[in] Router - Pointer to USB4 router instance.
  @param[in] AdpNum - Adapter number.

  @retval EFI_SUCCESS           - Disconnect and remove the downstream routers from domain successfully.
  @retval EFI_UNSUPPORTED       - Disconnect the downstream router failure.
  @retval EFI_INVALID_PARAMETER - Disconnect and remove downstream Routers from Domain Topology failure.
**/
EFI_STATUS
Usb4RtDsDisconnect (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum
  )
{
  EFI_STATUS      Status;
  PUSB4_ROUTER    DsRouter;

  DEBUG ((DEBUG_INFO, "Usb4RtDsDisconnect entry\n"));

  if (Router == NULL) {
    DEBUG ((DEBUG_ERROR, "Usb4RtDsDisconnect: Null Router\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if ((AdpNum == 0) || (AdpNum > Router->MaxAdapter) || (AdpNum >= USB4_ADAPTER_SUPPORT_MAX)) {
    DEBUG ((DEBUG_ERROR, "Usb4RtDsDisconnect: Invalid AdpNum %d, max = %d\n", AdpNum, Router->MaxAdapter));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Router->Adapter[AdpNum].Type != USB4_ADP_TYPE_LANE) {
    DEBUG ((DEBUG_ERROR, "Usb4RtDsDisconnect: Rt 0x%016llX Adp %d is not Lane adp\n", Router->TopologyId, AdpNum));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  DEBUG ((DEBUG_INFO, "Usb4RtDsDisconnect entry - Rt 0x%016llX Adp %d\n", Router->TopologyId, AdpNum));
  DsRouter = Router->Adapter[AdpNum].AdpDep.Lane.DsRouter;

  if (DsRouter != NULL) {
    Usb4LogWrite (mUsb4Log, USB4_DBG_INFO, USB4_EVT_CODE_RT_DISCONNECT, DsRouter->TopologyId.Id.High, DsRouter->TopologyId.Id.Low);
    //
    // Remove Protocol Tunneling settings related to connection between Router and the removed downstream Routers.
    //
    Status = Usb4RtRemoveDsProtocols (Router, AdpNum);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Usb4RtRemoveDsProtocols failure, (%r)\n", Status));
    }

    //
    // Disable DFP TMU if TMU Enhanced Uni-Directional Mode is enabled for USB4 Ver2 link
    //
    Status = LinkConfigTmuMode (Router, AdpNum, FALSE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "LinkConfigTmuMode DFP TMU disable failure, (%r)\n", Status));
    }

    //
    // Remove Router nodes from Domain Topology.
    //
    Status = Usb4DomainRemoveRouter (DsRouter);
  } else {
    DEBUG ((DEBUG_INFO, "Usb4RtDsDisconnect: Rt 0x%016llX Adp %d DsRouter is not present\n", Router->TopologyId, AdpNum));
    Status = EFI_SUCCESS;
  }

Exit:
  DEBUG ((DEBUG_INFO, "Usb4RtDsDisconnect exit\n"));
  return Status;
}