/** @file
  Header file for macro definitions of Report Status Code.

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

@par Specification
**/

#ifndef _RC_STATUS_CODE_H_
#define _RC_STATUS_CODE_H_
#include <Library/ReportStatusCodeLib.h>
//
// Intel RC Status Code Definitions
//
typedef struct{
  EFI_STATUS_CODE_VALUE Value;
  UINT32                Data;
} STATUS_CODE_TO_POST_CODE_MAP;

#define INTEL_RC_STATUS_CODE        0xF8000000
#define INTEL_RC_STATUS_CODE_MASK   0xFF000000

//
// Subclass Definitions
//
// 0x01 - 7F : Intel Silicon IP Block Reference Code REPORT_STATUS_CODE values.
// 0x80 - FF : Intel Platform Reference Code REPORT_STATUS_CODE values.
//
#define INTEL_RC_SUBCLASS_CODE_MASK           0x00FF0000
#define INTEL_RC_PLATFORM_STATUS_CODE_BEGIN   0x00800000
#define INTEL_RC_CLASS_SUBCLASS_CODE_MASK     0xFFFF0000

#define INTEL_RC_STATUS_CODE_CPU    (INTEL_RC_STATUS_CODE | 0x00010000)            // CPU
#define INTEL_RC_STATUS_CODE_SA     (INTEL_RC_STATUS_CODE | 0x00020000)            // SA
#define INTEL_RC_STATUS_CODE_PCH    (INTEL_RC_STATUS_CODE | 0x00030000)            // PCH
#define INTEL_RC_STATUS_CODE_ME     (INTEL_RC_STATUS_CODE | 0x00040000)            // ME
#define INTEL_RC_STATUS_CODE_MRC    (INTEL_RC_STATUS_CODE | 0x00050000)            // MRC
#define INTEL_RC_STATUS_CODE_FSP    (INTEL_RC_STATUS_CODE | 0x00060000)            // FSP
#define INTEL_RC_STATUS_CODE_VPRO   (INTEL_RC_STATUS_CODE | 0x00070000)            // vPro
#define INTEL_RC_STATUS_CODE_TCSS   (INTEL_RC_STATUS_CODE | 0x00080000)            // TCSS
#define INTEL_RC_STATUS_CODE_SECURITY (INTEL_RC_STATUS_CODE | 0x00090000)          // Security

#define INTEL_RC_STATUS_CODE_BOARD       (INTEL_RC_STATUS_CODE | 0x00800000)       // BOARD
#define INTEL_RC_STATUS_CODE_FEATURES    (INTEL_RC_STATUS_CODE | 0x00810000)       // FEATURES


//
// CPU Subclass Progress Code Definitions
//
#define CPU_POST_MEM_STATUS_CODE   0x9C00
#define CPU_PRE_MEM_STATUS_CODE    0xDC00

