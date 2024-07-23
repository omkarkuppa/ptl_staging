/** @file
  This file Initialize processor power management after memory PEI module.

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

@par Specification
**/

#ifndef _CPU_POWER_MANAGEMENT_INIT_LIB_H
#define _CPU_POWER_MANAGEMENT_INIT_LIB_H

#include <Ppi/SiPolicy.h>
#include <CpuRegs.h>
#include <CpuPowerDeliveryConfig.h>

#define V_IO_CAPT_LVL3                                                0x1   ///< C6
#define V_IO_CAPT_LVL4                                                0x2   ///< C7
#define V_IO_CAPT_LVL5                                                0x3   ///< C8
#define V_IO_CAPT_LVL6                                                0x4   ///< C9
#define V_IO_CAPT_LVL7                                                0x5   ///< C10

/**
  Initialize performance and power management features before RESET_CPL at Post-memory phase.

  @param[in] SiPolicyPpi             The SI Policy PPI instance.
  @param[in] CpuPowerDeliveryConfig  Pointer to CPU_POWER_DELIVERY_CONFIG
**/
VOID
EFIAPI
CpuPowerMgmtInitPreResetCpl (
  IN SI_POLICY_PPI             *SiPolicyPpi,
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  );

/**
  Initialize power management after RESET_CPL at post-memory phase.

  @param[in] CpuPowerDeliveryConfig       Pointer to CPU_POWER_DELIVERY_CONFIG

  @retval EFI_SUCCESS     The driver installed/initialized correctly.
  @retval Driver will ASSERT in debug builds on error.  PPM functionality is considered critical for mobile systems.
**/
EFI_STATUS
EFIAPI
CpuPowerMgmtInit (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  );

/**
  Get TCC cross throttling temperature.
  Set Thermal trip point temperature indicated by MSR 1A2h
  If (RATL mode), T0L = MSR 1A2h[23:16]
  Else          , T0L = MSR 1A2h[23:16] - MSR 1A2h[29:24]

  @retval  Value of the activation temperature
**/
UINT32
EFIAPI
CpuGetCrossThrottlingTripPoint (
  VOID
  );

/**
  Initialize Cpu Power Data hob and processor power management based on hardware capabilities
  and user configuration settings. Include below features:
  1. Create CPU power management private Data Hob
  2. Program RATL
  3. Configure PL4 Boost through PCODE mailbox
  4. Initialize Ctdp and transfer related config to hob

  @param[in] CpuPowerDeliveryConfig       Pointer to CPU_POWER_DELIVERY_CONFIG
**/
VOID
EFIAPI
CpuPowerDeliveryInit (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  );

/**
  Initialize CPU power management private Data Hob
**/
VOID
EFIAPI
InitializeCpuPowerMgmtPrivateDataHob (
  VOID
  );

/**
  Configures Isys Control MMIO register.
    -Configures Isys Platform Power Limit 1 Enable, current and time window
    -Configures Isys Platform Power Limit 2 Enable, current
  @param[in] CpuPowerDeliveryConfig  Pointer to the function parameters passed in
**/

VOID
EFIAPI
ConfigureIsysControl (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  );

/**
  Configures SoC firmware based skin temperature control MMIO register.
    - Configures these MMIO option to set 3 sensors: thresholds, gain, min perf
    - Controls setting enable/disable sensor temperature and ranges

  @param[in] CpuPowerDeliveryConfig  Pointer to the function parameters passed in
**/

VOID
EFIAPI
ConfigureSkinTempControl (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  );

/**
  Configures following fields of PL3 MSR 0x615 based on user configuration:
    Configures power limit 3 power level and time window

  @param[in] CpuPowerDeliveryConfig  Pointer to the function parameters passed in
**/
VOID
EFIAPI
ConfigurePl3PowerLimits (
  IN CPU_POWER_DELIVERY_CONFIG *CpuPowerDeliveryConfig
  );
#endif
