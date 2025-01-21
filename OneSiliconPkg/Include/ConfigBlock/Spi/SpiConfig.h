/** @file
  PCH SPI Flash Controller config block

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#ifndef _SPI_CONFIG_H_
#define _SPI_CONFIG_H_

#include <ConfigBlock.h>

#define SPI_CONFIG_REVISION 1
extern EFI_GUID gSpiConfigGuid;

#pragma pack (push,1)

/**
  Basic configuration for option features of PCH SPI Flash controller
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;           ///< Config Block Header
  /**
    Enable extended BIOS Direct Read Region feature
    Enabling this will make all memory accesses in a decode range to be translated
    to BIOS region reads from SPI flash
    <b>0: Disabled</b>, 1: Enabled
  **/
  UINT32    ExtendedBiosDecodeRangeEnable   :  1;
  UINT32    RsvdBits0                       : 31;       ///< Reserved bits
  /**
    Base address that will be used for Extended Decode Range.
    This will be ignored when ExtendedBiosDecodeRangeEnable is set to 0.
  **/
  UINT32    ExtendedBiosDecodeRangeBase;
  /**
    Limit address that will be used for Extended Decode Range.
    This will be ignored when ExtendedBiosDecodeRangeEnable is set to 0.
  **/
  UINT32    ExtendedBiosDecodeRangeLimit;
} SPI_CONFIG;

#pragma pack (pop)

#endif // _SPI_CONFIG_H_
