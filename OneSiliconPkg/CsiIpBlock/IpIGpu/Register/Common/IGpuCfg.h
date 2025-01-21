/** @file
  IGPU.h

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

#ifndef _IGPU_h
#define _IGPU_h


// IP GUID: a32ad311-f8a9-4978-a2f9-6d3f171bc86d
#define IGPU_ID 0xa32ad311f8a94978

//************************************************ RegisterStructs


/** VID2_0_2_0_PCI desc:
  */

#define VID2_IGPU_REG                    (0x00000000U)

typedef union {
  struct {
    UINT16 vid : 16; /**< VENDOR IDENTIFICATION NUMBER */
                            /* Bits[15:0], default=0x00008086*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} VID2_IGPU_STRUCT;

#define VID2_IGPU_WIDTH 16
#define VID2_IGPU_DEFAULT 0x8086U
#define VID2_IGPU_ENDPOINT IGPU

#define VID2_IGPU_VID_LSB 0x0000
#define VID2_IGPU_VID_SIZE 0x0010


/** DID2_0_2_0_PCI desc:
  */

#define DID2_IGPU_REG                    (0x00000002U)

typedef union {
  struct {
    UINT16 did : 16; /**< DEVICE IDENTIFICATION NUMBER */
                            /* Bits[15:0], default=0x00000000*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} DID2_IGPU_STRUCT;

#define DID2_IGPU_WIDTH 16
#define DID2_IGPU_DEFAULT 0x0000U
#define DID2_IGPU_ENDPOINT IGPU

#define DID2_IGPU_DID_LSB 0x0000
#define DID2_IGPU_DID_SIZE 0x0010


/** PCICMD_0_2_0_PCI desc:
  */

#define PCICMD_IGPU_REG                  (0x00000004U)

typedef union {
  struct {
    UINT16 ioae : 1; /**< I/O ACCESS ENABLE */
                            /* Bits[0:0], default=0x00000000*/
    UINT16 mae : 1; /**< MEMORY ACCESS ENABLE */
                            /* Bits[1:1], default=0x00000000*/
    UINT16 bme : 1; /**< BUS MASTER ENABLE */
                            /* Bits[2:2], default=0x00000000*/
    UINT16 sce : 1; /**< SPECIAL CYCLE ENABLE */
                            /* Bits[3:3], default=0x00000000*/
    UINT16 mwie : 1; /**< MEMORY WRITE AND INVALIDATE ENABLE */
                            /* Bits[4:4], default=0x00000000*/
    UINT16 vps : 1; /**< VIDEO PALETTE SNOOPING */
                            /* Bits[5:5], default=0x00000000*/
    UINT16 per : 1; /**< PARITY ERROR ENABLE */
                            /* Bits[6:6], default=0x00000000*/
    UINT16 wcc : 1; /**< WAIT CYCLE CONTROL */
                            /* Bits[7:7], default=0x00000000*/
    UINT16 sen : 1; /**< SERR ENABLE */
                            /* Bits[8:8], default=0x00000000*/
    UINT16 fb2b : 1; /**< FAST BACK-TO-BACK */
                            /* Bits[9:9], default=0x00000000*/
    UINT16 intdis : 1; /**< INTERRUPT DISABLE */
                            /* Bits[10:10], default=0x00000000*/
    UINT16 reserved_0 : 5; /**< RESERVED */
                            /* Bits[15:11], default=0x00000000*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} PCICMD_IGPU_STRUCT;

#define PCICMD_IGPU_WIDTH 16
#define PCICMD_IGPU_DEFAULT 0x0000U
#define PCICMD_IGPU_ENDPOINT IGPU

#define PCICMD_IGPU_IOAE_LSB 0x0000
#define PCICMD_IGPU_IOAE_SIZE 0x0001
#define PCICMD_IGPU_MAE_LSB 0x0001
#define PCICMD_IGPU_MAE_SIZE 0x0001
#define PCICMD_IGPU_BME_LSB 0x0002
#define PCICMD_IGPU_BME_SIZE 0x0001
#define PCICMD_IGPU_SCE_LSB 0x0003
#define PCICMD_IGPU_SCE_SIZE 0x0001
#define PCICMD_IGPU_MWIE_LSB 0x0004
#define PCICMD_IGPU_MWIE_SIZE 0x0001
#define PCICMD_IGPU_VPS_LSB 0x0005
#define PCICMD_IGPU_VPS_SIZE 0x0001
#define PCICMD_IGPU_PER_LSB 0x0006
#define PCICMD_IGPU_PER_SIZE 0x0001
#define PCICMD_IGPU_WCC_LSB 0x0007
#define PCICMD_IGPU_WCC_SIZE 0x0001
#define PCICMD_IGPU_SEN_LSB 0x0008
#define PCICMD_IGPU_SEN_SIZE 0x0001
#define PCICMD_IGPU_FB2B_LSB 0x0009
#define PCICMD_IGPU_FB2B_SIZE 0x0001
#define PCICMD_IGPU_INTDIS_LSB 0x000a
#define PCICMD_IGPU_INTDIS_SIZE 0x0001
#define PCICMD_IGPU_RESERVED_0_LSB 0x000b
#define PCICMD_IGPU_RESERVED_0_SIZE 0x0005


/** RID2_CC_0_2_0_PCI desc:
  */

#define RID2_CC_IGPU_REG                 (0x00000008U)

typedef union {
  struct {
    UINT32 rid : 8; /**< REVISION IDENTIFICATION NUMBER */
                            /* Bits[7:0], default=0x00000000*/
    UINT32 pi : 8; /**< PROGRAMMING INTERFACE */
                            /* Bits[15:8], default=0x00000000*/
    UINT32 subcc : 8; /**< SUB-CLASS CODE */
                            /* Bits[23:16], default=0x00000000*/
    UINT32 bcc : 8; /**< BASE CLASS CODE */
                            /* Bits[31:24], default=0x00000003*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RID2_CC_IGPU_STRUCT;

#define RID2_CC_IGPU_WIDTH 32
#define RID2_CC_IGPU_DEFAULT 0x03000000U
#define RID2_CC_IGPU_ENDPOINT IGPU

#define RID2_CC_IGPU_RID_LSB 0x0000
#define RID2_CC_IGPU_RID_SIZE 0x0008
#define RID2_CC_IGPU_PI_LSB 0x0008
#define RID2_CC_IGPU_PI_SIZE 0x0008
#define RID2_CC_IGPU_SUBCC_LSB 0x0010
#define RID2_CC_IGPU_SUBCC_SIZE 0x0008
#define RID2_CC_IGPU_BCC_LSB 0x0018
#define RID2_CC_IGPU_BCC_SIZE 0x0008


/** GTTMMADR0_0_2_0_PCI desc:
  */

#define GTTMMADR0_IGPU_REG               (0x00000010U)

typedef union {
  struct {
    UINT32 mios : 1; /**< MEMORY/IO SPACE */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 memtyp : 2; /**< MEMORY TYPE */
                            /* Bits[2:1], default=0x00000002*/
    UINT32 prefmem : 1; /**< PREFETCHABLE_MEMORY */
                            /* Bits[3:3], default=0x00000001*/
    UINT32 adm : 20; /**< ADDRESS MASK */
                            /* Bits[23:4], default=0x00000000*/
    UINT32 mbat_0 : 1; /**< MEMORY BASE ADDRESS TILE 0 */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 mbat_1 : 1; /**< MEMORY BASE ADDRESS TILE 1 */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 mba_0 : 6; /**< MEMORY BASE ADDRESS0 */
                            /* Bits[31:26], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GTTMMADR0_IGPU_STRUCT;

#define GTTMMADR0_IGPU_WIDTH 32
#define GTTMMADR0_IGPU_DEFAULT 0x0000000cU
#define GTTMMADR0_IGPU_ENDPOINT IGPU

#define GTTMMADR0_IGPU_MIOS_LSB 0x0000
#define GTTMMADR0_IGPU_MIOS_SIZE 0x0001
#define GTTMMADR0_IGPU_MEMTYP_LSB 0x0001
#define GTTMMADR0_IGPU_MEMTYP_SIZE 0x0002
#define GTTMMADR0_IGPU_PREFMEM_LSB 0x0003
#define GTTMMADR0_IGPU_PREFMEM_SIZE 0x0001
#define GTTMMADR0_IGPU_ADM_LSB 0x0004
#define GTTMMADR0_IGPU_ADM_SIZE 0x0014
#define GTTMMADR0_IGPU_MBAT_0_LSB 0x0018
#define GTTMMADR0_IGPU_MBAT_0_SIZE 0x0001
#define GTTMMADR0_IGPU_MBAT_1_LSB 0x0019
#define GTTMMADR0_IGPU_MBAT_1_SIZE 0x0001
#define GTTMMADR0_IGPU_MBA_0_LSB 0x001a
#define GTTMMADR0_IGPU_MBA_0_SIZE 0x0006


/** LMEMBAR0_0_2_0_PCI desc:
  */

#define LMEMBAR0_IGPU_REG                (0x00000018U)

typedef union {
  struct {
    UINT32 mios : 1; /**< MEMORY/IO SPACE */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 memtyp : 2; /**< MEMORY TYPE */
                            /* Bits[2:1], default=0x00000002*/
    UINT32 prefmem : 1; /**< PREFETCHABLE MEMORY */
                            /* Bits[3:3], default=0x00000001*/
    UINT32 adm : 23; /**< ADDRESS MASK */
                            /* Bits[26:4], default=0x00000000*/
    UINT32 admsk256 : 1; /**< 256 MB ADDRESS MASK */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 admsk512 : 1; /**< 512MB ADDRESS MASK */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 admsk1gb : 1; /**< 1 GB ADDRESS MASK */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 admsk2gb : 1; /**< 2 GB ADDRESS MASK */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 admsk4gb : 1; /**< 4 GB ADDRESS MASK */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} LMEMBAR0_IGPU_STRUCT;

#define LMEMBAR0_IGPU_WIDTH 32
#define LMEMBAR0_IGPU_DEFAULT 0x0000000cU
#define LMEMBAR0_IGPU_ENDPOINT IGPU

#define LMEMBAR0_IGPU_MIOS_LSB 0x0000
#define LMEMBAR0_IGPU_MIOS_SIZE 0x0001
#define LMEMBAR0_IGPU_MEMTYP_LSB 0x0001
#define LMEMBAR0_IGPU_MEMTYP_SIZE 0x0002
#define LMEMBAR0_IGPU_PREFMEM_LSB 0x0003
#define LMEMBAR0_IGPU_PREFMEM_SIZE 0x0001
#define LMEMBAR0_IGPU_ADM_LSB 0x0004
#define LMEMBAR0_IGPU_ADM_SIZE 0x0017
#define LMEMBAR0_IGPU_ADMSK256_LSB 0x001b
#define LMEMBAR0_IGPU_ADMSK256_SIZE 0x0001
#define LMEMBAR0_IGPU_ADMSK512_LSB 0x001c
#define LMEMBAR0_IGPU_ADMSK512_SIZE 0x0001
#define LMEMBAR0_IGPU_ADMSK1GB_LSB 0x001d
#define LMEMBAR0_IGPU_ADMSK1GB_SIZE 0x0001
#define LMEMBAR0_IGPU_ADMSK2GB_LSB 0x001e
#define LMEMBAR0_IGPU_ADMSK2GB_SIZE 0x0001
#define LMEMBAR0_IGPU_ADMSK4GB_LSB 0x001f
#define LMEMBAR0_IGPU_ADMSK4GB_SIZE 0x0001


/** SVID2_0_2_0_PCI desc:
  */

#define SVID2_IGPU_REG                   (0x0000002cU)

typedef union {
  struct {
    UINT16 subvid : 16; /**< SUBSYSTEM VENDOR ID */
                            /* Bits[15:0], default=0x00008086*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SVID2_IGPU_STRUCT;

#define SVID2_IGPU_WIDTH 16
#define SVID2_IGPU_DEFAULT 0x8086U
#define SVID2_IGPU_ENDPOINT IGPU

#define SVID2_IGPU_SUBVID_LSB 0x0000
#define SVID2_IGPU_SUBVID_SIZE 0x0010


/** SID2_0_2_0_PCI desc:
  */

#define SID2_IGPU_REG                    (0x0000002eU)

typedef union {
  struct {
    UINT16 subid : 16; /**< SUBSYSTEM  ID */
                            /* Bits[15:0], default=0x00000000*/
  }     Bits;
  UINT16 Data;
  UINT8  Data8[2];
} SID2_IGPU_STRUCT;

#define SID2_IGPU_WIDTH 16
#define SID2_IGPU_DEFAULT 0x0000U
#define SID2_IGPU_ENDPOINT IGPU

#define SID2_IGPU_SUBID_LSB 0x0000
#define SID2_IGPU_SUBID_SIZE 0x0010


/** PAVPC0_0_2_0_PCI desc:
  */

#define PAVPC0_IGPU_REG                  (0x000000f0U)

typedef union {
  struct {
    UINT32 pcme : 1; /**< PCM ENABLE */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 pavpe : 1; /**< PAVP ENABLE */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 lock : 1; /**< LOCK */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 spare_3 : 1; /**< SPARE_3 */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 ovtattack : 1; /**< OVERRIDE TERMINATE ATTACK */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 vdboxresetcountercheckindisable : 1; /**< VDBOX RESET COUNTER CHECKIN DISABLE */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 kcr_reset_fix : 1; /**< KCR RESET FIX */
                            /* Bits[6:6], default=0x00000001*/
    UINT32 wopcmsize : 3; /**< WOPCM SIZE */
                            /* Bits[9:7], default=0x00000003*/
    UINT32 reserved_1 : 1; /**< RESERVED */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 sam_wopcmsize : 3; /**< SA MEDIA WOPCM SIZE */
                            /* Bits[13:11], default=0x00000004*/
    UINT32 reserved_0 : 6; /**< RESERVED */
                            /* Bits[19:14], default=0x00000000*/
    UINT32 wopcmbase_lsb : 12; /**< WOPCM BASE LSB */
                            /* Bits[31:20], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PAVPC0_IGPU_STRUCT;

#define PAVPC0_IGPU_WIDTH 32
#define PAVPC0_IGPU_DEFAULT 0x000021c0U
#define PAVPC0_IGPU_ENDPOINT IGPU

#define PAVPC0_IGPU_PCME_LSB 0x0000
#define PAVPC0_IGPU_PCME_SIZE 0x0001
#define PAVPC0_IGPU_PAVPE_LSB 0x0001
#define PAVPC0_IGPU_PAVPE_SIZE 0x0001
#define PAVPC0_IGPU_LOCK_LSB 0x0002
#define PAVPC0_IGPU_LOCK_SIZE 0x0001
#define PAVPC0_IGPU_SPARE_3_LSB 0x0003
#define PAVPC0_IGPU_SPARE_3_SIZE 0x0001
#define PAVPC0_IGPU_OVTATTACK_LSB 0x0004
#define PAVPC0_IGPU_OVTATTACK_SIZE 0x0001
#define PAVPC0_IGPU_VDBOXRESETCOUNTERCHECKINDISABLE_LSB 0x0005
#define PAVPC0_IGPU_VDBOXRESETCOUNTERCHECKINDISABLE_SIZE 0x0001
#define PAVPC0_IGPU_KCR_RESET_FIX_LSB 0x0006
#define PAVPC0_IGPU_KCR_RESET_FIX_SIZE 0x0001
#define PAVPC0_IGPU_WOPCMSIZE_LSB 0x0007
#define PAVPC0_IGPU_WOPCMSIZE_SIZE 0x0003
#define PAVPC0_IGPU_RESERVED_1_LSB 0x000a
#define PAVPC0_IGPU_RESERVED_1_SIZE 0x0001
#define PAVPC0_IGPU_SAM_WOPCMSIZE_LSB 0x000b
#define PAVPC0_IGPU_SAM_WOPCMSIZE_SIZE 0x0003
#define PAVPC0_IGPU_RESERVED_0_LSB 0x000e
#define PAVPC0_IGPU_RESERVED_0_SIZE 0x0006
#define PAVPC0_IGPU_WOPCMBASE_LSB_LSB 0x0014
#define PAVPC0_IGPU_WOPCMBASE_LSB_SIZE 0x000c


/** ASLS_0_2_0_PCI desc:
  */

#define ASLS_IGPU_REG                    (0x000000fcU)

typedef union {
  struct {
    UINT32 dss : 32; /**< DEVICE SWITCHING STORAGE */
                            /* Bits[31:0], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} ASLS_IGPU_STRUCT;

#define ASLS_IGPU_WIDTH 32
#define ASLS_IGPU_DEFAULT 0x00000000U
#define ASLS_IGPU_ENDPOINT IGPU

#define ASLS_IGPU_DSS_LSB 0x0000
#define ASLS_IGPU_DSS_SIZE 0x0020


/** GU_CNTL_PROTECTED desc:
  */

#define GU_CNTL_PROTECTED_IGPU_REG       (0x0010100cU)

typedef union {
  struct {
    UINT32 lp_nonsnoop : 1; /**< LP NON-SNOOP */
                            /* Bits[0:0], default=0x00000001*/
    UINT32 vtdfaultfix_dis : 1; /**< VTD FAULT FIX DISABLE */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 blkiosfsb_on_pmblk : 1; /**< BLOCK IOSF SB ON PM BLOCK */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 spare3 : 1; /**< SPARE3 */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 widi_chk_dis : 1; /**< WIDI_CHK_DIS */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 bar0_pref : 1; /**< BAR0_PREF */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 spare_bit_6 : 1; /**< SPARE6 */
                            /* Bits[6:6], default=0x00000000*/
    UINT32 spareb : 1; /**< SPAREB */
                            /* Bits[7:7], default=0x00000000*/
    UINT32 gtpresent : 1; /**< GTPRESENT */
                            /* Bits[8:8], default=0x00000001*/
    UINT32 depresent : 1; /**< DEPRESENT */
                            /* Bits[9:9], default=0x00000001*/
    UINT32 impresent : 1; /**< IMPRESENT */
                            /* Bits[10:10], default=0x00000001*/
    UINT32 soc_viral_mask : 1; /**< SOC VIRAL WIRE MASK */
                            /* Bits[11:11], default=0x00000000*/
    UINT32 soc_gate_viral_usp : 1; /**< SOC GATE VIRAL TO USP */
                            /* Bits[12:12], default=0x00000000*/
    UINT32 erase_ord_en : 1; /**< SPI ERASE ORDERING ENABLE */
                            /* Bits[13:13], default=0x00000000*/
    UINT32 spare1614 : 3; /**< SPARE1614 */
                            /* Bits[16:14], default=0x00000000*/
    UINT32 stolen_mem_wipe_dis : 1; /**< STOLEN MEMORY WIPE */
                            /* Bits[17:17], default=0x00000000*/
    UINT32 pmrsp_auto_debug_enable : 1; /**< PMRSP AUTO DEBUG ENABLE */
                            /* Bits[18:18], default=0x00000000*/
    UINT32 pmrsp_auto : 1; /**< PMRSP AUTO DEBUG MODE */
                            /* Bits[19:19], default=0x00000000*/
    UINT32 disable_sbgp_block : 1; /**< DISABLE SBGP BLOCK */
                            /* Bits[20:20], default=0x00000000*/
    UINT32 disable_cdie_block : 1; /**< DISABLE CDIE BLOCK */
                            /* Bits[21:21], default=0x00000000*/
    UINT32 sparea : 8; /**< SPARE29 */
                            /* Bits[29:22], default=0x00000000*/
    UINT32 driverint_pcicmdmask_dis : 1; /**< DRIVERINT_PCICMDMASK_DIS */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 driverint_dis : 1; /**< DRIVERINT_DIS */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GU_CNTL_PROTECTED_IGPU_STRUCT;

#define GU_CNTL_PROTECTED_IGPU_WIDTH 32
#define GU_CNTL_PROTECTED_IGPU_DEFAULT 0x00000701U
#define GU_CNTL_PROTECTED_IGPU_ENDPOINT IGPU

#define GU_CNTL_PROTECTED_IGPU_LP_NONSNOOP_LSB 0x0000
#define GU_CNTL_PROTECTED_IGPU_LP_NONSNOOP_SIZE 0x0001
#define GU_CNTL_PROTECTED_IGPU_VTDFAULTFIX_DIS_LSB 0x0001
#define GU_CNTL_PROTECTED_IGPU_VTDFAULTFIX_DIS_SIZE 0x0001
#define GU_CNTL_PROTECTED_IGPU_BLKIOSFSB_ON_PMBLK_LSB 0x0002
#define GU_CNTL_PROTECTED_IGPU_BLKIOSFSB_ON_PMBLK_SIZE 0x0001
#define GU_CNTL_PROTECTED_IGPU_SPARE3_LSB 0x0003
#define GU_CNTL_PROTECTED_IGPU_SPARE3_SIZE 0x0001
#define GU_CNTL_PROTECTED_IGPU_WIDI_CHK_DIS_LSB 0x0004
#define GU_CNTL_PROTECTED_IGPU_WIDI_CHK_DIS_SIZE 0x0001
#define GU_CNTL_PROTECTED_IGPU_BAR0_PREF_LSB 0x0005
#define GU_CNTL_PROTECTED_IGPU_BAR0_PREF_SIZE 0x0001
#define GU_CNTL_PROTECTED_IGPU_SPARE_BIT_6_LSB 0x0006
#define GU_CNTL_PROTECTED_IGPU_SPARE_BIT_6_SIZE 0x0001
#define GU_CNTL_PROTECTED_IGPU_SPAREB_LSB 0x0007
#define GU_CNTL_PROTECTED_IGPU_SPAREB_SIZE 0x0001
#define GU_CNTL_PROTECTED_IGPU_GTPRESENT_LSB 0x0008
#define GU_CNTL_PROTECTED_IGPU_GTPRESENT_SIZE 0x0001
#define GU_CNTL_PROTECTED_IGPU_DEPRESENT_LSB 0x0009
#define GU_CNTL_PROTECTED_IGPU_DEPRESENT_SIZE 0x0001
#define GU_CNTL_PROTECTED_IGPU_IMPRESENT_LSB 0x000a
#define GU_CNTL_PROTECTED_IGPU_IMPRESENT_SIZE 0x0001
#define GU_CNTL_PROTECTED_IGPU_SOC_VIRAL_MASK_LSB 0x000b
#define GU_CNTL_PROTECTED_IGPU_SOC_VIRAL_MASK_SIZE 0x0001
#define GU_CNTL_PROTECTED_IGPU_SOC_GATE_VIRAL_USP_LSB 0x000c
#define GU_CNTL_PROTECTED_IGPU_SOC_GATE_VIRAL_USP_SIZE 0x0001
#define GU_CNTL_PROTECTED_IGPU_ERASE_ORD_EN_LSB 0x000d
#define GU_CNTL_PROTECTED_IGPU_ERASE_ORD_EN_SIZE 0x0001
#define GU_CNTL_PROTECTED_IGPU_SPARE1614_LSB 0x000e
#define GU_CNTL_PROTECTED_IGPU_SPARE1614_SIZE 0x0003
#define GU_CNTL_PROTECTED_IGPU_STOLEN_MEM_WIPE_DIS_LSB 0x0011
#define GU_CNTL_PROTECTED_IGPU_STOLEN_MEM_WIPE_DIS_SIZE 0x0001
#define GU_CNTL_PROTECTED_IGPU_PMRSP_AUTO_DEBUG_ENABLE_LSB 0x0012
#define GU_CNTL_PROTECTED_IGPU_PMRSP_AUTO_DEBUG_ENABLE_SIZE 0x0001
#define GU_CNTL_PROTECTED_IGPU_PMRSP_AUTO_LSB 0x0013
#define GU_CNTL_PROTECTED_IGPU_PMRSP_AUTO_SIZE 0x0001
#define GU_CNTL_PROTECTED_IGPU_DISABLE_SBGP_BLOCK_LSB 0x0014
#define GU_CNTL_PROTECTED_IGPU_DISABLE_SBGP_BLOCK_SIZE 0x0001
#define GU_CNTL_PROTECTED_IGPU_DISABLE_CDIE_BLOCK_LSB 0x0015
#define GU_CNTL_PROTECTED_IGPU_DISABLE_CDIE_BLOCK_SIZE 0x0001
#define GU_CNTL_PROTECTED_IGPU_SPAREA_LSB 0x0016
#define GU_CNTL_PROTECTED_IGPU_SPAREA_SIZE 0x0008
#define GU_CNTL_PROTECTED_IGPU_DRIVERINT_PCICMDMASK_DIS_LSB 0x001e
#define GU_CNTL_PROTECTED_IGPU_DRIVERINT_PCICMDMASK_DIS_SIZE 0x0001
#define GU_CNTL_PROTECTED_IGPU_DRIVERINT_DIS_LSB 0x001f
#define GU_CNTL_PROTECTED_IGPU_DRIVERINT_DIS_SIZE 0x0001


/** CSMEBDF desc:
  */

#define CSMEBDF_IGPU_REG                 (0x00101070U)

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
    UINT32 reserved_0 : 13; /**< RESERVED */
                            /* Bits[31:19], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CSMEBDF_IGPU_STRUCT;

#define CSMEBDF_IGPU_WIDTH 32
#define CSMEBDF_IGPU_DEFAULT 0x000700b0U
#define CSMEBDF_IGPU_ENDPOINT IGPU

#define CSMEBDF_IGPU_FUNNUM_LSB 0x0000
#define CSMEBDF_IGPU_FUNNUM_SIZE 0x0003
#define CSMEBDF_IGPU_DEVNUM_LSB 0x0003
#define CSMEBDF_IGPU_DEVNUM_SIZE 0x0005
#define CSMEBDF_IGPU_BUSNUM_LSB 0x0008
#define CSMEBDF_IGPU_BUSNUM_SIZE 0x0008
#define CSMEBDF_IGPU_BARNUM_LSB 0x0010
#define CSMEBDF_IGPU_BARNUM_SIZE 0x0003
#define CSMEBDF_IGPU_RESERVED_0_LSB 0x0013
#define CSMEBDF_IGPU_RESERVED_0_SIZE 0x000d


/** GGC desc:
  */

#define GGC_IGPU_REG                     (0x00108040U)

typedef union {
  struct {
    UINT32 spare : 1; /**< SPARE */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 ivd : 1; /**< IVD */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 vamen : 1; /**< VAMEN */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 reserved_1 : 3; /**< RESERVED */
                            /* Bits[5:3], default=0x00000000*/
    UINT32 ggms : 2; /**< GGMS */
                            /* Bits[7:6], default=0x00000003*/
    UINT32 gms : 8; /**< GMS */
                            /* Bits[15:8], default=0x00000002*/
    UINT32 reserved_0 : 16; /**< RESERVED */
                            /* Bits[31:16], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GGC_IGPU_STRUCT;

#define GGC_IGPU_WIDTH 32
#define GGC_IGPU_DEFAULT 0x000002c0U
#define GGC_IGPU_ENDPOINT IGPU

#define GGC_IGPU_SPARE_LSB 0x0000
#define GGC_IGPU_SPARE_SIZE 0x0001
#define GGC_IGPU_IVD_LSB 0x0001
#define GGC_IGPU_IVD_SIZE 0x0001
#define GGC_IGPU_VAMEN_LSB 0x0002
#define GGC_IGPU_VAMEN_SIZE 0x0001
#define GGC_IGPU_RESERVED_1_LSB 0x0003
#define GGC_IGPU_RESERVED_1_SIZE 0x0003
#define GGC_IGPU_GGMS_LSB 0x0006
#define GGC_IGPU_GGMS_SIZE 0x0002
#define GGC_IGPU_GMS_LSB 0x0008
#define GGC_IGPU_GMS_SIZE 0x0008
#define GGC_IGPU_RESERVED_0_LSB 0x0010
#define GGC_IGPU_RESERVED_0_SIZE 0x0010


/** DSMBASE0 desc:
  */

#define DSMBASE0_IGPU_REG                (0x001080c0U)

typedef union {
  struct {
    UINT32 spare : 1; /**< SPARE */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 reserved_0 : 19; /**< RESERVED */
                            /* Bits[19:1], default=0x00000000*/
    UINT32 bdsm_0 : 12; /**< BDSM */
                            /* Bits[31:20], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DSMBASE0_IGPU_STRUCT;

#define DSMBASE0_IGPU_WIDTH 32
#define DSMBASE0_IGPU_DEFAULT 0x00000000U
#define DSMBASE0_IGPU_ENDPOINT IGPU

#define DSMBASE0_IGPU_SPARE_LSB 0x0000
#define DSMBASE0_IGPU_SPARE_SIZE 0x0001
#define DSMBASE0_IGPU_RESERVED_0_LSB 0x0001
#define DSMBASE0_IGPU_RESERVED_0_SIZE 0x0013
#define DSMBASE0_IGPU_BDSM_0_LSB 0x0014
#define DSMBASE0_IGPU_BDSM_0_SIZE 0x000c


/** GSMBASE0 desc:
  */

#define GSMBASE0_IGPU_REG                (0x00108100U)

typedef union {
  struct {
    UINT32 spare : 1; /**< SPARE */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 reserved_0 : 19; /**< RESERVED */
                            /* Bits[19:1], default=0x00000000*/
    UINT32 bgsm_lsb : 12; /**< BGSM_LSB */
                            /* Bits[31:20], default=0x00000001*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GSMBASE0_IGPU_STRUCT;

#define GSMBASE0_IGPU_WIDTH 32
#define GSMBASE0_IGPU_DEFAULT 0x00100000U
#define GSMBASE0_IGPU_ENDPOINT IGPU

#define GSMBASE0_IGPU_SPARE_LSB 0x0000
#define GSMBASE0_IGPU_SPARE_SIZE 0x0001
#define GSMBASE0_IGPU_RESERVED_0_LSB 0x0001
#define GSMBASE0_IGPU_RESERVED_0_SIZE 0x0013
#define GSMBASE0_IGPU_BGSM_LSB_LSB 0x0014
#define GSMBASE0_IGPU_BGSM_LSB_SIZE 0x000c


/** PAVPC0_REG desc:
  */

#define PAVPC0_REG_IGPU_REG              (0x001082c0U)

typedef union {
  struct {
    UINT32 pcme : 1; /**< PCM ENABLE */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 pavpe : 1; /**< PAVP ENABLE */
                            /* Bits[1:1], default=0x00000000*/
    UINT32 lock : 1; /**< LOCK */
                            /* Bits[2:2], default=0x00000000*/
    UINT32 spare_3 : 1; /**< SPARE_3 */
                            /* Bits[3:3], default=0x00000000*/
    UINT32 ovtattack : 1; /**< OVERRIDE TERMINATE ATTACK */
                            /* Bits[4:4], default=0x00000000*/
    UINT32 vdboxresetcountercheckindisable : 1; /**< VDBOX RESET COUNTER CHECKIN DISABLE */
                            /* Bits[5:5], default=0x00000000*/
    UINT32 kcr_reset_fix : 1; /**< KCR RESET FIX */
                            /* Bits[6:6], default=0x00000001*/
    UINT32 wopcm_size : 3; /**< WOPCM SIZE */
                            /* Bits[9:7], default=0x00000003*/
    UINT32 reserved_1 : 1; /**< RESERVED */
                            /* Bits[10:10], default=0x00000000*/
    UINT32 sa_media_wopcm_size : 3; /**< SA MEDIA WOPCM SIZE */
                            /* Bits[13:11], default=0x00000004*/
    UINT32 reserved_0 : 6; /**< RESERVED */
                            /* Bits[19:14], default=0x00000000*/
    UINT32 wopcmbase_0 : 12; /**< WOPCMBASE */
                            /* Bits[31:20], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PAVPC0_REG_IGPU_STRUCT;

#define PAVPC0_REG_IGPU_WIDTH 32
#define PAVPC0_REG_IGPU_DEFAULT 0x000021c0U
#define PAVPC0_REG_IGPU_ENDPOINT IGPU

#define PAVPC0_REG_IGPU_PCME_LSB 0x0000
#define PAVPC0_REG_IGPU_PCME_SIZE 0x0001
#define PAVPC0_REG_IGPU_PAVPE_LSB 0x0001
#define PAVPC0_REG_IGPU_PAVPE_SIZE 0x0001
#define PAVPC0_REG_IGPU_LOCK_LSB 0x0002
#define PAVPC0_REG_IGPU_LOCK_SIZE 0x0001
#define PAVPC0_REG_IGPU_SPARE_3_LSB 0x0003
#define PAVPC0_REG_IGPU_SPARE_3_SIZE 0x0001
#define PAVPC0_REG_IGPU_OVTATTACK_LSB 0x0004
#define PAVPC0_REG_IGPU_OVTATTACK_SIZE 0x0001
#define PAVPC0_REG_IGPU_VDBOXRESETCOUNTERCHECKINDISABLE_LSB 0x0005
#define PAVPC0_REG_IGPU_VDBOXRESETCOUNTERCHECKINDISABLE_SIZE 0x0001
#define PAVPC0_REG_IGPU_KCR_RESET_FIX_LSB 0x0006
#define PAVPC0_REG_IGPU_KCR_RESET_FIX_SIZE 0x0001
#define PAVPC0_REG_IGPU_WOPCM_SIZE_LSB 0x0007
#define PAVPC0_REG_IGPU_WOPCM_SIZE_SIZE 0x0003
#define PAVPC0_REG_IGPU_RESERVED_1_LSB 0x000a
#define PAVPC0_REG_IGPU_RESERVED_1_SIZE 0x0001
#define PAVPC0_REG_IGPU_SA_MEDIA_WOPCM_SIZE_LSB 0x000b
#define PAVPC0_REG_IGPU_SA_MEDIA_WOPCM_SIZE_SIZE 0x0003
#define PAVPC0_REG_IGPU_RESERVED_0_LSB 0x000e
#define PAVPC0_REG_IGPU_RESERVED_0_SIZE 0x0006
#define PAVPC0_REG_IGPU_WOPCMBASE_0_LSB 0x0014
#define PAVPC0_REG_IGPU_WOPCMBASE_0_SIZE 0x000c

//************************************************ /RegisterStructs


#endif      // _IGPU_h
