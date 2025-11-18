/** @file
  This file contains code related to initializing and configuring the DDRIO.

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

**/


#include "MrcCommon.h"
#include "MrcDdrIoUtils.h"  // for prototypes
#include "MrcDebugPrint.h"  // for MRC_DEBUG_MSG()
#include "MrcHalApi.h"  // for MrcGetSetChStrb()
#include "MrcDdr5.h"  // for MrcSetDdr5Mr48()
#include "MrcDdrIoApi.h"  // for MrcGetDataOffset()
#include "MrcLpddr5.h" // for MrcGetWckPreRdTotalLpddr5()
#include "MrcDdrIoRefPi.h"

UINT8 AuxClkRef100[] = { 33,  33,  27,  28,  27,  30,  33,  28,  33,  32,  33,  32,  32,  33,  36,  36, //  0 .. 15
                         39,  33,  42,  39,  42,  44,  44,  42,  53,  44,  45,  42,  51,  48,  52,  40, // 16 .. 31
                         54,  52,  52,  57,  47,  45,  56,  52,  57,  63,  64,  60,  66,  66,  68,  69, // 32 .. 47
                         69,  66,  66,  68,  69,  69,  76,  63,  78,  78,  30,  75,  80,  76,  78,  78, // 48 .. 63
                         78,  81,  87,  88,  90,  84,  84,  92,  87,  87,  88,  92,  88,  93,  92,  84, // 64 .. 79
                         93,  92,  63,  99, 105, 100, 100, 102, 102, 104, 116,  78, 114, 120, 114, 108, // 80 .. 95
                        103, 111, 111, 112, 126, 114, 128, 117, 105, 144, 144, 120, 135,  90, 120, 124, // 96 .. 111
                        123, 141, 123, 126, 136, 129, 138, 129, 105, 132, 132, 135, 132, 136, 136, 138, // 112 .. 127
                        136, 140, 138, 141, 141, 144, 141, 144, 144, 108, 100, 132, 135, 111, 111, 135, // 128 .. 143
                        104, 114, 140, 138, 114, 140, 144, 117, 141, 120, 132, 120, 136, 123, 120, 123, // 144 .. 159
                        123, 124,  68, 126, 126, 128, 126, 129, 129};


const INT8 DdrioChDelta[MAX_SYS_CHANNEL] = {0, 0, -1, -1, -2, -2, -1, -1};
const INT8 DdrioChDeltaCccIL[MAX_SYS_CHANNEL] = {0, 0, 0, 0, -1, -1, -1, -1};

static const UINT8 HwChBitMasks[][MAX_MRC_DDR_TYPE] = {
    [ipDerivativePtlx64] = {
      [MRC_DDR_TYPE_LPDDR5] = 0x0F,
      [MRC_DDR_TYPE_DDR5]   = 0x03,
    },
    [ipDerivativePtl] = {
      [MRC_DDR_TYPE_LPDDR5] = 0xFF,
      [MRC_DDR_TYPE_DDR5]   = 0x0F,
    },
  };


#define DDR5CATMLEFTRANGE    (128)  ///< DDR5 CA Training Mode Phase 1 Sweep Range
#define DDR5CATMRIGHTRANGE   (96)   ///< DDR5 CA Training Mode Phase 1 Sweep Range

/**
  This function programs the WorkPoint CR, including enforcing any fuse limits to avoid HW from NACKing PLLLock Requests.

  @param[in, out] MrcData - MRC global data.

  @retval MrcStatus - mrcSuccess if Workpoint CR is set correctly, otherwise mrcFail.
**/
MrcStatus
MrcSetWorkpointCR (
  IN OUT MrcParameters *const MrcData
  )
{
  return mrcSuccess;
}

/**
  This function sets workpoint data including Qclk ratio, Gear, and AuxClk ratio

  @param[in, out] MrcData - MRC global data.
**/
VOID
MrcSetWorkPointDataForPhy (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcOutput         *Outputs;
  INT64             GetSetVal;
  UINT32            AuxClk;
  MrcClockRatio     Ratio;
  MrcSaGvPoint      SaGvPoint;

  Outputs   = &MrcData->Outputs;
  SaGvPoint = Outputs->SaGvPoint;

  // Configure work point data
  Ratio = MrcFrequencyToRatio (MrcData, Outputs->Frequency);
  Ratio =  Outputs->GearMode ? (Ratio / 4) : (Ratio / 2);
  GetSetVal = (INT64) Ratio;
  MrcGetSetFreqIndex (MrcData, SaGvPoint, GsmWorkPointRatio, WriteToCache | PrintValue, &GetSetVal);
  GetSetVal = (INT64) SaGvPoint;
  MrcGetSetNoScope (MrcData, GsmMiscPmMessageWpSel, WriteNoCache | PrintValue, &GetSetVal);
  AuxClk = GetAuxClkRatio (MrcData, Ratio);
  GetSetVal = (INT64) AuxClk;
  MrcGetSetFreqIndex (MrcData, SaGvPoint, GsmWorkPointAuxClk, WriteToCache | PrintValue, &GetSetVal);
  GetSetVal = Outputs->GearMode ? 1 : 0;
  MrcGetSetFreqIndex (MrcData, SaGvPoint, GsmWorkPointGear4, WriteToCache | PrintValue, &GetSetVal);

  MrcFlushRegisterCachedData (MrcData);
}

