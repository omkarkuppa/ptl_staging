/** @file
  Describes the functions visible to the rest of the BiosGuardInit.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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

#ifndef _BIOSGUARD_INIT_H_
#define _BIOSGUARD_INIT_H_

#include <Ppi/SiPolicy.h>

/**
  Check if BIOS Guard Module is Debug Signed

  @param[in] ModulePtr - Input is the BIOS Guard module address.

  @retval TRUE  - BIOS Guard Module is Debug Signed
  @retval FALSE - BIOS Guard Module is Production Signed
**/
BOOLEAN
IsBiosGuardModuleDebugSigned (
  IN EFI_PHYSICAL_ADDRESS   ModulePtr
  );

/**
  Execute Early-Post initialization of BIOS Guard specific MSRs

  @param[in] SiPolicyPpi - SI Policy PPI

  @retval VOID - No value to return
**/
VOID
BiosGuardInit (
  IN SI_POLICY_PPI *SiPolicyPpi
  );

/**
  Configure BIOS Guard MSRs MSR_PLAT_FRMW_PROT_HASH

  @param[in] BIOS_GUARD_CONFIG  - BIOS Guard Configuration pointer

**/
VOID
ConfigureBiosGuardHashMsrRegisters (
  IN BIOS_GUARD_CONFIG     *BiosGuardConfig
  );

#endif
