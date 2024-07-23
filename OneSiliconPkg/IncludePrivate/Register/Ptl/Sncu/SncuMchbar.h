/** @file
  SNCU_MCHBAR.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2007 Intel Corporation.

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

  This file contains Silicon register definitions.

  This is a generated file; please do not modify it directly.

**/

/* The following security policy groups are used by registers in this file:     */
/* SNCU_OS_W_PG                                                                 */
/*  Security_ReadAccess_Str:                                                    */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI | HOSTCP_PMA_SAI |      */
/*    CSE_INTEL_SAI | DFX_INTEL_MANUFACTURING_SAI | GT_PMA_SAI |                */
/*    DFX_INTEL_PRODUCTION_SAI | DFX_THIRDPARTY_SAI | DISPLAY_SAI               */
/* SNCU_PAM_W_PG                                                                */
/*  Security_ReadAccess_Str:                                                    */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI |             */
/*    HOSTIA_BOOT_SAI | PM_PCS_SAI | DFX_INTEL_MANUFACTURING_SAI |              */
/*    DFX_INTEL_PRODUCTION_SAI | DFX_THIRDPARTY_SAI                             */

#ifndef _HOSTBRIDGE_MCHBAR_PTL_CDIE_H_
#define _HOSTBRIDGE_MCHBAR_PTL_CDIE_H_

#include <Base.h>


// IP GUID: d6cfc617-ca66-4e6f-ac4f-1bfc126103f5
#define HOSTBRIDGE_MCHBAR_ID 0xd6cfc617ca664e6f

//************************************************ RegisterStructs


/** CAPID0_F_PCI_SNCU_MCHBAR desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/MSG_MCHBAR_MSG_MCHBAR_MMIO/CAPID0_F_0_0_0_PCI
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.CAPID0_F_0_0_0_PCI, OSX=sncu_sncu_top.sncdecs_728_102.MSG_MCHBAR_MSG_MCHBAR_MMIO_240_50.CAPID0_F_0_0_0_PCI
  * Security PolicyGroup: SNCU_OS_W_PG
  * Control of bits in this register are only required for customer visible SKU differentiation.
  */

#define CAPID0_F_PCI_HOSTBRIDGE_MCHBAR_REG     (0x00005610U)