/**
  This function calculates the AuxClkRatio based on QClkRatio.

  @param[in, out] MrcData - All the MRC global data.
  @param[in]      Ratio33 - QClkRatio

  @return Returns AuxClk ratio.
**/
UINT32
GetAuxClkRatio (
  IN MrcParameters *const MrcData,
  IN UINT32               Ratio33
  )
{
  MrcOutput      *Outputs;
  BOOLEAN        Gear4;
  UINT32         AuxClkRatio33;
  UINT32         QClkRatio;

  Outputs     = &MrcData->Outputs;
  Gear4       = Outputs->GearMode ? 1 : 0;

  QClkRatio = Ratio33 * (1 + Gear4);

  if (QClkRatio < 24) {
    QClkRatio = 24;
  }

  AuxClkRatio33  = AuxClkRef100[QClkRatio - 24];

  return AuxClkRatio33;
}


/**
  Calculates sweeping range for CATM based on min/max values of CtlPiCode.

  @param[in]  MinPiCode - Min CTL PI accross all channels.
  @param[in]  MaxPiCode - Max CTL PI accross all channels.
  @param[out] PiLow     - Starting PI code value for CATM.
  @param[out] PiHigh    - Ending PI code value for CATM.

  @returns nothing.
**/
VOID
MrcGetCatmSweepingRange (
  IN  UINT16 MinPiCode,
  IN  UINT16 MaxPiCode,
  OUT UINT16 *PiLow,
  OUT UINT16 *PiHigh
  )
{
  // Calculate the sweep range: -128 PI from (min), +96 PI from (max)
  // Limit PiLow to MAX(0, min - 128)
  // Limit PiHigh to MIN(512, max + 96)
  if (PiLow != NULL) {
    *PiLow  = (MinPiCode < DDR5CATMLEFTRANGE) ? 0 : (MinPiCode - DDR5CATMLEFTRANGE);
  }
  if (PiHigh != NULL) {
    *PiHigh = (MaxPiCode > (DDR5CATMMAXPI - DDR5CATMRIGHTRANGE)) ? DDR5CATMMAXPI : (MaxPiCode + DDR5CATMRIGHTRANGE);
  }
}


/*
  Update the requested Param offset with the given Value,
  using direct multicast CR access for speed.

  @param[in]  MrcData - Global MRC data structure
  @param[in]  Rank    - Target rank
  @param[in]  Param   - Margin param
  @param[in]  Value   - The value to program
*/
VOID
MrcWriteDirectMulticast (
  IN MrcParameters *const MrcData,
  IN UINT32               Rank,
  IN UINT8                Param,
  IN INT32                Value
  )
{
  BOOLEAN UpdateDataOffsetTrain = FALSE;
  DATA0CH0_CR_DDRCRADC_STRUCT DdrCrAdc;
  DATA0CH0_CR_TXCONTROL0RANK0_STRUCT      TxControl0;
  DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_STRUCT DataOffsetTrain;

  DataOffsetTrain.Data = 0;
  // Pre-populate the RdT offset with the base offset
  // ChangeMargin may offset it with a given Value below, but for other params we keep the base RdT offset.
  // ChangeMargin is never used to modify multiple DataOffsetTrain params together.
  DataOffsetTrain.Bits.RxDqsOffset = MrcData->Save.Data.RxDqsBaseOffset;

  switch (Param) {
    case RdV:
      DdrCrAdc.Data = DATA0CH0_CR_DDRCRADC_DEF;
      DdrCrAdc.Bits.VrefOffset = Value;
      MrcWriteCrMulticast (MrcData, DATA_CR_DDRCRADC_REG, DdrCrAdc.Data);
      break;

    case RcvEna:
      DataOffsetTrain.Bits.RcvEnOffset = Value;
      UpdateDataOffsetTrain = TRUE;
      break;

    case RdT:
    case RdTN:
    case RdTP:
      DataOffsetTrain.Bits.RxDqsOffset += Value;  // Added on top of base offset
      UpdateDataOffsetTrain = TRUE;
      break;

    case WrDqsT:
      DataOffsetTrain.Bits.TxDqsOffset = Value;
      UpdateDataOffsetTrain = TRUE;
      break;

    case WrT:
      DataOffsetTrain.Bits.TxDqOffset = Value;
      UpdateDataOffsetTrain = TRUE;
      break;

    case WrTUnMatched:
      // Only LP5 is supported; preserving TxDqsDelay value which is set in Loopback mode init
      // In DDR5 this param is a function of TxDqsDelay, which is trained per byte in JWL, hence cannot use multicast
      if (Rank == 0) {
        TxControl0.Data = MrcReadCR (MrcData, DATA0CH0_CR_TXCONTROL0RANK0_REG);
        TxControl0.Bits.TxDqDelay  = Value;
        MrcWriteCrMulticast (MrcData, DATA_CR_TXCONTROL0RANK0_REG, TxControl0.Data);
      } else {
        TxControl0.Data = MrcReadCR (MrcData, DATA0CH0_CR_TXCONTROL0RANK1_REG);
        TxControl0.Bits.TxDqDelay  = Value;
        MrcWriteCrMulticast (MrcData, DATA_CR_TXCONTROL0RANK1_REG, TxControl0.Data);
      }
      break;

    default:
      break;
  }

  if (UpdateDataOffsetTrain) {
    MrcWriteCrMulticast (MrcData, DATA_CR_DDRCRDATAOFFSETTRAIN_REG, DataOffsetTrain.Data);
  }
}

