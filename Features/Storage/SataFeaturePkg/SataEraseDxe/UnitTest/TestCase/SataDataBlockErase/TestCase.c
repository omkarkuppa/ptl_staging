/**@file
    Unit test cases of SataDataBlockErase.

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

#include "SataDataBlockEraseMock.h"

/**
  Unit test for SataDataBlockErase.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestDataBlockErase_S (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                  Status;
  EFI_ATA_PASS_THRU_PROTOCOL  *AtaDevice;
  SATA_ERASE_BLOCK_PRIVATE    *Private;

  Private   = AllocateZeroPool (sizeof (SATA_ERASE_BLOCK_PRIVATE));
  AtaDevice = AllocateZeroPool (sizeof (EFI_ATA_PASS_THRU_PROTOCOL));
  AtaDevice->Mode = AllocateZeroPool (sizeof (EFI_ATA_PASS_THRU_MODE));
  AtaDevice->PassThru = MockAtaPassThru_PassThru;
  Private->Port = 1;
  Private->PortMultiplier = 0xffff;
  Private->Passthru = AtaDevice;

  Status = SataDataBlockErase (Private, 1, 0, NULL, 0);
  FreePool (AtaDevice->Mode);
  FreePool (AtaDevice);
  FreePool(Private);

  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  return UNIT_TEST_PASSED;
}

/**
  Unit test for SataDataBlockErase.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestDataBlockErase_F (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                  Status;
  EFI_ATA_PASS_THRU_PROTOCOL  *AtaDevice;
  SATA_ERASE_BLOCK_PRIVATE    *Private;

  Private   = AllocateZeroPool (sizeof (SATA_ERASE_BLOCK_PRIVATE));
  AtaDevice = AllocateZeroPool (sizeof (EFI_ATA_PASS_THRU_PROTOCOL));
  AtaDevice->Mode = AllocateZeroPool (sizeof (EFI_ATA_PASS_THRU_MODE));
  AtaDevice->PassThru = MockAtaPassThru_PassThru;
  Private->Port = 1;
  Private->PortMultiplier = 0xffff;
  Private->Passthru = AtaDevice;

  Status = SataDataBlockErase (Private, 1, 0, NULL, 0);
  FreePool (AtaDevice->Mode);
  FreePool (AtaDevice);
  FreePool(Private);

  UT_ASSERT_NOT_EQUAL (Status, EFI_SUCCESS);
  return UNIT_TEST_PASSED;
}
