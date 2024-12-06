/** @file
  Display_Mem.h

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

#ifndef _DISPLAY_MEM_h
#define _DISPLAY_MEM_h



// IP GUID: b4af374f-4eec-4c75-a449-a2f0467773f5
#define DISPLAY_MEM_ID 0xb4af374f4eec4c75

//************************************************ RegisterStructs

#ifdef SV_HOOKS

/** SDE_CHKN_BIOS_SECURED desc:
  */

#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_REG (0x000c2004U)

typedef union {
  struct {
    UINT32 spare_0 : 1; /**< SPARE 0 */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 spare_1 : 1; /**< SPARE 1 */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 spare_2 : 1; /**< SPARE 2 */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 spare_3 : 1; /**< SPARE 3 */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 spare_4 : 1; /**< SPARE 4 */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 spare_5 : 1; /**< SPARE 5 */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 spare_6 : 1; /**< SPARE 6 */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 spare_7 : 1; /**< SPARE 7 */
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
} SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_STRUCT;

#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_WIDTH 32
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_DEFAULT 0x00000000U
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_ENDPOINT DISPLAY_MEM

#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_0_LSB 0x0000
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_0_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_1_LSB 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_1_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_2_LSB 0x0002
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_2_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_3_LSB 0x0003
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_3_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_4_LSB 0x0004
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_4_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_5_LSB 0x0005
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_5_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_6_LSB 0x0006
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_6_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_7_LSB 0x0007
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_7_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_8_LSB 0x0008
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_8_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_9_LSB 0x0009
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_9_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_10_LSB 0x000a
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_10_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_11_LSB 0x000b
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_11_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_12_LSB 0x000c
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_12_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_13_LSB 0x000d
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_13_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_14_LSB 0x000e
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_14_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_15_LSB 0x000f
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_15_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_16_LSB 0x0010
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_16_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_17_LSB 0x0011
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_17_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_18_LSB 0x0012
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_18_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_19_LSB 0x0013
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_19_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_20_LSB 0x0014
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_20_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_21_LSB 0x0015
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_21_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_22_LSB 0x0016
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_22_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_23_LSB 0x0017
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_23_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_24_LSB 0x0018
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_24_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_25_LSB 0x0019
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_25_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_26_LSB 0x001a
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_26_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_27_LSB 0x001b
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_27_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_28_LSB 0x001c
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_28_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_29_LSB 0x001d
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_29_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_30_LSB 0x001e
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_30_SIZE 0x0001
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_31_LSB 0x001f
#define SDE_CHKN_BIOS_SECURED_DISPLAY_MEM_SPARE_31_SIZE 0x0001

#endif      // SV_HOOKS

#ifdef SV_HOOKS

/** PCH_RESET_PREP_ACK_TIMEOUT desc:
  */

#define PCH_RESET_PREP_ACK_TIMEOUT_DISPLAY_MEM_REG (0x000c5164U)

typedef union {
  struct {
    UINT32 reset_prep_ack_timeout : 32; /**< RESET PREP ACK TIMEOUT */
                            /* Bits[31:0], default=0x00AFC800*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PCH_RESET_PREP_ACK_TIMEOUT_DISPLAY_MEM_STRUCT;

#define PCH_RESET_PREP_ACK_TIMEOUT_DISPLAY_MEM_WIDTH 32
#define PCH_RESET_PREP_ACK_TIMEOUT_DISPLAY_MEM_DEFAULT 0x00afc800U
#define PCH_RESET_PREP_ACK_TIMEOUT_DISPLAY_MEM_ENDPOINT DISPLAY_MEM

#define PCH_RESET_PREP_ACK_TIMEOUT_DISPLAY_MEM_RESET_PREP_ACK_TIMEOUT_LSB 0x0000
#define PCH_RESET_PREP_ACK_TIMEOUT_DISPLAY_MEM_RESET_PREP_ACK_TIMEOUT_SIZE 0x0020

#endif      // SV_HOOKS

/** PP_CONTROL desc:
  */

#define PP_CONTROL_DISPLAY_MEM_REG       (0x000c7204U)

typedef union {
  struct {
    UINT32 power_state_target : 1; /**< POWER STATE TARGET */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 power_down_on_reset : 1; /**< POWER DOWN ON RESET */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 backlight_enable : 1; /**< BACKLIGHT ENABLE */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 vdd_override : 1; /**< VDD OVERRIDE */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 power_cycle_delay : 5; /**< POWER CYCLE DELAY */
                            /* Bits[8:4], default=0x00000000*/
    UINT32 rsvd_0 : 7; /**< Undefined - auto filled rsvd_[15:09] */
                            /* Bits[15:9], default=0*/
    UINT32 spare_31_16 : 16; /**< SPARE 31 16 */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PP_CONTROL_DISPLAY_MEM_STRUCT;

#define PP_CONTROL_DISPLAY_MEM_WIDTH 32
#define PP_CONTROL_DISPLAY_MEM_DEFAULT 0x00000000U
#define PP_CONTROL_DISPLAY_MEM_ENDPOINT DISPLAY_MEM

#define PP_CONTROL_DISPLAY_MEM_POWER_STATE_TARGET_LSB 0x0000
#define PP_CONTROL_DISPLAY_MEM_POWER_STATE_TARGET_SIZE 0x0001
#define PP_CONTROL_DISPLAY_MEM_POWER_DOWN_ON_RESET_LSB 0x0001
#define PP_CONTROL_DISPLAY_MEM_POWER_DOWN_ON_RESET_SIZE 0x0001
#define PP_CONTROL_DISPLAY_MEM_BACKLIGHT_ENABLE_LSB 0x0002
#define PP_CONTROL_DISPLAY_MEM_BACKLIGHT_ENABLE_SIZE 0x0001
#define PP_CONTROL_DISPLAY_MEM_VDD_OVERRIDE_LSB 0x0003
#define PP_CONTROL_DISPLAY_MEM_VDD_OVERRIDE_SIZE 0x0001
#define PP_CONTROL_DISPLAY_MEM_POWER_CYCLE_DELAY_LSB 0x0004
#define PP_CONTROL_DISPLAY_MEM_POWER_CYCLE_DELAY_SIZE 0x0005
#define PP_CONTROL_DISPLAY_MEM_SPARE_31_16_LSB 0x0010
#define PP_CONTROL_DISPLAY_MEM_SPARE_31_16_SIZE 0x0010


/** PP_CONTROL_2 desc:
  */

#define PP_CONTROL_2_DISPLAY_MEM_REG     (0x000c7304U)

typedef union {
  struct {
    UINT32 power_state_target : 1; /**< POWER STATE TARGET */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 power_down_on_reset : 1; /**< POWER DOWN ON RESET */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 backlight_enable : 1; /**< BACKLIGHT ENABLE */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 vdd_override : 1; /**< VDD OVERRIDE */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 power_cycle_delay : 5; /**< POWER CYCLE DELAY */
                            /* Bits[8:4], default=0x00000000*/
    UINT32 rsvd_0 : 7; /**< Undefined - auto filled rsvd_[15:09] */
                            /* Bits[15:9], default=0*/
    UINT32 spare_31_16 : 16; /**< SPARE 31 16 */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PP_CONTROL_2_DISPLAY_MEM_STRUCT;

#define PP_CONTROL_2_DISPLAY_MEM_WIDTH 32
#define PP_CONTROL_2_DISPLAY_MEM_DEFAULT 0x00000000U
#define PP_CONTROL_2_DISPLAY_MEM_ENDPOINT DISPLAY_MEM

#define PP_CONTROL_2_DISPLAY_MEM_POWER_STATE_TARGET_LSB 0x0000
#define PP_CONTROL_2_DISPLAY_MEM_POWER_STATE_TARGET_SIZE 0x0001
#define PP_CONTROL_2_DISPLAY_MEM_POWER_DOWN_ON_RESET_LSB 0x0001
#define PP_CONTROL_2_DISPLAY_MEM_POWER_DOWN_ON_RESET_SIZE 0x0001
#define PP_CONTROL_2_DISPLAY_MEM_BACKLIGHT_ENABLE_LSB 0x0002
#define PP_CONTROL_2_DISPLAY_MEM_BACKLIGHT_ENABLE_SIZE 0x0001
#define PP_CONTROL_2_DISPLAY_MEM_VDD_OVERRIDE_LSB 0x0003
#define PP_CONTROL_2_DISPLAY_MEM_VDD_OVERRIDE_SIZE 0x0001
#define PP_CONTROL_2_DISPLAY_MEM_POWER_CYCLE_DELAY_LSB 0x0004
#define PP_CONTROL_2_DISPLAY_MEM_POWER_CYCLE_DELAY_SIZE 0x0005
#define PP_CONTROL_2_DISPLAY_MEM_SPARE_31_16_LSB 0x0010
#define PP_CONTROL_2_DISPLAY_MEM_SPARE_31_16_SIZE 0x0010

//************************************************ /RegisterStructs


#endif      // _DISPLAY_MEM_h
