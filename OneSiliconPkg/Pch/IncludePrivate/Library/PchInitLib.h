/** @file
  Header file for Pch Init Lib Pei Phase

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#ifndef _PCH_INIT_LIB_H_
#define _PCH_INIT_LIB_H_

#include <Ppi/SiPolicy.h>
#include <PmcSocConfig.h>

/**
  Describes the configuration and capabilities of the
  upstream component to which PCH is connected.
**/
typedef struct {
  PMC_ADR_SOC_CONFIG  AdrConfig;
  UINT16              SnoopLtrReqUs;
} PCH_UPSTREAM_COMPONENT_CONFIG;

/**
  PCH Init Pre Policy Entry Point
  Performing Pch init in before Poilcy init
**/
VOID
EFIAPI
PchInitPrePolicy (
  VOID
  );

/**
  Check some PCH policies are valid for debugging unexpected problem if these
  values are not initialized or assigned incorrect resource.

  @param[in] SiPreMemPolicy  The SI PreMem Policy PPI instance

**/
VOID
PchValidatePolicy (
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
PchOnPolicyInstalled (
  IN  SI_PREMEM_POLICY_PPI    *SiPreMemPolicyPpi
  );

/**
  Pch init after memory PEI module

  @param[in] SiPolicy     The Silicon Policy PPI instance

  @retval None
**/
VOID
EFIAPI
PchInit (
  IN SI_POLICY_PPI                  *SiPolicy,
  IN PCH_UPSTREAM_COMPONENT_CONFIG  *PchUpstreamComponentConfig
  );

/**
  Pch End of PEI callback function. This is the last change before entering DXE and OS when S3 resume.
**/
VOID
PchOnEndOfPei (
  VOID
  );

/**
 * Soc Init on Memory Discovered
 */
VOID
PtlPcdOnMemoryDiscovered (
  VOID
  );

#endif // _PEI_PCH_INIT_LIB_H_