/*
  IsDataPopulated

  @param[in]  MrcData       - Global MRC data structure.
  @param[in]  PartInst       - Partition instance

  Returns True if a data partition instance is populated, otherwise False
*/
BOOLEAN
IsDataPopulated (
  IN MrcParameters* const MrcData,
  IN UINT8                PartInst
  )
{
  return TRUE;
}

/*
  IsCCCPopulated

  @param[in]  MrcData       - Global MRC data structure.
  @param[in]  Instance       - Partition instance

  Returns True if a CCC partition instance is populated, otherwise False
*/
BOOLEAN
IsCCCPopulated (
  IN MrcParameters* const MrcData,
  IN UINT8                Instance
  )
{
  return TRUE;
}

/*
  IsPartPopulated

  @param[in]  MrcData       - Global MRC data structure.
  @param[in]  PartName       - Data/CCC/Comp.
  @param[in]  Instance       - Partition instance

  Returns True if partName and partInst is populated
*/
BOOLEAN
IsPartPopulated (
  IN MrcParameters* const MrcData,
  IN RAIL_PARTTION     PartName,
  IN UINT8             Instance
  )
{
  return TRUE;
}

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
  )
{
}

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
  )
{

  return 0;
}

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
  )
{

  return mrcSuccess;
}

/*
  Setup LVR Registers

  @param[in]  MrcData       - Global MRC data structure.
*/
VOID
WriteAllLvrTarget (
  IN MrcParameters* const MrcData
  )
{
}

/**
  This function calculates QClkRatio.

  @param[in] MrcData  - Include all MRC global data.

  @return Returns QClkRatio.
**/
UINT32
MrcCalcRatio33 (
  IN MrcParameters* const MrcData
  )
{
  MrcOutput    *Outputs;
  BOOLEAN      IsGear4;
  MrcFrequency DdrFrequency;
  UINT32       QClkRatio33;

  Outputs         = &MrcData->Outputs;
  IsGear4         = Outputs->GearMode ? 1 : 0;
  DdrFrequency    = Outputs->Frequency;

  QClkRatio33     = DdrFrequency * 3;
  QClkRatio33     = UDIVIDEROUND (QClkRatio33, 100);
  QClkRatio33     /= (IsGear4 ? 4 : 2);

  return QClkRatio33;
}

/**
  This function updates the DLL VCDL DCC Code

  @param[in, out] MrcData               - Pointer to MRC global data.
  @param[in]      Init                  - Init or not.

**/
VOID
DllVcdlDccCodeUpdate (
  IN OUT MrcParameters *const MrcData,
  IN     BOOLEAN       Init
  )
{
  UINT32    ChIdx;
  UINT32    DataIdx;
  UINT32    Offset;
  DATA0CH0_CR_DDRCRDLLCONTROL0_STRUCT  DllControl0;

  for (DataIdx = 0; DataIdx < MRC_DATA_MOBILE_NUM; DataIdx++) {
    if (!(MrcGetHwPartitionExists (MrcData, PartitionDataShared, DataIdx, MRC_IGNORE_ARG))) {
      continue;
    }
    for (ChIdx = 0; ChIdx < MRC_DATA_CH_NUM; ChIdx++) {
      Offset = MrcGetDataOffset (MrcData, DATA0CH0_CR_DDRCRDLLCONTROL0_REG, MRC_IGNORE_ARG, ChIdx, DataIdx);
      DllControl0.Data = MrcReadCR (MrcData, Offset);
      if (Init) {
        DllControl0.Bits.DllVcdlDccCodeEn = 1;
        DllControl0.Bits.DllVcdlDccCode   = 0; // set DCC code to default value center of [-8,7] range
      } else {
        DllControl0.Bits.DllVcdlDccCodeEn = 0;
      }
      MrcWriteCR (MrcData, Offset, DllControl0.Data);
    }
  }
}

