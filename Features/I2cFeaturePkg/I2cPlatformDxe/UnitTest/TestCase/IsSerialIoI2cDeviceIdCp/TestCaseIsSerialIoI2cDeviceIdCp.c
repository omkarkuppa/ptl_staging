/**@file
    Unit test case of the I2c Platform Dxe driver.

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
#include "TestCaseIsSerialIoI2cDeviceIdCp.h"
#include <I2cPlatformDxe.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include "PrivateMock/PrivateMock.h"
#include "TestCase/GetSocSeries/TestCaseGetSocSeries.h"

/**
  Checks if Device with given PciDeviceId is one of SerialIo I2C controllers
  If yes, its number is returned through I2cIndex parameter, otherwise I2cIndex is not updated

  @param[in]  PciDevId                  Device ID
  @param[out] I2cNumber                 Number of SerialIo I2C controller

  @retval TRUE                          yes it is a SerialIo I2C controller
  @retval FALSE                         no it isn't a SerialIo I2C controller
**/
extern
BOOLEAN
IsSerialIoI2cDeviceIdCp (
  IN  UINT16    PciDevId,
  OUT UINT8     *I2cNumber
  );

/**
  Mock IsSerialIoI2cDeviceIdCp for I2c Platform Dxe.

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
UnitTestIsSerialIoI2cDeviceIdCp (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINT8                    I2cNumber;
  SERIAL_IO_I2C_DEVID      *SerialIoI2cDevid;
  UINT8                    *I2cDevidTable;
  UINT8                    Index;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  //
  // Mock SERIAL_IO_I2C_DEVID structure.
  //
  SerialIoI2cDevid = AllocateZeroPool (sizeof (SERIAL_IO_I2C_DEVID));
  SerialIoI2cDevid->Count = MAX_I2C_CONTROLLERS_NUM;
  for (Index = 0; Index < SerialIoI2cDevid->Count; Index++) {
    SerialIoI2cDevid->SerialIoI2cDevId[Index] = GetLpssI2cDeviceId (Index);
  }
  I2cDevidTable = (UINT8 *)SerialIoI2cDevid;
  will_return_always (LibPcdGetExPtr, I2cDevidTable);

  //
  // Test function IsSerialIoI2cDeviceIdCp with input parameter using incorrect PciDevId case
  //
  // Expect Result:
  //   - FALSE: Test case have error handle with incorrect PciDevId input parameter
  //
  //

  // Check PcdSerialIoI2cDevId
  expect_value (LibPcdGetExPtr, TokenNumber, _PCD_TOKEN_gI2cFeaturePkgTokenSpaceGuid_PcdSerialIoI2cDevId);
  UT_ASSERT_EQUAL (IsSerialIoI2cDeviceIdCp (0, &I2cNumber), FALSE);

  //
  // Test function IsSerialIoI2cDeviceIdCp with input parameter using Mock case
  //
  // Expect Result:
  //   - TRUE:   It is a SerialIo I2C controller
  //   - FALSE:  It isn't a SerialIo I2C controller

  //
  //  Serial IO I2C0 - I2C5 Controllers DevId
  //
  for (Index = 0; Index < SerialIoI2cDevid->Count; Index++) {
    expect_value (LibPcdGetExPtr, TokenNumber, _PCD_TOKEN_gI2cFeaturePkgTokenSpaceGuid_PcdSerialIoI2cDevId);
    UT_ASSERT_EQUAL (IsSerialIoI2cDeviceIdCp (SerialIoI2cDevid->SerialIoI2cDevId[Index], &I2cNumber), TRUE);
  }
  //
  // Destroy Mock Service
  //
  FreePool (SerialIoI2cDevid);
  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
  return UNIT_TEST_PASSED;
}
