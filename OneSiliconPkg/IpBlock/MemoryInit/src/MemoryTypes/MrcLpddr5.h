/** @file
  Implementation of LPDDR5 Specific functions, and constants.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
  JEDEC
**/

#ifndef _MRC_LPDDR5_H_
#define _MRC_LPDDR5_H_

#include "MrcDdrCommon.h"
#include "MrcMemoryApi.h"
#include "MrcLpddr5Registers.h"
#include "MrcRowHammer.h"  // for MRC_RFM_SETUP_CONFIG

#define MRC_LP_tINIT1_US   200     ///< Min RESET_n low time after Voltage Ramp
#define MRC_LP_tINIT3_US   2000    ///< Min CKE low time after RESET_n high
#define MRC_LP_tINIT5_US   2       ///< Min idle time before first MR[W,R]

///
/// Multi-Purpose Commands (MPC)
///
#define MRC_LP5_MPC_START_WCK2DQI_OSC (0x81)
#define MRC_LP5_MPC_ZQCAL_START       (0x85)
#define MRC_LP5_MPC_ZQCAL_LATCH       (0x86)

///
/// WCK:CK 4:1 is POR
///
#define WCK_TO_CK_RATIO (4)

///
/// Timings
///
///  Precharge to Precharge Delay for all Frequencies in tCK
#define MRC_LP5_tPPD_ALL_FREQ (2)

/*
  tWCK2CK
  MR18 Op[5]  Min   Max
  0 (4:1)     -.5   .5
  1 (2:1)     -.25  .25
*/
//minimum nuber of cycles to get 1 CL in BG mode
#define MRC_LP5_BG_MODE_MIN_1CL_ACCESS_TIME (6)
#define MRC_LP5_BG_MODE_MIN_CAS2CAS_DELAY (2)
#define MRC_LP5_tRPST_INtCK (1)
#define WCK2CK_RATIO 4

/// WCKDQO (pS)
#define MRC_LP5_tWCKDQO_MIN (650)
#define MRC_LP5_tWCKDQO_LF_MAX (1900)
#define MRC_LP5_tWCKDQO_HF_MAX (1600)
#define MRC_LP5_tWCK2DQO_MAX_MIN_DIFF_IN_NCK (2)

/// WCKDQI (pS)
#define MRC_LP5_tWCKDQI_MIN (300)
#define MRC_LP5_tWCKDQI_LF_MAX (900)
#define MRC_LP5_tWCKDQI_MF_MAX (700)
#define MRC_LP5_tWCKDQI_HF_MAX (600)

// tCPDED
#define MRC_LP5_tCPDED (8)

// tCKCKEH (fs)
#define MRC_LP5_tCKCKEH     (1750000)
#define MRC_LP5_tCKCKEH_nCK (2)

// tCACSH (fs)
#define MRC_LP5_tCACSH (1750000)

// tCKCSH (nCK)
#define MRC_LP5_CKCSH (2)

///tWCK2DQO_Temp (fS)
#define MRC_LP5_tWCK2DQO_TEMP_LF (1800)
#define MRC_LP5_tWCK2DQO_TEMP_HF (1500)

///tWCK2DQO_Volt (pS * 10)
#define MRC_LP5_tWCK2DQO_VOLT_LF (50)
#define MRC_LP5_tWCK2DQO_VOLT_HF (30)

///tWCK2DQI_Temp (fS)
#define MRC_LP5_tWCK2DQI_TEMP_LF (700)
#define MRC_LP5_tWCK2DQI_TEMP_HF (600)

///tWCK2DQI_Volt (pS * 10)
#define MRC_LP5_tWCK2DQI_VOLT_LF (1000)
#define MRC_LP5_tWCK2DQI_VOLT_HF (500)

/// tODT On Off (pS)
#define MRC_LP5_tODT_ON_OFF_MIN (1500)
#define MRC_LP5_tODT_ON_OFF_MAX (3500)

/// tFC_Long = 250ns
#define MRC_LP5_tFC_LONG_NS (250)

/// tFC_Extended = 350ns
#define MRC_LP5_tFC_EXTENDED_NS (350)

/// Minimum interval between PDE and PDX or between PDX and PDE (tCSPD) fS
#define MRC_LP5_tCSPD_MIN           (10000000)
// tCSLCK (pS)
#define MRC_LP5_tCSLCK              (500)

/// Delay from valid command to PDE (tCMDPD) pS
#define MRC_LP5_tCMDPD_MIN_NCK      (3)

/// Valid Clock Requirement after PDE (tCSLCK) fS
#define MRC_LP5_tCSCLK_MIN          (5000000)
#define MRC_LP5_tCSCLK_MIN_NCK      (3)

/// Valid Clock Requirement after PDE (tCKFSPX) fS
#define MRC_LP5_tCKFSPX_MIN          (7500000)
#define MRC_LP5_tCKFSPX_MIN_NCK      (4)

/// LP5x tRCDw fs
#define MRC_LP5X_TRCDW_MIN          (8000000)
#define MRC_LP5X_TRCDW_DVFSC_MIN    (9000000)
#define MRC_LP5X_TRCDW_NCK_MIN      (2)

/// tRRD - Active bank-A to active bank-B
#define MRC_LP5_TRRD_8B_MIN_PS      (10000)
#define MRC_LP5_TRRD_BG_16B_MIN_PS  (5000)
#define MRC_LP5_TRRD_MIN_NCK        (2)

// tbpR2act - Timing before issuing an ACTIVATE command to a different non-refreshing bank.
#define MRC_LP5_tBPR2ACT_PS     7500 // 7.5ns
#define MRC_LP5_tBPR2ACT_8B_PS  10000 // 10ns (8 Bank mode)

// tRFMpb - Refresh Management Cycle time (per bank)
#define MRC_LP5_tRFMPB_8GB_PS   170000
#define MRC_LP5_tRFMPB_12GB_PS  190000
#define MRC_LP5_tRFMPB_16GB_PS  190000
#define MRC_LP5_tRFMPB_24GB_PS  260000
#define MRC_LP5_tRFMPB_32GB_PS  260000

// tRRFpb - Row Refresh (per bank)
#define MRC_LP5_tRRFPB_8GB_PS    85000
#define MRC_LP5_tRRFPB_12GB_PS   95000
#define MRC_LP5_tRRFPB_16GB_PS   95000
#define MRC_LP5_tRRFPB_24GB_PS  130000
#define MRC_LP5_tRRFPB_32GB_PS  130000

