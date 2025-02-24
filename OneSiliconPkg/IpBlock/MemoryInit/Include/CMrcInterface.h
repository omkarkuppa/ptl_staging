/** @file
  This file defines the portion of the BIOS-MRC interface that is common
  to both the Platform and the MRC core.

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
#ifndef MRC_INTERFACE_H
#define MRC_INTERFACE_H
#include "MrcSpdData.h"
#include "CMrcInterfaceGlobalTypes.h"

#include <Ptl/CMrcExtTypes.h>


#ifdef PRODUCT_RZLWS
#include <Nvl/CMrcExtTypes.h>
#endif

#include <Ptl/MrcGenSaveRestoreRegCount.h>


#ifdef PRODUCT_RZLWS
#include <Nvl/MrcGenSaveRestoreRegCount.h>
#endif

#define CPUID_FULL_FAMILY_MODEL_LUNARLAKE_MOBILE 0x000B06D0

///
/// DRAM Mode register numbers.
/// mrMR0 till mrMR6 can also be used as index into DimmOut->MR[],
/// for other MRs use MrcModeRegisterIndex enum.
///
typedef enum {
  mrMR0  = 0,
  mrMR1  = 1,
  mrMR2  = 2,
  mrMR3  = 3,
  mrMR4  = 4,
  mrMR5  = 5,
  mrMR6  = 6,
  mrMR7  = 7,
  mrMR8  = 8,
  mrMR10 = 10,
  mrMR11 = 11,
  mrMR12 = 12,
  mrMR13 = 13,
  mrMR14 = 14,
  mrMR15 = 15,
  mrMR16 = 16,
  mrMR17 = 17,
  mrMR18 = 18,
  mrMR19 = 19,
  mrMR20 = 20,
  mrMR21 = 21,
  mrMR22 = 22,
  mrMR23 = 23,
  mrMR24 = 24,
  mrMR25 = 25,
  mrMR26 = 26,
  mrMR27 = 27,
  mrMR28 = 28,
  mrMR29 = 29,
  mrMR30 = 30,
  mrMR31 = 31,
  mrMR32 = 32,
  mrMR34 = 34,
  mrMR35 = 35,
  mrMR36 = 36,
  mrMR37 = 37,
  mrMR38 = 38,
  mrMR39 = 39,
  mrMR40 = 40,
  mrMR41 = 41,
  mrMR42 = 42,
  mrMR45 = 45,
  mrMR46 = 46,
  mrMR48 = 48,
  mrMR50 = 50,
  mrMR54 = 54,
  mrMR55 = 55,
  mrMR56 = 56,
  mrMR57 = 57,
  mrMR58 = 58,
  mrMR59 = 59,
  mrMR69 = 69,
  mrMR75 = 75,
  mrDuplicateMrInstance = 100, ///< This is the delimiter for MR's where we have the same MR address but different values in a MR sequence.
  mrMR111 = 111,
  mrMR12b = 120,         ///< This is a special case to handle ByteMode instance of Upper device for CA Vref.
  mrMR16FspOp = 121,     ///< This is a special case to handle FSP-OP Toggle in LP5 SAGV flow.
  mrMR28ZqReset = 122,   ///< This is a special case to handle ZQ Reset in LP5 SAGV flow.
  mrMR28ZqStop = 123,    ///< This is a special case to set ZQ stop in LP5 SAGV flow.
  mrREFab = 124,         ///< This is a special case to issue REFab via MRS FSM.
  mrPreAll= 125,         ///< This is a special case to issue PreAll via MRS FSM.
  mrMR127 = 127,         ///< This is a special case to issue CAS-OFF via MRS FSM by sending MRW to the non-existing MR127.
  mrMR129 = 129, ///< DFE MR DQL
  mrMR130 = 130, ///< DFE MR DQL
  mrMR131 = 131, ///< DFE MR DQL
  mrMR132 = 132, ///< DFE MR DQL
  mrMR137 = 137, ///< DFE MR DQL
  mrMR138 = 138, ///< DFE MR DQL
  mrMR139 = 139, ///< DFE MR DQL
  mrMR140 = 140, ///< DFE MR DQL
  mrMR145 = 145, ///< DFE MR DQL
  mrMR146 = 146, ///< DFE MR DQL
  mrMR147 = 147, ///< DFE MR DQL
  mrMR148 = 148, ///< DFE MR DQL
  mrMR153 = 153, ///< DFE MR DQL
  mrMR154 = 154, ///< DFE MR DQL
  mrMR155 = 155, ///< DFE MR DQL
  mrMR156 = 156, ///< DFE MR DQL
  mrMR161 = 161, ///< DFE MR DQL
  mrMR162 = 162, ///< DFE MR DQL
  mrMR163 = 163, ///< DFE MR DQL
  mrMR164 = 164, ///< DFE MR DQL
  mrMR169 = 169, ///< DFE MR DQL
  mrMR170 = 170, ///< DFE MR DQL
  mrMR171 = 171, ///< DFE MR DQL
  mrMR172 = 172, ///< DFE MR DQL
  mrMR177 = 177, ///< DFE MR DQL
  mrMR178 = 178, ///< DFE MR DQL
  mrMR179 = 179, ///< DFE MR DQL
  mrMR180 = 180, ///< DFE MR DQL
  mrMR185 = 185, ///< DFE MR DQL
  mrMR186 = 186, ///< DFE MR DQL
  mrMR187 = 187, ///< DFE MR DQL
  mrMR188 = 188, ///< DFE MR DQL
  mrMR193 = 193, ///< DFE MR DQU
  mrMR194 = 194, ///< DFE MR DQU
  mrMR195 = 195, ///< DFE MR DQU
  mrMR196 = 196, ///< DFE MR DQU
  mrMR201 = 201, ///< DFE MR DQU
  mrMR202 = 202, ///< DFE MR DQU
  mrMR203 = 203, ///< DFE MR DQU
  mrMR204 = 204, ///< DFE MR DQU
  mrMR209 = 209, ///< DFE MR DQU
  mrMR210 = 210, ///< DFE MR DQU
  mrMR211 = 211, ///< DFE MR DQU
  mrMR212 = 212, ///< DFE MR DQU
  mrMR217 = 217, ///< DFE MR DQU
  mrMR218 = 218, ///< DFE MR DQU
  mrMR219 = 219, ///< DFE MR DQU
  mrMR220 = 220, ///< DFE MR DQU
  mrMR225 = 225, ///< DFE MR DQU
  mrMR226 = 226, ///< DFE MR DQU
  mrMR227 = 227, ///< DFE MR DQU
  mrMR228 = 228, ///< DFE MR DQU
  mrMR233 = 233, ///< DFE MR DQU
  mrMR234 = 234, ///< DFE MR DQU
  mrMR235 = 235, ///< DFE MR DQU
  mrMR236 = 236, ///< DFE MR DQU
  mrMR241 = 241, ///< DFE MR DQU
  mrMR242 = 242, ///< DFE MR DQU
  mrMR243 = 243, ///< DFE MR DQU
  mrMR244 = 244, ///< DFE MR DQU
  mrMR249 = 249, ///< DFE MR DQU
  mrMR250 = 250, ///< DFE MR DQU
  mrMR251 = 251, ///< DFE MR DQU
  mrMR252 = 252, ///< DFE MR DQU
  mpcMR13             = 256, // MRC_UINT8_MAX + 1
  mpcMR32a0           = 256 + 1,
  mpcMR32a1           = 256 + 2,
  mpcMR32b0           = 256 + 3,
  mpcMR32b1           = 256 + 4,
  mpcMR33a0           = 256 + 5,
  mpcMR33             = 256 + 6,
  mpcMR33b0           = 256 + 7,
  mpcMR34             = 256 + 8,
  mpcApplyVrefCa      = 256 + 9,
  mpcDllReset         = 256 + 10,
  mpcZqCal            = 256 + 11,
  mpcZqLat            = 256 + 12,
  mpcEnterCaTrainMode = 256 + 13,
  mpcSetCmdTiming     = 256 + 14,
  mpcSelectAllPDA     = 256 + 15,
  mrEndOfSequence     = MRC_UINT16_MAX // Sentinel value for MRW\MPC sequence arrays.
} MrcModeRegister;
#define MRC_MR_DUP_SCALE  (10)

///
/// DRAM Mode register indices in DimmOut
///
typedef enum {
  mrIndexMR0,
  mrIndexMR1,
  mrIndexMR2,
  mrIndexMR3,
  mrIndexMR4,
  mrIndexMR5,
  mrIndexMR6,
  mrIndexMR7,
  mrIndexMR8,
  mrIndexMR10,
  mrIndexMR11,
  mrIndexMR12,
  mrIndexMR12Upper, ///< This is specific to LPDDR5 where we can configure CA Vref on each Byte with Byte-Mode devices.
  mrIndexMR13,
  mrIndexMR14,
  mrIndexMR15,
  mrIndexMR16,
  mrIndexMR17,
  mrIndexMR18,
  mrIndexMR19,
  mrIndexMR20,
  mrIndexMR21,
  mrIndexMR22,
  mrIndexMR23,
  mrIndexMR24,
  mrIndexMR25,
  mrIndexMR26,
  mrIndexMR27,
  mrIndexMR28,
  mrIndexMR29,
  mrIndexMR30,
  mrIndexMR34,
  mrIndexMR35,
  mrIndexMR36,
  mrIndexMR37,
  mrIndexMR38,
  mrIndexMR39,
  mrIndexMR40,
  mrIndexMR41,
  mrIndexMR45,
  mrIndexMR46,
  mrIndexMR48,
  mrIndexMR50,
  mrIndexMR54,
  mrIndexMR55,
  mrIndexMR56,
  mrIndexMR57,
  mrIndexMR58,
  mrIndexMR59,
  mrIndexMR69,
  mrIndexMR75,
  mrIndexMR111,
  // Becase we only train DIMM DFE per rank, we don't need to store per-bit DFE values in the host struct, they are all the same
  // Use DQL bit [0] Tap1..4 values for all bits: MR129..132
  mrIndexMR129, // DQL[0] DFE Tap1
  mrIndexMR130, // DQL[0] DFE Tap2
  mrIndexMR131, // DQL[0] DFE Tap3
  mrIndexMR132, // DQL[0] DFE Tap4
  mrIndexMR193, // DQU[0] DFE Tap1
  mrIndexMR194, // DQU[0] DFE Tap2
  mrIndexMR195, // DQU[0] DFE Tap3
  mrIndexMR196, // DQU[0] DFE Tap4
  mrIndexMpcMr13,
  mrIndexMpcMr32a0,
  mrIndexMpcMr32a1,
  mrIndexMpcMr32b0,
  mrIndexMpcMr32b1,
  mrIndexMpcMr33a0,
  mrIndexMpcMr33,
  mrIndexMpcMr33b0,
  mrIndexMpcMr34,
  mrIndexMpcApplyVrefCa,
  mrIndexMpcDllReset,
  mrIndexMpcZqCal,
  mrIndexMpcZqLat, // Needed for LP5
  mrIndexMpcEnterCaTrainMode,
  mrIndexMpcSetCmdTiming,
  mrIndexMpcSelectAllPDA,
  mrIndexMax
} MrcModeRegisterIndex;

// MRC currently only reports out and can disable at the channel level.  In the case of DDR5 Desktop, both DIMM's must be set for Error/Disable reporting.
#define MRC_DIMM_STATUS_ERROR         (1)
#define MRC_DIMM_STATUS_NO_ERROR      (0)
#define MRC_DIMM_STATUS_DISABLE       (1)
#define MRC_DIMM_STATUS_ENABLE        (0)

#define READ_RESTORE_FROM_SCRATCHPAD  (FALSE)
#define WRITE_SAVE_TO_SCRATCHPAD      (TRUE)

#define MRC_DIMM_STATUS_MAX_DIMMS_CONTROLLER (8)
#define MRC_DIMM_STATUS_MAX_DIMMS_CHANNEL (2)

#define MRC_DIMM_STATUS_INDEX(Controller, Channel, Dimm) (MRC_DIMM_STATUS_MAX_DIMMS_CONTROLLER*Controller + MRC_DIMM_STATUS_MAX_DIMMS_CHANNEL*Channel + Dimm)

typedef union {
  struct {
    UINT16 Mc0Ch0Dimm0 : 1;  // Bit 0
    UINT16 Mc0Ch0Dimm1 : 1;  // Bit 1 - Only Applicable to DDR5 Desktop
    UINT16 Mc0Ch1Dimm0 : 1;  // Bit 2
    UINT16 Mc0Ch1Dimm1 : 1;  // Bit 3 - Only Applicable to DDR5 Desktop
    UINT16 Mc0Ch2Dimm0 : 1;  // Bit 4 - Only Applicable to LP5
    UINT16 RsvdBit5    : 1;  // Bit 5
    UINT16 Mc0Ch3Dimm0 : 1;  // Bit 6 - Only Applicable to LP5
    UINT16 RsvdBit7    : 1;  // Bit 7
    UINT16 Mc1Ch0Dimm0 : 1;  // Bit 8
    UINT16 Mc1Ch0Dimm1 : 1;  // Bit 9 - Only Applicable to DDR5 Desktop
    UINT16 Mc1Ch1Dimm0 : 1;  // Bit 10
    UINT16 Mc1Ch1Dimm1 : 1;  // Bit 11 - Only Applicable to DDR5 Desktop
    UINT16 Mc1Ch2Dimm0 : 1;  // Bit 12 - Only Applicable to LP5
    UINT16 RsvdBit13   : 1;  // Bit 13
    UINT16 Mc1Ch3Dimm0 : 1;  // Bit 14 - Only Applicable to LP5
    UINT16 RsvdBit15   : 1;  // Bit 15
  } BitMask;
  UINT16 Data16;
} MRC_DIMM_STATUS_STRUCT;

typedef MRC_DIMM_STATUS_STRUCT MrcDimmStatus;

typedef enum {
   MrcSaveEnum,
   MrcRestoreEnum
 } MrcSaveOrRestore;

#define MRC_SAGV_DISABLED (0)
#define MRC_SAGV_ENABLED  (1)
#define MRC_SAGV_FIXED_POINT_0 (0x1)
#define MRC_SAGV_FIXED_POINT_1 (0x2)
#define MRC_SAGV_FIXED_POINT_2 (0x4)
#define MRC_SAGV_FIXED_POINT_3 (0x8)
#define MRC_SAGV_MASK_0_1      (MRC_SAGV_FIXED_POINT_0 | MRC_SAGV_FIXED_POINT_1)
#define MRC_SAGV_MASK_0_1_2    (MRC_SAGV_MASK_0_1 | MRC_SAGV_FIXED_POINT_2)
#define MRC_SAGV_MASK_0_1_2_3  (MRC_SAGV_MASK_0_1_2 | MRC_SAGV_FIXED_POINT_3)

#define MAX_CPU_SOCKETS           (1)       ///< The maximum number of CPUs per system.
#define MAX_CHANNEL_SHARE_REGS    (MAX_CHANNEL / MAX_SUB_CHANNEL) ///< The maximum number of channels per memory controller that share registers
#define MAX_SYS_CHANNEL           (MAX_CONTROLLER * MAX_CHANNEL)  ///< The maximum number of channels per CPU socket.
#define MAX_PHY_CHANNEL           (MAX_SUB_CHANNEL * MAX_CHANNEL)  ///< The maximum number of channels treating each sub channels as an independent channel, per controller.
#define MAX_DDR5_CHANNEL          (2)       ///< The maximum number of x32 channels per memory controller for DDR5.
#define MAX_DDR5_SYS_CHANNEL      (MAX_CONTROLLER * MAX_DDR5_CHANNEL)  ///< The maximum number of channels for DDR5 per CPU socket.
#define MAX_DDR5_BANKS            (2)       ///< Max Number of Banks for DDR5 currently supported in CPGC
#define MAX_2D_EYE_TYPE           (2)       ///< Maximum number of supported Margin 2D Eye Types
#define MAX_2D_EYE_OFFSETS        (7)       ///< Number of 2D Eye Offsets

#define MAX_DIMMS_IN_SYSTEM       (4)       ///< Max Number of Dimms per system.
#define MAX_BANKS                 (32)      ///< Max Number of Banks supported in CPGC.  This is a combination of BankGroup and Bank.
#define MAX_CPGC_B2B_BANKS        (2)       ///< Max Number of Banks to support B2B CPGC tests
#define MAX_CPGC_RECEN_BG_BANKS   (4)       ///< Max Number of Banks to support LP5 BG Mode RecEn CPGC test
#define MAX_DDR5_8Gb_x16_BANKS    (8)       ///< Max Number of Banks in DDR5 8Gb x16.  This is a combination of BankGroup and Bank.
#define MAX_DDR5_8Gb_x8_BANKS     (16)      ///< Max Number of Banks in DDR5 8Gb x8.  This is a combination of BankGroup and Bank.
#define MAX_DDR5_16Gb_x8_BANKS    (32)      ///< Max Number of Banks in DDR5 16Gb x8.  This is a combination of BankGroup and Bank.
#define MAX_DDR5_16Gb_x16_BANKS   (16)      ///< Max Number of Banks in DDR5 16Gb x16.  This is a combination of BankGroup and Bank.
#define MAX_DDR5_BANK_GROUPS      (8)       ///< Max Number of Bank Groups for DDR5.
#define MAX_DDR5_BANK_GROUPS      (8)       ///< Max Number of Bank Groups for DDR5
#define MAX_DDR5_BANKS_PER_BG     (4)       ///< Max Number of Banks per-BG for DDR5.
#define MIN_DDR5_BANK_MODES       (2)       ///< Min Number of Banks for DDR5 Org.
#define MAX_SYS_SDRAM             (MAX_CONTROLLER * MAX_SDRAM_IN_DIMM) ///< The maximum number of SDRAMs per CPU socket when ECC is enabled.
#define MAX_BYTE_IN_LP_CHANNEL    (2)       ///< Max number of Bytes in a LPDDR Channel
#define MAX_LPDDR5_BANKS          (16)      ///< Max Number of Banks for LPDDR5 (not in BG mode)
#define MAX_LPDDR5_BANK_GROUPS    (4)       ///< Max Number of Bank Groups for LPDDR5
#define MAX_BANK_GROUP_CNT        (MAX(MAX_DDR5_BANK_GROUPS, MAX_LPDDR5_BANK_GROUPS))  ///< Max Number of Bank Groups across all technologies.
#define MAX_CCC_PER_CONTROLLER    (4)       ///< Max number of CCC blocks per Controller
#define MAX_BYTE_IN_BLOCK         (2)       ///< Number of Bytes in a DataShared block
#define MAX_MR_IN_DIMM            (mrIndexMax)  ///< Maximum number of mode registers in a DIMM.
#define MAX_BITS                  (8)       ///< BITS per byte.
#define MAX_BITS_WITH_DBI         (MAX_BITS + 1) ///< 8 DQ bits plus 1 DBI bit
#define DBI_BIT                   (8)       ///< DBI bit number
#define DQS_INDEX                 (9)       ///< DQS Index
#define MAX_BITS_MASK             ((1 << MAX_BITS) - 1)  ///< Mask for MAX_BITS
#define CHAR_BITS                 (8)       ///< Number of bits in a char.
#define MAX_DQ_IN_LP_CHANNEL      (MAX_BITS * MAX_BYTE_IN_LP_CHANNEL)  ///< Max number of DQ bits in LPDDR Channel
#define BCLK_DEFAULT              (100 * 1000 * 1000) ///< BCLK default value, in Hertz.
#define MAX_COMMAND_GROUPS        (2)       ///< 2 for DDR5. 1 for LPDDR per controller/channel
#define MAX_DCC_CLOCK_PI          (128)     ///< CLK PI Range (0 - 127) Only used for DCC Look Up Table
#define MAX_DCC_TX_DQS_DQ_PI      (128)     ///< Tx Dqs/Dq PI Range (0 - 127) Only used for DCC Look Up Table
#define MAX_DCC_LANE_STATUS       (10)      ///< # of lanes in DccLaneStatus
#define MAX_CCC_LANE              (10)      ///< # of CCCLanex registers
#define MAX_CCC_PER_BIT_UY        (11)      ///< # of PBD settings for CCC UlxUlt (10 for CCC and 1 for WCK)
#define MAX_CCC_PER_BIT           (15)      ///< # of PBD settings for CCC max(UlxUlt, DtHalo)
#define MAX_GEARS                 (2)       ///< Max numbers of Gears supported by MRC.
#define MAX_SPD_PAGE_COUNT        (4)       ///< Max (DDR5 page Count)
#define MAX_SPD_PAGE_SIZE         (256)
#define MAX_SPD_PAGE_SIZE_SPD5    (128)     ///< Page size for SPD5 in 1 byte addressing mode (MR11[3]=0)
#define MAX_SPD_SIZE              (MAX_SPD_PAGE_SIZE * MAX_SPD_PAGE_COUNT)
#define NUM_ECC_BITS              (8)       ///< Number of error correcting code (ECC) bits.
#define NUM_CPU_MODELS            (2)       ///< Number of skus supported (Ulx/Ult, DtHalo)
#define MAX_DFE_TAP_DDR5          (4)       ///< MAX number of DFE taps in DDR5
#define MAX_DFE_TAP_LPDDR5        (1)       ///< MAX number of DFE taps in LPDDR5
#define DFE_TAP_NUM               (4)       ///< Number of DFE taps
#define MAX_CKD_PIN               (4)
#define CKD_QCK_SHARE             (2)
#define MAX_WRITE_CKD             (9)       /// Number of Write Control Word
#define MAX_CKD_REG               (0x60)    /// Total of CKD Registers
#define CKD_NOT_PRESENT           (0)
#define MAX_DDR5_1SPC_ODT         (2)       /// Max sets of ODT table for 1SPC platform (1R, 2R)
#define MAX_DDR5_2SPC_ODT         (6)       /// Max sets of ODT table for 2SPC platform (0R/1R, 0R/2R, 1R/1R, 1R/2R, 2R/1R, 2R/2R)
#define MAX_DDR5_ODT_STRAP        (2)       /// ODT Group A/Group B

/// Edge Definitions
#define MAX_EDGES                 (2)       ///< Maximum number of edges.
// Signals Rise and Fall Edges
#define MRC_EDGE_RISE             (0)
#define MRC_EDGE_FALL             (1)
// Left and Right Margin Eye Edges
#define MRC_EDGE_LEFT             (0)
#define MRC_EDGE_RIGHT            (1)

/// Error Counter for Failure in BER
#define MRC_BER_ERROR_COUNTER_FOR_FAILURE  (1)  ///< Number of errors before considered failure

/// CCC PerBit defines
#define MAX_DDR5_CMD_PINS          (13)      ///< CA0-CA12
#define MAX_LPDDR5_CMD_PINS        (7)       ///< CA0-CA6

#define DDR5_ALL_CMD_PINS_MASK     ((1 << MAX_DDR5_CMD_PINS) - 1)

#define MAX_PDA_MR_IN_CHANNEL      (MAX_RANK_IN_CHANNEL * MAX_BYTE_IN_DDR5_CHANNEL) ///< Maximum Devices per MR per Channel
#define MAX_ROW_FAILURES_PER_BANK  (4)
#define MAX_CTL_PINS_UY            (2)       ///< CS0/CS1 or CKE0/CKE1 or ODT0/ODT1
#define MAX_CTL_PINS_H             (4)       ///< CS0/CS1/CS2/CS3 or CKE0/CKE1/CKE2/CKE3 or ODT0/ODT1/ODT2/ODT3
#define MAX_CLK_PINS               (1)       ///< Shared pin for CLKP/CLKN
#define MAX_MISC_PINS              (2)       ///< BA0/BA1 or BG0/BG1 or ACTn/PAR
#define MAX_LPDDR5_ASCEND_DESCEND  (2)       ///< Ascending/Descending

#define MRC_POST_CODE_LOW_BYTE_ADDR   (0x48) ///< Post code to check for stop break - Low byte
#define MRC_POST_CODE_HIGH_BYTE_ADDR  (0x49) ///< Post code to check for stop break - High byte
#define MRC_CHANNEL_DISABLE_CMOS_ADDR (0x4A) ///< Memory channel disable, 0 = all enabled, 0xFF = all disabled.
///<  [Bit7:0] Mc1Ch3, Mc1Ch2, Mc1Ch1, Mc1Ch0, Mc0Ch3, Mc0Ch2, Mc0Ch1, Mc0Ch0
#define MRC_DDR_FREQ_CMOS_ADDR        (0x4B) ///< Limit DDR speed using CMOS override
#define MRC_DEBUG_LEVEL_CMOS_ADDR     (0x4C) ///< Debug level override (32-bit value)
#define MRC_RESET_COUNT_CMOS_ADDR     (0x70) ///< 1-byte counter for reset cycling test. 0 = reset cycling disabled.
// Map the below 8-bit cmos voltage values to actual mV value with the following equation: (CmosVoltageValue * 26) / 10, resulting in a range of 0 - 663 mV, with a stepping of 2.6 mV
#define MRC_VCCCLK_CMOS_ADDR          (0xD0) ///< VccCLK override value
#define MRC_VCCIOG_CMOS_ADDR          (0xD1) ///< VccIOG override value
#define MRC_VCCDDQ_CMOS_ADDR          (0xD2) ///< VccDDQ override value
#define MRC_DEBUG_CONFIG4_CMOS_ADDR   (0xD3) ///< CMOS CONFIG4 RFU
#define MRC_DEBUG_SIGN0_CMOS_ADDR     (0xD4) ///< Signature0: Enable MRC CMOS DEBUG FLOW with the special signature (sign = addr)
#define MRC_DEBUG_SIGN1_CMOS_ADDR     (0xD5) ///< Signature1: Enable MRC CMOS DEBUG FLOW with the special signature (sign = addr)
#define MRC_DEBUG_SIGN2_CMOS_ADDR     (0xD6) ///< Signature2: Enable MRC CMOS DEBUG FLOW with the special signature (sign = addr)
#define MRC_DEBUG_SIGN3_CMOS_ADDR     (0xD7) ///< Signature3: Enable MRC CMOS DEBUG FLOW with the special signature (sign = addr)
#define MRC_DEBUG_CONFIG3_CMOS_ADDR   (0xD8) ///< CMOS CONFIG3 hooks miscellaneous bits as follows,
///< [Bit0]    = SAGV: <b>0=Disabled</b>, 1=Enabled
///< [Bit1]    = 0: SAGV override using bit [0]; 1: SAGV is Auto (use existing MRC config, do not override)
///< [Bit2]    = Reserved
///< [Bit3]    = Reserved
///< [Bit7:4]  = Four bit group to select the usage for MRC_DEBUG_CONFIG2_CMOS_ADDR value
/*
    Bit7 | Bit6 | Bit5 | Bit4
      0      0      0      0  ->  No usage selected for MRC_DEBUG_CONFIG2_CMOS_ADDR value
      0      0      0      1  ->  RcvEn Offset applied to register value
      0      0      1      0  ->  RxDqs Offset applied to register value
      0      0      1      1  ->  TxDqs Offset applied to register value
      0      1      0      0  ->  TxDq  Offset applied to register value
      0      1      0      1  ->  WrVref Offset applied to register value
      0      1      1      0  ->  RxVerf Offset applied to register value
      0      1      1      1  ->  CmdVref Offset applied to register value
      1      0      0      0  ->  CtlVref Offset applied to register value
      1      0      0      1  ->  CmdPi Offset applied to register value
      1      0      1      0  ->  CtlPi Offset applied to register value
      1      0      1      1  ->  ClkPi Offset applied to register value
      1      1      0      0  ->  WckPi Offset applied to register value
      1      1      0      1  ->  Reserved for future use
      1      1      1      0  ->  Reserved for future use
      1      1      1      1  ->  Reserved for future use
**/
#define MRC_FREQ_POINT3_CMOS_ADDR     (0xD9) ///< SAGV Point 3 Frequency Value (33.33 Based) 0=Auto 32=1067 40=1333 .. 64=2133 .. 112=3733 .. 128=4267
#define MRC_FREQ_POINT2_CMOS_ADDR     (0xDA) ///< SAGV Point 2 Frequency Value (33.33 Based) 0=Auto 32=1067 40=1333 .. 64=2133 .. 112=3733 .. 128=4267
#define MRC_FREQ_POINT1_CMOS_ADDR     (0xDB) ///< SAGV Point 1 Frequency Value (33.33 Based) 0=Auto 32=1067 40=1333 .. 64=2133 .. 112=3733 .. 128=4267
#define MRC_FREQ_POINT0_CMOS_ADDR     (0xDC) ///< SAGV Point 0 Frequency Value (33.33 Based) 0=Auto 32=1067 40=1333 .. 64=2133 .. 112=3733 .. 128=4267
#define MRC_DEBUG_CONFIG0_CMOS_ADDR   (0xDD) ///< CMOS CONFIG0 hooks miscellaneous bits as follows,
///< [Bit0] = RmtPerTask: Rank Margin Tool Per Task. <b>0 = Disabled</b>, 1 = Enabled
///< [Bit1] = for RComp Cycle Prints if DCC enabled and Gear2
///< [Bit2] = for enabling MRC Time Measurement
///< [Bit3] = additional MSB for reaching higher Ratio when using CMOS 0x4B (MRC_DDR_FREQ_CMOS_ADDR) address
///< [Bit4] = additional MSB for reaching higher SAGV Point 0 Frequency Value
///< [Bit5] = additional MSB for reaching higher SAGV Point 1 Frequency Value
///< [Bit6] = additional MSB for reaching higher SAGV Point 2 Frequency Value
///< [Bit7] = additional MSB for reaching higher SAGV Point 3 Frequency Value
#define MRC_DEBUG_CONFIG1_CMOS_ADDR   (0xDE) ///< Gear mode values for WPx as follows,
///< [Bit1:0] for Gear mode WP0: 00=Auto, 01=Gear2, 10=Gear4, 11=Auto
///< [Bit3:2] for Gear mode WP1: 00=Auto, 01=Gear2, 10=Gear4, 11=Auto
///< [Bit5:4] for Gear mode WP2: 00=Auto, 01=Gear2, 10=Gear4, 11=Auto
///< [Bit7:6] for Gear mode WP3: 00=Auto, 01=Gear2, 10=Gear4, 11=Auto
#define MRC_DEBUG_CONFIG2_CMOS_ADDR   (0xDF) ///< CMOS values applied based upon MRC_DEBUG_CONFIG3_CMOS_ADDR[Bit7:4] selection table
#define MRC_DEBUG_OFFSET_PARAM_CMOS_ADDR     (0xE1)
#define MRC_DEBUG_MC_CNL_MASK_CMOS_ADDR      (0xE2)
#define MRC_DEBUG_PARAM_OFFSET_0_CMOS_ADDR   (0xE3)
#define MRC_DEBUG_PARAM_OFFSET_1_CMOS_ADDR   (0xE4)
#define MRC_DEBUG_PARAM_OFFSET_2_CMOS_ADDR   (0xE5)
#define MRC_FORCECKDBYPASS_CMOS_ADDR         (0xE6) ///< CMOS CONFIG0 hooks miscellaneous bits as follows:
///< [Bit0] = ForceCkdBypass: Force CKD in bypass mode <b>0 = Disabled</b>, 1 = Enabled
///< [Bit7:1] Reserved for future use
#define MAX_OFFSETS_TO_CHANGE_VIA_CMOS (3)
#define MAX_PARAM_TO_CHANGE_VIA_CMOS   (7)
//In case increasing number of possible offset to control with single burn:
//Adding  MRC_DEBUG_OFFSET_3_CMOS_ADDR  -> MRC_DEBUG_OFFSET_7_CMOS_ADDR
//1. Addresses should be continuous to MRC_DEBUG_PARAM_OFFSET_0_CMOS_ADDR
//2. MAX_OFFSETS_TO_CHANGE_VIA_CMOS should be adjusted accordingly

