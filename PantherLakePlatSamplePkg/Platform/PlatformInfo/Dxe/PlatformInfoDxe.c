/** @file
  Platform Info driver to install protocol for DXE and
  creat a PlatformInfo varible.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#include "PlatformInfoDxe.h"

/**
  Entry point for the driver.

  This routine gets info from the PlatformInfo and produces a protocol
  to be consumed by DXE drivers to identify platforms.

  @param[in]  ImageHandle  Image Handle.
  @param[in]  SystemTable  EFI System Table.

  @retval     EFI_SUCCESS  Function has completed successfully.
  @retval     Others       All other error conditions encountered result in an ASSERT.
**/
EFI_STATUS
EFIAPI
PlatformInfoInit (
  IN EFI_HANDLE               ImageHandle,
  IN EFI_SYSTEM_TABLE         *SystemTable
  )
{
  EFI_STATUS                             Status;
  DXE_BOARD_CONFIG_INIT                  BoardConfigInit;

  //
  // Init Board Config.
  //
  BoardConfigInit = (DXE_BOARD_CONFIG_INIT) (UINTN) PcdGet64 (PcdFuncBoardConfigInit);
  ASSERT (BoardConfigInit != NULL);
  if (BoardConfigInit == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = BoardConfigInit (NULL);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
