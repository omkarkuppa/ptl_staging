/** @file
  Definition of DDR5 Specific functions, constants, defines,
  and data structures.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
  DDR5 JEDEC Spec
**/
#ifndef MRC_DDR_5_H_
#define MRC_DDR_5_H_

#include "MrcChipApi.h"
#include "MrcCommon.h"
#include "MrcDdrCommon.h"
#include "MrcDdr5Registers.h"
#include "MrcMemoryApi.h"

// DDR5 MPC Command definitions
#define DDR5_MPC_EXIT_CS_TRAINING_MODE    0x00
#define DDR5_MPC_ENTER_CS_TRAINING_MODE   0x01
#define DDR5_MPC_DLL_RESET                0x02
#define DDR5_MPC_ENTER_CA_TRAINING_MODE   0x03
#define DDR5_MPC_ZQCAL_LATCH              0x04
#define DDR5_MPC_ZQCAL_START              0x05
#define DDR5_MPC_STOP_DQS_INTERVAL_OSC    0x06
#define DDR5_MPC_START_DQS_INTERVAL_OSC   0x07
#define DDR5_MPC_SET_2N_COMMAND_TIMING    0x08
#define DDR5_MPC_SET_1N_COMMAND_TIMING    0x09
#define DDR5_MPC_EXIT_PDA_ENUM_PROG_MODE  0x0A
#define DDR5_MPC_ENTER_PDA_ENUM_PROG_MODE 0x0B
#define DDR5_MPC_MANUAL_ECS_OPERATION     0x0C
// 0x0D - 0x1E = Reserved for Future Use
#define DDR5_MPC_APPLY_VREF_RTT           0x1F
#define DDR5_MPC_CS_GEARDOWN_ENABLE       0x91
// MPC Masks
// These MPC commands must be masked with the data
// or ID codes to be used. Refer to the DDR5 spec
// MPC Opcodes section.

// MPC commands using OP[2:0] to specify MR value
#define DDR5_MPC_GROUP_A_RTT_CK(val)      (0x20 | (0x7 & (val)))
#define DDR5_MPC_GROUP_B_RTT_CK(val)      (0x28 | (0x7 & (val)))
#define DDR5_MPC_GROUP_A_RTT_CS(val)      (0x30 | (0x7 & (val)))
#define DDR5_MPC_GROUP_B_RTT_CS(val)      (0x38 | (0x7 & (val)))
#define DDR5_MPC_GROUP_A_RTT_CA(val)      (0x40 | (0x7 & (val)))
#define DDR5_MPC_GROUP_B_RTT_CA(val)      (0x48 | (0x7 & (val)))
#define DDR5_MPC_SET_DQS_RTT_PARK(val)    (0x50 | (0x7 & (val)))
#define DDR5_MPC_SET_RTT_PARK(val)        (0x58 | (0x7 & (val)))

#define DDR5_MPC_GET_RTT_VALUE(val)       (0x7 & (val))

// MPC commands using OP[3:0] to specify ID value
#define DDR5_MPC_PDA_ENUMERATE_ID(val)    (0x60 | (0xF & (val)))
#define DDR5_MPC_PDA_SELECT_ID(val)       (0x70 | (0xF & (val)))
#define DDR5_MPC_CFG_TDLLK_TCCD_L(val)    (0x80 | (0xF & (val)))

// VREF commands using OP[6:0] to specify VREF calibration setting
#define DDR5_VREFCA(val)                  (0x7F & (val))
#define DDR5_VREFCS(val)                  ((0x7F & (val)) | MRC_BIT7)
#define DDR5_VREFCS_RAW(val)              (0x7F & (val))

#define LSB_DQS_OSQ_MR (46)
#define MSB_DQS_OSQ_MR (47)

///
/// Timings
///

/// Minimum CS High Pulse width at PDX (tCSH_SRexit) fS
#define MRC_DDR5_tCSH_MIN_FS            (13 * 1000 * 1000)
#define MRC_DDR5_tCSH_SREXIT_PS      MRC_DDR5_tCSH_MIN_FS

/// Minimum CS Low Duration time at PDX (tCSL_SRexit) pS
#define MRC_DDR5_tCSL_MIN_NCK        (3)
#define MRC_DDR5_tCSL_SREXIT_1N      MRC_DDR5_tCSL_MIN_NCK
#define MRC_DDR5_tCSL_SREXIT_2N      (6)

// Minimum Self Resfresh (tCSL) fS
#define MRC_DDR5_tSR_MIN             (10 * 1000 * 1000)

/// DDR5 ZQ Timings
// tZQCAL is 1us for all frequencies.
#define tZQCAL_FS_DDR5        (1 * 1000 * 1000 * 1000)
// tZQLAT timing is max(30ns,8nCK)
#define tZQLAT_DDR5_CK_MIN    8
#define tZQLAT_FS_DDR5        (30 * 1000 * 1000)

///
/// tDLLK values in units of tCK
///
#define tDLLK_DDR5_2000_3600 1024
#define tDLLK_DDR5_3600_4400 1280
#define tDLLK_DDR5_4400_5200 1536
#define tDLLK_DDR5_5200_6000 1792
#define tDLLK_DDR5_6000_6800 2048
#define tDLLK_DDR5_6800_7600 2304
#define tDLLK_DDR5_7600_8400 2560
#define tDLLK_DDR5_8400_8800 2816
#define tDLLK_DDR5_MAX       2816

/// tMOD values for DDR5 (AKA tMRD). max(14ns, 16nCK)
#define tMOD_DDR5_FS          (14 * 1000 * 1000)

/// tMRW max(5ns, 8nCK)
#define tMRW_DDR5_FS          (5 * 1000 * 1000)

///  Precharge to Precharge Delay for all Frequencies in tCK
#define MRC_DDR5_tPPD_LOW_FREQ  (2)
#define MRC_DDR5_tPPD_HIGH_FREQ (4)

// DDR5 Muti-cycle CS timings (nCK)
// Use CA/CS 22N/6N timing
#define DDR5_tMC_MPC_SETUP_MIN             8 // tMC_MPC_Setup
#define DDR5_tMC_MPC_HOLD_MIN              8 // tMC_MPC_Hold
#define DDR5_tMPC_CS                       6 // tMPC_CS

// DDR5 tCCDL_L_WR/tCCDL_L_WR2
#define DDR5_tCCDL_L_WR_nCK      (32)
#define DDR5_tCCDL_L_WR_FS       (20 * 1000 * 1000)

/// CAS-to-CAS delay for all frequencies in tCK
#define MRC_DDR5_tCCD_ALL_FREQ (8)

