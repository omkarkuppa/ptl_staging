/** @file
  Description file for BootState Variable Information during Dxe Phase.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#include <Library/DebugLib.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <BootStateLib.h>

/**
  This function call Checks BootState variable is NULL or not.
  @retval FALSE  It's the first boot after reflashing.
  @retval TRUE   It's not the first boot after reflashing.
**/
BOOLEAN
EFIAPI
IsBootStatePresent (
  VOID
  )
{
  EFI_STATUS  Status;
  UINT8       *BootState;
  BOOLEAN     IsPresented;

  BootState   = NULL;
  IsPresented = FALSE;

  ///
  /// Get last Boot State Variable From NVRAM
  ///
  Status = GetVariable2 (
             BOOT_STATE_VARIABLE_NAME,
             &gBootStateGuid,
             (VOID **) &BootState,
             NULL
             );
  if (Status == EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "DxeIsBootStatePresent : BootState is present.\n"));
    IsPresented = TRUE;
  } else if (Status == EFI_NOT_FOUND) {
    DEBUG ((DEBUG_INFO, "DxeIsBootStatePresent : BootState is NOT present.\n"));
    IsPresented = FALSE;
  } else {
    DEBUG ((DEBUG_ERROR, "DxeIsBootStatePresent : Get variable failure.\n"));
    ASSERT_EFI_ERROR (Status);
    IsPresented = FALSE;
  }

  if (BootState != NULL) {
    FreePool (BootState);
  }

  return IsPresented;
}

/**
 This function call use to set bootstate variable.
 In DXE  phase BootState variable got set using setvariable().From onward it will treat as second boot.

 @param[in]      BootState              Set value for bootstate
**/
VOID
EFIAPI
SetBootState (
  VOID
)
{
  EFI_STATUS                        Status;
  BOOLEAN                           BootState;
  Status = gRT->SetVariable (
                  BOOT_STATE_VARIABLE_NAME,
                  &gBootStateGuid,
                  EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                  sizeof(BOOLEAN),
                  &BootState
                  );
  ASSERT_EFI_ERROR(Status);
  return;
}

/**
 Delete BootState variable to force next boot is FullConfiguration boot.
**/
VOID
EFIAPI
UnsetBootState (
  VOID
  )
{
  EFI_STATUS  Status;

  //
  // If BootState is not presented, no need to unset.
  //
  if (!IsBootStatePresent ()) {
    return;
  }

  Status = gRT->SetVariable (
                  BOOT_STATE_VARIABLE_NAME,
                  &gBootStateGuid,
                  0,
                  0,
                  NULL
                  );

  ASSERT_EFI_ERROR(Status);

  return;
}
