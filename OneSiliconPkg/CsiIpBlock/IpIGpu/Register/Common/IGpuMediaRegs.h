/** @file
  MEDIA.h

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

#ifndef _MEDIA_h
#define _MEDIA_h



// IP GUID: 8f260443-199f-4090-9003-ff9e98b8d9c0
#define MEDIA_ID 0x8f260443199f4090

//************************************************ RegisterStructs


/** RC_PSMI_CTRL_GSCCS desc:
  */

#define RC_PSMI_CTRL_GSCCS_MEDIA_REG     (0x0011a050U)

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
    UINT32 disable_idle_rc6_flush : 1; /**< Disable IDLE RC6 flush */
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
                            /* Bits[12:12], default=0x00000000*/
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
} RC_PSMI_CTRL_GSCCS_MEDIA_STRUCT;

#define RC_PSMI_CTRL_GSCCS_MEDIA_WIDTH 32
#define RC_PSMI_CTRL_GSCCS_MEDIA_DEFAULT 0x00000000U
#define RC_PSMI_CTRL_GSCCS_MEDIA_ENDPOINT MEDIA

#define RC_PSMI_CTRL_GSCCS_MEDIA_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_GSCCS_MEDIA_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_GSCCS_MEDIA_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_GSCCS_MEDIA_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_GSCCS_MEDIA_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_GSCCS_MEDIA_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_GSCCS_MEDIA_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_GSCCS_MEDIA_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_GSCCS_MEDIA_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_GSCCS_MEDIA_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_GSCCS_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_GSCCS_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_GSCCS_MEDIA_DISABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_GSCCS_MEDIA_DISABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_GSCCS_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_GSCCS_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_GSCCS_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_GSCCS_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_GSCCS_MEDIA_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_GSCCS_MEDIA_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_GSCCS_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_GSCCS_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_GSCCS_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_GSCCS_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_GSCCS_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_GSCCS_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_GSCCS_MEDIA_MASK_LSB 0x0010
#define RC_PSMI_CTRL_GSCCS_MEDIA_MASK_SIZE 0x0010


/** PWRCTX_MAXCNT_GSCCS desc:
  */

#define PWRCTX_MAXCNT_GSCCS_MEDIA_REG    (0x0011a054U)

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
} PWRCTX_MAXCNT_GSCCS_MEDIA_STRUCT;

#define PWRCTX_MAXCNT_GSCCS_MEDIA_WIDTH 32
#define PWRCTX_MAXCNT_GSCCS_MEDIA_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_GSCCS_MEDIA_ENDPOINT MEDIA

#define PWRCTX_MAXCNT_GSCCS_MEDIA_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_GSCCS_MEDIA_IDLE_WAIT_TIME_SIZE 0x0014


/** RC_PSMI_CTRL_VCSUNIT0 desc:
  */

#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_REG  (0x001c0050U)

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
    UINT32 disable_idle_rc6_flush : 1; /**< Disable IDLE RC6 flush */
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
                            /* Bits[12:12], default=0x00000000*/
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
} RC_PSMI_CTRL_VCSUNIT0_MEDIA_STRUCT;

#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_WIDTH 32
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_DEFAULT 0x00000000U
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_ENDPOINT MEDIA

#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_DISABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_DISABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_MASK_LSB 0x0010
#define RC_PSMI_CTRL_VCSUNIT0_MEDIA_MASK_SIZE 0x0010


/** PWRCTX_MAXCNT_VCSUNIT0 desc:
  */

#define PWRCTX_MAXCNT_VCSUNIT0_MEDIA_REG (0x001c0054U)

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
} PWRCTX_MAXCNT_VCSUNIT0_MEDIA_STRUCT;

#define PWRCTX_MAXCNT_VCSUNIT0_MEDIA_WIDTH 32
#define PWRCTX_MAXCNT_VCSUNIT0_MEDIA_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_VCSUNIT0_MEDIA_ENDPOINT MEDIA

#define PWRCTX_MAXCNT_VCSUNIT0_MEDIA_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_VCSUNIT0_MEDIA_IDLE_WAIT_TIME_SIZE 0x0014


/** VDCGCTL3F0C_VDBOX0 desc:
  */

#define VDCGCTL3F0C_VDBOX0_MEDIA_REG     (0x001c3f0cU)

typedef union {
  struct {
    UINT32 cg3ddisvcp : 1; /**< VCPunit Clock Gating Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cg3ddis_awf : 1; /**< AWF Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 cg3ddisvcs_cs : 1; /**< Csunit's Clock Gating Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 cg3ddisvdl1 : 1; /**< vdl1unit Clock Gating Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 cg3ddisvds : 1; /**< VDSunit Clock Gating Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 cg3ddisvdx : 1; /**< VDXunit Clock Gating Disable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 cg3ddisvft : 1; /**< VFTunit Clock Gating Disable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 cg3ddisvhme : 1; /**< VHMEunit Clock Gating Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 cg3ddisvhr : 1; /**< VHRunit's Clock Gating Disable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 cg3ddisvime : 1; /**< VIMEunit Clock Gating Disable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 cg3ddisvin_vid1 : 1; /**< VID1 Clock Gating Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 cg3ddisvin_vid2 : 1; /**< VID2 Clock Gating Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 cg3ddisvin_vid3 : 1; /**< VID3 Clock Gating Disable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 cg3ddisvin_vid4 : 1; /**< VID4 Clock Gating Disable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 cg3ddisvin_vid5 : 1; /**< VID5 Clock Gating Disable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 cg3ddisvin_vid6 : 1; /**< VID6 Clock Gating Disable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 cg3ddisvip : 1; /**< VIPunit Clock Gating Disable */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 cg3ddisvis : 1; /**< VISunit Clock Gating Disable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 cg3ddisvit : 1; /**< VITunit Clock Gating Disable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 cg3ddisvlf : 1; /**< VLFunit Clock Gating Disable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 cg3ddisvmb : 1; /**< VMBunit Clock Gating Disable */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 cg3ddisvmc : 1; /**< VMCunit Clock Gating Disable */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 cg3ddisvmcr : 1; /**< VMCRunit Clock Gating Disable */
                            /* Bits[22:22], default=0x00000001*/
    UINT32 cg3ddisvmd : 1; /**< VMDunit Clock Gating Disable */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 cg3ddisvmpc : 1; /**< VMPCunit Clock Gating Disable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 cg3ddisvmts : 1; /**< VMTSunit Clock Gating Disable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 cg3ddisvmx : 1; /**< VMXunit Clock Gating Disable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 cg3ddisvnc : 1; /**< VNCunit Clock Gating Disable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 cg3ddis_spare9 : 1; /**< SPARE Clock Gating Disable9 */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 cg3ddis_spare10 : 1; /**< SPARE Clock Gating Disable10 */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 cg3ddis_spare11 : 1; /**< SPARE Clock Gating Disable11 */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 cg3ddis_spare12 : 1; /**< SPARE Clock Gating Disable12 */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} VDCGCTL3F0C_VDBOX0_MEDIA_STRUCT;

#define VDCGCTL3F0C_VDBOX0_MEDIA_WIDTH 32
#define VDCGCTL3F0C_VDBOX0_MEDIA_DEFAULT 0x00400000U
#define VDCGCTL3F0C_VDBOX0_MEDIA_ENDPOINT MEDIA

#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVCP_LSB 0x0000
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVCP_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDIS_AWF_LSB 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDIS_AWF_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVCS_CS_LSB 0x0002
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVCS_CS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVDL1_LSB 0x0003
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVDL1_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVDS_LSB 0x0004
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVDS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVDX_LSB 0x0005
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVDX_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVFT_LSB 0x0006
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVFT_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVHME_LSB 0x0007
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVHME_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVHR_LSB 0x0008
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVHR_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVIME_LSB 0x0009
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVIME_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVIN_VID1_LSB 0x000a
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVIN_VID1_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVIN_VID2_LSB 0x000b
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVIN_VID2_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVIN_VID3_LSB 0x000c
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVIN_VID3_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVIN_VID4_LSB 0x000d
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVIN_VID4_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVIN_VID5_LSB 0x000e
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVIN_VID5_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVIN_VID6_LSB 0x000f
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVIN_VID6_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVIP_LSB 0x0010
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVIP_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVIS_LSB 0x0011
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVIS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVIT_LSB 0x0012
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVIT_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVLF_LSB 0x0013
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVLF_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVMB_LSB 0x0014
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVMB_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVMC_LSB 0x0015
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVMC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVMCR_LSB 0x0016
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVMCR_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVMD_LSB 0x0017
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVMD_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVMPC_LSB 0x0018
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVMPC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVMTS_LSB 0x0019
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVMTS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVMX_LSB 0x001a
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVMX_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVNC_LSB 0x001b
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDISVNC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDIS_SPARE9_LSB 0x001c
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDIS_SPARE9_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDIS_SPARE10_LSB 0x001d
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDIS_SPARE10_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDIS_SPARE11_LSB 0x001e
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDIS_SPARE11_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDIS_SPARE12_LSB 0x001f
#define VDCGCTL3F0C_VDBOX0_MEDIA_CG3DDIS_SPARE12_SIZE 0x0001


/** VDCGCTL3F1C_VDBOX0 desc:
  */

#define VDCGCTL3F1C_VDBOX0_MEDIA_REG     (0x001c3f1cU)

typedef union {
  struct {
    UINT32 cg3ddis_afg : 1; /**< AFG unit Clock Gating Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cg3ddis_av1pipe : 1; /**< AV1 pipeline Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 cg3ddis_hcppipe : 1; /**< HCP pipeline Clock Gating Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 cg3ddis_mfxpipe : 1; /**< MFX pipeline Clock Gating Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 cg3ddis_vdencpipe : 1; /**< VDENC pipeline Clock Gating Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 cg3ddis_hucpipe : 1; /**< HUC pipeline Clock Gating Disable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 cg3ddis_vdaqmpipe : 1; /**< VDAQM pipeline Clock Gating Disable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 cg3ddis_vrtpipe : 1; /**< VRT clients pipeline Clock Gating Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 cg3ddis_vepipe : 1; /**< VEBOX pipeline Clock Gating Disable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 cg3ddis_sfcpipe : 1; /**< SFC pipeline Clock Gating Disable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 cg3ddisaqs : 1; /**< AQSunit Clock Gating Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 cg3ddisaqm : 1; /**< AQMunit Clock Gating Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 cg3ddisaqx : 1; /**< AQXunit Clock Gating Disable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 cg3ddisaqa : 1; /**< AQAunit Clock Gating Disable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 cg3ddisoaaddrmscmi : 1; /**< OAADDRMunit in SCMI Clock Gating Disable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 cg3ddisoamscmi : 1; /**< oamscmiUnit Clock Gating Disable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 cg3ddisgamctrl : 1; /**< gamctrl Clock Gating Disable */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 cg3ddisgamstlb : 1; /**< gamstlb Clock Gating Disable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 cg3ddisgamcmdiscmi : 1; /**< gamcmdiscmi Clock Gating Disable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 cg3ddisgamdatiscmi : 1; /**< gamdatiscmi Clock Gating Disable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 cg3ddisgamreqstrmscmi : 1; /**< gamreqstrmscmi Clock Gating Disable */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 cg3ddisgamftlbscmi : 1; /**< gamftlbscmi Clock Gating Disable */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 cg3ddisgamfxbarscmi : 1; /**< gamfxbarscmi Clock Gating Disable */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 cg3ddisaxf : 1; /**< AFX Clock Gating Disable */
                            /* Bits[23:23], default=0x00000001*/
    UINT32 cg3ddismrruscmi : 1; /**< MRRUSCMI Clock Gating Disable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 cg3ddispsmi : 1; /**< SCMI PSMI Clock Gating Disable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 cg3ddisaqh : 1; /**< AQH unit Clock Gating Disable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 cg3ddisrdm : 1; /**< RDM unit Clock Gating Disable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 cg3ddis_mgmc : 1; /**< MGMC unit Clock Gating Disable */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 rsvd_0 : 3; /**< Undefined - auto filled rsvd_[31:29] */
                            /* Bits[31:29], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} VDCGCTL3F1C_VDBOX0_MEDIA_STRUCT;

#define VDCGCTL3F1C_VDBOX0_MEDIA_WIDTH 32
#define VDCGCTL3F1C_VDBOX0_MEDIA_DEFAULT 0x00800000U
#define VDCGCTL3F1C_VDBOX0_MEDIA_ENDPOINT MEDIA

#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDIS_AFG_LSB 0x0000
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDIS_AFG_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDIS_AV1PIPE_LSB 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDIS_AV1PIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDIS_HCPPIPE_LSB 0x0002
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDIS_HCPPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDIS_MFXPIPE_LSB 0x0003
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDIS_MFXPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDIS_VDENCPIPE_LSB 0x0004
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDIS_VDENCPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDIS_HUCPIPE_LSB 0x0005
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDIS_HUCPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDIS_VDAQMPIPE_LSB 0x0006
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDIS_VDAQMPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDIS_VRTPIPE_LSB 0x0007
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDIS_VRTPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDIS_VEPIPE_LSB 0x0008
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDIS_VEPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDIS_SFCPIPE_LSB 0x0009
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDIS_SFCPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISAQS_LSB 0x000a
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISAQS_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISAQM_LSB 0x000b
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISAQM_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISAQX_LSB 0x000c
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISAQX_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISAQA_LSB 0x000d
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISAQA_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISOAADDRMSCMI_LSB 0x000e
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISOAADDRMSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISOAMSCMI_LSB 0x000f
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISOAMSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISGAMCTRL_LSB 0x0010
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISGAMCTRL_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISGAMSTLB_LSB 0x0011
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISGAMSTLB_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISGAMCMDISCMI_LSB 0x0012
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISGAMCMDISCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISGAMDATISCMI_LSB 0x0013
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISGAMDATISCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISGAMREQSTRMSCMI_LSB 0x0014
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISGAMREQSTRMSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISGAMFTLBSCMI_LSB 0x0015
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISGAMFTLBSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISGAMFXBARSCMI_LSB 0x0016
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISGAMFXBARSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISAXF_LSB 0x0017
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISAXF_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISMRRUSCMI_LSB 0x0018
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISMRRUSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISPSMI_LSB 0x0019
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISPSMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISAQH_LSB 0x001a
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISAQH_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISRDM_LSB 0x001b
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDISRDM_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDIS_MGMC_LSB 0x001c
#define VDCGCTL3F1C_VDBOX0_MEDIA_CG3DDIS_MGMC_SIZE 0x0001


/** RC_PSMI_CTRL_VCSUNIT1 desc:
  */

#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_REG  (0x001c4050U)

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
    UINT32 disable_idle_rc6_flush : 1; /**< Disable IDLE RC6 flush */
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
                            /* Bits[12:12], default=0x00000000*/
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
} RC_PSMI_CTRL_VCSUNIT1_MEDIA_STRUCT;

#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_WIDTH 32
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_DEFAULT 0x00000000U
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_ENDPOINT MEDIA

#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_DISABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_DISABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_MASK_LSB 0x0010
#define RC_PSMI_CTRL_VCSUNIT1_MEDIA_MASK_SIZE 0x0010


/** PWRCTX_MAXCNT_VCSUNIT1 desc:
  */

#define PWRCTX_MAXCNT_VCSUNIT1_MEDIA_REG (0x001c4054U)

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
} PWRCTX_MAXCNT_VCSUNIT1_MEDIA_STRUCT;

#define PWRCTX_MAXCNT_VCSUNIT1_MEDIA_WIDTH 32
#define PWRCTX_MAXCNT_VCSUNIT1_MEDIA_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_VCSUNIT1_MEDIA_ENDPOINT MEDIA

#define PWRCTX_MAXCNT_VCSUNIT1_MEDIA_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_VCSUNIT1_MEDIA_IDLE_WAIT_TIME_SIZE 0x0014


/** VDCGCTL3F0C_VDBOX1 desc:
  */

#define VDCGCTL3F0C_VDBOX1_MEDIA_REG     (0x001c7f0cU)

typedef union {
  struct {
    UINT32 cg3ddisvcp : 1; /**< VCPunit Clock Gating Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cg3ddis_awf : 1; /**< AWF Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 cg3ddisvcs_cs : 1; /**< Csunit's Clock Gating Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 cg3ddisvdl1 : 1; /**< vdl1unit Clock Gating Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 cg3ddisvds : 1; /**< VDSunit Clock Gating Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 cg3ddisvdx : 1; /**< VDXunit Clock Gating Disable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 cg3ddisvft : 1; /**< VFTunit Clock Gating Disable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 cg3ddisvhme : 1; /**< VHMEunit Clock Gating Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 cg3ddisvhr : 1; /**< VHRunit's Clock Gating Disable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 cg3ddisvime : 1; /**< VIMEunit Clock Gating Disable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 cg3ddisvin_vid1 : 1; /**< VID1 Clock Gating Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 cg3ddisvin_vid2 : 1; /**< VID2 Clock Gating Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 cg3ddisvin_vid3 : 1; /**< VID3 Clock Gating Disable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 cg3ddisvin_vid4 : 1; /**< VID4 Clock Gating Disable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 cg3ddisvin_vid5 : 1; /**< VID5 Clock Gating Disable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 cg3ddisvin_vid6 : 1; /**< VID6 Clock Gating Disable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 cg3ddisvip : 1; /**< VIPunit Clock Gating Disable */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 cg3ddisvis : 1; /**< VISunit Clock Gating Disable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 cg3ddisvit : 1; /**< VITunit Clock Gating Disable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 cg3ddisvlf : 1; /**< VLFunit Clock Gating Disable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 cg3ddisvmb : 1; /**< VMBunit Clock Gating Disable */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 cg3ddisvmc : 1; /**< VMCunit Clock Gating Disable */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 cg3ddisvmcr : 1; /**< VMCRunit Clock Gating Disable */
                            /* Bits[22:22], default=0x00000001*/
    UINT32 cg3ddisvmd : 1; /**< VMDunit Clock Gating Disable */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 cg3ddisvmpc : 1; /**< VMPCunit Clock Gating Disable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 cg3ddisvmts : 1; /**< VMTSunit Clock Gating Disable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 cg3ddisvmx : 1; /**< VMXunit Clock Gating Disable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 cg3ddisvnc : 1; /**< VNCunit Clock Gating Disable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 cg3ddis_spare9 : 1; /**< SPARE Clock Gating Disable9 */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 cg3ddis_spare10 : 1; /**< SPARE Clock Gating Disable10 */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 cg3ddis_spare11 : 1; /**< SPARE Clock Gating Disable11 */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 cg3ddis_spare12 : 1; /**< SPARE Clock Gating Disable12 */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} VDCGCTL3F0C_VDBOX1_MEDIA_STRUCT;

#define VDCGCTL3F0C_VDBOX1_MEDIA_WIDTH 32
#define VDCGCTL3F0C_VDBOX1_MEDIA_DEFAULT 0x00400000U
#define VDCGCTL3F0C_VDBOX1_MEDIA_ENDPOINT MEDIA

#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVCP_LSB 0x0000
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVCP_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDIS_AWF_LSB 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDIS_AWF_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVCS_CS_LSB 0x0002
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVCS_CS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVDL1_LSB 0x0003
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVDL1_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVDS_LSB 0x0004
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVDS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVDX_LSB 0x0005
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVDX_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVFT_LSB 0x0006
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVFT_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVHME_LSB 0x0007
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVHME_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVHR_LSB 0x0008
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVHR_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVIME_LSB 0x0009
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVIME_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVIN_VID1_LSB 0x000a
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVIN_VID1_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVIN_VID2_LSB 0x000b
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVIN_VID2_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVIN_VID3_LSB 0x000c
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVIN_VID3_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVIN_VID4_LSB 0x000d
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVIN_VID4_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVIN_VID5_LSB 0x000e
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVIN_VID5_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVIN_VID6_LSB 0x000f
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVIN_VID6_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVIP_LSB 0x0010
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVIP_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVIS_LSB 0x0011
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVIS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVIT_LSB 0x0012
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVIT_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVLF_LSB 0x0013
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVLF_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVMB_LSB 0x0014
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVMB_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVMC_LSB 0x0015
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVMC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVMCR_LSB 0x0016
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVMCR_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVMD_LSB 0x0017
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVMD_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVMPC_LSB 0x0018
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVMPC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVMTS_LSB 0x0019
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVMTS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVMX_LSB 0x001a
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVMX_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVNC_LSB 0x001b
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDISVNC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDIS_SPARE9_LSB 0x001c
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDIS_SPARE9_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDIS_SPARE10_LSB 0x001d
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDIS_SPARE10_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDIS_SPARE11_LSB 0x001e
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDIS_SPARE11_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDIS_SPARE12_LSB 0x001f
#define VDCGCTL3F0C_VDBOX1_MEDIA_CG3DDIS_SPARE12_SIZE 0x0001


/** RC_PSMI_CTRL_VECSUNIT0 desc:
  */

#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_REG (0x001c8050U)

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
    UINT32 disable_idle_rc6_flush : 1; /**< Disable IDLE RC6 flush */
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
                            /* Bits[12:12], default=0x00000000*/
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
} RC_PSMI_CTRL_VECSUNIT0_MEDIA_STRUCT;

#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_WIDTH 32
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_DEFAULT 0x00000000U
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_ENDPOINT MEDIA

#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_DISABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_DISABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_MASK_LSB 0x0010
#define RC_PSMI_CTRL_VECSUNIT0_MEDIA_MASK_SIZE 0x0010


/** PWRCTX_MAXCNT_VECSUNIT0 desc:
  */

#define PWRCTX_MAXCNT_VECSUNIT0_MEDIA_REG (0x001c8054U)

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
} PWRCTX_MAXCNT_VECSUNIT0_MEDIA_STRUCT;

#define PWRCTX_MAXCNT_VECSUNIT0_MEDIA_WIDTH 32
#define PWRCTX_MAXCNT_VECSUNIT0_MEDIA_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_VECSUNIT0_MEDIA_ENDPOINT MEDIA

#define PWRCTX_MAXCNT_VECSUNIT0_MEDIA_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_VECSUNIT0_MEDIA_IDLE_WAIT_TIME_SIZE 0x0014


/** RC_PSMI_CTRL_VCSUNIT2 desc:
  */

#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_REG  (0x001d0050U)

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
    UINT32 disable_idle_rc6_flush : 1; /**< Disable IDLE RC6 flush */
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
                            /* Bits[12:12], default=0x00000000*/
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
} RC_PSMI_CTRL_VCSUNIT2_MEDIA_STRUCT;

#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_WIDTH 32
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_DEFAULT 0x00000000U
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_ENDPOINT MEDIA

#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_DISABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_DISABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_MASK_LSB 0x0010
#define RC_PSMI_CTRL_VCSUNIT2_MEDIA_MASK_SIZE 0x0010


