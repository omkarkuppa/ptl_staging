/** @file
  BIOS Guard EarlyPost initializations.

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

@par Specification
**/
#include <Uefi.h>
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
  )
{
  return FALSE;
}

/**
  Perform BIOS Guard initialization.

  @param[in] SiPolicyPpi  - SI Policy PPI
**/
VOID
BiosGuardInit (
  IN SI_POLICY_PPI *SiPolicyPpi
  )
{
  return;
}
