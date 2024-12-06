/** @file
  This file contains the Row Hammering training algorithm

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
//
// Include files
//

#include "CMrcTypes.h"
#include "CMrcApi.h"
#include "MrcMcSiSpecific.h"
#include "MrcCommon.h"
#include "MrcRowHammer.h"
#include "MrcRowHammerPrivate.h"
#include "MrcDdr5Registers.h"
#include "MrcLpddr5Registers.h"
#include "MrcChipApi.h"
#include "BlueMrcSpdProcessingApi.h"  // MrcGetVendorSwizzling()
#include "BlueMrcJedecApi.h"
#include "MrcMemoryApi.h"
#include "MrcRefreshConfiguration.h"
#include "MrcDdr5.h"
#include "MrcLpddr5.h"
#include "MrcSpdProcessingDefs.h"  // for SdramCapacityTable[]
#include "MrcPmaApi.h"

/**
 * This functions maps MrcRhDrfmBrcType to MrcTrrControlBrcType.
 *
 * @param[in] DrfmBrc BRC value as MrcRhDrfmBrcType
 *
 * @returns Convert BRC value as MrcTrrControlBrcType
**/
MrcTrrControlBrcType
ConvertDrfmBrcToTrrControlBrcType (
  MrcRhDrfmBrcType BrcVal
  )
{
  MrcTrrControlBrcType TrrControlBrcType;

  switch (BrcVal) {
    case DrfmBrc2:
      TrrControlBrcType = TrrControlBrc2;
      break;
    case DrfmBrc3:
      TrrControlBrcType = TrrControlBrc3;
      break;
    case DrfmBrc4:
      TrrControlBrcType = TrrControlBrc4;
      break;
    case DrfmBrcDisabled:
    default:
      TrrControlBrcType = TrrControlBrcDisabled;
  }

  return TrrControlBrcType;
}

/**
  Check if Adaptive Refresh Management (ARFM) is supported on a DIMM.

   @param[in]  MrcData              - Pointer to global MRC data
   @param[in]  Controller           - MC number
   @param[in]  Channel              - Channel number
   @param[in]  Dimm                 - DIMM number
   @param[out] ArfmLevelToConfigure - Maximum supported ARFM Level lower than requested,
                                      only valid if function returns TRUE. Applies to DDR5 only.

   @retval - TRUE if ARFM is supported
   @retval - FALSE if ARFM is not supported
**/
BOOLEAN
MrcIsArfmSupported (
  IN MrcParameters *const MrcData,
  IN UINT32               Controller,
  IN UINT32               Channel,
  IN UINT32               Dimm,
  OUT MrcDramRfmModeType  *ArfmLevelToConfigure
  )
{
  MrcOutput  *Outputs;
  MrcDebug   *Debug;
  BOOLEAN    ArfmSupported;

  Outputs       = &MrcData->Outputs;
  Debug         = &Outputs->Debug;

  switch (Outputs->DdrType) {
    case MRC_DDR_TYPE_DDR5:
      ArfmSupported = MrcDdr5IsArfmSupported (MrcData, Controller, Channel, Dimm, ArfmLevelToConfigure);
      break;

    case MRC_DDR_TYPE_LPDDR5:
      ArfmSupported = MrcLpddr5IsArfmSupported (MrcData, Controller, Channel, Dimm);
      break;

    default:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s, ARFM is not supported\n", gUnsupportedTechnology);
      ArfmSupported = FALSE;
      break;
  }

  return ArfmSupported;
}

/**
  Check if Refresh Management (RFM) is required by checking RFM Required bit in MR27 (LP5) or MR58 (DDR5)

   @param[in]  MrcData         - Pointer to global MRC data.
   @param[in]  Controller      - MC number
   @param[in]  Channel         - Channel number
   @param[in]  Dimm            - Dimm number

   @retval - FALSE means RFM not required
   @retval - TRUE means RFM required
**/
BOOLEAN
MrcIsRfmRequired (
  IN MrcParameters *const MrcData,
  IN UINT32            Controller,
  IN UINT32            Channel,
  IN UINT32            Dimm
  )
{
  MrcOutput  *Outputs;
  MrcDebug   *Debug;
  BOOLEAN    IsRfmRequired;

  Outputs       = &MrcData->Outputs;
  Debug         = &Outputs->Debug;

  switch (Outputs->DdrType) {
    case MRC_DDR_TYPE_DDR5:
      IsRfmRequired =  MrcDdr5IsRfmRequired (MrcData, Controller, Channel, Dimm);
      break;

    case MRC_DDR_TYPE_LPDDR5:
      IsRfmRequired =  MrcLpddr5IsRfmRequired (MrcData, Controller, Channel, Dimm);
      break;

    default:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s, RFM not required\n", gUnsupportedTechnology);
      IsRfmRequired = FALSE;
      break;
  }

  return IsRfmRequired;
}

