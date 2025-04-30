/** @file
  Starting point for the core memory reference code.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#include "MrcDebugPrint.h"
#include "MrcSagv.h"
#include "MrcMcConfiguration.h"
#include "BlueMrcSpdProcessingApi.h"
#include "CMrcStartMemoryConfiguration.h"
#include "MrcBlueGreenCommunication.h"
#include "CMrcStatsTracker.h"  // for MrcStatsBeginCallTablePhase()


/**
  Save Static SPD Data in a given structure.

  @param[in, out] MrcData       - Include all MRC global data.
  @param[in]      StaticSpdData - Static SPD Data storage pointer.

**/
VOID
MrcSaveStaticSpdData (
  IN OUT MrcParameters *const MrcData,
  IN OUT STATIC_SPD_DATA      *StaticSpdData
  )
{
  MrcOutput               *Outputs;
  const MRC_FUNCTION      *MrcCall;
  MrcControllerOut        *ControllerOut;
  MrcChannelOut           *ChannelOut;
  MrcDimmOut              *DimmOut;
  UINT8                   Controller;
  UINT8                   Channel;
  UINT8                   Dimm;

  Outputs = &MrcData->Outputs;
  MrcCall = MrcData->Inputs.Call.Func;

  // Save Static SPD Outputs
  StaticSpdData->OutDdrType             = Outputs->DdrType;
  StaticSpdData->OutLpX                 = Outputs->LpX;
  StaticSpdData->OutLpByteMode          = Outputs->LpByteMode;
  StaticSpdData->OutIsLpddr             = Outputs->IsLpddr;
  StaticSpdData->OutIsLpddr5            = Outputs->IsLpddr5;
  StaticSpdData->OutIsDdr5              = Outputs->IsDdr5;
  StaticSpdData->OutIsLP5Camm2          = Outputs->IsLP5Camm2;
  StaticSpdData->OutSystemDimmCount     = Outputs->SystemDimmCount;
  StaticSpdData->OutVdd2Mv              = Outputs->Vdd2Mv;
  StaticSpdData->OutBurstLength         = Outputs->BurstLength;
  StaticSpdData->OutMaxChannels         = Outputs->MaxChannels;
  StaticSpdData->OutMaxDimm             = Outputs->MaxDimm;
  StaticSpdData->OutMaxDqBits           = Outputs->MaxDqBits;
  StaticSpdData->OutIsAny2Ranks         = Outputs->IsAny2Ranks;
  StaticSpdData->OutXmpProfileEnable    = Outputs->XmpProfileEnable;
  StaticSpdData->OutXmpConfigWarning    = Outputs->XmpConfigWarning;
  StaticSpdData->OutIsDimm1Enabled      = Outputs->IsDimm1Enabled;
  StaticSpdData->OutIsAny2Dpc           = Outputs->IsAny2Dpc;
  StaticSpdData->OutIs1DPC1R            = Outputs->Is1DPC1R;
  StaticSpdData->OutIs1DPC2R            = Outputs->Is1DPC2R;
  StaticSpdData->OutIs2SPC0R1R          = Outputs->Is2SPC0R1R;
  StaticSpdData->OutIs2SPC0R2R          = Outputs->Is2SPC0R2R;
  StaticSpdData->OutIs2DPC1R1R          = Outputs->Is2DPC1R1R;
  StaticSpdData->OutIs2DPC2R2R          = Outputs->Is2DPC2R2R;
  StaticSpdData->OutIsSoDimm            = Outputs->IsSoDimm;
  StaticSpdData->OutIs2RankDdr5         = Outputs->Is2RankDdr5;
  MrcCall->MrcCopyMem ((UINT8 *) &StaticSpdData->OutVddVoltage[0], (UINT8 *) &Outputs->VddVoltage[0], sizeof (Outputs->VddVoltage));
  MrcCall->MrcCopyMem ((UINT8 *) &StaticSpdData->OutVddqVoltage[0], (UINT8 *) &Outputs->VddqVoltage[0], sizeof (Outputs->VddqVoltage));
  MrcCall->MrcCopyMem ((UINT8 *) &StaticSpdData->OutVppVoltage[0], (UINT8 *) &Outputs->VppVoltage[0], sizeof (Outputs->VppVoltage));
  MrcCall->MrcCopyMem ((UINT8 *) &StaticSpdData->OutMemFrequency[0], (UINT8 *) &Outputs->MemFrequency[0], sizeof (Outputs->MemFrequency));
  MrcCall->MrcCopyMem ((UINT8 *) &StaticSpdData->OutCkdBuffer[0], (UINT8 *) &Outputs->CkdBuffer[0], sizeof (Outputs->CkdBuffer));

  // Save Static SPD DimmOut
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    ControllerOut = &Outputs->Controller[Controller];
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      ChannelOut  = &ControllerOut->Channel[Channel];
      StaticSpdData->ChannelOutDimmCount[Controller][Channel] = ChannelOut->DimmCount;
      for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
        DimmOut                                                                  = &ChannelOut->Dimm[Dimm];
        StaticSpdData->DimmOutStatus[Controller][Channel][Dimm]                  = DimmOut->Status;
        StaticSpdData->DimmOutDdrType[Controller][Channel][Dimm]                 = DimmOut->DdrType;
        StaticSpdData->DimmOutModuleType[Controller][Channel][Dimm]              = DimmOut->ModuleType;
        StaticSpdData->DimmOutEccSupport[Controller][Channel][Dimm]              = DimmOut->EccSupport;
        StaticSpdData->DimmOutAddressMirrored[Controller][Channel][Dimm]         = DimmOut->AddressMirrored;
        StaticSpdData->DimmOutSelfRefreshTemp[Controller][Channel][Dimm]         = DimmOut->SelfRefreshTemp;
        StaticSpdData->DimmOutAutoSelfRefresh[Controller][Channel][Dimm]         = DimmOut->AutoSelfRefresh;
        StaticSpdData->DimmOutPartialSelfRefresh[Controller][Channel][Dimm]      = DimmOut->PartialSelfRefresh;
        StaticSpdData->DimmOutOnDieThermalSensor[Controller][Channel][Dimm]      = DimmOut->OnDieThermalSensor;
        StaticSpdData->DimmOutExtendedTemperRange[Controller][Channel][Dimm]     = DimmOut->ExtendedTemperRange;
        StaticSpdData->DimmOutExtendedTemperRefresh[Controller][Channel][Dimm]   = DimmOut->ExtendedTemperRefresh;
        StaticSpdData->DimmOutDimmCapacity[Controller][Channel][Dimm]            = DimmOut->DimmCapacity;
        StaticSpdData->DimmOutRowSize[Controller][Channel][Dimm]                 = DimmOut->RowSize;
        StaticSpdData->DimmOutColumnSize[Controller][Channel][Dimm]              = DimmOut->ColumnSize;
        StaticSpdData->DimmOutCrc[Controller][Channel][Dimm]                     = DimmOut->Crc;
        StaticSpdData->DimmOutRankInDimm[Controller][Channel][Dimm]              = DimmOut->RankInDimm;
        StaticSpdData->DimmOutBanks[Controller][Channel][Dimm]                   = DimmOut->Banks;
        StaticSpdData->DimmOutBankGroups[Controller][Channel][Dimm]              = DimmOut->BankGroups;
        StaticSpdData->DimmOutPrimaryBusWidth[Controller][Channel][Dimm]         = DimmOut->PrimaryBusWidth;
        StaticSpdData->DimmOutDiePerSdramPackage[Controller][Channel][Dimm]      = DimmOut->DiePerSdramPackage;
        StaticSpdData->DimmOutChannelsPerSdramPackage[Controller][Channel][Dimm] = DimmOut->ChannelsPerSdramPackage;
        StaticSpdData->DimmOutNumLpSysChannel[Controller][Channel][Dimm]         = DimmOut->NumLpSysChannel;
        StaticSpdData->DimmOutSdramWidth[Controller][Channel][Dimm]              = DimmOut->SdramWidth;
        StaticSpdData->DimmOutSdramWidthIndex[Controller][Channel][Dimm]         = DimmOut->SdramWidthIndex;
        StaticSpdData->DimmOutDensityIndex[Controller][Channel][Dimm]            = DimmOut->DensityIndex;
        StaticSpdData->DimmOuttMAC[Controller][Channel][Dimm]                    = DimmOut->tMAC;
        StaticSpdData->DimmOutReferenceRawCard[Controller][Channel][Dimm]        = DimmOut->ReferenceRawCard;
        StaticSpdData->DimmOutXmpSupport[Controller][Channel][Dimm]              = DimmOut->XmpSupport;
        StaticSpdData->DimmOutCkdSupport[Controller][Channel][Dimm]              = DimmOut->IsCkdSupport;
        StaticSpdData->DimmOutMbistMpprSupport[Controller][Channel][Dimm]        = DimmOut->IsMbistMpprSupport;
        StaticSpdData->DimmOutDeviceDensity[Controller][Channel][Dimm]           = DimmOut->DeviceDensity;
      } // for Dimm
    } // for Channel
  } // for Controller
} // MrcSaveStaticSpdData

