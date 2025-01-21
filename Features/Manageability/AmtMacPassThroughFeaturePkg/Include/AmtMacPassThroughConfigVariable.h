/** @file
  Definition of the configuration variable for AMT MAC PassThrough Feature.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _AMT_MAC_PASSTHROUGH_CONFIG__H_
#define _AMT_MAC_PASSTHROUGH_CONFIG__H_

#define AMT_MAC_PASSTHROUGH_CONFIG_REVISION 1

#define AMT_MAC_PASSTHROUGH_CONFIG_NAME  L"AmtMacPassThroughConfig"

#define AMT_MAC_PASSTHROUGH_CONFIG_GUID \
  {0xB4C97F7C, 0x0B12, 0x43B0, {0x89, 0x07, 0x06, 0x8A, 0xBC, 0x50, 0x98, 0x17}}

#define MAC_ADDRESS_SIZE 6

/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
**/

#pragma pack (1)
typedef struct {
  UINT8 Revision;                           ///< AMT MAC PassThrough Configuration Revision
  UINT8 AmtMacPassThroughState;             ///< AMT MAC PassThrough feature state: <b>0: Disable</b>; 1: Enable
  UINT8 AmtMacPassThroughPreviousBootState; ///< AMT MAC PassThrough Previous boot feature state
  UINT8 MacBuffer[MAC_ADDRESS_SIZE];        ///< Buffer holding Integrated GBE MAC Address
} AMT_MAC_PASSTHROUGH_CONFIG;
#pragma pack ()

#define AMT_MAC_PASSTHROUGH_SETUP_VARSTORE\
    efivarstore AMT_MAC_PASSTHROUGH_CONFIG, \
        attribute = 0x3, name = AmtMacPassThroughConfig, guid = AMT_MAC_PASSTHROUGH_CONFIG_GUID;

#endif
