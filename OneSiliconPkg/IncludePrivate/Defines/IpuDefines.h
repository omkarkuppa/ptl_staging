/** @file
  Register names for IPU block
  <b>Conventions</b>:
  - Prefixes:
    - Definitions beginning with "R_" are registers
    - Definitions beginning with "B_" are bits within registers
    - Definitions beginning with "V_" are meaningful values of bits within the registers
    - Definitions beginning with "S_" are register sizes
    - Definitions beginning with "N_" are the bit position
  - IPU registers are denoted by "_IPU_" in register names
  - Registers / bits that are different between IPU generations are denoted by
    "_IPU_[generation_name]_" in register/bit names. e.g., "_IPU_TGL_"
  - Registers / bits that are different between SKUs are denoted by "_[SKU_name]"
    at the end of the register/bit names
  - Registers / bits of new devices introduced in a IPU generation will be just named
    as "_IPU_" without [generation_name] inserted.

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
#ifndef _IPU_DEFINES_
#define _IPU_DEFINES_

//
// Device 5 Equates
//
#define IPU_BUS_NUM    0x00
#define IPU_DEV_NUM    0x05
#define IPU_FUN_NUM    0x00


#define R_IPU_LBAR     (0x10)
#define R_IPU_UBAR     (0x14)

#endif