/** PWRCTX_MAXCNT_VCSUNIT2 desc:
  */

#define PWRCTX_MAXCNT_VCSUNIT2_MEDIA_REG (0x001d0054U)

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
} PWRCTX_MAXCNT_VCSUNIT2_MEDIA_STRUCT;

#define PWRCTX_MAXCNT_VCSUNIT2_MEDIA_WIDTH 32
#define PWRCTX_MAXCNT_VCSUNIT2_MEDIA_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_VCSUNIT2_MEDIA_ENDPOINT MEDIA

#define PWRCTX_MAXCNT_VCSUNIT2_MEDIA_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_VCSUNIT2_MEDIA_IDLE_WAIT_TIME_SIZE 0x0014


/** VDCGCTL3F0C_VDBOX2 desc:
  */

#define VDCGCTL3F0C_VDBOX2_MEDIA_REG     (0x001d3f0cU)

typedef union {
  struct {
    UINT32 cg3ddisvcp : 1; /**< VCPunit Clock Gating Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cg3ddis_awf : 1; /**< AWF Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 cg3ddisvcs_cs : 1; /**< Csunit's Clock Gating Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 cg3ddisvdl1 : 1; /**< vdl1unit Clock Gating Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 cg3ddisvds : 1; /**< VDSunit Clock Gating Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 cg3ddisvdx : 1; /**< VDXunit Clock Gating Disable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 cg3ddisvft : 1; /**< VFTunit Clock Gating Disable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 cg3ddisvhme : 1; /**< VHMEunit Clock Gating Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 cg3ddisvhr : 1; /**< VHRunit's Clock Gating Disable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 cg3ddisvime : 1; /**< VIMEunit Clock Gating Disable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 cg3ddisvin_vid1 : 1; /**< VID1 Clock Gating Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 cg3ddisvin_vid2 : 1; /**< VID2 Clock Gating Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 cg3ddisvin_vid3 : 1; /**< VID3 Clock Gating Disable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 cg3ddisvin_vid4 : 1; /**< VID4 Clock Gating Disable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 cg3ddisvin_vid5 : 1; /**< VID5 Clock Gating Disable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 cg3ddisvin_vid6 : 1; /**< VID6 Clock Gating Disable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 cg3ddisvip : 1; /**< VIPunit Clock Gating Disable */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 cg3ddisvis : 1; /**< VISunit Clock Gating Disable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 cg3ddisvit : 1; /**< VITunit Clock Gating Disable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 cg3ddisvlf : 1; /**< VLFunit Clock Gating Disable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 cg3ddisvmb : 1; /**< VMBunit Clock Gating Disable */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 cg3ddisvmc : 1; /**< VMCunit Clock Gating Disable */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 cg3ddisvmcr : 1; /**< VMCRunit Clock Gating Disable */
                            /* Bits[22:22], default=0x00000001*/
    UINT32 cg3ddisvmd : 1; /**< VMDunit Clock Gating Disable */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 cg3ddisvmpc : 1; /**< VMPCunit Clock Gating Disable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 cg3ddisvmts : 1; /**< VMTSunit Clock Gating Disable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 cg3ddisvmx : 1; /**< VMXunit Clock Gating Disable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 cg3ddisvnc : 1; /**< VNCunit Clock Gating Disable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 cg3ddis_spare9 : 1; /**< SPARE Clock Gating Disable9 */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 cg3ddis_spare10 : 1; /**< SPARE Clock Gating Disable10 */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 cg3ddis_spare11 : 1; /**< SPARE Clock Gating Disable11 */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 cg3ddis_spare12 : 1; /**< SPARE Clock Gating Disable12 */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} VDCGCTL3F0C_VDBOX2_MEDIA_STRUCT;

#define VDCGCTL3F0C_VDBOX2_MEDIA_WIDTH 32
#define VDCGCTL3F0C_VDBOX2_MEDIA_DEFAULT 0x00400000U
#define VDCGCTL3F0C_VDBOX2_MEDIA_ENDPOINT MEDIA

#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVCP_LSB 0x0000
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVCP_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDIS_AWF_LSB 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDIS_AWF_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVCS_CS_LSB 0x0002
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVCS_CS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVDL1_LSB 0x0003
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVDL1_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVDS_LSB 0x0004
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVDS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVDX_LSB 0x0005
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVDX_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVFT_LSB 0x0006
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVFT_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVHME_LSB 0x0007
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVHME_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVHR_LSB 0x0008
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVHR_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVIME_LSB 0x0009
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVIME_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVIN_VID1_LSB 0x000a
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVIN_VID1_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVIN_VID2_LSB 0x000b
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVIN_VID2_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVIN_VID3_LSB 0x000c
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVIN_VID3_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVIN_VID4_LSB 0x000d
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVIN_VID4_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVIN_VID5_LSB 0x000e
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVIN_VID5_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVIN_VID6_LSB 0x000f
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVIN_VID6_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVIP_LSB 0x0010
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVIP_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVIS_LSB 0x0011
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVIS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVIT_LSB 0x0012
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVIT_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVLF_LSB 0x0013
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVLF_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVMB_LSB 0x0014
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVMB_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVMC_LSB 0x0015
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVMC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVMCR_LSB 0x0016
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVMCR_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVMD_LSB 0x0017
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVMD_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVMPC_LSB 0x0018
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVMPC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVMTS_LSB 0x0019
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVMTS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVMX_LSB 0x001a
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVMX_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVNC_LSB 0x001b
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDISVNC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDIS_SPARE9_LSB 0x001c
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDIS_SPARE9_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDIS_SPARE10_LSB 0x001d
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDIS_SPARE10_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDIS_SPARE11_LSB 0x001e
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDIS_SPARE11_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDIS_SPARE12_LSB 0x001f
#define VDCGCTL3F0C_VDBOX2_MEDIA_CG3DDIS_SPARE12_SIZE 0x0001


/** VDCGCTL3F1C_VDBOX2 desc:
  */

#define VDCGCTL3F1C_VDBOX2_MEDIA_REG     (0x001d3f1cU)

typedef union {
  struct {
    UINT32 cg3ddis_afg : 1; /**< AFG unit Clock Gating Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cg3ddis_av1pipe : 1; /**< AV1 pipeline Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 cg3ddis_hcppipe : 1; /**< HCP pipeline Clock Gating Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 cg3ddis_mfxpipe : 1; /**< MFX pipeline Clock Gating Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 cg3ddis_vdencpipe : 1; /**< VDENC pipeline Clock Gating Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 cg3ddis_hucpipe : 1; /**< HUC pipeline Clock Gating Disable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 cg3ddis_vdaqmpipe : 1; /**< VDAQM pipeline Clock Gating Disable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 cg3ddis_vrtpipe : 1; /**< VRT clients pipeline Clock Gating Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 cg3ddis_vepipe : 1; /**< VEBOX pipeline Clock Gating Disable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 cg3ddis_sfcpipe : 1; /**< SFC pipeline Clock Gating Disable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 cg3ddisaqs : 1; /**< AQSunit Clock Gating Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 cg3ddisaqm : 1; /**< AQMunit Clock Gating Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 cg3ddisaqx : 1; /**< AQXunit Clock Gating Disable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 cg3ddisaqa : 1; /**< AQAunit Clock Gating Disable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 cg3ddisoaaddrmscmi : 1; /**< OAADDRMunit in SCMI Clock Gating Disable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 cg3ddisoamscmi : 1; /**< oamscmiUnit Clock Gating Disable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 cg3ddisgamctrl : 1; /**< gamctrl Clock Gating Disable */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 cg3ddisgamstlb : 1; /**< gamstlb Clock Gating Disable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 cg3ddisgamcmdiscmi : 1; /**< gamcmdiscmi Clock Gating Disable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 cg3ddisgamdatiscmi : 1; /**< gamdatiscmi Clock Gating Disable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 cg3ddisgamreqstrmscmi : 1; /**< gamreqstrmscmi Clock Gating Disable */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 cg3ddisgamftlbscmi : 1; /**< gamftlbscmi Clock Gating Disable */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 cg3ddisgamfxbarscmi : 1; /**< gamfxbarscmi Clock Gating Disable */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 cg3ddisaxf : 1; /**< AFX Clock Gating Disable */
                            /* Bits[23:23], default=0x00000001*/
    UINT32 cg3ddismrruscmi : 1; /**< MRRUSCMI Clock Gating Disable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 cg3ddispsmi : 1; /**< SCMI PSMI Clock Gating Disable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 cg3ddisaqh : 1; /**< AQH unit Clock Gating Disable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 cg3ddisrdm : 1; /**< RDM unit Clock Gating Disable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 cg3ddis_mgmc : 1; /**< MGMC unit Clock Gating Disable */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 rsvd_0 : 3; /**< Undefined - auto filled rsvd_[31:29] */
                            /* Bits[31:29], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} VDCGCTL3F1C_VDBOX2_MEDIA_STRUCT;

#define VDCGCTL3F1C_VDBOX2_MEDIA_WIDTH 32
#define VDCGCTL3F1C_VDBOX2_MEDIA_DEFAULT 0x00800000U
#define VDCGCTL3F1C_VDBOX2_MEDIA_ENDPOINT MEDIA

#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDIS_AFG_LSB 0x0000
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDIS_AFG_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDIS_AV1PIPE_LSB 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDIS_AV1PIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDIS_HCPPIPE_LSB 0x0002
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDIS_HCPPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDIS_MFXPIPE_LSB 0x0003
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDIS_MFXPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDIS_VDENCPIPE_LSB 0x0004
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDIS_VDENCPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDIS_HUCPIPE_LSB 0x0005
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDIS_HUCPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDIS_VDAQMPIPE_LSB 0x0006
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDIS_VDAQMPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDIS_VRTPIPE_LSB 0x0007
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDIS_VRTPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDIS_VEPIPE_LSB 0x0008
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDIS_VEPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDIS_SFCPIPE_LSB 0x0009
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDIS_SFCPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISAQS_LSB 0x000a
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISAQS_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISAQM_LSB 0x000b
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISAQM_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISAQX_LSB 0x000c
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISAQX_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISAQA_LSB 0x000d
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISAQA_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISOAADDRMSCMI_LSB 0x000e
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISOAADDRMSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISOAMSCMI_LSB 0x000f
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISOAMSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISGAMCTRL_LSB 0x0010
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISGAMCTRL_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISGAMSTLB_LSB 0x0011
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISGAMSTLB_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISGAMCMDISCMI_LSB 0x0012
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISGAMCMDISCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISGAMDATISCMI_LSB 0x0013
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISGAMDATISCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISGAMREQSTRMSCMI_LSB 0x0014
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISGAMREQSTRMSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISGAMFTLBSCMI_LSB 0x0015
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISGAMFTLBSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISGAMFXBARSCMI_LSB 0x0016
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISGAMFXBARSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISAXF_LSB 0x0017
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISAXF_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISMRRUSCMI_LSB 0x0018
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISMRRUSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISPSMI_LSB 0x0019
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISPSMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISAQH_LSB 0x001a
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISAQH_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISRDM_LSB 0x001b
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDISRDM_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDIS_MGMC_LSB 0x001c
#define VDCGCTL3F1C_VDBOX2_MEDIA_CG3DDIS_MGMC_SIZE 0x0001


/** RC_PSMI_CTRL_VCSUNIT3 desc:
  */

#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_REG  (0x001d4050U)

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
    UINT32 disable_idle_rc6_flush : 1; /**< Disable IDLE RC6 flush */
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
                            /* Bits[12:12], default=0x00000000*/
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
} RC_PSMI_CTRL_VCSUNIT3_MEDIA_STRUCT;

#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_WIDTH 32
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_DEFAULT 0x00000000U
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_ENDPOINT MEDIA

#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_DISABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_DISABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_MASK_LSB 0x0010
#define RC_PSMI_CTRL_VCSUNIT3_MEDIA_MASK_SIZE 0x0010


/** PWRCTX_MAXCNT_VCSUNIT3 desc:
  */

#define PWRCTX_MAXCNT_VCSUNIT3_MEDIA_REG (0x001d4054U)

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
} PWRCTX_MAXCNT_VCSUNIT3_MEDIA_STRUCT;

#define PWRCTX_MAXCNT_VCSUNIT3_MEDIA_WIDTH 32
#define PWRCTX_MAXCNT_VCSUNIT3_MEDIA_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_VCSUNIT3_MEDIA_ENDPOINT MEDIA

#define PWRCTX_MAXCNT_VCSUNIT3_MEDIA_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_VCSUNIT3_MEDIA_IDLE_WAIT_TIME_SIZE 0x0014


/** VDCGCTL3F0C_VDBOX3 desc:
  */

#define VDCGCTL3F0C_VDBOX3_MEDIA_REG     (0x001d7f0cU)

typedef union {
  struct {
    UINT32 cg3ddisvcp : 1; /**< VCPunit Clock Gating Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cg3ddis_awf : 1; /**< AWF Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 cg3ddisvcs_cs : 1; /**< Csunit's Clock Gating Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 cg3ddisvdl1 : 1; /**< vdl1unit Clock Gating Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 cg3ddisvds : 1; /**< VDSunit Clock Gating Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 cg3ddisvdx : 1; /**< VDXunit Clock Gating Disable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 cg3ddisvft : 1; /**< VFTunit Clock Gating Disable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 cg3ddisvhme : 1; /**< VHMEunit Clock Gating Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 cg3ddisvhr : 1; /**< VHRunit's Clock Gating Disable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 cg3ddisvime : 1; /**< VIMEunit Clock Gating Disable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 cg3ddisvin_vid1 : 1; /**< VID1 Clock Gating Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 cg3ddisvin_vid2 : 1; /**< VID2 Clock Gating Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 cg3ddisvin_vid3 : 1; /**< VID3 Clock Gating Disable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 cg3ddisvin_vid4 : 1; /**< VID4 Clock Gating Disable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 cg3ddisvin_vid5 : 1; /**< VID5 Clock Gating Disable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 cg3ddisvin_vid6 : 1; /**< VID6 Clock Gating Disable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 cg3ddisvip : 1; /**< VIPunit Clock Gating Disable */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 cg3ddisvis : 1; /**< VISunit Clock Gating Disable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 cg3ddisvit : 1; /**< VITunit Clock Gating Disable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 cg3ddisvlf : 1; /**< VLFunit Clock Gating Disable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 cg3ddisvmb : 1; /**< VMBunit Clock Gating Disable */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 cg3ddisvmc : 1; /**< VMCunit Clock Gating Disable */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 cg3ddisvmcr : 1; /**< VMCRunit Clock Gating Disable */
                            /* Bits[22:22], default=0x00000001*/
    UINT32 cg3ddisvmd : 1; /**< VMDunit Clock Gating Disable */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 cg3ddisvmpc : 1; /**< VMPCunit Clock Gating Disable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 cg3ddisvmts : 1; /**< VMTSunit Clock Gating Disable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 cg3ddisvmx : 1; /**< VMXunit Clock Gating Disable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 cg3ddisvnc : 1; /**< VNCunit Clock Gating Disable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 cg3ddis_spare9 : 1; /**< SPARE Clock Gating Disable9 */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 cg3ddis_spare10 : 1; /**< SPARE Clock Gating Disable10 */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 cg3ddis_spare11 : 1; /**< SPARE Clock Gating Disable11 */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 cg3ddis_spare12 : 1; /**< SPARE Clock Gating Disable12 */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} VDCGCTL3F0C_VDBOX3_MEDIA_STRUCT;

#define VDCGCTL3F0C_VDBOX3_MEDIA_WIDTH 32
#define VDCGCTL3F0C_VDBOX3_MEDIA_DEFAULT 0x00400000U
#define VDCGCTL3F0C_VDBOX3_MEDIA_ENDPOINT MEDIA

#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVCP_LSB 0x0000
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVCP_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDIS_AWF_LSB 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDIS_AWF_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVCS_CS_LSB 0x0002
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVCS_CS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVDL1_LSB 0x0003
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVDL1_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVDS_LSB 0x0004
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVDS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVDX_LSB 0x0005
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVDX_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVFT_LSB 0x0006
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVFT_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVHME_LSB 0x0007
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVHME_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVHR_LSB 0x0008
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVHR_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVIME_LSB 0x0009
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVIME_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVIN_VID1_LSB 0x000a
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVIN_VID1_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVIN_VID2_LSB 0x000b
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVIN_VID2_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVIN_VID3_LSB 0x000c
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVIN_VID3_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVIN_VID4_LSB 0x000d
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVIN_VID4_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVIN_VID5_LSB 0x000e
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVIN_VID5_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVIN_VID6_LSB 0x000f
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVIN_VID6_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVIP_LSB 0x0010
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVIP_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVIS_LSB 0x0011
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVIS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVIT_LSB 0x0012
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVIT_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVLF_LSB 0x0013
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVLF_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVMB_LSB 0x0014
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVMB_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVMC_LSB 0x0015
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVMC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVMCR_LSB 0x0016
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVMCR_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVMD_LSB 0x0017
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVMD_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVMPC_LSB 0x0018
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVMPC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVMTS_LSB 0x0019
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVMTS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVMX_LSB 0x001a
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVMX_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVNC_LSB 0x001b
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDISVNC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDIS_SPARE9_LSB 0x001c
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDIS_SPARE9_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDIS_SPARE10_LSB 0x001d
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDIS_SPARE10_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDIS_SPARE11_LSB 0x001e
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDIS_SPARE11_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDIS_SPARE12_LSB 0x001f
#define VDCGCTL3F0C_VDBOX3_MEDIA_CG3DDIS_SPARE12_SIZE 0x0001


/** RC_PSMI_CTRL_VECSUNIT1 desc:
  */

#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_REG (0x001d8050U)

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
    UINT32 disable_idle_rc6_flush : 1; /**< Disable IDLE RC6 flush */
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
                            /* Bits[12:12], default=0x00000000*/
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
} RC_PSMI_CTRL_VECSUNIT1_MEDIA_STRUCT;

#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_WIDTH 32
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_DEFAULT 0x00000000U
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_ENDPOINT MEDIA

#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_DISABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_DISABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_MASK_LSB 0x0010
#define RC_PSMI_CTRL_VECSUNIT1_MEDIA_MASK_SIZE 0x0010


/** PWRCTX_MAXCNT_VECSUNIT1 desc:
  */

#define PWRCTX_MAXCNT_VECSUNIT1_MEDIA_REG (0x001d8054U)

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
} PWRCTX_MAXCNT_VECSUNIT1_MEDIA_STRUCT;

#define PWRCTX_MAXCNT_VECSUNIT1_MEDIA_WIDTH 32
#define PWRCTX_MAXCNT_VECSUNIT1_MEDIA_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_VECSUNIT1_MEDIA_ENDPOINT MEDIA

#define PWRCTX_MAXCNT_VECSUNIT1_MEDIA_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_VECSUNIT1_MEDIA_IDLE_WAIT_TIME_SIZE 0x0014


/** RC_PSMI_CTRL_VCSUNIT4 desc:
  */

#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_REG  (0x001e0050U)

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
    UINT32 disable_idle_rc6_flush : 1; /**< Disable IDLE RC6 flush */
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
                            /* Bits[12:12], default=0x00000000*/
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
} RC_PSMI_CTRL_VCSUNIT4_MEDIA_STRUCT;

#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_WIDTH 32
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_DEFAULT 0x00000000U
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_ENDPOINT MEDIA

#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_DISABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_DISABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_MASK_LSB 0x0010
#define RC_PSMI_CTRL_VCSUNIT4_MEDIA_MASK_SIZE 0x0010


/** PWRCTX_MAXCNT_VCSUNIT4 desc:
  */

#define PWRCTX_MAXCNT_VCSUNIT4_MEDIA_REG (0x001e0054U)

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
} PWRCTX_MAXCNT_VCSUNIT4_MEDIA_STRUCT;

#define PWRCTX_MAXCNT_VCSUNIT4_MEDIA_WIDTH 32
#define PWRCTX_MAXCNT_VCSUNIT4_MEDIA_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_VCSUNIT4_MEDIA_ENDPOINT MEDIA

#define PWRCTX_MAXCNT_VCSUNIT4_MEDIA_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_VCSUNIT4_MEDIA_IDLE_WAIT_TIME_SIZE 0x0014


/** VDCGCTL3F0C_VDBOX4 desc:
  */

#define VDCGCTL3F0C_VDBOX4_MEDIA_REG     (0x001e3f0cU)

