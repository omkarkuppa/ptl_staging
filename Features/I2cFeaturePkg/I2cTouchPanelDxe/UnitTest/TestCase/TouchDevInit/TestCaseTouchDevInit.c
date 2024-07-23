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
#include "TestCaseTouchDevInit.h"
#include <I2cTouchPanel.h>
#include <BootServicesMock.h>
#include <PrivateMock.h>

/**
  Mock of TouchDevInit

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
UnitTestTouchDevInit (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                      Status;
  EFI_I2C_REQUEST_PACKET          *RequestPacket;
  ABSOLUTE_POINTER_DEV            *TouchDev;
  UINT64                          GetHidDescriptorStatus[]          = {EFI_DEVICE_ERROR, EFI_SUCCESS     , EFI_SUCCESS     , EFI_SUCCESS     , EFI_SUCCESS     , EFI_SUCCESS};
  UINT64                          SetHidPowerStatus[]               = {EFI_DEVICE_ERROR, EFI_DEVICE_ERROR, EFI_SUCCESS     , EFI_SUCCESS     , EFI_SUCCESS     , EFI_SUCCESS};
  UINT64                          SetHidResetStatus[]               = {EFI_DEVICE_ERROR, EFI_DEVICE_ERROR, EFI_DEVICE_ERROR, EFI_SUCCESS     , EFI_SUCCESS     , EFI_SUCCESS};
  UINT64                          ReadInputReportDescriptorStatus[] = {EFI_DEVICE_ERROR, EFI_DEVICE_ERROR, EFI_DEVICE_ERROR, EFI_DEVICE_ERROR, EFI_SUCCESS     , EFI_SUCCESS};
  UINT64                          SetInterruptFrequencyStatus[]     = {EFI_DEVICE_ERROR, EFI_DEVICE_ERROR, EFI_DEVICE_ERROR, EFI_DEVICE_ERROR, EFI_DEVICE_ERROR, EFI_SUCCESS};
  UINT64                          TouchDevInitStatus[]              = {EFI_DEVICE_ERROR, EFI_DEVICE_ERROR, EFI_DEVICE_ERROR, EFI_DEVICE_ERROR, EFI_DEVICE_ERROR, EFI_SUCCESS};
  UINT8                           Index;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  //
  // Mock Boot Services and inital ABSOLUTE_POINTER_DEV structure.
  //
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS->CreateEvent = MockCreateEventforGetHidDescriptor;
  gBS->CloseEvent = MockCloseEvent;
  TouchDev = AllocateZeroPool (sizeof (ABSOLUTE_POINTER_DEV));
  TouchDev->I2cIoProtocol = AllocateZeroPool (sizeof (EFI_I2C_IO_PROTOCOL));
  TouchDev->I2cIoProtocol->QueueRequest = MockI2cBusQueueRequest;
  TouchDev->HidI2cPlatformSupport.GetHidRegisterAddress = HidI2cGetHidRegisterAddress;
  TouchDev->InputReportTable.Quantity = 1;
  //
  // Test function UnitTestTouchPanelDriverDriverBindingStart will be executed 6 times as follows:
  //
  //        Test1: Mock GetHidDescriptor() return EFI_DEVICE_ERROR.
  //        Expect Result: EFI_DEVICE_ERROR
  //
  //        Test2: Mock SetHidPower(), return EFI_DEVICE_ERROR.
  //        Expect Result: EFI_DEVICE_ERROR
  //
  //        Test3: Mock SetHidReset() was failed, return EFI_DEVICE_ERROR.
  //        Expect Result: EFI_DEVICE_ERROR
  //
  //        Test4: Mock ReadInputReportDescriptor() was failed, return EFI_DEVICE_ERROR.
  //        Expect Result: EFI_DEVICE_ERROR
  //
  //        Test5: Mock SetInterruptFrequency() was failed, return EFI_DEVICE_ERROR.
  //        Expect Result: EFI_DEVICE_ERROR
  //
  //        Test6: Mock all functions work success.
  //        Expect Result: EFI_SUCCESS
  //
  //
  for (Index = 0; Index <= TEST_CASE_5_ROUNDS; Index++) {
    // GetHidDescriptor Start
    TouchDev->HidDescriptor.HIDDescLengthLsb = 1;
    expect_value (MockCreateEventforGetHidDescriptor, Type, EVT_NOTIFY_SIGNAL);                      // Mock of Create Event
    expect_value (MockCreateEventforGetHidDescriptor, NotifyTpl, TPL_NOTIFY);
    expect_value (MockCreateEventforGetHidDescriptor, NotifyFunction, HidDescriptorReceivedEvent);
    will_return (MockCreateEventforGetHidDescriptor, FALSE);
    will_return (MockCreateEventforGetHidDescriptor, GetHidDescriptorStatus[Index]);
    // Check CreateEvent status which in GetHidDescriptor() return EFI_SUCCESS.
    if (GetHidDescriptorStatus[Index] == EFI_SUCCESS) {
      expect_value (MockI2cBusQueueRequest, SlaveAddressIndex, 0);                                   // Mock of I2cIoProtocol->QueueRequest()
      will_return (MockI2cBusQueueRequest, &RequestPacket);
      will_return (MockI2cBusQueueRequest, EFI_SUCCESS);
      will_return (MockCloseEvent, EFI_SUCCESS);                                                     // Mock of close event.
      // GetHidDescriptor end

      // SetHidPower Start
      expect_value (MockI2cBusQueueRequest, SlaveAddressIndex, 0);                                   // Mock of I2cIoProtocol->QueueRequest()
      will_return (MockI2cBusQueueRequest, &RequestPacket);
      will_return (MockI2cBusQueueRequest, SetHidPowerStatus[Index]);
      // SetHidPower end
      if (SetHidPowerStatus[Index] == EFI_SUCCESS) {
        // SetHidReset Start
        expect_value (MockI2cBusQueueRequest, SlaveAddressIndex, 0);                                 // Mock of I2cIoProtocol->QueueRequest()
        will_return (MockI2cBusQueueRequest, &RequestPacket);
        will_return (MockI2cBusQueueRequest, SetHidResetStatus[Index]);
        // SetHidReset end
        if (SetHidResetStatus[Index] == EFI_SUCCESS) {
          // ReadInputReportDescriptor Start
          expect_value (MockI2cBusQueueRequest, SlaveAddressIndex, 0);                               // Mock of I2cIoProtocol->QueueRequest()
          will_return (MockI2cBusQueueRequest, &RequestPacket);
          will_return (MockI2cBusQueueRequest, ReadInputReportDescriptorStatus[Index]);
          // ReadInputReportDescriptor end
          if (ReadInputReportDescriptorStatus[Index] == EFI_SUCCESS) {
            // SetInterruptFrequency Start
            expect_value (MockI2cBusQueueRequest, SlaveAddressIndex, 0);                             // Mock of I2cIoProtocol->QueueRequest()
            will_return (MockI2cBusQueueRequest, &RequestPacket);
            will_return (MockI2cBusQueueRequest, EFI_SUCCESS);
            expect_value (MockI2cBusQueueRequest, SlaveAddressIndex, 0);
            will_return (MockI2cBusQueueRequest, &RequestPacket);
            will_return (MockI2cBusQueueRequest, SetInterruptFrequencyStatus[Index]);
            // SetInterruptFrequency() end, TouchDevInit() end.
          }
        }
      }
    }
    Status = TouchDevInit  (TouchDev);
    UT_ASSERT_EQUAL (Status, TouchDevInitStatus[Index]);
  }
  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));
  FreePool (gBS);
  FreePool (TouchDev);
  return UNIT_TEST_PASSED;
}