// tRFC - Refresh Cycle time (All banks)
#define MRC_LP5_tRFCAB_8GB_NS   230
#define MRC_LP5_tRFCAB_12GB_NS  300
#define MRC_LP5_tRFCAB_16GB_NS  300
#define MRC_LP5_tRFCAB_24GB_NS  410
#define MRC_LP5_tRFCAB_32GB_NS  410

// tRFC - Refresh Cycle time (per bank)
#define MRC_LP5_tRFCPB_8GB_NS   130
#define MRC_LP5_tRFCPB_12GB_NS  150
#define MRC_LP5_tRFCPB_16GB_NS  150
#define MRC_LP5_tRFCPB_24GB_NS  205
#define MRC_LP5_tRFCPB_32GB_NS  205

/// Valid Write Clock Request after CAS Command (tWCKSTOP) fS
#define MRC_LP5_tWCKSTOP_MIN          (6000000)
#define MRC_LP5_tWCKSTOP_MIN_NCK      (2)

/// Exit from Power-Down to next valid command delay (tXP)
#define MRC_LP5_tXP_MIN_NCK         (3)

/// Read Preamble Toggle (tWCK)
#define MRC_LP5_tRPRE_TOGGLE_2tWCK    (2)
#define MRC_LP5_tRPRE_TOGGLE_4tWCK    (4)
#define MRC_LP5_tRPRE_TOGGLE_4tWCK_2STATIC (6)

/// Minimum CS High Pulse width at PDX (tCSH) fS
#define MRC_LP5_tCSH_MIN            (3 * 1000 * 1000)

/// Minimum CS Low Duration time at PDX (tCSL) fS
#define MRC_LP5_tCSL_MIN            (4 * 1000 * 1000)

// Minimum Self Resfresh (tSR) fS
#define MRC_LP5_tSR_MIN             (15 * 1000 * 1000)

/// LPDDR5 ZQ Timings
/// (NZQ <= 4) 1.5us
#define tZQCAL_4NZQ_FS_LPDDR5 (1500000000)
/// (4 < NZQ <= 8) 3us
#define tZQCAL_8NZQ_FS_LPDDR5 (3000000000UL)
/// (8 < NZQ <= 16) 6us
#define tZQCAL_16NZQ_FS_LPDDR5 (6000000000UL)
#define tZQCAL_LPDDR5_CK_MIN 4
/// As above, in ns instead of fs
/// (NZQ <= 4) 1.5us
#define tZQCAL_4NZQ_NS_LPDDR5 (1500)
/// (4 < NZQ <= 8) 3us
#define tZQCAL_8NZQ_NS_LPDDR5 (3000)
/// (8 < NZQ <= 16) 6us
#define tZQCAL_16NZQ_NS_LPDDR5 (6000)

// tZQLAT MAX(30ns, 4nCK)
#define tZQLAT_LP5_FS      (30 * 1000 * 1000)
#define tZQLAT_LP5_MIN_NCK 4

// tZQRESET MAX(50ns, 3nCK)
#define tZQRESET_FS            (50 * 1000 * 1000)

// tZQSTOP 30ns
#define tZQSTOP_NS             (30)

/// tMRW for LPDDR4/5 max(10ns, 5nCK) in femtoseconds
#define tMRW_LPDDR_FS         (10 * 1000 * 1000)

/// tMOD for LPDDR4/5 max(14ns, 5nCK) in femtoseconds (tMRD)
#define tMOD_LPDDR_FS         (14 * 1000 * 1000)

/// Valid CA LOW requirement before CS change Low to High (tCA2CS_PRE) pS
#define MRC_LP5_tCA2CS_PRE_MIN      (1750)
#define MRC_LP5_tCA2CS_PRE_MIN_NCK  (2)

/// ECT Timing parameters in pS
#define MRC_LP5_tWCK2DQ7H_PS        (5000)
#define MRC_LP5_tADR_PS             (20000)
#define MRC_LP5_tDQ7LWCK_PS         (5000)
#define MRC_LP5_tVREFCA_LONG_PS     (250000)

/// tVrefCA_Long = 250ns
#define MRC_LP5_tVREFCA_LONG_NS     (250)

/// tVRCG_DISABLE = 100ns
#define MRC_LP5_tVRCG_DISABLE_NS    (100)
#define MRC_LP5_tVRCG_DISABLE_FS    (100 * 1000 * 1000)

/// tVRCG_ENABLE = 150ns
#define MRC_LP5_tVRCG_ENABLE_NS     (150)
#define MRC_LP5_tVRCG_ENABLE_FS     (150 * 1000 * 1000)

#define MRC_LP5_TXDQ_CENTER         (1024)
#define MRC_LP5_TXDQ_RANGE          (1020)

///< tCCD_L is = 8 tCK (32wck) for LP5 frequencies greater than 3200
///< tCCD_S is = 6 tCK (24wck) for LP5 frequencies greater than 3200.
///< tCCD is = 4 tCK (16wck) for LP5 frequencies less then or equal to 3200
#define TCCD_L_LP5_GT3200  (32)
#define TCCD_S_LP5_GT3200  (24)
#define TCCD_LP5_FREQ      (16)

///< MRR uses BL16 (half cacheline mode). Regular reads use BL32
///< tCCD_L is = BL/4 = 16/4 = 4 -> Need to set tCCD_L to 2 to avoid bubbles in BG mode during Read MPR
///< tCCD_S is = BL/8 = 16/8 = 2 (Non-BG mode)
///< tCCD_S is = 2 (BG mode)
#define TCCD_L_LP5_MRR    (2)
#define TCCD_S_LP5_MRR    (2)

/// MR30 range for DCA
#define LP5_DCA_VALID_RANGE     (7)
#define LP5_RDDCA_TEMP          (25)

/// ODT C/A Value Update Time tODTUP (pS)
#define LPDDR5_CA_ODT_DELAY (250000)

#define LPDDR5_CA_ODT_DELAY_NS (250)

/// tOSCO = max(40ns, 8nCK). 40ns is the max for all supported frequencies
#define MRC_LP5_tOSCO_FS  (40 * 1000 * 1000)
#define MRC_LP5_tOSCO_nCK (8)

#define ZQCS_PERIOD_LPDDR 128

// tPGM (ms) PPR Programming Time
#define MRC_LPDDR5_tPGM 2000
// tPGMPST (us) PPR new Address Setting Time
#define MRC_LPDDR5_tPGMPST 500
// tPGM_Exit (ns) PPR Exit Time
#define MRC_LPDDR5_tPGM_EXIT 15

