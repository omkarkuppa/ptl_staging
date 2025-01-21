/** @file
  This file defines the PCH NVS Area Protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#ifndef _BIOSGUARD_NVS_AREA_H_
#define _BIOSGUARD_NVS_AREA_H_

//
// BIOS Guard NVS Area definition
//
#include <IncludePrivate/BiosGuardNvsAreaDef.h>

/**
  BIOS Guard NVS Area Revisions
  <b>Revision 1</b>:
  - Initial version.
**/
#define BIOSGUARD_NVS_AREA_REVISION 1

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                         gBiosGuardNvsAreaProtocolGuid;

/**
  This protocol is used to sync BIOS Guard information from POST to runtime ASL.
  This protocol exposes the pointer of BIOS GUARD NVS Area only. Please refer to
  ASL definition for BIOS GUARD NVS Area.
**/
typedef struct {
  BIOSGUARD_NVS_AREA                          *Area;
} BIOSGUARD_NVS_AREA_PROTOCOL;

#endif // _BIOSGUARD_NVS_AREA_H_
