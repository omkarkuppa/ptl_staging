/** @file
  This file contains all the MRC general API to the MRC wrapper.

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
#include "MrcGeneral.h"
#include "MrcSagv.h"
#include "MrcMaintenance.h"
#include "MrcMemoryApi.h"
#include "MrcLpddr5.h"
#include "MrcDdr5.h"
#include "CMrcStartMemoryConfiguration.h"
#include "MrcCpgcApi.h"
#include "MrcPmaApi.h"
#include "MrcChipApi.h"
#include "MrcLpddr5Registers.h"
#include "MrcRegisterCache.h"
#include "MrcDdrIoLvr.h"
#include "MrcDdrIoComp.h"
#include "MrcDdrIoUtils.h"
#include "MrcDdrIoApi.h"
#include "MrcDdrCommon.h"
#include "MrcSpdProcessingDefs.h" // for MRC_SPD_SDRAM_DENSITY_8Gb
#include "MrcSaveRestore.h"
#include "MrcGlobalDefinitions.h"
#include "MrcUcPayloadData.h"

// Number of "b" in y=mx+b linear calculation equation for VF curve, "m" is scaled to 100 due to decimal numbers usage
#define VDDQ_VF_CURVE_LINEAR_POINT_700 ((VDD_0_70)-(((((VDD_0_70 - VDD_0_65) * 100) / (f4267 - f2667)) * f4267) / 100))
#define VDDQ_VF_CURVE_LINEAR_POINT_800 ((VDD_0_80)-(((((VDD_0_80 - VDD_0_70) * 100) / (f5400 - f4267)) * f5400) / 100))

#ifdef MRC_DEBUG_PRINT
const char ThermEnString[]  = "ThermalEnables";
const char PrintBorder[]    = "*************************************\n";
const char *DimmOdtStr[]    = { "Wr", "NomRd", "NomWr", "Park", "ParkDqs", "Ca GroupA", "Cs GroupA", "Ck GroupA", "Ca GroupB", "Cs GroupB", "Ck GroupB"};
#endif // MRC_DEBUG_PRINT

#define SAGV_GEAR_MASK      (3)
#define SAGV_GEAR_MASK_BITS (2)
#define VR_RESOLUTION       (26)

/**
  Program MC/CPGC engines to either Normal mode of operation
  or CPGC training mode.

  @param[in] MrcData       - The MRC general data.
  @param[in] IsNormalMode  - TRUE for Normal mode, FALSE for CPGC mode

  @retval Returns mrcSuccess or mrcFail if CPGC Mode is locked.
**/
MrcStatus
MrcSetNormalMode (
  IN MrcParameters *const MrcData,
  IN BOOLEAN              IsNormalMode
  )
{
  MrcOutput     *Outputs;
  MrcDebug      *Debug;
  INT64         GetSetVal;
  INT64         CmdStretch;
  INT64         GetSetEn;
  INT64         GetSetDis;
  UINT32        Channel;
  UINT32        Controller;
  UINT32        Offset;
  BOOLEAN       IsLpddr;
  BOOLEAN       IsCpgcActive;
  MC0_NORMALMODE_CFG_STRUCT NormalModeCfg;

  Outputs       = &MrcData->Outputs;
  IsLpddr       = Outputs->IsLpddr;
  Debug         = &Outputs->Debug;
  GetSetEn      = 1;
  GetSetDis     = 0;

  IsCpgcActive = (!IsNormalMode);

  if (Outputs->CpgcModeLocked) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Cannot set Normal Mode CPGC Mode is locked\n");
    return mrcFail;
  }

  if (IsNormalMode) {
    MrcSetCpgcInitMode (MrcData, IsCpgcActive);   // Disable CPGC engines before clearing CPGC_CMI.CPGC_ACTIVE
  }

  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccBlockXarb, WriteToCache, &GetSetEn);  // Block XARB when changing normalmode and cadb_enable
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccBlockCke,  WriteToCache, &GetSetEn);
  MrcFlushRegisterCachedData (MrcData);

  NormalModeCfg.Data = 0;
  NormalModeCfg.Bits.normalmode = IsNormalMode;
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (MrcControllerExist (MrcData, Controller)) {
      if (IsCpgcActive) {
        MrcGetSetMc (MrcData, Controller, GsmMccCpgcActive, WriteCached, &GetSetEn);
      }
      MrcCpgcConfigInitialCredits (MrcData, Controller, IsCpgcActive);
      for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
        if (MrcChannelExist (MrcData, Controller, Channel) && (!IS_MC_SUB_CH (IsLpddr, Channel))) {
          // Set bus_retain_on_n_to_1_bubble when N:1 is used and we are in Training mode; otherwise clear it.
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccCmdStretch, ReadUncached, &CmdStretch);

          if (IsNormalMode) {
            MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccRefreshRankMask,  WriteToCache, &GetSetDis);
            MrcFlushRegisterCachedData (MrcData);
          }

          GetSetVal = IsLpddr ? 0x20 : 0x30;
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccVC0ReadEntries, WriteToCache, &GetSetVal);
          GetSetVal = IsLpddr ? 0x16 : 0x30;
          MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccVC1ReadEntries, WriteToCache, &GetSetVal);
          MrcFlushRegisterCachedData (MrcData);
        }
      }
      Offset = OFFSET_CALC_CH (MC0_NORMALMODE_CFG_REG, MC1_NORMALMODE_CFG_REG, Controller);
      MrcWriteCR (MrcData, Offset, NormalModeCfg.Data);
    }
  }
  GetSetVal = IsNormalMode ? GetSetDis : GetSetEn;
  // This ensures equal backpressure on WPQ / RPQ which is needed for CPGC test.
  // This bit is set to 1 during training and 0 at the end of MRC (Normal Mode)
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccInOrderIngress, WriteToCache, &GetSetVal);
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmDisVc1CplInterleave, WriteToCache, &GetSetDis);
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccBlockXarb, WriteToCache, &GetSetDis);  // Unblock XARB
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccBlockCke,  WriteToCache, &GetSetDis);  // Unblock CKE
  MrcFlushRegisterCachedData (MrcData);

  if (IsCpgcActive) {
    MrcSetCpgcInitMode (MrcData, IsCpgcActive);   // Enable CPGC engines after setting CPGC_CMI.CPGC_ACTIVE
  }

  if (!IsCpgcActive) { // Set cpgc active to 0 at the end
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      if (MrcControllerExist (MrcData, Controller)) {
        MrcGetSetMc (MrcData, Controller, GsmMccCpgcActive, WriteCached, &GetSetDis);
      }
    }
  }
  return mrcSuccess;
}

/**
  This function syncronizes the MC lpddr_current_fsp(_tracking) register fields
  with LPDDR5 MR16.FSP-OP

  @param[in] MrcData - The MRC general data.

  @retval None
**/
VOID
SyncLpddrFspOp (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs;
  INT64     GetSetVal;
  UINT8     MrrResult[MRC_MRR_ARRAY_SIZE];
  UINT32    FirstController;
  UINT32    FirstChannel;
  UINT32    FirstRank;

  Outputs = &MrcData->Outputs;
  FirstController = Outputs->FirstPopController;
  FirstChannel = Outputs->Controller[FirstController].FirstPopCh;
  FirstRank = GetRankToStoreResults (
                MrcData,
                Outputs->Controller[FirstController].Channel[FirstChannel].ValidRankBitMask
                );

  GetSetVal = 0xF;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccCkeOn, WriteCached | PrintValue, &GetSetVal);
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccCkeOverride, WriteCached | PrintValue, &GetSetVal);
  MrcIssueMrr (MrcData, FirstController, FirstChannel, FirstRank, mrMR16, MrrResult);
  GetSetVal = ((LPDDR5_MODE_REGISTER_16_TYPE *) &MrrResult[0])->Bits.FspOp;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccLpddrCurrentFsp, WriteCached | PrintValue, &GetSetVal);
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccLpddrCurrentFspTrk, WriteCached | PrintValue, &GetSetVal);
  GetSetVal = 0x0;
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccCkeOverride, WriteCached | PrintValue, &GetSetVal);
}

/**
  This function allow Pcode turning off VDDQ when entering PkgC from DVFSQ_LOW point, in which ZQ is disabled anyway
  Allocate 4 new bits [4:1] in MEMSS_PMA-CR_BIOS_MAILBOX, 1 bit per GV point, each indicating if ZQ is disabled at that point
  This will be executed by writing cr: mcmnts_spare2_0_0_0_mchbar.spare_rw[1] per GV point into MEMSS_PMA-CR_BIOS_MAILBOX

  @param[in] MrcData - The MRC global data.
**/
VOID
TurningOffVddqForDvfsqLowPoint (
  IN MrcParameters *const MrcData
  )
{
  const MrcInput   *Inputs;
  UINT32           SaGvPoint;
  INT64            GetSetVal;

  const MRC_EXT_INPUTS_TYPE  *ExtInputs;

  Inputs = &MrcData->Inputs;
  ExtInputs = Inputs->ExtInputs.Ptr;

  GetSetVal = 0;

  if (MrcIsSaGvEnabled (MrcData)) {
    for (SaGvPoint = 0; SaGvPoint < MAX_SAGV_POINTS; SaGvPoint++) {
      if ((ExtInputs->SaGvFreq[SaGvPoint] <= 3200) && ((ExtInputs->SaGvWpMask & (1 << SaGvPoint)))) {
        GetSetVal |= (UINT64) (1 << SaGvPoint);
      }
    }
  } else {
    GetSetVal = 0x1;
  }

  MrcGetSetNoScope (MrcData, GsmPmaBiosMailboxReserved, ForceWriteCached | PrintValue, &GetSetVal);
}

/**
  Adjust Wck Mode

  @param[in] MrcData - The MRC global data.
**/
VOID
MrcAdjustWckMode (
  IN MrcParameters *MrcData
  )
{
  MrcInput          *Inputs;
  MrcOutput         *Outputs;
  MrcChannelOut     *ChannelOut;
  UINT8             Controller;
  UINT8             Channel;
  UINT8             Rank;
  INT64             GetSetVal;
  INT64             GetSetDis;
  MrcModeRegister        MrAddress;
  MrcModeRegisterIndex   Mr18ArrayIndex;
  LPDDR5_MODE_REGISTER_18_TYPE Mr18;
  MRC_EXT_INPUTS_TYPE    *ExtInputs;

  Inputs        = &MrcData->Inputs;
  ExtInputs     = Inputs->ExtInputs.Ptr;
  Outputs       = &MrcData->Outputs;
  MrAddress     = mrMR18;
  Mr18ArrayIndex = MrcMrAddrToIndex (MrcData, &MrAddress);
  GetSetDis = 0;

  switch (ExtInputs->AdjustWckMode) {
    case MrcLp5WckSafe:
      GetSetVal = 0;
      break;
    case MrcLp5WckManual:
      GetSetVal = 1;
      break;
    case MrcLp5WckDynamic:
      GetSetVal = 2;
      break;
    default:
      return;
      break;
  }

  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccLp5WckMode, WriteCached | PrintValue, &GetSetVal);
  if (GetSetVal == MrcLp5WckDynamic) {
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccLp5WckFastMode, WriteCached | PrintValue, &GetSetDis);
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (MrcRankExist (MrcData, Controller, Channel, Rank)) {
          ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
          Mr18.Data8 = ChannelOut->Dimm[RANK_TO_DIMM_NUMBER(Rank)].Rank[(Rank % MAX_RANK_IN_DIMM)].MR[Mr18ArrayIndex];
          Mr18.Bits.WckAlwaysOn = (ExtInputs->AdjustWckMode == MrcLp5WckDynamic) ? 0 : 1;
          MrcIssueMrw (MrcData, Controller, Channel, Rank, mrMR18, Mr18.Data8, MRC_PRINTS_ON);
          ChannelOut->Dimm[RANK_TO_DIMM_NUMBER(Rank)].Rank[(Rank % MAX_RANK_IN_DIMM)].MR[Mr18ArrayIndex] = Mr18.Data8;
        }
      }
    }
  }
}

/**
  control MC/PMA telemetry (enable for debug purpose, disable by default)

  @param[in] MrcData - The MRC global data.
**/
VOID
MrcEnableTelemetry (
  IN MrcParameters *MrcData
  )
{
  MEMSS_PMA_CR_SURVIVABILITY_MISC_STRUCT MiscSurvivabilityControl;
  MC0_OS_TELEMETRY_CONTROL_STRUCT        OsTelemetryControl;
  MRC_EXT_INPUTS_TYPE  *ExtInputs;
  MrcInput  *Inputs;
  UINT8     Controller;
  UINT32    Offset;
  UINT8     Val;

  Inputs = &MrcData->Inputs;
  ExtInputs = Inputs->ExtInputs.Ptr;

  if (ExtInputs->TelemetryControl == 0) {
    return;
  } else if (ExtInputs->TelemetryControl == 1) {
    Val = 1;
  } else {
    Val = 0;
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (!MrcControllerExist (MrcData, Controller)) {
      continue;
    }

    Offset = MC0_OS_TELEMETRY_CONTROL_REG + (MC1_OS_TELEMETRY_CONTROL_REG - MC0_OS_TELEMETRY_CONTROL_REG) * Controller;
    OsTelemetryControl.Data = MrcReadCR (MrcData, Offset);
    OsTelemetryControl.Bits.enable_idle_ostelemetry = Val;
    OsTelemetryControl.Bits.EnableOSTelemetry = Val;
    MrcWriteCR (MrcData, Offset, OsTelemetryControl.Data);
  }

  MiscSurvivabilityControl.Data = MrcReadCR (MrcData, MEMSS_PMA_CR_SURVIVABILITY_MISC_REG);
  MiscSurvivabilityControl.Bits.TELEMETRY_CNTRS_EN = Val;
  MrcWriteCR (MrcData, MEMSS_PMA_CR_SURVIVABILITY_MISC_REG, MiscSurvivabilityControl.Data);
}

