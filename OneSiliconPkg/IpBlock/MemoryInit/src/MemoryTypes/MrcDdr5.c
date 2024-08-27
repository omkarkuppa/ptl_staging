/** @file
  Implementation of DDR5 Specific functions, and constants.

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

#include "MrcDdr5.h"  // to check declarations against implementations here (public API)
#include "MrcDdr5Settings.h"
#include "MrcDdr5Private.h" // to check declarations against implementations here (private API)
#include "MrcHalRegisterAccess.h"
#include "CMrcApi.h"
#include "MrcMemoryApi.h"
#include "MrcDdrIoUtils.h"

///
/// Initialization Timing Parameters
///

const MrcModeRegister DFETap1Arrx16[DDR5_DFE_PER_TAP] = { mrMR129, mrMR137, mrMR145, mrMR153, mrMR161, mrMR169, mrMR177, mrMR185,
                                                   mrMR193, mrMR201, mrMR209, mrMR217, mrMR225, mrMR233, mrMR241, mrMR249};
const MrcModeRegister DFETap2Arrx16[DDR5_DFE_PER_TAP] = { mrMR130, mrMR138, mrMR146, mrMR154, mrMR162, mrMR170, mrMR178, mrMR186,
                                                   mrMR194, mrMR202, mrMR210, mrMR218, mrMR226, mrMR234, mrMR242, mrMR250};
const MrcModeRegister DFETap3Arrx16[DDR5_DFE_PER_TAP] = { mrMR131, mrMR139, mrMR147, mrMR155, mrMR163, mrMR171, mrMR179, mrMR187,
                                                   mrMR195, mrMR203, mrMR211, mrMR219, mrMR227, mrMR235, mrMR243, mrMR251};
const MrcModeRegister DFETap4Arrx16[DDR5_DFE_PER_TAP] = { mrMR132, mrMR140, mrMR148, mrMR156, mrMR164, mrMR172, mrMR180, mrMR188,
                                                   mrMR196, mrMR204, mrMR212, mrMR220, mrMR228, mrMR236, mrMR244, mrMR252};

typedef enum {
  GenericFsmConfigNonPdaMrs = 0,
  GenericFsmConfigPdaMrs
} GenericFsmConfigMode;
// DDR5 Single Cycle MPC commands (1 cycle of CA data)
// These commands can be issued prior to EctComplete
const MrcModeRegister JedecInitSequenceSingleCycle[] = {
  mpcSetCmdTiming,
  mpcMR32a0,
  mpcMR32a1,
  mpcMR33a0,
  mpcMR32b0,
  mpcMR32b1,
  mpcMR33b0,
  mrMR11,
  mrMR12,
  mpcMR33,
  mpcMR34,
  mpcApplyVrefCa,
  mrEndOfSequence
};

// DDR5 Multi Cycle MRW commands (2 cycles of CA data)
// These commands can only be issued after EctComplete
const MrcModeRegister JedecInitSequenceMultiCycle[] = {
  mrMR0,
  mrMR2,
  mrMR4,
  mrMR5,
  mrMR6,
  mrMR8,
  mrMR13,
  mrMR34,
  mrMR35,
  mrMR36,
  mrMR37,
  mrMR38,
  mrMR39,
  mrMR40,
  mrMR45,
  // PDA MR's start here
  // IMPORTANT note: the order of the MR's here should be opposite to their training order in MRC call table
  // For example, MR3 is trained as PDA before MR10.
  mrMR129, // DFE Tap1 DQL[0]
  mrMR130, // DFE Tap2 DQL[0]
  mrMR131, // DFE Tap3 DQL[0]
  mrMR132, // DFE Tap4 DQL[0]
  mrMR137,
  mrMR138,
  mrMR139,
  mrMR140,
  mrMR145,
  mrMR146,
  mrMR147,
  mrMR148,
  mrMR153,
  mrMR154,
  mrMR155,
  mrMR156,
  mrMR161,
  mrMR162,
  mrMR163,
  mrMR164,
  mrMR169,
  mrMR170,
  mrMR171,
  mrMR172,
  mrMR177,
  mrMR178,
  mrMR179,
  mrMR180,
  mrMR185,
  mrMR186,
  mrMR187,
  mrMR188,
  mrMR193,
  mrMR194,
  mrMR195,
  mrMR196,
  mrMR201,
  mrMR202,
  mrMR203,
  mrMR204,
  mrMR209,
  mrMR210,
  mrMR211,
  mrMR212,
  mrMR217,
  mrMR218,
  mrMR219,
  mrMR220,
  mrMR225,
  mrMR226,
  mrMR227,
  mrMR228,
  mrMR233,
  mrMR234,
  mrMR235,
  mrMR236,
  mrMR241,
  mrMR242,
  mrMR243,
  mrMR244,
  mrMR249,
  mrMR250,
  mrMR251,
  mrMR252,
  mrMR10,
  mrMR3,
  mrMR7,
  mpcSelectAllPDA,
  mrEndOfSequence
};

  // DDR5 PPR Beginning Sequence
  static const MR_DATA_STRUCT Ddr5PprGuardKeySequence[] = {
    { mrMR24, 0xCF },
    { mrMR24, 0x73 },
    { mrMR24, 0xBB },
    { mrMR24, 0x3B },
    { mrEndOfSequence, 0 }
  };

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
  )
{
  MrcDebug    *Debug;
  MrcStatus   Status;
  DDR5_MR0_RL MrValue;

  Debug = &MrcData->Outputs.Debug;
  Status = mrcSuccess;

  if (Value < CASLATMINPOSSIBLE ||  Value > CASLATMAXPOSSIBLE || (Value % 2 != 0)) {
    MrValue = Ddr5RlMax;
  } else {
    MrValue = (Value - 22) / 2;
  }

  if (MrValue < Ddr5Rl_22 || MrValue >= Ddr5RlMax) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid %s Latency Value: %d\n", gErrString, gRdString, Value);
    Status = mrcWrongInputParameter;
  }


  if (EncVal != NULL) {
    *EncVal = MrValue;
  } else {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s\n", __FUNCTION__, gNullPtrErrStr);
    Status = mrcWrongInputParameter;
  }

  return Status;
}

/**
  This function converts from the integer defined Write Recovery to the Mode Register (MR6)
  encoding of the timing in DDR5.

  @param[in]  MrcData - Pointer to global MRC data.
  @param[in]  Value   - Requested Write Recovery value.
  @param[out] EncVal  - Encoded Mode Register value.

  @retval MrcStatus - mrcSuccess if the latency is supported.  Else mrcWrongInputParameter.
**/
MrcStatus
EncodeWriteRecoveryDdr5 (
  IN  MrcParameters *MrcData,
  IN  UINT16        Value,
  OUT UINT8         *EncVal
  )
{
  MrcDebug    *Debug;
  MrcStatus   Status;
  DDR5_MR6_WR MrValue;

  Debug = &MrcData->Outputs.Debug;
  Status = mrcSuccess;

  if (Value < DDR5_TWRMINPOSSIBLE || Value > DDR5_TWRMAXPOSSIBLE || (Value % 6 != 0)) {
    MrValue = Ddr5WrMax;
  } else {
    MrValue = (Value - 48) / 6;
  }

  if (MrValue < Ddr5Wr_48 || MrValue >= Ddr5WrMax) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Unsupported Write Recovery value\n", gErrString);
    Status = mrcWrongInputParameter;
  }

  if (EncVal != NULL) {
    *EncVal = MrValue;
  } else {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s\n", __FUNCTION__, gNullPtrErrStr);
    Status = mrcWrongInputParameter;
  }

  return Status;
}


/**
  This function converts from the integer defined tRTP timing to the Mode Register (MR6)
  encoding of the timing in DDR5.

  @param[in]  MrcData - Pointer to global MRC data.
  @param[in]  Value   - Requested tRTP value.
  @param[out] EncVal  - Encoded Mode Register value.

  @retval MrcStatus - mrcSuccess if the timing is supported.  Else mrcWrongInputParameter.
**/
MrcStatus
EncodeTrtpDdr5 (
  IN  MrcParameters *MrcData,
  IN  UINT16        Value,
  OUT UINT8         *EncVal
  )
{
  MrcDebug      *Debug;
  MrcStatus     Status;
  DDR5_MR6_TRTP MrValue;

  Debug = &MrcData->Outputs.Debug;
  Status = mrcSuccess;


  switch (Value) {
    case 12:
      MrValue = Ddr5Trtp_12;
      break;

    case 14:
      MrValue = Ddr5Trtp_14;
      break;

    case 15:
      MrValue = Ddr5Trtp_15;
      break;

    case 17:
      MrValue = Ddr5Trtp_17;
      break;

    case 18:
      MrValue = Ddr5Trtp_18;
      break;

    case 20:
      MrValue = Ddr5Trtp_20;
      break;

    case 21:
      MrValue = Ddr5Trtp_21;
      break;

    case 23:
      MrValue = Ddr5Trtp_23;
      break;

    case 24:
      MrValue = Ddr5Trtp_24;
      break;

    case 26:
      MrValue = Ddr5Trtp_26;
      break;
    case 27:
      MrValue = Ddr5Trtp_27;
      break;
    case 28:
      MrValue = Ddr5Trtp_28;
      break;
    case 30:
      MrValue = Ddr5Trtp_30;
      break;
    case 32:
      MrValue = Ddr5Trtp_32;
      break;
    case 33:
      MrValue = Ddr5Trtp_33;
      break;
    default:
      MrValue = Ddr5TrtpMax;
      Status = mrcWrongInputParameter;
      break;
  }

  if (MrValue < Ddr5TrtpMax) {
    if (EncVal != NULL) {
      *EncVal = MrValue;
    } else {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s\n", __FUNCTION__, gNullPtrErrStr);
      Status = mrcWrongInputParameter;
    }
  } else {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid tRTP Value: %d\n", gErrString, Value);
    Status = mrcWrongInputParameter;
  }

  return Status;
}

/**
  This function returns the tCCD_L/tCCD_L_WR/tDLLK MR13 encoded value
  for the input frequency value.

  @param[in]  MrcData   - Pointer to global MRC data.
  @param[in]  Frequency - Data Rate
  @param[out] EncVal    - Encoded Mode Register value.

  @retval MrcStatus - mrcSuccess if the timing is supported.  Else mrcWrongInputParameter.
**/
MrcStatus
EncodeDdr5TccdlTdllk (
  IN  MrcParameters *MrcData,
  IN  MrcFrequency  Frequency,
  OUT UINT8         *EncVal
  )
{
  MrcStatus Status;
  MrcDebug  *Debug;
  UINT8     MrValue;

  Debug = &MrcData->Outputs.Debug;
  Status = mrcSuccess;

  if (((f2000 <= Frequency) && (Frequency <= f2100)) || (Frequency == f3200)) {
    MrValue = 0;
  } else if ((f3200 < Frequency) && (Frequency <= f3600)) {
    MrValue = 1;
  } else if ((f3600 < Frequency) && (Frequency <= f4000)) {
    MrValue = 2;
  } else if ((f4000 < Frequency) && (Frequency <= f4400)) {
    MrValue = 3;
  } else if ((f4400 < Frequency) && (Frequency <= f4800)) {
    MrValue = 4;
  } else if ((f4800 < Frequency) && (Frequency <= f5200)) {
    MrValue = 5;
  } else if ((f5200 < Frequency) && (Frequency <= f5600)) {
    MrValue = 6;
  } else if ((f5600 < Frequency) && (Frequency <= f6000)) {
    MrValue = 7;
  } else if ((f6000 < Frequency) && (Frequency <= f6400)) {
    MrValue = 8;
  } else if ((f6400 < Frequency) && (Frequency <= f6800)) {
    MrValue = 9;
  } else if ((f6800 < Frequency) && (Frequency <= f7200)) {
    MrValue = 10;
  } else if ((f7200 < Frequency) && (Frequency <= f7600)) {
    MrValue = 11;
  } else if ((f7600 < Frequency) && (Frequency <= f8000)) {
    MrValue = 12;
  } else if ((f8000 < Frequency) && (Frequency <= f8400)) {
    MrValue = 13;
  } else if ((f8400 < Frequency) && (Frequency <= f8800)) {
    MrValue = 14;
  } else {
    MrValue = 0xFF;
    if (MrcData->Outputs.IsOCProfile) {
      MrValue = 8;
    }
  }

  if (MrValue != 0xFF) {
    if (EncVal != NULL) {
      *EncVal = MrValue;
    } else {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s\n", __FUNCTION__, gNullPtrErrStr);
      Status = mrcWrongInputParameter;
    }
  } else {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid Frequency Value: %d\n", gErrString, Frequency);
    Status = mrcWrongInputParameter;
  }

  return Status;
}

/**
  This function returns the Read Preamble Setting MR8 encoded value.

  @param[in]  MrcData   - Pointer to global MRC data.
  @param[out] EncVal    - Encoded Mode Register value.

  @retval MrcStatus - mrcSuccess if the timing is supported.  Else mrcWrongInputParameter.
**/
MrcStatus
MrcDdr5GetReadPreambleSetting (
  IN  MrcParameters  *MrcData,
  OUT DDR5_MR8_tRPRE *EncVal
  )
{
  MrcStatus      Status;
  MrcDebug       *Debug;
  UINT32         tRPRE;
  DDR5_MR8_tRPRE ReadPreambleSettings;

  Debug = &MrcData->Outputs.Debug;
  Status = mrcSuccess;

  tRPRE = MrcData->Outputs.ReadPreamble;
  if (tRPRE == 2) {
    ReadPreambleSettings = Ddr5tRPRE_2tCK_0010;
  } else if (tRPRE == 3) {
    ReadPreambleSettings = Ddr5tRPRE_3tCK_000010;
  } else if (tRPRE == 4) {
    ReadPreambleSettings = Ddr5tRPRE_4tCK_00001010;
  } else {
    ReadPreambleSettings = 0;
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid tRPRE %d\n", gErrString, tRPRE);
    Status = mrcWrongInputParameter;
  }

  if (Status == mrcSuccess && EncVal != NULL) {
    *EncVal = ReadPreambleSettings;
  } else {
    Status = mrcWrongInputParameter;
  }

  return Status;
}

/**
  This function returns the Write Preamble Setting MR8 encoded value.

  @param[in]  MrcData   - Pointer to global MRC data.
  @param[out] EncVal    - Encoded Mode Register value.

  @retval MrcStatus - mrcSuccess if the timing is supported.  Else mrcWrongInputParameter.
**/
MrcStatus
MrcDdr5GetWritePreambleSetting (
  IN  MrcParameters  *MrcData,
  OUT DDR5_MR8_tWPRE *EncVal
  )
{
  MrcStatus      Status;
  MrcDebug       *Debug;
  UINT32         tWPRE;
  DDR5_MR8_tWPRE WritePreambleSettings;

  Debug = &MrcData->Outputs.Debug;
  Status = mrcSuccess;

  tWPRE = MrcGetTwpre (MrcData);
  if (tWPRE == 2) {
    WritePreambleSettings = Ddr5tWPRE_2tCK_0010;
  } else if (tWPRE == 3) {
    WritePreambleSettings = Ddr5tWPRE_3tCK_000010;
  } else if (tWPRE == 4) {
    WritePreambleSettings = Ddr5tWPRE_4tCK_00001010;
  } else {
    WritePreambleSettings = 0;
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid tWPRE %d\n", gErrString, tWPRE);
    Status = mrcWrongInputParameter;
  }

  if (Status == mrcSuccess && EncVal != NULL) {
    *EncVal = WritePreambleSettings;
  }

  return Status;
}

MrcStatus
MrcDdr5GetWritePostambleSetting (
  IN  MrcParameters   *MrcData,
  OUT DDR5_MR8_tWPOST *EncVal
  )
{
  DDR5_MR8_tWPOST  WritePostambleSettings;
  if (EncVal == NULL) {
    return mrcWrongInputParameter;
  }
  if (MrcData->Outputs.Frequency <= f4800) {
    WritePostambleSettings = Ddr5tWPOST_0pt5tCK_0;
  } else {
    WritePostambleSettings = Ddr5tWPOST_1pt5tCK_000;
  }
  *EncVal = WritePostambleSettings;
  return mrcSuccess;
}
// List of MR array indexes to initialize during DDR5 Jedec Init
MrcModeRegister Ddr5MrInitList [] = {
  mrMR0,
  mrMR2,
  mrMR3,
  mrMR4,
  mrMR5,
  mrMR6,
  mrMR7,
  mrMR8,
  mrMR10,
  mrMR11,
  mrMR12,
  mrMR13,
  mpcMR13,
  mrMR37,
  mrMR38,
  mrMR39,
  mrMR40,
  mrMR45,
  mrMR48,
  mrMR111,
  mrMR129,
  mrMR130,
  mrMR131,
  mrMR132,
  mrMR193,
  mrMR194,
  mrMR195,
  mrMR196,
  mpcMR32a0,
  mpcMR32a1,
  mpcMR32b0,
  mpcMR32b1,
  mpcMR33a0,
  mpcMR33b0,
  mpcMR33,
  mpcMR34,
  mpcApplyVrefCa,
  mrMR34,
  mrMR35,
  mrMR36,
  mpcDllReset,
  mpcZqCal,
  mpcZqLat,
  mpcEnterCaTrainMode,
  mpcSetCmdTiming,
  mpcSelectAllPDA,
  mrEndOfSequence
};

/**
  Determine the ODT table for the given Memory Controller / Channel / DIMM.

  if Inputs has user-input CCC non-0 values, use user inputs instead of
  from the default table

  @param[in]     MrcData         - Include all the mrc global data.
  @param[in]     Controller      - Controller to work on.
  @param[in]     Channel         - Channel to work on.
  @param[in]     Dimm            - DIMM to work on.
  @param[in out] CCCTable        - CCC structure

  @retval mrcSuccess Data referenced by CCCTable updated
  @retval mrcFail Data referenced by CCCtable not updated
**/
MrcStatus
GetDdr5CCCTableIndex (
  IN MrcParameters *const     MrcData,
  IN const UINT32             Controller,
  IN const UINT32             Channel,
  IN const UINT32             Dimm,
  IN OUT   TOdtValueCccDdr5   *CCCTable
  )
{
  MrcInput            *Inputs;
  MrcOutput           *Outputs;
  MRC_EXT_INPUTS_TYPE *ExtInputs;
  MrcDebug            *Debug;
  const MRC_FUNCTION  *MrcCall;
  TOdtValueCccDdr5    *Ddr5CCCOdtTableIndex;
  UINT16              Index;
  UINT16              *CCC;

  Inputs       = &MrcData->Inputs;
  Outputs      = &MrcData->Outputs;
  ExtInputs    = Inputs->ExtInputs.Ptr;
  Debug        = &Outputs->Debug;
  MrcCall      = Inputs->Call.Func;

  if ((!Outputs->IsDdr5) || (CCCTable == NULL)) {
    return mrcFail;
  }

  Ddr5CCCOdtTableIndex = SelectCccTableDdr5 (MrcData, ExtInputs->MemoryProfile, Controller, Channel, Dimm);
  if (Ddr5CCCOdtTableIndex == NULL) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s: Ddr5CCCOdtTableIndex is NULL!\n", gErrString);
    return mrcFail;
  }
  // Copy both GroupA and GroupB
  MrcCall->MrcCopyMem ((UINT8 *)&CCCTable[0], (UINT8 *)&Ddr5CCCOdtTableIndex[0], sizeof(TOdtValueCccDdr5) * 2);

  // Use customized values if Inputs have non-0 values
  CCC = (UINT16 *) &CCCTable[0];
  for (Index = OFFSET_RTTCA_GROUPA; Index <= OFFSET_RTTCK_GROUPB; Index++) {
    //DimmOdt[Dimm][Index] equals 0 meaning AUTO
    if (ExtInputs->DimmOdt[Dimm][Index] > MAX_DIMMODT_VALUE) {
      //Need disable
      CCC[Index - OFFSET_RTTCA_GROUPA] = 0;
    } else if (ExtInputs->DimmOdt[Dimm][Index] != 0) {
      CCC[Index - OFFSET_RTTCA_GROUPA] = ExtInputs->DimmOdt[Dimm][Index];
    }
  }

  return mrcSuccess;
}