/**
  This function updates the DATA BUFFER BONUS PER BIT

  @param[in, out] MrcData               - Pointer to MRC global data.
  @param[in]      Set                   - Set or clear bit 3

**/
VOID
DataBufferBonusPerBitUpdate (
  IN OUT MrcParameters *const MrcData,
  IN     BOOLEAN       Set
  )
{
  DDRDATA_SHARED0_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT Byte0DataBufferBonusPerBit;
  DDRDATA_SHARED0_CR_DDRCRBYTE1DATABUFFERBONUSPERBIT0_STRUCT Byte1DataBufferBonusPerBit;

  Byte0DataBufferBonusPerBit.Data = MrcReadCR (MrcData, DDRDATA_SHARED0_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_REG);
  if (Set) {
    Byte0DataBufferBonusPerBit.Data |= 0x88888888; // Set bit 3 of each 4-bit bonus field for each lane
  } else {
    Byte0DataBufferBonusPerBit.Data &= 0x77777777; // Clear bit 3 of each 4-bit bonus field for each lane
  }
  MrcWriteCrMulticast (MrcData, DATASHARED_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_REG, Byte0DataBufferBonusPerBit.Data);

  Byte1DataBufferBonusPerBit.Data = MrcReadCR (MrcData, DDRDATA_SHARED0_CR_DDRCRBYTE1DATABUFFERBONUSPERBIT0_REG);
  if (Set) {
    Byte1DataBufferBonusPerBit.Data |= 0x88888888; // Set bit 3 of each 4-bit bonus field for each lane
  } else {
    Byte1DataBufferBonusPerBit.Data &= 0x77777777; // Clear bit 3 of each 4-bit bonus field for each lane
  }
  MrcWriteCrMulticast (MrcData, DATASHARED_CR_DDRCRBYTE1DATABUFFERBONUSPERBIT0_REG, Byte1DataBufferBonusPerBit.Data);
}

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
  )
{
  DDRSCRAM_CR_DDRMISCCONTROL2_STRUCT MiscControl2;
  MCMISCS_DDRWCKCONTROL_STRUCT WckControl;

  WckControl.Data = MrcReadCR (MrcData, MCMISCS_DDRWCKCONTROL_REG);
  MiscControl2.Data = MrcReadCR (MrcData, DDRSCRAM_CR_DDRMISCCONTROL2_REG);

  if (IsCbtEnter) {
    // Ungate WckDiffLowInIdle
    MiscControl2.Bits.TrainWCkEn = 1;
    MrcWriteCR (MrcData, DDRSCRAM_CR_DDRMISCCONTROL2_REG, MiscControl2.Data);

    //Set WCK Control for BL/Mask and DiffLowInIdle for training
    WckControl.Bits.TrainWCkBL = 0;
    WckControl.Bits.TrainWCkMask = 7;
    WckControl.Bits.WCkDiffLowInIdle = 1;
    MrcWriteCR (MrcData, MCMISCS_DDRWCKCONTROL_REG, WckControl.Data);
  } else {
    // Disable Diff Low In Idle & Clear BurstLength/Mask. Previous read will have TrainWCkPulse set.
    // It needs to be 0 here after we drive DQ7 low to not leave it on after completing training.
    WckControl.Bits.TrainWCkBL = 0;
    WckControl.Bits.TrainWCkMask = 0;
    WckControl.Bits.WCkDiffLowInIdle = 0;
    WckControl.Bits.TrainWCkPulse = 0;
    MrcWriteCR (MrcData, MCMISCS_DDRWCKCONTROL_REG, WckControl.Data);

    MiscControl2.Bits.TrainWCkEn = 0;
    MrcWriteCR (MrcData, DDRSCRAM_CR_DDRMISCCONTROL2_REG, MiscControl2.Data);
  }
}

/**
  This function does the refpi control register init.

  @param[in]  MrcData - Pointer to MRC global data.

**/
VOID
RefPiControlInit (
  IN  MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs;
  MrcDebug *Debug;
  BOOLEAN IsGear4;
  CCC_CR_REFPICONTROL_STRUCT RefPiControl;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  IsGear4 = Outputs->GearMode;

  // CTE requires different initial values when running RefPi Calibration
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Using CTE initial values for RefPi\n");
  RefPiControl.Data = 0;
  RefPiControl.Bits.RefPi4Xover       = 0;
  RefPiControl.Bits.RefPi             = 0;
  RefPiControl.Bits.RefPi4XoverOffset = 0x20;
  RefPiControl.Bits.RefPiOffset       = IsGear4 ? 0x80 : 0x40;
  MrcWriteCrMulticast (MrcData, CCC_CR_REFPICONTROL_REG, RefPiControl.Data);
  MrcWriteCrMulticast (MrcData, DATA_CR_REFPICONTROL_REG, RefPiControl.Data);
}

/**
  This function initializes the RefPiFsmControl register for DDRIO.
  All partitions which have RefPiFsmControl have the same structure which allows for a single init function.

  @param[in] MrcData   - Pointer to global MRC data.
  @param[in] FsmStage  - FSM stage
  @param[in] Offset    - Partiton offset for restoring previous values
  @param[in] PrintTask - Qualifier to include print statemenets

  @return UINT32 - Initialized value of RefPiFsmControl Register
**/
UINT32
MrcDdrIoInitRefPiFsmCtl (
  IN MrcParameters* const MrcData,
  IN MRC_REFPI_CAL_STAGE  FsmStage,
  IN UINT32               Offset,
  IN BOOLEAN              PrintTask
  )
{
  MrcDebug  *Debug;
  UINT16    MinWidth;
  UINT16    MaxWidth;
  UINT32    PrintLevel;
  BOOLEAN   IsGear4;
  BOOLEAN   IsCccGear4;
  DATA0CH0_CR_REFPIFSMCONTROL_STRUCT  RefPiFsmControl;
  MrcInput  *Inputs;

  Inputs     = &MrcData->Inputs;
  Debug      = &MrcData->Outputs.Debug;
  IsGear4    = MrcData->Outputs.GearMode;
  IsCccGear4 = (BOOLEAN) MrcData->Inputs.ExtInputs.Ptr->CccHalfFrequency;
  PrintLevel = PrintTask ? MSG_LEVEL_ALGO : MSG_LEVEL_NONE;
  if (Inputs->IsDdrIoMbA0) {
    MinWidth = 4;
  } else {
    MinWidth = 6;
  }
  MaxWidth   = IsGear4 ? 256 : 128;

  if (FsmStage & MrcRefPi2XOverAlign) {
    RefPiFsmControl.Data = MrcReadCR (MrcData, Offset);
  } else {
    RefPiFsmControl.Data = 0;
    RefPiFsmControl.Bits.TestCycle = 6;   // take 2^6 samples per PI tick
    RefPiFsmControl.Bits.MinWidth = MinWidth;
    RefPiFsmControl.Bits.MaxVal   = MaxWidth + (2 * MinWidth);
    RefPiFsmControl.Bits.SaveModN = IsGear4 ? 2 : 1;
  }

  switch (FsmStage) {
    case MrcRefPi2QClkDataCcc:
      RefPiFsmControl.Bits.Param = 1;           // setup FSM to run RefPi Training
      RefPiFsmControl.Bits.WriteValParam = 0;
      MRC_DEBUG_MSG (Debug, PrintLevel, " > Edge align QClk to Refpi for %s", "Data/CCC partition");
      break;

    case MrcRefPi2XOverAlign:                   // RefPi4XOver Alignment
      RefPiFsmControl.Bits.MinVal = 0;
      RefPiFsmControl.Bits.MaxVal = (IsCccGear4 ? 256 : 128) + (2 * MinWidth);
      RefPiFsmControl.Bits.Param = 2;           // configure FSM to align RefPi4XOver to Refpi
      RefPiFsmControl.Bits.WriteValParam = 1;   // Automatically update Refpi4XOver field
      MRC_DEBUG_MSG (Debug, PrintLevel, " > Align RefPi4XOver to Refpi");
      break;

    case MrcRefPiDefault:                       // FSM disabled (not run for COMP partition)
    default:
      RefPiFsmControl.Bits.Param = 0;
      RefPiFsmControl.Bits.WriteValParam = 0;
      break;
  }

  MRC_DEBUG_MSG (Debug, PrintLevel, " (0x%x)\n", RefPiFsmControl.Data);
  return RefPiFsmControl.Data;
}