/**
  Performs Geardown entry flow during Normal Mode enabling.

  @param[in]  MrcData  - Include all MRC global data.
  @param[in]  SaveData - Pointer to storage struct to keep original values.

  @retval MrcStatus
**/
MrcStatus
NormalModeCsGeardownEntryFlow (
  IN MrcParameters *const MrcData,
  MRC_DDR5_CS_GEARDOWN_SAVE *SaveData
  )
{
  MrcStatus  Status;
  MrcOutput *Outputs;
  BOOLEAN  IsGvPointCa1N;
  UINT32   FirstController;
  UINT32   FirstChannel;
  INT64    MultiCycCmd;

  Outputs = &MrcData->Outputs;
  IsGvPointCa1N = (MrcGetNMode (MrcData) == CA_1_NMODE);
  FirstController = Outputs->FirstPopController;
  FirstChannel = Outputs->Controller[FirstController].FirstPopCh;

  SetMcForCs2nTransition (MrcData, SaveData, MrcSaveEnum);

  if (IsGvPointCa1N) {
    Status = SetCa2N (MrcData, MRC_ENABLE);
    if (Status != mrcSuccess) {
      return Status;
    }
  }
  MrcGetSetMcCh (MrcData, FirstController, FirstChannel, GsmMccMultiCycCmd, ReadFromCache, &MultiCycCmd);
  EnableMcMulticycleCs (MrcData);
  EnableDdr5MulticycleCs (MrcData);

  McCs2NEnableOrDisable (MrcData, MRC_ENABLE);

  Status = Ddr5EnableCsGearDown (MrcData);
  if (Status != mrcSuccess) {
    return Status;
  }
  DdrIoCs2NEnableOrDisable (MrcData, MRC_ENABLE);

  if (MultiCycCmd == 0) {
    DisableMcMulticycleCs (MrcData, FALSE);
  }

  return Status;
}

/**
  Performs Geardown exit flow during Normal Mode enabling.

  @param[in]  MrcData  - Include all MRC global data.
  @param[in]  SaveData - Pointer to storage struct to keep original values.

  @retval MrcStatus
**/
MrcStatus
NormalModeCsGeardownExitFlow (
  IN MrcParameters *const MrcData,
  MRC_DDR5_CS_GEARDOWN_SAVE *SaveData
  )
{
  BOOLEAN IsGvPointCa1N;
  MrcStatus Status;

  IsGvPointCa1N = (MrcGetNMode (MrcData) == CA_1_NMODE);

  Status = MrcCs2NSrefEntry (MrcData);
  if (Status != mrcSuccess) {
    return Status;
  }

  McCs2NEnableOrDisable (MrcData, MRC_DISABLE);
  DdrIoCs2NEnableOrDisable (MrcData, MRC_DISABLE);

  Status = MrcCs2NForceSrExit (MrcData);
  if (Status != mrcSuccess) {
    return Status;
  }

  if (IsGvPointCa1N) {
    Status = SetCa2N (MrcData, MRC_DISABLE);
    if (Status != mrcSuccess) {
      return Status;
    }
  }

  SetMcForCs2nTransition (MrcData, SaveData, MrcRestoreEnum);

  return Status;
}

/**
  This function enables Normal Mode and configures the Power Down Modes.
  We also have special flow here for SAGV in S3/Warm boot modes.

  @param[in] MrcData - The MRC general data.

  @retval Always returns mrcSuccess.
**/
MrcStatus
MrcNormalMode (
  IN MrcParameters *const MrcData
  )
{
  const MrcInput      *Inputs;
  MrcOutput           *Outputs;
  MrcDebug            *Debug;
  MrcSaveData         *SaveData;
  MRC_BOOT_MODE       BootMode;
  INT64               GetSetVal;
  UINT32              Controller;
  BOOLEAN             MemInSr;
  BOOLEAN             IsLpddr;
  UINT8               Channel;
  MrcStatus           Status;
  const MRC_EXT_INPUTS_TYPE *ExtInputs;
  DDR_PTM_CTL_PCU_STRUCT  DdrPtmCtl;
  MRC_DDR5_CS_GEARDOWN_SAVE CsGearDownSaveData;

  Inputs    = &MrcData->Inputs;
  Outputs   = &MrcData->Outputs;
  ExtInputs = Inputs->ExtInputs.Ptr;
  Debug     = &Outputs->Debug;
  SaveData  = &MrcData->Save.Data;
  BootMode  = Inputs->BootMode;
  MemInSr   = (BootMode == bmWarm) || (BootMode == bmS3);
  IsLpddr   = Outputs->IsLpddr;


  MrcEnableTelemetry (MrcData);

  if (Outputs->EccSupport) {
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        if (!MrcChannelExist (MrcData, Controller, Channel)) {
          continue;
        }
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "ECC support\n");
        GetSetVal = Outputs->EccSupport;
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccEccMode, WriteCached | PrintValue, &GetSetVal);
      }
    }
  }

  if (ExtInputs->LpMode || ExtInputs->LpMode4) {
    GetSetVal = 1;
    MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccPdwnLpmodeEnable, WriteCached, &GetSetVal);
  }

  if (SaveData->IsCs2NEver && !MemInSr && !Outputs->IsCs2NEnabled) {
    Status = NormalModeCsGeardownEntryFlow (MrcData, &CsGearDownSaveData);
    if (Status != mrcSuccess) {
      return Status;
    }
  }


  if (IsLpddr) {
    {
      // Synchronize MC lpddr_current_fsp With LPDDR5 MR16.FSP-OP
      SyncLpddrFspOp (MrcData);
    }
  }

  Outputs->CpgcModeLocked = FALSE; // Unlocking CPGC mode
  MrcSetNormalMode (MrcData, TRUE);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "CPGC Mode Unlocked\n");

  {
    if (!MemInSr) {
      // Issue ZQ on both channels
      MrcIssueZQ (MrcData);
    }
  }

  GetSetVal = 1;
  MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccEnableRefresh, ReadCrWriteCached, &GetSetVal);

  if (SaveData->IsCs2NEver && !MemInSr && !Outputs->IsCs2NEnabled) {
    Status = NormalModeCsGeardownExitFlow (MrcData, &CsGearDownSaveData);
    if (Status != mrcSuccess) {
      return Status;
    }
  }

  DdrPtmCtl.Data                  = MrcReadCR (MrcData, DDR_PTM_CTL_PCU_REG);
  DdrPtmCtl.Bits.PDWN_CONFIG_CTL  = (IsLpddr ? ExtInputs->EnablePwrDnLpddr :ExtInputs->EnablePwrDn);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "DDR_PTM_CTL %Xh: %Xh\n", DDR_PTM_CTL_PCU_REG, DdrPtmCtl.Data);
  MrcWriteCR (MrcData, DDR_PTM_CTL_PCU_REG, DdrPtmCtl.Data);

  // Pcode will read mcmnts_spare2_0_0_0_mchbar.spare_rw[1] and MEMSS_PMA-CR_BIOS_MAILBOX spare fields after MRC_DONE
  if (ExtInputs->DvfsqEnabled) {
    TurningOffVddqForDvfsqLowPoint(MrcData);
  }
  return mrcSuccess;
}

/**
  This function calculates the Storage size per selected Ibecc Region.

  @param[in] IbeccProtectedRegionMask - Selected IBECC region mask to calculate Storage Size

  @retval Storage Size in size of 1MB.
**/
UINT32
MrcCalcStorageSize (
  IN UINT32        IbeccProtectedRegionMask
  )
{
  UINT32                    ProtectedSize;
  UINT32                    StorageSize;

  // The size granularity for protected range per IBECC instance is 32MB (CMI address bits 38:25)
  ProtectedSize = (0x4000 - IbeccProtectedRegionMask) * 32;
  // Storage size is 1/32 of protected range
  StorageSize = ProtectedSize / 32;
  // Minimum storage size is 16MB (Storage size in MB)
  StorageSize = MAX (StorageSize, 16);

  return StorageSize;
}

/**
  This function calculates the total required Storage size in both IBECC instance based on Protected ranges.

  @param[in] MrcData - Include all MRC global data.

  @retval Total Storage Size in size of 1MB.
**/
UINT32
MrcGetIbeccStorageSize (
  IN MrcParameters *MrcData
  )
{
  const MrcInput            *Inputs;
  const MRC_EXT_INPUTS_TYPE *ExtInputs;
  UINT8                     Controller;
  UINT8                     ControllerCount;
  UINT8                     IbeccRegion;
  UINT32                    TotalStorageSize;

  Inputs           = &MrcData->Inputs;
  ExtInputs        = Inputs->ExtInputs.Ptr;
  ControllerCount  = 0;
  TotalStorageSize = 0;

  for (IbeccRegion = 0; IbeccRegion < MAX_IBECC_REGIONS; IbeccRegion++) {
    if (ExtInputs->IbeccProtectedRegionEnable[IbeccRegion] == 1) {
      TotalStorageSize += MrcCalcStorageSize (ExtInputs->IbeccProtectedRegionMask[IbeccRegion]);
    }
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (MrcControllerExist (MrcData, Controller)) {
      ControllerCount++;
    }
  }

  ControllerCount = MAX (1, ControllerCount);
  TotalStorageSize *= ControllerCount;

  // return TotalStorageSize in granularity of 1MB since TotalPhysicalMemorySize is in size of 1MB
  return (TotalStorageSize);
}

/**
  This function calculates the TotalPhysicalMemorySize - EDSR size and set in MemoryMap->TotalPhysicalMemorySize.

  @param[in] MrcData - Include all MRC global data.

  @retval TOM - EDSR.
**/
UINT32
MrcConfigureIbeccTomMinusEdsr (
  IN MrcParameters *MrcData
  )
{
  MrcDebug                  *Debug;
  const MrcInput*           Inputs;
  MrcOutput                 *Outputs;
  const MRC_EXT_INPUTS_TYPE *ExtInputs;
  MrcMemoryMap              *MemoryMap;
  UINT8                     EccSpaceFraction;
  UINT32                    TomMinusEdsr;
  UINT32                    IbeccStorageSize;

  Inputs    = &MrcData->Inputs;
  Outputs   = &MrcData->Outputs;
  Debug     = &Outputs->Debug;
  ExtInputs = Inputs->ExtInputs.Ptr;
  MemoryMap = &Outputs->MemoryMapData;

  IbeccStorageSize = 0;
  EccSpaceFraction = ExtInputs->EccGranularity32BEn ? 16 : 32;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "TOM (Total physical memory size) = %u MB\n", MemoryMap->TotalPhysicalMemorySize);

  // Carve out TOM - EDSR
  // EDSR = EccGranularity32BEn ? (TOM / 16) : (TOM / 32)
  IbeccStorageSize = MemoryMap->TotalPhysicalMemorySize / EccSpaceFraction;
  MemoryMap->TotalPhysicalMemorySize = MemoryMap->TotalPhysicalMemorySize - IbeccStorageSize;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "EDSR = %u MB\n", IbeccStorageSize);

  TomMinusEdsr = MemoryMap->TotalPhysicalMemorySize;

  return TomMinusEdsr;
}

