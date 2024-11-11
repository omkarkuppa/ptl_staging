/**@file

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
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>

#include <PlatformBoardId.h>
#include <Library/PreSiliconEnvDetectLib.h>


EFI_STATUS
EFIAPI
PtlBoardDetect (
  VOID
  )
{
  if (LibPcdGetSku () != 0) {
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "Board Detection Callback\n"));

  // @todo Needs external solution to detect different SKUs
  if (IsSimicsEnvironment()) {
    LibPcdSetSku (SkuIdPtlSimics);
    PcdSetBoolS (PcdSpdPresent, TRUE);
  } else {
    LibPcdSetSku (SkuIdPtlUHLp5Rvp1);
  }
  DEBUG ((DEBUG_INFO, "SKU_ID: 0x%x\n", LibPcdGetSku()));

  return EFI_SUCCESS;
}
