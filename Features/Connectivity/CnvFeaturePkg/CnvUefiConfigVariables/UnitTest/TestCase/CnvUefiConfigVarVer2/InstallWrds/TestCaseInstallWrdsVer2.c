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
#include "TestCaseInstallWrdsVer2.h"
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
  Install WRDS for WiFi


  @retval EFI_SUCCESS        The function completed successfully

**/
extern
EFI_STATUS
InstallWrds (
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
  Unit-Test Function for InstallWrds.

  @param[in]  Context                            An optional parameter for Unit Test.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestInstallWrdsVer2 (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                  Status;
  CNV_UEFI_VAR_WRDS_WIFI_SAR  TestWrdsVar;
  CNV_UEFI_VAR_WRDS_WIFI_SAR  *GetWrdsVar;

  ZeroMem (&TestWrdsVar, sizeof (CNV_UEFI_VAR_WRDS_WIFI_SAR));
  TestWrdsVar.Header.Revision = 99;

  GetWrdsVar = AllocateZeroPool (sizeof (CNV_UEFI_VAR_WRDS_WIFI_SAR));

  //
  // Mock run time service: GetVariable for InstallWrds ()
  //
  gRT = AllocateZeroPool (sizeof (EFI_RUNTIME_SERVICES));
  gRT->GetVariable = MockGetVariable;

  expect_any_always (MockGetVariable, VariableName);
  expect_any_always (MockGetVariable, VendorGuid);
  will_return (MockGetVariable, sizeof (CNV_UEFI_VAR_WRDS_WIFI_SAR));
  will_return (MockGetVariable, GetWrdsVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  //
  // Mock get PCD
  //
  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);

  //
  // Mock run time service: GetVariable for UpdateVariables ()
  //
  will_return (MockGetVariable, sizeof (CNV_UEFI_VAR_WRDS_WIFI_SAR));
  will_return (MockGetVariable, GetWrdsVar);
  will_return (MockGetVariable, EFI_SUCCESS);

  //
  // Mock get PCD for UpdateVariables ()
  //
  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdCnvGuidLockStatus);
  will_return (LibPcdGet8, 0x0);

  //
  // Execute test function with above mock environment
  //
  Status = InstallWrds ();
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  if (Status != EFI_SUCCESS) {
    return UNIT_TEST_ERROR_TEST_FAILED;
  }

  Status = UpdateVariables (
             sizeof (CNV_UEFI_VAR_WRDS_WIFI_SAR),
             CNV_UEFI_WRDS_VAR_NAME,
             &gUefiIntelCnvWlanVariablesGuid,
             (VOID *)&TestWrdsVar,
             FALSE,
             FALSE
             );

  UT_ASSERT_EQUAL (TestWrdsVar.Header.Revision, 99);
  if (TestWrdsVar.Header.Revision != 99) {
    return UNIT_TEST_ERROR_TEST_FAILED;
  }

  FreePool (GetWrdsVar);
  FreePool (gRT);

  return UNIT_TEST_PASSED;
}