/**
  This function sets RefPiFsmControl to the correct value for input FSM stage.
  This is acieved via CR Multicast in the DDRIO.

  @param[in]  MrcData      - Pointer to MRC global data.
  @param[in]  FsmStage     - FSM calibration stage
**/
VOID
MrcDdrIoResetRefPiFsmCtl (
  IN  MrcParameters* const MrcData,
  IN  MRC_REFPI_CAL_STAGE  FsmStage
  )
{
  UINT32   Offset;
  UINT32   ChIdx;
  UINT32   Index;
  UINT32   DataIdx;
  UINT32   DataMax;
  CCC_CR_REFPIFSMCONTROL_STRUCT RefPiFsmCtrl;

  DataMax = MRC_DATA_MOBILE_NUM;

  // CCC Partitions
  for (Index = 0; Index < MRC_CCC_NUM; Index++) {
    if (!(MrcGetHwPartitionExists (MrcData, PartitionCcc, Index, MRC_IGNORE_ARG))) {
      continue;
    }
    Offset = OFFSET_CALC_CH (CH0CCC_CR_REFPIFSMCONTROL_REG, CH1CCC_CR_REFPIFSMCONTROL_REG, Index);
    RefPiFsmCtrl.Data = MrcDdrIoInitRefPiFsmCtl (MrcData, FsmStage, Offset, (Index==0 ? MRC_PRINT_CURRENT_TASK : MRC_SKIP_PRINT));
    MrcWriteCR (MrcData, Offset, RefPiFsmCtrl.Data);
  }

  // DATA Partitions
  for (DataIdx = 0; DataIdx < DataMax; DataIdx++) {
    if (!(MrcGetHwPartitionExists (MrcData, PartitionDataShared, DataIdx, MRC_IGNORE_ARG))) {
      continue;
    }
    for (ChIdx = 0; ChIdx < MRC_DATA_CH_NUM; ChIdx++) {
      Offset = MrcGetDataOffset (MrcData, DATA0CH0_CR_REFPIFSMCONTROL_REG, MRC_IGNORE_ARG, ChIdx, DataIdx);
      RefPiFsmCtrl.Data = MrcDdrIoInitRefPiFsmCtl (MrcData, FsmStage, Offset, (DataIdx==0 ? MRC_PRINT_CURRENT_TASK : MRC_SKIP_PRINT));
      MrcWriteCR (MrcData, Offset, RefPiFsmCtrl.Data);
    }
  }
}

/**
  This function accesses RefPi and RefP4Xover from the DDRIO based on the specific partition
  passed in ParInfo and stores the result in the output variable RefPiData.

  @param[in]  MrcData   - Pointer to global MRC data.
  @param[out] RefPiData - Pointer to the storage variable for RefPi and RefPi4Xover.
  @param[out] SaveCrPtr - Pointer to the storage variable for saved CR.
  @param[in]  Offset    - The register address offset to be read.

  @retval MrcStatus - mrcWrongInputParameter if RefPiData or ParInfo is NULL;
                    - mrcFail if the ParType in ParInfo isn't supported
                    - mrcSuccess otherwise.
**/
MrcStatus
MrcReadUpdateRefPiData (
  IN        MrcParameters             *MrcData,
  OUT       MRC_REFPI_DATA_TYPE       *RefPiData,
  OUT       UINT32                    *SaveCrPtr,
  IN        UINT32                     Offset
  )
{
  MrcStatus Status;
  DATA_CR_REFPICONTROL_STRUCT RefPiCtl;

  Status = mrcSuccess;
  // Read Data
  RefPiCtl.Data = MrcReadCR (MrcData, Offset);

  // Get RefPi and RefPi4Xover
  // Store in Output location.
  RefPiData->RefPi = (UINT8) RefPiCtl.Bits.RefPi;
  RefPiData->RefPiOffset = (UINT8) RefPiCtl.Bits.RefPiOffset;
  RefPiData->RefPi4Xover = (UINT8) RefPiCtl.Bits.RefPi4Xover;
  RefPiData->RefPi4XoverOffset = (UINT8) RefPiCtl.Bits.RefPi4XoverOffset;

  if (SaveCrPtr != NULL) {
    *SaveCrPtr = RefPiCtl.Data;
  } else {
    Status = mrcFail;
  }

  return Status;
}