/**
  This function sets a bit in a BitMap (representing a DIMM) to 1 for that
  DIMM.

  @param[in]  MrcData    - Pointer to global MRC data.
  @param[in]  Channel    - Channel number
  @param[in]  Dimm       - Dimm number
  @param[out] DimmBitMap - Holds if a DIMM is enabled or exists within each bit.
**/
VOID
MrcSetDimmBit (
  IN  MrcParameters *const MrcData,
  IN  UINT32               Channel,
  IN  UINT32               Dimm,
  OUT UINT8                *DimmBitMap
  )
{
  BOOLEAN   IsDdr5;

  IsDdr5 = MrcData->Outputs.IsDdr5;

  if (IsDdr5) {
    *DimmBitMap |= (1 << Dimm);
  } else { // LPDDR5
    if (Channel % 2 == 0) {
      *DimmBitMap |= (1 << dDIMM0);
    } else {
      *DimmBitMap |= (1 << dDIMM1);
    }
  }
}

/**
  This function assigns default values to RfmSetupConfig fields.

  @param[out] RfmSetupConfig  - Holds Rfm Required and Threshold information
**/
VOID
MrcRfmSetupConfigInit (
  OUT MRC_RFM_SETUP_CONFIG *RfmSetupConfig
  )
{
  *RfmSetupConfig = (MRC_RFM_SETUP_CONFIG) {
    .RfmDimmRequiredMap = 0,
    .Raaimt             = MC0_CH0_CR_ROWHAMMER_CTL_RH_LOW_WM_MAX,
    .Raammt             = MC0_CH0_CR_ROWHAMMER_CTL_RH_HIGH_WM_MAX,
    .RefSub             = MC0_CH0_CR_ROWHAMMER_CTL_REFM_SUB_MAX,
    .NormalRefSub       = MC0_CH0_CR_ROWHAMMER_CTL_NORMAL_REF_SUB_MAX
  };
}

/**
  This function checks whether ARFM was requested based on a BIOS knob value.

  @param[in]  MrcData - Pointer to global MRC data.

  @retval - FALSE means ARFM was not requested
  @retval - TRUE means ARFM was requested
**/
BOOLEAN
MrcIsArfmRequested (
  IN MrcParameters *const  MrcData
  )
{
  MrcInput             *Inputs;
  MRC_EXT_INPUTS_TYPE  *ExtInputs;
  BOOLEAN              ArfmRequested;

  Inputs     = &MrcData->Inputs;
  ExtInputs  = Inputs->ExtInputs.Ptr;
  ArfmRequested = FALSE;

  if (ExtInputs->DramRfmMode > Rfm && ExtInputs->DramRfmMode < DramRfmDisabled) {
    ArfmRequested = TRUE;
  }

  return ArfmRequested;
}

/**
  Grab Refresh Management (RFM) threshold information.
  All of these threshold values are used in operations on Rolling Accumulated ACT (RAA) counter.

   @param[in]  MrcData         - Pointer to global MRC data.
   @param[in]  Controller      - MC number
   @param[in]  Channel         - Channel number
   @param[in]  Dimm            - Dimm number
   @param[out] RfmSetupConfig  - Holds Rfm Required and Threshold information

   @retval - mrcUnsupportedTechnology if not DDR5 or LP5
   @retval - mrcSuccess if RFM threshold info gathered successfully
**/
MrcStatus
MrcGetRfmThresholdInfo (
  IN MrcParameters *const  MrcData,
  IN UINT32                Controller,
  IN UINT32                Channel,
  IN UINT32                Dimm,
  OUT MRC_RFM_SETUP_CONFIG *RfmSetupConfig
  )
{
  MrcStatus  Status;
  MrcOutput  *Outputs;
  MrcDebug   *Debug;

  Outputs       = &MrcData->Outputs;
  Debug         = &Outputs->Debug;

  switch (Outputs->DdrType) {
    case MRC_DDR_TYPE_DDR5:
      Status = MrcDdr5GetRfmThresholdInfo (MrcData, Controller, Channel, Dimm, RfmSetupConfig);
      break;

    case MRC_DDR_TYPE_LPDDR5:
      Status = MrcLpddr5GetRfmThresholdInfo (MrcData, Controller, Channel, Dimm, RfmSetupConfig);
      break;

    default:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s, Cannot get RFM threshold info\n", gUnsupportedTechnology);
      Status = mrcUnsupportedTechnology;
      break;
  }

  return Status;
}

