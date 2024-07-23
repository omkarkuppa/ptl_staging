/**@file
  Header file of ASF ACPI table.

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

#ifndef _ASF_TABLE_UPDATE_PROTOCOL_H_
#define _ASF_TABLE_UPDATE_PROTOCOL_H_

#include <IndustryStandard/AlertStandardFormatTable.h>

#define ASF_INFO_MIN_WATCH_DOG_RESET_VALUE  0xFF
#define ASF_INFO_MIN_POLLING_INTERVAL       0xFF
#define ASF_INFO_SYSTEM_ID                  0x01

typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER TableHeader;
  UINT8                       TableData[];
} ASF_TABLE;

/**
  Calculate parts of ASF Table.

  @param[in]        AsfTable     - Pointer to ASF Table.
  @param[in,out]    TableOffset  - On input  - offset to the beginning of ASF Table
                                   On output - current size of the ASF Table
**/
typedef
VOID
(EFIAPI *ASF_UPDATE_ACPI_TABLE) (
  IN      ASF_TABLE        *AsfTable,
  IN OUT  UINT32           *TableOffset
  );

/**
  Get ASF Table Size.

  @retval UINT32 Size of the ASF Table
**/
typedef
UINT32
(EFIAPI *GET_TABLE_SIZE) (
  VOID
  );

typedef struct {
  ASF_UPDATE_ACPI_TABLE CalculateAsfTable;
  GET_TABLE_SIZE        GetAsfTableSize;
} ASF_ACPI_TABLES_UPDATE_PROTOCOL;

#endif