/**
  Determine the Dimm DFE table for the given Memory Controller / Channel / DIMM.

  If Inputs has user-input Dimm DFE non-0 values, use user inputs instead of
  from the default table

  @param[in]     MrcData              - Include all the mrc global data.
  @param[in]     Controller           - Controller to work on.
  @param[in]     Channel              - Channel to work on.
  @param[in]     Dimm                 - DIMM to work on.
  @param[in out] TDFEValueDdr5Table   - Point to TDFEValueDdr5 structure

  @retval Pointer to the relevant CCC structure
**/
MrcStatus
GetDdr5DFETableIndex (
  IN MrcParameters *const     MrcData,
  IN const UINT32             Controller,
  IN const UINT32             Channel,
  IN const UINT32             Dimm,
  IN OUT   TDFEValueDdr5      *TDFEValueDdr5Table
  )
{
  MrcInput            *Inputs;
  MrcOutput           *Outputs;
  MrcDebug            *Debug;
  const MRC_FUNCTION  *MrcCall;
  TDFEValueDdr5       *TDFEValueDdr5Index;

  Inputs       = &MrcData->Inputs;
  Outputs      = &MrcData->Outputs;
  Debug        = &Outputs->Debug;
  MrcCall      = Inputs->Call.Func;

  if ((!Outputs->IsDdr5) || (TDFEValueDdr5Table == NULL)) {
    return mrcFail;
  }

  TDFEValueDdr5Index = SelectDfeTableDdr5 (MrcData, Inputs->ExtInputs.Ptr->MemoryProfile, Controller, Channel, Dimm);
  if (TDFEValueDdr5Index == NULL) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s: TDFEValueDdr5Indexis NULL!\n", gErrString);
    return mrcFail;
  }
  MrcCall->MrcCopyMem ((UINT8 *)&TDFEValueDdr5Table->Tap1, (UINT8 *)&TDFEValueDdr5Index->Tap1, sizeof(TDFEValueDdr5));

  return mrcSuccess;
}
/**
  This function returns an MR value or MPC command for the input MrRegNum.
  Some MR values (such as timing or vref MRs) will be different depending
  on the system configuration specified in the MrcData variable.

  @param[in]  MrcData      - Pointer to global MRC data.
  @param[in]  MrRegNum     - Requested MrRegiser
  @param[out] MrRegValOut  - The value for the requested MR Register
  @param[in]  Controller   - Index of the requested Controler
  @param[in]  Channel      - Index of the requested Channel
  @param[in]  Rank         - Index of the requested Rank

  @retval MrcStatus - mrcSuccess if the MrRegNum is supported. Else mrcWrongInputParameter.
**/
MrcStatus
Ddr5JedecInitVal (
  IN  MrcParameters *const MrcData,
  IN  MrcModeRegister      MrRegNum,
  OUT UINT8                *MrRegValOut,
  IN  UINT32               Controller,
  IN  UINT32               Channel,
  IN  UINT32               Rank
  )
{
  MrcStatus      Status;
  MrcInput       *Inputs;
  MrcOutput      *Outputs;
  MrcDebug       *Debug;
  MrcChannelOut  *ChannelOut;
  MrcTiming      *TimingPtr;
  UINT8          RetVal;
  UINT8          RlEnc;
  UINT8          WrEnc;
  UINT8          TrtpEnc;
  UINT8          TdllkEnc;
  UINT8          DfeGain;
  UINT8          DfeTap1;
  UINT8          DfeTap2;
  UINT8          DfeTap3;
  UINT8          DfeTap4;
  UINT32         Profile;
  UINT32         Dimm;
  DDR5_MR32_CKCSCA_ODT CccRttValue;
  DDR5_MR33_RTT_PARK   DataRttValue;
  DDR5_MR33_RTT_PARK   RttNomWr;
  DDR5_MR33_RTT_PARK   RttNomRd;
  TOdtValueDqDdr5  DqOdtTableIndex;
  TOdtValueCccDdr5 CccOdtTableIndex[2];
  TDFEValueDdr5    DFETableIndex;
  DDR5_MR8_tRPRE RpreVal;
  DDR5_MR8_tWPRE WpreVal;
  DDR5_MR8_tWPOST  WpostVal;
  DDR5_MODE_REGISTER_0_TYPE *Mr0;
  DDR5_MODE_REGISTER_2_TYPE *Mr2;
  DDR5_MODE_REGISTER_4_TYPE *Mr4;
  DDR5_MODE_REGISTER_5_TYPE *Mr5;
  DDR5_MODE_REGISTER_6_TYPE *Mr6;
  DDR5_MODE_REGISTER_8_TYPE *Mr8;
  DDR5_MODE_REGISTER_10_TYPE *Mr10;
  DDR5_MODE_REGISTER_11_TYPE *Mr11;
  DDR5_MODE_REGISTER_12_TYPE *Mr12;
  DDR5_MODE_REGISTER_13_TYPE *Mr13;
  DDR5_MODE_REGISTER_34_TYPE *Mr34;
  DDR5_MODE_REGISTER_35_TYPE *Mr35;
  DDR5_MODE_REGISTER_37_TYPE *Mr37;
  DDR5_MODE_REGISTER_38_TYPE *Mr38;
  DDR5_MODE_REGISTER_39_TYPE *Mr39;
  DDR5_MODE_REGISTER_40_TYPE* Mr40;
  DDR5_MODE_REGISTER_45_TYPE *Mr45;
  DDR5_MODE_REGISTER_111_TYPE *Mr111;
  DDR5_MODE_REGISTER_129_TYPE *Mr129;
  DDR5_MODE_REGISTER_130_TYPE *Mr130;
  DDR5_MODE_REGISTER_131_TYPE *Mr131;
  DDR5_MODE_REGISTER_132_TYPE *Mr132;

  Inputs     = &MrcData->Inputs;
  Outputs    = &MrcData->Outputs;
  Debug      = &Outputs->Debug;
  Profile    = Inputs->ExtInputs.Ptr->MemoryProfile;
  ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
  TimingPtr  = &ChannelOut->Timing[Profile];
  RetVal     = 0;
  CccRttValue = 0;
  DataRttValue = 0;

  Dimm = RANK_TO_DIMM_NUMBER (Rank);
  Status = GetOdtTableIndex (MrcData, Controller, Channel, Dimm, &DqOdtTableIndex);
  if (Status == mrcSuccess) {
    Status = GetDdr5CCCTableIndex(MrcData, Controller, Channel, Dimm, CccOdtTableIndex);
  }
  if (Status != mrcSuccess) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s: invalid DQ/CCC ODT index", gErrString);
    return mrcFail;
  }
  Status = GetDdr5DFETableIndex(MrcData, Controller, Channel, Dimm, &DFETableIndex);
  if (Status != mrcSuccess) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s: invalid DFE index", gErrString);
    return mrcFail;
  }

  // We will only return one value for the input MR Num.
  Mr0  = (DDR5_MODE_REGISTER_0_TYPE *) &RetVal;
  Mr2  = (DDR5_MODE_REGISTER_2_TYPE *) &RetVal;
  Mr4  = (DDR5_MODE_REGISTER_4_TYPE *) &RetVal;
  Mr5  = (DDR5_MODE_REGISTER_5_TYPE *) &RetVal;
  Mr6  = (DDR5_MODE_REGISTER_6_TYPE *) &RetVal;
  Mr8  = (DDR5_MODE_REGISTER_8_TYPE *) &RetVal;
  Mr10 = (DDR5_MODE_REGISTER_10_TYPE *) &RetVal;
  Mr11 = (DDR5_MODE_REGISTER_11_TYPE *) &RetVal;
  Mr12 = (DDR5_MODE_REGISTER_12_TYPE *) &RetVal;
  Mr13 = (DDR5_MODE_REGISTER_13_TYPE *) &RetVal;
  Mr34 = (DDR5_MODE_REGISTER_34_TYPE *) &RetVal;
  Mr35 = (DDR5_MODE_REGISTER_35_TYPE *) &RetVal;
  Mr37 = (DDR5_MODE_REGISTER_37_TYPE *) &RetVal;
  Mr38 = (DDR5_MODE_REGISTER_38_TYPE *) &RetVal;
  Mr39 = (DDR5_MODE_REGISTER_39_TYPE *) &RetVal;
  Mr40 = (DDR5_MODE_REGISTER_40_TYPE *) &RetVal;
  Mr45 = (DDR5_MODE_REGISTER_45_TYPE *) &RetVal;
  Mr111 = (DDR5_MODE_REGISTER_111_TYPE *) &RetVal;
  Mr129 = (DDR5_MODE_REGISTER_129_TYPE *) &RetVal;
  Mr130 = (DDR5_MODE_REGISTER_130_TYPE *) &RetVal;
  Mr131 = (DDR5_MODE_REGISTER_131_TYPE *) &RetVal;
  Mr132 = (DDR5_MODE_REGISTER_132_TYPE *) &RetVal;

  switch (MrRegNum) {
    case mrMR0:
      // Mr0->Bits.BurstLength = 1 (BC8 OTF)
      Mr0->Bits.BurstLength = 1;
      if (EncodeReadLatencyDdr5 (MrcData, TimingPtr->tCL, &RlEnc) != mrcSuccess) {
        Status = mrcWrongInputParameter;
      }
      Mr0->Bits.CasLatency = RlEnc;
      break;

    case mrMR2:
      // Mr2->Bits.ReadPreambleTraining = 0 (Normal Mode)
      // Mr2->Bits.WriteLevelingTraining = 0 (Normal Mode)
      // Mr2->Bits.Mode1n = 0 (2N Mode) (Read Only)
      // Mr2->Bits.MaxPowerSavingsMode = 0 (Disable);
      Mr2->Bits.CsAssertionDuration = 0; // (Multiple cycles of CS assertion supported)
      // Mr2->Bits.Device15Mpsm = 0 (Disable)
      // Mr2->Bits.InternalWriteTiming = 0 (Disable)
      break;

    case mrMR3:
      // Mr3->Bits.WriteLevelingInternalCycleLowerByte = 0
      // Mr3->Bits.WriteLevelingInternalCycleUpperByte = 0
      break;

    case mrMR4:
      Mr4->Bits.RefreshRate = 0x2; // Read Only, setting to default value
      Mr4->Bits.RefreshTrfcMode = 1; // MC only supports Fine Granularity Refresh (FGR)
      // Mr4->Bits.Tuf = 0 [Read Only]
      break;

    case mrMR5:
      // Mr5->Bits.DataOutputDisable = 0; (Default)
      // Mr5->Bits.PullUpOutputDriverImpedance = 0; (RZQ/7)
      // Mr5->Bits.TdqsEnable = 0; (Default)
      // Mr5->Bits.DmEnable = 0; (Default)
      Mr5->Bits.PullDownOutputDriverImpedance = 0; // (RZQ/7)
      break;

    case mrMR6:
      if (EncodeWriteRecoveryDdr5 (MrcData, TimingPtr->tWR, &WrEnc) != mrcSuccess) {
        Status = mrcWrongInputParameter;
      }
      if (EncodeTrtpDdr5 (MrcData, TimingPtr->tRTP, &TrtpEnc) != mrcSuccess) {
        Status = mrcWrongInputParameter;
      }
      Mr6->Bits.WriteRecoveryTime = WrEnc;
      Mr6->Bits.tRTP = TrtpEnc;
      break;

    case mrMR7:
      // Mr7->Bits.WriteLevelingInternalCycleHalfStepLowerByte = 0;
      // Mr7->Bits.WriteLevelingInternalCycleHalfStepUpperByte = 0;
      break;

    case mrMR8:
      if (MrcDdr5GetReadPreambleSetting (MrcData, &RpreVal) != mrcSuccess) {
        Status = mrcWrongInputParameter;
      }
      Mr8->Bits.ReadPreambleSettings = RpreVal;

      if (MrcDdr5GetWritePreambleSetting (MrcData, &WpreVal) != mrcSuccess) {
        Status = mrcWrongInputParameter;
      }
      Mr8->Bits.WritePreambleSettings = WpreVal;

      // Mr8->Bits.ReadPostambleSettings = 0;  // 0.5 tCK - 0 Pattern
      if (MrcDdr5GetWritePostambleSetting (MrcData, &WpostVal) != mrcSuccess) {
        Status = mrcWrongInputParameter;
      }
      Mr8->Bits.WritePostambleSettings = WpostVal;
      break;

    case mrMR10:
      Mr10->Bits.VrefDqCalibrationValue = Ddr5Vref_75p0; // 75% of VDDQ
      if (Inputs->IsDdrIoDtHalo && (Outputs->Is2DPC1R1R || Outputs->Is2DPC2R2R)) { // 2DPC 1R1R/2R2R Config
        Mr10->Bits.VrefDqCalibrationValue = Ddr5Vref_80p0; /// 80% of VDDQ
      }
      if (Outputs->IsOCProfile) {
        Mr10->Bits.VrefDqCalibrationValue = Ddr5Vref_68p0;
      }
      ChannelOut->IsMr10PdaEnabled = FALSE;
      break;

    case mrMR11:
      // Read Only: Configured via VrefCA Command
      Mr11->Bits.VrefCaCalibrationValue = DDR5_VREFCA (Ddr5Vref_75p0); // 75% of VDDQ (Default)
      ChannelOut->IsMr11PdaEnabled = FALSE;
      break;

    case mrMR12:
      // Read Only: Configured via VrefCS Command
      Mr12->Bits.VrefCsCalibrationValue = DDR5_VREFCS (Ddr5Vref_75p0); // 75% of VDDQ (Default)
      break;

    case mrMR13:
      if (EncodeDdr5TccdlTdllk (MrcData, Outputs->HighFrequency, &TdllkEnc) != mrcSuccess) {
        Status = mrcWrongInputParameter;
      }
      Mr13->Bits.tCCD_L_tDLLK = TdllkEnc;
      break;

    case mpcMR13:
      if (EncodeDdr5TccdlTdllk (MrcData, Outputs->HighFrequency, &TdllkEnc) != mrcSuccess) {
        Status = mrcWrongInputParameter;
      }
      RetVal = DDR5_MPC_CFG_TDLLK_TCCD_L (TdllkEnc);
      break;

    case mrMR37:
      Mr37->Bits.OdtlOnWrOffset = OdtlOnWrOffsetMinus4;
      Mr37->Bits.OdtlOffWrOffset = OdtlOffWrOffsetPlus4;
      break;

    case mrMR38:
      Mr38->Bits.OdtlOnWrNtOffset = OdtlOnWrOffsetMinus4;
      Mr38->Bits.OdtlOffWrNtOffset = OdtlOffWrOffsetPlus4;
      break;

    case mrMR39:
      Mr39->Bits.OdtlOnRdNtOffset = OdtlOnRdNtOffsetMinus3;
      Mr39->Bits.OdtlOffRdNtOffset = OdtlOffRdNtOffsetPlus3;
      break;

    case mrMR40:
      Mr40->Bits.ReadDqsOffsetTiming = Outputs->EarlyDqs;
      if (TimingPtr->tCL <= 30 ) {
        if ((Outputs->ReadPreamble + Outputs->EarlyDqs) >= 5) {
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s When CL <= 30, tRPRE + Read DQS Offset >= 5 Clocks cannot be supported, ReadPreamble %d  EarlyDqs %d\n", gErrString, Outputs->ReadPreamble, Outputs->EarlyDqs);
          Status = mrcWrongInputParameter;
        }
      }
      break;

    case mrMR45:
      Mr45->Bits.DqsIntervalTimerRunTime = 4; // Stop after 64 clocks
      break;

    case mrMR48:
      //Mr48->Bits.WritePatternMode = 0 (default)
      break;

    case mrMR111:
      MrcDdr5GetDFE (&DfeGain, &DfeTap1, &DfeTap2, &DfeTap3, &DfeTap4);
      Mr111->Bits.GlobalDfeGainEnable = DfeGain;
      Mr111->Bits.GlobalDfeTap1Enable = DfeTap1;
      Mr111->Bits.GlobalDfeTap2Enable = DfeTap2;
      Mr111->Bits.GlobalDfeTap3Enable = DfeTap3;
      Mr111->Bits.GlobalDfeTap4Enable = DfeTap4;
      break;
    case mrMR129:
    case mrMR137:
    case mrMR145:
    case mrMR153:
    case mrMR161:
    case mrMR169:
    case mrMR177:
    case mrMR185:
    case mrMR193:
    case mrMR201:
    case mrMR209:
    case mrMR217:
    case mrMR225:
    case mrMR233:
    case mrMR241:
    case mrMR249:
      Mr129->Bits.DfeTap1Bias = ABS(DFETableIndex.Tap1); // Step size of 5mV
      Mr129->Bits.DfeTap1BiasSignBit = DFETableIndex.Tap1 < 0 ? 1 : 0; // Negative value
      Mr129->Bits.DfeTap1Enable = 1;
      break;
    case mrMR130:
    case mrMR138:
    case mrMR146:
    case mrMR154:
    case mrMR162:
    case mrMR170:
    case mrMR178:
    case mrMR186:
    case mrMR194:
    case mrMR202:
    case mrMR210:
    case mrMR218:
    case mrMR226:
    case mrMR234:
    case mrMR242:
    case mrMR250:
      Mr130->Bits.DfeTap2Bias = ABS(DFETableIndex.Tap2); // Step size of 5mV
      Mr130->Bits.DfeTap2BiasSignBit = DFETableIndex.Tap2 < 0 ? 1 : 0; // Negative value
      Mr130->Bits.DfeTap2Enable = 1;
      break;
    case mrMR131:
    case mrMR139:
    case mrMR147:
    case mrMR155:
    case mrMR163:
    case mrMR171:
    case mrMR179:
    case mrMR187:
    case mrMR195:
    case mrMR203:
    case mrMR211:
    case mrMR219:
    case mrMR227:
    case mrMR235:
    case mrMR243:
    case mrMR251:
      Mr131->Bits.DfeTap3Bias = ABS(DFETableIndex.Tap3); // Step size of 5mV
      Mr131->Bits.DfeTap3BiasSignBit = DFETableIndex.Tap3 < 0 ? 1 : 0;
      Mr131->Bits.DfeTap3Enable = 1;
      break;

    // DFE Tap 4
    case mrMR132:
    case mrMR140:
    case mrMR148:
    case mrMR156:
    case mrMR164:
    case mrMR172:
    case mrMR180:
    case mrMR188:
    case mrMR196:
    case mrMR204:
    case mrMR212:
    case mrMR220:
    case mrMR228:
    case mrMR236:
    case mrMR244:
    case mrMR252:
      Mr132->Bits.DfeTap4Bias = ABS(DFETableIndex.Tap4); // Step size of 5mV
      Mr132->Bits.DfeTap4BiasSignBit = DFETableIndex.Tap4 < 0 ? 1 : 0;
      Mr132->Bits.DfeTap4Enable = 1;
      break;

    case mpcMR32a0:
      // MR32 Group A RTT_CK
      CccRttValue = CccOdtEncode_DDR5 (CccOdtTableIndex[0].RttCk);
      RetVal =  DDR5_MPC_GROUP_A_RTT_CK (CccRttValue);
      break;

    case mpcMR32b0:
      // MR32 Group B RTT_CK
      CccRttValue = CccOdtEncode_DDR5 (CccOdtTableIndex[1].RttCk);
      RetVal = DDR5_MPC_GROUP_B_RTT_CK (CccRttValue);
      break;

    case mpcMR32a1:
      // MR32 Group A RTT_CS
      CccRttValue = CccOdtEncode_DDR5 (CccOdtTableIndex[0].RttCs);
      RetVal = DDR5_MPC_GROUP_A_RTT_CS (CccRttValue);
      break;

    case mpcMR32b1:
      // MR32 Group B RTT_CS
      CccRttValue = CccOdtEncode_DDR5 (CccOdtTableIndex[1].RttCs);
      RetVal = DDR5_MPC_GROUP_B_RTT_CS (CccRttValue);
      break;

    case mpcMR33a0:
      // MR33 Group A RTT_CA
      CccRttValue = CccOdtEncode_DDR5 (CccOdtTableIndex[0].RttCa);
      RetVal = DDR5_MPC_GROUP_A_RTT_CA (CccRttValue);
      break;

    case mpcMR33b0:
      // MR33 Group B RTT_CA
      CccRttValue = CccOdtEncode_DDR5 (CccOdtTableIndex[1].RttCa);
      RetVal = DDR5_MPC_GROUP_B_RTT_CA (CccRttValue);
      break;

    case mpcMR33:
      // MR33 DQS_RTT_PARK - set via MPC command
      DataRttValue = Ddr5OdtEncode (DqOdtTableIndex.RttParkDqs);
      RetVal = DDR5_MPC_SET_DQS_RTT_PARK (DataRttValue);
      break;

    case mpcMR34:
      // MR34 RTT_PARK
      DataRttValue = Ddr5OdtEncode (DqOdtTableIndex.RttPark);
      RetVal = DDR5_MPC_SET_RTT_PARK (DataRttValue);
      break;

    case mpcApplyVrefCa:
      // Apply Vrefca and RTT_CA/CS/CK
      RetVal = DDR5_MPC_APPLY_VREF_RTT;
      break;

    case mrMR34:
      // RTT_PARK must be configured via MPC command: DDR5_MPC_SET_RTT_PARK, see mpcMR34
      // We still want to populate the RttPark value in MR34 in the host struct as it's used in the code.
      DataRttValue = Ddr5OdtEncode (DqOdtTableIndex.RttWr);
      if (DataRttValue == -1) {
        break;
      }
      Mr34->Bits.RttWr   = (UINT8) DataRttValue;
      DataRttValue = Ddr5OdtEncode (DqOdtTableIndex.RttPark);
      Mr34->Bits.RttPark = (UINT8) DataRttValue;
      break;

    case mrMR35:
      // MR35 RTT_NOM_WR / RTT_NOM_RD
      RttNomWr = Ddr5OdtEncode (DqOdtTableIndex.RttNomWr);
      RttNomRd = Ddr5OdtEncode (DqOdtTableIndex.RttNomRd);
      if (RttNomWr == -1) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Bad RTT_NOM_%s Value\n", gErrString, "WR");
        Status = mrcWrongInputParameter;
        break;
      }
      if (RttNomRd == -1) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Bad RTT_NOM_%s Value\n", gErrString, "RD");
        Status = mrcWrongInputParameter;
        break;
      }
      Mr35->Bits.RttNomWr = (UINT8) RttNomWr;
      Mr35->Bits.RttNomRd = (UINT8) RttNomRd;
      break;

    case mrMR36:
      // Mr36->Bits.RttLoopback = 0 (RTT_OFF Default)
      break;

    case mpcDllReset:
      RetVal = DDR5_MPC_DLL_RESET;
      break;

    case mpcZqCal:
      RetVal = DDR5_MPC_ZQCAL_START;
      break;

    case mpcZqLat:
      RetVal = DDR5_MPC_ZQCAL_LATCH;
      break;

    case mpcEnterCaTrainMode:
      RetVal = DDR5_MPC_ENTER_CA_TRAINING_MODE;
      break;

    case mpcSetCmdTiming:
      RetVal = (TimingPtr->NMode == 2) ? DDR5_MPC_SET_2N_COMMAND_TIMING : DDR5_MPC_SET_1N_COMMAND_TIMING;
      break;

    case mpcSelectAllPDA:
      RetVal =  DDR5_MPC_PDA_SELECT_ID(0xf);
      break;

    default:
      Status = mrcWrongInputParameter;
      break;
  }

  if (DataRttValue == -1) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid ODT Value\n", gErrString);
    Status = mrcWrongInputParameter;
  }

  if (CccRttValue == -1) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid ODT Value\n", gErrString);
    Status = mrcWrongInputParameter;
  }

  if (MrRegValOut != NULL) {
    *MrRegValOut = RetVal;
  } else {
    Status = mrcWrongInputParameter;
  }

  if (Status != mrcSuccess) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s failed for MrRegNum = %d\n", __FUNCTION__, MrRegNum);
  }

  return Status;
}

