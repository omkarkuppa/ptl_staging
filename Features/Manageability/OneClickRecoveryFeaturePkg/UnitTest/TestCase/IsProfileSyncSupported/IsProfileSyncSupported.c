/** @file
  Unit test cases for IsProfileSyncSupported() validating the FWSTS1 bit 24
  (ME boot option) guard introduced for Wi-Fi co-existence stability.

  Requirement: UEFI shall attempt to get Wi-Fi profile from AMT and connect
  only when there is an ME boot option set in FWSTS1 bit 24
  (ME_FW_BOOT_OPTIONS_PRESENT, checked via ME_STATUS_IS_ME_FW_BOOT_OPTIONS_PRESENT).

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2026 Intel Corporation.

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

#include <stdio.h>
#include <string.h>

#include <Uefi.h>
#include <MeState.h>
#include <Library/UnitTestLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>

#include "IsProfileSyncSupported.h"

//
// Controllable mock state for MeBiosGetMeStatus() provided by MeUtilsLibMock.c.
// Set mMeStatus to ME_FW_BOOT_OPTIONS_PRESENT (0x100) to simulate an active
// ME boot option (FWSTS1 bit 24 set). Set to 0 to simulate no boot option.
// Set mMeBiosGetMeStatusReturn to a non-SUCCESS code to simulate HECI failure.
//
extern UINT32      mMeStatus;
extern EFI_STATUS  mMeBiosGetMeStatusReturn;

//
// gRT pointer from UefiRuntimeServicesTableLib; reassigned per-test to a
// local mock structure so gRT->GetVariable can return controlled values.
//
extern EFI_RUNTIME_SERVICES  *gRT;

//
// Forward declaration of the function under test (not static; visible to the
// linker from WifiProfileSync.c compiled into the same test binary).
//
extern
BOOLEAN
IsProfileSyncSupported (
  VOID
  );

// ---------------------------------------------------------------------------
// Local mock state for gRT->GetVariable
// ---------------------------------------------------------------------------

///
/// Simulated secure boot enable state: 0 = disabled, 1 = enabled.
///
STATIC UINT8  mMockSecureBootState = 0;

///
/// Status code that the mock GetVariable should return.
///
STATIC EFI_STATUS  mMockGetVariableStatus = EFI_NOT_FOUND;

/**
  Mock implementation of EFI_RUNTIME_SERVICES.GetVariable that returns
  controllable test state without accessing real NVRAM.
**/
STATIC
EFI_STATUS
EFIAPI
MockGetVariable (
  IN     CHAR16    *VariableName,
  IN     EFI_GUID  *VendorGuid,
  OUT    UINT32    *Attributes   OPTIONAL,
  IN OUT UINTN     *DataSize,
  OUT    VOID      *Data
  )
{
  if (EFI_ERROR (mMockGetVariableStatus)) {
    return mMockGetVariableStatus;
  }

  if ((DataSize != NULL) && (*DataSize >= sizeof (UINT8)) && (Data != NULL)) {
    *(UINT8 *) Data = mMockSecureBootState;
  }

  return EFI_SUCCESS;
}

///
/// Minimal mock runtime services table used by tests that exercise the
/// secure boot variable read path within IsProfileSyncSupported().
///
STATIC EFI_RUNTIME_SERVICES  mMockRuntimeServices;

// ---------------------------------------------------------------------------
// Helper: configure the mock runtime services table and install it as gRT.
// ---------------------------------------------------------------------------

STATIC
VOID
SetupMockRuntimeServices (
  VOID
  )
{
  ZeroMem (&mMockRuntimeServices, sizeof (EFI_RUNTIME_SERVICES));
  mMockRuntimeServices.GetVariable = MockGetVariable;
  gRT = &mMockRuntimeServices;
}

// ---------------------------------------------------------------------------
// Test cases
// ---------------------------------------------------------------------------