#define SPD5_MANUF_START       512         ///< The starting point for the SPD DRAM module manufacturing data.
#define SPD5_MANUF_END         520         ///< The ending point for the SPD DRAM module manufacturing data.
#define SPD5_DRAM_MFG_START    552         ///< The starting point for the SPD DRAM manufacturing data.
#define SPD5_DRAM_MFG_END      554         ///< The ending point for the SPD DRAM manufacturing data.
#define SPDLP_MANUF_START      320         ///< The starting point for the SPD manufacturing data.
#define SPDLP_MANUF_END        328         ///< The ending point for the SPD manufacturing data.
#define SPDLP_JEDEC_SPEC_MANUF_START   512 ///< The starting point for the SPD manufacturing data.
#define SPDLP_JEDEC_SPEC_MANUF_END     520 ///< The ending point for the SPD manufacturing data.

#define MRC_DDR5_ECC_CPGC_COUNTER_INDEX 8
#define MRC_DDR5_ECC_BYTE 4       /// Indicates the Byte index of the DDR5 ECC Byte
#define MRC_DDR5_ECC_MAX_BITS 4
#define MRC_DDR5_ECC_MAX_BYTES 5  /// Max amount of bytes when ECC is enabled

#define MEM_4GB   (0x1000)  ///< Define the 4GB size in 1MB units (1000MB = 1GB).
#define MEM_128MB 128       ///< Define the 128MB size in 1MB units

#define MEMORY_MAP_MAX_HOLE  5

// PPR Defines
#define MAX_FAIL_RANGE                          (16)
#define DQ_MASK_INDEX_MAX                       (2)
#define MAX_PPR_ADDR_ENTRIES_DDR                (20)
#define MAX_PPR_ADDR_ENTRIES_DDR_SPPR           (MAX_PPR_ADDR_ENTRIES_DDR + 20)
#define MAX_MEMTEST_BANK_INTERLEAVE_NUMBER      (32)
#define MAX_BAD_ROW_LIMIT_PER_BANK              (8)
#define PPR_REPAIR_STATUS_FAIL                  (0)
#define PPR_REPAIR_STATUS_UNCORRECTABLE         (1)
#define PPR_REPAIR_STATUS_SUCCESS               (2)

#define NUM_LCPLL 2

#pragma pack (push, 1)

//
// ---------------------------- Test Menu Macro -----------------------------
//
#define FORCE_ENABLE  1
#define FORCE_DISABLE 2
#define PLATFORM_POR  0
#define SHARED_MAILBOX_SIZE 8  ///< Define the 8MB size in 1MB units, 8MB is the SharedMailbox size needed for SV and XmlCli module


///
//////////////////////////////////////////////////////////////////////////////////////
///                           OEM platform  routines and types                      //
//////////////////////////////////////////////////////////////////////////////////////
///
/// define the oem check points the OEM can define more point and locate them in the code.
///
typedef enum {
  OemFastBootPermitted,     ///<  before fast boot.
  OemRestoreNonTraining,
  OemPrintInputParameters,  ///<  before printing input parameters.
  OemSpdProcessingRun,      ///<  before SPD processing code.
  OemSetOverridePreSpd,     ///<  before set overrides pre SPD.
  OemSetOverride,           ///<  before set overrides.
  OemMcCapability,          ///<  before MC capability.
  OemMcInitRun,             ///<  before mc init code.
  OemMcMemoryMap,           ///<  before memory map.
  OemMcResetRun,            ///<  before jedec reset.
  OemPreTraining,           ///<  before the training.
  OemMcTrainingRun,         ///<  before training code
  OemEarlyCommandTraining,  ///<  before Early Command training
  OemMapDqDqsSwizzle,       ///<  before Map Dq Dqs Swizzle training
  OemSenseAmpTraining,      ///<  before Sense Amp Training
  OemWckClkPreDriverDcc,    ///<  before Duty Cycle Correction for Wck/Clk Pre-Driver
  OemWckPadDccCal,          ///<  before Wck Pad Dcc Calibration
  OemReadMprTraining,       ///<  before Read MPR Training
  OemReceiveEnable,         ///<  before Read Leveling
  OemRoundtripMatch,        ///<  before MrcRoundtripMatch
  OemJedecWriteLeveling,    ///<  before Jedec Write Leveling
  OemWriteDqDqs,            ///<  before Write Timing Centering
  OemWriteVoltage,          ///<  before Write Voltage Centering
  OemReadVoltage,           ///<  before Read Voltage Centering
  OemUnMatchedWriteDqDqs,   ///<  before Unmatched Write Time Centering 1D
  OemEarlyWriteDqDqs2D,     ///<  before Early Write Timing Centering 2D
  OemEarlyReadDqDqs2D,      ///<  before Early Read Timing Centering 2D
  OemEarlyReadMprDqDqs2D,   ///<  before Early MPR Read Timing Centering 2D
  OemReadDqDqs,             ///<  before Read Timing Centering
  OemDimmRonTraining,       ///<  before DIMM Ron Training
  OemTxDqsDccTraining,      ///<  before TxDqs DCC Training
  OemDimmODTTraining,       ///<  before DIMM ODT Training
  OemDimmNTODTTraining,     ///<  before DIMM NT ODT Training
  OemDimmOdtCaTraining,     ///<  before DIMM ODT CA Training
  OemDimmDfeTraining,       ///<  before DIMM DFE Training
  OemWriteDriveStrengthEq,  ///<  before Write Drive Strength/Equalization 2D Training
  OemReadDqsODTTraining,    ///<  before Read Dqs ODT algorithm.
  OemReadDqODTTraining,     ///<  before Read Dq ODT algorithm.
  OemReadEQTraining,        ///<  before Read Equalization Training.
  OemReadCTLETraining,      ///<  before Read CTLE Training.
  OemOptimizeComp,          ///<  before Comp Optimization Training.
  OemWriteDqDqs2D,          ///<  before Write Timing Centering 2D.
  OemReadDqDqs2D,           ///<  before Read Timing Centering 2D.
  OemCmdVoltCentering,      ///<  before Command Voltage Centering.
  OemWriteVoltCentering2D,  ///<  before Write Voltage Centering 2D.
  OemReadVoltCentering2D,   ///<  before Read Voltage Centering 2D.
  OemLateCommandTraining,   ///<  before Late Command training.
  OemTurnAroundTimes,       ///<  before Turn Around Times.
  OemSaveMCValues,          ///<  before saving memory controller values.
  OemRmt,                   ///<  before RMT crosser tool.
  OemRmtEvenOdd,            ///<  before RMT even odd tool.
  OemRestoreTraining,       ///<  before Restoring Training Values.
  OemEarlyTxDqTcoComp,      ///<  before Early TxDqTCO Comp training.
  OemTxDqTcoComp,           ///<  before TxDqTCO Comp training.
  OemClkTcoCompTraining,    ///<  before ClkTCO Comp training.
  OemWckTcoCompTraining,    ///<  before WckTCO Comp training.
  OemCmdDriveStrengthEq,    ///<  before CMD/CTL TxEq/Drive Strength training
  OemCmdSlewRate,           ///<  before CMD/CTL/CLK Slew Rate training
  OemReadVrefDecapTraining, ///<  before RxVref Decap training
  OemApplyMcRegOffsets,     ///<  before applying MC Register Offsets.
  OemSelfRefreshExit,       ///<  before Self Refresh Exit.
  OemNormalMode,            ///<  before Normal Mode on non-cold boots.
  OemTxtAliasCheck,         ///<  before TxT Alias Check Call.
  OemAliasCheck,            ///<  before alias checking on cold boots.
  OemIbecc,                 ///<  ibecc enabling.
  OemHwMemInit,
  OemMrcDataSummary,        ///<  before MrcDataSummary call.
  OemMrcMcFinalize,         ///<  before MrcMcFinalize
  OemMrcRhPrevention,       ///<  before MrcRhPrevention
  OemPprEnable,             ///<  before Post Package Repair
  OemDdr5CkdConfig,         ///<  before DDR5 CKD Config
  OemSaGvSwitch,            ///<  before SA GV switch
  OemFrequencySet,          ///<  do operation before frequency set.
  OemFrequencySetDone,      ///<  do operation after frequency set.
  OemStartMemoryConfiguration,
  OemBeforeNormalMode,      ///<  call to MrcOemCheckPoint before normal mode is enabled.
  OemAfterNormalMode,       ///<  call to MrcOemCheckPoint after normal mode is enabled.
  OemMrcFillBdat,
  OemRetrainMarginCheck,
  OemSsaStopPoint,          ///< Call to SSA stop point.
  OemDunitConfig,           ///< before DUnit Init Sequence.
  OemDdrPhyInit,            ///< before Phy Init Sequence.
  OemDdrPhyInitFast,        ///< before Phy Init Sequence for Green MRC Fast Boot.
  OemBwSelCal,              ///< before BwSel Calibration.
  OemViewPinCal,            ///< before View Pin Calibration
  OemWriteDqDqsReTraining,
  OemRmtPerBit,             ///< before Rank Margin Tool Per Bit training
  OemRmtLvr,                ///< before Lvr Rmt
  OemDllDccCal,             ///< before Initializing DLL Duty Cycle Correction
  OemDccWckClkRiseFall,     ///< before Duty Cycle Correction for WCK/CLK Rise/Fall Alignment
  OemDccRiseFall,           ///< before Duty Cycle Correction for DQS Rise/Fall Alignment
  OemVccClkRxFfCal,         ///< before VccClkRx FeedForward Leg Calibration
  OemRxDqsDcc,              ///< before Duty Cycle Correction for RxDqs
  OemPrintDdrMrs,           ///< before the printing non-LPDDR$ MRs
  OemWckLeveling,           ///< before Write Clock Leveling for LPDDR5.
  OemMbist,                 ///< before Memory Subsystem MBIST
  OemUpdateSaveMCValues,    ///< before Updating memory controller values.
  OemPmaConfig,             ///< before PMA Config
  OemSagvFinalize,          ///< before SAGV Finalization
  OemMrcDdrIoFinalize,      ///< before DdrIo Finalization
  OemMrcCurrentSensorRMT,   ///< before Current Sensor RMT
  OemSwitchDfiControl,      ///< before MrcSwitchDfiControl
  OemMrcUnmatchedRxCalPre,  ///< before Unmatched Receiver Calibration Pre
  OemDccLpddr5Wck,          ///< before Functional DCC - LPDDR5 WCK
  OemDccDdr5Clk,            ///< before Functional DCC - DDR5 CLK
  OemDccDdr5Dqs,            ///< before Functional DCC - DDR5 DQS
  OemDccDataDq,             ///< before Functional DCC - DATA DQ
  OemDccLpddr5WckDca,       ///< before DRAM DCC - LPDDR5 WCK DCA
  OemDccLpddr5ReadDca,      ///< before DRAM DCC - LPDDR5 Read DCA
  OemRefPiCal,              ///< before Ref PI Calibration
  OemDccPhaseClkCal,        ///< before DCC Phase CLK calibration
  OemDccPiCodeLut,          ///< before DCC PI Code LUT
  OemDataPiLinearity,       ///< before Data PI Linearity Calibration
  OemQClkPhAlign,           ///< before Align MC QClk to Ph QClk calibration
  OemDccPiSerializerCal,    ///< before DCC PI Seriallizer Calibration
  OemVccLvrAutoTrim,        ///< before Vcc LVR Auto Trim
  OemDccTlineClkCal,        ///< before Tline CLK calibration
  OemQClkDcc,             ///< before QCLK DCC calibration
  OemMrcDdr5RxXTalk,        ///< before Ddr5 Rx Cross-Talk Cancellation
  OemMarginLimitCheck,      ///< before Margin Limit Check
  OemTestMtgRegAccess,      ///< before MrcTestMtgRegAccess
  OemShortMtgTest,          ///< before MrcShortMtgTest
  OemDdr5OdtTimingConfig,   ///< before Ddr5 ODT Timing Config
  OemUcReset,               ///< before UC undergoes soft reset
  OemUcLoadGreenCode,       ///< before Green code is loaded to UC
  OemUcLoadGreenData,       ///< before Green data is loaded to UC
  OemUcExecuteGreen,        ///< before Green MRC begins executing
  OemDbi,                   ///< before DBI training
  OemDvfsqVoltageRamp,      ///< before DVFSQ asks Blue to ramp VDDQ voltage down to 0.3V
  OemDimmRxOffsetCal,       ///< before checking Dimm Rx Offset Calibration
  OemMcDeswizzleRegisters,  ///< before MrcMcProgramDeswizzleRegisters
  OemIsDramSupportsDvfsc,   ///< Check if DRAM supports E-DVFSC before finishing SAGV0 training flow
  OemEmphasisTraining,      ///< before Pre-Emphasis LP5 Training
  OemMrcHVMFinalize,        ///< before HVM Finalization
  OemDqLoopbackTest,        ///< before Dq Loopback test
  OemDqsPadDcc,             ///< before DQS PAD DCC Optimization
  OemRxDqsVoc,              ///< before RxDqs VOC Centering
  OemDunitTatOptimization,  ///< before Dunit TAT optimization
  OemVccLvrInit,            ///< before Vcc Lvr init configuration
  ///
  ///*********************************************************************************
  ///
  OemNumOfCommands          ///<  Should always be last in the list!
} MrcOemStatusCommand;

