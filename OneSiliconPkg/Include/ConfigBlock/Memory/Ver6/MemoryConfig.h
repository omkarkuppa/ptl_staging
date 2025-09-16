/** @file
  Policy definition of Memory Config Block
  Used by projects: PantherLake

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#ifndef _MEMORY_CONFIG_H_
#define _MEMORY_CONFIG_H_

#include <ConfigBlock.h>
#include "Ptl/CMrcExtInputs.h"
#include "Ptl/CMrcExtTypes.h"


#pragma pack(push, 1)

// MEMORY_CONFIGURATION_REVISION:
// Keep in sync with MRC_EXT_INPUTS_REVISION in MrcExtInputs.h
#define MEMORY_CONFIGURATION_REVISION 1
#define MEMORY_CONFIG_NO_CRC_REVISION 5

///
/// Memory SubSystem Definitions
///
#define MEM_CFG_MAX_CONTROLLERS          MAX_CONTROLLER
#define MEM_CFG_MAX_CHANNELS             MAX_CHANNEL
#define MEM_CFG_MAX_CHANNEL_SHARE_REGS   2
#define MEM_CFG_MAX_DIMMS                EXT_MAX_DIMMS_IN_CHANNEL
#define MEM_CFG_MAX_RANKS_PER_DIMM       MAX_RANK_IN_DIMM
#define MEM_CFG_NUM_BYTES_MAPPED         2
#define MEM_CFG_MAX_SPD_SIZE             1024
#define MEM_CFG_MAX_SOCKETS              (MEM_CFG_MAX_CONTROLLERS * MEM_CFG_MAX_CHANNELS * MEM_CFG_MAX_DIMMS)
#define MEM_CFG_MAX_DDR5_SOCKETS         (MEM_CFG_MAX_CONTROLLERS * 2 * MEM_CFG_MAX_DIMMS)
#define MEM_CFG_MAX_ROWS                 (MEM_CFG_MAX_RANKS_PER_DIMM * MEM_CFG_MAX_SOCKETS)
#ifndef MEM_MAX_SAGV_POINTS
#define MEM_MAX_SAGV_POINTS              MAX_SAGV_POINTS
#endif
#define MEM_MAX_IBECC_REGIONS            8
///
/// SMRAM Memory Range
///
#define PEI_MR_SMRAM_ABSEG_MASK     0x01
#define PEI_MR_SMRAM_HSEG_MASK      0x02

///
/// SA SPD profile selections.
///
typedef enum {
  Default,             ///< 0, Default SPD
  UserDefined,         ///< 1, User Defined profile
  XMPProfile1,         ///< 2, XMP Profile 1
  XMPProfile2,         ///< 3, XMP Profile 2
  XMPProfileMax = 0xFF ///< Ensures SA_SPD is UINT8
} SA_SPD;

///
/// Define the boot modes used by the SPD read function.
///
typedef enum {
  SpdCold,       ///< Cold boot
  SpdWarm,       ///< Warm boot
  SpdS3,         ///< S3 resume
  SpdFast,       ///< Fast boot
  SpdBootModeMax ///< Delimiter
} SPD_BOOT_MODE;

/**
  SPD Data Buffer
**/
typedef struct {
  UINT8 SpdData[MEM_CFG_MAX_CONTROLLERS][MEM_CFG_MAX_CHANNELS][MEM_CFG_MAX_DIMMS][MEM_CFG_MAX_SPD_SIZE];  ///< SpdData
//Next Field Offset 2048
} SPD_DATA_BUFFER;

/**
  DqDqs Mapping
**/
typedef struct {
  UINT8 DqsMapCpu2Dram[MEM_CFG_MAX_CONTROLLERS][MEM_CFG_MAX_CHANNELS][MEM_CFG_NUM_BYTES_MAPPED];  ///< DqsMapCpu2Dram
  UINT8 DqMapCpu2Dram[MEM_CFG_MAX_CONTROLLERS][MEM_CFG_MAX_CHANNELS][MEM_CFG_NUM_BYTES_MAPPED][8];  ///< DqMapCpu2Dram
//Next Field Offset 16
} MEMORY_DQDQS_MAPPING;

/**
  Rcomp Policies
**/
typedef struct {
  UINT16  RcompResistor;                      ///< Offset 0: Reference RCOMP resistors on motherboard ~ 100 ohms
  UINT16  RcompTarget[MAX_RCOMP_TARGETS];     ///< Offset 1: RCOMP target values for DqOdt, DqDrv, CmdDrv, CtlDrv, ClkDrv
//Next Field Offset 16
} MEMORY_RCOMP;

