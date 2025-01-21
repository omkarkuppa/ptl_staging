/** @file
  MCHBAR_CBO_EGRESS.h

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
**/

/* The following security policy groups are used by registers in this file:     */
/* CCF_BIOS_LT_W_PG                                                             */
/*  Security_ReadAccess_Str:                                                    */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |    */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/* CCF_IMR_W_PG                                                                 */
/*  Security_ReadAccess_Str:                                                    */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |    */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/* CCF_PAM_W_PG                                                                 */
/*  Security_ReadAccess_Str:                                                    */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI |             */
/*    HOSTIA_BOOT_SAI | PM_PCS_SAI | DFX_INTEL_MANUFACTURING_SAI |              */
/*    DFX_INTEL_PRODUCTION_SAI | DFX_THIRDPARTY_SAI                             */

#ifndef _MCHBAR_CBO_EGRESS_PTL_CDIE_h
#define _MCHBAR_CBO_EGRESS_PTL_CDIE_h

#include <Base.h>


// IP GUID: 6f51add6-f0d0-46e1-80f1-d4611e42a6a4
#define MCHBAR_CBO_EGRESS_ID 0x6f51add6f0d046e1

//************************************************ RegisterStructs

#ifdef SV_HOOKS

/** EGRESS_BIOS_LT_W_PG_CP_MCHBAR_CBO_EGRESS desc:
  * Register default value:        0x40001000208
  * Register full path in IP: ccf_gpsb_top/egress_sai_regs_all/mem_mchbar_cbo_egress_msg_mchbar_cbo_egress/egress_bios_lt_w_pg_cp
  * Std path: VER=1, UVM=ccf_gpsb_top.egress_sai_regs_all.egress_bios_lt_w_pg_cp, OSX=ccf_gpsb_top.egress_sai_regs_all_727_261.mem_mchbar_cbo_egress_msg_mchbar_cbo_egress.egress_bios_lt_w_pg_cp
  * Security PolicyGroup: CCF_BIOS_LT_W_PG
  * LT_SA Unit Control Policy Register
  */

#define EGRESS_BIOS_LT_W_PG_CP_MCHBAR_CBO_EGRESS_REG (0x00006828U)

