/** @file

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

#ifndef _BOOT_MAINT_SETUP_H_
#define _BOOT_MAINT_SETUP_H_

#pragma pack(push, 1)
typedef struct {
  UINT8   ComNum;
  UINT8   DevicePath[1];
} SERIAL_PORT_TAG;
typedef struct _SERIAL_PORT_ATTRIBUTE {
  UINT64            BaudRate;
  UINT8             DataBits;
  UINT8             StopBits;
  UINT8             Parity;
  UINT8             TerminalType;
  UINT8             FlowControl;
  UINT8             LegacyResolution;
  UINT8             IsEnabled;
  UINT8             Length;
  SERIAL_PORT_TAG   ComTag;
} SERIAL_PORT_ATTRIBUTE;

/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
**/
#define TOTAL_SERIAL_PORT_INFO_REVISION 1

typedef struct {
  UINT64 TotalSize;
  SERIAL_PORT_ATTRIBUTE PortAttributes[1];
} TOTAL_SERIAL_PORT_INFO;

#pragma pack(pop)
#endif
