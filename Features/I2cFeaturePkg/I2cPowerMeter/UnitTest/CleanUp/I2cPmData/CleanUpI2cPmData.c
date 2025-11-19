/**@file
  Clean-up functions of the I2cPmData function.

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

#include "CleanUpI2cPmData.h"
#include <Library/MemoryAllocationLib.h>

extern  EFI_BOOT_SERVICES *gBS;
extern  EFI_FIRMWARE_VOLUME2_PROTOCOL* gFwVol;
extern  EFI_ACPI_DESCRIPTION_HEADER *gVerifyTable;
extern  EFI_ACPI_TABLE_PROTOCOL     *gAcpiTableProtocol;
extern  POWER_METER_PKG                *PowerMtrData;

/**
  Clean up all variables used in testing I2cPmData function.

  @retval EFI_SUCCESS  Clean up success.
**/
EFI_STATUS
EFIAPI
CleanI2cPmDataEnvironment (
  VOID
  )
{
  FreePool (gBS);
  FreePool (gFwVol);
  FreePool (gVerifyTable);
  FreePool (gAcpiTableProtocol);
  FreePool (PowerMtrData);

  return EFI_SUCCESS;
}

/**
  Clean up the environment for next test case.

  @param[in]  Context                          The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED                 Clean up success.
  @retval     UNIT_TEST_ERROR_CLEANUP_FAILED   Fail to clean up.
**/
UNIT_TEST_STATUS
EFIAPI
CleanUpI2cPmData (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS   Status;

  Status = CleanI2cPmDataEnvironment ();
  UT_ASSERT_NOT_EFI_ERROR (Status);

  return UNIT_TEST_PASSED;
}