/**
  SPD Offset Table
**/
typedef struct {
  UINT16 Start;           ///< Offset 0
  UINT16 End;             ///< Offset 2
  UINT8  BootMode;        ///< Offset 4
  UINT8  Reserved3[3];    ///< Offset 5 Reserved for future use
} SPD_OFFSET_TABLE;

///
/// SA memory address decode.
///
typedef struct
{
  UINT8  Controller; ///< Offset 0 Zero based Controller number
  UINT8  Channel;    ///< Offset 1 Zero based Channel number
  UINT8  Dimm;       ///< Offset 2 Zero based DIMM number
  UINT8  Rank;       ///< Offset 3 Zero based Rank number
  UINT8  BankGroup;  ///< Offset 4 Zero based Bank Group number
  UINT8  Bank;       ///< Offset 5 Zero based Bank number
  UINT16 Cas;        ///< Offset 6 Zero based CAS number
  UINT32 Ras;        ///< Offset 8 Zero based RAS number
} SA_ADDRESS_DECODE;

/**
 Memory Configuration
 The contents of this structure are CRC'd by the MRC for option change detection.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;    ///< Offset 0-27 Config Block Header
  MRC_EXT_INPUTS_TYPE  ExternalInputs;
} MEMORY_CONFIGURATION;

/// Memory Configuration
/// The contents of this structure are not CRC'd by the MRC for option change detection.
/// <b>Revision 1</b>:  - Initial version.
/// <b>Revision 5</b>:  - Added CkdAddressTable
typedef struct {
  CONFIG_BLOCK_HEADER      Header;              ///< Offset 0-27 Config Block Header
  UINT8                    RsvdBytes151[4];     ///< Offset 28-31 Reserved for alignment
  SPD_DATA_BUFFER          *SpdData;            ///< Offset 32  Memory SPD data, will be used by the MRC when SPD SmBus address is zero.
  MEMORY_DQDQS_MAPPING     *DqDqsMap;           ///< Offset 40  LPDDR DQ bit and DQS byte swizzling between CPU and DRAM.
  MEMORY_RCOMP             *RcompData;          ///< Offset 48  DDR RCOMP resistors and target values.
  UINT64                   PlatformMemorySize;  ///< Offset 56  The minimum platform memory size required to pass control into DXE
  UINT32                   CleanMemory:1;       ///< Offset 64  Ask MRC to clear memory content: <b>FALSE=Do not Clear Memory</b>; TRUE=Clear Memory
  UINT32                   ReservedBits5:31;
  /**
   Sets the serial debug message level\n
     0x00 = Disabled\n
     0x01 = Errors only\n
     0x02 = Errors and Warnings\n
     <b>0x03 = Errors, Warnings, and Info</b>\n
     0x04 = Errors, Warnings, Info, and Events\n
     0x05 = Displays Memory Init Execution Time Summary only\n
  **/
  UINT8   SerialDebugLevel;                     ///< Offset 68
  UINT8   MemTestOnWarmBoot;                    ///< Offset 69  Run Base Memory Test On WarmBoot:  0=Disabled, <b>1=Enabled</b>
  UINT8   DisableMrcRetraining;                 ///< Offset 70 Enable/Disable DisableMrcRetraining.
  /**
    Offset 71 Memory DIMMs' SPD address for reading SPD data.
      0 - Controller 0 Channel 0 Dimm 0 - DDR5 ---- LPDDR5
      1 - Controller 0 Channel 0 Dimm 1 - DDR5 2DPC
      2 - Controller 0 Channel 1 Dimm 0 - DDR5 ---- LPDDR5
      3 - Controller 0 Channel 1 Dimm 1 - DDR5 2DPC
      4 - Controller 0 Channel 2 Dimm 0 ----------- LPDDR5
      5 - Controller 0 Channel 2 Dimm 1 ------------------
      6 - Controller 0 Channel 3 Dimm 0 ----------- LPDDR5
      7 - Controller 0 Channel 3 Dimm 1 ------------------
      8 - Controller 1 Channel 0 Dimm 0 - DDR5 ---- LPDDR5
      9 - Controller 1 Channel 0 Dimm 1 - DDR5 2DPC
     10 - Controller 1 Channel 1 Dimm 0 - DDR5 ---- LPDDR5
     11 - Controller 1 Channel 1 Dimm 1 - DDR5 2DPC
     12 - Controller 1 Channel 2 Dimm 0 ----------- LPDDR5
     13 - Controller 1 Channel 2 Dimm 1 ------------------
     14 - Controller 1 Channel 3 Dimm 0 ----------- LPDDR5
     15 - Controller 1 Channel 3 Dimm 1 ------------------
  **/
  UINT8   SpdAddressTable[MEM_CFG_MAX_SOCKETS];
  UINT8   MrcBdatEnable;                     ///< Offset 87 This field enables the generation of the BIOS DATA ACPI Tables: <b>0=FALSE</b>, 1=TRUE.
  UINT8   MrcBdatTestType;                   ///< Offset 88 When BdatEnable is set to TRUE, this option selects the type of data which will be populated in the BIOS Data ACPI Tables: <b>0=RMT</b>, 1=RMT Per Bit, 2=Margin 2D.

  UINT8   SafeLoadingBiosEnableState;        ///< Offset 89 Safe Loading Bios Enable State
  UINT8   PprRecoveryStatusEnable;           ///< Offset 90 Ppr Recovery Status Enable
  UINT8   TsegMemoryTestStatus;              ///< Offset 91 Tseg Memory Test Status
  UINT8   MrcPprStatus;                      ///< Offset 92 Mrc Ppr Status
  UINT8   RetryCount;                        ///< Offset 93 TSEG RetryCount
  /**
  ///< Offset 94 CKD QCK to Controller/Channel/DIMM Mapping.
  This mapping is aligned with CkdAddressTable[16], but skipping channel 2/3 (LPDDR) instances.
  Encoding:
    0 - QCK[0:1]_A
    1 - QCK[0:1]_B
    Keep as 0 for unused since we won't be using the instance if it's non-CKD/unpopulated, so the encoding doesn't matter.
  Array Index Mapping
    0 - Controller 0 Channel 0 Dimm 0
    1 - Controller 0 Channel 0 Dimm 1
    2 - Controller 0 Channel 1 Dimm 0
    3 - Controller 0 Channel 1 Dimm 1
    4 - Controller 1 Channel 0 Dimm 0
    5 - Controller 1 Channel 0 Dimm 1
    6 - Controller 1 Channel 1 Dimm 0
    7 - Controller 1 Channel 1 Dimm 1
  **/
  UINT8 ChannelToCkdQckMapping[MEM_CFG_MAX_DDR5_SOCKETS];
  /**
  ///< Offset 102 DDRIO to CKD Clock.
  This mapping is aligned with CkdAddressTable[16], but skipping channel 2/3 (LPDDR) instances.
  This is a 0-based index which identifies the Phy Clock this Controller/Channel/DIMM is using.
  For Channels mapping to the same CKD DIMM, the encoding will be the same.
  The following requirements exist for selecting a CKD Clock:
    1. There must be two instances of the clock requested and no more.
         A CKD DIMM must service two channels
    2. The clock selected must be in the channel that is servicing the CKD DIMM
    3. This policy must have two Channels of the same DIMM mapping to the same Phy Clock.
    4. In the case of CKD and Non-CKD mixed 2DPC systems:
       a. If the CKD DIMM is DIMM 0, user must map Rank 0 or 1 from either Channel attached to the DIMM. If the CKD DIMM is DIMM 1, user must map Rank 2 or 3 from either Channel attached to the CKD DIMM.
       b. If the Non-CKD DIMM is in DIMM 0, Rank 0 must be routed to the DIMM Rank 0 and Rank 1 must be routed to the DIMM Rank 1.  If the Non-CKD DIMM is in DIMM 1, Rank 2 must be routed to DIMM Rank 2, and Rank 3 must be routed to DIMM Rank 3.  All ranks must be from the Channel the Non-CKD DIMM is attached.
  Encoding: a - xCLKy where a is the array index, x is Phy channel, and y is Phy Rank.
    0  - 0CLK0, 4  - 1CLK0, 8  - 2CLK0, 12 - 3CLK0
    1  - 0CLK1, 5  - 1CLK1, 9  - 2CLK1, 13 - 3CLK1
    2  - 0CLK2, 6  - 1CLK2, 10 - 2CLK2, 14 - 3CLK2
    3  - 0CLK3, 7  - 1CLK3, 11 - 2CLK3, 15 - 3CLK3
    Keep as 0 for unused since we won't be using the instance if it's non-CKD/unpopulated, so the encoding doesn't matter.
  Array Index Mapping:
    0 - Controller 0 Channel 0 Dimm 0
    1 - Controller 0 Channel 0 Dimm 1
    2 - Controller 0 Channel 1 Dimm 0
    3 - Controller 0 Channel 1 Dimm 1
    4 - Controller 1 Channel 0 Dimm 0
    5 - Controller 1 Channel 0 Dimm 1
    6 - Controller 1 Channel 1 Dimm 0
    7 - Controller 1 Channel 1 Dimm 1
  **/
  UINT8                 PhyClockToCkdDimm[MEM_CFG_MAX_DDR5_SOCKETS];

  UINT8                 RsvdBytes106[2];                  ///< Offset 110 Reserved bytes for struct natural alignment
  MRC_PPR_ENTRY_INFO    PprEntryInfo[PPR_REQUEST_MAX];    ///< Offset 112 PPR Repair Info
  MRC_PPR_ENTRY_ADDRESS PprEntryAddress[PPR_REQUEST_MAX]; ///< Offset 128 PPR Repair Memory Address
  MRC_PPR_TEST_TYPE     PprTestType;       ///< Offset 144 PPR Test Type, enable or disable memory tests in the PPR flow
  MRC_PPR_REPAIR_TYPE   PprRepairType;     ///< Offset 152 PPR Repair Type: 0 = Do not repair, 1 = Soft PPR, 2 = Hard PPR
  UINT8                 PprRunOnce;        ///< Offset 156 PPR Run Once, used to un-set PPR Policies after PPR execution is completed
  UINT8                 PprErrorInjection; ///< Offset 157 PPR Error Injection
  UINT8                 PprForceRepair;    ///< Offset 158 PPR Force Repair Some Rows
  UINT8                 PprRetryLimit;     ///< Offset 159 PPR Retry Limit
  UINT8                 CAVrefCtlOffset;   ///< Offset 160
  UINT8                 VrefCtlOffset;     ///< Offset 161
  UINT8                 ClkPiCodeOffset;   ///< Offset 162
  UINT8                 RcvEnOffset;       ///< Offset 163
  UINT8                 RxDqsOffset;       ///< Offset 164
  UINT8                 TxDqOffset;        ///< Offset 165
  UINT8                 TxDqsOffset;       ///< Offset 166
  UINT8                 VrefOffset;        ///< Offset 167
  UINT8                 CntrlrMask;        ///< Offset 168
  UINT8                 ChMask;            ///< Offset 169
  /**
    ///< Offset 170 Memory DIMMs' CKD address for read/write CKD data.
      0 - Controller 0 Channel 0 Dimm 0 - DDR5 ---- LPDDR5
      1 - Controller 0 Channel 0 Dimm 1 - DDR5 2DPC
      2 - Controller 0 Channel 1 Dimm 0 - DDR5 ---- LPDDR5
      3 - Controller 0 Channel 1 Dimm 1 - DDR5 2DPC
      4 - Controller 0 Channel 2 Dimm 0 ----------- LPDDR5
      5 - Controller 0 Channel 2 Dimm 1 ------------------
      6 - Controller 0 Channel 3 Dimm 0 ----------- LPDDR5
      7 - Controller 0 Channel 3 Dimm 1 ------------------
      8 - Controller 1 Channel 0 Dimm 0 - DDR5 ---- LPDDR5
      9 - Controller 1 Channel 0 Dimm 1 - DDR5 2DPC
     10 - Controller 1 Channel 1 Dimm 0 - DDR5 ---- LPDDR5
     11 - Controller 1 Channel 1 Dimm 1 - DDR5 2DPC
     12 - Controller 1 Channel 2 Dimm 0 ----------- LPDDR5
     13 - Controller 1 Channel 2 Dimm 1 ------------------
     14 - Controller 1 Channel 3 Dimm 0 ----------- LPDDR5
     15 - Controller 1 Channel 3 Dimm 1 ------------------
  **/
  UINT8  CkdAddressTable[MEM_CFG_MAX_SOCKETS];
  UINT8                 Reserved[6];       ///< Offset 186 Reserved bytes for struct alignment
} MEMORY_CONFIG_NO_CRC;
#pragma pack(pop)

#endif // _MEMORY_CONFIG_H_