/**
  If RFM is required: The RFM threshold values (used in operations on Rolling
  Accumulated ACT (RAA) counter) for that DIMM for RFM will be configured.

  @param[in]  MrcData        - Pointer to global MRC data.
  @param[in]  Controller     - MC number
  @param[in]  Channel        - Channel number
  @param[in]  Dimm           - Dimm number
  @param[out] RfmSetupConfig - Holds Rfm Required and Threshold information

  @retval - mrcUnsupportedTechnology if not DDR5 or LP5
  @retval - mrcSuccess if RFM threshold info gathered successfully
*/
MrcStatus
MrcSetRfmThreshold (
  IN  MrcParameters *const MrcData,
  IN  UINT32               Controller,
  IN  UINT32               Channel,
  IN  UINT32               Dimm,
  OUT MRC_RFM_SETUP_CONFIG *RfmSetupConfig
  )
{
  MrcStatus Status;

  Status = mrcSuccess;

  // Need to get the minimum across all DIMMs as per spec
  Status = MrcGetRfmThresholdInfo (MrcData, Controller, Channel, Dimm, RfmSetupConfig);
  if (Status != mrcSuccess) {
    return Status;
  }

  RfmSetupConfig->Raaimt       = MIN (RfmSetupConfig->Raaimt, RfmSetupConfig->RaaimtOneDimm);
  RfmSetupConfig->Raammt       = MIN (RfmSetupConfig->Raammt, RfmSetupConfig->RaammtOneDimm);
  RfmSetupConfig->RefSub       = MIN (RfmSetupConfig->RefSub, RfmSetupConfig->RefSubOneDimm);
  RfmSetupConfig->NormalRefSub = MIN (RfmSetupConfig->NormalRefSub, RfmSetupConfig->NormalRefSubOneDimm);

  return Status;
}

/**
  Check if Directed Refresh Management (DRFM) is supported.

   @param[in]  MrcData        - Pointer to global MRC data.
   @param[in]  Controller     - MC number
   @param[in]  Channel        - Channel number
   @param[in]  Dimm           - Dimm number

   @retval - FALSE mean DRFM is not supported
   @retval - TRUE means DRFM is supported
**/
BOOLEAN
MrcIsDrfmSupported (
  IN MrcParameters *const MrcData,
  IN UINT32            Controller,
  IN UINT32            Channel,
  IN UINT32            Dimm
  )
{
  MrcOutput  *Outputs;
  MrcDebug   *Debug;
  BOOLEAN    IsDrfmSupported;

  Outputs       = &MrcData->Outputs;
  Debug         = &Outputs->Debug;

  switch (Outputs->DdrType) {
    case MRC_DDR_TYPE_DDR5:
      IsDrfmSupported = MrcDdr5IsDrfmSupported (MrcData, Controller, Channel, Dimm);
      break;

    case MRC_DDR_TYPE_LPDDR5:
      IsDrfmSupported = MrcLpddr5IsDrfmSupported (MrcData, Controller, Channel, Dimm);
      break;

    default:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s, DRFM not supported\n", gUnsupportedTechnology);
      IsDrfmSupported = FALSE;
      break;
  }

  return IsDrfmSupported;
}

/**
   Configure Adaptive Refresh Management (ARFM) in DRAM.

   @param[in]  MrcData              - Pointer to global MRC data
   @param[in]  Controller           - MC number
   @param[in]  Channel              - Channel number
   @param[in]  Dimm                 - Dimm number
   @param[in]  ArfmLevelToConfigure - ARFM level to configure in DRAM

   @retval - mrcUnsupportedTechnology if technology not supported
   @retval - mrcSuccess if ARFM configured correctly
**/
MrcStatus
MrcConfigArfm (
  IN  MrcParameters* const MrcData,
  IN  UINT32               Controller,
  IN  UINT32               Channel,
  IN  UINT32               Dimm,
  IN  MrcDramRfmModeType   ArfmLevelToConfigure
  )
{
  MrcStatus  Status;
  MrcOutput  *Outputs;
  MrcDebug   *Debug;

  Outputs       = &MrcData->Outputs;
  Debug         = &Outputs->Debug;

  switch (Outputs->DdrType) {
    case MRC_DDR_TYPE_DDR5:
      Status = MrcDdr5ConfigArfm (MrcData, Controller, Channel, Dimm, ArfmLevelToConfigure);
      break;

    case MRC_DDR_TYPE_LPDDR5:
      Status = MrcLpddr5ConfigArfm (MrcData, Controller, Channel, Dimm);
      break;

    default:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s, ARFM cannot be configured.\n", gUnsupportedTechnology);
      Status = mrcUnsupportedTechnology;
      break;
  }

  if (Status == mrcSuccess) {
    MrcData->Save.Data.IsRowHammerConfigured[Controller][Channel][Dimm] = TRUE;
  }

  return Status;
}

