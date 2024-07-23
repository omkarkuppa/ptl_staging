/**@file
    Unit test case mock header for SataGetEraseTimeout.

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
#ifndef _GET_ERASE_TIMEOUT_MOCK_H_
#define _GET_ERASE_TIMEOUT_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include "PreRequisite.h"

#include "AtaPassThruProtocolMock.h"

/**
  Returns estimated secure erase time

  @param[in]       ControllerData      The buffer used to store the identify controller data.

  @retval          Secure erase time in 100 ns units
**/
UINT64
SataGetEraseTimeout (
  IN ATA_IDENTIFY_DATA  *ControllerData
  );

/**
  Unit test for SataGetEraseTimeout.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestGetEraseTimeout (
  IN UNIT_TEST_CONTEXT  Context
  );

#endif /*_GET_ERASE_TIMEOUT_MOCK_H_*/