/**
  This function program OffsetCalCnt.

  @param[in, out] MrcData - MRC global data.

**/
VOID
MrcProgramOffsetCalCnt (
  IN OUT MrcParameters *const MrcData
  )
{
  DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_STRUCT   PllFusePhClk;

  PllFusePhClk.Data = MrcReadCR (MrcData, DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_REG);
  PllFusePhClk.Bits.OffsetCalCnt = 7;
  MrcWriteCR (MrcData, DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_REG, PllFusePhClk.Data);
  MrcWriteCR (MrcData, DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEAUXCLK_REG, PllFusePhClk.Data);
}

/**
  This function program MISCCONTROL2.

  @param[in, out] MrcData - MRC global data.
  @param[in]      Restore - MRC global data.
  @param[in, out] OrigMiscWckControl - The data to be read.

**/
VOID
MrcProgramMiscWckControl (
  IN OUT MrcParameters *const MrcData,
  IN     BOOLEAN        Restore,
  IN OUT UINT32         *OrigMiscWckControl
  )
{
  MCMISCS_DDRWCKCONTROL_STRUCT                MiscWckControl;
  if (Restore) {
    MrcWriteCR (MrcData, MCMISCS_DDRWCKCONTROL_REG, *OrigMiscWckControl);
  } else {
    MiscWckControl.Data = (*OrigMiscWckControl) = MrcReadCR (MrcData, MCMISCS_DDRWCKCONTROL_REG);
    MiscWckControl.Bits.TrainWCkBL = DCC_RISE_FALL_WCK_BL;
    MiscWckControl.Bits.TrainWCkMask = DCC_RISE_FALL_WCK_MASK;
    MiscWckControl.Bits.TrainWCkPulse = DCC_RISE_FALL_WCK_PULSE;
    MiscWckControl.Bits.WCkDiffLowInIdle = 1;
    MrcWriteCR (MrcData, MCMISCS_DDRWCKCONTROL_REG, MiscWckControl.Data);
  }
}

/**
  This function configures the RefPI FSM control configuration for POR operation after calibration is complete.

  @param[in]  MrcData     - Pointer to global MRC data.

  @retval MrcStatus
**/
MrcStatus
MrcFinalizeRefPiFsmControl (
  IN  MrcParameters *const MrcData
  )
{
  MrcStatus Status;
  DATA_CR_REFPIFSMCONTROL_STRUCT  RefPiFsmCtl;

  Status = mrcSuccess;

  RefPiFsmCtl.Data = MrcDdrIoInitRefPiFsmCtl (MrcData, MrcRefPiDefault, MRC_REFPI_DEFAULT, MRC_PRINT_CURRENT_TASK);   // Reset FsmControl for CCC and DATA partitions
  MrcWriteCrMulticast (MrcData, CCC_CR_REFPIFSMCONTROL_REG, RefPiFsmCtl.Data);
  MrcWriteCrMulticast (MrcData, DATA_CR_REFPIFSMCONTROL_REG, RefPiFsmCtl.Data);

  return Status;
}

/**
  Setup Data Dcc Lane En.

  @param[in, out] MrcData       - Include all MRC global data.
  @param[in]      DataDccLaneEn -  The value for GsmDataDccLaneEn.

  @retval none
**/
VOID
MrcDataDccLaneEn (
  IN OUT MrcParameters *const   MrcData,
  IN     INT64                  DataDccLaneEn
  )
{
}

/**
  Setup Data Dcc Lane En.

  @param[in, out] MrcData         - Include all MRC global data.
  @param[in]      DccPHClkPhaseEn -  The value for GsmDccPHClkPhaseEn.

  @retval none
**/
VOID
MrcDccPHClkPhaseEn (
  IN OUT MrcParameters *const   MrcData,
  IN     INT64                  DccPHClkPhaseEn
  )
{
}

