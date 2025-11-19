/**@file
    Unit test cases of SetAtaPassword.

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

#include "SetAtaPasswordMock.h"

/**
  Unit test for SetAtaPassword.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestSetAtaPassword (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                  Status;
  EFI_ATA_PASS_THRU_PROTOCOL  *AtaDevice;
  CHAR8                       DefaultPassStr[ATA_PASSWORD_MAX_LENGTH] = "PLATFORMERASE";
  UINT16                      ResetPassword[16] = {0};
  ATA_IDENTIFY_DATA           *IdentifyData;

  //
  // Mock EFI_ATA_PASS_THRU_PROTOCOL
  //
  AtaDevice = AllocateZeroPool (sizeof (EFI_ATA_PASS_THRU_PROTOCOL));
  AtaDevice->Mode = AllocateZeroPool (sizeof (EFI_ATA_PASS_THRU_MODE));
  AtaDevice->PassThru = MockAtaPassThru_PassThru;
  SetMem (gMasterPassword, 32, 0);

  //
  // Security not supported
  //
  Status = SetAtaPassword (
             AtaDevice,
             1,
             0xffff,
             ATA_SECURITY_ERASE_IDENTIFY_MASTER_PASSWORD,
             ATA_SECURITY_ERASE_LEVEL_MAXIMUM,
             0,
             DefaultPassStr
             );
  UT_ASSERT_EQUAL (Status, EFI_DEVICE_ERROR);
  UT_ASSERT_MEM_EQUAL(gMasterPassword, ResetPassword, AsciiStrLen((VOID*) ResetPassword));

  //
  // Mock Idenfity device data
  // refer to ATA spec, Security supported
  //
  IdentifyData = IdentifyDataConstructMock ();
  IdentifyData->security_status = BIT0;
  Status = SetAtaPassword (
             AtaDevice,
             1,
             0xffff,
             ATA_SECURITY_ERASE_IDENTIFY_MASTER_PASSWORD,
             ATA_SECURITY_ERASE_LEVEL_MAXIMUM,
             0,
             DefaultPassStr
             );
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_MEM_EQUAL(gMasterPassword, DefaultPassStr, AsciiStrLen(DefaultPassStr));
  IdendifyDataDestructorMock ();

  if (AtaDevice != NULL) {
    if (AtaDevice->Mode != NULL) {
      FreePool (AtaDevice->Mode);
    }
    FreePool (AtaDevice);
  }
  return UNIT_TEST_PASSED;
}
