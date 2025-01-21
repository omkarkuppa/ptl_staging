/** @file
  Prototype of the SiPolicyLib library.

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
#ifndef _SI_POLICY_LIB_H_
#define _SI_POLICY_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  Print whole SI_PREMEM_POLICY_PPI and serial out.

  @param[in] SiPreMemPolicyPpi          The RC PREMEM Policy PPI instance
**/
VOID
EFIAPI
SiPreMemPrintPolicyPpi (
  IN  SI_PREMEM_POLICY_PPI          *SiPreMemPolicyPpi
  );

/**
  Print whole SI_POLICY_PPI and serial out.

  @param[in] SiPolicyPpi          The RC Policy PPI instance
**/
VOID
EFIAPI
SiPrintPolicyPpi (
  IN  SI_POLICY_PPI          *SiPolicyPpi
  );

/**
  SiCreatePreMemConfigBlocks creates the config blocksg of Silicon Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[out] SiPreMemPolicyPpi   The pointer to get Silicon PREMEM Policy PPI instance

  @retval EFI_SUCCESS             The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES    Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SiCreatePreMemConfigBlocks (
  OUT  SI_PREMEM_POLICY_PPI         **SiPreMemPolicyPpi
  );

/**
  PchCreatePreMemConfigBlocks creates the config blocksg of PCH Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[out] PchPreMemPolicyPpi  The pointer to get PCH PREMEM Policy PPI instance

  @retval EFI_SUCCESS             The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES    Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
PchCreatePreMemConfigBlocks (
  OUT  SI_PREMEM_POLICY_PPI         **PchPreMemPolicyPpi
  );

/**
  SiCreateConfigBlocks creates the config blocksg of Silicon Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[out] SiPolicyPpi         The pointer to get Silicon Policy PPI instance

  @retval EFI_SUCCESS             The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES    Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SiCreateConfigBlocks (
  OUT  SI_POLICY_PPI         **SiPolicyPpi
  );

/**
  PchCreateConfigBlocks creates the config blocksg of PCH Policy.
  It allocates and zero out buffer, and fills in the Intel default settings.

  @param[out] PchPolicyPpi        The pointer to get PCH Policy PPI instance

  @retval EFI_SUCCESS             The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES    Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
PchCreateConfigBlocks (
  OUT  SI_POLICY_PPI         **PchPolicyPpi
  );

/**
  SiPreMemInstallPolicyPpi installs SiPreMemPolicyPpi.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] SiPreMemPolicyPpi   The pointer to Silicon PREMEM Policy PPI instance

  @retval EFI_SUCCESS            The policy is installed.
  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SiPreMemInstallPolicyPpi (
  IN  SI_PREMEM_POLICY_PPI          *SiPreMemPolicyPpi
  );

/**
  PchPreMemInstallPolicyPpi installs PchPreMemPolicyPpi.
  While installed, RC assumes the PCH Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] PchPreMemPolicyPpi   The pointer to PCH PREMEM Policy PPI instance

  @retval EFI_SUCCESS            The policy is installed.
  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
PchPreMemInstallPolicyPpi (
  IN  SI_PREMEM_POLICY_PPI          *PchPreMemPolicyPpi
  );

/**
  SiPreMemInstallPolicyReadyPpi installs SiPreMemPolicyReadyPpi.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @retval EFI_SUCCESS            The policy is installed.
  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SiPreMemInstallPolicyReadyPpi (
  VOID
  );

/**
  SiInstallPolicyPpi installs SiPolicyPpi.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] SiPolicyPpi         The pointer to Silicon Policy PPI instance

  @retval EFI_SUCCESS            The policy is installed.
  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SiInstallPolicyPpi (
  IN  SI_POLICY_PPI          *SiPolicyPpi
  );

/**
  PchInstallPolicyPpi installs PchPolicyPpi.
  While installed, RC assumes the PCH Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @param[in] PchPolicyPpi         The pointer to PCH Policy PPI instance

  @retval EFI_SUCCESS            The policy is installed.
  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
PchInstallPolicyPpi (
  IN  SI_POLICY_PPI          *PchPolicyPpi
  );

/**
  SiInstallPolicyReadyPpi installs SiPolicyReadyPpi.
  While installed, RC assumes the Policy is ready and finalized. So please update and override
  any setting before calling this function.

  @retval EFI_SUCCESS            The policy is installed.
  @retval EFI_OUT_OF_RESOURCES   Insufficient resources to create buffer
**/
EFI_STATUS
EFIAPI
SiInstallPolicyReadyPpi (
  VOID
  );

#endif // _SI_POLICY_LIB_H_
