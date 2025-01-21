/** @file
  Platform Board Sanitize Dxe driver supports Platform and OEM specific sanitization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include "PsBoardOemDxe.h"

//
// PS Board OEM Protocol
//
GLOBAL_REMOVE_IF_UNREFERENCED PS_BOARD_OEM_PROTOCOL mPsBoardOemProtocol = {
  PS_BOARD_OEM_PROTOCOL_REVISION,
  PsNvmGoldenConfig,
  PsOemCustomActionBitMask16,
  PsPostClearAllBiosNvmVariableRegionAction
};

/**
  Entry Point function for Platform Sanitize Board/OEM specific

  @param[in]  ImageHandle  Not used.
  @param[in]  SystemTable  Global system service table.

  @retval     EFI_SUCCESS  This constructor always return EFI_SUCCESS.
              It will ASSERT on errors.
**/
EFI_STATUS
EFIAPI
PsBoardOemEntryPoint (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS       Status;

  DEBUG ((DEBUG_INFO, "PS: Board, OEM Sanitize service EntryPoint.\n"));

  //
  // Install Protocol for platform/OEM sanitize services
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                &ImageHandle,
                &gPsBoardOemProtocolGuid,
                &mPsBoardOemProtocol,
                NULL
                );
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "PS: InstallProtocol for Platform and OEM Specific Erase Failed.\n"));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  DEBUG ((DEBUG_INFO, "PS: Board specific and OEM Custom Sanitize services Installed, Exit.\n"));
  return Status;
}
