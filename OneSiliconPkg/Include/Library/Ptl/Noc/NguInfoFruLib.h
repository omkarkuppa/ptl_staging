/** @file
  @ Header file to get NGU information.

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
#ifndef _NGU_INFO_LIB_VER1_H_
#define _NGU_INFO_LIB_VER1_H_

#define NOC_MMIO_RELOC                0x10000
#define GET_NOC_EFFECTIVE_ADDRESS(x)  x + NOC_MMIO_RELOC

#define HBO_MMIO_RELOC                0x10C00
#define GET_HBO_EFFECTIVE_ADDRESS(x)  x + HBO_MMIO_RELOC

#endif /* _NGU_INFO_LIB_VER1_H_ */
