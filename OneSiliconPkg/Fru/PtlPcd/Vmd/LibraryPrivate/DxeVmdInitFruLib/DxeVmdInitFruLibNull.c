/** @file
  DXE Null Library for Initializing VMD FRU

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
#include <Library/DxeVmdInitLib.h>

/**
  Update VMD NVS area.

  @param[in]  VmdInfoHob              Pointer to VMD_INFO_HOB

  @retval VOID
**/
VOID
UpdateVmdNvs (
  VMD_INFO_HOB                    *VmdInfoHob
  )
{
  return;
}

/**
  Initialize VMD Nvs Area operation region.

  @retval EFI_SUCCESS          - The function completed successfully.
**/
EFI_STATUS
EFIAPI
PatchVmdNvsAreaAddress (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  Install VMD Global NVS protocol

  @param[in]  ImageHandle  Image Handle.

  @retval VOID
**/
VOID
EFIAPI
InstallVmdNvsProtocol (
  IN EFI_HANDLE         ImageHandle
  )
{
  return;
}
