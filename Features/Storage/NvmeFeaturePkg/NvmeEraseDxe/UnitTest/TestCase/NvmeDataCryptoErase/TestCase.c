/**@file
    Unit test cases of NvmeDataCryptoErase.

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

#include "NvmeDataCryptoEraseMock.h"

/**
  Unit test for NvmeDataCryptoErase.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestDataCryptoErase_S (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                          Status;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *NvmeDevice;
  NVME_ERASE_BLOCK_PRIVATE            *Private;

  Private    = AllocateZeroPool (sizeof (NVME_ERASE_BLOCK_PRIVATE));
  NvmeDevice = AllocateZeroPool (sizeof (EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL));
  NvmeDevice->PassThru = MockNvmePassThruProtocol_PassThru;
  Private->Passthru = NvmeDevice;
  Private->NameSpaceId = 1;

  Status = NvmeDataCryptoErase (Private, 1, 0, NULL, 0);
  FreePool (NvmeDevice);
  FreePool (Private);

  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  return UNIT_TEST_PASSED;
}

/**
  Unit test for NvmeDataCryptoErase.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestDataCryptoErase_F (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                          Status;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *NvmeDevice;
  NVME_ERASE_BLOCK_PRIVATE            *Private;

  Private    = AllocateZeroPool (sizeof (NVME_ERASE_BLOCK_PRIVATE));
  NvmeDevice = AllocateZeroPool (sizeof (EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL));
  NvmeDevice->PassThru = MockNvmePassThruProtocol_PassThru;
  Private->Passthru = NvmeDevice;
  Private->NameSpaceId = 1;

  Status = NvmeDataCryptoErase (Private, 1, 0, NULL, 0);
  FreePool (NvmeDevice);
  FreePool (Private);

  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);
  return UNIT_TEST_PASSED;
}
