/**@file
    Unit test case Prerequisite of NvmeEraseEntryPoint.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include "NvmeEraseEntryPointMock.h"

/**
  Unit-Test Mock Function.

  @param[in]  Context    Input context for mock.
**/
VOID
PreNvmeEraseEntryPoint_Mock (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS *Status = (EFI_STATUS*)Context;
  will_return (EfiLibInstallDriverBindingComponentName2, (*Status));
}

/**
  Unit-Test Prerequisite Function pointer.

  EfiLibInstallDriverBindingComponentName2 success

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreNvmeEraseEntryPoint_1 (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;
  EFI_STATUS  *Output;
  EFI_STATUS  *InstallDriverStsMock;

  //
  // Mock Input
  //
  InstallDriverStsMock = AllocateZeroPool (sizeof (EFI_STATUS));
  InstallDriverStsMock[0] = EFI_SUCCESS;

  //
  // Expect Output
  //
  Status = EFI_SUCCESS;
  Output = AllocateZeroPool (sizeof (EFI_STATUS));
  CopyMem (Output, &Status, sizeof (EFI_STATUS));

  PreRequisiteSetMock (
    (VOID*) InstallDriverStsMock, // Mock context
    PreNvmeEraseEntryPoint_Mock,  // Mock function
    CleanUpNvmeEraseEntryPoint,   // CleanUp function
    (VOID*) Output                // Expect Output
    );

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Prerequisite Function pointer.

  EfiLibInstallDriverBindingComponentName2 fail

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreNvmeEraseEntryPoint_2 (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;
  EFI_STATUS  *Output;
  EFI_STATUS  *InstallDriverStsMock;

  //
  // Mock Input
  //
  InstallDriverStsMock = AllocateZeroPool (sizeof (EFI_STATUS));
  InstallDriverStsMock[0] = EFI_INVALID_PARAMETER;

  //
  // Expect Output
  //
  Status = EFI_INVALID_PARAMETER;
  Output = AllocateZeroPool (sizeof (EFI_STATUS));
  CopyMem (Output, &Status, sizeof (EFI_STATUS));

  PreRequisiteSetMock (
    (VOID*) InstallDriverStsMock, // Mock context
    PreNvmeEraseEntryPoint_Mock,  // Mock function
    CleanUpNvmeEraseEntryPoint,   // CleanUp function
    (VOID*) Output                // Expect Output
    );

  return UNIT_TEST_PASSED;
}
