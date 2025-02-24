/** @file
  This file contains code related to initializing and configuring the DDRIO.

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

**/


#include "MrcCommon.h"
#include "MrcDdrIoUtils.h"  // for prototypes
#include "MrcDebugPrint.h"  // for MRC_DEBUG_MSG()
#include "MrcHalApi.h"  // for MrcGetSetChStrb()
#include "MrcDdr5.h"  // for MrcSetDdr5Mr48()
#include "MrcDdrIoApi.h"  // for MrcGetDataOffset()
#include "MrcLpddr5.h" // for MrcGetWckPreRdTotalLpddr5()
#include "MrcDdrIoRefPi.h"

UINT8 AuxClkRef100[] = { 33,  33,  27,  28,  27,  30,  33,  28,  33,  32,  33,  32,  32,  33,  36,  36, //  0 .. 15
                         39,  33,  42,  39,  42,  44,  44,  42,  53,  44,  45,  42,  51,  48,  52,  40, // 16 .. 31
                         54,  52,  52,  57,  54,  45,  56,  52,  57,  63,  64,  60,  66,  66,  68,  69, // 32 .. 47
                         69,  66,  66,  68,  69,  69,  76,  63,  78,  78,  30,  75,  83,  76,  78,  78, // 48 .. 63
                         78,  81,  87,  88,  90,  84,  84,  92,  87,  87,  88,  92,  88,  93,  92,  84, // 64 .. 79
                         93,  92,  63,  99, 105, 100, 100, 102, 108, 104, 116,  78, 114, 120, 114, 108, // 80 .. 95
                        116, 111, 111, 112, 126, 114, 128, 117, 104, 144, 144, 120, 135,  90, 120, 124, // 96 .. 111
                        123, 141, 123, 126, 136, 129, 138, 129, 129, 132, 132, 135, 132, 136, 136, 138, // 112 .. 127
                        136, 140, 138, 141, 141, 144, 141, 144, 144, 108, 100, 132, 135, 111, 111, 135, // 128 .. 143
                        104, 114, 140, 138, 114, 140, 144, 117, 141, 120, 132, 120, 136, 123, 120, 123, // 144 .. 159
                        123, 124,  68, 126, 126, 128, 126, 129, 129};


const INT8 DdrioChDelta[MAX_SYS_CHANNEL] = {0, 0, -1, -1, -2, -2, -1, -1};
const INT8 DdrioChDeltaCccIL[MAX_SYS_CHANNEL] = {0, 0, 0, 0, -1, -1, -1, -1};

static const UINT8 HwChBitMasks[][MAX_MRC_DDR_TYPE] = {
    [ipDerivativeWcl] = {
      [MRC_DDR_TYPE_LPDDR5] = 0x0F,
      [MRC_DDR_TYPE_DDR5]   = 0x03,
    },
    [ipDerivativePtl] = {
      [MRC_DDR_TYPE_LPDDR5] = 0xFF,
      [MRC_DDR_TYPE_DDR5]   = 0x0F,
    },
  };

#define DQS_PARK_DIFF_LOW_WHEN_LPM0_NO_ODT_NO_DRVEN_TXANALOGEN   2


/**
  This function configures the Data Invert Nibble feature in the Phy based on the Phy ODT configuration


  @param[in]  MrcData - Pointer to MRC global data.

  @retval MrcStatus - mrcFail if the ODT mode doesn't have a mapping for Data Invert Nibble.
  @retval MrcStatus - mrcSuccess otherwise.
**/
MrcStatus
MrcDataInvertNibble (
  IN  MrcParameters *const MrcData
  )
{
  MrcInput    *Inputs;
  MrcOutput   *Outputs;
  INT64       GetSetVal;
  INT32       ByteOff;
  INT32       ByteSel;
  UINT32      Controller;
  UINT32      Channel;
  UINT32      Byte;
  UINT32      ByteMax;
  UINT32      SwizOffset;
  UINT32      DataCtrl5Offset;
  UINT8       DniDataMask;
  UINT8       DniMiscMask;
  UINT8       ByteRead;
  BOOLEAN     IsDdr5;
  BOOLEAN     IsDataPatternSupported;
  MrcStatus   Status;
  MRC_EXT_INPUTS_TYPE                         *ExtInputs;
  DATA_CR_DDRCRDATACONTROL5_STRUCT            DataControl5;
  DATA_CR_DDRCRWRRETRAINSWIZZLECONTROL_STRUCT WrRetrainSwizzleControl;
  DDRSCRAM_CR_DDRMISCCONTROL2_STRUCT          MiscControl2;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  ByteMax = (Inputs->IsDdrIoUlxUlt) ? 8 : 10;
  ExtInputs = Inputs->ExtInputs.Ptr;
  IsDdr5 = Outputs->IsDdr5;
  Status = mrcSuccess;

  IsDataPatternSupported = TRUE;
  switch (Outputs->OdtMode) {
    case MrcOdtModeVss:
      DniDataMask = DniMiscMask = 0;
      break;

    case MrcOdtModeVddq:
      DniDataMask = 3;
      DniMiscMask = 1;
      break;

    default:
      MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_ERROR, "Invalid ODT mode for DataInvertNibble: %u\n", Outputs->OdtMode);
      return mrcFail;
  }

  if (ExtInputs->DataInvertNibble == 0) {
    DniDataMask = DniMiscMask = 0;
    IsDataPatternSupported = FALSE;
  }

  // Update the value across the entire phy regardless of population.  Logic won't run on non-populated channels.
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
        GetSetVal = DniDataMask;
        MrcGetSetChStrb (MrcData, Controller, Channel, Byte, GsmIocDataInvertNibble, WriteToCache, &GetSetVal);
      }
    }
  }
  MrcFlushRegisterCachedData (MrcData);

  if (ExtInputs->ScramblerSupport) {
    MiscControl2.Data = MrcReadCR (MrcData, DDRSCRAM_CR_DDRMISCCONTROL2_REG);
    MiscControl2.Bits.DataInvertNibble = DniMiscMask;
    MrcWriteCR (MrcData, DDRSCRAM_CR_DDRMISCCONTROL2_REG, MiscControl2.Data);
  }

    // DDR5 needs to program MR48 to match DataInvertNibble programming
  if (IsDdr5) {
    Status = MrcSetDdr5Mr48 (MrcData, Outputs->OdtMode, IsDataPatternSupported);
    MRC_DEBUG_ASSERT (Status == mrcSuccess, &Outputs->Debug, "Ddr5 MR48 not properly programmed\n");
  }

  for (Channel = 0; Channel < 2; Channel++) {
    for (Byte = 0; Byte < ByteMax; Byte++) {
      SwizOffset = MrcGetDataOffset (MrcData, DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG, MRC_IGNORE_ARG, Channel, Byte);
      //0: ODT to Vss 1: ODT to Vddq
      //(OdtMode==0 or DDRMiscControl0.Write0En==0)? 0 : (OdtMode==1) ? 3 : (IamDDR5 ? 0x3*IamOddWord : "Error")
      WrRetrainSwizzleControl.Data = MrcReadCR (MrcData, SwizOffset);
      if (Outputs->IsLpddr) {
        ByteRead = (UINT8) (((Byte % 2) << 1) | WrRetrainSwizzleControl.Bits.DataRetrain_ByteSel);
      } else {
        ByteRead = 0;
      }
      switch (ByteRead) {
        case 1:
          ByteOff = 1;
          break;

        case 3:
          ByteOff = -1;
          break;

        default:
        case 0:
        case 2:
          ByteOff = 0;
          break;
      }
      ByteSel = Byte + ByteOff;

      DataCtrl5Offset = MrcGetDataOffset (MrcData, DATA0CH0_CR_DDRCRDATACONTROL5_REG, MRC_IGNORE_ARG, Channel, ByteSel);
      DataControl5.Data = MrcReadCR (MrcData, DataCtrl5Offset);
      WrRetrainSwizzleControl.Bits.DataInvertNibble = DataControl5.Bits.DataInvertNibble;
      MrcWriteCR (MrcData, SwizOffset, WrRetrainSwizzleControl.Data);
    }
  }

  return mrcSuccess;
}

