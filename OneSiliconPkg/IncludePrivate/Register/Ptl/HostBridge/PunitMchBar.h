/** @file
  PUNIT_MCHBAR.h

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
/* PUNIT_BIOS_MEM_SAI_POLICY                                                    */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | RESERVED_1_SAI | RESERVED_2_SAI |  */
/*    HOSTIA_SEAM_SAI | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI |        */
/*    VTD_SAI | PM_DIE_TO_DIE_SAI | UNASSIGNED_1C_SAI | HOSTCP_PMA_SAI |        */
/*    CSE_INTEL_SAI | DMU_UNTRUSTED_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |     */
/*    GSC_SAI | PM_IOSS_SAI | UNASSIGNED_2C_SAI | UNASSIGNED_2E_SAI |           */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | ISH_SAI |               */
/*    CLINK_KVM_SAI | NPK_SAI | TBT_SAI | IOM_SAI | HW_PCH_SAI | SPI_SAI |      */
/*    UNASSIGNED_42_SAI | ESE_UNTRUSTED_SAI | GT_PMA_SAI | ESE_SAI |            */
/*    CSE_UNTRUSTED_SAI | UNASSIGNED_4C_SAI | UNASSIGNED_4E_SAI | IPU_SAI |     */
/*    RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI | DFX_THIRDPARTY_SAI |          */
/*    DISPLAY_SAI | LT_SAI | UNASSIGNED_5C_SAI | DISPLAY_KVM_SAI |              */
/*    UNASSIGNED_60_SAI | ACE_SAI | UNASSIGNED_64_SAI | CSME_SMS_SAI |          */
/*    CORE_EVENT_PROXY_SAI | DEVICE_POSTBOOT_SAI | RCIOMMU_BYPASS_SAI |         */
/*    PCH_IMR_SAI | EC_ESPI_SAI | CNVi_SAI | UNASSIGNED_74_SAI |                */
/*    UNASSIGNED_76_SAI | UNASSIGNED_78_SAI | CRASHLOG_SAI |                    */
/*    UNASSIGNED_7C_SAI | DEVICE_UNTRUSTED_SAI                                  */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI |    */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI                    */
/* PUNIT_OS_READONLY_SAI_POLICY                                                 */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | RESERVED_1_SAI | RESERVED_2_SAI |  */
/*    HOSTIA_SEAM_SAI | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI |        */
/*    VTD_SAI | PM_DIE_TO_DIE_SAI | UNASSIGNED_1C_SAI | HOSTCP_PMA_SAI |        */
/*    CSE_INTEL_SAI | DMU_UNTRUSTED_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |     */
/*    GSC_SAI | PM_IOSS_SAI | UNASSIGNED_2C_SAI | UNASSIGNED_2E_SAI |           */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | ISH_SAI |               */
/*    CLINK_KVM_SAI | NPK_SAI | TBT_SAI | IOM_SAI | HW_PCH_SAI | SPI_SAI |      */
/*    UNASSIGNED_42_SAI | ESE_UNTRUSTED_SAI | GT_PMA_SAI | ESE_SAI |            */
/*    CSE_UNTRUSTED_SAI | UNASSIGNED_4C_SAI | UNASSIGNED_4E_SAI | IPU_SAI |     */
/*    RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI | DFX_THIRDPARTY_SAI |          */
/*    DISPLAY_SAI | LT_SAI | UNASSIGNED_5C_SAI | DISPLAY_KVM_SAI |              */
/*    UNASSIGNED_60_SAI | ACE_SAI | UNASSIGNED_64_SAI | CSME_SMS_SAI |          */
/*    CORE_EVENT_PROXY_SAI | DEVICE_POSTBOOT_SAI | RCIOMMU_BYPASS_SAI |         */
/*    PCH_IMR_SAI | EC_ESPI_SAI | CNVi_SAI | UNASSIGNED_74_SAI |                */
/*    UNASSIGNED_76_SAI | UNASSIGNED_78_SAI | CRASHLOG_SAI |                    */
/*    UNASSIGNED_7C_SAI | DEVICE_UNTRUSTED_SAI                                  */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_UCODE_SAI | HOSTIA_SUNPASS_SAI | PM_PCS_SAI | HW_CPU_SAI |         */
/*    HOSTCP_PMA_SAI | DFX_INTEL_MANUFACTURING_SAI | GT_PMA_SAI |               */
/*    DFX_INTEL_PRODUCTION_SAI                                                  */
/* PUNIT_OS_WRITE_SAI_POLICY                                                    */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | RESERVED_1_SAI | RESERVED_2_SAI |  */
/*    HOSTIA_SEAM_SAI | GT_SAI | PM_PCS_SAI | HW_CPU_SAI | MEM_CPL_SAI |        */
/*    VTD_SAI | PM_DIE_TO_DIE_SAI | UNASSIGNED_1C_SAI | HOSTCP_PMA_SAI |        */
/*    CSE_INTEL_SAI | DMU_UNTRUSTED_SAI | FUSE_CTRL_SAI | FUSE_PULLER_SAI |     */
/*    GSC_SAI | PM_IOSS_SAI | UNASSIGNED_2C_SAI | UNASSIGNED_2E_SAI |           */
/*    DFX_INTEL_MANUFACTURING_SAI | DFX_UNTRUSTED_SAI | ISH_SAI |               */
/*    CLINK_KVM_SAI | NPK_SAI | TBT_SAI | IOM_SAI | HW_PCH_SAI | SPI_SAI |      */
/*    UNASSIGNED_42_SAI | ESE_UNTRUSTED_SAI | GT_PMA_SAI | ESE_SAI |            */
/*    CSE_UNTRUSTED_SAI | UNASSIGNED_4C_SAI | UNASSIGNED_4E_SAI | IPU_SAI |     */
/*    RC_MORPHED_SAI | DFX_INTEL_PRODUCTION_SAI | DFX_THIRDPARTY_SAI |          */
/*    DISPLAY_SAI | LT_SAI | UNASSIGNED_5C_SAI | DISPLAY_KVM_SAI |              */
/*    UNASSIGNED_60_SAI | ACE_SAI | UNASSIGNED_64_SAI | CSME_SMS_SAI |          */
/*    CORE_EVENT_PROXY_SAI | DEVICE_POSTBOOT_SAI | RCIOMMU_BYPASS_SAI |         */
/*    PCH_IMR_SAI | EC_ESPI_SAI | CNVi_SAI | UNASSIGNED_74_SAI |                */
/*    UNASSIGNED_76_SAI | UNASSIGNED_78_SAI | CRASHLOG_SAI |                    */
/*    UNASSIGNED_7C_SAI | DEVICE_UNTRUSTED_SAI                                  */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | GT_SAI | PM_PCS_SAI |              */
/*    HW_CPU_SAI | HOSTCP_PMA_SAI | CSE_INTEL_SAI | PM_IOSS_SAI |               */
/*    DFX_INTEL_MANUFACTURING_SAI | HW_PCH_SAI | GT_PMA_SAI | IPU_SAI |         */
/*    DFX_INTEL_PRODUCTION_SAI | DISPLAY_SAI | DEVICE_POSTBOOT_SAI              */

