/** @file
  Unit test case of the UpdateMiscSmbios function.

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
#include <SmbiosPlatformInfoTable.h>

/**
  Unit Test case for UpdateMiscSmbios function.

  @param[in]  Context                       The pointer for the Unit Test context.

  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestUpdateMiscSmbios (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Update Misc SMBIOS records.

  @param[in]  SmbiosPlatformInfo              Pointer to SmbiosPlatformInfo table.
  @param[in]  SmbiosPlatformInfoTableEntries  Size of SmbiosPlatformInfo table.

  @retval     EFI_SUCCESS               All parameters were valid.
  @retval     EFI_NOT_FOUND             Specified Protocal can not be found.
  @retval     EFI_UNSUPPORTED           Unexpected RecordType value.
  @retval     EFI_INVALID_PARAMETER     Invalid parameter was found.

**/
EFI_STATUS
UpdateMiscSmbios (
  SMBIOS_PLATFORM_INFO  *SmbiosPlatformInfo,
  UINTN                 SmbiosPlatformInfoTableEntries
  );
