/**@file
    Unit test case of the I2c TouchPanel Dxe driver.

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "TestCaseGetHidDescriptor.h"
#include <Library/DebugLib.h>
#include <I2cTouchPanel.h>
#include <BootServicesMock.h>
#include <Library/UefiBootServicesTableLib.h>
#include <PrivateMock.h>

/**
  Event used for first read HID Descriptor transaction from Touch Device

  @param[in]  Event    Event called by I2C HOST on the I2C request completion
  @param[in]  Context  Context passed by the event creator
**/
extern
VOID
EFIAPI
HidDescriptorReceivedEvent (
  IN EFI_EVENT   Event,
  IN VOID        *Context
  );

/**
  Get HID descriptor via I2C protocol.

  @param  DriverContext         The code context.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_DEVICE_ERROR      Failed to obtain HID Descriptor.
  @retval Others                An unexpected error occurred.
**/
extern
EFI_STATUS
EFIAPI
GetHidDescriptor (
  IN OUT ABSOLUTE_POINTER_DEV  *TouchDev
  );

/**
  Mock of GetHidDescriptor

  @param[in]  Context                   [Optional] An optional parameter that enables:
                                        1) test-case reuse with varied parameters and
                                        2) test-case re-entry for Target tests that need a
                                        reboot.  This parameter is a VOID* and it is the
                                        responsibility of the test author to ensure that the
                                        contents are well understood by all test cases that may
                                        consume it.
  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestGetHidDescriptor (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                      Status;
  ABSOLUTE_POINTER_DEV            *TouchDev;
  UINT64                          CreateEventStatus[TEST_CASE_5_ROUNDS]      = {EFI_INVALID_PARAMETER, EFI_SUCCESS     , EFI_SUCCESS     , EFI_SUCCESS     , EFI_SUCCESS};
  UINT64                          CloseEventStatus[TEST_CASE_5_ROUNDS]       = {EFI_DEVICE_ERROR     , EFI_DEVICE_ERROR, EFI_SUCCESS     , EFI_SUCCESS     , EFI_SUCCESS};
  BOOLEAN                         WaitForResponseStatus[TEST_CASE_5_ROUNDS]  = {TRUE                 , TRUE            , TRUE            , FALSE           , FALSE};
  UINT8                           GetLengthStatus[TEST_CASE_5_ROUNDS]        = {0                    , 0               , 0               , 0               , 1};
  UINT8                           Index;
  UINT8                           ExitLoopCounter;
  EFI_I2C_REQUEST_PACKET          *RequestPacket;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  // Mock Boot services.
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS->OpenProtocol = MockBootServiceOpenProtocol;
  gBS->CloseProtocol = MockBootServiceCloseProtocol;
  gBS->CreateEvent = MockCreateEventforGetHidDescriptor;
  gBS->CloseEvent = MockCloseEvent;
  gBS->Stall = MockMicroSecondDelay;

  // Initial TouchDev.
  TouchDev =  AllocateZeroPool (sizeof (ABSOLUTE_POINTER_DEV));
  TouchDev->Signature = TOUCH_SIGNATURE;
  TouchDev->I2cIoProtocol = AllocateZeroPool (sizeof (EFI_I2C_IO_PROTOCOL));
  TouchDev->I2cIoProtocol->QueueRequest = MockI2cBusQueueRequest;
  TouchDev->HidI2cPlatformSupport.GetHidRegisterAddress = HidI2cGetHidRegisterAddress;

  //
  // Test function GetHidDescriptor will be executed 5 times as follows:
  //
  //        Test1: Mock CreateEvent return EFI_INVALID_PARAMETER.
  //        Expect Result: EFI_INVALID_PARAMETER
  //
  //        Test2: Mock CloseEvent return EFI_DEVICE_ERROR.
  //        Expect Result: EFI_DEVICE_ERROR
  //
  //        Test3: Mock HidDescriptorReceivedEvent() wasn't responded in 100ms, return EFI_DEVICE_ERROR.
  //        Expect Result: EFI_DEVICE_ERROR
  //
  //        Test4: Mock GetLength() failed, return EFI_DEVICE_ERROR.
  //        Expect Result: EFI_DEVICE_ERROR
  //
  //        Test5: Mock all functions work success.
  //        Expect Result: EFI_SUCCESS

  for (Index = 0; Index <= TEST_CASE_5_ROUNDS; Index++) {
    ExitLoopCounter = 100;
    //
    // Mock Create Event and check event type. NotifyTpl and NotifyFunction, then return status.
    //
    expect_value (MockCreateEventforGetHidDescriptor, Type, EVT_NOTIFY_SIGNAL);                   // Mock of Create Event
    expect_value (MockCreateEventforGetHidDescriptor, NotifyTpl, TPL_NOTIFY);
    expect_value (MockCreateEventforGetHidDescriptor, NotifyFunction, HidDescriptorReceivedEvent);
    will_return (MockCreateEventforGetHidDescriptor, WaitForResponseStatus[Index]);
    will_return (MockCreateEventforGetHidDescriptor, CreateEventStatus[Index]);
    if (CreateEventStatus[Index] == EFI_SUCCESS) {
      // Mock of I2cIoProtocol->QueueRequest()
      expect_value (MockI2cBusQueueRequest, SlaveAddressIndex, 0);
      will_return (MockI2cBusQueueRequest, &RequestPacket);
      will_return (MockI2cBusQueueRequest, EFI_SUCCESS);
      // gBS->Stall wait parameter "WaitForResponse" modify to FALSE.
      if (WaitForResponseStatus[Index] == TRUE) {
        while (ExitLoopCounter > 0) {
          will_return (MockMicroSecondDelay, EFI_SUCCESS);
          ExitLoopCounter--;
        }
      }
      will_return (MockCloseEvent, CloseEventStatus[Index]);                                       // Mock of close event.
      // Mock HIDDescLengthLsb equal to 1.
      TouchDev->HidDescriptor.HIDDescLengthLsb = GetLengthStatus[Index];
    }
    Status = GetHidDescriptor (TouchDev);
    // If Test case 1-4, GetHidDescriptor will return failure.
    if (GetLengthStatus[Index] == 1) {
      UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
      // If Test case 5, GetHidDescriptor will return success.
      } else {
        UT_ASSERT_NOT_EQUAL (Status, EFI_SUCCESS);
      }
  }
  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));
  FreePool (gBS);
  FreePool (TouchDev);
  return UNIT_TEST_PASSED;
}