#ifndef _PUNIT_MCHBAR_h
#define _PUNIT_MCHBAR_h

#include <Base.h>


// IP GUID: 4624b1bd-38ff-443e-8ee6-0d284ccfdfe9
#define PUNIT_MCHBAR_ID 0x4624b1bd38ff443e

//************************************************ RegisterStructs


/** BIOS_POST_CODE_PCU_PUNIT_MCHBAR desc:
  * Register default value:        0x00000000
  * Register full path in IP: punit_top/punit_gpsb/gpsb_infvnn_crs/MEM_MCHBAR_MSG_MCHBAR/BIOS_POST_CODE_0_0_0_MCHBAR_PCU
  * Std path: VER=1, UVM=punit_top.punit_gpsb.gpsb_infvnn_crs.BIOS_POST_CODE_0_0_0_MCHBAR_PCU, OSX=punit_top.punit_gpsb_551_18.gpsb_infvnn_crs_549_44.MEM_MCHBAR_MSG_MCHBAR_105_23.BIOS_POST_CODE_0_0_0_MCHBAR_PCU
  * Security PolicyGroup: PUNIT_OS_WRITE_SAI_POLICY
  * This register holds 32 writable bits with no functionality behind them. BIOS will write here the current POST code (port 80).
  */

#define BIOS_POST_CODE_PCU_PUNIT_MCHBAR_REG (0x00000024U)

