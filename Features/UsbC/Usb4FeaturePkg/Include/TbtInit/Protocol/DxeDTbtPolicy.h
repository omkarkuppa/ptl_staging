/** @file
  TBT DXE Policy

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

@par Specification
**/

#ifndef _DXE_DTBT_POLICY_PROTOCOL_H_
#define _DXE_DTBT_POLICY_PROTOCOL_H_

#include <DxeTbtCommonDefinition.h>
#include <DTbtDefinition.h>

#pragma pack(push, 1)

#define DXE_DTBT_POLICY_REVISION 2

//
// TBT Common Data Structure
//
typedef struct _TBT_COMMON_CONFIG{
  /**
    Tbt Vtd Base Security for disable BME from TBT tree.
    <b>0: Disabled</b>, 1: Enabled
  **/
  UINT8   TbtVtdBaseSecurity;
  UINT8   Reserved[3];       ///< Reserved bits
} TBT_COMMON_CONFIG;

//
// DTBT Generic Data Structure
//
typedef struct _DXE_DTBT_GENERIC_CONFIG {
  UINT32   DTbtRtd3 : 1;
  UINT32   DTbtRtd3OffDelay : 16;
  UINT32   DTbtRtd3ClkReq : 1;
  UINT32   DTbtRtd3ClkReqDelay : 16;
  UINT32   Reserved : 30;
} DXE_DTBT_GENERIC_CONFIG;

/**
 DTBT DXE configuration\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct _DXE_DTBT_POLICY_PROTOCOL {
  TBT_COMMON_CONFIG       TbtCommonConfig;                                  ///< Tbt Common Information
  DXE_DTBT_GENERIC_CONFIG DTbtGenericConfig;                                ///< dTbt Generic Information
  TBT_RESOURCE_CONFIG     DTbtResourceConfig[MAX_DTBT_CONTROLLER_NUMBER];   ///< dTbt Resource Configuration
} DXE_DTBT_POLICY_PROTOCOL;

#pragma pack(pop)

#endif