/// tRX_DQS2DQ (Units of 100UI for precision)
#define MRC_DDR5_tRX_DQS2DQ_MIN  (114)
#define MRC_DDR5_tRX_DQS2DQ_3200 (937)
#define MRC_DDR5_tRX_DQS2DQ_3600 (833)
#define MRC_DDR5_tRX_DQS2DQ_4000 (750)
#define MRC_DDR5_tRX_DQS2DQ_4400 (738)
#define MRC_DDR5_tRX_DQS2DQ_4800 (729)
#define MRC_DDR5_tRX_DQS2DQ_5200 (721)
#define MRC_DDR5_tRX_DQS2DQ_5600 (714)
#define MRC_DDR5_tRX_DQS2DQ_6000 (708)
#define MRC_DDR5_tRX_DQS2DQ_6400_8800 (703)

#define MRC_DDR5_tDQSCK_MIN (135)
#define MRC_DDR5_tDQSCK_MAX (135)
#define MRC_DDR5_tDQSS_MIN (375)
#define MRC_DDR5_tDQSS_MAX (375)
/// tXP value for DDR5: max(7.5ns, 8nCK)
#define tXP_DDR5_FS  7500000

/// tRPRE value (nCK)
#define tRPRE_ALL_FREQ_DDR5_2tCK  (2)
#define tRPRE_ALL_FREQ_DDR5_3tCK  (3)
#define tRPRE_ALL_FREQ_DDR5_4tCK  (4)

/// tWPRE value (nCK)
#define tWPRE_ALL_FREQ_DDR5_2tCK  (2)
#define tWPRE_ALL_FREQ_DDR5_3tCK  (3)
#define tWPRE_ALL_FREQ_DDR5_4tCK  (4)

/// tMRR values for DDR5. max(14ns, 16nCK)
#define tMRR_DDR5_FS          (14 * 1000 * 1000)
#define tMRR_DDR5_nCK         (16)
#define MRC_DDR5_tODT_ON_WR_OFFSET  (-3)
#define MRC_DDR5_tODT_OFF_WR_OFFSET (2)

// tMRW values for DDR5 - max(5ns, 8nck)
#define tMRW_DDR5_FS          (5 * 1000 * 1000)
#define TMRW_DDR5_nCK         (8)

/// CKE min pulse width/ Power down Entry to Exit (FS)
#define MRC_DDR5_tCKE_MIN           (7500000)
#define MRC_DDR5_tCKE_MIN_NCK       (8)

/// DDR5 CS Training Timings
// Registration of CSTM Entry command to start of training samples time
// Registration of CSTM exit to next valid command delay
#define MRC_DDR5_tCSTM_ENTRY_EXIT_DELAY_NS (20)

// Min time between last CS_N pluse and first pulse of MPC Command to exit CSTM
#define MRC_DDR5_tCSTM_MIN_TO_MPC_EXIT_NCK (4)

// Time from sample evaluation to output on DQ bus
#define MRC_DDR5_tCSTM_VALID_NS            (20)

#define MRC_DDR5_tCSTM_DQ_WINDOW_NCK       (2)
/// DDR5 CA Training Timings
// Registration of CATM Entry command to start of training samples time
// Registration of CATM exit to next valid command delay
#define MRC_DDR5_tCATM_ENTRY_EXIT_DELAY_NS (20)

// Time from sample evaluation to output on DQ bus
#define MRC_DDR5_tCATM_VALID_NS            (20)

#define MRC_DDR5_tCATM_DQ_WINDOW_NCK       (2)
#define DDR5_CL_30                         4

// tCPDED constant 5000 fs
#define MRC_DDR5_tCPDED_PS (5000000)

// tCKCKEH
#define MRC_DDR5_tCKCKEH_MIN (8)
#define MRC_DDR5_tCKCKEH_MAX_FS (3500000)

// Write Leveling Pulse Enable - Time from Write Leveling Training Enable MRW to
// when Internal Write Leveling Pulse logic level is valid (NS)
#define MRC_DDR5_tWLPEN_NS             (15)

// MAX tWLO acording to DDR5 spec is 9.5 ns
#define tWLO_MAX_FS     (95 * 100 * 1000)

#define MIN_DDR5_tRTP 12  // Minimum tRTP is 12 clocks

// tREFSBRD - Timing before issuing ACTIVATE command to a different non-refresh bank
#define MRC_DDR5_tREFSBRD_PS  30000

// tRFMsb - Same Bank Set Refresh Management Cycle time
#define MRC_DDR5_tRFMSB_8GB_PS   115000
#define MRC_DDR5_tRFMSB_16GB_PS  130000
#define MRC_DDR5_tRFMSB_24GB_PS  190000
#define MRC_DDR5_tRFMSB_32GB_PS  190000

#define MRC_DDR5_tRRFSB_PS       60000
#define BRC2                     2
#define BRC3                     3
#define BRC4                     4

#define ZQCS_PERIOD_DDR5  256

/// tADC(min) .2tck = ROUND (128 * .2)
/// tADC(max) .8tck = ROUND (128 * .8)
#define MRC_DDR5_tADC_5600_MIN (26)
#define MRC_DDR5_tADC_5600_MAX (102)
#define MRC_DDR5_tADC_6000_MIN (23)
#define MRC_DDR5_tADC_6000_MAX (105)
#define MRC_DDR5_tADC_6400_MIN (20)
#define MRC_DDR5_tADC_6400_MAX (108)
#define MRC_DDR5_tADC_7200_MIN (15)
#define MRC_DDR5_tADC_7200_MAX (113)
#define MRC_DDR5_tADC_8000_MIN (10)
#define MRC_DDR5_tADC_8000_MAX (118)
#define MRC_DDR5_tADC_8400_MIN (8)
#define MRC_DDR5_tADC_8400_MAX (120)

/// tDQSOffset(min) -.5tck = -128 * .5
/// tDQSOffset(max) .5tck = 128 * .5
#define MRC_DDR5_tDQSOFFSET_MIN (-64)
#define MRC_DDR5_tDQSOFFSET_MAX (64)

/// tDQSQ(min) -.08tck = -ROUND (128 * .08)
/// tDQSQ(max) .08tck = ROUND (128 * .08)
#define MRC_DDR5_tDQSQ_MIN (-10)
#define MRC_DDR5_tDQSQ_MAX (10)
// tPGMa (ms) time for DRAM to repair target row for x8 Device
// tPGMb (ms) time for DRAM to repair target row for x16 Device
#define MRC_DDR5_tPGMa  1000
#define MRC_DDR5_tPGMb  2000

// tPGMPST (us) hPPR Program Exit and New Address Setting time
#define MRC_DDR5_tPGMPST 50

