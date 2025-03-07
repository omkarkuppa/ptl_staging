/** @file
  Contains functions that are used outside of the DdrIo Library.

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
#ifndef MRC_DDR_IO_API_H_
#define MRC_DDR_IO_API_H_

#include "MrcDebugPrint.h"
#include "MrcDdrIoDefines.h"
#include "MrcDdrIoUtils.h"
#include "MrcRegisterStruct.h"
#include "MrcCommon.h"
#include "MrcTurnAround.h"
#include "MrcDdrIoComp.h"

extern const char *GlobalCompOffsetStr[];

/// Defines
#define PI_TO_QCLK_G2    (128)
#define PI_TO_QCLK_G4    (256)
#define PI_PER_TCK       (128)
#define PI_PER_TCK_LP5   (512)
#define MRC_UI_IN_PI     (64)
#define MRC_2UI_IN_PI    (MRC_UI_IN_PI * 2)

#define FREQ_TO_TCK_PS   (2000000)

/// PI DCC LUT Defines
#define FINE_PI_INDEX_MAX         (8)
#define MAX_DCC_TX_DQS_DQ_PI_VAL  (16)      ///< Tx Dqs/Dq PI Subset Range (0 - 16) Only used for DCC Look Up Table Val Procedure

/// DCC Parameter Mask
#define DCC_DQ_MASK            (MRC_BIT0)
#define DCC_DQS_MASK           (MRC_BIT1)
#define DCC_CLK_MASK           (MRC_BIT2)
#define DCC_WCK_MASK           (MRC_BIT3)
#define DCC_DATASH_MASK        (MRC_BIT4)
#define DCC_CCCSH_MASK         (MRC_BIT5)
#define DCC_COMP_MASK          (MRC_BIT6)
#define DCC_CLK                     (0)
#define DCC_WCK                     (1)
#define DCC_RISE_FALL_WCK_BL        (0)
#define DCC_RISE_FALL_WCK_MASK      (7)
#define DCC_RISE_FALL_WCK_PULSE     (1)
#define DCC_RISE_FALL_WCK_EN        (1)
#define DCC_LUT_DATA_SAMPLES        (3)
#define DCC_CCC_LANE_EN_CLK         (0x4)
#define DCC_CCC_LANE_EN_WCK         (0x3)
#define DCC_CCC_LANE_EN_ALL         (0xC)

#define REFCLK_DCC_MAX_CORRECTION_LOOPS    (10)
#define REFCLK_DCC_DELTA_LIMIT             (4)
#define REFCLK_TLINE_DCC_REGION_NUM        (2)

// The offset between each CCC channel instance is constant across all CCC Registers.
#define CCC_CH_OFFSET (CH1CCC_CR_DDRCRCACOMP_REG - CH0CCC_CR_DDRCRCACOMP_REG)       // 0x100

// RxDQS DCC Calibration
#define  DCC_CORRECTION_LIMIT_DQS_VOC                        16         // max number of corrections allowed for DCC loop
#define  PICODE_VCDL_START                                   0          // sets the starting PI code for the post-SDL DCC. This specifies the beginning of the delay line to be selected by the RxPI
#define  RXDQS_VOC_CODE_START                                16         // when presdl DCC is selected, this is the starting dcc code
#define  DCC_RXDQS_TARGET                                    50         // Duty Cycle Target in duty cycle percentage
#define  DCC_RXDQS_NUM_SAMPLES_ON_ALL_LANES                  (16 * 512) // 16 lanes * 512
#define  DCC_RXDQS_SENSE_AT_RXDLL_NUM_SAMPLES_ON_ALL_LANES   (4 * 512)  // 4 lanes * 512

#define  DCC_SAMPLE                    5   // Max out samples for best result

#define  RXDQS_VOC_RANGE_MAX           29
#define  RXDQS_VOC_RANGE_MIN           0

// Bit Masks for MTS_MAS_100 Table 1 - Voltage Sensor Assignment (MP) (S)
// Setting the mask-bit will enable Voltage Sensor Assignment
// Bit_0 -> PartitionX_0 .. Bit_N -> PartitionX_N
#define SEL_LVR_VCCIOG_DT_PG_MASK     0xD01
#define SEL_LVR_VCCIOG_PG_MASK        0x83
#define SEL_LVR_VCCDDQ_DT_PG_MASK     0x2A8
#define SEL_LVR_VCCDDQ_PG_MASK        0x50
#define SEL_LVR_VCCCLK_DT_PG_MASK     0x1050
#define SEL_LVR_VCCCLK_PG_MASK        0x24
#define SEL_LVR_VCCDLL_DT_PG_MASK     0x06
#define SEL_LVR_VCCDLL_PG_MASK        0x08
#define SEL_LVR_COMP_MASK             0x01

// Bit Masks for MTS_MAS_100 Table 2 - Current Sensor Assignment (MP) (S)
// Setting the mask-bit will enable Current Sensor Assignment
// Bit_0 -> PartitionX_0 .. Bit_N -> PartitionX_N
#define CUR_SENS_SEL_LVR_VCCIOG_DATA_MASK       0x41
#define CUR_SENS_SEL_LVR_VCCCLK_DT_DATA_MASK    0x22
#define CUR_SENS_SEL_LVR_NBIASFF_DT_DATA_MASK   0x208
#define CUR_SENS_SEL_LVR_NBIASFF_DATA_MASK      0x84
#define CUR_SENS_SEL_LVR_VCCDLL_DT_DATA_MASK    0x180
#define CUR_SENS_SEL_LVR_VCCDLL_DATA_MASK       0x18
#define CUR_SENS_SEL_LVR_VCCDDQ_DT_DATA_MASK    0x14
#define CUR_SENS_SEL_LVR_VCCDDQ_DATA_MASK       0x22
#define CUR_SENS_SEL_LVR_VCCIOG_DT_CCC_MASK     0x55
#define CUR_SENS_SEL_LVR_VCCIOG_CCC_MASK        0x01
#define CUR_SENS_SEL_LVR_VCCCLK_DT_CCC_MASK     0x00
#define CUR_SENS_SEL_LVR_VCCCLK_CCC_MASK        0x0A
#define CUR_SENS_SEL_LVR_VCCDLL_CCC_MASK        0x00
#define CUR_SENS_SEL_LVR_VCCDDQ_DT_CCC_MASK     0xAA
#define CUR_SENS_SEL_LVR_VCCDDQ_CCC_MASK        0x04

// Bit Masks for LNL (M) LP5 - LVR Auto Trim Assignment
// Setting the mask-bit will enable Current Sensor Assignment
// Bit_0 -> PartitionX_0 .. Bit_N -> PartitionX_N
#define CUR_SENS_SEL_LVR_PARTITION_PG_MASK      0x102
#define CUR_SENS_SEL_LVR_DATA_SB_MASK           0x42

#define CPGC_RMT_CL_NUMBER  65536 // 2 ^ 16

#define RXTX_CONTROLS_SREP_CNT_0   7

#define MAX_LVR_VCM      520 // Max allowed input common mode in mV for LVR analog feedback loop
#define MIN_LVR_VCM      320 // Min allowed input common mode in mV for LVR analog feedback loop

//Maps datashared partition to the 2 channels in that partition
static const INT8 Data2ChLp5[MRC_DATA_DT_NUM][2] = { {0,1}, {0,1}, {2,3}, {2,3}, {4,5}, {4,5}, {6,7}, {6,7}, {-1,-1}, {-1,-1} };
static const INT8 Data2ChDdr5NIL[MRC_DATA_DT_NUM][2] = { {0,0}, {0,0}, {1,1}, {1,1}, {2,2}, {2,2}, {3,3}, {3,3}, {-1,-1}, {-1,-1} };
static const INT8 Data2ChDdr5ILEcc[MRC_DATA_DT_NUM][2] = { {0,1}, {0,1}, {0,1}, {0,1}, {2,3}, {2,3}, {2,3}, {2,3}, {0, 1}, {2, 3} };
static const INT8 Data2ChDdr5IL[MRC_DATA_DT_NUM][2] = { {0,1}, {0,1}, {0,1}, {0,1}, {2,3}, {2,3}, {2,3}, {2,3}, {-1,-1}, {-1,-1} };

//DCC DLL Constants
#define DCC_DLL_CODE_1        (8)
#define DCC_DLL_CODE_2        (7)
#define DCC_DLL_CALC_LOOPS    (2)

// WakAMole constants
#define NUMBER_PHASES (4)

// Default Delta Picodes to use in initial TAT equations
#define DEFAULT_DELTA_TXDQS_PICODE (16)
#define DEFAULT_DELTA_RCVEN_PICODE (16)

/// Structs
typedef enum {
  PhdDrvPi,
  PiData,
  PiRead,
  RxDrv,
  PhDrvG,
  PiCcc
} FFCODE_LEGS_TYPE;

typedef enum {
  VccIo,
  VccClk
} VF_CURVE_TYPE;

typedef enum {
  VCCCLKRX0,
  VCCCLKRX1,
  VCCCLKTX,
  NBIASFF,
  VCCIOG,
  VCCPLL,
  VCCDIST,
  VCCCLKQ,
  VCCMEMG,
  VCCDD2G,
  VCCDDQG,
  MaxRail
} RAIL_TYPE;

typedef enum {
  DataRailPartion,
  CccRailPartion,
  CompRailPartion,
  MaxRailPartion
} RAIL_PARTTION;


#define DATA_PART_MASK  (MRC_BIT0 << DataRailPartion) // data bit0
#define CCC_PART_MASK   (MRC_BIT0 << CccRailPartion)  // ccc bit1
#define COMP_PART_MASK  (MRC_BIT0 << CompRailPartion) // comp bit2

//Mapping Table: Maps each voltage rail to a list containing the [+/- AC Tolerance (% of Target), +/- DC Tolerance (mV)]
//hese tolerance numbers are combined with the target voltage and used by runLvrRMT to check if voltage is within spec.
//All are x1000 to avoid using float number.
                                            // VCCCLKRX0    VCCCLKRX1    VCCCLKTX     NBIASFF     VCCIOG        VCCPLL     VCCDIST     VCCCLKQ      VCCMEMG      VCCDD2G      VCCDDQG
static const UINT32 RailTol[MaxRail][2] =     {{15,  5000}, {15,  5000}, {15, 5000}, {10, 5000}, {30,  8000}, { 5, 5000}, {10, 5000}, {20, 15000}, {50, 50000}, {50, 50000}, {50, 50000}};
static const UINT8 SelVSense[MaxRail] =       { 1,            2,          3,           5,          4,            2,         1,          3,           6,          5,           7};
static const UINT8 SelLvrAutoTrim[MaxRail] =  { 0,            1,          2,           4,          3,            1,         0,          2,           0,          0,           0};
static const UINT8 RailPartsMask[MaxRail] =   { DATA_PART_MASK,                                   // VCCCLKRX0
                                                DATA_PART_MASK,                                   // VCCCLKRX1
                                                DATA_PART_MASK | CCC_PART_MASK,                   // VCCCLKTX
                                                DATA_PART_MASK,                                   // NBIASFF
                                                DATA_PART_MASK | CCC_PART_MASK | COMP_PART_MASK,  // VCCIOG
                                                                                 COMP_PART_MASK,  // VCCPLL
                                                                                 COMP_PART_MASK,  // VCCDIST
                                                                                 COMP_PART_MASK,  // VCCCLKQ
                                                DATA_PART_MASK | CCC_PART_MASK | COMP_PART_MASK,  // VCCMEMG
                                                                 CCC_PART_MASK | COMP_PART_MASK,  // VCCDD2G
                                                DATA_PART_MASK | CCC_PART_MASK | COMP_PART_MASK}; // VCCDDQG

#ifdef MRC_DEBUG_PRINT
static const CHAR8* const RailStr[MaxRail] = {"VCCCLKRX0", "VCCCLKRX1", "VCCCLKTX", "NBIASFF", "VCCIOG", "VCCPLL", "VCCDIST", "VCCCLKQ", "VCCMEMG", "VCCDD2G", "VCCDDQG"};
static const CHAR8* const PartStr[MaxRailPartion] = {"Data", "Ccc", "Comp"};
#endif

static const UINT8 PartInstanceNum[MaxRailPartion] = {MRC_DATA_SHARED_NUM , MRC_CCC_SHARED_NUM , MRC_COMP_NUM};

typedef struct {
  INT64 RxPiPwrDnDis;
  INT64 ForceRxAmpOn;
  INT64 BiasPMCtrl;
  INT64 InternalClocksOn;
  INT64 ForceRcvEnOn;
  INT64 RptChRepClkOn;
  INT64 CompClkOn;
  INT64 DccClkGateDisable;
  INT64 DqsIdleModeDrvEn;
  INT64 ForceOdtOn;
  INT64 DqOdtParkMode;
  INT64 ForceRxOnDqsMux;
  INT64 ForceRxOnDq;
} MRC_RX_DQS_DCC_SAVE;

typedef enum {
  DccLow          = 0,                  ///< DCC Low
  DccHigh         = 1,                  ///< DCC High
  DccStatusAlign  = 2,                  ///< DCC Align
  DccStatusDither = 3,                  ///< DCC Dither
  DccInit         = 4,                  ///< DCC Init
} DCC_STATUS;

typedef enum {
  RefClkDccType,
  TlineDccType
} REFCLK_DCC_TYPE;

typedef enum {
  RefclkSaturationFalse = 0x0,
  RefclkSaturationLow   = 0x1,
  RefclkSaturationHigh  = 0x2,
  RefclkSaturationBoth  = 0x3,
  RefclkSaturationMax
} REFCLK_SATURATION_TYPE;

typedef enum {
  MrcLp5tWckDqoRound,
  MrcLp5tWckDqoFloor,
  MrcLp5tWckDqoRemainder
} MRC_TWCKDQO_TYPE;

typedef enum {
  SaveValue,
  UpdateValue,
  RestoreValue
} UPDATE_MODE;

typedef struct {
  UINT32 RefPiDqsWckOffset[MRC_DATA_SHARED_NUM];
  UINT32 CccPiCodes[MRC_DATA_SHARED_NUM];
  UINT32 RefPiControl[MRC_DATA_MOBILE_NUM][MRC_DATA_CH_NUM];
  UINT32 TxControl0Rank0[MRC_DATA_MOBILE_NUM][MRC_DATA_CH_NUM];
  UINT32 TxControl0Rank1[MRC_DATA_MOBILE_NUM][MRC_DATA_CH_NUM];
} MRC_DCC_PI_SRZ_SAVE_CONFIG;

#define MRC_WAM_PARTITIONS        (2)
#define MRC_DCC_PARTITIONS        (1)
#define DDRIO_CCC_CLK_LANE        (5)
#define DDRIO_CCC_LANES_34        (3)
#define DDRIO_CCC_LANE_5          (5)
#define DDRIO_CCC_LANES_67        (6)
#define DDRIO_CCC_LANES_89        (8)
#define HALF_DUTY_CYCLE           (256)

#define MRC_MAX_RESET_RECOVERY_ATTEMPTS  (10)
#define MRC_DCC_STEP_SIZE_FORMULA        (15) //For use of +15 - -15 in determining DCC Step Size
#define MRC_NTC_DCC_STEP_SIZE_FORMULA    (16) //For use of +16 - -16 in determining NTC DCC Step Size

/// Segment numbers for comp vrefs
#define TWO_SEGMENTS (2)

#define MRC_UY_LP5_CCC_TxEn_MSK      (0x03FF)
#define MRC_UY_DDR5_CCC_CLK_MSK      (0x0020)
#define PANICV0                      (25)   // mV
#define PANICV1                      (40)   // mV
#define VCCANA_EH                    (1800) // mV
#define MRC_VCCDLL_TARGET            (850)  // mV
#define MRC_VCCBG                    (1000) // mV
#define CA_VOLT_SEL                  (0)
#define CLK_VOLT_SEL                 (0)

// Voltage and Current Sensor point test cases
#define VOLTAGE_SENS_TEST_MAX (2)
#define CURRENT_SENS_TEST_MAX (3)

// Static Legs
#define STATIC_LEGS  (2)
#define STATIC_LEGS_WRTTX (12)

// Dynamic Legs
#define DYNAMIC_LEGS_MSB (32)

// Max Comp value
#define MAX_COMP_VALUE  (63)

// Half UI
#define MRC_0p5UI_IN_PI  (32)

typedef enum {
  SelLvrVccIog,
  SelLvrVccDdq,
  SelLvrVccClk,
  SelLvrVccDll, // @todo - LNL[1309699097]: need to confirm with DE team the VTarget for VccDll
  SelLvrVccMax
} SelLvrSetting;

#define SAL_USE_PMOS_MV  (400)

/// Values for Data ODT modes as per the MAS. Max doesn't represent a specified odt mode.
typedef enum {
  DdrIoOdtModeVssRegVal,
  DdrIoOdtModeVddqRegVal,
  DdrIoOdtModeMaxRegVal
} DdrIoOdtModeRegVal;

typedef enum {
  DCC_PHCLK,
  DCC_PHCLK_BUFF,
  DCC_REFCLK,
  DCC_VCDL
} DCC_COMPONENT;

#define BW_SEL_ENABLE         (TRUE)
#define BW_SEL_DISABLE        (FALSE)
#define MRC_NO_VALID_BWSEL    (0xFF)

// DFI Control
#define MRC_DFI_CONTROL_MC    (0)
#define MRC_DFI_CONTROL_MPTU  (1)

// Data PI Linearity
#define MRC_PI_MIXER_NUM       (25)
#define MRC_MIXER_SET_NUM      (2)
#define MRC_PI_PHASE_MIDDLE    (32)
#define MRC_PI_PHASE_QUARTER   (16)
#define MRC_PILIN_IDEAL_STEP   (16)
#define MRC_DATA_PI_LIN_OFFSET (12)
#define MRC_PILIN_NUM_LANES    (8)

#define FIRST_TWO_RANKS_MASK   (0x3)

#define MAX_COARSE             (15)

#define LVR_SENSOR_PARTITIONS (3)
#define PG_PARTITION (0)
#define COMP_PARTITION (2)
#define PG_MASK (0)
#define PG1 (MRC_BIT1)
#define COMP_MASK (2)
#define DATA_MASK (0)
#define CCC_MASK (1)
#define LVR_COUNTERS (2)
#define COUNTER_0 (0)
#define COUNTER_1 (1)
#define COUNTER_PASS_LIMIT (2027)
#define FB_PARTITION_MAX (5)
#define FF_PARTITION_MAX (3)
#define FF_CODE_MAX      (6)
#define WREF_LEGS_NUM    (5)
#define COEFF_DURATION   (14)
#define COEFF_NUM        (3)
#define PI_CCC           (2)
#define PICLKDURATION_MIN (4)

// Data FF Count types
#define DATA_FFPH_DRV_PI  (0)
#define DATA_FFPI_DATA    (1)
#define DATA_FFPI_READ    (2)
#define DATA_FFRX_DRV     (3)
#define DATA_FFPH_DRV_G   (4)

// CCC FF Count types
#define CCC_FFPH_DRV_PI   (0)
#define CCC_FFPI_CCC      (1)
#define CCC_FFPH_DRV_G    (2)

#ifdef MRC_DEBUG_PRINT
#define MRC_PRINT_DDR_IO_GROUP_RCV_EN(MrcData, Socket, Controller, Channel, Rank, Strobe, Bit, FreqIndex, Group)\
  MrcPrintDdrIoGroupRcvEn (MrcData, Socket, Controller, Channel, Rank, Strobe, Bit, FreqIndex, Group )
#else
#define MRC_PRINT_DDR_IO_GROUP_RCV_EN( MrcData, Socket, Controller, Channel, Rank, Strobe, Bit, FreqIndex, Group)
#endif

// MrcDdrioPiLinearity, PI DCC struct
typedef struct {
  UINT16 TxDq[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM];
  UINT16 TxDqs[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM];
  UINT16 TxDqOffset[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM];
  UINT16 Deltapicoder0[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM];
  UINT16 SkipCrWrite[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM];
  UINT16 SaveFullDcc[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM];
  UINT16 EnChop[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM];
  UINT16 Samples[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM];
  UINT16 LaneEn[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM];
  UINT16 RankMap[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM];
  UINT16 RankEn[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM];
  UINT16 MeasPoint[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM];
  UINT16 RefPi4X[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM];
  UINT16 RefPi4XOffset[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM];
  UINT16 TxDqBit[MAX_CONTROLLER][MAX_CHANNEL][MAX_SDRAM_IN_DIMM][MAX_BITS];
  INT64  TxDqDelaySave;
  INT64  GetSetDccSamples;
} MRC_DATA_PI_LIN_SAVE;

/// Functions

MrcStatus
MrcPiLinearitySaveRestoreData(
  IN      MrcParameters* const  MrcData,
  IN OUT  MRC_DATA_PI_LIN_SAVE* SaveData,
  IN      MrcSaveOrRestore      SaveOrRestore
);

VOID
MrcPiLinearityTrainingInitData(
  IN  MrcParameters* const MrcData,
  IN  BOOLEAN              RunPiDCC
);

MrcStatus
MrcPiDCCInitAndSaveData(
  IN      MrcParameters* const  MrcData,
  IN OUT  MRC_DATA_PI_LIN_SAVE* SaveData
);

MrcStatus
MrcPiDCCRestoreSaveData(
  IN      MrcParameters* const  MrcData,
  IN OUT  MRC_DATA_PI_LIN_SAVE* SaveData
);

#ifdef MRC_DEBUG_PRINT
/**
  This function outputs the GSM group values for RcvEn to the debug print device.

  @param[in] MrcData   - Pointer to global data.
  @param[in] Group     - DDRIO group to access.
  @param[in] Socket    - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in] Controller- DDR Memory Controller Number within the processor socket (0-based).
  @param[in] Channel   - DDR Channel Number within the memory controller (0-based).
  @param[in] Rank      - Rank number within a channel (0-based).
  @param[in] Strobe    - Dqs data group within the rank (0-based).
  @param[in] Bit       - Bit index within the data group (0-based).
  @param[in] FreqIndex - Index supporting multiple operating frequencies. (Not used in Client CPU's)

  @returns MrcStatus value reflecting any errors
**/
extern
MrcStatus
MrcPrintDdrIoGroupRcvEn (
  IN MrcParameters *const  MrcData,
  IN UINT32  const Socket,
  IN UINT32  const Controller,
  IN UINT32  const Channel,
  IN UINT32  const Rank,
  IN UINT32  const Strobe,
  IN UINT32  const Bit,
  IN UINT32  const FreqIndex,
  IN GSM_GT  const Group
  );