typedef UINT8 MrcIteration; ///< MRC invocation sequence number, start with 0 and increment by one each time MRC library is called.
#define MRC_ITERATION_MAX ((1 << ((sizeof (MrcIteration) * 8) - 1)) + ((1 << ((sizeof (MrcIteration) * 8) - 1)) - 1))

#define MAX_DIMMODT_VALUE       (480)

//Sequence need match with members of TOdtValueDqDdr5, and TOdtValueCccDdr5
typedef enum {
  OFFSET_RTTWR,
  OFFSET_RTTNOMRD,
  OFFSET_RTTNOMWR,
  OFFSET_RTTPARK,
  OFFSET_RTTPARKDQS,
  OFFSET_RTTCA_GROUPA,
  OFFSET_RTTCS_GROUPA,
  OFFSET_RTTCK_GROUPA,
  OFFSET_RTTCA_GROUPB,
  OFFSET_RTTCS_GROUPB,
  OFFSET_RTTCK_GROUPB,
  MAX_DIMMODT_ENTRIES
} DIMMODT_INDEX;

typedef UINT8        (EFIAPI *MRC_IO_READ_8)               (UINT32 IoAddress);
typedef UINT16       (EFIAPI *MRC_IO_READ_16)              (UINT32 IoAddress);
typedef UINT32       (EFIAPI *MRC_IO_READ_32)              (UINT32 IoAddress);
typedef void         (EFIAPI *MRC_IO_WRITE_8)              (UINT32 IoAddress, UINT8 Value);
typedef void         (EFIAPI *MRC_IO_WRITE_16)             (UINT32 IoAddress, UINT16 Value);
typedef void         (EFIAPI *MRC_IO_WRITE_32)             (UINT32 IoAddress, UINT32 Value);
typedef UINT8        (EFIAPI *MRC_MMIO_READ_8)             (UINT64 Address);
typedef UINT16       (EFIAPI *MRC_MMIO_READ_16)            (UINT64 Address);
typedef UINT32       (EFIAPI *MRC_MMIO_READ_32)            (UINT64 Address);
typedef UINT64       (EFIAPI *MRC_MMIO_READ_64)            (UINT64 Address);
typedef UINT8        (EFIAPI *MRC_MMIO_WRITE_8)            (UINT64 Address, UINT8 Value);
typedef UINT16       (EFIAPI *MRC_MMIO_WRITE_16)           (UINT64 Address, UINT16 Value);
typedef UINT32       (EFIAPI *MRC_MMIO_WRITE_32)           (UINT64 Address, UINT32 Value);
typedef UINT64       (EFIAPI *MRC_MMIO_WRITE_64)           (UINT64 Address, UINT64 Value);
typedef UINT8        (EFIAPI *MRC_SMBUS_READ_8)            (UINT32 Address, RETURN_STATUS *Status);
typedef UINT16       (EFIAPI *MRC_SMBUS_READ_16)           (UINT32 Address, RETURN_STATUS *Status);
typedef UINT8        (EFIAPI *MRC_SMBUS_WRITE_8)           (UINT32 Address, UINT8 Value, RETURN_STATUS *Status);
typedef UINT16       (EFIAPI *MRC_SMBUS_WRITE_16)          (UINT32 Address, UINT16 Value, RETURN_STATUS *Status);
typedef UINT32       (EFIAPI *MRC_GET_PCI_DEVICE_ADDRESS)  (UINT8 Bus, UINT8 Device, UINT8 Function, UINT8 Offset);
typedef UINT32       (EFIAPI *MRC_GET_PCIE_DEVICE_ADDRESS) (UINT8 Bus, UINT8 Device, UINT8 Function, UINT8 Offset);
typedef void         (EFIAPI *MRC_GET_RTC_TIME)            (UINT8 *Second, UINT8 *Minute, UINT8 *Hour, UINT8 *Day, UINT8 *Month, UINT16 *Year);
typedef UINT64       (EFIAPI *MRC_GET_CPU_TIME)            (void);
typedef UINT32       (EFIAPI *MRC_GET_CPU_TIME_US)         (void *MrcData);
typedef void *       (EFIAPI *MRC_MEMORY_COPY)             (UINT8 *Destination, UINT8 *Source, UINT32 NumBytes);
typedef void *       (EFIAPI *MRC_MEMORY_SET_BYTE)         (UINT8 *Destination, UINT32 NumBytes, UINT8 Value);
typedef void *       (EFIAPI *MRC_MEMORY_SET_WORD)         (UINT16 *Destination, UINT32 NumWords, UINT16 Value);
typedef void *       (EFIAPI *MRC_MEMORY_SET_DWORD)        (UINT32 *Destination, UINT32 NumDwords, UINT32 Value);
typedef UINT64       (EFIAPI *MRC_LEFT_SHIFT_64)           (UINT64 Data, UINT32 NumBits);
typedef UINT64       (EFIAPI *MRC_RIGHT_SHIFT_64)          (UINT64 Data, UINT32 NumBits);
typedef UINT64       (EFIAPI *MRC_MULT_U64_U32)            (UINT64 Multiplicand, UINT32 Multiplier);
typedef UINT64       (EFIAPI *MRC_DIV_U64_U64)             (UINT64 Dividend, UINT64 Divisor, UINT64 *Remainder);
typedef BOOLEAN      (EFIAPI *MRC_GET_SPD_DATA)            (UINT8 BootMode, UINT8 SpdAddress, UINT8 *SpdData, UINT8 *Ddr5Table, UINT32 Ddr5TableSize, UINT8 *LpddrTable, UINT32 LpddrTableSize);
typedef BOOLEAN      (EFIAPI *MRC_GET_RANDOM_NUMBER)       (UINT32 *Rand);
typedef UINT32       (EFIAPI *MRC_CPU_MAILBOX_READ)        (UINT32 Command, UINT32 *Value, UINT32 *Status);
typedef UINT32       (EFIAPI *MRC_CPU_MAILBOX_WRITE)       (UINT32 Command, UINT32 Value, UINT32 *Status);
typedef UINT32       (EFIAPI *MRC_CHECKPOINT)              (void *MrcData, UINT32 CheckPoint, void *Status);
typedef void         (EFIAPI *MRC_DEBUG_HOOK)              (void *GlobalData, UINT16 DisplayDebugNumber);
#ifdef MRC_MINIBIOS_BUILD
typedef void         (*MRC_PRINT_STRING)            (void *MrcData, void *String);
#else
typedef void         (EFIAPI *MRC_PRINT_STRING)            (void *String);
#endif //MRC_MINIBIOS_BUILD
typedef UINT8        (EFIAPI *MRC_GET_RTC_CMOS)            (UINT8 Location);
typedef UINT64       (EFIAPI *MRC_MSR_READ_64)             (UINT32 Location);
typedef UINT64       (EFIAPI *MRC_MSR_WRITE_64)            (UINT32 Location, UINT64 Data);
typedef void         (EFIAPI *MRC_RETURN_FROM_SMC)         (void *GlobalData, UINT32 MrcStatus);
typedef void         (EFIAPI *MRC_DRAM_RESET)              (UINT64 PciEBaseAddress, UINT32 ResetValue);
typedef void         (EFIAPI *MRC_DELAY_NS)                (void *MrcData, UINT32 DelayNs);
typedef RETURN_STATUS(EFIAPI *MRC_SET_LOCK_PRMRR)          (UINT64 PrmrrBase, UINT32 PrmrrSize);

/// These functions are implemented outside of the MRC Core.
/// They can be modified by OEM as needed.
typedef struct {
  MRC_IO_READ_8               MrcIoRead8;
  MRC_IO_READ_16              MrcIoRead16;
  MRC_IO_READ_32              MrcIoRead32;
  MRC_IO_WRITE_8              MrcIoWrite8;
  MRC_IO_WRITE_16             MrcIoWrite16;
  MRC_IO_WRITE_32             MrcIoWrite32;
  MRC_MMIO_READ_8             MrcMmioRead8;
  MRC_MMIO_READ_16            MrcMmioRead16;
  MRC_MMIO_READ_32            MrcMmioRead32;
  MRC_MMIO_READ_64            MrcMmioRead64;
  MRC_MMIO_WRITE_8            MrcMmioWrite8;
  MRC_MMIO_WRITE_16           MrcMmioWrite16;
  MRC_MMIO_WRITE_32           MrcMmioWrite32;
  MRC_MMIO_WRITE_64           MrcMmioWrite64;
  MRC_SMBUS_READ_8            MrcSmbusRead8;
  MRC_SMBUS_READ_16           MrcSmbusRead16;
  MRC_SMBUS_WRITE_8           MrcSmbusWrite8;
  MRC_SMBUS_WRITE_16          MrcSmbusWrite16;
  MRC_GET_PCI_DEVICE_ADDRESS  MrcGetPciDeviceAddress;
  MRC_GET_PCIE_DEVICE_ADDRESS MrcGetPcieDeviceAddress;
  MRC_GET_RTC_TIME            MrcGetRtcTime;
  MRC_GET_CPU_TIME            MrcGetCpuTime;
  MRC_GET_CPU_TIME_US         MrcGetCpuTimeUs;
  MRC_MEMORY_COPY             MrcCopyMem;
  MRC_MEMORY_SET_BYTE         MrcSetMem;
  MRC_MEMORY_SET_WORD         MrcSetMemWord;
  MRC_MEMORY_SET_DWORD        MrcSetMemDword;
  MRC_LEFT_SHIFT_64           MrcLeftShift64;
  MRC_RIGHT_SHIFT_64          MrcRightShift64;
  MRC_MULT_U64_U32            MrcMultU64x32;
  MRC_DIV_U64_U64             MrcDivU64x64;
  MRC_GET_SPD_DATA            MrcGetSpdData;
  MRC_GET_RANDOM_NUMBER       MrcGetRandomNumber;
  MRC_CPU_MAILBOX_READ        MrcCpuMailboxRead;
  MRC_CPU_MAILBOX_WRITE       MrcCpuMailboxWrite;
  MRC_CHECKPOINT              MrcCheckpoint;
  MRC_DEBUG_HOOK              MrcDebugHook;
  MRC_PRINT_STRING            MrcPrintString;
  MRC_GET_RTC_CMOS            MrcRtcCmos;
  MRC_MSR_READ_64             MrcReadMsr64;
  MRC_MSR_WRITE_64            MrcWriteMsr64;
  MRC_RETURN_FROM_SMC         MrcReturnFromSmc;
  MRC_DELAY_NS                MrcDelayNs;
  MRC_SET_LOCK_PRMRR          MrcSetLockPrmrr;
} MRC_FUNCTION;

typedef union {
  struct {
    UINT32 Row          : 18; // [17:0] This field is used for < > comparisons; more significant bits are used for match comparison only
    UINT32 BankPair     : 4;  // [21:18] This field covers worst case of DDR5 BG[1:0],BA[1:0], with BG[2] being the upper most BG bit for interleave
    UINT32 Rank         : 3;  // [24:22]
    UINT32 UpperBgMask  : 2;  // [26:25] Not used for DDR5
    UINT32 Valid        : 1;  // [27]
  } Bits;
  UINT32 Data;
} ROW_ADDR;

// PPR row fail entry
typedef struct {
  ROW_ADDR  Addr;
  UINT32    BankGroupMask;
  UINT32    BankGroupRepairedMask;
  UINT8     BankGroupAttempts[MAX_BANK_GROUP_CNT];
  BOOLEAN   Overflow;
  UINT8     DeviceTemp; // Device temperature after fail detected
  UINT8     Device;
} ROW_FAIL_RANGE;

// PPR test Type bits. Keep synced with defines in LunarLakePlatSamplePkg\Library\PeiPolicyUpdateLib\PeiSaPolicyUpdatePreMem.c
#define PPR_MEMTEST_WCMATS8_BIT               (0)
#define PPR_MEMTEST_DATA_RETENTION_BIT        (1)
#define PPR_MEMTEST_XMARCH_BIT                (2)
#define PPR_MEMTEST_XMARCHG_BIT               (3)
#define PPR_MEMTEST_YMARCHSHORT_BIT           (4)
#define PPR_MEMTEST_YMARCHLONG_BIT            (5)
#define PPR_MEMTEST_MMRW_BIT                  (6)

// PPR Running State
typedef enum {
  PPR_IS_BEFORE_RUNNING,
  PPR_IS_RUNNING,
  PPR_IS_DONE
} PPR_RUNNING_STATE;

typedef enum {
  LastRxV,
  LastRxT,
  LastTxV,
  LastTxT,
  LastRcvEna,
  LastWrLevel,
  LastCmdT,
  LastCmdV,
  MAX_RESULT_TYPE
} MrcMarginResult;

typedef enum {
  CccChCsPiGroup0,
  CccChCsPiGroup1,
  CccChCsPiGroupMax
} CccChCsPiGroup;

#define MSG_LEVEL_NONE        0
#define MSG_LEVEL_ERROR       MRC_BIT0
#define MSG_LEVEL_WARNING     MRC_BIT1
#define MSG_LEVEL_NOTE        MRC_BIT2
#define MSG_LEVEL_EVENT       MRC_BIT3
#define MSG_LEVEL_ALGO        MRC_BIT4
#define MSG_LEVEL_HAL         MRC_BIT5
#define MSG_LEVEL_MMIO        MRC_BIT6
#define MSG_LEVEL_CSV         MRC_BIT7
#define MSG_LEVEL_TIME        MRC_BIT10
#define MSG_LEVEL_INTERPRETER MRC_BIT11
#define MSG_LEVEL_ALL         MRC_INT32_MAX

// 0:Disable, 1:Error Only, 2:Error & Warnings, 3:Load, Error, Warnings & Info, 4:Load, Error, Warnings, Info & Event, 5:Load, Error, Warnings, Info & Verbose.
#define MSG_LEVEL_COMBO_1 MSG_LEVEL_ERROR
#define MSG_LEVEL_COMBO_2 (MSG_LEVEL_COMBO_1 | MSG_LEVEL_WARNING)
#define MSG_LEVEL_COMBO_3 (MSG_LEVEL_COMBO_2 | MSG_LEVEL_NOTE)
#define MSG_LEVEL_COMBO_4 (MSG_LEVEL_COMBO_3 | MSG_LEVEL_EVENT)
#define MSG_LEVEL_COMBO_5 (MSG_LEVEL_COMBO_4 | MSG_LEVEL_ALGO | MSG_LEVEL_TIME)

typedef UINT32 MrcDebugMsgLevel;

