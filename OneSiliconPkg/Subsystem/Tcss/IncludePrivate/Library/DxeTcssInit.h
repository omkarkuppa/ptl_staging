/** @file
  Header file for TCSS North Lib.

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
#ifndef _DXE_TCSS_INIT_LIB_H_
#define _DXE_TCSS_INIT_LIB_H_

#include <Protocol/TcssNvsArea.h>

/**
  DXE TCSS Initialization

**/
VOID
DxeTcssInit (
  VOID
  );

/**
  Install TCSS NVS protocol
  @param[in]  ImageHandle  Image Handle.
**/
VOID
InstallTcssNvsProtocol (
  IN EFI_HANDLE         ImageHandle
  );

/**
  Initialize TCSS Nvs Area operation region.
  @retval EFI_SUCCESS    initialized successfully
  @retval EFI_NOT_FOUND  Nvs Area operation region is not found
**/
EFI_STATUS
EFIAPI
PatchTcssNvsAreaAddress (
  VOID
  );

/**
  Update TCSS NVS AREA tables
**/
VOID
UpdateTcssNVS (
  VOID
  );

/**
  This function prints TCSS NVS AREA tables
**/
VOID
EFIAPI
TcssPrintNvs (
  IN TCSS_NVS_AREA_PROTOCOL *NvsArea
  );

/**
This function performs Itbt PCI initialization before EndOfDxe.

**/
VOID
TcssIbtPcieConfigPciEnumComplete (
  VOID
  );
#endif
