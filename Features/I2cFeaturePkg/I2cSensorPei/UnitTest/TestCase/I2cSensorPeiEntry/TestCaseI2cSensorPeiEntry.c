/**@file
    Unit test case of the I2c Sensor PEIM.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#include "TestCaseI2cSensorPeiEntry.h"
#include <I2cSensorPei.h>
#include <Library/UefiBootServicesTableLib.h>
#include <BootServicesMock.h>
#include <PrivateMock.h>

UNIT_TEST_STATUS
EFIAPI
UnitTestI2cSensorPeiEntry (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                      Status;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  // Mock function for PeiServicesInstallPpi
  expect_any (PeiServicesInstallPpi, PpiList);
  will_return (PeiServicesInstallPpi, EFI_SUCCESS);

  // Check Status is EFI_SUCCESS.
  Status = I2cSensorPeiEntry (NULL, NULL);
  UT_ASSERT_NOT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));

  return UNIT_TEST_PASSED;
}