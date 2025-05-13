/** @file
  This file declares the PMA interface and holds the project PMA specific
  changes.

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

#include "MrcHalApi.h"
#include "MrcGeneral.h"
#include "MrcPmaApi.h"
#include "CMrcTypes.h"
#include "BlueMrcSpdProcessingApi.h" // for ConvertFreq2Clock()
#include "MrcCommon.h"
#include "MrcGeneral.h"
#include "MrcSagv.h"
#include "CMcAddress.h"

/**
  Sends PM14 message to MC using MemSS PMA.

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess PM14 message sending is successful
  @retval mrcDeviceBusy if poll InitComplete hits timeout
**/
MrcStatus
MrcMcPm14Message (
  IN MrcParameters* const MrcData
  )
{
  const MrcInput      *Inputs;
  const MRC_FUNCTION  *MrcCall;
  INT64               GetSetVal;
  UINT32              FreqIndex;
  MrcDebug            *Debug;
  MrcOutput           *Outputs;
  UINT64              Timeout;
  BOOLEAN             Busy;

  Inputs      = &MrcData->Inputs;
  MrcCall     = MrcData->Inputs.Call.Func;
  Outputs     = &MrcData->Outputs;
  Debug       = &Outputs->Debug;

  // Save the current point
  GetSetVal = 1;
  FreqIndex = Outputs->SaGvPoint;

  // PM14 is issued by writing to MEMSS_PMA_CR_INIT_STATE_REG.MrcSave
  MrcGetSetFreqIndex (MrcData, FreqIndex, GsmPmaMrcSave, WriteNoCache, &GetSetVal);

  // Poll for acknowledgment
  Timeout = MrcCall->MrcGetCpuTime () + MRC_WAIT_TIMEOUT; // 10 seconds timeout
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "MC PM14: Waiting for SAGV %d point save acknowledge\n", FreqIndex);
  do {
    MrcGetSetFreqIndex (MrcData, FreqIndex, GsmPmaMrcSave, ReadNoCache, &GetSetVal);
    Busy = (GetSetVal == 1);
    if (Inputs->ExtInputs.Ptr->SimicsFlag == 1) {
      Busy = FALSE;
    }
  } while (Busy && (MrcCall->MrcGetCpuTime () < Timeout));

  if (Busy) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s - %s\n", gErrString, gTimeout);
    return mrcDeviceBusy;
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Done\n");
  return mrcSuccess;
}

/**
  This function checks the DDRSZ filed.

  @param[in, out] MrcData - All the MRC global data.
  @param[in] PmaMcCapData - The value of MEMSS_PMA_CR_CAPABILITIES_MISC_STRUCT.

  @retval mrcSuccess memory controller's capability has been determined
  @retval mrcFail memory controller's capability was not determined
**/
MrcStatus
MrcMcCheckDDRSZ (
  IN OUT MrcParameters *const MrcData,
  IN     UINT32         PmaMcCapData
  )
{
  MrcOutput                              *Outputs;
  MEMSS_PMA_CR_CAPABILITIES_MISC_STRUCT  PmaMcCap;

  Outputs     = &MrcData->Outputs;

  PmaMcCap.Data      = PmaMcCapData;
  // Determine the maximum size of memory per channel, based on PMA
  switch (PmaMcCap.Bits.DDRSZ) {
    case tcs64GB:
      Outputs->MrcTotalChannelLimit = (64 * 1024);
      break;

    case tcs8GB:
      Outputs->MrcTotalChannelLimit = (8 * 1024);
      break;

    case tcs4GB:
      Outputs->MrcTotalChannelLimit = (4 * 1024);
      break;

    case tcs2GB:
    default:
      Outputs->MrcTotalChannelLimit = (2 * 1024);
      break;
  }
  return mrcSuccess;
}