/**
  This function converts from Ohms to DDR5 MR5 PU/PD Drive Strength encoding.

  @param[in]  DrvStrValue  - Ron Value in Ohms.

  @retval INT8 - Encoding if valid Ron value.  Else, -1.
**/
INT8
Ddr5DrvStrEncode (
  IN  UINT16  DrvStrValue
  )
{
  INT8      EncodeVal;

  switch (DrvStrValue) {
    case 48:
      EncodeVal = DDR5_ODIC_48;
      break;

    case 40:
      EncodeVal = DDR5_ODIC_40;
      break;

    case 34:
      EncodeVal = DDR5_ODIC_34;
      break;

    default:
      EncodeVal = -1;
      break;
  }

  return EncodeVal;
}

/**
  This function converts from DDR5 MR5 PU/PD Drive Strength encoding to Ohms.

  @param[in]  DecodeVal - Decoded Ron value.

  @retval INT16 - Ron Value in Ohms if valid. Else, -1.
**/
INT16
DdrDrvStrDecode (
  IN  UINT16 DecodeVal
  )
{
  INT16      RonValue;

  switch (DecodeVal) {
    case DDR5_ODIC_48:
      RonValue = 48;
      break;

    case DDR5_ODIC_40:
      RonValue = 40;
      break;

    case DDR5_ODIC_34:
      RonValue = 34;
      break;

    default:
      RonValue = -1;
      break;
  }

  return RonValue;
}

/**
  This function returns the Generic MRS FSM Command Type and Delay Type associated with the
  input MrcModeRegister value.

  @param[in]  MrcData      - Pointer to global MRC data.
  @param[in]  MrRegNum     - Requested MrRegiser
  @param[out] CmdTypeOut   - The Generic MRS FSM Command Type to use for the input MrRegister
  @param[out] DelayTypeOut - The type of delay to use for the input MrRegister

  @retval MrcStatus - mrcSuccess if the MrRegNum is supported. Else mrcWrongInputParameter.
**/
MrcStatus
MrcDdr5GetGmfAttributes (
  IN  MrcParameters *const MrcData,
  IN  MrcModeRegister      MrRegNum,
  OUT GmfCmdType           *CmdTypeOut,  OPTIONAL
  OUT GmfDdr5DelayType     *DelayTypeOut OPTIONAL
  )
{
  MrcStatus     Status;
  MrcOutput     *Outputs;
  MrcDebug      *Debug;
  GmfCmdType    CmdType;
  GmfDdr5DelayType DelayType;

  Outputs    = &MrcData->Outputs;
  Debug      = &Outputs->Debug;
  Status     = mrcSuccess;
  CmdType    = GmfCmdMrw;
  DelayType  = GmfDdr5Delay_tMRD;

  switch (MrRegNum) {
    case mrMR0:
    case mrMR2:
    case mrMR3:
    case mrMR4:
    case mrMR5:
    case mrMR6:
    case mrMR7:
    case mrMR8:
    case mrMR10:
    case mrMR13:
    case mrMR37:
    case mrMR38:
    case mrMR39:
    case mrMR40:
    case mrMR45:
    case mrMR48:
    case mrMR34:
    case mrMR35:
    case mrMR36:
    case mrMR129:
    case mrMR130:
    case mrMR131:
    case mrMR132:
    case mrMR137:
    case mrMR138:
    case mrMR139:
    case mrMR140:
    case mrMR145:
    case mrMR146:
    case mrMR147:
    case mrMR148:
    case mrMR153:
    case mrMR154:
    case mrMR155:
    case mrMR156:
    case mrMR161:
    case mrMR162:
    case mrMR163:
    case mrMR164:
    case mrMR169:
    case mrMR170:
    case mrMR171:
    case mrMR172:
    case mrMR177:
    case mrMR178:
    case mrMR179:
    case mrMR180:
    case mrMR185:
    case mrMR186:
    case mrMR187:
    case mrMR188:
    case mrMR193:
    case mrMR194:
    case mrMR195:
    case mrMR196:
    case mrMR201:
    case mrMR202:
    case mrMR203:
    case mrMR204:
    case mrMR209:
    case mrMR210:
    case mrMR211:
    case mrMR212:
    case mrMR217:
    case mrMR218:
    case mrMR219:
    case mrMR220:
    case mrMR225:
    case mrMR226:
    case mrMR227:
    case mrMR228:
    case mrMR233:
    case mrMR234:
    case mrMR235:
    case mrMR236:
    case mrMR241:
    case mrMR242:
    case mrMR243:
    case mrMR244:
    case mrMR249:
    case mrMR250:
    case mrMR251:

    case mrMR252:
      CmdType   = GmfCmdMrw;
      DelayType  = GmfDdr5Delay_tMRW;
      break;

    case mrMR11:
    case mrMR12:
      CmdType   = GmfCmdVref;
      DelayType = GmfDdr5Delay_tVREFCA;
      break;

    case mpcApplyVrefCa:
    case mpcMR13:
    case mpcMR32a0:
    case mpcMR32a1:
    case mpcMR32b0:
    case mpcMR32b1:
    case mpcMR33a0:
    case mpcMR33b0:
    case mpcMR33:
    case mpcMR34:
    case mpcDllReset:
    case mpcSetCmdTiming:
    case mpcSelectAllPDA:
      CmdType   = GmfCmdMpc;
      DelayType = GmfDdr5Delay_tMRD;
      break;

    case mpcZqCal:
      CmdType = GmfCmdMpc;
      DelayType = GmfDdr5Delay_tZQCAL;
      break;

    case mpcZqLat:
    case mpcEnterCaTrainMode:
      CmdType = GmfCmdMpc;
      DelayType = GmfDdr5Delay_tZQLAT;
      break;

    case mrPreAll:
      CmdType = GmfCmdPreAll;
      DelayType = GmfDdr5Delay_tPreAll;
      break;

    default:
      Status = mrcWrongInputParameter;
      break;
  }

  if (CmdTypeOut != NULL) {
    *CmdTypeOut = CmdType;
  }

  if (DelayTypeOut != NULL) {
    *DelayTypeOut = DelayType;
  }

  if (Status != mrcSuccess) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s failed for MrRegNum = %d\n", __FUNCTION__, MrRegNum);
  }

  return Status;
}

/**
  This function returns the requested DelayType timing in nCK units.

  @param[in]  MrcData      - Pointer to global MRC data.
  @param[in]  DelayType    - Requested delay type
  @param[out] TimingNckOut - Output variable for the requested delay timing in nCK units

  @retval MrcStatus - mrcSuccess if the DelayType is supported.
                      mrcWrongInputParameter if TimingNckOut is NULL.
                      mrcTimingError if the timing value overflows the output pointer.**/
MrcStatus
Ddr5GmfDelayTimings (
  IN  MrcParameters *const MrcData,
  IN  GmfTimingIndex       DelayType,
  OUT UINT16               *TimingNckOut
  )
{
  UINT32          TimingNck;
  UINT32          tCK;
  MrcOutput*      Outputs;
  const MrcInput* Inputs;
  MrcTiming*      Timing;
  UINT32          FirstController;
  UINT32          FirstChannel;
#ifdef MRC_DEBUG_PRINT
  MrcDebug  *Debug;

  Debug = &MrcData->Outputs.Debug;
#endif

  Outputs = &MrcData->Outputs;
  Inputs = &MrcData->Inputs;
  FirstController = Outputs->FirstPopController;
  FirstChannel = Outputs->Controller[FirstController].FirstPopCh;
  Timing = &Outputs->Controller[FirstController].Channel[FirstChannel].Timing[Inputs->ExtInputs.Ptr->MemoryProfile];


  if (TimingNckOut == NULL) {
    return mrcWrongInputParameter;
  }

  tCK = MrcData->Outputs.MemoryClock;
  switch ((GmfDdr5DelayType) DelayType)
  {
    case GmfDdr5Delay_tVREFCA:
      TimingNck = MrcGetTmod (MrcData, tCK);
      break;

    case GmfDdr5Delay_tMRD:
      // DDR5 "tMRD"
      // max(14ns, 16nCK)
      TimingNck = MrcGetTmod (MrcData, tCK);
      break;

    case GmfDdr5Delay_tZQCAL:
      TimingNck = MrcGetTzqcal (MrcData, tCK);
      break;

    case GmfDdr5Delay_tZQLAT:
      TimingNck = MrcGetTzqcs (MrcData, tCK);
      break;

    case GmfDdr5Delay_tDFE:
      // DDR5 DFE Mode Register Write Update Delay Time
      // Min 80ns
      TimingNck = DIVIDECEIL (80*1000*1000, tCK);
      break;

    case GmfDdr5Delay_tPreAll:
      TimingNck = Timing->tRCDtRP; // in tCK units
      break;

     case GmfDdr5Delay_tMRW:
      // max(5ns, 8nCK)
      TimingNck = MrcGettMRW (MrcData, tCK);
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
  This function will setup the default MR values for DDR5 based on
  DRAM Timings and Frequency in MRC global data.
  Only populated Channels and Ranks are initialized.

  @param[in, out]  MrcData   -  Pointer to MRC global data

  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
InitMrwDdr5 (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcDebug      *Debug;
  MrcOutput     *Outputs;
  MrcChannelOut *ChannelOut;
  UINT8         *MrPtr;
  UINT32        MrIndex;
  MrcStatus     Status;
  UINT32        Controller;
  UINT32        Channel;
  UINT32        Dimm;
  UINT32        DimmRank;
  UINT32        Rank;
  UINT32        Index;
  INT16         OdtDecodeValue1;
  INT16         OdtDecodeValue2;
  MrcModeRegister CurMrAddr;
  DDR5_MODE_REGISTER_34_TYPE *Mr34;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  Status = mrcSuccess;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "InitMrwDdr5:\n");

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
          continue;
        }
        Dimm = RANK_TO_DIMM_NUMBER (Rank);
        DimmRank = Rank % MAX_RANK_IN_DIMM;
        for (Index = 0; Ddr5MrInitList[Index] != mrEndOfSequence; Index++) {
          CurMrAddr = Ddr5MrInitList[Index];
          ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
          MrPtr = ChannelOut->Dimm[Dimm].Rank[DimmRank].MR;
          MrIndex = MrcMrAddrToIndex (MrcData, &CurMrAddr);
            // Fetch the init value
            Status = Ddr5JedecInitVal (MrcData, CurMrAddr, &MrPtr[MrIndex], Controller, Channel, Rank);
            if (Status != mrcSuccess) {
              return Status;
            }

          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " MC%u C%u R%u MrAddr = %3u MrIndex = %2u Value = 0x%02X ",
                         Controller, Channel, Rank, CurMrAddr, MrIndex, MrPtr[MrIndex]);
          switch (CurMrAddr) {
            case mrMR34:
              Mr34 = (DDR5_MODE_REGISTER_34_TYPE *) &MrPtr[MrIndex];
              OdtDecodeValue1 = LpddrOdtDecode (Mr34->Bits.RttWr);
              OdtDecodeValue2 = LpddrOdtDecode (Mr34->Bits.RttPark);
              MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RttWr=%d RttPark=%d", OdtDecodeValue1, OdtDecodeValue2);
              break;
            default:
              break;
          }
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
        } // Index
      } // Rank
    } // Channel
  } // Controller
  return Status;
}

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
  @retval mrcWrongInputParameter DelayType Pointer is NULL
