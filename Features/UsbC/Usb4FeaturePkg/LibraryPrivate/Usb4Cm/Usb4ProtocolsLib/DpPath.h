/** @file
  Header file for DisplayPort path configuration.

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

#ifndef _DP_PATH_H_
#define _DP_PATH_H_

#include <Uefi.h>
#include <Library/Usb4RouterDefs.h>

/**
  Enable/Disable Video/Aux path of DP adapter.

  @param[in] Router  - Pointer to Router.
  @param[in] AdpNum  - DP adapter number.
  @param[in] Enable  - Indicate the path should be enabled or disabled.

  @retval EFI_SUCCESS           - Enable Video/Aux path of DP adapter success.
  @retval EFI_UNSUPPORTED       - Enable Video/Aux path of DP adapter failure.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
DpAdpEnablePath (
  IN PUSB4_ROUTER    Router,
  IN UINT8           AdpNum,
  IN BOOLEAN         Enable
  );

/**
  Traverse and configure the path between DP source adapter and DP sink adapter.

  @param[in] DpSrcRt      - Pointer to DP Source Router.
  @param[in] DpSrcAdpNum  - DP-IN adapter number of DP source.
  @param[in] DpSinkRt     - Pointer to DP Sink Router.
  @param[in] DpSinkAdpNum - DP_OUT adapter number of DP sink.
  @param[in] StreamId     - DP stream ID for identifying DP path.

  @retval EFI_SUCCESS           - Setup DP path successfully.
  @retval EFI_UNSUPPORTED       - Fail to setup DP path.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
DpPathSetup (
  IN PUSB4_ROUTER    DpSrcRt,
  IN UINT8           DpSrcAdpNum,
  IN PUSB4_ROUTER    DpSinkRt,
  IN UINT8           DpSinkAdpNum,
  IN UINT32          StreamId
  );

/**
  Tear down the DP OUT path between the DP source and DP sink.
  If RemovedRt is specified, tear down DP path from DP source to RemovedRt's upper level router.

  @param[in] DpSrcRt      - Pointer to DP Source Router.
  @param[in] DpSrcAdpNum  - DP-IN adapter number of DP source.
  @param[in] DpSinkRt     - Pointer to DP Sink Router.
  @param[in] DpSinkAdpNum - DP_OUT adapter number of DP sink.
  @param[in] StreamId     - DP stream ID for identifying DP path.
  @param[in] RemovedRt    - Removed router in the DP path (optional)

  @retval EFI_SUCCESS           - DP path tear down success.
  @retval EFI_UNSUPPORTED       - Fail to tear down DP path.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
DpPathTearDown (
  IN PUSB4_ROUTER    DpSrcRt,
  IN UINT8           DpSrcAdpNum,
  IN PUSB4_ROUTER    DpSinkRt,
  IN UINT8           DpSinkAdpNum,
  IN UINT32          StreamId,
  IN PUSB4_ROUTER    RemovedRt
  );
#endif
