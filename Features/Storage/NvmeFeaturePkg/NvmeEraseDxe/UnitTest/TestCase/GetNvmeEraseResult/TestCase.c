/**@file
    Unit test cases of GetNvmeEraseInfo.

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

#include "GetNvmeEraseResultMock.h"

/**
  Unit test for GetNvmeEraseInfo.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestGetNvmeEraseInfo (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                  Status;
  NVME_ERASE_INFO             *DevInfo;
  NVME_ERASE_BLOCK_PRIVATE    *Private;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL *NvmeDevice;

  Private = AllocateZeroPool (sizeof (NVME_ERASE_BLOCK_PRIVATE));
  Private->Info = AllocateZeroPool (sizeof (NVME_ERASE_INFO));
  Private->Signature = NVME_ERASE_BLOCK_SIGNATURE;

  //
  // Mock PassThru
  //
  NvmeDevice = AllocateZeroPool (sizeof (EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL));
  NvmeDevice->PassThru = MockNvmePassThruProtocol_PassThru;
  Private->Passthru = NvmeDevice;

  //
  // Mock device information
  //
  CopyMem (Private->Info->SerialNumber, "22115G800412", NVME_SN_MAX_LEN);
  CopyMem (Private->Info->ModelName, "WDC PC SN810 SDCPNRY-512G", NVME_MN_MAX_LEN);

  Status = GetNvmeEraseInfo (&Private->EraseBlock, &DevInfo);

  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_MEM_EQUAL (DevInfo->SerialNumber, "22115G800412", NVME_SN_MAX_LEN);
  UT_ASSERT_MEM_EQUAL (DevInfo->ModelName, "WDC PC SN810 SDCPNRY-512G", NVME_MN_MAX_LEN);

  FreePool(NvmeDevice);
  FreePool(Private->Info);
  FreePool(Private);
  return UNIT_TEST_PASSED;
}
