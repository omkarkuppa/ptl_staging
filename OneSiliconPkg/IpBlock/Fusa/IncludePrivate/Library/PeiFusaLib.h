/** @file
  Header file for PEI FUSA Init Library

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

#ifndef _PEI_FUSA_LIB_H_
#define _PEI_FUSA_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  FusaOverrideProgramming: Override the IGD FUSA register for enabling/disabling FUSA features

  @param[in] SiPolicyPpi        The SI Policy PPI instance
  @retval     EFI_SUCCESS     - IGD workarounds done
**/
EFI_STATUS
IgdFusaOverrideProgramming (
   IN  SI_POLICY_PPI    *SiPolicy
  );

/**
  FusaOverrideProgramming: Override the Opio FUSA register for enabling/disabling FUSA features

  @param[in] SiPolicyPpi        The SI Policy PPI instance
  @retval     EFI_SUCCESS     - Opio workarounds done
**/
EFI_STATUS
OpioFusaOverrideProgramming (
   IN  SI_POLICY_PPI    *SiPolicy
  );

/**
  FusaOverrideProgramming: Override the Psf FUSA register for enabling/disabling FUSA features

  @param[in] SiPolicyPpi        The SI Policy PPI instance
  @retval     EFI_SUCCESS     - Psf workarounds done
**/
EFI_STATUS
PsfFusaOverrideProgramming (
  IN  SI_POLICY_PPI    *SiPolicy
  );

/**
  Lock FUSA Registers After BIOS_DONE.
**/
VOID
FusaRegisterLock (
  VOID
  );

/**
  FusaOverrideProgramming: Override the IOP FUSA register for enabling/disabling FUSA features

  @param[in] SiPolicyPpi        The SI Policy PPI instance
  @retval     EFI_SUCCESS     - IOP workarounds done
**/
EFI_STATUS
IopFusaOverrideProgramming (
  IN SI_POLICY_PPI *SiPolicy
  );

///Common register definition used by CTC with MCA_IOP usage
#define R_SA_MCHBAR_FUSA_MCA_REPORTING_EN_0_0_0_MCHBAR_IMPH                      0x6F30U
#define B_SA_MCHBAR_FUSA_MCA_REPORTING_EN_0_0_0_MCHBAR_IMPH_MCA_REPORTING_EN     BIT0
#define B_MCH_IMPH_FUSA_MCA_REPORTING_EN                                         BIT0
#define R_MCH_IMPH_FUSA_MCA_REPORTING                                            0x6F30U

/**
  Check is the silicon is supporting fusa.

  @retval BOOLEAN TRUE/FALSE
**/
BOOLEAN
IsFusaSupported (
  VOID
  );

#endif // _PEI_FUSA_LIB_H_
