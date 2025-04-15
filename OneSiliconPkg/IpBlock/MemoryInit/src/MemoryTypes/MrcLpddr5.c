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
#include "MrcLpddr5Private.h" // to cross check declarations agains implementations here for private API
#include "MrcLpddr5.h"  // to cross check declarations against implementations here for public API
#include "CMrcTypes.h"
#include "CMrcApi.h"
#include "MrcHalRegisterAccess.h"
#include "MrcMaintenance.h"
#include "MrcChipApi.h"
#include "MrcDdrIoDefines.h"

///
/// Timing Frequency Range defines
///
#define LP5_TFR_MIN (0)
#define LP5_TFR_MAX (1)
#define LP5_TFR_NUM_ENTRIES   (Lp5Wl4t1BMax)
#define LP5_TFR_UPPER_LIMIT (1)
#define LP5_TFR_LOWER_LIMIT (0)
#define MRC_LP_tINIT0_US   20000   ///< Max voltage-ramp time
#define MRC_LP_tINIT4_TCK  5       ///< Min stable clock before first CKE high

///
/// Local Constants
///
// This table is based on the following timing tables from LPDDR5 JEDEC spec:
//  Table 22 - Read Latencies and Read with AP to Activate
//  Table 23 - Write Latency
//  Table 24 - Write Recovery time
// The frequency for the entry being programmed must be greater than the minimum and
// less than or equal to the maximum.
static const MrcFrequency TimingFreqRangeLp5[LP5_TFR_NUM_ENTRIES][2] = {
  {fNoInit, f533},
  {f533,    f1067},
  {f1067,   f1600},
  {f1600,   f2133},
  {f2133,   f2750},
  {f2750,   f3200},
  {f3200,   f3733},
  {f3733,   f4267},
  {f4267,   f4800},
  {f4800,   f5500},
  {f5500,   f6000},
  {f6000,   f6400},
  {f6400,   f7500},
  {f7500,   f8533},
  {f8533,   f9600},
  {f9600,   fInvalid}
  };

typedef enum {
  MrcLp5RlDvfscDisIndex533,
  MrcLp5RlDvfscDisIndex1067,
  MrcLp5RlDvfscDisIndex1600,
  MrcLp5RlDvfscDisIndex2133,
  MrcLp5RlDvfscDisIndex2750,
  MrcLp5RlDvfscDisIndex3200,
  MrcLp5RlDvfscDisIndex3733,
  MrcLp5RlDvfscDisIndex4267,
  MrcLp5RlDvfscDisIndex4800,
  MrcLp5RlDvfscDisIndex5500,
  MrcLp5RlDvfscDisIndex6000,
  MrcLp5RlDvfscDisIndex6400,
  MrcLp5RlDvfscDisIndex7500,
  MrcLp5RlDvfscDisIndex8533,
  MrcLp5RlDvfscDisIndex9600,
  MrcLp5RlDvfscDisIndex10667,
  MrcLp5RlDvfscDisIndexMax
} MrcLp5RlDvfscDisIndex;

const UINT8 Lp5ReadLatencyDvfscDisabledSet0[MrcLp5RlDvfscDisIndexMax] = { 3, 4, 5, 6, 8,  9, 10, 12, 13, 15, 16, 17, 20, 23, 25, 28 };
const UINT8 Lp5ReadLatencyDvfscDisabledSet1[MrcLp5RlDvfscDisIndexMax] = { 3, 4, 5, 7, 8, 10, 11, 13, 14, 16, 17, 18, 22, 25, 28, 30 };
const UINT8 Lp5ReadLatencyDvfscDisabledSet2[MrcLp5RlDvfscDisIndexMax] = { 3, 4, 6, 7, 9, 10, 12, 14, 15, 17, 19, 20, 24, 26, 29, 33 };

const UINT8 Lp5ReadLatencyDvfscEnabledSet0[6] = { 3, 5, 7, 8, 10, 12 };
const UINT8 Lp5ReadLatencyDvfscEnabledSet1[6] = { 3, 5, 7, 8, 10, 12 };
const UINT8 Lp5ReadLatencyDvfscEnabledSet2[6] = { 3, 5, 7, 10, 12, 14 };

/**
  This function checks that the Vref requested is within the spec defined range for LPDDR5.

  @param[in]  MrcData   - Pointer to global MRC data.
  @param[in]  Vref      - Requested Vref in mV
  @param[in]  VrefType  - CmdV or WrV

  @param  UINT16 - The requested Vref or a clamped Vref value.
**/
UINT16
CheckVrefLimits (
  IN  MrcParameters   *MrcData,
  IN  UINT16          Vref,
  IN  MrcMarginTypes  VrefType
  )
{
  MrcOutput *Outputs;
  UINT16    RetVref;
  UINT16    MinVref;
  UINT16    MaxVref;
#ifdef MRC_DEBUG_PRINT
  UINT32    ErrorLimit;
#endif

  Outputs = &MrcData->Outputs;

  MinVref = LP5_VREF_MIN_MV;
  if (VrefType == CmdV) {
    MaxVref = LP5_CA_VREF_MAX;
  } else {
    // WrV
    if (!Outputs->IsDvfscEnabled) {
      // MR14 table in Jedec spec - Note 4: Absolute DQ[7:0] Vref high level (%code * VDDQ) must be lower than or equal
      // to 350mV when WCK is less than or equal to 1600MHz. Absolute DQ[7:0] Vref high level (%code * VDDQ) must be lower
      // than or equal to 225mV when WCK is higher than 1600MHz and less than or equal to 3200MHz.
      MaxVref = (MrcData->Outputs.HighFrequency <= f1600) ? LP5_DQ_VREF_LOW_F_MAX : LP5_DQ_VREF_HIGH_F_MAX;
    } else {
      // MR14 table in Jedec spec - Note 5: Absolute DQ[7:0] Vref high level (%code * VDDQ) must be
      // lower than or equal to 300mV regardless of WCK frequency when Enhanced DVFSC is enabled.
      MaxVref = LP5_DQ_VREF_EDVFSC_F_MAX;
    }
  }

#ifdef MRC_DEBUG_PRINT
  ErrorLimit = 0;
  if (Vref < MinVref) {
    ErrorLimit = MinVref;
  } else if (Vref > MaxVref) {
    ErrorLimit = MaxVref;
  }
  if (ErrorLimit != 0) {
    MRC_DEBUG_MSG (
      &MrcData->Outputs.Debug,
      MSG_LEVEL_WARNING,
      "%s %s %u violates spec limit: %u\n",
      gWarnString,
      gMarginTypesStr[VrefType],
      Vref,
      ErrorLimit
      );
  }
#endif
  RetVref = RANGE (Vref, MinVref, MaxVref);

  return RetVref;
}

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
  )
{
  MrcDebug      *Debug;
  MrcStatus     Status;
  MrcFrequency  CurFreq;
  MrcFrequency  MaxFreq;
  MrcFrequency  MinFreq;

  Debug = &MrcData->Outputs.Debug;

  // Check to see if the time requested matches JEDEC Frequency table
  if ((Latency >= 0) && (Latency < LP5_TFR_NUM_ENTRIES)) {
    CurFreq = MrcData->Outputs.HighFrequency;
    MaxFreq = TimingFreqRangeLp5[(UINT8) Latency][LP5_TFR_MAX];
    MinFreq = TimingFreqRangeLp5[(UINT8) Latency][LP5_TFR_MIN];

    if ((CurFreq <= MinFreq) || (CurFreq > MaxFreq)) {
      Status = mrcWrongInputParameter;
      MRC_DEBUG_MSG (
        Debug,
        MSG_LEVEL_ERROR,
        "%s Current frequency(%d) is not within supported range[%d:%d] for latency=%d",
        gErrString,
        CurFreq,
        MinFreq,
        MaxFreq,
        Latency
        );
    } else {
      Status = mrcSuccess;
    }
  } else {
    Status = mrcWrongInputParameter;
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_ERROR,
      "%s Latency(%d) entry out of range for TimingFreqRange Table(%d)",
      gErrString,
      Latency,
      LP5_TFR_NUM_ENTRIES
      );
  }

  return Status;
}

/**
  This function converts from DRAM Vref encoding to MRC training offset:
  Vref [10:127] - Offset [-58:58] :: LP5 (15% - 73.5%) * 500mV.

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
)
{
  MrcStatus Status;
  UINT32    TrainOffset;

  Status = mrcSuccess;

  if (Vref < 10) {
    Status = mrcWrongInputParameter;
  } else {
    TrainOffset = Vref - (LP5_VREF_OFFSET_MAX + 10);

    MRC_DEBUG_ASSERT (Offset != NULL, &MrcData->Outputs.Debug, "Null pointer\n");
    *Offset = TrainOffset;
  }

  return Status;
}

/**
  This function converts the Vref number in millivolts to the MRC training offset.
  LPDDR5 has one flat range from 15% of VDDq to 75% of VDDq.

  @param[in]  MrcData - Pointer to MRC global data.
  @param[in]  VrefMv  - Request Vref in millivolts.
  @param[out] Offset  - Pointer to return training index.

  @retval mrcSuccess              Input parameters are valid.
  @retval mrcWrongInputParameter  Input parameters are invalid.
**/
MrcStatus
VrefToOffset (
  IN  MrcParameters *const  MrcData,
  IN  UINT16                VrefMv,
  OUT INT32                 *Offset
  )
{
  INT32     TrainOffset;
  UINT32    StepSizeUv;
  UINT32    VrefUv;
  UINT32    MinVrefUv;
  UINT32    MaxVrefUv;
  UINT32    OffsetVrefUv;
  MrcDebug  *Debug;

  Debug = &MrcData->Outputs.Debug;

  if (Offset == NULL) {
    MRC_DEBUG_ASSERT (1 == 0, &MrcData->Outputs.Debug, "Null pointer\n");
    return mrcWrongInputParameter;
  }

  MinVrefUv   = LP5_VREF_MIN_UV;
  MaxVrefUv   = 1000 * LP5_CA_VREF_MAX;
  StepSizeUv  = LP5_VREF_STEP_SIZE;
  VrefUv = VrefMv * 1000;

  if (VrefUv <= MinVrefUv) {
    TrainOffset = LP5_VREF_OFFSET_MIN;
  } else if (VrefUv >= MaxVrefUv) {
    TrainOffset = LP5_VREF_OFFSET_MAX;
  } else {
    // Subtract the minimum range, then do a divide round by the step size.
    OffsetVrefUv = VrefUv - MinVrefUv;
    TrainOffset = LP5_VREF_OFFSET_MIN + UDIVIDEROUND (OffsetVrefUv, StepSizeUv);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NONE, "Converting Vrev=%umV into Offset=%d\n", VrefMv, TrainOffset);
  *Offset = TrainOffset;

  return mrcSuccess;
}

/**
  This function converts from the integer defined Write Latency to the Mode Register
  encoding of the timing in LPDDR5.

  @param[in]  MrcData - Pointer to global MRC data.
  @param[in]  Value   - Requested Write Latency value.
  @param[out] EncVal  - Encoded Mode Register value.

  @retval MrcStatus - mrcSuccess if the latency is supported.  Else mrcWrongInputParameter.
**/
MrcStatus
EncodeWriteLatencyLpddr5 (
  IN  MrcParameters *MrcData,
  IN  UINT16        Value,
  OUT UINT8         *EncVal
  )
{
  MrcOutput *Outputs;
  MrcDebug  *Debug;
  MrcStatus Status;
  UINT8     MrValue;

  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;
  Status = mrcSuccess;

  switch (Value) {
    case 2:
      MrValue = Lp5Wl4t1B2;
      break;

    case 3:
      MrValue = Lp5Wl4t1B3;
      break;

    case 4:
      MrValue = Lp5Wl4t1B4;
      break;

    case 5:
      MrValue = Lp5Wl4t1B5;
      break;

    case 7:
      MrValue = Lp5Wl4t1B7;
      break;

    case 8:
      MrValue = Lp5Wl4t1B8;
      break;

    case 9:
      MrValue = Lp5Wl4t1B9;
      break;

    case 11:
      MrValue = Lp5Wl4t1B11;
      break;

    case 12:
      MrValue = Lp5Wl4t1B12;
      break;

    case 14:
      MrValue = Lp5Wl4t1B14;
      break;

    case 15:
      MrValue = Lp5Wl4t1B15;
      break;

    case 16:
      MrValue = Lp5Wl4t1B16;
      break;

    case 19:
      MrValue = Lp5Wl4t1B19;
      break;

    case 22:
      MrValue = Lp5Wl4t1B22;
      break;

    case 24:
      MrValue = Lp5Wl4t1B24;
      break;

    case 26:
      MrValue = Lp5Wl4t1B26;
      break;

    default:
      MrValue = 0xFF;
      break;
  }

  if (MrValue != 0xFF) {
    // Check to see if the time requested matches JEDEC Frequency table
    if (!Outputs->IsDvfscEnabled) {
      Status = LatencyFreqCheckLpddr5 (MrcData, MrValue);
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, (Status != mrcSuccess) ? " (WL)\n" : "");
    }

    if (EncVal != NULL) {
      *EncVal = MrValue;
    } else {
      Status = mrcWrongInputParameter;
    }
  } else {
    Status = mrcWrongInputParameter;
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid %s Latency Value: %d\n", gErrString, gWrString, Value);
  }

  return Status;
}

/**
  This function converts from the integer defined Write Recovery to the Mode Register
  encoding of the timing in LPDDR5.

  @param[in]  MrcData - Pointer to global MRC data.
  @param[in]  Value   - Requested Write Recovery value.
  @param[out] EncVal  - Encoded Mode Register value.

  @retval MrcStatus - mrcSuccess if the latency is supported.  Else mrcWrongInputParameter.
**/
MrcStatus
EncodeWriteRecoveryLpddr5 (
  IN  MrcParameters *MrcData,
  IN  UINT16        Value,
  OUT UINT8         *EncVal
  )
{
  MrcOutput *Outputs;
  MrcDebug  *Debug;
  MrcStatus Status;
  UINT8     MrValue;

  Outputs = &MrcData->Outputs;
  Debug = &MrcData->Outputs.Debug;

  Status = mrcSuccess;
  if (!Outputs->IsDvfscEnabled) {
    if (MrcData->Outputs.LpByteMode) {
      switch (Value) {
        case 3:
          MrValue = Lp5Wr4t1X8_3;
          break;

        case 5:
          MrValue = Lp5Wr4t1X8_5;
          break;

        case 8:
          MrValue = Lp5Wr4t1X8_8;
          break;

        case 10:
          MrValue = Lp5Wr4t1X8_10;
          break;

        case 13:
          MrValue = Lp5Wr4t1X8_13;
          break;

        case 15:
          MrValue = Lp5Wr4t1X8_15;
          break;

        case 17:
          MrValue = Lp5Wr4t1X8_17;
          break;

        case 20:
          MrValue = Lp5Wr4t1X8_20;
          break;

        case 22:
          MrValue = Lp5Wr4t1X8_22;
          break;

        case 25:
          MrValue = Lp5Wr4t1X8_25;
          break;

        case 28:
          MrValue = Lp5Wr4t1X8_28;
          break;

        case 29:
          MrValue = Lp5Wr4t1X8_29;
          break;

        case 34:
          MrValue = Lp5Wr4t1X8_34;
          break;

        case 39:
          MrValue = Lp5Wr4t1X8_39;
          break;

        case 44:
          MrValue = Lp5Wr4t1X8_44;
          break;

        case 48:
          MrValue = Lp5Wr4t1X8_48;
          break;

        default:
          MrValue = 0xFF;
          Status = mrcWrongInputParameter;
          break;
      }
    } else {
      switch (Value) {
        case 3:
          MrValue = Lp5Wr4t1X16_3;
          break;

        case 5:
          MrValue = Lp5Wr4t1X16_5;
          break;

        case 7:
          MrValue = Lp5Wr4t1X16_7;
          break;

        case 10:
          MrValue = Lp5Wr4t1X16_10;
          break;

        case 12:
          MrValue = Lp5Wr4t1X16_12;
          break;

        case 14:
          MrValue = Lp5Wr4t1X16_14;
          break;

        case 16:
          MrValue = Lp5Wr4t1X16_16;
          break;

        case 19:
          MrValue = Lp5Wr4t1X16_19;
          break;

        case 21:
          MrValue = Lp5Wr4t1X16_21;
          break;

        case 24:
          MrValue = Lp5Wr4t1X16_24;
          break;

        case 26:
          MrValue = Lp5Wr4t1X16_26;
          break;

        case 28:
          MrValue = Lp5Wr4t1X16_28;
          break;

        case 32:
          MrValue = Lp5Wr4t1X16_32;
          break;

        case 37:
          MrValue = Lp5Wr4t1X16_37;
          break;

        case 41:
          MrValue = Lp5Wr4t1X16_41;
          break;

        case 48:
          MrValue = Lp5Wr4t1X16_48;
          break;

        default:
          MrValue = 0xFF;
          Status = mrcWrongInputParameter;
          break;
      }
    }
  } else {  //E-DVFSC is enabled
    if (MrcData->Outputs.LpByteMode) {
      switch (Value) {
        case 3:
          MrValue = Lp5DvfscWr4t1X8_3;
          break;

        case 6:
          MrValue = Lp5DvfscWr4t1X8_6;
          break;

        case 9:
          MrValue = Lp5DvfscWr4t1X8_9;
          break;

        case 12:
          MrValue = Lp5DvfscWr4t1X8_12;
          break;

        case 15:
          MrValue = Lp5DvfscWr4t1X8_15;
          break;

        case 18:
          MrValue = Lp5DvfscWr4t1X8_18;
          break;

        default:
          MrValue = 0xFF;
          Status = mrcWrongInputParameter;
          break;
      }
    } else {
      switch (Value) {
        case 3:
          MrValue = Lp5DvfscWr4t1X16_3;
          break;

        case 6:
          MrValue = Lp5DvfscWr4t1X16_6;
          break;

        case 9:
          MrValue = Lp5DvfscWr4t1X16_9;
          break;

        case 11:
          MrValue = Lp5DvfscWr4t1X16_11;
          break;

        case 15:
          MrValue = Lp5DvfscWr4t1X16_15;
          break;

        case 17:
          MrValue = Lp5DvfscWr4t1X16_17;
          break;

        default:
          MrValue = 0xFF;
          Status = mrcWrongInputParameter;
          break;
      }
    }
  }

  if (MrValue != 0xFF) {
    // Check to see if the time requested matches JEDEC Frequency table
    // @todo: Spec has conflict with itself
    //Status = LatencyFreqCheckLpddr5 (MrcData, MrValue);
    //MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, (Status != mrcSuccess) ? " (WR)\n" : "");
    if (EncVal != NULL) {
      *EncVal = MrValue;
    } else {
      Status = mrcWrongInputParameter;
    }
  } else {
    Status = mrcWrongInputParameter;
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Unsupported Write Recovery value: %u\n", gErrString, Value);
  }

  return Status;
}

