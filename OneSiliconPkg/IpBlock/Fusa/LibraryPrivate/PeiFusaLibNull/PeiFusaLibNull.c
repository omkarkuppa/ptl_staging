/** @file
  Initilize FUSA device in PEI

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#include <Ppi/SiPolicy.h>

/**
  FusaOverrideProgramming: Override the IGD FUSA register for enabling/disabling FUSA features

  @param[in] SiPolicyPpi        The SI Policy PPI instance
  @retval     EFI_SUCCESS     - Display workarounds done
**/
EFI_STATUS
IgdFusaOverrideProgramming (
   IN  SI_POLICY_PPI    *SiPolicy
  )
{
  return EFI_SUCCESS;
}

/**
  FusaOverrideProgramming: Override the Opio FUSA register for enabling/disabling FUSA features

  @param[in] SiPolicyPpi        The SI Policy PPI instance
  @retval     EFI_SUCCESS     - Display workarounds done
**/
EFI_STATUS
OpioFusaOverrideProgramming (
   IN  SI_POLICY_PPI    *SiPolicy
  )
{
  return EFI_SUCCESS;
}

/**
  FusaOverrideProgramming: Override the Psf FUSA register for enabling/disabling FUSA features

  @param[in] SiPolicyPpi        The SI Policy PPI instance
  @retval     EFI_SUCCESS     - Display workarounds done
**/
EFI_STATUS
PsfFusaOverrideProgramming (
  IN  SI_POLICY_PPI    *SiPolicy
  )
{
  return EFI_SUCCESS;
}

/**
  Lock FUSA Registers After BIOS_DONE.
**/
VOID
FusaRegisterLock (
  VOID
  )
{
}

/**
  FusaOverrideProgramming: Override the IOP FUSA register for enabling/disabling FUSA features

  @param[in] SiPolicyPpi        The SI Policy PPI instance
  @retval     EFI_SUCCESS     - IOP workarounds done
**/
EFI_STATUS
IopFusaOverrideProgramming (
  IN SI_POLICY_PPI *SiPolicy
  )
{
  return EFI_SUCCESS;
}

/**
  Check is the silicon is supporting fusa.

  @retval BOOLEAN TRUE/FALSE
**/
BOOLEAN
IsFusaSupported (
  VOID
  )
{
  return FALSE;
}