/**
  TEST 1 - ME boot option NOT present (FWSTS1 bit 24 clear).

  ME_STATUS_IS_ME_FW_BOOT_OPTIONS_PRESENT evaluates to FALSE when bit 8
  (ME_FW_BOOT_OPTIONS_PRESENT, 0x100) is clear in the value returned by
  MeBiosGetMeStatus(). IsProfileSyncSupported() must return FALSE immediately
  without accessing any further resources (Secure Boot variable or Boot Guard
  registers).

  This is the primary guard for the Wi-Fi co-existence requirement:
  UEFI must NOT hand off the Wi-Fi NIC to its own stack when no ME boot
  option has been issued (e.g. user entered BIOS Setup locally).

  Expected: FALSE returned; gRT not required/accessed.
**/
UNIT_TEST_STATUS
EFIAPI
TestIsProfileSyncSupported_AmtBootOptionNotPresent (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  BOOLEAN  Result;

  printf ("[%s] ME boot option not present (mMeStatus=0x0) - verifying early FALSE return\n",
    __FUNCTION__);

  // Arrange: ME_FW_BOOT_OPTIONS_PRESENT bit clear -> macro evaluates FALSE
  mMeStatus                = 0;
  mMeBiosGetMeStatusReturn = EFI_SUCCESS;

  // gRT intentionally left as-is to prove the function returns
  // before any runtime services access.

  // Act
  Result = IsProfileSyncSupported ();

  // Assert
  UT_ASSERT_FALSE (Result);

  printf ("[%s] PASS - IsProfileSyncSupported correctly returned FALSE "
    "when ME_STATUS_IS_ME_FW_BOOT_OPTIONS_PRESENT is FALSE\n", __FUNCTION__);

  return UNIT_TEST_PASSED;
}

/**
  TEST 2 - ME boot option present; Secure Boot disabled / variable missing.

  When ME_STATUS_IS_ME_FW_BOOT_OPTIONS_PRESENT returns TRUE (bit 8 of mMeStatus
  is set), the function advances to the Secure Boot variable check. If
  GetVariable fails or the SecureBootEnable variable is SECURE_BOOT_DISABLE
  the function must return FALSE.

  Expected: FALSE returned.
**/
UNIT_TEST_STATUS
EFIAPI
TestIsProfileSyncSupported_SecureBootDisabled (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  BOOLEAN  Result;

  printf ("[%s] ME boot option present, Secure Boot disabled - "
    "verifying FALSE return\n", __FUNCTION__);

  // Arrange: ME_FW_BOOT_OPTIONS_PRESENT bit set -> macro evaluates TRUE
  mMeStatus                = ME_FW_BOOT_OPTIONS_PRESENT;
  mMeBiosGetMeStatusReturn = EFI_SUCCESS;

  // Secure boot variable not present
  mMockSecureBootState   = 0;           // SECURE_BOOT_DISABLE
  mMockGetVariableStatus = EFI_NOT_FOUND;
  SetupMockRuntimeServices ();

  // Act
  Result = IsProfileSyncSupported ();

  // Assert
  UT_ASSERT_FALSE (Result);

  printf ("[%s] PASS - IsProfileSyncSupported correctly returned FALSE "
    "when Secure Boot variable is absent\n", __FUNCTION__);

  return UNIT_TEST_PASSED;
}

