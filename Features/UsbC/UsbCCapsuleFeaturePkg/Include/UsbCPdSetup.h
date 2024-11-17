/** @file
 Variable definition for UsbC PD Setup Configuration.

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

#ifndef USBC_PD_SETUP_H
#define USBC_PD_SETUP_H

#define USBC_PD_NAME      L"UsbCPdSetup"

#define USBC_PD_SETUP_GUID \
 { 0x191FC86C, 0x6215, 0x440F, { 0x84, 0x88, 0xCD, 0xBA, 0x9E, 0xFA, 0x28, 0x53 } }

#pragma pack(1)

typedef struct {
  UINT64  UsbCPd1Version;
  UINT64  UsbCPd2Version;
  UINT64  UsbCPd3Version;
  UINT64  UsbCPd4Version;
} USBC_PD_SETUP;

#pragma pack()


#endif  // USBC_PD_SETUP_H
