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
#include "TestCaseEnumerate.h"
#include <I2cPlatformDxe.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include "PrivateMock/PrivateMock.h"
#include <Library/MemoryAllocationLib.h>
/**
  Enumerate the I2C devices

  This function enables the caller to traverse the set of I2C devices
  on an I2C bus.

  @param[in]  This              The platform data for the next device on
                                the I2C bus was returned successfully.
  @param[in, out] Device        Pointer to a buffer containing an
                                EFI_I2C_DEVICE structure.  Enumeration is
                                started by setting the initial EFI_I2C_DEVICE
                                structure pointer to NULL.  The buffer
                                receives an EFI_I2C_DEVICE structure pointer
                                to the next I2C device.

  @retval EFI_SUCCESS           The platform data for the next device on
                                the I2C bus was returned successfully.
  @retval EFI_INVALID_PARAMETER Device is NULL
  @retval EFI_NO_MAPPING        *Device does not point to a valid
                                EFI_I2C_DEVICE structure returned in a
                                previous call Enumerate().
  @retval EFI_NOT_FOUND         No any I2c devices on the I2C bus.
**/
extern
EFI_STATUS
EFIAPI
Enumerate (
  IN CONST EFI_I2C_ENUMERATE_PROTOCOL *This,
  IN OUT CONST EFI_I2C_DEVICE         **Device
  );
/**
  Mock Enumerate.

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
UnitTestEnumerate (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_I2C_DEVICE                  *I2cDevice;
  I2C_PLATFORM_CONTEXT            *Private;
  EFI_STATUS                      Status;
  UINT8                           I2cConDevNum;
  EFI_I2C_DEVICE_PCD              I2cDeviceListI2c0 [] = {{0, 0, 0, 0, 0, 0}};
  UINT8                           Index;

  Status = EFI_SUCCESS;
  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));
  //
  // Mock I2C_PLATFORM_CONTEXT.
  //
  Private = AllocateZeroPool (sizeof (I2C_PLATFORM_CONTEXT));
  Private->Signature = I2C_PLATFORM_SIGNATURE;
  //
  // Test Function "Enumerate"
  // Mock Data
  //  1.Mock I2C device
  //
  // Expected Result
  //
  // EFI_SUCCESS: 1.if I2C device was found.
  //

  //
  // Test1: Device = NULL.
  // Expected Result
  // EFI_INVALID_PARAMETER: if I2C device was NULL.
  //
  for (Private->I2cNumber = 0; Private->I2cNumber <= MAX_I2C_CONTROLLERS_NUM; (Private->I2cNumber)++) {
      Private->I2cConDevList = AllocateZeroPool (sizeof (I2C_CONTROLLER_DEVICE_LIST));
      Private->I2cConDevList->I2cControllerDeviceList[Private->I2cNumber].Count = 0;
  }
  Status = Enumerate (&(Private->Enumerate), NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);
  //
  // Test2: Don't exist any I2C device.
  //        if the I2cConDevList.Count < 2 means there is no any devices in this I2c Bus.
  //
  // Expected Result: EFI_NOT_FOUND
  //
  for (Private->I2cNumber = 0; Private->I2cNumber <= MAX_I2C_CONTROLLERS_NUM; (Private->I2cNumber)++) {
    for (I2cConDevNum = 0; I2cConDevNum < 2; I2cConDevNum++) {
      I2cDevice = AllocateZeroPool (sizeof (EFI_I2C_DEVICE));
      Private->I2cConDevList = AllocateZeroPool (sizeof (I2C_CONTROLLER_DEVICE_LIST));
      Private->I2cConDevList->I2cControllerDeviceList[Private->I2cNumber].Count = I2cConDevNum;
      Status = Enumerate (&(Private->Enumerate), (CONST EFI_I2C_DEVICE **) &I2cDevice);
      UT_ASSERT_EQUAL (Status, EFI_NOT_FOUND);
    }
  }
  //
  // Test3: *Device == NULL
  //
  // Expected Result: EFI_SUCCESS
  //
  for (Private->I2cNumber = 0; Private->I2cNumber <= MAX_I2C_CONTROLLERS_NUM; (Private->I2cNumber)++) {
    for (I2cConDevNum = 2; I2cConDevNum <= MAX_I2C_CONTROLLERS_NUM; I2cConDevNum++) {
      I2cDevice = NULL;
      Private->I2cConDevList = AllocateZeroPool (sizeof (I2C_CONTROLLER_DEVICE_LIST));
      for (Index = 0; Index <=I2cConDevNum; Index++) {
        CopyMem (&(Private->I2cConDevList->I2cControllerDeviceList[Private->I2cNumber].I2cDeviceList[I2cConDevNum]), I2cDeviceListI2c0, sizeof (EFI_I2C_DEVICE_PCD));
      }
      Private->I2cConDevList->I2cControllerDeviceList[Private->I2cNumber].Count = I2cConDevNum;
      Status = Enumerate (&(Private->Enumerate), (CONST EFI_I2C_DEVICE **) &I2cDevice);
      UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
    }
  }
  //
  // Test4: There has I2C devices on an I2C bus. It will request next device until there's no I2C device.
  //
  // Expected Result: EFI_NO_MAPPING
  //
  I2cDevice = AllocateZeroPool (sizeof (EFI_I2C_DEVICE));
  for (Private->I2cNumber = 0; Private->I2cNumber <= MAX_I2C_CONTROLLERS_NUM; (Private->I2cNumber)++) {
    for (I2cConDevNum = 0; I2cConDevNum <= MAX_I2C_CONTROLLERS_NUM; I2cConDevNum++) {
      Private->I2cConDevList = AllocateZeroPool (sizeof (I2C_CONTROLLER_DEVICE_LIST));
      for (Index = 0; Index <=I2cConDevNum; Index++) {
        CopyMem (&(Private->I2cConDevList->I2cControllerDeviceList[Private->I2cNumber].I2cDeviceList[Index]), I2cDeviceListI2c0, sizeof (EFI_I2C_DEVICE_PCD));
      }
    I2cDevice = (EFI_I2C_DEVICE *) &(Private->I2cConDevList->I2cControllerDeviceList[Private->I2cNumber].I2cDeviceList[0]);
    Private->I2cConDevList->I2cControllerDeviceList[Private->I2cNumber].Count = MAX_I2C_CONTROLLERS_NUM;
    do {
        Status = Enumerate (&(Private->Enumerate), (CONST EFI_I2C_DEVICE **) &I2cDevice);
        if ( Status == EFI_SUCCESS) {
          UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
        }
      } while (Status == EFI_SUCCESS);
      UT_ASSERT_EQUAL (Status, EFI_NO_MAPPING);
    }
  }
  //
  // Destroy Mock Service
  //
  FreePool (Private);
  FreePool (I2cDevice);

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
  return UNIT_TEST_PASSED;
}
