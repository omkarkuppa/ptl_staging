/**@file
  PreRequisite functions of the I2cPmData function.

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

#include <Uefi.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <PrivateMock.h>

/**
  Set up services for I2cPmData function.

  @retval EFI_SUCCESS  Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupI2cPmDataEnvironment (
  VOID
  );

/**
  Set up the environment and execute I2cPmData function.

  @param[in]  Context                                The pointer for the Unit Test context.

  @retval     UNIT_TEST_PASSED                       Set up success.
  @retval     UNIT_TEST_ERROR_PREREQUISITE_NOT_MET   Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreI2cPmData(
  IN UNIT_TEST_CONTEXT        Context
  );

/**
  Function used to update data in Power Meter ACPI table from I2c Power Meter structure.

  @param[in] POWER_METER_PKG     I2C Power Meter structure.
**/
EFI_STATUS
EFIAPI
I2cPmData (
  IN      POWER_METER_PKG             *PowerMtrData,
  IN      UINT8                       PowerMeterEnable
);