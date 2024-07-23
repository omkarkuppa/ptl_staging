/** @file
  This file defines the MIPI Cam NVS Area Protocol.

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


#ifndef MIPICAM_NVS_AREA_H
#define MIPICAM_NVS_AREA_H

//
// MIPI CAM NVS Area definition
//
#include <MipiCamNvsAreaDef.h>

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                         gMipiCamNvsAreaProtocolGuid;

/**
  This protocol is used to sync MIPICAM information from POST to runtime ASL.
  This protocol exposes the pointer of MIPICAM NVS Area only. Please refer to
  ASL definition for MIPICAM NVS AREA.
**/
typedef struct {
  MIPICAM_NVS_AREA                          *Area;
} MIPICAM_NVS_AREA_PROTOCOL;

#endif // MIPICAM_NVS_AREA_H

