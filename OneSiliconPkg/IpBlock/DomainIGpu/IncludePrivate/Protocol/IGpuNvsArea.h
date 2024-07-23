/** @file
  This file defines the IGPU NVS Area Protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef _IGPU_NVS_AREA_H_
#define _IGPU_NVS_AREA_H_

//
// Platform NVS Area definition
//
#include <IGpuNvsAreaDef.h>

//
// Forward reference for pure ANSI compatibility
//
typedef struct _IGPU_NVS_AREA_PROTOCOL IGPU_NVS_AREA_PROTOCOL;

///
/// Extern the GUID for protocol users.
///
extern EFI_GUID gIGpuNvsAreaProtocolGuid;

#define IGPU_NVS_AREA_REVISION       1

//
// Platform NVS Area Protocol
//
typedef struct _IGPU_NVS_AREA_PROTOCOL {
  IGPU_NVS_AREA     *Area;
} IGPU_NVS_AREA_PROTOCOL;

#endif // _IGPU_NVS_AREA_H_
