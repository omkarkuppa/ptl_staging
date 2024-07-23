/** @file
 This file defines the HOB data structure to indicate that Fast Boot function
 is enabled in PEI phase, which may be used by some DXE phase drivers as well.

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

#ifndef _FAST_BOOT_FUNCTION_ENABLED_HOB_H_
#define _FAST_BOOT_FUNCTION_ENABLED_HOB_H_

#include <Pi/PiHob.h>

typedef struct _FAST_BOOT_FUNCTION_ENABLED_HOB {
  EFI_HOB_GUID_TYPE             Header;
  BOOLEAN                       FastBootEnabled;
} FAST_BOOT_FUNCTION_ENABLED_HOB;

#endif // _FAST_BOOT_FUNCTION_ENABLED_HOB_H_
