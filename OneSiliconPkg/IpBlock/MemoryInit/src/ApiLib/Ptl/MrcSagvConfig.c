/** @file
  This file contains all the MRC Sagv Configuration

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

#include "MrcCommon.h"
#include "MrcSagv.h"

// PTL POR SAGV Point Configurations
SaGvTypes SagvFreqPor8533plus[MAX_SAGV_POINTS] = {
  //      0,            1,                2,             3
  LowBwSaGv,    MidBwSaGv,      MidHiBwSaGv,    HighBwSaGv // LPDDR5
};

// PTL POR SAGV Point Configurations
SaGvTypes SagvFreqPor[CALC_MRC_DDR_TYPE_MAX (MAX_MRC_DDR_TYPE)][MAX_SAGV_POINTS] = {
  //       0,             1,               2,               3
  {LowBwSaGv,     MidBwSaGv,   RfiHighBwSaGv,      HighBwSaGv}, // LPDDR5
  {LowBwSaGv,     MidBwSaGv,   RfiHighBwSaGv,      HighBwSaGv}, // DDR5
};

// LPDDR5/x Supported Frequencies
MrcFrequency Lp5SupportedFrequencies[] = {
  f9600,
  f8533,
  f8400,
  f8000,
  f7467,
  f6800,
  f6400,
  f6000,
  f5600,
  f5467,
  f4800,
  f4267,
  f3200,
  f2667,
  f2400
};

// DDR5 Supported Frequencies
MrcFrequency Ddr5SupportedFrequencies[] = {
  f8000,
  f7200,
  f6800,
  f6400,
  f6000,
  f5600,
  f5200,
  f4800,
  f4400,
  f4000,
  f3600,
  f3200
};

UINT8 Ddr5SupportedFrequenciesSize = ARRAY_COUNT (Ddr5SupportedFrequencies);
UINT8 Lp5SupportedFrequenciesSize  = ARRAY_COUNT  (Lp5SupportedFrequencies);

/**
  Returns the frequency point POR type for the input SaGvPoint and the current
  global system configuration.

  Returns HighBwSaGv by default if the current config is not specified in the POR config tables.

  @param[in]  MrcData     - Pointer to global MRC data.
  @param[in]  SaGvPoint   - Current operating SAGV point.
  @param[in]  FreqMax     - The maximum memory frequency allowed considering fuse, SPD, and topology limitations.

  @retval The SAGV Point config type
**/
SaGvTypes
MrcGetSaGvPointType (
  IN  MrcParameters *const  MrcData,
  IN  MrcSaGvPoint          SaGvPoint,
  IN  MrcFrequency          FreqMax
  )
{
  SaGvTypes  SaGvType;
  MrcOutput  *Outputs;
  MrcDdrType DdrType;
  BOOLEAN    IsLpddr5;

  Outputs  = &MrcData->Outputs;
  DdrType  = Outputs->DdrType;
  IsLpddr5 = Outputs->IsLpddr5;

  SaGvType = HighBwSaGv;
  if (SaGvPoint < MAX_SAGV_POINTS) {
    if (IsLpddr5 && FreqMax >= f8533) {
      SaGvType = SagvFreqPor8533plus[SaGvPoint];
    } else {
      SaGvType = SagvFreqPor[DdrType][SaGvPoint];
    }
  }
  return SaGvType;
}

