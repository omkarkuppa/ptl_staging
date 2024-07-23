/** @file
  Top Swap support library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification Reference:
**/
#ifndef __TOP_SWAP_SUPPORT_LIB_H__
#define __TOP_SWAP_SUPPORT_LIB_H__

#include <Uefi.h>


/**
  Set/Clear TopSwap bit by request.

  @param[in] Enable  TRUE: Set TopSwap bit; FALSE: Clear TopSwap bit.

  @retval EFI_UNSUPPORTED  This function is not supported.
  @retval EFI_SUCCESS      Triggered TopSwap enable/disable by request.

**/
EFI_STATUS
EFIAPI
TopSwapControl (
  IN BOOLEAN  Enable
  );

/**
  Returns Top Swap functionality enable state

  @retval TRUE    Top Swap is enabled
  @retval FALSE   Top Swap is disabled
**/
BOOLEAN
EFIAPI
IsTopSwapEnabled (
  VOID
  );

#endif
