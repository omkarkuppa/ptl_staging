/** @file
  Header file for Dxe VMD Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
#ifndef _DXE_VMD_INIT_LIB_H_
#define _DXE_VMD_INIT_LIB_H_

#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/VmdInfoLib.h>
#include <VmdInfoHob.h>
#include <Library/UefiBootServicesTableLib.h>

/**
  Update VMD NVS area.

  @param[in]  VmdInfoHob              Pointer to VMD_INFO_HOB
**/
VOID
UpdateVmdNvs (
  VMD_INFO_HOB                    *VmdInfoHob
  );

/**
  Initialize VMD Nvs Area operation region.

  @retval EFI_SUCCESS    initialized successfully
  @retval EFI_NOT_FOUND  Nvs Area operation region is not found
**/
EFI_STATUS
EFIAPI
PatchVmdNvsAreaAddress (
  VOID
  );

/**
  Install VMD Global NVS protocol

  @param[in]  ImageHandle  Image Handle.
**/
VOID
EFIAPI
InstallVmdNvsProtocol (
  IN EFI_HANDLE         ImageHandle
  );


/**
  Initializes VMD NVS area.
**/
VOID
VmdNvsInit (
  VOID
  );

#endif /* _DXE_VMD_INIT_LIB_H_ */
