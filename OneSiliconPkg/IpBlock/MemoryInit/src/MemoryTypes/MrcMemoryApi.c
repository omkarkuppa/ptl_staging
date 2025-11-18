/** @file
Implementation of memory abstraction interface.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
#include "MrcDdrCommon.h"
#include "MrcChipApi.h"
#include "MrcMemoryApi.h"
#include "MrcDdr5.h"
#include "MrcLpddr5.h"
#include "MrcCommon.h"
#include "MrcSpdProcessingDefs.h" // for MrcDensityMax
#include "MrcHalRegisterAccess.h"
#include "MrcDdrIoUtils.h"
#include "MrcDdr5Settings.h"
#include "MrcLpddr5Settings.h"

//
// This table follows JEDEC LP4/5 SPD spec encoding for SPD byte 4 bits [3:0], except the last two entries which were added for DDR5.
// DDR5 SPD value should be converted using Ddr5SpdToDensityIndex(), before indexing into this table.
//
const UINT32 SdramCapacityTable[MrcDensityMax] = {
  (256 / 8),   // 256Mb (32MB)
  (512 / 8),   // 512Mb (64MB)
  (1024 / 8),  // 1Gb   (128MB)
  (2048 / 8),  // 2Gb   (256MB)
  (4096 / 8),  // 4Gb   (512MB)
  (8192 / 8),  // 8Gb   (1024MB)
  (16384 / 8), // 16Gb  (2048MB)
  (32768 / 8), // 32Gb  (4096MB)
  (12288 / 8), // 12Gb  (1536MB)
  (24576 / 8), // 24Gb  (3072MB)
  (3072 / 8),  // 3Gb   (384MB)
  (6144 / 8),  // 6Gb   (768MB)
  (18432 / 8), // 18Gb  (2304MB)
  (49152 / 8), // 48Gb  (6144MB)
  (65536 / 8)  // 64Gb  (8192MB)
};

/**
  This function returns the ODTL timing.

  @param[in]  MrcData - Pointer to MRC global data.
  @param[in]  Frequency - Current data rate.
  @param[in]  OdtType   - Selects which command ODT type.
  @param[in]  OdtlParam - Determines if this is for ODT On or Off.

  @retval INT8 - ODT impact to the command timing.
**/
INT8
MrcGetOdtlTiming (
  IN  MrcParameters *const  MrcData,
  IN  MrcFrequency          Frequency,
  IN  LPDDR_ODT_TYPE        OdtType,
  IN  LPDDR_ODTL_PARAM      OdtlParam
  )
{
  INT8            RetVal;
  MRC_LP5_BANKORG Lp5BankOrg;

  RetVal = 0;
  switch (MrcData->Outputs.DdrType)  {
    case MRC_DDR_TYPE_LPDDR5:
      if (OdtType == LpWrOdt) {
        Lp5BankOrg = MrcGetBankBgOrg (MrcData, Frequency);
        RetVal = MrcGetWrOdtlLpddr5 (MrcData, Frequency, OdtlParam, Lp5BankOrg);
      } else {
        RetVal = MrcGetNtRdOdtlLpddr5 (Frequency, OdtlParam);
      }
      break;

    default:
      break;
  }

  return RetVal;
}

/**
 * This functions maps MrcRhDrfmBrcType to MrcJedecBrcType.
 *
 * @param[in] DrfmBrc BRC value as MrcRhDrfmBrcType
 *
 * @returns Convert BRC value as MrcJedecBrcType
**/
MrcJedecBrcType
ConvertDrfmBrcToTMrcJedecBrcType (
  MrcRhDrfmBrcType BrcVal
  )
{
  MrcJedecBrcType JedecBrcType;

  switch (BrcVal) {
    case DrfmBrc4:
      JedecBrcType = JedecBrc4;
      break;
    case DrfmBrc3:
      JedecBrcType = JedecBrc3;
      break;
    case DrfmBrc2:
    default:
      JedecBrcType = JedecBrc2;
  }

  return JedecBrcType;
}

/**
  This function returns the timing value for Write Preamble.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval UINT8 - The delay in tCK.
**/
UINT8
MrcGetTwpre (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs;
  UINT8     tWPRE;

  Outputs = &MrcData->Outputs;

  switch (Outputs->DdrType) {
    case MRC_DDR_TYPE_DDR5:
      if (MrcData->Inputs.ExtInputs.Ptr->DqLoopbackTest) {
        // WRPRE should match RDPRE
        tWPRE = Outputs->ReadPreamble;
      } else {
        if (Outputs->Frequency <= f4800) {
          tWPRE = tWPRE_ALL_FREQ_DDR5_2tCK;
        } else if (Outputs->Frequency <= f6400) {
          tWPRE = tWPRE_ALL_FREQ_DDR5_3tCK;
        } else {
          tWPRE = tWPRE_ALL_FREQ_DDR5_4tCK;
        }
      }
      break;

    default:
      tWPRE = tWPRE_ALL_FREQ;
      break;
  }

  return tWPRE;
}

/**
  This function looks up the delay timing associated with the input TimingIndex.
  If no timing is associated with the TimingIndex then a value of 0 is returned on
  TimingNckOut pointer.

  @param[in]  MrcData      - Pointer to global MRC data.
  @param[out] TimingIndex  - The delay index to look up
  @param[out] TimingNckOut - Output pointer for the delay timing associated with the input DelayIndex in tCK units

  @retval MrcStatus - mrcSuccess if no errors
**/
MrcStatus
MrcGetGmfDelayTiming (
  IN      MrcParameters *const  MrcData,
  IN      GmfTimingIndex        TimingIndex,
  OUT     UINT16                *TimingNckOut
  )
{
  MrcStatus Status;

  Status = mrcSuccess;
  switch (MrcData->Outputs.DdrType) {
    case MRC_DDR_TYPE_LPDDR5:
      Status = Lpddr5GmfDelayTimings (MrcData, TimingIndex, TimingNckOut);
      break;

    case MRC_DDR_TYPE_DDR5:
      Status = Ddr5GmfDelayTimings (MrcData, TimingIndex, TimingNckOut);
      break;

    default:
      Status = mrcUnsupportedTechnology;
      MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "%s %s\n", gErrString, gUnsupportedTechnology);
      break;
  }

  return Status;
}

/**
  This function returns tWCKPRE_Static for both Writes and Reads

  @param[in]  MrcData   - Pointer to MRC global data.

  @retval INT8 - Timing in tCK, or 0 if unsupported for the DDR type.
**/
UINT8
MrcGetWckPreStatic (
  IN  MrcParameters *const  MrcData
  )
{
  MrcOutput *Outputs;
  UINT8     RetVal;

  Outputs = &MrcData->Outputs;

  switch (Outputs->DdrType) {
    case MRC_DDR_TYPE_LPDDR5:
      RetVal = MrcGetWckPreStaticLpddr5 (Outputs->Frequency);
      break;

    default:
      RetVal = 0;
      break;
  }

  return RetVal;
}

/**
  This function returns tWCKENL_FS for the current DRAM configuration.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval UINT8 - Timing in tCK, or 0 if unsupported.
**/
UINT8
MrcGetWckEnlFs (
  IN  MrcParameters *const  MrcData
  )
{
  MrcOutput *Outputs;
  UINT8     RetVal;

  Outputs = &MrcData->Outputs;

  switch (Outputs->DdrType) {
    case MRC_DDR_TYPE_LPDDR5:
      RetVal = MrcGetWckEnlFsLpddr5 (Outputs->Frequency);
      break;

    default:
      RetVal = 0;
      break;
  }

  return RetVal;
}

