/** @file
  Library header file for NGU (Next Generation Uncore).

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
#ifndef _PEI_NGU_INIT_LIB_H_
#define _PEI_NGU_INIT_LIB_H_

#include <Uefi.h>
#include <CMrcInterface.h>

#define DRAIN_CONTROL_REG_CTRL_OFFSET  BIT1
#define DRAIN_CONTROL_REG_STS_OFFSET   BIT0

//
// TPR register bit definition
//
#define TPR_BASE_LIMIT_ADDR_MASK           (((UINT64) 0x3FFFFF) << 20)
#define TPR_CONTROL_BIT_OFFSET             BIT4

typedef struct {
  UINT32  BaseAddrRegOffset;
  UINT32  Cp;
} IMR_REGISTER_STRUCT;

/**
  Init and Install TPR INFO Hob

  @retval EFI_SUCCESS - TPR HOB installed successfully.
**/
EFI_STATUS
EFIAPI
InstallTprInfoHob (
  VOID
  );

/**
  This function initialize NOC.

  @param[in] Mc0Size                  - Size of MC0
  @param[in] Mc1Size                  - Size of MC1
  @param[in] RemapBase                - Remap Base Address
  @param[in] RemapLimit               - Remap Limit
  @param[in] IsDdr5                   - Memory Type
**/
VOID
EFIAPI
NocInit (
  IN UINT32   Mc0Size,
  IN UINT32   Mc1Size,
  IN UINT32   RemapBase,
  IN UINT32   RemapLimit,
  IN BOOLEAN  IsDdr5
  );

#endif
