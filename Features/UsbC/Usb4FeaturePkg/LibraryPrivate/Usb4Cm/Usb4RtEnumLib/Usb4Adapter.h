/** @file
  Header file for USB4 adapter specific implementation.

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

#ifndef _USB4_ADAPTER_H_
#define _USB4_ADAPTER_H_

#include <Uefi.h>
#include <Library/Usb4RouterDefs.h>

/**
  Enumerate USB4 Adapter and its Capabilities.

  @param[in,out] Router - Pointer to USB4 Router instance.
  @param[in]     AdpNum - Adapter Number.
  @param[in]     Lane0  - Lane0 indicator. Enumerated adapter should be Lane0 in USB4 Port if it's Lane adapter.

  @retval EFI_SUCCESS           - Enumerate USB4 Adapter success.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval Errors in config space access.
**/
EFI_STATUS
Usb4AdpEnum (
  IN OUT PUSB4_ROUTER    Router,
  IN  UINT8              AdpNum,
  IN  BOOLEAN            Lane0
  );

/**
  Query Lane Adapter State.

  @param[in]  Router       - Pointer to Router instance.
  @param[in]  AdpNum       - Downstream Adapter number.
  @param[out] AdapterState - Lane adapter state.

  @retval EFI_SUCCESS           - Query Lane adapter state successfully.
  @retval EFI_UNSUPPORTED       - Not a Lane adapter or Lane adapter capability is not present.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
QueryLaneAdapterState (
  IN  PUSB4_ROUTER          Router,
  IN  UINT8                 AdpNum,
  OUT LANE_ADAPTER_STATE    *AdapterState
  );

/**
  Unlock Router Downstream Adapter.

  @param[in] Router - Pointer to Router instance.
  @param[in] AdpNum - Adapter number to be unlocked.

  @retval EFI_SUCCESS           - Unlock Router Downstream Adapter success.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
  @retval EFI_UNSUPPORTED       - Unsupported adapter type.
  @retval Errors in config space access.
**/
EFI_STATUS
LaneAdapterUnlock (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum
  );

/**
  Bonding dual lanes link in USB4 Port.

  @param[in,out] Router - Pointer to Router instance.
  @param[in]     AdpNum - Adapter Number.

  @retval EFI_SUCCESS           - Bonding dual lanes link success.
  @retval EFI_UNSUPPORTED       - Lane bonding is not supported.
  @retval EFI_TIMEOUT           - Lane bonding timeout
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
AdapterLaneBonding (
  IN OUT PUSB4_ROUTER    Router,
  IN     UINT8           AdpNum
  );

/**
  Configure link TMU mode.

  @param[in,out] Router - Pointer to Router instance.
  @param[in]     AdpNum - Adapter Number.
  @param[in]     Connect - TRUE  : Config link TMU for router connection
                           FALSE : Config link TMU for router disconnect

  @retval EFI_SUCCESS           - Config link TMU mode success.
  @retval EFI_UNSUPPORTED       - Config link TMU is not supported.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
LinkConfigTmuMode (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum,
  IN BOOLEAN         Connect
  );

#endif