/// Define the frequencies that may be possible in the memory controller.
/// Note that not all these values may be supported.
#define fNoInit     (0)
#define f533        (533)
#define f1067       (1067)
#define f1100       (1100)
#define f1133       (1133)
#define f1200       (1200)
#define f1267       (1267)
#define f1333       (1333)
#define f1400       (1400)
#define f1533       (1533)
#define f1600       (1600)
#define f1667       (1667)
#define f1733       (1733)
#define f1800       (1800)
#define f1867       (1867)
#define f1933       (1933)
#define f2000       (2000)
#define f2100       (2100)
#define f2067       (2067)
#define f2133       (2133)
#define f2200       (2200)
#define f2267       (2267)
#define f2333       (2333)
#define f2400       (2400)
#define f2467       (2467)
#define f2533       (2533)
#define f2600       (2600)
#define f2667       (2667)
#define f2733       (2733)
#define f2750       (2750)
#define f2800       (2800)
#define f2867       (2867)
#define f2933       (2933)
#define f3000       (3000)
#define f3067       (3067)
#define f3133       (3133)
#define f3200       (3200)
#define f3267       (3267)
#define f3300       (3300)
#define f3333       (3333)
#define f3400       (3400)
#define f3467       (3467)
#define f3533       (3533)
#define f3600       (3600)
#define f3667       (3667)
#define f3733       (3733)
#define f3800       (3800)
#define f3867       (3867)
#define f3933       (3933)
#define f4000       (4000)
#define f4067       (4067)
#define f4133       (4133)
#define f4200       (4200)
#define f4267       (4267)
#define f4333       (4333)
#define f4400       (4400)
#define f4467       (4467)
#define f4500       (4500)
#define f4533       (4533)
#define f4600       (4600)
#define f4667       (4667)
#define f4733       (4733)
#define f4800       (4800)
#define f4867       (4867)
#define f4933       (4933)
#define f5000       (5000)
#define f5067       (5067)
#define f5133       (5133)
#define f5200       (5200)
#define f5267       (5267)
#define f5333       (5333)
#define f5400       (5400)
#define f5467       (5467)
#define f5500       (5500)
#define f5533       (5533)
#define f5600       (5600)
#define f5667       (5667)
#define f5733       (5733)
#define f5800       (5800)
#define f5867       (5867)
#define f5933       (5933)
#define f6000       (6000)
#define f6067       (6067)
#define f6133       (6133)
#define f6200       (6200)
#define f6267       (6267)
#define f6333       (6333)
#define f6400       (6400)
#define f6533       (6533)
#define f6667       (6667)
#define f6800       (6800)
#define f6933       (6933)
#define f7067       (7067)
#define f7200       (7200)
#define f7333       (7333)
#define f7400       (7400)
#define f7467       (7467)
#define f7500       (7500)
#define f7600       (7600)
#define f7733       (7733)
#define f7867       (7867)
#define f8000       (8000)
#define f8133       (8133)
#define f8267       (8267)
#define f8400       (8400)
#define f8533       (8533)
#define f8667       (8667)
#define f8800       (8800)
#define f8933       (8933)
#define f9067       (9067)
#define f9200       (9200)
#define f9333       (9333)
#define f9467       (9467)
#define f9600       (9600)
#define f9733       (9733)
#define f9867       (9867)
#define f10000      (10000)
#define f10133      (10133)
#define f10267      (10267)
#define f10400      (10400)
#define f10533      (10533)
#define f10667      (10667)
#define f10800      (10800)
#define f10933      (10933)
#define f11067      (11067)
#define f11200      (11200)
#define f11333      (11333)
#define f11467      (11467)
#define f11600      (11600)
#define f11733      (11733)
#define f11867      (11867)
#define f12000      (12000)
#define f12133      (12133)
#define f12267      (12267)
#define f12400      (12400)
#define f12533      (12533)
#define f12667      (12667)
#define f12800      (12800)
#define f12933      (12933)
#define f13067      (13067)
#define f13200      (13200)
#define f13333      (13333)
#define f13467      (13467)
#define f13600      (13600)
#define f13733      (13733)
#define f13867      (13867)
#define f14000      (14000)
#define f14133      (14133)
#define f14267      (14267)
#define f14400      (14400)
#define f14533      (14533)
#define f14667      (14667)
#define f14800      (14800)
#define f14933      (14933)
#define f15067      (15067)
#define f16000      (16000)
#define fInvalid    (0xFFFFFFFF)
typedef UINT32 MrcFrequency;

#define REF_FREQ 33

// Max supported frequency in OC mode
#define MAX_FREQ_OC   f15067

// Memory Max POR speeds
#define LPDDR_MAX_POR_SPEED (f8400)
#define DDR_MAX_POR_SPEED_DTHALO (f6400)
#define DDR_MAX_POR_SPEED_ULXULT (f5600)

// tCK value in femtoseconds for various frequencies
// If Freq is in MHz, then tCK[fs] = 10^9 * 1/(Freq/2)

// Gear 2 cadence 33.34 * 2
#define MRC_DDR_533_TCK_MIN     3752345
#define MRC_DDR_1067_TCK_MIN    1875000
#define MRC_DDR_1134_TCK_MIN    1764700
#define MRC_DDR_1200_TCK_MIN    1666667
#define MRC_DDR_1267_TCK_MIN    1578947
#define MRC_DDR_1334_TCK_MIN    1499996
#define MRC_DDR_1400_TCK_MIN    1428571
#define MRC_DDR_1534_TCK_MIN    1304348
#define MRC_DDR_1600_TCK_MIN    1250000
#define MRC_DDR_1667_TCK_MIN    1200000
#define MRC_DDR_1733_TCK_MIN    1153846
#define MRC_DDR_1800_TCK_MIN    1111111
#define MRC_DDR_1867_TCK_MIN    1071429
#define MRC_DDR_1933_TCK_MIN    1034483
#define MRC_DDR_2000_TCK_MIN    1000000
#define MRC_DDR_2067_TCK_MIN    967742
#define MRC_DDR_2133_TCK_MIN    937500
#define MRC_DDR_2200_TCK_MIN    909091
#define MRC_DDR_2267_TCK_MIN    882353
#define MRC_DDR_2333_TCK_MIN    857143
#define MRC_DDR_2400_TCK_MIN    833333
#define MRC_DDR_2467_TCK_MIN    810811
#define MRC_DDR_2533_TCK_MIN    789474
#define MRC_DDR_2600_TCK_MIN    769231
#define MRC_DDR_2667_TCK_MIN    750000
#define MRC_DDR_2733_TCK_MIN    731707
#define MRC_DDR_2750_TCK_MIN    727272
#define MRC_DDR_2800_TCK_MIN    714286
#define MRC_DDR_2867_TCK_MIN    697674
#define MRC_DDR_2933_TCK_MIN    681818
#define MRC_DDR_3000_TCK_MIN    666667
#define MRC_DDR_3067_TCK_MIN    652174
#define MRC_DDR_3133_TCK_MIN    638298
#define MRC_DDR_3200_TCK_MIN    625000
#define MRC_DDR_3267_TCK_MIN    612245
#define MRC_DDR_3333_TCK_MIN    600000
#define MRC_DDR_3400_TCK_MIN    588235
#define MRC_DDR_3467_TCK_MIN    576923
#define MRC_DDR_3533_TCK_MIN    566038
#define MRC_DDR_3600_TCK_MIN    555556
#define MRC_DDR_3667_TCK_MIN    545455
#define MRC_DDR_3733_TCK_MIN    535714
#define MRC_DDR_3800_TCK_MIN    526316
#define MRC_DDR_3867_TCK_MIN    517241
#define MRC_DDR_3933_TCK_MIN    508475
#define MRC_DDR_4000_TCK_MIN    500000
#define MRC_DDR_4067_TCK_MIN    491803
#define MRC_DDR_4133_TCK_MIN    483871
#define MRC_DDR_4200_TCK_MIN    476190
#define MRC_DDR_4267_TCK_MIN    468750
#define MRC_DDR_4333_TCK_MIN    461538
#define MRC_DDR_4400_TCK_MIN    454545
#define MRC_DDR_4467_TCK_MIN    447761
#define MRC_DDR_4533_TCK_MIN    441176
#define MRC_DDR_4600_TCK_MIN    434783
#define MRC_DDR_4667_TCK_MIN    428571
#define MRC_DDR_4733_TCK_MIN    422535
#define MRC_DDR_4800_TCK_MIN    416667
#define MRC_DDR_4867_TCK_MIN    410959
#define MRC_DDR_4933_TCK_MIN    405405
#define MRC_DDR_5000_TCK_MIN    400000
#define MRC_DDR_5067_TCK_MIN    394737
#define MRC_DDR_5133_TCK_MIN    389610
#define MRC_DDR_5200_TCK_MIN    384615
#define MRC_DDR_5267_TCK_MIN    379747
#define MRC_DDR_5333_TCK_MIN    375000
#define MRC_DDR_5400_TCK_MIN    370370
#define MRC_DDR_5467_TCK_MIN    365854
#define MRC_DDR_5500_TCK_MIN    363636
#define MRC_DDR_5533_TCK_MIN    361446
#define MRC_DDR_5600_TCK_MIN    357143
#define MRC_DDR_5667_TCK_MIN    352941
#define MRC_DDR_5733_TCK_MIN    348837
#define MRC_DDR_5800_TCK_MIN    344828
#define MRC_DDR_5867_TCK_MIN    340909
#define MRC_DDR_5933_TCK_MIN    337079
#define MRC_DDR_6000_TCK_MIN    333333
#define MRC_DDR_6067_TCK_MIN    329670
#define MRC_DDR_6133_TCK_MIN    326087
#define MRC_DDR_6200_TCK_MIN    322581
#define MRC_DDR_6267_TCK_MIN    319149
#define MRC_DDR_6333_TCK_MIN    315789
#define MRC_DDR_6400_TCK_MIN    312500

// Gear 4 cadence - 33.34 x 4
#define MRC_DDR_6533_TCK_MIN     306122
#define MRC_DDR_6667_TCK_MIN     300000
#define MRC_DDR_6800_TCK_MIN     294118
#define MRC_DDR_6933_TCK_MIN     288462
#define MRC_DDR_7067_TCK_MIN     283019
#define MRC_DDR_7200_TCK_MIN     277778
#define MRC_DDR_7333_TCK_MIN     272727
#define MRC_DDR_7467_TCK_MIN     267857
#define MRC_DDR_7500_TCK_MIN     266667
#define MRC_DDR_7600_TCK_MIN     263158
#define MRC_DDR_7733_TCK_MIN     258621
#define MRC_DDR_7867_TCK_MIN     254237
#define MRC_DDR_8000_TCK_MIN     250000
#define MRC_DDR_8133_TCK_MIN     245902
#define MRC_DDR_8267_TCK_MIN     241935
#define MRC_DDR_8400_TCK_MIN     238095
#define MRC_DDR_8533_TCK_MIN     234375
#define MRC_DDR_8667_TCK_MIN     230769
#define MRC_DDR_8800_TCK_MIN     227273
#define MRC_DDR_8933_TCK_MIN     223881
#define MRC_DDR_9067_TCK_MIN     220588
#define MRC_DDR_9200_TCK_MIN     217391
#define MRC_DDR_9333_TCK_MIN     214286
#define MRC_DDR_9467_TCK_MIN     211268
#define MRC_DDR_9600_TCK_MIN     208333
#define MRC_DDR_9733_TCK_MIN     205479
#define MRC_DDR_9867_TCK_MIN     202703
#define MRC_DDR_10000_TCK_MIN    200000
#define MRC_DDR_10133_TCK_MIN    197368
#define MRC_DDR_10267_TCK_MIN    194805
#define MRC_DDR_10400_TCK_MIN    192308
#define MRC_DDR_10533_TCK_MIN    189873
#define MRC_DDR_10667_TCK_MIN    187500
#define MRC_DDR_10800_TCK_MIN    185185
#define MRC_DDR_10933_TCK_MIN    182927
#define MRC_DDR_11067_TCK_MIN    180723
#define MRC_DDR_11200_TCK_MIN    178571
#define MRC_DDR_11333_TCK_MIN    176471
#define MRC_DDR_11467_TCK_MIN    174419
#define MRC_DDR_11600_TCK_MIN    172414
#define MRC_DDR_11733_TCK_MIN    170455
#define MRC_DDR_11867_TCK_MIN    168539
#define MRC_DDR_12000_TCK_MIN    166667
#define MRC_DDR_12133_TCK_MIN    164835
#define MRC_DDR_12267_TCK_MIN    163043
#define MRC_DDR_12400_TCK_MIN    161290
#define MRC_DDR_12533_TCK_MIN    159574
#define MRC_DDR_12667_TCK_MIN    157895
#define MRC_DDR_12800_TCK_MIN    156250
#define MRC_DDR_12933_TCK_MIN    154639
#define MRC_DDR_13067_TCK_MIN    153061
#define MRC_DDR_13200_TCK_MIN    151515
#define MRC_DDR_13333_TCK_MIN    150000
#define MRC_DDR_13467_TCK_MIN    148515
#define MRC_DDR_13600_TCK_MIN    147059
#define MRC_DDR_13733_TCK_MIN    145631
#define MRC_DDR_13867_TCK_MIN    144231
#define MRC_DDR_14000_TCK_MIN    142857
#define MRC_DDR_14133_TCK_MIN    141509
#define MRC_DDR_14267_TCK_MIN    140187
#define MRC_DDR_14400_TCK_MIN    138889
#define MRC_DDR_14533_TCK_MIN    137615
#define MRC_DDR_14667_TCK_MIN    136364
#define MRC_DDR_14800_TCK_MIN    135135
#define MRC_DDR_14933_TCK_MIN    133929
#define MRC_DDR_15067_TCK_MIN    132743
#define MRC_DDR_16000_TCK_MIN    125000

/// Define the memory nominal voltage (VDD).
/// Note that not all these values may be supported.
typedef enum {
  VDD_INVALID,
  VDD_0_30    =  300,
  VDD_0_40    =  400,
  VDD_0_50    =  500,
  VDD_0_60    =  600,
  VDD_0_65    =  650,
  VDD_0_70    =  700,
  VDD_0_75    =  750,
  VDD_0_80    =  800,
  VDD_0_85    =  850,
  VDD_0_875   =  875,
  VDD_0_90    =  900,
  VDD_1_00    = 1000,
  VDD_1_05    = 1050,
  VDD_1_10    = 1100,
  VDD_1_15    = 1150,
  VDD_1_20    = 1200,
  VDD_1_25    = 1250,
  VDD_1_30    = 1300,
  VDD_1_35    = 1350,
  VDD_1_40    = 1400,
  VDD_1_45    = 1450,
  VDD_1_50    = 1500,
  VDD_1_55    = 1550,
  VDD_1_60    = 1600,
  VDD_1_65    = 1650,
  VDD_1_70    = 1700,
  VDD_1_75    = 1750,
  VDD_1_80    = 1800,
  VDD_1_85    = 1850,
  VDD_1_90    = 1900,
  VDD_1_95    = 1950,
  VDD_2_00    = 2000,
  VDD_2_05    = 2050,
  VDD_2_10    = 2100,
  VDD_2_15    = 2150,
  VDD_2_20    = 2200,
  VDD_2_25    = 2250,
  VDD_2_30    = 2300,
  VDD_2_35    = 2350,
  VDD_2_40    = 2400,
  VDD_2_45    = 2450,
  VDD_2_50    = 2500,
  VDD_2_55    = 2550,
  VDD_2_60    = 2600,
  VDD_2_65    = 2650,
  VDD_2_70    = 2700,
  VDD_2_75    = 2750,
  VDD_2_80    = 2800,
  VDD_2_85    = 2850,
  VDD_2_90    = 2900,
  VDD_2_95    = 2950,
  VDD_MAXIMUM = 0x7FFFFFFF
} MrcVddSelect;

/// DIMM SPD Security Status
typedef enum {
  MrcSpdStatusGood,      ///< Memory is in a secure state.
  MrcSpdStatusAliased,   ///< Memory is aliased.
  MrcSpdStatusLast       ///< Must be last in the list
} MrcSpdStatus;

/// Define the board types.
typedef enum {
  btCRBMB,    ///< 0 - CRB Mobile
  btCRBDT,    ///< 1 - CRB Desktop
  btUser1,    ///< 2 - SV Karkom
  btUser2,    ///< 3 - SV desktop
  btUser3,    ///< 4 - SV miniDVP
  btUser4,    ///< 5 - Ult
  btCRBEMB,   ///< 6 - CRB Embedded
  btReserved, ///< 7 - Reserved
  btUnknown,  ///< 8 - Unknown
  btMaximum   ///< Delimiter
} MrcBoardType;

typedef union {
  struct {
    UINT8   Version;
    UINT8   Derivative;
    UINT8   Segment;
    UINT8   Stepping;
  } Bits;
  UINT64 Data;
} MrcMcIpVersion;

typedef union {
  struct {
    UINT8   Version;
    UINT8   Derivative;
    UINT8   Segment;
    UINT8   Stepping;
  } Bits;
  UINT64 Data;
} MrcDdrIoIpVersion;

/// Define the CPU SKU Types
typedef enum {
  MrcSkuTypeUnspecified,
  MrcSkuTypeM,
  MrcSkuTypeP,
  MrcSkuTypeS,
  MrcSkuTypeU,
  MrcSkuTypeH12Xe,
  MrcSkuTypeH4Xe,
  MrcSkuTypeMax
} MrcSkuType;

/// Define the IP Segment.
typedef enum {
  IpSegmentDesktop,      ///< DT/HALO
  IpSegmentMobile = 3,   ///< P/M
} MrcIpSegment;

#define MC_PTLPUH_IP_SEGMENT 0xA8
#define DDRIO_PTLPUH_IP_SEGMENT 0x3

#define DDRIO_NVLPUH_IP_SEGMENT 0x3
#define DDRIO_NVLDT_IP_SEGMENT 0x1

/// Define the Version of IP for DDRIO.
typedef enum {
  ipVerDdrIoMtl = 7,         ///< Gen7: MeteorLake
  ipVerDdrIoLnlPtl = 8,      ///< Gen8: LunarLake / PantherLake
#ifdef PRODUCT_RZLWS
  ipVerDdrIoNvl = 10         ///< Gen10: NovaLake
#endif
} MrcDdrIoIpVer;

/// Define the Version of IP for MC.
typedef enum {
  ipVerMcMtl = 9,       ///< Gen9: MeteorLake
  ipVerMcLnlPtl = 10,   ///< Gen10: LunarLake/PantherLake
#ifdef PRODUCT_RZLWS
  ipVerMcNvl = 11,      ///< Gen11: NovaLake
#endif
} MrcMcIpVer;

/// Define the MRC Test Environment
typedef enum {
  MrcTestEnvPhy,
  MrcTestEnvMc,
  MrcTestEnvMcDdrio
} MrcIpTestEnv;

typedef enum {
  CONTROLLER_NOT_PRESENT, ///< There is no controller present in the system.
  CONTROLLER_DISABLED,    ///< There is a controller present but it is disabled.
  CONTROLLER_PRESENT,     ///< There is a controller present and it is enabled.
  MAX_CONTROLLER_STATUS   ///< Delimiter
} MrcControllerSts;

typedef enum {
  CHANNEL_NOT_PRESENT,    ///< There is no channel present on the controller.
  CHANNEL_DISABLED,       ///< There is a channel present but it is disabled.
  CHANNEL_PRESENT,        ///< There is a channel present and it is enabled.
  MAX_CHANNEL_STATUS      ///< Delimiter
} MrcChannelSts;

typedef enum {
  DIMM_ENABLED,           ///< DIMM/rank Pair is enabled, presence will be detected.
  DIMM_DISABLED,          ///< DIMM/rank Pair is disabled, regardless of presence.
  DIMM_PRESENT,           ///< There is a DIMM present in the slot/rank pair and it will be used.
  DIMM_NOT_PRESENT,       ///< There is no DIMM present in the slot/rank pair.
  MAX_DIMM_STATUS         ///< Delimiter
} MrcDimmSts;

typedef enum {
  STD_PROFILE,            ///< Standard DIMM profile select.
  CUSTOM_PROFILE1,        ///< User specifies various override values.
  /*
   * On DDR5, XMP3.0 has 5 profiles:
   *    Profile 1 (Enthusiast / Certified Settings)
   *    Profile 2 (Extreme Settings)
   *    Profile 3 (Extreme Settings)
   *    Profile 4 (User Settings)
   *    Profile 5 (User Settings)
   */
  XMP_PROFILE1,           ///< XMP enthusiast settings select (XMP profile #1).
  XMP_PROFILE2,           ///< XMP extreme settings select (XMP profile #2).
  XMP_PROFILE3,           ///< XMP extreme settings select (XMP profile #3), XMP3.0 only.
  USER_PROFILE4,          ///< User Settings Profile4, XMP3.0 only.
  USER_PROFILE5,          ///< User Settings Profile5, XMP3.0 only.
  MAX_PROFILE             ///< Delimiter
} MrcProfile;

typedef UINT32 MrcBClkRef;   ///< Base clock, in Hertz, Default is 100MHz or leave at zero for default.

// This encoding matches SC_GS_CFG.dram_technology and MAD_INTER_CHANNEL.DDR_TYPE registers
typedef enum {
  MrcIterationClock,
  MrcIterationCmd,
  MrcIterationCtl,
  MrcIterationCmdCtl,
  MrcIterationMax
} MrcIterationType;

typedef enum {
  UpmLimit,
  PowerLimit,
  RetrainLimit,
  MarginLimitMax
} MRC_MARGIN_LIMIT_TYPE;

typedef enum {
  Rfm,             // Default RFM
  ArfmLevelA,      // ARFM Setting A
  ArfmLevelB,      // ARFM Setting B
  ArfmLevelC,      // ARFM Setting C
  DramRfmDisabled  // ARFM Disabled
} MrcDramRfmModeType;

typedef enum {
  Drfm,        // Directed Refresh Management (same aggressor detection algo)
  Ptrr,        // Pseudo Targeted Row Refresh
  TrrDisabled  // Targeted Row Refresh Disabled
} MrcTargetedRowRefreshModeType;

typedef enum {
  RhDisable,
  RhRfm,  //RFM first, Fallback to PTRR if RFM is unavailable
  RhPtrr
} MrcRhSelectType;

typedef enum {
  DrfmBrcDisabled = 0,
  DrfmBrc2        = 2,  // Always triggers refresh of 1 victim row and may target a 2nd at a reduced rate
  DrfmBrc3        = 3,  // Always triggers refresh of 2 victim rows and may target a 3rd at a reduced rate
  DrfmBrc4        = 4   // Always triggers refresh of 3 victim rows and may target a 4th at a reduced rate
} MrcRhDrfmBrcType;