**/
MrcStatus
MrcDdr5MrwGetTiming (
  IN  MrcParameters *const MrcData,
  IN  MrcModeRegister      CurMrAddr,
  IN  MrcModeRegister      NextMrAddr, OPTIONAL
  IN  GmfCmdType           NextCmdType, OPTIONAL
  IN  BOOLEAN              Isx8DramWidth,
  OUT GmfDdr5DelayType     *DelayType
  )
{
  if (DelayType == NULL) {
    return mrcWrongInputParameter;
  }

  if (Isx8DramWidth && Ddr5IsDimmDfeMr (CurMrAddr) && ((NextMrAddr >= mrMR193) && (NextMrAddr <= mrMR252))) {
    // x8 Should not program DFE DQU, so ensure last DQL DFE MR uses tDFE
    *DelayType = GmfDdr5Delay_tDFE;
  } else if (Ddr5IsDimmDfeMr (CurMrAddr) && !Ddr5IsDimmDfeMr (NextMrAddr)) {
    // Ensure last DFE DQU MR uses tDFE
    *DelayType = GmfDdr5Delay_tDFE;
  } else if (NextMrAddr == mrMR10 || NextMrAddr == mrMR3 || NextMrAddr == mrMR7 || NextMrAddr == mpcSelectAllPDA) {
    // MRs that are programmed with PDA require an MPC command to precede the MRW, therefore use tMRD timing
    *DelayType = GmfDdr5Delay_tMRD;
  } else if (NextCmdType != GmfCmdMrw) {
    // Ensure MRW to non MRW uses tMRD
    *DelayType = GmfDdr5Delay_tMRD;
  } else {
    // Default to tMRW
    *DelayType = GmfDdr5Delay_tMRW;
  }
  return mrcSuccess;
}

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
  )
{
  const MRC_FUNCTION  *MrcCall;
  MrcStatus     Status;
  MrcInput      *Inputs;
  MrcOutput     *Outputs;
  MrcChannelOut *ChannelOut;
  MrcDebug      *Debug;
  UINT32        Channel;
  UINT32        Controller;
  UINT32        Dimm;
  UINT32        DimmRank;
  UINT32        Rank;
  UINT32        OutIdx;
  UINT16        Delay;
  UINT8         Data;
  UINT32        MrIndex;
  UINT32        Index;
  UINT8         *MrPtr;
  GmfDdr5DelayType DelayType;
  MrcModeRegister  CurMrAddr;
  MrcModeRegister  NextMrAddr;
  GmfCmdType       CurCmdType;
  GmfCmdType       NextCmdType;
  const MrcModeRegister  *MrPerRank;
  BOOLEAN                 IsPdaMr;
  GenericFsmConfigMode    ConfigSeq;
  MRC_GEN_MRS_FSM_MR_TYPE *GenMrsFsmMr;
  MRC_GEN_MRS_FSM_MR_TYPE MrData[MAX_CONTROLLER][MAX_CHANNEL][MAX_RANK_IN_CHANNEL][MAX_MR_GEN_FSM];
  BOOLEAN Isx8DramWidth;
  BOOLEAN Isx16DramWidthSameChannel;
  MrcDebugMsgLevel DebugLevel;

  // These MR's can be trained per rank and hence should be marked per Rank in Gen MRS FSM for runtime
  static const MrcModeRegister SagvMrPerRankDdr5[] = {
    mpcMR32a0,  // RTT_CK Group A
    mpcMR32a1,  // RTT_CS Group A
    mpcMR33a0,  // RTT_CA Group A
    mpcMR32b0,  // RTT_CK Group B
    mpcMR32b1,  // RTT_CS Group B
    mpcMR33b0,  // RTT_CA Group B
    mpcMR33,    // DQS_RTT_PARK
    mpcMR34,    // RTT_PARK
    mrMR3,      // Write Leveling Internal Cycle Alignment
//  mrMR5,      // Ron pull-up / pull-down  @todo_adl Include this if DIMM Ron will be trained
    mrMR7,      // Write Leveling Internal Cycle Half Step Alignment
    mrMR10,     // VREF_DQ
    mrMR11,     // VREF_CA
    mrMR12,     // VREF_CS
    mrMR34,     // RTT_WR
    mrMR35,     // RTT_NOM_WR, RTT_NOM_RD
//  mrMR37,     // ODTLon_WR_Offset,    ODTLoff_WR_Offset
//  mrMR38,     // ODTLon_WR_NT_Offset, ODTLoff_WR_NT_Offset
//  mrMR39,     // ODTLon_RD_NT_Offset, ODTLoff_RD_NT_Offset
    mrMR48,     // Write Pattern
    mrMR129,    // Start DFE Registers
    mrMR130,
    mrMR131,
    mrMR132,
    mrMR137,
    mrMR138,
    mrMR139,
    mrMR140,
    mrMR145,
    mrMR146,
    mrMR147,
    mrMR148,
    mrMR153,
    mrMR154,
    mrMR155,
    mrMR156,
    mrMR161,
    mrMR162,
    mrMR163,
    mrMR164,
    mrMR169,
    mrMR170,
    mrMR171,
    mrMR172,
    mrMR177,
    mrMR178,
    mrMR179,
    mrMR180,
    mrMR185,
    mrMR186,
    mrMR187,
    mrMR188,
    mrMR193,
    mrMR194,
    mrMR195,
    mrMR196,
    mrMR201,
    mrMR202,
    mrMR203,
    mrMR204,
    mrMR209,
    mrMR210,
    mrMR211,
    mrMR212,
    mrMR217,
    mrMR218,
    mrMR219,
    mrMR220,
    mrMR225,
    mrMR226,
    mrMR227,
    mrMR228,
    mrMR233,
    mrMR234,
    mrMR235,
    mrMR236,
    mrMR241,
    mrMR242,
    mrMR243,
    mrMR244,
    mrMR249,
    mrMR250,
    mrMR251,
    mrMR252, // End DFE Registers
    mrEndOfSequence
  };

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  MrcCall = Inputs->Call.Func;
  Delay   = 0;
  DebugLevel = ((SagvConfig) ? MSG_LEVEL_ERROR : MSG_LEVEL_NONE);

  // Clear out our array
  MrcCall->MrcSetMem ((UINT8 *) MrData, sizeof (MrData), 0);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      Isx16DramWidthSameChannel = FALSE;
      ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
      for (Dimm = 0; Dimm < Outputs->MaxDimm; Dimm++) {
        if (ChannelOut->Dimm[Dimm].Status != DIMM_PRESENT) {
          continue;
        }
        Isx16DramWidthSameChannel = (ChannelOut->Dimm[Dimm].SdramWidth == 16) || Isx16DramWidthSameChannel;
      }
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
          continue;
        }
        Dimm = RANK_TO_DIMM_NUMBER (Rank);
        DimmRank = Rank % MAX_RANK_IN_DIMM;
        MrPtr = ChannelOut->Dimm[Dimm].Rank[DimmRank].MR;
        OutIdx = 0;
        MRC_DEBUG_MSG (Debug, DebugLevel, "MC%u.C%u.R%u\tData\tDelay\n", Controller, Channel, Rank);
        // Pass on the list twice. First pass: non-PDA, second pass: PDA
        for (ConfigSeq = GenericFsmConfigNonPdaMrs; ConfigSeq <= GenericFsmConfigPdaMrs; ConfigSeq++) {
          for (Index = 0; Sequence[Index] != mrEndOfSequence; Index++) {
            CurMrAddr = Sequence[Index];
            MrIndex = MrcMrAddrToIndex (MrcData, &CurMrAddr);
            Isx8DramWidth = (ChannelOut->Dimm[Dimm].SdramWidth == 8) && !Isx16DramWidthSameChannel;
            if (Isx8DramWidth && ((CurMrAddr >= mrMR193) && (CurMrAddr <= mrMR252))) {
              continue;
            }
            if ((!Inputs->ExtInputs.Ptr->IsDdr5MR7WicaSupported) && (CurMrAddr == mrMR7)) {
              continue;
            }
            if (MrIndex <  MAX_MR_IN_DIMM) {
              IsPdaMr = MrcMrIsPda (MrcData, Controller, Channel, CurMrAddr);
              if (ConfigSeq == GenericFsmConfigNonPdaMrs) {
                // In non-PDA section discard PDA MR's and also mpcSelectAllPDA
                if (IsPdaMr || (CurMrAddr == mpcSelectAllPDA)) {
                  continue;
                }
              } else { // GenericFsmConfigPdaMrs
                // In PDA section include only PDA MR's and also mpcSelectAllPDA
                if (!IsPdaMr && (CurMrAddr != mpcSelectAllPDA)) {
                  continue;
                }
              }

              // Fetch the Command and Delay type
              Status = MrcDdr5GetGmfAttributes (MrcData, CurMrAddr, &CurCmdType, &DelayType);
              if (Status != mrcSuccess) {
                return Status;
              }
              if (CurCmdType == GmfCmdMrw) {
                NextMrAddr = Sequence[Index+1];
                if (NextMrAddr != mrEndOfSequence) {
                  MrcMrAddrToIndex (MrcData, &NextMrAddr);
                  Status = MrcDdr5GetGmfAttributes (MrcData, NextMrAddr, &NextCmdType, NULL);
                  if (Status != mrcSuccess) {
                    return Status;
                  }
                  Status = MrcDdr5MrwGetTiming (MrcData, CurMrAddr, NextMrAddr, NextCmdType, Isx8DramWidth, &DelayType);
                  if (Status != mrcSuccess) {
                    return Status;
                  }
                }
              }
              // Fetch the data
              Data = MrPtr[MrIndex];
              if (OutIdx >= MAX_MR_GEN_FSM) {
                MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "OutIdx(%d) exceeded Max MR Addresses(%d)\n", OutIdx, MAX_MR_GEN_FSM);
                return mrcWrongInputParameter;
              }
              // Populate the Generic MRS FSM entry
              GenMrsFsmMr = &MrData[Controller][Channel][Rank][OutIdx];
              // In PDA Mr data will be extracted in the inner functions
              GenMrsFsmMr->PdaMr = IsPdaMr;

              // Fetch the data
              GenMrsFsmMr->MrData  = Data;

              if (CurMrAddr > MRC_UINT8_MAX) {
                // Special case for MPC commands
                if (!SagvConfig) {
                GenMrsFsmMr->MrAddr = 0;
                } else {
                  GenMrsFsmMr->MrAddr = CurMrAddr;
                }
              } else {
                GenMrsFsmMr->MrAddr = (UINT8) CurMrAddr;
                if (SagvConfig && (CurMrAddr == mrMR12)) {
                  GenMrsFsmMr->FreqSwitchPoint = TRUE;
                }
              }
              GenMrsFsmMr->Valid   = TRUE;
              GenMrsFsmMr->DelayIndex = (GmfTimingIndex) DelayType;
              GenMrsFsmMr->CmdType = CurCmdType;
              if (DebugLevel) {
                // Needed only for debug print
                MrcGetGmfDelayTiming (MrcData, (GmfTimingIndex) DelayType, &Delay);
              }
              MRC_DEBUG_MSG (Debug, DebugLevel, " MR%3u:\t\t0x%02X\t%3u\n", CurMrAddr, Data, Delay);
            } else {
              MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "MR index(%d) exceeded MR array length(%d)\n", MrIndex, MAX_MR_IN_DIMM);
              Status = mrcWrongInputParameter;
              return Status;
            }
            OutIdx++;
          } // Index
        } // ConfigSeq
      } // Rank
    } // Channel
  } // Controller

  MrPerRank = SagvConfig ? SagvMrPerRankDdr5 : NULL;

  // Program Generic MRS FSM Per Controller/Channel
  Status = MrcGenMrsFsmConfig (MrcData, MrData, SagvConfig, MrPerRank);
  if (Status != mrcSuccess) {
    return Status;
  }

  if (!SagvConfig) {
    // Run Generic FSM
    // Since we use the MRS FSM, we've configured it to send ZQ at the end if the sequence.
    // DDR5 ZQ is skipped if the sequence includes a ZQCAL MPC command.
    Status = MrcGenMrsFsmRun (MrcData);
    if (Status != mrcSuccess) {
      return Status;
    }

    // Clean The Generic FSM used  entries
    // Entries need to be invalidated for next run
    Status = MrcGenMrsFsmClean (MrcData, MrData, FALSE);
    if (Status != mrcSuccess) {
      return Status;
    }
  }

  return Status;
}

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
  )
{
  MrcOutput   *Outputs;
  UINT32      UIPs;
  UINT32      Tdqs2dqPs;
  MrcFrequency DdrFrequency;

  Outputs = &MrcData->Outputs;
  UIPs   = Outputs->UIps;
  DdrFrequency = Outputs->Frequency;

  // tRX_DQS2DQ is in 100UI units
  if (IsMin) {
    Tdqs2dqPs = MRC_DDR5_tRX_DQS2DQ_MIN;
  } else {
    if (DdrFrequency <= f3200) {
      Tdqs2dqPs = MRC_DDR5_tRX_DQS2DQ_3200;
    } else if (DdrFrequency <= f3600) {
      Tdqs2dqPs = MRC_DDR5_tRX_DQS2DQ_3600;
    } else if (DdrFrequency <= f4000) {
      Tdqs2dqPs = MRC_DDR5_tRX_DQS2DQ_4000;
    } else if (DdrFrequency <= f4400) {
      Tdqs2dqPs = MRC_DDR5_tRX_DQS2DQ_4400;
    } else if (DdrFrequency <= f4800) {
      Tdqs2dqPs = MRC_DDR5_tRX_DQS2DQ_4800;
    } else if (DdrFrequency <= f5200) {
      Tdqs2dqPs = MRC_DDR5_tRX_DQS2DQ_5200;
    } else if (DdrFrequency <= f5600) {
      Tdqs2dqPs = MRC_DDR5_tRX_DQS2DQ_5600;
    } else if (DdrFrequency <= f6000) {
      Tdqs2dqPs = MRC_DDR5_tRX_DQS2DQ_6000;
    } else if (DdrFrequency <= f8800) {
      Tdqs2dqPs = MRC_DDR5_tRX_DQS2DQ_6400_8800;
    } else {
      // Default to 5UI Sweep
      Tdqs2dqPs = 5 * UIPs;
    }
  }
  // Divide by 100 due to 100UI factor

  return (Tdqs2dqPs * 1000); // Convert Picoseconds to Femptoseconds
}

/**
  Set CAS latency on on All DRAMs in the system

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
  )
{
  UINT32                      Controller;
  UINT32                      Channel;
  UINT8                       Rank;
  UINT8                       RankHalf;
  UINT8                       RankMod2;
  UINT8                       ValidRankMask;
  UINT8                       RankMask;
  MrcStatus                   Status;
  MrcStatus                   MrsStatus;
  MrcChannelOut               *ChannelOut;
  MrcOutput                   *Outputs;
  DDR5_MODE_REGISTER_0_TYPE   MR0;

  Outputs       = &MrcData->Outputs;
  ValidRankMask = Outputs->ValidRankMask;
  Status        = mrcSuccess;


  for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
    RankMask = (1 << Rank);
    if (!(RankMask & ValidRankMask)) {
      // Skip if all channels empty
      continue;
    }

    RankHalf      = Rank / MAX_RANK_IN_DIMM;
    RankMod2      = Rank % MAX_RANK_IN_DIMM;

    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        if (!(MrcRankExist (MrcData, Controller, Channel, Rank))) {
          continue;
        }
        ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
        MR0.Data8 = ChannelOut->Dimm[RankHalf].Rank[RankMod2].MR[mrIndexMR0];
        MR0.Bits.CasLatency = CasLatency;
        MrsStatus = MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR0, MR0.Data8, FALSE);
        if (MrsStatus != mrcSuccess) {
          Status |= MrsStatus;
        }
      }  // for Channel
    } // Controller
  }

  return Status;
}


/**
  This function will set up the pointer passed in based on DDR5 Mode Register definition for CCC ODT (CS/CA supported)
  If MRC_IGNORE_ARGx is passed in, that parameter is ignored.
  OdtValue does error checking against spec valid values.

  @param[in]      MrcData     - Pointer to global MRC data.
  @param[in]      OdtParam    - ODT Param that is being updated
  @param[in]      OdtValue    - ODT value to set (in Ohms).
  @param[in,out]  MrData      - Pointer to MR data to update.

  @retval MrcStatus - mrcSuccess if a supported ODT value, else mrcWrongInputParameter.
**/
MrcStatus
MrcDdr5SetCccOdt (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 OdtParam,
  IN  UINT16                OdtValue,
  IN OUT  UINT8     *const  MrData
  )
{
  MrcDebug  *Debug;
  MrcStatus Status;
  INT8      Encoding;

  Status  = mrcSuccess;
  Debug   = &MrcData->Outputs.Debug;

  if (OdtValue != MRC_IGNORE_ARG_16) {
    Encoding = CccOdtEncode_DDR5 (OdtValue);

    if (Encoding == -1) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid ODT Value: %u\n", gErrString, OdtValue);
      Status = mrcWrongInputParameter;
    } else {
      switch (OdtParam) {
        case OptDimmOdtCAGroupA:
          *MrData = DDR5_MPC_GROUP_A_RTT_CA (Encoding);
          break;

        case OptDimmOdtCAGroupB:
          *MrData = DDR5_MPC_GROUP_B_RTT_CA (Encoding);
          break;

        case OptDimmOdtCSGroupA:
          *MrData = DDR5_MPC_GROUP_A_RTT_CS (Encoding);
          break;

        case OptDimmOdtCSGroupB:
          *MrData = DDR5_MPC_GROUP_B_RTT_CS (Encoding);
          break;

        default:
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Unsupported ODT Param: %u\n", gErrString, OdtParam);
          Status = mrcWrongInputParameter;
          break;
      }
    }
  }

  return Status;
}

/**
  This function will set up the pointer passed in based on DDR5 Mode Register definition for RTT_PARK
  If MRC_IGNORE_ARGx is passed in, that parameter is ignored.
  OdtValue does error checking against spec valid values.

  @param[in]      MrcData     - Pointer to global MRC data.
  @param[in]      OdtParam    - ODT Param that is being updated
  @param[in]      OdtValue    - ODT value to set (in Ohms).
  @param[in,out]  MrData      - Pointer to MR data to update.

  @retval MrcStatus - mrcSuccess if a supported ODT value, else mrcWrongInputParameter.
**/
MrcStatus
MrcDdr5SetParkOdt (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 OdtParam,
  IN  UINT16                OdtValue,
  IN OUT  UINT8     *const  MrData
  )
{
  MrcDebug  *Debug;
  MrcStatus Status;
  INT8      Encoding;

  Status  = mrcSuccess;
  Debug   = &MrcData->Outputs.Debug;

  if (OdtValue != MRC_IGNORE_ARG_16) {
    Encoding = Ddr5OdtEncode (OdtValue);

    if (Encoding == -1) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid RTT_PARK Value: %u\n", gErrString, OdtValue);
      Status = mrcWrongInputParameter;
    } else {
      switch (OdtParam) {
        case OptDimmOdtPark:
          *MrData = DDR5_MPC_SET_RTT_PARK (Encoding);
          break;

        case OptDimmOdtParkDqs:
          *MrData = DDR5_MPC_SET_DQS_RTT_PARK (Encoding);
          break;

        default:
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Unsupported ODT Param: %u\n", gErrString, OdtParam);
          Status = mrcWrongInputParameter;
          break;
      }
    }
  }

  return Status;
}

/**
  This function will set up the pointer passed in based on DDR5 Mode Register definition for MR5
  If MRC_IGNORE_ARGx is passed in, that parameter is ignored.
  OdtValue does error checking against spec valid values.

  @param[in]      MrcData     - Pointer to global MRC data.
  @param[in]      DataOutDis  - Data Output Disable
  @param[in]      PuDrvStr    - Pull-Up Output Driver Impedance
  @param[in]      TDQSEn      - TDQS Enable
  @param[in]      DMEn        - DM Enable
  @param[in]      PdDrvStr    - Pull-Down Output Driver Impedance
  @param[in,out]  MrData      - Pointer to MR data to update.

  @retval MrcStatus - mrcSuccess if a supported ODT value, else mrcWrongInputParameter.
**/
MrcStatus
MrcDdr5SetMr5 (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 DataOutDis,
  IN  UINT16                PuDrvStr,
  IN  UINT8                 TDQSEn,
  IN  UINT8                 DMEn,
  IN  UINT16                PdDrvStr,
  IN OUT  UINT8     *const  MrData
  )
{
  MrcDebug  *Debug;
  MrcStatus Status;
  INT8      Encoding;
  DDR5_MODE_REGISTER_5_TYPE  *Mr5;

  Status  = mrcSuccess;
  Mr5     = (DDR5_MODE_REGISTER_5_TYPE *) MrData;
  Debug   = &MrcData->Outputs.Debug;

  if (DataOutDis != MRC_IGNORE_ARG_8) {
    Mr5->Bits.DataOutputDisable = DataOutDis;
  }

  if (PuDrvStr != MRC_IGNORE_ARG_16) {
    Encoding = Ddr5DrvStrEncode (PuDrvStr);
    if (Encoding == -1) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid %s Value: %u\n", gErrString, gDrvStr, PuDrvStr);
      Status = mrcWrongInputParameter;
    } else {
      Mr5->Bits.PullUpOutputDriverImpedance = Encoding;
    }
  }

  if (TDQSEn != MRC_IGNORE_ARG_8) {
    Mr5->Bits.TdqsEnable = TDQSEn;
  }

  if (DMEn != MRC_IGNORE_ARG_8) {
    Mr5->Bits.DmEnable = DMEn;
  }

  if (PdDrvStr != MRC_IGNORE_ARG_16) {
    Encoding = Ddr5DrvStrEncode (PdDrvStr);
    if (Encoding == -1) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid %s Value: %u\n", gErrString, gDrvStr, PdDrvStr);
      Status = mrcWrongInputParameter;
    } else {
      Mr5->Bits.PullDownOutputDriverImpedance = Encoding;
    }
  }

  return Status;
}

/**
  This function will set up the pointer passed in based on DDR5 Mode Register definition for MR34
  If MRC_IGNORE_ARGx is passed in, that parameter is ignored.
  OdtValue does error checking against spec valid values.

  @param[in]      MrcData     - Pointer to global MRC data.
  @param[in]      RttWr       - ODT WR
  @param[in,out]  MrData      - Pointer to MR data to update.

  @retval MrcStatus - mrcSuccess if a supported ODT value, else mrcWrongInputParameter.
**/
MrcStatus
MrcDdr5SetMr34 (
  IN  MrcParameters *const  MrcData,
  IN  UINT16                RttWr,
  IN OUT  UINT8     *const  MrData
  )
{
  MrcDebug  *Debug;
  MrcStatus Status;
  INT8      Encoding;
  DDR5_MODE_REGISTER_34_TYPE  *Mr34;

  Status  = mrcSuccess;
  Mr34    = (DDR5_MODE_REGISTER_34_TYPE *) MrData;
  Debug   = &MrcData->Outputs.Debug;

  if (RttWr != MRC_IGNORE_ARG_16) {
    Encoding = Ddr5OdtEncode (RttWr);
    if (Encoding == -1) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid ODT WR Value: %u\n", gErrString, RttWr);
      Status = mrcWrongInputParameter;
    } else {
      Mr34->Bits.RttWr = Encoding;
    }
  }

  return Status;
}

/**
  This function will set up the pointer passed in based on DDR5 Mode Register definition for MR35
  If MRC_IGNORE_ARGx is passed in, that parameter is ignored.
  OdtValue does error checking against spec valid values.

  @param[in]      MrcData     - Pointer to global MRC data.
  @param[in]      RttNomWr    - ODT NOM WR
  @param[in]      RttNomRd    - ODT NOM RD
  @param[in,out]  MrData      - Pointer to MR data to update.

  @retval MrcStatus - mrcSuccess if a supported ODT value, else mrcWrongInputParameter.
**/
MrcStatus
MrcDdr5SetMr35 (
  IN  MrcParameters *const  MrcData,
  IN  UINT16                RttNomWr,
  IN  UINT16                RttNomRd,
  IN OUT  UINT8     *const  MrData
  )
{
  MrcDebug  *Debug;
  MrcStatus Status;
  INT8      Encoding;
  DDR5_MODE_REGISTER_35_TYPE  *Mr35;

  Status  = mrcSuccess;
  Mr35    = (DDR5_MODE_REGISTER_35_TYPE *) MrData;
  Debug   = &MrcData->Outputs.Debug;

  if (RttNomWr != MRC_IGNORE_ARG_16) {
    Encoding = Ddr5OdtEncode (RttNomWr);
    if (Encoding == -1) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid ODT NOM WR Value: %u\n", gErrString, RttNomWr);
      Status = mrcWrongInputParameter;
    } else {
      Mr35->Bits.RttNomWr = Encoding;
    }
  }

  if (RttNomRd != MRC_IGNORE_ARG_16) {
    Encoding = Ddr5OdtEncode (RttNomRd);
    if (Encoding == -1) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid ODT NOM RD Value: %u\n", gErrString, RttNomRd);
      Status = mrcWrongInputParameter;
    } else {
      Mr35->Bits.RttNomRd = Encoding;
    }
  }

  return Status;
}

/**
  This function will set up the pointer passed in based on DDR5 Mode Register definition for MR37
  MRC_IGNORE_ARG8 must be used to ignore the parameter
  OdtValue does error checking against spec valid values.

  @param[in]      MrcData     - Pointer to global MRC data.
  @param[in]      OdtlOnWr    - ODTLON WR OFFSET
  @param[in]      OdtlOffWr   - ODTLOFF WR OFFSET
  @param[in,out]  MrData      - Pointer to MR data to update.

  @retval MrcStatus - mrcSuccess if a supported ODT Offset value, else mrcWrongInputParameter.
**/
MrcStatus
MrcDdr5SetMr37 (
  IN  MrcParameters *const MrcData,
  IN  INT16                OdtlOnWr,
  IN  INT16                OdtlOffWr,
  IN OUT  UINT8     *const MrData
  )
{
  MrcDebug  *Debug;
  MrcStatus Status;
  INT8     Encoding;
  DDR5_MODE_REGISTER_37_TYPE  *Mr37;

  Status  = mrcSuccess;
  Mr37    = (DDR5_MODE_REGISTER_37_TYPE *) MrData;
  Debug   = &MrcData->Outputs.Debug;

  if (OdtlOnWr != MRC_IGNORE_ARG_8) {
    Encoding = Ddr5OdttOnWrOffset (OdtlOnWr);
    if (Encoding == -1) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid ODTLON WR OFFSET Value: %d\n", gErrString, OdtlOnWr);
      Status = mrcWrongInputParameter;
    } else {
      Mr37->Bits.OdtlOnWrOffset = Encoding;
    }
  }
  if (OdtlOffWr != MRC_IGNORE_ARG_8) {
    Encoding = Ddr5OdttOffWrOffset (OdtlOffWr);
    if (Encoding == -1) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid ODTLOFF WR OFFSET Value: %d\n", gErrString, OdtlOffWr);
      Status = mrcWrongInputParameter;
    } else {
      Mr37->Bits.OdtlOffWrOffset = Encoding;
    }
  }

  return Status;
}

