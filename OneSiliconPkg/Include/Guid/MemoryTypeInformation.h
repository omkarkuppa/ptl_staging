/** @file
  This file defines:
  * Memory Type Information GUID for HOB and Variable.
  * Memory Type Information Variable Name.
  * Memory Type Information GUID HOB data structure.

  The memory type information HOB and variable can
  be used to store the information for each memory type in Variable or HOB.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2006 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#ifndef __MEMORY_TYPE_INFORMATION_GUID_H__
#define __MEMORY_TYPE_INFORMATION_GUID_H__

#define EFI_MEMORY_TYPE_INFORMATION_GUID \
  { 0x4c19049f,0x4137,0x4dd3, { 0x9c,0x10,0x8b,0x97,0xa8,0x3f,0xfd,0xfa } }

#define EFI_MEMORY_TYPE_INFORMATION_VARIABLE_NAME L"MemoryTypeInformation"

extern EFI_GUID gEfiMemoryTypeInformationGuid;

typedef struct {
  UINT32  Type;             ///< EFI memory type defined in UEFI specification.
  UINT32  NumberOfPages;    ///< The pages of this type memory.
} EFI_MEMORY_TYPE_INFORMATION;

#endif