/**
  This function returns tWCKPRE_total_WR for the current DRAM configuration.

  @param[in]  MrcData   - Pointer to MRC global data.

  @retval UINT8 - Timing in tCK, or 0 if unsupported for the DDR type.
**/
UINT8
MrcGetWckPreWrTotal (
  IN  MrcParameters *const  MrcData
  )
{
  MrcOutput *Outputs;
  UINT8     RetVal;

  Outputs = &MrcData->Outputs;

  switch (Outputs->DdrType) {
    case MRC_DDR_TYPE_LPDDR5:
      RetVal = MrcGetWckPreWrTotalLpddr5 (Outputs->Frequency);
      break;

    default:
      RetVal = 0;
      break;
  }

  return RetVal;
}

/**
  This function returns tWCKPRE_total_RD for the current DRAM configuration.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval UINT8 - Timing in tCK, or 0 if unsupported for the DDR type.
**/
UINT8
MrcGetWckPreRdTotal (
  IN  MrcParameters *const  MrcData
  )
{
  MrcOutput *Outputs;
  UINT8     RetVal;

  Outputs = &MrcData->Outputs;

  switch (Outputs->DdrType) {
    case MRC_DDR_TYPE_LPDDR5:

      RetVal = MrcGetWckPreRdTotalLpddr5 (MrcData, Outputs->Frequency);
      break;

    default:
      RetVal = 0;
      break;
  }

  return RetVal;
}


/**
  This function determines if the SOC ODT requested is valid for the memory technology.
  If, the requested ODT is not supported, it will return the the closest matching ODT.
  If there is no limitation by the memory technology or the ODT value is supported,
  it will return the requested ODT.

  @param[in]  MrcData - Pointer to the global MRC data
  @param[in]  SocOdt  - The ODT requested by the system

  @retval - Valid ODT value.
**/
UINT16
MrcCheckForSocOdtEnc (
  IN  MrcParameters *const  MrcData,
  IN  UINT16                SocOdt
  )
{
  UINT16  SocOdtRet;

  switch (MrcData->Outputs.DdrType) {
    case MRC_DDR_TYPE_LPDDR5:
      SocOdtRet = MrcCheckSocOdtLpddr(MrcData, Lp5RzqValues, LP5_RZQ_NUM_VALUES, &PuCalSocOdtValidLp5[0], SocOdt);
      break;

    default:
      SocOdtRet = SocOdt;
  }

  return SocOdtRet;
}

/**
  This function handles calling the correct Init Mrw sequence based on DDR Technology.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval MrcStatus
**/
MrcStatus
MrcMrwInit (
  IN  MrcParameters *MrcData
  )
{
  MrcStatus Status;

  switch (MrcData->Outputs.DdrType) {
    case MRC_DDR_TYPE_LPDDR5:
      Status = InitMrwLpddr5 (MrcData);
      break;

    case MRC_DDR_TYPE_DDR5:
      Status = InitMrwDdr5 (MrcData);
      break;

    default:
      Status = mrcUnsupportedTechnology;
      break;
  }

  return Status;
}

/**
  This function handles calling the correct JEDEC Reset sequence based on DDR Technology.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval MrcStatus
**/
MrcStatus
MrcJedecReset (
  IN  MrcParameters *MrcData
  )
{
  MrcStatus Status;

  switch (MrcData->Outputs.DdrType) {
    case MRC_DDR_TYPE_LPDDR5:
      Status = MrcJedecResetLpddr (MrcData);
      break;

    case MRC_DDR_TYPE_DDR5:
      Status = MrcJedecResetDdr5 (MrcData);
      break;

    default:
      Status = mrcUnsupportedTechnology;
      break;
  }

  return Status;
}

/**
  This function handles calling the correct JEDEC Init sequence based on DDR Technology.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval MrcStatus
**/
MrcStatus
MrcJedecInit (
  IN  MrcParameters *MrcData
  )
{
  MrcStatus Status;

  switch (MrcData->Outputs.DdrType) {
    case MRC_DDR_TYPE_LPDDR5:
      Status = MrcJedecInitLpddr5 (MrcData);
      break;

    case MRC_DDR_TYPE_DDR5:
      Status = MrcJedecInitDdr5 (MrcData);
      break;

    default:
      Status = mrcUnsupportedTechnology;
      break;
  }

  return Status;
}

