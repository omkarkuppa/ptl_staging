/** @file
  Unit test case of the UpdateMiscSmbiosCallback function.

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
  Unit Test case for UpdateMiscSmbiosCallback function.

  @param[in]  Context                       The pointer for the Unit Test context.

  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestUpdateMiscSmbiosCallback (
  IN UNIT_TEST_CONTEXT  Context
  );
/**
  Callback Function to handle all SMBIOS records.

  @param[in] Event     Event whose notification function is being invoked (Ready To Boot).
  @param[in] Context   Pointer to the notification functions context, which is implementation dependent.

**/
VOID
EFIAPI
UpdateMiscSmbiosCallback (
  IN EFI_EVENT          Event,
  IN VOID               *Context
  );