/**
  This function sets DDRIO IP Version, Derivative, Segment, Stepping based on DDRPHY_MISC_SAUG_CR_IPVERSION_REG.

  @param[in]  MrcData     - Include all MRC global data.

  @retval Nothing.
**/
MrcStatus
MrcSetupDdrIoIpInfo (
  OUT MrcParameters *const MrcData
  )
{
  MrcInput             *Inputs;
  MrcDebug             *Debug;
  MrcStatus            Status;
  MrcDdrIoIpVersion    *IpVersion;
  DDRPHY_MISC_SAUG_CR_IPVERSION_STRUCT   DdrMiscIpVersion;
  const char           *StepStr;

  Inputs          = &MrcData->Inputs;
  Debug           = &MrcData->Outputs.Debug;
  IpVersion       = &Inputs->DdrIoIpVersion;
  Status          = mrcSuccess;
  StepStr         = NULL;

  DdrMiscIpVersion.Data = MrcReadCR (MrcData, DDRPHY_MISC_SAUG_CR_IPVERSION_REG);
  IpVersion->Bits.Version    = (UINT8)DdrMiscIpVersion.Bits.Version;
  IpVersion->Bits.Derivative = (UINT8)DdrMiscIpVersion.Bits.Derivative;
  IpVersion->Bits.Segment    = (UINT8)DdrMiscIpVersion.Bits.Segment;
  IpVersion->Bits.Stepping   = (UINT8)DdrMiscIpVersion.Bits.Subversion;

  if (IpVersion->Bits.Stepping > ipStepLast) {
    IpVersion->Bits.Stepping = ipStepLast;
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "\n%s DDRIO IP: %s = %d\n", gWarnString, gMrcIpSteppingStr, ipStepLast);
  }

  if (IpVersion->Bits.Version != ipVerDdrIoLnlPtl) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "\n%s DDRIO IP: %s\n", gWarnString, gMrcIpVersionStr);
      Status  = mrcFail;
  }

  if (Inputs->ExtInputs.Ptr->SimicsFlag && IpVersion->Bits.Derivative != ipDerivativeWcl) {
    StepStr = "A0";
    Inputs->IsDdrIoMbA0 = TRUE;
    IpVersion->Bits.Derivative = ipDerivativePtl;
  } else {
    switch (IpVersion->Bits.Segment) {
      case IpSegmentMobile:
        Inputs->IsDdrIoUlxUlt = TRUE;
        switch (IpVersion->Bits.Stepping) {
          case ipStepB0:
            StepStr = "B0";
            Inputs->IsDdrIoMbB0 = TRUE;
            break;

          default:
          case ipStepA0:
            StepStr = "A0";
            Inputs->IsDdrIoMbA0 = TRUE;
            break;
        }
        break;

      default:
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "\n%s DDRIO IP: %s\n", gWarnString, gMrcIpSegmentStr);
        Status  = mrcFail;
        break;
    }
  }

  if (IpVersion->Bits.Derivative == ipDerivativeWcl) {
    Inputs->IsDdrphyx64 = TRUE;
  }

  if (Status == mrcSuccess) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s\n", Inputs->IsDdrphyx64 ? "PantherLake" : "PantherLake" );
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_NOTE,
      "\nPHY IP Version: %d.%d.%d %s\n",
      IpVersion->Bits.Derivative,
      IpVersion->Bits.Segment,
      IpVersion->Bits.Version,
      (StepStr == NULL) ? "Unknown" : StepStr
    );
  }

  return Status;
}

/**
  This function programs the WorkPoint CR, including enforcing any fuse limits to avoid HW from NACKing PLLLock Requests.

  @param[in, out] MrcData - MRC global data.

  @retval MrcStatus - mrcSuccess if Workpoint CR is set correctly, otherwise mrcFail.
**/
MrcStatus
MrcSetWorkpointCR (
  IN OUT MrcParameters *const MrcData
  )
{
  return mrcSuccess;
}

/**
  This function sets workpoint data including Qclk ratio, Gear, and AuxClk ratio

  @param[in, out] MrcData - MRC global data.
**/
VOID
MrcSetWorkPointDataForPhy (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcOutput         *Outputs;
  INT64             GetSetVal;
  UINT32            AuxClk;
  MrcClockRatio     Ratio;
  MrcSaGvPoint      SaGvPoint;

  Outputs   = &MrcData->Outputs;
  SaGvPoint = Outputs->SaGvPoint;

  // Configure work point data
  Ratio = MrcFrequencyToRatio (MrcData, Outputs->Frequency);
  Ratio =  Outputs->GearMode ? (Ratio / 4) : (Ratio / 2);
  GetSetVal = (INT64) Ratio;
  MrcGetSetFreqIndex (MrcData, SaGvPoint, GsmWorkPointRatio, WriteToCache | PrintValue, &GetSetVal);
  GetSetVal = (INT64) SaGvPoint;
  MrcGetSetNoScope (MrcData, GsmMiscPmMessageWpSel, WriteNoCache | PrintValue, &GetSetVal);
  AuxClk = GetAuxClkRatio (MrcData, Ratio);
  GetSetVal = (INT64) AuxClk;
  MrcGetSetFreqIndex (MrcData, SaGvPoint, GsmWorkPointAuxClk, WriteToCache | PrintValue, &GetSetVal);
  GetSetVal = Outputs->GearMode ? 1 : 0;
  MrcGetSetFreqIndex (MrcData, SaGvPoint, GsmWorkPointGear4, WriteToCache | PrintValue, &GetSetVal);

  MrcFlushRegisterCachedData (MrcData);
}

/**
  This function calculates the AuxClkRatio based on QClkRatio.

  @param[in, out] MrcData - All the MRC global data.
  @param[in]      Ratio33 - QClkRatio

  @return Returns AuxClk ratio.
**/
UINT32
GetAuxClkRatio (
  IN MrcParameters *const MrcData,
  IN UINT32               Ratio33
  )
{
  MrcOutput      *Outputs;
  BOOLEAN        Gear4;
  UINT32         AuxClkRatio33;
  UINT32         QClkRatio;

  Outputs     = &MrcData->Outputs;
  Gear4       = Outputs->GearMode ? 1 : 0;

  QClkRatio = Ratio33 * (1 + Gear4);

  if (QClkRatio < 24) {
    QClkRatio = 24;
  }

  AuxClkRatio33  = AuxClkRef100[QClkRatio - 24];

  return AuxClkRatio33;
}

/**
  Programming of CCC_CR_DDRCRCCCCLKCONTROLS_BlockTrainRst

  @param[in] MrcData - The MRC global data.
  @param[in] BlockTrainReset - TRUE to BlockTrainReset for most training algos.  FALSE for specific training algos that need PiDivider sync.

**/
VOID
MrcBlockTrainResetToggle (
  IN MrcParameters *const MrcData,
  IN BOOLEAN              BlockTrainReset
)
{
  UINT8   Value;
  UINT8   Index;
  UINT32  Offset;
  DDRCCC_SHARED0_CR_DDRCRCCCPINCONTROLS_STRUCT  CccPinControls;

  Value = (BlockTrainReset) ? 1 : 0;

  for (Index = 0; Index < (MAX_SYS_CHANNEL / MRC_NUM_PAR_PER_SHARED); Index++) {
    if (!(MrcGetHwPartitionExists (MrcData, PartitionCccShared, Index, MRC_IGNORE_ARG))) {
      continue;
    }
    Offset = OFFSET_CALC_CH (DDRCCC_SHARED0_CR_DDRCRCCCPINCONTROLS_REG, DDRCCC_SHARED1_CR_DDRCRCCCPINCONTROLS_REG, Index);
    CccPinControls.Data = MrcReadCR (MrcData, Offset);
    if (CccPinControls.Bits.BlockTrainRst == Value) {
      continue;
    }
    CccPinControls.Bits.BlockTrainRst = Value;
    MrcWriteCR (MrcData, Offset, CccPinControls.Data);
  }
}


/**
  This function gets the endqsodtparkmode value.

  @retval The value for endqsodtparkmode
**/
UINT8
MrcGetEnDqsOdtParkModeValue (
  )
{
  return DQS_PARK_DIFF_LOW_WHEN_LPM0_NO_ODT_NO_DRVEN_TXANALOGEN;
}


