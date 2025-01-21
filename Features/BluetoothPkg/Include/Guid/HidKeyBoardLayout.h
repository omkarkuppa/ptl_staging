/** @file
  GUID and definitions for HID keyboard layout.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#ifndef __HID_KEYBOARD_LAYOUT_GUID_H__
#define __HID_KEYBOARD_LAYOUT_GUID_H__

//
// GUID for HID keyboard HII package list.
//
#define HID_KEYBOARD_LAYOUT_PACKAGE_GUID \
  { \
    0x16388a95, 0x4d5c, 0x459a, { 0xa1, 0x62, 0x7e, 0x2, 0x13, 0xaa, 0xc9, 0x70 } \
  }

//
// GUID for HID keyboard layout
//
#define HID_KEYBOARD_LAYOUT_KEY_GUID \
  { \
    0xf07b4ce2, 0x2de1, 0x472d, { 0xa8, 0x52, 0x75, 0xd3, 0x3e, 0xd1, 0xd1, 0x5b } \
  }

extern EFI_GUID  gHidKeyboardLayoutPackageGuid;
extern EFI_GUID  gHidKeyboardLayoutKeyGuid;

#endif

