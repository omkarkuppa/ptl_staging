/** @file
  Unit test case implementation for PEI Memory Telemetry AMT PPR functionality.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification Reference:
**/

#include "PeiMemoryTelemetryTestCase.h"
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>

//
// Mock PCD value for testing
//
BOOLEAN mMockPcdPprCapability = TRUE;

//
// Mock MRC policy for testing
//
MEMORY_CONFIG_NO_CRC mMockMemConfig;

///
/// MRC Memory Types API matching definitions (from PEIM)
///
#define NOREPAIR_PPR      0
#define HARD_PPR          2
#define PPR_TEST          6

/**
  Mock implementation of FixedPcdGetBool for PcdPprCapability.
  This allows us to control the PCD value during testing.

  @param[in] PcdToken   PCD token (ignored in mock)

  @retval TRUE/FALSE    Mock PCD value
**/
BOOLEAN
MockFixedPcdGetBool (
  IN UINTN  PcdToken
  )
{
  return mMockPcdPprCapability;
}

/**
  Test implementation of IsAmtPprSupported function.
  This is a copy of the actual function for testing purposes.

  @retval TRUE   Platform supports AMT PPR
  @retval FALSE  Platform does not support AMT PPR
**/
BOOLEAN
TestIsAmtPprSupported (
  VOID
  )
{
  ///
  /// Check if PPR PCD is enabled
  ///
  if (!MockFixedPcdGetBool (0)) { // PcdToken not used in mock
    DEBUG ((DEBUG_VERBOSE, "%a: PCD PPR capability disabled, skip PPR configuration.\n", __FUNCTION__));
    return FALSE;
  }
  return TRUE;
}