/**
  Enables IBECC if supported by CPU, and if memory controller population is symmetric

  @param[in] MrcData - Pointer to the MRC Debug structure.

  @retval mrcSuccess Ibecc task completed
**/
MrcStatus
MrcIbecc (
  IN MrcParameters *MrcData
  )
{
  MrcDebug                                      *Debug;
  const MrcInput                                *Inputs;
  MrcOutput                                     *Outputs;
  const MRC_FUNCTION                            *MrcCall;
  MRC_EXT_INPUTS_TYPE                           *ExtInputs;
  UINT32                                        Offset;
  UINT32                                        TomMinusEdsr;
  UINT8                                         Controller;
  UINT8                                         IbeccRegion;
  UINT8                                         ControllerCount;
  BOOLEAN                                       IsIbeccSymmetric;
  UINT32                                        HashMask;
  UINT32                                        HashLsb;
  INT64                                         GetSetEnable;
  IbeccOpMode                                   IbeccOperationMode;
  MC0_IBECC_CONTROL_STRUCT                      IbeccControl;
  MC0_IBECC_PARITY_CONTROL_STRUCT               IbeccParityControl;
  MC0_PARITY_CONTROL_STRUCT                     ParityControl;
  MC0_IBECC_MEMORY_INIT_CONTROL_STRUCT          IbeccMemInit;
  MC0_IBECC_ACTIVATE_STRUCT                     IbeccActivate;
  MC0_IBECC_ECC_INJ_CONTROL_STRUCT              IbeccEccInjControl;
  MC0_IBECC_ECC_INJ_ADDR_BASE_STRUCT            IbeccEccInjAddrBase;
  MC0_IBECC_ECC_INJ_ADDR_MASK_STRUCT            IbeccEccInjAddrMask;
  MC0_IBECC_ADDR_HASH_STRUCT                    IbeccAddrHash;
  MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_STRUCT     EccStorage;
  MC0_IBECC_PROTECT_ADDR_RANGE_0_STRUCT         IbeccAddressRange;

  Inputs    = &MrcData->Inputs;
  Outputs   = &MrcData->Outputs;
  Debug     = &Outputs->Debug;
  ExtInputs = Inputs->ExtInputs.Ptr;
  MrcCall   = Inputs->Call.Func;
  ControllerCount       = 0;
  GetSetEnable          = 1;
  TomMinusEdsr          = 0;
  Outputs->FinalIbeccOperationMode = (UINT8) ExtInputs->IbeccOperationMode;

  if (Inputs->IsIbeccPmaEnabled) {
    IsIbeccSymmetric = MrcIsIbeccSymmetric (MrcData);
    IbeccOperationMode = (!Inputs->IsIbeccEnabled || !IsIbeccSymmetric) ? IbeccNonProtect : ExtInputs->IbeccOperationMode;
    Outputs->FinalIbeccOperationMode = IbeccOperationMode;

    if (!IsIbeccSymmetric) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "IBECC is disabled since memory is not symmetrical\n");
    }

    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      if (MrcControllerExist (MrcData, Controller)) {
        ControllerCount++;
      }
    }
    ControllerCount = MAX (1, ControllerCount);

    if (ExtInputs->EccCorrectionMode == 1) {
      // 1 = ZECTED (Zero error correct triple error detect)
      MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccEcDis, WriteCached, &GetSetEnable);
      MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccEccCorrectionDisable, WriteCached, &GetSetEnable);
    }

    if (ExtInputs->EccGranularity32BEn == 1) {
      MrcGetSetMc (MrcData, MAX_CONTROLLER, GsmMccEccGranularity, WriteCached, &GetSetEnable);
    }

    if (IbeccOperationMode != IbeccNonProtect) {
      TomMinusEdsr = MrcConfigureIbeccTomMinusEdsr (MrcData);
    }

    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      if (MrcControllerExist (MrcData, Controller)) {

        if (ExtInputs->IbeccParity) {
          Offset = OFFSET_CALC_CH (MC0_PARITY_CONTROL_REG, MC1_PARITY_CONTROL_REG, Controller);
          ParityControl.Data = MrcReadCR (MrcData, Offset);
          ParityControl.Bits.PARITY_EN = 1;
          MrcWriteCR (MrcData, Offset, ParityControl.Data);

          Offset = OFFSET_CALC_CH (MC0_IBECC_PARITY_CONTROL_REG, MC1_IBECC_PARITY_CONTROL_REG, Controller);
          IbeccParityControl.Data = MrcReadCR (MrcData, Offset);
          IbeccParityControl.Bits.PARITY_CHK_EN = 1;
          MrcWriteCR (MrcData, Offset, IbeccParityControl.Data);
        }

        Offset = OFFSET_CALC_CH (MC0_IBECC_CONTROL_REG, MC1_IBECC_CONTROL_REG, Controller);
        IbeccControl.Data = MrcReadCR (MrcData, Offset);
        IbeccControl.Bits.RSB_ENABLE = 1;
        IbeccControl.Bits.OPERATION_MODE = IbeccOperationMode;
        MrcWriteCR (MrcData, Offset, IbeccControl.Data);
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "MC%u:\nIbeccControl OPERATION_MODE: %d\n", Controller, IbeccControl.Bits.OPERATION_MODE);
        if (IbeccOperationMode == IbeccAllProtect) {
          // When IBECC is configured to protect all of memory, the ECC_STORAGE_ADDR_RANGE_0 will be used for indicating the start address of the protected range
          Offset = OFFSET_CALC_CH (MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_REG, MC1_IBECC_ECC_STORAGE_ADDR_RANGE_0_REG, Controller);
          EccStorage.Data = MrcReadCR (MrcData, Offset);
          // TomMinusEdsr correspond to address bits 38:20 (Bits 19:0 are assumed to be 0), EccStorage.ADDRESS correspond to adderss bits 38:24 (Bits 23:0 are assumed to be 0)
          EccStorage.Bits.ADDRESS = TomMinusEdsr >> (24 - 20);
          MrcWriteCR (MrcData, Offset, EccStorage.Data);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "EccStorage ADDRESS: 0x%x\n", EccStorage.Bits.ADDRESS);

        } else if (IbeccOperationMode == IbeccPartialProtect) {
          for (IbeccRegion = 0; IbeccRegion < MAX_IBECC_REGIONS; IbeccRegion++) {
            // Specifies the Address Space that is reserved to store ECC data for all protected ranges
            Offset = OFFSET_CALC_CH (MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_REG, MC1_IBECC_ECC_STORAGE_ADDR_RANGE_0_REG, Controller);
            Offset += ((MC0_IBECC_ECC_STORAGE_ADDR_RANGE_1_REG - MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_REG) * IbeccRegion);
            EccStorage.Data = MrcReadCR (MrcData, Offset);
            if (ExtInputs->IbeccProtectedRegionEnable[IbeccRegion] == 1) {
              EccStorage.Bits.ADDRESS = TomMinusEdsr >> (24 - 20);
              MrcWriteCR (MrcData, Offset, EccStorage.Data);
              MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "EccStorage Range %d ADDRESS: 0x%x\n", IbeccRegion, EccStorage.Bits.ADDRESS);
              Offset = OFFSET_CALC_CH (MC0_IBECC_PROTECT_ADDR_RANGE_0_REG, MC1_IBECC_PROTECT_ADDR_RANGE_0_REG, Controller);
              Offset += ((MC0_IBECC_PROTECT_ADDR_RANGE_1_REG - MC0_IBECC_PROTECT_ADDR_RANGE_0_REG) * IbeccRegion);
              IbeccAddressRange.Data = MrcReadCR(MrcData, Offset);
              IbeccAddressRange.Bits.BASE = ExtInputs->IbeccProtectedRegionBase[IbeccRegion] & (MC0_IBECC_PROTECT_ADDR_RANGE_0_BASE_MSK >> MC0_IBECC_PROTECT_ADDR_RANGE_0_BASE_OFF);
              IbeccAddressRange.Bits.MASK = ExtInputs->IbeccProtectedRegionMask[IbeccRegion] & (MC0_IBECC_PROTECT_ADDR_RANGE_0_MASK_MSK >> MC0_IBECC_PROTECT_ADDR_RANGE_0_MASK_OFF);
              IbeccAddressRange.Bits.RANGE_EN = 0x1;
              MrcWriteCR (MrcData, Offset, IbeccAddressRange.Data);
              MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Protected Range %d: BASE = 0x%x, MASK = 0x%x\n", IbeccRegion, IbeccAddressRange.Bits.BASE, IbeccAddressRange.Bits.MASK);
            }
          }
        }

        if (ExtInputs->IbeccEccInjControl != IbeccEccInjNoErrorInjection) {
          if ((ExtInputs->IbeccEccInjControl == IbeccEccInjCorrAddressMatch) || (ExtInputs->IbeccEccInjControl == IbeccEccInjUncorrAddressMatch)) {
            IbeccEccInjAddrBase.Data =  (MrcCall->MrcLeftShift64 (ExtInputs->IbeccEccInjAddrBase, 25));
            Offset = OFFSET_CALC_CH (MC0_IBECC_ECC_INJ_ADDR_BASE_REG, MC1_IBECC_ECC_INJ_ADDR_BASE_REG, Controller);
            MrcWriteCR64 (MrcData, Offset, IbeccEccInjAddrBase.Data);
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Inj BASE: 0x%llx\n", IbeccEccInjAddrBase.Data);
            IbeccEccInjAddrMask.Data = 0x7FFE000000ULL;
            Offset = OFFSET_CALC_CH (MC0_IBECC_ECC_INJ_ADDR_MASK_REG, MC1_IBECC_ECC_INJ_ADDR_MASK_REG, Controller);
            MrcWriteCR64 (MrcData, Offset, IbeccEccInjAddrMask.Data);
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Inj MASK: 0x%llx\n", IbeccEccInjAddrMask.Data);
          }

          IbeccEccInjControl.Data = 0;
          if ((ExtInputs->IbeccEccInjControl == IbeccEccInjCorrCountInsertion) || (ExtInputs->IbeccEccInjControl == IbeccEccInjUncorrCountInsertion)) {
            IbeccEccInjControl.Bits.COUNT = ExtInputs->IbeccEccInjCount;
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Inj COUNT: 0x%x\n", IbeccEccInjControl.Bits.COUNT);
          }

          IbeccEccInjControl.Bits.ECC_INJECT = (ExtInputs->IbeccEccInjControl) & MC0_IBECC_ECC_INJ_CONTROL_ECC_INJECT_MAX;
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Inj ECC_INJECT: %x\n", IbeccEccInjControl.Bits.ECC_INJECT);
          Offset = OFFSET_CALC_CH (MC0_IBECC_ECC_INJ_CONTROL_REG, MC1_IBECC_ECC_INJ_CONTROL_REG, Controller);
          MrcWriteCR (MrcData, Offset, IbeccEccInjControl.Data);
        }

        Offset = OFFSET_CALC_CH (MC0_IBECC_ACTIVATE_REG, MC1_IBECC_ACTIVATE_REG, Controller);
        IbeccActivate.Data = MrcReadCR (MrcData, Offset);
        IbeccActivate.Bits.IBECC_EN = 1;
        MrcWriteCR (MrcData, Offset, IbeccActivate.Data);
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "IbeccActivate IBECC_EN: %d\n", IbeccActivate.Bits.IBECC_EN);

        if (ControllerCount == MAX_CONTROLLER) {
          Offset = OFFSET_CALC_CH (MC0_IBECC_ADDR_HASH_REG, MC1_IBECC_ADDR_HASH_REG, Controller);
          IbeccAddrHash.Data = MrcReadCR (MrcData, Offset);
          // SAF FW & BIOS programming HASH_MASK 5.1.5.2, section MC hash:
          // The IBECC register should be configured to same values as the HBO one.
          IbeccAddrHash.Bits.HASH_ENABLED = TRUE;
          GetMcIbeccHash (MrcData, &HashMask, &HashLsb);
          IbeccAddrHash.Bits.HASH_MASK    = HashMask;
          IbeccAddrHash.Bits.HASH_LSB     = HashLsb;
          MrcWriteCR (MrcData, Offset, IbeccAddrHash.Data);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "IbeccAddrHash: 0x%x\n", IbeccAddrHash.Data);
        }

        // Do not run the IBECC Init Ranges FSM when:
        // - The Boot Mode is bmWarm and the IbeccOperationMode has not changed
        // - The IbeccOperationMode is Bypass Mode (IbeccNonProtect)
        // - The Boot Mode is S3
        Outputs->IsIbeccInitRangesRequired =
          (Inputs->BootMode != bmWarm || (Inputs->BootMode == bmWarm && IbeccOperationMode != Inputs->LastIbeccOperationMode)) &&
          IbeccOperationMode != IbeccNonProtect;
        if (Inputs->BootMode == bmS3) {
          Outputs->IsIbeccInitRangesRequired = FALSE;
        }


        if (Outputs->IsIbeccInitRangesRequired) {
          Offset = OFFSET_CALC_CH (MC0_IBECC_MEMORY_INIT_CONTROL_REG, MC1_IBECC_MEMORY_INIT_CONTROL_REG, Controller);
          IbeccMemInit.Data = MrcReadCR (MrcData, Offset);
          IbeccMemInit.Bits.INIT_ALL_RANGES = 1;
          MrcWriteCR (MrcData, Offset, IbeccMemInit.Data);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "IbeccMemInit.INIT_ALL_RANGES: %u\n", IbeccMemInit.Bits.INIT_ALL_RANGES);
        }
      }
    } // Controller
  }

  return mrcSuccess;
}

/**
  Poll for IBECC_MEMORY_INIT.INIT_ALL_RANGES to go from 1->0

  @param[in] MrcData - Include all MRC global data. include also the memory map data.

  @retval MrcStatus -  if the bit was reset within the timeout
**/
MrcStatus
MrcPollIbeccFSMInit (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput                             *Outputs;
  MrcDebug                              *Debug;
  UINT8                                 Controller;
  UINT32                                Offset;
  const MRC_FUNCTION                    *MrcCall;
  UINT64                                Timeout;
  UINT64                                StartTime;
  UINT64                                EndTime;
  BOOLEAN                               Done;
  MC0_IBECC_MEMORY_INIT_CONTROL_STRUCT  IbeccMemInit;

  Outputs   = &MrcData->Outputs;
  Debug     = &Outputs->Debug;
  MrcCall   = MrcData->Inputs.Call.Func;
  StartTime = MrcCall->MrcGetCpuTime ();
  Timeout = StartTime + 15000;  // 15 sec timeout
  Done = FALSE;
  if (Outputs->IsIbeccInitRangesRequired) {
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      if (MrcControllerExist (MrcData, Controller)) {
        Offset = OFFSET_CALC_CH (MC0_IBECC_MEMORY_INIT_CONTROL_REG, MC1_IBECC_MEMORY_INIT_CONTROL_REG, Controller);
        do {
          IbeccMemInit.Data = MrcReadCR (MrcData, Offset);
          Done = (IbeccMemInit.Bits.INIT_ALL_RANGES == 0);
        } while (!Done && (MrcCall->MrcGetCpuTime () < Timeout));
      }
    }

    EndTime = MrcCall->MrcGetCpuTime ();

    if (!Done) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s Timeout!\n", gErrString, "IbeccMemInit.INIT_ALL_RANGES");
      return mrcTimeout;
    } else {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s took %u ms\n", "IbeccMemInit.INIT_ALL_RANGES", (UINT32) (EndTime - StartTime));
      return mrcSuccess;
    }
  } else {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "IBECC ranges initialization is NOT required\n");
  }

  return mrcSuccess;
}

/**
  This function reads out DDRPHY_MISC_SAUG_CR_RTL_VERSION_REG.IP_Model and returns the IP Model written by TE.

  @param[in]  MrcData - Include all MRC global data.

  @retval UINT32 IpModel - IP Model Version
**/
UINT32
MrcIpModelGet (
  IN MrcParameters *const MrcData
  )
{
  UINT32                                 IpModel;
  DDRPHY_MISC_SAUG_CR_RTL_VERSION_STRUCT DdrMiscRtlVer;

  DdrMiscRtlVer.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_RTL_VERSION_REG);
  IpModel = DdrMiscRtlVer.Bits.IP_Model;

  return IpModel;
}

