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
#include "TestCaseTouchPanelDriverDriverBindingStart.h"
#include <I2cTouchPanel.h>
#include <BootServicesMock.h>
#include <PrivateMock.h>

/**
  Starts touchpanel device controller.
  If initialization completes successfully, this driver will install AbsolutePointer protocol
  on this device. This protocol can be further used for features such as virtual keyboard.

  Initialization steps:
  - Call init function supplied from platform-specific lib to initialize hardware
  - Read device's HID descriptor, according to Microsoft's HID over I2C specification
  - Using data from HID descriptor, build a table of input descriptors. A single HID device may
    produce data in more than 1 format and each needs to be parsed differently.

  Device context caches one touch event. Cache gets invalidated when its data is consumed by calling
  GetState() from AbsolutePointer protocol.
  Driver checks device's interrupt and potentially retrieves new touch data only if
  cache is empty and either GetState() was called or a periodic timer event was triggered.
  When new data is retrieved from device, absolute pointer's WaitForInput event gets signalled
  to inform users of this protocol that new data is available.
  Such construction allows the following uses of Absolute Pointer protocol:
  - consumer calls gBS->WaitForEvent on AbsPtr's WaitForEvent event, then calls GetState which is
    guaranteed to contain valid data (the suggested way of using AbsolutePointer protocol)
  - consumer calls gBS->checkEvent on AbsPtr's WaitForevent event, and if it returned success then
    consumer may call Getstate() which is guaranteed to contain valid data
  - consumer calls GetState() without bothering with events; GetState() either returns valid data
    or an error

  @param[in]  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle     The handle of the controller to start. This handle
                                   must support I2C protocol interface.
  @param[in]  RemainingDevicePath  ignored

  @retval EFI_SUCCESS              The device was started.
  @retval EFI_DEVICE_ERROR         The device could not be started due to a device error.Currently not implemented.
  @retval EFI_OUT_OF_RESOURCES     The request could not be completed due to a lack of resources.
  @retval Others                   The driver failded to start the device.

**/
extern
EFI_STATUS
EFIAPI
TouchPanelDriverDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  );

