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

#ifndef _PRE_PATCH_RTD3_BUFFER_H_
#define _PRE_PATCH_RTD3_BUFFER_H_

#include <PrivateMock.h>

/**
  Fetch for I2c Rtd3 Buffer and patch the Board Data accordingly.

  @param[in] I2cDevTable              I2c Device Table
  @param[in] I2cControllerInfo        I2c Controller Board Data Structure
  @param[in] Count                    Maximum Pch I2C ports.
**/
VOID
PatchRtd3Buffer (
  EFI_ACPI_DESCRIPTION_HEADER  *I2cDevTable,
  I2C_ACPI_DEV                 *I2cControllerInfo,
  UINT8                        Count
  );

/**
  Set up services for PatchRtd3Buffer function.

  @retval EFI_SUCCESS  Set up success.

**/
EFI_STATUS
EFIAPI
SetupPatchRtd3BufferEnvironment (
  VOID
  );

/**
  Set up the environment and execute PatchRtd3Buffer function.

  @param[in] Context                            The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Fail to set up environment.

**/
UNIT_TEST_STATUS
EFIAPI
PrePatchRtd3Buffer (
  IN UNIT_TEST_CONTEXT  Context
  );

#endif  // _PRE_PATCH_RTD3_BUFFER_H_
