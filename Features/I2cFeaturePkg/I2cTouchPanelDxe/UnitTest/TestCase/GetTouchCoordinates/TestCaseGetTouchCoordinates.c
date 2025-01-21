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
#include "TestCaseGetTouchCoordinates.h"
#include <Library/DebugLib.h>
#include <I2cTouchPanel.h>
#include <PrivateMock.h>
/**
  Get new touch data via I2C.

  @param  DriverContext         The Code context.
  @param  *X                    The touch controller report x location.
  @param  *Y                    The touch controller report y location.
  @param  *Z                    The touch controller report z location.
  @param  ActiveButtons         The touch controller report touch/release event.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.
**/
extern
EFI_STATUS
GetTouchCoordinates (
  IN     ABSOLUTE_POINTER_DEV  *TouchDev,
  IN OUT UINT16                *X,
  IN OUT UINT16                *Y,
  IN OUT UINT16                *Z,
  IN OUT UINT16                *ActiveButtons
  );
/**
  Mock of GetTouchCoordinates.

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
UnitTestGetTouchCoordinates (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                      Status;
  ABSOLUTE_POINTER_DEV            *TouchDev;
  EFI_I2C_REQUEST_PACKET          *RequestPacket;
  UINT8                           Index;
  UINT8                           BufferSize;
  UINT8                           Buffer[] = {0, 0, 0};
  UINT16                          *X;
  UINT16                          *Y;
  UINT16                          *Z;
  UINT16                          *ActiveButtons;
  // Inital TouchDev structure.
  TouchDev = AllocateZeroPool (sizeof (ABSOLUTE_POINTER_DEV));
  TouchDev->I2cIoProtocol = AllocateZeroPool (sizeof (EFI_I2C_IO_PROTOCOL));
  TouchDev->I2cIoProtocol->QueueRequest = MockI2cBusQueueRequest;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  //
  // Test function GetTouchCoordinates will be executed 6 times as follows:
  //
  //        Test1: Mock ReadBuffer Allocates memory failed. return EFI_OUT_OF_RESOURCES.
  //        Expect Result: EFI_OUT_OF_RESOURCES
  //
  TouchDev->HidDescriptor.MaxInputLengthLsb = 0;
  TouchDev->HidDescriptor.MaxInputLengthMsb = 0;
  Status = GetTouchCoordinates (TouchDev, NULL, NULL, NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_OUT_OF_RESOURCES);
  //
  //        Test2: Mock I2cIoProtocol->QueueRequest return EFI_UNSUPPORTED.
  //        Expect Result: EFI_UNSUPPORTED
  //
  TouchDev->HidDescriptor.MaxInputLengthLsb = 1;
  TouchDev->HidDescriptor.MaxInputLengthMsb = 1;
  expect_value (MockI2cBusQueueRequest, SlaveAddressIndex, 0);                                 // Mock of I2cIoProtocol->QueueRequest()
  will_return (MockI2cBusQueueRequest, &RequestPacket);
  will_return (MockI2cBusQueueRequest, EFI_UNSUPPORTED);
  Status = GetTouchCoordinates (TouchDev, NULL, NULL, NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);
  //        Test3: Mock ParseInput() failed. return EFI_NOT_FOUND.
  //        Expect Result: EFI_NOT_FOUND
  //
  expect_value (MockI2cBusQueueRequest, SlaveAddressIndex, 0);                                 // Mock of I2cIoProtocol->QueueRequest()
  will_return (MockI2cBusQueueRequest, &RequestPacket);
  will_return (MockI2cBusQueueRequest, EFI_SUCCESS);
  Status = GetTouchCoordinates (TouchDev, NULL, NULL, NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_NOT_FOUND);
  //        Test4: Mock all functions work success.
  //        Expect Result: EFI_SUCCESS
  //

  // Mock TouchDev structure.
  TouchDev->HidDescriptor.MaxInputLengthLsb = 0;
  TouchDev->HidDescriptor.MaxInputLengthMsb = 2;
  TouchDev->I2cIoProtocol->QueueRequest = MockI2cBusQueueRequestReturnRequestPacket;
  TouchDev->InputReportTable.Quantity = 3;
  TouchDev->InputReportTable.Report = AllocateZeroPool (sizeof (INPUT_REPORT_FORMAT)* 3);
  TouchDev->InputReportTable.Report[0].Id = 0;
  TouchDev->InputReportTable.Report[0].CollectionCount = 1;
  BufferSize = 3;

  // Mock GetTouchCoordinates parameters.
  X = AllocateZeroPool (sizeof (UINT16));
  Y = AllocateZeroPool (sizeof (UINT16));
  Z = AllocateZeroPool (sizeof (UINT16));
  ActiveButtons = AllocateZeroPool (sizeof (UINT16));

  // Mock I2cIoProtocol->QueueRequest and RequestPacket->Operation[0].Buffer to GetTouchCoordinates().
  will_return (MockI2cBusQueueRequestReturnRequestPacket, BufferSize);
  for (Index = 0; Index < BufferSize; Index++) {
    will_return (MockI2cBusQueueRequestReturnRequestPacket, Buffer[Index]);
  }
  will_return (MockI2cBusQueueRequestReturnRequestPacket, EFI_SUCCESS);
  Status = GetTouchCoordinates (TouchDev, X, Y, Z, ActiveButtons);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));

  FreePool (X);
  FreePool (Y);
  FreePool (Z);
  FreePool (ActiveButtons);
  FreePool (TouchDev);
  return UNIT_TEST_PASSED;
}