///
/// DIMM DFE Tap defines
///
#define DDR5_DIMM_DFE_TAP1_RANGE  (40) ///< MR113
#define DDR5_DIMM_DFE_TAP2_RANGE  (15) ///< MR114
#define DDR5_DIMM_DFE_TAP3_RANGE  (12) ///< MR115
#define DDR5_DIMM_DFE_TAP4_RANGE  (9) ///< MR116

///
/// Vref related defines
///
#define DDR5_WRV_VREF_OFFSET_MIN     (-52)     ///< Minimum possible Vref offset
#define DDR5_WRV_VREF_OFFSET_MAX     (52)      ///< Maximum possible Vref offset
#define DDR5_WRV_VREF_RANGE_MAX      (105)
#define DDR5_CMD_VREF_OFFSET_MIN     (-47)     ///< Minimum possible Vref offset
#define DDR5_CMD_VREF_OFFSET_MAX     (47)      ///< Maximum possible Vref offset
#define DDR5_CMD_VREF_RANGE_MAX      (95)
#define DDR5_DQ_CMD_VREF_MID_ENC     (53)

//
// Defines for "all ranks Hot" for RANK_TEMPERATURE
//
#define RANK_TEMPERATURE_1XREF_DDR5     0x01010101
#define RANK_TEMPERATURE_HOT_DDR5       0x03

#define MRC_DDR5_CA_LANE_WIDTH (14)

#define DDR5_DFE_PER_TAP  (16)
#define DDR5_DFE_TAP_MR_DQ_OFFSET (8)

#define DDR5_ODT_OFFSET_DECODER (5)

extern const MrcModeRegister DFETap1Arrx16[DDR5_DFE_PER_TAP];
extern const MrcModeRegister DFETap2Arrx16[DDR5_DFE_PER_TAP];
extern const MrcModeRegister DFETap3Arrx16[DDR5_DFE_PER_TAP];
extern const MrcModeRegister DFETap4Arrx16[DDR5_DFE_PER_TAP];

extern const MrcModeRegister JedecInitSequenceMultiCycle[];
extern const MrcModeRegister JedecInitSequenceSingleCycle[];

//
/// Valid Clock Requirement after SRE (tCKLCS) fS
//
#define MRC_DDR5_tCSCLK_MIN          (5000000)
#define MRC_DDR5_tCSCLK_MIN_NCK      (8)

typedef struct {
  UINT16  RttWr;   ///< Wa - Write ODT on active rank
  UINT16  RttNomRd;
  UINT16  RttNomWr;
  UINT16  RttPark;
  UINT16  RttParkDqs;
} TOdtValueDqDdr5;

typedef struct {
  UINT16  RttCa;
  UINT16  RttCs;
  UINT16  RttCk;
} TOdtValueCccDdr5;

typedef struct {
  INT8    Tap1;
  INT8    Tap2;
  INT8    Tap3;
  INT8    Tap4;
} TDFEValueDdr5;

typedef struct {
  UINT8 DckOdt;
  UINT8 SlewRate;
  UINT8 Ron;
} TCkdValue;

typedef enum {
  DDR5_ODIC_34,     // 0 - RZQ/7 = 34 Ohm
  DDR5_ODIC_40,     // 1 - RZQ/6 = 40 Ohm
  DDR5_ODIC_48,     // 2 - RZQ/5 = 48 Ohm
  DDR5_ODIC_RSVD_1  // 3 - Reserved
} OutputDriverImpedanceControlDdr5;

// DDR5 Write leveling modes
typedef enum {
  Ddr5WrLvlModeDisable,   // MR2[1]: Write Leveling = 0, MR2[7]: Internal Write Timing = 0
  Ddr5WrLvlModeExternal,  // MR2[1]: Write Leveling = 1, MR2[7]: Internal Write Timing = 0
  Ddr5WrLvlModeInternal,  // MR2[1]: Write Leveling = 1, MR2[7]: Internal Write Timing = 1
  Ddr5WrLvlModeFinal      // MR2[1]: Write Leveling = 0, MR2[7]: Internal Write Timing = 1
} Ddr5WrLvlMode;

typedef union {
  struct {
    UINT32 RowBits0_3  : 4;
    UINT32 RowBits4_16 : 13;
    UINT32 RowBit17    : 1;
  }Bits;
  UINT32 Data32;
} Ddr5ActStruct;


// DDR5 DRAM_CMD_REG Masks
#define DDR5_MRW_CMD     (0x5)   // L-L-H-L-H (00101)
#define DDR5_MRR_CMD     (0x15)  // H-L-H-L-H (10101)
#define DDR5_MPC_CMD     (0xF)   // L-H-H-H-H (01111)
#define DDR5_PREA_CMD    (0xB)   // L-H-L-H-H (01011)
#define DDR5_VREF_CMD    (0x3)   // L-L-L-H-H (00011)
#define DDR5_NOP_CMD     (0x1F)  // H-H-H-H-H (11111)


// tECSinit configuration from DDR Jedec spec
// Table 150 - Average Periodic ECS Interval (tECSint)
// -----------------------------
//   16Gb   |   24Gb   |   32Gb
// -----------------------------
//   644uS  |   429uS  |   322uS
// -----------------------------
#define MRC_DDR5_tECSinit_16GB_US  644
#define MRC_DDR5_tECSinit_24GB_US  429
#define MRC_DDR5_tECSinit_32GB_US  322

// tREFI configuration from DDR Jedec spec / MC arch
// Table 71 - tREFI parameters for REFaband REFsb Commands
#define MRC_DDR5_tREFI_32GB_NS  1950  // 1.95usec - Defined in nsec to avoid usage of floating-point numbers
#define MRC_DDR5_tREFI_16GB_NS  7800  // 4 * 1.95usec
#define MRC_DDR5_tREFI_24GB_NS  3900  // 2 * 1.95usec

/**
  This function is used to get the timing parameter tRX_DQS2DQ Min or Max.

  @param[in]  MrcData    - Pointer to MRC global data.
  @param[in]  IsMin      - TRUE returns the minimum value, FALSE returns the maximum value.

  @retval UINT32 - The requested parameter in Femtoseconds.
**/
UINT32
Ddr5GetTdqs2dq (
  IN  MrcParameters   *const MrcData,
  IN  BOOLEAN         IsMin
  );


/**
  This function returns the tADCmin/max based on frequency.

  @param[in]  Frequency  - The memory frequency.
  @param[out] tADCmin    - Min limit for tADC based on freq
  @param[out] tADCmax    - Max limit for tADC based on freq
**/
VOID
MrcGetDdr5Tadc (
  IN const  MrcFrequency  Frequency,
  OUT       UINT8         *tADCmin,
  OUT       UINT8         *tADCmax
  );