/**
  This function will set up the pointer passed in based on DDR5 Mode Register definition for MR38
  MRC_IGNORE_ARG8 must be used to ignore the parameter
  OdtValue does error checking against spec valid values.

  @param[in]      MrcData     - Pointer to global MRC data.
  @param[in]      OdtlOnWrNt  - ODTLON WR NT OFFSET
  @param[in]      OdtlOffWrNt - ODTLOFF WR NT OFFSET
  @param[in,out]  MrData      - Pointer to MR data to update.

  @retval MrcStatus - mrcSuccess if a supported ODT Offset value, else mrcWrongInputParameter.
**/
MrcStatus
MrcDdr5SetMr38 (
  IN  MrcParameters *const  MrcData,
  IN  INT16                 OdtlOnWrNt,
  IN  INT16                 OdtlOffWrNt,
  IN OUT  UINT8     *const  MrData
  )
{
  MrcDebug  *Debug;
  MrcStatus Status;
  INT8      Encoding;
  DDR5_MODE_REGISTER_38_TYPE  *Mr38;

  Status  = mrcSuccess;
  Mr38    = (DDR5_MODE_REGISTER_38_TYPE *) MrData;
  Debug   = &MrcData->Outputs.Debug;
  if (OdtlOnWrNt != MRC_IGNORE_ARG_8) {
    Encoding = Ddr5OdttOnWrOffset (OdtlOnWrNt);
    if (Encoding == -1) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid ODTLON WR NT Offset Value: %d\n", gErrString, OdtlOnWrNt);
      Status = mrcWrongInputParameter;
    } else {
      Mr38->Bits.OdtlOnWrNtOffset = Encoding;
    }
  }
  if (OdtlOffWrNt != MRC_IGNORE_ARG_8) {
    Encoding = Ddr5OdttOffWrOffset (OdtlOffWrNt);
    if (Encoding == -1) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid ODTLOFF WR NT Offset Value: %d\n", gErrString, OdtlOffWrNt);
      Status = mrcWrongInputParameter;
    } else {
      Mr38->Bits.OdtlOffWrNtOffset = Encoding;
    }
  }

  return Status;
}

/**
  This function will set up the pointer passed in based on DDR5 Mode Register definition for MR39
  MRC_IGNORE_ARG8 must be used to ignore the parameter
  OdtValue does error checking against spec valid values.

  @param[in]      MrcData     - Pointer to global MRC data.
  @param[in]      OdtlOnRd    - ODTLON RD OFFSET
  @param[in]      OdtlOffRd   - ODTLOFF RD OFFSET
  @param[in,out]  MrData      - Pointer to MR data to update.

  @retval MrcStatus - mrcSuccess if a supported ODT value, else mrcWrongInputParameter.
**/
MrcStatus
MrcDdr5SetMr39 (
  IN  MrcParameters *const  MrcData,
  IN  INT16                 OdtlOnRd,
  IN  INT16                 OdtlOffRd,
  IN OUT  UINT8     *const  MrData
  )
{
  MrcDebug  *Debug;
  MrcStatus Status;
  INT8      Encoding;
  DDR5_MODE_REGISTER_39_TYPE  *Mr39;

  Status  = mrcSuccess;
  Mr39    = (DDR5_MODE_REGISTER_39_TYPE *) MrData;
  Debug   = &MrcData->Outputs.Debug;
  if (OdtlOnRd != MRC_IGNORE_ARG_8) {
    Encoding = Ddr5OdttOnRdNtOffset (OdtlOnRd);
    if (Encoding == -1) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid ODTLON RD OFFSET Value: %d\n", gErrString, OdtlOnRd);
      Status = mrcWrongInputParameter;
    } else {
      Mr39->Bits.OdtlOnRdNtOffset = Encoding;
    }
  }
  if (OdtlOffRd != MRC_IGNORE_ARG_8) {
    Encoding = Ddr5OdttOffRdNtOffset (OdtlOffRd);
    if (Encoding == -1) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid ODTLOFF RD OFFSET Value: %d\n", gErrString, OdtlOffRd);
      Status = mrcWrongInputParameter;
    } else {
      Mr39->Bits.OdtlOffRdNtOffset = Encoding;
    }
  }

  return Status;
}

/**
  Update the Ddr5 Rx Dqs Offset Timing in MR40 and send this to DRAM.
  Update MR40 in the host struct as well

  @param[in]  MrcData -  Pointer to MRC global data.

  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcSetDdr5Mr40 (
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
  DDR5_MODE_REGISTER_40_TYPE  Mr40;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  Status  = mrcSuccess;
  Mr40.Data8 = 0;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
          continue;
        }
        ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
        MrPtr = ChannelOut->Dimm[Rank / MAX_RANK_IN_DIMM].Rank[Rank % MAX_RANK_IN_DIMM].MR;
        Mr40.Bits.ReadDqsOffsetTiming = Outputs->EarlyDqs;
        MrPtr[mrIndexMR40] = Mr40.Data8;
      }
    }
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "New MR40: 0x%x\n", Mr40.Data8);

  // Program the new MR40 value on all populated ranks
  MrcIssueMrwMulticast (MrcData, Outputs->ValidRankMask, mrMR40, Mr40.Data8, MRC_PRINTS_OFF);

  return Status;
}

/**
  This function will set up the pointer passed in based on DDR5 Mode Register definition for DFE Tap
  If MRC_IGNORE_ARGx is passed in, that parameter is ignored.
  OdtValue does error checking against spec valid values.

  @param[in]      MrcData     - Pointer to global MRC data.
  @param[in]      OptParam    - Param that is being updated
  @param[in]      DfeValue    - DFE value to program
  @param[in,out]  MrData      - Pointer to MR data to update.

  @retval MrcStatus - mrcSuccess if a supported ODT value, else mrcWrongInputParameter.
**/
MrcStatus
MrcDdr5SetDfe (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 OptParam,
  IN  INT16                 DfeValue,
  IN OUT  UINT8     *const  MrData
  )
{
  MrcDebug  *Debug;
  MrcStatus Status;
  INT16     Range;
  INT16     Value;
  DDR5_MODE_REGISTER_129_TYPE  *MrDfe;

  Status  = mrcSuccess;
  MrDfe   = (DDR5_MODE_REGISTER_129_TYPE *) MrData;
  Debug   = &MrcData->Outputs.Debug;
  Range   = 0;

  if ((UINT16) DfeValue != MRC_IGNORE_ARG_16) {
    switch (OptParam) {
      case OptDdr5DimmDFETap1:
        Range = DDR5_DIMM_DFE_TAP1_RANGE;
        break;

      case OptDdr5DimmDFETap2:
        Range = DDR5_DIMM_DFE_TAP2_RANGE;
        break;

      case OptDdr5DimmDFETap3:
        Range = DDR5_DIMM_DFE_TAP3_RANGE;
        break;

      case OptDdr5DimmDFETap4:
        Range = DDR5_DIMM_DFE_TAP4_RANGE;
        break;

      default:
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid OptParam: %u\n", gErrString, OptParam);
        Status = mrcWrongInputParameter;
        break;

    }
    if (Status != mrcSuccess) {
      return Status;
    }
    Value = RANGE (DfeValue, -Range, Range);

    if (Value != DfeValue) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "%s DfeValue: %u exceeded Range (%d:%d)\n", gWarnString, DfeValue, -Range, Range);
    }

    *MrDfe = Ddr5DfeEncode (Value);
  }
  return Status;
}

/**
  This function will set up DFE value for DDR5 in PDA mode

  @param[in]      MrcData     - Pointer to global MRC data.
  @param[in]      Controller  - Controller index to store data.
  @param[in]      Channel     - Channel index to store data.
  @param[in]      Rank        - Rank to store data.
  @param[in]      Device      - Device to store data.
  @param[in]      Byte        - Byte to store data.
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
  )
{

  MrcOutput*          Outputs;
  MrcChannelOut*      ChannelOut;
  MrcDimmOut*         DimmOut;
  MrcRankOut*         RankOut;
  MrcDebug*           Debug;
  INT16               Range;
  INT16               Value;
  MrcStatus           Status;
  DDR5_MODE_REGISTER_129_TYPE MrDfe;


  Outputs = &MrcData->Outputs;
  Status = mrcSuccess;
  Debug = &MrcData->Outputs.Debug;
  Range = 0;
  MrDfe.Data8 = 0;

  if ((UINT16)DfeValue != MRC_IGNORE_ARG_16) {
    switch (OptParam) {
    case OptDdr5DimmDFETap1:
      Range = DDR5_DIMM_DFE_TAP1_RANGE;
      break;

    case OptDdr5DimmDFETap2:
      Range = DDR5_DIMM_DFE_TAP2_RANGE;
      break;

    case OptDdr5DimmDFETap3:
      Range = DDR5_DIMM_DFE_TAP3_RANGE;
      break;

    case OptDdr5DimmDFETap4:
      Range = DDR5_DIMM_DFE_TAP4_RANGE;
      break;

    default:
      MRC_DEBUG_MSG(Debug, MSG_LEVEL_ERROR, "%s Invalid OptParam: %u\n", gErrString, OptParam);
      Status = mrcWrongInputParameter;
      break;
    }
    if (Status != mrcSuccess) {
      return Status;
    }
    Value = RANGE(DfeValue, -Range, Range);
    if (Value != DfeValue) {
      MRC_DEBUG_MSG(Debug, MSG_LEVEL_WARNING, "%s DfeValue: %u exceeded Range (%d:%d)\n", gWarnString, DfeValue, -Range, Range);
    }

    MrDfe = Ddr5DfeEncode(Value);
  }

  ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
  DimmOut = &ChannelOut->Dimm[Rank / MAX_RANK_IN_DIMM];
  RankOut = &DimmOut->Rank[Rank%2];

  switch (OptParam) {
  case OptDdr5DimmDFETap1:
    if ((Byte % 2) == 0) { // Lower DQ's
      RankOut->Ddr5PdaMr129[Device] = MrDfe.Data8;
    } else {  // Upper DQ's
      RankOut->Ddr5PdaMr193[Device] = MrDfe.Data8;
    }
    break;

  case OptDdr5DimmDFETap2:
    if ((Byte % 2) == 0) { // Lower DQ's
      RankOut->Ddr5PdaMr130[Device] = MrDfe.Data8;
    } else {  // Upper DQ's
      RankOut->Ddr5PdaMr194[Device] = MrDfe.Data8;
    }
    break;

  case OptDdr5DimmDFETap3:
    if ((Byte % 2) == 0) { // Lower DQ's
      RankOut->Ddr5PdaMr131[Device] = MrDfe.Data8;
    } else {  // Upper DQ's
      RankOut->Ddr5PdaMr195[Device] = MrDfe.Data8;
    }
    break;

  case OptDdr5DimmDFETap4:
    if ((Byte % 2) == 0) { // Lower DQ's
      RankOut->Ddr5PdaMr132[Device] = MrDfe.Data8;
    } else {  // Upper DQ's
      RankOut->Ddr5PdaMr196[Device] = MrDfe.Data8;
    }
    break;

  default:
    break;
  }

  return Status;
}

/**
  This function enables/disables DFE through MR111 for the given Controller/Channel/Rank

  @param[in]      MrcData         - Pointer to global MRC data.
  @param[in]      Controller      - Controller in use
  @param[in]      Channel         - Channel in use
  @param[in]      Rank            - Rank in use
  @param[in]      OptParam        - The Dimm Opt Param
  @param[in]      IsDFEEnabled    - TRUE if DFE should be enabled

  @retval MrcStatus - mrcSuccess if a MRW was successful
**/
MrcStatus
Ddr5EnableDFE (
  IN  MrcParameters* const  MrcData,
  IN  UINT8   Controller,
  IN  UINT8   Channel,
  IN  UINT8   Rank,
  IN  UINT8   OptParam,
  IN  BOOLEAN IsDFEEnabled
  )
{
  MrcOutput      *Outputs;
  MrcChannelOut  *ChannelOut;
  MrcStatus      Status;
  UINT8          DdrRank;
  UINT8          DFEEnabled;
  DDR5_MODE_REGISTER_111_TYPE   *MR111;

  Outputs     = &MrcData->Outputs;
  ChannelOut  = &Outputs->Controller[Controller].Channel[Channel];
  DdrRank = Rank / MAX_RANK_IN_DIMM;
  MR111 = (DDR5_MODE_REGISTER_111_TYPE *) &ChannelOut->Dimm[DdrRank].Rank[(Rank % MAX_RANK_IN_DIMM)].MR[mrIndexMR111];
  DFEEnabled = IsDFEEnabled ? 0 : 1;
  Status = mrcSuccess;

  // If exisiting tap is already set to DFEEnabled, than return to save MRW
  switch (OptParam) {
    case OptDdr5DimmDFETap1:
      if (MR111->Bits.GlobalDfeTap1Enable != DFEEnabled) {
        MR111->Bits.GlobalDfeTap1Enable = DFEEnabled;
      } else {
        return Status;
      }
      break;
    case OptDdr5DimmDFETap2:
      if (MR111->Bits.GlobalDfeTap2Enable != DFEEnabled) {
        MR111->Bits.GlobalDfeTap2Enable = DFEEnabled;
      } else {
        return Status;
      }
      break;
    case OptDdr5DimmDFETap3:
      if (MR111->Bits.GlobalDfeTap3Enable != DFEEnabled) {
        MR111->Bits.GlobalDfeTap3Enable = DFEEnabled;
      } else {
        return Status;
      }
      break;
    case OptDdr5DimmDFETap4:
    default:
      if (MR111->Bits.GlobalDfeTap4Enable != DFEEnabled) {
        MR111->Bits.GlobalDfeTap4Enable = DFEEnabled;
      } else {
        return Status;
      }
      break;
  }
  Status = MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR111, MR111->Data8, MRC_PRINTS_ON);

  return Status;
}

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
  )
{
  MrcDebug      *Debug;
  MrcOutput     *Outputs;
  MrcChannelOut *ChannelOut;
  MrcRankOut    *RankOut;
  UINT8         *MrPtr;
  UINT8         Rank;
  UINT8         RankIdx;
  UINT8         MrIndex;
  UINT8         Device;
  INT32         CurrentOffset;
  UINT8         VrefCode;
  MrcDimmOut    *DimmOut;
  UINT8         NumDevices;
  UINT8         DimmIdx;
  BOOLEAN       ProgramPda;

  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;
  ChannelOut = &Outputs->Controller[Controller].Channel[Channel];

  switch (VrefType) {
  case CmdV:
    MrIndex = mrIndexMR11;
    ProgramPda = (PdaMode && ChannelOut->IsMr11PdaEnabled);
    break;
  case WrV:
    MrIndex = mrIndexMR10;
    ProgramPda = (PdaMode && ChannelOut->IsMr10PdaEnabled);
    break;
  default:
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s %u\n", gErrString, gWrongInputParam, VrefType);
    return mrcWrongInputParameter;
    break;
  }

  DeviceMask = (ProgramPda) ? DeviceMask : 1;

  //MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Ddr5SetDramVref: MC%u C%u RankMask 0x%X DeviceMask 0x%X -> Offset: %d PdaMode: %u UpdateMrcData: %u IsCachedOffset: %u\n",
  //  Controller, Channel, RankMask, DeviceMask, Offset, PdaMode, UpdateMrcData, IsCachedOffset);

  for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
    if ((MrcRankExist(MrcData, Controller, Channel, Rank)) && (RankMask & (1 << Rank))) {
      DimmIdx = RANK_TO_DIMM_NUMBER (Rank);
      RankIdx = Rank % MAX_RANK_IN_DIMM;
      DimmOut = &ChannelOut->Dimm[DimmIdx];
      RankOut = &DimmOut->Rank[RankIdx];
      MrPtr   = RankOut->MR;

      // Channel Width (ddr5 = 32 Bytes) / SdramWidth (x8 or x16)
      if (ProgramPda) {
        NumDevices = 32 / DimmOut->SdramWidth;
        if (DimmOut->EccSupport) {
          NumDevices += 1;
        }
      } else {
        NumDevices = 1;
      }
      for (Device = 0; Device < NumDevices; Device++) {
        if (DeviceMask & (1 << Device)) {
          if (IsCachedOffset) {
            // Offset value -  calculate the new Vref
            if (ProgramPda) {
              if (VrefType == CmdV) {
                VrefCode = RankOut->DdrPdaVrefCmd[Device];
              } else {
                VrefCode = RankOut->DdrPdaVrefDq[Device];
              }
            } else {
              VrefCode = MrPtr[MrIndex];
            }
            CurrentOffset = MrcVrefToOffsetDdr5 (VrefCode, VrefType);
            CurrentOffset += Offset;
          } else {
            // If !IsCachedOffset
            CurrentOffset = Offset;
          }
          MrcSetTxVrefDdr5 (MrcData, Controller, Channel, Rank, Device, VrefType, CurrentOffset, UpdateMrcData, ProgramPda);
        }
      }
      // Apply CA Vref after it is programmed above
      if (VrefType == CmdV) {
        MrcIssueMpc (MrcData, Controller, Channel, Rank, DDR5_MPC_APPLY_VREF_RTT, MRC_PRINTS_OFF);
      }
      if (ProgramPda) {
        // Send PDA command of Index 15 to resume normal rank operation mode for MR/CAVREF/MPC
        MrcPdaSelect (MrcData, Controller, Channel, Rank, 15, MRC_PRINTS_OFF);
      }
    }
  }
  return mrcSuccess;
}


 /**
   Set Tx Dimm Vref absolute Value for DDR5.
   Use custom MRC range [-52:+53] :: (45% - 97.5%) * 1.1V where offset zero is 71.0%

   @param[in, out] MrcData       - Include all MRC global data.
   @param[in]      Controller    - Memory Controller Number within the processor (0-based).
   @param[in]      Channel       - Selecting which Channel to talk to
   @param[in]      Rank          - Selecting which Rank to talk to
   @param[in]      Device        - Selecting which Device to talk to (only valid when PDAmode is TRUE)
   @param[in]      VrefType       - Determines the Vref type to change, only CmdV and TxVref are valid.
   @param[in]      TxVrefOffset  - TxVref Offset to set
   @param[in]      UpdateMrcData - update MRC host struct
   @param[in]      PdaMode       - Selecting to use MPC or old way of MRS
 **/
VOID
MrcSetTxVrefDdr5 (
  IN OUT MrcParameters *const MrcData,
  IN     UINT32               Controller,
  IN     UINT8                Channel,
  IN     UINT8                Rank,
  IN     UINT8                Device,
  IN     UINT8                VrefType,
  IN     INT32                TxVrefOffset,
  IN     BOOLEAN              UpdateMrcData,
  IN     BOOLEAN              PdaMode
  )
{
   //MrcDebug                *Debug;
   MrcChannelOut           *ChannelOut;
   MrcOutput               *Outputs;
   MrcRankOut              *RankOut;
   UINT8                   Vref;
   UINT8                   ByteLoop;
   UINT8                   MrAddress;
   UINT32                  Delay;
   BOOLEAN                 ProgramPda;

   Outputs = &MrcData->Outputs;
   //Debug = &Outputs->Debug;
   ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
   RankOut    = &ChannelOut->Dimm[RANK_TO_DIMM_NUMBER (Rank) % MAX_DIMMS_IN_CHANNEL].Rank[Rank % MAX_RANK_IN_DIMM];
   MrAddress  = 0;
   ProgramPda = FALSE;

   switch (VrefType) {
   case CmdV:
     MrAddress = mrMR11;
     ProgramPda = (PdaMode && ChannelOut->IsMr11PdaEnabled);
     break;
   case WrV:
     MrAddress = mrMR10;
     ProgramPda = (PdaMode && ChannelOut->IsMr10PdaEnabled);
     break;
   default:
     break;
   }

   Vref = MrcOffsetToVrefDdr5 (MrcData, WrV, TxVrefOffset);
   //MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Vref: %u\n", Vref);

   if (ProgramPda) {
     //MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "MrcSetTxVrefDdr5: PDA mode, Device %d\n", Device);
     MrcPdaSelect (MrcData, Controller, Channel, Rank, Device, MRC_PRINTS_OFF);
   }
   if (VrefType == CmdV) {
     MrcIssueVrefCmd (MrcData, Controller, Channel, Rank, DDR5_VREFCA (Vref), MRC_PRINTS_OFF);
   } else {
     // VrefType TxVref
     Delay = (tVREF_DQ_PS * MRC_TIMER_1NS) / 1000;
     MrcIssueMrw (MrcData, Controller, Channel, Rank, MrAddress, Vref, MRC_PRINTS_OFF);
     MrcWait (MrcData, Delay);
   }

    if (UpdateMrcData) {
      if (ProgramPda) {
        if (VrefType == CmdV) {
          RankOut->DdrPdaVrefCmd[Device] = Vref;
        } else {
          RankOut->DdrPdaVrefDq[Device] = Vref;
        }
      } else {
        if (VrefType == CmdV) {
          RankOut->MR[mrIndexMR11] = Vref;
        } else {
          RankOut->MR[mrIndexMR10] = Vref;
        }
        // Update the per byte Vref in case needed later
        for (ByteLoop = 0; ByteLoop < Outputs->SdramCount; ByteLoop++) {
          if (VrefType == CmdV) {
            RankOut->DdrPdaVrefCmd[ByteLoop] = Vref;
          } else {
            RankOut->DdrPdaVrefDq[ByteLoop] = Vref;
          }
        }
      }
    }
  }