#define  INTEL_RC_STATUS_CODE_CPU_POST_MEM_ENTRY                  (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x00))
#define  INTEL_RC_STATUS_CODE_CPU_ADD_POSTMEM_CONFIGBLOCKS_DONE   (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x0F))
#define  INTEL_RC_STATUS_CODE_CPU_INIT_START_POSTMEM              (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x10))
#define  INTEL_RC_STATUS_CODE_CPU_SET_PCD_CPUFEATURE              (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x11))
#define  INTEL_RC_STATUS_CODE_CPU_PROGRAM_PROCFEATURE_START       (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x12))
#define  INTEL_RC_STATUS_CODE_CPU_PROGRAM_PROCFEATURE_DONE        (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x13))
#define  INTEL_RC_STATUS_CODE_CPU_INIT_DONE                       (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x1F))
#define  INTEL_RC_STATUS_CODE_CPU_BIOSGUARD_INIT_START            (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x20))
#define  INTEL_RC_STATUS_CODE_CPU_BIOSGUARD_INIT_RESET_START      (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x21))
#define  INTEL_RC_STATUS_CODE_CPU_BIOSGUARD_INIT_RESET_DONE       (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x29))
#define  INTEL_RC_STATUS_CODE_CPU_BIOSGUARD_INIT_EXIT             (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x2F))
#define  INTEL_RC_STATUS_CODE_CPU_PWRMGMT_PRERESETCPL_START       (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x30))
#define  INTEL_RC_STATUS_CODE_CPU_INITRATL_START                  (INTEL_RC_STATUS_CODE_CPU | (CPU_PRE_MEM_STATUS_CODE | 0x31))
#define  INTEL_RC_STATUS_CODE_CPU_INITRATL_DONE                   (INTEL_RC_STATUS_CODE_CPU | (CPU_PRE_MEM_STATUS_CODE | 0x32))
#define  INTEL_RC_STATUS_CODE_CPU_SETBOOTFREQUENCY_START          (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x33))
#define  INTEL_RC_STATUS_CODE_CPU_SETBOOTFREQUENCY_DONE           (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x34))
#define  INTEL_RC_STATUS_CODE_CPU_PWRMGMT_PRERESETCPL_DONE        (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x3F))
#define  INTEL_RC_STATUS_CODE_CPU_CFG_SVIDVRS_START               (INTEL_RC_STATUS_CODE_CPU | (CPU_PRE_MEM_STATUS_CODE | 0x40))
#define  INTEL_RC_STATUS_CODE_CPU_VR_UPDATE_OVEERRIDES            (INTEL_RC_STATUS_CODE_CPU | (CPU_PRE_MEM_STATUS_CODE | 0x41))
#define  INTEL_RC_STATUS_CODE_CPU_GETCPU_VR_TOPOLOGY              (INTEL_RC_STATUS_CODE_CPU | (CPU_PRE_MEM_STATUS_CODE | 0x42))
#define  INTEL_RC_STATUS_CODE_CPU_CFG_SVIDVRS_DONE                (INTEL_RC_STATUS_CODE_CPU | (CPU_PRE_MEM_STATUS_CODE | 0x4E))
#define  INTEL_RC_STATUS_CODE_CPU_SEND_EARLY_PM_DONE              (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x4F))
#define  INTEL_RC_STATUS_CODE_CPU_RELOAD_UCODEPATCH_START         (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x60))
#define  INTEL_RC_STATUS_CODE_CPU_RELOAD_UCODEPATCH_DONE          (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x61))
#define  INTEL_RC_STATUS_CODE_CPU_POST_UCODEPATCH_INIT_START      (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x62))
#define  INTEL_RC_STATUS_CODE_CPU_UCODEPATCH_INIT_DONE            (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x6F))
#define  INTEL_RC_STATUS_CODE_CPU_PWRMGMTINIT_START               (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x70))
#define  INTEL_RC_STATUS_CODE_CPU_PPM_INIT_START                  (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x71))
#define  INTEL_RC_STATUS_CODE_CPU_PWRMGMT_PVTDATAHOB_START        (INTEL_RC_STATUS_CODE_CPU | (CPU_PRE_MEM_STATUS_CODE | 0x72))
#define  INTEL_RC_STATUS_CODE_CPU_PWRMGMT_PVTDATAHOB_DONE         (INTEL_RC_STATUS_CODE_CPU | (CPU_PRE_MEM_STATUS_CODE | 0x73))
#define  INTEL_RC_STATUS_CODE_CPU_PWRMGMT_DATAHOB_START           (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x74))
#define  INTEL_RC_STATUS_CODE_CPU_PWRMGMT_DATAHOB_DONE            (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x75))
#define  INTEL_RC_STATUS_CODE_CPU_PPM_INIT_DONE                   (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x76))
#define  INTEL_RC_STATUS_CODE_CPU_PWRMGMT_ADDCFGBLKS              (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x7D))
#define  INTEL_RC_STATUS_CODE_CPU_POST_MEM_EXIT                   (INTEL_RC_STATUS_CODE_CPU | (CPU_POST_MEM_STATUS_CODE | 0x7F))
#define  INTEL_RC_STATUS_CODE_CPU_PRE_MEM_ENTRY                   (INTEL_RC_STATUS_CODE_CPU | (CPU_PRE_MEM_STATUS_CODE | 0x00))
#define  INTEL_RC_STATUS_CODE_CPU_ADD_PREMEM_CONFIGBLOCKS_DONE    (INTEL_RC_STATUS_CODE_CPU | (CPU_PRE_MEM_STATUS_CODE | 0x0F))
#define  INTEL_RC_STATUS_CODE_CPU_INIT_START_PREMEM               (INTEL_RC_STATUS_CODE_CPU | (CPU_PRE_MEM_STATUS_CODE | 0x10))
#define  INTEL_RC_STATUS_CODE_CPU_XMM_INIT_START                  (INTEL_RC_STATUS_CODE_CPU | (CPU_PRE_MEM_STATUS_CODE | 0x11))
#define  INTEL_RC_STATUS_CODE_CPU_CPUSTRAPS_INIT_PREMEM           (INTEL_RC_STATUS_CODE_CPU | (CPU_PRE_MEM_STATUS_CODE | 0x12))
#define  INTEL_RC_STATUS_CODE_CPU_SET_CPUSTRAP_AND_EARLYPOWERONCONFIG_START (INTEL_RC_STATUS_CODE_CPU | (CPU_PRE_MEM_STATUS_CODE | 0x13))
#define  INTEL_RC_STATUS_CODE_CPU_SET_CPUSTRAP_AND_EARLYPOWERONCONFIG_RESET (INTEL_RC_STATUS_CODE_CPU | (CPU_PRE_MEM_STATUS_CODE | 0x14))
#define  INTEL_RC_STATUS_CODE_CPU_SET_CPUSTRAP_AND_EARLYPOWERONCONFIG_DONE  (INTEL_RC_STATUS_CODE_CPU | (CPU_PRE_MEM_STATUS_CODE | 0x15))
#define  INTEL_RC_STATUS_CODE_CPU_DEBUG_MSRSET                    (INTEL_RC_STATUS_CODE_CPU | (CPU_PRE_MEM_STATUS_CODE | 0x16))
#define  INTEL_RC_STATUS_CODE_CPU_INIT_DONE_PREMEM                (INTEL_RC_STATUS_CODE_CPU | (CPU_PRE_MEM_STATUS_CODE | 0x1F))
#define  INTEL_RC_STATUS_CODE_CPU_OCINIT_START_PREMEM             (INTEL_RC_STATUS_CODE_CPU | (CPU_PRE_MEM_STATUS_CODE | 0x20))
#define  INTEL_RC_STATUS_CODE_CPU_OCINIT_SET_RESET_FLAG           (INTEL_RC_STATUS_CODE_CPU | (CPU_PRE_MEM_STATUS_CODE | 0x21))
#define  INTEL_RC_STATUS_CODE_CPU_OCINIT_END_PREMEM               (INTEL_RC_STATUS_CODE_CPU | (CPU_PRE_MEM_STATUS_CODE | 0x2F))

/**
  vPro PostCodes format:
  +------------------------------------------------------------------------------------------------------------------+
  | Subclass Code      | IP                 | Phase               | Operation                       | Sub-Task       |
  +--------------------+--------------------+--------+---------------------------------------------------------------+
  | [19:16]            | [15:12]            | [11:8]              | [7:4]                           | [3:0]          |
  +------------------- ----------------------------------------------------------------------------------------------+
  | 0x7 - vPro         | 0x0 -  DGR         | 0x0 - SEC           | 0x1 - MSEG Created              | 0x00           |
  |                    |                    | 0x1 - PEI PrePolicy | 0x2 - DGR Enabled               |                |
  |                    |                    | 0x2 - PEI Pre-Mem   | 0x3 - DGR Disabled              |                |
  |                    |                    | 0x3 - PEI Post-Mem  | 0x4 - Sps Binaries Loaded       |                |
  |                    |                    | 0x4 - End of PEI    | 0x5 - PPAM Binaries Loaded      |                |
  |                    |                    | 0x5 - DXE           | 0x6 - IO MSR Bitmap Loaded      |                |
  |                    |                    | 0x6 - End of DXE    | 0x7 - DGR Set Policy            |                |
  |                    |                    | 0x7 - BDS           |                                 |                |
**/
//
// DGR IP Progress Code Definitions
//
#define  INTEL_RC_STATUS_CODE_VPRO_DGR_MSEG_CREATED               (INTEL_RC_STATUS_CODE_VPRO | 0x0310)
#define  INTEL_RC_STATUS_CODE_VPRO_DGR_IP_ENABLED                 (INTEL_RC_STATUS_CODE_VPRO | 0x0520)
#define  INTEL_RC_STATUS_CODE_VPRO_DGR_IP_DISABLED                (INTEL_RC_STATUS_CODE_VPRO | 0x0530)
#define  INTEL_RC_STATUS_CODE_VPRO_DGR_SPS_BIN_LOADED             (INTEL_RC_STATUS_CODE_VPRO | 0x0540)
#define  INTEL_RC_STATUS_CODE_VPRO_DGR_PPAM_BIN_LOADED            (INTEL_RC_STATUS_CODE_VPRO | 0x0550)
#define  INTEL_RC_STATUS_CODE_VPRO_DGR_IOMSR_BITMAP_LOADED        (INTEL_RC_STATUS_CODE_VPRO | 0x0560)
#define  INTEL_RC_STATUS_CODE_VPRO_DGR_SET_POLICY                 (INTEL_RC_STATUS_CODE_VPRO | 0x0670)

