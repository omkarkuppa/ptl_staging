/** @file
  DISP.h

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


#ifndef _DISP_h
#define _DISP_h



// IP GUID: 5e6f35c4-4393-41d9-8aa3-a4a0f804f6a3
#define DISP_ID 0x5e6f35c4439341d9

//************************************************ RegisterStructs

#ifdef SV_HOOKS

/** RM_TIMEOUT desc:
  */

#define RM_TIMEOUT_DISP_REG              (0x00042060U)

typedef union {
  struct {
    UINT32 rm_timeout_value : 16; /**< RM TIMEOUT VALUE */
                            /* Bits[15:0], default=0x000005DC*/
    UINT32 rsvd_0 : 15; /**< Undefined - auto filled rsvd_[30:16] */
                            /* Bits[30:16], default=0*/
    UINT32 rm_timeout : 1; /**< RM TIMEOUT */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RM_TIMEOUT_DISP_STRUCT;

#define RM_TIMEOUT_DISP_WIDTH 32
#define RM_TIMEOUT_DISP_DEFAULT 0x000005dcU
#define RM_TIMEOUT_DISP_ENDPOINT DISP

#define RM_TIMEOUT_DISP_RM_TIMEOUT_VALUE_LSB 0x0000
#define RM_TIMEOUT_DISP_RM_TIMEOUT_VALUE_SIZE 0x0010
#define RM_TIMEOUT_DISP_RM_TIMEOUT_LSB 0x001f
#define RM_TIMEOUT_DISP_RM_TIMEOUT_SIZE 0x0001

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** CHICKEN_BIOS_SECURED desc:
  */

#define CHICKEN_BIOS_SECURED_DISP_REG    (0x00042094U)

typedef union {
  struct {
    UINT32 pavp_check_for_vrh_dis : 1; /**< PAVP CHECK FOR VRH DIS */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 dbuf_wipe_dis : 1; /**< DBUF WIPE DIS */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 override_crc_restriction : 1; /**< OVERRIDE CRC RESTRICTION */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 vga_io_through_mmio_disable : 1; /**< VGA IO THROUGH MMIO DISABLE */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 disable_display_page_tables : 1; /**< DISABLE DISPLAY PAGE TABLES */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 ignore_pm_fill_abort : 1; /**< IGNORE PM FILL ABORT */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 hp_and_lp_idle_mem_status_disable : 1; /**< HP AND LP IDLE MEM STATUS DISABLE */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 enable_pavp_valid_readback : 1; /**< ENABLE PAVP VALID READBACK */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 spare_8 : 1; /**< SPARE 8 */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 spare_9 : 1; /**< SPARE 9 */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 spare_10 : 1; /**< SPARE 10 */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 spare_11 : 1; /**< SPARE 11 */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 spare_12 : 1; /**< SPARE 12 */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 spare_13 : 1; /**< SPARE 13 */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 spare_14 : 1; /**< SPARE 14 */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 spare_15 : 1; /**< SPARE 15 */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 spare_16 : 1; /**< SPARE 16 */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 spare_17 : 1; /**< SPARE 17 */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 spare_18 : 1; /**< SPARE 18 */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 spare_19 : 1; /**< SPARE 19 */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 spare_20 : 1; /**< SPARE 20 */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 spare_21 : 1; /**< SPARE 21 */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 spare_22 : 1; /**< SPARE 22 */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 spare_23 : 1; /**< SPARE 23 */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 spare_24 : 1; /**< SPARE 24 */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 spare_25 : 1; /**< SPARE 25 */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 spare_26 : 1; /**< SPARE 26 */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 spare_27 : 1; /**< SPARE 27 */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 spare_28 : 1; /**< SPARE 28 */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 spare_29 : 1; /**< SPARE 29 */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 spare_30 : 1; /**< SPARE 30 */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 spare_31 : 1; /**< SPARE 31 */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CHICKEN_BIOS_SECURED_DISP_STRUCT;

#define CHICKEN_BIOS_SECURED_DISP_WIDTH 32
#define CHICKEN_BIOS_SECURED_DISP_DEFAULT 0x00000000U
#define CHICKEN_BIOS_SECURED_DISP_ENDPOINT DISP

#define CHICKEN_BIOS_SECURED_DISP_PAVP_CHECK_FOR_VRH_DIS_LSB 0x0000
#define CHICKEN_BIOS_SECURED_DISP_PAVP_CHECK_FOR_VRH_DIS_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_DBUF_WIPE_DIS_LSB 0x0001
#define CHICKEN_BIOS_SECURED_DISP_DBUF_WIPE_DIS_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_OVERRIDE_CRC_RESTRICTION_LSB 0x0002
#define CHICKEN_BIOS_SECURED_DISP_OVERRIDE_CRC_RESTRICTION_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_VGA_IO_THROUGH_MMIO_DISABLE_LSB 0x0003
#define CHICKEN_BIOS_SECURED_DISP_VGA_IO_THROUGH_MMIO_DISABLE_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_DISABLE_DISPLAY_PAGE_TABLES_LSB 0x0004
#define CHICKEN_BIOS_SECURED_DISP_DISABLE_DISPLAY_PAGE_TABLES_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_IGNORE_PM_FILL_ABORT_LSB 0x0005
#define CHICKEN_BIOS_SECURED_DISP_IGNORE_PM_FILL_ABORT_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_HP_AND_LP_IDLE_MEM_STATUS_DISABLE_LSB 0x0006
#define CHICKEN_BIOS_SECURED_DISP_HP_AND_LP_IDLE_MEM_STATUS_DISABLE_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_ENABLE_PAVP_VALID_READBACK_LSB 0x0007
#define CHICKEN_BIOS_SECURED_DISP_ENABLE_PAVP_VALID_READBACK_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_8_LSB 0x0008
#define CHICKEN_BIOS_SECURED_DISP_SPARE_8_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_9_LSB 0x0009
#define CHICKEN_BIOS_SECURED_DISP_SPARE_9_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_10_LSB 0x000a
#define CHICKEN_BIOS_SECURED_DISP_SPARE_10_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_11_LSB 0x000b
#define CHICKEN_BIOS_SECURED_DISP_SPARE_11_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_12_LSB 0x000c
#define CHICKEN_BIOS_SECURED_DISP_SPARE_12_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_13_LSB 0x000d
#define CHICKEN_BIOS_SECURED_DISP_SPARE_13_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_14_LSB 0x000e
#define CHICKEN_BIOS_SECURED_DISP_SPARE_14_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_15_LSB 0x000f
#define CHICKEN_BIOS_SECURED_DISP_SPARE_15_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_16_LSB 0x0010
#define CHICKEN_BIOS_SECURED_DISP_SPARE_16_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_17_LSB 0x0011
#define CHICKEN_BIOS_SECURED_DISP_SPARE_17_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_18_LSB 0x0012
#define CHICKEN_BIOS_SECURED_DISP_SPARE_18_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_19_LSB 0x0013
#define CHICKEN_BIOS_SECURED_DISP_SPARE_19_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_20_LSB 0x0014
#define CHICKEN_BIOS_SECURED_DISP_SPARE_20_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_21_LSB 0x0015
#define CHICKEN_BIOS_SECURED_DISP_SPARE_21_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_22_LSB 0x0016
#define CHICKEN_BIOS_SECURED_DISP_SPARE_22_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_23_LSB 0x0017
#define CHICKEN_BIOS_SECURED_DISP_SPARE_23_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_24_LSB 0x0018
#define CHICKEN_BIOS_SECURED_DISP_SPARE_24_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_25_LSB 0x0019
#define CHICKEN_BIOS_SECURED_DISP_SPARE_25_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_26_LSB 0x001a
#define CHICKEN_BIOS_SECURED_DISP_SPARE_26_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_27_LSB 0x001b
#define CHICKEN_BIOS_SECURED_DISP_SPARE_27_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_28_LSB 0x001c
#define CHICKEN_BIOS_SECURED_DISP_SPARE_28_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_29_LSB 0x001d
#define CHICKEN_BIOS_SECURED_DISP_SPARE_29_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_30_LSB 0x001e
#define CHICKEN_BIOS_SECURED_DISP_SPARE_30_SIZE 0x0001
#define CHICKEN_BIOS_SECURED_DISP_SPARE_31_LSB 0x001f
#define CHICKEN_BIOS_SECURED_DISP_SPARE_31_SIZE 0x0001

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DPCE_WB_CTL desc:
  */

#define DPCE_WB_CTL_DISP_REG             (0x00042310U)

typedef union {
  struct {
    UINT32 writeback_counter_threshold : 10; /**< WRITEBACK COUNTER THRESHOLD */
                            /* Bits[9:0], default=0x00000080*/
    UINT32 rsvd_0 : 18; /**< Undefined - auto filled rsvd_[27:10] */
                            /* Bits[27:10], default=0*/
    UINT32 missing_write_completions : 1; /**< MISSING WRITE COMPLETIONS */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 excess_write_completions : 1; /**< EXCESS WRITE COMPLETIONS */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 dpce_writeback_completion_disable : 1; /**< DPCE WRITEBACK COMPLETION DISABLE */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 clear_writeback_counter : 1; /**< CLEAR WRITEBACK COUNTER */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DPCE_WB_CTL_DISP_STRUCT;

#define DPCE_WB_CTL_DISP_WIDTH 32
#define DPCE_WB_CTL_DISP_DEFAULT 0x00000080U
#define DPCE_WB_CTL_DISP_ENDPOINT DISP

#define DPCE_WB_CTL_DISP_WRITEBACK_COUNTER_THRESHOLD_LSB 0x0000
#define DPCE_WB_CTL_DISP_WRITEBACK_COUNTER_THRESHOLD_SIZE 0x000a
#define DPCE_WB_CTL_DISP_MISSING_WRITE_COMPLETIONS_LSB 0x001c
#define DPCE_WB_CTL_DISP_MISSING_WRITE_COMPLETIONS_SIZE 0x0001
#define DPCE_WB_CTL_DISP_EXCESS_WRITE_COMPLETIONS_LSB 0x001d
#define DPCE_WB_CTL_DISP_EXCESS_WRITE_COMPLETIONS_SIZE 0x0001
#define DPCE_WB_CTL_DISP_DPCE_WRITEBACK_COMPLETION_DISABLE_LSB 0x001e
#define DPCE_WB_CTL_DISP_DPCE_WRITEBACK_COMPLETION_DISABLE_SIZE 0x0001
#define DPCE_WB_CTL_DISP_CLEAR_WRITEBACK_COUNTER_LSB 0x001f
#define DPCE_WB_CTL_DISP_CLEAR_WRITEBACK_COUNTER_SIZE 0x0001

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DCPR_PFET_EN_DELAY desc:
  */

#define DCPR_PFET_EN_DELAY_DISP_REG      (0x0004240cU)

typedef union {
  struct {
    UINT32 powerup_pfet_delay : 10; /**< POWERUP PFET DELAY */
                            /* Bits[9:0], default=0x0000015E*/
    UINT32 forceon_pfet_delay : 10; /**< FORCEON PFET DELAY */
                            /* Bits[19:10], default=0x0000015E*/
    UINT32 ebb_pfet_delay : 10; /**< EBB PFET DELAY */
                            /* Bits[29:20], default=0x0000015E*/
    UINT32 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[31:30] */
                            /* Bits[31:30], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DCPR_PFET_EN_DELAY_DISP_STRUCT;

#define DCPR_PFET_EN_DELAY_DISP_WIDTH 32
#define DCPR_PFET_EN_DELAY_DISP_DEFAULT 0x15e5795eU
#define DCPR_PFET_EN_DELAY_DISP_ENDPOINT DISP

#define DCPR_PFET_EN_DELAY_DISP_POWERUP_PFET_DELAY_LSB 0x0000
#define DCPR_PFET_EN_DELAY_DISP_POWERUP_PFET_DELAY_SIZE 0x000a
#define DCPR_PFET_EN_DELAY_DISP_FORCEON_PFET_DELAY_LSB 0x000a
#define DCPR_PFET_EN_DELAY_DISP_FORCEON_PFET_DELAY_SIZE 0x000a
#define DCPR_PFET_EN_DELAY_DISP_EBB_PFET_DELAY_LSB 0x0014
#define DCPR_PFET_EN_DELAY_DISP_EBB_PFET_DELAY_SIZE 0x000a

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DCPR_PFET_EN_DELAY_PG1 desc:
  */

#define DCPR_PFET_EN_DELAY_PG1_DISP_REG  (0x00042410U)

typedef union {
  struct {
    UINT32 powerup_pfet_delay : 10; /**< POWERUP PFET DELAY */
                            /* Bits[9:0], default=0x00000019*/
    UINT32 forceon_pfet_delay : 10; /**< FORCEON PFET DELAY */
                            /* Bits[19:10], default=0x00000019*/
    UINT32 ebb_pfet_delay : 10; /**< EBB PFET DELAY */
                            /* Bits[29:20], default=0x00000019*/
    UINT32 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[31:30] */
                            /* Bits[31:30], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DCPR_PFET_EN_DELAY_PG1_DISP_STRUCT;

#define DCPR_PFET_EN_DELAY_PG1_DISP_WIDTH 32
#define DCPR_PFET_EN_DELAY_PG1_DISP_DEFAULT 0x01906419U
#define DCPR_PFET_EN_DELAY_PG1_DISP_ENDPOINT DISP

#define DCPR_PFET_EN_DELAY_PG1_DISP_POWERUP_PFET_DELAY_LSB 0x0000
#define DCPR_PFET_EN_DELAY_PG1_DISP_POWERUP_PFET_DELAY_SIZE 0x000a
#define DCPR_PFET_EN_DELAY_PG1_DISP_FORCEON_PFET_DELAY_LSB 0x000a
#define DCPR_PFET_EN_DELAY_PG1_DISP_FORCEON_PFET_DELAY_SIZE 0x000a
#define DCPR_PFET_EN_DELAY_PG1_DISP_EBB_PFET_DELAY_LSB 0x0014
#define DCPR_PFET_EN_DELAY_PG1_DISP_EBB_PFET_DELAY_SIZE 0x000a

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** PMA_TIMEOUT desc:
  */

#define PMA_TIMEOUT_DISP_REG             (0x00045620U)

typedef union {
  struct {
    UINT32 pm_fill_timeout : 12; /**< PM FILL TIMEOUT */
                            /* Bits[11:0], default=0x000001F4*/
    UINT32 sagv_req_timeout_value : 12; /**< SAGV REQ TIMEOUT VALUE */
                            /* Bits[23:12], default=0x000001F4*/
    UINT32 hp_and_lp_idle_stable_count : 5; /**< HP AND LP IDLE STABLE COUNT */
                            /* Bits[28:24], default=0x00000008*/
    UINT32 rsvd_0 : 3; /**< Undefined - auto filled rsvd_[31:29] */
                            /* Bits[31:29], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PMA_TIMEOUT_DISP_STRUCT;

#define PMA_TIMEOUT_DISP_WIDTH 32
#define PMA_TIMEOUT_DISP_DEFAULT 0x081f41f4U
#define PMA_TIMEOUT_DISP_ENDPOINT DISP

#define PMA_TIMEOUT_DISP_PM_FILL_TIMEOUT_LSB 0x0000
#define PMA_TIMEOUT_DISP_PM_FILL_TIMEOUT_SIZE 0x000c
#define PMA_TIMEOUT_DISP_SAGV_REQ_TIMEOUT_VALUE_LSB 0x000c
#define PMA_TIMEOUT_DISP_SAGV_REQ_TIMEOUT_VALUE_SIZE 0x000c
#define PMA_TIMEOUT_DISP_HP_AND_LP_IDLE_STABLE_COUNT_LSB 0x0018
#define PMA_TIMEOUT_DISP_HP_AND_LP_IDLE_STABLE_COUNT_SIZE 0x0005

#endif      // SV_HOOKS


/** MEM_SS_INFO_GLOBAL desc:
  */

#define MEM_SS_INFO_GLOBAL_DISP_REG      (0x00045700U)

typedef union {
  struct {
    UINT32 ddr_type : 4; /**< DDR TYPE */
                            /* Bits[3:0], default=0x00000000*/
    UINT32 number_of_populated_channels : 4; /**< NUMBER OF POPULATED CHANNELS */
                            /* Bits[7:4], default=0x00000000*/
    UINT32 number_of_enabled_qgv_points : 4; /**< NUMBER OF ENABLED QGV POINTS */
                            /* Bits[11:8], default=0x00000000*/
    UINT32 spare : 20; /**< SPARE */
                            /* Bits[31:12], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEM_SS_INFO_GLOBAL_DISP_STRUCT;

#define MEM_SS_INFO_GLOBAL_DISP_WIDTH 32
#define MEM_SS_INFO_GLOBAL_DISP_DEFAULT 0x00000000U
#define MEM_SS_INFO_GLOBAL_DISP_ENDPOINT DISP

#define MEM_SS_INFO_GLOBAL_DISP_DDR_TYPE_LSB 0x0000
#define MEM_SS_INFO_GLOBAL_DISP_DDR_TYPE_SIZE 0x0004
#define MEM_SS_INFO_GLOBAL_DISP_NUMBER_OF_POPULATED_CHANNELS_LSB 0x0004
#define MEM_SS_INFO_GLOBAL_DISP_NUMBER_OF_POPULATED_CHANNELS_SIZE 0x0004
#define MEM_SS_INFO_GLOBAL_DISP_NUMBER_OF_ENABLED_QGV_POINTS_LSB 0x0008
#define MEM_SS_INFO_GLOBAL_DISP_NUMBER_OF_ENABLED_QGV_POINTS_SIZE 0x0004
#define MEM_SS_INFO_GLOBAL_DISP_SPARE_LSB 0x000c
#define MEM_SS_INFO_GLOBAL_DISP_SPARE_SIZE 0x0014

/** MEM_SS_INFO_QGV_POINT_0_0 desc:
  */

#define MEM_SS_INFO_QGV_POINT_0_0_DISP_REG (0x00045710U)

typedef union {
  struct {
    UINT32 dclk_in_multiples_of_16_6666_mhz : 16; /**< DCLK IN MULTIPLES OF 16 6666 MHZ */
                            /* Bits[15:0], default=0x00000000*/
    UINT32 trp_in_dclks : 8; /**< TRP IN DCLKS */
                            /* Bits[23:16], default=0x00000000*/
    UINT32 trcd_in_dclks : 8; /**< TRCD IN DCLKS */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEM_SS_INFO_QGV_POINT_0_0_DISP_STRUCT;

#define MEM_SS_INFO_QGV_POINT_0_0_DISP_WIDTH 32
#define MEM_SS_INFO_QGV_POINT_0_0_DISP_DEFAULT 0x00000000U
#define MEM_SS_INFO_QGV_POINT_0_0_DISP_ENDPOINT DISP

#define MEM_SS_INFO_QGV_POINT_0_0_DISP_DCLK_IN_MULTIPLES_OF_16_6666_MHZ_LSB 0x0000
#define MEM_SS_INFO_QGV_POINT_0_0_DISP_DCLK_IN_MULTIPLES_OF_16_6666_MHZ_SIZE 0x0010
#define MEM_SS_INFO_QGV_POINT_0_0_DISP_TRP_IN_DCLKS_LSB 0x0010
#define MEM_SS_INFO_QGV_POINT_0_0_DISP_TRP_IN_DCLKS_SIZE 0x0008
#define MEM_SS_INFO_QGV_POINT_0_0_DISP_TRCD_IN_DCLKS_LSB 0x0018
#define MEM_SS_INFO_QGV_POINT_0_0_DISP_TRCD_IN_DCLKS_SIZE 0x0008

/** MEM_SS_INFO_QGV_POINT_0_1 desc:
  */

#define MEM_SS_INFO_QGV_POINT_0_1_DISP_REG (0x00045714U)

typedef union {
  struct {
    UINT32 trdpre_in_dclks : 8; /**< TRDPRE IN DCLKS */
                            /* Bits[7:0], default=0x00000000*/
    UINT32 tras_in_dclks : 9; /**< TRAS IN DCLKS */
                            /* Bits[16:8], default=0x00000000*/
    UINT32 rsvd_0 : 15; /**< Undefined - auto filled rsvd_[31:17] */
                            /* Bits[31:17], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEM_SS_INFO_QGV_POINT_0_1_DISP_STRUCT;

#define MEM_SS_INFO_QGV_POINT_0_1_DISP_WIDTH 32
#define MEM_SS_INFO_QGV_POINT_0_1_DISP_DEFAULT 0x00000000U
#define MEM_SS_INFO_QGV_POINT_0_1_DISP_ENDPOINT DISP

#define MEM_SS_INFO_QGV_POINT_0_1_DISP_TRDPRE_IN_DCLKS_LSB 0x0000
#define MEM_SS_INFO_QGV_POINT_0_1_DISP_TRDPRE_IN_DCLKS_SIZE 0x0008
#define MEM_SS_INFO_QGV_POINT_0_1_DISP_TRAS_IN_DCLKS_LSB 0x0008
#define MEM_SS_INFO_QGV_POINT_0_1_DISP_TRAS_IN_DCLKS_SIZE 0x0009

/** MEM_SS_INFO_QGV_POINT_1_0 desc:
  */

#define MEM_SS_INFO_QGV_POINT_1_0_DISP_REG (0x00045718U)

typedef union {
  struct {
    UINT32 dclk_in_multiples_of_16_6666_mhz : 16; /**< DCLK IN MULTIPLES OF 16 6666 MHZ */
                            /* Bits[15:0], default=0x00000000*/
    UINT32 trp_in_dclks : 8; /**< TRP IN DCLKS */
                            /* Bits[23:16], default=0x00000000*/
    UINT32 trcd_in_dclks : 8; /**< TRCD IN DCLKS */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEM_SS_INFO_QGV_POINT_1_0_DISP_STRUCT;

#define MEM_SS_INFO_QGV_POINT_1_0_DISP_WIDTH 32
#define MEM_SS_INFO_QGV_POINT_1_0_DISP_DEFAULT 0x00000000U
#define MEM_SS_INFO_QGV_POINT_1_0_DISP_ENDPOINT DISP

#define MEM_SS_INFO_QGV_POINT_1_0_DISP_DCLK_IN_MULTIPLES_OF_16_6666_MHZ_LSB 0x0000
#define MEM_SS_INFO_QGV_POINT_1_0_DISP_DCLK_IN_MULTIPLES_OF_16_6666_MHZ_SIZE 0x0010
#define MEM_SS_INFO_QGV_POINT_1_0_DISP_TRP_IN_DCLKS_LSB 0x0010
#define MEM_SS_INFO_QGV_POINT_1_0_DISP_TRP_IN_DCLKS_SIZE 0x0008
#define MEM_SS_INFO_QGV_POINT_1_0_DISP_TRCD_IN_DCLKS_LSB 0x0018
#define MEM_SS_INFO_QGV_POINT_1_0_DISP_TRCD_IN_DCLKS_SIZE 0x0008

/** MEM_SS_INFO_QGV_POINT_1_1 desc:
  */

#define MEM_SS_INFO_QGV_POINT_1_1_DISP_REG (0x0004571cU)

typedef union {
  struct {
    UINT32 trdpre_in_dclks : 8; /**< TRDPRE IN DCLKS */
                            /* Bits[7:0], default=0x00000000*/
    UINT32 tras_in_dclks : 9; /**< TRAS IN DCLKS */
                            /* Bits[16:8], default=0x00000000*/
    UINT32 rsvd_0 : 15; /**< Undefined - auto filled rsvd_[31:17] */
                            /* Bits[31:17], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEM_SS_INFO_QGV_POINT_1_1_DISP_STRUCT;

#define MEM_SS_INFO_QGV_POINT_1_1_DISP_WIDTH 32
#define MEM_SS_INFO_QGV_POINT_1_1_DISP_DEFAULT 0x00000000U
#define MEM_SS_INFO_QGV_POINT_1_1_DISP_ENDPOINT DISP

#define MEM_SS_INFO_QGV_POINT_1_1_DISP_TRDPRE_IN_DCLKS_LSB 0x0000
#define MEM_SS_INFO_QGV_POINT_1_1_DISP_TRDPRE_IN_DCLKS_SIZE 0x0008
#define MEM_SS_INFO_QGV_POINT_1_1_DISP_TRAS_IN_DCLKS_LSB 0x0008
#define MEM_SS_INFO_QGV_POINT_1_1_DISP_TRAS_IN_DCLKS_SIZE 0x0009

/** MEM_SS_INFO_QGV_POINT_2_0 desc:
  */

#define MEM_SS_INFO_QGV_POINT_2_0_DISP_REG (0x00045720U)

typedef union {
  struct {
    UINT32 dclk_in_multiples_of_16_6666_mhz : 16; /**< DCLK IN MULTIPLES OF 16 6666 MHZ */
                            /* Bits[15:0], default=0x00000000*/
    UINT32 trp_in_dclks : 8; /**< TRP IN DCLKS */
                            /* Bits[23:16], default=0x00000000*/
    UINT32 trcd_in_dclks : 8; /**< TRCD IN DCLKS */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEM_SS_INFO_QGV_POINT_2_0_DISP_STRUCT;

#define MEM_SS_INFO_QGV_POINT_2_0_DISP_WIDTH 32
#define MEM_SS_INFO_QGV_POINT_2_0_DISP_DEFAULT 0x00000000U
#define MEM_SS_INFO_QGV_POINT_2_0_DISP_ENDPOINT DISP

#define MEM_SS_INFO_QGV_POINT_2_0_DISP_DCLK_IN_MULTIPLES_OF_16_6666_MHZ_LSB 0x0000
#define MEM_SS_INFO_QGV_POINT_2_0_DISP_DCLK_IN_MULTIPLES_OF_16_6666_MHZ_SIZE 0x0010
#define MEM_SS_INFO_QGV_POINT_2_0_DISP_TRP_IN_DCLKS_LSB 0x0010
#define MEM_SS_INFO_QGV_POINT_2_0_DISP_TRP_IN_DCLKS_SIZE 0x0008
#define MEM_SS_INFO_QGV_POINT_2_0_DISP_TRCD_IN_DCLKS_LSB 0x0018
#define MEM_SS_INFO_QGV_POINT_2_0_DISP_TRCD_IN_DCLKS_SIZE 0x0008


/** MEM_SS_INFO_QGV_POINT_2_1 desc:
  */

#define MEM_SS_INFO_QGV_POINT_2_1_DISP_REG (0x00045724U)

typedef union {
  struct {
    UINT32 trdpre_in_dclks : 8; /**< TRDPRE IN DCLKS */
                            /* Bits[7:0], default=0x00000000*/
    UINT32 tras_in_dclks : 9; /**< TRAS IN DCLKS */
                            /* Bits[16:8], default=0x00000000*/
    UINT32 rsvd_0 : 15; /**< Undefined - auto filled rsvd_[31:17] */
                            /* Bits[31:17], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEM_SS_INFO_QGV_POINT_2_1_DISP_STRUCT;

#define MEM_SS_INFO_QGV_POINT_2_1_DISP_WIDTH 32
#define MEM_SS_INFO_QGV_POINT_2_1_DISP_DEFAULT 0x00000000U
#define MEM_SS_INFO_QGV_POINT_2_1_DISP_ENDPOINT DISP

#define MEM_SS_INFO_QGV_POINT_2_1_DISP_TRDPRE_IN_DCLKS_LSB 0x0000
#define MEM_SS_INFO_QGV_POINT_2_1_DISP_TRDPRE_IN_DCLKS_SIZE 0x0008
#define MEM_SS_INFO_QGV_POINT_2_1_DISP_TRAS_IN_DCLKS_LSB 0x0008
#define MEM_SS_INFO_QGV_POINT_2_1_DISP_TRAS_IN_DCLKS_SIZE 0x0009

/** MEM_SS_INFO_QGV_POINT_3_0 desc:
  */

#define MEM_SS_INFO_QGV_POINT_3_0_DISP_REG (0x00045728U)

typedef union {
  struct {
    UINT32 dclk_in_multiples_of_16_6666_mhz : 16; /**< DCLK IN MULTIPLES OF 16 6666 MHZ */
                            /* Bits[15:0], default=0x00000000*/
    UINT32 trp_in_dclks : 8; /**< TRP IN DCLKS */
                            /* Bits[23:16], default=0x00000000*/
    UINT32 trcd_in_dclks : 8; /**< TRCD IN DCLKS */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEM_SS_INFO_QGV_POINT_3_0_DISP_STRUCT;

#define MEM_SS_INFO_QGV_POINT_3_0_DISP_WIDTH 32
#define MEM_SS_INFO_QGV_POINT_3_0_DISP_DEFAULT 0x00000000U
#define MEM_SS_INFO_QGV_POINT_3_0_DISP_ENDPOINT DISP

#define MEM_SS_INFO_QGV_POINT_3_0_DISP_DCLK_IN_MULTIPLES_OF_16_6666_MHZ_LSB 0x0000
#define MEM_SS_INFO_QGV_POINT_3_0_DISP_DCLK_IN_MULTIPLES_OF_16_6666_MHZ_SIZE 0x0010
#define MEM_SS_INFO_QGV_POINT_3_0_DISP_TRP_IN_DCLKS_LSB 0x0010
#define MEM_SS_INFO_QGV_POINT_3_0_DISP_TRP_IN_DCLKS_SIZE 0x0008
#define MEM_SS_INFO_QGV_POINT_3_0_DISP_TRCD_IN_DCLKS_LSB 0x0018
#define MEM_SS_INFO_QGV_POINT_3_0_DISP_TRCD_IN_DCLKS_SIZE 0x0008

/** MEM_SS_INFO_QGV_POINT_3_1 desc:
  */

#define MEM_SS_INFO_QGV_POINT_3_1_DISP_REG (0x0004572cU)

typedef union {
  struct {
    UINT32 trdpre_in_dclks : 8; /**< TRDPRE IN DCLKS */
                            /* Bits[7:0], default=0x00000000*/
    UINT32 tras_in_dclks : 9; /**< TRAS IN DCLKS */
                            /* Bits[16:8], default=0x00000000*/
    UINT32 rsvd_0 : 15; /**< Undefined - auto filled rsvd_[31:17] */
                            /* Bits[31:17], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEM_SS_INFO_QGV_POINT_3_1_DISP_STRUCT;

#define MEM_SS_INFO_QGV_POINT_3_1_DISP_WIDTH 32
#define MEM_SS_INFO_QGV_POINT_3_1_DISP_DEFAULT 0x00000000U
#define MEM_SS_INFO_QGV_POINT_3_1_DISP_ENDPOINT DISP

#define MEM_SS_INFO_QGV_POINT_3_1_DISP_TRDPRE_IN_DCLKS_LSB 0x0000
#define MEM_SS_INFO_QGV_POINT_3_1_DISP_TRDPRE_IN_DCLKS_SIZE 0x0008
#define MEM_SS_INFO_QGV_POINT_3_1_DISP_TRAS_IN_DCLKS_LSB 0x0008
#define MEM_SS_INFO_QGV_POINT_3_1_DISP_TRAS_IN_DCLKS_SIZE 0x0009


#ifdef SV_HOOKS

/** MEM_SS_INFO_QGV_POINT_4_0 desc:
  */

#define MEM_SS_INFO_QGV_POINT_4_0_DISP_REG (0x00045730U)

typedef union {
  struct {
    UINT32 dclk_in_multiples_of_16_6666_mhz : 16; /**< DCLK IN MULTIPLES OF 16 6666 MHZ */
                            /* Bits[15:0], default=0x00000000*/
    UINT32 trp_in_dclks : 8; /**< TRP IN DCLKS */
                            /* Bits[23:16], default=0x00000000*/
    UINT32 trcd_in_dclks : 8; /**< TRCD IN DCLKS */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEM_SS_INFO_QGV_POINT_4_0_DISP_STRUCT;

#define MEM_SS_INFO_QGV_POINT_4_0_DISP_WIDTH 32
#define MEM_SS_INFO_QGV_POINT_4_0_DISP_DEFAULT 0x00000000U
#define MEM_SS_INFO_QGV_POINT_4_0_DISP_ENDPOINT DISP

#define MEM_SS_INFO_QGV_POINT_4_0_DISP_DCLK_IN_MULTIPLES_OF_16_6666_MHZ_LSB 0x0000
#define MEM_SS_INFO_QGV_POINT_4_0_DISP_DCLK_IN_MULTIPLES_OF_16_6666_MHZ_SIZE 0x0010
#define MEM_SS_INFO_QGV_POINT_4_0_DISP_TRP_IN_DCLKS_LSB 0x0010
#define MEM_SS_INFO_QGV_POINT_4_0_DISP_TRP_IN_DCLKS_SIZE 0x0008
#define MEM_SS_INFO_QGV_POINT_4_0_DISP_TRCD_IN_DCLKS_LSB 0x0018
#define MEM_SS_INFO_QGV_POINT_4_0_DISP_TRCD_IN_DCLKS_SIZE 0x0008

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** MEM_SS_INFO_QGV_POINT_4_1 desc:
  */

#define MEM_SS_INFO_QGV_POINT_4_1_DISP_REG (0x00045734U)

typedef union {
  struct {
    UINT32 trdpre_in_dclks : 8; /**< TRDPRE IN DCLKS */
                            /* Bits[7:0], default=0x00000000*/
    UINT32 tras_in_dclks : 9; /**< TRAS IN DCLKS */
                            /* Bits[16:8], default=0x00000000*/
    UINT32 rsvd_0 : 15; /**< Undefined - auto filled rsvd_[31:17] */
                            /* Bits[31:17], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEM_SS_INFO_QGV_POINT_4_1_DISP_STRUCT;

#define MEM_SS_INFO_QGV_POINT_4_1_DISP_WIDTH 32
#define MEM_SS_INFO_QGV_POINT_4_1_DISP_DEFAULT 0x00000000U
#define MEM_SS_INFO_QGV_POINT_4_1_DISP_ENDPOINT DISP

#define MEM_SS_INFO_QGV_POINT_4_1_DISP_TRDPRE_IN_DCLKS_LSB 0x0000
#define MEM_SS_INFO_QGV_POINT_4_1_DISP_TRDPRE_IN_DCLKS_SIZE 0x0008
#define MEM_SS_INFO_QGV_POINT_4_1_DISP_TRAS_IN_DCLKS_LSB 0x0008
#define MEM_SS_INFO_QGV_POINT_4_1_DISP_TRAS_IN_DCLKS_SIZE 0x0009

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** MEM_SS_INFO_QGV_POINT_5_0 desc:
  */

#define MEM_SS_INFO_QGV_POINT_5_0_DISP_REG (0x00045738U)

typedef union {
  struct {
    UINT32 dclk_in_multiples_of_16_6666_mhz : 16; /**< DCLK IN MULTIPLES OF 16 6666 MHZ */
                            /* Bits[15:0], default=0x00000000*/
    UINT32 trp_in_dclks : 8; /**< TRP IN DCLKS */
                            /* Bits[23:16], default=0x00000000*/
    UINT32 trcd_in_dclks : 8; /**< TRCD IN DCLKS */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEM_SS_INFO_QGV_POINT_5_0_DISP_STRUCT;

#define MEM_SS_INFO_QGV_POINT_5_0_DISP_WIDTH 32
#define MEM_SS_INFO_QGV_POINT_5_0_DISP_DEFAULT 0x00000000U
#define MEM_SS_INFO_QGV_POINT_5_0_DISP_ENDPOINT DISP

#define MEM_SS_INFO_QGV_POINT_5_0_DISP_DCLK_IN_MULTIPLES_OF_16_6666_MHZ_LSB 0x0000
#define MEM_SS_INFO_QGV_POINT_5_0_DISP_DCLK_IN_MULTIPLES_OF_16_6666_MHZ_SIZE 0x0010
#define MEM_SS_INFO_QGV_POINT_5_0_DISP_TRP_IN_DCLKS_LSB 0x0010
#define MEM_SS_INFO_QGV_POINT_5_0_DISP_TRP_IN_DCLKS_SIZE 0x0008
#define MEM_SS_INFO_QGV_POINT_5_0_DISP_TRCD_IN_DCLKS_LSB 0x0018
#define MEM_SS_INFO_QGV_POINT_5_0_DISP_TRCD_IN_DCLKS_SIZE 0x0008

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** MEM_SS_INFO_QGV_POINT_5_1 desc:
  */

#define MEM_SS_INFO_QGV_POINT_5_1_DISP_REG (0x0004573cU)

typedef union {
  struct {
    UINT32 trdpre_in_dclks : 8; /**< TRDPRE IN DCLKS */
                            /* Bits[7:0], default=0x00000000*/
    UINT32 tras_in_dclks : 9; /**< TRAS IN DCLKS */
                            /* Bits[16:8], default=0x00000000*/
    UINT32 rsvd_0 : 15; /**< Undefined - auto filled rsvd_[31:17] */
                            /* Bits[31:17], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEM_SS_INFO_QGV_POINT_5_1_DISP_STRUCT;

#define MEM_SS_INFO_QGV_POINT_5_1_DISP_WIDTH 32
#define MEM_SS_INFO_QGV_POINT_5_1_DISP_DEFAULT 0x00000000U
#define MEM_SS_INFO_QGV_POINT_5_1_DISP_ENDPOINT DISP

#define MEM_SS_INFO_QGV_POINT_5_1_DISP_TRDPRE_IN_DCLKS_LSB 0x0000
#define MEM_SS_INFO_QGV_POINT_5_1_DISP_TRDPRE_IN_DCLKS_SIZE 0x0008
#define MEM_SS_INFO_QGV_POINT_5_1_DISP_TRAS_IN_DCLKS_LSB 0x0008
#define MEM_SS_INFO_QGV_POINT_5_1_DISP_TRAS_IN_DCLKS_SIZE 0x0009

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** MEM_SS_INFO_QGV_POINT_6_0 desc:
  */

#define MEM_SS_INFO_QGV_POINT_6_0_DISP_REG (0x00045740U)

typedef union {
  struct {
    UINT32 dclk_in_multiples_of_16_6666_mhz : 16; /**< DCLK IN MULTIPLES OF 16 6666 MHZ */
                            /* Bits[15:0], default=0x00000000*/
    UINT32 trp_in_dclks : 8; /**< TRP IN DCLKS */
                            /* Bits[23:16], default=0x00000000*/
    UINT32 trcd_in_dclks : 8; /**< TRCD IN DCLKS */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEM_SS_INFO_QGV_POINT_6_0_DISP_STRUCT;

#define MEM_SS_INFO_QGV_POINT_6_0_DISP_WIDTH 32
#define MEM_SS_INFO_QGV_POINT_6_0_DISP_DEFAULT 0x00000000U
#define MEM_SS_INFO_QGV_POINT_6_0_DISP_ENDPOINT DISP

#define MEM_SS_INFO_QGV_POINT_6_0_DISP_DCLK_IN_MULTIPLES_OF_16_6666_MHZ_LSB 0x0000
#define MEM_SS_INFO_QGV_POINT_6_0_DISP_DCLK_IN_MULTIPLES_OF_16_6666_MHZ_SIZE 0x0010
#define MEM_SS_INFO_QGV_POINT_6_0_DISP_TRP_IN_DCLKS_LSB 0x0010
#define MEM_SS_INFO_QGV_POINT_6_0_DISP_TRP_IN_DCLKS_SIZE 0x0008
#define MEM_SS_INFO_QGV_POINT_6_0_DISP_TRCD_IN_DCLKS_LSB 0x0018
#define MEM_SS_INFO_QGV_POINT_6_0_DISP_TRCD_IN_DCLKS_SIZE 0x0008

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** MEM_SS_INFO_QGV_POINT_6_1 desc:
  */

#define MEM_SS_INFO_QGV_POINT_6_1_DISP_REG (0x00045744U)

typedef union {
  struct {
    UINT32 trdpre_in_dclks : 8; /**< TRDPRE IN DCLKS */
                            /* Bits[7:0], default=0x00000000*/
    UINT32 tras_in_dclks : 9; /**< TRAS IN DCLKS */
                            /* Bits[16:8], default=0x00000000*/
    UINT32 rsvd_0 : 15; /**< Undefined - auto filled rsvd_[31:17] */
                            /* Bits[31:17], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEM_SS_INFO_QGV_POINT_6_1_DISP_STRUCT;

#define MEM_SS_INFO_QGV_POINT_6_1_DISP_WIDTH 32
#define MEM_SS_INFO_QGV_POINT_6_1_DISP_DEFAULT 0x00000000U
#define MEM_SS_INFO_QGV_POINT_6_1_DISP_ENDPOINT DISP

#define MEM_SS_INFO_QGV_POINT_6_1_DISP_TRDPRE_IN_DCLKS_LSB 0x0000
#define MEM_SS_INFO_QGV_POINT_6_1_DISP_TRDPRE_IN_DCLKS_SIZE 0x0008
#define MEM_SS_INFO_QGV_POINT_6_1_DISP_TRAS_IN_DCLKS_LSB 0x0008
#define MEM_SS_INFO_QGV_POINT_6_1_DISP_TRAS_IN_DCLKS_SIZE 0x0009

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** MEM_SS_INFO_QGV_POINT_7_0 desc:
  */

#define MEM_SS_INFO_QGV_POINT_7_0_DISP_REG (0x00045748U)

typedef union {
  struct {
    UINT32 dclk_in_multiples_of_16_6666_mhz : 16; /**< DCLK IN MULTIPLES OF 16 6666 MHZ */
                            /* Bits[15:0], default=0x00000000*/
    UINT32 trp_in_dclks : 8; /**< TRP IN DCLKS */
                            /* Bits[23:16], default=0x00000000*/
    UINT32 trcd_in_dclks : 8; /**< TRCD IN DCLKS */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEM_SS_INFO_QGV_POINT_7_0_DISP_STRUCT;

#define MEM_SS_INFO_QGV_POINT_7_0_DISP_WIDTH 32
#define MEM_SS_INFO_QGV_POINT_7_0_DISP_DEFAULT 0x00000000U
#define MEM_SS_INFO_QGV_POINT_7_0_DISP_ENDPOINT DISP

#define MEM_SS_INFO_QGV_POINT_7_0_DISP_DCLK_IN_MULTIPLES_OF_16_6666_MHZ_LSB 0x0000
#define MEM_SS_INFO_QGV_POINT_7_0_DISP_DCLK_IN_MULTIPLES_OF_16_6666_MHZ_SIZE 0x0010
#define MEM_SS_INFO_QGV_POINT_7_0_DISP_TRP_IN_DCLKS_LSB 0x0010
#define MEM_SS_INFO_QGV_POINT_7_0_DISP_TRP_IN_DCLKS_SIZE 0x0008
#define MEM_SS_INFO_QGV_POINT_7_0_DISP_TRCD_IN_DCLKS_LSB 0x0018
#define MEM_SS_INFO_QGV_POINT_7_0_DISP_TRCD_IN_DCLKS_SIZE 0x0008

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** MEM_SS_INFO_QGV_POINT_7_1 desc:
  */

#define MEM_SS_INFO_QGV_POINT_7_1_DISP_REG (0x0004574cU)

typedef union {
  struct {
    UINT32 trdpre_in_dclks : 8; /**< TRDPRE IN DCLKS */
                            /* Bits[7:0], default=0x00000000*/
    UINT32 tras_in_dclks : 9; /**< TRAS IN DCLKS */
                            /* Bits[16:8], default=0x00000000*/
    UINT32 rsvd_0 : 15; /**< Undefined - auto filled rsvd_[31:17] */
                            /* Bits[31:17], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEM_SS_INFO_QGV_POINT_7_1_DISP_STRUCT;

#define MEM_SS_INFO_QGV_POINT_7_1_DISP_WIDTH 32
#define MEM_SS_INFO_QGV_POINT_7_1_DISP_DEFAULT 0x00000000U
#define MEM_SS_INFO_QGV_POINT_7_1_DISP_ENDPOINT DISP

#define MEM_SS_INFO_QGV_POINT_7_1_DISP_TRDPRE_IN_DCLKS_LSB 0x0000
#define MEM_SS_INFO_QGV_POINT_7_1_DISP_TRDPRE_IN_DCLKS_SIZE 0x0008
#define MEM_SS_INFO_QGV_POINT_7_1_DISP_TRAS_IN_DCLKS_LSB 0x0008
#define MEM_SS_INFO_QGV_POINT_7_1_DISP_TRAS_IN_DCLKS_SIZE 0x0009

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** LATENCY_LP0_LP1 desc:
  */

#define LATENCY_LP0_LP1_DISP_REG         (0x00045780U)

typedef union {
  struct {
    UINT32 latency_level_0 : 13; /**< LATENCY LEVEL 0 */
                            /* Bits[12:0], default=0x00000000*/
    UINT32 rsvd_0 : 3; /**< Undefined - auto filled rsvd_[15:13] */
                            /* Bits[15:13], default=0*/
    UINT32 latency_level_1 : 13; /**< LATENCY LEVEL 1 */
                            /* Bits[28:16], default=0x00000000*/
    UINT32 rsvd_1 : 3; /**< Undefined - auto filled rsvd_[31:29] */
                            /* Bits[31:29], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} LATENCY_LP0_LP1_DISP_STRUCT;

#define LATENCY_LP0_LP1_DISP_WIDTH 32
#define LATENCY_LP0_LP1_DISP_DEFAULT 0x00000000U
#define LATENCY_LP0_LP1_DISP_ENDPOINT DISP

#define LATENCY_LP0_LP1_DISP_LATENCY_LEVEL_0_LSB 0x0000
#define LATENCY_LP0_LP1_DISP_LATENCY_LEVEL_0_SIZE 0x000d
#define LATENCY_LP0_LP1_DISP_LATENCY_LEVEL_1_LSB 0x0010
#define LATENCY_LP0_LP1_DISP_LATENCY_LEVEL_1_SIZE 0x000d

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** LATENCY_LP2_LP3 desc:
  */

#define LATENCY_LP2_LP3_DISP_REG         (0x00045784U)

typedef union {
  struct {
    UINT32 latency_level_2 : 13; /**< LATENCY LEVEL 2 */
                            /* Bits[12:0], default=0x00000000*/
    UINT32 rsvd_0 : 3; /**< Undefined - auto filled rsvd_[15:13] */
                            /* Bits[15:13], default=0*/
    UINT32 latency_level_3 : 13; /**< LATENCY LEVEL 3 */
                            /* Bits[28:16], default=0x00000000*/
    UINT32 rsvd_1 : 3; /**< Undefined - auto filled rsvd_[31:29] */
                            /* Bits[31:29], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} LATENCY_LP2_LP3_DISP_STRUCT;

#define LATENCY_LP2_LP3_DISP_WIDTH 32
#define LATENCY_LP2_LP3_DISP_DEFAULT 0x00000000U
#define LATENCY_LP2_LP3_DISP_ENDPOINT DISP

#define LATENCY_LP2_LP3_DISP_LATENCY_LEVEL_2_LSB 0x0000
#define LATENCY_LP2_LP3_DISP_LATENCY_LEVEL_2_SIZE 0x000d
#define LATENCY_LP2_LP3_DISP_LATENCY_LEVEL_3_LSB 0x0010
#define LATENCY_LP2_LP3_DISP_LATENCY_LEVEL_3_SIZE 0x000d

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** LATENCY_LP4_LP5 desc:
  */

#define LATENCY_LP4_LP5_DISP_REG         (0x00045788U)

typedef union {
  struct {
    UINT32 latency_level_4 : 13; /**< LATENCY LEVEL 4 */
                            /* Bits[12:0], default=0x00000000*/
    UINT32 rsvd_0 : 3; /**< Undefined - auto filled rsvd_[15:13] */
                            /* Bits[15:13], default=0*/
    UINT32 latency_level_5 : 13; /**< LATENCY LEVEL 5 */
                            /* Bits[28:16], default=0x00000000*/
    UINT32 rsvd_1 : 3; /**< Undefined - auto filled rsvd_[31:29] */
                            /* Bits[31:29], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} LATENCY_LP4_LP5_DISP_STRUCT;

#define LATENCY_LP4_LP5_DISP_WIDTH 32
#define LATENCY_LP4_LP5_DISP_DEFAULT 0x00000000U
#define LATENCY_LP4_LP5_DISP_ENDPOINT DISP

#define LATENCY_LP4_LP5_DISP_LATENCY_LEVEL_4_LSB 0x0000
#define LATENCY_LP4_LP5_DISP_LATENCY_LEVEL_4_SIZE 0x000d
#define LATENCY_LP4_LP5_DISP_LATENCY_LEVEL_5_LSB 0x0010
#define LATENCY_LP4_LP5_DISP_LATENCY_LEVEL_5_SIZE 0x000d

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** LATENCY_SAGV desc:
  */

#define LATENCY_SAGV_DISP_REG            (0x0004578cU)

typedef union {
  struct {
    UINT32 latency_level_qclk_sagv : 13; /**< LATENCY LEVEL QCLK SAGV */
                            /* Bits[12:0], default=0x00000000*/
    UINT32 rsvd_0 : 19; /**< Undefined - auto filled rsvd_[31:13] */
                            /* Bits[31:13], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} LATENCY_SAGV_DISP_STRUCT;

#define LATENCY_SAGV_DISP_WIDTH 32
#define LATENCY_SAGV_DISP_DEFAULT 0x00000000U
#define LATENCY_SAGV_DISP_ENDPOINT DISP

#define LATENCY_SAGV_DISP_LATENCY_LEVEL_QCLK_SAGV_LSB 0x0000
#define LATENCY_SAGV_DISP_LATENCY_LEVEL_QCLK_SAGV_SIZE 0x000d

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** PGFSM_POWERGOOD_COUNTER desc:
  */

#define PGFSM_POWERGOOD_COUNTER_DISP_REG (0x000457bcU)

typedef union {
  struct {
    UINT32 pwr_good_cntr_pg1 : 10; /**< PWR GOOD CNTR PG1 */
                            /* Bits[9:0], default=0x00000019*/
    UINT32 pwr_good_cntr : 10; /**< PWR GOOD CNTR */
                            /* Bits[19:10], default=0x0000015E*/
    UINT32 rsvd_0 : 12; /**< Undefined - auto filled rsvd_[31:20] */
                            /* Bits[31:20], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PGFSM_POWERGOOD_COUNTER_DISP_STRUCT;

#define PGFSM_POWERGOOD_COUNTER_DISP_WIDTH 32
#define PGFSM_POWERGOOD_COUNTER_DISP_DEFAULT 0x00057819U
#define PGFSM_POWERGOOD_COUNTER_DISP_ENDPOINT DISP

#define PGFSM_POWERGOOD_COUNTER_DISP_PWR_GOOD_CNTR_PG1_LSB 0x0000
#define PGFSM_POWERGOOD_COUNTER_DISP_PWR_GOOD_CNTR_PG1_SIZE 0x000a
#define PGFSM_POWERGOOD_COUNTER_DISP_PWR_GOOD_CNTR_LSB 0x000a
#define PGFSM_POWERGOOD_COUNTER_DISP_PWR_GOOD_CNTR_SIZE 0x000a

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DCPR_DEEP_SLEEP_DELAY desc:
  */

#define DCPR_DEEP_SLEEP_DELAY_DISP_REG   (0x00045ff0U)

typedef union {
  struct {
    UINT32 ds_delay_pgpipe : 4; /**< DS DELAY PGPIPE */
                            /* Bits[3:0], default=0x00000001*/
    UINT32 ds_delay_pg0 : 4; /**< DS DELAY PG0 */
                            /* Bits[7:4], default=0x00000001*/
    UINT32 rsvd_0 : 24; /**< Undefined - auto filled rsvd_[31:08] */
                            /* Bits[31:8], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DCPR_DEEP_SLEEP_DELAY_DISP_STRUCT;

#define DCPR_DEEP_SLEEP_DELAY_DISP_WIDTH 32
#define DCPR_DEEP_SLEEP_DELAY_DISP_DEFAULT 0x00000011U
#define DCPR_DEEP_SLEEP_DELAY_DISP_ENDPOINT DISP

#define DCPR_DEEP_SLEEP_DELAY_DISP_DS_DELAY_PGPIPE_LSB 0x0000
#define DCPR_DEEP_SLEEP_DELAY_DISP_DS_DELAY_PGPIPE_SIZE 0x0004
#define DCPR_DEEP_SLEEP_DELAY_DISP_DS_DELAY_PG0_LSB 0x0004
#define DCPR_DEEP_SLEEP_DELAY_DISP_DS_DELAY_PG0_SIZE 0x0004

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** CDCLK_CTL desc:
  */

#define CDCLK_CTL_DISP_REG               (0x00046000U)

typedef union {
  struct {
    UINT32 rsvd_0 : 17; /**< Undefined - auto filled rsvd_[16:00] */
                            /* Bits[16:0], default=0*/
    UINT32 override_to_crystal : 1; /**< OVERRIDE TO CRYSTAL */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 override_to_slow_clock : 1; /**< OVERRIDE TO SLOW CLOCK */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 rsvd_1 : 3; /**< Undefined - auto filled rsvd_[21:19] */
                            /* Bits[21:19], default=0*/
    UINT32 cd2x_divider_select : 2; /**< CD2X DIVIDER SELECT */
                            /* Bits[23:22], default=0x00000000*/
    UINT32 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[24:24] */
                            /* Bits[24:24], default=0*/
    UINT32 mdclk_source_select : 1; /**< MDCLK SOURCE SELECT */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 rsvd_3 : 6; /**< Undefined - auto filled rsvd_[31:26] */
                            /* Bits[31:26], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CDCLK_CTL_DISP_STRUCT;

#define CDCLK_CTL_DISP_WIDTH 32
#define CDCLK_CTL_DISP_DEFAULT 0x00000000U
#define CDCLK_CTL_DISP_ENDPOINT DISP

#define CDCLK_CTL_DISP_OVERRIDE_TO_CRYSTAL_LSB 0x0011
#define CDCLK_CTL_DISP_OVERRIDE_TO_CRYSTAL_SIZE 0x0001
#define CDCLK_CTL_DISP_OVERRIDE_TO_SLOW_CLOCK_LSB 0x0012
#define CDCLK_CTL_DISP_OVERRIDE_TO_SLOW_CLOCK_SIZE 0x0001
#define CDCLK_CTL_DISP_CD2X_DIVIDER_SELECT_LSB 0x0016
#define CDCLK_CTL_DISP_CD2X_DIVIDER_SELECT_SIZE 0x0002
#define CDCLK_CTL_DISP_MDCLK_SOURCE_SELECT_LSB 0x0019
#define CDCLK_CTL_DISP_MDCLK_SOURCE_SELECT_SIZE 0x0001

#endif      // SV_HOOKS

/** NDE_RSTWRN_OPT desc:
  */

#define NDE_RSTWRN_OPT_DISP_REG          (0x00046408U)

typedef union {
  struct {
    UINT32 rsvd_0 : 4; /**< Undefined - auto filled rsvd_[03:00] */
                            /* Bits[3:0], default=0*/
    UINT32 rst_pch_handshake_en : 1; /**< RST PCH HANDSHAKE EN */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[05:05] */
                            /* Bits[5:5], default=0*/
    UINT32 rst_pica_handshake_en : 1; /**< RST PICA HANDSHAKE EN */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 disable_reset_wake_lock : 1; /**< DISABLE RESET WAKE LOCK */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 rsvd_2 : 24; /**< Undefined - auto filled rsvd_[31:08] */
                            /* Bits[31:8], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} NDE_RSTWRN_OPT_DISP_STRUCT;

#define NDE_RSTWRN_OPT_DISP_WIDTH 32
#define NDE_RSTWRN_OPT_DISP_DEFAULT 0x00000000U
#define NDE_RSTWRN_OPT_DISP_ENDPOINT DISP

#define NDE_RSTWRN_OPT_DISP_RST_PCH_HANDSHAKE_EN_LSB 0x0004
#define NDE_RSTWRN_OPT_DISP_RST_PCH_HANDSHAKE_EN_SIZE 0x0001
#define NDE_RSTWRN_OPT_DISP_RST_PICA_HANDSHAKE_EN_LSB 0x0006
#define NDE_RSTWRN_OPT_DISP_RST_PICA_HANDSHAKE_EN_SIZE 0x0001
#define NDE_RSTWRN_OPT_DISP_DISABLE_RESET_WAKE_LOCK_LSB 0x0007
#define NDE_RSTWRN_OPT_DISP_DISABLE_RESET_WAKE_LOCK_SIZE 0x0001

#ifdef SV_HOOKS

/** AUDIO_PIN_BUF_CTL desc:
  */

#define AUDIO_PIN_BUF_CTL_DISP_REG       (0x00048414U)

typedef union {
  struct {
    UINT32 rsvd_0 : 31; /**< Undefined - auto filled rsvd_[30:00] */
                            /* Bits[30:0], default=0*/
    UINT32 enable : 1; /**< ENABLE */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} AUDIO_PIN_BUF_CTL_DISP_STRUCT;

#define AUDIO_PIN_BUF_CTL_DISP_WIDTH 32
#define AUDIO_PIN_BUF_CTL_DISP_DEFAULT 0x00000000U
#define AUDIO_PIN_BUF_CTL_DISP_ENDPOINT DISP

#define AUDIO_PIN_BUF_CTL_DISP_ENABLE_LSB 0x001f
#define AUDIO_PIN_BUF_CTL_DISP_ENABLE_SIZE 0x0001

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DE_ID desc:
  */

#define DE_ID_DISP_REG                   (0x000510a0U)

typedef union {
  struct {
    UINT32 id : 32; /**< ID */
                            /* Bits[31:0], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DE_ID_DISP_STRUCT;

#define DE_ID_DISP_WIDTH 32
#define DE_ID_DISP_DEFAULT 0x00000000U
#define DE_ID_DISP_ENDPOINT DISP

#define DE_ID_DISP_ID_LSB 0x0000
#define DE_ID_DISP_ID_SIZE 0x0020

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** AUD_FREQ_CNTRL_TOP desc:
  */

#define AUD_FREQ_CNTRL_TOP_DISP_REG      (0x00065900U)

typedef union {
  struct {
    UINT32 rsvd_0 : 3; /**< Undefined - auto filled rsvd_[02:00] */
                            /* Bits[2:0], default=0*/
    UINT32 _48mhz_bclk : 1; /**< 48MHZ BCLK */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 _96mhz_bclk : 1; /**< 96MHZ BCLK */
                            /* Bits[4:4], default=0x00000001*/
    UINT32 rsvd_1 : 6; /**< Undefined - auto filled rsvd_[10:05] */
                            /* Bits[10:5], default=0*/
    UINT32 detect_frame_sync_early : 2; /**< DETECT FRAME SYNC EARLY */
                            /* Bits[12:11], default=0x00000000*/
    UINT32 bypass_flop : 1; /**< BYPASS FLOP */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 tmode : 2; /**< TMODE */
                            /* Bits[15:14], default=0x00000002*/
    UINT32 rsvd_2 : 16; /**< Undefined - auto filled rsvd_[31:16] */
                            /* Bits[31:16], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} AUD_FREQ_CNTRL_TOP_DISP_STRUCT;

#define AUD_FREQ_CNTRL_TOP_DISP_WIDTH 32
#define AUD_FREQ_CNTRL_TOP_DISP_DEFAULT 0x00008010U
#define AUD_FREQ_CNTRL_TOP_DISP_ENDPOINT DISP

#define AUD_FREQ_CNTRL_TOP_DISP__48MHZ_BCLK_LSB 0x0003
#define AUD_FREQ_CNTRL_TOP_DISP__48MHZ_BCLK_SIZE 0x0001
#define AUD_FREQ_CNTRL_TOP_DISP__96MHZ_BCLK_LSB 0x0004
#define AUD_FREQ_CNTRL_TOP_DISP__96MHZ_BCLK_SIZE 0x0001
#define AUD_FREQ_CNTRL_TOP_DISP_DETECT_FRAME_SYNC_EARLY_LSB 0x000b
#define AUD_FREQ_CNTRL_TOP_DISP_DETECT_FRAME_SYNC_EARLY_SIZE 0x0002
#define AUD_FREQ_CNTRL_TOP_DISP_BYPASS_FLOP_LSB 0x000d
#define AUD_FREQ_CNTRL_TOP_DISP_BYPASS_FLOP_SIZE 0x0001
#define AUD_FREQ_CNTRL_TOP_DISP_TMODE_LSB 0x000e
#define AUD_FREQ_CNTRL_TOP_DISP_TMODE_SIZE 0x0002

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DE_POISON_DATA_HANDLING_ENABLE desc:
  */

#define DE_POISON_DATA_HANDLING_ENABLE_DISP_REG (0x00134000U)

typedef union {
  struct {
    UINT32 error_control : 1; /**< ERROR CONTROL */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 31; /**< Undefined - auto filled rsvd_[31:01] */
                            /* Bits[31:1], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DE_POISON_DATA_HANDLING_ENABLE_DISP_STRUCT;

#define DE_POISON_DATA_HANDLING_ENABLE_DISP_WIDTH 32
#define DE_POISON_DATA_HANDLING_ENABLE_DISP_DEFAULT 0x00000000U
#define DE_POISON_DATA_HANDLING_ENABLE_DISP_ENDPOINT DISP

#define DE_POISON_DATA_HANDLING_ENABLE_DISP_ERROR_CONTROL_LSB 0x0000
#define DE_POISON_DATA_HANDLING_ENABLE_DISP_ERROR_CONTROL_SIZE 0x0001

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DE_POISON_DATA_STATUS desc:
  */

#define DE_POISON_DATA_STATUS_DISP_REG   (0x00134004U)

typedef union {
  struct {
    UINT32 error_status : 1; /**< ERROR STATUS */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 7; /**< Undefined - auto filled rsvd_[07:01] */
                            /* Bits[7:1], default=0*/
    UINT32 dedicated_path_error_status0 : 1; /**< DEDICATED PATH ERROR STATUS0 */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 dedicated_path_error_status1 : 1; /**< DEDICATED PATH ERROR STATUS1 */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_1 : 22; /**< Undefined - auto filled rsvd_[31:10] */
                            /* Bits[31:10], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DE_POISON_DATA_STATUS_DISP_STRUCT;

#define DE_POISON_DATA_STATUS_DISP_WIDTH 32
#define DE_POISON_DATA_STATUS_DISP_DEFAULT 0x00000000U
#define DE_POISON_DATA_STATUS_DISP_ENDPOINT DISP

#define DE_POISON_DATA_STATUS_DISP_ERROR_STATUS_LSB 0x0000
#define DE_POISON_DATA_STATUS_DISP_ERROR_STATUS_SIZE 0x0001
#define DE_POISON_DATA_STATUS_DISP_DEDICATED_PATH_ERROR_STATUS0_LSB 0x0008
#define DE_POISON_DATA_STATUS_DISP_DEDICATED_PATH_ERROR_STATUS0_SIZE 0x0001
#define DE_POISON_DATA_STATUS_DISP_DEDICATED_PATH_ERROR_STATUS1_LSB 0x0009
#define DE_POISON_DATA_STATUS_DISP_DEDICATED_PATH_ERROR_STATUS1_SIZE 0x0001

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DEFUSA_IOSF_PARITY_CNTRL desc:
  */

#define DEFUSA_IOSF_PARITY_CNTRL_DISP_REG (0x00134010U)

typedef union {
  struct {
    UINT32 defusa_iosf_parity_cntrl_lock : 1; /**< DEFUSA IOSF PARITY CNTRL LOCK */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 3; /**< Undefined - auto filled rsvd_[03:01] */
                            /* Bits[3:1], default=0*/
    UINT32 psf_0_data_parity_error_inj : 2; /**< PSF 0 DATA PARITY ERROR INJ */
                            /* Bits[5:4], default=0x00000000*/
    UINT32 psf_0_cmd_parity_err_inj : 1; /**< PSF 0 CMD PARITY ERR INJ */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[07:07] */
                            /* Bits[7:7], default=0*/
    UINT32 psf_0_data_parity_chk_en : 1; /**< PSF 0 DATA PARITY CHK EN */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 psf_0_cmd_parity_chk_en : 1; /**< PSF 0 CMD PARITY CHK EN */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_2 : 2; /**< Undefined - auto filled rsvd_[11:10] */
                            /* Bits[11:10], default=0*/
    UINT32 psf_0_data_parity_gen_dis : 1; /**< PSF 0 DATA PARITY GEN DIS */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 psf_0_cmd_parity_gen_dis : 1; /**< PSF 0 CMD PARITY GEN DIS */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 rsvd_3 : 8; /**< Undefined - auto filled rsvd_[21:14] */
                            /* Bits[21:14], default=0*/
    UINT32 psf0_dip_cmd_parity_err_inj : 1; /**< PSF0 DIP CMD PARITY ERR INJ */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 psf1_dip_cmd_parity_err_inj : 1; /**< PSF1 DIP CMD PARITY ERR INJ */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 psf_dip_data_parity_chk_en : 1; /**< PSF DIP DATA PARITY CHK EN */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 psf_dip_cmd_parity_chk_en : 1; /**< PSF DIP CMD PARITY CHK EN */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 rsvd_4 : 3; /**< Undefined - auto filled rsvd_[28:26] */
                            /* Bits[28:26], default=0*/
    UINT32 psf_dip_cmd_parity_gen_dis : 1; /**< PSF DIP CMD PARITY GEN DIS */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 rsvd_5 : 2; /**< Undefined - auto filled rsvd_[31:30] */
                            /* Bits[31:30], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DEFUSA_IOSF_PARITY_CNTRL_DISP_STRUCT;

#define DEFUSA_IOSF_PARITY_CNTRL_DISP_WIDTH 32
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_DEFAULT 0x00000000U
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_ENDPOINT DISP

#define DEFUSA_IOSF_PARITY_CNTRL_DISP_DEFUSA_IOSF_PARITY_CNTRL_LOCK_LSB 0x0000
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_DEFUSA_IOSF_PARITY_CNTRL_LOCK_SIZE 0x0001
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_PSF_0_DATA_PARITY_ERROR_INJ_LSB 0x0004
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_PSF_0_DATA_PARITY_ERROR_INJ_SIZE 0x0002
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_PSF_0_CMD_PARITY_ERR_INJ_LSB 0x0006
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_PSF_0_CMD_PARITY_ERR_INJ_SIZE 0x0001
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_PSF_0_DATA_PARITY_CHK_EN_LSB 0x0008
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_PSF_0_DATA_PARITY_CHK_EN_SIZE 0x0001
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_PSF_0_CMD_PARITY_CHK_EN_LSB 0x0009
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_PSF_0_CMD_PARITY_CHK_EN_SIZE 0x0001
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_PSF_0_DATA_PARITY_GEN_DIS_LSB 0x000c
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_PSF_0_DATA_PARITY_GEN_DIS_SIZE 0x0001
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_PSF_0_CMD_PARITY_GEN_DIS_LSB 0x000d
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_PSF_0_CMD_PARITY_GEN_DIS_SIZE 0x0001
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_PSF0_DIP_CMD_PARITY_ERR_INJ_LSB 0x0016
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_PSF0_DIP_CMD_PARITY_ERR_INJ_SIZE 0x0001
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_PSF1_DIP_CMD_PARITY_ERR_INJ_LSB 0x0017
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_PSF1_DIP_CMD_PARITY_ERR_INJ_SIZE 0x0001
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_PSF_DIP_DATA_PARITY_CHK_EN_LSB 0x0018
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_PSF_DIP_DATA_PARITY_CHK_EN_SIZE 0x0001
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_PSF_DIP_CMD_PARITY_CHK_EN_LSB 0x0019
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_PSF_DIP_CMD_PARITY_CHK_EN_SIZE 0x0001
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_PSF_DIP_CMD_PARITY_GEN_DIS_LSB 0x001d
#define DEFUSA_IOSF_PARITY_CNTRL_DISP_PSF_DIP_CMD_PARITY_GEN_DIS_SIZE 0x0001

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DEDICATED_PATH_ARB_CREDITS desc:
  */

#define DEDICATED_PATH_ARB_CREDITS_DISP_REG (0x00134050U)

typedef union {
  struct {
    UINT32 abox0_credits : 7; /**< ABOX0 CREDITS */
                            /* Bits[6:0], default=0x00000000*/
    UINT32 spare_1 : 1; /**< SPARE 1 */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 abox1_credits : 7; /**< ABOX1 CREDITS */
                            /* Bits[14:8], default=0x00000000*/
    UINT32 spare_0 : 17; /**< SPARE 0 */
                            /* Bits[31:15], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DEDICATED_PATH_ARB_CREDITS_DISP_STRUCT;

#define DEDICATED_PATH_ARB_CREDITS_DISP_WIDTH 32
#define DEDICATED_PATH_ARB_CREDITS_DISP_DEFAULT 0x00000000U
#define DEDICATED_PATH_ARB_CREDITS_DISP_ENDPOINT DISP

#define DEDICATED_PATH_ARB_CREDITS_DISP_ABOX0_CREDITS_LSB 0x0000
#define DEDICATED_PATH_ARB_CREDITS_DISP_ABOX0_CREDITS_SIZE 0x0007
#define DEDICATED_PATH_ARB_CREDITS_DISP_SPARE_1_LSB 0x0007
#define DEDICATED_PATH_ARB_CREDITS_DISP_SPARE_1_SIZE 0x0001
#define DEDICATED_PATH_ARB_CREDITS_DISP_ABOX1_CREDITS_LSB 0x0008
#define DEDICATED_PATH_ARB_CREDITS_DISP_ABOX1_CREDITS_SIZE 0x0007
#define DEDICATED_PATH_ARB_CREDITS_DISP_SPARE_0_LSB 0x000f
#define DEDICATED_PATH_ARB_CREDITS_DISP_SPARE_0_SIZE 0x0011

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DG_MC_CRD desc:
  */

#define DG_MC_CRD_DISP_REG               (0x00134060U)

typedef union {
  struct {
    UINT32 memory_controller_credits : 8; /**< MEMORY CONTROLLER CREDITS */
                            /* Bits[7:0], default=0x00000010*/
    UINT32 rsvd_0 : 24; /**< Undefined - auto filled rsvd_[31:08] */
                            /* Bits[31:8], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DG_MC_CRD_DISP_STRUCT;

#define DG_MC_CRD_DISP_WIDTH 32
#define DG_MC_CRD_DISP_DEFAULT 0x00000010U
#define DG_MC_CRD_DISP_ENDPOINT DISP

#define DG_MC_CRD_DISP_MEMORY_CONTROLLER_CREDITS_LSB 0x0000
#define DG_MC_CRD_DISP_MEMORY_CONTROLLER_CREDITS_SIZE 0x0008

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DE_FUSE_ERROR desc:
  */

#define DE_FUSE_ERROR_DISP_REG           (0x00134100U)

typedef union {
  struct {
    UINT32 rsvd_0 : 23; /**< Undefined - auto filled rsvd_[22:00] */
                            /* Bits[22:0], default=0*/
    UINT32 fuse_error_code : 8; /**< FUSE ERROR CODE */
                            /* Bits[30:23], default=0x00000000*/
    UINT32 fuse_error_valid : 1; /**< FUSE ERROR VALID */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DE_FUSE_ERROR_DISP_STRUCT;

#define DE_FUSE_ERROR_DISP_WIDTH 32
#define DE_FUSE_ERROR_DISP_DEFAULT 0x00000000U
#define DE_FUSE_ERROR_DISP_ENDPOINT DISP

#define DE_FUSE_ERROR_DISP_FUSE_ERROR_CODE_LSB 0x0017
#define DE_FUSE_ERROR_DISP_FUSE_ERROR_CODE_SIZE 0x0008
#define DE_FUSE_ERROR_DISP_FUSE_ERROR_VALID_LSB 0x001f
#define DE_FUSE_ERROR_DISP_FUSE_ERROR_VALID_SIZE 0x0001

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DG_FUSE_STATUS desc:
  */

#define DG_FUSE_STATUS_DISP_REG          (0x00134104U)

typedef union {
  struct {
    UINT32 display_fusa_enable : 1; /**< DISPLAY FUSA ENABLE */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 iosf_parity_disable : 1; /**< IOSF PARITY DISABLE */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 dni_parity_disable : 1; /**< DNI PARITY DISABLE */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 ecc_error_injection_enable : 1; /**< ECC ERROR INJECTION ENABLE */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 dg_spare_fuse : 4; /**< DG SPARE FUSE */
                            /* Bits[7:4], default=0x00000000*/
    UINT32 rsvd_0 : 24; /**< Undefined - auto filled rsvd_[31:08] */
                            /* Bits[31:8], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DG_FUSE_STATUS_DISP_STRUCT;

#define DG_FUSE_STATUS_DISP_WIDTH 32
#define DG_FUSE_STATUS_DISP_DEFAULT 0x00000000U
#define DG_FUSE_STATUS_DISP_ENDPOINT DISP

#define DG_FUSE_STATUS_DISP_DISPLAY_FUSA_ENABLE_LSB 0x0000
#define DG_FUSE_STATUS_DISP_DISPLAY_FUSA_ENABLE_SIZE 0x0001
#define DG_FUSE_STATUS_DISP_IOSF_PARITY_DISABLE_LSB 0x0001
#define DG_FUSE_STATUS_DISP_IOSF_PARITY_DISABLE_SIZE 0x0001
#define DG_FUSE_STATUS_DISP_DNI_PARITY_DISABLE_LSB 0x0002
#define DG_FUSE_STATUS_DISP_DNI_PARITY_DISABLE_SIZE 0x0001
#define DG_FUSE_STATUS_DISP_ECC_ERROR_INJECTION_ENABLE_LSB 0x0003
#define DG_FUSE_STATUS_DISP_ECC_ERROR_INJECTION_ENABLE_SIZE 0x0001
#define DG_FUSE_STATUS_DISP_DG_SPARE_FUSE_LSB 0x0004
#define DG_FUSE_STATUS_DISP_DG_SPARE_FUSE_SIZE 0x0004

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DG_CNTL_PROTECTED desc:
  */

#define DG_CNTL_PROTECTED_DISP_REG       (0x00134110U)

typedef union {
  struct {
    UINT32 lp_nonsnoop : 1; /**< LP NONSNOOP */
                            /* Bits[0:0], default=0x00000001*/
    UINT32 vtd_fault_fix_disable : 1; /**< VTD FAULT FIX DISABLE */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 spare2 : 1; /**< SPARE2 */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 sai_check_disable_on_flr_ack : 1; /**< SAI CHECK DISABLE ON FLR ACK */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 disable_wait_for_ats_invalidation_done_from_dsb : 1; /**< DISABLE WAIT FOR ATS INVALIDATION DONE FROM DSB */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 disable_dsb_invalidation : 1; /**< DISABLE DSB INVALIDATION */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 ccs_hashing_performance_update_disable : 1; /**< CCS HASHING PERFORMANCE UPDATE DISABLE */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 fusa_and_interrupt_memup_override : 1; /**< FUSA AND INTERRUPT MEMUP OVERRIDE */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 hdcp_iaptk_flow_delay : 5; /**< HDCP IAPTK FLOW DELAY */
                            /* Bits[12:8], default=0x00000008*/
    UINT32 hdcp_iaptk_flow_delay_dis : 1; /**< HDCP IAPTK FLOW DELAY DIS */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 dgs_wake_dc5_check_en : 1; /**< DGS WAKE DC5 CHECK EN */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 pm_sai_check_disable : 1; /**< PM SAI CHECK DISABLE */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 kvmr_ravdm_enable : 1; /**< KVMR RAVDM ENABLE */
                            /* Bits[16:16], default=0x00000001*/
    UINT32 iaptk_disable : 1; /**< IAPTK DISABLE */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 spare18 : 1; /**< SPARE18 */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 spare19 : 1; /**< SPARE19 */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 spare20 : 1; /**< SPARE20 */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 spare21 : 1; /**< SPARE21 */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 spare22 : 1; /**< SPARE22 */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 spare23 : 1; /**< SPARE23 */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 spare24 : 1; /**< SPARE24 */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 spare25 : 1; /**< SPARE25 */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 spare26 : 1; /**< SPARE26 */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 spare27 : 1; /**< SPARE27 */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 spare28 : 1; /**< SPARE28 */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 spare29 : 1; /**< SPARE29 */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 spare30 : 1; /**< SPARE30 */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 spare31 : 1; /**< SPARE31 */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DG_CNTL_PROTECTED_DISP_STRUCT;

#define DG_CNTL_PROTECTED_DISP_WIDTH 32
#define DG_CNTL_PROTECTED_DISP_DEFAULT 0x00010801U
#define DG_CNTL_PROTECTED_DISP_ENDPOINT DISP

#define DG_CNTL_PROTECTED_DISP_LP_NONSNOOP_LSB 0x0000
#define DG_CNTL_PROTECTED_DISP_LP_NONSNOOP_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_VTD_FAULT_FIX_DISABLE_LSB 0x0001
#define DG_CNTL_PROTECTED_DISP_VTD_FAULT_FIX_DISABLE_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_SPARE2_LSB 0x0002
#define DG_CNTL_PROTECTED_DISP_SPARE2_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_SAI_CHECK_DISABLE_ON_FLR_ACK_LSB 0x0003
#define DG_CNTL_PROTECTED_DISP_SAI_CHECK_DISABLE_ON_FLR_ACK_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_DISABLE_WAIT_FOR_ATS_INVALIDATION_DONE_FROM_DSB_LSB 0x0004
#define DG_CNTL_PROTECTED_DISP_DISABLE_WAIT_FOR_ATS_INVALIDATION_DONE_FROM_DSB_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_DISABLE_DSB_INVALIDATION_LSB 0x0005
#define DG_CNTL_PROTECTED_DISP_DISABLE_DSB_INVALIDATION_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_CCS_HASHING_PERFORMANCE_UPDATE_DISABLE_LSB 0x0006
#define DG_CNTL_PROTECTED_DISP_CCS_HASHING_PERFORMANCE_UPDATE_DISABLE_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_FUSA_AND_INTERRUPT_MEMUP_OVERRIDE_LSB 0x0007
#define DG_CNTL_PROTECTED_DISP_FUSA_AND_INTERRUPT_MEMUP_OVERRIDE_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_HDCP_IAPTK_FLOW_DELAY_LSB 0x0008
#define DG_CNTL_PROTECTED_DISP_HDCP_IAPTK_FLOW_DELAY_SIZE 0x0005
#define DG_CNTL_PROTECTED_DISP_HDCP_IAPTK_FLOW_DELAY_DIS_LSB 0x000d
#define DG_CNTL_PROTECTED_DISP_HDCP_IAPTK_FLOW_DELAY_DIS_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_DGS_WAKE_DC5_CHECK_EN_LSB 0x000e
#define DG_CNTL_PROTECTED_DISP_DGS_WAKE_DC5_CHECK_EN_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_PM_SAI_CHECK_DISABLE_LSB 0x000f
#define DG_CNTL_PROTECTED_DISP_PM_SAI_CHECK_DISABLE_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_KVMR_RAVDM_ENABLE_LSB 0x0010
#define DG_CNTL_PROTECTED_DISP_KVMR_RAVDM_ENABLE_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_IAPTK_DISABLE_LSB 0x0011
#define DG_CNTL_PROTECTED_DISP_IAPTK_DISABLE_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_SPARE18_LSB 0x0012
#define DG_CNTL_PROTECTED_DISP_SPARE18_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_SPARE19_LSB 0x0013
#define DG_CNTL_PROTECTED_DISP_SPARE19_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_SPARE20_LSB 0x0014
#define DG_CNTL_PROTECTED_DISP_SPARE20_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_SPARE21_LSB 0x0015
#define DG_CNTL_PROTECTED_DISP_SPARE21_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_SPARE22_LSB 0x0016
#define DG_CNTL_PROTECTED_DISP_SPARE22_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_SPARE23_LSB 0x0017
#define DG_CNTL_PROTECTED_DISP_SPARE23_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_SPARE24_LSB 0x0018
#define DG_CNTL_PROTECTED_DISP_SPARE24_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_SPARE25_LSB 0x0019
#define DG_CNTL_PROTECTED_DISP_SPARE25_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_SPARE26_LSB 0x001a
#define DG_CNTL_PROTECTED_DISP_SPARE26_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_SPARE27_LSB 0x001b
#define DG_CNTL_PROTECTED_DISP_SPARE27_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_SPARE28_LSB 0x001c
#define DG_CNTL_PROTECTED_DISP_SPARE28_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_SPARE29_LSB 0x001d
#define DG_CNTL_PROTECTED_DISP_SPARE29_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_SPARE30_LSB 0x001e
#define DG_CNTL_PROTECTED_DISP_SPARE30_SIZE 0x0001
#define DG_CNTL_PROTECTED_DISP_SPARE31_LSB 0x001f
#define DG_CNTL_PROTECTED_DISP_SPARE31_SIZE 0x0001

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DG_CNTL desc:
  */

#define DG_CNTL_DISP_REG                 (0x00134114U)

typedef union {
  struct {
    UINT32 aboxportmapdis : 1; /**< ABOXPORTMAPDIS */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 ipcovrddis : 1; /**< IPCOVRDDIS */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 spare2 : 1; /**< SPARE2 */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 spare3 : 1; /**< SPARE3 */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 spare : 28; /**< SPARE */
                            /* Bits[31:4], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DG_CNTL_DISP_STRUCT;

#define DG_CNTL_DISP_WIDTH 32
#define DG_CNTL_DISP_DEFAULT 0x00000000U
#define DG_CNTL_DISP_ENDPOINT DISP

#define DG_CNTL_DISP_ABOXPORTMAPDIS_LSB 0x0000
#define DG_CNTL_DISP_ABOXPORTMAPDIS_SIZE 0x0001
#define DG_CNTL_DISP_IPCOVRDDIS_LSB 0x0001
#define DG_CNTL_DISP_IPCOVRDDIS_SIZE 0x0001
#define DG_CNTL_DISP_SPARE2_LSB 0x0002
#define DG_CNTL_DISP_SPARE2_SIZE 0x0001
#define DG_CNTL_DISP_SPARE3_LSB 0x0003
#define DG_CNTL_DISP_SPARE3_SIZE 0x0001
#define DG_CNTL_DISP_SPARE_LSB 0x0004
#define DG_CNTL_DISP_SPARE_SIZE 0x001c

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DG_CNTL2 desc:
  */

#define DG_CNTL2_DISP_REG                (0x00134118U)

typedef union {
  struct {
    UINT32 rsvd_0 : 16; /**< Undefined - auto filled rsvd_[15:00] */
                            /* Bits[15:0], default=0*/
    UINT32 spare1 : 3; /**< SPARE1 */
                            /* Bits[18:16], default=0x00000000*/
    UINT32 spare2 : 5; /**< SPARE2 */
                            /* Bits[23:19], default=0x00000000*/
    UINT32 spare3 : 8; /**< SPARE3 */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DG_CNTL2_DISP_STRUCT;

#define DG_CNTL2_DISP_WIDTH 32
#define DG_CNTL2_DISP_DEFAULT 0x00000000U
#define DG_CNTL2_DISP_ENDPOINT DISP

#define DG_CNTL2_DISP_SPARE1_LSB 0x0010
#define DG_CNTL2_DISP_SPARE1_SIZE 0x0003
#define DG_CNTL2_DISP_SPARE2_LSB 0x0013
#define DG_CNTL2_DISP_SPARE2_SIZE 0x0005
#define DG_CNTL2_DISP_SPARE3_LSB 0x0018
#define DG_CNTL2_DISP_SPARE3_SIZE 0x0008

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DG_DEBUG desc:
  */

#define DG_DEBUG_DISP_REG                (0x00134120U)

typedef union {
  struct {
    UINT32 dgp_upstream_abort : 1; /**< DGP UPSTREAM ABORT */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 dgdp_upstream_abort0 : 1; /**< DGDP UPSTREAM ABORT0 */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 dgdp_upstream_abort1 : 1; /**< DGDP UPSTREAM ABORT1 */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 spare1 : 5; /**< SPARE1 */
                            /* Bits[7:3], default=0x00000000*/
    UINT32 flr_state : 4; /**< FLR STATE */
                            /* Bits[11:8], default=0x00000000*/
    UINT32 spare : 20; /**< SPARE */
                            /* Bits[31:12], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DG_DEBUG_DISP_STRUCT;

#define DG_DEBUG_DISP_WIDTH 32
#define DG_DEBUG_DISP_DEFAULT 0x00000000U
#define DG_DEBUG_DISP_ENDPOINT DISP

#define DG_DEBUG_DISP_DGP_UPSTREAM_ABORT_LSB 0x0000
#define DG_DEBUG_DISP_DGP_UPSTREAM_ABORT_SIZE 0x0001
#define DG_DEBUG_DISP_DGDP_UPSTREAM_ABORT0_LSB 0x0001
#define DG_DEBUG_DISP_DGDP_UPSTREAM_ABORT0_SIZE 0x0001
#define DG_DEBUG_DISP_DGDP_UPSTREAM_ABORT1_LSB 0x0002
#define DG_DEBUG_DISP_DGDP_UPSTREAM_ABORT1_SIZE 0x0001
#define DG_DEBUG_DISP_SPARE1_LSB 0x0003
#define DG_DEBUG_DISP_SPARE1_SIZE 0x0005
#define DG_DEBUG_DISP_FLR_STATE_LSB 0x0008
#define DG_DEBUG_DISP_FLR_STATE_SIZE 0x0004
#define DG_DEBUG_DISP_SPARE_LSB 0x000c
#define DG_DEBUG_DISP_SPARE_SIZE 0x0014

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DG_ECO desc:
  */

#define DG_ECO_DISP_REG                  (0x00134124U)

typedef union {
  struct {
    UINT32 spare : 32; /**< SPARE */
                            /* Bits[31:0], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DG_ECO_DISP_STRUCT;

#define DG_ECO_DISP_WIDTH 32
#define DG_ECO_DISP_DEFAULT 0x00000000U
#define DG_ECO_DISP_ENDPOINT DISP

#define DG_ECO_DISP_SPARE_LSB 0x0000
#define DG_ECO_DISP_SPARE_SIZE 0x0020

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DG_CLOCK_GATE_DIS desc:
  */

#define DG_CLOCK_GATE_DIS_DISP_REG       (0x00134130U)

typedef union {
  struct {
    UINT32 fclk_idle_timer : 4; /**< FCLK IDLE TIMER */
                            /* Bits[3:0], default=0x00000000*/
    UINT32 fdpclk_idle_timer : 4; /**< FDPCLK IDLE TIMER */
                            /* Bits[7:4], default=0x00000000*/
    UINT32 dgc_dis : 1; /**< DGC DIS */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 dgp_dis : 1; /**< DGP DIS */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 dgr_dis : 1; /**< DGR DIS */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 dgs_dis : 1; /**< DGS DIS */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 dgdp_dis : 1; /**< DGDP DIS */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 rsvd_0 : 19; /**< Undefined - auto filled rsvd_[31:13] */
                            /* Bits[31:13], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DG_CLOCK_GATE_DIS_DISP_STRUCT;

#define DG_CLOCK_GATE_DIS_DISP_WIDTH 32
#define DG_CLOCK_GATE_DIS_DISP_DEFAULT 0x00000000U
#define DG_CLOCK_GATE_DIS_DISP_ENDPOINT DISP

#define DG_CLOCK_GATE_DIS_DISP_FCLK_IDLE_TIMER_LSB 0x0000
#define DG_CLOCK_GATE_DIS_DISP_FCLK_IDLE_TIMER_SIZE 0x0004
#define DG_CLOCK_GATE_DIS_DISP_FDPCLK_IDLE_TIMER_LSB 0x0004
#define DG_CLOCK_GATE_DIS_DISP_FDPCLK_IDLE_TIMER_SIZE 0x0004
#define DG_CLOCK_GATE_DIS_DISP_DGC_DIS_LSB 0x0008
#define DG_CLOCK_GATE_DIS_DISP_DGC_DIS_SIZE 0x0001
#define DG_CLOCK_GATE_DIS_DISP_DGP_DIS_LSB 0x0009
#define DG_CLOCK_GATE_DIS_DISP_DGP_DIS_SIZE 0x0001
#define DG_CLOCK_GATE_DIS_DISP_DGR_DIS_LSB 0x000a
#define DG_CLOCK_GATE_DIS_DISP_DGR_DIS_SIZE 0x0001
#define DG_CLOCK_GATE_DIS_DISP_DGS_DIS_LSB 0x000b
#define DG_CLOCK_GATE_DIS_DISP_DGS_DIS_SIZE 0x0001
#define DG_CLOCK_GATE_DIS_DISP_DGDP_DIS_LSB 0x000c
#define DG_CLOCK_GATE_DIS_DISP_DGDP_DIS_SIZE 0x0001

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DE_CLKREQ_POLICY desc:
  */

#define DE_CLKREQ_POLICY_DISP_REG        (0x00134134U)

typedef union {
  struct {
    UINT32 clkreq_ovrd : 1; /**< CLKREQ OVRD */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 mem_up_ovrd : 1; /**< MEM UP OVRD */
                            /* Bits[1:1], default=0x00000001*/
    UINT32 spare1 : 14; /**< SPARE1 */
                            /* Bits[15:2], default=0x00000000*/
    UINT32 clkreq_hyst_cntr : 8; /**< CLKREQ HYST CNTR */
                            /* Bits[23:16], default=0x00000004*/
    UINT32 spare2 : 8; /**< SPARE2 */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DE_CLKREQ_POLICY_DISP_STRUCT;

#define DE_CLKREQ_POLICY_DISP_WIDTH 32
#define DE_CLKREQ_POLICY_DISP_DEFAULT 0x00040002U
#define DE_CLKREQ_POLICY_DISP_ENDPOINT DISP

#define DE_CLKREQ_POLICY_DISP_CLKREQ_OVRD_LSB 0x0000
#define DE_CLKREQ_POLICY_DISP_CLKREQ_OVRD_SIZE 0x0001
#define DE_CLKREQ_POLICY_DISP_MEM_UP_OVRD_LSB 0x0001
#define DE_CLKREQ_POLICY_DISP_MEM_UP_OVRD_SIZE 0x0001
#define DE_CLKREQ_POLICY_DISP_SPARE1_LSB 0x0002
#define DE_CLKREQ_POLICY_DISP_SPARE1_SIZE 0x000e
#define DE_CLKREQ_POLICY_DISP_CLKREQ_HYST_CNTR_LSB 0x0010
#define DE_CLKREQ_POLICY_DISP_CLKREQ_HYST_CNTR_SIZE 0x0008
#define DE_CLKREQ_POLICY_DISP_SPARE2_LSB 0x0018
#define DE_CLKREQ_POLICY_DISP_SPARE2_SIZE 0x0008

#endif      // SV_HOOKS


/** VDMBDFBARKVM desc:
  */

#define VDMBDFBARKVM_DISP_REG            (0x00134140U)

typedef union {
  struct {
    UINT32 funnum : 3; /**< FUNNUM */
                            /* Bits[2:0], default=0x00000000*/
    UINT32 devnum : 5; /**< DEVNUM */
                            /* Bits[7:3], default=0x00000016*/
    UINT32 busnum : 8; /**< BUSNUM */
                            /* Bits[15:8], default=0x00000000*/
    UINT32 barnum : 3; /**< BARNUM */
                            /* Bits[18:16], default=0x00000007*/
    UINT32 rsvd_0 : 13; /**< Undefined - auto filled rsvd_[31:19] */
                            /* Bits[31:19], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} VDMBDFBARKVM_DISP_STRUCT;

#define VDMBDFBARKVM_DISP_WIDTH 32
#define VDMBDFBARKVM_DISP_DEFAULT 0x000700b0U
#define VDMBDFBARKVM_DISP_ENDPOINT DISP

#define VDMBDFBARKVM_DISP_FUNNUM_LSB 0x0000
#define VDMBDFBARKVM_DISP_FUNNUM_SIZE 0x0003
#define VDMBDFBARKVM_DISP_DEVNUM_LSB 0x0003
#define VDMBDFBARKVM_DISP_DEVNUM_SIZE 0x0005
#define VDMBDFBARKVM_DISP_BUSNUM_LSB 0x0008
#define VDMBDFBARKVM_DISP_BUSNUM_SIZE 0x0008
#define VDMBDFBARKVM_DISP_BARNUM_LSB 0x0010
#define VDMBDFBARKVM_DISP_BARNUM_SIZE 0x0003


/** DE_BDF desc:
  */

#define DE_BDF_DISP_REG                  (0x0013414cU)

typedef union {
  struct {
    UINT32 rsvd_0 : 16; /**< Undefined - auto filled rsvd_[15:00] */
                            /* Bits[15:0], default=0*/
    UINT32 function : 3; /**< FUNCTION */
                            /* Bits[18:16], default=0x00000000*/
    UINT32 device : 5; /**< DEVICE */
                            /* Bits[23:19], default=0x00000002*/
    UINT32 bus : 8; /**< BUS */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DE_BDF_DISP_STRUCT;

#define DE_BDF_DISP_WIDTH 32
#define DE_BDF_DISP_DEFAULT 0x00100000U
#define DE_BDF_DISP_ENDPOINT DISP

#define DE_BDF_DISP_FUNCTION_LSB 0x0010
#define DE_BDF_DISP_FUNCTION_SIZE 0x0003
#define DE_BDF_DISP_DEVICE_LSB 0x0013
#define DE_BDF_DISP_DEVICE_SIZE 0x0005
#define DE_BDF_DISP_BUS_LSB 0x0018
#define DE_BDF_DISP_BUS_SIZE 0x0008


#ifdef SV_HOOKS

/** DE_SOUTH_RST_STS desc:
  */

#define DE_SOUTH_RST_STS_DISP_REG        (0x00134184U)

typedef union {
  struct {
    UINT32 pch_reset_done : 1; /**< PCH RESET DONE */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 7; /**< Undefined - auto filled rsvd_[07:01] */
                            /* Bits[7:1], default=0*/
    UINT32 pica_reset_done : 1; /**< PICA RESET DONE */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 rsvd_1 : 23; /**< Undefined - auto filled rsvd_[31:09] */
                            /* Bits[31:9], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DE_SOUTH_RST_STS_DISP_STRUCT;

#define DE_SOUTH_RST_STS_DISP_WIDTH 32
#define DE_SOUTH_RST_STS_DISP_DEFAULT 0x00000000U
#define DE_SOUTH_RST_STS_DISP_ENDPOINT DISP

#define DE_SOUTH_RST_STS_DISP_PCH_RESET_DONE_LSB 0x0000
#define DE_SOUTH_RST_STS_DISP_PCH_RESET_DONE_SIZE 0x0001
#define DE_SOUTH_RST_STS_DISP_PICA_RESET_DONE_LSB 0x0008
#define DE_SOUTH_RST_STS_DISP_PICA_RESET_DONE_SIZE 0x0001

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** CP_DEBIOSEARLY_LSB desc:
  */

#define CP_DEBIOSEARLY_LSB_DISP_REG      (0x00134378U)

typedef union {
  struct {
    UINT32 cp_debiosearly_lsb : 32;
                            /* Bits[31:0], default=0x01100218*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CP_DEBIOSEARLY_LSB_DISP_STRUCT;

#define CP_DEBIOSEARLY_LSB_DISP_WIDTH 32
#define CP_DEBIOSEARLY_LSB_DISP_DEFAULT 0x01100218U
#define CP_DEBIOSEARLY_LSB_DISP_ENDPOINT DISP

#define CP_DEBIOSEARLY_LSB_DISP_CP_DEBIOSEARLY_LSB_LSB 0x0000
#define CP_DEBIOSEARLY_LSB_DISP_CP_DEBIOSEARLY_LSB_SIZE 0x0020

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** CP_DEBIOSEARLY_MSB desc:
  */

#define CP_DEBIOSEARLY_MSB_DISP_REG      (0x0013437cU)

typedef union {
  struct {
    UINT32 cp_debiosearly_msb : 32;
                            /* Bits[31:0], default=0x00000400*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CP_DEBIOSEARLY_MSB_DISP_STRUCT;

#define CP_DEBIOSEARLY_MSB_DISP_WIDTH 32
#define CP_DEBIOSEARLY_MSB_DISP_DEFAULT 0x00000400U
#define CP_DEBIOSEARLY_MSB_DISP_ENDPOINT DISP

#define CP_DEBIOSEARLY_MSB_DISP_CP_DEBIOSEARLY_MSB_LSB 0x0000
#define CP_DEBIOSEARLY_MSB_DISP_CP_DEBIOSEARLY_MSB_SIZE 0x0020

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** RAC_DEBIOSEARLY_LSB desc:
  */

#define RAC_DEBIOSEARLY_LSB_DISP_REG     (0x00134380U)

typedef union {
  struct {
    UINT32 rac_debiosearly_lsb : 32;
                            /* Bits[31:0], default=0xFFFFFFFF*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RAC_DEBIOSEARLY_LSB_DISP_STRUCT;

#define RAC_DEBIOSEARLY_LSB_DISP_WIDTH 32
#define RAC_DEBIOSEARLY_LSB_DISP_DEFAULT 0xffffffffU
#define RAC_DEBIOSEARLY_LSB_DISP_ENDPOINT DISP

#define RAC_DEBIOSEARLY_LSB_DISP_RAC_DEBIOSEARLY_LSB_LSB 0x0000
#define RAC_DEBIOSEARLY_LSB_DISP_RAC_DEBIOSEARLY_LSB_SIZE 0x0020

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** RAC_DEBIOSEARLY_MSB desc:
  */

#define RAC_DEBIOSEARLY_MSB_DISP_REG     (0x00134384U)

typedef union {
  struct {
    UINT32 rac_debiosearly_msb : 32;
                            /* Bits[31:0], default=0xFFFFFFFF*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RAC_DEBIOSEARLY_MSB_DISP_STRUCT;

#define RAC_DEBIOSEARLY_MSB_DISP_WIDTH 32
#define RAC_DEBIOSEARLY_MSB_DISP_DEFAULT 0xffffffffU
#define RAC_DEBIOSEARLY_MSB_DISP_ENDPOINT DISP

#define RAC_DEBIOSEARLY_MSB_DISP_RAC_DEBIOSEARLY_MSB_LSB 0x0000
#define RAC_DEBIOSEARLY_MSB_DISP_RAC_DEBIOSEARLY_MSB_SIZE 0x0020

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** WAC_DEBIOSEARLY_LSB desc:
  */

#define WAC_DEBIOSEARLY_LSB_DISP_REG     (0x00134388U)

typedef union {
  struct {
    UINT32 wac_debiosearly_lsb : 32;
                            /* Bits[31:0], default=0x0110021A*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} WAC_DEBIOSEARLY_LSB_DISP_STRUCT;

#define WAC_DEBIOSEARLY_LSB_DISP_WIDTH 32
#define WAC_DEBIOSEARLY_LSB_DISP_DEFAULT 0x0110021aU
#define WAC_DEBIOSEARLY_LSB_DISP_ENDPOINT DISP

#define WAC_DEBIOSEARLY_LSB_DISP_WAC_DEBIOSEARLY_LSB_LSB 0x0000
#define WAC_DEBIOSEARLY_LSB_DISP_WAC_DEBIOSEARLY_LSB_SIZE 0x0020

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** WAC_DEBIOSEARLY_MSB desc:
  */

#define WAC_DEBIOSEARLY_MSB_DISP_REG     (0x0013438cU)

typedef union {
  struct {
    UINT32 wac_debiosearly_msb : 32;
                            /* Bits[31:0], default=0x00000400*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} WAC_DEBIOSEARLY_MSB_DISP_STRUCT;

#define WAC_DEBIOSEARLY_MSB_DISP_WIDTH 32
#define WAC_DEBIOSEARLY_MSB_DISP_DEFAULT 0x00000400U
#define WAC_DEBIOSEARLY_MSB_DISP_ENDPOINT DISP

#define WAC_DEBIOSEARLY_MSB_DISP_WAC_DEBIOSEARLY_MSB_LSB 0x0000
#define WAC_DEBIOSEARLY_MSB_DISP_WAC_DEBIOSEARLY_MSB_SIZE 0x0020

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** CP_DEBIOS_LSB desc:
  */

#define CP_DEBIOS_LSB_DISP_REG           (0x00134390U)

typedef union {
  struct {
    UINT32 cp_debios_lsb : 32;
                            /* Bits[31:0], default=0x01100208*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CP_DEBIOS_LSB_DISP_STRUCT;

#define CP_DEBIOS_LSB_DISP_WIDTH 32
#define CP_DEBIOS_LSB_DISP_DEFAULT 0x01100208U
#define CP_DEBIOS_LSB_DISP_ENDPOINT DISP

#define CP_DEBIOS_LSB_DISP_CP_DEBIOS_LSB_LSB 0x0000
#define CP_DEBIOS_LSB_DISP_CP_DEBIOS_LSB_SIZE 0x0020

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** CP_DEBIOS_MSB desc:
  */

#define CP_DEBIOS_MSB_DISP_REG           (0x00134394U)

typedef union {
  struct {
    UINT32 cp_debios_msb : 32;
                            /* Bits[31:0], default=0x00000400*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CP_DEBIOS_MSB_DISP_STRUCT;

#define CP_DEBIOS_MSB_DISP_WIDTH 32
#define CP_DEBIOS_MSB_DISP_DEFAULT 0x00000400U
#define CP_DEBIOS_MSB_DISP_ENDPOINT DISP

#define CP_DEBIOS_MSB_DISP_CP_DEBIOS_MSB_LSB 0x0000
#define CP_DEBIOS_MSB_DISP_CP_DEBIOS_MSB_SIZE 0x0020

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** RAC_DEBIOS_LSB desc:
  */

#define RAC_DEBIOS_LSB_DISP_REG          (0x00134398U)

typedef union {
  struct {
    UINT32 rac_debios_lsb : 32;
                            /* Bits[31:0], default=0xFFFFFFFF*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RAC_DEBIOS_LSB_DISP_STRUCT;

#define RAC_DEBIOS_LSB_DISP_WIDTH 32
#define RAC_DEBIOS_LSB_DISP_DEFAULT 0xffffffffU
#define RAC_DEBIOS_LSB_DISP_ENDPOINT DISP

#define RAC_DEBIOS_LSB_DISP_RAC_DEBIOS_LSB_LSB 0x0000
#define RAC_DEBIOS_LSB_DISP_RAC_DEBIOS_LSB_SIZE 0x0020

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** RAC_DEBIOS_MSB desc:
  */

#define RAC_DEBIOS_MSB_DISP_REG          (0x0013439cU)

typedef union {
  struct {
    UINT32 rac_debios_msb : 32;
                            /* Bits[31:0], default=0xFFFFFFFF*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RAC_DEBIOS_MSB_DISP_STRUCT;

#define RAC_DEBIOS_MSB_DISP_WIDTH 32
#define RAC_DEBIOS_MSB_DISP_DEFAULT 0xffffffffU
#define RAC_DEBIOS_MSB_DISP_ENDPOINT DISP

#define RAC_DEBIOS_MSB_DISP_RAC_DEBIOS_MSB_LSB 0x0000
#define RAC_DEBIOS_MSB_DISP_RAC_DEBIOS_MSB_SIZE 0x0020

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** WAC_DEBIOS_LSB desc:
  */

#define WAC_DEBIOS_LSB_DISP_REG          (0x001343a0U)

typedef union {
  struct {
    UINT32 wac_debios_lsb : 32;
                            /* Bits[31:0], default=0x0110021A*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} WAC_DEBIOS_LSB_DISP_STRUCT;

#define WAC_DEBIOS_LSB_DISP_WIDTH 32
#define WAC_DEBIOS_LSB_DISP_DEFAULT 0x0110021aU
#define WAC_DEBIOS_LSB_DISP_ENDPOINT DISP

#define WAC_DEBIOS_LSB_DISP_WAC_DEBIOS_LSB_LSB 0x0000
#define WAC_DEBIOS_LSB_DISP_WAC_DEBIOS_LSB_SIZE 0x0020

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** WAC_DEBIOS_MSB desc:
  */

#define WAC_DEBIOS_MSB_DISP_REG          (0x001343a4U)

typedef union {
  struct {
    UINT32 wac_debios_msb : 32;
                            /* Bits[31:0], default=0x00000400*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} WAC_DEBIOS_MSB_DISP_STRUCT;

#define WAC_DEBIOS_MSB_DISP_WIDTH 32
#define WAC_DEBIOS_MSB_DISP_DEFAULT 0x00000400U
#define WAC_DEBIOS_MSB_DISP_ENDPOINT DISP

#define WAC_DEBIOS_MSB_DISP_WAC_DEBIOS_MSB_LSB 0x0000
#define WAC_DEBIOS_MSB_DISP_WAC_DEBIOS_MSB_SIZE 0x0020

#endif      // SV_HOOKS


/** DE_GGC desc:
  */

#define DE_GGC_DISP_REG                  (0x00134400U)

typedef union {
  struct {
    UINT32 spare : 1; /**< SPARE */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 5; /**< Undefined - auto filled rsvd_[05:01] */
                            /* Bits[5:1], default=0*/
    UINT32 ggms : 2; /**< GGMS */
                            /* Bits[7:6], default=0x00000003*/
    UINT32 gms : 8; /**< GMS */
                            /* Bits[15:8], default=0x00000005*/
    UINT32 rsvd_1 : 16; /**< Undefined - auto filled rsvd_[31:16] */
                            /* Bits[31:16], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DE_GGC_DISP_STRUCT;

#define DE_GGC_DISP_WIDTH 32
#define DE_GGC_DISP_DEFAULT 0x000005c0U
#define DE_GGC_DISP_ENDPOINT DISP

#define DE_GGC_DISP_SPARE_LSB 0x0000
#define DE_GGC_DISP_SPARE_SIZE 0x0001
#define DE_GGC_DISP_GGMS_LSB 0x0006
#define DE_GGC_DISP_GGMS_SIZE 0x0002
#define DE_GGC_DISP_GMS_LSB 0x0008
#define DE_GGC_DISP_GMS_SIZE 0x0008



/** DEDSMBASE_0 desc:
  */

#define DEDSMBASE_0_DISP_REG             (0x00134410U)

typedef union {
  struct {
    UINT32 spare : 1; /**< SPARE */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 19; /**< Undefined - auto filled rsvd_[19:01] */
                            /* Bits[19:1], default=0*/
    UINT32 bdsm : 12; /**< BDSM */
                            /* Bits[31:20], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DEDSMBASE_0_DISP_STRUCT;

#define DEDSMBASE_0_DISP_WIDTH 32
#define DEDSMBASE_0_DISP_DEFAULT 0x00000000U
#define DEDSMBASE_0_DISP_ENDPOINT DISP

#define DEDSMBASE_0_DISP_SPARE_LSB 0x0000
#define DEDSMBASE_0_DISP_SPARE_SIZE 0x0001
#define DEDSMBASE_0_DISP_BDSM_LSB 0x0014
#define DEDSMBASE_0_DISP_BDSM_SIZE 0x000c


#ifdef SV_HOOKS

/** DEDSMBASE_1 desc:
  */

#define DEDSMBASE_1_DISP_REG             (0x00134414U)

typedef union {
  struct {
    UINT32 bdsm : 32; /**< BDSM */
                            /* Bits[31:0], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DEDSMBASE_1_DISP_STRUCT;

#define DEDSMBASE_1_DISP_WIDTH 32
#define DEDSMBASE_1_DISP_DEFAULT 0x00000000U
#define DEDSMBASE_1_DISP_ENDPOINT DISP

#define DEDSMBASE_1_DISP_BDSM_LSB 0x0000
#define DEDSMBASE_1_DISP_BDSM_SIZE 0x0020

#endif      // SV_HOOKS


/** DEGSMBASE_0 desc:
  */

#define DEGSMBASE_0_DISP_REG             (0x00134420U)

typedef union {
  struct {
    UINT32 spare : 1; /**< SPARE */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 19; /**< Undefined - auto filled rsvd_[19:01] */
                            /* Bits[19:1], default=0*/
    UINT32 bgsm_lsb : 12; /**< BGSM LSB */
                            /* Bits[31:20], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DEGSMBASE_0_DISP_STRUCT;

#define DEGSMBASE_0_DISP_WIDTH 32
#define DEGSMBASE_0_DISP_DEFAULT 0x00000000U
#define DEGSMBASE_0_DISP_ENDPOINT DISP

#define DEGSMBASE_0_DISP_SPARE_LSB 0x0000
#define DEGSMBASE_0_DISP_SPARE_SIZE 0x0001
#define DEGSMBASE_0_DISP_BGSM_LSB_LSB 0x0014
#define DEGSMBASE_0_DISP_BGSM_LSB_SIZE 0x000c


#ifdef SV_HOOKS

/** DEGSMBASE_1 desc:
  */

#define DEGSMBASE_1_DISP_REG             (0x00134424U)

typedef union {
  struct {
    UINT32 bgsm_msb : 32; /**< BGSM MSB */
                            /* Bits[31:0], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DEGSMBASE_1_DISP_STRUCT;

#define DEGSMBASE_1_DISP_WIDTH 32
#define DEGSMBASE_1_DISP_DEFAULT 0x00000000U
#define DEGSMBASE_1_DISP_ENDPOINT DISP

#define DEGSMBASE_1_DISP_BGSM_MSB_LSB 0x0000
#define DEGSMBASE_1_DISP_BGSM_MSB_SIZE 0x0020

#endif      // SV_HOOKS

/** DEPAVPC_REG_0 desc:
  */

#define DEPAVPC_REG_0_DISP_REG           (0x00134430U)

typedef union {
  struct {
    UINT32 pcm_enable : 1; /**< PCM ENABLE */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 pavp_enable : 1; /**< PAVP ENABLE */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 lock : 1; /**< LOCK */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 heavy_mode_select : 1; /**< HEAVY MODE SELECT */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 override_terminate_attack : 1; /**< OVERRIDE TERMINATE ATTACK */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 reservd : 1; /**< RESERVD */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 asmf_method_enable : 1; /**< ASMF METHOD ENABLE */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 rsvd_0 : 13; /**< Undefined - auto filled rsvd_[19:07] */
                            /* Bits[19:7], default=0*/
    UINT32 wopcmbase : 12; /**< WOPCMBASE */
                            /* Bits[31:20], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DEPAVPC_REG_0_DISP_STRUCT;

#define DEPAVPC_REG_0_DISP_WIDTH 32
#define DEPAVPC_REG_0_DISP_DEFAULT 0x00000000U
#define DEPAVPC_REG_0_DISP_ENDPOINT DISP

#define DEPAVPC_REG_0_DISP_PCM_ENABLE_LSB 0x0000
#define DEPAVPC_REG_0_DISP_PCM_ENABLE_SIZE 0x0001
#define DEPAVPC_REG_0_DISP_PAVP_ENABLE_LSB 0x0001
#define DEPAVPC_REG_0_DISP_PAVP_ENABLE_SIZE 0x0001
#define DEPAVPC_REG_0_DISP_LOCK_LSB 0x0002
#define DEPAVPC_REG_0_DISP_LOCK_SIZE 0x0001
#define DEPAVPC_REG_0_DISP_HEAVY_MODE_SELECT_LSB 0x0003
#define DEPAVPC_REG_0_DISP_HEAVY_MODE_SELECT_SIZE 0x0001
#define DEPAVPC_REG_0_DISP_OVERRIDE_TERMINATE_ATTACK_LSB 0x0004
#define DEPAVPC_REG_0_DISP_OVERRIDE_TERMINATE_ATTACK_SIZE 0x0001
#define DEPAVPC_REG_0_DISP_RESERVD_LSB 0x0005
#define DEPAVPC_REG_0_DISP_RESERVD_SIZE 0x0001
#define DEPAVPC_REG_0_DISP_ASMF_METHOD_ENABLE_LSB 0x0006
#define DEPAVPC_REG_0_DISP_ASMF_METHOD_ENABLE_SIZE 0x0001
#define DEPAVPC_REG_0_DISP_WOPCMBASE_LSB 0x0014
#define DEPAVPC_REG_0_DISP_WOPCMBASE_SIZE 0x000c

#ifdef SV_HOOKS

/** DEPAVPC_REG_1 desc:
  */

#define DEPAVPC_REG_1_DISP_REG           (0x00134434U)

typedef union {
  struct {
    UINT32 wopcmbase : 32; /**< WOPCMBASE */
                            /* Bits[31:0], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DEPAVPC_REG_1_DISP_STRUCT;

#define DEPAVPC_REG_1_DISP_WIDTH 32
#define DEPAVPC_REG_1_DISP_DEFAULT 0x00000000U
#define DEPAVPC_REG_1_DISP_ENDPOINT DISP

#define DEPAVPC_REG_1_DISP_WOPCMBASE_LSB 0x0000
#define DEPAVPC_REG_1_DISP_WOPCMBASE_SIZE 0x0020

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DISMBASE_LSB desc:
  */

#define DISMBASE_LSB_DISP_REG            (0x00134440U)

typedef union {
  struct {
    UINT32 rsvd_0 : 24; /**< Undefined - auto filled rsvd_[23:00] */
                            /* Bits[23:0], default=0*/
    UINT32 dism_base_lsb : 8; /**< DISM BASE LSB */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DISMBASE_LSB_DISP_STRUCT;

#define DISMBASE_LSB_DISP_WIDTH 32
#define DISMBASE_LSB_DISP_DEFAULT 0x00000000U
#define DISMBASE_LSB_DISP_ENDPOINT DISP

#define DISMBASE_LSB_DISP_DISM_BASE_LSB_LSB 0x0018
#define DISMBASE_LSB_DISP_DISM_BASE_LSB_SIZE 0x0008

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DISMBASE_MSB desc:
  */

#define DISMBASE_MSB_DISP_REG            (0x00134444U)

typedef union {
  struct {
    UINT32 dism_base_msb : 32; /**< DISM BASE MSB */
                            /* Bits[31:0], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DISMBASE_MSB_DISP_STRUCT;

#define DISMBASE_MSB_DISP_WIDTH 32
#define DISMBASE_MSB_DISP_DEFAULT 0x00000000U
#define DISMBASE_MSB_DISP_ENDPOINT DISP

#define DISMBASE_MSB_DISP_DISM_BASE_MSB_LSB 0x0000
#define DISMBASE_MSB_DISP_DISM_BASE_MSB_SIZE 0x0020

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DISMLIMIT_LSB desc:
  */

#define DISMLIMIT_LSB_DISP_REG           (0x00134448U)

typedef union {
  struct {
    UINT32 rsvd_0 : 24; /**< Undefined - auto filled rsvd_[23:00] */
                            /* Bits[23:0], default=0*/
    UINT32 dism_limit_lsb : 8; /**< DISM LIMIT LSB */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DISMLIMIT_LSB_DISP_STRUCT;

#define DISMLIMIT_LSB_DISP_WIDTH 32
#define DISMLIMIT_LSB_DISP_DEFAULT 0x00000000U
#define DISMLIMIT_LSB_DISP_ENDPOINT DISP

#define DISMLIMIT_LSB_DISP_DISM_LIMIT_LSB_LSB 0x0018
#define DISMLIMIT_LSB_DISP_DISM_LIMIT_LSB_SIZE 0x0008

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DISMLIMIT_MSB desc:
  */

#define DISMLIMIT_MSB_DISP_REG           (0x0013444cU)

typedef union {
  struct {
    UINT32 dism_limit_msb : 32; /**< DISM LIMIT MSB */
                            /* Bits[31:0], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DISMLIMIT_MSB_DISP_STRUCT;

#define DISMLIMIT_MSB_DISP_WIDTH 32
#define DISMLIMIT_MSB_DISP_DEFAULT 0x00000000U
#define DISMLIMIT_MSB_DISP_ENDPOINT DISP

#define DISMLIMIT_MSB_DISP_DISM_LIMIT_MSB_LSB 0x0000
#define DISMLIMIT_MSB_DISP_DISM_LIMIT_MSB_SIZE 0x0020

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** DG_LMTT_DIR_PTR desc:
  */

#define DG_LMTT_DIR_PTR_DISP_REG         (0x001344a0U)

typedef union {
  struct {
    UINT32 lmtt_directory_pointer : 31; /**< LMTT DIRECTORY POINTER */
                            /* Bits[30:0], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[31:31] */
                            /* Bits[31:31], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DG_LMTT_DIR_PTR_DISP_STRUCT;

#define DG_LMTT_DIR_PTR_DISP_WIDTH 32
#define DG_LMTT_DIR_PTR_DISP_DEFAULT 0x00000000U
#define DG_LMTT_DIR_PTR_DISP_ENDPOINT DISP

#define DG_LMTT_DIR_PTR_DISP_LMTT_DIRECTORY_POINTER_LSB 0x0000
#define DG_LMTT_DIR_PTR_DISP_LMTT_DIRECTORY_POINTER_SIZE 0x001f

#endif      // SV_HOOKS

/** FLATCCSBASEANDRANGE_0 desc:
  */

#define FLATCCSBASEANDRANGE_0_DISP_REG   (0x001344b0U)

typedef union {
  struct {
    UINT32 ccs_enable : 1; /**< CCS ENABLE */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 5; /**< Undefined - auto filled rsvd_[05:01] */
                            /* Bits[5:1], default=0*/
    UINT32 flat_ccs_base_low : 26; /**< FLAT CCS BASE LOW */
                            /* Bits[31:6], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FLATCCSBASEANDRANGE_0_DISP_STRUCT;

#define FLATCCSBASEANDRANGE_0_DISP_WIDTH 32
#define FLATCCSBASEANDRANGE_0_DISP_DEFAULT 0x00000000U
#define FLATCCSBASEANDRANGE_0_DISP_ENDPOINT DISP

#define FLATCCSBASEANDRANGE_0_DISP_CCS_ENABLE_LSB 0x0000
#define FLATCCSBASEANDRANGE_0_DISP_CCS_ENABLE_SIZE 0x0001
#define FLATCCSBASEANDRANGE_0_DISP_FLAT_CCS_BASE_LOW_LSB 0x0006
#define FLATCCSBASEANDRANGE_0_DISP_FLAT_CCS_BASE_LOW_SIZE 0x001a

/** FLATCCSBASEANDRANGE_1 desc:
  */

#define FLATCCSBASEANDRANGE_1_DISP_REG   (0x001344b4U)

typedef union {
  struct {
    UINT32 flat_ccs_base_high : 10; /**< FLAT CCS BASE HIGH */
                            /* Bits[9:0], default=0x00000000*/
    UINT32 rsvd_0 : 4; /**< Undefined - auto filled rsvd_[13:10] */
                            /* Bits[13:10], default=0*/
    UINT32 flat_ccs_size : 14; /**< FLAT CCS SIZE */
                            /* Bits[27:14], default=0x00000000*/
    UINT32 rsvd_1 : 4; /**< Undefined - auto filled rsvd_[31:28] */
                            /* Bits[31:28], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FLATCCSBASEANDRANGE_1_DISP_STRUCT;

#define FLATCCSBASEANDRANGE_1_DISP_WIDTH 32
#define FLATCCSBASEANDRANGE_1_DISP_DEFAULT 0x00000000U
#define FLATCCSBASEANDRANGE_1_DISP_ENDPOINT DISP

#define FLATCCSBASEANDRANGE_1_DISP_FLAT_CCS_BASE_HIGH_LSB 0x0000
#define FLATCCSBASEANDRANGE_1_DISP_FLAT_CCS_BASE_HIGH_SIZE 0x000a
#define FLATCCSBASEANDRANGE_1_DISP_FLAT_CCS_SIZE_LSB 0x000e
#define FLATCCSBASEANDRANGE_1_DISP_FLAT_CCS_SIZE_SIZE 0x000e

//************************************************ /RegisterStructs


#endif      // _DISP_h

