/**@file
    Unit test main of the Nvme Erase Dxe driver.

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

#define UNIT_TEST_NAME     "NvmeEraseDxe Unit Test"
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
  { "Test-GetParentProtocol_Fail_1",          "GetParentProtocol",         UnitTestGetParentProtocol,         PreGetParentProtocol_Fail_1,         UnitTestCleanup, NULL,           0,            0},
  { "Test-GetParentProtocol_Fail_2",          "GetParentProtocol",         UnitTestGetParentProtocol,         PreGetParentProtocol_Fail_2,         UnitTestCleanup, NULL,           0,            0},
  { "Test-GetParentProtocol_Fail_3",          "GetParentProtocol",         UnitTestGetParentProtocol,         PreGetParentProtocol_Fail_3,         UnitTestCleanup, NULL,           0,            0},
  { "Test-GetParentProtocol_Success",         "GetParentProtocol",         UnitTestGetParentProtocol,         PreGetParentProtocol_Success,        UnitTestCleanup, NULL,           0,            0},
  { "Test-IsNvmeDevice_True",                 "IsNvmeDevice",              UnitTestIsNvmeDevice,              PreIsNvmeDevice_True,                UnitTestCleanup, NULL,           0,            0},
  { "Test-IsNvmeDevice_False",                "IsNvmeDevice",              UnitTestIsNvmeDevice,              PreIsNvmeDevice_False,               UnitTestCleanup, NULL,           0,            0},
  { "Test-NvmeEraseEntryPoint_1",             "NvmeEraseEntryPoint",       UnitTestNvmeEraseEntryPoint,       PreNvmeEraseEntryPoint_1,            UnitTestCleanup, NULL,           0,            0},
  { "Test-NvmeEraseEntryPoint_2",             "NvmeEraseEntryPoint",       UnitTestNvmeEraseEntryPoint,       PreNvmeEraseEntryPoint_2,            UnitTestCleanup, NULL,           0,            0},
  { "Test-NvmeEraseBindingSupported_1",       "NvmeEraseBindingSupported", UnitTestNvmeEraseBindingSupported, PreNvmeEraseBindingSupported_1,      UnitTestCleanup, NULL,           0,            0},
  { "Test-NvmeEraseBindingSupported_2",       "NvmeEraseBindingSupported", UnitTestNvmeEraseBindingSupported, PreNvmeEraseBindingSupported_2,      UnitTestCleanup, NULL,           0,            0},
  { "Test-NvmeEraseBindingSupported_3",       "NvmeEraseBindingSupported", UnitTestNvmeEraseBindingSupported, PreNvmeEraseBindingSupported_3,      UnitTestCleanup, NULL,           0,            0},
  { "Test-NvmeEraseBindingSupported_4",       "NvmeEraseBindingSupported", UnitTestNvmeEraseBindingSupported, PreNvmeEraseBindingSupported_4,      UnitTestCleanup, NULL,           0,            0},
  { "Test-NvmeEraseBindingSupported_5",       "NvmeEraseBindingSupported", UnitTestNvmeEraseBindingSupported, PreNvmeEraseBindingSupported_5,      UnitTestCleanup, NULL,           0,            0},
  { "Test-NvmeIdentifyController_S",          "NvmeIdentifyController",    UnitTestIdentifyController_S,      PreIdentifyController_S,             CleanUpIdentifyController, NULL, 0,            0},
  { "Test-NvmeIdentifyController_F",          "NvmeIdentifyController",    UnitTestIdentifyController_F,      NULL,                                NULL,            NULL,           0,            0},
  { "Test-NvmeFormatNvmCommand_S",            "NvmeFormatNvmCommand",      UnitTestFormatNvmCommand_S,        PreFormatNvmCommand_S,               CleanUpFormatNvmCommand,   NULL, 0,            0},
  { "Test-NvmeFormatNvmCommand_F",            "NvmeFormatNvmCommand",      UnitTestFormatNvmCommand_F,        PreFormatNvmCommand_F,               CleanUpFormatNvmCommand,   NULL, 0,            0},
  { "Test-NvmeDataOverwrite",                 "NvmeDataOverwrite",         UnitTestDataOverwrite,             NULL,                                NULL,            NULL,           0,            0},
  { "Test-NvmeDataBlockErase_S",              "NvmeDataBlockErase",        UnitTestDataBlockErase_S,          PreDataBlockErase_S,                 CleanUpDataBlockErase,     NULL, 0,            0},
  { "Test-NvmeDataBlockErase_F",              "NvmeDataBlockErase",        UnitTestDataBlockErase_F,          PreDataBlockErase_F,                 CleanUpDataBlockErase,     NULL, 0,            0},
  { "Test-NvmeDataCryptoErase_S",             "NvmeDataCryptoErase",       UnitTestDataCryptoErase_S,         PreDataCryptoErase_S,                CleanUpDataCryptoErase,    NULL, 0,            0},
  { "Test-NvmeDataCryptoErase_F",             "NvmeDataCryptoErase",       UnitTestDataCryptoErase_F,         PreDataCryptoErase_F,                CleanUpDataCryptoErase,    NULL, 0,            0},
  { "Test-UpdateResult",                      "UpdateResult",              UnitTestUpdateResult,              NULL,                                NULL,            NULL,           0,            0},
  { "Test-GetNvmeEraseInfo  ",                "GetNvmeEraseInfo",          UnitTestGetNvmeEraseInfo,          NULL,                                NULL,            NULL,           0,            0},
  { "Test-NvmeEraseBlocks_1",                 "NvmeEraseBlocks",           UnitTestEraseBlocks_1,             NULL,                                NULL,            NULL,           0,            0},
  { "Test-NvmeEraseBlocks_2",                 "NvmeEraseBlocks",           UnitTestEraseBlocks_2,             NULL,                                NULL,            NULL,           0,            0},
  { "Test-NvmeEraseBlocks_3",                 "NvmeEraseBlocks",           UnitTestEraseBlocks_3,             NULL,                                NULL,            NULL,           0,            0},
  { "Test-GetNvmeEstimateTime",               "GetNvmeEstimateTime",       UnitTestGetNvmeEstimateTime,       NULL,                                NULL,            NULL,           0,            0},
  { "Test-SetNvmeEstimateTime",               "SetNvmeEstimateTime",       UnitTestSetNvmeEstimateTime,       NULL,                                NULL,            NULL,           0,            0},
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