/**
  Mock of TouchPanelDriverDriverBindingStart

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
UnitTestTouchPanelDriverDriverBindingStart (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_I2C_IO_PROTOCOL             *I2cIoProtocol;
  EFI_STATUS                      Status;
  EFI_DRIVER_BINDING_PROTOCOL     DriverBinding;
  UINT8                           Counter;
  PROTOCOLS_LIST                  *ProtocolList;
  CHAR16                          *UnicodeString1;
  CHAR16                          *UnicodeString2;
  CHAR16                          UnicodeCheck[] = L"HID-compatible Touchpanel Driver";
  EFI_I2C_REQUEST_PACKET          *RequestPacket;
  UINT8                           TestIndex;
  UINT64                          OpenProtocolStatus[TEST_CASE_ROUND]                      = {EFI_UNSUPPORTED,       EFI_SUCCESS          , EFI_SUCCESS,           EFI_SUCCESS,           EFI_SUCCESS, EFI_SUCCESS};
  UINT64                          CreateEventStatus1[TEST_CASE_ROUND]                      = {EFI_INVALID_PARAMETER, EFI_INVALID_PARAMETER, EFI_SUCCESS,           EFI_SUCCESS,           EFI_SUCCESS, EFI_SUCCESS};
  UINT64                          CreateEventStatus2[TEST_CASE_ROUND]                      = {EFI_INVALID_PARAMETER, EFI_INVALID_PARAMETER, EFI_INVALID_PARAMETER, EFI_SUCCESS,           EFI_SUCCESS, EFI_SUCCESS};
  UINT64                          SetTimerStatus[TEST_CASE_ROUND]                          = {EFI_INVALID_PARAMETER, EFI_INVALID_PARAMETER, EFI_INVALID_PARAMETER, EFI_INVALID_PARAMETER, EFI_SUCCESS, EFI_SUCCESS};
  UINT64                          InstallMultipleProtocolInterfacesStatus[TEST_CASE_ROUND] = {EFI_INVALID_PARAMETER, EFI_INVALID_PARAMETER, EFI_INVALID_PARAMETER, EFI_INVALID_PARAMETER, EFI_INVALID_PARAMETER, EFI_SUCCESS};

  ProtocolList = NULL;
  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  //
  // Mock Boot services and I2cIoProtocol.
  //
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS->OpenProtocol = MockBootServiceOpenProtocol;
  gBS->CloseProtocol = MockBootServiceCloseProtocolForStart;
  gBS->InstallMultipleProtocolInterfaces = MockBootServiceInstallMultipleProtocolInterfaces;
  gBS->CreateEvent = MockCreateEventforGetHidDescriptor;
  gBS->SetTimer = MockSetTimer;
  gBS->CloseEvent = MockCloseEvent;
  I2cIoProtocol = AllocateZeroPool (sizeof (EFI_I2C_IO_PROTOCOL));
  I2cIoProtocol->QueueRequest = MockI2cBusQueueRequest;
  for (TestIndex = 0; TestIndex < TEST_CASE_ROUND; TestIndex++ ) {
    //
    // Test function UnitTestTouchPanelDriverDriverBindingStart will be executed 6 times as follows:
    //
    //        Test1: Mock ControllerHandle doesn't support gEfiI2cIoProtocolGuid.
    //        Expect Result: EFI_UNSUPPORTED
    //
    //        Test2: Mock first CreateEvent return EFI_INVALID_PARAMETER.
    //        Expect Result: EFI_INVALID_PARAMETER
    //
    //        Test3: Mock second CreateEvent return EFI_INVALID_PARAMETER.
    //        Expect Result: EFI_INVALID_PARAMETER
    //
    //        Test4: Mock Event or Type in SetTimer() is not valid, return EFI_INVALID_PARAMETER.
    //        Expect Result: EFI_INVALID_PARAMETER
    //
    //        Test5: Mock InstallMultipleProtocolInterfaces() if Protocol is already installed on the handle specified by Handle.
    //        Expect Result: EFI_INVALID_PARAMETER
    //
    //        Test6: Mock all functions work success.
    //        Expect Result: EFI_SUCCESS
    //
    expect_value (MockBootServiceOpenProtocol, Protocol, &gEfiI2cIoProtocolGuid);                           // Assume TouchPanelDriverDriverBindingStart will locate gEfiI2cIoProtocolGuid
    will_return (MockBootServiceOpenProtocol, OpenProtocolStatus[TestIndex]);                               // Mock return Status = EFI_SUCCESS
    will_return (MockBootServiceOpenProtocol, I2cIoProtocol);
    if (OpenProtocolStatus[TestIndex] == EFI_SUCCESS) {
      //
      // Mock AddUnicodeString2 and return it's UnicodeString.
      //
      will_return (AddUnicodeString2, &UnicodeString1);
      will_return (AddUnicodeString2, EFI_SUCCESS);
      will_return (AddUnicodeString2, &UnicodeString2);
      will_return (AddUnicodeString2, EFI_SUCCESS);

      //
      // Mock Create Event and check event type. NotifyTpl and NotifyFunction, then return status.
      //
      expect_value (MockCreateEventforGetHidDescriptor, Type, EVT_NOTIFY_WAIT);
      expect_value (MockCreateEventforGetHidDescriptor, NotifyTpl, TPL_CALLBACK);
      expect_value (MockCreateEventforGetHidDescriptor, NotifyFunction, CheckDataAvailableCallback);
      will_return (MockCreateEventforGetHidDescriptor, CreateEventStatus1[TestIndex]);
      if (CreateEventStatus1[TestIndex] == EFI_SUCCESS) {
        //
        // TouchDevInit() start. Below functions which in TouchDevInit() will return EFI_SUCCESS every time,
        // TouchDevInit() will careful examination on other test case.
        //

        // GetHidDescriptor Start
        expect_value (MockCreateEventforGetHidDescriptor, Type, EVT_NOTIFY_SIGNAL);                   // Mock of Create Event
        expect_value (MockCreateEventforGetHidDescriptor, NotifyTpl, TPL_NOTIFY);
        expect_value (MockCreateEventforGetHidDescriptor, NotifyFunction, HidDescriptorReceivedEvent);
        will_return (MockCreateEventforGetHidDescriptor, FALSE);
        will_return (MockCreateEventforGetHidDescriptor, EFI_SUCCESS);
        expect_value (MockI2cBusQueueRequest, SlaveAddressIndex, 0);                                 // Mock of I2cIoProtocol->QueueRequest()
        will_return (MockI2cBusQueueRequest, &RequestPacket);
        will_return (MockI2cBusQueueRequest, EFI_SUCCESS);
        will_return (MockCloseEvent, EFI_SUCCESS);                                                   // Mock of close event.
        // GetHidDescriptor end

        // SetHidPower Start
        expect_value (MockI2cBusQueueRequest, SlaveAddressIndex, 0);                                 // Mock of I2cIoProtocol->QueueRequest()
        will_return (MockI2cBusQueueRequest, &RequestPacket);
        will_return (MockI2cBusQueueRequest, EFI_SUCCESS);
        // SetHidPower end

        // SetHidReset Start
        expect_value (MockI2cBusQueueRequest, SlaveAddressIndex, 0);                                 // Mock of I2cIoProtocol->QueueRequest()
        will_return (MockI2cBusQueueRequest, &RequestPacket);
        will_return (MockI2cBusQueueRequest, EFI_SUCCESS);
        // SetHidReset end

        // ReadInputReportDescriptor Start
        expect_value (MockI2cBusQueueRequest, SlaveAddressIndex, 0);                                 // Mock of I2cIoProtocol->QueueRequest()
        will_return (MockI2cBusQueueRequest, &RequestPacket);
        will_return (MockI2cBusQueueRequest, EFI_SUCCESS);
        // ReadInputReportDescriptor end

        // SetInterruptFrequency Start
        expect_value (MockI2cBusQueueRequest, SlaveAddressIndex, 0);                                 // Mock of I2cIoProtocol->QueueRequest()
        will_return (MockI2cBusQueueRequest, &RequestPacket);
        will_return (MockI2cBusQueueRequest, EFI_SUCCESS);
        expect_value (MockI2cBusQueueRequest, SlaveAddressIndex, 0);
        will_return (MockI2cBusQueueRequest, &RequestPacket);
        will_return (MockI2cBusQueueRequest, EFI_SUCCESS);
        // SetInterruptFrequency() end, TouchDevInit() end. Return to TouchPanelDriverDriverBindingStart().

        //
        // Mock Create Event and check event type. NotifyTpl and NotifyFunction, then return status.
        //
        expect_value (MockCreateEventforGetHidDescriptor, Type, EVT_TIMER | EVT_NOTIFY_SIGNAL);
        expect_value (MockCreateEventforGetHidDescriptor, NotifyTpl, TPL_CALLBACK);
        expect_value (MockCreateEventforGetHidDescriptor, NotifyFunction, CheckDataAvailableCallback);
        will_return (MockCreateEventforGetHidDescriptor, CreateEventStatus2[TestIndex]);
        if (CreateEventStatus2[TestIndex] == EFI_SUCCESS) {
          // Mock of SetTimer.
          expect_value (MockSetTimer, TriggerTime, EFI_TIMER_PERIOD_MICROSECONDS (1));
          will_return (MockSetTimer, SetTimerStatus[TestIndex]);
          if (SetTimerStatus[TestIndex] == EFI_SUCCESS) {
            //
            // Mock InstallMultipleProtocolInterfaces. All protocols will store in ProtocolList.
            // Then check protocols that we expected by CompareGuid().
            //
            will_return (MockBootServiceInstallMultipleProtocolInterfaces, &ProtocolList);
            will_return (MockBootServiceInstallMultipleProtocolInterfaces, &Counter);
            will_return (MockBootServiceInstallMultipleProtocolInterfaces, InstallMultipleProtocolInterfacesStatus[TestIndex]);
          }
        }
      }
    }
      Status = TouchPanelDriverDriverBindingStart  (&DriverBinding, NULL, NULL);
      // If Test case 1-5, TouchPanelDriverDriverBindingStart will return failure.
      if (InstallMultipleProtocolInterfacesStatus[TestIndex] == EFI_SUCCESS) {
        UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
      // If Test case 6, TouchPanelDriverDriverBindingStart will return success.
      } else {
        UT_ASSERT_NOT_EQUAL (Status, EFI_SUCCESS);
      }
  }
  // Check ProtocolList include gEdkiiTouchPanelGuid and gEfiAbsolutePointerProtocolGuid.
  if (((CompareGuid (ProtocolList[0].Protocol, &gEfiAbsolutePointerProtocolGuid)) && (CompareGuid (ProtocolList[1].Protocol, &gEdkiiTouchPanelGuid))) ||
      ((CompareGuid (ProtocolList[0].Protocol, &gEdkiiTouchPanelGuid)) && (CompareGuid (ProtocolList[1].Protocol, &gEfiAbsolutePointerProtocolGuid)))) {
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_INVALID_PARAMETER;
      }
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  // Check UnicodeString which in AddUnicodeString2() is "HID-compatible Touchpanel Driver".
  if ((!StrCmp (UnicodeString1,UnicodeCheck)) && (!StrCmp (UnicodeString2,UnicodeCheck))) {
    Status = EFI_SUCCESS;
  } else {
  // If UnicodeString not we expected.
    Status =EFI_INVALID_PARAMETER;
  }
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));
  FreePool (gBS);
  FreePool (I2cIoProtocol);
  return UNIT_TEST_PASSED;
}