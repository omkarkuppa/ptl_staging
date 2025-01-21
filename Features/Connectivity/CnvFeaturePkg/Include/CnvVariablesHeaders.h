/** @file
  Declaration of Intel Connectivity Common Variables Headers and guids for WLAN, BT.

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

  @par Specification Reference: 559910_Intel_Connectivity_Platforms_BIOS_Guidelines_Rev6_6.pdf

**/

#ifndef __CNV_VARIABLES_HEADERS_H__
#define __CNV_VARIABLES_HEADERS_H__
//
// Indicate the API changes
//
#define CNV_UEFI_VAR_API_FILE_VERSION  1

/**
  Update below history table to indicate the API changes
  <b>Revision 0</b>:  - Initial version.
  <b>Revision 1</b>:  - Migration from ACPI to UEFI Variable.
**/

//
// A variable GUID where all Intel WLAN CNV configuration features
//
#define INTEL_UEFI_CNV_WLAN_VARIABLES_GUID \
  { \
    0x92daaf2f, 0xc02b, 0x455b, { 0xb2, 0xec, 0xf5, 0xa3, 0x59, 0x4f, 0x4a, 0xea } \
  }
//
// A variable GUID where all Intel Bluetooth CNV configuration features
//
#define INTEL_UEFI_CNV_BT_VARIABLES_GUID \
  { \
    0x42780dd5, 0x9a7d, 0x404c, { 0x80, 0xe4, 0x7f, 0x70, 0x94, 0x36, 0x3, 0x94 }\
  }
//
// A variable GUID where all Intel CNV Common definations for Bluetooth and WiFi
//
#define INTEL_UEFI_CNV_COMMON_VARIABLES_GUID \
  { \
    0xe65d8884, 0xd4af, 0x4b20, { 0x8d, 0x3, 0x77, 0x2e, 0xcc, 0x3d, 0xa5, 0x31 }\
  }

#pragma pack(push, 1)
//
// CNV Header Definition
// All the CNV UEFI Variables main structures shall start with the header.
// revisions control
//
typedef struct {
  UINT8    Revision;
} CNV_UEFI_VAR_HEADER;

#pragma pack(pop)

#endif //__CNV_VARIABLES_HEADERS_H__