/**
  This function programs the current post code to PHYPMSTATUS1 register field.

  @param[in]  MrcData  - Include all MRC global data.
  @param[in]  PostCode - MRC post code to be programmed

**/
VOID
MrcProgramPostCode (
  IN MrcParameters* const MrcData,
  IN INT64          PostCode
  )
{
}


/**
  This function is to set up the margin parameters for RdV.

  @param[in,out] MrcData      - Include all MRC global data.
  @param[in]     McStart      - MC number starts
  @param[in]     McEnd        - MC number ends.
  @param[in]     ChannelStart - Channel number starts.
  @param[in]     ChannelEnd   - Channel number ends
  @param[in]     ByteStart    - Byte number starts.
  @param[in]     ByteEnd      - Byte number ends.
  @param[in]     GsmMode      - Get Set mode.
  @param[in]     GetSetVal    - The pointer to the get set value
  @param[in,out] UpdateGrp    - Update Group or not.
  @param[in,out] Group        - The group parameter which will be used during margin

**/
VOID
SetupMarginRdV (
  IN OUT MrcParameters *const MrcData,
  IN           UINT32          McStart,
  IN           UINT32          McEnd,
  IN           UINT32          ChannelStart,
  IN           UINT32          ChannelEnd,
  IN           UINT32          ByteStart,
  IN           UINT32          ByteEnd,
  IN           UINT8           GsmMode,
  IN           INT64           *GetSetVal,
  IN OUT       BOOLEAN         *UpdateGrp,
  IN OUT       GSM_GT          *Group
  )
{
  *UpdateGrp = TRUE;
  *Group = RxVrefOffset;
}

/*
  Update the requested Param offset with the given Value,
  using direct multicast CR access for speed.

  @param[in]  MrcData - Global MRC data structure
  @param[in]  Param   - Margin param
  @param[in]  Value   - The value to program
*/
VOID
MrcWriteDirectMulticast (
  IN MrcParameters *const MrcData,
  IN UINT8                Param,
  IN INT32                Value
  )
{
  DATA0CH0_CR_DDRCRADC_STRUCT DdrCrAdc;

  switch (Param) {
    case RdV:
      DdrCrAdc.Data = DATA0CH0_CR_DDRCRADC_DEF;
      DdrCrAdc.Bits.VrefOffset = Value;
      MrcWriteCrMulticast (MrcData, DATA_CR_DDRCRADC_REG, DdrCrAdc.Data);
      break;

    default:
      break;
  }
}

/*
  IsDataPopulated

  @param[in]  MrcData       - Global MRC data structure.
  @param[in]  PartInst       - Partition instance

  Returns True if a data partition instance is populated, otherwise False
*/
BOOLEAN
IsDataPopulated (
  IN MrcParameters* const MrcData,
  IN UINT8                PartInst
  )
{
  return TRUE;
}

/*
  IsCCCPopulated

  @param[in]  MrcData       - Global MRC data structure.
  @param[in]  Instance       - Partition instance

  Returns True if a CCC partition instance is populated, otherwise False
*/
BOOLEAN
IsCCCPopulated (
  IN MrcParameters* const MrcData,
  IN UINT8                Instance
  )
{
  return TRUE;
}

/*
  IsPartPopulated

  @param[in]  MrcData       - Global MRC data structure.
  @param[in]  PartName       - Data/CCC/Comp.
  @param[in]  Instance       - Partition instance

  Returns True if partName and partInst is populated
*/
BOOLEAN
IsPartPopulated (
  IN MrcParameters* const MrcData,
  IN RAIL_PARTTION     PartName,
  IN UINT8             Instance
  )
{
  return TRUE;
}

/*
  Calculates the encoded target voltage and divider ratio to program into the LVR Voltage Sensor
  Returns both the Target and VinDivider voltages.

  @param[in]   MrcData       - Global MRC data structure.
  @param[in]   Rail          - Voltage Rail Type
  @param[in]   Target        - Target Voltage
  @param[out]  Vid           - Voltage
  @param[out]  FbDivider     - Divider Ratio
*/
VOID
CalcLvrTarget (
  IN  MrcParameters *const MrcData,
  IN  RAIL_TYPE            Rail,
  IN  UINT32               Target,
  OUT UINT32*              Vid, OPTIONAL
  OUT UINT32*              FbDivider
  )
{
}

/*
  Calculates the target voltage based on DataRate and Fuse Limits
  Returns the target voltage in mV

  @param[in]   MrcData       - Global MRC data structure.
  @param[in]   Type          - Vccclk or VccIo

  @retval TargetVoltage
*/
UINT32
CalcVFCurve (
  IN MrcParameters *const MrcData,
  VF_CURVE_TYPE           Type
  )
{

  return 0;
}

/*
  Write LVR Target

  @param[in]  MrcData       - Global MRC data structure.
  @param[in]  Rail          - Voltage Rail Type

  @retval mrcSuccess - Always returned as this step is non-blocking
*/
MrcStatus
WriteLvrTarget (
  IN MrcParameters* const MrcData,
  IN RAIL_TYPE            Rail
  )
{

  return mrcSuccess;
}

/*
  Setup LVR Registers

  @param[in]  MrcData       - Global MRC data structure.
*/
VOID
WriteAllLvrTarget (
  IN MrcParameters* const MrcData
  )
{
}

