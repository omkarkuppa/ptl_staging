/** @file
  CpuPowerOnConfig PEI Disable Library

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

#include <Library/PmcPrivateLib.h>

/**
  Set Early Power On Configuration setting based on default value for SKU.

  @param[in] SiPreMemPolicyPpi - Address of the SI_POLICY_PPI.
**/
VOID
SetCpuStrapAndEarlyPowerOnConfig (
  IN SI_PREMEM_POLICY_PPI        *SiPreMemPolicyPpi
  )
{
  PmcLockSetStrapMsgInterface ();
}