typedef union {
  struct {
    UINT32 cg3ddisvcp : 1; /**< VCPunit Clock Gating Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cg3ddis_awf : 1; /**< AWF Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 cg3ddisvcs_cs : 1; /**< Csunit's Clock Gating Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 cg3ddisvdl1 : 1; /**< vdl1unit Clock Gating Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 cg3ddisvds : 1; /**< VDSunit Clock Gating Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 cg3ddisvdx : 1; /**< VDXunit Clock Gating Disable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 cg3ddisvft : 1; /**< VFTunit Clock Gating Disable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 cg3ddisvhme : 1; /**< VHMEunit Clock Gating Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 cg3ddisvhr : 1; /**< VHRunit's Clock Gating Disable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 cg3ddisvime : 1; /**< VIMEunit Clock Gating Disable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 cg3ddisvin_vid1 : 1; /**< VID1 Clock Gating Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 cg3ddisvin_vid2 : 1; /**< VID2 Clock Gating Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 cg3ddisvin_vid3 : 1; /**< VID3 Clock Gating Disable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 cg3ddisvin_vid4 : 1; /**< VID4 Clock Gating Disable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 cg3ddisvin_vid5 : 1; /**< VID5 Clock Gating Disable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 cg3ddisvin_vid6 : 1; /**< VID6 Clock Gating Disable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 cg3ddisvip : 1; /**< VIPunit Clock Gating Disable */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 cg3ddisvis : 1; /**< VISunit Clock Gating Disable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 cg3ddisvit : 1; /**< VITunit Clock Gating Disable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 cg3ddisvlf : 1; /**< VLFunit Clock Gating Disable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 cg3ddisvmb : 1; /**< VMBunit Clock Gating Disable */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 cg3ddisvmc : 1; /**< VMCunit Clock Gating Disable */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 cg3ddisvmcr : 1; /**< VMCRunit Clock Gating Disable */
                            /* Bits[22:22], default=0x00000001*/
    UINT32 cg3ddisvmd : 1; /**< VMDunit Clock Gating Disable */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 cg3ddisvmpc : 1; /**< VMPCunit Clock Gating Disable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 cg3ddisvmts : 1; /**< VMTSunit Clock Gating Disable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 cg3ddisvmx : 1; /**< VMXunit Clock Gating Disable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 cg3ddisvnc : 1; /**< VNCunit Clock Gating Disable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 cg3ddis_spare9 : 1; /**< SPARE Clock Gating Disable9 */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 cg3ddis_spare10 : 1; /**< SPARE Clock Gating Disable10 */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 cg3ddis_spare11 : 1; /**< SPARE Clock Gating Disable11 */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 cg3ddis_spare12 : 1; /**< SPARE Clock Gating Disable12 */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} VDCGCTL3F0C_VDBOX4_MEDIA_STRUCT;

#define VDCGCTL3F0C_VDBOX4_MEDIA_WIDTH 32
#define VDCGCTL3F0C_VDBOX4_MEDIA_DEFAULT 0x00400000U
#define VDCGCTL3F0C_VDBOX4_MEDIA_ENDPOINT MEDIA

#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVCP_LSB 0x0000
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVCP_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDIS_AWF_LSB 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDIS_AWF_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVCS_CS_LSB 0x0002
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVCS_CS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVDL1_LSB 0x0003
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVDL1_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVDS_LSB 0x0004
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVDS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVDX_LSB 0x0005
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVDX_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVFT_LSB 0x0006
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVFT_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVHME_LSB 0x0007
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVHME_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVHR_LSB 0x0008
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVHR_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVIME_LSB 0x0009
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVIME_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVIN_VID1_LSB 0x000a
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVIN_VID1_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVIN_VID2_LSB 0x000b
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVIN_VID2_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVIN_VID3_LSB 0x000c
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVIN_VID3_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVIN_VID4_LSB 0x000d
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVIN_VID4_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVIN_VID5_LSB 0x000e
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVIN_VID5_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVIN_VID6_LSB 0x000f
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVIN_VID6_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVIP_LSB 0x0010
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVIP_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVIS_LSB 0x0011
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVIS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVIT_LSB 0x0012
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVIT_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVLF_LSB 0x0013
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVLF_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVMB_LSB 0x0014
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVMB_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVMC_LSB 0x0015
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVMC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVMCR_LSB 0x0016
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVMCR_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVMD_LSB 0x0017
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVMD_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVMPC_LSB 0x0018
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVMPC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVMTS_LSB 0x0019
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVMTS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVMX_LSB 0x001a
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVMX_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVNC_LSB 0x001b
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDISVNC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDIS_SPARE9_LSB 0x001c
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDIS_SPARE9_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDIS_SPARE10_LSB 0x001d
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDIS_SPARE10_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDIS_SPARE11_LSB 0x001e
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDIS_SPARE11_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDIS_SPARE12_LSB 0x001f
#define VDCGCTL3F0C_VDBOX4_MEDIA_CG3DDIS_SPARE12_SIZE 0x0001


/** VDCGCTL3F1C_VDBOX4 desc:
  */

#define VDCGCTL3F1C_VDBOX4_MEDIA_REG     (0x001e3f1cU)

typedef union {
  struct {
    UINT32 cg3ddis_afg : 1; /**< AFG unit Clock Gating Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cg3ddis_av1pipe : 1; /**< AV1 pipeline Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 cg3ddis_hcppipe : 1; /**< HCP pipeline Clock Gating Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 cg3ddis_mfxpipe : 1; /**< MFX pipeline Clock Gating Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 cg3ddis_vdencpipe : 1; /**< VDENC pipeline Clock Gating Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 cg3ddis_hucpipe : 1; /**< HUC pipeline Clock Gating Disable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 cg3ddis_vdaqmpipe : 1; /**< VDAQM pipeline Clock Gating Disable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 cg3ddis_vrtpipe : 1; /**< VRT clients pipeline Clock Gating Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 cg3ddis_vepipe : 1; /**< VEBOX pipeline Clock Gating Disable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 cg3ddis_sfcpipe : 1; /**< SFC pipeline Clock Gating Disable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 cg3ddisaqs : 1; /**< AQSunit Clock Gating Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 cg3ddisaqm : 1; /**< AQMunit Clock Gating Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 cg3ddisaqx : 1; /**< AQXunit Clock Gating Disable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 cg3ddisaqa : 1; /**< AQAunit Clock Gating Disable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 cg3ddisoaaddrmscmi : 1; /**< OAADDRMunit in SCMI Clock Gating Disable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 cg3ddisoamscmi : 1; /**< oamscmiUnit Clock Gating Disable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 cg3ddisgamctrl : 1; /**< gamctrl Clock Gating Disable */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 cg3ddisgamstlb : 1; /**< gamstlb Clock Gating Disable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 cg3ddisgamcmdiscmi : 1; /**< gamcmdiscmi Clock Gating Disable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 cg3ddisgamdatiscmi : 1; /**< gamdatiscmi Clock Gating Disable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 cg3ddisgamreqstrmscmi : 1; /**< gamreqstrmscmi Clock Gating Disable */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 cg3ddisgamftlbscmi : 1; /**< gamftlbscmi Clock Gating Disable */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 cg3ddisgamfxbarscmi : 1; /**< gamfxbarscmi Clock Gating Disable */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 cg3ddisaxf : 1; /**< AFX Clock Gating Disable */
                            /* Bits[23:23], default=0x00000001*/
    UINT32 cg3ddismrruscmi : 1; /**< MRRUSCMI Clock Gating Disable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 cg3ddispsmi : 1; /**< SCMI PSMI Clock Gating Disable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 cg3ddisaqh : 1; /**< AQH unit Clock Gating Disable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 cg3ddisrdm : 1; /**< RDM unit Clock Gating Disable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 cg3ddis_mgmc : 1; /**< MGMC unit Clock Gating Disable */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 rsvd_0 : 3; /**< Undefined - auto filled rsvd_[31:29] */
                            /* Bits[31:29], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} VDCGCTL3F1C_VDBOX4_MEDIA_STRUCT;

#define VDCGCTL3F1C_VDBOX4_MEDIA_WIDTH 32
#define VDCGCTL3F1C_VDBOX4_MEDIA_DEFAULT 0x00800000U
#define VDCGCTL3F1C_VDBOX4_MEDIA_ENDPOINT MEDIA

#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDIS_AFG_LSB 0x0000
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDIS_AFG_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDIS_AV1PIPE_LSB 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDIS_AV1PIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDIS_HCPPIPE_LSB 0x0002
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDIS_HCPPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDIS_MFXPIPE_LSB 0x0003
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDIS_MFXPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDIS_VDENCPIPE_LSB 0x0004
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDIS_VDENCPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDIS_HUCPIPE_LSB 0x0005
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDIS_HUCPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDIS_VDAQMPIPE_LSB 0x0006
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDIS_VDAQMPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDIS_VRTPIPE_LSB 0x0007
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDIS_VRTPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDIS_VEPIPE_LSB 0x0008
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDIS_VEPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDIS_SFCPIPE_LSB 0x0009
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDIS_SFCPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISAQS_LSB 0x000a
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISAQS_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISAQM_LSB 0x000b
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISAQM_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISAQX_LSB 0x000c
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISAQX_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISAQA_LSB 0x000d
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISAQA_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISOAADDRMSCMI_LSB 0x000e
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISOAADDRMSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISOAMSCMI_LSB 0x000f
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISOAMSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISGAMCTRL_LSB 0x0010
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISGAMCTRL_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISGAMSTLB_LSB 0x0011
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISGAMSTLB_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISGAMCMDISCMI_LSB 0x0012
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISGAMCMDISCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISGAMDATISCMI_LSB 0x0013
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISGAMDATISCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISGAMREQSTRMSCMI_LSB 0x0014
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISGAMREQSTRMSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISGAMFTLBSCMI_LSB 0x0015
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISGAMFTLBSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISGAMFXBARSCMI_LSB 0x0016
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISGAMFXBARSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISAXF_LSB 0x0017
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISAXF_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISMRRUSCMI_LSB 0x0018
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISMRRUSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISPSMI_LSB 0x0019
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISPSMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISAQH_LSB 0x001a
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISAQH_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISRDM_LSB 0x001b
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDISRDM_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDIS_MGMC_LSB 0x001c
#define VDCGCTL3F1C_VDBOX4_MEDIA_CG3DDIS_MGMC_SIZE 0x0001


/** RC_PSMI_CTRL_VCSUNIT5 desc:
  */

#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_REG  (0x001e4050U)

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
    UINT32 disable_idle_rc6_flush : 1; /**< Disable IDLE RC6 flush */
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
                            /* Bits[12:12], default=0x00000000*/
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
} RC_PSMI_CTRL_VCSUNIT5_MEDIA_STRUCT;

#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_WIDTH 32
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_DEFAULT 0x00000000U
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_ENDPOINT MEDIA

#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_DISABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_DISABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_MASK_LSB 0x0010
#define RC_PSMI_CTRL_VCSUNIT5_MEDIA_MASK_SIZE 0x0010


/** PWRCTX_MAXCNT_VCSUNIT5 desc:
  */

#define PWRCTX_MAXCNT_VCSUNIT5_MEDIA_REG (0x001e4054U)

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
} PWRCTX_MAXCNT_VCSUNIT5_MEDIA_STRUCT;

#define PWRCTX_MAXCNT_VCSUNIT5_MEDIA_WIDTH 32
#define PWRCTX_MAXCNT_VCSUNIT5_MEDIA_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_VCSUNIT5_MEDIA_ENDPOINT MEDIA

#define PWRCTX_MAXCNT_VCSUNIT5_MEDIA_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_VCSUNIT5_MEDIA_IDLE_WAIT_TIME_SIZE 0x0014


/** VDCGCTL3F0C_VDBOX5 desc:
  */

#define VDCGCTL3F0C_VDBOX5_MEDIA_REG     (0x001e7f0cU)

typedef union {
  struct {
    UINT32 cg3ddisvcp : 1; /**< VCPunit Clock Gating Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cg3ddis_awf : 1; /**< AWF Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 cg3ddisvcs_cs : 1; /**< Csunit's Clock Gating Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 cg3ddisvdl1 : 1; /**< vdl1unit Clock Gating Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 cg3ddisvds : 1; /**< VDSunit Clock Gating Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 cg3ddisvdx : 1; /**< VDXunit Clock Gating Disable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 cg3ddisvft : 1; /**< VFTunit Clock Gating Disable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 cg3ddisvhme : 1; /**< VHMEunit Clock Gating Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 cg3ddisvhr : 1; /**< VHRunit's Clock Gating Disable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 cg3ddisvime : 1; /**< VIMEunit Clock Gating Disable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 cg3ddisvin_vid1 : 1; /**< VID1 Clock Gating Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 cg3ddisvin_vid2 : 1; /**< VID2 Clock Gating Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 cg3ddisvin_vid3 : 1; /**< VID3 Clock Gating Disable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 cg3ddisvin_vid4 : 1; /**< VID4 Clock Gating Disable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 cg3ddisvin_vid5 : 1; /**< VID5 Clock Gating Disable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 cg3ddisvin_vid6 : 1; /**< VID6 Clock Gating Disable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 cg3ddisvip : 1; /**< VIPunit Clock Gating Disable */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 cg3ddisvis : 1; /**< VISunit Clock Gating Disable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 cg3ddisvit : 1; /**< VITunit Clock Gating Disable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 cg3ddisvlf : 1; /**< VLFunit Clock Gating Disable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 cg3ddisvmb : 1; /**< VMBunit Clock Gating Disable */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 cg3ddisvmc : 1; /**< VMCunit Clock Gating Disable */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 cg3ddisvmcr : 1; /**< VMCRunit Clock Gating Disable */
                            /* Bits[22:22], default=0x00000001*/
    UINT32 cg3ddisvmd : 1; /**< VMDunit Clock Gating Disable */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 cg3ddisvmpc : 1; /**< VMPCunit Clock Gating Disable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 cg3ddisvmts : 1; /**< VMTSunit Clock Gating Disable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 cg3ddisvmx : 1; /**< VMXunit Clock Gating Disable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 cg3ddisvnc : 1; /**< VNCunit Clock Gating Disable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 cg3ddis_spare9 : 1; /**< SPARE Clock Gating Disable9 */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 cg3ddis_spare10 : 1; /**< SPARE Clock Gating Disable10 */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 cg3ddis_spare11 : 1; /**< SPARE Clock Gating Disable11 */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 cg3ddis_spare12 : 1; /**< SPARE Clock Gating Disable12 */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} VDCGCTL3F0C_VDBOX5_MEDIA_STRUCT;

#define VDCGCTL3F0C_VDBOX5_MEDIA_WIDTH 32
#define VDCGCTL3F0C_VDBOX5_MEDIA_DEFAULT 0x00400000U
#define VDCGCTL3F0C_VDBOX5_MEDIA_ENDPOINT MEDIA

#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVCP_LSB 0x0000
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVCP_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDIS_AWF_LSB 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDIS_AWF_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVCS_CS_LSB 0x0002
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVCS_CS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVDL1_LSB 0x0003
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVDL1_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVDS_LSB 0x0004
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVDS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVDX_LSB 0x0005
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVDX_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVFT_LSB 0x0006
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVFT_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVHME_LSB 0x0007
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVHME_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVHR_LSB 0x0008
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVHR_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVIME_LSB 0x0009
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVIME_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVIN_VID1_LSB 0x000a
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVIN_VID1_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVIN_VID2_LSB 0x000b
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVIN_VID2_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVIN_VID3_LSB 0x000c
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVIN_VID3_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVIN_VID4_LSB 0x000d
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVIN_VID4_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVIN_VID5_LSB 0x000e
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVIN_VID5_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVIN_VID6_LSB 0x000f
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVIN_VID6_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVIP_LSB 0x0010
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVIP_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVIS_LSB 0x0011
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVIS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVIT_LSB 0x0012
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVIT_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVLF_LSB 0x0013
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVLF_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVMB_LSB 0x0014
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVMB_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVMC_LSB 0x0015
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVMC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVMCR_LSB 0x0016
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVMCR_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVMD_LSB 0x0017
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVMD_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVMPC_LSB 0x0018
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVMPC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVMTS_LSB 0x0019
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVMTS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVMX_LSB 0x001a
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVMX_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVNC_LSB 0x001b
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDISVNC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDIS_SPARE9_LSB 0x001c
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDIS_SPARE9_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDIS_SPARE10_LSB 0x001d
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDIS_SPARE10_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDIS_SPARE11_LSB 0x001e
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDIS_SPARE11_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDIS_SPARE12_LSB 0x001f
#define VDCGCTL3F0C_VDBOX5_MEDIA_CG3DDIS_SPARE12_SIZE 0x0001


/** RC_PSMI_CTRL_VECSUNIT2 desc:
  */

#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_REG (0x001e8050U)

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
    UINT32 disable_idle_rc6_flush : 1; /**< Disable IDLE RC6 flush */
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
                            /* Bits[12:12], default=0x00000000*/
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
} RC_PSMI_CTRL_VECSUNIT2_MEDIA_STRUCT;

#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_WIDTH 32
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_DEFAULT 0x00000000U
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_ENDPOINT MEDIA

#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_DISABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_DISABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_MASK_LSB 0x0010
#define RC_PSMI_CTRL_VECSUNIT2_MEDIA_MASK_SIZE 0x0010


/** PWRCTX_MAXCNT_VECSUNIT2 desc:
  */

#define PWRCTX_MAXCNT_VECSUNIT2_MEDIA_REG (0x001e8054U)

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
} PWRCTX_MAXCNT_VECSUNIT2_MEDIA_STRUCT;

#define PWRCTX_MAXCNT_VECSUNIT2_MEDIA_WIDTH 32
#define PWRCTX_MAXCNT_VECSUNIT2_MEDIA_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_VECSUNIT2_MEDIA_ENDPOINT MEDIA

#define PWRCTX_MAXCNT_VECSUNIT2_MEDIA_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_VECSUNIT2_MEDIA_IDLE_WAIT_TIME_SIZE 0x0014


/** RC_PSMI_CTRL_VCSUNIT6 desc:
  */

#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_REG  (0x001f0050U)

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
    UINT32 disable_idle_rc6_flush : 1; /**< Disable IDLE RC6 flush */
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
                            /* Bits[12:12], default=0x00000000*/
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
} RC_PSMI_CTRL_VCSUNIT6_MEDIA_STRUCT;

#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_WIDTH 32
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_DEFAULT 0x00000000U
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_ENDPOINT MEDIA

#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_DISABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_DISABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_MASK_LSB 0x0010
#define RC_PSMI_CTRL_VCSUNIT6_MEDIA_MASK_SIZE 0x0010


/** PWRCTX_MAXCNT_VCSUNIT6 desc:
  */

#define PWRCTX_MAXCNT_VCSUNIT6_MEDIA_REG (0x001f0054U)

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
} PWRCTX_MAXCNT_VCSUNIT6_MEDIA_STRUCT;

#define PWRCTX_MAXCNT_VCSUNIT6_MEDIA_WIDTH 32
#define PWRCTX_MAXCNT_VCSUNIT6_MEDIA_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_VCSUNIT6_MEDIA_ENDPOINT MEDIA

#define PWRCTX_MAXCNT_VCSUNIT6_MEDIA_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_VCSUNIT6_MEDIA_IDLE_WAIT_TIME_SIZE 0x0014


/** VDCGCTL3F0C_VDBOX6 desc:
  */

#define VDCGCTL3F0C_VDBOX6_MEDIA_REG     (0x001f3f0cU)

