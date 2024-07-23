/** @file
  PreRequisite functions of the CountOfLeapYearsInRange function.

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

#include <Uefi.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>

/**
  Calculate number of leap years in a given year range - between (EPOCH, Year).

  Difference of the number of Leap years in range (1, EPOCH) with the number of leap
  years in range (1, Year) will be the desired output

  @param[in] Year      Upperbound of year range

  @retval    Integer   Count of leap years between EPOCH (1970) and Year.
**/
UINT32
CountOfLeapYearsInRange (
  UINT32 Year
  );

/**
  Set up services for CountOfLeapYearsInRange function.
  @retval     EFI_SUCCESS                 Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupCountOfLeapYearsInRangeEnvironment (
  VOID
  );

/**
  Set up the environment and execute CountOfLeapYearsInRange function.
  @param[in]  Context                                The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED                       Set up success.
  @retval     UNIT_TEST_ERROR_PREREQUISITE_NOT_MET   Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreCountOfLeapYearsInRange (
  IN UNIT_TEST_CONTEXT  Context
  );