/**
  Configure Refresh Management (RFM) on the given controller / channel

   @param[in] MrcData        - Pointer to global MRC data.
   @param[in] Controller     - MC index
   @param[in] Channel        - Channel index
   @param[in] RfmSetupConfig - Holds Rfm Required and Threshold information
**/
VOID
MrcConfigRfm (
  IN MrcParameters *const MrcData,
  IN UINT8  Controller,
  IN UINT8  Channel,
  IN MRC_RFM_SETUP_CONFIG *RfmSetupConfig
  )
{
  MrcDebug        *Debug;
  MrcOutput       *Outputs;
  INT64           GetSetVal;
  INT64           GetSetEn;

  Outputs   = &MrcData->Outputs;
  Debug     = &Outputs->Debug;
  GetSetEn  = 1;

  // Setting to 1 if bit0 in RfmDimmRequiredMap was set
  if (RfmSetupConfig->RfmDimmRequiredMap & (1 << dDIMM0)) {
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRhEnableDimm0, WriteToCache | PrintValue, &GetSetEn);
  }

  // Setting to 1 if bit1 in RfmDimmRequiredMap was set
  if (RfmSetupConfig->RfmDimmRequiredMap & (1 << dDIMM1)) {
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRhEnableDimm1, WriteToCache | PrintValue, &GetSetEn);
  }

  if (RfmSetupConfig->RfmDimmRequiredMap) {
    GetSetVal = RfmSetupConfig->Raammt;
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRhHighWM, WriteToCache | PrintValue, &GetSetVal);
    GetSetVal = RfmSetupConfig->Raaimt;
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRhLowWM, WriteToCache | PrintValue, &GetSetVal);
    GetSetVal = RfmSetupConfig->NormalRefSub;
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRhNormalRefSub, WriteToCache | PrintValue, &GetSetVal);
    GetSetVal = RfmSetupConfig->RefSub;
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRhRefmSub, WriteToCache | PrintValue, &GetSetVal);
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRhRefmEn, WriteToCache | PrintValue, &GetSetEn);
  }
  MrcFlushRegisterCachedData (MrcData);

  MRC_DEBUG_MSG (Debug,
    MSG_LEVEL_NOTE,
    "RAAIMT: 0x%x, RAAMMT: 0x%x, NORMAL_REF_SUB: 0x%x, REF_SUB: 0x%x\n",
    RfmSetupConfig->Raaimt,
    RfmSetupConfig->Raammt,
    RfmSetupConfig->NormalRefSub,
    RfmSetupConfig->RefSub
  );
}

