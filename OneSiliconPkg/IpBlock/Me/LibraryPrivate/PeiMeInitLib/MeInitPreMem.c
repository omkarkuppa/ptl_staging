/** @file
  The PEI Library Implements ME Init.

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

#include <Library/DebugLib.h>
#include <Library/MeInitLib.h>
#include <Ppi/SiPolicy.h>

/**
  Check some ME policies are valid for debugging unexpected problem if these
  values are not initialized or assigned incorrect resource.

  @param[in] SiPreMemPolicyPpi    The SI Policy protocol instance

**/
VOID
MeValidatePolicy (
  IN SI_PREMEM_POLICY_PPI *SiPreMemPolicyPpi
  )
{
  return;
}

/**
  This function performs basic initialization for ME in PEI phase after Policy produced.
**/
VOID
MeOnPolicyInstalled (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "%a () - Start\n", __FUNCTION__));
  //
  // Performing Me initialization from here.
  //
  MePmInit ();

  DEBUG ((DEBUG_INFO, "%a () - End\n", __FUNCTION__));
}
