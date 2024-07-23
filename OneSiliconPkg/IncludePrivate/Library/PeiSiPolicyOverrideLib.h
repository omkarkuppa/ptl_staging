/** @file
  Header file for PeiSiPolicyOverrideLib.

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
#ifndef _PEI_SI_POLICY_OVERRIDE_LIB_H_
#define _PEI_SI_POLICY_OVERRIDE_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  Policy override in PreMem phase

  @param[in] SiPreMemPolicyPpi         The Silicon PreMem Policy PPI instance
**/
VOID
PeiSiPolicyOverridePreMem (
  IN  SI_PREMEM_POLICY_PPI             *SiPreMemPolicyPpi
  );

/**
  Policy override in PostMem phase

  @param[in] SiPreMemPolicyPpi         The Silicon PreMem Policy PPI instance
  @param[in] SiPolicyPpi               The Silicon PostMem Policy PPI instance
**/
VOID
PeiSiPolicyOverride (
  IN  SI_PREMEM_POLICY_PPI             *SiPreMemPolicyPpi,
  IN  SI_POLICY_PPI                    *SiPolicyPpi
  );

/**
  PCH Policy override in PostMem phase

  @param[in] SiPreMemPolicyPpi         The PCH PreMem Policy PPI instance
  @param[in] SiPolicyPpi               The PCH PostMem Policy PPI instance
**/
VOID
PeiAttachedPchPolicyOverride (
  IN  SI_PREMEM_POLICY_PPI             *SiPreMemPolicyPpi,
  IN  SI_POLICY_PPI                    *SiPolicyPpi
  );

/**
  Policy override on end of PEI phase

  @param[in] SiPolicyPpi               The Silicon PostMem Policy PPI instance
**/
VOID
PeiSiPolicyOverrideOnEndOfPei (
  IN  SI_POLICY_PPI                    *SiPolicyPpi
  );

#endif