//
// SA Subclass Progress Code Definitions
//
#define  INTEL_RC_STATUS_CODE_SA_DEVICE_CONFIG_START              (INTEL_RC_STATUS_CODE_SA | 0x9A01)
#define  INTEL_RC_STATUS_CODE_SA_PEI_DISPLAY_INIT                 (INTEL_RC_STATUS_CODE_SA | 0x9A03)
#define  INTEL_RC_STATUS_CODE_SA_PEI_IGPU_NTFYCBK_ENTRY           (INTEL_RC_STATUS_CODE_SA | 0x9A04)
#define  INTEL_RC_STATUS_CODE_SA_CALLPPI_AND_FILLFRAMEBUFFER      (INTEL_RC_STATUS_CODE_SA | 0x9A05)
#define  INTEL_RC_STATUS_CODE_SA_IGPU_PPI_INIT_START              (INTEL_RC_STATUS_CODE_SA | 0x9A06)
#define  INTEL_RC_STATUS_CODE_SA_GRPAHICS_PPIGETMODE_START        (INTEL_RC_STATUS_CODE_SA | 0x9A07)
#define  INTEL_RC_STATUS_CODE_SA_DISPLAY_LOGO                     (INTEL_RC_STATUS_CODE_SA | 0x9A08)
#define  INTEL_RC_STATUS_CODE_SA_PEI_IGPU_NTFYCBK_EXIT            (INTEL_RC_STATUS_CODE_SA | 0x9A0F)
#define  INTEL_RC_STATUS_CODE_SA_STREAMTRACER_INIT                (INTEL_RC_STATUS_CODE_SA | 0x9A13)
#define  INTEL_RC_STATUS_CODE_SA_IPUINIT_BEGIN                    (INTEL_RC_STATUS_CODE_SA | 0x9A14)
#define  INTEL_RC_STATUS_CODE_SA_NPUINIT_BEGIN                    (INTEL_RC_STATUS_CODE_SA | 0x9A15)
#define  INTEL_RC_STATUS_CODE_SA_GNA_INIT                         (INTEL_RC_STATUS_CODE_SA | 0x9A16)
#define  INTEL_RC_STATUS_CODE_SA_TCSS_INIT                        (INTEL_RC_STATUS_CODE_SA | 0x9A31)
#define  INTEL_RC_STATUS_CODE_SA_PAVP_INIT                        (INTEL_RC_STATUS_CODE_SA | 0x9A32)
#define  INTEL_RC_STATUS_CODE_SA_VMD_INIT                         (INTEL_RC_STATUS_CODE_SA | 0x9A33)
#define  INTEL_RC_STATUS_CODE_SA_CPUCRASHLOG_DISABLE              (INTEL_RC_STATUS_CODE_SA | 0x9A35)
#define  INTEL_RC_STATUS_CODE_SA_SET_BIOS_RESET_CPL               (INTEL_RC_STATUS_CODE_SA | 0x9A61)
#define  INTEL_RC_STATUS_CODE_SA_IGPU_PM_INIT_START               (INTEL_RC_STATUS_CODE_SA | 0x9A63)
#define  INTEL_RC_STATUS_CODE_SA_SAPCIPRINT_START                 (INTEL_RC_STATUS_CODE_SA | 0x9A64)
#define  INTEL_RC_STATUS_CODE_SA_LOCK_PAM_ENDOFPEI                (INTEL_RC_STATUS_CODE_SA | 0x9A70)
#define  INTEL_RC_STATUS_CODE_SA_S3_RESUME_CALLBACK_EXIT          (INTEL_RC_STATUS_CODE_SA | 0x9A7F)
#define  INTEL_RC_STATUS_CODE_SA_PROGRAM_SA_BARS                  (INTEL_RC_STATUS_CODE_SA | 0xDA06)
#define  INTEL_RC_STATUS_CODE_SA_INSTALL_SA_HOBS                  (INTEL_RC_STATUS_CODE_SA | 0xDA08)
#define  INTEL_RC_STATUS_CODE_SA_TCSS_INIT_PREMEM                 (INTEL_RC_STATUS_CODE_SA | 0xDA09)
#define  INTEL_RC_STATUS_CODE_SA_DISPLAY_INIT_PREMEM              (INTEL_RC_STATUS_CODE_SA | 0xDA0B)
#define  INTEL_RC_STATUS_CODE_SA_CHK_OFFBOARD_PCIEVGA             (INTEL_RC_STATUS_CODE_SA | 0xDA42)
#define  INTEL_RC_STATUS_CODE_SA_GFX_INIT                         (INTEL_RC_STATUS_CODE_SA | 0xDA50)
#define  INTEL_RC_STATUS_CODE_SA_VTD_INIT                         (INTEL_RC_STATUS_CODE_SA | 0xDA51)

