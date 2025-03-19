/** @file
  Definition of Foxville device generic information.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#ifndef __FOXVILLE_COMMON_H__
#define __FOXVILLE_COMMON_H__

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Protocol/PciIo.h>

#pragma pack (push, 1)

typedef UINT8 FOXVILLE_FLASH_MODE;

#define FLASH_MODE_PROTECTED          0x00  ///< Normal programming mode.
#define FLASH_MODE_UNPROTECTED        0x01  ///< Unprotected flash programming mode.
#define FLASH_MODE_NO_FLASH_DETECTED  0x02  ///< No flash detected.
#define FLASH_MODE_BLANK              0x03  ///< Blank flash programming mode.

typedef struct {
  EFI_PCI_IO_PROTOCOL    *PciIo;
  UINT32                 FlashSize;
} FOXVILLE_HW_INSTANCE;

typedef struct {
  BOOLEAN                IsValid;
  UINT16                 VendorId;
  UINT16                 DeviceId;
  UINT16                 NvmVersion;
  FOXVILLE_FLASH_MODE    FlashMode;
  UINT32                 FlashSize;
} FOXVILLE_DEVICE_INFO;

#pragma pack (pop)

#endif
