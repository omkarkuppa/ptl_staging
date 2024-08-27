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
    UINT64 nd : 3;
    UINT64 afl : 1;
	  UINT64 rwbf : 1;
    UINT64 plmr : 1;
    UINT64 phmr : 1;
    UINT64 cm : 1;
	  UINT64 sagaw : 5;
    UINT64 rsvd_0 : 3;
    UINT64 mgaw : 6;
    UINT64 zlr : 1; 
    UINT64 rsvd_1 : 1;
    UINT64 fro : 10;
    UINT64 sllps : 4;
    UINT64 rsvd_2 : 1;
    UINT64 psi : 1;
    UINT64 nfr : 8;
    UINT64 mamv : 6;
    UINT64 dwd : 1;
    UINT64 drd : 1;
    UINT64 fl1gp : 1;
    UINT64 rsvd_3 : 2;
    UINT64 pi : 1;
    UINT64 fl5lp : 1;
    UINT64 ecmds : 1;
    UINT64 esirtps : 1;
    UINT64 esrtps : 1;
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
    UINT64 c : 1;
    UINT64 qi : 1;
    UINT64 dt : 1;
    UINT64 ir : 1;
    UINT64 eim : 1;
    UINT64 rsvd_0 : 1;
    UINT64 pt : 1;
    UINT64 sc : 1;
    UINT64 iro : 10;
    UINT64 rsvd_1 : 2;
    UINT64 mhmv : 4;
    UINT64 rsvd_2 : 1;
    UINT64 mts : 1;
    UINT64 nest : 1;
    UINT64 rsvd_3 : 2;
    UINT64 prs : 1;
    UINT64 ers : 1;
    UINT64 srs : 1;
    UINT64 rsvd_4 : 1;
    UINT64 nwfs : 1;
    UINT64 eafs : 1;
    UINT64 pss : 5;
    UINT64 pasid : 1;
    UINT64 dit : 1;
    UINT64 pds : 1;
    UINT64 smts : 1;
    UINT64 vcs : 1;
    UINT64 slads : 1;
    UINT64 slts : 1;
    UINT64 flts : 1;
    UINT64 smpwcs : 1;
	  UINT64 rps : 1;
#ifdef SV_HOOKS
    UINT64 tdxio : 1;

#else
    UINT64 rsvd_5 : 1;
#endif      // SV_HOOKS

    UINT64 pms : 1;
    UINT64 adms : 1;
    UINT64 rprivs : 1;
    UINT64 rsvd_6 : 1;
#ifdef SV_HOOKS
    UINT64 hpts : 1;

#else
    UINT64 rsvd_7 : 1;
#endif      // SV_HOOKS

#ifdef SV_HOOKS
    UINT64 ptrs : 1;

#else
    UINT64 rsvd_8 : 1;
#endif      // SV_HOOKS

#ifdef SV_HOOKS
    UINT64 pbds : 1;

#else
    UINT64 rsvd_9 : 1;
#endif      // SV_HOOKS

    UINT64 rsvd_10 : 6;
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
    UINT32 rsvd_0 : 23;
    UINT32 cfis : 1;
    UINT32 irtps : 1;
    UINT32 ires : 1;
    UINT32 qies : 1;
    UINT32 wbfs : 1;
    UINT32 afls : 1;
    UINT32 fls : 1;
    UINT32 rtps : 1;
    UINT32 tes : 1;
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
    UINT32 prs : 1;
    UINT32 rsvd_0 : 30;
    UINT32 epm : 1;
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
    UINT32 rsvd_0 : 20;
    UINT32 plmb : 12;
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
    UINT32 rsvd_0 : 20;
    UINT32 plml : 12;
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

