/** @file
  This protocol provides the memory information data, such as
  total physical memory size, memory frequency, memory size
  of each dimm and rank.

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
#ifndef _MEM_INFO_PROTOCOL_H_
#define _MEM_INFO_PROTOCOL_H_


//
// Extern the GUID for protocol users.
//
extern EFI_GUID gMemInfoProtocolGuid;

//
// Protocol definitions
//
#define NODE_NUM            2
#define CH_NUM              4
#define DDR5_CH_NUM         2
#define DIMM_NUM            2
#define RANK_NUM            2
#define SLOT_NUM            (NODE_NUM * CH_NUM * DIMM_NUM)
#define PROFILE_NUM         7 // number of memory profiles supported
#ifndef RCOMP_TARGETS_NUM
#define RCOMP_TARGETS_NUM   5
#endif

#ifndef DIMM_ODT_NUM
#define DIMM_ODT_NUM        11
#endif

// Copy only Tap1 and Tap2
#ifndef COPY_DFE_TAP_NUM
#define COPY_DFE_TAP_NUM        2
#endif

#define IBECC_SIZE_NUM      14

//
// Matches MrcDdrType enum in MRC
//
#ifndef MRC_DDR_TYPE_LPDDR5
#define MRC_DDR_TYPE_LPDDR5   0
#endif
#ifndef MRC_DDR_TYPE_DDR5
#define MRC_DDR_TYPE_DDR5     1
#endif
#ifndef MRC_DDR_TYPE_UNKNOWN
#define MRC_DDR_TYPE_UNKNOWN  2
#endif

//
// Matches MrcChannelSts enum in MRC
//
#ifndef CHANNEL_NOT_PRESENT
#define CHANNEL_NOT_PRESENT     0  // There is no channel present on the controller.
#endif
#ifndef CHANNEL_DISABLED
#define CHANNEL_DISABLED        1  // There is a channel present but it is disabled.
#endif
#ifndef CHANNEL_PRESENT
#define CHANNEL_PRESENT         2  // There is a channel present and it is enabled.
#endif

//
// Matches MrcDimmSts enum in MRC
//
#ifndef DIMM_ENABLED
#define DIMM_ENABLED     0  // DIMM/rank Pair is enabled, presence will be detected.
#endif
#ifndef DIMM_DISABLED
#define DIMM_DISABLED    1  // DIMM/rank Pair is disabled, regardless of presence.
#endif
#ifndef DIMM_PRESENT
#define DIMM_PRESENT     2  // There is a DIMM present in the slot/rank pair and it will be used.
#endif
#ifndef DIMM_NOT_PRESENT
#define DIMM_NOT_PRESENT 3  // There is no DIMM present in the slot/rank pair.
#endif

#pragma pack(1)
///
/// Memory timing Structure
///
typedef struct {
  UINT32 tCK;     ///< Offset 0  Memory cycle time, in femtoseconds.
  UINT16 NMode;   ///< Offset 4  Number of tCK cycles for the channel DIMM's command rate mode.
  UINT16 tCL;     ///< Offset 6  Number of tCK cycles for the channel DIMM's CAS latency.
  UINT16 tCWL;    ///< Offset 8  Number of tCK cycles for the channel DIMM's minimum CAS write latency time.
  UINT16 tFAW;    ///< Offset 10 Number of tCK cycles for the channel DIMM's minimum four activate window delay time.
  UINT16 tRAS;    ///< Offset 12 Number of tCK cycles for the channel DIMM's minimum active to precharge delay time.
  UINT16 tRCDtRP; ///< Offset 14 Number of tCK cycles for the channel DIMM's minimum RAS# to CAS# delay time and Row Precharge delay time.
  UINT32 tREFI;   ///< Offset 16 Number of tCK cycles for the channel DIMM's minimum Average Periodic Refresh Interval.
  UINT16 tRFC;    ///< Offset 20 Number of tCK cycles for the channel DIMM's minimum refresh recovery delay time.
  UINT16 tRFCpb;  ///< Offset 22 Number of tCK cycles for the channel DIMM's minimum per bank refresh recovery delay time.
  UINT16 tRFC2;   ///< Offset 24 Number of tCK cycles for the channel DIMM's minimum refresh recovery delay time.
  UINT16 tRFC4;   ///< Offset 26 Number of tCK cycles for the channel DIMM's minimum refresh recovery delay time.
  UINT16 tRPab;   ///< Offset 28 Number of tCK cycles for the channel DIMM's minimum row precharge delay time for all banks.
  UINT16 tRRD;    ///< Offset 30 Number of tCK cycles for the channel DIMM's minimum row active to row active delay time.
  UINT16 tRRD_L;  ///< Offset 32 Number of tCK cycles for the channel DIMM's minimum row active to row active delay time for same bank groups.
  UINT16 tRRD_S;  ///< Offset 34 Number of tCK cycles for the channel DIMM's minimum row active to row active delay time for different bank groups.
  UINT16 tRTP;    ///< Offset 36 Number of tCK cycles for the channel DIMM's minimum internal read to precharge command delay time.
  UINT16 tWR;     ///< Offset 38 Number of tCK cycles for the channel DIMM's minimum write recovery time.
  UINT16 tWTR;    ///< Offset 40 Number of tCK cycles for the channel DIMM's minimum internal write to read command delay time.
  UINT16 tWTR_L;  ///< Offset 42 Number of tCK cycles for the channel DIMM's minimum internal write to read command delay time for same bank groups.
  UINT16 tWTR_S;  ///< Offset 44 Number of tCK cycles for the channel DIMM's minimum internal write to read command delay time for different bank groups.
  UINT16 tCCD_L;  ///< Offset 46 Number of tCK cycles for the channel DIMM's minimum CAS-to-CAS delay for same bank group.
  UINT16 tCCD_L_WR; ///< Offset 48 Number of tCK cycles for the channel DIMM's minimum Write-to-Write delay for same bank group.
  UINT8  Resv[2]; ///< Offset 50 Reserved.
} MEMORY_TIMING;

// @todo use the MemInfoHob data instead of duplicate structure.
///
/// Memory information Data Structure
///
typedef struct {
  MEMORY_TIMING Timing[PROFILE_NUM];                   ///< Offset 0 Timming information for the DIMM
  UINT32  memSize;                                     ///< Offset 364: Total physical memory size
  UINT16  ddrFreq;                                     ///< Offset 368: DDR Current Frequency
  UINT16  ddrFreqMax;                                  ///< Offset 370: DDR Maximum Frequency
  UINT8   DdrType;                                     ///< Offset 372: Current DDR type, see DDR_TYPE_xxx defines above
  BOOLEAN EccSupport;                                  ///< Offset 373: ECC supported or not
  UINT8   Profile;                                     ///< Offset 374: Currently running memory profile
  UINT8   RefClk;                                      ///< Offset 375: Reference Clock
  UINT16  Ratio;                                       ///< Offset 376: Clock Multiplier UINT16
  UINT8   XmpProfileEnable;                            ///< Offset 378: 0 = no XMP DIMMs in system
  BOOLEAN XmpConfigWarning;                            ///< Offset 379: XMP Dimm Config Warning - 1DPC capable but 2DPC is installed
  UINT32  dimmSize[NODE_NUM * CH_NUM * DIMM_NUM];      ///< Offset 380: Size of each DIMM
  UINT8   DimmStatus[NODE_NUM * CH_NUM * DIMM_NUM];    ///< Offset 412: The enumeration value from MrcDimmSts
  UINT8   RankInDimm[NODE_NUM * CH_NUM * DIMM_NUM];    ///< Offset 428: No. of ranks in a dimm
  UINT8   *DimmsSpdData[NODE_NUM * CH_NUM * DIMM_NUM]; ///< Offset 444: SPD data of each DIMM
  UINT16  VddVoltage[PROFILE_NUM];                     ///< Offset 572: The voltage setting for the DIMM
  UINT16  VddqVoltage[PROFILE_NUM];                    ///< Offset 586: The voltage setting for the DIMM
  UINT16  VppVoltage[PROFILE_NUM];                     ///< Offset 600: The voltage setting for the DIMM
  BOOLEAN DynamicMemoryBoostTrainingFailed;            ///< Offset 614: if Dynamic Memory Boost failed to train and was force disabled on the last full training boot. FALSE otherwise.
  UINT8   Reserved587;                                 ///< Offset 615: Reserved
  UINT16  RcompTarget[PROFILE_NUM][RCOMP_TARGETS_NUM];    ///< Offset 616: Per-Profile Default RcompTarget
  UINT16  DimmOdt[PROFILE_NUM][DIMM_NUM][DIMM_ODT_NUM];   ///< Offset 686: Per-Profile Default Dimm ODT values
  INT8    DimmDFE[PROFILE_NUM][DDR5_CH_NUM][DIMM_NUM][COPY_DFE_TAP_NUM];  ///< Offset 994: Per-Profile Default Dimm DFE values
  UINT8   MopVendor;                                   ///< Offset 1050: Mop DRAM vendor ID
  UINT16  PprDetectedErrors;                           ///< Offset 1051: PPR: Counts of detected bad rows.
  UINT16  PprRepairFails;                              ///< Offset 1053: PPR: Counts of repair failure.
  UINT16  PprForceRepairStatus;                        ///< Offset 1055: PPR: Force Repair Status.
} MEMORY_INFO_DATA;
#pragma pack()

///
/// Memory information Protocol definition
///
typedef struct {
  MEMORY_INFO_DATA  MemInfoData; ///< Memory Information Data Structure
} MEM_INFO_PROTOCOL;

#endif