/**
  This function searches through the Frequency Range table to find out what Bin index
  the current Frequency is in for Timing parameters that are Bin based.

  @param[in]  Frequency - Data Rate

  @param[in]  UINT8 - Frequency Bin Index
**/
UINT8
GetFreqBinIndex (
  IN  MrcFrequency Frequency
  )
{
  UINT8 Bin;
  MrcFrequency Lower;
  MrcFrequency Upper;

  Bin = 0;
  if (Frequency != 0) {
    for (Bin = 0; Bin < LP5_TFR_NUM_ENTRIES; Bin++) {
      Lower = TimingFreqRangeLp5[Bin][LP5_TFR_LOWER_LIMIT];
      Upper = TimingFreqRangeLp5[Bin][LP5_TFR_UPPER_LIMIT];
      if ((Lower < Frequency) &&
          (Upper >= Frequency)) {
        break;
      }
    }
  }

  return Bin;
}

/**
  This function converts from MRC training index to DRAM Vref encoding:
    Vref [10:127] - Offset [-58:58] :: LP5 (15% - 73.5%) * 500mV.

  @param[in]  MrcData - Pointer to MRC global data.
  @param[in]  Offset  - Training index to convert.
  @param[out] Vref    - Pointer to return the Vref MR encoding.

  @retval mrcSuccess              Input parameters are valid (LPDDR5 Spec).
  @retval mrcWrongInputParameter  Input parameters are invalid (LPDDR5 Spec).
**/
MrcStatus
OffsetToMRencoding (
  IN  MrcParameters *const  MrcData,
  IN  INT32                 Offset,
  OUT UINT8                 *Vref
  )
{
  MrcDebug  *Debug;
  MrcStatus Status;
  UINT8     MrVref;

  Debug   = &MrcData->Outputs.Debug;
  Status  = mrcSuccess;

  if ((Offset > LP5_VREF_OFFSET_MAX) || (Offset < LP5_VREF_OFFSET_MIN)) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid Vref Setting %d is out of range [%d:%d]. Clamping\n",
      gErrString, Offset, LP5_VREF_OFFSET_MIN, LP5_VREF_OFFSET_MAX);
    Status = mrcParamSaturation;
    if (Offset > LP5_VREF_OFFSET_MAX) {
      Offset  = LP5_VREF_OFFSET_MAX;
    } else {
      Offset  = LP5_VREF_OFFSET_MIN;
    }
  }

  // MIN Offset is -58 -> 10  Vref MR Encoding
  // MAX Offset is +58 -> 126 Vref MR Encondig
  MrVref = (UINT8) Offset + 68;

  MRC_DEBUG_ASSERT (((Vref != NULL)), &MrcData->Outputs.Debug, "Null pointer\n");

  *Vref   = MrVref;

  return Status;
}

/**
  Update the Read Preamble type in MR10 and send this to DRAM.
  Update MR10 in the host struct as well

  @param[in]  MrcData -  Pointer to MRC global data.

  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcSetLpddr5Mr10ReadPreamble (
  IN  MrcParameters *const MrcData
  )
{
  MrcStatus             Status;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcChannelOut         *ChannelOut;
  UINT8                 *MrPtr;
  UINT32                Channel;
  UINT32                Controller;
  UINT32                Rank;
  LPDDR5_MR10_tRPRE     RpreVal;
  LPDDR5_MODE_REGISTER_10_TYPE  Mr10;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  Status  = mrcSuccess;
  Mr10.Data8 = 0;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
          continue;
        }
        ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
        MrPtr = ChannelOut->Dimm[dDIMM0].Rank[Rank % MAX_RANK_IN_DIMM].MR;
        Mr10.Data8 = MrPtr[mrIndexMR10];
        if (MrcLp5GetReadPreambleSetting (MrcData, &RpreVal) != mrcSuccess) {
          Status = mrcWrongInputParameter;
        }
        Mr10.Bits.RdqsPreLen = RpreVal;
        MrPtr[mrIndexMR10] = Mr10.Data8;
      }
    }
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "New MR10: 0x%x\n", Mr10.Data8);

  // Program the new MR10 value on all populated ranks
  MrcIssueMrwMulticast (MrcData, Outputs->ValidRankMask, mrMR10, Mr10.Data8, MRC_PRINTS_OFF);

  return Status;
}

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
  )
{
  MrcDebug      *Debug;
  MrcInput      *Inputs;
  MrcOutput     *Outputs;
  MrcChannelOut *ChannelOut;
  UINT8         *MrPtr;
  MrcTiming     *TimingPtr;
  MrcStatus     Status;
  UINT32        Channel;
  UINT32        Controller;
  UINT32        FirstCh;
  UINT32        FirstMc;
  UINT32        Profile;
  UINT32        Rank;
  UINT16        CaDrvStrength;
  UINT16        CaOdt;
  UINT16        CsOdt;
  UINT16        CaVrefMv;
  UINT16        DqOdt;
  UINT16        DqVrefMv;
  UINT16        PdDrvStr;
  UINT16        SocOdt;
  UINT16        tCWL;
  INT32         Offset;
  INT8          CaOdtEnc;
  INT8          CsOdtEnc;
  INT8          DqOdtEnc;
  INT8          PdDsEnc;
  INT8          SocOdtEnc;
  INT8          WckOdtEnc;
  UINT8         CaVrefEnc;
  UINT8         DqVrefEnc;
  UINT8         NtOdtEnc;
  UINT8         RlEnc;
  UINT8         WlEnc;
  UINT8         WrEnc;
  UINT8         DfeQu;
  UINT8         DfeQl;
  UINT8         DcaValue;

  MRC_EXT_INPUTS_TYPE  *ExtInputs;
  TOdtValueLpddr LpddrOdtTableIndex;
  LPDDR5_MR10_tRPRE             RpreVal;
  LPDDR5_MODE_REGISTER_1_TYPE   Mr1;
  LPDDR5_MODE_REGISTER_2_TYPE   Mr2;
  LPDDR5_MODE_REGISTER_3_TYPE   Mr3;
  LPDDR5_MODE_REGISTER_10_TYPE  Mr10;
  LPDDR5_MODE_REGISTER_11_TYPE  Mr11;
  LPDDR5_MODE_REGISTER_12_TYPE  Mr12;
  LPDDR5_MODE_REGISTER_13_TYPE  Mr13;
  LPDDR5_MODE_REGISTER_14_TYPE  Mr14;
  LPDDR5_MODE_REGISTER_15_TYPE  Mr15;
  LPDDR5_MODE_REGISTER_16_TYPE  Mr16;
  LPDDR5_MODE_REGISTER_17_TYPE  Mr17;
  LPDDR5_MODE_REGISTER_18_TYPE  Mr18;
  LPDDR5_MODE_REGISTER_19_TYPE  Mr19;
  LPDDR5_MODE_REGISTER_20_TYPE  Mr20;
  LPDDR5_MODE_REGISTER_24_TYPE  Mr24;
  LPDDR5_MODE_REGISTER_30_TYPE  Mr30;
  LPDDR5_MODE_REGISTER_37_TYPE  Mr37;
  LPDDR5_MODE_REGISTER_40_TYPE  Mr40;
  LPDDR5_MODE_REGISTER_41_TYPE  Mr41;
  LPDDR5_MODE_REGISTER_58_TYPE  Mr58;
  LPDDR5_MODE_REGISTER_69_TYPE  Mr69;

  Inputs = &MrcData->Inputs;
  ExtInputs = Inputs->ExtInputs.Ptr;
  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;
  Profile = ExtInputs->MemoryProfile;
  Status = mrcSuccess;
  //SocOdt = Outputs->RcompTarget[RdOdt];
  SocOdt = 40; // Separate SocOdt from RdOdt
  CaDrvStrength = Outputs->RcompTarget[WrDSCmd];
  PdDrvStr = 60; // Ohms
  CsOdtEnc = 0;
  if (ExtInputs->DqLoopbackTest) {
#ifndef HVM_FLAG
    Inputs->NonTargetOdtEn = 1;
#endif
    if ((2 & Outputs->ValidRankMask) == 0) {
      NtOdtEnc = 6; // 1R: RZQ/6 = 40 Ohm
    } else {
      NtOdtEnc = 3; // 2R: RZQ/3 = 80 Ohm
    }
  } else {
    NtOdtEnc = 0;
  }

  // Extract ODT table for first populated channel
  FirstMc = (MrcControllerExist (MrcData, cCONTROLLER0) ? cCONTROLLER0 : cCONTROLLER1);
  FirstCh = Outputs->Controller[FirstMc].FirstPopCh;
  Status = GetOdtTableIndex (MrcData, FirstMc, FirstCh, dDIMM0, &LpddrOdtTableIndex);
  if (Status != mrcSuccess) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s: Retrieving Lpddr OdtTableIndex failed!\n", gNullPtrErrStr);
    return mrcFail;
  }
  DqOdt = (Outputs->DramDqOdtEn) ? LpddrOdtTableIndex.RttWr : 0xFFFF;
  DqOdtEnc = LpddrOdtEncode (DqOdt);

  WckOdtEnc = LpddrOdtEncode (LpddrOdtTableIndex.RttWck); // Ohms

  if (WckOdtEnc == -1) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid %s ODT Value %u\n", gErrString, gCmdString, WckOdtEnc);
    Status  = mrcWrongInputParameter;
  }

  if (DqOdtEnc == -1) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid %s ODT Value %u\n", gErrString, gDataString, DqOdtEnc);
    Status  = mrcWrongInputParameter;
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
          continue;
        }
        ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
        MrPtr = ChannelOut->Dimm[dDIMM0].Rank[Rank % MAX_RANK_IN_DIMM].MR;
        TimingPtr = &Outputs->Timing[Profile];

        if (ChannelOut->Dimm[dDIMM0].RankInDimm == 2) {
          CaOdt = Outputs->LpByteMode ? LpddrOdtTableIndex.RttCa2RByteMode : LpddrOdtTableIndex.RttCa;
        } else {
          CaOdt = LpddrOdtTableIndex.RttCa;
        }
        CaOdtEnc = LpddrOdtEncode (CaOdt);
        if (CaOdtEnc == -1) {
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid %s ODT Value %u\n", gErrString, gCmdString, CaOdtEnc);
          Status  = mrcWrongInputParameter;
        }

        // Setting CsOdt value is only supported for Lp5x
        if (Outputs->LpX) {
          if (ChannelOut->Dimm[dDIMM0].RankInDimm == 2) {
            CsOdt = Outputs->LpByteMode ? LpddrOdtTableIndex.RttCs2RByteMode : LpddrOdtTableIndex.RttCs;
          } else {
            CsOdt = LpddrOdtTableIndex.RttCs;
          }

          CsOdtEnc = LpddrCsOdtEncode (CsOdt);

          if (CsOdtEnc == -1) {
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid %s CS ODT Value %u\n", gErrString, gCmdString, CsOdtEnc);
            Status  = mrcWrongInputParameter;
          }
        }

        DqVrefMv = MrcLp5GetVrefDq (MrcData);
        CaVrefMv = (UINT16) MrcCalcIdealVref (MrcData, CaDrvStrength, 0, CaOdt, CmdV, TRUE);
        CaVrefMv = CheckVrefLimits (MrcData, CaVrefMv, CmdV);
        VrefToOffset (MrcData, DqVrefMv, &Offset);
        OffsetToMRencoding (MrcData, Offset, &DqVrefEnc);
        VrefToOffset (MrcData, CaVrefMv, &Offset);
        OffsetToMRencoding (MrcData, Offset, &CaVrefEnc);


        //MR16 - FSP-OP = 0, CBT = 0, CBT-PH = 0
        Mr16.Data8 = 0;
        Mr16.Bits.FspWr = 1;
        Mr16.Bits.Vrcg  = 1;
        MrPtr[mrIndexMR16] = Mr16.Data8;

        //MR13 - 0 by default from Host Structure Init.
        Mr13.Data8 = 0;
        Mr13.Bits.DmDisable = 1;
        Mr13.Bits.DualVdd2 = (ExtInputs->BoardDetails.SingleVdd2Rail == 1) ? 1 : 0; // Single or Dual VDD2 rail. If Single VDD2 rail, E-DVFSC cannot be used.
        MrPtr[mrIndexMR13] = Mr13.Data8;

        //MR20 - WCK Mode is 0 (Differential)
        Mr20.Data8 = 0;
        Mr20.Bits.RdqsMode = 0x2;
        MrPtr[mrIndexMR20] = Mr20.Data8;

        //MR24 - DFE
        MrcLp5GetDFE (MrcData, &DfeQl, &DfeQu);
        Mr24.Data8 = 0;
        Mr24.Bits.Dfeql = DfeQl;
        Mr24.Bits.Dfequ = DfeQu;
        MrPtr[mrIndexMR24] = Mr24.Data8;

        DcaValue = (Outputs->Frequency > f4800) ? 9 : 0;
        //MR30 - DCA WCK
        Mr30.Data8 = 0;
        Mr30.Bits.DcaUpperByte = DcaValue;
        Mr30.Bits.DcaLowByte = DcaValue;
        MrPtr[mrIndexMR30] = Mr30.Data8;

        //MR58 - PRE EMP
        Mr58.Data8 = 0;
        Mr58.Bits.PUEmphasisLower = 3;
        Mr58.Bits.PDEmphasisLower = 2;
        Mr58.Bits.PUEmphasisUpper = 3;
        Mr58.Bits.PDEmphasisUpper = 2;
        MrPtr[mrIndexMR58] = Mr58.Data8;

        //MR69 - DCA READ
        Mr69.Data8 = 0;
        DcaValue = 2;
        Mr69.Bits.ReadDcaUpperByte = DcaValue;
        Mr69.Bits.ReadDcaLowByte = DcaValue;
        MrPtr[mrIndexMR69] = Mr69.Data8;

        //MR17 -Rank 0 CK/CA/CS ODT = 0 (Enabled), x8 ODT Lower/Upper = (Enabled)
        Mr17.Data8 = 0;
        SocOdtEnc = LpddrOdtEncode (SocOdt);
        if (SocOdtEnc == -1) {
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid %s ODT Value 0x%x\n", gErrString, gSocString, SocOdt);
          Status = mrcWrongInputParameter;
        }
        Mr17.Bits.SocOdt = SocOdtEnc;
        if ((Rank > 0) || Outputs->IsDvfsqEnabled || Outputs->IsDvfscEnabled) {
          Mr17.Bits.CaOdtDis = 1;
          Mr17.Bits.CkOdtDis = 1;
        }
        if (Outputs->IsDvfsqEnabled || Outputs->IsDvfscEnabled) {
          Mr17.Bits.CsOdtDis = 1;
        }
        MrPtr[mrIndexMR17] = Mr17.Data8;

        //MR25 - Partial Array Self Refresh = 0 (Disabled), CaInput/CkPair ODT = 0
        //MR10 - RDQS Post Amble Mode = 1 (Static), RDQS PST = 0 (0.5tWCK), WCK PST = 0 (2.5tWCK)
        Mr10.Data8 = 0;
        Mr10.Bits.RdqsPstMode = 1;
        if (MrcLp5GetReadPreambleSetting (MrcData, &RpreVal) != mrcSuccess) {
          Status = mrcWrongInputParameter;
        }
        Mr10.Bits.RdqsPreLen  = RpreVal;
        Mr10.Bits.RdqsPstLen  = MrcLp5GetRdqsPostambleLength ();
        MrPtr[mrIndexMR10]    = Mr10.Data8;

        //MR11
        Mr11.Data8 = 0;
        if (!Outputs->IsDvfsqEnabled && !Outputs->IsDvfscEnabled) {
          Mr11.Bits.DqOdt = DqOdtEnc;
          Mr11.Bits.CaOdt = CaOdtEnc;
          Mr11.Bits.NtOdtEn = Inputs->NonTargetOdtEn;
        }
        MrPtr[mrIndexMR11] = Mr11.Data8;

        //MR12 - CA VREF (Byte Mode Lower or whole x16)
        Mr12.Data8 = 0;
        Mr12.Bits.CaVref = CaVrefEnc;
        MrPtr[mrIndexMR12] = Mr12.Data8;

        Mr12.Data8 = 0;
        Mr12.Bits.CaVref = CaVrefEnc;
        Mr12.Bits.VrefByteSel = 1;
        MrPtr[mrIndexMR12Upper] = Mr12.Data8;

        //MR14 - VDLC = 0 (DQ[15:8]
        Mr14.Data8 = 0;
        Mr14.Bits.DqVref = DqVrefEnc;
        MrPtr[mrIndexMR14] = Mr14.Data8;

        //MR15
        Mr15.Data8 = 0;
        Mr15.Bits.DqVref = DqVrefEnc;
        MrPtr[mrIndexMR15] = Mr15.Data8;

        //MR18 - CKR = 0 (Write to CK ratio is 4:1), WCK Sync Preamble = 0 (Half Rate for 4:1 mode)
        Mr18.Data8 = 0;
        // Wck Pins are shared for ranks, and the termination on a single rank will be applied on all states. Therefore only as single rank needs
        // define a termination
        if ((Rank == 0) && !Outputs->IsDvfsqEnabled && !Outputs->IsDvfscEnabled) {
          Mr18.Bits.WckOdt = WckOdtEnc; // Ohms
        }
        Mr18.Bits.WckFreqMode = (Outputs->HighFrequency >= f3200) ? 1 : 0;
        // If we have more than 1 rank, we do not use dynamic, so we set Always on = 1.
        // Needs to align with WCK_CONFIG.WCK_MODE
        if (ExtInputs->WckModeOverride < 2) {
          Mr18.Bits.WckAlwaysOn = ExtInputs->WckModeOverride;
        } else {
          Mr18.Bits.WckAlwaysOn = (ChannelOut->ValidRankBitMask >  1) ? 1 : 0;
        }

        MrPtr[mrIndexMR18] = Mr18.Data8;

        //MR3 - DBI-RD/WR = 0 (Disabled)
        Mr3.Data8 = 0;
        PdDsEnc = LpddrOdtEncode (PdDrvStr);
        if (PdDsEnc == -1) {
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid %s Value: %u\n", gErrString, gDrvStr, PdDrvStr);
          Status = mrcWrongInputParameter;
        }
        Mr3.Bits.PullDnDrvStr = PdDsEnc;
        Mr3.Bits.WriteLatencySet = 1; // Write Latency Set B
        Mr3.Bits.BankBgOrg = MrcGetBankBgOrg (MrcData, Outputs->HighFrequency);
        MrPtr[mrIndexMR3] = Mr3.Data8;

        //MR1 - Clock Mode = 0 (Differential)
        Mr1.Data8 = 0;
        if (ExtInputs->DqLoopbackTest) {
          // CWL in Timing profile was adjusted in SPD Processing for loopback mode
          // Use the actual CWL for MR encoding only
          tCWL = (UINT16) GetLpddr5tCWL (TimingPtr->tCK, 1); // We always use Set B (1)
        } else {
          tCWL = TimingPtr->tCWL;
        }
        if (EncodeWriteLatencyLpddr5 (MrcData, tCWL, &WlEnc) != mrcSuccess) {
          Status = mrcWrongInputParameter;
        }
        Mr1.Bits.WriteLatency = WlEnc;
        Mr1.Bits.CkMode = Outputs->HighFrequency <= f3600;
        MrPtr[mrIndexMR1] = Mr1.Data8;

        //MR2
        Mr2.Data8 = 0;
        if (EncodeWriteRecoveryLpddr5 (MrcData, TimingPtr->tWR, &WrEnc) != mrcSuccess) {
          Status = mrcWrongInputParameter;
        }

        if (EncodeReadLatencyLpddr5 (MrcData, TimingPtr->tCL, &RlEnc) != mrcSuccess) {
          Status = mrcWrongInputParameter;
        }
        Mr2.Bits.RlAndRbtp = RlEnc;
        Mr2.Bits.WriteRecovery = WrEnc;
        MrPtr[mrIndexMR2] = Mr2.Data8;

        //MR19 - Default is 0's. DVFSC and DVFSQ set based on feature enable status.
        Mr19.Data8 = 0;
        if (Outputs->IsDvfsqEnabled) {
          Mr19.Bits.DVFSQ = 1;
        }

        if (Outputs->IsDvfscEnabled) {
          Mr19.Bits.DVFSC = 0x2;  // Only E-DVFSC is supported
        }
        if (Outputs->LpX) {
          Mr19.Bits.CsOdt = CsOdtEnc;
        }
        MrPtr[mrIndexMR19] = Mr19.Data8;

        //MR21 - Default is 0's: All write functions disabled.

        //MR28 - Default is 0's: Normal Reset Operation, Normal Stop Operation, 32ms interval, and Background Calibration Mode.

        //MR30 - Default is 0's: No DCA adjustment

        //MR37
        Mr37.Data8 = 0;
        Mr37.Bits.Wck2DqiInterval = 4; // Stop after 64 clocks
        MrPtr[mrIndexMR37] = Mr37.Data8;

        //MR40
        Mr40.Data8 = 0;
        Mr40.Bits.Wck2DqoInterval = 4; // Stop after 64 clocks
        MrPtr[mrIndexMR40] = Mr40.Data8;

        //MR41 - PPRE = 0 (Disabled)
        Mr41.Data8 = 0;
        Mr41.Bits.NtDqOdt = NtOdtEnc;
        MrPtr[mrIndexMR41] = Mr41.Data8;

        //mpcZqLat
        MrPtr[mrIndexMpcZqLat] = MRC_LP5_MPC_ZQCAL_LATCH;

        if (Status != mrcSuccess) {
          break;
        }
      }
    }
  }
  return Status;
}

/**
  This function returns the impact to Write Latency for the requested LPDDR_ODTL_PARAM.

  @param[in]  MrcData - Pointer to MRC global data.
  @param[in]  Frequency - Data Rate.
  @param[in]  OdtlParam - Select between On or Off timing.
  @param[in]  Lp5BankOrg - Select the bank orginizaiton

  @retval INT8 - Timing impact in nCK.
**/
INT8
MrcGetWrOdtlLpddr5 (
  IN  MrcParameters *const MrcData,
  IN  MrcFrequency         Frequency,
  IN  LPDDR_ODTL_PARAM     OdtlParam,
  IN  MRC_LP5_BANKORG      Lp5BankOrg
  )
{
  MrcOutput  *Outputs;
  INT8        RetVal;

  Outputs = &MrcData->Outputs;

  if (OdtlParam == LpOdtlOn) {
    // JEDEC Spec: Table 257 - ODTLon and ODTLoff Latency Values
    if (Frequency > f8533) {
      RetVal = -5;
    } else if (Frequency > f6400) {
      RetVal = -4;
    } else if (Frequency > f3733) {
      RetVal = -3;
    } else if (Frequency > f1600) {
      RetVal = -2;
    } else {
      RetVal = -1;
    }
  } else {
    if (Lp5BankOrg == MrcLp5BgMode) {
      if (Outputs->BurstLength == 2) {
        RetVal = 3;
      } else {
        RetVal = 7;
      }
    } else if (Lp5BankOrg == MrcLp516Bank) {
      // If Burst length is 16. tCK in 4:1 is 8 UI's per clock
      if (Outputs->BurstLength == 2) {
        RetVal = 3;
      } else {
        RetVal = 5;
      }
    } else {
      // LP5 8Bank
      RetVal = 5;
    }
  }

  return RetVal;
}

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
  )
{
  INT8  RetVal;

  if (OdtlParam == LpOdtlOn) {
    if (Frequency >= f4267) {
      RetVal = -5;
    } else if (Frequency >= f2133) {
      RetVal = -4;
    } else {
      RetVal = -3;
    }
    // if RDQS is disabled, add 2.
    // If RDQS is enabled and MR10[4:5] != 3 add 1
  } else {
    RetVal = 6;
    // if RDQS is disabled, add 1.
  }

  return RetVal;
}

