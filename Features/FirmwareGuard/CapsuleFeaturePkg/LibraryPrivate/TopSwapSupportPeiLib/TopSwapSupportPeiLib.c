/** @file
  PEI Top Swap support library.

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
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/SpiAccessLib.h>
#include <Library/TopSwapSupportLib.h>

/**
  Set/Clear TopSwap bit by request.

  @param[in] Enable  TRUE: Set TopSwap bit; FALSE: Clear TopSwap bit.

  @retval EFI_UNSUPPORTED  This function is not supported.

**/
EFI_STATUS
EFIAPI
TopSwapControl (
  IN BOOLEAN  Enable
  )
{
  DEBUG ((DEBUG_ERROR, "!!! Top Swap Control is not supported in PEI !!!\n"));
  ASSERT (FALSE);

  return EFI_UNSUPPORTED;
}

/**
  Returns Top Swap functionality enable state

  @retval TRUE    Top Swap is enabled
  @retval FALSE   Top Swap is disabled
**/
BOOLEAN
EFIAPI
IsTopSwapEnabled (
  VOID
  )
{
  return SpiIsTopSwapEnabled ();
}