/**
  Configure Directed Refresh Management (DRFM) Blast Radius Configuration (BRC) in MR75 (LP5) or MR59 (DDR5)
  Configure DRFM on given Conroller/Channel

   @param[in]  MrcData          - Pointer to global MRC data
   @param[in]  Controller       - MC index
   @param[in]  Channel          - Channel index
   @param[in]  DimmMap          - Bitmap of present dimms of the Channel
   @param[in]  DrfmEnableBits   - 2-bit bit mask indicating which DIMMs support DRFM

   @retval - mrcUnsupportedTechnology if DDR technology unsupported
   @retval - mrcSuccess if DRFM configured properly
**/
MrcStatus
MrcConfigDrfm (
  IN MrcParameters *const MrcData,
  IN UINT8                Controller,
  IN UINT8                Channel,
  IN UINT8                DimmMap,
  IN UINT8                DrfmEnableBits
  )
{
  MrcStatus     Status;
  MrcOutput     *Outputs;
  MrcDebug      *Debug;
  INT64         GetSetVal;
  UINT8         BrcValue;
  UINT8         Dimm;
  MrcChannelOut *ChannelOut;
  BOOLEAN       IsBrcSupported[MAX_DIMMS_IN_CHANNEL];
  UINT32        MaxDeviceDensityInChannel;
  BOOLEAN       IsDrfmEnabled;
  INT64         tDRFM;
  UINT32        tRRF;

  Outputs    = &MrcData->Outputs;
  Debug      = &Outputs->Debug;
  ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
  Status     = mrcSuccess;

  // Configure DRFM Enable and Blast Radius Configuration in MRs
  for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
    if ((DimmMap & (1 << Dimm)) == 0) {
      continue;
    }

    switch (Outputs->DdrType) {
      case MRC_DDR_TYPE_DDR5:
        Status = MrcDdr5ConfigDrfm (MrcData, Controller, Channel, Dimm, &IsBrcSupported[Dimm]);
        break;

      case MRC_DDR_TYPE_LPDDR5:
        Status = MrcLpddr5ConfigDrfm (MrcData, Controller, Channel, Dimm, &IsBrcSupported[Dimm]);
        break;

      default:
        // No sense in continuing with function technology is not supported
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s, DRFM cannot be configured!\n", gUnsupportedTechnology);
        Status = mrcUnsupportedTechnology;
        break;
    }

    if (Status != mrcSuccess) {
      return Status;
    }

    // MR75/MR59 (LP5/DDR5) is updated so we treat it as configured RowHammer
    MrcData->Save.Data.IsRowHammerConfigured[Controller][Channel][Dimm] = TRUE;

    if ((1 << Dimm) & DrfmEnableBits) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "DRFM enabled on MC%u C%u Dimm%u\n", Controller, Channel, Dimm);
    } else {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "pTRR enabled on MC%u C%u Dimm%u\n", Controller, Channel, Dimm);
    }
  }

  // 0x0: both DIMMs disabled for DRFM, 0x1: Dimm0 Enabled for DRFM
  // 0x2: Dimm1 Enabled for DRFM,       0x3: both DIMMs enabled for DRFM
  if (DrfmEnableBits <= DRFM_ENABLED_BOTH_DIMMS) {
    IsDrfmEnabled = FALSE;
    GetSetVal = DrfmEnableBits;
    BrcValue = DrfmBrc2; // By default it is BRC2
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRhDrfmEnable, WriteCached | PrintValue, &GetSetVal);
    for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
      if ((DimmMap & (1 << Dimm)) == 0) {
        continue;
      }
      IsDrfmEnabled = TRUE;
      if (IsBrcSupported[Dimm]) {
        BrcValue = Outputs->DrfmBrc;
        GetSetVal = ConvertDrfmBrcToTrrControlBrcType (BrcValue);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccBrc, WriteCached | PrintValue, &GetSetVal);
        break;
      }
    }

    if (IsDrfmEnabled) {
      if (Outputs->DdrType == MRC_DDR_TYPE_LPDDR5) {
        MaxDeviceDensityInChannel = 0;
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++){
          if (ChannelOut->Dimm[Dimm].Status == DIMM_PRESENT) {
            MaxDeviceDensityInChannel = MAX (MaxDeviceDensityInChannel, SdramCapacityTable[ChannelOut->Dimm[Dimm].DensityIndex] * 8);
          }
        }
        tRRF = MrcGetTrrfpb (MrcData, MaxDeviceDensityInChannel);
      } else { // DDR5
        tRRF = MrcGetTrrfsb (MrcData);
      }

      // If BRC is disabled then tDRFM = 4*tRRF, so to calculate tDRFM,
      // the default BRC2 value is used to meet this requirement.
      tDRFM = 2 * tRRF * BrcValue;

      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMctDRFM, WriteCached | PrintValue, &tDRFM);
    }
  } else {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Bitmask of enabled DIMMs invalid: 0x%X", DrfmEnableBits);
  }

  // MrcConfigPtrr() calls MrcFlushRegisterCachedData()
  // The remaining configuration for DRFM and pTRR is the same
  MrcConfigPtrr (MrcData, Controller, Channel, DimmMap);

  return Status;
}