/**
  Offset to DDR5 Vref (DQ/CS/CA) Range/Value (MR10)
  Offset [-52:+53] - Vref 0 - 105 :: (45% - 97.5%) * 1.1V
  @param[in]  MrcData  - Include all MRC global data.
  @param[in]  VrefType - CmdV or WrV
  @param[in]  Offset   - Value to be converted to actual Vref (DQ/CS/CA) Range/Value.

  @retval DDR5 Vref (DQ/CS/CA) Range/Value
**/
UINT8
MrcOffsetToVrefDdr5 (
  IN MrcParameters *const MrcData,
  IN MrcMarginTypes       VrefType,
  IN INT32                Offset
)
{
  INT32 Vref;
  INT32 MinOffset;
  INT32 MaxOffset;
  INT32 MinEncoding;
  INT32 MaxEncoding;

  if (VrefType == CmdV) {
    MinOffset = DDR5_CMD_VREF_OFFSET_MIN;
    MaxOffset = DDR5_CMD_VREF_OFFSET_MAX;
    MinEncoding = Ddr5Vref_47p5;
    MaxEncoding = Ddr5Vref_95p0;
  } else {
    MinOffset = DDR5_WRV_VREF_OFFSET_MIN;
    MaxOffset = DDR5_WRV_VREF_OFFSET_MAX;
    MinEncoding = Ddr5Vref_45p0;
    MaxEncoding = Ddr5Vref_97p5;
  }

  if ((Offset < MinOffset) || (Offset > MaxOffset)) {
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "%s Invalid Vref Setting %d is out of range [%d:%d]. Clamping\n",
      gErrString, Offset, MinOffset, MaxOffset);
  }

  if (Offset <= MinOffset) {
    Vref = MinEncoding;
  } else if (Offset > MaxOffset) {
    Vref = MaxEncoding;
  } else {
    Vref = DDR5_DQ_CMD_VREF_MID_ENC - Offset;
  }

  return (UINT8) Vref;
}

/**
  DDR5 Vref (DQ/CS/CA) Range/Value (MR10) to Offset
  Vref Range 0 - 105 - Offset [-52:+53] :: (45% - 97.5%) * 1.1V where offset zero is 71.0%

  @param[in]  Vref   - Vref (DQ/CS/CA) Range/Value to be converted back to corresponding Offset value.

  @retval Offset
**/
INT32
MrcVrefToOffsetDdr5 (
  IN UINT8           Vref,
  IN MrcMarginTypes  VrefType
  )
{
  INT32     Offset;
  INT32 MinOffset;
  INT32 MaxOffset;
  INT32 MinEncoding;
  INT32 MaxEncoding;

  if (VrefType == CmdV) {
    MinOffset = DDR5_CMD_VREF_OFFSET_MIN;
    MaxOffset = DDR5_CMD_VREF_OFFSET_MAX + 1;
    MinEncoding = Ddr5Vref_47p5;
    MaxEncoding = Ddr5Vref_95p0;
  } else {
    MinOffset = DDR5_WRV_VREF_OFFSET_MIN;
    MaxOffset = DDR5_WRV_VREF_OFFSET_MAX;
    MinEncoding = Ddr5Vref_45p0;
    MaxEncoding = Ddr5Vref_97p5;
  }
  if (Vref >= MinEncoding) {
    Offset = MinOffset;
  } else if (Vref < MaxEncoding) {
    Offset = MaxOffset;
  } else {
    Offset = DDR5_DQ_CMD_VREF_MID_ENC - Vref;
  }

  return Offset;
}

/**
  This function converts from Ohms to MR ODT encoding.

  @param[in]  OdtValue  - ODT Value in Ohms.

  @retval INT8 - Encoding if valid ODT value.  Else, -1.
**/
DDR5_MR32_CKCSCA_ODT
CccOdtEncode_DDR5 (
  IN  UINT32  OdtValue
  )
{
  DDR5_MR32_CKCSCA_ODT  EncodeVal;

  switch (OdtValue) {
    case 0xFFFF:
    case 0:
      EncodeVal = CkCsCaOdt_RTT_OFF;
      break;

    case 480:
      EncodeVal = CkCsCaOdt_RZQ_0p5_480;
      break;

    case 240:
      EncodeVal = CkCsCaOdt_RZQ_1_240;
      break;

    case 120:
      EncodeVal = CkCsCaOdt_RZQ_2_120;
      break;

    case 80:
      EncodeVal = CkCsCaOdt_RZQ_3_80;
      break;

    case 60:
      EncodeVal = CkCsCaOdt_RZQ_4_60;
      break;

    case 40:
      EncodeVal = CkCsCaOdt_RZQ_6_40;
      break;

    default:
      EncodeVal = -1;
      break;
  }

  return EncodeVal;
}

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
  )
{

  UINT32                      RankMod2;
  UINT32                      Rank;
  MrcStatus                   Status;
  MrcChannelOut               *ChannelOut;
  MrcOutput                   *Outputs;
  UINT8                       DevsPerRank[MAX_RANK_IN_CHANNEL];
  UINT32                      DimmIdx;
  MrcDimmOut                  *DimmOut;
  MrcRankOut                  *RankOut;
  UINT32                      DeviceIdx;
  BOOLEAN                     IsAnyRankx8;

  Outputs       = &MrcData->Outputs;
  Status        = mrcSuccess;
  ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
  *NumMrData = 0;

  if ((MrAddress != mrMR3) && (MrAddress != mrMR7) && (MrAddress != mrMR10) && (MrAddress != mrMR11) && (MrAddress != mrMR48) &&
    !(Ddr5IsDimmDfeMr(MrAddress))) {
    MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_ERROR, "MrFillPdaMrsDataDdr5 does not support  MR %d \n", MrAddress);
    return mrcWrongInputParameter;
  }

  // First see if any rank is populated with x8
  // In this case Generic MRS FSM will use 4 or 5 devices in a device loop, for all ranks, even if some ranks are x16.
  // Only if all ranks are x16 it will use 2 devices per loop.
  IsAnyRankx8 = FALSE;
  for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank+=2) {
    if (!MrcRankExist(MrcData, Controller, Channel, Rank)) {
      continue;
    }
    DimmIdx = RANK_TO_DIMM_NUMBER(Rank);
    DimmOut = &ChannelOut->Dimm[DimmIdx];
    if (DimmOut->SdramWidth == 8) {
      IsAnyRankx8 = TRUE;
    }
  }

   // Fill the dev Rank array
  // some ranks can be x16 some x8
  for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
    DevsPerRank[Rank] = 0;
    if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
      continue;
    }
    DimmIdx = RANK_TO_DIMM_NUMBER(Rank);
    DimmOut = &ChannelOut->Dimm[DimmIdx];
    // Channel Width (ddr5 = 32 Bytes) / SdramWidth (x8 or x16)
    DevsPerRank[Rank] = DimmOut->PrimaryBusWidth / DimmOut->SdramWidth;
    if (IsAnyRankx8) {
      DevsPerRank[Rank] = 4;
    }
    if (DimmOut->EccSupport) {
      DevsPerRank[Rank]++;
    }
  }

  // Get the data for all ranks and devices
  for (DeviceIdx = 0; DeviceIdx < MAX_BYTE_IN_DDR5_CHANNEL; DeviceIdx++) {
    for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
      if (!MrcRankExist(MrcData, Controller, Channel, Rank)) {
        continue;
      }
      DimmIdx = RANK_TO_DIMM_NUMBER(Rank);
      RankMod2 = Rank % MAX_RANK_IN_DIMM;
      RankOut = &ChannelOut->Dimm[DimmIdx].Rank[RankMod2];
      if (DeviceIdx < DevsPerRank[Rank]) {
        switch (MrAddress) {
        case mrMR3:
          MrPdaData[*NumMrData] = RankOut->Ddr5PdaMr3[DeviceIdx];
          break;
        case mrMR7:
          MrPdaData[*NumMrData] = RankOut->Ddr5PdaMr7[DeviceIdx];
          break;
        case mrMR10:
          MrPdaData[*NumMrData] = RankOut->DdrPdaVrefDq[DeviceIdx];
          break;
        case mrMR11:
          MrPdaData[*NumMrData] = RankOut->DdrPdaVrefCmd[DeviceIdx];
          break;
        case mrMR48:
          MrPdaData[*NumMrData] = RankOut->Ddr5PdaMr48[DeviceIdx];
          break;
        case mrMR129: // DFE PDA Tap1 DQL
        case mrMR137:
        case mrMR145:
        case mrMR153:
        case mrMR161:
        case mrMR169:
        case mrMR177:
        case mrMR185:
          MrPdaData[*NumMrData] = RankOut->Ddr5PdaMr129[DeviceIdx];
          break;
        case mrMR130: // DFE PDA Tap2 DQL
        case mrMR138:
        case mrMR146:
        case mrMR154:
        case mrMR162:
        case mrMR170:
        case mrMR178:
        case mrMR186:
          MrPdaData[*NumMrData] = RankOut->Ddr5PdaMr130[DeviceIdx];
          break;
        case mrMR131: // DFE PDA Tap3 DQL
        case mrMR139:
        case mrMR147:
        case mrMR155:
        case mrMR163:
        case mrMR171:
        case mrMR179:
        case mrMR187:
          MrPdaData[*NumMrData] = RankOut->Ddr5PdaMr131[DeviceIdx];
          break;
        case mrMR132: // DFE PDA Tap4 DQL
        case mrMR140:
        case mrMR148:
        case mrMR156:
        case mrMR164:
        case mrMR172:
        case mrMR180:
        case mrMR188:
          MrPdaData[*NumMrData] = RankOut->Ddr5PdaMr132[DeviceIdx];
          break;
        case mrMR193: // DFE PDA Tap1 DQU
        case mrMR201:
        case mrMR209:
        case mrMR217:
        case mrMR225:
        case mrMR233:
        case mrMR241:
        case mrMR249:
          MrPdaData[*NumMrData] = RankOut->Ddr5PdaMr193[DeviceIdx];
          break;
        case mrMR194: // DFE PDA Tap2 DQU
        case mrMR202:
        case mrMR210:
        case mrMR218:
        case mrMR226:
        case mrMR234:
        case mrMR242:
        case mrMR250:
          MrPdaData[*NumMrData] = RankOut->Ddr5PdaMr194[DeviceIdx];
          break;
        case mrMR195: // DFE PDA Tap3 DQU
        case mrMR203:
        case mrMR211:
        case mrMR219:
        case mrMR227:
        case mrMR235:
        case mrMR243:
        case mrMR251:
          MrPdaData[*NumMrData] = RankOut->Ddr5PdaMr195[DeviceIdx];
          break;
        case mrMR196: // DFE PDA Tap4 DQU
        case mrMR204:
        case mrMR212:
        case mrMR220:
        case mrMR228:
        case mrMR236:
        case mrMR244:
        case mrMR252:
          MrPdaData[*NumMrData] = RankOut->Ddr5PdaMr196[DeviceIdx];
          break;
        default:
          break;
        }
        (*NumMrData)++;
      }
    } // for Rank
  } // for DeviceIdx

  return Status;
}

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
        MrIndex = mrIndexMR45;
        MrAddr = mrMR45;
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
  Program MR48 (DDR5) with Data Invert Nibble
  MR48 should be programmed if WriteX feature is supported.
  If IsReadPatternSupported is FALSE, then program all 0's to MR48

  @param[in] MrcData                - Include all MRC global data
  @param[in] OdtMode                - Specifies the ODT Mode for DDRIO
  @param[in] IsReadPatternSupported - TRUE is MR48 should be programmed based on OdtMode, FALSE if MR48 should be programmed to 0's

  @retval mcrSuccess successfully programmed MR48
  @retval mrcFail failed to program MR48
**/
MrcStatus
MrcSetDdr5Mr48 (
  IN MrcParameters *const MrcData,
  IN MRC_ODT_MODE_TYPE    OdtMode,
  IN BOOLEAN              IsReadPatternSupported
  )
{
  MrcInput  *Inputs;
  MrcOutput *Outputs;
  MrcChannelOut *ChannelOut;
  MrcRankOut *RankOut;
  UINT32 Controller;
  UINT32 Channel;
  UINT8 Rank;
  UINT8 RankHalf;
  UINT8 Device;
  UINT8 NumDevices;
  UINT8 static const VddqData[MAX_SDRAM_IN_DIMM_NO_ECC] =  {0xFF, 0xFF, 0xFF, 0xFF};
  UINT8 static const VssData[MAX_SDRAM_IN_DIMM_NO_ECC]  =  {0x00, 0x00, 0x00, 0x00};
  UINT8 static const NoData[MAX_SDRAM_IN_DIMM_NO_ECC]   =  {0x00, 0x00, 0x00, 0x00};
  UINT8 const *DataInvertNibbleData;
  MrcStatus Status;
  MrcDimmOut *DimmOut;
  DDR5_MODE_REGISTER_48_TYPE MR48;

  Inputs = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Status = mrcSuccess;

  switch (OdtMode) {
    case MrcOdtModeVss:
      DataInvertNibbleData = VssData;
      break;

    case MrcOdtModeVddq:
      DataInvertNibbleData = VddqData;
      break;

    default:
      MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_ERROR, "Invalid ODT mode for DataInvertNibble: %u\n", Outputs->OdtMode);
      return mrcFail;
  }
  // if feature is not supported, program 0's to MR to clear anything that may be programmed in the MR before
  if (!IsReadPatternSupported) {
    DataInvertNibbleData = NoData;
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
          continue;
        }
        RankHalf = Rank / MAX_RANK_IN_DIMM;
        ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
        DimmOut    = &ChannelOut->Dimm[RankHalf];
        RankOut    = &DimmOut->Rank[RankHalf];
        // Channel Width (ddr5 = 32 Bytes) / SdramWidth (x8 or x16)
        // DDR5 ECC does not support WriteX feature so do not need account for ECC Device
        NumDevices = DimmOut->PrimaryBusWidth / DimmOut->SdramWidth;
        if (Inputs->EnablePda) {
          ChannelOut->IsMr48PdaEnabled = TRUE;
          for (Device = 0; Device < NumDevices; Device++) {
            MR48.Bits.WritePatternMode = DataInvertNibbleData[Device];
            RankOut->Ddr5PdaMr48[Device] = MR48.Bits.WritePatternMode;
            MrcPdaSelect (MrcData, Controller, Channel, Rank, Device, MRC_PRINTS_OFF);
            if (MrcIssueMrw(MrcData, Controller, Channel, Rank, mrMR48, MR48.Data8, FALSE) != mrcSuccess) {
              Status = mrcFail;
            }
          }
          // Send PDA command of Index 15 to resume normal rank operation mode for MR/CAVREF/MRC
          MrcPdaSelect (MrcData, Controller, Channel, Rank, 15, MRC_PRINTS_OFF);
        } else {
          // No PDA, use byte0 value for all devices
          MR48.Bits.WritePatternMode = DataInvertNibbleData[0];
          if (MrcIssueMrw(MrcData, Controller, Channel, Rank, mrMR48, MR48.Data8, FALSE) != mrcSuccess) {
            Status = mrcFail;
          }
        }
      }
    }
  }
  return Status;
}

/**
  Update the Read Preamble type in MR8 and send this to DRAM.
  Update MR8 in the host struct as well

  @param[in]  MrcData -  Pointer to MRC global data.

  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcSetDdr5Mr8ReadPreamble (
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
  DDR5_MR8_tRPRE        RpreVal;
  DDR5_MODE_REGISTER_8_TYPE  Mr8;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  Status  = mrcSuccess;
  Mr8.Data8 = 0;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
          continue;
        }
        ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
        MrPtr = ChannelOut->Dimm[Rank / MAX_RANK_IN_DIMM].Rank[Rank % MAX_RANK_IN_DIMM].MR;
        Mr8.Data8 = MrPtr[mrIndexMR8];
        if (MrcDdr5GetReadPreambleSetting (MrcData, &RpreVal) != mrcSuccess) {
          Status = mrcWrongInputParameter;
        }
        Mr8.Bits.ReadPreambleSettings = RpreVal;
        MrPtr[mrIndexMR8] = Mr8.Data8;
      }
    }
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "New MR8: 0x%x\n", Mr8.Data8);

  // Program the new MR8 value on all populated ranks
  MrcIssueMrwMulticast (MrcData, Outputs->ValidRankMask, mrMR8, Mr8.Data8, MRC_PRINTS_OFF);

  return Status;
}

/**
  This function converts from Ohms to DDR5 MR ODT encoding.

  @param[in]  OdtValue  - ODT Value in Ohms.

  @retval INT8 - Encoding if valid ODT value.  Else, -1.
**/

DDR5_MR33_RTT_PARK
Ddr5OdtEncode (
  IN  UINT16  OdtValue
  )
{
  DDR5_MR33_RTT_PARK  EncodeVal;

  switch (OdtValue) {
    case 0xFFFF:
    case 0:
      EncodeVal = Rtt_RTT_OFF;
      break;

    case 240:
      EncodeVal = Rtt_RZQ_240;
      break;

    case 120:
      EncodeVal = Rtt_RZQ_2_120;
      break;

    case 80:
      EncodeVal = Rtt_RZQ_3_80;
      break;

    case 60:
      EncodeVal = Rtt_RZQ_4_60;
      break;

    case 48:
      EncodeVal = Rtt_RZQ_5_48;
      break;

    case 40:
      EncodeVal = Rtt_RZQ_6_40;
      break;

    case 34:
      EncodeVal = Rtt_RZQ_7_34;
      break;

    default:
      EncodeVal = -1;
      break;
  }

  return EncodeVal;
}

/**
  This function converts from Odt Offset to DDR5 MR OdtlOn Wr encoding.

  @param[in]  OdttOnWrOffset - Offset to push/pull out/in the WriteRTT or Non-Target Write RTT enable time

  @retval INT8 - Encoding if valid ODT On Wr Offset.  Else, -1.
**/
DDR5_MR37_ODTLON_WR_OFFSET
Ddr5OdttOnWrOffset (
  IN  INT16  OdttOnWrOffset
  )
{
  DDR5_MR37_ODTLON_WR_OFFSET  EncodeVal;

  switch (OdttOnWrOffset) {
    case -4:
      EncodeVal = OdtlOnWrOffsetMinus4;
      break;

    case -3:
      EncodeVal = OdtlOnWrOffsetMinus3;
      break;

    case -2:
      EncodeVal = OdtlOnWrOffsetMinus2;
      break;

    case -1:
      EncodeVal = OdtlOnWrOffsetMinus1;
      break;

    case 0:
      EncodeVal = OdtlOnWrOffset0;
      break;

    case 1:
      EncodeVal = OdtlOnWrOffsetPlus1;
      break;

    case 2:
      EncodeVal = OdtlOnWrOffsetPlus2;
      break;

    default:
      EncodeVal = -1;
      break;
  }

  return EncodeVal;
}

/**
  This function converts from Odt Offset to DDR5 MR OdtlOff Wr encoding.

  @param[in] OdttOffWrOffset - Offset to push/pull out/in the WriteRTT or Non-Target Write RTT disable time

  @retval INT8 - Encoding if valid ODT Off Wr Offset.  Else, -1.
**/
INT8
Ddr5OdttOffWrOffset (
  IN  INT16  OdttOffWrOffset
  )
{
  DDR5_MR37_ODTLOFF_WR_OFFSET  EncodeVal;

  switch (OdttOffWrOffset) {
    case 4:
      EncodeVal = OdtlOffWrOffsetPlus4;
      break;

    case 3:
      EncodeVal = OdtlOffWrOffsetPlus3;
      break;

    case 2:
      EncodeVal = OdtlOffWrOffsetPlus2;
      break;

    case 1:
      EncodeVal = OdtlOffWrOffsetPlus1;
      break;

    case 0:
      EncodeVal = OdtlOffWrOffset0;
      break;

    case -1:
      EncodeVal = OdtlOffWrOffsetMinus1;
      break;

    case -2:
      EncodeVal = OdtlOffWrOffsetMinus2;
      break;

    default:
      EncodeVal = -1;
      break;
  }

  return EncodeVal;
}

