/**@file
  Panther Lake Simics Multi-Board Initialization Post-Memory library

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
#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/BoardInitLib.h>
#include <Library/MultiBoardInitSupportLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>

#include <PlatformBoardId.h>

EFI_STATUS
EFIAPI
PtlBoardInitBeforeSiliconInit (
  VOID
  );

BOARD_POST_MEM_INIT_FUNC  mPtlBoardInitFunc = {
  PtlBoardInitBeforeSiliconInit,
  NULL, // BoardInitAfterSiliconInit
};

EFI_STATUS
EFIAPI
PeiPtlMultiBoardInitLibConstructor (
  VOID
  )
{

  return RegisterBoardPostMemInit (&mPtlBoardInitFunc);

}
