/** @file
  This file contains definitions required for creation of
  Memory Info data and Memory Platform data hobs for the
  express purpose of unit testing the Memory Telemetry
  feature.

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
#ifndef _MEM_INFO_HOB_H_
#define _MEM_INFO_HOB_H_

#pragma pack (push, 1)

#define MAX_NODE        2
#define MAX_CH          4
#define MAX_DDR5_CH     2
#define MAX_DIMM        2

//
// MOCKED DEFINES FOR TESTING PURPOSES
//
#define MAX_RANK_IN_CHANNEL       (4)       ///< The maximum number of ranks per channel.
#define MAX_SDRAM_IN_DIMM         (5)       ///< The maximum number of SDRAMs per DIMM.
#define HOB_MAX_SAGV_POINTS 4

///
/// Memory SMBIOS & OC Memory Data Hob
///
typedef struct {
  UINT8            Status;                  ///< See MrcDimmStatus for the definition of this field.
  UINT8            DimmId;
  UINT32           DimmCapacity;            ///< DIMM size in MBytes.
  UINT16           MfgId;                   ///< Dram module manufacturer ID
  UINT8            ModulePartNum[20];       ///< Module part number for DDR3 is 18 bytes however for DDR4 20 bytes as per JEDEC Spec, so reserving 20 bytes
  UINT8            RankInDimm;              ///< The number of ranks in this DIMM.
  UINT8            SpdDramDeviceType;       ///< Save SPD DramDeviceType information needed for SMBIOS structure creation.
  UINT8            SpdModuleType;           ///< Save SPD ModuleType information needed for SMBIOS structure creation.
  UINT8            SpdModuleMemoryBusWidth; ///< Save SPD ModuleMemoryBusWidth information needed for SMBIOS structure creation.
  UINT16           Speed;                   ///< The maximum capable speed of the device, in MHz
  UINT8            Banks;                   ///< Number of banks the DIMM contains.
  UINT8            BankGroups;              ///< Number of bank groups the DIMM contains.
} DIMM_INFO;

typedef struct {
  UINT8            Status;                  ///< Indicates whether this channel should be used.
  UINT8            ChannelId;
  UINT8            DimmCount;               ///< Number of valid DIMMs that exist in the channel.
  DIMM_INFO        DimmInfo[MAX_DIMM];      ///< Save the DIMM output characteristics.
} CHANNEL_INFO;

typedef struct {
  UINT8            Status;                  ///< Indicates whether this controller should be used.
  UINT16           DeviceId;                ///< The PCI device id of this memory controller.
  UINT8            RevisionId;              ///< The PCI revision id of this memory controller.
  UINT8            ChannelCount;            ///< Number of valid channels that exist on the controller.
  CHANNEL_INFO     ChannelInfo[MAX_CH];     ///< The following are channel level definitions.
} CONTROLLER_INFO;

/// This data structure contains per-SaGv timing values that are considered output by the MRC.
typedef struct {
  UINT32        DataRate;    ///< The memory rate for the current SaGv Point in units of MT/s
  UINT16        MaxMemoryBandwidth; ///< Maximum theoretical bandwidth in GB/s supported by GV
} HOB_SAGV_TIMING_OUT;

/// This data structure contains SAGV config values that are considered output by the MRC.
typedef struct {
  UINT32              NumSaGvPointsEnabled; ///< Count of the total number of SAGV Points enabled.
  UINT32              SaGvPointMask;        ///< Bit mask where each bit indicates an enabled SAGV point.
  HOB_SAGV_TIMING_OUT SaGvTiming[HOB_MAX_SAGV_POINTS];
} HOB_SAGV_INFO;

typedef struct _PPR_RESULT_COLUMNS_HOB {
  UINT8  PprRowRepairsSuccessful;
  UINT8  Controller;
  UINT8  Channel;
  UINT8  Rank;
  UINT8  BankGroup;
  UINT8  Bank;
  UINT32 Row;
  UINT8  Device;
} PPR_RESULT_COLUMNS_HOB;

/**
  Memory Info Data Hob

  Revision 1:
  - Initial version.
  Revision 2:
  - Added MopPackages, MopDensity, MopRanks, MopVendor fields
**/
typedef struct {
  /** As defined in SMBIOS 3.0 spec
    Section 7.18.2 and Table 75
  **/
  UINT8                   MemoryType;                 ///< DDR type: DDR3, DDR4, or LPDDR3
  UINT16                  MaximumMemoryClockSpeed;    ///< The maximum capable speed of the device, in megahertz (MHz)
  /** As defined in SMBIOS 3.0 spec
    Section 7.17.3 and Table 72
  **/
  CONTROLLER_INFO         Controller[MAX_NODE];
  HOB_SAGV_INFO           SagvConfigInfo;             ///< This data structure contains SAGV config values that are considered output by the MRC.
  UINT8                   PprRanInLastBoot;           ///< Whether PPR ran in the prior boot
  UINT16                  PprDetectedErrors;          ///< PPR: Counts of detected bad rows
  UINT16                  PprRepairFails;             ///< PPR: Counts of repair failure
  UINT16                  PprForceRepairStatus;       ///< PPR: Force Repair Status
  UINT16                  PprRepairsSuccessful;       ///< PPR: Counts of repair successes
  PPR_RESULT_COLUMNS_HOB  PprErrorInfo;               ///< PPR: Error location
  UINT8                   PprAvailableResources[MAX_NODE][MAX_CH][MAX_RANK_IN_CHANNEL][MAX_SDRAM_IN_DIMM];
} MEMORY_INFO_DATA_HOB;

/**
  Memory Platform Data Hob

  Revision 1:
  - Initial version.
  Revision 2:
  - Added TsegBase, PrmrrSize, PrmrrBase, Gttbase, MmioSize, PciEBaseAddress fields
**/
typedef struct {
  UINT8                 Revision;
} MEMORY_PLATFORM_DATA;

typedef struct {
  EFI_HOB_GUID_TYPE     EfiHobGuidType;
  MEMORY_PLATFORM_DATA  Data;
  UINT8                 *Buffer;
} MEMORY_PLATFORM_DATA_HOB;

#pragma pack (pop)

#endif // _MEM_INFO_HOB_H_