/**
  This function returns tWCKPRE_Static for both Writes and Reads in 4:1 mode.
  The implementation is based off of Table 26/27 WCK2CK Sync AC Parameters for Write/Read Operation

  @param[in]  Frequency - Data Rate.

  @retval INT8 - Timing in tCK
**/
INT8
MrcGetWckPreStaticLpddr5 (
  IN  MrcFrequency  Frequency
  )
{
  INT8  RetVal;

  if (Frequency > f8533) {
    RetVal = 7;
  } else if (Frequency > f7500) {
    RetVal = 6;
  } else if (Frequency > f6400) {
    RetVal = 5;
  } else if (Frequency > f4800) {
    RetVal = 4;
  } else if (Frequency > f3200) {
    RetVal = 3;
  } else if (Frequency > f1600) {
    RetVal = 2;
  } else {
    RetVal = 1;
  }

  return RetVal;
}

// Module scope array defining the list of MRs that must be configured on a
// per-rank basis due to possible unique values per rank. The list is terminated with mrEndOfSequence
static MrcModeRegister mSagvMrPerRankLpddr5[] = {
  mrMR11, // DQ_ODT, NT_ODT, CA_ODT
  mrMR12, // VREF_CA
  mrMR14, // VREF_DQ[7:0]
  mrMR15, // VREF_DQ[15:8]
  mrMR17, // CK_ODT, CS_ODT, CA_ODT
  mrMR18, // WCK_ODT
  mrMR24, // DFE
  mrMR30, // DIMM DCA (WCK)
  mrMR69, // DIMM DCA (Read)
  mrEndOfSequence
};

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
  )
{
  MrcOutput           *Outputs;
  const MRC_FUNCTION  *MrcCall;
  const MrcInput      *Inputs;
  MrcTiming*          Timing;
  UINT32              TimingNck;
  UINT32              tCK;
  UINT64              tZQCALfs;
  UINT32              tFC;
#ifdef MRC_DEBUG_PRINT
  MrcDebug  *Debug;

  Debug = &MrcData->Outputs.Debug;
#endif

  Outputs = &MrcData->Outputs;
  Inputs  = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;
  Timing = &Outputs->Timing[Inputs->ExtInputs.Ptr->MemoryProfile];

  if (TimingNckOut == NULL) {
    return mrcWrongInputParameter;
  }

  tCK = MrcData->Outputs.MemoryClock;
  switch ((GmfLpddr5DelayType) DelayType) {
    case GmfLpddr5Delay_tODTUP:
      // LPDDR5 "tODTUP"
      // 250ns
      TimingNck = LPDDR5_CA_ODT_DELAY * 1000; // Convert Picoseconds to Femtoseconds
      TimingNck = DIVIDECEIL (TimingNck, tCK);
      break;

    case GmfLpddr5Delay_tVREFCA:
      // LPDDR5 "tVREFCA_LONG"
      // 250ns
      TimingNck = MRC_LP5_tVREFCA_LONG_NS * FEMTOSECONDS_PER_NANOSECOND;
      TimingNck = DIVIDECEIL (TimingNck, tCK);
      break;

    case GmfLpddr5Delay_tFC:
      tFC = MrcGetLpddr5Tfc (MrcData->Outputs.Frequency);
      TimingNck = (tFC * FEMTOSECONDS_PER_NANOSECOND) + DIVIDECEIL(tCK, 2);
      TimingNck = DIVIDECEIL (TimingNck, tCK);
      break;

    case GmfLpddr5Delay_tVRCG:
      // tVRCG_ENABLE  = 150ns
      // tVRCG_DISABLE = 100ns
      // Delay max (tVRCG_ENABLE, tVRCG_DISABLE, tFC_Long) to avoid checking the MR value
      TimingNck = MAX (MRC_LP5_tVRCG_ENABLE_NS, MRC_LP5_tVRCG_DISABLE_NS) * FEMTOSECONDS_PER_NANOSECOND;
      TimingNck = DIVIDECEIL (TimingNck, tCK);
      break;

    case GmfLpddr5Delay_tZQRESET:
      /// If DVFSQ enabled, need tZQRESET or tZQSTOP delay.
      /// Otherwise, it will not set MR28[0], so normal tMRW or tMRD delay is sufficient.
      /// For simplicity, always programming the longest delay of 50ns.
      /// tZQRESET = MAX(50ns,3nCK)
      /// tZQSTOP = 30 ns
      TimingNck = DIVIDECEIL (tZQRESET_FS, tCK);
      break;

    case GmfLpddr5Delay_MR28:
      /// If old GV point sets ZQ Stop (MR28[1] = 1), then when new GV point writes MR28 and clears ZQ stop bit
      /// (MR28[1] = 0), it will need to delay tZQCALx.
      /// MR28 is written before frequency switch, and before frequency switch, timing is based on the register value
      /// programmed for the old GV point, therefore the long delay tZQCALx is needed if and only if the prior GV point
      /// sets tZQSTOP = 1, which only DVFSQ enabled point does.
      if (Outputs->IsDvfsqEnabled) {
        /// tZQCALx depends on NZQ count:
        ///   (NZQ <= 4) 1.5us
        ///   (4 < NZQ <= 8) 3us
        ///   (8 < NZQ <= 16) 6us
        if (Inputs->Lp5NZQCount <= 4) {
          tZQCALfs = tZQCAL_4NZQ_FS_LPDDR5;
        } else if (Inputs->Lp5NZQCount <= 8) {
          tZQCALfs = tZQCAL_8NZQ_FS_LPDDR5;
        } else {
          tZQCALfs = tZQCAL_16NZQ_FS_LPDDR5;
        }
        TimingNck = (UINT32)(MrcCall->MrcDivU64x64 (tZQCALfs, tCK, NULL));
      } else {
        // LPDDR5 "tMRW"
        // max(10ns, 5nCK)
        TimingNck = DIVIDECEIL (tMRW_LPDDR_FS, tCK);
        TimingNck = MAX (TimingNck, 5);
      }
      break;

    case GmfLpddr5Delay_tZQLAT:
      // tZQLAT = MAX(30ns,4nCK)
      TimingNck = MrcGetLpddr5Tzqlat (tCK);
      break;

    //    Delay Time from ZQ Stop Bit Set to ZQ Resistor
    case GmfLpddr5Delay_tMRW:
      // LPDDR5 "tMRW"
      // max(10ns, 5nCK)
      TimingNck = MrcGetLpddr5tMRW (tCK);
      break;

    case GmfLpddr5Delay_tMRD:
      // tMRD
      TimingNck = MrcGetLpddr5Tmod(tCK);
      break;

    case GmfLpddr5Delay_tREFab:
      TimingNck = Timing->tRFC; // in tCK units
      break;

    case GmfLpddr5Delay_tPreAll:
      TimingNck = Timing->tRPab; // in tCK units
      break;

    default:
      TimingNck = 0;
      break;
  }

  MRC_DEBUG_ASSERT (TimingNck <= MRC_UINT16_MAX, Debug, "%s: TimingNckOut Overflow\n", gErrString);
  if (TimingNck > MRC_UINT16_MAX) {
    return mrcTimingError;
  }

  *TimingNckOut = (UINT16) TimingNck;
  return mrcSuccess;
}

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
  IN  MrcParameters *const MrcData,
  IN  MrcModeRegister      MrAddr,
  OUT GmfLpddr5DelayType   *DelayTypeOut,
  IN  BOOLEAN              IsLastEntry
  )
{
  GmfLpddr5DelayType DelayType;

  if (DelayTypeOut == NULL) {
    return mrcWrongInputParameter;
  }

  switch (MrAddr) {
    case mrMR17:
    case mrMR11:
      DelayType = GmfLpddr5Delay_tODTUP;
      break;

    case mrMR12:
    case mrMR12b:
      DelayType = GmfLpddr5Delay_tVREFCA;
      break;

    case mrMR16FspOp:
      DelayType = GmfLpddr5Delay_tFC;
      break;

    case mrMR16:
      DelayType = GmfLpddr5Delay_tVRCG;
      break;

    case mrMR28ZqReset:
    case mrMR28ZqStop:
      DelayType = GmfLpddr5Delay_tZQRESET;
      break;

    case mrMR28:
      DelayType = GmfLpddr5Delay_MR28;
      break;

    case mpcZqLat:
      DelayType = GmfLpddr5Delay_tZQLAT;
      break;

    case mrREFab:
      DelayType = GmfLpddr5Delay_tREFab;
      break;

    case mrPreAll:
      DelayType = GmfLpddr5Delay_tPreAll;
      break;


    default:
      if (IsLastEntry) {
        // Delay a minimum of tMRD for the last entry
        DelayType = GmfLpddr5Delay_tMRD;
      } else {
        DelayType = GmfLpddr5Delay_tMRW;
      }
      break;
  }

  *DelayTypeOut = (UINT16) DelayType;
  return mrcSuccess;
}

