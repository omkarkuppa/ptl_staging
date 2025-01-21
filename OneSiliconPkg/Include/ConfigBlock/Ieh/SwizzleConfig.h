/** @file
  Config block for interrupt swizzle functionality configuration

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

@par Specification
**/

#ifndef _SWIZZLE_CONFIG_H_
#define _SWIZZLE_CONFIG_H_

#include <ConfigBlock.h>

#define SWIZZLE_CONFIG_REVISION  1
extern EFI_GUID  gSwizzleConfigGuid;

#pragma pack (push,1)

typedef enum {
  SwizzleBy0 = 0,
  SwizzleBy1,
  SwizzleBy2,
  SwizzleBy3,
  SwizzleMax
} SWIZZLE_VALUE;

#define MAX_DEVICE_NUMBER  32   // Max device supported number

/**
  This config block describes how to configure Interrupt Swizzling functionality
  in EAH.
  INTx swizzling works on offset basis.
  If swizzle by 0 then INTA becomes INTA
  If swizzle by 1 then INTA becomes INTB
  If swizzle by 2 then INTA becomes INTC
  If swizzle by 3 then INTA becomes INTD
**/
typedef struct {
  CONFIG_BLOCK_HEADER    Header;                     ///< Config Block Header
  SWIZZLE_VALUE          Swizzle[MAX_DEVICE_NUMBER]; // Per device swizzle configuration
} SWIZZLE_CONFIG;

#pragma pack (pop)

#endif // _SWIZZLE_CONFIG_H_
