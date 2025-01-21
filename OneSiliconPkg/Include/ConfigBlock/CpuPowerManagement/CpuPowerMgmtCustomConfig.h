/** @file
  CPU Power Managment Custom Config Block.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#ifndef _CPU_POWER_MGMT_CUSTOM_CONFIG_H_
#define _CPU_POWER_MGMT_CUSTOM_CONFIG_H_

#define CPU_POWER_MGMT_CUSTOM_CONFIG_REVISION 1

extern EFI_GUID gCpuPowerMgmtCustomConfigGuid;

#pragma pack (push,1)

///
/// Defines the maximum number of custom ratio states supported.
///
#define MAX_CUSTOM_RATIO_TABLE_ENTRIES    40
#define MAX_16_CUSTOM_RATIO_TABLE_ENTRIES 16

///
/// This structure is used to describe the custom processor ratio table desired by the platform.
///
typedef struct {
  UINT8  MaxRatio;                                           ///< The maximum ratio of the custom ratio table.
  UINT8  NumberOfEntries;                                    ///< The number of custom ratio state entries, ranges from 2 to 40 for a valid custom ratio table.
  UINT8  Rsvd0[2];                                           ///< Reserved for DWORD alignment.
  UINT32 Cpuid;                                              ///< The CPU ID for which this custom ratio table applies.
  UINT8  StateRatio[MAX_CUSTOM_RATIO_TABLE_ENTRIES];         ///< The processor ratios in the custom ratio table.
  ///
  /// If there are more than 16 total entries in the StateRatio table, then use these 16 entries to fill max 16 table.
  /// @note If NumberOfEntries is 16 or less, or the first entry of this table is 0, then this table is ignored,
  /// and up to the top 16 values from the StateRatio table is used instead.
  ///
  UINT8  StateRatioMax16[MAX_16_CUSTOM_RATIO_TABLE_ENTRIES];
#if ((MAX_CUSTOM_RATIO_TABLE_ENTRIES + MAX_16_CUSTOM_RATIO_TABLE_ENTRIES) % 4)
  UINT8  Rsvd1[4 - ((MAX_CUSTOM_RATIO_TABLE_ENTRIES + MAX_16_CUSTOM_RATIO_TABLE_ENTRIES) % 4)];  ///< If needed, add padding for dword alignment.
#endif
} PPM_CUSTOM_RATIO_TABLE;

/**
  CPU Power Management Custom Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER    Header;                                                ///< Config Block Header
  PPM_CUSTOM_RATIO_TABLE CustomRatioTable;                                      ///< Custom Processor Ratio Table Instance
} CPU_POWER_MGMT_CUSTOM_CONFIG;

#pragma pack (pop)

#endif // _CPU_POWER_MGMT_CUSTOM_CONFIG_H_
