/** @file
  PreRequisite functions of the UnixTimeStampToDate function.

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
  Convert Unix Timestamp to user readable YYYY/MM/DD HH:MM:SS format
  The Unix epoch is 00:00:00 UTC on 1 January 1970

  @param[in]       Timestamp     Unix timestamp in sec.
  @param[in] [out] LocalTime     Time Structure having converted value.

**/
VOID
UnixTimeStampToDate (
  IN     UINT32     Timestamp,
  IN OUT EFI_TIME   *LocalTime
  );

/**
  Set up services for UnixTimeStampToDate function.
  @retval     EFI_SUCCESS                 Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupUnixTimeStampToDateEnvironment (
  VOID
  );

/**
  Set up the environment and execute UnixTimeStampToDate function.
  @param[in]  Context                                The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED                       Set up success.
  @retval     UNIT_TEST_ERROR_PREREQUISITE_NOT_MET   Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreUnixTimeStampToDate (
  IN UNIT_TEST_CONTEXT  Context
  );