///
/// Vref related defines
///
#define LP5_VREF_OFFSET_MIN      (-58)     ///< Minimum possible Vref offset for Write/Command Vref
#define LP5_VREF_OFFSET_MAX      (58)      ///< Maximum possible Vref offset for Write/Command Vref
#define LP5_VREF_MIN_MV          (75)      ///< mV
#define LP5_CA_VREF_MAX          (350)     ///< mV
#define LP5_DQ_VREF_LOW_F_MAX    (350)     ///< mV
#define LP5_DQ_VREF_EDVFSC_F_MAX (300)     ///< mV
#define LP5_DQ_VREF_HIGH_F_MAX   (225)     ///< mV
#define LP5_VREF_MIN_UV          (75000)   ///< uV
#define LP5_VREF_STEP_SIZE       (2500)    ///< uV

#define LP5_DQ_VREF_MAX_SUPPORTED_MR_CODE (0x46)          ///< 225mV
#define LP5_DQ_VREF_MAX_SUPPORTED_OFFSET  (2)             ///< 225mV

#define LP5_DQ_VREF_MAX_SUPPORTED_MR_CODE_EDVFSC (0x64)   ///< 300mV
#define LP5_DQ_VREF_MAX_SUPPORTED_OFFSET_EDVFSC  (32)     ///< 300mV

#define LSB_WCK2DQI_OSQ_MR (35)
#define MSB_WCK2DQI_OSQ_MR (36)
#define LP5_DEFAULT_NT_DQ_ODT (0x3) ///< (RZQ/3) Encoded NT DQ ODT for MR41

///
/// tXP value for LPDDR5: max(7ns, 3nCK)
///
#define MRC_LPDDR5_tXP_FS (7000000)

///
/// tPDXCSODTON value for LPDDR5: 20ns
///
#define MRC_LPDDR5_tPDXCSODTON (20000000)

///
/// tWR values for LPDDR5: x16 -> 34ns, x8 -> 36ns in fs
///
#define MRC_LPDDR5_x16_tWR (34000000)
#define MRC_LPDDR5_x8_tWR  (36000000)
#define MRC_LPDDR5_tWR_MIN (3)

//
// Defines for "all ranks Hot" for RANK_TEMPERATURE
//
#define RANK_TEMPERATURE_HOT_LPDDR5     0x0f
#define RANK_TEMPERATURE_1XREF_LPDDR5   0x09090909

#define MRC_LP5_CA_LANE_WIDTH (7)

///
/// DIMM DFE Tap defines
///
#define LPDDR5_DIMM_DFE_TAP_RANGE  (7) ///< MR24

///
/// LP5 Pre Emphasis define
///
#define LPDDR5_EMPHASIS_RANGE  (3) ///< MR58

// MPTU CS WIDTH selection and CBT Phase selection
#define LP5_DRAM_CS_WIDTH (1)
#define LP5_CBT_PH_RISING (0)
#define LP5_CBT_PH_FALLING (1)

// LPDDR5 DRAM_CMD_REG Masks
#define LP5_MRW_CMD_RISE_EDGE1   (0x58)  // H-L-H-H-L-L-L (1011000)
#define LP5_MRW_CMD_RISE_EDGE2   (0x08)  // L-L-L-H-L-L-L (0001000)
#define LP5_MRR_CMD_RISE_EDGE    (0x18)  // L-L-H-H-L-L-L (0011000)
#define LP5_MPC_CMD_RISE_EDGE    (0x30)  // L-H-H-L-L-L-L (0110000)
#define LP5_SRE_CMD_RISE_EDGE    (0x68)  // H-H-L-H-L-L-L (1101000)
#define LP5_PREA_CMD_RISE_EDGE   (0x78)  // H-H-H-H-L-L-L (1111000)
#define LP5_PREA_CMD_FALL_EDGE   (0x40)  // H-L-L-L-L-L-L (1000000)
#define LP5_WSFS_CMD_RISE_EDGE   (0x4c)  // H-L-L-H-H-L-L (1001100)
#define LP5_WS_OFF_CMD_RISE_EDGE (0x7c)  // H-H-H-H-H-L-L (1111100)
#define LP5_NOP_CMD_RISE_EDGE    (0)     // L-L-L-L-L-L-L (0)
#define LP5_ACT1_CMD_RISE_EDGE   (0x07)  //         H-H-H (    111)
#define LP5_ACT2_CMD_RISE_EDGE   (0x03)  //         L-H-H (    011)

/// tFC_Long = 250ns.  Common for LPDDR5
#define MRC_LP_tFC_LONG_NS (250)

#define LP5_READ_LATENCY_VALUES       15
#define LP5_READ_LATENCY_VALUES_DVFSC 6

typedef enum {
  MrcLp5BgMode,
  MrcLp58Bank,
  MrcLp516Bank
} MRC_LP5_BANKORG;

typedef enum {
  MrcLp5WckSafe,
  MrcLp5WckManual,
  MrcLp5WckDynamic,
  MrcLp5WckOff
} MRC_LP5_WCKMODE;

typedef union {
  struct {
    UINT32 RowBits0_6   : 7;
    UINT32 RowBits7_10  : 4;
    UINT32 RowBits11_13 : 3;
    UINT32 RowBits14_17 : 4;
  }Bits;
  UINT32 Data32;
} LpDdr5ActStruct;

///
/// Constants
///

#define LP5_RZQ_NUM_VALUES (7)     ///< Number of ODT encodings in LPDDR MR's

// This table is the list of possible terminations the DRAM can achieve using ZQ Resistor.
extern const UINT16 Lp5RzqValues[LP5_RZQ_NUM_VALUES];

// Valid Config Table for PU-Cal versus Soc ODT
extern const BOOLEAN  PuCalSocOdtValidLp5[OdtMax];

///
/// External functions
///

/**
  This function returns the requested MrAddr delay type

  @param[in]  MrcData      - Pointer to global MRC data.
  @param[in]  MrAddr       - MR Address to look up
  @param[out] DelayTypeOut - Output variable for the requested delay type
  @param[out] IsLastEntry  - Specifies if this is the last entry in the Generic MRS FSM sequence

  @retval mrcSuccess if the DelayType is supported. Else mrcWrongInputParameter.
  @retval mrcWrongInputParameter TimingNckOut is NULL
**/
MrcStatus
Lpddr5GmfDelayType (
  IN  MrcParameters* const MrcData,
  IN  MrcModeRegister      MrAddr,
  OUT GmfLpddr5DelayType* DelayTypeOut,
  IN  BOOLEAN              IsLastEntry
  );

/**
  This functions checks to see if the requested latency is within the frequency range
  required by JEDEC.

  @param[in]  MrcData - Pointer to MRC global data.
  @param[in]  Latency - Encoded latency entry.  Supports Read Latency, Write Latency, tWR, and tRTP.

  @retval MrcStatus - mrcSuccess if Latency is valid and the frequency is in the range.  Else mrcWrongInputParameter.
**/
MrcStatus
LatencyFreqCheckLpddr5 (
  IN  MrcParameters *MrcData,
  IN  INT8          Latency
  );

