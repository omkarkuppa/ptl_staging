/** @file
  Header file for CCE in MCHBAR

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

#ifndef _CCE_LIB_H_
#define _CCE_LIB_H_

#include <CMrcInterface.h>

// default vaule of Encrypt_PMEM field
#define ENCRYPTPMEM_DEFAULT                       1

// Bits 0-6 mask
#define BITS0TO6_MASK                             0x3F

// Bits 7-13 mask
#define BITS7TO13_MASK                            0x1FC0

// CMIM hash mask
#define CMIM_CCE_ADM_HASH_6_19_NGU_PMA_REG_MASK   0xFFFFFFFF

// TME_CONTROL register offset in CCE IP
#define R_SA_MCHBAR_CCE_TME_CONTROL_OFFSET        0

// Register shows status of CCE0 and CCE1 enable
#define R_SA_MCHBAR_MEMSS_PMA_MEM_CONFIG_REG      0x00013D04

// CCE0 enabled mask
#define B_SA_MCHBAR_MEMSS_PMA_CCE0_EN             BIT4

// CCE1 enabled mask
#define B_SA_MCHBAR_MEMSS_PMA_CCE1_EN             BIT5

typedef union {
  struct {
    UINT64 reserved_0 : 20; /**< reserved */

                            /* Bits[19:0], Access Type=RO, default=0x00000000*/

                            /* reserved */

    UINT64 remapbase : 22; /**< remap base */

                            /* Bits[41:20], Access Type=RW, default=0x0007FFFF*/

                            /*
                               Remap Base The value in this register defines
                               the lower boundary of the remap window. The
                               remap window is inclusive of this address.
                               These bits are compared with bits 41:20 of
                               the address and only they will be changed
                               in case of remapping. When the value in this
                               register is greater than the value programmed
                               into the Remap Limit register, the remap window
                               is disabled. These bits are Intel TXT lockable.
                            */

    UINT64 reserved_1 : 22; /**< reserved */

                            /* Bits[63:42], Access Type=RO, default=0x00000000*/

                            /* Reserved bits */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} REMAP_BASE_CCE_MEM_STRUCT;

typedef union {
  struct {
    UINT64 reserved_0 : 20; /**< reserved */

                            /* Bits[19:0], Access Type=RO, default=0x000FFFFF*/

                            /* Reserved bits */

    UINT64 remaplmt : 22; /**< remap limit */

                            /* Bits[41:20], Access Type=RW, default=0x00000000*/

                            /*
                               Remap limit The value in this regsiter defines
                               the upper boundary of the remap window. The
                               remap window is inclusive of this address.
                               These bits are compared with bits 41:20 of
                               the address and only they will be changed
                               in case of remapping. When the value in this
                               register is less than the value programmed
                               into the Remap Base register, the remap window
                               is disabled. These bits are Intel TXT lockable.
                            */

    UINT64 reserved_1 : 22; /**< reserved */

                            /* Bits[63:42], Access Type=RO, default=0x00000000*/

                            /* Reserved bits */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} REMAP_LIMIT_CCE_MEM_STRUCT;

/**
  Perform CCE IP initialization.

  @param[in] RemapBase       - Remap Base Address
  @param[in] RemapLimit      - Remap Limit

  @retval VOID - No value to return
**/
VOID
CceInit (
  IN UINT32   RemapBase,
  IN UINT32   RemapLimit
  );

#endif // _CCE_LIB_H_
