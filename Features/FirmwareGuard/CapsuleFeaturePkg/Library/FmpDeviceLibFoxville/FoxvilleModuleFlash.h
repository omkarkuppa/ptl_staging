/** @file
  Internal file to support the Foxville module in flash operation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#ifndef __FOXVILLE_MODULE_FLASH_H__
#define __FOXVILLE_MODULE_FLASH_H__

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Protocol/PciIo.h>
//
// Self-Module Foundation.
//
#include <Defines/FoxvilleModule.h>

/**
  Get the module component information from the flash.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]   Module            Module type.
  @param[out]  AddressPtr        Pointer to indicate the address of the module from flash.
  @param[out]  SizePtr           Pointer to indicate the size of the module from flash.

  @retval  EFI_SUCCESS            Succeed to get the module information from flash.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_UNSUPPORTED        Unsupported module type.
  @retval  Others                 Failed to get the module information from flash.

**/
EFI_STATUS
EFIAPI
GetModuleInfoFromFlash (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN     FOXVILLE_MODULE      Module,
     OUT UINTN                *AddressPtr,
     OUT UINTN                *SizePtr
  );

#endif
