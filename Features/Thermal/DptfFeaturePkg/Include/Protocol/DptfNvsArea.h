/** @file
  This file defines the DPTF NVS Area Protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _DPTF_NVS_AREA_H_
#define _DPTF_NVS_AREA_H_

//
// CPU NVS Area definition
//
#include <DptfNvsAreaDef.h>

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                         gDptfNvsAreaProtocolGuid;

/**
  This protocol is used to sync DPTF information from POST to runtime ASL.
  This protocol exposes the pointer of DPTF NVS Area only. Please refer to
  ASL definition for DPTF NVS AREA.
**/
typedef struct {
  DPTF_NVS_AREA                          *Area;
} DPTF_NVS_AREA_PROTOCOL;

#endif // _DPTF_NVS_AREA_H_
