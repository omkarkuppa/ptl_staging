/** @file
  PCIE_MEM_RCRB.h

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

#ifndef _PCIE_MEM_RCRB_h
#define _PCIE_MEM_RCRB_h

// IP GUID: 95451bea-88e9-421d-8455-4540ebfab797
#define PCIE_MEM_RCRB_ID 0x95451bea88e9421d

//************************************************ RegisterStructs


/** IPRV desc:
  */

#define IPRV_PCIE_MEM_RCRB_REG           (0x00000080U)

typedef union {
  struct {
    UINT32 ipgenrv : 5; /**< IP Generation Revision */
                            /* Bits[4:0], default=0x00000003*/
    UINT32 ipdvrv : 4; /**< IP Derivative Revision */
                            /* Bits[8:5], default=0x00000004*/
    UINT32 iptpg : 5; /**< IP Topology */
                            /* Bits[13:9], default=0x00000000*/
    UINT32 rsvd_m : 18; /**< Reserved */
                            /* Bits[31:14], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} IPRV_PCIE_MEM_RCRB_STRUCT;

#define IPRV_PCIE_MEM_RCRB_WIDTH 32
#define IPRV_PCIE_MEM_RCRB_DEFAULT 0x00000083U
#define IPRV_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define IPRV_PCIE_MEM_RCRB_IPGENRV_LSB 0x0000
#define IPRV_PCIE_MEM_RCRB_IPGENRV_SIZE 0x0005
#define IPRV_PCIE_MEM_RCRB_IPDVRV_LSB 0x0005
#define IPRV_PCIE_MEM_RCRB_IPDVRV_SIZE 0x0004
#define IPRV_PCIE_MEM_RCRB_IPTPG_LSB 0x0009
#define IPRV_PCIE_MEM_RCRB_IPTPG_SIZE 0x0005
#define IPRV_PCIE_MEM_RCRB_RSVD_M_LSB 0x000e
#define IPRV_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0012



/** IPSCE desc:
  */

#define IPSCE_PCIE_MEM_RCRB_REG          (0x000000e0U)

typedef union {
  struct {
    UINT32 acmmrefclk : 1; /**< AFE Common Reference Clock Mode Control */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 arxescmmrefclk : 1; /**< AFE RX ES Common Reference Clock Mode Control */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[02:02] */
                            /* Bits[2:2], default=0*/
    UINT32 psphyfsel : 2; /**< Pclk Sourcing PHY Family Select\n */
                            /* Bits[4:3], default=0x00000000*/
    UINT32 pfpllc : 1; /**< Phy Per-Family PLL Control */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 rpcdcselp1 : 1; /**< Receive Posted Command and Data Credit Select Port 1 */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 rxdetep1 : 1; /**< Receiver Detection Enable Port 1 */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 pfpgcce : 1; /**< Per-Family PG Control Chain Enable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 dynlre : 1; /**< Dynamic Lane Reversal Enable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 sportlre : 1; /**< Static Per Port Lane Reversal Enable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 sourcedecdis : 1; /**< Source Decode Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 mtdm : 2; /**< Master Transaction Decode Mode */
                            /* Bits[13:12], default=0x00000000*/
    UINT32 utme : 1; /**< UT mechanism Enable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 txstrictorderme : 1; /**< Transmit Strict Ordering Mode Enable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 txrom : 2; /**< Transmit Completion Relaxed Ordering Mode */
                            /* Bits[17:16], default=0x00000000*/
    UINT32 rxrom : 2; /**< Receive Completion Relaxed Ordering Mode */
                            /* Bits[19:18], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[20:20] */
                            /* Bits[20:20], default=0*/
    UINT32 rsvd_m : 11; /**< Reserved */
                            /* Bits[31:21], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} IPSCE_PCIE_MEM_RCRB_STRUCT;

#define IPSCE_PCIE_MEM_RCRB_WIDTH 32
#define IPSCE_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define IPSCE_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define IPSCE_PCIE_MEM_RCRB_ACMMREFCLK_LSB 0x0000
#define IPSCE_PCIE_MEM_RCRB_ACMMREFCLK_SIZE 0x0001
#define IPSCE_PCIE_MEM_RCRB_ARXESCMMREFCLK_LSB 0x0001
#define IPSCE_PCIE_MEM_RCRB_ARXESCMMREFCLK_SIZE 0x0001
#define IPSCE_PCIE_MEM_RCRB_PSPHYFSEL_LSB 0x0003
#define IPSCE_PCIE_MEM_RCRB_PSPHYFSEL_SIZE 0x0002
#define IPSCE_PCIE_MEM_RCRB_PFPLLC_LSB 0x0005
#define IPSCE_PCIE_MEM_RCRB_PFPLLC_SIZE 0x0001
#define IPSCE_PCIE_MEM_RCRB_RPCDCSELP1_LSB 0x0006
#define IPSCE_PCIE_MEM_RCRB_RPCDCSELP1_SIZE 0x0001
#define IPSCE_PCIE_MEM_RCRB_RXDETEP1_LSB 0x0007
#define IPSCE_PCIE_MEM_RCRB_RXDETEP1_SIZE 0x0001
#define IPSCE_PCIE_MEM_RCRB_PFPGCCE_LSB 0x0008
#define IPSCE_PCIE_MEM_RCRB_PFPGCCE_SIZE 0x0001
#define IPSCE_PCIE_MEM_RCRB_DYNLRE_LSB 0x0009
#define IPSCE_PCIE_MEM_RCRB_DYNLRE_SIZE 0x0001
#define IPSCE_PCIE_MEM_RCRB_SPORTLRE_LSB 0x000a
#define IPSCE_PCIE_MEM_RCRB_SPORTLRE_SIZE 0x0001
#define IPSCE_PCIE_MEM_RCRB_SOURCEDECDIS_LSB 0x000b
#define IPSCE_PCIE_MEM_RCRB_SOURCEDECDIS_SIZE 0x0001
#define IPSCE_PCIE_MEM_RCRB_MTDM_LSB 0x000c
#define IPSCE_PCIE_MEM_RCRB_MTDM_SIZE 0x0002
#define IPSCE_PCIE_MEM_RCRB_UTME_LSB 0x000e
#define IPSCE_PCIE_MEM_RCRB_UTME_SIZE 0x0001
#define IPSCE_PCIE_MEM_RCRB_TXSTRICTORDERME_LSB 0x000f
#define IPSCE_PCIE_MEM_RCRB_TXSTRICTORDERME_SIZE 0x0001
#define IPSCE_PCIE_MEM_RCRB_TXROM_LSB 0x0010
#define IPSCE_PCIE_MEM_RCRB_TXROM_SIZE 0x0002
#define IPSCE_PCIE_MEM_RCRB_RXROM_LSB 0x0012
#define IPSCE_PCIE_MEM_RCRB_RXROM_SIZE 0x0002
#define IPSCE_PCIE_MEM_RCRB_RSVD_M_LSB 0x0015
#define IPSCE_PCIE_MEM_RCRB_RSVD_M_SIZE 0x000b



/** PDCTL desc:
  */

#define PDCTL_PCIE_MEM_RCRB_REG          (0x00000100U)

typedef union {
  struct {
    UINT8 inbpo : 1; /**< Use Inband Presence Only */
                            /* Bits[0:0], default=0x00000000*/
    UINT8 oobpo : 1; /**< Use OOB Presence Only */
                            /* Bits[1:1], default=0x00000000*/
    UINT8 rsvd_m : 6; /**< Reserved */
                            /* Bits[7:2], default=0x00000000*/
  }     Bits;
  UINT8 Data;
} PDCTL_PCIE_MEM_RCRB_STRUCT;

#define PDCTL_PCIE_MEM_RCRB_WIDTH 8
#define PDCTL_PCIE_MEM_RCRB_DEFAULT 0x00U
#define PDCTL_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define PDCTL_PCIE_MEM_RCRB_INBPO_LSB 0x0000
#define PDCTL_PCIE_MEM_RCRB_INBPO_SIZE 0x0001
#define PDCTL_PCIE_MEM_RCRB_OOBPO_LSB 0x0001
#define PDCTL_PCIE_MEM_RCRB_OOBPO_SIZE 0x0001
#define PDCTL_PCIE_MEM_RCRB_RSVD_M_LSB 0x0002
#define PDCTL_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0006



/** FCTL desc:
  */

#define FCTL_PCIE_MEM_RCRB_REG           (0x00000300U)

typedef union {
  struct {
    UINT32 pasidtpse : 1; /**< PASID TLP Prefix Support Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 enqcse : 1; /**< ENQ Command Support Enable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 crsrtv : 8; /**< CRS Retry Timeout Value */
                            /* Bits[9:2], default=0x00000010*/
    UINT32 pbl1nake : 1; /**< Pin Based L1 NAK Enable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 ocilc : 1; /**< Optional Credit Initialization Limit Checkling */
                            /* Bits[11:11], default=0x00000001*/
    UINT32 crspsel : 1; /**< Completion Timer Timeout Policy */
                            /* Bits[12:12], default=0x00000001*/
    UINT32 aeroiosfsb : 1; /**< AER Over IOSF-SB */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 g12fsfrm : 1; /**< Gen1 and 2 Frame Steer FSM in Full Recovery Mode */
                            /* Bits[14:14], default=0x00000001*/
    UINT32 tonptpal1p : 1; /**< Transmit Outstanding Non-Posted Tracking Policy For ASPM L1 Prevention */
                            /* Bits[15:15], default=0x00000001*/
    UINT32 ronptpal1p : 1; /**< Receive Outstanding Non-Posted Tracking Policy For ASPM L1 Prevention */
                            /* Bits[16:16], default=0x00000001*/
    UINT32 ctm : 2; /**< Config Tunnelling Mode */
                            /* Bits[18:17], default=0x00000000*/
    UINT32 ctcd : 1; /**< Config Type Conversion Disable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 rsvd_m : 12; /**< Reserved */
                            /* Bits[31:20], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FCTL_PCIE_MEM_RCRB_STRUCT;

#define FCTL_PCIE_MEM_RCRB_WIDTH 32
#define FCTL_PCIE_MEM_RCRB_DEFAULT 0x0001d840U
#define FCTL_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define FCTL_PCIE_MEM_RCRB_PASIDTPSE_LSB 0x0000
#define FCTL_PCIE_MEM_RCRB_PASIDTPSE_SIZE 0x0001
#define FCTL_PCIE_MEM_RCRB_ENQCSE_LSB 0x0001
#define FCTL_PCIE_MEM_RCRB_ENQCSE_SIZE 0x0001
#define FCTL_PCIE_MEM_RCRB_CRSRTV_LSB 0x0002
#define FCTL_PCIE_MEM_RCRB_CRSRTV_SIZE 0x0008
#define FCTL_PCIE_MEM_RCRB_PBL1NAKE_LSB 0x000a
#define FCTL_PCIE_MEM_RCRB_PBL1NAKE_SIZE 0x0001
#define FCTL_PCIE_MEM_RCRB_OCILC_LSB 0x000b
#define FCTL_PCIE_MEM_RCRB_OCILC_SIZE 0x0001
#define FCTL_PCIE_MEM_RCRB_CRSPSEL_LSB 0x000c
#define FCTL_PCIE_MEM_RCRB_CRSPSEL_SIZE 0x0001
#define FCTL_PCIE_MEM_RCRB_AEROIOSFSB_LSB 0x000d
#define FCTL_PCIE_MEM_RCRB_AEROIOSFSB_SIZE 0x0001
#define FCTL_PCIE_MEM_RCRB_G12FSFRM_LSB 0x000e
#define FCTL_PCIE_MEM_RCRB_G12FSFRM_SIZE 0x0001
#define FCTL_PCIE_MEM_RCRB_TONPTPAL1P_LSB 0x000f
#define FCTL_PCIE_MEM_RCRB_TONPTPAL1P_SIZE 0x0001
#define FCTL_PCIE_MEM_RCRB_RONPTPAL1P_LSB 0x0010
#define FCTL_PCIE_MEM_RCRB_RONPTPAL1P_SIZE 0x0001
#define FCTL_PCIE_MEM_RCRB_CTM_LSB 0x0011
#define FCTL_PCIE_MEM_RCRB_CTM_SIZE 0x0002
#define FCTL_PCIE_MEM_RCRB_CTCD_LSB 0x0013
#define FCTL_PCIE_MEM_RCRB_CTCD_SIZE 0x0001
#define FCTL_PCIE_MEM_RCRB_RSVD_M_LSB 0x0014
#define FCTL_PCIE_MEM_RCRB_RSVD_M_SIZE 0x000c



/** FCUCTL desc:
  */

#define FCUCTL_PCIE_MEM_RCRB_REG         (0x00000304U)

typedef union {
  struct {
    UINT32 fcu_mode : 2; /**< Flow Control Update Mode */
                            /* Bits[1:0], default=0x00000000*/
    UINT32 fcp_p_period : 6; /**< Flow Control Posted Period */
                            /* Bits[7:2], default=0x00000006*/
    UINT32 fcp_np_period : 6; /**< Flow Control Non-Posted Period */
                            /* Bits[13:8], default=0x00000006*/
    UINT32 fcp_cp_period : 6; /**< Flow Control Completion Period */
                            /* Bits[19:14], default=0x00000006*/
    UINT32 fc_cp_fcm : 1; /**< Flow Control Completion Finite Credit Mode */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 idle_fc_period : 6; /**< Idle Flow Control Period */
                            /* Bits[26:21], default=0x00000000*/
    UINT32 fc_cp_fcm_vc1 : 1; /**< Flow Control Completion Finite Credit Mode VC1 */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 fc_cp_fcm_vcm : 1; /**< Flow Control Completion Finite Credit Mode VCM */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 rsvd_m : 3; /**< Reserved */
                            /* Bits[31:29], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FCUCTL_PCIE_MEM_RCRB_STRUCT;

#define FCUCTL_PCIE_MEM_RCRB_WIDTH 32
#define FCUCTL_PCIE_MEM_RCRB_DEFAULT 0x00018618U
#define FCUCTL_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define FCUCTL_PCIE_MEM_RCRB_FCU_MODE_LSB 0x0000
#define FCUCTL_PCIE_MEM_RCRB_FCU_MODE_SIZE 0x0002
#define FCUCTL_PCIE_MEM_RCRB_FCP_P_PERIOD_LSB 0x0002
#define FCUCTL_PCIE_MEM_RCRB_FCP_P_PERIOD_SIZE 0x0006
#define FCUCTL_PCIE_MEM_RCRB_FCP_NP_PERIOD_LSB 0x0008
#define FCUCTL_PCIE_MEM_RCRB_FCP_NP_PERIOD_SIZE 0x0006
#define FCUCTL_PCIE_MEM_RCRB_FCP_CP_PERIOD_LSB 0x000e
#define FCUCTL_PCIE_MEM_RCRB_FCP_CP_PERIOD_SIZE 0x0006
#define FCUCTL_PCIE_MEM_RCRB_FC_CP_FCM_LSB 0x0014
#define FCUCTL_PCIE_MEM_RCRB_FC_CP_FCM_SIZE 0x0001
#define FCUCTL_PCIE_MEM_RCRB_IDLE_FC_PERIOD_LSB 0x0015
#define FCUCTL_PCIE_MEM_RCRB_IDLE_FC_PERIOD_SIZE 0x0006
#define FCUCTL_PCIE_MEM_RCRB_FC_CP_FCM_VC1_LSB 0x001b
#define FCUCTL_PCIE_MEM_RCRB_FC_CP_FCM_VC1_SIZE 0x0001
#define FCUCTL_PCIE_MEM_RCRB_FC_CP_FCM_VCM_LSB 0x001c
#define FCUCTL_PCIE_MEM_RCRB_FC_CP_FCM_VCM_SIZE 0x0001
#define FCUCTL_PCIE_MEM_RCRB_RSVD_M_LSB 0x001d
#define FCUCTL_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0003



/** TXCRSTOCTL desc:
  */

#define TXCRSTOCTL_PCIE_MEM_RCRB_REG     (0x00000320U)

typedef union {
  struct {
    UINT32 txtofcen : 3; /**< Transmit Timer Flow Class Enable */
                            /* Bits[2:0], default=0x00000000*/
    UINT32 txnpctodis : 1; /**< Transmit Non-Posted Completion Timeout Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 txtov : 4; /**< Transmit Timeout Value */
                            /* Bits[7:4], default=0x00000000*/
    UINT32 txtouren : 3; /**< Transmit Timeout UR Enable */
                            /* Bits[10:8], default=0x00000000*/
    UINT32 txtolden : 3; /**< Transmit Timeout Link Down Enable */
                            /* Bits[13:11], default=0x00000000*/
    UINT32 txtostr : 1; /**< Transmit Timeout Single Transaction Response */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 crs_to_dis : 1; /**< Configuration Retry Timeout Disable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 txtosts : 3; /**< Transmit Timeout Status */
                            /* Bits[18:16], default=0x00000000*/
    UINT32 txnpctosts : 1; /**< Transmit Non-Posted Completion Timeout Status */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 crs_to_sts : 1; /**< Configuration Retry Timeout Status */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 rsvd_m : 11; /**< Reserved */
                            /* Bits[31:21], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} TXCRSTOCTL_PCIE_MEM_RCRB_STRUCT;

#define TXCRSTOCTL_PCIE_MEM_RCRB_WIDTH 32
#define TXCRSTOCTL_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define TXCRSTOCTL_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define TXCRSTOCTL_PCIE_MEM_RCRB_TXTOFCEN_LSB 0x0000
#define TXCRSTOCTL_PCIE_MEM_RCRB_TXTOFCEN_SIZE 0x0003
#define TXCRSTOCTL_PCIE_MEM_RCRB_TXNPCTODIS_LSB 0x0003
#define TXCRSTOCTL_PCIE_MEM_RCRB_TXNPCTODIS_SIZE 0x0001
#define TXCRSTOCTL_PCIE_MEM_RCRB_TXTOV_LSB 0x0004
#define TXCRSTOCTL_PCIE_MEM_RCRB_TXTOV_SIZE 0x0004
#define TXCRSTOCTL_PCIE_MEM_RCRB_TXTOUREN_LSB 0x0008
#define TXCRSTOCTL_PCIE_MEM_RCRB_TXTOUREN_SIZE 0x0003
#define TXCRSTOCTL_PCIE_MEM_RCRB_TXTOLDEN_LSB 0x000b
#define TXCRSTOCTL_PCIE_MEM_RCRB_TXTOLDEN_SIZE 0x0003
#define TXCRSTOCTL_PCIE_MEM_RCRB_TXTOSTR_LSB 0x000e
#define TXCRSTOCTL_PCIE_MEM_RCRB_TXTOSTR_SIZE 0x0001
#define TXCRSTOCTL_PCIE_MEM_RCRB_CRS_TO_DIS_LSB 0x000f
#define TXCRSTOCTL_PCIE_MEM_RCRB_CRS_TO_DIS_SIZE 0x0001
#define TXCRSTOCTL_PCIE_MEM_RCRB_TXTOSTS_LSB 0x0010
#define TXCRSTOCTL_PCIE_MEM_RCRB_TXTOSTS_SIZE 0x0003
#define TXCRSTOCTL_PCIE_MEM_RCRB_TXNPCTOSTS_LSB 0x0013
#define TXCRSTOCTL_PCIE_MEM_RCRB_TXNPCTOSTS_SIZE 0x0001
#define TXCRSTOCTL_PCIE_MEM_RCRB_CRS_TO_STS_LSB 0x0014
#define TXCRSTOCTL_PCIE_MEM_RCRB_CRS_TO_STS_SIZE 0x0001
#define TXCRSTOCTL_PCIE_MEM_RCRB_RSVD_M_LSB 0x0015
#define TXCRSTOCTL_PCIE_MEM_RCRB_RSVD_M_SIZE 0x000b



/** LLPC desc:
  */

#define LLPC_PCIE_MEM_RCRB_REG           (0x0000032cU)

typedef union {
  struct {
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[00:00] */
                            /* Bits[0:0], default=0*/
    UINT32 al1cpp : 2; /**< ASPM L1 Cooling Period Policies */
                            /* Bits[2:1], default=0x00000000*/
    UINT32 tlrxocd8b10b : 1; /**< Link Receiver Optional Check Disable for 8b/10b Encoding */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 tlrxocd128b130b : 1; /**< Link Receiver Optional Check Disable for 128b/130b Encoding */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 lilndno : 1; /**< Link Initialization Link Down Overide */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 al1ntp : 1; /**< ASPM L1 Nak latency Timer Policies */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 lrlrsmf8b10b : 3; /**< Link Receive Link Recovery Surviveability Mechanisms For 8B10B Encoding */
                            /* Bits[9:7], default=0x00000000*/
    UINT32 rsvd_1 : 2; /**< Undefined - auto filled rsvd_[11:10] */
                            /* Bits[11:10], default=0*/
    UINT32 rsvd_m : 20; /**< Reserved */
                            /* Bits[31:12], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} LLPC_PCIE_MEM_RCRB_STRUCT;

#define LLPC_PCIE_MEM_RCRB_WIDTH 32
#define LLPC_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define LLPC_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define LLPC_PCIE_MEM_RCRB_AL1CPP_LSB 0x0001
#define LLPC_PCIE_MEM_RCRB_AL1CPP_SIZE 0x0002
#define LLPC_PCIE_MEM_RCRB_TLRXOCD8B10B_LSB 0x0003
#define LLPC_PCIE_MEM_RCRB_TLRXOCD8B10B_SIZE 0x0001
#define LLPC_PCIE_MEM_RCRB_TLRXOCD128B130B_LSB 0x0004
#define LLPC_PCIE_MEM_RCRB_TLRXOCD128B130B_SIZE 0x0001
#define LLPC_PCIE_MEM_RCRB_LILNDNO_LSB 0x0005
#define LLPC_PCIE_MEM_RCRB_LILNDNO_SIZE 0x0001
#define LLPC_PCIE_MEM_RCRB_AL1NTP_LSB 0x0006
#define LLPC_PCIE_MEM_RCRB_AL1NTP_SIZE 0x0001
#define LLPC_PCIE_MEM_RCRB_LRLRSMF8B10B_LSB 0x0007
#define LLPC_PCIE_MEM_RCRB_LRLRSMF8B10B_SIZE 0x0003
#define LLPC_PCIE_MEM_RCRB_RSVD_M_LSB 0x000c
#define LLPC_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0014



/** FCTL2 desc:
  */

#define FCTL2_PCIE_MEM_RCRB_REG          (0x00000330U)

typedef union {
  struct {
    UINT32 rsvd_0 : 4; /**< Undefined - auto filled rsvd_[03:00] */
                            /* Bits[3:0], default=0*/
    UINT32 rptot : 3; /**< Reset Prep Timeout Timer */
                            /* Bits[6:4], default=0x00000000*/
    UINT32 hrtctl : 3; /**< Hot Reset Timer Control */
                            /* Bits[9:7], default=0x00000000*/
    UINT32 hpictl : 1; /**< Hot Plug Interrupt Control */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 blknat : 1; /**< Block NOP to ACTIVE Transition */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 txenctl : 1; /**< Transmit Enable Control on Link Layer Transmit */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 disctocfg : 1; /**< Disable Completion Timer Timeout for Outstanding Config Transaction */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 ldtctl : 3; /**< Link Disable Timer Control */
                            /* Bits[16:14], default=0x00000000*/
    UINT32 berpfhrt : 3; /**< Best Effort Reset Prep Flow Hot Reset Timeout */
                            /* Bits[19:17], default=0x00000000*/
    UINT32 srltewld : 1; /**< Synchonize Reset Link Transmit Enable When LPIF+ Down */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 fact2nopt : 1; /**< Force ACTIVE to NOP Transition */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 flpforcedtt : 1; /**< Force lp_force_detect Transition */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 flplnkerrt : 1; /**< Force lp_linkerror Transition */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 mfd2lepc : 1; /**< Map lp_force_detect to lp_linkerror Policy Control */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 ttifmtm : 2; /**< TTIF Master Transfer Mode */
                            /* Bits[26:25], default=0x00000000*/
    UINT32 rxcppreallocen : 1; /**< Receive Completion Preallocation Enable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 clkctlsaveaddrvnnremen : 1; /**< Clock Control using Save Address for VNN Removal Enable */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 rsvd_m : 3; /**< Reserved */
                            /* Bits[31:29], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FCTL2_PCIE_MEM_RCRB_STRUCT;

#define FCTL2_PCIE_MEM_RCRB_WIDTH 32
#define FCTL2_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define FCTL2_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define FCTL2_PCIE_MEM_RCRB_RPTOT_LSB 0x0004
#define FCTL2_PCIE_MEM_RCRB_RPTOT_SIZE 0x0003
#define FCTL2_PCIE_MEM_RCRB_HRTCTL_LSB 0x0007
#define FCTL2_PCIE_MEM_RCRB_HRTCTL_SIZE 0x0003
#define FCTL2_PCIE_MEM_RCRB_HPICTL_LSB 0x000a
#define FCTL2_PCIE_MEM_RCRB_HPICTL_SIZE 0x0001
#define FCTL2_PCIE_MEM_RCRB_BLKNAT_LSB 0x000b
#define FCTL2_PCIE_MEM_RCRB_BLKNAT_SIZE 0x0001
#define FCTL2_PCIE_MEM_RCRB_TXENCTL_LSB 0x000c
#define FCTL2_PCIE_MEM_RCRB_TXENCTL_SIZE 0x0001
#define FCTL2_PCIE_MEM_RCRB_DISCTOCFG_LSB 0x000d
#define FCTL2_PCIE_MEM_RCRB_DISCTOCFG_SIZE 0x0001
#define FCTL2_PCIE_MEM_RCRB_LDTCTL_LSB 0x000e
#define FCTL2_PCIE_MEM_RCRB_LDTCTL_SIZE 0x0003
#define FCTL2_PCIE_MEM_RCRB_BERPFHRT_LSB 0x0011
#define FCTL2_PCIE_MEM_RCRB_BERPFHRT_SIZE 0x0003
#define FCTL2_PCIE_MEM_RCRB_SRLTEWLD_LSB 0x0014
#define FCTL2_PCIE_MEM_RCRB_SRLTEWLD_SIZE 0x0001
#define FCTL2_PCIE_MEM_RCRB_FACT2NOPT_LSB 0x0015
#define FCTL2_PCIE_MEM_RCRB_FACT2NOPT_SIZE 0x0001
#define FCTL2_PCIE_MEM_RCRB_FLPFORCEDTT_LSB 0x0016
#define FCTL2_PCIE_MEM_RCRB_FLPFORCEDTT_SIZE 0x0001
#define FCTL2_PCIE_MEM_RCRB_FLPLNKERRT_LSB 0x0017
#define FCTL2_PCIE_MEM_RCRB_FLPLNKERRT_SIZE 0x0001
#define FCTL2_PCIE_MEM_RCRB_MFD2LEPC_LSB 0x0018
#define FCTL2_PCIE_MEM_RCRB_MFD2LEPC_SIZE 0x0001
#define FCTL2_PCIE_MEM_RCRB_TTIFMTM_LSB 0x0019
#define FCTL2_PCIE_MEM_RCRB_TTIFMTM_SIZE 0x0002
#define FCTL2_PCIE_MEM_RCRB_RXCPPREALLOCEN_LSB 0x001b
#define FCTL2_PCIE_MEM_RCRB_RXCPPREALLOCEN_SIZE 0x0001
#define FCTL2_PCIE_MEM_RCRB_CLKCTLSAVEADDRVNNREMEN_LSB 0x001c
#define FCTL2_PCIE_MEM_RCRB_CLKCTLSAVEADDRVNNREMEN_SIZE 0x0001
#define FCTL2_PCIE_MEM_RCRB_RSVD_M_LSB 0x001d
#define FCTL2_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0003



/** RPR desc:
  */

#define RPR_PCIE_MEM_RCRB_REG            (0x00000334U)

typedef union {
  struct {
    UINT32 dm : 2; /**< Default Mode */
                            /* Bits[1:0], default=0x00000000*/
    UINT32 wrm : 2; /**< Warm Reset Mode */
                            /* Bits[3:2], default=0x00000000*/
    UINT32 crm : 2; /**< Cold Reset Mode */
                            /* Bits[5:4], default=0x00000000*/
    UINT32 s3sm : 2; /**< S3 Sleep Mode */
                            /* Bits[7:6], default=0x00000000*/
    UINT32 s4sm : 2; /**< S4 Sleep Mode */
                            /* Bits[9:8], default=0x00000000*/
    UINT32 s5sm : 2; /**< S5 Sleep Mode */
                            /* Bits[11:10], default=0x00000000*/
    UINT32 dmto : 2; /**< Default Mode Timeout */
                            /* Bits[13:12], default=0x00000000*/
    UINT32 wrmto : 2; /**< Warm Reset Mode Timeout */
                            /* Bits[15:14], default=0x00000000*/
    UINT32 crmto : 2; /**< Cold Reset Mode Timeout */
                            /* Bits[17:16], default=0x00000000*/
    UINT32 s3smto : 2; /**< S3 Sleep Mode Timeout */
                            /* Bits[19:18], default=0x00000000*/
    UINT32 s4smto : 2; /**< S4 Sleep Mode Timeout */
                            /* Bits[21:20], default=0x00000000*/
    UINT32 s5smto : 2; /**< S5 Sleep Mode Timeout */
                            /* Bits[23:22], default=0x00000000*/
    UINT32 rsvd_m : 8; /**< Reserved */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RPR_PCIE_MEM_RCRB_STRUCT;

#define RPR_PCIE_MEM_RCRB_WIDTH 32
#define RPR_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define RPR_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define RPR_PCIE_MEM_RCRB_DM_LSB 0x0000
#define RPR_PCIE_MEM_RCRB_DM_SIZE 0x0002
#define RPR_PCIE_MEM_RCRB_WRM_LSB 0x0002
#define RPR_PCIE_MEM_RCRB_WRM_SIZE 0x0002
#define RPR_PCIE_MEM_RCRB_CRM_LSB 0x0004
#define RPR_PCIE_MEM_RCRB_CRM_SIZE 0x0002
#define RPR_PCIE_MEM_RCRB_S3SM_LSB 0x0006
#define RPR_PCIE_MEM_RCRB_S3SM_SIZE 0x0002
#define RPR_PCIE_MEM_RCRB_S4SM_LSB 0x0008
#define RPR_PCIE_MEM_RCRB_S4SM_SIZE 0x0002
#define RPR_PCIE_MEM_RCRB_S5SM_LSB 0x000a
#define RPR_PCIE_MEM_RCRB_S5SM_SIZE 0x0002
#define RPR_PCIE_MEM_RCRB_DMTO_LSB 0x000c
#define RPR_PCIE_MEM_RCRB_DMTO_SIZE 0x0002
#define RPR_PCIE_MEM_RCRB_WRMTO_LSB 0x000e
#define RPR_PCIE_MEM_RCRB_WRMTO_SIZE 0x0002
#define RPR_PCIE_MEM_RCRB_CRMTO_LSB 0x0010
#define RPR_PCIE_MEM_RCRB_CRMTO_SIZE 0x0002
#define RPR_PCIE_MEM_RCRB_S3SMTO_LSB 0x0012
#define RPR_PCIE_MEM_RCRB_S3SMTO_SIZE 0x0002
#define RPR_PCIE_MEM_RCRB_S4SMTO_LSB 0x0014
#define RPR_PCIE_MEM_RCRB_S4SMTO_SIZE 0x0002
#define RPR_PCIE_MEM_RCRB_S5SMTO_LSB 0x0016
#define RPR_PCIE_MEM_RCRB_S5SMTO_SIZE 0x0002
#define RPR_PCIE_MEM_RCRB_RSVD_M_LSB 0x0018
#define RPR_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0008



/** DCGEN1 desc:
  */

#define DCGEN1_PCIE_MEM_RCRB_REG         (0x00000350U)

typedef union {
  struct {
    UINT32 pxkguldcgen : 1; /**< PXKG Unit Link Clock DCG Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 pxculdcgen : 1; /**< PXC Unit Link Clock DCG Enable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 rsvd_0 : 6; /**< Undefined - auto filled rsvd_[07:02] */
                            /* Bits[7:2], default=0*/
    UINT32 pxlruldcgen : 1; /**< PXLR Unit Link Clock DCG Enable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 pxltuldcgen : 1; /**< PXLT Unit Link Clock DCG Enable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 pxliuldcgen : 1; /**< PXLI Unit Link Clock DCG Enable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 pxlsuldcgen : 1; /**< PXLS Unit Link Clock DCG Enable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 pxcp2uldcgen : 1; /**< PXCP2 Unit Link Clock DCG Enable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 rsvd_1 : 3; /**< Undefined - auto filled rsvd_[15:13] */
                            /* Bits[15:13], default=0*/
    UINT32 pxtruldcgen : 1; /**< PXTR Unit Link Clock DCG Enable */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 pxtrsuldcgen : 1; /**< PXTRS Unit Link Clock DCG Enable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 pxtrssuldcgen : 1; /**< PXTRSS Unit Link Clock DCG Enable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 pxcp3uldcgen : 1; /**< PXCP3 Unit Link Clock DCG Enable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 rsvd_2 : 4; /**< Undefined - auto filled rsvd_[23:20] */
                            /* Bits[23:20], default=0*/
    UINT32 pxttuldcgen : 1; /**< PXTT Unit Link Clock DCG Enable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 pxttsuldcgen : 1; /**< PXTTS Unit Link Clock PXTTSS DCG Enable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 pxttssuldcgen : 1; /**< PXTTS Unit Link Clock DCG Enable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 pxcp4uldcgen : 1; /**< PXCP4 Unit Link Clock DCG Enable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 rsvd_3 : 3; /**< Undefined - auto filled rsvd_[30:28] */
                            /* Bits[30:28], default=0*/
    UINT32 rsvd_m : 1; /**< Reserved */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DCGEN1_PCIE_MEM_RCRB_STRUCT;

#define DCGEN1_PCIE_MEM_RCRB_WIDTH 32
#define DCGEN1_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define DCGEN1_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define DCGEN1_PCIE_MEM_RCRB_PXKGULDCGEN_LSB 0x0000
#define DCGEN1_PCIE_MEM_RCRB_PXKGULDCGEN_SIZE 0x0001
#define DCGEN1_PCIE_MEM_RCRB_PXCULDCGEN_LSB 0x0001
#define DCGEN1_PCIE_MEM_RCRB_PXCULDCGEN_SIZE 0x0001
#define DCGEN1_PCIE_MEM_RCRB_PXLRULDCGEN_LSB 0x0008
#define DCGEN1_PCIE_MEM_RCRB_PXLRULDCGEN_SIZE 0x0001
#define DCGEN1_PCIE_MEM_RCRB_PXLTULDCGEN_LSB 0x0009
#define DCGEN1_PCIE_MEM_RCRB_PXLTULDCGEN_SIZE 0x0001
#define DCGEN1_PCIE_MEM_RCRB_PXLIULDCGEN_LSB 0x000a
#define DCGEN1_PCIE_MEM_RCRB_PXLIULDCGEN_SIZE 0x0001
#define DCGEN1_PCIE_MEM_RCRB_PXLSULDCGEN_LSB 0x000b
#define DCGEN1_PCIE_MEM_RCRB_PXLSULDCGEN_SIZE 0x0001
#define DCGEN1_PCIE_MEM_RCRB_PXCP2ULDCGEN_LSB 0x000c
#define DCGEN1_PCIE_MEM_RCRB_PXCP2ULDCGEN_SIZE 0x0001
#define DCGEN1_PCIE_MEM_RCRB_PXTRULDCGEN_LSB 0x0010
#define DCGEN1_PCIE_MEM_RCRB_PXTRULDCGEN_SIZE 0x0001
#define DCGEN1_PCIE_MEM_RCRB_PXTRSULDCGEN_LSB 0x0011
#define DCGEN1_PCIE_MEM_RCRB_PXTRSULDCGEN_SIZE 0x0001
#define DCGEN1_PCIE_MEM_RCRB_PXTRSSULDCGEN_LSB 0x0012
#define DCGEN1_PCIE_MEM_RCRB_PXTRSSULDCGEN_SIZE 0x0001
#define DCGEN1_PCIE_MEM_RCRB_PXCP3ULDCGEN_LSB 0x0013
#define DCGEN1_PCIE_MEM_RCRB_PXCP3ULDCGEN_SIZE 0x0001
#define DCGEN1_PCIE_MEM_RCRB_PXTTULDCGEN_LSB 0x0018
#define DCGEN1_PCIE_MEM_RCRB_PXTTULDCGEN_SIZE 0x0001
#define DCGEN1_PCIE_MEM_RCRB_PXTTSULDCGEN_LSB 0x0019
#define DCGEN1_PCIE_MEM_RCRB_PXTTSULDCGEN_SIZE 0x0001
#define DCGEN1_PCIE_MEM_RCRB_PXTTSSULDCGEN_LSB 0x001a
#define DCGEN1_PCIE_MEM_RCRB_PXTTSSULDCGEN_SIZE 0x0001
#define DCGEN1_PCIE_MEM_RCRB_PXCP4ULDCGEN_LSB 0x001b
#define DCGEN1_PCIE_MEM_RCRB_PXCP4ULDCGEN_SIZE 0x0001
#define DCGEN1_PCIE_MEM_RCRB_RSVD_M_LSB 0x001f
#define DCGEN1_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0001



/** DCGEN2 desc:
  */

#define DCGEN2_PCIE_MEM_RCRB_REG         (0x00000354U)

typedef union {
  struct {
    UINT32 pxpiuldcgen : 1; /**< PXPI Unit Link Clock DCG Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 pxpsuldcgen : 1; /**< PXPS Unit Link Clock DCG Enable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 pxpbuldcgen : 1; /**< PXPB Unit Link Clock DCG Enable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 pxfiuldcgen : 1; /**< PXFI Unit Link Clock DCG Enable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 pxftuldcgen : 1; /**< PXFT Unit Link Clock DCG Enable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 pxfruldcgen : 1; /**< PXFR Unit Link Clock DCG Enable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 pxcp5uldcgen : 1; /**< PXCP5 Unit Link Clock DCG Enable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 rsvd_0 : 17; /**< Undefined - auto filled rsvd_[23:07] */
                            /* Bits[23:7], default=0*/
    UINT32 rsvd_m : 8; /**< Reserved */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DCGEN2_PCIE_MEM_RCRB_STRUCT;

#define DCGEN2_PCIE_MEM_RCRB_WIDTH 32
#define DCGEN2_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define DCGEN2_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define DCGEN2_PCIE_MEM_RCRB_PXPIULDCGEN_LSB 0x0000
#define DCGEN2_PCIE_MEM_RCRB_PXPIULDCGEN_SIZE 0x0001
#define DCGEN2_PCIE_MEM_RCRB_PXPSULDCGEN_LSB 0x0001
#define DCGEN2_PCIE_MEM_RCRB_PXPSULDCGEN_SIZE 0x0001
#define DCGEN2_PCIE_MEM_RCRB_PXPBULDCGEN_LSB 0x0002
#define DCGEN2_PCIE_MEM_RCRB_PXPBULDCGEN_SIZE 0x0001
#define DCGEN2_PCIE_MEM_RCRB_PXFIULDCGEN_LSB 0x0003
#define DCGEN2_PCIE_MEM_RCRB_PXFIULDCGEN_SIZE 0x0001
#define DCGEN2_PCIE_MEM_RCRB_PXFTULDCGEN_LSB 0x0004
#define DCGEN2_PCIE_MEM_RCRB_PXFTULDCGEN_SIZE 0x0001
#define DCGEN2_PCIE_MEM_RCRB_PXFRULDCGEN_LSB 0x0005
#define DCGEN2_PCIE_MEM_RCRB_PXFRULDCGEN_SIZE 0x0001
#define DCGEN2_PCIE_MEM_RCRB_PXCP5ULDCGEN_LSB 0x0006
#define DCGEN2_PCIE_MEM_RCRB_PXCP5ULDCGEN_SIZE 0x0001
#define DCGEN2_PCIE_MEM_RCRB_RSVD_M_LSB 0x0018
#define DCGEN2_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0008



/** DCGM1 desc:
  */

#define DCGM1_PCIE_MEM_RCRB_REG          (0x00000358U)

typedef union {
  struct {
    UINT32 pxkguldcgm : 1; /**< PXKG Unit Link Clock DCG Mode */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 pxculdcgm : 1; /**< PXC Unit Link Clock DCG Mode */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 rsvd_0 : 6; /**< Undefined - auto filled rsvd_[07:02] */
                            /* Bits[7:2], default=0*/
    UINT32 pxlruldcgm : 1; /**< PXLR Unit Link Clock DCG Mode */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 pxltuldcgm : 1; /**< PXLT Unit Link Clock DCG Mode */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 pxliuldcgm : 1; /**< PXLI Unit Link Clock DCG Mode */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 pxlsuldcgm : 1; /**< PXLS Unit Link Clock DCG Mode */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 pxcp2uldcgm : 1; /**< PXCP2 Unit Link Clock DCG Mode */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 rsvd_1 : 3; /**< Undefined - auto filled rsvd_[15:13] */
                            /* Bits[15:13], default=0*/
    UINT32 pxtruldcgm : 1; /**< PXTR Unit Link Clock DCG Mode */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 pxtrsuldcgm : 1; /**< PXTRS Unit Link Clock DCG Mode */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 pxtrssuldcgm : 1; /**< PXTRSS Unit Link Clock DCG Mode */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 pxcp3uldcgm : 1; /**< PXCP3 Unit Link Clock DCG Mode */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 rsvd_2 : 4; /**< Undefined - auto filled rsvd_[23:20] */
                            /* Bits[23:20], default=0*/
    UINT32 pxttuldcgm : 1; /**< PXTT Unit Link Clock DCG Mode */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 pxttsuldcgm : 1; /**< PXTTS Unit Link Clock PXTTSS DCG Mode */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 pxttssuldcgm : 1; /**< PXTTS Unit Link Clock DCG Mode */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 pxcp4uldcgm : 1; /**< PXCP4 Unit Link Clock DCG Mode */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 rsvd_3 : 3; /**< Undefined - auto filled rsvd_[30:28] */
                            /* Bits[30:28], default=0*/
    UINT32 rsvd_m : 1; /**< Reserved */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DCGM1_PCIE_MEM_RCRB_STRUCT;

#define DCGM1_PCIE_MEM_RCRB_WIDTH 32
#define DCGM1_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define DCGM1_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define DCGM1_PCIE_MEM_RCRB_PXKGULDCGM_LSB 0x0000
#define DCGM1_PCIE_MEM_RCRB_PXKGULDCGM_SIZE 0x0001
#define DCGM1_PCIE_MEM_RCRB_PXCULDCGM_LSB 0x0001
#define DCGM1_PCIE_MEM_RCRB_PXCULDCGM_SIZE 0x0001
#define DCGM1_PCIE_MEM_RCRB_PXLRULDCGM_LSB 0x0008
#define DCGM1_PCIE_MEM_RCRB_PXLRULDCGM_SIZE 0x0001
#define DCGM1_PCIE_MEM_RCRB_PXLTULDCGM_LSB 0x0009
#define DCGM1_PCIE_MEM_RCRB_PXLTULDCGM_SIZE 0x0001
#define DCGM1_PCIE_MEM_RCRB_PXLIULDCGM_LSB 0x000a
#define DCGM1_PCIE_MEM_RCRB_PXLIULDCGM_SIZE 0x0001
#define DCGM1_PCIE_MEM_RCRB_PXLSULDCGM_LSB 0x000b
#define DCGM1_PCIE_MEM_RCRB_PXLSULDCGM_SIZE 0x0001
#define DCGM1_PCIE_MEM_RCRB_PXCP2ULDCGM_LSB 0x000c
#define DCGM1_PCIE_MEM_RCRB_PXCP2ULDCGM_SIZE 0x0001
#define DCGM1_PCIE_MEM_RCRB_PXTRULDCGM_LSB 0x0010
#define DCGM1_PCIE_MEM_RCRB_PXTRULDCGM_SIZE 0x0001
#define DCGM1_PCIE_MEM_RCRB_PXTRSULDCGM_LSB 0x0011
#define DCGM1_PCIE_MEM_RCRB_PXTRSULDCGM_SIZE 0x0001
#define DCGM1_PCIE_MEM_RCRB_PXTRSSULDCGM_LSB 0x0012
#define DCGM1_PCIE_MEM_RCRB_PXTRSSULDCGM_SIZE 0x0001
#define DCGM1_PCIE_MEM_RCRB_PXCP3ULDCGM_LSB 0x0013
#define DCGM1_PCIE_MEM_RCRB_PXCP3ULDCGM_SIZE 0x0001
#define DCGM1_PCIE_MEM_RCRB_PXTTULDCGM_LSB 0x0018
#define DCGM1_PCIE_MEM_RCRB_PXTTULDCGM_SIZE 0x0001
#define DCGM1_PCIE_MEM_RCRB_PXTTSULDCGM_LSB 0x0019
#define DCGM1_PCIE_MEM_RCRB_PXTTSULDCGM_SIZE 0x0001
#define DCGM1_PCIE_MEM_RCRB_PXTTSSULDCGM_LSB 0x001a
#define DCGM1_PCIE_MEM_RCRB_PXTTSSULDCGM_SIZE 0x0001
#define DCGM1_PCIE_MEM_RCRB_PXCP4ULDCGM_LSB 0x001b
#define DCGM1_PCIE_MEM_RCRB_PXCP4ULDCGM_SIZE 0x0001
#define DCGM1_PCIE_MEM_RCRB_RSVD_M_LSB 0x001f
#define DCGM1_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0001



/** DCGM2 desc:
  */

#define DCGM2_PCIE_MEM_RCRB_REG          (0x0000035cU)

typedef union {
  struct {
    UINT32 pxpiuldcgm : 1; /**< PXPI Unit Link Clock DCG Mode */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 pxpsuldcgm : 1; /**< PXPS Unit Link Clock DCG Mode */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 pxpbuldcgm : 1; /**< PXPB Unit Link Clock DCG Mode */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 pxfiuldcgm : 1; /**< PXFI Unit Link Clock DCG Mode */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 pxftuldcgm : 1; /**< PXFT Unit Link Clock DCG Mode */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 pxfruldcgm : 1; /**< PXFR Unit Link Clock DCG Mode */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 pxcp5uldcgm : 1; /**< PXCP5 Unit Link Clock DCG Mode */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 rsvd_0 : 25; /**< Undefined - auto filled rsvd_[31:07] */
                            /* Bits[31:7], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DCGM2_PCIE_MEM_RCRB_STRUCT;

#define DCGM2_PCIE_MEM_RCRB_WIDTH 32
#define DCGM2_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define DCGM2_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define DCGM2_PCIE_MEM_RCRB_PXPIULDCGM_LSB 0x0000
#define DCGM2_PCIE_MEM_RCRB_PXPIULDCGM_SIZE 0x0001
#define DCGM2_PCIE_MEM_RCRB_PXPSULDCGM_LSB 0x0001
#define DCGM2_PCIE_MEM_RCRB_PXPSULDCGM_SIZE 0x0001
#define DCGM2_PCIE_MEM_RCRB_PXPBULDCGM_LSB 0x0002
#define DCGM2_PCIE_MEM_RCRB_PXPBULDCGM_SIZE 0x0001
#define DCGM2_PCIE_MEM_RCRB_PXFIULDCGM_LSB 0x0003
#define DCGM2_PCIE_MEM_RCRB_PXFIULDCGM_SIZE 0x0001
#define DCGM2_PCIE_MEM_RCRB_PXFTULDCGM_LSB 0x0004
#define DCGM2_PCIE_MEM_RCRB_PXFTULDCGM_SIZE 0x0001
#define DCGM2_PCIE_MEM_RCRB_PXFRULDCGM_LSB 0x0005
#define DCGM2_PCIE_MEM_RCRB_PXFRULDCGM_SIZE 0x0001
#define DCGM2_PCIE_MEM_RCRB_PXCP5ULDCGM_LSB 0x0006
#define DCGM2_PCIE_MEM_RCRB_PXCP5ULDCGM_SIZE 0x0001



/** DCGEN3 desc:
  */

#define DCGEN3_PCIE_MEM_RCRB_REG         (0x00000360U)

typedef union {
  struct {
    UINT32 pxeupdcgen : 1; /**< PXE Unit Prim Clock DCG Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 pxbupdcgen : 1; /**< PXB Unit Prim Clock DCG Enable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 pxcupdcgen : 1; /**< PXC Unit Prim Port Clock DCG Enable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[03:03] */
                            /* Bits[3:3], default=0*/
    UINT32 pxcupsrcdcgen : 1; /**< PXC Unit Prim Shared Resource Clock DCG Enable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 pxcupsnrdcgen : 1; /**< PXC Unit Prim SnR DCG Enable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 pxsrussnrdcgen : 1; /**< PXSR Unit Side SnR DCG Enable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[07:07] */
                            /* Bits[7:7], default=0*/
    UINT32 pxlrupdcgen : 1; /**< PXLR Unit Prim Clock DCG Enable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 pxltupdcgen : 1; /**< PXLT Unit Prim Clock DCG Enable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 pxliupdcgen : 1; /**< PXLI Unit Prim Clock DCG Enable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[11:11] */
                            /* Bits[11:11], default=0*/
    UINT32 pxcp2updcgen : 1; /**< PXCP2 Unit Prim Clock DCG Enable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 rsvd_3 : 3; /**< Undefined - auto filled rsvd_[15:13] */
                            /* Bits[15:13], default=0*/
    UINT32 pxtrupdcgen : 1; /**< PXTR Unit Prim Clock DCG Enable */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 pxtrsupdcgen : 1; /**< PXTRS Unit Prim Clock DCG Enable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 pxtrssupdcgen : 1; /**< PXTRSS Unit Prim Clock DCG Enable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 pxtoupdcgen : 1; /**< PXTO Unit Prim Clock DCG Enable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 pxcp3updcgen : 1; /**< PXCP3 Unit Prim Clock DCG Enable */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 rsvd_4 : 3; /**< Undefined - auto filled rsvd_[23:21] */
                            /* Bits[23:21], default=0*/
    UINT32 pxttupdcgen : 1; /**< PXTT Unit Prim Clock DCG Enable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 pxttsupdcgen : 1; /**< PXTTS Unit Prim Clock DCG Enable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 pxttssupdcgen : 1; /**< PXTTSS Unit Prim Clock DCG Enable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 pxcp4updcgen : 1; /**< PXCP4 Unit Prim Clock DCG Enable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 rsvd_5 : 3; /**< Undefined - auto filled rsvd_[30:28] */
                            /* Bits[30:28], default=0*/
    UINT32 rsvd_m : 1; /**< Reserved */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DCGEN3_PCIE_MEM_RCRB_STRUCT;

#define DCGEN3_PCIE_MEM_RCRB_WIDTH 32
#define DCGEN3_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define DCGEN3_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define DCGEN3_PCIE_MEM_RCRB_PXEUPDCGEN_LSB 0x0000
#define DCGEN3_PCIE_MEM_RCRB_PXEUPDCGEN_SIZE 0x0001
#define DCGEN3_PCIE_MEM_RCRB_PXBUPDCGEN_LSB 0x0001
#define DCGEN3_PCIE_MEM_RCRB_PXBUPDCGEN_SIZE 0x0001
#define DCGEN3_PCIE_MEM_RCRB_PXCUPDCGEN_LSB 0x0002
#define DCGEN3_PCIE_MEM_RCRB_PXCUPDCGEN_SIZE 0x0001
#define DCGEN3_PCIE_MEM_RCRB_PXCUPSRCDCGEN_LSB 0x0004
#define DCGEN3_PCIE_MEM_RCRB_PXCUPSRCDCGEN_SIZE 0x0001
#define DCGEN3_PCIE_MEM_RCRB_PXCUPSNRDCGEN_LSB 0x0005
#define DCGEN3_PCIE_MEM_RCRB_PXCUPSNRDCGEN_SIZE 0x0001
#define DCGEN3_PCIE_MEM_RCRB_PXSRUSSNRDCGEN_LSB 0x0006
#define DCGEN3_PCIE_MEM_RCRB_PXSRUSSNRDCGEN_SIZE 0x0001
#define DCGEN3_PCIE_MEM_RCRB_PXLRUPDCGEN_LSB 0x0008
#define DCGEN3_PCIE_MEM_RCRB_PXLRUPDCGEN_SIZE 0x0001
#define DCGEN3_PCIE_MEM_RCRB_PXLTUPDCGEN_LSB 0x0009
#define DCGEN3_PCIE_MEM_RCRB_PXLTUPDCGEN_SIZE 0x0001
#define DCGEN3_PCIE_MEM_RCRB_PXLIUPDCGEN_LSB 0x000a
#define DCGEN3_PCIE_MEM_RCRB_PXLIUPDCGEN_SIZE 0x0001
#define DCGEN3_PCIE_MEM_RCRB_PXCP2UPDCGEN_LSB 0x000c
#define DCGEN3_PCIE_MEM_RCRB_PXCP2UPDCGEN_SIZE 0x0001
#define DCGEN3_PCIE_MEM_RCRB_PXTRUPDCGEN_LSB 0x0010
#define DCGEN3_PCIE_MEM_RCRB_PXTRUPDCGEN_SIZE 0x0001
#define DCGEN3_PCIE_MEM_RCRB_PXTRSUPDCGEN_LSB 0x0011
#define DCGEN3_PCIE_MEM_RCRB_PXTRSUPDCGEN_SIZE 0x0001
#define DCGEN3_PCIE_MEM_RCRB_PXTRSSUPDCGEN_LSB 0x0012
#define DCGEN3_PCIE_MEM_RCRB_PXTRSSUPDCGEN_SIZE 0x0001
#define DCGEN3_PCIE_MEM_RCRB_PXTOUPDCGEN_LSB 0x0013
#define DCGEN3_PCIE_MEM_RCRB_PXTOUPDCGEN_SIZE 0x0001
#define DCGEN3_PCIE_MEM_RCRB_PXCP3UPDCGEN_LSB 0x0014
#define DCGEN3_PCIE_MEM_RCRB_PXCP3UPDCGEN_SIZE 0x0001
#define DCGEN3_PCIE_MEM_RCRB_PXTTUPDCGEN_LSB 0x0018
#define DCGEN3_PCIE_MEM_RCRB_PXTTUPDCGEN_SIZE 0x0001
#define DCGEN3_PCIE_MEM_RCRB_PXTTSUPDCGEN_LSB 0x0019
#define DCGEN3_PCIE_MEM_RCRB_PXTTSUPDCGEN_SIZE 0x0001
#define DCGEN3_PCIE_MEM_RCRB_PXTTSSUPDCGEN_LSB 0x001a
#define DCGEN3_PCIE_MEM_RCRB_PXTTSSUPDCGEN_SIZE 0x0001
#define DCGEN3_PCIE_MEM_RCRB_PXCP4UPDCGEN_LSB 0x001b
#define DCGEN3_PCIE_MEM_RCRB_PXCP4UPDCGEN_SIZE 0x0001
#define DCGEN3_PCIE_MEM_RCRB_RSVD_M_LSB 0x001f
#define DCGEN3_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0001



/** DCGM3 desc:
  */

#define DCGM3_PCIE_MEM_RCRB_REG          (0x00000368U)

typedef union {
  struct {
    UINT32 pxeupdcgm : 1; /**< PXE Unit Prim Clock DCG Mode */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 pxbupdcgm : 1; /**< PXB Unit Prim Clock DCG Mode */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 pxcupdcgm : 1; /**< PXC Unit Prim Clock DCG Mode */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[03:03] */
                            /* Bits[3:3], default=0*/
    UINT32 pxcupsrcdcgm : 1; /**< PXC Unit Prim Shared Resource Clock DCG Mode */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 pxcupsnrdcgm : 1; /**< PXC Unit Prim SnR DCG Mode */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 pxsrussnrdcgm : 1; /**< PXSR Unit Side SnR DCG Mode */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[07:07] */
                            /* Bits[7:7], default=0*/
    UINT32 pxlrupdcgm : 1; /**< PXLR Unit Prim Clock DCG Mode */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 pxltupdcgm : 1; /**< PXLT Unit Prim Clock DCG Mode */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 pxliupdcgm : 1; /**< PXLI Unit Prim Clock DCG Mode */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[11:11] */
                            /* Bits[11:11], default=0*/
    UINT32 pxcp2updcgm : 1; /**< PXCP2 Unit Prim Clock DCG Mode */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 rsvd_3 : 3; /**< Undefined - auto filled rsvd_[15:13] */
                            /* Bits[15:13], default=0*/
    UINT32 pxtrupdcgm : 1; /**< PXTR Unit Prim Clock DCG Mode */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 pxtrsupdcgm : 1; /**< PXTRS Unit Prim Clock DCG Mode */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 pxtrssupdcgm : 1; /**< PXTRSS Unit Prim Clock DCG Mode */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 pxtoupdcgm : 1; /**< PXTO Unit Prim Clock DCG Mode */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 pxcp3updcgm : 1; /**< PXCP3 Unit Prim Clock DCG Mode */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 rsvd_4 : 3; /**< Undefined - auto filled rsvd_[23:21] */
                            /* Bits[23:21], default=0*/
    UINT32 pxttupdcgm : 1; /**< PXTT Unit Prim Clock DCG Mode */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 pxttsupdcgm : 1; /**< PXTTS Unit Prim Clock DCG Mode */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 pxttssupdcgm : 1; /**< PXTTSS Unit Prim Clock DCG Mode */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 pxcp4updcgm : 1; /**< PXCP4 Unit Prim Clock DCG Mode */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 rsvd_5 : 3; /**< Undefined - auto filled rsvd_[30:28] */
                            /* Bits[30:28], default=0*/
    UINT32 rsvd_m : 1; /**< Reserved */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DCGM3_PCIE_MEM_RCRB_STRUCT;

#define DCGM3_PCIE_MEM_RCRB_WIDTH 32
#define DCGM3_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define DCGM3_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define DCGM3_PCIE_MEM_RCRB_PXEUPDCGM_LSB 0x0000
#define DCGM3_PCIE_MEM_RCRB_PXEUPDCGM_SIZE 0x0001
#define DCGM3_PCIE_MEM_RCRB_PXBUPDCGM_LSB 0x0001
#define DCGM3_PCIE_MEM_RCRB_PXBUPDCGM_SIZE 0x0001
#define DCGM3_PCIE_MEM_RCRB_PXCUPDCGM_LSB 0x0002
#define DCGM3_PCIE_MEM_RCRB_PXCUPDCGM_SIZE 0x0001
#define DCGM3_PCIE_MEM_RCRB_PXCUPSRCDCGM_LSB 0x0004
#define DCGM3_PCIE_MEM_RCRB_PXCUPSRCDCGM_SIZE 0x0001
#define DCGM3_PCIE_MEM_RCRB_PXCUPSNRDCGM_LSB 0x0005
#define DCGM3_PCIE_MEM_RCRB_PXCUPSNRDCGM_SIZE 0x0001
#define DCGM3_PCIE_MEM_RCRB_PXSRUSSNRDCGM_LSB 0x0006
#define DCGM3_PCIE_MEM_RCRB_PXSRUSSNRDCGM_SIZE 0x0001
#define DCGM3_PCIE_MEM_RCRB_PXLRUPDCGM_LSB 0x0008
#define DCGM3_PCIE_MEM_RCRB_PXLRUPDCGM_SIZE 0x0001
#define DCGM3_PCIE_MEM_RCRB_PXLTUPDCGM_LSB 0x0009
#define DCGM3_PCIE_MEM_RCRB_PXLTUPDCGM_SIZE 0x0001
#define DCGM3_PCIE_MEM_RCRB_PXLIUPDCGM_LSB 0x000a
#define DCGM3_PCIE_MEM_RCRB_PXLIUPDCGM_SIZE 0x0001
#define DCGM3_PCIE_MEM_RCRB_PXCP2UPDCGM_LSB 0x000c
#define DCGM3_PCIE_MEM_RCRB_PXCP2UPDCGM_SIZE 0x0001
#define DCGM3_PCIE_MEM_RCRB_PXTRUPDCGM_LSB 0x0010
#define DCGM3_PCIE_MEM_RCRB_PXTRUPDCGM_SIZE 0x0001
#define DCGM3_PCIE_MEM_RCRB_PXTRSUPDCGM_LSB 0x0011
#define DCGM3_PCIE_MEM_RCRB_PXTRSUPDCGM_SIZE 0x0001
#define DCGM3_PCIE_MEM_RCRB_PXTRSSUPDCGM_LSB 0x0012
#define DCGM3_PCIE_MEM_RCRB_PXTRSSUPDCGM_SIZE 0x0001
#define DCGM3_PCIE_MEM_RCRB_PXTOUPDCGM_LSB 0x0013
#define DCGM3_PCIE_MEM_RCRB_PXTOUPDCGM_SIZE 0x0001
#define DCGM3_PCIE_MEM_RCRB_PXCP3UPDCGM_LSB 0x0014
#define DCGM3_PCIE_MEM_RCRB_PXCP3UPDCGM_SIZE 0x0001
#define DCGM3_PCIE_MEM_RCRB_PXTTUPDCGM_LSB 0x0018
#define DCGM3_PCIE_MEM_RCRB_PXTTUPDCGM_SIZE 0x0001
#define DCGM3_PCIE_MEM_RCRB_PXTTSUPDCGM_LSB 0x0019
#define DCGM3_PCIE_MEM_RCRB_PXTTSUPDCGM_SIZE 0x0001
#define DCGM3_PCIE_MEM_RCRB_PXTTSSUPDCGM_LSB 0x001a
#define DCGM3_PCIE_MEM_RCRB_PXTTSSUPDCGM_SIZE 0x0001
#define DCGM3_PCIE_MEM_RCRB_PXCP4UPDCGM_LSB 0x001b
#define DCGM3_PCIE_MEM_RCRB_PXCP4UPDCGM_SIZE 0x0001
#define DCGM3_PCIE_MEM_RCRB_RSVD_M_LSB 0x001f
#define DCGM3_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0001



/** IPCLKCTR desc:
  */

#define IPCLKCTR_PCIE_MEM_RCRB_REG       (0x00000370U)

typedef union {
  struct {
    UINT32 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[01:00] */
                            /* Bits[1:0], default=0*/
    UINT32 pxdcge : 1; /**< PXD Clock Gate Enable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 mdpccen : 1; /**< Mild DCG Prim Clock Coupling Enable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 itcght : 2; /**< ITCG Hystersis Timer */
                            /* Bits[5:4], default=0x00000000*/
    UINT32 rsvd_1 : 10; /**< Undefined - auto filled rsvd_[15:06] */
                            /* Bits[15:6], default=0*/
    UINT32 pfg12pllsw : 8; /**< Per-Family Gen1/2 PLL Survivability Wake */
                            /* Bits[23:16], default=0x00000000*/
    UINT32 pfg3pllsw : 8; /**< Per-Family Gen3 PLL Survivability Wake */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} IPCLKCTR_PCIE_MEM_RCRB_STRUCT;

#define IPCLKCTR_PCIE_MEM_RCRB_WIDTH 32
#define IPCLKCTR_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define IPCLKCTR_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define IPCLKCTR_PCIE_MEM_RCRB_PXDCGE_LSB 0x0002
#define IPCLKCTR_PCIE_MEM_RCRB_PXDCGE_SIZE 0x0001
#define IPCLKCTR_PCIE_MEM_RCRB_MDPCCEN_LSB 0x0003
#define IPCLKCTR_PCIE_MEM_RCRB_MDPCCEN_SIZE 0x0001
#define IPCLKCTR_PCIE_MEM_RCRB_ITCGHT_LSB 0x0004
#define IPCLKCTR_PCIE_MEM_RCRB_ITCGHT_SIZE 0x0002
#define IPCLKCTR_PCIE_MEM_RCRB_PFG12PLLSW_LSB 0x0010
#define IPCLKCTR_PCIE_MEM_RCRB_PFG12PLLSW_SIZE 0x0008
#define IPCLKCTR_PCIE_MEM_RCRB_PFG3PLLSW_LSB 0x0018
#define IPCLKCTR_PCIE_MEM_RCRB_PFG3PLLSW_SIZE 0x0008



/** LPHYCP1 desc:
  */

#define LPHYCP1_PCIE_MEM_RCRB_REG        (0x00000410U)

typedef union {
  struct {
    UINT32 d8b10bcdntsc : 1; /**< 8b10b COM Dskew N-TS Count */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 d128b130bedntsc : 1; /**< 128b130B EIEOS Dskew N-TS Count */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 rxadpsvh : 1; /**< Receiver Adaptation Status Violation Handling */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 rxadphm : 1; /**< Receiver Adaptation Handshake Mapping */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 rxeqfnevc : 1; /**< Receiver Equalization Final Evaluation Control */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 l1lowcdpc : 1; /**< L1.Low CLKREQ Driver Policy Control */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 pipembip : 1; /**< PIPE Message Bus Initialization Policy */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[08:07] */
                            /* Bits[8:7], default=0*/
    UINT32 rxadprece : 1; /**< Receiver Adaptation Recovery Enable */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rxadprecp : 2; /**< Receiver Adaptation Recovery Policy */
                            /* Bits[11:10], default=0x00000000*/
    UINT32 bprxstndbyhsrecal : 1; /**< Bypass RxStandby Handshake during Recal */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 p2ipclkreqe : 1; /**< Pin To IP CLKREQ Override Enable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 p2ipclkreqv : 1; /**< Pin To IP CLKREQ Override Value */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 ip2pclkreqe : 1; /**< IP To Pin CLKREQ Override Enable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 ip2pclkreqv : 1; /**< IP To Pin CLKREQ Override Value */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 hawdtwud : 1; /**< HAWD Trigger Width Upconfigure Disable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 hawdtwup : 1; /**< HAWD Trigger Width Upconfigure Policy */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 awccwcp : 1; /**< AWCC Width Change Policy */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 hawdwul1ep : 1; /**< HAWD Width Upconfigure L1 Exit Policy */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 rsvd_m : 11; /**< Reserved */
                            /* Bits[31:21], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} LPHYCP1_PCIE_MEM_RCRB_STRUCT;

#define LPHYCP1_PCIE_MEM_RCRB_WIDTH 32
#define LPHYCP1_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define LPHYCP1_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define LPHYCP1_PCIE_MEM_RCRB_D8B10BCDNTSC_LSB 0x0000
#define LPHYCP1_PCIE_MEM_RCRB_D8B10BCDNTSC_SIZE 0x0001
#define LPHYCP1_PCIE_MEM_RCRB_D128B130BEDNTSC_LSB 0x0001
#define LPHYCP1_PCIE_MEM_RCRB_D128B130BEDNTSC_SIZE 0x0001
#define LPHYCP1_PCIE_MEM_RCRB_RXADPSVH_LSB 0x0002
#define LPHYCP1_PCIE_MEM_RCRB_RXADPSVH_SIZE 0x0001
#define LPHYCP1_PCIE_MEM_RCRB_RXADPHM_LSB 0x0003
#define LPHYCP1_PCIE_MEM_RCRB_RXADPHM_SIZE 0x0001
#define LPHYCP1_PCIE_MEM_RCRB_RXEQFNEVC_LSB 0x0004
#define LPHYCP1_PCIE_MEM_RCRB_RXEQFNEVC_SIZE 0x0001
#define LPHYCP1_PCIE_MEM_RCRB_L1LOWCDPC_LSB 0x0005
#define LPHYCP1_PCIE_MEM_RCRB_L1LOWCDPC_SIZE 0x0001
#define LPHYCP1_PCIE_MEM_RCRB_PIPEMBIP_LSB 0x0006
#define LPHYCP1_PCIE_MEM_RCRB_PIPEMBIP_SIZE 0x0001
#define LPHYCP1_PCIE_MEM_RCRB_RXADPRECE_LSB 0x0009
#define LPHYCP1_PCIE_MEM_RCRB_RXADPRECE_SIZE 0x0001
#define LPHYCP1_PCIE_MEM_RCRB_RXADPRECP_LSB 0x000a
#define LPHYCP1_PCIE_MEM_RCRB_RXADPRECP_SIZE 0x0002
#define LPHYCP1_PCIE_MEM_RCRB_BPRXSTNDBYHSRECAL_LSB 0x000c
#define LPHYCP1_PCIE_MEM_RCRB_BPRXSTNDBYHSRECAL_SIZE 0x0001
#define LPHYCP1_PCIE_MEM_RCRB_P2IPCLKREQE_LSB 0x000d
#define LPHYCP1_PCIE_MEM_RCRB_P2IPCLKREQE_SIZE 0x0001
#define LPHYCP1_PCIE_MEM_RCRB_P2IPCLKREQV_LSB 0x000e
#define LPHYCP1_PCIE_MEM_RCRB_P2IPCLKREQV_SIZE 0x0001
#define LPHYCP1_PCIE_MEM_RCRB_IP2PCLKREQE_LSB 0x000f
#define LPHYCP1_PCIE_MEM_RCRB_IP2PCLKREQE_SIZE 0x0001
#define LPHYCP1_PCIE_MEM_RCRB_IP2PCLKREQV_LSB 0x0010
#define LPHYCP1_PCIE_MEM_RCRB_IP2PCLKREQV_SIZE 0x0001
#define LPHYCP1_PCIE_MEM_RCRB_HAWDTWUD_LSB 0x0011
#define LPHYCP1_PCIE_MEM_RCRB_HAWDTWUD_SIZE 0x0001
#define LPHYCP1_PCIE_MEM_RCRB_HAWDTWUP_LSB 0x0012
#define LPHYCP1_PCIE_MEM_RCRB_HAWDTWUP_SIZE 0x0001
#define LPHYCP1_PCIE_MEM_RCRB_AWCCWCP_LSB 0x0013
#define LPHYCP1_PCIE_MEM_RCRB_AWCCWCP_SIZE 0x0001
#define LPHYCP1_PCIE_MEM_RCRB_HAWDWUL1EP_LSB 0x0014
#define LPHYCP1_PCIE_MEM_RCRB_HAWDWUL1EP_SIZE 0x0001
#define LPHYCP1_PCIE_MEM_RCRB_RSVD_M_LSB 0x0015
#define LPHYCP1_PCIE_MEM_RCRB_RSVD_M_SIZE 0x000b



/** RXL0SESQCP4 desc:
  */

#define RXL0SESQCP4_PCIE_MEM_RCRB_REG    (0x0000042cU)

typedef union {
  struct {
    UINT32 g1lbwsst : 8; /**< Gen 1 Low Bandwidth Squelch Settling Timer */
                            /* Bits[7:0], default=0x00000000*/
    UINT32 g2lbwsst : 8; /**< Gen 2 Low Bandwidth Squelch Settling Timer */
                            /* Bits[15:8], default=0x00000000*/
    UINT32 g3lbwsst : 8; /**< Gen 3 Low Bandwidth Squelch Settling Timer */
                            /* Bits[23:16], default=0x00000000*/
    UINT32 g4lbwsst : 8; /**< Gen 4 Low Bandwidth Squelch Settling Timer */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RXL0SESQCP4_PCIE_MEM_RCRB_STRUCT;

#define RXL0SESQCP4_PCIE_MEM_RCRB_WIDTH 32
#define RXL0SESQCP4_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define RXL0SESQCP4_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define RXL0SESQCP4_PCIE_MEM_RCRB_G1LBWSST_LSB 0x0000
#define RXL0SESQCP4_PCIE_MEM_RCRB_G1LBWSST_SIZE 0x0008
#define RXL0SESQCP4_PCIE_MEM_RCRB_G2LBWSST_LSB 0x0008
#define RXL0SESQCP4_PCIE_MEM_RCRB_G2LBWSST_SIZE 0x0008
#define RXL0SESQCP4_PCIE_MEM_RCRB_G3LBWSST_LSB 0x0010
#define RXL0SESQCP4_PCIE_MEM_RCRB_G3LBWSST_SIZE 0x0008
#define RXL0SESQCP4_PCIE_MEM_RCRB_G4LBWSST_LSB 0x0018
#define RXL0SESQCP4_PCIE_MEM_RCRB_G4LBWSST_SIZE 0x0008



/** RXL0SESQCP5 desc:
  */

#define RXL0SESQCP5_PCIE_MEM_RCRB_REG    (0x00000430U)

typedef union {
  struct {
    UINT32 g5lbwsst : 8; /**< Gen 5 Low Bandwidth Squelch Settling Timer */
                            /* Bits[7:0], default=0x00000000*/
    UINT32 rsvd_m : 24; /**< Reserved */
                            /* Bits[31:8], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RXL0SESQCP5_PCIE_MEM_RCRB_STRUCT;

#define RXL0SESQCP5_PCIE_MEM_RCRB_WIDTH 32
#define RXL0SESQCP5_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define RXL0SESQCP5_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define RXL0SESQCP5_PCIE_MEM_RCRB_G5LBWSST_LSB 0x0000
#define RXL0SESQCP5_PCIE_MEM_RCRB_G5LBWSST_SIZE 0x0008
#define RXL0SESQCP5_PCIE_MEM_RCRB_RSVD_M_LSB 0x0008
#define RXL0SESQCP5_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0018



/** LPHYCP4 desc:
  */

#define LPHYCP4_PCIE_MEM_RCRB_REG        (0x00000434U)

typedef union {
  struct {
    UINT32 lgclksqexitdbtimers : 3; /**< Link Clock Domain Squelch Exit Debounce Timers */
                            /* Bits[2:0], default=0x00000000*/
    UINT32 oscclksqexitdbtimers : 3; /**< OSC Clock Domain Squelch Exit Debounce Timers */
                            /* Bits[5:3], default=0x00000000*/
    UINT32 lgussp : 2; /**< LG Clock Un-Squelch Sampling Period */
                            /* Bits[7:6], default=0x00000002*/
    UINT32 osussp : 2; /**< OSC Clock Un-Squelch Sampling Period */
                            /* Bits[9:8], default=0x00000002*/
    UINT32 pllbusdrc : 1; /**< PLL Bring Up Sequence During Rate Change */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 reefrxl0sed : 1; /**< RxElecidle Exit Evaluation For RxL0s Entry Disable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 dpmdndbfe : 1; /**< Data Parity Mask During Non Data Block For 128b130b Encoding */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 tlspdrs : 1; /**< Target Link Speed Policy During Recovery Substates */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[15:14] */
                            /* Bits[15:14], default=0*/
    UINT32 g1aredrl0s : 1; /**< Gen 1 Async RxElecidle Detector For RXL0s */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 g2aredrl0s : 1; /**< Gen 2 Async RxElecidle Detector For RXL0s */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 g3aredrl0s : 1; /**< Gen 3 Async RxElecidle Detector For RXL0s */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 g4aredrl0s : 1; /**< Gen 4 Async RxElecidle Detector For RXL0s */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 g5aredrl0s : 1; /**< Gen 5 Async RxElecidle Detector For RXL0s */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 rxl0sewrvld : 2; /**< RXL0s Exit With RX Valid Assertion Policy */
                            /* Bits[22:21], default=0x00000002*/
    UINT32 rxl0sexbpron : 1; /**< RxL0s Exit to Bypass Phy Receive On Policy */
                            /* Bits[23:23], default=0x00000001*/
    UINT32 rxl0seg12ftse : 2; /**< RxL0s Exit Gen 1/2 FTS Timeout Extension */
                            /* Bits[25:24], default=0x00000000*/
    UINT32 rxvldrxl0ssp : 3; /**< RX Valid Trigger RXL0s Exit Suppression Period */
                            /* Bits[28:26], default=0x00000000*/
    UINT32 rsvd_m : 3; /**< Reserved */
                            /* Bits[31:29], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} LPHYCP4_PCIE_MEM_RCRB_STRUCT;

#define LPHYCP4_PCIE_MEM_RCRB_WIDTH 32
#define LPHYCP4_PCIE_MEM_RCRB_DEFAULT 0x00c00280U
#define LPHYCP4_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define LPHYCP4_PCIE_MEM_RCRB_LGCLKSQEXITDBTIMERS_LSB 0x0000
#define LPHYCP4_PCIE_MEM_RCRB_LGCLKSQEXITDBTIMERS_SIZE 0x0003
#define LPHYCP4_PCIE_MEM_RCRB_OSCCLKSQEXITDBTIMERS_LSB 0x0003
#define LPHYCP4_PCIE_MEM_RCRB_OSCCLKSQEXITDBTIMERS_SIZE 0x0003
#define LPHYCP4_PCIE_MEM_RCRB_LGUSSP_LSB 0x0006
#define LPHYCP4_PCIE_MEM_RCRB_LGUSSP_SIZE 0x0002
#define LPHYCP4_PCIE_MEM_RCRB_OSUSSP_LSB 0x0008
#define LPHYCP4_PCIE_MEM_RCRB_OSUSSP_SIZE 0x0002
#define LPHYCP4_PCIE_MEM_RCRB_PLLBUSDRC_LSB 0x000a
#define LPHYCP4_PCIE_MEM_RCRB_PLLBUSDRC_SIZE 0x0001
#define LPHYCP4_PCIE_MEM_RCRB_REEFRXL0SED_LSB 0x000b
#define LPHYCP4_PCIE_MEM_RCRB_REEFRXL0SED_SIZE 0x0001
#define LPHYCP4_PCIE_MEM_RCRB_DPMDNDBFE_LSB 0x000c
#define LPHYCP4_PCIE_MEM_RCRB_DPMDNDBFE_SIZE 0x0001
#define LPHYCP4_PCIE_MEM_RCRB_TLSPDRS_LSB 0x000d
#define LPHYCP4_PCIE_MEM_RCRB_TLSPDRS_SIZE 0x0001
#define LPHYCP4_PCIE_MEM_RCRB_G1AREDRL0S_LSB 0x0010
#define LPHYCP4_PCIE_MEM_RCRB_G1AREDRL0S_SIZE 0x0001
#define LPHYCP4_PCIE_MEM_RCRB_G2AREDRL0S_LSB 0x0011
#define LPHYCP4_PCIE_MEM_RCRB_G2AREDRL0S_SIZE 0x0001
#define LPHYCP4_PCIE_MEM_RCRB_G3AREDRL0S_LSB 0x0012
#define LPHYCP4_PCIE_MEM_RCRB_G3AREDRL0S_SIZE 0x0001
#define LPHYCP4_PCIE_MEM_RCRB_G4AREDRL0S_LSB 0x0013
#define LPHYCP4_PCIE_MEM_RCRB_G4AREDRL0S_SIZE 0x0001
#define LPHYCP4_PCIE_MEM_RCRB_G5AREDRL0S_LSB 0x0014
#define LPHYCP4_PCIE_MEM_RCRB_G5AREDRL0S_SIZE 0x0001
#define LPHYCP4_PCIE_MEM_RCRB_RXL0SEWRVLD_LSB 0x0015
#define LPHYCP4_PCIE_MEM_RCRB_RXL0SEWRVLD_SIZE 0x0002
#define LPHYCP4_PCIE_MEM_RCRB_RXL0SEXBPRON_LSB 0x0017
#define LPHYCP4_PCIE_MEM_RCRB_RXL0SEXBPRON_SIZE 0x0001
#define LPHYCP4_PCIE_MEM_RCRB_RXL0SEG12FTSE_LSB 0x0018
#define LPHYCP4_PCIE_MEM_RCRB_RXL0SEG12FTSE_SIZE 0x0002
#define LPHYCP4_PCIE_MEM_RCRB_RXVLDRXL0SSP_LSB 0x001a
#define LPHYCP4_PCIE_MEM_RCRB_RXVLDRXL0SSP_SIZE 0x0003
#define LPHYCP4_PCIE_MEM_RCRB_RSVD_M_LSB 0x001d
#define LPHYCP4_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0003



/** IORCCP1 desc:
  */

#define IORCCP1_PCIE_MEM_RCRB_REG        (0x0000044cU)

typedef union {
  struct {
    UINT32 disorcl12rec : 1; /**< Disable Offset Re-Calibration Request from L1 to Recovery */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 drcorrp : 1; /**< Disable Recovery / Configurations Offset Re-calibration Request Policy */
                            /* Bits[1:1], default=0x00000001*/
    UINT32 disorcrodi : 1; /**< Disable Offset Re-Calibration Request On Downconfigure / Inactive Lanes */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 orts : 1; /**< Offset Recalibrations Trigger Status */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 rsvd_0 : 4; /**< Undefined - auto filled rsvd_[07:04] */
                            /* Bits[7:4], default=0*/
    UINT32 orrpgm : 8; /**< Offset Re-Calibration Request Per Gen Mask */
                            /* Bits[15:8], default=0x00000000*/
    UINT32 g1orrrxecc : 3; /**< Gen 1 Offset Re-Calibrations RX Error Collapsing Count */
                            /* Bits[18:16], default=0x00000000*/
    UINT32 g2orrrxecc : 3; /**< Gen 2 Offset Re-Calibrations RX Error Collapsing Count */
                            /* Bits[21:19], default=0x00000000*/
    UINT32 g3orrrxecc : 3; /**< Gen 3 Offset Re-Calibrations RX Error Collapsing Count */
                            /* Bits[24:22], default=0x00000000*/
    UINT32 g4orrrxecc : 3; /**< Gen 4 Offset Re-Calibrations RX Error Collapsing Count */
                            /* Bits[27:25], default=0x00000000*/
    UINT32 g5orrrxecc : 3; /**< Gen 5 Offset Re-Calibrations RX Error Collapsing Count */
                            /* Bits[30:28], default=0x00000000*/
    UINT32 rsvd_m : 1; /**< Reserved */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} IORCCP1_PCIE_MEM_RCRB_STRUCT;

#define IORCCP1_PCIE_MEM_RCRB_WIDTH 32
#define IORCCP1_PCIE_MEM_RCRB_DEFAULT 0x00000002U
#define IORCCP1_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define IORCCP1_PCIE_MEM_RCRB_DISORCL12REC_LSB 0x0000
#define IORCCP1_PCIE_MEM_RCRB_DISORCL12REC_SIZE 0x0001
#define IORCCP1_PCIE_MEM_RCRB_DRCORRP_LSB 0x0001
#define IORCCP1_PCIE_MEM_RCRB_DRCORRP_SIZE 0x0001
#define IORCCP1_PCIE_MEM_RCRB_DISORCRODI_LSB 0x0002
#define IORCCP1_PCIE_MEM_RCRB_DISORCRODI_SIZE 0x0001
#define IORCCP1_PCIE_MEM_RCRB_ORTS_LSB 0x0003
#define IORCCP1_PCIE_MEM_RCRB_ORTS_SIZE 0x0001
#define IORCCP1_PCIE_MEM_RCRB_ORRPGM_LSB 0x0008
#define IORCCP1_PCIE_MEM_RCRB_ORRPGM_SIZE 0x0008
#define IORCCP1_PCIE_MEM_RCRB_G1ORRRXECC_LSB 0x0010
#define IORCCP1_PCIE_MEM_RCRB_G1ORRRXECC_SIZE 0x0003
#define IORCCP1_PCIE_MEM_RCRB_G2ORRRXECC_LSB 0x0013
#define IORCCP1_PCIE_MEM_RCRB_G2ORRRXECC_SIZE 0x0003
#define IORCCP1_PCIE_MEM_RCRB_G3ORRRXECC_LSB 0x0016
#define IORCCP1_PCIE_MEM_RCRB_G3ORRRXECC_SIZE 0x0003
#define IORCCP1_PCIE_MEM_RCRB_G4ORRRXECC_LSB 0x0019
#define IORCCP1_PCIE_MEM_RCRB_G4ORRRXECC_SIZE 0x0003
#define IORCCP1_PCIE_MEM_RCRB_G5ORRRXECC_LSB 0x001c
#define IORCCP1_PCIE_MEM_RCRB_G5ORRRXECC_SIZE 0x0003
#define IORCCP1_PCIE_MEM_RCRB_RSVD_M_LSB 0x001f
#define IORCCP1_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0001



/** PHYPG desc:
  */

#define PHYPG_PCIE_MEM_RCRB_REG          (0x00000590U)

typedef union {
  struct {
    UINT32 detphypge : 1; /**< Detect PHY Power Gating Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 disphypge : 1; /**< Disabled PHY Power Gating Enable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 l23phypge : 1; /**< L23 PHY Power Gating Enable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 ducfgphypge : 1; /**< Downconfigure PHY Power Gating Enable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 dlppgp : 1; /**< Downconfigure Lanes Phy Power Gating Policy */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 ulppgp : 1; /**< Unconfigure Lanes Phy Power Gating Policy */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 uncfgphypge : 1; /**< Unconfigure PHY Power Gating Enable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 mrfdpmreqa : 3; /**< Min Residency For DeepPmReq Assertion */
                            /* Bits[9:7], default=0x00000000*/
    UINT32 mrfdpmreqda : 3; /**< Min Residency For DeepPmReq De-assertion */
                            /* Bits[12:10], default=0x00000000*/
    UINT32 lnmdlupp0t : 2; /**< Link Number Masking Duration during Lane Upconfiguration after P0 transition */
                            /* Bits[14:13], default=0x00000000*/
    UINT32 sqeetup : 2; /**< Squelch Exit Evaluation Timer for Upconfiguration */
                            /* Bits[16:15], default=0x00000000*/
    UINT32 rsvd_m : 15; /**< Reserved */
                            /* Bits[31:17], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PHYPG_PCIE_MEM_RCRB_STRUCT;

#define PHYPG_PCIE_MEM_RCRB_WIDTH 32
#define PHYPG_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define PHYPG_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define PHYPG_PCIE_MEM_RCRB_DETPHYPGE_LSB 0x0000
#define PHYPG_PCIE_MEM_RCRB_DETPHYPGE_SIZE 0x0001
#define PHYPG_PCIE_MEM_RCRB_DISPHYPGE_LSB 0x0001
#define PHYPG_PCIE_MEM_RCRB_DISPHYPGE_SIZE 0x0001
#define PHYPG_PCIE_MEM_RCRB_L23PHYPGE_LSB 0x0002
#define PHYPG_PCIE_MEM_RCRB_L23PHYPGE_SIZE 0x0001
#define PHYPG_PCIE_MEM_RCRB_DUCFGPHYPGE_LSB 0x0003
#define PHYPG_PCIE_MEM_RCRB_DUCFGPHYPGE_SIZE 0x0001
#define PHYPG_PCIE_MEM_RCRB_DLPPGP_LSB 0x0004
#define PHYPG_PCIE_MEM_RCRB_DLPPGP_SIZE 0x0001
#define PHYPG_PCIE_MEM_RCRB_ULPPGP_LSB 0x0005
#define PHYPG_PCIE_MEM_RCRB_ULPPGP_SIZE 0x0001
#define PHYPG_PCIE_MEM_RCRB_UNCFGPHYPGE_LSB 0x0006
#define PHYPG_PCIE_MEM_RCRB_UNCFGPHYPGE_SIZE 0x0001
#define PHYPG_PCIE_MEM_RCRB_MRFDPMREQA_LSB 0x0007
#define PHYPG_PCIE_MEM_RCRB_MRFDPMREQA_SIZE 0x0003
#define PHYPG_PCIE_MEM_RCRB_MRFDPMREQDA_LSB 0x000a
#define PHYPG_PCIE_MEM_RCRB_MRFDPMREQDA_SIZE 0x0003
#define PHYPG_PCIE_MEM_RCRB_LNMDLUPP0T_LSB 0x000d
#define PHYPG_PCIE_MEM_RCRB_LNMDLUPP0T_SIZE 0x0002
#define PHYPG_PCIE_MEM_RCRB_SQEETUP_LSB 0x000f
#define PHYPG_PCIE_MEM_RCRB_SQEETUP_SIZE 0x0002
#define PHYPG_PCIE_MEM_RCRB_RSVD_M_LSB 0x0011
#define PHYPG_PCIE_MEM_RCRB_RSVD_M_SIZE 0x000f



/** PIPEPDCTL desc:
  */

#define PIPEPDCTL_PCIE_MEM_RCRB_REG      (0x00000594U)

typedef union {
  struct {
    UINT32 detnopgpdctl : 4; /**< Detect without Power Gating Power Down Control */
                            /* Bits[3:0], default=0x00000000*/
    UINT32 detpgpdctl : 4; /**< Detect with Power Gating Power Down Control */
                            /* Bits[7:4], default=0x00000003*/
    UINT32 l23nopgpdctl : 4; /**< L23 without Power Gating Power Down Control */
                            /* Bits[11:8], default=0x00000003*/
    UINT32 l23pgpdctl : 4; /**< L23 with Power Gating Power Down Control */
                            /* Bits[15:12], default=0x00000003*/
    UINT32 disnopgpdctl : 4; /**< Disabled without Power Gating Power Down Control */
                            /* Bits[19:16], default=0x00000003*/
    UINT32 dispgpdctl : 4; /**< Disabled with Power Gating Power Down Control */
                            /* Bits[23:20], default=0x00000003*/
    UINT32 l1pgnopgpdctl : 4; /**< L1 Powergateable without Power Gating Power Down Control */
                            /* Bits[27:24], default=0x00000004*/
    UINT32 l1pgupgpdctl : 4; /**< L1 Powergateable with Power Gating Power Down Control */
                            /* Bits[31:28], default=0x00000004*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PIPEPDCTL_PCIE_MEM_RCRB_STRUCT;

#define PIPEPDCTL_PCIE_MEM_RCRB_WIDTH 32
#define PIPEPDCTL_PCIE_MEM_RCRB_DEFAULT 0x44333330U
#define PIPEPDCTL_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define PIPEPDCTL_PCIE_MEM_RCRB_DETNOPGPDCTL_LSB 0x0000
#define PIPEPDCTL_PCIE_MEM_RCRB_DETNOPGPDCTL_SIZE 0x0004
#define PIPEPDCTL_PCIE_MEM_RCRB_DETPGPDCTL_LSB 0x0004
#define PIPEPDCTL_PCIE_MEM_RCRB_DETPGPDCTL_SIZE 0x0004
#define PIPEPDCTL_PCIE_MEM_RCRB_L23NOPGPDCTL_LSB 0x0008
#define PIPEPDCTL_PCIE_MEM_RCRB_L23NOPGPDCTL_SIZE 0x0004
#define PIPEPDCTL_PCIE_MEM_RCRB_L23PGPDCTL_LSB 0x000c
#define PIPEPDCTL_PCIE_MEM_RCRB_L23PGPDCTL_SIZE 0x0004
#define PIPEPDCTL_PCIE_MEM_RCRB_DISNOPGPDCTL_LSB 0x0010
#define PIPEPDCTL_PCIE_MEM_RCRB_DISNOPGPDCTL_SIZE 0x0004
#define PIPEPDCTL_PCIE_MEM_RCRB_DISPGPDCTL_LSB 0x0014
#define PIPEPDCTL_PCIE_MEM_RCRB_DISPGPDCTL_SIZE 0x0004
#define PIPEPDCTL_PCIE_MEM_RCRB_L1PGNOPGPDCTL_LSB 0x0018
#define PIPEPDCTL_PCIE_MEM_RCRB_L1PGNOPGPDCTL_SIZE 0x0004
#define PIPEPDCTL_PCIE_MEM_RCRB_L1PGUPGPDCTL_LSB 0x001c
#define PIPEPDCTL_PCIE_MEM_RCRB_L1PGUPGPDCTL_SIZE 0x0004



/** PIPEPDCTL2 desc:
  */

#define PIPEPDCTL2_PCIE_MEM_RCRB_REG     (0x00000598U)

typedef union {
  struct {
    UINT32 l1upnopgpdctl : 4; /**< L1 Unpowergateable without Power Gating Power Down Control */
                            /* Bits[3:0], default=0x00000006*/
    UINT32 l1d1upupgpdctl : 4; /**< L1.1 Unpowergateable with Un Power Gating Power Down Control */
                            /* Bits[7:4], default=0x00000006*/
    UINT32 l1d1pgnopgpdctl : 4; /**< L1.1 Powergateable without Power Gating Power Down Control */
                            /* Bits[11:8], default=0x00000004*/
    UINT32 l1d1pgpgpdctl : 4; /**< L1.1 Powergateable with Power Gating Power Down Control */
                            /* Bits[15:12], default=0x00000004*/
    UINT32 l1d2nopgpdctl : 4; /**< L1.2 without Power Gating Power Down Control */
                            /* Bits[19:16], default=0x00000004*/
    UINT32 l1d2pgpdctl : 4; /**< L1.2 with Power Gating Power Down Control */
                            /* Bits[23:20], default=0x00000004*/
    UINT32 ducfgnopgpdctl : 4; /**< Down and Unconfigure without Power Gating Power Down Control */
                            /* Bits[27:24], default=0x00000000*/
    UINT32 ducfgpgpdctl : 4; /**< Down and Unconfigure with Power Gating Power Down Control */
                            /* Bits[31:28], default=0x00000003*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PIPEPDCTL2_PCIE_MEM_RCRB_STRUCT;

#define PIPEPDCTL2_PCIE_MEM_RCRB_WIDTH 32
#define PIPEPDCTL2_PCIE_MEM_RCRB_DEFAULT 0x30444466U
#define PIPEPDCTL2_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define PIPEPDCTL2_PCIE_MEM_RCRB_L1UPNOPGPDCTL_LSB 0x0000
#define PIPEPDCTL2_PCIE_MEM_RCRB_L1UPNOPGPDCTL_SIZE 0x0004
#define PIPEPDCTL2_PCIE_MEM_RCRB_L1D1UPUPGPDCTL_LSB 0x0004
#define PIPEPDCTL2_PCIE_MEM_RCRB_L1D1UPUPGPDCTL_SIZE 0x0004
#define PIPEPDCTL2_PCIE_MEM_RCRB_L1D1PGNOPGPDCTL_LSB 0x0008
#define PIPEPDCTL2_PCIE_MEM_RCRB_L1D1PGNOPGPDCTL_SIZE 0x0004
#define PIPEPDCTL2_PCIE_MEM_RCRB_L1D1PGPGPDCTL_LSB 0x000c
#define PIPEPDCTL2_PCIE_MEM_RCRB_L1D1PGPGPDCTL_SIZE 0x0004
#define PIPEPDCTL2_PCIE_MEM_RCRB_L1D2NOPGPDCTL_LSB 0x0010
#define PIPEPDCTL2_PCIE_MEM_RCRB_L1D2NOPGPDCTL_SIZE 0x0004
#define PIPEPDCTL2_PCIE_MEM_RCRB_L1D2PGPDCTL_LSB 0x0014
#define PIPEPDCTL2_PCIE_MEM_RCRB_L1D2PGPDCTL_SIZE 0x0004
#define PIPEPDCTL2_PCIE_MEM_RCRB_DUCFGNOPGPDCTL_LSB 0x0018
#define PIPEPDCTL2_PCIE_MEM_RCRB_DUCFGNOPGPDCTL_SIZE 0x0004
#define PIPEPDCTL2_PCIE_MEM_RCRB_DUCFGPGPDCTL_LSB 0x001c
#define PIPEPDCTL2_PCIE_MEM_RCRB_DUCFGPGPDCTL_SIZE 0x0004



/** PIPEPDCTL3 desc:
  */

#define PIPEPDCTL3_PCIE_MEM_RCRB_REG     (0x0000059cU)

typedef union {
  struct {
    UINT32 l1dlownopgpdctl : 4; /**< L1.Low without Power Gating Power Down Control */
                            /* Bits[3:0], default=0x00000004*/
    UINT32 l1dlowpgpdctl : 4; /**< L1.Low with Power Gating Power Down Control */
                            /* Bits[7:4], default=0x00000004*/
    UINT32 rsvd_m : 24; /**< Reserved */
                            /* Bits[31:8], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PIPEPDCTL3_PCIE_MEM_RCRB_STRUCT;

#define PIPEPDCTL3_PCIE_MEM_RCRB_WIDTH 32
#define PIPEPDCTL3_PCIE_MEM_RCRB_DEFAULT 0x00000044U
#define PIPEPDCTL3_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define PIPEPDCTL3_PCIE_MEM_RCRB_L1DLOWNOPGPDCTL_LSB 0x0000
#define PIPEPDCTL3_PCIE_MEM_RCRB_L1DLOWNOPGPDCTL_SIZE 0x0004
#define PIPEPDCTL3_PCIE_MEM_RCRB_L1DLOWPGPDCTL_LSB 0x0004
#define PIPEPDCTL3_PCIE_MEM_RCRB_L1DLOWPGPDCTL_SIZE 0x0004
#define PIPEPDCTL3_PCIE_MEM_RCRB_RSVD_M_LSB 0x0008
#define PIPEPDCTL3_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0018



/** PIPEPDCTLEXT desc:
  */

#define PIPEPDCTLEXT_PCIE_MEM_RCRB_REG   (0x000005a0U)

typedef union {
  struct {
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[00:00] */
                            /* Bits[0:0], default=0*/
    UINT32 p2tp2tcd : 1; /**< P2 to P2 Transition Clock Domain */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 p2tp2tp : 1; /**< P2 to P2 Transition Policy */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 p2ugtpgsm : 1; /**< P2 Unpower Gating To Power Gating Synchronize Mode */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 lsdpmrfm : 8; /**< Link State Deeppm Request Forwarding Mask */
                            /* Bits[11:4], default=0x00000000*/
    UINT32 ep2tp2tp : 1; /**< Enhanced P2 to P2 Transition Policy */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 rsvd_1 : 3; /**< Undefined - auto filled rsvd_[15:13] */
                            /* Bits[15:13], default=0*/
    UINT32 pddpmrfm : 16; /**< Powerdown Deeppm Request Forwarding Mask */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PIPEPDCTLEXT_PCIE_MEM_RCRB_STRUCT;

#define PIPEPDCTLEXT_PCIE_MEM_RCRB_WIDTH 32
#define PIPEPDCTLEXT_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define PIPEPDCTLEXT_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define PIPEPDCTLEXT_PCIE_MEM_RCRB_P2TP2TCD_LSB 0x0001
#define PIPEPDCTLEXT_PCIE_MEM_RCRB_P2TP2TCD_SIZE 0x0001
#define PIPEPDCTLEXT_PCIE_MEM_RCRB_P2TP2TP_LSB 0x0002
#define PIPEPDCTLEXT_PCIE_MEM_RCRB_P2TP2TP_SIZE 0x0001
#define PIPEPDCTLEXT_PCIE_MEM_RCRB_P2UGTPGSM_LSB 0x0003
#define PIPEPDCTLEXT_PCIE_MEM_RCRB_P2UGTPGSM_SIZE 0x0001
#define PIPEPDCTLEXT_PCIE_MEM_RCRB_LSDPMRFM_LSB 0x0004
#define PIPEPDCTLEXT_PCIE_MEM_RCRB_LSDPMRFM_SIZE 0x0008
#define PIPEPDCTLEXT_PCIE_MEM_RCRB_EP2TP2TP_LSB 0x000c
#define PIPEPDCTLEXT_PCIE_MEM_RCRB_EP2TP2TP_SIZE 0x0001
#define PIPEPDCTLEXT_PCIE_MEM_RCRB_PDDPMRFM_LSB 0x0010
#define PIPEPDCTLEXT_PCIE_MEM_RCRB_PDDPMRFM_SIZE 0x0010



/** MPHYCAPCFG desc:
  */

#define MPHYCAPCFG_PCIE_MEM_RCRB_REG     (0x000005a8U)

typedef union {
  struct {
    UINT32 mlsosgssvcc : 7; /**< MACPHY Lower SKP OS Generation Supported Speeds Vector Capability Configurations */
                            /* Bits[6:0], default=0x0000007F*/
    UINT32 mlsosrssvcc : 7; /**< MACPHY Lower SKP OS Reception Supported Speeds Vector Capability Configurations */
                            /* Bits[13:7], default=0x00000000*/
    UINT32 rsvd_m : 18; /**< Reserved */
                            /* Bits[31:14], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MPHYCAPCFG_PCIE_MEM_RCRB_STRUCT;

#define MPHYCAPCFG_PCIE_MEM_RCRB_WIDTH 32
#define MPHYCAPCFG_PCIE_MEM_RCRB_DEFAULT 0x0000007fU
#define MPHYCAPCFG_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define MPHYCAPCFG_PCIE_MEM_RCRB_MLSOSGSSVCC_LSB 0x0000
#define MPHYCAPCFG_PCIE_MEM_RCRB_MLSOSGSSVCC_SIZE 0x0007
#define MPHYCAPCFG_PCIE_MEM_RCRB_MLSOSRSSVCC_LSB 0x0007
#define MPHYCAPCFG_PCIE_MEM_RCRB_MLSOSRSSVCC_SIZE 0x0007
#define MPHYCAPCFG_PCIE_MEM_RCRB_RSVD_M_LSB 0x000e
#define MPHYCAPCFG_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0012



/** RPDEC1 desc:
  */

#define RPDEC1_PCIE_MEM_RCRB_REG         (0x00000780U)

typedef union {
  struct {
    UINT32 rpwrert : 8; /**< Reset Prep Warm Reset Encoding Reset Type */
                            /* Bits[7:0], default=0x00000000*/
    UINT32 rpwrept : 8; /**< Reset Prep Warm Reset Encoding Prep Type */
                            /* Bits[15:8], default=0x00000000*/
    UINT32 rpcrert : 8; /**< Reset Prep Cold Reset Encoding Reset Type */
                            /* Bits[23:16], default=0x00000000*/
    UINT32 rpcrept : 8; /**< Reset Prep Cold Reset Encoding Prep Type */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RPDEC1_PCIE_MEM_RCRB_STRUCT;

#define RPDEC1_PCIE_MEM_RCRB_WIDTH 32
#define RPDEC1_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define RPDEC1_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define RPDEC1_PCIE_MEM_RCRB_RPWRERT_LSB 0x0000
#define RPDEC1_PCIE_MEM_RCRB_RPWRERT_SIZE 0x0008
#define RPDEC1_PCIE_MEM_RCRB_RPWREPT_LSB 0x0008
#define RPDEC1_PCIE_MEM_RCRB_RPWREPT_SIZE 0x0008
#define RPDEC1_PCIE_MEM_RCRB_RPCRERT_LSB 0x0010
#define RPDEC1_PCIE_MEM_RCRB_RPCRERT_SIZE 0x0008
#define RPDEC1_PCIE_MEM_RCRB_RPCREPT_LSB 0x0018
#define RPDEC1_PCIE_MEM_RCRB_RPCREPT_SIZE 0x0008



/** RPDEC2 desc:
  */

#define RPDEC2_PCIE_MEM_RCRB_REG         (0x00000784U)

typedef union {
  struct {
    UINT32 rps3ert : 8; /**< Reset Prep S3 Encoding Reset Type */
                            /* Bits[7:0], default=0x00000000*/
    UINT32 rps3ept : 8; /**< Reset Prep S3 Encoding Prep Type */
                            /* Bits[15:8], default=0x00000000*/
    UINT32 rps4ert : 8; /**< Reset Prep S4 Encoding Reset Type */
                            /* Bits[23:16], default=0x00000000*/
    UINT32 rps4ept : 8; /**< Reset Prep S4 Encoding Prep Type */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RPDEC2_PCIE_MEM_RCRB_STRUCT;

#define RPDEC2_PCIE_MEM_RCRB_WIDTH 32
#define RPDEC2_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define RPDEC2_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define RPDEC2_PCIE_MEM_RCRB_RPS3ERT_LSB 0x0000
#define RPDEC2_PCIE_MEM_RCRB_RPS3ERT_SIZE 0x0008
#define RPDEC2_PCIE_MEM_RCRB_RPS3EPT_LSB 0x0008
#define RPDEC2_PCIE_MEM_RCRB_RPS3EPT_SIZE 0x0008
#define RPDEC2_PCIE_MEM_RCRB_RPS4ERT_LSB 0x0010
#define RPDEC2_PCIE_MEM_RCRB_RPS4ERT_SIZE 0x0008
#define RPDEC2_PCIE_MEM_RCRB_RPS4EPT_LSB 0x0018
#define RPDEC2_PCIE_MEM_RCRB_RPS4EPT_SIZE 0x0008



/** RPDEC3 desc:
  */

#define RPDEC3_PCIE_MEM_RCRB_REG         (0x00000788U)

typedef union {
  struct {
    UINT32 rps5ert : 8; /**< Reset Prep S5 Encoding Reset Type */
                            /* Bits[7:0], default=0x00000000*/
    UINT32 rps5ept : 8; /**< Reset Prep S5 Encoding Prep Type */
                            /* Bits[15:8], default=0x00000000*/
    UINT32 rpdh : 3; /**< Reset Prep Default Handling */
                            /* Bits[18:16], default=0x00000000*/
    UINT32 rsvd_m : 13; /**< Reserved */
                            /* Bits[31:19], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RPDEC3_PCIE_MEM_RCRB_STRUCT;

#define RPDEC3_PCIE_MEM_RCRB_WIDTH 32
#define RPDEC3_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define RPDEC3_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define RPDEC3_PCIE_MEM_RCRB_RPS5ERT_LSB 0x0000
#define RPDEC3_PCIE_MEM_RCRB_RPS5ERT_SIZE 0x0008
#define RPDEC3_PCIE_MEM_RCRB_RPS5EPT_LSB 0x0008
#define RPDEC3_PCIE_MEM_RCRB_RPS5EPT_SIZE 0x0008
#define RPDEC3_PCIE_MEM_RCRB_RPDH_LSB 0x0010
#define RPDEC3_PCIE_MEM_RCRB_RPDH_SIZE 0x0003
#define RPDEC3_PCIE_MEM_RCRB_RSVD_M_LSB 0x0013
#define RPDEC3_PCIE_MEM_RCRB_RSVD_M_SIZE 0x000d



/** TXMDEC1 desc:
  */

#define TXMDEC1_PCIE_MEM_RCRB_REG        (0x000007a0U)

typedef union {
  struct {
    UINT8 cridbncd : 1; /**< Completions REQID Bus Number Check Disable */
                            /* Bits[0:0], default=0x00000001*/
    UINT8 mrbidbncd : 1; /**< Message Route By ID Bus Number Check Disable */
                            /* Bits[1:1], default=0x00000001*/
    UINT8 npmwrse : 1; /**< Non Posted Memory Write Support Enable */
                            /* Bits[2:2], default=0x00000000*/
    UINT8 memrangechkdis : 1; /**< Memory Range Check Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT8 cplreqidbnchkdis : 1; /**< Completion Requester ID Bus Number Range Check Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT8 msgvdmreqidbnchkdis : 1; /**< Message and VDM Requester ID Bus Number Range Check Disable */
                            /* Bits[5:5], default=0x00000000*/
    UINT8 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[07:06] */
                            /* Bits[7:6], default=0*/
  }     Bits;
  UINT8 Data;
} TXMDEC1_PCIE_MEM_RCRB_STRUCT;

#define TXMDEC1_PCIE_MEM_RCRB_WIDTH 8
#define TXMDEC1_PCIE_MEM_RCRB_DEFAULT 0x03U
#define TXMDEC1_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define TXMDEC1_PCIE_MEM_RCRB_CRIDBNCD_LSB 0x0000
#define TXMDEC1_PCIE_MEM_RCRB_CRIDBNCD_SIZE 0x0001
#define TXMDEC1_PCIE_MEM_RCRB_MRBIDBNCD_LSB 0x0001
#define TXMDEC1_PCIE_MEM_RCRB_MRBIDBNCD_SIZE 0x0001
#define TXMDEC1_PCIE_MEM_RCRB_NPMWRSE_LSB 0x0002
#define TXMDEC1_PCIE_MEM_RCRB_NPMWRSE_SIZE 0x0001
#define TXMDEC1_PCIE_MEM_RCRB_MEMRANGECHKDIS_LSB 0x0003
#define TXMDEC1_PCIE_MEM_RCRB_MEMRANGECHKDIS_SIZE 0x0001
#define TXMDEC1_PCIE_MEM_RCRB_CPLREQIDBNCHKDIS_LSB 0x0004
#define TXMDEC1_PCIE_MEM_RCRB_CPLREQIDBNCHKDIS_SIZE 0x0001
#define TXMDEC1_PCIE_MEM_RCRB_MSGVDMREQIDBNCHKDIS_LSB 0x0005
#define TXMDEC1_PCIE_MEM_RCRB_MSGVDMREQIDBNCHKDIS_SIZE 0x0001



/** DECCTL desc:
  */

#define DECCTL_PCIE_MEM_RCRB_REG         (0x00000904U)

typedef union {
  struct {
    UINT32 atsce : 1; /**< ATS Check Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 busnumzchk : 1; /**< Bus Number Zero Check */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 mtrxltc : 1; /**< Malform Receive LT Cycle */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 outbexpcplchken : 1; /**< Outbound Expected Completion Check Enable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 dropcplatnzce : 1; /**< Drop Completion for AT Not Zero Check Enable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[05:05] */
                            /* Bits[5:5], default=0*/
    UINT32 lcrxint : 1; /**< Local Consume Receive Interrupt */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 vdmatac : 1; /**< VDM Address Translation Attribute Check */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 urvdme16dw : 1; /**< Unsupported Request VDM exceeding 16DW */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 urrxuvdmintelvid : 1; /**< UR Received Unknown VDM from Intel Vendor ID */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 urrxuvdmuvid : 1; /**< UR Received Unknown VDM from Unknown Vendor ID */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 urrxurtrcvdm : 1; /**< UR Received Unknown Route To Root Complex VDM */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 urrxultvdm : 1; /**< UR Received Unknown Locally Terminated VDM */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 urrxuravdm : 1; /**< UR Received Unknown Routed by Address VDM */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 urrxuridvdm : 1; /**< UR Received Unknown Routed by ID VDM */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 urrxuorvdm : 1; /**< UR Received Unknown Other Routing VDM */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 urrxvmctpbfrc : 1; /**< UR Received Valid MCTP which is Broadcast From Root Complex */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 ichkinvreqrmsgrbid : 1; /**< Ignore checking Invalidate Request Message to be Routed by ID */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 rxmctpdecen : 1; /**< Receive MCTP Decode Enable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 rxvdmdece : 1; /**< Receive VDMs Decode Enable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 rxgpedecen : 1; /**< Receive GPE Decode Enable */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 rxsbemcapdecen : 1; /**< Receive SBEM_CAP Decode Enable */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 rxadleddecen : 1; /**< Receive ASRT_LED/ DSRT_LED Decode Enable */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 rxltrmdech : 1; /**< Receive LTR Message Decode Enable Handling */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 lcrxerrmsg : 1; /**< Local Consume Receive Error Message */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 rsvd_rw : 1; /**< Reserved */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 lcrxptmreq : 1; /**< Local Consume Receive PTM_REQ */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 urrxuvdmrbfrc : 1; /**< UR Received Unknown VDM Routed by Broadcast from Root Complex */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 urrxuvdmrgrtrc : 1; /**< UR Received Unknown VDM Routed by Gather and Route to Root Complex */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 rxmctpbrcdecen : 1; /**< Receive MCTP Broadcast From Root Complex Decode Enable */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 urrxmctpntco : 1; /**< UR Receive MCTP Not TC0 */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 rximdecen : 1; /**< Receive Invalidate Message Decode Enable */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DECCTL_PCIE_MEM_RCRB_STRUCT;

#define DECCTL_PCIE_MEM_RCRB_WIDTH 32
#define DECCTL_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define DECCTL_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define DECCTL_PCIE_MEM_RCRB_ATSCE_LSB 0x0000
#define DECCTL_PCIE_MEM_RCRB_ATSCE_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_BUSNUMZCHK_LSB 0x0001
#define DECCTL_PCIE_MEM_RCRB_BUSNUMZCHK_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_MTRXLTC_LSB 0x0002
#define DECCTL_PCIE_MEM_RCRB_MTRXLTC_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_OUTBEXPCPLCHKEN_LSB 0x0003
#define DECCTL_PCIE_MEM_RCRB_OUTBEXPCPLCHKEN_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_DROPCPLATNZCE_LSB 0x0004
#define DECCTL_PCIE_MEM_RCRB_DROPCPLATNZCE_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_LCRXINT_LSB 0x0006
#define DECCTL_PCIE_MEM_RCRB_LCRXINT_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_VDMATAC_LSB 0x0007
#define DECCTL_PCIE_MEM_RCRB_VDMATAC_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_URVDME16DW_LSB 0x0008
#define DECCTL_PCIE_MEM_RCRB_URVDME16DW_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_URRXUVDMINTELVID_LSB 0x0009
#define DECCTL_PCIE_MEM_RCRB_URRXUVDMINTELVID_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_URRXUVDMUVID_LSB 0x000a
#define DECCTL_PCIE_MEM_RCRB_URRXUVDMUVID_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_URRXURTRCVDM_LSB 0x000b
#define DECCTL_PCIE_MEM_RCRB_URRXURTRCVDM_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_URRXULTVDM_LSB 0x000c
#define DECCTL_PCIE_MEM_RCRB_URRXULTVDM_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_URRXURAVDM_LSB 0x000d
#define DECCTL_PCIE_MEM_RCRB_URRXURAVDM_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_URRXURIDVDM_LSB 0x000e
#define DECCTL_PCIE_MEM_RCRB_URRXURIDVDM_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_URRXUORVDM_LSB 0x000f
#define DECCTL_PCIE_MEM_RCRB_URRXUORVDM_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_URRXVMCTPBFRC_LSB 0x0010
#define DECCTL_PCIE_MEM_RCRB_URRXVMCTPBFRC_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_ICHKINVREQRMSGRBID_LSB 0x0011
#define DECCTL_PCIE_MEM_RCRB_ICHKINVREQRMSGRBID_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_RXMCTPDECEN_LSB 0x0012
#define DECCTL_PCIE_MEM_RCRB_RXMCTPDECEN_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_RXVDMDECE_LSB 0x0013
#define DECCTL_PCIE_MEM_RCRB_RXVDMDECE_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_RXGPEDECEN_LSB 0x0014
#define DECCTL_PCIE_MEM_RCRB_RXGPEDECEN_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_RXSBEMCAPDECEN_LSB 0x0015
#define DECCTL_PCIE_MEM_RCRB_RXSBEMCAPDECEN_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_RXADLEDDECEN_LSB 0x0016
#define DECCTL_PCIE_MEM_RCRB_RXADLEDDECEN_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_RXLTRMDECH_LSB 0x0017
#define DECCTL_PCIE_MEM_RCRB_RXLTRMDECH_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_LCRXERRMSG_LSB 0x0018
#define DECCTL_PCIE_MEM_RCRB_LCRXERRMSG_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_RSVD_RW_LSB 0x0019
#define DECCTL_PCIE_MEM_RCRB_RSVD_RW_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_LCRXPTMREQ_LSB 0x001a
#define DECCTL_PCIE_MEM_RCRB_LCRXPTMREQ_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_URRXUVDMRBFRC_LSB 0x001b
#define DECCTL_PCIE_MEM_RCRB_URRXUVDMRBFRC_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_URRXUVDMRGRTRC_LSB 0x001c
#define DECCTL_PCIE_MEM_RCRB_URRXUVDMRGRTRC_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_RXMCTPBRCDECEN_LSB 0x001d
#define DECCTL_PCIE_MEM_RCRB_RXMCTPBRCDECEN_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_URRXMCTPNTCO_LSB 0x001e
#define DECCTL_PCIE_MEM_RCRB_URRXMCTPNTCO_SIZE 0x0001
#define DECCTL_PCIE_MEM_RCRB_RXIMDECEN_LSB 0x001f
#define DECCTL_PCIE_MEM_RCRB_RXIMDECEN_SIZE 0x0001



/** PIDECCTL desc:
  */

#define PIDECCTL_PCIE_MEM_RCRB_REG       (0x0000090cU)

typedef union {
  struct {
    UINT32 cplbnchk : 1; /**< Completion Bus Number Check */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_m : 31; /**< Reserved */
                            /* Bits[31:1], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PIDECCTL_PCIE_MEM_RCRB_STRUCT;

#define PIDECCTL_PCIE_MEM_RCRB_WIDTH 32
#define PIDECCTL_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define PIDECCTL_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define PIDECCTL_PCIE_MEM_RCRB_CPLBNCHK_LSB 0x0000
#define PIDECCTL_PCIE_MEM_RCRB_CPLBNCHK_SIZE 0x0001
#define PIDECCTL_PCIE_MEM_RCRB_RSVD_M_LSB 0x0001
#define PIDECCTL_PCIE_MEM_RCRB_RSVD_M_SIZE 0x001f



/** PTMPSDC1 desc:
  */

#define PTMPSDC1_PCIE_MEM_RCRB_REG       (0x00000920U)

typedef union {
  struct {
    UINT32 g1x1tpsd : 8; /**< GEN1 X1 TX Pipe Stage Delay */
                            /* Bits[7:0], default=0x00000000*/
    UINT32 g1x1rpsd : 8; /**< GEN1 X1 RX Pipe Stage Delay */
                            /* Bits[15:8], default=0x00000000*/
    UINT32 g1x2tpsd : 8; /**< GEN1 X2 TX Pipe Stage Delay */
                            /* Bits[23:16], default=0x00000000*/
    UINT32 g1x2rpsd : 8; /**< GEN1 X2 RX Pipe Stage Delay */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PTMPSDC1_PCIE_MEM_RCRB_STRUCT;

#define PTMPSDC1_PCIE_MEM_RCRB_WIDTH 32
#define PTMPSDC1_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define PTMPSDC1_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define PTMPSDC1_PCIE_MEM_RCRB_G1X1TPSD_LSB 0x0000
#define PTMPSDC1_PCIE_MEM_RCRB_G1X1TPSD_SIZE 0x0008
#define PTMPSDC1_PCIE_MEM_RCRB_G1X1RPSD_LSB 0x0008
#define PTMPSDC1_PCIE_MEM_RCRB_G1X1RPSD_SIZE 0x0008
#define PTMPSDC1_PCIE_MEM_RCRB_G1X2TPSD_LSB 0x0010
#define PTMPSDC1_PCIE_MEM_RCRB_G1X2TPSD_SIZE 0x0008
#define PTMPSDC1_PCIE_MEM_RCRB_G1X2RPSD_LSB 0x0018
#define PTMPSDC1_PCIE_MEM_RCRB_G1X2RPSD_SIZE 0x0008



/** G5L0SCTL desc:
  */

#define G5L0SCTL_PCIE_MEM_RCRB_REG       (0x00000e00U)

typedef union {
  struct {
    UINT32 g5ccnfts : 8; /**< Gen5 Common Clock N_FTS */
                            /* Bits[7:0], default=0x0000001E*/
    UINT32 g5ucnfts : 8; /**< Gen5 Unique Clock N_FTS */
                            /* Bits[15:8], default=0x00000028*/
    UINT32 rsvd_0 : 6; /**< Undefined - auto filled rsvd_[21:16] */
                            /* Bits[21:16], default=0*/
    UINT32 g5l0sic : 2; /**< Gen5 L0s Entry Idle Control */
                            /* Bits[23:22], default=0x00000000*/
    UINT32 g5asl0spl : 8; /**< Gen5Active State L0s Preparation Latency */
                            /* Bits[31:24], default=0x0000000C*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} G5L0SCTL_PCIE_MEM_RCRB_STRUCT;

#define G5L0SCTL_PCIE_MEM_RCRB_WIDTH 32
#define G5L0SCTL_PCIE_MEM_RCRB_DEFAULT 0x0c00281eU
#define G5L0SCTL_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define G5L0SCTL_PCIE_MEM_RCRB_G5CCNFTS_LSB 0x0000
#define G5L0SCTL_PCIE_MEM_RCRB_G5CCNFTS_SIZE 0x0008
#define G5L0SCTL_PCIE_MEM_RCRB_G5UCNFTS_LSB 0x0008
#define G5L0SCTL_PCIE_MEM_RCRB_G5UCNFTS_SIZE 0x0008
#define G5L0SCTL_PCIE_MEM_RCRB_G5L0SIC_LSB 0x0016
#define G5L0SCTL_PCIE_MEM_RCRB_G5L0SIC_SIZE 0x0002
#define G5L0SCTL_PCIE_MEM_RCRB_G5ASL0SPL_LSB 0x0018
#define G5L0SCTL_PCIE_MEM_RCRB_G5ASL0SPL_SIZE 0x0008



/** PX32EQCFG1 desc:
  */

#define PX32EQCFG1_PCIE_MEM_RCRB_REG     (0x00000e04U)

typedef union {
  struct {
    UINT32 px32gmflntl : 1; /**< 32.0 GT/s Multi-Fragment Linear and Nine-Tile List Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 px32gmeqsmmflntl : 1; /**< 32.0 GT/s Multi-EQ Search Mode, Multi fragment linear and nine tile list Enable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 px32ghapccpie : 1; /**< 32.0 GT/s Hardware Autonomous Preset/Coefficient Count Per-Iteration Extention */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 px32ghapccpi : 4; /**< 32.0 GT/s Hardware Autonomous Preset/Coefficient Count Per-Iteration */
                            /* Bits[6:3], default=0x0000000A*/
    UINT32 px32geqts2irrc : 1; /**< 32.0 GT/s EQ TS2 in Recovery.ReceiverConfig Enable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 px32grwtneve : 4; /**< 32.0 GT/s Receiver Wait Time For New Equalization Value Evaluation */
                            /* Bits[11:8], default=0x00000001*/
    UINT32 px32ghaed : 1; /**< 32.0 GT/s Hardware Autonomous Equalization Done */
                            /* Bits[12:12], default=0x00000001*/
    UINT32 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[14:13] */
                            /* Bits[14:13], default=0*/
    UINT32 px32grtpcoe : 1; /**< 32.0 GT/s Remote Transmitter Preset Coefficient Override Enable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 px32grtlepceb : 1; /**< 32.0 GT/s Remote Transmit Link Equalization Preset/Coefficient Evaluation Bypass */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 px32glep3b : 1; /**< 32.0 GT/s Link Equalization 3 Bypass */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 px32glep23b : 1; /**< 32.0 GT/s Link Equalization Phase 2 and 3 Bypass */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[19:19] */
                            /* Bits[19:19], default=0*/
    UINT32 px32greic : 1; /**< 32.0GT/s Reset EIEOS Interval Count */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 rsvd_2 : 6; /**< Undefined - auto filled rsvd_[26:21] */
                            /* Bits[26:21], default=0*/
    UINT32 px32gtswlpce : 3; /**< 32.0GT/s Training Sequence Wait Latency For Presets / Coefficients Evaluation */
                            /* Bits[29:27], default=0x00000000*/
    UINT32 rsvd_3 : 1; /**< Undefined - auto filled rsvd_[30:30] */
                            /* Bits[30:30], default=0*/
    UINT32 rsvd_m : 1; /**< Reserved */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PX32EQCFG1_PCIE_MEM_RCRB_STRUCT;

#define PX32EQCFG1_PCIE_MEM_RCRB_WIDTH 32
#define PX32EQCFG1_PCIE_MEM_RCRB_DEFAULT 0x00001150U
#define PX32EQCFG1_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GMFLNTL_LSB 0x0000
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GMFLNTL_SIZE 0x0001
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GMEQSMMFLNTL_LSB 0x0001
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GMEQSMMFLNTL_SIZE 0x0001
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GHAPCCPIE_LSB 0x0002
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GHAPCCPIE_SIZE 0x0001
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GHAPCCPI_LSB 0x0003
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GHAPCCPI_SIZE 0x0004
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GEQTS2IRRC_LSB 0x0007
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GEQTS2IRRC_SIZE 0x0001
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GRWTNEVE_LSB 0x0008
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GRWTNEVE_SIZE 0x0004
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GHAED_LSB 0x000c
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GHAED_SIZE 0x0001
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GRTPCOE_LSB 0x000f
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GRTPCOE_SIZE 0x0001
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GRTLEPCEB_LSB 0x0010
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GRTLEPCEB_SIZE 0x0001
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GLEP3B_LSB 0x0011
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GLEP3B_SIZE 0x0001
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GLEP23B_LSB 0x0012
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GLEP23B_SIZE 0x0001
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GREIC_LSB 0x0014
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GREIC_SIZE 0x0001
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GTSWLPCE_LSB 0x001b
#define PX32EQCFG1_PCIE_MEM_RCRB_PX32GTSWLPCE_SIZE 0x0003
#define PX32EQCFG1_PCIE_MEM_RCRB_RSVD_M_LSB 0x001f
#define PX32EQCFG1_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0001



/** PX32GRTPCL1 desc:
  */

#define PX32GRTPCL1_PCIE_MEM_RCRB_REG    (0x00000e08U)

typedef union {
  struct {
    UINT32 rtprecl0pl0 : 6; /**< Remote Transmitter Pre-Cursor Coefficient List 0/Preset List 0 */
                            /* Bits[5:0], default=0x00000000*/
    UINT32 rtpostcl0pl1 : 6; /**< Remote Transmitter Post-Cursor Coefficient List 0/Preset List 1 */
                            /* Bits[11:6], default=0x00000000*/
    UINT32 rtprecl1pl2 : 6; /**< Remote Transmitter Pre-Cursor Coefficient List 1/Preset List 2 */
                            /* Bits[17:12], default=0x00000000*/
    UINT32 rtpostcl1pl3 : 6; /**< Remote Transmitter Post-Cursor Coefficient List 1/Preset List 3 */
                            /* Bits[23:18], default=0x00000000*/
    UINT32 rtprecl2pl4 : 6; /**< Remote Transmitter Pre-Cursor Coefficient List 2/Preset List 4 */
                            /* Bits[29:24], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[30:30] */
                            /* Bits[30:30], default=0*/
    UINT32 pcm : 1; /**< Preset/Coefficient Mode */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PX32GRTPCL1_PCIE_MEM_RCRB_STRUCT;

#define PX32GRTPCL1_PCIE_MEM_RCRB_WIDTH 32
#define PX32GRTPCL1_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define PX32GRTPCL1_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define PX32GRTPCL1_PCIE_MEM_RCRB_RTPRECL0PL0_LSB 0x0000
#define PX32GRTPCL1_PCIE_MEM_RCRB_RTPRECL0PL0_SIZE 0x0006
#define PX32GRTPCL1_PCIE_MEM_RCRB_RTPOSTCL0PL1_LSB 0x0006
#define PX32GRTPCL1_PCIE_MEM_RCRB_RTPOSTCL0PL1_SIZE 0x0006
#define PX32GRTPCL1_PCIE_MEM_RCRB_RTPRECL1PL2_LSB 0x000c
#define PX32GRTPCL1_PCIE_MEM_RCRB_RTPRECL1PL2_SIZE 0x0006
#define PX32GRTPCL1_PCIE_MEM_RCRB_RTPOSTCL1PL3_LSB 0x0012
#define PX32GRTPCL1_PCIE_MEM_RCRB_RTPOSTCL1PL3_SIZE 0x0006
#define PX32GRTPCL1_PCIE_MEM_RCRB_RTPRECL2PL4_LSB 0x0018
#define PX32GRTPCL1_PCIE_MEM_RCRB_RTPRECL2PL4_SIZE 0x0006
#define PX32GRTPCL1_PCIE_MEM_RCRB_PCM_LSB 0x001f
#define PX32GRTPCL1_PCIE_MEM_RCRB_PCM_SIZE 0x0001



/** PX32EQCFG2 desc:
  */

#define PX32EQCFG2_PCIE_MEM_RCRB_REG     (0x00000e24U)

typedef union {
  struct {
    UINT32 rewmet : 8; /**< Receiver Eye Margin Error Threshold */
                            /* Bits[7:0], default=0x00000001*/
    UINT32 rewmetm : 2; /**< Receiver Eye Margin Error Threshold Multiplier */
                            /* Bits[9:8], default=0x00000000*/
    UINT32 mpeme : 1; /**< Mid-Point Equalization Mechanism Enable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 nteme : 1; /**< Nine-Tiles Equalization Mechanism Enable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 hapcsb : 4; /**< Hardware Autonomous Preset/Coefficient Search Bound */
                            /* Bits[15:12], default=0x0000000A*/
    UINT32 pcet : 4; /**< Preset/Coefficient Evaluation Timeout */
                            /* Bits[19:16], default=0x00000000*/
    UINT32 ntss : 3; /**< Nine-Tiles Step Size */
                            /* Bits[22:20], default=0x00000000*/
    UINT32 emd : 1; /**< Equalization Margining Disable */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 ntic : 8; /**< Nine-Tiles Iteration Count */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PX32EQCFG2_PCIE_MEM_RCRB_STRUCT;

#define PX32EQCFG2_PCIE_MEM_RCRB_WIDTH 32
#define PX32EQCFG2_PCIE_MEM_RCRB_DEFAULT 0x0000a001U
#define PX32EQCFG2_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define PX32EQCFG2_PCIE_MEM_RCRB_REWMET_LSB 0x0000
#define PX32EQCFG2_PCIE_MEM_RCRB_REWMET_SIZE 0x0008
#define PX32EQCFG2_PCIE_MEM_RCRB_REWMETM_LSB 0x0008
#define PX32EQCFG2_PCIE_MEM_RCRB_REWMETM_SIZE 0x0002
#define PX32EQCFG2_PCIE_MEM_RCRB_MPEME_LSB 0x000a
#define PX32EQCFG2_PCIE_MEM_RCRB_MPEME_SIZE 0x0001
#define PX32EQCFG2_PCIE_MEM_RCRB_NTEME_LSB 0x000b
#define PX32EQCFG2_PCIE_MEM_RCRB_NTEME_SIZE 0x0001
#define PX32EQCFG2_PCIE_MEM_RCRB_HAPCSB_LSB 0x000c
#define PX32EQCFG2_PCIE_MEM_RCRB_HAPCSB_SIZE 0x0004
#define PX32EQCFG2_PCIE_MEM_RCRB_PCET_LSB 0x0010
#define PX32EQCFG2_PCIE_MEM_RCRB_PCET_SIZE 0x0004
#define PX32EQCFG2_PCIE_MEM_RCRB_NTSS_LSB 0x0014
#define PX32EQCFG2_PCIE_MEM_RCRB_NTSS_SIZE 0x0003
#define PX32EQCFG2_PCIE_MEM_RCRB_EMD_LSB 0x0017
#define PX32EQCFG2_PCIE_MEM_RCRB_EMD_SIZE 0x0001
#define PX32EQCFG2_PCIE_MEM_RCRB_NTIC_LSB 0x0018
#define PX32EQCFG2_PCIE_MEM_RCRB_NTIC_SIZE 0x0008



/** PX32GP0P1PCM desc:
  */

#define PX32GP0P1PCM_PCIE_MEM_RCRB_REG   (0x00000e28U)

typedef union {
  struct {
    UINT32 p0cc : 6; /**< Preset 0 Cursor Coefficient */
                            /* Bits[5:0], default=0x00000000*/
    UINT32 p0precc : 6; /**< Preset 0 Pre-Cursor Coefficient */
                            /* Bits[11:6], default=0x00000000*/
    UINT32 p0pstcc : 6; /**< Preset 0 Post-Cursor Coefficient */
                            /* Bits[17:12], default=0x00000000*/
    UINT32 p1cc : 6; /**< Preset 1 Cursor Coefficient */
                            /* Bits[23:18], default=0x00000000*/
    UINT32 p1precc : 6; /**< Preset 1 Pre-Cursor Coefficient */
                            /* Bits[29:24], default=0x00000000*/
    UINT32 rsvd_m : 2; /**< Reserved */
                            /* Bits[31:30], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PX32GP0P1PCM_PCIE_MEM_RCRB_STRUCT;

#define PX32GP0P1PCM_PCIE_MEM_RCRB_WIDTH 32
#define PX32GP0P1PCM_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define PX32GP0P1PCM_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define PX32GP0P1PCM_PCIE_MEM_RCRB_P0CC_LSB 0x0000
#define PX32GP0P1PCM_PCIE_MEM_RCRB_P0CC_SIZE 0x0006
#define PX32GP0P1PCM_PCIE_MEM_RCRB_P0PRECC_LSB 0x0006
#define PX32GP0P1PCM_PCIE_MEM_RCRB_P0PRECC_SIZE 0x0006
#define PX32GP0P1PCM_PCIE_MEM_RCRB_P0PSTCC_LSB 0x000c
#define PX32GP0P1PCM_PCIE_MEM_RCRB_P0PSTCC_SIZE 0x0006
#define PX32GP0P1PCM_PCIE_MEM_RCRB_P1CC_LSB 0x0012
#define PX32GP0P1PCM_PCIE_MEM_RCRB_P1CC_SIZE 0x0006
#define PX32GP0P1PCM_PCIE_MEM_RCRB_P1PRECC_LSB 0x0018
#define PX32GP0P1PCM_PCIE_MEM_RCRB_P1PRECC_SIZE 0x0006
#define PX32GP0P1PCM_PCIE_MEM_RCRB_RSVD_M_LSB 0x001e
#define PX32GP0P1PCM_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0002



/** PX32GP1P2P3PCM desc:
  */

#define PX32GP1P2P3PCM_PCIE_MEM_RCRB_REG (0x00000e2cU)

typedef union {
  struct {
    UINT32 p1pstcc : 6; /**< Preset 1 Post-Cursor Coefficient */
                            /* Bits[5:0], default=0x00000000*/
    UINT32 p2cc : 6; /**< Preset 2 Cursor Coefficient */
                            /* Bits[11:6], default=0x00000000*/
    UINT32 p2precc : 6; /**< Preset 2 Pre-Cursor Coefficient */
                            /* Bits[17:12], default=0x00000000*/
    UINT32 p2pstcc : 6; /**< Preset 2 Post-Cursor Coefficient */
                            /* Bits[23:18], default=0x00000000*/
    UINT32 p3cc : 6; /**< Preset 3 Cursor Coefficient */
                            /* Bits[29:24], default=0x00000000*/
    UINT32 rsvd_m : 2; /**< Reserved */
                            /* Bits[31:30], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PX32GP1P2P3PCM_PCIE_MEM_RCRB_STRUCT;

#define PX32GP1P2P3PCM_PCIE_MEM_RCRB_WIDTH 32
#define PX32GP1P2P3PCM_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define PX32GP1P2P3PCM_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define PX32GP1P2P3PCM_PCIE_MEM_RCRB_P1PSTCC_LSB 0x0000
#define PX32GP1P2P3PCM_PCIE_MEM_RCRB_P1PSTCC_SIZE 0x0006
#define PX32GP1P2P3PCM_PCIE_MEM_RCRB_P2CC_LSB 0x0006
#define PX32GP1P2P3PCM_PCIE_MEM_RCRB_P2CC_SIZE 0x0006
#define PX32GP1P2P3PCM_PCIE_MEM_RCRB_P2PRECC_LSB 0x000c
#define PX32GP1P2P3PCM_PCIE_MEM_RCRB_P2PRECC_SIZE 0x0006
#define PX32GP1P2P3PCM_PCIE_MEM_RCRB_P2PSTCC_LSB 0x0012
#define PX32GP1P2P3PCM_PCIE_MEM_RCRB_P2PSTCC_SIZE 0x0006
#define PX32GP1P2P3PCM_PCIE_MEM_RCRB_P3CC_LSB 0x0018
#define PX32GP1P2P3PCM_PCIE_MEM_RCRB_P3CC_SIZE 0x0006
#define PX32GP1P2P3PCM_PCIE_MEM_RCRB_RSVD_M_LSB 0x001e
#define PX32GP1P2P3PCM_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0002



/** PX32GP3P4PCM desc:
  */

#define PX32GP3P4PCM_PCIE_MEM_RCRB_REG   (0x00000e30U)

typedef union {
  struct {
    UINT32 p3precc : 6; /**< Preset 3 Pre-Cursor Coefficient */
                            /* Bits[5:0], default=0x00000000*/
    UINT32 p3pstcc : 6; /**< Preset 3 Post-Cursor Coefficient */
                            /* Bits[11:6], default=0x00000000*/
    UINT32 p4cc : 6; /**< Preset 4 Cursor Coefficient */
                            /* Bits[17:12], default=0x00000000*/
    UINT32 p4precc : 6; /**< Preset 4 Pre-Cursor Coefficient */
                            /* Bits[23:18], default=0x00000000*/
    UINT32 p4pstcc : 6; /**< Preset 4 Post-Cursor Coefficient */
                            /* Bits[29:24], default=0x00000000*/
    UINT32 rsvd_m : 2; /**< Reserved */
                            /* Bits[31:30], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PX32GP3P4PCM_PCIE_MEM_RCRB_STRUCT;

#define PX32GP3P4PCM_PCIE_MEM_RCRB_WIDTH 32
#define PX32GP3P4PCM_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define PX32GP3P4PCM_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define PX32GP3P4PCM_PCIE_MEM_RCRB_P3PRECC_LSB 0x0000
#define PX32GP3P4PCM_PCIE_MEM_RCRB_P3PRECC_SIZE 0x0006
#define PX32GP3P4PCM_PCIE_MEM_RCRB_P3PSTCC_LSB 0x0006
#define PX32GP3P4PCM_PCIE_MEM_RCRB_P3PSTCC_SIZE 0x0006
#define PX32GP3P4PCM_PCIE_MEM_RCRB_P4CC_LSB 0x000c
#define PX32GP3P4PCM_PCIE_MEM_RCRB_P4CC_SIZE 0x0006
#define PX32GP3P4PCM_PCIE_MEM_RCRB_P4PRECC_LSB 0x0012
#define PX32GP3P4PCM_PCIE_MEM_RCRB_P4PRECC_SIZE 0x0006
#define PX32GP3P4PCM_PCIE_MEM_RCRB_P4PSTCC_LSB 0x0018
#define PX32GP3P4PCM_PCIE_MEM_RCRB_P4PSTCC_SIZE 0x0006
#define PX32GP3P4PCM_PCIE_MEM_RCRB_RSVD_M_LSB 0x001e
#define PX32GP3P4PCM_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0002



/** PX32GP5P6PCM desc:
  */

#define PX32GP5P6PCM_PCIE_MEM_RCRB_REG   (0x00000e34U)

typedef union {
  struct {
    UINT32 p5cc : 6; /**< Preset 5 Cursor Coefficient */
                            /* Bits[5:0], default=0x00000000*/
    UINT32 p5precc : 6; /**< Preset 5 Pre-Cursor Coefficient */
                            /* Bits[11:6], default=0x00000000*/
    UINT32 p5pstcc : 6; /**< Preset 5 Post-Cursor Coefficient */
                            /* Bits[17:12], default=0x00000000*/
    UINT32 p6cc : 6; /**< Preset 6 Cursor Coefficient */
                            /* Bits[23:18], default=0x00000000*/
    UINT32 p6precc : 6; /**< Preset 6 Pre-Cursor Coefficient */
                            /* Bits[29:24], default=0x00000000*/
    UINT32 rsvd_m : 2; /**< Reserved */
                            /* Bits[31:30], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PX32GP5P6PCM_PCIE_MEM_RCRB_STRUCT;

#define PX32GP5P6PCM_PCIE_MEM_RCRB_WIDTH 32
#define PX32GP5P6PCM_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define PX32GP5P6PCM_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define PX32GP5P6PCM_PCIE_MEM_RCRB_P5CC_LSB 0x0000
#define PX32GP5P6PCM_PCIE_MEM_RCRB_P5CC_SIZE 0x0006
#define PX32GP5P6PCM_PCIE_MEM_RCRB_P5PRECC_LSB 0x0006
#define PX32GP5P6PCM_PCIE_MEM_RCRB_P5PRECC_SIZE 0x0006
#define PX32GP5P6PCM_PCIE_MEM_RCRB_P5PSTCC_LSB 0x000c
#define PX32GP5P6PCM_PCIE_MEM_RCRB_P5PSTCC_SIZE 0x0006
#define PX32GP5P6PCM_PCIE_MEM_RCRB_P6CC_LSB 0x0012
#define PX32GP5P6PCM_PCIE_MEM_RCRB_P6CC_SIZE 0x0006
#define PX32GP5P6PCM_PCIE_MEM_RCRB_P6PRECC_LSB 0x0018
#define PX32GP5P6PCM_PCIE_MEM_RCRB_P6PRECC_SIZE 0x0006
#define PX32GP5P6PCM_PCIE_MEM_RCRB_RSVD_M_LSB 0x001e
#define PX32GP5P6PCM_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0002



/** PX32GP6P7P8PCM desc:
  */

#define PX32GP6P7P8PCM_PCIE_MEM_RCRB_REG (0x00000e38U)

typedef union {
  struct {
    UINT32 p6pstcc : 6; /**< Preset 6 Post-Cursor Coefficient */
                            /* Bits[5:0], default=0x00000000*/
    UINT32 p7cc : 6; /**< Preset 7 Cursor Coefficient */
                            /* Bits[11:6], default=0x00000000*/
    UINT32 p7precc : 6; /**< Preset 7 Pre-Cursor Coefficient */
                            /* Bits[17:12], default=0x00000000*/
    UINT32 p7pstcc : 6; /**< Preset 7 Post-Cursor Coefficient */
                            /* Bits[23:18], default=0x00000000*/
    UINT32 p8cc : 6; /**< Preset 8 Cursor Coefficient */
                            /* Bits[29:24], default=0x00000000*/
    UINT32 rsvd_m : 2; /**< Reserved */
                            /* Bits[31:30], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PX32GP6P7P8PCM_PCIE_MEM_RCRB_STRUCT;

#define PX32GP6P7P8PCM_PCIE_MEM_RCRB_WIDTH 32
#define PX32GP6P7P8PCM_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define PX32GP6P7P8PCM_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define PX32GP6P7P8PCM_PCIE_MEM_RCRB_P6PSTCC_LSB 0x0000
#define PX32GP6P7P8PCM_PCIE_MEM_RCRB_P6PSTCC_SIZE 0x0006
#define PX32GP6P7P8PCM_PCIE_MEM_RCRB_P7CC_LSB 0x0006
#define PX32GP6P7P8PCM_PCIE_MEM_RCRB_P7CC_SIZE 0x0006
#define PX32GP6P7P8PCM_PCIE_MEM_RCRB_P7PRECC_LSB 0x000c
#define PX32GP6P7P8PCM_PCIE_MEM_RCRB_P7PRECC_SIZE 0x0006
#define PX32GP6P7P8PCM_PCIE_MEM_RCRB_P7PSTCC_LSB 0x0012
#define PX32GP6P7P8PCM_PCIE_MEM_RCRB_P7PSTCC_SIZE 0x0006
#define PX32GP6P7P8PCM_PCIE_MEM_RCRB_P8CC_LSB 0x0018
#define PX32GP6P7P8PCM_PCIE_MEM_RCRB_P8CC_SIZE 0x0006
#define PX32GP6P7P8PCM_PCIE_MEM_RCRB_RSVD_M_LSB 0x001e
#define PX32GP6P7P8PCM_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0002



/** PX32GP8P9PCM desc:
  */

#define PX32GP8P9PCM_PCIE_MEM_RCRB_REG   (0x00000e3cU)

typedef union {
  struct {
    UINT32 p8precc : 6; /**< Preset 8 Pre-Cursor Coefficient */
                            /* Bits[5:0], default=0x00000000*/
    UINT32 p8pstcc : 6; /**< Preset 8 Post-Cursor Coefficient */
                            /* Bits[11:6], default=0x00000000*/
    UINT32 p9cc : 6; /**< Preset 9 Cursor Coefficient */
                            /* Bits[17:12], default=0x00000000*/
    UINT32 p9precc : 6; /**< Preset 9 Pre-Cursor Coefficient */
                            /* Bits[23:18], default=0x00000000*/
    UINT32 p9pstcc : 6; /**< Preset 9 Post-Cursor Coefficient */
                            /* Bits[29:24], default=0x00000000*/
    UINT32 rsvd_m : 2; /**< Reserved */
                            /* Bits[31:30], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PX32GP8P9PCM_PCIE_MEM_RCRB_STRUCT;

#define PX32GP8P9PCM_PCIE_MEM_RCRB_WIDTH 32
#define PX32GP8P9PCM_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define PX32GP8P9PCM_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define PX32GP8P9PCM_PCIE_MEM_RCRB_P8PRECC_LSB 0x0000
#define PX32GP8P9PCM_PCIE_MEM_RCRB_P8PRECC_SIZE 0x0006
#define PX32GP8P9PCM_PCIE_MEM_RCRB_P8PSTCC_LSB 0x0006
#define PX32GP8P9PCM_PCIE_MEM_RCRB_P8PSTCC_SIZE 0x0006
#define PX32GP8P9PCM_PCIE_MEM_RCRB_P9CC_LSB 0x000c
#define PX32GP8P9PCM_PCIE_MEM_RCRB_P9CC_SIZE 0x0006
#define PX32GP8P9PCM_PCIE_MEM_RCRB_P9PRECC_LSB 0x0012
#define PX32GP8P9PCM_PCIE_MEM_RCRB_P9PRECC_SIZE 0x0006
#define PX32GP8P9PCM_PCIE_MEM_RCRB_P9PSTCC_LSB 0x0018
#define PX32GP8P9PCM_PCIE_MEM_RCRB_P9PSTCC_SIZE 0x0006
#define PX32GP8P9PCM_PCIE_MEM_RCRB_RSVD_M_LSB 0x001e
#define PX32GP8P9PCM_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0002



/** PX32GP10PCM desc:
  */

#define PX32GP10PCM_PCIE_MEM_RCRB_REG    (0x00000e40U)

typedef union {
  struct {
    UINT32 p10cc : 6; /**< Preset 10 Cursor Coefficient */
                            /* Bits[5:0], default=0x00000000*/
    UINT32 p10precc : 6; /**< Preset 10 Pre-Cursor Coefficient */
                            /* Bits[11:6], default=0x00000000*/
    UINT32 p10pstcc : 6; /**< Preset 10 Post-Cursor Coefficient */
                            /* Bits[17:12], default=0x00000000*/
    UINT32 rsvd_m : 14; /**< Reserved */
                            /* Bits[31:18], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PX32GP10PCM_PCIE_MEM_RCRB_STRUCT;

#define PX32GP10PCM_PCIE_MEM_RCRB_WIDTH 32
#define PX32GP10PCM_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define PX32GP10PCM_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define PX32GP10PCM_PCIE_MEM_RCRB_P10CC_LSB 0x0000
#define PX32GP10PCM_PCIE_MEM_RCRB_P10CC_SIZE 0x0006
#define PX32GP10PCM_PCIE_MEM_RCRB_P10PRECC_LSB 0x0006
#define PX32GP10PCM_PCIE_MEM_RCRB_P10PRECC_SIZE 0x0006
#define PX32GP10PCM_PCIE_MEM_RCRB_P10PSTCC_LSB 0x000c
#define PX32GP10PCM_PCIE_MEM_RCRB_P10PSTCC_SIZE 0x0006
#define PX32GP10PCM_PCIE_MEM_RCRB_RSVD_M_LSB 0x0012
#define PX32GP10PCM_PCIE_MEM_RCRB_RSVD_M_SIZE 0x000e



/** PX32GLFFS desc:
  */

#define PX32GLFFS_PCIE_MEM_RCRB_REG      (0x00000e44U)

typedef union {
  struct {
    UINT32 lplfv : 6; /**< Local PHY Low Frequency Value */
                            /* Bits[5:0], default=0x00000000*/
    UINT32 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[07:06] */
                            /* Bits[7:6], default=0*/
    UINT32 lpfsv : 6; /**< Local PHY Full Swing Value */
                            /* Bits[13:8], default=0x00000000*/
    UINT32 rsvd_1 : 2; /**< Undefined - auto filled rsvd_[15:14] */
                            /* Bits[15:14], default=0*/
    UINT32 rplfv : 6; /**< Remote PHY Low Frequency Value */
                            /* Bits[21:16], default=0x00000000*/
    UINT32 rsvd_2 : 2; /**< Undefined - auto filled rsvd_[23:22] */
                            /* Bits[23:22], default=0*/
    UINT32 rpfsv : 6; /**< Remote PHY Full Swing Value */
                            /* Bits[29:24], default=0x00000000*/
    UINT32 rsvd_m : 2; /**< Reserved */
                            /* Bits[31:30], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PX32GLFFS_PCIE_MEM_RCRB_STRUCT;

#define PX32GLFFS_PCIE_MEM_RCRB_WIDTH 32
#define PX32GLFFS_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define PX32GLFFS_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define PX32GLFFS_PCIE_MEM_RCRB_LPLFV_LSB 0x0000
#define PX32GLFFS_PCIE_MEM_RCRB_LPLFV_SIZE 0x0006
#define PX32GLFFS_PCIE_MEM_RCRB_LPFSV_LSB 0x0008
#define PX32GLFFS_PCIE_MEM_RCRB_LPFSV_SIZE 0x0006
#define PX32GLFFS_PCIE_MEM_RCRB_RPLFV_LSB 0x0010
#define PX32GLFFS_PCIE_MEM_RCRB_RPLFV_SIZE 0x0006
#define PX32GLFFS_PCIE_MEM_RCRB_RPFSV_LSB 0x0018
#define PX32GLFFS_PCIE_MEM_RCRB_RPFSV_SIZE 0x0006
#define PX32GLFFS_PCIE_MEM_RCRB_RSVD_M_LSB 0x001e
#define PX32GLFFS_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0002



/** PX32GLTCO1 desc:
  */

#define PX32GLTCO1_PCIE_MEM_RCRB_REG     (0x00000e48U)

typedef union {
  struct {
    UINT32 l0tpreco : 6; /**< Lane 0 Transmitter Pre-Cursor Coefficient Override */
                            /* Bits[5:0], default=0x00000000*/
    UINT32 l0tpostco : 6; /**< Lane 0 Transmitter Post-Cursor Coefficient Override */
                            /* Bits[11:6], default=0x00000000*/
    UINT32 l1tpreco : 6; /**< Lane 1 Transmitter Pre-Cursor Coefficient Override */
                            /* Bits[17:12], default=0x00000000*/
    UINT32 l1tpostco : 6; /**< Lane 1 Transmitter Post-Cursor Coefficient Override */
                            /* Bits[23:18], default=0x00000000*/
    UINT32 l0tcoe : 1; /**< Lane 0 Transmitter Coefficient Override Enable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 l1tcoe : 1; /**< Lane 1 Transmitter Coefficient Override Enable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 rsvd_m : 6; /**< Reserved */
                            /* Bits[31:26], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PX32GLTCO1_PCIE_MEM_RCRB_STRUCT;

#define PX32GLTCO1_PCIE_MEM_RCRB_WIDTH 32
#define PX32GLTCO1_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define PX32GLTCO1_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define PX32GLTCO1_PCIE_MEM_RCRB_L0TPRECO_LSB 0x0000
#define PX32GLTCO1_PCIE_MEM_RCRB_L0TPRECO_SIZE 0x0006
#define PX32GLTCO1_PCIE_MEM_RCRB_L0TPOSTCO_LSB 0x0006
#define PX32GLTCO1_PCIE_MEM_RCRB_L0TPOSTCO_SIZE 0x0006
#define PX32GLTCO1_PCIE_MEM_RCRB_L1TPRECO_LSB 0x000c
#define PX32GLTCO1_PCIE_MEM_RCRB_L1TPRECO_SIZE 0x0006
#define PX32GLTCO1_PCIE_MEM_RCRB_L1TPOSTCO_LSB 0x0012
#define PX32GLTCO1_PCIE_MEM_RCRB_L1TPOSTCO_SIZE 0x0006
#define PX32GLTCO1_PCIE_MEM_RCRB_L0TCOE_LSB 0x0018
#define PX32GLTCO1_PCIE_MEM_RCRB_L0TCOE_SIZE 0x0001
#define PX32GLTCO1_PCIE_MEM_RCRB_L1TCOE_LSB 0x0019
#define PX32GLTCO1_PCIE_MEM_RCRB_L1TCOE_SIZE 0x0001
#define PX32GLTCO1_PCIE_MEM_RCRB_RSVD_M_LSB 0x001a
#define PX32GLTCO1_PCIE_MEM_RCRB_RSVD_M_SIZE 0x0006



/** MEMBARCTL desc:
  */

#define MEMBARCTL_PCIE_MEM_RCRB_REG      (0x00000f00U)

typedef union {
  struct {
    UINT32 membaren : 1; /**< MEMBAR Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_m : 31; /**< Reserved */
                            /* Bits[31:1], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MEMBARCTL_PCIE_MEM_RCRB_STRUCT;

#define MEMBARCTL_PCIE_MEM_RCRB_WIDTH 32
#define MEMBARCTL_PCIE_MEM_RCRB_DEFAULT 0x00000000U
#define MEMBARCTL_PCIE_MEM_RCRB_ENDPOINT PCIE_MEM_RCRB

#define MEMBARCTL_PCIE_MEM_RCRB_MEMBAREN_LSB 0x0000
#define MEMBARCTL_PCIE_MEM_RCRB_MEMBAREN_SIZE 0x0001
#define MEMBARCTL_PCIE_MEM_RCRB_RSVD_M_LSB 0x0001
#define MEMBARCTL_PCIE_MEM_RCRB_RSVD_M_SIZE 0x001f


//************************************************ /RegisterStructs
#endif      // _PCIE_MEM_RCRB_h
