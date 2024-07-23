/**@file
  Unit test case of the I2c Touch Device Acpi Driver

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

@par Specification Reference:
**/

#include "PreI2CBoardData.h"

#define PRE_FUNCTION_NAME     "PreI2CBoardData"
#define PRE_FUNCTION_VERSION  "0.1"

/**
  Set up services for I2CBoardData function.

  @retval EFI_SUCCESS  Set up success.

**/
EFI_STATUS
EFIAPI
SetupI2CBoardDataEnvironment (
  VOID
  )
{
  TABLE_POINTER_ADDR_PROTOCOL  *TableProtocol;
  EFI_ACPI_DESCRIPTION_HEADER  *I2cDevTable;

  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  UT_ASSERT_NOT_NULL (gBS);

  //
  // Mock boot service LocateProtocol
  //
  gBS->LocateProtocol = MockBootServiceLocateProtocol;

  TableProtocol = AllocateZeroPool (sizeof (TABLE_POINTER_ADDR_PROTOCOL));
  UT_ASSERT_NOT_NULL (TableProtocol);

  expect_value (MockBootServiceLocateProtocol, Protocol, &gI2cDeviceProtocolGuid);
  will_return (MockBootServiceLocateProtocol, TableProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);

  //
  // Mock PcdSet64S
  //
  expect_value (LibPcdSet64S, TokenNumber, _PCD_TOKEN_PcdI2cDevTableName);
  will_return (LibPcdSet64S, EFI_SUCCESS);

  //
  // Mock TableProtocol->GetDevTablePointer
  //
  TableProtocol->GetDevTablePointer = MockGetDevTablePointer;

  I2cDevTable = AllocateZeroPool (sizeof (EFI_ACPI_DESCRIPTION_HEADER));
  UT_ASSERT_NOT_NULL (I2cDevTable);

  will_return (MockGetDevTablePointer, I2cDevTable);

  return EFI_SUCCESS;
}

/**
  Set up the environment and execute I2CBoardData function.

  @param[in] Context                            The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Fail to set up environment.

**/
UNIT_TEST_STATUS
EFIAPI
PreI2CBoardData (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS    Status;
  UINT8         Count;
  I2C_ACPI_DEV  *I2cControllerInfo;
  UINT8         Rtd3Support;

  DEBUG ((DEBUG_INFO, "[PreRequisite] %a v%a\n", PRE_FUNCTION_NAME, PRE_FUNCTION_VERSION));

  Status = SetupI2CBoardDataEnvironment ();
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //
  // Initialize
  //
  Count = 1;
  Rtd3Support = 1;

  I2cControllerInfo = AllocateZeroPool (sizeof (I2C_ACPI_DEV));
  UT_ASSERT_NOT_NULL (I2cControllerInfo);

  //
  // Execute test function with above mock environment
  //
  I2CBoardData (Count, I2cControllerInfo, Rtd3Support);

  return UNIT_TEST_PASSED;
}

