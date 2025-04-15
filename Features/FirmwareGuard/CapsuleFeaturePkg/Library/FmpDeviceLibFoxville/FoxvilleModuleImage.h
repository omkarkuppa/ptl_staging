/** @file
  Internal file to support the Foxville module in image operation.

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

#ifndef __FOXVILLE_MODULE_IMAGE_H__
#define __FOXVILLE_MODULE_IMAGE_H__

//
// EDK-II Foundation.
//
#include <Uefi.h>
//
// Self-Module Foundation.
//
#include "Defines/FoxvilleModule.h"

/**
  Get the module component from the buffer.

  @param[in]   Module            Module type.
  @param[in]   BufferPtr         Pointer to the buffer.
  @param[in]   BufferSize        Size of the buffer.
  @param[out]  ModuleBufferPtr   Pointer to the module buffer.
  @param[out]  ModuleSizePtr     Pointer to the size of the module buffer.

  @retval  EFI_SUCCESS            Succeed to get the module from buffer.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_UNSUPPORTED        Unsupported module type.
  @retval  Others                 Failed to get the module from buffer.

**/
EFI_STATUS
EFIAPI
GetModuleFromBuffer (
  IN     FOXVILLE_MODULE  Module,
  IN     VOID             *BufferPtr,
  IN     UINTN            BufferSize,
     OUT VOID             **ModuleBufferPtr,
     OUT UINTN            *ModuleSizePtr
  );

#endif
