/**@file

    TestCase of InstallPmaxTable function.

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

@par Specification
**/

#include <PiPei.h>
#include <Uefi.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include "PrivateMock/PrivateMock.h"
#include <PrivateMock.h>

/**

  Unit Test case for InstallPmaxTable function.
  @param[in]  Context                       The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.

**/
UNIT_TEST_STATUS
EFIAPI
TestCaseInstallPmaxTable (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  PMAX table installation driver.

  @param[in] ImageHandle     Handle for this drivers loaded image protocol.
  @param[in] SystemTable     EFI system table.

  @retval EFI_SUCCESS        The driver installed without error.
  @retval Other              The driver encountered an error and could not complete installation of
                             the ACPI tables.

**/
EFI_STATUS
EFIAPI
InstallPmaxTable (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
);
