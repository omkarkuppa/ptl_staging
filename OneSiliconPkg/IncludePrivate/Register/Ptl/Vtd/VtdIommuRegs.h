/** @file
  VTD_IOMMU.h

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
/* IOMMU_STANDARD                                                               */
/*  Security_ReadAccess_Str:                                                    */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI | ESE_SAI |             */
/*    PM_IOSS_SAI | DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |    */
/*    DFX_THIRDPARTY_SAI                                                        */
/*  Security_WriteAccess_Str:                                                   */
/*    HOSTIA_POSTBOOT_SAI | HOSTIA_UCODE_SAI | HOSTIA_SMM_SAI |                 */
/*    HOSTIA_SUNPASS_SAI | HOSTIA_BOOT_SAI | PM_PCS_SAI | ESE_SAI |             */
/*    PM_IOSS_SAI | DFX_INTEL_MANUFACTURING_SAI | DFX_INTEL_PRODUCTION_SAI |    */
/*    DFX_THIRDPARTY_SAI                                                        */

#ifndef _VTD_IOMMU_PTL_CDIE_H_
#define _VTD_IOMMU_PTL_CDIE_H_

#include <Base.h>


// IP GUID: 53ead291-e2ed-4052-b540-0b79643067c6
#define VTD_IOMMU_ID 0x53ead291e2ed4052

//************************************************ RegisterStructs


/** CAP_VTD_IOMMU desc:
  */

#define CAP_VTD_IOMMU_REG                (0x00000008U)