/**
  This routine computes the read/write fifo pointer delays

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  tCWL              - Write Latency for current channel
  @param[in]  Controller        - Current Controller
  @param[in]  Channel           - Current Channel
  @param[out] tCWL4TxDqFifoWrEn - Pointer to the write TX DQ fifo delay
  @param[out] tCWL4TxDqFifoRdEn - Pointer to the read TX DQ fifo delay

  @retval N/A.
**/
VOID
MrcGetTxDqFifoDelay(
  IN OUT MrcParameters* const MrcData,
  IN     UINT8          tCWL,
  IN     UINT8          Controller,
  IN     UINT8          Channel,
  OUT    UINT32*        tCWL4TxDqFifoWrEn,
  OUT    UINT32*        tCWL4TxDqFifoRdEn
  )
{
  MrcOutput* Outputs;
  MrcInput*  Inputs;
  UINT8      Index;
  UINT8      WrPreambleT;
  UINT8      WrPreambleLow;
  UINT8      WrPreambleVar;
  BOOLEAN    IsGear4;
  BOOLEAN    IsLpddr;

  Outputs = &MrcData->Outputs;
  Inputs = &MrcData->Inputs;
  IsLpddr = Outputs->IsLpddr;
  IsGear4 = (Outputs->GearMode) ? 1 : 0;
  MrcGetPrePostamble (MrcData, GetTimingWrite, &WrPreambleT, &WrPreambleLow, NULL);

  *tCWL4TxDqFifoWrEn = tCWL;
  *tCWL4TxDqFifoRdEn = tCWL;

  // tcwl4txdqfifowren = ((tech==DDRPHY_LPDDR5) ? ((gears==2 ? ((cwls*4)-'h8) : ((cwls*2)-8)) -4):
  //                       ((gears==2)?(cwls-6):(cwls/2-6))-4)
  // tcwl4txdqfifowren = (tcwl4txdqfifowren) + (tcwl4txdqfifowren%2);
  //
  // wr_preamble = WrPreambleT + WrPreambleLow - 1
  // tcwl4txdqfiforden = (tech==DDRPHY_LPDDR5) ? ((gears==2)? (Write_latency*4-7) : (write_latency*2-7)) :
  //                      ((gears==2) ? ((write_latency-'h7)-(wr_preamble) ): (write_latency/2-'h7-((wr_preamble+1)/2) ));
  //
  if (IsLpddr) {
    Index = (Controller * MAX_CHANNEL) + Channel;
    *tCWL4TxDqFifoWrEn *= (IsGear4 ? 2 : 4);
    *tCWL4TxDqFifoWrEn -= 12;

    *tCWL4TxDqFifoRdEn *= (IsGear4 ? 2 : 4);
    *tCWL4TxDqFifoRdEn -= 7;

    *tCWL4TxDqFifoRdEn += ((Index >= 6) ? 1 : 0);
  } else { // Ddr5
    Index = (Controller * MAX_CHANNEL) + (Channel * 2);
    *tCWL4TxDqFifoWrEn /= (IsGear4 ? 2 : 1);
    *tCWL4TxDqFifoWrEn -= 6;
    *tCWL4TxDqFifoWrEn -= 4;

    WrPreambleVar = WrPreambleT + WrPreambleLow - 1;
    *tCWL4TxDqFifoRdEn /= (IsGear4 ? 2 : 1);
    *tCWL4TxDqFifoRdEn -= 7;
    *tCWL4TxDqFifoRdEn -= IsGear4 ? ((WrPreambleVar + 1) / 2) : WrPreambleVar;
    *tCWL4TxDqFifoRdEn += (Outputs->Frequency == f3200) ? (IsGear4 ? 1 : 0) : 0;
  }
  if (Inputs->ExtInputs.Ptr->DqLoopbackTest) {
    *tCWL4TxDqFifoRdEn -= 1;
  }
  *tCWL4TxDqFifoWrEn += *tCWL4TxDqFifoWrEn % 2;
  *tCWL4TxDqFifoRdEn += Inputs->ExtInputs.Ptr->CccPinsInterleaved ? DdrioChDeltaCccIL[Index] : DdrioChDelta[Index];
}

/**
  This function returns channel bit mask of the channels that hardware exists even though it may not be populated.

  @param[in] MrcData - Pointer to MRC global data.

  @returns channel bit mask that exists in hardware.
**/
UINT8
MrcGetValidHwChBitMask (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs = &MrcData->Outputs;
  MrcIpDerivative IpDerivative = MrcData->Inputs.DdrIoIpVersion.Bits.Derivative;
  MrcDdrType DdrType = Outputs->DdrType;

  MRC_DEBUG_ASSERT (DdrType == MRC_DDR_TYPE_DDR5 || DdrType == MRC_DDR_TYPE_LPDDR5,
                    &Outputs->Debug,
                    "Invalid DdrType (%d) which is not supported\n", DdrType);
  MRC_DEBUG_ASSERT (IpDerivative == ipDerivativePtlx64 || IpDerivative == ipDerivativePtl,
                    &Outputs->Debug,
                    "Invalid DdrioIpDerivative (%d) which is not supported\n", IpDerivative);

  return HwChBitMasks[IpDerivative][DdrType];
}

/**
  Setup DQ override.

  @param[in, out] MrcData         - Include all MRC global data.
  @param[in]      IsEnabled       - If set to TRUE, programs dq pins high.
                                    If set to FALSE, disables DQ override.
**/
VOID
MrcConfigureDqPins (
  IN OUT MrcParameters *const   MrcData,
  BOOLEAN                       IsEnabled
  )
{
  INT64   GetSetVal;

  if (IsEnabled) {
    GetSetVal = 0xFF;
  } else {
    GetSetVal = 0x0;
  }

  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocDqOverrideData, WriteCached, &GetSetVal);
  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocDqOverrideEn,   WriteCached, &GetSetVal);
}

