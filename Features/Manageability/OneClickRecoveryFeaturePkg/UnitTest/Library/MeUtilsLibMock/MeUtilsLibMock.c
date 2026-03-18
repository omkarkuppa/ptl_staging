/** @file
  Mock MeUtilsLib for One Click Recovery Unit Testing.

  Provides a controllable stub implementation of MeBiosGetMeStatus() used by
  the WifiProfileSync production code under test. The test case sets mMeStatus
  to simulate the FWSTS1 register value returned by ME firmware, allowing
  ME_STATUS_IS_ME_FW_BOOT_OPTIONS_PRESENT() to be exercised without hardware.

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

#include <Uefi.h>
#include <MeState.h>
#include <Library/DebugLib.h>

///
/// Controllable ME firmware status register value returned by the mock.
///
/// Set ME_FW_BOOT_OPTIONS_PRESENT (bit 8, value 0x100) to simulate an active
/// AMT boot option in FWSTS1 bit 24:
///   mMeStatus = ME_FW_BOOT_OPTIONS_PRESENT;   // boot option present
///   mMeStatus = 0;                             // no boot option (default)
///
UINT32  mMeStatus = 0;

///
/// Controllable return code for MeBiosGetMeStatus().
/// Set to a non-EFI_SUCCESS value to simulate HECI read failure.
///
EFI_STATUS  mMeBiosGetMeStatusReturn = EFI_SUCCESS;

/**
  Return the mock ME firmware status value to the caller.

  This mock replaces the real MeBiosGetMeStatus() which reads the HECI
  FWSTS1 register. Test cases control the simulated value via mMeStatus
  and the return code via mMeBiosGetMeStatusReturn.

  @param[out] MeStatus   Receives the mock ME status value.

  @retval EFI_SUCCESS            mMeStatus copied into *MeStatus.
  @retval EFI_INVALID_PARAMETER  MeStatus is NULL.
  @retval Other                  Value of mMeBiosGetMeStatusReturn when set to failure.
**/
EFI_STATUS
EFIAPI
MeBiosGetMeStatus (
  OUT UINT32  *MeStatus
  )
{
  if (MeStatus == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (EFI_ERROR (mMeBiosGetMeStatusReturn)) {
    DEBUG ((DEBUG_INFO, "[%a] Mock returning failure status %r\n",
      __FUNCTION__, mMeBiosGetMeStatusReturn));
    return mMeBiosGetMeStatusReturn;
  }

  *MeStatus = mMeStatus;
  DEBUG ((DEBUG_INFO, "[%a] Mock returning MeStatus=0x%x (BootOptionsPresent=%a)\n",
    __FUNCTION__, mMeStatus,
    ME_STATUS_IS_ME_FW_BOOT_OPTIONS_PRESENT (mMeStatus) ? "TRUE" : "FALSE"));

  return EFI_SUCCESS;
}