/**
  Configure Pseudo Targeted Row Refresh (pTRR) on the given controller / channel

   @param[in]  MrcData      - Pointer to global MRC data.
   @param[in]  Controller   - MC index
   @param[in]  Channel      - Channel index
   @param[in]  DimmMap      - Bitmap of present DIMMs of the Channel
**/
VOID
MrcConfigPtrr (
  IN MrcParameters *const MrcData,
  IN UINT8                Controller,
  IN UINT8                Channel,
  IN UINT8                DimmMap
  )
{
  MrcInput             *Inputs;
  INT64                GetSetVal;
  UINT32               MA1Swizzle;
  UINT32               MA2Swizzle;
  UINT8                Dimm;
  MRC_EXT_INPUTS_TYPE  *ExtInputs;

  Inputs     = &MrcData->Inputs;
  ExtInputs  = Inputs->ExtInputs.Ptr;
  MA2Swizzle = 0;
  MA1Swizzle = 0;

  GetSetVal = 0x3;  // Both DIMMs should be configured to the same value
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRhTRRDimmEnable, WriteToCache | PrintValue, &GetSetVal);

  GetSetVal = ExtInputs->Lfsr0Mask;
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRhLfsr0Mask, WriteToCache | PrintValue, &GetSetVal);
  GetSetVal = ExtInputs->Lfsr1Mask;
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRhLfsr1Mask, WriteToCache | PrintValue, &GetSetVal);

  for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
    if (DimmMap & (1 << Dimm)) {
      MrcGetVendorSwizzling (MrcData, Controller, Channel, Dimm, 1 << Dimm, &MA1Swizzle, &MA2Swizzle);
    }
  }
  GetSetVal = MA1Swizzle;
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRhMA1Swizzle, WriteToCache | PrintValue, &GetSetVal);
  GetSetVal = MA2Swizzle;
  MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRhMA2Swizzle, WriteToCache | PrintValue, &GetSetVal);

  if (Inputs->RHMASwizzlingOverride) {
    GetSetVal = Inputs->RHMA1Swizzling;
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRhMA1Swizzle, WriteToCache | PrintValue, &GetSetVal);
    GetSetVal = Inputs->RHMA2Swizzling;
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRhMA2Swizzle, WriteToCache | PrintValue, &GetSetVal);
  }
  if (Inputs->RHMicronSwizzlingOverride) {
    GetSetVal = Inputs->RHMicronFSwizzling;
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccMicronFSwizzle, WriteToCache | PrintValue, &GetSetVal);
    GetSetVal = Inputs->RHMicronRSwizzling;
    MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccMicronRSwizzle, WriteToCache | PrintValue, &GetSetVal);
  }

  MrcFlushRegisterCachedData (MrcData);
}

/**
  Configure Row Hammer Ptrr Lfsr, should be called for any boot mode
  @param[in] MrcData - The MRC general data.
**/
VOID
MrcRhPtrrLfsrConfig (
  IN MrcParameters *const MrcData
  )
{
  const MrcInput      *Inputs;
  const MRC_FUNCTION  *MrcCall;
  MrcOutput           *Outputs;
  UINT32              RandomNumber;
  UINT32              Controller;
  UINT32              Channel;
  INT64               LfsrSeed;
  BOOLEAN             IsLpddr;

  Outputs     = &MrcData->Outputs;
  IsLpddr     = Outputs->IsLpddr;
  Inputs      = &MrcData->Inputs;
  MrcCall     = Inputs->Call.Func;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      if (!(MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (IsLpddr, Channel)) {
        continue;
      }

      MrcCall->MrcGetRandomNumber (&RandomNumber);
      LfsrSeed = RandomNumber;
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRhLfsr0, WriteNoCache | PrintValue, &LfsrSeed);

      MrcCall->MrcGetRandomNumber (&RandomNumber);
      LfsrSeed = RandomNumber;
      MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRhLfsr1, WriteNoCache | PrintValue, &LfsrSeed);
    } // for Channel
  } // for Controller
}