/**
  This function locks the memory controller and memory map registers.

  @param[in] MrcData - Include all MRC global data.

  @retval Nothing.
**/
void
McRegistersLock (
  IN     MrcParameters *const MrcData
  )
{
  const MrcInput                                 *Inputs;
  const MRC_FUNCTION                             *MrcCall;
  MrcDebug                                       *Debug;
  UINT64                                         Offset;
  UINT64                                         PciEBaseAddress;
  DPR_0_0_0_PCI_STRUCT                           Dpr;

  Debug   = &MrcData->Outputs.Debug;
  Inputs  = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;
  PciEBaseAddress = Inputs->PciEBaseAddress;

  // Lock PRMRR.  Convert from MB to Address.
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "MrcSetLockPrmrr: 0x%p\n", MrcCall->MrcSetLockPrmrr);
  MrcCall->MrcSetLockPrmrr (MrcCall->MrcLeftShift64 (MrcData->Outputs.MemoryMapData.PrmrrBase, 20), Inputs->PrmrrSize << 20);

  // Lock DPR register
  Offset        = PciEBaseAddress + MrcCall->MrcGetPcieDeviceAddress (0, 0, 0, DPR_0_0_0_PCI_REG);
  Dpr.Data      = MrcCall->MrcMmioRead32 (Offset);
  Dpr.Bits.LOCK = 1;

  if (Inputs->ExtInputs.Ptr->DprLock == FORCE_DISABLE) {
    Dpr.Bits.LOCK = 0;
  }
  MrcCall->MrcMmioWrite32 (Offset, Dpr.Data);

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\nMcRegisters are locked\n");
}

/**
  This function returns the recommended MRC boot mode.

  @param[in] MrcData - The global host structure

  @retval bmWarm if we are in self refresh and the DISB bit is set, otherwise returns bmCold.
**/
MRC_BOOT_MODE
MrcGetBootMode (
  IN MrcParameters * const MrcData
  )
{
  MRC_BOOT_MODE BootMode;
  MrcInput     *Inputs;
  MRC_FUNCTION *MrcCall;
  UINT32       RegisterVal32;

  Inputs    = &MrcData->Inputs;
  MrcCall   = Inputs->Call.Func;

  RegisterVal32 = MrcCall->MrcMmioRead32((UINTN)PCH_PWRM_BASE_ADDRESS + R_PCH_PWRM_GEN_PMCON_A);
  if (((RegisterVal32 & B_PCH_PWRM_GEN_PMCON_A_MEM_SR_MRC) != 0) && ((RegisterVal32 & B_PCH_PWRM_GEN_PMCON_A_DISB_MRC) != 0))  {
    BootMode = bmWarm;
  } else {
    BootMode = bmCold;
  }

  return BootMode;
}

/**
  This function sets the DISB bit in General PM Configuration.

  @param[in] MrcData - The global host structure

  @retval Nothing.
**/
void
MrcSetDISB (
  IN MrcParameters *const MrcData
  )
{
  MrcInput      *Inputs;
  MRC_FUNCTION  *MrcCall;
  UINT32        RegOffset;
  UINT8         RegisterVal;
  UINT8         Rw1cBits;

  Inputs    = &MrcData->Inputs;
  MrcCall   = Inputs->Call.Func;

  RegOffset = PCH_PWRM_BASE_ADDRESS + R_PCH_PWRM_GEN_PMCON_A;
  // The bits we care about are between Bit 16-23.  There are many RW/1C bits which we would like to avoid.
  // Thus we will read/write a UINT8 at the register offset +2 Bytes.
  RegOffset += 2;

  RegisterVal = MrcCall->MrcMmioRead8 (RegOffset);

  // Bit 23 is DISB.  So we want to set this.  Bit 23 of the register is bit 7 at RegOffset+2
  RegisterVal |=  MRC_BIT7;

  // Bit 18 and Bit 16 are RW/1C.  So we will set these bits to 0 before writing the register.
  // Bit 18 is Bit 2 of the Byte.
  // Bit 16 is Bit 0 of the Byte.
  Rw1cBits = MRC_BIT0 | MRC_BIT2;
  // Invert the mask so RW/1C bits are 0 and the rest are 1's.  Then AND this with the value to be written.
  Rw1cBits = ~Rw1cBits;
  RegisterVal &= Rw1cBits;

  MrcCall->MrcMmioWrite8 (RegOffset, RegisterVal);
}

/**
  This function resets the DISB bit in General PM Configuration.

  @param[in] MrcData - The global host structure

  @retval Nothing.
**/
void
MrcResetDISB (
  IN MrcParameters *const MrcData
  )
{
  MrcInput      *Inputs;
  MRC_FUNCTION  *MrcCall;
  UINT32        RegOffset;
  UINT8         RegisterVal;
  UINT8         Rw1cBits;

  Inputs    = &MrcData->Inputs;
  MrcCall   = Inputs->Call.Func;

  RegOffset = PCH_PWRM_BASE_ADDRESS + R_PCH_PWRM_GEN_PMCON_A;
  // The bits we care about are between Bit 16-23.  There are many RW/1C bits which we would like to avoid.
  // Thus we will read/write a UINT8 at the register offset +2 Bytes.
  RegOffset += 2;

  RegisterVal = MrcCall->MrcMmioRead8 (RegOffset);

  // Bit 18 and Bit 16 are RW/1C.  So we will set these bits to 0 before writing the register.
  // Bit 18 is Bit 2 of the Byte.
  // Bit 16 is Bit 0 of the Byte.
  Rw1cBits = MRC_BIT0 | MRC_BIT2;
  // Bit 23 is DISB.  So we want to clear this.  Bit 23 of the register is bit 7 at RegOffset+2
  Rw1cBits |=  MRC_BIT7;
  // Invert the mask so RW/1C bits and DISB are 0 and the rest are 1's.  Then AND this with the value to be written.
  Rw1cBits = ~Rw1cBits;

  RegisterVal &= Rw1cBits;

  MrcCall->MrcMmioWrite8 (RegOffset, RegisterVal);
}

/**
  This function checks if all DIMMs have RTT_NOM_RD >= 240 (RTT_NOM_RD = RZQ (240) or RTT_OFF)

  @param[in]  MrcData                   - The global host structure
  @param[out] IsRttNomRdGreaterThan240  - TRUE if RTT_NOM_RD >= 240 for all channels
                                          FALSE If any of the channels has RTT_NOM_RD < 240.

  @retval mrcSuccess if IsRttNomRdGreaterThan240 was calculated properly; otherwise a failing status.
**/
MrcStatus
MrcIsRttNomRdRzqOrOff (
  IN    MrcParameters  *const MrcData,
  OUT   BOOLEAN        *IsRttNomRdGreaterThan240
  )
{
  MrcDebug          *Debug;
  MrcDimmOut        *DimmOut;
  MrcOutput         *Outputs;
  UINT32            Controller;
  UINT32            Channel;
  UINT32            Dimm;
  UINT32            NonTargetRdOdt;
  TOdtValueDqDdr5   Ddr5OdtTableIndex;
  MrcStatus         Status;

  Outputs     = &MrcData->Outputs;
  Debug       = &Outputs->Debug;

  if (IsRttNomRdGreaterThan240 == NULL) {
    return mrcWrongInputParameter;
  }

  // Assume all DIMMs in the system have weak RttNomRd
  *IsRttNomRdGreaterThan240 = TRUE;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
        DimmOut = &Outputs->Controller[Controller].Channel[Channel].Dimm[Dimm];
        if (DimmOut->Status != DIMM_PRESENT) {
          continue;
        }

        Status = GetOdtTableIndex (MrcData, Controller, Channel, Dimm, &Ddr5OdtTableIndex);
        if (Status != mrcSuccess) {
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s GetOdtTableIndex failed!\n", gErrString);
          return Status;
        }

        NonTargetRdOdt = Ddr5OdtTableIndex.RttNomRd;
        // return false if any of the DIMMs is not RTT_NOM_RD = RZQ (240) or RTT_OFF
        if (!((NonTargetRdOdt >= 240) || (NonTargetRdOdt == 0))) {
          *IsRttNomRdGreaterThan240 = FALSE;
          return mrcSuccess;
        }
      }
    }
  }

  return mrcSuccess;
}

/**
  This function sets the safe mode configuration using the input override values.
  @param[in, out] MrcData - All the MRC global data.
  @retval Returns mrcSuccess if the timing overrides have been conpleted.
**/
MrcStatus
MrcSetSafeModeOverrides (
  IN OUT MrcParameters *const MrcData
  )
{
  MRC_FUNCTION *MrcCall;
  MrcInput  *Inputs;
  MRC_EXT_INPUTS_TYPE *ExtInputs;

  Inputs  = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;
  ExtInputs = Inputs->ExtInputs.Ptr;

  // Use pre-defined safe modes when the override bit is not set
  if ((ExtInputs->SafeModeOverride & MRC_SAFE_OVERRIDE_MC) == 0) {
    ExtInputs->McSafeMode = MRC_UINT8_MAX;  // enable all MC safemodes
  }
  if ((ExtInputs->SafeModeOverride & MRC_SAFE_OVERRIDE_DDR) == 0) {
    ExtInputs->DdrSafeMode = MRC_UINT32_MAX; // enable all DDR safemodes
  }

  //Note: logic is inverse to above safe modes, 0 disables the training
  if ((ExtInputs->SafeModeOverride & MRC_SAFE_OVERRIDE_TRAINING) == 0) {
      // Safe Mode Enabling Stage B
    ExtInputs->TrainingEnables.WRVC2D          = 0;
    ExtInputs->TrainingEnables.RDVC2D          = 0;
    ExtInputs->TrainingEnables.CMDVC           = 0;
    ExtInputs->TrainingEnables.LCT             = 0;
    ExtInputs->TrainingEnables3.LVRAUTOTRIM    = 0;
    ExtInputs->TrainingEnables3.TLINECLKCAL    = 0;

    // Safe Mode Enabling Stage C
    ExtInputs->TrainingEnables.WRDSEQT         = 0;
    ExtInputs->TrainingEnables.RDDQSODTT       = 0;
    ExtInputs->TrainingEnables3.RDDQODTT       = 0;
    ExtInputs->TrainingEnables.RDEQT           = 0;
    ExtInputs->TrainingEnables3.RDCTLET        = 0;
    ExtInputs->TrainingEnables2.DCCPICODELUT   = 0;
    ExtInputs->TrainingEnables2.FUNCDCCDQS     = 0;
    ExtInputs->TrainingEnables2.FUNCDCCCLK     = 0;
    ExtInputs->TrainingEnables2.FUNCDCCWCK     = 0;
    ExtInputs->TrainingEnables2.FUNCDCCDQ      = 0;
    ExtInputs->TrainingEnables2.DIMMODTT       = 0;
    ExtInputs->TrainingEnables2.DIMMRONT       = 0;
    ExtInputs->TrainingEnables2.TXTCO          = 0;
    ExtInputs->TrainingEnables2.CLKTCO         = 0;
    ExtInputs->TrainingEnables2.CMDSR          = 0;
    ExtInputs->TrainingEnables2.CMDDSEQ        = 0;
    ExtInputs->TrainingEnables2.DIMMODTCA      = 0;
    ExtInputs->TrainingEnables2.RDVREFDC       = 0;
    ExtInputs->TrainingEnables2.DATAPILIN      = 0;
    ExtInputs->TrainingEnables2.DDR5XTALK      = 0;
    ExtInputs->TrainingEnables2.WRTDIMMDFE     = 0;
    ExtInputs->TrainingEnables2.DCCLP5WCKDCA   = 0;
    ExtInputs->TrainingEnables3.WRTRETRAIN     = 0;
    ExtInputs->TrainingEnables3.OPTIMIZECOMP   = 0;
    ExtInputs->TrainingEnables3.DCCPISERIALCAL = 0;
    ExtInputs->TrainingEnables3.PHASECLKCAL    = 0;
    ExtInputs->TrainingEnables3.EMPHASIS       = 0;
    ExtInputs->TrainingEnables3.DIMMRXOFFSET   = 0;
    ExtInputs->TrainingEnables2.DCCLP5READDCA  = 0;

    // Safe Mode Enabling Stage D
    ExtInputs->TrainingEnables.ALIASCHK        = 0;
    ExtInputs->TrainingEnables.RMC             = 0;
    ExtInputs->TrainingEnables2.VCCCLKFF       = 0;
    Inputs->BdatEnable   = 0;

    // Safe Mode Enabling Undefined
    ExtInputs->TrainingEnables.RTL             = 0;
    ExtInputs->TrainingEnables.ERDMPRTC2D      = 0;
    ExtInputs->TrainingEnables.WRDSEQT         = 0;
    ExtInputs->TrainingEnables.DQSRF           = 0;
    ExtInputs->TrainingEnables.RMTEVENODD      = 0;
    ExtInputs->TrainingEnables.RCVENC1D        = 0;
    ExtInputs->TrainingEnables2.DLLDCC         = 0;
    ExtInputs->TrainingEnables2.DLLBWSEL       = 0;
    ExtInputs->TrainingEnables3.RXDQSDCC       = 0;
    ExtInputs->TrainingEnables3.DIMMNTODT      = 0;
    ExtInputs->TrainingEnables3.RXVREFPERBIT   = 0;
    ExtInputs->TrainingEnables3.WCKPADDCCCAL   = 0;
    ExtInputs->TrainingEnables3.QCLKDCC        = 0;
    ExtInputs->TrainingEnables3.WCKCLKPREDCC   = 0;
    ExtInputs->TrainingEnables3.DQSPADDCC      = 0;
    ExtInputs->TrainingEnables3.ISENSERMT      = 0;
    ExtInputs->TrainingEnables3.QCLKPHALIGN    = 0;
    ExtInputs->TrainingEnables3.RXDQSVOCC      = 0;
    ExtInputs->TrainingEnables3.WCKCLKRF       = 0;
    ExtInputs->MarginLimitCheck                = 0;
  }

  if ((ExtInputs->SafeModeOverride & MRC_SAFE_OVERRIDE_SAGV) == 0) {
    MrcSagvOverride (MrcData);
  }

  if (ExtInputs->SimicsFlag || ExtInputs->HsleFlag) {
    // Skip training / calibration steps which are not supported in Simics
    MrcCall->MrcSetMem ((UINT8 *) &ExtInputs->TrainingEnables,  sizeof (TrainingStepsEn),  0);
    MrcCall->MrcSetMem ((UINT8 *) &ExtInputs->TrainingEnables2, sizeof (TrainingStepsEn2), 0);
    MrcCall->MrcSetMem ((UINT8 *) &ExtInputs->TrainingEnables3, sizeof (TrainingStepsEn3), 0);
    ExtInputs->TrainingEnables.DUNITC = 1;
  }

  return mrcSuccess;
}

