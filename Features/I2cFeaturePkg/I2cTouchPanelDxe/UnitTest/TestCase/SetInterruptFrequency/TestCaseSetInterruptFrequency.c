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
#include "TestCaseSetInterruptFrequency.h"
#include <I2cTouchPanel.h>
#include <PrivateMock.h>

/**
  Mock of SetInterruptFrequency

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
UnitTestSetInterruptFrequency (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  ABSOLUTE_POINTER_DEV       *TouchDev;
  EFI_STATUS                 Status;
  EFI_I2C_REQUEST_PACKET     *RequestPacket;
  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  // Inital ABSOLUTE_POINTER_DEV structure.
  TouchDev = AllocateZeroPool (sizeof (ABSOLUTE_POINTER_DEV));
  TouchDev->I2cIoProtocol = AllocateZeroPool (sizeof (EFI_I2C_IO_PROTOCOL));
  TouchDev->I2cIoProtocol->QueueRequest = MockI2cBusQueueRequest;
  TouchDev->InputReportTable.Quantity = 1;
  TouchDev->InputReportTable.Report = AllocateZeroPool (sizeof (INPUT_REPORT_FORMAT));
  TouchDev->InputReportTable.Report[0].Id = 0;
  TouchDev->HidDescriptor.DataRegisterLsb = 0;
  TouchDev->HidDescriptor.DataRegisterMsb = 0;
  TouchDev->HidDescriptor.CommandRegisterLsb = 0;
  TouchDev->HidDescriptor.CommandRegisterMsb = 0;

  //
  // Test function SetInterruptFrequency will be executed 3 times as follows:
  //
  //        Test1: Mock first I2cIoProtocol->QueueRequest failed. return EFI_UNSUPPORTED.
  //        Expect Result: EFI_UNSUPPORTED
  //
  expect_value (MockI2cBusQueueRequest, SlaveAddressIndex, 0);                                 // Mock of I2cIoProtocol->QueueRequest()
  will_return (MockI2cBusQueueRequest, &RequestPacket);
  will_return (MockI2cBusQueueRequest, EFI_UNSUPPORTED);

  Status = SetInterruptFrequency (TouchDev);
  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);
  //
  //        Test2: Mock second I2cIoProtocol->QueueRequest failed. return EFI_UNSUPPORTED.
  //        Expect Result: EFI_UNSUPPORTED
  //
  expect_value (MockI2cBusQueueRequest, SlaveAddressIndex, 0);                                 // Mock of I2cIoProtocol->QueueRequest()
  will_return (MockI2cBusQueueRequest, &RequestPacket);
  will_return (MockI2cBusQueueRequest, EFI_SUCCESS);
  expect_value (MockI2cBusQueueRequest, SlaveAddressIndex, 0);                                 // Mock of I2cIoProtocol->QueueRequest()
  will_return (MockI2cBusQueueRequest, &RequestPacket);
  will_return (MockI2cBusQueueRequest, EFI_UNSUPPORTED);

  Status = SetInterruptFrequency (TouchDev);
  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);
  //
  //        Test3: Mock all functions work successful.
  //        Expect Result: EFI_SUCCESS
  //
  expect_value (MockI2cBusQueueRequest, SlaveAddressIndex, 0);                                 // Mock of I2cIoProtocol->QueueRequest()
  will_return (MockI2cBusQueueRequest, &RequestPacket);
  will_return (MockI2cBusQueueRequest, EFI_SUCCESS);
  expect_value (MockI2cBusQueueRequest, SlaveAddressIndex, 0);                                 // Mock of I2cIoProtocol->QueueRequest()
  will_return (MockI2cBusQueueRequest, &RequestPacket);
  will_return (MockI2cBusQueueRequest, EFI_SUCCESS);

  Status = SetInterruptFrequency (TouchDev);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));
  FreePool (TouchDev);
  return UNIT_TEST_PASSED;
}