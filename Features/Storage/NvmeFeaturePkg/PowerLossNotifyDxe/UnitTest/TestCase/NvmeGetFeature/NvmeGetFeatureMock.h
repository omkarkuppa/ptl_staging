/**@file
    Unit test case mock header for NvmeGetFeature.

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
#ifndef _NVME_GET_FEATURE_MOCK_H_
#define _NVME_GET_FEATURE_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/NvmExpressPassthru.h>
#include "PreRequisite.h"
#include "NvmePassThruProtocolMock.h"

//
// Define mock parameter for NvmeGetFeature Unit Test.
//
typedef struct {
  EFI_STATUS                  Status;       // Status of PassThru
  EFI_NVM_EXPRESS_COMPLETION  *Completion;  // Point to EFI_NVM_EXPRESS_COMPLETION
} PASSTHRU_GET_FEATURE_MOCK;

/**
  Unit-Test Prerequisite Function pointer.

  Nvme passthru command timeout.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreNvmeGetFeature_Fail (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Prerequisite Function pointer.

  Nvme passthru command run successful, PLN is disable.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreNvmeGetFeature_Disable (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test Prerequisite Function pointer.

  Nvme passthru command run successful, PLN is enable.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreNvmeGetFeature_Enable (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test NvmeGetFeature CleanUp Function. Clean the parameter for mock

  @param[in]  Context    Input context for mock.

**/
VOID
EFIAPI
CleanUpNvmeGetFeature (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Get feature command.

  @param[in]   NvmeDevice       The pointer to the NVME_PASS_THRU_DEVICE data structure.
  @param[in]   Sel              Select field for get feature command.
  @param[in]   NameSpaceId      Namespace ID
  @param[out]  Result           capability status

  @return EFI_SUCCESS           Successfully send command to device.
  @return EFI_DEVICE_ERROR      Fail to send command to device.

**/
EFI_STATUS
NvmeGetFeature (
  IN  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *NvmeDevice,
  IN  UINT8                                 Sel,
  IN  UINT32                                NameSpaceId,
  OUT UINT32                                *Result
  );

/**
  Unit test for NvmeGetFeature.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestNvmeGetFeature_Fail (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit test for NvmeGetFeature.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestNvmeGetFeature_Disable (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit test for NvmeGetFeature.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestNvmeGetFeature_Enable (
  IN UNIT_TEST_CONTEXT  Context
  );
#endif /*_NVME_GET_FEATURE_MOCK_H_*/
