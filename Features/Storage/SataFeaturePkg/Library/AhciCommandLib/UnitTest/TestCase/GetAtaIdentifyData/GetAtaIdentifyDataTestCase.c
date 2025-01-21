/**@file
    Unit test cases of GetAtaIdentifyData.

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

#include "GetAtaIdentifyDataMock.h"

/**
  Unit test for GetAtaIdentifyData.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
GetAtaIdentifyDataUnitTest (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                  Status;
  EFI_ATA_PASS_THRU_PROTOCOL  *AtaDevice;
  ATA_IDENTIFY_DATA           ControllerData;
  ATA_IDENTIFY_DATA           *IdentifyData;

  //
  // Mock EFI_ATA_PASS_THRU_PROTOCOL
  //
  AtaDevice = AllocateZeroPool (sizeof (EFI_ATA_PASS_THRU_PROTOCOL));
  AtaDevice->Mode = AllocateZeroPool (sizeof (EFI_ATA_PASS_THRU_MODE));
  AtaDevice->PassThru = MockAtaPassThru_PassThru;

  Status = GetAtaIdentifyData (AtaDevice, 1, 0xffff, &ControllerData);
  UT_ASSERT_EQUAL (Status, EFI_DEVICE_ERROR);

  //
  // Mock Idenfity device data
  // refer to ATA spec, Security supported & enabled
  //
  IdentifyData = IdentifyDataConstructMock ();
  IdentifyData->security_status = BIT0 | BIT1;
  Status = GetAtaIdentifyData (AtaDevice, 1, 0xffff, &ControllerData);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_EQUAL (ControllerData.security_status, BIT0 | BIT1);
  IdendifyDataDestructorMock ();

  if (AtaDevice != NULL) {
    if (AtaDevice->Mode != NULL) {
      FreePool (AtaDevice->Mode);
    }
    FreePool (AtaDevice);
  }
  return UNIT_TEST_PASSED;
}

