/** @file
  Header file for the SMBIOS Memory library.
  This driver will determine memory configuration information from the chipset
  and memory and create SMBIOS Memory structures appropriately.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#ifndef _SMBIOS_MEMORY_H_
#define _SMBIOS_MEMORY_H_

#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PrintLib.h>
#include <Library/HobLib.h>
#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBus.h>
#include <MemInfoHob.h>
#include <IndustryStandard/SmBios.h>
#include <Library/PcdLib.h>

///
/// Driver Consumed Protocol Prototypes
///
#include <Protocol/MemInfo.h>

///
/// Non-static SMBIOS table data to be filled later with a dynamically generated value
///
#define TO_BE_FILLED  0
#define TO_BE_FILLED_STRING  " "        ///< Initial value should not be NULL

///
/// String references in SMBIOS tables. This eliminates the need for pointers.
/// See the DMTF SMBIOS Specification v2.7.1, section 6.1.3.
///
#define NO_STRING_AVAILABLE  0
#define STRING_1  1
#define STRING_2  2
#define STRING_3  3
#define STRING_4  4
#define STRING_5  5
#define STRING_6  6
#define STRING_7  7

///
/// SMBIOS Table values with special meaning
///
#define SMBIOS_TYPE16_USE_EXTENDED_MAX_CAPACITY  0x80000000

#define SMBIOS_TYPE17_UNKNOWN_SIZE               0xFFFF
#define SMBIOS_TYPE17_USE_EXTENDED_SIZE          0x7FFF
#define SMBIOS_TYPE17_KB_BIT_MASK                0x8000
#define SMBIOS_TYPE17_NUMBER_OF_STRINGS               7

#define SMBIOS_TYPE19_USE_EXTENDED_ADDRESSES     0xFFFFFFFF

///
/// Memory module type definition in DDR3 SPD Data
///
#define DDR_MTYPE_SPD_OFFSET  3     ///< Module type, offset 3, bits (3:0)
#define DDR_MTYPE_SPD_MASK    0x0F  ///< Module Type mask
#define DDR_MTYPE_RDIMM       0x01  ///< Registered DIMM Memory
#define DDR_MTYPE_UDIMM       0x02  ///< Unbuffered DIMM Memory
#define DDR_MTYPE_SODIMM      0x03  ///< Small Outline DIMM Memory
#define DDR_MTYPE_LR_DIMM     0x04  ///< LR-DIMM Memory
#define DDR_MTYPE_CUDIMM      0x05  ///< CU DIMM Memory
#define DDR_MTYPE_CSODIMM     0x06  ///< CSO DIMM Memory
#define DDR_MTYPE_MRDIMM      0x07  ///< MR DIMM Memory
#define DDR_MTYPE_DDIMM       0x0A  ///< Small Outline Unbuffered DIMM Memory, 72-bit data bus
#define DDR_MTYPE_MEM_DOWN    0x0B  ///< Soldered Down Memory

///
/// Memory device type definition in DDR3 SPD Data
///
#define DDR_DTYPE_SPD_OFFSET    2     ///< Device type, offset 2, bits (7:0)
#define DDR_DTYPE_DDR3          11    ///< DDR3 memory type
#define DDR_DTYPE_DDR4          12    ///< DDR4 memory type
#define DDR_DTYPE_DDR5          18    ///< DDR5 memory type
#define DDR_DTYPE_LPDDR5        19    ///< LPDDR5 memory type
#define DDR_DTYPE_LPDDR3        0xF1  ///< LPDDR3 memory type
#define DDR_DTYPE_JEDEC_LPDDR3  0x0F  ///< LPDDR3 memory type, JEDEC SPD Specification

///
/// Maximum rank memory size supported by the memory controller: 16GB for DDR5, 16GB (in terms of KB) for DDR4 and 4 GB for other types
///
#define MAX_RANK_CAPACITY       (4 * 1024 * 1024)
#define MAX_RANK_CAPACITY_DDR4  (16 * 1024 * 1024)
#define MAX_RANK_CAPACITY_DDR5  (16 * 1024 * 1024)

//
// DDR3 and DDR4 SPD Register Bytes
//
#define DDR4_SPD_BUFFER_SIZE              384
#define DDR4_SPD_MANUFACTURER_ID_LSB      320
#define DDR4_SPD_MANUFACTURER_ID_MSB      321
#define DDR4_SPD_PART_NO_START_BYTE       329
#define DDR4_SPD_PART_NO_END_BYTE         348
#define DDR4_SPD_SERIAL_NO_START_BYTE     325
#define DDR4_SPD_SERIAL_NO_END_BYTE       328
#define DDR4_SPD_MEMORY_BUS_WIDTH_BYTE    13

#define DDR3_SPD_BUFFER_SIZE              256
#define DDR3_SPD_MANUFACTURER_ID_LSB      117
#define DDR3_SPD_MANUFACTURER_ID_MSB      118
#define DDR3_SPD_PART_NO_START_BYTE       128
#define DDR3_SPD_PART_NO_END_BYTE         145
#define DDR3_SPD_SERIAL_NO_START_BYTE     122
#define DDR3_SPD_SERIAL_NO_END_BYTE       125
#define DDR3_SPD_MEMORY_BUS_WIDTH_BYTE    8

///
/// Memory Module Manufacture ID List Structure
///
typedef struct {
  UINT8  Index;
  UINT8  ManufactureId;
  CHAR8  *ManufactureName;
} MEMORY_MODULE_MANUFACTURE_LIST;

#pragma pack(1)
typedef struct {
  CHAR8 *DeviceLocator;
  CHAR8 *BankLocator;
  CHAR8 *Manufacturer;
  CHAR8 *SerialNumber;
  CHAR8 *AssetTag;
  CHAR8 *PartNumber;
  CHAR8 *FirmwareVersion;
} SMBIOS_TYPE17_STRING_ARRAY;
#pragma pack()

//
// Module-wide global variables
//
extern MEMORY_INFO_DATA_HOB   *mMemInfo;
extern EFI_SMBIOS_HANDLE      mSmbiosType16Handle;

extern CHAR8 *DeviceLocatorStringsDimm[][8];
extern CHAR8 *DeviceLocatorStringsMemoryDown[][8];
extern CHAR8 *BankLocatorStrings[];
extern MEMORY_MODULE_MANUFACTURE_LIST MemoryModuleManufactureList[];

///
/// Prototypes
///
/**
  Add an SMBIOS table entry using EFI_SMBIOS_PROTOCOL.
  Create the full table record using the formatted section plus each non-null string, plus the terminating (double) null.

  @param[in]  Entry                 The data for the fixed portion of the SMBIOS entry.
                                    The format of the data is determined by EFI_SMBIOS_TABLE_HEADER.
                                    Type. The size of the formatted area is defined by
                                    EFI_SMBIOS_TABLE_HEADER. Length and either followed by a
                                    double-null (0x0000) or a set of null terminated strings and a null.
  @param[in]  TableStrings          Set of string pointers to append onto the full record.
                                    If TableStrings is null, no strings are appended. Null strings
                                    are skipped.
  @param[in]  NumberOfStrings       Number of TableStrings to append, null strings are skipped.
  @param[in]  SmbiosProtocol        Instance of Smbios Protocol
  @param[out] SmbiosHandle          A unique handle will be assigned to the SMBIOS record.

  @retval     EFI_SUCCESS           Table was added.
  @retval     EFI_OUT_OF_RESOURCES  Table was not added due to lack of system resources.
**/
EFI_STATUS
AddSmbiosEntry (
  IN  EFI_SMBIOS_TABLE_HEADER *Entry,
  IN  CHAR8                   **TableStrings,
  IN  UINT8                   NumberOfStrings,
  IN  EFI_SMBIOS_PROTOCOL     *SmbiosProtocol,
  OUT EFI_SMBIOS_HANDLE       *SmbiosHandle
  );