/**
  PCH/PCD PostCodes format:
  +-----------------------------------------------------------------------------------------------------------+
  | PCH Subclass Code | PCH/PCD Instance      | Phase               | Operation         | IP                  |
  +-------------------+-----------------------+--------+------------------------------------------------------+
  | [19:16]           | [15:14]               | [13:11]             | [10:7]            | [6:0]               |
  +-----------------------------------------------------------------------------------------------------------+
  | 0x3 - PCH         | 0x0 - PCD (SOC)       | 0x0 - SEC           | 0x1 - Init Start  | 0x00 - SoC          |
  |                   | 0x1 - PCH (discrete)  | 0x1 - PEI PrePolicy | 0X2 - Init Done   | 0x01 - ACE          |
  |                   | 0x2 - IBL             | 0x2 - PEI Pre-Mem   | 0x3 - Reset IP    | 0x02 - USB HOST     |
  |                   |                       | 0x3 - PEI Post-Mem  | 0X4 - Init Link   | 0x03 - USB DEVICE   |
  |                   |                       | 0x4 - End Of PEI    | 0x5 - Init Codec  | 0x04 - GBE          |
  |                   |                       | 0X5 - DXE           | 0x6 - Init Service| 0x05 - PCIE RP      |
  |                   |                       |                     | 0x7 - Enable      | 0x06 - P2SB         |
  |                   |                       |                     | 0x8 - Disable     | 0x07 - P2SB 2       |
  |                   |                       |                     | 0x9-0xF - Other   | 0x08 - PSTH         |
  |                   |                       |                     |                   | 0x09 - ITSS         |
  |                   |                       |                     |                   | 0x0A - SMBUS        |
  |                   |                       |                     |                   | 0x0B - CNVI         |
  |                   |                       |                     |                   | 0x0C - THC          |
  |                   |                       |                     |                   | 0x0D - SCS          |
  |                   |                       |                     |                   | 0x0E - ISH          |
  |                   |                       |                     |                   | 0x0F - GPIO         |
  |                   |                       |                     |                   | 0x10 - RTC          |
  |                   |                       |                     |                   | 0x11 - LPSS         |
  |                   |                       |                     |                   | 0x12 - SPI          |
  |                   |                       |                     |                   | 0x13 - ESPI         |
  |                   |                       |                     |                   | 0x14 - IEH          |
  |                   |                       |                     |                   | 0x15 - PMC          |
  |                   |                       |                     |                   | 0x16 - SSE          |
  |                   |                       |                     |                   | 0x17 - THERMAL      |
  |                   |                       |                     |                   | 0x18 - CHIPSETINNIT |
  |                   |                       |                     |                   | 0x19 - TRACEHUB     |
  |                   |                       |                     |                   | 0x1A - DCI          |
  |                   |                       |                     |                   | 0x1B - HPET         |
  |                   |                       |                     |                   | 0x1C - SATA         |
  |                   |                       |                     |                   | 0x1D - DMI          |
  |                   |                       |                     |                   | 0x1E - EAH          |
  |                   |                       |                     |                   | 0x1F-0x3F - Other   |
  +-----------------------------------------------------------------------------------------------------------+
**/

//
// Common PCH and PCD Subclass Progress Code Definitions
//
#define N_OPERATION 7
#define N_PHASE 11
#define N_INSTANCE 14

#define PC_INST_PCD         0x0 << N_INSTANCE
#define PC_INST_PCH         0x1 << N_INSTANCE
#define PC_INST_IBL         0x2 << N_INSTANCE

#define PC_PHASE_SEC        0x0 << N_PHASE
#define PC_PHASE_PREPOLICY  0x1 << N_PHASE
#define PC_PHASE_PREMEM     0x2 << N_PHASE
#define PC_PHASE_POSTMEM    0x3 << N_PHASE
#define PC_PHASE_ENDOFPEI   0x4 << N_PHASE
#define PC_PHASE_DXE        0x5 << N_PHASE

#define PC_OP_INIT          0x1 << N_OPERATION
#define PC_OP_DONE          0x2 << N_OPERATION
#define PC_OP_RESET         0x3 << N_OPERATION
#define PC_OP_INIT_LINK     0x4 << N_OPERATION
#define PC_OP_INIT_CODEC    0x5 << N_OPERATION
#define PC_OP_INIT_SERVICE  0x6 << N_OPERATION
#define PC_OP_ENABLE        0x7 << N_OPERATION
#define PC_OP_DISABLE       0x8 << N_OPERATION

#define PC_IP_ACE         0x01
#define PC_IP_USB_HOST    0x02
#define PC_IP_USB_DEVICE  0x03
#define PC_IP_GBE         0x04
#define PC_IP_PCIE_RP     0x05
#define PC_IP_P2SB        0x06
#define PC_IP_P2SB_2      0x07
#define PC_IP_PSTH        0x08
#define PC_IP_ITSS        0x09
#define PC_IP_SMBUS       0x0A
#define PC_IP_CNVI        0x0B
#define PC_IP_THC         0x0C
#define PC_IP_SCS         0x0D
#define PC_IP_ISH         0x0E
#define PC_IP_GPIO        0x0F
#define PC_IP_RTC         0x10
#define PC_IP_LPSS        0x11
#define PC_IP_SPI         0x12
#define PC_IP_ESPI        0x13
#define PC_IP_IEH         0x14
#define PC_IP_PMC         0x15
#define PC_IP_SSE         0x16
#define PC_IP_THERMAL     0x17
#define PC_IP_CHIPSETINIT 0x18
#define PC_IP_TRACEHUB    0x19
#define PC_IP_DCI         0x1A
#define PC_IP_HPET        0x1B
#define PC_IP_SATA        0x1C
#define PC_IP_DMI         0x1D
#define PC_IP_EAH         0x1E
#define PC_IP_CAN         0x1F

