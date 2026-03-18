/** @file
  Unit test case for the IsProfileSyncSupported function, verifying that
  the AMT boot option check in FWSTS1 bit 24 is correctly enforced.

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
#ifndef __IS_PROFILE_SYNC_SUPPORTED_H__
#define __IS_PROFILE_SYNC_SUPPORTED_H__

#include <Uefi.h>
#include <Library/UnitTestLib.h>

/**
  Verify that IsProfileSyncSupported returns FALSE when no AMT boot option
  is set in FWSTS1 bit 24 (MeBootOptionsPresent = 0).

  This validates the new guard added per the Wi-Fi co-existence requirement:
  UEFI shall only attempt Wi-Fi profile sync when an AMT boot option is active.

  @retval UNIT_TEST_PASSED              Test passed.
  @retval UNIT_TEST_ERROR_TEST_FAILED   Test failed.
**/
UNIT_TEST_STATUS
EFIAPI
TestIsProfileSyncSupported_AmtBootOptionNotPresent (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Verify that IsProfileSyncSupported returns FALSE when the AMT boot option
  is present but Secure Boot is disabled (or the variable is inaccessible).

  @retval UNIT_TEST_PASSED              Test passed.
  @retval UNIT_TEST_ERROR_TEST_FAILED   Test failed.
**/
UNIT_TEST_STATUS
EFIAPI
TestIsProfileSyncSupported_SecureBootDisabled (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Verify that IsProfileSyncSupported returns FALSE when the AMT boot option
  is present and Secure Boot is enabled, but Boot Guard is not supported
  (null BootGuardLib always returns FALSE for IsBootGuardSupported).

  @retval UNIT_TEST_PASSED              Test passed.
  @retval UNIT_TEST_ERROR_TEST_FAILED   Test failed.
**/
UNIT_TEST_STATUS
EFIAPI
TestIsProfileSyncSupported_SecureBootEnabled_BootGuardNotSupported (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Verify that IsProfileSyncSupported returns FALSE when MeBiosGetMeStatus()
  itself returns an error (e.g. EFI_DEVICE_ERROR), covering the EFI_ERROR(Status)
  branch of the first guard condition.

  @retval UNIT_TEST_PASSED              Test passed.
  @retval UNIT_TEST_ERROR_TEST_FAILED   Test failed.
**/
UNIT_TEST_STATUS
EFIAPI
TestIsProfileSyncSupported_MeBiosGetMeStatusFails (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Verify that IsProfileSyncSupported returns FALSE when mMeStatus has other ME
  state bits set (ME_FW_INIT_COMPLETE = 0x80) but NOT the boot-option bit
  (ME_FW_BOOT_OPTIONS_PRESENT = 0x100), confirming the macro is bit-precise.

  @retval UNIT_TEST_PASSED              Test passed.
  @retval UNIT_TEST_ERROR_TEST_FAILED   Test failed.
**/
UNIT_TEST_STATUS
EFIAPI
TestIsProfileSyncSupported_MeStatusNoBootOptionBit (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Verify that IsProfileSyncSupported advances past the boot-option guard when
  mMeStatus has both ME_FW_INIT_COMPLETE (0x80) and ME_FW_BOOT_OPTIONS_PRESENT
  (0x100) set (combined 0x180), then returns FALSE due to absent Secure Boot
  variable. Confirms the macro works correctly with combined status values.

  @retval UNIT_TEST_PASSED              Test passed.
  @retval UNIT_TEST_ERROR_TEST_FAILED   Test failed.
**/
UNIT_TEST_STATUS
EFIAPI
TestIsProfileSyncSupported_MeStatusWithOtherBitsSet (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Verify that IsProfileSyncSupported returns FALSE when the Secure Boot variable
  is readable (GetVariable succeeds) but explicitly contains SECURE_BOOT_DISABLE
  (0), exercising the (SecureBootState == SECURE_BOOT_DISABLE) clause separately
  from the EFI_NOT_FOUND path tested by TestIsProfileSyncSupported_SecureBootDisabled.

  @retval UNIT_TEST_PASSED              Test passed.
  @retval UNIT_TEST_ERROR_TEST_FAILED   Test failed.
**/
UNIT_TEST_STATUS
EFIAPI
TestIsProfileSyncSupported_SecureBootExplicitlyDisabled (
  IN UNIT_TEST_CONTEXT  Context
  );

#endif // __IS_PROFILE_SYNC_SUPPORTED_H__
