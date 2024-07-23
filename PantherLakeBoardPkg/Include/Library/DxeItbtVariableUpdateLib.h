/** @file
  Function prototype of DXE ItbtVariableUpdateLib.

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

@par Specification Reference:
**/

#ifndef _DXE_ITBT_VARIABLE_UPDATE_LIB_H_
#define _DXE_ITBT_VARIABLE_UPDATE_LIB_H_

#include <Uefi/UefiBaseType.h>
#include <Library/PcdLib.h>

#if FixedPcdGetBool (PcdTcssSupport) == 1
/**
  Update Setup variable for keep DMA UUID
**/
VOID
UpdateItbtDmaUuidVariable (
  VOID
  );
#endif

/**
  Sync ITBT Variables
**/
VOID
SyncItbtVariables (
  VOID
  );

#endif // _DXE_ITBT_VARIABLE_UPDATE_LIB_H_
