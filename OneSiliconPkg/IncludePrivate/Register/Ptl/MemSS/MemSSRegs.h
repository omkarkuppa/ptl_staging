/** @file
  MEMSS.h

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
/* MEMSS_PMA_SAI_POLICY                                                         */
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

#ifndef _MEMSS_PTL_CDIE_H_
#define _MEMSS_PTL_CDIE_H_

#include <Base.h>


// IP GUID: 2fea646c-495f-4712-b1d7-3c72dae30928
#define MEMSS_ID 0x2fea646c495f4712

//************************************************ RegisterStructs


/** MEMSS_PMA_CR_BIOS_MEM_CONFIG_MEMSS desc:
  * Register default value:        0x00000000
  * Register full path in IP: memss_pma_cr_sys_top/memss_pma_regs/ami_memss_pma_addrmap/MEMSS_PMA_CR_BIOS_MEM_CONFIG
  * Std path: VER=1, UVM=memss_pma_cr_sys_top.memss_pma_regs.MEMSS_PMA_CR_BIOS_MEM_CONFIG, OSX=memss_pma_cr_sys_top.memss_pma_regs.ami_memss_pma_addrmap.MEMSS_PMA_CR_BIOS_MEM_CONFIG
  * Security PolicyGroup: MEMSS_PMA_SAI_POLICY
  * This register is used as interface between BIOS and MemSS PMA. Bios writes memory configuration and individual IP enable/disable status into the register. It is written by BIOS and read by MemSS PMA. The register must be locked for write after MRC sets MEMSS_PMA_CR_BIOS_REQ.RUN_BUSY.
  */

#define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MEMSS_REG (0x00013d00U)

