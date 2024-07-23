/** @file
  SampleCode header file of the library for Intel PCH PEI Debug Policy initialization.

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
#ifndef _PEI_PCH_POLICY_DEBUG_LIB_H_
#define _PEI_PCH_POLICY_DEBUG_LIB_H_


/**
  This function performs PCH PEI Debug Policy initialization.

  @param[in] SiPreMemPolicyPpi  The RC PREMEM Policy PPI instance
  @param[in] SiPolicyPpi        The RC POSTMEM Policy PPI instance

  @retval EFI_SUCCESS           The PPI is installed and initialized.
  @retval EFI ERRORS            The PPI is not successfully installed.
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicyDebug (
  SI_PREMEM_POLICY_PPI            *SiPreMemPolicyPpi,
  SI_POLICY_PPI                   *SiPolicyPpi
  );

/**
  This function performs PCH PEI Debug Policy initialization.

  @param[in] SiPreMemPolicyPpi       The RC PREMEM Policy PPI instance

  @retval EFI_SUCCESS        The PPI is installed and initialized.
  @retval EFI ERRORS         The PPI is not successfully installed.
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicyDebugPreMem (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  );
#endif