//
// PCH/PCD Post-Mem Subclass Progress Code Definitions
//
#define PC_PEI_POSTMEM_INIT_START         (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT)
#define PC_PEI_POSTMEM_INIT_USB_HOST      (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_USB_HOST)
#define PC_PEI_POSTMEM_INIT_USB_DEVICE    (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_USB_DEVICE)
#define PC_PEI_POSTMEM_INIT_GBE           (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_GBE)
#define PC_PEI_POSTMEM_INIT_PCIE_RP       (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_PCIE_RP)
#define PC_PEI_POSTMEM_INIT_P2SB          (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_P2SB)
#define PC_PEI_POSTMEM_INIT_PSTH          (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_PSTH)
#define PC_PEI_POSTMEM_INIT_ITSS          (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_ITSS)
#define PC_PEI_POSTMEM_INIT_SMBUS         (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_SMBUS)
#define PC_PEI_POSTMEM_INIT_CNVI          (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_CNVI)
#define PC_PEI_POSTMEM_INIT_THC           (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_THC)
#define PC_PEI_POSTMEM_INIT_SCS           (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_SCS)
#define PC_PEI_POSTMEM_INIT_ISH           (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_ISH)
#define PC_PEI_POSTMEM_INIT_GPIO          (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_GPIO)
#define PC_PEI_POSTMEM_INIT_RTC           (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_RTC)
#define PC_PEI_POSTMEM_INIT_LPSS          (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_LPSS)
#define PC_PEI_POSTMEM_INIT_SPI           (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_SPI)
#define PC_PEI_POSTMEM_INIT_ESPI          (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_ESPI)
#define PC_PEI_POSTMEM_INIT_IEH           (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_IEH)
#define PC_PEI_POSTMEM_INIT_PMC           (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_PMC)
#define PC_PEI_POSTMEM_INIT_CHIPSETINIT   (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_CHIPSETINIT)
#define PC_PEI_POSTMEM_INIT_SSE           (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_SSE)
#define PC_PEI_POSTMEM_INIT_CAN           (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_CAN)
#define PC_PEI_POSTMEM_INIT_SATA          (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_SATA)
#define PC_PEI_POSTMEM_INIT_THERMAL       (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_THERMAL)
#define PC_PEI_POSTMEM_INIT_DMI           (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_DMI)
#define PC_PEI_POSTMEM_INIT_ACE           (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_ACE)
#define PC_PEI_POSTMEM_INIT_EAH           (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_INIT | PC_IP_EAH)
#define PC_PEI_POSTMEM_INIT_DONE          (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_POSTMEM | PC_OP_DONE)

//
// PCH/PCD End of PEI Subclass Progress Code Definitions
//
#define PC_ENDOFPEI_START                 (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_ENDOFPEI | PC_OP_INIT)
#define PC_ENDOFPEI_INIT_THERMAL          (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_ENDOFPEI | PC_OP_INIT       | PC_IP_THERMAL)
#define PC_ENDOFPEI_INIT_ACE              (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_ENDOFPEI | PC_OP_INIT       | PC_IP_ACE)
#define PC_ENDOFPEI_CTRL_RESET_ACE        (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_ENDOFPEI | PC_OP_RESET      | PC_IP_ACE)
#define PC_ENDOFPEI_INIT_LINKS_ACE        (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_ENDOFPEI | PC_OP_INIT_LINK  | PC_IP_ACE)
#define PC_ENDOFPEI_INIT_HDA_CODECS       (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_ENDOFPEI | PC_OP_INIT_CODEC | PC_IP_ACE)
#define PC_ENDOFPEI_DONE                  (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_ENDOFPEI | PC_OP_DONE)

//
// PCH/PCD PEI Pre-Mem Pre-Policy Subclass Progress Code Definitions
//
#define  PC_PEI_PREPOLICY_START             (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_PREPOLICY | PC_OP_INIT)
#define  PC_PEI_PREPOLICY_INIT_SERVICE_SPI  (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_PREPOLICY | PC_OP_INIT_SERVICE | PC_IP_SPI)
#define  PC_PEI_PREPOLICY_DONE              (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_PREPOLICY | PC_OP_DONE)

//
// PCH/PCD PEI Pre-Mem Post-Policy Subclass Progress Code Definitions
//
#define  PC_PEI_PREMEM_START              (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_PREMEM | PC_OP_INIT)
#define  PC_PEI_PREMEM_INIT_SMBUS         (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_PREMEM | PC_OP_INIT    | PC_IP_SMBUS)
#define  PC_PEI_PREMEM_INIT_DCI           (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_PREMEM | PC_OP_INIT    | PC_IP_DCI)
#define  PC_PEI_PREMEM_RESET_DCI          (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_PREMEM | PC_OP_RESET   | PC_IP_DCI)
#define  PC_PEI_PREMEM_INIT_TRACEHUB      (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_PREMEM | PC_OP_INIT    | PC_IP_TRACEHUB)
#define  PC_PEI_PREMEM_ENABLE_TRACEHUB    (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_PREMEM | PC_OP_DISABLE | PC_IP_TRACEHUB)
#define  PC_PEI_PREMEM_DISABLE_TRACEHUB   (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_PREMEM | PC_OP_ENABLE  | PC_IP_TRACEHUB)
#define  PC_PEI_PREMEM_INIT_HPET          (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_PREMEM | PC_OP_INIT    | PC_IP_HPET)
#define  PC_PEI_PREMEM_INIT_CNVI          (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_PREMEM | PC_OP_INIT    | PC_IP_CNVI)
#define  PC_PEI_PREMEM_DONE               (INTEL_RC_STATUS_CODE_PCH | PC_PHASE_PREMEM | PC_OP_DONE)

#define  INTEL_RC_STATUS_CODE_PCH_SMM_RESOURCE_ACCESS_VIOLATION   (INTEL_RC_STATUS_CODE_PCH | 0x0B23)

