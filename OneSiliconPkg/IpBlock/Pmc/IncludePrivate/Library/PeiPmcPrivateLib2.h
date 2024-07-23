/** @file
  Header file for private PEI PMC Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
#ifndef _PEI_PMC_PRIVATE_LIB_H_
#define _PEI_PMC_PRIVATE_LIB_H_

#include <PmcHandle.h>

/**
  Configure PM Timer.
  Once PM Timer is disabled, TCO timer stops counting.
  This must be executed after uCode PM timer emulation.

  @param[in] SiPolicy                  The SI Policy PPI instance
**/
VOID
PmcConfigurePmTimer (
  IN PMC_HANDLE  *PmcHandle
  );

/**
  Lock down PMC settings

  @param[in] SiPolicy                  The SI Policy PPI instance
  @param[in] PmcSocConfig              PMC SoC config
**/
VOID
PmcLockSettings (
  IN PMC_HANDLE      *PmcHandle,
  IN PMC_SOC_CONFIG  *PmcSocConfig
  );

/**
  Perform PCH PMC initialization

  @param[in] PmcHandle  Handle to PMC controller
**/
VOID
PmcInit (
  IN PMC_HANDLE  *PmcHandle
  );

/**
  Perform FIVR initialization
  This function does configuration for below settings:
   - external V1p05 and Vnn rails
   - characteristics of VCCIN_AUX voltage rail
   - CPPM interaction with FIVR
  Many of those settings reside in RTC power well.

  @param[in] SiPolicy                  The SI Policy PPI instance
**/
VOID
PmcPchFivrInit (
  IN PMC_FIVR_HANDLE  *PmcFivrHandle
  );

/**
  Clear MSE and BM bits in PMC Pci config space and clear PMC BAR.

  @param[in] PmcPciCfgBase             Pmc pci config base address
**/
VOID
PmcDisableMmio (
  IN UINT64  PmcPciCfgBase
  );

/**
  This function locks FIVR Configuration

  @param[in] PchPwrmBase  PMC base address
**/
VOID
PmcLockFivrConfig (
  IN UINTN  PchPwrmBase
  );

/**
  This function configures PCH FIVR RFI Control4 setting

  @param[in] RfiControl4  FIVR RFI Control4 setting value
**/
VOID
PmcConfigureFivrRfiControl4 (
  IN UINT32  RfiControl4
  );

/**
  This function configures PCH FIVR EMI Control0 setting

  @param[in] EmiControl0  FIVR EMI Control0 setting value
**/
VOID
PmcConfigureFivrEmiControl0 (
  IN UINT32  EmiControl0
  );

#endif // _PEI_PMC_PRIVATE_LIB_H_