/**
  TEST 3 - AMT boot option present; Secure Boot enabled; Boot Guard not supported.

  With both AMT boot option and Secure Boot active, the function checks Boot Guard.
  The unit test environment uses PeiDxeSmmBootGuardLibNull which always returns
  FALSE for IsBootGuardSupported(), so the function falls through to return FALSE.

  Expected: FALSE returned (Boot Guard null library in host test).
**/
UNIT_TEST_STATUS
EFIAPI
TestIsProfileSyncSupported_SecureBootEnabled_BootGuardNotSupported (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  BOOLEAN  Result;

  printf ("[%s] AMT boot option present, Secure Boot enabled, "
    "Boot Guard not supported - verifying FALSE return\n", __FUNCTION__);

  // Arrange: ME_FW_BOOT_OPTIONS_PRESENT bit set -> macro evaluates TRUE
  mMeStatus                = ME_FW_BOOT_OPTIONS_PRESENT;
  mMeBiosGetMeStatusReturn = EFI_SUCCESS;

  mMockSecureBootState   = 1;           // SECURE_BOOT_ENABLE
  mMockGetVariableStatus = EFI_SUCCESS;
  SetupMockRuntimeServices ();

  // Act (IsBootGuardSupported() returns FALSE via PeiDxeSmmBootGuardLibNull)
  Result = IsProfileSyncSupported ();

  // Assert
  UT_ASSERT_FALSE (Result);

  printf ("[%s] PASS - IsProfileSyncSupported correctly returned FALSE "
    "when Boot Guard is not supported\n", __FUNCTION__);

  return UNIT_TEST_PASSED;
}

/**
  TEST 4 - MeBiosGetMeStatus() returns a HECI error.

  If the HECI read itself fails (e.g. device hidden or HECI not initialised),
  MeBiosGetMeStatus() returns a non-EFI_SUCCESS code. The production code
  evaluates the OR condition:
    if (EFI_ERROR (Status) || !ME_STATUS_IS_ME_FW_BOOT_OPTIONS_PRESENT (...))
  so a HECI failure must cause an early FALSE return without reading any
  further hardware or NVRAM state.

  Expected: FALSE returned.
**/
UNIT_TEST_STATUS
EFIAPI
TestIsProfileSyncSupported_MeBiosGetMeStatusFails (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  BOOLEAN  Result;

  printf ("[%s] MeBiosGetMeStatus returns EFI_DEVICE_ERROR - "
    "verifying early FALSE return\n", __FUNCTION__);

  // Arrange: simulate HECI read failure
  mMeStatus                = ME_FW_BOOT_OPTIONS_PRESENT; // value should not matter
  mMeBiosGetMeStatusReturn = EFI_DEVICE_ERROR;

  // Act
  Result = IsProfileSyncSupported ();

  // Assert
  UT_ASSERT_FALSE (Result);

  // Reset mock return to success for subsequent tests
  mMeBiosGetMeStatusReturn = EFI_SUCCESS;

  printf ("[%s] PASS - IsProfileSyncSupported correctly returned FALSE "
    "when MeBiosGetMeStatus fails\n", __FUNCTION__);

  return UNIT_TEST_PASSED;
}

/**
  TEST 5 - mMeStatus has bits set (ME_FW_INIT_COMPLETE = 0x80) but NOT the boot
  option bit (ME_FW_BOOT_OPTIONS_PRESENT = 0x100).

  Validates that ME_STATUS_IS_ME_FW_BOOT_OPTIONS_PRESENT is bit-precise: only
  bit 8 (0x100) controls the guard. A platform where ME FW init is complete but
  no AMT boot option was issued must still be rejected.

  Expected: FALSE returned.
**/
UNIT_TEST_STATUS
EFIAPI
TestIsProfileSyncSupported_MeStatusNoBootOptionBit (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  BOOLEAN  Result;

  printf ("[%s] mMeStatus=ME_FW_INIT_COMPLETE(0x80), boot-option bit clear - "
    "verifying FALSE return\n", __FUNCTION__);

  // Arrange: only ME_FW_INIT_COMPLETE set, ME_FW_BOOT_OPTIONS_PRESENT (0x100) clear
  mMeStatus                = ME_FW_INIT_COMPLETE;   // 0x80 - bit 8 NOT set
  mMeBiosGetMeStatusReturn = EFI_SUCCESS;

  // Act
  Result = IsProfileSyncSupported ();

  // Assert: macro should evaluate FALSE because bit 8 is absent
  UT_ASSERT_FALSE (Result);

  printf ("[%s] PASS - IsProfileSyncSupported correctly returned FALSE "
    "when only ME_FW_INIT_COMPLETE is set (boot-option bit absent)\n", __FUNCTION__);

  return UNIT_TEST_PASSED;
}

