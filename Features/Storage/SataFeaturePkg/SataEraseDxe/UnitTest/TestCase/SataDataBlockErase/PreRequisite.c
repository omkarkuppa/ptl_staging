/**@file
    Unit test case Prerequisite of SataDataBlockErase.

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

#include "SataDataBlockEraseMock.h"

/**
  Unit-Test Prerequisite Function pointer.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreDataBlockErase_S (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  ATA_IDENTIFY_DATA *IdentifyData;

  //
  // Mock Idenfity device data
  // refer to ATA spec, Security supported
  //
  IdentifyData = IdentifyDataConstructMock ();
  IdentifyData->security_status |= BIT0;

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
PreDataBlockErase_F (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  ATA_IDENTIFY_DATA *IdentifyData;

  //
  // Mock Idenfity device data
  // refer to ATA spec, Security supported, enabled and frozen
  //
  IdentifyData = IdentifyDataConstructMock ();
  IdentifyData->security_status |= BIT0 | BIT1 | BIT3;

  return UNIT_TEST_PASSED;
}