/**
  This function returns the impact to Write Latency for the requested LPDDR_ODTL_PARAM.

  @param[in]  MrcData - Pointer to MRC global data.
  @param[in]  Frequency - Data Rate.
  @param[in]  OdtlParam - Select between On or Off timing.
  @param[in]  Lp5BankOrg - Select the bank orginizaiton

  @retval INT8 - Timing impact.
**/
INT8
MrcGetWrOdtlLpddr5 (
  IN  MrcParameters *const MrcData,
  IN  MrcFrequency         Frequency,
  IN  LPDDR_ODTL_PARAM     OdtlParam,
  IN  MRC_LP5_BANKORG      Lp5BankOrg
  );

/**
  This function returns the impact to Read Latency for Non-target ODT of the requested LPDDR_ODTL_PARAM.
  Only supports BL32 8-Bank mode.

  @param[in]  Frequency - Data Rate.
  @param[in]  OdtlParam - Select between On or Off timing.

  @retval INT8 - Timing impact.
**/
INT8
MrcGetNtRdOdtlLpddr5 (
  IN  MrcFrequency      Frequency,
  IN  LPDDR_ODTL_PARAM  OdtlParam
  );

/**
  This function returns tWCKPRE_Static for both Writes and Reads in 4:1 mode.
  The implementation is based off of Table 26/27 WCK2CK Sync AC Parameters for Write/Read Operation

  @param[in]  Frequency - Data Rate.

  @retval INT8 - Timing in tCK
**/
INT8
MrcGetWckPreStaticLpddr5 (
  IN  MrcFrequency  Frequency
  );

/**
  This function returns tWCKENL_FS in 4:1 mode.
  The implementation si based off of Table 28 WCK2CK Sync AC Paramters for CAS(WS_FAST)

  @param[in]  Frequency - Data Rate.

  @retval UINT8 - Timing in tCK
**/
UINT8
MrcGetWckEnlFsLpddr5 (
  IN  MrcFrequency  Frequency
  );

/**
  This function returns tWCKPRE_total_WR.
  The implementation is based off of Table 26 WCK2CK Sync AC Parameters for Write Operation

  @param[in]  Frequency - Data rate.

  @retval UINT8 - Timing in tCK.
**/
UINT8
MrcGetWckPreWrTotalLpddr5 (
  IN  MrcFrequency  Frequency
  );

/**
  This function returns tWCKPRE_total_RD for SetA or SetB.
  This function assumes DVFSC is disabled and DBI is off.
  The implementation is based off of Table 27 WCK2CK Sync AC Parameters for Read Operation.

  @param[in]  MrcData - Pointer to MRC global data.
  @param[in]  Frequency - Data rate.

  @retval UINT8 - Timing in tCK.
**/
INT8
MrcGetWckPreRdTotalLpddr5 (
  IN  MrcParameters *const  MrcData,
  IN  MrcFrequency  Frequency
  );

/**
  This function will issue the JEDEC init MR sequence for LPDDR5.
  If RestoreMRs is set, the sequence will use the MR values saved in
  the MRC global data.  Otherwise, an initial value is used.
  Flow:
   1. Set Low frequency (1100)
   2. Send FSP-OP[0] MR2 to program RL of the high frequency
     - This is needed for DQ mapping step of ECT
   3. Set FSP-WR = 1, FSP-OP = 0
   4. Send all MRs
   5. If ECT_Done
     a. Set High frequency
     b. Set FSP-OP = 1

  @param[in]  MrcData - Pointer to MRC global data.

  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcJedecInitLpddr5 (
  IN  MrcParameters *const  MrcData
  );

/**
  This function will set up the pointer passed in based on LPDDR5 Mode Register definition.
  If MRC_IGNORE_ARG8 is passed in, that parameter is ignored.

  @param[in]      MrcData   - Pointer to global MRC data.
  @param[in]      FspWrite  - Frequency Set Point write enable switch.
  @param[in]      FspOpMode - Frequency Set Point Operation Mode switch.
  @param[in]      CbtMode   - Command Bus Training mode switch.
  @param[in]      VrcgMode  - VREF Current Generator mode switch.
  @param[in]      CbtPhase  - Controls which phase of the clock the CA pattern is latched.
  @param[in,out]  MrData    - Pointer to MR data to update.
**/
MrcStatus
MrcLpddr5SetMr16 (
  IN      MrcParameters *const  MrcData,
  IN      UINT8                 FspWrite,
  IN      UINT8                 FspOpMode,
  IN      UINT8                 CbtMode,
  IN      UINT8                 VrcgMode,
  IN      UINT8                 CbtPhase,
  IN OUT  UINT8         *const  MrData
  );

/**
  This function will set up the pointer passed in based on LPDDR5 Mode Register definition.
  If MRC_IGNORE_ARG8 is passed in, that parameter is ignored.

  @param[in]      MrcData         - Pointer to global MRC data
  @param[in]      EnhRdqsTraining - Enhanced RDQS training mode
  @param[in]      RdqsToggle      - RDQS toggle mode
  @param[in]      FifoRdqsTrain   - WCK-RDQS_t/Parity Training mode
  @param[in,out]  MrData          - Pointer to MR data to update.

**/
VOID
MrcLpddr5SetMr46 (
  IN      MrcParameters *const  MrcData,
  IN      UINT8                 EnhRdqsTraining,
  IN      UINT8                 RdqsToggle,
  IN      UINT8                 FifoRdqsTrain,
  IN OUT  UINT8         *const  MrData
  );

/**
  This function will set up the pointer passed in based on LPDDR5 Mode Register definition.
  DqNTOdt does error checking against spec valid values.

  @param[in]      MrcData - Pointer to global MRC data.
  @param[in]      DqNTOdt - Data NT ODT value to set (in Ohms).
  @param[in,out]  MrData  - Pointer to MR data to update.

  @retval MrcStatus - mrcSuccess if a supported ODT value, else mrcWrongInputParameter.
**/
MrcStatus
MrcLpddr5SetMr41 (
  IN      MrcParameters *const  MrcData,
  IN      UINT16                DqNTOdt,
  IN OUT  UINT8         *const  MrData
  );

