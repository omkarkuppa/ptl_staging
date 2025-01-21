/**@file
    Unit test case Prerequisite of NvmeGetFeature.

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

#include "NvmeGetFeatureMock.h"
#include "PowerLossNotify.h"

/**
  Unit-Test Mock Function.

  @param[in]  Context    Input context for mock.
**/
VOID
PreNvmeGetFeature_Mock (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  PASSTHRU_GET_FEATURE_MOCK *PassThruMock = (PASSTHRU_GET_FEATURE_MOCK*)Context;

  will_return (MockNvmePassThruProtocol_PassThru, PassThruMock->Status);
  will_return (MockNvmePassThruProtocol_PassThru, PassThruMock->Completion);
}

/**
  Unit-Test Prerequisite Function pointer.

  Nvme passthru command timeout.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreNvmeGetFeature_Fail (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                 Status;
  EFI_STATUS                 *Output;
  PASSTHRU_GET_FEATURE_MOCK  *PassThruMock;
  GET_FEATURE_ATTR_CUR       *DW0;

  //
  // Mock Input
  //
  PassThruMock = AllocateZeroPool (sizeof (PASSTHRU_GET_FEATURE_MOCK));
  PassThruMock->Status     = EFI_TIMEOUT;  //Get feature command timeout
  PassThruMock->Completion = AllocateZeroPool (sizeof (EFI_NVM_EXPRESS_COMPLETION));
  DW0 = (GET_FEATURE_ATTR_CUR *) &PassThruMock->Completion->DW0;
  DW0->PlnEnable = PLN_DISABLE;  // Don't care this for command failure

  //
  // Expect Output
  //
  Status = EFI_TIMEOUT;
  Output = AllocateZeroPool (sizeof (EFI_STATUS));
  CopyMem (Output, &Status, sizeof (EFI_STATUS));

  PreRequisiteSetMock (
    PassThruMock,              // Mock context
    PreNvmeGetFeature_Mock,    // Mock function
    CleanUpNvmeGetFeature,     // CleanUp function
    (VOID *) Output            // Expect Output
    );

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Prerequisite Function pointer.

  Nvme passthru command run successful, PLN is disable.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreNvmeGetFeature_Disable (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                 Status;
  EFI_STATUS                 *Output;
  PASSTHRU_GET_FEATURE_MOCK  *PassThruMock;
  GET_FEATURE_ATTR_CUR       *DW0;

  //
  // Mock Input
  //
  PassThruMock = AllocateZeroPool (sizeof (PASSTHRU_GET_FEATURE_MOCK));
  PassThruMock->Status     = EFI_SUCCESS;  //Get feature command success
  PassThruMock->Completion = AllocateZeroPool (sizeof (EFI_NVM_EXPRESS_COMPLETION));

  //
  // Ref to NVM Express spec, DW0 contains feature information
  // Mock PLN disable
  //
  DW0 = (GET_FEATURE_ATTR_CUR *) &PassThruMock->Completion->DW0;
  DW0->PlnEnable = PLN_DISABLE;

  //
  // Expect Output
  //
  Status = EFI_SUCCESS;
  Output = AllocateZeroPool (sizeof (EFI_STATUS));
  CopyMem (Output, &Status, sizeof (EFI_STATUS));

  PreRequisiteSetMock (
    PassThruMock,              // Mock context
    PreNvmeGetFeature_Mock,    // Mock function
    CleanUpNvmeGetFeature,     // CleanUp function
    (VOID *) Output            // Expect Output
    );

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Prerequisite Function pointer.

  Nvme passthru command run successful, and PLN is enable.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreNvmeGetFeature_Enable (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                 Status;
  EFI_STATUS                 *Output;
  PASSTHRU_GET_FEATURE_MOCK  *PassThruMock;
  GET_FEATURE_ATTR_CUR       *DW0;

  //
  // Mock Input
  //
  PassThruMock = AllocateZeroPool (sizeof (PASSTHRU_GET_FEATURE_MOCK));
  PassThruMock->Status     = EFI_SUCCESS;  //Get feature command success
  PassThruMock->Completion = AllocateZeroPool (sizeof (EFI_NVM_EXPRESS_COMPLETION));

  //
  // Ref to NVM Express spec, DW0 contains feature information
  // Mock PLN enable
  //
  DW0 = (GET_FEATURE_ATTR_CUR *) &PassThruMock->Completion->DW0;
  DW0->PlnEnable = PLN_ENABLE;

  //
  // Expect Output
  //
  Status = EFI_SUCCESS;
  Output = AllocateZeroPool (sizeof (EFI_STATUS));
  CopyMem (Output, &Status, sizeof (EFI_STATUS));

  PreRequisiteSetMock (
    PassThruMock,              // Mock context
    PreNvmeGetFeature_Mock,    // Mock function
    CleanUpNvmeGetFeature,     // CleanUp function
    (VOID *) Output            // Expect Output
    );

  return UNIT_TEST_PASSED;
}
