/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#include <PiPei.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/EcMiscLib.h>
#include <PlatformBoardType.h>
#include <Library/PeiDxeBoardIdsLib.h>
#include <PlatformBoardId.h>
#include <PlatformBoardConfig.h>
#include <Library/BoardConfigLib.h>


EFI_STATUS
EFIAPI
PtlBoardDetect (
  VOID
  )
{
  UINTN          SkuId;
  UINT16         BoardId;
  SkuId      = 0;
  BoardId    = 0;

  if (LibPcdGetSku () != 0) {
    return EFI_SUCCESS;
  }

  BoardId = PcdGet16 (PcdBoardId);
  if (BoardId == 0) {
    DEBUG ((DEBUG_INFO, "Let's get Board information first ...\n"));
    GetBoardConfig ();
    BoardId = PcdGet16 (PcdBoardId);
  }

  DEBUG ((DEBUG_INFO, "Board Detection Callback\n"));
  SkuId = (UINTN)(LShiftU64((UINT64) PcdGet16 (PcdDisplayId), 24) |
                  LShiftU64((UINT64) PcdGet16 (PcdBoardBomId), 16) |
                  LShiftU64((UINT64) PcdGet16 (PcdBoardRev), 8) |
                  (BoardId));

  LibPcdSetSku (SkuId);
  DEBUG ((DEBUG_INFO, "SKU_ID: 0x%x\n", LibPcdGetSku()));
  return EFI_SUCCESS;
}