/**
  This function reads the input data structure and sets the appropriate overrides in the output structure.

  @param[in, out] MrcData - All the MRC global data.

  @retval Returns mrcSuccess if the timing overrides have been conpleted.
**/
MrcStatus
MrcSetOverrides (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcInput        *Inputs;
  MrcOutput       *Outputs;
  MrcSaveData     *SaveData;
  MrcDebug        *Debug;
  const UINT16    *RcompTarget;
  MrcDdrType      DdrType;
  MrcStatus       Status;
  MrcChannelOut   *ChannelOut;
  MrcDimmOut      *DimmOut;
  UINT32          Controller;
  UINT32          Channel;
  UINT32          Dimm;
  MrcFrequency    LowFreq;
  UINT8           Index;
  UINT8           MemoryProfile;
  BOOLEAN         lDramDqOdtEn;
  MRC_LP5_BANKORG Lp5BankOrg;
  MRC_EXT_INPUTS_TYPE *ExtInputs;

  Inputs      = &MrcData->Inputs;
  Outputs     = &MrcData->Outputs;
  SaveData    = &MrcData->Save.Data;
  Debug       = &Outputs->Debug;
  DdrType     = Outputs->DdrType;
  ExtInputs   = Inputs->ExtInputs.Ptr;
  Status      = mrcSuccess;
  RcompTarget = NULL;
  Outputs->EccSupport = ExtInputs->EccSupport != 0;
  Outputs->VoltageDone = FALSE;
  Outputs->CaDeselectStress = FALSE;

  Outputs->IpModel = MrcIpModelGet (MrcData);
  Lp5BankOrg = MrcGetBankBgOrg (MrcData, Outputs->Frequency);

  Outputs->OdtMode  = ExtInputs->IoOdtMode;

  // Project specific overides
  MrcSetPrjOverrides (MrcData);

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "WeaklockEn: %sabled\n", Outputs->WeaklockEn ? "En" : "Dis");

  if (Inputs->DramDqOdt == MrcAuto) {
    lDramDqOdtEn = TRUE;
  } else {
    lDramDqOdtEn = (Inputs->DramDqOdt == MrcEnable) ? TRUE : FALSE;
  }
  Outputs->DramDqOdtEn  = lDramDqOdtEn;

  // Configure LowFrequency
  switch (DdrType) {
    case MRC_DDR_TYPE_LPDDR5:
      LowFreq = LP5_LOW_FREQ_INIT;
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "LPDDR Low Frequency: %d\n", LowFreq);
      break;

    default:
      LowFreq = fInvalid;
      break;
  }
  Outputs->LowFrequency = LowFreq;

  // If RcompResistors are not zero, then user is overriding default termination
  if (ExtInputs->RcompResistor == 0) {
    ExtInputs->RcompResistor = 100; // All platform designs default to 100 Ohm.
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Updating Rcomp Resistors: %u\n", ExtInputs->RcompResistor);
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Outputs->MaxRanks: %u\n", Outputs->MaxRanks);
  RcompTarget = MrcGetDefaultRcompTarget(MrcData, ExtInputs->MemoryProfile);
  if (NULL == RcompTarget) {
    Status = mrcFail;
  }
#ifdef MRC_DEBUG_PRINT
  if ((ExtInputs->RcompTarget[0] == 0) || (ExtInputs->RcompTarget[1] == 0) || (ExtInputs->RcompTarget[2] == 0) ||
      (ExtInputs->RcompTarget[3] == 0) || (ExtInputs->RcompTarget[4] == 0)) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Updating Rcomp Targets:\n");
  }
#endif // MRC_DEBUG_PRINT
  for (Index = 0; Index < MAX_RCOMP_TARGETS; Index++) {
    if ((ExtInputs->RcompTarget[Index] == 0) && (RcompTarget != NULL)) {
      Outputs->RcompTarget[Index] = RcompTarget[Index];
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " RcompTarget[%s]: %u\n", GlobalCompOffsetStr[Index], Outputs->RcompTarget[Index]);
    } else {
      Outputs->RcompTarget[Index] = ExtInputs->RcompTarget[Index];
    }
  }

  //  LP5           VSS
  //  Ddr5          VDDq
  if (Outputs->OdtMode == MrcOdtModeDefault) {
    switch (DdrType) {
      case MRC_DDR_TYPE_DDR5:
        Outputs->OdtMode = MrcOdtModeVddq;
        break;

      case MRC_DDR_TYPE_LPDDR5:
        Outputs->OdtMode = MrcOdtModeVss;
        break;

      default:
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s: %d\n", gErrString, gUnsupportedTechnology, DdrType);
        Status = mrcFail;
        break;
    }
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "DDRIO ODT Mode: %s\n", gIoOdtModeStr[Outputs->OdtMode]);

  // If we have ECT disabled, set the ECT done flag so Reset Flows will behave normally for DDR5 and LPDDR systems.
  if ((ExtInputs->TrainingEnables.ECT == 0) || (ExtInputs->SimicsFlag == 1)) {
    Outputs->EctDone = TRUE;
  }

  // If Deswizzle mapping step is disabled, disabled the oscillator for LPDDR as
  //  it depends on deswizzling to work properly
  if ((!ExtInputs->TrainingEnables2.DQDQSSWZ) && (Outputs->IsLpddr)) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "%s Disabling WRTRETRAIN as DQ Mapping is not enabled\n", gWarnString);
    ExtInputs->TrainingEnables3.WRTRETRAIN = 0;
  }

  Inputs->IsApplyMrCommandDelays = TRUE;

  // Watermark granularity support; if RefreshWm == HIGH or contains invalid value, set to default watermark HIGH values
  if (ExtInputs->RefreshWm == REFRESH_WM_LOW) {
    Outputs->RefreshPanicWm = REFRESH_PANIC_WM_LOW;
    Outputs->RefreshHpWm    = REFRESH_HP_WM_LOW;
  } else {
    Outputs->RefreshPanicWm = REFRESH_PANIC_WM_HIGH;
    Outputs->RefreshHpWm    = REFRESH_HP_WM_HIGH;
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RefreshPanicWm: %u\nRefreshHpWm: %u\n",  Outputs->RefreshPanicWm, Outputs->RefreshHpWm);

  MrcData->Save.Data.CpgcGlobalStart = TRUE;  // Start all CPGC engines together

  // Set Default Read Preamble
  // LP5: will be updated to a longer preamble after LockUI if needed
  // DDR5: LockUI won't change RDPRE
  if (Outputs->IsDdr5) {
    Outputs->ReadPreamble = (Outputs->Frequency < f4000) ? tRPRE_ALL_FREQ_DDR5_2tCK : (Outputs->Frequency == f4000) ? tRPRE_ALL_FREQ_DDR5_3tCK : tRPRE_ALL_FREQ_DDR5_4tCK;
  } else { // LP5
    Outputs->ReadPreamble = MRC_LP5_tRPRE_TOGGLE_2tWCK;
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "ReadPreamble: %u\n", Outputs->ReadPreamble);



  if (Outputs->IsLpddr5) {
    // Override Lpddr5 Bank / Bank Group count depending on bank group mode
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Lpddr5 Bank/BankGroup Override:\n");
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DimmOut->Status != DIMM_PRESENT) {
            continue;
          }
          // Override bank value depending on bank group mode
          if (Lp5BankOrg == MrcLp5BgMode) {
            DimmOut->Banks = 4;
            DimmOut->BankGroups = 4;
            DimmOut->ColumnSize = 1 << 10;
          } else if (Lp5BankOrg == MrcLp516Bank) {
            DimmOut->Banks = 16;
            DimmOut->BankGroups = 1;
            DimmOut->ColumnSize = 1 << 10;
          }
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Mc%u.Ch%u.Dimm%u: %u Banks in %u Bank Groups; ", Controller, Channel, Dimm, DimmOut->Banks, DimmOut->BankGroups);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "ColumnSize: %u\n", DimmOut->ColumnSize);
        }
      }
    }

    // There is a limitation for MC to perform DVFSQ_LOW->DVFSQ_LOW SAGV transition, due to ZQ-stop limitation.
    // Transition from DVFSQ_LOW->DVFSQ_LOW can still be supported for debug, if ALL SAGV point are DVFSQ_LOW and
    // MRS FSM is programmed with no ZQ operation whatsoever. in that mode ZQ should be completely disabled, both periodic,
    // SRX exit and in MRS FSM for SAGV.  The request: (a) no ZQ whatsoever, (b) More than 1 DVFSQ_LOW point is allowed
    // 2 options will be supported: 1 DVFSQ point or all points are configured as DVFSQ points
    Outputs->IsZqDisabled = FALSE;
    //
    // Set Outputs->IsDvfsqEnabled based on frequency, and limit to lowest GV point only.
    // DVFSQ low (enabled) is restricted to single point, as all GV points must write same MRs at the same time,
    // and DVFSQ high must write MR28 before FSP-OP change. However, to avoid breaking any requirements, DVFSQ low would then need to write
    // contradictory data depending on whether switching from High to Low, vs staying Low to Low.
    // This is not possible to program with current register implementation, so prevent Low to Low transition by restricting DVFSQ Low to only GV0
    // Setting Outputs->IsZqDisabled allows us to overcome different SAGV points DVFSQ restriction - Can run multipule points
    //
    if (ExtInputs->DvfsqEnabled && (Outputs->HighFrequency <= f3200) && (Outputs->IsZqDisabled || (Outputs->SaGvPoint == 0) || !MrcIsSaGvEnabled (MrcData))) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "DVFSQ is enabled\n");
      Outputs->IsDvfsqEnabled = TRUE;
    }

    // Set Outputs->IsDvfscEnabled based on Inputs->IsDvfscEnable
    Outputs->IsDvfscEnabled = FALSE;
    if ((ExtInputs->DvfscEnabled && (Outputs->Frequency <= f3200)) && ((Outputs->SaGvPoint == 0) || !MrcIsSaGvEnabled (MrcData)) && (ExtInputs->BoardDetails.SingleVdd2Rail == 0)) {
      Outputs->IsDvfscEnabled = TRUE;
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "IsDvfscEnabled: %u\n", Outputs->IsDvfscEnabled);

    //
    // In Cold boot or Fast boot, start with 0.5V until after JEDEC Init. And after training, if applicable.
    // However, if memory in Self-Refresh (warm boot or S3 boot), start with 0.3V for DVFSQ enabled GV point.
    //
    Outputs->VccddqVoltage = VDD_0_50;
    if (Outputs->IsDvfsqEnabled && ((Inputs->BootMode == bmWarm) || (Inputs->BootMode == bmS3))) {
      Outputs->VccddqVoltage = VDD_0_30;
    }
  } else {
    MemoryProfile = ExtInputs->MemoryProfile;
    // DDR5 should be 1.1V unless changed for overclocking
    if (SaveData->IsXmpSagvEnabled || SaveData->DynamicMemoryBoostTrainingFailed) {
      MemoryProfile = Inputs->MemoryProfileSave;
    }
    Outputs->VccddqVoltage = Outputs->VddVoltage[MemoryProfile];
    if (Outputs->IsOCProfile || SaveData->IsXmpSagvEnabled) {
      Outputs->Vdd2Mv = Outputs->VccddqVoltage;
    }

    // Assign Vdd2Mv
    if ((Outputs->IsOCProfile) && (ExtInputs->Vdd2Mv != 0)) {
      Outputs->Vdd2Mv = ExtInputs->Vdd2Mv;
    }
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "VccDdq(mV) = %u\n", Outputs->VccddqVoltage);
  SetVccClk (MrcData);
  SetVccIo (MrcData);

  Outputs->VccDist = VDD_0_875;

  if (ExtInputs->VddqVoltageOverride && !Outputs->IsLpddr) {
    Outputs->VccddqVoltage = ExtInputs->VddqVoltageOverride;
  }
  if (ExtInputs->VccIogVoltageOverride) {
    Outputs->VccIogVoltage = ExtInputs->VccIogVoltageOverride;
  }
  if (ExtInputs->VccClkVoltageOverride) {
    Outputs->VccClkVoltage = ExtInputs->VccClkVoltageOverride;
  }

  Outputs->IsCmdNormalizationEnabled = FALSE;

  if (ExtInputs->DqLoopbackTest) {
    ExtInputs->TrainingEnables.ECT       = 0;
    ExtInputs->TrainingEnables2.DQDQSSWZ = 0;
    ExtInputs->TrainingEnables.JWRL      = 0;
    ExtInputs->TrainingEnables3.RXDQSDCC = 0;
  }

  return Status;
}

/**
  This function get the current value of the sticky scratchpad register.

  @param[in] MrcData - include all the MRC data.

  @retval The current value of the sticky scratchpad register.
**/
UINT64
MrcWmRegGet (
  IN     MrcParameters *const MrcData
  )
{
  return (MrcReadCR64 (MrcData, SSKPD_PCU_REG));
}

/**
  This function Set a newvalue of the sticky scratchpad register by set new givin Bit(s)

  @param[in] MrcData   - include all the MRC data.
  @param[in] SskpdBits - Bit(s) Need to Set

**/
void
MrcWmRegSetBits (
  IN     MrcParameters *const MrcData,
  IN     UINT64        SskpdBits
)
{
  UINT64 Sskpd;

  Sskpd = MrcReadCR64 (MrcData, SSKPD_PCU_REG);
  MrcWriteCR64(MrcData, SSKPD_PCU_REG, (Sskpd | SskpdBits));
}

/**
  This function Set a newvalue of the sticky scratchpad register by clear givin Bit(s)

  @param[in] MrcData   - include all the MRC data.
  @param[in] SskpdBits - Bit(s) Need to Clear

**/
void
MrcWmRegClrBits(
  IN     MrcParameters *const MrcData,
  IN     UINT64        SskpdBits
  )
{
  UINT64 Sskpd;

  Sskpd = MrcReadCR64 (MrcData, SSKPD_PCU_REG);
  MrcWriteCR64(MrcData, SSKPD_PCU_REG, Sskpd & (~SskpdBits));
}