/**
  Perform JEDEC Init sequence for DDR5.

  @param[in] MrcData - Pointer to MRC global data.

  @retval MrcStatus
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcJedecInitDdr5 (
  IN MrcParameters *const MrcData
  );

/**
  Perform JEDEC reset sequence for DDR5.

  @param[in] MrcData - Include all MRC global data.

  @retval - mrcSuccess if no errors encountered, mrcFail otherwise
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcJedecResetDdr5 (
  IN MrcParameters *const MrcData
  );
/**
  Set CAS latency on on DRAM

  @param[in] MrcData     - Include all MRC global data.
  @param[in] CasLatency  - CAS latency value:
                            00000B: 22
                            00001B: 24
                            00010B: 26
                            00011B: 28
                            ...
                            10011B: 60
                            10100B: 62
                            10101B: 64
                            10110B: 66
                            All other encodings reserved.

  @retval - mrcSuccess if no errors encountered, mrcFail otherwise
**/
MrcStatus
MrcDdr5SetCasLatency (
  IN MrcParameters  *const MrcData,
  UINT8                    CasLatency
  );

/**
  This function converts from the integer defined Read Latency to the Mode Register (MR0)
  encoding of the timing in DDR5.

  @param[in]  MrcData - Pointer to global MRC data.
  @param[in]  Value   - Requested Read Latency value.
  @param[out] EncVal  - Encoded Mode Register value.

  @retval MrcStatus - mrcSuccess if the latency is supported.  Else mrcWrongInputParameter.
**/
MrcStatus
EncodeReadLatencyDdr5 (
  IN  MrcParameters *MrcData,
  IN  UINT16        Value,
  OUT UINT8         *EncVal
  );

/**
  Used to update TxVref for DDR5.
  Uses input offset value to increment/decrement current setting.

  @param[in,out] MrcData        - Include all MRC global data.
  @param[in]     Controller     - Selecting which Controller to talk to.
  @param[in]     Channel        - Selecting which Channel to talk to.
  @param[in]     RankMask       - Selecting which Ranks to talk to.
  @param[in]     DeviceMask     - Selecting which Devices to talk to (only valid for DDR5 and adjusting VrefDQ).
  @param[in]     VrefType       - Determines the Vref type to change, only CmdV and TxVref are valid.
  @param[in]     Offset         - Vref offset value.
  @param[in]     UpdateMrcData  - Used to decide if Mrc host must be updated.
  @param[in]     PdaMode        - Selecting if we are using DDR5 PDA.
  @param[in]     IsCachedOffset - Determines if the paramter is an offset (relative to cache) or absolute value.

  @retval MrcStatus - mrcWrongInputParameter if unsupported OptParam,  mrcSuccess otherwise
**/
MRC_IRAM0_FUNCTION
MrcStatus
Ddr5SetDramVref (
  IN OUT MrcParameters *const MrcData,
  IN     UINT8                Controller,
  IN     UINT8                Channel,
  IN     UINT8                RankMask,
  IN     UINT16               DeviceMask,
  IN     UINT8                VrefType,
  IN     INT32                Offset,
  IN     BOOLEAN              UpdateMrcData,
  IN     BOOLEAN              PdaMode,
  IN     BOOLEAN              IsCachedOffset
  );

/**
  Offset to DDR5 Vref (DQ/CS/CA) Range/Value (MR10)

  ==================================================
  | VrefType |  Offset  | MR Value |  Vref % VDDQ  |
  ==================================================
  |    DQ    | -52 : 53 | 105 : 0  |   45% : 97.5% |
  |    CA    |          | 100 : 5  | 47.5% : 95.0% |
  |    CS    |          | 100 : 5  | 47.5% : 95.0% |
  ==================================================

  @param[in]  MrcData  - Include all MRC global data.
  @param[in]  VrefType - CmdV or WrV
  @param[in]  Offset   - Value to be converted to actual Vref (DQ/CS/CA) Range/Value.

  @retval DDR5 Vref (DQ/CS/CA) Range/Value
**/
MRC_IRAM0_FUNCTION
UINT8
MrcOffsetToVrefDdr5 (
  IN MrcParameters *const MrcData,
  IN MrcMarginTypes       VrefType,
  IN INT32                Offset
  );

/**
  DDR5 Vref (DQ/CS/CA) MR Value (MR10) to Offset

  ==================================================
  | VrefType |  Offset  | MR Value |  Vref % VDDQ  |
  ==================================================
  |    DQ    | -52 : 53 | 105 : 0  |   45% : 97.5% |
  |    CA    |          | 100 : 5  | 47.5% : 95.0% |
  |    CS    |          | 100 : 5  | 47.5% : 95.0% |
  ==================================================

  @param[in] Vref     - Vref MR Value to be converted back to corresponding Offset value.
  @param[in] VrefType - CmdV or WrV

  @retval Offset
**/
MRC_IRAM0_FUNCTION
INT32
MrcVrefToOffsetDdr5 (
  IN UINT8           Vref,
  IN MrcMarginTypes  VrefType
  );

/**
  This function returns the Write Postamble Setting MR8 encoded value.

  @param[in]  MrcData   - Pointer to global MRC data.
  @param[out] EncVal    - Encoded Mode Register value.

  @retval mrcSuccess the timing is supported
  @retval mrcWrongInputParameter the timing is not supported.
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcDdr5GetWritePostambleSetting (
  IN  MrcParameters   *MrcData,
  OUT DDR5_MR8_tWPOST *EncVal
  );

/**
  This function returns the Read Preamble Setting MR8 encoded value.

  @param[in]  MrcData   - Pointer to global MRC data.
  @param[out] EncVal    - Encoded Mode Register value.

  @retval MrcStatus - mrcSuccess if the timing is supported.  Else mrcWrongInputParameter.
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcDdr5GetReadPreambleSetting (
  IN  MrcParameters  *MrcData,
  OUT DDR5_MR8_tRPRE *EncVal
  );

/**
This function returns the Write Preamble Setting MR8 encoded value.

@param[in]  MrcData   - Pointer to global MRC data.
@param[out] EncVal    - Encoded Mode Register value.

@retval MrcStatus - mrcSuccess if the timing is supported.  Else mrcWrongInputParameter.
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcDdr5GetWritePreambleSetting (
  IN  MrcParameters  *MrcData,
  OUT DDR5_MR8_tWPRE *EncVal
  );

/**
  Enter / exit DDR5 CSTM or CATM, depending on Iteration.

  @param[in] MrcData     - The MRC global data.
  @param[in] Iteration   - Command training type: MrcIterationCtl for CSTM and MrcIterationCmd for CATM.
  @param[in] McChBitMask - McChannels to work on.
  @param[in] Rank        - Rank to work on
  @param[in] Enable      - Enable or disable the training mode.
  @param[in] IsPrint     - If TRUE enable debug messages; otherwise disable debug messages.

  @retval mrcSuccess if succeeded
**/
MrcStatus
MrcDdr5DramEctTrainMode (
  IN MrcParameters *const  MrcData,
  IN MrcIterationType      Iteration,
  IN UINT8                 McChBitMask,
  IN UINT32                Rank,
  IN UINT8                 Enable,
  IN BOOLEAN               IsPrint
  );