/**
  Restore Static SPD Data from a given structure.

  @param[in, out] MrcData       - Include all MRC global data.
  @param[in]      StaticSpdData - Static SPD Data storage pointer.

**/
VOID
MrcRestoreStaticSpdData (
  IN OUT MrcParameters *const MrcData,
  IN OUT STATIC_SPD_DATA      *StaticSpdData
  )
{
  MrcOutput               *Outputs;
  const MRC_FUNCTION      *MrcCall;
  MrcControllerOut        *ControllerOut;
  MrcChannelOut           *ChannelOut;
  MrcDimmOut              *DimmOut;
  UINT8                   Controller;
  UINT8                   Channel;
  UINT8                   Dimm;

  Outputs = &MrcData->Outputs;
  MrcCall = MrcData->Inputs.Call.Func;

 // Restore Static SPD Outputs
  Outputs->DdrType             = StaticSpdData->OutDdrType;
  Outputs->LpX                 = StaticSpdData->OutLpX;
  Outputs->LpByteMode          = StaticSpdData->OutLpByteMode;
  Outputs->IsLpddr             = StaticSpdData->OutIsLpddr;
  Outputs->IsLpddr5            = StaticSpdData->OutIsLpddr5;
  Outputs->IsDdr5              = StaticSpdData->OutIsDdr5;
  Outputs->IsLP5Camm2          = StaticSpdData->OutIsLP5Camm2;
  Outputs->SystemDimmCount     = StaticSpdData->OutSystemDimmCount;
  Outputs->Vdd2Mv              = StaticSpdData->OutVdd2Mv;
  Outputs->BurstLength         = StaticSpdData->OutBurstLength;
  Outputs->MaxChannels         = StaticSpdData->OutMaxChannels;
  Outputs->MaxDimm             = StaticSpdData->OutMaxDimm;
  Outputs->MaxDqBits           = StaticSpdData->OutMaxDqBits;
  Outputs->IsAny2Ranks         = StaticSpdData->OutIsAny2Ranks;
  Outputs->XmpProfileEnable    = StaticSpdData->OutXmpProfileEnable;
  Outputs->XmpConfigWarning    = StaticSpdData->OutXmpConfigWarning;
  Outputs->IsDimm1Enabled      = StaticSpdData->OutIsDimm1Enabled;
  Outputs->IsAny2Dpc           = StaticSpdData->OutIsAny2Dpc;
  Outputs->Is1DPC1R            = StaticSpdData->OutIs1DPC1R;
  Outputs->Is1DPC2R            = StaticSpdData->OutIs1DPC2R;
  Outputs->Is2SPC0R1R          = StaticSpdData->OutIs2SPC0R1R;
  Outputs->Is2SPC0R2R          = StaticSpdData->OutIs2SPC0R2R;
  Outputs->Is2DPC1R1R          = StaticSpdData->OutIs2DPC1R1R;
  Outputs->Is2DPC2R2R          = StaticSpdData->OutIs2DPC2R2R;
  Outputs->IsSoDimm            = StaticSpdData->OutIsSoDimm;
  Outputs->Is2RankDdr5         = StaticSpdData->OutIs2RankDdr5;
  MrcCall->MrcCopyMem ((UINT8 *) &Outputs->VddVoltage[0], (UINT8 *) &StaticSpdData->OutVddVoltage[0], sizeof (Outputs->VddVoltage));
  MrcCall->MrcCopyMem ((UINT8 *) &Outputs->VddqVoltage[0], (UINT8 *) &StaticSpdData->OutVddqVoltage[0], sizeof (Outputs->VddqVoltage));
  MrcCall->MrcCopyMem ((UINT8 *) &Outputs->VppVoltage[0], (UINT8 *) &StaticSpdData->OutVppVoltage[0], sizeof (Outputs->VppVoltage));
  MrcCall->MrcCopyMem ((UINT8 *) &Outputs->MemFrequency[0], (UINT8 *) &StaticSpdData->OutMemFrequency[0], sizeof (Outputs->MemFrequency));
  MrcCall->MrcCopyMem ((UINT8 *) &Outputs->CkdBuffer[0], (UINT8 *) &StaticSpdData->OutCkdBuffer[0], sizeof (Outputs->CkdBuffer));

  // Restore Static SPD DimmOut
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    ControllerOut = &Outputs->Controller[Controller];
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      ChannelOut  = &ControllerOut->Channel[Channel];
      ChannelOut->DimmCount = StaticSpdData->ChannelOutDimmCount[Controller][Channel];
      for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
        DimmOut                          = &ChannelOut->Dimm[Dimm];
        DimmOut->Status                  = StaticSpdData->DimmOutStatus[Controller][Channel][Dimm];
        DimmOut->DdrType                 = StaticSpdData->DimmOutDdrType[Controller][Channel][Dimm];
        DimmOut->ModuleType              = StaticSpdData->DimmOutModuleType[Controller][Channel][Dimm];
        DimmOut->EccSupport              = StaticSpdData->DimmOutEccSupport[Controller][Channel][Dimm];
        DimmOut->AddressMirrored         = StaticSpdData->DimmOutAddressMirrored[Controller][Channel][Dimm];
        DimmOut->SelfRefreshTemp         = StaticSpdData->DimmOutSelfRefreshTemp[Controller][Channel][Dimm];
        DimmOut->AutoSelfRefresh         = StaticSpdData->DimmOutAutoSelfRefresh[Controller][Channel][Dimm];
        DimmOut->PartialSelfRefresh      = StaticSpdData->DimmOutPartialSelfRefresh[Controller][Channel][Dimm];
        DimmOut->OnDieThermalSensor      = StaticSpdData->DimmOutOnDieThermalSensor[Controller][Channel][Dimm];
        DimmOut->ExtendedTemperRange     = StaticSpdData->DimmOutExtendedTemperRange[Controller][Channel][Dimm];
        DimmOut->ExtendedTemperRefresh   = StaticSpdData->DimmOutExtendedTemperRefresh[Controller][Channel][Dimm];
        DimmOut->DimmCapacity            = StaticSpdData->DimmOutDimmCapacity[Controller][Channel][Dimm];
        DimmOut->RowSize                 = StaticSpdData->DimmOutRowSize[Controller][Channel][Dimm];
        DimmOut->ColumnSize              = StaticSpdData->DimmOutColumnSize[Controller][Channel][Dimm];
        DimmOut->Crc                     = StaticSpdData->DimmOutCrc[Controller][Channel][Dimm];
        DimmOut->RankInDimm              = StaticSpdData->DimmOutRankInDimm[Controller][Channel][Dimm];
        DimmOut->Banks                   = StaticSpdData->DimmOutBanks[Controller][Channel][Dimm];
        DimmOut->BankGroups              = StaticSpdData->DimmOutBankGroups[Controller][Channel][Dimm];
        DimmOut->PrimaryBusWidth         = StaticSpdData->DimmOutPrimaryBusWidth[Controller][Channel][Dimm];
        DimmOut->DiePerSdramPackage      = StaticSpdData->DimmOutDiePerSdramPackage[Controller][Channel][Dimm];
        DimmOut->ChannelsPerSdramPackage = StaticSpdData->DimmOutChannelsPerSdramPackage[Controller][Channel][Dimm];
        DimmOut->NumLpSysChannel         = StaticSpdData->DimmOutNumLpSysChannel[Controller][Channel][Dimm];
        DimmOut->SdramWidth              = StaticSpdData->DimmOutSdramWidth[Controller][Channel][Dimm];
        DimmOut->SdramWidthIndex         = StaticSpdData->DimmOutSdramWidthIndex[Controller][Channel][Dimm];
        DimmOut->DensityIndex            = StaticSpdData->DimmOutDensityIndex[Controller][Channel][Dimm];
        DimmOut->tMAC                    = StaticSpdData->DimmOuttMAC[Controller][Channel][Dimm];
        DimmOut->ReferenceRawCard        = StaticSpdData->DimmOutReferenceRawCard[Controller][Channel][Dimm];
        DimmOut->XmpSupport              = StaticSpdData->DimmOutXmpSupport[Controller][Channel][Dimm];
        DimmOut->IsCkdSupport            = StaticSpdData->DimmOutCkdSupport[Controller][Channel][Dimm];
        DimmOut->IsMbistMpprSupport      = StaticSpdData->DimmOutMbistMpprSupport[Controller][Channel][Dimm];
        DimmOut->DeviceDensity           = StaticSpdData->DimmOutDeviceDensity[Controller][Channel][Dimm];
      } // for Dimm
    } // for Channel
  } // for Controller
} // MrcRestoreStaticSpdData