typedef union {
  struct {
    UINT32 cg3ddisvcp : 1; /**< VCPunit Clock Gating Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cg3ddis_awf : 1; /**< AWF Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 cg3ddisvcs_cs : 1; /**< Csunit's Clock Gating Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 cg3ddisvdl1 : 1; /**< vdl1unit Clock Gating Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 cg3ddisvds : 1; /**< VDSunit Clock Gating Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 cg3ddisvdx : 1; /**< VDXunit Clock Gating Disable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 cg3ddisvft : 1; /**< VFTunit Clock Gating Disable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 cg3ddisvhme : 1; /**< VHMEunit Clock Gating Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 cg3ddisvhr : 1; /**< VHRunit's Clock Gating Disable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 cg3ddisvime : 1; /**< VIMEunit Clock Gating Disable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 cg3ddisvin_vid1 : 1; /**< VID1 Clock Gating Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 cg3ddisvin_vid2 : 1; /**< VID2 Clock Gating Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 cg3ddisvin_vid3 : 1; /**< VID3 Clock Gating Disable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 cg3ddisvin_vid4 : 1; /**< VID4 Clock Gating Disable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 cg3ddisvin_vid5 : 1; /**< VID5 Clock Gating Disable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 cg3ddisvin_vid6 : 1; /**< VID6 Clock Gating Disable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 cg3ddisvip : 1; /**< VIPunit Clock Gating Disable */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 cg3ddisvis : 1; /**< VISunit Clock Gating Disable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 cg3ddisvit : 1; /**< VITunit Clock Gating Disable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 cg3ddisvlf : 1; /**< VLFunit Clock Gating Disable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 cg3ddisvmb : 1; /**< VMBunit Clock Gating Disable */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 cg3ddisvmc : 1; /**< VMCunit Clock Gating Disable */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 cg3ddisvmcr : 1; /**< VMCRunit Clock Gating Disable */
                            /* Bits[22:22], default=0x00000001*/
    UINT32 cg3ddisvmd : 1; /**< VMDunit Clock Gating Disable */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 cg3ddisvmpc : 1; /**< VMPCunit Clock Gating Disable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 cg3ddisvmts : 1; /**< VMTSunit Clock Gating Disable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 cg3ddisvmx : 1; /**< VMXunit Clock Gating Disable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 cg3ddisvnc : 1; /**< VNCunit Clock Gating Disable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 cg3ddis_spare9 : 1; /**< SPARE Clock Gating Disable9 */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 cg3ddis_spare10 : 1; /**< SPARE Clock Gating Disable10 */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 cg3ddis_spare11 : 1; /**< SPARE Clock Gating Disable11 */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 cg3ddis_spare12 : 1; /**< SPARE Clock Gating Disable12 */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} VDCGCTL3F0C_VDBOX6_MEDIA_STRUCT;

#define VDCGCTL3F0C_VDBOX6_MEDIA_WIDTH 32
#define VDCGCTL3F0C_VDBOX6_MEDIA_DEFAULT 0x00400000U
#define VDCGCTL3F0C_VDBOX6_MEDIA_ENDPOINT MEDIA

#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVCP_LSB 0x0000
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVCP_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDIS_AWF_LSB 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDIS_AWF_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVCS_CS_LSB 0x0002
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVCS_CS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVDL1_LSB 0x0003
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVDL1_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVDS_LSB 0x0004
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVDS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVDX_LSB 0x0005
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVDX_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVFT_LSB 0x0006
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVFT_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVHME_LSB 0x0007
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVHME_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVHR_LSB 0x0008
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVHR_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVIME_LSB 0x0009
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVIME_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVIN_VID1_LSB 0x000a
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVIN_VID1_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVIN_VID2_LSB 0x000b
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVIN_VID2_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVIN_VID3_LSB 0x000c
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVIN_VID3_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVIN_VID4_LSB 0x000d
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVIN_VID4_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVIN_VID5_LSB 0x000e
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVIN_VID5_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVIN_VID6_LSB 0x000f
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVIN_VID6_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVIP_LSB 0x0010
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVIP_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVIS_LSB 0x0011
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVIS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVIT_LSB 0x0012
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVIT_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVLF_LSB 0x0013
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVLF_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVMB_LSB 0x0014
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVMB_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVMC_LSB 0x0015
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVMC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVMCR_LSB 0x0016
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVMCR_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVMD_LSB 0x0017
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVMD_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVMPC_LSB 0x0018
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVMPC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVMTS_LSB 0x0019
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVMTS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVMX_LSB 0x001a
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVMX_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVNC_LSB 0x001b
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDISVNC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDIS_SPARE9_LSB 0x001c
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDIS_SPARE9_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDIS_SPARE10_LSB 0x001d
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDIS_SPARE10_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDIS_SPARE11_LSB 0x001e
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDIS_SPARE11_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDIS_SPARE12_LSB 0x001f
#define VDCGCTL3F0C_VDBOX6_MEDIA_CG3DDIS_SPARE12_SIZE 0x0001


/** VDCGCTL3F1C_VDBOX6 desc:
  */

#define VDCGCTL3F1C_VDBOX6_MEDIA_REG     (0x001f3f1cU)

typedef union {
  struct {
    UINT32 cg3ddis_afg : 1; /**< AFG unit Clock Gating Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cg3ddis_av1pipe : 1; /**< AV1 pipeline Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 cg3ddis_hcppipe : 1; /**< HCP pipeline Clock Gating Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 cg3ddis_mfxpipe : 1; /**< MFX pipeline Clock Gating Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 cg3ddis_vdencpipe : 1; /**< VDENC pipeline Clock Gating Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 cg3ddis_hucpipe : 1; /**< HUC pipeline Clock Gating Disable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 cg3ddis_vdaqmpipe : 1; /**< VDAQM pipeline Clock Gating Disable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 cg3ddis_vrtpipe : 1; /**< VRT clients pipeline Clock Gating Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 cg3ddis_vepipe : 1; /**< VEBOX pipeline Clock Gating Disable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 cg3ddis_sfcpipe : 1; /**< SFC pipeline Clock Gating Disable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 cg3ddisaqs : 1; /**< AQSunit Clock Gating Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 cg3ddisaqm : 1; /**< AQMunit Clock Gating Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 cg3ddisaqx : 1; /**< AQXunit Clock Gating Disable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 cg3ddisaqa : 1; /**< AQAunit Clock Gating Disable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 cg3ddisoaaddrmscmi : 1; /**< OAADDRMunit in SCMI Clock Gating Disable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 cg3ddisoamscmi : 1; /**< oamscmiUnit Clock Gating Disable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 cg3ddisgamctrl : 1; /**< gamctrl Clock Gating Disable */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 cg3ddisgamstlb : 1; /**< gamstlb Clock Gating Disable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 cg3ddisgamcmdiscmi : 1; /**< gamcmdiscmi Clock Gating Disable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 cg3ddisgamdatiscmi : 1; /**< gamdatiscmi Clock Gating Disable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 cg3ddisgamreqstrmscmi : 1; /**< gamreqstrmscmi Clock Gating Disable */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 cg3ddisgamftlbscmi : 1; /**< gamftlbscmi Clock Gating Disable */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 cg3ddisgamfxbarscmi : 1; /**< gamfxbarscmi Clock Gating Disable */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 cg3ddisaxf : 1; /**< AFX Clock Gating Disable */
                            /* Bits[23:23], default=0x00000001*/
    UINT32 cg3ddismrruscmi : 1; /**< MRRUSCMI Clock Gating Disable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 cg3ddispsmi : 1; /**< SCMI PSMI Clock Gating Disable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 cg3ddisaqh : 1; /**< AQH unit Clock Gating Disable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 cg3ddisrdm : 1; /**< RDM unit Clock Gating Disable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 cg3ddis_mgmc : 1; /**< MGMC unit Clock Gating Disable */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 rsvd_0 : 3; /**< Undefined - auto filled rsvd_[31:29] */
                            /* Bits[31:29], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} VDCGCTL3F1C_VDBOX6_MEDIA_STRUCT;

#define VDCGCTL3F1C_VDBOX6_MEDIA_WIDTH 32
#define VDCGCTL3F1C_VDBOX6_MEDIA_DEFAULT 0x00800000U
#define VDCGCTL3F1C_VDBOX6_MEDIA_ENDPOINT MEDIA

#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDIS_AFG_LSB 0x0000
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDIS_AFG_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDIS_AV1PIPE_LSB 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDIS_AV1PIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDIS_HCPPIPE_LSB 0x0002
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDIS_HCPPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDIS_MFXPIPE_LSB 0x0003
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDIS_MFXPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDIS_VDENCPIPE_LSB 0x0004
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDIS_VDENCPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDIS_HUCPIPE_LSB 0x0005
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDIS_HUCPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDIS_VDAQMPIPE_LSB 0x0006
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDIS_VDAQMPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDIS_VRTPIPE_LSB 0x0007
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDIS_VRTPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDIS_VEPIPE_LSB 0x0008
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDIS_VEPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDIS_SFCPIPE_LSB 0x0009
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDIS_SFCPIPE_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISAQS_LSB 0x000a
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISAQS_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISAQM_LSB 0x000b
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISAQM_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISAQX_LSB 0x000c
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISAQX_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISAQA_LSB 0x000d
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISAQA_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISOAADDRMSCMI_LSB 0x000e
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISOAADDRMSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISOAMSCMI_LSB 0x000f
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISOAMSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISGAMCTRL_LSB 0x0010
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISGAMCTRL_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISGAMSTLB_LSB 0x0011
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISGAMSTLB_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISGAMCMDISCMI_LSB 0x0012
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISGAMCMDISCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISGAMDATISCMI_LSB 0x0013
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISGAMDATISCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISGAMREQSTRMSCMI_LSB 0x0014
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISGAMREQSTRMSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISGAMFTLBSCMI_LSB 0x0015
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISGAMFTLBSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISGAMFXBARSCMI_LSB 0x0016
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISGAMFXBARSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISAXF_LSB 0x0017
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISAXF_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISMRRUSCMI_LSB 0x0018
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISMRRUSCMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISPSMI_LSB 0x0019
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISPSMI_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISAQH_LSB 0x001a
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISAQH_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISRDM_LSB 0x001b
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDISRDM_SIZE 0x0001
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDIS_MGMC_LSB 0x001c
#define VDCGCTL3F1C_VDBOX6_MEDIA_CG3DDIS_MGMC_SIZE 0x0001


/** RC_PSMI_CTRL_VCSUNIT7 desc:
  */

#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_REG  (0x001f4050U)

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
    UINT32 disable_idle_rc6_flush : 1; /**< Disable IDLE RC6 flush */
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
                            /* Bits[12:12], default=0x00000000*/
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
} RC_PSMI_CTRL_VCSUNIT7_MEDIA_STRUCT;

#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_WIDTH 32
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_DEFAULT 0x00000000U
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_ENDPOINT MEDIA

#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_DISABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_DISABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_MASK_LSB 0x0010
#define RC_PSMI_CTRL_VCSUNIT7_MEDIA_MASK_SIZE 0x0010


/** PWRCTX_MAXCNT_VCSUNIT7 desc:
  */

#define PWRCTX_MAXCNT_VCSUNIT7_MEDIA_REG (0x001f4054U)

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
} PWRCTX_MAXCNT_VCSUNIT7_MEDIA_STRUCT;

#define PWRCTX_MAXCNT_VCSUNIT7_MEDIA_WIDTH 32
#define PWRCTX_MAXCNT_VCSUNIT7_MEDIA_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_VCSUNIT7_MEDIA_ENDPOINT MEDIA

#define PWRCTX_MAXCNT_VCSUNIT7_MEDIA_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_VCSUNIT7_MEDIA_IDLE_WAIT_TIME_SIZE 0x0014


/** VDCGCTL3F0C_VDBOX7 desc:
  */

#define VDCGCTL3F0C_VDBOX7_MEDIA_REG     (0x001f7f0cU)

typedef union {
  struct {
    UINT32 cg3ddisvcp : 1; /**< VCPunit Clock Gating Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cg3ddis_awf : 1; /**< AWF Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 cg3ddisvcs_cs : 1; /**< Csunit's Clock Gating Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 cg3ddisvdl1 : 1; /**< vdl1unit Clock Gating Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 cg3ddisvds : 1; /**< VDSunit Clock Gating Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 cg3ddisvdx : 1; /**< VDXunit Clock Gating Disable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 cg3ddisvft : 1; /**< VFTunit Clock Gating Disable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 cg3ddisvhme : 1; /**< VHMEunit Clock Gating Disable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 cg3ddisvhr : 1; /**< VHRunit's Clock Gating Disable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 cg3ddisvime : 1; /**< VIMEunit Clock Gating Disable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 cg3ddisvin_vid1 : 1; /**< VID1 Clock Gating Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 cg3ddisvin_vid2 : 1; /**< VID2 Clock Gating Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 cg3ddisvin_vid3 : 1; /**< VID3 Clock Gating Disable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 cg3ddisvin_vid4 : 1; /**< VID4 Clock Gating Disable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 cg3ddisvin_vid5 : 1; /**< VID5 Clock Gating Disable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 cg3ddisvin_vid6 : 1; /**< VID6 Clock Gating Disable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 cg3ddisvip : 1; /**< VIPunit Clock Gating Disable */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 cg3ddisvis : 1; /**< VISunit Clock Gating Disable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 cg3ddisvit : 1; /**< VITunit Clock Gating Disable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 cg3ddisvlf : 1; /**< VLFunit Clock Gating Disable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 cg3ddisvmb : 1; /**< VMBunit Clock Gating Disable */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 cg3ddisvmc : 1; /**< VMCunit Clock Gating Disable */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 cg3ddisvmcr : 1; /**< VMCRunit Clock Gating Disable */
                            /* Bits[22:22], default=0x00000001*/
    UINT32 cg3ddisvmd : 1; /**< VMDunit Clock Gating Disable */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 cg3ddisvmpc : 1; /**< VMPCunit Clock Gating Disable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 cg3ddisvmts : 1; /**< VMTSunit Clock Gating Disable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 cg3ddisvmx : 1; /**< VMXunit Clock Gating Disable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 cg3ddisvnc : 1; /**< VNCunit Clock Gating Disable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 cg3ddis_spare9 : 1; /**< SPARE Clock Gating Disable9 */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 cg3ddis_spare10 : 1; /**< SPARE Clock Gating Disable10 */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 cg3ddis_spare11 : 1; /**< SPARE Clock Gating Disable11 */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 cg3ddis_spare12 : 1; /**< SPARE Clock Gating Disable12 */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} VDCGCTL3F0C_VDBOX7_MEDIA_STRUCT;

#define VDCGCTL3F0C_VDBOX7_MEDIA_WIDTH 32
#define VDCGCTL3F0C_VDBOX7_MEDIA_DEFAULT 0x00400000U
#define VDCGCTL3F0C_VDBOX7_MEDIA_ENDPOINT MEDIA

#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVCP_LSB 0x0000
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVCP_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDIS_AWF_LSB 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDIS_AWF_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVCS_CS_LSB 0x0002
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVCS_CS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVDL1_LSB 0x0003
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVDL1_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVDS_LSB 0x0004
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVDS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVDX_LSB 0x0005
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVDX_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVFT_LSB 0x0006
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVFT_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVHME_LSB 0x0007
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVHME_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVHR_LSB 0x0008
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVHR_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVIME_LSB 0x0009
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVIME_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVIN_VID1_LSB 0x000a
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVIN_VID1_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVIN_VID2_LSB 0x000b
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVIN_VID2_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVIN_VID3_LSB 0x000c
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVIN_VID3_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVIN_VID4_LSB 0x000d
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVIN_VID4_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVIN_VID5_LSB 0x000e
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVIN_VID5_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVIN_VID6_LSB 0x000f
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVIN_VID6_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVIP_LSB 0x0010
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVIP_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVIS_LSB 0x0011
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVIS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVIT_LSB 0x0012
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVIT_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVLF_LSB 0x0013
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVLF_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVMB_LSB 0x0014
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVMB_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVMC_LSB 0x0015
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVMC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVMCR_LSB 0x0016
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVMCR_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVMD_LSB 0x0017
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVMD_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVMPC_LSB 0x0018
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVMPC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVMTS_LSB 0x0019
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVMTS_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVMX_LSB 0x001a
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVMX_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVNC_LSB 0x001b
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDISVNC_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDIS_SPARE9_LSB 0x001c
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDIS_SPARE9_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDIS_SPARE10_LSB 0x001d
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDIS_SPARE10_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDIS_SPARE11_LSB 0x001e
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDIS_SPARE11_SIZE 0x0001
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDIS_SPARE12_LSB 0x001f
#define VDCGCTL3F0C_VDBOX7_MEDIA_CG3DDIS_SPARE12_SIZE 0x0001


/** RC_PSMI_CTRL_VECSUNIT3 desc:
  */

#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_REG (0x001f8050U)

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
    UINT32 disable_idle_rc6_flush : 1; /**< Disable IDLE RC6 flush */
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
                            /* Bits[12:12], default=0x00000000*/
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
} RC_PSMI_CTRL_VECSUNIT3_MEDIA_STRUCT;

#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_WIDTH 32
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_DEFAULT 0x00000000U
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_ENDPOINT MEDIA

#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_RC__IDLE_MESSAGE_DISABLE_LSB 0x0000
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_RC__IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_MESSAGE_BYPASS_LSB 0x0001
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_MESSAGE_BYPASS_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_IDLE_FLUSH_DISABLE_LSB 0x0002
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_IDLE_FLUSH_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_IDLE_INDICATOR_LSB 0x0003
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_IDLE_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_GO_INDICATOR_LSB 0x0004
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_GO_INDICATOR_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_LSB 0x0005
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_DISABLE_IDLE_DOP_FLUSH_PIPELINING_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_DISABLE_IDLE_RC6_FLUSH_LSB 0x0006
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_DISABLE_IDLE_RC6_FLUSH_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_LSB 0x0007
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_SYNC_FLIP_V_BLANK_WAIT_FOR_EVENT_POWER_DOWN_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_LSB 0x0009
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_DEBUG_FLOW_FOR_GO_0_ACKNOWLEDGE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_PSMI_PARSER_BLOCK_LSB 0x000b
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_PSMI_PARSER_BLOCK_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_LSB 0x000c
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_SEMAPHORE_WAIT_EVENT_IDLE_MESSAGE_DISABLE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_LSB 0x000d
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_IDLE_MESSAGE_OVERRIDE_CONTROL_SIZE 0x0002
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_LSB 0x000f
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_INHIBIT_RC6_ENTRY_ON_WAIT_FOR_SCANLINE_SIZE 0x0001
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_MASK_LSB 0x0010
#define RC_PSMI_CTRL_VECSUNIT3_MEDIA_MASK_SIZE 0x0010


/** PWRCTX_MAXCNT_VECSUNIT3 desc:
  */

#define PWRCTX_MAXCNT_VECSUNIT3_MEDIA_REG (0x001f8054U)

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
} PWRCTX_MAXCNT_VECSUNIT3_MEDIA_STRUCT;

#define PWRCTX_MAXCNT_VECSUNIT3_MEDIA_WIDTH 32
#define PWRCTX_MAXCNT_VECSUNIT3_MEDIA_DEFAULT 0x0000000aU
#define PWRCTX_MAXCNT_VECSUNIT3_MEDIA_ENDPOINT MEDIA

#define PWRCTX_MAXCNT_VECSUNIT3_MEDIA_IDLE_WAIT_TIME_LSB 0x0000
#define PWRCTX_MAXCNT_VECSUNIT3_MEDIA_IDLE_WAIT_TIME_SIZE 0x0014


/** RC1P_FREQ_VALUE_RPM_MEDIA desc:
  */

#define RC1P_FREQ_VALUE_RPM_MEDIA_MEDIA_REG (0x00380c14U)

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
} RC1P_FREQ_VALUE_RPM_MEDIA_MEDIA_STRUCT;

#define RC1P_FREQ_VALUE_RPM_MEDIA_MEDIA_WIDTH 32
#define RC1P_FREQ_VALUE_RPM_MEDIA_MEDIA_DEFAULT 0x00000000U
#define RC1P_FREQ_VALUE_RPM_MEDIA_MEDIA_ENDPOINT MEDIA

#define RC1P_FREQ_VALUE_RPM_MEDIA_MEDIA_RC1P_REQ_FREQ_LSB 0x0000
#define RC1P_FREQ_VALUE_RPM_MEDIA_MEDIA_RC1P_REQ_FREQ_SIZE 0x0009
#define RC1P_FREQ_VALUE_RPM_MEDIA_MEDIA_LOCK_LSB 0x001f
#define RC1P_FREQ_VALUE_RPM_MEDIA_MEDIA_LOCK_SIZE 0x0001


/** RC1P_FREQ_CHNG_EN_RPM_MEDIA desc:
  */

#define RC1P_FREQ_CHNG_EN_RPM_MEDIA_MEDIA_REG (0x00380c18U)

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
} RC1P_FREQ_CHNG_EN_RPM_MEDIA_MEDIA_STRUCT;

#define RC1P_FREQ_CHNG_EN_RPM_MEDIA_MEDIA_WIDTH 32
#define RC1P_FREQ_CHNG_EN_RPM_MEDIA_MEDIA_DEFAULT 0x00000000U
#define RC1P_FREQ_CHNG_EN_RPM_MEDIA_MEDIA_ENDPOINT MEDIA

#define RC1P_FREQ_CHNG_EN_RPM_MEDIA_MEDIA_RC1P_ENABLE_LSB 0x0000
#define RC1P_FREQ_CHNG_EN_RPM_MEDIA_MEDIA_RC1P_ENABLE_SIZE 0x0001


/** RC1P_BUSY_THRESH_RPM_MEDIA desc:
  */

#define RC1P_BUSY_THRESH_RPM_MEDIA_MEDIA_REG (0x00380c1cU)

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
} RC1P_BUSY_THRESH_RPM_MEDIA_MEDIA_STRUCT;

#define RC1P_BUSY_THRESH_RPM_MEDIA_MEDIA_WIDTH 32
#define RC1P_BUSY_THRESH_RPM_MEDIA_MEDIA_DEFAULT 0x00000000U
#define RC1P_BUSY_THRESH_RPM_MEDIA_MEDIA_ENDPOINT MEDIA

#define RC1P_BUSY_THRESH_RPM_MEDIA_MEDIA_RC1P_BUSY_HYST_COUNT_LSB 0x0000
#define RC1P_BUSY_THRESH_RPM_MEDIA_MEDIA_RC1P_BUSY_HYST_COUNT_SIZE 0x0010


/** CONFIG0_RPM_MEDIA desc:
  */

#define CONFIG0_RPM_MEDIA_MEDIA_REG      (0x00380d00U)

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
} CONFIG0_RPM_MEDIA_MEDIA_STRUCT;

#define CONFIG0_RPM_MEDIA_MEDIA_WIDTH 32
#define CONFIG0_RPM_MEDIA_MEDIA_DEFAULT 0x00000004U
#define CONFIG0_RPM_MEDIA_MEDIA_ENDPOINT MEDIA

#define CONFIG0_RPM_MEDIA_MEDIA_DIS_TSC_SYNC_LSB 0x0000
#define CONFIG0_RPM_MEDIA_MEDIA_DIS_TSC_SYNC_SIZE 0x0001
#define CONFIG0_RPM_MEDIA_MEDIA_CTC_SHIFT_PARAM_LSB 0x0001
#define CONFIG0_RPM_MEDIA_MEDIA_CTC_SHIFT_PARAM_SIZE 0x0002
#define CONFIG0_RPM_MEDIA_MEDIA_CRYSCLOCK_LSB 0x0003
#define CONFIG0_RPM_MEDIA_MEDIA_CRYSCLOCK_SIZE 0x0003
#define CONFIG0_RPM_MEDIA_MEDIA_PSMI_FUNC_EQ_LSB 0x0006
#define CONFIG0_RPM_MEDIA_MEDIA_PSMI_FUNC_EQ_SIZE 0x0001
#define CONFIG0_RPM_MEDIA_MEDIA_PLACEHOLDER_LSB 0x0007
#define CONFIG0_RPM_MEDIA_MEDIA_PLACEHOLDER_SIZE 0x0018
#define CONFIG0_RPM_MEDIA_MEDIA_LOCK_LSB 0x001f
#define CONFIG0_RPM_MEDIA_MEDIA_LOCK_SIZE 0x0001


/** DFD_RESTORE_CFG_MSB_RPM_MEDIA desc:
  */

#define DFD_RESTORE_CFG_MSB_RPM_MEDIA_MEDIA_REG (0x00380d38U)

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
} DFD_RESTORE_CFG_MSB_RPM_MEDIA_MEDIA_STRUCT;

#define DFD_RESTORE_CFG_MSB_RPM_MEDIA_MEDIA_WIDTH 32
#define DFD_RESTORE_CFG_MSB_RPM_MEDIA_MEDIA_DEFAULT 0x00000000U
#define DFD_RESTORE_CFG_MSB_RPM_MEDIA_MEDIA_ENDPOINT MEDIA

#define DFD_RESTORE_CFG_MSB_RPM_MEDIA_MEDIA_BASEADDRMSB_LSB 0x0000
#define DFD_RESTORE_CFG_MSB_RPM_MEDIA_MEDIA_BASEADDRMSB_SIZE 0x000b
#define DFD_RESTORE_CFG_MSB_RPM_MEDIA_MEDIA_LOCK_LSB 0x001f
#define DFD_RESTORE_CFG_MSB_RPM_MEDIA_MEDIA_LOCK_SIZE 0x0001


/** RC6CTXBASE_RPM_MEDIA_0 desc:
  */

#define RC6CTXBASE_RPM_MEDIA_0_MEDIA_REG (0x00380d48U)

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
} RC6CTXBASE_RPM_MEDIA_0_MEDIA_STRUCT;

#define RC6CTXBASE_RPM_MEDIA_0_MEDIA_WIDTH 32
#define RC6CTXBASE_RPM_MEDIA_0_MEDIA_DEFAULT 0x00000000U
#define RC6CTXBASE_RPM_MEDIA_0_MEDIA_ENDPOINT MEDIA

#define RC6CTXBASE_RPM_MEDIA_0_MEDIA_RC6MEMLOCK_LSB 0x0000
#define RC6CTXBASE_RPM_MEDIA_0_MEDIA_RC6MEMLOCK_SIZE 0x0001
#define RC6CTXBASE_RPM_MEDIA_0_MEDIA_RC6MEMBASE_LSB 0x000c
#define RC6CTXBASE_RPM_MEDIA_0_MEDIA_RC6MEMBASE_SIZE 0x0014


/** RC6CTXBASE_RPM_MEDIA_1 desc:
  */

#define RC6CTXBASE_RPM_MEDIA_1_MEDIA_REG (0x00380d4cU)

