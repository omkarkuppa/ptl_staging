/** @file
  USB4 router management library header.

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

#ifndef _USB4_ROUTER_LIB_H_
#define _USB4_ROUTER_LIB_H_

#include <Uefi.h>
#include <Library/Usb4ConfigLayer.h>
#include <Library/Usb4CsIo.h>
#include <Library/Usb4RouterDefs.h>
#include <Library/Usb4Capabilities.h>
#include <Library/Usb4DomainDefs.h>

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
  );

/**
  Generic USB4 router destroy.

  @param[in] Router - Pointer to USB4 Router instance.

  @retval EFI_SUCCESS     - USB4 Router destroy successfully.
  @retval EFI_UNSUPPORTED - Downstream router is still present.
**/
EFI_STATUS
Usb4RtDestroy (
  IN PUSB4_ROUTER   Router
  );

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
  );

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
  );

/**
  Scan router adapters and find the connectivity between Lane adapter and PCIe/USB3 adapter.

  @param[in] Router  - Pointer to Router instance.

  @retval EFI_SUCCESS           - Scan router adapters successfully.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval Errors in config space access
**/
EFI_STATUS
Usb4RtScanAdapters (
  IN OUT PUSB4_ROUTER    Router
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

/**
  Dump DP-IN adapter capability.

  @param[in] DpInCap - Pointer to DP-IN adapter capability.
**/
VOID
DumpAdpDpInCap (
  IN PUSB4_ADP_CAP_DP_IN    DpInCap
  );

/**
  Dump DP-OUT adapter capability.

  @param[in] DpOutCap - Pointer to DP-OUT adapter capability.
**/
VOID
DumpAdpDpOutCap (
  IN PUSB4_ADP_CAP_DP_OUT    DpOutCap
  );

/**
  Dump path entry

  @param[in] PathEntry - Pointer to the path entry
**/
VOID
DumpPathEntry (
  IN PUSB4_PATH_CS_ENTRY  PathEntry
  );

#endif
