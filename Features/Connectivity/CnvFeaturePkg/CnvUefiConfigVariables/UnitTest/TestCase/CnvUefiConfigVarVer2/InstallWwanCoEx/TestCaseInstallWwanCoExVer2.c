/**@file
  Unit test case of the CnvUefiConfigVariablesVer2 driver.

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

#include <CnvUefiConfigVariables.h>
#include "TestCaseInstallWwanCoExVer2.h"
#include <BootServicesMock.h>
#include <RunTimeServicesMock.h>
#include <PrivateMock.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

extern EFI_RUNTIME_SERVICES  *gRT;

/**
  Install CnvWwanCoEx for WiFi, BT


  @retval EFI_SUCCESS        The function completed successfully

**/
extern
EFI_STATUS
InstallWwanCoEx (
  );

/**
  Update/Create Connectivity UEFI variable

  @param[in] CnvVarSize      Connectivity variable size
  @param[in] VariableName    Pointer to variable name
  @param[in] VendorGuid      Pointer to variable vendor guid
  @param[in] NewData         Pointer to variable data
  @param[in] LockVariable    Variable to be kept locked/unlocked
  @param[in] CheckVarUpdate  Check if variable needs to be updated

  @retval EFI_SUCCESS        The function completed successfully

**/
extern
EFI_STATUS
UpdateVariables (
  IN UINTN     CnvVarSize,
  IN CHAR16    *VariableName,
  IN EFI_GUID  *VendorGuid,
  IN VOID      *NewData,
  IN BOOLEAN   LockVariable,
  IN BOOLEAN   CheckVarUpdate
  );

/**
  Unit-Test Function for InstallWwanCoEx.

  @param[in]  Context                            An optional parameter for Unit Test.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestInstallWwanCoExVer2 (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS           Status;
  ICNV_WWAN_SKU_REV_1  TestCnvWwanCoExVar;
  ICNV_WWAN_SKU_REV_1  *GetCnvWwanCoExVar;

  ZeroMem (&TestCnvWwanCoExVar, sizeof (ICNV_WWAN_SKU_REV_1));
  TestCnvWwanCoExVar.Header.Revision      = 99;
  TestCnvWwanCoExVar.WwanMode             = 9;
  TestCnvWwanCoExVar.AntIsolation.LowBand = 9;

  GetCnvWwanCoExVar = AllocateZeroPool (sizeof (ICNV_WWAN_SKU_REV_1));

  //
  // Mock run time service: GetVariable for InstallWwanCoEx ()
  //
  gRT = AllocateZeroPool (sizeof (EFI_RUNTIME_SERVICES));
  gRT->GetVariable = MockGetVariable;

  expect_any_always (MockGetVariable, VariableName);
  expect_any_always (MockGetVariable, VendorGuid);
  will_return (MockGetVariable, sizeof (ICNV_WWAN_SKU_REV_1));
  will_return (MockGetVariable, GetCnvWwanCoExVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  //
  // Mock get PCD
  //
  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);

  //
  // Mock run time service: GetVariable for UpdateVariables ()
  //
  will_return (MockGetVariable, sizeof (ICNV_WWAN_SKU_REV_1));
  will_return (MockGetVariable, GetCnvWwanCoExVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  //
  // Mock get PCD for UpdateVariables ()
  //
  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);

  //
  // Execute test function with above mock environment
  //
  Status = InstallWwanCoEx ();
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  if (Status != EFI_SUCCESS) {
    return UNIT_TEST_ERROR_TEST_FAILED;
  }

  Status = UpdateVariables (
             sizeof (ICNV_WWAN_SKU_REV_1),
             CNV_UEFI_WWAN_CO_EX_VAR_NAME,
             &gUefiIntelCnvCommonVariablesGuid,
             (VOID *)&TestCnvWwanCoExVar,
             FALSE,
             FALSE
             );

  UT_ASSERT_EQUAL (TestCnvWwanCoExVar.Header.Revision, 99);
  if (TestCnvWwanCoExVar.Header.Revision != 99) {
    return UNIT_TEST_ERROR_TEST_FAILED;
  }

  UT_ASSERT_EQUAL (TestCnvWwanCoExVar.WwanMode, 9);
  if (TestCnvWwanCoExVar.WwanMode != 9) {
    return UNIT_TEST_ERROR_TEST_FAILED;
  }

  UT_ASSERT_EQUAL (TestCnvWwanCoExVar.AntIsolation.LowBand, 9);
  if (TestCnvWwanCoExVar.AntIsolation.LowBand != 9) {
    return UNIT_TEST_ERROR_TEST_FAILED;
  }

  FreePool (GetCnvWwanCoExVar);
  FreePool (gRT);

  return UNIT_TEST_PASSED;
}
