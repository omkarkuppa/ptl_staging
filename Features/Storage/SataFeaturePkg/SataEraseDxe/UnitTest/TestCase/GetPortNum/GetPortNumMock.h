/**@file
    Unit test case mock header for GetPortNum.

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
#ifndef _GET_PORT_NUM_MOCK_H_
#define _GET_PORT_NUM_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include "PreRequisite.h"

/**
  Get device port number from device path

  @param[in] DevicePath       Device path of device
  @param[out] Port            Port number
  @param[out] PortMultiplier  Port multiplier number

  @retval EFI_STATUS

**/
EFI_STATUS
EFIAPI
GetPortNum (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  OUT UINT16                   *Port,
  OUT UINT16                   *PortMultiplier
  );

/**
  Unit test for GetPortNum.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestGetPortNum_Success (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit test for GetPortNum.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestGetPortNum_Fail (
  IN UNIT_TEST_CONTEXT  Context
  );
#endif /*_GET_PORT_NUM_MOCK_H_*/
