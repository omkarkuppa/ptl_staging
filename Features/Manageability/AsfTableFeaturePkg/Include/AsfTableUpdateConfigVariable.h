/** @file
  Definition of the configuration variable for AMT ASF Table Update feature.

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

#ifndef _ASF_TABLE_UPDATE_CONFIG_H_
#define _ASF_TABLE_UPDATE_CONFIG_H_

#define ASF_TABLE_UPDATE_CONFIG_NAME  L"AsfTableUpdateConfig"

#define ASF_TABLE_UPDATE_GUID \
 {0xa4e0dfd2, 0xdeac, 0x4835, {0xa4, 0x56, 0x15, 0xc9, 0x0f, 0x48, 0x08, 0x55}}

/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
**/

#define ASF_TABLE_UPDATE_CONFIG_REVISION 1

#pragma pack (1)
typedef struct {
  UINT8   Revision;        ///< ASF Table Update Configuration Revision
  UINT8   AsfSensorsTable; ///< Adds ASF Sensor Table into ASF ACPI Table
} ASF_TABLE_UPDATE_CONFIG;
#pragma pack ()

#define ASF_TABLE_UPDATE_SETUP_VARSTORE \
    efivarstore ASF_TABLE_UPDATE_CONFIG, \
        attribute = 0x3, name = AsfTableUpdateConfig, guid = ASF_TABLE_UPDATE_GUID;

#endif
