/** @file
  Implementation of memory abstraction interface code that runs
  on the Intel processor.

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

@par Specification
**/

#include "MrcMemoryApi.h" // temp to ensure prototypes match implementation (prototypes will move soon)
#include "MrcLpddr5.h"  // for MRC_LP5_BANKORG
#include "MrcDdr5.h"  // for MRC_DDR5_tPPD_ALL_FREQ
#include "MrcSpdProcessingDefs.h"  // for SdramCapacityTable[]

/**
  This function looks up the SAGV MR sequence and delays for the memory being initialized.
  The values will be stored in the array pointers passed in sequential order.
  The function will use the Length parameter to ensure it does not overflow the array passed.

  @param[in]  MrcData - Pointer to global MRC data.
  @param[out] MrSeq   - Pointer to array storing MR addresses.
  @param[out] MrDelay - Pointer to array storing MR delay after write.
  @param[in, out] Length - Pointer containing the number of entries available in the array.  Will be updated to the number of entries used upon return.
  @param[out] MrPerRank  - Output pointer to an array containing a list of MRs that must be configured on a per-rank basis
  due to possible unique values per rank. The is terminated using the value mrEndOfSequence.

  @retval MrcStatus - mrcSuccess if no errors
  @retval MrcStatus - mrcUnsupportedTechnology if the memory type is not supported.
  @retval MrcStatus - The return value of MrcSagvMrSeq per technology function.
**/
MrcStatus
MrcGetSagvMrSeq (
  IN      MrcParameters *const  MrcData,
  OUT     MrcModeRegister       *MrSeq,
  OUT     GmfTimingIndex        *MrDelay,
  IN OUT  UINT32                *Length,
  OUT     MrcModeRegister       **MrPerRank OPTIONAL
  )
{
  MrcStatus Status;

  switch (MrcData->Outputs.DdrType) {
    case MRC_DDR_TYPE_LPDDR5:
      Status = MrcSagvMrSeqLpddr5 (MrcData, MrSeq, MrDelay, Length, MrPerRank);
      break;

    default:
      MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "%s %s\n", gErrString, gUnsupportedTechnology);
      Length = 0;
      Status = mrcUnsupportedTechnology;
      if (MrPerRank != NULL) {
        *MrPerRank = NULL;
      }
      break;
  }

  return Status;
}

/**
  Calculate DqioDuration based on frequency and memory techmology

  @param[in] MrcData               - Include all MRC global data
  @param[out] *DqioDuration        - DqioDuration encoded to DDR5 MR45 / LPDDR5 MR37 definition
  @param[out] *RunTimeClocksBy16   - DqioDuration in units of (tCK * 16)

  @retval mrcSuccess               - if it success
  @retval mrcUnsupportedTechnology - if the frequency doesn't match
**/
MrcStatus
MrcGetDqioDuration (
  IN     MrcParameters *const MrcData,
  OUT    UINT8               *DqioDuration,
  OUT    UINT16              *RunTimeClocksBy16
  )
{
  MrcStatus Status;
  MrcOutput *Outputs;
  UINT32    Duration;
  Outputs = &MrcData->Outputs;
  Status  = mrcSuccess;
  Duration = DIVIDECEIL ((2047 * 2 * 300), (Outputs->tCKps * 16));
  if (Duration > 511) {
    *DqioDuration = (MRC_BIT7|MRC_BIT6);
    *RunTimeClocksBy16 = 512; // 8192 clocks
  } else if (Duration > 255) {
    *DqioDuration = MRC_BIT7;
    *RunTimeClocksBy16 = 256; // 4096 clocks
  } else if (Duration > 127) {
    *DqioDuration = MRC_BIT6;
    *RunTimeClocksBy16 = 128; // 2048 clocks
  } else if (Duration > 63) {
    *DqioDuration = 63;
    *RunTimeClocksBy16 = 63;  // 1008 clocks
  } else {
    *DqioDuration = (UINT8) Duration;
    *RunTimeClocksBy16 = (UINT16) Duration;
  }

  return Status;
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
      // Scale to WCK
      tPRPDEN *= 4;
      break;

    default:
    case MRC_DDR_TYPE_DDR5:
      tPRPDEN = 2;
      break;
  } // end switch

  // tPRPDEN must be programmed to a minimum of 4
  tPRPDEN = MAX (tPRPDEN, MRC_tPRPDEN_MIN);

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
  MinDlySbRef2Act = MinDlySbRef2Act * 4; // Multiply by 4 for units of WCK per tCK
  tBPR2ACT = DIVIDECEIL (MinDlySbRef2Act, TckPs);
  tBPR2ACT += ((tBPR2ACT % 4) == 0 ? 0 : (4 - (tBPR2ACT % 4))); // Round to the next number divisible by 4

  return tBPR2ACT;
}

/**
  This function returns the tRFM in DCLKs.
  DDR5 timing is based on tRFMsb
  LPDDR5 timing is based on tRFMpb

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - Controller to configure.
  @param[in] Channel    - Channel to configure.

  @retval tRFM - tRFM timing based on Device Capacity
**/
UINT32
MrcGetTrfm (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller,
  IN const UINT32         Channel
  )
{
  MrcOutput     *Outputs;
  MrcDdrType     DdrType;
  MrcChannelOut  *ChannelOut;
  UINT32         tRFM;
  UINT32         tCKps;
  UINT32         DeviceCapacity;

  Outputs   = &MrcData->Outputs;
  DdrType   = Outputs->DdrType;
  tCKps     = Outputs->tCKps;
  ChannelOut = &Outputs->Controller[Controller].Channel[Channel];

  DeviceCapacity = MAX ((SdramCapacityTable[ChannelOut->Dimm[dDIMM0].DensityIndex] * 8),
                        (SdramCapacityTable[ChannelOut->Dimm[dDIMM1].DensityIndex] * 8));
  switch (DdrType) {
    case MRC_DDR_TYPE_DDR5:
      tRFM = MrcGetDdr5Trfm (DeviceCapacity);
      break;

    case MRC_DDR_TYPE_LPDDR5:
      tRFM = MrcGetLpddr5Trfm (DeviceCapacity);
      break;

    default:
      tRFM = 0;
      break;
  }

  tRFM = DIVIDECEIL (tRFM / tCKps, 1);

  return tRFM;
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