/**
  This function reads the PMA capacity register and sets the following values
  according to memory controller's capability and user input:
    Outputs->FreqMax
    Outputs->MemoryClockMax

  @param[in, out] MrcData - All the MRC global data.

  @retval mrcSuccess listed values has been set
  @retval mrcFail listed values was not set
**/
MrcStatus
MrcMcCapabilityPreSpd (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput                *Inputs;
  const MRC_EXT_INPUTS_TYPE     *ExtInputs;
  MrcDebug                      *Debug;
  MrcOutput                     *Outputs;
  MrcFrequency                  FreqMax;
  MrcClockRatio                 MaxRatio;
  MrcDdrType                    DdrType;
  MrcStatus                     Status;
  UINT32                        MaxFreqCap;
  UINT32                        MaxFreq;
  UINT32                        MaxQclkFreq;
  UINT32                        MaxQclkFreqRatio;
  UINT32                        MaxQclkDataRate;
  BOOLEAN                       OverclockCapable;
  MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_STRUCT PmaFuseTechEn;
  MEMSS_PMA_CR_CAPABILITIES_MISC_STRUCT        PmaMcCap;

  Inputs     = &MrcData->Inputs;
  ExtInputs  = Inputs->ExtInputs.Ptr;
  Outputs    = &MrcData->Outputs;
  Debug      = &Outputs->Debug;
  DdrType    = Outputs->DdrType;
  Status     = mrcSuccess;

  PmaFuseTechEn.Data = MrcReadCR (MrcData, MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_REG);
  PmaMcCap.Data = MrcReadCR (MrcData, MEMSS_PMA_CR_CAPABILITIES_MISC_REG);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "PmaFuseTechEn: 0x%X\nPmaMcCap: 0x%X\n", PmaFuseTechEn.Data, PmaMcCap.Data);

  // Determine the maximum memory frequency supported and the memory reference clock.
  switch (DdrType) {
    case MRC_DDR_TYPE_LPDDR5:
      MaxFreqCap = PmaFuseTechEn.Bits.MAX_F_LPDDR5;
      break;

    case MRC_DDR_TYPE_DDR5:
      MaxFreqCap = PmaFuseTechEn.Bits.MAX_F_DDR5;
      break;

    default:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Invalid DDR Type detected, Setting MaxFreq to 0\n");
      MaxFreqCap = 0x0;
      break;
  }

  if (MaxFreqCap == 0) {
    // 0 means unlimited - Default to the max supported frequency
    MaxFreq = f15067;
  } else {
    MaxFreq = MrcRatioToFrequency (MrcData, (UINT16) MaxFreqCap);
  }

  if (PmaMcCap.Bits.MAX_F_QCLK == 0) {
    // 0 means unlimited - Default to largest possible QCLK
    MaxQclkFreqRatio = 0x48; // 9600, QCLK 2400 at Gear4
  } else {
    MaxQclkFreqRatio = PmaMcCap.Bits.MAX_F_QCLK;
    if (MaxQclkFreqRatio > 0x48) {
      MaxQclkFreqRatio = 0x48; // add clipping
    }
  }
  MaxQclkFreq = MrcRatioToFrequency (MrcData, (UINT16) MaxQclkFreqRatio);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "MaxQclkFreq = %d\n", MaxQclkFreq);

  // Print DMFC decoding. 0 means Unlimited
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "DMFC: %u\n", MaxFreq);

  OverclockCapable = ((PmaMcCap.Bits.DDR_OC_EN > 0) ? TRUE : FALSE) && Outputs->IsOCProfile;

  // Now use the RefClk to determine the Max Ratio/Freq
  // Start by using FreqMax to determine what is the max frequency is based on the RefClk we found.
  // If OverclockCapable is set, this will be our limit.
  if (OverclockCapable) {
    FreqMax = MAX_FREQ_OC;
  } else {
    FreqMax = MaxFreq;
  }
  Outputs->FreqMax = ((Inputs->FreqMax > fNoInit) && (Inputs->FreqMax < fInvalid)) ? Inputs->FreqMax : FreqMax;
  if (Inputs->FreqMax == fNoInit) {
    Status = MrcBoardAndSkuFreqCheck (MrcData); // Limit Outputs->FreqMax by board layout if needed
    if (Status != mrcSuccess) {
      return Status;
    }
    if (Outputs->IsLpddr5) {
      if (Inputs->IsDdrIoB0) {
        Outputs->FreqMax = MIN (Outputs->FreqMax, f8533); // Default SAGV: 2400, 4800, 6400, 8533 (all G4)
      } else {
        Outputs->FreqMax = MIN (Outputs->FreqMax, f7467); // Default SAGV: 2400, 4800, 6400, 7467 (all G4)
      }
    } else { // DDR5
      Outputs->FreqMax = MIN (Outputs->FreqMax, f6400); // Default SAGV: 3200, 4800, 5600, 6400 (all G4)
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Auto FreqMax resolved to: %u\n", Outputs->FreqMax);
  }

  if (!OverclockCapable) {
    // Use the ratio limit from PMA to determine the MaxFreq
    MaxRatio = (MrcClockRatio) ((MaxFreqCap == 0) ? MEMSS_PMA_CR_CAPABILITIES_TECH_EN_MAX_F_MAX_F_DDR5_MAX : MaxFreqCap);
    FreqMax  = MrcRatioToFrequency (MrcData, MaxRatio);
  }
  // Now check if the CPU is the limiter of the frequency, and apply that limit over user/DIMM request.
  if (FreqMax < Outputs->FreqMax) {
    Outputs->FreqMax  = FreqMax;
  }

  // Check if the max DIMM frequency is the limiter
  if (Outputs->MaxDimmFreq < Outputs->FreqMax) {
    Outputs->FreqMax = Outputs->MaxDimmFreq;
  }

  if (MrcIsSaGvEnabled (MrcData) && ((ExtInputs->MemoryProfile == STD_PROFILE) || ((ExtInputs->SafeModeOverride & MRC_SAFE_OVERRIDE_SAGV) != 0))) {
    MrcGetSagvConfig (MrcData, Outputs->SaGvPoint, Outputs->FreqMax, MaxQclkFreq, TRUE, &FreqMax, &Outputs->GearMode);
    Outputs->FreqMax = MIN (FreqMax, Outputs->FreqMax);
  } else {
    // Set Gear by the input parameter if not auto, otherwise default to G2 up to 2133 MHz.
    if (ExtInputs->GearRatio) {
      Outputs->GearMode = (ExtInputs->GearRatio == 4) ? 1 : 0;
    } else {
      Outputs->GearMode = (Outputs->FreqMax >= 2133) ? 1 : 0;
    }
    // Check if MAX_F_QCLK is the limiter at the selected Gear
    MaxQclkDataRate = (MaxQclkFreqRatio * 333334) * (Outputs->GearMode ? 4 : 2);
    MaxQclkDataRate = UDIVIDEROUND (MaxQclkDataRate, 10000);
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "MaxQclkDataRate %d\n", MaxQclkDataRate);
    if (MaxQclkDataRate < Outputs->FreqMax) {
      Outputs->FreqMax = MaxQclkDataRate;
    }
  }

  if (Outputs->GearMode) {
    Outputs->Gear4Ever = TRUE;
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Gear4Ever %d\n", Outputs->Gear4Ever);

  Outputs->MemoryClockMax = ConvertFreq2Clock (MrcData, Outputs->FreqMax);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Outputs->MemoryClockMax: %u, Outputs->FreqMax : %u, Gear%d\n", Outputs->MemoryClockMax, Outputs->FreqMax, Outputs->GearMode ? 4 : 2);

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "The maximum memory frequency allowed is %u, tCK=%ufs\n", Outputs->FreqMax, Outputs->MemoryClockMax);
  MRC_DEBUG_MSG (
    Debug,
    MSG_LEVEL_NOTE,
    "%uMHz reference clock is selected\n",
    REF_FREQ
    );

  if (Outputs->SaGvPoint == Outputs->SaGvFirst) {
    MrcCalculatePprSaGvPoint (MrcData, MaxQclkFreq);
  }

  return Status;
}

