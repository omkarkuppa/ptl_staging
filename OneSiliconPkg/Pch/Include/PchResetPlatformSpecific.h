/** @file
  PCH Reset Platform Specific definitions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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
#ifndef _PCH_RESET_PLATFORM_SPECIFIC_H_
#define _PCH_RESET_PLATFORM_SPECIFIC_H_

#define PCH_PLATFORM_SPECIFIC_RESET_STRING   L"PCH_RESET"
#define PCH_RESET_DATA_STRING_MAX_LENGTH     (sizeof (PCH_PLATFORM_SPECIFIC_RESET_STRING) / sizeof (UINT16))

extern EFI_GUID gPchGlobalResetGuid;

typedef struct _RESET_DATA {
  CHAR16   Description[PCH_RESET_DATA_STRING_MAX_LENGTH];
  EFI_GUID Guid;
} PCH_RESET_DATA;

#endif // _PCH_RESET_PLATFORM_SPECIFIC_H_

