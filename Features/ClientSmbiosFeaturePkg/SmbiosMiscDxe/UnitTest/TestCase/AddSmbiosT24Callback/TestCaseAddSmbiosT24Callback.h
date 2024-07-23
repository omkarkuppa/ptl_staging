/** @file
  Unit test case of the AddSmbiosT24Callback function.

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
  Check the result of test case with AddSmbiosT24Callback.

  @param[in]  Context                          The pointer for the Unit Test context.

  @retval     UNIT_TEST_PASSED                 The result is correct.
  @retval     UNIT_TEST_ERROR_TEST_FAILED      The result is wrong.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAddSmbiosT24Callback (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Install a SMBIOS type 24 record - Hardware Security.
  The SMBIOS record is constructed with the 'Power-on Password Status value' and 'Front Panel Reset
  value' set to 'Not Implemented.  The status of the 'Keyboard Password Status value' and 'Administrator
  Password Status value' set according to their variable settings.

  @param[in] Event    Event whose notification function is being invoked (Ready To Boot).
  @param[in] Context  Pointer to the notification functions context, which is implementation dependent.

**/
VOID
EFIAPI
AddSmbiosT24Callback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  );