/**
  Get all the data for all Ranks all Devs for specific controller, channel and MR for DDR5

  Example 1: in DDR5 2R x8 with ECC the data will return as:
  MrPdaData[]= {R0D0,R1D0,R0D1,R1D1,R0D2,R1D2,R0D3,R1D3,R0ECC,R1ECC}

  Example 2: in DDR5 2DPC 1R-1R, DIMM0 x8, DIMM1 x16, no ECC - the data will return as:
  MrPdaData[]= {R0D0,R2D0,R0D1,R2D1,R0D2,R2D2,R0D3,R2D3}

  Example 3: in DDR5 2DPC 1R-1R, DIMM0 x16, DIMM1 x16, no ECC - the data will return as:
  MrPdaData[]= {R0D0,R2D0,R0D1,R2D1}

  Important:
  The *NumMrData can be different depending on the MR

  @param[in] MrcData      - Pointer to global MRC data.
  @param[in] Controller   - Controller to work on.
  @param[in] Channel      - channel to work on.
  @param[in] MrAddress    - MR Address
  @param[in] MrPdaData    - Array of Data that will be filled
  @param[in] NumMrData    - pointer that will contain # of data that filled in the array.

  @retval MrcStatus - mrcSuccess, otherwise an error status.
  **/
MrcStatus
MrFillPdaMrsDataDdr5 (
  IN      MrcParameters   *MrcData,
  IN      UINT32          Controller,
  IN      UINT32          Channel,
  IN      MrcModeRegister MrAddress,
  IN OUT  UINT8           MrPdaData[MAX_PDA_MR_IN_CHANNEL],
  IN OUT  UINT8           *NumMrData
  );

/**
  Update the Read Preamble type in MR8 and send this to DRAM.
  Update MR8 in the host struct as well

  @param[in]  MrcData -  Pointer to MRC global data.

  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcSetDdr5Mr8ReadPreamble (
  IN  MrcParameters *const MrcData
  );

/**
  Update the Ddr5 Rx Dqs Offset Timing in MR40 and send this to DRAM.
  Update MR40 in the host struct as well

  @param[in]  MrcData -  Pointer to MRC global data.

  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcSetDdr5Mr40 (
  IN  MrcParameters *const MrcData
  );

/**
  Program MR45 (DDR5) with  DQS interval timer run time

  @param[in] MrcData         - Include all MRC global data
  @param[in] DqioDuration    - DqioDuration

  @retval MrcStatus         - if it success return mrcSuccess,
                              if timed out waiting for MRH return mrcDeviceBusy
**/
MrcStatus
MrcSetDdr5Mr45 (
 IN     MrcParameters *const MrcData,
 IN     UINT8               DqioDuration
  );

/**
  This function will setup the default MR values for DDR5 based on
  DRAM Timings and Frequency in MRC global data.
  Only populated Channels and Ranks are initialized.

  @param[in, out]  MrcData   -  Pointer to MRC global data

  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
InitMrwDdr5 (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function converts from Odt Offset to DDR5 MR OdtlOff Wr encoding.

  @param[in] OdttOffWrOffset - Offset to push/pull out/in the WriteRTT or Non-Target Write RTT disable time

  @retval INT8 - Encoding if valid ODT Off Wr Offset.  Else, -1.
**/
INT8
Ddr5OdttOffWrOffset (
  IN  INT16  OdttOffWrOffset
  );

/**
  This function converts from Odt Offset to DDR5 MR OdtlOn Rd Nt encoding.

  @param[in]  OdttOnRdOffset - Offset to push/pull out/in the Non-Target Read RTT enable time

  @retval INT8 - Encoding if valid ODT On Rd Nt Offset.  Else, -1.
**/
INT8
Ddr5OdttOnRdNtOffset (
  IN  INT16  OdttOnRdNtOffset
  );

/**
  This function converts from Odt Offset to DDR5 MR OdtlOff Rd Nt encoding.

  @param[in]  OdttOffRdOffset  - Offset to push/pull out/in the Non-Target Read RTT disable time

  @return Encoding if valid ODT Off Rd Nt Offset.  Else, -1.
**/
INT8
Ddr5OdttOffRdNtOffset (
  IN  INT32  OdttOffRdNtOffset
  );

/**
  This function enables/disables DFE through MR111 for the given Controller/Channel/Rank

  @param[in]      MrcData         - Pointer to global MRC data.
  @param[in]      Controller      - Controller in use
  @param[in]      Channel         - Channel in use
  @param[in]      Rank            - Rank in use
  @param[in]      OptParam        - The Dimm Opt Param
  @param[in]      IsDFEEnabled    - TRUE if DFE should be supported

  @retval MrcStatus - mrcSuccess if a MRW was successful
**/
MrcStatus
Ddr5EnableDFE (
  IN  MrcParameters* const  MrcData,
  IN  UINT8  Controller,
  IN  UINT8  Channel,
  IN  UINT8  Rank,
  IN  UINT8  OptParam,
  IN  BOOLEAN IsDFEEnabled
  );

/**
  This function returns the value of tXSR in tCK.

  @param[in] tRFC  - Jedec timing tRFC

  @retval UINT32 - tXSR in tCK.
**/
UINT32
MrcGetDdr5Txsr (
  IN  UINT16  tRFC
  );

/**
  This function returns the tXSDLL in DCLKs based on DDR Type, and Frequency.
  DDR5 has values per frequency.

  @param[in] Frequency  - The memory frequency.
**/
UINT32
MrcGetDdr5Txsdll (
  IN const  MrcFrequency  Frequency
  );

/**
  This function returns the tRFM in DCLKs.
  DDR5 timing is based on tRFMsb

  @param[in] DeviceCapacity    - DRAM Device Capacity

  @retval tRFM - tRFM timing based on Device Capacity
**/
UINT32
MrcGetDdr5Trfm (
  IN  UINT32  DeviceCapacity
  );

/**
  This function returns the tMOD value.
  For DDR5 this is for "tRMD"

  @param[in] tCK       - DCLK period in femtoseconds.

  @retval UINT32 - Timing in tCK.
**/
UINT32
MrcGetDdr5Tmod (
  IN const UINT32  tCK
  );