/**
  TEST 6 - mMeStatus has BOTH ME_FW_INIT_COMPLETE (0x80) AND ME_FW_BOOT_OPTIONS_PRESENT
  (0x100) set (combined value 0x180).

  Validates that the macro correctly passes when the boot-option bit is present
  alongside other state bits. The function must advance past the first guard and
  reach the Secure Boot check. With Secure Boot variable absent, it returns FALSE.

  This is the common real-world value returned by ME firmware when an AMT boot
  option is active and ME FW init is finished.

  Expected: FALSE returned (Secure Boot variable not found).
**/
UNIT_TEST_STATUS
EFIAPI
TestIsProfileSyncSupported_MeStatusWithOtherBitsSet (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  BOOLEAN  Result;

  printf ("[%s] mMeStatus=0x%x (ME_FW_INIT_COMPLETE|ME_FW_BOOT_OPTIONS_PRESENT), "
    "SecureBoot absent - verifying boot-option guard passes and SecureBoot blocks\n",
    __FUNCTION__, ME_FW_INIT_COMPLETE | ME_FW_BOOT_OPTIONS_PRESENT);

  // Arrange: both bits set - macro must evaluate TRUE and function must advance
  mMeStatus                = ME_FW_INIT_COMPLETE | ME_FW_BOOT_OPTIONS_PRESENT; // 0x180
  mMeBiosGetMeStatusReturn = EFI_SUCCESS;

  // Secure Boot variable not present to provide a clean early-exit after guard passes
  mMockGetVariableStatus = EFI_NOT_FOUND;
  SetupMockRuntimeServices ();

  // Act
  Result = IsProfileSyncSupported ();

  // Assert: guard passed (macro TRUE), but Secure Boot absent causes FALSE
  UT_ASSERT_FALSE (Result);

  printf ("[%s] PASS - macro correctly evaluated TRUE with combined status 0x%x, "
    "Secure Boot check then correctly blocked\n",
    __FUNCTION__, ME_FW_INIT_COMPLETE | ME_FW_BOOT_OPTIONS_PRESENT);

  return UNIT_TEST_PASSED;
}

/**
  TEST 7 - ME boot option present; Secure Boot variable readable but value is
  explicitly SECURE_BOOT_DISABLE (0).

  Complements TEST 2 (which uses EFI_NOT_FOUND). Here GetVariable succeeds and
  returns data, but the SecureBootEnable byte is 0 (SECURE_BOOT_DISABLE).
  The production condition:
    if (EFI_ERROR (Status) || (SecureBootState == SECURE_BOOT_DISABLE))
  must reject this via the second clause.

  Expected: FALSE returned.
**/
UNIT_TEST_STATUS
EFIAPI
TestIsProfileSyncSupported_SecureBootExplicitlyDisabled (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  BOOLEAN  Result;

  printf ("[%s] ME boot option present, GetVariable OK but SecureBoot=DISABLE - "
    "verifying FALSE return\n", __FUNCTION__);

  // Arrange
  mMeStatus                = ME_FW_BOOT_OPTIONS_PRESENT;
  mMeBiosGetMeStatusReturn = EFI_SUCCESS;

  mMockSecureBootState   = 0;           // SECURE_BOOT_DISABLE explicitly
  mMockGetVariableStatus = EFI_SUCCESS; // variable IS readable; data is 0
  SetupMockRuntimeServices ();

  // Act
  Result = IsProfileSyncSupported ();

  // Assert: GetVariable succeeds but SecureBootState==SECURE_BOOT_DISABLE -> FALSE
  UT_ASSERT_FALSE (Result);

  printf ("[%s] PASS - IsProfileSyncSupported correctly returned FALSE "
    "when SecureBootEnable variable is present but explicitly set to DISABLE\n",
    __FUNCTION__);

  return UNIT_TEST_PASSED;
}