/**
  Configure LPDDR5 DRAM ZQ calibration settings in MR28.

  After configuration, wait appropriate delay.
  Finally, if ending ZQ stop, also send ZQ latch and wait tZQLAT.
  Not saving ZQ Stop status in host structure, after ZQ Stop the next call to this function is expected to be MrcConfigEndZqStop.

  @param[in]  MrcData     - Pointer to MRC global data.
  @param[in]  RankMask    - Bit mask of Rank's to configure.
  @param[in]  ZqRequest   - Options are MrcConfigSetZqReset, MrcConfigSetZqStop, or MrcConfigEndZqStop.


  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcConfigureZq (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 RankMask,
  IN  UINT8                 ZqRequest
  )
{
  MrcOutput      *Outputs;
  MrcInput       *Inputs;
  MrcChannelOut  *ChannelOut;
  MrcStatus      Status;
  UINT32         Controller;
  UINT32         Channel;
  UINT32         Delay;
  UINT8          *MrPtr;
  UINT8          Rank;
  LPDDR5_MODE_REGISTER_28_TYPE  Mr28Data;

  Outputs = &MrcData->Outputs;
  Inputs = &MrcData->Inputs;

  if (!Outputs->IsLpddr5) {
    return mrcUnsupportedTechnology;
  }
  if (ZqRequest >= MrcConfigZqMax) {
    return mrcWrongInputParameter;
  }
  Status = mrcSuccess;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if ((!MrcRankExist (MrcData, Controller, Channel, Rank)) || ((RankMask & (1 << Rank)) == 0)) {
          continue;
        }
        MrPtr = ChannelOut->Dimm[dDIMM0].Rank[Rank % MAX_RANK_IN_DIMM].MR;

        // Directly assign to local variable to avoid saving the changes to host structure.
        // ZqReset and ZqStop bits are always 0 in host structure, set the bits based on ZqRequest type
        Mr28Data.Data8 = MrPtr[mrIndexMR28];
        if (ZqRequest == MrcConfigSetZqReset) {
          Mr28Data.Bits.ZqReset = 1;
        } else if (ZqRequest == MrcConfigSetZqStop) {
          Mr28Data.Bits.ZqStop = 1;
        }

        Status = MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR28, Mr28Data.Data8, TRUE);
        if (Status != mrcSuccess) {
          return Status;
        }
      } // for Rank
    } // for Channel
  } // for Controller

  if (ZqRequest == MrcConfigEndZqStop) {
    // Wait tZQCALx, 1.5 us or higher, which is always greater than 4nCK.
    if (Inputs->Lp5NZQCount <= 4) {
      Delay = tZQCAL_4NZQ_NS_LPDDR5;
    } else if (Inputs->Lp5NZQCount <= 8) {
      Delay = tZQCAL_8NZQ_NS_LPDDR5;
    } else {
      Delay = tZQCAL_16NZQ_NS_LPDDR5;
    }
    MrcWait (MrcData, Delay * MRC_TIMER_1NS);

    // Send ZQ Latch, which includes waiting tZQLAT.
    Status = MrcIssueZQ (MrcData);
  } else if (ZqRequest == MrcConfigSetZqReset) {
    // tZQRESET is MAX(50ns, 3nCK)
    Delay = 3 * Outputs->MemoryClock;
    Delay = MAX (Delay, tZQRESET_FS);
    Delay = DIVIDECEIL (Delay, FEMTOSECONDS_PER_NANOSECOND);
    MrcWait (MrcData, Delay * MRC_TIMER_1NS);
  } else if (ZqRequest == MrcConfigSetZqStop) {
    // Wait tZQSTOP = 30ns
    MrcWait (MrcData, tZQSTOP_NS * MRC_TIMER_1NS);
  }

  return Status;
}

/**
  Program LPDDR5 FSP-OP and FSP-WR values.
  It will set the ranks specified in the rank mask in all Channels and Controllers populated.
  NOTE:
        Encoding for Vrcg MR field is the same for LPDDR5.
          So we only need to pass through the external type defined in MrcMemoryApi.h.
          If this changes, then this function will need to translate between MRC enumeration to MR enumeration.
        FSP points for LPDDR5 are integer points.  LPDDR5 supports 3 points.

  @param[in]  MrcData     - Pointer to MRC global data.
  @param[in]  RankMask    - Bit mask of Rank's to configure.
  @param[in]  VrcgMode    - VREF Current Generator mode switch.
  @param[in]  FspWePoint  - FSP-WR. Valid Values: LP5 - 0:2
  @param[in]  FspOpPoint  - FSP-OP. Valid Values: LP5 - 0:2

  @retval MrcStatus - mrcSuccess if successful, else an error status.
**/
MrcStatus
MrcSetFspVrcg (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 RankMask,
  IN  UINT8                 VrcgMode,
  IN  UINT8                 FspWePoint,
  IN  UINT8                 FspOpPoint
  )
{
  MrcOutput     *Outputs;
  MrcChannelOut *ChannelOut;
  MrcStatus     Status;
  UINT32        Controller;
  UINT32        Channel;
  UINT32        Delay;
  UINT8         *MrPtr;
  UINT8         Rank;

  Outputs = &MrcData->Outputs;

  if (!Outputs->IsLpddr5) {
    return mrcUnsupportedTechnology;
  }
  Status  = mrcSuccess;

  if (Outputs->IsDvfsqEnabled && (!Outputs->IsZqDisabled)) {
    if (FspOpPoint == LpFspOpPoint0) {
      // DVFSQ Enabled to Disabled uses JEDEC "DVFSQ Low-to-High Transition" flow
      // Before writing FSP-OP=1: End ZQ stop by resetting ZQ Stop bit. This will also handle appropriate wait times and send ZQ latch.
      MrcConfigureZq (MrcData, RankMask, MrcConfigEndZqStop);
    }
  }

  for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
    MrPtr = NULL;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
        if ((!MrcRankExist (MrcData, Controller, Channel, Rank)) || ((RankMask & (1 << Rank)) == 0)) {
          continue;
        }
        MrPtr = ChannelOut->Dimm[dDIMM0].Rank[Rank % MAX_RANK_IN_DIMM].MR;
        MrcLpddr5SetMr16 (MrcData, FspWePoint, FspOpPoint, MRC_IGNORE_ARG_8, VrcgMode, MRC_IGNORE_ARG_8, &MrPtr[mrIndexMR16]);
      }
    }
    // All channels have the same MR16 value on a given rank, so we can use multicast MRW
    if (MrPtr != NULL) {
      Status = MrcIssueMrwMulticast (MrcData, 1 << Rank, mrMR16, MrPtr[mrIndexMR16], TRUE);
      if (Status != mrcSuccess) {
        return Status;
      }
    }
  }

  if (FspOpPoint != MRC_IGNORE_ARG_8) {
    // When FSP-OP is changed, wait tFC_Long = 250ns or tFCExtended = 350 ns for frequencies above 8533
    // This covers for tVRCG_ENABLE/tVRCG_DISABLE as well if needed
    Delay = MrcGetLpddr5Tfc (Outputs->Frequency);
    // 4.16.1 Frequency Set Point Update Timing
    // Table 82 - AC Timing Table
    Delay += Outputs->tCKps / 2;

    MrcWait (MrcData, DIVIDECEIL (Delay, MRC_TIMER_1NS));

    if (Outputs->IsDvfsqEnabled) {
      if (FspOpPoint > LpFspOpPoint0) {
        Outputs->IsDramDvfsqOn = TRUE;
        if (!Outputs->IsZqDisabled) {
          // DVFSQ Disabled to Enabled uses JEDEC "DVFSQ High-to-Low Transition" flow.
          // After changing frequency, send ZQ Reset, then ZQ Stop.
          MrcConfigureZq (MrcData, RankMask, MrcConfigSetZqReset);
          MrcConfigureZq (MrcData, RankMask, MrcConfigSetZqStop);
        }
      } else {
        Outputs->IsDramDvfsqOn = FALSE;
      }
    }
  } else if (VrcgMode != MRC_IGNORE_ARG_8) {
    // When VRCG mode is changed, wait tVRCG_ENABLE or tVRCG_DISABLE
    Delay = (VrcgMode == LpVrcgNormal) ? MRC_LP5_tVRCG_DISABLE_NS : MRC_LP5_tVRCG_ENABLE_NS;
    MrcWait (MrcData, DIVIDECEIL (Delay, MRC_TIMER_1NS));
  }

  return Status;
}

/**
  GetOptDimmParamMrIndex using Technology specific GetOptDimmParamMrIndex functions

  @param[in]  MrcData   - Include all MRC global data.
  @param[in]  OptParam  - The Dimm Opt Param (e.g., OptDimmRon, OptDimmOdtWr, OptDimmOdtPark, OptDimmOdtNom)
  @param[out] *MrIndex  - Updated Pointer to the MR index.
  @param[out] *MrNum    - Updated Pointer to the MR number.

  @retval MrcStatus - mrcWrongInputParameter if unsupported Technology
                      Status of the Technology specific functions otherwise
**/

MrcStatus
GetOptDimmParamMrIndex (
  IN MrcParameters *const MrcData,
  IN UINT8                OptDimmParam,
  OUT UINT8               *MrIndex,
  OUT UINT16              *MrNum
  )
{
  MrcOutput  *Outputs;
  MrcDebug   *Debug;
  MrcStatus  Status;

  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;

  switch (Outputs->DdrType) {
    case MRC_DDR_TYPE_LPDDR5:
      Status = Lpddr5GetOptDimmParamMrIndex (MrcData, OptDimmParam, MrIndex, MrNum);
      break;

    case MRC_DDR_TYPE_DDR5:
      Status = Ddr5GetOptDimmParamMrIndex (MrcData, OptDimmParam, MrIndex, MrNum);
      break;

    default:
      Status = mrcWrongInputParameter;
      MRC_DEBUG_MSG(Debug, MSG_LEVEL_ERROR, "%s ", gUnsupportedTechnology);
  }

  return Status;
}

