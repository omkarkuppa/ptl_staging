/** @file
  General SAI definition

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
#ifndef _SAI_REGS_H_
#define _SAI_REGS_H_

#define B_SAI_HOSTIA_BOOT      BIT4
#define B_SAI_HOSTIA_SUNPASS   BIT3
#define B_SAI_HOSTIA_SMM       BIT2
#define B_SAI_HOSTIA_UCODE     BIT1
#define B_SAI_HOSTIA_POSTBOOT  BIT0

#define B_SAI_CSE_INTEL        BIT16
#define B_SAI_ISH_SAI          BIT24

#endif
