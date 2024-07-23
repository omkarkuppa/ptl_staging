/** @file
  This header file contains list of IO Ports and MSR registers which are required
  to allow access for Read & Write in SMM Handler execution code after End Of DXE.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#ifndef __SMM_IOMSR_ACCESS_H__
#define __SMM_IOMSR_ACCESS_H__

#include <CpuRegs.h>
#include <Register/Msr.h>
#include <Register/EspiRegs.h>
#include <Register/RtcRegs.h>
#include <Register/GenerationMsr.h>

// Com ports
#define COM1_BASE               0x3F8
#define COM2_BASE               0x2F8
#define COM3_BASE               0x3E8
#define COM4_BASE               0x2E8
// EC Communication
#define EC_CMD                  0x62
#define EC_DATA                 0x66
// KBC
#define KBC_C_PORT              0x64
#define KBC_D_PORT              0x60

#define ACPI_BASE               FixedPcdGet16 (PcdAcpiBaseAddress)
#define SMBUS_BASE              FixedPcdGet16 (PcdSmbusBaseAddress)
#define TCO_BASE                FixedPcdGet16 (PcdTcoBaseAddress)

typedef struct {
  UINT16      PortBase;
  UINT16      PortLength;
} DGR_IO_TABLE;

typedef struct {
  UINT32       Msr;
  BOOLEAN      DenyReadAccess;
  BOOLEAN      DenyWriteAccess;
} DGR_MSR_TABLE;

const DGR_IO_TABLE mAllowedListIo[] = {
  { COM1_BASE,                   8 },
  { COM2_BASE,                   8 },
  { COM3_BASE,                   8 },
  { COM4_BASE,                   8 },
  { EC_CMD,                      1 },
  { EC_DATA,                     1 },
  { KBC_D_PORT,                  1 },
  { KBC_C_PORT,                  1 },
  { R_PCH_IO_RST_CNT,            1 },        // PCI Reset
  { R_PCH_IO_APM_CNT,            2 },        // SW SMI Port B2, B3
  { R_RTC_IO_INDEX,              8 },        // RTC
  { B_RTC_IO_REGISTER_B_RTC_SET, 4 },        // Port 80
  { ACPI_BASE,                0x80 },
  { SMBUS_BASE,               0x18 },
  { TCO_BASE,                 0x14 }
};

const DGR_MSR_TABLE mAllowedListMsr[] = {
  { MSR_SMM_FEATURE_CONTROL,                  FALSE, FALSE },
  { MSR_IA32_MCG_CAP,                         FALSE, FALSE },
  { MSR_IA32_FEATURE_CONTROL,                 FALSE, FALSE },
  { MSR_IA32_MCG_EXT_CTL,                     FALSE, FALSE },
  { MSR_IA32_MCG_STATUS,                      FALSE, FALSE },
  { MSR_IA32_DEBUGCTL,                        FALSE, FALSE },
  { MSR_IA32_MISC_ENABLE,                     FALSE, FALSE },
  { MSR_TEMPERATURE_TARGET,                   FALSE, FALSE },
  { MSR_PLATFORM_INFO,                        FALSE, FALSE },
  { MSR_PACKAGE_RAPL_LIMIT,                   FALSE, FALSE },
  { MSR_PACKAGE_POWER_SKU_UNIT,               FALSE, FALSE },
  { MSR_CORE_THREAD_COUNT,                    FALSE, FALSE },
  { MSR_PACKAGE_POWER_SKU,                    FALSE, FALSE },
  { MSR_FLEX_RATIO,                           FALSE, FALSE },
  { MSR_TURBO_RATIO_LIMIT,                    FALSE, FALSE },
  { MSR_SMM_MCA_CAP,                          FALSE, FALSE },
  { MSR_PLAT_FRMW_PROT_CTRL,                  FALSE, FALSE },
  { MSR_PLAT_FRMW_PROT_TRIG_PARAM,            FALSE, FALSE },
  { MSR_PLAT_FRMW_PROT_TRIGGER,               FALSE, FALSE },
  { MSR_IA32_MTRRCAP,                         FALSE, FALSE },
  { MSR_IA32_SMRR_PHYSBASE,                   FALSE, FALSE },
  { MSR_IA32_SMRR_PHYSMASK,                   FALSE, FALSE },
  { MSR_SMM_PROT_MODE_BASE,                   FALSE, FALSE },
  { MSR_IA32_APIC_BASE,                       FALSE, FALSE },
  { MSR_IA32_THERM_INTERRUPT,                 FALSE, FALSE },
  { MSR_IA32_THERM_STATUS,                    FALSE, FALSE },
  { MSR_MISC_PWR_MGMT,                        FALSE, FALSE },
  { MSR_IA32_X2APIC_LVT_THERMAL,              FALSE, FALSE },
  { MSR_IA32_PACKAGE_THERM_STATUS,            FALSE, FALSE },
  { MSR_IA32_PACKAGE_THERM_INTERRUPT,         FALSE, FALSE },
  { MSR_CORE_THREAD_COUNT,                    FALSE, FALSE },
  { MSR_IA32_PM_ENABLE,                       FALSE, FALSE },
  { MSR_IA32_HWP_STATUS,                      FALSE, FALSE },
  { MSR_IA32_HWP_INTERRUPT,                   FALSE, FALSE },
  { MSR_IA32_SMBASE,                          FALSE, TRUE  },
  { MSR_IA32_X2APIC_APICID,                   FALSE, TRUE  },
  { MSR_IA32_X2APIC_ICR,                      FALSE, FALSE },
  { MSR_SPCL_CHIPSET_USAGE,                   FALSE, FALSE },
  { MSR_SMM_BLOCKED,                          FALSE, FALSE }
};

#endif