/**
  This function returns the tMRW value.
  @param[in] tCK  - DCLK period in femtoseconds.
  @retval UINT32  - Timing in tCK.
**/
UINT32
MrcGetDdr5tMRW (
  IN const  UINT32  tCK
  );

/**
  This function returns the tContExitDelay value (time from Continuous Burst Mode Exit to next valid command).

  @param[in]  MrcData  - Include all MRC global data.

  @retval UINT32 - Timing in tCK.
**/
UINT32
MrcGetDdr5tContExitDelay (
  IN MrcParameters *const MrcData
  );

/**
  This function returns the DDR5 tZQCAL value in DCLKs.

  @param[in] tCK  - DCLK period in femtoseconds.

  @retval tZQCAL  - Timing in tCK.
**/
UINT32
MrcGetDdr5Tzqcal (
  IN UINT32               tCK
  );

/**
  This function returns the DDR5 tZQLAT value in DCLKs.

  @param[in] tCK  - DCLK period in femtoseconds.

  @retval tZQLAT  - Timing in tCK.
**/
UINT32
MrcGetDdr5Tzqlat (
  IN UINT32               tCK
  );

/**
  This function returns the DDR5 tZQCS value.

  @param[in] tZQLAT  - tZQLAT in femtoseconds over tCK.

  @retval tZQCS  - Value in tCK.
**/
UINT32
MrcGetDdr5Tzqcs (
  IN UINT32               tZQLAT
  );

/**
  This function converts from DDR5 MR5 PU/PD Drive Strength encoding to Ohms.

  @param[in]  DecodeVal - Decoded Ron value.

  @return Ron Value in Ohms if valid. Else, -1.
**/
INT16
DdrDrvStrDecode (
  IN  UINT16 DecodeVal
  );

/**
  This function returns the requested DelayType timing in nCK units.

  @param[in]  MrcData      - Pointer to global MRC data.
  @param[in]  DelayType    - Requested delay type
  @param[out] TimingNckOut - Output variable for the requested delay timing in nCK units

  @retval MrcStatus - mrcSuccess if the DelayType is supported.
                      mrcWrongInputParameter if TimingNckOut is NULL.
                      mrcTimingError if the timing value overflows the output pointer.
**/
MrcStatus
Ddr5GmfDelayTimings (
  IN  MrcParameters *const MrcData,
  IN  GmfTimingIndex       DelayType,
  OUT UINT16               *TimingNckOut
  );

/**
  Perform JEDEC Init sequence for DDR5.

  @param[in] MrcData  - Pointer to MRC global data.
  @param[in] Sequence - Array of MrcModeRegister values defining the sequence
                        of MRW and MPC commands to perform. The entire array
                        is index until the sentinel value mrEndOfSequence
                        is reached.
  @param[in] SagvConfig  - Configure the Generic FSM for SAGV transition (don't run the FSM and don't Clean it)

  @retval MrcStatus
**/
MrcStatus
PerformGenericMrsFsmSequence (
  IN MrcParameters *const MrcData,
  IN const MrcModeRegister  *const Sequence,
  IN BOOLEAN              SagvConfig
  );

/**
  DDR5 get available values and the number of possible values of a given DimmOptParam.

  @param[in]      MrcData               - Include all MRC global data.
  @param[in]      DimmOptParam          - e.g., OptDimmOdtWr, OptDimmOdtNom, OptDimmOdtPark, OptDimmRon
  @param[out]     **DimmOptParamVals    - Reference to the pointer of values.
  @param[out]     *NumDimmOptParamVals  - Reference to the number of values.

  @retval MrcStatus - mrcWrongInputParameter if unsupported OptParam, mrcSuccess otherwise
**/
MrcStatus
Ddr5GetDimmOptParamValues (
  IN MrcParameters *const MrcData,
  IN UINT8                DimmOptParam,
  OUT UINT16              **DimmOptParamVals,
  OUT UINT8               *NumDimmOptParamVals
  );

/**
  Ddr5 Get the MR value and its corresponding index for a given DIMM Opt Param.
  Value is set by reference to the corresponding pointers.

  @param[in]      MrcData     - Include all MRC global data.
  @param[in]      OptParam    - The Dimm Opt Param (e.g., OptDimmRon, OptDimmOdtWr, OptDimmOdtPark, OptDimmOdtNom)
  @param[out]     *MrIndex    - Updated Pointer to the MR index.
  @param[out]     *MrNum      - Updated Pointer to the MR number.

  @retval MrcStatus - mrcWrongInputParameter if unsupported OptParam, mrcSuccess otherwise
**/
MrcStatus
Ddr5GetOptDimmParamMrIndex (
  IN MrcParameters *const MrcData,
  IN UINT8                OptDimmParam,
  OUT UINT8               *MrIndex,
  OUT UINT16              *MrNum
  );

/**
  Ddr5 Set DimmParamValue is responsible for performing the concrete set DIMM parameter to value,
  using Ddr5 specific MR set functions.
  Parameters supported: OptDimmRonUp, OdtDimmRonDn, OptDimmOdtWr, OptDimmOdtNom, OptDimmOdtNomWr, OptDimmOdtNomRd, OptDimmOdtPark, OptDimmOdtParkDqs,
                        OptDimmOdtCAGroupA, OptDimmOdtCAGroupB, OptDimmOdtCSGroupA, OptDimmOdtCSGroupB, OptDdr5DimmDFETap1, OptDdr5DimmDFETap2,
                        OptDdr5DimmDFETap3, OptDdr5DimmDFETap4

  @param[in,out]  MrcData         - Include all MRC global data.
  @param[in,out]  MrData          - Pointer to the MR data to update.
  @param[in]      OptParam        - The Dimm Opt Param (e.g., OptDimmRon, OptDimmOdtWr, OptDimmOdtPark, OptDimmOdtNom)
  @param[in]      ParamValue      - The actual values (Typically in Ohms)

  @retval MrcStatus - mrcWrongInputParameter if unsupported OptParam, MrcStatus of the MR set functions otherwise

**/
MrcStatus
Ddr5SetDimmParamValue (
  IN OUT MrcParameters *const MrcData,
  IN OUT UINT8         *const MrData,
  IN     UINT8                OptParam,
  IN     UINT16               ParamValue
  );

