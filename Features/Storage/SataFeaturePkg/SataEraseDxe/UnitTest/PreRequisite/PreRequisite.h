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

#ifndef _MOCK_PRE_REQUISITE_H_
#define _MOCK_PRE_REQUISITE_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>

typedef
VOID
(EFIAPI *MOCK_FUNCTION)(
  IN UNIT_TEST_CONTEXT  Context
  );

//
// Define Unit Test Context struct().
//
#define UNIT_TEST_CONTEXT_SIGNATURE     SIGNATURE_32 ('U', 'T', 'C', 'S')

typedef struct {
  UINTN         Signature;
  LIST_ENTRY    MockLink;

  MOCK_FUNCTION MockFunc;    // Mock function.
  MOCK_FUNCTION MockCleanUp; // Function for Cleanup input.
  VOID          *MockInput;  // Mock Input.
} UNIT_TEST_CONTEXT_STRUCT;

#define UNIT_TEST_CONTEXT_FROM_LINK(a)  CR(a, UNIT_TEST_CONTEXT_STRUCT, MockLink, UNIT_TEST_CONTEXT_SIGNATURE)

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
  );

/**
  Unit-Test Prerequisite Function to run mock functionality.

**/
VOID
EFIAPI
PreRequisiteRunMock (
  );

/**
  Unit-Test Prerequisite Function to get the expect output.

**/
VOID*
EFIAPI
PreRequisiteExpectOutput (
  );

/**
  Unit-Test Prerequisite Function to set the expect output.

**/
VOID
EFIAPI
PreRequisiteSetOutput (
  VOID * Output
  );
#endif /*_MOCK_PRE_REQUISITE_H_*/