/**
  This function checks if DRAM is configured to WCK Always-On mode
  by looking at MR18 OP[4] value in the host struct.

  @param[in] MrcData - The MRC general data.

  @retval TRUE  - If WCK is in always_on mode.
  @retval FALSE - If WCK is not in always_on mode.
**/
BOOLEAN
IsWckInAlwaysOnMode (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput                    *Outputs;
  MrcChannelOut                *ChannelOut;
  UINT32                       FirstController;
  UINT32                       FirstChannel;
  LPDDR5_MODE_REGISTER_18_TYPE Mr18;

  Outputs         = &MrcData->Outputs;
  FirstController = Outputs->FirstPopController;
  FirstChannel    = Outputs->Controller[FirstController].FirstPopCh;
  ChannelOut      = &Outputs->Controller[FirstController].Channel[FirstChannel];

  Mr18.Data8 = ChannelOut->Dimm[dDIMM0].Rank[rRank0].MR[mrIndexMR18]; // LP5 is 1DPC only, and Rank0 must be present in any populated channel
  return (Mr18.Bits.WckAlwaysOn == 1);
}

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
  )
{
  static const MrcModeRegister SagvMrOrder[] = {
    mrPreAll, mrREFab, mrMR127, mrMR16, mrMR17, mrMR10, mrMR11, mrMR12, mrMR12b, mrMR14, mrMR15,
    mrMR18, mrMR3, mrMR1, mrMR2, mrMR19, mrMR30, mrMR69, mrMR37, mrMR41, mrMR24, mrMR58, mrMR16FspOp, mrREFab
    };
  MrcModeRegister    CurMrAddr;
  MrcOutput          *Outputs;
  MrcStatus          Status;
  UINT32             Index;
  UINT32             OutIdx;
  UINT8              SagvMrLen;
  BOOLEAN            ByteMode;
  BOOLEAN            IsFirstRefab;
  GmfLpddr5DelayType DelayType;
#ifdef MRC_DEBUG_PRINT
  MrcDebug  *Debug;

  Debug = &MrcData->Outputs.Debug;
#endif

  Outputs = &MrcData->Outputs;
  Status  = mrcSuccess;
  SagvMrLen = ARRAY_COUNT (SagvMrOrder);
  ByteMode = MrcData->Outputs.LpByteMode;
  IsFirstRefab = TRUE;

  if ((MrSeq == NULL) || (MrDelay == NULL) || (Length == NULL)) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "MrcSagvMrSeqLpddr5: %s %s\n", gErrString, gNullPtrErrStr);
    Status = mrcFail;
  } else if (*Length < SagvMrLen) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "MrcSagvMrSeqLpddr5: %s %s\n", gErrString, gArrayOverflow);
    Status = mrcFail;
  } else {
    for (Index = 0, OutIdx = 0; Index < SagvMrLen; Index++) {
      CurMrAddr = SagvMrOrder[Index];

      // MRS FSM to send MRW to the non-existing MR127. MC internal modeling will decode it and change it to CAS(WCK_OFF) + NOP
      // Should be done just for LP5 and if WCK is in always_on mode (MR18 OP[4]=1).
      // Storage value is a don't care.
      // MRC can allocate storage for the command or share storage pointer with any other command.
      // As for the flow, MR127 should be executed after the 1st REFab and before FSP-WR (MR16 programming)
      if (CurMrAddr == mrMR127) {
        if (!Outputs->IsLpddr5 || !IsWckInAlwaysOnMode (MrcData)) {
          continue;   // Remove mrMR127 from the sequence if not LP5 or CAS-OFF is not needed
        }
      }

      if (!ByteMode && (CurMrAddr == mrMR12b)) {
        continue;
      }
      if (MrcData->Outputs.IsZqDisabled && ((CurMrAddr == mpcZqLat) || (CurMrAddr == mrMR28ZqReset) || (CurMrAddr == mrMR28ZqStop))) {
        continue;   // Skip ZQ-related commands
      }
      MrSeq[OutIdx] = CurMrAddr;
      Status = Lpddr5GmfDelayType (
                 MrcData,
                 CurMrAddr,
                 &DelayType,
                 Index == (ARRAY_COUNT (SagvMrOrder) - 1)
                 );
      if (Status != mrcSuccess) {
        break;
      }

      // In order to save latency during SAGV transition, and following Jedec spec which indicates
      // "MRW commands are allowed after Refresh command registered until tRFCab(min) expires."
      // Delay will set to the minumum (GmfLpddr5Delay_tMRW) for the first REFab command in MRS FSM config.
      if (CurMrAddr == mrREFab) {
        if (IsFirstRefab) {
          DelayType = GmfLpddr5Delay_tMRW;
          IsFirstRefab = FALSE;
        }
      }

      MrDelay[OutIdx] = (GmfTimingIndex) DelayType;
      OutIdx++;
      *Length = OutIdx;
    }
  }
  // MrPerRank is an optional return pointer and may be NULL
  if (MrPerRank != NULL) {
    *MrPerRank = mSagvMrPerRankLpddr5;
  }

  return Status;
}

/**
  This function returns tWCKENL_FS in 4:1 mode.
  The implementation si based off of Table 28 WCK2CK Sync AC Parameters for CAS(WS_FAST)

  @param[in]  Frequency - Data Rate.

  @retval UINT8 - Timing in tCK
**/
UINT8
MrcGetWckEnlFsLpddr5 (
  IN  MrcFrequency  Frequency
  )
{
  INT8  RetVal;

  if (Frequency > f6400) {
    RetVal = 3;
  } else if (Frequency > f4267) {
    RetVal = 2;
  } else if (Frequency > f1067) {
    RetVal = 1;
  } else {
    RetVal = 0;
  }

  return RetVal;
}

/**
  This function returns tWCKPRE_total_WR.
  The implementation is based off of Table 26 WCK2CK Sync AC Parameters for Write Operation

  @param[in]  Frequency - Data rate.

  @retval UINT8 - Timing in tCK.
**/
UINT8
MrcGetWckPreWrTotalLpddr5 (
  IN  MrcFrequency  Frequency
  )
{
  UINT8 RetVal;

  if (Frequency <= f1600) {
    RetVal = 3;
  } else if (Frequency <= f3200) {
    RetVal = 4;
  } else if (Frequency <= f4800) {
    RetVal = 5;
  } else if (Frequency <= f6400) {
    RetVal = 6;
  } else if (Frequency <= f7500) {
    RetVal = 7;
  } else if (Frequency <= f8533) {
    RetVal = 8;
  } else {
    RetVal = 9;
  }

  return RetVal;
}

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
  IN  MrcFrequency          Frequency
  )
{
  static const UINT8 tWckPreTotalRd[LP5_TFR_NUM_ENTRIES] =  {4, 5, 5, 6, 7, 7, 8, 9, 9, 10, 11, 11, 14, 16, 18, 18};
  INT8  RetVal;
  UINT8 FreqBin;

  FreqBin = GetFreqBinIndex (Frequency);

  MRC_DEBUG_ASSERT (FreqBin < LP5_TFR_NUM_ENTRIES, &MrcData->Outputs.Debug, "FreqBin %u is larger than LP5_TFR_NUM_ENTRIES (%u)!\n", FreqBin, LP5_TFR_NUM_ENTRIES);

  RetVal = tWckPreTotalRd[FreqBin];

  return RetVal;
}

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
  )
{
  MrcStatus Status;
  LPDDR5_MODE_REGISTER_16_TYPE  *Mr16;

  Status  = mrcSuccess;
  Mr16 = (LPDDR5_MODE_REGISTER_16_TYPE *) MrData;

  if (FspWrite != MRC_IGNORE_ARG_8) {
    Mr16->Bits.FspWr = FspWrite;
  }

  if (FspOpMode != MRC_IGNORE_ARG_8) {
    Mr16->Bits.FspOp = FspOpMode;
  }

  if (CbtMode != MRC_IGNORE_ARG_8) {
    Mr16->Bits.CbtEn = CbtMode;
  }

  if (VrcgMode != MRC_IGNORE_ARG_8) {
    Mr16->Bits.Vrcg = VrcgMode;
  }

  if (CbtPhase != MRC_IGNORE_ARG_8) {
    Mr16->Bits.CbtPhase = CbtPhase;
  }

  return Status;
}


/**
  Set Tx and CA DIMM Vref absolute Value for LPDDR5.

  @param[in]      MrcData       - Include all MRC global data.
  @param[in]      VrefType      - The type of Vref to set
  @param[in]      Controller    - Selecting which Controller to talk to.
  @param[in]      Channel       - Channel to program.
  @param[in]      Rank          - Selecting which Rank to talk to.
  @param[in]      Offset        - Vref Offset to program.
  @param[in]      UpdateMrcData - Update MRC host struct.

  @retval mrcSuccess              - Within programmable range.
  @retval mrcWrongInputParameter  - Not in programmable range.
  @retval mrcDeviceBusy           - MRH could not complete MR write.
**/
MrcStatus
MrcSetVrefLpddr5 (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 VrefType,
  IN  UINT8                 Controller,
  IN  UINT8                 Channel,
  IN  UINT8                 Rank,
  IN  INT32                 Offset,
  IN  BOOLEAN               UpdateMrcData
  )
{
  MrcOutput     *Outputs;
  MrcChannelOut *ChannelOut;
#if (defined (MRC_MEMORY_API_DEBUG) && (MRC_MEMORY_API_DEBUG == SUPPORT))
  MrcDebug    *Debug;
#endif
  MrcStatus   Status;
  UINT8       RankMask;
  UINT8       VrefCode;
  UINT8       *MrPtr;
  UINT8       MrAddress;
  UINT8       MrIndex;
  UINT8       ByteSel;
  LPDDR5_MODE_REGISTER_12_TYPE  Lpddr5Mr12;
  LPDDR5_MODE_REGISTER_14_TYPE  Lpddr5Mr14;
#if defined(LOCAL_STUB_STATE_FLAG)
  MRC_LOCAL_STUB_STATE_STRUCT   LsState;
#endif

  Status     = mrcSuccess;
  Outputs    = &MrcData->Outputs;
#if (defined (MRC_MEMORY_API_DEBUG) && (MRC_MEMORY_API_DEBUG == SUPPORT))
  Debug      = &Outputs->Debug;
#endif
  ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
  RankMask   = ChannelOut->ValidRankBitMask;
  Lpddr5Mr12.Data8 = 0;
  Lpddr5Mr14.Data8 = 0;

  if (RankMask &  (1 << Rank)) {
    MrPtr = ChannelOut->Dimm[dDIMM0].Rank[Rank % MAX_RANK_IN_DIMM].MR;

    Status = OffsetToMRencoding (MrcData, Offset, &VrefCode);
    MRC_API_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " Mc%u.Ch%u.Rank%d, Requested Offset %d, VrefCode 0x%X\n", Controller, Channel, Rank, Offset, VrefCode);

    if (VrefType == CmdV) {
      MrAddress = mrMR12;
      // run twice for Byte mode
      for (ByteSel = 0; ByteSel < (Outputs->LpByteMode ? 2 : 1); ByteSel++) {
        MrIndex = mrIndexMR12 + ByteSel; // mrIndexMR12 + 1 = mrIndexMR12Upper
        Lpddr5Mr12.Bits.CaVref = VrefCode;
#if defined(LOCAL_STUB_STATE_FLAG)
        LsState.Data = MrcReadCR64 (MrcData, MRC_LOCAL_STUB_STATE_REG);
        LsState.Bits.CmdVrefSetting = VrefCode;
        MrcWriteCR64 (MrcData, MRC_LOCAL_STUB_STATE_REG, LsState.Data);
#endif
        Lpddr5Mr12.Bits.VrefByteSel = ByteSel;
        Status = MrcIssueMrw (
                  MrcData,
                  Controller,
                  Channel,
                  Rank,
                  MrAddress,
                  Lpddr5Mr12.Data8,
                  FALSE   // DebugPrint
                  );
        if (Status != mrcSuccess) {
          return Status;
        }
        if (UpdateMrcData) {
          MrPtr[MrIndex] = Lpddr5Mr12.Data8;
        }
        MrcWait (MrcData, DIVIDECEIL (MRC_LP5_tVREFCA_LONG_NS, MRC_TIMER_1NS));
      }
    } else { // WrV
      Lpddr5Mr14.Bits.DqVref = VrefCode;
      for (ByteSel = 0; ByteSel < Outputs->SdramCount; ByteSel++) {
        // Byte mode device doesn't support VDLC function (VREF DQ Lower byte copy).
        // SOC need to set MR14 and MR15 to individual devices.
        if (ByteSel == 1) {
          MrAddress = mrMR15; //DqVref for [15:8]
          MrIndex   = mrIndexMR15;
        } else {
          MrAddress = mrMR14; //DqVref for [7:0]
          MrIndex   = mrIndexMR14;
        }
        Status = MrcIssueMrw (
                  MrcData,
                  Controller,
                  Channel,
                  Rank,
                  MrAddress,
                  Lpddr5Mr14.Data8,
                  FALSE   // DebugPrint
                  );
        if (Status != mrcSuccess) {
          return Status;
        }
        if (UpdateMrcData) {
          MrPtr[MrIndex] = Lpddr5Mr14.Data8;
        }
        MrcWait (MrcData, DIVIDECEIL (MRC_LP5_tVREFCA_LONG_NS, MRC_TIMER_1NS));
      } // ByteSel loop
    } // else WrV
  } // if RankMask &  (1 << Rank)
  return Status;
}

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
  )
{
  MrcDebug      *Debug;
  MrcOutput     *Outputs;
  MrcChannelOut *ChannelOut;
  UINT8         *MrPtr;
  UINT8         Rank;
  UINT8         MrIndex;
  INT32         CurrentOffset;
  MrcStatus     Status;
  LPDDR5_MODE_REGISTER_14_TYPE Lpddr5VrefMr; // Register struct is common to CaVref and TxVref

  Outputs            = &MrcData->Outputs;
  Debug              = &Outputs->Debug;
  ChannelOut         = &Outputs->Controller[Controller].Channel[Channel];
  Status             = mrcSuccess;
  MrIndex            = 0;
  CurrentOffset      = 0;
  Lpddr5VrefMr.Data8 = 0;

  switch (VrefType) {
    case CmdV:
      MrIndex = mrIndexMR12;
      break;

    case WrV:
      MrIndex = mrIndexMR14;
      break;

    default:
      Status = mrcWrongInputParameter;
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %u ", gWrongInputParam, VrefType);
      break;
  }

  if (Status != mrcSuccess) {
    return Status;
  }

  for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
    if ((MrcRankExist (MrcData, Controller, Channel, Rank)) && (RankMask & (1 << Rank))) {
      MrPtr = ChannelOut->Dimm[dDIMM0].Rank[Rank % MAX_RANK_IN_DIMM].MR;

      if (IsCachedOffsetParam) {
        // Offset value.  Calculate the new Vref.
        Lpddr5VrefMr.Data8 = MrPtr[MrIndex];
        MrcVrefEncToOffsetLpddr5 (MrcData, Lpddr5VrefMr.Bits.DqVref, &CurrentOffset);
        CurrentOffset += Offset;
      } else {
        // For Non-Cached OffsetParam set value as-is
        CurrentOffset = Offset;
      }
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NONE, "Lpddr5SetDramVref - Mc%u.Ch%u.R%u RankMask = 0x%x  IsCachedOffsetParam:%d, MR14: 0x%02X, CurrentOffset: %d, Offset: %d\n",
                     Controller, Channel, Rank, RankMask, IsCachedOffsetParam, Lpddr5VrefMr.Data8, CurrentOffset, Offset);
      MrcSetVrefLpddr5 (MrcData, VrefType, Controller, Channel, Rank, CurrentOffset, UpdateMrcData);
    }

  }
  return Status;
}

