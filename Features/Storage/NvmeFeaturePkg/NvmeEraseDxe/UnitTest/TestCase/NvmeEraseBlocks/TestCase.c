/**@file
    Unit test cases of NvmeEraseBlocks.

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

#include "NvmeEraseBlocksMock.h"
#include "TestCase/NvmeDataBlockErase/NvmeDataBlockEraseMock.h"
#include "TestCase/NvmeDataCryptoErase/NvmeDataCryptoEraseMock.h"

/**
  Unit test for NvmeEraseBlocks.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestEraseBlocks_1 (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                  Status;
  NVME_ERASE_BLOCK_PRIVATE    *Private;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL *NvmeDevice;

  Private = AllocateZeroPool (sizeof (NVME_ERASE_BLOCK_PRIVATE));
  Private->Signature = NVME_ERASE_BLOCK_SIGNATURE;
  Private->Info = AllocateZeroPool (sizeof (NVME_ERASE_INFO));

  //
  // Mock PassThru
  //
  NvmeDevice = AllocateZeroPool (sizeof (EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL));
  NvmeDevice->PassThru = MockNvmePassThruProtocol_PassThru;
  Private->Passthru = NvmeDevice;

  //
  // Mock Idenfity device data
  //
  PreDataBlockErase_S (Context);

  Status = NvmeEraseBlocks (&Private->EraseBlock, 1, 0, NULL, 0, NvmeBlockErase, NULL);

  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_EQUAL (Private->Info->Method, NvmeBlockErase);
  UT_ASSERT_EQUAL (Private->Info->Result, EFI_SUCCESS);

  FreePool(NvmeDevice);
  FreePool(Private->Info);
  FreePool(Private);
  CleanUpDataBlockErase (Context);
  return UNIT_TEST_PASSED;
}

/**
  Unit test for NvmeEraseBlocks.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestEraseBlocks_2 (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                  Status;
  NVME_ERASE_BLOCK_PRIVATE    *Private;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL *NvmeDevice;

  Private = AllocateZeroPool (sizeof (NVME_ERASE_BLOCK_PRIVATE));
  Private->Signature = NVME_ERASE_BLOCK_SIGNATURE;
  Private->Info = AllocateZeroPool (sizeof (NVME_ERASE_INFO));

  //
  // Mock PassThru
  //
  NvmeDevice = AllocateZeroPool (sizeof (EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL));
  NvmeDevice->PassThru = MockNvmePassThruProtocol_PassThru;
  Private->Passthru = NvmeDevice;

  //
  // Mock Idenfity device data
  //
  PreDataCryptoErase_S (Context);

  Status = NvmeEraseBlocks (&Private->EraseBlock, 1, 0, NULL, 0, NvmeCryptoErase, NULL);

  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_EQUAL (Private->Info->Method, NvmeCryptoErase);
  UT_ASSERT_EQUAL (Private->Info->Result, EFI_SUCCESS);

  FreePool(NvmeDevice);
  FreePool(Private->Info);
  FreePool(Private);
  CleanUpDataCryptoErase (Context);
  return UNIT_TEST_PASSED;
}

/**
  Unit test for NvmeEraseBlocks.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestEraseBlocks_3 (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                  Status;
  NVME_ERASE_BLOCK_PRIVATE    *Private;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL *NvmeDevice;

  Private = AllocateZeroPool (sizeof (NVME_ERASE_BLOCK_PRIVATE));
  Private->Signature = NVME_ERASE_BLOCK_SIGNATURE;
  Private->Info = AllocateZeroPool (sizeof (NVME_ERASE_INFO));

  //
  // Mock PassThru
  //
  NvmeDevice = AllocateZeroPool (sizeof (EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL));
  NvmeDevice->PassThru = MockNvmePassThruProtocol_PassThru;
  Private->Passthru = NvmeDevice;

  //
  // Mock Idenfity device data
  //
  PreDataBlockErase_S (Context);

  Status = NvmeEraseBlocks (&Private->EraseBlock, 1, 0, NULL, 0, NvmeOverwrite, NULL);

  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);
  UT_ASSERT_EQUAL (Private->Info->Method, NvmeOverwrite);
  UT_ASSERT_EQUAL (Private->Info->Result, EFI_UNSUPPORTED);

  FreePool(NvmeDevice);
  FreePool(Private->Info);
  FreePool(Private);
  CleanUpDataBlockErase (Context);
  return UNIT_TEST_PASSED;
}
