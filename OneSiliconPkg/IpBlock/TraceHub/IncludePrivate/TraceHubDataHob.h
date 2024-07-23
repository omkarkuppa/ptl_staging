/** @file
  The GUID definition for TraceHub Data Hob

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#ifndef _TRACEHUB_DATA_HOB_H_
#define _TRACEHUB_DATA_HOB_H_

#include <Base.h>

extern EFI_GUID gTraceHubDataHobGuid;

#pragma pack (push,1)

/**
  Trace Hub Data Hob.
**/
typedef struct {
  EFI_HOB_GUID_TYPE        EfiHobGuidType;
  UINT64                   TraceHubMemBase;
  UINT64                   TraceHubMemSize;
  UINT8                    TraceHubMode[3];
  UINT8                    TraceVerbosity;
  UINT32                   TraceHubFwBase;
} TRACEHUB_DATA_HOB;

#pragma pack (pop)
#endif
