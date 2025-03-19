/** @file
  The definition of unified payload item for UsbC capsule

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

#ifndef __USBC_PD_BRIDGE_RETIMER_H__
#define __USBC_PD_BRIDGE_RETIMER_H__

#include <Uefi.h>

///
/// Data structures used in PD Bridge image payload
///
#define PD_BRIDGE_PAYLOAD_HEADER_SIGNATURE     SIGNATURE_32 ('P', 'D', 'B', 'G')

#pragma pack(1)
typedef struct {
  UINT32  Signature;
  UINT32  HeaderSize;
  UINT32  PayloadCount;
  UINT32  Reserved;
  ///
  /// Variable length array of dimension [EmbeddedDriverCount + PayloadItemCount]
  /// containing offsets of each of the drivers and payload items contained within the capsule
  ///
  /// PAYLOAD_ITEM PayloadItem[];
} PAYLOAD_HEADER;

typedef union {
  struct {
    UINT8  PdCntrlIndex;
    UINT8  ShareFlashMode;  ///< 0: Disabled, 1:Enabled
    UINT8  PdBridgeType;    ///< 0: Gothic Bridge, Others: Reserved
    UINT8  Reserved[13];
  } PdBridge;  ///< FirmwareType 3: PdBridge

  UINT8 Data8[16];

} FIRMWARE_PRIVATE_DATA;

typedef struct {
  FIRMWARE_PRIVATE_DATA  PrivateData;
  UINT32                 ImageOffset;
  UINT32                 ImageSize;
  UINT8                  FirmwareType;   ///< 3: PdBridge, Others: Reserved
  UINT8                  Reserve[3];
} PD_BRIDGE_PAYLOAD_ITEM;

#pragma pack()

typedef enum {
  INTEGRATED_TBT_RETIMER = 0x0,
  DISCRETE_TBT_RETIMER = 0x1,
  DISCRETE_TBT = 0x02,
  PD_BRIDGE = 0x03,
  FIRMWARE_TYPE_MAX_VALUE
} FIRMWARE_TYPE;

typedef enum {
  GOTHIC_BRIDGE = 0x0,
  PD_BRIDGE_TYPE_INVALID_VALUE
} PD_BRIDGE_TYPE;

#endif  ///< __USBC_PD_BRIDGE_RETIMER_H__
