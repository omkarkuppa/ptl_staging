/**@file
    Unit test cases of NvmeGetFeature.

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

#include "NvmeGetFeatureMock.h"
#include "PowerLossNotify.h"

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
  )
{
  EFI_STATUS                          Status;
  EFI_STATUS                          *Output;
  GET_FEATURE_ATTR_CUR                CurrentAttr;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *PassThru;

  PassThru = AllocateZeroPool (sizeof (EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL));
  PassThru->PassThru = MockNvmePassThruProtocol_PassThru;

  PreRequisiteRunMock ();
  Output = (EFI_STATUS*)PreRequisiteExpectOutput ();

  Status = NvmeGetFeature (PassThru, 0, 0, (UINT32*) &CurrentAttr);
  FreePool (PassThru);
  UT_ASSERT_EQUAL (Status, (*Output));
  return UNIT_TEST_PASSED;
}

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
  )
{
  EFI_STATUS                          Status;
  EFI_STATUS                          *Output;
  GET_FEATURE_ATTR_CUR                CurrentAttr;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *PassThru;

  PassThru = AllocateZeroPool (sizeof (EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL));
  PassThru->PassThru = MockNvmePassThruProtocol_PassThru;

  PreRequisiteRunMock ();
  Output = (EFI_STATUS*)PreRequisiteExpectOutput ();

  Status = NvmeGetFeature (PassThru, 0, 0, (UINT32*) &CurrentAttr);
  FreePool (PassThru);
  UT_ASSERT_EQUAL (Status, (*Output));
  if (Status == EFI_SUCCESS) {
    UT_ASSERT_EQUAL (CurrentAttr.PlnEnable, PLN_DISABLE);
  }
  return UNIT_TEST_PASSED;
}

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
  )
{
  EFI_STATUS                          Status;
  EFI_STATUS                          *Output;
  GET_FEATURE_ATTR_CUR                CurrentAttr;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *PassThru;

  PassThru = AllocateZeroPool (sizeof (EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL));
  PassThru->PassThru = MockNvmePassThruProtocol_PassThru;

  PreRequisiteRunMock ();
  Output = (EFI_STATUS*)PreRequisiteExpectOutput ();

  Status = NvmeGetFeature (PassThru, 0, 0, (UINT32*) &CurrentAttr);
  FreePool (PassThru);
  UT_ASSERT_EQUAL (Status, (*Output));
  if (Status == EFI_SUCCESS) {
    UT_ASSERT_EQUAL (CurrentAttr.PlnEnable, PLN_ENABLE);
  }
  return UNIT_TEST_PASSED;
}
