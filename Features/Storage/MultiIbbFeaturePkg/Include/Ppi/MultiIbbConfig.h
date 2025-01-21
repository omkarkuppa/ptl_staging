/** @file
  Header of Multi-IBB support configuration.

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
**/

#ifndef _MULTI_IBB_CONFIG_PPI_H_
#define _MULTI_IBB_CONFIG_PPI_H_

//
// Global ID for the PEI_FIRMWARE_VOLUME_VERIFY_HASH_FV_PPI
//
#define PEI_FIRMWARE_VOLUME_VERIFY_HASH_FV_PPI_GUID \
  { \
    0xc6f495a3, 0xc4a3, 0x4f51, { 0x8c, 0x1d, 0x1a, 0x94, 0x80, 0x6c, 0x3b, 0xdb } \
  }

#define MULTI_IBB_INFO_COUNT \
  FixedPcdGet32 (PcdMultiIbbInfoCount)

#define ACM_CACHE_INITIAL_IBB_INDEX 0
#define BIOS_CACHE_SECOND_IBB_INDEX 2

typedef enum {
  IBB_CACHE_AGENT_ACM    = 0, // IBB0 a.k.a. common IBB
  IBB_CACHE_AGENT_BIOS   = 1, // Rest of pre-memory IBBs
} AGENT_LOADING_IBB_INTO_CACHE;

typedef struct _MULTI_IBB_INFO {
  UINT64                             Base;              // Firmware Volume (FV) Base of IBB
  UINT64                             Length;            // FV Size of IBB
  UINT64                             IbbIndex;          // IBB Index
  EFI_GUID                           *HashGuid;         // IBB Hash File GUID
  BOOLEAN                            CurrentlyCached;   // IBB is cached already or not
  BOOLEAN                            FvInstallNeeded;   // FV installation is required
  AGENT_LOADING_IBB_INTO_CACHE       IbbCacheAgent;     // Who caches the IBB, ACM or BIOS
  MTRR_MEMORY_CACHE_TYPE             CacheType;         // Cacheability type
} MULTI_IBB_INFO;

typedef struct _PEI_FIRMWARE_VOLUME_VERIFY_HASH_FV_PPI {
  MULTI_IBB_INFO          MultiIbbInfo[MULTI_IBB_INFO_COUNT];
  UINTN                   CommonIbbBase;
  UINTN                   CommonIbbSize;
  UINTN                   DataStackSize;
} PEI_FIRMWARE_VOLUME_VERIFY_HASH_FV_PPI;

extern EFI_GUID gPeiFirmwareVolumeVerifyHashFvPpiGuid;

#endif // _MULTI_IBB_CONFIG_PPI_H_