//
// ME Subclass Progress Code Definitions
//
#define  INTEL_RC_STATUS_CODE_ME_HECI_CONTROL_PPI_ENTRY           (INTEL_RC_STATUS_CODE_ME | 0xDE03)
#define  INTEL_RC_STATUS_CODE_ME_HECI_INIT_START                  (INTEL_RC_STATUS_CODE_ME | 0x0E04)
#define  INTEL_RC_STATUS_CODE_ME_EOP_ENTRY_START                  (INTEL_RC_STATUS_CODE_ME | 0x0E05)
#define  INTEL_RC_STATUS_CODE_ME_RESETREQ_ENTRY_START             (INTEL_RC_STATUS_CODE_ME | 0x0E06)
#define  INTEL_RC_STATUS_CODE_ME_MEBX_ENTRY                       (INTEL_RC_STATUS_CODE_ME | 0x0E07)
#define  INTEL_RC_STATUS_CODE_ME_HECI_GETIMR_START                (INTEL_RC_STATUS_CODE_ME | 0xDE08)
#define  INTEL_RC_STATUS_CODE_ME_ASF_GETBOOTOPTN_ENTRY            (INTEL_RC_STATUS_CODE_ME | 0x0E09)
#define  INTEL_RC_STATUS_CODE_ME_MEDEV_INIT_ENTRY                 (INTEL_RC_STATUS_CODE_ME | 0x9E0A)
#define  INTEL_RC_STATUS_CODE_ME_HECI_CONTROL_PPI_EXIT_SUCCESS    (INTEL_RC_STATUS_CODE_ME | 0xDE23)
#define  INTEL_RC_STATUS_CODE_ME_HECI_INIT_EXIT                   (INTEL_RC_STATUS_CODE_ME | 0x0E24)
#define  INTEL_RC_STATUS_CODE_ME_EOP_EXIT_SUCCESS                 (INTEL_RC_STATUS_CODE_ME | 0x0E25)
#define  INTEL_RC_STATUS_CODE_ME_RESETREQ_EXIT_SUCCESS            (INTEL_RC_STATUS_CODE_ME | 0x0E26)
#define  INTEL_RC_STATUS_CODE_ME_MEBX_EXIT                        (INTEL_RC_STATUS_CODE_ME | 0x0E27)
#define  INTEL_RC_STATUS_CODE_ME_HECI_GETIMR_EXIT_SUCCESS         (INTEL_RC_STATUS_CODE_ME | 0xDE28)
#define  INTEL_RC_STATUS_CODE_ME_ASF_GETBOOTOPTN_EXIT             (INTEL_RC_STATUS_CODE_ME | 0x0E29)
#define  INTEL_RC_STATUS_CODE_ME_DEV_INIT_EXIT_SUCCESS            (INTEL_RC_STATUS_CODE_ME | 0x0E2A)
#define  INTEL_RC_STATUS_CODE_ME_MBP_EXIT_ERR_GETMBPMSGFAIL       (INTEL_RC_STATUS_CODE_ME | 0x9E82)
#define  INTEL_RC_STATUS_CODE_ME_HECI_EXIT_ERR_LACKOFMEM          (INTEL_RC_STATUS_CODE_ME | 0x0E84)
#define  INTEL_RC_STATUS_CODE_ME_EOP_EXIT_ERR_EOPMSGSNDFAIL       (INTEL_RC_STATUS_CODE_ME | 0x0E85)
#define  INTEL_RC_STATUS_CODE_ME_RESETREQ_EXIT_ERR_MSGNA          (INTEL_RC_STATUS_CODE_ME | 0x0E86)
#define  INTEL_RC_STATUS_CODE_ME_HECI_GETIMR_EXIT_ERR_NOHECI      (INTEL_RC_STATUS_CODE_ME | 0xDE88)
#define  INTEL_RC_STATUS_CODE_ME_ASF_GETBOOTOPTN_ERR_HECI_NOSRVC  (INTEL_RC_STATUS_CODE_ME | 0x0E89)
#define  INTEL_RC_STATUS_CODE_ME_MEDEV_EXIT_ERR_INVALIDPARAM      (INTEL_RC_STATUS_CODE_ME | 0x0E8A)
#define  INTEL_RC_STATUS_CODE_ME_HECI_EXIT_ERR_INITFAIL           (INTEL_RC_STATUS_CODE_ME | 0x0EA4)
#define  INTEL_RC_STATUS_CODE_ME_RESETREQ_EXIT_ERR_NOHECI         (INTEL_RC_STATUS_CODE_ME | 0x0EA6)
#define  INTEL_RC_STATUS_CODE_ME_HECI_GETIMR_EXIT_ERR_NORESPONSE  (INTEL_RC_STATUS_CODE_ME | 0xDEA8)
#define  INTEL_RC_STATUS_CODE_ME_HECI_EXIT_ERR_INSTALPROTOFAIL    (INTEL_RC_STATUS_CODE_ME | 0x0EC4)
#define  INTEL_RC_STATUS_CODE_ME_RESETREQ_EXIT_ERR_MSG_SENTFAIL   (INTEL_RC_STATUS_CODE_ME | 0x0EC6)
#define  INTEL_RC_STATUS_CODE_ME_ASF_GETBOOTOPTN_ERR_CLRBOOTOPT   (INTEL_RC_STATUS_CODE_ME | 0x0EC9)
#define  INTEL_RC_STATUS_CODE_ME_RESETREQ_EXIT_ERR_MSG_ACKERR     (INTEL_RC_STATUS_CODE_ME | 0x0EE6)
#define  INTEL_RC_STATUS_CODE_RESETREQ_EXIT_ERR_MSG_ACKERR        (INTEL_RC_STATUS_CODE_ME | 0x0EE6)
#define  INTEL_RC_STATUS_CODE_ME_ASF_GETBOOTOPTN_ERR_HECI_ACSFAIL (INTEL_RC_STATUS_CODE_ME | 0x0EE9)
#define  INTEL_RC_STATUS_CODE_ME_MBP_ENTRY_START                  (INTEL_RC_STATUS_CODE_ME | 0x9E02)
#define  INTEL_RC_STATUS_CODE_ME_MBP_EXIT_SUCCESS                 (INTEL_RC_STATUS_CODE_ME | 0x9E22)
#define  INTEL_RC_STATUS_CODE_ME_MBP_EXIT_ERR_PAYLOAD_FAIL        (INTEL_RC_STATUS_CODE_ME | 0x9EA2)
#define  INTEL_RC_STATUS_CODE_ME_DID_ENTRY_START                  (INTEL_RC_STATUS_CODE_ME | 0xDE01)
#define  INTEL_RC_STATUS_CODE_ME_DID_EXIT_SUCCESS                 (INTEL_RC_STATUS_CODE_ME | 0xDE21)
#define  INTEL_RC_STATUS_CODE_ME_DID_EXIT_ERR_SIPREMEMPPIFAIL     (INTEL_RC_STATUS_CODE_ME | 0xDE81)
#define  INTEL_RC_STATUS_CODE_ME_HECI_CONTROL_PPI_EXIT_ERR_HECIDEV        (INTEL_RC_STATUS_CODE_ME | 0xDE83)
#define  INTEL_RC_STATUS_CODE_ME_DID_EXIT_ERR_DIDMSGSENDFAIL      (INTEL_RC_STATUS_CODE_ME | 0xDEA1)
#define  INTEL_RC_STATUS_CODE_ME_HECI_CONTROL_PPI_EXIT_ERR_INSTALLPPIFAIL (INTEL_RC_STATUS_CODE_ME | 0xDEA3)


