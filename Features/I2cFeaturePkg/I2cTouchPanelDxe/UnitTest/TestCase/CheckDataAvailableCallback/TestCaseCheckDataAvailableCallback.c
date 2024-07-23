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
#include "TestCaseCheckDataAvailableCallback.h"
#include <Library/DebugLib.h>
#include <I2cTouchPanel.h>
#include <Library/UefiBootServicesTableLib.h>
#include "PrivateMock/PrivateMock.h"
#include <BootServicesMock.h>
/**
  Check the touch device do send interrupt event.

  @param[in] Event    Event whose notification function is being invoked.
  @param[in] Context  Pointer to the notification function's context,
                      which is implementation-dependent.  Context corresponds
                      to NotifyContext in CreateEventEx().
**/
VOID
EFIAPI
CheckDataAvailableCallback (
  IN EFI_EVENT Event,
  IN VOID      *Context
  );

/**
  Mock of CheckDataAvailableCallback.

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
UnitTestCheckDataAvailableCallback (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  ABSOLUTE_POINTER_DEV            *TouchDev;
  UINT8                           Index;
  UINT8                           BufferSize;
  UINT8                           Buffer[] = {0, 0, 0};

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  //
  // Test function UnitTestTouchPanelDriverDriverBindingStart will be executed 1 time as follows:
  //
  //        Test1: Mock all functions work success.
  //        Expect Result: EFI_SUCCESS
  //

  // Mock TouchDev structure.
  TouchDev = AllocateZeroPool (sizeof (ABSOLUTE_POINTER_DEV));
  TouchDev->ControllerHandle = NULL;
  TouchDev->HidDescriptor.MaxInputLengthLsb = 0;
  TouchDev->HidDescriptor.MaxInputLengthMsb = 2;
  TouchDev->I2cIoProtocol = AllocateZeroPool (sizeof (EFI_I2C_IO_PROTOCOL));
  TouchDev->I2cIoProtocol->QueueRequest = MockI2cBusQueueRequestReturnRequestPacket;
  TouchDev->InputReportTable.Quantity = 3;
  TouchDev->InputReportTable.Report = AllocateZeroPool (sizeof (INPUT_REPORT_FORMAT)* 3);
  TouchDev->NewStateAvailable = FALSE;
  TouchDev->HidI2cPlatformSupport.CheckInterrupt = HidI2cCheckInterrupt;
  BufferSize = 3;
  //
  // Mock boot service SignalEvent.
  //
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS->SignalEvent = MockSignalEvent;

  // Mock I2cIoProtocol->QueueRequest and RequestPacket->Operation[0].Buffer to GetTouchCoordinates().
  will_return (MockI2cBusQueueRequestReturnRequestPacket, BufferSize);
  for (Index = 0; Index < BufferSize; Index++) {
    will_return (MockI2cBusQueueRequestReturnRequestPacket, Buffer[Index]);
  }
  will_return (MockI2cBusQueueRequestReturnRequestPacket, EFI_SUCCESS);
  // Mock gBS->SignalEvent and return EFI_SUCCESS.
  will_return (MockSignalEvent, EFI_SUCCESS);
  CheckDataAvailableCallback (NULL, TouchDev);
  // Check TouchDev->NewStateAvailable is changed to TRUE by CheckDataAvailableCallback(),
  //because TouchDev->CurrentX.CurrentY.CurrentZ and NewStateAvailable is changed by CheckDataAvailableCallback().
  UT_ASSERT_EQUAL (TouchDev->NewStateAvailable, TRUE);
  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));

  FreePool (TouchDev);
  return UNIT_TEST_PASSED;
}