/**
  Prepare MRC structures for next MRC Iteraion (SAGV or margin retrain).

  @param[in, out] MrcData - Include all MRC global data.
  @param[in]      Retrain - Turn on/off UpmPwrRetrainFlag (Should be always FALSE in Blue MRC)

**/
void
MrcPrepareNextMrcIteration (
  IN OUT MrcParameters *const MrcData,
  IN BOOLEAN                  Retrain
  )
{
  UINT16                  PrevRdDataValid;
  BOOLEAN                 PrevGear4Ever;
  BOOLEAN                 LowFreqCsCmd2dDone[MAX_SAGV_POINTS];
  BOOLEAN                 PrevBitByteSwizzleFound;
  MrcFrequency            PrevMaxDimmFreq;
  const MRC_FUNCTION      *MrcCall;
  MrcOutput               *Outputs;
  MrcInput                *Inputs;
  UINT8                   PrevCmosConfig0;
  UINT8                   PrevCmosConfig1;
  UINT8                   PrevCmosConfig2;
  STATIC_SPD_DATA         StaticSpdData;
  MrcSaGvPoint            PrevSagvFirst;
  MrcSaGvPoint            PrevSagvLast;
  MrcSaGvPoint            PrevSagvPpr;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  MrcCall = MrcData->Inputs.Call.Func;

  MrcCall->MrcSetMem ((UINT8 *) &StaticSpdData, sizeof (StaticSpdData), 0);

  MrcSaveStaticSpdData (MrcData, &StaticSpdData);
  MrcCall->MrcCopyMem ((UINT8 *) &LowFreqCsCmd2dDone, (UINT8 *) &Outputs->LowFreqCsCmd2DSweepDone, sizeof (Outputs->LowFreqCsCmd2DSweepDone));
  PrevRdDataValid = Outputs->MaxRdDataValid;
  PrevGear4Ever = Outputs->Gear4Ever;
  PrevBitByteSwizzleFound = Outputs->BitByteSwizzleFound;
  PrevMaxDimmFreq = Outputs->MaxDimmFreq;
  PrevSagvFirst = Outputs->SaGvFirst;
  PrevSagvLast = Outputs->SaGvLast;
  PrevSagvPpr  = Outputs->SaGvPprPoint;
  PrevCmosConfig0 = Outputs->CmosConfig0;
  PrevCmosConfig1 = Outputs->CmosConfig1;
  PrevCmosConfig2 = Outputs->CmosConfig2;

  MrcCall->MrcSetMem ((UINT8 *) Outputs, sizeof (MrcOutput), 0);
  Outputs->Size = sizeof (MrcOutput);
  Outputs->MaxRdDataValid = PrevRdDataValid;
  Outputs->Gear4Ever = PrevGear4Ever;
  Outputs->BitByteSwizzleFound = PrevBitByteSwizzleFound;
  Outputs->MaxDimmFreq = PrevMaxDimmFreq;
  Outputs->SaGvFirst = PrevSagvFirst;
  Outputs->SaGvLast = PrevSagvLast;
  Outputs->SaGvPprPoint = PrevSagvPpr;
  Outputs->CmosConfig0 = PrevCmosConfig0;
  Outputs->CmosConfig1 = PrevCmosConfig1;
  Outputs->CmosConfig2 = PrevCmosConfig2;

  MrcRestoreStaticSpdData (MrcData, &StaticSpdData);
  MrcCall->MrcCopyMem ((UINT8 *) &Outputs->LowFreqCsCmd2DSweepDone, (UINT8 *) &LowFreqCsCmd2dDone, sizeof (Outputs->LowFreqCsCmd2DSweepDone));

  Outputs->UpmPwrRetrainFlag = Retrain;

  MRC_DEBUG_MSG_OPEN (MrcData, Inputs->SerialDebugLevel, Inputs->DebugStream);
}

