/** @file
  Header file of CPU feature control module

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#ifndef _CPU_INIT_POST_MEM_H_
#define _CPU_INIT_POST_MEM_H_

#include <Ppi/SiPolicy.h>

#define V_BIOS_UCODE_PM_TMR_EMULATION_CFG_CORRECTION_FACTOR_19_MHZ    0x2FBA2E26
#define V_BIOS_UCODE_PM_TMR_EMULATION_CFG_CORRECTION_FACTOR_24_MHZ    0x262E8B52
#define V_BIOS_UCODE_PM_TMR_EMULATION_CFG_CORRECTION_FACTOR_38_MHZ    0x17DD1713
#define V_BIOS_UCODE_PM_TMR_EMULATION_CFG_DELAY_VALUE                 0x13
#define MMIO_MCHECK_FLOW_STATUS_0_0_0_MCHBAR                          0x5A54

/**
  Set the BIOS DONE MSR for all Enabled Cores
**/
VOID
EFIAPI
SetBiosDone (
  VOID
  );

/**
  This function handles Cpu Initialization routine at the end of PEI

  @retval EFI_STATUS       - Always return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
CpuInitAtEndOfPei (
  VOID
  );

/**
  Initialize processor features, performance and power management features, BIOS GUARD, and Overclocking etc features before RESET_CPL at post-memory phase.

  @param[in] SiPolicyPpi    The SI Policy PPI instance

  @retval EFI_SUCCESS     The driver installed/initialized correctly.
**/
EFI_STATUS
EFIAPI
CpuInit (
  IN  SI_POLICY_PPI           *SiPolicyPpi
  );
#endif
