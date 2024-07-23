/** @file
  Unit test case of the SmbiosMiscEntryPoint function.

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
#include <Uefi.h>
#include <Library/UnitTestLib.h>

/**
  Unit Test case for SmbiosMiscEntryPoint function.

  @param[in]  Context                       The pointer for the Unit Test context.

  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestSmbiosMiscEntryPoint (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Standard EFI driver point.  This driver parses the mSmbiosMiscDataTable
  structure and reports any generated data using SMBIOS protocol.

  @param[in] ImageHandle     Handle for the image of this driver
  @param[in] SystemTable     Pointer to the EFI System Table

  @retval EFI_SUCCESS        The data was successfully stored.

**/
EFI_STATUS
EFIAPI
SmbiosMiscEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  );