/**
  This function converts from 2s complement value to LPDDR5 MR DCA encoding (MR30 for LPDDR5).

  @param[in]  DcaValue - 2s complement value.

  @retval UINT8 - Encoding if valid DCA value.  Else, 0xFF.
**/
UINT8
LpddrDcaEncode (
  IN  INT16  DcaValue
  )
{
  UINT8  EncodeVal;

  if ((DcaValue < -LP5_DCA_VALID_RANGE) || (DcaValue > LP5_DCA_VALID_RANGE)) {
    EncodeVal = 0xFF;
  } else if (DcaValue <= 0) {
    //Negative (include 0)
    EncodeVal = (UINT8) (ABS (DcaValue));
  } else {
    //Positive (exclude 0)
    EncodeVal = (UINT8) (DcaValue) | 0x8;
  }

  return EncodeVal;
}

/**
  This function will set up the pointer passed in based on LPDDR5 Mode Register definition.
  If MRC_IGNORE_ARGx is passed in, that parameter is ignored.
  PdDrvStr does error checking against spec valid values.

  @param[in]      MrcData     - Pointer to global MRC data.
  @param[in]      PdDrvStr    - Pull Down Drive Strength value to set (in Ohms).  Does not support infinity (0xFFFF) as RFU.
  @param[in,out]  MrData      - Pointer to MR data to update.

  @retval MrcStatus - mrcSuccess if a supported ODT value, else mrcWrongInputParameter.
**/
MrcStatus
MrcLpddr5SetMr3 (
  IN  MrcParameters *const  MrcData,
  IN  UINT16                PdDrvStr,
  IN OUT  UINT8     *const  MrData
  )
{
  MrcDebug  *Debug;
  MrcStatus Status;
  INT8      Encoding;
  LPDDR5_MODE_REGISTER_3_TYPE  *Mr3;

  Status  = mrcSuccess;
  Mr3    = (LPDDR5_MODE_REGISTER_3_TYPE *) MrData;
  Debug   = &MrcData->Outputs.Debug;

  if (PdDrvStr != MRC_IGNORE_ARG_8) {
    Encoding = LpddrOdtEncode (PdDrvStr);

    if (Encoding == -1) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid %s Value: %u\n", gErrString, gDrvStr, PdDrvStr);
      Status  = mrcWrongInputParameter;
    } else {
      Mr3->Bits.PullDnDrvStr  = Encoding;
    }
  }

  return Status;
}

/**
  This function will set up the pointer passed in based on LPDDR5 Mode Register definition.
  If MRC_IGNORE_ARG8 is passed in, that parameter is ignored.
  DqOdt and CaOdt does error checking against spec valid values.

  @param[in]      MrcData   - Pointer to global MRC data.
  @param[in]      DqOdt     - Data ODT value to set (in Ohms).
  @param[in]      DqNTOdtEn - Enable / Disable Data NT ODT.
  @param[in]      CaOdt     - Command/Address ODT value to set (in Ohms).
  @param[in,out]  MrData    - Pointer to MR data to update.

  @retval MrcStatus - mrcSuccess if a supported ODT value, else mrcWrongInputParameter.
**/
MrcStatus
MrcLpddr5SetMr11 (
  IN      MrcParameters *const  MrcData,
  IN      UINT16                DqOdt,
  IN      UINT16                DqNTOdtEn,
  IN      UINT16                CaOdt,
  IN OUT  UINT8         *const  MrData
  )
{
  MrcDebug  *Debug;
  MrcStatus Status;
  INT8      Encoding;
  LPDDR5_MODE_REGISTER_11_TYPE  *Mr11;

  Status  = mrcSuccess;
  Mr11    = (LPDDR5_MODE_REGISTER_11_TYPE *) MrData;
  Debug   = &MrcData->Outputs.Debug;

  if (DqOdt != MRC_IGNORE_ARG_8) {
    Encoding = LpddrOdtEncode (DqOdt);

    if (Encoding == -1) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid %s ODT Value %u\n", gErrString, gDataString, DqOdt);
      Status  = mrcWrongInputParameter;
    } else {
      Mr11->Bits.DqOdt = Encoding;
    }
  }
  if (DqNTOdtEn != MRC_IGNORE_ARG_8) {
    if (DqNTOdtEn > 1) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid %s ODT Value %u\n", gErrString, "Enable NT", DqNTOdtEn);
      Status = mrcWrongInputParameter;
    } else {
      Mr11->Bits.NtOdtEn = (UINT8) DqNTOdtEn;
    }
  }
  if (CaOdt != MRC_IGNORE_ARG_8) {
    Encoding = LpddrOdtEncode (CaOdt);

    if (Encoding == -1) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid %s ODT Value %u\n", gErrString, gCmdString, CaOdt);
      Status  = mrcWrongInputParameter;
    } else {
      Mr11->Bits.CaOdt = Encoding;
    }
  }

  return Status;
}

/**
  This function will set up the pointer passed in based on LPDDR5 Mode Register definition.
  If MRC_IGNORE_ARG8 is passed in, that parameter is ignored.
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
  IN OUT  UINT8  *const         MrData
  )
{
  MrcDebug  *Debug;
  MrcStatus Status;
  INT8      Encoding;
  LPDDR5_MODE_REGISTER_41_TYPE *Mr41;

  Status = mrcSuccess;
  Mr41 = (LPDDR5_MODE_REGISTER_41_TYPE *) MrData;
  Debug = &MrcData->Outputs.Debug;

  if (DqNTOdt != MRC_IGNORE_ARG_8) {
    Encoding = LpddrOdtEncode (DqNTOdt);

    if (Encoding == -1) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid %s ODT Value %u\n", gErrString, "NT", DqNTOdt);
      Status = mrcWrongInputParameter;
    } else {
      Mr41->Bits.NtDqOdt = Encoding;
    }
  }

  return Status;
}

/*
  This function will set up the pointer passed in based on LPDDR5 Mode Register definition.
  If MRC_IGNORE_ARG8 is passed in, that parameter is ignored.
  SocOdt does error checking against spec valid values.

  @param[in]      MrcData - Pointer to global MRC data.
  @param[in]      SocOdt  - SOC ODT value to set (in Ohms).
  @param[in,out]  MrData  - Pointer to MR data to update.

  @retval MrcStatus - mrcSuccess if a supported ODT value, else mrcWrongInputParameter.
**/
MrcStatus
MrcLpddr5SetMr17 (
  IN      MrcParameters *const  MrcData,
  IN      UINT16                SocOdt,
  IN OUT  UINT8         *const  MrData
  )
{
  MrcDebug  *Debug;
  MrcStatus Status;
  INT8      Encoding;
  LPDDR5_MODE_REGISTER_17_TYPE  *Mr17;

  Status  = mrcSuccess;
  Mr17    = (LPDDR5_MODE_REGISTER_17_TYPE *) MrData;
  Debug   = &MrcData->Outputs.Debug;

  if (SocOdt != MRC_IGNORE_ARG_8) {
    Encoding = LpddrOdtEncode (SocOdt);

    if (Encoding == -1) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid %s ODT Value 0x%x\n", gErrString, gSocString, SocOdt);
      Status  = mrcWrongInputParameter;
    } else {
      Mr17->Bits.SocOdt  = Encoding;
    }
  }

  return Status;
}

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
MrcLpddr5SetMr30 (
  IN      MrcParameters *const  MrcData,
  IN      UINT16                DcaLower, OPTIONAL
  IN      UINT16                DcaUpper, OPTIONAL
  IN OUT  UINT8         *const  MrData
  )
{
  MrcDebug  *Debug;
  MrcStatus Status;
  UINT8     Encoding;
  LPDDR5_MODE_REGISTER_30_TYPE  *Mr30;

  Status  = mrcSuccess;
  Mr30    = (LPDDR5_MODE_REGISTER_30_TYPE *) MrData;
  Debug   = &MrcData->Outputs.Debug;

  if (DcaLower != MRC_IGNORE_ARG_16_2) {
    Encoding = LpddrDcaEncode ((INT16) DcaLower);

    if (Encoding == 0xFF) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid DCA Value (%d)\n", gErrString, DcaLower);
      Status = mrcWrongInputParameter;
    } else {
      Mr30->Bits.DcaLowByte  = Encoding;
    }
  }
  if (DcaUpper != MRC_IGNORE_ARG_16_2) {
    Encoding = LpddrDcaEncode ((INT16) DcaUpper);

    if (Encoding == 0xFF) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid DCA Value (%d)\n", gErrString, DcaUpper);
      Status = mrcWrongInputParameter;
    } else {
      Mr30->Bits.DcaUpperByte  = Encoding;
    }
  }

  return Status;
}

/**
  This function will set up the pointer passed in based on LPDDR5 Mode Register definition.
  If MRC_IGNORE_ARG16 is passed in, that parameter is ignored.

  @param[in]      MrcData - Pointer to global MRC data.
  @param[in]      DFELower - DFE value to program to Lower Byte.
  @param[in]      DFEUpper - DFE value to program to Upper Byte.
  @param[in,out]  MrData  - Pointer to MR data to update.

  @retval MrcStatus - mrcSuccess if a supported ODT value, else mrcWrongInputParameter.
**/
MrcStatus
MrcLpddr5SetMr24 (
  IN      MrcParameters *const  MrcData,
  IN      UINT16                DFELower,
  IN      UINT16                DFEUpper,
  IN OUT  UINT8         *const  MrData
  )
{
  MrcDebug  *Debug;
  MrcStatus Status;
  LPDDR5_MODE_REGISTER_24_TYPE  *Mr24;

  Status  = mrcSuccess;
  Mr24    = (LPDDR5_MODE_REGISTER_24_TYPE *) MrData;
  Debug   = &MrcData->Outputs.Debug;

  if (DFELower != MRC_IGNORE_ARG_16) {
    if (DFELower > LPDDR5_DIMM_DFE_TAP_RANGE) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid DFE Value (%d)\n", gErrString, DFELower);
      Status = mrcWrongInputParameter;
    } else {
      Mr24->Bits.Dfeql  = (UINT8) DFELower;
    }
  }
  if (DFEUpper != MRC_IGNORE_ARG_16) {
    if (DFEUpper > LPDDR5_DIMM_DFE_TAP_RANGE) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid DFE Value (%d)\n", gErrString, DFEUpper);
      Status = mrcWrongInputParameter;
    } else {
      Mr24->Bits.Dfequ  = (UINT8) DFEUpper;
    }
  }

  return Status;
}


/**
  This function will set up the pointer passed in based on LPDDR5 Mode Register definition.
  If MRC_IGNORE_ARG8 is passed in, that parameter is ignored.
  DqUpEmLB, DqDnEmLB, DqUpEmUB, DqDnEmUB does error checking against spec valid values.

  @param[in]      MrcData   - Pointer to global MRC data.
  @param[in]      DqUpEmLB  - DQ pull-up pre-emphasis lower Byte.
  @param[in]      DqDnEmLB  - DQ pull-down pre-emphasis lower Byte.
  @param[in]      DqUpEmUB  - DQ pull-up pre-emphasis upper Byte.
  @param[in]      DqDnEmUB  - DQ pull-down pre-emphasis upper Byte.
  @param[in,out]  MrData    - Pointer to MR data to update.

  @retval MrcStatus - mrcSuccess if a supported ODT value, else mrcWrongInputParameter.
**/
MrcStatus
MrcLpddr5SetMr58(
  IN      MrcParameters* const  MrcData,
  IN      UINT16                DqUpEmLB,
  IN      UINT16                DqDnEmLB,
  IN      UINT16                DqUpEmUB,
  IN      UINT16                DqDnEmUB,
  IN OUT  UINT8* const          MrData
)
{
  MrcDebug* Debug;
  MrcStatus Status;

  LPDDR5_MODE_REGISTER_58_TYPE* Mr58;

  Status = mrcSuccess;
  Mr58 = (LPDDR5_MODE_REGISTER_58_TYPE*)MrData;
  Debug = &MrcData->Outputs.Debug;

  if (DqUpEmLB != MRC_IGNORE_ARG_8) {
    if (DqUpEmLB > LPDDR5_EMPHASIS_RANGE) {
      MRC_DEBUG_MSG(Debug, MSG_LEVEL_ERROR, "%s Invalid DqUpEmLB Pre Emphasis Value %u\n", gErrString, DqUpEmLB);
      Status = mrcWrongInputParameter;
    } else {
      Mr58->Bits.PUEmphasisLower = (UINT8)DqUpEmLB;
    }
  }

  if (DqDnEmLB != MRC_IGNORE_ARG_8) {
    if (DqDnEmLB > LPDDR5_EMPHASIS_RANGE) {
      MRC_DEBUG_MSG(Debug, MSG_LEVEL_ERROR, "%s Invalid DqDnEmLB Pre Emphasis Value %u\n", gErrString, DqDnEmLB);
      Status = mrcWrongInputParameter;
    } else {
      Mr58->Bits.PDEmphasisLower = (UINT8)DqDnEmLB;
    }
  }

  if (DqUpEmUB != MRC_IGNORE_ARG_8) {
    if (DqUpEmUB > LPDDR5_EMPHASIS_RANGE) {
      MRC_DEBUG_MSG(Debug, MSG_LEVEL_ERROR, "%s Invalid DqUpEmUB Pre Emphasis Value %u\n", gErrString, DqUpEmUB);
      Status = mrcWrongInputParameter;
    } else {
      Mr58->Bits.PUEmphasisUpper = (UINT8)DqUpEmUB;
    }
  }

  if (DqDnEmUB != MRC_IGNORE_ARG_8) {
    if (DqDnEmUB > LPDDR5_EMPHASIS_RANGE) {
      MRC_DEBUG_MSG(Debug, MSG_LEVEL_ERROR, "%s Invalid DqDnEmUB Pre Emphasis Value %u\n", gErrString, DqDnEmUB);
      Status = mrcWrongInputParameter;
    } else {
      Mr58->Bits.PDEmphasisUpper = (UINT8)DqDnEmUB;
    }
  }

  return Status;
}

/**
  This function will encode the pointer being passed in to the correct
  MR value based on input parameters and LPDDR5 MR69 encoding requirements
  If MRC_IGNORE_ARG_16_2 is passed in, that parameter is ignored.

  @param[in]      MrcData - Pointer to global MRC data.
  @param[in]      ReadDcaLower - 2s complement value to program to Lower Byte.
  @param[in]      ReadDcaUpper - 2s complement value to program to Upper Byte.
  @param[in,out]  MrData  - Pointer to MR data to update.

  @retval MrcStatus - mrcSuccess if a supported MR value, else mrcWrongInputParameter.
**/
MrcStatus
MrcLpddr5SetMr69 (
  IN      MrcParameters* const  MrcData,
  IN      UINT16                ReadDcaLower, OPTIONAL
  IN      UINT16                ReadDcaUpper, OPTIONAL
  IN OUT  UINT8* const          MrData
)
{
  MrcDebug* Debug;
  MrcStatus Status;
  UINT8     Encoding;
  LPDDR5_MODE_REGISTER_69_TYPE* Mr69;

  Status = mrcSuccess;
  Mr69 = (LPDDR5_MODE_REGISTER_69_TYPE*)MrData;
  Debug = &MrcData->Outputs.Debug;

  if (ReadDcaLower != MRC_IGNORE_ARG_16_2) {
    Encoding = LpddrDcaEncode ((INT16)ReadDcaLower);

    if (Encoding == 0xFF) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid Read DCA Value (%d)\n", gErrString, ReadDcaLower);
      Status = mrcWrongInputParameter;
    } else {
      Mr69->Bits.ReadDcaLowByte = Encoding;
    }
  }
  if (ReadDcaUpper != MRC_IGNORE_ARG_16_2) {
    Encoding = LpddrDcaEncode ((INT16)ReadDcaUpper);

    if (Encoding == 0xFF) {
      MRC_DEBUG_MSG(Debug, MSG_LEVEL_ERROR, "%s Invalid Read DCA Value (%d)\n", gErrString, ReadDcaUpper);
      Status = mrcWrongInputParameter;
    } else {
      Mr69->Bits.ReadDcaUpperByte = Encoding;
    }
  }

  return Status;
}

