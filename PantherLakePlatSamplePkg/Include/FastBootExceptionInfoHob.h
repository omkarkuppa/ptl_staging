/** @file

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

#ifndef __FAST_BOOT_EXCEPTION_INFO_HOB_H__
#define __FAST_BOOT_EXCEPTION_INFO_HOB_H__

#include <Pi/PiHob.h>
#include <FastBootDataDef.h>

typedef struct _FAST_BOOT_EXCEPTION_INFO_HOB {
  EFI_HOB_GUID_TYPE             Header;
  EFI_GUID                      GuidName;
  FAST_BOOT_EXCEPTION_TYPE      FbExceptionType;
  FAST_BOOT_EXCEPTION_CATEGORY  FbExceptionCategory;
} FAST_BOOT_EXCEPTION_INFO_HOB;

#endif // __FAST_BOOT_EXCEPTION_INFO_HOB_H__