/**
  This function gets or sets the value for ForceRxOn.

  @param[in]      MrcData               - Include all MRC global data.
  @param[in]      UpdateMode            - The mode to be used for program.
  @param[in]      GetSetMode            - The GetSet mode to be used for program.
  @param[in, out] *ForceRxOnDqsOrAmpOn  - Value to be written / Saved for ForceRxOn.
  @param[in, out] *ForceRxOnDqsMux      - Value not used for PTL.
  @param[in, out] *ForceRxOnDq          - Value not used for PTL.

  @retval NA
**/
VOID
MrcGetSetForceRxOn (
  IN     MrcParameters* const MrcData,
  IN     UPDATE_MODE    UpdateMode,
  IN     UINT32         GetSetMode,
  IN OUT INT64          *ForceRxOnDqsOrAmpOn,
  IN OUT INT64          *ForceRxOnDqsMux,
  IN OUT INT64          *ForceRxOnDq
  )
{
  MrcOutput       *Outputs;
  UINT32          Mc;
  UINT32          Ch;

  Outputs = &MrcData->Outputs;

  if (UpdateMode == SaveValue) {
    Mc = Outputs->FirstPopController;
    Ch = Outputs->Controller[Mc].FirstPopCh;

    // Assumption made that value in ForceRxOnDqs is the same as in RxOnDqsMux and RxOnDq
    MrcGetSetChStrb (MrcData, Mc, Ch, 0, GsmIocForceRxAmpOn, GetSetMode, ForceRxOnDqsOrAmpOn);
  } else if (UpdateMode == UpdateValue) {
    MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocForceRxAmpOn, GetSetMode, ForceRxOnDqsOrAmpOn);
  }
}

/**
  Configuring the bit channel_not_populated.

  @param[in] MrcData - Pointer to MRC global data
  @return VOID
**/
VOID
MrcSetChannelNotPopulated (
  MrcParameters * MrcData
)
{
}

/**
  This function toggles the DLL Reset.

  @param[in] MrcData - Include all MRC global data.
  @param[in] Value   - DLL Reset value
**/
VOID
MrcToggleDllReset (
  IN MrcParameters * const MrcData,
  IN UINT32                Value
  )
{
  UINT32     Offset;
  UINT32     Index;
  DATASHARED_CR_DDRCRDLLCONTROL1_STRUCT       DllControl1;
  DATASHARED_CR_DDRCRTXDLLCONTROL1_STRUCT     DataTxDllControl1;
  CCCSHARED_CR_DDRCRTXDLLCONTROL1_STRUCT      CccTxDllControl1;

  // Issue DLL Reset
  for (Index = 0; Index < MRC_CCC_SHARED_NUM; Index++) {
    if (!(MrcGetHwPartitionExists (MrcData, PartitionCccShared, Index, MRC_IGNORE_ARG))) {
      continue;
    }
    Offset = OFFSET_CALC_CH (DDRCCC_SHARED0_CR_DDRCRTXDLLCONTROL1_REG, DDRCCC_SHARED1_CR_DDRCRTXDLLCONTROL1_REG, Index);
    CccTxDllControl1.Data = MrcReadCR (MrcData, Offset);
    CccTxDllControl1.Bits.ForceDLLReset = Value;
    MrcWriteCR (MrcData, Offset, CccTxDllControl1.Data);
  }
  for (Index = 0; Index < MRC_DATA_SHARED_NUM; Index++) {
    if (!(MrcGetHwPartitionExists (MrcData, PartitionDataShared, Index, MRC_IGNORE_ARG))) {
      continue;
    }
    Offset = OFFSET_CALC_CH (DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL1_REG, DDRDATA_SHARED1_CR_DDRCRTXDLLCONTROL1_REG, Index);
    DataTxDllControl1.Data = MrcReadCR (MrcData, Offset);
    DataTxDllControl1.Bits.ForceDLLReset = Value;
    MrcWriteCR (MrcData, Offset, DataTxDllControl1.Data);

    Offset = OFFSET_CALC_CH (DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_REG, DDRDATA_SHARED1_CR_DDRCRDLLCONTROL1_REG, Index);
    DllControl1.Data = MrcReadCR (MrcData, Offset);
    DllControl1.Bits.ForceDLLReset = Value;
    MrcWriteCR (MrcData, Offset, DllControl1.Data);
  }
}

/**
  This procedure sets the pctle settings.

  @param[in,out] MrcData        - Include all MRC global data.
  @param[out]    Vref           - The VREF value
  @param[out]    EquOdt         - The EquOdt value
  @param[out]    Controller     - The Controller value
  @param[out]    Channel        - The Channel value
  @param[out]    Rank           - The Rank value

**/
void
MRCSetPctle (
  IN MrcParameters *const MrcData,
  IN INT32                *Vref,
  IN UINT32               EquOdt,
  IN UINT32               Controller,
  IN UINT8                Channel,
  IN UINT8                Rank
  )
{
}

/**
  This function is used to set the default RxVREF.

  @param[in]  MrcData         - Pointer to global data structure.
  @param[in]  Vref            - Pointer to the VREF value.

**/
void
MrcSetDefaultRxVrefValueDdr5 (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                *Vref
  )
{
}
/**
  This function gets the value for TxDqNmosOnlyValue

  @param[in] MrcData - Pointer to global MRC data.
  @param[in] VccDdq  - VCC DDQ Value.

  @retval TxDqNmosOnly Value
**/
UINT32
MrcGetTxDqNmosOnlyValue (
  IN MrcParameters *const MrcData,
  IN  UINT32              VccDdq
  )
{
  if (VccDdq <= VDD_0_50) {
    return 1;
  } else {
    return 0;
  }
}