/**
  Lpddr5 Set DimmParamValue is responsible for performing the concrete set DIMM parameter to value,
  using Lpddr specific MR set functions.
  Parameters supported: OptDimmRon, OptDimmOdtWr

  @param[in,out]  MrcData         - Include all MRC global data.
  @param[in,out]  MrData          - Pointer to the MR data to update.
  @param[in]      OptParam        - The Dimm Opt Param (e.g., OptDimmRon, OptDimmOdtWr, OptDimmOdtPark, OptDimmOdtNom)
  @param[in]      ParamValue      - The actual values (Typically in Ohms)

  @retval MrcStatus - mrcWrongInputParameter if unsupported OptParam, MrcStatus of the MR set functions otherwise

**/
MrcStatus
Lpddr5SetDimmParamValue (
  IN OUT MrcParameters *const MrcData,
  IN OUT UINT8         *const MrData,
  IN     UINT8                OptParam,
  IN     UINT16               ParamValue
)
{
  MrcDebug        *Debug;
  MrcStatus       Status;

  Debug = &MrcData->Outputs.Debug;

  switch (OptParam) {

  case OptDimmRon:
    // DIMM RON
    Status = MrcLpddr5SetMr3 (MrcData, ParamValue, MrData);
    break;

  case OptDimmOdtWr:
    // DIMM RTT WR
    Status = MrcLpddr5SetMr11 (MrcData, ParamValue, MRC_IGNORE_ARG_8, MRC_IGNORE_ARG_8, MrData);
    break;

 case OptDimmNTOdtEn:
    // DIMM NT ODT EN
    Status = MrcLpddr5SetMr11 (MrcData, MRC_IGNORE_ARG_8, ParamValue, MRC_IGNORE_ARG_8, MrData);
    break;

  case OptDimmNTOdt:
    // DIMM NT ODT
    Status = MrcLpddr5SetMr41 (MrcData, ParamValue, MrData);
    break;

  case OptDimmOdtCA:
    // DIMM ODT CA
    Status = MrcLpddr5SetMr11 (MrcData, MRC_IGNORE_ARG_8, MRC_IGNORE_ARG_8, ParamValue, MrData);
    break;

  case OptDimmSocOdt:
    // SOC ODT
    Status = MrcLpddr5SetMr17 (MrcData, ParamValue, MrData);
    break;

  case OptLp5WckDcaBoth:
    //LP5 WCK DCA Both Upper and Lower Byte
    Status = MrcLpddr5SetMr30 (MrcData, ParamValue, ParamValue, MrData);
    break;

  case OptLp5WckDcaUpper:
    //LP5 WCK DCA Upper Byte
    Status = MrcLpddr5SetMr30 (MrcData, MRC_IGNORE_ARG_16_2, ParamValue, MrData);
    break;

  case OptLp5WckDcaLower:
    //LP5 WCK DCA Lower Byte
    Status = MrcLpddr5SetMr30 (MrcData, ParamValue, MRC_IGNORE_ARG_16_2, MrData);
    break;
  case OptLp5ReadDcaBoth:
    //LP5 Read DCA Both Upper and Lower Byte
    Status = MrcLpddr5SetMr69 (MrcData, ParamValue, ParamValue, MrData);
    break;

  case OptLp5ReadDcaUpper:
    //LP5 Read DCA Upper Byte
    Status = MrcLpddr5SetMr69 (MrcData, MRC_IGNORE_ARG_16_2, ParamValue, MrData);
    break;

  case OptLp5ReadDcaLower:
    //LP5 Read DCA Lower Byte
    Status = MrcLpddr5SetMr69 (MrcData, ParamValue, MRC_IGNORE_ARG_16_2, MrData);
    break;

  case OptLpddr5DimmDFETapBoth:
    // LP5 DIMM DFE Upper and Lower Byte
    Status = MrcLpddr5SetMr24 (MrcData, ParamValue, ParamValue, MrData);
    break;

  case OptLpddr5DimmDFETapLower:
    // LP5 DIMM DFE Lower Byte
    Status = MrcLpddr5SetMr24 (MrcData, ParamValue, MRC_IGNORE_ARG_16, MrData);
    break;

  case OptLpddr5DimmDFETapUpper:
    // LP5 DIMM DFE Upper Byte
    Status = MrcLpddr5SetMr24 (MrcData, MRC_IGNORE_ARG_16, ParamValue, MrData);
    break;

  case OptLpddr5EmphasisUpBoth:
    // LP5 Emphasis DQ UP
    Status = MrcLpddr5SetMr58(MrcData, ParamValue, MRC_IGNORE_ARG_8, ParamValue, MRC_IGNORE_ARG_8, MrData);
    break;

  case OptLpddr5EmphasisDnBoth:
    // LP5 Emphasis DQ Dn
    Status = MrcLpddr5SetMr58(MrcData, MRC_IGNORE_ARG_8, ParamValue, MRC_IGNORE_ARG_8, ParamValue, MrData);
    break;

  case OptLpddr5EmphasisUpLB:
    // LP5 Emphasis DQ UP
    Status = MrcLpddr5SetMr58(MrcData, ParamValue, MRC_IGNORE_ARG_8, MRC_IGNORE_ARG_8, MRC_IGNORE_ARG_8, MrData);
    break;

  case OptLpddr5EmphasisDnLB:
    // LP5 Emphasis DQ Dn
    Status = MrcLpddr5SetMr58(MrcData, MRC_IGNORE_ARG_8, ParamValue, MRC_IGNORE_ARG_8, MRC_IGNORE_ARG_8, MrData);
    break;

  case OptLpddr5EmphasisUpUB:
    // LP5 Emphasis DQ UP
    Status = MrcLpddr5SetMr58(MrcData, MRC_IGNORE_ARG_8, MRC_IGNORE_ARG_8, ParamValue, MRC_IGNORE_ARG_8, MrData);
    break;

  case OptLpddr5EmphasisDnUB:
    // LP5 Emphasis DQ Dn
    Status = MrcLpddr5SetMr58(MrcData, MRC_IGNORE_ARG_8, MRC_IGNORE_ARG_8, MRC_IGNORE_ARG_8, ParamValue, MrData);
    break;

  default:
    Status = mrcWrongInputParameter;
    MRC_DEBUG_MSG(Debug, MSG_LEVEL_ERROR, "%s %u ", gWrongInputParam, OptParam);
    break;
  }

  return Status;
}


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
)
{
  MrcDebug          *Debug;
  MrcStatus         Status;

  Status = mrcSuccess;
  Debug = &MrcData->Outputs.Debug;

  switch (OptDimmParam) {

  case OptDimmRon:
    // DIMM RON
    *MrIndex = mrIndexMR3;
    *MrNum = mrMR3;
    break;

  case OptDimmOdtCA:
  case OptDimmOdtWr:
  case OptDimmNTOdtEn:
    // DIMM RTT WR, NT-ODT enable or DIMM ODT CA
    *MrIndex = mrIndexMR11;
    *MrNum = mrMR11;
    break;

  case OptDimmSocOdt:
    // SOC ODT
    *MrIndex = mrIndexMR17;
    *MrNum = mrMR17;
    break;

   case OptDimmNTOdt:
    // DIMM RTT NT
    *MrIndex = mrIndexMR41;
    *MrNum = mrMR41;
    break;

  case OptLpddr5DimmDFETapBoth:
  case OptLpddr5DimmDFETapLower:
  case OptLpddr5DimmDFETapUpper:
    // DIMM DFE
    *MrIndex = mrIndexMR24;
    *MrNum   = mrMR24;
    break;

  case OptLp5WckDcaBoth:
  case OptLp5WckDcaUpper:
  case OptLp5WckDcaLower:
    //LP5 WCK DCA
    *MrIndex = mrIndexMR30;
    *MrNum   = mrMR30;
    break;

  case OptLpddr5EmphasisUpBoth:
  case OptLpddr5EmphasisDnBoth:
  case OptLpddr5EmphasisUpLB:
  case OptLpddr5EmphasisDnLB:
  case OptLpddr5EmphasisUpUB:
  case OptLpddr5EmphasisDnUB:
    //LP5 Pre Emphasis
    *MrIndex = mrIndexMR58;
    *MrNum = mrMR58;
    break;

  case OptLp5ReadDcaBoth:
  case OptLp5ReadDcaUpper:
  case OptLp5ReadDcaLower:
    //LP5 Read DCA
    *MrIndex = mrIndexMR69;
    *MrNum = mrMR69;
    break;

  default:
    *MrIndex = 0xFF;
    *MrNum = 0xFF;
    Status = mrcWrongInputParameter;
    MRC_DEBUG_MSG(Debug, MSG_LEVEL_ERROR, "%s %u ", gWrongInputParam, OptDimmParam);
    break;

  }

  return Status;
}

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
  )
{
  static const UINT16    Lpddr5DimmRonVals[] = { 240, 120, 80, 60, 48, 40 };              // LPDDR5 Pull Down Drive Strength in Ohms
  static const UINT16    Lpddr5DimmRttWrVals[] = { 0, 240, 120, 80, 60, 48, 40 };         // LPDDR5 RttWr Values in Ohms
  static const UINT16    Lpddr5DimmRttNtVals[] = { 0xFFFF, 0, 240, 120, 80, 60, 48, 40 }; // LPDDR5 RttNt Values in Ohms, first 0xFFFF means NtODT disabled
  MrcDebug               *Debug;
  MrcStatus              Status;

  Debug = &MrcData->Outputs.Debug;
  Status = mrcSuccess;

  switch (DimmOptParam) {
  case OptDimmOdtCA:
  case OptDimmOdtWr:
    // DIMM RTT_WR / DIMM CA_ODT
    *DimmOptParamVals = (UINT16 *)Lpddr5DimmRttWrVals;
    *NumDimmOptParamVals = ARRAY_COUNT(Lpddr5DimmRttWrVals);
    break;

  case OptDimmNTOdt:
    // NT_ODT
    *DimmOptParamVals = (UINT16 *) Lpddr5DimmRttNtVals;
    *NumDimmOptParamVals = ARRAY_COUNT (Lpddr5DimmRttNtVals);
    break;

  case OptDimmRon:
    // DIMM RON
    *DimmOptParamVals = (UINT16 *)Lpddr5DimmRonVals;
    *NumDimmOptParamVals = ARRAY_COUNT(Lpddr5DimmRonVals);
    break;

  case OptDimmOdtNom:
  case OptDimmOdtPark:
  default:
    *DimmOptParamVals = (UINT16 *)NULL;
    *NumDimmOptParamVals = 0;
    Status = mrcWrongInputParameter;
    MRC_DEBUG_MSG(Debug, MSG_LEVEL_ERROR, "%s %u ", gWrongInputParam, DimmOptParam);
    break;
  }

  return Status;
}

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
  )
{
  MrcStatus                     Status;
  MrcOutput                     *Outputs;
  MrcChannelOut                 *ChannelOut;
  MrcDebug                      *Debug;
  // LPDDR5 Parameters
  LPDDR5_MODE_REGISTER_11_TYPE  Lp5ModeRegister11;
  LPDDR5_MODE_REGISTER_3_TYPE   Lp5ModeRegister3;
  LPDDR5_MODE_REGISTER_41_TYPE  Lp5ModeRegister41;

  UINT32                        Dimm;
  UINT8                         *MR;
  UINT16                        *DimmOptParamValues;
  UINT8                         DimmOptParamMaxIndex;
  TOdtValueLpddr                LpddrOdtTableIndex;
  UINT16                        Impedance;
  UINT8                         MrIndex;
  UINT16                        MrNum;
  UINT8                         MrValue;

  Outputs     = &MrcData->Outputs;
  ChannelOut  = &Outputs->Controller[Controller].Channel[Channel];
  Debug       = &Outputs->Debug;
  Dimm        = RANK_TO_DIMM_NUMBER (Rank);

  DimmOptParamMaxIndex = 0;
  DimmOptParamValues  = NULL;
  Impedance = 0;  // Odt off

  if (GetFromTable) {
    Status = GetOdtTableIndex (MrcData, Controller, Channel, Dimm, &LpddrOdtTableIndex);
    if (Status != mrcSuccess) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s: Retrieving OdtTableIndex failed!\n", gErrString);
      return 0;
    }
    if (OptParam == OptDimmOdtCA) {
      Impedance = LpddrOdtTableIndex.RttCa;
    } else if (OptParam == OptDimmOdtWr) {
      Impedance = LpddrOdtTableIndex.RttWr;
    } else if (OptParam == OptDimmNTOdt) {
      Impedance = LpddrOdtTableIndex.RttNT;
    }
  } else { // else GetFromTable == FALSE
    MR = ChannelOut->Dimm[(Rank / 2) % MAX_DIMMS_IN_CHANNEL].Rank[Rank % 2].MR;
    Status = Lpddr5GetOptDimmParamMrIndex(MrcData, OptParam, &MrIndex, &MrNum);
    if (Status != mrcSuccess) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s: %s - %u!\n", gErrString, gWrongInputParam, OptParam);
      return 0;
    }
    MrValue = MR[MrIndex];

    Status = Lpddr5GetDimmOptParamValues(MrcData, OptParam, &DimmOptParamValues, &DimmOptParamMaxIndex);
    if (Status != mrcSuccess || DimmOptParamValues == NULL) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s: DimmOptParamValues is NULL!\n", gErrString);
      return 0;
    }

    // DimmOptParamMaxIndex need -1 before being used as MAX Index
    DimmOptParamMaxIndex--;
    switch (OptParam) {
      case OptDimmOdtWr:
        Lp5ModeRegister11.Data8 = MrValue;
        Impedance = Override ? OverrideValue : DimmOptParamValues[RANGE (Lp5ModeRegister11.Bits.DqOdt, 0, DimmOptParamMaxIndex)];
        break;
      case OptDimmNTOdt:
        Lp5ModeRegister41.Data8 = MrValue;
        Impedance = Override ? OverrideValue : DimmOptParamValues[RANGE(Lp5ModeRegister41.Bits.NtDqOdt, 0, DimmOptParamMaxIndex)];
        break;
      case OptDimmRon:
      case OptDimmRonUp:
      case OptDimmRonDn:
        // Extract PullDnDrvStr
        Lp5ModeRegister3.Data8 = MrValue;
        Impedance = Override ? OverrideValue : DimmOptParamValues[RANGE (Lp5ModeRegister3.Bits.PullDnDrvStr - 1, 0, DimmOptParamMaxIndex)];
        break;
      case OptDimmOdtCA:
        Lp5ModeRegister11.Data8 = MrValue;
        Impedance = Override ? OverrideValue : DimmOptParamValues[RANGE (Lp5ModeRegister11.Bits.CaOdt, 0, DimmOptParamMaxIndex)];
        Impedance /= 2;
        break;
      default:
        break;
    } // switch OptParam
  } //end else GetFromTable

  return Impedance;
}

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
  )
{
  UINT32          Controller;
  UINT8           Channel;
  UINT8           Rank;
  UINT32          MrAddr;
  UINT8           *MrPtr;
  MrcStatus       Status;
  MrcOutput       *Outputs;
  MrcDebug        *Debug;
  UINT8           MrIndex;
  Outputs         = &MrcData->Outputs;
  Status          = mrcSuccess;
  Debug           = &Outputs->Debug;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if (!(MrcChannelExist (MrcData, Controller, Channel))) {
        continue;
      }
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!(MrcRankExist (MrcData, Controller, Channel, Rank))) {
          continue;
        }
        MrPtr = Outputs->Controller[Controller].Channel[Channel].Dimm[Rank / MAX_RANK_IN_DIMM].Rank[Rank % MAX_RANK_IN_DIMM].MR;
        MrIndex = mrIndexMR37;
        MrAddr = mrMR37;
        MrPtr[MrIndex] = DqioDuration;
        // Set Interval timer run time setting
        Status = MrcIssueMrw (
                      MrcData,
                      Controller,
                      Channel,
                      Rank,
                      MrAddr,
                      MrPtr[MrIndex],
                      MRC_PRINTS_ON
                      );
        if (Status != mrcSuccess) {
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "MC%u:Ch%u:R%u unable to set MR%d\n", Controller, Channel, Rank, MrAddr);
        }
      } // for Rank
    } // for Channel
  } // for Controller
  return Status;
}

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
  )
{
  MrcInput *Inputs;

  Inputs = &MrcData->Inputs;

  if (Inputs->Lp58BankMode) {
    return MrcLp58Bank; //8-Bank Mode
  } else if ((Frequency > f3200) && !Inputs->ExtInputs.Ptr->DqLoopbackTest) {
    return MrcLp5BgMode; // BG Mode
  } else {
    return MrcLp516Bank; // 16-Bank Mode
  }
}

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
  )
{
  MrcOutput *Outputs;
  MrcChannelOut *ChannelOut;
  UINT32    Controller;
  UINT32    Channel;
  UINT32    Rank;
  UINT32    MrAddr;
  UINT32    Device;
  UINT8     MrrResult[MRC_MRR_ARRAY_SIZE];
  UINT32    MaxDeviceNumber;
  LPDDR5_MODE_REGISTER_24_TYPE Mr24;

  Outputs = &MrcData->Outputs;
  MrAddr = mrMR24;
  *IsDFESupported = *IsDFEEnabled = TRUE;
  MaxDeviceNumber = (Outputs->LpByteMode) ? 2 : 1; // In ByteMode we have two x8 devices per channel

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
         continue;
        }
        ChannelOut  = &Outputs->Controller[Controller].Channel[Channel];
        MrcIssueMrr (MrcData, Controller, Channel, Rank, MrAddr, MrrResult);
        for (Device = 0; Device < MaxDeviceNumber; Device++) {
          Mr24.Data8 = MrrResult[Device];
          *IsDFESupported &= (Mr24.Bits.Dfes == 1) ? TRUE : FALSE;

        }
        // Read DFE Quantities from cache
        Mr24.Data8 = ChannelOut->Dimm[dDIMM0].Rank[Rank % MAX_RANK_IN_DIMM].MR[mrIndexMR24];
        *IsDFEEnabled &= ((Mr24.Bits.Dfeql != 0) || (Mr24.Bits.Dfequ != 0)) ? TRUE : FALSE;
      }
    }
  }
}

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
  )
{
  UINT32    tXSR;
  UINT32    MinCK;
  UINT32    tXSOffset;
  //
  // Units here are in Fempto Seconds.
  // 7.5ns for LP5, rest are 10 ns.
  //
  tXSOffset = 7500 * 1000;
  tXSOffset = DIVIDECEIL (tXSOffset, tCK);
  MinCK = 2;
  tXSOffset = MAX (tXSOffset, MinCK);
  //
  // Current LPDDR SPD places tRFCab in tRFC entry.
  //
  tXSR = tRFC + tXSOffset;

  return tXSR;
}