/**
  This function will update the MrcModeRegister pointer and MR Delay arrays with the sequence that enables
  SAGV functionality in normal operation.  The list can be different depending on the DRAM used: x8/x16.

  @param[in]      MrcData - Pointer to the MRC global data.
  @param[out]     MrSeq   - Output array for the MR address sequence.
  @param[out]     MrDelay - Output array for the delay for the MR action at those indexes.
  @param[in, out] Length  - Pointer to the length of the output array pointers and the length of the MR sequence.
  @param[out] MrPerRank   - Output pointer to an array containing a list of MRs that must be configured on a per-rank basis
  due to possible unique values per rank. The is terminated using the value mrEndOfSequence

  @retval mrcFail - If the pointers are NULL.
  @retval mrcFail - if the array length is smaller than the MR sequence.
  @retval mrcSuccess - If neither of the error checks are met.
**/
MrcStatus
MrcSagvMrSeqLpddr5 (
  IN      MrcParameters *const  MrcData,
  OUT     MrcModeRegister       *MrSeq,
  OUT     GmfTimingIndex        *MrDelay,
  IN OUT  UINT32                *Length,
  OUT     MrcModeRegister       **MrPerRank OPTIONAL
  );

/**
  This function returns the requested DelayType in nCK units.

  @param[in]  MrcData      - Pointer to global MRC data.
  @param[in]  DelayType    - Requested delay type
  @param[out] TimingNckOut - Output variable for the requested delay timing in nCK units

  @retval mrcSuccess if the DelayType is supported. Else mrcWrongInputParameter.
  @retval mrcWrongInputParameter TimingNckOut is NULL
  @retval mrcTimingError if the timing values is geater than MRC_UINT16_MAX
**/
MrcStatus
Lpddr5GmfDelayTimings (
  IN  MrcParameters *const MrcData,
  IN  GmfTimingIndex       DelayType,
  OUT UINT16               *TimingNckOut
  );

/**
  This function converts from DRAM Vref encoding to MRC training offset:
  Vref [0:127] - Offset [10:117] :: LP5 (15% - 73.5%) * 500mV.

  @param[in]  MrcData - Pointer to global MRC data.
  @param[in]  Vref    - Vref MR encoding.
  @param[out] Offset  - Pointer to return training index.

  @retval mrcSuccess              Input parameters are valid (LPDDR5 Spec).
  @retval mrcWrongInputParameter  Input parameters are invalid (LPDDR5 Spec).
**/
MrcStatus
MrcVrefEncToOffsetLpddr5 (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 Vref,
  OUT INT32                 *Offset
);

/**
  Used to update TxVref and CaVref for LPDDR5.
  Uses input offset value to increment/decrement current setting.

  @param[in,out] MrcData        - Include all MRC global data.
  @param[in,out] Controller     - Selecting which Controller to talk to.
  @param[in]     Channel        - Selecting which Channel to talk to.
  @param[in]     RankMask       - Selecting which Ranks to talk to.
  @param[in]     VrefType       - Determines the Vref type to change, only CmdV and TxVref are valid.
  @param[in]     Offset         - Vref offset value.
  @param[in]     UpdateMrcData  - Used to decide if Mrc host must be updated.
  @param[in]     IsCachedOffsetParam - Determines if the parameter is an offset (relative to cache) or absolute value.

  @retval MrcStatus - mrcWrongInputParameter if unsupported OptParam,  mrcSuccess otherwise
**/
MrcStatus
Lpddr5SetDramVref (
  IN OUT MrcParameters *const MrcData,
  IN     UINT8                Controller,
  IN     UINT8                Channel,
  IN     UINT8                RankMask,
  IN     UINT8                VrefType,
  IN     INT32                Offset,
  IN     BOOLEAN              UpdateMrcData,
  IN     BOOLEAN              IsCachedOffsetParam
  );

/**
  Lpddr5 Set DimmParamValue is responsible for performing the concrete set DIMM paramter to value,
  using Lpddr specific MR set functions.
  Parameters supported: OptDimmRon, OptDimmOdtWr

  @param[in,out]  MrcData         - Include all MRC global data.
  @param[in,out]  MrData          - Pointer to the MR data to update.
  @param[in]      OptParam        - The Dimm Opt Param (e.g., OptDimmRon, OptDimmOdtWr, OptDimmOdtPark, OptDimmOdtNom)
  @param[in]      ParamValue      - The actual values (Typically in Ohms)

  @retval MrcStatus - mrcWrongInputParameter if unsupported OptParam, MrcStatus of the MR set functions otherwise

**/
MrcStatus
Lpddr5SetDimmParamValue(
  IN OUT MrcParameters *const MrcData,
  IN OUT UINT8         *const MrData,
  IN     UINT8                OptParam,
  IN     UINT16               ParamValue
);

/**
  Lppdr5 Get the MR value and its corresponding index for a given DIMM Opt Param.
  Value is set by reference to the corresponding pointers.

  @param[in]      MrcData     - Include all MRC global data.
  @param[in]      OptParam    - The Dimm Opt Param (e.g., OptDimmRon, OptDimmOdtWr, OptDimmOdtPark, OptDimmOdtNom)
  @param[out]     *MrIndex    - Updated Pointer to the MR index.
  @param[out]     *MrNum      - Updated Pointer to the MR number.

  @retval MrcStatus - mrcWrongInputParameter if unsupported OptParam, mrcSuccess otherwise
**/
MrcStatus
Lpddr5GetOptDimmParamMrIndex(
  IN MrcParameters *const MrcData,
  IN UINT8                OptDimmParam,
  OUT UINT8               *MrIndex,
  OUT UINT16              *MrNum
);

