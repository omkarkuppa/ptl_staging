/**@file
    Unit test case Prerequisite of SetUfsEstimateTime.

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

#include "SetUfsEstimateTimeMock.h"

EFI_BOOT_SERVICES           *mBS      = NULL;
EFI_BLOCK_IO_PROTOCOL       *mBlockIo = NULL;

/**
  Queries a handle to determine if it supports a specified protocol.

  @param  UserHandle             The handle being queried.
  @param  Protocol               The published unique identifier of the protocol.
  @param  Interface              Supplies the address where a pointer to the
                                 corresponding Protocol Interface is returned.

  @retval EFI_SUCCESS            The interface information for the specified protocol was returned.
  @retval EFI_UNSUPPORTED        The device does not support the specified protocol.
  @retval EFI_INVALID_PARAMETER  Handle is NULL..
  @retval EFI_INVALID_PARAMETER  Protocol is NULL.
  @retval EFI_INVALID_PARAMETER  Interface is NULL.

**/
EFI_STATUS
EFIAPI
MockBootServiceHandleProtocol (
  IN EFI_HANDLE  UserHandle,
  IN EFI_GUID    *Protocol,
  OUT VOID       **Interface
  )
{
  EFI_STATUS Status;

  Status = mock_type (EFI_STATUS);
  *Interface = mock_type (VOID *);

  return Status;
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
PreSetUfsEstimateTime_BlockErase (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_ERASE_BLOCK_PROTOCOL  *EraseBlockProtocol;

  //
  // Mock EFI_ERASE_BLOCK_PROTOCOL
  //
  EraseBlockProtocol = AllocateZeroPool (sizeof (EFI_ERASE_BLOCK_PROTOCOL));

  //
  // Mock boot service HandleProtocol
  //
  mBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS = mBS;
  mBS->HandleProtocol = MockBootServiceHandleProtocol;
  will_return (MockBootServiceHandleProtocol, EFI_UNSUPPORTED);
  will_return (MockBootServiceHandleProtocol, mBlockIo);
  will_return (MockBootServiceHandleProtocol, EFI_SUCCESS);
  will_return (MockBootServiceHandleProtocol, EraseBlockProtocol);

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
PreSetUfsEstimateTime_128MB (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  //
  // Mock EFI_BLOCK_IO_PROTOCOL
  //
  mBlockIo = AllocateZeroPool (sizeof (EFI_BLOCK_IO_PROTOCOL));
  if (mBlockIo != NULL) {
    mBlockIo->Media = AllocateZeroPool (sizeof (EFI_BLOCK_IO_MEDIA));
    if (mBlockIo->Media != NULL) {
      mBlockIo->Media->LastBlock = 0x40000 - 1;  // set media size to 128MB
      mBlockIo->Media->BlockSize = 0x200;
    }
  }
  //
  // Mock boot service HandleProtocol
  //
  mBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS = mBS;
  mBS->HandleProtocol = MockBootServiceHandleProtocol;
  will_return (MockBootServiceHandleProtocol, EFI_SUCCESS);
  will_return (MockBootServiceHandleProtocol, mBlockIo);
  will_return (MockBootServiceHandleProtocol, EFI_SUCCESS);
  will_return (MockBootServiceHandleProtocol, NULL);
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
PreSetUfsEstimateTime_256MB (
  IN UNIT_TEST_CONTEXT  Context
  )
{
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
  mBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS = mBS;
  mBS->HandleProtocol = MockBootServiceHandleProtocol;
  will_return (MockBootServiceHandleProtocol, EFI_SUCCESS);
  will_return (MockBootServiceHandleProtocol, mBlockIo);
  will_return (MockBootServiceHandleProtocol, EFI_SUCCESS);
  will_return (MockBootServiceHandleProtocol, NULL);
  return UNIT_TEST_PASSED;
}
