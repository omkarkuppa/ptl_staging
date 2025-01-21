/** @file
  The header file defined value and structure used to generate
  Boot error record table (BERT) table

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

@par Specification Reference:
  Advanced Configuration and Power Interface (ACPI) Specification, version 6.3
**/

#ifndef __BERT_TABLE_H__
#define __BERT_TABLE_H__

//
// BERT table definitions & structures
//
#define EFI_ACPI_CREATOR_ID        SIGNATURE_32 ('I', 'N', 'T', 'L')
#define EFI_ACPI_CREATOR_REVISION  0x00000001
#define EFI_ACPI_OEM_TABLE_ID      SIGNATURE_64 ('E', 'D', 'K', '2', ' ', ' ', ' ', ' ') // OEM table id 8 bytes long
#define EFI_WHEA_OEM_REVISION      0x0001;

#define EFI_2_7_FIRMWARE_ERROR_RECORD_REVISION  2  // Header Revision. Will be removed when Cper.h has defined it.

#define CRASHLOG_RECORDID_GUID  \
{ 0x8f87f311, 0xc998, 0x4d9e, { 0xa0, 0xc4, 0x60, 0x65, 0x51, 0x8c, 0x4f, 0x6d } }

#define TELEMETRY_RECORDID_GUID  \
{ 0x26d769a7, 0xc31a, 0x43d0, { 0x93, 0x78, 0x3c, 0x6c, 0x87, 0x2e, 0xea, 0x4d } }

#define TRACE_RECORDID_GUID  \
{ 0x024508e0, 0xd564, 0x42ed, { 0xb2, 0x36, 0x58, 0x0d, 0x54, 0x2b, 0xc9, 0xd6 } }

#pragma pack(1)
typedef struct {
  EFI_ACPI_6_5_GENERIC_ERROR_DATA_ENTRY_STRUCTURE    GenericErrorDataEntry;
  EFI_FIRMWARE_ERROR_DATA                            EfiFirmwareErrorData;
} FIRMWARE_GENERIC_ERROR;
#pragma pack()


#endif
