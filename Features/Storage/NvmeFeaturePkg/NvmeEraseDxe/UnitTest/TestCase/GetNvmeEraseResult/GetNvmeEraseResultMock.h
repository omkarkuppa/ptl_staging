/**@file
    Unit test case mock header for GetNvmeEraseInfo.

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
#ifndef _GET_NVME_ERASE_RESULT_MOCK_H_
#define _GET_NVME_ERASE_RESULT_MOCK_H_

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
  Return the result of erase operation on the device.

  @param[in]  This          Indicates a pointer to the calling context.
  @param[out] EraseInfo    A pointer to the data of erasing device information.

  @retval EFI_SUCCESS             Get result of erase operation successfully.
  @retval EFI_NOT_FOUND           There is no result can be found.
  @retval EFI_INVALID_PARAMETER   The input parameter is invalid.

**/
EFI_STATUS
EFIAPI
GetNvmeEraseInfo (
  IN  NVME_ERASE_BLOCK_PROTOCOL      *This,
  OUT VOID                           **EraseInfo
  );

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
  );

#endif /*_GET_NVME_ERASE_RESULT_MOCK_H_*/
