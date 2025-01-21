/** @file
  Platform hook to check the boot state.

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

@par Specification Reference:
**/

#include <Uefi.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <BootStateLib.h>
#include <Library/PlatformBootStateHookLib.h>

BOOLEAN  mIsSetupVarInitialed = FALSE;

/**
  Check if this is the first boot.

  @retval FALSE  It's not the first boot.
  @retval TRUE   It's the first boot.
**/
BOOLEAN
EFIAPI
IsFirstBoot (
  VOID
  )
{
  //
  // If BootState variable is present, then it's not the first boot.
  //
  if (IsBootStatePresent ()) {
    return FALSE;
  }

  //
  // If setup variable had been initialed, then it's not the first boot.
  //
  if (mIsSetupVarInitialed) {
    return FALSE;
  }

  return TRUE;
}

/**
  Constructor for the Platform DXE Boot State Hook library.

  @param ImageHandle  The firmware allocated handle for the EFI image.
  @param SystemTable  A pointer to the EFI System Table.

  @retval EFI_SUCCESS  The constructor always returns EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
PlatformDxeBootStateHookLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  UINTN       VarSize;

  VarSize = 0;
  Status  = gRT->GetVariable (
                   L"InitSetupVariable",
                   &gSetupVariableGuid,
                   NULL,
                   &VarSize,
                   NULL
                   );
  if (Status == EFI_BUFFER_TOO_SMALL) {
    mIsSetupVarInitialed = TRUE;
  }

  return EFI_SUCCESS;
}
