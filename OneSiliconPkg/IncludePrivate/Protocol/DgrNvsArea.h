/** @file
  This file defines the DGR NVS Area Protocol.

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

#ifndef _DGR_NVS_AREA_H_
#define _DGR_NVS_AREA_H_

//
// DGR NVS Area definition
//
#include <IncludePrivate/DgrNvsAreaDef.h>

/**
  DGR NVS Area Revisions
  <b>Revision 1</b>:
  - Initial version.
**/
#define DGR_NVS_AREA_REVISION 1

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                         gDgrNvsAreaProtocolGuid;

/**
  This protocol is used to sync DGR information from POST to runtime ASL.
  This protocol exposes the pointer of DGR NVS Area only. Please refer to
  ASL definition for DGR NVS Area.
**/
typedef struct {
  DGR_NVS_AREA                          *Area;
} DGR_NVS_AREA_PROTOCOL;

#endif // _DGR_NVS_AREA_H_
