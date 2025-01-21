/**@file
  Unit test case of the I2C Device Common Driver

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

#include "PreInstallI2cTable.h"

#define PRE_FUNCTION_NAME     "PreInstallI2cTable"
#define PRE_FUNCTION_VERSION  "0.1"

/**
  Set up services for InstallI2cTable function.

  @retval EFI_SUCCESS  Set up success.

**/
EFI_STATUS
EFIAPI
SetupInstallI2cTableEnvironment (
  VOID
  )
{
  UINTN  SsdtTargetBufferSize;

  mAcpiTable = AllocateZeroPool (sizeof (EFI_ACPI_TABLE_PROTOCOL));
  UT_ASSERT_NOT_NULL (mAcpiTable);

  //
  // Mock mAcpiTable->InstallAcpiTable
  //
  mAcpiTable->InstallAcpiTable = MockInstallAcpiTable;

  mI2CAcpiTable = AllocateZeroPool (sizeof (EFI_ACPI_DESCRIPTION_HEADER));
  UT_ASSERT_NOT_NULL (mI2CAcpiTable);

  will_return (MockInstallAcpiTable, &mI2CAcpiTable);
  will_return (MockInstallAcpiTable, &SsdtTargetBufferSize);
  will_return (MockInstallAcpiTable, EFI_SUCCESS);

  return EFI_SUCCESS;
}

/**
  Set up the environment and execute InstallI2cTable function.

  @param[in] Context                            The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Fail to set up environment.

**/
UNIT_TEST_STATUS
EFIAPI
PreInstallI2cTable (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "[PreRequisite] %a v%a\n", PRE_FUNCTION_NAME, PRE_FUNCTION_VERSION));

  Status = SetupInstallI2cTableEnvironment ();
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //
  // Execute test function with above mock environment
  //
  InstallI2cTable (NULL, NULL);

  return UNIT_TEST_PASSED;
}