typedef union {
  struct {
    UINT32 max_data_rate_at_gear1 : 7; /**< MAX DATA RATE AT GEAR1 */

                            /* Bits[6:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This field controls the max DDR data rate
                               at gear1 (it is equal to the QCLK ratio) in
                               33MHz granularity. 0 means unlimited
                            */

    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[07:07] */

                            /* Bits[7:7], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 lpddr5_en : 1; /**< LPDDR5 ENABLE */

                            /* Bits[8:8], Access Type=RW/L, default=0x00000000*/

                            /*
                               Allow LPDDR5 operation. PCODE will update
                               this field with the value of FUSE_LPDDR5_EN.
                            */

    UINT32 max_data_rate_lpddr5 : 9; /**< MAX DATA RATE LPDDR5 */

                            /* Bits[17:9], Access Type=RW/L, default=0x00000000*/

                            /*
                               LPDDR5 Maximum frequency capability in 33Mhz
                               units. (0 means unlimited).
                            */

    UINT32 ddr5_en : 1; /**< DDR5 ENABLE */

                            /* Bits[18:18], Access Type=RW/L, default=0x00000000*/

                            /*
                               Allow DDR5 operation. PCODE will update this
                               field with the value of FUSE_DDR5_EN.
                            */

    UINT32 max_data_rate_ddr5 : 9; /**< MAX DATA RATE DDR5 */

                            /* Bits[27:19], Access Type=RW/L, default=0x00000000*/

                            /*
                               DDR5 Maximum frequency capability in 33Mhz
                               units. (0 means unlimited).
                            */

    UINT32 rsvd_1 : 4; /**< Undefined - auto filled rsvd_[31:28] */

                            /* Bits[31:28], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CAPID0_F_PCI_HOSTBRIDGE_MCHBAR_STRUCT;

#define CAPID0_F_PCI_HOSTBRIDGE_MCHBAR_WIDTH 32

#define CAPID0_F_PCI_HOSTBRIDGE_MCHBAR_MAX_DATA_RATE_AT_GEAR1_LSB 0x0000
#define CAPID0_F_PCI_HOSTBRIDGE_MCHBAR_MAX_DATA_RATE_AT_GEAR1_SIZE 0x0007
#define CAPID0_F_PCI_HOSTBRIDGE_MCHBAR_LPDDR5_EN_LSB 0x0008
#define CAPID0_F_PCI_HOSTBRIDGE_MCHBAR_LPDDR5_EN_SIZE 0x0001
#define CAPID0_F_PCI_HOSTBRIDGE_MCHBAR_MAX_DATA_RATE_LPDDR5_LSB 0x0009
#define CAPID0_F_PCI_HOSTBRIDGE_MCHBAR_MAX_DATA_RATE_LPDDR5_SIZE 0x0009
#define CAPID0_F_PCI_HOSTBRIDGE_MCHBAR_DDR5_EN_LSB 0x0012
#define CAPID0_F_PCI_HOSTBRIDGE_MCHBAR_DDR5_EN_SIZE 0x0001
#define CAPID0_F_PCI_HOSTBRIDGE_MCHBAR_MAX_DATA_RATE_DDR5_LSB 0x0013
#define CAPID0_F_PCI_HOSTBRIDGE_MCHBAR_MAX_DATA_RATE_DDR5_SIZE 0x0009

#ifdef SV_HOOKS

/** SCRATCHPAD_MCHBAR_NCU_0_SNCU_MCHBAR desc:
  * Register default value:        0x00000000
  * Register full path in IP: sncu_sncu_top/sncdecs/MSG_MCHBAR_MSG_MCHBAR_MMIO/SCRATCHPAD_0_0_0_MCHBAR_NCU
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.SCRATCHPAD_0_0_0_MCHBAR_NCU[0], OSX=sncu_sncu_top.sncdecs_728_102.MSG_MCHBAR_MSG_MCHBAR_MMIO_240_50.SCRATCHPAD_0_0_0_MCHBAR_NCU[0]
  * Security PolicyGroup: SNCU_OS_W_PG
  * Software-visible Scratchpad registers for general purpose use by BIOS.  No side effects
  */

#define SCRATCHPAD_MCHBAR_NCU_0_HOSTBRIDGE_MCHBAR_REG (0x00005628U)

typedef union {
  struct {
    UINT32 data : 32; /**< Data */

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* Data field. */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} SCRATCHPAD_MCHBAR_NCU_0_HOSTBRIDGE_MCHBAR_STRUCT;

#define SCRATCHPAD_MCHBAR_NCU_0_HOSTBRIDGE_MCHBAR_WIDTH 32

#define SCRATCHPAD_MCHBAR_NCU_0_HOSTBRIDGE_MCHBAR_DATA_LSB 0x0000
#define SCRATCHPAD_MCHBAR_NCU_0_HOSTBRIDGE_MCHBAR_DATA_SIZE 0x0020
#define SCRATCHPAD_MCHBAR_NCU_0_HOSTBRIDGE_MCHBAR_COUNT0 (4)
#define SCRATCHPAD_MCHBAR_NCU_0_HOSTBRIDGE_MCHBAR_DIST0 (0x0004)

#endif      // SV_HOOKS

#ifdef SV_HOOKS
#define SCRATCHPAD_MCHBAR_NCU_1_HOSTBRIDGE_MCHBAR_REG (0x0000562cU)

typedef union {
  struct {
    UINT32 data : 32; /**< Data */

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* Data field. */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} SCRATCHPAD_MCHBAR_NCU_1_HOSTBRIDGE_MCHBAR_STRUCT;

#define SCRATCHPAD_MCHBAR_NCU_1_HOSTBRIDGE_MCHBAR_WIDTH 32

#define SCRATCHPAD_MCHBAR_NCU_1_HOSTBRIDGE_MCHBAR_DATA_LSB 0x0000
#define SCRATCHPAD_MCHBAR_NCU_1_HOSTBRIDGE_MCHBAR_DATA_SIZE 0x0020
#define SCRATCHPAD_MCHBAR_NCU_1_HOSTBRIDGE_MCHBAR_COUNT0 (4)
#define SCRATCHPAD_MCHBAR_NCU_1_HOSTBRIDGE_MCHBAR_DIST0 (0x0004)

#endif      // SV_HOOKS

#ifdef SV_HOOKS
#define SCRATCHPAD_MCHBAR_NCU_2_HOSTBRIDGE_MCHBAR_REG (0x00005630U)

typedef union {
  struct {
    UINT32 data : 32; /**< Data */

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* Data field. */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} SCRATCHPAD_MCHBAR_NCU_2_HOSTBRIDGE_MCHBAR_STRUCT;

#define SCRATCHPAD_MCHBAR_NCU_2_HOSTBRIDGE_MCHBAR_WIDTH 32

#define SCRATCHPAD_MCHBAR_NCU_2_HOSTBRIDGE_MCHBAR_DATA_LSB 0x0000
#define SCRATCHPAD_MCHBAR_NCU_2_HOSTBRIDGE_MCHBAR_DATA_SIZE 0x0020
#define SCRATCHPAD_MCHBAR_NCU_2_HOSTBRIDGE_MCHBAR_COUNT0 (4)
#define SCRATCHPAD_MCHBAR_NCU_2_HOSTBRIDGE_MCHBAR_DIST0 (0x0004)

#endif      // SV_HOOKS

#ifdef SV_HOOKS
#define SCRATCHPAD_MCHBAR_NCU_3_HOSTBRIDGE_MCHBAR_REG (0x00005634U)

typedef union {
  struct {
    UINT32 data : 32; /**< Data */

                            /* Bits[31:0], Access Type=RW/P, default=0x00000000*/

                            /* Data field. */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} SCRATCHPAD_MCHBAR_NCU_3_HOSTBRIDGE_MCHBAR_STRUCT;

#define SCRATCHPAD_MCHBAR_NCU_3_HOSTBRIDGE_MCHBAR_WIDTH 32

#define SCRATCHPAD_MCHBAR_NCU_3_HOSTBRIDGE_MCHBAR_DATA_LSB 0x0000
#define SCRATCHPAD_MCHBAR_NCU_3_HOSTBRIDGE_MCHBAR_DATA_SIZE 0x0020
#define SCRATCHPAD_MCHBAR_NCU_3_HOSTBRIDGE_MCHBAR_COUNT0 (4)
#define SCRATCHPAD_MCHBAR_NCU_3_HOSTBRIDGE_MCHBAR_DIST0 (0x0004)

#endif      // SV_HOOKS

/** SNCU_PAM_W_PG_CP_HOSTBRIDGE_MCHBAR desc:
  * Register default value:        0x4000100021B
  * Register full path in IP: sncu_sncu_top/sncdecs/MEM_NCDECS_MMIO_SECURITY_MSG_NCDECS_MMIO_SECURITY/SNCU_PAM_W_PG_CP
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.SNCU_PAM_W_PG_CP, OSX=sncu_sncu_top.sncdecs_728_102.MEM_NCDECS_MMIO_SECURITY_MSG_NCDECS_MMIO_SECURITY_240_59.SNCU_PAM_W_PG_CP
  * Security PolicyGroup: SNCU_PAM_W_PG
  * Unit Control Policy Registe
  */

#define SNCU_PAM_W_PG_CP_HOSTBRIDGE_MCHBAR_REG (0x00005760U)

typedef union {
  struct {
    UINT64 sai_mask; /**< Control Policy */

                            /* Bits[63:0], Access Type=RW, default=0x4000100021B*/

                            /*
                               Bit-vector used to determine which agents
                               are allowed write access to the policy access
                               control registers of this policy group, based
                               on the value from each agent's 6bit SAI field.
                            */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} SNCU_PAM_W_PG_CP_HOSTBRIDGE_MCHBAR_STRUCT;

#define SNCU_PAM_W_PG_CP_HOSTBRIDGE_MCHBAR_WIDTH 64

#define SNCU_PAM_W_PG_CP_HOSTBRIDGE_MCHBAR_SAI_MASK_LSB 0x0000
#define SNCU_PAM_W_PG_CP_HOSTBRIDGE_MCHBAR_SAI_MASK_SIZE 0x0040

/** SNCU_PAM_W_PG_WAC_HOSTBRIDGE_MCHBAR desc:
  * Register default value:        0xC000100021B
  * Register full path in IP: sncu_sncu_top/sncdecs/MEM_NCDECS_MMIO_SECURITY_MSG_NCDECS_MMIO_SECURITY/SNCU_PAM_W_PG_WAC
  * Std path: VER=1, UVM=sncu_sncu_top.sncdecs.SNCU_PAM_W_PG_WAC, OSX=sncu_sncu_top.sncdecs_728_102.MEM_NCDECS_MMIO_SECURITY_MSG_NCDECS_MMIO_SECURITY_240_59.SNCU_PAM_W_PG_WAC
  * Security PolicyGroup: SNCU_PAM_W_PG
  * Unit Write Access Register
  */

#define SNCU_PAM_W_PG_WAC_HOSTBRIDGE_MCHBAR_REG (0x00005770U)

typedef union {
  struct {
    UINT64 sai_mask; /**< Write Access Control Policy */

                            /* Bits[63:0], Access Type=RW, default=0xC000100021B*/

                            /*
                               Bit-vector used to determine which agents
                               are allowed write access to registers in this
                               policy group, based on the value from each
                               agent's 6bit SAI field.
                            */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} SNCU_PAM_W_PG_WAC_HOSTBRIDGE_MCHBAR_STRUCT;

#define SNCU_PAM_W_PG_WAC_HOSTBRIDGE_MCHBAR_WIDTH 64

#define SNCU_PAM_W_PG_WAC_HOSTBRIDGE_MCHBAR_SAI_MASK_LSB 0x0000
#define SNCU_PAM_W_PG_WAC_HOSTBRIDGE_MCHBAR_SAI_MASK_SIZE 0x0040

//************************************************ /RegisterStructs

#endif      // _HOSTBRIDGE_MCHBAR_h