/**
  Gets pointers to functions inside of core.

  @param[out] CallChannelExist            - Pointer to the function MrcChannelExist
  @param[out] CallPrintf                  - Pointer to the function MrcPrintf
  @param[out] CallSignExtend              - Pointer to the function MrcSignExtend

  @retval Returns mrcSuccess if the function succeeds.
**/
MrcStatus
MrcGetCoreFunction (
  OUT UINTN                    *CallChannelExist,
  OUT UINTN                    *CallPrintf,
  OUT UINTN                    *CallSignExtend
  )
{
  *CallChannelExist            = (UINTN) &MrcChannelExist;
  *CallPrintf                  = (UINTN) &MrcPrintf;
  *CallSignExtend              = (UINTN) &MrcSE;
  return (mrcSuccess);
}

/**
  Read the current DDRPHY temperature.

  @param[in]  MrcData     - Include all MRC global data.
  @param[out] Temperature - DDRPHY temperature in degrees C

  @retval Status - whether the temperature read was successful
**/
MrcStatus
MrcReadTemperature (
  IN  MrcParameters *const MrcData,
  OUT INT32                *Temperature
  )
{
  MrcStatus                 Status = mrcFail;
  const MRC_FUNCTION        *MrcCall;
  MrcOutput                 *Outputs;
  MrcDebug                  *Debug;
  UINT32                    MailboxCommand;
  UINT32                    MailboxStatus;
  DDRPHY_TEMPERATURE_RESULT DtResult;
  INT32                     Temp1;
  BOOLEAN                   Temp1Valid;
  INT32                     Temp2;
  BOOLEAN                   Temp2Valid;
  UINT8                     Loop;

  MrcCall = MrcData->Inputs.Call.Func;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  Temp1Valid = FALSE;
  Temp2Valid = FALSE;
  Temp1   = 0;
  Temp2   = 0;
  Status  = mrcSuccess;

  for (Loop = 0; Loop < 5; Loop++) {
    MailboxCommand = CPU_MAILBOX_BIOS_CMD_READ_DDRPHY_TEMPERATURE;
    MrcCall->MrcCpuMailboxRead (MailboxCommand, &DtResult.Data, &MailboxStatus);
    if (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s: %s: Status=0x%x, Data=0x%x\n", gErrString, "MAILBOX_BIOS_CMD_READ_DDRPHY_TEMPERATURE", MailboxStatus, DtResult.Data);
      continue;
    }

    if (!Temp1Valid && DtResult.Bits.DdrphyTemperature1Valid) {
      Temp1 = DtResult.Bits.DdrphyTemperature1;
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "PHY Temperature%u: %d C\n", 1, Temp1);
      Temp1Valid = TRUE;
    }
    if (!Temp2Valid && DtResult.Bits.DdrphyTemperature2Valid) {
      Temp2 = DtResult.Bits.DdrphyTemperature2;
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "PHY Temperature%u: %d C\n", 2, Temp2);
      Temp2Valid = TRUE;
    }

    if (Temp1Valid && Temp2Valid) {
      // Average the two temperatures if both are valid
      *Temperature = (Temp1 + Temp2) / 2;
      break;
    }
  }
  if (Temp1Valid) {
    *Temperature = Temp1;
  } else if (Temp2Valid) {
    *Temperature = Temp2;
  } else {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s: %s: Temperature is not valid for either diode\n", gErrString, "MAILBOX_BIOS_CMD_READ_DDRPHY_TEMPERATURE");
    *Temperature = DDRPHY_TEMPERATURE_INVALID;
    Status = mrcFail;
  }
  return Status;
}

