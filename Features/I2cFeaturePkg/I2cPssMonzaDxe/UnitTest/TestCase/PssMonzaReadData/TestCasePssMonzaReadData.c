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
#include "TestCasePssMonzaReadData.h"
#include "PrivateMock/PrivateMock.h"

const UINT8 UnknownPssChipClassId[] = {0x0, 0x0 , 0x0 , 0x0};
const UINT8 PssChipClassId[] = {0xE2, 0x80 , 0x11 , 0x50};
const UINT8 SizeofPssChipClassId = sizeof (PssChipClassId);


/**
  Read data from PSS chip.

  @param[in]   I2cIoProtocol           Pointer of EFI_I2C_IO_PROTOCOL.
  @param[out]  Buffer                  Return read data from PSS chip
  @param[in]   Address                 Address located in PSS chip
  @param[in]   Size                    Data Size

  @retval  EFI_DEVICE_ERROR            Fail to QueueRequest I2C device
  @retval  EFI_INVALID_PARAMETER       Buffer is NULL
  @retval  EFI_OUT_OF_RESOURCES        Don't have enough to create I2C request packet.
  @retval  EFI_SUCCESS                 Read data from PSS chip successfully
**/
extern
EFI_STATUS
EFIAPI
PssMonzaReadData (
  IN  EFI_I2C_IO_PROTOCOL *I2cIoProtocol,
  OUT UINT8               *Buffer,
  IN  UINT32              Address,
  IN  UINTN               Size
  );

/**
  Mock PssMonzaReadData

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
UnitTestPssMonzaReadData (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINTN                  Size;
  EFI_STATUS             Status;
  EFI_I2C_IO_PROTOCOL    *I2cIoProtocol;
  UINT8                  *Buffer;

  // Mock I2cIoProtocol service.
  I2cIoProtocol = AllocateZeroPool (sizeof (EFI_I2C_IO_PROTOCOL));
  I2cIoProtocol->QueueRequest = MockI2cBusQueueRequest;
  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  //
  // Test1: If Size = 0, Status will return EFI_SUCCESS.
  // Expected Result: EFI_SUCCESS
  //
  Size = 0;
  Status = PssMonzaReadData (I2cIoProtocol, NULL, 0, Size);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  //
  // Test2: If Size isn't 0 but Buffer is NULL, Status will return EFI_INVALID_PARAMETER.
  // Expected Result: EFI_INVALID_PARAMETER
  //
  Size = PSS_SN_LENGTH;
  Status = PssMonzaReadData (I2cIoProtocol, NULL, 0, Size);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);
  //
  // Test3: If I2cIoProtocol->QueueRequest return error, Status will return EFI_DEVICE_ERROR.
  // Expected Result: EFI_DEVICE_ERROR
  //
  Size = PSS_SN_LENGTH;
  Buffer = AllocateZeroPool (sizeof (UINT8));
  // Mock I2cIoProtocol->QueueRequest status only.
  will_return (MockI2cBusQueueRequest, 0);
  will_return (MockI2cBusQueueRequest, EFI_UNSUPPORTED);
  Status = PssMonzaReadData (I2cIoProtocol, Buffer, 0, Size);
  UT_ASSERT_EQUAL (Status, EFI_DEVICE_ERROR);
  //
  // Test4: I2cIoProtocol->QueueRequest return EFI_SUCCESS, Status will return EFI_SUCCESS.
  // Expected Result: EFI_SUCCESS
  //
  Size = PSS_SN_LENGTH;
  Buffer = AllocateZeroPool (sizeof (UINT8));
  // Mock I2cIoProtocol->QueueRequest status only.
  will_return (MockI2cBusQueueRequest, 0);
  will_return (MockI2cBusQueueRequest, EFI_SUCCESS);
  Status = PssMonzaReadData (I2cIoProtocol, Buffer, 0, Size);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));
  //
  // Destroy I2cIoProtocol and Buffer.
  //
  FreePool (Buffer);
  FreePool (I2cIoProtocol);

  return UNIT_TEST_PASSED;
}