#endif

/*
  Setup LVR Registers

  @param[in]  MrcData       - Global MRC data structure.
*/
VOID
WriteAllLvrTarget (
  IN MrcParameters* const MrcData
  );

/*
  Write LVR Target

  @param[in]  MrcData       - Global MRC data structure.
  @param[in]  Rail          - Voltage Rail Type

  @retval mrcSuccess - Always returned as this step is non-blocking
*/
MrcStatus
WriteLvrTarget (
  IN MrcParameters* const MrcData,
  IN RAIL_TYPE            Rail
  );

/*
  Calculates the target voltage based on DataRate and Fuse Limits
  Returns the target voltage in mV

  @param[in]   MrcData       - Global MRC data structure.
  @param[in]   Type          - Vccclk or VccIo

  @retval TargetVoltage
*/
UINT32
CalcVFCurve (
  IN MrcParameters *const MrcData,
  VF_CURVE_TYPE           Type
  );

/*
  Calculates the encoded target voltage and divider ratio to program into the LVR Voltage Sensor
  Returns both the Target and VinDivider voltages.

  @param[in]   MrcData       - Global MRC data structure.
  @param[in]   Rail          - Voltage Rail Type
  @param[in]   Target        - Target Voltage
  @param[out]  Vid           - Voltage
  @param[out]  FbDivider     - Divider Ratio
*/
VOID
CalcLvrTarget (
  IN  MrcParameters *const MrcData,
  IN  RAIL_TYPE            Rail,
  IN  UINT32               Target,
  OUT UINT32*              Vid, OPTIONAL
  OUT UINT32*              FbDivider
  );

