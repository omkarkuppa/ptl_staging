/** @file
  Definition of Foxville Non-Volatile Memory (NVM) map.

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

#ifndef __FOXVILLE_NVM_MAP_H__
#define __FOXVILLE_NVM_MAP_H__

//
// EDK-II Foundation.
//
#include <Uefi.h>

#define SHADOW_RAM_SECTOR_0_ADDRESS  0
#define SHADOW_RAM_SECTOR_1_ADDRESS  BASE_4KB
#define SHADOW_RAM_SECTOR_SIZE       SIZE_4KB

#define WORD_OFFSET_OF(TYPE, Field)  ((OFFSET_OF(TYPE, Field)) / 2)

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
#define DEVICE_ID_I226_BLANK_NVM  0x125F

typedef UINT16 DEVICE_ID;

#pragma pack (push, 1)

//
// Common definition.
//
typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits 14: 0] Pointer to the beginning of FW image CSS header section.
    ///
    UINT16    Pointer     : 15;
    ///
    /// [Bits    15] Indicate the pointer is in 4KB unit.
    ///
    UINT16    Is4KPtrType :  1;
  } Bits;
  ///
  /// All bit fields as a 16-bit value.
  ///
  UINT16    Uint16;
} FOXVILLE_MODULE_PTR;

typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits  7: 0] Indicate the length of the section in WORD.
    ///
    UINT16    Length :  8;
    ///
    /// [Bits 15: 8] Indicate the CRC value of the section.
    ///
    UINT16    Crc    :  8;
  } Bits;
  ///
  /// All bit fields as a 16-bit value.
  ///
  UINT16    Uint16;
} FOXVILLE_SECTION_HEADER;

//
// WORD 0x0005: Dev_starter Version.
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
// WORD 0x0010: Firmware Image Pointer.
//
typedef FOXVILLE_MODULE_PTR FOXVILLE_FW_IMAGE_PTR;

//
// WORD 0x0011: Flash Device Size.
//
#define V_FOXVILLE_FLASH_SIZE_1MB  0x4
#define V_FOXVILLE_FLASH_SIZE_2MB  0x5
#define V_FOXVILLE_FLASH_SIZE_4MB  0x6
#define V_FOXVILLE_FLASH_SIZE_8MB  0x7

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
    UINT16    Reserved1             : 2;
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
    UINT16    Reserved2             : 4;
  } Bits;
  ///
  /// All bit fields as a 16-bit value.
  ///
  UINT16    Uint16;
} FOXVILLE_FLASH_DEVICE_SIZE;

//
// WORD 0x0012: EEPROM Sizing and Protected Fields.
//
#define V_EEPROM_SIZING_AND_PROTECTED_FIELDS_START_OF_2ND_PROTECTED_AREA_NOT_PRESENTED  0x7FF
#define V_EEPROM_SIZING_AND_PROTECTED_FIELDS_SIGNATURE                                  0x01

typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits 10: 0] Indicate the begining of the write protected area up and including WORD 0x7FF.
    ///              (If value of this field is )
    ///
    UINT16    StartOf2ndProtectedArea : 11;
    ///
    /// [Bits    11] Reserved field.
    ///
    UINT16    Reserved1               :  1;
    ///
    /// [Bits    12] Control the printing and imaging features embedded in FW code.
    ///
    UINT16    PiFeaturesEnabled       :  1;
    ///
    /// [Bits    13] Reserved field.
    ///
    UINT16    Reserved2               :  1;
    ///
    /// [Bits 15:14] Indicate there is a valid EEPROM present.
    ///
    UINT16    Signature               :  2;
  } Bits;
  ///
  /// All bit fields as a 16-bit value.
  ///
  UINT16    Uint16;
} FOXVILLE_EEPROM_SIZING_AND_PROTECTED_FIELDS;

//
// WORD 0x003F: Checksum.
//
#define FOXVILLE_CHECKSUM_VALUE  0xBABA

typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits  7: 0] Lower byte of the checksum field.
    ///
    UINT16    LowerByte : 8;
    ///
    /// [Bits 15: 8] Upper byte of the checksum field.
    ///
    UINT16    UpperByte : 8;
  } Bits;
  ///
  /// All bit fields as a 16-bit value.
  ///
  UINT16    Uint16;
} FOXVILLE_CHECKSUM;

//
// WORD 0x0040: Free Provisioning Area Pointer.
//
typedef FOXVILLE_MODULE_PTR FOXVILLE_FREE_PROV_AREA_PTR;

//
// WORD 0x0041: Free Provisioning Area Size.
//
typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits 15: 0] Size of free provisioning area in 4KB.
    ///                - Free provisioning area without expansion ROM is 0x7B.
    ///                - Free provisioning area with expansion ROM is 0xFB.
    ///
    UINT16    Size : 16;
  } Bits;
  ///
  /// All bit fields as a 16-bit value.
  ///
  UINT16    Uint16;
} FOXVILLE_FREE_PROV_AREA_SIZE;

//
// WORD 0x004A: Expansion ROM Boot Code Section Pointer.
//
typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits 11: 0] Pointer to the expansion ROM section.
    ///
    UINT16    Pointer     : 12;
    ///
    /// [Bits 14:12] Reserved field.
    ///
    UINT16    Reserved    :  3;
    ///
    /// [Bits    15] Indicate the pointer is in 4KB unit.
    ///
    UINT16    Is4KPtrType :  1;
  } Bits;
  ///
  /// All bit fields as a 16-bit value.
  ///
  UINT16    Uint16;
} FOXVILLE_EXP_ROM_AREA_PTR;

//
// WORD 0x0051: Firmware Module Configuration Pointer.
//
typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits 15: 0] Pointer to the loader patch structure.
    ///
    UINT16    Pointer : 16;
  } Bits;
  ///
  /// All bit fields as a 16-bit value.
  ///
  UINT16    Uint16;
} FOXVILLE_FW_MODULE_CFG_PTR;

typedef struct {
  UINT16    Reserved1[3];           ///< WORD [ 2: 0]: Reserved field.
  UINT16    CommonFwParamsPtr;      ///< WORD [    3]: Common FW parameters pointer.
  UINT16    Reserved2[1];           ///< WORD [    4]: Reserved field.
  UINT16    PtLanCfgPtr;            ///< WORD [    5]: Pass-through LAN configuration pointer.
  UINT16    SidebandCfgPtr;         ///< WORD [    6]: Sideband configuration pointer.
  UINT16    FlexTcoFilterCfgPtr;    ///< WORD [    7]: Flexible TCO filter configuration pointer.
  UINT16    Reserved3[3];           ///< WORD [10: 8]: Reserved field.
  UINT16    TrafficTypesParamsPtr;  ///< WORD [   11]: Traffic types parameters pointer.
  UINT16    Reserved4[4];           ///< WORD [15:12]: Reserved field.
} FOXVILLE_FW_MODULE_CFG;

#define FOXVILLE_MANAGEABILITY_MODE_DISABLED  0x0
#define FOXVILLE_MANAGEABILITY_MODE_ENABLED   0x2

typedef union {
  ///
  /// Individual bit fields.
  ///
  struct {
    ///
    /// [Bits  7: 0] Reserved field.
    ///
    UINT16    Reserved1         :  8;
    ///
    /// [Bits 10: 8] Indicate the mode of manageability.
    ///
    UINT16    ManageabilityMode :  3;
    ///
    /// [Bits 15:11] Reserved field.
    ///
    UINT16    Reserved2         :  5;
  } Bits;
  ///
  /// All bit fields as a 16-bit value.
  ///
  UINT16    Uint16;
} FOXVILLE_COMMON_FW_PARAMS_1;

typedef struct {
  FOXVILLE_SECTION_HEADER        SectionHeader;    ///< WORD [    0]: Section header.
  FOXVILLE_COMMON_FW_PARAMS_1    CommonFwParams1;  ///< WORD [    1]: Common firmware parameters 1.
  UINT16                         CommonFwParams2;  ///< WORD [    2]: Common firmware parameters 2.
} FOXVILLE_COMMON_FW_PARAMS_SECTION;

//
// Foxville Non-Volatile Memory (NVM) map.
//
typedef struct {
  UINT16                                         Reserved1[5];                    ///< WORD [  4:  0]: Reserved field.
  FOXVILLE_DEV_STARTER_VERSION                   DevStarterVersion;               ///< WORD [      5]: Dev_starter Version.
  UINT16                                         Reserved2[7];                    ///< WORD [ 12:  6]: Reserved field.
  UINT16                                         DeviceId;                        ///< WORD [     13]: Device ID.
  UINT16                                         VendorId;                        ///< WORD [     14]: Vendor ID.
  UINT16                                         Reserved3[1];                    ///< WORD [     15]: Reserved field.
  FOXVILLE_FW_IMAGE_PTR                          FwImagePtr;                      ///< WORD [     16]: Firmware image pointer.
  FOXVILLE_FLASH_DEVICE_SIZE                     FlashDeviceSize;                 ///< WORD [     17]: Flash device size.
  FOXVILLE_EEPROM_SIZING_AND_PROTECTED_FIELDS    EepromSizeingAndProtectedFields; ///< WORD [     18]: EEPROM Sizing and Protected Fields.
  UINT16                                         Reserved4[44];                   ///< WORD [ 62: 19]: Reserved field.
  FOXVILLE_CHECKSUM                              Checksum;                        ///< WORD [     63]: Checksum.
  FOXVILLE_FREE_PROV_AREA_PTR                    FreeProvAreaPtr;                 ///< WORD [     64]: Free provisioning area pointer.
  FOXVILLE_FREE_PROV_AREA_SIZE                   FreeProvAreaSize;                ///< WORD [     65]: Reserved field.
  UINT16                                         Reserved5[8];                    ///< WORD [ 73: 66]: Reserved field.
  FOXVILLE_EXP_ROM_AREA_PTR                      ExpRomAreaPtr;                   ///< WORD [     74]: Expansion ROM area pointer.
  UINT16                                         Reserved6[6];                    ///< WORD [ 80: 75]: Reserved field.
  FOXVILLE_FW_MODULE_CFG_PTR                     FwModuleCfgPtr;                  ///< WORD [     81]: FW module configuration pointer.
  UINT16                                         Reserved7[46];                   ///< WORD [127: 82]: Reserved field.
} FOXVILLE_NVM_MAP;

#define FXOVILLE_PHY_MODULE_WORD_OFFSET  0x7F0
#define FOXVILLE_SW_AREA_WORD_OFFSET     0x7F1

#pragma pack (pop)

#endif
