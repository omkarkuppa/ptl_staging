/** @file
  Header file for DisplayPort/PCIe/USB3 tunneling.

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

#ifndef _USB4_PROTOCOLS_H_
#define _USB4_PROTOCOLS_H_

#include <Library/Usb4RouterDefs.h>

/**
  Setup PCIe path between router's PCIe downstream adapter and downstream router's PCIe upstream adapter

  @param[in] Router - Pointer to Router instance.
  @param[in] AdpNum - Adapter number of Lane adapter to setup PCIe path.

  @retval EFI_SUCCESS           - Setup PCIe path successfully.
  @retval EFI_UNSUPPORTED       - Setup PCIe path failure.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4RtConfigDsPciePath (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum
  );

/**
  Remove PCIe Tunneling that are configured for the connection between the Router and the downstream Router.

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
  );

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
  );

/**
  Remove USB3 Tunneling that are configured for the connection between the Router and the downstream Router.

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
  );

/**
  Remove DP tunnel if the downstream router is in DP tunnel path.

  @param[in] Router    - Pointer to Router instance.
  @param[in] AdpNum    - Adapter number.

  @retval EFI_SUCCESS           - Remove PCIe Tunneling successfully.
  @retval EFI_UNSUPPORTED       - Remove PCIe Tunneling failure.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4RtRemoveDsDp (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum
  );

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
  );

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
  );
#endif
