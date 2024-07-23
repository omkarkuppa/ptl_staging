/** @file
  Definition of the configuration variable for Nhlt Endpoints table.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#ifndef _NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE_H_
#define _NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE_H_

#define NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE_VERSION 1

#define NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE_NAME  L"NhltEndpointsTableConfigurationVariable"

#define NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE_GUID \
 { 0xA1D89A3A, 0x4A90, 0x429D, { 0x43, 0x65, 0x1F, 0x64, 0xC3, 0xA2, 0x96, 0x14 } }

#define NHLT_ENDPOINTS_TABLE_MAX_CONFIGURATION_NUMBER 40

#pragma pack (1)

/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
**/
#define NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE_REVISION  1

typedef struct {
  UINT8  Version;
  /**
   <b>NHLT config enablement</b>
   0x0 : Disable;
   0x1 : Clock 38_4MHz, Format 24b/48kHz;
   0x2 : Clock 38_4MHz, Format 24b/96kHz;
  **/
  UINT8  NhltDmicMonoEnabled;
  UINT8  NhltDmicStereoEnabled;
  UINT8  NhltDmicQuadEnabled;
  UINT8  NhltI2sAlc274Enabled;
  /**
   <b>Nhlt config enablement:</b>
   0: Disable;
   1: Enable;
  **/
  UINT8  NhltDmicStereoVpEnabled;
  UINT8  NhltBluetoothEnabled;
} NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE;
#pragma pack ()

#endif