typedef union {
  struct {
    UINT64 sai_mask : 64;

                            /* Bits[63:0], Access Type=RW, default=0x40001000208*/

                            /* --- */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} EGRESS_BIOS_LT_W_PG_CP_MCHBAR_CBO_EGRESS_STRUCT;

#define EGRESS_BIOS_LT_W_PG_CP_MCHBAR_CBO_EGRESS_WIDTH 64

#define EGRESS_BIOS_LT_W_PG_CP_MCHBAR_CBO_EGRESS_SAI_MASK_LSB 0x0000
#define EGRESS_BIOS_LT_W_PG_CP_MCHBAR_CBO_EGRESS_SAI_MASK_SIZE 0x0040

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** EGRESS_BIOS_LT_W_PG_WAC_MCHBAR_CBO_EGRESS desc:
  * Register default value:        0x4000100021A
  * Register full path in IP: ccf_gpsb_top/egress_sai_regs_all/mem_mchbar_cbo_egress_msg_mchbar_cbo_egress/egress_bios_lt_w_pg_wac
  * Std path: VER=1, UVM=ccf_gpsb_top.egress_sai_regs_all.egress_bios_lt_w_pg_wac, OSX=ccf_gpsb_top.egress_sai_regs_all_727_261.mem_mchbar_cbo_egress_msg_mchbar_cbo_egress.egress_bios_lt_w_pg_wac
  * Security PolicyGroup: CCF_BIOS_LT_W_PG
  * LT_SA Unit Write Access Register
  */

#define EGRESS_BIOS_LT_W_PG_WAC_MCHBAR_CBO_EGRESS_REG (0x00006830U)

typedef union {
  struct {
    UINT64 sai_mask : 64;

                            /* Bits[63:0], Access Type=RW, default=0x4000100021A*/

                            /* --- */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} EGRESS_BIOS_LT_W_PG_WAC_MCHBAR_CBO_EGRESS_STRUCT;

#define EGRESS_BIOS_LT_W_PG_WAC_MCHBAR_CBO_EGRESS_WIDTH 64

#define EGRESS_BIOS_LT_W_PG_WAC_MCHBAR_CBO_EGRESS_SAI_MASK_LSB 0x0000
#define EGRESS_BIOS_LT_W_PG_WAC_MCHBAR_CBO_EGRESS_SAI_MASK_SIZE 0x0040

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** IMR_W_PG_CP_MCHBAR_CBO_EGRESS desc:
  * Register default value:        0x40001000208
  * Register full path in IP: ccf_gpsb_top/cbregs_all_all/mem_mchbar_cbo_ingress_msg_mchbar_cbo_ingress/imr_w_pg_cp
  * Std path: VER=1, UVM=ccf_gpsb_top.cbregs_all_all.imr_w_pg_cp, OSX=ccf_gpsb_top.cbregs_all_all_727_274.mem_mchbar_cbo_ingress_msg_mchbar_cbo_ingress.imr_w_pg_cp
  * Security PolicyGroup: CCF_IMR_W_PG
  * IMR Unit Control Policy Register
  */

#define IMR_W_PG_CP_MCHBAR_CBO_EGRESS_REG (0x00006a20U)

typedef union {
  struct {
    UINT64 sai_mask : 64;

                            /* Bits[63:0], Access Type=RW, default=0x40001000208*/

                            /* --- */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} IMR_W_PG_CP_MCHBAR_CBO_EGRESS_STRUCT;

#define IMR_W_PG_CP_MCHBAR_CBO_EGRESS_WIDTH 64

#define IMR_W_PG_CP_MCHBAR_CBO_EGRESS_SAI_MASK_LSB 0x0000
#define IMR_W_PG_CP_MCHBAR_CBO_EGRESS_SAI_MASK_SIZE 0x0040

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** IMR_W_PG_WAC_MCHBAR_CBO_EGRESS desc:
  * Register default value:        0x4000100021A
  * Register full path in IP: ccf_gpsb_top/cbregs_all_all/mem_mchbar_cbo_ingress_msg_mchbar_cbo_ingress/imr_w_pg_wac
  * Std path: VER=1, UVM=ccf_gpsb_top.cbregs_all_all.imr_w_pg_wac, OSX=ccf_gpsb_top.cbregs_all_all_727_274.mem_mchbar_cbo_ingress_msg_mchbar_cbo_ingress.imr_w_pg_wac
  * Security PolicyGroup: CCF_IMR_W_PG
  * IMR Unit Write Access Register
  */

#define IMR_W_PG_WAC_MCHBAR_CBO_EGRESS_REG (0x00006a28U)

typedef union {
  struct {
    UINT64 sai_mask : 64;

                            /* Bits[63:0], Access Type=RW, default=0x4000100021A*/

                            /* --- */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} IMR_W_PG_WAC_MCHBAR_CBO_EGRESS_STRUCT;

#define IMR_W_PG_WAC_MCHBAR_CBO_EGRESS_WIDTH 64

#define IMR_W_PG_WAC_MCHBAR_CBO_EGRESS_SAI_MASK_LSB 0x0000
#define IMR_W_PG_WAC_MCHBAR_CBO_EGRESS_SAI_MASK_SIZE 0x0040

#endif      // SV_HOOKS

/** PAM_W_PG_CP_MCHBAR_CBO_EGRESS desc:
  * Register default value:        0x4000100021B
  * Register full path in IP: ccf_gpsb_top/cbregs_all_all/mem_mchbar_cbo_ingress_msg_mchbar_cbo_ingress/pam_w_pg_cp
  * Std path: VER=1, UVM=ccf_gpsb_top.cbregs_all_all.pam_w_pg_cp, OSX=ccf_gpsb_top.cbregs_all_all_727_274.mem_mchbar_cbo_ingress_msg_mchbar_cbo_ingress.pam_w_pg_cp
  * Security PolicyGroup: CCF_PAM_W_PG
  * PAM Unit Control Policy Register
  */

#define PAM_W_PG_CP_MCHBAR_CBO_EGRESS_REG (0x00006a30U)

typedef union {
  struct {
    UINT64 sai_mask : 64;

                            /* Bits[63:0], Access Type=RW, default=0x4000100021B*/

                            /* --- */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} PAM_W_PG_CP_MCHBAR_CBO_EGRESS_STRUCT;

#define PAM_W_PG_CP_MCHBAR_CBO_EGRESS_WIDTH 64

#define PAM_W_PG_CP_MCHBAR_CBO_EGRESS_SAI_MASK_LSB 0x0000
#define PAM_W_PG_CP_MCHBAR_CBO_EGRESS_SAI_MASK_SIZE 0x0040

/** PAM_W_PG_WAC_MCHBAR_CBO_EGRESS desc:
  * Register default value:        0xC000100021B
  * Register full path in IP: ccf_gpsb_top/cbregs_all_all/mem_mchbar_cbo_ingress_msg_mchbar_cbo_ingress/pam_w_pg_wac
  * Std path: VER=1, UVM=ccf_gpsb_top.cbregs_all_all.pam_w_pg_wac, OSX=ccf_gpsb_top.cbregs_all_all_727_274.mem_mchbar_cbo_ingress_msg_mchbar_cbo_ingress.pam_w_pg_wac
  * Security PolicyGroup: CCF_PAM_W_PG
  * PAM Unit Write Access Register
  */

#define PAM_W_PG_WAC_MCHBAR_CBO_EGRESS_REG (0x00006a38U)

typedef union {
  struct {
    UINT64 sai_mask : 64;

                            /* Bits[63:0], Access Type=RW, default=0xC000100021B*/

                            /* --- */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} PAM_W_PG_WAC_MCHBAR_CBO_EGRESS_STRUCT;

#define PAM_W_PG_WAC_MCHBAR_CBO_EGRESS_WIDTH 64

#define PAM_W_PG_WAC_MCHBAR_CBO_EGRESS_SAI_MASK_LSB 0x0000
#define PAM_W_PG_WAC_MCHBAR_CBO_EGRESS_SAI_MASK_SIZE 0x0040

#ifdef SV_HOOKS

/** IMRIAEXCBASE_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS desc:
  * Register default value:        0x00000000
  * Register full path in IP: ccf_gpsb_top/cbregs_all_all/mem_mchbar_cbo_ingress_msg_mchbar_cbo_ingress/imriaexcbase_mchbar_cbo_ingress
  * Std path: VER=1, UVM=ccf_gpsb_top.cbregs_all_all.imriaexcbase_mchbar_cbo_ingress, OSX=ccf_gpsb_top.cbregs_all_all_727_274.mem_mchbar_cbo_ingress_msg_mchbar_cbo_ingress.imriaexcbase_mchbar_cbo_ingress
  * Security PolicyGroup: CCF_IMR_W_PG
  * This register contains the base address of IA exclusion IMR.
  */

#define IMRIAEXCBASE_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_REG (0x00006a40U)

typedef union {
  struct {
    UINT64 lock : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit will lock all writeable settings
                               in this register, including itself.
                            */

    UINT64 rsvd_0 : 9; /**< Undefined - auto filled rsvd_[09:01] */

                            /* Bits[9:1], Access Type=RO, default=None*/

                            /* Reserved */

    UINT64 imriaexcbase : 32;

                            /* Bits[41:10], Access Type=RW/L, default=0x00000000*/

                            /*
                               This register contains the base address of
                               IA exclusion IMR.
                            */

    UINT64 rsvd_1 : 22; /**< Undefined - auto filled rsvd_[63:42] */

                            /* Bits[63:42], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} IMRIAEXCBASE_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_STRUCT;

#define IMRIAEXCBASE_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_WIDTH 64

#define IMRIAEXCBASE_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_LOCK_LSB 0x0000
#define IMRIAEXCBASE_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_LOCK_SIZE 0x0001
#define IMRIAEXCBASE_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_IMRIAEXCBASE_LSB 0x000a
#define IMRIAEXCBASE_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_IMRIAEXCBASE_SIZE 0x0020

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** IMRIAEXCLIMIT_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS desc:
  * Register default value:        0x00000000
  * Register full path in IP: ccf_gpsb_top/cbregs_all_all/mem_mchbar_cbo_ingress_msg_mchbar_cbo_ingress/imriaexclimit_mchbar_cbo_ingress
  * Std path: VER=1, UVM=ccf_gpsb_top.cbregs_all_all.imriaexclimit_mchbar_cbo_ingress, OSX=ccf_gpsb_top.cbregs_all_all_727_274.mem_mchbar_cbo_ingress_msg_mchbar_cbo_ingress.imriaexclimit_mchbar_cbo_ingress
  * Security PolicyGroup: CCF_IMR_W_PG
  * This register contains the limit address of IA exclusion IMR.
  */

#define IMRIAEXCLIMIT_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_REG (0x00006a48U)

typedef union {
  struct {
    UINT64 lock : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit will lock all writeable settings
                               in this register, including itself.
                            */

    UINT64 rsvd_0 : 9; /**< Undefined - auto filled rsvd_[09:01] */

                            /* Bits[9:1], Access Type=RO, default=None*/

                            /* Reserved */

    UINT64 imriaexclimit : 32;

                            /* Bits[41:10], Access Type=RW/L, default=0x00000000*/

                            /*
                               This register contains the limit address of
                               IA exclusion IMR.
                            */

    UINT64 rsvd_1 : 22; /**< Undefined - auto filled rsvd_[63:42] */

                            /* Bits[63:42], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} IMRIAEXCLIMIT_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_STRUCT;

#define IMRIAEXCLIMIT_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_WIDTH 64

#define IMRIAEXCLIMIT_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_LOCK_LSB 0x0000
#define IMRIAEXCLIMIT_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_LOCK_SIZE 0x0001
#define IMRIAEXCLIMIT_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_IMRIAEXCLIMIT_LSB 0x000a
#define IMRIAEXCLIMIT_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_IMRIAEXCLIMIT_SIZE 0x0020

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** IMRIAEXCBASE1_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS desc:
  * Register default value:        0x00000000
  * Register full path in IP: ccf_gpsb_top/cbregs_all_all/mem_mchbar_cbo_ingress_msg_mchbar_cbo_ingress/imriaexcbase1_mchbar_cbo_ingress
  * Std path: VER=1, UVM=ccf_gpsb_top.cbregs_all_all.imriaexcbase1_mchbar_cbo_ingress, OSX=ccf_gpsb_top.cbregs_all_all_727_274.mem_mchbar_cbo_ingress_msg_mchbar_cbo_ingress.imriaexcbase1_mchbar_cbo_ingress
  * Security PolicyGroup: CCF_IMR_W_PG
  * This register contains the base address of IA exclusion IMR.
  */

#define IMRIAEXCBASE1_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_REG (0x00006a50U)

typedef union {
  struct {
    UINT64 lock : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit will lock all writeable settings
                               in this register, including itself.
                            */

    UINT64 rsvd_0 : 9; /**< Undefined - auto filled rsvd_[09:01] */

                            /* Bits[9:1], Access Type=RO, default=None*/

                            /* Reserved */

    UINT64 imriaexcbase : 32;

                            /* Bits[41:10], Access Type=RW/L, default=0x00000000*/

                            /*
                               This register contains the base address of
                               IA exclusion IMR.
                            */

    UINT64 rsvd_1 : 22; /**< Undefined - auto filled rsvd_[63:42] */

                            /* Bits[63:42], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} IMRIAEXCBASE1_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_STRUCT;

#define IMRIAEXCBASE1_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_WIDTH 64

#define IMRIAEXCBASE1_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_LOCK_LSB 0x0000
#define IMRIAEXCBASE1_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_LOCK_SIZE 0x0001
#define IMRIAEXCBASE1_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_IMRIAEXCBASE_LSB 0x000a
#define IMRIAEXCBASE1_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_IMRIAEXCBASE_SIZE 0x0020

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** IMRIAEXCLIMIT1_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS desc:
  * Register default value:        0x00000000
  * Register full path in IP: ccf_gpsb_top/cbregs_all_all/mem_mchbar_cbo_ingress_msg_mchbar_cbo_ingress/imriaexclimit1_mchbar_cbo_ingress
  * Std path: VER=1, UVM=ccf_gpsb_top.cbregs_all_all.imriaexclimit1_mchbar_cbo_ingress, OSX=ccf_gpsb_top.cbregs_all_all_727_274.mem_mchbar_cbo_ingress_msg_mchbar_cbo_ingress.imriaexclimit1_mchbar_cbo_ingress
  * Security PolicyGroup: CCF_IMR_W_PG
  * This register contains the limit address of IA exclusion IMR.
  */

#define IMRIAEXCLIMIT1_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_REG (0x00006a58U)

typedef union {
  struct {
    UINT64 lock : 1;

                            /* Bits[0:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit will lock all writeable settings
                               in this register, including itself.
                            */

    UINT64 rsvd_0 : 9; /**< Undefined - auto filled rsvd_[09:01] */

                            /* Bits[9:1], Access Type=RO, default=None*/

                            /* Reserved */

    UINT64 imriaexclimit : 32;

                            /* Bits[41:10], Access Type=RW/L, default=0x00000000*/

                            /*
                               This register contains the limit address of
                               IA exclusion IMR.
                            */

    UINT64 rsvd_1 : 22; /**< Undefined - auto filled rsvd_[63:42] */

                            /* Bits[63:42], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} IMRIAEXCLIMIT1_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_STRUCT;

#define IMRIAEXCLIMIT1_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_WIDTH 64

#define IMRIAEXCLIMIT1_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_LOCK_LSB 0x0000
#define IMRIAEXCLIMIT1_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_LOCK_SIZE 0x0001
#define IMRIAEXCLIMIT1_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_IMRIAEXCLIMIT_LSB 0x000a
#define IMRIAEXCLIMIT1_MCHBAR_CBO_INGRESS_MCHBAR_CBO_EGRESS_IMRIAEXCLIMIT_SIZE 0x0020

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** BIOS_LT_W_PG_CP_MCHBAR_CBO_EGRESS desc:
  * Register default value:        0x40001000208
  * Register full path in IP: ccf_gpsb_top/cbregs_all_all/mem_mchbar_cbo_ingress_msg_mchbar_cbo_ingress/bios_lt_w_pg_cp
  * Std path: VER=1, UVM=ccf_gpsb_top.cbregs_all_all.bios_lt_w_pg_cp, OSX=ccf_gpsb_top.cbregs_all_all_727_274.mem_mchbar_cbo_ingress_msg_mchbar_cbo_ingress.bios_lt_w_pg_cp
  * Security PolicyGroup: CCF_BIOS_LT_W_PG
  * LT_SA Unit Control Policy Register
  */

#define BIOS_LT_W_PG_CP_MCHBAR_CBO_EGRESS_REG (0x00006a60U)

typedef union {
  struct {
    UINT64 sai_mask : 64;

                            /* Bits[63:0], Access Type=RW, default=0x40001000208*/

                            /* --- */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} BIOS_LT_W_PG_CP_MCHBAR_CBO_EGRESS_STRUCT;

#define BIOS_LT_W_PG_CP_MCHBAR_CBO_EGRESS_WIDTH 64

#define BIOS_LT_W_PG_CP_MCHBAR_CBO_EGRESS_SAI_MASK_LSB 0x0000
#define BIOS_LT_W_PG_CP_MCHBAR_CBO_EGRESS_SAI_MASK_SIZE 0x0040

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** BIOS_LT_W_PG_WAC_MCHBAR_CBO_EGRESS desc:
  * Register default value:        0x4000100021A
  * Register full path in IP: ccf_gpsb_top/cbregs_all_all/mem_mchbar_cbo_ingress_msg_mchbar_cbo_ingress/bios_lt_w_pg_wac
  * Std path: VER=1, UVM=ccf_gpsb_top.cbregs_all_all.bios_lt_w_pg_wac, OSX=ccf_gpsb_top.cbregs_all_all_727_274.mem_mchbar_cbo_ingress_msg_mchbar_cbo_ingress.bios_lt_w_pg_wac
  * Security PolicyGroup: CCF_BIOS_LT_W_PG
  * LT_SA Unit Write Access Register
  */

#define BIOS_LT_W_PG_WAC_MCHBAR_CBO_EGRESS_REG (0x00006a68U)

typedef union {
  struct {
    UINT64 sai_mask : 64;

                            /* Bits[63:0], Access Type=RW, default=0x4000100021A*/

                            /* --- */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} BIOS_LT_W_PG_WAC_MCHBAR_CBO_EGRESS_STRUCT;

#define BIOS_LT_W_PG_WAC_MCHBAR_CBO_EGRESS_WIDTH 64

#define BIOS_LT_W_PG_WAC_MCHBAR_CBO_EGRESS_SAI_MASK_LSB 0x0000
#define BIOS_LT_W_PG_WAC_MCHBAR_CBO_EGRESS_SAI_MASK_SIZE 0x0040

#endif      // SV_HOOKS

//************************************************ /RegisterStructs


#endif      // _MCHBAR_CBO_EGRESS_h

