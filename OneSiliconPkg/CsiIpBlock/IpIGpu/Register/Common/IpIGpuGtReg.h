/**@file
  IP IGpu GT library.

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

#ifndef _GT_h
#define _GT_h



// IP GUID: 90e8cab0-752a-4c5c-a376-6efd2192f397
#define GT_ID 0x90e8cab0752a4c5c

//************************************************ RegisterStructs

/** RC1P_FREQ_VALUE_RPM_GCD desc:
  */

#define RC1P_FREQ_VALUE_RPM_GCD_GT_REG   (0x00000c14U)

typedef union {
  struct {
    UINT32 rc1p_req_freq : 9; /**< Rc1p requested Freq */
                            /* Bits[8:0], default=0x00000000*/
    UINT32 rsvd_0 : 22; /**< Undefined - auto filled rsvd_[30:09] */
                            /* Bits[30:9], default=0*/
    UINT32 lock : 1; /**< Lock for R/WL Fields in this register */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RC1P_FREQ_VALUE_RPM_GCD_GT_STRUCT;

#define RC1P_FREQ_VALUE_RPM_GCD_GT_WIDTH 32
#define RC1P_FREQ_VALUE_RPM_GCD_GT_DEFAULT 0x00000000U
#define RC1P_FREQ_VALUE_RPM_GCD_GT_ENDPOINT GT

#define RC1P_FREQ_VALUE_RPM_GCD_GT_RC1P_REQ_FREQ_LSB 0x0000
#define RC1P_FREQ_VALUE_RPM_GCD_GT_RC1P_REQ_FREQ_SIZE 0x0009
#define RC1P_FREQ_VALUE_RPM_GCD_GT_LOCK_LSB 0x001f
#define RC1P_FREQ_VALUE_RPM_GCD_GT_LOCK_SIZE 0x0001

/** RC1P_FREQ_CHNG_EN_RPM_GCD desc:
  */

#define RC1P_FREQ_CHNG_EN_RPM_GCD_GT_REG (0x00000c18U)

typedef union {
  struct {
    UINT32 rc1p_enable : 1; /**< Rc1p Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 31; /**< Undefined - auto filled rsvd_[31:01] */
                            /* Bits[31:1], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RC1P_FREQ_CHNG_EN_RPM_GCD_GT_STRUCT;

#define RC1P_FREQ_CHNG_EN_RPM_GCD_GT_WIDTH 32
#define RC1P_FREQ_CHNG_EN_RPM_GCD_GT_DEFAULT 0x00000000U
#define RC1P_FREQ_CHNG_EN_RPM_GCD_GT_ENDPOINT GT

#define RC1P_FREQ_CHNG_EN_RPM_GCD_GT_RC1P_ENABLE_LSB 0x0000
#define RC1P_FREQ_CHNG_EN_RPM_GCD_GT_RC1P_ENABLE_SIZE 0x0001


/** RC1P_BUSY_THRESH_RPM_GCD desc:
  */

#define RC1P_BUSY_THRESH_RPM_GCD_GT_REG  (0x00000c1cU)

typedef union {
  struct {
    UINT32 rc1p_busy_hyst_count : 16; /**< RC1p Busy Hysteris Count Value */
                            /* Bits[15:0], default=0x00000000*/
    UINT32 rsvd_0 : 16; /**< Undefined - auto filled rsvd_[31:16] */
                            /* Bits[31:16], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RC1P_BUSY_THRESH_RPM_GCD_GT_STRUCT;

#define RC1P_BUSY_THRESH_RPM_GCD_GT_WIDTH 32
#define RC1P_BUSY_THRESH_RPM_GCD_GT_DEFAULT 0x00000000U
#define RC1P_BUSY_THRESH_RPM_GCD_GT_ENDPOINT GT

#define RC1P_BUSY_THRESH_RPM_GCD_GT_RC1P_BUSY_HYST_COUNT_LSB 0x0000
#define RC1P_BUSY_THRESH_RPM_GCD_GT_RC1P_BUSY_HYST_COUNT_SIZE 0x0010

/** CONFIG0_RPM_GCD desc:
  */

#define CONFIG0_RPM_GCD_GT_REG           (0x00000d00U)

typedef union {
  struct {
    UINT32 dis_tsc_sync : 1; /**< Disable TSC Synchronization */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 ctc_shift_param : 2; /**< CTC SHIFT parameter */
                            /* Bits[2:1], default=0x00000002*/
    UINT32 crysclock : 3; /**< Crystal Clock Freq Selector */
                            /* Bits[5:3], default=0x00000000*/
    UINT32 psmi_func_eq : 1; /**< PSMI Functional Selector */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 placeholder : 24; /**< Placeholder Bits */
                            /* Bits[30:7], default=0x00000000*/
    UINT32 lock : 1; /**< Lock for RW/L Fields in this Register */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CONFIG0_RPM_GCD_GT_STRUCT;

#define CONFIG0_RPM_GCD_GT_WIDTH 32
#define CONFIG0_RPM_GCD_GT_DEFAULT 0x00000004U
#define CONFIG0_RPM_GCD_GT_ENDPOINT GT

#define CONFIG0_RPM_GCD_GT_DIS_TSC_SYNC_LSB 0x0000
#define CONFIG0_RPM_GCD_GT_DIS_TSC_SYNC_SIZE 0x0001
#define CONFIG0_RPM_GCD_GT_CTC_SHIFT_PARAM_LSB 0x0001
#define CONFIG0_RPM_GCD_GT_CTC_SHIFT_PARAM_SIZE 0x0002
#define CONFIG0_RPM_GCD_GT_CRYSCLOCK_LSB 0x0003
#define CONFIG0_RPM_GCD_GT_CRYSCLOCK_SIZE 0x0003
#define CONFIG0_RPM_GCD_GT_PSMI_FUNC_EQ_LSB 0x0006
#define CONFIG0_RPM_GCD_GT_PSMI_FUNC_EQ_SIZE 0x0001
#define CONFIG0_RPM_GCD_GT_PLACEHOLDER_LSB 0x0007
#define CONFIG0_RPM_GCD_GT_PLACEHOLDER_SIZE 0x0018
#define CONFIG0_RPM_GCD_GT_LOCK_LSB 0x001f
#define CONFIG0_RPM_GCD_GT_LOCK_SIZE 0x0001


#ifdef SV_HOOKS

/** DFD_RESTORE_CFG_MSB_RPM_GCD desc:
  */

#define DFD_RESTORE_CFG_MSB_RPM_GCD_GT_REG (0x00000d38U)

typedef union {
  struct {
    UINT32 baseaddrmsb : 11; /**< Base address MSB */
                            /* Bits[10:0], default=0x00000000*/
    UINT32 rsvd_0 : 20; /**< Undefined - auto filled rsvd_[30:11] */
                            /* Bits[30:11], default=0*/
    UINT32 lock : 1; /**< Lock bit */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DFD_RESTORE_CFG_MSB_RPM_GCD_GT_STRUCT;

#define DFD_RESTORE_CFG_MSB_RPM_GCD_GT_WIDTH 32
#define DFD_RESTORE_CFG_MSB_RPM_GCD_GT_DEFAULT 0x00000000U
#define DFD_RESTORE_CFG_MSB_RPM_GCD_GT_ENDPOINT GT

#define DFD_RESTORE_CFG_MSB_RPM_GCD_GT_BASEADDRMSB_LSB 0x0000
#define DFD_RESTORE_CFG_MSB_RPM_GCD_GT_BASEADDRMSB_SIZE 0x000b
#define DFD_RESTORE_CFG_MSB_RPM_GCD_GT_LOCK_LSB 0x001f
#define DFD_RESTORE_CFG_MSB_RPM_GCD_GT_LOCK_SIZE 0x0001

#endif      // SV_HOOKS

/** RC6CTXBASE_RPM_GCD_0 desc:
  */

#define RC6CTXBASE_RPM_GCD_0_GT_REG      (0x00000d48U)

typedef union {
  struct {
    UINT32 rc6memlock : 1; /**< RC6Context Base Register Lock */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 11; /**< Undefined - auto filled rsvd_[11:01] */
                            /* Bits[11:1], default=0*/
    UINT32 rc6membase : 20; /**< RC6 Memory Base Low */
                            /* Bits[31:12], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RC6CTXBASE_RPM_GCD_0_GT_STRUCT;

#define RC6CTXBASE_RPM_GCD_0_GT_WIDTH 32
#define RC6CTXBASE_RPM_GCD_0_GT_DEFAULT 0x00000000U
#define RC6CTXBASE_RPM_GCD_0_GT_ENDPOINT GT

#define RC6CTXBASE_RPM_GCD_0_GT_RC6MEMLOCK_LSB 0x0000
#define RC6CTXBASE_RPM_GCD_0_GT_RC6MEMLOCK_SIZE 0x0001
#define RC6CTXBASE_RPM_GCD_0_GT_RC6MEMBASE_LSB 0x000c
#define RC6CTXBASE_RPM_GCD_0_GT_RC6MEMBASE_SIZE 0x0014

/** RC6CTXBASE_RPM_GCD_1 desc:
  */

#define RC6CTXBASE_RPM_GCD_1_GT_REG      (0x00000d4cU)

typedef union {
  struct {
    UINT32 rc6membase : 32; /**< RC6 Memory Base High */
                            /* Bits[31:0], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RC6CTXBASE_RPM_GCD_1_GT_STRUCT;

#define RC6CTXBASE_RPM_GCD_1_GT_WIDTH 32
#define RC6CTXBASE_RPM_GCD_1_GT_DEFAULT 0x00000000U
#define RC6CTXBASE_RPM_GCD_1_GT_ENDPOINT GT

#define RC6CTXBASE_RPM_GCD_1_GT_RC6MEMBASE_LSB 0x0000
#define RC6CTXBASE_RPM_GCD_1_GT_RC6MEMBASE_SIZE 0x0020

/** MIRROR_GMD_ID_RPM_GCD desc:
  */

#define MIRROR_GMD_ID_RPM_GCD_GT_REG     (0x00000d8cU)

typedef union {
  struct {
    UINT32 arch_id : 32; /**< Architecture ID */
                            /* Bits[31:0], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MIRROR_GMD_ID_RPM_GCD_GT_STRUCT;

#define MIRROR_GMD_ID_RPM_GCD_GT_WIDTH 32
#define MIRROR_GMD_ID_RPM_GCD_GT_DEFAULT 0x00000000U
#define MIRROR_GMD_ID_RPM_GCD_GT_ENDPOINT GT

#define MIRROR_GMD_ID_RPM_GCD_GT_ARCH_ID_LSB 0x0000
#define MIRROR_GMD_ID_RPM_GCD_GT_ARCH_ID_SIZE 0x0020

/** DRIVER_IP_FWAKE_ACK_RPM_GCD desc:
  */

#define DRIVER_IP_FWAKE_ACK_RPM_GCD_GT_REG (0x00000dfcU)

typedef union {
  struct {
    UINT32 ack : 16; /**< Force Wake Ack */
                            /* Bits[15:0], default=0x00000000*/
    UINT32 rsvd_0 : 16; /**< Undefined - auto filled rsvd_[31:16] */
                            /* Bits[31:16], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DRIVER_IP_FWAKE_ACK_RPM_GCD_GT_STRUCT;

#define DRIVER_IP_FWAKE_ACK_RPM_GCD_GT_WIDTH 32
#define DRIVER_IP_FWAKE_ACK_RPM_GCD_GT_DEFAULT 0x00000000U
#define DRIVER_IP_FWAKE_ACK_RPM_GCD_GT_ENDPOINT GT

#define DRIVER_IP_FWAKE_ACK_RPM_GCD_GT_ACK_LSB 0x0000
#define DRIVER_IP_FWAKE_ACK_RPM_GCD_GT_ACK_SIZE 0x0010

#ifdef SV_HOOKS

/** RC_PSMI_CTRL_RCSUNIT desc:
  */

#define RC_PSMI_CTRL_RCSUNIT_GT_REG      (0x00002050U)

typedef union {
  struct {
    UINT32 rc__idle_message_disable : 1; /**< RC* IDLE Message Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 message_bypass : 1; /**< Message Bypass */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 idle_flush_disable : 1; /**< IDLE Flush Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 idle_indicator : 1; /**< IDLE Indicator */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 go_indicator : 1; /**< GO Indicator */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 disable_idle_dop_flush_pipelining : 1; /**< Disable IDLE DOP flush pipelining */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 enable_idle_rc6_flush : 1; /**< Enable IDLE RC6 flush */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 sync_flip_v_blank_wait_for_event_power_down_disable : 1; /**< Sync Flip/V-blank Wait For Event Power Down Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[08:08] */
                            /* Bits[8:8], default=0*/
    UINT32 debug_flow_for_go_0_acknowledge : 1; /**< Debug Flow for GO=0 Acknowledge */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[10:10] */
                            /* Bits[10:10], default=0*/
    UINT32 psmi_parser_block : 1; /**< PSMI Parser Block */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 semaphore_wait_event_idle_message_disable : 1; /**< Semaphore Wait Event IDLE message Disable */
                            /* Bits[12:12], default=0x00000001*/
    UINT32 idle_message_override_control : 2; /**< IDLE Message Override Control */
                            /* Bits[14:13], default=0x00000000*/
    UINT32 inhibit_rc6_entry_on_wait_for_scanline : 1; /**< Inhibit RC6 Entry on Wait For Scanline */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 mask : 16; /**< Mask */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RC_PSMI_CTRL_RCSUNIT_GT_STRUCT;

#define RC_PSMI_CTRL_RCSUNIT_GT_WIDTH 32
#define RC_PSMI_CTRL_RCSUNIT_GT_DEFAULT 0x00001000U
#define RC_PSMI_CTRL_RCSUNIT_GT_ENDPOINT GT

#define RC_PSMI_CTRL_RCSUNIT_GT_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_RCSUNIT_GT_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_RCSUNIT_GT_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_RCSUNIT_GT_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_RCSUNIT_GT_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_RCSUNIT_GT_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_RCSUNIT_GT_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_RCSUNIT_GT_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_RCSUNIT_GT_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_RCSUNIT_GT_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_RCSUNIT_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_RCSUNIT_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_RCSUNIT_GT_ENABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_RCSUNIT_GT_ENABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_RCSUNIT_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_RCSUNIT_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_RCSUNIT_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_RCSUNIT_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_RCSUNIT_GT_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_RCSUNIT_GT_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_RCSUNIT_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_RCSUNIT_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_RCSUNIT_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_RCSUNIT_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_RCSUNIT_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_RCSUNIT_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_RCSUNIT_GT_MASK_LSB 0x0010
#define RC_PSMI_CTRL_RCSUNIT_GT_MASK_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** PWRCTX_MAXCNT_RCSUNIT desc:
  */

#define PWRCTX_MAXCNT_RCSUNIT_GT_REG     (0x00002054U)

typedef union {
  struct {
    UINT32 idle_wait_time : 20; /**< IDLE Wait Time */
                            /* Bits[19:0], default=0x0000000A*/
    UINT32 rsvd_0 : 12; /**< Undefined - auto filled rsvd_[31:20] */
                            /* Bits[31:20], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PWRCTX_MAXCNT_RCSUNIT_GT_STRUCT;

#define PWRCTX_MAXCNT_RCSUNIT_GT_WIDTH 32
#define PWRCTX_MAXCNT_RCSUNIT_GT_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_RCSUNIT_GT_ENDPOINT GT

#define PWRCTX_MAXCNT_RCSUNIT_GT_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_RCSUNIT_GT_IDLE_WAIT_TIME_SIZE 0x0014

#endif      // SV_HOOKS

/** FLAT_CCS_BASE_ADDR_GAMREQ_3D desc:
  */

#define FLAT_CCS_BASE_ADDR_GAMREQ_3D_GT_REG (0x00004910U)

typedef union {
  struct {
    UINT32 flat_ccs_en : 1; /**< Flat CCS enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 3; /**< Undefined - auto filled rsvd_[03:01] */
                            /* Bits[3:1], default=0*/
    UINT32 flat_ccs_lock : 1; /**< Flat CCS Lock */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 rsvd_1 : 27; /**< Undefined - auto filled rsvd_[31:05] */
                            /* Bits[31:5], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FLAT_CCS_BASE_ADDR_GAMREQ_3D_GT_STRUCT;

#define FLAT_CCS_BASE_ADDR_GAMREQ_3D_GT_WIDTH 32
#define FLAT_CCS_BASE_ADDR_GAMREQ_3D_GT_DEFAULT 0x00000000U
#define FLAT_CCS_BASE_ADDR_GAMREQ_3D_GT_ENDPOINT GT

#define FLAT_CCS_BASE_ADDR_GAMREQ_3D_GT_FLAT_CCS_EN_LSB 0x0000
#define FLAT_CCS_BASE_ADDR_GAMREQ_3D_GT_FLAT_CCS_EN_SIZE 0x0001
#define FLAT_CCS_BASE_ADDR_GAMREQ_3D_GT_FLAT_CCS_LOCK_LSB 0x0004
#define FLAT_CCS_BASE_ADDR_GAMREQ_3D_GT_FLAT_CCS_LOCK_SIZE 0x0001

#ifdef SV_HOOKS

/** SQCM_MCFG_GRP desc:
  */

#define SQCM_MCFG_GRP_GT_REG             (0x00008724U)

typedef union {
  struct {
    UINT32 cmiarb : 1; /**< CMI arbitration. */
                            /* Bits[0:0], default=0x00000001*/
    UINT32 ecorsvd_3_1 : 3; /**< ECORSVD_3_1 */
                            /* Bits[3:1], default=0x00000000*/
    UINT32 sqpi : 1; /**< SQPI */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 ecorsvd_10_5 : 6; /**< ECORSVD_10_5 */
                            /* Bits[10:5], default=0x00000000*/
    UINT32 lock : 1; /**< Security Lock bit */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 ecorsvd_13_12 : 2; /**< ECORSVD_13_12 */
                            /* Bits[13:12], default=0x00000000*/
    UINT32 aes_bypass_disable : 1; /**< AES BYPASS Disable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 ecorsvd_15 : 1; /**< ECORSVD_15 */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 sq_gdr_wd_en : 1; /**< Enable GDR/WD in SQ */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 sqdfv : 1; /**< Enable for DFV Ownership Window */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[18:18] */
                            /* Bits[18:18], default=0*/
    UINT32 ecorsvd_29_18 : 11; /**< ECORSVD_29_19 */
                            /* Bits[29:19], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[30:30] */
                            /* Bits[30:30], default=0*/
    UINT32 ecorsvd_31 : 1; /**< ECORSVD_31 */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} SQCM_MCFG_GRP_GT_STRUCT;

#define SQCM_MCFG_GRP_GT_WIDTH 32
#define SQCM_MCFG_GRP_GT_DEFAULT 0x00000001U
#define SQCM_MCFG_GRP_GT_ENDPOINT GT

#define SQCM_MCFG_GRP_GT_CMIARB_LSB 0x0000
#define SQCM_MCFG_GRP_GT_CMIARB_SIZE 0x0001
#define SQCM_MCFG_GRP_GT_ECORSVD_3_1_LSB 0x0001
#define SQCM_MCFG_GRP_GT_ECORSVD_3_1_SIZE 0x0003
#define SQCM_MCFG_GRP_GT_SQPI_LSB 0x0004
#define SQCM_MCFG_GRP_GT_SQPI_SIZE 0x0001
#define SQCM_MCFG_GRP_GT_ECORSVD_10_5_LSB 0x0005
#define SQCM_MCFG_GRP_GT_ECORSVD_10_5_SIZE 0x0006
#define SQCM_MCFG_GRP_GT_LOCK_LSB 0x000b
#define SQCM_MCFG_GRP_GT_LOCK_SIZE 0x0001
#define SQCM_MCFG_GRP_GT_ECORSVD_13_12_LSB 0x000c
#define SQCM_MCFG_GRP_GT_ECORSVD_13_12_SIZE 0x0002
#define SQCM_MCFG_GRP_GT_AES_BYPASS_DISABLE_LSB 0x000e
#define SQCM_MCFG_GRP_GT_AES_BYPASS_DISABLE_SIZE 0x0001
#define SQCM_MCFG_GRP_GT_ECORSVD_15_LSB 0x000f
#define SQCM_MCFG_GRP_GT_ECORSVD_15_SIZE 0x0001
#define SQCM_MCFG_GRP_GT_SQ_GDR_WD_EN_LSB 0x0010
#define SQCM_MCFG_GRP_GT_SQ_GDR_WD_EN_SIZE 0x0001
#define SQCM_MCFG_GRP_GT_SQDFV_LSB 0x0011
#define SQCM_MCFG_GRP_GT_SQDFV_SIZE 0x0001
#define SQCM_MCFG_GRP_GT_ECORSVD_29_18_LSB 0x0013
#define SQCM_MCFG_GRP_GT_ECORSVD_29_18_SIZE 0x000b
#define SQCM_MCFG_GRP_GT_ECORSVD_31_LSB 0x001f
#define SQCM_MCFG_GRP_GT_ECORSVD_31_SIZE 0x0001

#endif      // SV_HOOKS

/** FLATCCSBASEANDRANGE_LOWER_I_GCD desc:
  */

#define FLATCCSBASEANDRANGE_LOWER_I_GCD_GT_REG (0x00008800U)

typedef union {
  struct {
    UINT32 ccsenable : 1; /**< CCS enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 5; /**< Undefined - auto filled rsvd_[05:01] */
                            /* Bits[5:1], default=0*/
    UINT32 hwviewofflatccsbaseatcclevello : 26; /**< Hardware view of Flat CCS at CCunit level Lo */
                            /* Bits[31:6], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FLATCCSBASEANDRANGE_LOWER_I_GCD_GT_STRUCT;

#define FLATCCSBASEANDRANGE_LOWER_I_GCD_GT_WIDTH 32
#define FLATCCSBASEANDRANGE_LOWER_I_GCD_GT_DEFAULT 0x00000000U
#define FLATCCSBASEANDRANGE_LOWER_I_GCD_GT_ENDPOINT GT

#define FLATCCSBASEANDRANGE_LOWER_I_GCD_GT_CCSENABLE_LSB 0x0000
#define FLATCCSBASEANDRANGE_LOWER_I_GCD_GT_CCSENABLE_SIZE 0x0001
#define FLATCCSBASEANDRANGE_LOWER_I_GCD_GT_HWVIEWOFFLATCCSBASEATCCLEVELLO_LSB 0x0006
#define FLATCCSBASEANDRANGE_LOWER_I_GCD_GT_HWVIEWOFFLATCCSBASEATCCLEVELLO_SIZE 0x001a

/** FLATCCSBASEANDRANGE_UPPER_I_GCD desc:
  */

#define FLATCCSBASEANDRANGE_UPPER_I_GCD_GT_REG (0x00008804U)

typedef union {
  struct {
    UINT32 swviewofflatccsbasehi : 10; /**< SoftWare view of Flat CCS Hi */
                            /* Bits[9:0], default=0x00000000*/
    UINT32 rsvd_0 : 4; /**< Undefined - auto filled rsvd_[13:10] */
                            /* Bits[13:10], default=0*/
    UINT32 flatccsize : 14; /**< Flat CCS size */
                            /* Bits[27:14], default=0x00000000*/
    UINT32 rsvd_1 : 4; /**< Undefined - auto filled rsvd_[31:28] */
                            /* Bits[31:28], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FLATCCSBASEANDRANGE_UPPER_I_GCD_GT_STRUCT;

#define FLATCCSBASEANDRANGE_UPPER_I_GCD_GT_WIDTH 32
#define FLATCCSBASEANDRANGE_UPPER_I_GCD_GT_DEFAULT 0x00000000U
#define FLATCCSBASEANDRANGE_UPPER_I_GCD_GT_ENDPOINT GT

#define FLATCCSBASEANDRANGE_UPPER_I_GCD_GT_SWVIEWOFFLATCCSBASEHI_LSB 0x0000
#define FLATCCSBASEANDRANGE_UPPER_I_GCD_GT_SWVIEWOFFLATCCSBASEHI_SIZE 0x000a
#define FLATCCSBASEANDRANGE_UPPER_I_GCD_GT_FLATCCSIZE_LSB 0x000e
#define FLATCCSBASEANDRANGE_UPPER_I_GCD_GT_FLATCCSIZE_SIZE 0x000e

#ifdef SV_HOOKS

/** CCCHKNREG2_I_GCD desc:
  */

#define CCCHKNREG2_I_GCD_GT_REG          (0x0000881cU)

typedef union {
  struct {
    UINT32 ccchknbits2 : 32; /**< Chicken bits for CC Unit */
                            /* Bits[31:0], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CCCHKNREG2_I_GCD_GT_STRUCT;

#define CCCHKNREG2_I_GCD_GT_WIDTH 32
#define CCCHKNREG2_I_GCD_GT_DEFAULT 0x00000000U
#define CCCHKNREG2_I_GCD_GT_ENDPOINT GT

#define CCCHKNREG2_I_GCD_GT_CCCHKNBITS2_LSB 0x0000
#define CCCHKNREG2_I_GCD_GT_CCCHKNBITS2_SIZE 0x0020

#endif      // SV_HOOKS

/** CCCHKNREG1_I_GCD desc:
  */

#define CCCHKNREG1_I_GCD_GT_REG          (0x00008828U)

typedef union {
  struct {
    UINT32 slothashlogicchkn : 1; /**< slot hash logic chicken bit */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 setxorlogicchkn : 1; /**< Set XoR chicken bit */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 hbmboundcheckdis : 1; /**< hbm bound check dis */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 ccscacheparityinv : 1; /**< parity inversion bit for ccs cache */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 readmissparityinv : 1; /**< parity inverse bit for read miss */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 writemissparityinv : 1; /**< parity inverse bit for write miss */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 sqccparityinv : 1; /**< parity inverse bit for sq-cc interface */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 cclnepparityinv : 1; /**< parity inverse bit for cc-lnep interface */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 paritycheckdis : 1; /**< Parity check disable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 rsvd_0 : 5; /**< Undefined - auto filled rsvd_[13:09] */
                            /* Bits[13:9], default=0*/
    UINT32 _32brddisable : 1; /**< Disable 32B read from CCunit */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 _32bwrdisable : 1; /**< Disable 32B writes from CCunit */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 tdflushcntl : 1; /**< Disable special handling of transient display CCS flush */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 l4cacntlcsw : 1; /**< L4 Cacheability control for CCS access */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 encompperffix : 1; /**< Enable Compression performance fix */
                            /* Bits[18:18], default=0x00000001*/
    UINT32 disoooccsrtn : 1; /**< Disable OOO CCS return processing */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 rsvd_1 : 2; /**< Undefined - auto filled rsvd_[21:20] */
                            /* Bits[21:20], default=0*/
    UINT32 enccspf : 1; /**< Enable CCS cache prefetch */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 l3cmpctrl : 1; /**< L3 Compression Control */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 l2dyncompctrl : 1; /**< L2 Dynamic Compression Control */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 compovfdis : 1; /**< Overfetch Disable for compressed surfaces */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 ccchknbits : 6; /**< Chicken bits for CC Unit */
                            /* Bits[31:26], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CCCHKNREG1_I_GCD_GT_STRUCT;

#define CCCHKNREG1_I_GCD_GT_WIDTH 32
#define CCCHKNREG1_I_GCD_GT_DEFAULT 0x00040000U
#define CCCHKNREG1_I_GCD_GT_ENDPOINT GT

#define CCCHKNREG1_I_GCD_GT_SLOTHASHLOGICCHKN_LSB 0x0000
#define CCCHKNREG1_I_GCD_GT_SLOTHASHLOGICCHKN_SIZE 0x0001
#define CCCHKNREG1_I_GCD_GT_SETXORLOGICCHKN_LSB 0x0001
#define CCCHKNREG1_I_GCD_GT_SETXORLOGICCHKN_SIZE 0x0001
#define CCCHKNREG1_I_GCD_GT_HBMBOUNDCHECKDIS_LSB 0x0002
#define CCCHKNREG1_I_GCD_GT_HBMBOUNDCHECKDIS_SIZE 0x0001
#define CCCHKNREG1_I_GCD_GT_CCSCACHEPARITYINV_LSB 0x0003
#define CCCHKNREG1_I_GCD_GT_CCSCACHEPARITYINV_SIZE 0x0001
#define CCCHKNREG1_I_GCD_GT_READMISSPARITYINV_LSB 0x0004
#define CCCHKNREG1_I_GCD_GT_READMISSPARITYINV_SIZE 0x0001
#define CCCHKNREG1_I_GCD_GT_WRITEMISSPARITYINV_LSB 0x0005
#define CCCHKNREG1_I_GCD_GT_WRITEMISSPARITYINV_SIZE 0x0001
#define CCCHKNREG1_I_GCD_GT_SQCCPARITYINV_LSB 0x0006
#define CCCHKNREG1_I_GCD_GT_SQCCPARITYINV_SIZE 0x0001
#define CCCHKNREG1_I_GCD_GT_CCLNEPPARITYINV_LSB 0x0007
#define CCCHKNREG1_I_GCD_GT_CCLNEPPARITYINV_SIZE 0x0001
#define CCCHKNREG1_I_GCD_GT_PARITYCHECKDIS_LSB 0x0008
#define CCCHKNREG1_I_GCD_GT_PARITYCHECKDIS_SIZE 0x0001
#define CCCHKNREG1_I_GCD_GT__32BRDDISABLE_LSB 0x000e
#define CCCHKNREG1_I_GCD_GT__32BRDDISABLE_SIZE 0x0001
#define CCCHKNREG1_I_GCD_GT__32BWRDISABLE_LSB 0x000f
#define CCCHKNREG1_I_GCD_GT__32BWRDISABLE_SIZE 0x0001
#define CCCHKNREG1_I_GCD_GT_TDFLUSHCNTL_LSB 0x0010
#define CCCHKNREG1_I_GCD_GT_TDFLUSHCNTL_SIZE 0x0001
#define CCCHKNREG1_I_GCD_GT_L4CACNTLCSW_LSB 0x0011
#define CCCHKNREG1_I_GCD_GT_L4CACNTLCSW_SIZE 0x0001
#define CCCHKNREG1_I_GCD_GT_ENCOMPPERFFIX_LSB 0x0012
#define CCCHKNREG1_I_GCD_GT_ENCOMPPERFFIX_SIZE 0x0001
#define CCCHKNREG1_I_GCD_GT_DISOOOCCSRTN_LSB 0x0013
#define CCCHKNREG1_I_GCD_GT_DISOOOCCSRTN_SIZE 0x0001
#define CCCHKNREG1_I_GCD_GT_ENCCSPF_LSB 0x0016
#define CCCHKNREG1_I_GCD_GT_ENCCSPF_SIZE 0x0001
#define CCCHKNREG1_I_GCD_GT_L3CMPCTRL_LSB 0x0017
#define CCCHKNREG1_I_GCD_GT_L3CMPCTRL_SIZE 0x0001
#define CCCHKNREG1_I_GCD_GT_L2DYNCOMPCTRL_LSB 0x0018
#define CCCHKNREG1_I_GCD_GT_L2DYNCOMPCTRL_SIZE 0x0001
#define CCCHKNREG1_I_GCD_GT_COMPOVFDIS_LSB 0x0019
#define CCCHKNREG1_I_GCD_GT_COMPOVFDIS_SIZE 0x0001
#define CCCHKNREG1_I_GCD_GT_CCCHKNBITS_LSB 0x001a
#define CCCHKNREG1_I_GCD_GT_CCCHKNBITS_SIZE 0x0006

/** CCLOCK_I_GCD desc:
  */

#define CCLOCK_I_GCD_GT_REG              (0x0000882cU)

typedef union {
  struct {
    UINT32 ccchknreg1_lock : 1; /**< Lockdown CCchicken register for write */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 flatccsbaseandrange_lock : 1; /**< Lockdown flat ccs base register for write */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 rsvd_0 : 29; /**< Undefined - auto filled rsvd_[30:02] */
                            /* Bits[30:2], default=0*/
    UINT32 cclock_lock : 1; /**< Lockdown CCLOCK for writes */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CCLOCK_I_GCD_GT_STRUCT;

#define CCLOCK_I_GCD_GT_WIDTH 32
#define CCLOCK_I_GCD_GT_DEFAULT 0x00000000U
#define CCLOCK_I_GCD_GT_ENDPOINT GT

#define CCLOCK_I_GCD_GT_CCCHKNREG1_LOCK_LSB 0x0000
#define CCLOCK_I_GCD_GT_CCCHKNREG1_LOCK_SIZE 0x0001
#define CCLOCK_I_GCD_GT_FLATCCSBASEANDRANGE_LOCK_LSB 0x0001
#define CCLOCK_I_GCD_GT_FLATCCSBASEANDRANGE_LOCK_SIZE 0x0001
#define CCLOCK_I_GCD_GT_CCLOCK_LOCK_LSB 0x001f
#define CCLOCK_I_GCD_GT_CCLOCK_LOCK_SIZE 0x0001

/** UNSLCGCTL9430_3D desc:
  */

#define UNSLCGCTL9430_3D_GT_REG          (0x00009430U)

typedef union {
  struct {
    UINT32 cg3ddismcr : 1; /**< MCRunit Clock Gating Disable */
                            /* Bits[0:0], default=0x00000001*/
    UINT32 cg3ddismisd : 1; /**< MISDunit Clock Gating Disable */
                            /* Bits[1:1], default=0x00000001*/
    UINT32 cg3ddismertxg : 1; /**< MERTXG Clock Gating Disable */
                            /* Bits[2:2], default=0x00000001*/
    UINT32 cg3ddismsqcc : 1; /**< MSQCCUNIT Clock Gating Disable */
                            /* Bits[3:3], default=0x00000001*/
    UINT32 cg3ddismertg : 1; /**< cg3ddismertg Clock Gating Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 cg3ddismbgf : 1; /**< MBGFunit Clock Gating Disable */
                            /* Bits[5:5], default=0x00000001*/
    UINT32 cg3ddisgpm : 1; /**< GPMunit Clock Gating Disable */
                            /* Bits[6:6], default=0x00000001*/
    UINT32 cg3ddis_spare4 : 1; /**< SPARE4 Clock Gating Disable */
                            /* Bits[7:7], default=0x00000001*/
    UINT32 cg3ddis_spare5 : 1; /**< SPARE5 Clock Gating Disable */
                            /* Bits[8:8], default=0x00000001*/
    UINT32 cg3ddis_spare6 : 1; /**< SPARE6 Clock Gating Disable */
                            /* Bits[9:9], default=0x00000001*/
    UINT32 cg3ddisgcp : 1; /**< GCPunit Clock Gating Disable */
                            /* Bits[10:10], default=0x00000001*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[11:11] */
                            /* Bits[11:11], default=0*/
    UINT32 cg3ddislngp : 1; /**< LNGP Clock Gating Disable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 cg3ddislnep : 1; /**< LNEP Clock Gating Disable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 cg3ddiscfe : 1; /**< SCFE Clock Gating Disable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 cg3ddisrcpbpix : 1; /**< SRCPBPIX Clock Gating Disable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 cg3ddisoac : 1; /**< OAC Clock Gating Disable */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 ramcgdis_rcc : 1; /**< RAMRCC Clock Gating Disable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 cg3ddisrcc : 1; /**< RCC Clock Gating Disable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 cg3ddisdap : 1; /**< DAP Clock Gating Disable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 cg3ddissarb : 1; /**< SARB Clock Gating Disable */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 cg3ddissmrocdata : 1; /**< SMROCDATA Clock Gating Disable */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 cg3ddissmroctag : 1; /**< SMROCTAG Clock Gating Disable */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 cg3ddislnib : 1; /**< LNIB Clock Gating Disable */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 cg3ddislnic : 1; /**< LNIC Clock Gating Disable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 cg3ddisrcpbarb : 1; /**< RCPBARB Clock Gating Disable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 cg3ddisrcpblat : 1; /**< RCPBLAT Clock Gating Disable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 cg3ddismertgart : 1; /**< mertgart Clock Gating Disable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 cg3ddisoaal3 : 1; /**< oaal3 Clock Gating Disable */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 cg3ddisoaal2 : 1; /**< oaal2 Clock Gating Disable */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 cg3ddisaxfgt : 1; /**< AXFGT Clock Gating Disable */
                            /* Bits[30:30], default=0x00000001*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[31:31] */
                            /* Bits[31:31], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UNSLCGCTL9430_3D_GT_STRUCT;

#define UNSLCGCTL9430_3D_GT_WIDTH 32
#define UNSLCGCTL9430_3D_GT_DEFAULT 0x400007efU
#define UNSLCGCTL9430_3D_GT_ENDPOINT GT

#define UNSLCGCTL9430_3D_GT_CG3DDISMCR_LSB 0x0000
#define UNSLCGCTL9430_3D_GT_CG3DDISMCR_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISMISD_LSB 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISMISD_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISMERTXG_LSB 0x0002
#define UNSLCGCTL9430_3D_GT_CG3DDISMERTXG_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISMSQCC_LSB 0x0003
#define UNSLCGCTL9430_3D_GT_CG3DDISMSQCC_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISMERTG_LSB 0x0004
#define UNSLCGCTL9430_3D_GT_CG3DDISMERTG_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISMBGF_LSB 0x0005
#define UNSLCGCTL9430_3D_GT_CG3DDISMBGF_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISGPM_LSB 0x0006
#define UNSLCGCTL9430_3D_GT_CG3DDISGPM_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDIS_SPARE4_LSB 0x0007
#define UNSLCGCTL9430_3D_GT_CG3DDIS_SPARE4_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDIS_SPARE5_LSB 0x0008
#define UNSLCGCTL9430_3D_GT_CG3DDIS_SPARE5_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDIS_SPARE6_LSB 0x0009
#define UNSLCGCTL9430_3D_GT_CG3DDIS_SPARE6_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISGCP_LSB 0x000a
#define UNSLCGCTL9430_3D_GT_CG3DDISGCP_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISLNGP_LSB 0x000c
#define UNSLCGCTL9430_3D_GT_CG3DDISLNGP_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISLNEP_LSB 0x000d
#define UNSLCGCTL9430_3D_GT_CG3DDISLNEP_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISCFE_LSB 0x000e
#define UNSLCGCTL9430_3D_GT_CG3DDISCFE_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISRCPBPIX_LSB 0x000f
#define UNSLCGCTL9430_3D_GT_CG3DDISRCPBPIX_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISOAC_LSB 0x0010
#define UNSLCGCTL9430_3D_GT_CG3DDISOAC_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_RAMCGDIS_RCC_LSB 0x0011
#define UNSLCGCTL9430_3D_GT_RAMCGDIS_RCC_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISRCC_LSB 0x0012
#define UNSLCGCTL9430_3D_GT_CG3DDISRCC_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISDAP_LSB 0x0013
#define UNSLCGCTL9430_3D_GT_CG3DDISDAP_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISSARB_LSB 0x0014
#define UNSLCGCTL9430_3D_GT_CG3DDISSARB_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISSMROCDATA_LSB 0x0015
#define UNSLCGCTL9430_3D_GT_CG3DDISSMROCDATA_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISSMROCTAG_LSB 0x0016
#define UNSLCGCTL9430_3D_GT_CG3DDISSMROCTAG_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISLNIB_LSB 0x0017
#define UNSLCGCTL9430_3D_GT_CG3DDISLNIB_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISLNIC_LSB 0x0018
#define UNSLCGCTL9430_3D_GT_CG3DDISLNIC_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISRCPBARB_LSB 0x0019
#define UNSLCGCTL9430_3D_GT_CG3DDISRCPBARB_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISRCPBLAT_LSB 0x001a
#define UNSLCGCTL9430_3D_GT_CG3DDISRCPBLAT_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISMERTGART_LSB 0x001b
#define UNSLCGCTL9430_3D_GT_CG3DDISMERTGART_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISOAAL3_LSB 0x001c
#define UNSLCGCTL9430_3D_GT_CG3DDISOAAL3_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISOAAL2_LSB 0x001d
#define UNSLCGCTL9430_3D_GT_CG3DDISOAAL2_SIZE 0x0001
#define UNSLCGCTL9430_3D_GT_CG3DDISAXFGT_LSB 0x001e
#define UNSLCGCTL9430_3D_GT_CG3DDISAXFGT_SIZE 0x0001

/** UNSLCGCTL9434_3D desc:
  */

#define UNSLCGCTL9434_3D_GT_REG          (0x00009434U)

typedef union {
  struct {
    UINT32 cg3ddisbcsfe : 1; /**< BCS FE Clock Gating Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cg3ddisbcsbe : 1; /**< BCS BE Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 cgacgdis_oa : 1; /**< OA Arch Clock Gating Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 cg3ddisoar : 1; /**< OARUNIT Clock Gating Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 cg3ddisnoa : 1; /**< NOAunit Clock Gating Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 cg3ddisoacs : 1; /**< OACSunit Clock Gating Disable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 cg3ddissuc : 1; /**< SUCunit Clock Gating Disable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 cg3ddisvfgr : 1; /**< cg3ddisvfgr Clock Gating Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 cg3ddisbsc : 1; /**< cg3ddisbsc Clock Gating Disable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 cg3ddisgacs : 1; /**< gacs Clock Gating Disable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 cg3ddisgamid : 1; /**< gamid Clock Gating Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 cg3ddisgamedia : 1; /**< gamedia Clock Gating Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 cgacgdis_pix_be : 1; /**< PixBE Arch Clock Gating Disable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 cgacgdis_td : 1; /**< ThreadDisp Arch Clock Gating Disable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 cg3ddispocsbe : 1; /**< POCSBE Clock Gating Disable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 cg3ddispocsfe : 1; /**< POCSFE Clock Gating Disable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 cg3ddispocs : 1; /**< POCS Clock Gating Disable */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 cg3ddiscfeg : 1; /**< cg3ddiscfeg Clock Gating Disable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 cg3ddisgamtlbmisarb : 1; /**< gamtlbmisarb Clock Gating Disable */
                            /* Bits[18:18], default=0x00000001*/
    UINT32 cgacgdis_cs : 1; /**< CS Arch Clock Gating Disable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 cg3ddisgamtlbhitarb : 1; /**< gamtlbhitarb Clock Gating Disable */
                            /* Bits[20:20], default=0x00000001*/
    UINT32 cg3ddisbcs : 1; /**< BCSunit Clock Gating Disable */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 cg3ddisbd : 1; /**< BDunit Clock Gating Disable */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 cg3ddisbf : 1; /**< BFunit Clock Gating Disable */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 cg3ddisblb : 1; /**< BLBunit Clock Gating Disable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 cg3ddisbls : 1; /**< BLSunit Clock Gating Disable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 cg3ddisvfg : 1; /**< cg3ddisvfg Clock Gating Disable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 cg3ddisdt : 1; /**< DTunit Clock Gating Disable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 cg3ddisgab : 1; /**< GABunit Clock Gating Disable */
                            /* Bits[28:28], default=0x00000001*/
    UINT32 cgacgdis_guc : 1; /**< GUC Arch Clock Gating Disable */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 cg3ddismbc : 1; /**< MBCunit Clock Gating Disable */
                            /* Bits[30:30], default=0x00000001*/
    UINT32 cg3ddisgti : 1; /**< GTIunit Clock Gating Disable */
                            /* Bits[31:31], default=0x00000001*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UNSLCGCTL9434_3D_GT_STRUCT;

#define UNSLCGCTL9434_3D_GT_WIDTH 32
#define UNSLCGCTL9434_3D_GT_DEFAULT 0xd0140000U
#define UNSLCGCTL9434_3D_GT_ENDPOINT GT

#define UNSLCGCTL9434_3D_GT_CG3DDISBCSFE_LSB 0x0000
#define UNSLCGCTL9434_3D_GT_CG3DDISBCSFE_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISBCSBE_LSB 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISBCSBE_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CGACGDIS_OA_LSB 0x0002
#define UNSLCGCTL9434_3D_GT_CGACGDIS_OA_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISOAR_LSB 0x0003
#define UNSLCGCTL9434_3D_GT_CG3DDISOAR_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISNOA_LSB 0x0004
#define UNSLCGCTL9434_3D_GT_CG3DDISNOA_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISOACS_LSB 0x0005
#define UNSLCGCTL9434_3D_GT_CG3DDISOACS_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISSUC_LSB 0x0006
#define UNSLCGCTL9434_3D_GT_CG3DDISSUC_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISVFGR_LSB 0x0007
#define UNSLCGCTL9434_3D_GT_CG3DDISVFGR_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISBSC_LSB 0x0008
#define UNSLCGCTL9434_3D_GT_CG3DDISBSC_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISGACS_LSB 0x0009
#define UNSLCGCTL9434_3D_GT_CG3DDISGACS_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISGAMID_LSB 0x000a
#define UNSLCGCTL9434_3D_GT_CG3DDISGAMID_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISGAMEDIA_LSB 0x000b
#define UNSLCGCTL9434_3D_GT_CG3DDISGAMEDIA_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CGACGDIS_PIX_BE_LSB 0x000c
#define UNSLCGCTL9434_3D_GT_CGACGDIS_PIX_BE_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CGACGDIS_TD_LSB 0x000d
#define UNSLCGCTL9434_3D_GT_CGACGDIS_TD_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISPOCSBE_LSB 0x000e
#define UNSLCGCTL9434_3D_GT_CG3DDISPOCSBE_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISPOCSFE_LSB 0x000f
#define UNSLCGCTL9434_3D_GT_CG3DDISPOCSFE_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISPOCS_LSB 0x0010
#define UNSLCGCTL9434_3D_GT_CG3DDISPOCS_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISCFEG_LSB 0x0011
#define UNSLCGCTL9434_3D_GT_CG3DDISCFEG_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISGAMTLBMISARB_LSB 0x0012
#define UNSLCGCTL9434_3D_GT_CG3DDISGAMTLBMISARB_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CGACGDIS_CS_LSB 0x0013
#define UNSLCGCTL9434_3D_GT_CGACGDIS_CS_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISGAMTLBHITARB_LSB 0x0014
#define UNSLCGCTL9434_3D_GT_CG3DDISGAMTLBHITARB_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISBCS_LSB 0x0015
#define UNSLCGCTL9434_3D_GT_CG3DDISBCS_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISBD_LSB 0x0016
#define UNSLCGCTL9434_3D_GT_CG3DDISBD_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISBF_LSB 0x0017
#define UNSLCGCTL9434_3D_GT_CG3DDISBF_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISBLB_LSB 0x0018
#define UNSLCGCTL9434_3D_GT_CG3DDISBLB_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISBLS_LSB 0x0019
#define UNSLCGCTL9434_3D_GT_CG3DDISBLS_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISVFG_LSB 0x001a
#define UNSLCGCTL9434_3D_GT_CG3DDISVFG_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISDT_LSB 0x001b
#define UNSLCGCTL9434_3D_GT_CG3DDISDT_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISGAB_LSB 0x001c
#define UNSLCGCTL9434_3D_GT_CG3DDISGAB_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CGACGDIS_GUC_LSB 0x001d
#define UNSLCGCTL9434_3D_GT_CGACGDIS_GUC_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISMBC_LSB 0x001e
#define UNSLCGCTL9434_3D_GT_CG3DDISMBC_SIZE 0x0001
#define UNSLCGCTL9434_3D_GT_CG3DDISGTI_LSB 0x001f
#define UNSLCGCTL9434_3D_GT_CG3DDISGTI_SIZE 0x0001

/** UNSLCGCTL9438_3D desc:
  */

#define UNSLCGCTL9438_3D_GT_REG          (0x00009438U)

typedef union {
  struct {
    UINT32 cg3ddisccs0 : 1; /**< ccsunit0 Clock Gating Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cg3ddisccs1 : 1; /**< ccsunit1 Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 cg3ddisccs2 : 1; /**< ccsunit2 Clock Gating Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 cg3ddisccs3 : 1; /**< ccsunit3 Clock Gating Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 cg3ddisccs0fe : 1; /**< ccs0fe Clock Gating Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 cg3ddisccs1fe : 1; /**< ccs1fe Clock Gating Disable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 cg3ddisccs2fe : 1; /**< ccs2fe Clock Gating Disable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 cg3ddisccs3fe : 1; /**< ccs3fe Clock Gating Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 cg3ddisguc : 1; /**< GUC Clock Gating Disable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 rsvd_0 : 11; /**< Undefined - auto filled rsvd_[19:09] */
                            /* Bits[19:9], default=0*/
    UINT32 cg3ddisccs0be : 1; /**< ccs0be Clock Gating Disable */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 cg3ddisccs1be : 1; /**< ccs1be Clock Gating Disable */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 cg3ddisccs2be : 1; /**< ccs2be Clock Gating Disable */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 cg3ddisccs3be : 1; /**< ccs3be Clock Gating Disable */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 cg3ddismgsx : 1; /**< mgsx Clock Gating Disable */
                            /* Bits[24:24], default=0x00000001*/
    UINT32 cg3ddisgamxbl : 1; /**< GAMXBL Clock Gating Disable */
                            /* Bits[25:25], default=0x00000001*/
    UINT32 cg3ddislne : 1; /**< LNEUNIT Clock Gating Disable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 rsvd_1 : 2; /**< Undefined - auto filled rsvd_[28:27] */
                            /* Bits[28:27], default=0*/
    UINT32 cg3ddismsqcw : 1; /**< MSQCW Clock Gating Disable */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 cg3ddismsqcr : 1; /**< MSQCR Clock Gating Disable */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 cg3ddismaxf : 1; /**< maxf Clock Gating Disable */
                            /* Bits[31:31], default=0x00000001*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UNSLCGCTL9438_3D_GT_STRUCT;

#define UNSLCGCTL9438_3D_GT_WIDTH 32
#define UNSLCGCTL9438_3D_GT_DEFAULT 0x83000000U
#define UNSLCGCTL9438_3D_GT_ENDPOINT GT

#define UNSLCGCTL9438_3D_GT_CG3DDISCCS0_LSB 0x0000
#define UNSLCGCTL9438_3D_GT_CG3DDISCCS0_SIZE 0x0001
#define UNSLCGCTL9438_3D_GT_CG3DDISCCS1_LSB 0x0001
#define UNSLCGCTL9438_3D_GT_CG3DDISCCS1_SIZE 0x0001
#define UNSLCGCTL9438_3D_GT_CG3DDISCCS2_LSB 0x0002
#define UNSLCGCTL9438_3D_GT_CG3DDISCCS2_SIZE 0x0001
#define UNSLCGCTL9438_3D_GT_CG3DDISCCS3_LSB 0x0003
#define UNSLCGCTL9438_3D_GT_CG3DDISCCS3_SIZE 0x0001
#define UNSLCGCTL9438_3D_GT_CG3DDISCCS0FE_LSB 0x0004
#define UNSLCGCTL9438_3D_GT_CG3DDISCCS0FE_SIZE 0x0001
#define UNSLCGCTL9438_3D_GT_CG3DDISCCS1FE_LSB 0x0005
#define UNSLCGCTL9438_3D_GT_CG3DDISCCS1FE_SIZE 0x0001
#define UNSLCGCTL9438_3D_GT_CG3DDISCCS2FE_LSB 0x0006
#define UNSLCGCTL9438_3D_GT_CG3DDISCCS2FE_SIZE 0x0001
#define UNSLCGCTL9438_3D_GT_CG3DDISCCS3FE_LSB 0x0007
#define UNSLCGCTL9438_3D_GT_CG3DDISCCS3FE_SIZE 0x0001
#define UNSLCGCTL9438_3D_GT_CG3DDISGUC_LSB 0x0008
#define UNSLCGCTL9438_3D_GT_CG3DDISGUC_SIZE 0x0001
#define UNSLCGCTL9438_3D_GT_CG3DDISCCS0BE_LSB 0x0014
#define UNSLCGCTL9438_3D_GT_CG3DDISCCS0BE_SIZE 0x0001
#define UNSLCGCTL9438_3D_GT_CG3DDISCCS1BE_LSB 0x0015
#define UNSLCGCTL9438_3D_GT_CG3DDISCCS1BE_SIZE 0x0001
#define UNSLCGCTL9438_3D_GT_CG3DDISCCS2BE_LSB 0x0016
#define UNSLCGCTL9438_3D_GT_CG3DDISCCS2BE_SIZE 0x0001
#define UNSLCGCTL9438_3D_GT_CG3DDISCCS3BE_LSB 0x0017
#define UNSLCGCTL9438_3D_GT_CG3DDISCCS3BE_SIZE 0x0001
#define UNSLCGCTL9438_3D_GT_CG3DDISMGSX_LSB 0x0018
#define UNSLCGCTL9438_3D_GT_CG3DDISMGSX_SIZE 0x0001
#define UNSLCGCTL9438_3D_GT_CG3DDISGAMXBL_LSB 0x0019
#define UNSLCGCTL9438_3D_GT_CG3DDISGAMXBL_SIZE 0x0001
#define UNSLCGCTL9438_3D_GT_CG3DDISLNE_LSB 0x001a
#define UNSLCGCTL9438_3D_GT_CG3DDISLNE_SIZE 0x0001
#define UNSLCGCTL9438_3D_GT_CG3DDISMSQCW_LSB 0x001d
#define UNSLCGCTL9438_3D_GT_CG3DDISMSQCW_SIZE 0x0001
#define UNSLCGCTL9438_3D_GT_CG3DDISMSQCR_LSB 0x001e
#define UNSLCGCTL9438_3D_GT_CG3DDISMSQCR_SIZE 0x0001
#define UNSLCGCTL9438_3D_GT_CG3DDISMAXF_LSB 0x001f
#define UNSLCGCTL9438_3D_GT_CG3DDISMAXF_SIZE 0x0001

/** UNSLCGCTL9440_3D desc:
  */

#define UNSLCGCTL9440_3D_GT_REG          (0x00009440U)

typedef union {
  struct {
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[00:00] */
                            /* Bits[0:0], default=0*/
    UINT32 cg3ddisramdft : 1; /**< ramdft Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[02:02] */
                            /* Bits[2:2], default=0*/
    UINT32 cg3ddisl3_cr : 1; /**< L3_CR Clock Gating Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[04:04] */
                            /* Bits[4:4], default=0*/
    UINT32 cg3ddisgamwkrs : 1; /**< gamwkrs Clock Gating Disable */
                            /* Bits[5:5], default=0x00000001*/
    UINT32 rsvd_3 : 2; /**< Undefined - auto filled rsvd_[07:06] */
                            /* Bits[7:6], default=0*/
    UINT32 cg3ddisgamctrl : 1; /**< gamctrl Clock Gating Disable */
                            /* Bits[8:8], default=0x00000001*/
    UINT32 rsvd_4 : 3; /**< Undefined - auto filled rsvd_[11:09] */
                            /* Bits[11:9], default=0*/
    UINT32 cg3ddisgamstlb : 1; /**< gamstlb Clock Gating Disable */
                            /* Bits[12:12], default=0x00000001*/
    UINT32 cg3ddisgamtrtlb : 1; /**< gamtrtlb Clock Gating Disable */
                            /* Bits[13:13], default=0x00000001*/
    UINT32 rsvd_5 : 5; /**< Undefined - auto filled rsvd_[18:14] */
                            /* Bits[18:14], default=0*/
    UINT32 cg3ddis_spare2 : 1; /**< SPARE Clock Gating Disable1 */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 cg3ddis_spare3 : 1; /**< SPARE Clock Gating Disable2 */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 rsvd_6 : 2; /**< Undefined - auto filled rsvd_[22:21] */
                            /* Bits[22:21], default=0*/
    UINT32 cg3ddisslsarb : 1; /**< SLSARB Clock Gating Disable */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 cg3ddislniccomp : 1; /**< LNICCOMP Clock Gating Disable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 cg3ddislsnlite : 1; /**< LSNLITE Clock Gating Disable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 cg3ddislnilite : 1; /**< LNILITE Clock Gating Disable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 cg3ddislnelite : 1; /**< LNELITE Clock Gating Disable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 cg3ddislneplite : 1; /**< LNEPLITE Clock Gating Disable */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 cg3ddisrxbar : 1; /**< RXBAR Clock Gating Disable */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 ramcgdis_rxbar : 1; /**< RAM_RXBAR Clock Gating Disable */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 cgacgdis_fixfunc_gti : 1; /**< FIX FUNC GTI Arch Clock Gating Disable */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UNSLCGCTL9440_3D_GT_STRUCT;

#define UNSLCGCTL9440_3D_GT_WIDTH 32
#define UNSLCGCTL9440_3D_GT_DEFAULT 0x00003120U
#define UNSLCGCTL9440_3D_GT_ENDPOINT GT

#define UNSLCGCTL9440_3D_GT_CG3DDISRAMDFT_LSB 0x0001
#define UNSLCGCTL9440_3D_GT_CG3DDISRAMDFT_SIZE 0x0001
#define UNSLCGCTL9440_3D_GT_CG3DDISL3_CR_LSB 0x0003
#define UNSLCGCTL9440_3D_GT_CG3DDISL3_CR_SIZE 0x0001
#define UNSLCGCTL9440_3D_GT_CG3DDISGAMWKRS_LSB 0x0005
#define UNSLCGCTL9440_3D_GT_CG3DDISGAMWKRS_SIZE 0x0001
#define UNSLCGCTL9440_3D_GT_CG3DDISGAMCTRL_LSB 0x0008
#define UNSLCGCTL9440_3D_GT_CG3DDISGAMCTRL_SIZE 0x0001
#define UNSLCGCTL9440_3D_GT_CG3DDISGAMSTLB_LSB 0x000c
#define UNSLCGCTL9440_3D_GT_CG3DDISGAMSTLB_SIZE 0x0001
#define UNSLCGCTL9440_3D_GT_CG3DDISGAMTRTLB_LSB 0x000d
#define UNSLCGCTL9440_3D_GT_CG3DDISGAMTRTLB_SIZE 0x0001
#define UNSLCGCTL9440_3D_GT_CG3DDIS_SPARE2_LSB 0x0013
#define UNSLCGCTL9440_3D_GT_CG3DDIS_SPARE2_SIZE 0x0001
#define UNSLCGCTL9440_3D_GT_CG3DDIS_SPARE3_LSB 0x0014
#define UNSLCGCTL9440_3D_GT_CG3DDIS_SPARE3_SIZE 0x0001
#define UNSLCGCTL9440_3D_GT_CG3DDISSLSARB_LSB 0x0017
#define UNSLCGCTL9440_3D_GT_CG3DDISSLSARB_SIZE 0x0001
#define UNSLCGCTL9440_3D_GT_CG3DDISLNICCOMP_LSB 0x0018
#define UNSLCGCTL9440_3D_GT_CG3DDISLNICCOMP_SIZE 0x0001
#define UNSLCGCTL9440_3D_GT_CG3DDISLSNLITE_LSB 0x0019
#define UNSLCGCTL9440_3D_GT_CG3DDISLSNLITE_SIZE 0x0001
#define UNSLCGCTL9440_3D_GT_CG3DDISLNILITE_LSB 0x001a
#define UNSLCGCTL9440_3D_GT_CG3DDISLNILITE_SIZE 0x0001
#define UNSLCGCTL9440_3D_GT_CG3DDISLNELITE_LSB 0x001b
#define UNSLCGCTL9440_3D_GT_CG3DDISLNELITE_SIZE 0x0001
#define UNSLCGCTL9440_3D_GT_CG3DDISLNEPLITE_LSB 0x001c
#define UNSLCGCTL9440_3D_GT_CG3DDISLNEPLITE_SIZE 0x0001
#define UNSLCGCTL9440_3D_GT_CG3DDISRXBAR_LSB 0x001d
#define UNSLCGCTL9440_3D_GT_CG3DDISRXBAR_SIZE 0x0001
#define UNSLCGCTL9440_3D_GT_RAMCGDIS_RXBAR_LSB 0x001e
#define UNSLCGCTL9440_3D_GT_RAMCGDIS_RXBAR_SIZE 0x0001
#define UNSLCGCTL9440_3D_GT_CGACGDIS_FIXFUNC_GTI_LSB 0x001f
#define UNSLCGCTL9440_3D_GT_CGACGDIS_FIXFUNC_GTI_SIZE 0x0001

/** UNSLCGCTL9444_3D desc:
  */

#define UNSLCGCTL9444_3D_GT_REG          (0x00009444U)

typedef union {
  struct {
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[00:00] */
                            /* Bits[0:0], default=0*/
    UINT32 cg3ddislsn : 1; /**< lsnunit Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 cg3ddisrcu : 1; /**< rcuunit Clock Gating Disable */
                            /* Bits[2:2], default=0x00000001*/
    UINT32 cg3ddis_kcr : 1; /**< KCRunit Clock Gating Disable */
                            /* Bits[3:3], default=0x00000001*/
    UINT32 cg3ddiscsc : 1; /**< CSCunit Clock Gating Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 cg3ddislc : 1; /**< LCunit Clock Gating Disable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 cg3ddisld : 1; /**< LDunit Clock Gating Disable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 cg3ddispsmi : 1; /**< PSMIunit Clock Gating Disable */
                            /* Bits[7:7], default=0x00000001*/
    UINT32 cg3ddissmcr : 1; /**< SMCR Clock Gating Disable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 cg3ddisltcdt : 1; /**< LTCDT Clock Gating Disable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 cg3ddisltcdd : 1; /**< LTCDD Clock Gating Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 cg3ddisltcc : 1; /**< LTCC Clock Gating Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 cg3ddislsqd : 1; /**< LSQD Clock Gating Disable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 cg3ddislsqc : 1; /**< LSQC Clock Gating Disable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 cg3ddislbi : 1; /**< LBI Clock Gating Disable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 cg3ddislbcf : 1; /**< LBCF Clock Gating Disable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 rsvd_1 : 16; /**< Undefined - auto filled rsvd_[31:16] */
                            /* Bits[31:16], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UNSLCGCTL9444_3D_GT_STRUCT;

#define UNSLCGCTL9444_3D_GT_WIDTH 32
#define UNSLCGCTL9444_3D_GT_DEFAULT 0x0000008cU
#define UNSLCGCTL9444_3D_GT_ENDPOINT GT

#define UNSLCGCTL9444_3D_GT_CG3DDISLSN_LSB 0x0001
#define UNSLCGCTL9444_3D_GT_CG3DDISLSN_SIZE 0x0001
#define UNSLCGCTL9444_3D_GT_CG3DDISRCU_LSB 0x0002
#define UNSLCGCTL9444_3D_GT_CG3DDISRCU_SIZE 0x0001
#define UNSLCGCTL9444_3D_GT_CG3DDIS_KCR_LSB 0x0003
#define UNSLCGCTL9444_3D_GT_CG3DDIS_KCR_SIZE 0x0001
#define UNSLCGCTL9444_3D_GT_CG3DDISCSC_LSB 0x0004
#define UNSLCGCTL9444_3D_GT_CG3DDISCSC_SIZE 0x0001
#define UNSLCGCTL9444_3D_GT_CG3DDISLC_LSB 0x0005
#define UNSLCGCTL9444_3D_GT_CG3DDISLC_SIZE 0x0001
#define UNSLCGCTL9444_3D_GT_CG3DDISLD_LSB 0x0006
#define UNSLCGCTL9444_3D_GT_CG3DDISLD_SIZE 0x0001
#define UNSLCGCTL9444_3D_GT_CG3DDISPSMI_LSB 0x0007
#define UNSLCGCTL9444_3D_GT_CG3DDISPSMI_SIZE 0x0001
#define UNSLCGCTL9444_3D_GT_CG3DDISSMCR_LSB 0x0008
#define UNSLCGCTL9444_3D_GT_CG3DDISSMCR_SIZE 0x0001
#define UNSLCGCTL9444_3D_GT_CG3DDISLTCDT_LSB 0x0009
#define UNSLCGCTL9444_3D_GT_CG3DDISLTCDT_SIZE 0x0001
#define UNSLCGCTL9444_3D_GT_CG3DDISLTCDD_LSB 0x000a
#define UNSLCGCTL9444_3D_GT_CG3DDISLTCDD_SIZE 0x0001
#define UNSLCGCTL9444_3D_GT_CG3DDISLTCC_LSB 0x000b
#define UNSLCGCTL9444_3D_GT_CG3DDISLTCC_SIZE 0x0001
#define UNSLCGCTL9444_3D_GT_CG3DDISLSQD_LSB 0x000c
#define UNSLCGCTL9444_3D_GT_CG3DDISLSQD_SIZE 0x0001
#define UNSLCGCTL9444_3D_GT_CG3DDISLSQC_LSB 0x000d
#define UNSLCGCTL9444_3D_GT_CG3DDISLSQC_SIZE 0x0001
#define UNSLCGCTL9444_3D_GT_CG3DDISLBI_LSB 0x000e
#define UNSLCGCTL9444_3D_GT_CG3DDISLBI_SIZE 0x0001
#define UNSLCGCTL9444_3D_GT_CG3DDISLBCF_LSB 0x000f
#define UNSLCGCTL9444_3D_GT_CG3DDISLBCF_SIZE 0x0001

#ifdef SV_HOOKS

/** UNSLCGCTL9448_3D desc:
  */

#define UNSLCGCTL9448_3D_GT_REG          (0x00009448U)

typedef union {
  struct {
    UINT32 spare : 32; /**< ECO Spare Bits */
                            /* Bits[31:0], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UNSLCGCTL9448_3D_GT_STRUCT;

#define UNSLCGCTL9448_3D_GT_WIDTH 32
#define UNSLCGCTL9448_3D_GT_DEFAULT 0x00000000U
#define UNSLCGCTL9448_3D_GT_ENDPOINT GT

#define UNSLCGCTL9448_3D_GT_SPARE_LSB 0x0000
#define UNSLCGCTL9448_3D_GT_SPARE_SIZE 0x0020

#endif      // SV_HOOKS

/** SCCGCTL94D0_3D desc:
  */

#define SCCGCTL94D0_3D_GT_REG            (0x000094d0U)

typedef union {
  struct {
    UINT32 cg3ddissmcr : 1; /**< SMCRunit Clock Gating Disable */
                            /* Bits[0:0], default=0x00000001*/
    UINT32 cg3ddisgcp : 1; /**< GCPunit Clock Gating Disable */
                            /* Bits[1:1], default=0x00000001*/
    UINT32 rsvd_0 : 30; /**< Undefined - auto filled rsvd_[31:02] */
                            /* Bits[31:2], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} SCCGCTL94D0_3D_GT_STRUCT;

#define SCCGCTL94D0_3D_GT_WIDTH 32
#define SCCGCTL94D0_3D_GT_DEFAULT 0x00000003U
#define SCCGCTL94D0_3D_GT_ENDPOINT GT

#define SCCGCTL94D0_3D_GT_CG3DDISSMCR_LSB 0x0000
#define SCCGCTL94D0_3D_GT_CG3DDISSMCR_SIZE 0x0001
#define SCCGCTL94D0_3D_GT_CG3DDISGCP_LSB 0x0001
#define SCCGCTL94D0_3D_GT_CG3DDISGCP_SIZE 0x0001

/** SCCGCTL94D4_3D desc:
  */

#define SCCGCTL94D4_3D_GT_REG            (0x000094d4U)

typedef union {
  struct {
    UINT32 cg3ddiswmfe : 1; /**< WMFEunit Clock Gating Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cg3ddiswmbe : 1; /**< WMBE Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 cg3ddissvl : 1; /**< SVLunit Clock Gating Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 cg3ddisstc : 1; /**< STCunit Clock Gating Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 cg3ddissbe : 1; /**< SBEunit Clock Gating Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 cg3ddissarb : 1; /**< Sarbunit Clock Gating Disable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 cg3ddisrcz : 1; /**< RCZunit Clock Gating Disable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[07:07] */
                            /* Bits[7:7], default=0*/
    UINT32 cg3ddisoascrep : 1; /**< OASCREP Clock Gating Disable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 cg3ddisoaaddr : 1; /**< OAADDRunit Clock Gating Disable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 cg3ddismsc : 1; /**< MSCunit Clock Gating Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 cg3ddisnodex : 1; /**< NODEX Clock Gating Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 cg3ddisnodedss : 1; /**< NODEDSS Clock Gating Disable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 cg3ddisslcfg : 1; /**< SLCFG Clock Gating Disable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 cg3ddisrru : 1; /**< RRU Clock Gating Disable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 rsvd_1 : 3; /**< Undefined - auto filled rsvd_[17:15] */
                            /* Bits[17:15], default=0*/
    UINT32 cg3ddisiz : 1; /**< IZunit Clock Gating Disable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 cg3ddishiz : 1; /**< HIZunit Clock Gating Disable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 cg3ddiszarb : 1; /**< zarbunit Clock Gating Disable */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 cg3ddisgapz : 1; /**< GAPZunit Clock Gating Disable */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 cg3ddisgaps : 1; /**< GAPSunit Clock Gating Disable */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 cg3ddisgapl3 : 1; /**< GAPL3unit Clock Gating Disable */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 cg3ddisgapc : 1; /**< GAPCunit Clock Gating Disable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 cg3ddisgahs : 1; /**< GAHSunit Clock Gating Disable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 cg3ddisgafsrrb : 1; /**< GAFSRRB Clock Gating Disable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 cg3ddisgacb : 1; /**< GACBunit Clock Gating Disable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 cg3ddisdap : 1; /**< DAPunit Clock Gating Disable */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 cg3ddiscc : 1; /**< ccunit Clock Gating Disable */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 cg3ddisoaal1 : 1; /**< oaal1unit Clock Gating Disable */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 cg3ddis_spare1 : 1; /**< SPARE Clock Gating Disable */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} SCCGCTL94D4_3D_GT_STRUCT;

#define SCCGCTL94D4_3D_GT_WIDTH 32
#define SCCGCTL94D4_3D_GT_DEFAULT 0x00000000U
#define SCCGCTL94D4_3D_GT_ENDPOINT GT

#define SCCGCTL94D4_3D_GT_CG3DDISWMFE_LSB 0x0000
#define SCCGCTL94D4_3D_GT_CG3DDISWMFE_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISWMBE_LSB 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISWMBE_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISSVL_LSB 0x0002
#define SCCGCTL94D4_3D_GT_CG3DDISSVL_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISSTC_LSB 0x0003
#define SCCGCTL94D4_3D_GT_CG3DDISSTC_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISSBE_LSB 0x0004
#define SCCGCTL94D4_3D_GT_CG3DDISSBE_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISSARB_LSB 0x0005
#define SCCGCTL94D4_3D_GT_CG3DDISSARB_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISRCZ_LSB 0x0006
#define SCCGCTL94D4_3D_GT_CG3DDISRCZ_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISOASCREP_LSB 0x0008
#define SCCGCTL94D4_3D_GT_CG3DDISOASCREP_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISOAADDR_LSB 0x0009
#define SCCGCTL94D4_3D_GT_CG3DDISOAADDR_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISMSC_LSB 0x000a
#define SCCGCTL94D4_3D_GT_CG3DDISMSC_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISNODEX_LSB 0x000b
#define SCCGCTL94D4_3D_GT_CG3DDISNODEX_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISNODEDSS_LSB 0x000c
#define SCCGCTL94D4_3D_GT_CG3DDISNODEDSS_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISSLCFG_LSB 0x000d
#define SCCGCTL94D4_3D_GT_CG3DDISSLCFG_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISRRU_LSB 0x000e
#define SCCGCTL94D4_3D_GT_CG3DDISRRU_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISIZ_LSB 0x0012
#define SCCGCTL94D4_3D_GT_CG3DDISIZ_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISHIZ_LSB 0x0013
#define SCCGCTL94D4_3D_GT_CG3DDISHIZ_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISZARB_LSB 0x0014
#define SCCGCTL94D4_3D_GT_CG3DDISZARB_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISGAPZ_LSB 0x0015
#define SCCGCTL94D4_3D_GT_CG3DDISGAPZ_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISGAPS_LSB 0x0016
#define SCCGCTL94D4_3D_GT_CG3DDISGAPS_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISGAPL3_LSB 0x0017
#define SCCGCTL94D4_3D_GT_CG3DDISGAPL3_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISGAPC_LSB 0x0018
#define SCCGCTL94D4_3D_GT_CG3DDISGAPC_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISGAHS_LSB 0x0019
#define SCCGCTL94D4_3D_GT_CG3DDISGAHS_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISGAFSRRB_LSB 0x001a
#define SCCGCTL94D4_3D_GT_CG3DDISGAFSRRB_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISGACB_LSB 0x001b
#define SCCGCTL94D4_3D_GT_CG3DDISGACB_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISDAP_LSB 0x001c
#define SCCGCTL94D4_3D_GT_CG3DDISDAP_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISCC_LSB 0x001d
#define SCCGCTL94D4_3D_GT_CG3DDISCC_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDISOAAL1_LSB 0x001e
#define SCCGCTL94D4_3D_GT_CG3DDISOAAL1_SIZE 0x0001
#define SCCGCTL94D4_3D_GT_CG3DDIS_SPARE1_LSB 0x001f
#define SCCGCTL94D4_3D_GT_CG3DDIS_SPARE1_SIZE 0x0001

/** SCCGCTL94D8_3D desc:
  */

#define SCCGCTL94D8_3D_GT_REG            (0x000094d8U)

typedef union {
  struct {
    UINT32 cg3ddisnoa : 1; /**< NOA Clock Gating Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cg3ddisramdft : 1; /**< RAMdft Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 cg3ddistdc : 1; /**< TDCunit Clock Gating Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 cg3ddisrcpbe : 1; /**< RCPBEunit Clock Gating Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 cg3ddisrcpb : 1; /**< RCPBunit Clock Gating Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 cg3ddislni : 1; /**< LNIunit Clock Gating Disable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[06:06] */
                            /* Bits[6:6], default=0*/
    UINT32 cg3ddissfbe : 1; /**< SFBEunit Clock Gating Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 rsvd_1 : 2; /**< Undefined - auto filled rsvd_[09:08] */
                            /* Bits[9:8], default=0*/
    UINT32 cg3ddisamfsd : 1; /**< AMFSD unit Clock Gating Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 cg3ddisamfsc : 1; /**< AMFSC unit Clock Gating Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 cg3ddisamfsf : 1; /**< AMFSF unit Clock Gating Disable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 cg3ddiscl : 1; /**< CL unit Clock Gating Disable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 cg3ddisclr : 1; /**< CLR unit Clock Gating Disable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[15:15] */
                            /* Bits[15:15], default=0*/
    UINT32 cg3ddisgafm : 1; /**< GAFM unit Clock Gating Disable */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 cg3ddisgafs : 1; /**< GAFS unit Clock Gating Disable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 cg3ddisgafd : 1; /**< GAFD unit Clock Gating Disable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 rsvd_3 : 3; /**< Undefined - auto filled rsvd_[21:19] */
                            /* Bits[21:19], default=0*/
    UINT32 cg3ddisgafswrblk_vf : 1; /**< GAFSWRBLK unit Clock Gating Disable vf */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 cg3ddisgafswrblk_wmf : 1; /**< GAFSWRBLK unit Clock Gating Disable wmf */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 cg3ddisgs : 1; /**< GS unit Clock Gating Disable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 cg3ddishs : 1; /**< HS unit Clock Gating Disable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 cg3ddisovr : 1; /**< OVR unit Clock Gating Disable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 rsvd_4 : 1; /**< Undefined - auto filled rsvd_[27:27] */
                            /* Bits[27:27], default=0*/
    UINT32 cg3ddisrcu : 1; /**< RCU unit Clock Gating Disable */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 rsvd_5 : 1; /**< Undefined - auto filled rsvd_[29:29] */
                            /* Bits[29:29], default=0*/
    UINT32 cg3ddissf : 1; /**< SF unit Clock Gating Disable */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 cg3ddissfr : 1; /**< SFR unit Clock Gating Disable */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} SCCGCTL94D8_3D_GT_STRUCT;

#define SCCGCTL94D8_3D_GT_WIDTH 32
#define SCCGCTL94D8_3D_GT_DEFAULT 0x00000000U
#define SCCGCTL94D8_3D_GT_ENDPOINT GT

#define SCCGCTL94D8_3D_GT_CG3DDISNOA_LSB 0x0000
#define SCCGCTL94D8_3D_GT_CG3DDISNOA_SIZE 0x0001
#define SCCGCTL94D8_3D_GT_CG3DDISRAMDFT_LSB 0x0001
#define SCCGCTL94D8_3D_GT_CG3DDISRAMDFT_SIZE 0x0001
#define SCCGCTL94D8_3D_GT_CG3DDISTDC_LSB 0x0002
#define SCCGCTL94D8_3D_GT_CG3DDISTDC_SIZE 0x0001
#define SCCGCTL94D8_3D_GT_CG3DDISRCPBE_LSB 0x0003
#define SCCGCTL94D8_3D_GT_CG3DDISRCPBE_SIZE 0x0001
#define SCCGCTL94D8_3D_GT_CG3DDISRCPB_LSB 0x0004
#define SCCGCTL94D8_3D_GT_CG3DDISRCPB_SIZE 0x0001
#define SCCGCTL94D8_3D_GT_CG3DDISLNI_LSB 0x0005
#define SCCGCTL94D8_3D_GT_CG3DDISLNI_SIZE 0x0001
#define SCCGCTL94D8_3D_GT_CG3DDISSFBE_LSB 0x0007
#define SCCGCTL94D8_3D_GT_CG3DDISSFBE_SIZE 0x0001
#define SCCGCTL94D8_3D_GT_CG3DDISAMFSD_LSB 0x000a
#define SCCGCTL94D8_3D_GT_CG3DDISAMFSD_SIZE 0x0001
#define SCCGCTL94D8_3D_GT_CG3DDISAMFSC_LSB 0x000b
#define SCCGCTL94D8_3D_GT_CG3DDISAMFSC_SIZE 0x0001
#define SCCGCTL94D8_3D_GT_CG3DDISAMFSF_LSB 0x000c
#define SCCGCTL94D8_3D_GT_CG3DDISAMFSF_SIZE 0x0001
#define SCCGCTL94D8_3D_GT_CG3DDISCL_LSB 0x000d
#define SCCGCTL94D8_3D_GT_CG3DDISCL_SIZE 0x0001
#define SCCGCTL94D8_3D_GT_CG3DDISCLR_LSB 0x000e
#define SCCGCTL94D8_3D_GT_CG3DDISCLR_SIZE 0x0001
#define SCCGCTL94D8_3D_GT_CG3DDISGAFM_LSB 0x0010
#define SCCGCTL94D8_3D_GT_CG3DDISGAFM_SIZE 0x0001
#define SCCGCTL94D8_3D_GT_CG3DDISGAFS_LSB 0x0011
#define SCCGCTL94D8_3D_GT_CG3DDISGAFS_SIZE 0x0001
#define SCCGCTL94D8_3D_GT_CG3DDISGAFD_LSB 0x0012
#define SCCGCTL94D8_3D_GT_CG3DDISGAFD_SIZE 0x0001
#define SCCGCTL94D8_3D_GT_CG3DDISGAFSWRBLK_VF_LSB 0x0016
#define SCCGCTL94D8_3D_GT_CG3DDISGAFSWRBLK_VF_SIZE 0x0001
#define SCCGCTL94D8_3D_GT_CG3DDISGAFSWRBLK_WMF_LSB 0x0017
#define SCCGCTL94D8_3D_GT_CG3DDISGAFSWRBLK_WMF_SIZE 0x0001
#define SCCGCTL94D8_3D_GT_CG3DDISGS_LSB 0x0018
#define SCCGCTL94D8_3D_GT_CG3DDISGS_SIZE 0x0001
#define SCCGCTL94D8_3D_GT_CG3DDISHS_LSB 0x0019
#define SCCGCTL94D8_3D_GT_CG3DDISHS_SIZE 0x0001
#define SCCGCTL94D8_3D_GT_CG3DDISOVR_LSB 0x001a
#define SCCGCTL94D8_3D_GT_CG3DDISOVR_SIZE 0x0001
#define SCCGCTL94D8_3D_GT_CG3DDISRCU_LSB 0x001c
#define SCCGCTL94D8_3D_GT_CG3DDISRCU_SIZE 0x0001
#define SCCGCTL94D8_3D_GT_CG3DDISSF_LSB 0x001e
#define SCCGCTL94D8_3D_GT_CG3DDISSF_SIZE 0x0001
#define SCCGCTL94D8_3D_GT_CG3DDISSFR_LSB 0x001f
#define SCCGCTL94D8_3D_GT_CG3DDISSFR_SIZE 0x0001


/** SCCGCTL94DC_3D desc:
  */

#define SCCGCTL94DC_3D_GT_REG            (0x000094dcU)

typedef union {
  struct {
    UINT32 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[01:00] */
                            /* Bits[1:0], default=0*/
    UINT32 cgacgdis_nodex : 1; /**< SliceNode Arch Clock Gating Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[03:03] */
                            /* Bits[3:3], default=0*/
    UINT32 cgacgdis_rt : 1; /**< RayTrace Arch Clock Gating Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[05:05] */
                            /* Bits[5:5], default=0*/
    UINT32 cgacgdis_ss_pix_pipe : 1; /**< SS PixPipe Arch Clock Gating Disable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 cgacgdis_pix_pipe : 1; /**< PixPipe Arch Clock Gating Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 cgacgdis_raster_xbar : 1; /**< RastXbar Arch Clock Gating Disable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 cgacgdis_raster_geom : 1; /**< RastGeom Arch Clock Gating Disable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 cgacgdis_z : 1; /**< Z Arch Clock Gating Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 cgacgdis_fixfunc : 1; /**< FF Arch Clock Gating Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 rsvd_3 : 2; /**< Undefined - auto filled rsvd_[13:12] */
                            /* Bits[13:12], default=0*/
    UINT32 cg3ddisurb : 1; /**< URBUNIT Clock Gating Disable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 rsvd_4 : 17; /**< Undefined - auto filled rsvd_[31:15] */
                            /* Bits[31:15], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} SCCGCTL94DC_3D_GT_STRUCT;

#define SCCGCTL94DC_3D_GT_WIDTH 32
#define SCCGCTL94DC_3D_GT_DEFAULT 0x00000000U
#define SCCGCTL94DC_3D_GT_ENDPOINT GT

#define SCCGCTL94DC_3D_GT_CGACGDIS_NODEX_LSB 0x0002
#define SCCGCTL94DC_3D_GT_CGACGDIS_NODEX_SIZE 0x0001
#define SCCGCTL94DC_3D_GT_CGACGDIS_RT_LSB 0x0004
#define SCCGCTL94DC_3D_GT_CGACGDIS_RT_SIZE 0x0001
#define SCCGCTL94DC_3D_GT_CGACGDIS_SS_PIX_PIPE_LSB 0x0006
#define SCCGCTL94DC_3D_GT_CGACGDIS_SS_PIX_PIPE_SIZE 0x0001
#define SCCGCTL94DC_3D_GT_CGACGDIS_PIX_PIPE_LSB 0x0007
#define SCCGCTL94DC_3D_GT_CGACGDIS_PIX_PIPE_SIZE 0x0001
#define SCCGCTL94DC_3D_GT_CGACGDIS_RASTER_XBAR_LSB 0x0008
#define SCCGCTL94DC_3D_GT_CGACGDIS_RASTER_XBAR_SIZE 0x0001
#define SCCGCTL94DC_3D_GT_CGACGDIS_RASTER_GEOM_LSB 0x0009
#define SCCGCTL94DC_3D_GT_CGACGDIS_RASTER_GEOM_SIZE 0x0001
#define SCCGCTL94DC_3D_GT_CGACGDIS_Z_LSB 0x000a
#define SCCGCTL94DC_3D_GT_CGACGDIS_Z_SIZE 0x0001
#define SCCGCTL94DC_3D_GT_CGACGDIS_FIXFUNC_LSB 0x000b
#define SCCGCTL94DC_3D_GT_CGACGDIS_FIXFUNC_SIZE 0x0001
#define SCCGCTL94DC_3D_GT_CG3DDISURB_LSB 0x000e
#define SCCGCTL94DC_3D_GT_CG3DDISURB_SIZE 0x0001

/** SCCGCTL94E0_3D desc:
  */

#define SCCGCTL94E0_3D_GT_REG            (0x000094e0U)

typedef union {
  struct {
    UINT32 ramcgdis_wmfe : 1; /**< WMFEunit Clock Gating Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 ramcgdis_svl : 1; /**< SVLunit Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 ramcgdis_stc : 1; /**< STCunit Clock Gating Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 ramcgdis_sbe : 1; /**< SBEunit Clock Gating Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 ramcgdis_rcz : 1; /**< RCZunit Clock Gating Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[05:05] */
                            /* Bits[5:5], default=0*/
    UINT32 ramcgdis_iz : 1; /**< IZunit Clock Gating Disable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 ramcgdis_hiz : 1; /**< HIZunit Clock Gating Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[08:08] */
                            /* Bits[8:8], default=0*/
    UINT32 ramcgdis_avs : 1; /**< AVSunit Clock Gating Disable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 ramcgdis_rcpfe : 1; /**< RCPFEunit Clock Gating Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 ramcgdis_msc : 1; /**< MSCunit Clock Gating Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 ramcgdis_bc : 1; /**< BCunit Clock Gating Disable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 ramcgdis_psd : 1; /**< PSDunit Clock Gating Disable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 ramcgdis_cpss : 1; /**< CPSSunit Clock Gating Disable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 ramcgdis_spare1 : 1; /**< SPARE Clock Gating Disable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 ramcgdis_cs : 1; /**< CS Clock Gating Disable */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 ramcgdis_gs : 1; /**< GS Clock Gating Disable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 ramcgdis_hs : 1; /**< HS Clock Gating Disable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 ramcgdis_ovr : 1; /**< OVR Clock Gating Disable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 rsvd_2 : 2; /**< Undefined - auto filled rsvd_[21:20] */
                            /* Bits[21:20], default=0*/
    UINT32 ramcgdis_sol : 1; /**< SOL Clock Gating Disable */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 ramcgdis_svg : 1; /**< SVG Clock Gating Disable */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 ramcgdis_svgr : 1; /**< SVGR Clock Gating Disable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 ramcgdis_tds : 1; /**< TDS Clock Gating Disable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 ramcgdis_vf : 1; /**< VF Clock Gating Disable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 ramcgdis_vfr : 1; /**< VFR Clock Gating Disable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 ramcgdis_vs : 1; /**< VS Clock Gating Disable */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 ramcgdis_vsr : 1; /**< VSR Clock Gating Disable */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 ramcgdis_pss : 1; /**< PSS Clock Gating Disable */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 rsvd_3 : 1; /**< Undefined - auto filled rsvd_[31:31] */
                            /* Bits[31:31], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} SCCGCTL94E0_3D_GT_STRUCT;

#define SCCGCTL94E0_3D_GT_WIDTH 32
#define SCCGCTL94E0_3D_GT_DEFAULT 0x00000000U
#define SCCGCTL94E0_3D_GT_ENDPOINT GT

#define SCCGCTL94E0_3D_GT_RAMCGDIS_WMFE_LSB 0x0000
#define SCCGCTL94E0_3D_GT_RAMCGDIS_WMFE_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_SVL_LSB 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_SVL_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_STC_LSB 0x0002
#define SCCGCTL94E0_3D_GT_RAMCGDIS_STC_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_SBE_LSB 0x0003
#define SCCGCTL94E0_3D_GT_RAMCGDIS_SBE_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_RCZ_LSB 0x0004
#define SCCGCTL94E0_3D_GT_RAMCGDIS_RCZ_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_IZ_LSB 0x0006
#define SCCGCTL94E0_3D_GT_RAMCGDIS_IZ_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_HIZ_LSB 0x0007
#define SCCGCTL94E0_3D_GT_RAMCGDIS_HIZ_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_AVS_LSB 0x0009
#define SCCGCTL94E0_3D_GT_RAMCGDIS_AVS_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_RCPFE_LSB 0x000a
#define SCCGCTL94E0_3D_GT_RAMCGDIS_RCPFE_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_MSC_LSB 0x000b
#define SCCGCTL94E0_3D_GT_RAMCGDIS_MSC_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_BC_LSB 0x000c
#define SCCGCTL94E0_3D_GT_RAMCGDIS_BC_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_PSD_LSB 0x000d
#define SCCGCTL94E0_3D_GT_RAMCGDIS_PSD_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_CPSS_LSB 0x000e
#define SCCGCTL94E0_3D_GT_RAMCGDIS_CPSS_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_SPARE1_LSB 0x000f
#define SCCGCTL94E0_3D_GT_RAMCGDIS_SPARE1_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_CS_LSB 0x0010
#define SCCGCTL94E0_3D_GT_RAMCGDIS_CS_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_GS_LSB 0x0011
#define SCCGCTL94E0_3D_GT_RAMCGDIS_GS_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_HS_LSB 0x0012
#define SCCGCTL94E0_3D_GT_RAMCGDIS_HS_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_OVR_LSB 0x0013
#define SCCGCTL94E0_3D_GT_RAMCGDIS_OVR_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_SOL_LSB 0x0016
#define SCCGCTL94E0_3D_GT_RAMCGDIS_SOL_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_SVG_LSB 0x0017
#define SCCGCTL94E0_3D_GT_RAMCGDIS_SVG_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_SVGR_LSB 0x0018
#define SCCGCTL94E0_3D_GT_RAMCGDIS_SVGR_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_TDS_LSB 0x0019
#define SCCGCTL94E0_3D_GT_RAMCGDIS_TDS_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_VF_LSB 0x001a
#define SCCGCTL94E0_3D_GT_RAMCGDIS_VF_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_VFR_LSB 0x001b
#define SCCGCTL94E0_3D_GT_RAMCGDIS_VFR_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_VS_LSB 0x001c
#define SCCGCTL94E0_3D_GT_RAMCGDIS_VS_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_VSR_LSB 0x001d
#define SCCGCTL94E0_3D_GT_RAMCGDIS_VSR_SIZE 0x0001
#define SCCGCTL94E0_3D_GT_RAMCGDIS_PSS_LSB 0x001e
#define SCCGCTL94E0_3D_GT_RAMCGDIS_PSS_SIZE 0x0001

/** SCCGCTL94E4_3D desc:
  */

#define SCCGCTL94E4_3D_GT_REG            (0x000094e4U)

typedef union {
  struct {
    UINT32 cg3ddistdp : 1; /**< tdpunit Clock Gating Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cg3ddisbc : 1; /**< sbc Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 cg3ddisbesbuf : 1; /**< besbufunit Clock Gating Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 cg3ddiscpss : 1; /**< cpssunit Clock Gating Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 cg3ddishdc : 1; /**< hdcunit Clock Gating Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 cg3ddispsd : 1; /**< psdunit Clock Gating Disable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 cg3ddisgw : 1; /**< gwunit Clock Gating Disable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 cg3ddissol : 1; /**< SOL unit Clock Gating Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 cg3ddissvg : 1; /**< SVG unit Clock Gating Disable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 cg3ddissvgr : 1; /**< SVGR unit Clock Gating Disable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 cg3ddistdg : 1; /**< TDG unit Clock Gating Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 cg3ddistds : 1; /**< TDS unit Clock Gating Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 cg3ddiste : 1; /**< TE unit Clock Gating Disable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 cg3ddistetg : 1; /**< TETG unit Clock Gating Disable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 cg3ddistsg : 1; /**< TSG unit Clock Gating Disable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 cg3ddisurbm : 1; /**< URBM unit Clock Gating Disable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 cg3ddisvf : 1; /**< VF unit Clock Gating Disable */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 cg3ddisvfe : 1; /**< VFE Clock Gating Disable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 cg3ddisvfr : 1; /**< VFR unit Clock Gating Disable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 cg3ddisvs : 1; /**< VS unit Clock Gating Disable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 cg3ddisvsr : 1; /**< VSR unit Clock Gating Disable */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 cg3ddisgafarb : 1; /**< GAFARB unit Clock Gating Disable */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 cg3ddiszsc_compute : 1; /**< ZSCCOMPUTE unit Clock Gating Disable */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 cg3ddiszsc_bank : 1; /**< ZSCBANK unit Clock Gating Disable */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 cg3ddiszpbe : 1; /**< ZPBE unit Clock Gating Disable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 cg3ddiscarb : 1; /**< CARB unit Clock Gating Disable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 cg3ddispss : 1; /**< PSS unit Clock Gating Disable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 cg3ddissvlsdfifo : 1; /**< sdfifo Clock Gating Disable svl */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 cg3ddissvgsdfifo : 1; /**< sdfifo Clock Gating Disable svg */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[29:29] */
                            /* Bits[29:29], default=0*/
    UINT32 cg3ddispcpr : 1; /**< PCPR Clock Gating Disable */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 cg3ddispcp : 1; /**< PCP Clock Gating Disable */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} SCCGCTL94E4_3D_GT_STRUCT;

#define SCCGCTL94E4_3D_GT_WIDTH 32
#define SCCGCTL94E4_3D_GT_DEFAULT 0x00000000U
#define SCCGCTL94E4_3D_GT_ENDPOINT GT

#define SCCGCTL94E4_3D_GT_CG3DDISTDP_LSB 0x0000
#define SCCGCTL94E4_3D_GT_CG3DDISTDP_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISBC_LSB 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISBC_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISBESBUF_LSB 0x0002
#define SCCGCTL94E4_3D_GT_CG3DDISBESBUF_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISCPSS_LSB 0x0003
#define SCCGCTL94E4_3D_GT_CG3DDISCPSS_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISHDC_LSB 0x0004
#define SCCGCTL94E4_3D_GT_CG3DDISHDC_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISPSD_LSB 0x0005
#define SCCGCTL94E4_3D_GT_CG3DDISPSD_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISGW_LSB 0x0006
#define SCCGCTL94E4_3D_GT_CG3DDISGW_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISSOL_LSB 0x0007
#define SCCGCTL94E4_3D_GT_CG3DDISSOL_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISSVG_LSB 0x0008
#define SCCGCTL94E4_3D_GT_CG3DDISSVG_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISSVGR_LSB 0x0009
#define SCCGCTL94E4_3D_GT_CG3DDISSVGR_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISTDG_LSB 0x000a
#define SCCGCTL94E4_3D_GT_CG3DDISTDG_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISTDS_LSB 0x000b
#define SCCGCTL94E4_3D_GT_CG3DDISTDS_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISTE_LSB 0x000c
#define SCCGCTL94E4_3D_GT_CG3DDISTE_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISTETG_LSB 0x000d
#define SCCGCTL94E4_3D_GT_CG3DDISTETG_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISTSG_LSB 0x000e
#define SCCGCTL94E4_3D_GT_CG3DDISTSG_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISURBM_LSB 0x000f
#define SCCGCTL94E4_3D_GT_CG3DDISURBM_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISVF_LSB 0x0010
#define SCCGCTL94E4_3D_GT_CG3DDISVF_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISVFE_LSB 0x0011
#define SCCGCTL94E4_3D_GT_CG3DDISVFE_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISVFR_LSB 0x0012
#define SCCGCTL94E4_3D_GT_CG3DDISVFR_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISVS_LSB 0x0013
#define SCCGCTL94E4_3D_GT_CG3DDISVS_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISVSR_LSB 0x0014
#define SCCGCTL94E4_3D_GT_CG3DDISVSR_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISGAFARB_LSB 0x0015
#define SCCGCTL94E4_3D_GT_CG3DDISGAFARB_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISZSC_COMPUTE_LSB 0x0016
#define SCCGCTL94E4_3D_GT_CG3DDISZSC_COMPUTE_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISZSC_BANK_LSB 0x0017
#define SCCGCTL94E4_3D_GT_CG3DDISZSC_BANK_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISZPBE_LSB 0x0018
#define SCCGCTL94E4_3D_GT_CG3DDISZPBE_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISCARB_LSB 0x0019
#define SCCGCTL94E4_3D_GT_CG3DDISCARB_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISPSS_LSB 0x001a
#define SCCGCTL94E4_3D_GT_CG3DDISPSS_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISSVLSDFIFO_LSB 0x001b
#define SCCGCTL94E4_3D_GT_CG3DDISSVLSDFIFO_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISSVGSDFIFO_LSB 0x001c
#define SCCGCTL94E4_3D_GT_CG3DDISSVGSDFIFO_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISPCPR_LSB 0x001e
#define SCCGCTL94E4_3D_GT_CG3DDISPCPR_SIZE 0x0001
#define SCCGCTL94E4_3D_GT_CG3DDISPCP_LSB 0x001f
#define SCCGCTL94E4_3D_GT_CG3DDISPCP_SIZE 0x0001

/** SSMCGCTL9520_3D desc:
  */

#define SSMCGCTL9520_3D_GT_REG           (0x00009520U)

typedef union {
  struct {
    UINT32 cg3ddissmcr : 1; /**< SMCRunit Clock Gating Disable */
                            /* Bits[0:0], default=0x00000001*/
    UINT32 cg3ddisgcp : 1; /**< GCPunit Clock Gating Disable */
                            /* Bits[1:1], default=0x00000001*/
    UINT32 rsvd_0 : 30; /**< Undefined - auto filled rsvd_[31:02] */
                            /* Bits[31:2], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} SSMCGCTL9520_3D_GT_STRUCT;

#define SSMCGCTL9520_3D_GT_WIDTH 32
#define SSMCGCTL9520_3D_GT_DEFAULT 0x00000003U
#define SSMCGCTL9520_3D_GT_ENDPOINT GT

#define SSMCGCTL9520_3D_GT_CG3DDISSMCR_LSB 0x0000
#define SSMCGCTL9520_3D_GT_CG3DDISSMCR_SIZE 0x0001
#define SSMCGCTL9520_3D_GT_CG3DDISGCP_LSB 0x0001
#define SSMCGCTL9520_3D_GT_CG3DDISGCP_SIZE 0x0001

/** SSMCGCTL9524_3D desc:
  */

#define SSMCGCTL9524_3D_GT_REG           (0x00009524U)

typedef union {
  struct {
    UINT32 cg3ddisso : 1; /**< SOunit Clock Gating Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cg3ddissi : 1; /**< SIunit Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 cg3ddissc : 1; /**< SCunit Clock Gating Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 cg3ddisramdft : 1; /**< RAMDFT Clock Gating Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 cg3ddispsd : 1; /**< PSDunit Clock Gating Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 cg3ddispl : 1; /**< PLunit Clock Gating Disable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 cg3ddisoatrep : 1; /**< OATREPunit Clock Gating Disable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 cg3ddismt : 1; /**< MTunit Clock Gating Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 cg3ddisma : 1; /**< MAunit Clock Gating Disable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 cg3ddisime : 1; /**< IMEunit's Clock Gating Disable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[10:10] */
                            /* Bits[10:10], default=0*/
    UINT32 cg3ddisic : 1; /**< ICunit Clock Gating Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 rsvd_1 : 3; /**< Undefined - auto filled rsvd_[14:12] */
                            /* Bits[14:12], default=0*/
    UINT32 cg3ddisvmemreqarb : 1; /**< VMEMREQARB Clock Gating Disable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 cg3ddisgw : 1; /**< GWunit Clock Gating Disable */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 cg3ddisvmemsoarb : 1; /**< VMEMSOARB Clock Gating Disable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 cg3ddisft : 1; /**< Ftunit Clock Gating Disable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 cg3ddis_dpas_eu : 1; /**< DPAS_EU Clock Gating Disable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 cg3ddisfl : 1; /**< FLunit Clock Gating Disable */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 rsvd_2 : 2; /**< Undefined - auto filled rsvd_[22:21] */
                            /* Bits[22:21], default=0*/
    UINT32 cg3ddisdm : 1; /**< DMunit Clock Gating Disable */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 cg3ddisdg : 1; /**< DGunit Clock Gating Disable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 cg3ddiscre : 1; /**< CREunit Clock Gating Disable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 cg3ddisgadss : 1; /**< GADSS Clock Gating Disable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 cg3ddisbc : 1; /**< BCunit Clock Gating Disable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 cg3ddisdssrouter : 1; /**< DSS Router Clock Gating Disable */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 cg3ddis_tc_eu : 1; /**< EU_TCunit Clock Gating Disable */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 cg3ddis_ga_eu : 1; /**< EU_GAunit Clock Gating Disable */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 cg3ddis_fpu_eu : 1; /**< EU_FPUunit Clock Gating Disable */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} SSMCGCTL9524_3D_GT_STRUCT;

#define SSMCGCTL9524_3D_GT_WIDTH 32
#define SSMCGCTL9524_3D_GT_DEFAULT 0x00000000U
#define SSMCGCTL9524_3D_GT_ENDPOINT GT

#define SSMCGCTL9524_3D_GT_CG3DDISSO_LSB 0x0000
#define SSMCGCTL9524_3D_GT_CG3DDISSO_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDISSI_LSB 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDISSI_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDISSC_LSB 0x0002
#define SSMCGCTL9524_3D_GT_CG3DDISSC_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDISRAMDFT_LSB 0x0003
#define SSMCGCTL9524_3D_GT_CG3DDISRAMDFT_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDISPSD_LSB 0x0004
#define SSMCGCTL9524_3D_GT_CG3DDISPSD_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDISPL_LSB 0x0005
#define SSMCGCTL9524_3D_GT_CG3DDISPL_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDISOATREP_LSB 0x0006
#define SSMCGCTL9524_3D_GT_CG3DDISOATREP_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDISMT_LSB 0x0007
#define SSMCGCTL9524_3D_GT_CG3DDISMT_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDISMA_LSB 0x0008
#define SSMCGCTL9524_3D_GT_CG3DDISMA_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDISIME_LSB 0x0009
#define SSMCGCTL9524_3D_GT_CG3DDISIME_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDISIC_LSB 0x000b
#define SSMCGCTL9524_3D_GT_CG3DDISIC_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDISVMEMREQARB_LSB 0x000f
#define SSMCGCTL9524_3D_GT_CG3DDISVMEMREQARB_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDISGW_LSB 0x0010
#define SSMCGCTL9524_3D_GT_CG3DDISGW_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDISVMEMSOARB_LSB 0x0011
#define SSMCGCTL9524_3D_GT_CG3DDISVMEMSOARB_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDISFT_LSB 0x0012
#define SSMCGCTL9524_3D_GT_CG3DDISFT_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDIS_DPAS_EU_LSB 0x0013
#define SSMCGCTL9524_3D_GT_CG3DDIS_DPAS_EU_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDISFL_LSB 0x0014
#define SSMCGCTL9524_3D_GT_CG3DDISFL_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDISDM_LSB 0x0017
#define SSMCGCTL9524_3D_GT_CG3DDISDM_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDISDG_LSB 0x0018
#define SSMCGCTL9524_3D_GT_CG3DDISDG_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDISCRE_LSB 0x0019
#define SSMCGCTL9524_3D_GT_CG3DDISCRE_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDISGADSS_LSB 0x001a
#define SSMCGCTL9524_3D_GT_CG3DDISGADSS_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDISBC_LSB 0x001b
#define SSMCGCTL9524_3D_GT_CG3DDISBC_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDISDSSROUTER_LSB 0x001c
#define SSMCGCTL9524_3D_GT_CG3DDISDSSROUTER_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDIS_TC_EU_LSB 0x001d
#define SSMCGCTL9524_3D_GT_CG3DDIS_TC_EU_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDIS_GA_EU_LSB 0x001e
#define SSMCGCTL9524_3D_GT_CG3DDIS_GA_EU_SIZE 0x0001
#define SSMCGCTL9524_3D_GT_CG3DDIS_FPU_EU_LSB 0x001f
#define SSMCGCTL9524_3D_GT_CG3DDIS_FPU_EU_SIZE 0x0001

#ifdef SV_HOOKS

/** SSMCGCTL9528_3D desc:
  */

#define SSMCGCTL9528_3D_GT_REG           (0x00009528U)

typedef union {
  struct {
    UINT32 cg3ddistdl : 1; /**< TDLunit Clock Gating Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cg3ddissvsm : 1; /**< SVSMunit Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[02:02] */
                            /* Bits[2:2], default=0*/
    UINT32 cg3ddisst : 1; /**< STunit Clock Gating Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 cg3ddisstarb : 1; /**< starb Clock unit Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 cg3ddisdap : 1; /**< DAPunit Clock Gating Disable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 cg3ddisbesbuf : 1; /**< sbe_ssbuf Clock Gating Disable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 cg3ddis_em_eu : 1; /**< EU_EMunit Clock Gating Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 cg3ddis_tc_ph_eu : 1; /**< EUunit Clock Gating Disable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 cg3ddiscpss : 1; /**< CPSSunit Clock Gating Disable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 cg3ddisnoa : 1; /**< Noaunit Clock Gating Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 cg3ddisrde : 1; /**< RDEunit Clock Gating Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 cg3ddisslmfe : 1; /**< SLMFE unit Clock Gating Disable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 cg3ddisdto : 1; /**< DTOunit unit Clock Gating Disable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 cg3ddismsarb : 1; /**< MediaSampler arb unit Clock Gating Disable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 cg3ddistsl : 1; /**< TSL unit Clock Gating Disable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[16:16] */
                            /* Bits[16:16], default=0*/
    UINT32 cg3ddisssla : 1; /**< sslaunit Clock Gating Disable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 cg3ddismsarb1 : 1; /**< MediaSampler arb unit1 Clock Gating Disable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 cg3ddismsarb2 : 1; /**< MediaSampler arb unit2 Clock Gating Disable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 cg3ddisvmesc : 1; /**< VMESC Clock Gating Disable */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 rsvd_2 : 6; /**< Undefined - auto filled rsvd_[26:21] */
                            /* Bits[26:21], default=0*/
    UINT32 cgacgdis_sampler : 1; /**< Sampler Arch Clock Gating Disable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 cgacgdis_dataport : 1; /**< Dataport Arch Clock Gating Disable */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 cgacgdis_eu : 1; /**< EU Arch Clock Gating Disable */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 rsvd_3 : 2; /**< Undefined - auto filled rsvd_[31:30] */
                            /* Bits[31:30], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} SSMCGCTL9528_3D_GT_STRUCT;

#define SSMCGCTL9528_3D_GT_WIDTH 32
#define SSMCGCTL9528_3D_GT_DEFAULT 0x00000000U
#define SSMCGCTL9528_3D_GT_ENDPOINT GT

#define SSMCGCTL9528_3D_GT_CG3DDISTDL_LSB 0x0000
#define SSMCGCTL9528_3D_GT_CG3DDISTDL_SIZE 0x0001
#define SSMCGCTL9528_3D_GT_CG3DDISSVSM_LSB 0x0001
#define SSMCGCTL9528_3D_GT_CG3DDISSVSM_SIZE 0x0001
#define SSMCGCTL9528_3D_GT_CG3DDISST_LSB 0x0003
#define SSMCGCTL9528_3D_GT_CG3DDISST_SIZE 0x0001
#define SSMCGCTL9528_3D_GT_CG3DDISSTARB_LSB 0x0004
#define SSMCGCTL9528_3D_GT_CG3DDISSTARB_SIZE 0x0001
#define SSMCGCTL9528_3D_GT_CG3DDISDAP_LSB 0x0005
#define SSMCGCTL9528_3D_GT_CG3DDISDAP_SIZE 0x0001
#define SSMCGCTL9528_3D_GT_CG3DDISBESBUF_LSB 0x0006
#define SSMCGCTL9528_3D_GT_CG3DDISBESBUF_SIZE 0x0001
#define SSMCGCTL9528_3D_GT_CG3DDIS_EM_EU_LSB 0x0007
#define SSMCGCTL9528_3D_GT_CG3DDIS_EM_EU_SIZE 0x0001
#define SSMCGCTL9528_3D_GT_CG3DDIS_TC_PH_EU_LSB 0x0008
#define SSMCGCTL9528_3D_GT_CG3DDIS_TC_PH_EU_SIZE 0x0001
#define SSMCGCTL9528_3D_GT_CG3DDISCPSS_LSB 0x0009
#define SSMCGCTL9528_3D_GT_CG3DDISCPSS_SIZE 0x0001
#define SSMCGCTL9528_3D_GT_CG3DDISNOA_LSB 0x000a
#define SSMCGCTL9528_3D_GT_CG3DDISNOA_SIZE 0x0001
#define SSMCGCTL9528_3D_GT_CG3DDISRDE_LSB 0x000b
#define SSMCGCTL9528_3D_GT_CG3DDISRDE_SIZE 0x0001
#define SSMCGCTL9528_3D_GT_CG3DDISSLMFE_LSB 0x000c
#define SSMCGCTL9528_3D_GT_CG3DDISSLMFE_SIZE 0x0001
#define SSMCGCTL9528_3D_GT_CG3DDISDTO_LSB 0x000d
#define SSMCGCTL9528_3D_GT_CG3DDISDTO_SIZE 0x0001
#define SSMCGCTL9528_3D_GT_CG3DDISMSARB_LSB 0x000e
#define SSMCGCTL9528_3D_GT_CG3DDISMSARB_SIZE 0x0001
#define SSMCGCTL9528_3D_GT_CG3DDISTSL_LSB 0x000f
#define SSMCGCTL9528_3D_GT_CG3DDISTSL_SIZE 0x0001
#define SSMCGCTL9528_3D_GT_CG3DDISSSLA_LSB 0x0011
#define SSMCGCTL9528_3D_GT_CG3DDISSSLA_SIZE 0x0001
#define SSMCGCTL9528_3D_GT_CG3DDISMSARB1_LSB 0x0012
#define SSMCGCTL9528_3D_GT_CG3DDISMSARB1_SIZE 0x0001
#define SSMCGCTL9528_3D_GT_CG3DDISMSARB2_LSB 0x0013
#define SSMCGCTL9528_3D_GT_CG3DDISMSARB2_SIZE 0x0001
#define SSMCGCTL9528_3D_GT_CG3DDISVMESC_LSB 0x0014
#define SSMCGCTL9528_3D_GT_CG3DDISVMESC_SIZE 0x0001
#define SSMCGCTL9528_3D_GT_CGACGDIS_SAMPLER_LSB 0x001b
#define SSMCGCTL9528_3D_GT_CGACGDIS_SAMPLER_SIZE 0x0001
#define SSMCGCTL9528_3D_GT_CGACGDIS_DATAPORT_LSB 0x001c
#define SSMCGCTL9528_3D_GT_CGACGDIS_DATAPORT_SIZE 0x0001
#define SSMCGCTL9528_3D_GT_CGACGDIS_EU_LSB 0x001d
#define SSMCGCTL9528_3D_GT_CGACGDIS_EU_SIZE 0x0001

#endif      // SV_HOOKS

/** INFCGCTL9560_3D desc:
  */

#define INFCGCTL9560_3D_GT_REG           (0x00009560U)

typedef union {
  struct {
    UINT32 cg3ddisrmcr : 1; /**< MRCunit Clock Gating Disable */
                            /* Bits[0:0], default=0x00000001*/
    UINT32 cg3ddismgsr : 1; /**< MGSRunit Clock Gating Disable */
                            /* Bits[1:1], default=0x00000001*/
    UINT32 cg3ddismdrb : 1; /**< MDRBunit Clock Gating Disable */
                            /* Bits[2:2], default=0x00000001*/
    UINT32 cg3ddiscgpsf : 1; /**< CGPSFunit Clock Gating Disable */
                            /* Bits[3:3], default=0x00000001*/
    UINT32 cg3ddismbgfuc : 1; /**< MBGFUCunit Clock Gating Disable */
                            /* Bits[4:4], default=0x00000001*/
    UINT32 cg3ddisrpm : 1; /**< RPMunit Clock Gating Disable */
                            /* Bits[5:5], default=0x00000001*/
    UINT32 cg3ddisgtfs : 1; /**< GTFSunit Clock Gating Disable */
                            /* Bits[6:6], default=0x00000001*/
    UINT32 cg3ddiscpma : 1; /**< CPMAunit Clock Gating Disable */
                            /* Bits[7:7], default=0x00000001*/
    UINT32 cg3ddis_sf_gu : 1; /**< SnoopFilter Clock Gating Disable */
                            /* Bits[8:8], default=0x00000001*/
    UINT32 cg3ddis_cohpsmi : 1; /**< COH PSMI Clock Gating Disable */
                            /* Bits[9:9], default=0x00000001*/
    UINT32 cg3ddismertf : 1; /**< MERFUNIT Clock Gating Disable */
                            /* Bits[10:10], default=0x00000001*/
    UINT32 cg3ddismertxf : 1; /**< MERTXFUNIT Clock Gating Disable */
                            /* Bits[11:11], default=0x00000001*/
    UINT32 cg3ddisramdft : 1; /**< Ramdft Clock Gating Disable */
                            /* Bits[12:12], default=0x00000001*/
    UINT32 cg3ddisgdtu : 1; /**< GDTU Clock Gating Disable */
                            /* Bits[13:13], default=0x00000001*/
    UINT32 cg3ddismxbc2u : 1; /**< MXBC2U Clock Gating Disable */
                            /* Bits[14:14], default=0x00000001*/
    UINT32 cg3ddismxbu2c : 1; /**< MXBU2C Clock Gating Disable */
                            /* Bits[15:15], default=0x00000001*/
    UINT32 cg3ddismdat : 1; /**< MDAT Clock Gating Disable */
                            /* Bits[16:16], default=0x00000001*/
    UINT32 cg3ddismaxfuc : 1; /**< MAXFU2C Clock Gating Disable */
                            /* Bits[17:17], default=0x00000001*/
    UINT32 cg3ddismcmi : 1; /**< MCMI Clock Gating Disable */
                            /* Bits[18:18], default=0x00000001*/
    UINT32 cg3ddis_mcxl : 1; /**< mcxlunit Clock Gating Disable */
                            /* Bits[19:19], default=0x00000001*/
    UINT32 rsvd_0 : 12; /**< Undefined - auto filled rsvd_[31:20] */
                            /* Bits[31:20], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} INFCGCTL9560_3D_GT_STRUCT;

#define INFCGCTL9560_3D_GT_WIDTH 32
#define INFCGCTL9560_3D_GT_DEFAULT 0x000fffffU
#define INFCGCTL9560_3D_GT_ENDPOINT GT

#define INFCGCTL9560_3D_GT_CG3DDISRMCR_LSB 0x0000
#define INFCGCTL9560_3D_GT_CG3DDISRMCR_SIZE 0x0001
#define INFCGCTL9560_3D_GT_CG3DDISMGSR_LSB 0x0001
#define INFCGCTL9560_3D_GT_CG3DDISMGSR_SIZE 0x0001
#define INFCGCTL9560_3D_GT_CG3DDISMDRB_LSB 0x0002
#define INFCGCTL9560_3D_GT_CG3DDISMDRB_SIZE 0x0001
#define INFCGCTL9560_3D_GT_CG3DDISCGPSF_LSB 0x0003
#define INFCGCTL9560_3D_GT_CG3DDISCGPSF_SIZE 0x0001
#define INFCGCTL9560_3D_GT_CG3DDISMBGFUC_LSB 0x0004
#define INFCGCTL9560_3D_GT_CG3DDISMBGFUC_SIZE 0x0001
#define INFCGCTL9560_3D_GT_CG3DDISRPM_LSB 0x0005
#define INFCGCTL9560_3D_GT_CG3DDISRPM_SIZE 0x0001
#define INFCGCTL9560_3D_GT_CG3DDISGTFS_LSB 0x0006
#define INFCGCTL9560_3D_GT_CG3DDISGTFS_SIZE 0x0001
#define INFCGCTL9560_3D_GT_CG3DDISCPMA_LSB 0x0007
#define INFCGCTL9560_3D_GT_CG3DDISCPMA_SIZE 0x0001
#define INFCGCTL9560_3D_GT_CG3DDIS_SF_GU_LSB 0x0008
#define INFCGCTL9560_3D_GT_CG3DDIS_SF_GU_SIZE 0x0001
#define INFCGCTL9560_3D_GT_CG3DDIS_COHPSMI_LSB 0x0009
#define INFCGCTL9560_3D_GT_CG3DDIS_COHPSMI_SIZE 0x0001
#define INFCGCTL9560_3D_GT_CG3DDISMERTF_LSB 0x000a
#define INFCGCTL9560_3D_GT_CG3DDISMERTF_SIZE 0x0001
#define INFCGCTL9560_3D_GT_CG3DDISMERTXF_LSB 0x000b
#define INFCGCTL9560_3D_GT_CG3DDISMERTXF_SIZE 0x0001
#define INFCGCTL9560_3D_GT_CG3DDISRAMDFT_LSB 0x000c
#define INFCGCTL9560_3D_GT_CG3DDISRAMDFT_SIZE 0x0001
#define INFCGCTL9560_3D_GT_CG3DDISGDTU_LSB 0x000d
#define INFCGCTL9560_3D_GT_CG3DDISGDTU_SIZE 0x0001
#define INFCGCTL9560_3D_GT_CG3DDISMXBC2U_LSB 0x000e
#define INFCGCTL9560_3D_GT_CG3DDISMXBC2U_SIZE 0x0001
#define INFCGCTL9560_3D_GT_CG3DDISMXBU2C_LSB 0x000f
#define INFCGCTL9560_3D_GT_CG3DDISMXBU2C_SIZE 0x0001
#define INFCGCTL9560_3D_GT_CG3DDISMDAT_LSB 0x0010
#define INFCGCTL9560_3D_GT_CG3DDISMDAT_SIZE 0x0001
#define INFCGCTL9560_3D_GT_CG3DDISMAXFUC_LSB 0x0011
#define INFCGCTL9560_3D_GT_CG3DDISMAXFUC_SIZE 0x0001
#define INFCGCTL9560_3D_GT_CG3DDISMCMI_LSB 0x0012
#define INFCGCTL9560_3D_GT_CG3DDISMCMI_SIZE 0x0001
#define INFCGCTL9560_3D_GT_CG3DDIS_MCXL_LSB 0x0013
#define INFCGCTL9560_3D_GT_CG3DDIS_MCXL_SIZE 0x0001

/** GCP_CLKDIV_CONTROL0_3D desc:
  */

#define GCP_CLKDIV_CONTROL0_3D_GT_REG    (0x00009590U)

typedef union {
  struct {
    UINT32 cep_rgm_enable : 1; /**< CEP_RGM_ENABLE */
                            /* Bits[0:0], default=0x00000001*/
    UINT32 cep_rgm_der_n : 3; /**< CEP_RGM_DER_N */
                            /* Bits[3:1], default=0x00000004*/
    UINT32 cep_rgm_down_duration_interval : 2; /**< CEP_RGM_DOWN_DURATION_INTERVAL */
                            /* Bits[5:4], default=0x00000000*/
    UINT32 cep_rgm_up_duration_interval : 3; /**< CEP_RGM_UP_DURATION_INTERVAL */
                            /* Bits[8:6], default=0x00000004*/
    UINT32 fastthr_rgm_enable : 1; /**< FASTTHR_RGM_ENABLE */
                            /* Bits[9:9], default=0x00000001*/
    UINT32 fastthr_rgm_der_n : 3; /**< FASTTHR_RGM_DER_N */
                            /* Bits[12:10], default=0x00000001*/
    UINT32 fastthr_rgm_down_duration_interval : 2; /**< FASTTHR_RGM_DOWN_DURATION_INTERVAL */
                            /* Bits[14:13], default=0x00000001*/
    UINT32 fastthr_rgm_up_duration_interval : 3; /**< FASTTHR_RGM_UP_DURATION_INTERVAL */
                            /* Bits[17:15], default=0x00000004*/
    UINT32 gv_rgm_enable : 1; /**< GV_RGM_ENABLE */
                            /* Bits[18:18], default=0x00000001*/
    UINT32 gv_rgm_der_n : 3; /**< GV_RGM_DER_N */
                            /* Bits[21:19], default=0x00000001*/
    UINT32 gv_rgm_down_duration_interval : 2; /**< GV_RGM_DOWN_DURATION_INTERVAL */
                            /* Bits[23:22], default=0x00000001*/
    UINT32 gv_rgm_up_duration_interval : 3; /**< GV_RGM_UP_DURATION_INTERVAL */
                            /* Bits[26:24], default=0x00000004*/
    UINT32 rsvd_0 : 4; /**< Undefined - auto filled rsvd_[30:27] */
                            /* Bits[30:27], default=0*/
    UINT32 clkdiv_lockbit : 1; /**< CLKDIV_LOCKBIT */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GCP_CLKDIV_CONTROL0_3D_GT_STRUCT;

#define GCP_CLKDIV_CONTROL0_3D_GT_WIDTH 32
#define GCP_CLKDIV_CONTROL0_3D_GT_DEFAULT 0x044e2709U
#define GCP_CLKDIV_CONTROL0_3D_GT_ENDPOINT GT

#define GCP_CLKDIV_CONTROL0_3D_GT_CEP_RGM_ENABLE_LSB 0x0000
#define GCP_CLKDIV_CONTROL0_3D_GT_CEP_RGM_ENABLE_SIZE 0x0001
#define GCP_CLKDIV_CONTROL0_3D_GT_CEP_RGM_DER_N_LSB 0x0001
#define GCP_CLKDIV_CONTROL0_3D_GT_CEP_RGM_DER_N_SIZE 0x0003
#define GCP_CLKDIV_CONTROL0_3D_GT_CEP_RGM_DOWN_DURATION_INTERVAL_LSB 0x0004
#define GCP_CLKDIV_CONTROL0_3D_GT_CEP_RGM_DOWN_DURATION_INTERVAL_SIZE 0x0002
#define GCP_CLKDIV_CONTROL0_3D_GT_CEP_RGM_UP_DURATION_INTERVAL_LSB 0x0006
#define GCP_CLKDIV_CONTROL0_3D_GT_CEP_RGM_UP_DURATION_INTERVAL_SIZE 0x0003
#define GCP_CLKDIV_CONTROL0_3D_GT_FASTTHR_RGM_ENABLE_LSB 0x0009
#define GCP_CLKDIV_CONTROL0_3D_GT_FASTTHR_RGM_ENABLE_SIZE 0x0001
#define GCP_CLKDIV_CONTROL0_3D_GT_FASTTHR_RGM_DER_N_LSB 0x000a
#define GCP_CLKDIV_CONTROL0_3D_GT_FASTTHR_RGM_DER_N_SIZE 0x0003
#define GCP_CLKDIV_CONTROL0_3D_GT_FASTTHR_RGM_DOWN_DURATION_INTERVAL_LSB 0x000d
#define GCP_CLKDIV_CONTROL0_3D_GT_FASTTHR_RGM_DOWN_DURATION_INTERVAL_SIZE 0x0002
#define GCP_CLKDIV_CONTROL0_3D_GT_FASTTHR_RGM_UP_DURATION_INTERVAL_LSB 0x000f
#define GCP_CLKDIV_CONTROL0_3D_GT_FASTTHR_RGM_UP_DURATION_INTERVAL_SIZE 0x0003
#define GCP_CLKDIV_CONTROL0_3D_GT_GV_RGM_ENABLE_LSB 0x0012
#define GCP_CLKDIV_CONTROL0_3D_GT_GV_RGM_ENABLE_SIZE 0x0001
#define GCP_CLKDIV_CONTROL0_3D_GT_GV_RGM_DER_N_LSB 0x0013
#define GCP_CLKDIV_CONTROL0_3D_GT_GV_RGM_DER_N_SIZE 0x0003
#define GCP_CLKDIV_CONTROL0_3D_GT_GV_RGM_DOWN_DURATION_INTERVAL_LSB 0x0016
#define GCP_CLKDIV_CONTROL0_3D_GT_GV_RGM_DOWN_DURATION_INTERVAL_SIZE 0x0002
#define GCP_CLKDIV_CONTROL0_3D_GT_GV_RGM_UP_DURATION_INTERVAL_LSB 0x0018
#define GCP_CLKDIV_CONTROL0_3D_GT_GV_RGM_UP_DURATION_INTERVAL_SIZE 0x0003
#define GCP_CLKDIV_CONTROL0_3D_GT_CLKDIV_LOCKBIT_LSB 0x001f
#define GCP_CLKDIV_CONTROL0_3D_GT_CLKDIV_LOCKBIT_SIZE 0x0001

/** GCP_CLKDIV_CONTROL1_3D desc:
  */

#define GCP_CLKDIV_CONTROL1_3D_GT_REG    (0x00009594U)

typedef union {
  struct {
    UINT32 rcp1_rgm_enable : 1; /**< RCP1_RGM_ENABLE */
                            /* Bits[0:0], default=0x00000001*/
    UINT32 rcp1_rgm_der_n : 3; /**< RCP1_RGM_DER_N */
                            /* Bits[3:1], default=0x00000001*/
    UINT32 rcp1_rgm_down_duration_interval : 2; /**< RCP1_RGM_DOWN_DURATION_INTERVAL */
                            /* Bits[5:4], default=0x00000001*/
    UINT32 rcp1_rgm_up_duration_interval : 3; /**< RCP1_RGM_UP_DURATION_INTERVAL */
                            /* Bits[8:6], default=0x00000004*/
    UINT32 rsvd_0 : 22; /**< Undefined - auto filled rsvd_[30:09] */
                            /* Bits[30:9], default=0*/
    UINT32 clkdiv_lockbit : 1; /**< CLKDIV_LOCKBIT */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GCP_CLKDIV_CONTROL1_3D_GT_STRUCT;

#define GCP_CLKDIV_CONTROL1_3D_GT_WIDTH 32
#define GCP_CLKDIV_CONTROL1_3D_GT_DEFAULT 0x00000113U
#define GCP_CLKDIV_CONTROL1_3D_GT_ENDPOINT GT

#define GCP_CLKDIV_CONTROL1_3D_GT_RCP1_RGM_ENABLE_LSB 0x0000
#define GCP_CLKDIV_CONTROL1_3D_GT_RCP1_RGM_ENABLE_SIZE 0x0001
#define GCP_CLKDIV_CONTROL1_3D_GT_RCP1_RGM_DER_N_LSB 0x0001
#define GCP_CLKDIV_CONTROL1_3D_GT_RCP1_RGM_DER_N_SIZE 0x0003
#define GCP_CLKDIV_CONTROL1_3D_GT_RCP1_RGM_DOWN_DURATION_INTERVAL_LSB 0x0004
#define GCP_CLKDIV_CONTROL1_3D_GT_RCP1_RGM_DOWN_DURATION_INTERVAL_SIZE 0x0002
#define GCP_CLKDIV_CONTROL1_3D_GT_RCP1_RGM_UP_DURATION_INTERVAL_LSB 0x0006
#define GCP_CLKDIV_CONTROL1_3D_GT_RCP1_RGM_UP_DURATION_INTERVAL_SIZE 0x0003
#define GCP_CLKDIV_CONTROL1_3D_GT_CLKDIV_LOCKBIT_LSB 0x001f
#define GCP_CLKDIV_CONTROL1_3D_GT_CLKDIV_LOCKBIT_SIZE 0x0001

/** RP_FREQ_NORMAL_GPM_GRP_GCD desc:
  */

#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_REG (0x0000a008U)

typedef union {
  struct {
    UINT32 slice_unslice_throttle_policy : 4; /**< Slice UnSlice Throttle Policy */
                            /* Bits[3:0], default=0x00000000*/
    UINT32 rpfreq_slice_sqush_hint : 1; /**< Slice clock squash/unsquash Hint */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 rpfreq_unslice_sqush_hint : 1; /**< Unslice clock squash/unsquash Hint */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 rpfreq_slice_change_hint : 1; /**< Slice Frequency Change Hint */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 rpfreq_unslice_change_hint : 1; /**< Unslice Frequency Change Hint */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 rpfreq_fabzric_change_hint : 1; /**< Fabric Frequency Change Hint */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 rpfreq_not_used_1 : 4; /**< FIELD NOT USED */
                            /* Bits[12:9], default=0x00000000*/
    UINT32 media_freq_ratio : 1; /**< Software requested media to GT frequency ratio */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 slice_ratio : 9; /**< Software requested Slice ratio */
                            /* Bits[22:14], default=0x00000000*/
    UINT32 unslice_ratio : 9; /**< Software requested UnSlice ratio */
                            /* Bits[31:23], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RP_FREQ_NORMAL_GPM_GRP_GCD_GT_STRUCT;

#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_WIDTH 32
#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_DEFAULT 0x00000000U
#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_ENDPOINT GT

#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_SLICE_UNSLICE_THROTTLE_POLICY_LSB 0x0000
#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_SLICE_UNSLICE_THROTTLE_POLICY_SIZE 0x0004
#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_RPFREQ_SLICE_SQUSH_HINT_LSB 0x0004
#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_RPFREQ_SLICE_SQUSH_HINT_SIZE 0x0001
#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_RPFREQ_UNSLICE_SQUSH_HINT_LSB 0x0005
#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_RPFREQ_UNSLICE_SQUSH_HINT_SIZE 0x0001
#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_RPFREQ_SLICE_CHANGE_HINT_LSB 0x0006
#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_RPFREQ_SLICE_CHANGE_HINT_SIZE 0x0001
#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_RPFREQ_UNSLICE_CHANGE_HINT_LSB 0x0007
#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_RPFREQ_UNSLICE_CHANGE_HINT_SIZE 0x0001
#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_RPFREQ_FABZRIC_CHANGE_HINT_LSB 0x0008
#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_RPFREQ_FABZRIC_CHANGE_HINT_SIZE 0x0001
#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_RPFREQ_NOT_USED_1_LSB 0x0009
#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_RPFREQ_NOT_USED_1_SIZE 0x0004
#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_MEDIA_FREQ_RATIO_LSB 0x000d
#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_MEDIA_FREQ_RATIO_SIZE 0x0001
#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_SLICE_RATIO_LSB 0x000e
#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_SLICE_RATIO_SIZE 0x0009
#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_UNSLICE_RATIO_LSB 0x0017
#define RP_FREQ_NORMAL_GPM_GRP_GCD_GT_UNSLICE_RATIO_SIZE 0x0009

/** RP_CTRL_GPM_GRP_GCD desc:
  */

#define RP_CTRL_GPM_GRP_GCD_GT_REG       (0x0000a024U)

typedef union {
  struct {
    UINT32 rsvd_0 : 9; /**< Undefined - auto filled rsvd_[08:00] */
                            /* Bits[8:0], default=0*/
    UINT32 rp_sw_mode_sel : 2; /**< RP Software Mode Control */
                            /* Bits[10:9], default=0x00000000*/
    UINT32 media_videoturbo_en : 1; /**< RP Video Turbo Enable for Media Engine */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 rsvd_1 : 20; /**< Undefined - auto filled rsvd_[31:12] */
                            /* Bits[31:12], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RP_CTRL_GPM_GRP_GCD_GT_STRUCT;

#define RP_CTRL_GPM_GRP_GCD_GT_WIDTH 32
#define RP_CTRL_GPM_GRP_GCD_GT_DEFAULT 0x00000000U
#define RP_CTRL_GPM_GRP_GCD_GT_ENDPOINT GT

#define RP_CTRL_GPM_GRP_GCD_GT_RP_SW_MODE_SEL_LSB 0x0009
#define RP_CTRL_GPM_GRP_GCD_GT_RP_SW_MODE_SEL_SIZE 0x0002
#define RP_CTRL_GPM_GRP_GCD_GT_MEDIA_VIDEOTURBO_EN_LSB 0x000b
#define RP_CTRL_GPM_GRP_GCD_GT_MEDIA_VIDEOTURBO_EN_SIZE 0x0001

/** RC_CTRL1_GPM_GRP_GCD desc:
  */

#define RC_CTRL1_GPM_GRP_GCD_GT_REG      (0x0000a094U)

typedef union {
  struct {
    UINT32 rsvd_0 : 16; /**< Undefined - auto filled rsvd_[15:00] */
                            /* Bits[15:0], default=0*/
    UINT32 rc_sw_state_req : 3; /**< Target SW/FSM/PCU Controlled RC state */
                            /* Bits[18:16], default=0x00000000*/
    UINT32 rsvd_1 : 13; /**< Undefined - auto filled rsvd_[31:19] */
                            /* Bits[31:19], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RC_CTRL1_GPM_GRP_GCD_GT_STRUCT;

#define RC_CTRL1_GPM_GRP_GCD_GT_WIDTH 32
#define RC_CTRL1_GPM_GRP_GCD_GT_DEFAULT 0x00000000U
#define RC_CTRL1_GPM_GRP_GCD_GT_ENDPOINT GT

#define RC_CTRL1_GPM_GRP_GCD_GT_RC_SW_STATE_REQ_LSB 0x0010
#define RC_CTRL1_GPM_GRP_GCD_GT_RC_SW_STATE_REQ_SIZE 0x0003

/** MISC_CTRL0_GPM_GRP_GCD desc:
  */

#define MISC_CTRL0_GPM_GRP_GCD_GT_REG    (0x0000a180U)

typedef union {
  struct {
    UINT32 psmi_gal_cntr_thresh : 16; /**< Threshold Gal Based Counter for PSMI */
                            /* Bits[15:0], default=0x00000054*/
    UINT32 rsvd_0 : 3; /**< Undefined - auto filled rsvd_[18:16] */
                            /* Bits[18:16], default=0*/
    UINT32 gacfg_done : 1; /**< Indication if GACFG pipeline is idle */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 noa_sel : 1; /**< Secondary NOA signal Select */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 rc6_all_idle_en : 1; /**< Include idleness of all agents in RC6 qualification */
                            /* Bits[21:21], default=0x00000001*/
    UINT32 rc_sw_state_flr_default : 3; /**< Programmable Default for Software Render C-State Control Wish upon FLR */
                            /* Bits[24:22], default=0x00000004*/
    UINT32 cpd_gam_go_en : 1; /**< CPD GAM GO Messaging Enable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 rc_fifoblock_type : 1; /**< RC6 FIFO Block Type When Requesting FIFO Block */
                            /* Bits[26:26], default=0x00000001*/
    UINT32 override_coreactive : 1; /**< Force CoreActive High */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 cpd_core_rw_en : 1; /**< CPD Core Status Register Access Enable */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[29:29] */
                            /* Bits[29:29], default=0*/
    UINT32 cpd_override_ia_as_gvl : 1; /**< Force CPD IA as Non-IA */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 lock : 1; /**< Lock Bit */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MISC_CTRL0_GPM_GRP_GCD_GT_STRUCT;

#define MISC_CTRL0_GPM_GRP_GCD_GT_WIDTH 32
#define MISC_CTRL0_GPM_GRP_GCD_GT_DEFAULT 0x05200054U
#define MISC_CTRL0_GPM_GRP_GCD_GT_ENDPOINT GT

#define MISC_CTRL0_GPM_GRP_GCD_GT_PSMI_GAL_CNTR_THRESH_LSB 0x0000
#define MISC_CTRL0_GPM_GRP_GCD_GT_PSMI_GAL_CNTR_THRESH_SIZE 0x0010
#define MISC_CTRL0_GPM_GRP_GCD_GT_GACFG_DONE_LSB 0x0013
#define MISC_CTRL0_GPM_GRP_GCD_GT_GACFG_DONE_SIZE 0x0001
#define MISC_CTRL0_GPM_GRP_GCD_GT_NOA_SEL_LSB 0x0014
#define MISC_CTRL0_GPM_GRP_GCD_GT_NOA_SEL_SIZE 0x0001
#define MISC_CTRL0_GPM_GRP_GCD_GT_RC6_ALL_IDLE_EN_LSB 0x0015
#define MISC_CTRL0_GPM_GRP_GCD_GT_RC6_ALL_IDLE_EN_SIZE 0x0001
#define MISC_CTRL0_GPM_GRP_GCD_GT_RC_SW_STATE_FLR_DEFAULT_LSB 0x0016
#define MISC_CTRL0_GPM_GRP_GCD_GT_RC_SW_STATE_FLR_DEFAULT_SIZE 0x0003
#define MISC_CTRL0_GPM_GRP_GCD_GT_CPD_GAM_GO_EN_LSB 0x0019
#define MISC_CTRL0_GPM_GRP_GCD_GT_CPD_GAM_GO_EN_SIZE 0x0001
#define MISC_CTRL0_GPM_GRP_GCD_GT_RC_FIFOBLOCK_TYPE_LSB 0x001a
#define MISC_CTRL0_GPM_GRP_GCD_GT_RC_FIFOBLOCK_TYPE_SIZE 0x0001
#define MISC_CTRL0_GPM_GRP_GCD_GT_OVERRIDE_COREACTIVE_LSB 0x001b
#define MISC_CTRL0_GPM_GRP_GCD_GT_OVERRIDE_COREACTIVE_SIZE 0x0001
#define MISC_CTRL0_GPM_GRP_GCD_GT_CPD_CORE_RW_EN_LSB 0x001c
#define MISC_CTRL0_GPM_GRP_GCD_GT_CPD_CORE_RW_EN_SIZE 0x0001
#define MISC_CTRL0_GPM_GRP_GCD_GT_CPD_OVERRIDE_IA_AS_GVL_LSB 0x001e
#define MISC_CTRL0_GPM_GRP_GCD_GT_CPD_OVERRIDE_IA_AS_GVL_SIZE 0x0001
#define MISC_CTRL0_GPM_GRP_GCD_GT_LOCK_LSB 0x001f
#define MISC_CTRL0_GPM_GRP_GCD_GT_LOCK_SIZE 0x0001

/** FORCE_WAKE_GPM_GRP_GCD desc:
  */

#define FORCE_WAKE_GPM_GRP_GCD_GT_REG    (0x0000a188U)

typedef union {
  struct {
    UINT32 req_0 : 1; /**< Force Wake Request for Thread 0 */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 req_1 : 1; /**< Force Wake Request for Thread 1 */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 req_2 : 1; /**< Force Wake Request for Thread 2 */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 req_3 : 1; /**< Force Wake Request for Thread 3 */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 req_4 : 1; /**< Force Wake Request for Thread 4 */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 req_5 : 1; /**< Force Wake Request for Thread 5 */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 req_6 : 1; /**< Force Wake Request for Thread 6 */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 req_7 : 1; /**< Force Wake Request for Thread 7 */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 req_8 : 1; /**< Force Wake Request for Thread 8 */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 req_9 : 1; /**< Force Wake Request for Thread 9 */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 req_10 : 1; /**< Force Wake Request for Thread 10 */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 req_11 : 1; /**< Force Wake Request for Thread 11 */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 req_12 : 1; /**< Force Wake Request for Thread 12 */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 req_13 : 1; /**< Force Wake Request for Thread 13 */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 req_14 : 1; /**< Force Wake Request for Thread 14 */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 req_15 : 1; /**< Force Wake Request for Thread 15 */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 mask_en : 16; /**< Multiple Force Wake Mask */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FORCE_WAKE_GPM_GRP_GCD_GT_STRUCT;

#define FORCE_WAKE_GPM_GRP_GCD_GT_WIDTH 32
#define FORCE_WAKE_GPM_GRP_GCD_GT_DEFAULT 0x00000000U
#define FORCE_WAKE_GPM_GRP_GCD_GT_ENDPOINT GT

#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_0_LSB 0x0000
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_0_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_1_LSB 0x0001
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_1_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_2_LSB 0x0002
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_2_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_3_LSB 0x0003
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_3_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_4_LSB 0x0004
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_4_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_5_LSB 0x0005
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_5_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_6_LSB 0x0006
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_6_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_7_LSB 0x0007
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_7_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_8_LSB 0x0008
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_8_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_9_LSB 0x0009
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_9_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_10_LSB 0x000a
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_10_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_11_LSB 0x000b
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_11_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_12_LSB 0x000c
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_12_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_13_LSB 0x000d
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_13_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_14_LSB 0x000e
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_14_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_15_LSB 0x000f
#define FORCE_WAKE_GPM_GRP_GCD_GT_REQ_15_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_GCD_GT_MASK_EN_LSB 0x0010
#define FORCE_WAKE_GPM_GRP_GCD_GT_MASK_EN_SIZE 0x0010

/** ECO_BUSRST_GPM_GRP_GCD desc:
  */

#define ECO_BUSRST_GPM_GRP_GCD_GT_REG    (0x0000a194U)

typedef union {
  struct {
    UINT32 subwells_powergate_chknmode : 1; /**< Subwells PowerGate Chicken Mode */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cs_busyack_powergate_disable_chknmode : 1; /**< PowerGate Disable Mode CS BusyAck Chicken Mode */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 ecorsvd : 1; /**< ECORSVD */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 ignore_vcr_media_wake_req : 1; /**< Chicken bit to ignore VCR Media Wake */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 rcrp_comp_2129979_bugfix_mask : 1; /**< RCRP Compensation 2129979 BugFix Mask Bit */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 gacfg_fence_c6status_write : 1; /**< GACFG Fence Set Only for C6 Status Writes */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 gam_gpm_busy_legacy : 1; /**< GAM to GPM Busy Protocol Chicken mode */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 cpd_block_mgsr_disable : 1; /**< Disable Blocking MGSR for CPD Flow Chicken mode */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 cpd_bypass_go_msg_strms : 1; /**< CPD Bypass GO Messaging to Streamers */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 pocs_disable : 1; /**< Disable POCS */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 axf_iso_on_flr_disable : 1; /**< AXF Isolation On FLR Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 oa_cnt_push_disable : 1; /**< Disable OA handshake to push counter value to RPM */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 psmi_rc6_lite_enable : 1; /**< PSMI RC6 LITE ENABLE */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 enable_pmflush_for_rst : 1; /**< PM Flush Enable for reset flow */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 gsc_ctx_rstr_needed : 1; /**< GSC CTX Restore needed */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 kcr_unsl_ctx_chkn : 1; /**< Chicken bit to control KCR/UNSLICE SAVE/RESTORE Opt */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 wait_mem_wipe_post_flr_chkn : 1; /**< Wait for memory wipe complete post FLR */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 eco_busrst : 14; /**< ECO Bits - Bus Reset Domain */
                            /* Bits[30:17], default=0x00000000*/
    UINT32 lock : 1; /**< ECO Bits - Bus Reset Domain - LOCK BIT */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} ECO_BUSRST_GPM_GRP_GCD_GT_STRUCT;

#define ECO_BUSRST_GPM_GRP_GCD_GT_WIDTH 32
#define ECO_BUSRST_GPM_GRP_GCD_GT_DEFAULT 0x00000000U
#define ECO_BUSRST_GPM_GRP_GCD_GT_ENDPOINT GT

#define ECO_BUSRST_GPM_GRP_GCD_GT_SUBWELLS_POWERGATE_CHKNMODE_LSB 0x0000
#define ECO_BUSRST_GPM_GRP_GCD_GT_SUBWELLS_POWERGATE_CHKNMODE_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_GCD_GT_CS_BUSYACK_POWERGATE_DISABLE_CHKNMODE_LSB 0x0001
#define ECO_BUSRST_GPM_GRP_GCD_GT_CS_BUSYACK_POWERGATE_DISABLE_CHKNMODE_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_GCD_GT_ECORSVD_LSB 0x0002
#define ECO_BUSRST_GPM_GRP_GCD_GT_ECORSVD_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_GCD_GT_IGNORE_VCR_MEDIA_WAKE_REQ_LSB 0x0003
#define ECO_BUSRST_GPM_GRP_GCD_GT_IGNORE_VCR_MEDIA_WAKE_REQ_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_GCD_GT_RCRP_COMP_2129979_BUGFIX_MASK_LSB 0x0004
#define ECO_BUSRST_GPM_GRP_GCD_GT_RCRP_COMP_2129979_BUGFIX_MASK_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_GCD_GT_GACFG_FENCE_C6STATUS_WRITE_LSB 0x0005
#define ECO_BUSRST_GPM_GRP_GCD_GT_GACFG_FENCE_C6STATUS_WRITE_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_GCD_GT_GAM_GPM_BUSY_LEGACY_LSB 0x0006
#define ECO_BUSRST_GPM_GRP_GCD_GT_GAM_GPM_BUSY_LEGACY_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_GCD_GT_CPD_BLOCK_MGSR_DISABLE_LSB 0x0007
#define ECO_BUSRST_GPM_GRP_GCD_GT_CPD_BLOCK_MGSR_DISABLE_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_GCD_GT_CPD_BYPASS_GO_MSG_STRMS_LSB 0x0008
#define ECO_BUSRST_GPM_GRP_GCD_GT_CPD_BYPASS_GO_MSG_STRMS_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_GCD_GT_POCS_DISABLE_LSB 0x0009
#define ECO_BUSRST_GPM_GRP_GCD_GT_POCS_DISABLE_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_GCD_GT_AXF_ISO_ON_FLR_DISABLE_LSB 0x000a
#define ECO_BUSRST_GPM_GRP_GCD_GT_AXF_ISO_ON_FLR_DISABLE_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_GCD_GT_OA_CNT_PUSH_DISABLE_LSB 0x000b
#define ECO_BUSRST_GPM_GRP_GCD_GT_OA_CNT_PUSH_DISABLE_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_GCD_GT_PSMI_RC6_LITE_ENABLE_LSB 0x000c
#define ECO_BUSRST_GPM_GRP_GCD_GT_PSMI_RC6_LITE_ENABLE_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_GCD_GT_ENABLE_PMFLUSH_FOR_RST_LSB 0x000d
#define ECO_BUSRST_GPM_GRP_GCD_GT_ENABLE_PMFLUSH_FOR_RST_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_GCD_GT_GSC_CTX_RSTR_NEEDED_LSB 0x000e
#define ECO_BUSRST_GPM_GRP_GCD_GT_GSC_CTX_RSTR_NEEDED_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_GCD_GT_KCR_UNSL_CTX_CHKN_LSB 0x000f
#define ECO_BUSRST_GPM_GRP_GCD_GT_KCR_UNSL_CTX_CHKN_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_GCD_GT_WAIT_MEM_WIPE_POST_FLR_CHKN_LSB 0x0010
#define ECO_BUSRST_GPM_GRP_GCD_GT_WAIT_MEM_WIPE_POST_FLR_CHKN_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_GCD_GT_ECO_BUSRST_LSB 0x0011
#define ECO_BUSRST_GPM_GRP_GCD_GT_ECO_BUSRST_SIZE 0x000e
#define ECO_BUSRST_GPM_GRP_GCD_GT_LOCK_LSB 0x001f
#define ECO_BUSRST_GPM_GRP_GCD_GT_LOCK_SIZE 0x0001

/** L3BANKLOCK_LBCF_GFX desc:
  */

#define L3BANKLOCK_LBCF_GFX_GT_REG       (0x0000b160U)

typedef union {
  struct {
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[00:00] */
                            /* Bits[0:0], default=0*/
    UINT32 l3chmd_lock : 1; /**< Lockdown L3CHMD for writes */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[02:02] */
                            /* Bits[2:2], default=0*/
    UINT32 ltcdreg_lock : 1; /**< Lockdown LTCDREG for writes */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 rsvd_2 : 2; /**< Undefined - auto filled rsvd_[05:04] */
                            /* Bits[5:4], default=0*/
    UINT32 l3scratch3_lock : 1; /**< Lockdown SCRATCH3 for writes */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 l3sqcreg2_lock : 1; /**< Lockdown L3SQCREG2 for writes */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 ltcdreg2_lock : 1; /**< Lockdown LTCDREG2 for writes */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 rsvd_3 : 3; /**< Undefined - auto filled rsvd_[11:09] */
                            /* Bits[11:9], default=0*/
    UINT32 l3sqcreg3_lock : 1; /**< Lockdown L3SQCREG3 for writes */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 rsvd_4 : 2; /**< Undefined - auto filled rsvd_[14:13] */
                            /* Bits[14:13], default=0*/
    UINT32 l3scrub_lock : 1; /**< Lockdown L3SCRUB for writes */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 lbcflockmsgreg_lock : 1; /**< Lockdown L3LOCKMSGREG for writes */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 rsvd_5 : 14; /**< Undefined - auto filled rsvd_[30:17] */
                            /* Bits[30:17], default=0*/
    UINT32 l3banklock_lock : 1; /**< Lockdown L3BANKLOCK for writes */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} L3BANKLOCK_LBCF_GFX_GT_STRUCT;

#define L3BANKLOCK_LBCF_GFX_GT_WIDTH 32
#define L3BANKLOCK_LBCF_GFX_GT_DEFAULT 0x00000000U
#define L3BANKLOCK_LBCF_GFX_GT_ENDPOINT GT

#define L3BANKLOCK_LBCF_GFX_GT_L3CHMD_LOCK_LSB 0x0001
#define L3BANKLOCK_LBCF_GFX_GT_L3CHMD_LOCK_SIZE 0x0001
#define L3BANKLOCK_LBCF_GFX_GT_LTCDREG_LOCK_LSB 0x0003
#define L3BANKLOCK_LBCF_GFX_GT_LTCDREG_LOCK_SIZE 0x0001
#define L3BANKLOCK_LBCF_GFX_GT_L3SCRATCH3_LOCK_LSB 0x0006
#define L3BANKLOCK_LBCF_GFX_GT_L3SCRATCH3_LOCK_SIZE 0x0001
#define L3BANKLOCK_LBCF_GFX_GT_L3SQCREG2_LOCK_LSB 0x0007
#define L3BANKLOCK_LBCF_GFX_GT_L3SQCREG2_LOCK_SIZE 0x0001
#define L3BANKLOCK_LBCF_GFX_GT_LTCDREG2_LOCK_LSB 0x0008
#define L3BANKLOCK_LBCF_GFX_GT_LTCDREG2_LOCK_SIZE 0x0001
#define L3BANKLOCK_LBCF_GFX_GT_L3SQCREG3_LOCK_LSB 0x000c
#define L3BANKLOCK_LBCF_GFX_GT_L3SQCREG3_LOCK_SIZE 0x0001
#define L3BANKLOCK_LBCF_GFX_GT_L3SCRUB_LOCK_LSB 0x000f
#define L3BANKLOCK_LBCF_GFX_GT_L3SCRUB_LOCK_SIZE 0x0001
#define L3BANKLOCK_LBCF_GFX_GT_LBCFLOCKMSGREG_LOCK_LSB 0x0010
#define L3BANKLOCK_LBCF_GFX_GT_LBCFLOCKMSGREG_LOCK_SIZE 0x0001
#define L3BANKLOCK_LBCF_GFX_GT_L3BANKLOCK_LOCK_LSB 0x001f
#define L3BANKLOCK_LBCF_GFX_GT_L3BANKLOCK_LOCK_SIZE 0x0001

#ifdef SV_HOOKS

/** GSCPSMI_PWRCTXSAVE_GT_SAG desc:
  */

#define GSCPSMI_PWRCTXSAVE_GT_SAG_GT_REG (0x0000b500U)

typedef union {
  struct {
    UINT32 pwrctx_save_qwcredits : 9; /**< Power Context Save Quad Word Credits */
                            /* Bits[8:0], default=0x00000000*/
    UINT32 pwrctx_save_req : 1; /**< Power Context Save Request */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_0 : 6; /**< Undefined - auto filled rsvd_[15:10] */
                            /* Bits[15:10], default=0*/
    UINT32 mb : 16; /**< Mask Bits */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GSCPSMI_PWRCTXSAVE_GT_SAG_GT_STRUCT;

#define GSCPSMI_PWRCTXSAVE_GT_SAG_GT_WIDTH 32
#define GSCPSMI_PWRCTXSAVE_GT_SAG_GT_DEFAULT 0x00000000U
#define GSCPSMI_PWRCTXSAVE_GT_SAG_GT_ENDPOINT GT

#define GSCPSMI_PWRCTXSAVE_GT_SAG_GT_PWRCTX_SAVE_QWCREDITS_LSB 0x0000
#define GSCPSMI_PWRCTXSAVE_GT_SAG_GT_PWRCTX_SAVE_QWCREDITS_SIZE 0x0009
#define GSCPSMI_PWRCTXSAVE_GT_SAG_GT_PWRCTX_SAVE_REQ_LSB 0x0009
#define GSCPSMI_PWRCTXSAVE_GT_SAG_GT_PWRCTX_SAVE_REQ_SIZE 0x0001
#define GSCPSMI_PWRCTXSAVE_GT_SAG_GT_MB_LSB 0x0010
#define GSCPSMI_PWRCTXSAVE_GT_SAG_GT_MB_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** GSCPSMI_CONTROL_0_GT_SAG desc:
  */

#define GSCPSMI_CONTROL_0_GT_SAG_GT_REG  (0x0000b504U)

typedef union {
  struct {
    UINT32 psmi_en : 1; /**< PSMI_EN */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[01:01] */
                            /* Bits[1:1], default=0*/
    UINT32 psmi_preallocation_en : 1; /**< PSMI_PREALLOCATION_EN */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 sw_end_of_capture : 1; /**< SW_END_OF_CAPTURE */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 reset_to_base : 1; /**< RESET_TO_BASE */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 reset_cl_count : 1; /**< RESET_CL_COUNT */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 halt_at_top_en : 1; /**< HALT_AT_MAX */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 bufovrflw_intr_en : 1; /**< BUFOVRFLW_INTR_EN */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 n_8th_intr_en : 8; /**< N_8th_INTR_EN */
                            /* Bits[15:8], default=0x00000000*/
    UINT32 mask : 16; /**< MASK */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GSCPSMI_CONTROL_0_GT_SAG_GT_STRUCT;

#define GSCPSMI_CONTROL_0_GT_SAG_GT_WIDTH 32
#define GSCPSMI_CONTROL_0_GT_SAG_GT_DEFAULT 0x00000000U
#define GSCPSMI_CONTROL_0_GT_SAG_GT_ENDPOINT GT

#define GSCPSMI_CONTROL_0_GT_SAG_GT_PSMI_EN_LSB 0x0000
#define GSCPSMI_CONTROL_0_GT_SAG_GT_PSMI_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SAG_GT_PSMI_PREALLOCATION_EN_LSB 0x0002
#define GSCPSMI_CONTROL_0_GT_SAG_GT_PSMI_PREALLOCATION_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SAG_GT_SW_END_OF_CAPTURE_LSB 0x0003
#define GSCPSMI_CONTROL_0_GT_SAG_GT_SW_END_OF_CAPTURE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SAG_GT_RESET_TO_BASE_LSB 0x0004
#define GSCPSMI_CONTROL_0_GT_SAG_GT_RESET_TO_BASE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SAG_GT_RESET_CL_COUNT_LSB 0x0005
#define GSCPSMI_CONTROL_0_GT_SAG_GT_RESET_CL_COUNT_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SAG_GT_HALT_AT_TOP_EN_LSB 0x0006
#define GSCPSMI_CONTROL_0_GT_SAG_GT_HALT_AT_TOP_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SAG_GT_BUFOVRFLW_INTR_EN_LSB 0x0007
#define GSCPSMI_CONTROL_0_GT_SAG_GT_BUFOVRFLW_INTR_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SAG_GT_N_8TH_INTR_EN_LSB 0x0008
#define GSCPSMI_CONTROL_0_GT_SAG_GT_N_8TH_INTR_EN_SIZE 0x0008
#define GSCPSMI_CONTROL_0_GT_SAG_GT_MASK_LSB 0x0010
#define GSCPSMI_CONTROL_0_GT_SAG_GT_MASK_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** GSCPSMI_PWRCTXSAVE_GT_SMCI0 desc:
  */

#define GSCPSMI_PWRCTXSAVE_GT_SMCI0_GT_REG (0x0000b580U)

typedef union {
  struct {
    UINT32 pwrctx_save_qwcredits : 9; /**< Power Context Save Quad Word Credits */
                            /* Bits[8:0], default=0x00000000*/
    UINT32 pwrctx_save_req : 1; /**< Power Context Save Request */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_0 : 6; /**< Undefined - auto filled rsvd_[15:10] */
                            /* Bits[15:10], default=0*/
    UINT32 mb : 16; /**< Mask Bits */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GSCPSMI_PWRCTXSAVE_GT_SMCI0_GT_STRUCT;

#define GSCPSMI_PWRCTXSAVE_GT_SMCI0_GT_WIDTH 32
#define GSCPSMI_PWRCTXSAVE_GT_SMCI0_GT_DEFAULT 0x00000000U
#define GSCPSMI_PWRCTXSAVE_GT_SMCI0_GT_ENDPOINT GT

#define GSCPSMI_PWRCTXSAVE_GT_SMCI0_GT_PWRCTX_SAVE_QWCREDITS_LSB 0x0000
#define GSCPSMI_PWRCTXSAVE_GT_SMCI0_GT_PWRCTX_SAVE_QWCREDITS_SIZE 0x0009
#define GSCPSMI_PWRCTXSAVE_GT_SMCI0_GT_PWRCTX_SAVE_REQ_LSB 0x0009
#define GSCPSMI_PWRCTXSAVE_GT_SMCI0_GT_PWRCTX_SAVE_REQ_SIZE 0x0001
#define GSCPSMI_PWRCTXSAVE_GT_SMCI0_GT_MB_LSB 0x0010
#define GSCPSMI_PWRCTXSAVE_GT_SMCI0_GT_MB_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** GSCPSMI_CONTROL_0_GT_SMCI0 desc:
  */

#define GSCPSMI_CONTROL_0_GT_SMCI0_GT_REG (0x0000b584U)

typedef union {
  struct {
    UINT32 psmi_en : 1; /**< PSMI_EN */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[01:01] */
                            /* Bits[1:1], default=0*/
    UINT32 psmi_preallocation_en : 1; /**< PSMI_PREALLOCATION_EN */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 sw_end_of_capture : 1; /**< SW_END_OF_CAPTURE */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 reset_to_base : 1; /**< RESET_TO_BASE */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 reset_cl_count : 1; /**< RESET_CL_COUNT */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 halt_at_top_en : 1; /**< HALT_AT_MAX */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 bufovrflw_intr_en : 1; /**< BUFOVRFLW_INTR_EN */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 n_8th_intr_en : 8; /**< N_8th_INTR_EN */
                            /* Bits[15:8], default=0x00000000*/
    UINT32 mask : 16; /**< MASK */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GSCPSMI_CONTROL_0_GT_SMCI0_GT_STRUCT;

#define GSCPSMI_CONTROL_0_GT_SMCI0_GT_WIDTH 32
#define GSCPSMI_CONTROL_0_GT_SMCI0_GT_DEFAULT 0x00000000U
#define GSCPSMI_CONTROL_0_GT_SMCI0_GT_ENDPOINT GT

#define GSCPSMI_CONTROL_0_GT_SMCI0_GT_PSMI_EN_LSB 0x0000
#define GSCPSMI_CONTROL_0_GT_SMCI0_GT_PSMI_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI0_GT_PSMI_PREALLOCATION_EN_LSB 0x0002
#define GSCPSMI_CONTROL_0_GT_SMCI0_GT_PSMI_PREALLOCATION_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI0_GT_SW_END_OF_CAPTURE_LSB 0x0003
#define GSCPSMI_CONTROL_0_GT_SMCI0_GT_SW_END_OF_CAPTURE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI0_GT_RESET_TO_BASE_LSB 0x0004
#define GSCPSMI_CONTROL_0_GT_SMCI0_GT_RESET_TO_BASE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI0_GT_RESET_CL_COUNT_LSB 0x0005
#define GSCPSMI_CONTROL_0_GT_SMCI0_GT_RESET_CL_COUNT_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI0_GT_HALT_AT_TOP_EN_LSB 0x0006
#define GSCPSMI_CONTROL_0_GT_SMCI0_GT_HALT_AT_TOP_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI0_GT_BUFOVRFLW_INTR_EN_LSB 0x0007
#define GSCPSMI_CONTROL_0_GT_SMCI0_GT_BUFOVRFLW_INTR_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI0_GT_N_8TH_INTR_EN_LSB 0x0008
#define GSCPSMI_CONTROL_0_GT_SMCI0_GT_N_8TH_INTR_EN_SIZE 0x0008
#define GSCPSMI_CONTROL_0_GT_SMCI0_GT_MASK_LSB 0x0010
#define GSCPSMI_CONTROL_0_GT_SMCI0_GT_MASK_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** GSCPSMI_PWRCTXSAVE_GT_SMCI1 desc:
  */

#define GSCPSMI_PWRCTXSAVE_GT_SMCI1_GT_REG (0x0000b600U)

typedef union {
  struct {
    UINT32 pwrctx_save_qwcredits : 9; /**< Power Context Save Quad Word Credits */
                            /* Bits[8:0], default=0x00000000*/
    UINT32 pwrctx_save_req : 1; /**< Power Context Save Request */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_0 : 6; /**< Undefined - auto filled rsvd_[15:10] */
                            /* Bits[15:10], default=0*/
    UINT32 mb : 16; /**< Mask Bits */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GSCPSMI_PWRCTXSAVE_GT_SMCI1_GT_STRUCT;

#define GSCPSMI_PWRCTXSAVE_GT_SMCI1_GT_WIDTH 32
#define GSCPSMI_PWRCTXSAVE_GT_SMCI1_GT_DEFAULT 0x00000000U
#define GSCPSMI_PWRCTXSAVE_GT_SMCI1_GT_ENDPOINT GT

#define GSCPSMI_PWRCTXSAVE_GT_SMCI1_GT_PWRCTX_SAVE_QWCREDITS_LSB 0x0000
#define GSCPSMI_PWRCTXSAVE_GT_SMCI1_GT_PWRCTX_SAVE_QWCREDITS_SIZE 0x0009
#define GSCPSMI_PWRCTXSAVE_GT_SMCI1_GT_PWRCTX_SAVE_REQ_LSB 0x0009
#define GSCPSMI_PWRCTXSAVE_GT_SMCI1_GT_PWRCTX_SAVE_REQ_SIZE 0x0001
#define GSCPSMI_PWRCTXSAVE_GT_SMCI1_GT_MB_LSB 0x0010
#define GSCPSMI_PWRCTXSAVE_GT_SMCI1_GT_MB_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** GSCPSMI_CONTROL_0_GT_SMCI1 desc:
  */

#define GSCPSMI_CONTROL_0_GT_SMCI1_GT_REG (0x0000b604U)

typedef union {
  struct {
    UINT32 psmi_en : 1; /**< PSMI_EN */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[01:01] */
                            /* Bits[1:1], default=0*/
    UINT32 psmi_preallocation_en : 1; /**< PSMI_PREALLOCATION_EN */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 sw_end_of_capture : 1; /**< SW_END_OF_CAPTURE */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 reset_to_base : 1; /**< RESET_TO_BASE */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 reset_cl_count : 1; /**< RESET_CL_COUNT */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 halt_at_top_en : 1; /**< HALT_AT_MAX */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 bufovrflw_intr_en : 1; /**< BUFOVRFLW_INTR_EN */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 n_8th_intr_en : 8; /**< N_8th_INTR_EN */
                            /* Bits[15:8], default=0x00000000*/
    UINT32 mask : 16; /**< MASK */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GSCPSMI_CONTROL_0_GT_SMCI1_GT_STRUCT;

#define GSCPSMI_CONTROL_0_GT_SMCI1_GT_WIDTH 32
#define GSCPSMI_CONTROL_0_GT_SMCI1_GT_DEFAULT 0x00000000U
#define GSCPSMI_CONTROL_0_GT_SMCI1_GT_ENDPOINT GT

#define GSCPSMI_CONTROL_0_GT_SMCI1_GT_PSMI_EN_LSB 0x0000
#define GSCPSMI_CONTROL_0_GT_SMCI1_GT_PSMI_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI1_GT_PSMI_PREALLOCATION_EN_LSB 0x0002
#define GSCPSMI_CONTROL_0_GT_SMCI1_GT_PSMI_PREALLOCATION_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI1_GT_SW_END_OF_CAPTURE_LSB 0x0003
#define GSCPSMI_CONTROL_0_GT_SMCI1_GT_SW_END_OF_CAPTURE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI1_GT_RESET_TO_BASE_LSB 0x0004
#define GSCPSMI_CONTROL_0_GT_SMCI1_GT_RESET_TO_BASE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI1_GT_RESET_CL_COUNT_LSB 0x0005
#define GSCPSMI_CONTROL_0_GT_SMCI1_GT_RESET_CL_COUNT_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI1_GT_HALT_AT_TOP_EN_LSB 0x0006
#define GSCPSMI_CONTROL_0_GT_SMCI1_GT_HALT_AT_TOP_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI1_GT_BUFOVRFLW_INTR_EN_LSB 0x0007
#define GSCPSMI_CONTROL_0_GT_SMCI1_GT_BUFOVRFLW_INTR_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI1_GT_N_8TH_INTR_EN_LSB 0x0008
#define GSCPSMI_CONTROL_0_GT_SMCI1_GT_N_8TH_INTR_EN_SIZE 0x0008
#define GSCPSMI_CONTROL_0_GT_SMCI1_GT_MASK_LSB 0x0010
#define GSCPSMI_CONTROL_0_GT_SMCI1_GT_MASK_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** GSCPSMI_PWRCTXSAVE_GT_SMCI2 desc:
  */

#define GSCPSMI_PWRCTXSAVE_GT_SMCI2_GT_REG (0x0000b680U)

typedef union {
  struct {
    UINT32 pwrctx_save_qwcredits : 9; /**< Power Context Save Quad Word Credits */
                            /* Bits[8:0], default=0x00000000*/
    UINT32 pwrctx_save_req : 1; /**< Power Context Save Request */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_0 : 6; /**< Undefined - auto filled rsvd_[15:10] */
                            /* Bits[15:10], default=0*/
    UINT32 mb : 16; /**< Mask Bits */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GSCPSMI_PWRCTXSAVE_GT_SMCI2_GT_STRUCT;

#define GSCPSMI_PWRCTXSAVE_GT_SMCI2_GT_WIDTH 32
#define GSCPSMI_PWRCTXSAVE_GT_SMCI2_GT_DEFAULT 0x00000000U
#define GSCPSMI_PWRCTXSAVE_GT_SMCI2_GT_ENDPOINT GT

#define GSCPSMI_PWRCTXSAVE_GT_SMCI2_GT_PWRCTX_SAVE_QWCREDITS_LSB 0x0000
#define GSCPSMI_PWRCTXSAVE_GT_SMCI2_GT_PWRCTX_SAVE_QWCREDITS_SIZE 0x0009
#define GSCPSMI_PWRCTXSAVE_GT_SMCI2_GT_PWRCTX_SAVE_REQ_LSB 0x0009
#define GSCPSMI_PWRCTXSAVE_GT_SMCI2_GT_PWRCTX_SAVE_REQ_SIZE 0x0001
#define GSCPSMI_PWRCTXSAVE_GT_SMCI2_GT_MB_LSB 0x0010
#define GSCPSMI_PWRCTXSAVE_GT_SMCI2_GT_MB_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** GSCPSMI_CONTROL_0_GT_SMCI2 desc:
  */

#define GSCPSMI_CONTROL_0_GT_SMCI2_GT_REG (0x0000b684U)

typedef union {
  struct {
    UINT32 psmi_en : 1; /**< PSMI_EN */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[01:01] */
                            /* Bits[1:1], default=0*/
    UINT32 psmi_preallocation_en : 1; /**< PSMI_PREALLOCATION_EN */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 sw_end_of_capture : 1; /**< SW_END_OF_CAPTURE */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 reset_to_base : 1; /**< RESET_TO_BASE */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 reset_cl_count : 1; /**< RESET_CL_COUNT */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 halt_at_top_en : 1; /**< HALT_AT_MAX */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 bufovrflw_intr_en : 1; /**< BUFOVRFLW_INTR_EN */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 n_8th_intr_en : 8; /**< N_8th_INTR_EN */
                            /* Bits[15:8], default=0x00000000*/
    UINT32 mask : 16; /**< MASK */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GSCPSMI_CONTROL_0_GT_SMCI2_GT_STRUCT;

#define GSCPSMI_CONTROL_0_GT_SMCI2_GT_WIDTH 32
#define GSCPSMI_CONTROL_0_GT_SMCI2_GT_DEFAULT 0x00000000U
#define GSCPSMI_CONTROL_0_GT_SMCI2_GT_ENDPOINT GT

#define GSCPSMI_CONTROL_0_GT_SMCI2_GT_PSMI_EN_LSB 0x0000
#define GSCPSMI_CONTROL_0_GT_SMCI2_GT_PSMI_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI2_GT_PSMI_PREALLOCATION_EN_LSB 0x0002
#define GSCPSMI_CONTROL_0_GT_SMCI2_GT_PSMI_PREALLOCATION_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI2_GT_SW_END_OF_CAPTURE_LSB 0x0003
#define GSCPSMI_CONTROL_0_GT_SMCI2_GT_SW_END_OF_CAPTURE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI2_GT_RESET_TO_BASE_LSB 0x0004
#define GSCPSMI_CONTROL_0_GT_SMCI2_GT_RESET_TO_BASE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI2_GT_RESET_CL_COUNT_LSB 0x0005
#define GSCPSMI_CONTROL_0_GT_SMCI2_GT_RESET_CL_COUNT_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI2_GT_HALT_AT_TOP_EN_LSB 0x0006
#define GSCPSMI_CONTROL_0_GT_SMCI2_GT_HALT_AT_TOP_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI2_GT_BUFOVRFLW_INTR_EN_LSB 0x0007
#define GSCPSMI_CONTROL_0_GT_SMCI2_GT_BUFOVRFLW_INTR_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI2_GT_N_8TH_INTR_EN_LSB 0x0008
#define GSCPSMI_CONTROL_0_GT_SMCI2_GT_N_8TH_INTR_EN_SIZE 0x0008
#define GSCPSMI_CONTROL_0_GT_SMCI2_GT_MASK_LSB 0x0010
#define GSCPSMI_CONTROL_0_GT_SMCI2_GT_MASK_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** GSCPSMI_PWRCTXSAVE_GT_SMCI3 desc:
  */

#define GSCPSMI_PWRCTXSAVE_GT_SMCI3_GT_REG (0x0000b700U)

typedef union {
  struct {
    UINT32 pwrctx_save_qwcredits : 9; /**< Power Context Save Quad Word Credits */
                            /* Bits[8:0], default=0x00000000*/
    UINT32 pwrctx_save_req : 1; /**< Power Context Save Request */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_0 : 6; /**< Undefined - auto filled rsvd_[15:10] */
                            /* Bits[15:10], default=0*/
    UINT32 mb : 16; /**< Mask Bits */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GSCPSMI_PWRCTXSAVE_GT_SMCI3_GT_STRUCT;

#define GSCPSMI_PWRCTXSAVE_GT_SMCI3_GT_WIDTH 32
#define GSCPSMI_PWRCTXSAVE_GT_SMCI3_GT_DEFAULT 0x00000000U
#define GSCPSMI_PWRCTXSAVE_GT_SMCI3_GT_ENDPOINT GT

#define GSCPSMI_PWRCTXSAVE_GT_SMCI3_GT_PWRCTX_SAVE_QWCREDITS_LSB 0x0000
#define GSCPSMI_PWRCTXSAVE_GT_SMCI3_GT_PWRCTX_SAVE_QWCREDITS_SIZE 0x0009
#define GSCPSMI_PWRCTXSAVE_GT_SMCI3_GT_PWRCTX_SAVE_REQ_LSB 0x0009
#define GSCPSMI_PWRCTXSAVE_GT_SMCI3_GT_PWRCTX_SAVE_REQ_SIZE 0x0001
#define GSCPSMI_PWRCTXSAVE_GT_SMCI3_GT_MB_LSB 0x0010
#define GSCPSMI_PWRCTXSAVE_GT_SMCI3_GT_MB_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** GSCPSMI_CONTROL_0_GT_SMCI3 desc:
  */

#define GSCPSMI_CONTROL_0_GT_SMCI3_GT_REG (0x0000b704U)

typedef union {
  struct {
    UINT32 psmi_en : 1; /**< PSMI_EN */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[01:01] */
                            /* Bits[1:1], default=0*/
    UINT32 psmi_preallocation_en : 1; /**< PSMI_PREALLOCATION_EN */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 sw_end_of_capture : 1; /**< SW_END_OF_CAPTURE */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 reset_to_base : 1; /**< RESET_TO_BASE */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 reset_cl_count : 1; /**< RESET_CL_COUNT */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 halt_at_top_en : 1; /**< HALT_AT_MAX */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 bufovrflw_intr_en : 1; /**< BUFOVRFLW_INTR_EN */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 n_8th_intr_en : 8; /**< N_8th_INTR_EN */
                            /* Bits[15:8], default=0x00000000*/
    UINT32 mask : 16; /**< MASK */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GSCPSMI_CONTROL_0_GT_SMCI3_GT_STRUCT;

#define GSCPSMI_CONTROL_0_GT_SMCI3_GT_WIDTH 32
#define GSCPSMI_CONTROL_0_GT_SMCI3_GT_DEFAULT 0x00000000U
#define GSCPSMI_CONTROL_0_GT_SMCI3_GT_ENDPOINT GT

#define GSCPSMI_CONTROL_0_GT_SMCI3_GT_PSMI_EN_LSB 0x0000
#define GSCPSMI_CONTROL_0_GT_SMCI3_GT_PSMI_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI3_GT_PSMI_PREALLOCATION_EN_LSB 0x0002
#define GSCPSMI_CONTROL_0_GT_SMCI3_GT_PSMI_PREALLOCATION_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI3_GT_SW_END_OF_CAPTURE_LSB 0x0003
#define GSCPSMI_CONTROL_0_GT_SMCI3_GT_SW_END_OF_CAPTURE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI3_GT_RESET_TO_BASE_LSB 0x0004
#define GSCPSMI_CONTROL_0_GT_SMCI3_GT_RESET_TO_BASE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI3_GT_RESET_CL_COUNT_LSB 0x0005
#define GSCPSMI_CONTROL_0_GT_SMCI3_GT_RESET_CL_COUNT_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI3_GT_HALT_AT_TOP_EN_LSB 0x0006
#define GSCPSMI_CONTROL_0_GT_SMCI3_GT_HALT_AT_TOP_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI3_GT_BUFOVRFLW_INTR_EN_LSB 0x0007
#define GSCPSMI_CONTROL_0_GT_SMCI3_GT_BUFOVRFLW_INTR_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI3_GT_N_8TH_INTR_EN_LSB 0x0008
#define GSCPSMI_CONTROL_0_GT_SMCI3_GT_N_8TH_INTR_EN_SIZE 0x0008
#define GSCPSMI_CONTROL_0_GT_SMCI3_GT_MASK_LSB 0x0010
#define GSCPSMI_CONTROL_0_GT_SMCI3_GT_MASK_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** GSCPSMI_PWRCTXSAVE_GT_SMCI4 desc:
  */

#define GSCPSMI_PWRCTXSAVE_GT_SMCI4_GT_REG (0x0000b780U)

typedef union {
  struct {
    UINT32 pwrctx_save_qwcredits : 9; /**< Power Context Save Quad Word Credits */
                            /* Bits[8:0], default=0x00000000*/
    UINT32 pwrctx_save_req : 1; /**< Power Context Save Request */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_0 : 6; /**< Undefined - auto filled rsvd_[15:10] */
                            /* Bits[15:10], default=0*/
    UINT32 mb : 16; /**< Mask Bits */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GSCPSMI_PWRCTXSAVE_GT_SMCI4_GT_STRUCT;

#define GSCPSMI_PWRCTXSAVE_GT_SMCI4_GT_WIDTH 32
#define GSCPSMI_PWRCTXSAVE_GT_SMCI4_GT_DEFAULT 0x00000000U
#define GSCPSMI_PWRCTXSAVE_GT_SMCI4_GT_ENDPOINT GT

#define GSCPSMI_PWRCTXSAVE_GT_SMCI4_GT_PWRCTX_SAVE_QWCREDITS_LSB 0x0000
#define GSCPSMI_PWRCTXSAVE_GT_SMCI4_GT_PWRCTX_SAVE_QWCREDITS_SIZE 0x0009
#define GSCPSMI_PWRCTXSAVE_GT_SMCI4_GT_PWRCTX_SAVE_REQ_LSB 0x0009
#define GSCPSMI_PWRCTXSAVE_GT_SMCI4_GT_PWRCTX_SAVE_REQ_SIZE 0x0001
#define GSCPSMI_PWRCTXSAVE_GT_SMCI4_GT_MB_LSB 0x0010
#define GSCPSMI_PWRCTXSAVE_GT_SMCI4_GT_MB_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** GSCPSMI_CONTROL_0_GT_SMCI4 desc:
  */

#define GSCPSMI_CONTROL_0_GT_SMCI4_GT_REG (0x0000b784U)

typedef union {
  struct {
    UINT32 psmi_en : 1; /**< PSMI_EN */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[01:01] */
                            /* Bits[1:1], default=0*/
    UINT32 psmi_preallocation_en : 1; /**< PSMI_PREALLOCATION_EN */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 sw_end_of_capture : 1; /**< SW_END_OF_CAPTURE */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 reset_to_base : 1; /**< RESET_TO_BASE */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 reset_cl_count : 1; /**< RESET_CL_COUNT */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 halt_at_top_en : 1; /**< HALT_AT_MAX */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 bufovrflw_intr_en : 1; /**< BUFOVRFLW_INTR_EN */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 n_8th_intr_en : 8; /**< N_8th_INTR_EN */
                            /* Bits[15:8], default=0x00000000*/
    UINT32 mask : 16; /**< MASK */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GSCPSMI_CONTROL_0_GT_SMCI4_GT_STRUCT;

#define GSCPSMI_CONTROL_0_GT_SMCI4_GT_WIDTH 32
#define GSCPSMI_CONTROL_0_GT_SMCI4_GT_DEFAULT 0x00000000U
#define GSCPSMI_CONTROL_0_GT_SMCI4_GT_ENDPOINT GT

#define GSCPSMI_CONTROL_0_GT_SMCI4_GT_PSMI_EN_LSB 0x0000
#define GSCPSMI_CONTROL_0_GT_SMCI4_GT_PSMI_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI4_GT_PSMI_PREALLOCATION_EN_LSB 0x0002
#define GSCPSMI_CONTROL_0_GT_SMCI4_GT_PSMI_PREALLOCATION_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI4_GT_SW_END_OF_CAPTURE_LSB 0x0003
#define GSCPSMI_CONTROL_0_GT_SMCI4_GT_SW_END_OF_CAPTURE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI4_GT_RESET_TO_BASE_LSB 0x0004
#define GSCPSMI_CONTROL_0_GT_SMCI4_GT_RESET_TO_BASE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI4_GT_RESET_CL_COUNT_LSB 0x0005
#define GSCPSMI_CONTROL_0_GT_SMCI4_GT_RESET_CL_COUNT_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI4_GT_HALT_AT_TOP_EN_LSB 0x0006
#define GSCPSMI_CONTROL_0_GT_SMCI4_GT_HALT_AT_TOP_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI4_GT_BUFOVRFLW_INTR_EN_LSB 0x0007
#define GSCPSMI_CONTROL_0_GT_SMCI4_GT_BUFOVRFLW_INTR_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI4_GT_N_8TH_INTR_EN_LSB 0x0008
#define GSCPSMI_CONTROL_0_GT_SMCI4_GT_N_8TH_INTR_EN_SIZE 0x0008
#define GSCPSMI_CONTROL_0_GT_SMCI4_GT_MASK_LSB 0x0010
#define GSCPSMI_CONTROL_0_GT_SMCI4_GT_MASK_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** GSCPSMI_PWRCTXSAVE_GT_SMCI5 desc:
  */

#define GSCPSMI_PWRCTXSAVE_GT_SMCI5_GT_REG (0x0000b800U)

typedef union {
  struct {
    UINT32 pwrctx_save_qwcredits : 9; /**< Power Context Save Quad Word Credits */
                            /* Bits[8:0], default=0x00000000*/
    UINT32 pwrctx_save_req : 1; /**< Power Context Save Request */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_0 : 6; /**< Undefined - auto filled rsvd_[15:10] */
                            /* Bits[15:10], default=0*/
    UINT32 mb : 16; /**< Mask Bits */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GSCPSMI_PWRCTXSAVE_GT_SMCI5_GT_STRUCT;

#define GSCPSMI_PWRCTXSAVE_GT_SMCI5_GT_WIDTH 32
#define GSCPSMI_PWRCTXSAVE_GT_SMCI5_GT_DEFAULT 0x00000000U
#define GSCPSMI_PWRCTXSAVE_GT_SMCI5_GT_ENDPOINT GT

#define GSCPSMI_PWRCTXSAVE_GT_SMCI5_GT_PWRCTX_SAVE_QWCREDITS_LSB 0x0000
#define GSCPSMI_PWRCTXSAVE_GT_SMCI5_GT_PWRCTX_SAVE_QWCREDITS_SIZE 0x0009
#define GSCPSMI_PWRCTXSAVE_GT_SMCI5_GT_PWRCTX_SAVE_REQ_LSB 0x0009
#define GSCPSMI_PWRCTXSAVE_GT_SMCI5_GT_PWRCTX_SAVE_REQ_SIZE 0x0001
#define GSCPSMI_PWRCTXSAVE_GT_SMCI5_GT_MB_LSB 0x0010
#define GSCPSMI_PWRCTXSAVE_GT_SMCI5_GT_MB_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** GSCPSMI_CONTROL_0_GT_SMCI5 desc:
  */

#define GSCPSMI_CONTROL_0_GT_SMCI5_GT_REG (0x0000b804U)

typedef union {
  struct {
    UINT32 psmi_en : 1; /**< PSMI_EN */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[01:01] */
                            /* Bits[1:1], default=0*/
    UINT32 psmi_preallocation_en : 1; /**< PSMI_PREALLOCATION_EN */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 sw_end_of_capture : 1; /**< SW_END_OF_CAPTURE */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 reset_to_base : 1; /**< RESET_TO_BASE */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 reset_cl_count : 1; /**< RESET_CL_COUNT */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 halt_at_top_en : 1; /**< HALT_AT_MAX */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 bufovrflw_intr_en : 1; /**< BUFOVRFLW_INTR_EN */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 n_8th_intr_en : 8; /**< N_8th_INTR_EN */
                            /* Bits[15:8], default=0x00000000*/
    UINT32 mask : 16; /**< MASK */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GSCPSMI_CONTROL_0_GT_SMCI5_GT_STRUCT;

#define GSCPSMI_CONTROL_0_GT_SMCI5_GT_WIDTH 32
#define GSCPSMI_CONTROL_0_GT_SMCI5_GT_DEFAULT 0x00000000U
#define GSCPSMI_CONTROL_0_GT_SMCI5_GT_ENDPOINT GT

#define GSCPSMI_CONTROL_0_GT_SMCI5_GT_PSMI_EN_LSB 0x0000
#define GSCPSMI_CONTROL_0_GT_SMCI5_GT_PSMI_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI5_GT_PSMI_PREALLOCATION_EN_LSB 0x0002
#define GSCPSMI_CONTROL_0_GT_SMCI5_GT_PSMI_PREALLOCATION_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI5_GT_SW_END_OF_CAPTURE_LSB 0x0003
#define GSCPSMI_CONTROL_0_GT_SMCI5_GT_SW_END_OF_CAPTURE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI5_GT_RESET_TO_BASE_LSB 0x0004
#define GSCPSMI_CONTROL_0_GT_SMCI5_GT_RESET_TO_BASE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI5_GT_RESET_CL_COUNT_LSB 0x0005
#define GSCPSMI_CONTROL_0_GT_SMCI5_GT_RESET_CL_COUNT_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI5_GT_HALT_AT_TOP_EN_LSB 0x0006
#define GSCPSMI_CONTROL_0_GT_SMCI5_GT_HALT_AT_TOP_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI5_GT_BUFOVRFLW_INTR_EN_LSB 0x0007
#define GSCPSMI_CONTROL_0_GT_SMCI5_GT_BUFOVRFLW_INTR_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI5_GT_N_8TH_INTR_EN_LSB 0x0008
#define GSCPSMI_CONTROL_0_GT_SMCI5_GT_N_8TH_INTR_EN_SIZE 0x0008
#define GSCPSMI_CONTROL_0_GT_SMCI5_GT_MASK_LSB 0x0010
#define GSCPSMI_CONTROL_0_GT_SMCI5_GT_MASK_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** GSCPSMI_PWRCTXSAVE_GT_SMCI6 desc:
  */

#define GSCPSMI_PWRCTXSAVE_GT_SMCI6_GT_REG (0x0000b880U)

typedef union {
  struct {
    UINT32 pwrctx_save_qwcredits : 9; /**< Power Context Save Quad Word Credits */
                            /* Bits[8:0], default=0x00000000*/
    UINT32 pwrctx_save_req : 1; /**< Power Context Save Request */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_0 : 6; /**< Undefined - auto filled rsvd_[15:10] */
                            /* Bits[15:10], default=0*/
    UINT32 mb : 16; /**< Mask Bits */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GSCPSMI_PWRCTXSAVE_GT_SMCI6_GT_STRUCT;

#define GSCPSMI_PWRCTXSAVE_GT_SMCI6_GT_WIDTH 32
#define GSCPSMI_PWRCTXSAVE_GT_SMCI6_GT_DEFAULT 0x00000000U
#define GSCPSMI_PWRCTXSAVE_GT_SMCI6_GT_ENDPOINT GT

#define GSCPSMI_PWRCTXSAVE_GT_SMCI6_GT_PWRCTX_SAVE_QWCREDITS_LSB 0x0000
#define GSCPSMI_PWRCTXSAVE_GT_SMCI6_GT_PWRCTX_SAVE_QWCREDITS_SIZE 0x0009
#define GSCPSMI_PWRCTXSAVE_GT_SMCI6_GT_PWRCTX_SAVE_REQ_LSB 0x0009
#define GSCPSMI_PWRCTXSAVE_GT_SMCI6_GT_PWRCTX_SAVE_REQ_SIZE 0x0001
#define GSCPSMI_PWRCTXSAVE_GT_SMCI6_GT_MB_LSB 0x0010
#define GSCPSMI_PWRCTXSAVE_GT_SMCI6_GT_MB_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** GSCPSMI_CONTROL_0_GT_SMCI6 desc:
  */

#define GSCPSMI_CONTROL_0_GT_SMCI6_GT_REG (0x0000b884U)

typedef union {
  struct {
    UINT32 psmi_en : 1; /**< PSMI_EN */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[01:01] */
                            /* Bits[1:1], default=0*/
    UINT32 psmi_preallocation_en : 1; /**< PSMI_PREALLOCATION_EN */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 sw_end_of_capture : 1; /**< SW_END_OF_CAPTURE */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 reset_to_base : 1; /**< RESET_TO_BASE */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 reset_cl_count : 1; /**< RESET_CL_COUNT */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 halt_at_top_en : 1; /**< HALT_AT_MAX */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 bufovrflw_intr_en : 1; /**< BUFOVRFLW_INTR_EN */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 n_8th_intr_en : 8; /**< N_8th_INTR_EN */
                            /* Bits[15:8], default=0x00000000*/
    UINT32 mask : 16; /**< MASK */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GSCPSMI_CONTROL_0_GT_SMCI6_GT_STRUCT;

#define GSCPSMI_CONTROL_0_GT_SMCI6_GT_WIDTH 32
#define GSCPSMI_CONTROL_0_GT_SMCI6_GT_DEFAULT 0x00000000U
#define GSCPSMI_CONTROL_0_GT_SMCI6_GT_ENDPOINT GT

#define GSCPSMI_CONTROL_0_GT_SMCI6_GT_PSMI_EN_LSB 0x0000
#define GSCPSMI_CONTROL_0_GT_SMCI6_GT_PSMI_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI6_GT_PSMI_PREALLOCATION_EN_LSB 0x0002
#define GSCPSMI_CONTROL_0_GT_SMCI6_GT_PSMI_PREALLOCATION_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI6_GT_SW_END_OF_CAPTURE_LSB 0x0003
#define GSCPSMI_CONTROL_0_GT_SMCI6_GT_SW_END_OF_CAPTURE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI6_GT_RESET_TO_BASE_LSB 0x0004
#define GSCPSMI_CONTROL_0_GT_SMCI6_GT_RESET_TO_BASE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI6_GT_RESET_CL_COUNT_LSB 0x0005
#define GSCPSMI_CONTROL_0_GT_SMCI6_GT_RESET_CL_COUNT_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI6_GT_HALT_AT_TOP_EN_LSB 0x0006
#define GSCPSMI_CONTROL_0_GT_SMCI6_GT_HALT_AT_TOP_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI6_GT_BUFOVRFLW_INTR_EN_LSB 0x0007
#define GSCPSMI_CONTROL_0_GT_SMCI6_GT_BUFOVRFLW_INTR_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI6_GT_N_8TH_INTR_EN_LSB 0x0008
#define GSCPSMI_CONTROL_0_GT_SMCI6_GT_N_8TH_INTR_EN_SIZE 0x0008
#define GSCPSMI_CONTROL_0_GT_SMCI6_GT_MASK_LSB 0x0010
#define GSCPSMI_CONTROL_0_GT_SMCI6_GT_MASK_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** GSCPSMI_PWRCTXSAVE_GT_SMCI7 desc:
  */

#define GSCPSMI_PWRCTXSAVE_GT_SMCI7_GT_REG (0x0000b900U)

typedef union {
  struct {
    UINT32 pwrctx_save_qwcredits : 9; /**< Power Context Save Quad Word Credits */
                            /* Bits[8:0], default=0x00000000*/
    UINT32 pwrctx_save_req : 1; /**< Power Context Save Request */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_0 : 6; /**< Undefined - auto filled rsvd_[15:10] */
                            /* Bits[15:10], default=0*/
    UINT32 mb : 16; /**< Mask Bits */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GSCPSMI_PWRCTXSAVE_GT_SMCI7_GT_STRUCT;

#define GSCPSMI_PWRCTXSAVE_GT_SMCI7_GT_WIDTH 32
#define GSCPSMI_PWRCTXSAVE_GT_SMCI7_GT_DEFAULT 0x00000000U
#define GSCPSMI_PWRCTXSAVE_GT_SMCI7_GT_ENDPOINT GT

#define GSCPSMI_PWRCTXSAVE_GT_SMCI7_GT_PWRCTX_SAVE_QWCREDITS_LSB 0x0000
#define GSCPSMI_PWRCTXSAVE_GT_SMCI7_GT_PWRCTX_SAVE_QWCREDITS_SIZE 0x0009
#define GSCPSMI_PWRCTXSAVE_GT_SMCI7_GT_PWRCTX_SAVE_REQ_LSB 0x0009
#define GSCPSMI_PWRCTXSAVE_GT_SMCI7_GT_PWRCTX_SAVE_REQ_SIZE 0x0001
#define GSCPSMI_PWRCTXSAVE_GT_SMCI7_GT_MB_LSB 0x0010
#define GSCPSMI_PWRCTXSAVE_GT_SMCI7_GT_MB_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** GSCPSMI_CONTROL_0_GT_SMCI7 desc:
  */

#define GSCPSMI_CONTROL_0_GT_SMCI7_GT_REG (0x0000b904U)

typedef union {
  struct {
    UINT32 psmi_en : 1; /**< PSMI_EN */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[01:01] */
                            /* Bits[1:1], default=0*/
    UINT32 psmi_preallocation_en : 1; /**< PSMI_PREALLOCATION_EN */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 sw_end_of_capture : 1; /**< SW_END_OF_CAPTURE */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 reset_to_base : 1; /**< RESET_TO_BASE */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 reset_cl_count : 1; /**< RESET_CL_COUNT */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 halt_at_top_en : 1; /**< HALT_AT_MAX */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 bufovrflw_intr_en : 1; /**< BUFOVRFLW_INTR_EN */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 n_8th_intr_en : 8; /**< N_8th_INTR_EN */
                            /* Bits[15:8], default=0x00000000*/
    UINT32 mask : 16; /**< MASK */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GSCPSMI_CONTROL_0_GT_SMCI7_GT_STRUCT;

#define GSCPSMI_CONTROL_0_GT_SMCI7_GT_WIDTH 32
#define GSCPSMI_CONTROL_0_GT_SMCI7_GT_DEFAULT 0x00000000U
#define GSCPSMI_CONTROL_0_GT_SMCI7_GT_ENDPOINT GT

#define GSCPSMI_CONTROL_0_GT_SMCI7_GT_PSMI_EN_LSB 0x0000
#define GSCPSMI_CONTROL_0_GT_SMCI7_GT_PSMI_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI7_GT_PSMI_PREALLOCATION_EN_LSB 0x0002
#define GSCPSMI_CONTROL_0_GT_SMCI7_GT_PSMI_PREALLOCATION_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI7_GT_SW_END_OF_CAPTURE_LSB 0x0003
#define GSCPSMI_CONTROL_0_GT_SMCI7_GT_SW_END_OF_CAPTURE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI7_GT_RESET_TO_BASE_LSB 0x0004
#define GSCPSMI_CONTROL_0_GT_SMCI7_GT_RESET_TO_BASE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI7_GT_RESET_CL_COUNT_LSB 0x0005
#define GSCPSMI_CONTROL_0_GT_SMCI7_GT_RESET_CL_COUNT_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI7_GT_HALT_AT_TOP_EN_LSB 0x0006
#define GSCPSMI_CONTROL_0_GT_SMCI7_GT_HALT_AT_TOP_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI7_GT_BUFOVRFLW_INTR_EN_LSB 0x0007
#define GSCPSMI_CONTROL_0_GT_SMCI7_GT_BUFOVRFLW_INTR_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_GT_SMCI7_GT_N_8TH_INTR_EN_LSB 0x0008
#define GSCPSMI_CONTROL_0_GT_SMCI7_GT_N_8TH_INTR_EN_SIZE 0x0008
#define GSCPSMI_CONTROL_0_GT_SMCI7_GT_MASK_LSB 0x0010
#define GSCPSMI_CONTROL_0_GT_SMCI7_GT_MASK_SIZE 0x0010

#endif      // SV_HOOKS

/** GUC_WOPCM_SIZE_GUC_REG desc:
  */

#define GUC_WOPCM_SIZE_GUC_REG_GT_REG    (0x0000c050U)

typedef union {
  struct {
    UINT32 locked : 1; /**< Locked */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 11; /**< Undefined - auto filled rsvd_[11:01] */
                            /* Bits[11:1], default=0*/
    UINT32 guc_allocated_wopcm_size : 14; /**< GuC allocated WOPCM Size */
                            /* Bits[25:12], default=0x00000100*/
    UINT32 rsvd_1 : 6; /**< Undefined - auto filled rsvd_[31:26] */
                            /* Bits[31:26], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GUC_WOPCM_SIZE_GUC_REG_GT_STRUCT;

#define GUC_WOPCM_SIZE_GUC_REG_GT_WIDTH 32
#define GUC_WOPCM_SIZE_GUC_REG_GT_DEFAULT 0x00100000U
#define GUC_WOPCM_SIZE_GUC_REG_GT_ENDPOINT GT

#define GUC_WOPCM_SIZE_GUC_REG_GT_LOCKED_LSB 0x0000
#define GUC_WOPCM_SIZE_GUC_REG_GT_LOCKED_SIZE 0x0001
#define GUC_WOPCM_SIZE_GUC_REG_GT_GUC_ALLOCATED_WOPCM_SIZE_LSB 0x000c
#define GUC_WOPCM_SIZE_GUC_REG_GT_GUC_ALLOCATED_WOPCM_SIZE_SIZE 0x000e

/** DMA_GUC_WOPCM_OFFSET_GUC_REG desc:
  */

#define DMA_GUC_WOPCM_OFFSET_GUC_REG_GT_REG (0x0000c340U)

typedef union {
  struct {
    UINT32 offset_valid : 1; /**< Offset Valid */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 huc_ukernel_loading_agent : 1; /**< HuC uKernel Loading Agent */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 rsvd_0 : 12; /**< Undefined - auto filled rsvd_[13:02] */
                            /* Bits[13:2], default=0*/
    UINT32 guc_wopcm_offset : 18; /**< GuC WOPCM Offset */
                            /* Bits[31:14], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DMA_GUC_WOPCM_OFFSET_GUC_REG_GT_STRUCT;

#define DMA_GUC_WOPCM_OFFSET_GUC_REG_GT_WIDTH 32
#define DMA_GUC_WOPCM_OFFSET_GUC_REG_GT_DEFAULT 0x00000000U
#define DMA_GUC_WOPCM_OFFSET_GUC_REG_GT_ENDPOINT GT

#define DMA_GUC_WOPCM_OFFSET_GUC_REG_GT_OFFSET_VALID_LSB 0x0000
#define DMA_GUC_WOPCM_OFFSET_GUC_REG_GT_OFFSET_VALID_SIZE 0x0001
#define DMA_GUC_WOPCM_OFFSET_GUC_REG_GT_HUC_UKERNEL_LOADING_AGENT_LSB 0x0001
#define DMA_GUC_WOPCM_OFFSET_GUC_REG_GT_HUC_UKERNEL_LOADING_AGENT_SIZE 0x0001
#define DMA_GUC_WOPCM_OFFSET_GUC_REG_GT_GUC_WOPCM_OFFSET_LSB 0x000e
#define DMA_GUC_WOPCM_OFFSET_GUC_REG_GT_GUC_WOPCM_OFFSET_SIZE 0x0012

/** GUC_PM_MAXCNT_GUC_REG desc:
  */

#define GUC_PM_MAXCNT_GUC_REG_GT_REG     (0x0000c3e4U)

typedef union {
  struct {
    UINT32 idle_wait_time : 20; /**< Idle Wait Time */
                            /* Bits[19:0], default=0x00000040*/
    UINT32 rsvd_0 : 11; /**< Undefined - auto filled rsvd_[30:20] */
                            /* Bits[30:20], default=0*/
    UINT32 guc_power_management_participation_in_idle_flow : 1; /**< GuC Power Management Participation in Idle Flow */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GUC_PM_MAXCNT_GUC_REG_GT_STRUCT;

#define GUC_PM_MAXCNT_GUC_REG_GT_WIDTH 32
#define GUC_PM_MAXCNT_GUC_REG_GT_DEFAULT 0x00000040U
#define GUC_PM_MAXCNT_GUC_REG_GT_ENDPOINT GT

#define GUC_PM_MAXCNT_GUC_REG_GT_IDLE_WAIT_TIME_LSB 0x0000
#define GUC_PM_MAXCNT_GUC_REG_GT_IDLE_WAIT_TIME_SIZE 0x0014
#define GUC_PM_MAXCNT_GUC_REG_GT_GUC_POWER_MANAGEMENT_PARTICIPATION_IN_IDLE_FLOW_LSB 0x001f
#define GUC_PM_MAXCNT_GUC_REG_GT_GUC_POWER_MANAGEMENT_PARTICIPATION_IN_IDLE_FLOW_SIZE 0x0001

/** GAMCTRLLOCK_GAMCTRL_3D desc:
  */

#define GAMCTRLLOCK_GAMCTRL_3D_GT_REG    (0x0000cec0U)

typedef union {
  struct {
    UINT32 lock_reg_cf08 : 1; /**< Lockdown mmio reg cf08 ECO chiken bit register */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 lock_reg_cf54 : 1; /**< Lockdown mmio reg cf54 GAM Defeature ctrl */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 reg_rsv_lock0 : 1; /**< lock dowm mmio register lock bit0 */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 reg_rsv_lock1 : 1; /**< lock dowm mmio register lock bit1 */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 rsvd_0 : 27; /**< Undefined - auto filled rsvd_[30:04] */
                            /* Bits[30:4], default=0*/
    UINT32 gamctrllock_l : 1; /**< Lockdown GAMCTRLLOCK */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GAMCTRLLOCK_GAMCTRL_3D_GT_STRUCT;

#define GAMCTRLLOCK_GAMCTRL_3D_GT_WIDTH 32
#define GAMCTRLLOCK_GAMCTRL_3D_GT_DEFAULT 0x00000000U
#define GAMCTRLLOCK_GAMCTRL_3D_GT_ENDPOINT GT

#define GAMCTRLLOCK_GAMCTRL_3D_GT_LOCK_REG_CF08_LSB 0x0000
#define GAMCTRLLOCK_GAMCTRL_3D_GT_LOCK_REG_CF08_SIZE 0x0001
#define GAMCTRLLOCK_GAMCTRL_3D_GT_LOCK_REG_CF54_LSB 0x0001
#define GAMCTRLLOCK_GAMCTRL_3D_GT_LOCK_REG_CF54_SIZE 0x0001
#define GAMCTRLLOCK_GAMCTRL_3D_GT_REG_RSV_LOCK0_LSB 0x0002
#define GAMCTRLLOCK_GAMCTRL_3D_GT_REG_RSV_LOCK0_SIZE 0x0001
#define GAMCTRLLOCK_GAMCTRL_3D_GT_REG_RSV_LOCK1_LSB 0x0003
#define GAMCTRLLOCK_GAMCTRL_3D_GT_REG_RSV_LOCK1_SIZE 0x0001
#define GAMCTRLLOCK_GAMCTRL_3D_GT_GAMCTRLLOCK_L_LSB 0x001f
#define GAMCTRLLOCK_GAMCTRL_3D_GT_GAMCTRLLOCK_L_SIZE 0x0001

/** ECOCHK_GAMCTRL_3D desc:
  */

#define ECOCHK_GAMCTRL_3D_GT_REG         (0x0000cf08U)

typedef union {
  struct {
    UINT32 dis_hdc_inv : 1; /**< Disable HDC Invalidation */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 en_atomic_rd_ad : 1; /**< Enable Atomic Read for A/D */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 dis_kcr_int : 1; /**< Disable KCR WDDM INTERRUPT generation */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 dis_ctrl_arb_fix : 1; /**< Disable CNTRLCOM MCR arbitration fix */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 dis_ctrl_flw_delay : 1; /**< Disable Control Flow Processing Delay */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 dis_err_rt_ptr_setup : 1; /**< Disable  Error out on root pointer set up */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 dis_flt_drop_on_rst : 1; /**< Disable Flt Response droping on reset */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 dis_mert_skip : 1; /**< Disable MERT Skipping */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 bonus10_7 : 3; /**< bonus10_7 */
                            /* Bits[10:8], default=0x00000000*/
    UINT32 dis_unbind_on_rst_ctxidle : 1; /**< DISABLE UNBIND ON SOFTRST OR CTXIDL */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 last_faultrpt_en : 1; /**< Last Fault Report Enable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 flt_rpt_mode : 1; /**< Legacy Fault Reporting Mode */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 en_pf_report : 1; /**< Enable Page Fault Reporting */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 en_pf_int_guc : 1; /**< Enable Page fault interrupt to GuC */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 en_pf_repair : 1; /**< Enable Page Fault Repair */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 lock_security_chk : 1; /**< Lock Security Check */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 en_security_chk : 1; /**< Enable Security Checks */
                            /* Bits[18:18], default=0x00000001*/
    UINT32 dis_ats_replay : 1; /**< Disable ATS replay */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 stall_fltrsp_oninv : 1; /**< STALL FLTRSP TO MOD ON INV */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 dis_par_caterr_to_fabric : 1; /**< Disable Parity indication to fabric */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 enb_bdst_atsrestart : 1; /**< ENABLE ATS RESTAT BDCAST TO MOD */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 dis_vfid_overd_on_cs_descinv : 1; /**< DISABLE VIFD OVERIDE ON CS DESC INV */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 dis_to_nop_hostinv_on_flr : 1; /**< DISABLE TREATING HOST INV AS NOP DURING FLR */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 en_enginv_mask_on_reset : 1; /**< ENABLE ENG INV MASK ON RESET */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 en_bind_flow_with_highprior : 1; /**< ENABLE BIND FLOW WITH HIGH PRIORITY */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 dbg_eng_sel : 5; /**< Debug Engine Select */
                            /* Bits[31:27], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} ECOCHK_GAMCTRL_3D_GT_STRUCT;

#define ECOCHK_GAMCTRL_3D_GT_WIDTH 32
#define ECOCHK_GAMCTRL_3D_GT_DEFAULT 0x00040000U
#define ECOCHK_GAMCTRL_3D_GT_ENDPOINT GT

#define ECOCHK_GAMCTRL_3D_GT_DIS_HDC_INV_LSB 0x0000
#define ECOCHK_GAMCTRL_3D_GT_DIS_HDC_INV_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_EN_ATOMIC_RD_AD_LSB 0x0001
#define ECOCHK_GAMCTRL_3D_GT_EN_ATOMIC_RD_AD_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_DIS_KCR_INT_LSB 0x0002
#define ECOCHK_GAMCTRL_3D_GT_DIS_KCR_INT_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_DIS_CTRL_ARB_FIX_LSB 0x0003
#define ECOCHK_GAMCTRL_3D_GT_DIS_CTRL_ARB_FIX_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_DIS_CTRL_FLW_DELAY_LSB 0x0004
#define ECOCHK_GAMCTRL_3D_GT_DIS_CTRL_FLW_DELAY_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_DIS_ERR_RT_PTR_SETUP_LSB 0x0005
#define ECOCHK_GAMCTRL_3D_GT_DIS_ERR_RT_PTR_SETUP_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_DIS_FLT_DROP_ON_RST_LSB 0x0006
#define ECOCHK_GAMCTRL_3D_GT_DIS_FLT_DROP_ON_RST_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_DIS_MERT_SKIP_LSB 0x0007
#define ECOCHK_GAMCTRL_3D_GT_DIS_MERT_SKIP_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_BONUS10_7_LSB 0x0008
#define ECOCHK_GAMCTRL_3D_GT_BONUS10_7_SIZE 0x0003
#define ECOCHK_GAMCTRL_3D_GT_DIS_UNBIND_ON_RST_CTXIDLE_LSB 0x000b
#define ECOCHK_GAMCTRL_3D_GT_DIS_UNBIND_ON_RST_CTXIDLE_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_LAST_FAULTRPT_EN_LSB 0x000c
#define ECOCHK_GAMCTRL_3D_GT_LAST_FAULTRPT_EN_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_FLT_RPT_MODE_LSB 0x000d
#define ECOCHK_GAMCTRL_3D_GT_FLT_RPT_MODE_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_EN_PF_REPORT_LSB 0x000e
#define ECOCHK_GAMCTRL_3D_GT_EN_PF_REPORT_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_EN_PF_INT_GUC_LSB 0x000f
#define ECOCHK_GAMCTRL_3D_GT_EN_PF_INT_GUC_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_EN_PF_REPAIR_LSB 0x0010
#define ECOCHK_GAMCTRL_3D_GT_EN_PF_REPAIR_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_LOCK_SECURITY_CHK_LSB 0x0011
#define ECOCHK_GAMCTRL_3D_GT_LOCK_SECURITY_CHK_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_EN_SECURITY_CHK_LSB 0x0012
#define ECOCHK_GAMCTRL_3D_GT_EN_SECURITY_CHK_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_DIS_ATS_REPLAY_LSB 0x0013
#define ECOCHK_GAMCTRL_3D_GT_DIS_ATS_REPLAY_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_STALL_FLTRSP_ONINV_LSB 0x0014
#define ECOCHK_GAMCTRL_3D_GT_STALL_FLTRSP_ONINV_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_DIS_PAR_CATERR_TO_FABRIC_LSB 0x0015
#define ECOCHK_GAMCTRL_3D_GT_DIS_PAR_CATERR_TO_FABRIC_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_ENB_BDST_ATSRESTART_LSB 0x0016
#define ECOCHK_GAMCTRL_3D_GT_ENB_BDST_ATSRESTART_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_DIS_VFID_OVERD_ON_CS_DESCINV_LSB 0x0017
#define ECOCHK_GAMCTRL_3D_GT_DIS_VFID_OVERD_ON_CS_DESCINV_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_DIS_TO_NOP_HOSTINV_ON_FLR_LSB 0x0018
#define ECOCHK_GAMCTRL_3D_GT_DIS_TO_NOP_HOSTINV_ON_FLR_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_EN_ENGINV_MASK_ON_RESET_LSB 0x0019
#define ECOCHK_GAMCTRL_3D_GT_EN_ENGINV_MASK_ON_RESET_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_EN_BIND_FLOW_WITH_HIGHPRIOR_LSB 0x001a
#define ECOCHK_GAMCTRL_3D_GT_EN_BIND_FLOW_WITH_HIGHPRIOR_SIZE 0x0001
#define ECOCHK_GAMCTRL_3D_GT_DBG_ENG_SEL_LSB 0x001b
#define ECOCHK_GAMCTRL_3D_GT_DBG_ENG_SEL_SIZE 0x0005

/** GAMCNTRL_CTRL_GAMCTRL_3D desc:
  */

#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_REG  (0x0000cf54U)

typedef union {
  struct {
    UINT32 cgdis_flct : 1; /**< CG Disable FLWCTRL */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cgdis_comm : 1; /**< CG Disable COMM */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 inv_mode : 1; /**< Global Invalidation Mode */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 go_arb : 1; /**< GO Arbitration */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 inv_ltrstr : 1; /**< Invalidation on Light Restore */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 go_ack_ctl : 1; /**< GO Ack Control */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 rt_rsvdchk : 1; /**< ROOT Entry Reserved Check */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 ctx_rsvdchk : 1; /**< Context Entry Reserved Check */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 go_stall : 1; /**< Stall on GO_0 */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 flr_timeout_en : 1; /**< Timeout enabled for FLR */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 en_flush_flr : 1; /**< Enable Fabric Flush for FLR */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 en_flush_on_pwrdn : 1; /**< Enable fabric flush on power down */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 dis_bcast_inval : 1; /**< Disable Broadcast Mode of Invalidations */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 dis_flt_retry_to : 1; /**< Disable Fault Semaphone retry timeout */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 g2g_lnk_cg_dis : 1; /**< Disable GAM2GAM Link Clock Gating */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 dis_cs_cat_from_slave : 1; /**< Disable CS CAT INT reporting from Slave Gam */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 dis_kcr_rst_in_prog : 1; /**< Disable KCR Reset in Progress */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 dis_g2g_pend_ctx_flow : 1; /**< Disable Multi GAM G2G CTX Flow */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 skip_cflush_ack : 1; /**< Skip CFlush Ack in TLB Invalidation */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 en_stpdrn_on_ctxidle : 1; /**< SEND STOP AND DRAIN ON CTX IDLE */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 timeout_val : 12; /**< Timeout Value */
                            /* Bits[31:20], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GAMCNTRL_CTRL_GAMCTRL_3D_GT_STRUCT;

#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_WIDTH 32
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_DEFAULT 0x00000000U
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_ENDPOINT GT

#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_CGDIS_FLCT_LSB 0x0000
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_CGDIS_FLCT_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_CGDIS_COMM_LSB 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_CGDIS_COMM_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_INV_MODE_LSB 0x0002
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_INV_MODE_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_GO_ARB_LSB 0x0003
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_GO_ARB_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_INV_LTRSTR_LSB 0x0004
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_INV_LTRSTR_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_GO_ACK_CTL_LSB 0x0005
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_GO_ACK_CTL_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_RT_RSVDCHK_LSB 0x0006
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_RT_RSVDCHK_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_CTX_RSVDCHK_LSB 0x0007
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_CTX_RSVDCHK_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_GO_STALL_LSB 0x0008
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_GO_STALL_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_FLR_TIMEOUT_EN_LSB 0x0009
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_FLR_TIMEOUT_EN_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_EN_FLUSH_FLR_LSB 0x000a
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_EN_FLUSH_FLR_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_EN_FLUSH_ON_PWRDN_LSB 0x000b
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_EN_FLUSH_ON_PWRDN_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_DIS_BCAST_INVAL_LSB 0x000c
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_DIS_BCAST_INVAL_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_DIS_FLT_RETRY_TO_LSB 0x000d
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_DIS_FLT_RETRY_TO_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_G2G_LNK_CG_DIS_LSB 0x000e
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_G2G_LNK_CG_DIS_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_DIS_CS_CAT_FROM_SLAVE_LSB 0x000f
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_DIS_CS_CAT_FROM_SLAVE_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_DIS_KCR_RST_IN_PROG_LSB 0x0010
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_DIS_KCR_RST_IN_PROG_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_DIS_G2G_PEND_CTX_FLOW_LSB 0x0011
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_DIS_G2G_PEND_CTX_FLOW_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_SKIP_CFLUSH_ACK_LSB 0x0012
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_SKIP_CFLUSH_ACK_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_EN_STPDRN_ON_CTXIDLE_LSB 0x0013
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_EN_STPDRN_ON_CTXIDLE_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_TIMEOUT_VAL_LSB 0x0014
#define GAMCNTRL_CTRL_GAMCTRL_3D_GT_TIMEOUT_VAL_SIZE 0x000c

#ifdef SV_HOOKS

/** RC_PSMI_CTRL_CCSUNIT0 desc:
  */

#define RC_PSMI_CTRL_CCSUNIT0_GT_REG     (0x0001a050U)

typedef union {
  struct {
    UINT32 rc__idle_message_disable : 1; /**< RC* IDLE Message Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 message_bypass : 1; /**< Message Bypass */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 idle_flush_disable : 1; /**< IDLE Flush Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 idle_indicator : 1; /**< IDLE Indicator */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 go_indicator : 1; /**< GO Indicator */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 disable_idle_dop_flush_pipelining : 1; /**< Disable IDLE DOP flush pipelining */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 enable_idle_rc6_flush : 1; /**< Enable IDLE RC6 flush */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 sync_flip_v_blank_wait_for_event_power_down_disable : 1; /**< Sync Flip/V-blank Wait For Event Power Down Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[08:08] */
                            /* Bits[8:8], default=0*/
    UINT32 debug_flow_for_go_0_acknowledge : 1; /**< Debug Flow for GO=0 Acknowledge */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[10:10] */
                            /* Bits[10:10], default=0*/
    UINT32 psmi_parser_block : 1; /**< PSMI Parser Block */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 semaphore_wait_event_idle_message_disable : 1; /**< Semaphore Wait Event IDLE message Disable */
                            /* Bits[12:12], default=0x00000001*/
    UINT32 idle_message_override_control : 2; /**< IDLE Message Override Control */
                            /* Bits[14:13], default=0x00000000*/
    UINT32 inhibit_rc6_entry_on_wait_for_scanline : 1; /**< Inhibit RC6 Entry on Wait For Scanline */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 mask : 16; /**< Mask */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RC_PSMI_CTRL_CCSUNIT0_GT_STRUCT;

#define RC_PSMI_CTRL_CCSUNIT0_GT_WIDTH 32
#define RC_PSMI_CTRL_CCSUNIT0_GT_DEFAULT 0x00001000U
#define RC_PSMI_CTRL_CCSUNIT0_GT_ENDPOINT GT

#define RC_PSMI_CTRL_CCSUNIT0_GT_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_CCSUNIT0_GT_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT0_GT_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_CCSUNIT0_GT_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT0_GT_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_CCSUNIT0_GT_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT0_GT_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_CCSUNIT0_GT_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT0_GT_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_CCSUNIT0_GT_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT0_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_CCSUNIT0_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT0_GT_ENABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_CCSUNIT0_GT_ENABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT0_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_CCSUNIT0_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT0_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_CCSUNIT0_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT0_GT_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_CCSUNIT0_GT_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT0_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_CCSUNIT0_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT0_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_CCSUNIT0_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_CCSUNIT0_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_CCSUNIT0_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT0_GT_MASK_LSB 0x0010
#define RC_PSMI_CTRL_CCSUNIT0_GT_MASK_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** PWRCTX_MAXCNT_CCSUNIT0 desc:
  */

#define PWRCTX_MAXCNT_CCSUNIT0_GT_REG    (0x0001a054U)

typedef union {
  struct {
    UINT32 idle_wait_time : 20; /**< IDLE Wait Time */
                            /* Bits[19:0], default=0x0000000A*/
    UINT32 rsvd_0 : 12; /**< Undefined - auto filled rsvd_[31:20] */
                            /* Bits[31:20], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PWRCTX_MAXCNT_CCSUNIT0_GT_STRUCT;

#define PWRCTX_MAXCNT_CCSUNIT0_GT_WIDTH 32
#define PWRCTX_MAXCNT_CCSUNIT0_GT_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_CCSUNIT0_GT_ENDPOINT GT

#define PWRCTX_MAXCNT_CCSUNIT0_GT_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_CCSUNIT0_GT_IDLE_WAIT_TIME_SIZE 0x0014

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** RC_PSMI_CTRL_CCSUNIT1 desc:
  */

#define RC_PSMI_CTRL_CCSUNIT1_GT_REG     (0x0001c050U)

typedef union {
  struct {
    UINT32 rc__idle_message_disable : 1; /**< RC* IDLE Message Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 message_bypass : 1; /**< Message Bypass */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 idle_flush_disable : 1; /**< IDLE Flush Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 idle_indicator : 1; /**< IDLE Indicator */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 go_indicator : 1; /**< GO Indicator */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 disable_idle_dop_flush_pipelining : 1; /**< Disable IDLE DOP flush pipelining */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 enable_idle_rc6_flush : 1; /**< Enable IDLE RC6 flush */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 sync_flip_v_blank_wait_for_event_power_down_disable : 1; /**< Sync Flip/V-blank Wait For Event Power Down Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[08:08] */
                            /* Bits[8:8], default=0*/
    UINT32 debug_flow_for_go_0_acknowledge : 1; /**< Debug Flow for GO=0 Acknowledge */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[10:10] */
                            /* Bits[10:10], default=0*/
    UINT32 psmi_parser_block : 1; /**< PSMI Parser Block */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 semaphore_wait_event_idle_message_disable : 1; /**< Semaphore Wait Event IDLE message Disable */
                            /* Bits[12:12], default=0x00000001*/
    UINT32 idle_message_override_control : 2; /**< IDLE Message Override Control */
                            /* Bits[14:13], default=0x00000000*/
    UINT32 inhibit_rc6_entry_on_wait_for_scanline : 1; /**< Inhibit RC6 Entry on Wait For Scanline */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 mask : 16; /**< Mask */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RC_PSMI_CTRL_CCSUNIT1_GT_STRUCT;

#define RC_PSMI_CTRL_CCSUNIT1_GT_WIDTH 32
#define RC_PSMI_CTRL_CCSUNIT1_GT_DEFAULT 0x00001000U
#define RC_PSMI_CTRL_CCSUNIT1_GT_ENDPOINT GT

#define RC_PSMI_CTRL_CCSUNIT1_GT_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_CCSUNIT1_GT_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT1_GT_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_CCSUNIT1_GT_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT1_GT_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_CCSUNIT1_GT_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT1_GT_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_CCSUNIT1_GT_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT1_GT_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_CCSUNIT1_GT_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT1_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_CCSUNIT1_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT1_GT_ENABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_CCSUNIT1_GT_ENABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT1_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_CCSUNIT1_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT1_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_CCSUNIT1_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT1_GT_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_CCSUNIT1_GT_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT1_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_CCSUNIT1_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT1_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_CCSUNIT1_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_CCSUNIT1_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_CCSUNIT1_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT1_GT_MASK_LSB 0x0010
#define RC_PSMI_CTRL_CCSUNIT1_GT_MASK_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** PWRCTX_MAXCNT_CCSUNIT1 desc:
  */

#define PWRCTX_MAXCNT_CCSUNIT1_GT_REG    (0x0001c054U)

typedef union {
  struct {
    UINT32 idle_wait_time : 20; /**< IDLE Wait Time */
                            /* Bits[19:0], default=0x0000000A*/
    UINT32 rsvd_0 : 12; /**< Undefined - auto filled rsvd_[31:20] */
                            /* Bits[31:20], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PWRCTX_MAXCNT_CCSUNIT1_GT_STRUCT;

#define PWRCTX_MAXCNT_CCSUNIT1_GT_WIDTH 32
#define PWRCTX_MAXCNT_CCSUNIT1_GT_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_CCSUNIT1_GT_ENDPOINT GT

#define PWRCTX_MAXCNT_CCSUNIT1_GT_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_CCSUNIT1_GT_IDLE_WAIT_TIME_SIZE 0x0014

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** RC_PSMI_CTRL_CCSUNIT2 desc:
  */

#define RC_PSMI_CTRL_CCSUNIT2_GT_REG     (0x0001e050U)

typedef union {
  struct {
    UINT32 rc__idle_message_disable : 1; /**< RC* IDLE Message Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 message_bypass : 1; /**< Message Bypass */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 idle_flush_disable : 1; /**< IDLE Flush Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 idle_indicator : 1; /**< IDLE Indicator */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 go_indicator : 1; /**< GO Indicator */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 disable_idle_dop_flush_pipelining : 1; /**< Disable IDLE DOP flush pipelining */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 enable_idle_rc6_flush : 1; /**< Enable IDLE RC6 flush */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 sync_flip_v_blank_wait_for_event_power_down_disable : 1; /**< Sync Flip/V-blank Wait For Event Power Down Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[08:08] */
                            /* Bits[8:8], default=0*/
    UINT32 debug_flow_for_go_0_acknowledge : 1; /**< Debug Flow for GO=0 Acknowledge */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[10:10] */
                            /* Bits[10:10], default=0*/
    UINT32 psmi_parser_block : 1; /**< PSMI Parser Block */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 semaphore_wait_event_idle_message_disable : 1; /**< Semaphore Wait Event IDLE message Disable */
                            /* Bits[12:12], default=0x00000001*/
    UINT32 idle_message_override_control : 2; /**< IDLE Message Override Control */
                            /* Bits[14:13], default=0x00000000*/
    UINT32 inhibit_rc6_entry_on_wait_for_scanline : 1; /**< Inhibit RC6 Entry on Wait For Scanline */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 mask : 16; /**< Mask */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RC_PSMI_CTRL_CCSUNIT2_GT_STRUCT;

#define RC_PSMI_CTRL_CCSUNIT2_GT_WIDTH 32
#define RC_PSMI_CTRL_CCSUNIT2_GT_DEFAULT 0x00001000U
#define RC_PSMI_CTRL_CCSUNIT2_GT_ENDPOINT GT

#define RC_PSMI_CTRL_CCSUNIT2_GT_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_CCSUNIT2_GT_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT2_GT_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_CCSUNIT2_GT_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT2_GT_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_CCSUNIT2_GT_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT2_GT_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_CCSUNIT2_GT_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT2_GT_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_CCSUNIT2_GT_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT2_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_CCSUNIT2_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT2_GT_ENABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_CCSUNIT2_GT_ENABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT2_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_CCSUNIT2_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT2_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_CCSUNIT2_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT2_GT_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_CCSUNIT2_GT_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT2_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_CCSUNIT2_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT2_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_CCSUNIT2_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_CCSUNIT2_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_CCSUNIT2_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT2_GT_MASK_LSB 0x0010
#define RC_PSMI_CTRL_CCSUNIT2_GT_MASK_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** PWRCTX_MAXCNT_CCSUNIT2 desc:
  */

#define PWRCTX_MAXCNT_CCSUNIT2_GT_REG    (0x0001e054U)

typedef union {
  struct {
    UINT32 idle_wait_time : 20; /**< IDLE Wait Time */
                            /* Bits[19:0], default=0x0000000A*/
    UINT32 rsvd_0 : 12; /**< Undefined - auto filled rsvd_[31:20] */
                            /* Bits[31:20], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PWRCTX_MAXCNT_CCSUNIT2_GT_STRUCT;

#define PWRCTX_MAXCNT_CCSUNIT2_GT_WIDTH 32
#define PWRCTX_MAXCNT_CCSUNIT2_GT_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_CCSUNIT2_GT_ENDPOINT GT

#define PWRCTX_MAXCNT_CCSUNIT2_GT_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_CCSUNIT2_GT_IDLE_WAIT_TIME_SIZE 0x0014

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** RC_PSMI_CTRL_BCSUNIT0 desc:
  */

#define RC_PSMI_CTRL_BCSUNIT0_GT_REG     (0x00022050U)

typedef union {
  struct {
    UINT32 rc__idle_message_disable : 1; /**< RC* IDLE Message Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 message_bypass : 1; /**< Message Bypass */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 idle_flush_disable : 1; /**< IDLE Flush Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 idle_indicator : 1; /**< IDLE Indicator */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 go_indicator : 1; /**< GO Indicator */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 disable_idle_dop_flush_pipelining : 1; /**< Disable IDLE DOP flush pipelining */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 enable_idle_rc6_flush : 1; /**< Enable IDLE RC6 flush */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 sync_flip_v_blank_wait_for_event_power_down_disable : 1; /**< Sync Flip/V-blank Wait For Event Power Down Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[08:08] */
                            /* Bits[8:8], default=0*/
    UINT32 debug_flow_for_go_0_acknowledge : 1; /**< Debug Flow for GO=0 Acknowledge */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[10:10] */
                            /* Bits[10:10], default=0*/
    UINT32 psmi_parser_block : 1; /**< PSMI Parser Block */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 semaphore_wait_event_idle_message_disable : 1; /**< Semaphore Wait Event IDLE message Disable */
                            /* Bits[12:12], default=0x00000001*/
    UINT32 idle_message_override_control : 2; /**< IDLE Message Override Control */
                            /* Bits[14:13], default=0x00000000*/
    UINT32 inhibit_rc6_entry_on_wait_for_scanline : 1; /**< Inhibit RC6 Entry on Wait For Scanline */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 mask : 16; /**< Mask */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RC_PSMI_CTRL_BCSUNIT0_GT_STRUCT;

#define RC_PSMI_CTRL_BCSUNIT0_GT_WIDTH 32
#define RC_PSMI_CTRL_BCSUNIT0_GT_DEFAULT 0x00001000U
#define RC_PSMI_CTRL_BCSUNIT0_GT_ENDPOINT GT

#define RC_PSMI_CTRL_BCSUNIT0_GT_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_BCSUNIT0_GT_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT0_GT_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_BCSUNIT0_GT_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT0_GT_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_BCSUNIT0_GT_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT0_GT_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_BCSUNIT0_GT_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT0_GT_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_BCSUNIT0_GT_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT0_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_BCSUNIT0_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT0_GT_ENABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_BCSUNIT0_GT_ENABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT0_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_BCSUNIT0_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT0_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_BCSUNIT0_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT0_GT_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_BCSUNIT0_GT_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT0_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_BCSUNIT0_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT0_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_BCSUNIT0_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_BCSUNIT0_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_BCSUNIT0_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT0_GT_MASK_LSB 0x0010
#define RC_PSMI_CTRL_BCSUNIT0_GT_MASK_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** PWRCTX_MAXCNT_BCSUNIT0 desc:
  */

#define PWRCTX_MAXCNT_BCSUNIT0_GT_REG    (0x00022054U)

typedef union {
  struct {
    UINT32 idle_wait_time : 20; /**< IDLE Wait Time */
                            /* Bits[19:0], default=0x0000000A*/
    UINT32 rsvd_0 : 12; /**< Undefined - auto filled rsvd_[31:20] */
                            /* Bits[31:20], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PWRCTX_MAXCNT_BCSUNIT0_GT_STRUCT;

#define PWRCTX_MAXCNT_BCSUNIT0_GT_WIDTH 32
#define PWRCTX_MAXCNT_BCSUNIT0_GT_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_BCSUNIT0_GT_ENDPOINT GT

#define PWRCTX_MAXCNT_BCSUNIT0_GT_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_BCSUNIT0_GT_IDLE_WAIT_TIME_SIZE 0x0014

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** RC_PSMI_CTRL_CCSUNIT3 desc:
  */

#define RC_PSMI_CTRL_CCSUNIT3_GT_REG     (0x00026050U)

typedef union {
  struct {
    UINT32 rc__idle_message_disable : 1; /**< RC* IDLE Message Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 message_bypass : 1; /**< Message Bypass */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 idle_flush_disable : 1; /**< IDLE Flush Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 idle_indicator : 1; /**< IDLE Indicator */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 go_indicator : 1; /**< GO Indicator */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 disable_idle_dop_flush_pipelining : 1; /**< Disable IDLE DOP flush pipelining */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 enable_idle_rc6_flush : 1; /**< Enable IDLE RC6 flush */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 sync_flip_v_blank_wait_for_event_power_down_disable : 1; /**< Sync Flip/V-blank Wait For Event Power Down Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[08:08] */
                            /* Bits[8:8], default=0*/
    UINT32 debug_flow_for_go_0_acknowledge : 1; /**< Debug Flow for GO=0 Acknowledge */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[10:10] */
                            /* Bits[10:10], default=0*/
    UINT32 psmi_parser_block : 1; /**< PSMI Parser Block */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 semaphore_wait_event_idle_message_disable : 1; /**< Semaphore Wait Event IDLE message Disable */
                            /* Bits[12:12], default=0x00000001*/
    UINT32 idle_message_override_control : 2; /**< IDLE Message Override Control */
                            /* Bits[14:13], default=0x00000000*/
    UINT32 inhibit_rc6_entry_on_wait_for_scanline : 1; /**< Inhibit RC6 Entry on Wait For Scanline */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 mask : 16; /**< Mask */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RC_PSMI_CTRL_CCSUNIT3_GT_STRUCT;

#define RC_PSMI_CTRL_CCSUNIT3_GT_WIDTH 32
#define RC_PSMI_CTRL_CCSUNIT3_GT_DEFAULT 0x00001000U
#define RC_PSMI_CTRL_CCSUNIT3_GT_ENDPOINT GT

#define RC_PSMI_CTRL_CCSUNIT3_GT_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_CCSUNIT3_GT_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT3_GT_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_CCSUNIT3_GT_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT3_GT_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_CCSUNIT3_GT_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT3_GT_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_CCSUNIT3_GT_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT3_GT_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_CCSUNIT3_GT_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT3_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_CCSUNIT3_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT3_GT_ENABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_CCSUNIT3_GT_ENABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT3_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_CCSUNIT3_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT3_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_CCSUNIT3_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT3_GT_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_CCSUNIT3_GT_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT3_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_CCSUNIT3_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT3_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_CCSUNIT3_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_CCSUNIT3_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_CCSUNIT3_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_CCSUNIT3_GT_MASK_LSB 0x0010
#define RC_PSMI_CTRL_CCSUNIT3_GT_MASK_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** PWRCTX_MAXCNT_CCSUNIT3 desc:
  */

#define PWRCTX_MAXCNT_CCSUNIT3_GT_REG    (0x00026054U)

typedef union {
  struct {
    UINT32 idle_wait_time : 20; /**< IDLE Wait Time */
                            /* Bits[19:0], default=0x0000000A*/
    UINT32 rsvd_0 : 12; /**< Undefined - auto filled rsvd_[31:20] */
                            /* Bits[31:20], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PWRCTX_MAXCNT_CCSUNIT3_GT_STRUCT;

#define PWRCTX_MAXCNT_CCSUNIT3_GT_WIDTH 32
#define PWRCTX_MAXCNT_CCSUNIT3_GT_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_CCSUNIT3_GT_ENDPOINT GT

#define PWRCTX_MAXCNT_CCSUNIT3_GT_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_CCSUNIT3_GT_IDLE_WAIT_TIME_SIZE 0x0014

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** RC_PSMI_CTRL_BCSUNIT1 desc:
  */

#define RC_PSMI_CTRL_BCSUNIT1_GT_REG     (0x003e0050U)

typedef union {
  struct {
    UINT32 rc__idle_message_disable : 1; /**< RC* IDLE Message Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 message_bypass : 1; /**< Message Bypass */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 idle_flush_disable : 1; /**< IDLE Flush Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 idle_indicator : 1; /**< IDLE Indicator */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 go_indicator : 1; /**< GO Indicator */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 disable_idle_dop_flush_pipelining : 1; /**< Disable IDLE DOP flush pipelining */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 enable_idle_rc6_flush : 1; /**< Enable IDLE RC6 flush */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 sync_flip_v_blank_wait_for_event_power_down_disable : 1; /**< Sync Flip/V-blank Wait For Event Power Down Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[08:08] */
                            /* Bits[8:8], default=0*/
    UINT32 debug_flow_for_go_0_acknowledge : 1; /**< Debug Flow for GO=0 Acknowledge */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[10:10] */
                            /* Bits[10:10], default=0*/
    UINT32 psmi_parser_block : 1; /**< PSMI Parser Block */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 semaphore_wait_event_idle_message_disable : 1; /**< Semaphore Wait Event IDLE message Disable */
                            /* Bits[12:12], default=0x00000001*/
    UINT32 idle_message_override_control : 2; /**< IDLE Message Override Control */
                            /* Bits[14:13], default=0x00000000*/
    UINT32 inhibit_rc6_entry_on_wait_for_scanline : 1; /**< Inhibit RC6 Entry on Wait For Scanline */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 mask : 16; /**< Mask */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RC_PSMI_CTRL_BCSUNIT1_GT_STRUCT;

#define RC_PSMI_CTRL_BCSUNIT1_GT_WIDTH 32
#define RC_PSMI_CTRL_BCSUNIT1_GT_DEFAULT 0x00001000U
#define RC_PSMI_CTRL_BCSUNIT1_GT_ENDPOINT GT

#define RC_PSMI_CTRL_BCSUNIT1_GT_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_BCSUNIT1_GT_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT1_GT_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_BCSUNIT1_GT_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT1_GT_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_BCSUNIT1_GT_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT1_GT_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_BCSUNIT1_GT_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT1_GT_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_BCSUNIT1_GT_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT1_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_BCSUNIT1_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT1_GT_ENABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_BCSUNIT1_GT_ENABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT1_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_BCSUNIT1_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT1_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_BCSUNIT1_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT1_GT_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_BCSUNIT1_GT_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT1_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_BCSUNIT1_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT1_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_BCSUNIT1_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_BCSUNIT1_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_BCSUNIT1_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT1_GT_MASK_LSB 0x0010
#define RC_PSMI_CTRL_BCSUNIT1_GT_MASK_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** PWRCTX_MAXCNT_BCSUNIT1 desc:
  */

#define PWRCTX_MAXCNT_BCSUNIT1_GT_REG    (0x003e0054U)

typedef union {
  struct {
    UINT32 idle_wait_time : 20; /**< IDLE Wait Time */
                            /* Bits[19:0], default=0x0000000A*/
    UINT32 rsvd_0 : 12; /**< Undefined - auto filled rsvd_[31:20] */
                            /* Bits[31:20], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PWRCTX_MAXCNT_BCSUNIT1_GT_STRUCT;

#define PWRCTX_MAXCNT_BCSUNIT1_GT_WIDTH 32
#define PWRCTX_MAXCNT_BCSUNIT1_GT_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_BCSUNIT1_GT_ENDPOINT GT

#define PWRCTX_MAXCNT_BCSUNIT1_GT_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_BCSUNIT1_GT_IDLE_WAIT_TIME_SIZE 0x0014

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** RC_PSMI_CTRL_BCSUNIT2 desc:
  */

#define RC_PSMI_CTRL_BCSUNIT2_GT_REG     (0x003e2050U)

typedef union {
  struct {
    UINT32 rc__idle_message_disable : 1; /**< RC* IDLE Message Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 message_bypass : 1; /**< Message Bypass */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 idle_flush_disable : 1; /**< IDLE Flush Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 idle_indicator : 1; /**< IDLE Indicator */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 go_indicator : 1; /**< GO Indicator */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 disable_idle_dop_flush_pipelining : 1; /**< Disable IDLE DOP flush pipelining */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 enable_idle_rc6_flush : 1; /**< Enable IDLE RC6 flush */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 sync_flip_v_blank_wait_for_event_power_down_disable : 1; /**< Sync Flip/V-blank Wait For Event Power Down Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[08:08] */
                            /* Bits[8:8], default=0*/
    UINT32 debug_flow_for_go_0_acknowledge : 1; /**< Debug Flow for GO=0 Acknowledge */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[10:10] */
                            /* Bits[10:10], default=0*/
    UINT32 psmi_parser_block : 1; /**< PSMI Parser Block */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 semaphore_wait_event_idle_message_disable : 1; /**< Semaphore Wait Event IDLE message Disable */
                            /* Bits[12:12], default=0x00000001*/
    UINT32 idle_message_override_control : 2; /**< IDLE Message Override Control */
                            /* Bits[14:13], default=0x00000000*/
    UINT32 inhibit_rc6_entry_on_wait_for_scanline : 1; /**< Inhibit RC6 Entry on Wait For Scanline */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 mask : 16; /**< Mask */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RC_PSMI_CTRL_BCSUNIT2_GT_STRUCT;

#define RC_PSMI_CTRL_BCSUNIT2_GT_WIDTH 32
#define RC_PSMI_CTRL_BCSUNIT2_GT_DEFAULT 0x00001000U
#define RC_PSMI_CTRL_BCSUNIT2_GT_ENDPOINT GT

#define RC_PSMI_CTRL_BCSUNIT2_GT_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_BCSUNIT2_GT_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT2_GT_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_BCSUNIT2_GT_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT2_GT_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_BCSUNIT2_GT_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT2_GT_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_BCSUNIT2_GT_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT2_GT_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_BCSUNIT2_GT_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT2_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_BCSUNIT2_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT2_GT_ENABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_BCSUNIT2_GT_ENABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT2_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_BCSUNIT2_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT2_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_BCSUNIT2_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT2_GT_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_BCSUNIT2_GT_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT2_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_BCSUNIT2_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT2_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_BCSUNIT2_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_BCSUNIT2_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_BCSUNIT2_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT2_GT_MASK_LSB 0x0010
#define RC_PSMI_CTRL_BCSUNIT2_GT_MASK_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** PWRCTX_MAXCNT_BCSUNIT2 desc:
  */

#define PWRCTX_MAXCNT_BCSUNIT2_GT_REG    (0x003e2054U)

typedef union {
  struct {
    UINT32 idle_wait_time : 20; /**< IDLE Wait Time */
                            /* Bits[19:0], default=0x0000000A*/
    UINT32 rsvd_0 : 12; /**< Undefined - auto filled rsvd_[31:20] */
                            /* Bits[31:20], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PWRCTX_MAXCNT_BCSUNIT2_GT_STRUCT;

#define PWRCTX_MAXCNT_BCSUNIT2_GT_WIDTH 32
#define PWRCTX_MAXCNT_BCSUNIT2_GT_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_BCSUNIT2_GT_ENDPOINT GT

#define PWRCTX_MAXCNT_BCSUNIT2_GT_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_BCSUNIT2_GT_IDLE_WAIT_TIME_SIZE 0x0014

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** RC_PSMI_CTRL_BCSUNIT3 desc:
  */

#define RC_PSMI_CTRL_BCSUNIT3_GT_REG     (0x003e4050U)

typedef union {
  struct {
    UINT32 rc__idle_message_disable : 1; /**< RC* IDLE Message Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 message_bypass : 1; /**< Message Bypass */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 idle_flush_disable : 1; /**< IDLE Flush Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 idle_indicator : 1; /**< IDLE Indicator */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 go_indicator : 1; /**< GO Indicator */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 disable_idle_dop_flush_pipelining : 1; /**< Disable IDLE DOP flush pipelining */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 enable_idle_rc6_flush : 1; /**< Enable IDLE RC6 flush */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 sync_flip_v_blank_wait_for_event_power_down_disable : 1; /**< Sync Flip/V-blank Wait For Event Power Down Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[08:08] */
                            /* Bits[8:8], default=0*/
    UINT32 debug_flow_for_go_0_acknowledge : 1; /**< Debug Flow for GO=0 Acknowledge */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[10:10] */
                            /* Bits[10:10], default=0*/
    UINT32 psmi_parser_block : 1; /**< PSMI Parser Block */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 semaphore_wait_event_idle_message_disable : 1; /**< Semaphore Wait Event IDLE message Disable */
                            /* Bits[12:12], default=0x00000001*/
    UINT32 idle_message_override_control : 2; /**< IDLE Message Override Control */
                            /* Bits[14:13], default=0x00000000*/
    UINT32 inhibit_rc6_entry_on_wait_for_scanline : 1; /**< Inhibit RC6 Entry on Wait For Scanline */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 mask : 16; /**< Mask */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RC_PSMI_CTRL_BCSUNIT3_GT_STRUCT;

#define RC_PSMI_CTRL_BCSUNIT3_GT_WIDTH 32
#define RC_PSMI_CTRL_BCSUNIT3_GT_DEFAULT 0x00001000U
#define RC_PSMI_CTRL_BCSUNIT3_GT_ENDPOINT GT

#define RC_PSMI_CTRL_BCSUNIT3_GT_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_BCSUNIT3_GT_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT3_GT_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_BCSUNIT3_GT_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT3_GT_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_BCSUNIT3_GT_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT3_GT_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_BCSUNIT3_GT_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT3_GT_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_BCSUNIT3_GT_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT3_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_BCSUNIT3_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT3_GT_ENABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_BCSUNIT3_GT_ENABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT3_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_BCSUNIT3_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT3_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_BCSUNIT3_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT3_GT_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_BCSUNIT3_GT_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT3_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_BCSUNIT3_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT3_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_BCSUNIT3_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_BCSUNIT3_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_BCSUNIT3_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT3_GT_MASK_LSB 0x0010
#define RC_PSMI_CTRL_BCSUNIT3_GT_MASK_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** PWRCTX_MAXCNT_BCSUNIT3 desc:
  */

#define PWRCTX_MAXCNT_BCSUNIT3_GT_REG    (0x003e4054U)

typedef union {
  struct {
    UINT32 idle_wait_time : 20; /**< IDLE Wait Time */
                            /* Bits[19:0], default=0x0000000A*/
    UINT32 rsvd_0 : 12; /**< Undefined - auto filled rsvd_[31:20] */
                            /* Bits[31:20], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PWRCTX_MAXCNT_BCSUNIT3_GT_STRUCT;

#define PWRCTX_MAXCNT_BCSUNIT3_GT_WIDTH 32
#define PWRCTX_MAXCNT_BCSUNIT3_GT_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_BCSUNIT3_GT_ENDPOINT GT

#define PWRCTX_MAXCNT_BCSUNIT3_GT_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_BCSUNIT3_GT_IDLE_WAIT_TIME_SIZE 0x0014

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** RC_PSMI_CTRL_BCSUNIT4 desc:
  */

#define RC_PSMI_CTRL_BCSUNIT4_GT_REG     (0x003e6050U)

typedef union {
  struct {
    UINT32 rc__idle_message_disable : 1; /**< RC* IDLE Message Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 message_bypass : 1; /**< Message Bypass */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 idle_flush_disable : 1; /**< IDLE Flush Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 idle_indicator : 1; /**< IDLE Indicator */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 go_indicator : 1; /**< GO Indicator */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 disable_idle_dop_flush_pipelining : 1; /**< Disable IDLE DOP flush pipelining */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 enable_idle_rc6_flush : 1; /**< Enable IDLE RC6 flush */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 sync_flip_v_blank_wait_for_event_power_down_disable : 1; /**< Sync Flip/V-blank Wait For Event Power Down Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[08:08] */
                            /* Bits[8:8], default=0*/
    UINT32 debug_flow_for_go_0_acknowledge : 1; /**< Debug Flow for GO=0 Acknowledge */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[10:10] */
                            /* Bits[10:10], default=0*/
    UINT32 psmi_parser_block : 1; /**< PSMI Parser Block */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 semaphore_wait_event_idle_message_disable : 1; /**< Semaphore Wait Event IDLE message Disable */
                            /* Bits[12:12], default=0x00000001*/
    UINT32 idle_message_override_control : 2; /**< IDLE Message Override Control */
                            /* Bits[14:13], default=0x00000000*/
    UINT32 inhibit_rc6_entry_on_wait_for_scanline : 1; /**< Inhibit RC6 Entry on Wait For Scanline */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 mask : 16; /**< Mask */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RC_PSMI_CTRL_BCSUNIT4_GT_STRUCT;

#define RC_PSMI_CTRL_BCSUNIT4_GT_WIDTH 32
#define RC_PSMI_CTRL_BCSUNIT4_GT_DEFAULT 0x00001000U
#define RC_PSMI_CTRL_BCSUNIT4_GT_ENDPOINT GT

#define RC_PSMI_CTRL_BCSUNIT4_GT_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_BCSUNIT4_GT_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT4_GT_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_BCSUNIT4_GT_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT4_GT_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_BCSUNIT4_GT_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT4_GT_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_BCSUNIT4_GT_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT4_GT_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_BCSUNIT4_GT_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT4_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_BCSUNIT4_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT4_GT_ENABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_BCSUNIT4_GT_ENABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT4_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_BCSUNIT4_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT4_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_BCSUNIT4_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT4_GT_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_BCSUNIT4_GT_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT4_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_BCSUNIT4_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT4_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_BCSUNIT4_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_BCSUNIT4_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_BCSUNIT4_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT4_GT_MASK_LSB 0x0010
#define RC_PSMI_CTRL_BCSUNIT4_GT_MASK_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** PWRCTX_MAXCNT_BCSUNIT4 desc:
  */

#define PWRCTX_MAXCNT_BCSUNIT4_GT_REG    (0x003e6054U)

typedef union {
  struct {
    UINT32 idle_wait_time : 20; /**< IDLE Wait Time */
                            /* Bits[19:0], default=0x0000000A*/
    UINT32 rsvd_0 : 12; /**< Undefined - auto filled rsvd_[31:20] */
                            /* Bits[31:20], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PWRCTX_MAXCNT_BCSUNIT4_GT_STRUCT;

#define PWRCTX_MAXCNT_BCSUNIT4_GT_WIDTH 32
#define PWRCTX_MAXCNT_BCSUNIT4_GT_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_BCSUNIT4_GT_ENDPOINT GT

#define PWRCTX_MAXCNT_BCSUNIT4_GT_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_BCSUNIT4_GT_IDLE_WAIT_TIME_SIZE 0x0014

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** RC_PSMI_CTRL_BCSUNIT5 desc:
  */

#define RC_PSMI_CTRL_BCSUNIT5_GT_REG     (0x003e8050U)

typedef union {
  struct {
    UINT32 rc__idle_message_disable : 1; /**< RC* IDLE Message Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 message_bypass : 1; /**< Message Bypass */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 idle_flush_disable : 1; /**< IDLE Flush Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 idle_indicator : 1; /**< IDLE Indicator */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 go_indicator : 1; /**< GO Indicator */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 disable_idle_dop_flush_pipelining : 1; /**< Disable IDLE DOP flush pipelining */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 enable_idle_rc6_flush : 1; /**< Enable IDLE RC6 flush */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 sync_flip_v_blank_wait_for_event_power_down_disable : 1; /**< Sync Flip/V-blank Wait For Event Power Down Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[08:08] */
                            /* Bits[8:8], default=0*/
    UINT32 debug_flow_for_go_0_acknowledge : 1; /**< Debug Flow for GO=0 Acknowledge */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[10:10] */
                            /* Bits[10:10], default=0*/
    UINT32 psmi_parser_block : 1; /**< PSMI Parser Block */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 semaphore_wait_event_idle_message_disable : 1; /**< Semaphore Wait Event IDLE message Disable */
                            /* Bits[12:12], default=0x00000001*/
    UINT32 idle_message_override_control : 2; /**< IDLE Message Override Control */
                            /* Bits[14:13], default=0x00000000*/
    UINT32 inhibit_rc6_entry_on_wait_for_scanline : 1; /**< Inhibit RC6 Entry on Wait For Scanline */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 mask : 16; /**< Mask */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RC_PSMI_CTRL_BCSUNIT5_GT_STRUCT;

#define RC_PSMI_CTRL_BCSUNIT5_GT_WIDTH 32
#define RC_PSMI_CTRL_BCSUNIT5_GT_DEFAULT 0x00001000U
#define RC_PSMI_CTRL_BCSUNIT5_GT_ENDPOINT GT

#define RC_PSMI_CTRL_BCSUNIT5_GT_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_BCSUNIT5_GT_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT5_GT_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_BCSUNIT5_GT_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT5_GT_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_BCSUNIT5_GT_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT5_GT_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_BCSUNIT5_GT_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT5_GT_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_BCSUNIT5_GT_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT5_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_BCSUNIT5_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT5_GT_ENABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_BCSUNIT5_GT_ENABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT5_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_BCSUNIT5_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT5_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_BCSUNIT5_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT5_GT_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_BCSUNIT5_GT_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT5_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_BCSUNIT5_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT5_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_BCSUNIT5_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_BCSUNIT5_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_BCSUNIT5_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT5_GT_MASK_LSB 0x0010
#define RC_PSMI_CTRL_BCSUNIT5_GT_MASK_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** PWRCTX_MAXCNT_BCSUNIT5 desc:
  */

#define PWRCTX_MAXCNT_BCSUNIT5_GT_REG    (0x003e8054U)

typedef union {
  struct {
    UINT32 idle_wait_time : 20; /**< IDLE Wait Time */
                            /* Bits[19:0], default=0x0000000A*/
    UINT32 rsvd_0 : 12; /**< Undefined - auto filled rsvd_[31:20] */
                            /* Bits[31:20], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PWRCTX_MAXCNT_BCSUNIT5_GT_STRUCT;

#define PWRCTX_MAXCNT_BCSUNIT5_GT_WIDTH 32
#define PWRCTX_MAXCNT_BCSUNIT5_GT_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_BCSUNIT5_GT_ENDPOINT GT

#define PWRCTX_MAXCNT_BCSUNIT5_GT_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_BCSUNIT5_GT_IDLE_WAIT_TIME_SIZE 0x0014

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** RC_PSMI_CTRL_BCSUNIT6 desc:
  */

#define RC_PSMI_CTRL_BCSUNIT6_GT_REG     (0x003ea050U)

typedef union {
  struct {
    UINT32 rc__idle_message_disable : 1; /**< RC* IDLE Message Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 message_bypass : 1; /**< Message Bypass */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 idle_flush_disable : 1; /**< IDLE Flush Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 idle_indicator : 1; /**< IDLE Indicator */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 go_indicator : 1; /**< GO Indicator */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 disable_idle_dop_flush_pipelining : 1; /**< Disable IDLE DOP flush pipelining */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 enable_idle_rc6_flush : 1; /**< Enable IDLE RC6 flush */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 sync_flip_v_blank_wait_for_event_power_down_disable : 1; /**< Sync Flip/V-blank Wait For Event Power Down Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[08:08] */
                            /* Bits[8:8], default=0*/
    UINT32 debug_flow_for_go_0_acknowledge : 1; /**< Debug Flow for GO=0 Acknowledge */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[10:10] */
                            /* Bits[10:10], default=0*/
    UINT32 psmi_parser_block : 1; /**< PSMI Parser Block */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 semaphore_wait_event_idle_message_disable : 1; /**< Semaphore Wait Event IDLE message Disable */
                            /* Bits[12:12], default=0x00000001*/
    UINT32 idle_message_override_control : 2; /**< IDLE Message Override Control */
                            /* Bits[14:13], default=0x00000000*/
    UINT32 inhibit_rc6_entry_on_wait_for_scanline : 1; /**< Inhibit RC6 Entry on Wait For Scanline */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 mask : 16; /**< Mask */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RC_PSMI_CTRL_BCSUNIT6_GT_STRUCT;

#define RC_PSMI_CTRL_BCSUNIT6_GT_WIDTH 32
#define RC_PSMI_CTRL_BCSUNIT6_GT_DEFAULT 0x00001000U
#define RC_PSMI_CTRL_BCSUNIT6_GT_ENDPOINT GT

#define RC_PSMI_CTRL_BCSUNIT6_GT_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_BCSUNIT6_GT_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT6_GT_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_BCSUNIT6_GT_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT6_GT_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_BCSUNIT6_GT_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT6_GT_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_BCSUNIT6_GT_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT6_GT_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_BCSUNIT6_GT_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT6_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_BCSUNIT6_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT6_GT_ENABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_BCSUNIT6_GT_ENABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT6_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_BCSUNIT6_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT6_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_BCSUNIT6_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT6_GT_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_BCSUNIT6_GT_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT6_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_BCSUNIT6_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT6_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_BCSUNIT6_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_BCSUNIT6_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_BCSUNIT6_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT6_GT_MASK_LSB 0x0010
#define RC_PSMI_CTRL_BCSUNIT6_GT_MASK_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** PWRCTX_MAXCNT_BCSUNIT6 desc:
  */

#define PWRCTX_MAXCNT_BCSUNIT6_GT_REG    (0x003ea054U)

typedef union {
  struct {
    UINT32 idle_wait_time : 20; /**< IDLE Wait Time */
                            /* Bits[19:0], default=0x0000000A*/
    UINT32 rsvd_0 : 12; /**< Undefined - auto filled rsvd_[31:20] */
                            /* Bits[31:20], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PWRCTX_MAXCNT_BCSUNIT6_GT_STRUCT;

#define PWRCTX_MAXCNT_BCSUNIT6_GT_WIDTH 32
#define PWRCTX_MAXCNT_BCSUNIT6_GT_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_BCSUNIT6_GT_ENDPOINT GT

#define PWRCTX_MAXCNT_BCSUNIT6_GT_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_BCSUNIT6_GT_IDLE_WAIT_TIME_SIZE 0x0014

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** RC_PSMI_CTRL_BCSUNIT7 desc:
  */

#define RC_PSMI_CTRL_BCSUNIT7_GT_REG     (0x003ec050U)

typedef union {
  struct {
    UINT32 rc__idle_message_disable : 1; /**< RC* IDLE Message Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 message_bypass : 1; /**< Message Bypass */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 idle_flush_disable : 1; /**< IDLE Flush Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 idle_indicator : 1; /**< IDLE Indicator */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 go_indicator : 1; /**< GO Indicator */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 disable_idle_dop_flush_pipelining : 1; /**< Disable IDLE DOP flush pipelining */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 enable_idle_rc6_flush : 1; /**< Enable IDLE RC6 flush */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 sync_flip_v_blank_wait_for_event_power_down_disable : 1; /**< Sync Flip/V-blank Wait For Event Power Down Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[08:08] */
                            /* Bits[8:8], default=0*/
    UINT32 debug_flow_for_go_0_acknowledge : 1; /**< Debug Flow for GO=0 Acknowledge */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[10:10] */
                            /* Bits[10:10], default=0*/
    UINT32 psmi_parser_block : 1; /**< PSMI Parser Block */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 semaphore_wait_event_idle_message_disable : 1; /**< Semaphore Wait Event IDLE message Disable */
                            /* Bits[12:12], default=0x00000001*/
    UINT32 idle_message_override_control : 2; /**< IDLE Message Override Control */
                            /* Bits[14:13], default=0x00000000*/
    UINT32 inhibit_rc6_entry_on_wait_for_scanline : 1; /**< Inhibit RC6 Entry on Wait For Scanline */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 mask : 16; /**< Mask */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RC_PSMI_CTRL_BCSUNIT7_GT_STRUCT;

#define RC_PSMI_CTRL_BCSUNIT7_GT_WIDTH 32
#define RC_PSMI_CTRL_BCSUNIT7_GT_DEFAULT 0x00001000U
#define RC_PSMI_CTRL_BCSUNIT7_GT_ENDPOINT GT

#define RC_PSMI_CTRL_BCSUNIT7_GT_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_BCSUNIT7_GT_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT7_GT_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_BCSUNIT7_GT_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT7_GT_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_BCSUNIT7_GT_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT7_GT_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_BCSUNIT7_GT_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT7_GT_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_BCSUNIT7_GT_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT7_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_BCSUNIT7_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT7_GT_ENABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_BCSUNIT7_GT_ENABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT7_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_BCSUNIT7_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT7_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_BCSUNIT7_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT7_GT_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_BCSUNIT7_GT_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT7_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_BCSUNIT7_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT7_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_BCSUNIT7_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_BCSUNIT7_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_BCSUNIT7_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT7_GT_MASK_LSB 0x0010
#define RC_PSMI_CTRL_BCSUNIT7_GT_MASK_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** PWRCTX_MAXCNT_BCSUNIT7 desc:
  */

#define PWRCTX_MAXCNT_BCSUNIT7_GT_REG    (0x003ec054U)

typedef union {
  struct {
    UINT32 idle_wait_time : 20; /**< IDLE Wait Time */
                            /* Bits[19:0], default=0x0000000A*/
    UINT32 rsvd_0 : 12; /**< Undefined - auto filled rsvd_[31:20] */
                            /* Bits[31:20], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PWRCTX_MAXCNT_BCSUNIT7_GT_STRUCT;

#define PWRCTX_MAXCNT_BCSUNIT7_GT_WIDTH 32
#define PWRCTX_MAXCNT_BCSUNIT7_GT_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_BCSUNIT7_GT_ENDPOINT GT

#define PWRCTX_MAXCNT_BCSUNIT7_GT_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_BCSUNIT7_GT_IDLE_WAIT_TIME_SIZE 0x0014

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** RC_PSMI_CTRL_BCSUNIT8 desc:
  */

#define RC_PSMI_CTRL_BCSUNIT8_GT_REG     (0x003ee050U)

typedef union {
  struct {
    UINT32 rc__idle_message_disable : 1; /**< RC* IDLE Message Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 message_bypass : 1; /**< Message Bypass */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 idle_flush_disable : 1; /**< IDLE Flush Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 idle_indicator : 1; /**< IDLE Indicator */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 go_indicator : 1; /**< GO Indicator */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 disable_idle_dop_flush_pipelining : 1; /**< Disable IDLE DOP flush pipelining */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 enable_idle_rc6_flush : 1; /**< Enable IDLE RC6 flush */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 sync_flip_v_blank_wait_for_event_power_down_disable : 1; /**< Sync Flip/V-blank Wait For Event Power Down Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[08:08] */
                            /* Bits[8:8], default=0*/
    UINT32 debug_flow_for_go_0_acknowledge : 1; /**< Debug Flow for GO=0 Acknowledge */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[10:10] */
                            /* Bits[10:10], default=0*/
    UINT32 psmi_parser_block : 1; /**< PSMI Parser Block */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 semaphore_wait_event_idle_message_disable : 1; /**< Semaphore Wait Event IDLE message Disable */
                            /* Bits[12:12], default=0x00000001*/
    UINT32 idle_message_override_control : 2; /**< IDLE Message Override Control */
                            /* Bits[14:13], default=0x00000000*/
    UINT32 inhibit_rc6_entry_on_wait_for_scanline : 1; /**< Inhibit RC6 Entry on Wait For Scanline */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 mask : 16; /**< Mask */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RC_PSMI_CTRL_BCSUNIT8_GT_STRUCT;

#define RC_PSMI_CTRL_BCSUNIT8_GT_WIDTH 32
#define RC_PSMI_CTRL_BCSUNIT8_GT_DEFAULT 0x00001000U
#define RC_PSMI_CTRL_BCSUNIT8_GT_ENDPOINT GT

#define RC_PSMI_CTRL_BCSUNIT8_GT_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_BCSUNIT8_GT_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT8_GT_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_BCSUNIT8_GT_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT8_GT_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_BCSUNIT8_GT_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT8_GT_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_BCSUNIT8_GT_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT8_GT_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_BCSUNIT8_GT_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT8_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_BCSUNIT8_GT_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT8_GT_ENABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_BCSUNIT8_GT_ENABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT8_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_BCSUNIT8_GT_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT8_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_BCSUNIT8_GT_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT8_GT_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_BCSUNIT8_GT_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT8_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_BCSUNIT8_GT_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT8_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_BCSUNIT8_GT_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_BCSUNIT8_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_BCSUNIT8_GT_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_BCSUNIT8_GT_MASK_LSB 0x0010
#define RC_PSMI_CTRL_BCSUNIT8_GT_MASK_SIZE 0x0010

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** PWRCTX_MAXCNT_BCSUNIT8 desc:
  */

#define PWRCTX_MAXCNT_BCSUNIT8_GT_REG    (0x003ee054U)

typedef union {
  struct {
    UINT32 idle_wait_time : 20; /**< IDLE Wait Time */
                            /* Bits[19:0], default=0x0000000A*/
    UINT32 rsvd_0 : 12; /**< Undefined - auto filled rsvd_[31:20] */
                            /* Bits[31:20], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PWRCTX_MAXCNT_BCSUNIT8_GT_STRUCT;

#define PWRCTX_MAXCNT_BCSUNIT8_GT_WIDTH 32
#define PWRCTX_MAXCNT_BCSUNIT8_GT_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_BCSUNIT8_GT_ENDPOINT GT

#define PWRCTX_MAXCNT_BCSUNIT8_GT_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_BCSUNIT8_GT_IDLE_WAIT_TIME_SIZE 0x0014

#endif      // SV_HOOKS

//************************************************ /RegisterStructs


#endif      // _GT_h