/**
  Determine the ODT table for the given Memory Profile / Controller / Channel / DIMM.

  @param[in]     MrcData         - Include all the mrc global data.
  @param[in]     Profile         - Memory Profile.
  @param[in]     Controller      - Controller to work on.
  @param[in]     Channel         - Channel to work on.
  @param[in]     Dimm            - DIMM to work on.
  @param[in out] OdtTable        - ODT structure - or (TOdtValueDdr5 *) or (TOdtValueLpddr *)

  @retval Pointer to the relevant ODT structure - or (TOdtValueDqDdr5 *) or (TOdtValueLpddr *)
**/
extern
MrcStatus
GetProfileOdtTableIndex (
  IN MrcParameters *const MrcData,
  IN const MrcProfile     Profile,
  IN const UINT32         Controller,
  IN const UINT32         Channel,
  IN const UINT32         Dimm,
  IN OUT   void           *OdtTable
  )
{
  MrcInput            *Inputs;
  MrcOutput           *Outputs;
  MRC_EXT_INPUTS_TYPE *ExtInputs;
  const MRC_FUNCTION  *MrcCall;
  void                *OdtTableRet;
  MrcChannelOut       *ChannelOut;
  MrcDimmOut          *DimmOut;
  TOdtIndex           OdtIndex;
  MrcStatus           Status;
  TOdtValueLpddr      OdtLpddr;
  UINT8               RanksInDimm0;
  BOOLEAN             IsDdr5;
  BOOLEAN             IsLpddr5;
  BOOLEAN             IsLpddr;

  Inputs       = &MrcData->Inputs;
  Outputs      = &MrcData->Outputs;
  ExtInputs    = Inputs->ExtInputs.Ptr;
  MrcCall      = Inputs->Call.Func;
  ChannelOut   = &Outputs->Controller[Controller].Channel[Channel];
  IsLpddr      = Outputs->IsLpddr;
  IsDdr5       = Outputs->IsDdr5;
  IsLpddr5     = Outputs->IsLpddr5;
  DimmOut      = &ChannelOut->Dimm[dDIMM0];
  RanksInDimm0 = DimmOut[dDIMM0].RankInDimm;
  OdtIndex     = oiNotValid;
  OdtTableRet  = NULL;
  Status = mrcSuccess;

  if (OdtTable == NULL) {
    return mrcFail;
  }

  if (IsLpddr) {
    OdtIndex = (RanksInDimm0 == 2) ? oi1DPC2R : oi1DPC1R;
  } else {
    OdtIndex = GetOdtIndexDdr5 (MrcData, Controller, Channel);
  }

  if (OdtIndex != oiNotValid) {
    // Retreive the appropriate ODT Table
    if (IsDdr5) {
      OdtTableRet = SelectTableDdr5 (MrcData, Profile, Dimm, OdtIndex);
    } else if (IsLpddr5) {
      OdtTableRet = SelectTableLpddr5 (MrcData, Dimm, OdtIndex);
    }

    // Check if the table is valid
    if (OdtTableRet == NULL) {
      return mrcFail;
    }

    // Copy the appropriate data to caller's pointer
    if (IsDdr5) {
      MrcCall->MrcCopyMem (OdtTable, OdtTableRet, sizeof(TOdtValueDqDdr5));
    } else if (IsLpddr) {
      // Start with the POR table data
      MrcCall->MrcCopyMem ((UINT8 *)&OdtLpddr, (UINT8 *)OdtTableRet, sizeof(TOdtValueLpddr));
      // Fill in user specified data
      if (ExtInputs->LpddrRttWr != 0) {
        OdtLpddr.RttWr = ExtInputs->LpddrRttWr;
      }
      if (ExtInputs->LpddrRttCa != 0) {
        OdtLpddr.RttCa = ExtInputs->LpddrRttCa;
      }

      // Valid Rtt value check
      if (LpddrOdtEncode (OdtLpddr.RttWr) == -1) {
        MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "%s Invalid %s ODT RttWr Value %u\n", gErrString, gCmdString, OdtLpddr.RttWr);
        Status = mrcWrongInputParameter;
      }
      if (LpddrOdtEncode (OdtLpddr.RttCa) == -1) {
        MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "%s Invalid %s ODT RttCa Value %u\n", gErrString, gCmdString, OdtLpddr.RttCa);
        Status = mrcWrongInputParameter;
      }

      // Copy data to caller pointer
      MrcCall->MrcCopyMem ((UINT8 *)OdtTable, (UINT8 *)&OdtLpddr, sizeof(TOdtValueLpddr));
    }

  } else {
    Status = mrcFail;
  }

  return Status;
}

/**
  Determine the ODT table for the given Controller / Channel / DIMM.
  Use Inputs values if there are customized values

  @param[in]     MrcData         - Include all the mrc global data.
  @param[in]     Controller      - Controller to work on.
  @param[in]     Channel         - Channel to work on.
  @param[in]     Dimm            - DIMM to work on.
  @param[in out] OdtTable        - ODT structure - or (TOdtValueDdr5 *) or (TOdtValueLpddr *)

  @retval Pointer to the relevant ODT structure - or (TOdtValueDqDdr5 *) or (TOdtValueLpddr *)
**/
extern
MrcStatus
GetOdtTableIndex (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller,
  IN const UINT32         Channel,
  IN const UINT32         Dimm,
  IN OUT   void           *OdtTable
  )
{
  MrcInput            *Inputs;
  MRC_EXT_INPUTS_TYPE *ExtInputs;
  UINT16              *OdtValue;
  UINT16              Index;
  MrcStatus           Status;

  Inputs    = &MrcData->Inputs;
  ExtInputs = Inputs->ExtInputs.Ptr;

  Status = GetProfileOdtTableIndex(MrcData,
      ExtInputs->MemoryProfile,
      Controller,
      Channel,
      Dimm,
      OdtTable);

  if ((mrcSuccess != Status) || (!MrcData->Outputs.IsDdr5)) {
    return Status;
  }

  // Check Inputs values
  OdtValue = (UINT16 *) OdtTable;
  for (Index = OFFSET_RTTWR; Index <= OFFSET_RTTPARKDQS; Index ++) {
    //DimmOdt[Dimm][Index] equals 0 meaning AUTO
    if (ExtInputs->DimmOdt[Dimm][Index] > MAX_DIMMODT_VALUE) {
      //Need disable
      OdtValue[Index] = 0;
    } else if (ExtInputs->DimmOdt[Dimm][Index] != 0) {
      OdtValue[Index] = ExtInputs->DimmOdt[Dimm][Index];
    }
  }

  return Status;
}

/**
  This function returns the tCKE value for the specified frequency.

  @param[in] DdrType    - DDR type
  @param[in] tCK        - DCLK period in femtoseconds.

  @return The tCKE value for the specified frequency.
**/
UINT32
MrcGetTcke (
  IN MrcDdrType         DdrType,
  IN UINT32             tCK
  )
{
  UINT32 tCKE;

  if (DdrType == MRC_DDR_TYPE_LPDDR5) {
    // tCSPD = 1 + tCSPD_MIN/tCK
    tCKE = DIVIDECEIL (MRC_LP5_tCSPD_MIN, tCK);
    tCKE += 1;
  } else {
    // DDR5
    tCKE = DIVIDECEIL (MRC_DDR5_tCKE_MIN, tCK);
    tCKE = MAX (tCKE, MRC_DDR5_tCKE_MIN_NCK);
  }

  return tCKE;
}

/**
  This function returns the tXP value for the specified frequency in tCK.

  @param[in] DdrType   - DDR type
  @param[in] tCK       - Clock period in femtoseconds.

  @retval The tXP value for the specified frequency.
**/
UINT32
MrcGetTxp (
  IN  MrcDdrType  DdrType,
  IN  UINT32      tCK
  )
{
  UINT32 tXP;
  UINT32 MinCk;
  UINT32 tXP_fs;

  if (DdrType == MRC_DDR_TYPE_DDR5) {
    // max(7.5ns, 8nCK)
    tXP = DIVIDECEIL (tXP_DDR5_FS, tCK);
    tXP = MAX (tXP, 8);
  } else {
    MinCk = MRC_LP5_tXP_MIN_NCK;
    tXP_fs = MRC_LPDDR5_tXP_FS;
    tXP = DIVIDECEIL (tXP_fs, tCK);
    tXP = MAX (tXP, MinCk);
  }

  return tXP;
}

