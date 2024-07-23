/** @file
  This file defines the PCH NVS Area Protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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

#ifndef _PCH_NVS_AREA_H_
#define _PCH_NVS_AREA_H_

//
// PCH NVS Area definition
//
#include <Pch/IncludePrivate/PchNvsAreaDef.h>

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                         gPchNvsAreaProtocolGuid;

/**
  This protocol is used to sync PCH information from POST to runtime ASL.
  This protocol exposes the pointer of PCH NVS Area only. Please refer to
  ASL definition for PCH NVS AREA.
**/
typedef struct {
  PCH_NVS_AREA                          *Area;
} PCH_NVS_AREA_PROTOCOL;

#endif // _PCH_NVS_AREA_H_
