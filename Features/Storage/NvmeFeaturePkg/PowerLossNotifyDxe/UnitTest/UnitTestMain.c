/**@file
    Unit test main of the Power Loss Notify Dxe driver.

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

#include "UnitTestMain.h"
//
// Unit Test Required Library.
//
#include <PiPei.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UnitTestLib.h>
#include <Library/PrintLib.h>

#define UNIT_TEST_NAME     "PowerLossNotifyDxe Unit Test"
#define UNIT_TEST_VERSION  "0.1"


//
// Define Unit Test Cases struct for AddTestCase().
//
typedef struct {
  CHAR8                   *Description;    // User friendly ASCII String for Test Case Description.
  CHAR8                   *ClassName;      // User friendly ASCII String for Suite Name (no spaces).
  UNIT_TEST_FUNCTION      Func;            // Unit Test Function.
  UNIT_TEST_PREREQUISITE  PreReq;          // Optional Prerequisite Function runs before test function.
  UNIT_TEST_CLEANUP       CleanUp;         // Optional Clean-up Function runs after test function.
  UNIT_TEST_CONTEXT       Context;         // UNIT_TEST_CONTEXT - Optional Pointer to Context.
  UINT8                   Attribute;       // @TODO : According this element to execute test case.
  UINT64                  Reserved;        // Reserved.
} UNIT_TEST_CASE_STRUCT;

//
// Description---------------------------------Name--------------Function-----------------------Pre------------------------------------Post---------------------------------Context---------Attribute-----Reserved
//
UNIT_TEST_CASE_STRUCT mUnitTestCasesStruct [] = {
  { "Test-NvmeSetFeature_DisableFail",       "Test NvmeSetFeature",     UnitTestNvmeSetFeature_Disable,    PreNvmeSetFeature_Fail,    UnitTestCleanup,  NULL, 0, 0},
  { "Test-NvmeSetFeature_DisableFail2",      "Test NvmeSetFeature",     UnitTestNvmeSetFeature_Disable,    PreNvmeSetFeature_Fail2,   UnitTestCleanup,  NULL, 0, 0},
  { "Test-NvmeSetFeature_DisableSuccess",    "Test NvmeSetFeature",     UnitTestNvmeSetFeature_Disable,    PreNvmeSetFeature_Success, UnitTestCleanup,  NULL, 0, 0},
  { "Test-NvmeSetFeature_EnableFail",        "Test NvmeSetFeature",     UnitTestNvmeSetFeature_Enable,     PreNvmeSetFeature_Fail,    UnitTestCleanup,  NULL, 0, 0},
  { "Test-NvmeSetFeature_EnableFail2",       "Test NvmeSetFeature",     UnitTestNvmeSetFeature_Enable,     PreNvmeSetFeature_Fail2,   UnitTestCleanup,  NULL, 0, 0},
  { "Test-NvmeSetFeature_EnableSuccess",     "Test NvmeSetFeature",     UnitTestNvmeSetFeature_Enable,     PreNvmeSetFeature_Success, UnitTestCleanup,  NULL, 0, 0},
  { "Test-NvmeGetFeature_Fail",              "Test NvmeGetFeature",     UnitTestNvmeGetFeature_Fail,       PreNvmeGetFeature_Fail,    UnitTestCleanup,  NULL, 0, 0},
  { "Test-NvmeGetFeature_Disable",           "Test NvmeGetFeature",     UnitTestNvmeGetFeature_Disable,    PreNvmeGetFeature_Disable, UnitTestCleanup,  NULL, 0, 0},
  { "Test-NvmeGetFeature_Enable",            "Test NvmeGetFeature",     UnitTestNvmeGetFeature_Enable,     PreNvmeGetFeature_Enable,  UnitTestCleanup,  NULL, 0, 0},
  { "Test-ConfigureDevicePln_DisableFail",     "Test ConfigureDevicePln", UnitTestConfigureDevicePln_Disable,PreConfigureDevicePln_Fail,           UnitTestCleanup,  NULL, 0, 0},
  { "Test-ConfigureDevicePln_EnableFail",      "Test ConfigureDevicePln", UnitTestConfigureDevicePln_Enable, PreConfigureDevicePln_Fail,           UnitTestCleanup,  NULL, 0, 0},
  { "Test-ConfigureDevicePln_DisToEnFail",     "Test ConfigureDevicePln", UnitTestConfigureDevicePln_Enable, PreConfigureDevicePln_DisToEnFail,    UnitTestCleanup,  NULL, 0, 0},
  { "Test-ConfigureDevicePln_DisToEnSuccess",  "Test ConfigureDevicePln", UnitTestConfigureDevicePln_Enable, PreConfigureDevicePln_DisToEnSuccess, UnitTestCleanup,  NULL, 0, 0},
  { "Test-ConfigureDevicePln_Enabled",         "Test ConfigureDevicePln", UnitTestConfigureDevicePln_Enable, PreConfigureDevicePln_Enabled,        UnitTestCleanup,  NULL, 0, 0},
  { "Test-PowerLossNotifyEntryPoint_1",        "Test PowerLossNotifyEntryPoint", UnitTestPlnEntryPoint_1,    NULL,        NULL,  NULL, 0, 0},
  { "Test-PowerLossNotifyEntryPoint_2",        "Test PowerLossNotifyEntryPoint", UnitTestPlnEntryPoint_2,    NULL,        NULL,  NULL, 0, 0},
  { "Test-PowerLossNotifyEntryPoint_3",        "Test PowerLossNotifyEntryPoint", UnitTestPlnEntryPoint_3,    NULL,        NULL,  NULL, 0, 0},
  { "Test-NvmePassthruNotificationEvent_Fail_1",  "Test NvmePassthruNotificationEvent", UnitTestNotificationEvent_Fail,    PreNotificationEvent_Fail_1,  UnitTestCleanup,  NULL, 0, 0},
  { "Test-NvmePassthruNotificationEvent_Fail_2",  "Test NvmePassthruNotificationEvent", UnitTestNotificationEvent_Fail,    PreNotificationEvent_Fail_2,  UnitTestCleanup,  NULL, 0, 0},
  { "Test-NvmePassthruNotificationEvent_Success", "Test NvmePassthruNotificationEvent", UnitTestNotificationEvent_Success, PreNotificationEvent_Success, UnitTestCleanup,  NULL, 0, 0},
};
/**
  Initialize the unit test framework, suite, and unit tests for the
  sample unit tests and run the unit tests.

  @retval  EFI_SUCCESS           All test cases were dispatched.
  @retval  EFI_OUT_OF_RESOURCES  There are not enough resources available to
                                 initialize the unit tests.
**/
EFI_STATUS
EFIAPI
UnitTestMain (
  VOID
  )
{
  EFI_STATUS                    Status;
  UNIT_TEST_FRAMEWORK_HANDLE    Framework;
  UNIT_TEST_SUITE_HANDLE        UnitTest;
  UINTN                         Index;

  Framework = NULL;

  DEBUG ((DEBUG_INFO, "%a v%a\n", UNIT_TEST_NAME, UNIT_TEST_VERSION));

  //
  // [Register the Unit Test Framework]
  //
  Status = InitUnitTestFramework (
             &Framework,          // Framework          : Unit test framework to be created
             UNIT_TEST_NAME,      // UNIT_TEST_NAME     : Constant Define user defined string
             gEfiCallerBaseName,  // gEfiCallerBaseName : Base name from EDK II build
             UNIT_TEST_VERSION    // UNIT_TEST_VERSION  : Constant of user defined sting version number
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed in InitUnitTestFramework. Status = %r\n", Status));
    goto EXIT;
  }

  //
  // [Create a Test Suite]
  //
  Status = CreateUnitTestSuite (
             &UnitTest,               // SuiteHandle  : Unit test suite to create
             Framework,               // Framework    : Unit test framework to add unit test suite to
             UNIT_TEST_NAME,          // "Test Title" : User friendly ASCII String for Suite Title
             UNIT_TEST_VERSION,       // "Test.Name"  : User friendly ASCII String for Suite Name (no spaces)
             NULL,                    // NULL         : Optional Setup Function runs before suite
             NULL                     // NULL         : Optional Teardown Function runs after suite
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed in CreateUnitTestSuite for Test\n"));
    goto EXIT;
  }

  //
  // [Create / Add a Test Case]
  //
  for (Index = 0; Index < ARRAY_SIZE (mUnitTestCasesStruct); Index++) {
    Status = AddTestCase (
               UnitTest,                                 // SuiteHandle        : Unit test suite to add test to
               mUnitTestCasesStruct[Index].Description,  // "Test Description" : User friendly ASCII String for Test Case Description
               mUnitTestCasesStruct[Index].ClassName,    // "Test.Name"        : User friendly ASCII String for Suite Name (no spaces)
               mUnitTestCasesStruct[Index].Func,         // UnitTestFunction   : Unit Test Function
               mUnitTestCasesStruct[Index].PreReq,       // NULL               : Optional Prerequisite Function runs before test function
               mUnitTestCasesStruct[Index].CleanUp,      // NULL               : Optional Clean-up Function runs after test function
               mUnitTestCasesStruct[Index].Context       // NULL               : UNIT_TEST_CONTEXT - Optional Pointer to Context
               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Failed in AddTestCase for Test\n"));
      goto EXIT;
    }
  }

  //
  // [Run the Test Case Suite]
  //
  Status = RunAllTestSuites (Framework);  //UNIT_TEST_FRAMEWORK_HANDLE (Same as the "InitUnitTestFramework")
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Framework Handle is NULL\n"));
    goto EXIT;
  }
EXIT:
  //
  // [Free Framework]
  //
  if (Framework) {
    FreeUnitTestFramework (Framework);
  }

  return Status;
}

/**
  Standard POSIX C entry point for host based unit test execution.
**/
int
main (
  int    argc,
  char*  argv[]
  )
{
  //
  // @TODO Need to check parameter which come from command prompt.
  //
  return UnitTestMain ();
}
