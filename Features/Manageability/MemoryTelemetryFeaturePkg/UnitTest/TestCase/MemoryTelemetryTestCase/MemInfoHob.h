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
  UINT16           CkdMfgID;                ///< Clock Driver (CKD) Manufacturer ID
  UINT8            CkdDeviceRev;            ///< Clock Driver (CKD) device revision
  UINT16           DramMfgID;               ///< Manufacturer ID code for DRAM chip on the module
  UINT8            ModulePartNum[20];       ///< Module part number for DDR3 is 18 bytes however for DDR4 20 bytes as per JEDEC Spec, so reserving 20 bytes
  UINT8            RankInDimm;              ///< The number of ranks in this DIMM.
  UINT8            SpdDramDeviceType;       ///< Save SPD DramDeviceType information needed for SMBIOS structure creation.
  UINT8            SpdModuleType;           ///< Save SPD ModuleType information needed for SMBIOS structure creation.
  UINT8            SpdModuleMemoryBusWidth; ///< Save SPD ModuleMemoryBusWidth information needed for SMBIOS structure creation.
  UINT8            SpdSave[MAX_SPD_SAVE];   ///< Save SPD Manufacturing information needed for SMBIOS structure creation.
  UINT16           Speed;                   ///< The maximum capable speed of the device, in MHz
  UINT8            MdSocket;                ///< MdSocket: 0 = Memory Down, 1 = Socketed. Needed for SMBIOS structure creation.
  UINT8            Banks;                   ///< Number of banks the DIMM contains.
  UINT8            BankGroups;              ///< Number of bank groups the DIMM contains.
} DIMM_INFO;

typedef struct {
  UINT8            Status;                  ///< Indicates whether this channel should be used.
  UINT8            ChannelId;
  UINT8            DimmCount;               ///< Number of valid DIMMs that exist in the channel.
  MRC_CH_TIMING    Timing[MAX_PROFILE_NUM]; ///< The channel timing values.
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
  <b>Revision 1:</b>
  - Initial version. (from MTL)
  <b>Revision 2:</b>
  - Added MopPackages, MopDensity, MopRanks, MopVendor fields
**/
typedef struct {
  UINT8             Revision;
  UINT16            DataWidth;              ///< Data width, in bits, of this memory device
  /** As defined in SMBIOS 3.0 spec
    Section 7.18.2 and Table 75
  **/
  UINT8             MemoryType;             ///< DDR type: DDR3, DDR4, or LPDDR3
  UINT16            MaximumMemoryClockSpeed;///< The maximum capable speed of the device, in megahertz (MHz)
  UINT16            ConfiguredMemoryClockSpeed; ///< The configured clock speed to the memory device, in megahertz (MHz)
  /** As defined in SMBIOS 3.0 spec
    Section 7.17.3 and Table 72
  **/
  UINT8             ErrorCorrectionType;

  SiMrcVersion      Version;
  BOOLEAN           EccSupport;
  UINT8             MemoryProfile;
  UINT32            TotalPhysicalMemorySize;
  UINT32            DefaultXmptCK[MAX_XMP_PROFILE_NUM];///< Stores the tCK value read from SPD XMP profiles if they exist.
  UINT8             XmpProfileEnable;                  ///< If XMP capable DIMMs are detected, this will indicate which XMP Profiles are common among all DIMMs.
  UINT8             XmpConfigWarning;                  ///< If XMP capable DIMMs config support only 1DPC, but 2DPC is installed
  BOOLEAN           DynamicMemoryBoostTrainingFailed;  ///< TRUE if Dynamic Memory Boost failed to train and was force disabled on the last full training boot. FALSE otherwise.
  UINT16            Ratio;                             ///< DDR Frequency Ratio, used for programs that require ratios higher then 255
  UINT8             RefClk;
  UINT32            VddVoltage[MAX_PROFILE_NUM];
  UINT32            VddqVoltage[MAX_PROFILE_NUM];
  UINT32            VppVoltage[MAX_PROFILE_NUM];
  UINT16            RcompTarget[MAX_PROFILE_NUM][MAX_RCOMP_TARGETS];
  UINT16            DimmOdt[MAX_PROFILE_NUM][MAX_DIMM][MAX_ODT_ENTRIES];
  INT8              DimmDFE[MAX_PROFILE_NUM][MAX_DDR5_CH][MAX_DIMM][MAX_COPY_DIMM_DFE_TAPS];
  CONTROLLER_INFO   Controller[MAX_NODE];
  UINT32            NumPopulatedChannels;              ///< Total number of memory channels populated
  HOB_SAGV_INFO     SagvConfigInfo;                    ///< This data structure contains SAGV config values that are considered output by the MRC.
  BOOLEAN           IsIbeccEnabled;
  UINT16            TotalMemWidth;                     ///< Total Memory Width in bits from all populated channels
  UINT8             MopPackages;                       ///< Mop DRAM package population
  UINT8             MopDensity;                        ///< Mop DRAM die density
  UINT8             MopRanks;                          ///< Mop Number of ranks
  UINT8             MopVendor;                         ///< Mop DRAM vendor ID
  UINT8             PprRanInLastBoot;                  ///< Whether PPR ran in the prior boot
  UINT16            PprDetectedErrors;                 ///< PPR: Counts of detected bad rows
  UINT16            PprRepairFails;                    ///< PPR: Counts of repair failure
  UINT16            PprForceRepairStatus;              ///< PPR: Force Repair Status
  UINT16            PprRepairsSuccessful;              ///< PPR: Counts of repair successes
  PPR_RESULT_COLUMNS_HOB PprErrorInfo;                 ///< PPR: Error location
  UINT8             PprAvailableResources[MAX_NODE][MAX_CH][MAX_RANK_IN_CHANNEL][MAX_SDRAM_IN_DIMM]; ///< PPR available resources per device
} MEMORY_INFO_DATA_HOB;

/**
  Memory Platform Data Hob
  <b>Revision 1:</b>
  - Initial version.
  <b>Revision 2:</b>
  - Added TsegBase, PrmrrSize, PrmrrBase, Gttbase, MmioSize, PciEBaseAddress fields
**/
typedef struct {
  UINT8             Revision;
} MEMORY_PLATFORM_DATA;

typedef struct {
  EFI_HOB_GUID_TYPE    EfiHobGuidType;
  MEMORY_PLATFORM_DATA Data;
  UINT8                *Buffer;
} MEMORY_PLATFORM_DATA_HOB;

#pragma pack (pop)

#endif // _MEM_INFO_HOB_H_