typedef union {
  struct {
    UINT64 nd : 3; /**< Number of Domains Supported */
                            /* Bits[2:0], default=0x00000002*/
    UINT64 afl : 1; /**< Advanced Fault Logging */
                            /* Bits[3:3], default=0x00000000*/
    UINT64 rwbf : 1; /**< Required Write-Buffer Flushing */
                            /* Bits[4:4], default=0x00000000*/
    UINT64 plmr : 1; /**< Protected Low-Memory Region */
                            /* Bits[5:5], default=0x00000000*/
    UINT64 phmr : 1; /**< Protected High-Memory Region */
                            /* Bits[6:6], default=0x00000000*/
    UINT64 cm : 1; /**< Caching Mode */
                            /* Bits[7:7], default=0x00000000*/
    UINT64 sagaw : 5; /**< Supported Adjusted Guest Address Widths */
                            /* Bits[12:8], default=0x00000004*/
    UINT64 rsvd_0 : 3; /**< Undefined - auto filled rsvd_[15:13] */
                            /* Bits[15:13], default=None*/
    UINT64 mgaw : 6; /**< Maximum Guest Address Width */
                            /* Bits[21:16], default=0x00000029*/
    UINT64 zlr : 1; /**< Zero Length Read */
                            /* Bits[22:22], default=0x00000001*/
    UINT64 rsvd_1 : 1; /**< Undefined - auto filled rsvd_[23:23] */
                            /* Bits[23:23], default=None*/
    UINT64 fro : 10; /**< Fault-Recording Register Offset */
                            /* Bits[33:24], default=0x000000EE*/
    UINT64 sllps : 4; /**< Second Level Large Page Support */
                            /* Bits[37:34], default=0x00000003*/
    UINT64 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[38:38] */
                            /* Bits[38:38], default=None*/
    UINT64 psi : 1; /**< Page Selective Invalidation */
                            /* Bits[39:39], default=0x00000001*/
    UINT64 nfr : 8; /**< Number of Fault-Recording Registers */
                            /* Bits[47:40], default=0x00000000*/
    UINT64 mamv : 6; /**< Maximum Address Mask Value */
                            /* Bits[53:48], default=0x0000001E*/
    UINT64 dwd : 1; /**< Write Draining */
                            /* Bits[54:54], default=0x00000001*/
    UINT64 drd : 1; /**< Read Draining */
                            /* Bits[55:55], default=0x00000001*/
    UINT64 fl1gp : 1; /**< First Level 1-GByte Page Support */
                            /* Bits[56:56], default=0x00000001*/
    UINT64 rsvd_3 : 2; /**< Undefined - auto filled rsvd_[58:57] */
                            /* Bits[58:57], default=None*/
    UINT64 pi : 1; /**< Posted Interrupt Support */
                            /* Bits[59:59], default=0x00000001*/
    UINT64 fl5lp : 1; /**< First Level 5-level Paging */
                            /* Bits[60:60], default=0x00000000*/
    UINT64 ecmds : 1; /**< Enhanced Command Support */
                            /* Bits[61:61], default=0x00000001*/
    UINT64 esirtps : 1; /**< Enhanced Set Interrupt Root Table Pointer Support */
                            /* Bits[62:62], default=0x00000001*/
    UINT64 esrtps : 1; /**< Enhanced Set Root Table Pointer Support */
                            /* Bits[63:63], default=0x00000001*/
  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} CAP_VTD_IOMMU_STRUCT;

#define CAP_VTD_IOMMU_WIDTH 64

#define CAP_VTD_IOMMU_ND_LSB 0x0000
#define CAP_VTD_IOMMU_ND_SIZE 0x0003
#define CAP_VTD_IOMMU_AFL_LSB 0x0003
#define CAP_VTD_IOMMU_AFL_SIZE 0x0001
#define CAP_VTD_IOMMU_RWBF_LSB 0x0004
#define CAP_VTD_IOMMU_RWBF_SIZE 0x0001
#define CAP_VTD_IOMMU_PLMR_LSB 0x0005
#define CAP_VTD_IOMMU_PLMR_SIZE 0x0001
#define CAP_VTD_IOMMU_PHMR_LSB 0x0006
#define CAP_VTD_IOMMU_PHMR_SIZE 0x0001
#define CAP_VTD_IOMMU_CM_LSB 0x0007
#define CAP_VTD_IOMMU_CM_SIZE 0x0001
#define CAP_VTD_IOMMU_SAGAW_LSB 0x0008
#define CAP_VTD_IOMMU_SAGAW_SIZE 0x0005
#define CAP_VTD_IOMMU_MGAW_LSB 0x0010
#define CAP_VTD_IOMMU_MGAW_SIZE 0x0006
#define CAP_VTD_IOMMU_ZLR_LSB 0x0016
#define CAP_VTD_IOMMU_ZLR_SIZE 0x0001
#define CAP_VTD_IOMMU_FRO_LSB 0x0018
#define CAP_VTD_IOMMU_FRO_SIZE 0x000a
#define CAP_VTD_IOMMU_SLLPS_LSB 0x0022
#define CAP_VTD_IOMMU_SLLPS_SIZE 0x0004
#define CAP_VTD_IOMMU_PSI_LSB 0x0027
#define CAP_VTD_IOMMU_PSI_SIZE 0x0001
#define CAP_VTD_IOMMU_NFR_LSB 0x0028
#define CAP_VTD_IOMMU_NFR_SIZE 0x0008
#define CAP_VTD_IOMMU_MAMV_LSB 0x0030
#define CAP_VTD_IOMMU_MAMV_SIZE 0x0006
#define CAP_VTD_IOMMU_DWD_LSB 0x0036
#define CAP_VTD_IOMMU_DWD_SIZE 0x0001
#define CAP_VTD_IOMMU_DRD_LSB 0x0037
#define CAP_VTD_IOMMU_DRD_SIZE 0x0001
#define CAP_VTD_IOMMU_FL1GP_LSB 0x0038
#define CAP_VTD_IOMMU_FL1GP_SIZE 0x0001
#define CAP_VTD_IOMMU_PI_LSB 0x003b
#define CAP_VTD_IOMMU_PI_SIZE 0x0001
#define CAP_VTD_IOMMU_FL5LP_LSB 0x003c
#define CAP_VTD_IOMMU_FL5LP_SIZE 0x0001
#define CAP_VTD_IOMMU_ECMDS_LSB 0x003d
#define CAP_VTD_IOMMU_ECMDS_SIZE 0x0001
#define CAP_VTD_IOMMU_ESIRTPS_LSB 0x003e
#define CAP_VTD_IOMMU_ESIRTPS_SIZE 0x0001
#define CAP_VTD_IOMMU_ESRTPS_LSB 0x003f
#define CAP_VTD_IOMMU_ESRTPS_SIZE 0x0001


/** ECAP_VTD_IOMMU desc:
  */

#define ECAP_VTD_IOMMU_REG               (0x00000010U)

typedef union {
  struct {
    UINT64 c : 1; /**< Page-Walk Coherency */
                            /* Bits[0:0], default=0x00000000*/
    UINT64 qi : 1; /**< Queued Invalidation Support */
                            /* Bits[1:1], default=0x00000001*/
    UINT64 dt : 1; /**< Device-TLB Support */
                            /* Bits[2:2], default=0x00000000*/
    UINT64 ir : 1; /**< Interrupt Remapping Suport */
                            /* Bits[3:3], default=0x00000001*/
    UINT64 eim : 1; /**< Extended Interrupt Mode */
                            /* Bits[4:4], default=0x00000001*/
    UINT64 rsvd_0 : 1; /**< Undefined - auto filled rsvd_[05:05] */
                            /* Bits[5:5], default=None*/
    UINT64 pt : 1; /**< Pass Through */
                            /* Bits[6:6], default=0x00000001*/
    UINT64 sc : 1; /**< Snoop Control */
                            /* Bits[7:7], default=0x00000001*/
    UINT64 iro : 10; /**< IOTLB Register Offset */
                            /* Bits[17:8], default=0x000000EF*/
    UINT64 rsvd_1 : 2; /**< Undefined - auto filled rsvd_[19:18] */
                            /* Bits[19:18], default=None*/
    UINT64 mhmv : 4; /**< Maximum Handle Mask Value */
                            /* Bits[23:20], default=0x0000000F*/
    UINT64 rsvd_2 : 1; /**< Undefined - auto filled rsvd_[24:24] */
                            /* Bits[24:24], default=None*/
    UINT64 mts : 1; /**< Memory Type Support */
                            /* Bits[25:25], default=0x00000000*/
    UINT64 nest : 1; /**< Nested Translation Support */
                            /* Bits[26:26], default=0x00000001*/
    UINT64 rsvd_3 : 2; /**< Undefined - auto filled rsvd_[28:27] */
                            /* Bits[28:27], default=None*/
    UINT64 prs : 1; /**< Page Request Support */
                            /* Bits[29:29], default=0x00000000*/
    UINT64 ers : 1; /**< Execute Request Support */
                            /* Bits[30:30], default=0x00000000*/
    UINT64 srs : 1; /**< Supervisor Request Support */
                            /* Bits[31:31], default=0x00000001*/
    UINT64 rsvd_4 : 1; /**< Undefined - auto filled rsvd_[32:32] */
                            /* Bits[32:32], default=None*/
    UINT64 nwfs : 1; /**< No Write Flag Support */
                            /* Bits[33:33], default=0x00000000*/
    UINT64 eafs : 1; /**< Extended Accessed Flag Support */
                            /* Bits[34:34], default=0x00000000*/
    UINT64 pss : 5; /**< PASID Size Supported */
                            /* Bits[39:35], default=0x00000013*/
    UINT64 pasid : 1; /**< Process Address Space ID Support */
                            /* Bits[40:40], default=0x00000000*/
    UINT64 dit : 1; /**< Device-TLB Invalidation Throttle */
                            /* Bits[41:41], default=0x00000000*/
    UINT64 pds : 1; /**< Page Request Draining Support */
                            /* Bits[42:42], default=0x00000000*/
    UINT64 smts : 1; /**< Scalable Mode Translation Support */
                            /* Bits[43:43], default=0x00000001*/
    UINT64 vcs : 1; /**< Virtual Command Support */
                            /* Bits[44:44], default=0x00000000*/
    UINT64 slads : 1; /**< Second-Level Accessed/Dirty Support */
                            /* Bits[45:45], default=0x00000000*/
    UINT64 slts : 1; /**< Second-Level Translation Support */
                            /* Bits[46:46], default=0x00000001*/
    UINT64 flts : 1; /**< First-Level Translation Support */
                            /* Bits[47:47], default=0x00000001*/
    UINT64 smpwcs : 1; /**< Scalable Mode Page-walk Coherency */
                            /* Bits[48:48], default=0x00000000*/
    UINT64 rps : 1; /**< RID_PASID Support */
                            /* Bits[49:49], default=0x00000001*/
#ifdef SV_HOOKS
    UINT64 tdxio : 1; /**< TDX_IO Support */
                            /* Bits[50:50], default=0x00000000*/

#else
    UINT64 rsvd_5 : 1;

                            /* Bits[50:50], Access Type=RO, default=None*/

                            /* Reserved */
#endif      // SV_HOOKS

    UINT64 pms : 1; /**< Performance Monitoring Support */
                            /* Bits[51:51], default=0x00000001*/
    UINT64 adms : 1; /**< Abort DMA Mode Support */
                            /* Bits[52:52], default=0x00000001*/
    UINT64 rprivs : 1; /**< RID-PRIV Supported */
                            /* Bits[53:53], default=0x00000001*/
    UINT64 rsvd_6 : 1; /**< Undefined - auto filled rsvd_[54:54] */
                            /* Bits[54:54], default=None*/
#ifdef SV_HOOKS
    UINT64 hpts : 1; /**< HPT Support */
                            /* Bits[55:55], default=0x00000000*/

#else
    UINT64 rsvd_7 : 1;

                            /* Bits[55:55], Access Type=RO, default=None*/

                            /* Reserved */
#endif      // SV_HOOKS

#ifdef SV_HOOKS
    UINT64 ptrs : 1; /**< PASID in Translated Requests Support */
                            /* Bits[56:56], default=0x00000000*/

#else
    UINT64 rsvd_8 : 1;

                            /* Bits[56:56], Access Type=RO, default=None*/

                            /* Reserved */
#endif      // SV_HOOKS

#ifdef SV_HOOKS
    UINT64 pbds : 1; /**< Page Specific DMA Support */
                            /* Bits[57:57], default=0x00000000*/

#else
    UINT64 rsvd_9 : 1;

                            /* Bits[57:57], Access Type=RO, default=None*/

                            /* Reserved */
#endif      // SV_HOOKS

    UINT64 rsvd_10 : 6; /**< Undefined - auto filled rsvd_[63:58] */
                            /* Bits[63:58], default=None*/
  }     Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} ECAP_VTD_IOMMU_STRUCT;

#define ECAP_VTD_IOMMU_WIDTH 64

#define ECAP_VTD_IOMMU_C_LSB 0x0000
#define ECAP_VTD_IOMMU_C_SIZE 0x0001
#define ECAP_VTD_IOMMU_QI_LSB 0x0001
#define ECAP_VTD_IOMMU_QI_SIZE 0x0001
#define ECAP_VTD_IOMMU_DT_LSB 0x0002
#define ECAP_VTD_IOMMU_DT_SIZE 0x0001
#define ECAP_VTD_IOMMU_IR_LSB 0x0003
#define ECAP_VTD_IOMMU_IR_SIZE 0x0001
#define ECAP_VTD_IOMMU_EIM_LSB 0x0004
#define ECAP_VTD_IOMMU_EIM_SIZE 0x0001
#define ECAP_VTD_IOMMU_PT_LSB 0x0006
#define ECAP_VTD_IOMMU_PT_SIZE 0x0001
#define ECAP_VTD_IOMMU_SC_LSB 0x0007
#define ECAP_VTD_IOMMU_SC_SIZE 0x0001
#define ECAP_VTD_IOMMU_IRO_LSB 0x0008
#define ECAP_VTD_IOMMU_IRO_SIZE 0x000a
#define ECAP_VTD_IOMMU_MHMV_LSB 0x0014
#define ECAP_VTD_IOMMU_MHMV_SIZE 0x0004
#define ECAP_VTD_IOMMU_MTS_LSB 0x0019
#define ECAP_VTD_IOMMU_MTS_SIZE 0x0001
#define ECAP_VTD_IOMMU_NEST_LSB 0x001a
#define ECAP_VTD_IOMMU_NEST_SIZE 0x0001
#define ECAP_VTD_IOMMU_PRS_LSB 0x001d
#define ECAP_VTD_IOMMU_PRS_SIZE 0x0001
#define ECAP_VTD_IOMMU_ERS_LSB 0x001e
#define ECAP_VTD_IOMMU_ERS_SIZE 0x0001
#define ECAP_VTD_IOMMU_SRS_LSB 0x001f
#define ECAP_VTD_IOMMU_SRS_SIZE 0x0001
#define ECAP_VTD_IOMMU_NWFS_LSB 0x0021
#define ECAP_VTD_IOMMU_NWFS_SIZE 0x0001
#define ECAP_VTD_IOMMU_EAFS_LSB 0x0022
#define ECAP_VTD_IOMMU_EAFS_SIZE 0x0001
#define ECAP_VTD_IOMMU_PSS_LSB 0x0023
#define ECAP_VTD_IOMMU_PSS_SIZE 0x0005
#define ECAP_VTD_IOMMU_PASID_LSB 0x0028
#define ECAP_VTD_IOMMU_PASID_SIZE 0x0001
#define ECAP_VTD_IOMMU_DIT_LSB 0x0029
#define ECAP_VTD_IOMMU_DIT_SIZE 0x0001
#define ECAP_VTD_IOMMU_PDS_LSB 0x002a
#define ECAP_VTD_IOMMU_PDS_SIZE 0x0001
#define ECAP_VTD_IOMMU_SMTS_LSB 0x002b
#define ECAP_VTD_IOMMU_SMTS_SIZE 0x0001
#define ECAP_VTD_IOMMU_VCS_LSB 0x002c
#define ECAP_VTD_IOMMU_VCS_SIZE 0x0001
#define ECAP_VTD_IOMMU_SLADS_LSB 0x002d
#define ECAP_VTD_IOMMU_SLADS_SIZE 0x0001
#define ECAP_VTD_IOMMU_SLTS_LSB 0x002e
#define ECAP_VTD_IOMMU_SLTS_SIZE 0x0001
#define ECAP_VTD_IOMMU_FLTS_LSB 0x002f
#define ECAP_VTD_IOMMU_FLTS_SIZE 0x0001
#define ECAP_VTD_IOMMU_SMPWCS_LSB 0x0030
#define ECAP_VTD_IOMMU_SMPWCS_SIZE 0x0001
#define ECAP_VTD_IOMMU_RPS_LSB 0x0031
#define ECAP_VTD_IOMMU_RPS_SIZE 0x0001
#ifdef SV_HOOKS
#define ECAP_VTD_IOMMU_TDXIO_LSB 0x0032
#define ECAP_VTD_IOMMU_TDXIO_SIZE 0x0001
#endif      // SV_HOOKS
#define ECAP_VTD_IOMMU_PMS_LSB 0x0033
#define ECAP_VTD_IOMMU_PMS_SIZE 0x0001
#define ECAP_VTD_IOMMU_ADMS_LSB 0x0034
#define ECAP_VTD_IOMMU_ADMS_SIZE 0x0001
#define ECAP_VTD_IOMMU_RPRIVS_LSB 0x0035
#define ECAP_VTD_IOMMU_RPRIVS_SIZE 0x0001
#ifdef SV_HOOKS
#define ECAP_VTD_IOMMU_HPTS_LSB 0x0037
#define ECAP_VTD_IOMMU_HPTS_SIZE 0x0001
#endif      // SV_HOOKS
#ifdef SV_HOOKS
#define ECAP_VTD_IOMMU_PTRS_LSB 0x0038
#define ECAP_VTD_IOMMU_PTRS_SIZE 0x0001
#endif      // SV_HOOKS
#ifdef SV_HOOKS
#define ECAP_VTD_IOMMU_PBDS_LSB 0x0039
#define ECAP_VTD_IOMMU_PBDS_SIZE 0x0001
#endif      // SV_HOOKS


/** GSTS_VTD_IOMMU desc:
  */

#define GSTS_VTD_IOMMU_REG               (0x0000001cU)

typedef union {
  struct {
    UINT32 rsvd_0 : 23; /**< Undefined - auto filled rsvd_[22:00] */
                            /* Bits[22:0], default=None*/
    UINT32 cfis : 1; /**< Compatibility Format Interrupt Status */
                            /* Bits[23:23], default=0x00000000*/
    UINT32 irtps : 1; /**< Interrupt Remapping Pointer Status */
                            /* Bits[24:24], default=0x00000000*/
    UINT32 ires : 1; /**< Interrupt Remapping Enable Status */
                            /* Bits[25:25], default=0x00000000*/
    UINT32 qies : 1; /**< Queued Invalidation Enable Status */
                            /* Bits[26:26], default=0x00000000*/
    UINT32 wbfs : 1; /**< Write Buffer Flush Status */
                            /* Bits[27:27], default=0x00000000*/
    UINT32 afls : 1; /**< Adavanced Fault Logging Status */
                            /* Bits[28:28], default=0x00000000*/
    UINT32 fls : 1; /**< Fault Log Status */
                            /* Bits[29:29], default=0x00000000*/
    UINT32 rtps : 1; /**< Root Table Pointer Status */
                            /* Bits[30:30], default=0x00000000*/
    UINT32 tes : 1; /**< Translation Enable Status */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GSTS_VTD_IOMMU_STRUCT;

#define GSTS_VTD_IOMMU_WIDTH 32

#define GSTS_VTD_IOMMU_CFIS_LSB 0x0017
#define GSTS_VTD_IOMMU_CFIS_SIZE 0x0001
#define GSTS_VTD_IOMMU_IRTPS_LSB 0x0018
#define GSTS_VTD_IOMMU_IRTPS_SIZE 0x0001
#define GSTS_VTD_IOMMU_IRES_LSB 0x0019
#define GSTS_VTD_IOMMU_IRES_SIZE 0x0001
#define GSTS_VTD_IOMMU_QIES_LSB 0x001a
#define GSTS_VTD_IOMMU_QIES_SIZE 0x0001
#define GSTS_VTD_IOMMU_WBFS_LSB 0x001b
#define GSTS_VTD_IOMMU_WBFS_SIZE 0x0001
#define GSTS_VTD_IOMMU_AFLS_LSB 0x001c
#define GSTS_VTD_IOMMU_AFLS_SIZE 0x0001
#define GSTS_VTD_IOMMU_FLS_LSB 0x001d
#define GSTS_VTD_IOMMU_FLS_SIZE 0x0001
#define GSTS_VTD_IOMMU_RTPS_LSB 0x001e
#define GSTS_VTD_IOMMU_RTPS_SIZE 0x0001
#define GSTS_VTD_IOMMU_TES_LSB 0x001f
#define GSTS_VTD_IOMMU_TES_SIZE 0x0001


/** PMEN_VTD_IOMMU desc:
  */

#define PMEN_VTD_IOMMU_REG               (0x00000064U)

typedef union {
  struct {
    UINT32 prs : 1; /**< Protected Region Status */
                            /* Bits[0:0], default=0x00000000*/
    UINT32 rsvd_0 : 30; /**< Undefined - auto filled rsvd_[30:01] */
                            /* Bits[30:1], default=None*/
    UINT32 epm : 1; /**< Enable Protected Memory */
                            /* Bits[31:31], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PMEN_VTD_IOMMU_STRUCT;

#define PMEN_VTD_IOMMU_WIDTH 32

#define PMEN_VTD_IOMMU_PRS_LSB 0x0000
#define PMEN_VTD_IOMMU_PRS_SIZE 0x0001
#define PMEN_VTD_IOMMU_EPM_LSB 0x001f
#define PMEN_VTD_IOMMU_EPM_SIZE 0x0001


/** PLMBASE_VTD_IOMMU desc:
  */

#define PLMBASE_VTD_IOMMU_REG            (0x00000068U)

typedef union {
  struct {
    UINT32 rsvd_0 : 20; /**< Undefined - auto filled rsvd_[19:00] */
                            /* Bits[19:0], default=None*/
    UINT32 plmb : 12; /**< Protected Low-Memory Base */
                            /* Bits[31:20], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PLMBASE_VTD_IOMMU_STRUCT;

#define PLMBASE_VTD_IOMMU_WIDTH 32

#define PLMBASE_VTD_IOMMU_PLMB_LSB 0x0014
#define PLMBASE_VTD_IOMMU_PLMB_SIZE 0x000c


/** PLMLIMIT_VTD_IOMMU desc:
  */

#define PLMLIMIT_VTD_IOMMU_REG           (0x0000006cU)

typedef union {
  struct {
    UINT32 rsvd_0 : 20; /**< Undefined - auto filled rsvd_[19:00] */
                            /* Bits[19:0], default=None*/
    UINT32 plml : 12; /**< Protected Low-Memory Limit */
                            /* Bits[31:20], default=0x00000000*/
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PLMLIMIT_VTD_IOMMU_STRUCT;

#define PLMLIMIT_VTD_IOMMU_WIDTH 32

#define PLMLIMIT_VTD_IOMMU_PLML_LSB 0x0014
#define PLMLIMIT_VTD_IOMMU_PLML_SIZE 0x000c

//************************************************ /RegisterStructs

#endif      // _VTD_IOMMU_h

