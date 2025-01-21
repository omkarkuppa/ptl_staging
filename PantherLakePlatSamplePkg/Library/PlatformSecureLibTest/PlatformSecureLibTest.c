/** @file
  Provides a secure platform-specific method to determine whether Secure Boot option is visible.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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

#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeAmtSupportLib.h>
#include <Protocol/SmmBase2.h>

BOOLEAN    mSecurityPhysicalPresent;
BOOLEAN    mInSmm = FALSE;

/**
  Constructor function for Platform Security library.

  @param[in]  ImageHandle   The firmware allocated handle for the EFI image.
  @param[in]  SystemTable   A pointer to the EFI System Table.

  @retval  EFI_SUCCESS  When this function completed.

**/
EFI_STATUS
EFIAPI
PlatformSecureLibTestConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                Status;
  EFI_SMM_BASE2_PROTOCOL    *SmmBase2;

  //
  // Detect whether BIOS CFG Jumper is set in configuration mode.
  // Setting means platform is operated by a physical present user.
  //
  mSecurityPhysicalPresent = PcdGetBool (PcdPhysicalPresent);

  Status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, (VOID **) &SmmBase2);
  if (!EFI_ERROR (Status)) {
    Status = SmmBase2->InSmm (SmmBase2, &mInSmm);
  }

  return EFI_SUCCESS;
}

/**
  This function provides a platform-specific method to detect whether the platform
  is operating by a physically present user.

  Programmatic changing of platform security policy (such as disable Secure Boot,
  or switch between Standard/Custom Secure Boot mode) MUST NOT be possible during
  Boot Services or after exiting EFI Boot Services. Only a physically present user
  is allowed to perform these operations.

  NOTE THAT: This function cannot depend on any EFI Variable Service since they are
  not available when this function is called in AuthenticateVariable driver.

  @retval  TRUE       The platform is operated by a physically present user.
  @retval  FALSE      The platform is NOT operated by a physically present user.

**/
BOOLEAN
EFIAPI
UserPhysicalPresent (
  VOID
  )
{
  if (mInSmm) {
    return mSecurityPhysicalPresent;
  }

  if (!mSecurityPhysicalPresent) {
    return FALSE;
  }

  //
  // If physical present is true, the next step is to check remote session connection.
  //
  if (AmtIsRemoteSessionEnabled ()) {
    return FALSE;
  } else {
    return TRUE;
  }
}
