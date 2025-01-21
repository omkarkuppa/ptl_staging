/** @file
  PCIE_CFG.h

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

#ifndef _PCIE_CFG_h
#define _PCIE_CFG_h

// IP GUID: dc6bb884-1558-436c-aec8-a85483b06e19
#define PCIE_CFG_ID 0xdc6bb8841558436c

//************************************************ RegisterStructs


/** ID_PCIE_CFG desc:
  */

#define ID_PCIE_CFG_REG                  (0x00000000U)

typedef union {
  struct {
    UINT32 vid : 16; /**< Vendor Identification */
                            /* Bits[15:0], default=0x00008086*/
    UINT32 did : 16; /**< Device Identification */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} ID_PCIE_CFG_STRUCT;

#define ID_PCIE_CFG_WIDTH 32
#define ID_PCIE_CFG_DEFAULT 0x00008086U
#define ID_PCIE_CFG_ENDPOINT PCIE_CFG

#define ID_PCIE_CFG_VID_LSB 0x0000
#define ID_PCIE_CFG_VID_SIZE 0x0010
#define ID_PCIE_CFG_DID_LSB 0x0010
#define ID_PCIE_CFG_DID_SIZE 0x0010

/** CMD desc:
  */

#define CMD_PCIE_CFG_REG                 (0x00000004U)

typedef union {
  struct {
    UINT16 iose : 1; /**< I/O Space Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT16 mse : 1; /**< Memory Space Enable */
                            /* Bits[1:1], default=0x00000000*/
    UINT16 bme : 1; /**< Bus Master Enable */
                            /* Bits[2:2], default=0x00000000*/
    UINT16 sce : 1; /**< Special Cycle Enable */
                            /* Bits[3:3], default=0x00000000*/
    UINT16 mwie : 1; /**< Memory Write and Invalidate Enable */
                            /* Bits[4:4], default=0x00000000*/
    UINT16 vga_pse : 1; /**< VGA Palette Snoop */
                            /* Bits[5:5], default=0x00000000*/
    UINT16 pere : 1; /**< Parity Error Response Enable */
                            /* Bits[6:6], default=0x00000000*/
    UINT16 wcc : 1; /**< Wait Cycle Control */
                            /* Bits[7:7], default=0x00000000*/
    UINT16 see : 1; /**< SERR Enable */
                            /* Bits[8:8], default=0x00000000*/
    UINT16 fbe : 1; /**< Fast Back to Back Enable */
                            /* Bits[9:9], default=0x00000000*/
    UINT16 id : 1; /**< Interrupt Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT16 rsvd_m : 5; /**< Reserved */
                            /* Bits[15:11], default=0x00000000*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} CMD_PCIE_CFG_STRUCT;

#define CMD_PCIE_CFG_WIDTH 16
#define CMD_PCIE_CFG_DEFAULT 0x0000U
#define CMD_PCIE_CFG_ENDPOINT PCIE_CFG

#define CMD_PCIE_CFG_IOSE_LSB 0x0000
#define CMD_PCIE_CFG_IOSE_SIZE 0x0001
#define CMD_PCIE_CFG_MSE_LSB 0x0001
#define CMD_PCIE_CFG_MSE_SIZE 0x0001
#define CMD_PCIE_CFG_BME_LSB 0x0002
#define CMD_PCIE_CFG_BME_SIZE 0x0001
#define CMD_PCIE_CFG_SCE_LSB 0x0003
#define CMD_PCIE_CFG_SCE_SIZE 0x0001
#define CMD_PCIE_CFG_MWIE_LSB 0x0004
#define CMD_PCIE_CFG_MWIE_SIZE 0x0001
#define CMD_PCIE_CFG_VGA_PSE_LSB 0x0005
#define CMD_PCIE_CFG_VGA_PSE_SIZE 0x0001
#define CMD_PCIE_CFG_PERE_LSB 0x0006
#define CMD_PCIE_CFG_PERE_SIZE 0x0001
#define CMD_PCIE_CFG_WCC_LSB 0x0007
#define CMD_PCIE_CFG_WCC_SIZE 0x0001
#define CMD_PCIE_CFG_SEE_LSB 0x0008
#define CMD_PCIE_CFG_SEE_SIZE 0x0001
#define CMD_PCIE_CFG_FBE_LSB 0x0009
#define CMD_PCIE_CFG_FBE_SIZE 0x0001
#define CMD_PCIE_CFG_ID_LSB 0x000a
#define CMD_PCIE_CFG_ID_SIZE 0x0001
#define CMD_PCIE_CFG_RSVD_M_LSB 0x000b
#define CMD_PCIE_CFG_RSVD_M_SIZE 0x0005

/** RID_CC desc:
  */

#define RID_CC_PCIE_CFG_REG              (0x00000008U)

typedef union {
  struct {
    UINT32 rid : 8; /**< Revision ID */
                            /* Bits[7:0], default=0x000000F0*/
    UINT32 pi : 8; /**< Programming Interface */
                            /* Bits[15:8], default=0x00000000*/
    UINT32 scc : 8; /**< Sub-Class Code */
                            /* Bits[23:16], default=0x00000004*/
    UINT32 bcc : 8; /**< Base Class Code */
                            /* Bits[31:24], default=0x00000006*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RID_CC_PCIE_CFG_STRUCT;

#define RID_CC_PCIE_CFG_WIDTH 32
#define RID_CC_PCIE_CFG_DEFAULT 0x060400f0U
#define RID_CC_PCIE_CFG_ENDPOINT PCIE_CFG

#define RID_CC_PCIE_CFG_RID_LSB 0x0000
#define RID_CC_PCIE_CFG_RID_SIZE 0x0008
#define RID_CC_PCIE_CFG_PI_LSB 0x0008
#define RID_CC_PCIE_CFG_PI_SIZE 0x0008
#define RID_CC_PCIE_CFG_SCC_LSB 0x0010
#define RID_CC_PCIE_CFG_SCC_SIZE 0x0008
#define RID_CC_PCIE_CFG_BCC_LSB 0x0018
#define RID_CC_PCIE_CFG_BCC_SIZE 0x0008

/** BAR0 desc:
  */

#define BAR0_PCIE_CFG_REG                (0x00000010U)

typedef union {
  struct {
    UINT32 msi : 1; /**< Memory Space Indicator */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 mmt : 2; /**< Memory Mapping Type */
                            /* Bits[2:1], default=0x00000000*/
    UINT32 p : 1; /**< Prefetchable Memory */
                            /* Bits[3:3], default=0x00000001*/
    UINT32 lba : 28; /**< Lower Base Address */
                            /* Bits[31:4], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} BAR0_PCIE_CFG_STRUCT;

#define BAR0_PCIE_CFG_WIDTH 32
#define BAR0_PCIE_CFG_DEFAULT 0x00000008U
#define BAR0_PCIE_CFG_ENDPOINT PCIE_CFG

#define BAR0_PCIE_CFG_MSI_LSB 0x0000
#define BAR0_PCIE_CFG_MSI_SIZE 0x0001
#define BAR0_PCIE_CFG_MMT_LSB 0x0001
#define BAR0_PCIE_CFG_MMT_SIZE 0x0002
#define BAR0_PCIE_CFG_P_LSB 0x0003
#define BAR0_PCIE_CFG_P_SIZE 0x0001
#define BAR0_PCIE_CFG_LBA_LSB 0x0004
#define BAR0_PCIE_CFG_LBA_SIZE 0x001c

/** BAR1 desc:
  */

#define BAR1_PCIE_CFG_REG                (0x00000014U)

typedef union {
  struct {
    UINT32 uba : 32; /**< Upper Base Address */
                            /* Bits[31:0], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} BAR1_PCIE_CFG_STRUCT;

#define BAR1_PCIE_CFG_WIDTH 32
#define BAR1_PCIE_CFG_DEFAULT 0x00000000U
#define BAR1_PCIE_CFG_ENDPOINT PCIE_CFG

#define BAR1_PCIE_CFG_UBA_LSB 0x0000
#define BAR1_PCIE_CFG_UBA_SIZE 0x0020

/** BNUM_SLT desc:
  */

#define BNUM_SLT_PCIE_CFG_REG            (0x00000018U)

typedef union {
  struct {
    UINT32 pbn : 8; /**< Primary Bus Number */
                            /* Bits[7:0], default=0x00000000*/
    UINT32 scbn : 8; /**< Secondary Bus Number */
                            /* Bits[15:8], default=0x00000000*/
    UINT32 sbbn : 8; /**< Subordinate Bus Number */
                            /* Bits[23:16], default=0x00000000*/
    UINT32 slt : 8; /**< Secondary Latency Timer */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} BNUM_SLT_PCIE_CFG_STRUCT;

#define BNUM_SLT_PCIE_CFG_WIDTH 32
#define BNUM_SLT_PCIE_CFG_DEFAULT 0x00000000U
#define BNUM_SLT_PCIE_CFG_ENDPOINT PCIE_CFG

#define BNUM_SLT_PCIE_CFG_PBN_LSB 0x0000
#define BNUM_SLT_PCIE_CFG_PBN_SIZE 0x0008
#define BNUM_SLT_PCIE_CFG_SCBN_LSB 0x0008
#define BNUM_SLT_PCIE_CFG_SCBN_SIZE 0x0008
#define BNUM_SLT_PCIE_CFG_SBBN_LSB 0x0010
#define BNUM_SLT_PCIE_CFG_SBBN_SIZE 0x0008
#define BNUM_SLT_PCIE_CFG_SLT_LSB 0x0018
#define BNUM_SLT_PCIE_CFG_SLT_SIZE 0x0008

/** CAPP desc:
  */

#define CAPP_PCIE_CFG_REG                (0x00000034U)

typedef union {
  struct {
    UINT8 ptr : 8; /**< Capabilities Pointer */
                            /* Bits[7:0], default=0x00000040*/
  }     Bits;
  UINT8 Data;
} CAPP_PCIE_CFG_STRUCT;

#define CAPP_PCIE_CFG_WIDTH 8
#define CAPP_PCIE_CFG_DEFAULT 0x40U
#define CAPP_PCIE_CFG_ENDPOINT PCIE_CFG

#define CAPP_PCIE_CFG_PTR_LSB 0x0000
#define CAPP_PCIE_CFG_PTR_SIZE 0x0008

/** BCTRL desc:
  */

#define BCTRL_PCIE_CFG_REG               (0x0000003eU)

typedef union {
  struct {
    UINT16 pere : 1; /**< Parity Error Response Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT16 se : 1; /**< SERR Enable */
                            /* Bits[1:1], default=0x00000000*/
    UINT16 ie : 1; /**< ISA Enable */
                            /* Bits[2:2], default=0x00000000*/
    UINT16 ve : 1; /**< VGA Enable */
                            /* Bits[3:3], default=0x00000000*/
    UINT16 v16 : 1; /**< VGA 16-Bit Decode */
                            /* Bits[4:4], default=0x00000000*/
    UINT16 mam : 1; /**< Master Abort Mode */
                            /* Bits[5:5], default=0x00000000*/
    UINT16 sbr : 1; /**< Secondary Bus Reset */
                            /* Bits[6:6], default=0x00000000*/
    UINT16 fbe : 1; /**< Fast Back to Back Enable */
                            /* Bits[7:7], default=0x00000000*/
    UINT16 pdt : 1; /**< Primary Discard Timer */
                            /* Bits[8:8], default=0x00000000*/
    UINT16 sdt : 1; /**< Secondary Discard Timer */
                            /* Bits[9:9], default=0x00000000*/
    UINT16 dts : 1; /**< Discard Timer Status */
                            /* Bits[10:10], default=0x00000000*/
    UINT16 dtse : 1; /**< Discard Timer SERR Enable */
                            /* Bits[11:11], default=0x00000000*/
    UINT16 rsvd_m : 4; /**< Reserved */
                            /* Bits[15:12], default=0x00000000*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} BCTRL_PCIE_CFG_STRUCT;

#define BCTRL_PCIE_CFG_WIDTH 16
#define BCTRL_PCIE_CFG_DEFAULT 0x0000U
#define BCTRL_PCIE_CFG_ENDPOINT PCIE_CFG

#define BCTRL_PCIE_CFG_PERE_LSB 0x0000
#define BCTRL_PCIE_CFG_PERE_SIZE 0x0001
#define BCTRL_PCIE_CFG_SE_LSB 0x0001
#define BCTRL_PCIE_CFG_SE_SIZE 0x0001
#define BCTRL_PCIE_CFG_IE_LSB 0x0002
#define BCTRL_PCIE_CFG_IE_SIZE 0x0001
#define BCTRL_PCIE_CFG_VE_LSB 0x0003
#define BCTRL_PCIE_CFG_VE_SIZE 0x0001
#define BCTRL_PCIE_CFG_V16_LSB 0x0004
#define BCTRL_PCIE_CFG_V16_SIZE 0x0001
#define BCTRL_PCIE_CFG_MAM_LSB 0x0005
#define BCTRL_PCIE_CFG_MAM_SIZE 0x0001
#define BCTRL_PCIE_CFG_SBR_LSB 0x0006
#define BCTRL_PCIE_CFG_SBR_SIZE 0x0001
#define BCTRL_PCIE_CFG_FBE_LSB 0x0007
#define BCTRL_PCIE_CFG_FBE_SIZE 0x0001
#define BCTRL_PCIE_CFG_PDT_LSB 0x0008
#define BCTRL_PCIE_CFG_PDT_SIZE 0x0001
#define BCTRL_PCIE_CFG_SDT_LSB 0x0009
#define BCTRL_PCIE_CFG_SDT_SIZE 0x0001
#define BCTRL_PCIE_CFG_DTS_LSB 0x000a
#define BCTRL_PCIE_CFG_DTS_SIZE 0x0001
#define BCTRL_PCIE_CFG_DTSE_LSB 0x000b
#define BCTRL_PCIE_CFG_DTSE_SIZE 0x0001
#define BCTRL_PCIE_CFG_RSVD_M_LSB 0x000c
#define BCTRL_PCIE_CFG_RSVD_M_SIZE 0x0004

/** CLIST desc:
  */

#define CLIST_PCIE_CFG_REG               (0x00000040U)

typedef union {
  struct {
    UINT16 cid : 8; /**< Capability ID */
                            /* Bits[7:0], default=0x00000010*/
    UINT16 next : 8; /**< Next Capability */
                            /* Bits[15:8], default=0x00000080*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} CLIST_PCIE_CFG_STRUCT;

#define CLIST_PCIE_CFG_WIDTH 16
#define CLIST_PCIE_CFG_DEFAULT 0x8010U
#define CLIST_PCIE_CFG_ENDPOINT PCIE_CFG

#define CLIST_PCIE_CFG_CID_LSB 0x0000
#define CLIST_PCIE_CFG_CID_SIZE 0x0008
#define CLIST_PCIE_CFG_NEXT_LSB 0x0008
#define CLIST_PCIE_CFG_NEXT_SIZE 0x0008

/** XCAP desc:
  */

#define XCAP_PCIE_CFG_REG                (0x00000042U)

typedef union {
  struct {
    UINT16 cv : 4; /**< Capability Version */
                            /* Bits[3:0], default=0x00000002*/
    UINT16 dt : 4; /**< Device / Port Type */
                            /* Bits[7:4], default=0x00000004*/
    UINT16 si : 1; /**< Slot Implemented */
                            /* Bits[8:8], default=0x00000000*/
    UINT16 imn : 5; /**< Interrupt Message Number */
                            /* Bits[13:9], default=0x00000000*/
    UINT16 rsvd_m : 2; /**< Reserved */
                            /* Bits[15:14], default=0x00000000*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} XCAP_PCIE_CFG_STRUCT;

#define XCAP_PCIE_CFG_WIDTH 16
#define XCAP_PCIE_CFG_DEFAULT 0x0042U
#define XCAP_PCIE_CFG_ENDPOINT PCIE_CFG

#define XCAP_PCIE_CFG_CV_LSB 0x0000
#define XCAP_PCIE_CFG_CV_SIZE 0x0004
#define XCAP_PCIE_CFG_DT_LSB 0x0004
#define XCAP_PCIE_CFG_DT_SIZE 0x0004
#define XCAP_PCIE_CFG_SI_LSB 0x0008
#define XCAP_PCIE_CFG_SI_SIZE 0x0001
#define XCAP_PCIE_CFG_IMN_LSB 0x0009
#define XCAP_PCIE_CFG_IMN_SIZE 0x0005
#define XCAP_PCIE_CFG_RSVD_M_LSB 0x000e
#define XCAP_PCIE_CFG_RSVD_M_SIZE 0x0002

/** DCAP desc:
  */

#define DCAP_PCIE_CFG_REG                (0x00000044U)

typedef union {
  struct {
    UINT32 mps : 3; /**< Max Payload Size Supported */
                            /* Bits[2:0], default=0x00000001*/
    UINT32 pfs : 2; /**< Phantom Functions Supported */
                            /* Bits[4:3], default=0x00000000*/
    UINT32 etfs : 1; /**< Extended Tag Field Supported */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 e0al : 3; /**< Endpoint L0s Acceptable Latency */
                            /* Bits[8:6], default=0x00000000*/
    UINT32 e1al : 3; /**< Endpoint L1 Acceptable Latency */
                            /* Bits[11:9], default=0x00000000*/
    UINT32 abp : 1; /**< Reserved. On previous version of the specification this was Attention Button Present */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 aip : 1; /**< Reserved. On previous version of the specification this was Attention Indicator Present */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 pip : 1; /**< Reserved. On previous version of the specification this was Power Indicator Present */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 rber : 1; /**< Role Based Error Reporting */
                            /* Bits[15:15], default=0x00000001*/
    UINT32 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[17:16] */
                            /* Bits[17:16], default=0*/
    UINT32 cspv : 8; /**< Captured Slot Power Limit Value */
                            /* Bits[25:18], default=0x00000000*/
    UINT32 csps : 2; /**< Captured Slot Power Limit Scale */
                            /* Bits[27:26], default=0x00000000*/
    UINT32 flrc : 1; /**< Function Level Reset Capable */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 rsvd_m : 3; /**< Reserved */
                            /* Bits[31:29], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DCAP_PCIE_CFG_STRUCT;

#define DCAP_PCIE_CFG_WIDTH 32
#define DCAP_PCIE_CFG_DEFAULT 0x00008001U
#define DCAP_PCIE_CFG_ENDPOINT PCIE_CFG

#define DCAP_PCIE_CFG_MPS_LSB 0x0000
#define DCAP_PCIE_CFG_MPS_SIZE 0x0003
#define DCAP_PCIE_CFG_PFS_LSB 0x0003
#define DCAP_PCIE_CFG_PFS_SIZE 0x0002
#define DCAP_PCIE_CFG_ETFS_LSB 0x0005
#define DCAP_PCIE_CFG_ETFS_SIZE 0x0001
#define DCAP_PCIE_CFG_E0AL_LSB 0x0006
#define DCAP_PCIE_CFG_E0AL_SIZE 0x0003
#define DCAP_PCIE_CFG_E1AL_LSB 0x0009
#define DCAP_PCIE_CFG_E1AL_SIZE 0x0003
#define DCAP_PCIE_CFG_ABP_LSB 0x000c
#define DCAP_PCIE_CFG_ABP_SIZE 0x0001
#define DCAP_PCIE_CFG_AIP_LSB 0x000d
#define DCAP_PCIE_CFG_AIP_SIZE 0x0001
#define DCAP_PCIE_CFG_PIP_LSB 0x000e
#define DCAP_PCIE_CFG_PIP_SIZE 0x0001
#define DCAP_PCIE_CFG_RBER_LSB 0x000f
#define DCAP_PCIE_CFG_RBER_SIZE 0x0001
#define DCAP_PCIE_CFG_CSPV_LSB 0x0012
#define DCAP_PCIE_CFG_CSPV_SIZE 0x0008
#define DCAP_PCIE_CFG_CSPS_LSB 0x001a
#define DCAP_PCIE_CFG_CSPS_SIZE 0x0002
#define DCAP_PCIE_CFG_FLRC_LSB 0x001c
#define DCAP_PCIE_CFG_FLRC_SIZE 0x0001
#define DCAP_PCIE_CFG_RSVD_M_LSB 0x001d
#define DCAP_PCIE_CFG_RSVD_M_SIZE 0x0003

/** DCTL desc:
  */

#define DCTL_PCIE_CFG_REG                (0x00000048U)

typedef union {
  struct {
    UINT16 cee : 1; /**< Correctable Error Reporting Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT16 nfe : 1; /**< Non-Fatal Error Reporting Enable */
                            /* Bits[1:1], default=0x00000000*/
    UINT16 fee : 1; /**< Fatal Error Reporting Enable */
                            /* Bits[2:2], default=0x00000000*/
    UINT16 ure : 1; /**< Unsupported Request Reporting Enable */
                            /* Bits[3:3], default=0x00000000*/
    UINT16 ero : 1; /**< Enable Relaxed Ordering */
                            /* Bits[4:4], default=0x00000000*/
    UINT16 mps : 3; /**< Max Payload Size */
                            /* Bits[7:5], default=0x00000001*/
    UINT16 etfe : 1; /**< Extended Tag Field Enable */
                            /* Bits[8:8], default=0x00000000*/
    UINT16 pfe : 1; /**< Phantom Functions Enable */
                            /* Bits[9:9], default=0x00000000*/
    UINT16 apme : 1; /**< Aux Power PM Enable */
                            /* Bits[10:10], default=0x00000000*/
    UINT16 ens : 1; /**< Enable No Snoop */
                            /* Bits[11:11], default=0x00000000*/
    UINT16 mrrs : 3; /**< Max Read Request Size */
                            /* Bits[14:12], default=0x00000000*/
    UINT16 rsvd_m : 1; /**< Reserved */
                            /* Bits[15:15], default=0x00000000*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} DCTL_PCIE_CFG_STRUCT;

#define DCTL_PCIE_CFG_WIDTH 16
#define DCTL_PCIE_CFG_DEFAULT 0x0020U
#define DCTL_PCIE_CFG_ENDPOINT PCIE_CFG

#define DCTL_PCIE_CFG_CEE_LSB 0x0000
#define DCTL_PCIE_CFG_CEE_SIZE 0x0001
#define DCTL_PCIE_CFG_NFE_LSB 0x0001
#define DCTL_PCIE_CFG_NFE_SIZE 0x0001
#define DCTL_PCIE_CFG_FEE_LSB 0x0002
#define DCTL_PCIE_CFG_FEE_SIZE 0x0001
#define DCTL_PCIE_CFG_URE_LSB 0x0003
#define DCTL_PCIE_CFG_URE_SIZE 0x0001
#define DCTL_PCIE_CFG_ERO_LSB 0x0004
#define DCTL_PCIE_CFG_ERO_SIZE 0x0001
#define DCTL_PCIE_CFG_MPS_LSB 0x0005
#define DCTL_PCIE_CFG_MPS_SIZE 0x0003
#define DCTL_PCIE_CFG_ETFE_LSB 0x0008
#define DCTL_PCIE_CFG_ETFE_SIZE 0x0001
#define DCTL_PCIE_CFG_PFE_LSB 0x0009
#define DCTL_PCIE_CFG_PFE_SIZE 0x0001
#define DCTL_PCIE_CFG_APME_LSB 0x000a
#define DCTL_PCIE_CFG_APME_SIZE 0x0001
#define DCTL_PCIE_CFG_ENS_LSB 0x000b
#define DCTL_PCIE_CFG_ENS_SIZE 0x0001
#define DCTL_PCIE_CFG_MRRS_LSB 0x000c
#define DCTL_PCIE_CFG_MRRS_SIZE 0x0003
#define DCTL_PCIE_CFG_RSVD_M_LSB 0x000f
#define DCTL_PCIE_CFG_RSVD_M_SIZE 0x0001

/** LCAP desc:
  */

#define LCAP_PCIE_CFG_REG                (0x0000004cU)

typedef union {
  struct {
    UINT32 mls : 4; /**< Max Link Speed */
                            /* Bits[3:0], default=0x00000000*/
    UINT32 mlw : 6; /**< Maximum Link Width */
                            /* Bits[9:4], default=0x00000001*/
    UINT32 apms : 2; /**< Active State Link PM Support */
                            /* Bits[11:10], default=0x00000003*/
    UINT32 el0 : 3; /**< L0s Exit Latency */
                            /* Bits[14:12], default=0x00000004*/
    UINT32 el1 : 3; /**< L1 Exit Latency */
                            /* Bits[17:15], default=0x00000002*/
    UINT32 cpm : 1; /**< Clock Power Management */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 sderc : 1; /**< Surprise Down Error Reporting Capable */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 larc : 1; /**< Link Active Reporting Capable */
                            /* Bits[20:20], default=0x00000001*/
    UINT32 lbnc : 1; /**< Link Bandwidth Notification Capability */
                            /* Bits[21:21], default=0x00000001*/
    UINT32 aspmoc : 1; /**< ASPM Optionality Compliance */
                            /* Bits[22:22], default=0x00000001*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[23:23] */
                            /* Bits[23:23], default=0*/
    UINT32 pn : 8; /**< Port Number */
                            /* Bits[31:24], default=0x00000001*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} LCAP_PCIE_CFG_STRUCT;

#define LCAP_PCIE_CFG_WIDTH 32
#define LCAP_PCIE_CFG_DEFAULT 0x01714c10U
#define LCAP_PCIE_CFG_ENDPOINT PCIE_CFG

#define LCAP_PCIE_CFG_MLS_LSB 0x0000
#define LCAP_PCIE_CFG_MLS_SIZE 0x0004
#define LCAP_PCIE_CFG_MLW_LSB 0x0004
#define LCAP_PCIE_CFG_MLW_SIZE 0x0006
#define LCAP_PCIE_CFG_APMS_LSB 0x000a
#define LCAP_PCIE_CFG_APMS_SIZE 0x0002
#define LCAP_PCIE_CFG_EL0_LSB 0x000c
#define LCAP_PCIE_CFG_EL0_SIZE 0x0003
#define LCAP_PCIE_CFG_EL1_LSB 0x000f
#define LCAP_PCIE_CFG_EL1_SIZE 0x0003
#define LCAP_PCIE_CFG_CPM_LSB 0x0012
#define LCAP_PCIE_CFG_CPM_SIZE 0x0001
#define LCAP_PCIE_CFG_SDERC_LSB 0x0013
#define LCAP_PCIE_CFG_SDERC_SIZE 0x0001
#define LCAP_PCIE_CFG_LARC_LSB 0x0014
#define LCAP_PCIE_CFG_LARC_SIZE 0x0001
#define LCAP_PCIE_CFG_LBNC_LSB 0x0015
#define LCAP_PCIE_CFG_LBNC_SIZE 0x0001
#define LCAP_PCIE_CFG_ASPMOC_LSB 0x0016
#define LCAP_PCIE_CFG_ASPMOC_SIZE 0x0001
#define LCAP_PCIE_CFG_PN_LSB 0x0018
#define LCAP_PCIE_CFG_PN_SIZE 0x0008

/** LCTL desc:
  */

#define LCTL_PCIE_CFG_REG                (0x00000050U)

typedef union {
  struct {
    UINT16 aspm : 2; /**< Active State Link PM Control */
                            /* Bits[1:0], default=0x00000000*/
    UINT16 ptmpdaib : 1; /**< PTM Propagation Delay Adaptation Interpretation B */
                            /* Bits[2:2], default=0x00000000*/
    UINT16 rcbc : 1; /**< Read Completion Boundary Control */
                            /* Bits[3:3], default=0x00000000*/
    UINT16 ld : 1; /**< Link Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT16 rl : 1; /**< Retrain Link */
                            /* Bits[5:5], default=0x00000000*/
    UINT16 ccc : 1; /**< Common Clock Configuration */
                            /* Bits[6:6], default=0x00000000*/
    UINT16 es : 1; /**< Extended Synch */
                            /* Bits[7:7], default=0x00000000*/
    UINT16 ecpm : 1; /**< Enable Clock Power Management */
                            /* Bits[8:8], default=0x00000000*/
    UINT16 hawd : 1; /**< Hardware Autonomous Width Disable */
                            /* Bits[9:9], default=0x00000000*/
    UINT16 lbmie : 1; /**< Link Bandwidth Management Interrupt Enable */
                            /* Bits[10:10], default=0x00000000*/
    UINT16 labie : 1; /**< Link Autonomous Bandwidth Interrupt Enable */
                            /* Bits[11:11], default=0x00000000*/
    UINT16 rsvd_m : 4; /**< Reserved */
                            /* Bits[15:12], default=0x00000000*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} LCTL_PCIE_CFG_STRUCT;

#define LCTL_PCIE_CFG_WIDTH 16
#define LCTL_PCIE_CFG_DEFAULT 0x0000U
#define LCTL_PCIE_CFG_ENDPOINT PCIE_CFG

#define LCTL_PCIE_CFG_ASPM_LSB 0x0000
#define LCTL_PCIE_CFG_ASPM_SIZE 0x0002
#define LCTL_PCIE_CFG_PTMPDAIB_LSB 0x0002
#define LCTL_PCIE_CFG_PTMPDAIB_SIZE 0x0001
#define LCTL_PCIE_CFG_RCBC_LSB 0x0003
#define LCTL_PCIE_CFG_RCBC_SIZE 0x0001
#define LCTL_PCIE_CFG_LD_LSB 0x0004
#define LCTL_PCIE_CFG_LD_SIZE 0x0001
#define LCTL_PCIE_CFG_RL_LSB 0x0005
#define LCTL_PCIE_CFG_RL_SIZE 0x0001
#define LCTL_PCIE_CFG_CCC_LSB 0x0006
#define LCTL_PCIE_CFG_CCC_SIZE 0x0001
#define LCTL_PCIE_CFG_ES_LSB 0x0007
#define LCTL_PCIE_CFG_ES_SIZE 0x0001
#define LCTL_PCIE_CFG_ECPM_LSB 0x0008
#define LCTL_PCIE_CFG_ECPM_SIZE 0x0001
#define LCTL_PCIE_CFG_HAWD_LSB 0x0009
#define LCTL_PCIE_CFG_HAWD_SIZE 0x0001
#define LCTL_PCIE_CFG_LBMIE_LSB 0x000a
#define LCTL_PCIE_CFG_LBMIE_SIZE 0x0001
#define LCTL_PCIE_CFG_LABIE_LSB 0x000b
#define LCTL_PCIE_CFG_LABIE_SIZE 0x0001
#define LCTL_PCIE_CFG_RSVD_M_LSB 0x000c
#define LCTL_PCIE_CFG_RSVD_M_SIZE 0x0004

/** LSTS desc:
  */

#define LSTS_PCIE_CFG_REG                (0x00000052U)

typedef union {
  struct {
    UINT16 cls : 4; /**< Current Link Speed */
                            /* Bits[3:0], default=0x00000001*/
    UINT16 nlw : 6; /**< Negotiated Link Width */
                            /* Bits[9:4], default=0x00000001*/
    UINT16 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[10:10] */
                            /* Bits[10:10], default=0*/
    UINT16 lt : 1; /**< Link Training */
                            /* Bits[11:11], default=0x00000000*/
    UINT16 scc : 1; /**< Slot Clock Configuration */
                            /* Bits[12:12], default=0x00000001*/
    UINT16 la : 1; /**< Link Active */
                            /* Bits[13:13], default=0x00000000*/
    UINT16 lbms : 1; /**< Link Bandwidth Management Status */
                            /* Bits[14:14], default=0x00000000*/
    UINT16 labs : 1; /**< Link Autonomous Bandwidth Status */
                            /* Bits[15:15], default=0x00000000*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} LSTS_PCIE_CFG_STRUCT;

#define LSTS_PCIE_CFG_WIDTH 16
#define LSTS_PCIE_CFG_DEFAULT 0x1011U
#define LSTS_PCIE_CFG_ENDPOINT PCIE_CFG

#define LSTS_PCIE_CFG_CLS_LSB 0x0000
#define LSTS_PCIE_CFG_CLS_SIZE 0x0004
#define LSTS_PCIE_CFG_NLW_LSB 0x0004
#define LSTS_PCIE_CFG_NLW_SIZE 0x0006
#define LSTS_PCIE_CFG_LT_LSB 0x000b
#define LSTS_PCIE_CFG_LT_SIZE 0x0001
#define LSTS_PCIE_CFG_SCC_LSB 0x000c
#define LSTS_PCIE_CFG_SCC_SIZE 0x0001
#define LSTS_PCIE_CFG_LA_LSB 0x000d
#define LSTS_PCIE_CFG_LA_SIZE 0x0001
#define LSTS_PCIE_CFG_LBMS_LSB 0x000e
#define LSTS_PCIE_CFG_LBMS_SIZE 0x0001
#define LSTS_PCIE_CFG_LABS_LSB 0x000f
#define LSTS_PCIE_CFG_LABS_SIZE 0x0001

/** SLCAP desc:
  */

#define SLCAP_PCIE_CFG_REG               (0x00000054U)

typedef union {
  struct {
    UINT32 abp : 1; /**< Attention Button Present */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 pcp : 1; /**< Power Controller Present */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 msp : 1; /**< MRL Sensor Present */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 aip : 1; /**< Attention Indicator Present */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 pip : 1; /**< Power Indicator Present */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 hps : 1; /**< Hot Plug Surprise */
                            /* Bits[5:5], default=0x00000001*/
    UINT32 hpc : 1; /**< Hot Plug Capable */
                            /* Bits[6:6], default=0x00000001*/
    UINT32 slv : 8; /**< Slot Power Limit Value */
                            /* Bits[14:7], default=0x00000000*/
    UINT32 sls : 2; /**< Slot Power Limit Scale */
                            /* Bits[16:15], default=0x00000000*/
    UINT32 emip : 1; /**< Electromechanical Interlock Present */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 nccs : 1; /**< No Command Completed Support */
                            /* Bits[18:18], default=0x00000001*/
    UINT32 psn : 13; /**< Physical Slot Number */
                            /* Bits[31:19], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} SLCAP_PCIE_CFG_STRUCT;

#define SLCAP_PCIE_CFG_WIDTH 32
#define SLCAP_PCIE_CFG_DEFAULT 0x00040060U
#define SLCAP_PCIE_CFG_ENDPOINT PCIE_CFG

#define SLCAP_PCIE_CFG_ABP_LSB 0x0000
#define SLCAP_PCIE_CFG_ABP_SIZE 0x0001
#define SLCAP_PCIE_CFG_PCP_LSB 0x0001
#define SLCAP_PCIE_CFG_PCP_SIZE 0x0001
#define SLCAP_PCIE_CFG_MSP_LSB 0x0002
#define SLCAP_PCIE_CFG_MSP_SIZE 0x0001
#define SLCAP_PCIE_CFG_AIP_LSB 0x0003
#define SLCAP_PCIE_CFG_AIP_SIZE 0x0001
#define SLCAP_PCIE_CFG_PIP_LSB 0x0004
#define SLCAP_PCIE_CFG_PIP_SIZE 0x0001
#define SLCAP_PCIE_CFG_HPS_LSB 0x0005
#define SLCAP_PCIE_CFG_HPS_SIZE 0x0001
#define SLCAP_PCIE_CFG_HPC_LSB 0x0006
#define SLCAP_PCIE_CFG_HPC_SIZE 0x0001
#define SLCAP_PCIE_CFG_SLV_LSB 0x0007
#define SLCAP_PCIE_CFG_SLV_SIZE 0x0008
#define SLCAP_PCIE_CFG_SLS_LSB 0x000f
#define SLCAP_PCIE_CFG_SLS_SIZE 0x0002
#define SLCAP_PCIE_CFG_EMIP_LSB 0x0011
#define SLCAP_PCIE_CFG_EMIP_SIZE 0x0001
#define SLCAP_PCIE_CFG_NCCS_LSB 0x0012
#define SLCAP_PCIE_CFG_NCCS_SIZE 0x0001
#define SLCAP_PCIE_CFG_PSN_LSB 0x0013
#define SLCAP_PCIE_CFG_PSN_SIZE 0x000d

/** SLCTL desc:
  */

#define SLCTL_PCIE_CFG_REG               (0x00000058U)

typedef union {
  struct {
    UINT16 abe : 1; /**< Attention Button Pressed Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT16 pfe : 1; /**< Power Fault Detected Enable */
                            /* Bits[1:1], default=0x00000000*/
    UINT16 mse : 1; /**< MRL Sensor Changed Enable */
                            /* Bits[2:2], default=0x00000000*/
    UINT16 pde : 1; /**< Presence Detect Changed Enable */
                            /* Bits[3:3], default=0x00000000*/
    UINT16 cce : 1; /**< Command Completed Interrupt Enable */
                            /* Bits[4:4], default=0x00000000*/
    UINT16 hpe : 1; /**< Hot Plug Interrupt Enable */
                            /* Bits[5:5], default=0x00000000*/
    UINT16 aic : 2; /**< Attention Indicator Control */
                            /* Bits[7:6], default=0x00000000*/
    UINT16 pic : 2; /**< Power Indicator Control */
                            /* Bits[9:8], default=0x00000000*/
    UINT16 pcc : 1; /**< Power Controller Control */
                            /* Bits[10:10], default=0x00000000*/
    UINT16 emic : 1; /**< Electromechanical Interlock Control */
                            /* Bits[11:11], default=0x00000000*/
    UINT16 dllsce : 1; /**< Data Link Layer State Changed Enable */
                            /* Bits[12:12], default=0x00000000*/
    UINT16 aspld : 1; /**< Auto Slot Power Limit Disable */
                            /* Bits[13:13], default=0x00000000*/
    UINT16 rsvd_m : 2; /**< Reserved */
                            /* Bits[15:14], default=0x00000000*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SLCTL_PCIE_CFG_STRUCT;

#define SLCTL_PCIE_CFG_WIDTH 16
#define SLCTL_PCIE_CFG_DEFAULT 0x0000U
#define SLCTL_PCIE_CFG_ENDPOINT PCIE_CFG

#define SLCTL_PCIE_CFG_ABE_LSB 0x0000
#define SLCTL_PCIE_CFG_ABE_SIZE 0x0001
#define SLCTL_PCIE_CFG_PFE_LSB 0x0001
#define SLCTL_PCIE_CFG_PFE_SIZE 0x0001
#define SLCTL_PCIE_CFG_MSE_LSB 0x0002
#define SLCTL_PCIE_CFG_MSE_SIZE 0x0001
#define SLCTL_PCIE_CFG_PDE_LSB 0x0003
#define SLCTL_PCIE_CFG_PDE_SIZE 0x0001
#define SLCTL_PCIE_CFG_CCE_LSB 0x0004
#define SLCTL_PCIE_CFG_CCE_SIZE 0x0001
#define SLCTL_PCIE_CFG_HPE_LSB 0x0005
#define SLCTL_PCIE_CFG_HPE_SIZE 0x0001
#define SLCTL_PCIE_CFG_AIC_LSB 0x0006
#define SLCTL_PCIE_CFG_AIC_SIZE 0x0002
#define SLCTL_PCIE_CFG_PIC_LSB 0x0008
#define SLCTL_PCIE_CFG_PIC_SIZE 0x0002
#define SLCTL_PCIE_CFG_PCC_LSB 0x000a
#define SLCTL_PCIE_CFG_PCC_SIZE 0x0001
#define SLCTL_PCIE_CFG_EMIC_LSB 0x000b
#define SLCTL_PCIE_CFG_EMIC_SIZE 0x0001
#define SLCTL_PCIE_CFG_DLLSCE_LSB 0x000c
#define SLCTL_PCIE_CFG_DLLSCE_SIZE 0x0001
#define SLCTL_PCIE_CFG_ASPLD_LSB 0x000d
#define SLCTL_PCIE_CFG_ASPLD_SIZE 0x0001
#define SLCTL_PCIE_CFG_RSVD_M_LSB 0x000e
#define SLCTL_PCIE_CFG_RSVD_M_SIZE 0x0002

/** SLSTS desc:
  */

#define SLSTS_PCIE_CFG_REG               (0x0000005aU)

typedef union {
  struct {
    UINT16 abp : 1; /**< Attention Button Pressed */
                            /* Bits[0:0], default=0x00000000*/
    UINT16 pfd : 1; /**< Power Fault Detected */
                            /* Bits[1:1], default=0x00000000*/
    UINT16 msc : 1; /**< MRL Sensor Changed */
                            /* Bits[2:2], default=0x00000000*/
    UINT16 pdc : 1; /**< Presence Detect Changed */
                            /* Bits[3:3], default=0x00000000*/
    UINT16 cc : 1; /**< Command Completed */
                            /* Bits[4:4], default=0x00000000*/
    UINT16 ms : 1; /**< MRL Sensor State */
                            /* Bits[5:5], default=0x00000000*/
    UINT16 pds : 1; /**< Presence Detect State */
                            /* Bits[6:6], default=0x00000000*/
    UINT16 emis : 1; /**< Electromechanical Interlock Status */
                            /* Bits[7:7], default=0x00000000*/
    UINT16 dllsc : 1; /**< Data Link Layer State Changed */
                            /* Bits[8:8], default=0x00000000*/
    UINT16 rsvd_m : 7; /**< Reserved */
                            /* Bits[15:9], default=0x00000000*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SLSTS_PCIE_CFG_STRUCT;

#define SLSTS_PCIE_CFG_WIDTH 16
#define SLSTS_PCIE_CFG_DEFAULT 0x0000U
#define SLSTS_PCIE_CFG_ENDPOINT PCIE_CFG

#define SLSTS_PCIE_CFG_ABP_LSB 0x0000
#define SLSTS_PCIE_CFG_ABP_SIZE 0x0001
#define SLSTS_PCIE_CFG_PFD_LSB 0x0001
#define SLSTS_PCIE_CFG_PFD_SIZE 0x0001
#define SLSTS_PCIE_CFG_MSC_LSB 0x0002
#define SLSTS_PCIE_CFG_MSC_SIZE 0x0001
#define SLSTS_PCIE_CFG_PDC_LSB 0x0003
#define SLSTS_PCIE_CFG_PDC_SIZE 0x0001
#define SLSTS_PCIE_CFG_CC_LSB 0x0004
#define SLSTS_PCIE_CFG_CC_SIZE 0x0001
#define SLSTS_PCIE_CFG_MS_LSB 0x0005
#define SLSTS_PCIE_CFG_MS_SIZE 0x0001
#define SLSTS_PCIE_CFG_PDS_LSB 0x0006
#define SLSTS_PCIE_CFG_PDS_SIZE 0x0001
#define SLSTS_PCIE_CFG_EMIS_LSB 0x0007
#define SLSTS_PCIE_CFG_EMIS_SIZE 0x0001
#define SLSTS_PCIE_CFG_DLLSC_LSB 0x0008
#define SLSTS_PCIE_CFG_DLLSC_SIZE 0x0001
#define SLSTS_PCIE_CFG_RSVD_M_LSB 0x0009
#define SLSTS_PCIE_CFG_RSVD_M_SIZE 0x0007

/** RCTL desc:
  */

#define RCTL_PCIE_CFG_REG                (0x0000005cU)

typedef union {
  struct {
    UINT16 sce : 1; /**< System Error on Correctable Error Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT16 sne : 1; /**< System Error on Non-Fatal Error Enable */
                            /* Bits[1:1], default=0x00000000*/
    UINT16 sfe : 1; /**< System Error on Fatal Error Enable */
                            /* Bits[2:2], default=0x00000000*/
    UINT16 pie : 1; /**< PME Interrupt Enable */
                            /* Bits[3:3], default=0x00000000*/
    UINT16 crssve : 1; /**< CRS Software Visibility Enable */
                            /* Bits[4:4], default=0x00000000*/
    UINT16 rsvd_m : 11; /**< Reserved */
                            /* Bits[15:5], default=0x00000000*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} RCTL_PCIE_CFG_STRUCT;

#define RCTL_PCIE_CFG_WIDTH 16
#define RCTL_PCIE_CFG_DEFAULT 0x0000U
#define RCTL_PCIE_CFG_ENDPOINT PCIE_CFG

#define RCTL_PCIE_CFG_SCE_LSB 0x0000
#define RCTL_PCIE_CFG_SCE_SIZE 0x0001
#define RCTL_PCIE_CFG_SNE_LSB 0x0001
#define RCTL_PCIE_CFG_SNE_SIZE 0x0001
#define RCTL_PCIE_CFG_SFE_LSB 0x0002
#define RCTL_PCIE_CFG_SFE_SIZE 0x0001
#define RCTL_PCIE_CFG_PIE_LSB 0x0003
#define RCTL_PCIE_CFG_PIE_SIZE 0x0001
#define RCTL_PCIE_CFG_CRSSVE_LSB 0x0004
#define RCTL_PCIE_CFG_CRSSVE_SIZE 0x0001
#define RCTL_PCIE_CFG_RSVD_M_LSB 0x0005
#define RCTL_PCIE_CFG_RSVD_M_SIZE 0x000b

/** DCAP2 desc:
  */

#define DCAP2_PCIE_CFG_REG               (0x00000064U)

typedef union {
  struct {
    UINT32 ctrs : 4; /**< Completion Timeout Ranges Supported */
                            /* Bits[3:0], default=0x00000007*/
    UINT32 ctds : 1; /**< Completion Timeout Disable Supported */
                            /* Bits[4:4], default=0x00000001*/
    UINT32 afs : 1; /**< ARI Forwarding Supported */
                            /* Bits[5:5], default=0x00000001*/
    UINT32 ars : 1; /**< Atomic Routing Supported */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 ac32bs : 1; /**< AtomicOp Completer 32-bit Supported */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 ac64bs : 1; /**< AtomicOp Completer 64-bit Supported */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 ac128bs : 1; /**< CAS Completer 128-bit Supported */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[10:10] */
                            /* Bits[10:10], default=0*/
    UINT32 ltrms : 1; /**< LTR Mechanism Supported */
                            /* Bits[11:11], default=0x00000001*/
    UINT32 rsvd_1 : 4; /**< Undefined - auto filled rsvd_[15:12] */
                            /* Bits[15:12], default=0*/
    UINT32 px10btcs : 1; /**< 10-Bit Tag Completer Supported */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 px10btrs : 1; /**< 10-Bit Tag Requester Supported */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 obffs : 2; /**< Optimized Buffer Flush/Fill Supported */
                            /* Bits[19:18], default=0x00000002*/
    UINT32 effs : 1; /**< Extended Fmt Field Supported */
                            /* Bits[20:20], default=0x00000001*/
    UINT32 eetlpps : 1; /**< End-End TLP Prefix Supported */
                            /* Bits[21:21], default=0x00000001*/
    UINT32 meetlpp : 2; /**< Max End-End TLP Prefixes */
                            /* Bits[23:22], default=0x00000002*/
    UINT32 rsvd_m : 8; /**< Reserved */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DCAP2_PCIE_CFG_STRUCT;

#define DCAP2_PCIE_CFG_WIDTH 32
#define DCAP2_PCIE_CFG_DEFAULT 0x00b80837U
#define DCAP2_PCIE_CFG_ENDPOINT PCIE_CFG

#define DCAP2_PCIE_CFG_CTRS_LSB 0x0000
#define DCAP2_PCIE_CFG_CTRS_SIZE 0x0004
#define DCAP2_PCIE_CFG_CTDS_LSB 0x0004
#define DCAP2_PCIE_CFG_CTDS_SIZE 0x0001
#define DCAP2_PCIE_CFG_AFS_LSB 0x0005
#define DCAP2_PCIE_CFG_AFS_SIZE 0x0001
#define DCAP2_PCIE_CFG_ARS_LSB 0x0006
#define DCAP2_PCIE_CFG_ARS_SIZE 0x0001
#define DCAP2_PCIE_CFG_AC32BS_LSB 0x0007
#define DCAP2_PCIE_CFG_AC32BS_SIZE 0x0001
#define DCAP2_PCIE_CFG_AC64BS_LSB 0x0008
#define DCAP2_PCIE_CFG_AC64BS_SIZE 0x0001
#define DCAP2_PCIE_CFG_AC128BS_LSB 0x0009
#define DCAP2_PCIE_CFG_AC128BS_SIZE 0x0001
#define DCAP2_PCIE_CFG_LTRMS_LSB 0x000b
#define DCAP2_PCIE_CFG_LTRMS_SIZE 0x0001
#define DCAP2_PCIE_CFG_PX10BTCS_LSB 0x0010
#define DCAP2_PCIE_CFG_PX10BTCS_SIZE 0x0001
#define DCAP2_PCIE_CFG_PX10BTRS_LSB 0x0011
#define DCAP2_PCIE_CFG_PX10BTRS_SIZE 0x0001
#define DCAP2_PCIE_CFG_OBFFS_LSB 0x0012
#define DCAP2_PCIE_CFG_OBFFS_SIZE 0x0002
#define DCAP2_PCIE_CFG_EFFS_LSB 0x0014
#define DCAP2_PCIE_CFG_EFFS_SIZE 0x0001
#define DCAP2_PCIE_CFG_EETLPPS_LSB 0x0015
#define DCAP2_PCIE_CFG_EETLPPS_SIZE 0x0001
#define DCAP2_PCIE_CFG_MEETLPP_LSB 0x0016
#define DCAP2_PCIE_CFG_MEETLPP_SIZE 0x0002
#define DCAP2_PCIE_CFG_RSVD_M_LSB 0x0018
#define DCAP2_PCIE_CFG_RSVD_M_SIZE 0x0008

/** DCTL2 desc:
  */

#define DCTL2_PCIE_CFG_REG               (0x00000068U)

typedef union {
  struct {
    UINT16 ctv : 4; /**< Completion Timeout Value */
                            /* Bits[3:0], default=0x00000000*/
    UINT16 ctd : 1; /**< Completion Timeout Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT16 afe : 1; /**< ARI Forwarding Enable */
                            /* Bits[5:5], default=0x00000000*/
    UINT16 are : 1; /**< AtomicOp Requester Enable */
                            /* Bits[6:6], default=0x00000000*/
    UINT16 aeb : 1; /**< AtomicOp Egress Blocking */
                            /* Bits[7:7], default=0x00000000*/
    UINT16 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[09:08] */
                            /* Bits[9:8], default=0*/
    UINT16 ltren : 1; /**< LTR Mechanism Enable */
                            /* Bits[10:10], default=0x00000000*/
    UINT16 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[11:11] */
                            /* Bits[11:11], default=0*/
    UINT16 px10btre : 1; /**< 10-Bit Tag Requester Enable */
                            /* Bits[12:12], default=0x00000000*/
    UINT16 obffen : 2; /**< Optimized Buffer Flush/Fill Enable */
                            /* Bits[14:13], default=0x00000000*/
    UINT16 eetlppb : 1; /**< End-End TLP Prefix Blocking */
                            /* Bits[15:15], default=0x00000000*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} DCTL2_PCIE_CFG_STRUCT;

#define DCTL2_PCIE_CFG_WIDTH 16
#define DCTL2_PCIE_CFG_DEFAULT 0x0000U
#define DCTL2_PCIE_CFG_ENDPOINT PCIE_CFG

#define DCTL2_PCIE_CFG_CTV_LSB 0x0000
#define DCTL2_PCIE_CFG_CTV_SIZE 0x0004
#define DCTL2_PCIE_CFG_CTD_LSB 0x0004
#define DCTL2_PCIE_CFG_CTD_SIZE 0x0001
#define DCTL2_PCIE_CFG_AFE_LSB 0x0005
#define DCTL2_PCIE_CFG_AFE_SIZE 0x0001
#define DCTL2_PCIE_CFG_ARE_LSB 0x0006
#define DCTL2_PCIE_CFG_ARE_SIZE 0x0001
#define DCTL2_PCIE_CFG_AEB_LSB 0x0007
#define DCTL2_PCIE_CFG_AEB_SIZE 0x0001
#define DCTL2_PCIE_CFG_LTREN_LSB 0x000a
#define DCTL2_PCIE_CFG_LTREN_SIZE 0x0001
#define DCTL2_PCIE_CFG_PX10BTRE_LSB 0x000c
#define DCTL2_PCIE_CFG_PX10BTRE_SIZE 0x0001
#define DCTL2_PCIE_CFG_OBFFEN_LSB 0x000d
#define DCTL2_PCIE_CFG_OBFFEN_SIZE 0x0002
#define DCTL2_PCIE_CFG_EETLPPB_LSB 0x000f
#define DCTL2_PCIE_CFG_EETLPPB_SIZE 0x0001

/** LCAP2 desc:
  */

#define LCAP2_PCIE_CFG_REG               (0x0000006cU)

typedef union {
  struct {
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[00:00] */
                            /* Bits[0:0], default=0*/
    UINT32 slsv : 7; /**< Supported Link Speeds Vector */
                            /* Bits[7:1], default=0x00000007*/
    UINT32 cs : 1; /**< Crosslink Supported */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 lsosgssv : 7; /**< Lower SKP OS Generation Supported Speeds Vector */
                            /* Bits[15:9], default=0x00000000*/
    UINT32 lsosrss : 7; /**< Lower SKP OS Reception Supported Speeds Vector */
                            /* Bits[22:16], default=0x00000000*/
    UINT32 rpds : 1; /**< Retimer Presence Detect Supported */
                            /* Bits[23:23], default=0x00000001*/
    UINT32 trpds : 1; /**< Two Retimers Presence Detect Supported */
                            /* Bits[24:24], default=0x00000001*/
    UINT32 rsvd_m : 7; /**< Reserved */
                            /* Bits[31:25], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} LCAP2_PCIE_CFG_STRUCT;

#define LCAP2_PCIE_CFG_WIDTH 32
#define LCAP2_PCIE_CFG_DEFAULT 0x0180000eU
#define LCAP2_PCIE_CFG_ENDPOINT PCIE_CFG

#define LCAP2_PCIE_CFG_SLSV_LSB 0x0001
#define LCAP2_PCIE_CFG_SLSV_SIZE 0x0007
#define LCAP2_PCIE_CFG_CS_LSB 0x0008
#define LCAP2_PCIE_CFG_CS_SIZE 0x0001
#define LCAP2_PCIE_CFG_LSOSGSSV_LSB 0x0009
#define LCAP2_PCIE_CFG_LSOSGSSV_SIZE 0x0007
#define LCAP2_PCIE_CFG_LSOSRSS_LSB 0x0010
#define LCAP2_PCIE_CFG_LSOSRSS_SIZE 0x0007
#define LCAP2_PCIE_CFG_RPDS_LSB 0x0017
#define LCAP2_PCIE_CFG_RPDS_SIZE 0x0001
#define LCAP2_PCIE_CFG_TRPDS_LSB 0x0018
#define LCAP2_PCIE_CFG_TRPDS_SIZE 0x0001
#define LCAP2_PCIE_CFG_RSVD_M_LSB 0x0019
#define LCAP2_PCIE_CFG_RSVD_M_SIZE 0x0007

/** LCTL2 desc:
  */

#define LCTL2_PCIE_CFG_REG               (0x00000070U)

typedef union {
  struct {
    UINT16 tls : 4; /**< Target Link Speed */
                            /* Bits[3:0], default=0x00000001*/
    UINT16 ec : 1; /**< Enter Compliance */
                            /* Bits[4:4], default=0x00000000*/
    UINT16 hasd : 1; /**< Reserved. Hardware Autonomous Speed Disable */
                            /* Bits[5:5], default=0x00000000*/
    UINT16 sd : 1; /**< Selectable De-emphasis */
                            /* Bits[6:6], default=0x00000000*/
    UINT16 tm : 3; /**< Transmit Margin */
                            /* Bits[9:7], default=0x00000000*/
    UINT16 emc : 1; /**< Enter Modified Compliance */
                            /* Bits[10:10], default=0x00000000*/
    UINT16 csos : 1; /**< Compliance SOS */
                            /* Bits[11:11], default=0x00000000*/
    UINT16 cd : 4; /**< Compliance Preset/De-emphasis */
                            /* Bits[15:12], default=0x00000000*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} LCTL2_PCIE_CFG_STRUCT;

#define LCTL2_PCIE_CFG_WIDTH 16
#define LCTL2_PCIE_CFG_DEFAULT 0x0001U
#define LCTL2_PCIE_CFG_ENDPOINT PCIE_CFG

#define LCTL2_PCIE_CFG_TLS_LSB 0x0000
#define LCTL2_PCIE_CFG_TLS_SIZE 0x0004
#define LCTL2_PCIE_CFG_EC_LSB 0x0004
#define LCTL2_PCIE_CFG_EC_SIZE 0x0001
#define LCTL2_PCIE_CFG_HASD_LSB 0x0005
#define LCTL2_PCIE_CFG_HASD_SIZE 0x0001
#define LCTL2_PCIE_CFG_SD_LSB 0x0006
#define LCTL2_PCIE_CFG_SD_SIZE 0x0001
#define LCTL2_PCIE_CFG_TM_LSB 0x0007
#define LCTL2_PCIE_CFG_TM_SIZE 0x0003
#define LCTL2_PCIE_CFG_EMC_LSB 0x000a
#define LCTL2_PCIE_CFG_EMC_SIZE 0x0001
#define LCTL2_PCIE_CFG_CSOS_LSB 0x000b
#define LCTL2_PCIE_CFG_CSOS_SIZE 0x0001
#define LCTL2_PCIE_CFG_CD_LSB 0x000c
#define LCTL2_PCIE_CFG_CD_SIZE 0x0004

/** LSTS2 desc:
  */

#define LSTS2_PCIE_CFG_REG               (0x00000072U)

typedef union {
  struct {
    UINT16 cdl : 1; /**< Current De-emphasis Level */
                            /* Bits[0:0], default=0x00000000*/
    UINT16 eqc : 1; /**< Equalization Complete */
                            /* Bits[1:1], default=0x00000000*/
    UINT16 eqp1s : 1; /**< Equalization Phase 1 Successful */
                            /* Bits[2:2], default=0x00000000*/
    UINT16 eqp2s : 1; /**< Equalization Phase 2 Successful */
                            /* Bits[3:3], default=0x00000000*/
    UINT16 eqp3s : 1; /**< Equalization Phase 3 Successful */
                            /* Bits[4:4], default=0x00000000*/
    UINT16 ler : 1; /**< Link Equalization Request */
                            /* Bits[5:5], default=0x00000000*/
    UINT16 rpd : 1; /**< Retimer Presence Detected */
                            /* Bits[6:6], default=0x00000000*/
    UINT16 px2rpd : 1; /**< Two Retimers Presence Detected */
                            /* Bits[7:7], default=0x00000000*/
    UINT16 rsvd_m : 8; /**< Reserved */
                            /* Bits[15:8], default=0x00000000*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} LSTS2_PCIE_CFG_STRUCT;

#define LSTS2_PCIE_CFG_WIDTH 16
#define LSTS2_PCIE_CFG_DEFAULT 0x0000U
#define LSTS2_PCIE_CFG_ENDPOINT PCIE_CFG

#define LSTS2_PCIE_CFG_CDL_LSB 0x0000
#define LSTS2_PCIE_CFG_CDL_SIZE 0x0001
#define LSTS2_PCIE_CFG_EQC_LSB 0x0001
#define LSTS2_PCIE_CFG_EQC_SIZE 0x0001
#define LSTS2_PCIE_CFG_EQP1S_LSB 0x0002
#define LSTS2_PCIE_CFG_EQP1S_SIZE 0x0001
#define LSTS2_PCIE_CFG_EQP2S_LSB 0x0003
#define LSTS2_PCIE_CFG_EQP2S_SIZE 0x0001
#define LSTS2_PCIE_CFG_EQP3S_LSB 0x0004
#define LSTS2_PCIE_CFG_EQP3S_SIZE 0x0001
#define LSTS2_PCIE_CFG_LER_LSB 0x0005
#define LSTS2_PCIE_CFG_LER_SIZE 0x0001
#define LSTS2_PCIE_CFG_RPD_LSB 0x0006
#define LSTS2_PCIE_CFG_RPD_SIZE 0x0001
#define LSTS2_PCIE_CFG_PX2RPD_LSB 0x0007
#define LSTS2_PCIE_CFG_PX2RPD_SIZE 0x0001
#define LSTS2_PCIE_CFG_RSVD_M_LSB 0x0008
#define LSTS2_PCIE_CFG_RSVD_M_SIZE 0x0008

/** MID desc:
  */

#define MID_PCIE_CFG_REG                 (0x00000080U)

typedef union {
  struct {
    UINT16 cid : 8; /**< Capability ID */
                            /* Bits[7:0], default=0x00000005*/
    UINT16 next : 8; /**< Next Pointer */
                            /* Bits[15:8], default=0x00000090*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} MID_PCIE_CFG_STRUCT;

#define MID_PCIE_CFG_WIDTH 16
#define MID_PCIE_CFG_DEFAULT 0x9005U
#define MID_PCIE_CFG_ENDPOINT PCIE_CFG

#define MID_PCIE_CFG_CID_LSB 0x0000
#define MID_PCIE_CFG_CID_SIZE 0x0008
#define MID_PCIE_CFG_NEXT_LSB 0x0008
#define MID_PCIE_CFG_NEXT_SIZE 0x0008

/** SVCAP desc:
  */

#define SVCAP_PCIE_CFG_REG               (0x00000098U)

typedef union {
  struct {
    UINT16 cid : 8; /**< Capability Identifier */
                            /* Bits[7:0], default=0x0000000D*/
    UINT16 next : 8; /**< Next Capability */
                            /* Bits[15:8], default=0x000000A0*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SVCAP_PCIE_CFG_STRUCT;

#define SVCAP_PCIE_CFG_WIDTH 16
#define SVCAP_PCIE_CFG_DEFAULT 0xa00dU
#define SVCAP_PCIE_CFG_ENDPOINT PCIE_CFG

#define SVCAP_PCIE_CFG_CID_LSB 0x0000
#define SVCAP_PCIE_CFG_CID_SIZE 0x0008
#define SVCAP_PCIE_CFG_NEXT_LSB 0x0008
#define SVCAP_PCIE_CFG_NEXT_SIZE 0x0008

/** PMCS desc:
  */

#define PMCS_PCIE_CFG_REG                (0x000000a4U)

typedef union {
  struct {
    UINT32 ps : 2; /**< Power State */
                            /* Bits[1:0], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[02:02] */
                            /* Bits[2:2], default=0*/
    UINT32 nsr : 1; /**< No Soft Reset */
                            /* Bits[3:3], default=0x00000001*/
    UINT32 rsvd_1 : 4; /**< Undefined - auto filled rsvd_[07:04] */
                            /* Bits[7:4], default=0*/
    UINT32 pmee : 1; /**< PME Enable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 dsel : 4; /**< Data Select */
                            /* Bits[12:9], default=0x00000000*/
    UINT32 dsc : 2; /**< Data Scale */
                            /* Bits[14:13], default=0x00000000*/
    UINT32 pmes : 1; /**< PME Status */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 rsvd_2 : 6; /**< Undefined - auto filled rsvd_[21:16] */
                            /* Bits[21:16], default=0*/
    UINT32 b23s : 1; /**< B2/B3 Support */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 bpce : 1; /**< Bus Power / Clock Control Enable */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 dta : 8; /**< PME Data */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PMCS_PCIE_CFG_STRUCT;

#define PMCS_PCIE_CFG_WIDTH 32
#define PMCS_PCIE_CFG_DEFAULT 0x00000008U
#define PMCS_PCIE_CFG_ENDPOINT PCIE_CFG

#define PMCS_PCIE_CFG_PS_LSB 0x0000
#define PMCS_PCIE_CFG_PS_SIZE 0x0002
#define PMCS_PCIE_CFG_NSR_LSB 0x0003
#define PMCS_PCIE_CFG_NSR_SIZE 0x0001
#define PMCS_PCIE_CFG_PMEE_LSB 0x0008
#define PMCS_PCIE_CFG_PMEE_SIZE 0x0001
#define PMCS_PCIE_CFG_DSEL_LSB 0x0009
#define PMCS_PCIE_CFG_DSEL_SIZE 0x0004
#define PMCS_PCIE_CFG_DSC_LSB 0x000d
#define PMCS_PCIE_CFG_DSC_SIZE 0x0002
#define PMCS_PCIE_CFG_PMES_LSB 0x000f
#define PMCS_PCIE_CFG_PMES_SIZE 0x0001
#define PMCS_PCIE_CFG_B23S_LSB 0x0016
#define PMCS_PCIE_CFG_B23S_SIZE 0x0001
#define PMCS_PCIE_CFG_BPCE_LSB 0x0017
#define PMCS_PCIE_CFG_BPCE_SIZE 0x0001
#define PMCS_PCIE_CFG_DTA_LSB 0x0018
#define PMCS_PCIE_CFG_DTA_SIZE 0x0008


/** CCFG desc:
  */

#define CCFG_PCIE_CFG_REG                (0x000000d0U)

typedef union {
  struct {
    UINT32 uprs : 3; /**< Upstream Posted Request Size */
                            /* Bits[2:0], default=0x00000001*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[03:03] */
                            /* Bits[3:3], default=0*/
    UINT32 unrs : 3; /**< Upstream Non-Posted Request Size */
                            /* Bits[6:4], default=0x00000001*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[07:07] */
                            /* Bits[7:7], default=0*/
    UINT32 mrpc : 1; /**< Minimum Receive Posted Credits */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 mrnpc : 1; /**< Minimum Receive Non-Posted Credits */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 upsaap : 1; /**< Upstream Posted Split Address Align Policy */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 rbms : 1; /**< Retry Buffers Minimum Size */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 unrd : 2; /**< Upstream Non-Posted Request Delay */
                            /* Bits[13:12], default=0x00000000*/
    UINT32 unpsaap : 1; /**< Upstream Non Posted Split Address Align Policy */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 dcgeisma : 1; /**< Dynamic Clock Gating Enable on ISM Active */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 npap : 1; /**< Non-Posted Pre-Allocation Policy */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 npas : 6; /**< Non-Posted Pre-Allocation Size */
                            /* Bits[22:17], default=0x00000000*/
    UINT32 unsd : 1; /**< Upstream Non-Posted Split Disable */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 upsd : 1; /**< Upstream Posted Split Disable */
                            /* Bits[24:24], default=0x00000001*/
    UINT32 upmwpd : 1; /**< Upstream Posted Memory Write Peer Disable */
                            /* Bits[25:25], default=0x00000001*/
    UINT32 umrpd : 1; /**< Upstream Memory Read Peer Disable */
                            /* Bits[26:26], default=0x00000001*/
    UINT32 uarpd : 1; /**< Upstream Atomic Request Peer Disable */
                            /* Bits[27:27], default=0x00000001*/
    UINT32 ufdm : 1; /**< Upstream Fabric Decode Mode */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[29:29] */
                            /* Bits[29:29], default=0*/
    UINT32 iore : 1; /**< I/O Receive Enabled */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 cre : 1; /**< Config Receive Enabled */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CCFG_PCIE_CFG_STRUCT;

#define CCFG_PCIE_CFG_WIDTH 32
#define CCFG_PCIE_CFG_DEFAULT 0x0f000011U
#define CCFG_PCIE_CFG_ENDPOINT PCIE_CFG

#define CCFG_PCIE_CFG_UPRS_LSB 0x0000
#define CCFG_PCIE_CFG_UPRS_SIZE 0x0003
#define CCFG_PCIE_CFG_UNRS_LSB 0x0004
#define CCFG_PCIE_CFG_UNRS_SIZE 0x0003
#define CCFG_PCIE_CFG_MRPC_LSB 0x0008
#define CCFG_PCIE_CFG_MRPC_SIZE 0x0001
#define CCFG_PCIE_CFG_MRNPC_LSB 0x0009
#define CCFG_PCIE_CFG_MRNPC_SIZE 0x0001
#define CCFG_PCIE_CFG_UPSAAP_LSB 0x000a
#define CCFG_PCIE_CFG_UPSAAP_SIZE 0x0001
#define CCFG_PCIE_CFG_RBMS_LSB 0x000b
#define CCFG_PCIE_CFG_RBMS_SIZE 0x0001
#define CCFG_PCIE_CFG_UNRD_LSB 0x000c
#define CCFG_PCIE_CFG_UNRD_SIZE 0x0002
#define CCFG_PCIE_CFG_UNPSAAP_LSB 0x000e
#define CCFG_PCIE_CFG_UNPSAAP_SIZE 0x0001
#define CCFG_PCIE_CFG_DCGEISMA_LSB 0x000f
#define CCFG_PCIE_CFG_DCGEISMA_SIZE 0x0001
#define CCFG_PCIE_CFG_NPAP_LSB 0x0010
#define CCFG_PCIE_CFG_NPAP_SIZE 0x0001
#define CCFG_PCIE_CFG_NPAS_LSB 0x0011
#define CCFG_PCIE_CFG_NPAS_SIZE 0x0006
#define CCFG_PCIE_CFG_UNSD_LSB 0x0017
#define CCFG_PCIE_CFG_UNSD_SIZE 0x0001
#define CCFG_PCIE_CFG_UPSD_LSB 0x0018
#define CCFG_PCIE_CFG_UPSD_SIZE 0x0001
#define CCFG_PCIE_CFG_UPMWPD_LSB 0x0019
#define CCFG_PCIE_CFG_UPMWPD_SIZE 0x0001
#define CCFG_PCIE_CFG_UMRPD_LSB 0x001a
#define CCFG_PCIE_CFG_UMRPD_SIZE 0x0001
#define CCFG_PCIE_CFG_UARPD_LSB 0x001b
#define CCFG_PCIE_CFG_UARPD_SIZE 0x0001
#define CCFG_PCIE_CFG_UFDM_LSB 0x001c
#define CCFG_PCIE_CFG_UFDM_SIZE 0x0001
#define CCFG_PCIE_CFG_IORE_LSB 0x001e
#define CCFG_PCIE_CFG_IORE_SIZE 0x0001
#define CCFG_PCIE_CFG_CRE_LSB 0x001f
#define CCFG_PCIE_CFG_CRE_SIZE 0x0001



/** MPC2 desc:
  */

#define MPC2_PCIE_CFG_REG                (0x000000d4U)

typedef union {
  struct {
    UINT32 l1ctm : 1; /**< L1 Completion Timeout Mode */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 eoifd : 1; /**< EOI Forwarding Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 aspmco : 2; /**< ASPM Control Override */
                            /* Bits[3:2], default=0x00000000*/
    UINT32 aspmcoen : 1; /**< ASPM Control Override Enable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 ieime : 1; /**< Infer Electrical Idle Mechanism Enable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 lstp : 1; /**< Link Speed Training Policy */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[07:07] */
                            /* Bits[7:7], default=0*/
    UINT32 cam : 1; /**< Credit Allocated Update Mode */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 tlpf : 1; /**< Transaction Layer Packet Fast Transmit Mode */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 itcum : 1; /**< IOSF Transaction Credit Update Mode */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 ipf : 1; /**< IOSF Packet Fast Transmit Mode */
                            /* Bits[11:11], default=0x00000001*/
    UINT32 ptnfae : 1; /**< Poisoned TLP Non-Fatal Advisory Error Enable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 orcr : 1; /**< Offset Re-Calibration Request */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 orce : 2; /**< Offset Re-Calibration Enable */
                            /* Bits[15:14], default=0x00000000*/
    UINT32 displlewl1se : 1; /**< Disable PLL Early Wake on L1 Substate Exit */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 rxl0sebpro : 1; /**< RxL0s Entry to Bypass Phy Receive Off */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 macphyimpghpu : 1; /**< MAC PHY to Ignore modPHY Power Request Handshake during Powering Up */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 dnprl : 1; /**< Downstream Non-Posted Request Limit */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 gen3pllc : 1; /**< Reserved */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 gen2pllc : 1; /**< Reserved */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 mipipllc : 1; /**< Reserved */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 rud : 1; /**< Reserved */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 pllwait : 3; /**< PLL Wait */
                            /* Bits[26:24], default=0x00000000*/
    UINT32 rsvd_1 : 2; /**< Undefined - auto filled rsvd_[28:27] */
                            /* Bits[28:27], default=0*/
    UINT32 ebrwuodil0 : 1; /**< Elastic Buffer Reset When Underrun Or Overrun Detected In L0 */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 l1ssese : 1; /**< L1 Substate Exit SCI Enable */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 devinitupcfgplcy : 1; /**< Device Initiated Width Upconfiguration Policy */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MPC2_PCIE_CFG_STRUCT;

#define MPC2_PCIE_CFG_WIDTH 32
#define MPC2_PCIE_CFG_DEFAULT 0x00000800U
#define MPC2_PCIE_CFG_ENDPOINT PCIE_CFG

#define MPC2_PCIE_CFG_L1CTM_LSB 0x0000
#define MPC2_PCIE_CFG_L1CTM_SIZE 0x0001
#define MPC2_PCIE_CFG_EOIFD_LSB 0x0001
#define MPC2_PCIE_CFG_EOIFD_SIZE 0x0001
#define MPC2_PCIE_CFG_ASPMCO_LSB 0x0002
#define MPC2_PCIE_CFG_ASPMCO_SIZE 0x0002
#define MPC2_PCIE_CFG_ASPMCOEN_LSB 0x0004
#define MPC2_PCIE_CFG_ASPMCOEN_SIZE 0x0001
#define MPC2_PCIE_CFG_IEIME_LSB 0x0005
#define MPC2_PCIE_CFG_IEIME_SIZE 0x0001
#define MPC2_PCIE_CFG_LSTP_LSB 0x0006
#define MPC2_PCIE_CFG_LSTP_SIZE 0x0001
#define MPC2_PCIE_CFG_CAM_LSB 0x0008
#define MPC2_PCIE_CFG_CAM_SIZE 0x0001
#define MPC2_PCIE_CFG_TLPF_LSB 0x0009
#define MPC2_PCIE_CFG_TLPF_SIZE 0x0001
#define MPC2_PCIE_CFG_ITCUM_LSB 0x000a
#define MPC2_PCIE_CFG_ITCUM_SIZE 0x0001
#define MPC2_PCIE_CFG_IPF_LSB 0x000b
#define MPC2_PCIE_CFG_IPF_SIZE 0x0001
#define MPC2_PCIE_CFG_PTNFAE_LSB 0x000c
#define MPC2_PCIE_CFG_PTNFAE_SIZE 0x0001
#define MPC2_PCIE_CFG_ORCR_LSB 0x000d
#define MPC2_PCIE_CFG_ORCR_SIZE 0x0001
#define MPC2_PCIE_CFG_ORCE_LSB 0x000e
#define MPC2_PCIE_CFG_ORCE_SIZE 0x0002
#define MPC2_PCIE_CFG_DISPLLEWL1SE_LSB 0x0010
#define MPC2_PCIE_CFG_DISPLLEWL1SE_SIZE 0x0001
#define MPC2_PCIE_CFG_RXL0SEBPRO_LSB 0x0011
#define MPC2_PCIE_CFG_RXL0SEBPRO_SIZE 0x0001
#define MPC2_PCIE_CFG_MACPHYIMPGHPU_LSB 0x0012
#define MPC2_PCIE_CFG_MACPHYIMPGHPU_SIZE 0x0001
#define MPC2_PCIE_CFG_DNPRL_LSB 0x0013
#define MPC2_PCIE_CFG_DNPRL_SIZE 0x0001
#define MPC2_PCIE_CFG_GEN3PLLC_LSB 0x0014
#define MPC2_PCIE_CFG_GEN3PLLC_SIZE 0x0001
#define MPC2_PCIE_CFG_GEN2PLLC_LSB 0x0015
#define MPC2_PCIE_CFG_GEN2PLLC_SIZE 0x0001
#define MPC2_PCIE_CFG_MIPIPLLC_LSB 0x0016
#define MPC2_PCIE_CFG_MIPIPLLC_SIZE 0x0001
#define MPC2_PCIE_CFG_RUD_LSB 0x0017
#define MPC2_PCIE_CFG_RUD_SIZE 0x0001
#define MPC2_PCIE_CFG_PLLWAIT_LSB 0x0018
#define MPC2_PCIE_CFG_PLLWAIT_SIZE 0x0003
#define MPC2_PCIE_CFG_EBRWUODIL0_LSB 0x001d
#define MPC2_PCIE_CFG_EBRWUODIL0_SIZE 0x0001
#define MPC2_PCIE_CFG_L1SSESE_LSB 0x001e
#define MPC2_PCIE_CFG_L1SSESE_SIZE 0x0001
#define MPC2_PCIE_CFG_DEVINITUPCFGPLCY_LSB 0x001f
#define MPC2_PCIE_CFG_DEVINITUPCFGPLCY_SIZE 0x0001



/** MPC desc:
  */

#define MPC_PCIE_CFG_REG                 (0x000000d8U)

typedef union {
  struct {
    UINT32 pmme : 1; /**< Power Management SMI Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 hpme : 1; /**< Hot Plug SMI Enable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 bt : 1; /**< Bridge Type */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 mctpse : 1; /**< MCTP Support Enable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 fcp : 3; /**< Flow Control Update Policy */
                            /* Bits[6:4], default=0x00000000*/
    UINT32 pae : 1; /**< Port I/OxApic Enable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 at : 4; /**< Address Translater */
                            /* Bits[11:8], default=0x00000000*/
    UINT32 pciesd : 3; /**< PCIe Speed Disable */
                            /* Bits[14:12], default=0x00000000*/
    UINT32 ccel : 3; /**< Common Clock Exit Latency */
                            /* Bits[17:15], default=0x00000002*/
    UINT32 ucel : 3; /**< Unique Clock Exit Latency */
                            /* Bits[20:18], default=0x00000004*/
    UINT32 fcdl1e : 1; /**< Flow Control During L1 Entry */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[23:22] */
                            /* Bits[23:22], default=0*/
    UINT32 bmerce : 1; /**< BME Receive Check Enable */
                            /* Bits[24:24], default=0x00000001*/
    UINT32 irrce : 1; /**< Invalid Receive Range Check Enable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 p8xde : 1; /**< Port8xh Decode Enable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 mmbnce : 1; /**< MCTP Message Bus Number Check Enable */
                            /* Bits[27:27], default=0x00000001*/
    UINT32 ate : 1; /**< Address Translater Enable */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 lho : 1; /**< Link Hold Off */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 hpce : 1; /**< Hot Plug SCI Enable */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 pmce : 1; /**< Power Management SCI Enable */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MPC_PCIE_CFG_STRUCT;

#define MPC_PCIE_CFG_WIDTH 32
#define MPC_PCIE_CFG_DEFAULT 0x09110000U
#define MPC_PCIE_CFG_ENDPOINT PCIE_CFG

#define MPC_PCIE_CFG_PMME_LSB 0x0000
#define MPC_PCIE_CFG_PMME_SIZE 0x0001
#define MPC_PCIE_CFG_HPME_LSB 0x0001
#define MPC_PCIE_CFG_HPME_SIZE 0x0001
#define MPC_PCIE_CFG_BT_LSB 0x0002
#define MPC_PCIE_CFG_BT_SIZE 0x0001
#define MPC_PCIE_CFG_MCTPSE_LSB 0x0003
#define MPC_PCIE_CFG_MCTPSE_SIZE 0x0001
#define MPC_PCIE_CFG_FCP_LSB 0x0004
#define MPC_PCIE_CFG_FCP_SIZE 0x0003
#define MPC_PCIE_CFG_PAE_LSB 0x0007
#define MPC_PCIE_CFG_PAE_SIZE 0x0001
#define MPC_PCIE_CFG_AT_LSB 0x0008
#define MPC_PCIE_CFG_AT_SIZE 0x0004
#define MPC_PCIE_CFG_PCIESD_LSB 0x000c
#define MPC_PCIE_CFG_PCIESD_SIZE 0x0003
#define MPC_PCIE_CFG_CCEL_LSB 0x000f
#define MPC_PCIE_CFG_CCEL_SIZE 0x0003
#define MPC_PCIE_CFG_UCEL_LSB 0x0012
#define MPC_PCIE_CFG_UCEL_SIZE 0x0003
#define MPC_PCIE_CFG_FCDL1E_LSB 0x0015
#define MPC_PCIE_CFG_FCDL1E_SIZE 0x0001
#define MPC_PCIE_CFG_BMERCE_LSB 0x0018
#define MPC_PCIE_CFG_BMERCE_SIZE 0x0001
#define MPC_PCIE_CFG_IRRCE_LSB 0x0019
#define MPC_PCIE_CFG_IRRCE_SIZE 0x0001
#define MPC_PCIE_CFG_P8XDE_LSB 0x001a
#define MPC_PCIE_CFG_P8XDE_SIZE 0x0001
#define MPC_PCIE_CFG_MMBNCE_LSB 0x001b
#define MPC_PCIE_CFG_MMBNCE_SIZE 0x0001
#define MPC_PCIE_CFG_ATE_LSB 0x001c
#define MPC_PCIE_CFG_ATE_SIZE 0x0001
#define MPC_PCIE_CFG_LHO_LSB 0x001d
#define MPC_PCIE_CFG_LHO_SIZE 0x0001
#define MPC_PCIE_CFG_HPCE_LSB 0x001e
#define MPC_PCIE_CFG_HPCE_SIZE 0x0001
#define MPC_PCIE_CFG_PMCE_LSB 0x001f
#define MPC_PCIE_CFG_PMCE_SIZE 0x0001



/** SPR desc:
  */

#define SPR_PCIE_CFG_REG                 (0x000000e0U)

typedef union {
  struct {
    UINT8 scb : 4; /**< Sticky Chicken Bits */
                            /* Bits[3:0], default=0x00000000*/
    UINT8 bdfcp : 1; /**< Block DLLP Flag Clear Policy */
                            /* Bits[4:4], default=0x00000000*/
    UINT8 dlrfad : 1; /**< Down-Configured Lanes RXStandby Force Assertion Disable */
                            /* Bits[5:5], default=0x00000000*/
    UINT8 nscb : 2; /**< Non-Sticky Chicken Bits */
                            /* Bits[7:6], default=0x00000000*/
  }     Bits;
  UINT8 Data;
} SPR_PCIE_CFG_STRUCT;

#define SPR_PCIE_CFG_WIDTH 8
#define SPR_PCIE_CFG_DEFAULT 0x00U
#define SPR_PCIE_CFG_ENDPOINT PCIE_CFG

#define SPR_PCIE_CFG_SCB_LSB 0x0000
#define SPR_PCIE_CFG_SCB_SIZE 0x0004
#define SPR_PCIE_CFG_BDFCP_LSB 0x0004
#define SPR_PCIE_CFG_BDFCP_SIZE 0x0001
#define SPR_PCIE_CFG_DLRFAD_LSB 0x0005
#define SPR_PCIE_CFG_DLRFAD_SIZE 0x0001
#define SPR_PCIE_CFG_NSCB_LSB 0x0006
#define SPR_PCIE_CFG_NSCB_SIZE 0x0002



/** RPDCGEN desc:
  */

#define RPDCGEN_PCIE_CFG_REG             (0x000000e1U)

typedef union {
  struct {
    UINT8 rpdbcgen : 1; /**< Root Port Dynamic Backbone Clock Gate Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT8 rpdlcgen : 1; /**< Root Port Dynamic Link Clock Gate Enable */
                            /* Bits[1:1], default=0x00000000*/
    UINT8 srdbcgen : 1; /**< Shared Resource Dynamic Backbone Clock Gate Enable */
                            /* Bits[2:2], default=0x00000000*/
    UINT8 rsvd_rw : 1; /**< Reserved */
                            /* Bits[3:3], default=0x00000000*/
    UINT8 bbclkreqen : 1; /**< Backbone CLKREQ Enable */
                            /* Bits[4:4], default=0x00000000*/
    UINT8 lclkreqen : 1; /**< Link CLKREQ Enable */
                            /* Bits[5:5], default=0x00000000*/
    UINT8 ptocge : 1; /**< Partition/Trunk Oscillator Clock Gate Enable */
                            /* Bits[6:6], default=0x00000000*/
    UINT8 rsvd_m : 1; /**< Reserved */
                            /* Bits[7:7], default=0x00000000*/
  }     Bits;
  UINT8 Data;
} RPDCGEN_PCIE_CFG_STRUCT;

#define RPDCGEN_PCIE_CFG_WIDTH 8
#define RPDCGEN_PCIE_CFG_DEFAULT 0x00U
#define RPDCGEN_PCIE_CFG_ENDPOINT PCIE_CFG

#define RPDCGEN_PCIE_CFG_RPDBCGEN_LSB 0x0000
#define RPDCGEN_PCIE_CFG_RPDBCGEN_SIZE 0x0001
#define RPDCGEN_PCIE_CFG_RPDLCGEN_LSB 0x0001
#define RPDCGEN_PCIE_CFG_RPDLCGEN_SIZE 0x0001
#define RPDCGEN_PCIE_CFG_SRDBCGEN_LSB 0x0002
#define RPDCGEN_PCIE_CFG_SRDBCGEN_SIZE 0x0001
#define RPDCGEN_PCIE_CFG_RSVD_RW_LSB 0x0003
#define RPDCGEN_PCIE_CFG_RSVD_RW_SIZE 0x0001
#define RPDCGEN_PCIE_CFG_BBCLKREQEN_LSB 0x0004
#define RPDCGEN_PCIE_CFG_BBCLKREQEN_SIZE 0x0001
#define RPDCGEN_PCIE_CFG_LCLKREQEN_LSB 0x0005
#define RPDCGEN_PCIE_CFG_LCLKREQEN_SIZE 0x0001
#define RPDCGEN_PCIE_CFG_PTOCGE_LSB 0x0006
#define RPDCGEN_PCIE_CFG_PTOCGE_SIZE 0x0001
#define RPDCGEN_PCIE_CFG_RSVD_M_LSB 0x0007
#define RPDCGEN_PCIE_CFG_RSVD_M_SIZE 0x0001



/** RPPGEN desc:
  */

#define RPPGEN_PCIE_CFG_REG              (0x000000e2U)

typedef union {
  struct {
    UINT8 mdlswpr : 1; /**< mod-PHY Data Lane Suspend Well Power Request */
                            /* Bits[0:0], default=0x00000001*/
    UINT8 mdlswpgea : 1; /**< mod-PHY Data Lane Suspend Well Power Gate Entry Ack */
                            /* Bits[1:1], default=0x00000000*/
    UINT8 l23er : 1; /**< L23_Rdy Entry Request */
                            /* Bits[2:2], default=0x00000000*/
    UINT8 l23r2dt : 1; /**< L23_Rdy to Detect Transition */
                            /* Bits[3:3], default=0x00000000*/
    UINT8 seoscge : 1; /**< Sideband Endpoint Oscillator/Side Clock Gating Enable */
                            /* Bits[4:4], default=0x00000000*/
    UINT8 mdlswpgexa : 1; /**< mod-PHY Data Lane Suspend Well Power Gating Exit Ack */
                            /* Bits[5:5], default=0x00000000*/
    UINT8 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[06:06] */
                            /* Bits[6:6], default=0*/
    UINT8 mcwpgs : 1; /**< mod-PHY Core Well Power Gating Status */
                            /* Bits[7:7], default=0x00000000*/
  }     Bits;
  UINT8 Data;
} RPPGEN_PCIE_CFG_STRUCT;

#define RPPGEN_PCIE_CFG_WIDTH 8
#define RPPGEN_PCIE_CFG_DEFAULT 0x01U
#define RPPGEN_PCIE_CFG_ENDPOINT PCIE_CFG

#define RPPGEN_PCIE_CFG_MDLSWPR_LSB 0x0000
#define RPPGEN_PCIE_CFG_MDLSWPR_SIZE 0x0001
#define RPPGEN_PCIE_CFG_MDLSWPGEA_LSB 0x0001
#define RPPGEN_PCIE_CFG_MDLSWPGEA_SIZE 0x0001
#define RPPGEN_PCIE_CFG_L23ER_LSB 0x0002
#define RPPGEN_PCIE_CFG_L23ER_SIZE 0x0001
#define RPPGEN_PCIE_CFG_L23R2DT_LSB 0x0003
#define RPPGEN_PCIE_CFG_L23R2DT_SIZE 0x0001
#define RPPGEN_PCIE_CFG_SEOSCGE_LSB 0x0004
#define RPPGEN_PCIE_CFG_SEOSCGE_SIZE 0x0001
#define RPPGEN_PCIE_CFG_MDLSWPGEXA_LSB 0x0005
#define RPPGEN_PCIE_CFG_MDLSWPGEXA_SIZE 0x0001
#define RPPGEN_PCIE_CFG_MCWPGS_LSB 0x0007
#define RPPGEN_PCIE_CFG_MCWPGS_SIZE 0x0001



/** PWRCTL desc:
  */

#define PWRCTL_PCIE_CFG_REG              (0x000000e8U)

typedef union {
  struct {
    UINT32 rpdtsqpol : 1; /**< Root Port Detect Squelch Polling */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rpl1sqpol : 1; /**< Root Port L1 Squelch Polling */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 rpsewl : 2; /**< Root Port Squelch Exit Wait Latency */
                            /* Bits[3:2], default=0x00000001*/
    UINT32 ltrr : 1; /**< LTR Received */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 isct : 1; /**< Invalid Speed Change Transition */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 pcsceiost : 1; /**< Pollling Compliance Speed Change EIOS Transmission */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 sctem : 1; /**< Synthesized Completion Termination Encoding Masking */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 rptfsmwi : 1; /**< Reset Progress Tracker FSM When Idle */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 selaq : 1; /**< Squelch Exit Lane Active Qualification */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 hrbf : 1; /**< Hot Removal Bug Fix */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 atodedtt : 1; /**< Assert TxElecIdle On Detect Entry Due To TimeOut */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 efsostoetr : 1; /**< Enable Forced SKP Ordered Set Transmission On Entry To Recovery */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 txswing : 1; /**< Analog PHY Transmitter Voltage Swing */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 ilsvrx : 1; /**< Invalid LTR Latency Scale Value Received */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 dbupi : 1; /**< De-skew Buffer Unload Pointer Increment */
                            /* Bits[15:15], default=0x00000001*/
    UINT32 dlp : 1; /**< Down-configured Lanes Policy */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 wpdmpgep : 1; /**< Wake PLL On Detect mod-PHY Power Gating Exit Policy */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 g3snb : 1; /**< GEN3 Support Noted Behavior */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 lwued : 1; /**< Link Width Upconfiguration Evaluation Delay */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 ltssmrtc : 1; /**< LTSSM Received TSx Count */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 bishb : 1; /**< BlockAlign Invalid Sync Header Behavior */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 ishcf : 1; /**< Invalid Sync Header Check Filter */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 lifecf : 1; /**< Logical Idle Framing Error Check Filter */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 ecf128b130b : 1; /**< 128b/130b Error Check Filter */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[25:25] */
                            /* Bits[25:25], default=0*/
    UINT32 l1ssvnnre : 1; /**< L1 Substate VNN Removal Enable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[27:27] */
                            /* Bits[27:27], default=0*/
    UINT32 darece : 1; /**< Delayed Ack for Recovery Entry */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 rxeispl1s : 1; /**< RxEiDetectDisable Suppression in L1.x States */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 txcmdspl1s : 1; /**< TxCommonModeDisable Suppression in L1.x States */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 rsvd_m : 1; /**< Reserved */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PWRCTL_PCIE_CFG_STRUCT;

#define PWRCTL_PCIE_CFG_WIDTH 32
#define PWRCTL_PCIE_CFG_DEFAULT 0x00008004U
#define PWRCTL_PCIE_CFG_ENDPOINT PCIE_CFG

#define PWRCTL_PCIE_CFG_RPDTSQPOL_LSB 0x0000
#define PWRCTL_PCIE_CFG_RPDTSQPOL_SIZE 0x0001
#define PWRCTL_PCIE_CFG_RPL1SQPOL_LSB 0x0001
#define PWRCTL_PCIE_CFG_RPL1SQPOL_SIZE 0x0001
#define PWRCTL_PCIE_CFG_RPSEWL_LSB 0x0002
#define PWRCTL_PCIE_CFG_RPSEWL_SIZE 0x0002
#define PWRCTL_PCIE_CFG_LTRR_LSB 0x0004
#define PWRCTL_PCIE_CFG_LTRR_SIZE 0x0001
#define PWRCTL_PCIE_CFG_ISCT_LSB 0x0005
#define PWRCTL_PCIE_CFG_ISCT_SIZE 0x0001
#define PWRCTL_PCIE_CFG_PCSCEIOST_LSB 0x0006
#define PWRCTL_PCIE_CFG_PCSCEIOST_SIZE 0x0001
#define PWRCTL_PCIE_CFG_SCTEM_LSB 0x0007
#define PWRCTL_PCIE_CFG_SCTEM_SIZE 0x0001
#define PWRCTL_PCIE_CFG_RPTFSMWI_LSB 0x0008
#define PWRCTL_PCIE_CFG_RPTFSMWI_SIZE 0x0001
#define PWRCTL_PCIE_CFG_SELAQ_LSB 0x0009
#define PWRCTL_PCIE_CFG_SELAQ_SIZE 0x0001
#define PWRCTL_PCIE_CFG_HRBF_LSB 0x000a
#define PWRCTL_PCIE_CFG_HRBF_SIZE 0x0001
#define PWRCTL_PCIE_CFG_ATODEDTT_LSB 0x000b
#define PWRCTL_PCIE_CFG_ATODEDTT_SIZE 0x0001
#define PWRCTL_PCIE_CFG_EFSOSTOETR_LSB 0x000c
#define PWRCTL_PCIE_CFG_EFSOSTOETR_SIZE 0x0001
#define PWRCTL_PCIE_CFG_TXSWING_LSB 0x000d
#define PWRCTL_PCIE_CFG_TXSWING_SIZE 0x0001
#define PWRCTL_PCIE_CFG_ILSVRX_LSB 0x000e
#define PWRCTL_PCIE_CFG_ILSVRX_SIZE 0x0001
#define PWRCTL_PCIE_CFG_DBUPI_LSB 0x000f
#define PWRCTL_PCIE_CFG_DBUPI_SIZE 0x0001
#define PWRCTL_PCIE_CFG_DLP_LSB 0x0010
#define PWRCTL_PCIE_CFG_DLP_SIZE 0x0001
#define PWRCTL_PCIE_CFG_WPDMPGEP_LSB 0x0011
#define PWRCTL_PCIE_CFG_WPDMPGEP_SIZE 0x0001
#define PWRCTL_PCIE_CFG_G3SNB_LSB 0x0012
#define PWRCTL_PCIE_CFG_G3SNB_SIZE 0x0001
#define PWRCTL_PCIE_CFG_LWUED_LSB 0x0013
#define PWRCTL_PCIE_CFG_LWUED_SIZE 0x0001
#define PWRCTL_PCIE_CFG_LTSSMRTC_LSB 0x0014
#define PWRCTL_PCIE_CFG_LTSSMRTC_SIZE 0x0001
#define PWRCTL_PCIE_CFG_BISHB_LSB 0x0015
#define PWRCTL_PCIE_CFG_BISHB_SIZE 0x0001
#define PWRCTL_PCIE_CFG_ISHCF_LSB 0x0016
#define PWRCTL_PCIE_CFG_ISHCF_SIZE 0x0001
#define PWRCTL_PCIE_CFG_LIFECF_LSB 0x0017
#define PWRCTL_PCIE_CFG_LIFECF_SIZE 0x0001
#define PWRCTL_PCIE_CFG_ECF128B130B_LSB 0x0018
#define PWRCTL_PCIE_CFG_ECF128B130B_SIZE 0x0001
#define PWRCTL_PCIE_CFG_L1SSVNNRE_LSB 0x001a
#define PWRCTL_PCIE_CFG_L1SSVNNRE_SIZE 0x0001
#define PWRCTL_PCIE_CFG_DARECE_LSB 0x001c
#define PWRCTL_PCIE_CFG_DARECE_SIZE 0x0001
#define PWRCTL_PCIE_CFG_RXEISPL1S_LSB 0x001d
#define PWRCTL_PCIE_CFG_RXEISPL1S_SIZE 0x0001
#define PWRCTL_PCIE_CFG_TXCMDSPL1S_LSB 0x001e
#define PWRCTL_PCIE_CFG_TXCMDSPL1S_SIZE 0x0001
#define PWRCTL_PCIE_CFG_RSVD_M_LSB 0x001f
#define PWRCTL_PCIE_CFG_RSVD_M_SIZE 0x0001



/** DC desc:
  */

#define DC_PCIE_CFG_REG                  (0x000000ecU)

typedef union {
  struct {
    UINT32 vc0nsaov : 1; /**< VC0 No-Snoop Attribute Override Value */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 vc0nsao : 1; /**< VC0 No-Snoop Attribute Override */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 pcibem : 1; /**< PCI Bus Emulation Mode */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 ecd : 1; /**< Extended Config Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 rcrbnrce : 1; /**< RCRB MMIO Range Claim Enable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 rsvd_0 : 8; /**< Undefined - auto filled rsvd_[12:05] */
                            /* Bits[12:5], default=0*/
    UINT32 com : 1; /**< Completion Ordering Mode */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 dct0c : 1; /**< Downstream Config Type 0 Conversion */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 dct1c : 1; /**< Downstream Config Type 1 Conversion */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 ioapicrb : 8; /**< IOxAPIC Range Base */
                            /* Bits[23:16], default=0x00000000*/
    UINT32 ioapicrl : 8; /**< IOxAPIC Range Limit */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DC_PCIE_CFG_STRUCT;

#define DC_PCIE_CFG_WIDTH 32
#define DC_PCIE_CFG_DEFAULT 0x00000000U
#define DC_PCIE_CFG_ENDPOINT PCIE_CFG

#define DC_PCIE_CFG_VC0NSAOV_LSB 0x0000
#define DC_PCIE_CFG_VC0NSAOV_SIZE 0x0001
#define DC_PCIE_CFG_VC0NSAO_LSB 0x0001
#define DC_PCIE_CFG_VC0NSAO_SIZE 0x0001
#define DC_PCIE_CFG_PCIBEM_LSB 0x0002
#define DC_PCIE_CFG_PCIBEM_SIZE 0x0001
#define DC_PCIE_CFG_ECD_LSB 0x0003
#define DC_PCIE_CFG_ECD_SIZE 0x0001
#define DC_PCIE_CFG_RCRBNRCE_LSB 0x0004
#define DC_PCIE_CFG_RCRBNRCE_SIZE 0x0001
#define DC_PCIE_CFG_COM_LSB 0x000d
#define DC_PCIE_CFG_COM_SIZE 0x0001
#define DC_PCIE_CFG_DCT0C_LSB 0x000e
#define DC_PCIE_CFG_DCT0C_SIZE 0x0001
#define DC_PCIE_CFG_DCT1C_LSB 0x000f
#define DC_PCIE_CFG_DCT1C_SIZE 0x0001
#define DC_PCIE_CFG_IOAPICRB_LSB 0x0010
#define DC_PCIE_CFG_IOAPICRB_SIZE 0x0008
#define DC_PCIE_CFG_IOAPICRL_LSB 0x0018
#define DC_PCIE_CFG_IOAPICRL_SIZE 0x0008



/** IPCS desc:
  */

#define IPCS_PCIE_CFG_REG                (0x000000f0U)

typedef union {
  struct {
    UINT32 urre : 1; /**< Unsupported Request Reporting Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 urd : 1; /**< Unsupported Request Detected */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[03:02] */
                            /* Bits[3:2], default=0*/
    UINT32 imrs : 3; /**< IOSF Max Read Request Size */
                            /* Bits[6:4], default=0x00000007*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[07:07] */
                            /* Bits[7:7], default=0*/
    UINT32 imps : 3; /**< IOSF_Max_Payload_Size */
                            /* Bits[10:8], default=0x00000001*/
    UINT32 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[11:11] */
                            /* Bits[11:11], default=0*/
    UINT32 pric : 3; /**< IOSF Primary ISM Idle Counter */
                            /* Bits[14:12], default=0x00000000*/
    UINT32 rsvd_3 : 1; /**< Undefined - auto filled rsvd_[15:15] */
                            /* Bits[15:15], default=0*/
    UINT32 pppcibsn : 4; /**< PCIe Port PCI Bus Segment Number */
                            /* Bits[19:16], default=0x00000000*/
    UINT32 hbpcibsn : 4; /**< Host-Bridge PCI Bus Segment Number */
                            /* Bits[23:20], default=0x00000000*/
    UINT32 rsvd_m : 8; /**< Reserved */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} IPCS_PCIE_CFG_STRUCT;

#define IPCS_PCIE_CFG_WIDTH 32
#define IPCS_PCIE_CFG_DEFAULT 0x00000170U
#define IPCS_PCIE_CFG_ENDPOINT PCIE_CFG

#define IPCS_PCIE_CFG_URRE_LSB 0x0000
#define IPCS_PCIE_CFG_URRE_SIZE 0x0001
#define IPCS_PCIE_CFG_URD_LSB 0x0001
#define IPCS_PCIE_CFG_URD_SIZE 0x0001
#define IPCS_PCIE_CFG_IMRS_LSB 0x0004
#define IPCS_PCIE_CFG_IMRS_SIZE 0x0003
#define IPCS_PCIE_CFG_IMPS_LSB 0x0008
#define IPCS_PCIE_CFG_IMPS_SIZE 0x0003
#define IPCS_PCIE_CFG_PRIC_LSB 0x000c
#define IPCS_PCIE_CFG_PRIC_SIZE 0x0003
#define IPCS_PCIE_CFG_PPPCIBSN_LSB 0x0010
#define IPCS_PCIE_CFG_PPPCIBSN_SIZE 0x0004
#define IPCS_PCIE_CFG_HBPCIBSN_LSB 0x0014
#define IPCS_PCIE_CFG_HBPCIBSN_SIZE 0x0004
#define IPCS_PCIE_CFG_RSVD_M_LSB 0x0018
#define IPCS_PCIE_CFG_RSVD_M_SIZE 0x0008



/** PHYCTL2 desc:
  */

#define PHYCTL2_PCIE_CFG_REG             (0x000000f5U)

typedef union {
  struct {
    UINT8 pxpg2plloffen : 1; /**< PCI Express GEN2 PLL Off Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT8 pxpg3plloffen : 1; /**< PCI Express GEN3 PLL Off Enable */
                            /* Bits[1:1], default=0x00000000*/
    UINT8 blkpllen : 1; /**< Block PLL Request */
                            /* Bits[2:2], default=0x00000001*/
    UINT8 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[03:03] */
                            /* Bits[3:3], default=0*/
    UINT8 txcfgchgwait : 2; /**< Transmit Configuration Change Wait Time */
                            /* Bits[5:4], default=0x00000003*/
    UINT8 tdft : 2; /**< Transmit Datapath Flush Timer */
                            /* Bits[7:6], default=0x00000003*/
  }     Bits;
  UINT8 Data;
} PHYCTL2_PCIE_CFG_STRUCT;

#define PHYCTL2_PCIE_CFG_WIDTH 8
#define PHYCTL2_PCIE_CFG_DEFAULT 0xf4U
#define PHYCTL2_PCIE_CFG_ENDPOINT PCIE_CFG

#define PHYCTL2_PCIE_CFG_PXPG2PLLOFFEN_LSB 0x0000
#define PHYCTL2_PCIE_CFG_PXPG2PLLOFFEN_SIZE 0x0001
#define PHYCTL2_PCIE_CFG_PXPG3PLLOFFEN_LSB 0x0001
#define PHYCTL2_PCIE_CFG_PXPG3PLLOFFEN_SIZE 0x0001
#define PHYCTL2_PCIE_CFG_BLKPLLEN_LSB 0x0002
#define PHYCTL2_PCIE_CFG_BLKPLLEN_SIZE 0x0001
#define PHYCTL2_PCIE_CFG_TXCFGCHGWAIT_LSB 0x0004
#define PHYCTL2_PCIE_CFG_TXCFGCHGWAIT_SIZE 0x0002
#define PHYCTL2_PCIE_CFG_TDFT_LSB 0x0006
#define PHYCTL2_PCIE_CFG_TDFT_SIZE 0x0002



/** PHYCTL3 desc:
  */

#define PHYCTL3_PCIE_CFG_REG             (0x000000f6U)

typedef union {
  struct {
    UINT8 txhsdeempctl : 1; /**< TX Half Swing Deemphasis Control */
                            /* Bits[0:0], default=0x00000000*/
    UINT8 sqdirctrl : 1; /**< Squelch Direction */
                            /* Bits[1:1], default=0x00000000*/
    UINT8 sqdirovren : 1; /**< Squelch Direction Override Enable */
                            /* Bits[2:2], default=0x00000000*/
    UINT8 rsvd_m : 5; /**< Reserved */
                            /* Bits[7:3], default=0x00000000*/
  }     Bits;
  UINT8 Data;
} PHYCTL3_PCIE_CFG_STRUCT;

#define PHYCTL3_PCIE_CFG_WIDTH 8
#define PHYCTL3_PCIE_CFG_DEFAULT 0x00U
#define PHYCTL3_PCIE_CFG_ENDPOINT PCIE_CFG

#define PHYCTL3_PCIE_CFG_TXHSDEEMPCTL_LSB 0x0000
#define PHYCTL3_PCIE_CFG_TXHSDEEMPCTL_SIZE 0x0001
#define PHYCTL3_PCIE_CFG_SQDIRCTRL_LSB 0x0001
#define PHYCTL3_PCIE_CFG_SQDIRCTRL_SIZE 0x0001
#define PHYCTL3_PCIE_CFG_SQDIROVREN_LSB 0x0002
#define PHYCTL3_PCIE_CFG_SQDIROVREN_SIZE 0x0001
#define PHYCTL3_PCIE_CFG_RSVD_M_LSB 0x0003
#define PHYCTL3_PCIE_CFG_RSVD_M_SIZE 0x0005



/** IOSFSBCS desc:
  */

#define IOSFSBCS_PCIE_CFG_REG            (0x000000f7U)

typedef union {
  struct {
    UINT8 sbic : 2; /**< IOSF Sideband ISM Idle Counter */
                            /* Bits[1:0], default=0x00000000*/
    UINT8 siid : 2; /**< IOSF Sideband Interface Idle Counter */
                            /* Bits[3:2], default=0x00000001*/
    UINT8 isurd : 1; /**< IOSF Sideband Unsupported Request Detected */
                            /* Bits[4:4], default=0x00000000*/
    UINT8 isfsided : 1; /**< IOSF Sideband Fuse/Strap/ID Distribution Error Detected */
                            /* Bits[5:5], default=0x00000000*/
    UINT8 scptcge : 1; /**< Side Clock Partition/Trunk Clock Gating Enable */
                            /* Bits[6:6], default=0x00000000*/
    UINT8 rsvd_m : 1; /**< Reserved */
                            /* Bits[7:7], default=0x00000000*/
  }     Bits;
  UINT8 Data;
} IOSFSBCS_PCIE_CFG_STRUCT;

#define IOSFSBCS_PCIE_CFG_WIDTH 8
#define IOSFSBCS_PCIE_CFG_DEFAULT 0x04U
#define IOSFSBCS_PCIE_CFG_ENDPOINT PCIE_CFG

#define IOSFSBCS_PCIE_CFG_SBIC_LSB 0x0000
#define IOSFSBCS_PCIE_CFG_SBIC_SIZE 0x0002
#define IOSFSBCS_PCIE_CFG_SIID_LSB 0x0002
#define IOSFSBCS_PCIE_CFG_SIID_SIZE 0x0002
#define IOSFSBCS_PCIE_CFG_ISURD_LSB 0x0004
#define IOSFSBCS_PCIE_CFG_ISURD_SIZE 0x0001
#define IOSFSBCS_PCIE_CFG_ISFSIDED_LSB 0x0005
#define IOSFSBCS_PCIE_CFG_ISFSIDED_SIZE 0x0001
#define IOSFSBCS_PCIE_CFG_SCPTCGE_LSB 0x0006
#define IOSFSBCS_PCIE_CFG_SCPTCGE_SIZE 0x0001
#define IOSFSBCS_PCIE_CFG_RSVD_M_LSB 0x0007
#define IOSFSBCS_PCIE_CFG_RSVD_M_SIZE 0x0001



/** STRPFUSECFG desc:
  */

#define STRPFUSECFG_PCIE_CFG_REG         (0x000000fcU)

typedef union {
  struct {
    UINT32 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[01:00] */
                            /* Bits[1:0], default=0*/
    UINT32 desktopmob : 1; /**< Desktop or Mobile Fuse */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 cdcgdis : 1; /**< Core Dynamic Clock Gating Disable Fuse */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 ltcgdis : 1; /**< Link Trunk Clock Gating Disable Fuse */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 ldcgdis : 1; /**< Link Dynamic Clock Gating Disable Fuse */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 aspmdis : 1; /**< ASPM Disable Fuse */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 rsvd_rof : 1; /**< Reserved */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 pllshtdwndis : 1; /**< PLL Shut Down Disable Fuse */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 mphyiopmdis : 1; /**< mPHY I/O PM Disable Fuse */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 nal : 2; /**< Number of Active Lane */
                            /* Bits[11:10], default=0x00000000*/
    UINT32 nalpg : 1; /**< Number of Active Lane with Power Gating enable */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 lr : 1; /**< Lane Reversal */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 rpc : 3; /**< Root Port Configuration Strap */
                            /* Bits[16:14], default=0x00000000*/
    UINT32 xoscf : 2; /**< Crystal Oscillator Frequency */
                            /* Bits[18:17], default=0x00000000*/
    UINT32 ftm : 2; /**< Frequency Throttling Mode */
                            /* Bits[20:19], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[21:21] */
                            /* Bits[21:21], default=0*/
    UINT32 mphypgd : 1; /**< mod-PHY Power Gating Disable Fuse */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[23:23] */
                            /* Bits[23:23], default=0*/
    UINT32 pxip : 4; /**< PCI Express Interrupt Pin */
                            /* Bits[27:24], default=0x00000000*/
    UINT32 drpc : 1; /**< Dynamic Root Port Configuration Supported */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 serm : 1; /**< Server Error Reporting Mode */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 blkdqda_rstvalue : 1; /**< BLKDQDA Register Default value */
                            /* Bits[30:30], default=0x00000001*/
    UINT32 blkpllen_rstvalue : 1; /**< BLKPLLEN Register Default value */
                            /* Bits[31:31], default=0x00000001*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} STRPFUSECFG_PCIE_CFG_STRUCT;

#define STRPFUSECFG_PCIE_CFG_WIDTH 32
#define STRPFUSECFG_PCIE_CFG_DEFAULT 0xc0000000U
#define STRPFUSECFG_PCIE_CFG_ENDPOINT PCIE_CFG

#define STRPFUSECFG_PCIE_CFG_DESKTOPMOB_LSB 0x0002
#define STRPFUSECFG_PCIE_CFG_DESKTOPMOB_SIZE 0x0001
#define STRPFUSECFG_PCIE_CFG_CDCGDIS_LSB 0x0003
#define STRPFUSECFG_PCIE_CFG_CDCGDIS_SIZE 0x0001
#define STRPFUSECFG_PCIE_CFG_LTCGDIS_LSB 0x0004
#define STRPFUSECFG_PCIE_CFG_LTCGDIS_SIZE 0x0001
#define STRPFUSECFG_PCIE_CFG_LDCGDIS_LSB 0x0005
#define STRPFUSECFG_PCIE_CFG_LDCGDIS_SIZE 0x0001
#define STRPFUSECFG_PCIE_CFG_ASPMDIS_LSB 0x0006
#define STRPFUSECFG_PCIE_CFG_ASPMDIS_SIZE 0x0001
#define STRPFUSECFG_PCIE_CFG_RSVD_ROF_LSB 0x0007
#define STRPFUSECFG_PCIE_CFG_RSVD_ROF_SIZE 0x0001
#define STRPFUSECFG_PCIE_CFG_PLLSHTDWNDIS_LSB 0x0008
#define STRPFUSECFG_PCIE_CFG_PLLSHTDWNDIS_SIZE 0x0001
#define STRPFUSECFG_PCIE_CFG_MPHYIOPMDIS_LSB 0x0009
#define STRPFUSECFG_PCIE_CFG_MPHYIOPMDIS_SIZE 0x0001
#define STRPFUSECFG_PCIE_CFG_NAL_LSB 0x000a
#define STRPFUSECFG_PCIE_CFG_NAL_SIZE 0x0002
#define STRPFUSECFG_PCIE_CFG_NALPG_LSB 0x000c
#define STRPFUSECFG_PCIE_CFG_NALPG_SIZE 0x0001
#define STRPFUSECFG_PCIE_CFG_LR_LSB 0x000d
#define STRPFUSECFG_PCIE_CFG_LR_SIZE 0x0001
#define STRPFUSECFG_PCIE_CFG_RPC_LSB 0x000e
#define STRPFUSECFG_PCIE_CFG_RPC_SIZE 0x0003
#define STRPFUSECFG_PCIE_CFG_XOSCF_LSB 0x0011
#define STRPFUSECFG_PCIE_CFG_XOSCF_SIZE 0x0002
#define STRPFUSECFG_PCIE_CFG_FTM_LSB 0x0013
#define STRPFUSECFG_PCIE_CFG_FTM_SIZE 0x0002
#define STRPFUSECFG_PCIE_CFG_MPHYPGD_LSB 0x0016
#define STRPFUSECFG_PCIE_CFG_MPHYPGD_SIZE 0x0001
#define STRPFUSECFG_PCIE_CFG_PXIP_LSB 0x0018
#define STRPFUSECFG_PCIE_CFG_PXIP_SIZE 0x0004
#define STRPFUSECFG_PCIE_CFG_DRPC_LSB 0x001c
#define STRPFUSECFG_PCIE_CFG_DRPC_SIZE 0x0001
#define STRPFUSECFG_PCIE_CFG_SERM_LSB 0x001d
#define STRPFUSECFG_PCIE_CFG_SERM_SIZE 0x0001
#define STRPFUSECFG_PCIE_CFG_BLKDQDA_RSTVALUE_LSB 0x001e
#define STRPFUSECFG_PCIE_CFG_BLKDQDA_RSTVALUE_SIZE 0x0001
#define STRPFUSECFG_PCIE_CFG_BLKPLLEN_RSTVALUE_LSB 0x001f
#define STRPFUSECFG_PCIE_CFG_BLKPLLEN_RSTVALUE_SIZE 0x0001


/** AECH desc:
  */

#define AECH_PCIE_CFG_REG                (0x00000100U)

typedef union {
  struct {
    UINT32 cid : 16; /**< Capability ID */
                            /* Bits[15:0], default=0x00000000*/
    UINT32 cv : 4; /**< Capability Version */
                            /* Bits[19:16], default=0x00000000*/
    UINT32 nco : 12; /**< Next Capability Offset */
                            /* Bits[31:20], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} AECH_PCIE_CFG_STRUCT;

#define AECH_PCIE_CFG_WIDTH 32
#define AECH_PCIE_CFG_DEFAULT 0x00000000U
#define AECH_PCIE_CFG_ENDPOINT PCIE_CFG

#define AECH_PCIE_CFG_CID_LSB 0x0000
#define AECH_PCIE_CFG_CID_SIZE 0x0010
#define AECH_PCIE_CFG_CV_LSB 0x0010
#define AECH_PCIE_CFG_CV_SIZE 0x0004
#define AECH_PCIE_CFG_NCO_LSB 0x0014
#define AECH_PCIE_CFG_NCO_SIZE 0x000c

/** UEM desc:
  */

#define UEM_PCIE_CFG_REG                 (0x00000108U)

typedef union {
  struct {
    UINT32 te : 1; /**< Training Error Mask */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 3; /**< Undefined - auto filled rsvd_[03:01] */
                            /* Bits[3:1], default=0*/
    UINT32 dlpe : 1; /**< Data Link Protocol Error Mask */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 sde : 1; /**< Surprise Down Error Mask */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 rsvd_1 : 6; /**< Undefined - auto filled rsvd_[11:06] */
                            /* Bits[11:6], default=0*/
    UINT32 pt : 1; /**< Poisoned TLP Mask */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 fcpe : 1; /**< Flow Control Protocol Error Mask */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 ct : 1; /**< Completion Timeout Mask */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 cm : 1; /**< Completer Abort Mask */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 uc : 1; /**< Unexpected Completion Mask */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 ro : 1; /**< Receiver Overflow Mask */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 mt : 1; /**< Malformed TLP Mask */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[19:19] */
                            /* Bits[19:19], default=0*/
    UINT32 ure : 1; /**< Unsupported Request Error Mask */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 avm : 1; /**< ACS Violation Mask */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 uiem : 1; /**< Uncorrectable Internal Error Mask */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 mcbtlpm : 1; /**< MC Blocked TLP Mask */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 aebm : 1; /**< AtomicOp Egress Blocked Mask */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 tlppbem : 1; /**< TLP Prefix Blocked Error Mask */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 ptlpebm : 1; /**< Poisoned TLP Egress Blocked Mask */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 rsvd_3 : 5; /**< Undefined - auto filled rsvd_[31:27] */
                            /* Bits[31:27], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} UEM_PCIE_CFG_STRUCT;

#define UEM_PCIE_CFG_WIDTH 32
#define UEM_PCIE_CFG_DEFAULT 0x00000000U
#define UEM_PCIE_CFG_ENDPOINT PCIE_CFG

#define UEM_PCIE_CFG_TE_LSB 0x0000
#define UEM_PCIE_CFG_TE_SIZE 0x0001
#define UEM_PCIE_CFG_DLPE_LSB 0x0004
#define UEM_PCIE_CFG_DLPE_SIZE 0x0001
#define UEM_PCIE_CFG_SDE_LSB 0x0005
#define UEM_PCIE_CFG_SDE_SIZE 0x0001
#define UEM_PCIE_CFG_PT_LSB 0x000c
#define UEM_PCIE_CFG_PT_SIZE 0x0001
#define UEM_PCIE_CFG_FCPE_LSB 0x000d
#define UEM_PCIE_CFG_FCPE_SIZE 0x0001
#define UEM_PCIE_CFG_CT_LSB 0x000e
#define UEM_PCIE_CFG_CT_SIZE 0x0001
#define UEM_PCIE_CFG_CM_LSB 0x000f
#define UEM_PCIE_CFG_CM_SIZE 0x0001
#define UEM_PCIE_CFG_UC_LSB 0x0010
#define UEM_PCIE_CFG_UC_SIZE 0x0001
#define UEM_PCIE_CFG_RO_LSB 0x0011
#define UEM_PCIE_CFG_RO_SIZE 0x0001
#define UEM_PCIE_CFG_MT_LSB 0x0012
#define UEM_PCIE_CFG_MT_SIZE 0x0001
#define UEM_PCIE_CFG_URE_LSB 0x0014
#define UEM_PCIE_CFG_URE_SIZE 0x0001
#define UEM_PCIE_CFG_AVM_LSB 0x0015
#define UEM_PCIE_CFG_AVM_SIZE 0x0001
#define UEM_PCIE_CFG_UIEM_LSB 0x0016
#define UEM_PCIE_CFG_UIEM_SIZE 0x0001
#define UEM_PCIE_CFG_MCBTLPM_LSB 0x0017
#define UEM_PCIE_CFG_MCBTLPM_SIZE 0x0001
#define UEM_PCIE_CFG_AEBM_LSB 0x0018
#define UEM_PCIE_CFG_AEBM_SIZE 0x0001
#define UEM_PCIE_CFG_TLPPBEM_LSB 0x0019
#define UEM_PCIE_CFG_TLPPBEM_SIZE 0x0001
#define UEM_PCIE_CFG_PTLPEBM_LSB 0x001a
#define UEM_PCIE_CFG_PTLPEBM_SIZE 0x0001

/** CEM_PCIE_CFG desc:
  * Register default value:        0x0000E000
  * Register full path in IP: PXPC/sippcie2x4_configreg_top_inst0/sippcie2x4_inst0_pcie_cfg_port_0_/sippcie2x4_cfg_pri_sippcie2x4_cfg_sb/CEM
  * Security PolicyGroup: SIPPCIE2X4_IOSFPRI_PLY0_1
  * This is the Correctable Error Mask registers. Refer description for each individual field below for more details of the register functionality.
  */

#define CEM_PCIE_CFG_REG                 (0x00000114U)

typedef union {
  struct {
    UINT32 re : 1; /**< Receiver Error Mask */
    UINT32 rsvd_0 : 5; /**< Undefined - auto filled rsvd_[05:01] */
    UINT32 bt : 1; /**< Bad TLP Mask */
    UINT32 bd : 1; /**< Bad DLLP Mask */
    UINT32 rnr : 1; /**< Replay Number Rollover Mask */
    UINT32 rsvd_1 : 3; /**< Undefined - auto filled rsvd_[11:09] */
    UINT32 rtt : 1; /**< Replay Timer Timeout Mask */
    UINT32 anfem : 1; /**< Advisory Non-Fatal Error Mask */
    UINT32 ciem : 1; /**< Corrected Internal Error Mask */
    UINT32 hlom : 1; /**< Header Log Overflow Mask */
    UINT32 rsvd_m : 16; /**< Reserved */
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CEM_PCIE_CFG_STRUCT;

/** PTMECH desc:
  */

#define PTMECH_PCIE_CFG_REG              (0x00000150U)

typedef union {
  struct {
    UINT32 cid : 16; /**< Capability ID */
                            /* Bits[15:0], default=0x00000000*/
    UINT32 cv : 4; /**< Capability Version */
                            /* Bits[19:16], default=0x00000000*/
    UINT32 nco : 12; /**< Next Capability Offset */
                            /* Bits[31:20], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PTMECH_PCIE_CFG_STRUCT;

#define PTMECH_PCIE_CFG_WIDTH 32
#define PTMECH_PCIE_CFG_DEFAULT 0x00000000U
#define PTMECH_PCIE_CFG_ENDPOINT PCIE_CFG

#define PTMECH_PCIE_CFG_CID_LSB 0x0000
#define PTMECH_PCIE_CFG_CID_SIZE 0x0010
#define PTMECH_PCIE_CFG_CV_LSB 0x0010
#define PTMECH_PCIE_CFG_CV_SIZE 0x0004
#define PTMECH_PCIE_CFG_NCO_LSB 0x0014
#define PTMECH_PCIE_CFG_NCO_SIZE 0x000c

/** PTMCAPR desc:
  */

#define PTMCAPR_PCIE_CFG_REG             (0x00000154U)

typedef union {
  struct {
    UINT32 ptmreqc : 1; /**< PTM Requester Capable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 ptmrspc : 1; /**< PTM Responder Capable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 ptmrc : 1; /**< PTM Root Capable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[03:03] */
                            /* Bits[3:3], default=0*/
    UINT32 ptmpdac : 1; /**< PTM Propagation Delay Adaptation Capable */
                            /* Bits[4:4], default=0x00000001*/
    UINT32 rsvd_1 : 3; /**< Undefined - auto filled rsvd_[07:05] */
                            /* Bits[7:5], default=0*/
    UINT32 lcg : 8; /**< Local Clock Granularity */
                            /* Bits[15:8], default=0x00000004*/
    UINT32 rsvd_m : 16; /**< Reserved */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PTMCAPR_PCIE_CFG_STRUCT;

#define PTMCAPR_PCIE_CFG_WIDTH 32
#define PTMCAPR_PCIE_CFG_DEFAULT 0x00000410U
#define PTMCAPR_PCIE_CFG_ENDPOINT PCIE_CFG

#define PTMCAPR_PCIE_CFG_PTMREQC_LSB 0x0000
#define PTMCAPR_PCIE_CFG_PTMREQC_SIZE 0x0001
#define PTMCAPR_PCIE_CFG_PTMRSPC_LSB 0x0001
#define PTMCAPR_PCIE_CFG_PTMRSPC_SIZE 0x0001
#define PTMCAPR_PCIE_CFG_PTMRC_LSB 0x0002
#define PTMCAPR_PCIE_CFG_PTMRC_SIZE 0x0001
#define PTMCAPR_PCIE_CFG_PTMPDAC_LSB 0x0004
#define PTMCAPR_PCIE_CFG_PTMPDAC_SIZE 0x0001
#define PTMCAPR_PCIE_CFG_LCG_LSB 0x0008
#define PTMCAPR_PCIE_CFG_LCG_SIZE 0x0008
#define PTMCAPR_PCIE_CFG_RSVD_M_LSB 0x0010
#define PTMCAPR_PCIE_CFG_RSVD_M_SIZE 0x0010

/** PTMCTLR desc:
  */

#define PTMCTLR_PCIE_CFG_REG             (0x00000158U)

typedef union {
  struct {
    UINT32 ptme : 1; /**< PTM Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rs : 1; /**< Root Select */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 rsvd_0 : 6; /**< Undefined - auto filled rsvd_[07:02] */
                            /* Bits[7:2], default=0*/
    UINT32 eg : 8; /**< Effective Granularity */
                            /* Bits[15:8], default=0x00000000*/
    UINT32 rsvd_m : 16; /**< Reserved */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PTMCTLR_PCIE_CFG_STRUCT;

#define PTMCTLR_PCIE_CFG_WIDTH 32
#define PTMCTLR_PCIE_CFG_DEFAULT 0x00000000U
#define PTMCTLR_PCIE_CFG_ENDPOINT PCIE_CFG

#define PTMCTLR_PCIE_CFG_PTME_LSB 0x0000
#define PTMCTLR_PCIE_CFG_PTME_SIZE 0x0001
#define PTMCTLR_PCIE_CFG_RS_LSB 0x0001
#define PTMCTLR_PCIE_CFG_RS_SIZE 0x0001
#define PTMCTLR_PCIE_CFG_EG_LSB 0x0008
#define PTMCTLR_PCIE_CFG_EG_SIZE 0x0008
#define PTMCTLR_PCIE_CFG_RSVD_M_LSB 0x0010
#define PTMCTLR_PCIE_CFG_RSVD_M_SIZE 0x0010

/** L1SECH desc:
  */

#define L1SECH_PCIE_CFG_REG              (0x00000200U)

typedef union {
  struct {
    UINT32 pcieec : 16; /**< PCI Express Extended Capability ID */
                            /* Bits[15:0], default=0x00000000*/
    UINT32 cv : 4; /**< Capability Version */
                            /* Bits[19:16], default=0x00000000*/
    UINT32 nco : 12; /**< Next Capability Offset */
                            /* Bits[31:20], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} L1SECH_PCIE_CFG_STRUCT;

#define L1SECH_PCIE_CFG_WIDTH 32
#define L1SECH_PCIE_CFG_DEFAULT 0x00000000U
#define L1SECH_PCIE_CFG_ENDPOINT PCIE_CFG

#define L1SECH_PCIE_CFG_PCIEEC_LSB 0x0000
#define L1SECH_PCIE_CFG_PCIEEC_SIZE 0x0010
#define L1SECH_PCIE_CFG_CV_LSB 0x0010
#define L1SECH_PCIE_CFG_CV_SIZE 0x0004
#define L1SECH_PCIE_CFG_NCO_LSB 0x0014
#define L1SECH_PCIE_CFG_NCO_SIZE 0x000c

/** L1SCAP desc:
  */

#define L1SCAP_PCIE_CFG_REG              (0x00000204U)

typedef union {
  struct {
    UINT32 ppl12s : 1; /**< PCI-PM L1.2 Supported */
                            /* Bits[0:0], default=0x00000001*/
    UINT32 ppl11s : 1; /**< PCI-PM L1.1 Supported */
                            /* Bits[1:1], default=0x00000001*/
    UINT32 al12s : 1; /**< ASPM L1.2 Supported */
                            /* Bits[2:2], default=0x00000001*/
    UINT32 al11s : 1; /**< ASPM L1.1 Supported */
                            /* Bits[3:3], default=0x00000001*/
    UINT32 l1pss : 1; /**< L1 PM Substates Supported */
                            /* Bits[4:4], default=0x00000001*/
    UINT32 l1sses : 1; /**< CLKREQ Acceleration Supported */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[07:06] */
                            /* Bits[7:6], default=0*/
    UINT32 pcmrt : 8; /**< Port Common Mode Restore Time */
                            /* Bits[15:8], default=0x00000028*/
    UINT32 ptpos : 2; /**< Port Tpower_on Scale */
                            /* Bits[17:16], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[18:18] */
                            /* Bits[18:18], default=0*/
    UINT32 ptv : 5; /**< Port Tpower_on Value */
                            /* Bits[23:19], default=0x00000005*/
    UINT32 rsvd_m : 8; /**< Reserved */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} L1SCAP_PCIE_CFG_STRUCT;

#define L1SCAP_PCIE_CFG_WIDTH 32
#define L1SCAP_PCIE_CFG_DEFAULT 0x0028281fU
#define L1SCAP_PCIE_CFG_ENDPOINT PCIE_CFG

#define L1SCAP_PCIE_CFG_PPL12S_LSB 0x0000
#define L1SCAP_PCIE_CFG_PPL12S_SIZE 0x0001
#define L1SCAP_PCIE_CFG_PPL11S_LSB 0x0001
#define L1SCAP_PCIE_CFG_PPL11S_SIZE 0x0001
#define L1SCAP_PCIE_CFG_AL12S_LSB 0x0002
#define L1SCAP_PCIE_CFG_AL12S_SIZE 0x0001
#define L1SCAP_PCIE_CFG_AL11S_LSB 0x0003
#define L1SCAP_PCIE_CFG_AL11S_SIZE 0x0001
#define L1SCAP_PCIE_CFG_L1PSS_LSB 0x0004
#define L1SCAP_PCIE_CFG_L1PSS_SIZE 0x0001
#define L1SCAP_PCIE_CFG_L1SSES_LSB 0x0005
#define L1SCAP_PCIE_CFG_L1SSES_SIZE 0x0001
#define L1SCAP_PCIE_CFG_PCMRT_LSB 0x0008
#define L1SCAP_PCIE_CFG_PCMRT_SIZE 0x0008
#define L1SCAP_PCIE_CFG_PTPOS_LSB 0x0010
#define L1SCAP_PCIE_CFG_PTPOS_SIZE 0x0002
#define L1SCAP_PCIE_CFG_PTV_LSB 0x0013
#define L1SCAP_PCIE_CFG_PTV_SIZE 0x0005
#define L1SCAP_PCIE_CFG_RSVD_M_LSB 0x0018
#define L1SCAP_PCIE_CFG_RSVD_M_SIZE 0x0008

/** L1SCTL1 desc:
  */

#define L1SCTL1_PCIE_CFG_REG             (0x00000208U)

typedef union {
  struct {
    UINT32 ppl12e : 1; /**< PCI-PM L1.2 Enabled */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 ppl11e : 1; /**< PCI-PM L1.1 Enable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 al12e : 1; /**< ASPM L1.2 Enable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 al11e : 1; /**< ASPM L1.1 Enable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 l1sseie : 1; /**< CLKREQ Acceleration Interrupt Enable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 l1ssec : 1; /**< L1 Substate Exit Control */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[07:06] */
                            /* Bits[7:6], default=0*/
    UINT32 cmrt : 8; /**< Common Mode Restore Time */
                            /* Bits[15:8], default=0x00000000*/
    UINT32 l12ltrtlv : 10; /**< L1.2 LTR Threshold Latency Value */
                            /* Bits[25:16], default=0x00000000*/
    UINT32 rsvd_1 : 3; /**< Undefined - auto filled rsvd_[28:26] */
                            /* Bits[28:26], default=0*/
    UINT32 l12ltrtlsv : 3; /**< L1.2 LTR Threshold Latency Scale Value */
                            /* Bits[31:29], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} L1SCTL1_PCIE_CFG_STRUCT;

#define L1SCTL1_PCIE_CFG_WIDTH 32
#define L1SCTL1_PCIE_CFG_DEFAULT 0x00000000U
#define L1SCTL1_PCIE_CFG_ENDPOINT PCIE_CFG

#define L1SCTL1_PCIE_CFG_PPL12E_LSB 0x0000
#define L1SCTL1_PCIE_CFG_PPL12E_SIZE 0x0001
#define L1SCTL1_PCIE_CFG_PPL11E_LSB 0x0001
#define L1SCTL1_PCIE_CFG_PPL11E_SIZE 0x0001
#define L1SCTL1_PCIE_CFG_AL12E_LSB 0x0002
#define L1SCTL1_PCIE_CFG_AL12E_SIZE 0x0001
#define L1SCTL1_PCIE_CFG_AL11E_LSB 0x0003
#define L1SCTL1_PCIE_CFG_AL11E_SIZE 0x0001
#define L1SCTL1_PCIE_CFG_L1SSEIE_LSB 0x0004
#define L1SCTL1_PCIE_CFG_L1SSEIE_SIZE 0x0001
#define L1SCTL1_PCIE_CFG_L1SSEC_LSB 0x0005
#define L1SCTL1_PCIE_CFG_L1SSEC_SIZE 0x0001
#define L1SCTL1_PCIE_CFG_CMRT_LSB 0x0008
#define L1SCTL1_PCIE_CFG_CMRT_SIZE 0x0008
#define L1SCTL1_PCIE_CFG_L12LTRTLV_LSB 0x0010
#define L1SCTL1_PCIE_CFG_L12LTRTLV_SIZE 0x000a
#define L1SCTL1_PCIE_CFG_L12LTRTLSV_LSB 0x001d
#define L1SCTL1_PCIE_CFG_L12LTRTLSV_SIZE 0x0003

/** L1SCTL2 desc:
  */

#define L1SCTL2_PCIE_CFG_REG             (0x0000020cU)

typedef union {
  struct {
    UINT32 tpos : 2; /**< Tpower_on Scale */
                            /* Bits[1:0], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[02:02] */
                            /* Bits[2:2], default=0*/
    UINT32 powt : 5; /**< Power On Wait Time */
                            /* Bits[7:3], default=0x00000005*/
    UINT32 rsvd_m : 24; /**< Reserved */
                            /* Bits[31:8], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} L1SCTL2_PCIE_CFG_STRUCT;

#define L1SCTL2_PCIE_CFG_WIDTH 32
#define L1SCTL2_PCIE_CFG_DEFAULT 0x00000028U
#define L1SCTL2_PCIE_CFG_ENDPOINT PCIE_CFG

#define L1SCTL2_PCIE_CFG_TPOS_LSB 0x0000
#define L1SCTL2_PCIE_CFG_TPOS_SIZE 0x0002
#define L1SCTL2_PCIE_CFG_POWT_LSB 0x0003
#define L1SCTL2_PCIE_CFG_POWT_SIZE 0x0005
#define L1SCTL2_PCIE_CFG_RSVD_M_LSB 0x0008
#define L1SCTL2_PCIE_CFG_RSVD_M_SIZE 0x0018

/** ACSECH desc:
  */

#define ACSECH_PCIE_CFG_REG              (0x00000220U)

typedef union {
  struct {
    UINT32 cid : 16; /**< Capability ID */
                            /* Bits[15:0], default=0x00000000*/
    UINT32 cv : 4; /**< Capability Version */
                            /* Bits[19:16], default=0x00000000*/
    UINT32 nco : 12; /**< Next Capability Offset */
                            /* Bits[31:20], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} ACSECH_PCIE_CFG_STRUCT;

#define ACSECH_PCIE_CFG_WIDTH 32
#define ACSECH_PCIE_CFG_DEFAULT 0x00000000U
#define ACSECH_PCIE_CFG_ENDPOINT PCIE_CFG

#define ACSECH_PCIE_CFG_CID_LSB 0x0000
#define ACSECH_PCIE_CFG_CID_SIZE 0x0010
#define ACSECH_PCIE_CFG_CV_LSB 0x0010
#define ACSECH_PCIE_CFG_CV_SIZE 0x0004
#define ACSECH_PCIE_CFG_NCO_LSB 0x0014
#define ACSECH_PCIE_CFG_NCO_SIZE 0x000c

/** ACSCAPR desc:
  */

#define ACSCAPR_PCIE_CFG_REG             (0x00000224U)

typedef union {
  struct {
    UINT16 v : 1; /**< ACS Source Validation */
                            /* Bits[0:0], default=0x00000001*/
    UINT16 b : 1; /**< ACS Translation Blocking */
                            /* Bits[1:1], default=0x00000001*/
    UINT16 r : 1; /**< ACS P2P Request Redirect */
                            /* Bits[2:2], default=0x00000001*/
    UINT16 c : 1; /**< ACS P2P Completion Redirect */
                            /* Bits[3:3], default=0x00000001*/
    UINT16 u : 1; /**< ACS Upstream Forwarding */
                            /* Bits[4:4], default=0x00000001*/
    UINT16 e : 1; /**< ACS P2P Egress Control */
                            /* Bits[5:5], default=0x00000000*/
    UINT16 t : 1; /**< ACS Direct Translated P2P */
                            /* Bits[6:6], default=0x00000000*/
    UINT16 rsvd_m : 9; /**< Reserved */
                            /* Bits[15:7], default=0x00000000*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} ACSCAPR_PCIE_CFG_STRUCT;

#define ACSCAPR_PCIE_CFG_WIDTH 16
#define ACSCAPR_PCIE_CFG_DEFAULT 0x001fU
#define ACSCAPR_PCIE_CFG_ENDPOINT PCIE_CFG

#define ACSCAPR_PCIE_CFG_V_LSB 0x0000
#define ACSCAPR_PCIE_CFG_V_SIZE 0x0001
#define ACSCAPR_PCIE_CFG_B_LSB 0x0001
#define ACSCAPR_PCIE_CFG_B_SIZE 0x0001
#define ACSCAPR_PCIE_CFG_R_LSB 0x0002
#define ACSCAPR_PCIE_CFG_R_SIZE 0x0001
#define ACSCAPR_PCIE_CFG_C_LSB 0x0003
#define ACSCAPR_PCIE_CFG_C_SIZE 0x0001
#define ACSCAPR_PCIE_CFG_U_LSB 0x0004
#define ACSCAPR_PCIE_CFG_U_SIZE 0x0001
#define ACSCAPR_PCIE_CFG_E_LSB 0x0005
#define ACSCAPR_PCIE_CFG_E_SIZE 0x0001
#define ACSCAPR_PCIE_CFG_T_LSB 0x0006
#define ACSCAPR_PCIE_CFG_T_SIZE 0x0001
#define ACSCAPR_PCIE_CFG_RSVD_M_LSB 0x0007
#define ACSCAPR_PCIE_CFG_RSVD_M_SIZE 0x0009

/** VCCH desc:
  */

#define VCCH_PCIE_CFG_REG                (0x00000280U)

typedef union {
  struct {
    UINT32 cid : 16; /**< PCI Express Extended Capability ID */
                            /* Bits[15:0], default=0x00000000*/
    UINT32 cv : 4; /**< Capability Version */
                            /* Bits[19:16], default=0x00000000*/
    UINT32 nco : 12; /**< Next Capability Offset */
                            /* Bits[31:20], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} VCCH_PCIE_CFG_STRUCT;

#define VCCH_PCIE_CFG_WIDTH 32
#define VCCH_PCIE_CFG_DEFAULT 0x00000000U
#define VCCH_PCIE_CFG_ENDPOINT PCIE_CFG

#define VCCH_PCIE_CFG_CID_LSB 0x0000
#define VCCH_PCIE_CFG_CID_SIZE 0x0010
#define VCCH_PCIE_CFG_CV_LSB 0x0010
#define VCCH_PCIE_CFG_CV_SIZE 0x0004
#define VCCH_PCIE_CFG_NCO_LSB 0x0014
#define VCCH_PCIE_CFG_NCO_SIZE 0x000c


/** PVCCR1 desc:
  */

#define PVCCR1_PCIE_CFG_REG              (0x00000284U)

typedef union {
  struct {
    UINT32 evcc : 3; /**< Extended VC Count */
                            /* Bits[2:0], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[03:03] */
                            /* Bits[3:3], default=0*/
    UINT32 lpevcc : 3; /**< Low Priority Extended VC Count */
                            /* Bits[6:4], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[07:07] */
                            /* Bits[7:7], default=0*/
    UINT32 rc : 2; /**< Reference Clock */
                            /* Bits[9:8], default=0x00000000*/
    UINT32 fares : 2; /**< Function Arbitration Table Entry Size */
                            /* Bits[11:10], default=0x00000000*/
    UINT32 rsvd_m : 20; /**< Reserved */
                            /* Bits[31:12], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PVCCR1_PCIE_CFG_STRUCT;

#define PVCCR1_PCIE_CFG_WIDTH 32
#define PVCCR1_PCIE_CFG_DEFAULT 0x00000000U
#define PVCCR1_PCIE_CFG_ENDPOINT PCIE_CFG

#define PVCCR1_PCIE_CFG_EVCC_LSB 0x0000
#define PVCCR1_PCIE_CFG_EVCC_SIZE 0x0003
#define PVCCR1_PCIE_CFG_LPEVCC_LSB 0x0004
#define PVCCR1_PCIE_CFG_LPEVCC_SIZE 0x0003
#define PVCCR1_PCIE_CFG_RC_LSB 0x0008
#define PVCCR1_PCIE_CFG_RC_SIZE 0x0002
#define PVCCR1_PCIE_CFG_FARES_LSB 0x000a
#define PVCCR1_PCIE_CFG_FARES_SIZE 0x0002
#define PVCCR1_PCIE_CFG_RSVD_M_LSB 0x000c
#define PVCCR1_PCIE_CFG_RSVD_M_SIZE 0x0014

/** V0VCRC desc:
  */

#define V0VCRC_PCIE_CFG_REG              (0x00000290U)

typedef union {
  struct {
    UINT32 fac : 8; /**< Function Arbitration Capability */
                            /* Bits[7:0], default=0x00000000*/
    UINT32 rsvd_0 : 8; /**< Undefined - auto filled rsvd_[15:08] */
                            /* Bits[15:8], default=0*/
    UINT32 mts : 7; /**< Maximum Time Slots */
                            /* Bits[22:16], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[23:23] */
                            /* Bits[23:23], default=0*/
    UINT32 fato : 8; /**< Function Arbitration Table Offset */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} V0VCRC_PCIE_CFG_STRUCT;

#define V0VCRC_PCIE_CFG_WIDTH 32
#define V0VCRC_PCIE_CFG_DEFAULT 0x00000000U
#define V0VCRC_PCIE_CFG_ENDPOINT PCIE_CFG

#define V0VCRC_PCIE_CFG_FAC_LSB 0x0000
#define V0VCRC_PCIE_CFG_FAC_SIZE 0x0008
#define V0VCRC_PCIE_CFG_MTS_LSB 0x0010
#define V0VCRC_PCIE_CFG_MTS_SIZE 0x0007
#define V0VCRC_PCIE_CFG_FATO_LSB 0x0018
#define V0VCRC_PCIE_CFG_FATO_SIZE 0x0008

/** V0CTL desc:
  */

#define V0CTL_PCIE_CFG_REG               (0x00000294U)

typedef union {
  struct {
    UINT32 tvmt0 : 1; /**< Transaction Class / Virtual Channel Map TC0 */
                            /* Bits[0:0], default=0x00000001*/
    UINT32 tvm : 7; /**< Transaction Class / Virtual Channel Map */
                            /* Bits[7:1], default=0x00000000*/
    UINT32 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[09:08] */
                            /* Bits[9:8], default=0*/
    UINT32 etvm : 6; /**< Extended TC/VC Map */
                            /* Bits[15:10], default=0x00000000*/
    UINT32 lfat : 1; /**< Load Function Arbitration Table */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 fas : 3; /**< Function Arbitration Select */
                            /* Bits[19:17], default=0x00000000*/
    UINT32 rsvd_1 : 4; /**< Undefined - auto filled rsvd_[23:20] */
                            /* Bits[23:20], default=0*/
    UINT32 id : 3; /**< Virtual Channel Identifier */
                            /* Bits[26:24], default=0x00000000*/
    UINT32 rsvd_2 : 4; /**< Undefined - auto filled rsvd_[30:27] */
                            /* Bits[30:27], default=0*/
    UINT32 en : 1; /**< Virtual Channel Enable */
                            /* Bits[31:31], default=0x00000001*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} V0CTL_PCIE_CFG_STRUCT;

#define V0CTL_PCIE_CFG_WIDTH 32
#define V0CTL_PCIE_CFG_DEFAULT 0x80000001U
#define V0CTL_PCIE_CFG_ENDPOINT PCIE_CFG

#define V0CTL_PCIE_CFG_TVMT0_LSB 0x0000
#define V0CTL_PCIE_CFG_TVMT0_SIZE 0x0001
#define V0CTL_PCIE_CFG_TVM_LSB 0x0001
#define V0CTL_PCIE_CFG_TVM_SIZE 0x0007
#define V0CTL_PCIE_CFG_ETVM_LSB 0x000a
#define V0CTL_PCIE_CFG_ETVM_SIZE 0x0006
#define V0CTL_PCIE_CFG_LFAT_LSB 0x0010
#define V0CTL_PCIE_CFG_LFAT_SIZE 0x0001
#define V0CTL_PCIE_CFG_FAS_LSB 0x0011
#define V0CTL_PCIE_CFG_FAS_SIZE 0x0003
#define V0CTL_PCIE_CFG_ID_LSB 0x0018
#define V0CTL_PCIE_CFG_ID_SIZE 0x0003
#define V0CTL_PCIE_CFG_EN_LSB 0x001f
#define V0CTL_PCIE_CFG_EN_SIZE 0x0001

/** V1VCRC desc:
  */

#define V1VCRC_PCIE_CFG_REG              (0x0000029cU)

typedef union {
  struct {
    UINT32 fac : 8; /**< Function Arbitration Capability */
                            /* Bits[7:0], default=0x00000000*/
    UINT32 rsvd_0 : 8; /**< Undefined - auto filled rsvd_[15:08] */
                            /* Bits[15:8], default=0*/
    UINT32 mts : 7; /**< Maximum Time Slots */
                            /* Bits[22:16], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[23:23] */
                            /* Bits[23:23], default=0*/
    UINT32 fato : 8; /**< Function Arbitration Table Offset */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} V1VCRC_PCIE_CFG_STRUCT;

#define V1VCRC_PCIE_CFG_WIDTH 32
#define V1VCRC_PCIE_CFG_DEFAULT 0x00000000U
#define V1VCRC_PCIE_CFG_ENDPOINT PCIE_CFG

#define V1VCRC_PCIE_CFG_FAC_LSB 0x0000
#define V1VCRC_PCIE_CFG_FAC_SIZE 0x0008
#define V1VCRC_PCIE_CFG_MTS_LSB 0x0010
#define V1VCRC_PCIE_CFG_MTS_SIZE 0x0007
#define V1VCRC_PCIE_CFG_FATO_LSB 0x0018
#define V1VCRC_PCIE_CFG_FATO_SIZE 0x0008

/** V1CTL desc:
  */

#define V1CTL_PCIE_CFG_REG               (0x000002a0U)

typedef union {
  struct {
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[00:00] */
                            /* Bits[0:0], default=0*/
    UINT32 tvm : 7; /**< Transaction Class / Virtual Channel Map */
                            /* Bits[7:1], default=0x00000000*/
    UINT32 rsvd_1 : 2; /**< Undefined - auto filled rsvd_[09:08] */
                            /* Bits[9:8], default=0*/
    UINT32 etvm : 6; /**< Extended TC/VC Map */
                            /* Bits[15:10], default=0x00000000*/
    UINT32 lfat : 1; /**< Load Function Arbitration Table */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 fas : 3; /**< Function Arbitration Select */
                            /* Bits[19:17], default=0x00000000*/
    UINT32 rsvd_2 : 4; /**< Undefined - auto filled rsvd_[23:20] */
                            /* Bits[23:20], default=0*/
    UINT32 id : 4; /**< Virtual Channel Identifier */
                            /* Bits[27:24], default=0x00000000*/
    UINT32 rsvd_3 : 3; /**< Undefined - auto filled rsvd_[30:28] */
                            /* Bits[30:28], default=0*/
    UINT32 en : 1; /**< Virtual Channel Enable */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} V1CTL_PCIE_CFG_STRUCT;

#define V1CTL_PCIE_CFG_WIDTH 32
#define V1CTL_PCIE_CFG_DEFAULT 0x00000000U
#define V1CTL_PCIE_CFG_ENDPOINT PCIE_CFG

#define V1CTL_PCIE_CFG_TVM_LSB 0x0001
#define V1CTL_PCIE_CFG_TVM_SIZE 0x0007
#define V1CTL_PCIE_CFG_ETVM_LSB 0x000a
#define V1CTL_PCIE_CFG_ETVM_SIZE 0x0006
#define V1CTL_PCIE_CFG_LFAT_LSB 0x0010
#define V1CTL_PCIE_CFG_LFAT_SIZE 0x0001
#define V1CTL_PCIE_CFG_FAS_LSB 0x0011
#define V1CTL_PCIE_CFG_FAS_SIZE 0x0003
#define V1CTL_PCIE_CFG_ID_LSB 0x0018
#define V1CTL_PCIE_CFG_ID_SIZE 0x0004
#define V1CTL_PCIE_CFG_EN_LSB 0x001f
#define V1CTL_PCIE_CFG_EN_SIZE 0x0001

/** PCIERTP1 desc:
  */

#define PCIERTP1_PCIE_CFG_REG            (0x00000300U)

typedef union {
  struct {
    UINT8 g1x4 : 4; /**< Gen 1 x4 */
                            /* Bits[3:0], default=0x00000006*/
    UINT8 rsvd_0 : 4; /**< Undefined - auto filled rsvd_[07:04] */
                            /* Bits[7:4], default=0*/
  }     Bits;
  UINT8 Data;
} PCIERTP1_PCIE_CFG_STRUCT;

#define PCIERTP1_PCIE_CFG_WIDTH 8
#define PCIERTP1_PCIE_CFG_DEFAULT 0x06U
#define PCIERTP1_PCIE_CFG_ENDPOINT PCIE_CFG

#define PCIERTP1_PCIE_CFG_G1X4_LSB 0x0000
#define PCIERTP1_PCIE_CFG_G1X4_SIZE 0x0004



/** G4L0SCTL desc:
  */

#define G4L0SCTL_PCIE_CFG_REG            (0x00000310U)

typedef union {
  struct {
    UINT32 g4ccnfts : 8; /**< Gen4 Common Clock N_FTS */
                            /* Bits[7:0], default=0x0000001E*/
    UINT32 g4ucnfts : 8; /**< Gen4 Unique Clock N_FTS */
                            /* Bits[15:8], default=0x00000028*/
    UINT32 rsvd_0 : 6; /**< Undefined - auto filled rsvd_[21:16] */
                            /* Bits[21:16], default=0*/
    UINT32 g4l0sic : 2; /**< Gen4 L0s Entry Idle Control */
                            /* Bits[23:22], default=0x00000000*/
    UINT32 g4asl0spl : 8; /**< Gen4Active State L0s Preparation Latency */
                            /* Bits[31:24], default=0x0000000C*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} G4L0SCTL_PCIE_CFG_STRUCT;

#define G4L0SCTL_PCIE_CFG_WIDTH 32
#define G4L0SCTL_PCIE_CFG_DEFAULT 0x0c00281eU
#define G4L0SCTL_PCIE_CFG_ENDPOINT PCIE_CFG

#define G4L0SCTL_PCIE_CFG_G4CCNFTS_LSB 0x0000
#define G4L0SCTL_PCIE_CFG_G4CCNFTS_SIZE 0x0008
#define G4L0SCTL_PCIE_CFG_G4UCNFTS_LSB 0x0008
#define G4L0SCTL_PCIE_CFG_G4UCNFTS_SIZE 0x0008
#define G4L0SCTL_PCIE_CFG_G4L0SIC_LSB 0x0016
#define G4L0SCTL_PCIE_CFG_G4L0SIC_SIZE 0x0002
#define G4L0SCTL_PCIE_CFG_G4ASL0SPL_LSB 0x0018
#define G4L0SCTL_PCIE_CFG_G4ASL0SPL_SIZE 0x0008



/** PCIENFTS desc:
  */

#define PCIENFTS_PCIE_CFG_REG            (0x00000314U)

typedef union {
  struct {
    UINT32 g1ccnfts : 8; /**< Gen 1 Common Clock N_FTS */
                            /* Bits[7:0], default=0x00000013*/
    UINT32 g1ucnfts : 8; /**< Gen1 Unique Clock N_FTS */
                            /* Bits[15:8], default=0x0000002A*/
    UINT32 g2ccnfts : 8; /**< Gen2 Common Clock N_FTS */
                            /* Bits[23:16], default=0x00000026*/
    UINT32 g2ucnfts : 8; /**< Gen2 Unique Clock N_FTS */
                            /* Bits[31:24], default=0x00000054*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PCIENFTS_PCIE_CFG_STRUCT;

#define PCIENFTS_PCIE_CFG_WIDTH 32
#define PCIENFTS_PCIE_CFG_DEFAULT 0x54262a13U
#define PCIENFTS_PCIE_CFG_ENDPOINT PCIE_CFG

#define PCIENFTS_PCIE_CFG_G1CCNFTS_LSB 0x0000
#define PCIENFTS_PCIE_CFG_G1CCNFTS_SIZE 0x0008
#define PCIENFTS_PCIE_CFG_G1UCNFTS_LSB 0x0008
#define PCIENFTS_PCIE_CFG_G1UCNFTS_SIZE 0x0008
#define PCIENFTS_PCIE_CFG_G2CCNFTS_LSB 0x0010
#define PCIENFTS_PCIE_CFG_G2CCNFTS_SIZE 0x0008
#define PCIENFTS_PCIE_CFG_G2UCNFTS_LSB 0x0018
#define PCIENFTS_PCIE_CFG_G2UCNFTS_SIZE 0x0008



/** PCIEL0SC desc:
  */

#define PCIEL0SC_PCIE_CFG_REG            (0x00000318U)

typedef union {
  struct {
    UINT32 g1l0sic : 2; /**< Gen1 L0s Entry Idle Control */
                            /* Bits[1:0], default=0x00000000*/
    UINT32 g2l0sic : 2; /**< Gen2 L0s Entry Idle Control */
                            /* Bits[3:2], default=0x00000000*/
    UINT32 txl0srxexit : 2; /**< Tx L0s Rx Exit Control */
                            /* Bits[5:4], default=0x00000000*/
    UINT32 txl0srxentry : 1; /**< Tx L0s Rx Entry */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 anftsen : 1; /**< Adaptive N_FTS Enable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 anftso : 8; /**< Adaptive N_FTS Offset */
                            /* Bits[15:8], default=0x00000000*/
    UINT32 g1asl0spl : 8; /**< Gen1 Active State L0s Preparation Latency */
                            /* Bits[23:16], default=0x0000000C*/
    UINT32 g2asl0spl : 8; /**< Gen2 Active State L0s Preparation Latency */
                            /* Bits[31:24], default=0x0000000C*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PCIEL0SC_PCIE_CFG_STRUCT;

#define PCIEL0SC_PCIE_CFG_WIDTH 32
#define PCIEL0SC_PCIE_CFG_DEFAULT 0x0c0c0000U
#define PCIEL0SC_PCIE_CFG_ENDPOINT PCIE_CFG

#define PCIEL0SC_PCIE_CFG_G1L0SIC_LSB 0x0000
#define PCIEL0SC_PCIE_CFG_G1L0SIC_SIZE 0x0002
#define PCIEL0SC_PCIE_CFG_G2L0SIC_LSB 0x0002
#define PCIEL0SC_PCIE_CFG_G2L0SIC_SIZE 0x0002
#define PCIEL0SC_PCIE_CFG_TXL0SRXEXIT_LSB 0x0004
#define PCIEL0SC_PCIE_CFG_TXL0SRXEXIT_SIZE 0x0002
#define PCIEL0SC_PCIE_CFG_TXL0SRXENTRY_LSB 0x0006
#define PCIEL0SC_PCIE_CFG_TXL0SRXENTRY_SIZE 0x0001
#define PCIEL0SC_PCIE_CFG_ANFTSEN_LSB 0x0007
#define PCIEL0SC_PCIE_CFG_ANFTSEN_SIZE 0x0001
#define PCIEL0SC_PCIE_CFG_ANFTSO_LSB 0x0008
#define PCIEL0SC_PCIE_CFG_ANFTSO_SIZE 0x0008
#define PCIEL0SC_PCIE_CFG_G1ASL0SPL_LSB 0x0010
#define PCIEL0SC_PCIE_CFG_G1ASL0SPL_SIZE 0x0008
#define PCIEL0SC_PCIE_CFG_G2ASL0SPL_LSB 0x0018
#define PCIEL0SC_PCIE_CFG_G2ASL0SPL_SIZE 0x0008



/** PCIECFG2 desc:
  */

#define PCIECFG2_PCIE_CFG_REG            (0x00000320U)

typedef union {
  struct {
    UINT32 laangc : 3; /**< Link Arbiter Ack/Nak Grant count */
                            /* Bits[2:0], default=0x00000003*/
    UINT32 lafgc : 3; /**< Link Arbiter FCP Grant count */
                            /* Bits[5:3], default=0x00000003*/
    UINT32 latgc : 3; /**< Link Arbiter TLP Grant count */
                            /* Bits[8:6], default=0x00000003*/
    UINT32 g12skposl : 11; /**< GEN1/2 SKIP Ordered-Set Latency */
                            /* Bits[19:9], default=0x000004B0*/
    UINT32 pmet : 2; /**< PME Timeout */
                            /* Bits[21:20], default=0x00000000*/
    UINT32 crsren : 1; /**< Completion Retry Status Replay Enable */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[24:23] */
                            /* Bits[24:23], default=0*/
    UINT32 rpagc : 1; /**< Reset Prep ACK Generation Check */
                            /* Bits[25:25], default=0x00000001*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[26:26] */
                            /* Bits[26:26], default=0*/
    UINT32 rllg3r : 1; /**< Reset Link Layer In GEN3 Recovery */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 lsieid : 1; /**< Loopback Slave Infer Electrical Idle Disable */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 rrcp : 1; /**< RXL0s Receiver Control Policy */
                            /* Bits[29:29], default=0x00000001*/
    UINT32 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[30:30] */
                            /* Bits[30:30], default=0*/
    UINT32 g3l0sedp : 1; /**< GEN3 L0s Exit Deskew Policy */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PCIECFG2_PCIE_CFG_STRUCT;

#define PCIECFG2_PCIE_CFG_WIDTH 32
#define PCIECFG2_PCIE_CFG_DEFAULT 0x220960dbU
#define PCIECFG2_PCIE_CFG_ENDPOINT PCIE_CFG

#define PCIECFG2_PCIE_CFG_LAANGC_LSB 0x0000
#define PCIECFG2_PCIE_CFG_LAANGC_SIZE 0x0003
#define PCIECFG2_PCIE_CFG_LAFGC_LSB 0x0003
#define PCIECFG2_PCIE_CFG_LAFGC_SIZE 0x0003
#define PCIECFG2_PCIE_CFG_LATGC_LSB 0x0006
#define PCIECFG2_PCIE_CFG_LATGC_SIZE 0x0003
#define PCIECFG2_PCIE_CFG_G12SKPOSL_LSB 0x0009
#define PCIECFG2_PCIE_CFG_G12SKPOSL_SIZE 0x000b
#define PCIECFG2_PCIE_CFG_PMET_LSB 0x0014
#define PCIECFG2_PCIE_CFG_PMET_SIZE 0x0002
#define PCIECFG2_PCIE_CFG_CRSREN_LSB 0x0016
#define PCIECFG2_PCIE_CFG_CRSREN_SIZE 0x0001
#define PCIECFG2_PCIE_CFG_RPAGC_LSB 0x0019
#define PCIECFG2_PCIE_CFG_RPAGC_SIZE 0x0001
#define PCIECFG2_PCIE_CFG_RLLG3R_LSB 0x001b
#define PCIECFG2_PCIE_CFG_RLLG3R_SIZE 0x0001
#define PCIECFG2_PCIE_CFG_LSIEID_LSB 0x001c
#define PCIECFG2_PCIE_CFG_LSIEID_SIZE 0x0001
#define PCIECFG2_PCIE_CFG_RRCP_LSB 0x001d
#define PCIECFG2_PCIE_CFG_RRCP_SIZE 0x0001
#define PCIECFG2_PCIE_CFG_G3L0SEDP_LSB 0x001f
#define PCIECFG2_PCIE_CFG_G3L0SEDP_SIZE 0x0001



/** PCIEDBG desc:
  */

#define PCIEDBG_PCIE_CFG_REG             (0x00000324U)

typedef union {
  struct {
    UINT32 sncd : 1; /**< Sequence Number Checking Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 crcd : 1; /**< CRC Disable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 scmbb : 1; /**< Scrambler Bypass */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 dmil1edm : 1; /**< DMI L1 Entry Disable Mask */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[04:04] */
                            /* Bits[4:4], default=0*/
    UINT32 spce : 1; /**< Squelch Propagation Control Enable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 cmplrx : 1; /**< Compliance Receive */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 sqol0 : 1; /**< Squelch Off in L0 */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 repid : 1; /**< Receive Error Packet Invalidation Disable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 dsb : 1; /**< De-skew Bypass */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 irfelb : 1; /**< Initiate Remote Far-end Loop Back */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 nedlbe : 1; /**< Near End Digital Loopback Mode Enable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 g3csos : 1; /**< GEN3 Compliance SOS */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 ldswqrp : 1; /**< Link Down SWQ Reset Policy */
                            /* Bits[13:13], default=0x00000001*/
    UINT32 ctonfae : 1; /**< Completion Time-Out Non-Fatal Advisory Error Enable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 reutflpbkme : 1; /**< REUT Forced Loopback Master Entry */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 tals : 8; /**< Training Abort LTSSM State */
                            /* Bits[23:16], default=0x00000000*/
    UINT32 rsvd_1 : 4; /**< Undefined - auto filled rsvd_[27:24] */
                            /* Bits[27:24], default=0*/
    UINT32 reutlpbkme : 1; /**< REUT Loopback Master Entry */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 txnftsadd : 3; /**< Transmit nFTS Adder */
                            /* Bits[31:29], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PCIEDBG_PCIE_CFG_STRUCT;

#define PCIEDBG_PCIE_CFG_WIDTH 32
#define PCIEDBG_PCIE_CFG_DEFAULT 0x00002000U
#define PCIEDBG_PCIE_CFG_ENDPOINT PCIE_CFG

#define PCIEDBG_PCIE_CFG_SNCD_LSB 0x0000
#define PCIEDBG_PCIE_CFG_SNCD_SIZE 0x0001
#define PCIEDBG_PCIE_CFG_CRCD_LSB 0x0001
#define PCIEDBG_PCIE_CFG_CRCD_SIZE 0x0001
#define PCIEDBG_PCIE_CFG_SCMBB_LSB 0x0002
#define PCIEDBG_PCIE_CFG_SCMBB_SIZE 0x0001
#define PCIEDBG_PCIE_CFG_DMIL1EDM_LSB 0x0003
#define PCIEDBG_PCIE_CFG_DMIL1EDM_SIZE 0x0001
#define PCIEDBG_PCIE_CFG_SPCE_LSB 0x0005
#define PCIEDBG_PCIE_CFG_SPCE_SIZE 0x0001
#define PCIEDBG_PCIE_CFG_CMPLRX_LSB 0x0006
#define PCIEDBG_PCIE_CFG_CMPLRX_SIZE 0x0001
#define PCIEDBG_PCIE_CFG_SQOL0_LSB 0x0007
#define PCIEDBG_PCIE_CFG_SQOL0_SIZE 0x0001
#define PCIEDBG_PCIE_CFG_REPID_LSB 0x0008
#define PCIEDBG_PCIE_CFG_REPID_SIZE 0x0001
#define PCIEDBG_PCIE_CFG_DSB_LSB 0x0009
#define PCIEDBG_PCIE_CFG_DSB_SIZE 0x0001
#define PCIEDBG_PCIE_CFG_IRFELB_LSB 0x000a
#define PCIEDBG_PCIE_CFG_IRFELB_SIZE 0x0001
#define PCIEDBG_PCIE_CFG_NEDLBE_LSB 0x000b
#define PCIEDBG_PCIE_CFG_NEDLBE_SIZE 0x0001
#define PCIEDBG_PCIE_CFG_G3CSOS_LSB 0x000c
#define PCIEDBG_PCIE_CFG_G3CSOS_SIZE 0x0001
#define PCIEDBG_PCIE_CFG_LDSWQRP_LSB 0x000d
#define PCIEDBG_PCIE_CFG_LDSWQRP_SIZE 0x0001
#define PCIEDBG_PCIE_CFG_CTONFAE_LSB 0x000e
#define PCIEDBG_PCIE_CFG_CTONFAE_SIZE 0x0001
#define PCIEDBG_PCIE_CFG_REUTFLPBKME_LSB 0x000f
#define PCIEDBG_PCIE_CFG_REUTFLPBKME_SIZE 0x0001
#define PCIEDBG_PCIE_CFG_TALS_LSB 0x0010
#define PCIEDBG_PCIE_CFG_TALS_SIZE 0x0008
#define PCIEDBG_PCIE_CFG_REUTLPBKME_LSB 0x001c
#define PCIEDBG_PCIE_CFG_REUTLPBKME_SIZE 0x0001
#define PCIEDBG_PCIE_CFG_TXNFTSADD_LSB 0x001d
#define PCIEDBG_PCIE_CFG_TXNFTSADD_SIZE 0x0003



/** PCIESTS1 desc:
  */

#define PCIESTS1_PCIE_CFG_REG            (0x00000328U)

typedef union {
  struct {
    UINT32 nxttxseqnum : 12; /**< Next Transmitted Sequence Number */
                            /* Bits[11:0], default=0x00000000*/
    UINT32 rsvd_0 : 4; /**< Undefined - auto filled rsvd_[15:12] */
                            /* Bits[15:12], default=0*/
    UINT32 dllretry : 1; /**< Data Link Layer Retry */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 replaynum : 2; /**< Replay Number */
                            /* Bits[18:17], default=0x00000000*/
    UINT32 lnkstat : 4; /**< Link Status */
                            /* Bits[22:19], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[23:23] */
                            /* Bits[23:23], default=0*/
    UINT32 ltsmstate : 8; /**< LTSM State */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PCIESTS1_PCIE_CFG_STRUCT;

#define PCIESTS1_PCIE_CFG_WIDTH 32
#define PCIESTS1_PCIE_CFG_DEFAULT 0x00000000U
#define PCIESTS1_PCIE_CFG_ENDPOINT PCIE_CFG

#define PCIESTS1_PCIE_CFG_NXTTXSEQNUM_LSB 0x0000
#define PCIESTS1_PCIE_CFG_NXTTXSEQNUM_SIZE 0x000c
#define PCIESTS1_PCIE_CFG_DLLRETRY_LSB 0x0010
#define PCIESTS1_PCIE_CFG_DLLRETRY_SIZE 0x0001
#define PCIESTS1_PCIE_CFG_REPLAYNUM_LSB 0x0011
#define PCIESTS1_PCIE_CFG_REPLAYNUM_SIZE 0x0002
#define PCIESTS1_PCIE_CFG_LNKSTAT_LSB 0x0013
#define PCIESTS1_PCIE_CFG_LNKSTAT_SIZE 0x0004
#define PCIESTS1_PCIE_CFG_LTSMSTATE_LSB 0x0018
#define PCIESTS1_PCIE_CFG_LTSMSTATE_SIZE 0x0008



/** PCIEALC desc:
  */

#define PCIEALC_PCIE_CFG_REG             (0x00000338U)

typedef union {
  struct {
    UINT32 g3skposl : 9; /**< GEN3 SKIP Ordered-Set Latency */
                            /* Bits[8:0], default=0x00000172*/
    UINT32 acknakgh : 1; /**< ACK NAK Generation Handling */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 lpadllpgd : 1; /**< Low Priority ACK DLLP Grant Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 onpraspml1p : 1; /**< Outstanding Non-Posted Request ASPM L1 Prevention */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 obtmrtnal1r : 2; /**< Outstanding Block L1 Timer To NAK ASPM L1 Request */
                            /* Bits[13:12], default=0x00000000*/
    UINT32 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[15:14] */
                            /* Bits[15:14], default=0*/
    UINT32 clg3fe : 4; /**< Cause of Last GEN3 Framing Error */
                            /* Bits[19:16], default=0x00000000*/
    UINT32 pdsp : 1; /**< Present Detect State Policy */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 rtd3pdsp : 1; /**< RTD3 Present Detect State Policy */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 rrptlpc : 1; /**< Reset Receive Path using TLP Count */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 ssrrs : 1; /**< Survivability Sync Reset in Recovery state */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 ssrld : 1; /**< Survivability Sync Reset during Link Down */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 blkdqdasd : 1; /**< Block Detect.Quiet -> Detect.Active Strap Default */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 blkdqda : 1; /**< Block Detect.Quiet -> Detect.Active */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 blkpapc : 1; /**< Block Polling.Active -> Polling.Configuration */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 illrcld : 1; /**< Initialize Link Layer Receiver Control on Link Down */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 itlrcld : 1; /**< Initialize Transaction Layer Receiver Control on Link Down */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 dpchserm : 1; /**< DPC Handling during SERM */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 cssaif : 1; /**< Completion Status during SAI Failure */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PCIEALC_PCIE_CFG_STRUCT;

#define PCIEALC_PCIE_CFG_WIDTH 32
#define PCIEALC_PCIE_CFG_DEFAULT 0x00000172U
#define PCIEALC_PCIE_CFG_ENDPOINT PCIE_CFG

#define PCIEALC_PCIE_CFG_G3SKPOSL_LSB 0x0000
#define PCIEALC_PCIE_CFG_G3SKPOSL_SIZE 0x0009
#define PCIEALC_PCIE_CFG_ACKNAKGH_LSB 0x0009
#define PCIEALC_PCIE_CFG_ACKNAKGH_SIZE 0x0001
#define PCIEALC_PCIE_CFG_LPADLLPGD_LSB 0x000a
#define PCIEALC_PCIE_CFG_LPADLLPGD_SIZE 0x0001
#define PCIEALC_PCIE_CFG_ONPRASPML1P_LSB 0x000b
#define PCIEALC_PCIE_CFG_ONPRASPML1P_SIZE 0x0001
#define PCIEALC_PCIE_CFG_OBTMRTNAL1R_LSB 0x000c
#define PCIEALC_PCIE_CFG_OBTMRTNAL1R_SIZE 0x0002
#define PCIEALC_PCIE_CFG_CLG3FE_LSB 0x0010
#define PCIEALC_PCIE_CFG_CLG3FE_SIZE 0x0004
#define PCIEALC_PCIE_CFG_PDSP_LSB 0x0014
#define PCIEALC_PCIE_CFG_PDSP_SIZE 0x0001
#define PCIEALC_PCIE_CFG_RTD3PDSP_LSB 0x0015
#define PCIEALC_PCIE_CFG_RTD3PDSP_SIZE 0x0001
#define PCIEALC_PCIE_CFG_RRPTLPC_LSB 0x0016
#define PCIEALC_PCIE_CFG_RRPTLPC_SIZE 0x0001
#define PCIEALC_PCIE_CFG_SSRRS_LSB 0x0017
#define PCIEALC_PCIE_CFG_SSRRS_SIZE 0x0001
#define PCIEALC_PCIE_CFG_SSRLD_LSB 0x0018
#define PCIEALC_PCIE_CFG_SSRLD_SIZE 0x0001
#define PCIEALC_PCIE_CFG_BLKDQDASD_LSB 0x0019
#define PCIEALC_PCIE_CFG_BLKDQDASD_SIZE 0x0001
#define PCIEALC_PCIE_CFG_BLKDQDA_LSB 0x001a
#define PCIEALC_PCIE_CFG_BLKDQDA_SIZE 0x0001
#define PCIEALC_PCIE_CFG_BLKPAPC_LSB 0x001b
#define PCIEALC_PCIE_CFG_BLKPAPC_SIZE 0x0001
#define PCIEALC_PCIE_CFG_ILLRCLD_LSB 0x001c
#define PCIEALC_PCIE_CFG_ILLRCLD_SIZE 0x0001
#define PCIEALC_PCIE_CFG_ITLRCLD_LSB 0x001d
#define PCIEALC_PCIE_CFG_ITLRCLD_SIZE 0x0001
#define PCIEALC_PCIE_CFG_DPCHSERM_LSB 0x001e
#define PCIEALC_PCIE_CFG_DPCHSERM_SIZE 0x0001
#define PCIEALC_PCIE_CFG_CSSAIF_LSB 0x001f
#define PCIEALC_PCIE_CFG_CSSAIF_SIZE 0x0001



/** PTMECFG desc:
  */

#define PTMECFG_PCIE_CFG_REG             (0x000003b0U)

typedef union {
  struct {
    UINT32 iosfmadp : 4; /**< IOSF Max Allowed Delay programming */
                            /* Bits[3:0], default=0x00000000*/
    UINT32 ptmrnopad : 2; /**< PTM Request Number Of Periodic ACK DLLP */
                            /* Bits[5:4], default=0x00000000*/
    UINT32 ptmrpae : 1; /**< PTM Request Periodic ACK Enable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 tgtscf : 1; /**< Trigger Global Time Stamp Counter Fetch */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 pgtscfe : 1; /**< Periodic Global Time Stamp Counter Fetch Enable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 pgtscff : 3; /**< Periodic Global Time Stamp Counter Fetch Frequency */
                            /* Bits[11:9], default=0x00000000*/
    UINT32 gtfspc : 1; /**< Global Time Fetch Status Pending Completion */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 gtffc : 2; /**< Global Time Fetch Fail Counter */
                            /* Bits[14:13], default=0x00000000*/
    UINT32 gtfrc : 3; /**< Global Time Fetch Retry Counter */
                            /* Bits[17:15], default=0x00000000*/
    UINT32 pltlff : 3; /**< Periodic Local TSC Link Fetch Frequency */
                            /* Bits[20:18], default=0x00000000*/
    UINT32 ptmmmc : 2; /**< PTM Monitor Mux Control */
                            /* Bits[22:21], default=0x00000000*/
    UINT32 rsvd_m : 9; /**< Reserved */
                            /* Bits[31:23], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PTMECFG_PCIE_CFG_STRUCT;

#define PTMECFG_PCIE_CFG_WIDTH 32
#define PTMECFG_PCIE_CFG_DEFAULT 0x00000000U
#define PTMECFG_PCIE_CFG_ENDPOINT PCIE_CFG

#define PTMECFG_PCIE_CFG_IOSFMADP_LSB 0x0000
#define PTMECFG_PCIE_CFG_IOSFMADP_SIZE 0x0004
#define PTMECFG_PCIE_CFG_PTMRNOPAD_LSB 0x0004
#define PTMECFG_PCIE_CFG_PTMRNOPAD_SIZE 0x0002
#define PTMECFG_PCIE_CFG_PTMRPAE_LSB 0x0006
#define PTMECFG_PCIE_CFG_PTMRPAE_SIZE 0x0001
#define PTMECFG_PCIE_CFG_TGTSCF_LSB 0x0007
#define PTMECFG_PCIE_CFG_TGTSCF_SIZE 0x0001
#define PTMECFG_PCIE_CFG_PGTSCFE_LSB 0x0008
#define PTMECFG_PCIE_CFG_PGTSCFE_SIZE 0x0001
#define PTMECFG_PCIE_CFG_PGTSCFF_LSB 0x0009
#define PTMECFG_PCIE_CFG_PGTSCFF_SIZE 0x0003
#define PTMECFG_PCIE_CFG_GTFSPC_LSB 0x000c
#define PTMECFG_PCIE_CFG_GTFSPC_SIZE 0x0001
#define PTMECFG_PCIE_CFG_GTFFC_LSB 0x000d
#define PTMECFG_PCIE_CFG_GTFFC_SIZE 0x0002
#define PTMECFG_PCIE_CFG_GTFRC_LSB 0x000f
#define PTMECFG_PCIE_CFG_GTFRC_SIZE 0x0003
#define PTMECFG_PCIE_CFG_PLTLFF_LSB 0x0012
#define PTMECFG_PCIE_CFG_PLTLFF_SIZE 0x0003
#define PTMECFG_PCIE_CFG_PTMMMC_LSB 0x0015
#define PTMECFG_PCIE_CFG_PTMMMC_SIZE 0x0002
#define PTMECFG_PCIE_CFG_RSVD_M_LSB 0x0017
#define PTMECFG_PCIE_CFG_RSVD_M_SIZE 0x0009



/** LTROVR desc:
  */

#define LTROVR_PCIE_CFG_REG              (0x00000400U)

typedef union {
  struct {
    UINT32 ltrslovrv : 10; /**< LTR Snoop Latency Override Value */
                            /* Bits[9:0], default=0x00000000*/
    UINT32 ltrslsovrv : 3; /**< LTR Snoop Latency Scale Override Value */
                            /* Bits[12:10], default=0x00000000*/
    UINT32 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[14:13] */
                            /* Bits[14:13], default=0*/
    UINT32 ltrsrovr : 1; /**< LTR Snoop Requirement Bit Override */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 ltrnslovrv : 10; /**< LTR Non-Snoop Latency Override Value */
                            /* Bits[25:16], default=0x00000000*/
    UINT32 ltrnslsovrv : 3; /**< LTR Non-Snoop Latency Scale Override Value */
                            /* Bits[28:26], default=0x00000000*/
    UINT32 rsvd_1 : 2; /**< Undefined - auto filled rsvd_[30:29] */
                            /* Bits[30:29], default=0*/
    UINT32 ltrnsrovr : 1; /**< LTR Non-Snoop Requirement Bit Override */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} LTROVR_PCIE_CFG_STRUCT;

#define LTROVR_PCIE_CFG_WIDTH 32
#define LTROVR_PCIE_CFG_DEFAULT 0x00000000U
#define LTROVR_PCIE_CFG_ENDPOINT PCIE_CFG

#define LTROVR_PCIE_CFG_LTRSLOVRV_LSB 0x0000
#define LTROVR_PCIE_CFG_LTRSLOVRV_SIZE 0x000a
#define LTROVR_PCIE_CFG_LTRSLSOVRV_LSB 0x000a
#define LTROVR_PCIE_CFG_LTRSLSOVRV_SIZE 0x0003
#define LTROVR_PCIE_CFG_LTRSROVR_LSB 0x000f
#define LTROVR_PCIE_CFG_LTRSROVR_SIZE 0x0001
#define LTROVR_PCIE_CFG_LTRNSLOVRV_LSB 0x0010
#define LTROVR_PCIE_CFG_LTRNSLOVRV_SIZE 0x000a
#define LTROVR_PCIE_CFG_LTRNSLSOVRV_LSB 0x001a
#define LTROVR_PCIE_CFG_LTRNSLSOVRV_SIZE 0x0003
#define LTROVR_PCIE_CFG_LTRNSROVR_LSB 0x001f
#define LTROVR_PCIE_CFG_LTRNSROVR_SIZE 0x0001



/** LTROVR2 desc:
  */

#define LTROVR2_PCIE_CFG_REG             (0x00000404U)

typedef union {
  struct {
    UINT8 ltrsovren : 1; /**< LTR Snoop Override Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT8 ltrnsovren : 1; /**< LTR Non-Snoop Override Enable */
                            /* Bits[1:1], default=0x00000000*/
    UINT8 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[02:02] */
                            /* Bits[2:2], default=0*/
    UINT8 ltrovrplcy : 1; /**< LTR Override Policy */
                            /* Bits[3:3], default=0x00000000*/
    UINT8 dvltrrpl1p0 : 1; /**< Device LTR Requirement Policy For L1.0 & L1.Low */
                            /* Bits[4:4], default=0x00000000*/
    UINT8 dvltrrpl1p1 : 1; /**< Device LTR Requirement Policy For L1.1 */
                            /* Bits[5:5], default=0x00000000*/
    UINT8 dvltrrpl1p2 : 1; /**< Device LTR Requirement Policy For L1.2 */
                            /* Bits[6:6], default=0x00000000*/
    UINT8 rsvd_m : 1; /**< Reserved */
                            /* Bits[7:7], default=0x00000000*/
  }     Bits;
  UINT8 Data;
} LTROVR2_PCIE_CFG_STRUCT;

#define LTROVR2_PCIE_CFG_WIDTH 8
#define LTROVR2_PCIE_CFG_DEFAULT 0x00U
#define LTROVR2_PCIE_CFG_ENDPOINT PCIE_CFG

#define LTROVR2_PCIE_CFG_LTRSOVREN_LSB 0x0000
#define LTROVR2_PCIE_CFG_LTRSOVREN_SIZE 0x0001
#define LTROVR2_PCIE_CFG_LTRNSOVREN_LSB 0x0001
#define LTROVR2_PCIE_CFG_LTRNSOVREN_SIZE 0x0001
#define LTROVR2_PCIE_CFG_LTROVRPLCY_LSB 0x0003
#define LTROVR2_PCIE_CFG_LTROVRPLCY_SIZE 0x0001
#define LTROVR2_PCIE_CFG_DVLTRRPL1P0_LSB 0x0004
#define LTROVR2_PCIE_CFG_DVLTRRPL1P0_SIZE 0x0001
#define LTROVR2_PCIE_CFG_DVLTRRPL1P1_LSB 0x0005
#define LTROVR2_PCIE_CFG_DVLTRRPL1P1_SIZE 0x0001
#define LTROVR2_PCIE_CFG_DVLTRRPL1P2_LSB 0x0006
#define LTROVR2_PCIE_CFG_DVLTRRPL1P2_SIZE 0x0001
#define LTROVR2_PCIE_CFG_RSVD_M_LSB 0x0007
#define LTROVR2_PCIE_CFG_RSVD_M_SIZE 0x0001



/** PHYCTL4 desc:
  */

#define PHYCTL4_PCIE_CFG_REG             (0x00000408U)

typedef union {
  struct {
    UINT32 g12ctsdp : 1; /**< GEN1/2 Consecutive Training Sequence Decode Policy */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 g3ctsdp : 1; /**< GEN3 Consecutive Training Sequence Decode Policy */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 g4ctsdp : 1; /**< GEN4 Consecutive Training Sequence Decode Policy */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 ltssmpcoe : 1; /**< LTSSM Port Config Override Enable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 ltssmpcod : 1; /**< LTSSM Port Config Override Directions */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 aartle : 1; /**< AFE Analog RX to TX Loopback Enable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 rcvdpdsp : 1; /**< Receiver Detection in Powerdown State Policy */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 aatrle : 1; /**< AFE Analog X to RX Loopback Enable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 arcfuncfl : 1; /**< Allow Rate Change for Unconfigured Lane */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 rsvd_0 : 18; /**< Undefined - auto filled rsvd_[26:09] */
                            /* Bits[26:9], default=0*/
    UINT32 sqdis : 1; /**< Squelch Disable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 rsvd_m : 4; /**< Reserved */
                            /* Bits[31:28], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PHYCTL4_PCIE_CFG_STRUCT;

#define PHYCTL4_PCIE_CFG_WIDTH 32
#define PHYCTL4_PCIE_CFG_DEFAULT 0x00000000U
#define PHYCTL4_PCIE_CFG_ENDPOINT PCIE_CFG

#define PHYCTL4_PCIE_CFG_G12CTSDP_LSB 0x0000
#define PHYCTL4_PCIE_CFG_G12CTSDP_SIZE 0x0001
#define PHYCTL4_PCIE_CFG_G3CTSDP_LSB 0x0001
#define PHYCTL4_PCIE_CFG_G3CTSDP_SIZE 0x0001
#define PHYCTL4_PCIE_CFG_G4CTSDP_LSB 0x0002
#define PHYCTL4_PCIE_CFG_G4CTSDP_SIZE 0x0001
#define PHYCTL4_PCIE_CFG_LTSSMPCOE_LSB 0x0003
#define PHYCTL4_PCIE_CFG_LTSSMPCOE_SIZE 0x0001
#define PHYCTL4_PCIE_CFG_LTSSMPCOD_LSB 0x0004
#define PHYCTL4_PCIE_CFG_LTSSMPCOD_SIZE 0x0001
#define PHYCTL4_PCIE_CFG_AARTLE_LSB 0x0005
#define PHYCTL4_PCIE_CFG_AARTLE_SIZE 0x0001
#define PHYCTL4_PCIE_CFG_RCVDPDSP_LSB 0x0006
#define PHYCTL4_PCIE_CFG_RCVDPDSP_SIZE 0x0001
#define PHYCTL4_PCIE_CFG_AATRLE_LSB 0x0007
#define PHYCTL4_PCIE_CFG_AATRLE_SIZE 0x0001
#define PHYCTL4_PCIE_CFG_ARCFUNCFL_LSB 0x0008
#define PHYCTL4_PCIE_CFG_ARCFUNCFL_SIZE 0x0001
#define PHYCTL4_PCIE_CFG_SQDIS_LSB 0x001b
#define PHYCTL4_PCIE_CFG_SQDIS_SIZE 0x0001
#define PHYCTL4_PCIE_CFG_RSVD_M_LSB 0x001c
#define PHYCTL4_PCIE_CFG_RSVD_M_SIZE 0x0004



/** STRPFUSECFG2 desc:
  */

#define STRPFUSECFG2_PCIE_CFG_REG        (0x00000414U)

typedef union {
  struct {
    UINT32 ltlppdis : 1; /**< Local TLP Prefix Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 cpusel : 1; /**< CPU Select */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 dmitxswing : 1; /**< DMI TX Swing */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 fdsv : 3; /**< DMI Force Detect Strap Value */
                            /* Bits[5:3], default=0x00000000*/
    UINT32 vtdte : 1; /**< VT-d Translation Enable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 rsvd_0 : 7; /**< Undefined - auto filled rsvd_[13:07] */
                            /* Bits[13:7], default=0*/
    UINT32 ebm : 1; /**< Elastic Buffer Mode */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 forcel : 3; /**< Force Limit to x1,x2,x4,x8,x16 */
                            /* Bits[17:15], default=0x00000000*/
    UINT32 fcsl : 2; /**< Flow Control Scale Limit */
                            /* Bits[19:18], default=0x00000003*/
    UINT32 pcieslf : 3; /**< PCI Express Speed Limit */
                            /* Bits[22:20], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[23:23] */
                            /* Bits[23:23], default=0*/
    UINT32 pciesl : 3; /**< PCI Express Speed Limit */
                            /* Bits[26:24], default=0x00000000*/
    UINT32 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[27:27] */
                            /* Bits[27:27], default=0*/
    UINT32 ebmg3 : 1; /**< Elastic Buffer Mode Gen3 */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 rsvd_m : 3; /**< Reserved */
                            /* Bits[31:29], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} STRPFUSECFG2_PCIE_CFG_STRUCT;

#define STRPFUSECFG2_PCIE_CFG_WIDTH 32
#define STRPFUSECFG2_PCIE_CFG_DEFAULT 0x000c0000U
#define STRPFUSECFG2_PCIE_CFG_ENDPOINT PCIE_CFG

#define STRPFUSECFG2_PCIE_CFG_LTLPPDIS_LSB 0x0000
#define STRPFUSECFG2_PCIE_CFG_LTLPPDIS_SIZE 0x0001
#define STRPFUSECFG2_PCIE_CFG_CPUSEL_LSB 0x0001
#define STRPFUSECFG2_PCIE_CFG_CPUSEL_SIZE 0x0001
#define STRPFUSECFG2_PCIE_CFG_DMITXSWING_LSB 0x0002
#define STRPFUSECFG2_PCIE_CFG_DMITXSWING_SIZE 0x0001
#define STRPFUSECFG2_PCIE_CFG_FDSV_LSB 0x0003
#define STRPFUSECFG2_PCIE_CFG_FDSV_SIZE 0x0003
#define STRPFUSECFG2_PCIE_CFG_VTDTE_LSB 0x0006
#define STRPFUSECFG2_PCIE_CFG_VTDTE_SIZE 0x0001
#define STRPFUSECFG2_PCIE_CFG_EBM_LSB 0x000e
#define STRPFUSECFG2_PCIE_CFG_EBM_SIZE 0x0001
#define STRPFUSECFG2_PCIE_CFG_FORCEL_LSB 0x000f
#define STRPFUSECFG2_PCIE_CFG_FORCEL_SIZE 0x0003
#define STRPFUSECFG2_PCIE_CFG_FCSL_LSB 0x0012
#define STRPFUSECFG2_PCIE_CFG_FCSL_SIZE 0x0002
#define STRPFUSECFG2_PCIE_CFG_PCIESLF_LSB 0x0014
#define STRPFUSECFG2_PCIE_CFG_PCIESLF_SIZE 0x0003
#define STRPFUSECFG2_PCIE_CFG_PCIESL_LSB 0x0018
#define STRPFUSECFG2_PCIE_CFG_PCIESL_SIZE 0x0003
#define STRPFUSECFG2_PCIE_CFG_EBMG3_LSB 0x001c
#define STRPFUSECFG2_PCIE_CFG_EBMG3_SIZE 0x0001
#define STRPFUSECFG2_PCIE_CFG_RSVD_M_LSB 0x001d
#define STRPFUSECFG2_PCIE_CFG_RSVD_M_SIZE 0x0003



/** TNPT desc:
  */

#define TNPT_PCIE_CFG_REG                (0x00000418U)

typedef union {
  struct {
    UINT32 dtxlte : 1; /**< Dynamic TX Link Throttling Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 drxlte : 1; /**< Dynamic RX Link Throttling Enable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 ttg : 1; /**< Throttling Timer Granularity */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[03:03] */
                            /* Bits[3:3], default=0*/
    UINT32 tsl0m : 2; /**< Throttling Severity Level 0 Multiplier */
                            /* Bits[5:4], default=0x00000003*/
    UINT32 tsl1m : 2; /**< Throttling Severity Level 1 Multiplier */
                            /* Bits[7:6], default=0x00000000*/
    UINT32 tsl2m : 2; /**< Throttling Severity Level 2 Multiplier */
                            /* Bits[9:8], default=0x00000001*/
    UINT32 tsl3m : 2; /**< Throttling Severity Level 3 Multiplier */
                            /* Bits[11:10], default=0x00000002*/
    UINT32 rsvd_1 : 4; /**< Undefined - auto filled rsvd_[15:12] */
                            /* Bits[15:12], default=0*/
    UINT32 tt : 8; /**< Throttle Time */
                            /* Bits[23:16], default=0x00000000*/
    UINT32 tp : 8; /**< Throttle Period */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} TNPT_PCIE_CFG_STRUCT;

#define TNPT_PCIE_CFG_WIDTH 32
#define TNPT_PCIE_CFG_DEFAULT 0x00000930U
#define TNPT_PCIE_CFG_ENDPOINT PCIE_CFG

#define TNPT_PCIE_CFG_DTXLTE_LSB 0x0000
#define TNPT_PCIE_CFG_DTXLTE_SIZE 0x0001
#define TNPT_PCIE_CFG_DRXLTE_LSB 0x0001
#define TNPT_PCIE_CFG_DRXLTE_SIZE 0x0001
#define TNPT_PCIE_CFG_TTG_LSB 0x0002
#define TNPT_PCIE_CFG_TTG_SIZE 0x0001
#define TNPT_PCIE_CFG_TSL0M_LSB 0x0004
#define TNPT_PCIE_CFG_TSL0M_SIZE 0x0002
#define TNPT_PCIE_CFG_TSL1M_LSB 0x0006
#define TNPT_PCIE_CFG_TSL1M_SIZE 0x0002
#define TNPT_PCIE_CFG_TSL2M_LSB 0x0008
#define TNPT_PCIE_CFG_TSL2M_SIZE 0x0002
#define TNPT_PCIE_CFG_TSL3M_LSB 0x000a
#define TNPT_PCIE_CFG_TSL3M_SIZE 0x0002
#define TNPT_PCIE_CFG_TT_LSB 0x0010
#define TNPT_PCIE_CFG_TT_SIZE 0x0008
#define TNPT_PCIE_CFG_TP_LSB 0x0018
#define TNPT_PCIE_CFG_TP_SIZE 0x0008



/** PCIEPMECTL desc:
  */

#define PCIEPMECTL_PCIE_CFG_REG          (0x00000420U)

typedef union {
  struct {
    UINT32 l1fsoe : 1; /**< L1 Full Squelch OFF Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[02:01] */
                            /* Bits[2:1], default=0*/
    UINT32 l1snzcrewd : 1; /**< L1.SNOOZ/L1.LOW Clock Request Early Wake Disable */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 l1ltrtlv : 10; /**< L1.LOW LTR Threshold Latency Value */
                            /* Bits[13:4], default=0x00000014*/
    UINT32 l1ltrtlsv : 3; /**< L1.LOW LTR Threshold Latency ScaleValue */
                            /* Bits[16:14], default=0x00000002*/
    UINT32 l1le : 1; /**< L1.LOW Enable */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 poffwt : 2; /**< Power Off Wait Time */
                            /* Bits[19:18], default=0x00000003*/
    UINT32 ipacpe : 1; /**< IP-Accessible Context Propagation Enable */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 ipiep : 1; /**< IP-Inaccessible Entry Policy */
                            /* Bits[21:21], default=0x00000001*/
    UINT32 l1offmrt : 2; /**< L1.OFF Minimum Residency Time */
                            /* Bits[23:22], default=0x00000002*/
    UINT32 l1ormrt : 2; /**< L1.OFF_RDY Minimum Residency Time */
                            /* Bits[25:24], default=0x00000002*/
    UINT32 rsvd_1 : 2; /**< Undefined - auto filled rsvd_[27:26] */
                            /* Bits[27:26], default=0*/
    UINT32 l1ocrewd : 1; /**< L1.OFF Clock Request Early Wake Disable */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 dlsuldlsd : 1; /**< Disabled, Detect, L23_Rdy State,Un-Configured Lane and Down-Configured Lane Squelch Disable */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 dlsulppge : 1; /**< Disabled, Detect and L23_Rdy State PHY Lane Power Gating Enable */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 fdppge : 1; /**< Function Disable PHY Power Gating Enable */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PCIEPMECTL_PCIE_CFG_STRUCT;

#define PCIEPMECTL_PCIE_CFG_WIDTH 32
#define PCIEPMECTL_PCIE_CFG_DEFAULT 0x02ac8140U
#define PCIEPMECTL_PCIE_CFG_ENDPOINT PCIE_CFG

#define PCIEPMECTL_PCIE_CFG_L1FSOE_LSB 0x0000
#define PCIEPMECTL_PCIE_CFG_L1FSOE_SIZE 0x0001
#define PCIEPMECTL_PCIE_CFG_L1SNZCREWD_LSB 0x0003
#define PCIEPMECTL_PCIE_CFG_L1SNZCREWD_SIZE 0x0001
#define PCIEPMECTL_PCIE_CFG_L1LTRTLV_LSB 0x0004
#define PCIEPMECTL_PCIE_CFG_L1LTRTLV_SIZE 0x000a
#define PCIEPMECTL_PCIE_CFG_L1LTRTLSV_LSB 0x000e
#define PCIEPMECTL_PCIE_CFG_L1LTRTLSV_SIZE 0x0003
#define PCIEPMECTL_PCIE_CFG_L1LE_LSB 0x0011
#define PCIEPMECTL_PCIE_CFG_L1LE_SIZE 0x0001
#define PCIEPMECTL_PCIE_CFG_POFFWT_LSB 0x0012
#define PCIEPMECTL_PCIE_CFG_POFFWT_SIZE 0x0002
#define PCIEPMECTL_PCIE_CFG_IPACPE_LSB 0x0014
#define PCIEPMECTL_PCIE_CFG_IPACPE_SIZE 0x0001
#define PCIEPMECTL_PCIE_CFG_IPIEP_LSB 0x0015
#define PCIEPMECTL_PCIE_CFG_IPIEP_SIZE 0x0001
#define PCIEPMECTL_PCIE_CFG_L1OFFMRT_LSB 0x0016
#define PCIEPMECTL_PCIE_CFG_L1OFFMRT_SIZE 0x0002
#define PCIEPMECTL_PCIE_CFG_L1ORMRT_LSB 0x0018
#define PCIEPMECTL_PCIE_CFG_L1ORMRT_SIZE 0x0002
#define PCIEPMECTL_PCIE_CFG_L1OCREWD_LSB 0x001c
#define PCIEPMECTL_PCIE_CFG_L1OCREWD_SIZE 0x0001
#define PCIEPMECTL_PCIE_CFG_DLSULDLSD_LSB 0x001d
#define PCIEPMECTL_PCIE_CFG_DLSULDLSD_SIZE 0x0001
#define PCIEPMECTL_PCIE_CFG_DLSULPPGE_LSB 0x001e
#define PCIEPMECTL_PCIE_CFG_DLSULPPGE_SIZE 0x0001
#define PCIEPMECTL_PCIE_CFG_FDPPGE_LSB 0x001f
#define PCIEPMECTL_PCIE_CFG_FDPPGE_SIZE 0x0001



/** PCIEPMECTL2 desc:
  */

#define PCIEPMECTL2_PCIE_CFG_REG         (0x00000424U)

typedef union {
  struct {
    UINT32 sl1oee : 1; /**< Speculative L1.OFF Exit Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[01:01] */
                            /* Bits[1:1], default=0*/
    UINT32 l1otcmrp : 1; /**< L1.OFF TX Common Mode Re-Establishment Policy */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 dspm : 1; /**< Detect State P-State Mapping */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 l1scpge : 1; /**< L1 State Controller Power Gating Enable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 disscpge : 1; /**< Disabled State Controller Power Gating Enable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 l23rdyscpge : 1; /**< L23_Rdy State Controller Power Gating Enable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 detscpge : 1; /**< Detect State Controller Power Gating Enable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 fdcpge : 1; /**< Function Disable Controller Power Gating Enable */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 l1sphydlpge : 1; /**< Reserved */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[10:10] */
                            /* Bits[10:10], default=0*/
    UINT32 phyclpge : 1; /**< PHY Common Lane Power Gating Enable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 cpgenh : 2; /**< Controller Power Gating Entry Hysteresis */
                            /* Bits[13:12], default=0x00000001*/
    UINT32 cpgexh : 2; /**< Controller Power Gating Exit Hysteresis */
                            /* Bits[15:14], default=0x00000003*/
    UINT32 mexl1pgltrtlv : 10; /**< Mobile Express L1 Power Gating LTR Threshold Latency Value */
                            /* Bits[25:16], default=0x00000000*/
    UINT32 l1sppge : 1; /**< Mobile Express L1 State PHY Power Gating Enable */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 cpmcsre : 1; /**< Chassis PMC Save and Restore Enable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 pgcbbld : 1; /**< PGCB Boundary Lock Disable */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 mexl1pgltrtlsv : 3; /**< Mobile Express L1 Power Gating LTR Threshold Latency Scale Value */
                            /* Bits[31:29], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PCIEPMECTL2_PCIE_CFG_STRUCT;

#define PCIEPMECTL2_PCIE_CFG_WIDTH 32
#define PCIEPMECTL2_PCIE_CFG_DEFAULT 0x0000d000U
#define PCIEPMECTL2_PCIE_CFG_ENDPOINT PCIE_CFG

#define PCIEPMECTL2_PCIE_CFG_SL1OEE_LSB 0x0000
#define PCIEPMECTL2_PCIE_CFG_SL1OEE_SIZE 0x0001
#define PCIEPMECTL2_PCIE_CFG_L1OTCMRP_LSB 0x0002
#define PCIEPMECTL2_PCIE_CFG_L1OTCMRP_SIZE 0x0001
#define PCIEPMECTL2_PCIE_CFG_DSPM_LSB 0x0003
#define PCIEPMECTL2_PCIE_CFG_DSPM_SIZE 0x0001
#define PCIEPMECTL2_PCIE_CFG_L1SCPGE_LSB 0x0004
#define PCIEPMECTL2_PCIE_CFG_L1SCPGE_SIZE 0x0001
#define PCIEPMECTL2_PCIE_CFG_DISSCPGE_LSB 0x0005
#define PCIEPMECTL2_PCIE_CFG_DISSCPGE_SIZE 0x0001
#define PCIEPMECTL2_PCIE_CFG_L23RDYSCPGE_LSB 0x0006
#define PCIEPMECTL2_PCIE_CFG_L23RDYSCPGE_SIZE 0x0001
#define PCIEPMECTL2_PCIE_CFG_DETSCPGE_LSB 0x0007
#define PCIEPMECTL2_PCIE_CFG_DETSCPGE_SIZE 0x0001
#define PCIEPMECTL2_PCIE_CFG_FDCPGE_LSB 0x0008
#define PCIEPMECTL2_PCIE_CFG_FDCPGE_SIZE 0x0001
#define PCIEPMECTL2_PCIE_CFG_L1SPHYDLPGE_LSB 0x0009
#define PCIEPMECTL2_PCIE_CFG_L1SPHYDLPGE_SIZE 0x0001
#define PCIEPMECTL2_PCIE_CFG_PHYCLPGE_LSB 0x000b
#define PCIEPMECTL2_PCIE_CFG_PHYCLPGE_SIZE 0x0001
#define PCIEPMECTL2_PCIE_CFG_CPGENH_LSB 0x000c
#define PCIEPMECTL2_PCIE_CFG_CPGENH_SIZE 0x0002
#define PCIEPMECTL2_PCIE_CFG_CPGEXH_LSB 0x000e
#define PCIEPMECTL2_PCIE_CFG_CPGEXH_SIZE 0x0002
#define PCIEPMECTL2_PCIE_CFG_MEXL1PGLTRTLV_LSB 0x0010
#define PCIEPMECTL2_PCIE_CFG_MEXL1PGLTRTLV_SIZE 0x000a
#define PCIEPMECTL2_PCIE_CFG_L1SPPGE_LSB 0x001a
#define PCIEPMECTL2_PCIE_CFG_L1SPPGE_SIZE 0x0001
#define PCIEPMECTL2_PCIE_CFG_CPMCSRE_LSB 0x001b
#define PCIEPMECTL2_PCIE_CFG_CPMCSRE_SIZE 0x0001
#define PCIEPMECTL2_PCIE_CFG_PGCBBLD_LSB 0x001c
#define PCIEPMECTL2_PCIE_CFG_PGCBBLD_SIZE 0x0001
#define PCIEPMECTL2_PCIE_CFG_MEXL1PGLTRTLSV_LSB 0x001d
#define PCIEPMECTL2_PCIE_CFG_MEXL1PGLTRTLSV_SIZE 0x0003



/** PCE desc:
  */

#define PCE_PCIE_CFG_REG                 (0x00000428U)

typedef union {
  struct {
    UINT8 pmcre : 1; /**< PMC Request Enable */
                            /* Bits[0:0], default=0x00000001*/
    UINT8 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[02:01] */
                            /* Bits[2:1], default=0*/
    UINT8 se : 1; /**< Sleep Enable */
                            /* Bits[3:3], default=0x00000001*/
    UINT8 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[04:04] */
                            /* Bits[4:4], default=0*/
    UINT8 hae : 1; /**< Hardware Autonomous Enable */
                            /* Bits[5:5], default=0x00000000*/
    UINT8 rsvd_m : 2; /**< Reserved */
                            /* Bits[7:6], default=0x00000000*/
  }     Bits;
  UINT8 Data;
} PCE_PCIE_CFG_STRUCT;

#define PCE_PCIE_CFG_WIDTH 8
#define PCE_PCIE_CFG_DEFAULT 0x09U
#define PCE_PCIE_CFG_ENDPOINT PCIE_CFG

#define PCE_PCIE_CFG_PMCRE_LSB 0x0000
#define PCE_PCIE_CFG_PMCRE_SIZE 0x0001
#define PCE_PCIE_CFG_SE_LSB 0x0003
#define PCE_PCIE_CFG_SE_SIZE 0x0001
#define PCE_PCIE_CFG_HAE_LSB 0x0005
#define PCE_PCIE_CFG_HAE_SIZE 0x0001
#define PCE_PCIE_CFG_RSVD_M_LSB 0x0006
#define PCE_PCIE_CFG_RSVD_M_SIZE 0x0002



/** PCIEPMECTL3 desc:
  */

#define PCIEPMECTL3_PCIE_CFG_REG         (0x00000434U)

typedef union {
  struct {
    UINT32 pmreqcpgexh : 2; /**< PM Request Controller Power Gating Exit Hysteresis */
                            /* Bits[1:0], default=0x00000003*/
    UINT32 osccgh : 2; /**< Osc Clock Gate Hysterisis */
                            /* Bits[3:2], default=0x00000002*/
    UINT32 l1pgautopgen : 1; /**< L1.PG Auto Power Gate Enable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 rsvd_rw : 27; /**< Reserved */
                            /* Bits[31:5], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PCIEPMECTL3_PCIE_CFG_STRUCT;

#define PCIEPMECTL3_PCIE_CFG_WIDTH 32
#define PCIEPMECTL3_PCIE_CFG_DEFAULT 0x0000000bU
#define PCIEPMECTL3_PCIE_CFG_ENDPOINT PCIE_CFG

#define PCIEPMECTL3_PCIE_CFG_PMREQCPGEXH_LSB 0x0000
#define PCIEPMECTL3_PCIE_CFG_PMREQCPGEXH_SIZE 0x0002
#define PCIEPMECTL3_PCIE_CFG_OSCCGH_LSB 0x0002
#define PCIEPMECTL3_PCIE_CFG_OSCCGH_SIZE 0x0002
#define PCIEPMECTL3_PCIE_CFG_L1PGAUTOPGEN_LSB 0x0004
#define PCIEPMECTL3_PCIE_CFG_L1PGAUTOPGEN_SIZE 0x0001
#define PCIEPMECTL3_PCIE_CFG_RSVD_RW_LSB 0x0005
#define PCIEPMECTL3_PCIE_CFG_RSVD_RW_SIZE 0x001b



/** EQCFG1 desc:
  */

#define EQCFG1_PCIE_CFG_REG              (0x00000450U)

typedef union {
  struct {
    UINT32 rupp : 1; /**< Receiver Use Preset Policy */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 tupp : 1; /**< Transmitter Use Preset Policy */
                            /* Bits[1:1], default=0x00000001*/
    UINT32 mflntl : 1; /**< Multi-Fragment Linear and Nine-Tile List Enable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 leqp01tcsp : 1; /**< Link EQ Phase 0 or 1 Transmit Coefficient Settling Policy */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 meqsmmflntl : 1; /**< Multi-EQ Search Mode, Multi fragment linear and nine tile list Enable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 hapccpie : 1; /**< Hardware Autonomous Preset/Coefficient Count Per-Iteration Extention */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[06:06] */
                            /* Bits[6:6], default=0*/
    UINT32 eqts2irrc : 1; /**< EQ TS2 in Recovery.ReceiverConfig Enable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 rwtneve : 4; /**< Receiver Wait Time For New Equalization Value Evaluation */
                            /* Bits[11:8], default=0x00000001*/
    UINT32 haed : 1; /**< Hardware Autonomous Equalization Done */
                            /* Bits[12:12], default=0x00000001*/
    UINT32 hpcmqe : 1; /**< Hardware Preset to Coefficient Mapping Query Enable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 lerscie : 1; /**< Link Equalization Request SCI Enable */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 rtpcoe : 1; /**< Remote Transmitter Preset Coefficient Override Enable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 rtlepceb : 1; /**< Remote Transmit Link Equalization Preset/Coefficient Evaluation Bypass */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 lep3b : 1; /**< Link Equalization 3 Bypass */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 lep23b : 1; /**< Link Equalization Phase 2 and 3 Bypass */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 leb : 1; /**< Link Equalization Bypass */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 reic : 1; /**< Reset EIEOS Interval Count */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 lersmie : 1; /**< Link Equalization Request SMI Enable */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 qg : 1; /**< Quiesce Guarantee */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 reifece : 1; /**< Recovery Entry and Idle Framing Error Count Enable */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 rec : 8; /**< Recovery Entry Count */
                            /* Bits[31:24], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} EQCFG1_PCIE_CFG_STRUCT;

#define EQCFG1_PCIE_CFG_WIDTH 32
#define EQCFG1_PCIE_CFG_DEFAULT 0x00001102U
#define EQCFG1_PCIE_CFG_ENDPOINT PCIE_CFG

#define EQCFG1_PCIE_CFG_RUPP_LSB 0x0000
#define EQCFG1_PCIE_CFG_RUPP_SIZE 0x0001
#define EQCFG1_PCIE_CFG_TUPP_LSB 0x0001
#define EQCFG1_PCIE_CFG_TUPP_SIZE 0x0001
#define EQCFG1_PCIE_CFG_MFLNTL_LSB 0x0002
#define EQCFG1_PCIE_CFG_MFLNTL_SIZE 0x0001
#define EQCFG1_PCIE_CFG_LEQP01TCSP_LSB 0x0003
#define EQCFG1_PCIE_CFG_LEQP01TCSP_SIZE 0x0001
#define EQCFG1_PCIE_CFG_MEQSMMFLNTL_LSB 0x0004
#define EQCFG1_PCIE_CFG_MEQSMMFLNTL_SIZE 0x0001
#define EQCFG1_PCIE_CFG_HAPCCPIE_LSB 0x0005
#define EQCFG1_PCIE_CFG_HAPCCPIE_SIZE 0x0001
#define EQCFG1_PCIE_CFG_EQTS2IRRC_LSB 0x0007
#define EQCFG1_PCIE_CFG_EQTS2IRRC_SIZE 0x0001
#define EQCFG1_PCIE_CFG_RWTNEVE_LSB 0x0008
#define EQCFG1_PCIE_CFG_RWTNEVE_SIZE 0x0004
#define EQCFG1_PCIE_CFG_HAED_LSB 0x000c
#define EQCFG1_PCIE_CFG_HAED_SIZE 0x0001
#define EQCFG1_PCIE_CFG_HPCMQE_LSB 0x000d
#define EQCFG1_PCIE_CFG_HPCMQE_SIZE 0x0001
#define EQCFG1_PCIE_CFG_LERSCIE_LSB 0x000e
#define EQCFG1_PCIE_CFG_LERSCIE_SIZE 0x0001
#define EQCFG1_PCIE_CFG_RTPCOE_LSB 0x000f
#define EQCFG1_PCIE_CFG_RTPCOE_SIZE 0x0001
#define EQCFG1_PCIE_CFG_RTLEPCEB_LSB 0x0010
#define EQCFG1_PCIE_CFG_RTLEPCEB_SIZE 0x0001
#define EQCFG1_PCIE_CFG_LEP3B_LSB 0x0011
#define EQCFG1_PCIE_CFG_LEP3B_SIZE 0x0001
#define EQCFG1_PCIE_CFG_LEP23B_LSB 0x0012
#define EQCFG1_PCIE_CFG_LEP23B_SIZE 0x0001
#define EQCFG1_PCIE_CFG_LEB_LSB 0x0013
#define EQCFG1_PCIE_CFG_LEB_SIZE 0x0001
#define EQCFG1_PCIE_CFG_REIC_LSB 0x0014
#define EQCFG1_PCIE_CFG_REIC_SIZE 0x0001
#define EQCFG1_PCIE_CFG_LERSMIE_LSB 0x0015
#define EQCFG1_PCIE_CFG_LERSMIE_SIZE 0x0001
#define EQCFG1_PCIE_CFG_QG_LSB 0x0016
#define EQCFG1_PCIE_CFG_QG_SIZE 0x0001
#define EQCFG1_PCIE_CFG_REIFECE_LSB 0x0017
#define EQCFG1_PCIE_CFG_REIFECE_SIZE 0x0001
#define EQCFG1_PCIE_CFG_REC_LSB 0x0018
#define EQCFG1_PCIE_CFG_REC_SIZE 0x0008



/** RTPCL1 desc:
  */

#define RTPCL1_PCIE_CFG_REG              (0x00000454U)

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
} RTPCL1_PCIE_CFG_STRUCT;

#define RTPCL1_PCIE_CFG_WIDTH 32
#define RTPCL1_PCIE_CFG_DEFAULT 0x00000000U
#define RTPCL1_PCIE_CFG_ENDPOINT PCIE_CFG

#define RTPCL1_PCIE_CFG_RTPRECL0PL0_LSB 0x0000
#define RTPCL1_PCIE_CFG_RTPRECL0PL0_SIZE 0x0006
#define RTPCL1_PCIE_CFG_RTPOSTCL0PL1_LSB 0x0006
#define RTPCL1_PCIE_CFG_RTPOSTCL0PL1_SIZE 0x0006
#define RTPCL1_PCIE_CFG_RTPRECL1PL2_LSB 0x000c
#define RTPCL1_PCIE_CFG_RTPRECL1PL2_SIZE 0x0006
#define RTPCL1_PCIE_CFG_RTPOSTCL1PL3_LSB 0x0012
#define RTPCL1_PCIE_CFG_RTPOSTCL1PL3_SIZE 0x0006
#define RTPCL1_PCIE_CFG_RTPRECL2PL4_LSB 0x0018
#define RTPCL1_PCIE_CFG_RTPRECL2PL4_SIZE 0x0006
#define RTPCL1_PCIE_CFG_PCM_LSB 0x001f
#define RTPCL1_PCIE_CFG_PCM_SIZE 0x0001



/** RTPCL2 desc:
  */

#define RTPCL2_PCIE_CFG_REG              (0x00000458U)

typedef union {
  struct {
    UINT32 rtpostcl2pl5 : 6; /**< Remote Transmitter Post-Cursor Coefficient List 2/Preset List 5 */
                            /* Bits[5:0], default=0x00000000*/
    UINT32 rtprecl3pl6 : 6; /**< Remote Transmitter Pre-Cursor Coefficient List 3/Preset List 6 */
                            /* Bits[11:6], default=0x00000000*/
    UINT32 rtpostcl3pl7 : 6; /**< Remote Transmitter Post-Cursor Coefficient List 3/Preset List 7 */
                            /* Bits[17:12], default=0x00000000*/
    UINT32 rtprecl4pl8 : 6; /**< Remote Transmitter Pre-Cursor Coefficient List 4/Preset List 8 */
                            /* Bits[23:18], default=0x00000000*/
    UINT32 rtpostcl4pl9 : 6; /**< Remote Transmitter Post-Cursor Coefficient List 4/Preset List 9 */
                            /* Bits[29:24], default=0x00000000*/
    UINT32 rsvd_m : 2; /**< Reserved */
                            /* Bits[31:30], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RTPCL2_PCIE_CFG_STRUCT;

#define RTPCL2_PCIE_CFG_WIDTH 32
#define RTPCL2_PCIE_CFG_DEFAULT 0x00000000U
#define RTPCL2_PCIE_CFG_ENDPOINT PCIE_CFG

#define RTPCL2_PCIE_CFG_RTPOSTCL2PL5_LSB 0x0000
#define RTPCL2_PCIE_CFG_RTPOSTCL2PL5_SIZE 0x0006
#define RTPCL2_PCIE_CFG_RTPRECL3PL6_LSB 0x0006
#define RTPCL2_PCIE_CFG_RTPRECL3PL6_SIZE 0x0006
#define RTPCL2_PCIE_CFG_RTPOSTCL3PL7_LSB 0x000c
#define RTPCL2_PCIE_CFG_RTPOSTCL3PL7_SIZE 0x0006
#define RTPCL2_PCIE_CFG_RTPRECL4PL8_LSB 0x0012
#define RTPCL2_PCIE_CFG_RTPRECL4PL8_SIZE 0x0006
#define RTPCL2_PCIE_CFG_RTPOSTCL4PL9_LSB 0x0018
#define RTPCL2_PCIE_CFG_RTPOSTCL4PL9_SIZE 0x0006
#define RTPCL2_PCIE_CFG_RSVD_M_LSB 0x001e
#define RTPCL2_PCIE_CFG_RSVD_M_SIZE 0x0002



/** FOMS desc:
  */

#define FOMS_PCIE_CFG_REG                (0x00000464U)

typedef union {
  struct {
    UINT32 fomsv : 24; /**< Figure of Merit Scoreboard Value */
                            /* Bits[23:0], default=0x00000000*/
    UINT32 ln : 5; /**< Lane Number */
                            /* Bits[28:24], default=0x00000000*/
    UINT32 idx : 3; /**< Index */
                            /* Bits[31:29], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FOMS_PCIE_CFG_STRUCT;

#define FOMS_PCIE_CFG_WIDTH 32
#define FOMS_PCIE_CFG_DEFAULT 0x00000000U
#define FOMS_PCIE_CFG_ENDPOINT PCIE_CFG

#define FOMS_PCIE_CFG_FOMSV_LSB 0x0000
#define FOMS_PCIE_CFG_FOMSV_SIZE 0x0018
#define FOMS_PCIE_CFG_LN_LSB 0x0018
#define FOMS_PCIE_CFG_LN_SIZE 0x0005
#define FOMS_PCIE_CFG_IDX_LSB 0x001d
#define FOMS_PCIE_CFG_IDX_SIZE 0x0003



/** HAEQ desc:
  */

#define HAEQ_PCIE_CFG_REG                (0x00000468U)

typedef union {
  struct {
    UINT32 sfomfm : 8; /**< Starting FOM For Margining */
                            /* Bits[7:0], default=0x00000000*/
    UINT32 dl : 8; /**< Dwelling Latency */
                            /* Bits[15:8], default=0x0000000E*/
    UINT32 sl : 3; /**< Speeding Latency */
                            /* Bits[18:16], default=0x00000000*/
    UINT32 macfomc : 1; /**< MAC FOM Control */
                            /* Bits[19:19], default=0x00000001*/
    UINT32 fomem : 8; /**< FOM Error Mask */
                            /* Bits[27:20], default=0x00000000*/
    UINT32 hapccpi : 4; /**< Hardware Autonomous Preset/Coefficient Count Per-Iteration */
                            /* Bits[31:28], default=0x0000000A*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} HAEQ_PCIE_CFG_STRUCT;

#define HAEQ_PCIE_CFG_WIDTH 32
#define HAEQ_PCIE_CFG_DEFAULT 0xa0080e00U
#define HAEQ_PCIE_CFG_ENDPOINT PCIE_CFG

#define HAEQ_PCIE_CFG_SFOMFM_LSB 0x0000
#define HAEQ_PCIE_CFG_SFOMFM_SIZE 0x0008
#define HAEQ_PCIE_CFG_DL_LSB 0x0008
#define HAEQ_PCIE_CFG_DL_SIZE 0x0008
#define HAEQ_PCIE_CFG_SL_LSB 0x0010
#define HAEQ_PCIE_CFG_SL_SIZE 0x0003
#define HAEQ_PCIE_CFG_MACFOMC_LSB 0x0013
#define HAEQ_PCIE_CFG_MACFOMC_SIZE 0x0001
#define HAEQ_PCIE_CFG_FOMEM_LSB 0x0014
#define HAEQ_PCIE_CFG_FOMEM_SIZE 0x0008
#define HAEQ_PCIE_CFG_HAPCCPI_LSB 0x001c
#define HAEQ_PCIE_CFG_HAPCCPI_SIZE 0x0004



/** LTCO1 desc:
  */

#define LTCO1_PCIE_CFG_REG               (0x00000470U)

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
} LTCO1_PCIE_CFG_STRUCT;

#define LTCO1_PCIE_CFG_WIDTH 32
#define LTCO1_PCIE_CFG_DEFAULT 0x00000000U
#define LTCO1_PCIE_CFG_ENDPOINT PCIE_CFG

#define LTCO1_PCIE_CFG_L0TPRECO_LSB 0x0000
#define LTCO1_PCIE_CFG_L0TPRECO_SIZE 0x0006
#define LTCO1_PCIE_CFG_L0TPOSTCO_LSB 0x0006
#define LTCO1_PCIE_CFG_L0TPOSTCO_SIZE 0x0006
#define LTCO1_PCIE_CFG_L1TPRECO_LSB 0x000c
#define LTCO1_PCIE_CFG_L1TPRECO_SIZE 0x0006
#define LTCO1_PCIE_CFG_L1TPOSTCO_LSB 0x0012
#define LTCO1_PCIE_CFG_L1TPOSTCO_SIZE 0x0006
#define LTCO1_PCIE_CFG_L0TCOE_LSB 0x0018
#define LTCO1_PCIE_CFG_L0TCOE_SIZE 0x0001
#define LTCO1_PCIE_CFG_L1TCOE_LSB 0x0019
#define LTCO1_PCIE_CFG_L1TCOE_SIZE 0x0001
#define LTCO1_PCIE_CFG_RSVD_M_LSB 0x001a
#define LTCO1_PCIE_CFG_RSVD_M_SIZE 0x0006



/** G3L0SCTL desc:
  */

#define G3L0SCTL_PCIE_CFG_REG            (0x00000478U)

typedef union {
  struct {
    UINT32 g3ccnfts : 8; /**< Gen3 Common Clock N_FTS */
                            /* Bits[7:0], default=0x0000001E*/
    UINT32 g3ucnfts : 8; /**< Gen3 Unique Clock N_FTS */
                            /* Bits[15:8], default=0x00000028*/
    UINT32 rsvd_0 : 6; /**< Undefined - auto filled rsvd_[21:16] */
                            /* Bits[21:16], default=0*/
    UINT32 g3l0sic : 2; /**< Gen3 L0s Entry Idle Control */
                            /* Bits[23:22], default=0x00000000*/
    UINT32 g3asl0spl : 8; /**< Gen3 Active State L0s Preparation Latency */
                            /* Bits[31:24], default=0x0000000C*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} G3L0SCTL_PCIE_CFG_STRUCT;

#define G3L0SCTL_PCIE_CFG_WIDTH 32
#define G3L0SCTL_PCIE_CFG_DEFAULT 0x0c00281eU
#define G3L0SCTL_PCIE_CFG_ENDPOINT PCIE_CFG

#define G3L0SCTL_PCIE_CFG_G3CCNFTS_LSB 0x0000
#define G3L0SCTL_PCIE_CFG_G3CCNFTS_SIZE 0x0008
#define G3L0SCTL_PCIE_CFG_G3UCNFTS_LSB 0x0008
#define G3L0SCTL_PCIE_CFG_G3UCNFTS_SIZE 0x0008
#define G3L0SCTL_PCIE_CFG_G3L0SIC_LSB 0x0016
#define G3L0SCTL_PCIE_CFG_G3L0SIC_SIZE 0x0002
#define G3L0SCTL_PCIE_CFG_G3ASL0SPL_LSB 0x0018
#define G3L0SCTL_PCIE_CFG_G3ASL0SPL_SIZE 0x0008



/** EQCFG2 desc:
  */

#define EQCFG2_PCIE_CFG_REG              (0x0000047cU)

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
} EQCFG2_PCIE_CFG_STRUCT;

#define EQCFG2_PCIE_CFG_WIDTH 32
#define EQCFG2_PCIE_CFG_DEFAULT 0x0000a001U
#define EQCFG2_PCIE_CFG_ENDPOINT PCIE_CFG

#define EQCFG2_PCIE_CFG_REWMET_LSB 0x0000
#define EQCFG2_PCIE_CFG_REWMET_SIZE 0x0008
#define EQCFG2_PCIE_CFG_REWMETM_LSB 0x0008
#define EQCFG2_PCIE_CFG_REWMETM_SIZE 0x0002
#define EQCFG2_PCIE_CFG_MPEME_LSB 0x000a
#define EQCFG2_PCIE_CFG_MPEME_SIZE 0x0001
#define EQCFG2_PCIE_CFG_NTEME_LSB 0x000b
#define EQCFG2_PCIE_CFG_NTEME_SIZE 0x0001
#define EQCFG2_PCIE_CFG_HAPCSB_LSB 0x000c
#define EQCFG2_PCIE_CFG_HAPCSB_SIZE 0x0004
#define EQCFG2_PCIE_CFG_PCET_LSB 0x0010
#define EQCFG2_PCIE_CFG_PCET_SIZE 0x0004
#define EQCFG2_PCIE_CFG_NTSS_LSB 0x0014
#define EQCFG2_PCIE_CFG_NTSS_SIZE 0x0003
#define EQCFG2_PCIE_CFG_EMD_LSB 0x0017
#define EQCFG2_PCIE_CFG_EMD_SIZE 0x0001
#define EQCFG2_PCIE_CFG_NTIC_LSB 0x0018
#define EQCFG2_PCIE_CFG_NTIC_SIZE 0x0008



/** MM desc:
  */

#define MM_PCIE_CFG_REG                  (0x00000480U)

typedef union {
  struct {
    UINT32 mss : 8; /**< Monitor Signal Select */
                            /* Bits[7:0], default=0x00000000*/
    UINT32 msst : 24; /**< Monitor Signal State */
                            /* Bits[31:8], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} MM_PCIE_CFG_STRUCT;

#define MM_PCIE_CFG_WIDTH 32
#define MM_PCIE_CFG_DEFAULT 0x00000000U
#define MM_PCIE_CFG_ENDPOINT PCIE_CFG

#define MM_PCIE_CFG_MSS_LSB 0x0000
#define MM_PCIE_CFG_MSS_SIZE 0x0008
#define MM_PCIE_CFG_MSST_LSB 0x0008
#define MM_PCIE_CFG_MSST_SIZE 0x0018



/** CDM desc:
  */

#define CDM_PCIE_CFG_REG                 (0x00000484U)

typedef union {
  struct {
    UINT32 mcs : 4; /**< Monitor Category Select */
                            /* Bits[3:0], default=0x00000000*/
    UINT32 mss : 4; /**< Monitor Segment Select */
                            /* Bits[7:4], default=0x00000000*/
    UINT32 rsvd_m : 24; /**< Reserved */
                            /* Bits[31:8], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CDM_PCIE_CFG_STRUCT;

#define CDM_PCIE_CFG_WIDTH 32
#define CDM_PCIE_CFG_DEFAULT 0x00000000U
#define CDM_PCIE_CFG_ENDPOINT PCIE_CFG

#define CDM_PCIE_CFG_MCS_LSB 0x0000
#define CDM_PCIE_CFG_MCS_SIZE 0x0004
#define CDM_PCIE_CFG_MSS_LSB 0x0004
#define CDM_PCIE_CFG_MSS_SIZE 0x0004
#define CDM_PCIE_CFG_RSVD_M_LSB 0x0008
#define CDM_PCIE_CFG_RSVD_M_SIZE 0x0018



/** EQCFG4 desc:
  */

#define EQCFG4_PCIE_CFG_REG              (0x0000048cU)

typedef union {
  struct {
    UINT32 px16gmflntl : 1; /**< 16GT/s Multi-Fragment Linear and Nine-Tile List Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 px16gmeqsmmflntl : 1; /**< 16.0 GT/s Multi-EQ Search Mode, Multi fragment linear and nine tile list Enable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 px16ghapccpie : 1; /**< 16.0 GT/s Hardware Autonomous Preset/Coefficient Count Per-Iteration Extention */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 px16ghapccpi : 4; /**< 16GT/s Hardware Autonomous Preset/Coefficient Count Per-Iteration */
                            /* Bits[6:3], default=0x0000000A*/
    UINT32 px16geqts2irrc : 1; /**< 16GT/s EQ TS2 in Recovery.ReceiverConfig Enable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 px16grwtneve : 4; /**< 16GT/s Receiver Wait Time For New Equalization Value Evaluation */
                            /* Bits[11:8], default=0x00000001*/
    UINT32 px16ghaed : 1; /**< 16GT/s Hardware Autonomous Equalization Done */
                            /* Bits[12:12], default=0x00000001*/
    UINT32 rsvd_0 : 2; /**< Undefined - auto filled rsvd_[14:13] */
                            /* Bits[14:13], default=0*/
    UINT32 px16grtpcoe : 1; /**< 16GT/s Remote Transmitter Preset Coefficient Override Enable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 px16grtlepceb : 1; /**< 16GT/s Remote Transmit Link Equalization Preset/Coefficient Evaluation Bypass */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 px16glep3b : 1; /**< 16.0 GT/s Link Equalization 3 Bypass */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 px16glep23b : 1; /**< 16.0 GT/s Link Equalization Phase 2 and 3 Bypass */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[19:19] */
                            /* Bits[19:19], default=0*/
    UINT32 px16greic : 1; /**< 16.0GT/s Reset EIEOS Interval Count */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 fomscp : 3; /**< FOM Scoreboard Control Policy */
                            /* Bits[23:21], default=0x00000000*/
    UINT32 px8gtswlpce : 3; /**< 8.0GT/s Training Sequence Wait Latency For Presets / Coefficients Evaluation */
                            /* Bits[26:24], default=0x00000000*/
    UINT32 px16gtswlpce : 3; /**< 16.0GT/s Training Sequence Wait Latency For Presets / Coefficients Evaluation */
                            /* Bits[29:27], default=0x00000000*/
    UINT32 seqwpeqt2p : 1; /**< Start EQ With Presets and EqTS2 Policy */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 rsvd_m : 1; /**< Reserved */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} EQCFG4_PCIE_CFG_STRUCT;

#define EQCFG4_PCIE_CFG_WIDTH 32
#define EQCFG4_PCIE_CFG_DEFAULT 0x00001150U
#define EQCFG4_PCIE_CFG_ENDPOINT PCIE_CFG

#define EQCFG4_PCIE_CFG_PX16GMFLNTL_LSB 0x0000
#define EQCFG4_PCIE_CFG_PX16GMFLNTL_SIZE 0x0001
#define EQCFG4_PCIE_CFG_PX16GMEQSMMFLNTL_LSB 0x0001
#define EQCFG4_PCIE_CFG_PX16GMEQSMMFLNTL_SIZE 0x0001
#define EQCFG4_PCIE_CFG_PX16GHAPCCPIE_LSB 0x0002
#define EQCFG4_PCIE_CFG_PX16GHAPCCPIE_SIZE 0x0001
#define EQCFG4_PCIE_CFG_PX16GHAPCCPI_LSB 0x0003
#define EQCFG4_PCIE_CFG_PX16GHAPCCPI_SIZE 0x0004
#define EQCFG4_PCIE_CFG_PX16GEQTS2IRRC_LSB 0x0007
#define EQCFG4_PCIE_CFG_PX16GEQTS2IRRC_SIZE 0x0001
#define EQCFG4_PCIE_CFG_PX16GRWTNEVE_LSB 0x0008
#define EQCFG4_PCIE_CFG_PX16GRWTNEVE_SIZE 0x0004
#define EQCFG4_PCIE_CFG_PX16GHAED_LSB 0x000c
#define EQCFG4_PCIE_CFG_PX16GHAED_SIZE 0x0001
#define EQCFG4_PCIE_CFG_PX16GRTPCOE_LSB 0x000f
#define EQCFG4_PCIE_CFG_PX16GRTPCOE_SIZE 0x0001
#define EQCFG4_PCIE_CFG_PX16GRTLEPCEB_LSB 0x0010
#define EQCFG4_PCIE_CFG_PX16GRTLEPCEB_SIZE 0x0001
#define EQCFG4_PCIE_CFG_PX16GLEP3B_LSB 0x0011
#define EQCFG4_PCIE_CFG_PX16GLEP3B_SIZE 0x0001
#define EQCFG4_PCIE_CFG_PX16GLEP23B_LSB 0x0012
#define EQCFG4_PCIE_CFG_PX16GLEP23B_SIZE 0x0001
#define EQCFG4_PCIE_CFG_PX16GREIC_LSB 0x0014
#define EQCFG4_PCIE_CFG_PX16GREIC_SIZE 0x0001
#define EQCFG4_PCIE_CFG_FOMSCP_LSB 0x0015
#define EQCFG4_PCIE_CFG_FOMSCP_SIZE 0x0003
#define EQCFG4_PCIE_CFG_PX8GTSWLPCE_LSB 0x0018
#define EQCFG4_PCIE_CFG_PX8GTSWLPCE_SIZE 0x0003
#define EQCFG4_PCIE_CFG_PX16GTSWLPCE_LSB 0x001b
#define EQCFG4_PCIE_CFG_PX16GTSWLPCE_SIZE 0x0003
#define EQCFG4_PCIE_CFG_SEQWPEQT2P_LSB 0x001e
#define EQCFG4_PCIE_CFG_SEQWPEQT2P_SIZE 0x0001
#define EQCFG4_PCIE_CFG_RSVD_M_LSB 0x001f
#define EQCFG4_PCIE_CFG_RSVD_M_SIZE 0x0001



/** CTRL2 desc:
  */

#define CTRL2_PCIE_CFG_REG               (0x000004a4U)

typedef union {
  struct {
    UINT16 rsvd_0 : 6; /**< Undefined - auto filled rsvd_[05:00] */
                            /* Bits[5:0], default=0*/
    UINT16 pmetofd : 1; /**< PMETO Timeout Fix Disable */
                            /* Bits[6:6], default=0x00000000*/
    UINT16 rsvd_1 : 7; /**< Undefined - auto filled rsvd_[13:07] */
                            /* Bits[13:7], default=0*/
    UINT16 ltlpped : 1; /**< Local TLP Prefix Error Detected */
                            /* Bits[14:14], default=0x00000000*/
    UINT16 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[15:15] */
                            /* Bits[15:15], default=0*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} CTRL2_PCIE_CFG_STRUCT;

#define CTRL2_PCIE_CFG_WIDTH 16
#define CTRL2_PCIE_CFG_DEFAULT 0x0000U
#define CTRL2_PCIE_CFG_ENDPOINT PCIE_CFG

#define CTRL2_PCIE_CFG_PMETOFD_LSB 0x0006
#define CTRL2_PCIE_CFG_PMETOFD_SIZE 0x0001
#define CTRL2_PCIE_CFG_LTLPPED_LSB 0x000e
#define CTRL2_PCIE_CFG_LTLPPED_SIZE 0x0001



/** PX16GRTPCL1 desc:
  */

#define PX16GRTPCL1_PCIE_CFG_REG         (0x000004dcU)

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
} PX16GRTPCL1_PCIE_CFG_STRUCT;

#define PX16GRTPCL1_PCIE_CFG_WIDTH 32
#define PX16GRTPCL1_PCIE_CFG_DEFAULT 0x00000000U
#define PX16GRTPCL1_PCIE_CFG_ENDPOINT PCIE_CFG

#define PX16GRTPCL1_PCIE_CFG_RTPRECL0PL0_LSB 0x0000
#define PX16GRTPCL1_PCIE_CFG_RTPRECL0PL0_SIZE 0x0006
#define PX16GRTPCL1_PCIE_CFG_RTPOSTCL0PL1_LSB 0x0006
#define PX16GRTPCL1_PCIE_CFG_RTPOSTCL0PL1_SIZE 0x0006
#define PX16GRTPCL1_PCIE_CFG_RTPRECL1PL2_LSB 0x000c
#define PX16GRTPCL1_PCIE_CFG_RTPRECL1PL2_SIZE 0x0006
#define PX16GRTPCL1_PCIE_CFG_RTPOSTCL1PL3_LSB 0x0012
#define PX16GRTPCL1_PCIE_CFG_RTPOSTCL1PL3_SIZE 0x0006
#define PX16GRTPCL1_PCIE_CFG_RTPRECL2PL4_LSB 0x0018
#define PX16GRTPCL1_PCIE_CFG_RTPRECL2PL4_SIZE 0x0006
#define PX16GRTPCL1_PCIE_CFG_PCM_LSB 0x001f
#define PX16GRTPCL1_PCIE_CFG_PCM_SIZE 0x0001



/** EQCFG5 desc:
  */

#define EQCFG5_PCIE_CFG_REG              (0x000004f8U)

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
} EQCFG5_PCIE_CFG_STRUCT;

#define EQCFG5_PCIE_CFG_WIDTH 32
#define EQCFG5_PCIE_CFG_DEFAULT 0x0000a001U
#define EQCFG5_PCIE_CFG_ENDPOINT PCIE_CFG

#define EQCFG5_PCIE_CFG_REWMET_LSB 0x0000
#define EQCFG5_PCIE_CFG_REWMET_SIZE 0x0008
#define EQCFG5_PCIE_CFG_REWMETM_LSB 0x0008
#define EQCFG5_PCIE_CFG_REWMETM_SIZE 0x0002
#define EQCFG5_PCIE_CFG_MPEME_LSB 0x000a
#define EQCFG5_PCIE_CFG_MPEME_SIZE 0x0001
#define EQCFG5_PCIE_CFG_NTEME_LSB 0x000b
#define EQCFG5_PCIE_CFG_NTEME_SIZE 0x0001
#define EQCFG5_PCIE_CFG_HAPCSB_LSB 0x000c
#define EQCFG5_PCIE_CFG_HAPCSB_SIZE 0x0004
#define EQCFG5_PCIE_CFG_PCET_LSB 0x0010
#define EQCFG5_PCIE_CFG_PCET_SIZE 0x0004
#define EQCFG5_PCIE_CFG_NTSS_LSB 0x0014
#define EQCFG5_PCIE_CFG_NTSS_SIZE 0x0003
#define EQCFG5_PCIE_CFG_EMD_LSB 0x0017
#define EQCFG5_PCIE_CFG_EMD_SIZE 0x0001
#define EQCFG5_PCIE_CFG_NTIC_LSB 0x0018
#define EQCFG5_PCIE_CFG_NTIC_SIZE 0x0008



/** L0P0P1PCM desc:
  */

#define L0P0P1PCM_PCIE_CFG_REG           (0x00000500U)

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
} L0P0P1PCM_PCIE_CFG_STRUCT;

#define L0P0P1PCM_PCIE_CFG_WIDTH 32
#define L0P0P1PCM_PCIE_CFG_DEFAULT 0x00000000U
#define L0P0P1PCM_PCIE_CFG_ENDPOINT PCIE_CFG

#define L0P0P1PCM_PCIE_CFG_P0CC_LSB 0x0000
#define L0P0P1PCM_PCIE_CFG_P0CC_SIZE 0x0006
#define L0P0P1PCM_PCIE_CFG_P0PRECC_LSB 0x0006
#define L0P0P1PCM_PCIE_CFG_P0PRECC_SIZE 0x0006
#define L0P0P1PCM_PCIE_CFG_P0PSTCC_LSB 0x000c
#define L0P0P1PCM_PCIE_CFG_P0PSTCC_SIZE 0x0006
#define L0P0P1PCM_PCIE_CFG_P1CC_LSB 0x0012
#define L0P0P1PCM_PCIE_CFG_P1CC_SIZE 0x0006
#define L0P0P1PCM_PCIE_CFG_P1PRECC_LSB 0x0018
#define L0P0P1PCM_PCIE_CFG_P1PRECC_SIZE 0x0006
#define L0P0P1PCM_PCIE_CFG_RSVD_M_LSB 0x001e
#define L0P0P1PCM_PCIE_CFG_RSVD_M_SIZE 0x0002



/** L0P1P2P3PCM desc:
  */

#define L0P1P2P3PCM_PCIE_CFG_REG         (0x00000504U)

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
} L0P1P2P3PCM_PCIE_CFG_STRUCT;

#define L0P1P2P3PCM_PCIE_CFG_WIDTH 32
#define L0P1P2P3PCM_PCIE_CFG_DEFAULT 0x00000000U
#define L0P1P2P3PCM_PCIE_CFG_ENDPOINT PCIE_CFG

#define L0P1P2P3PCM_PCIE_CFG_P1PSTCC_LSB 0x0000
#define L0P1P2P3PCM_PCIE_CFG_P1PSTCC_SIZE 0x0006
#define L0P1P2P3PCM_PCIE_CFG_P2CC_LSB 0x0006
#define L0P1P2P3PCM_PCIE_CFG_P2CC_SIZE 0x0006
#define L0P1P2P3PCM_PCIE_CFG_P2PRECC_LSB 0x000c
#define L0P1P2P3PCM_PCIE_CFG_P2PRECC_SIZE 0x0006
#define L0P1P2P3PCM_PCIE_CFG_P2PSTCC_LSB 0x0012
#define L0P1P2P3PCM_PCIE_CFG_P2PSTCC_SIZE 0x0006
#define L0P1P2P3PCM_PCIE_CFG_P3CC_LSB 0x0018
#define L0P1P2P3PCM_PCIE_CFG_P3CC_SIZE 0x0006
#define L0P1P2P3PCM_PCIE_CFG_RSVD_M_LSB 0x001e
#define L0P1P2P3PCM_PCIE_CFG_RSVD_M_SIZE 0x0002



/** L0P3P4PCM desc:
  */

#define L0P3P4PCM_PCIE_CFG_REG           (0x00000508U)

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
} L0P3P4PCM_PCIE_CFG_STRUCT;

#define L0P3P4PCM_PCIE_CFG_WIDTH 32
#define L0P3P4PCM_PCIE_CFG_DEFAULT 0x00000000U
#define L0P3P4PCM_PCIE_CFG_ENDPOINT PCIE_CFG

#define L0P3P4PCM_PCIE_CFG_P3PRECC_LSB 0x0000
#define L0P3P4PCM_PCIE_CFG_P3PRECC_SIZE 0x0006
#define L0P3P4PCM_PCIE_CFG_P3PSTCC_LSB 0x0006
#define L0P3P4PCM_PCIE_CFG_P3PSTCC_SIZE 0x0006
#define L0P3P4PCM_PCIE_CFG_P4CC_LSB 0x000c
#define L0P3P4PCM_PCIE_CFG_P4CC_SIZE 0x0006
#define L0P3P4PCM_PCIE_CFG_P4PRECC_LSB 0x0012
#define L0P3P4PCM_PCIE_CFG_P4PRECC_SIZE 0x0006
#define L0P3P4PCM_PCIE_CFG_P4PSTCC_LSB 0x0018
#define L0P3P4PCM_PCIE_CFG_P4PSTCC_SIZE 0x0006
#define L0P3P4PCM_PCIE_CFG_RSVD_M_LSB 0x001e
#define L0P3P4PCM_PCIE_CFG_RSVD_M_SIZE 0x0002



/** L0P5P6PCM desc:
  */

#define L0P5P6PCM_PCIE_CFG_REG           (0x0000050cU)

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
} L0P5P6PCM_PCIE_CFG_STRUCT;

#define L0P5P6PCM_PCIE_CFG_WIDTH 32
#define L0P5P6PCM_PCIE_CFG_DEFAULT 0x00000000U
#define L0P5P6PCM_PCIE_CFG_ENDPOINT PCIE_CFG

#define L0P5P6PCM_PCIE_CFG_P5CC_LSB 0x0000
#define L0P5P6PCM_PCIE_CFG_P5CC_SIZE 0x0006
#define L0P5P6PCM_PCIE_CFG_P5PRECC_LSB 0x0006
#define L0P5P6PCM_PCIE_CFG_P5PRECC_SIZE 0x0006
#define L0P5P6PCM_PCIE_CFG_P5PSTCC_LSB 0x000c
#define L0P5P6PCM_PCIE_CFG_P5PSTCC_SIZE 0x0006
#define L0P5P6PCM_PCIE_CFG_P6CC_LSB 0x0012
#define L0P5P6PCM_PCIE_CFG_P6CC_SIZE 0x0006
#define L0P5P6PCM_PCIE_CFG_P6PRECC_LSB 0x0018
#define L0P5P6PCM_PCIE_CFG_P6PRECC_SIZE 0x0006
#define L0P5P6PCM_PCIE_CFG_RSVD_M_LSB 0x001e
#define L0P5P6PCM_PCIE_CFG_RSVD_M_SIZE 0x0002



/** L0P6P7P8PCM desc:
  */

#define L0P6P7P8PCM_PCIE_CFG_REG         (0x00000510U)

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
} L0P6P7P8PCM_PCIE_CFG_STRUCT;

#define L0P6P7P8PCM_PCIE_CFG_WIDTH 32
#define L0P6P7P8PCM_PCIE_CFG_DEFAULT 0x00000000U
#define L0P6P7P8PCM_PCIE_CFG_ENDPOINT PCIE_CFG

#define L0P6P7P8PCM_PCIE_CFG_P6PSTCC_LSB 0x0000
#define L0P6P7P8PCM_PCIE_CFG_P6PSTCC_SIZE 0x0006
#define L0P6P7P8PCM_PCIE_CFG_P7CC_LSB 0x0006
#define L0P6P7P8PCM_PCIE_CFG_P7CC_SIZE 0x0006
#define L0P6P7P8PCM_PCIE_CFG_P7PRECC_LSB 0x000c
#define L0P6P7P8PCM_PCIE_CFG_P7PRECC_SIZE 0x0006
#define L0P6P7P8PCM_PCIE_CFG_P7PSTCC_LSB 0x0012
#define L0P6P7P8PCM_PCIE_CFG_P7PSTCC_SIZE 0x0006
#define L0P6P7P8PCM_PCIE_CFG_P8CC_LSB 0x0018
#define L0P6P7P8PCM_PCIE_CFG_P8CC_SIZE 0x0006
#define L0P6P7P8PCM_PCIE_CFG_RSVD_M_LSB 0x001e
#define L0P6P7P8PCM_PCIE_CFG_RSVD_M_SIZE 0x0002



/** L0P8P9PCM desc:
  */

#define L0P8P9PCM_PCIE_CFG_REG           (0x00000514U)

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
} L0P8P9PCM_PCIE_CFG_STRUCT;

#define L0P8P9PCM_PCIE_CFG_WIDTH 32
#define L0P8P9PCM_PCIE_CFG_DEFAULT 0x00000000U
#define L0P8P9PCM_PCIE_CFG_ENDPOINT PCIE_CFG

#define L0P8P9PCM_PCIE_CFG_P8PRECC_LSB 0x0000
#define L0P8P9PCM_PCIE_CFG_P8PRECC_SIZE 0x0006
#define L0P8P9PCM_PCIE_CFG_P8PSTCC_LSB 0x0006
#define L0P8P9PCM_PCIE_CFG_P8PSTCC_SIZE 0x0006
#define L0P8P9PCM_PCIE_CFG_P9CC_LSB 0x000c
#define L0P8P9PCM_PCIE_CFG_P9CC_SIZE 0x0006
#define L0P8P9PCM_PCIE_CFG_P9PRECC_LSB 0x0012
#define L0P8P9PCM_PCIE_CFG_P9PRECC_SIZE 0x0006
#define L0P8P9PCM_PCIE_CFG_P9PSTCC_LSB 0x0018
#define L0P8P9PCM_PCIE_CFG_P9PSTCC_SIZE 0x0006
#define L0P8P9PCM_PCIE_CFG_RSVD_M_LSB 0x001e
#define L0P8P9PCM_PCIE_CFG_RSVD_M_SIZE 0x0002



/** L0P10PCM desc:
  */

#define L0P10PCM_PCIE_CFG_REG            (0x00000518U)

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
} L0P10PCM_PCIE_CFG_STRUCT;

#define L0P10PCM_PCIE_CFG_WIDTH 32
#define L0P10PCM_PCIE_CFG_DEFAULT 0x00000000U
#define L0P10PCM_PCIE_CFG_ENDPOINT PCIE_CFG

#define L0P10PCM_PCIE_CFG_P10CC_LSB 0x0000
#define L0P10PCM_PCIE_CFG_P10CC_SIZE 0x0006
#define L0P10PCM_PCIE_CFG_P10PRECC_LSB 0x0006
#define L0P10PCM_PCIE_CFG_P10PRECC_SIZE 0x0006
#define L0P10PCM_PCIE_CFG_P10PSTCC_LSB 0x000c
#define L0P10PCM_PCIE_CFG_P10PSTCC_SIZE 0x0006
#define L0P10PCM_PCIE_CFG_RSVD_M_LSB 0x0012
#define L0P10PCM_PCIE_CFG_RSVD_M_SIZE 0x000e



/** L0LFFS desc:
  */

#define L0LFFS_PCIE_CFG_REG              (0x0000051cU)

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
    UINT32 rsvd_3 : 2; /**< Undefined - auto filled rsvd_[31:30] */
                            /* Bits[31:30], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} L0LFFS_PCIE_CFG_STRUCT;

#define L0LFFS_PCIE_CFG_WIDTH 32
#define L0LFFS_PCIE_CFG_DEFAULT 0x00000000U
#define L0LFFS_PCIE_CFG_ENDPOINT PCIE_CFG

#define L0LFFS_PCIE_CFG_LPLFV_LSB 0x0000
#define L0LFFS_PCIE_CFG_LPLFV_SIZE 0x0006
#define L0LFFS_PCIE_CFG_LPFSV_LSB 0x0008
#define L0LFFS_PCIE_CFG_LPFSV_SIZE 0x0006
#define L0LFFS_PCIE_CFG_RPLFV_LSB 0x0010
#define L0LFFS_PCIE_CFG_RPLFV_SIZE 0x0006
#define L0LFFS_PCIE_CFG_RPFSV_LSB 0x0018
#define L0LFFS_PCIE_CFG_RPFSV_SIZE 0x0006



/** PX16GP0P1PCM desc:
  */

#define PX16GP0P1PCM_PCIE_CFG_REG        (0x00000520U)

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
} PX16GP0P1PCM_PCIE_CFG_STRUCT;

#define PX16GP0P1PCM_PCIE_CFG_WIDTH 32
#define PX16GP0P1PCM_PCIE_CFG_DEFAULT 0x00000000U
#define PX16GP0P1PCM_PCIE_CFG_ENDPOINT PCIE_CFG

#define PX16GP0P1PCM_PCIE_CFG_P0CC_LSB 0x0000
#define PX16GP0P1PCM_PCIE_CFG_P0CC_SIZE 0x0006
#define PX16GP0P1PCM_PCIE_CFG_P0PRECC_LSB 0x0006
#define PX16GP0P1PCM_PCIE_CFG_P0PRECC_SIZE 0x0006
#define PX16GP0P1PCM_PCIE_CFG_P0PSTCC_LSB 0x000c
#define PX16GP0P1PCM_PCIE_CFG_P0PSTCC_SIZE 0x0006
#define PX16GP0P1PCM_PCIE_CFG_P1CC_LSB 0x0012
#define PX16GP0P1PCM_PCIE_CFG_P1CC_SIZE 0x0006
#define PX16GP0P1PCM_PCIE_CFG_P1PRECC_LSB 0x0018
#define PX16GP0P1PCM_PCIE_CFG_P1PRECC_SIZE 0x0006
#define PX16GP0P1PCM_PCIE_CFG_RSVD_M_LSB 0x001e
#define PX16GP0P1PCM_PCIE_CFG_RSVD_M_SIZE 0x0002



/** PX16GP1P2P3PCM desc:
  */

#define PX16GP1P2P3PCM_PCIE_CFG_REG      (0x00000524U)

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
} PX16GP1P2P3PCM_PCIE_CFG_STRUCT;

#define PX16GP1P2P3PCM_PCIE_CFG_WIDTH 32
#define PX16GP1P2P3PCM_PCIE_CFG_DEFAULT 0x00000000U
#define PX16GP1P2P3PCM_PCIE_CFG_ENDPOINT PCIE_CFG

#define PX16GP1P2P3PCM_PCIE_CFG_P1PSTCC_LSB 0x0000
#define PX16GP1P2P3PCM_PCIE_CFG_P1PSTCC_SIZE 0x0006
#define PX16GP1P2P3PCM_PCIE_CFG_P2CC_LSB 0x0006
#define PX16GP1P2P3PCM_PCIE_CFG_P2CC_SIZE 0x0006
#define PX16GP1P2P3PCM_PCIE_CFG_P2PRECC_LSB 0x000c
#define PX16GP1P2P3PCM_PCIE_CFG_P2PRECC_SIZE 0x0006
#define PX16GP1P2P3PCM_PCIE_CFG_P2PSTCC_LSB 0x0012
#define PX16GP1P2P3PCM_PCIE_CFG_P2PSTCC_SIZE 0x0006
#define PX16GP1P2P3PCM_PCIE_CFG_P3CC_LSB 0x0018
#define PX16GP1P2P3PCM_PCIE_CFG_P3CC_SIZE 0x0006
#define PX16GP1P2P3PCM_PCIE_CFG_RSVD_M_LSB 0x001e
#define PX16GP1P2P3PCM_PCIE_CFG_RSVD_M_SIZE 0x0002



/** PX16GP3P4PCM desc:
  */

#define PX16GP3P4PCM_PCIE_CFG_REG        (0x00000528U)

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
} PX16GP3P4PCM_PCIE_CFG_STRUCT;

#define PX16GP3P4PCM_PCIE_CFG_WIDTH 32
#define PX16GP3P4PCM_PCIE_CFG_DEFAULT 0x00000000U
#define PX16GP3P4PCM_PCIE_CFG_ENDPOINT PCIE_CFG

#define PX16GP3P4PCM_PCIE_CFG_P3PRECC_LSB 0x0000
#define PX16GP3P4PCM_PCIE_CFG_P3PRECC_SIZE 0x0006
#define PX16GP3P4PCM_PCIE_CFG_P3PSTCC_LSB 0x0006
#define PX16GP3P4PCM_PCIE_CFG_P3PSTCC_SIZE 0x0006
#define PX16GP3P4PCM_PCIE_CFG_P4CC_LSB 0x000c
#define PX16GP3P4PCM_PCIE_CFG_P4CC_SIZE 0x0006
#define PX16GP3P4PCM_PCIE_CFG_P4PRECC_LSB 0x0012
#define PX16GP3P4PCM_PCIE_CFG_P4PRECC_SIZE 0x0006
#define PX16GP3P4PCM_PCIE_CFG_P4PSTCC_LSB 0x0018
#define PX16GP3P4PCM_PCIE_CFG_P4PSTCC_SIZE 0x0006
#define PX16GP3P4PCM_PCIE_CFG_RSVD_M_LSB 0x001e
#define PX16GP3P4PCM_PCIE_CFG_RSVD_M_SIZE 0x0002



/** PX16GP5P6PCM desc:
  */

#define PX16GP5P6PCM_PCIE_CFG_REG        (0x0000052cU)

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
} PX16GP5P6PCM_PCIE_CFG_STRUCT;

#define PX16GP5P6PCM_PCIE_CFG_WIDTH 32
#define PX16GP5P6PCM_PCIE_CFG_DEFAULT 0x00000000U
#define PX16GP5P6PCM_PCIE_CFG_ENDPOINT PCIE_CFG

#define PX16GP5P6PCM_PCIE_CFG_P5CC_LSB 0x0000
#define PX16GP5P6PCM_PCIE_CFG_P5CC_SIZE 0x0006
#define PX16GP5P6PCM_PCIE_CFG_P5PRECC_LSB 0x0006
#define PX16GP5P6PCM_PCIE_CFG_P5PRECC_SIZE 0x0006
#define PX16GP5P6PCM_PCIE_CFG_P5PSTCC_LSB 0x000c
#define PX16GP5P6PCM_PCIE_CFG_P5PSTCC_SIZE 0x0006
#define PX16GP5P6PCM_PCIE_CFG_P6CC_LSB 0x0012
#define PX16GP5P6PCM_PCIE_CFG_P6CC_SIZE 0x0006
#define PX16GP5P6PCM_PCIE_CFG_P6PRECC_LSB 0x0018
#define PX16GP5P6PCM_PCIE_CFG_P6PRECC_SIZE 0x0006
#define PX16GP5P6PCM_PCIE_CFG_RSVD_M_LSB 0x001e
#define PX16GP5P6PCM_PCIE_CFG_RSVD_M_SIZE 0x0002



/** PX16GP6P7P8PCM desc:
  */

#define PX16GP6P7P8PCM_PCIE_CFG_REG      (0x00000530U)

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
} PX16GP6P7P8PCM_PCIE_CFG_STRUCT;

#define PX16GP6P7P8PCM_PCIE_CFG_WIDTH 32
#define PX16GP6P7P8PCM_PCIE_CFG_DEFAULT 0x00000000U
#define PX16GP6P7P8PCM_PCIE_CFG_ENDPOINT PCIE_CFG

#define PX16GP6P7P8PCM_PCIE_CFG_P6PSTCC_LSB 0x0000
#define PX16GP6P7P8PCM_PCIE_CFG_P6PSTCC_SIZE 0x0006
#define PX16GP6P7P8PCM_PCIE_CFG_P7CC_LSB 0x0006
#define PX16GP6P7P8PCM_PCIE_CFG_P7CC_SIZE 0x0006
#define PX16GP6P7P8PCM_PCIE_CFG_P7PRECC_LSB 0x000c
#define PX16GP6P7P8PCM_PCIE_CFG_P7PRECC_SIZE 0x0006
#define PX16GP6P7P8PCM_PCIE_CFG_P7PSTCC_LSB 0x0012
#define PX16GP6P7P8PCM_PCIE_CFG_P7PSTCC_SIZE 0x0006
#define PX16GP6P7P8PCM_PCIE_CFG_P8CC_LSB 0x0018
#define PX16GP6P7P8PCM_PCIE_CFG_P8CC_SIZE 0x0006
#define PX16GP6P7P8PCM_PCIE_CFG_RSVD_M_LSB 0x001e
#define PX16GP6P7P8PCM_PCIE_CFG_RSVD_M_SIZE 0x0002



/** PX16GP8P9PCM desc:
  */

#define PX16GP8P9PCM_PCIE_CFG_REG        (0x00000534U)

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
} PX16GP8P9PCM_PCIE_CFG_STRUCT;

#define PX16GP8P9PCM_PCIE_CFG_WIDTH 32
#define PX16GP8P9PCM_PCIE_CFG_DEFAULT 0x00000000U
#define PX16GP8P9PCM_PCIE_CFG_ENDPOINT PCIE_CFG

#define PX16GP8P9PCM_PCIE_CFG_P8PRECC_LSB 0x0000
#define PX16GP8P9PCM_PCIE_CFG_P8PRECC_SIZE 0x0006
#define PX16GP8P9PCM_PCIE_CFG_P8PSTCC_LSB 0x0006
#define PX16GP8P9PCM_PCIE_CFG_P8PSTCC_SIZE 0x0006
#define PX16GP8P9PCM_PCIE_CFG_P9CC_LSB 0x000c
#define PX16GP8P9PCM_PCIE_CFG_P9CC_SIZE 0x0006
#define PX16GP8P9PCM_PCIE_CFG_P9PRECC_LSB 0x0012
#define PX16GP8P9PCM_PCIE_CFG_P9PRECC_SIZE 0x0006
#define PX16GP8P9PCM_PCIE_CFG_P9PSTCC_LSB 0x0018
#define PX16GP8P9PCM_PCIE_CFG_P9PSTCC_SIZE 0x0006
#define PX16GP8P9PCM_PCIE_CFG_RSVD_M_LSB 0x001e
#define PX16GP8P9PCM_PCIE_CFG_RSVD_M_SIZE 0x0002



/** PX16GP10PCM desc:
  */

#define PX16GP10PCM_PCIE_CFG_REG         (0x00000538U)

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
} PX16GP10PCM_PCIE_CFG_STRUCT;

#define PX16GP10PCM_PCIE_CFG_WIDTH 32
#define PX16GP10PCM_PCIE_CFG_DEFAULT 0x00000000U
#define PX16GP10PCM_PCIE_CFG_ENDPOINT PCIE_CFG

#define PX16GP10PCM_PCIE_CFG_P10CC_LSB 0x0000
#define PX16GP10PCM_PCIE_CFG_P10CC_SIZE 0x0006
#define PX16GP10PCM_PCIE_CFG_P10PRECC_LSB 0x0006
#define PX16GP10PCM_PCIE_CFG_P10PRECC_SIZE 0x0006
#define PX16GP10PCM_PCIE_CFG_P10PSTCC_LSB 0x000c
#define PX16GP10PCM_PCIE_CFG_P10PSTCC_SIZE 0x0006
#define PX16GP10PCM_PCIE_CFG_RSVD_M_LSB 0x0012
#define PX16GP10PCM_PCIE_CFG_RSVD_M_SIZE 0x000e



/** PX16GLFFS desc:
  */

#define PX16GLFFS_PCIE_CFG_REG           (0x0000053cU)

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
} PX16GLFFS_PCIE_CFG_STRUCT;

#define PX16GLFFS_PCIE_CFG_WIDTH 32
#define PX16GLFFS_PCIE_CFG_DEFAULT 0x00000000U
#define PX16GLFFS_PCIE_CFG_ENDPOINT PCIE_CFG

#define PX16GLFFS_PCIE_CFG_LPLFV_LSB 0x0000
#define PX16GLFFS_PCIE_CFG_LPLFV_SIZE 0x0006
#define PX16GLFFS_PCIE_CFG_LPFSV_LSB 0x0008
#define PX16GLFFS_PCIE_CFG_LPFSV_SIZE 0x0006
#define PX16GLFFS_PCIE_CFG_RPLFV_LSB 0x0010
#define PX16GLFFS_PCIE_CFG_RPLFV_SIZE 0x0006
#define PX16GLFFS_PCIE_CFG_RPFSV_LSB 0x0018
#define PX16GLFFS_PCIE_CFG_RPFSV_SIZE 0x0006
#define PX16GLFFS_PCIE_CFG_RSVD_M_LSB 0x001e
#define PX16GLFFS_PCIE_CFG_RSVD_M_SIZE 0x0002



/** COCTL desc:
  */

#define COCTL_PCIE_CFG_REG               (0x00000594U)

typedef union {
  struct {
    UINT32 pwce : 1; /**< Posted Write Chaining Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 ddce : 1; /**< Completion Chaining Enable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 ct : 8; /**< Chain Timer */
                            /* Bits[9:2], default=0x00000000*/
    UINT32 cte : 1; /**< Chain Timer Enable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 roaop : 1; /**< Relaxed Order Attribute Override Policy */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 magparcd : 1; /**< Maximum Addressable GPA Range Check Disable */
                            /* Bits[12:12], default=0x00000001*/
    UINT32 pclm : 2; /**< Posted Chain Limit Mode */
                            /* Bits[14:13], default=0x00000000*/
    UINT32 npclm : 2; /**< Non-Posted Chain Limit Mode */
                            /* Bits[16:15], default=0x00000000*/
    UINT32 ccsm : 1; /**< Coalescing Chain select mode */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 chainbarbe : 1; /**< Chain Bit Arbitration Enable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 rsvd_m : 13; /**< Reserved */
                            /* Bits[31:19], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} COCTL_PCIE_CFG_STRUCT;

#define COCTL_PCIE_CFG_WIDTH 32
#define COCTL_PCIE_CFG_DEFAULT 0x00001000U
#define COCTL_PCIE_CFG_ENDPOINT PCIE_CFG

#define COCTL_PCIE_CFG_PWCE_LSB 0x0000
#define COCTL_PCIE_CFG_PWCE_SIZE 0x0001
#define COCTL_PCIE_CFG_DDCE_LSB 0x0001
#define COCTL_PCIE_CFG_DDCE_SIZE 0x0001
#define COCTL_PCIE_CFG_CT_LSB 0x0002
#define COCTL_PCIE_CFG_CT_SIZE 0x0008
#define COCTL_PCIE_CFG_CTE_LSB 0x000a
#define COCTL_PCIE_CFG_CTE_SIZE 0x0001
#define COCTL_PCIE_CFG_ROAOP_LSB 0x000b
#define COCTL_PCIE_CFG_ROAOP_SIZE 0x0001
#define COCTL_PCIE_CFG_MAGPARCD_LSB 0x000c
#define COCTL_PCIE_CFG_MAGPARCD_SIZE 0x0001
#define COCTL_PCIE_CFG_PCLM_LSB 0x000d
#define COCTL_PCIE_CFG_PCLM_SIZE 0x0002
#define COCTL_PCIE_CFG_NPCLM_LSB 0x000f
#define COCTL_PCIE_CFG_NPCLM_SIZE 0x0002
#define COCTL_PCIE_CFG_CCSM_LSB 0x0011
#define COCTL_PCIE_CFG_CCSM_SIZE 0x0001
#define COCTL_PCIE_CFG_CHAINBARBE_LSB 0x0012
#define COCTL_PCIE_CFG_CHAINBARBE_SIZE 0x0001
#define COCTL_PCIE_CFG_RSVD_M_LSB 0x0013
#define COCTL_PCIE_CFG_RSVD_M_SIZE 0x000d



/** ADVMCTRL desc:
  */

#define ADVMCTRL_PCIE_CFG_REG            (0x000005bcU)

typedef union {
  struct {
    UINT32 pl1mit : 3; /**< Prolong L1 Min Idle Timer */
                            /* Bits[2:0], default=0x00000000*/
    UINT32 rtd3perst : 1; /**< RTD3 PERST */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 rtd3pfetdis : 1; /**< RTD3 PFET Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 pmreqblkpgrspt : 3; /**< PM_REQ Block and Power Gate Response Time */
                            /* Bits[7:5], default=0x00000000*/
    UINT32 lmeqce : 1; /**< Loopback Master EQ Change Enable */
                            /* Bits[8:8], default=0x00000001*/
    UINT32 lmeqts1e : 1; /**< Loopback Master EQ TS1 Enable */
                            /* Bits[9:9], default=0x00000001*/
    UINT32 rllg12r : 1; /**< Reset Link Layer in Gen1, 2 Recovery */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 rrllcl : 1; /**< Reset Receiver Link Layer Common Logic */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 stlprs : 1; /**< Short TLP Reporting Status */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 g3stfer : 1; /**< Gen3 Short TLP Framing Error Reporting */
                            /* Bits[13:13], default=0x00000001*/
    UINT32 clkreqsm : 1; /**< ClkReq Serialization Mode */
                            /* Bits[14:14], default=0x00000001*/
    UINT32 rxl0dc : 1; /**< RxL0 De-Assertion Control */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 pmreqcwc : 3; /**< PM_REQ Clock Wake Control */
                            /* Bits[18:16], default=0x00000000*/
    UINT32 eiosmaskrx : 1; /**< EIOS Mask Receiver Datapath */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 eiosdisds : 1; /**< EIOS Disable DeSkew */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[21:21] */
                            /* Bits[21:21], default=0*/
    UINT32 inrxl0ctrl : 1; /**< InRxL0 Control */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 ccbe : 1; /**< Completion Coalescing Break Event */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 f10btse : 1; /**< Fabric 10-bit Tag Support Enable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 i8gteqts2tpv : 1; /**< Ignore 8GT_EQ_TS2 Transmitter Preset Values */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 cttvs : 1; /**< Completion Timer Timeout Value Select */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 rsvd_rw2 : 5; /**< Reserved */
                            /* Bits[31:27], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} ADVMCTRL_PCIE_CFG_STRUCT;

#define ADVMCTRL_PCIE_CFG_WIDTH 32
#define ADVMCTRL_PCIE_CFG_DEFAULT 0x00006300U
#define ADVMCTRL_PCIE_CFG_ENDPOINT PCIE_CFG

#define ADVMCTRL_PCIE_CFG_PL1MIT_LSB 0x0000
#define ADVMCTRL_PCIE_CFG_PL1MIT_SIZE 0x0003
#define ADVMCTRL_PCIE_CFG_RTD3PERST_LSB 0x0003
#define ADVMCTRL_PCIE_CFG_RTD3PERST_SIZE 0x0001
#define ADVMCTRL_PCIE_CFG_RTD3PFETDIS_LSB 0x0004
#define ADVMCTRL_PCIE_CFG_RTD3PFETDIS_SIZE 0x0001
#define ADVMCTRL_PCIE_CFG_PMREQBLKPGRSPT_LSB 0x0005
#define ADVMCTRL_PCIE_CFG_PMREQBLKPGRSPT_SIZE 0x0003
#define ADVMCTRL_PCIE_CFG_LMEQCE_LSB 0x0008
#define ADVMCTRL_PCIE_CFG_LMEQCE_SIZE 0x0001
#define ADVMCTRL_PCIE_CFG_LMEQTS1E_LSB 0x0009
#define ADVMCTRL_PCIE_CFG_LMEQTS1E_SIZE 0x0001
#define ADVMCTRL_PCIE_CFG_RLLG12R_LSB 0x000a
#define ADVMCTRL_PCIE_CFG_RLLG12R_SIZE 0x0001
#define ADVMCTRL_PCIE_CFG_RRLLCL_LSB 0x000b
#define ADVMCTRL_PCIE_CFG_RRLLCL_SIZE 0x0001
#define ADVMCTRL_PCIE_CFG_STLPRS_LSB 0x000c
#define ADVMCTRL_PCIE_CFG_STLPRS_SIZE 0x0001
#define ADVMCTRL_PCIE_CFG_G3STFER_LSB 0x000d
#define ADVMCTRL_PCIE_CFG_G3STFER_SIZE 0x0001
#define ADVMCTRL_PCIE_CFG_CLKREQSM_LSB 0x000e
#define ADVMCTRL_PCIE_CFG_CLKREQSM_SIZE 0x0001
#define ADVMCTRL_PCIE_CFG_RXL0DC_LSB 0x000f
#define ADVMCTRL_PCIE_CFG_RXL0DC_SIZE 0x0001
#define ADVMCTRL_PCIE_CFG_PMREQCWC_LSB 0x0010
#define ADVMCTRL_PCIE_CFG_PMREQCWC_SIZE 0x0003
#define ADVMCTRL_PCIE_CFG_EIOSMASKRX_LSB 0x0013
#define ADVMCTRL_PCIE_CFG_EIOSMASKRX_SIZE 0x0001
#define ADVMCTRL_PCIE_CFG_EIOSDISDS_LSB 0x0014
#define ADVMCTRL_PCIE_CFG_EIOSDISDS_SIZE 0x0001
#define ADVMCTRL_PCIE_CFG_INRXL0CTRL_LSB 0x0016
#define ADVMCTRL_PCIE_CFG_INRXL0CTRL_SIZE 0x0001
#define ADVMCTRL_PCIE_CFG_CCBE_LSB 0x0017
#define ADVMCTRL_PCIE_CFG_CCBE_SIZE 0x0001
#define ADVMCTRL_PCIE_CFG_F10BTSE_LSB 0x0018
#define ADVMCTRL_PCIE_CFG_F10BTSE_SIZE 0x0001
#define ADVMCTRL_PCIE_CFG_I8GTEQTS2TPV_LSB 0x0019
#define ADVMCTRL_PCIE_CFG_I8GTEQTS2TPV_SIZE 0x0001
#define ADVMCTRL_PCIE_CFG_CTTVS_LSB 0x001a
#define ADVMCTRL_PCIE_CFG_CTTVS_SIZE 0x0001
#define ADVMCTRL_PCIE_CFG_RSVD_RW2_LSB 0x001b
#define ADVMCTRL_PCIE_CFG_RSVD_RW2_SIZE 0x0005



/** PGTHRES desc:
  */

#define PGTHRES_PCIE_CFG_REG             (0x000005c0U)

typedef union {
  struct {
    UINT32 l1pgltren : 1; /**< L1 Power Gating LTR Enable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 15; /**< Undefined - auto filled rsvd_[15:01] */
                            /* Bits[15:1], default=0*/
    UINT32 l1pgltrtlv : 10; /**< L1 Power Gating LTR Threshold Latency Value */
                            /* Bits[25:16], default=0x00000000*/
    UINT32 rsvd_1 : 3; /**< Undefined - auto filled rsvd_[28:26] */
                            /* Bits[28:26], default=0*/
    UINT32 l1pgltrtlsv : 3; /**< L1 Power Gating LTR Threshold Latency Scale Value */
                            /* Bits[31:29], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PGTHRES_PCIE_CFG_STRUCT;

#define PGTHRES_PCIE_CFG_WIDTH 32
#define PGTHRES_PCIE_CFG_DEFAULT 0x00000000U
#define PGTHRES_PCIE_CFG_ENDPOINT PCIE_CFG

#define PGTHRES_PCIE_CFG_L1PGLTREN_LSB 0x0000
#define PGTHRES_PCIE_CFG_L1PGLTREN_SIZE 0x0001
#define PGTHRES_PCIE_CFG_L1PGLTRTLV_LSB 0x0010
#define PGTHRES_PCIE_CFG_L1PGLTRTLV_SIZE 0x000a
#define PGTHRES_PCIE_CFG_L1PGLTRTLSV_LSB 0x001d
#define PGTHRES_PCIE_CFG_L1PGLTRTLSV_SIZE 0x0003



/** HWSNR desc:
  */

#define HWSNR_PCIE_CFG_REG               (0x000005f0U)

typedef union {
  struct {
    UINT32 bepw : 4; /**< Bank Enable Pulse Width */
                            /* Bits[3:0], default=0x00000007*/
    UINT32 repw : 4; /**< Restore Enable Pulse Width */
                            /* Bits[7:4], default=0x00000001*/
    UINT32 eeh : 2; /**< Entry and Exit Hysteresis */
                            /* Bits[9:8], default=0x00000002*/
    UINT32 ready4pg : 1; /**< Ready for Power Gating */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 rsvd_m : 21; /**< Reserved */
                            /* Bits[31:11], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} HWSNR_PCIE_CFG_STRUCT;

#define HWSNR_PCIE_CFG_WIDTH 32
#define HWSNR_PCIE_CFG_DEFAULT 0x00000217U
#define HWSNR_PCIE_CFG_ENDPOINT PCIE_CFG

#define HWSNR_PCIE_CFG_BEPW_LSB 0x0000
#define HWSNR_PCIE_CFG_BEPW_SIZE 0x0004
#define HWSNR_PCIE_CFG_REPW_LSB 0x0004
#define HWSNR_PCIE_CFG_REPW_SIZE 0x0004
#define HWSNR_PCIE_CFG_EEH_LSB 0x0008
#define HWSNR_PCIE_CFG_EEH_SIZE 0x0002
#define HWSNR_PCIE_CFG_READY4PG_LSB 0x000a
#define HWSNR_PCIE_CFG_READY4PG_SIZE 0x0001
#define HWSNR_PCIE_CFG_RSVD_M_LSB 0x000b
#define HWSNR_PCIE_CFG_RSVD_M_SIZE 0x0015



/** PGCTRL desc:
  */

#define PGCTRL_PCIE_CFG_REG              (0x000005f4U)

typedef union {
  struct {
    UINT32 pmreqblkrspt : 3; /**< PM_REQ Block Response Time */
                            /* Bits[2:0], default=0x00000000*/
    UINT32 pmreqblkrsptm : 2; /**< PM_REQ Block Response Time Multiplier */
                            /* Bits[4:3], default=0x00000000*/
    UINT32 rsvd_m : 27; /**< Reserved */
                            /* Bits[31:5], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PGCTRL_PCIE_CFG_STRUCT;

#define PGCTRL_PCIE_CFG_WIDTH 32
#define PGCTRL_PCIE_CFG_DEFAULT 0x00000000U
#define PGCTRL_PCIE_CFG_ENDPOINT PCIE_CFG

#define PGCTRL_PCIE_CFG_PMREQBLKRSPT_LSB 0x0000
#define PGCTRL_PCIE_CFG_PMREQBLKRSPT_SIZE 0x0003
#define PGCTRL_PCIE_CFG_PMREQBLKRSPTM_LSB 0x0003
#define PGCTRL_PCIE_CFG_PMREQBLKRSPTM_SIZE 0x0002
#define PGCTRL_PCIE_CFG_RSVD_M_LSB 0x0005
#define PGCTRL_PCIE_CFG_RSVD_M_SIZE 0x001b



/** PX16GLTCO1 desc:
  */

#define PX16GLTCO1_PCIE_CFG_REG          (0x00000600U)

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
} PX16GLTCO1_PCIE_CFG_STRUCT;

#define PX16GLTCO1_PCIE_CFG_WIDTH 32
#define PX16GLTCO1_PCIE_CFG_DEFAULT 0x00000000U
#define PX16GLTCO1_PCIE_CFG_ENDPOINT PCIE_CFG

#define PX16GLTCO1_PCIE_CFG_L0TPRECO_LSB 0x0000
#define PX16GLTCO1_PCIE_CFG_L0TPRECO_SIZE 0x0006
#define PX16GLTCO1_PCIE_CFG_L0TPOSTCO_LSB 0x0006
#define PX16GLTCO1_PCIE_CFG_L0TPOSTCO_SIZE 0x0006
#define PX16GLTCO1_PCIE_CFG_L1TPRECO_LSB 0x000c
#define PX16GLTCO1_PCIE_CFG_L1TPRECO_SIZE 0x0006
#define PX16GLTCO1_PCIE_CFG_L1TPOSTCO_LSB 0x0012
#define PX16GLTCO1_PCIE_CFG_L1TPOSTCO_SIZE 0x0006
#define PX16GLTCO1_PCIE_CFG_L0TCOE_LSB 0x0018
#define PX16GLTCO1_PCIE_CFG_L0TCOE_SIZE 0x0001
#define PX16GLTCO1_PCIE_CFG_L1TCOE_LSB 0x0019
#define PX16GLTCO1_PCIE_CFG_L1TCOE_SIZE 0x0001
#define PX16GLTCO1_PCIE_CFG_RSVD_M_LSB 0x001a
#define PX16GLTCO1_PCIE_CFG_RSVD_M_SIZE 0x0006



/** GDR desc:
  */

#define GDR_PCIE_CFG_REG                 (0x00000690U)

typedef union {
  struct {
    UINT32 rsvd_0 : 10; /**< Undefined - auto filled rsvd_[09:00] */
                            /* Bits[9:0], default=0*/
    UINT32 gpioald : 1; /**< GPIO Assertion on Link Down */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 vgpioalde : 1; /**< Virtual GPIO Assertion on Link Down Enable */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 ldvwmidx : 8; /**< Link Down Virtual Wire Message Index */
                            /* Bits[19:12], default=0x00000000*/
    UINT32 ldvwmbitloc : 3; /**< Link Down Virtual Wire Message Bit Location */
                            /* Bits[22:20], default=0x00000000*/
    UINT32 ldvwmdestid : 8; /**< Link Down Virtual Wire Message Destination ID */
                            /* Bits[30:23], default=0x00000000*/
    UINT32 rsvd_m : 1; /**< Reserved */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GDR_PCIE_CFG_STRUCT;

#define GDR_PCIE_CFG_WIDTH 32
#define GDR_PCIE_CFG_DEFAULT 0x00000000U
#define GDR_PCIE_CFG_ENDPOINT PCIE_CFG

#define GDR_PCIE_CFG_GPIOALD_LSB 0x000a
#define GDR_PCIE_CFG_GPIOALD_SIZE 0x0001
#define GDR_PCIE_CFG_VGPIOALDE_LSB 0x000b
#define GDR_PCIE_CFG_VGPIOALDE_SIZE 0x0001
#define GDR_PCIE_CFG_LDVWMIDX_LSB 0x000c
#define GDR_PCIE_CFG_LDVWMIDX_SIZE 0x0008
#define GDR_PCIE_CFG_LDVWMBITLOC_LSB 0x0014
#define GDR_PCIE_CFG_LDVWMBITLOC_SIZE 0x0003
#define GDR_PCIE_CFG_LDVWMDESTID_LSB 0x0017
#define GDR_PCIE_CFG_LDVWMDESTID_SIZE 0x0008
#define GDR_PCIE_CFG_RSVD_M_LSB 0x001f
#define GDR_PCIE_CFG_RSVD_M_SIZE 0x0001



/** ACRG3 desc:
  */

#define ACRG3_PCIE_CFG_REG               (0x000006ccU)

typedef union {
  struct {
    UINT32 uphlrcfg : 1; /**< Upconfiguration Hold Link in Recovery.Cfg */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 blkrxdsa : 1; /**< Block RxData Stream Assertion in Recovery.Cfg/Idle */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 ltsmstateosc : 5; /**< LTSM State in Oscillator Clock Domain */
                            /* Bits[6:2], default=0x00000000*/
    UINT32 rsvd_0 : 3; /**< Undefined - auto filled rsvd_[09:07] */
                            /* Bits[9:7], default=0*/
    UINT32 adeskew_dis : 1; /**< Adapted Deskew Disable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 lnrstctl : 3; /**< Lane Reset Control */
                            /* Bits[13:11], default=0x00000000*/
    UINT32 rsvd_1 : 5; /**< Undefined - auto filled rsvd_[18:14] */
                            /* Bits[18:14], default=0*/
    UINT32 pasv : 1; /**< Primary Access SAI Violation */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 sasv : 1; /**< Sideband Access SAI Violation */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 cbgm : 1; /**< Chain Bit Generation Mode */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 cpgwakectrl : 2; /**< CPG Wake Control */
                            /* Bits[23:22], default=0x00000000*/
    UINT32 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[24:24] */
                            /* Bits[24:24], default=0*/
    UINT32 vspfscg : 1; /**< VISA Signals PV Flop Static Clock Gating Enable. */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 vspfb : 1; /**< Visa Signals PV Flop Bypass */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 rrxdme : 1; /**< Redo Receiver Detection Mechanism Enable */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 rrxc : 3; /**< Redo Receiver Detection Count */
                            /* Bits[30:28], default=0x00000000*/
    UINT32 cpudmiudecmsic : 1; /**< CPU DMI Upstream Decoder MSI Checking */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} ACRG3_PCIE_CFG_STRUCT;

#define ACRG3_PCIE_CFG_WIDTH 32
#define ACRG3_PCIE_CFG_DEFAULT 0x00000000U
#define ACRG3_PCIE_CFG_ENDPOINT PCIE_CFG

#define ACRG3_PCIE_CFG_UPHLRCFG_LSB 0x0000
#define ACRG3_PCIE_CFG_UPHLRCFG_SIZE 0x0001
#define ACRG3_PCIE_CFG_BLKRXDSA_LSB 0x0001
#define ACRG3_PCIE_CFG_BLKRXDSA_SIZE 0x0001
#define ACRG3_PCIE_CFG_LTSMSTATEOSC_LSB 0x0002
#define ACRG3_PCIE_CFG_LTSMSTATEOSC_SIZE 0x0005
#define ACRG3_PCIE_CFG_ADESKEW_DIS_LSB 0x000a
#define ACRG3_PCIE_CFG_ADESKEW_DIS_SIZE 0x0001
#define ACRG3_PCIE_CFG_LNRSTCTL_LSB 0x000b
#define ACRG3_PCIE_CFG_LNRSTCTL_SIZE 0x0003
#define ACRG3_PCIE_CFG_PASV_LSB 0x0013
#define ACRG3_PCIE_CFG_PASV_SIZE 0x0001
#define ACRG3_PCIE_CFG_SASV_LSB 0x0014
#define ACRG3_PCIE_CFG_SASV_SIZE 0x0001
#define ACRG3_PCIE_CFG_CBGM_LSB 0x0015
#define ACRG3_PCIE_CFG_CBGM_SIZE 0x0001
#define ACRG3_PCIE_CFG_CPGWAKECTRL_LSB 0x0016
#define ACRG3_PCIE_CFG_CPGWAKECTRL_SIZE 0x0002
#define ACRG3_PCIE_CFG_VSPFSCG_LSB 0x0019
#define ACRG3_PCIE_CFG_VSPFSCG_SIZE 0x0001
#define ACRG3_PCIE_CFG_VSPFB_LSB 0x001a
#define ACRG3_PCIE_CFG_VSPFB_SIZE 0x0001
#define ACRG3_PCIE_CFG_RRXDME_LSB 0x001b
#define ACRG3_PCIE_CFG_RRXDME_SIZE 0x0001
#define ACRG3_PCIE_CFG_RRXC_LSB 0x001c
#define ACRG3_PCIE_CFG_RRXC_SIZE 0x0003
#define ACRG3_PCIE_CFG_CPUDMIUDECMSIC_LSB 0x001f
#define ACRG3_PCIE_CFG_CPUDMIUDECMSIC_SIZE 0x0001



/** DPCECH desc:
  */

#define DPCECH_PCIE_CFG_REG              (0x00000a00U)

typedef union {
  struct {
    UINT32 cid : 16; /**< Capability ID */
                            /* Bits[15:0], default=0x00000000*/
    UINT32 cv : 4; /**< Capability Version */
                            /* Bits[19:16], default=0x00000000*/
    UINT32 nco : 12; /**< Next Capability Offset */
                            /* Bits[31:20], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DPCECH_PCIE_CFG_STRUCT;

#define DPCECH_PCIE_CFG_WIDTH 32
#define DPCECH_PCIE_CFG_DEFAULT 0x00000000U
#define DPCECH_PCIE_CFG_ENDPOINT PCIE_CFG

#define DPCECH_PCIE_CFG_CID_LSB 0x0000
#define DPCECH_PCIE_CFG_CID_SIZE 0x0010
#define DPCECH_PCIE_CFG_CV_LSB 0x0010
#define DPCECH_PCIE_CFG_CV_SIZE 0x0004
#define DPCECH_PCIE_CFG_NCO_LSB 0x0014
#define DPCECH_PCIE_CFG_NCO_SIZE 0x000c

/** DPCCAPR desc:
  */

#define DPCCAPR_PCIE_CFG_REG             (0x00000a04U)

typedef union {
  struct {
    UINT16 dpcimn : 5; /**< DPC Interrupt Message Number */
                            /* Bits[4:0], default=0x00000000*/
    UINT16 rpefdpc : 1; /**< RP Extensions For DPC */
                            /* Bits[5:5], default=0x00000001*/
    UINT16 ptlpebs : 1; /**< Poisoned TLP Egress Blocking Supported */
                            /* Bits[6:6], default=0x00000001*/
    UINT16 dpcsts : 1; /**< DPC Software Triggering Supported */
                            /* Bits[7:7], default=0x00000001*/
    UINT16 rppiols : 4; /**< RP PIO Log Size */
                            /* Bits[11:8], default=0x00000004*/
    UINT16 dlaecss : 1; /**< DL_Active ERR_COR Signaling Supported */
                            /* Bits[12:12], default=0x00000001*/
    UINT16 rsvd_m : 3; /**< Reserved */
                            /* Bits[15:13], default=0x00000000*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} DPCCAPR_PCIE_CFG_STRUCT;

#define DPCCAPR_PCIE_CFG_WIDTH 16
#define DPCCAPR_PCIE_CFG_DEFAULT 0x14e0U
#define DPCCAPR_PCIE_CFG_ENDPOINT PCIE_CFG

#define DPCCAPR_PCIE_CFG_DPCIMN_LSB 0x0000
#define DPCCAPR_PCIE_CFG_DPCIMN_SIZE 0x0005
#define DPCCAPR_PCIE_CFG_RPEFDPC_LSB 0x0005
#define DPCCAPR_PCIE_CFG_RPEFDPC_SIZE 0x0001
#define DPCCAPR_PCIE_CFG_PTLPEBS_LSB 0x0006
#define DPCCAPR_PCIE_CFG_PTLPEBS_SIZE 0x0001
#define DPCCAPR_PCIE_CFG_DPCSTS_LSB 0x0007
#define DPCCAPR_PCIE_CFG_DPCSTS_SIZE 0x0001
#define DPCCAPR_PCIE_CFG_RPPIOLS_LSB 0x0008
#define DPCCAPR_PCIE_CFG_RPPIOLS_SIZE 0x0004
#define DPCCAPR_PCIE_CFG_DLAECSS_LSB 0x000c
#define DPCCAPR_PCIE_CFG_DLAECSS_SIZE 0x0001
#define DPCCAPR_PCIE_CFG_RSVD_M_LSB 0x000d
#define DPCCAPR_PCIE_CFG_RSVD_M_SIZE 0x0003

/** SPEECH desc:
  */

#define SPEECH_PCIE_CFG_REG              (0x00000a30U)

typedef union {
  struct {
    UINT32 pcieecid : 16; /**< PCI Express Extended Capability ID */
                            /* Bits[15:0], default=0x00000000*/
    UINT32 cv : 4; /**< Capability Version */
                            /* Bits[19:16], default=0x00000000*/
    UINT32 nco : 12; /**< Next Capability Offset */
                            /* Bits[31:20], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} SPEECH_PCIE_CFG_STRUCT;

#define SPEECH_PCIE_CFG_WIDTH 32
#define SPEECH_PCIE_CFG_DEFAULT 0x00000000U
#define SPEECH_PCIE_CFG_ENDPOINT PCIE_CFG

#define SPEECH_PCIE_CFG_PCIEECID_LSB 0x0000
#define SPEECH_PCIE_CFG_PCIEECID_SIZE 0x0010
#define SPEECH_PCIE_CFG_CV_LSB 0x0010
#define SPEECH_PCIE_CFG_CV_SIZE 0x0004
#define SPEECH_PCIE_CFG_NCO_LSB 0x0014
#define SPEECH_PCIE_CFG_NCO_SIZE 0x000c

/** LCTL3 desc:
  */

#define LCTL3_PCIE_CFG_REG               (0x00000a34U)

typedef union {
  struct {
    UINT32 pe : 1; /**< Perform Equalization */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 lerie : 1; /**< Link Equalization Request Interrupt Enable */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 rsvd_0 : 7; /**< Undefined - auto filled rsvd_[08:02] */
                            /* Bits[8:2], default=0*/
    UINT32 elsosgv : 7; /**< Enable Lower SKP OS Generation Vector */
                            /* Bits[15:9], default=0x00000000*/
    UINT32 rsvd_m : 16; /**< Reserved */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} LCTL3_PCIE_CFG_STRUCT;

#define LCTL3_PCIE_CFG_WIDTH 32
#define LCTL3_PCIE_CFG_DEFAULT 0x00000000U
#define LCTL3_PCIE_CFG_ENDPOINT PCIE_CFG

#define LCTL3_PCIE_CFG_PE_LSB 0x0000
#define LCTL3_PCIE_CFG_PE_SIZE 0x0001
#define LCTL3_PCIE_CFG_LERIE_LSB 0x0001
#define LCTL3_PCIE_CFG_LERIE_SIZE 0x0001
#define LCTL3_PCIE_CFG_ELSOSGV_LSB 0x0009
#define LCTL3_PCIE_CFG_ELSOSGV_SIZE 0x0007
#define LCTL3_PCIE_CFG_RSVD_M_LSB 0x0010
#define LCTL3_PCIE_CFG_RSVD_M_SIZE 0x0010

/** L01EC desc:
  */

#define L01EC_PCIE_CFG_REG               (0x00000a3cU)

typedef union {
  struct {
    UINT32 dpl0tp : 4; /**< Downstream Port Lane 0 Transmitter Preset */
                            /* Bits[3:0], default=0x0000000F*/
    UINT32 dpl0rph : 3; /**< Downstream Port Lane 0 Receiver Preset Hint */
                            /* Bits[6:4], default=0x00000007*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[07:07] */
                            /* Bits[7:7], default=0*/
    UINT32 upl0tp : 4; /**< Upstream Port Lane 0 Transmitter Preset */
                            /* Bits[11:8], default=0x0000000F*/
    UINT32 upl0rph : 3; /**< Upstream Port Lane 0 Receiver Preset Hint */
                            /* Bits[14:12], default=0x00000007*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[15:15] */
                            /* Bits[15:15], default=0*/
    UINT32 dpl1tp : 4; /**< Downstream Port Lane 1 Transmitter Preset */
                            /* Bits[19:16], default=0x0000000F*/
    UINT32 dpl1rph : 3; /**< Downstream Port Lane 1 Receiver Preset Hint */
                            /* Bits[22:20], default=0x00000007*/
    UINT32 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[23:23] */
                            /* Bits[23:23], default=0*/
    UINT32 upl1tp : 4; /**< Upstream Port Lane 1 Transmitter Preset */
                            /* Bits[27:24], default=0x0000000F*/
    UINT32 upl1rph : 3; /**< Upstream Port Lane 1 Receiver Preset Hint */
                            /* Bits[30:28], default=0x00000007*/
    UINT32 rsvd_m : 1; /**< Reserved */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} L01EC_PCIE_CFG_STRUCT;

#define L01EC_PCIE_CFG_WIDTH 32
#define L01EC_PCIE_CFG_DEFAULT 0x7f7f7f7fU
#define L01EC_PCIE_CFG_ENDPOINT PCIE_CFG

#define L01EC_PCIE_CFG_DPL0TP_LSB 0x0000
#define L01EC_PCIE_CFG_DPL0TP_SIZE 0x0004
#define L01EC_PCIE_CFG_DPL0RPH_LSB 0x0004
#define L01EC_PCIE_CFG_DPL0RPH_SIZE 0x0003
#define L01EC_PCIE_CFG_UPL0TP_LSB 0x0008
#define L01EC_PCIE_CFG_UPL0TP_SIZE 0x0004
#define L01EC_PCIE_CFG_UPL0RPH_LSB 0x000c
#define L01EC_PCIE_CFG_UPL0RPH_SIZE 0x0003
#define L01EC_PCIE_CFG_DPL1TP_LSB 0x0010
#define L01EC_PCIE_CFG_DPL1TP_SIZE 0x0004
#define L01EC_PCIE_CFG_DPL1RPH_LSB 0x0014
#define L01EC_PCIE_CFG_DPL1RPH_SIZE 0x0003
#define L01EC_PCIE_CFG_UPL1TP_LSB 0x0018
#define L01EC_PCIE_CFG_UPL1TP_SIZE 0x0004
#define L01EC_PCIE_CFG_UPL1RPH_LSB 0x001c
#define L01EC_PCIE_CFG_UPL1RPH_SIZE 0x0003
#define L01EC_PCIE_CFG_RSVD_M_LSB 0x001f
#define L01EC_PCIE_CFG_RSVD_M_SIZE 0x0001

/** DLFECH desc:
  */

#define DLFECH_PCIE_CFG_REG              (0x00000a90U)

typedef union {
  struct {
    UINT32 pciecid : 16; /**< PCI Express Extended Capability ID */
                            /* Bits[15:0], default=0x00000000*/
    UINT32 cv : 4; /**< Capability Version */
                            /* Bits[19:16], default=0x00000000*/
    UINT32 nco : 12; /**< Next Capability Offset */
                            /* Bits[31:20], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DLFECH_PCIE_CFG_STRUCT;

#define DLFECH_PCIE_CFG_WIDTH 32
#define DLFECH_PCIE_CFG_DEFAULT 0x00000000U
#define DLFECH_PCIE_CFG_ENDPOINT PCIE_CFG

#define DLFECH_PCIE_CFG_PCIECID_LSB 0x0000
#define DLFECH_PCIE_CFG_PCIECID_SIZE 0x0010
#define DLFECH_PCIE_CFG_CV_LSB 0x0010
#define DLFECH_PCIE_CFG_CV_SIZE 0x0004
#define DLFECH_PCIE_CFG_NCO_LSB 0x0014
#define DLFECH_PCIE_CFG_NCO_SIZE 0x000c

/** DLFCAP desc:
  */

#define DLFCAP_PCIE_CFG_REG              (0x00000a94U)

typedef union {
  struct {
    UINT32 lsfcs : 1; /**< Local Scaled Flow Control Supported */
                            /* Bits[0:0], default=0x00000001*/
    UINT32 lfs : 22; /**< Local Feature Supported */
                            /* Bits[22:1], default=0x00000000*/
    UINT32 rsvd_0 : 8; /**< Undefined - auto filled rsvd_[30:23] */
                            /* Bits[30:23], default=0*/
    UINT32 dlfee : 1; /**< Data Link Feature Exchange Enable */
                            /* Bits[31:31], default=0x00000001*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DLFCAP_PCIE_CFG_STRUCT;

#define DLFCAP_PCIE_CFG_WIDTH 32
#define DLFCAP_PCIE_CFG_DEFAULT 0x80000001U
#define DLFCAP_PCIE_CFG_ENDPOINT PCIE_CFG

#define DLFCAP_PCIE_CFG_LSFCS_LSB 0x0000
#define DLFCAP_PCIE_CFG_LSFCS_SIZE 0x0001
#define DLFCAP_PCIE_CFG_LFS_LSB 0x0001
#define DLFCAP_PCIE_CFG_LFS_SIZE 0x0016
#define DLFCAP_PCIE_CFG_DLFEE_LSB 0x001f
#define DLFCAP_PCIE_CFG_DLFEE_SIZE 0x0001

/** PL16GECH desc:
  */

#define PL16GECH_PCIE_CFG_REG            (0x00000a9cU)

typedef union {
  struct {
    UINT32 pciecid : 16; /**< PCI Express Extended Capability ID */
                            /* Bits[15:0], default=0x00000000*/
    UINT32 cv : 4; /**< Capability Version */
                            /* Bits[19:16], default=0x00000000*/
    UINT32 nco : 12; /**< Next Capability Offset */
                            /* Bits[31:20], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PL16GECH_PCIE_CFG_STRUCT;

#define PL16GECH_PCIE_CFG_WIDTH 32
#define PL16GECH_PCIE_CFG_DEFAULT 0x00000000U
#define PL16GECH_PCIE_CFG_ENDPOINT PCIE_CFG

#define PL16GECH_PCIE_CFG_PCIECID_LSB 0x0000
#define PL16GECH_PCIE_CFG_PCIECID_SIZE 0x0010
#define PL16GECH_PCIE_CFG_CV_LSB 0x0010
#define PL16GECH_PCIE_CFG_CV_SIZE 0x0004
#define PL16GECH_PCIE_CFG_NCO_LSB 0x0014
#define PL16GECH_PCIE_CFG_NCO_SIZE 0x000c

/** PL16S desc:
  */

#define PL16S_PCIE_CFG_REG               (0x00000aa8U)

typedef union {
  struct {
    UINT32 eqg4 : 1; /**< Equalization 16.0 GT/s Complete */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 eqp1sg4 : 1; /**< Equalization 16.0 GT/s Phase 1 Successful */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 eqp2sg4 : 1; /**< Equalization 16.0 GT/s Phase 2 Successful */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 eqp3sg4 : 1; /**< Equalization 16.0 GT/s Phase 3 Successful */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 lerg4 : 1; /**< Link Equalization Request 16.0 GT/s */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 rsvd_m : 27; /**< Reserved */
                            /* Bits[31:5], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PL16S_PCIE_CFG_STRUCT;

#define PL16S_PCIE_CFG_WIDTH 32
#define PL16S_PCIE_CFG_DEFAULT 0x00000000U
#define PL16S_PCIE_CFG_ENDPOINT PCIE_CFG

#define PL16S_PCIE_CFG_EQG4_LSB 0x0000
#define PL16S_PCIE_CFG_EQG4_SIZE 0x0001
#define PL16S_PCIE_CFG_EQP1SG4_LSB 0x0001
#define PL16S_PCIE_CFG_EQP1SG4_SIZE 0x0001
#define PL16S_PCIE_CFG_EQP2SG4_LSB 0x0002
#define PL16S_PCIE_CFG_EQP2SG4_SIZE 0x0001
#define PL16S_PCIE_CFG_EQP3SG4_LSB 0x0003
#define PL16S_PCIE_CFG_EQP3SG4_SIZE 0x0001
#define PL16S_PCIE_CFG_LERG4_LSB 0x0004
#define PL16S_PCIE_CFG_LERG4_SIZE 0x0001
#define PL16S_PCIE_CFG_RSVD_M_LSB 0x0005
#define PL16S_PCIE_CFG_RSVD_M_SIZE 0x001b

/** PL16L01EC desc:
  */

#define PL16L01EC_PCIE_CFG_REG           (0x00000abcU)

typedef union {
  struct {
    UINT16 dp16l0tp : 4; /**< Downstream Port 16 GT/s Lane 0 Transmitter Preset */
                            /* Bits[3:0], default=0x0000000F*/
    UINT16 up16l0tp : 4; /**< Upstream Port 16 GT/s Port Lane 0 Transmitter Preset */
                            /* Bits[7:4], default=0x0000000F*/
    UINT16 dp16l1tp : 4; /**< Downstream Port 16 GT/s Lane 1 Transmitter Preset */
                            /* Bits[11:8], default=0x0000000F*/
    UINT16 up16l1tp : 4; /**< Upstream Port 16 GT/s Port Lane 1 Transmitter Preset */
                            /* Bits[15:12], default=0x0000000F*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} PL16L01EC_PCIE_CFG_STRUCT;

#define PL16L01EC_PCIE_CFG_WIDTH 16
#define PL16L01EC_PCIE_CFG_DEFAULT 0xffffU
#define PL16L01EC_PCIE_CFG_ENDPOINT PCIE_CFG

#define PL16L01EC_PCIE_CFG_DP16L0TP_LSB 0x0000
#define PL16L01EC_PCIE_CFG_DP16L0TP_SIZE 0x0004
#define PL16L01EC_PCIE_CFG_UP16L0TP_LSB 0x0004
#define PL16L01EC_PCIE_CFG_UP16L0TP_SIZE 0x0004
#define PL16L01EC_PCIE_CFG_DP16L1TP_LSB 0x0008
#define PL16L01EC_PCIE_CFG_DP16L1TP_SIZE 0x0004
#define PL16L01EC_PCIE_CFG_UP16L1TP_LSB 0x000c
#define PL16L01EC_PCIE_CFG_UP16L1TP_SIZE 0x0004

/** G5ECH desc:
  */

#define G5ECH_PCIE_CFG_REG               (0x00000adcU)

typedef union {
  struct {
    UINT32 ecid : 16; /**< PCI Express Extended Capability ID */
                            /* Bits[15:0], default=0x0000002A*/
    UINT32 cv : 4; /**< Capability Version */
                            /* Bits[19:16], default=0x00000000*/
    UINT32 nco : 12; /**< Next Capability Offset */
                            /* Bits[31:20], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} G5ECH_PCIE_CFG_STRUCT;

#define G5ECH_PCIE_CFG_WIDTH 32
#define G5ECH_PCIE_CFG_DEFAULT 0x0000002aU
#define G5ECH_PCIE_CFG_ENDPOINT PCIE_CFG

#define G5ECH_PCIE_CFG_ECID_LSB 0x0000
#define G5ECH_PCIE_CFG_ECID_SIZE 0x0010
#define G5ECH_PCIE_CFG_CV_LSB 0x0010
#define G5ECH_PCIE_CFG_CV_SIZE 0x0004
#define G5ECH_PCIE_CFG_NCO_LSB 0x0014
#define G5ECH_PCIE_CFG_NCO_SIZE 0x000c

/** G5CAP desc:
  */

#define G5CAP_PCIE_CFG_REG               (0x00000ae0U)

typedef union {
  struct {
    UINT32 eqbypsup : 1; /**< Equalization bypass to highest rate Supported */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 noeqsup : 1; /**< No Equalization Needed Supported\n */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 rsvd_0 : 6; /**< Undefined - auto filled rsvd_[07:02] */
                            /* Bits[7:2], default=0*/
    UINT32 modtsusgmd0sup : 1; /**< Modified TS Usage Mode 0 Supported - PCI Express */
                            /* Bits[8:8], default=0x00000001*/
    UINT32 modtsusgmd1sup : 1; /**< Modified TS Usage Mode 1 Supported - Training Set Message */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 modtsusgmd2sup : 1; /**< Modified TS Usage Mode 2 Supported - Alternate Protocol */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 modtsusgmdrsvrvd : 5; /**< Modified TS Reserved Usage Modes */
                            /* Bits[15:11], default=0x00000000*/
    UINT32 rsvd_m : 16; /**< Reserved */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} G5CAP_PCIE_CFG_STRUCT;

#define G5CAP_PCIE_CFG_WIDTH 32
#define G5CAP_PCIE_CFG_DEFAULT 0x00000100U
#define G5CAP_PCIE_CFG_ENDPOINT PCIE_CFG

#define G5CAP_PCIE_CFG_EQBYPSUP_LSB 0x0000
#define G5CAP_PCIE_CFG_EQBYPSUP_SIZE 0x0001
#define G5CAP_PCIE_CFG_NOEQSUP_LSB 0x0001
#define G5CAP_PCIE_CFG_NOEQSUP_SIZE 0x0001
#define G5CAP_PCIE_CFG_MODTSUSGMD0SUP_LSB 0x0008
#define G5CAP_PCIE_CFG_MODTSUSGMD0SUP_SIZE 0x0001
#define G5CAP_PCIE_CFG_MODTSUSGMD1SUP_LSB 0x0009
#define G5CAP_PCIE_CFG_MODTSUSGMD1SUP_SIZE 0x0001
#define G5CAP_PCIE_CFG_MODTSUSGMD2SUP_LSB 0x000a
#define G5CAP_PCIE_CFG_MODTSUSGMD2SUP_SIZE 0x0001
#define G5CAP_PCIE_CFG_MODTSUSGMDRSVRVD_LSB 0x000b
#define G5CAP_PCIE_CFG_MODTSUSGMDRSVRVD_SIZE 0x0005
#define G5CAP_PCIE_CFG_RSVD_M_LSB 0x0010
#define G5CAP_PCIE_CFG_RSVD_M_SIZE 0x0010

/** G5CTL desc:
  */

#define G5CTL_PCIE_CFG_REG               (0x00000ae4U)

typedef union {
  struct {
    UINT32 eqbypdis : 1; /**< Equalization bypass to highest rate Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 noeqdis : 1; /**< No Equalization Needed Disable */
                            /* Bits[1:1], default=0x00000001*/
    UINT32 rsvd_0 : 6; /**< Undefined - auto filled rsvd_[07:02] */
                            /* Bits[7:2], default=0*/
    UINT32 modtsusgsel : 3; /**< Modified TS Usage Mode Selected */
                            /* Bits[10:8], default=0x00000000*/
    UINT32 rsvd_m : 21; /**< Reserved */
                            /* Bits[31:11], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} G5CTL_PCIE_CFG_STRUCT;

#define G5CTL_PCIE_CFG_WIDTH 32
#define G5CTL_PCIE_CFG_DEFAULT 0x00000002U
#define G5CTL_PCIE_CFG_ENDPOINT PCIE_CFG

#define G5CTL_PCIE_CFG_EQBYPDIS_LSB 0x0000
#define G5CTL_PCIE_CFG_EQBYPDIS_SIZE 0x0001
#define G5CTL_PCIE_CFG_NOEQDIS_LSB 0x0001
#define G5CTL_PCIE_CFG_NOEQDIS_SIZE 0x0001
#define G5CTL_PCIE_CFG_MODTSUSGSEL_LSB 0x0008
#define G5CTL_PCIE_CFG_MODTSUSGSEL_SIZE 0x0003
#define G5CTL_PCIE_CFG_RSVD_M_LSB 0x000b
#define G5CTL_PCIE_CFG_RSVD_M_SIZE 0x0015

/** G5STS desc:
  */

#define G5STS_PCIE_CFG_REG               (0x00000ae8U)

typedef union {
  struct {
    UINT32 eq32cmplt : 1; /**< Equalization 32.0 GT/s Complete */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 eq32ph1succ : 1; /**< Equalization 32.0 GT/s Phase 1 Successful */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 eq32ph2succ : 1; /**< Equalization 32.0 GT/s Phase 2 Successful */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 eq32ph3succ : 1; /**< Equalization 32.0 GT/s Phase 3 Successfu */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 lerg5 : 1; /**< Link Equalization Request 32.0 GT/s */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 modtsrcvd : 1; /**< Modified TS Received */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 rcvdelbctl : 2; /**< Received Enhanced Link Behavior Control */
                            /* Bits[7:6], default=0x00000000*/
    UINT32 txprecodeo : 1; /**< Transmitter Precoding On */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 txprecoder : 1; /**< Transmitter Precode Request */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 noeqr : 1; /**< No Equalization Needed Received */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 rsvd_m : 21; /**< Reserved */
                            /* Bits[31:11], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} G5STS_PCIE_CFG_STRUCT;

#define G5STS_PCIE_CFG_WIDTH 32
#define G5STS_PCIE_CFG_DEFAULT 0x00000000U
#define G5STS_PCIE_CFG_ENDPOINT PCIE_CFG

#define G5STS_PCIE_CFG_EQ32CMPLT_LSB 0x0000
#define G5STS_PCIE_CFG_EQ32CMPLT_SIZE 0x0001
#define G5STS_PCIE_CFG_EQ32PH1SUCC_LSB 0x0001
#define G5STS_PCIE_CFG_EQ32PH1SUCC_SIZE 0x0001
#define G5STS_PCIE_CFG_EQ32PH2SUCC_LSB 0x0002
#define G5STS_PCIE_CFG_EQ32PH2SUCC_SIZE 0x0001
#define G5STS_PCIE_CFG_EQ32PH3SUCC_LSB 0x0003
#define G5STS_PCIE_CFG_EQ32PH3SUCC_SIZE 0x0001
#define G5STS_PCIE_CFG_LERG5_LSB 0x0004
#define G5STS_PCIE_CFG_LERG5_SIZE 0x0001
#define G5STS_PCIE_CFG_MODTSRCVD_LSB 0x0005
#define G5STS_PCIE_CFG_MODTSRCVD_SIZE 0x0001
#define G5STS_PCIE_CFG_RCVDELBCTL_LSB 0x0006
#define G5STS_PCIE_CFG_RCVDELBCTL_SIZE 0x0002
#define G5STS_PCIE_CFG_TXPRECODEO_LSB 0x0008
#define G5STS_PCIE_CFG_TXPRECODEO_SIZE 0x0001
#define G5STS_PCIE_CFG_TXPRECODER_LSB 0x0009
#define G5STS_PCIE_CFG_TXPRECODER_SIZE 0x0001
#define G5STS_PCIE_CFG_NOEQR_LSB 0x000a
#define G5STS_PCIE_CFG_NOEQR_SIZE 0x0001
#define G5STS_PCIE_CFG_RSVD_M_LSB 0x000b
#define G5STS_PCIE_CFG_RSVD_M_SIZE 0x0015

/** G5LANEEQCTL_0 desc:
  */

#define G5LANEEQCTL_0_PCIE_CFG_REG       (0x00000afcU)

typedef union {
  struct {
    UINT32 l0dptp : 4; /**< Lane 0 Downstream Port 32.0 GT/s Transmitter Preset */
                            /* Bits[3:0], default=0x0000000F*/
    UINT32 l0uptp : 4; /**< Lane 0 Upstream Port 32.0 GT/s Transmitter Preset */
                            /* Bits[7:4], default=0x0000000F*/
    UINT32 l1dptp : 4; /**< Lane 1 Downstream Port 32.0 GT/s Transmitter Preset */
                            /* Bits[11:8], default=0x0000000F*/
    UINT32 l1uptp : 4; /**< Lane 1 Upstream Port 32.0 GT/s Transmitter Preset */
                            /* Bits[15:12], default=0x0000000F*/
    UINT32 l2dptp : 4; /**< Lane 2 Downstream Port 32.0 GT/s Transmitter Preset */
                            /* Bits[19:16], default=0x0000000F*/
    UINT32 l2uptp : 4; /**< Lane 2 Upstream Port 32.0 GT/s Transmitter Preset */
                            /* Bits[23:20], default=0x0000000F*/
    UINT32 l3dptp : 4; /**< Lane 3 Downstream Port 32.0 GT/s Transmitter Preset */
                            /* Bits[27:24], default=0x0000000F*/
    UINT32 l3uptp : 4; /**< Lane 3 Upstream Port 32.0 GT/s Transmitter Preset */
                            /* Bits[31:28], default=0x0000000F*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} G5LANEEQCTL_0_PCIE_CFG_STRUCT;

#define G5LANEEQCTL_0_PCIE_CFG_WIDTH 32
#define G5LANEEQCTL_0_PCIE_CFG_DEFAULT 0xffffffffU
#define G5LANEEQCTL_0_PCIE_CFG_ENDPOINT PCIE_CFG

#define G5LANEEQCTL_0_PCIE_CFG_L0DPTP_LSB 0x0000
#define G5LANEEQCTL_0_PCIE_CFG_L0DPTP_SIZE 0x0004
#define G5LANEEQCTL_0_PCIE_CFG_L0UPTP_LSB 0x0004
#define G5LANEEQCTL_0_PCIE_CFG_L0UPTP_SIZE 0x0004
#define G5LANEEQCTL_0_PCIE_CFG_L1DPTP_LSB 0x0008
#define G5LANEEQCTL_0_PCIE_CFG_L1DPTP_SIZE 0x0004
#define G5LANEEQCTL_0_PCIE_CFG_L1UPTP_LSB 0x000c
#define G5LANEEQCTL_0_PCIE_CFG_L1UPTP_SIZE 0x0004
#define G5LANEEQCTL_0_PCIE_CFG_L2DPTP_LSB 0x0010
#define G5LANEEQCTL_0_PCIE_CFG_L2DPTP_SIZE 0x0004
#define G5LANEEQCTL_0_PCIE_CFG_L2UPTP_LSB 0x0014
#define G5LANEEQCTL_0_PCIE_CFG_L2UPTP_SIZE 0x0004
#define G5LANEEQCTL_0_PCIE_CFG_L3DPTP_LSB 0x0018
#define G5LANEEQCTL_0_PCIE_CFG_L3DPTP_SIZE 0x0004
#define G5LANEEQCTL_0_PCIE_CFG_L3UPTP_LSB 0x001c
#define G5LANEEQCTL_0_PCIE_CFG_L3UPTP_SIZE 0x0004

/** ACGR3S2 desc:
  */

#define ACGR3S2_PCIE_CFG_REG             (0x00000c50U)

typedef union {
  struct {
    UINT32 lstptls : 4; /**< LSTP Target Link Speed */
                            /* Bits[3:0], default=0x00000004*/
    UINT32 drxtermdq : 1; /**< Disabled RxTermination in Detect.Quiet */
                            /* Bits[4:4], default=0x00000001*/
    UINT32 srt : 1; /**< Simplified Replay Timer */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 g1ebm : 1; /**< Gen 1 Elastic Buffer Mode */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 g2ebm : 1; /**< Gen 2 Elastic Buffer Mode */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 g3ebm : 1; /**< Gen 3 Elastic Buffer Mode */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 g4ebm : 1; /**< Gen 4 Elastic Buffer Mode */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 clkreqmsgtxvlmdestid : 8; /**< CLKREQ Message Transmit VLM Destination ID */
                            /* Bits[17:10], default=0x00000000*/
    UINT32 rxadtrecrxmrge : 1; /**< Receiver Adaptation in Recovery during Receiver Margining Enable */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[19:19] */
                            /* Bits[19:19], default=0*/
    UINT32 g5ebm : 1; /**< Gen 5 Elastic Buffer Mode */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 g6ebm_rsvd : 1; /**< Reserved */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 rtd3wrphrp : 1; /**< RTD3 with Reset Prep Hot Reset Policy */
                            /* Bits[22:22], default=0x00000000*/
    UINT32 pg5tprcc : 1; /**< PCIe Gen 5 Transmitter Precode Request Capability Control */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 cfgsllncp : 1; /**< Configuration Substates Link Lanes Number Capture Policy */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 ltatgce : 1; /**< Link Transmitter Arbiter TLP Grant Count Enable */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 rsvd_rw1 : 1; /**< Reserved */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[27:27] */
                            /* Bits[27:27], default=0*/
    UINT32 rsvd_rw : 3; /**< Reserved */
                            /* Bits[30:28], default=0x00000000*/
    UINT32 rsvd_m : 1; /**< Reserved - This flops being recycle to use for ECO fix for PTMPDAIB registers implementations. */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} ACGR3S2_PCIE_CFG_STRUCT;

#define ACGR3S2_PCIE_CFG_WIDTH 32
#define ACGR3S2_PCIE_CFG_DEFAULT 0x00000014U
#define ACGR3S2_PCIE_CFG_ENDPOINT PCIE_CFG

#define ACGR3S2_PCIE_CFG_LSTPTLS_LSB 0x0000
#define ACGR3S2_PCIE_CFG_LSTPTLS_SIZE 0x0004
#define ACGR3S2_PCIE_CFG_DRXTERMDQ_LSB 0x0004
#define ACGR3S2_PCIE_CFG_DRXTERMDQ_SIZE 0x0001
#define ACGR3S2_PCIE_CFG_SRT_LSB 0x0005
#define ACGR3S2_PCIE_CFG_SRT_SIZE 0x0001
#define ACGR3S2_PCIE_CFG_G1EBM_LSB 0x0006
#define ACGR3S2_PCIE_CFG_G1EBM_SIZE 0x0001
#define ACGR3S2_PCIE_CFG_G2EBM_LSB 0x0007
#define ACGR3S2_PCIE_CFG_G2EBM_SIZE 0x0001
#define ACGR3S2_PCIE_CFG_G3EBM_LSB 0x0008
#define ACGR3S2_PCIE_CFG_G3EBM_SIZE 0x0001
#define ACGR3S2_PCIE_CFG_G4EBM_LSB 0x0009
#define ACGR3S2_PCIE_CFG_G4EBM_SIZE 0x0001
#define ACGR3S2_PCIE_CFG_CLKREQMSGTXVLMDESTID_LSB 0x000a
#define ACGR3S2_PCIE_CFG_CLKREQMSGTXVLMDESTID_SIZE 0x0008
#define ACGR3S2_PCIE_CFG_RXADTRECRXMRGE_LSB 0x0012
#define ACGR3S2_PCIE_CFG_RXADTRECRXMRGE_SIZE 0x0001
#define ACGR3S2_PCIE_CFG_G5EBM_LSB 0x0014
#define ACGR3S2_PCIE_CFG_G5EBM_SIZE 0x0001
#define ACGR3S2_PCIE_CFG_G6EBM_RSVD_LSB 0x0015
#define ACGR3S2_PCIE_CFG_G6EBM_RSVD_SIZE 0x0001
#define ACGR3S2_PCIE_CFG_RTD3WRPHRP_LSB 0x0016
#define ACGR3S2_PCIE_CFG_RTD3WRPHRP_SIZE 0x0001
#define ACGR3S2_PCIE_CFG_PG5TPRCC_LSB 0x0017
#define ACGR3S2_PCIE_CFG_PG5TPRCC_SIZE 0x0001
#define ACGR3S2_PCIE_CFG_CFGSLLNCP_LSB 0x0018
#define ACGR3S2_PCIE_CFG_CFGSLLNCP_SIZE 0x0001
#define ACGR3S2_PCIE_CFG_LTATGCE_LSB 0x0019
#define ACGR3S2_PCIE_CFG_LTATGCE_SIZE 0x0001
#define ACGR3S2_PCIE_CFG_RSVD_RW1_LSB 0x001a
#define ACGR3S2_PCIE_CFG_RSVD_RW1_SIZE 0x0001
#define ACGR3S2_PCIE_CFG_RSVD_RW_LSB 0x001c
#define ACGR3S2_PCIE_CFG_RSVD_RW_SIZE 0x0003
#define ACGR3S2_PCIE_CFG_RSVD_M_LSB 0x001f
#define ACGR3S2_PCIE_CFG_RSVD_M_SIZE 0x0001



/** LTRSUBL1STD desc:
  */

#define LTRSUBL1STD_PCIE_CFG_REG         (0x00000c5cU)

typedef union {
  struct {
    UINT32 ltrslsubv : 10; /**< LTR Snoop Latency Subtraction Value */
                            /* Bits[9:0], default=0x00000000*/
    UINT32 ltrslssubv : 3; /**< LTR Snoop Latency Scale Subtraction Value */
                            /* Bits[12:10], default=0x00000000*/
    UINT32 rsvd_rw1 : 2; /**< Reserved */
                            /* Bits[14:13], default=0x00000000*/
    UINT32 ltrslsuben : 1; /**< LTR Snoop Latency Subtraction Enable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 ltrnslsubv : 10; /**< LTR Non-Snoop Latency Subtraction Value */
                            /* Bits[25:16], default=0x00000000*/
    UINT32 ltrnslssubv : 3; /**< LTR Non-Snoop Latency Scale Subtraction Value */
                            /* Bits[28:26], default=0x00000000*/
    UINT32 rsvd_rw2 : 2; /**< Reserved */
                            /* Bits[30:29], default=0x00000000*/
    UINT32 ltrnslsuben : 1; /**< LTR Non-Snoop Latency Subtraction Enable */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} LTRSUBL1STD_PCIE_CFG_STRUCT;

#define LTRSUBL1STD_PCIE_CFG_WIDTH 32
#define LTRSUBL1STD_PCIE_CFG_DEFAULT 0x00000000U
#define LTRSUBL1STD_PCIE_CFG_ENDPOINT PCIE_CFG

#define LTRSUBL1STD_PCIE_CFG_LTRSLSUBV_LSB 0x0000
#define LTRSUBL1STD_PCIE_CFG_LTRSLSUBV_SIZE 0x000a
#define LTRSUBL1STD_PCIE_CFG_LTRSLSSUBV_LSB 0x000a
#define LTRSUBL1STD_PCIE_CFG_LTRSLSSUBV_SIZE 0x0003
#define LTRSUBL1STD_PCIE_CFG_RSVD_RW1_LSB 0x000d
#define LTRSUBL1STD_PCIE_CFG_RSVD_RW1_SIZE 0x0002
#define LTRSUBL1STD_PCIE_CFG_LTRSLSUBEN_LSB 0x000f
#define LTRSUBL1STD_PCIE_CFG_LTRSLSUBEN_SIZE 0x0001
#define LTRSUBL1STD_PCIE_CFG_LTRNSLSUBV_LSB 0x0010
#define LTRSUBL1STD_PCIE_CFG_LTRNSLSUBV_SIZE 0x000a
#define LTRSUBL1STD_PCIE_CFG_LTRNSLSSUBV_LSB 0x001a
#define LTRSUBL1STD_PCIE_CFG_LTRNSLSSUBV_SIZE 0x0003
#define LTRSUBL1STD_PCIE_CFG_RSVD_RW2_LSB 0x001d
#define LTRSUBL1STD_PCIE_CFG_RSVD_RW2_SIZE 0x0002
#define LTRSUBL1STD_PCIE_CFG_LTRNSLSUBEN_LSB 0x001f
#define LTRSUBL1STD_PCIE_CFG_LTRNSLSUBEN_SIZE 0x0001



/** LTRSUBL11 desc:
  */

#define LTRSUBL11_PCIE_CFG_REG           (0x00000c60U)

typedef union {
  struct {
    UINT32 ltrslsubv : 10; /**< LTR Snoop Latency Subtraction Value */
                            /* Bits[9:0], default=0x00000000*/
    UINT32 ltrslssubv : 3; /**< LTR Snoop Latency Scale Subtraction Value */
                            /* Bits[12:10], default=0x00000000*/
    UINT32 rsvd_rw1 : 2; /**< Reserved */
                            /* Bits[14:13], default=0x00000000*/
    UINT32 ltrslsuben : 1; /**< LTR Snoop Latency Subtraction Enable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 ltrnslsubv : 10; /**< LTR Non-Snoop Latency Subtraction Value */
                            /* Bits[25:16], default=0x00000000*/
    UINT32 ltrnslssubv : 3; /**< LTR Non-Snoop Latency Scale Subtraction Value */
                            /* Bits[28:26], default=0x00000000*/
    UINT32 rsvd_rw2 : 2; /**< Reserved */
                            /* Bits[30:29], default=0x00000000*/
    UINT32 ltrnslsuben : 1; /**< LTR Non-Snoop Latency Subtraction Enable */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} LTRSUBL11_PCIE_CFG_STRUCT;

#define LTRSUBL11_PCIE_CFG_WIDTH 32
#define LTRSUBL11_PCIE_CFG_DEFAULT 0x00000000U
#define LTRSUBL11_PCIE_CFG_ENDPOINT PCIE_CFG

#define LTRSUBL11_PCIE_CFG_LTRSLSUBV_LSB 0x0000
#define LTRSUBL11_PCIE_CFG_LTRSLSUBV_SIZE 0x000a
#define LTRSUBL11_PCIE_CFG_LTRSLSSUBV_LSB 0x000a
#define LTRSUBL11_PCIE_CFG_LTRSLSSUBV_SIZE 0x0003
#define LTRSUBL11_PCIE_CFG_RSVD_RW1_LSB 0x000d
#define LTRSUBL11_PCIE_CFG_RSVD_RW1_SIZE 0x0002
#define LTRSUBL11_PCIE_CFG_LTRSLSUBEN_LSB 0x000f
#define LTRSUBL11_PCIE_CFG_LTRSLSUBEN_SIZE 0x0001
#define LTRSUBL11_PCIE_CFG_LTRNSLSUBV_LSB 0x0010
#define LTRSUBL11_PCIE_CFG_LTRNSLSUBV_SIZE 0x000a
#define LTRSUBL11_PCIE_CFG_LTRNSLSSUBV_LSB 0x001a
#define LTRSUBL11_PCIE_CFG_LTRNSLSSUBV_SIZE 0x0003
#define LTRSUBL11_PCIE_CFG_RSVD_RW2_LSB 0x001d
#define LTRSUBL11_PCIE_CFG_RSVD_RW2_SIZE 0x0002
#define LTRSUBL11_PCIE_CFG_LTRNSLSUBEN_LSB 0x001f
#define LTRSUBL11_PCIE_CFG_LTRNSLSUBEN_SIZE 0x0001



/** LTRSUBL12 desc:
  */

#define LTRSUBL12_PCIE_CFG_REG           (0x00000c64U)

typedef union {
  struct {
    UINT32 ltrslsubv : 10; /**< LTR Snoop Latency Subtraction Value */
                            /* Bits[9:0], default=0x00000000*/
    UINT32 ltrslssubv : 3; /**< LTR Snoop Latency Scale Subtraction Value */
                            /* Bits[12:10], default=0x00000000*/
    UINT32 rsvd_rw1 : 2; /**< Reserved */
                            /* Bits[14:13], default=0x00000000*/
    UINT32 ltrslsuben : 1; /**< LTR Snoop Latency Subtraction Enable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 ltrnslsubv : 10; /**< LTR Non-Snoop Latency Subtraction Value */
                            /* Bits[25:16], default=0x00000000*/
    UINT32 ltrnslssubv : 3; /**< LTR Non-Snoop Latency Scale Subtraction Value */
                            /* Bits[28:26], default=0x00000000*/
    UINT32 rsvd_rw2 : 2; /**< Reserved */
                            /* Bits[30:29], default=0x00000000*/
    UINT32 ltrnslsuben : 1; /**< LTR Non-Snoop Latency Subtraction Enable */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} LTRSUBL12_PCIE_CFG_STRUCT;

#define LTRSUBL12_PCIE_CFG_WIDTH 32
#define LTRSUBL12_PCIE_CFG_DEFAULT 0x00000000U
#define LTRSUBL12_PCIE_CFG_ENDPOINT PCIE_CFG

#define LTRSUBL12_PCIE_CFG_LTRSLSUBV_LSB 0x0000
#define LTRSUBL12_PCIE_CFG_LTRSLSUBV_SIZE 0x000a
#define LTRSUBL12_PCIE_CFG_LTRSLSSUBV_LSB 0x000a
#define LTRSUBL12_PCIE_CFG_LTRSLSSUBV_SIZE 0x0003
#define LTRSUBL12_PCIE_CFG_RSVD_RW1_LSB 0x000d
#define LTRSUBL12_PCIE_CFG_RSVD_RW1_SIZE 0x0002
#define LTRSUBL12_PCIE_CFG_LTRSLSUBEN_LSB 0x000f
#define LTRSUBL12_PCIE_CFG_LTRSLSUBEN_SIZE 0x0001
#define LTRSUBL12_PCIE_CFG_LTRNSLSUBV_LSB 0x0010
#define LTRSUBL12_PCIE_CFG_LTRNSLSUBV_SIZE 0x000a
#define LTRSUBL12_PCIE_CFG_LTRNSLSSUBV_LSB 0x001a
#define LTRSUBL12_PCIE_CFG_LTRNSLSSUBV_SIZE 0x0003
#define LTRSUBL12_PCIE_CFG_RSVD_RW2_LSB 0x001d
#define LTRSUBL12_PCIE_CFG_RSVD_RW2_SIZE 0x0002
#define LTRSUBL12_PCIE_CFG_LTRNSLSUBEN_LSB 0x001f
#define LTRSUBL12_PCIE_CFG_LTRNSLSUBEN_SIZE 0x0001



/** LTRSUBL11NPG desc:
  */

#define LTRSUBL11NPG_PCIE_CFG_REG        (0x00000c68U)

typedef union {
  struct {
    UINT32 ltrslsubv : 10; /**< LTR Snoop Latency Subtraction Value */
                            /* Bits[9:0], default=0x00000000*/
    UINT32 ltrslssubv : 3; /**< LTR Snoop Latency Scale Subtraction Value */
                            /* Bits[12:10], default=0x00000000*/
    UINT32 rsvd_rw1 : 2; /**< Reserved */
                            /* Bits[14:13], default=0x00000000*/
    UINT32 ltrslsuben : 1; /**< LTR Snoop Latency Subtraction Enable */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 ltrnslsubv : 10; /**< LTR Non-Snoop Latency Subtraction Value */
                            /* Bits[25:16], default=0x00000000*/
    UINT32 ltrnslssubv : 3; /**< LTR Non-Snoop Latency Scale Subtraction Value */
                            /* Bits[28:26], default=0x00000000*/
    UINT32 rsvd_rw2 : 2; /**< Reserved */
                            /* Bits[30:29], default=0x00000000*/
    UINT32 ltrnslsuben : 1; /**< LTR Non-Snoop Latency Subtraction Enable */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} LTRSUBL11NPG_PCIE_CFG_STRUCT;

#define LTRSUBL11NPG_PCIE_CFG_WIDTH 32
#define LTRSUBL11NPG_PCIE_CFG_DEFAULT 0x00000000U
#define LTRSUBL11NPG_PCIE_CFG_ENDPOINT PCIE_CFG

#define LTRSUBL11NPG_PCIE_CFG_LTRSLSUBV_LSB 0x0000
#define LTRSUBL11NPG_PCIE_CFG_LTRSLSUBV_SIZE 0x000a
#define LTRSUBL11NPG_PCIE_CFG_LTRSLSSUBV_LSB 0x000a
#define LTRSUBL11NPG_PCIE_CFG_LTRSLSSUBV_SIZE 0x0003
#define LTRSUBL11NPG_PCIE_CFG_RSVD_RW1_LSB 0x000d
#define LTRSUBL11NPG_PCIE_CFG_RSVD_RW1_SIZE 0x0002
#define LTRSUBL11NPG_PCIE_CFG_LTRSLSUBEN_LSB 0x000f
#define LTRSUBL11NPG_PCIE_CFG_LTRSLSUBEN_SIZE 0x0001
#define LTRSUBL11NPG_PCIE_CFG_LTRNSLSUBV_LSB 0x0010
#define LTRSUBL11NPG_PCIE_CFG_LTRNSLSUBV_SIZE 0x000a
#define LTRSUBL11NPG_PCIE_CFG_LTRNSLSSUBV_LSB 0x001a
#define LTRSUBL11NPG_PCIE_CFG_LTRNSLSSUBV_SIZE 0x0003
#define LTRSUBL11NPG_PCIE_CFG_RSVD_RW2_LSB 0x001d
#define LTRSUBL11NPG_PCIE_CFG_RSVD_RW2_SIZE 0x0002
#define LTRSUBL11NPG_PCIE_CFG_LTRNSLSUBEN_LSB 0x001f
#define LTRSUBL11NPG_PCIE_CFG_LTRNSLSUBEN_SIZE 0x0001


/** VNNREMCTL desc:
  */

#define VNNREMCTL_PCIE_CFG_REG           (0x00000c70U)

typedef union {
  struct {
    UINT32 lrslfvnnre : 2; /**< Link Reset Suppression Latency For VNN Removal Exit */
                            /* Bits[1:0], default=0x00000001*/
    UINT32 isplfvnnre : 2; /**< Internal States Propagation Latency For VNN Removal Exit */
                            /* Bits[3:2], default=0x00000000*/
    UINT32 ldvnnre : 1; /**< Link Disable VNN Removal Enable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 rtd3vnnre : 1; /**< RTD3 VNN Removal Enable */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 dnpvnnre : 1; /**< Detect Not PCIe VNN Removal Enable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 hpvnnre : 1; /**< Hot Plug VNN Removal Enable */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 fdvnnre : 1; /**< Function Disable VNN Removal */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 rsvd : 22; /**< Reserved */
                            /* Bits[30:9], default=0x00000000*/
    UINT32 remetmd : 1; /**< Receiver Eye Margin Error Tracking Mechanism Disable */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} VNNREMCTL_PCIE_CFG_STRUCT;

#define VNNREMCTL_PCIE_CFG_WIDTH 32
#define VNNREMCTL_PCIE_CFG_DEFAULT 0x00000001U
#define VNNREMCTL_PCIE_CFG_ENDPOINT PCIE_CFG

#define VNNREMCTL_PCIE_CFG_LRSLFVNNRE_LSB 0x0000
#define VNNREMCTL_PCIE_CFG_LRSLFVNNRE_SIZE 0x0002
#define VNNREMCTL_PCIE_CFG_ISPLFVNNRE_LSB 0x0002
#define VNNREMCTL_PCIE_CFG_ISPLFVNNRE_SIZE 0x0002
#define VNNREMCTL_PCIE_CFG_LDVNNRE_LSB 0x0004
#define VNNREMCTL_PCIE_CFG_LDVNNRE_SIZE 0x0001
#define VNNREMCTL_PCIE_CFG_RTD3VNNRE_LSB 0x0005
#define VNNREMCTL_PCIE_CFG_RTD3VNNRE_SIZE 0x0001
#define VNNREMCTL_PCIE_CFG_DNPVNNRE_LSB 0x0006
#define VNNREMCTL_PCIE_CFG_DNPVNNRE_SIZE 0x0001
#define VNNREMCTL_PCIE_CFG_HPVNNRE_LSB 0x0007
#define VNNREMCTL_PCIE_CFG_HPVNNRE_SIZE 0x0001
#define VNNREMCTL_PCIE_CFG_FDVNNRE_LSB 0x0008
#define VNNREMCTL_PCIE_CFG_FDVNNRE_SIZE 0x0001
#define VNNREMCTL_PCIE_CFG_RSVD_LSB 0x0009
#define VNNREMCTL_PCIE_CFG_RSVD_SIZE 0x0016
#define VNNREMCTL_PCIE_CFG_REMETMD_LSB 0x001f
#define VNNREMCTL_PCIE_CFG_REMETMD_SIZE 0x0001

/** AECR1G3 desc:
  */

#define AECR1G3_PCIE_CFG_REG             (0x00000c80U)

typedef union {
  struct {
    UINT32 dcdcctdt : 1; /**< Disable Config.Dskew to Config.Complete transition delay timer */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[01:01] */
                            /* Bits[1:1], default=0*/
    UINT32 crmtdde : 1; /**< Clock Request Message TX Demotion Delay Enable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 crmtddt : 2; /**< Clock Request Message TX Demotion Delay Timer */
                            /* Bits[4:3], default=0x00000000*/
    UINT32 lesrrp : 1; /**< Lane Error Status Register Reporting Policy */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 l1offrdyhewten : 1; /**< L1OFFRDY Host Exit Wait Time Enable */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 l1offrdyhewt : 3; /**< L1OFFRDY Host Exit Wait Time */
                            /* Bits[9:7], default=0x00000000*/
    UINT32 tpse : 1; /**< Transmitted Preset Sticky Enable */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 haedg3s : 1; /**< Hardware Autonomous Equalization Done for Gen3 with Success */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 haedg4s : 1; /**< Hardware Autonomous Equalization Done for Gen4 with Success */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 acedskrmd : 1; /**< Adaptive COM/EIEOS Deskew Reset Mechanism Disable */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 dtcgcm : 1; /**< Dynamic and Trunk Clock Gating Coupling Mode */
                            /* Bits[14:14], default=0x00000000*/
    UINT32 rxadcem : 1; /**< RX Adaptation Control During EQ Margining Phase */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 clearblkdllp : 1; /**< Clear Block DLLP */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 rsvd_rw : 1; /**< Reserved */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[18:18] */
                            /* Bits[18:18], default=0*/
    UINT32 vc1rxccqnm : 3; /**< VC1 RX Completion Command Queue NAK Mechanisms */
                            /* Bits[21:19], default=0x00000000*/
    UINT32 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[22:22] */
                            /* Bits[22:22], default=0*/
    UINT32 fcptodis : 1; /**< FCP Timeout Mechanism Disabled */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 ilppstwfd : 1; /**< Inactive Lane P2 Power State Transition Wait Fix Disable */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 tdtmtsfud : 1; /**< TX Deemphasis TX Margin Tx Swing Message Bus Force Update */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 enptmrc : 1; /**< Enable PTM Reset Count */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 rsvd_rw3 : 2; /**< Reserved. */
                            /* Bits[28:27], default=0x00000000*/
    UINT32 ifc2cup : 1; /**< INITFC2 CA Update Policy */
                            /* Bits[29:29], default=0x00000001*/
    UINT32 ifc1cap : 1; /**< INITFC1 Credit Advertise Policy */
                            /* Bits[30:30], default=0x00000001*/
    UINT32 reqtmbof : 1; /**< RxEqTrain Message Bus Offset Mapping */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} AECR1G3_PCIE_CFG_STRUCT;

#define AECR1G3_PCIE_CFG_WIDTH 32
#define AECR1G3_PCIE_CFG_DEFAULT 0x60000000U
#define AECR1G3_PCIE_CFG_ENDPOINT PCIE_CFG

#define AECR1G3_PCIE_CFG_DCDCCTDT_LSB 0x0000
#define AECR1G3_PCIE_CFG_DCDCCTDT_SIZE 0x0001
#define AECR1G3_PCIE_CFG_CRMTDDE_LSB 0x0002
#define AECR1G3_PCIE_CFG_CRMTDDE_SIZE 0x0001
#define AECR1G3_PCIE_CFG_CRMTDDT_LSB 0x0003
#define AECR1G3_PCIE_CFG_CRMTDDT_SIZE 0x0002
#define AECR1G3_PCIE_CFG_LESRRP_LSB 0x0005
#define AECR1G3_PCIE_CFG_LESRRP_SIZE 0x0001
#define AECR1G3_PCIE_CFG_L1OFFRDYHEWTEN_LSB 0x0006
#define AECR1G3_PCIE_CFG_L1OFFRDYHEWTEN_SIZE 0x0001
#define AECR1G3_PCIE_CFG_L1OFFRDYHEWT_LSB 0x0007
#define AECR1G3_PCIE_CFG_L1OFFRDYHEWT_SIZE 0x0003
#define AECR1G3_PCIE_CFG_TPSE_LSB 0x000a
#define AECR1G3_PCIE_CFG_TPSE_SIZE 0x0001
#define AECR1G3_PCIE_CFG_HAEDG3S_LSB 0x000b
#define AECR1G3_PCIE_CFG_HAEDG3S_SIZE 0x0001
#define AECR1G3_PCIE_CFG_HAEDG4S_LSB 0x000c
#define AECR1G3_PCIE_CFG_HAEDG4S_SIZE 0x0001
#define AECR1G3_PCIE_CFG_ACEDSKRMD_LSB 0x000d
#define AECR1G3_PCIE_CFG_ACEDSKRMD_SIZE 0x0001
#define AECR1G3_PCIE_CFG_DTCGCM_LSB 0x000e
#define AECR1G3_PCIE_CFG_DTCGCM_SIZE 0x0001
#define AECR1G3_PCIE_CFG_RXADCEM_LSB 0x000f
#define AECR1G3_PCIE_CFG_RXADCEM_SIZE 0x0001
#define AECR1G3_PCIE_CFG_CLEARBLKDLLP_LSB 0x0010
#define AECR1G3_PCIE_CFG_CLEARBLKDLLP_SIZE 0x0001
#define AECR1G3_PCIE_CFG_RSVD_RW_LSB 0x0011
#define AECR1G3_PCIE_CFG_RSVD_RW_SIZE 0x0001
#define AECR1G3_PCIE_CFG_VC1RXCCQNM_LSB 0x0013
#define AECR1G3_PCIE_CFG_VC1RXCCQNM_SIZE 0x0003
#define AECR1G3_PCIE_CFG_FCPTODIS_LSB 0x0017
#define AECR1G3_PCIE_CFG_FCPTODIS_SIZE 0x0001
#define AECR1G3_PCIE_CFG_ILPPSTWFD_LSB 0x0018
#define AECR1G3_PCIE_CFG_ILPPSTWFD_SIZE 0x0001
#define AECR1G3_PCIE_CFG_TDTMTSFUD_LSB 0x0019
#define AECR1G3_PCIE_CFG_TDTMTSFUD_SIZE 0x0001
#define AECR1G3_PCIE_CFG_ENPTMRC_LSB 0x001a
#define AECR1G3_PCIE_CFG_ENPTMRC_SIZE 0x0001
#define AECR1G3_PCIE_CFG_RSVD_RW3_LSB 0x001b
#define AECR1G3_PCIE_CFG_RSVD_RW3_SIZE 0x0002
#define AECR1G3_PCIE_CFG_IFC2CUP_LSB 0x001d
#define AECR1G3_PCIE_CFG_IFC2CUP_SIZE 0x0001
#define AECR1G3_PCIE_CFG_IFC1CAP_LSB 0x001e
#define AECR1G3_PCIE_CFG_IFC1CAP_SIZE 0x0001
#define AECR1G3_PCIE_CFG_REQTMBOF_LSB 0x001f
#define AECR1G3_PCIE_CFG_REQTMBOF_SIZE 0x0001



/** AECR2G3 desc:
  */

#define AECR2G3_PCIE_CFG_REG             (0x00000c84U)

typedef union {
  struct {
    UINT32 rsvd_rw : 2; /**< Reserved */
                            /* Bits[1:0], default=0x00000000*/
    UINT32 ptmrrptrsplbugfixdis : 1; /**< PTM Response Retry Pointer Sample Bug Fix Disable */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[03:03] */
                            /* Bits[3:3], default=0*/
    UINT32 tqrofd : 1; /**< Transmit Queue Relax Ordering Fix Disable */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[05:05] */
                            /* Bits[5:5], default=0*/
    UINT32 iosfmpagc : 2; /**< IOSF Master Port Arbitration Grand Count */
                            /* Bits[7:6], default=0x00000000*/
    UINT32 rsvd_rw2 : 23; /**< Reserved */
                            /* Bits[30:8], default=0x00000000*/
    UINT32 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[31:31] */
                            /* Bits[31:31], default=0*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} AECR2G3_PCIE_CFG_STRUCT;

#define AECR2G3_PCIE_CFG_WIDTH 32
#define AECR2G3_PCIE_CFG_DEFAULT 0x00000000U
#define AECR2G3_PCIE_CFG_ENDPOINT PCIE_CFG

#define AECR2G3_PCIE_CFG_RSVD_RW_LSB 0x0000
#define AECR2G3_PCIE_CFG_RSVD_RW_SIZE 0x0002
#define AECR2G3_PCIE_CFG_PTMRRPTRSPLBUGFIXDIS_LSB 0x0002
#define AECR2G3_PCIE_CFG_PTMRRPTRSPLBUGFIXDIS_SIZE 0x0001
#define AECR2G3_PCIE_CFG_TQROFD_LSB 0x0004
#define AECR2G3_PCIE_CFG_TQROFD_SIZE 0x0001
#define AECR2G3_PCIE_CFG_IOSFMPAGC_LSB 0x0006
#define AECR2G3_PCIE_CFG_IOSFMPAGC_SIZE 0x0002
#define AECR2G3_PCIE_CFG_RSVD_RW2_LSB 0x0008
#define AECR2G3_PCIE_CFG_RSVD_RW2_SIZE 0x0017



/** LPCRE desc:
  */

#define LPCRE_PCIE_CFG_REG               (0x00000c88U)

typedef union {
  struct {
    UINT32 ipcl : 1; /**< IP Capability Lock */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 7; /**< Undefined - auto filled rsvd_[07:01] */
                            /* Bits[7:1], default=0*/
    UINT32 ipvccapl : 1; /**< IP VC Capability Lock */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 rsvd_1 : 7; /**< Undefined - auto filled rsvd_[15:09] */
                            /* Bits[15:9], default=0*/
    UINT32 rsvd_rwo2 : 1; /**< Reserved */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 rsvd_2 : 7; /**< Undefined - auto filled rsvd_[23:17] */
                            /* Bits[23:17], default=0*/
    UINT32 rsvd_rwo3 : 1; /**< Reserved */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 rsvd_m : 7; /**< Reserved */
                            /* Bits[31:25], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} LPCRE_PCIE_CFG_STRUCT;

#define LPCRE_PCIE_CFG_WIDTH 32
#define LPCRE_PCIE_CFG_DEFAULT 0x00000000U
#define LPCRE_PCIE_CFG_ENDPOINT PCIE_CFG

#define LPCRE_PCIE_CFG_IPCL_LSB 0x0000
#define LPCRE_PCIE_CFG_IPCL_SIZE 0x0001
#define LPCRE_PCIE_CFG_IPVCCAPL_LSB 0x0008
#define LPCRE_PCIE_CFG_IPVCCAPL_SIZE 0x0001
#define LPCRE_PCIE_CFG_RSVD_RWO2_LSB 0x0010
#define LPCRE_PCIE_CFG_RSVD_RWO2_SIZE 0x0001
#define LPCRE_PCIE_CFG_RSVD_RWO3_LSB 0x0018
#define LPCRE_PCIE_CFG_RSVD_RWO3_SIZE 0x0001
#define LPCRE_PCIE_CFG_RSVD_M_LSB 0x0019
#define LPCRE_PCIE_CFG_RSVD_M_SIZE 0x0007



/** LPCR desc:
  */

#define LPCR_PCIE_CFG_REG                (0x00000c8cU)

typedef union {
  struct {
    UINT32 srl : 1; /**< Secured Register Lock */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 7; /**< Undefined - auto filled rsvd_[07:01] */
                            /* Bits[7:1], default=0*/
    UINT32 serl : 1; /**< Secure Equalization Register Lock */
                            /* Bits[8:8], default=0x00000000*/
    UINT32 rsvd_1 : 7; /**< Undefined - auto filled rsvd_[15:09] */
                            /* Bits[15:9], default=0*/
    UINT32 ltrcfglock : 1; /**< LTR Configuration Lock */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 rsvd_2 : 7; /**< Undefined - auto filled rsvd_[23:17] */
                            /* Bits[23:17], default=0*/
    UINT32 didovr_lock : 1; /**< Device ID Override Lock */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 rsvd_m : 7; /**< Reserved */
                            /* Bits[31:25], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} LPCR_PCIE_CFG_STRUCT;

#define LPCR_PCIE_CFG_WIDTH 32
#define LPCR_PCIE_CFG_DEFAULT 0x00000000U
#define LPCR_PCIE_CFG_ENDPOINT PCIE_CFG

#define LPCR_PCIE_CFG_SRL_LSB 0x0000
#define LPCR_PCIE_CFG_SRL_SIZE 0x0001
#define LPCR_PCIE_CFG_SERL_LSB 0x0008
#define LPCR_PCIE_CFG_SERL_SIZE 0x0001
#define LPCR_PCIE_CFG_LTRCFGLOCK_LSB 0x0010
#define LPCR_PCIE_CFG_LTRCFGLOCK_SIZE 0x0001
#define LPCR_PCIE_CFG_DIDOVR_LOCK_LSB 0x0018
#define LPCR_PCIE_CFG_DIDOVR_LOCK_SIZE 0x0001
#define LPCR_PCIE_CFG_RSVD_M_LSB 0x0019
#define LPCR_PCIE_CFG_RSVD_M_SIZE 0x0007



/** RXMC1 desc:
  */

#define RXMC1_PCIE_CFG_REG               (0x00000c90U)

typedef union {
  struct {
    UINT32 miess : 1; /**< Margin Independent Error Sampler Support */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 miudvms : 1; /**< Margin Independent Up Down Voltage Margin Support */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 milrts : 1; /**< Margin Independent Left Right Timing Support */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 mvs : 1; /**< Margin Voltage Support */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 mmnols : 5; /**< Margin Max Number Of Lanes Support */
                            /* Bits[8:4], default=0x00000000*/
    UINT32 msrm : 1; /**< Margin Sample Reporting Method */
                            /* Bits[9:9], default=0x00000000*/
    UINT32 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[10:10] */
                            /* Bits[10:10], default=0*/
    UINT32 vmslop : 2; /**< Voltage Margin Steps Limit On PIPE */
                            /* Bits[12:11], default=0x00000000*/
    UINT32 tmslop : 2; /**< Time Margin Steps Limit On PIPE */
                            /* Bits[14:13], default=0x00000000*/
    UINT32 recfnmrg : 1; /**< Reset Error Count For New Margin */
                            /* Bits[15:15], default=0x00000000*/
    UINT32 bpecrrxmrgna : 1; /**< Block Phy Error Count Reset when Rx Margin not active */
                            /* Bits[16:16], default=0x00000000*/
    UINT32 wfecrfcelc : 1; /**< Wait For Error Count Reset for Clear Error Log Command */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 rsvd : 2; /**< RSVD */
                            /* Bits[19:18], default=0x00000000*/
    UINT32 msrts : 6; /**< Margin Sampling Rate Timing Support */
                            /* Bits[25:20], default=0x00000000*/
    UINT32 msrvs : 6; /**< Margin Sampling Rate Voltage Support */
                            /* Bits[31:26], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RXMC1_PCIE_CFG_STRUCT;

#define RXMC1_PCIE_CFG_WIDTH 32
#define RXMC1_PCIE_CFG_DEFAULT 0x00000000U
#define RXMC1_PCIE_CFG_ENDPOINT PCIE_CFG

#define RXMC1_PCIE_CFG_MIESS_LSB 0x0000
#define RXMC1_PCIE_CFG_MIESS_SIZE 0x0001
#define RXMC1_PCIE_CFG_MIUDVMS_LSB 0x0001
#define RXMC1_PCIE_CFG_MIUDVMS_SIZE 0x0001
#define RXMC1_PCIE_CFG_MILRTS_LSB 0x0002
#define RXMC1_PCIE_CFG_MILRTS_SIZE 0x0001
#define RXMC1_PCIE_CFG_MVS_LSB 0x0003
#define RXMC1_PCIE_CFG_MVS_SIZE 0x0001
#define RXMC1_PCIE_CFG_MMNOLS_LSB 0x0004
#define RXMC1_PCIE_CFG_MMNOLS_SIZE 0x0005
#define RXMC1_PCIE_CFG_MSRM_LSB 0x0009
#define RXMC1_PCIE_CFG_MSRM_SIZE 0x0001
#define RXMC1_PCIE_CFG_VMSLOP_LSB 0x000b
#define RXMC1_PCIE_CFG_VMSLOP_SIZE 0x0002
#define RXMC1_PCIE_CFG_TMSLOP_LSB 0x000d
#define RXMC1_PCIE_CFG_TMSLOP_SIZE 0x0002
#define RXMC1_PCIE_CFG_RECFNMRG_LSB 0x000f
#define RXMC1_PCIE_CFG_RECFNMRG_SIZE 0x0001
#define RXMC1_PCIE_CFG_BPECRRXMRGNA_LSB 0x0010
#define RXMC1_PCIE_CFG_BPECRRXMRGNA_SIZE 0x0001
#define RXMC1_PCIE_CFG_WFECRFCELC_LSB 0x0011
#define RXMC1_PCIE_CFG_WFECRFCELC_SIZE 0x0001
#define RXMC1_PCIE_CFG_RSVD_LSB 0x0012
#define RXMC1_PCIE_CFG_RSVD_SIZE 0x0002
#define RXMC1_PCIE_CFG_MSRTS_LSB 0x0014
#define RXMC1_PCIE_CFG_MSRTS_SIZE 0x0006
#define RXMC1_PCIE_CFG_MSRVS_LSB 0x001a
#define RXMC1_PCIE_CFG_MSRVS_SIZE 0x0006



/** RXMC2 desc:
  */

#define RXMC2_PCIE_CFG_REG               (0x00000c94U)

typedef union {
  struct {
    UINT32 mmvos : 6; /**< Margin Max Voltage Offset Support */
                            /* Bits[5:0], default=0x00000000*/
    UINT32 mnovss : 7; /**< Margin Number Of Voltage Steps Support */
                            /* Bits[12:6], default=0x00000000*/
    UINT32 mmtos : 6; /**< Margin Max Timing Offset Support */
                            /* Bits[18:13], default=0x00000000*/
    UINT32 mnotss : 6; /**< Margin Number Of Timing Steps Support */
                            /* Bits[24:19], default=0x00000000*/
    UINT32 rsvd_m : 7; /**< RSVD */
                            /* Bits[31:25], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RXMC2_PCIE_CFG_STRUCT;

#define RXMC2_PCIE_CFG_WIDTH 32
#define RXMC2_PCIE_CFG_DEFAULT 0x00000000U
#define RXMC2_PCIE_CFG_ENDPOINT PCIE_CFG

#define RXMC2_PCIE_CFG_MMVOS_LSB 0x0000
#define RXMC2_PCIE_CFG_MMVOS_SIZE 0x0006
#define RXMC2_PCIE_CFG_MNOVSS_LSB 0x0006
#define RXMC2_PCIE_CFG_MNOVSS_SIZE 0x0007
#define RXMC2_PCIE_CFG_MMTOS_LSB 0x000d
#define RXMC2_PCIE_CFG_MMTOS_SIZE 0x0006
#define RXMC2_PCIE_CFG_MNOTSS_LSB 0x0013
#define RXMC2_PCIE_CFG_MNOTSS_SIZE 0x0006
#define RXMC2_PCIE_CFG_RSVD_M_LSB 0x0019
#define RXMC2_PCIE_CFG_RSVD_M_SIZE 0x0007



/** EINJCTL desc:
  */

#define EINJCTL_PCIE_CFG_REG             (0x00000ca8U)

typedef union {
  struct {
    UINT32 einjdis : 1; /**< Error Injection Disable */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 injrcverr : 1; /**< Inject Receiver Error */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 injctoerr : 1; /**< Inject Completion Timeout Error */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 rsvd_m : 29; /**< Reserved */
                            /* Bits[31:3], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} EINJCTL_PCIE_CFG_STRUCT;

#define EINJCTL_PCIE_CFG_WIDTH 32
#define EINJCTL_PCIE_CFG_DEFAULT 0x00000000U
#define EINJCTL_PCIE_CFG_ENDPOINT PCIE_CFG

#define EINJCTL_PCIE_CFG_EINJDIS_LSB 0x0000
#define EINJCTL_PCIE_CFG_EINJDIS_SIZE 0x0001
#define EINJCTL_PCIE_CFG_INJRCVERR_LSB 0x0001
#define EINJCTL_PCIE_CFG_INJRCVERR_SIZE 0x0001
#define EINJCTL_PCIE_CFG_INJCTOERR_LSB 0x0002
#define EINJCTL_PCIE_CFG_INJCTOERR_SIZE 0x0001
#define EINJCTL_PCIE_CFG_RSVD_M_LSB 0x0003
#define EINJCTL_PCIE_CFG_RSVD_M_SIZE 0x001d



/** PL16MECH desc:
  */

#define PL16MECH_PCIE_CFG_REG            (0x00000edcU)

typedef union {
  struct {
    UINT32 pciecid : 16; /**< PCI Express Extended Capability ID */
                            /* Bits[15:0], default=0x00000027*/
    UINT32 cv : 4; /**< Capability Version */
                            /* Bits[19:16], default=0x00000001*/
    UINT32 nco : 12; /**< Next Capability Offset */
                            /* Bits[31:20], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PL16MECH_PCIE_CFG_STRUCT;

#define PL16MECH_PCIE_CFG_WIDTH 32
#define PL16MECH_PCIE_CFG_DEFAULT 0x00010027U
#define PL16MECH_PCIE_CFG_ENDPOINT PCIE_CFG

#define PL16MECH_PCIE_CFG_PCIECID_LSB 0x0000
#define PL16MECH_PCIE_CFG_PCIECID_SIZE 0x0010
#define PL16MECH_PCIE_CFG_CV_LSB 0x0010
#define PL16MECH_PCIE_CFG_CV_SIZE 0x0004
#define PL16MECH_PCIE_CFG_NCO_LSB 0x0014
#define PL16MECH_PCIE_CFG_NCO_SIZE 0x000c

/** PL16MPCPSB01 desc:
  */

#define PL16MPCPSB01_PCIE_CFG_REG        (0x00000ee0U)

typedef union {
  struct {
    UINT16 margindrisw : 1; /**< Margining uses Driver Software */
                            /* Bits[0:0], default=0x00000000*/
    UINT16 rsvd_m : 15; /**< Reserved */
                            /* Bits[15:1], default=0x00000000*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} PL16MPCPSB01_PCIE_CFG_STRUCT;

#define PL16MPCPSB01_PCIE_CFG_WIDTH 16
#define PL16MPCPSB01_PCIE_CFG_DEFAULT 0x0000U
#define PL16MPCPSB01_PCIE_CFG_ENDPOINT PCIE_CFG

#define PL16MPCPSB01_PCIE_CFG_MARGINDRISW_LSB 0x0000
#define PL16MPCPSB01_PCIE_CFG_MARGINDRISW_SIZE 0x0001
#define PL16MPCPSB01_PCIE_CFG_RSVD_M_LSB 0x0001
#define PL16MPCPSB01_PCIE_CFG_RSVD_M_SIZE 0x000f

//************************************************ /RegisterStructs
#endif      // _PCIE_CFG_h