/**
  Initializes the memory controller and DIMMs.

  This is expected to be called from MrcStartMemoryConfiguration() so the stats framework is properly initialized
  and torn down.  Trying to ensure that the stats framework is properly initialized and torn down within this function
  would be challenging due to the multiple return statements that would need to be updated (and more importantly, the
  challenge of ensuring stats are torn down when a new return statement is added).

  @param[in, out] MrcData                - Include all MRC global data.
  @param[in]      Select                 - The post code of the call table entry to execute.
  @param[in, out] CurrentSaGvPoint       - The SAGV point that we want to execute the Select post code entry or to continue from (relevant if SAGV enabled).
  @param[in]      ExecuteFromSelectPoint - Execute Memory configuration from (Select,CurrentSaGvPoint) to the end

  @retval mrcSuccess if the initialization succeeded, otherwise an error status indicating the failure.
**/
static MrcStatus
MrcWrappedStartMemoryConfiguration (
  IN OUT MrcParameters *const MrcData,
  IN     MrcPostCode          Select,
  IN OUT MrcSaGvPoint  *const CurrentSaGvPoint,
  IN BOOLEAN                  ExecuteFromSelectPoint
  )
{
  const MRC_FUNCTION      *MrcCall;
  const CallTableEntry    *Task;
  MrcDebug                *Debug;
  MrcInput                *Inputs;
  MrcOutput               *Outputs;
  MrcSaveData             *SaveData;
  MRC_EXT_INPUTS_TYPE     *ExtInputs;
  MrcSaGvPoint            SaGvPoint;
  MrcSaGvPoint            SaGvFirst;
  MrcSaGvPoint            SaGvLast;
  MrcPostCode             post_code;
  MrcPostCode             PostCodeOut;
  MrcStatus               CpStatus;
  MrcStatus               MrcStatus;
  MRC_BOOT_MODE           BootMode;
  BOOLEAN                 SaGv;
  MrcSaGvPoint            SaGvOrder[MAX_SAGV_POINTS];
  MrcSaGvPoint            SaGvBeforeReset;
  UINT32                  SaGvIndex;
  UINT32                  SaGvNumPoints;
  UINT64                  start_time;
  UINT64                  finish_time;
  UINTN                   LowestStackAddr;
  UINT32                  DebugLevel;
  UINT32                  ElapsedTime;
  UINT32                  TotalTime;
  UINT32                  PointTime;
  UINT16                  index;
  UINT8                   Run;
  UINT8                   MemoryProfileSave;
  UINT8                   SaGvWpMask;
  UINT8                   SaGvOther;

  Inputs  = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  ExtInputs = Inputs->ExtInputs.Ptr;

  SaveData   = &MrcData->Save.Data;
  BootMode    = Inputs->BootMode;
  DebugLevel  = Inputs->SerialDebugLevel;
  SaGvBeforeReset = Inputs->SaGvBeforeReset;

  if (Select == 0) {
    MrcVersionGet (MrcData, &Outputs->Version);
    MrcVersionCheck (MrcData);
    MrcVersionPrint (MrcData, &Outputs->Version);
  }
  MemoryProfileSave = ExtInputs->MemoryProfile;

  if (BootMode == bmCold || BootMode == bmFast) {
    // MrcGetSpdData is only called on Cold and Fast boot
    MrcGetSpdDdrTypeParams (MrcData);
    if (BootMode == bmFast) {
      Outputs->IsLP5Camm2 = MrcData->Save.Data.IsLP5Camm2;
    }
  } else {
    // On S3 and Warm Boot, do an early restore of the DDR Type
    Outputs->DdrType = MrcData->Save.Data.DdrType;
    Outputs->IsDdr5 = MrcData->Save.Data.IsDdr5;
    Outputs->IsLpddr5 = MrcData->Save.Data.IsLpddr5;
    Outputs->IsLP5Camm2 = MrcData->Save.Data.IsLP5Camm2;
    Outputs->Is2RankDdr5 = MrcData->Save.Data.Is2RankDdr5;
  }


  // Restoring Is2RankDdr5 since SPD Byte 234 is not read in MrcGetSpdDdrTypeParams during Fast Boot.
  if (BootMode == bmFast) {
    Outputs->Is2RankDdr5 = MrcData->Save.Data.Is2RankDdr5;
  }
  Outputs->IsOCProfile = (MemoryProfileSave != STD_PROFILE) && Outputs->IsDdr5;
  MrcSetSafeModeOverrides (MrcData);

  post_code        = MRC_INITIALIZATION_START;
  Run              = 1;
  TotalTime        = 0;
  LowestStackAddr  = Debug->LowestStackAddr.DataN;
  SaGv             = MrcIsSaGvEnabled (MrcData);
  SaGvWpMask       = ExtInputs->SaGvWpMask;
  SaGvFirst        = MrcSaGvPointMax;
  SaGvLast         = MrcSaGvPointMax;

  if (BootMode == bmCold) {
    SaveData->IsXmpSagvEnabled = (ExtInputs->DynamicMemoryBoost || ExtInputs->RealtimeMemoryFrequency);
  } else if (ExtInputs->DynamicMemoryBoost || ExtInputs->RealtimeMemoryFrequency) {
    MRC_DEBUG_MSG (Debug,
        MSG_LEVEL_NOTE,
        "Dynamic Memory Boost or Realtime Memory Frequency %s\nMemoryProfile:%d\n",
        SaveData->IsXmpSagvEnabled ? "is running": "failed",
        ExtInputs->MemoryProfile);
  }

  // Determine if the SaGv configuration is allowed
  MrcStatus = MrcSaGvBoundsCheck (MrcData);
  if (MrcStatus != mrcSuccess) {
    return MrcStatus;
  }

  // Replicate SAGV - all points will be replicated from point 0
  if (ExtInputs->ReplicateSagv != 0) {
    if ((BootMode == bmCold) && SaGv) {
      ExtInputs->SaGv = MRC_SAGV_DISABLED;     //  Disable Sagv
      SaGv = MRC_SAGV_DISABLED;
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Replicate SAGV points on Cold Boot\n");
    } else {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Replicate SAGV cannot run in BootMode=%d SaGv=%d\n", BootMode, SaGv);
      ExtInputs->ReplicateSagv = 0;
    }
  }

  if (!SaGv) {
    // If SaGv is disabled, then only Point 0 runs
    SaGvWpMask = MRC_SAGV_FIXED_POINT_0;
  }

  // Use sequential SAGV order by default
  for (SaGvIndex = 0, index = 0; SaGvIndex < MAX_SAGV_POINTS; SaGvIndex++) {
    if (((SaGvWpMask >> SaGvIndex) & 1) == 0) {
      continue;
    }
    SaGvOrder[index++] = SaGvIndex;
  }
  SaGvNumPoints = MrcCountBitsEqOne (SaGvWpMask);

  SaveData->PostCodesDone  = 0;
  SaveData->PostCodesTotal = (MRC_POST_CODE_LAST - MRC_INITIALIZATION_START - MRC_POST_CODES_UNUSED) * SaGvNumPoints;

  if (SaGv) {
    // Regular SAGV case - reorder the points in Warm / S3 resume flow
    if ((BootMode == bmWarm) || (BootMode == bmS3)) {
      // Set the last point to this one, so that we take DRAM out of Self-Refresh at the same frequency that it entered Warm/S3.
      // MRC reports to Pcode on each point that is configured so order of the other points is not important but it is importent to exit in the entered point
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Previous SAGV point before Warm/S3: %u\n", SaGvBeforeReset);
      for (SaGvPoint = MrcSaGvPoint0, index = 0; index < (UINT16)(SaGvNumPoints - 1); SaGvPoint++) {
        if (SaGvPoint != SaGvBeforeReset) {
          SaGvOrder[index++] = SaGvPoint;
        }
      }
      SaGvOrder[SaGvNumPoints - 1] = SaGvBeforeReset;  // Last point - SR exit will be here
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "SAGV order: ");
      for (SaGvIndex = 0; SaGvIndex < SaGvNumPoints; SaGvIndex++) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%u ", SaGvOrder[SaGvIndex]);
      }
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
    }
  }

  // Determine the point that should be associated with the MF_SAGV_FIRST and MF_SAGV_LAST flags
  for (SaGvIndex = 0; SaGvIndex < SaGvNumPoints; SaGvIndex++) {
    SaGvPoint = SaGvOrder[SaGvIndex];
    if (((SaGvWpMask >> SaGvPoint) & 1) == 0) {
      continue;
    }
    if (SaGvFirst == MrcSaGvPointMax) {
      SaGvFirst = SaGvPoint;
    }
    SaGvLast = SaGvPoint;
  }

  // Save the first and last point in the MRC Outputs
  Outputs->SaGvFirst = SaGvFirst;
  Outputs->SaGvLast = SaGvLast;

  // Determine the point that should be associated with the MF_SAGV_OTHER flag
  SaGvOther = (~((1 << SaGvFirst) | (1 << SaGvLast)) & SaGvWpMask);

  for (SaGvIndex = 0; Run && (SaGvIndex < SaGvNumPoints); SaGvIndex++) {
    // Run SaGv Points that are enabled.
    SaGvPoint = SaGvOrder[SaGvIndex];
    Outputs->SaGvPoint = SaGvPoint;
    // Update  *CurrentSaGvPoint for future calling
    if (CurrentSaGvPoint != NULL) {
      *CurrentSaGvPoint = SaGvPoint;
    }
    Inputs->Iteration = 0;
    PointTime = 0;
    if (SaveData->IsXmpSagvEnabled) {
      if (SaGvPoint == MrcSaGvPoint0) {
        // Use STD_PROFILE for SaGv point 0
        ExtInputs->MemoryProfile = STD_PROFILE;
      } else {
        // Use selected MemoryProfile for SaGv Points 1~3
        ExtInputs->MemoryProfile = MemoryProfileSave;
      }
    }

    Outputs->IsOCProfile = (ExtInputs->MemoryProfile != STD_PROFILE) && Outputs->IsDdr5;

    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_NOTE,
      "SAGV: %sabled, running at point %u%s\n",
      (SaGv) ? "En" : "Dis",
      SaGvPoint,
      (SaGvPoint == SaGvLast) ? " (last)" : ""
      );

    MrcVersionGet (MrcData, &Outputs->Version);

    for (index = 0; Run && (index < MrcGetCallTableCount ()); index++, post_code++) {

      Task = &MrcCallTable[index];
      //
      // Decide if we need to execute the selected MRC task.
      //
      // If MRC task selected and ExecuteFromSelectPoint == TRUE, it means that we need to execute all MRC tasks from Selected point.
      // By clearing the Select variable we force to continue the execution of all remaining Tasks
      if ((Select != 0) && (ExecuteFromSelectPoint) && (Select == Task->post_code_ovr) ) {
        Select = 0;
      }

      if ((NULL == Task->mrc_task) || (Inputs->Iteration >= Task->iteration)) {
        continue;
      }
      if   (((BootMode == bmS3)   && (Task->policy_flag & MF_S3))
        ||  ((BootMode == bmFast) && (Task->policy_flag & MF_FAST))
        ||  ((BootMode == bmWarm) && (Task->policy_flag & MF_WARM))
        ||  ((BootMode == bmCold) && (Task->policy_flag & MF_COLD))
        ||  ((Select != 0))) {
        if  ((Select == 0) || (Select == Task->post_code_ovr)) {
          if (((Outputs->SaGvPoint == SaGvFirst)              && (Task->policy_flag & MF_GV_FIRST)) ||
              ((((SaGvOther >> Outputs->SaGvPoint) & 1) != 0) && (Task->policy_flag & MF_GV_OTHER)) ||
              ((Outputs->SaGvPoint == SaGvLast)               && (Task->policy_flag & MF_GV_LAST))){
            CpStatus = MrcInternalCheckPoint (MrcData, Task->oem_cmd);
            PostCodeOut = (Task->post_code_ovr == POST_CODE_NO_OVR) ? post_code : Task->post_code_ovr;
            if (mrcSuccess != CpStatus && !Inputs->IgnoreCheckPoint) {
              // MrcDebugHook is called here to update the MRC progress bar for skipped call table steps.
              MrcCall->MrcDebugHook (MrcData, PostCodeOut);
              continue;
            }
            // Output post code to post code I/O port.
            if ((Select == 0) || ((Select > 0) && (Select == PostCodeOut))) {
              MrcCall->MrcDebugHook (MrcData, PostCodeOut);
            }
            if (Inputs->StopPostCode == PostCodeOut) {
              return mrcSuccess;
            }
            // Output debug string to serial output and execute the MRC task.
            MRC_DEBUG_MSG(Debug, MSG_LEVEL_NOTE, "\nMRC task -- %s -- Started.\n", Task->String);
            if (!Task->DebugPrint) {
              DebugLevel = Debug->Level;
              Debug->Level = MSG_LEVEL_NONE;
            }
            start_time  = MrcCall->MrcGetCpuTime ();
            /* Want stats framework in an "idle" state when the code transfers control to green, conseqeuntly certain
               functions cannot have an active call table phase when they run.  If we did not do this, then
               green would get a data structure indicating that the call table phase was active, and potentially
               have non-zero data for stats associated with the phase.  This would result in incorrect, or
               at least misleading, stats being reported.

               Another potential problem is that starting a call table phase stores a "current time" in a variable.
               I doubt that the current time is synchronized between the micro controller and the IA cores.

               Chose to add a flag to indicate whether the funtion should start/end a call table phase.
               Chose MF_SKIP_STATS flag because it is required on fewer entries in the table.  Unfortunately,
               without additional work, it results in "negative logic" here (we do someting if a condition is false).
               A colleague came up with an alternative if/then/else organization that used positive logic.
            */

            if (Task->policy_flag & MF_SKIP_STATS) {
              MrcStatsPauseTracking (MrcData);
            } else {
              MrcStatsBeginCallTablePhase (MrcData);
            }
            MrcStatus   = Task->mrc_task (MrcData);
            finish_time = MrcCall->MrcGetCpuTime ();
            if (Task->policy_flag & MF_SKIP_STATS) {
              MrcStatsContinueTracking (MrcData);
            } else {
              MrcStatsEndCallTablePhase (MrcData, Task->String);
            }

            if (!Task->DebugPrint) {
              Debug->Level = DebugLevel;
            }
            MRC_DEBUG_MSG (
              Debug,
              MSG_LEVEL_ERROR,
              "MRC task %s -- %s, Status = %Xh.",
              Task->String,
              (mrcSuccess == MrcStatus) ? "SUCCEEDED" : "FAILED",
              MrcStatus
              );
            if (Debug->LowestStackAddr.DataN < LowestStackAddr) {
              MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, " Stack Depth: %" PRIdMAX "\n", Debug->TopStackAddr.DataN - Debug->LowestStackAddr.DataN);
              LowestStackAddr = Debug->LowestStackAddr.DataN;
            } else {
              MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "\n");
            }
            if ((mrcSuccess != MrcStatus) || (Select != 0)) {
              Run = 0;  // Stop task execution on failure or running one Task.
            }

            ElapsedTime = (UINT32) (finish_time - start_time);
            TotalTime += ElapsedTime;
            PointTime += ElapsedTime;
            // <MrcTimer Task="Print input parameters" msec="2"/>
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_TIME, "<MrcTimer Task=\"%s\" msec=\"%u\"/>\n", Task->String, ElapsedTime);

            // break out if task finished is the specified POST code to enter interpreter on
            if ((Inputs->EnterInterpreterOnPostCode != ENTER_INTERPRETER_NO_POST_CODE_SPECIFIED)
               && (Task->post_code_ovr == MRC_UC_EXECUTE_GREEN)) {
                MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Exit call table due to EnterInterpreterOnPostCode: 0x%x\n", Inputs->EnterInterpreterOnPostCode);
              break;
            }
          } else {
            // MrcDebugHook is called here to update the MRC progress bar for call table steps skipped due to the SaGv point check.
            PostCodeOut = (Task->post_code_ovr == POST_CODE_NO_OVR) ? post_code : Task->post_code_ovr;
            MrcCall->MrcDebugHook (MrcData, PostCodeOut);
          } // SAGV point match
        } // Select match
      } else {
        // MrcDebugHook is called here to update the MRC progress bar for call table steps skipped due to the boot mode check.
        PostCodeOut = (Task->post_code_ovr == POST_CODE_NO_OVR) ? post_code : Task->post_code_ovr;
        MrcCall->MrcDebugHook (MrcData, PostCodeOut);
      } // for boot mode match
    } // for index

    if ((MrcStatus == mrcSuccess) && (SaGv) && (Outputs->SaGvPoint != Outputs->SaGvLast) && (Select == 0)) {
      // Prepare for the next MRC iteration
      MrcPrepareNextMrcIteration (MrcData, FALSE);
    }

    if ((SaGv) && (Select == 0)) {
      MRC_DEBUG_MSG (
        Debug,
        MSG_LEVEL_TIME,
       "<!-- MRC timer: Total time for SAGV point %u = %u msec. -->\n",
        SaGvPoint,
        PointTime
        );
    }
  } // for GvPoint

  if (Select == 0) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_TIME, "<!-- MRC timer: Total time to execute tasks = %u msec. -->\n", TotalTime);
  }

  if (Outputs->IsOCProfile &&
      (MrcStatus != mrcSuccess) &&
      (BootMode == bmCold) &&
      (SaveData->IsXmpSagvEnabled)) {
    // Trigger Full Train with STD_PROFILE
    MRC_DEBUG_MSG (Debug,
                  MSG_LEVEL_NOTE,
                  "MRC training fails at profile %d. Trigger reset for full train with STD_PROFILE\n",
                  ExtInputs->MemoryProfile);
    MrcStatus = mrcResetFullTrain;
  }

  return MrcStatus;
}


