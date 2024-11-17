/** @file
  This file declares the PMA interface

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

#include "MrcCommon.h"
#include "MrcHalApi.h"
#include "MrcGeneral.h"
#include "MrcPmaApi.h"

/**
  This function kicks off the Pma RunBusy and waits for response and outputs response for failures or success.

  @param[in, out] MrcData - All the MRC global data.

  @retval mrcSuccess PMA RunBusy response is successful
  @retval mrcFrequencyError PMA RunBusy response was not successful
**/
static MrcStatus
MrcPmaRunBusy (
  IN OUT MrcParameters* const MrcData
  )
{
  MrcStatus  Status;
  MrcDebug* Debug;
  INT64      RunBusy;
  INT64      PmaErrorCode;
  const MRC_FUNCTION* MrcCall;
  UINT64              Timeout;

#ifdef MRC_DEBUG_PRINT
  char* Str;
#endif

  Debug = &MrcData->Outputs.Debug;

  Status = mrcSuccess;

  RunBusy = 1;
  MrcGetSetNoScope (MrcData, GsmPmaRunBusy, WriteCached, &RunBusy);


  MrcCall = MrcData->Inputs.Call.Func;
  // Poll MEMSS_PMA_CR_BIOS_REQ.RUN_BUSY = 0, Timeout 10 seconds
  Timeout = MrcCall->MrcGetCpuTime () + MRC_WAIT_TIMEOUT; // 10 seconds timeout
  do {
    MrcGetSetNoScope (MrcData, GsmPmaRunBusy, ReadCached | PrintValue, &RunBusy);
  } while (RunBusy && (MrcCall->MrcGetCpuTime() < Timeout));

  // Check for Timeout
  if (RunBusy) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "- TIMEOUT.\n");
    Status = mrcTimeout;
  }

  MrcGetSetNoScope (MrcData, GsmPmaErrorCode, ReadCached | PrintValue, &PmaErrorCode);

  if (PmaErrorCode != PMA_ERROR_DDR_CHECKS_PASSED) {
#ifdef MRC_DEBUG_PRINT
    if (PmaErrorCode == PMA_ERROR_NO_UPDATE) {
      Str = "No update to error status\n";
    } else if (PmaErrorCode == PMA_ERROR_DDR_CHECKS_ILLEGAL_DDR_TYPE) {
      Str = "DDR Type is not one of the fuse enabled technologies\n";
    } else if (PmaErrorCode == PMA_ERROR_DDR_CHECKS_FAILED_LT_CONDITIONS) {
      Str = "LT Conditions Not Met\n";
    } else if (PmaErrorCode == PMA_ERROR_DDR_CHECKS_QCLK_FREQ_BELOW_MIN) {
      Str = "Frequency Below Minimum\n";
    } else if (PmaErrorCode == PMA_ERROR_DDR_CHECKS_QCLK_FREQ_ABOVE_OC_LIMIT) {
      Str = "Frequency Above OC Limit\n";
    } else if (PmaErrorCode == PMA_ERROR_DDR_CHECKS_QCLK_FREQ_ABOVE_MAX_F_LIMIT) {
      Str = "Frequency Above Maximum allowed QCLK\n";
    } else if (PmaErrorCode == PMA_ERROR_DDR_CHECKS_DDR_FREQ_ABOVE_MAX_F_TECH_LIMIT) {
      Str = "The frequency is above the maximum allowed Qclk for the requested DDR Type\n";
    } else if (PmaErrorCode == PMA_ERROR_DDR_CHECKS_DDRPHY_ERROR) {
      Str = "Error in DDR Phy\n";
    } else {
      Str = "Undefined PCode Error Status\n";
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s %s", gErrString, Str);
#endif
  Status = mrcFrequencyError;
  }

  return Status;
}

