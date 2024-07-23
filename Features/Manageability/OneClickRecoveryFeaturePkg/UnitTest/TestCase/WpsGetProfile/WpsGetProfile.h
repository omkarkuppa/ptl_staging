
/** @file
  Unit test case of the GetAsfProfile function

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
#ifndef __WPS_GET_ASF_PROFILE_H__
#define __WPS_GET_ASF_PROFILE_H__

#include <Uefi.h>
#include <Library/UnitTestLib.h>

/**
  Verify Wifi Profile Access and Store

  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
VerifyGetAsfWifiProfile (
  IN UNIT_TEST_CONTEXT  Context
  );

#endif // __WPS_GET_ASF_PROFILE_H__
