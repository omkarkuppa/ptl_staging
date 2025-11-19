/**@file
    Unit test Prerequisite.

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

#include "PreRequisite.h"

VOID *gMockOutput = NULL;
LIST_ENTRY gMockInput = INITIALIZE_LIST_HEAD_VARIABLE (gMockInput);

/**
  Unit-Test Prerequisite Function to set mock parameter.

  @param[in]  Context        A point to the input parameter.
  @param[in]  MockFunc       A point to the Mock fucntion.
  @param[in]  MockCleanUp    A point to the CleanUp function.
  @param[in]  Output         A point to the expect output.

  @retval  UNIT_TEST_PASSED  Successful set mock.

**/
UNIT_TEST_STATUS
EFIAPI
PreRequisiteSetMock (
  IN UNIT_TEST_CONTEXT  Context,
  IN MOCK_FUNCTION      MockFunc,
  IN MOCK_FUNCTION      MockCleanUp,
  IN VOID               *Output
  )
{
  UNIT_TEST_CONTEXT_STRUCT *ContextInput;

  ContextInput = AllocateZeroPool (sizeof (UNIT_TEST_CONTEXT_STRUCT));
  ContextInput->Signature = UNIT_TEST_CONTEXT_SIGNATURE;

  //
  // Mock enter point
  //
  ContextInput->MockFunc    = MockFunc;
  ContextInput->MockCleanUp = MockCleanUp;

  //
  // Mock Input
  //
  ContextInput->MockInput = Context;
  InsertTailList (&gMockInput, &ContextInput->MockLink);

  //
  // Expect Output
  //
  PreRequisiteSetOutput (Output);
  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Prerequisite Function to run mock functionality.

**/
VOID
EFIAPI
PreRequisiteRunMock (
  )
{
  LIST_ENTRY               *Link;
  UNIT_TEST_CONTEXT_STRUCT *ContextInput;

  for (Link = GetFirstNode (&gMockInput);
       !IsNull (&gMockInput, Link);
       Link = GetNextNode (&gMockInput, Link)) {
    ContextInput = UNIT_TEST_CONTEXT_FROM_LINK (Link);
    ContextInput->MockFunc (ContextInput->MockInput);
  }
}

/**
  Unit-Test Prerequisite Function to get the expect output.

**/
VOID*
EFIAPI
PreRequisiteExpectOutput (
  )
{
  return gMockOutput;
}

/**
  Unit-Test Prerequisite Function to set the expect output.

**/
VOID
EFIAPI
PreRequisiteSetOutput (
  VOID * Output
  )
{
  //
  // Expect Output
  //
  gMockOutput = Output;
}
