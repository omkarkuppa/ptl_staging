/** @file
  Library header file for TPR Init.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#ifndef _PEI_TPR_INIT_LIB_H_
#define _PEI_TPR_INIT_LIB_H_

#include <Uefi.h>

#define MMIO_ACM_POLICY_STATUS_DMA_EN_OFFSET                          BIT29

/**
  This function disables TPR based DMA protection.
  @retval EFI_SUCCESS - TPR disabled successfully.
**/
EFI_STATUS
DisableTprDmaProtection (
  VOID
);

/**
  TPR configuration function.

**/
VOID
TprInit (
  VOID
);

#endif