/**
  This function converts from Odt Offset to DDR5 MR OdtlOn Rd NT encoding.

  @param[in]  OdttOnRdNtOffset - Offset to push/pull out/in the Non-Target Read RTT enable time

  @retval INT8 - Encoding if valid ODT On Rd NT Offset.  Else, -1.
**/
INT8
Ddr5OdttOnRdNtOffset (
  IN  INT16  OdttOnRdNtOffset
  )
{
  DDR5_MR39_ODTLON_RD_NT_OFFSET  EncodeVal;

  switch (OdttOnRdNtOffset) {
    case -3:
      EncodeVal = OdtlOnRdNtOffsetMinus3;
      break;

    case -2:
      EncodeVal = OdtlOnRdNtOffsetMinus2;
      break;

    case -1:
      EncodeVal = OdtlOnRdNtOffsetMinus1;
      break;

    case 0:
      EncodeVal = OdtlOnRdNtOffset0;
      break;

    case 1:
      EncodeVal = OdtlOnRdNtOffsetPlus1;
      break;

    default:
      EncodeVal = -1;
      break;
  }

  return EncodeVal;
}

/**
  This function converts from Odt Offset to DDR5 MR OdtlOff Rd NT encoding.

  @param[in]  OdttOffRdNtOffset  - Offset to push/pull out/in the Non-Target Read RTT disable time

  @retval INT8 - Encoding if valid ODT Off Rd NT Offset.  Else, -1.
**/
INT8
Ddr5OdttOffRdNtOffset (
  IN  INT32  OdttOffRdNtOffset
  )
{
  DDR5_MR39_ODTLOFF_RD_NT_OFFSET  EncodeVal;

  switch (OdttOffRdNtOffset) {
    case 3:
      EncodeVal = OdtlOffRdNtOffsetPlus3;
      break;

    case 2:
      EncodeVal = OdtlOffRdNtOffsetPlus2;
      break;

    case 1:
      EncodeVal = OdtlOffRdNtOffsetPlus1;
      break;

    case 0:
      EncodeVal = OdtlOffRdNtOffset0;
      break;

    case -1:
      EncodeVal = OdtlOffRdNtOffsetMinus1;
      break;

    default:
      EncodeVal = -1;
      break;
  }

  return EncodeVal;
}

/**
  This function converts from DFE Tap Value to DDR5 MR encoding.

  @param[in]  DfeValue  - DFE Value

  @retval - Encoding if valid DFE value.  Else, 0.
**/
DDR5_MODE_REGISTER_129_TYPE
Ddr5DfeEncode (
  IN  INT16  DfeValue
  )
{
  DDR5_MODE_REGISTER_129_TYPE  EncodeVal;

  EncodeVal.Data8 = 0;

  EncodeVal.Bits.DfeTap1Bias = (UINT8) (ABS (DfeValue));
  if (EncodeVal.Bits.DfeTap1Bias != ABS (DfeValue) || (DfeValue == 0)) {
    //Exceeds field width or DfeValue is 0 which is to disable
    EncodeVal.Data8 = 0;
  } else {
    EncodeVal.Bits.DfeTap1Enable = 1;
    if (DfeValue < 0) {
      EncodeVal.Bits.DfeTap1BiasSignBit = 1;
    }
  }

  return EncodeVal;
}

/**
  This function returns the DFE Setting for MR111.

  @param[in]  MrcData   - Pointer to global MRC data.
  @param[out] DfeGain   - DFE Gain Enable.
  @param[out] DfeTap1   - DFE Tap1 Enable.
  @param[out] DfeTap2   - DFE Tap2 Enable.
  @param[out] DfeTap3   - DFE Tap3 Enable.
  @param[out] DfeTap4   - DFE Tap4 Enable.

  @retval MrcStatus - mrcSuccess
**/
MrcStatus
MrcDdr5GetDFE (
  OUT UINT8          *DfeGain,
  OUT UINT8          *DfeTap1,
  OUT UINT8          *DfeTap2,
  OUT UINT8          *DfeTap3,
  OUT UINT8          *DfeTap4
  )
{
  if (DfeGain != NULL) {
    *DfeGain = 0;
  }
  if (DfeTap1 != NULL) {
    *DfeTap1 = 0;
  }
  if (DfeTap2 != NULL) {
    *DfeTap2 = 0;
  }
  if (DfeTap3 != NULL) {
    *DfeTap3 = 0;
  }
  if (DfeTap4 != NULL) {
    *DfeTap4 = 0;
  }

  return mrcSuccess;
}

/**
  This function returns the value of tXSR in tCK.

  @param[in] tRFC  - Jedec timing tRFC

  @retval UINT32 - tXSR in tCK.
**/
UINT32
MrcGetDdr5Txsr (
  IN  UINT16  tRFC
  )
{
  UINT32  tXSR;

  // In nCK; DDR5 uses tRFC1 during Self-Refresh,
  // even when configured to Fine Granularity Refresh mode
  tXSR = tRFC;

  return tXSR;
}

/**
  This function returns the tMOD value.
  For DDR5 this is for "tRMD"

  @param[in] tCK       - DCLK period in femtoseconds.

  @retval UINT32 - Timing in tCK.
**/
UINT32
MrcGetDdr5Tmod (
  IN const UINT32  tCK
  )
{
  UINT32  tMOD;

  // DDR5 "tMRD"
  // max(14ns, 16nCK)
  tMOD = DIVIDECEIL (tMOD_DDR5_FS, tCK);
  tMOD = MAX (tMOD, 16);

  return tMOD;
}

/**
  This function returns the tMRW value.
  @param[in] tCK  - DCLK period in femtoseconds.
  @retval UINT32  - Timing in tCK.
**/
UINT32
MrcGetDdr5tMRW (
  IN const  UINT32  tCK
  )
{
  UINT32 tMRW;

 // max(5ns, 8nCK)
 tMRW = DIVIDECEIL (tMRW_DDR5_FS, tCK);
 tMRW = MAX (tMRW, 8);
 return tMRW;
}

/**
  This function returns the tContExitDelay value (time from Continuous Burst Mode Exit to next valid command).

  @param[in]  MrcData  - Include all MRC global data.

  @retval UINT32 - Timing in tCK.
**/
UINT32
MrcGetDdr5tContExitDelay (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput  *Outputs;
  MrcInput   *Inputs;
  MrcTiming  *TimingProfile;
  UINT8      FirstController;
  UINT8      FirstChannel;
  UINT16     tCL;
  UINT32     tMRW;
  UINT32     tContExitDelay;

  Outputs = &MrcData->Outputs;
  Inputs  = &MrcData->Inputs;
  FirstController = Outputs->FirstPopController;
  FirstChannel    = Outputs->Controller[FirstController].FirstPopCh;
  TimingProfile   = &Outputs->Controller[FirstController].Channel[FirstChannel].Timing[Inputs->ExtInputs.Ptr->MemoryProfile];
  tCL  = TimingProfile->tCL;
  tMRW = MrcGetDdr5tMRW (Outputs->MemoryClock);

  // tCont_Exit_Delay = tCont_Exit + tMRW = RL + BL/2 + 10nCK + tMRW
  tContExitDelay = tCL + Outputs->BurstLength + tMRW + 10;

  return tContExitDelay;
}

/**
  This function returns the DDR5 tZQCAL value in DCLKs.

  @param[in] tCK  - DCLK period in femtoseconds.

  @retval tZQCAL  - Timing in tCK.
**/
UINT32
MrcGetDdr5Tzqcal (
  IN UINT32               tCK
  )
{
  return DIVIDECEIL (tZQCAL_FS_DDR5, tCK);
}

/**
  This function returns the DDR5 tZQLAT value in DCLKs.

  @param[in] tCK  - DCLK period in femtoseconds.

  @retval tZQLAT  - Timing in tCK.
**/
UINT32
MrcGetDdr5Tzqlat (
  IN UINT32               tCK
  )
{
  UINT32   Zqlat;

  //tZQLAT = max (30ns, 8nCK)
  Zqlat = DIVIDECEIL (tZQLAT_FS_DDR5, tCK);
  Zqlat = MAX (Zqlat, tZQLAT_DDR5_CK_MIN);

  return Zqlat;
}

/**
  This function returns the DDR5 tZQCS value.

  @param[in] tZQLAT  - tZQLAT in femtoseconds over tCK.

  @retval tZQCS  - Value in tCK.
**/
UINT32
MrcGetDdr5Tzqcs (
  IN UINT32               tZQLAT
  )
{
  return MAX (tZQLAT, tZQLAT_DDR5_CK_MIN);;
}

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
  )
{
  static const UINT16    Ddr5DimmRonVals[] = { 48, 40, 34 }; // DDR5 Pull Down Drive Strength in Ohms
  static const UINT16    Ddr5DimmRttCccVals[] = { 0, 480, 240, 120, 80, 60, 40 }; // DDR5 Rtt for CCC (CK, CS, CA) Values in Ohms
  static const UINT16    Ddr5DimmRttVals[] = { 0, 240, 120, 80, 60, 48, 40, 34 }; // DDR5 Rtt for DQSPark, Park, Wr, Nom Values in Ohms
  MrcDebug               *Debug;
  MrcStatus              Status;

  Debug = &MrcData->Outputs.Debug;
  Status = mrcSuccess;

  switch (DimmOptParam) {
    case OptDimmOdtCA:
    case OptDimmOdtCccGroupA:
    case OptDimmOdtCccGroupB:
    case OptDimmOdtCAGroupA:
    case OptDimmOdtCAGroupB:
    case OptDimmOdtCSGroupA:
    case OptDimmOdtCSGroupB:
      // DIMM CA_ODT
      *DimmOptParamVals = (UINT16 *)Ddr5DimmRttCccVals;
      *NumDimmOptParamVals = ARRAY_COUNT(Ddr5DimmRttCccVals);
      break;

    case OptDimmRonUp:
    case OptDimmRonDn:
      // DIMM RON
      *DimmOptParamVals = (UINT16 *)Ddr5DimmRonVals;
      *NumDimmOptParamVals = ARRAY_COUNT(Ddr5DimmRonVals);
      break;

    case OptDimmOdtWr:
    case OptDimmOdtNom:
    case OptDimmOdtNomWr:
    case OptDimmOdtNomRd:
    case OptDimmOdtPark:
    case OptDimmOdtParkDqs:
      // DIMM WR_ODT/NOM_ODT/PARK_ODT
      *DimmOptParamVals = (UINT16 *)Ddr5DimmRttVals;
      *NumDimmOptParamVals = ARRAY_COUNT(Ddr5DimmRttVals);
      break;

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
  )
{
  MrcDebug          *Debug;
  MrcStatus         Status;
  UINT16            LocalMrNum;
  UINT8             LocalMrIndex;

  Status = mrcSuccess;
  Debug = &MrcData->Outputs.Debug;

  switch (OptDimmParam) {
    case OptDimmOdtCaStrap:
      // DIMM CA_ODT Strap
      LocalMrIndex = 0xFF;
      LocalMrNum = mrMR32;
      break;

    case OptDimmOdtCAGroupA:
      // DIMM CA_ODT Group A
      LocalMrIndex = mrIndexMpcMr33a0;
      LocalMrNum = mpcMR33a0;
      break;

    case OptDimmOdtCAGroupB:
      // DIMM CA_ODT Group B
      LocalMrIndex = mrIndexMpcMr33b0;
      LocalMrNum = mpcMR33b0;
      break;

    case OptDimmOdtCSGroupA:
      // DIMM CS_ODT Group A
      LocalMrIndex = mrIndexMpcMr32a1;
      LocalMrNum = mpcMR32a1;
      break;

    case OptDimmOdtCSGroupB:
      // DIMM CS_ODT Group B
      LocalMrIndex = mrIndexMpcMr32b1;
      LocalMrNum = mpcMR32b1;
      break;

    case OptDimmRonUp:
    case OptDimmRonDn:
      // DIMM RON
      LocalMrIndex = mrIndexMR5;
      LocalMrNum = mrMR5;
      break;

    case OptDimmOdtWr:
      // DIMM WR_ODT
      LocalMrIndex = mrIndexMR34;
      LocalMrNum = mrMR34;
      break;

    case OptDimmOdtNom:
    case OptDimmOdtNomWr:
    case OptDimmOdtNomRd:
      // DIMM NOM_ODT
      LocalMrIndex = mrIndexMR35;
      LocalMrNum = mrMR35;
      break;

    case OptDimmOdtPark:
      // DIMM PARK_ODT
      LocalMrIndex = mrIndexMpcMr34;
      LocalMrNum = mpcMR34;
      break;

    case OptDimmOdtParkDqs:
      // DIMM DQS_PARK_ODT
      LocalMrIndex = mrIndexMpcMr33;
      LocalMrNum = mpcMR33;
      break;

    case Ddr5OdtlOnWr:
    case Ddr5OdtlOffWr:
      // ODTL WR Offset
      LocalMrIndex = mrIndexMR37;
      LocalMrNum = mrMR37;
      break;

    case Ddr5OdtlOnWrNT:
    case Ddr5OdtlOffWrNT:
      // ODTL WR NT Offset
      LocalMrIndex = mrIndexMR38;
      LocalMrNum = mrMR38;
      break;

    case Ddr5OdtlOnRdNT:
    case Ddr5OdtlOffRdNT:
      // ODTL RD NT Offset
      LocalMrIndex = mrIndexMR39;
      LocalMrNum = mrMR39;
      break;

    case OptDdr5DimmDFETap1:
      LocalMrIndex = mrIndexMR129;
      LocalMrNum   = mrMR129;
      break;

    case OptDdr5DimmDFETap2:
      LocalMrIndex = mrIndexMR130;
      LocalMrNum   = mrMR130;
      break;

    case OptDdr5DimmDFETap3:
      LocalMrIndex = mrIndexMR131;
      LocalMrNum   = mrMR131;
      break;

    case OptDdr5DimmDFETap4:
      LocalMrIndex = mrIndexMR132;
      LocalMrNum   = mrMR132;
      break;

    default:
      LocalMrIndex = 0xFF;
      LocalMrNum = 0xFFFF;
      Status = mrcWrongInputParameter;
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %u ", gWrongInputParam, OptDimmParam);
      break;
  }
  if ((LocalMrIndex == 0xFF) && (MrIndex != NULL)) {
    Status = mrcWrongInputParameter;
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %u w/ MrIndex expected to be NULL!", gWrongInputParam, OptDimmParam);
  }

  if (MrIndex != NULL) {
    *MrIndex = LocalMrIndex;
  }
  if (MrNum != NULL) {
    *MrNum = LocalMrNum;
  }

  return Status;
}

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
  )
{
  MrcDebug        *Debug;
  MrcStatus       Status;

  Debug = &MrcData->Outputs.Debug;

  switch (OptParam) {

    case OptDimmOdtCAGroupA:
    case OptDimmOdtCAGroupB:
    case OptDimmOdtCSGroupA:
    case OptDimmOdtCSGroupB:
      Status = MrcDdr5SetCccOdt (MrcData, OptParam, ParamValue, MrData);
      break;

    case OptDimmRonUp:
      // DIMM RON
      Status = MrcDdr5SetMr5 (MrcData, MRC_IGNORE_ARG_8, ParamValue, MRC_IGNORE_ARG_8, MRC_IGNORE_ARG_8, MRC_IGNORE_ARG_16, MrData);
      break;

    case OptDimmRonDn:
      // DIMM RON
      Status = MrcDdr5SetMr5 (MrcData, MRC_IGNORE_ARG_8, MRC_IGNORE_ARG_16, MRC_IGNORE_ARG_8, MRC_IGNORE_ARG_8, ParamValue, MrData);
      break;

    case OptDimmOdtWr:
      // DIMM WR_ODT
      Status = MrcDdr5SetMr34 (MrcData, ParamValue, MrData);
      break;

    case OptDimmOdtNom:
      // DIMM NOM_ODT
      Status = MrcDdr5SetMr35 (MrcData, ParamValue, ParamValue, MrData);
      break;

    case OptDimmOdtNomRd:
      // DIMM NOM_ODT
      Status = MrcDdr5SetMr35 (MrcData, MRC_IGNORE_ARG_16, ParamValue, MrData);
      break;

    case OptDimmOdtNomWr:
      // DIMM NOM_ODT
      Status = MrcDdr5SetMr35 (MrcData, ParamValue, MRC_IGNORE_ARG_16, MrData);
      break;

    case Ddr5OdtlOnWr:
      // ODTL ON WR Offset
      Status = MrcDdr5SetMr37 (MrcData, (INT16) ParamValue, (INT16) MRC_IGNORE_ARG_8, MrData);
      break;

    case Ddr5OdtlOffWr:
      // ODTL OFF WR Offset
      Status = MrcDdr5SetMr37 (MrcData, (INT16) MRC_IGNORE_ARG_8, (INT16) ParamValue, MrData);
      break;

    case Ddr5OdtlOnWrNT:
      // ODTL ON WR NT Offset
      Status = MrcDdr5SetMr38 (MrcData, (INT16) ParamValue, (INT16) MRC_IGNORE_ARG_8, MrData);
      break;

    case Ddr5OdtlOffWrNT:
      // ODTL OFF WR NT Offset
      Status = MrcDdr5SetMr38 (MrcData, (INT16) MRC_IGNORE_ARG_8, (INT16) ParamValue, MrData);
      break;

    case Ddr5OdtlOnRdNT:
      // ODTL ON RD NT Offset
      Status = MrcDdr5SetMr39 (MrcData, (INT16) ParamValue, (INT16) MRC_IGNORE_ARG_8, MrData);
      break;

    case Ddr5OdtlOffRdNT:
      // ODTL OFF WR NT Offset
      Status = MrcDdr5SetMr39 (MrcData, (INT16) MRC_IGNORE_ARG_8, (INT16) ParamValue, MrData);
      break;

    case OptDdr5DimmDFETap1:
    case OptDdr5DimmDFETap2:
    case OptDdr5DimmDFETap3:
    case OptDdr5DimmDFETap4:
      Status = MrcDdr5SetDfe (MrcData, OptParam, (INT16) ParamValue, MrData);
      break;

    case OptDimmOdtPark:
    case OptDimmOdtParkDqs:
      Status = MrcDdr5SetParkOdt (MrcData, OptParam, ParamValue, MrData);
      break;

    default:
      Status = mrcWrongInputParameter;
      MRC_DEBUG_MSG(Debug, MSG_LEVEL_ERROR, "%s %u ", gWrongInputParam, OptParam);
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
Ddr5CalcDimmImpedance (
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

  // DDR5 Parameters
  DDR5_MODE_REGISTER_5_TYPE     Ddr5ModeRegister5;
  DDR5_MODE_REGISTER_34_TYPE    Ddr5ModeRegister34;
  DDR5_MODE_REGISTER_35_TYPE    Ddr5ModeRegister35;

  UINT32                        Dimm;
  UINT8                         *MR;
  UINT16                        *DimmOptParamValues;
  UINT8                         DimmOptParamMaxIndex;
  TOdtValueDqDdr5               Ddr5DqOdtTableIndex;
  TOdtValueCccDdr5              Ddr5CCCOdtTableIndex[2];
  UINT16                        Impedance;
  UINT8                         MrIndex;
  UINT16                        MrNum;
  UINT16                        MrValue;

  Outputs     = &MrcData->Outputs;
  ChannelOut  = &Outputs->Controller[Controller].Channel[Channel];
  Debug       = &Outputs->Debug;
  Dimm        = RANK_TO_DIMM_NUMBER (Rank);

  DimmOptParamMaxIndex = 0;
  DimmOptParamValues  = NULL;
  Impedance = 0x0; // Odt off

  if (GetFromTable) {
    Status = GetOdtTableIndex (MrcData, Controller, Channel, Dimm, &Ddr5DqOdtTableIndex);
    if (Status != mrcSuccess) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s: OdtTableIndex is NULL!\n", gErrString);
      return 0;
    }
    Status = GetDdr5CCCTableIndex(MrcData, Controller, Channel, Dimm, Ddr5CCCOdtTableIndex);
    if (mrcSuccess != Status) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s: Failed to get Ddr5CCCOdtTableIndex!\n", gErrString);
      return 0;
    }
    switch (OptParam) {
      case OptDimmOdtWr:
        Impedance = Ddr5DqOdtTableIndex.RttWr;
        break;
      case OptDimmOdtNomWr:
        Impedance = Ddr5DqOdtTableIndex.RttNomWr;
        break;
      case OptDimmOdtNomRd:
        Impedance = Ddr5DqOdtTableIndex.RttNomRd;
        break;
      case OptDimmOdtPark:
        Impedance = Ddr5DqOdtTableIndex.RttPark;
        break;
      case OptDimmOdtParkDqs:
        Impedance = Ddr5DqOdtTableIndex.RttParkDqs;
        break;
      case OptDimmOdtCAGroupA:
        Impedance = Ddr5CCCOdtTableIndex[0].RttCa;
        break;
      case OptDimmOdtCAGroupB:
        Impedance = Ddr5CCCOdtTableIndex[1].RttCa;
        break;
      case OptDimmOdtCSGroupA:
        Impedance = Ddr5CCCOdtTableIndex[0].RttCs;
        break;
      case OptDimmOdtCSGroupB:
        Impedance = Ddr5CCCOdtTableIndex[1].RttCs;
        break;
      default:
        break;
    }
  } else { // else GetFromTable == FALSE
    MR = ChannelOut->Dimm[(Rank / 2) % MAX_DIMMS_IN_CHANNEL].Rank[Rank % 2].MR;
    Ddr5GetOptDimmParamMrIndex (MrcData, OptParam, &MrIndex, &MrNum);
    MrValue = MR[MrIndex];

    Status = Ddr5GetDimmOptParamValues (MrcData, OptParam, &DimmOptParamValues, &DimmOptParamMaxIndex);
    if (Status != mrcSuccess || DimmOptParamValues == NULL) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s: DimmOptParamValues is NULL!\n", gErrString);
      return 0;
    }

    // DimmOptParamMaxIndex need -1 before being used as MAX Index
    DimmOptParamMaxIndex--;
    switch (OptParam) {
      case OptDimmOdtWr:
        Ddr5ModeRegister34.Data8 = (UINT8) MrValue;
        Impedance = Override ? OverrideValue : DimmOptParamValues[RANGE (Ddr5ModeRegister34.Bits.RttWr, 0, DimmOptParamMaxIndex)];
        break;
      case OptDimmOdtNom:
        //Assume caller uses OptDimmOdtNomWr or OptDimmOdtNomRd
        break;
      case OptDimmOdtNomWr:
        Ddr5ModeRegister35.Data8 = (UINT8) MrValue;
        Impedance = Override ? OverrideValue : DimmOptParamValues[RANGE (Ddr5ModeRegister35.Bits.RttNomWr, 0, DimmOptParamMaxIndex)];
        break;
      case OptDimmOdtNomRd:
        Ddr5ModeRegister35.Data8 = (UINT8) MrValue;
        Impedance = Override ? OverrideValue : DimmOptParamValues[RANGE (Ddr5ModeRegister35.Bits.RttNomRd, 0, DimmOptParamMaxIndex)];
        break;
      case OptDimmOdtPark:
      case OptDimmOdtParkDqs:
        // MPC CK/CS/CA GROUP A/B uses OP[2:0] to specify MR value
        MrValue = DDR5_MPC_GET_RTT_VALUE(MrValue);
        Impedance = Override ? OverrideValue : DimmOptParamValues[RANGE (MrValue, 0, DimmOptParamMaxIndex)];
        break;
      case OptDimmRon:
      case OptDimmRonUp:
        Ddr5ModeRegister5.Data8 = (UINT8) MrValue;
        // Need call DdrDrvStrDecode as DimmOptParamValues points to a table which is hard to map to MrValue directly
        Impedance = Override ? OverrideValue : ((UINT16) (DdrDrvStrDecode(Ddr5ModeRegister5.Bits.PullUpOutputDriverImpedance)));
        break;
      case OptDimmRonDn:
        Ddr5ModeRegister5.Data8 = (UINT8) MrValue;
        // Need call DdrDrvStrDecode as DimmOptParamValues points to a table which is hard to map to MrValue directly
        Impedance = Override ? OverrideValue : ((UINT16) (DdrDrvStrDecode(Ddr5ModeRegister5.Bits.PullDownOutputDriverImpedance)));
        break;
      case OptDimmOdtCA:
        /*
         *  Ddr5 uses OptDimmOdtCAGroupA and OptDimmOdtCAGroupB
         */
        break;
      case OptDimmOdtCAGroupA:
      case OptDimmOdtCAGroupB:
      case OptDimmOdtCSGroupA:
      case OptDimmOdtCSGroupB:
        // MPC CK/CS/CA GROUP A/B uses OP[2:0] to specify MR value
        MrValue = DDR5_MPC_GET_RTT_VALUE(MrValue);
        if (MrValue == 6) {
          // 110b is RFU
          MrValue = 0;
        } else if (MrValue == 7) {
          /*
           * 110b is RFU and 111b is RZQ/6, DimmOptParamValues points
           * to a table which has no the RFU member
           */
          MrValue--;
        }
        Impedance = Override ? OverrideValue : DimmOptParamValues[RANGE (MrValue, 0, DimmOptParamMaxIndex)];
        break;

      default:
        break;
    } // switch OptParam
  } //end else GetFromTable

  return Impedance;
}

