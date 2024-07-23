/**@file
    Unit test cases of AtaSanitizeCommand.

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

#include "AtaSanitizeCommandMock.h"

/**
  Unit test for AtaSanitizeCommand.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestAtaSanitizeCommand (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                  Status;
  EFI_ATA_PASS_THRU_PROTOCOL  *AtaDevice;
  ATA_IDENTIFY_DATA           *IdentifyData;

  //
  // Mock EFI_ATA_PASS_THRU_PROTOCOL
  //
  AtaDevice = AllocateZeroPool (sizeof (EFI_ATA_PASS_THRU_PROTOCOL));
  AtaDevice->Mode = AllocateZeroPool (sizeof (EFI_ATA_PASS_THRU_MODE));
  AtaDevice->PassThru = MockAtaPassThru_PassThru;
  IdentifyData = IdentifyDataConstructMock ();

  //
  // ATA_SANITIZE_FEATURE_BLOCKERASE, Sanitize not supported
  //
  Status = AtaSanitizeCommand (AtaDevice, 1, 0xffff, 0x12, 0x426B4572); // ATA_SANITIZE_LBA_BLOCKERASE
  UT_ASSERT_EQUAL (Status, EFI_DEVICE_ERROR);

  //
  // Mock Idenfity device data. Refer to ATA spec, Sanitize supported
  //
  IdentifyData->multi_sector_setting |= BIT12;
  //
  // ATA_SANITIZE_FEATURE_BLOCKERASE
  //
  Status = AtaSanitizeCommand (AtaDevice, 1, 0xffff, 0x12, 0x426B4572); // ATA_SANITIZE_LBA_BLOCKERASE
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  IdendifyDataDestructorMock ();
  if (AtaDevice != NULL) {
    if (AtaDevice->Mode != NULL) {
      FreePool (AtaDevice->Mode);
    }
    FreePool (AtaDevice);
  }
  return UNIT_TEST_PASSED;
}