/**
  LPDDR5 get available values and the number of possible values of a given DimmOptParam.

  @param[in]      MrcData               - Include all MRC global data.
  @param[in]      DimmOptParam          - e.g., OptDimmOdtWr, OptDimmOdtNom, OptDimmOdtPark, OptDimmRon
  @param[out]     **DimmOptParamVals    - Reference to the pointer of values.
  @param[out]     *NumDimmOptParamVals  - Reference to the number of values.

  @retval MrcStatus - mrcWrongInputParameter if unsupported OptParam, mrcSuccess otherwise
**/
MrcStatus
Lpddr5GetDimmOptParamValues (
  IN MrcParameters *const MrcData,
  IN UINT8                DimmOptParam,
  OUT UINT16              **DimmOptParamVals,
  OUT UINT8               *NumDimmOptParamVals
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
Lpddr5CalcDimmImpedance (
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
  This function returns the Bank/BankGroup Organization based on Frequency.

  @param[in]  MrcData     - Include all MRC global data.
  @param[in]  Frequency   - Data Rate.

  @retval MRC_LP5_BANKORG - Bank/BankGroup Organization.
**/
MRC_LP5_BANKORG
MrcGetBankBgOrg (
  IN MrcParameters *const MrcData,
  IN  MrcFrequency      Frequency
  );

/**
  This function will setup the default MR values for LPDDR5 based on
  DRAM Timings and Frequency in MRC global data.
  Only populated Channels and Ranks are initialized.

  @param[in]  MrcData -  Pointer to MRC global data.

  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
InitMrwLpddr5 (
  IN  MrcParameters *const MrcData
  );

/**
  Program MR37 (LPDDR5) with  WCK2DQI interval timer run time

  @param[in] MrcData         - Include all MRC global data
  @param[in] DqioDuration    - DqioDuration

  @retval MrcStatus         - if it success return mrcSuccess,
                              if timed out waiting for MRH return mrcDeviceBusy
**/
MrcStatus
MrcSetLp5MR37 (
 IN     MrcParameters *const MrcData,
 IN     UINT8               DqioDuration
  );

/**
  This function returns the value of tXSR in tCK.

  @param[in] tCK   - Jedec tCK timing value
  @param[in] tRFC  - Jedec tRFC timing value

  @retval UINT32 - tXSR in tCK.
**/
UINT32
MrcGetLpddr5Txsr (
  IN  UINT32  tCK,
  IN  UINT16  tRFC
  );

/**
  This function returns the tRFM in DCLKs.
  LPDDR5 timing is based on tRFMpb

  @param[in] DeviceCapacity - DRAM Device Capacity

  @retval tRFM - tRFM timing based on Device Capacity
**/
UINT32
MrcGetLpddr5Trfm (
  IN UINT32 DeviceCapacity
  );

/**
  This function returns the tMOD value.

  @param[in] tCK  - DCLK period in femtoseconds.

  @retval UINT32  - Timing in tCK.
**/
UINT32
MrcGetLpddr5Tmod (
  IN const  UINT32  tCK
  );

/**
  This function returns the tMRW value.
  @param[in] tCK  - DCLK period in femtoseconds.
  @retval UINT32  - Timing in tCK.
**/
UINT32
MrcGetLpddr5tMRW (
  IN const  UINT32  tCK
  );

/**
  This function returns the LPDDR5 tZQCAL value in DCLKs.

  @param[in]  MrcData        - Pointer to global MRC data.
  @param[in] tCK  - DCLK period in femtoseconds.

  @retval tZQCAL  - Timing in nCK.
**/
UINT32
MrcGetLpddr5Tzqcal (
  IN  MrcParameters  *MrcData,
  IN UINT32  tCK
  );

/**
  This function returns the LPDDR5 tZQLAT value in tCK.

  @param[in] tCK  - CK period in femtoseconds.

  @retval tZQLAT  - Timing in tCK.
**/
UINT32
MrcGetLpddr5Tzqlat (
  IN UINT32 tCK
  );

/**
  Check if Adaptive Refresh Management (ARFM) is supported in MR1

   @param[in]  MrcData        - Pointer to global MRC data
   @param[in]  Controller     - MC number
   @param[in]  Channel        - Channel number
   @param[in]  Dimm           - DIMM number

   @retval - FALSE if ARFM not supported (cannot check ARFM Level)
   @retval - TRUE means ARFM is supported (can check ARFM Level)
**/
BOOLEAN
MrcLpddr5IsArfmSupported (
  IN  MrcParameters* const MrcData,
  IN  UINT32  Controller,
  IN  UINT32  Channel,
  IN  UINT32  Dimm
  );

/**
  Check if Refresh Management (RFM) is required by checking RFM Required bit in MR27

   @param[in]  MrcData         - Pointer to global MRC data.
   @param[in]  Controller      - MC number
   @param[in]  Channel         - Channel number
   @param[in]  Dimm            - Dimm number

   @retval - FALSE means RFM not required
   @retval - TRUE means RFM required
**/
BOOLEAN
MrcLpddr5IsRfmRequired (
  IN  MrcParameters* const MrcData,
  IN  UINT32  Controller,
  IN  UINT32  Channel,
  IN  UINT32  Dimm
  );

/**
  Grab Refresh Management (RFM) threshold info from MR27 and MR57
  All of these threshold values are used in operations on Rolling Accumulated ACT (RAA) counter

   @param[in]  MrcData         - Pointer to global MRC data.
   @param[in]  Controller      - MC number
   @param[in]  Channel         - Channel number
   @param[in]  Dimm            - Dimm number
   @param[out] Raaimt          - RAA Initial Management Threshold (RAAIMT)
   @param[out] Raammt          - RAA Maximum Management Threshold (RAAMMT)
   @param[out] RefSub          - Decrement for RAA from RFMab or RFMpb commands (RAAIMT * RAADEC)
   @param[out] NormalRefSub    - Decrement for RAA from normal refresh commands (RAAIMT)

   @retval - mrcFail if bad RAAIMT value
   @retval - mrcSuccess if RFM threshold info gathered successfully
**/
MrcStatus
MrcLpddr5GetRfmThresholdInfo (
  IN MrcParameters *const MrcData,
  IN UINT32  Controller,
  IN UINT32  Channel,
  IN UINT32  Dimm,
  OUT MRC_RFM_SETUP_CONFIG *RfmSetupConfig
  );

/**
  Check if Directed Refresh Management (DRFM) is supported based on MR1

   @param[in]  MrcData        - Pointer to global MRC data.
   @param[in]  Controller     - MC number
   @param[in]  Channel        - Channel number
   @param[in]  Dimm           - Dimm number

   @retval - FALSE mean DRFM is not supported
   @retval - TRUE means DRFM is supported
**/
BOOLEAN
MrcLpddr5IsDrfmSupported (
  IN  MrcParameters* const MrcData,
  IN  UINT32  Controller,
  IN  UINT32  Channel,
  IN  UINT32  Dimm
  );

/**
  Configure Adaptive Refresh Management (ARFM) Levels in MR57 based on BIOS Inputs

   @param[in]  MrcData        - Pointer to global MRC data.
   @param[in]  Controller     - MC number
   @param[in]  Channel        - Channel number
   @param[in]  Dimm           - Dimm number

   @retval - mrcSuccess if ARFM configured correctly
**/
MrcStatus
MrcLpddr5ConfigArfm (
  IN  MrcParameters* const MrcData,
  IN  UINT32  Controller,
  IN  UINT32  Channel,
  IN  UINT32  Dimm
  );

/**
  Configure Directed Refresh Management (DRFM) Enable and Blast Radius Configuration (BRC) through MR75

   @param[in]  MrcData        - Pointer to global MRC data.
   @param[in]  Controller     - MC number
   @param[in]  Channel        - Channel number
   @param[in]  Dimm           - Dimm number
   @param[out] BrcSupported   - True if BRC supported by the DIMM

   @retval - mrcSuccess if DRFM successfully configured
**/
MrcStatus
MrcLpddr5ConfigDrfm (
  IN  MrcParameters* const MrcData,
  IN  UINT32  Controller,
  IN  UINT32  Channel,
  IN  UINT32  Dimm,
  OUT BOOLEAN *BrcSupported
  );

/**
  This function returns TRUE if Lpddr5 supports DFE
  Read MR24 OP[7]. DFE is only supported if all Controller/Channel/Rank support DFE
  Read MR24 OP[2:0] and MR24[6:4]. DFE quantity 0 implies DFE is Disabled. Should be observed after DFE training

  @param[in] MrcData          - Pointer to global MRC data.
  @param[in] IsDFESupported   - TRUE if DFE is supported on all DIMMs
  @param[in] IsDFEQuantitySet - TRUE if DFE Quantity is non 0 - signalling that DFE is supported
**/
VOID
IsLpddr5DFESupported (
  IN  MrcParameters  *MrcData,
  OUT BOOLEAN        *IsDFESupported,
  OUT BOOLEAN        *IsDFEEnabled
  );

/**
  This function performs the reset sequence required by JEDEC spec for LPDDR5.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcJedecResetLpddr (
  IN  MrcParameters* const  MrcData
  );

/**
  This function will encode the pointer being passed in to the correct
  MR value based on input parameters and LPDDR5 MR30 encoding requirments
  If MRC_IGNORE_ARG_16_2 is passed in, that parameter is ignored.

  @param[in]      MrcData - Pointer to global MRC data.
  @param[in]      DcaLower - 2s complement value to program to Lower Byte.
  @param[in]      DcaUpper - 2s complement value to program to Upper Byte.
  @param[in,out]  MrData  - Pointer to MR data to update.

  @retval MrcStatus - mrcSuccess if a supported MR value, else mrcWrongInputParameter.
**/
MrcStatus
MrcLpddr5SetMr30(
  IN      MrcParameters *const  MrcData,
  IN      UINT16                DcaLower, OPTIONAL
  IN      UINT16                DcaUpper, OPTIONAL
  IN OUT  UINT8         *const  MrData
);

/**
  If Enable = TRUE:  Issue CAS WS_FS on all channels / ranks, and enable WCK always-on mode in DUNIT
  If Enable = FALSE: Issue CAS WS_OFF on all channels / ranks, and restore original WCK mode in DUNIT

  @param[in]      MrcData - Include all MRC global data.
  @param[in,out]  WckMode - Wck mode value to save/restore.
  @param[in]      Enable  - Fast sync on/off and WCK mode save/restore.
**/
VOID
MrcIssueLp5FastSync (
  IN     MrcParameters *const MrcData,
  IN OUT UINT8         *const WckMode,
  IN     BOOLEAN              Enable
  );

/**
  If WCK sync is required (WCK Always On mode and after WCK leveling):
   - Issue CAS WS_OFF on all channels / ranks, followed by CAS WS_FS

  @param[in] MrcData    - Pointer to MRC global data.
**/
VOID
MrcLp5WckOffAndSync (
  IN MrcParameters *const MrcData
  );

/**
  Return the RDQS Postamble Length encoding for MR10

  @return RDQS Postamble Length encoding for MR10
**/
LPDDR5_MR10_RDQS_PST_LENGTH
MrcLp5GetRdqsPostambleLength (
  VOID
  );

/**
  Program CS ODT OP in MR11 if CS ODT is supported.

  @param[in] MrcData   - The global host structure

  @return    MrcStatus - mrcSuccess.
**/
MrcStatus
MrcMr11WriteCsOdtOp (
  IN MrcParameters* const MrcData
);

/**
  Check if conditions are met to optimize MC tXP value.

  @param[in]  MrcData       - The global host structure
  @param[out] IfOptimizeTxp - True if CS ODT behavior option is enabled (MR OP[7] = 1)

  @return MrcStatus.
**/
MrcStatus
MrcIfOptimizeTxp (
  IN  MrcParameters* const MrcData,
  OUT BOOLEAN              *IfOptimizeTxp
);


/**
  This function returns the RD Pre-amble Length MR10 encoded value.

  @param[in]  MrcData   - Pointer to global MRC data.
  @param[out] EncVal    - Encoded Mode Register value.

  @retval MrcStatus - mrcSuccess if the timing is supported.  Else mrcWrongInputParameter.
**/
MrcStatus
MrcLp5GetReadPreambleSetting (
  IN  MrcParameters* MrcData,
  OUT LPDDR5_MR10_tRPRE* EncVal
  );

/**
  Update the Read Preamble type in MR10 and send this to DRAM.
  Update MR10 in the host struct as well

  @param[in]  MrcData -  Pointer to MRC global data.

  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcSetLpddr5Mr10ReadPreamble (
  IN  MrcParameters *const MrcData
  );

/**
  This function compiles the DRAM command needed to preform the specifc MR Cmd requested,
  with the given Data and address information

  @param[in] MrcData              - Include all MRC global data.
  @param[in] Address              - MRR / MRW address
  @param[in] Data                 - MRW Data or MPC Opcode
  @param[in] MRCmd                - Cmd Type: MRR / MRW / MPC / NOP
  @param[in] IsMulticast          - True if Dram cmd is to be written as multicast
  @param[out] DramCmdData         - Compiled Dram command value
  @param[in out] IsSingleCycleCmd - True if MPC sended as a single cycle command

  @retval mrcSuccess Valid Dram Command composed successfully
  @retval mrcFail MrCmd not supported
**/
MrcStatus
MrcLpddr5GetDramCommandMap (
  IN  MrcParameters* const  MrcData,
  IN  UINT32                Address,
  IN  UINT32                Data,
  IN  MrDramCmdType         MrCmd,
  IN  BOOLEAN               IsMultiCast,
  OUT UINT32                *DramCmdData,
  IN OUT BOOLEAN            *IsSingleCycleCmd
  );

/**
  Enable / Disable DRAM Continuous DQS Toggle mode for LPDDR5

  @param[in, out] MrcData - Include all MRC global data
  @param[in]      Enable  - Start / Stop the mode
  @param[in]      Rank    - Target rank

  @retval mrcSuccess if MR configuration is succesfull otherwise mrcFail.
**/
MrcStatus
StartDramDqsToggleModeLpddr5 (
  IN OUT MrcParameters * const  MrcData,
  IN     BOOLEAN                Enable,
  IN     UINT32                 Rank
  );

/**
  This function returns the tRRFpb in ps.

  @param[in] DeviceCapacity - DRAM Device Capacity in MB

  @retval tRRFpb - tRRFpb timing based on Device Capacity
**/
UINT32
MrcGetLpddr5Trrfpb (
  IN UINT32 DeviceCapacity
  );

/**
  Check if LPDDR5x is supported via MR8.
  Then check if E-DVFSC mode is supported - Read MR41 - bits[2:1]
  MR41 - bits[2:1] (Valid only in case MR8 reports LP5x):
    00 - Only DVFSC mode supported
    01 - Only E-DVFSC (Enhanced DVFSC) supported
    10 - DVFSC and E-DVFSC modes supported

  @param[in] MrcData - include all the MRC general data.

  @retval mrcSuccess if E-DVFSC is supported
  @retval mrcColdBootRequired if E-DVFSC not supported
**/
MrcStatus
MrcIsDramEdvfscSupported (
  IN MrcParameters *const MrcData
  );

/**
  Read LP5 Frequency capability from MR8

  @param[in] MrcData - include all the MRC general data.

  @retval mrcDramSafeSpeedUpdate if DRAM frequency capability is higher than the current safe speed
  @retval mrcSuccess             Otherwise
**/
MrcStatus
MrcCheckLp5Frequency (
  IN MrcParameters *const MrcData
  );

/**
  Get tFC delay which depends on DDR data rate.

  @param[in] Frequency DDR data rate.

  @returns tFC in ns
**/
UINT32
MrcGetLpddr5Tfc (
  MrcFrequency Frequency
);

/**
  This function returns the tPRPDEN value for the specified Memory type.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] DdrFreq  - The memory frequency.

  @return The tPRPDEN value for the specified configuration.
**/
UINT32
MrcGetTprpden (
  IN       MrcParameters *const MrcData,
  IN const MrcFrequency  DdrFreq
  );

/**
  This function returns the tBPR2ACT value for the current Memory type.

  @param[in] MrcData  - Include all MRC global data.

  @retval The tBPR2ACT value for the specified configuration.
**/
UINT32
MrcGetTbpr2act (
  IN MrcParameters *const MrcData
  );

/**
  This function calculates the LPDDR5 Read Drift

  @param[in]  MrcData - Include all MRC global data.

  @retval DramReadDriftPI - Read Drift in Pi ticks
**/
UINT32
GetLpddr5ReadDrift (
  IN  MrcParameters* const MrcData
  );

/**
  This function returns tCCD_L for LPDDR.

  @param[in]  MrcData - Pointer to MRC global data.

  @returns nWR_diff parameter.
**/
UINT32
GetLpddrtCCDL (
  IN MrcParameters *const MrcData
  );

/**
  This function returns tCCD_S for LPDDR.

  @param[in]  MrcData - Pointer to MRC global data.

  @returns nWR_diff parameter.
**/
UINT32
GetLpddrtCCDS (
  IN MrcParameters *const MrcData
  );

/**
  This function calculates DRAM temp/voltage drift

  @param[in]  MrcData - Pointer to MRC global data.

  @retval UINT32 DramWriteDrift in pS
**/
UINT32
MrcGetDramWriteDrift (
  IN     MrcParameters *const MrcData
  );

/**
  Calculate the tCWL value for LPDDR5.

  JEDEC Spec x8/x16 WL values:
    Lower Clk   Upper Clk      SetA   SetB
    Freq Limit  Freq Limit     WL     WL
    --------------------------------------
    10            67           2      2
    67            133          2      3
    133           200          3      4
    200           267          4      5
    267           344          4      7
    344           400          5      8
    400           467          6      9
    467           533          6      11
    533           600          7      12
    600           688          8      14
    688           750          9      15
    750           800          9      16

  @param[in] tCK   - The memory DCLK in femtoseconds.
  @param[in] WlSet - 0: Set A, 1: Set B

@retval LpDDR5 tCWL Value
**/
UINT32
GetLpddr5tCWL (
  IN UINT32 tCKmin,
  IN UINT8  WlSet
  );

/**
  Return the initial LPDDR5 DQ Vref (MR14/MR15)

  @param[in]  MrcData      - Pointer to global MRC data

  @retval Initial DQ Vref in [mV]
**/
UINT16
MrcLp5GetVrefDq (
  IN  MrcParameters *const  MrcData
  );

///
/// Function Declarations
///

/**
  Calculate the tCL value for LPDDR5.
  JEDEC Spec Table 220 - Read Latencies for Read Link ECC off case (DVFSC disabled)

    Lower Clk   Upper Clk        Read Latency
    Freq Limit  Freq Limit     Set0  Set1  Set2
    -------------------------------------------
      5            67           3      3      3
     67           133           4      4      4
    133           200           5      5      6
    200           267           6      7      7
    267           344           8      8      9
    344           400           9     10     10
    400           467          10     11     12
    467           533          12     13     14
    533           600          13     14     15
    600           688          15     16     17
    688           750          16     17     19
    750           800          17     18     20
    800           937.5        20     22     24
    937.5        1066.5        23     25     26

    Set0 - x16, No DBI
    Set1 - x8 and No DBI, or x16 and DBI
    Set2 - x8 and DBI

    @param[in] tCK          - The memory tCK in femtoseconds.
    @param[in] SdramWidth   - SDRAM width (8 or 16)
    @param[in] IsDbiEnabled - TRUE if DBI is enabled
    @param[in] IsDvfscEnabled - TRUE if Dvfsc is enabled

    @retval LPDDR5 tCL in tCK units
**/
UINT32
GetLpddr5tCL (
  IN const UINT32     tCK,
  IN UINT8            SdramWidth,
  IN BOOLEAN          IsDbiEnabled,
  IN BOOLEAN          IsDvfscEnabled
  );

/**
  This function converts from the integer defined Read Latency to the Mode Register
  encoding of the timing in LPDDR5.

  @param[in]  MrcData - Pointer to global MRC data.
  @param[in]  Value   - Requested Read Latency value.
  @param[out] EncVal  - Encoded Mode Register value.

  @retval MrcStatus - mrcSuccess if the latency is supported.  Else mrcWrongInputParameter.
**/
MrcStatus
EncodeReadLatencyLpddr5 (
  IN  MrcParameters *MrcData,
  IN  UINT16        Value,
  OUT UINT8         *EncVal
  );

/**
  This function converts from 2s complement value to LPDDR5 MR DCA encoding (MR30 for LPDDR5).

  @param[in]  DcaValue - 2s complement value.

  @retval UINT8 - Encoding if valid DCA value.  Else, 0xFF.
**/
UINT8
Lpddr5DcaEncode (
  IN  INT16  DcaValue
);
#endif // _MRC_LPDDR5_H_
