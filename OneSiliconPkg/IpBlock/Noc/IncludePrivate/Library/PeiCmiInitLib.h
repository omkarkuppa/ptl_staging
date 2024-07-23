/** @file
  Header file for CMI Init Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _PEI_CMI_LIB_H_
#define _PEI_CMI_LIB_H_

//
// Define the memory size in 1MB units (1024MB = 1GB).
//
#define MEM_1G                                 0x400
#define MEM_2G                                 0x800
#define MEM_4G                                 0x1000
#define MEM_8G                                 0x2000

//
// SAI definitions for CP, RAC, WAC
//
#define B_HOSTIA_POSTBOOT_SAI         BIT0
#define B_HOSTIA_UCODE_SAI            BIT1
#define B_HOSTIA_SMM_SAI              BIT2
#define B_HOSTIA_SUNPASS_SAI          BIT3
#define B_HOSTIA_BOOT_SAI             BIT4
#define B_RESERVED_1_SAI              BIT5
#define B_RESERVED_2_SAI              BIT6
#define B_HOSTIA_SEAM_SAI             BIT7
#define B_GT_SAI                      BIT8
#define B_PM_PCS_SAI                  BIT9
#define B_HW_CPU_SAI                  BIT10
#define B_MEM_CPL_SAI                 BIT11
#define B_VTD_SAI                     BIT12
#define B_PM_DIE_TO_DIE_SAI           BIT13
#define B_UNASSIGNED_1C_SAI           BIT14
#define B_HOSTCP_PMA_SAI              BIT15
#define B_CSE_INTEL_SAI               BIT16
#define B_DMU_UNTRUSTED_SAI           BIT17
#define B_FUSE_CTRL_SAI               BIT18
#define B_FUSE_PULLER_SAI             BIT19
#define B_GSC_SAI                     BIT20
#define B_PM_IOSS_SAI                 BIT21
#define B_UNASSIGNED_2C_SAI           BIT22
#define B_UNASSIGNED_2E_SAI           BIT23
#define B_DFX_INTEL_MANUFACTURING_SAI BIT24
#define B_DFX_UNTRUSTED_SAI           BIT25
#define B_ISH_SAI                     BIT26
#define B_CLINK_KVM_SAI               BIT27
#define B_NPK_SAI                     BIT28
#define B_TBT_SAI                     BIT29
#define B_IOM_SAI                     BIT30
#define B_HW_PCH_SAI                  BIT31
#define B_SPI_SAI                     BIT32
#define B_UNASSIGNED_42_SAI           BIT33
#define B_ESE_UNTRUSTED_SAI           BIT34
#define B_GT_PMA_SAI                  BIT35
#define B_ESE_SAI                     BIT36
#define B_CSE_UNTRUSTED_SAI           BIT37
#define B_UNASSIGNED_4C_SAI           BIT38
#define B_UNASSIGNED_4E_SAI           BIT39
#define B_IPU_SAI                     BIT40
#define B_RC_MORPHED_SAI              BIT41
#define B_DFX_INTEL_PRODUCTION_SAI    BIT42
#define B_DFX_THIRDPARTY_SAI          BIT43
#define B_DISPLAY_SAI                 BIT44
#define B_LT_SAI                      BIT45
#define B_UNASSIGNED_5C_SAI           BIT46
#define B_DISPLAY_KVM_SAI             BIT47
#define B_UNASSIGNED_60_SAI           BIT48
#define B_ACE_SAI                     BIT49
#define B_UNASSIGNED_64_SAI           BIT50
#define B_CSME_SMS_SAI                BIT51
#define B_CORE_EVENT_PROXY_SAI        BIT52
#define B_DEVICE_POSTBOOT_SAI         BIT53
#define B_RCIOMMU_BYPASS_SAI          BIT54
#define B_PCH_IMR_SAI                 BIT55
#define B_EC_ESPI_SAI                 BIT56
#define B_CNVi_SAI                    BIT57
#define B_UNASSIGNED_74_SAI           BIT58
#define B_UNASSIGNED_76_SAI           BIT59
#define B_UNASSIGNED_78_SAI           BIT60
#define B_CRASHLOG_SAI                BIT61
#define B_UNASSIGNED_7C_SAI           BIT62
#define B_DEVICE_UNTRUSTED_SAI        BIT63
#define V_SAI_NONE                    0x0

//
// Hash value is subject to change based on post silicon characterization
//
#define V_MC_HASH_RANGE_1_2_3_HASH_BITS  (BIT9 | BIT10 | BIT13 | BIT19) >> CMIM_MC_HASH_RANGE_1_2_3_NGU_PMA_HASHBITS_LSB
#define V_MC_HASH_RANGE_4_5_HASH_BITS    0x0
#define V_CCE_ADM_HASH_6_19_BITS         (BIT9 | BIT10 | BIT13 | BIT19) >> CMIM_CCE_ADM_HASH_6_19_NGU_PMA_HASHBITS_LSB    // 82600

#endif