/**
  This function returns the tRDPDEN value for the specified Memory type.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] tCL      - CAS Latency in tCK.

  @retval The tRDPDEN value for the specified configuration.
**/
UINT32
MrcGetTrdpden (
  IN MrcParameters *const MrcData,
  IN UINT32               tCL
  )
{
  MrcOutput       *Outputs;
  UINT32          Result;
  UINT32          tBLn_max;
  UINT32          tWCKDQO_MAX;

  Outputs = &MrcData->Outputs;
  tBLn_max = Outputs->BurstLength + (Outputs->Frequency > f3200 ? 4 : 0);

  if (Outputs->IsDdr5) {
    Result = tCL + MRC_DDR5_tCCD_ALL_FREQ + 1;
  } else { // LPDDR
    tWCKDQO_MAX = MrcLpddrIsLowFreq (Outputs->Frequency) ? MRC_LP5_tWCKDQO_LF_MAX : MRC_LP5_tWCKDQO_HF_MAX;
    Result = (tCL + tBLn_max + DIVIDECEIL(tWCKDQO_MAX, Outputs->tCKps) + 1);
  }

  return Result;
}

/**
  This function returns the tWRPDEN value for the specified Memory type.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] tCWL     - WR CAS Latency in tCK.
  @param[in] tWR      - Write recovery time in tCK.

  @retval The tWRPDEN value for the specified configuration.
**/
UINT32
MrcGetTwrpden (
  IN MrcParameters *const MrcData,
  IN UINT32               tCWL,
  IN UINT32               tWR
  )
{
  MrcOutput       *Outputs;
  MrcFrequency    Frequency;
  UINT32          Result;
  UINT32          tWCKDQI;
  UINT32          tBLn_max;

  Outputs = &MrcData->Outputs;
  Frequency = Outputs->Frequency;


  if (Outputs->IsDdr5) {
    Result = tCWL + MRC_DDR5_tCCD_ALL_FREQ  + tWR + 1;
  } else { // LPDDR
    tWCKDQI = MrcLpddrIsLowFreq (Frequency) ? MRC_LP5_tWCKDQI_LF_MAX :
                                 (Frequency <= f6400 ? MRC_LP5_tWCKDQI_MF_MAX : MRC_LP5_tWCKDQI_HF_MAX);

    // tWRPDEN = (tCWL + CEIL(tWCKDQImax/tCKPs) + tBL/n_max + tWR + 1) 4
    tBLn_max = Outputs->BurstLength + (Outputs->Frequency > f3200 ? 4 : 0);
    Result = (tCWL + DIVIDECEIL (tWCKDQI, Outputs->tCKps) + tBLn_max + tWR + 1);
  }

  return Result;
}

/**
  This function returns the tCPDED value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tCPDED value for the specified configuration.
**/
UINT32
MrcGetTcpded (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs;
  UINT32 Result = 0;

  Outputs = &MrcData->Outputs;

  if (Outputs->IsLpddr5) {
    Result = MRC_LP5_tCPDED;
  } else if (Outputs->IsDdr5){
    // tCPDED = max (5ns, 8nCK)
    Result = MAX (DIVIDECEIL (MRC_DDR5_tCPDED_PS, Outputs->MemoryClock), 8);
  }

  return Result;
}


/**
  This function returns the tCKCKEH value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tCKCKEH value for the specified configuration.
**/
UINT32
MrcGetTckckeh (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs = &MrcData->Outputs;

  UINT32 Result = 0;

  if (Outputs->IsLpddr5) {
    // tCKCKEH = MAX (1.75ns; 2nCK)
    Result = MAX (DIVIDECEIL (MRC_LP5_tCKCKEH, Outputs->MemoryClock), MRC_LP5_tCKCKEH_nCK);
  } else if (Outputs->IsDdr5) {
    // tCKCKEH = MAX(8; 3.5nCK)
    Result = MAX (MRC_DDR5_tCKCKEH_MIN, DIVIDECEIL (MRC_DDR5_tCKCKEH_MAX_FS, Outputs->MemoryClock));
  }

  return Result;
}

/**
  This function returns the tCACSH value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tCACSH value for the specified configuration.
**/
UINT32
MrcGetTcacsh (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs = &MrcData->Outputs;
  UINT32 Result = 0;

  if (Outputs->IsLpddr5) {
    // tCACSH = 1.75 ns
    Result = DIVIDECEIL (MRC_LP5_tCACSH, Outputs->MemoryClock);
  } else { // DDR5
    if (Outputs->GearMode == 0) { // Gear2
      Result = 12;
    } else { // Gear4
      Result = 20;
    }
  }

  return Result;
}

/**
  This function returns the value of tXSR in tCK based on DdrType.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Timing     - Pointer to timing values.

  @retval UINT32 - tXSR in tCK.
**/
UINT32
MrcGetTxsr (
  IN MrcParameters *const MrcData,
  IN MrcTiming *const     Timing
  )
{
  MrcOutput *Outputs;
  UINT32    tXSR;

  Outputs = &MrcData->Outputs;

  switch (Outputs->DdrType) {
    case MRC_DDR_TYPE_DDR5:
      tXSR = MrcGetDdr5Txsr (Timing->tRFC);
      break;

    case MRC_DDR_TYPE_LPDDR5:
      tXSR = MrcGetLpddr5Txsr (Timing->tCK, Timing->tRFC);
      break;

    default:
      tXSR = 0;
      break;
  }

  return tXSR;
}

/**
  This function returns the tMOD value.
  For DDR5 this is for "tRMD"

  @param[in] MrcData   - Include all MRC global data.
  @param[in] tCK       - DCLK period in femtoseconds.

  @retval UINT32 - Timing in tCK.
**/
UINT32
MrcGetTmod (
  IN MrcParameters *const MrcData,
  IN const UINT32         tCK
  )
{
  UINT32  tMOD;

  switch (MrcData->Outputs.DdrType) {
    case MRC_DDR_TYPE_DDR5:
      tMOD = MrcGetDdr5Tmod (tCK);
      break;

    case MRC_DDR_TYPE_LPDDR5:
      tMOD = MrcGetLpddr5Tmod (tCK);
      break;

    default:
      tMOD = 0;
      break;
  }

  return tMOD;
}

/**
  This function returns the tMRW value.
  @param[in] MrcData   - Include all MRC global data.
  @param[in] tCK       - DCLK period in femtoseconds.
  @retval UINT32 - Timing in tCK.
**/
UINT32
MrcGettMRW (
  IN MrcParameters *const MrcData,
  IN const UINT32         tCK
  )
{
  UINT32  tMRW;

  switch (MrcData->Outputs.DdrType) {
    case MRC_DDR_TYPE_DDR5:
      tMRW = MrcGetDdr5tMRW (tCK);
      break;

    case MRC_DDR_TYPE_LPDDR5:
      tMRW = MrcGetLpddr5tMRW (tCK);
      break;

    default:
      tMRW = 0;
      break;
  }

  return tMRW;
}