#ifdef MRC_DEBUG_PRINT
/**
  Print the input parameters to the debug message output port.

  @param[in] MrcData - The MRC global data.

  @retval mrcSuccess
**/
MrcStatus
MrcPrintInputParameters (
  IN MrcParameters *const MrcData
  )
{
  MrcDebug                *Debug;
  const MrcInput          *Inputs;
  MrcOutput               *Outputs;
  const MrcControllerIn   *ControllerIn;
  const MrcChannelIn      *ChannelIn;
  const MrcDimmIn         *DimmIn;
  const MrcTiming         *Timing;
  const TrainingStepsEn   *TrainingSteps;
  const TrainingStepsEn2  *TrainingSteps2;
  const TrainingStepsEn3  *TrainingSteps3;
  const UINT8             *Buffer;
  const MRC_EXT_INPUTS_TYPE *ExtInputs;
  UINT32                  Line;
  UINT32                  Address;
  UINT32                  Offset;
  UINT32                  MaxSpd;
  UINT8                   Controller;
  UINT8                   Channel;
  UINT8                   Dimm;
  UINT32                  Index;
  CHAR8                   HexDump[16 * 3 + 16 + 1];
  CHAR8                   *p;
  UINT8                   Data8;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  ExtInputs = Inputs->ExtInputs.Ptr;

  // The following are system level definitions. All memory controllers in the system are set to these values.
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "Debug Items:\n"
    "\tDebug.Stream: %ph\n"
    "\tDebug.Level: %Xh\n",
    Debug->Stream.Ptr,
    Debug->Level
    );
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tSafeModeOverride: %Xh\n"
    "\tMcSafeMode: %Xh\n"
    "\tDdrSafeMode: %Xh\n"
    "\tProbelessTrace: %u\n"
    "\tForce1Dpc: %u\n"
    "\tForceSingleRank: %u\n"
    "\tEnablePda: %u\n",
    ExtInputs->SafeModeOverride,
    ExtInputs->McSafeMode,
    ExtInputs->DdrSafeMode,
    ExtInputs->ProbelessTrace,
    ExtInputs->Force1Dpc,
    ExtInputs->ForceSingleRank,
    Inputs->EnablePda
    );

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tNumCL: %u\n"
    "\tLoopCount: %u\n"
    "\tDqLoopbackTest: %u\n"
    "\tRxDqsStepSizeLB: %u\n"
    "\tRxVrefStepSizeLB: %u\n"
    "\tTxStepSizeLB: %u\n"
    "\tEnPeriodicComp: %u\n"
    "\tDunitTatOptimization: %u\n",
    Inputs->NumCL,
    Inputs->LoopCount,
    ExtInputs->DqLoopbackTest,
    ExtInputs->RxDqsStepSizeLB,
    ExtInputs->RxVrefStepSizeLB,
    ExtInputs->TxStepSizeLB,
    ExtInputs->EnPeriodicComp,
    ExtInputs->DunitTatOptimization
    );

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\tDisableChannel:\n");
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\t\tM[%u]C[%u]:%u\n", Controller, Channel, ExtInputs->DisableChannel[Controller][Channel]);
    }
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\nPlatform Config Items:\n"
    "\tMemoryProfile: %Xh\n"
    "\tSkuType: %s\n"
    "\tCmdRanksTerminated: %Xh\n"
    "\tDqPinsInterleaved: %u\n"
    "\tCccPinsInterleaved: %u\n",
    ExtInputs->MemoryProfile,
    gMrcSkuTypeStr[Inputs->SkuType < MrcSkuTypeMax ? Inputs->SkuType : 0],
    ExtInputs->CmdRanksTerminated,
    ExtInputs->DqPinsInterleaved,
    ExtInputs->CccPinsInterleaved
    );
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tTscTimeBase: %llu\n",
    Inputs->TscTimeBase
    );
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tDdrIoIpVersion: %Xh\n"
    "\tDdrIoIpDerivative: %Xh\n"
    "\tDdrIoIpSegment: %Xh\n"
    "\tDdrIoIpStepping: %Xh\n"
    "\tBootMode: %Xh\n"
    "\tTxtClean: %Xh\n"
    "\tSimicsFlag: %Xh\n"
    "\tHsleFlag: %Xh\n",
    Inputs->DdrIoIpVersion.Bits.Version,
    Inputs->DdrIoIpVersion.Bits.Derivative,
    Inputs->DdrIoIpVersion.Bits.Segment,
    Inputs->DdrIoIpVersion.Bits.Stepping,
    Inputs->BootMode,
    Inputs->TxtClean,
    ExtInputs->SimicsFlag,
    ExtInputs->HsleFlag
    );
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tBaseAddresses\n"
    "\t\tPciE: %llXh\n"
    "\t\tMchBar: %llXh\n"
    "\t\tSmbus: %Xh\n"
    "\tMeStolenSize: %Xh\n"
    "\tMmioSize: %Xh\n"
    "\tTsegSize: %Xh\n"
    "\tDprSize: %Xh\n"
    "\tPrmrrSize: %Xh\n",
    Inputs->PciEBaseAddress,
    Inputs->MchBarBaseAddress,
    Inputs->SmbusBaseAddress,
    Inputs->MeStolenSize,
    Inputs->MmioSize,
    Inputs->TsegSize,
    Inputs->DprSize,
    Inputs->PrmrrSize
    );
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tChHashOverride: %Xh\n"
    "\tChHashEnable: %Xh\n"
    "\tChHashMask: %Xh\n"
    "\tChHashInterleaveBit: %u\n"
    "\tExtendedBankHashing: %Xh\n"
    "\tPerBankRefresh: %Xh\n",
    ExtInputs->ChHashOverride,
    ExtInputs->ChHashEnable,
    ExtInputs->ChHashMask,
    ExtInputs->ChHashInterleaveBit,
    ExtInputs->ExtendedBankHashing,
    ExtInputs->PerBankRefresh
    );
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tMsHashOverride: %u\n"
    "\tMsHashMask: %Xh\n"
    "\tMsHashInterleaveBit: %u\n"
    "\tSubChHashOverride: %Xh\n"
    "\tSubChHashMask: %Xh\n"
    "\tSubChHashInterleaveBit: %u\n",
    ExtInputs->MsHashOverride,
    ExtInputs->MsHashMask,
    ExtInputs->MsHashInterleaveBit,
    ExtInputs->SubChHashOverride,
    ExtInputs->SubChHashMask,
    ExtInputs->SubChHashInterleaveBit
  );
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tLp5CccConfig: 0x%x\n"
    "\tCmdMirror: 0x%x\n"
    "\tBdatEnable: %u\n"
    "\tBdatTestType: %u\n"
    "\tIsCs2NRequested: %u\n"
    "\tRowPressEn: %x\n",
    ExtInputs->Lp5CccConfig,
    ExtInputs->CmdMirror,
    Inputs->BdatEnable,
    Inputs->BdatTestType,
    Inputs->IsCs2NRequested,
    ExtInputs->RowPressEn
    );

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tDynamicMemoryBoost: %u\n"
    "\tRealtimeMemoryFrequency: %u\n" ,
    ExtInputs->DynamicMemoryBoost,
    ExtInputs->RealtimeMemoryFrequency
    );

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tUcPayloadAddress: %llXh\n"
    "\tIsKeepUcssPostMrc: %u\n"
    "\tIsIOTestAddressRandom: %u\n"
    "\tIsCaDeselectStress: %u\n"
    "\tRxDqVrefPerBit: %u\n"
    "\tFourToggleReadPreamble: %u\n",
    Inputs->UcPayloadAddress,
    Inputs->IsKeepUcssPostMrc,
    Inputs->IsIOTestAddressRandom,
    Inputs->IsCaDeselectStress,
    Inputs->RxDqVrefPerBit,
    Inputs->FourToggleReadPreamble
    );

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\nDRAM setup Items:\n"
    "\tFreqMax: %u\n"
    "\tGearRatio: %u\n"
    "\tRatio: %u\n"
    "\tRefClk: %uMHz\n"
    "\tMarginLimitCheck: %d\n"
    "\tMarginLimitL2: %d\n",
    Inputs->FreqMax,
    ExtInputs->GearRatio,
    ExtInputs->Ratio,
    REF_FREQ,
    ExtInputs->MarginLimitCheck,
    ExtInputs->MarginLimitL2
    );
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tVddqVoltageOverride(# of 1mV): %u\n"
    "\tVccIogVoltageOverride(# of 1mV): %u\n"
    "\tVccClkVoltageOverride(# of 1mV): %u\n",
    ExtInputs->VddqVoltageOverride,
    ExtInputs->VccIogVoltageOverride,
    ExtInputs->VccClkVoltageOverride
    );
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tVdd2Mv: %u mV\n"
    "\tVddVoltage: %u mV\n"
    "\tVddqVoltage: %u mV\n"
    "\tVppVoltage: %u mV\n",
    ExtInputs->Vdd2Mv,
    ExtInputs->VddVoltage,
    ExtInputs->VddqVoltage,
    ExtInputs->VppVoltage
    );
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tEccSupport: %Xh\n"
    "\tCleanMemory: %u\n"
    "\tExtInputs Ibecc: %Xh\n"
    "\tInputs IsIbeccEnabled: %d\n"
    "\tIbeccOperationMode: 0x%X\n"
    "\tIbeccParity: 0x%X\n",
    ExtInputs->EccSupport,
    Inputs->CleanMemory,
    ExtInputs->Ibecc,
    Inputs->IsIbeccEnabled,
    ExtInputs->IbeccOperationMode,
    ExtInputs->IbeccParity
    );

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tIbeccEccInjControl: 0x%X\n"
    "\tIbeccEccInjAddrBase: 0x%X\n"
    "\tIbeccEccInjCount: 0x%X\n",
    ExtInputs->IbeccEccInjControl,
    ExtInputs->IbeccEccInjAddrBase,
    ExtInputs->IbeccEccInjCount
    );

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tScramblerSupport: %Xh\n"
    "\tLpFreqSwitch: %Xh\n"
    "\tReadMprVA: %Xh\n"
    "\tAutoSelfRefreshSupport: %u\n"
    "\tExtTemperatureSupport: %u\n",
    ExtInputs->ScramblerSupport,
    Inputs->LpFreqSwitch,
    Inputs->ReadMprVA,
    ExtInputs->AutoSelfRefreshSupport,
    ExtInputs->ExtTemperatureSupport
    );
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tDramRfmMode: %u\n"
    "\tTargetedRowRefreshMode: %u\n"
    "\tDrfmBrc: %u\n"
    "\tSpineGatePerLpmode: %Xh\n"
    "\tPhclkGatePerLpmode: %Xh\n",
    ExtInputs->DramRfmMode,
    ExtInputs->TargetedRowRefreshMode,
    ExtInputs->DrfmBrc,
    ExtInputs->SpineGatePerLpmode,
    ExtInputs->PhclkGatePerLpmode
    );
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tLfsr0Mask: %u\n"
    "\tLfsr1Mask: %u\n"
    "\tRefreshWm: %u\n"
    "\tRetrainOnFastFail: %u\n",
    ExtInputs->Lfsr0Mask,
    ExtInputs->Lfsr1Mask,
    ExtInputs->RefreshWm,
    ExtInputs->RetrainOnFastFail
    );
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tDynamicRangeLow: %d\n"
    "\tIteration: %Xh\n"
    "\tMbist: %Xh\n"
    "\tRemapEnable: %Xh\n"
    "\tVoltageReadout: %u\n",
    ExtInputs->DynamicRangeLow,
    Inputs->Iteration,
    Inputs->Mbist,
    ExtInputs->RemapEnable,
    ExtInputs->VoltageReadout
    );
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tPowerDownMode: %Xh\n"
    "\tDisPgCloseIdleTimeout: %Xh\n"
    "\tPwdwnIdleCounter: %Xh\n"
    "\tRankInterleave: %Xh\n",
    ExtInputs->PowerDownMode,
    ExtInputs->DisPgCloseIdleTimeout,
    ExtInputs->PwdwnIdleCounter,
    ExtInputs->RankInterleave
    );
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tEnhancedInterleave: %Xh\n"
    "\tWeaklockEn: %u\n"
    "\tRmtPerTask: %d\n"
    "\tTrainTrace: %d\n",
    ExtInputs->EnhancedInterleave,
    ExtInputs->WeaklockEn,
    ExtInputs->RmtPerTask,
    ExtInputs->TrainTrace
    );
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tDramDqOdt: %u\n"
    "\tNonTargetOdtEn: %u\n"
    "\tLpMode: %u\n"
    "\tLpMode4: %u\n"
    "\tDvfscEnabled: %u\n"
    "\tDvfsqEnabled: %u\n",
    Inputs->DramDqOdt,
    Inputs->NonTargetOdtEn,
    ExtInputs->LpMode,
    ExtInputs->LpMode4,
    ExtInputs->DvfscEnabled,
    ExtInputs->DvfsqEnabled
    );

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tPprRunOnce: %u\n"
    "\tPprTestType: %Xh\n"
    "\tPprRepairType: %u\n"
    "\tPprRetryLimit: %d\n"
    "\tPprErrorInjection: %u\n"
    "\tPprForceRepair: %u\n",
    Inputs->PprRunOnce,
    (UINT32) Inputs->PprTestType.Value,
    (UINT32) Inputs->PprRepairType,
    Inputs->PprRetryLimit,
    Inputs->PprErrorInjection,
    Inputs->PprForceRepair
  );

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tPprRepairController: %u\n"
    "\tPprRepairChannel: %u\n"
    "\tPprRepairDimm: %u\n"
    "\tPprRepairRank: %u\n"
    "\tPprRepairBankGroup: %u\n"
    "\tPprRepairBank: %u\n",
    ExtInputs->PprRepairController,
    ExtInputs->PprRepairChannel,
    ExtInputs->PprRepairDimm,
    ExtInputs->PprRepairRank,
    ExtInputs->PprRepairBankGroup,
    ExtInputs->PprRepairBank
  );

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "LowerBasicMemTestSize: %d\n", ExtInputs->LowerBasicMemTestSize);

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,"\tRetrainToWorkingChannel: %u\n", ExtInputs->RetrainToWorkingChannel);

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tTrrsg: %d\n"
    "\tTrrdg: %d\n"
    "\tTrrdr: %d\n"
    "\tTrrdd: %d\n"
    "\tTrwsg: %d\n"
    "\tTrwdg: %d\n"
    "\tTrwdr: %d\n"
    "\tTrwdd: %d\n",
    ExtInputs->Trrsg,
    ExtInputs->Trrdg,
    ExtInputs->Trrdr,
    ExtInputs->Trrdd,
    ExtInputs->Trwsg,
    ExtInputs->Trwdg,
    ExtInputs->Trwdr,
    ExtInputs->Trwdd
    );
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tTwrsg: %d\n"
    "\tTwrdg: %d\n"
    "\tTwrdr: %d\n"
    "\tTwrdd: %d\n"
    "\tTwwsg: %d\n"
    "\tTwwdg: %d\n"
    "\tTwwdr: %d\n"
    "\tTwwdd: %d\n",
    ExtInputs->Twrsg,
    ExtInputs->Twrdg,
    ExtInputs->Twrdr,
    ExtInputs->Twrdd,
    ExtInputs->Twwsg,
    ExtInputs->Twwdg,
    ExtInputs->Twwdr,
    ExtInputs->Twwdd
    );

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tReplicateSagv: %u\n",
    ExtInputs->ReplicateSagv
    );

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tAsyncOdtDis: %u\n"
    "\tCccHalfFrequency: %u\n"
    "\tBoardStackUp: %u\n"
    "\tSingleVdd2Rail: %u\n",
    ExtInputs->AsyncOdtDis,
    ExtInputs->CccHalfFrequency,
    ExtInputs->BoardDetails.BoardStackUp,
    ExtInputs->BoardDetails.SingleVdd2Rail
    );
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tForceRetrainPath: %Xh\n"
    "\tDisable2CycleBypass: %Xh\n"
    "\tInterpreter: %u\n"
    "\tEnterInterpreterOnPostCode: %u\n",
    ExtInputs->ForceRetrainPath,
    ExtInputs->Disable2CycleBypass,
    ExtInputs->Interpreter,
    Inputs->EnterInterpreterOnPostCode
    );
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tOpportunisticRead: %Xh\n"
    "\tPcuDdrVoltage: %Xh\n"
    "\tIoOdtMode: %u\n"
    "\tDdr5AutoPrechargeEnable: %X\n"
    "\tLp5SplitACTEnable: %X\n",
    ExtInputs->OpportunisticRead,
    ExtInputs->PcuDdrVoltage,
    ExtInputs->IoOdtMode,
    ExtInputs->Ddr5AutoPrechargeEnable,
    ExtInputs->Lp5SplitACTEnable
    );

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE,
    "\tForceCkdBypass: %Xh\n"
    "\tIsDdrphyx64: %Xh\n"
    "\tSenseAtRxDll: %Xh\n"
    "\tWckModeOverride: %u\n",
    ExtInputs->ForceCkdBypass,
    Inputs->IsDdrphyx64,
    Inputs->SenseAtRxDll,
    ExtInputs->WckModeOverride
    );

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n%s*****    MRC TRAINING STEPS     *****\n%s", PrintBorder, PrintBorder);
  TrainingSteps = &ExtInputs->TrainingEnables;
  TrainingSteps2 = &ExtInputs->TrainingEnables2;
  TrainingSteps3 = &ExtInputs->TrainingEnables3;
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s:\n", "TrainingEnables");
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "ECT: %u\nSOT: %u\nERDMPRTC2D: %u\nRDMPRT: %u\n",             TrainingSteps->ECT,             TrainingSteps->SOT,       TrainingSteps->ERDMPRTC2D,  TrainingSteps->RDMPRT);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RCVET: %u\nJWRL: %u\nEWRTC2D: %u\nERDTC2D: %u\n",            TrainingSteps->RCVET,           TrainingSteps->JWRL,      TrainingSteps->EWRTC2D,     TrainingSteps->ERDTC2D);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "UNMATCHEDWRTC1D: %u\nWRTC1D: %u\nWRVC1D: %u\nRDTC1D: %u\n",  TrainingSteps->UNMATCHEDWRTC1D, TrainingSteps->WRTC1D,    TrainingSteps->WRVC1D,      TrainingSteps->RDTC1D);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RDVC1D: %u\nWRTC2D: %u\nRDTC2D: %u\nWRVC2D: %u\n",           TrainingSteps->RDVC1D,          TrainingSteps->WRTC2D,    TrainingSteps->RDTC2D,      TrainingSteps->WRVC2D);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RDVC2D: %u\nWRDSEQT: %u\nDQSRF: %u\nRDDQSODTT: %u\n",        TrainingSteps->RDVC2D,          TrainingSteps->WRDSEQT,   TrainingSteps->DQSRF,       TrainingSteps->RDDQSODTT);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RDEQT: %u\nDUNITC: %u\nCMDVC: %u\nLCT: %u\n",                TrainingSteps->RDEQT,           TrainingSteps->DUNITC,    TrainingSteps->CMDVC,       TrainingSteps->LCT);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RTL: %u\nTAT: %u\nRMT: %u\nRMTEVENODD: %u\n",                TrainingSteps->RTL,             TrainingSteps->TAT,       TrainingSteps->RMT,         TrainingSteps->RMTEVENODD);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "ALIASCHK: %u\nRCVENC1D: %u\nRMC: %u\nPRETRAIN: %u\n",        TrainingSteps->ALIASCHK,        TrainingSteps->RCVENC1D,  TrainingSteps->RMC,         TrainingSteps->PRETRAIN);

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s2:\n", "TrainingEnables");
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "DCCPICODELUT: %u\nDIMMODTT: %u\nDIMMRONT: %u\nTXTCO: %u\n",              TrainingSteps2->DCCPICODELUT,   TrainingSteps2->DIMMODTT,       TrainingSteps2->DIMMRONT,       TrainingSteps2->TXTCO);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "CLKTCO: %u\nCMDSR: %u\nCMDDSEQ: %u\nDIMMODTCA: %u\n",                    TrainingSteps2->CLKTCO,         TrainingSteps2->CMDSR,          TrainingSteps2->CMDDSEQ,        TrainingSteps2->DIMMODTCA);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "DDR5ODTTIMING: %u\nDBI: %u\nDLLDCC: %u\nDLLBWSEL: %u\n",                 TrainingSteps2->DDR5ODTTIMING,  TrainingSteps2->DBI,            TrainingSteps2->DLLDCC,         TrainingSteps2->DLLBWSEL);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RDVREFDC: %u\nRDTCIDLE: %u\nRMTBIT: %u\nDQDQSSWZ: %u\n",                 TrainingSteps2->RDVREFDC,       TrainingSteps2->RDTCIDLE,       TrainingSteps2->RMTBIT,         TrainingSteps2->DQDQSSWZ);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "REFPI: %u\nDCCLP5READDCA: %u\nVCCCLKFF: %u\nFUNCDCCDQS: %u\n",           TrainingSteps2->REFPI,          TrainingSteps2->DCCLP5READDCA,  TrainingSteps2->VCCCLKFF,       TrainingSteps2->FUNCDCCDQS);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "FUNCDCCCLK: %u\nFUNCDCCWCK: %u\nFUNCDCCDQ: %u\nDATAPILIN: %u\n",         TrainingSteps2->FUNCDCCCLK,     TrainingSteps2->FUNCDCCWCK,     TrainingSteps2->FUNCDCCDQ,      TrainingSteps2->DATAPILIN);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "DDR5XTALK: %u\nDCCLP5WCKDCA: %u\nRXUNMATCHEDCAL: %u\nWRTDIMMDFE: %u\n",  TrainingSteps2->DDR5XTALK,      TrainingSteps2->DCCLP5WCKDCA,   TrainingSteps2->RXUNMATCHEDCAL, TrainingSteps2->WRTDIMMDFE);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RMTLVR: %u\n DCCDDR5READDCA: %u\n",                                      TrainingSteps2->RMTLVR,         TrainingSteps2->DCCDDR5READDCA /* Reserved2Bit30 */            /* SimicsReservedBit */);

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s3:\n", "TrainingEnables");
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RXDQSDCC: %u\nDIMMNTODT: %u\nTXDQSDCC: %u\nRXVREFPERBIT: %u\n",              TrainingSteps3->RXDQSDCC,       TrainingSteps3->DIMMNTODT,   TrainingSteps3->TXDQSDCC,        TrainingSteps3->RXVREFPERBIT);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "PPR: %u\nLVRAUTOTRIM: %u\nOPTIMIZECOMP: %u\n",                               TrainingSteps3->PPR,            TrainingSteps3->LVRAUTOTRIM, /* Reserved3Bit6 */              TrainingSteps3->OPTIMIZECOMP);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "WRTRETRAIN: %u\nJEDECRESET: %u\n",                                           TrainingSteps3->WRTRETRAIN,     /* Reserved3Bit9 */          /* Reserved3Bit10 */             TrainingSteps3->JEDECRESET);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "ROUNDTRIPMATCH: %u\nTLINECLKCAL: %u\nDCCPISERIALCAL: %u\nPHASECLKCAL: %u\n", TrainingSteps3->ROUNDTRIPMATCH, TrainingSteps3->TLINECLKCAL,  TrainingSteps3->DCCPISERIALCAL, TrainingSteps3->PHASECLKCAL);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "WCKPADDCCCAL: %u\nRDCTLET: %u\nRDDQODTT: %u\nEMPHASIS: %u\n",                TrainingSteps3->WCKPADDCCCAL,   TrainingSteps3->RDCTLET,      TrainingSteps3->RDDQODTT,       TrainingSteps3->EMPHASIS);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "DIMMRXOFFSET: %u\nVIEWPINCAL: %u\nQCLKDCC: %u\nWCKCLKPREDCC: %u\n",          TrainingSteps3->DIMMRXOFFSET,   TrainingSteps3->VIEWPINCAL,   TrainingSteps3->QCLKDCC,        TrainingSteps3->WCKCLKPREDCC);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "DQSPADDCC: %u\nQCLKPHALIGN: %u\nRXDQSVOCC: %u\nISENSERMT: %u\n",             TrainingSteps3->DQSPADDCC,      TrainingSteps3->QCLKPHALIGN,  TrainingSteps3->RXDQSVOCC,      TrainingSteps3->ISENSERMT);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "WCKCLKRF: %u\n",                                                             TrainingSteps3->WCKCLKRF        /* Reserved3Bit29 */          /* Reserved3Bit30 */            /* Reserved3Bit31 */);

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "MCREGOFFSET: %u\n", ExtInputs->MCREGOFFSET);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "DFETap1StepSize: %u\nDFETap2StepSize: %u\n", ExtInputs->DFETap1StepSize, ExtInputs->DFETap2StepSize);

  for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Dimm%d ODT Values:\n", Dimm);
    for (Index = 0; Index < MAX_DIMMODT_ENTRIES; Index++) {
      if (ExtInputs->DimmOdt[Dimm][Index] <= MAX_DIMMODT_VALUE) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s: %u\n", DimmOdtStr[Index], ExtInputs->DimmOdt[Dimm][Index]);
      } else {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s: Disabled\n", DimmOdtStr[Index]);
      }
    }
  }

  // SAGV Inputs
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "SAGV Inputs\nPoints\tFreq\tGears\n");
  for (Index = 0; Index < MAX_SAGV_POINTS; Index++) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%u\t%u\t%u\n", Index, ExtInputs->SaGvFreq[Index], ExtInputs->SaGvGear[Index]);
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n%s", PrintBorder);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "*****      MRC TIMING DATA      *****\n");
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s", PrintBorder);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    ControllerIn = &Inputs->Controller[Controller];
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Controller[%u] ChannelCount: %Xh\n", Controller, ControllerIn->ChannelCount);
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      ChannelIn = &ControllerIn->Channel[Channel];
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Channel[%u].Status: %Xh\n", Channel, ChannelIn->Status);
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Channel[%u].DimmCount: %Xh\n", Channel, ChannelIn->DimmCount);

      for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
        DimmIn = &ChannelIn->Dimm[Dimm];
        Timing = &DimmIn->Timing;
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u Status: %Xh\n", CcdString, Controller, Channel, Dimm, DimmIn->Status);
        if (ExtInputs->MemoryProfile == CUSTOM_PROFILE1) {
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u tCK    : %u\n", CcdString, Controller, Channel, Dimm, Timing->tCK);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u NMode  : %u\n", CcdString, Controller, Channel, Dimm, Timing->NMode);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u tCL    : %u\n", CcdString, Controller, Channel, Dimm, Timing->tCL);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u tCWL   : %u\n", CcdString, Controller, Channel, Dimm, Timing->tCWL);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u tFAW   : %u\n", CcdString, Controller, Channel, Dimm, Timing->tFAW);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u tRAS   : %u\n", CcdString, Controller, Channel, Dimm, Timing->tRAS);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u tRCDtRP: %u\n", CcdString, Controller, Channel, Dimm, Timing->tRCDtRP);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u tREFI  : %u\n", CcdString, Controller, Channel, Dimm, Timing->tREFI);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u tRFC   : %u\n", CcdString, Controller, Channel, Dimm, Timing->tRFC);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u tRFCpb : %u\n", CcdString, Controller, Channel, Dimm, Timing->tRFCpb);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u tRFC2  : %u\n", CcdString, Controller, Channel, Dimm, Timing->tRFC2);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u tRFC4  : %u\n", CcdString, Controller, Channel, Dimm, Timing->tRFC4);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u tRRD   : %u\n", CcdString, Controller, Channel, Dimm, Timing->tRRD);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u tRRD_L : %u\n", CcdString, Controller, Channel, Dimm, Timing->tRRD_L);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u tRRD_S : %u\n", CcdString, Controller, Channel, Dimm, Timing->tRRD_S);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u tRTP   : %u\n", CcdString, Controller, Channel, Dimm, Timing->tRTP);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u tWR    : %u\n", CcdString, Controller, Channel, Dimm, Timing->tWR);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u tWTR   : %u\n", CcdString, Controller, Channel, Dimm, Timing->tWTR);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u tWTR_L : %u\n", CcdString, Controller, Channel, Dimm, Timing->tWTR_L);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u tWTR_S : %u\n", CcdString, Controller, Channel, Dimm, Timing->tWTR_S);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u tCCD_L : %u\n", CcdString, Controller, Channel, Dimm, Timing->tCCD_L);
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u tCCD_L_WR : %u\n", CcdString, Controller, Channel, Dimm, Timing->tCCD_L_WR);
        }
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s %u/%u/%u SpdAddress: %Xh\n", CcdString, Controller, Channel, Dimm, DimmIn->SpdAddress);
        Buffer = (UINT8 *) &DimmIn->Spd.Data;
        if (Buffer[2] == 0) {
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " empty\n");
          continue;
        }
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "SPD:           00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
        // Dump SPD data only up to manufacturing info, minus Manufacturer's Specific Data
        if (Outputs->IsDdr5) {
          MaxSpd = OFFSET_OF (MrcSpdDdr5, EndUser) - (sizeof (SPD5_MANUFACTURING_DATA) - OFFSET_OF (SPD5_MANUFACTURING_DATA, ManufactureSpecificData));
        } else if (Outputs->IsLP5Camm2) {
          MaxSpd = OFFSET_OF (MrcSpdJedecSpecLpddr, EndUser) - (sizeof (SPD_LPDDR_JEDEC_SPEC_MANUFACTURING_INFO) - OFFSET_OF (SPD_LPDDR_JEDEC_SPEC_MANUFACTURING_INFO, ManufactureSpecificData));
        } else { // LP5 memory down
          MaxSpd = OFFSET_OF (MrcSpdLpddr, EndUser) - (sizeof (SPD_LPDDR_MANUFACTURING_DATA) - OFFSET_OF (SPD_LPDDR_MANUFACTURING_DATA, ManufactureSpecificData));
        }
        for (Line = 0; Line < DIVIDECEIL (MaxSpd, 16); Line++) {
          Address = Line * 16;
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %4Xh(%5u): ", Address, Address);
          p = HexDump;
          for (Offset = 0; Offset < 16; Offset++) {
            p += MrcSPrintf (MrcData, p, sizeof (HexDump) - (p - HexDump), "%02X ", Buffer[Address + Offset]) - 1;
          }
          for (Offset = 0; Offset < 16; Offset++) {
            Data8 = Buffer[Address + Offset];
            *p++ = MRC_ISPRINT (Data8) ? Data8 : '.';
          }
          *p = '\0';
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s\n", HexDump);
        }
      }
    }
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s", PrintBorder);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "*****    THERMAL OVERWRITE    *******\n");
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s", PrintBorder);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s.EnablePwrDn     : %Xh\n",   ThermEnString, ExtInputs->EnablePwrDn);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s.EnablePwrDnLpddr: %Xh\n",   ThermEnString, ExtInputs->EnablePwrDnLpddr);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s.McRefreshRate   : %Xh\n",   ThermEnString, ExtInputs->McRefreshRate);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "SrefCfgEna      : %Xh\n",      ExtInputs->SrefCfgEna);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "SrefCfgIdleTmr  : %Xh\n",      ExtInputs->SrefCfgIdleTmr);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "ThrtCkeMinDefeat: %Xh\n",      ExtInputs->ThrtCkeMinDefeat);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "ThrtCkeMinTmr   : %Xh\n",      ExtInputs->ThrtCkeMinTmr);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "ThrtCkeMinDefeatLpddr: %Xh\n", ExtInputs->ThrtCkeMinDefeatLpddr);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "ThrtCkeMinTmrLpddr   : %Xh\n", ExtInputs->ThrtCkeMinTmrLpddr);

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s", PrintBorder);

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RcompResistor: %u\n",  ExtInputs->RcompResistor);
  for (Index = 0; Index < MAX_RCOMP_TARGETS; Index++) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "RcompTarget[%s]: %u\n", GlobalCompOffsetStr[Index], ExtInputs->RcompTarget[Index]);
  }

  return mrcSuccess;
}

