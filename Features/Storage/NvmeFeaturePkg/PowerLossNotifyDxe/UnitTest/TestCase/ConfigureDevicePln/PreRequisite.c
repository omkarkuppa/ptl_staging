/**@file
    Unit test case Prerequisite of ConfigureDevicePln.

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

#include "ConfigureDevicePlnMock.h"
#include "PowerLossNotify.h"
#include "TestCase/NvmeGetFeature/NvmeGetFeatureMock.h"
#include "TestCase/NvmeSetFeature/NvmeSetFeatureMock.h"

/**
  Unit-Test Mock Function.

  @param[in]  Context    Input context for mock.
**/
VOID
PreConfigureDevicePln_Mock (
  IN UNIT_TEST_CONTEXT  Context
  )
{
}

/**
  Unit-Test Prerequisite Function pointer.

  Get PLN feature failure.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreConfigureDevicePln_Fail (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                 Status;
  EFI_STATUS                 *Output;

  //
  // Mock Input
  //
  PreNvmeGetFeature_Fail (Context);

  //
  // Expect Output
  //
  Status = EFI_TIMEOUT;
  Output = AllocateZeroPool (sizeof (EFI_STATUS));
  CopyMem (Output, &Status, sizeof (EFI_STATUS));

  PreRequisiteSetMock (
    NULL,                        // Mock context
    PreConfigureDevicePln_Mock,  // Mock function
    CleanUpConfigureDevicePln,   // CleanUp function
    (VOID *) Output              // Expect Output
    );

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Prerequisite Function pointer.

  PLN is disable, Enable the PLN, but enable failure.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreConfigureDevicePln_DisToEnFail (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;
  EFI_STATUS  *Output;

  //
  // Mock Input
  //
  PreNvmeGetFeature_Disable (Context);
  PreNvmeSetFeature_Fail (Context);

  //
  // Expect Output
  //
  Status = EFI_TIMEOUT;
  Output = AllocateZeroPool (sizeof (EFI_STATUS));
  CopyMem (Output, &Status, sizeof (EFI_STATUS));

  PreRequisiteSetMock (
    NULL,                        // Mock context
    PreConfigureDevicePln_Mock,  // Mock function
    CleanUpConfigureDevicePln,   // CleanUp function
    (VOID *) Output              // Expect Output
    );

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Prerequisite Function pointer.

  PLN is disable, Enable the PLN and success.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreConfigureDevicePln_DisToEnSuccess (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;
  EFI_STATUS  *Output;

  //
  // Mock Input
  //
  PreNvmeGetFeature_Disable (Context);
  PreNvmeSetFeature_Success (Context);

  //
  // Expect Output
  //
  Status = EFI_SUCCESS;
  Output = AllocateZeroPool (sizeof (EFI_STATUS));
  CopyMem (Output, &Status, sizeof (EFI_STATUS));

  PreRequisiteSetMock (
    NULL,                        // Mock context
    PreConfigureDevicePln_Mock,  // Mock function
    CleanUpConfigureDevicePln,   // CleanUp function
    (VOID *) Output              // Expect Output
    );

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Prerequisite Function pointer.

  PLN is enabled, Enable the PLN, return success directly.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreConfigureDevicePln_Enabled (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;
  EFI_STATUS  *Output;

  //
  // Mock Input
  //
  PreNvmeGetFeature_Enable (Context);

  //
  // Expect Output
  //
  Status = EFI_SUCCESS;
  Output = AllocateZeroPool (sizeof (EFI_STATUS));
  CopyMem (Output, &Status, sizeof (EFI_STATUS));

  PreRequisiteSetMock (
    NULL,                        // Mock context
    PreConfigureDevicePln_Mock,  // Mock function
    CleanUpConfigureDevicePln,   // CleanUp function
    (VOID *) Output              // Expect Output
    );

  return UNIT_TEST_PASSED;
}