typedef enum {
  RefreshNORMAL,
  RefreshHalfx,
  Refresh1x,
  Refresh2x,
  Refresh4x,
  RefreshMax
} MrcRefreshRateType;

typedef enum {
  OneIn2To9 = 9,
  OneIn2To10,
  OneIn2To11,
  OneIn2To12,
} MrcRhProbType;

typedef enum {
  MRC_POST_CODE,
  MRC_POST_CODE_WRITE,
  MRC_POST_CODE_READ,
  MRC_POST_CODE_MAX
} MrcDebugPostCode;

/// Enumeration used to determine which test engine the product supports.
typedef enum {
  MrcTeCpgc20,
  MrcTeMax
} MRC_TE_TYPE;

/// Enumeration for ODT Mode selection via Input Parameter and current ODT Mode.
typedef enum {
  MrcOdtModeDefault,
  MrcOdtModeVddq,
  MrcOdtModeVss,
  MrcOdtModeMax
} MRC_ODT_MODE_TYPE;

/// Generic enumeration interface for parameters that are Auto, Enable, Disable inputs
typedef enum {
  MrcAuto,
  MrcEnable,
  MrcDisable,
  MrcEnDisAutoMax
} MRC_EN_DIS_AUTO_TYPE;

// Enumeration Margin Limit Check mode
typedef enum {
  Margin_Check_Disable,
  Margin_Check_L1,
  Margin_Check_L2,
  Margin_Check_Both
} MarginCheckMode;

// CCE Config Enumerator
typedef enum {
  MrcCceDisable,     ///< Don't enable the CCEs
  MrcCceEnableAll,   ///< Enable all CCEs
  MrcCceEnablePerMc, ///< Enable CCEs based to MC population
  MrcCceEnable0,     ///< Enable CCE0 only
  MrcCceEnable1,     ///< Enable CCE1 only
  MrcCceEnSingle,    ///< Enable CCE0 only if MC0 is populated
} MRC_CCE_CONFIG;

typedef struct {
  POINTER_STRUCT MrcData;
  POINTER_STRUCT Stream;
  POINTER_STRUCT Start;
  POINTER_STRUCT End;
  POINTER_STRUCT Current;
  UINT32 Level;
  UINT16 PostCode[MRC_POST_CODE_MAX];
  POINTER_STRUCT TopStackAddr;     ///< Initial stack address.
  POINTER_STRUCT LowestStackAddr;  ///< Track the lowest stack address used through MrcPrintfVaList()
  UINT8 Dummy8AlignmentBuffer[2];
} MrcDebug;

typedef UINT16 MrcPostCode;
typedef UINT16 MrcClockRatio;  ///< This value times the REF_FREQ determines the MrcFrequency.

/// This data structure contains all the "DDR power saving data" values that are considered output by the MRC.
/// The following are memory controller level definitions. All channels on a controller are set to these values.

/// MRC version description.
typedef struct {
  UINT8  Major;  ///< Major version number
  UINT8  Minor;  ///< Minor version number
  UINT8  Rev;    ///< Revision number
  UINT8  Build;  ///< Build number
} MrcVersion;

// This is used to locate MRC version in a binary BIOS file
typedef struct {
  UINT64     Signature;
  MrcVersion Version;
} MrcVersionWithSignature;

typedef struct {
  UINT32   BaseAddress;
  UINT32   Length;
} MemoryMapHole;

/// Memory map configuration information.
typedef struct {
  UINT32     TomMinusMe;
  UINT32     ToludBase;
  UINT32     BdsmBase;
  UINT32     GttBase;
  UINT32     TsegBase;
  BOOLEAN    ReclaimEnable;
  UINT8      Dummy8AlignmentBuffer[3];
  UINT32     RemapBase;
  UINT32     RemapLimit;
  UINT32     TouudBase;
  UINT32     TotalPhysicalMemorySize;
  UINT32     McMemorySize[MAX_CONTROLLER];
  UINT32     MeStolenBase;
  UINT32     MeStolenSize;
  UINT32     TseImrBase;
  UINT32     TseImrSize;
  UINT32     PrmrrBase;
  UINT32     LowestBase;
  UINT32     StreamTracerSize;
  UINT32     StreamTracerBase;
  UINT64     SeamrrBase;   ///< Holds the Seamrr base
  MemoryMapHole  MemoryHoles[MEMORY_MAP_MAX_HOLE];
  UINT32     SharedMailboxBase;
  UINT64     TopUseableMemAddr;
  UINT64     TopUseableMemSize;
} MrcMemoryMap;

/// DIMM timings
typedef struct {
  UINT32 tCK;         ///< Offset 0  Memory cycle time, in femtoseconds.
  UINT16 NMode;       ///< Offset 4  Number of tCK cycles for the channel DIMM's command rate mode.
  UINT16 tCL;         ///< Offset 6  Number of tCK cycles for the channel DIMM's CAS latency.
  UINT16 tCWL;        ///< Offset 8  Number of tCK cycles for the channel DIMM's minimum CAS write latency time.
  UINT16 tFAW;        ///< Offset 10 Number of tCK cycles for the channel DIMM's minimum four activate window delay time.
  UINT16 tRAS;        ///< Offset 12 Number of tCK cycles for the channel DIMM's minimum active to precharge delay time.
  UINT16 tRCDtRP;     ///< Offset 14 Number of tCK cycles for the channel DIMM's minimum RAS# to CAS# delay time and Row Precharge delay time.
  UINT32 tREFI;       ///< Offset 16 Number of tCK cycles for the channel DIMM's minimum Average Periodic Refresh Interval.
  UINT16 tRFC;        ///< Offset 20 Number of tCK cycles for the channel DIMM's minimum refresh recovery delay time.
  UINT16 tRFCpb;      ///< Offset 22 Number of tCK cycles for the channel DIMM's minimum per bank refresh recovery delay time.
  UINT16 tRFC2;       ///< Offset 24 Number of tCK cycles for the channel DIMM's minimum refresh recovery delay time.
  UINT16 tRFC4;       ///< Offset 26 Number of tCK cycles for the channel DIMM's minimum refresh recovery delay time.
  UINT16 tRPab;       ///< Offset 28 Number of tCK cycles for the channel DIMM's minimum row precharge delay time for all banks.
  UINT16 tRRD;        ///< Offset 30 Number of tCK cycles for the channel DIMM's minimum row active to row active delay time.
  UINT16 tRRD_L;      ///< Offset 32 Number of tCK cycles for the channel DIMM's minimum row active to row active delay time for same bank groups.
  UINT16 tRRD_S;      ///< Offset 34 Number of tCK cycles for the channel DIMM's minimum row active to row active delay time for different bank groups.
  UINT16 tRTP;        ///< Offset 36 Number of tCK cycles for the channel DIMM's minimum internal read to precharge command delay time.
  UINT16 tWR;         ///< Offset 38 Number of tCK cycles for the channel DIMM's minimum write recovery time.
  UINT16 tWTR;        ///< Offset 40 Number of tCK cycles for the channel DIMM's minimum internal write to read command delay time.
  UINT16 tWTR_L;      ///< Offset 42 Number of tCK cycles for the channel DIMM's minimum internal write to read command delay time for same bank groups.
  UINT16 tWTR_S;      ///< Offset 44 Number of tCK cycles for the channel DIMM's minimum internal write to read command delay time for different bank groups.
  UINT16 tCCD_L;      ///< Offset 46 Number of tCK cycles for the channel DIMM's minimum CAS-to-CAS delay for same bank group.
  UINT16 tCCD_L_WR;   ///< Offset 48 Number of tCK cycles for the channel DIMM's minimum Write-to-Write delay for same bank group.
  UINT8  Resv[2];     ///< Offset 50
} MrcTiming;

/// This data structure contains IP specific timing values that are considered output by the MRC.
typedef struct {
  UINT16 tRDPRE;     ///< Read CAS to Precharge cmd delay
} MrcIpTiming;

typedef struct {
  INT32 Mtb;    ///< Medium time base.
  INT32 Ftb;    ///< Fine time base.
} MrcTimeBase;

typedef struct {
  UINT8  Left;   ///< The left side of the timing eye.
  UINT8  Center; ///< The center of the timing eye.
  UINT8  Right;  ///< The right side of the timing eye.
} MrcDqTimeMargin;

typedef struct {
  INT8  High;   ///< The high side of the Vref eye.
  INT8  Center; ///< The center of the Vref eye.
  INT8  Low;    ///< The low side of the Vref eye.
} MrcDqVrefMargin;

typedef struct {
  UINT8  Left;   ///< The left side of the command eye.
  UINT8  Right;  ///< The right side of the command eye.
  UINT8  High;   ///< The high side of the command eye.
  UINT8  Low;    ///< The low side of the command eye.
} MrcCommandMargin;

typedef struct {
  UINT8  Left;   ///< The left side of the receive enable eye.
  UINT8  Right;  ///< The right side of the receive enable eye.
} MrcRecvEnMargin;

typedef struct {
  UINT8  Left;   ///< The left side of the write leveling eye.
  UINT8  Right;  ///< The right side of the write leveling eye.
} MrcWrLevelMargin;

/// The struct defines must be aligned to the DDRIO layout defines in MrcDdrIoApi.h
/// We cannot include the header here to use the symbol because MrcDdrIoApi.h depends on CMrcInterface.h
typedef struct {
  UINT32  RefPiCtrlData[10][2]; ///< [MRC_DATA_PAR_DT_NUM][MRC_DATA_CH_NUM]
  UINT32  RefPiCtrlCcc[8];      ///< [MRC_CCC_NUM]
} MrcRefPiControl;

typedef struct {
  UINT32  RefPiCtrl0Data[10];   ///< [MRC_DATA_DT_NUM]
  UINT32  RefPiCtrl1Data[10];   ///< [MRC_DATA_DT_NUM]
  UINT32  RefPiCtrl0Ccc[4];     ///< [MRC_CCC_SHARED_DT_NUM]
  UINT32  RefPiCtrl1Ccc[4];     ///< [MRC_CCC_SHARED_DT_NUM]
} MrcRefPiCal;

typedef union {
  MrcRefPiControl RefPiControl;
  MrcRefPiCal     RefPiCalData;
} MRC_REF_PI_FREQ_SWITCH_DATA;

typedef struct {
  UINT32  DllControl0Data[10][2]; ///< [MRC_DATA_DT_NUM][MRC_DATA_CH_NUM]
  UINT32  DllControl1DataSh[10];  ///< [MRC_DATA_DT_NUM]
  UINT32  QclkPhclockRepeater0;
  UINT32  PhClockRepeater0DataSh;
  UINT32  PhClockRepeater0CccSh;
} MrcBwSel;

typedef struct {
  UINT8 DataVccClkWpLvrVrefSel[8];        ///< [MRC_DATA_MOBILE_NUM]
  UINT8 DataVccClkWpLvrRxdllb1vrefsel[8]; ///< [MRC_DATA_MOBILE_NUM]
  UINT8 DataVccClkWpLvrRxdllb0vrefsel[8]; ///< [MRC_DATA_MOBILE_NUM]
  UINT8 DataVccIogWpLvrVrefSel[8];        ///< [MRC_DATA_MOBILE_NUM]
  UINT8 CccVccClkWpLvrVrefSel[4];         ///< [MRC_CCC_SBMEM_NUM]
  UINT8 CccVccIogWpLvrVrefSel[4];         ///< [MRC_CCC_SBMEM_NUM]
  UINT8 CompVccIogWpLvrVrefSel;
  UINT8 WorkPointDISTGLVRVrefSel;
  UINT8 CompVccIog40WpLvrVrefSel;
} MrcLvrSaveRestore;

typedef struct {
  UINT8     SpdValid[sizeof (MrcSpd) / (CHAR_BITS * sizeof (UINT8))]; ///< Each valid bit maps to SPD byte.
  UINT8     MrcSpdString[3]; ///< The SPD data start marker. This must be located at the start of the SPD data structure. It includes this string plus the following flag.
  union {
    struct {
      UINT8   DimmNumber    : 4; ///< SPD zero based DIMM number.
      UINT8   ChannelNumber : 3; ///< SPD zero based channel number.
      UINT8   MdSocket      : 1; ///< 0 = memory down, 1 = socketed.
    } Bit;
    UINT8 Data;
  } Flag;
  MrcSpd Data;            ///< The SPD data for each DIMM. SPDGeneral field = 0 when absent.
} MrcSpdData;

typedef struct {
  UINT32 Enable; // 0 - MRC use Data in Struct, 1 - MRC uses Data in Struct
  // Read
  UINT32 RcvEnPi[MAX_CONTROLLER][MAX_RANK_IN_CHANNEL][MAX_SYS_SDRAM];
  UINT32 tCL4RcvEn[MAX_SYS_CHANNEL];
  UINT32 tCl4RxFifoRdEn[MAX_SYS_CHANNEL];
  UINT32 RxFifoRdEnRank[MAX_SYS_CHANNEL][MAX_RANK_IN_CHANNEL];
  // Write
  UINT32 TxDqs[MAX_CONTROLLER][MAX_RANK_IN_CHANNEL][MAX_SYS_SDRAM];
  UINT32 TxDq[MAX_CONTROLLER][MAX_RANK_IN_CHANNEL][MAX_SYS_SDRAM];
  UINT32 tCWL4TxDqFifoRdEn[MAX_SYS_CHANNEL];
  // WCK
  UINT32 WckPi;
} MrcDdrioFlyBy;

typedef struct {
  BOOLEAN IsDirty;
  UINT8   Data;
} CKD_STRUCT;

typedef struct {
  CKD_STRUCT Data[MAX_WRITE_CKD];
  UINT8      CkdAddress;
} MRC_CKD_BUFFER;

///*****************************************
/// Output related "global data" structures.
///*****************************************
/// This data structure contains all the "global data" values that are considered output by the MRC.
/// The following are SDRAM level definitions. All ranks on a rank are set to these values.

/* Commented out until needed, in order to save space.
typedef struct {
} MrcSdramOut;
*/

/// This data structure contains all the "global data" values that are considered output by the MRC.
/// The following are rank level definitions. All ranks on a DIMM are set to these values.
typedef struct {
//MrcSdramOut Sdram[MAX_SDRAM_IN_DIMM];       ///< The following are SDRAM level definitions.
  UINT8       MR[MAX_MR_IN_DIMM];                      ///< DRAM mode register values, indexed by MrcModeRegisterIndex enum.
  UINT8       DdrPdaVrefDq[MAX_BYTE_IN_DDR5_CHANNEL];  ///< DDR5 MR10 [7:0] for per-DRAM VrefDQ (PDA)
  UINT8       DdrPdaVrefCmd[MAX_BYTE_IN_DDR5_CHANNEL]; ///< DDR5 MR11 [7:0] for per-DRAM VrefCmd (PDA)
  UINT8       Ddr5PdaMr3[MAX_BYTE_IN_DDR5_CHANNEL];    ///< DDR5 MR3 for per-DRAM
  UINT8       Ddr5PdaMr7[MAX_BYTE_IN_DDR5_CHANNEL];    ///< DDR5 MR7 for per-DRAM
  UINT8       Ddr5PdaCaOdtStrap[MAX_BYTE_IN_DDR5_CHANNEL];  ///< DDR5 MR32 CA ODT Strap per-DRAM
  UINT8       Ddr5PdaMr48[MAX_BYTE_IN_DDR5_CHANNEL];   ///< DDR5 MR48 for per-DRAM
  UINT8       Ddr5PdaMr129[MAX_BYTE_IN_DDR5_CHANNEL];  ///< DDR5 MR129 for per-DRAM DFE Tap1 Lower Byte DQ[0]
  UINT8       Ddr5PdaMr130[MAX_BYTE_IN_DDR5_CHANNEL];  ///< DDR5 MR130 for per-DRAM DFE Tap2 Lower Byte DQ[0]
  UINT8       Ddr5PdaMr131[MAX_BYTE_IN_DDR5_CHANNEL];  ///< DDR5 MR131 for per-DRAM DFE Tap3 Lower Byte DQ[0]
  UINT8       Ddr5PdaMr132[MAX_BYTE_IN_DDR5_CHANNEL];  ///< DDR5 MR132 for per-DRAM DFE Tap4 Lower Byte DQ[0]
  UINT8       Ddr5PdaMr193[MAX_BYTE_IN_DDR5_CHANNEL];  ///< DDR5 MR193 for per-DRAM DFE Tap1 Upper Byte DQ[0]
  UINT8       Ddr5PdaMr194[MAX_BYTE_IN_DDR5_CHANNEL];  ///< DDR5 MR194 for per-DRAM DFE Tap2 Upper Byte DQ[0]
  UINT8       Ddr5PdaMr195[MAX_BYTE_IN_DDR5_CHANNEL];  ///< DDR5 MR195 for per-DRAM DFE Tap3 Upper Byte DQ[0]
  UINT8       Ddr5PdaMr196[MAX_BYTE_IN_DDR5_CHANNEL];  ///< DDR5 MR196 for per-DRAM DFE Tap4 Upper Byte DQ[0]
  //UINT8       Device[MAX_SDRAM_IN_DIMM];      ///< Which Bytes are tied to which Device where BIT0 set means Byte 0 - needed for PDA Mapping, not used currently
} MrcRankOut;

/// This data structure contains all the "global data" values that are considered output by the MRC.
/// The following are DIMM level definitions. All ranks on a DIMM are set to these values.
typedef struct {
  MrcDimmSts     Status;                  ///< See MrcDimmSts for the definition of this field.
  MrcVddSelect   VddVoltage[MAX_PROFILE]; ///< The voltage (VDD) setting for this DIMM, per profile.
  MrcVddSelect   VddqVoltage[MAX_PROFILE];///< The voltage (VDDQ) setting for this DIMM, per profile.
  MrcVddSelect   VppVoltage[MAX_PROFILE]; ///< The voltage (VPP) setting for this DIMM, per profile.
  BOOLEAN        EccSupport;              ///< TRUE if ECC is enabled and supported on this DIMM.
  BOOLEAN        IgnoreNonEccDimm;        ///< TRUE if a DIMM without ECC capability should be ignored.
  BOOLEAN        AddressMirrored;         ///< TRUE if the DIMM is address mirrored.
  BOOLEAN        SelfRefreshTemp;         ///< TRUE if the DIMM supports self refresh extended operating temperature range (SRT).
  BOOLEAN        AutoSelfRefresh;         ///< TRUE if the DIMM supports automatic self refresh (ASR).
  BOOLEAN        PartialSelfRefresh;      ///< TRUE if the DIMM supports Partial Array Self Refresh (PASR).
  BOOLEAN        OnDieThermalSensor;      ///< TRUE if the DIMM supports On-die Thermal Sensor (ODTS) Readout.
  BOOLEAN        ExtendedTemperRange;     ///< TRUE if the DIMM supports Extended Temperature Range (ETR).
  BOOLEAN        ExtendedTemperRefresh;   ///< TRUE if the DIMM supports 1x Extended Temperature Refresh rate, FALSE = 2x.
  MrcDdrType     DdrType;                 ///< DDR type: DDR5, LPDDR5
  MEMORY_PACKAGE ModuleType;              ///< Module type: UDIMM, SO-DIMM, etc.
  UINT32         DimmCapacity;            ///< DIMM size in MBytes.
  UINT32         RowSize;                 ///< The DIMMs row address size.
  UINT16         ColumnSize;              ///< The DIMMs column address size.
  UINT16         Crc;                     ///< Calculated CRC16 of the DIMM's provided SPD. Can be used to detect DIMM change.
  UINT8          RankInDimm;              ///< The number of ranks in this DIMM.
  UINT8          Banks;                   ///< Number of banks the DIMM contains.
  UINT8          BankGroups;              ///< Number of bank groups the DIMM contains.
  UINT8          PrimaryBusWidth;         ///< DIMM primary bus width.
  UINT8          DiePerSdramPackage;      ///< Number of Die in the DRAM Package (LPDDR Only)
  UINT8          ChannelsPerSdramPackage; ///< Number of Channels in the DRAM Package (LPDDR Only)
  UINT8          NumLpSysChannel;         ///< Number of channels at PrimaryBusWidth for this DIMM's SPD (LPDDR Only)
  UINT8          SdramWidth;              ///< DIMM SDRAM width.
  UINT8          SdramWidthIndex;         ///< DIMM SDRAM width index (0 = x4, 1 = x8, 2 = x16, 3 = x32).
  UINT8          DensityIndex;            ///< Total SDRAM capacity index (0 = 256Mb, 1 = 512Mb, 2 = 1Gb, etc).
  UINT8          DeviceDensity;           ///< Device density in Gb
  UINT8          tMAC;                    ///< Maximum Activate Count for pTRR.
  UINT8          ReferenceRawCard;        ///< Indicates which JEDEC reference design raw card was used as the basis for the module assembly.
  UINT8          XmpSupport;              ///< Indicates if XMP profiles are supported. 0 = None, 1 = XMP1 only, 2 = XMP2 only, 3 = All.
  UINT8          XmpRevision;             ///< Indicates the XMP revision of this DIMM. 0 = None, 12h = 1.2, 13h = 1.3.
  UINT8          XmpProfile1Config;       ///< Indicates the XMP Profile1Config of this DIMM. 0 = 1DimmPerCH, 1 = 2DimmPerCH, 2 = 3DimmPerCH, 3 = 4DimmPerCH.
  UINT8          XmpProfile2Config;       ///< Indicates the XMP Profile2Config of this DIMM. 0 = 1DimmPerCH, 1 = 2DimmPerCH, 2 = 3DimmPerCH, 3 = 4DimmPerCH.
  UINT8          XmpProfile3Config;       ///< Indicates the XMP Profile3Config of this DIMM. 0 = 1DimmPerCH, 1 = 2DimmPerCH, 2 = 3DimmPerCH, 3 = 4DimmPerCH.
  UINT8          XmpOverClockFeat[MAX_XMP3_PROFILES];     ///< Indicates the XMP Advanced Memory OverCLock Feature. Bit 0: RealTimeMemoryFreqOC is supported. Bit 1: DynamicMemoryBoost is supported.
  BOOLEAN        PmicProgrammable;        ///< TRUE if the PMIC Programmable Mode is set on DDR5
  BOOLEAN        IsPmicSupport10MVStep;   ///< TRUE if PMIC supports 10mv step size. PMIC must support 5mv step size.
  UINT8          PmicDefaultStepSize;     ///< Either 5 or 10.
  MrcFrequency   Speed;                   ///< Max DIMM speed in the current profile - needed  for SMBIOS
  MrcRankOut     Rank[MAX_RANK_IN_DIMM];  ///< The following are rank level definitions.
  BOOLEAN        IsCkdSupport;            ///< TRUE if CKD is installed on this DIMM.
  UINT8          CkdDimmIndex;            ///< Indicates the index of Physical DIMM (Max up to 4 for 2DPC), this is used when programming CKD Control Word
} MrcDimmOut;

