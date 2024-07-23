/** @file
  This code provides an instance of Pei PSMI Init Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#include <Uefi.h>
#include <Base.h>
#include <Ppi/SiPolicy.h>

/**
  Initializes PSMI Handler and Trace memory size

  @retval EFI_SUCCESS           PsmiInit Function executed successfully
  @retval EFI_UNSUPPORTED       PsmiInit failed to execute
**/
EFI_STATUS
PsmiInit (
  IN    SI_PREMEM_POLICY_PPI        *SiPreMemPolicyPpi
  )
{
  return EFI_SUCCESS;
}

/**
  Configures PSMI Handler/Trace Memory

  @retval EFI_SUCCESS           ConfigurePsmi Function executed successfully
  @retval EFI_UNSUPPORTED       ConfigurePsmi failed to execute
**/
EFI_STATUS
ConfigurePsmi (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  Configures PSMI Handler Base

  @retval EFI_SUCCESS           ConfigurePsmi Function executed successfully
  @retval EFI_UNSUPPORTED       ConfigurePsmi failed to execute
**/
EFI_STATUS
ConfigurePsmiHandler (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  Override policies relevant to CPU Trace Hub to enable PSMI debug in PreMem.

  @param[in] SiPreMemPolicyPpi         The Silicon PreMem Policy PPI instance
**/
VOID
PsmiTraceHubPolicyOverride (
  IN    SI_PREMEM_POLICY_PPI        *SiPreMemPolicyPpi
  )
{
  return;
}

/**
  Update PSMI Handler Base

  @param[in] BaseAddress  - Base Address of PSMI Hanlder

**/
VOID
EFIAPI
UpdatePsmiHandlerBaseAddress (
  IN UINT32     BaseAddress
  )
{
  return;
}