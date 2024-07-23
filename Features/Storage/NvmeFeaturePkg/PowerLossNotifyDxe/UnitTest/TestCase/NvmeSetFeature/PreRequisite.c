/**@file
    Unit test case Prerequisite of NvmeSetFeature.

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

#include "NvmeSetFeatureMock.h"
#include "PowerLossNotify.h"

/**
  Unit-Test Mock Function.

  @param[in]  Context    Input context for mock.
**/
VOID
PreNvmeSetFeature_Mock (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  PASSTHRU_SET_FEATURE_MOCK *PassThruMock = (PASSTHRU_SET_FEATURE_MOCK*)Context;

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
PreNvmeSetFeature_Fail (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                 Status;
  EFI_STATUS                 *Output;
  PASSTHRU_SET_FEATURE_MOCK  *PassThruMock;
  SAVE_FEATURE_DW3           *DW3;

  //
  // Mock Input
  //
  PassThruMock = AllocateZeroPool (sizeof (PASSTHRU_SET_FEATURE_MOCK));
  PassThruMock->Status     = EFI_TIMEOUT;  //Set feature command timeout
  PassThruMock->Completion = AllocateZeroPool (sizeof (EFI_NVM_EXPRESS_COMPLETION));

  //
  // Ref NVM Express spec, SC: Feature identifier not saveable, SCT: Generic command status
  //
  DW3 = (SAVE_FEATURE_DW3 *) &PassThruMock->Completion->DW3;
  DW3->StatusCode     = 0x0D;
  DW3->StatusCodeType = 0;

  //
  // Expect Output
  //
  Status = EFI_TIMEOUT;
  Output = AllocateZeroPool (sizeof (EFI_STATUS));
  CopyMem (Output, &Status, sizeof (EFI_STATUS));

  PreRequisiteSetMock (
    PassThruMock,              // Mock context
    PreNvmeSetFeature_Mock,    // Mock function
    CleanUpNvmeSetFeature,     // CleanUp function
    (VOID *) Output            // Expect Output
    );

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Prerequisite Function pointer.

  Nvme passthru command run successful, but status Code show not successful completion.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreNvmeSetFeature_Fail2 (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                 Status;
  EFI_STATUS                 *Output;
  PASSTHRU_SET_FEATURE_MOCK  *PassThruMock;
  SAVE_FEATURE_DW3           *DW3;

  //
  // Mock Input
  //
  PassThruMock = AllocateZeroPool (sizeof (PASSTHRU_SET_FEATURE_MOCK));
  PassThruMock->Status     = EFI_SUCCESS;  //Set feature command success
  PassThruMock->Completion = AllocateZeroPool (sizeof (EFI_NVM_EXPRESS_COMPLETION));

  //
  // Ref NVM Express spec, SC: Feature identifier not saveable, SCT: Generic command status
  //
  DW3 = (SAVE_FEATURE_DW3 *) &PassThruMock->Completion->DW3;
  DW3->StatusCode     = 0x0D;
  DW3->StatusCodeType = 0;

  //
  // Expect Output
  //
  Status = EFI_UNSUPPORTED;
  Output = AllocateZeroPool (sizeof (EFI_STATUS));
  CopyMem (Output, &Status, sizeof (EFI_STATUS));

  PreRequisiteSetMock (
    PassThruMock,              // Mock context
    PreNvmeSetFeature_Mock,    // Mock function
    CleanUpNvmeSetFeature,     // CleanUp function
    (VOID *) Output            // Expect Output
    );

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Prerequisite Function pointer.

  Nvme passthru command run successful, and status Code show successful completion.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreNvmeSetFeature_Success (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                 Status;
  EFI_STATUS                 *Output;
  PASSTHRU_SET_FEATURE_MOCK  *PassThruMock;
  SAVE_FEATURE_DW3           *DW3;

  //
  // Mock Input
  //
  PassThruMock = AllocateZeroPool (sizeof (PASSTHRU_SET_FEATURE_MOCK));
  PassThruMock->Status     = EFI_SUCCESS;  //Set feature command success
  PassThruMock->Completion = AllocateZeroPool (sizeof (EFI_NVM_EXPRESS_COMPLETION));

  //
  // Ref NVM Express spec, SC: Successful completion, SCT: Generic command status
  //
  DW3 = (SAVE_FEATURE_DW3 *) &PassThruMock->Completion->DW3;
  DW3->StatusCode     = 0;
  DW3->StatusCodeType = 0;

  //
  // Expect Output
  //
  Status = EFI_SUCCESS;
  Output = AllocateZeroPool (sizeof (EFI_STATUS));
  CopyMem (Output, &Status, sizeof (EFI_STATUS));

  PreRequisiteSetMock (
    PassThruMock,              // Mock context
    PreNvmeSetFeature_Mock,    // Mock function
    CleanUpNvmeSetFeature,     // CleanUp function
    (VOID *) Output            // Expect Output
    );

  return UNIT_TEST_PASSED;
}