/// This data structure contains all the "global data" values that are considered output by the MRC.
/// The following are channel level definitions. All DIMMs on a memory channel are set to these values.
typedef struct {
  MrcChannelSts     Status;                                                       ///< Indicates whether this channel should be used.
  MrcTimeBase       TimeBase[MAX_DIMMS_IN_CHANNEL][MAX_PROFILE];                  ///< Medium and fine timebases for each DIMM in the channel and each memory profile.
  UINT32            Capacity;                                                     ///< Amount of memory in this channel, in MBytes.
  UINT32            DimmCount;                                                    ///< Number of valid DIMMs that exist in the channel.
  UINT16            WlDelay[MAX_RANK_IN_DIMM][MAX_SDRAM_IN_DIMM];              ///< WlDelay PI Code
  MrcCommandMargin  Command[MAX_RANK_IN_DIMM];                                 ///< Cmd setting
  MrcDqTimeMargin   RxDqPb[MAX_RANK_IN_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS];     ///< Rx PerBit Pi Code
  MrcDqTimeMargin   TxDqPb[MAX_RANK_IN_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS];     ///< Tx PerBit Pi Code
  MrcDqVrefMargin   RxDqVrefPb[MAX_RANK_IN_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS]; ///< Rx PerBit Vref
  MrcDqVrefMargin   TxDqVrefPb[MAX_RANK_IN_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS]; ///< Rx PerBit Vref
  MrcDqTimeMargin   CmdPiGrpPb[MAX_DDR5_CMD_PINS];                                ///< Cmd PerBit Pi Code
  MrcRecvEnMargin   ReceiveEnable[MAX_RANK_IN_DIMM];                           ///< Receive enable per rank
  MrcWrLevelMargin  WriteLevel[MAX_RANK_IN_DIMM];                              ///< Write leveling per rank
  UINT8             ValidRankBitMask;                                             ///< Bit map of the populated ranks per channel
  UINT8             ValidSubChBitMask;                                            ///< Bit map of the populated 32-bit sub-channels per 64-bit channel (2 bits)
  UINT16            ValidByteMask;                                                ///< Bit map of the populated bytes per channel
  UINT8             ByteStart[MAX_SUB_CHANNEL];                                   ///< Defines the CPU Byte start associated with the SubChannel requested as the index to the array.
  UINT8             ByteEnd[MAX_SUB_CHANNEL];                                     ///< Defines the CPU Byte limit associated with the SubChannel requested as the index to the array.
  MrcDimmOut        Dimm[MAX_DIMMS_IN_CHANNEL];                                   ///< DIMM specific output variables.
  UINT8             CpgcChAssign;                                                 ///< Stores the CpgcSeqCfgA.Channel_Assign field
  BOOLEAN           Mr3PdaEnabled;                                                ///< Defines if MR3 (Write Leveling) is required as PDA for this channel.
  BOOLEAN           Mr7PdaEnabled;                                                ///< Defines if MR7 (Write Leveling Half Step) is required as PDA for this channel.
  BOOLEAN           IsMr10PdaEnabled;                                             ///< Defines if MR10 (VrefDQ) is required as PDA for this channel.
  BOOLEAN           IsMr11PdaEnabled;                                             ///< Defines if MR11 (VrefCA) is required as PDA for this channel.
  BOOLEAN           IsMr48PdaEnabled;                                             ///< Defines if MR48 (Write Pattern) is required as PDA for this channel.
  BOOLEAN           MrXPdaDfeTap1Enabled;                                         ///< Defines if MRs of DFE TAP1 is required as PDA for this channel.
  BOOLEAN           MrXPdaDfeTap2Enabled;                                         ///< Defines if MRs of DFE TAP2 is required as PDA for this channel.
  BOOLEAN           MrXPdaDfeTap3Enabled;                                         ///< Defines if MRs of DFE TAP3 is required as PDA for this channel.
  BOOLEAN           MrXPdaDfeTap4Enabled;                                         ///< Defines if MRs of DFE TAP4 is required as PDA for this channel.
} MrcChannelOut;

/// This data structure contains all the "global data" values that are considered output by the MRC.
/// The following are memory controller level definitions. All channels on a controller are set to these values.
typedef struct {
  MrcControllerSts Status;                     ///< Indicates whether this controller should be used.
  UINT16           DeviceId;                   ///< The PCI device id of this memory controller.
  UINT8            RevisionId;                 ///< The PCI revision id of this memory controller.
  UINT8            ChannelCount;               ///< Number of valid channels that exist on the controller.
  UINT8            ValidChBitMask;             ///< Bit mask of valid channels on this memory controller.
  UINT8            FirstPopCh;                 ///< First populated channel
  MrcChannelOut    Channel[MAX_CHANNEL];       ///< The following are channel level definitions.
  MrcIntCmdTimingOut CmdTiming[MAX_CHANNEL];
  MrcIntClkAlignedParam IntClkAlignedMargins[MAX_CHANNEL][MAX_RANK_IN_CHANNEL][MAX_SDRAM_IN_DIMM];
} MrcControllerOut;

///********************************************
/// Saved data related "global data" structures.
///********************************************
/// This data structure contains all the "global data" values that are considered to be needed
/// by the MRC between power state transitions (S0->S3->S0) and also fast and warm boot modes.
/// The following are DIMM level definitions.
typedef struct {
  UINT8  SpdDramDeviceType;       ///< Save SPD DramDeviceType information needed for SMBIOS structure creation.
  UINT8  SpdModuleType;           ///< Save SPD ModuleType information needed for SMBIOS structure creation.
  UINT8  SpdModuleMemoryBusWidth; ///< Save SPD ModuleMemoryBusWidth information needed for SMBIOS structure creation.
  UINT8  SpdSave[MAX_SPD_SAVE];   ///< Save SPD Manufacturing information needed for SMBIOS structure creation.
  UINT16 CkdMfgID;                ///< Save SPD clock driver manufacturer ID needed for SMBIOS structure creation.
  UINT8  CkdDeviceRev;            ///< Save SPD clock driver device revision needed for SMBIOS structure creation.
  UINT16 DramMfgID;               ///< Save SPD DRAM chip manufacturerID needed for SMBIOS structure creation.
} MrcDimmSpdSave;

/// This data structure contains all the "global data" values that are considered to be needed
/// by the MRC between power state transitions (S0->S3->S0) and also fast and warm boot modes.
/// The following are channel level definitions.
typedef struct {
  MrcChannelSts   Status;                                                         ///< Indicates whether this channel should be used.
  UINT32          DimmCount;                                                      ///< Number of valid DIMMs that exist in the channel.
  UINT8           ValidRankBitMask;                                               ///< Bit map of the populated ranks per channel
  UINT8           ValidSubChBitMask;                                              ///< Bit map of the populated sub channels per channel.
  UINT16          ValidByteMask;                                                  ///< Bit map of the populated bytes per channel.
  UINT8           DqsMapCpu2Dram[MAX_DIMMS_IN_CHANNEL][MAX_BYTE_IN_DDR5_CHANNEL]; ///< Mapping from CPU DQS pins to SDRAM DQS pins. LP5 use DIMM0 index only; DDR5 uses both DIMMs
  MrcDimmOut      Dimm[MAX_DIMMS_IN_CHANNEL];                                     ///< Save the DIMM output characteristics.
  MrcDimmSpdSave  DimmSpdSave[MAX_DIMMS_IN_CHANNEL];                              ///< Save SPD information needed for SMBIOS structure creation.
} MrcChannelSave;

/// This data structure contains all the "global data" values that are considered to be needed
/// by the MRC between power state transitions (S0->S3->S0) and also fast and warm boot modes.
/// The following are controller level definitions.
typedef struct {
  MrcControllerSts  Status;               ///< Indicates whether this controller should be used.
  UINT8             ChannelCount;         ///< Number of valid channels that exist on the controller.
  UINT8             FirstPopCh;           ///< First populated channel
  UINT8             ValidChBitMask;       ///< Bit mask of valid channels on this memory controller.
  MrcChannelSave    Channel[MAX_CHANNEL]; ///< The following are channel level definitions.
} MrcContSave;

/// This data structure contains all the "global data" values that are considered to be needed
/// by the MRC between power state transitions (S0->S3->S0) and also fast and warm boot modes.
/// The following are system level definitions.
typedef struct {
  UINT32 Crc;                  ///< The CRC-32 of the data in this structure.
} MrcSaveHeader;

/// This data structure contains per-SaGv timing values that are considered output by the MRC.
typedef struct {
  UINT32      DataRate;    ///< The memory rate for the current SaGv Point in units of MT/s
  MrcTiming   JedecTiming; ///< Timings used for this entry's corresponding SaGv Point - derived from JEDEC SPD spec
  MrcIpTiming IpTiming;    ///< Timings used for this entry's corresponding SaGv Point - IP specific
  UINT16      MaxMemoryBandwidth; ///< Maximum theoretical bandwidth in GB/s supported by GV
} MrcSaGvTimingOut;

/// This data structure contains SAGV config values that are considered output by the MRC.
typedef struct {
  UINT32           NumSaGvPointsEnabled; ///< Count of the total number of SAGV Points enabled.
  UINT32           SaGvPointMask;        ///< Bit mask where each bit indicates an enabled SAGV point.
  MrcSaGvTimingOut SaGvTiming[MAX_SAGV_POINTS];
} MrcSaGvOutput;

typedef struct {
  MRC_REF_PI_FREQ_SWITCH_DATA RefPiCalHighFreq[MAX_SAGV_POINTS];    ///< Per sagv point Refpi cal training results for high frequency
  MRC_REF_PI_FREQ_SWITCH_DATA RefPiCalLowFreq[MAX_SAGV_POINTS];     ///< Per sagv point Refpi cal training results for low frequency
  MrcLvrSaveRestore LvrSaveRestore[MAX_SAGV_POINTS];        ///< Per sagv point Lvr Autotrim saved values
  UINT32          PmaMcTechEnable;                          ///< The enabled memory technologies and their max frequency
  UINT32          PmaMcCap;                                 ///< The memory controller's capabilities.UINT32
  UINT32          MeStolenSize;                             ///< The manageability engine memory size, in Mbyte units.
  UINT32          ImrAlignment;                             ///< The Largest CSE IMR Memory Size, in Mbytes units
  MrcMcIpVersion  McIpVersion;                              ///< The last cold boot happened with this MC IP Version
  MrcDdrIoIpVersion DdrIoIpVersion;                         ///< The last cold boot happened with this DDRIO IP Version
  MrcVersion      Version;                                  ///< The last cold boot happened with this MRC version.
  UINT32          SaMemCfgCrc;                              ///< The CRC32 of the system agent memory configuration structure.
  UINT8           FirstPopController;                       ///< First Populated Controller.
  MrcFrequency    Frequency[MAX_SAGV_POINTS];               ///< The system's  memory controller frequency.
  BOOLEAN         GearMode[MAX_SAGV_POINTS];                ///< Current memory frequency gear: 0:Gear2, 1: Gear4.
  MrcFrequency    FreqMax;                                  ///< The system's requested maximum frequency.
  UINT32          MemoryClock;                              ///< The system's common memory controller clock, in femtoseconds.
  MrcClockRatio   Ratio;                                    ///< Request for this memory controller to use this clock ratio.
  MrcVddSelect    VddVoltage[MAX_PROFILE];                  ///< The voltage (VDD) setting for all DIMMs in the system, per profile.
  UINT32          Vdd2Mv;                                   ///< The VR Rail tied to the DRAM.
  BOOLEAN         EccSupport;                               ///< TRUE if ECC is enabled and supported on this controller.
  MrcDdrType      DdrType;                                  ///< DDR type: DDR5, LPDDR5.
  BOOLEAN         IsLpddr;                                  ///< TRUE if current memory type is LPDDR-class
  BOOLEAN         IsLpddr5;                                 ///< TRUE if current memory type is LPDDR5
  BOOLEAN         IsDdr5;                                   ///< TRUE if current memory type is DDR5
  UINT8           MaxChannels;                              ///< Maximum number of channels supported by the controller varies per technology.
  UINT8           BurstLength;                              ///< Burst length in DCLKs
  BOOLEAN         LpByteMode;                               ///< DRAM die width is x8 (Byte mode).
  UINT8           XmpProfileEnable;                         ///< XMP capable DIMMs detected in system. Bit 0 is XMP1, ...
  BOOLEAN           Is2RankDdr5;                   ///< TRUE if any DDR5 2R Dimm is populated.
  BOOLEAN           XmpConfigWarning;              ///< XMP Dimm Config Warning - 1DPC capable but installed 2DPC
  BOOLEAN           IsXmpSagvEnabled;              ///< TRUE if Dynamic Memory Boost feature or Realtime Memory Frequency feature has been enabled and trained
  McRegOffsets      OffsetKnobs;                              ///< Options for MC Register Offset settings
  BOOLEAN           DynamicMemoryBoostTrainingFailed;         ///< TRUE if Dynamic Memory Boost failed to train
  BOOLEAN           IsLP5Camm2;                                ///< TRUE if current memory is JEDEC spec LP5 CAMM
  UINT32            SaMemCfgCrcNoOffsetKnobs;                 ///< The CRC32 of the SA memory configuration without OffsetKnobs.
  UINT8             ValidRankMask;                 ///< Rank bit map.  Includes both channels across memory controllers.
  UINT8             ValidChBitMask;                ///< Channel bit map of the populated channels
  UINT8             ValidMcBitMask;                ///< Memory Controller population bit mask.
  BOOLEAN           CpgcGlobalStart;               ///< TRUE if using CPGC Global Start feature
  UINT8             MaxDqBits;                     ///< Maximum number of Dq bits for the memory type.
  BOOLEAN           IsMr10PdaEnabled;              ///< TRUE if PDA programming of MR10 is used in DDR5
  BOOLEAN           IsMr11PdaEnabled;              ///< TRUE if PDA programming of MR11 is used in DDR5
  BOOLEAN           IsMr48PdaEnabled;              ///< Defines if MR48 (Write Pattern) is required as PDA for this channel.
  BOOLEAN           MrXPdaDfeTap1Enabled;          ///< Defines if MRs of DFE TAP1 is required as PDA for this channel.
  BOOLEAN           MrXPdaDfeTap2Enabled;          ///< Defines if MRs of DFE TAP2 is required as PDA for this channel.
  BOOLEAN           MrXPdaDfeTap3Enabled;          ///< Defines if MRs of DFE TAP3 is required as PDA for this channel.
  BOOLEAN           MrXPdaDfeTap4Enabled;          ///< Defines if MRs of DFE TAP4 is required as PDA for this channel.
  BOOLEAN           IsDdr5Hynix[MAX_CONTROLLER][MAX_DDR5_CHANNEL][MAX_DIMMS_IN_CHANNEL];       ///< If a DDR5 Hynix DIMM is detected set it to TRUE per MC/Ch/DIMM being populated otherwise FALSE
  BOOLEAN           IsOneDpcSplitBgEnabled;        ///< If One DPC 1R split bg is enabled.
  BOOLEAN           IsRowHammerConfigured[MAX_CONTROLLER][MAX_CHANNEL][MAX_DIMMS_IN_CHANNEL]; ///< TRUE if ARFM or DRFM is configured for a DIMM
  /// Cmd and Ctl Pi Code for Low frequency. This is required to track for LP5 Frequency switching.
  UINT32            LowFCtlPiCode[MAX_SAGV_POINTS][MAX_CONTROLLER][MAX_CHANNEL][MAX_RANK_IN_CHANNEL]; ///< Cmd and Ctl Pi Code for Low frequency. It will be used for Jedec Reset for Lp5
  UINT32            LowFCmdPiCode[MAX_SAGV_POINTS][MAX_CONTROLLER][MAX_CHANNEL]; ///< Cmd and Ctl Pi Code for Low frequency. It will be used for Jedec Reset for Lp5
  BOOLEAN           RefPiLowSaved[MAX_SAGV_POINTS]; ///< TRUE if Low Frequency RefPi has been calculated
  BOOLEAN           QClkCalOffLowSaved[MAX_SAGV_POINTS];
  UINT32            QClkCalOffLowFreq[MAX_SAGV_POINTS];
  BOOLEAN           QClkCalOffHighSaved[MAX_SAGV_POINTS];
  UINT32            QClkCalOffHighFreq[MAX_SAGV_POINTS];
  UINT8             CkdShift[MAX_CONTROLLER][MAX_DDR5_CHANNEL][MAX_RANK_IN_CHANNEL]; ///< CKD QCK Output Delay obtained from Early Command Training
  BOOLEAN           LpX;                          ///< Low power die of the LPDDR part is detected.
  BOOLEAN           IsCkdSupported;               ///< TRUE if SPD byte 242 CKD Buffer is supported
  BOOLEAN           IsCs2NEver;                   ///< If any SAGV point has CS 2N Mode
  BOOLEAN           IsMixedEccDimms;              ///< TRUE if at least one ECC DIMM and at least one nonECC DIMM are present
  MrcVddSelect      VddqVoltage[MAX_PROFILE];     ///< The voltage (VDDQ) setting for all DIMMs in the system, per profile.
  MrcVddSelect      VppVoltage[MAX_PROFILE];      ///< The voltage (VPP) setting for all DIMMs in the system, per profile.
  MrcFrequency      MemFrequency[MAX_PROFILE];    ///< Every Profile's Frequency
  UINT8             Ibecc;
  UINT8             TmeEnable;
  UINT8             PmaCceConfig;
  UINT8             MaxRanks;                     ///< Maximum number of ranks detected in any channel on the Phy. Per Channel MaxRanks may be different.
  UINT8             ReservedBytesAlign[4];        ///< Align to 4 bytes for MrcSavedata
  //
  // IMPORTANT: data items below are not produced / consumed by Green MRC and hence are not copied from Blue to Green and back
  //
  UINT32          RegSaveCommon[MRC_REGISTER_COUNT_COMMON]; ///< The MC registers that are common to all SA GV points.
  UINT32          SaGvRegSave[MAX_SAGV_POINTS][MRC_REGISTER_COUNT_SAGV]; ///< The MC registers for each SA GV point.
  MrcContSave     Controller[MAX_CONTROLLER];               ///< The following are controller level definitions.
  MrcSaGvOutput   SaGvOutputs;                    ///< Per-SaGv Point output data
  MrcTiming       Timing[MAX_PROFILE];            ///< The Memory Timing Values
  UINT16          VCO[NUM_LCPLL][MAX_SAGV_POINTS];          ///< Tracks which VCO frequencies have been allocated in each LCPLL
  INT8            LCPLLWPSel[NUM_LCPLL][MAX_SAGV_POINTS];   ///< Tracks which workpoints have been allocated in each LCPLL
  UINT8           WP2LCPLL[MAX_SAGV_POINTS];                ///< Tracks which LCPLL (0/1) has been selected for the current workpoint
  INT8            LCPLLWPSelLowFreq;                        ///< It will be used for LP5 low frequency initialization during JEDEC Reset
  MrcBwSel        BwSelCalHighFreq[MAX_SAGV_POINTS];        ///< Per sagv point BwSel cal training results for high frequency
  MrcBwSel        BwSelCalLowFreq[MAX_SAGV_POINTS];         ///< Per sagv point BwSel cal training results for low frequency
  BOOLEAN         BwSelLowSaved[MAX_SAGV_POINTS];           ///< TRUE if Low Frequency BwSel cal training results have been calculated
} MrcSaveData;