typedef union {
  struct {
    UINT32 mc0_en : 1; /**< MC0 enable */

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* BIOS Enable for MC0 */

    UINT32 mc1_en : 1; /**< MC1 enable */

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /* BIOS Enable for MC1 */

    UINT32 ibecc0_en : 1; /**< IBECC0 enable */

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /* BIOS Enable for IBECC0 */

    UINT32 ibecc1_en : 1; /**< IBECC1 enable */

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /* BIOS Enable for IBECC1 */

    UINT32 cce0_en : 1; /**< CCE0 enable */

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* BIOS Enable for CCE0 */

    UINT32 cce1_en : 1; /**< CCE1 enable */

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* BIOS Enable for CCE1 */

    UINT32 ddr_type : 3; /**< DDR Type */

                            /* Bits[8:6], Access Type=RW, default=0x00000000*/

                            /*
                               DDR Type  000 - Reserved, 001 - DDR5, 010
                               - LPDDR5, 011 - LPDDR4, 1xx - reserved
                            */

    UINT32 tme_en : 1; /**< TME enable */

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* TME en bit to enable traffic via CCE */

    UINT32 reserved : 22; /**< Reserved Bits */

                            /* Bits[31:10], Access Type=RW, default=0x00000000*/

                            /* Reserved bits. */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_BIOS_MEM_CONFIG_MEMSS_STRUCT;

#define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MEMSS_WIDTH 32

#define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MEMSS_MC0_EN_LSB 0x0000
#define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MEMSS_MC0_EN_SIZE 0x0001
#define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MEMSS_MC1_EN_LSB 0x0001
#define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MEMSS_MC1_EN_SIZE 0x0001
#define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MEMSS_IBECC0_EN_LSB 0x0002
#define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MEMSS_IBECC0_EN_SIZE 0x0001
#define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MEMSS_IBECC1_EN_LSB 0x0003
#define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MEMSS_IBECC1_EN_SIZE 0x0001
#define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MEMSS_CCE0_EN_LSB 0x0004
#define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MEMSS_CCE0_EN_SIZE 0x0001
#define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MEMSS_CCE1_EN_LSB 0x0005
#define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MEMSS_CCE1_EN_SIZE 0x0001
#define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MEMSS_DDR_TYPE_LSB 0x0006
#define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MEMSS_DDR_TYPE_SIZE 0x0003
#define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MEMSS_TME_EN_LSB 0x0009
#define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MEMSS_TME_EN_SIZE 0x0001
#define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MEMSS_RESERVED_LSB 0x000a
#define MEMSS_PMA_CR_BIOS_MEM_CONFIG_MEMSS_RESERVED_SIZE 0x0016

#ifdef SV_HOOKS

/** MEMSS_PMA_CR_BIOS_REQ_MEMSS desc:
  * Register default value:        0x00000000
  * Register full path in IP: memss_pma_cr_sys_top/memss_pma_regs/ami_memss_pma_addrmap/MEMSS_PMA_CR_BIOS_REQ
  * Std path: VER=1, UVM=memss_pma_cr_sys_top.memss_pma_regs.MEMSS_PMA_CR_BIOS_REQ, OSX=memss_pma_cr_sys_top.memss_pma_regs.ami_memss_pma_addrmap.MEMSS_PMA_CR_BIOS_REQ
  * Security PolicyGroup: MEMSS_PMA_SAI_POLICY
  * This register is used as interface between MRC and MemSS PMA during memory initialization and training to communicate Qclk target ratio. LNL only: Register must be locked after MRC_DONE.
  */

#define MEMSS_PMA_CR_BIOS_REQ_MEMSS_REG  (0x00013d08U)

typedef union {
  struct {
    UINT32 qclk_ratio : 8; /**< Qclk ratio */

                            /* Bits[7:0], Access Type=RW, default=0x00000000*/

                            /* Qclk ratio with reference of 33.33MHz. */

    UINT32 gear_type : 1; /**< Gear Type */

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /*
                               Gear Type associated with requested Qclk ratio.
                               0x0: Gear2, 0x1: Gear4
                            */

    UINT32 dvfs_vddq : 5; /**< VDDQ Voltage in 50mV steps */

                            /* Bits[13:9], Access Type=RW, default=0x00000000*/

                            /* VDDQ Voltage in 50mV steps */

    UINT32 vddq_change_only : 1; /**< VDDQ change only indication */

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               VDDQ change only indication. 0 - disabled
                               1 - enabled  If set. PMA will perform only
                               handshake with Pcode
                            */

    UINT32 rsvd_0 : 3; /**< Undefined - auto filled rsvd_[17:15] */

                            /* Bits[17:15], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 max_bw_gbps : 11; /**< Max Bandwidth GBps */

                            /* Bits[28:18], Access Type=RW, default=0x00000000*/

                            /*
                               Maximum theoretical bandwidth supported at
                               the requested Qclk freqency in GB/s
                            */

    UINT32 qclk_wp_idx : 2; /**< QCLK WP Index */

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /* QClk WP index to be passed on to DDR PHY PMA */

    UINT32 run_busy : 1; /**< Run Busy bit */

                            /* Bits[31:31], Access Type=RW/V, default=0x00000000*/

                            /*
                               Run busy bit set by MRC to inform MemSS PMA
                               of new Qclk point request.
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_BIOS_REQ_MEMSS_STRUCT;

#define MEMSS_PMA_CR_BIOS_REQ_MEMSS_WIDTH 32

#define MEMSS_PMA_CR_BIOS_REQ_MEMSS_QCLK_RATIO_LSB 0x0000
#define MEMSS_PMA_CR_BIOS_REQ_MEMSS_QCLK_RATIO_SIZE 0x0008
#define MEMSS_PMA_CR_BIOS_REQ_MEMSS_GEAR_TYPE_LSB 0x0008
#define MEMSS_PMA_CR_BIOS_REQ_MEMSS_GEAR_TYPE_SIZE 0x0001
#define MEMSS_PMA_CR_BIOS_REQ_MEMSS_DVFS_VDDQ_LSB 0x0009
#define MEMSS_PMA_CR_BIOS_REQ_MEMSS_DVFS_VDDQ_SIZE 0x0005
#define MEMSS_PMA_CR_BIOS_REQ_MEMSS_VDDQ_CHANGE_ONLY_LSB 0x000e
#define MEMSS_PMA_CR_BIOS_REQ_MEMSS_VDDQ_CHANGE_ONLY_SIZE 0x0001
#define MEMSS_PMA_CR_BIOS_REQ_MEMSS_MAX_BW_GBPS_LSB 0x0012
#define MEMSS_PMA_CR_BIOS_REQ_MEMSS_MAX_BW_GBPS_SIZE 0x000b
#define MEMSS_PMA_CR_BIOS_REQ_MEMSS_QCLK_WP_IDX_LSB 0x001d
#define MEMSS_PMA_CR_BIOS_REQ_MEMSS_QCLK_WP_IDX_SIZE 0x0002
#define MEMSS_PMA_CR_BIOS_REQ_MEMSS_RUN_BUSY_LSB 0x001f
#define MEMSS_PMA_CR_BIOS_REQ_MEMSS_RUN_BUSY_SIZE 0x0001

#endif      // SV_HOOKS

/** MEMSS_PMA_CR_INIT_STATE_MEMSS desc:
  * Register default value:        0x00000000
  * Register full path in IP: memss_pma_cr_sys_top/memss_pma_regs/ami_memss_pma_addrmap/MEMSS_PMA_CR_INIT_STATE
  * Std path: VER=1, UVM=memss_pma_cr_sys_top.memss_pma_regs.MEMSS_PMA_CR_INIT_STATE, OSX=memss_pma_cr_sys_top.memss_pma_regs.ami_memss_pma_addrmap.MEMSS_PMA_CR_INIT_STATE
  * Security PolicyGroup: MEMSS_PMA_SAI_POLICY
  * This register is used as interface between MRC and MemSS PMA during memory initialization and training by MRC to communicate save or MRC done indication to MemSS PMA. LNL only: Register must be locked after MRC_DONE.
  */

#define MEMSS_PMA_CR_INIT_STATE_MEMSS_REG (0x00013d0cU)

typedef union {
  struct {
#ifdef SV_HOOKS
    UINT32 mrc_save0 : 1; /**< MRC GV0 Save Request */

                            /* Bits[0:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               MRC requests to save GV0 by setting the bit.
                               MemSS PMA acknowledges by clearing the bit
                               after save operation is complete.
                            */


#else
    UINT32 rsvd_0 : 1;

                            /* Bits[0:0], Access Type=RO, default=None*/

                            /* Reserved */
#endif      // SV_HOOKS

#ifdef SV_HOOKS
    UINT32 mrc_save1 : 1; /**< MRC GV1 Save Request */

                            /* Bits[1:1], Access Type=RW/V, default=0x00000000*/

                            /*
                               MRC requests to save GV1 by setting the bit.
                               MemSS PMA acknowledges by clearing the bit
                               after save operation is complete.
                            */


#else
    UINT32 rsvd_1 : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
#endif      // SV_HOOKS

#ifdef SV_HOOKS
    UINT32 mrc_save2 : 1; /**< MRC GV2 Save Request */

                            /* Bits[2:2], Access Type=RW/V, default=0x00000000*/

                            /*
                               MRC requests to save GV2 by setting the bit.
                               MemSS PMA acknowledges by clearing the bit
                               after save operation is complete.
                            */


#else
    UINT32 rsvd_2 : 1;

                            /* Bits[2:2], Access Type=RO, default=None*/

                            /* Reserved */
#endif      // SV_HOOKS

#ifdef SV_HOOKS
    UINT32 mrc_save3 : 1; /**< MRC GV3 Save Request */

                            /* Bits[3:3], Access Type=RW/V, default=0x00000000*/

                            /*
                               MRC requests to save GV3 by setting the bit.
                               MemSS PMA acknowledges by clearing the bit
                               after save operation is complete.
                            */


#else
    UINT32 rsvd_3 : 1;

                            /* Bits[3:3], Access Type=RO, default=None*/

                            /* Reserved */
#endif      // SV_HOOKS

    UINT32 mem_config_done : 1; /**< Memory Configuration Done */

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               BIOS sets the bit to indicate that all the
                               memory configuration is complet to MemSS PMA.
                            */

    UINT32 mem_config_done_ack : 1; /**< Memory Configuration Done Acknowledgement */

                            /* Bits[5:5], Access Type=RO/V, default=0x00000000*/

                            /* Mem Config Done Ack from MemSS PMA to MRC. */

    UINT32 reserved : 26; /**< Reserved bits */

                            /* Bits[31:6], Access Type=RW/V, default=0x00000000*/

                            /* Reserved */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_INIT_STATE_MEMSS_STRUCT;

#define MEMSS_PMA_CR_INIT_STATE_MEMSS_WIDTH 32

#ifdef SV_HOOKS
#define MEMSS_PMA_CR_INIT_STATE_MEMSS_MRC_SAVE0_LSB 0x0000
#define MEMSS_PMA_CR_INIT_STATE_MEMSS_MRC_SAVE0_SIZE 0x0001
#endif      // SV_HOOKS
#ifdef SV_HOOKS
#define MEMSS_PMA_CR_INIT_STATE_MEMSS_MRC_SAVE1_LSB 0x0001
#define MEMSS_PMA_CR_INIT_STATE_MEMSS_MRC_SAVE1_SIZE 0x0001
#endif      // SV_HOOKS
#ifdef SV_HOOKS
#define MEMSS_PMA_CR_INIT_STATE_MEMSS_MRC_SAVE2_LSB 0x0002
#define MEMSS_PMA_CR_INIT_STATE_MEMSS_MRC_SAVE2_SIZE 0x0001
#endif      // SV_HOOKS
#ifdef SV_HOOKS
#define MEMSS_PMA_CR_INIT_STATE_MEMSS_MRC_SAVE3_LSB 0x0003
#define MEMSS_PMA_CR_INIT_STATE_MEMSS_MRC_SAVE3_SIZE 0x0001
#endif      // SV_HOOKS
#define MEMSS_PMA_CR_INIT_STATE_MEMSS_MEM_CONFIG_DONE_LSB 0x0004
#define MEMSS_PMA_CR_INIT_STATE_MEMSS_MEM_CONFIG_DONE_SIZE 0x0001
#define MEMSS_PMA_CR_INIT_STATE_MEMSS_MEM_CONFIG_DONE_ACK_LSB 0x0005
#define MEMSS_PMA_CR_INIT_STATE_MEMSS_MEM_CONFIG_DONE_ACK_SIZE 0x0001
#define MEMSS_PMA_CR_INIT_STATE_MEMSS_RESERVED_LSB 0x0006
#define MEMSS_PMA_CR_INIT_STATE_MEMSS_RESERVED_SIZE 0x001a

#ifdef SV_HOOKS

/** MEMSS_PMA_CR_BIOS_DATA_MEMSS desc:
  * Register default value:        0x00000000
  * Register full path in IP: memss_pma_cr_sys_top/memss_pma_regs/ami_memss_pma_addrmap/MEMSS_PMA_CR_BIOS_DATA
  * Std path: VER=1, UVM=memss_pma_cr_sys_top.memss_pma_regs.MEMSS_PMA_CR_BIOS_DATA, OSX=memss_pma_cr_sys_top.memss_pma_regs.ami_memss_pma_addrmap.MEMSS_PMA_CR_BIOS_DATA
  * Security PolicyGroup: MEMSS_PMA_SAI_POLICY
  * This register is used as interface between MRC and MemSS PMA during memory initialization and training by MemSS PMA to communicate locked Qclk ratio to MRC. LNL only: Register must be locked after MRC_DONE.
  */

#define MEMSS_PMA_CR_BIOS_DATA_MEMSS_REG (0x00013d10U)

typedef union {
  struct {
    UINT32 qclk_ratio : 8; /**< Qclk Ratio */

                            /* Bits[7:0], Access Type=RO/V, default=0x00000000*/

                            /*
                               Qclk ratio locked by MemSS PMA with reference
                               of 33.33 MHz.
                            */

    UINT32 gear_type : 1; /**< Gear Type */

                            /* Bits[8:8], Access Type=RO/V, default=0x00000000*/

                            /*
                               Gear Type associated with locked Qclk ratio.
                               0x0: Gear2, 0x1: Gear4.
                            */

    UINT32 rsvd_0 : 7; /**< Undefined - auto filled rsvd_[15:09] */

                            /* Bits[15:9], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 vccmem_voltage : 16; /**< VccMEM Voltage value */

                            /* Bits[31:16], Access Type=RO/V, default=0x00000000*/

                            /*
                               VccMEM Voltage value as resolved by Punit
                               . This is a representation of MEMSS_PMA_CR_VOLTAGE_FOR_MRC
                            */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_BIOS_DATA_MEMSS_STRUCT;

#define MEMSS_PMA_CR_BIOS_DATA_MEMSS_WIDTH 32

#define MEMSS_PMA_CR_BIOS_DATA_MEMSS_QCLK_RATIO_LSB 0x0000
#define MEMSS_PMA_CR_BIOS_DATA_MEMSS_QCLK_RATIO_SIZE 0x0008
#define MEMSS_PMA_CR_BIOS_DATA_MEMSS_GEAR_TYPE_LSB 0x0008
#define MEMSS_PMA_CR_BIOS_DATA_MEMSS_GEAR_TYPE_SIZE 0x0001
#define MEMSS_PMA_CR_BIOS_DATA_MEMSS_VCCMEM_VOLTAGE_LSB 0x0010
#define MEMSS_PMA_CR_BIOS_DATA_MEMSS_VCCMEM_VOLTAGE_SIZE 0x0010

#endif      // SV_HOOKS


/** MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS desc:
  * Register default value:        0x00000000
  * Register full path in IP: memss_pma_cr_sys_top/memss_pma_regs/ami_memss_pma_addrmap/MEMSS_PMA_CR_BIOS_MAILBOX
  * Std path: VER=1, UVM=memss_pma_cr_sys_top.memss_pma_regs.MEMSS_PMA_CR_BIOS_MAILBOX, OSX=memss_pma_cr_sys_top.memss_pma_regs.ami_memss_pma_addrmap.MEMSS_PMA_CR_BIOS_MAILBOX
  * Security PolicyGroup: MEMSS_PMA_SAI_POLICY
  * The register is used as interface between MRC and MemSS PMA to enable miscellaneous features. LNL only: Register must be locked after MRC_DONE.
  */

#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_REG (0x00013d14U)

typedef union {
  struct {
    UINT32 ramp_vccsa_en : 1; /**< Ramp VccSA Enable */

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /*
                               MRC sets the bit to enable VCCSA change during
                               MRC training.
                            */

    UINT32 zq_is_disabled_0 : 1; /**< used for indication per GV point, if ZQ is disabled at that point */

                            /* Bits[1:1], Access Type=RW, default=0x00000000*/

                            /*
                               MRC sets the bit to indicate if at this GV
                               point ZQ is disabled . using cr: mcmnts_spare2_0_0_0_mchbar.spare_rw[1]
                               for GV point0
                            */

    UINT32 zq_is_disabled_1 : 1; /**< used for indication per GV point, if ZQ is disabled at that point */

                            /* Bits[2:2], Access Type=RW, default=0x00000000*/

                            /*
                               MRC sets the bit to indicate if at this GV
                               point ZQ is disabled . using cr: mcmnts_spare2_0_0_0_mchbar.spare_rw[1]
                               for GV point1
                            */

    UINT32 zq_is_disabled_2 : 1; /**< used for indication per GV point, if ZQ is disabled at that point */

                            /* Bits[3:3], Access Type=RW, default=0x00000000*/

                            /*
                               MRC sets the bit to indicate if at this GV
                               point ZQ is disabled . using cr: mcmnts_spare2_0_0_0_mchbar.spare_rw[1]
                               for GV point2
                            */

    UINT32 zq_is_disabled_3 : 1; /**< used for indication per GV point, if ZQ is disabled at that point */

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /*
                               MRC sets the bit to indicate if at this GV
                               point ZQ is disabled . using cr: mcmnts_spare2_0_0_0_mchbar.spare_rw[1]
                               for GV point3
                            */

    UINT32 l_shape_config : 1; /**< L-shape memory configuration */

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /*
                               L-shape memory configuration 0 - L Shape disabled-
                               Symmetric memory per MC 1 - L shape enabled
                               - Asymmetric memory per MC
                            */

    UINT32 edvfsc_is_enabled_0 : 1; /**< EDVFSC is enabled in GV point 0 */

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* LPDDR5 E-DVFSC is enabled in GV point */

    UINT32 edvfsc_is_enabled_1 : 1; /**< EDVFSC is enabled in GV point 1 */

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* LPDDR5 E-DVFSC is enabled in GV point */

    UINT32 edvfsc_is_enabled_2 : 1; /**< EDVFSC is enabled in GV point 2 */

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /* LPDDR5 E-DVFSC is enabled in GV point */

    UINT32 edvfsc_is_enabled_3 : 1; /**< EDVFSC is enabled in GV point 3 */

                            /* Bits[9:9], Access Type=RW, default=0x00000000*/

                            /* LPDDR5 E-DVFSC is enabled in GV point */

    UINT32 reserved : 22; /**< Reserved bits */

                            /* Bits[31:10], Access Type=RW, default=0x00000000*/

                            /* Reserved */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_STRUCT;

#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_WIDTH 32

#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_RAMP_VCCSA_EN_LSB 0x0000
#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_RAMP_VCCSA_EN_SIZE 0x0001
#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_ZQ_IS_DISABLED_0_LSB 0x0001
#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_ZQ_IS_DISABLED_0_SIZE 0x0001
#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_ZQ_IS_DISABLED_1_LSB 0x0002
#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_ZQ_IS_DISABLED_1_SIZE 0x0001
#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_ZQ_IS_DISABLED_2_LSB 0x0003
#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_ZQ_IS_DISABLED_2_SIZE 0x0001
#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_ZQ_IS_DISABLED_3_LSB 0x0004
#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_ZQ_IS_DISABLED_3_SIZE 0x0001
#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_L_SHAPE_CONFIG_LSB 0x0005
#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_L_SHAPE_CONFIG_SIZE 0x0001
#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_EDVFSC_IS_ENABLED_0_LSB 0x0006
#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_EDVFSC_IS_ENABLED_0_SIZE 0x0001
#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_EDVFSC_IS_ENABLED_1_LSB 0x0007
#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_EDVFSC_IS_ENABLED_1_SIZE 0x0001
#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_EDVFSC_IS_ENABLED_2_LSB 0x0008
#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_EDVFSC_IS_ENABLED_2_SIZE 0x0001
#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_EDVFSC_IS_ENABLED_3_LSB 0x0009
#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_EDVFSC_IS_ENABLED_3_SIZE 0x0001
#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_RESERVED_LSB 0x000a
#define MEMSS_PMA_CR_BIOS_MAILBOX_MEMSS_RESERVED_SIZE 0x0016

#ifdef SV_HOOKS

/** MEMSS_PMA_CR_BIOS_ERROR_STATUS_MEMSS desc:
  * Register default value:        0x000000FF
  * Register full path in IP: memss_pma_cr_sys_top/memss_pma_regs/ami_memss_pma_addrmap/MEMSS_PMA_CR_BIOS_ERROR_STATUS
  * Std path: VER=1, UVM=memss_pma_cr_sys_top.memss_pma_regs.MEMSS_PMA_CR_BIOS_ERROR_STATUS, OSX=memss_pma_cr_sys_top.memss_pma_regs.ami_memss_pma_addrmap.MEMSS_PMA_CR_BIOS_ERROR_STATUS
  * Security PolicyGroup: MEMSS_PMA_SAI_POLICY
  * The register is used as interface between MRC and MemSS PMA to communicate error status to MRC. LNL only: Register must be locked after MRC_DONE.
  */

#define MEMSS_PMA_CR_BIOS_ERROR_STATUS_MEMSS_REG (0x00013d18U)

typedef union {
  struct {
    UINT32 error_code : 8; /**< Error Code */

                            /* Bits[7:0], Access Type=RO/V, default=0x000000FF*/

                            /*
                               Error status indication from MemSS PMA to
                               MRC. MRC must check these bits after MemSS
                               PMA clears MEMSS_PMA_CR_BIOS_REQ.RUN_BUSY.
                               0xFF indicates no error.
                            */

    UINT32 rsvd_0 : 24; /**< Undefined - auto filled rsvd_[31:08] */

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_BIOS_ERROR_STATUS_MEMSS_STRUCT;

#define MEMSS_PMA_CR_BIOS_ERROR_STATUS_MEMSS_WIDTH 32

#define MEMSS_PMA_CR_BIOS_ERROR_STATUS_MEMSS_ERROR_CODE_LSB 0x0000
#define MEMSS_PMA_CR_BIOS_ERROR_STATUS_MEMSS_ERROR_CODE_SIZE 0x0008

#endif      // SV_HOOKS


/** MEMSS_PMA_CR_MRC_VERSION_MEMSS desc:
  * Register default value:        0x00000000
  * Register full path in IP: memss_pma_cr_sys_top/memss_pma_regs/ami_memss_pma_addrmap/MEMSS_PMA_CR_MRC_VERSION
  * Std path: VER=1, UVM=memss_pma_cr_sys_top.memss_pma_regs.MEMSS_PMA_CR_MRC_VERSION, OSX=memss_pma_cr_sys_top.memss_pma_regs.ami_memss_pma_addrmap.MEMSS_PMA_CR_MRC_VERSION
  * Security PolicyGroup: MEMSS_PMA_SAI_POLICY
  * This register holds the BIOS MRC revision.
  */

#define MEMSS_PMA_CR_MRC_VERSION_MEMSS_REG (0x00013d40U)

typedef union {
  struct {
    UINT32 mrc_revision : 32; /**< MRC revision */

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /* MRC revision field */

  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMSS_PMA_CR_MRC_VERSION_MEMSS_STRUCT;

#define MEMSS_PMA_CR_MRC_VERSION_MEMSS_WIDTH 32

#define MEMSS_PMA_CR_MRC_VERSION_MEMSS_MRC_REVISION_LSB 0x0000
#define MEMSS_PMA_CR_MRC_VERSION_MEMSS_MRC_REVISION_SIZE 0x0020

//************************************************ /RegisterStructs

#endif      // _MEMSS_h

