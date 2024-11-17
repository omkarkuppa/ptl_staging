/** @file
  Definition of Foxville Non-Volatile Memory (NVM) map.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef __FOXVILLE_NVM_MAP_H__
#define __FOXVILLE_NVM_MAP_H__

//
// EDK-II Foundation.
//
#include <Uefi.h>

//
// Definition of vendor ID.
//
#define VENDOR_ID_INTEL  0x8086

//
// Defintions of device ID - I225 series.
//
#define DEVICE_ID_I225_LM         0x15F2
#define DEVICE_ID_I225_V          0x15F3
#define DEVICE_ID_I225_K          0x3100
#define DEVICE_ID_I225_K2         0x3101
#define DEVICE_ID_I225_LMVP       0x5502
#define DEVICE_ID_I225_IT         0x0D9F
#define DEVICE_ID_I225_BLANK_NVM  0x15FD
//
// Definitions of device ID - I226 series.
//
#define DEVICE_ID_I226_LM         0x125B
#define DEVICE_ID_I226_V          0x125C
#define DEVICE_ID_I226_IT         0x125D
#define DEVICE_ID_I226_K          0x3102
#define DEVICE_ID_I226_LMVP       0x5503

typedef UINT16 DEVICE_ID;

#pragma pack (push, 1)

//
// WORD 0x0005: Dev_starter Version
//
typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits  7: 0] Lower byte of this firmware version.
    ///
    UINT16    LowerByte : 8;
    ///
    /// [Bits 15: 8] Upper byte of this firmware version.
    ///
    UINT16    UpperByte : 8;
  } Bits;
  ///
  /// All bit fields as a 16-bit value.
  ///
  UINT16    Uint16;
} FOXVILLE_DEV_STARTER_VERSION;

//
// WORD 0x0011: Flash Device Size
//
#define B_FOXVILLE_FLASH_SIZE_1MB  0x4
#define B_FOXVILLE_FLASH_SIZE_2MB  0x5
#define B_FOXVILLE_FLASH_SIZE_4MB  0x6
#define B_FOXVILLE_FLASH_SIZE_8MB  0x7

typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits  2: 0] Indicate flash size.
    ///
    UINT16    FlashSize             : 3;
    ///
    /// [Bit      3] SST mode bit.
    ///
    UINT16    SstMode               : 1;
    ///
    /// [Bit      4] Unprotected after reset bit.
    ///
    UINT16    UnprotectedAfterReset : 1;
    ///
    /// [Bits  6: 5] Reserved field.
    ///
    UINT16    Rsvd1                 : 2;
    ///
    /// [Bit      7] Fast read support bit.
    ///
    UINT16    FastReadSupport       : 1;
    ///
    /// [Bit      8] Flash defaults from this word bit.
    ///
    UINT16    FlashDefaults         : 1;
    ///
    /// [Bits 11: 9] Flash speed controls the frequency of the SPI flash clock.
    ///
    UINT16    FlashSpeed            : 3;
    ///
    /// [Bits 15:12] Reserved field.
    ///
    UINT16    Rsvd2                 : 4;
  } Bits;
  ///
  /// All bit fields as a 16-bit value.
  ///
  UINT16    Uint16;
} FOXVILLE_FLASH_DEVICE_SIZE;

#pragma pack (pop)

#endif
