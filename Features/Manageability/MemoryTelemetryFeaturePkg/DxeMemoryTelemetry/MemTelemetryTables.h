/** @file
  Memory Telemetry Table Definitions

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

#ifndef _MEM_TELEMETRY_TABLES_H_
#define _MEM_TELEMETRY_TABLES_H_

#include <PiDxe.h>
#include <Uefi.h>
#include <MemInfoHob.h>

///
/// This value depicts the format and contents of the MEMORY_TELEMETRY record
///
#define MEMORY_TELEMETRY_PHAT_RECORD_TYPE       0x2000
#define MEMORY_TELEMETRY_PHAT_RECORD_VERSION    2

///
/// Memory Telemetry Table ID's
///
#define MEMORY_TELEMETRY_MAIN_TABLE_ID            1
#define MEMORY_TELEMETRY_MODULE_TABLE_ID          2
#define MEMORY_TELEMETRY_SAGV_TABLE_ID            3
#define MEMORY_TELEMETRY_AMT_PPR_TABLE_ID         4
#define MEMORY_TELEMETRY_RANK_RESOURCE_TABLE_ID   5

///
/// Memory Telemetry Main Table Revision ID
///
#define MEMORY_TELEMETRY_MAIN_TABLE_REVISION            1
#define MEMORY_TELEMETRY_MODULE_TABLE_REVISION          1
#define MEMORY_TELEMETRY_SAGV_TABLE_REVISION            1
#define MEMORY_TELEMETRY_AMT_PPR_TABLE_REVISION         1
#define MEMORY_TELEMETRY_RANK_RESOURCE_TABLE_REVISION   1

#pragma pack(1)
///
/// Memory Telemetry Data Subtable header
///
typedef struct {
    UINT8       TableID;        ///< Table ID
    UINT16      TableLength;    ///< Table Length
    UINT8       Revision;       ///< Revision
} MEMORY_TELEMETRY_TABLE_HEADER;

///
/// Memory Module Telemetry Data Table Structures
///
typedef struct {
  UINT32  Node      :  4;
  UINT32  Channel   :  4;
  UINT32  Dimm      :  4;
  UINT32  Reserved  : 20;
} MEMORY_MODULE_LOCATION;

typedef struct {
  MEMORY_MODULE_LOCATION  Location;
  UINT8                   MemoryType;   ///< Memory Type Memory Type of the module (E.g DDR4, DDR5)
  UINT16                  MfgId;        ///< Manufacturer ID Part number of the memory module
  UINT16                  Speed;        ///< Speed Max data rate supported by the memory module on board, in MHz
  UINT8                   RankInDimm;   ///< Number of rank Memory organization: number of ranks of the memory module
  UINT8                   Banks;        ///< Number of banks Memory organization: number of banks of the memory module
  UINT8                   BankGroups;   ///< Bank group Memory organization
  UINT8                   FormFactor;   ///< SPD ModuleType
} MEMORY_MODULE_ENTRY;

typedef struct {
  MEMORY_TELEMETRY_TABLE_HEADER   Header;             ///< Table Header
  UINT8                           Reserved[8];        ///< Reserved
  UINT8                           ModuleEntryCount;   ///< Number of memory module tables added
  UINT32                          ModuleEntryLength;  ///< Length of the memory module table
  // MEMORY_MODULE_ENTRY[]                            ///< Memory Data Entry
} MEMORY_TELEMETRY_MODULE_TABLE;

///
/// SAGV Data table for Memory Telemetry
///
typedef struct {
  UINT32          DataRate;     ///< Actual current data rate
  UINT32          Bandwidth;    ///< Calculated Peak Bandwidth Theoretical max bandwidth
} SAGV_ENTRY;

typedef struct {
  MEMORY_TELEMETRY_TABLE_HEADER   Header;           ///< Table Header
  UINT32                          SagvPointMask;    ///< Maximum data rate of Integrated Memory Controller maximum fused data rate supported by the IMC Also, includes calculated SAGV points
  UINT16                          MaxFrequency;     ///< Maximum operating frequency of IMC
  UINT8                           Reserved[8];      ///< Reserved
  UINT8                           SagvEntryCount;   ///< Number of SaGv Tables
  UINT32                          SagvEntryLength;  ///< Length of the SAGV table
  // SAGV_ENTRY[]                                   ///< SAGV Data Entry
} MEMORY_TELEMETRY_SAGV_TABLE;

///
/// AMT PPR Data Table
///
typedef struct {
    UINT8 PprRanLastBoot  : 1;  ///< PPR Ran in last boot
    UINT8 AmtRanLastBoot  : 1;  ///< AMT ran in last boot
    UINT8 Reserved        : 6;  ///< Reserved
} RAN_LAST_BOOT_STATUS;

typedef struct {
  MEMORY_TELEMETRY_TABLE_HEADER   Header;                       ///< Table Header
  UINT8                           PprAvailable;                 ///< AMT Enable
  RAN_LAST_BOOT_STATUS            AmtPprRunStatus;              ///< Whether AMT and PPR ran in the prior boot [Bit 0: PPR, Bit 1: AMT]
  UINT16                          TotalRowFailuresDiscovered;   ///< PPR: Counts of detected bad rows
  UINT16                          TotalPprRowRepairsSuccessful; ///< PPR: Counts of repair successes
  UINT16                          TotalUncorrectableDevices;    ///< PPR: Counts of repair failure
  UINT8                           Reserved[8];                  ///< Reserved
  UINT8                           PprResultsHobCount;           ///< Number of PPR Result Hobs
  UINT32                          PprResultsHobLength;          ///< Length PPR available resources table
  // PPR_RESULT_COLUMNS_HOB[]                                   ///< PPR: Error location
} MEMORY_TELEMETRY_AMT_PPR_TABLE;

///
/// Rank Resource Table for Memory Telemetry
///
typedef struct {
  UINT32  Node     :  4;
  UINT32  Channel  :  4;
  UINT32  Dimm     :  4;
  UINT32  Rank     :  4;
  UINT32  Reserved : 16;
} MEMORY_RANK_LOCATION;

typedef struct {
  MEMORY_RANK_LOCATION  Location;           ///< Location of the Rank, 1 Dimm idx will correspond to 2 rank so DimmIdx = RankIdx/2
  UINT8                 ChipCount;          ///< Number of chips in a rank -> MAX_SDRAM_IN_DIMM
  UINT8                 ChipResource[32];   ///< Number of resources on chip
} RANK_RESOURCE_ENTRY;

typedef struct {
  MEMORY_TELEMETRY_TABLE_HEADER Header;               ///< Table Header
  UINT8                         Reserved[8];          ///< Reserved
  UINT8                         ResourceEntryCount;   ///< Number of Rank Resource Tables
  UINT16                        ResourceEntryLength;  ///< Length of the Rank Resource Table
  // RANK_RESOURCE_ENTRY[]                            ///< Rank Resource Data Entry
} MEMORY_TELEMETRY_RANK_RESOURCE_TABLE;

///
/// Main Memory Telemetry Table to accessing all telemetry data
///
typedef struct {
  MEMORY_TELEMETRY_TABLE_HEADER     Header;       ///< Table Header
  UINT8                             Reserved[8];  ///< Reserved
  // MEMORY_TELEMETRY_MODULE_TABLE                ///< Memory Module Table Data
  // MEMORY_TELEMETRY_SAGV_TABLE                  ///< SAGV Table Data
  // MEMORY_TELEMETRY_AMT_PPR_TABLE               ///< AMT PPR Table Data
  // MEMORY_TELEMETRY_RANK_RESOURCE_TABLE         ///< Rank Resource Table Data
} MEMORY_TELEMETRY_MAIN_DATA_TABLE;

///
/// ACPI structure for Memory Telemetry Table addition to PHAT
///
typedef struct {
  UINT16      PlatformRecordType;       ///< Platform Record Type
  UINT16      RecordLength;             ///< Length of the record
  UINT8       Revision;                 ///< Revision of the record
  // MEMORY_TELEMETRY_MAIN_DATA_TABLE   ///< Memory Telemetry Main Data Table
} PHAT_MEMORY_TELEMETRY_RECORD_STRUCTURE;

#pragma pack()

#endif  // _MEM_TELEMETRY_TABLES_H_
