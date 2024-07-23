/** @file
  Definition of BDAT data structures that are applicible for all schemas

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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
#ifndef BDAT4_H_
#define BDAT4_H_

#include <Base.h>
#include <IndustryStandard/Acpi30.h>
#include <Protocol/AcpiTable.h>

#define EFI_BDAT_TABLE_SIGNATURE  SIGNATURE_32 ('B', 'D', 'A', 'T')

#pragma pack(push, 1)

typedef struct {
  UINT8                       BiosDataSignature[8];  ///< "BDATHEAD"
  UINT32                      BiosDataStructSize;    ///< sizeof BDAT_STRUCTURE
  UINT16                      Crc16;                 ///< 16-bit CRC of BDAT_STRUCTURE (calculated with 0 in this field)
  UINT16                      Reserved;
  union {
    struct {
      UINT16                  Primary;
      UINT16                  Secondary;
    } Rev;
    UINT32                    Data32;
    UINT16                    Data16[2];
    UINT8                     Data8[4];
  }                           Version;               ///< Version, primary then secondary.
  UINT32                      OemOffset;             ///< Optional offset to OEM-defined structure
  UINT32                      Reserved1;
  UINT32                      Reserved2;
} BDAT_HEADER_STRUCTURE;

typedef struct {
  UINT16                      SchemaListLength;      ///< How many offsets are listed in the schema list.
  UINT16                      Reserved1;
  struct {
    UINT16                    Year;
    UINT8                     Month;
    UINT8                     Day;
    UINT8                     Hour;
    UINT8                     Minute;
    UINT8                     Second;
    UINT8                     Reserved2;
  } Time;                                            ///< Schema update time.
} BDAT_SCHEMA_LIST_STRUCTURE;

typedef struct {
  BDAT_HEADER_STRUCTURE       Header;
  BDAT_SCHEMA_LIST_STRUCTURE  Schemas;
} BDAT_STRUCTURE;

//
//

//
// Every BDAT schema should begin with this structure definition
//
typedef struct {
  EFI_GUID                    SchemaId;              ///< The GUID uniquely identifies the format of the data contained within the structure.
  UINT32                      DataSize;              ///< The total size of the memory block, including both the header as well as the schema specific data.
  UINT16                      Crc16;                 ///< Crc16 is computed in the same manner as the field in the BDAT_HEADER_STRUCTURE.
} BDAT_SCHEMA_HEADER_STRUCTURE;

///
/// BIOS Data ACPI structure
///
typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER             Header;
  EFI_ACPI_3_0_GENERIC_ADDRESS_STRUCTURE  BdatGas;
} EFI_BDAT_ACPI_DESCRIPTION_TABLE;

#pragma pack (pop)

#endif