/**
  This function returns the tZQCAL value in DCLKs.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] tCK      - DCLK period in femtoseconds.

  @retval UINT32 - Timing in nCK.
**/
UINT32
MrcGetTzqcal (
  IN MrcParameters *const MrcData,
  IN UINT32               tCK
  )
{
  UINT32  tZQCAL;

  switch (MrcData->Outputs.DdrType) {
    case MRC_DDR_TYPE_DDR5:
      tZQCAL = MrcGetDdr5Tzqcal (tCK);
      break;

    case MRC_DDR_TYPE_LPDDR5:
      tZQCAL = MrcGetLpddr5Tzqcal (MrcData, tCK);
      break;

    default:
      tZQCAL = 0;
      break;
  }

  return tZQCAL;
}

/**
  This function returns the tZQLAT value in DCLKs.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] tCK      - DCLK period in femtoseconds.

  @retval UINT32 - Timing in tCK.
**/
UINT32
MrcGetTzqlat (
  IN MrcParameters *const MrcData,
  IN UINT32               tCK
  )
{
  UINT32  tZQLAT;

  switch (MrcData->Outputs.DdrType) {
    case MRC_DDR_TYPE_DDR5:
      //max (30ns, 8nCK)
      tZQLAT = MrcGetDdr5Tzqlat (tCK);
      break;

    case MRC_DDR_TYPE_LPDDR5:
      //tZQLAT = MAX (30ns, 4nCK)
      tZQLAT = MrcGetLpddr5Tzqlat (tCK);
      break;

    default:
      tZQLAT = 0;
      break;
  }

  return tZQLAT;
}

/**
  Used to update VrefDQ or VrefCA.
  Uses input offset value to increment/decrement current setting.

  @param[in,out] MrcData             - Include all MRC global data.
  @param[in]     Controller          - Selecting which Controller to talk to.
  @param[in]     Channel             - Selecting which Channel to talk to.
  @param[in]     RankMask            - Selecting which Ranks to talk to.
  @param[in]     DeviceMask          - Selecting which Devices to talk to (only valid adjusting VrefDQ).
  @param[in]     VrefType            - Determines the Vref type to change, only CmdV and TxVref are valid.
  @param[in]     Offset              - Vref offset value.
  @param[in]     UpdateMrcData       - Used to decide if Mrc host must be updated.
  @param[in]     PdaMode             - Selecting to use MRH or old method for MRS (only valid for adjusting VrefDQ).
  @param[in]     IsCachedOffsetParam - Determines if the parameter is an offset (relative to cache) or absolute value.
**/
void
MrcSetDramVref (
  IN OUT MrcParameters *const MrcData,
  IN     UINT8                Controller,
  IN     UINT8                Channel,
  IN     UINT8                RankMask,
  IN     UINT16               DeviceMask,
  IN     UINT8                VrefType,
  IN     INT32                Offset,
  IN     BOOLEAN              UpdateMrcData,
  IN     BOOLEAN              PdaMode,
  IN     BOOLEAN              IsCachedOffsetParam
  )
{
  MrcDebug        *Debug;
  MrcOutput       *Outputs;
  MrcStatus        Status;

  Outputs       = &MrcData->Outputs;
  Debug         = &Outputs->Debug;

  switch (Outputs->DdrType) {
    case MRC_DDR_TYPE_DDR5:
      Status = Ddr5SetDramVref (MrcData, Controller, Channel, RankMask, DeviceMask, VrefType, Offset, UpdateMrcData, PdaMode, IsCachedOffsetParam);
      break;

    case MRC_DDR_TYPE_LPDDR5:
      Status = Lpddr5SetDramVref (MrcData, Controller, Channel, RankMask, VrefType, Offset, UpdateMrcData, IsCachedOffsetParam);
      break;

    default:
      Status = mrcWrongInputParameter;
      MRC_DEBUG_MSG(&Outputs->Debug, MSG_LEVEL_ERROR, "%s ", gUnsupportedTechnology);
      break;
    }

  MRC_DEBUG_ASSERT(Status == mrcSuccess, Debug, "Error Setting DIMM VREF");
}

/**
  This function returns the tXSDLL in DCLKs based on DDR Type, and Frequency.
  DDR5 has values per frequency, and LPDDR5 do not have
  a DLL.  From the spec, we must wait tXSR before issuing a command.  Thus,
  we set tXSDLL to tXSR to ensure tXSR before read/write commands.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Frequency  - The memory frequency.
  @param[in] tXSR       - Value of tXSR.
**/
UINT32
MrcGetTxsdll (
  IN MrcParameters *const MrcData,
  IN const MrcFrequency   Frequency,
  IN const INT64          tXSR
  )
{
  UINT32  tXSDLL;
  MrcDdrType  DdrType;

  DdrType = MrcData->Outputs.DdrType;

  switch (DdrType) {
    case MRC_DDR_TYPE_DDR5:
      tXSDLL = MrcGetDdr5Txsdll (Frequency);
      break;

    default:
      tXSDLL = 0;
      break;
  }

  return tXSDLL;
}

/**
  This function returns the tWCKOFF value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tWCKOFF value for the specified configuration.
**/
UINT32
MrcGetTwckoff (
  IN MrcParameters *const MrcData
  )
{
  UINT32 Result = 0;
  // twckoff holds the additional time on top wr_wck_async_gap to allow "RD/WR -> CAS WS_OFF"
  // In JEDEC spec "Write Clock Always on mode (WCK Always on mode)" chapter, this addition is 1nCK
  if (MrcData->Outputs.IsLpddr5) {
    Result = 1;
  }
  return Result;
}

/**
  This function returns the BL/n_min value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The BL/n_min value for the specified configuration.
**/
UINT32
MrcGetBurstLengthNMin (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs = &MrcData->Outputs;
  MrcFrequency Frequency = Outputs->Frequency;
  UINT32 Result = Outputs->BurstLength;
  MRC_LP5_BANKORG Lp5BGOrg = MrcGetBankBgOrg (MrcData, Frequency);

  if (Outputs->IsLpddr5 && Frequency > f3200 && Lp5BGOrg != MrcLp58Bank) {
    Result += 2;
  }

  return Result;
}

/**
  This function returns the BL/n_max value. This implementation assumes that only BL32 is supported by BIOS.
  Refer to Effective BurstLength Definition in JEDEC specs.

  @param[in] MrcData  - Include all MRC global data.

  @returns The BL/n_max value for the specified configuration.
**/
UINT32
MrcGetBurstLengthNMax (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs = &MrcData->Outputs;
  MrcFrequency Frequency = Outputs->Frequency;
  UINT32 Result = Outputs->BurstLength;
  MRC_LP5_BANKORG Lp5BGOrg = MrcGetBankBgOrg (MrcData, Frequency);

  if (Outputs->IsLpddr5 && Frequency > f3200 && Lp5BGOrg != MrcLp58Bank) {
    Result += 4;
  }

  return Result;
}

/**
  This function returns the trd_wck_async_gap value for the specified Memory type.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] tCL      - CAS Latency in tCK.

  @returns The trd_wck_async_gap value for the specified configuration.
**/
UINT32
MrcGetTrdWckAsyncGap (
  IN MrcParameters *const MrcData,
  IN UINT32               tCL
  )
{
  UINT32 Result = 0;
  if (MrcData->Outputs.IsLpddr5) {
    // rd_wck_async_gap = RL + BL/n_max + RD(tWCKPST/tCK)
    // tWCKPST is 2.5 tWCK, so RD(tWCKPST/tCK) is 0
    Result = (tCL + MrcGetBurstLengthNMax (MrcData));
  }
  return Result;
}