/**
  This function installs SMBIOS Table Type 16 (Physical Memory Array).

  @param[in]  SmbiosProtocol   - Instance of Smbios Protocol

  @retval EFI_SUCCESS          - if the data is successfully reported.
  @retval EFI_OUT_OF_RESOURCES - if not able to get resources.
**/
EFI_STATUS
InstallSmbiosType16 (
  IN  EFI_SMBIOS_PROTOCOL *SmbiosProtocol
  );

/**
  This function installs SMBIOS Table Type 17 (Memory Device).
  This function installs one table per memory device slot, whether populated or not.

  @param[in]  SmbiosProtocol   - Instance of Smbios Protocol

  @retval EFI_SUCCESS           - if the data is successfully reported.
  @retval EFI_OUT_OF_RESOURCES  - if not able to get resources.
  @retval EFI_INVALID_PARAMETER - if a required parameter in a subfunction is NULL.
**/
EFI_STATUS
InstallSmbiosType17 (
  IN  EFI_SMBIOS_PROTOCOL *SmbiosProtocol
  );

/**
  This function installs SMBIOS Table Type 19 (Physical Memory Array).

  @param[in]  SmbiosProtocol   - Instance of Smbios Protocol

  @retval EFI_SUCCESS          - if the data is successfully reported.
  @retval EFI_OUT_OF_RESOURCES - if not able to get resources.
**/
EFI_STATUS
InstallSmbiosType19 (
  IN  EFI_SMBIOS_PROTOCOL *SmbiosProtocol
  );

#endif
