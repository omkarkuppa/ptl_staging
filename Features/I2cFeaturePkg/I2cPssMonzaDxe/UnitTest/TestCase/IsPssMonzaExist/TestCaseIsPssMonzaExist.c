/**@file
    Unit test case of the I2c Pss Monza Dxe driver.

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
#include <I2cPssMonza.h>
#include "TestCaseIsPssMonzaExist.h"
#include "PrivateMock/PrivateMock.h"

/**
  Check PSS Monza chip whether exists.

  @param[in] This                     Pointer of EFI_I2C_IO_PROTOCOL.

  @retval  TRUE                       Detect PSS Monza chip successfully.
  @retval  FALSE                      Unable to detect PSS Monza chip.
**/
extern
BOOLEAN
EFIAPI
IsPssMonzaExist (
  IN EFI_I2C_IO_PROTOCOL *I2cIoProtocol
  );
/**
  Mock IsPssMonzaExist

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
UnitTestIsPssMonzaExist (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  BOOLEAN                  IsChipDetect;
  EFI_I2C_IO_PROTOCOL      *I2cIoProtocol;
  UINT8                    Index;

  // Mock EFI_I2C_IO_PROTOCOL service.
  I2cIoProtocol = AllocateZeroPool (sizeof (EFI_I2C_IO_PROTOCOL));
  I2cIoProtocol->QueueRequest = MockI2cBusQueueRequest;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  //
  // Test1: Unable to detect PSS Monza chip.
  //

  // Mock incorrect Pss chip Class Id [0x0, 0x0, 0x0, 0x0] to I2cIoProtocol->QueueRequest.
  will_return (MockI2cBusQueueRequest, SizeofPssChipClassId);
  for (Index = 0; Index < SizeofPssChipClassId; Index++) {
    will_return (MockI2cBusQueueRequest, UnknownPssChipClassId[Index]);
  }
  will_return (MockI2cBusQueueRequest, EFI_UNSUPPORTED);
  IsChipDetect = IsPssMonzaExist (I2cIoProtocol);
  UT_ASSERT_EQUAL (IsChipDetect, FALSE);
  //
  // Test2: Detect PSS Monza chip successfully.
  //

  // Mock correct Pss chip Class Id [0xE2, 0x80 , 0x11 , 0x50] to I2cIoProtocol->QueueRequest.
  will_return (MockI2cBusQueueRequest, SizeofPssChipClassId);
  for (Index = 0; Index < SizeofPssChipClassId; Index++) {
    will_return (MockI2cBusQueueRequest, PssChipClassId[Index]);
  }
  will_return (MockI2cBusQueueRequest, EFI_SUCCESS);
  IsChipDetect = IsPssMonzaExist (I2cIoProtocol);
  UT_ASSERT_EQUAL (IsChipDetect, TRUE);

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));

  FreePool (I2cIoProtocol);
  return UNIT_TEST_PASSED;
}