/**
  This function calculates the SA/ODT Delay and Duration
  @param[in]  MrcData       - Include all MRC global data.
  @param[in]  MinRcvEn      - Min Value of RcvEn in QClk across populated ranks
  @param[in]  GuardbandQ    - Guardband in QCLK
  @param[in]  RdPreambleLow - Read Preamble Static
  @param[in]  RdPreambleT   - Read Preamble Toggle
  @param[in]  RdPostamble   - Read Postamble
  @param[out] Returns SA/ODT Delay and Duration value
**/
VOID
MrcGetSaOdtValue (
  IN   MrcParameters *const MrcData,
  IN   UINT32        MinRcvEn,
  IN   UINT32        GuardbandQ,
  IN   UINT32        RdPreambleLow,
  IN   UINT32        RdPreambleT,
  IN   UINT32        RdPostamble,
  OUT  SaOdtTiming   *SaOdtValue
  )
{
  MrcOutput    *Outputs;
  BOOLEAN      IsDdr5;
  BOOLEAN      IsGear4;
  UINT32       SAWakeUp;
  UINT32       OdtWakeUp;
  UINT32       QClk;
  INT64        DataPath;
  INT64        TempVarInt;
  UINT32       DramDrift;
  UINT32       Guardband;
  UINT32       RdPreambleStatic;
  UINT32       RdPreambleToggle;
  UINT32       RcvEnPullInQclk;
  UINT32       OdtPullInQclk;
  UINT32       SenseAmpPullInQclk;

  Outputs = &MrcData->Outputs;


  IsDdr5       = Outputs->IsDdr5;
  IsGear4      = Outputs->GearMode ? 1 : 0;
  QClk         = Outputs->Qclkps;
  SAWakeUp     = 2000; // From UVopt POR equations
  OdtWakeUp    = 1000; // From UVopt POR equations

  // int tWCKDQO_drift_max = 0
  // int tDQSCK_drift_max = 0;
  // int DRAM_drift = ( tech == DDRPHY_LPDDR5 ) ? tWCKDQO_drift_max : tDQSCK_drift_max;
  DramDrift = 0;

  // int guardband = 2;
  Guardband = 2;

  DataPath = IsGear4 ? 5 : 8;

  // This mapping is needed to translate the MRC's definition of RdPreambleToggle/Static to RTL's definition
  RdPreambleToggle = IsDdr5 ? (2 * RdPreambleT) : RdPreambleT;
  RdPreambleStatic = IsDdr5 ? MIN ((2 * RdPreambleLow), 2) : MIN ((RdPreambleLow), 2);

  // odt_pull_in_qclk = odt_wakeuptime_qclk + (rdpreamble_static >> (gear == 4)) + DRAM_drift;
  OdtPullInQclk = UDIVIDEROUND (OdtWakeUp, QClk) + (RdPreambleStatic >> IsGear4) + DramDrift;

  // rcven_pull_in_qclk = int'(real'(rdpreamble_toggle + 1) / (gear == 4 ? 2 : 1));
  RcvEnPullInQclk = UDIVIDEROUND ((RdPreambleToggle + 1), (UINT32) (IsGear4 ? 2 : 1));

  // senseamp_pull_in_qclk = senseamp_wakeuptime_qclk + (rdpreamble_static >> (gear == 4)) + DRAM_drift;
  SenseAmpPullInQclk = UDIVIDEROUND (SAWakeUp, QClk) + (RdPreambleStatic >> IsGear4) + DramDrift;

  // dqsodtdelay_pre_sat         = rxrcvenpi_min + (gear == 4 ? 5:8) - odt_pull_in_qclk;
  // dqsodtdelay                 = SatMinMax(dqsodtdelay_pre_sat,0,20);
  TempVarInt = MinRcvEn + DataPath - OdtPullInQclk;
  SaOdtValue->DqsDelay = RANGE (TempVarInt, 0, 20);

  // dqodtdelay_pre_sat          = rxrcvenpi_min + (gear == 4 ? 5:8) - odt_pull_in_qclk + (rdpreamble_toggle >> (gear == 4));
  // dqodtdelay                  = SatMinMax(dqodtdelay_pre_sat,0,20);
  TempVarInt = MinRcvEn + DataPath - OdtPullInQclk + (RdPreambleToggle >> IsGear4);
  SaOdtValue->DqDelay = RANGE (TempVarInt, 0, 20);

  // senseampdelay_pre_sat       = rxrcvenpi_min + (gear == 4 ? 5:8) - senseamp_pull_in_qclk;
  // senseampdelay               = SatMinMax(senseampdelay_pre_sat,0,20);
  TempVarInt = MinRcvEn + DataPath - SenseAmpPullInQclk;
  SaOdtValue->SaDelay = RANGE (TempVarInt, 0, 20);

  // dqsodtduration_pre_sat      = 2 + odt_pull_in_qclk + DRAM_drift + guardband;
  // dqsodtduration              = SatMinMax(dqsodtduration_pre_sat,0,7);
  TempVarInt = 2 + OdtPullInQclk + DramDrift + Guardband;
  SaOdtValue->DqsDuration = MIN (TempVarInt, 7);

  // dqodtduration_pre_sat       = rcven_pull_in_qclk + 2 + odt_pull_in_qclk + DRAM_drift + guardband;
  // dqodtduration               = SatMinMax(dqodtduration_pre_sat,0,15);
  TempVarInt = RcvEnPullInQclk + 2 + OdtPullInQclk + DramDrift + Guardband;
  SaOdtValue->DqDuration = MIN (TempVarInt, 15);

  // senseampduration_pre_sat    = rcven_pull_in_qclk + 2 + senseamp_pull_in_qclk + DRAM_drift + guardband;
  // senseampduration            = SatMinMax(senseampduration_pre_sat,0,15);
  TempVarInt = RcvEnPullInQclk + 2 + SenseAmpPullInQclk + DramDrift + Guardband;
  SaOdtValue->SaDuration = MIN (TempVarInt, 15);
}

/**
  This function calculates QClkRatio.

  @param[in] MrcData  - Include all MRC global data.

  @return Returns QClkRatio.
**/
UINT32
MrcCalcRatio33 (
  IN MrcParameters* const MrcData
  )
{
  MrcOutput    *Outputs;
  BOOLEAN      IsGear4;
  MrcFrequency DdrFrequency;
  UINT32       QClkRatio33;

  Outputs         = &MrcData->Outputs;
  IsGear4         = Outputs->GearMode ? 1 : 0;
  DdrFrequency    = Outputs->Frequency;

  QClkRatio33     = DdrFrequency * 3;
  QClkRatio33     = UDIVIDEROUND (QClkRatio33, 100);
  QClkRatio33     /= (IsGear4 ? 4 : 2);

  return QClkRatio33;
}

/**
  This function resets VctlInit with value 0

  @param[in, out] MrcData - Pointer to MRC global data.
**/
VOID
ResetVctlInit (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcOutput    *Outputs;
  MrcFrequency DdrFrequency;
  UINT32       ChIdx;
  UINT32       DataIdx;
  UINT8        Index;
  UINT32       Offset;
  DATA0CH0_CR_DDRCRDLLCONTROL0_STRUCT            DllControl0;
  DATASHARED_CR_DDRCRTXDLLCONTROL0_STRUCT        TxDllControl0;
  DDRCCC_SHARED0_CR_DDRCRTXDLLCONTROL0_STRUCT    DdrCccTxDllControl0;

  Outputs = &MrcData->Outputs;
  DdrFrequency = Outputs->Frequency;

  for (DataIdx = 0; DataIdx < MRC_DATA_MOBILE_NUM; DataIdx++) {
    if (!(MrcGetHwPartitionExists (MrcData, PartitionDataShared, DataIdx, MRC_IGNORE_ARG))) {
      continue;
    }
    for (ChIdx = 0; ChIdx < MRC_DATA_CH_NUM; ChIdx++) {
      Offset = MrcGetDataOffset (MrcData, DATA0CH0_CR_DDRCRDLLCONTROL0_REG, MRC_IGNORE_ARG, ChIdx, DataIdx);
      DllControl0.Data = MrcReadCR (MrcData, Offset);
      DllControl0.Bits.VctlInit = 0;
      MrcWriteCR (MrcData, Offset, DllControl0.Data);
    }
    Offset = OFFSET_CALC_CH (DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL0_REG, DDRDATA_SHARED1_CR_DDRCRTXDLLCONTROL0_REG, DataIdx);
    TxDllControl0.Data = MrcReadCR (MrcData, Offset);
    TxDllControl0.Bits.VctlInit = 0;
    MrcWriteCR (MrcData, Offset, TxDllControl0.Data);
  }

  for (Index = 0; Index < MRC_CCC_SHARED_MOBILE_NUM; Index++) {
    if (!(MrcGetHwPartitionExists (MrcData, PartitionCccShared, Index, MRC_IGNORE_ARG))) {
      continue;
    }
    Offset = OFFSET_CALC_CH (DDRCCC_SHARED0_CR_DDRCRTXDLLCONTROL0_REG, DDRCCC_SHARED1_CR_DDRCRTXDLLCONTROL0_REG, Index);
    DdrCccTxDllControl0.Data = MrcReadCR (MrcData, Offset);
    DdrCccTxDllControl0.Bits.VctlInit = (DdrFrequency < f3200) ? 0 : ((DdrFrequency < f3600) ? 1 : ((DdrFrequency < f4400) ? 2 : 3));
    MrcWriteCR (MrcData, Offset, DdrCccTxDllControl0.Data);
  }
}

/**
  This function updates the DLL VCDL DCC Code

  @param[in, out] MrcData               - Pointer to MRC global data.
  @param[in]      Init                  - Init or not.

**/
VOID
DllVcdlDccCodeUpdate (
  IN OUT MrcParameters *const MrcData,
  IN     BOOLEAN       Init
  )
{
  UINT32    ChIdx;
  UINT32    DataIdx;
  UINT32    Offset;
  DATA0CH0_CR_DDRCRDLLCONTROL0_STRUCT  DllControl0;

  for (DataIdx = 0; DataIdx < MRC_DATA_MOBILE_NUM; DataIdx++) {
    if (!(MrcGetHwPartitionExists (MrcData, PartitionDataShared, DataIdx, MRC_IGNORE_ARG))) {
      continue;
    }
    for (ChIdx = 0; ChIdx < MRC_DATA_CH_NUM; ChIdx++) {
      Offset = MrcGetDataOffset (MrcData, DATA0CH0_CR_DDRCRDLLCONTROL0_REG, MRC_IGNORE_ARG, ChIdx, DataIdx);
      DllControl0.Data = MrcReadCR (MrcData, Offset);
      if (Init) {
        DllControl0.Bits.DllVcdlDccCodeEn = 1;
        DllControl0.Bits.DllVcdlDccCode   = 0; // set DCC code to default value center of [-8,7] range
      } else {
        DllControl0.Bits.DllVcdlDccCodeEn = 0;
      }
      MrcWriteCR (MrcData, Offset, DllControl0.Data);
    }
  }
}

