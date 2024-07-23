/**@file
    Unit test case Prerequisite of IsNvmeDevice.

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

#include "IsNvmeDeviceMock.h"
#include "TestCase/GetParentProtocol/GetParentProtocolMock.h"

/**
  Unit-Test Mock Function.

  @param[in]  Context    Input context for mock.
**/
VOID
PreIsNvmeDevice_Mock (
  IN UNIT_TEST_CONTEXT  Context
  )
{
}

/**
  Unit-Test Prerequisite Function pointer.

  It is Nvme device.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreIsNvmeDevice_True (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  BOOLEAN  Status;
  BOOLEAN  *Output;

  //
  // Mock Input
  //
  PreGetParentProtocol_Success (Context);

  //
  // Expect Output
  //
  Status = TRUE;
  Output = AllocateZeroPool (sizeof (BOOLEAN));
  CopyMem (Output, &Status, sizeof (BOOLEAN));

  PreRequisiteSetMock (
    NULL,                  // Mock context
    PreIsNvmeDevice_Mock,  // Mock function
    CleanUpIsNvmeDevice,   // CleanUp function
    (VOID *) Output        // Expect Output
    );

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Prerequisite Function pointer.

  It is not Nvme device.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreIsNvmeDevice_False (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  BOOLEAN  Status;
  BOOLEAN  *Output;

  //
  // Mock Input
  //
  PreGetParentProtocol_Fail_1 (Context);

  //
  // Expect Output
  //
  Status = FALSE;
  Output = AllocateZeroPool (sizeof (BOOLEAN));
  CopyMem (Output, &Status, sizeof (BOOLEAN));

  PreRequisiteSetMock (
    NULL,                  // Mock context
    PreIsNvmeDevice_Mock,  // Mock function
    CleanUpIsNvmeDevice,   // CleanUp function
    (VOID *) Output        // Expect Output
    );

  return UNIT_TEST_PASSED;
}
