/** @file
  Definition of the configuration variable for Nhlt Endpoints table.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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
#ifndef _NHLT_BT_CONFIGURATION_VARIABLE_H_
#define _NHLT_BT_CONFIGURATION_VARIABLE_H_

#define NHLT_BT_CONFIGURATION_VARIABLE_VERSION 1

#define NHLT_BT_CONFIGURATION_VARIABLE_NAME    L"NhltBtConfigurationVariable"

#define NHLT_BT_CONFIGURATION_VARIABLE_GUID    \
 { 0xB2364F9C, 0xB817, 0x4750, { 0xA5, 0xD2, 0x37, 0xA0, 0xD1, 0xDA, 0xF8, 0xDD } }

#pragma pack (1)

/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
**/
#define NHLT_BT_CONFIGURATION_VARIABLE_REVISION 1

typedef struct {
  UINT8  Revision;              ///< UEFI Variable Structure Revision
  UINT8  NhltBtActiveConfig;    ///< Enablement of NHLT for BT. 0 : Disabled 1 : BT drives I2S clock for HFP 2 : DSP drives I2S clock for HFP
} NHLT_BT_CONFIGURATION_VARIABLE;
#pragma pack ()

#endif /** _NHLT_BT_CONFIGURATION_VARIABLE_H_ **/