/**
  This function updates the DATA BUFFER BONUS PER BIT

  @param[in, out] MrcData               - Pointer to MRC global data.
  @param[in]      Set                   - Set or clear bit 3

**/
VOID
DataBufferBonusPerBitUpdate (
  IN OUT MrcParameters *const MrcData,
  IN     BOOLEAN       Set
  )
{
  DDRDATA_SHARED0_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_STRUCT Byte0DataBufferBonusPerBit;
  DDRDATA_SHARED0_CR_DDRCRBYTE1DATABUFFERBONUSPERBIT0_STRUCT Byte1DataBufferBonusPerBit;

  Byte0DataBufferBonusPerBit.Data = MrcReadCR (MrcData, DDRDATA_SHARED0_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_REG);
  if (Set) {
    Byte0DataBufferBonusPerBit.Data |= 0x88888888; // Set bit 3 of each 4-bit bonus field for each lane
  } else {
    Byte0DataBufferBonusPerBit.Data &= 0x77777777; // Clear bit 3 of each 4-bit bonus field for each lane
  }
  MrcWriteCrMulticast (MrcData, DATASHARED_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_REG, Byte0DataBufferBonusPerBit.Data);

  Byte1DataBufferBonusPerBit.Data = MrcReadCR (MrcData, DDRDATA_SHARED0_CR_DDRCRBYTE1DATABUFFERBONUSPERBIT0_REG);
  if (Set) {
    Byte1DataBufferBonusPerBit.Data |= 0x88888888; // Set bit 3 of each 4-bit bonus field for each lane
  } else {
    Byte1DataBufferBonusPerBit.Data &= 0x77777777; // Clear bit 3 of each 4-bit bonus field for each lane
  }
  MrcWriteCrMulticast (MrcData, DATASHARED_CR_DDRCRBYTE1DATABUFFERBONUSPERBIT0_REG, Byte1DataBufferBonusPerBit.Data);
}

/**
  Program MCMISCS_DDRWCKCONTROL

  @param[in] MrcData  - Include all MRC global data
**/
VOID
MrcSetWckControl (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput     *Outputs;
  MrcInput      *Inputs;
  MrcDebug      *Debug;
  MrcTiming     *TimingProfile;
  MrcFrequency  DdrFrequency;
  UINT8         tCL;
  UINT8         tCWL;
  UINT8         tWCKENL_RD;
  UINT8         tWCKENL_WR;
  UINT32        PHClk;
  UINT32        QClk;
  UINT32        nCK;
  BOOLEAN       IsGear4;
  BOOLEAN       IsLpddr5;
  MCMISCS_DDRWCKCONTROL_STRUCT  McMiscsWckControl;

  Outputs      = &MrcData->Outputs;
  Inputs       = &MrcData->Inputs;
  Debug        = &Outputs->Debug;
  DdrFrequency = Outputs->Frequency;
  IsGear4      = Outputs->GearMode ? 1 : 0;
  IsLpddr5     = Outputs->IsLpddr5;
  PHClk             = DIVIDECEIL (2000000, DdrFrequency);
  QClk              = IsGear4 ? (PHClk * 2) : PHClk;
  nCK               = IsLpddr5 ? (PHClk * 4) : PHClk;

  McMiscsWckControl.Data = 0;
  if (Outputs->IsLpddr5) {
    TimingProfile = &Outputs->Timing[Inputs->ExtInputs.Ptr->MemoryProfile];
    tCL  = (UINT8) TimingProfile->tCL;
    tCWL = (UINT8) TimingProfile->tCWL;
    //tWCKENL_RD = RL + 1 - tWCKPRE_total_RD
    tWCKENL_RD = tCL + 1 - MrcGetWckPreRdTotalLpddr5 (MrcData, DdrFrequency);
    //tWCKENL_RD*nCK/QClk
    McMiscsWckControl.Bits.cas2rdwck = (tWCKENL_RD * nCK) / QClk;
    //tWCKENL_WR = WL + 1 - tWCKPRE_total_WR
    tWCKENL_WR = tCWL + 1 - MrcGetWckPreWrTotalLpddr5 (DdrFrequency);
    //tWCKENL_WR*nCK/QClk
    McMiscsWckControl.Bits.cas2wrwck = (tWCKENL_WR * nCK) / QClk;
    //Log(NCK/QClk,2)
    McMiscsWckControl.Bits.tWckHalfRate = MrcLog2Round (MrcData, nCK/QClk);
    //tWCKPRE_Static*nCK/QClk
    McMiscsWckControl.Bits.tWckPre = (MrcGetWckPreStaticLpddr5 (DdrFrequency) * nCK) / QClk;
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "McMiscsWckControl: 0x%08X\n", McMiscsWckControl.Data);
  MrcWriteCR (MrcData, MCMISCS_DDRWCKCONTROL_REG, McMiscsWckControl.Data);
}

/**
  This function configures the DDRIO to drive WCK according to the JEDEC Spec before Entering, or
  after exiting Command Bus Training Mode.

  For Entry, WCK will be driven static low while idle and prepare for WCK to toggle indefinitely when the pulse is started.
  For Exit, the pulse will be stopped; WCK force to idle will be removed allowing to return back to Tristate; and the pulse
    configuration will be cleared.

  @param[in] MrcData - Pointer to global MRC data.
  @param[in] IsCbtEnter - Defines if the call is for a CBT enter (TRUE) or CBT exit (FALSE)
**/
VOID
MrcDdrIoLpddrCbtWckMode (
  IN MrcParameters* const MrcData,
  IN BOOLEAN              IsCbtEnter
  )
{
  DDRSCRAM_CR_DDRMISCCONTROL2_STRUCT MiscControl2;
  MCMISCS_DDRWCKCONTROL_STRUCT WckControl;

  WckControl.Data = MrcReadCR (MrcData, MCMISCS_DDRWCKCONTROL_REG);
  MiscControl2.Data = MrcReadCR (MrcData, DDRSCRAM_CR_DDRMISCCONTROL2_REG);

  if (IsCbtEnter) {
    // Ungate WckDiffLowInIdle
    MiscControl2.Bits.TrainWCkEn = 1;
    MrcWriteCR (MrcData, DDRSCRAM_CR_DDRMISCCONTROL2_REG, MiscControl2.Data);

    //Set WCK Control for BL/Mask and DiffLowInIdle for training
    WckControl.Bits.TrainWCkBL = 0;
    WckControl.Bits.TrainWCkMask = 7;
    WckControl.Bits.WCkDiffLowInIdle = 1;
    MrcWriteCR (MrcData, MCMISCS_DDRWCKCONTROL_REG, WckControl.Data);
  } else {
    // Disable Diff Low In Idle & Clear BurstLength/Mask. Previous read will have TrainWCkPulse set.
    // It needs to be 0 here after we drive DQ7 low to not leave it on after completing training.
    WckControl.Bits.TrainWCkBL = 0;
    WckControl.Bits.TrainWCkMask = 0;
    WckControl.Bits.WCkDiffLowInIdle = 0;
    WckControl.Bits.TrainWCkPulse = 0;
    MrcWriteCR (MrcData, MCMISCS_DDRWCKCONTROL_REG, WckControl.Data);

    MiscControl2.Bits.TrainWCkEn = 0;
    MrcWriteCR (MrcData, DDRSCRAM_CR_DDRMISCCONTROL2_REG, MiscControl2.Data);
  }
}

/**
  This function does the refpi control register init.

  @param[in]  MrcData - Pointer to MRC global data.

**/
VOID
RefPiControlInit (
  IN  MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs;
  MrcDebug *Debug;
  BOOLEAN IsGear4;
  CCC_CR_REFPICONTROL_STRUCT RefPiControl;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  IsGear4 = Outputs->GearMode;

  // CTE requires different initial values when running RefPi Calibration
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Using CTE initial values for RefPi\n");
  RefPiControl.Data = 0;
  RefPiControl.Bits.RefPi4Xover       = 0;
  RefPiControl.Bits.RefPi             = 0;
  RefPiControl.Bits.RefPi4XoverOffset = 0x20;
  RefPiControl.Bits.RefPiOffset       = IsGear4 ? 0x80 : 0x40;
  MrcWriteCrMulticast (MrcData, CCC_CR_REFPICONTROL_REG, RefPiControl.Data);
  MrcWriteCrMulticast (MrcData, DATA_CR_REFPICONTROL_REG, RefPiControl.Data);
}