/**
  This function locks both PHY and MC PLL by requesting memSS PMA

  @param[in] MrcData             - Include all MRC global data.
  @param[in] IsVoltageChangeOnly - FALSE for normal PM_MESSAGE_0. TRUE for PM_MESSAGE_0_VOLTAGE_ONLY, sent later on to change voltage after Green training is done.
  @param[in] DebugPrint          - Enable/disable debug printing

  @retval mrcSuccess PLL is locked successfully
  @retval other an unknown failure occurred
**/
MrcStatus
MrcPllLock (
  IN MrcParameters* const MrcData,
  IN BOOLEAN              IsVoltageChangeOnly,
  IN BOOLEAN              DebugPrint
  )
{
  MrcDebug          *Debug;
  MrcDebugMsgLevel  DebugLevel;
  MrcInput          *Inputs;
  MrcOutput         *Outputs;
  MrcSaGvPoint      SaGvPoint;
  BOOLEAN           GearMode;
  MrcStatus         Status;
  UINT32            NumPopChannel;
  UINT8             ChannelWidth;
  INT64             QclkRatioData;
  INT64             GearTypeData;
  INT64             QclkRatioReq;
  INT64             GearTypeReq;
  UINT32            MaxBwGbps;
  INT64             GetSetVal;
  UINT32            GsmMode;

  Inputs = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;
  DebugLevel = DebugPrint ? MSG_LEVEL_NOTE : MSG_LEVEL_NONE;
  Status = mrcSuccess;
  GearMode = Outputs->GearMode;
  NumPopChannel = Outputs->NumPopChannel;
  ChannelWidth = Outputs->IsDdr5 ? 4 : 2;
  SaGvPoint = MrcGetSaGvPoint (MrcData);

  // Set QClk_Ratio / Gear / BW_MPBS
  QclkRatioReq = (GearMode) ? Outputs->Ratio / 4 : Outputs->Ratio / 2;
  MrcGetSetNoScope (MrcData, GsmPmaQclkRatioReq, WriteToCache, &QclkRatioReq);
  GearTypeReq = (GearMode) ? 1 : 0; // 0: Gear2, 1: Gear4
  MrcGetSetNoScope (MrcData, GsmPmaGearTypeReq, WriteToCache, &GearTypeReq);

  // Set VDDQ Voltage in 50mV increments
  GetSetVal = Outputs->VccddqVoltage / 50;
  MrcGetSetNoScope (MrcData, GsmPmaDvfsVddq, WriteToCache, &GetSetVal);
  MRC_DEBUG_MSG (Debug, DebugLevel, "Vddq: %u, VddqChangeOnly: %u\n", (UINT32) GetSetVal, IsVoltageChangeOnly);
  GetSetVal = IsVoltageChangeOnly;
  MrcGetSetNoScope (MrcData, GsmPmaVddqChangeOnly, WriteToCache, &GetSetVal);

  MaxBwGbps = NumPopChannel * Outputs->Frequency * ChannelWidth;
  MaxBwGbps = UDIVIDEROUND (MaxBwGbps, 1024);                    //This register is in GB/s
  GetSetVal = MaxBwGbps;
  MrcGetSetNoScope (MrcData, GsmPmaMaxBwGbps, WriteToCache, &GetSetVal);

  GetSetVal = SaGvPoint;
  MrcGetSetNoScope (MrcData, GsmPmaQclkWpIdx, WriteToCache, &GetSetVal);
  MrcFlushRegisterCachedData (MrcData);

  MRC_DEBUG_MSG (
    Debug,
    DebugLevel,
    "Attempting QclkRatio: %d, GearType: %d, MaxBW: %u, WorkPoint: %d - Pll busy wait \n",
    Outputs->Ratio,
    Inputs->ExtInputs.Ptr->GearRatio,
    MaxBwGbps,
    SaGvPoint
  );

  Status |= MrcPmaRunBusy (MrcData);
  if (Status == mrcSuccess) {
    MRC_DEBUG_MSG (Debug, DebugLevel, "- done\n");
  } else {
    return Status;
  }

  GsmMode = ReadCached | (DebugPrint ? PrintValue : 0);
  MrcGetSetNoScope (MrcData, GsmPmaQclkRatioData, GsmMode, &QclkRatioData);
  MrcGetSetNoScope (MrcData, GsmPmaGearTypeData, GsmMode, &GearTypeData);

  if ((QclkRatioData != QclkRatioReq) || (GearTypeData != GearTypeReq)) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "- PMA DATA does not match requested values - FAILED!\n");
    return mrcFrequencyError;
  }
  return Status;
}