/**
  This function returns the current configuration of Frequency and Gear ratio based on the
  input SaGvTypem, FreqMax, MaxQclkFreq, and global system configuration.

  It will update FreqOut and GearOut with the result.

  @param[in]  MrcData     - Pointer to global MRC data.
  @param[in]  SaGvType    - Point config type
  @param[in]  FreqMax     - The maximum memory frequency allowed considering fuse, SPD, and topology limitations
  @param[in]  MaxQclkFreq - The maximum QCLK frequency supported by the current hardware
  @param[out] FreqOut     - Pointer to return the SAGV point Frequency.
  @param[out] GearOut     - Pointer to return the SAGV point Gear.
**/
VOID
MrcCalcSagvTypeConfig (
  IN  MrcParameters *const  MrcData,
  IN  SaGvTypes             SaGvType,
  IN  MrcFrequency          FreqMax,
  IN  UINT32                MaxQclkFreq,
  OUT MrcFrequency          *FreqOut,     OPTIONAL
  OUT UINT32                *GearRatioOut OPTIONAL
  )
{
  MrcFrequency Freq;
  UINT32       Gear;
  UINT32       LowLatencyFreq;
  MrcOutput    *Outputs;
  BOOLEAN      IsLpddr5;
  BOOLEAN      IsDdr5;

  Outputs  = &MrcData->Outputs;
  IsLpddr5 = Outputs->IsLpddr5;
  IsDdr5   = Outputs->IsDdr5;

  Freq = 0;
  Gear = 0;
  switch (SaGvType) {
  case LowBwSaGv:
    if (IsLpddr5) {
      Freq = f2400;
      Gear = 4;
    } else if (IsDdr5) {
      Freq = f3200;
      Gear = 4;
    }
    break;

  case MidBwSaGv:
    Freq = f4800;
    Gear = 4;
    break;

  case MidHiBwSaGv:
    Freq = f6400;
    Gear = 4;
    break;

  case HighBwSaGv:
  case RfiHighBwSaGv:
    Freq = MrcGetNextSupportedFreq (MrcData, FreqMax);
    Gear = 4;
    break;

  case LowLatencySaGv:
  case RfiLowLatencySaGv:
    LowLatencyFreq = 2 * MaxQclkFreq;
    Freq = MrcGetNextSupportedFreq (MrcData, LowLatencyFreq);
    Freq = MIN (f3200, Freq);
    Gear = 2;
    break;

  default:
    // Do nothing, default values are defined outside of switch statement
    break;
  }

  if (SaGvType == RfiHighBwSaGv || SaGvType == RfiLowLatencySaGv) {
    if (IsLpddr5) {
      Freq = f6400;
    } else if (IsDdr5) {
      if (FreqMax == f7200) {
        Freq = f6400;
      } else {
        Freq = f6000;
      }
    } else {
      // Recalculate Freq - 1 bin
      Freq = Freq - 1;
      Freq = MrcGetNextSupportedFreq (MrcData, Freq);
    }
  }

  if (FreqOut != NULL) {
    *FreqOut = Freq;
  }
  if (GearRatioOut != NULL) {
    *GearRatioOut = Gear;
  }
}

/**
  This function sets the MRC override values for Sagv freq and gear

  @param[in]  MrcData     - Pointer to global MRC data.
**/
VOID
MrcSagvOverride (
  IN  MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs;
  MrcInput  *Inputs;
  MRC_EXT_INPUTS_TYPE *ExtInputs;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  ExtInputs = Inputs->ExtInputs.Ptr;

  if (Outputs->IsLpddr5) {
    ExtInputs->SaGvFreq[MrcSaGvPoint0] = f3200;
    ExtInputs->SaGvGear[MrcSaGvPoint0] = 4;
    ExtInputs->SaGvFreq[MrcSaGvPoint1] = f4267;
    ExtInputs->SaGvGear[MrcSaGvPoint1] = 4;
    ExtInputs->SaGvFreq[MrcSaGvPoint2] = f4267;
    ExtInputs->SaGvGear[MrcSaGvPoint2] = 4;
    ExtInputs->SaGvFreq[MrcSaGvPoint3] = f4800;
    ExtInputs->SaGvGear[MrcSaGvPoint3] = 4;
  } else {
    ExtInputs->SaGvFreq[MrcSaGvPoint0] = f4000;
    ExtInputs->SaGvGear[MrcSaGvPoint0] = 2;
    ExtInputs->SaGvFreq[MrcSaGvPoint1] = f4000;
    ExtInputs->SaGvGear[MrcSaGvPoint1] = 2;
    ExtInputs->SaGvFreq[MrcSaGvPoint2] = f4000;
    ExtInputs->SaGvGear[MrcSaGvPoint2] = 2;
    ExtInputs->SaGvFreq[MrcSaGvPoint3] = f4000;
    ExtInputs->SaGvGear[MrcSaGvPoint3] = 4;
  }
}