/**
  This function initializes the RefPiFsmControl register for DDRIO.
  All partitions which have RefPiFsmControl have the same structure which allows for a single init function.

  @param[in] MrcData   - Pointer to global MRC data.
  @param[in] FsmStage  - FSM stage
  @param[in] Offset    - Partiton offset for restoring previous values
  @param[in] PrintTask - Qualifier to include print statemenets

  @return UINT32 - Initialized value of RefPiFsmControl Register
**/
UINT32
MrcDdrIoInitRefPiFsmCtl (
  IN MrcParameters* const MrcData,
  IN MRC_REFPI_CAL_STAGE  FsmStage,
  IN UINT32               Offset,
  IN BOOLEAN              PrintTask
  )
{
  MrcDebug  *Debug;
  UINT16    MinWidth;
  UINT16    MaxWidth;
  UINT32    PrintLevel;
  BOOLEAN   IsGear4;
  DATA0CH0_CR_REFPIFSMCONTROL_STRUCT  RefPiFsmControl;
  MrcInput  *Inputs;

  Inputs     = &MrcData->Inputs;
  Debug      = &MrcData->Outputs.Debug;
  IsGear4    = MrcData->Outputs.GearMode;
  PrintLevel = PrintTask ? MSG_LEVEL_ALGO : MSG_LEVEL_NONE;
  if (Inputs->IsDdrIoMbA0) {
    MinWidth = 4;
  } else {
    MinWidth = 6;
  }
  MaxWidth   = IsGear4 ? 256 : 128;

  if (FsmStage & MrcRefPi2XOverAlign) {
    RefPiFsmControl.Data = MrcReadCR (MrcData, Offset);
  } else {
    RefPiFsmControl.Data = 0;
    RefPiFsmControl.Bits.TestCycle = 6;   // take 2^6 samples per PI tick
    RefPiFsmControl.Bits.MinWidth = MinWidth;
    RefPiFsmControl.Bits.MaxVal   = MaxWidth + (2 * MinWidth);
    RefPiFsmControl.Bits.SaveModN = IsGear4 ? 2 : 1;
  }

  switch (FsmStage) {
    case MrcRefPi2QClkDataCcc:
      RefPiFsmControl.Bits.Param = 1;           // setup FSM to run RefPi Training
      RefPiFsmControl.Bits.WriteValParam = 0;
      MRC_DEBUG_MSG (Debug, PrintLevel, " > Edge align QClk to Refpi for %s", "Data/CCC partition");
      break;

    case MrcRefPi2XOverAlign:                   // RefPi4XOver Alignment
      RefPiFsmControl.Bits.MinVal = 0;
      RefPiFsmControl.Bits.MaxVal = 128 + (2 * MinWidth);
      RefPiFsmControl.Bits.Param = 2;           // configure FSM to align RefPi4XOver to Refpi
      RefPiFsmControl.Bits.WriteValParam = 1;   // Automatically update Refpi4XOver field
      MRC_DEBUG_MSG (Debug, PrintLevel, " > Align RefPi4XOver to Refpi");
      break;

    case MrcRefPiDefault:                       // FSM disabled (not run for COMP partition)
    default:
      RefPiFsmControl.Bits.Param = 0;
      RefPiFsmControl.Bits.WriteValParam = 0;
      break;
  }

  MRC_DEBUG_MSG (Debug, PrintLevel, " (0x%x)\n", RefPiFsmControl.Data);
  return RefPiFsmControl.Data;
}

/**
  This function sets RefPiFsmControl to the correct value for input FSM stage.
  This is acieved via CR Multicast in the DDRIO.

  @param[in]  MrcData      - Pointer to MRC global data.
  @param[in]  FsmStage     - FSM calibration stage
**/
VOID
MrcDdrIoResetRefPiFsmCtl (
  IN  MrcParameters* const MrcData,
  IN  MRC_REFPI_CAL_STAGE  FsmStage
  )
{
  UINT32   Offset;
  UINT32   ChIdx;
  UINT32   Index;
  UINT32   DataIdx;
  UINT32   DataMax;
  CCC_CR_REFPIFSMCONTROL_STRUCT RefPiFsmCtrl;

  DataMax = MRC_DATA_MOBILE_NUM;

  // CCC Partitions
  for (Index = 0; Index < MRC_CCC_NUM; Index++) {
    if (!(MrcGetHwPartitionExists (MrcData, PartitionCcc, Index, MRC_IGNORE_ARG))) {
      continue;
    }
    Offset = OFFSET_CALC_CH (CH0CCC_CR_REFPIFSMCONTROL_REG, CH1CCC_CR_REFPIFSMCONTROL_REG, Index);
    RefPiFsmCtrl.Data = MrcDdrIoInitRefPiFsmCtl (MrcData, FsmStage, Offset, (Index==0 ? MRC_PRINT_CURRENT_TASK : MRC_SKIP_PRINT));
    MrcWriteCR (MrcData, Offset, RefPiFsmCtrl.Data);
  }

  // DATA Partitions
  for (DataIdx = 0; DataIdx < DataMax; DataIdx++) {
    if (!(MrcGetHwPartitionExists (MrcData, PartitionDataShared, DataIdx, MRC_IGNORE_ARG))) {
      continue;
    }
    for (ChIdx = 0; ChIdx < MRC_DATA_CH_NUM; ChIdx++) {
      Offset = MrcGetDataOffset (MrcData, DATA0CH0_CR_REFPIFSMCONTROL_REG, MRC_IGNORE_ARG, ChIdx, DataIdx);
      RefPiFsmCtrl.Data = MrcDdrIoInitRefPiFsmCtl (MrcData, FsmStage, Offset, (DataIdx==0 ? MRC_PRINT_CURRENT_TASK : MRC_SKIP_PRINT));
      MrcWriteCR (MrcData, Offset, RefPiFsmCtrl.Data);
    }
  }
}

/**
  This function accesses RefPi and RefP4Xover from the DDRIO based on the specific partition
  passed in ParInfo and stores the result in the output variable RefPiData.

  @param[in]  MrcData   - Pointer to global MRC data.
  @param[out] RefPiData - Pointer to the storage variable for RefPi and RefPi4Xover.
  @param[out] SaveCrPtr - Pointer to the storage variable for saved CR.
  @param[in]  Offset    - The register address offset to be read.

  @retval MrcStatus - mrcWrongInputParameter if RefPiData or ParInfo is NULL;
                    - mrcFail if the ParType in ParInfo isn't supported
                    - mrcSuccess otherwise.
**/
MrcStatus
MrcReadUpdateRefPiData (
  IN        MrcParameters             *MrcData,
  OUT       MRC_REFPI_DATA_TYPE       *RefPiData,
  OUT       UINT32                    *SaveCrPtr,
  IN        UINT32                     Offset
  )
{
  MrcStatus Status;
  DATA_CR_REFPICONTROL_STRUCT RefPiCtl;

  Status = mrcSuccess;
  // Read Data
  RefPiCtl.Data = MrcReadCR (MrcData, Offset);

  // Get RefPi and RefPi4Xover
  // Store in Output location.
  RefPiData->RefPi = (UINT8) RefPiCtl.Bits.RefPi;
  RefPiData->RefPiOffset = (UINT8) RefPiCtl.Bits.RefPiOffset;
  RefPiData->RefPi4Xover = (UINT8) RefPiCtl.Bits.RefPi4Xover;
  RefPiData->RefPi4XoverOffset = (UINT8) RefPiCtl.Bits.RefPi4XoverOffset;

  if (SaveCrPtr != NULL) {
    *SaveCrPtr = RefPiCtl.Data;
  } else {
    Status = mrcFail;
  }

  return Status;
}