/**
  This function performs Bw DLL Calibration

  The bwsel comp scheme calibrates to the optimal bwsel setting by sweeping the bwsel code
  and monitoring the VccDLL after the DLL locks and relocks. The bwsel code starts at its max
  setting then decremented until the DLL pbias voltage falls within the specified range.

  @param[in] MrcData - Include all MRC global data.

  @returns MrcStatus value reflecting any errors
**/
MrcStatus
MrcBwSelCal (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function restores BWSel Calibration results

  @param[in] MrcData  - Include all MRC global data.
**/
VOID
MrcBwSelUpdate (
  IN MrcParameters *const MrcData
  );

/**
  Program MCMISCS_DDRWCKCONTROL

  @param[in] MrcData  - Include all MRC global data
**/
VOID
MrcSetWckControl (
  IN MrcParameters *const MrcData
  );

/**
  Program MCMISCS_RXDQFIFORDENCH[01..67], MCMISCS_WRITECFGCH[0..7] and MCMISCS_READCFGCH[0..7] registers.

  @param[in]  MrcData         - Include all MRC global data
**/
VOID
MrcSetReadWriteConfig (
  IN MrcParameters *const MrcData
  );

/**
  CPU specific function which encodes Vref (mV) for the GSM_GT RxVref.

  @param[in]  MrcData       - Pointer to global data.
  @param[in]  Vref          - Vref value to encode, in mV.
  @param[out] VrefEncoding  - Register value of the Vref to program.
**/
VOID
MrcEncodeRxVref (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Vref,
  OUT INT64                 *VrefEncoded
  );

/**
  CPU specific function which decodes RxVref to Vref

  @param[in]  MrcData       - Pointer to global data.
  @param[in]  RxVref        - RxVref value to Decode
  @param[out] VrefDecoded   - Vref value (mV)
**/
VOID
MrcDecodeRxVref (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                RxVref,
  OUT INT64                 *VrefDecoded
  );

/**
  This function initializes the Memory Controller Phy.

  @param[in, out] MrcData - Include all MRC global data.

  @returns MrcStatus value reflecting any errors
**/
MrcStatus
MrcDdrIoInit (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function calculates PHY values after changes to RcvEn

  @param[in]  MrcData - Pointer to MRC global data.

  @returns MrcStatus value reflecting any errors
**/
MrcStatus
MrcRcvEnFinal (
  IN     MrcParameters* const MrcData
  );

/**
  This function performs final DDRIO configuration after all training steps are done.
  - Enable Weaklock
  - Enable Scrambler
  - Update SampOdtTiming
  - Enable Spine Gate

  @param[in]  MrcData - Pointer to MRC global data.

  @returns MrcStatus value reflecting any errors
**/
MrcStatus
MrcDdrIoFinalize (
  IN     MrcParameters *const MrcData
  );

/**
  This function performs early DDRIO final configuration before Rank Margin Tool.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval MrcStatus - mrcSuccess
**/
MrcStatus
MrcEarlyDdrIoFinalize (
  IN     MrcParameters *const MrcData
  );

/**
  Enable or disable the scrambler.

  @param[in] MrcData - Pointer to the MRC global data area.
  @param[in] EnableScrambler - Enable (1) or disable (0) the scrambler
  @retval MrcStatus

**/
MrcStatus
ScramblerEnable(
  IN MrcParameters* const MrcData,
  IN         INT64        EnableScrambler
  );

/**
  This function performs late DDRIO final configuration after Rank Margin Tool.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval MrcStatus - mrcSuccess
**/
MrcStatus
MrcLateDdrIoFinalize (
  IN     MrcParameters *const MrcData
  );

/**
  This function performs final DDRIO configuration after all training steps are done.
  This is a second part of DDRIO finalize, which is called after DFI control is switched back to MC,
  and MRC is not going to use MPTU anymore.

  @param[in]  MrcData - Pointer to MRC global data.

  @returns MrcStatus value reflecting any errors
**/
MrcStatus
MrcDdrIoFinalizePostDfi (
  IN     MrcParameters *const MrcData
  );

/**
  This function sets the default Rx DFE tap values based on desired DFE tap mV.

  @param[in] MrcData - Pointer to global MRC data.
**/
VOID
MrcSetRxDfeTaps (
  IN MrcParameters* const MrcData
  );

/**
  This function calculates the DFE tap value based on desired DFE tap mV and Tap value

  @param[in] MrcData    - Pointer to global MRC data.
  @param[in] RxDfeTapmV - Desired DFE Tap mV value
  @param[in] TapValue   - Target DFE Tap

  @returns Calculated DFE Offset
**/
INT32
RxDfeTapEquation (
  IN MrcParameters* const MrcData,
  IN INT32                RxDfeTapmV,
  IN UINT8                TapValue
  );

/**
  This function runs Unmatched Receiver Calibration - PreTraining.

  @param[in, out] MrcData - Include all MRC global data.

  @returns MrcStatus value reflecting any errors
**/
MrcStatus
MrcUnmatchRxCompPre (
  IN MrcParameters* const MrcData
  );

/**
  This function executes PI Linearity calibration for DATA (and CCC for DDR5) partitions in the DDRIO.
  This will update MixerCode[1:15] based on the best linearity for PI sweeps once complete.

  @param[in] MrcData - Pointer to MRC global data.

  @retval mrcSuccess - Always returned as this step is non-blocking
**/
MrcStatus
MrcPiLinearityCal (
  IN  MrcParameters *const MrcData
  );

/**
  This function is the entrypoint for PI Look Up table calibration

  @param[in, out] MrcData - Include all MRC global data.

  @return mrcSuccess if PiLUT is successful otherwise mrcFail.
**/
MrcStatus
MrcDccPiCodeLut (
  IN OUT MrcParameters *const MrcData
  );

/**
  VccClkRx FeedForward Leg Calibration
  Run VccCLKRx* FFCode training for all Data Bytes in the PHY.

  @param[in, out] MrcData - Include all MRC global data.

  @retval mrcSuccess if success otherwise a failure code
**/
MrcStatus
MrcVccCLKRxFFCalibration (
  IN OUT MrcParameters *const MrcData
  );

/**
  Run Lvr RMT

  @param[in, out] MrcData - Include all MRC global data.

  @retval mrcSuccess if success otherwise a failure code
**/
MrcStatus
MrcRunLvrRmt (
  IN OUT MrcParameters *const MrcData
  );

/**
  RxDQS DCC calibration

  @param[in, out] MrcData - Include all MRC global data.

  @retval mrcSuccess if DCC converges
  @retval mrcFail otherwise
**/
MrcStatus
MrcRxDqsDcc (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function checks if the current Tline DccCode is saturated after an
  offset (if there is an offset).

  @param[in, out] MrcData - Include all MRC global data.
  @param[in]      DccType - Refclk (4 bits) or Tline (6 bits) -- denotes Min and Max values
  @param[in]      DccCode - Check this for saturation with/without offset.
  @param[in]      Offset  - 0 means saturation check only; non-zero means shift then saturation check.

  @retval - RefclkSaturationFalse - neither high/low saturation detected.
  @retval - RefclkSaturationHigh  - DccCode at highest possible positive value.
  @retval - RefclkSaturationLow   - DccCode at lowest possible negative value.
  @retval - RefclkSaturationMax   - Incorrect DccType passed in.
**/
REFCLK_SATURATION_TYPE
MrcCheckDccCodeSaturate (
  IN OUT MrcParameters *const MrcData,
  IN     REFCLK_DCC_TYPE      DccType,
  IN     INT32                DccCode,
  IN     INT32                Offset
  );

/**
  This function does DQ/DQS/Fall DCC calibration

  @param[in, out] MrcData - Include all MRC global data.

  @retval mrcSuccess if DCC converges otherwise mrcFail.
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcDccRiseFall (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function does WCK/CLK Rise/Fall DCC calibration
  @param[in, out] MrcData - Include all MRC global data.
  @retval mrcSuccess if DCC converges otherwise mrcFail.
**/
MRC_IRAM0_FUNCTION
MrcStatus
MrcDccRiseFallWckClk (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function does sweeping of DQ/DQS Rise/Fall DCC calibration

  @param[in, out] MrcData - Include all MRC global data.
  @param[out] UnexpectedResultOut - Optional return.

  @retval mrcSuccess if DCC converges
  @retval mrcFail otherwise
**/
MrcStatus
MrcDccDqsRiseFall (
  IN OUT MrcParameters *const MrcData,
  OUT BOOLEAN *UnexpectedResultOut  OPTIONAL
  );

/**
  This function sums duty cycle ccc results at the serializer for average calculation, only for populated partitions.

  @param[in] MrcData           - Include all MRC global data.
  @param[in] RankPop           - Array of Rank Population.
  @param[in] LaneEn            - Lanes to be operating on.
  @param[out] DutyCycleSum     - A 1D array of sum results (for each CCC partition).
  @param[out] DccSamples       - Number of Dcc Samples
**/
VOID
MrcGetAveDutyCycleAtSrzCcc (
  IN  MrcParameters* const MrcData,
  IN  UINT8          const RankPop[MAX_CONTROLLER][MAX_CHANNEL][MAX_RANK_IN_CHANNEL],
  IN  UINT32               LaneEn,
  OUT UINT32               DutyCycleSum [MRC_CCC_NUM],
  OUT UINT32               *DccSamples
  );

/**
  This function does calculation for DCC Step Size of CCC

  @param[in, out] MrcData - Include all MRC global data.
  @param[in] RankPop      - Array of Rank Population.
  @param[in] UpdateTco    - TcoCode or DccCode.
  @param[in] MeasPoint    - Value to set the MeasPoint.
  @param[in, out] UnexpectedResultOut - Return of whether result is unexpected.

  @retval mrcSuccess if DCC converges
  @retval mrcFail otherwise
**/
MrcStatus
MrcCalcCccDccStepSize (
  IN OUT MrcParameters *const MrcData,
  IN     UINT8  RankPop[MAX_CONTROLLER][MAX_CHANNEL][MAX_RANK_IN_CHANNEL],
  IN     BOOLEAN UpdateTco,
  IN     UINT8 MeasPoint,
  IN OUT BOOLEAN *UnexpectedResultOut
  );

/**
  This function reads and aggregate the LargeChange fields together

  @param[in, out] MrcData - Include all MRC global data.

  @returns Aggregated LargeChange
**/
UINT8
MrcDccReadLargeChange (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function reads from the StatusLargeChange field for WCK

  @param[in, out] MrcData    - Include all MRC global data.
  @param[in]      Controller - Controller to work on.
  @param[in]      Channel    - Channel to work on.

  @retval StatusLargeChange converted into a BOOLEAN data type
**/
BOOLEAN
MrcDccReadWckStatusLargeChange (
  IN OUT MrcParameters *const MrcData,
  IN     UINT32               Controller,
  IN     UINT32               Channel
  );

/**
  This function calculates the VTarget

  @param[in, out] MrcData - All the MRC global data.
  @param[in]              - Target voltage value (0: view0, 1: view1)
  @param[in]              - Target Rail (0: VccIOG, 1: VccDDQ, 3: VccClk)

  @returns VTarget
**/
UINT32
GetVTarget (
  IN MrcParameters *const MrcData,
  IN UINT32               TargetView,
  IN UINT32               TargetRail
  );

/**
  This function toggles the LargeChangeReset bit for the requested DCC types (DATA/CLK/WCK) with a 10ns wait between setting and clearing
  as well as setting LargeChangeEnable for the requested DCC types.
  Will only target populated partitions (DATA/CLK/WCK) via GetSet multicast.

  @param[in] MrcData     - Pointer to MRC global data.
  @param[in] DccTypeMask - DCC types Mask. BIT0 = DQ, BIT1 = DQS (Unused in this function), BIT2 = CLK, BIT3 = WCK
**/
VOID
MrcLargeChangeResetSetup (
  IN  MrcParameters *const MrcData,
  IN  UINT32  DccTypeMask
  );

/**
  This function runs for VccClk LVR Auto Trim

  @param[in]  MrcData       - Pointer to global MRC data.

  @returns MrcStatus value reflecting any errors
**/
MrcStatus
MrcVccLVRAutoTrim (
  IN MrcParameters    *const MrcData
  );

/**
  This function updates DqOdtParkMode and EnDQODTParkH which are dependent on DFE Enable

  @param[in]  MrcData      - Pointer to MRC global data.
  @param[in]  Controller   - Controller to review.
  @param[in]  Channel      - Channel to review.
  @param[in]  Byte         - Byte to review.
  @param[in]  IsDFEEnabled - TRUE if DFE Enabled
**/
VOID
MrcUpdateDFEDependentFields (
  IN MrcParameters* const MrcData,
  IN UINT8 Controller,
  IN UINT8 Channel,
  IN UINT8 Byte,
  IN BOOLEAN IsDFEEnabled
  );

/**
  This function performs task that are required to run after all Training and before MC/DDRIO Finalize.

  @param[in, out] MrcData - Include all MRC global data.

  @retval mrcSuccess.
**/
MrcStatus
MrcDataSummary (
  IN  MrcParameters *const MrcData
  );

/**
  Enable/Disable DLL WeakLock if needed.
  Note: We don't enable it in McConfig because CKE is still low during that step.

  @param[in] MrcData - The MRC general data.
  @param[in] Enable - BOOLEAN control to enable (if TRUE), or disable (if FALSE) WeakLock.
**/
VOID
MrcWeaklockEnDis (
  IN MrcParameters* const MrcData,
  IN BOOLEAN              Enable
  );

/**
  This function will assign the DFI control to MPTU or MC according to input parameter.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] DfiControl - MRC_DFI_CONTROL_MC or MRC_DFI_CONTROL_MPTU
**/
VOID
MrcAssignDfiControl (
  IN MrcParameters* const MrcData,
  IN UINT8                DfiControl
  );

/**
  This function will switch the DFI control from MPTU to MC at the end of Green MRC.

  @param[in] MrcData    - Include all MRC global data.

  @retval mrcSuccess if DFI control was switched successfully.
**/
MrcStatus
MrcSwitchDfiControl (
  IN MrcParameters *const MrcData
  );

/**
  This function decodes Controller/Channel/Rank to a group index.
  Desktop has two CCC groups for CS in MC0.Ch0.  Rank2/3 are assigned to group 1
  while Rank0/1 is group 0.  All other combinations are a single group.

  @param[in] MrcData    - Pointer to global MRC data.
  @param[in] Controller - 0-based index
  @param[in] Channel    - 0-based index
  @param[in] Rank       - 0-based index

  @returns CccChCsPiGroup - Group Index value.
**/
CccChCsPiGroup
MrcGetCsRankPiGroup (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Rank
  );

/**
  This function uses the MrcData->Outputs.Controller[].CmdTiming[].CtlPiCode[]
  host structure to finalize the CS timings on all populated ranks. The timings
  are normalized to distribute the per-rank timings between the shared CCC
  channel register field and the per-pin offset field.

  @param[in]  MrcData      - Include all MRC global data.

  @retval mrcFail if the per-rank offset is larger than the per-pin register max
  @retval mrcSuccess in all other cases
**/
MrcStatus
MrcCsPerPinNormalize (
  IN MrcParameters *const MrcData
  );

/**
  This function calculates the SA/ODT Delay and Duration

  @param[in]  MrcData       - Include all MRC global data.
  @param[in]  MinRcvEn      - Min Value of RcvEn in QClk across populated ranks
  @param[in]  GuardbandQ    - Guardband in QCLK
  @param[in]  RdPreambleLow - Read Preamble Static
  @param[in]  RdPreambleT   - Read Preamble Toggle
  @param[in]  RdPostamble   - Read Postamble
  @param[out] Returns SA/ODT Delay and Duration value
**/
VOID
MrcGetSaOdtValue (
  IN   MrcParameters *const MrcData,
  IN   UINT32        MinRcvEn,
  IN   UINT32        GuardbandQ,
  IN   UINT32        RdPreambleLow,
  IN   UINT32        RdPreambleT,
  IN   UINT32        RdPostamble,
  OUT  SaOdtTiming   *SaOdtValue
  );

/**
  This function finds the min UI delay for the RX unmatched path for the entire system.

  @param[in] MrcData - Pointer to global MRC data.

  @returns the min delay in UIs.
**/
UINT8
MrcCalcUIDelay (
  IN  MrcParameters* const MrcData
  );

/**
  This functions walks through all the bytes in the channel and returns the max NUI delay found.

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] Controller - Controller to work on.
  @param[in] Channel    - Channel to work on.

  @returns the max NUI delay on Channel
**/
UINT8
MrcChannelRxNuiDelay (
  IN MrcParameters* const  MrcData,
  IN UINT32                Controller,
  IN UINT32                Channel
  );

/**
  This function sets up Point Test through all targets for Voltage Sensor

  @param[in]  MrcData  - Pointer to global MRC data.

  @returns MrcStatus value reflecting any errors
**/
MrcStatus
MrcVoltageSensorSetup (
  IN MrcParameters    *const MrcData
  );

/**
  This function Reset all LVR Voltage Sensor Counters

  @param[in]  MrcData - Pointer to global MRC data.
  @param[in]  Chop    - Swap inputs to SAL comparator and invert outputs to average input offset.
**/
VOID
MrcVoltageSensorResetCount (
  IN MrcParameters    *const MrcData,
  IN UINT8             Chop
  );

/**
  This function reads all LVR Voltage Sensor Counters to determine if CounterPass based on counter limits
  and prints out the CounterPass status

  @param[in]  MrcData - Pointer to global MRC data.
  @param[in]  Chop    - SAL comparator and invert outputs to compare counters pass/fail test.
**/
VOID
MrcVoltageSensorCounter (
  IN      MrcParameters   *const MrcData,
  IN      UINT8           Chop
  );

/**
  This function sets up Current Sensor registers setup for all Target LVR

  @param[in]  MrcData - Pointer to global MRC data.
**/
VOID
MrcCurrentSensorSetup (
  IN MrcParameters  *const MrcData
  );

/**
  This function enables Current Sensor counter for all Target LVR
  and selects the Test Case to run for FF Current Sensor

  @param[in]  MrcData - Pointer to global MRC data.
  @param[in]  TestNum - Select Test Case for FF Current Sensor, see enum define
                        Test cases available:
                          Test Case 0    ,   Test Case 1 ,    Test Case 2
                        {DATA_FFPH_DRV_PI, DATA_FFPI_DATA, DATA_FFPH_DRV_G}
                        {DATA_FFPI_READ,   DATA_FFRX_DRV,  DATA_FFPH_DRV_G}
                        {CCC_FFPH_DRV_PI,  CCC_FFPI_CCC,   CCC_FFPH_DRV_G }
**/
VOID
MrcCurrentSensorEnable (
  IN MrcParameters  *const MrcData,
  IN UINT8           TestNum
  );

/**
  This function reads Current Sensor counter for all Target LVR and FF Current Sensor
  to calculate Analog Power and FF Power and prints out results for each test case.

  @param[in]  MrcData - Pointer to global MRC data.
**/
VOID
MrcCurrentSensorCounter (
  IN MrcParameters  *const MrcData
  );

/**
  This function converges the PIClk Duty Cycle at TX Pre-Driver node to 50
  percent for Clk and Wck.

  @param[in] MrcData - Include all MRC global data.

  @retval MrcStatus - mrcSuccess
**/
MrcStatus
MrcWckClkPreDriverDcc (
  IN MrcParameters* const MrcData
  );

/**
  This function performs Wck Pad DCC calibration

  @param[in] MrcData    - Include all MRC global data.
**/
MrcStatus
MrcWckPadDcc (
  IN MrcParameters *const MrcData
  );

/**
  This function sets up the RankMap/RankEn of Data FSMControl register

  @param[in, out] MrcData - Include all MRC global data.
  @param[in] Controller   - Controller to access, 0-based.
  @param[in] Channel      - Channel to access, 0-based.
  @param[in] RankPop      - Rank Population Array.
  @param[in] Map          - Map setting to use.
**/
VOID
MrcWriteRankMap (
  IN OUT MrcParameters* const MrcData,
  IN UINT32                   Controller,
  IN UINT32                   Channel,
  IN const UINT8              RankPop[MAX_RANK_IN_CHANNEL],
  IN UINT8                    Map
);

/**
  Program WRRETRAINSWIZZLECONTROL registers for MR18/MR19 decoding on LP5/DDR5

  @param[in] MrcData - The MRC global data.
**/
VOID
MrcPhyProgramDeswizzleRegisters (
  IN MrcParameters *const MrcData
  );

/**
  This function toggles the DLL Reset.

  @param[in] MrcData - Include all MRC global data.
  @param[in] Value   - DLL Reset value
**/
VOID
MrcToggleDllReset (
  IN MrcParameters * const MrcData,
  IN UINT32                Value
  );

/**
  This function sets workpoint data including Qclk, Gear, and AuxClk

  @param[in, out] MrcData - MRC global data.
  @param[in]      NewFreq - Target Frequency for PLL
  @param[in]      Save    - Indicator as to if we should save or restore PllOffset Data
**/
VOID
PllOffsetSaveRestore (
  IN OUT MrcParameters* const MrcData,
  IN     MrcFrequency         NewFreq,
  IN     BOOLEAN              Save
  );

/**
  Calculates different types (roundings) of tWCKDQO parameter:
  - tWCKDQO rounded up,
  - tWCKDQO rounded down (floor),
  - tWCKDQO remainder from rounding.

  @param[in]      MrcData   - Include all MRC global data.
  @param[in]      Type      - Type of tWCKDQO rounding.

  @returns tWCKDQO based on chosen tWCKDQO rounding type.
**/
UINT32 MrcGetTwckdqo (
  IN MrcParameters    *const MrcData,
  IN MRC_TWCKDQO_TYPE  const Type
  );

/**
  This function calculates PHY values after changes to TX DQ/DQS

  @param[in]  MrcData - Pointer to MRC global data.

  @returns MrcStatus value reflecting any errors
**/
MrcStatus
MrcTxDqDqsFinal (
  IN     MrcParameters *const MrcData
  );

/**
  This function forces an Rx DLL Reset

  @param[in] MrcData - Pointer to global MRC data.

  @returns MrcStatus value reflecting any errors
**/
MrcStatus
MrcForceDllReset (
  IN MrcParameters *const MrcData
  );

/**
  This function sets up RankEn/LaneEn based on Memory Tech and Population. Uses WriteToCache and expects caller to flush cache.

  @param[in, out] MrcData - Include all MRC global data.
  @param[out]     RankPop - Array of Rank Population
  **/
VOID
MrcDccRankEnLaneEnSetup (
  IN OUT MrcParameters *const MrcData,
  OUT  UINT8  RankPop[MAX_CONTROLLER][MAX_CHANNEL][MAX_RANK_IN_CHANNEL]
  );

/**
  This function sets up the RankMap/RankEn of Data FSMControl register based on POR settings

  @param[in, out] MrcData - Include all MRC global data.
  @param[in] Controller   - Controller to access, 0-based.
  @param[in] Channel      - Channel to access, 0-based.
  @param[in] RankPop      - Rank Population Array.
  @param[in] Map          - Map setting to use.
**/
VOID
MrcWriteRankMapPorSettings (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function does PI Serializer DCC calibration

  @param[in, out] MrcData - Include all MRC global data.

  @retval mrcSuccess if DCC converges
  @retval mrcFail otherwise
**/
MrcStatus
MrcDccPISerializerCalibration (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function gets or sets the value for ForceRxOn.

  @param[in]      MrcData               - Include all MRC global data.
  @param[in]      UpdateMode            - The mode to be used for program.
  @param[in]      GetSetMode            - The GetSet mode to be used for program.
  @param[in, out] *ForceRxOnDqsOrAmpOn  - Value to be written / Saved for ForceRxOnDqs [NVL] or ForceRxOn [PTL].
  @param[in, out] *ForceRxOnDqsMux      - Value to be written / Saved for ForceRxOnDqsMux [NVL], not used in PTL.
  @param[in, out] *ForceRxOnDq          - Value to be written / Saved for ForceRxOnDq [NVL], not used in PTL.
**/
MRC_IRAM1_FUNCTION
VOID
MrcGetSetForceRxOn (
  IN     MrcParameters* const MrcData,
  IN     UPDATE_MODE    UpdateMode,
  IN     UINT32         GetSetMode,
  IN OUT INT64          *ForceRxOnDqsOrAmpOn,
  IN OUT INT64          *ForceRxOnDqsMux,
  IN OUT INT64          *ForceRxOnDq
  );

/**
  Set up, perform, tear down and print results of LVR Auto Trim calibration.

  @param[in]  MrcData - Pointer to global MRC data.

  @retval mrcSuccess if the calibration succeeds
  @retval mrcFail otherwise
**/
MrcStatus
LvrAutoTrimRun (
  IN MrcParameters *const MrcData
  );

/**
  This function sets the DQ ODT park mode.

  @param[in]      MrcData      - Include all MRC global data.
  @param[in]      UpdateMode   - The mode to be used for program.
  @param[in]      GetSetMode   - The GetSet mode to be used for program.
  @param[in, out] *Value       - Value to be written for Dq Od tPark Mode.
**/
MRC_IRAM1_FUNCTION
VOID
MrcSetDqOdtParkMode (
  IN MrcParameters* const MrcData,
  IN UPDATE_MODE    UpdateMode,
  IN UINT32         GetSetMode,
  IN INT64          *Value
  );

/**
  This function sets the force odt on.

  @param[in]      MrcData      - Include all MRC global data.
  @param[in]      UpdateMode   - The mode to be used for program.
  @param[in]      GetSetMode   - The GetSet mode to be used for program.
  @param[in, out] *Value       - Value to be written for Dq Od tPark Mode.
**/
MRC_IRAM1_FUNCTION
VOID
MrcSetForceOdtOn (
  IN MrcParameters* const MrcData,
  IN UPDATE_MODE    UpdateMode,
  IN UINT32         GetSetMode,
  IN INT64          *Value
  );

/**
  This function gets the endqsodtparkmode value.

  @returns the value for endqsodtparkmode
**/
UINT8
MrcGetEnDqsOdtParkModeValue (
  );

/**
  This function handles getting the register offset for the requested parameter.
  It will determine multicast by the parameter exceeding the MAX of the
  Socket/Channel/Rank/Strobe/Bit.

  @param[in]  MrcData      - Pointer to global data.
  @param[in]  Group        - DDRIO group to access.
  @param[in]  Socket       - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]  Controller   - Memory controller in the processor socket (0-based).
  @param[in]  Channel      - DDR Channel Number within the memory controller (0-based).
  @param[in]  Rank         - Rank number within a channel (0-based).
  @param[in]  Strobe       - Dqs data group within the rank (0-based).
  @param[in]  Lane         - Lane index to GSM_GT Group.
  @param[in]  FreqIndex    - Index supporting multiple operating frequencies. (Not used in Client CPU's)
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR
**/
UINT32
MrcGetDdrIoRegOffset (
  IN  MrcParameters *const  MrcData,
  IN  GSM_GT        const   Group,
  IN  UINT32        const   Socket,
  IN  UINT32        const   Controller,
  IN  UINT32        const   Channel,
  IN  UINT32        const   Rank,
  IN  UINT32        const   Strobe,
  IN  UINT32        const   Lane,
  IN  UINT32        const   FreqIndex,
  OUT UINT64_STRUCT *const  VolatileMask
  );

/**
  Function used to get the CR Offset for Write Data Groups.

  @param[in]  Group        - DDRIO group being accessed.
  @param[in]  Channel      - DDR Channel Number within the processor socket (0-based).
  @param[in]  Rank         - Rank index in the channel. (0-based).
  @param[in]  Strobe       - Dqs data group within the rank (0-based).
  @param[in]  Bit          - Bit index within the data group (0-based).
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR.
**/
UINT32
GetDdrIoDataWriteOffsets (
  IN  GSM_GT                Group,
  IN  UINT32                Channel,
  IN  UINT32                Rank,
  IN  UINT32                Strobe,
  IN  UINT32                Bit,
  OUT UINT64_STRUCT *const  VolatileMask
  );

/**
  Function used to get the CR Offset for training offset and comp offset Groups.

  @param[in]  Group     - DDRIO group being accessed.
  @param[in]  Channel   - DDR Channel Number within the processor socket (0-based).
  @param[in]  Strobe    - Dqs data group within the rank (0-based).

  @returns the offset of the CR.
**/
UINT32
GetDdrIoDataTrainOffsets (
  IN  GSM_GT                Group,
  IN  UINT32                Channel,
  IN  UINT32                Strobe
  );

/**
  Function used to get the CR Offset for DDRDATA_SHARED groups.

  @param[in]  MrcData      - Global MRC data structure.
  @param[in]  Group        - DDRIO group being accessed.
  @param[in]  Socket       - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]  Channel      - DDR Channel Number within the processor socket (0-based).
  @param[in]  Rank         - Rank index in the channel. (0-based).
  @param[in]  Strobe       - Dqs data group within the rank (0-based).
  @param[in]  Bit          - Bit index within the data group (0-based).
  @param[in]  FreqIndex    - Index supporting multiple operating frequencies. (Not used in Client CPU's)
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR.
**/
UINT32
GetDdrIoDataSharedOffsets (
  IN MrcParameters          *MrcData,
  IN  GSM_GT                Group,
  IN  UINT32                Socket,
  IN  UINT32                Channel,
  IN  UINT32                Rank,
  IN  UINT32                Strobe,
  IN  UINT32                Bit,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT *const  VolatileMask
  );

/**
  Function used to get the CR Offset for DDR IO Configuration settings.

  @param[in]  MrcData      - Global MRC data structure.
  @param[in]  Group        - DDRIO group being accessed.
  @param[in]  Channel      - DDR Channel Number(0-based).
  @param[in]  Rank         - Rank index in the channel. (0-based).
  @param[in]  Strobe       - Dqs data group within the rank (0-based).
  @param[in]  Lane         - Lane index to GSM_GT Group.
  @param[in]  FreqIndex    - Index supporting multiple operating frequencies.
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR.
**/
UINT32
MrcGetDdrIoConfigOffsets (
  IN MrcParameters          *MrcData,
  IN  GSM_GT                Group,
  IN  UINT32                Channel,
  IN  UINT32                Rank,
  IN  UINT32                Strobe,
  IN  UINT32                Lane,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT *const  VolatileMask
  );

/**
  Function used to get the CR Offset for DDR IO Command Groups.

  @param[in]  Group        - DDRIO group being accessed.
  @param[in]  Channel      - DDR Channel Number within the processor socket (0-based).
  @param[in]  FreqIndex    - Index supporting multiple operating frequencies. (Not used in Client CPU's)
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR.
**/
UINT32
GetDdrIoCommandOffsets (
  IN  GSM_GT                Group,
  IN  UINT32                Channel,
  IN  UINT32                FreqIndex,
  IN  BOOLEAN               IsDimm1,
  OUT UINT64_STRUCT *const  VolatileMask
  );

/**
  This function returns the offset for the specific Channel/Strobe of RefPiControl in the DATA partition.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
RefPiControlDataOffset (
  IN  UINT32  const   Channel,
  IN  UINT32  const   Strobe
  );

/**
  This function returns the offset for the specific Channel of RefPiControl in the CCC partition.

  @param[in]  Channel - 0-based index of Channel to access.

  @returns the offset of the CR
**/
UINT32
RefPiControlCccOffset (
  IN  UINT32  const   Channel
  );

/**
  This function returns the offset for the specific
  Channel/Strobe of any register in the DATA partition.

  @param[in]  RegOffset  - 0-based RegOffset base address.
  @param[in]  Controller - 0-based index of Controller access.
  @param[in]  Channel    - 0-based index of Channel to access.
  @param[in]  Strobe     - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
MrcGetDataOffset (
  IN  MrcParameters   *MrcData,
  IN  UINT32  const   RegOffset,
  IN  UINT32  const   Controller,
  IN  UINT32  const   Channel,
  IN  UINT32  const   Strobe
  );

// These functions are called in DdrioInit()
/**
  This function returns the offset to access specific Channel/Strobe of DataControl0.

  @param[in]  Channel     - 0-based index of Channel to access.
  @param[in]  Strobe      - 0-based index of Strobe to access.
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR
**/
UINT32
DataControl0Offset (
  IN  UINT32  const   Channel,
  IN  UINT32  const   Strobe,
  OUT UINT64_STRUCT *const  VolatileMask
);

/**
  This function returns the offset to access specific Channel/Strobe of DataControl1.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
DataControl1Offset (
  IN  UINT32  const   Channel,
  IN  UINT32  const   Strobe
);

/**
  This function returns the offset to access specific Channel/Strobe of DataControl2.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
DataControl2Offset (
  IN  UINT32  const   Channel,
  IN  UINT32  const   Strobe
);

/**
  This function returns the offset to access specific Channel/Strobe of DataControl3.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
DataControl3Offset (
  IN  UINT32  const Channel,
  IN  UINT32  const Strobe
);

/**
  This function returns the offset to access specific Channel/Strobe of DataRxEqTap01.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
DataRxEqTap01Offset (
  IN  UINT32  const Channel,
  IN  UINT32  const Strobe
);

/**
  This function returns the offset to access specific Channel/Strobe of DataControl5.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
DataControl5Offset (
  IN  UINT32  const Channel,
  IN  UINT32  const Strobe
);

/**
  This function returns the offset to access specific Channel of DataControl6.

  @param[in]  Index - 0-based index of DATA_SHAREDx to access.

  @returns the offset of the CR
**/
UINT32
DataControl6Offset (
  IN  UINT32  const   Index
);

/**
  This function returns the offset to access specific Channel/Strobe of Cbt.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
CbtOffset (
  IN  UINT32  const   Channel,
  IN  UINT32  const   Strobe
);

/**
  This function returns the offset to access specific Channel/Strobe of WRRETRAINSWIZZLECONTROL.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
WrRetrainSwizzleControlOffset (
  IN  UINT32  const Channel,
  IN  UINT32  const Strobe
  );

/**
  This function returns the offset to access specific Channel/Strobe of WRRETRAINRANK[0..3].

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Rank    - 0-based index of Rank to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
WrRetrainRankOffset (
  IN  UINT32  const Channel,
  IN  UINT32  const Rank,
  IN  UINT32  const Strobe
  );

/**
  This function returns the offset to access specific Channel/Strobe of RETRAININITRANK[0..3].

  @params[in]  Channel - 0-based index of Channel to access.
  @params[in]  Rank    - 0-based index of Rank to access.
  @params[in]  Strobe  - 0-based index of Strobe to access.

  @retval UINT32 - CR offset
**/
UINT32
RetrainRankOffset(
  IN  UINT32  const Channel,
  IN  UINT32  const Rank,
  IN  UINT32  const Strobe
);

/**
  This function returns the offset to access specific Channel/Strobe of WrRetrainControlStatus.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
WrRetrainControlStatusOffset (
  IN  UINT32  const   Channel,
  IN  UINT32  const   Strobe
);

/**
  This function returns the offset to access specific Channel/Strobe of RdRetrainControlStatus.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
RdRetrainControlStatusOffset (
  IN  UINT32  const   Channel,
  IN  UINT32  const   Strobe
);

/**
  This function returns the offset to access specific VccDllComp register.
  There are 12 VccDll partitions, 8 for data and 4 for CCC

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
VccDllDataCCCCompOffset (
  IN  UINT32  const   Channel,
  IN  UINT32  const   Strobe
);

/**
  This function returns the offset to access specific VccDllCouplingCap registers.
  There are 12 VccDll partitions, 8 for data and 4 for CCC

  @param[in]  Channel - 0-based index of Channel to access.

  @returns the offset of the CR
**/
UINT32
CouplingCapOffset (
  IN  UINT32  const   Channel
);

/**
  This function returns the offset to access specific Channel/Strobe of Data's DCCFSMCONTROL.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.

  @returns the offset of the CR
**/
UINT32
DataDccFsmCtlOffset (
  IN  UINT32  const Channel,
  IN  UINT32  const Strobe
  );

/**
  This function returns the offset to access specific Index and SaGv Point for DDRCRVCCDDQCONTROL_Target

  @param[in]  Index - DDRDATA DDRCRVCCDDQCONTROL SBMEM Index
  @param[in]  FreqIndex  - SaGvIndex
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR
**/
UINT32
DataVccDdqTargetOffset (
  IN  UINT32                Index,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT  *const VolatileMask
  );

/**
  This function returns the offset to access specific Index for DDRCRLVRAutoTrim

  @param[in]  Index - DATA DDRCRLVRAutoTrim SBMEM Index
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR
**/
UINT32
DataLvrAutoTrimOffset (
  IN  UINT32                Index,
  OUT UINT64_STRUCT  *const VolatileMask
  );

/**
  This function returns the offset to access specific Index and SaGv Point for DDRCRVCCIOGCONTROL_Target

  @param[in]  Index - DDRDATA DDRCRVCCIOGCONTROL SBMEM Index
  @param[in]  FreqIndex  - SaGvIndex
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR
**/
UINT32
DataVccIogTargetOffset(
  IN  UINT32                Index,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT  *const VolatileMask
  );

/**
  This function returns the offset to access specific Index and SaGv Point for DDRCRVCCDDQCONTROL_Target

  @param[in]  Index - CCC DDRCRVCCDDQCONTROL SBMEM Index
  @param[in]  FreqIndex  - SaGvIndex
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR
**/
UINT32
CccVccDdqTargetOffset (
  IN  UINT32                Index,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT  *const VolatileMask
  );

/**
  This function returns the offset to access specific Index for DDRCRLvrAutoTrim

  @param[in]  Index - CCC DDRCRLVRAutoTrim SBMEM Index
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR
**/
UINT32
CccLvrAutoTrimOffset (
  IN  UINT32                Index,
  OUT UINT64_STRUCT  *const VolatileMask
  );

/**
  This function returns the offset to access specific Index and SaGv Point for DDRCRVCCIOGCONTROL_Target

  @param[in]  Index - CCC DDRCRVCCIOGCONTROL SBMEM Index
  @param[in]  FreqIndex  - SaGvIndex
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR
**/
UINT32
CccVccIogTargetOffset (
  IN  UINT32                Index,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT  *const VolatileMask
  );

/**
  This function returns the offset to access specific Channel/Lane of CCC Lane.

  @param[in] Channel      - 0-based index of Channel to access.
  @param[in]  Lane         - Lane index
  @param[out] VolatileMask - Volatile Mask

  @returns the offset of the CR.
**/
UINT32
GetDdrIoCccLaneOffsets (
  IN  UINT32                  Channel,
  IN  UINT32                  Lane,
  OUT UINT64_STRUCT  *const   VolatileMask
  );

/**
  This function returns the offset to access specific Channel/Lane of PGVTT CCC Lane.

  @param[in] Channel      - 0-based index of Channel to access.
  @param[out] VolatileMask - Volatile Mask

  @returns the offset of the CR.
**/
UINT32
GetDdrIoVttLaneOffsets (
  IN  UINT32                  Channel,
  OUT UINT64_STRUCT  *const   VolatileMask
  );

/**
  This function returns the offset to access specific AuxClk Offset Calibrarion for AuxClk

  @param[in] FreqIndex    - Sagv Point
  @param[out] VolatileMask - Volatile Mask

  @returns the offset of the CR.
**/
UINT32
AuxClkOffOffset (
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT  *const VolatileMask
  );

/**
  This function returns the offset to access specific AuxClk Offset Calibrarion for QClk/PHClk

  @param[in] FreqIndex    - Sagv Point
  @param[out] VolatileMask - Volatile Mask

  @returns the offset of the CR.
**/
UINT32
QclkPhClkOffset (
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT  *const VolatileMask
  );

/**
  This function returns the offset to access specific QClkCal

  @param[in] FreqIndex    - Sagv Point
  @param[out] VolatileMask - Volatile Mask

  @returns the offset of the CR.
**/
UINT32
QclkCalOffOffset (
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT  *const VolatileMask
  );


/**
  This function translates HW CCC Partition instance to RAL instance based on interleave configuration.

  @param[in] MrcData     - MRC global data.
  @param[in] CccInstance - CCC instance to be translated.

  @returns the translated CCC instance.
**/
UINT8
MrcTranslateCccInstance (
  IN MrcParameters *const MrcData,
  IN UINT8          const CccInstance
  );

/**
  This function does the ADC global override

  @param[in] MrcData - Include all MRC global data.
**/
VOID
MrcAdcGlobalOverride (
  IN MrcParameters *const MrcData
  );

/**
  Returns the current ADC count status per partition/channel/byte

  @param[in] MrcData   - Include all MRC global data.
  @param[in] Partition - Which partition's ADC is being accessed (Data, CCC, or Comp)
  @param[in] BaseReg   - The base register's address for that partition
  @param[in] Channel   - Which channel is being accessed
  @param[in] Byte      - Which byte is being accessed

  @returns the current value of the ADC's count status field
**/
UINT32
MrcGetAdcCount (
  IN MrcParameters *const MrcData,
  IN PARTITION_TYPE       Partition,
  IN UINT32               BaseReg,
  IN UINT8                Channel,
  IN UINT8                Byte
  );

/**
  Converts an ADC count to a voltage reading based on Vccddq2

  @param[in] MrcData  - Include all MRC global data.
  @param[in] AdcCount - The ADC count to be converted to a voltage value

  @returns the resulting Voltage (in mV) based on the ADC count
**/
UINT32
MrcAdcCountToVoltage (
  IN MrcParameters *const MrcData,
  UINT32           AdcCount
  );

/**
  This function configure some registers according to HVM requirements:
  1. clear DataValue(BSCAN override of data value for each ccc) in CCC_CR_DDRCRBSCANDATA_REG
  2. Enter into infinite loop in the end of the function.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval MrcStatus - mrcSuccess
**/
MrcStatus
MrcHVMFinalize (
  IN     MrcParameters* const MrcData
);

/**
  This function generates the hash used to execute the Get/Set function.
  The hash consists of: Register Offset, BitField start bit, BitField length.

  @param[in]      MrcData       - Pointer to global data structure.
  @param[in]      Socket        - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]      Controller    - Memory Controller Number within the processor (0-Based).
  @param[in]      Channel       - DDR Channel Number within the memory controller (0-based).
  @param[in]      Rank          - Rank number within a channel (0-based).
  @param[in]      Strobe        - Dqs data group within the rank (0-based).
  @param[in]      Lane          - Lane index within the GSM_GT group (0-based).
  @param[in]      FreqIndex     - Index supporting multiple operating frequencies.
  @param[in]      Group         - DDRIO group to access.
  @param[out]     HashVal       - Pointer to return the MMIO access instruction.
  @param[out]     VolatileMask  - Pointer to return the Register volatile mask.

  @returns MrcStatus value reflecting any errors
**/
MrcStatus
MrcGetDdrIoHash (
  IN   MrcParameters *const  MrcData,
  IN   UINT32        const   Socket,
  IN   UINT32        const   Controller,
  IN   UINT32        const   Channel,
  IN   UINT32        const   Rank,
  IN   UINT32        const   Strobe,
  IN   UINT32        const   Lane,
  IN   UINT32        const   FreqIndex,
  IN   GSM_GT        const   Group,
  OUT  UINT32        *const  HashVal,
  OUT  UINT64_STRUCT *const  VolatileMask
  );

/**
  This function generates the hash used to execute the Get/Set function.
  The hash consists of: Register Offset, BitField start bit, BitField length.

  @param[in]      MrcData       - Pointer to global data structure.
  @param[in]      Socket        - Unused here, but needed so we can use a pointer to this function or a different function
  @param[in]      Controller    - Unused here, but needed so we can use a pointer to this function or a different function
  @param[in]      Channel       - PHY channel (0 based).
  @param[in]      Rank          - Rank number within a channel (0-based).
  @param[in]      Strobe        - Dqs data group within the rank (0-based).
  @param[in]      Lane          - Lane index within the GSM_GT group (0-based).
  @param[in]      FreqIndex     - Index supporting multiple operating frequencies.
  @param[in]      Group         - DDRIO group to access.
  @param[out]     HashVal       - Pointer to return the MMIO access instruction.
  @param[out]     VolatileMask  - Pointer to return the Register volatile mask.

  @returns MrcStatus value reflecting any errors
**/
MrcStatus
MrcGetDdrIoCfgHash (
  IN   MrcParameters *const  MrcData,
  IN   UINT32        const   Socket,
  IN   UINT32        const   Controller,
  IN   UINT32        const   Channel,
  IN   UINT32        const   Rank,
  IN   UINT32        const   Strobe,
  IN   UINT32        const   Lane,
  IN   UINT32        const   FreqIndex,
  IN   GSM_GT        const   Group,
  OUT  UINT32        *const  HashVal,
  OUT  UINT64_STRUCT *const  VolatileMask
  );

/**
  This function returns the register limits.

  @param[in]  MrcData   - Pointer to global data.
  @param[in]  Group     - DDRIO group to access.
  @param[in]  Rank      - Rank number within a channel (0-based).
  @param[out] MinVal    - Return pointer for Minimum Value supported.
  @param[out] MaxVal    - Return pointer for Maximum Value supported.
  @param[out] WaitTime  - Return pointer for settle time required in microseconds.

  @retval mrcSuccess if the parameter is found
  @retval mrcFail otherwise
**/
MrcStatus
MrcGetDdrIoGroupLimits (
  IN  MrcParameters *const MrcData,
  IN  GSM_GT  const   Group,
  IN  UINT32  const   Rank,
  OUT INT64   *const  MinVal,
  OUT INT64   *const  MaxVal,
  OUT UINT32  *const  WaitTime
  );

/**
  This function returns DDRIO Configuration group limits.

  @param[in]  MrcData   - Pointer to global data.
  @param[in]  Group     - DDRIO group to access.
  @param[in]  Rank      - Rank number within a channel (0-based).
  @param[out] MinVal    - Return pointer for Minimum Value supported.
  @param[out] MaxVal    - Return pointer for Maximum Value supported.
  @param[out] WaitTime  - Return pointer for settle time required in microseconds.

  @retval mrcSuccess if the parameter is found
  @retval mrcFail otherwise
**/
MrcStatus
MrcGetDdrIoCfgGroupLimits (
  IN  MrcParameters *const MrcData,
  IN  GSM_GT  const   Group,
  IN  UINT32  const   Rank,
  OUT INT64   *const  MinVal,
  OUT INT64   *const  MaxVal,
  OUT UINT32  *const  WaitTime
  );

/**
  Programs Delay/Duration for the SenseAmp and MCODT based on RcvEn timing
  Provide GuardBand > 0 if needed to be more conservative in timing
  Main goal is to optimize power

  @param[in,out] MrcData   - Include all MRC global data.
**/
VOID
MrcUpdateSampOdtTiming (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function does get set for the EnPhaseGating.

  @param[in]      MrcData      - Include all MRC global data.
  @param[in]      UpdateMode   - The mode to be used for program.
  @param[in]      GetSetMode   - The GetSet mode to be used for program.
  @param[in, out] *Value       - Value to be written for EnPhaseGating.
**/
VOID
MrcGetSetEnPhaseGating (
  IN MrcParameters* const MrcData,
  IN UPDATE_MODE    UpdateMode,
  IN UINT32         GetSetMode,
  IN INT64          *Value
  );

/**
  This function is to set up the margin parameters for RdV.

  @param[in,out] MrcData      - Include all MRC global data.
  @param[in]     McStart      - MC number starts
  @param[in]     McEnd        - MC number ends.
  @param[in]     ChannelStart - Channel number starts.
  @param[in]     ChannelEnd   - Channel number ends
  @param[in]     ByteStart    - Byte number starts.
  @param[in]     ByteEnd      - Byte number ends.
  @param[in]     GsmMode      - Get Set mode.
  @param[in]     GetSetVal    - The pointer to the get set value
  @param[in,out] UpdateGrp    - Update Group or not.
  @param[in,out] Group        - The group parameter which will be used during margin
**/
VOID
SetupMarginRdV (
  IN OUT MrcParameters *const MrcData,
  IN           UINT32          McStart,
  IN           UINT32          McEnd,
  IN           UINT32          ChannelStart,
  IN           UINT32          ChannelEnd,
  IN           UINT32          ByteStart,
  IN           UINT32          ByteEnd,
  IN           UINT8           GsmMode,
  IN           INT64           *GetSetVal,
  IN OUT       BOOLEAN         *UpdateGrp,
  IN OUT       GSM_GT          *Group
  );

/*
  Update the requested Param offset with the given Value,
  using direct multicast CR access for speed.

  @param[in]  MrcData - Global MRC data structure
  @param[in]  Param   - Margin param
  @param[in]  Value   - The value to program
*/
VOID
MrcWriteDirectMulticast (
  IN MrcParameters *const MrcData,
  IN UINT8                Param,
  IN INT32                Value
  );

/**
  This function sets TlineTermination

  @param[in, out] MrcData - All the MRC global data.
**/
VOID
SetTlineTermination (
  IN OUT MrcParameters *const MrcData
 );

/**
  The function programs Rx VREF

  @param[in, out] MrcData        - Include all MRC global data.
  @param[in]      Controller     - Memory controller
  @param[in]      Channel        - Channel number
  @param[in]      Strobe         - Strobe number
  @param[in]      Mode           - Get Set Mode will be used in the GetSet function
  @param[in]      GetSetVref     - The pointer to the VREF value which will be used by GetSet
**/
VOID
MrcProgramRxVref (
  IN OUT MrcParameters *const MrcData,
  IN UINT8                    Controller,
  IN UINT8                    Channel,
  IN UINT8                    Strobe,
  IN UINT8                    Mode,
  IN INT64                    *GetSetVref
  );

/**
  The function sets Rx VREF for DQ.

  @param[in, out] MrcData        - Include all MRC global data.
  @param[in]      Controller     - Memory controller
  @param[in]      Channel        - Channel number
  @param[in]      Strobe         - Strobe number
  @param[in]      Mode           - Get Set Mode will be used in the GetSet function
  @param[in]      GetSetVref     - The pointer to the VREF value which will be used by GetSet

**/
void
MrcSetRxDqVref (
  IN OUT MrcParameters *const MrcData,
  IN UINT8                    Controller,
  IN UINT8                    Channel,
  IN UINT8                    Strobe,
  IN UINT8                    Mode,
  IN INT64                    *GetSetVref
  );

/**
  The function reads Rx VREF per byte for DQ.

  @param[in, out] MrcData        - Include all MRC global data.
  @param[in]      Controller     - Memory controller
  @param[in]      Channel        - Channel number
  @param[in]      Strobe         - Strobe number
  @param[in]      Mode           - Get Set Mode will be used in the GetSet function
  @param[in]      GetSetVref     - The pointer to the VREF value which will be used by GetSet

**/
void
MrcGetRxDqVref (
  IN OUT MrcParameters *const MrcData,
  IN UINT8                    Controller,
  IN UINT8                    Channel,
  IN UINT8                    Strobe,
  IN UINT8                    Mode,
  IN INT64                    *GetSetVref
  );

/**
  The function programs Rx DBI VREF which is in a byte register

  @param[in, out] MrcData        - Include all MRC global data.
  @param[in]      Controller     - Memory controller
  @param[in]      Channel        - Channel number
  @param[in]      Strobe         - Strobe number
  @param[in]      Mode           - Get Set Mode will be used in the GetSet function
  @param[in]      GetSetVref     - The pointer to the VREF value which will be used by GetSet
**/
VOID
MrcProgramRxDbiVrefPerByte (
  IN OUT MrcParameters *const MrcData,
  IN UINT8                    Controller,
  IN UINT8                    Channel,
  IN UINT8                    Strobe,
  IN UINT8                    Mode,
  IN INT64                    *GetSetVref
  );


/**
  This function determines the max UI delay of the read unmatched path per channel, and
  ensures all bytes align to that max by overriding CompDqsOffset

  @param[in, out] MrcData         - Pointer to MRC global data.
  @param[in, out] UpdateTargetNUI - Pointer to UpdateTargetNUI.
**/
VOID
MrcNormalizeTargetNui (
  IN OUT MrcParameters *const MrcData,
  IN OUT BOOLEAN       *UpdateTargetNUI
  );

/**
  Save or restore RxDLL VctlInit values
  @param[in]      MrcData       - Include all MRC global data.
  @param[in]      SaveRestore   - Control flag to save registers or to restore registers.
  @param[in, out] RxDllVctlInit - data to save / restore
**/
VOID
MrcVctlInitSaveRestore (
  IN     MrcParameters* const MrcData,
  IN     MrcSaveOrRestore     SaveOrRestore,
  IN OUT UINT32               RxDllVctlInit[MRC_DATA_MOBILE_NUM][MRC_DATA_CH_NUM]
  );

/**
  Program RxDqs Voc for a given byte

  @param[in, out] MrcData     - Include all MRC global data.
  @param[in     ] voccode     - voccode is RxDqsAmpOffset given in [0..31] range.
  @param[in     ] Rank        - Rank to operate on
  @param[in     ] Controller  - Controller to operate on
  @param[in     ] Channel     - Channel to operate on
  @param[in     ] Byte        - Byte to to operate on
**/
VOID
SetRxDqsVoc (
  IN OUT MrcParameters *const MrcData,
  IN     UINT8                voccode,
  IN     UINT32               Rank,
  IN     UINT32               Controller,
  IN     UINT32               Channel,
  IN     UINT32               Byte
  );

/**
  This function responsible for setting RX PI

  @param[in, out] MrcData - Include all MRC global data.
  @param[in] PiCode       - Linear delay (PI ticks) moves the RX DQS within the bitlane relative to the RX DQ signal.
  @param[in] Rank         - Rank to operate on.
  @param[in] Controller   - Controller to operate on.
  @param[in] Channel      - Channel to operate on.
  @param[in] Byte         - Byte to update the specified parameter.
**/
VOID
SetRxPI (
  IN OUT MrcParameters *const MrcData,
  IN     UINT32               PiCode,
  IN     UINT32               Rank,
  IN     UINT32               Controller,
  IN     UINT32               Channel,
  IN     UINT32               Byte
  );

/**
  Enable / Disable Phy Rx "always open" mode, allow it to get the continuous RX DQS although no RD commands are being sent.
  Also disable clock gating for proper ForceCompUpdate propagation.

  GsmIocRxPiPwrDnDis      DATA_CR_DDRCRDATACONTROL5.RxPiOn              = 1
  GsmIocForceRxAmpOn      DATA_CR_DDRCRDATACONTROL0.ForceRxOn           = 1
  GsmIocBiasPMCtrl        DATA_CR_DDRCRDATACONTROL0.BiasPMCtrl          = 0
  GsmIocInternalClocksOn  DATA_CR_DDRCRDATACONTROL0.InternalClocksOn    = 1
  GsmIocForceRcvEnOn      DATA_CR_DDRCRDATACONTROL2.ForceRcvEnOn        = 1
  GsmIocRptChRepClkOn     MCMISCS_WRITECFGCH[0..7].RptChRepClkOn        = 1
  GsmIocCompClkOn         DDRPHY_DDRCOMP_CR_DDRCRCOMPCTL1.CompClkOn     = 1
  GsmIocDccClkGateDisable DDRSCRAM_CR_DDRMISCCONTROL1.dccclkgatedisable = 1

  @param[in, out] MrcData       - Include all MRC global data.
  @param[in]      Enable        - Enable or disable Rx "always open" mode
  @param[in, out] RxDqsDccSave  - struct to save the original CR values when Enable is TRUE, or to restore the CR values from (when Enable = FALSE)
**/
VOID
SetPhyRxMode (
  IN OUT MrcParameters * const  MrcData,
  IN     BOOLEAN                Enable,
  IN OUT MRC_RX_DQS_DCC_SAVE    *RxDqsDccSave
  );

/**
  This function returns the offset to access specific Channel/Rank/Strobe/Bit of DataDqRankXLaneY.

  @param[in]  Channel - 0-based index of Channel to access.
  @param[in]  Rank    - 0-based index of Rank to access.
  @param[in]  Strobe  - 0-based index of Strobe to access.
  @param[in]  Bit     - 0-based index of Bit to access.
  @param[out] VolatileMask - Mask indicating which bits are volatile in register.

  @return UINT32 - CR offset
**/
UINT32
DataDqRankXLaneYOffset (
  IN  UINT32  const Channel,
  IN  UINT32  const Rank,
  IN  UINT32  const Strobe,
  IN  UINT32  const Bit,
  OUT UINT64_STRUCT *const VolatileMask
  );

/**
  This function enables or disables the rxdqs delay comp.

  @param[in, out] MrcData          - Include all MRC global data.
  @param[in]      RxDqsDelayCompEn - Enable or disable rxdqs delay comp.
**/
VOID
RxDqsDelayCompEnable (
  IN MrcParameters* const MrcData,
  IN BOOLEAN              RxDqsDelayCompEn
  );

/**
  Check RxDqsDelayP/N for saturation and update LockUI parameters if needed.

  @param[in] MrcData - Pointer to global MRC data.

  @retval TRUE  - LockUI parameters were updated due to saturation
  @retval FALSE - LockUI parameters were not updated
**/
BOOLEAN
FixLockUISaturate (
  IN MrcParameters * const  MrcData
  );

/**
  Offset the LockUI parameters by a given amount per byte and P/N

  @param[in] MrcData    - Pointer to global MRC data.
  @param[in] OffsetPs   - Arrays of RxDqs delay offset values
  @param[in] DebugPrint - Enable / Disable debug prints
**/
VOID
SetLockUIOffset (
  IN MrcParameters * const  MrcData,
  IN INT32                  OffsetPs[MRC_DATA_SHARED_NUM_WITH_ECC][MRC_DATA_CH_NUM][MAX_EDGES],
  IN BOOLEAN                DebugPrint
  );

/**
  This function switches between Div6 and Div1 Comp Mode.

  @param[in]  MrcData - Include all MRC global data.
  @param[in]  Enable  - True to Enable Div6 Comp mode, False to Enable Div1.
**/
VOID
EnableDiv6CompMode (
  IN MrcParameters* const MrcData,
  IN BOOLEAN        const Enable
  );

/**
  This function prints out NUI related fields

  @param[in, out] MrcData - Include all MRC global data.
**/
VOID
MrcPrintDdrCompNUI (
  IN OUT MrcParameters *const MrcData
  );

/**
  Sets Outputs->ReadPreamble (DDR5: Total Read Preamble, LP5: Read Preamble Toggle) and Outputs->EarlyDqs based on Unmatched Path Delay
  Calculates and sets RcvEnWaveShape, RXTogglePreAmble, RcvEnExtFF, RstNumPre, RxReadPointer

  T: Read Preamble Toggle
  E: EarlyDqs
  P: Postamble

  if (DDR5)
    if (DataRate <= 3200 MTs)          1T/0E/0P              No dummy toggle given low frequency
    if (UnmatchedPathDelay <= 2 UI)    2T/0E/0P              1 dummy toggle, Lock to 2.5 UI
    else                               2T/1E/0P              1 dummy toggle, Lock to 4.5 UI

  if (LP5)
    if (UnmatchedPathDelay <= 2UI)     2T/0E/0P              1 dummy toggle, Lock to 2.5 UI
    else                               4T/0E/0P              1 dummy toggle, Lock to 4.5 UI

  @param[in] MrcData  - Include all MRC global data.
**/
VOID
MrcRxUnmatchedUpdate(
    IN OUT MrcParameters* const MrcData
);

/**
  This function calculates DCC Samples based on RankMap

  @param[in]  MrcData - Pointer to MRC global data.
  @param[in]  RankMap - DCC supports 2 ranks such that the 2-bit rank select is mapped as (0: '0. 1: rank[0]. 2: rank[1]. 3: reserved).

  @returns calculated DCC samples using target clock 2^n times to measure duty cycle
**/
INT64
MrcCalcDccSamples (
  IN     MrcParameters *const MrcData,
  IN     INT64                RankMap
  );

/**
  Function used to get the CR Offset for registers listed under PLL partitions.

  @param[in]  Group        - DDRIO group being accessed.
  @param[in]  Index        - # of field.
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR.
**/
UINT32
GetDdrIoPllOffsets (
  IN  GSM_GT                Group,
  IN  UINT32                Index,
  OUT UINT64_STRUCT *const  VolatileMask
  );

/**
  Function used to get the CR Offset for registers listed under PGx partitions.

  @param[in]  MrcData      - Global MRC data structure.
  @param[in]  Group        - DDRIO group being accessed.
  @param[in]  Lane         - Lane index within the data group (0-based).
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR.
**/
UINT32
GetDdrIoPgOffsets (
  IN  MrcParameters         *MrcData,
  IN  GSM_GT                Group,
  IN  UINT32                Lane,
  OUT UINT64_STRUCT *const  VolatileMask
  );

/**
  Function used to get the CR Offset for registers listed under Comp partition.

  @param[in]  MrcData      - Global MRC data structure.
  @param[in]  Group        - DDRIO group being accessed.
  @param[in]  Lane         - Lane index within the data group (0-based).
  @param[in]  FreqIndex    - Workpoint Index.
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @retval CR Offset.
**/
UINT32
GetDdrIoCompOffsets (
  IN  MrcParameters         *MrcData,
  IN  GSM_GT                Group,
  IN  UINT32                Lane,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT *const  VolatileMask
  );

/**
  Function used to get the CR Offset for registers listed under DATA_SHAREDx / DATA_SBMEMx partitions.

  @param[in]  MrcData      - Global MRC data structure.
  @param[in]  Group        - DDRIO group being accessed.
  @param[in]  Index        - # of field.
  @param[in]  Lane         - Uses the Lane parameter from GetSet to select which PI Mixer code to access.
  @param[in]  FreqIndex    - Workpoint Index.
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR.
**/
UINT32
GetDdrIoDataOffsets (
  IN  MrcParameters* MrcData,
  IN  GSM_GT                Group,
  IN  UINT32                Index,
  IN  UINT32                Lane,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT* const  VolatileMask
  );

/**
  Function used to get the CR Offset for registers listed under CCC_SHAREDx partitions.

  @param[in]  MrcData      - Global MRC data structure.
  @param[in]  Group        - DDRIO group being accessed.
  @param[in]  Index        - # of field.
  @param[in]  Lane         - Uses the Lane parameter from GetSet to select which PI Mixer code to access.
  @param[in]  FreqIndex    - Access Index.
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR.
**/
UINT32
GetDdrIoCccOffsets (
  IN  MrcParameters         *MrcData,
  IN  GSM_GT                Group,
  IN  UINT32                Index,
  IN  UINT32                Lane,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT *const  VolatileMask
  );

/**
  Function used to get the CR Offset for registers listed under PGx/VCCCLKx/PLL/DATA_SHAREDx/CCC_SHAREDx partitions.

  @param[in]  MrcData      - Global MRC data structure.
  @param[in]  Group        - DDRIO group being accessed.
  @param[in]  Set          - Set index being targeted
  @param[in]  SubGroup     - SubGroup being targeted
  @param[in]  Lane         - Lane index within the data group (0-based).
  @param[out] VolatileMask - Mask indicating which bits are volatile in register

  @returns the offset of the CR.
**/
UINT32
GetDdrIoPartitionOffsets (
  IN  MrcParameters         *MrcData,
  IN  GSM_GT                Group,
  IN  UINT32                Set,
  IN  UINT32                SubGroup,
  IN  UINT32                Lane,
  IN  UINT32                FreqIndex,
  OUT UINT64_STRUCT *const  VolatileMask
  );

/**
  This function does PHCLK DCC sub function algorithm for fixing Min/Max outliers

  @param[in, out]     MrcData - Include all MRC global data.
  @param[in]          DccStepSize - The DCC Step Size
  @param[in]          PhData - Array indicating the amount of phase spacing between clocks
  @param[in, out]     DelayOffset - The PH Delay array offsets for alignment
  @param[in]          NumPh - The Number of Phases
  @param[in]          DebugPrint  - Print debug messages or not

  @retval mrcSuccess if DCC converges
  @retval mrcFail otherwise
**/
MrcStatus
PhaseWacAMoleAlgo (
  IN OUT MrcParameters *const MrcData,
  IN INT32       DccStepSize,
  IN UINT32      PhData[NUM_PHASES],
  IN OUT INT32   DelayOffset[NUM_PHASES],
  IN INT32       NumPh,
  IN BOOLEAN     DebugPrint
  );


/**
  This function does the refpi control register init.

  @param[in]  MrcData - Pointer to MRC global data.
**/
VOID
RefPiControlInit (
  IN  MrcParameters *const MrcData
  );

/**
  This function program OffsetCalCnt.

  @param[in, out] MrcData - MRC global data.
**/
VOID
MrcProgramOffsetCalCnt (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function program MISCCONTROL2.

  @param[in, out] MrcData - MRC global data.
  @param[in]      Restore - MRC global data.
  @param[in, out] OrigMiscWckControl - The pointer to the data to be read.
**/
VOID
MrcProgramMiscWckControl (
  IN OUT MrcParameters *const MrcData,
  IN     BOOLEAN        Restore,
  IN OUT UINT32         *OrigMiscWckControl
  );

/**
  This function sets/clears PI DCC FSMs

  @param[in] MrcData    - Include all MRC global data.
  @param[in] EnDis      - Enable/Disable

**/
VOID
EnDisWckPiDccFsms (
  IN MrcParameters *const MrcData,
  IN BOOLEAN              EnDis
  );

/**
  This function calculates then returns DCC step size

  @param[in, out] MrcData - Include all MRC global data.
  @param[in] Ave1 - First data average, must be less than Ave2
  @param[in] Ave2 - Second data average, must be greater than Ave1
  @param[in] Divisor - Divides the subtraction of the averages.
  @param[in] Max - We expect it to be a positive value. Maximum number the retvalue can be.
  @param[in] Steps - We expect it to be a positive value. Number of steps iterated.
  @param[in] DivisorFactor - divisor factor. tick code delta * number of samples.

  @retval Dcc step size. Calculated by dividing the difference between Ave2 and Ave1, then adding half
  of the former to our DCC step size formula, dividing it by the full value and constraining that
  result in a certain range.
  We have it as an INT64 to prevent typecasting for GetSets, although it should be positive in this function.
**/
INT64
MrcDccStepSizeFormula (
  IN     MrcParameters* MrcData,
  IN     UINT32  Ave1,
  IN     UINT32  Ave2,
  IN     UINT32  Divisor,
  IN     INT64   Max,
  IN     UINT16  Steps,
  IN     INT64   DivisorFactor
  );

/**
  This function verifies if the DCC step size is within the expected limits.
  Flags en error if the limit check fails.

  @param[in] MrcData         - Include all MRC global data.
  @param[in] DccStepSize     - DCC step size
  @param[in] DCCSteps        - DCC code delta

  @retval    TRUE if limit check failed
  @retval    FALSE if limit check passed
**/
BOOLEAN
MrcDccStepSizeLimitCheckFail (
  IN  MrcParameters* const MrcData,
  IN  INT64       DccStepSize,
  IN  UINT16      DCCSteps
  );

/**
  This function writes to critical DCC Controls regardless of population.

  @param[in] MrcData        - Include all MRC global data.
  @param[in] Controller     - Controller to work on.
  @param[in] Channel        - Channel to work on.
  @param[in] SubChBitmap    - Bitmap of Sub Channels for LPDDR5.
  @param[in] IsEnableBitmap - If TRUE, OR bit from SubChannel to LaneEn register field
**/
void
MrcEnableDisableWckRankLaneEn (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT8                SubChBitmap,
  IN BOOLEAN              IsEnableBitmap
  );

/**
  This function calculates the WCK DCC StepSize.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] UpdateTco  - Tco value to be updated in the register.

  @retval mrcSuccess if DCC converges otherwise mrcFail.
**/
MrcStatus
MrcCalcWckDccStepSize (
  IN MrcParameters *const MrcData,
  IN UINT8                UpdateTco
  );

/**
  Program given WCK DCC Offset in DATA_SHARED

  @param[in] MrcData - Include all MRC global data.
  @param[in] Offset  - WCK DCC Offset
**/
VOID
MrcSetWckDccOffset (
  IN MrcParameters* const MrcData,
  IN INT64                Offset,
  IN UINT8                UpdateTco
  );

/**
  This function gets the LaneEn value for data partition.

  @param[in] MrcData    - Include all MRC global data.

  @returns LaneEn value
**/
UINT32
MrcGetDataLaneEn (
  IN OUT MrcParameters *const MrcData
  );

/**
  Enables / Disables Ddr5Pll.

  @param[in] MrcData - Include all MRC global data.
  @param[in] Enable  - BOOLEAN control to enable (if TRUE), or disable (if FALSE) Ddr5Pll.
**/
VOID
MrcUpdateDdr5Pll (
  IN MrcParameters *const MrcData,
  IN BOOLEAN        const Enable
  );

/**
  This function does Rload compensation to calibrate the rload template pulldown resistance.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval MrcStatus
**/
MrcStatus
MrcRloadCompensation (
  IN MrcParameters* const MrcData
  );

/**
  This function sums up the differences between Duty Cycle Accumulator Results
  and then calculates Step Size for both PLL Partition and PLL Partition Phase.

  @param[in]      MrcData          - Include all MRC global data.
  @param[in]      ResultTotal      - Total phase result from Negative Duty Cycle Accumulator Results
  @param[in, out] ResultTotal1     - Total phase result from Positive Duty Cycle Accumulator Results
  @param[out]     DCCStepSizePll   - The DCC Step Size for PLL Partition
  @param[out]     PhaseStepSizePll - The DCC Step Size for PLL Partition Phase

  @retval mrcFail if the Sum of the Total Phase Results < 0 else mrcSuccess
**/
MrcStatus
MrcDccTlineCalcStepSize (
  IN     MrcParameters *const MrcData,
  IN     INT64                ResultTotal[NUM_PLL],
  IN OUT INT64                ResultTotal1[NUM_PLL],
  OUT    INT64                *DCCStepSizePll,
  OUT    INT64                *PhaseStepSizePll
  );


/**
  This function configures the DDRIO to drive WCK according to the JEDEC Spec before Entering, or
  after exiting Command Bus Training Mode.

  For Entry, WCK will be driven static low while idle and prepare for WCK to toggle indefinitely when the pulse is started.
  For Exit, the pulse will be stopped; WCK force to idle will be removed allowing to return back to Tristate; and the pulse
    configuration will be cleared.

  @param[in] MrcData - Pointer to global MRC data.
  @param[in] IsCbtEnter - Defines if the call is for a CBT enter (TRUE) or CBT exit (FALSE)
**/
VOID
MrcDdrIoLpddrCbtWckMode (
  IN MrcParameters* const MrcData,
  IN BOOLEAN              IsCbtEnter
  );

/**
  Enable running Voc Search in seseamp offset training or not

  @param[in] MrcData    - Include all MRC global data.

  @returns TRUE or FALSE
**/
BOOLEAN
EnableSenseAmpOffsetVocSearch (
  IN OUT MrcParameters *const MrcData
  );

/**
  This configures DDRIO registers required for DDRIO Init Complete on Green MRC Fast Boot.

  @param[in, out] MrcData - MRC global data.

  @retval MrcStatus - mrcSuccess if successful or an error status.
**/
MrcStatus
MrcDdrIoFastEnable (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function programs or saves the DqsRFTrainingMode for sense amp training.

  @param[in]  MrcData     - Pointer to MRC global data.
  @param[in]  UpdateMode  - It's to save or update the value.
  @param[in]  Mode        - Get Set Mode.
  @param[in out]  Value   - The value to be saved or programmed for DqsRFTrainingMode.

  @retval None
**/
VOID
MrcGetSetDqsRFTrainingMode  (
  IN  MrcParameters *const MrcData,
  IN  UPDATE_MODE          UpdateMode,
  IN  UINT32               Mode,
  IN OUT INT64             *Value
);

/**
  This function configures and prints PHY registers for DQ Loopback test

  @param[in] MrcData - Pointer to MRC global data.
**/
VOID
MrcDqLoopbackTestPhySetup (
  IN MrcParameters *const MrcData
  );

/**
 Check if Dqs DCC Lane Results are required to be printed.

  @param[in] MrcData - Pointer to MRC global data.

  @retval TRUE if Dqs DCC Lane Results are required to be printed.
**/
BOOLEAN
MrcCheckIfPrintDqsLaneResultsRequired (
  IN MrcParameters *const MrcData
  );

/**
  Program given GetSet Group

  @param[in] MrcData        - Include all MRC global data.
  @param[in] PartitionBlock - Which Block set of DCC registers (PLL, PG, VCCCLK) to access.
  @param[in] BlockIndex     - Which Index of Block to access (Passed on as Lane parameter).
  @param[in] Group          - DDRIO group to access.
  @param[in] Mode           - Get Set Mode.
  @param[in] GetSetVal      - The pointer to the value to be programmed.
**/
VOID
MrcGetSetPartitionBlockWrapper (
  IN MrcParameters* const MrcData,
  IN UINT32               PartitionBlock,
  IN UINT32               BlockIndex,
  IN GSM_GT        const  Group,
  IN UINT32               Mode,
  IN INT64                *GetSetVal
  );

/**
  Top level wrapper function used to interact with SOC. It's a wrapper of MrcGetSetChStrb.
  This function ignores unused parameters in Core MRC to reduce code space.
  This function is for those registers that are only per Strobe per Channel granularity.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      Strobe      - Dqs data group within the rank (0-based).
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
VOID
MrcGetSetChStrbWrapper (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   Strobe,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
  );

/**
   Top level wrapper function used to interact with SOC. It's a wrapper of MrcGetSetCcc.
  This function ignores unused parameters in Core MRC to reduce code space.
  This function is used to access indexed Command/Control/Clock groups.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      Rank        - Rank within the DDR Channel (0-based).
  @param[in]      Index       - Group index to Get/Set.
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
VOID
MrcGetSetCccWrapper (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   Rank,
  IN      UINT32        const   Index,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
  );

/**
  This function takes the offset to determine if a rank is populated.

  @param[in, out] MrcData - Include all MRC global data.
  @param[in]      Offset  - Offset to be checked.

  @retval TRUE if rank is populated on associated offset
          FALSE if rank is not populated on associated offset

**/
BOOLEAN
MrcClockEnable (
  IN OUT MrcParameters *const MrcData,
  IN     UINT32  Offset
  );

/**
  Returns whether PHY Clock associated with given Controller,
  Channel, Rank is enabled or not.

  @param[in] MrcData    - Pointer to MRC global data.
  @param[in] Controller - Controller to test.
  @param[in] Channel    - Channel to test.
  @param[in] Rank       - Rank to test.

  @retval BOOLEAN - TRUE if enabled, FALSE otherwise.
**/
BOOLEAN
MrcPhyClockExists (
  IN MrcParameters *const MrcData,
  IN UINT32         const Controller,
  IN UINT32         const Channel,
  IN UINT32         const Rank
  );

/**
  Configuring the bit channel_not_populated.

  @param[in] MrcData - Pointer to MRC global data
  @return VOID
**/
VOID
MrcSetChannelNotPopulated (
  MrcParameters * MrcData
);

/**
  The function programs Rx VREF training result.

  @param[in, out] MrcData          - Include all MRC global data.
  @param[in]      Controller       - Memory controller
  @param[in]      Channel          - Channel number
  @param[in]      Byte             - Byte number
  @param[in]      Mode             - Get Set Mode will be used in the GetSet function
  @param[in]      SweepResultsBit  - Structure to store liner search results on the Bit level

**/
void
MrcProgramRxVrefTrainingResult (
  IN OUT MrcParameters *const MrcData,
  IN UINT8                    Controller,
  IN UINT8                    Channel,
  IN UINT8                    Byte,
  IN UINT8                    Mode,
  IN SweepResultsBit   *const SweepResultBit
  );

/**
  This function programs the current MRC post code to PHYPMSTATUS1 register field.

  @param[in]  MrcData  - Include all MRC global data.
  @param[in]  PostCode - MRC post code to be programmed

**/
VOID
MrcProgramPostCode (
  IN MrcParameters* const MrcData,
  IN INT64          PostCode
  );

/**
  This function sets DDRIO IP Version, Derivative, Segment, Stepping based on DDRPHY_MISC_SAUG_CR_IP_VERSION_REG.

  @param[in]  MrcData     - Include all MRC global data.

  @retval Nothing
**/
MrcStatus
MrcSetupDdrIoIpInfo (
  OUT MrcParameters *const MrcData
  );

/**
  The function gets the getset Rx VREF per byte group enum

  @retval return the getset Rx VREF per byte group enum

**/
GSM_GT
MrcGetRdVPerByteGroup (
    );

/**
  Get the max CMD Groups per channel associated with the current memory technology

  @param[in] MrcData  - Pointer to global MRC data.

  @return The maximum number of CMD Groups per channel for the current memory technology
**/
UINT8
MrcGetCmdGroupMax (
  MrcParameters *const MrcData
);

/**
  This function updates CBMix and CBMux based on the PHClk ratio locked

  @param[in, out] MrcData - MRC global data.
**/
VOID
CbMixMuxConfig (
  IN OUT MrcParameters *const MrcData
  );

#endif //MRC_DDR_IO_API_H_
