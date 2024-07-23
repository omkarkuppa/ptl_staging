/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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
#include "..\CpuUnitTestFru.h"
#include <Library/CpuMailboxLib.h>
#include <Library/CpuPlatformLib.h>
#include <OcLimits.h>
#include <Guid/MpInformation2.h>


/**
  VerifyFusedCDieList

  @param[in]  Context    [Optional] An optional parameter

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.
**/
UNIT_TEST_STATUS
EFIAPI
VerifyFusedCDieList (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  CPU_UNIT_TEST_CPU_CAPABILITY  CpuCapability;
  EFI_STATUS                    Status;
  UINT8                         HubDieId;
  UINT8                         DieIds[MAX_CDIE];
  UINTN                         NumberOfFusedDie;
  UINT8                         NumberOfFusedPCore;
  UINT8                         NumberOfFusedECore;
  UINT8                         NumberOfFusedLpECore;

  //
  // 1. Create a fake CPU
  //
  CpuUnitTestInitializeCpuCapability (&CpuCapability, NULL); // initialize a default CPU @ OneSiliconPkg\Test\Cpu\Library\CpuUnitTestInputInitLib\CpuUnitTestCpuCapabilityData.h
  CpuUnitTestInitializeRegisters (&CpuCapability);

  //
  // 2. Run target code
  //
  Status = GetHubDieId (&HubDieId);
  UT_ASSERT_EQUAL (Status, EFI_NOT_FOUND);

  NumberOfFusedDie = MAX_CDIE;
  Status           = GetFusedCDieIds (&NumberOfFusedDie, DieIds);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_EQUAL (NumberOfFusedDie, 1);
  UT_ASSERT_EQUAL (DieIds[0], 0);

  //
  // Verify die 0 has 4+8+4, which is the default unit test config. PTL use PTL_H_12XE_SA_DEVICE_ID_4C_8A as default
  //
  Status = GetCpuFusedCoreCount (0, &NumberOfFusedPCore, &NumberOfFusedECore, &NumberOfFusedLpECore);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_EQUAL (NumberOfFusedPCore, 4);
  UT_ASSERT_EQUAL (NumberOfFusedECore, 8);
  UT_ASSERT_EQUAL (NumberOfFusedLpECore, 4);

  return UNIT_TEST_PASSED;
}

/**
  Create UnitTest Suite for Core Count

  @param[in]   Framework  Unit test framework to add unit test suite to

  @retval  EFI_SUCCESS            The unit test suite was created.
**/
EFI_STATUS
EFIAPI
CreateUnitTestSuiteCoreCount (
  IN  UNIT_TEST_FRAMEWORK_HANDLE  Framework
  )
{
  UNIT_TEST_SUITE_HANDLE  Tests;
  EFI_STATUS              Status;

  //
  // Populate the Unit Test Suite.
  //
  Status = CreateUnitTestSuite (&Tests, Framework, "Core Count Test Suite", "CoreCountTest", NULL, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed in CreateUnitTestSuite for CoreCountTest\n"));
    return Status;
  }

  AddTestCase (Tests, "VerifyFusedCDieList", "VerifyFusedCDieList", VerifyFusedCDieList, NULL, CpuUnitTestReset, NULL);
  return Status;
}