/**
  Returns the Actual DIMM Driver/Odt Impedance in Ohms.
  Note: host structure calculation based.

  @param[in] MrcData       - Pointer to MRC global data.
  @param[in] Controller    - Zero based controller number.
  @param[in] Channel       - Zero based channel number.
  @param[in] Rank          - Zero based rank number.
  @param[in] OptParam      - Param to read.
  @param[in] Override      - Override host read value.
  @param[in] OverrideValue - Value to override.
  @param[in] GetFromTable  - Get the Values from Odt tables

  @retval Returns the DIMM driver impedance value in Ohms
**/
UINT16
Ddr5CalcDimmImpedance (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank,
  IN UINT8                OptParam,
  IN BOOLEAN              Override,
  IN UINT16               OverrideValue,
  IN BOOLEAN              GetFromTable
  );

/**
  Check if RFM is required through MR58 / MR59. If it requires, save 4 parameters into vars

   @param[in]  MrcData        - Pointer to global MRC data.
   @param[in]  Controller     - MC number
   @param[in]  Channel        - Channel number
   @param[in]  Dimm           - Dimm number
   @param[out] RAAIMT         - RFM RAAIMT
   @param[out] RAAMMT         - RFM RAAMMT
   @param[out] REF_SUB        - RFM REF_SUB
   @param[out] NORMAL_REF_SUB - RFM NORMAL_REF_SUB

   @retval TRUE RFM is required and RAAIMT and other outputs save the parameter values
   @retval FALSE RFM is required and RAAIMT and other outputs are changed
**/
BOOLEAN
MrcGetDdr5RowHammeringData (
  IN  MrcParameters* const MrcData,
  IN  UINT32  Controller,
  IN  UINT32  Channel,
  IN  UINT32  Dimm,
  OUT UINT32* RAAIMT,
  OUT UINT32* RAAMMT,
  OUT UINT32* REF_SUB,
  OUT UINT32* NORMAL_REF_SUB
  );
/**
  This function will either read MR32[6] directly from devices or if already read, get the data from the MR array.

  @param[in]      MrcData     - Pointer to global MRC data.
  @param[in]      Controller  - Controller to access
  @param[in]      Channel     - Channel to access
  @param[in]      Rank        - Rank to access

  @retval the ByteMask of whether each byte is Group A (0) or Group B (1)
**/
UINT8
MrcDdr5GetCaOdtStrap (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT32                Rank
  );

/**
  Check if the given MR is one of the DFE TAP MR's

  @param[in] MrAddr - MR address.

  @retval TRUE if this is one of the DFE TAP MR's
**/
BOOLEAN
Ddr5IsDimmDfeMr (
  IN UINT16 MrAddr
  );

/**
  Return the DFE TAP storage of MR according to the MR Address.
  There are 4 taps, for each tap we have 16 registers 8 for DQL and 8 for DQH, this is designed that DFE can be programmed per Bit.
  In our implementation we are programming per byte, (all Bit regs of the same byte will get the same value)
  for example :
  The MRs DFE PDA Tap1 DQL are mrMR129,mrMR137,mrMR145,mrMR153,mrMR161,mrMR169,mrMR177,mrMR185
  all of the will be configured with the same value (actually stored in RankOut: Ddr5PdaMr129)
  So the 64 MRS are mapped into 8 values.
  Also check if the MR represents the Bit0 in this TAP.

  @param[in]      MrAddr          - MR address
  @param[in, out] IsDimmDfeMrBit0 - Pointer used to return the Bit0 Indication.

  @retval Tap Index (0-7).
**/
UINT8
Ddr5GetDimmDfeTapStorage (
  IN  UINT16     MrAddr,
  OUT BOOLEAN   *IsDimmDfeMrBit0
  );

/**
  Config Dimm's DFE Tap1 and Tap2

  @param[in]  MrcData      - Include all MRC global data
  @param[in]  Tap1         - DFE Tap1 value
  @param[in]  Tap2         - DFE Tap2 value
**/
VOID
Ddr5ConfigDimmDFE (
  IN     MrcParameters *const MrcData,
  IN     INT8                 Tap1,
  IN     INT8                 Tap2
  );

/**
  Program MR48 (DDR5) with Data Invert Nibble
  MR48 should be programmed if WriteX feature is supported.
  If IsReadPatternSupported is FALSE, then program all 0's to MR48

  @param[in] MrcData                - Include all MRC global data
  @param[in] OdtMode                - Specifies the ODT Mode for DDRIO
  @param[in] IsReadPatternSupported - TRUE if MR48 should be programmed based on OdtMode, FALSE if MR48 should be programmed to 0's

  @retval mcrSuccess successfully programmed MR48
  @retval mrcFail failed to program MR48
**/
MrcStatus
MrcSetDdr5Mr48 (
  IN MrcParameters *const MrcData,
  IN MRC_ODT_MODE_TYPE    OdtMode,
  IN BOOLEAN              IsReadPatternSupported
  );

/**
  This function looks at the current MRW and the next command that's part of an MRS FSM sequence and returns
  the proper timing requirements.

  MRW      -> MRW          - use tMRW
  MRW      -> Non-MRW      - use tMRD
  MRW(DFE) -> non-MRW(DFE) - use tDFE
  MRW      -> PDA MRW      - use tMRD

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  CurMrAddr     - Current MRW MR address in the FSM sequence. (Assumption that this is a valid MR and an MRW command)
  @param[in]  NextMrAddr    - [OPTIONAL] Next MR address to be sent following current MR command.
  @param[in]  NextCmdType   - [OPTIONAL] Next MR Cmd Type to be sent following current MR command.
  @param[in]  Isx8DramWidth - True if x8 Dram Width. This is needed to ensure proper timing is used for MRW(DFE)
                             x8 should not program DFE DQU MRs (MR193-MR252)
  @param[out] DelayType     - Adjusted MRW delay for current MRW based on timing requirments between current MRW and next MR

  @retval mrcSuccess Delay Type Successfully determined
  @retval mrcFail DelayType Pointer is NULL
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcDdr5MrwGetTiming (
  IN  MrcParameters *const MrcData,
  IN  MrcModeRegister      CurMrAddr,
  IN  MrcModeRegister      NextMrAddr,
  IN  GmfCmdType           NextCmdType,
  IN  BOOLEAN              Isx8DramWidth,
  OUT GmfDdr5DelayType     *DelayType
  );

/**
  Determines if DFE is supported by reading MR111
  DFE is only supported if all Controller/Channel/Rank support the DFE Tap
  If a Tap is disabled, then all the subsequent taps are disabled as well.
  Ex. If Tap 3 is disabled, then Tap 4 will be forced to disabled as well.
  Ex. If Tap 1 is disabled, all taps are forced to disabled.

  @param[in]  MrcData        - Include all MRC global data.
  @param[out] OptParamLength - Length of DFE params to test

  @retval BOOLEAN - TRUE if DFE is supported on DIMM technology
**/
BOOLEAN
IsDdr5DFESupported (
  IN  MrcParameters  *MrcData,
  OUT UINT8          *OptParamLength
);