#endif


/**
  Sets DdrIoIpVersion (Version, Derivative, Segment, Stepping) in MrcData.

  @param[out] MrcData     - The Mrc Host data structure

  @retval mrcSuccess if the IP Segment and IP Version is valid.  Otherwise mrcFail
**/
MrcStatus
MrcSetCpuInformation (
  OUT MrcParameters  *MrcData
  )
{
  const MRC_FUNCTION  *MrcCall;
  MrcInput            *Inputs;
  MrcDebug            *Debug;
  MrcStatus           Status;
  UINT64              Revision;

  Inputs  = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;
  Debug   = &MrcData->Outputs.Debug;

  Status = MrcSetupDdrIoIpInfo(MrcData);

  Revision = MrcCall->MrcReadMsr64 (MRC_MSR_IA32_BIOS_SIGN_ID);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Microcode Update: 0x%08X\n", (UINT32) (MrcCall->MrcRightShift64 (Revision, 32)));

  return Status;
}

/**
  Check the Blue MRC vs Green binary version

  @param[in] MrcData - MRC Host structure.
**/
void
MrcVersionCheck (
  IN MrcParameters     *MrcData
  )
{
  // Headless does not need to check for version mismatches, LocalStub will always rebuild the image, and CTE does not follow this flow.
  const MrcInput      *Inputs;
  MrcOutput           *Outputs;
  MrcDebug            *Debug;
  MrcVersion          *BlueVersion;
  MrcVersion          *GreenVersion;
  MRC_PAYLOAD_HEADER  *Header;
  MRC_EXT_INPUTS_TYPE *ExtInputs;

  Inputs  = &MrcData->Inputs;
  ExtInputs = Inputs->ExtInputs.Ptr;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  if (Inputs->UcPayloadAddress == 0) {
    MRC_DEBUG_ASSERT (FALSE, Debug, "Green MRC payload not found!\n");
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "UcPayloadAddress: 0x%08X\n", (UINT32) Inputs->UcPayloadAddress);

  // skip version check for Simics to bypass CI test-verified runs
  if (ExtInputs->SimicsFlag) {
    return;
  }

  Header = (MRC_PAYLOAD_HEADER *) (UINTN) (Inputs->UcPayloadAddress);
  BlueVersion  = &Outputs->Version;
  GreenVersion = &Header->Version;

  if ((GreenVersion->Major != BlueVersion->Major) ||
      (GreenVersion->Minor != BlueVersion->Minor) ||
      (GreenVersion->Rev   != BlueVersion->Rev)   ||
      (GreenVersion->Build != BlueVersion->Build)) {
    if ((GreenVersion->Major == 0xFF) &&
        (GreenVersion->Minor == 0xFF) &&
        (GreenVersion->Rev   == 0xFF) &&
        (GreenVersion->Build == 0xFF)) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Bypassing version mismatch\n");
    } else {
      MRC_DEBUG_ASSERT (FALSE, Debug, "Green MRC version %u.%u.%u.%u does not match Blue MRC version %u.%u.%u.%u!\n",
        GreenVersion->Major, GreenVersion->Minor, GreenVersion->Rev, GreenVersion->Build,
        BlueVersion->Major, BlueVersion->Minor, BlueVersion->Rev, BlueVersion->Build);
    }
  }
}

/**
  Print the MRC version to the MRC output device.

  @param[in] MrcData - MRC Host structure.
  @param[in] Version - The MRC version.

  @retval Nothing.
**/
void
MrcVersionPrint (
  IN MrcParameters* MrcData,
  IN const MrcVersion* Version
  )
{
#ifdef MRC_DEBUG_PRINT
  MrcDebug* Debug;

  Debug = &MrcData->Outputs.Debug;
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "**********************************************************************\n");
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "** Copyright (c) 2024 Intel Corporation. All rights reserved. **\n");
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "** Memory detection and initialization code.                        **\n");
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "** Blue Major version number is:   %2u                               **\n", Version->Major);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "** Blue Minor version number is:   %2u                               **\n", Version->Minor);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "** Blue Rev version number is:     %2u                               **\n", Version->Rev);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "** Blue Build number is:           %2u                               **\n", Version->Build);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "**********************************************************************\n");
#endif
}

/**
  Check for symmetric memory controller population

  @param[in] MrcData - Pointer to the MRC Debug structure.

  @retval TRUE memory controller is symmetric
  @retval FALSE memory controller is not symmetric
**/
BOOLEAN
MrcIsIbeccSymmetric (
  IN MrcParameters *MrcData
  )
{
  const MRC_FUNCTION *MrcCall;
  MrcInput           *Inputs;
  MrcOutput          *Outputs;
  MrcDebug           *Debug;
  UINT32             Controller;
  INT64              Ch0Size;
  INT64              Ch1Size;
  UINT32             ControllerSize[MAX_CONTROLLER];
  BOOLEAN            ControllerSymmetrical;

  Inputs  = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MrcCall->MrcSetMem ((UINT8 *) ControllerSize, sizeof (ControllerSize), 0);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (MrcControllerExist (MrcData, Controller)) {
      MrcGetSetMc (MrcData, Controller, GsmMccCh0Size, ReadUncached, &Ch0Size);
      MrcGetSetMc (MrcData, Controller, GsmMccCh1Size, ReadUncached, &Ch1Size);
      ControllerSize[Controller] = (UINT8) Ch0Size + (UINT8) Ch1Size;
    }
  }

  // Symmetric only if all of the below is true:
  // - MC/Slice sizes are equal, or only one exists:
  //   MC0 capacity == MC1 capacity ||
  //   MC0 capacity == 0 ||
  //   MC1 capacity == 0
  ControllerSymmetrical = ((ControllerSize[0] == ControllerSize[1]) || (ControllerSize[0] == 0) || (ControllerSize[1] == 0));
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "IBECC Symmetrical: %s, MC0: %u%sGB, MC1: %u%sGB\n",
    ControllerSymmetrical ? "yes" : "no",
    ControllerSize[0] >> 1, ControllerSize[0] % 2 ? ".5" : ".0",
    ControllerSize[1] >> 1, ControllerSize[1] % 2 ? ".5" : ".0"
    );

  return ControllerSymmetrical;
}
