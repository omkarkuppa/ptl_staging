/** @file
  Header file for PTL SoC Init Lib Pei Phase

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
#ifndef _PTL_SOC_INIT_LIB_H_
#define _PTL_SOC_INIT_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  PCH Init Pre Policy Entry Point
  Performing Pch init in before Poilcy init
**/
VOID
EFIAPI
PtlPcdInitPrePolicy (
  VOID
  );

/**
  Check some PCH policies are valid for debugging unexpected problem if these
  values are not initialized or assigned incorrect resource.
  
  @param[in] SiPreMemPolicy  The SI PreMem Policy PPI instance
  
**/
VOID
PtlPcdValidatePolicy (
  IN  SI_PREMEM_POLICY_PPI   *SiPreMemPolicy
  );

/**
  This function performs basic initialization for PCH in PEI phase after Policy produced.
  If any of the base address arguments is zero, this function will disable the corresponding
  decoding, otherwise this function will enable the decoding.
  This function locks down the AcpiBase.
  @param[in] SiPreMemPolicyPpi         The Silicon PreMem Policy PPI instance
**/
VOID
EFIAPI
PtlPcdOnPolicyInstalled (
  IN  SI_PREMEM_POLICY_PPI    *SiPreMemPolicyPpi
  );

/**
  Pch init after memory PEI module
  @param[in] SiPolicy     The Silicon Policy PPI instance
  @retval None
**/
VOID
EFIAPI
PtlPcdInit (
  IN  SI_POLICY_PPI  *SiPolicy
  );

/**
  Pch End of PEI callback function. This is the last change before entering DXE and OS when S3 resume.
**/
VOID
PtlPcdOnEndOfPei (
  VOID
  );

/**
  Dfd miscellaneous configurations
  @param[in] SiPreMemPolicyPpi  The SI PreMem Policy PPI instance
**/
VOID
DfdConfiguration (
  IN  SI_PREMEM_POLICY_PPI   *SiPreMemPolicyPpi
  );

/**
  Configure root port function number mapping
  @param[in] SiPolicy  Pointer to silicon policy
**/
VOID
PtlPcdPcieRpConfigureRpfnMapping (
  IN SI_POLICY_PPI  *SiPolicy
  );

#endif