/**
  Check if the given MR is one of the DFE TAP MR's

  @param[in] MrAddr - MR address.

  @retval TRUE if this is one of the DFE TAP MR's
**/
BOOLEAN
Ddr5IsDimmDfeMr (
  IN UINT16 MrAddr
  )
{
  UINT32  Bit;
  BOOLEAN IsDFEMr;

  IsDFEMr = FALSE;

  if (MrAddr >= mrMR129 && MrAddr <= mrMR252) {
    Bit = (MrAddr - mrMR129) / DDR5_DFE_TAP_MR_DQ_OFFSET;
    IsDFEMr |= (MrAddr == DFETap1Arrx16[Bit]);
    IsDFEMr |= (MrAddr == DFETap2Arrx16[Bit]);
    IsDFEMr |= (MrAddr == DFETap3Arrx16[Bit]);
    IsDFEMr |= (MrAddr == DFETap4Arrx16[Bit]);
  }
  return IsDFEMr;
}

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
  )
{
  UINT8  BitTap;
  BOOLEAN IsDQHMr;

  IsDQHMr = (MrAddr < mrMR193) ? FALSE: TRUE;

  BitTap = (MrAddr % (IsDQHMr ? mrMR193 : mrMR129));
  if (BitTap < MAX_DFE_TAP_DDR5) {
    *IsDimmDfeMrBit0 = TRUE;
  } else {
    *IsDimmDfeMrBit0 = FALSE;
    BitTap = ((MrAddr - mrMR129) % MAX_DFE_TAP_DDR5);
  }
  if (IsDQHMr) { // DQH will be 4-7 indexes
    BitTap += 4;
  }
  return BitTap;
}

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
  )
{
  MrcOutput     *Outputs;
  MrcChannelOut *ChannelOut;
  UINT32 Controller;
  UINT32 Channel;
  UINT32 Rank;
  UINT32 Device;
  UINT32 DdrRank;
  UINT8 MrrResult[MRC_MRR_ARRAY_SIZE];
  BOOLEAN IsDFESupported;
  BOOLEAN IsDFETap1Supported;
  BOOLEAN IsDFETap2Supported;
  BOOLEAN IsDFETap3Supported;
  BOOLEAN IsDFETap4Supported;
  DDR5_MODE_REGISTER_111_TYPE   *Mr111;

  Outputs = &MrcData->Outputs;

  IsDFESupported     = FALSE;
  IsDFETap1Supported = TRUE;
  IsDFETap2Supported = TRUE;
  IsDFETap3Supported = TRUE;
  IsDFETap4Supported = TRUE;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
         continue;
        }
        ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
        DdrRank = Rank / MAX_RANK_IN_DIMM;
        Mr111 = (DDR5_MODE_REGISTER_111_TYPE *)&ChannelOut->Dimm[DdrRank].Rank[(Rank % MAX_RANK_IN_DIMM)].MR[mrIndexMR111];
        MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR111, MrrResult);
        for (Device = 0; Device < Outputs->SdramCount; Device++) {
          Mr111->Data8 = MrrResult[Device];
          IsDFETap1Supported &= (Mr111->Bits.GlobalDfeTap1Enable == 0) ? TRUE : FALSE;
          IsDFETap2Supported &= (Mr111->Bits.GlobalDfeTap2Enable == 0) ? TRUE : FALSE;
          IsDFETap3Supported &= (Mr111->Bits.GlobalDfeTap3Enable == 0) ? TRUE : FALSE;
          IsDFETap4Supported &= (Mr111->Bits.GlobalDfeTap4Enable == 0) ? TRUE : FALSE;
        }
      }
    }
  }
  if (IsDFETap1Supported) {
    *OptParamLength = 1;
    if (IsDFETap2Supported) {
      *OptParamLength = 2;
      if (IsDFETap3Supported) {
        *OptParamLength = 3;
        if (IsDFETap4Supported) {
          *OptParamLength = 4;
        }
      }
    }
    IsDFESupported = TRUE;
  } else {
    *OptParamLength = 0;
  }
  return IsDFESupported;
}

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
  IN  UINT32               *tDQSCK, OPTIONAL
  IN  UINT32               *tDQSCKimax OPTIONAL
  )
{
  MrcOutput    *Outputs;
  MrcFrequency DdrFrequency;
  UINT32 tDQSCKiTemp;
  UINT32 tDQSCKMinMax;

  Outputs = &MrcData->Outputs;
  DdrFrequency  = Outputs->Frequency;

  if (DdrFrequency <= 3200) {
    tDQSCKMinMax = 240;
    tDQSCKiTemp = 410;
  } else if (DdrFrequency <= 3600) {
    tDQSCKMinMax = 252;
    tDQSCKiTemp = 430;
  } else if (DdrFrequency <= 4000) {
    tDQSCKMinMax = 270;
    tDQSCKiTemp = 460;
  } else if (DdrFrequency <= 4400) {
    tDQSCKMinMax = 286;
    tDQSCKiTemp = 475;
  } else if (DdrFrequency <= 4800) {
    tDQSCKMinMax = 300;
    tDQSCKiTemp = 490;
  } else if (DdrFrequency <= 5200) {
    tDQSCKMinMax = 313;
    tDQSCKiTemp = 510;
  } else if (DdrFrequency <= 5600) {
    tDQSCKMinMax = 325;
    tDQSCKiTemp = 530;
  } else if (DdrFrequency <= 6000) {
    tDQSCKMinMax = 337;
    tDQSCKiTemp = 549;
  } else if (DdrFrequency <= 6400) {
    tDQSCKMinMax = 348;
    tDQSCKiTemp = 567;
  } else if (DdrFrequency <= 6800) {
    tDQSCKMinMax = 359;
    tDQSCKiTemp = 585;
  } else if (DdrFrequency <= 7200) {
    tDQSCKMinMax = 370;
    tDQSCKiTemp = 602;
  } else if (DdrFrequency <= 7600) {
    tDQSCKMinMax = 380;
    tDQSCKiTemp = 619;
  } else if (DdrFrequency <= 8000) {
    tDQSCKMinMax = 390;
    tDQSCKiTemp = 635;
  } else if (DdrFrequency <= 8400) {
    tDQSCKMinMax = 400;
    tDQSCKiTemp = 651;
  } else {
    tDQSCKMinMax = 410;
    tDQSCKiTemp = 667;
  }

  if (tDQSCK != NULL) {
    *tDQSCK = tDQSCKMinMax;
  }
  if (tDQSCKimax != NULL) {
    *tDQSCKimax = tDQSCKiTemp;
  }
}

/**
  This function calculates the DDR5 timings tDQSS
  tDQSSMin/Max are same value except Min is negated

  @param[in]  MrcData     - Include all MRC global data.
  @param[out] tDQSSMinMax - Voltage/Temperature drift window of first rising DQS pre-amble
                              edge relative to CWL CK edge (tck*1000)
  @param[in] WrPreambleLow - Write Preamble low time
**/
VOID
GetDdr5tDQSSDram (
  IN   MrcParameters *const MrcData,
  OUT  INT32                *tDQSSMinMax,
  IN   UINT8                WrPreambleLow
  )
{
  if (tDQSSMinMax != NULL) {
    *tDQSSMinMax = ((WrPreambleLow == 1) ? 375 : 625);
  }
}

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
  )
{
  if (Frequency <= f5600) {
    *tADCmin = MRC_DDR5_tADC_5600_MIN;
    *tADCmax = MRC_DDR5_tADC_5600_MAX;
  } else if (Frequency <= f6000) {
    *tADCmin = MRC_DDR5_tADC_6000_MIN;
    *tADCmax = MRC_DDR5_tADC_6000_MAX;
  } else if (Frequency <= f6400) {
    *tADCmin = MRC_DDR5_tADC_6400_MIN;
    *tADCmax = MRC_DDR5_tADC_6400_MAX;
  } else if (Frequency <= f7200) {
    *tADCmin = MRC_DDR5_tADC_7200_MIN;
    *tADCmax = MRC_DDR5_tADC_7200_MAX;
  } else if (Frequency <= f8000) {
    *tADCmin = MRC_DDR5_tADC_8000_MIN;
    *tADCmax = MRC_DDR5_tADC_8000_MAX;
  } else if (Frequency <= f8400) {
    *tADCmin = MRC_DDR5_tADC_8400_MIN;
    *tADCmax = MRC_DDR5_tADC_8400_MAX;
  } else {
    *tADCmin = MRC_DDR5_tADC_8800_MIN;
    *tADCmax = MRC_DDR5_tADC_8800_MAX;
  }
}

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
MrcStatus
MrcDdr5GetDramCommandMap (
  IN  MrcParameters* const  MrcData,
  IN  UINT32                Address,
  IN  UINT32                Data,
  IN  MrDramCmdType         MrCmd,
  IN  BOOLEAN               IsMultiCast,
  OUT UINT32                *DramCmdData
  )
{
  // DDDR5 cmd map:
  // DATA[13:0]   = CA[13:0] Rise Edge
  // DATA[25:14]  = CA[13:0] Fall Edge
  // Refer to Command Truth Table for Shifting Operations
  switch (MrCmd) {
    case MrDramCmdMrw:
      *DramCmdData = (Data << 14) | (Address << 5) | DDR5_MRW_CMD;
      break;

    case MrDramCmdMpc:
      *DramCmdData = (Data << 5) | DDR5_MPC_CMD;
      break;

    case MrDramCmdNop:
      *DramCmdData = DDR5_NOP_CMD;
      break;

    case MrDramCmdPrea:
      *DramCmdData = DDR5_PREA_CMD;
      break;

    case MrcDramVref:
      *DramCmdData = (Data << 5) | DDR5_VREF_CMD;
      break;

    case MrDramCmdCs2nSyncNop:
      *DramCmdData = DDR5_NOP_CMD | (DDR5_NOP_CMD << 14);
      break;

    case MrDramCmdMrr:
      *DramCmdData = (Address << 5) | DDR5_MRR_CMD;
      // Mulitcast fall through to defualt
      if (!IsMultiCast) {
        break;
      }

    default:
      MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "Invalid MR Command !\n");
      return mrcFail;
  }
  return mrcSuccess;
}

/**
  This function returns the tXSDLL in DCLKs based on DDR Type, and Frequency.
  DDR5 has values per frequency.

  @param[in] Frequency  - The memory frequency.
**/
UINT32
MrcGetDdr5Txsdll (
  IN const  MrcFrequency  Frequency
  )
{
  UINT32  tXSDLL;

  if (Frequency <= f3600) {
    tXSDLL = tDLLK_DDR5_2000_3600;
  } else if (Frequency <= f4400) {
    tXSDLL = tDLLK_DDR5_3600_4400;
  } else if (Frequency <= f5200) {
    tXSDLL = tDLLK_DDR5_4400_5200;
  } else if (Frequency <= f6000) {
    tXSDLL = tDLLK_DDR5_5200_6000;
  } else if (Frequency <= f6800) {
    tXSDLL = tDLLK_DDR5_6000_6800;
  } else if (Frequency <= f7600) {
    tXSDLL = tDLLK_DDR5_6800_7600;
  } else if (Frequency <= f8400) {
    tXSDLL = tDLLK_DDR5_7600_8400;
  } else if (Frequency <= f8800) {
    tXSDLL = tDLLK_DDR5_8400_8800;
  } else {
    tXSDLL = tDLLK_DDR5_MAX;
  }

  return tXSDLL;
}

/**
  This function returns the tPDA_DELAY value

  @param[in] MrcData    - Include all MRC global data.

  @retval UINT32 - Timing in ns
**/
UINT32
MrcGetTpdaDelay (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput     *Outputs;
  UINT32        tCKps;
  UINT32        BLns;
  UINT32        tPDA_DELAY;

  Outputs = &MrcData->Outputs;
  tCKps  = Outputs->tCKps;
  BLns = DIVIDECEIL ((tCKps * Outputs->BurstLength), 1000);
  tPDA_DELAY = 18 + BLns + 19;
  return tPDA_DELAY;
}

/**
  Enabling RIR feature in DDR5 DRAM devices. It is controlled by MR4 OP[3] field
  which is Status Read/Host Write field, the MR field is read to check if the feature
  is implemented, if so it is enabled by writing 1 to the MR.

  @param[in]  MrcData        - The global host structure
  @param[out] IsRirSupported - True if DDR5 RIR is supported (MR4 OP[3] = 1)

  @return MrcStatus.
**/
MrcStatus
MrcTurnOnRirIfSupported (
  IN  MrcParameters* const MrcData,
  OUT BOOLEAN              *IsRirSupported
)
{
  MrcOutput *Outputs = &MrcData->Outputs;
  MrcStatus Status = mrcSuccess;

  UINT32 Controller;
  UINT32 Channel;
  MrcChannelOut *ChannelOut;
  UINT32 Rank;

  UINT8 MrrResult[MRC_MRR_ARRAY_SIZE];
  MrcModeRegister MrAddr = mrMR4;
  MrcModeRegisterIndex MrIndex;
  DDR5_MODE_REGISTER_4_TYPE Ddr5Mr4;
  DDR5_MODE_REGISTER_4_TYPE *Ddr5Mr4Host;
  UINT8 FirstController;
  UINT8 FirstChannel;

  *IsRirSupported = FALSE;

  FirstController   = Outputs->FirstPopController;
  FirstChannel      = Outputs->Controller[FirstController].FirstPopCh;

  MrIndex = MrcMrAddrToIndex(MrcData, &MrAddr);
  Status = MrcIssueMrr (MrcData, FirstController, FirstChannel, 0, MrAddr, MrrResult);
  if (Status != mrcSuccess) {
    return Status;
  }

  Ddr5Mr4.Data8 = MrrResult[0];
  *IsRirSupported = Ddr5Mr4.Bits.RefreshIntervalRateIndicator;

  if (*IsRirSupported) {
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
        if (!(MrcChannelExist (MrcData, Controller, Channel))) {
          continue;
        }
        for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
          if (!(MrcRankExist (MrcData, Controller, Channel, Rank))) {
            continue;
          }
          ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
          Ddr5Mr4Host = (DDR5_MODE_REGISTER_4_TYPE *) &ChannelOut->Dimm[RANK_TO_DIMM_NUMBER (Rank)].Rank[Rank % MAX_RANK_IN_DIMM].MR[MrIndex];
          Ddr5Mr4Host->Bits.RefreshIntervalRateIndicator = *IsRirSupported;
          MrcIssueMrw (MrcData, Controller, Channel, Rank, MrAddr, Ddr5Mr4Host->Data8, MRC_PRINTS_OFF);
        }
      }
    }
  }

  return Status;
}

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
  )
{
  MrcOutput                    *Outputs;
  MrcInput                     *Inputs;
  UINT8                        Index;
  UINT32                       tRCD;
  MrcStatus                    Status;

  Outputs = &MrcData->Outputs;
  Inputs  = &MrcData->Inputs;

  Status  = mrcSuccess;

  tRCD = Outputs->Controller[Controller].Channel[Channel].Timing[Inputs->ExtInputs.Ptr->MemoryProfile].tCL;
  tRCD *= Outputs->tCKps; // convert from nCK to ps
  tRCD /= 1000; //convert from ps to ns

  for (Index = 0; Ddr5PprGuardKeySequence[Index].MRnum != mrEndOfSequence; Index++) { // Pass PPR Guard Key
    Status = MrcIssueMrw (MrcData, Controller, Channel, Rank, Ddr5PprGuardKeySequence[Index].MRnum, Ddr5PprGuardKeySequence[Index].Data, TRUE);
    if (Status != mrcSuccess) {
      return Status;
    }
    MrcWait (MrcData, tRCD * MRC_TIMER_1NS); // wait tRCD
  }

  return Status;
}