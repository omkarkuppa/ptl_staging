/** @file
  USB4 protocol tunneling library header.

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

#ifndef _USB4_PROTOCOLS_LIB_H_
#define _USB4_PROTOCOLS_LIB_H_

#include <Uefi.h>
#include <Library/Usb4RouterDefs.h>

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
  );

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
  );

/**
  Execute DP Tunneling Router Operation

  @param[in]  Router    - Pointer to Router instance.
  @param[in]  OpCode    - Router operation OpCode.
  @param[in]  DpNum     - DisplayPort number.
  @param[out] OpStatus  - Pointer to Router operation returned status.

  @retval EFI_SUCCESS           - Router operation execution success.
  @retval EFI_TIMEOUT           - Timeout failure of router operation execution.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4DpRtOpExec (
  IN PUSB4_ROUTER   Router,
  IN UINT32         OpCode,
  IN UINT32         DpNum,
  OUT UINT32        *OpStatus
  );

/**
  Enable DP Tunnel between DP Source and DP Sink.

  @param[in] DpSrcRt      - Pointer to router instance which contains DP-IN adapter connecting DP source.
  @param[in] DpSrcAdpNum  - Adapter number of DP-IN adapter connecting DP source.
  @param[in] DpSinkRt     - Pointer to router instance which contains DP-OUT adapter connecting DP sink.
  @param[in] DpSinkAdpNum - Adapter number of DP-OUT adapter connecting DP sink.
  @param[in] StreamId     - DP stream ID for identifying DP path.

  @retval EFI_SUCCESS           - Enable DP Tunnel success.
  @retval EFI_UNSUPPORTED       - Fail to enable DP Tunnel.
  @retval EFI_OUT_OF_RESOURCES  - Fail to get DP resources to enable DP tunnel.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4EnableDpTunnel (
  IN PUSB4_ROUTER    DpSrcRt,
  IN UINT8           DpSrcAdpNum,
  IN PUSB4_ROUTER    DpSinkRt,
  IN UINT8           DpSinkAdpNum,
  IN UINT32          StreamId
  );

/**
  Disable DP Tunnel between DP Source and DP Sink for DP adapter removal or router removal.
  - Disable DP path of DP-IN adapter and DP-OUT adapter.
  - Tear down DP path between DP-IN and DP-OUT (until upper level router of the removed router if any).
  - De-allocate DP resources

  @param[in] DpSrcRt      - Pointer to router instance which contains DP-IN adapter connecting DP source.
  @param[in] DpSrcAdpNum  - Adapter number of DP-IN adapter connecting DP source.
  @param[in] DpSinkRt     - Pointer to router instance which contains DP-OUT adapter connecting DP sink.
  @param[in] DpSinkAdpNum - Adapter number of DP-OUT adapter connecting DP sink.
  @param[in] StreamId     - DP stream ID for identifying DP path.
  @param[in] RemovedRt    - Removed router on DP path.
                            This is specified if DP sink router is under the removed router tree.
                            USB4 config space access is not available from the removed router.

  @retval EFI_SUCCESS           - Disable DP Tunnel success.
  @retval EFI_UNSUPPORTED       - Fail to disable DP Tunnel.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Usb4DisableDpTunnel (
  IN PUSB4_ROUTER    DpSrcRt,
  IN UINT8           DpSrcAdpNum,
  IN PUSB4_ROUTER    DpSinkRt,
  IN UINT8           DpSinkAdpNum,
  IN UINT32          StreamId,
  IN PUSB4_ROUTER    RemovedRt
  );
#endif
