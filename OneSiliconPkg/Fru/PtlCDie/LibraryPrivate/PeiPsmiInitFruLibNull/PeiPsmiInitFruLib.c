/** @file
  This code provides an instance of Pei PSMI Init Fru Null Lib.

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

@par Specification Reference:
**/

#include <Library/PeiPsmiInitFruLib.h>

/**
  Override policies relevant to CPU Trace Hub to enable PSMI debug in PreMem.

  @param[in] SiPreMemPolicyPpi         The Silicon PreMem Policy PPI instance
**/
VOID
PsmiTraceHubPolicyOverride (
  IN  SI_PREMEM_POLICY_PPI             *SiPreMemPolicyPpi
  )
{
}

/**
  Returns the Base address of CPU Trace Hub

  @retval CPU Trace Hub Base Address
**/
UINT32
GetTraceHubBaseAddress (
  VOID
  )
{
  return 0;
}

/**
  Returns the Base address of Trace Hub MTB Bar

  @retval Tracehub MTB Bar base address
**/
UINT32
GetTraceHubMtbBarBaseAddress (
  VOID
  )
{
  return 0;
}
