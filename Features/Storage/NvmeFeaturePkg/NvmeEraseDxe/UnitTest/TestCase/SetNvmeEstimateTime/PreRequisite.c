/**@file
    Unit test case Prerequisite of SetNvmeEstimateTime.

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

#include "SetNvmeEstimateTimeMock.h"

EFI_BLOCK_IO_PROTOCOL       *mBlockIo      = NULL;
NVME_ADMIN_CONTROLLER_DATA  *mIdentifyData = NULL;

/**
  Unit-Test Prerequisite Function pointer.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreSetNvmeEstimateTime_SecureErase (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_BOOT_SERVICES           *BS;

  mIdentifyData = IdentifyControllerDataConstructMock ();
  mIdentifyData->Oacs |= FORMAT_NVM_SUPPORTED;
  mIdentifyData->Fna  |= BIT2;

  //
  // Mock boot service HandleProtocol
  //
  BS = BootServiceConstructorMock ();
  BS->HandleProtocol = MockBootServiceHandleProtocol;
  will_return (MockBootServiceOpenProtocol, EFI_UNSUPPORTED);
  will_return (MockBootServiceOpenProtocol, NULL);

  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Prerequisite Function pointer.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreSetNvmeEstimateTime_256MB (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_BOOT_SERVICES           *BS;

  //
  // Mock EFI_BLOCK_IO_PROTOCOL
  //
  mBlockIo = AllocateZeroPool (sizeof (EFI_BLOCK_IO_PROTOCOL));
  if (mBlockIo != NULL) {
    mBlockIo->Media = AllocateZeroPool (sizeof (EFI_BLOCK_IO_MEDIA));
    if (mBlockIo->Media != NULL) {
      mBlockIo->Media->LastBlock = 0x80000 - 1;  // set media size to 256MB
      mBlockIo->Media->BlockSize = 0x200;
    }
  }

  //
  // Mock boot service HandleProtocol
  //
  BS = BootServiceConstructorMock ();
  BS->HandleProtocol = MockBootServiceHandleProtocol;
  will_return (MockBootServiceOpenProtocol, EFI_SUCCESS);
  will_return (MockBootServiceOpenProtocol, mBlockIo);
  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Prerequisite Function pointer.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreSetNvmeEstimateTime_64GB (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_BOOT_SERVICES           *BS;

  //
  // Mock EFI_BLOCK_IO_PROTOCOL
  //
  mBlockIo = AllocateZeroPool (sizeof (EFI_BLOCK_IO_PROTOCOL));
  if (mBlockIo != NULL) {
    mBlockIo->Media = AllocateZeroPool (sizeof (EFI_BLOCK_IO_MEDIA));
    if (mBlockIo->Media != NULL) {
      mBlockIo->Media->LastBlock = 0x8000000 - 1;  // set media size to 64GB
      mBlockIo->Media->BlockSize = 0x200;
    }
  }

  //
  // Mock boot service HandleProtocol
  //
  BS = BootServiceConstructorMock ();
  BS->HandleProtocol = MockBootServiceHandleProtocol;
  will_return (MockBootServiceOpenProtocol, EFI_SUCCESS);
  will_return (MockBootServiceOpenProtocol, mBlockIo);
  return UNIT_TEST_PASSED;
}