typedef union {
  struct {
    UINT32 rc6membase : 32; /**< RC6 Memory Base High */
                            /* Bits[31:0], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RC6CTXBASE_RPM_MEDIA_1_MEDIA_STRUCT;

#define RC6CTXBASE_RPM_MEDIA_1_MEDIA_WIDTH 32
#define RC6CTXBASE_RPM_MEDIA_1_MEDIA_DEFAULT 0x00000000U
#define RC6CTXBASE_RPM_MEDIA_1_MEDIA_ENDPOINT MEDIA

#define RC6CTXBASE_RPM_MEDIA_1_MEDIA_RC6MEMBASE_LSB 0x0000
#define RC6CTXBASE_RPM_MEDIA_1_MEDIA_RC6MEMBASE_SIZE 0x0020


/** MIRROR_GMD_ID_RPM_MEDIA desc:
  */

#define MIRROR_GMD_ID_RPM_MEDIA_MEDIA_REG (0x00380d8cU)

typedef union {
  struct {
    UINT32 arch_id : 32; /**< Architecture ID */
                            /* Bits[31:0], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MIRROR_GMD_ID_RPM_MEDIA_MEDIA_STRUCT;

#define MIRROR_GMD_ID_RPM_MEDIA_MEDIA_WIDTH 32
#define MIRROR_GMD_ID_RPM_MEDIA_MEDIA_DEFAULT 0x00000000U
#define MIRROR_GMD_ID_RPM_MEDIA_MEDIA_ENDPOINT MEDIA

#define MIRROR_GMD_ID_RPM_MEDIA_MEDIA_ARCH_ID_LSB 0x0000
#define MIRROR_GMD_ID_RPM_MEDIA_MEDIA_ARCH_ID_SIZE 0x0020


/** DRIVER_IP_FWAKE_ACK_RPM_MEDIA desc:
  */

#define DRIVER_IP_FWAKE_ACK_RPM_MEDIA_MEDIA_REG (0x00380dfcU)

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
} DRIVER_IP_FWAKE_ACK_RPM_MEDIA_MEDIA_STRUCT;

#define DRIVER_IP_FWAKE_ACK_RPM_MEDIA_MEDIA_WIDTH 32
#define DRIVER_IP_FWAKE_ACK_RPM_MEDIA_MEDIA_DEFAULT 0x00000000U
#define DRIVER_IP_FWAKE_ACK_RPM_MEDIA_MEDIA_ENDPOINT MEDIA

#define DRIVER_IP_FWAKE_ACK_RPM_MEDIA_MEDIA_ACK_LSB 0x0000
#define DRIVER_IP_FWAKE_ACK_RPM_MEDIA_MEDIA_ACK_SIZE 0x0010


/** FLAT_CCS_BASE_ADDR_GAMREQ_MEDIA desc:
  */

#define FLAT_CCS_BASE_ADDR_GAMREQ_MEDIA_MEDIA_REG (0x00384910U)

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
} FLAT_CCS_BASE_ADDR_GAMREQ_MEDIA_MEDIA_STRUCT;

#define FLAT_CCS_BASE_ADDR_GAMREQ_MEDIA_MEDIA_WIDTH 32
#define FLAT_CCS_BASE_ADDR_GAMREQ_MEDIA_MEDIA_DEFAULT 0x00000000U
#define FLAT_CCS_BASE_ADDR_GAMREQ_MEDIA_MEDIA_ENDPOINT MEDIA

#define FLAT_CCS_BASE_ADDR_GAMREQ_MEDIA_MEDIA_FLAT_CCS_EN_LSB 0x0000
#define FLAT_CCS_BASE_ADDR_GAMREQ_MEDIA_MEDIA_FLAT_CCS_EN_SIZE 0x0001
#define FLAT_CCS_BASE_ADDR_GAMREQ_MEDIA_MEDIA_FLAT_CCS_LOCK_LSB 0x0004
#define FLAT_CCS_BASE_ADDR_GAMREQ_MEDIA_MEDIA_FLAT_CCS_LOCK_SIZE 0x0001


/** FLATCCSBASEANDRANGE_LOWER_I_SAM desc:
  */

#define FLATCCSBASEANDRANGE_LOWER_I_SAM_MEDIA_REG (0x00388280U)

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
} FLATCCSBASEANDRANGE_LOWER_I_SAM_MEDIA_STRUCT;

#define FLATCCSBASEANDRANGE_LOWER_I_SAM_MEDIA_WIDTH 32
#define FLATCCSBASEANDRANGE_LOWER_I_SAM_MEDIA_DEFAULT 0x00000000U
#define FLATCCSBASEANDRANGE_LOWER_I_SAM_MEDIA_ENDPOINT MEDIA

#define FLATCCSBASEANDRANGE_LOWER_I_SAM_MEDIA_CCSENABLE_LSB 0x0000
#define FLATCCSBASEANDRANGE_LOWER_I_SAM_MEDIA_CCSENABLE_SIZE 0x0001
#define FLATCCSBASEANDRANGE_LOWER_I_SAM_MEDIA_HWVIEWOFFLATCCSBASEATCCLEVELLO_LSB 0x0006
#define FLATCCSBASEANDRANGE_LOWER_I_SAM_MEDIA_HWVIEWOFFLATCCSBASEATCCLEVELLO_SIZE 0x001a


/** FLATCCSBASEANDRANGE_UPPER_I_SAM desc:
  */

#define FLATCCSBASEANDRANGE_UPPER_I_SAM_MEDIA_REG (0x00388284U)

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
} FLATCCSBASEANDRANGE_UPPER_I_SAM_MEDIA_STRUCT;

#define FLATCCSBASEANDRANGE_UPPER_I_SAM_MEDIA_WIDTH 32
#define FLATCCSBASEANDRANGE_UPPER_I_SAM_MEDIA_DEFAULT 0x00000000U
#define FLATCCSBASEANDRANGE_UPPER_I_SAM_MEDIA_ENDPOINT MEDIA

#define FLATCCSBASEANDRANGE_UPPER_I_SAM_MEDIA_SWVIEWOFFLATCCSBASEHI_LSB 0x0000
#define FLATCCSBASEANDRANGE_UPPER_I_SAM_MEDIA_SWVIEWOFFLATCCSBASEHI_SIZE 0x000a
#define FLATCCSBASEANDRANGE_UPPER_I_SAM_MEDIA_FLATCCSIZE_LSB 0x000e
#define FLATCCSBASEANDRANGE_UPPER_I_SAM_MEDIA_FLATCCSIZE_SIZE 0x000e


/** CCCHKNREG1_I_SAM desc:
  */

#define CCCHKNREG1_I_SAM_MEDIA_REG       (0x003882a8U)

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
} CCCHKNREG1_I_SAM_MEDIA_STRUCT;

#define CCCHKNREG1_I_SAM_MEDIA_WIDTH 32
#define CCCHKNREG1_I_SAM_MEDIA_DEFAULT 0x00040000U
#define CCCHKNREG1_I_SAM_MEDIA_ENDPOINT MEDIA

#define CCCHKNREG1_I_SAM_MEDIA_SLOTHASHLOGICCHKN_LSB 0x0000
#define CCCHKNREG1_I_SAM_MEDIA_SLOTHASHLOGICCHKN_SIZE 0x0001
#define CCCHKNREG1_I_SAM_MEDIA_SETXORLOGICCHKN_LSB 0x0001
#define CCCHKNREG1_I_SAM_MEDIA_SETXORLOGICCHKN_SIZE 0x0001
#define CCCHKNREG1_I_SAM_MEDIA_HBMBOUNDCHECKDIS_LSB 0x0002
#define CCCHKNREG1_I_SAM_MEDIA_HBMBOUNDCHECKDIS_SIZE 0x0001
#define CCCHKNREG1_I_SAM_MEDIA_CCSCACHEPARITYINV_LSB 0x0003
#define CCCHKNREG1_I_SAM_MEDIA_CCSCACHEPARITYINV_SIZE 0x0001
#define CCCHKNREG1_I_SAM_MEDIA_READMISSPARITYINV_LSB 0x0004
#define CCCHKNREG1_I_SAM_MEDIA_READMISSPARITYINV_SIZE 0x0001
#define CCCHKNREG1_I_SAM_MEDIA_WRITEMISSPARITYINV_LSB 0x0005
#define CCCHKNREG1_I_SAM_MEDIA_WRITEMISSPARITYINV_SIZE 0x0001
#define CCCHKNREG1_I_SAM_MEDIA_SQCCPARITYINV_LSB 0x0006
#define CCCHKNREG1_I_SAM_MEDIA_SQCCPARITYINV_SIZE 0x0001
#define CCCHKNREG1_I_SAM_MEDIA_CCLNEPPARITYINV_LSB 0x0007
#define CCCHKNREG1_I_SAM_MEDIA_CCLNEPPARITYINV_SIZE 0x0001
#define CCCHKNREG1_I_SAM_MEDIA_PARITYCHECKDIS_LSB 0x0008
#define CCCHKNREG1_I_SAM_MEDIA_PARITYCHECKDIS_SIZE 0x0001
#define CCCHKNREG1_I_SAM_MEDIA__32BRDDISABLE_LSB 0x000e
#define CCCHKNREG1_I_SAM_MEDIA__32BRDDISABLE_SIZE 0x0001
#define CCCHKNREG1_I_SAM_MEDIA__32BWRDISABLE_LSB 0x000f
#define CCCHKNREG1_I_SAM_MEDIA__32BWRDISABLE_SIZE 0x0001
#define CCCHKNREG1_I_SAM_MEDIA_TDFLUSHCNTL_LSB 0x0010
#define CCCHKNREG1_I_SAM_MEDIA_TDFLUSHCNTL_SIZE 0x0001
#define CCCHKNREG1_I_SAM_MEDIA_L4CACNTLCSW_LSB 0x0011
#define CCCHKNREG1_I_SAM_MEDIA_L4CACNTLCSW_SIZE 0x0001
#define CCCHKNREG1_I_SAM_MEDIA_ENCOMPPERFFIX_LSB 0x0012
#define CCCHKNREG1_I_SAM_MEDIA_ENCOMPPERFFIX_SIZE 0x0001
#define CCCHKNREG1_I_SAM_MEDIA_DISOOOCCSRTN_LSB 0x0013
#define CCCHKNREG1_I_SAM_MEDIA_DISOOOCCSRTN_SIZE 0x0001
#define CCCHKNREG1_I_SAM_MEDIA_ENCCSPF_LSB 0x0016
#define CCCHKNREG1_I_SAM_MEDIA_ENCCSPF_SIZE 0x0001
#define CCCHKNREG1_I_SAM_MEDIA_L3CMPCTRL_LSB 0x0017
#define CCCHKNREG1_I_SAM_MEDIA_L3CMPCTRL_SIZE 0x0001
#define CCCHKNREG1_I_SAM_MEDIA_L2DYNCOMPCTRL_LSB 0x0018
#define CCCHKNREG1_I_SAM_MEDIA_L2DYNCOMPCTRL_SIZE 0x0001
#define CCCHKNREG1_I_SAM_MEDIA_COMPOVFDIS_LSB 0x0019
#define CCCHKNREG1_I_SAM_MEDIA_COMPOVFDIS_SIZE 0x0001
#define CCCHKNREG1_I_SAM_MEDIA_CCCHKNBITS_LSB 0x001a
#define CCCHKNREG1_I_SAM_MEDIA_CCCHKNBITS_SIZE 0x0006


/** CCLOCK_I_SAM desc:
  */

#define CCLOCK_I_SAM_MEDIA_REG           (0x003882acU)

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
} CCLOCK_I_SAM_MEDIA_STRUCT;

#define CCLOCK_I_SAM_MEDIA_WIDTH 32
#define CCLOCK_I_SAM_MEDIA_DEFAULT 0x00000000U
#define CCLOCK_I_SAM_MEDIA_ENDPOINT MEDIA

#define CCLOCK_I_SAM_MEDIA_CCCHKNREG1_LOCK_LSB 0x0000
#define CCLOCK_I_SAM_MEDIA_CCCHKNREG1_LOCK_SIZE 0x0001
#define CCLOCK_I_SAM_MEDIA_FLATCCSBASEANDRANGE_LOCK_LSB 0x0001
#define CCLOCK_I_SAM_MEDIA_FLATCCSBASEANDRANGE_LOCK_SIZE 0x0001
#define CCLOCK_I_SAM_MEDIA_CCLOCK_LOCK_LSB 0x001f
#define CCLOCK_I_SAM_MEDIA_CCLOCK_LOCK_SIZE 0x0001


/** SQCM_MCFG_GRP_MEDIA desc:
  */

#define SQCM_MCFG_GRP_MEDIA_MEDIA_REG    (0x00388724U)

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
} SQCM_MCFG_GRP_MEDIA_MEDIA_STRUCT;

#define SQCM_MCFG_GRP_MEDIA_MEDIA_WIDTH 32
#define SQCM_MCFG_GRP_MEDIA_MEDIA_DEFAULT 0x00000001U
#define SQCM_MCFG_GRP_MEDIA_MEDIA_ENDPOINT MEDIA

#define SQCM_MCFG_GRP_MEDIA_MEDIA_CMIARB_LSB 0x0000
#define SQCM_MCFG_GRP_MEDIA_MEDIA_CMIARB_SIZE 0x0001
#define SQCM_MCFG_GRP_MEDIA_MEDIA_ECORSVD_3_1_LSB 0x0001
#define SQCM_MCFG_GRP_MEDIA_MEDIA_ECORSVD_3_1_SIZE 0x0003
#define SQCM_MCFG_GRP_MEDIA_MEDIA_SQPI_LSB 0x0004
#define SQCM_MCFG_GRP_MEDIA_MEDIA_SQPI_SIZE 0x0001
#define SQCM_MCFG_GRP_MEDIA_MEDIA_ECORSVD_10_5_LSB 0x0005
#define SQCM_MCFG_GRP_MEDIA_MEDIA_ECORSVD_10_5_SIZE 0x0006
#define SQCM_MCFG_GRP_MEDIA_MEDIA_LOCK_LSB 0x000b
#define SQCM_MCFG_GRP_MEDIA_MEDIA_LOCK_SIZE 0x0001
#define SQCM_MCFG_GRP_MEDIA_MEDIA_ECORSVD_13_12_LSB 0x000c
#define SQCM_MCFG_GRP_MEDIA_MEDIA_ECORSVD_13_12_SIZE 0x0002
#define SQCM_MCFG_GRP_MEDIA_MEDIA_AES_BYPASS_DISABLE_LSB 0x000e
#define SQCM_MCFG_GRP_MEDIA_MEDIA_AES_BYPASS_DISABLE_SIZE 0x0001
#define SQCM_MCFG_GRP_MEDIA_MEDIA_ECORSVD_15_LSB 0x000f
#define SQCM_MCFG_GRP_MEDIA_MEDIA_ECORSVD_15_SIZE 0x0001
#define SQCM_MCFG_GRP_MEDIA_MEDIA_SQ_GDR_WD_EN_LSB 0x0010
#define SQCM_MCFG_GRP_MEDIA_MEDIA_SQ_GDR_WD_EN_SIZE 0x0001
#define SQCM_MCFG_GRP_MEDIA_MEDIA_SQDFV_LSB 0x0011
#define SQCM_MCFG_GRP_MEDIA_MEDIA_SQDFV_SIZE 0x0001
#define SQCM_MCFG_GRP_MEDIA_MEDIA_ECORSVD_29_18_LSB 0x0013
#define SQCM_MCFG_GRP_MEDIA_MEDIA_ECORSVD_29_18_SIZE 0x000b
#define SQCM_MCFG_GRP_MEDIA_MEDIA_ECORSVD_31_LSB 0x001f
#define SQCM_MCFG_GRP_MEDIA_MEDIA_ECORSVD_31_SIZE 0x0001


/** GSCPSMI_PWRCTXSAVE_MEDIAGSCPSMI desc:
  */

#define GSCPSMI_PWRCTXSAVE_MEDIAGSCPSMI_MEDIA_REG (0x00388800U)

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
} GSCPSMI_PWRCTXSAVE_MEDIAGSCPSMI_MEDIA_STRUCT;

#define GSCPSMI_PWRCTXSAVE_MEDIAGSCPSMI_MEDIA_WIDTH 32
#define GSCPSMI_PWRCTXSAVE_MEDIAGSCPSMI_MEDIA_DEFAULT 0x00000000U
#define GSCPSMI_PWRCTXSAVE_MEDIAGSCPSMI_MEDIA_ENDPOINT MEDIA

#define GSCPSMI_PWRCTXSAVE_MEDIAGSCPSMI_MEDIA_PWRCTX_SAVE_QWCREDITS_LSB 0x0000
#define GSCPSMI_PWRCTXSAVE_MEDIAGSCPSMI_MEDIA_PWRCTX_SAVE_QWCREDITS_SIZE 0x0009
#define GSCPSMI_PWRCTXSAVE_MEDIAGSCPSMI_MEDIA_PWRCTX_SAVE_REQ_LSB 0x0009
#define GSCPSMI_PWRCTXSAVE_MEDIAGSCPSMI_MEDIA_PWRCTX_SAVE_REQ_SIZE 0x0001
#define GSCPSMI_PWRCTXSAVE_MEDIAGSCPSMI_MEDIA_MB_LSB 0x0010
#define GSCPSMI_PWRCTXSAVE_MEDIAGSCPSMI_MEDIA_MB_SIZE 0x0010


/** GSCPSMI_CONTROL_0_MEDIAGSCPSMI desc:
  */

#define GSCPSMI_CONTROL_0_MEDIAGSCPSMI_MEDIA_REG (0x00388804U)

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
} GSCPSMI_CONTROL_0_MEDIAGSCPSMI_MEDIA_STRUCT;

#define GSCPSMI_CONTROL_0_MEDIAGSCPSMI_MEDIA_WIDTH 32
#define GSCPSMI_CONTROL_0_MEDIAGSCPSMI_MEDIA_DEFAULT 0x00000000U
#define GSCPSMI_CONTROL_0_MEDIAGSCPSMI_MEDIA_ENDPOINT MEDIA

#define GSCPSMI_CONTROL_0_MEDIAGSCPSMI_MEDIA_PSMI_EN_LSB 0x0000
#define GSCPSMI_CONTROL_0_MEDIAGSCPSMI_MEDIA_PSMI_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_MEDIAGSCPSMI_MEDIA_PSMI_PREALLOCATION_EN_LSB 0x0002
#define GSCPSMI_CONTROL_0_MEDIAGSCPSMI_MEDIA_PSMI_PREALLOCATION_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_MEDIAGSCPSMI_MEDIA_SW_END_OF_CAPTURE_LSB 0x0003
#define GSCPSMI_CONTROL_0_MEDIAGSCPSMI_MEDIA_SW_END_OF_CAPTURE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_MEDIAGSCPSMI_MEDIA_RESET_TO_BASE_LSB 0x0004
#define GSCPSMI_CONTROL_0_MEDIAGSCPSMI_MEDIA_RESET_TO_BASE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_MEDIAGSCPSMI_MEDIA_RESET_CL_COUNT_LSB 0x0005
#define GSCPSMI_CONTROL_0_MEDIAGSCPSMI_MEDIA_RESET_CL_COUNT_SIZE 0x0001
#define GSCPSMI_CONTROL_0_MEDIAGSCPSMI_MEDIA_HALT_AT_TOP_EN_LSB 0x0006
#define GSCPSMI_CONTROL_0_MEDIAGSCPSMI_MEDIA_HALT_AT_TOP_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_MEDIAGSCPSMI_MEDIA_BUFOVRFLW_INTR_EN_LSB 0x0007
#define GSCPSMI_CONTROL_0_MEDIAGSCPSMI_MEDIA_BUFOVRFLW_INTR_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_MEDIAGSCPSMI_MEDIA_N_8TH_INTR_EN_LSB 0x0008
#define GSCPSMI_CONTROL_0_MEDIAGSCPSMI_MEDIA_N_8TH_INTR_EN_SIZE 0x0008
#define GSCPSMI_CONTROL_0_MEDIAGSCPSMI_MEDIA_MASK_LSB 0x0010
#define GSCPSMI_CONTROL_0_MEDIAGSCPSMI_MEDIA_MASK_SIZE 0x0010


/** UNSLCGCTL9430 desc:
  */

#define UNSLCGCTL9430_MEDIA_REG          (0x00389430U)

