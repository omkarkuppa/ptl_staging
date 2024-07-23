/**@file
    Unit test case mock header for SataDataCryptoErase.

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
#ifndef _SATA_CRYPTO_ERASE_MOCK_H_
#define _SATA_CRYPTO_ERASE_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include "PreRequisite.h"

#include <protocol/SataEraseBlock.h>
#include "SataEraseDxe.h"

/**
  Perform crypto erase operation on specific SATA device.

  @param[in] Private    Pointer to the device private data.
  @param[in] MediaId    The media ID that the write request is for.
  @param[in] Lba        The starting logical block address to be written.
                        The caller is responsible for writing to only
                        legitimate locations.
  @param[in,out] Token  A pointer to the token associated with the
                        transaction.
  @param[in] Size       Size data will be cleared.

  @retval EFI_SUCCESS  Operation is successful
  @retval others       Operation is failed due to some error

**/
EFI_STATUS
SataDataCryptoErase (
  IN     SATA_ERASE_BLOCK_PRIVATE    *Private,
  IN     UINT32                      MediaId,
  IN     EFI_LBA                     LBA,
  IN OUT SATA_ERASE_BLOCK_TOKEN      *Token,
  IN     UINTN                       Size
  );

/**
  Unit test for SataDataCryptoErase.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestSataDataCryptoErase (
  IN UNIT_TEST_CONTEXT  Context
  );

#endif /*_SATA_CRYPTO_ERASE_MOCK_H_*/