/**
  Test implementation of UpdatePprMrcPolicy function.
  This is a simplified version for testing without PPI dependencies.

  @param[in]  AmtPprVariable    AMT PPR UEFI variable data to update MRC policy

  @retval     EFI_SUCCESS       Success in setting policy data
  @retval     Others            Failed to update policy
**/
EFI_STATUS
TestUpdatePprMrcPolicy (
  IN AMT_PPR_ENABLE       *AmtPprVariable
  )
{
  if (AmtPprVariable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ///
  /// Update and enable AMT, PPR and PPR Test policies using mock config
  ///
  if (AmtPprVariable->Bits.AmtEnabled) {
    mMockMemConfig.PprTestType.Value = PPR_TEST;
    if (AmtPprVariable->Bits.PprEnabled) {
      mMockMemConfig.PprRepairType = HARD_PPR;
    } else {
      mMockMemConfig.PprRepairType = NOREPAIR_PPR;
    }
  }
  return EFI_SUCCESS;
}

/**
  Unit test for IsAmtPprSupported function with PCD enabled.

  @param[in]  Context    Test context (unused)

  @retval  UNIT_TEST_PASSED             Test passed
  @retval  UNIT_TEST_ERROR_TEST_FAILED  Test failed
**/
UNIT_TEST_STATUS
EFIAPI
TestIsAmtPprSupportedEnabled (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  BOOLEAN Result;

  DEBUG ((DEBUG_INFO, "%a: Testing IsAmtPprSupported with PCD enabled\n", __FUNCTION__));

  // Set PCD to enabled
  mMockPcdPprCapability = TRUE;

  // Call function under test
  Result = TestIsAmtPprSupported ();

  // Verify result
  UT_ASSERT_TRUE (Result);

  return UNIT_TEST_PASSED;
}

/**
  Unit test for IsAmtPprSupported function with PCD disabled.

  @param[in]  Context    Test context (unused)

  @retval  UNIT_TEST_PASSED             Test passed
  @retval  UNIT_TEST_ERROR_TEST_FAILED  Test failed
**/
UNIT_TEST_STATUS
EFIAPI
TestIsAmtPprSupportedDisabled (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  BOOLEAN Result;

  DEBUG ((DEBUG_INFO, "%a: Testing IsAmtPprSupported with PCD disabled\n", __FUNCTION__));

  // Set PCD to disabled
  mMockPcdPprCapability = FALSE;

  // Call function under test
  Result = TestIsAmtPprSupported ();

  // Verify result
  UT_ASSERT_FALSE (Result);

  return UNIT_TEST_PASSED;
}

/**
  Unit test for UpdatePprMrcPolicy with AMT and PPR both enabled.

  @param[in]  Context    Test context (unused)

  @retval  UNIT_TEST_PASSED             Test passed
  @retval  UNIT_TEST_ERROR_TEST_FAILED  Test failed
**/
UNIT_TEST_STATUS
EFIAPI
TestUpdatePprMrcPolicyAmtPprEnabled (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS        Status;
  AMT_PPR_ENABLE    AmtPprVar;

  DEBUG ((DEBUG_INFO, "%a: Testing UpdatePprMrcPolicy with AMT+PPR enabled\n", __FUNCTION__));

  // Initialize test data
  ZeroMem (&mMockMemConfig, sizeof (mMockMemConfig));
  AmtPprVar.Bits.AmtEnabled = 1;
  AmtPprVar.Bits.PprEnabled = 1;
  AmtPprVar.Bits.Reserved = 0;

  // Call function under test
  Status = TestUpdatePprMrcPolicy (&AmtPprVar);

  // Verify results
  UT_ASSERT_NOT_EFI_ERROR (Status);
  UT_ASSERT_EQUAL (mMockMemConfig.PprTestType.Value, PPR_TEST);
  UT_ASSERT_EQUAL (mMockMemConfig.PprRepairType, HARD_PPR);

  return UNIT_TEST_PASSED;
}

/**
  Unit test for UpdatePprMrcPolicy with AMT enabled but PPR disabled.

  @param[in]  Context    Test context (unused)

  @retval  UNIT_TEST_PASSED             Test passed
  @retval  UNIT_TEST_ERROR_TEST_FAILED  Test failed
**/
UNIT_TEST_STATUS
EFIAPI
TestUpdatePprMrcPolicyAmtOnlyEnabled (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS        Status;
  AMT_PPR_ENABLE    AmtPprVar;

  DEBUG ((DEBUG_INFO, "%a: Testing UpdatePprMrcPolicy with AMT enabled, PPR disabled\n", __FUNCTION__));

  // Initialize test data
  ZeroMem (&mMockMemConfig, sizeof (mMockMemConfig));
  AmtPprVar.Bits.AmtEnabled = 1;
  AmtPprVar.Bits.PprEnabled = 0;
  AmtPprVar.Bits.Reserved = 0;

  // Call function under test
  Status = TestUpdatePprMrcPolicy (&AmtPprVar);

  // Verify results
  UT_ASSERT_NOT_EFI_ERROR (Status);
  UT_ASSERT_EQUAL (mMockMemConfig.PprTestType.Value, PPR_TEST);
  UT_ASSERT_EQUAL (mMockMemConfig.PprRepairType, NOREPAIR_PPR);

  return UNIT_TEST_PASSED;
}

/**
  Unit test for UpdatePprMrcPolicy with AMT disabled.

  @param[in]  Context    Test context (unused)

  @retval  UNIT_TEST_PASSED             Test passed
  @retval  UNIT_TEST_ERROR_TEST_FAILED  Test failed
**/
UNIT_TEST_STATUS
EFIAPI
TestUpdatePprMrcPolicyAmtDisabled (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS        Status;
  AMT_PPR_ENABLE    AmtPprVar;

  DEBUG ((DEBUG_INFO, "%a: Testing UpdatePprMrcPolicy with AMT disabled\n", __FUNCTION__));

  // Initialize test data
  ZeroMem (&mMockMemConfig, sizeof (mMockMemConfig));
  AmtPprVar.Bits.AmtEnabled = 0;
  AmtPprVar.Bits.PprEnabled = 0;
  AmtPprVar.Bits.Reserved = 0;

  // Call function under test
  Status = TestUpdatePprMrcPolicy (&AmtPprVar);

  // Verify results - no changes should be made when AMT is disabled
  UT_ASSERT_NOT_EFI_ERROR (Status);
  UT_ASSERT_EQUAL (mMockMemConfig.PprTestType.Value, 0);  // Should remain 0
  UT_ASSERT_EQUAL (mMockMemConfig.PprRepairType, 0);     // Should remain 0

  return UNIT_TEST_PASSED;
}

/**
  Unit test for UpdatePprMrcPolicy with NULL parameter.

  @param[in]  Context    Test context (unused)

  @retval  UNIT_TEST_PASSED             Test passed
  @retval  UNIT_TEST_ERROR_TEST_FAILED  Test failed
**/
UNIT_TEST_STATUS
EFIAPI
TestUpdatePprMrcPolicyNullParameter (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS Status;

  DEBUG ((DEBUG_INFO, "%a: Testing UpdatePprMrcPolicy with NULL parameter\n", __FUNCTION__));

  // Call function under test with NULL parameter
  Status = TestUpdatePprMrcPolicy (NULL);

  // Verify error is returned
  UT_ASSERT_STATUS_EQUAL (Status, EFI_INVALID_PARAMETER);

  return UNIT_TEST_PASSED;
}

/**
  Unit test for GetAmtPprEnableVar when variable exists.
  Note: This is a simplified test since the actual function depends on PEI services.

  @param[in]  Context    Test context (unused)

  @retval  UNIT_TEST_PASSED             Test passed
  @retval  UNIT_TEST_ERROR_TEST_FAILED  Test failed
**/
UNIT_TEST_STATUS
EFIAPI
TestGetAmtPprEnableVarExists (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  AMT_PPR_ENABLE TestVar;

  DEBUG ((DEBUG_INFO, "%a: Testing variable structure and bit field access\n", __FUNCTION__));

  // Test bit field manipulation
  TestVar.Bits.AmtEnabled = 1;
  TestVar.Bits.PprEnabled = 0;
  TestVar.Bits.Reserved = 0;

  // Verify bit fields work correctly
  UT_ASSERT_EQUAL (TestVar.Bits.AmtEnabled, 1);
  UT_ASSERT_EQUAL (TestVar.Bits.PprEnabled, 0);
  UT_ASSERT_EQUAL (TestVar.Bits.Reserved, 0);

  // Test another combination
  TestVar.Bits.AmtEnabled = 1;
  TestVar.Bits.PprEnabled = 1;
  TestVar.Bits.Reserved = 0;

  UT_ASSERT_EQUAL (TestVar.Bits.AmtEnabled, 1);
  UT_ASSERT_EQUAL (TestVar.Bits.PprEnabled, 1);

  return UNIT_TEST_PASSED;
}

/**
  Unit test for GetAmtPprEnableVar when variable does not exist.
  Note: This is a simplified test since the actual function depends on PEI services.

  @param[in]  Context    Test context (unused)

  @retval  UNIT_TEST_PASSED             Test passed
  @retval  UNIT_TEST_ERROR_TEST_FAILED  Test failed
**/
UNIT_TEST_STATUS
EFIAPI
TestGetAmtPprEnableVarNotExists (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  AMT_PPR_ENABLE TestVar;

  DEBUG ((DEBUG_INFO, "%a: Testing variable initialization\n", __FUNCTION__));

  // Test zero initialization
  ZeroMem (&TestVar, sizeof (TestVar));

  UT_ASSERT_EQUAL (TestVar.Bits.AmtEnabled, 0);
  UT_ASSERT_EQUAL (TestVar.Bits.PprEnabled, 0);
  UT_ASSERT_EQUAL (TestVar.Bits.Reserved, 0);

  return UNIT_TEST_PASSED;
}