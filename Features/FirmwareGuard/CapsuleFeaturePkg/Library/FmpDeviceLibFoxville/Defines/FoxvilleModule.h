/** @file
  Definition of Foxville firmware module.

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

#ifndef __FOXVILLE_MODULE_H__
#define __FOXVILLE_MODULE_H__

//
// EDK-II Foundation.
//
#include <Uefi.h>

#define MODULE_SHADOW_RAM_SIZE  SIZE_4KB

typedef UINT16 FOXVILLE_MODULE;

#define MODULE_SHADOW_RAM      0x0000
#define MODULE_SECURE          0x0001
#define MODULE_FREE_PROV_AREA  0x0002

#endif