/**
  This function returns the twr_wck_async_gap value for the specified Memory type.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] tCWL     - WR CAS Latency in tCK.

  @returns The twr_wck_async_gap value for the specified configuration.
**/
UINT32
MrcGetTwrWckAsyncGap (
  IN MrcParameters *const MrcData,
  IN UINT32               tCWL
  )
{
  UINT32 Result = 0;
  if (MrcData->Outputs.IsLpddr5) {
    // wr_wck_async_gap = CWL + BL/n_max + RD(tWCKPST/tCK)
    // tWCKPST is 2.5 tWCK, so RD(tWCKPST/tCK) is 0
    Result = (tCWL + MrcGetBurstLengthNMax (MrcData));
  }
  return Result;
}

/**
  This function returns the tWCKSTOP value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tWCKSTOP value for the specified configuration.
**/
UINT32
MrcGetTwckstop (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs = &MrcData->Outputs;
  UINT32 Result = 0;
  if (Outputs->IsLpddr5) {
    // tWCKSTOP = MAX (6ns, 2nCK)
    Result =  MAX(DIVIDECEIL (MRC_LP5_tWCKSTOP_MIN, Outputs->MemoryClock), MRC_LP5_tWCKSTOP_MIN_NCK);
  }
  return Result;
}

/**
  This function returns the tCSLCK value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tCSLCK value for the specified configuration.
**/
UINT32
MrcGetTcsclk (
  IN MrcParameters *const MrcData
  )
{
  UINT32 Result = 0;
  MrcOutput *Outputs = &MrcData->Outputs;
  if (Outputs->IsLpddr5) {
    // tCSLCK = MAX (5ns, 3nCK)
    Result =  MAX(DIVIDECEIL (MRC_LP5_tCSCLK_MIN, Outputs->MemoryClock), MRC_LP5_tCSCLK_MIN_NCK);
  }
  return Result;
}

/**
  This function returns the tCKFSPX value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tCKFSPX value for the specified configuration.
**/
UINT32
MrcGetTckfspx (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs = &MrcData->Outputs;
  UINT32 Result = 0;

  if (Outputs->IsLpddr5) {
    // tCKFSPX = MAX (7.5ns, 4nCK)
    Result =  MAX(DIVIDECEIL (MRC_LP5_tCKFSPX_MIN, Outputs->MemoryClock), MRC_LP5_tCKFSPX_MIN_NCK);
  }

  return Result;
}

/**
  This function returns the tVRCG_ENABLE value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tVRCG_ENABLE value for the specified configuration.
**/
UINT32
MrcGetTvrcgEnable (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs = &MrcData->Outputs;
  UINT32 Result = 0;

  if (Outputs->IsLpddr5) {
    // tVRCG_ENABLE = 150 ns
    Result =  DIVIDECEIL (MRC_LP5_tVRCG_ENABLE_FS, Outputs->MemoryClock);
  }

  return Result;
}

/**
  This function returns the tVRCG_DISABLE value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tVRCG_DISABLE value for the specified configuration.
**/
UINT32
MrcGetTvrcgDisable (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs = &MrcData->Outputs;
  UINT32 Result = 0;

  if (Outputs->IsLpddr5) {
    // tVRCG_DISABLE = 100 ns
    Result =  DIVIDECEIL (MRC_LP5_tVRCG_DISABLE_FS, Outputs->MemoryClock);
  }

  return Result;
}

/**
  This function returns the tRCDw value.

  @param[in] MrcData  - Include all MRC global data.
  @param[in] tRCD      - RAS to CAS Latency in tCK.

  @returns The tRCDw value for the specified configuration.
**/
UINT32
MrcGetTrcdw (
  IN MrcParameters *const MrcData,
  IN UINT32               tRCD
  )
{
  MrcOutput *Outputs = &MrcData->Outputs;
  UINT32 Result = tRCD;

  if (Outputs->LpX) {
    if (Outputs->IsDvfscEnabled) {
      // tRCD LPx with DVFSC = max (9ns, 2nCK)
      Result = MAX (DIVIDECEIL (MRC_LP5X_TRCDW_DVFSC_MIN, Outputs->MemoryClock), MRC_LP5X_TRCDW_NCK_MIN);
    } else {
      // tRCD LPx = max (8ns, 2nCK)
      Result = MAX (DIVIDECEIL (MRC_LP5X_TRCDW_MIN, Outputs->MemoryClock), MRC_LP5X_TRCDW_NCK_MIN);
    }
  } else if (Outputs->IsLpddr5) {
    Result = tRCD;
  }

  return Result;
}

/**
  This function returns the tMRR value.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] tCL        - CAS Latency in tCK.

  @retval UINT32 - Timing in tCK
**/
UINT32
MrcGetTmrr (
  IN MrcParameters *const MrcData,
  IN const UINT32         tCL
  )
{
  MrcOutput *Outputs = &MrcData->Outputs;
  UINT32 Result = 0;

  if (Outputs->IsDdr5) {
    // tMRR = max(14ns, 16nCK)
    Result = MAX (DIVIDECEIL (tMRR_DDR5_FS, Outputs->MemoryClock), tMRR_DDR5_nCK);
  } else if (Outputs->IsLpddr5) {
    // tMRR = (RL + BL/n_max + RD (tWCKPST/tCK)  + 2)
    Result = (tCL + MrcGetBurstLengthNMax (MrcData) + 2);
  }

  return Result;
}

/**
  This function returns the Write to MRR value.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] tCWL       - CAS Latency in tCK.
  @param[in] tWTR       - WRITE-to-READ delay.
  @param[in] tWTR_L     - WRITE-to-READ delay long.

  @retval UINT32 - Timing in tCK
**/
UINT32
MrcGetWrToMrr (
  IN MrcParameters *const MrcData,
  IN const UINT32         tCWL,
  IN const UINT32         tWTR,
  IN const UINT32         tWTR_L
  )
{
  MrcOutput *Outputs = &MrcData->Outputs;
  MRC_LP5_BANKORG Lp5BankOrg;
  UINT32 Result = 0;

  if (Outputs->IsLpddr5) {
    Lp5BankOrg = MrcGetBankBgOrg (MrcData, Outputs->Frequency);
    if (Lp5BankOrg == MrcLp5BgMode) {
      // tMRW = (WL + (BL/N_max) + RU(tWTR_L/tCK))
      Result = (tCWL + MrcGetBurstLengthNMax (MrcData) + tWTR_L);
    } else {
      // tMRW = (WL + (BL/N_max) + RU(tWTR/tCK))
      Result = (tCWL + MrcGetBurstLengthNMax (MrcData) + tWTR);
    }
  }

  return Result;
}

/**
  This function returns the timing value for Precharge to Precharge delay.

  @param[in]  MrcData - Pointer to MRC global data.

  @retval UINT8 - 0 for unsupported technologies, otherwise the delay in tCK
**/
UINT8
MrcGetTppd (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs = &MrcData->Outputs;
  UINT8 tPPD;

  switch (Outputs->DdrType) {
    case MRC_DDR_TYPE_LPDDR5:
      tPPD = MRC_LP5_tPPD_ALL_FREQ;
      break;

    case MRC_DDR_TYPE_DDR5:
      if (Outputs->Frequency < f7600) {
        tPPD = MRC_DDR5_tPPD_LOW_FREQ;
      } else {
        tPPD = MRC_DDR5_tPPD_HIGH_FREQ;
      }
      break;

    default:
      tPPD = 0;
      break;
  }

  return tPPD;
}

