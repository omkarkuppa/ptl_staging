/** @file
  Header file for DXE Host Bridge Init Lib.

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
#ifndef _DXE_HOST_BRIDGE_INIT_LIB_H_
#define _DXE_HOST_BRIDGE_INIT_LIB_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciSegmentLib.h>


/**
  Update Host Bridge NVS Area
**/
VOID
EFIAPI
UpdateHostBridgeNvs (
  VOID
  );

/**
  Initialize Host Bridge Nvs Area operation region.

  @retval EFI_SUCCESS    initialized successfully
  @retval EFI_NOT_FOUND  Nvs Area operation region is not found
**/
EFI_STATUS
EFIAPI
PatchHostBridgeNvsAreaAddress (
  VOID
  );

/**
  Install Host Bridge Global NVS protocol

  @param[in]  ImageHandle  Image Handle.
**/
VOID
EFIAPI
InstallHostBridgeNvsProtocol (
  IN EFI_HANDLE         ImageHandle
  );
#endif