// MrcOutputs needs to be DWORD aligned
typedef struct {
  UINT32              Size;                        ///< The size of this structure, in bytes. Must be the first entry in this structure.
  MrcDebug            Debug;                       ///< MRC debug related variables used for serial output and debugging purposes.
  MrcVersion          Version;                     ///< The memory reference code version.
  UINT64              MchBarWriteCount;            ///< The number of MMIO writes performed during MRC execution.
  UINT64              MchBarReadCount;             ///< The number of MMIO reads performed during MRC execution.
  MrcUPRLimitPtr      UpmPwrRetrainLimits;         ///< Pointer to Global UPM/PWR/RETRAIN Limits on the stack the size of MRC_NUMBER_UPM_PWR_RETRAIN_MARGINS
  MrcFrequency        FreqMax;                     ///< The requested maximum valid frequency.
  MrcFrequency        Frequency;                   ///< The system's common memory controller frequency based on inputs, SPD, and capabilities.
  MrcFrequency        HighFrequency;               ///< High operating frequency used for LPDDR5 JEDEC Init functions as we are at low frequency during init.
  MrcFrequency        LowFrequency;                ///< Low operating frequency used in MrcReset() for LPDDR5.
  UINT32              MemoryClockMax;              ///< The system's common memory controller maximum clock, in femtoseconds.
  UINT32              MemoryClock;                 ///< The system's common memory controller clock, in femtoseconds.
  MrcMemoryMap        MemoryMapData;               ///< The system's memory map data.
  MrcVddSelect        VddVoltage[MAX_PROFILE];     ///< The currently running voltage (VDD) setting for all DIMMs in the system, per profile.
  MrcVddSelect        VddqVoltage[MAX_PROFILE];    ///< The voltage (VDDQ) setting for all DIMMs in the system, per profile.
  MrcVddSelect        VppVoltage[MAX_PROFILE];     ///< The voltage (VPP) setting for all DIMMs in the system, per profile.
  MrcFrequency        MemFrequency[MAX_PROFILE];   ///< Every Profile's Frequency
  MrcTiming           Timing[MAX_PROFILE];         ///< The memory timing values.
  UINT32              VccddqVoltage;               ///< DDR transmitter FIVR voltage rail per technology and per datarate. @todo : Does it need
  UINT32              VccIogVoltage;               ///< Data Tx PreDriver, Data receive, and analog FIVR voltage rail
  UINT32              VccClkVoltage;               ///< IO Clock and clock distrubition FIVR voltage rail
  UINT32              VccDist;                     ///< Vdd2G LVR that is fixed output
  UINT32              Vdd2Mv;                      ///< The VR Rail tied to the DRAM.
  UINT32              InWorkPointLock;             ///< Set to TRUE when in WorkPointLock
  UINT32              MrcTotalChannelLimit;        ///< The maximum allowed memory size per channel, in MBytes.
  UINT32              SystemDimmCount;             ///<
  UINT32              NumPopChannel;               ///< Total number of MC Channels Populated
  UINT32              GreenCallTableIndex;        ///< Hold the current Green Call Table index for interpreter calls to continue off
  UINT32              GreenCallTablePostCode;     ///< Hold the current Green Call Table post code for interpreter calls to continue off
  UINT32              InterpreterData;            ///< Generic variable to hold data that needs to be transferred from Blue to Green during Interpreter
  UINT32              InterpreterOrigDbgLvl;      ///< Store Original DebugLvl set in the interpreter
  UINT32              FailMax[MAX_CONTROLLER][MAX_CHANNEL];  // This is initialized to 0 prior to any memtest, indicating no failures
  INT32               FailIndex[MAX_CONTROLLER][MAX_CHANNEL];
  UINT32              RefreshPanicWm;              ///< Refresh Panic Watermark, Range 1-9
  UINT32              RefreshHpWm;                 ///< Refresh High Priority Watermark, Range 1-8
  MrcControllerOut    Controller[MAX_CONTROLLER];  ///< The following are controller level definitions.
  MrcFrequency        MaxDimmFreq;                 ///< The maximum frequency supported by the current system DIMM population.
  ROW_FAIL_RANGE      FailRange[MAX_CONTROLLER][MAX_CHANNEL][MAX_FAIL_RANGE]; // Advanced Memtest Failure Range tracking
  BOOLEAN             LowFreqCsCmd2DSweepDone[MAX_SAGV_POINTS];     ///< Used to run CsCmd2D sweep in ECT at low frequency of Lpddr. FALSE - Low freq sweep will be done TRUE - Low freq sweep has been done
  UINT16              MarginResult[MAX_RESULT_TYPE][MAX_RANK_IN_CHANNEL][MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_EDGES]; ///< Stores last margin measurement.
  UINT16              ExtWLTxDqs[MAX_CONTROLLER][MAX_CHANNEL][MAX_RANK_IN_CHANNEL][MAX_SDRAM_IN_DIMM]; ///< Holds DDR5 External Write Leveling results
  UINT8               Margin2DResult[MAX_2D_EYE_TYPE][MAX_RANK_IN_CHANNEL][MAX_CHANNEL][MAX_2D_EYE_OFFSETS][MAX_EDGES]; ///< Stores the 2D Eye Margin
  INT8                ClkWckDccCode[MAX_CONTROLLER][MAX_CHANNEL][MAX_BYTE_IN_LP_CHANNEL][MAX_RANK_IN_CHANNEL]; // CLK uses Byte 0 only, WCK is per byte
  UINT8               CkdShift[MAX_CONTROLLER][MAX_DDR5_CHANNEL][MAX_RANK_IN_CHANNEL]; ///< CKD QCK Output Delay obtained from Early Command Training
  MRC_CKD_BUFFER      CkdBuffer[MAX_DIMMS_IN_SYSTEM];
  UINT8               PprAvailableResources[MAX_CONTROLLER][MAX_CHANNEL][MAX_RANK_IN_CHANNEL][MAX_SDRAM_IN_DIMM]; ///< PPR available resources per device
#if MRC_ENABLE_STATS
  MrcStatsTracker     StatsTracker;                ///< Used to record the data for the stats and telemetry framework
#endif // MRC_ENABLE_STATS && MRC_DEBUG_PRINT
  UINT16              Qclkps;                      ///< QCLK period in pS, this is internal MC/DDRIO clock which is impacted by Gear
  UINT16              Dclkps;                      ///< DCLK period in pS, this is internal MC/DDRIO clock which is impacted by Gear
  UINT16              tCKps;                       ///< Memory clock period in pS (external bus clock)
  UINT16              Wckps;                       ///< Write clock period in pS
  UINT16              UIps;                        ///< Data Unit Interval period in pS (half of external bus clock)
  UINT16              NumCL;                       ///< Global Variables storing the current number of Cachelines
  UINT16              PprNumDetectedErrors;        ///< PPR Detect how many times of bad rows or devices
  UINT16              PprRepairFails;              ///< PPR repair fails
  UINT16              PprForceRepairStatus;        ///< PPR Force Repair Status
  UINT16              EWRTC2DSmallWidth;           ///< Record the small width of EarlyWrite Timing 2D training
  UINT16              MaxRdDataValid;              ///< Used to track the maximum RdDataValid delay across all Frequency Points, (Sub)Channels, and Ranks.
  UINT16              PostCodeStart;               ///< Green Start POST code for the interpreter
  UINT16              PostCodeStop;                ///< Green Stop  POST code for the interpreter
  UINT16              PprNumSuccessfulRepairs;     ///< PPR number of successful repairs
  UINT16              RcompTarget[MAX_RCOMP_TARGETS]; ///< RCOMP target values for DqOdt, DqDrv, CmdDrv, CtlDrv, ClkDrv
  MrcClockRatio       Ratio;                       ///< Request for this memory controller to use this clock ratio.
  MrcDimmStatus       FailingChannelBitMask;       ///< BitMask to detect failing Channels and disable them
                                                   ///< -------------- Items below should be single-byte only --------------
  BOOLEAN             IsCaOdtStrapRead;            ///< Set to TRUE once CA ODT Strap has been read for DDR5
  BOOLEAN             BitByteSwizzleFound;         ///< Indicates if the Bit/Byte level swizzling from CPU to DRAM have been identified.
  BOOLEAN             GearMode;                    ///< Current memory frequency gear: 0=Gear2, 1=Gear4
  BOOLEAN             IsZqDisabled;                ///< TRUE if Zq calibration is disabled and DVFSQ is enabled
  BOOLEAN             EccSupport;                  ///< TRUE if ECC is enabled and supported on this controller.
  BOOLEAN             RestoreMRs;                  ///< Enable/Disable restoring
  BOOLEAN             IsR2RDone;
  BOOLEAN             EctDone;                        ///< Set to TRUE once Early Command Training on is done, and we can run JEDEC Init
  BOOLEAN             JedecInitDone;                  ///< Set to TRUE once JEDEC Init is done
  BOOLEAN             XmpProfileEnable;               ///< XMP capable DIMMs detected in system (0 = no, 1 = yes).
  BOOLEAN             IsLpddr;                        ///< TRUE if current memory type is LPDDR-class
  BOOLEAN             IsLpddr5;                       ///< TRUE if current memory type is LPDDR5
  BOOLEAN             IsDdr5;                         ///< TRUE if current memory type is DDR5
  BOOLEAN             IsDvfsqEnabled;                 ///< TRUE if DVFSQ is enabled for a given GV point, which is determined by Inputs.DvfsqEnabled && Frequency <= f3200.
  BOOLEAN             DramDqOdtEn;                    ///< Specifies if the DRAM DQ ODT is enabled.
  BOOLEAN             LpByteMode;                     ///< DRAM die width is x8 (Byte mode).
  BOOLEAN             Gear4Ever;                      ///< If any SAGV point was Gear4, then this is TRUE otherwise FALSE.
  BOOLEAN             Is2RankDdr5;                    ///< Indicates if any DDR5 2R Dimm is populated.
  BOOLEAN             IsAny2Dpc;                      ///< TRUE if any channel has 2 DIMMs populated
  BOOLEAN             CaParityMode;                   ///< TRUE indicates if the CA Parity Mode is configured for DDR5
  BOOLEAN             PowerTrainingCoarseGranularity; ///< Can be set to indicate to power training functions that should run in coarse-granularity sweep mode
  BOOLEAN             LpX;                            ///< Low power die of the LPDDR part is detected.
  BOOLEAN             IsSoDimm;                       ///< Used for Desktop TRUE if SODIMM, FALSE if UDIMM
  BOOLEAN             IsDbiReadEnabled;               ///< TRUE if Read DBI is enabled (LP5)
  BOOLEAN             IsDbiWriteEnabled;              ///< TRUE if Write DBI is enabled (LP5)
  BOOLEAN             IsDramDvfsqOn;                  ///< TRUE if DVFSQ is currently set on the DRAM. So for DVFSQ enabled flow, true when FSP-OP=1, false when FSP-OP=0.
  BOOLEAN             IsInsideJedecReset;             ///< TRUE if MRC is currently executing the JEDEC reset/init sequence
  BOOLEAN             IsWckSynced;                    ///< TRUE if WCK is currently synced and should not be interrupted, such as during Enhanced RDQS Training mode
  BOOLEAN             IsNtOdtEnabled;                 ///< TRUE if LP5 NT ODT is currently enabled
  BOOLEAN             IsCs2NEnabled;                  ///< If Current SAGV point is CS 2N Mode
  BOOLEAN             IsWckLevelingDone;              ///< TRUE if WCK2CK Leveling is done on LP5
  BOOLEAN             IsDvfscEnabled;                 ///< TRUE if DVFSC is enabled for a given GV point, which is determined by Inputs.DvfscEnabled && Frequency <= f3200.
  BOOLEAN             IsLaneMode;                     ///< TRUE if in Lane Sensing Mode (Bit Mask), only set for training steps that will get feedback from DRAM on xxDQ pins
  BOOLEAN             IsDimm1Enabled;                 ///< TRUE if Dimm 1 (Rank[3:2]) is populated
  BOOLEAN             IsOCProfile;                    ///< MemoryProfile is OC profile and system supports OC for DDR5
  BOOLEAN             XmpConfigWarning;               ///< XMP Dimm Config Warning - 1DPC capable but installed 2DPC
  BOOLEAN             Is1DPC1R;
  BOOLEAN             Is1DPC2R;
  BOOLEAN             Is2SPC0R1R;
  BOOLEAN             Is2SPC0R2R;
  BOOLEAN             Is2DPC1R1R;
  BOOLEAN             Is2DPC2R2R;
  BOOLEAN             SkipZq;                      ///< Skip ZQ Cal in SetupIoTest
  BOOLEAN             CpgcModeLocked;              ///< Used to lock down CPGC Mode until NormalMode is executed in MRC Start flow.
  BOOLEAN             IsGreenCodeLoaded;           ///< Used to check if the Green MRC code is loaded into the UC SRAM
  BOOLEAN             IsInterpreterCommand;        ///< Indicates that Interpreter wants to execute task(s) from Green MRC, using start/stop codes below
  BOOLEAN             VoltageDone;                 ///< To determine if VoltageDone update has been done already
  BOOLEAN             IsCmdNormalizationEnabled;   ///< TRUE: Enable CMD Normalization
  BOOLEAN             TxtScrubSuccess;             ///< status memory scrubbing due to TXT
  BOOLEAN             IsSkipIoReset;               ///< Skip IoReset in RunIoTest
  BOOLEAN             MrcBasicMemoryTestPass;      ///< MRC health data for telemetry. Unhealthy status is reported on bmcold boot after bmFast failure
  BOOLEAN             IsCkdSupported;              ///< TRUE if SPD byte 242 CKD Buffer is supported
  BOOLEAN             IsEnDqsNRcvEn;               ///< TRUE if LP5 single ended receiver is enabled
  BOOLEAN             NeedManualParamDivide;       ///< Need manually divide MCT timing parameters which are in tCK cycles
  BOOLEAN             IsResetSequenceNeeded;       ///< TRUE if ClkPiCode was shifted and Reset Sequence is required to recover DRAM to operational state
  BOOLEAN             UpmPwrRetrainFlag;           ///< A flag that indicates if training with higher UPM/PWR limits.
  BOOLEAN             PdaEnable;                   ///< Current status of PDA - if true all the Mr6 operations need to use PDA mode.
  BOOLEAN             IsLastGvPoint;               ///<  Indicate to Green CTE Env that Green excecutble and connection can be closed
  BOOLEAN             IsPhyDqDeswizzleNeeded;      ///< Used to deswizzle DataTrainFeedback value, whenever DTF used in Per-Lane mode
  BOOLEAN             IsLP5Camm2;                  ///< Detects if the current JEDEC 1.0 Spec LP5 module uses the CAMM form factor
  BOOLEAN             IsIbeccInitRangesRequired;   ///< Flag to indicate that the IBECC Init Ranges FSM must be run
  BOOLEAN             IsMptuFast;                  ///< Flag to indicate that the MPTU is required on the current fast boot
  BOOLEAN             IsVocSearch;                 ///< TRUE if currently running VOC part of SOT
  BOOLEAN             IsAny2Ranks;                 ///< TRUE if any DIMM has 2 Ranks populated
  UINT8               FinalIbeccOperationMode;     ///< Output to BIOS on the state of IBECC
  UINT8               tMAC;                        ///< Maximum Activate Count for pTRR.
  UINT8               MaxChannels;                 ///< Maximum number of channels supported by the controller.  Varies per technology.
  UINT8               SdramCount;                  ///< The number of SDRAM components on a DIMM.
  UINT8               DQPat;                       ///< Global Variable storing the current DQPat REUT Test.
  INT8                DQPatLC;                     ///< Global Variable storing the current DQPat Loopcount.
  UINT8               ReutStopType;                ///< Global Variable storing the current test stop type.
  UINT8               ValidRankMask;               ///< Rank bit map.  Includes both channels across memory controllers.
  UINT8               ValidChBitMask;              ///< Channel bit map of the populated channels
  UINT8               ValidMcBitMask;              ///< Memory Controller population bit mask.
  UINT8               MaxDimm;                     ///< Maximum number of dimms supported by the channel.  Varies per technology.
  UINT8               BurstLength;                 ///< Burst length in tCK.
  UINT8               McChBitMask;                 ///< Flat Bit Mask of which CPGC/CADB engines to setup based on MaxChannels per technology.
  UINT8               OdtMode;                     ///< Specifies the current ODT Mode the DDRIO is set to.
  UINT8               MaxDqBits;                   ///< Maximum number of Dq bits for the memory type.
  UINT8               FirstPopController;          ///< First Populated Controller.
  UINT8               MaxRanks;                    ///< Maximum number of ranks detected in any channel on the Phy. Per Channel MaxRanks may be different.
  UINT8               EarlyDqs;                    ///< Value for DDR5 MR40 Read DQS offset timing
  UINT8               PprRunningState;             ///< PPR Running State. 0: PPR didn't run, 1: Is running, 3: PPR training is done.
  UINT8               PprPatBufShift;              ///< PPR Pattern Buffer pointer shift value
  UINT8               LockUI;                      ///< LOCK UI
  UINT8               DrfmBrc;
  UINT8               MrcPprStatus;                ///< Mrc Ppr Status
  UINT8               PsmiHandlerSize;             ///< PSMI handler size for reservation
  UINT8               ReadPreamble;                ///< Total Read Preamble for DDR5, Read Preamble Toggle for LPDDR5
  UINT8               EvenOddUIErr;                ///< Select if RMT will be run for all UI errors : 0 - All, 1 : Odd UI Errs, 2: EvenUI Errs
  UINT8               CmosConfig0;                 ///< Data of MRC_DEBUG_CONFIG0_CMOS_ADDR
  UINT8               CmosConfig1;                 ///< Data of MRC_DEBUG_CONFIG1_CMOS_ADDR
  UINT8               CmosConfig2;                 ///< Data of MRC_DEBUG_CONFIG2_CMOS_ADDR
  UINT8               BankIncOrder[MAX_MPTU];      ///< Used to program BankIncOrder in Ddr5 for each Dunit
  UINT8               CaDeselectStress;
  BOOLEAN             IsLoopbackSetupDone;
  BOOLEAN             WeaklockEn;                  ///< Weaklock enable
  BOOLEAN             RxDqsDelayCompEn;            ///< Rx DQS Delay Comp enable
  UINT8               ReservedBytesAlign[3];       ///< Reserved Bytes to ensure MrcOutput size is a multiple of DWORDs
  MrcIpTestEnv        IpModel;
  MrcDdrType          DdrType;                     ///< Current memory type: DDR5, LPDDR5
  MrcSaGvPoint        SaGvFirst;                   ///< First SaGv Point to be trained
  MrcSaGvPoint        SaGvLast;                    ///< Last SaGv Point to be trained
  MrcSaGvPoint        SaGvPoint;                   ///< SA GV point - Low, Med or High
  UINT8               BibOnRmt;                    ///< TRUE if  Bib is enabled for RMT
  UINT8               BibEnable;
  UINT8               BibIdle;
  UINT8               BibIdleType;
  BOOLEAN             CaTristateForISenseRmt;      ///< Enable calling MrcTriStateCa in IoReset for Current Sensor RMT
  // Entries below this point are not copied from green back to blue
  MRC_REGISTER_CACHE  RegisterCache;
} MrcOutput;

///****************************************
/// Input related "global data" structures.
///****************************************