/**
  This function returns the tRFM in DCLKs.
  LPDDR5 timing is based on tRFMpb

  @param[in] DeviceCapacity - DRAM Device Capacity

  @retval tRFM - tRFM timing based on Device Capacity
**/
UINT32
MrcGetLpddr5Trfm (
  IN UINT32 DeviceCapacity
  )
{
  UINT32  tRFM;

  switch (DeviceCapacity) {
    case 8192:
      tRFM = MRC_LP5_tRFMPB_8GB_PS;
      break;
    case 12288:
      tRFM = MRC_LP5_tRFMPB_12GB_PS;
      break;
    case 16384:
      tRFM = MRC_LP5_tRFMPB_16GB_PS;
      break;
    case 24576:
      tRFM = MRC_LP5_tRFMPB_24GB_PS;
      break;
    case 32768:
      tRFM = MRC_LP5_tRFMPB_32GB_PS;
      break;
    default:
      tRFM = MRC_LP5_tRFMPB_8GB_PS;
      break;
  }

  return tRFM;
}

/**
  This function returns the tMOD value.

  @param[in] tCK  - DCLK period in femtoseconds.

  @retval UINT32  - Timing in tCK.
**/
UINT32
MrcGetLpddr5Tmod (
  IN const  UINT32  tCK
  )
{
  UINT32 tMRD;

  // tMRD = MAX (14ns, 5nCK)
  tMRD = DIVIDECEIL (tMOD_LPDDR_FS, tCK);
  tMRD = MAX (tMRD, 5);
  return tMRD;
}

/**
  This function returns the tMRW value.
  @param[in] tCK  - DCLK period in femtoseconds.
  @retval UINT32  - Timing in tCK.
**/
UINT32
MrcGetLpddr5tMRW (
  IN const  UINT32  tCK
  )
{
  UINT32 tMRW;

 // max(10ns, 5nCK)
 tMRW = DIVIDECEIL (tMRW_LPDDR_FS, tCK);
 tMRW = MAX (tMRW, 5);
 return tMRW;
}

/**
  This function returns the LPDDR5 tZQCAL value in DCLKs.

  @param[in]  MrcData  - Pointer to global MRC data.
  @param[in]  tCK      - DCLK period in femtoseconds.

  @retval tZQCAL  - Timing in nCK.
**/
UINT32
MrcGetLpddr5Tzqcal (
  IN  MrcParameters  *MrcData,
  IN UINT32  tCK
  )
{
  MrcInput  *Inputs;
  const MRC_FUNCTION *MrcCall;
  UINT64     tZQCALfs;
  UINT64     tZQCAL;

  Inputs = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;

  if (Inputs->Lp5NZQCount <= 4) {
    tZQCALfs = tZQCAL_4NZQ_FS_LPDDR5;
  } else if (Inputs->Lp5NZQCount <= 8) {
    tZQCALfs = tZQCAL_8NZQ_FS_LPDDR5;
  } else {
    tZQCALfs = tZQCAL_16NZQ_FS_LPDDR5;
  }

  tZQCAL = MrcCall->MrcDivU64x64 (tZQCALfs + tCK - 1, tCK, NULL);

  return (UINT32) tZQCAL;
}

/**
  This function returns the LPDDR5 tZQLAT value in tCK

  @param[in] tCK  - CK period in femtoseconds.

  @retval tZQLAT  - Timing in tCK.
**/
UINT32
MrcGetLpddr5Tzqlat (
  IN UINT32 tCK
  )
{
  UINT32   Zqlat;

  //tZQLAT = max (30ns, 4nCK)
  Zqlat = DIVIDECEIL (tZQLAT_LP5_FS, tCK);
  Zqlat = MAX (Zqlat, tZQLAT_LP5_MIN_NCK);

  return Zqlat;
}

/**
  Return the RDQS Postamble Length encoding for MR10

  @return RDQS Postamble Length encoding for MR10
**/
LPDDR5_MR10_RDQS_PST_LENGTH
MrcLp5GetRdqsPostambleLength (
  VOID
  )
{
  // Use 0.5 tWCK postamble for all configurations / speeds
  return Lp5tRPST_0p5_tWck;
}

/**
  Program CS ODT OP in MR11 if CS ODT is supported.

  @param[in] MrcData - The global host structure

  @return MrcStatus - mrcSuccess.
**/
MrcStatus
MrcMr11WriteCsOdtOp (
  IN MrcParameters* const MrcData
)
{
  UINT8                         MrrResult[MRC_MRR_ARRAY_SIZE];
  UINT8                         CsOdtSupport;
  LPDDR5_MODE_REGISTER_1_TYPE   Lpddr5MR1;
  LPDDR5_MODE_REGISTER_11_TYPE  Lpddr5MR11;
  MrcOutput                     *Outputs;
  MrcChannelOut                 *ChannelOut;
  UINT8                         *MrPtr;
  UINT32                        Controller;
  UINT32                        Channel;
  UINT32                        Rank;
  MrcModeRegister               MrAddr;
  MrcModeRegisterIndex          MrIndex;

  Outputs = &MrcData->Outputs;
  MrAddr = mrMR11;
  CsOdtSupport = 0;

  // Translate from MrcModeRegister to Host structure index.MrIndex is used to update MrValues stored in MRC data structures.
  MrIndex = MrcMrAddrToIndex(MrcData, &MrAddr);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if (!(MrcChannelExist (MrcData, Controller, Channel))) {
        continue;
      }
      ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!(MrcRankExist (MrcData, Controller, Channel, Rank))) {
          continue;
        }

        if (CsOdtSupport != 1) {
          // Read MR1[OP0] to check if CS ODT behavior option is supported
          MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR1, MrrResult);
          Lpddr5MR1.Data8 = MrrResult[0];
          CsOdtSupport = Lpddr5MR1.Bits.CsOdtOpSupport;
        }
        MrPtr = ChannelOut->Dimm[dDIMM0].Rank[Rank % MAX_RANK_IN_DIMM].MR;
        Lpddr5MR11.Data8 = MrPtr[MrIndex];

        if (CsOdtSupport == 1) {         // Check if CS ODT is supported
          Lpddr5MR11.Bits.CsOdtOP = 1;   // Setting bit OP[7] to high for CS ODT behavior option

          // Update Mr11 in host struct
          MrPtr[MrIndex] = Lpddr5MR11.Data8;
          MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR11, Lpddr5MR11.Data8, MRC_PRINTS_OFF);
        } else {
          return mrcSuccess;
        }
      }
    }
  }
  MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_NOTE, "MR11[7] is set\n");
  return mrcSuccess;
}

/**
  Check if conditions are met to optimize MC tXP value.

  @param[in]  MrcData       - The global host structure
  @param[out] IfOptimizeTxp - True if CS ODT behavior option is enabled (MR OP[7] = 1)

  @return MrcStatus - mrcSuccess.
**/
MrcStatus
MrcIfOptimizeTxp (
  IN  MrcParameters* const MrcData,
  OUT BOOLEAN              *IsOptimizeTxp
)
{
  UINT8                         MrrResult[MRC_MRR_ARRAY_SIZE];
  LPDDR5_MODE_REGISTER_1_TYPE   Lpddr5MR1;
  LPDDR5_MODE_REGISTER_17_TYPE  Lpddr5MR17;
  MrcOutput *Outputs;
  UINT8     FirstController;
  UINT8     FirstChannel;
  MrcStatus Status;

  Outputs = &MrcData->Outputs;
  FirstController   = Outputs->FirstPopController;
  FirstChannel      = Outputs->Controller[FirstController].FirstPopCh;

  *IsOptimizeTxp = FALSE;

  if (Outputs->IsLpddr5) {
    Status = MrcIssueMrr (MrcData, FirstController, FirstChannel, 0, mrMR1, MrrResult);
    Lpddr5MR1.Data8 = MrrResult[0];
    if (Status != mrcSuccess) {
      return Status;
    }

    Status = MrcIssueMrr (MrcData, FirstController, FirstChannel, 0, mrMR17, MrrResult);
    Lpddr5MR17.Data8 = MrrResult[0];
    if (Status != mrcSuccess) {
      return Status;
    }

    *IsOptimizeTxp = TRUE;
    if (Lpddr5MR1.Bits.CsOdtOpSupport == 0 && Lpddr5MR17.Bits.CsOdtDis == 0) {
      *IsOptimizeTxp = FALSE;
    }
  }

  return mrcSuccess;
}

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
  )
{
  MrcStatus      Status;
  MrcDebug*      Debug;
  UINT32         RdPreambleT;
  LPDDR5_MR10_tRPRE ReadPreambleSettings;

  Debug = &MrcData->Outputs.Debug;
  Status = mrcSuccess;

  RdPreambleT = MrcData->Outputs.ReadPreamble;
  if (RdPreambleT == MRC_LP5_tRPRE_TOGGLE_2tWCK) {
    ReadPreambleSettings = Lp5tRPRE_2S_2T_00001010;
  } else if (RdPreambleT == MRC_LP5_tRPRE_TOGGLE_4tWCK) {
    ReadPreambleSettings = Lp5tRPRE_0S_4T_10101010;
  } else if (RdPreambleT == MRC_LP5_tRPRE_TOGGLE_4tWCK_2STATIC) {
    ReadPreambleSettings = Lp5tRPRE_2S_4T_000010101010;
  } else {
    ReadPreambleSettings = 0;
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid Read Preamble Toggle %d\n", gErrString, RdPreambleT);
    Status = mrcWrongInputParameter;
  }

  if (Status == mrcSuccess && EncVal != NULL) {
    *EncVal = ReadPreambleSettings;
  } else {
    // to catch NULL EncVal
    Status = mrcWrongInputParameter;
  }

  return Status;
}

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
  )
{
  LpDdr5ActStruct           LpDdr5ActCommand;
  // LPDDR5 cmd map:
  // DATA[6:0]   = CA[6:0], Cmd1 Rise edge
  // DATA[13:7]  = CA[6:0], Cmd1 Fall edge
  // DATA[20:14] = CA[6:0], Cmd2 Rise edge
  // DATA[27:21] = CA[6:0], Cmd2 Fall edge
  // Refer to Command Truth Table for Shifting Operations
  switch (MrCmd) {
    case MrDramCmdMrw:
      *DramCmdData = ((Data & 0x7F) << 21) | ((Data & 0x80) << (20 - 7)) | (LP5_MRW_CMD_RISE_EDGE2 << 14) | ((Address & 0x7F) << 7) | LP5_MRW_CMD_RISE_EDGE1;
      break;

    case MrDramCmdMpc:
      *DramCmdData = ((Data & 0x7F) << 7) | ((Data & 0x80) >> 1) | LP5_MPC_CMD_RISE_EDGE;
      *IsSingleCycleCmd = TRUE;
      break;

    case MrDramCmdSre:
      *DramCmdData = LP5_SRE_CMD_RISE_EDGE;
      *IsSingleCycleCmd = TRUE;
      break;

    case MrDramCmdNop:
      *DramCmdData = LP5_NOP_CMD_RISE_EDGE;
      break;

    case MrDramCmdPrea:
      *DramCmdData = (LP5_PREA_CMD_FALL_EDGE << 7) | LP5_PREA_CMD_RISE_EDGE;
      *IsSingleCycleCmd = TRUE;
      break;

    case MrDramCmdWsFs:
      *DramCmdData = LP5_WSFS_CMD_RISE_EDGE;
      break;

    case MrDramCmdWsOff:
      *DramCmdData = LP5_WS_OFF_CMD_RISE_EDGE;
      break;

    case MrDramCmdMrr:
      *DramCmdData = (Address << 7) | LP5_MRR_CMD_RISE_EDGE;
      // Mulitcast fall through to defualt
      if (!IsMultiCast) {
        break;
      }

    case MrDramCmdAct:
      LpDdr5ActCommand.Data32 = Data;
      *DramCmdData =
        (LpDdr5ActCommand.Bits.RowBits0_6 << 21) |
        (LpDdr5ActCommand.Bits.RowBits7_10 << 17) |
        (LP5_ACT2_CMD_RISE_EDGE << 14) |
        (LpDdr5ActCommand.Bits.RowBits11_13) << 11 |
        (Address << 7) |
        (LpDdr5ActCommand.Bits.RowBits14_17 << 3) |
        LP5_ACT1_CMD_RISE_EDGE;
      break;

    default:
      MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Invalid MR Command !\n");
      return mrcFail;
  }
  return mrcSuccess;
}

/**
  This function returns the tRRFpb in ps.

  @param[in] DeviceCapacity - DRAM Device Capacity in MB

  @retval tRRFpb - tRRFpb timing based on Device Capacity
**/
UINT32
MrcGetLpddr5Trrfpb (
  IN UINT32 DeviceCapacity
  )
{
  UINT32  tRRFpb;

  switch (DeviceCapacity) {
    case 8192:
      tRRFpb = MRC_LP5_tRRFPB_8GB_PS;
      break;
    case 12288:
      tRRFpb = MRC_LP5_tRRFPB_12GB_PS;
      break;
    case 16384:
      tRRFpb = MRC_LP5_tRRFPB_16GB_PS;
      break;
    case 24576:
      tRRFpb = MRC_LP5_tRRFPB_24GB_PS;
      break;
    case 32768:
      tRRFpb = MRC_LP5_tRRFPB_32GB_PS;
      break;
    default:
      tRRFpb = MRC_LP5_tRRFPB_8GB_PS;
      break;
  }

  return tRRFpb;
}

/**
  Get tFC delay which depends on DDR data rate.

  @param[in] Frequency DDR data rate.

  @returns tFC in ns
**/
UINT32
MrcGetLpddr5Tfc (
  MrcFrequency Frequency
)
{
  UINT32 Result;
  if (Frequency > f8533) {
    Result = MRC_LP5_tFC_EXTENDED_NS;
  } else {
    Result = MRC_LP5_tFC_LONG_NS;
  }
  return Result;
}


/**
  This function returns the tPRPDEN value for the specified Memory type.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] DdrFreq  - The memory frequency.

  @retval The tPRPDEN value for the specified configuration.
**/
UINT32
MrcGetTprpden (
  IN       MrcParameters *const MrcData,
  IN const MrcFrequency  DdrFreq
  )
{
  MrcOutput       *Outputs;
  MrcDdrType      DdrType;
  UINT32          tPRPDEN;

  Outputs = &MrcData->Outputs;
  DdrType = Outputs->DdrType;

  switch (DdrType) {
    case MRC_DDR_TYPE_LPDDR5:
      tPRPDEN = MRC_LP5_tCMDPD_MIN_NCK;
      break;

    default:
    case MRC_DDR_TYPE_DDR5:
      tPRPDEN = 2;
      break;
  } // end switch

  return tPRPDEN;
}