typedef union {
  struct {
    UINT32 cg3ddismcr : 1; /**< MCRunit Clock Gating Disable */
                            /* Bits[0:0], default=0x00000001*/
    UINT32 cg3ddismisd : 1; /**< MISDunit Clock Gating Disable */
                            /* Bits[1:1], default=0x00000001*/
    UINT32 cg3ddismertxg : 1; /**< MERTXG Clock Gating Disable */
                            /* Bits[2:2], default=0x00000001*/
    UINT32 cg3ddismsqd : 1; /**< MSQDunit Clock Gating Disable */
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
    UINT32 cg3ddisoam : 1; /**< OAM Clock Gating Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 cg3ddislngp : 1; /**< LNGP Clock Gating Disable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 cg3ddislnep : 1; /**< LNEP Clock Gating Disable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 cg3ddiscfe : 1; /**< SCFE Clock Gating Disable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 cg3ddisrcpbpix : 1; /**< SRCPBPIX  Clock Gating Disable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[16:16] */
                            /* Bits[16:16], default=0*/
    UINT32 ramcgdis_rcc : 1; /**< RAMRCC Clock Gating Disable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 cg3ddisrcc : 1; /**< RCC Clock Gating Disable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 cg3ddisdap : 1; /**< DAP Clock Gating Disable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 cg3ddissarb : 1; /**< SARB Clock Gating Disable */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 rsvd_1 : 2; /**< Undefined - auto filled rsvd_[22:21] */
                            /* Bits[22:21], default=0*/
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
    UINT32 rsvd_2 : 2; /**< Undefined - auto filled rsvd_[31:30] */
                            /* Bits[31:30], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UNSLCGCTL9430_MEDIA_STRUCT;

#define UNSLCGCTL9430_MEDIA_WIDTH 32
#define UNSLCGCTL9430_MEDIA_DEFAULT 0x000007efU
#define UNSLCGCTL9430_MEDIA_ENDPOINT MEDIA

#define UNSLCGCTL9430_MEDIA_CG3DDISMCR_LSB 0x0000
#define UNSLCGCTL9430_MEDIA_CG3DDISMCR_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDISMISD_LSB 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDISMISD_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDISMERTXG_LSB 0x0002
#define UNSLCGCTL9430_MEDIA_CG3DDISMERTXG_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDISMSQD_LSB 0x0003
#define UNSLCGCTL9430_MEDIA_CG3DDISMSQD_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDISMERTG_LSB 0x0004
#define UNSLCGCTL9430_MEDIA_CG3DDISMERTG_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDISMBGF_LSB 0x0005
#define UNSLCGCTL9430_MEDIA_CG3DDISMBGF_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDISGPM_LSB 0x0006
#define UNSLCGCTL9430_MEDIA_CG3DDISGPM_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDIS_SPARE4_LSB 0x0007
#define UNSLCGCTL9430_MEDIA_CG3DDIS_SPARE4_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDIS_SPARE5_LSB 0x0008
#define UNSLCGCTL9430_MEDIA_CG3DDIS_SPARE5_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDIS_SPARE6_LSB 0x0009
#define UNSLCGCTL9430_MEDIA_CG3DDIS_SPARE6_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDISGCP_LSB 0x000a
#define UNSLCGCTL9430_MEDIA_CG3DDISGCP_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDISOAM_LSB 0x000b
#define UNSLCGCTL9430_MEDIA_CG3DDISOAM_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDISLNGP_LSB 0x000c
#define UNSLCGCTL9430_MEDIA_CG3DDISLNGP_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDISLNEP_LSB 0x000d
#define UNSLCGCTL9430_MEDIA_CG3DDISLNEP_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDISCFE_LSB 0x000e
#define UNSLCGCTL9430_MEDIA_CG3DDISCFE_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDISRCPBPIX_LSB 0x000f
#define UNSLCGCTL9430_MEDIA_CG3DDISRCPBPIX_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_RAMCGDIS_RCC_LSB 0x0011
#define UNSLCGCTL9430_MEDIA_RAMCGDIS_RCC_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDISRCC_LSB 0x0012
#define UNSLCGCTL9430_MEDIA_CG3DDISRCC_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDISDAP_LSB 0x0013
#define UNSLCGCTL9430_MEDIA_CG3DDISDAP_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDISSARB_LSB 0x0014
#define UNSLCGCTL9430_MEDIA_CG3DDISSARB_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDISLNIB_LSB 0x0017
#define UNSLCGCTL9430_MEDIA_CG3DDISLNIB_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDISLNIC_LSB 0x0018
#define UNSLCGCTL9430_MEDIA_CG3DDISLNIC_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDISRCPBARB_LSB 0x0019
#define UNSLCGCTL9430_MEDIA_CG3DDISRCPBARB_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDISRCPBLAT_LSB 0x001a
#define UNSLCGCTL9430_MEDIA_CG3DDISRCPBLAT_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDISMERTGART_LSB 0x001b
#define UNSLCGCTL9430_MEDIA_CG3DDISMERTGART_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDISOAAL3_LSB 0x001c
#define UNSLCGCTL9430_MEDIA_CG3DDISOAAL3_SIZE 0x0001
#define UNSLCGCTL9430_MEDIA_CG3DDISOAAL2_LSB 0x001d
#define UNSLCGCTL9430_MEDIA_CG3DDISOAAL2_SIZE 0x0001


/** UNSLCGCTL9434 desc:
  */

#define UNSLCGCTL9434_MEDIA_REG          (0x00389434U)

typedef union {
  struct {
    UINT32 cg3ddisbcsfe : 1; /**< BCS FE Clock Gating Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cg3ddisbcsbe : 1; /**< BCS BE Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[03:02] */
                            /* Bits[3:2], default=0*/
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
    UINT32 rsvd_1 : 5; /**< Undefined - auto filled rsvd_[13:09] */
                            /* Bits[13:9], default=0*/
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
    UINT32 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[19:19] */
                            /* Bits[19:19], default=0*/
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
    UINT32 rsvd_3 : 1; /**< Undefined - auto filled rsvd_[29:29] */
                            /* Bits[29:29], default=0*/
    UINT32 cg3ddismbc : 1; /**< MBCunit Clock Gating Disable */
                            /* Bits[30:30], default=0x00000001*/
    UINT32 cg3ddisgti : 1; /**< GTIunit Clock Gating Disable */
                            /* Bits[31:31], default=0x00000001*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UNSLCGCTL9434_MEDIA_STRUCT;

#define UNSLCGCTL9434_MEDIA_WIDTH 32
#define UNSLCGCTL9434_MEDIA_DEFAULT 0xd0140000U
#define UNSLCGCTL9434_MEDIA_ENDPOINT MEDIA

#define UNSLCGCTL9434_MEDIA_CG3DDISBCSFE_LSB 0x0000
#define UNSLCGCTL9434_MEDIA_CG3DDISBCSFE_SIZE 0x0001
#define UNSLCGCTL9434_MEDIA_CG3DDISBCSBE_LSB 0x0001
#define UNSLCGCTL9434_MEDIA_CG3DDISBCSBE_SIZE 0x0001
#define UNSLCGCTL9434_MEDIA_CG3DDISNOA_LSB 0x0004
#define UNSLCGCTL9434_MEDIA_CG3DDISNOA_SIZE 0x0001
#define UNSLCGCTL9434_MEDIA_CG3DDISOACS_LSB 0x0005
#define UNSLCGCTL9434_MEDIA_CG3DDISOACS_SIZE 0x0001
#define UNSLCGCTL9434_MEDIA_CG3DDISSUC_LSB 0x0006
#define UNSLCGCTL9434_MEDIA_CG3DDISSUC_SIZE 0x0001
#define UNSLCGCTL9434_MEDIA_CG3DDISVFGR_LSB 0x0007
#define UNSLCGCTL9434_MEDIA_CG3DDISVFGR_SIZE 0x0001
#define UNSLCGCTL9434_MEDIA_CG3DDISBSC_LSB 0x0008
#define UNSLCGCTL9434_MEDIA_CG3DDISBSC_SIZE 0x0001
#define UNSLCGCTL9434_MEDIA_CG3DDISPOCSBE_LSB 0x000e
#define UNSLCGCTL9434_MEDIA_CG3DDISPOCSBE_SIZE 0x0001
#define UNSLCGCTL9434_MEDIA_CG3DDISPOCSFE_LSB 0x000f
#define UNSLCGCTL9434_MEDIA_CG3DDISPOCSFE_SIZE 0x0001
#define UNSLCGCTL9434_MEDIA_CG3DDISPOCS_LSB 0x0010
#define UNSLCGCTL9434_MEDIA_CG3DDISPOCS_SIZE 0x0001
#define UNSLCGCTL9434_MEDIA_CG3DDISCFEG_LSB 0x0011
#define UNSLCGCTL9434_MEDIA_CG3DDISCFEG_SIZE 0x0001
#define UNSLCGCTL9434_MEDIA_CG3DDISGAMTLBMISARB_LSB 0x0012
#define UNSLCGCTL9434_MEDIA_CG3DDISGAMTLBMISARB_SIZE 0x0001
#define UNSLCGCTL9434_MEDIA_CG3DDISGAMTLBHITARB_LSB 0x0014
#define UNSLCGCTL9434_MEDIA_CG3DDISGAMTLBHITARB_SIZE 0x0001
#define UNSLCGCTL9434_MEDIA_CG3DDISBCS_LSB 0x0015
#define UNSLCGCTL9434_MEDIA_CG3DDISBCS_SIZE 0x0001
#define UNSLCGCTL9434_MEDIA_CG3DDISBD_LSB 0x0016
#define UNSLCGCTL9434_MEDIA_CG3DDISBD_SIZE 0x0001
#define UNSLCGCTL9434_MEDIA_CG3DDISBF_LSB 0x0017
#define UNSLCGCTL9434_MEDIA_CG3DDISBF_SIZE 0x0001
#define UNSLCGCTL9434_MEDIA_CG3DDISBLB_LSB 0x0018
#define UNSLCGCTL9434_MEDIA_CG3DDISBLB_SIZE 0x0001
#define UNSLCGCTL9434_MEDIA_CG3DDISBLS_LSB 0x0019
#define UNSLCGCTL9434_MEDIA_CG3DDISBLS_SIZE 0x0001
#define UNSLCGCTL9434_MEDIA_CG3DDISVFG_LSB 0x001a
#define UNSLCGCTL9434_MEDIA_CG3DDISVFG_SIZE 0x0001
#define UNSLCGCTL9434_MEDIA_CG3DDISDT_LSB 0x001b
#define UNSLCGCTL9434_MEDIA_CG3DDISDT_SIZE 0x0001
#define UNSLCGCTL9434_MEDIA_CG3DDISGAB_LSB 0x001c
#define UNSLCGCTL9434_MEDIA_CG3DDISGAB_SIZE 0x0001
#define UNSLCGCTL9434_MEDIA_CG3DDISMBC_LSB 0x001e
#define UNSLCGCTL9434_MEDIA_CG3DDISMBC_SIZE 0x0001
#define UNSLCGCTL9434_MEDIA_CG3DDISGTI_LSB 0x001f
#define UNSLCGCTL9434_MEDIA_CG3DDISGTI_SIZE 0x0001


/** UNSLCGCTL9438 desc:
  */

#define UNSLCGCTL9438_MEDIA_REG          (0x00389438U)

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
    UINT32 cg3ddisgamxbe : 1; /**< GAMXBE Clock Gating Disable */
                            /* Bits[24:24], default=0x00000001*/
    UINT32 cg3ddisgamxbl : 1; /**< GAMXBL Clock Gating Disable */
                            /* Bits[25:25], default=0x00000001*/
    UINT32 cg3ddislne : 1; /**< LNEUNIT Clock Gating Disable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 rsvd_1 : 5; /**< Undefined - auto filled rsvd_[31:27] */
                            /* Bits[31:27], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UNSLCGCTL9438_MEDIA_STRUCT;

#define UNSLCGCTL9438_MEDIA_WIDTH 32
#define UNSLCGCTL9438_MEDIA_DEFAULT 0x03000000U
#define UNSLCGCTL9438_MEDIA_ENDPOINT MEDIA

#define UNSLCGCTL9438_MEDIA_CG3DDISCCS0_LSB 0x0000
#define UNSLCGCTL9438_MEDIA_CG3DDISCCS0_SIZE 0x0001
#define UNSLCGCTL9438_MEDIA_CG3DDISCCS1_LSB 0x0001
#define UNSLCGCTL9438_MEDIA_CG3DDISCCS1_SIZE 0x0001
#define UNSLCGCTL9438_MEDIA_CG3DDISCCS2_LSB 0x0002
#define UNSLCGCTL9438_MEDIA_CG3DDISCCS2_SIZE 0x0001
#define UNSLCGCTL9438_MEDIA_CG3DDISCCS3_LSB 0x0003
#define UNSLCGCTL9438_MEDIA_CG3DDISCCS3_SIZE 0x0001
#define UNSLCGCTL9438_MEDIA_CG3DDISCCS0FE_LSB 0x0004
#define UNSLCGCTL9438_MEDIA_CG3DDISCCS0FE_SIZE 0x0001
#define UNSLCGCTL9438_MEDIA_CG3DDISCCS1FE_LSB 0x0005
#define UNSLCGCTL9438_MEDIA_CG3DDISCCS1FE_SIZE 0x0001
#define UNSLCGCTL9438_MEDIA_CG3DDISCCS2FE_LSB 0x0006
#define UNSLCGCTL9438_MEDIA_CG3DDISCCS2FE_SIZE 0x0001
#define UNSLCGCTL9438_MEDIA_CG3DDISCCS3FE_LSB 0x0007
#define UNSLCGCTL9438_MEDIA_CG3DDISCCS3FE_SIZE 0x0001
#define UNSLCGCTL9438_MEDIA_CG3DDISGUC_LSB 0x0008
#define UNSLCGCTL9438_MEDIA_CG3DDISGUC_SIZE 0x0001
#define UNSLCGCTL9438_MEDIA_CG3DDISCCS0BE_LSB 0x0014
#define UNSLCGCTL9438_MEDIA_CG3DDISCCS0BE_SIZE 0x0001
#define UNSLCGCTL9438_MEDIA_CG3DDISCCS1BE_LSB 0x0015
#define UNSLCGCTL9438_MEDIA_CG3DDISCCS1BE_SIZE 0x0001
#define UNSLCGCTL9438_MEDIA_CG3DDISCCS2BE_LSB 0x0016
#define UNSLCGCTL9438_MEDIA_CG3DDISCCS2BE_SIZE 0x0001
#define UNSLCGCTL9438_MEDIA_CG3DDISCCS3BE_LSB 0x0017
#define UNSLCGCTL9438_MEDIA_CG3DDISCCS3BE_SIZE 0x0001
#define UNSLCGCTL9438_MEDIA_CG3DDISGAMXBE_LSB 0x0018
#define UNSLCGCTL9438_MEDIA_CG3DDISGAMXBE_SIZE 0x0001
#define UNSLCGCTL9438_MEDIA_CG3DDISGAMXBL_LSB 0x0019
#define UNSLCGCTL9438_MEDIA_CG3DDISGAMXBL_SIZE 0x0001
#define UNSLCGCTL9438_MEDIA_CG3DDISLNE_LSB 0x001a
#define UNSLCGCTL9438_MEDIA_CG3DDISLNE_SIZE 0x0001


/** UNSLCGCTL9440 desc:
  */

#define UNSLCGCTL9440_MEDIA_REG          (0x00389440U)

typedef union {
  struct {
    UINT32 cg3ddis_hwm : 1; /**< hwmunit Clock Gating Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cg3ddisramdft : 1; /**< ramdft Clock Gating Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 cg3ddisgamreqstrm : 1; /**< gamreqstrm Clock Gating Disable */
                            /* Bits[2:2], default=0x00000001*/
    UINT32 cg3ddisl3_cr : 1; /**< L3_CR Clock Gating Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 cg3ddisgamdati : 1; /**< gamdati Clock Gating Disable */
                            /* Bits[4:4], default=0x00000001*/
    UINT32 cg3ddisgamwkrs : 1; /**< gamwkrs Clock Gating Disable */
                            /* Bits[5:5], default=0x00000001*/
    UINT32 cg3ddisgamtlbvdbox1 : 1; /**< gamtlbvdbox1 Clock Gating Disable */
                            /* Bits[6:6], default=0x00000001*/
    UINT32 cg3ddisgamcmdi : 1; /**< gamcmdi Clock Gating Disable */
                            /* Bits[7:7], default=0x00000001*/
    UINT32 cg3ddisgamctrl : 1; /**< gamctrl Clock Gating Disable */
                            /* Bits[8:8], default=0x00000001*/
    UINT32 rsvd_0 : 3; /**< Undefined - auto filled rsvd_[11:09] */
                            /* Bits[11:9], default=0*/
    UINT32 cg3ddisgamstlb : 1; /**< gamstlb Clock Gating Disable */
                            /* Bits[12:12], default=0x00000001*/
    UINT32 cg3ddisgamtrtlb : 1; /**< gamtrtlb Clock Gating Disable */
                            /* Bits[13:13], default=0x00000001*/
    UINT32 rsvd_1 : 2; /**< Undefined - auto filled rsvd_[15:14] */
                            /* Bits[15:14], default=0*/
    UINT32 cgacgdis_guc : 1; /**< GUC Arch Clock Gating Disable */
                            /* Bits[16:16], default=0x00000001*/
    UINT32 cg3ddisgamtlbvdbox0 : 1; /**< gamtlbvdbox0 Clock Gating Disable */
                            /* Bits[17:17], default=0x00000001*/
    UINT32 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[18:18] */
                            /* Bits[18:18], default=0*/
    UINT32 cg3ddis_spare2 : 1; /**< SPARE Clock Gating Disable1 */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 cg3ddis_spare3 : 1; /**< SPARE Clock Gating Disable2 */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 cg3ddisgamtlbvdbox2 : 1; /**< gamtlbvdbox2 Clock Gating Disable */
                            /* Bits[21:21], default=0x00000001*/
    UINT32 cg3ddisgamtlbvdbox7 : 1; /**< gamtlbvdbox7 Clock Gating Disable */
                            /* Bits[22:22], default=0x00000001*/
    UINT32 cg3ddisgamtlbvdbox4 : 1; /**< gamtlbvdbox4 Clock Gating Disable */
                            /* Bits[23:23], default=0x00000001*/
    UINT32 cg3ddisgamtlbvdbox3 : 1; /**< gamtlbvdbox3 Clock Gating Disable */
                            /* Bits[24:24], default=0x00000001*/
    UINT32 cg3ddisgamdrtn : 1; /**< gamdrtnunit Clock Gating Disable */
                            /* Bits[25:25], default=0x00000001*/
    UINT32 cg3ddisgamtlbvdbox6 : 1; /**< gamtlbvdbox6 Clock Gating Disable */
                            /* Bits[26:26], default=0x00000001*/
    UINT32 cg3ddisgamtlbvdbox5 : 1; /**< gamtlbvdbox5 Clock Gating Disable */
                            /* Bits[27:27], default=0x00000001*/
    UINT32 rsvd_3 : 1; /**< Undefined - auto filled rsvd_[28:28] */
                            /* Bits[28:28], default=0*/
    UINT32 cg3ddisrxbar : 1; /**< RXBAR Clock Gating Disable */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 ramcgdis_rxbar : 1; /**< RAM_RXBAR Clock Gating Disable */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 rsvd_4 : 1; /**< Undefined - auto filled rsvd_[31:31] */
                            /* Bits[31:31], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UNSLCGCTL9440_MEDIA_STRUCT;

#define UNSLCGCTL9440_MEDIA_WIDTH 32
#define UNSLCGCTL9440_MEDIA_DEFAULT 0x0fe331f4U
#define UNSLCGCTL9440_MEDIA_ENDPOINT MEDIA

#define UNSLCGCTL9440_MEDIA_CG3DDIS_HWM_LSB 0x0000
#define UNSLCGCTL9440_MEDIA_CG3DDIS_HWM_SIZE 0x0001
#define UNSLCGCTL9440_MEDIA_CG3DDISRAMDFT_LSB 0x0001
#define UNSLCGCTL9440_MEDIA_CG3DDISRAMDFT_SIZE 0x0001
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMREQSTRM_LSB 0x0002
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMREQSTRM_SIZE 0x0001
#define UNSLCGCTL9440_MEDIA_CG3DDISL3_CR_LSB 0x0003
#define UNSLCGCTL9440_MEDIA_CG3DDISL3_CR_SIZE 0x0001
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMDATI_LSB 0x0004
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMDATI_SIZE 0x0001
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMWKRS_LSB 0x0005
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMWKRS_SIZE 0x0001
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMTLBVDBOX1_LSB 0x0006
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMTLBVDBOX1_SIZE 0x0001
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMCMDI_LSB 0x0007
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMCMDI_SIZE 0x0001
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMCTRL_LSB 0x0008
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMCTRL_SIZE 0x0001
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMSTLB_LSB 0x000c
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMSTLB_SIZE 0x0001
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMTRTLB_LSB 0x000d
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMTRTLB_SIZE 0x0001
#define UNSLCGCTL9440_MEDIA_CGACGDIS_GUC_LSB 0x0010
#define UNSLCGCTL9440_MEDIA_CGACGDIS_GUC_SIZE 0x0001
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMTLBVDBOX0_LSB 0x0011
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMTLBVDBOX0_SIZE 0x0001
#define UNSLCGCTL9440_MEDIA_CG3DDIS_SPARE2_LSB 0x0013
#define UNSLCGCTL9440_MEDIA_CG3DDIS_SPARE2_SIZE 0x0001
#define UNSLCGCTL9440_MEDIA_CG3DDIS_SPARE3_LSB 0x0014
#define UNSLCGCTL9440_MEDIA_CG3DDIS_SPARE3_SIZE 0x0001
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMTLBVDBOX2_LSB 0x0015
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMTLBVDBOX2_SIZE 0x0001
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMTLBVDBOX7_LSB 0x0016
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMTLBVDBOX7_SIZE 0x0001
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMTLBVDBOX4_LSB 0x0017
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMTLBVDBOX4_SIZE 0x0001
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMTLBVDBOX3_LSB 0x0018
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMTLBVDBOX3_SIZE 0x0001
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMDRTN_LSB 0x0019
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMDRTN_SIZE 0x0001
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMTLBVDBOX6_LSB 0x001a
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMTLBVDBOX6_SIZE 0x0001
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMTLBVDBOX5_LSB 0x001b
#define UNSLCGCTL9440_MEDIA_CG3DDISGAMTLBVDBOX5_SIZE 0x0001
#define UNSLCGCTL9440_MEDIA_CG3DDISRXBAR_LSB 0x001d
#define UNSLCGCTL9440_MEDIA_CG3DDISRXBAR_SIZE 0x0001
#define UNSLCGCTL9440_MEDIA_RAMCGDIS_RXBAR_LSB 0x001e
#define UNSLCGCTL9440_MEDIA_RAMCGDIS_RXBAR_SIZE 0x0001


/** UNSLCGCTL9444 desc:
  */

#define UNSLCGCTL9444_MEDIA_REG          (0x00389444U)

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
    UINT32 cg3ddisgamtlbvebox0 : 1; /**< gamtlbvebox0 Clock Gating Disable */
                            /* Bits[16:16], default=0x00000001*/
    UINT32 cg3ddisgamtlbvebox1 : 1; /**< gamtlbvebox1 Clock Gating Disable */
                            /* Bits[17:17], default=0x00000001*/
    UINT32 cg3ddisgamtlbvebox2 : 1; /**< gamtlbvebox2 Clock Gating Disable */
                            /* Bits[18:18], default=0x00000001*/
    UINT32 cg3ddisgamtlbvebox3 : 1; /**< gamtlbvebox3 Clock Gating Disable */
                            /* Bits[19:19], default=0x00000001*/
    UINT32 cg3ddisgamtlbmert : 1; /**< gamtlbmert Clock Gating Disable */
                            /* Bits[20:20], default=0x00000001*/
    UINT32 cg3ddisgamccs : 1; /**< gamccs Clock Gating Disable */
                            /* Bits[21:21], default=0x00000001*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[22:22] */
                            /* Bits[22:22], default=0*/
    UINT32 cg3ddisgsci_gsc : 1; /**< gscigsc Clock Gating Disable */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 cg3ddisgsci : 1; /**< gsci Clock Gating Disable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 cg3ddisgagsc : 1; /**< gagsc Clock Gating Disable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 cg3ddisgsccsbe : 1; /**< gsccsbe Clock Gating Disable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 cg3ddisgsccsfe : 1; /**< gsccsfe Clock Gating Disable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 cg3ddisgsccs_cs : 1; /**< gsccs Clock Gating Disable */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 rsvd_2 : 3; /**< Undefined - auto filled rsvd_[31:29] */
                            /* Bits[31:29], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UNSLCGCTL9444_MEDIA_STRUCT;

#define UNSLCGCTL9444_MEDIA_WIDTH 32
#define UNSLCGCTL9444_MEDIA_DEFAULT 0x003f008cU
#define UNSLCGCTL9444_MEDIA_ENDPOINT MEDIA

#define UNSLCGCTL9444_MEDIA_CG3DDISLSN_LSB 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISLSN_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISRCU_LSB 0x0002
#define UNSLCGCTL9444_MEDIA_CG3DDISRCU_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDIS_KCR_LSB 0x0003
#define UNSLCGCTL9444_MEDIA_CG3DDIS_KCR_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISCSC_LSB 0x0004
#define UNSLCGCTL9444_MEDIA_CG3DDISCSC_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISLC_LSB 0x0005
#define UNSLCGCTL9444_MEDIA_CG3DDISLC_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISLD_LSB 0x0006
#define UNSLCGCTL9444_MEDIA_CG3DDISLD_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISPSMI_LSB 0x0007
#define UNSLCGCTL9444_MEDIA_CG3DDISPSMI_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISSMCR_LSB 0x0008
#define UNSLCGCTL9444_MEDIA_CG3DDISSMCR_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISLTCDT_LSB 0x0009
#define UNSLCGCTL9444_MEDIA_CG3DDISLTCDT_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISLTCDD_LSB 0x000a
#define UNSLCGCTL9444_MEDIA_CG3DDISLTCDD_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISLTCC_LSB 0x000b
#define UNSLCGCTL9444_MEDIA_CG3DDISLTCC_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISLSQD_LSB 0x000c
#define UNSLCGCTL9444_MEDIA_CG3DDISLSQD_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISLSQC_LSB 0x000d
#define UNSLCGCTL9444_MEDIA_CG3DDISLSQC_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISLBI_LSB 0x000e
#define UNSLCGCTL9444_MEDIA_CG3DDISLBI_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISLBCF_LSB 0x000f
#define UNSLCGCTL9444_MEDIA_CG3DDISLBCF_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISGAMTLBVEBOX0_LSB 0x0010
#define UNSLCGCTL9444_MEDIA_CG3DDISGAMTLBVEBOX0_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISGAMTLBVEBOX1_LSB 0x0011
#define UNSLCGCTL9444_MEDIA_CG3DDISGAMTLBVEBOX1_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISGAMTLBVEBOX2_LSB 0x0012
#define UNSLCGCTL9444_MEDIA_CG3DDISGAMTLBVEBOX2_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISGAMTLBVEBOX3_LSB 0x0013
#define UNSLCGCTL9444_MEDIA_CG3DDISGAMTLBVEBOX3_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISGAMTLBMERT_LSB 0x0014
#define UNSLCGCTL9444_MEDIA_CG3DDISGAMTLBMERT_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISGAMCCS_LSB 0x0015
#define UNSLCGCTL9444_MEDIA_CG3DDISGAMCCS_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISGSCI_GSC_LSB 0x0017
#define UNSLCGCTL9444_MEDIA_CG3DDISGSCI_GSC_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISGSCI_LSB 0x0018
#define UNSLCGCTL9444_MEDIA_CG3DDISGSCI_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISGAGSC_LSB 0x0019
#define UNSLCGCTL9444_MEDIA_CG3DDISGAGSC_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISGSCCSBE_LSB 0x001a
#define UNSLCGCTL9444_MEDIA_CG3DDISGSCCSBE_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISGSCCSFE_LSB 0x001b
#define UNSLCGCTL9444_MEDIA_CG3DDISGSCCSFE_SIZE 0x0001
#define UNSLCGCTL9444_MEDIA_CG3DDISGSCCS_CS_LSB 0x001c
#define UNSLCGCTL9444_MEDIA_CG3DDISGSCCS_CS_SIZE 0x0001


/** INFCGCTL9560 desc:
  */

#define INFCGCTL9560_MEDIA_REG           (0x00389560U)

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
    UINT32 cg3ddismertf : 1; /**< MERTFUNIT Clock Gating Disable */
                            /* Bits[10:10], default=0x00000001*/
    UINT32 cg3ddismertxf : 1; /**< MERTXFUNIT Clock Gating Disable */
                            /* Bits[11:11], default=0x00000001*/
    UINT32 cg3ddisramdft : 1; /**< Ramdft Clock Gating Disable */
                            /* Bits[12:12], default=0x00000001*/
    UINT32 cg3ddisgdtu : 1; /**< GDTU Clock Gating Disable */
                            /* Bits[13:13], default=0x00000001*/
    UINT32 cg3ddismdat : 1; /**< mdat Clock Gating Disable */
                            /* Bits[14:14], default=0x00000001*/
    UINT32 cg3ddismxbc2u : 1; /**< mxbc2u Clock Gating Disable */
                            /* Bits[15:15], default=0x00000001*/
    UINT32 cg3ddismxbu2c : 1; /**< mxbu2c Clock Gating Disable */
                            /* Bits[16:16], default=0x00000001*/
    UINT32 cg3ddis_mcxl : 1; /**< mcxlunit Clock Gating Disable */
                            /* Bits[17:17], default=0x00000001*/
    UINT32 cg3ddis_infpsmi : 1; /**< INF psmi Clock Gating Disable */
                            /* Bits[18:18], default=0x00000001*/
    UINT32 rsvd_0 : 13; /**< Undefined - auto filled rsvd_[31:19] */
                            /* Bits[31:19], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} INFCGCTL9560_MEDIA_STRUCT;

#define INFCGCTL9560_MEDIA_WIDTH 32
#define INFCGCTL9560_MEDIA_DEFAULT 0x0007ffffU
#define INFCGCTL9560_MEDIA_ENDPOINT MEDIA

#define INFCGCTL9560_MEDIA_CG3DDISRMCR_LSB 0x0000
#define INFCGCTL9560_MEDIA_CG3DDISRMCR_SIZE 0x0001
#define INFCGCTL9560_MEDIA_CG3DDISMGSR_LSB 0x0001
#define INFCGCTL9560_MEDIA_CG3DDISMGSR_SIZE 0x0001
#define INFCGCTL9560_MEDIA_CG3DDISMDRB_LSB 0x0002
#define INFCGCTL9560_MEDIA_CG3DDISMDRB_SIZE 0x0001
#define INFCGCTL9560_MEDIA_CG3DDISCGPSF_LSB 0x0003
#define INFCGCTL9560_MEDIA_CG3DDISCGPSF_SIZE 0x0001
#define INFCGCTL9560_MEDIA_CG3DDISMBGFUC_LSB 0x0004
#define INFCGCTL9560_MEDIA_CG3DDISMBGFUC_SIZE 0x0001
#define INFCGCTL9560_MEDIA_CG3DDISRPM_LSB 0x0005
#define INFCGCTL9560_MEDIA_CG3DDISRPM_SIZE 0x0001
#define INFCGCTL9560_MEDIA_CG3DDISGTFS_LSB 0x0006
#define INFCGCTL9560_MEDIA_CG3DDISGTFS_SIZE 0x0001
#define INFCGCTL9560_MEDIA_CG3DDISCPMA_LSB 0x0007
#define INFCGCTL9560_MEDIA_CG3DDISCPMA_SIZE 0x0001
#define INFCGCTL9560_MEDIA_CG3DDIS_SF_GU_LSB 0x0008
#define INFCGCTL9560_MEDIA_CG3DDIS_SF_GU_SIZE 0x0001
#define INFCGCTL9560_MEDIA_CG3DDIS_COHPSMI_LSB 0x0009
#define INFCGCTL9560_MEDIA_CG3DDIS_COHPSMI_SIZE 0x0001
#define INFCGCTL9560_MEDIA_CG3DDISMERTF_LSB 0x000a
#define INFCGCTL9560_MEDIA_CG3DDISMERTF_SIZE 0x0001
#define INFCGCTL9560_MEDIA_CG3DDISMERTXF_LSB 0x000b
#define INFCGCTL9560_MEDIA_CG3DDISMERTXF_SIZE 0x0001
#define INFCGCTL9560_MEDIA_CG3DDISRAMDFT_LSB 0x000c
#define INFCGCTL9560_MEDIA_CG3DDISRAMDFT_SIZE 0x0001
#define INFCGCTL9560_MEDIA_CG3DDISGDTU_LSB 0x000d
#define INFCGCTL9560_MEDIA_CG3DDISGDTU_SIZE 0x0001
#define INFCGCTL9560_MEDIA_CG3DDISMDAT_LSB 0x000e
#define INFCGCTL9560_MEDIA_CG3DDISMDAT_SIZE 0x0001
#define INFCGCTL9560_MEDIA_CG3DDISMXBC2U_LSB 0x000f
#define INFCGCTL9560_MEDIA_CG3DDISMXBC2U_SIZE 0x0001
#define INFCGCTL9560_MEDIA_CG3DDISMXBU2C_LSB 0x0010
#define INFCGCTL9560_MEDIA_CG3DDISMXBU2C_SIZE 0x0001
#define INFCGCTL9560_MEDIA_CG3DDIS_MCXL_LSB 0x0011
#define INFCGCTL9560_MEDIA_CG3DDIS_MCXL_SIZE 0x0001
#define INFCGCTL9560_MEDIA_CG3DDIS_INFPSMI_LSB 0x0012
#define INFCGCTL9560_MEDIA_CG3DDIS_INFPSMI_SIZE 0x0001


/** GCP_CLKDIV_CONTROL0 desc:
  */

#define GCP_CLKDIV_CONTROL0_MEDIA_REG    (0x00389590U)

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
} GCP_CLKDIV_CONTROL0_MEDIA_STRUCT;

#define GCP_CLKDIV_CONTROL0_MEDIA_WIDTH 32
#define GCP_CLKDIV_CONTROL0_MEDIA_DEFAULT 0x044e2709U
#define GCP_CLKDIV_CONTROL0_MEDIA_ENDPOINT MEDIA

#define GCP_CLKDIV_CONTROL0_MEDIA_CEP_RGM_ENABLE_LSB 0x0000
#define GCP_CLKDIV_CONTROL0_MEDIA_CEP_RGM_ENABLE_SIZE 0x0001
#define GCP_CLKDIV_CONTROL0_MEDIA_CEP_RGM_DER_N_LSB 0x0001
#define GCP_CLKDIV_CONTROL0_MEDIA_CEP_RGM_DER_N_SIZE 0x0003
#define GCP_CLKDIV_CONTROL0_MEDIA_CEP_RGM_DOWN_DURATION_INTERVAL_LSB 0x0004
#define GCP_CLKDIV_CONTROL0_MEDIA_CEP_RGM_DOWN_DURATION_INTERVAL_SIZE 0x0002
#define GCP_CLKDIV_CONTROL0_MEDIA_CEP_RGM_UP_DURATION_INTERVAL_LSB 0x0006
#define GCP_CLKDIV_CONTROL0_MEDIA_CEP_RGM_UP_DURATION_INTERVAL_SIZE 0x0003
#define GCP_CLKDIV_CONTROL0_MEDIA_FASTTHR_RGM_ENABLE_LSB 0x0009
#define GCP_CLKDIV_CONTROL0_MEDIA_FASTTHR_RGM_ENABLE_SIZE 0x0001
#define GCP_CLKDIV_CONTROL0_MEDIA_FASTTHR_RGM_DER_N_LSB 0x000a
#define GCP_CLKDIV_CONTROL0_MEDIA_FASTTHR_RGM_DER_N_SIZE 0x0003
#define GCP_CLKDIV_CONTROL0_MEDIA_FASTTHR_RGM_DOWN_DURATION_INTERVAL_LSB 0x000d
#define GCP_CLKDIV_CONTROL0_MEDIA_FASTTHR_RGM_DOWN_DURATION_INTERVAL_SIZE 0x0002
#define GCP_CLKDIV_CONTROL0_MEDIA_FASTTHR_RGM_UP_DURATION_INTERVAL_LSB 0x000f
#define GCP_CLKDIV_CONTROL0_MEDIA_FASTTHR_RGM_UP_DURATION_INTERVAL_SIZE 0x0003
#define GCP_CLKDIV_CONTROL0_MEDIA_GV_RGM_ENABLE_LSB 0x0012
#define GCP_CLKDIV_CONTROL0_MEDIA_GV_RGM_ENABLE_SIZE 0x0001
#define GCP_CLKDIV_CONTROL0_MEDIA_GV_RGM_DER_N_LSB 0x0013
#define GCP_CLKDIV_CONTROL0_MEDIA_GV_RGM_DER_N_SIZE 0x0003
#define GCP_CLKDIV_CONTROL0_MEDIA_GV_RGM_DOWN_DURATION_INTERVAL_LSB 0x0016
#define GCP_CLKDIV_CONTROL0_MEDIA_GV_RGM_DOWN_DURATION_INTERVAL_SIZE 0x0002
#define GCP_CLKDIV_CONTROL0_MEDIA_GV_RGM_UP_DURATION_INTERVAL_LSB 0x0018
#define GCP_CLKDIV_CONTROL0_MEDIA_GV_RGM_UP_DURATION_INTERVAL_SIZE 0x0003
#define GCP_CLKDIV_CONTROL0_MEDIA_CLKDIV_LOCKBIT_LSB 0x001f
#define GCP_CLKDIV_CONTROL0_MEDIA_CLKDIV_LOCKBIT_SIZE 0x0001

/** GCP_CLKDIV_CONTROL1_MEDIA desc:
  */

#define GCP_CLKDIV_CONTROL1_MEDIA_REG    (0x00389594U)

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
} GCP_CLKDIV_CONTROL1_MEDIA_STRUCT;

#define GCP_CLKDIV_CONTROL1_MEDIA_WIDTH 32
#define GCP_CLKDIV_CONTROL1_MEDIA_DEFAULT 0x00000113U
#define GCP_CLKDIV_CONTROL1_MEDIA_ENDPOINT MEDIA

#define GCP_CLKDIV_CONTROL1_MEDIA_RCP1_RGM_ENABLE_LSB 0x0000
#define GCP_CLKDIV_CONTROL1_MEDIA_RCP1_RGM_ENABLE_SIZE 0x0001
#define GCP_CLKDIV_CONTROL1_MEDIA_RCP1_RGM_DER_N_LSB 0x0001
#define GCP_CLKDIV_CONTROL1_MEDIA_RCP1_RGM_DER_N_SIZE 0x0003
#define GCP_CLKDIV_CONTROL1_MEDIA_RCP1_RGM_DOWN_DURATION_INTERVAL_LSB 0x0004
#define GCP_CLKDIV_CONTROL1_MEDIA_RCP1_RGM_DOWN_DURATION_INTERVAL_SIZE 0x0002
#define GCP_CLKDIV_CONTROL1_MEDIA_RCP1_RGM_UP_DURATION_INTERVAL_LSB 0x0006
#define GCP_CLKDIV_CONTROL1_MEDIA_RCP1_RGM_UP_DURATION_INTERVAL_SIZE 0x0003
#define GCP_CLKDIV_CONTROL1_MEDIA_CLKDIV_LOCKBIT_LSB 0x001f
#define GCP_CLKDIV_CONTROL1_MEDIA_CLKDIV_LOCKBIT_SIZE 0x0001


/** RP_FREQ_NORMAL_GPM_GRP_MEDIA desc:
  */

#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_REG (0x0038a008U)

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
} RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_STRUCT;

#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_WIDTH 32
#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_DEFAULT 0x00000000U
#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_ENDPOINT MEDIA

#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_SLICE_UNSLICE_THROTTLE_POLICY_LSB 0x0000
#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_SLICE_UNSLICE_THROTTLE_POLICY_SIZE 0x0004
#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_RPFREQ_SLICE_SQUSH_HINT_LSB 0x0004
#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_RPFREQ_SLICE_SQUSH_HINT_SIZE 0x0001
#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_RPFREQ_UNSLICE_SQUSH_HINT_LSB 0x0005
#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_RPFREQ_UNSLICE_SQUSH_HINT_SIZE 0x0001
#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_RPFREQ_SLICE_CHANGE_HINT_LSB 0x0006
#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_RPFREQ_SLICE_CHANGE_HINT_SIZE 0x0001
#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_RPFREQ_UNSLICE_CHANGE_HINT_LSB 0x0007
#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_RPFREQ_UNSLICE_CHANGE_HINT_SIZE 0x0001
#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_RPFREQ_FABZRIC_CHANGE_HINT_LSB 0x0008
#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_RPFREQ_FABZRIC_CHANGE_HINT_SIZE 0x0001
#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_RPFREQ_NOT_USED_1_LSB 0x0009
#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_RPFREQ_NOT_USED_1_SIZE 0x0004
#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_MEDIA_FREQ_RATIO_LSB 0x000d
#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_MEDIA_FREQ_RATIO_SIZE 0x0001
#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_SLICE_RATIO_LSB 0x000e
#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_SLICE_RATIO_SIZE 0x0009
#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_UNSLICE_RATIO_LSB 0x0017
#define RP_FREQ_NORMAL_GPM_GRP_MEDIA_MEDIA_UNSLICE_RATIO_SIZE 0x0009


/** RP_CTRL_GPM_GRP_MEDIA desc:
  */

#define RP_CTRL_GPM_GRP_MEDIA_MEDIA_REG  (0x0038a024U)

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
} RP_CTRL_GPM_GRP_MEDIA_MEDIA_STRUCT;

#define RP_CTRL_GPM_GRP_MEDIA_MEDIA_WIDTH 32
#define RP_CTRL_GPM_GRP_MEDIA_MEDIA_DEFAULT 0x00000000U
#define RP_CTRL_GPM_GRP_MEDIA_MEDIA_ENDPOINT MEDIA

#define RP_CTRL_GPM_GRP_MEDIA_MEDIA_RP_SW_MODE_SEL_LSB 0x0009
#define RP_CTRL_GPM_GRP_MEDIA_MEDIA_RP_SW_MODE_SEL_SIZE 0x0002
#define RP_CTRL_GPM_GRP_MEDIA_MEDIA_MEDIA_VIDEOTURBO_EN_LSB 0x000b
#define RP_CTRL_GPM_GRP_MEDIA_MEDIA_MEDIA_VIDEOTURBO_EN_SIZE 0x0001


/** RC_CTRL1_GPM_GRP_MEDIA desc:
  */

#define RC_CTRL1_GPM_GRP_MEDIA_MEDIA_REG (0x0038a094U)

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
} RC_CTRL1_GPM_GRP_MEDIA_MEDIA_STRUCT;

#define RC_CTRL1_GPM_GRP_MEDIA_MEDIA_WIDTH 32
#define RC_CTRL1_GPM_GRP_MEDIA_MEDIA_DEFAULT 0x00000000U
#define RC_CTRL1_GPM_GRP_MEDIA_MEDIA_ENDPOINT MEDIA

#define RC_CTRL1_GPM_GRP_MEDIA_MEDIA_RC_SW_STATE_REQ_LSB 0x0010
#define RC_CTRL1_GPM_GRP_MEDIA_MEDIA_RC_SW_STATE_REQ_SIZE 0x0003


/** MISC_CTRL0_GPM_GRP_MEDIA desc:
  */

#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_REG (0x0038a180U)

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
} MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_STRUCT;

