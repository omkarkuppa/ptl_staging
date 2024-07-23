/**@file
    Unit test case mock header for SetSataEstimateTime.

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
#ifndef _SET_ESTIMATE_TIME_MOCK_H_
#define _SET_ESTIMATE_TIME_MOCK_H_

#include <setjmp.h>
#include <cmocka.h>

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>

#include <Protocol/SataEraseBlock.h>
#include "SataEraseDxe.h"
#include "AtaPassThruProtocolMock.h"
#include "BootServicesMock.h"

extern ATA_IDENTIFY_DATA           *mIdentifyData;
extern EFI_BLOCK_IO_PROTOCOL       *mBlockIo;

/**
  Set the estimate time.

  @param[in] Private          Pointer to the SATA_ERASE_BLOCK_PRIVATE.
  @param[in] IdentifyData     SATA identify data.

**/
VOID
SetSataEstimateTime (
  IN SATA_ERASE_BLOCK_PRIVATE    *Private,
  IN ATA_IDENTIFY_DATA           *IdentifyData
  );

/**
  Unit-Test Prerequisite Function pointer.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreSetSataEstimateTime_SecureErase (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Prerequisite Function pointer.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreSetSataEstimateTime_256MB (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Prerequisite Function pointer.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreSetSataEstimateTime_64GB (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test SetSataEstimateTime CleanUp Function. Clean the parameter for mock

  @param[in]  Context    Input context for mock.

**/
VOID
CleanUpSetSataEstimateTime (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit test for SetSataEstimateTime.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestSetSataEstimateTime (
  IN UNIT_TEST_CONTEXT  Context
  );

#endif /*_SET_ESTIMATE_TIME_MOCK_H_*/