/**
  This function sets XSyncDelay

  @param[in] MrcData  - Include all MRC global data.
**/
VOID
MrcSetXSyncDelay (
  IN MrcParameters *const MrcData
  )
{
  UINT32 PhyDelay;
  UINT32 Usync1OutStage;
  DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_STRUCT DdrCrTlClkDrvCtrl;
  DDRSCRAM_CR_DDRMISCCONTROL2_STRUCT DdrMiscControl2;

  PhyDelay = 5;

  //XsyncDelay = usync1_out_stage - [4 + PHYDelay]
  //usync1_out_stage = DDRTLCLK_CR_DDRCRTLCLKDRVCTRL_0_0_0_MCHBAR.LjpllUsync1Out
  DdrCrTlClkDrvCtrl.Data = MrcReadCR (MrcData, DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_REG);
  Usync1OutStage = DdrCrTlClkDrvCtrl.Bits.LjpllUsync1Out;
  DdrMiscControl2.Data = MrcReadCR (MrcData, DDRSCRAM_CR_DDRMISCCONTROL2_REG);
  DdrMiscControl2.Bits.XSyncDelay = Usync1OutStage - (4 + PhyDelay);
  MrcWriteCR (MrcData, DDRSCRAM_CR_DDRMISCCONTROL2_REG, DdrMiscControl2.Data);
}

/**
  This function updates CBMix and CBMux based on the data rate

  @param[in, out] MrcData - MRC global data.
**/
VOID
CbMixMuxConfig (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcOutput         *Outputs;
  MrcFrequency      Frequency;
  UINT32            CbMux;
  UINT32            CbMix;
  DATASHARED_DDRTXDLL_CR_PICODELUT1_STRUCT   PiCodeLUT1;
  DATASHARED_DDRTXDLL_CR_PICODELUT2_STRUCT   PiCodeLUT2;

  Outputs   = &MrcData->Outputs;
  Frequency = Outputs->Frequency;

  CbMux = (Frequency <= f2000) ? 0xA : ((Frequency <= f3300) ? 0x8 : 0);
  CbMix = (Frequency <= f2000) ? 0x3 : ((Frequency <= f3300) ? 0x1 : 0);

  PiCodeLUT1.Data = MrcReadCR (MrcData, DATASHARED_DDRTXDLL_CR_PICODELUT1_REG);
  PiCodeLUT2.Data = MrcReadCR (MrcData, DATASHARED_DDRTXDLL_CR_PICODELUT2_REG);
  PiCodeLUT1.Bits.CbMix = CbMix;
  PiCodeLUT2.Bits.CbMux = CbMux;

  MrcWriteCrMulticast (MrcData, DATASHARED_DDRTXDLL_CR_PICODELUT1_REG, PiCodeLUT1.Data);
  MrcWriteCrMulticast (MrcData, DATASHARED_DDRRXDLL_CR_PICODELUT1_REG, PiCodeLUT1.Data);

  MrcWriteCrMulticast (MrcData, DATASHARED_DDRTXDLL_CR_PICODELUT2_REG, PiCodeLUT2.Data);
  MrcWriteCrMulticast (MrcData, DATASHARED_DDRRXDLL_CR_PICODELUT2_REG, PiCodeLUT2.Data);
}

/**
  This function program OffsetCalCnt.

  @param[in, out] MrcData - MRC global data.

**/
VOID
MrcProgramOffsetCalCnt (
  IN OUT MrcParameters *const MrcData
  )
{
  DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_STRUCT   PllFusePhClk;

  PllFusePhClk.Data = MrcReadCR (MrcData, DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_REG);
  PllFusePhClk.Bits.OffsetCalCnt = 7;
  MrcWriteCR (MrcData, DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_REG, PllFusePhClk.Data);
  MrcWriteCR (MrcData, DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEAUXCLK_REG, PllFusePhClk.Data);
}

/**
  This function program MISCCONTROL2.

  @param[in, out] MrcData - MRC global data.
  @param[in]      Restore - MRC global data.
  @param[in, out] OrigMiscWckControl - The data to be read.

**/
VOID
MrcProgramMiscWckControl (
  IN OUT MrcParameters *const MrcData,
  IN     BOOLEAN        Restore,
  IN OUT UINT32         *OrigMiscWckControl
  )
{
  MCMISCS_DDRWCKCONTROL_STRUCT                MiscWckControl;
  if (Restore) {
    MrcWriteCR (MrcData, MCMISCS_DDRWCKCONTROL_REG, *OrigMiscWckControl);
  } else {
    MiscWckControl.Data = (*OrigMiscWckControl) = MrcReadCR (MrcData, MCMISCS_DDRWCKCONTROL_REG);
    MiscWckControl.Bits.TrainWCkBL = DCC_RISE_FALL_WCK_BL;
    MiscWckControl.Bits.TrainWCkMask = DCC_RISE_FALL_WCK_MASK;
    MiscWckControl.Bits.TrainWCkPulse = DCC_RISE_FALL_WCK_PULSE;
    MiscWckControl.Bits.WCkDiffLowInIdle = 1;
    MrcWriteCR (MrcData, MCMISCS_DDRWCKCONTROL_REG, MiscWckControl.Data);
  }
}

/**
  This function configures the RefPI FSM control configuration for POR operation after calibration is complete.

  @param[in]  MrcData     - Pointer to global MRC data.

  @retval MrcStatus
**/
MrcStatus
MrcFinalizeRefPiFsmControl (
  IN  MrcParameters *const MrcData
  )
{
  MrcStatus Status;
  DATA_CR_REFPIFSMCONTROL_STRUCT  RefPiFsmCtl;

  Status = mrcSuccess;

  RefPiFsmCtl.Data = MrcDdrIoInitRefPiFsmCtl(MrcData, MrcRefPiDefault, MRC_REFPI_DEFAULT, MRC_PRINT_CURRENT_TASK);   // Reset FsmControl for CCC and DATA partitions
  MrcWriteCrMulticast (MrcData, CCC_CR_REFPIFSMCONTROL_REG, RefPiFsmCtl.Data);
  MrcWriteCrMulticast (MrcData, DATA_CR_REFPIFSMCONTROL_REG, RefPiFsmCtl.Data);

  return Status;
}

/**
  Setup Data Dcc Lane En.

  @param[in, out] MrcData       - Include all MRC global data.
  @param[in]      DataDccLaneEn -  The value for GsmDataDccLaneEn.

  @retval none
**/
VOID
MrcDataDccLaneEn (
  IN OUT MrcParameters *const   MrcData,
  IN     INT64                  DataDccLaneEn
  )
{
}

/**
  Setup Data Dcc Lane En.

  @param[in, out] MrcData         - Include all MRC global data.
  @param[in]      DccPHClkPhaseEn -  The value for GsmDccPHClkPhaseEn.

  @retval none
**/
VOID
MrcDccPHClkPhaseEn (
  IN OUT MrcParameters *const   MrcData,
  IN     INT64                  DccPHClkPhaseEn
  )
{
}