/**
  This function returns the tCSH value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tCSH value for the specified configuration.
**/
UINT32
MrcGetTcsh (
  IN MrcParameters *const MrcData
  )
{
  UINT32 Result = 0;
  MrcOutput *Outputs = &MrcData->Outputs;

  if (Outputs->IsLpddr5) {
    Result = (DIVIDECEIL (MRC_LP5_tCSH_MIN, Outputs->MemoryClock));
  } else if (Outputs->IsDdr5) {
    Result = DIVIDECEIL (MRC_DDR5_tCSH_MIN_FS, Outputs->MemoryClock);
  }
  return Result;
}

/**
  This function returns the tCSL value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tCSL value for the specified configuration.
**/
UINT32
MrcGetTcsl (
  IN MrcParameters *const MrcData
  )
{
  UINT32 Result = 0;
  MrcOutput *Outputs = &MrcData->Outputs;

  if (Outputs->IsLpddr5) {
    Result = (DIVIDECEIL (MRC_LP5_tCSL_MIN, Outputs->MemoryClock));
  } else if (Outputs->IsDdr5) {
    Result = MRC_DDR5_tCSL_MIN_NCK;
  }

  return Result;
}

/**
  This function returns the tSR value.

  @param[in] MrcData  - Include all MRC global data.

  @returns The tSR value for the specified configuration.
**/
UINT32
MrcGetTsr (
  IN MrcParameters *const MrcData
  )
{
  UINT32 Result = 0;
  MrcOutput *Outputs = &MrcData->Outputs;

  if (Outputs->IsLpddr5) {
    Result = DIVIDECEIL (MRC_LP5_tSR_MIN, Outputs->MemoryClock);
  } else {
    Result = DIVIDECEIL (MRC_DDR5_tSR_MIN, Outputs->MemoryClock);
  }

  return Result;
}

/**
  This function returns the tOSCO value.

  @param[in] MrcData    - Include all MRC global data.

  @retval UINT32 - Timing in tCK.
**/
UINT32
MrcGetTosco (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput   *Outputs = &MrcData->Outputs;
  UINT32      Result = 0;

  if (Outputs->IsLpddr5) {
    Result = MAX (DIVIDECEIL (MRC_LP5_tOSCO_FS, Outputs->MemoryClock), MRC_LP5_tOSCO_nCK);
  } else if (Outputs->IsDdr5) {
    // tMPC_Delay
    Result = MrcGetTmod (MrcData, Outputs->MemoryClock);
  }

  return Result;
}

/**
  This function returns the tWRPRE value.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] tCWL       - WR CAS Latency in tCK.
  @param[in] tWR        - Write recovery time in tCK.

  @returns tWRPRE timing in tCK
**/
UINT32
MrcGetTwrpre (
  IN MrcParameters *const MrcData,
  IN const UINT32         tCWL,
  IN const UINT32         tWTR
)
{
  UINT32 Result = 0;
  MrcOutput   *Outputs = &MrcData->Outputs;

  if (Outputs->IsLpddr5) {
    Result = (tCWL + MrcGetBurstLengthNMin (MrcData) + tWTR + 1);
  } else if (Outputs->IsDdr5) {
    Result = (tCWL + Outputs->BurstLength + tWTR);
  }

  return Result;
}

/**
  This function returns tRRFpb in ps.

  @param[in] MrcData       - Include all MRC global data.
  @param[in] DeviceDensity - Device density in MB.

  @returns tRRFpb timing in ps.
**/
UINT32
MrcGetTrrfpb (
  IN MrcParameters *const MrcData,
  IN const UINT32         DeviceDensity
  )
{
  MrcOutput *Outputs = &(MrcData->Outputs);
  UINT32  tRRFpb;

  switch (Outputs->DdrType) {
    case MRC_DDR_TYPE_LPDDR5:
      tRRFpb = MrcGetLpddr5Trrfpb (DeviceDensity); // in PS
      break;

    default:
      tRRFpb = 0;
      break;
  }

  return tRRFpb;
}

/**
  This function returns tRRFsb in CK.

  @param[in] MrcData       - Include all MRC global data.

  @returns tRRFsb timing in CK.
**/
UINT32
MrcGetTrrfsb (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs = &(MrcData->Outputs);
  UINT32  tRRFsb;

  switch (Outputs->DdrType) {
    case MRC_DDR_TYPE_DDR5:
      tRRFsb =  DIVIDECEIL (MRC_DDR5_tRRFSB_PS, Outputs->tCKps);
      break;

    default:
      tRRFsb = 0;
      break;
  }

  return tRRFsb;
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
  This function determines if NT ODT for WR and/or RD are enabled on any ranks within a channel

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - Current Controller
  @param[in] Channel    - Current Channel
  @param[out] NTODTWr   - TRUE if any rank within the channel supports NT WR ODT
  @param[out] NTODTRd   - TRUE if any rank within the channel supports NT RD ODT
**/
VOID
IsNtOdtSupported (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  OUT BOOLEAN               *NTODTWr,
  OUT BOOLEAN               *NTODTRd
  )
{
  UINT8  OdtTypeIndex;
  UINT8  NtRankIndex;
  UINT8  NtRankSelect[MaxNtOdt];
  UINT8  OdtType[MaxOdtParamType] = {WriteOdt, ReadOdt};
  UINT32 Rank;
  UINT8  OdtParamSelect;

  // Non Target Settings are assumed off, then determined if they should be enabled below
  *NTODTWr = FALSE;
  *NTODTRd = FALSE;
  // Find Worst case ODT offsets among Ranks
  for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
    if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
      continue;
    }
    // Determine if Non Target ODT is supported on any Rank/Dimm
    GetNtRankSelection (Rank, NtRankSelect);
    for (OdtTypeIndex = 0; OdtTypeIndex < 2; OdtTypeIndex++) {
      for (NtRankIndex = 0; NtRankIndex < MaxNtOdt; NtRankIndex++) {
        OdtParamSelect = MrcGetOdtParam (MrcData, Controller, Channel, Rank, OdtType[OdtTypeIndex], NtRankSelect[NtRankIndex]);
        // If any Rank / Dimm supports NTODT, then report NTODT as TRUE
        if (OdtParamSelect == OptDimmOdtNomWr) {
          *NTODTWr = TRUE;
        } else if (OdtParamSelect == OptDimmOdtNomRd) {
          *NTODTRd = TRUE;
        }
      }
    }
  }
}

/**
  This function will set WCK2DQI (LP5) / DQS (DDR5) Interval Timer Run Time to MR37 (LP5) / MR45 (DDR5).

  @param[in]      MrcData       - Pointer to global MRC data.
  @param[in]      DqioDuration  - WCK2DQI/DQS interval timer run time.

  @retval MrcStatus - mrcSuccess if the value is supported, else mrcWrongInputParameter.
**/
MrcStatus
MrcSetIntervalTimerMr (
  IN      MrcParameters *const  MrcData,
  IN      UINT8                 DqioDuration
  )
{
  MrcStatus Status;

  if (MrcData->Outputs.IsDdr5) {
    Status = MrcSetDdr5Mr45 (MrcData, DqioDuration);
  } else {
    Status = MrcSetLp5MR37 (MrcData, DqioDuration);
  }

  return Status;
}

/**
  This function returns TRUE if LP5 frequency is low frequency (<= 3200 MT/s).

  @param[in]      Frequency     - Frequency to check

  @returns TRUE if LP5 frequency is low frequency (<= 3200 MT/s).
**/
BOOLEAN
MrcLpddrIsLowFreq (
  IN MrcFrequency Frequency
  )
{
  if (Frequency <= f3200) {
    return TRUE;
  } else {
    return FALSE;
  }
}
