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

#ifndef _HIDDEN_DEVICES_H_
#define _HIDDEN_DEVICES_H_

#include <Protocol/DevicePath.h>

#define INTC_HID_STR_SIZE 9

#pragma pack(1)
typedef struct {
  ACPI_EXTENDED_HID_DEVICE_PATH  Acpi;
  CHAR8  HidStr[INTC_HID_STR_SIZE];
} INTC_ACPI_EXTENDED_DEVICE_PATH;
#pragma pack()

#define PMC_HID_STR  "INTC10E7"
#define PMC_PCD_UID  0

#define P2SB_HID_STR  "INTC10E8"
#define P2SB_PCD_UID 0

#endif
