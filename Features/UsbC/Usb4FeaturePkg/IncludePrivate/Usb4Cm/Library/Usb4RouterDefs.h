/** @file
  Header file for the generic USB4 Router implementation

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

#ifndef _USB4_ROUTER_DEFS_H_
#define _USB4_ROUTER_DEFS_H_

#include <Uefi.h>
#include <Library/Usb4Common.h>
#include <Library/Usb4ConfigSpace.h>
#include <Library/Usb4ConfigLayer.h>
#include <Library/Usb4CsIo.h>

#define RT_NOTIFICATION_TIMEOUT_IN_MS    0xFE
#define USB_VENDOR_ID_INTEL_1            0x8086
#define USB_VENDOR_ID_INTEL_2            0x8087

typedef struct _USB4_ROUTER  USB4_ROUTER, *PUSB4_ROUTER;

//
// Lane adapter status indicator
//
typedef struct {
  // Indicate Lane0 adapter
  UINT32 Lane0        : 1;
  // Indicate lane bonding enabled
  UINT32 LbEnabled    : 1;
  UINT32 Reserved     : 30;
} LANE_ADAPTER_STATUS, *PLANE_ADAPTER_STATUS;

//
// Lane adapter capability support fields
//
typedef struct {
  UINT32 SupportedLinkSpeeds     : 4;
  UINT32 SupportedLinkWidths     : 6;
  UINT32 Cl0sSupport             : 1;
  UINT32 Cl1Support              : 1;
  UINT32 Cl2Support              : 1;
  UINT32 Reserved                : 19;
} LANE_ADAPTER_CAP, *PLANE_ADAPTER_CAP;

//
// Lane adapter specific variables for lane adapter implementation
//
typedef struct _USB4_ADAPTER_LANE {
  LANE_ADAPTER_STATUS    Status;
  LANE_ADAPTER_CAP       AdpCap;
  // Downstream Router instance connecting this port
  PUSB4_ROUTER           DsRouter;
  // Port group capability offset
  UINT8                  PgCapReg;
  // Paired USB3 adapter number in router
  UINT8                  PairedUsbAdp;
  // Paired PCIe adapter number in router
  UINT8                  PairedPcieAdp;
  // Bit mask for used Egress HopId 10 (bit 0) ~ 41 (bit 31)
  UINT32                 HopIdMask;
  // Ingress HopId assigned for DP tunneling main path
  UINT16                 IngressDpMainHopId[USB4_DP_STREAM_MAX];
  // Ingress HopId assigned for DP tunneling aux path
  UINT16                 IngressDpAuxHopId[USB4_DP_STREAM_MAX];
  // Egress HopId assigned for DP tunneling main path
  UINT16                 EgressDpMainHopId[USB4_DP_STREAM_MAX];
  // Egress HopId assigned for DP tunneling aux path
  UINT16                 EgressDpAuxHopId[USB4_DP_STREAM_MAX];
} USB4_ADAPTER_LANE, *PUSB4_ADAPTER_LANE;

//
// USB3 adapter status indicator
//
typedef struct {
  // Indicate USB3 path is enabled in this USB3 adapter
  UINT32 PathEnable   : 1;
  UINT32 Reserved     : 31;
} USB3_ADAPTER_STATUS, *PUSB3_ADAPTER_STATUS;

//
// USB3 adapter specific variables for USB3 adapter implementation
//
typedef struct _USB4_ADAPTER_USB3 {
  USB3_ADAPTER_STATUS         Status;
  // Paired lane adapter in router
  UINT8                       PairedLaneAdp;
} USB4_ADAPTER_USB3, *PUSB4_ADAPTER_USB3;

//
// PCIe adapter status indicator
//
typedef struct {
  // Indicate PCIe path is enable in this PCIe adapter
  UINT32 PathEnable    : 1;
  UINT32 Reserved      : 31;
} PCIE_ADAPTER_STAUS, *PCIE_ADAPTER_STATUS;

//
// PCIe adapter specific variables for PCIe adapter implementation
//
typedef struct _USB4_ADAPTER_PCIE {
  PCIE_ADAPTER_STAUS          Status;
  UINT8                       PairedLaneAdp;
} USB4_ADAPTER_PCIE, *PUSB4_ADAPTER_PCIE;

//
// Adapter type specific structure
//
typedef union _USB4_ADAPTER_SPECIFIC {
  USB4_ADAPTER_LANE     Lane;
  USB4_ADAPTER_USB3     AdpUsb3;
  USB4_ADAPTER_PCIE     AdpPcie;
} USB4_ADAPTER_SPECIFIC, *PUSB4_ADAPTER_SPECIFIC;

//
// USB4 adapter structure
//
typedef struct _USB4_ADAPTER {
  // Adapter type defined in USB4 spec
  USB4_ADAPTER_TYPE        Type;
  // Router instance containing this adapter
  PUSB4_ROUTER             Router;
  // Max input HopId supported defined in adapter config space
  UINT16                   MaxInputHopId;
  // Max output HopId supported defined in adapter config space
  UINT16                   MaxOutputHopId;
  // Adapter number of this adapter
  UINT8                    AdpNum;
  // Next capability offset defined in adapter config space
  UINT8                    NextCap;
  // Lane/DP/PCIe/USB3 capability offset in adapter config space
  UINT8                    AdpCapReg;
  // TMU capability offset in adapter config space
  UINT8                    TmuCapReg;
  // VSC0 capability offset in adapter config space (Used by TBT3 implementation)
  UINT16                   Vsc0Reg;
  // VSC1 capability offset in adapter config space (Used by TBT3 implementation)
  UINT16                   Vsc1Reg;
  // Adapter type dependent structure
  USB4_ADAPTER_SPECIFIC    AdpDep;
} USB4_ADAPTER, *PUSB4_ADAPTER;

//
// USB4 router state indicator
//
typedef struct _ROUTER_STATE {
  // Router is in use
  UINT32 InUse           : 1;
  // Router is a TBT3 Router
  UINT32 Tbt3            : 1;
  // Internal USB3 Host controller is implemented
  UINT32 Usb3HcImpl      : 1;
  // Internal USB3 Host controller is enabled
  UINT32 Usb3HcOn        : 1;
  // PCIe Tunneling is enabled
  UINT32 PtOn            : 1;
  // USB3 Tunneling is enabled
  UINT32 UtOn            : 1;
  // USB3 adapter is present
  UINT32 Usb3            : 1;
  // PCIe adapter is present
  UINT32 Pcie            : 1;
  // DP-IN adapter is present
  UINT32 DpIn            : 1;
  // DP-OUT adapter is present
  UINT32 DpOut           : 1;
} ROUTER_STATE, *PROUTER_STATE;

//
// <TBT3>
// TBT3 router VSEC_6 information
//
typedef struct _TBT3_VSEC_6 {
  UINT8     Usb4Ports;
  UINT8     CommonRegionLen;
  UINT16    PortRegionLen;
} TBT3_VSEC_6, *PTBT3_VSEC_6;

//
// Router Types of router instance.
//
typedef enum _USB4_ROUTER_TYPE {
  USB4_ROUTER_HOST_ROUTER  = 1,
  USB4_ROUTER_ALPINE_RIDGE,
  USB4_ROUTER_TITAN_RIDGE,
  USB4_ROUTER_MAPLE_RIDGE,
  USB4_ROUTER_USB4,
  USB4_ROUTER_INVALID
} USB4_ROUTER_TYPE, *PUSB4_ROUTER_TYPE;

#define GET_ROUTER(Addr, Field)  ((PUSB4_ROUTER)((UINT8 *)Addr - (UINTN)(&((PUSB4_ROUTER)0)->Field)))

//
// USB4 router entity in domain
//
struct _USB4_ROUTER {
  // Router topology ID in domain (Defined in USB4 configuration layer)
  TOPOLOGY_ID              TopologyId;
  // Router depth in domain topology
  UINT8                    Depth;
  UINT16                   VendorId;
  UINT16                   DeviceId;
  USB4_ROUTER_TYPE         RouterType;
  UINT8                    CmVer;
  // Domain context of this router (Used for host interface access)
  USB4_DOMAIN_CONTEXT      DomainContext;

  // Router state
  ROUTER_STATE             State;
  UINT32                   Revision;
  UINT32                   NotifyTimeout;
  UINT32                   Usb4Version;
  // Max Port number supported by this Router
  UINT8                    MaxAdapter;
  // Router Adapter instances
  USB4_ADAPTER             Adapter[USB4_ADAPTER_SUPPORT_MAX];
  // Lane Upstream Adapter number
  UINT8                    LaneUpAdpNum;
  // PCIe Upstream Adapter number
  UINT8                    PcieUpAdpNum;
  // USB3 Upstream Adapter number
  UINT8                    Usb3UpAdpNum;
  // Host Interface Adapter number
  UINT8                    HiAdpNum;
  // Router TMU capability offset in Router CS
  UINT16                   TmuCapReg;
  // <TBT3> VSC1 offset for TBT3 support
  UINT16                   Vsc1Reg;
  // <TBT3> VSEC6 offset for TBT3 support
  UINT16                   Vsec6Reg;
  // <TBT3> VSEC6 structure used for TBT3 support
  TBT3_VSEC_6              Tbt3Vsec6;
  // Upstream router's lane adapter instance that this Router is connecting to
  PUSB4_ADAPTER            UpperRtLane;
  // Upstream router instance
  PUSB4_ROUTER             ParentRt;
  // Upstream router's downstream lane adapter number that this router is connecting to
  UINT8                    ParentAdpNum;
  // Link list entry used in domain topology management
  LIST_ENTRY               DomainEntry;
  // Downstream router count
  UINT32                   DsRouterCount;
};

#endif