/// This data structure contains all the "global data" values that are considered input by the MRC.
/// The following are DIMM level definitions. All ranks on a DIMM are set to these values.
typedef struct {
  MrcDimmSts  Status;     ///< Indicates whether this DIMM should be used.
  MrcSpdData  Spd;        ///< The SPD data for each DIMM. SPDGeneral field = 0 when absent.
  MrcTiming   Timing;     ///< The DIMMs requested timing overrides.
  UINT8       SpdAddress; ///< The SMBus address for the DIMM's SPD data.
  UINT8       CkdAddress; ///< The SMBus address for the DIMM's CKD data.
  UINT8       ChannelToCkdQckMapping;
  UINT8       PhyClockToCkdDimm;
} MrcDimmIn;

/// This data structure contains all the "global data" values that are considered input by the MRC.
/// The following are channel level definitions. All DIMMs on a memory channel are set to these values.
typedef struct {
  MrcChannelSts Status;                                                         ///< Indicates whether this channel should be used.
  UINT32        DimmCount;                                                      ///< The maximum number of DIMMs on this channel.
  MrcDimmIn     Dimm[MAX_DIMMS_IN_CHANNEL];                                     ///< The following are DIMM level definitions.
  UINT8         DqsMapCpu2Dram[MAX_DIMMS_IN_CHANNEL][MAX_BYTE_IN_DDR5_CHANNEL]; ///< Mapping from CPU DQS pins to SDRAM DQS pins. LP5 use DIMM0 index only; DDR5 uses both DIMMs
  UINT8         DqMapCpu2Dram[MAX_RANK_IN_CHANNEL][MAX_BYTE_IN_DDR5_CHANNEL][MAX_BITS]; ///< Mapping from CPU DQ pins to SDRAM DQ pins
} MrcChannelIn;

/// This data structure contains all the "global data" values that are considered input by the MRC.
/// The following are memory controller level definitions. All channels on a controller are set to these values.
typedef struct {
  MrcControllerSts  Status;               ///< Indicates whether this controller should be used.
  UINT8             ChannelCount;         ///< Number of valid channels that are requested on the controller.
  MrcChannelIn      Channel[MAX_CHANNEL]; ///< The following are channel level definitions.
} MrcControllerIn;

/// This data structure contains all the "global data" values that are considered input by the MRC.
/// The following pointer is defined in the platform wrapper.
typedef union {
  struct PlatformData_ *Pointer; ///< Pointer to optional platform-specific data.
  UINT64 Data;                   ///< Required for alignment between green and blue
} PLATFORM_DATA_PTR;

/// This data structure contains all the "global data" values that are considered input by the MRC.
/// The following are system level definitions. All memory controllers in the system are set to these values.
typedef struct {
  union {
    MRC_EXT_INPUTS_TYPE *Ptr;                  ///< External Inputs Pointer
    UINT64 Data;
  } ExtInputs;

  UINT64          TscTimeBase;                 ///< CPU TSC timebase in [us] : (BCLK * Ratio / 1000 / 1000)
  MrcFrequency    FreqMax;                     ///< The requested maximum valid frequency.
  MrcSkuType      SkuType;                     ///< The CPU SKU Type: 0=MrcSkuTypeUnspecified
  MrcMcIpVersion  McIpVersion;                 ///< define the MC IP Version, Segment, Stepping
  MrcDdrIoIpVersion DdrIoIpVersion;            ///< define the DDRIO IP Version, Segment, Stepping
  MrcIteration    Iteration;                   ///< Number of iterations thru the MRC core call table.
  MRC_BOOT_MODE   BootMode;                    ///< The requested memory controller boot mode.
  UINT32          SaMemCfgCrc;                 ///< Save current MEMORY_CONFIGURATION crc before MemConfig is overwritten. Used for ColdBootRequired checking
  UINT64          PciEBaseAddress;             ///< define the PciE base address.
  UINT64          MchBarBaseAddress;           ///< define the MCH bar base address.
  UINT32          SmbusBaseAddress;            ///< This field defines the smbus base address.
  UINT32          MeStolenSize;                ///< Define the size that the ME need in MB.
  UINT32          ImrAlignment;                ///< Define the largest size of IMR that the ME need in MB.
  UINT32          MmioSize;                    ///< define the MMIO size in MB.
  UINT32          TsegSize;                    ///< TSEG size that require by the system in MB.
  UINT32          DprSize;                     ///< DPR size required by system in MB.
  UINT32          PrmrrSize;                   ///< Prmrr size required by the system in MB.
  UINT32          TmeEnable;                   ///< TME policy enable 0: Disable; <b>1: Enable</b>
  UINT64          TmeExcludeBase;              ///< Base physical address to be excluded for TME encryption.
  UINT64          TmeExcludeSize;              ///< Size of range to be excluded from TME encryption.
  UINT32          DebugStream;                 ///< The debug port pointer.
  MrcControllerIn Controller[MAX_CONTROLLER];  ///< The following are controller level definitions.
  POINTER_STRUCT    MrcStackTop;                  ///< Top of the stack at the beginning of MRC, for stack usage calculations.
  BOOLEAN           BdatEnable;                   ///< Option to enable output of training results into BDAT.
  UINT8             BdatTestType;                 ///< When BdatEnable is set to TRUE, this option selects the type of training results data which will be populated into BDAT: 0 - RMT, 1 - RMT Per Bit, 2 - Margin 2D.
  UINT8             DramDqOdt;                    ///< Controls the usage of the DRAM DQ ODT. 0 - Auto, 1 - Enable, 2 - Disable. See MRC_EN_DIS_AUTO_TYPE.
  UINT32            SiPreMemPolicyPpi;
  BOOLEAN           LpFreqSwitch;                       ///< Boolean variable to enable or disable frequency switching for supported LPDDR devices.
  UINT8             MinAllowedNModeOvrd;                ///< DDR5 CA Minimal Allowed NMode Override: Disabled, 1N or 2N: 0=DISABLED, 1=1N, 2=2N.
  BOOLEAN           MptuPropagationErrorFlow;           ///< Boolean variable to enable or disable special flow around CHAN_EN field switching to avoid X propagation in simulation
  BOOLEAN           Mars;                               ///< Controls MARS feature
  UINT32            CleanMemory:1;                  ///< TRUE to request a memory clean
  UINT32            BypassMemScrub:1;               ///< TRUE to skip memory scrub. Any memory scrub request (ECC, TxtClean, CleanMemory) will be ignored.
  UINT32            RsvdBits1:30;
  UINT32            ErrorCountForFail;              ///< Error count for last-pass margin failure
  UINT8             BER;                            ///< Use BER margining in MRC
  BOOLEAN           IsDdrphyx64;                    ///< Identifies that the current CPU SKU is x64 (PHY)
  BOOLEAN           IsMcDtHalo;                     ///< Identifies that the current CPU SKU is Desktop / Halo (MC)
  BOOLEAN           IsDdrIoDtHalo;                  ///< Identifies that the current CPU SKU is Desktop / Halo (PHY)
  BOOLEAN           IsMcUlxUlt;                     ///< Identifies that the current CPU SKU is ULX / ULT (MC)
  BOOLEAN           IsDdrIoUlxUlt;                  ///< Identifies that the current CPU SKU is ULX / ULT (PHY)
  BOOLEAN           IsMcMbA0;                       ///< Identifies that the current CPU stepping is Mobile A0 (MC)
  BOOLEAN           IsDdrIoMbA0;                    ///< Identifies that the current CPU stepping is Mobile A0 (PHY)
  BOOLEAN           IsMcMbB0;                       ///< Identifies that the current CPU stepping is Mobile B0 (MC)
  BOOLEAN           IsDdrIoMbB0;                    ///< Identifies that the current CPU stepping is Mobile B0 (PHY)
  BOOLEAN           IsMcDtA0;                       ///< Identified that the current CPU stepping is Desktop A0 (MC)
  BOOLEAN           IsDdrIoDtA0;                    ///< Identified that the current CPU stepping is Desktop A0 (PHY)
  BOOLEAN           IsDdrIoTc;                      ///< Identified that the current CPU is a test chip (PHY)
  BOOLEAN           NonTargetOdtEn;                 ///< Enables Non-Target ODT for LPDDR5
  BOOLEAN           TxtClean;                       ///< TRUE if we require to perform TxtClean when Trusted eXecution Technology flow enabled.
  UINT8             Reserved2;                      ///< Reserved to ensure config block size is a multiple of DWORDs
  BOOLEAN           Mbist;                          ///< TRUE if we require to perform Memory BIST
  BOOLEAN           EnablePda;                      ///< TRUE MRs will be configured per PDA.
  BOOLEAN           Lp58BankMode;                   ///< TRUE if running Lp5 8-Bank Mode
  BOOLEAN           EnableOdtMatrix;                ///< TRUE if enabling ODT Matrix
  BOOLEAN           ReadMprVA;                      ///< TRUE: use V/A in DDR5 ReadMpr, FALSE: no V/A, all bits use the same LFSR
  BOOLEAN           RHMASwizzlingOverride;          ///< TRUE: use RHMA1Swizzling and RHMA2Swizzling instead of checking DRAM ID
  BOOLEAN           RHMicronSwizzlingOverride;      ///< TRUE: use RHMicronMA1Swizzling and RHMicronMA2Swizzling instead of checking DRAM ID
  UINT8             RHMA1Swizzling;                 ///< Row Hammer MA1_Swizzling
  UINT8             RHMA2Swizzling;                 ///< Row Hammer MA2_Swizzling
  UINT8             RHMicronRSwizzling;             ///< Row Hammer Micron_R_Swizzling
  UINT8             RHMicronFSwizzling;             ///< Row Hammer Micron_F_Swizzling
  MRC_CCE_CONFIG    PmaCceConfig;                   ///< MEMSS PMA CCE Enable Configuration
  BOOLEAN           IsPmaMemConfigDoneSkip;
  UINT8             Lp5NZQCount;                    ///< Number of DRAM Die a ZQ resource may be shared among. Max value of 16 Die
  UINT8             SaGvBeforeReset;                ///< Self-Refresh Entry value from PMC register on Warm / S3 flow for SAGV
  UINT32            NumCL;                          ///< SetupIOTest: override value for number of cachelines
  UINT8             LoopCount;                      ///< SetupIOTest: overide for total test lengh
  BOOLEAN           IsCrBasedCommunication;         ///< TRUE: use CR-based CommBuffer instead of SRAM-based
  BOOLEAN           IsXtensaEccDisabled;            ///< TRUE: disable SRAM ECC on Xtensa and UCSS
  MrcPostCode       EnterInterpreterOnPostCode;     ///< Enter into interpreter mode upon completion of task denoted by its POST code
  UINT16            StopPostCode;                   ///< POST code to stop MRC at
  BOOLEAN           IgnoreCheckPoint;               ///< Ignore OEM checkpoint in MRC call table
  BOOLEAN           IsCs2NRequested;                ///< If Cs 2N Mode is requested
  BOOLEAN           Lpddr5Camm;                    ///< Indicates if the LPDDR5/x CAMM is there or not // RplSupportContent
  BOOLEAN           IsApplyMrCommandDelays;        ///< TRUE: MRC will explicitly wait Jedec defined MR Command delays FALSE: MRC assumes Jedec MR delays are satsisfied by hardware and no additional delay is needed
  BOOLEAN           IsCaDeselectStress;            ///< TRUE: Run LFSR stress on CA bus during deselect cycles
  BOOLEAN           IsIOTestAddressRandom;         ///< TRUE: randomize CA address during IO test
  BOOLEAN           IsDdr5Crc;                     ///< DFX feature
  UINT8             GenerateNewTmeKey;             ///< Indicates if a new TME key is to be generated
  UINT8             IsXMP3Revision12Supported;     ///< Indicates whether the SPD data supports XMP1.2 version
  UINT8             MemoryProfileSave;             ///< Save MemoryProfile. Used when DMB/RMF is enabled.
  BOOLEAN           IsIbeccEnabled;                ///< Overrides the ext inputs Ibecc field so that Ibecc inputs do not trigger a cold boot.
  BOOLEAN           IsIbeccPmaEnabled;             ///< Overrides the ext inputs Ibecc field in case IBECCDIS in MEMSS PMA.
  BOOLEAN           RxDqVrefPerBit;                ///< Enable or disable RX DQ VREF Per Bit
  BOOLEAN           FourToggleReadPreamble;        ///< Enable or disable Four Toggle Read Preamble
  BOOLEAN           PprEnable;                     ///< Effective PPR configuration for the current boot
  BOOLEAN           SenseAtRxDll;                  ///< Boolean variable to enable or disable RxDqsDcc SenseAtRxDll
  UINT8             LastIbeccOperationMode;        ///< Input from BIOS indicating the last IBECC operation mode. Valid only on warm boot.
  UINT8             Reserved[4];                   ///< Reserved to ensure config block size is a multiple of DWORDs
  UINT32            SaMemCfgCrcNoOffsetKnobs;      ///< The CRC32 of the SA memory configuration without OffsetKnobs.
  /**
   Sets the serial debug message level\n
     0x00 = Disabled\n
     0x01 = Errors only\n
     0x02 = Errors and Warnings\n
     <b>0x03 = Errors, Warnings, and Info</b>\n
     0x04 = Errors, Warnings, Info, and Events\n
     0x05 = Displays Memory Init Execution Time Summary only\n
  **/
  UINT32 SerialDebugLevel;
  UINT32 IsKeepUcssPostMrc; ///< keep Ucss Post MRC <b>0: Disable</b>; 1: Enable
  UINT64 UcPayloadAddress; ///< Location of Green MRC payload
#ifdef CTE_FLYBY_FLAG
  MrcDdrioFlyBy DdrioFlyBy;
#else
  UINT32 StructAlignBuf;
#endif
  union {
    MRC_FUNCTION    *Func;  ///< External to MRC function pointers
    UINT64          Data;
  } Call;
  UINT8   PsmiHandlerSize;    ///< PSMI handler size for reservation
  UINT8   MaxVrefSamplesOvrd; ///< Override maximum VREF Samples used for 2D sweep
  UINT32  SaMemCfgCrcForSave; ///< Save current MEMORY_CONFIGURATION crc after a few fields of MemConfig are overwritten. Used for saving CRC during saving training values.
  PLATFORM_DATA_PTR PlatformDataPtr; ///< Aligned pointer to optional platform-specific data.
  BOOLEAN LockUiDiv6Flow;     ///< LockUI Calibration flow uses Div6 mode
  UINT16  RloadTarget;        ///< Rload target value for Rload compensation
  BOOLEAN DiscardLvrAutoTrimResults; ///< Discard LVR Auto Trim Results and use PHY init values
  BOOLEAN PhClkSkipPhCorrection;  ///< Skip PhClk correction in PhClk calibration
  UINT8   PhClkCheckPhError;      ///< Defines min to max tolerance for phase spacing check, specified max - min of all 8 phases in 1/512 * phclk increments
  UINT8   PhClkCheckDcError;      ///< Defines duty cycle tolerance in 1/512 * phclk increments
  BOOLEAN IsOneDpcSplitBgEnabled; ///< TRUE: 1Rank Split Bg On SubChannel Enabled.
  UINT32  DebugValue;             ///< Used for general debug
  UINT8   ReservedBytesAlign[2];  ///< Reserved Bytes to ensure MrcInput size is a multiple of DWORDs
} MrcInput;

typedef struct {
  UINT32        Size;   ///< The size of this structure, in bytes. Must be the first entry in this structure.
  MrcSaveHeader Header; ///< The header portion of the MRC saved data.
  MrcSaveData   Data;   ///< The data portion of the MRC saved data.
} MrcSave;

typedef struct {
  UINT8        MrcDataString[4]; ///< Beginning of global data marker, starts with "MRC". Must be the first entry in this structure.
  UINT32       MrcDataSize;      ///< The size of the MRC global data area, in bytes. Must be the second entry in this structure.
  MrcInput     Inputs;           ///< System specific input variables.
  MrcOutput    Outputs;          ///< System specific output variables.
  MrcSave      Save;             ///< System specific save variables.
} MrcParameters;

/**
  This function returns the recommended MRC boot mode.

  @param[in]  MrcData - include all the MRC general data.

  @retval bmWarm if we are in self refresh and the DISB bit is set, otherwise returns bmCold.
**/
extern
MRC_BOOT_MODE
MrcGetBootMode (
  IN MrcParameters * const MrcData
  );

/**
  This function reads out DDRPHY_MISC_SAUG_CR_RTL_VERSION_REG.IP_Model and returns the IP Model written by TE.

  @param[in]  MrcData - Include all MRC global data.

  @retval UINT32 IpModel - IP Model Version
**/
UINT32
MrcIpModelGet (
  IN MrcParameters *const MrcData
  );

/**
  Calculates a CRC-32 of the specified data buffer.

  @param[in] Data     - Pointer to the data buffer.
  @param[in] DataSize - Size of the data buffer, in bytes.

  @retval The CRC-32 value.
**/
extern
UINT32
MrcCalculateCrc32 (
  IN     const UINT8  *const Data,
  IN     const UINT32 DataSize
  );

/**
  This function resets the DISB bit in General PM Configuration 2 B:D:F 0,31,0 offset 0xA2.

  @param[in]  MrcData - include all the MRC general data.
**/
extern
void
MrcResetDISB (
  IN MrcParameters * const MrcData
  );

/**
  Prepare MRC structures for next MRC Iteraion (SAGV or margin retrain).

  @param[in, out] MrcData - Include all MRC global data.
  @param[in]      Retrain - Turn on/off UpmPwrRetrainFlag

**/
extern
void
MrcPrepareNextMrcIteration (
  IN OUT MrcParameters *const MrcData,
  IN BOOLEAN                  Retrain
  );

/**
  Initializes the memory controller and DIMMs.

  @param[in, out] MrcData - Include all MRC global data.
  @param[in]      Select  - The post code of the call table entry to execute.
  @param[in, out] CurrentSaGvPoint  - The SaGV point that we want to execute the Select post code  entry or to continue from (relevan if SaGV enabled).
  @param[in]      ExecuteFromSelectPoint  - Execute Memory configuration from (Select,CurrentSaGvPoint) to the end

  @retval mrcSuccess if the initialization succeeded, otherwise an error status indicating the failure.
**/
extern
MrcStatus
MrcStartMemoryConfiguration (
  IN OUT MrcParameters *const MrcData,
  IN     MrcPostCode          Select,
  IN OUT MrcSaGvPoint  *const CurrentSaGvPoint,
  IN BOOLEAN                  ExecuteFromSelectPoint
  );
/**
  Retrieve the current memory frequency and clock from the memory controller.

  @param[in]      MrcData      - Include all MRC global data.
  @param[in, out] MemoryClock  - The current memory clock.
  @param[in, out] Ratio        - The current memory ratio setting.
  @param[in, out] RefClk       - The current memory reference clock.

  @retval: The current memory frequency.
**/
MrcFrequency
MrcGetCurrentMemoryFrequency (
  MrcParameters * const   MrcData,
  UINT32 * const          MemoryClock,
  MrcClockRatio * const   Ratio
  );

/**
  This function get the current value of the sticky scratchpad register.

  @param[in] MrcData - include all the MRC data.

  @retval The current value of the sticky scratchpad register
**/
extern
UINT64
MrcWmRegGet (
  IN     MrcParameters *const MrcData
  );

/**
  This function Set a newvalue of the sticky scratchpad register by set new givin Bit(s)

  @param[in] MrcData   - include all the MRC data.
  @param[in] SskpdBits - Bit(s) Need to Set

**/
void
MrcWmRegSetBits (
  IN    MrcParameters *const MrcData,
  IN    UINT64        SskpdBits
);

/**
This function Set a newvalue of the sticky scratchpad register by clear givin Bit(s)

@param[in] MrcData - include all the MRC data.
@param[in] SskpdBit - Bit(s) Need to Clear

@retval The current value of the sticky scratchpad register.

**/
void
MrcWmRegClrBits (
  IN     MrcParameters *const MrcData,
  IN     UINT64        SskpdBits
);

/**
  Gets pointers to functions inside of core.

  @param[out] CallChannelExist            - Pointer to the function MrcChannelExist
  @param[out] CallPrintf                  - Pointer to the function MrcPrintf
  @param[out] CallSignExtend              - Pointer to the function MrcSignExtend

  @retval Returns mrcSuccess if the function succeeds.
**/
extern
MrcStatus
MrcGetCoreFunction (
  OUT UINTN              *CallChannelExist,
  OUT UINTN              *CallPrintf,
  OUT UINTN              *CallSignExtend
  );


#pragma pack (pop)
#endif  // MRC_INTERFACE_H

