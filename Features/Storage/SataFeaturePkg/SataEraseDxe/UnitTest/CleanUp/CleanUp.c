/**@file
    Unit test CleanUp functionality.

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

#include "CleanUp.h"
#include "PreRequisite.h"

extern VOID *gMockOutput;
extern LIST_ENTRY gMockInput;

/**
  Unit-Test input Cleanup (after) function pointer.

  @param[in]  Context    cleanup the precondition parameter.

**/
VOID
UnitTestCleanupInput (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  LIST_ENTRY               *Link;
  UNIT_TEST_CONTEXT_STRUCT *ContextInput;

  //
  // Destroy mock input context
  //
  Link = GetFirstNode (&gMockInput);
  while (!IsNull (&gMockInput, Link)) {
    ContextInput = UNIT_TEST_CONTEXT_FROM_LINK (Link);
    Link = GetNextNode (&gMockInput, Link);
    RemoveEntryList (&ContextInput->MockLink);
    if (ContextInput->MockCleanUp != NULL) {
      ContextInput->MockCleanUp (ContextInput->MockInput);
    }
    if (ContextInput->MockInput != NULL) {
      FreePool (ContextInput->MockInput);
      ContextInput->MockInput = NULL;
    }
    ContextInput->MockFunc = NULL;
    FreePool (ContextInput);
  }
}

/**
  Unit-Test output Cleanup (after) function pointer.

  @param[in]  Context    cleanup the precondition parameter.

**/
VOID
UnitTestCleanupOutput (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  //
  // Destroy Mock output
  //
  if (gMockOutput != NULL) {
    FreePool (gMockOutput);
    gMockOutput = NULL;
  }
}

/**
  Unit-Test Cleanup (after) function pointer.

  @param[in]  Context    cleanup the precondition parameter.

**/
VOID
UnitTestCleanup (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UnitTestCleanupInput (Context);
  UnitTestCleanupOutput (Context);
}
