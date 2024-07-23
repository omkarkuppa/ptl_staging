/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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


#ifndef _BOOT_DEVICE_INFO_VARIABLE_H_
#define _BOOT_DEVICE_INFO_VARIABLE_H_

#define BOOT_DEVICE_INFO_GUID \
 { 0x5BD6B672, 0xB6EA, 0x4D6A, { 0xB5, 0x90, 0x18, 0xA9, 0x32, 0xB7, 0x87, 0x94 } }

#define BOOT_DEVICE_INFO_VARIABLE_NAME L"BootMediaInfo"
#define BOOT_DEVICE_INFO_REVISION      1
#define UFS_LUN_1                      0x01


typedef struct {
  /*
    Revision of this structure
  */
  UINT32  Version;
  /*
    BIT0 = SPI
    BIT1 = eMMC
    BIT2 = UFS
    BIT3 = NVMe
    BIT4 ~ BIT31 are reserved
  */
  UINT32 BootDeviceType;
  /*
    Partition for Boot image
  */
  UINT32 Partition;
} BOOT_DEVICE_SETUP;

#endif //_BOOT_DEVICE_INFO_VARIABLE_H_
