/** @file
  This file contains code related to Chip Library Routing.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#include "CMrcTypes.h"
#include "MrcChipRouting.h"
#include "MrcCommon.h"
#include "MrcSagv.h"
#include "MrcChipApi.h"
#include "CMrcStartMemoryConfiguration.h"
#include "MrcDdrIoVcc.h"
#include "MrcDdrIoUtils.h"
#include "MrcPmaApi.h"
#include "MrcDdrIoApi.h"
#include "MrcPowerMetering.h"



/**
  This function locks the DDR frequency requested from SPD, User, or POR Tables.
  It will update the frequency related members in the output structure.

  SetVccLvr(): DdrIoApi - Accesses DDRIO, programs the Vcc LVR registers.
  SetWorkPointData(): Accesses DDRIO, sets workpoint data including Qclk, Gear, and AuxClk
  McFrequencySet(): ChipApi - Accesses PMA and DDRIO.  Initializes MrcData Variables
  MrcInternalCheckPoint(): Api - External/Customer/Flow control notification of state.
  MrcSetMrcVersion(): PmaApi - Writes PMA register which stores MRC FW Version.

  @param[in, out] MrcData - MRC global data.

  @retval mrcSuccess
**/
MrcStatus
MrcWorkPointLock (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcOutput         *Outputs;
  MrcDebug          *Debug;
  MrcStatus         Status;
  MrcSaGvPoint      SaGvPoint;
  INT64             GetSetEn;
  INT64             GetSetDis;
  MrcProfile        Profile;
  MrcInput          *Inputs;
  Inputs     = &MrcData->Inputs;
  Profile    = Inputs->ExtInputs.Ptr->MemoryProfile;
  Outputs    = &MrcData->Outputs;
  Debug      = &Outputs->Debug;
  SaGvPoint  = Outputs->SaGvPoint;
  GetSetEn   = 1;
  GetSetDis  = 0;
  Outputs->InWorkPointLock = 1;

  MrcStaticPma1 (MrcData);

  // Set GsmSkipRestoreCR, GsmSkipRetentionCR, PchannelEn (not set for TC) at First SAGV point. If Sagv is not enabled then set and clear in the same boot
  if ((Outputs->SaGvPoint == Outputs->SaGvFirst) || !MrcIsSaGvEnabled (MrcData)) {
    MrcPmCfgCrAccess (MrcData, TRUE, TRUE);
  }

  // Make sure tCL-tCWL <= 4
  // This is needed to support ODT properly for 2DPC case
  if ((Outputs->Timing[Profile].tCL - Outputs->Timing[Profile].tCWL) > 4) {
    Outputs->Timing[Profile].tCWL = Outputs->Timing[Profile].tCL - 4;
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_NOTE,
      "(tCL-tCWL) > 4, tCWL has been updated to %u\n",
      Outputs->Timing[Profile].tCWL
    );
  }

  SetVccLvr (MrcData);
  WriteAllLvrTarget (MrcData);
  SetTlineTermination (MrcData);

  // For the configuration of Power Meter Registers, it needs to be placed in front of write to MEMSS_PMA_CR_BIOS_REQ
  MrcPowerMeteringConfig (MrcData);

  // PLL Offset Calibration before waking up the PHY with a PM0 message
  // CompQClkOCal{1:0}.OffsetCal{WP} should start at the reset default of 0
  // MRC should override the fuse values CompPLL
  // PHClkOffsetFastCal and AuxOffsetFastCal set to 1 if OffsetCal CR isn't yet known
  // (ex: First time PLL was locked to this frequency)
  MrcGetSetFreqIndex (MrcData, SaGvPoint, GsmIocQClkCalOff, WriteCached | PrintValue, &GetSetDis);
  MrcGetSetFreqIndex (MrcData, SaGvPoint, GsmIocAuxClkOffAuxClk, WriteCached | PrintValue, &GetSetDis);
  MrcGetSetNoScope (MrcData, GsmIocPHClkOffsetFastCal, WriteCached | PrintValue, &GetSetEn);
  MrcGetSetNoScope (MrcData, GsmIocAuxOffsetFastCal, WriteCached | PrintValue, &GetSetEn);

  Status = SetWorkPointData (MrcData);
  if (Status != mrcSuccess) {
    return Status;
  }

  if (Outputs->UpmPwrRetrainFlag) {
    // SAGV point does not change when MRC flow returns to WorkPointLock
    // from a Retrain check detected, so that PMA sends PM13 message after
    // PMA.Run_Busy is set in McFrequencySet. Per design PHY is required to
    // perform ForceComp after a PM13 message
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\nForceComp due to Retrain!\n");
    ForceRcomp (MrcData, FullComp);
  }

  // PLL Lock
  Status = McFrequencySet (MrcData, FALSE, MRC_PRINTS_ON);

  if ((Outputs->SaGvPoint == Outputs->SaGvFirst) || !MrcIsSaGvEnabled (MrcData)) {
    MrcPmCfgCrAccess (MrcData, FALSE, TRUE);
  }

  // After PLL Lock (PM0 wakeup)
  MrcGetSetNoScope (MrcData, GsmIocPHClkOffsetFastCal, WriteCached | PrintValue, &GetSetDis);
  MrcGetSetNoScope (MrcData, GsmIocAuxOffsetFastCal, WriteCached | PrintValue, &GetSetDis);
  MrcProgramOffsetCalCnt (MrcData);

  MrcInternalCheckPoint (MrcData, OemFrequencySetDone);

  Outputs->InWorkPointLock = 0;
  // Save MRC Version into CR
  MrcSetMrcVersion (MrcData);

  if (MrcSetupMcIpInfo (MrcData) != mrcSuccess) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Failed to obtain MC IP Version\n");
    return mrcFail;
  }
  return Status;
}
