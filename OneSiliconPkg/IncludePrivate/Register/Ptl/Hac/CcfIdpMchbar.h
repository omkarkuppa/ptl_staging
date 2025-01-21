/** @file
  CCF_IDP_MCHBAR.h

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

/* The following security policy groups are used by registers in this file:     */
/* NCU_BIOS_LT_W_PG                                                             */
/*  Security_ReadAccess_Str:                                                    */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |    */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */

#ifndef _CCF_IDP_MCHBAR_PTL_CDIE_H_
#define _CCF_IDP_MCHBAR_PTL_CDIE_H_

#include <Base.h>


// IP GUID: aa400b5e-b9e1-4bb2-a016-3b07c2b1518d
#define CCF_IDP_MCHBAR_ID 0xaa400b5eb9e14bb2

//************************************************ RegisterStructs

/** REGBAR_NCU_CCF_IDP_MCHBAR desc:
  * Register default value:        0x00000000
  * Register full path in IP: ccf_gpsb_top/ncdecs/MSG_MCHBAR_MSG_MCHBAR_MMIO/REGBAR_0_0_0_MCHBAR_NCU
  * Security PolicyGroup: NCU_BIOS_LT_W_PG
  * This is the base address for the REGBAR space to generate SB messages.
  */

#define REGBAR_NCU_CCF_IDP_MCHBAR_REG    (0x00005400U)

typedef union {
  struct {
    UINT64 regbaren : 1; /**< REGBAR Enable */

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* 0:  REGBAR disabled 1:  REGBAR enabled */

    UINT64 rsvd_0 : 26; /**< Undefined - auto filled rsvd_[26:01] */

                            /* Bits[26:1], Access Type=RO, default=None*/

                            /* Reserved */

    UINT64 regbar : 15; /**< REGBAR Base Address */

                            /* Bits[41:27], Access Type=RW, default=0x00000000*/

                            /*
                               This field corresponds to bits x to 27 of
                               the base address REGBAR configuration space.
                               BIOS will program this register resulting
                               in a base address for a 128MB block of contiguous
                               memory address space. This register ensures
                               that a naturally aligned 128MB space is allocated
                               within the first 4TB of addressable memory
                               space.
                            */

    UINT64 rsvd_1 : 22; /**< Undefined - auto filled rsvd_[63:42] */

                            /* Bits[63:42], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} REGBAR_NCU_CCF_IDP_MCHBAR_STRUCT;

#define REGBAR_NCU_CCF_IDP_MCHBAR_WIDTH 64

#define REGBAR_NCU_CCF_IDP_MCHBAR_REGBAREN_LSB 0x0000
#define REGBAR_NCU_CCF_IDP_MCHBAR_REGBAREN_SIZE 0x0001
#define REGBAR_NCU_CCF_IDP_MCHBAR_REGBAR_LSB 0x001b
#define REGBAR_NCU_CCF_IDP_MCHBAR_REGBAR_SIZE 0x000f


/** VTDBAR_NCU_CCF_IDP_MCHBAR desc:
  * Register default value:        0x00000000
  * Register full path in IP: ccf_gpsb_top/ncdecs/MSG_MCHBAR_MSG_MCHBAR_MMIO/VTDBAR_0_0_0_MCHBAR_NCU
  * Std path: VER=1, UVM=ccf_gpsb_top.ncdecs.VTDBAR_0_0_0_MCHBAR_NCU, OSX=ccf_gpsb_top.ncdecs_728_102.MSG_MCHBAR_MSG_MCHBAR_MMIO_240_50.VTDBAR_0_0_0_MCHBAR_NCU
  * Security PolicyGroup: NCU_BIOS_LT_W_PG
  * This is the base address for the VTDs configuration space. There is no physical memory within this 512KB window that can be addressed. The 512KB reserved by this register does not alias to any PCI 2.3 compliant memory mapped space. On reset, the VTDs configuration space is disabled and must be enabled by writing a 1 to the relevant VTDBAREN[7:0] bit. All the bits in this register are locked in LT mode. BIOS programs this register after which the register cannot be altered.
  */

#define VTDBAR_NCU_CCF_IDP_MCHBAR_REG    (0x00005410U)

typedef union {
  struct {
    UINT64 vtdbaren : 8; /**< VTD BAR Enable */

                            /* Bits[7:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               Enable bit per 64KB
                               0: The corresponding 64KB range is disabled
                               and does not claim any memory
                               1: The corresponding 64KB range memory mapped
                               accesses are claimed and decoded appropriately
                               This bit will remain 0 if VTd capability is
                               disabled.
                            */

    UINT64 rsvd_0 : 11; /**< Undefined - auto filled rsvd_[18:08] */

                            /* Bits[18:8], Access Type=RO, default=None*/

                            /* Reserved */

    UINT64 vtdbar : 23; /**< VTD Base Address */

                            /* Bits[41:19], Access Type=RW, default=0x00000000*/

                            /*
                               This field corresponds to bits x to 19 of
                               the base address VTD configuration space.
                               BIOS will program this register resulting
                               in a base address for a 512KB block of contiguous
                               memory address space. This register ensures
                               that a naturally aligned 512KB space is allocated
                               within the first 4TB of addressable memory
                               space. System Software uses this base address
                               to program the VTD register set. All the Bits
                               in this register are locked in LT mode.
                            */

    UINT64 rsvd_1 : 22; /**< Undefined - auto filled rsvd_[63:42] */

                            /* Bits[63:42], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} VTDBAR_NCU_CCF_IDP_MCHBAR_STRUCT;

#define VTDBAR_NCU_CCF_IDP_MCHBAR_WIDTH 64

#define VTDBAR_NCU_CCF_IDP_MCHBAR_VTDBAREN_LSB 0x0000
#define VTDBAR_NCU_CCF_IDP_MCHBAR_VTDBAREN_SIZE 0x0008
#define VTDBAR_NCU_CCF_IDP_MCHBAR_VTDBAR_LSB 0x0013
#define VTDBAR_NCU_CCF_IDP_MCHBAR_VTDBAR_SIZE 0x0017


#endif      // _CCF_IDP_MCHBAR_h

