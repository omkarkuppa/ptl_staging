/**@file
    Unit test case Prerequisite of GetParentProtocol.

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

#include "GetParentProtocolMock.h"

/**
  Unit-Test Mock Function.

  @param[in]  Context    Input context for mock.
**/
VOID
PreGetParentProtocol_Mock (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_BOOT_SERVICES  *mBS;

  GET_PARENT_PROTOCOL_MOCK *Parameter = (GET_PARENT_PROTOCOL_MOCK*)Context;

  //
  // Mock boot service
  //
  mBS = BootServiceConstructorMock ();
  mBS->LocateHandleBuffer = MockBootServiceLocateHandleBuffer;
  mBS->HandleProtocol     = MockBootServiceHandleProtocol;
  mBS->FreePool           = MockBootServiceFreePool;

  will_return (MockBootServiceLocateHandleBuffer, Parameter->LocateHandleBufferMock.Status);
  will_return (MockBootServiceLocateHandleBuffer, Parameter->LocateHandleBufferMock.HandleCount);
  if (Parameter->LocateHandleBufferMock.Status == EFI_SUCCESS) {
    will_return (EfiTestChildHandle, Parameter->TestChildHandleStsMock);
    if (Parameter->TestChildHandleStsMock == EFI_SUCCESS) {
      will_return (MockBootServiceOpenProtocol, Parameter->HandleProtocolStsMock);
      will_return (MockBootServiceOpenProtocol, Parameter->PassThru);
    }
  }
}

/**
  Unit-Test Prerequisite Function pointer.

  Not found the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL when LocateHandleBuffer.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreGetParentProtocol_Fail_1 (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  GET_PARENT_PROTOCOL_MOCK    *Parameter;

  //
  // Mock Input
  //
  Parameter = AllocateZeroPool (sizeof (GET_PARENT_PROTOCOL_MOCK));
  Parameter->LocateHandleBufferMock.Status      = EFI_NOT_FOUND;
  Parameter->LocateHandleBufferMock.HandleCount = 0;

  PreRequisiteSetMock (
    Parameter,                  // Mock context
    PreGetParentProtocol_Mock,  // Mock function
    CleanUpGetParentProtocol,   // CleanUp function
    NULL                        // Expect Output
    );

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Prerequisite Function pointer.

  Found the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL when LocateHandleBuffer,
  but it is not the child handle.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreGetParentProtocol_Fail_2 (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  GET_PARENT_PROTOCOL_MOCK    *Parameter;

  //
  // Mock Input
  //
  Parameter = AllocateZeroPool (sizeof (GET_PARENT_PROTOCOL_MOCK));
  Parameter->LocateHandleBufferMock.Status      = EFI_SUCCESS;
  Parameter->LocateHandleBufferMock.HandleCount = 1;
  Parameter->TestChildHandleStsMock             = EFI_UNSUPPORTED;

  PreRequisiteSetMock (
    Parameter,                  // Mock context
    PreGetParentProtocol_Mock,  // Mock function
    CleanUpGetParentProtocol,   // CleanUp function
    NULL                        // Expect Output
    );

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Prerequisite Function pointer.

  Found the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL when LocateHandleBuffer, and it is the child handle.
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
  )
{
  GET_PARENT_PROTOCOL_MOCK    *Parameter;

  //
  // Mock Input
  //
  Parameter = AllocateZeroPool (sizeof (GET_PARENT_PROTOCOL_MOCK));
  Parameter->LocateHandleBufferMock.Status      = EFI_SUCCESS;
  Parameter->LocateHandleBufferMock.HandleCount = 1;
  Parameter->TestChildHandleStsMock             = EFI_SUCCESS;
  Parameter->HandleProtocolStsMock              = EFI_UNSUPPORTED;
  Parameter->PassThru                           = NULL;

  PreRequisiteSetMock (
    Parameter,                  // Mock context
    PreGetParentProtocol_Mock,  // Mock function
    CleanUpGetParentProtocol,   // CleanUp function
    NULL                        // Expect Output
    );

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Prerequisite Function pointer.

  Found the EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL when LocateHandleBuffer, and it is the child handle.
  HandleProtocol success, return Interface of EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreGetParentProtocol_Success (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  GET_PARENT_PROTOCOL_MOCK           *Parameter;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL *PassThru;

  //
  // Mock Input
  //
  PassThru  = AllocateZeroPool (sizeof (EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL));
  Parameter = AllocateZeroPool (sizeof (GET_PARENT_PROTOCOL_MOCK));
  Parameter->LocateHandleBufferMock.Status      = EFI_SUCCESS;
  Parameter->LocateHandleBufferMock.HandleCount = 1;
  Parameter->TestChildHandleStsMock             = EFI_SUCCESS;
  Parameter->HandleProtocolStsMock              = EFI_SUCCESS;
  Parameter->PassThru                           = PassThru;

  PreRequisiteSetMock (
    Parameter,                  // Mock context
    PreGetParentProtocol_Mock,  // Mock function
    CleanUpGetParentProtocol,   // CleanUp function
    (VOID* )PassThru            // Expect Output
    );

  return UNIT_TEST_PASSED;
}