/**
  This function calculates the DDR5 timings tDQSCK and tDQSCKi
  tDQSCK Min/Max are the same value, except Min is negated

  @param[in]  MrcData - Include all MRC global data.
  @param[out] tDQSCK - Dqs rising edge output timing location from CK (tck*1000)
  @param[out] tDQSCKimax - Dqs rising edge output variance window (tck*1000)
**/
VOID
Ddr5GetTDqsCK (
  IN  MrcParameters* const MrcData,
  IN  UINT32               *tDQSCK,
  IN  UINT32               *tDQSCKimax
  );

/**
  This function calculates the DDR5 timings tDQSS
  tDQSSMin/Max are same value except Min is negated

  @param[in]  MrcData      - Include all MRC global data.
  @param[out] tDQSSMinMax - Voltage/Temperature drift window of first rising DQS pre-amble
                              edge relative to CWL CK edge (tck*1000)
  @param[in] WrPreambleLow - Write Preamble low time
**/
VOID
GetDdr5tDQSSDram (
  IN   MrcParameters *const MrcData,
  OUT  INT32                *tDQSSMinMax,
  IN   UINT8                WrPreambleLow
  );

/**
  This function issues the PDA Enumeration ID MPC. This will first send a MPC to enter PDA Enumeration Mode. It will then iterate through
  all DRAM devices and send an MPC to assign it an ID number, starting at 0. Once the iteration is done, a MPC will be sent to exit PDA Mode.
  This will wait tMPC Delay between PDA Entry and the first PDA Enumerate ID MPC's. This will wait tPDA_DELAY between the PDA Enumerate ID MPC's.

  @param[in]  MrcData   Pointer to global MRC data.

  @retval MrcStatus - mrcTimeout if the FSM does not complete after 1s.
  @retval MrcStatus - mrcSuccess if the MPC is sent successfuly.
  @retval MrcStatus - mrcFail for unexepected failures.
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcPdaEnumeration (
  IN MrcParameters *const MrcData
);

/**
  This function returns the Generic MRS FSM Command Type and Delay Type associated with the
  input MrcModeRegister value.

  @param[in]  MrcData      - Pointer to global MRC data.
  @param[in]  MrRegNum     - Requested MrRegiser
  @param[out] CmdTypeOut   - The Generic MRS FSM Command Type to use for the input MrRegister
  @param[out] DelayTypeOut - The type of delay to use for the input MrRegister

  @retval MrcStatus - mrcSuccess if the MrRegNum is supported. Else mrcWrongInputParameter.
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcDdr5GetGmfAttributes (
  IN  MrcParameters *const MrcData,
  IN  MrcModeRegister      MrRegNum,
  OUT GmfCmdType           *CmdTypeOut,  OPTIONAL
  OUT GmfDdr5DelayType     *DelayTypeOut OPTIONAL
  );

/**
  This function compiles the DRAM command needed to preform the specifc MR Cmd requested,
  with the given Data and address information

  @param[in] MrcData      - Include all MRC global data.
  @param[in] Address      - MRR / MRW address
  @param[in] Data         - MRW Data or MPC Opcode
  @param[in] MRCmd        - Cmd Type: MRR / MRW / MPC / NOP / VREF / PREA
  @param[in] IsMulticast  - True if Dram cmd is to be written as multicast
  @param[out] DramCmdData - Compiled Dram command value

  @retval mrcSuccess Valid Dram Command composed successfully
  @retval mrcFail MrCmd not supported
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcDdr5GetDramCommandMap (
  IN  MrcParameters* const  MrcData,
  IN  UINT32                Address,
  IN  UINT32                Data,
  IN  MrDramCmdType         MrCmd,
  IN  BOOLEAN               IsMultiCast,
  OUT UINT32                *MptuDramCmdData
  );

/**
  This function returns the pda_dealy value

  @param[in] MrcData    - Include all MRC global data.

  @retval UINT32 - Timing in ns
**/
UINT32
MrcGetTpdaDelay (
  IN MrcParameters *const MrcData
  );

/**
  This function calculates the DDR5 ODT Offsets that get programmed into MR37, MR38, and MR39

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus - If it succeeds return mrcSuccess.
**/
MrcStatus
MrcSetDdr5ODTTimings (
  IN MrcParameters *const MrcData
  );

/**
  Enable / Disable DRAM Continuous DQS Toggle mode for DDR5

  @param[in, out] MrcData - Include all MRC global data
  @param[in]      Enable  - Start / Stop the mode
  @param[in]      Rank    - Target rank

  @retval mrcSuccess if MR configuration is succesfull otherwise mrcFail.
**/
MrcStatus
StartDramDqsToggleModeDdr5 (
  IN OUT MrcParameters * const  MrcData,
  IN     BOOLEAN                Enable,
  IN     UINT32                 Rank
  );

#endif // MRC_DDR_5_H_

/**
  Check if refresh interval rate is implemented if so then enable it.

  @param[in]  MrcData        - The global host structure
  @param[out] IsRirSupported - True if DDR5 RIR is supported (MR4 OP[3] = 1)

  @return MrcStatus.
**/
MrcStatus
MrcTurnOnRirIfSupported (
  IN  MrcParameters* const MrcData,
  OUT BOOLEAN              *IsRirSupported
);

/*
  Program Ppr Guard key for s/h/mPpr or MBIST

  @param[in]  MrcData        - The global host structure
  @param[in] Controller      - Zero based controller number.
  @param[in] Channel         - Zero based channel number.
  @param[in] Rank            - Zero based rank number.

  @return MrcStatus.

*/

MrcStatus
Ddr5ProgramPprGuardKey (
  IN  MrcParameters* const MrcData,
  IN  UINT32               Controller,
  IN  UINT32               Channel,
  IN  UINT32               Rank
  );

/**
  This function will set up DFE value for DDR5 in PDA mode

  @param[in]      MrcData     - Pointer to global MRC data.
  @param[in]      Controller  - Controller index to work.
  @param[in]      Channel     - Channel index to work on.
  @param[in]      Rank        - Rank to store data.
  @param[in]      Device      - Device in use
  @param[in]      Byte        - Byte in use
  @param[in]      OptParam    - Param that is being updated
  @param[in]      DfeValue    - DFE value to program

  @retval MrcStatus - mrcSuccess if a supported ODT value, else mrcWrongInputParameter.
**/
MrcStatus
MrcDdr5SetDfePDA (
  IN  MrcParameters* const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT8                 Rank,
  IN  UINT8                 Device,
  IN  UINT8                 Byte,
  IN  UINT8                 OptParam,
  IN  INT16                 DfeValue
);