/**@file
    Unit test case mock header for ConfigureDevicePln.

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
#ifndef _CONFIGURE_DEVICE_PLN_MOCK_H_
#define _CONFIGURE_DEVICE_PLN_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/NvmExpressPassthru.h>
#include "PreRequisite.h"
#include "NvmePassThruProtocolMock.h"

/**
  Unit-Test Prerequisite Function pointer.

  Get PLN feature failure.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreConfigureDevicePln_Fail (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Prerequisite Function pointer.

  PLN is disable, Enable the PLN, but enable failure.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreConfigureDevicePln_DisToEnFail (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Prerequisite Function pointer.

  PLN is disable, Enable the PLN and success.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreConfigureDevicePln_DisToEnSuccess (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Prerequisite Function pointer.

  PLN is enabled, Enable the PLN, return success directly.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreConfigureDevicePln_Enabled (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test ConfigureDevicePln CleanUp Function. Clean the parameter for mock

  @param[in]  Context    Input context for mock.

**/
VOID
EFIAPI
CleanUpConfigureDevicePln (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Enable or disable PLN on device if device support PLN

  @param[in]  NvmePassthru        The pointer to the NVME_PASS_THRU_DEVICE data structure.
  @param[in]  NameSpaceId         Name space ID.
  @param[in]  EnablePln           Enable / Disable PLN feature.

**/
EFI_STATUS
ConfigureDevicePln (
  IN  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *NvmePassthru,
  IN  UINT32                                NameSpaceId,
  IN  UINT8                                 EnablePln
  );

/**
  Unit test for ConfigureDevicePln with PLN disable.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestConfigureDevicePln_Disable (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit test for ConfigureDevicePln with PLN enable.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestConfigureDevicePln_Enable (
  IN UNIT_TEST_CONTEXT  Context
  );
#endif /*_CONFIGURE_DEVICE_PLN_MOCK_H_*/
