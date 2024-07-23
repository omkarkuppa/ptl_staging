/**@file
    Unit test cases of GetSataEraseInfo.

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

#include "GetSataEraseResultMock.h"

/**
  Unit test for GetSataEraseInfo.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestGetSataEraseInfo (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                  Status;
  SATA_ERASE_INFO             *DevInfo;
  SATA_ERASE_BLOCK_PRIVATE    *Private;
  EFI_ATA_PASS_THRU_PROTOCOL  *AtaDevice;

  Private = AllocateZeroPool (sizeof (SATA_ERASE_BLOCK_PRIVATE));
  Private->Info = AllocateZeroPool (sizeof (SATA_ERASE_INFO));
  Private->Signature = SATA_ERASE_BLOCK_SIGNATURE;

  //
  // Mock PassThru
  //
  AtaDevice = AllocateZeroPool (sizeof (EFI_ATA_PASS_THRU_PROTOCOL));
  AtaDevice->PassThru = MockAtaPassThru_PassThru;
  Private->Port = 1;
  Private->PortMultiplier = 0xffff;
  Private->Passthru = AtaDevice;

  //
  // Mock device information
  //
  CopyMem (Private->Info->SerialNumber, "BTLT706501T6120GGN", SATA_SN_MAX_LEN);
  CopyMem (Private->Info->ModelName, "INTELSSDSC2KW120H6", SATA_MN_MAX_LEN);

  Status = GetSataEraseInfo (&Private->EraseBlock, &DevInfo);

  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_MEM_EQUAL (DevInfo->SerialNumber, "BTLT706501T6120GGN", SATA_SN_MAX_LEN);
  UT_ASSERT_MEM_EQUAL (DevInfo->ModelName, "INTELSSDSC2KW120H6", SATA_MN_MAX_LEN);

  FreePool(AtaDevice);
  FreePool(Private->Info);
  FreePool(Private);
  return UNIT_TEST_PASSED;
}
