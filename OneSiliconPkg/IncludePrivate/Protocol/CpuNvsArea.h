/** @file
  This file defines the CPU NVS Area Protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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

#ifndef _CPU_NVS_AREA_H_
#define _CPU_NVS_AREA_H_

//
// CPU NVS Area definition
//
#include <CpuNvsAreaDef.h>

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                         gCpuNvsAreaProtocolGuid;

/**
  This protocol is used to sync CPU information from POST to runtime ASL.
  This protocol exposes the pointer of CPU NVS Area only. Please refer to
  ASL definition for CPU NVS AREA.
**/
typedef struct {
  CPU_NVS_AREA                          *Area;
} CPU_NVS_AREA_PROTOCOL;

#endif // _CPU_NVS_AREA_H_
