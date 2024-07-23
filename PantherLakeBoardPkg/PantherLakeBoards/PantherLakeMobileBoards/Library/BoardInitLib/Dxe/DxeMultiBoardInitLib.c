/** @file
  DXE Multi-Board Initilialization Library

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
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MultiBoardInitSupportLib.h>
#include <PlatformBoardId.h>
#include <Library/PcdLib.h>

EFI_STATUS
EFIAPI
PtlBoardInitAfterPciEnumeration (
  VOID
  );

EFI_STATUS
EFIAPI
PtlBoardInitReadyToBoot (
  VOID
  );

EFI_STATUS
EFIAPI
PtlBoardInitEndOfFirmware (
  VOID
  );

BOARD_NOTIFICATION_INIT_FUNC mPtlBoardDxeInitFunc = {
  PtlBoardInitAfterPciEnumeration,
  PtlBoardInitReadyToBoot,
  PtlBoardInitEndOfFirmware
};

/**
  The constructor determines which board init functions should be registered.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
DxePtlMultiBoardInitLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return RegisterBoardNotificationInit (&mPtlBoardDxeInitFunc);
}