#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_WIDTH 32
#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_DEFAULT 0x05200054U
#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_ENDPOINT MEDIA

#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_PSMI_GAL_CNTR_THRESH_LSB 0x0000
#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_PSMI_GAL_CNTR_THRESH_SIZE 0x0010
#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_GACFG_DONE_LSB 0x0013
#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_GACFG_DONE_SIZE 0x0001
#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_NOA_SEL_LSB 0x0014
#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_NOA_SEL_SIZE 0x0001
#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_RC6_ALL_IDLE_EN_LSB 0x0015
#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_RC6_ALL_IDLE_EN_SIZE 0x0001
#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_RC_SW_STATE_FLR_DEFAULT_LSB 0x0016
#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_RC_SW_STATE_FLR_DEFAULT_SIZE 0x0003
#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_CPD_GAM_GO_EN_LSB 0x0019
#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_CPD_GAM_GO_EN_SIZE 0x0001
#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_RC_FIFOBLOCK_TYPE_LSB 0x001a
#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_RC_FIFOBLOCK_TYPE_SIZE 0x0001
#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_OVERRIDE_COREACTIVE_LSB 0x001b
#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_OVERRIDE_COREACTIVE_SIZE 0x0001
#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_CPD_CORE_RW_EN_LSB 0x001c
#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_CPD_CORE_RW_EN_SIZE 0x0001
#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_CPD_OVERRIDE_IA_AS_GVL_LSB 0x001e
#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_CPD_OVERRIDE_IA_AS_GVL_SIZE 0x0001
#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_LOCK_LSB 0x001f
#define MISC_CTRL0_GPM_GRP_MEDIA_MEDIA_LOCK_SIZE 0x0001


/** FORCE_WAKE_GPM_GRP_MEDIA desc:
  */

#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REG (0x0038a188U)

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
} FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_STRUCT;

#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_WIDTH 32
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_DEFAULT 0x00000000U
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_ENDPOINT MEDIA

#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_0_LSB 0x0000
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_0_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_1_LSB 0x0001
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_1_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_2_LSB 0x0002
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_2_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_3_LSB 0x0003
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_3_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_4_LSB 0x0004
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_4_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_5_LSB 0x0005
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_5_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_6_LSB 0x0006
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_6_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_7_LSB 0x0007
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_7_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_8_LSB 0x0008
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_8_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_9_LSB 0x0009
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_9_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_10_LSB 0x000a
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_10_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_11_LSB 0x000b
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_11_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_12_LSB 0x000c
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_12_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_13_LSB 0x000d
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_13_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_14_LSB 0x000e
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_14_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_15_LSB 0x000f
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_REQ_15_SIZE 0x0001
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_MASK_EN_LSB 0x0010
#define FORCE_WAKE_GPM_GRP_MEDIA_MEDIA_MASK_EN_SIZE 0x0010


/** ECO_BUSRST_GPM_GRP_MEDIA desc:
  */

#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_REG (0x0038a194U)

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
} ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_STRUCT;

#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_WIDTH 32
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_DEFAULT 0x00000000U
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_ENDPOINT MEDIA

#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_SUBWELLS_POWERGATE_CHKNMODE_LSB 0x0000
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_SUBWELLS_POWERGATE_CHKNMODE_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_CS_BUSYACK_POWERGATE_DISABLE_CHKNMODE_LSB 0x0001
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_CS_BUSYACK_POWERGATE_DISABLE_CHKNMODE_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_ECORSVD_LSB 0x0002
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_ECORSVD_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_IGNORE_VCR_MEDIA_WAKE_REQ_LSB 0x0003
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_IGNORE_VCR_MEDIA_WAKE_REQ_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_RCRP_COMP_2129979_BUGFIX_MASK_LSB 0x0004
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_RCRP_COMP_2129979_BUGFIX_MASK_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_GACFG_FENCE_C6STATUS_WRITE_LSB 0x0005
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_GACFG_FENCE_C6STATUS_WRITE_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_GAM_GPM_BUSY_LEGACY_LSB 0x0006
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_GAM_GPM_BUSY_LEGACY_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_CPD_BLOCK_MGSR_DISABLE_LSB 0x0007
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_CPD_BLOCK_MGSR_DISABLE_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_CPD_BYPASS_GO_MSG_STRMS_LSB 0x0008
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_CPD_BYPASS_GO_MSG_STRMS_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_POCS_DISABLE_LSB 0x0009
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_POCS_DISABLE_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_AXF_ISO_ON_FLR_DISABLE_LSB 0x000a
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_AXF_ISO_ON_FLR_DISABLE_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_OA_CNT_PUSH_DISABLE_LSB 0x000b
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_OA_CNT_PUSH_DISABLE_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_PSMI_RC6_LITE_ENABLE_LSB 0x000c
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_PSMI_RC6_LITE_ENABLE_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_ENABLE_PMFLUSH_FOR_RST_LSB 0x000d
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_ENABLE_PMFLUSH_FOR_RST_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_GSC_CTX_RSTR_NEEDED_LSB 0x000e
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_GSC_CTX_RSTR_NEEDED_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_KCR_UNSL_CTX_CHKN_LSB 0x000f
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_KCR_UNSL_CTX_CHKN_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_WAIT_MEM_WIPE_POST_FLR_CHKN_LSB 0x0010
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_WAIT_MEM_WIPE_POST_FLR_CHKN_SIZE 0x0001
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_ECO_BUSRST_LSB 0x0011
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_ECO_BUSRST_SIZE 0x000e
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_LOCK_LSB 0x001f
#define ECO_BUSRST_GPM_GRP_MEDIA_MEDIA_LOCK_SIZE 0x0001




