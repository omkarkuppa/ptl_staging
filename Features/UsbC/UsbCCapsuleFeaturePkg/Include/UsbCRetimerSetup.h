/** @file
 Variable definition for UsbC Retimer Setup Configuration.

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

#ifndef USBC_RETIMER_SETUP_H
#define USBC_RETIMER_SETUP_H

#define USBC_RETIMER_NAME      L"UsbCRetimer"

#define USBC_RETIMER_SETUP_GUID \
 { 0x8542C05D, 0x78F1, 0x4229, { 0xBF, 0x5C, 0x79, 0x8E, 0x44, 0xA0, 0xBF, 0xC9 } }

#pragma pack(1)

typedef struct {
  UINT32  UsbCRetimer0Version;
  UINT32  UsbCRetimer1Version;
  UINT32  UsbCRetimer2Version;
  UINT32  UsbCRetimer3Version;
} USBC_RETIMER_SETUP;

#pragma pack()


#endif  // USBC_RETIMER_SETUP_H
