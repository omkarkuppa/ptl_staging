/**@file
    Unit test case mock header for NvmeEraseBlocks.

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
#ifndef _NVME_ERASE_BLOCKS_MOCK_H_
#define _NVME_ERASE_BLOCKS_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include "PreRequisite.h"

#include "NvmePassThruProtocolMock.h"
#include <protocol/NvmeEraseBlock.h>
#include "NvmeEraseDxe.h"

/**
  Erase a specified number of device blocks.

  @param[in] This       Indicates a pointer to the calling context.
  @param[in] MediaId    The media ID that the erase request is for.
  @param[in] Lba        The starting logical block address to be erased.
                        The caller is responsible for erasing only legitimate locations.
  @param[in,out] Token  A pointer to the token associated with the transaction.
  @param[in] Size       The size in bytes to be erased. This must be a multiple
                        of the physical block size of the
                                  device.
  @param[in] EraseType  Erase type to be used to erase data on device.
  @param[in] Progress   Callback function to inform caller the progress of erase process.

  @retval EFI_SUCCESS             The erase request was queued if Event is not
                                  NULL. The data was erased correctly to the
                                  device if the Event is NULL.to the device.
  @retval EFI_WRITE_PROTECTED     The device cannot be erased due to write
                                  protection.
  @retval EFI_DEVICE_ERROR        The device reported an error while attempting
                                  to perform the erase operation.
  @retval EFI_INVALID_PARAMETER   The erase request contains LBAs that are not
                                  valid.
  @retval EFI_NO_MEDIA            There is no media in the device.
  @retval EFI_MEDIA_CHANGED       The MediaId is not for the current media.

**/
EFI_STATUS
EFIAPI
NvmeEraseBlocks (
  IN     NVME_ERASE_BLOCK_PROTOCOL     *This,
  IN     UINT32                         MediaId,
  IN     EFI_LBA                        Lba,
  IN OUT NVME_ERASE_BLOCK_TOKEN         *Token,
  IN     UINTN                          Size,
  IN     NVME_ERASE_TYPE                EraseType,
  IN     NVME_ERASE_BLOCK_PROGRESS      Progress OPTIONAL
  );

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
  );

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
  );

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
  );

#endif /*_NVME_ERASE_BLOCKS_MOCK_H_*/