//
// MRC Subclass Progress Code Definitions
//
#define INTEL_RC_STATUS_CODE_MRC_POST_CODE                        (INTEL_RC_STATUS_CODE_MRC | 0x0000)

//
// FSP Subclass Progress Code Definitions
//
#define INTEL_RC_STATUS_CODE_FSP_SILICON_INIT_API_ENTRY                                 (INTEL_RC_STATUS_CODE_FSP | 0x9800)
#define INTEL_RC_STATUS_CODE_FSP_MEM_TEMPRAMEXIT_API_ENTRY                              (INTEL_RC_STATUS_CODE_FSP | 0xB800)
#define INTEL_RC_STATUS_CODE_FSP_MEM_TEMPRAMEXIT_API_EXIT                               (INTEL_RC_STATUS_CODE_FSP | 0xB87F)
#define INTEL_RC_STATUS_CODE_FSP_MEM_INIT_API_ENTRY                                     (INTEL_RC_STATUS_CODE_FSP | 0xD800)
#define INTEL_RC_STATUS_CODE_FSP_MEM_INIT_API_EXIT                                      (INTEL_RC_STATUS_CODE_FSP | 0xD87F)

//
// Security Subclass Progress Code Definitions
//
#define INTEL_RC_STATUS_CODE_TXT_ACM_ENTRY                        (INTEL_RC_STATUS_CODE_SECURITY | 0x9901)
#define INTEL_RC_STATUS_CODE_TXT_ACM_EXIT                         (INTEL_RC_STATUS_CODE_SECURITY | 0x9902)
#define INTEL_RC_STATUS_CODE_TDX_ACM_ENTRY                        (INTEL_RC_STATUS_CODE_SECURITY | 0xD903)
#define INTEL_RC_STATUS_CODE_TDX_ACM_EXIT                         (INTEL_RC_STATUS_CODE_SECURITY | 0xD904)


//
// Board Subclass Progress Code Definitions
//
#define INTEL_RC_STATUS_CODE_BOARD_INIT_DXE_ENTRY                 (INTEL_RC_STATUS_CODE_BOARD | 0x7500)
#define INTEL_RC_STATUS_CODE_BOARD_INIT_DXE_EXIT                  (INTEL_RC_STATUS_CODE_BOARD | 0x757F)
#define INTEL_RC_STATUS_CODE_BOARD_INIT_SMM_ENTRY                 (INTEL_RC_STATUS_CODE_BOARD | 0x8500)
#define INTEL_RC_STATUS_CODE_BOARD_INIT_SMM_EXIT                  (INTEL_RC_STATUS_CODE_BOARD | 0x857F)
#define INTEL_RC_STATUS_CODE_BOARD_INIT_POSTMEM_ENTRY             (INTEL_RC_STATUS_CODE_BOARD | 0x9500)
#define INTEL_RC_STATUS_CODE_BOARD_INIT_POSTMEM_EXIT              (INTEL_RC_STATUS_CODE_BOARD | 0x957F)
#define INTEL_RC_STATUS_CODE_BOARD_BIOS_S3_ENTRY                  (INTEL_RC_STATUS_CODE_BOARD | 0xB503)
#define INTEL_RC_STATUS_CODE_BOARD_BIOS_S4_ENTRY                  (INTEL_RC_STATUS_CODE_BOARD | 0xB504)
#define INTEL_RC_STATUS_CODE_BOARD_BIOS_S5_ENTRY                  (INTEL_RC_STATUS_CODE_BOARD | 0xB505)
#define INTEL_RC_STATUS_CODE_BOARD_INIT_PREMEM_ENTRY              (INTEL_RC_STATUS_CODE_BOARD | 0xD500)
#define INTEL_RC_STATUS_CODE_BOARD_INIT_BOARD_ID_UNKNOWN          (INTEL_RC_STATUS_CODE_BOARD | 0xD50E)
#define INTEL_RC_STATUS_CODE_BOARD_INIT_PREMEM_EXIT               (INTEL_RC_STATUS_CODE_BOARD | 0xD57F)


//
// FeaturePkg Subclass Progress Code Definitions
//
//
// FeaturePkg: TCSS
//

/*
FeaturePkg: TCSS Status Code format:
  +------------------------------------------------------------------------------------------------------------=-+
  | TCSS Subclass Code | TCSS type     | Phase               | IP                | Operation                     |
  +--------------------+---------------+---------------------+-------------------+----------------------------- -+
  | [19:16]            | [15:14]       | [13:11]             | [10:8]            | [7:0]                         |
  +--------------------+---------------+---------------------+-------------------+------------------------------=+
  | 0x81 - Features    | 0x0 - iTBT    | 0x0 - PEI Pre-Mem   | 0x0 - TCSS        | 0x00 - Init Start             |
  |                    | 0x1 - dTBT    | 0x1 - PEI Post-Mem  |                   | 0x01 - Init Done              |
  |                    | 0x2 - Capsule | 0x2 - End Of PEI    |                   | 0x02 - 0xDF Reserved          |
  |                    |               | 0x3 - DXE           |                   | 0xE0 - Retimer Capsule Entry  |
  |                    |               |                     |                   | 0xE1 - Offline Mode Entry     |
  |                    |               |                     |                   | 0xE2 - Offline Mode Exit      |
  |                    |               |                     |                   | 0xE3 - ENUM CMD               |
  |                    |               |                     |                   | 0xE4 - LSUP CMD               |
  |                    |               |                     |                   | 0xE5 - USUP CMD               |
  |                    |               |                     |                   | 0xE6 - PD Drive               |
  |                    |               |                     |                   | 0xE7 - PD Restore             |
  |                    |               |                     |                   | 0xE8 - Retimer FP Assert      |
  |                    |               |                     |                   | 0xE9 - Retimer FP De-Assert   |
  |                    |               |                     |                   | 0xEA - Update Retimer NVM     |
  |                    |               |                     |                   | 0xEB - dTBT Capsule Entry     |
  |                    |               |                     |                   | 0xEC - dTBT Create Device Inst|
  |                    |               |                     |                   | 0xED - Update dTBT NVM        |
  |                    |               |                     |                   | 0xEE - Read Retimer Version   |
  +--------------------------------------------------------------------------------------------------------------+
*/

