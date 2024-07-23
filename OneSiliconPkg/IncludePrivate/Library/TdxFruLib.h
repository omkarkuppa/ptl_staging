/** @file
  This file contains TDX code for specific generation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef _TDX_FRU_LIB_H_
#define _TDX_FRU_LIB_H_

#include <Base.h>
#include <Library/BaseLib.h>
#include <Uefi/UefiBaseType.h>
#include <MemorySubSystemHandle.h>
#include <Library/DebugLib.h>

#define CONVERT_MB_TO_B 20

/**
  Calculate the SEAMRR size

  @param[in] MrcOutput  - MRC Output Parameter Structure

  @retval UINT32        - returns calculated Seamrr Size in MB
**/
UINT32
EFIAPI
CalculateSeamrrSize (
  IN MrcOutput *MrcOutputData
  );

/**
  Determine whether the Cache level allows TDX enablement or not.

  @retval EFI_SUCCESS     - Sufficient Cache is Available for enabling TDX
  @retval EFI_UNSUPPORTED - Cache level does not support TDX enablement
**/
EFI_STATUS
EFIAPI
IsSufficientCacheAvailable (
  VOID
  );

#endif // _TDX_FRU_LIB_H_
