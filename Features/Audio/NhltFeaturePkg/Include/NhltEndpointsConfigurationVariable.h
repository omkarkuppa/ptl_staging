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
  <b>Revision 2</b>:  - Restructure so the NHLT for BT is second byte.
**/
#define NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE_REVISION  2

typedef struct {
  UINT8  Revision;
  UINT8  NhltBluetoothEnabled;      ///< Enablement of NHLT for BT. 0 : Disabled 1 : BT drives I2S clock for HFP 2 : DSP drives I2S clock for HFP
  UINT8  NhltDmicMonoEnabled;       ///< Enablement of NHLT for DMIC Mono. 0 : Disabled 1 : Clock 38_4MHz, Format 24b/48kHz 2 : Clock 38_4MHz, Format 24b/96kHz
  UINT8  NhltDmicStereoEnabled;     ///< Enablement of NHLT for DMIC Stereo. 0 : Disabled 1 : Clock 38_4MHz, Format 24b/48kHz 2 : Clock 38_4MHz, Format 24b/96kHz
  UINT8  NhltDmicQuadEnabled;       ///< Enablement of NHLT for DMIC Quad. 0 : Disabled 1 : Clock 38_4MHz, Format 24b/48kHz 2 : Clock 38_4MHz, Format 24b/96kHz
  UINT8  NhltI2sAlc274Enabled;      ///< Enablement of NHLT for Alc274. 0 : Disabled 1 : Clock 38_4MHz, Format 24b/48kHz 2 : Clock 38_4MHz, Format 24b/96kHz
  UINT8  NhltDmicStereoVpEnabled;   ///< Enablement of NHLT for DMIC on VP platform. 0 : Disabled 1 : Enabled
} NHLT_ENDPOINTS_TABLE_CONFIGURATION_VARIABLE;
#pragma pack ()

#endif
