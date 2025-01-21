/** @file
  Definition of the configuration variable for AMT PET Initialization feature.

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

#ifndef _AMT_PET_INIT_CONFIG__H_
#define _AMT_PET_INIT_CONFIG__H_

#define AMT_PET_INIT_CONFIG_REVISION 1

#define AMT_PET_INIT_CONFIG_NAME  L"AmtPetInitConfig"

#define AMT_PET_INIT_CONFIG_GUID \
  {0xF1DD578D, 0x3B3B, 0x4788, {0xA9, 0x1A, 0x2D, 0x73, 0x37, 0xC7, 0x97, 0x10}}

/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
**/

#pragma pack (1)
typedef struct {
  UINT8 Revision;                           ///< AMT PET Initialization Configuration Revision
  UINT8 FwProgress;                         ///< PET Events Progress to receive PET Events. 0: Disable; <b>1: Enable</b>
} AMT_PET_INIT_CONFIG;
#pragma pack ()

#define AMT_PET_INIT_SETUP_VARSTORE\
    efivarstore AMT_PET_INIT_CONFIG, \
        attribute = 0x3, name = AmtPetInitConfig, guid = AMT_PET_INIT_CONFIG_GUID;

#endif
