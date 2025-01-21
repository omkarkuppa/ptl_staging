/**@file
  Unit test case of the Firmware Volume Copy Lib header

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.
**/

#ifndef TEST_CASE_FIRMWARE_VOLUME_COPY_LIB_H
#define TEST_CASE_FIRMWARE_VOLUME_COPY_LIB_H

#include <Uefi.h>
#include <Library/UnitTestLib.h>

/**
  Unit test for FirmwareVolume Copy Library.
  @param[in]  Context    [Optional] An optional parameter that enables:
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
UnitTestFirmwareVolumeCopyWithoutSpiDma (
  IN UNIT_TEST_CONTEXT  Context
  );

UNIT_TEST_STATUS
GetFvPaths(
  VOID
  );

UNIT_TEST_STATUS
EFIAPI
UnitTestFVCopyCheck(
  char *FvPath
  );

/**
  Unit test for FirmwareVolumeCopy with SPI DMA Enabled.
  @param[in]  Context    [Optional] An optional parameter that enables:
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
UnitTestFirmwareVolumeCopyWithSpiDmaEnabled (
  IN UNIT_TEST_CONTEXT  Context
  );

UNIT_TEST_STATUS
EFIAPI
UnitTestFreeSpaceSkipEntryPointFvShadowPpiandSpiDmaPpiFound (
  IN UNIT_TEST_CONTEXT  Context
  );

UNIT_TEST_STATUS
EFIAPI
UnitTestFreeSpaceSkipEntryPointFvShadowPpiNotFoundandSpiDmaPpiFound (
  IN UNIT_TEST_CONTEXT  Context
  );

#endif // TEST_CASE_FIRMWARE_VOLUME_COPY_LIB_H
