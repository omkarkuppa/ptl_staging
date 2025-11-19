/**@file
    Unit test case mock header for GetParentProtocol.

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
#ifndef _GET_PARENT_PROTOCOL_MOCK_H_
#define _GET_PARENT_PROTOCOL_MOCK_H_

#include <setjmp.h>
#include <cmocka.h>

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include "PreRequisite.h"

#include <Protocol/AtaPassThru.h>
#include <Library/UefiLib.h>
#include "BootServicesMock.h"

//
// Define Input parameter for GetParentProtocol Unit Test.
//
typedef struct {
  EFI_STATUS  Status;      // Status of LocateHandle for EFI_ATA_PASS_THRU_PROTOCOL
  UINTN       HandleCount; // Handle count of EFI_ATA_PASS_THRU_PROTOCOL
} LOCATE_HANDLE_BUFFER_MOCK;

typedef struct {
  LOCATE_HANDLE_BUFFER_MOCK  LocateHandleBufferMock; // Mock for the LocatHandleBuffer
  EFI_STATUS                 TestChildHandleStsMock; // Mock Status of EfiTestChildHandle
  EFI_STATUS                 HandleProtocolStsMock;  // Mock Status of HandleProtocol
  EFI_ATA_PASS_THRU_PROTOCOL *PassThru;              // Point to EFI_ATA_PASS_THRU_PROTOCOL
} GET_PARENT_PROTOCOL_MOCK;

/**
  Unit-Test Prerequisite Function pointer.

  Do not found the EFI_ATA_PASS_THRU_PROTOCOL when LocateHandleBuffer.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreGetParentProtocol_Fail_1 (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Prerequisite Function pointer.

  Found the EFI_ATA_PASS_THRU_PROTOCOL when LocateHandleBuffer, but it is not the child handle.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreGetParentProtocol_Fail_2 (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Prerequisite Function pointer.

  Found the EFI_ATA_PASS_THRU_PROTOCOL when LocateHandleBuffer, and it is the child handle.
  But HandleProtocol fail

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreGetParentProtocol_Fail_3 (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Prerequisite Function pointer.

  Found the EFI_ATA_PASS_THRU_PROTOCOL when LocateHandleBuffer, and it is the child handle.
  And HandleProtocol success, return Interface of EFI_ATA_PASS_THRU_PROTOCOL

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreGetParentProtocol_Success (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test GetParentProtocol CleanUp Function. Clean the parameter for mock

  @param[in]  Context    Input context for mock.

**/
VOID
EFIAPI
CleanUpGetParentProtocol (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Search protocol database and check to see if the protocol
  specified by ProtocolGuid is present on a ControllerHandle and opened by
  ChildHandle with an attribute of EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER.
  If the ControllerHandle is found, then the protocol specified by ProtocolGuid
  will be opened on it.

  @param  ProtocolGuid   ProtocolGuid pointer.
  @param  ChildHandle    Child Handle to retrieve Parent information.

**/
VOID *
EFIAPI
GetParentProtocol (
  IN  EFI_GUID    *ProtocolGuid,
  IN  EFI_HANDLE  ChildHandle
  );

/**
  Unit test for GetParentProtocol.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestGetParentProtocol (
  IN UNIT_TEST_CONTEXT  Context
  );
#endif /*_GET_PARENT_PROTOCOL_MOCK_H_*/
