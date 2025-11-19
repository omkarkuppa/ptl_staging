/**@file
    Unit test cases of NvmeIdentifyController.

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

#include "NvmeIdentifyControllerMock.h"

/**
  Unit test for NvmeIdentifyController.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestIdentifyController_S (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                  Status;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *NvmeDevice;
  NVME_ADMIN_CONTROLLER_DATA           ControllerData;

  NvmeDevice = AllocateZeroPool (sizeof (EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL));
  NvmeDevice->PassThru = MockNvmePassThruProtocol_PassThru;

  Status = NvmeIdentifyController (NvmeDevice, 0, &ControllerData);
  FreePool (NvmeDevice);

  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  return UNIT_TEST_PASSED;
}

/**
  Unit test for NvmeIdentifyController.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestIdentifyController_F (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                          Status;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *NvmeDevice;
  NVME_ADMIN_CONTROLLER_DATA          ControllerData;

  NvmeDevice = AllocateZeroPool (sizeof (EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL));
  NvmeDevice->PassThru = MockNvmePassThruProtocol_PassThru;

  Status = NvmeIdentifyController (NvmeDevice, 0, &ControllerData);
  FreePool (NvmeDevice);

  UT_ASSERT_EQUAL (Status, EFI_DEVICE_ERROR);
  return UNIT_TEST_PASSED;
}
