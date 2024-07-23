/** @file
  SoundWire Device Topology Configuration EFI variable
  
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

@par Specification Reference:
**/

#ifndef _SNDW_DEV_TOPOLOGY_CONFIGURATION_VARIABLE_H_
#define _SNDW_DEV_TOPOLOGY_CONFIGURATION_VARIABLE_H_

#define SNDW_DEV_TOPOLOGY_MAX_HDAS_PATH_LENGTH  20

#define SNDW_DEV_TOPOLOGY_CONFIGURATION_VARIABLE_VERSION 1

#define SNDW_DEV_TOPOLOGY_CONFIGURATION_VARIABLE_NAME  L"SndwDevTopologyConfigurationVariable"

#define SNDW_DEV_TOPOLOGY_CONFIGURATION_VARIABLE_GUID \
  { 0x7975600E, 0x4C1B, 0x4E52, { 0xB7, 0x5E, 0x1D, 0xED, 0x70, 0x34, 0x18, 0x13 }}

/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>: - Initial version.
  <b>Revision 2</b>: - Add separate options for enabling DisCo for BT and iDisplay
**/
#define SNDW_DEV_TOPOLOGY_CONFIGURATION_VARIABLE_REVISION  2

#pragma pack (1)
typedef struct {
  UINT8    Version;
  UINT8    SndwDevTopologyConfigurationNumber;
  UINT8    AcxSndwDevTopologyBtNdaiEnabled;
  UINT8    AcxSndwDevTopologyIDispNdaiEnabled;
} SNDW_DEV_TOPOLOGY_CONFIGURATION_VARIABLE;
#pragma pack ()

#endif
