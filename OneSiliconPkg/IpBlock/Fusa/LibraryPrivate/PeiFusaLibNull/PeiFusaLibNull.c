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
  Retrieve fusa test pattern

  @param[in,out] NameGuid    Guid Input

  @param[out] Address        Pointer out to the Fv address

  @param[out] Size           Size of the file
**/
EFI_STATUS
EFIAPI
PeiFusaGetSectionFromFv (
  IN CONST  EFI_GUID        NameGuid,
  OUT VOID                  **Address,
  OUT UINT32                *Size
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
  Initialize FUSA Before the MCHECK

  @param[in] FUSA_CONFIG      FusaConfig

  @retval EFI_SUCCESS          - FUSA initialization complete
**/
EFI_STATUS
FusaInitOnEndOfPei (
  IN  SI_POLICY_PPI    *SiPolicy
  )
{
  return EFI_SUCCESS;
}

/**
  Initialize FUSA

  @param[in] FUSA_CONFIG      FusaConfig

  @retval EFI_SUCCESS          - FUSA initialization complete
**/
EFI_STATUS
FusaInit (
  IN  SI_POLICY_PPI    *SiPolicy
  )
{
  return EFI_SUCCESS;
}

/**
  Fusa Post Bios Done

  @param[in] SI_POLICY_PPI      SiPolicy

  @retval EFI_SUCCESS          - FUSA initialization complete
**/
EFI_STATUS
FusaPostBiosDone (
  IN  SI_POLICY_PPI    *SiPolicy
  )
{
  return EFI_SUCCESS;
}