//
// UsbC Capsule Debug Progress Code Definitions
//
#define INTEL_RC_STATUS_CODE_FEATURES_TCSS_GET_SET_TBT_SEC_LEVEL_ENTRY                 (INTEL_RC_STATUS_CODE_FEATURES | 0x7680)
#define INTEL_RC_STATUS_CODE_FEATURES_TCSS_GET_SET_TBT_SEC_LEVEL_EXIT                  (INTEL_RC_STATUS_CODE_FEATURES | 0x7681)
#define INTEL_RC_STATUS_CODE_FEATURES_USBC_RETIMER_CAPSULE_START                       (INTEL_RC_STATUS_CODE_FEATURES | 0x98E0)
#define INTEL_RC_STATUS_CODE_FEATURES_USBC_RETIMER_CAPSULE_OFFLINE_MODE_ENTRY          (INTEL_RC_STATUS_CODE_FEATURES | 0x98E1)
#define INTEL_RC_STATUS_CODE_FEATURES_USBC_RETIMER_CAPSULE_OFFLINE_MODE_EXIT           (INTEL_RC_STATUS_CODE_FEATURES | 0x98E2)
#define INTEL_RC_STATUS_CODE_FEATURES_USBC_RETIMER_CAPSULE_ENUM_CMD                    (INTEL_RC_STATUS_CODE_FEATURES | 0x98E3)
#define INTEL_RC_STATUS_CODE_FEATURES_USBC_RETIMER_CAPSULE_LSUP_CMD                    (INTEL_RC_STATUS_CODE_FEATURES | 0x98E4)
#define INTEL_RC_STATUS_CODE_FEATURES_USBC_RETIMER_CAPSULE_USUP_CMD                    (INTEL_RC_STATUS_CODE_FEATURES | 0x98E5)
#define INTEL_RC_STATUS_CODE_FEATURES_USBC_RETIMER_CAPSULE_PD_DRIVE                    (INTEL_RC_STATUS_CODE_FEATURES | 0x98E6)
#define INTEL_RC_STATUS_CODE_FEATURES_USBC_RETIMER_CAPSULE_PD_RESTORE                  (INTEL_RC_STATUS_CODE_FEATURES | 0x98E7)
#define INTEL_RC_STATUS_CODE_FEATURES_USBC_RETIMER_CAPSULE_RETIMER_FP_GPIO_ASSERT      (INTEL_RC_STATUS_CODE_FEATURES | 0x98E8)
#define INTEL_RC_STATUS_CODE_FEATURES_USBC_RETIMER_CAPSULE_RETIMER_FP_GPIO_DEASSERT    (INTEL_RC_STATUS_CODE_FEATURES | 0x98E9)
#define INTEL_RC_STATUS_CODE_FEATURES_USBC_RETIMER_CAPSULE_UPDATE_RETIMER_NVM_FW       (INTEL_RC_STATUS_CODE_FEATURES | 0x98EA)
#define INTEL_RC_STATUS_CODE_FEATURES_USBC_DTBT_CAPSULE_START                          (INTEL_RC_STATUS_CODE_FEATURES | 0x98EB)
#define INTEL_RC_STATUS_CODE_FEATURES_USBC_DTBT_CAPSULE_CREATE_DEV_INST                (INTEL_RC_STATUS_CODE_FEATURES | 0x98EC)
#define INTEL_RC_STATUS_CODE_FEATURES_USBC_DTBT_CAPSULE_UPDATE_NVM_FW                  (INTEL_RC_STATUS_CODE_FEATURES | 0x98ED)
#define INTEL_RC_STATUS_CODE_FEATURES_USBC_READ_RETIMER_VERSION_START                  (INTEL_RC_STATUS_CODE_FEATURES | 0x98EE)

#define TC_TYPE            14
#define TC_PHASE           11
#define TC_IP               8

#define TC_TYPE_ITBT        0

#define TC_PHASE_PREMEM    (0x0 << TC_PHASE)
#define TC_PHASE_POSTMEM   (0X1 << TC_PHASE)
#define TC_PHASE_ENDOFPEI  (0x2 << TC_PHASE)

#define TC_TCSS            0

#define TC_INIT_START      0
#define TC_INIT_END        1

// TCSS Subclass Progress Code definitions
#define TC_PREMEM_TCSS_INIT_START     (INTEL_RC_STATUS_CODE_TCSS |  TC_PHASE_PREMEM | TC_TCSS | TC_INIT_START)
#define TC_PREMEM_TCSS_INIT_END       (INTEL_RC_STATUS_CODE_TCSS |  TC_PHASE_PREMEM | TC_TCSS | TC_INIT_END)

#define TC_POSTMEM_TCSS_INIT_START    (INTEL_RC_STATUS_CODE_TCSS |  TC_PHASE_POSTMEM | TC_TCSS | TC_INIT_START)
#define TC_POSTMEM_TCSS_INIT_END      (INTEL_RC_STATUS_CODE_TCSS |  TC_PHASE_POSTMEM | TC_TCSS | TC_INIT_END)

#define TC_ENDOFPEI_TCSS_INIT_START   (INTEL_RC_STATUS_CODE_TCSS |  TC_PHASE_ENDOFPEI | TC_TCSS | TC_INIT_START)
#define TC_ENDOFPEI_TCSS_INIT_END     (INTEL_RC_STATUS_CODE_TCSS |  TC_PHASE_ENDOFPEI | TC_TCSS | TC_INIT_END)

#endif