/**
  This routine computes the read/write fifo pointer delays

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[in]  tCWL              - Write Latency for current channel
  @param[in]  Controller        - Current Controller
  @param[in]  Channel           - Current Channel
  @param[out] tCWL4TxDqFifoWrEn - Pointer to the write TX DQ fifo delay
  @param[out] tCWL4TxDqFifoRdEn - Pointer to the read TX DQ fifo delay

  @retval N/A.
**/
VOID
MrcGetTxDqFifoDelay(
  IN OUT MrcParameters* const MrcData,
  IN     UINT8          tCWL,
  IN     UINT8          Controller,
  IN     UINT8          Channel,
  OUT    UINT32*        tCWL4TxDqFifoWrEn,
  OUT    UINT32*        tCWL4TxDqFifoRdEn
  )
{
  MrcOutput* Outputs;
  MrcInput*  Inputs;
  UINT8      Index;
  UINT8      WrPreambleT;
  UINT8      WrPreambleLow;
  UINT8      WrPreambleVar;
  BOOLEAN    IsGear4;
  BOOLEAN    IsLpddr;

  Outputs = &MrcData->Outputs;
  Inputs = &MrcData->Inputs;
  IsLpddr = Outputs->IsLpddr;
  IsGear4 = (Outputs->GearMode) ? 1 : 0;
  MrcGetPrePostamble (MrcData, GetTimingWrite, &WrPreambleT, &WrPreambleLow, NULL);

  *tCWL4TxDqFifoWrEn = tCWL;  // @todo_lnl should also include a programmable delay from Dunit
  *tCWL4TxDqFifoRdEn = tCWL;

  // tcwl4txdqfifowren = ((tech==DDRPHY_LPDDR5) ? ((gears==2 ? ((cwls*4)-'h8) : ((cwls*2)-8)) -4):
  //                       ((gears==2)?(cwls-6):(cwls/2-6))-4)
  // tcwl4txdqfifowren = (tcwl4txdqfifowren) + (tcwl4txdqfifowren%2);
  //
  // wr_preamble = WrPreambleT + WrPreambleLow - 1
  // tcwl4txdqfiforden = (tech==DDRPHY_LPDDR5) ? ((gears==2)? (Write_latency*4-7) : (write_latency*2-7)) :
  //                      ((gears==2) ? ((write_latency-'h7)-(wr_preamble) ): (write_latency/2-'h7-((wr_preamble+1)/2) ));
  //
  if (IsLpddr) {
    Index = (Controller * MAX_CHANNEL) + Channel;
    *tCWL4TxDqFifoWrEn *= (IsGear4 ? 2 : 4);
    *tCWL4TxDqFifoWrEn -= 12;

    *tCWL4TxDqFifoRdEn *= (IsGear4 ? 2 : 4);
    *tCWL4TxDqFifoRdEn -= 7;

    *tCWL4TxDqFifoRdEn += ((Index >= 6) ? 1 : 0);
  } else { // Ddr5
    Index = (Controller * MAX_CHANNEL) + (Channel * 2);
    *tCWL4TxDqFifoWrEn /= (IsGear4 ? 2 : 1);
    *tCWL4TxDqFifoWrEn -= 6;
    *tCWL4TxDqFifoWrEn -= 4;

    WrPreambleVar = WrPreambleT + WrPreambleLow - 1;
    *tCWL4TxDqFifoRdEn /= (IsGear4 ? 2 : 1);
    *tCWL4TxDqFifoRdEn -= 7;
    *tCWL4TxDqFifoRdEn -= IsGear4 ? ((WrPreambleVar + 1) / 2) : WrPreambleVar;
    *tCWL4TxDqFifoRdEn += (Outputs->Frequency == f3200) ? (IsGear4 ? 1 : 0) : 0;
  }
  if (Inputs->ExtInputs.Ptr->DqLoopbackTest) {
    *tCWL4TxDqFifoRdEn -= 1;
  }
  *tCWL4TxDqFifoWrEn += *tCWL4TxDqFifoWrEn % 2;
  *tCWL4TxDqFifoRdEn += Inputs->ExtInputs.Ptr->CccPinsInterleaved ? DdrioChDeltaCccIL[Index] : DdrioChDelta[Index];
}

/**
  This function returns channel bit mask of the channels that hardware exists even though it may not be populated.

  @param[in] MrcData - Pointer to MRC global data.

  @returns channel bit mask that exists in hardware.
**/
UINT8
MrcGetValidHwChBitMask (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs = &MrcData->Outputs;
  MrcIpDerivative IpDerivative = MrcData->Inputs.DdrIoIpVersion.Bits.Derivative;
  MrcDdrType DdrType = Outputs->DdrType;

  MRC_DEBUG_ASSERT (DdrType == MRC_DDR_TYPE_DDR5 || DdrType == MRC_DDR_TYPE_LPDDR5,
                    &Outputs->Debug,
                    "Invalid DdrType (%d) which is not supported\n", DdrType);
  MRC_DEBUG_ASSERT (IpDerivative == ipDerivativeWcl || IpDerivative == ipDerivativePtl,
                    &Outputs->Debug,
                    "Invalid DdrioIpDerivative (%d) which is not supported\n", IpDerivative);

  return HwChBitMasks[IpDerivative][DdrType];
}

/**
  Setup DQ override.

  @param[in, out] MrcData         - Include all MRC global data.
  @param[in]      IsEnabled       - If set to TRUE, programs dq pins high.
                                    If set to FALSE, disables DQ override.
**/
VOID
MrcConfigureDqPins (
  IN OUT MrcParameters *const   MrcData,
  BOOLEAN                       IsEnabled
  )
{
  INT64   GetSetVal;

  if (IsEnabled) {
    GetSetVal = 0xFF;
  } else {
    GetSetVal = 0x0;
  }

  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocDqOverrideData, WriteCached, &GetSetVal);
  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocDqOverrideEn,   WriteCached, &GetSetVal);
}

/**
  This function gets or sets the value for ForceRxOn.

  @param[in]      MrcData               - Include all MRC global data.
  @param[in]      UpdateMode            - The mode to be used for program.
  @param[in]      GetSetMode            - The GetSet mode to be used for program.
  @param[in, out] *ForceRxOnDqsOrAmpOn  - Value to be written / Saved for ForceRxOn.
  @param[in, out] *ForceRxOnDqsMux      - Value not used for PTL.
  @param[in, out] *ForceRxOnDq          - Value not used for PTL.

  @retval NA
**/
VOID
MrcGetSetForceRxOn (
  IN     MrcParameters* const MrcData,
  IN     UPDATE_MODE    UpdateMode,
  IN     UINT32         GetSetMode,
  IN OUT INT64          *ForceRxOnDqsOrAmpOn,
  IN OUT INT64          *ForceRxOnDqsMux,
  IN OUT INT64          *ForceRxOnDq
  )
{
  MrcOutput       *Outputs;
  UINT32          Mc;
  UINT32          Ch;

  Outputs = &MrcData->Outputs;

  if (UpdateMode == SaveValue) {
    Mc = Outputs->FirstPopController;
    Ch = Outputs->Controller[Mc].FirstPopCh;

    // Assumption made that value in ForceRxOnDqs is the same as in RxOnDqsMux and RxOnDq
    MrcGetSetChStrb (MrcData, Mc, Ch, 0, GsmIocForceRxAmpOn, GetSetMode, ForceRxOnDqsOrAmpOn);
  } else if (UpdateMode == UpdateValue) {
    MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocForceRxAmpOn, GetSetMode, ForceRxOnDqsOrAmpOn);
  }
}

/**
  Find initial Cben and BwSel values based on the current Frequency and Gear mode

  @param[in]  MrcData   - Pointer to global MRC data.
  @param[out] BwSel - The BW code based on gear and frequency
  @param[out] Cben  - The CB code based on gear and frequency
*/
VOID
MrcBwSelCbenFreqSet (
  IN  MrcParameters *const MrcData,
  OUT UINT32               *BwSel,
  OUT UINT32               *Cben
  )
{
  MrcInput            *Inputs;
  MRC_EXT_INPUTS_TYPE *ExtInputs;
  MrcFrequency        Frequency;

  Inputs    = &MrcData->Inputs;
  ExtInputs = Inputs->ExtInputs.Ptr;
  Frequency = MrcData->Outputs.Frequency;

  if (ExtInputs->CccHalfFrequency) {
    if (Frequency <= f2267) {
      *BwSel = 1;
    } else if (Frequency <= f3000) {
      *BwSel = 2;
    } else if (Frequency <= f4400) {
      *BwSel = 5;
    } else if (Frequency <= f8533) {
      *BwSel = 9;
    } else {
      *BwSel = 8;
    }

    if (Frequency < f6267) {
      *Cben = 3;
    } else if (Frequency <= f9200) {
      *Cben = 2;
    } else {
      *Cben = 1;
    }
  } else {
    if (Frequency <= f2267) {
      *BwSel = 5;
    } else if (Frequency <= f4267) {
      *BwSel = 9;
    } else if (Frequency <= f6133) {
      *BwSel = 8;
    } else if (Frequency <= f8533) {
      *BwSel = 4;
    } else {
      *BwSel = 7;
    }

    if (Frequency <= f3067) {
      *Cben = 3;
    } else if (Frequency <= f4267) {
      *Cben = 2;
    } else if (Frequency <= f6133) {
      *Cben = 1;
    } else if (Frequency <= f9200) {
      *Cben = 0;
    } else {
      *Cben = 3;
    }
  }
}

/**
  Configuring the bit channel_not_populated.

  @param[in] MrcData - Pointer to MRC global data
  @return VOID
**/
VOID
MrcSetChannelNotPopulated (
  MrcParameters * MrcData
)
{
}