typedef union {
  struct {
    UINT32 postcode : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS will write the current POST code in this
                               field
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} BIOS_POST_CODE_PCU_PUNIT_MCHBAR_STRUCT;

#define BIOS_POST_CODE_PCU_PUNIT_MCHBAR_WIDTH 32

#define BIOS_POST_CODE_PCU_PUNIT_MCHBAR_POSTCODE_LSB 0x0000
#define BIOS_POST_CODE_PCU_PUNIT_MCHBAR_POSTCODE_SIZE 0x0020

#ifdef SV_HOOKS

/** DDR_PTM_CTL_PCU_PUNIT_MCHBAR desc:
  * Register default value:        0x00000000
  * Register full path in IP: punit_top/punit_gpsb/gpsb_infvnn_crs/MEM_MCHBAR_MSG_MCHBAR/DDR_PTM_CTL_0_0_0_MCHBAR_PCU
  * Std path: VER=1, UVM=punit_top.punit_gpsb.gpsb_infvnn_crs.DDR_PTM_CTL_0_0_0_MCHBAR_PCU, OSX=punit_top.punit_gpsb_551_18.gpsb_infvnn_crs_549_44.MEM_MCHBAR_MSG_MCHBAR_105_23.DDR_PTM_CTL_0_0_0_MCHBAR_PCU
  * Security PolicyGroup: PUNIT_BIOS_MEM_SAI_POLICY
  * Mode control bits for DDR power and thermal management features.     [IntelRsvd]This register is written by BIOS and read by reset pcode (phase 4). Because pcode never samples this reg again after boot, no locking mechanism is necessary (SW reads will see previous SW writes, however, and this should be publically documented). This register is NOT accessible via PECI/PCS.[/IntelRsvd]
  */

#define DDR_PTM_CTL_PCU_PUNIT_MCHBAR_REG (0x00000080U)

typedef union {
  struct {
    UINT32 reserved_0 : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Reserved Bit. */

    UINT32 reserved_1 : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* Reserved Bit. */

    UINT32 rsvd_0 : 4; /**< Undefined - auto filled rsvd_[05:02] */

                            /* Bits[5:2], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 pdwn_config_ctl : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /*
                               This bit determined whether BIOS or pcode
                               will control DDR powerdown modes and idle
                               counter (via programming the PM_PDWN_config
                               regs in iMC). When clear, pcode will manage
                               the modes based on either core P-states or
                               IA32_ENERGY_PERFORMANCE_BIAS MSR value (when
                               enabled). When set, BIOS is in control of
                               DDR CKE mode and idle timer value, and pcode
                               algorithm does not run.
                            */

    UINT32 rsvd_1 : 25; /**< Undefined - auto filled rsvd_[31:07] */

                            /* Bits[31:7], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDR_PTM_CTL_PCU_PUNIT_MCHBAR_STRUCT;

#define DDR_PTM_CTL_PCU_PUNIT_MCHBAR_WIDTH 32

#define DDR_PTM_CTL_PCU_PUNIT_MCHBAR_RESERVED_0_LSB 0x0000
#define DDR_PTM_CTL_PCU_PUNIT_MCHBAR_RESERVED_0_SIZE 0x0001
#define DDR_PTM_CTL_PCU_PUNIT_MCHBAR_RESERVED_1_LSB 0x0001
#define DDR_PTM_CTL_PCU_PUNIT_MCHBAR_RESERVED_1_SIZE 0x0001
#define DDR_PTM_CTL_PCU_PUNIT_MCHBAR_PDWN_CONFIG_CTL_LSB 0x0006
#define DDR_PTM_CTL_PCU_PUNIT_MCHBAR_PDWN_CONFIG_CTL_SIZE 0x0001

#endif      // SV_HOOKS


/** RP_STATE_LIMITS_PCU_PUNIT_MCHBAR desc:
  * Register default value:        0x000000FF
  * Register full path in IP: punit_top/punit_gpsb/gpsb_infvnn_crs/MEM_MCHBAR_MSG_MCHBAR/RP_STATE_LIMITS_0_0_0_MCHBAR_PCU
  * Std path: VER=1, UVM=punit_top.punit_gpsb.gpsb_infvnn_crs.RP_STATE_LIMITS_0_0_0_MCHBAR_PCU, OSX=punit_top.punit_gpsb_551_18.gpsb_infvnn_crs_549_44.MEM_MCHBAR_MSG_MCHBAR_105_23.RP_STATE_LIMITS_0_0_0_MCHBAR_PCU
  * Security PolicyGroup: PUNIT_OS_WRITE_SAI_POLICY
  * This register allows SW to limit the maximum base frequency for the Integrated GFX Engine (GT) allowed during run-time.
  */

#define RP_STATE_LIMITS_PCU_PUNIT_MCHBAR_REG (0x00000194U)

typedef union {
  struct {
    UINT32 rpstt_lim : 8;

                            /* Bits[7:0], Access Type=RW, default=0x000000FF*/

                            /*
                               This field indicates the maximum base frequency
                               limit for the Integrated GFX Engine (GT) allowed
                               during run-time.       [IntelRsvd]PCODE Notes:
                               The value will be clipped to a maximum of
                               RP0 and a minimum of RPn.  It will be taken
                               into account only if the next slow loop after
                               a delay.[/IntelRsvd]
                            */

    UINT32 rsvd_0 : 24; /**< Undefined - auto filled rsvd_[31:08] */

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RP_STATE_LIMITS_PCU_PUNIT_MCHBAR_STRUCT;

#define RP_STATE_LIMITS_PCU_PUNIT_MCHBAR_WIDTH 32

#define RP_STATE_LIMITS_PCU_PUNIT_MCHBAR_RPSTT_LIM_LSB 0x0000
#define RP_STATE_LIMITS_PCU_PUNIT_MCHBAR_RPSTT_LIM_SIZE 0x0008


/** GT_PSTATE_CAP_PCU_PUNIT_MCHBAR desc:
  * Register default value:        0x00000000
  * Register full path in IP: punit_top/punit_gpsb/gpsb_infvnn_crs/MEM_MCHBAR_MSG_MCHBAR/GT_PSTATE_CAP_0_0_0_MCHBAR_PCU
  * Std path: VER=1, UVM=punit_top.punit_gpsb.gpsb_infvnn_crs.GT_PSTATE_CAP_0_0_0_MCHBAR_PCU, OSX=punit_top.punit_gpsb_551_18.gpsb_infvnn_crs_549_44.MEM_MCHBAR_MSG_MCHBAR_105_23.GT_PSTATE_CAP_0_0_0_MCHBAR_PCU
  * Security PolicyGroup: PUNIT_OS_READONLY_SAI_POLICY
  * Contains the static GT frequency limits for RP0/RPn P-States. Consumed by the GFX Driver.
  */

#define GT_PSTATE_CAP_PCU_PUNIT_MCHBAR_REG (0x00000198U)

typedef union {
  struct {
    UINT32 rp0_cap : 9; /**< RP0 field */

                            /* Bits[8:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               Indicates the maximum RP0 base frequency capability
                               for GT. Values are in units of 16.67MHz bins.
                            */

    UINT32 reserved : 7; /**< Reserved field */

                            /* Bits[15:9], Access Type=RW/P, default=0x00000000*/

                            /* RESERVED */

    UINT32 rpn_cap : 9; /**< RPN field */

                            /* Bits[24:16], Access Type=RW/P, default=0x00000000*/

                            /*
                               Indicates the maximum RPN base frequency capability
                               for GT. Values are in units of 16.67MHz bins.
                            */

    UINT32 rsvd_0 : 7; /**< Undefined - auto filled rsvd_[31:25] */

                            /* Bits[31:25], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GT_PSTATE_CAP_PCU_PUNIT_MCHBAR_STRUCT;

#define GT_PSTATE_CAP_PCU_PUNIT_MCHBAR_WIDTH 32

#define GT_PSTATE_CAP_PCU_PUNIT_MCHBAR_RP0_CAP_LSB 0x0000
#define GT_PSTATE_CAP_PCU_PUNIT_MCHBAR_RP0_CAP_SIZE 0x0009
#define GT_PSTATE_CAP_PCU_PUNIT_MCHBAR_RESERVED_LSB 0x0009
#define GT_PSTATE_CAP_PCU_PUNIT_MCHBAR_RESERVED_SIZE 0x0007
#define GT_PSTATE_CAP_PCU_PUNIT_MCHBAR_RPN_CAP_LSB 0x0010
#define GT_PSTATE_CAP_PCU_PUNIT_MCHBAR_RPN_CAP_SIZE 0x0009


/** PACKAGE_RAPL_LIMIT_PCU_PUNIT_MCHBAR desc:
  * Register default value:        0x00000000
  * Register full path in IP: punit_top/punit_gpsb/gpsb_infvnn_crs/MEM_MCHBAR_MSG_MCHBAR/PACKAGE_RAPL_LIMIT_0_0_0_MCHBAR_PCU
  * Std path: VER=1, UVM=punit_top.punit_gpsb.gpsb_infvnn_crs.PACKAGE_RAPL_LIMIT_0_0_0_MCHBAR_PCU, OSX=punit_top.punit_gpsb_551_18.gpsb_infvnn_crs_549_44.MEM_MCHBAR_MSG_MCHBAR_105_23.PACKAGE_RAPL_LIMIT_0_0_0_MCHBAR_PCU
  * Security PolicyGroup: PUNIT_OS_WRITE_SAI_POLICY
  * The Integrated Graphics driver, CPM driver, BIOS and OS can balance the power budget between the Primary Power Plane (IA) and the Secondary Power Plane (GT) via PRIMARY_PLANE_TURBO_POWER_LIMIT_MSR and SECONDARY_PLANE_TURBO_POWER_LIMIT_MSR.
  */

#define PACKAGE_RAPL_LIMIT_PCU_PUNIT_MCHBAR_REG (0x000001a0U)

typedef union {
  struct {
    UINT64 pkg_pwr_lim_1 : 15;

                            /* Bits[14:0], Access Type=RW/L, default=0x00000000*/

                            /*
                               This field indicates the power limitation
                               #1. The unit of measurement is defined in
                               PACKAGE_POWER_SKU_UNIT_MSR[PWR_UNIT].
                            */

    UINT64 pkg_pwr_lim_1_en : 1;

                            /* Bits[15:15], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit enables/disables PKG_PWR_LIM_1. 0b
                               Package Power Limit 1 is Disabled 1b  Package
                               Power Limit 1 is Enabled
                            */

    UINT64 pkg_clmp_lim_1 : 1;

                            /* Bits[16:16], Access Type=RW/L, default=0x00000000*/

                            /*
                               Package Clamping limitation #1 - Allow going
                               below P1. 0b     PBM is limited between P1
                               and P0. 1b     PBM can go below P1.
                            */

    UINT64 pkg_pwr_lim_1_time : 7;

                            /* Bits[23:17], Access Type=RW/L, default=0x00000000*/

                            /*
                               x = PKG_PWR_LIM_1_TIME[23:22] y = PKG_PWR_LIM_1_TIME[21:17]
                               The timing interval window is Floating Point
                               number given by 1.x * power(2,y). The unit
                               of measurement is defined in PACKAGE_POWER_SKU_UNIT_MSR[TIME_UNIT].
                               The maximal time window is bounded by PACKAGE_POWER_SKU_MSR[PKG_MAX_WIN].
                               The minimum time window is 1 unit of measurement
                               (as defined above).
                            */

    UINT64 rsvd_0 : 8; /**< Undefined - auto filled rsvd_[31:24] */

                            /* Bits[31:24], Access Type=RO, default=None*/

                            /* Reserved */

    UINT64 pkg_pwr_lim_2 : 15;

                            /* Bits[46:32], Access Type=RW/L, default=0x00000000*/

                            /*
                               This field indicates the power limitation
                               #2. The unit of measurement is defined in
                               PACKAGE_POWER_SKU_UNIT_MSR[PWR_UNIT].
                            */

    UINT64 pkg_pwr_lim_2_en : 1;

                            /* Bits[47:47], Access Type=RW/L, default=0x00000000*/

                            /*
                               This bit enables/disables PKG_PWR_LIM_2. 0b
                               Package Power Limit 2 is Disabled 1b  Package
                               Power Limit 2 is Enabled
                            */

    UINT64 pkg_clmp_lim_2 : 1;

                            /* Bits[48:48], Access Type=RW/L, default=0x00000000*/

                            /*
                               Package Clamping limitation #2 - Allow going
                               below P1. 0b     PBM is limited between P1
                               and P0. 1b     PBM can go below P1.
                            */

    UINT64 pkg_pwr_lim_2_time : 7;

                            /* Bits[55:49], Access Type=RW/L, default=0x00000000*/

                            /*
                               x = PKG_PWR_LIM_2_TIME[55:54] y = PKG_PWR_LIM_2_TIME[53:49]
                               The timing interval window is Floating Point
                               number given by 1.x * power(2,y). The unit
                               of measurement is defined in PACKAGE_POWER_SKU_UNIT_MSR[TIME_UNIT].
                               The maximal time window is bounded by PACKAGE_POWER_SKU_MSR[PKG_MAX_WIN].
                               The minimum time window is 1 unit of measurement
                               (as defined above).
                            */

    UINT64 rsvd_1 : 7; /**< Undefined - auto filled rsvd_[62:56] */

                            /* Bits[62:56], Access Type=RO, default=None*/

                            /* Reserved */

    UINT64 pkg_pwr_lim_lock : 1;

                            /* Bits[63:63], Access Type=RW/L, default=0x00000000*/

                            /*
                               When set, all settings in this register are
                               locked and are treated as Read Only. This
                               bit will typically set by BIOS during boot
                               time or resume from Sx.
                            */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} PACKAGE_RAPL_LIMIT_PCU_PUNIT_MCHBAR_STRUCT;

#define PACKAGE_RAPL_LIMIT_PCU_PUNIT_MCHBAR_WIDTH 64

#define PACKAGE_RAPL_LIMIT_PCU_PUNIT_MCHBAR_PKG_PWR_LIM_1_LSB 0x0000
#define PACKAGE_RAPL_LIMIT_PCU_PUNIT_MCHBAR_PKG_PWR_LIM_1_SIZE 0x000f
#define PACKAGE_RAPL_LIMIT_PCU_PUNIT_MCHBAR_PKG_PWR_LIM_1_EN_LSB 0x000f
#define PACKAGE_RAPL_LIMIT_PCU_PUNIT_MCHBAR_PKG_PWR_LIM_1_EN_SIZE 0x0001
#define PACKAGE_RAPL_LIMIT_PCU_PUNIT_MCHBAR_PKG_CLMP_LIM_1_LSB 0x0010
#define PACKAGE_RAPL_LIMIT_PCU_PUNIT_MCHBAR_PKG_CLMP_LIM_1_SIZE 0x0001
#define PACKAGE_RAPL_LIMIT_PCU_PUNIT_MCHBAR_PKG_PWR_LIM_1_TIME_LSB 0x0011
#define PACKAGE_RAPL_LIMIT_PCU_PUNIT_MCHBAR_PKG_PWR_LIM_1_TIME_SIZE 0x0007
#define PACKAGE_RAPL_LIMIT_PCU_PUNIT_MCHBAR_PKG_PWR_LIM_2_LSB 0x0020
#define PACKAGE_RAPL_LIMIT_PCU_PUNIT_MCHBAR_PKG_PWR_LIM_2_SIZE 0x000f
#define PACKAGE_RAPL_LIMIT_PCU_PUNIT_MCHBAR_PKG_PWR_LIM_2_EN_LSB 0x002f
#define PACKAGE_RAPL_LIMIT_PCU_PUNIT_MCHBAR_PKG_PWR_LIM_2_EN_SIZE 0x0001
#define PACKAGE_RAPL_LIMIT_PCU_PUNIT_MCHBAR_PKG_CLMP_LIM_2_LSB 0x0030
#define PACKAGE_RAPL_LIMIT_PCU_PUNIT_MCHBAR_PKG_CLMP_LIM_2_SIZE 0x0001
#define PACKAGE_RAPL_LIMIT_PCU_PUNIT_MCHBAR_PKG_PWR_LIM_2_TIME_LSB 0x0031
#define PACKAGE_RAPL_LIMIT_PCU_PUNIT_MCHBAR_PKG_PWR_LIM_2_TIME_SIZE 0x0007
#define PACKAGE_RAPL_LIMIT_PCU_PUNIT_MCHBAR_PKG_PWR_LIM_LOCK_LSB 0x003f
#define PACKAGE_RAPL_LIMIT_PCU_PUNIT_MCHBAR_PKG_PWR_LIM_LOCK_SIZE 0x0001

#ifdef SV_HOOKS

/** CHAP_CONFIG_PCU_PUNIT_MCHBAR desc:
  * Register default value:        0x00000000
  * Register full path in IP: punit_top/punit_gpsb/gpsb_infvnn_crs/MEM_MCHBAR_MSG_MCHBAR/CHAP_CONFIG_0_0_0_MCHBAR_PCU
  * Std path: VER=1, UVM=punit_top.punit_gpsb.gpsb_infvnn_crs.CHAP_CONFIG_0_0_0_MCHBAR_PCU, OSX=punit_top.punit_gpsb_551_18.gpsb_infvnn_crs_549_44.MEM_MCHBAR_MSG_MCHBAR_105_23.CHAP_CONFIG_0_0_0_MCHBAR_PCU
  * Security PolicyGroup: PUNIT_OS_WRITE_SAI_POLICY
  * CHAP Configuration register to configure PCU CHAP events.
  */

#define CHAP_CONFIG_PCU_PUNIT_MCHBAR_REG (0x00000200U)

typedef union {
  struct {
    UINT32 rsvd_0 : 4; /**< Undefined - auto filled rsvd_[03:00] */

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 peci_cmd : 8;

                            /* Bits[11:4], Access Type=RW, default=0x00000000*/

                            /*
                               This field holds the PECI Command to be tracked
                               for CHAP. The PCU will compare this value
                               to the actual PECI command.  If there is match
                               then PCU HW will send the following indications
                               to the CHAP unit: - PECI_CMD_SERVICE_LATENCY
                               is a level signal that indicates the service
                               latency for the PECI command. - PECI_CMD_RECEIVED_IND
                               is a pulse that indicates that there was a
                               match for the command value specified in this
                               field.
                            */

    UINT32 rsvd_1 : 20; /**< Undefined - auto filled rsvd_[31:12] */

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CHAP_CONFIG_PCU_PUNIT_MCHBAR_STRUCT;

#define CHAP_CONFIG_PCU_PUNIT_MCHBAR_WIDTH 32

#define CHAP_CONFIG_PCU_PUNIT_MCHBAR_PECI_CMD_LSB 0x0004
#define CHAP_CONFIG_PCU_PUNIT_MCHBAR_PECI_CMD_SIZE 0x0008

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** MCHECK_FLOW_STATUS_0_0_0_MCHBAR_PUNIT_MCHBAR desc:
  * Register default value:        0x00000000
  * Register full path in IP: punit_top/punit_gpsb/gpsb_infvnn_crs/MEM_MCHBAR_MSG_MCHBAR/MCHECK_FLOW_STATUS_0_0_0_MCHBAR
  * Std path: VER=1, UVM=punit_top.punit_gpsb.gpsb_infvnn_crs.MCHECK_FLOW_STATUS_0_0_0_MCHBAR, OSX=punit_top.punit_gpsb_551_18.gpsb_infvnn_crs_549_44.MEM_MCHBAR_MSG_MCHBAR_105_23.MCHECK_FLOW_STATUS_0_0_0_MCHBAR
  * Security PolicyGroup: PUNIT_OS_WRITE_SAI_POLICY
  * This register holds MCHECK flow status.
  */

#define MCHECK_FLOW_STATUS_0_0_0_MCHBAR_PUNIT_MCHBAR_REG (0x00000254U)

typedef union {
  struct {
    UINT32 mcheck_skipped_or_done : 1;

                            /* Bits[0:0], Access Type=RW/1S, default=0x00000000*/

                            /*
                               This bit is part of the interface between
                               BIOS, MCHECK and PCODE in the scheme to prevent
                               subset of power transitions while MCHECK is
                               using the MC Mailbox.
                               BIOS will set this bit  once MCHECK is skipped
                               or done, regardless of MCHECK return value.
                               After set - bit kept asserted till next Reset
                               entry.
                            */

    UINT32 reserved_31_1 : 31;

                            /* Bits[31:1], Access Type=RW, default=0x00000000*/

                            /* Reserved bits. */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MCHECK_FLOW_STATUS_0_0_0_MCHBAR_PUNIT_MCHBAR_STRUCT;

#define MCHECK_FLOW_STATUS_0_0_0_MCHBAR_PUNIT_MCHBAR_WIDTH 32

#define MCHECK_FLOW_STATUS_0_0_0_MCHBAR_PUNIT_MCHBAR_MCHECK_SKIPPED_OR_DONE_LSB 0x0000
#define MCHECK_FLOW_STATUS_0_0_0_MCHBAR_PUNIT_MCHBAR_MCHECK_SKIPPED_OR_DONE_SIZE 0x0001
#define MCHECK_FLOW_STATUS_0_0_0_MCHBAR_PUNIT_MCHBAR_RESERVED_31_1_LSB 0x0001
#define MCHECK_FLOW_STATUS_0_0_0_MCHBAR_PUNIT_MCHBAR_RESERVED_31_1_SIZE 0x001f

#endif      // SV_HOOKS


/** BIOS_RESET_CPL_PCU_PUNIT_MCHBAR desc:
  * Register default value:        0x00000000
  * Register full path in IP: punit_top/punit_gpsb/gpsb_infvnn_crs/MEM_MCHBAR_MSG_MCHBAR/BIOS_RESET_CPL_0_0_0_MCHBAR_PCU
  * Std path: VER=1, UVM=punit_top.punit_gpsb.gpsb_infvnn_crs.BIOS_RESET_CPL_0_0_0_MCHBAR_PCU, OSX=punit_top.punit_gpsb_551_18.gpsb_infvnn_crs_549_44.MEM_MCHBAR_MSG_MCHBAR_105_23.BIOS_RESET_CPL_0_0_0_MCHBAR_PCU
  * Security PolicyGroup: PUNIT_OS_WRITE_SAI_POLICY
  * This register is used as interface between BIOS and PCODE.  It is written by BIOS and read by PCODE.
  */

#define BIOS_RESET_CPL_PCU_PUNIT_MCHBAR_REG (0x00005da8U)

typedef union {
  struct {
    UINT32 rst_cpl : 1;

                            /* Bits[0:0], Access Type=RW/1S, default=0x00000000*/

                            /*
                               This bit is set by BIOS to indicate to the
                               CPU Power management function that it has
                               completed to set up all PM relevant configuration
                               and allow CPU Power management function  to
                               digest the configuration data and start active
                               PM operation. It is expected that this bit
                               will be set just before BIOS transfer of control
                               to the OS. 0b     Not ready 1b     BIOS PM
                               configuration complete
                            */

    UINT32 pcie_enumeration_done : 1;

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               This will be set after PCIe enumeration is
                               done.  This bit will be read by pcode.  If
                               it is set, pcode will look at the following
                               register bits:         MPVTDTRK_CR_DEVEN_0_0_0_PCI
                               Bit     Bit Name         1 D1F2EN
                               2 D1F1EN         3 D1F0EN         If all of
                               these bits are set to a 0x0, this means that
                               there is nothing connected to the PEG devices
                               and the Gen3 PLL can be shut off.
                               Note - implicit assumption - this bit is asserted
                               prior to (or with) asserting RST_CPL.
                            */

    UINT32 c7_allowed : 1;

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS/driver will set this bit when only discrete
                               graphics is being used and the PCIe lanes
                               will be down.  BIOS/driver will clear this
                               bit when discrete graphics is being used.
                               THIS FIELD IS OBSOLETE. NOT USED ANYWHERE.
                               (Nov-2013)
                            */

    UINT32 rsvd_0 : 29; /**< Undefined - auto filled rsvd_[31:03] */

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} BIOS_RESET_CPL_PCU_PUNIT_MCHBAR_STRUCT;

#define BIOS_RESET_CPL_PCU_PUNIT_MCHBAR_WIDTH 32

#define BIOS_RESET_CPL_PCU_PUNIT_MCHBAR_RST_CPL_LSB 0x0000
#define BIOS_RESET_CPL_PCU_PUNIT_MCHBAR_RST_CPL_SIZE 0x0001
#define BIOS_RESET_CPL_PCU_PUNIT_MCHBAR_PCIE_ENUMERATION_DONE_LSB 0x0001
#define BIOS_RESET_CPL_PCU_PUNIT_MCHBAR_PCIE_ENUMERATION_DONE_SIZE 0x0001
#define BIOS_RESET_CPL_PCU_PUNIT_MCHBAR_C7_ALLOWED_LSB 0x0002
#define BIOS_RESET_CPL_PCU_PUNIT_MCHBAR_C7_ALLOWED_SIZE 0x0001

/** STREAM_TRACER_MEM_BUFFER_BASE_PMBAR desc:
  * Register default value:        0x00000000
  * Register full path in IP: punit_top/punit_gpsb/punit_dev10_mmio_crs/MEM_PMBAR_MMIO_CRS_MSG_PMBAR_MMIO_CRS/STREAM_TRACER_MEM_BUFFER_BASE
  * Std path: VER=1, UVM=punit_top.punit_gpsb.punit_dev10_mmio_crs.STREAM_TRACER_MEM_BUFFER_BASE, OSX=punit_top.punit_gpsb_551_18.punit_dev10_mmio_crs_549_50.MEM_PMBAR_MMIO_CRS_MSG_PMBAR_MMIO_CRS_108_8.STREAM_TRACER_MEM_BUFFER_BASE
  * Security PolicyGroup: PUNIT_BIOS_SAI_POLICY
  * Base address of the streaming tracer allocated buffer in external memory
  */

#define STREAM_TRACER_MEM_BUFFER_BASE_PMBAR_REG (0x0000ffa8U)

typedef union {
  struct {
    UINT64 base_addr : 64;

                            /* Bits[63:0], Access Type=RW, default=0x00000000*/

                            /*
                               Base address of the external allocated memory
                               buffer
                            */

  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} STREAM_TRACER_MEM_BUFFER_BASE_PMBAR_STRUCT;

#define STREAM_TRACER_MEM_BUFFER_BASE_PMBAR_WIDTH 64

#define STREAM_TRACER_MEM_BUFFER_BASE_PMBAR_BASE_ADDR_LSB 0x0000
#define STREAM_TRACER_MEM_BUFFER_BASE_PMBAR_BASE_ADDR_SIZE 0x0040

/** STREAM_TRACER_MEM_BUFFER_SIZE_PMBAR desc:
  * Register default value:        0x00000000
  * Register full path in IP: punit_top/punit_gpsb/punit_dev10_mmio_crs/MEM_PMBAR_MMIO_CRS_MSG_PMBAR_MMIO_CRS/STREAM_TRACER_MEM_BUFFER_SIZE
  * Std path: VER=1, UVM=punit_top.punit_gpsb.punit_dev10_mmio_crs.STREAM_TRACER_MEM_BUFFER_SIZE, OSX=punit_top.punit_gpsb_551_18.punit_dev10_mmio_crs_549_50.MEM_PMBAR_MMIO_CRS_MSG_PMBAR_MMIO_CRS_108_8.STREAM_TRACER_MEM_BUFFER_SIZE
  * Security PolicyGroup: PUNIT_BIOS_SAI_POLICY
  * Size (in DW) of the streaming tracer allocated buffer in external memory. Size = 0 means no buffer was allocated and therefore streaming tracer should not be enabled.
  */

#define STREAM_TRACER_MEM_BUFFER_SIZE_PMBAR_REG (0x0000ffb0U)

typedef union {
  struct {
    UINT32 mem_buffer_size : 28;

                            /* Bits[27:0], Access Type=RW, default=0x00000000*/

                            /*
                               The size of the allocated memory buffer for
                               streaming tracer in DWORDs (1 DWORD == 4 Bytes).
                               E.g. dword size of 0x40000 means 1MB allocated
                               for the memory buffer
                            */

    UINT32 reserved_0 : 4;

                            /* Bits[31:28], Access Type=RW, default=0x00000000*/

                            /* Reserved */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} STREAM_TRACER_MEM_BUFFER_SIZE_PMBAR_STRUCT;

#define STREAM_TRACER_MEM_BUFFER_SIZE_PMBAR_WIDTH 32

#define STREAM_TRACER_MEM_BUFFER_SIZE_PMBAR_MEM_BUFFER_SIZE_LSB 0x0000
#define STREAM_TRACER_MEM_BUFFER_SIZE_PMBAR_MEM_BUFFER_SIZE_SIZE 0x001c
#define STREAM_TRACER_MEM_BUFFER_SIZE_PMBAR_RESERVED_LSB 0x001c
#define STREAM_TRACER_MEM_BUFFER_SIZE_PMBAR_RESERVED_SIZE 0x0004

//************************************************ /RegisterStructs


#endif      // _PUNIT_MCHBAR_h