/**
  This function set PtrrSeed for pTRR/DRFM.
  @param[in] MrcData - Include all MRC global data.
**/

VOID MrcPmaSetPtrrSeed (
  IN     MrcParameters *const MrcData
  )
{
    // empty
}

/**
  This function sends a given PM message (PM0,PM13,PM14). BlueMrc will use it to send PM14 to memSS PMA.
  PM 0  - It is issued to wake up the PHY and MC. MRC requests PM0 to MemSS PMA by programming MEMSS_PMA_CR_BIOS_REQ_REG
          in McFrequencySet which is called during WorkPointLock. When McFrequencySet is called for JedecReset/Init, it issues a PM 13 message.
          In earlier platforms PM13 message was issued by requesting MemSS PMA but in LNL it will be issued to PHY PMA directly.
  PM 13 - Relock PHY PLL for LP5 Jedec reset/Init. This will be done using DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_REG
          and it will be specific to PHY. MemSS PMA will not be involved. This is a difference from legacy projects
          because Xtensa cannot send request to any block outside PHY.
  PM 14 - Initiate save of all trained register values to SRAM.For PHY this will be issued using
          DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_REG
          For MC it will be issued by requesting to memSS PMA MEMSS_PMA_CR_INIT_STATE_REG.MrcSave.

  @param[in] MrcData - Include all MRC global data.
  @param[in] PmMsgCode - PM0 or PM13 or PM14.
  @param[in] RequestTo - Which block is being requested, PHY or MC

  @retval mrcSuccess If poll InitComplete completed successfully
  @retval other an error code propagated from a called function
**/
MrcStatus
MrcSendPmMessage (
  IN MrcParameters* const MrcData,
  IN UINT32               PmMsgCode,
  IN BOOLEAN              RequestTo
  )
{
  MrcStatus  Status;

  Status = mrcPmMsgErr;

  switch (PmMsgCode) {
    case PM_MESSAGE_14:
      if (RequestTo == PM_MSG_PHY) {
        Status = MrcPhyPmMessage (MrcData, PmMsgCode);
      } else if (RequestTo == PM_MSG_MC) { // PM14 to MC
        Status = MrcMcPm14Message (MrcData);
      }
      break;

    case PM_MESSAGE_6:
    case PM_MESSAGE_8:
      Status = MrcPhyPmMessage (MrcData, PmMsgCode);
      break;

    case PM_MESSAGE_0:
      if (RequestTo == PM_MSG_MC) {
        Status = MrcPllLock (MrcData, FALSE, MRC_PRINTS_ON);
      } else if (RequestTo == PM_MSG_PHY) {
        Status = MrcPhyPmMessage (MrcData, PmMsgCode);
      }
      break;

    case PM_MESSAGE_0_VOLTAGE_ONLY:
      if (RequestTo == PM_MSG_MC) {
        Status = MrcPllLock (MrcData, TRUE, MRC_PRINTS_ON);
      }
      break;

    default:
      MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "%s Unknown PM Message: %d\n", gErrString, PmMsgCode);
      break;
  }

  return Status;
}
