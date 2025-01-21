/** @file
  This file defines the Platform NVS Area Protocol.

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

#ifndef _PLATFORM_NVS_AREA_H_
#define _PLATFORM_NVS_AREA_H_

//
// Platform NVS Area definition
//
#include <PlatformNvsAreaDef.h>

//
// Includes
//
#define PLATFORM_NVS_DEVICE_ENABLE 1
#define PLATFORM_NVS_DEVICE_DISABLE 0

//
// Forward reference for pure ANSI compatibility
//
typedef struct _PLATFORM_NVS_AREA_PROTOCOL PLATFORM_NVS_AREA_PROTOCOL;

//
// Platform NVS Area Protocol GUID
//
#define PLATFORM_NVS_AREA_PROTOCOL_GUID \
{ 0xc77ae556,0x40a3, 0x41c0, 0xac, 0xe6, 0x71, 0x43, 0x8c, 0x60, 0xf8, 0x71 }

#define PLATFORM_NVS_AREA_REVISION_1       1

//
// Platform NVS Area Protocol
//
typedef struct _PLATFORM_NVS_AREA_PROTOCOL {
  PLATFORM_NVS_AREA     *Area;
} PLATFORM_NVS_AREA_PROTOCOL;

#endif // _PLATFORM_NVS_AREA_H_