/**
  This function returns the tBPR2ACT value for the current Memory type.

  @param[in] MrcData  - Include all MRC global data.

  @retval The tBPR2ACT value for the specified configuration.
**/
UINT32
MrcGetTbpr2act (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput       *Outputs;
  UINT32          tBPR2ACT;
  UINT32          MinDlySbRef2Act;
  MRC_LP5_BANKORG Lp5BGOrg;
  UINT32          TckPs;

  Outputs = &MrcData->Outputs;
  // Convert tCK from Femtoseconds to Picoseconds
  TckPs   = DIVIDECEIL (Outputs->MemoryClock, 1000);

  if (!Outputs->IsLpddr) {
    return 0;
  }

  Lp5BGOrg = MrcGetBankBgOrg (MrcData, Outputs->Frequency);
  MinDlySbRef2Act = (Lp5BGOrg == MrcLp58Bank ? MRC_LP5_tBPR2ACT_8B_PS : MRC_LP5_tBPR2ACT_PS);
  tBPR2ACT = DIVIDECEIL (MinDlySbRef2Act, TckPs);

  return tBPR2ACT;
}

/**
  This function calculates DRAM temp/voltage drift

  @param[in]  MrcData - Pointer to MRC global data.

  @retval UINT32 DramWriteDrift in pS
**/
UINT32
MrcGetDramWriteDrift (
  IN     MrcParameters *const MrcData
  )
{
  MrcOutput    *Outputs;
  MrcFrequency DdrFrequency;
  BOOLEAN      IsDdr5;
  UINT32       Temp;
  UINT32       Volt;
  UINT32       DramWriteDrift;
  UINT32       NomVdd2Mv;

  Outputs         = &MrcData->Outputs;
  IsDdr5          = Outputs->IsDdr5;
  DdrFrequency    = Outputs->Frequency;

  // Assume DRAM Voltage/Temp for different ranks in the same package is < 40 C and  <4% supply noise
  // DramWriteDrift = (IamDDR5?0.85:((DataRate >= 3200 MTs)?((DataRate>=6400 MTs)?0.5:0.6):0.7))pS/C * 40 C +
  //                  (IamDDR5?0.64:((DataRate >= 3200 MTs)?((DataRate>=6400 MTs)?0.4:0.5):1.0))pS/mV * Vdd2 * 0.04
  if (IsDdr5) {
    Temp = 850;
    Volt = 640;
  } else {
    if (DdrFrequency < 3200) {
      Temp = MRC_LP5_tWCK2DQI_TEMP_LF;
      Volt = MRC_LP5_tWCK2DQI_VOLT_LF;
    } else {
      Temp = MRC_LP5_tWCK2DQI_TEMP_HF;
      Volt = MRC_LP5_tWCK2DQI_VOLT_HF;
    }
  }

  // Nominal Vdd2
  NomVdd2Mv = IsDdr5 ? 1117 : 1065;

  DramWriteDrift = (Temp * MAX_TEMP_DRIFT * HUNDRED_MULTIPLIER) + (Volt * NomVdd2Mv * MAX_NOISE_PERCENT);
  DramWriteDrift = UDIVIDEROUND (DramWriteDrift, THOUSAND_MULTIPLIER * HUNDRED_MULTIPLIER);

  return DramWriteDrift;

}

/**
  Calculates different types (roundings) of tWCKDQO parameter:
  - tWCKDQO rounded up,
  - tWCKDQO rounded down (floor),
  - tWCKDQO remainder from rounding.

  @param[in]      MrcData   - Include all MRC global data.
  @param[in]      Type      - Type of tWCKDQO rounding.

  @retval tWCKDQO based on chosen tWCKDQO rounding type.
**/
UINT32 MrcGetTwckdqo (
  IN MrcParameters    *const MrcData,
  IN MRC_TWCKDQO_TYPE  const Type
  )
{
  MrcOutput *Outputs;
  UINT32    tWckDqoMaxPs;
  UINT32    tWckDqoRound;
  UINT32    tWckDqoFloor;
  UINT32    tWckDqoRemPs;
  UINT32    tWckDqoRem;
  UINT32    PhClkPeriod;
  UINT32    Value;

  /*
  tWCKDQO_DECI        = $floor(tWCKDQO / q_period) ;
  tWCKDQO_DECI_RNDUP  = $ceil(tWCKDQO / q_period);
  tWCKDQO_REM_ABS     = tWCKDQO - (tWCKDQO_DECI[pt] * q_period) ;
  tWCKDQO_REM         = real'(tWCKDQO_REM_ABS) / phclk_period * 128
  */

  Outputs = &MrcData->Outputs;
  PhClkPeriod = DIVIDECEIL (2000000, Outputs->Frequency);
  tWckDqoMaxPs = (Outputs->Frequency >= f3200) ? MRC_LP5_tWCKDQO_HF_MAX : MRC_LP5_tWCKDQO_LF_MAX;

  tWckDqoRound = UDIVIDEROUND (tWckDqoMaxPs, Outputs->Qclkps);
  tWckDqoFloor = DIVIDEFLOOR  (tWckDqoMaxPs, Outputs->Qclkps);

  tWckDqoRemPs = tWckDqoMaxPs - (tWckDqoFloor * Outputs->Qclkps);
  tWckDqoRem = UDIVIDEROUND (128 * tWckDqoRemPs, PhClkPeriod);

  if (MrcData->Inputs.ExtInputs.Ptr->DqLoopbackTest) {
    Value = 0;
  } else {
    switch (Type) {
      case MrcLp5tWckDqoFloor:
        Value = tWckDqoFloor;
        break;
      case MrcLp5tWckDqoRemainder:
        Value = tWckDqoRem;
        break;
      case MrcLp5tWckDqoRound:
      default:
        Value = tWckDqoRound;
        break;
    }
  }

  return Value;
}

/**
  This function calculates the LPDDR5 Read Drift

  @param[in]  MrcData - Include all MRC global data.

  @retval DramReadDriftPI - Read Drift in Pi ticks
**/
UINT32
GetLpddr5ReadDrift (
  IN  MrcParameters* const MrcData
  )
{
  MrcOutput      *Outputs;
  UINT32  TempDrift;
  UINT32  VoltDrift;
  UINT32  DramReadDrift;

  Outputs = &MrcData->Outputs;

  if (Outputs->Frequency < f3200) {
    TempDrift = MRC_LP5_tWCK2DQO_TEMP_LF;
    VoltDrift = MRC_LP5_tWCK2DQO_VOLT_LF;
  } else {
    TempDrift = MRC_LP5_tWCK2DQO_TEMP_HF;
    VoltDrift = MRC_LP5_tWCK2DQO_VOLT_HF;
  }

  DramReadDrift = (TempDrift * MAX_TEMP_DRIFT * HUNDRED_MULTIPLIER) + (VoltDrift * 1065 * MAX_NOISE_PERCENT);
  DramReadDrift = UDIVIDEROUND (DramReadDrift, THOUSAND_MULTIPLIER * HUNDRED_MULTIPLIER);

  return DramReadDrift;
}

/**
  This function returns tCCD_S for LPDDR.

  @param[in]  MrcData - Pointer to MRC global data.

  @returns nWR_diff parameter.
**/
UINT32
GetLpddrtCCDS (
  IN MrcParameters *const MrcData
  )
{
  UINT32 Result;
  if (MrcData->Outputs.Frequency > f3200) {
    Result = TCCD_S_LP5_GT3200;
  } else {
    Result = TCCD_LP5_FREQ;
  }

  return Result;
}

/**
  This function returns tCCD_L for LPDDR.

  @param[in]  MrcData - Pointer to MRC global data.

  @returns nWR_diff parameter.
**/
UINT32
GetLpddrtCCDL (
  IN MrcParameters *const MrcData
  )
{
  UINT32 Result;
  if (MrcData->Outputs.Frequency > f3200) {
    Result = TCCD_L_LP5_GT3200;
  } else {
    Result = TCCD_LP5_FREQ;
  }

  return Result;
}

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
  )
{
  UINT32 tCWL;
  UINT32 tCKNorm;

  tCKNorm = tCKmin / 4;
  //
  // Using WL Set B values from table 4.6.2 of LPDDR5 JEDEC Spec.
  //
  if (tCKNorm >= MRC_DDR_533_TCK_MIN) {
    tCWL = 2;
  } else if (tCKNorm >= MRC_DDR_1067_TCK_MIN) {
    tCWL = 3;
  } else if (tCKNorm >= MRC_DDR_1600_TCK_MIN) {
    tCWL = 4;
  } else if (tCKNorm >= MRC_DDR_2133_TCK_MIN) {
    tCWL = 5;
  } else if (tCKNorm >= MRC_DDR_2750_TCK_MIN) {
    tCWL = 7;
  } else if (tCKNorm >= MRC_DDR_3200_TCK_MIN) {
    tCWL = 8;
  } else if (tCKNorm >= MRC_DDR_3733_TCK_MIN) {
    tCWL = 9;
  } else if (tCKNorm >= MRC_DDR_4267_TCK_MIN) {
    tCWL = 11;
  } else if (tCKNorm >= MRC_DDR_4800_TCK_MIN) {
    tCWL = 12;
  } else if (tCKNorm >= MRC_DDR_5500_TCK_MIN) {
    tCWL = 14;
  } else if (tCKNorm >= MRC_DDR_6000_TCK_MIN) {
    tCWL = 15;
  } else if (tCKNorm >= MRC_DDR_6400_TCK_MIN) {
    tCWL = 16;
  } else if (tCKNorm >= MRC_DDR_7500_TCK_MIN) {
    tCWL = 19;
  } else if (tCKNorm >= MRC_DDR_8533_TCK_MIN) {
    tCWL = 22;
  } else if (tCKNorm >= MRC_DDR_9600_TCK_MIN) {
    tCWL = 24; // MRC_DDR_9600_TCK_MIN
  } else {
    tCWL = 26; // MRC_DDR_10667_TCK_MIN
  }
  return tCWL;
}

/**
  Calculate the tCL value for LPDDR5.

  JEDEC Spec Table 225 - Read Latencies for Read Link ECC off case (DVFSC disabled, Enhanced DVFSC disabled):

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


    JEDEC Spec Table 225 - Read Latencies for Read Link ECC off case (DVFSC disabled, Enhanced DVFSC enabled):

    Lower Clk   Upper Clk        Read Latency
    Freq Limit  Freq Limit     Set0  Set1  Set2
    -------------------------------------------
      5            67           3      3      3
     67           133           5      5      5
    133           200           7      7      7
    200           267           8      8     10
    267           344           10    10     12
    344           400           12    12     14


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
  )
{
  UINT32 tCL;
  UINT32 tCKNorm;
  UINT32 RlSet;
  UINT32 Index;
  MrcLp5RlDvfscDisIndex RlIndex;

  // Scale tCK up to typical DDR ratio of 2:1 between tCK and Data Rate
  // We are always in 4:1 mode for WCK.
  tCKNorm = tCK / 4;

  RlSet = 0;
  if (SdramWidth == 8) {
    RlSet += 1;
  }
  if (IsDbiEnabled) {
    RlSet += 1;
  }

  Index = 0;
  if (!IsDvfscEnabled) {
    if (tCKNorm >= MRC_DDR_533_TCK_MIN) {
      RlIndex = MrcLp5RlDvfscDisIndex533;
    } else if (tCKNorm >= MRC_DDR_1067_TCK_MIN) {
      RlIndex = MrcLp5RlDvfscDisIndex1067;
    } else if (tCKNorm >= MRC_DDR_1600_TCK_MIN) {
      RlIndex = MrcLp5RlDvfscDisIndex1600;
    } else if (tCKNorm >= MRC_DDR_2133_TCK_MIN) {
      RlIndex = MrcLp5RlDvfscDisIndex2133;
    } else if (tCKNorm >= MRC_DDR_2750_TCK_MIN) {
      RlIndex = MrcLp5RlDvfscDisIndex2750;
    } else if (tCKNorm >= MRC_DDR_3200_TCK_MIN) {
      RlIndex = MrcLp5RlDvfscDisIndex3200;
    } else if (tCKNorm >= MRC_DDR_3733_TCK_MIN) {
      RlIndex = MrcLp5RlDvfscDisIndex3733;
    } else if (tCKNorm >= MRC_DDR_4267_TCK_MIN) {
      RlIndex = MrcLp5RlDvfscDisIndex4267;
    } else if (tCKNorm >= MRC_DDR_4800_TCK_MIN) {
      RlIndex = MrcLp5RlDvfscDisIndex4800;
    } else if (tCKNorm >= MRC_DDR_5500_TCK_MIN) {
      RlIndex = MrcLp5RlDvfscDisIndex5500;
    } else if (tCKNorm >= MRC_DDR_6000_TCK_MIN) {
      RlIndex = MrcLp5RlDvfscDisIndex6000;
    } else if (tCKNorm >= MRC_DDR_6400_TCK_MIN) {
      RlIndex = MrcLp5RlDvfscDisIndex6400;
    } else if (tCKNorm >= MRC_DDR_7500_TCK_MIN) {
      RlIndex = MrcLp5RlDvfscDisIndex7500;
    } else if (tCKNorm >= MRC_DDR_8533_TCK_MIN) {
      RlIndex = MrcLp5RlDvfscDisIndex8533;
    } else if (tCKNorm >= MRC_DDR_9600_TCK_MIN) {
      RlIndex = MrcLp5RlDvfscDisIndex9600;
    } else {
      RlIndex = MrcLp5RlDvfscDisIndex10667;
    }
    if (RlSet == 0) {
      tCL = Lp5ReadLatencyDvfscDisabledSet0[RlIndex];
    } else if (RlSet == 1) {
      tCL = Lp5ReadLatencyDvfscDisabledSet1[RlIndex];
    } else {
      tCL = Lp5ReadLatencyDvfscDisabledSet2[RlIndex];
    }
  } else {  //Dvfsc enabled
    if (tCKNorm >= MRC_DDR_533_TCK_MIN) {
      Index = 0;
    } else if (tCKNorm >= MRC_DDR_1067_TCK_MIN) {
      Index = 1;
    } else if (tCKNorm >= MRC_DDR_1600_TCK_MIN) {
      Index = 2;
    } else if (tCKNorm >= MRC_DDR_2133_TCK_MIN) {
      Index = 3;
    } else if (tCKNorm >= MRC_DDR_2750_TCK_MIN) {
      Index = 4;
    } else {
      Index = 5;
    }

    if (RlSet == 0) {
      tCL = Lp5ReadLatencyDvfscEnabledSet0[Index];
    } else if (RlSet == 1) {
      tCL = Lp5ReadLatencyDvfscEnabledSet1[Index];
    } else {
      tCL = Lp5ReadLatencyDvfscEnabledSet2[Index];
    }
  }
  return tCL;
}

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
  )
{
  MrcOutput   *Outputs;
  MrcDebug    *Debug;
  MrcStatus   Status;
  UINT8       MrValue;
  UINT8       RlSet;
  UINT8       Index;
  UINT8       ReadLatencyValueArrLength;
  const UINT8 *ReadLatency;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  Status = mrcSuccess;
  RlSet = 0;

  if (Outputs->LpByteMode) {
    RlSet += 1;
  }

  // Use MR2 table from JEDEC spec - "MR2 Register Definition"
  if (!Outputs->IsDvfscEnabled) {
    if (RlSet == 0) {
      ReadLatency = Lp5ReadLatencyDvfscDisabledSet0;
    } else {
      ReadLatency = Lp5ReadLatencyDvfscDisabledSet1;
    }
  } else {
    if (RlSet == 0) {
      ReadLatency = Lp5ReadLatencyDvfscEnabledSet0;
    } else {
      ReadLatency = Lp5ReadLatencyDvfscEnabledSet1;
    }
  }

  MrValue = 0xFF;
  ReadLatencyValueArrLength = (Outputs->IsDvfscEnabled) ? LP5_READ_LATENCY_VALUES_DVFSC : MrcLp5RlDvfscDisIndexMax;
  for (Index = 0; Index < ReadLatencyValueArrLength; Index++) {
    if (ReadLatency[Index] == Value) {
      MrValue = Index;
      break;
    }
  }

  if (MrValue != 0xFF) {
    // Check to see if the time requested matches JEDEC Frequency table
    if (!Outputs->IsDvfscEnabled) {
      Status = LatencyFreqCheckLpddr5 (MrcData, MrValue);
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, (Status != mrcSuccess) ? " (RL)\n" : "");
    }
    if (EncVal != NULL) {
      *EncVal = MrValue;
    } else {
      Status = mrcWrongInputParameter;
    }
  } else {
    Status = mrcWrongInputParameter;
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid %s Latency Value: %d\n", gErrString, gRdString, Value);
  }

  return Status;
}