/** GSCPSMI_PWRCTXSAVE_MEDIA_SCMI0 desc:
  */

#define GSCPSMI_PWRCTXSAVE_MEDIA_SCMI0_MEDIA_REG (0x0038b080U)

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
} GSCPSMI_PWRCTXSAVE_MEDIA_SCMI0_MEDIA_STRUCT;

#define GSCPSMI_PWRCTXSAVE_MEDIA_SCMI0_MEDIA_WIDTH 32
#define GSCPSMI_PWRCTXSAVE_MEDIA_SCMI0_MEDIA_DEFAULT 0x00000000U
#define GSCPSMI_PWRCTXSAVE_MEDIA_SCMI0_MEDIA_ENDPOINT MEDIA

#define GSCPSMI_PWRCTXSAVE_MEDIA_SCMI0_MEDIA_PWRCTX_SAVE_QWCREDITS_LSB 0x0000
#define GSCPSMI_PWRCTXSAVE_MEDIA_SCMI0_MEDIA_PWRCTX_SAVE_QWCREDITS_SIZE 0x0009
#define GSCPSMI_PWRCTXSAVE_MEDIA_SCMI0_MEDIA_PWRCTX_SAVE_REQ_LSB 0x0009
#define GSCPSMI_PWRCTXSAVE_MEDIA_SCMI0_MEDIA_PWRCTX_SAVE_REQ_SIZE 0x0001
#define GSCPSMI_PWRCTXSAVE_MEDIA_SCMI0_MEDIA_MB_LSB 0x0010
#define GSCPSMI_PWRCTXSAVE_MEDIA_SCMI0_MEDIA_MB_SIZE 0x0010


/** GSCPSMI_CONTROL_0_MEDIA_SCMI0 desc:
  */

#define GSCPSMI_CONTROL_0_MEDIA_SCMI0_MEDIA_REG (0x0038b084U)

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
} GSCPSMI_CONTROL_0_MEDIA_SCMI0_MEDIA_STRUCT;

#define GSCPSMI_CONTROL_0_MEDIA_SCMI0_MEDIA_WIDTH 32
#define GSCPSMI_CONTROL_0_MEDIA_SCMI0_MEDIA_DEFAULT 0x00000000U
#define GSCPSMI_CONTROL_0_MEDIA_SCMI0_MEDIA_ENDPOINT MEDIA

#define GSCPSMI_CONTROL_0_MEDIA_SCMI0_MEDIA_PSMI_EN_LSB 0x0000
#define GSCPSMI_CONTROL_0_MEDIA_SCMI0_MEDIA_PSMI_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_MEDIA_SCMI0_MEDIA_PSMI_PREALLOCATION_EN_LSB 0x0002
#define GSCPSMI_CONTROL_0_MEDIA_SCMI0_MEDIA_PSMI_PREALLOCATION_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_MEDIA_SCMI0_MEDIA_SW_END_OF_CAPTURE_LSB 0x0003
#define GSCPSMI_CONTROL_0_MEDIA_SCMI0_MEDIA_SW_END_OF_CAPTURE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_MEDIA_SCMI0_MEDIA_RESET_TO_BASE_LSB 0x0004
#define GSCPSMI_CONTROL_0_MEDIA_SCMI0_MEDIA_RESET_TO_BASE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_MEDIA_SCMI0_MEDIA_RESET_CL_COUNT_LSB 0x0005
#define GSCPSMI_CONTROL_0_MEDIA_SCMI0_MEDIA_RESET_CL_COUNT_SIZE 0x0001
#define GSCPSMI_CONTROL_0_MEDIA_SCMI0_MEDIA_HALT_AT_TOP_EN_LSB 0x0006
#define GSCPSMI_CONTROL_0_MEDIA_SCMI0_MEDIA_HALT_AT_TOP_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_MEDIA_SCMI0_MEDIA_BUFOVRFLW_INTR_EN_LSB 0x0007
#define GSCPSMI_CONTROL_0_MEDIA_SCMI0_MEDIA_BUFOVRFLW_INTR_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_MEDIA_SCMI0_MEDIA_N_8TH_INTR_EN_LSB 0x0008
#define GSCPSMI_CONTROL_0_MEDIA_SCMI0_MEDIA_N_8TH_INTR_EN_SIZE 0x0008
#define GSCPSMI_CONTROL_0_MEDIA_SCMI0_MEDIA_MASK_LSB 0x0010
#define GSCPSMI_CONTROL_0_MEDIA_SCMI0_MEDIA_MASK_SIZE 0x0010




/** L3BANKLOCK_LBCF_MEDIA desc:
  */

#define L3BANKLOCK_LBCF_MEDIA_MEDIA_REG  (0x0038b660U)

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
} L3BANKLOCK_LBCF_MEDIA_MEDIA_STRUCT;

#define L3BANKLOCK_LBCF_MEDIA_MEDIA_WIDTH 32
#define L3BANKLOCK_LBCF_MEDIA_MEDIA_DEFAULT 0x00000000U
#define L3BANKLOCK_LBCF_MEDIA_MEDIA_ENDPOINT MEDIA

#define L3BANKLOCK_LBCF_MEDIA_MEDIA_L3CHMD_LOCK_LSB 0x0001
#define L3BANKLOCK_LBCF_MEDIA_MEDIA_L3CHMD_LOCK_SIZE 0x0001
#define L3BANKLOCK_LBCF_MEDIA_MEDIA_LTCDREG_LOCK_LSB 0x0003
#define L3BANKLOCK_LBCF_MEDIA_MEDIA_LTCDREG_LOCK_SIZE 0x0001
#define L3BANKLOCK_LBCF_MEDIA_MEDIA_L3SCRATCH3_LOCK_LSB 0x0006
#define L3BANKLOCK_LBCF_MEDIA_MEDIA_L3SCRATCH3_LOCK_SIZE 0x0001
#define L3BANKLOCK_LBCF_MEDIA_MEDIA_L3SQCREG2_LOCK_LSB 0x0007
#define L3BANKLOCK_LBCF_MEDIA_MEDIA_L3SQCREG2_LOCK_SIZE 0x0001
#define L3BANKLOCK_LBCF_MEDIA_MEDIA_LTCDREG2_LOCK_LSB 0x0008
#define L3BANKLOCK_LBCF_MEDIA_MEDIA_LTCDREG2_LOCK_SIZE 0x0001
#define L3BANKLOCK_LBCF_MEDIA_MEDIA_L3SQCREG3_LOCK_LSB 0x000c
#define L3BANKLOCK_LBCF_MEDIA_MEDIA_L3SQCREG3_LOCK_SIZE 0x0001
#define L3BANKLOCK_LBCF_MEDIA_MEDIA_L3SCRUB_LOCK_LSB 0x000f
#define L3BANKLOCK_LBCF_MEDIA_MEDIA_L3SCRUB_LOCK_SIZE 0x0001
#define L3BANKLOCK_LBCF_MEDIA_MEDIA_LBCFLOCKMSGREG_LOCK_LSB 0x0010
#define L3BANKLOCK_LBCF_MEDIA_MEDIA_LBCFLOCKMSGREG_LOCK_SIZE 0x0001
#define L3BANKLOCK_LBCF_MEDIA_MEDIA_L3BANKLOCK_LOCK_LSB 0x001f
#define L3BANKLOCK_LBCF_MEDIA_MEDIA_L3BANKLOCK_LOCK_SIZE 0x0001


/** GSCPSMI_PWRCTXSAVE_MEDIA_IMFSCMI0 desc:
  */

#define GSCPSMI_PWRCTXSAVE_MEDIA_IMFSCMI0_MEDIA_REG (0x0038b900U)

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
} GSCPSMI_PWRCTXSAVE_MEDIA_IMFSCMI0_MEDIA_STRUCT;

#define GSCPSMI_PWRCTXSAVE_MEDIA_IMFSCMI0_MEDIA_WIDTH 32
#define GSCPSMI_PWRCTXSAVE_MEDIA_IMFSCMI0_MEDIA_DEFAULT 0x00000000U
#define GSCPSMI_PWRCTXSAVE_MEDIA_IMFSCMI0_MEDIA_ENDPOINT MEDIA

#define GSCPSMI_PWRCTXSAVE_MEDIA_IMFSCMI0_MEDIA_PWRCTX_SAVE_QWCREDITS_LSB 0x0000
#define GSCPSMI_PWRCTXSAVE_MEDIA_IMFSCMI0_MEDIA_PWRCTX_SAVE_QWCREDITS_SIZE 0x0009
#define GSCPSMI_PWRCTXSAVE_MEDIA_IMFSCMI0_MEDIA_PWRCTX_SAVE_REQ_LSB 0x0009
#define GSCPSMI_PWRCTXSAVE_MEDIA_IMFSCMI0_MEDIA_PWRCTX_SAVE_REQ_SIZE 0x0001
#define GSCPSMI_PWRCTXSAVE_MEDIA_IMFSCMI0_MEDIA_MB_LSB 0x0010
#define GSCPSMI_PWRCTXSAVE_MEDIA_IMFSCMI0_MEDIA_MB_SIZE 0x0010


/** GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0 desc:
  */

#define GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0_MEDIA_REG (0x0038b904U)

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
} GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0_MEDIA_STRUCT;

#define GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0_MEDIA_WIDTH 32
#define GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0_MEDIA_DEFAULT 0x00000000U
#define GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0_MEDIA_ENDPOINT MEDIA

#define GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0_MEDIA_PSMI_EN_LSB 0x0000
#define GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0_MEDIA_PSMI_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0_MEDIA_PSMI_PREALLOCATION_EN_LSB 0x0002
#define GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0_MEDIA_PSMI_PREALLOCATION_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0_MEDIA_SW_END_OF_CAPTURE_LSB 0x0003
#define GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0_MEDIA_SW_END_OF_CAPTURE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0_MEDIA_RESET_TO_BASE_LSB 0x0004
#define GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0_MEDIA_RESET_TO_BASE_SIZE 0x0001
#define GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0_MEDIA_RESET_CL_COUNT_LSB 0x0005
#define GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0_MEDIA_RESET_CL_COUNT_SIZE 0x0001
#define GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0_MEDIA_HALT_AT_TOP_EN_LSB 0x0006
#define GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0_MEDIA_HALT_AT_TOP_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0_MEDIA_BUFOVRFLW_INTR_EN_LSB 0x0007
#define GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0_MEDIA_BUFOVRFLW_INTR_EN_SIZE 0x0001
#define GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0_MEDIA_N_8TH_INTR_EN_LSB 0x0008
#define GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0_MEDIA_N_8TH_INTR_EN_SIZE 0x0008
#define GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0_MEDIA_MASK_LSB 0x0010
#define GSCPSMI_CONTROL_0_MEDIA_IMFSCMI0_MEDIA_MASK_SIZE 0x0010


/** GUC_WOPCM_SIZE_GUCM_REG desc:
  */

#define GUC_WOPCM_SIZE_GUCM_REG_MEDIA_REG (0x0038c050U)

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
} GUC_WOPCM_SIZE_GUCM_REG_MEDIA_STRUCT;

#define GUC_WOPCM_SIZE_GUCM_REG_MEDIA_WIDTH 32
#define GUC_WOPCM_SIZE_GUCM_REG_MEDIA_DEFAULT 0x00100000U
#define GUC_WOPCM_SIZE_GUCM_REG_MEDIA_ENDPOINT MEDIA

#define GUC_WOPCM_SIZE_GUCM_REG_MEDIA_LOCKED_LSB 0x0000
#define GUC_WOPCM_SIZE_GUCM_REG_MEDIA_LOCKED_SIZE 0x0001
#define GUC_WOPCM_SIZE_GUCM_REG_MEDIA_GUC_ALLOCATED_WOPCM_SIZE_LSB 0x000c
#define GUC_WOPCM_SIZE_GUCM_REG_MEDIA_GUC_ALLOCATED_WOPCM_SIZE_SIZE 0x000e


/** DMA_GUC_WOPCM_OFFSET_GUCM_REG desc:
  */

#define DMA_GUC_WOPCM_OFFSET_GUCM_REG_MEDIA_REG (0x0038c340U)

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
} DMA_GUC_WOPCM_OFFSET_GUCM_REG_MEDIA_STRUCT;

#define DMA_GUC_WOPCM_OFFSET_GUCM_REG_MEDIA_WIDTH 32
#define DMA_GUC_WOPCM_OFFSET_GUCM_REG_MEDIA_DEFAULT 0x00000000U
#define DMA_GUC_WOPCM_OFFSET_GUCM_REG_MEDIA_ENDPOINT MEDIA

#define DMA_GUC_WOPCM_OFFSET_GUCM_REG_MEDIA_OFFSET_VALID_LSB 0x0000
#define DMA_GUC_WOPCM_OFFSET_GUCM_REG_MEDIA_OFFSET_VALID_SIZE 0x0001
#define DMA_GUC_WOPCM_OFFSET_GUCM_REG_MEDIA_HUC_UKERNEL_LOADING_AGENT_LSB 0x0001
#define DMA_GUC_WOPCM_OFFSET_GUCM_REG_MEDIA_HUC_UKERNEL_LOADING_AGENT_SIZE 0x0001
#define DMA_GUC_WOPCM_OFFSET_GUCM_REG_MEDIA_GUC_WOPCM_OFFSET_LSB 0x000e
#define DMA_GUC_WOPCM_OFFSET_GUCM_REG_MEDIA_GUC_WOPCM_OFFSET_SIZE 0x0012


/** GUC_PM_MAXCNT_GUCM_REG desc:
  */

#define GUC_PM_MAXCNT_GUCM_REG_MEDIA_REG (0x0038c3e4U)

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
} GUC_PM_MAXCNT_GUCM_REG_MEDIA_STRUCT;

#define GUC_PM_MAXCNT_GUCM_REG_MEDIA_WIDTH 32
#define GUC_PM_MAXCNT_GUCM_REG_MEDIA_DEFAULT 0x00000040U
#define GUC_PM_MAXCNT_GUCM_REG_MEDIA_ENDPOINT MEDIA

#define GUC_PM_MAXCNT_GUCM_REG_MEDIA_IDLE_WAIT_TIME_LSB 0x0000
#define GUC_PM_MAXCNT_GUCM_REG_MEDIA_IDLE_WAIT_TIME_SIZE 0x0014
#define GUC_PM_MAXCNT_GUCM_REG_MEDIA_GUC_POWER_MANAGEMENT_PARTICIPATION_IN_IDLE_FLOW_LSB 0x001f
#define GUC_PM_MAXCNT_GUCM_REG_MEDIA_GUC_POWER_MANAGEMENT_PARTICIPATION_IN_IDLE_FLOW_SIZE 0x0001


/** GAMCTRLLOCK_GAMCTRL_MEDIA desc:
  */

#define GAMCTRLLOCK_GAMCTRL_MEDIA_MEDIA_REG (0x0038cec0U)

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
} GAMCTRLLOCK_GAMCTRL_MEDIA_MEDIA_STRUCT;

#define GAMCTRLLOCK_GAMCTRL_MEDIA_MEDIA_WIDTH 32
#define GAMCTRLLOCK_GAMCTRL_MEDIA_MEDIA_DEFAULT 0x00000000U
#define GAMCTRLLOCK_GAMCTRL_MEDIA_MEDIA_ENDPOINT MEDIA

#define GAMCTRLLOCK_GAMCTRL_MEDIA_MEDIA_LOCK_REG_CF08_LSB 0x0000
#define GAMCTRLLOCK_GAMCTRL_MEDIA_MEDIA_LOCK_REG_CF08_SIZE 0x0001
#define GAMCTRLLOCK_GAMCTRL_MEDIA_MEDIA_LOCK_REG_CF54_LSB 0x0001
#define GAMCTRLLOCK_GAMCTRL_MEDIA_MEDIA_LOCK_REG_CF54_SIZE 0x0001
#define GAMCTRLLOCK_GAMCTRL_MEDIA_MEDIA_REG_RSV_LOCK0_LSB 0x0002
#define GAMCTRLLOCK_GAMCTRL_MEDIA_MEDIA_REG_RSV_LOCK0_SIZE 0x0001
#define GAMCTRLLOCK_GAMCTRL_MEDIA_MEDIA_REG_RSV_LOCK1_LSB 0x0003
#define GAMCTRLLOCK_GAMCTRL_MEDIA_MEDIA_REG_RSV_LOCK1_SIZE 0x0001
#define GAMCTRLLOCK_GAMCTRL_MEDIA_MEDIA_GAMCTRLLOCK_L_LSB 0x001f
#define GAMCTRLLOCK_GAMCTRL_MEDIA_MEDIA_GAMCTRLLOCK_L_SIZE 0x0001


/** ECOCHK_GAMCTRL_MEDIA desc:
  */

#define ECOCHK_GAMCTRL_MEDIA_MEDIA_REG   (0x0038cf08U)

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
} ECOCHK_GAMCTRL_MEDIA_MEDIA_STRUCT;

#define ECOCHK_GAMCTRL_MEDIA_MEDIA_WIDTH 32
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DEFAULT 0x00040000U
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_ENDPOINT MEDIA

#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_HDC_INV_LSB 0x0000
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_HDC_INV_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_EN_ATOMIC_RD_AD_LSB 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_EN_ATOMIC_RD_AD_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_KCR_INT_LSB 0x0002
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_KCR_INT_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_CTRL_ARB_FIX_LSB 0x0003
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_CTRL_ARB_FIX_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_CTRL_FLW_DELAY_LSB 0x0004
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_CTRL_FLW_DELAY_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_ERR_RT_PTR_SETUP_LSB 0x0005
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_ERR_RT_PTR_SETUP_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_FLT_DROP_ON_RST_LSB 0x0006
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_FLT_DROP_ON_RST_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_MERT_SKIP_LSB 0x0007
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_MERT_SKIP_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_BONUS10_7_LSB 0x0008
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_BONUS10_7_SIZE 0x0003
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_UNBIND_ON_RST_CTXIDLE_LSB 0x000b
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_UNBIND_ON_RST_CTXIDLE_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_LAST_FAULTRPT_EN_LSB 0x000c
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_LAST_FAULTRPT_EN_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_FLT_RPT_MODE_LSB 0x000d
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_FLT_RPT_MODE_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_EN_PF_REPORT_LSB 0x000e
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_EN_PF_REPORT_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_EN_PF_INT_GUC_LSB 0x000f
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_EN_PF_INT_GUC_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_EN_PF_REPAIR_LSB 0x0010
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_EN_PF_REPAIR_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_LOCK_SECURITY_CHK_LSB 0x0011
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_LOCK_SECURITY_CHK_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_EN_SECURITY_CHK_LSB 0x0012
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_EN_SECURITY_CHK_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_ATS_REPLAY_LSB 0x0013
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_ATS_REPLAY_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_STALL_FLTRSP_ONINV_LSB 0x0014
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_STALL_FLTRSP_ONINV_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_PAR_CATERR_TO_FABRIC_LSB 0x0015
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_PAR_CATERR_TO_FABRIC_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_ENB_BDST_ATSRESTART_LSB 0x0016
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_ENB_BDST_ATSRESTART_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_VFID_OVERD_ON_CS_DESCINV_LSB 0x0017
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_VFID_OVERD_ON_CS_DESCINV_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_TO_NOP_HOSTINV_ON_FLR_LSB 0x0018
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DIS_TO_NOP_HOSTINV_ON_FLR_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_EN_ENGINV_MASK_ON_RESET_LSB 0x0019
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_EN_ENGINV_MASK_ON_RESET_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_EN_BIND_FLOW_WITH_HIGHPRIOR_LSB 0x001a
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_EN_BIND_FLOW_WITH_HIGHPRIOR_SIZE 0x0001
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DBG_ENG_SEL_LSB 0x001b
#define ECOCHK_GAMCTRL_MEDIA_MEDIA_DBG_ENG_SEL_SIZE 0x0005


/** GAMCNTRL_CTRL_GAMCTRL_MEDIA desc:
  */

#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_REG (0x0038cf54U)

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
} GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_STRUCT;

#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_WIDTH 32
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_DEFAULT 0x00000000U
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_ENDPOINT MEDIA

#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_CGDIS_FLCT_LSB 0x0000
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_CGDIS_FLCT_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_CGDIS_COMM_LSB 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_CGDIS_COMM_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_INV_MODE_LSB 0x0002
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_INV_MODE_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_GO_ARB_LSB 0x0003
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_GO_ARB_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_INV_LTRSTR_LSB 0x0004
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_INV_LTRSTR_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_GO_ACK_CTL_LSB 0x0005
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_GO_ACK_CTL_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_RT_RSVDCHK_LSB 0x0006
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_RT_RSVDCHK_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_CTX_RSVDCHK_LSB 0x0007
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_CTX_RSVDCHK_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_GO_STALL_LSB 0x0008
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_GO_STALL_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_FLR_TIMEOUT_EN_LSB 0x0009
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_FLR_TIMEOUT_EN_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_EN_FLUSH_FLR_LSB 0x000a
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_EN_FLUSH_FLR_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_EN_FLUSH_ON_PWRDN_LSB 0x000b
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_EN_FLUSH_ON_PWRDN_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_DIS_BCAST_INVAL_LSB 0x000c
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_DIS_BCAST_INVAL_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_DIS_FLT_RETRY_TO_LSB 0x000d
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_DIS_FLT_RETRY_TO_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_G2G_LNK_CG_DIS_LSB 0x000e
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_G2G_LNK_CG_DIS_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_DIS_CS_CAT_FROM_SLAVE_LSB 0x000f
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_DIS_CS_CAT_FROM_SLAVE_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_DIS_KCR_RST_IN_PROG_LSB 0x0010
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_DIS_KCR_RST_IN_PROG_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_DIS_G2G_PEND_CTX_FLOW_LSB 0x0011
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_DIS_G2G_PEND_CTX_FLOW_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_SKIP_CFLUSH_ACK_LSB 0x0012
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_SKIP_CFLUSH_ACK_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_EN_STPDRN_ON_CTXIDLE_LSB 0x0013
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_EN_STPDRN_ON_CTXIDLE_SIZE 0x0001
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_TIMEOUT_VAL_LSB 0x0014
#define GAMCNTRL_CTRL_GAMCTRL_MEDIA_MEDIA_TIMEOUT_VAL_SIZE 0x000c



//************************************************ /RegisterStructs


#endif      // _MEDIA_h
