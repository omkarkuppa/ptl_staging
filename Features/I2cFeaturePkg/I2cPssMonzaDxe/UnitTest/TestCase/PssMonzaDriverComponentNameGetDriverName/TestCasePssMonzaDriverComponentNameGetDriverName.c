/**@file
    Unit test case of the I2c Pss Monza Dxe driver.

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "TestCasePssMonzaDriverComponentNameGetDriverName.h"
#include <Protocol/DriverBinding.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/ComponentName2.h>
#include <Protocol/ComponentName.h>
#include <Library/DebugLib.h>

extern EFI_COMPONENT_NAME2_PROTOCOL gPssMonzaDriverComponentName2;
extern EFI_COMPONENT_NAME_PROTOCOL  gPssMonzaDriverComponentName;
/**
  Retrieves a Unicode string that is the user-readable name of the EFI Driver.

  @param[in]  This       A pointer to the EFI_COMPONENT_NAME_PROTOCOL instance.
  @param[in]  Language   A pointer to a three-character ISO 639-2 language identifier.
                         This is the language of the driver name that that the caller
                         is requesting, and it must match one of the languages specified
                         in SupportedLanguages.  The number of languages supported by a
                         driver is up to the driver writer.
  @param[out] DriverName A pointer to the Unicode string to return.  This Unicode string
                         is the name of the driver specified by This in the language
                         specified by Language.

  @retval EFI_SUCCESS           The Unicode string for the Driver specified by This
                                and the language specified by Language was returned
                                in DriverName.
  @retval EFI_INVALID_PARAMETER Language is NULL.
  @retval EFI_INVALID_PARAMETER DriverName is NULL.
  @retval EFI_UNSUPPORTED       The driver specified by This does not support the
                                language specified by Language.

**/
extern
EFI_STATUS
EFIAPI
PssMonzaDriverComponentNameGetDriverName (
  IN  EFI_COMPONENT_NAME_PROTOCOL  *This,
  IN  CHAR8                        *Language,
  OUT CHAR16                       **DriverName
  );

/**
  Mock PssMonzaDriverComponentNameGetDriverName

  @param[in]  Context                   [Optional] An optional parameter that enables:
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
UnitTestPssMonzaDriverComponentNameGetDriverName (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                     Status;
  CHAR16                         *DriverName;
  CHAR16                         UnicodeCheck[] = L"I2C Pss Monza driver";

  DriverName = NULL;
  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  //
  // Test1 :Mock LookupUnicodeString2 to get Driver name. Check DriverName whether "I2C Pss Monza driver" or not.
  // Expected Result: EFI_SUCCESS.
  //

  // Mock LookupUnicodeString2 to get Driver name.
  will_return (LookupUnicodeString2, EFI_SUCCESS);

  Status = PssMonzaDriverComponentNameGetDriverName (&gPssMonzaDriverComponentName, NULL, &DriverName);
  // Check DriverName whether "I2C Pss Monza driver" or not.
  UT_ASSERT_EQUAL (StrCmp (DriverName,UnicodeCheck), 0);
  // Check Status is EFI_SUCCESS.
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));
  return UNIT_TEST_PASSED;
}