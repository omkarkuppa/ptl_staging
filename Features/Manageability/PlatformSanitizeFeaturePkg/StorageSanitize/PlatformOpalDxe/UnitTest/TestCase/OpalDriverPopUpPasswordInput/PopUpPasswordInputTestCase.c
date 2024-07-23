/**@file
    Unit test cases of OpalDriverPopUpPasswordInput.

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

#include "PopUpPasswordInputMock.h"

/**
  Unit test for OpalDriverPopUpPasswordInput.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
PopUpPasswordInputUnitTest (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  BOOLEAN       PressEsc;
  CHAR8         *Password;
  UINTN         Index;
  EFI_INPUT_KEY InputEsc = {SCAN_ESC, CHAR_NULL};
  EFI_INPUT_KEY InputRET = {SCAN_NULL, CHAR_CARRIAGE_RETURN};
  EFI_INPUT_KEY InputKey[OPAL_MAX_PASSWORD_SIZE] = {
    {SCAN_NULL, '1'}, {SCAN_NULL, '2'}, {SCAN_NULL, '3'}, {SCAN_NULL, '4'}, {SCAN_NULL, '5'}, {SCAN_NULL, '6'}, {SCAN_NULL, '7'}, {SCAN_NULL, '8'},
    {SCAN_NULL, 'a'}, {SCAN_NULL, 'b'}, {SCAN_NULL, 'c'}, {SCAN_NULL, 's'}, {SCAN_NULL, 'd'}, {SCAN_NULL, 'f'}, {SCAN_NULL, 'g'}, {SCAN_NULL, 'h'},
    {SCAN_NULL, '9'}, {SCAN_NULL, 'q'}, {SCAN_NULL, 'w'}, {SCAN_NULL, 'e'}, {SCAN_NULL, 'r'}, {SCAN_NULL, '0'}, {SCAN_NULL, 't'}, {SCAN_NULL, 'y'},
    {SCAN_NULL, 'A'}, {SCAN_NULL, 'B'}, {SCAN_NULL, 'C'}, {SCAN_NULL, 'S'}, {SCAN_NULL, 'D'}, {SCAN_NULL, 'F'}, {SCAN_NULL, 'G'}, {SCAN_NULL, 'H'},
  };

  for (Index = 0; Index < 10; Index++) {
    will_return (CreatePopUp, &InputKey[Index]);
  }
  will_return (CreatePopUp, &InputRET);
  Password = OpalDriverPopUpPasswordInput (L"test password", &PressEsc);
  UT_ASSERT_EQUAL (PressEsc, FALSE);
  UT_ASSERT_MEM_EQUAL (Password, "12345678ab", 10);

  for (Index = 0; Index < 10; Index++) {
    will_return (CreatePopUp, &InputKey[Index]);
  }
  will_return (CreatePopUp, &InputEsc);
  Password = OpalDriverPopUpPasswordInput (L"test password", &PressEsc);
  UT_ASSERT_EQUAL (PressEsc, TRUE);
  UT_ASSERT_EQUAL (Password, NULL);

  for (Index = 0; Index < OPAL_MAX_PASSWORD_SIZE; Index++) {
    will_return (CreatePopUp, &InputKey[Index]);
  }
  Password = OpalDriverPopUpPasswordInput (L"test password", &PressEsc);
  UT_ASSERT_EQUAL (PressEsc, FALSE);
  UT_ASSERT_MEM_EQUAL (Password, "12345678abcsdfgh9qwer0tyABCSDFGH", OPAL_MAX_PASSWORD_SIZE);

  return UNIT_TEST_PASSED;
}