/**
  Configure Row Hammer mitigation modes if they are supported and enabled

  @param[in] MrcData - The MRC general data.

  @retval - mrcSuccess if nothing went wrong while checking and configuring Row Hammer Mitigation Options
**/
MrcStatus
MrcRhPrevention (
  IN MrcParameters *const MrcData
  )
{
  MrcStatus            Status;
  const MrcInput       *Inputs;
  MrcDebug             *Debug;
  MrcOutput            *Outputs;
  UINT8                Controller;
  UINT8                Channel;
  UINT8                IpChannel;
  UINT8                Dimm;
  BOOLEAN              IsLpddr;
  UINT8                PopulatedDimmMask;
  UINT8                DrfmDimmSupportedMap;
  BOOLEAN              ArfmSupported;
  BOOLEAN              RfmRequired;
  BOOLEAN              DrfmSupported;
  BOOLEAN              ArfmConfigured;
  BOOLEAN              DrfmConfigured;
  BOOLEAN              PtrrConfigured;
  INT64                PanicWm;
  INT64                HpWm;
  MRC_RFM_SETUP_CONFIG RfmSetupConfig;
  BOOLEAN              IsMcSubChannel;

  const MRC_EXT_INPUTS_TYPE *ExtInputs;

  Status              = mrcSuccess;
  Inputs              = &MrcData->Inputs;
  ExtInputs           = Inputs->ExtInputs.Ptr;
  Outputs             = &MrcData->Outputs;
  Debug               = &Outputs->Debug;
  IsLpddr             = Outputs->IsLpddr;

  RfmRequired   = FALSE;
  ArfmSupported = FALSE;
  ArfmConfigured = FALSE;
  DrfmConfigured = FALSE;
  PtrrConfigured = FALSE;

  PopulatedDimmMask    = 0;
  DrfmDimmSupportedMap = 0;

  MrcRfmSetupConfigInit (&RfmSetupConfig);

  MrcDramRfmModeType ArfmLevelToConfigure = ArfmLevelA;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RH mode: ");
  MrcPrintDramRfmMode (MrcData);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RH mode: DRFM %srequested.\n", (ExtInputs->TargetedRowRefreshMode == Drfm) ? "" : "is not ");
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RH mode: pTRR %srequested.\n", (ExtInputs->TargetedRowRefreshMode == Ptrr) ? "" : "is not ");

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if (!MrcChannelExist (MrcData, Controller, Channel)) {
        continue;
      }

      IsMcSubChannel = IS_MC_SUB_CH (IsLpddr, Channel);
      if (!IsMcSubChannel) {
        PopulatedDimmMask    = 0;
        DrfmDimmSupportedMap = 0;
        MrcRfmSetupConfigInit (&RfmSetupConfig);
      }

      for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
        if (Outputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Status != DIMM_PRESENT) {
          continue;
        }

        MrcSetDimmBit (MrcData, Channel, Dimm, &PopulatedDimmMask);

        // Both RfmRequired and ArfmSupported need to be reset each iteration
        // Some DIMMs may not be Require RFM or Support ARFM
        RfmRequired   = FALSE;
        ArfmSupported = FALSE;
        // Clearing all the flags related to options in each iteration
        ArfmConfigured = FALSE;
        DrfmConfigured = FALSE;
        PtrrConfigured = FALSE;

        RfmRequired = MrcIsRfmRequired (MrcData, Controller, Channel, Dimm);
        if (RfmRequired) {
          MrcSetDimmBit (MrcData, Channel, Dimm, &RfmSetupConfig.RfmDimmRequiredMap);
        } else {
          ArfmSupported = MrcIsArfmSupported (MrcData, Controller, Channel, Dimm, &ArfmLevelToConfigure);
        }

        if (RfmRequired || ArfmSupported) {
          if (MrcIsArfmRequested (MrcData) ||
              (ArfmSupported && ExtInputs->TargetedRowRefreshMode == Drfm)) {
            // Only check ARFM MRs once for supported check (already checked
            // them above if RFM is not required) per DIMM
            if (RfmRequired && !ArfmSupported) {
              ArfmSupported = MrcIsArfmSupported (MrcData, Controller, Channel, Dimm, &ArfmLevelToConfigure);
            }

            if (ArfmSupported) {
              // To configure ARFM, we also need to set values for RFM
              // Force RFM flag = 1 for this DIMM
              RfmRequired = TRUE;
              MrcSetDimmBit (MrcData, Channel, Dimm, &RfmSetupConfig.RfmDimmRequiredMap);
              Status = MrcConfigArfm (MrcData, Controller, Channel, Dimm, ArfmLevelToConfigure);
              if (Status != mrcSuccess) {
                return Status;
              }
              ArfmConfigured = TRUE;
              MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "ARFM Configured\n");
            }
          }
        }

        if (RfmRequired) {
          Status = MrcSetRfmThreshold (MrcData, Controller, Channel, Dimm, &RfmSetupConfig);
          if (Status != mrcSuccess) {
            return Status;
          }
        }

        // Check if DRFM supported on each DIMM
        DrfmSupported = MrcIsDrfmSupported (MrcData, Controller, Channel, Dimm);

        if (DrfmSupported) {
          MrcSetDimmBit (MrcData, Channel, Dimm, &DrfmDimmSupportedMap);
        }
      } // for Dimm

      if ((PopulatedDimmMask == 0) || IsMcSubChannel) {
        //  MC registers are per big ch0/1 only
        //  As for LPDDR, we need check sub channel both 0 and 1,
        //  or both 2 and 3, before going ahead to configure RHCTL or PTRR.
        continue;
      }

      //  LPDDR5:
      //    When execution comes here, Channel is an odd number(1 or 3).
      //    Sub Channel 0/1 and 2/3 share the same MC registers
      //    MC registers are per big Ch0/1 only
      //    Within GetSet we map Ch0 to MC Ch0 and Ch2 to MC Ch1.
      //  DDR5:
      //    Just use the Channel number.
      IpChannel = Channel & (IsLpddr ? 0xFE : 0xFF);

      if (ExtInputs->TargetedRowRefreshMode != TrrDisabled) {
        // If DrfmDimmSupportedMap is 0x1 or 0x2, the non-DRFM DIMM will use pTRR
        DrfmConfigured = FALSE;
        if (ExtInputs->TargetedRowRefreshMode == Drfm && DrfmDimmSupportedMap > DRFM_DISABLED_BOTH_DIMMS && ArfmConfigured) {
          Status = MrcConfigDrfm (MrcData, Controller, IpChannel, PopulatedDimmMask, DrfmDimmSupportedMap);
          if (Status != mrcSuccess) {
            return Status;
          }
          DrfmConfigured = TRUE;
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "DRFM Configured\n");
        }

        // Configure pTRR if DRFM disabled or not supported on both DIMMs
        if (!DrfmConfigured) {
          MrcConfigPtrr (MrcData, Controller, IpChannel, PopulatedDimmMask);
          // Wm optimization based on testing results
          PanicWm = MIN (REFRESH_PANIC_WM_HIGH, Outputs->RefreshPanicWm);
          HpWm = MIN (REFRESH_HP_WM_HIGH, Outputs->RefreshHpWm);

          // Set REFRESH_PANIC_WM and REFRESH_HP_WM to support watermark finer granularity.
          MrcGetSetMcCh (MrcData, Controller, IpChannel, GsmMctRefreshPanicWm, WriteToCache | PrintValue, &PanicWm);
          MrcGetSetMcCh (MrcData, Controller, IpChannel, GsmMctRefreshHpWm, WriteToCache | PrintValue, &HpWm);
          // Configure RefreshAbrRelease with the same value like RefreshHpWm
          MrcGetSetMcCh (MrcData, Controller, IpChannel, GsmMccRefreshAbrRelease, WriteToCache | PrintValue, &HpWm);
          MrcFlushRegisterCachedData (MrcData);
          PtrrConfigured = TRUE;
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "pTRR Configured\n");
        }
      }

      // Adjusting the RAAMMT value and configuring RFM is at the end of the flow
      // because to change the RAAMMT value we need to know whether pTRR was enabled
      if (RfmRequired) {
        if (PtrrConfigured) {
          RfmSetupConfig.Raammt = SUBTRACT_NO_OVERFLOW (RfmSetupConfig.Raammt, 3);
        } else {
          RfmSetupConfig.Raammt = (IsLpddr) ? SUBTRACT_NO_OVERFLOW (RfmSetupConfig.Raammt, 2) : SUBTRACT_NO_OVERFLOW (RfmSetupConfig.Raammt, 1);
        }

        MrcConfigRfm (MrcData, Controller, IpChannel, &RfmSetupConfig);
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RFM Configured\n");
      }

      MrcFullRankBlockOptimization (MrcData, Controller, IpChannel);

    } // for Channel
  }  // for Controler

  if (PtrrConfigured || DrfmConfigured ) {
    MrcPmaSetPtrrSeed(MrcData);
  }

  return Status;
}

/**
  Print Adaptive Refresh Management (ARFM) Level according to JEDEC Spec

  @param[in] MrcData - The MRC general data.
**/
VOID
MrcPrintDramRfmMode (
  IN MrcParameters *const MrcData
  )
{
  const MrcInput  *Inputs;
  MrcOutput       *Outputs;
  MrcDebug        *Debug;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  switch (Inputs->ExtInputs.Ptr->DramRfmMode) {
    case Rfm:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Default RFM\n");
      break;
    case ArfmLevelA:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "ARFM Level A\n");
      break;
    case ArfmLevelB:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "ARFM Level B\n");
      break;
    case ArfmLevelC:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "ARFM Level C\n");
      break;
    case DramRfmDisabled:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "ARFM/RFM disabled\n");
      break;
    default:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Invalid input: %u for ARFM Level\n", Inputs->ExtInputs.Ptr->DramRfmMode);
      break;
  }
}