/**
  Setup stats (if configured), initialize the memory controller and DIMMs, finalize stats (if configured).

  This function is trivial, but it ensures that the stats framework is always paired (init with finalize).  Putting
  the calls to init/finalize the stats framework in the called function risks future updates adding an early return
  and neglecting to add the call to finalize.

  @param[in, out] MrcData                - Include all MRC global data.
  @param[in]      Select                 - The post code of the call table entry to execute.
  @param[in, out] CurrentSaGvPoint       - The SAGV point that we want to execute the Select post code entry or to continue from (relevant if SAGV enabled).
  @param[in]      ExecuteFromSelectPoint - Execute Memory configuration from (Select,CurrentSaGvPoint) to the end

  @retval mrcSuccess if the initialization succeeded, otherwise an error status indicating the failure.
**/
MrcStatus
MrcStartMemoryConfiguration (
  IN OUT MrcParameters *const MrcData,
  IN     MrcPostCode          Select,
  IN OUT MrcSaGvPoint  *const CurrentSaGvPoint,
  IN BOOLEAN                  ExecuteFromSelectPoint
  )
{
  MrcStatus Status;

  MrcStatsInit (MrcData);
  Status = MrcWrappedStartMemoryConfiguration (MrcData, Select, CurrentSaGvPoint, ExecuteFromSelectPoint);
  MrcStatsFinalize (MrcData);

  return Status;
}
