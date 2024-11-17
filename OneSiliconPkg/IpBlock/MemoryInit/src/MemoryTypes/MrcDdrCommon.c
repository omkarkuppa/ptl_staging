/** @file
  Implements functions that are common various LP/DDR technologies.

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
**/
#include "CMcAddress.h"
#include "MrcCommon.h"
#include "MrcDdrCommon.h"
#include "MrcDdr5.h"
#include "MrcChipApi.h"
#include "MrcGeneral.h"
#include "MrcLpddr5.h"
#include "MrcDdrIoUtils.h"
#include "MrcPmaApi.h"
#include "MrcHalRegisterAccess.h"

///
/// Initialization Timing Parameters
///
#define MRC_LP_tINIT0_US   20000   ///< Max voltage-ramp time
#define MRC_LP_tINIT4_TCK  5       ///< Min stable clock before first CKE high

const UINT8 Lp5ReadLatencyDvfscDisabledSet0[15] = { 3, 4, 5, 6, 8,  9, 10, 12, 13, 15, 16, 17, 20, 23, 25 };
const UINT8 Lp5ReadLatencyDvfscDisabledSet1[15] = { 3, 4, 5, 7, 8, 10, 11, 13, 14, 16, 17, 18, 22, 25, 28 };
const UINT8 Lp5ReadLatencyDvfscDisabledSet2[15] = { 3, 4, 6, 7, 9, 10, 12, 14, 15, 17, 19, 20, 24, 26, 29 };

const UINT8 Lp5ReadLatencyDvfscEnabledSet0[6] = { 3, 5, 7, 8, 10, 12 };
const UINT8 Lp5ReadLatencyDvfscEnabledSet1[6] = { 3, 5, 7, 8, 10, 12 };
const UINT8 Lp5ReadLatencyDvfscEnabledSet2[6] = { 3, 5, 7, 10, 12, 14 };

// This table is used for LPDDR MR5 decoding
struct {
  UINT8   VendorId;
  UINT16  JedecId;
  char    *VendorName;
} DramVendorList [] = {
  { 1,    0xCE00, "Samsung" },
  { 3,    0xFE02, "Elpida"  },
  { 6,    0xAD00, "Hynix"   },
  { 0xFF, 0x2C00, "Micron"  },
};

/**
  Calculate the tCL value for LPDDR5.

  JEDEC Spec Table 225 - Read Latencies for Read Link ECC off case (DVFSC disabled, Enhanced DVFSC disabled):

    Lower Clk   Upper Clk        Read Latency
    Freq Limit  Freq Limit     Set0  Set1  Set2
    -------------------------------------------
      5            67           3      3      3
     67           133           4      4      4
    133           200           5      5      6
    200           267           6      7      7
    267           344           8      8      9
    344           400           9     10     10
    400           467          10     11     12
    467           533          12     13     14
    533           600          13     14     15
    600           688          15     16     17
    688           750          16     17     19
    750           800          17     18     20
    800           937.5        20     22     24
    937.5        1066.5        23     25     26


    JEDEC Spec Table 225 - Read Latencies for Read Link ECC off case (DVFSC disabled, Enhanced DVFSC enabled):

    Lower Clk   Upper Clk        Read Latency
    Freq Limit  Freq Limit     Set0  Set1  Set2
    -------------------------------------------
      5            67           3      3      3
     67           133           5      5      5
    133           200           7      7      7
    200           267           8      8     10
    267           344           10    10     12
    344           400           12    12     14


    Set0 - x16, No DBI
    Set1 - x8 and No DBI, or x16 and DBI
    Set2 - x8 and DBI

    @param[in] tCK          - The memory tCK in femtoseconds.
    @param[in] SdramWidth   - SDRAM width (8 or 16)
    @param[in] IsDbiEnabled - TRUE if DBI is enabled
    @param[in] IsDvfscEnabled - TRUE if Dvfsc is enabled

    @retval LPDDR5 tCL in tCK units
**/
UINT32
GetLpddr5tCL (
  IN const UINT32     tCK,
  IN UINT8            SdramWidth,
  IN BOOLEAN          IsDbiEnabled,
  IN BOOLEAN          IsDvfscEnabled
  )
{
  UINT32 tCL;
  UINT32 tCKNorm;
  UINT32 RlSet;
  UINT32 Index;

  // Scale tCK up to typical DDR ratio of 2:1 between tCK and Data Rate
  // We are always in 4:1 mode for WCK.
  tCKNorm = tCK / 4;

  RlSet = 0;
  if (SdramWidth == 8) {
    RlSet += 1;
  }
  if (IsDbiEnabled) {
    RlSet += 1;
  }

  Index = 0;
  if (!IsDvfscEnabled) {
    if (tCKNorm >= MRC_DDR_533_TCK_MIN) {
      Index = 0;
    } else if (tCKNorm >= MRC_DDR_1067_TCK_MIN) {
      Index = 1;
    } else if (tCKNorm >= MRC_DDR_1600_TCK_MIN) {
      Index = 2;
    } else if (tCKNorm >= MRC_DDR_2133_TCK_MIN) {
      Index = 3;
    } else if (tCKNorm >= MRC_DDR_2750_TCK_MIN) {
      Index = 4;
    } else if (tCKNorm >= MRC_DDR_3200_TCK_MIN) {
      Index = 5;
    } else if (tCKNorm >= MRC_DDR_3733_TCK_MIN) {
      Index = 6;
    } else if (tCKNorm >= MRC_DDR_4267_TCK_MIN) {
      Index = 7;
    } else if (tCKNorm >= MRC_DDR_4800_TCK_MIN) {
      Index = 8;
    } else if (tCKNorm >= MRC_DDR_5500_TCK_MIN) {
      Index = 9;
    } else if (tCKNorm >= MRC_DDR_6000_TCK_MIN) {
      Index = 10;
    } else if (tCKNorm >= MRC_DDR_6400_TCK_MIN) {
      Index = 11;
    } else if (tCKNorm >= MRC_DDR_7500_TCK_MIN) {
      Index = 12;
    } else if (tCKNorm >= MRC_DDR_8533_TCK_MIN) {
      Index = 13;
    } else {
      Index = 14;
    }
    if (RlSet == 0) {
      tCL = Lp5ReadLatencyDvfscDisabledSet0[Index];
    } else if (RlSet == 1) {
      tCL = Lp5ReadLatencyDvfscDisabledSet1[Index];
    } else {
      tCL = Lp5ReadLatencyDvfscDisabledSet2[Index];
    }
  } else {  //Dvfsc enabled
    if (tCKNorm >= MRC_DDR_533_TCK_MIN) {
      Index = 0;
    } else if (tCKNorm >= MRC_DDR_1067_TCK_MIN) {
      Index = 1;
    } else if (tCKNorm >= MRC_DDR_1600_TCK_MIN) {
      Index = 2;
    } else if (tCKNorm >= MRC_DDR_2133_TCK_MIN) {
      Index = 3;
    } else if (tCKNorm >= MRC_DDR_2750_TCK_MIN) {
      Index = 4;
    } else {
      Index = 5;
    }

    if (RlSet == 0) {
      tCL = Lp5ReadLatencyDvfscEnabledSet0[Index];
    } else if (RlSet == 1) {
      tCL = Lp5ReadLatencyDvfscEnabledSet1[Index];
    } else {
      tCL = Lp5ReadLatencyDvfscEnabledSet2[Index];
    }
  }
  return tCL;
}

/**
  This function converts from the integer defined Read Latency to the Mode Register
  encoding of the timing in LPDDR5.

  @param[in]  MrcData - Pointer to global MRC data.
  @param[in]  Value   - Requested Read Latency value.
  @param[out] EncVal  - Encoded Mode Register value.

  @retval MrcStatus - mrcSuccess if the latency is supported.  Else mrcWrongInputParameter.
**/
MrcStatus
EncodeReadLatencyLpddr5 (
  IN  MrcParameters *MrcData,
  IN  UINT16        Value,
  OUT UINT8         *EncVal
  )
{
  MrcOutput   *Outputs;
  MrcDebug    *Debug;
  MrcStatus   Status;
  UINT8       MrValue;
  UINT8       RlSet;
  UINT8       Index;
  UINT8       ReadLatencyValueArrLength;
  const UINT8 *ReadLatency;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  Status = mrcSuccess;
  RlSet = 0;

  if (Outputs->LpByteMode) {
    RlSet += 1;
  }
  if (Outputs->IsDbiReadEnabled) {  // Check the current status of DBI enable, as this function is used for MR programming
    RlSet += 1;
  }

  // Use MR2 table from JEDEC spec - "MR2 Register Definition"
  if (!Outputs->IsDvfscEnabled) {
    if (RlSet == 0) {
      ReadLatency = Lp5ReadLatencyDvfscDisabledSet0;
    } else if (RlSet == 1) {
      ReadLatency = Lp5ReadLatencyDvfscDisabledSet1;
    } else {
      ReadLatency = Lp5ReadLatencyDvfscDisabledSet2;
    }
  } else {
    if (RlSet == 0) {
      ReadLatency = Lp5ReadLatencyDvfscEnabledSet0;
    } else if (RlSet == 1) {
      ReadLatency = Lp5ReadLatencyDvfscEnabledSet1;
    } else {
      ReadLatency = Lp5ReadLatencyDvfscEnabledSet2;
    }
  }

  MrValue = 0xFF;
  ReadLatencyValueArrLength = (Outputs->IsDvfscEnabled) ? LP5_READ_LATENCY_VALUES_DVFSC : LP5_READ_LATENCY_VALUES;
  for (Index = 0; Index < ReadLatencyValueArrLength; Index++) {
    if (ReadLatency[Index] == Value) {
      MrValue = Index;
      break;
    }
  }

  if (MrValue != 0xFF) {
    // Check to see if the time requested matches JEDEC Frequency table
    if (!Outputs->IsDvfscEnabled) {
      Status = LatencyFreqCheckLpddr5 (MrcData, MrValue);
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, (Status != mrcSuccess) ? " (RL)\n" : "");
    }
    if (EncVal != NULL) {
      *EncVal = MrValue;
    } else {
      Status = mrcWrongInputParameter;
    }
  } else {
    Status = mrcWrongInputParameter;
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Invalid %s Latency Value: %d\n", gErrString, gRdString, Value);
  }

  return Status;
}

/**
  Print non-LP MRs

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess
**/
MrcStatus
MrcPrintDdrMrs (
  IN MrcParameters *const MrcData
  )
{
  MrcDebug          *Debug;
  MrcOutput         *Outputs;
  MrcChannelOut     *ChannelOut;
  UINT32            Channel;
  UINT32            Controller;
  UINT32            Rank;
  UINT32            RankMod2;
  UINT32            MrIndex;

  Outputs       = &MrcData->Outputs;
  Debug         = &Outputs->Debug;

  if (Outputs->IsLpddr) {
    return mrcSuccess;
  }
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!(MrcRankExist (MrcData, Controller, Channel, Rank))) {
          continue;
        }
        ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
        RankMod2 = Rank % 2;
        for (MrIndex = mrMR0; MrIndex <= mrMR6; MrIndex++) {
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "MrcSetMR%u  Mc %u Channel %u Rank %u = 0x%X\n",
            MrIndex,
            Controller,
            Channel,
            Rank,
            ChannelOut->Dimm[RANK_TO_DIMM_NUMBER (Rank)].Rank[RankMod2].MR[MrIndex]
          );
        }
      } // Rank
    } // Channel
  } // Controller
  return mrcSuccess;
}

/**
  This function converts from LPDDR5 MR ODT encoding to Ohms.

  @param[in]  DecodeVal - Encoded ODT value.

  @retval INT16 - ODT Value in Ohms if valid. Else, -2.
**/
INT16
LpddrOdtDecode (
  IN  UINT16 DecodeVal
  )
{
  INT16      OdtValue;

  switch (DecodeVal) {
    case OdtOff:
      OdtValue = 0xFFFF;
      break;

    case Odt240:
      OdtValue = 240;
      break;

    case Odt120:
      OdtValue = 120;
      break;

    case Odt80:
      OdtValue = 80;
      break;

    case Odt60:
      OdtValue = 60;
      break;

    case Odt48:
      OdtValue = 48;
      break;

    case Odt40:
      OdtValue = 40;
      break;

    default:
      OdtValue = -2;
      break;
  }

  return OdtValue;
}


/**
  This function converts from Ohms to LPDDR5 MR ODT encoding.

  @param[in]  OdtValue  - ODT Value in Ohms.

  @retval INT8 - Encoding if valid ODT value.  Else, -1.
**/
LPDDR_ODT_ENCODING
LpddrOdtEncode (
  IN  UINT16  OdtValue
  )
{
  LPDDR_ODT_ENCODING  EncodeVal;

  switch (OdtValue) {
    case 0xFFFF:
    case 0:
      EncodeVal = OdtOff;
      break;

    case 240:
      EncodeVal = Odt240;
      break;

    case 120:
      EncodeVal = Odt120;
      break;

    case 80:
      EncodeVal = Odt80;
      break;

    case 60:
      EncodeVal = Odt60;
      break;

    case 48:
      EncodeVal = Odt48;
      break;

    case 40:
      EncodeVal = Odt40;
      break;

    default:
      EncodeVal = -1;
      break;
  }

  return EncodeVal;
}

/**
  This function converts from Ohms to LPDDR5 MR CS ODT encoding.

  @param[in]  OdtValue  - ODT Value in Ohms.

  @retval INT8 - Encoding if valid ODT value.  Else, -1.
**/
LPDDR_CS_ODT_ENCODING
LpddrCsOdtEncode (
  IN  UINT16  OdtValue
  )
{
  LPDDR_CS_ODT_ENCODING  EncodeVal;

  switch (OdtValue) {
    case 240:
      EncodeVal = OdtCs240;
      break;

    case 120:
      EncodeVal = OdtCs120;
      break;

    case 80:
      EncodeVal = OdtCs80;
      break;

    default:
      EncodeVal = -1;
      break;
  }

  return EncodeVal;
}

/**
  This function searches the list of valid ODT encodings for LPDDR5 to find the closest matching
  ODT.  The SOC ODT and the SOC_ODT programming to the LPDDR5 MR must match or unexpected behavior
  may occur.  It will return this value to the caller.

  @param[in]  MrcData - Pointer to the global MRC data
  @param[in]  LpRzqValues - Pointer to Rzq Value array for this LPDDR type
  @param[in]  LpRzqNumValues - Number of values in the Rzq Value array for this LPDDR type
  @param[in]  PuCalSocOdtValidLp - Pointer to PuCal SocOdt Valid array for this LPDDR type
  @param[in]  SocOdt  - The ODT requested by the system

  @retval - Closest matching ODT value.
**/
UINT16
MrcCheckSocOdtLpddr (
  IN  MrcParameters *const  MrcData,
  IN  const UINT16         *LpRzqValues,
  IN  UINT32                LpRzqNumValues,
  IN  const BOOLEAN        *PuCalSocOdtValid,
  IN  UINT16                SocOdt
  )
{
  UINT32  Index;
  UINT16  Distance1;
  UINT16  Distance2;
  UINT16  SocOdtRet;
  UINT16  LastValidValue;

  SocOdtRet = 0;
  LastValidValue = 0xFFFF;

  // Determine the SOC ODT Encoding that matches closest to the real RxOdt value.
  for (Index = 0; Index < LpRzqNumValues; Index++) {
    if (PuCalSocOdtValid[Index] == FALSE) {
      continue;
    }
    if (SocOdt == LpRzqValues[Index]) {
      SocOdtRet = SocOdt;
      break;
    }
    // No exact match, find the closest match.
    // Array is ordered from Largest to smallest.
    // If we're Index == 0, only check if the requested value is greater
    if (Index == 0) {
      if (SocOdt > LpRzqValues[Index]) {
        SocOdtRet = LpRzqValues[Index];
        break;
      }
    } else if (Index == (LpRzqNumValues - 1)) {
      // End of the list, return the last element.
      SocOdtRet = LpRzqValues[Index];
      break;
    } else {
      // See if it falls between this index and the previous.
      if ((SocOdt < LastValidValue) && (SocOdt > LpRzqValues[Index])) {
        // Determine which termination is closest to the SOC and select that
        Distance1 = LastValidValue - SocOdt;
        Distance2 = SocOdt - LpRzqValues[Index];
        // Select the one with the smallest distance
        SocOdtRet = (Distance1 < Distance2) ? LastValidValue : LpRzqValues[Index];
        break;
      }
    }
    LastValidValue = LpRzqValues[Index];
  }
  if (SocOdtRet == 0) {
    SocOdtRet = LastValidValue;
  }

  return SocOdtRet;
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
  This function sets the memory frequency.

  @param[in] MrcData           - Include all MRC global data.
  @param[in] LpJedecFreqSwitch - Boolean indicator that the current frequency update is for the LPDDR frequency switch flow.
  @param[in] DebugPrint        - Enable/disable debug printing

  @retval mrcSuccess on success, mrcFrequencyError on error.
**/
MrcStatus
McFrequencySet (
  IN MrcParameters* const MrcData,
  IN BOOLEAN              LpJedecFreqSwitch,
  IN BOOLEAN              DebugPrint
)
{
  MrcDebug*           Debug;
  MrcDebugMsgLevel    DebugLevel;
  MrcInput*           Inputs;
  MrcOutput*          Outputs;
  MrcClockRatio       Ratio;
  MrcStatus           Status;
  UINT32              MemoryClock;
  UINT16              BaseClock;
  BOOLEAN             GearMode;
  INT64               InitCompleteOvdValSave;
  MRC_EXT_INPUTS_TYPE *ExtInputs;

  Inputs = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  ExtInputs = Inputs->ExtInputs.Ptr;
  Debug = &Outputs->Debug;
  DebugLevel = DebugPrint ? MSG_LEVEL_NOTE : MSG_LEVEL_NONE;
  Status = mrcSuccess;
  GearMode = Outputs->GearMode;

  InitCompleteOverride (MrcData, LpJedecFreqSwitch, TRUE, &InitCompleteOvdValSave);

  MrcWait (MrcData, MRC_TIMER_1US);

  // PMA Initialization

  // Set QCLK Ratio
  Outputs->Ratio = MrcFrequencyToRatio (MrcData, Outputs->Frequency);
  // Only even ratios are supported in Gear2, Only ratios divisable by 4 are supported in Gear4
  Outputs->Ratio -= GearMode ? (Outputs->Ratio % 4) : (Outputs->Ratio % 2);

  if (Outputs->InWorkPointLock) {
    //Send PM0 to Lock PLL
    Status = MrcSendPmMessage (MrcData, PM_MESSAGE_0, PM_MSG_MC);
  } else {
    //Send PM13
    Status = MrcSendPmMessage (MrcData, PM_MESSAGE_13, PM_MSG_PHY);
  }

  if ((ExtInputs->SimicsFlag == 1) || Outputs->InWorkPointLock)  {
    Outputs->Frequency = MrcGetCurrentMemoryFrequency (MrcData, &MemoryClock, &Ratio);
  } else {
    Outputs->Frequency = MrcGetPhyCurrentMemoryFrequency (MrcData, &MemoryClock, &Ratio);
  }

  Outputs->MemoryClock = MemoryClock;

  MRC_DEBUG_MSG (
    Debug,
    DebugLevel,
    "Requested/actual ratio %u/%u, Frequency=%u, GearMode=%u",
    Outputs->Ratio,
    Ratio,
    Outputs->Frequency,
    GearMode
  );

  MRC_DEBUG_MSG (
    Debug,
    DebugLevel,
    ", RefClk=%uMHz, tCK=%ufs\n",
    33,
    MemoryClock
  );

  if (Ratio != Outputs->Ratio) {
    if (Ratio == 0) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "ERROR: MC PLL did not lock.\n");
    }
    return mrcFrequencyError;
  }

  Outputs->tCKps = (UINT16)(Outputs->MemoryClock / 1000);     // Memory clock period in pS (external bus clock)
  Outputs->Wckps = Outputs->tCKps / 4;                         // Write Clock period in pS; 4:1 mode.
  // Determine the base clock for UI/QCLK/DCLK
  // | DDR Type | Clock |
  // |==========|=======|
  // |  LPDDR5  | WCK   |
  // |    DDR5  | tCK   |
  // |==========|=======|
  if (Outputs->IsLpddr5) {
    BaseClock = Outputs->Wckps;
  } else {
    BaseClock = Outputs->tCKps;
  }
  Outputs->UIps = BaseClock / 2;                         // Data Unit Interval period in pS (half of external bus clock)
  Outputs->Qclkps = BaseClock * (GearMode ? 2 : 1);  // QCLK period in pS, this is internal MC/DDRIO clock which is impacted by Gear2/4
  Outputs->Dclkps = BaseClock * (GearMode ? 4 : 2);  // DCLK period in pS, this is internal MC/DDRIO clock which is impacted by Gear2/4
  MRC_DEBUG_MSG (
    Debug,
    DebugLevel,
    "tCKps=%u, wCKps=%u UIps=%u, Dclkps=%u, Qclkps=%u\n",
    Outputs->tCKps, Outputs->Wckps, Outputs->UIps, Outputs->Dclkps, Outputs->Qclkps
  );

  InitCompleteOverride (MrcData, LpJedecFreqSwitch, FALSE, &InitCompleteOvdValSave);

  return Status;
}

/**
  Read LPDDR information from MR5 and MR8 and print to the debug log.
  Also update the Manufacturer's ID in the SPD table, for BIOS Setup and SMBIOS table.

  @param[in] MrcData - include all the MRC general data.

  @retval none
**/
void
ShowLpddrInfo (
  IN  MrcParameters *const MrcData
  )
{
  MrcInput        *Inputs;
  MrcDebug        *Debug;
  MrcOutput       *Outputs;
  MrcSpd          *SpdIn;
  UINT32          Controller;
  UINT32          Channel;
  UINT32          Rank;
  UINT8           MrrResult[MRC_MRR_ARRAY_SIZE];
  UINT32          MrAddr;
  UINT32          Device;
  UINT32          DeviceMax;
  UINT32          Index;
  BOOLEAN         VendorFound;
  UINT16          JedecId;
#ifdef MRC_DEBUG_PRINT
  UINT8           MrrResult2[MRC_MRR_ARRAY_SIZE];
  MrcDimmOut      *DimmOut;
  LPDDR5_MODE_REGISTER_8_TYPE  Mr8;
  static const UINT8 DensityMap[] = { 2, 3, 4, 6, 8, 12, 16, 24, 32 };
#endif

  Inputs      = &MrcData->Inputs;
  Outputs     = &MrcData->Outputs;
  Debug       = &Outputs->Debug;

  VendorFound  = FALSE;
  Index        = 0;

  if (Inputs->BootMode != bmCold) {
    // Full deswizzle table is not present on non-cold flows, so cannot parse MR read.
    return;
  }

  // LPDDR: Read MR5 and MR8
  // DDR5:  Read MR0 and MR1
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
          continue;
        }

        if (Outputs->IsLpddr) {
          DeviceMax = (Outputs->LpByteMode) ? 2 : 1;
          // MR5 - Manufacturer ID
          MrAddr = mrMR5;
          MrcIssueMrr (MrcData, Controller, Channel, Rank, MrAddr, MrrResult);
          for (Device = 0; Device < DeviceMax; Device++) {
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\tDevice[%u]= 0x%02X", Device, MrrResult[Device]);
            VendorFound = FALSE;
            for (Index = 0; Index < sizeof (DramVendorList) / sizeof (DramVendorList[0]); Index++) {
              if (DramVendorList[Index].VendorId == MrrResult[Device]) {
                MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %s\n", DramVendorList[Index].VendorName);
                VendorFound = TRUE;
                break;
              }
            }
            if (!VendorFound) {
              MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " Unknown\n");
            }
          }

          if (VendorFound) {
            // Patch SPD data with vendor ID code.
            // This is consumed by BIOS Setup and SMBIOS Type 17 table creation code.
            // If SAGV enabled, only do this on the last point.
            if ((!MrcIsSaGvEnabled (MrcData)) || (Outputs->SaGvPoint == Outputs->SaGvLast)) {
              JedecId = DramVendorList[Index].JedecId;
              SpdIn = &Inputs->Controller[Controller].Channel[Channel].Dimm[dDIMM0].Spd.Data;
              SpdIn->Lpddr.ManufactureInfo.ModuleId.IdCode.Data = JedecId;
              SpdIn->Lpddr.ManufactureInfo.DramIdCode.Data      = JedecId;
            }
          }

#ifdef MRC_DEBUG_PRINT
          // MR8 - I/O Width, Density, Type
          MrAddr = mrMR8;
          MrcIssueMrr (MrcData, Controller, Channel, Rank, MrAddr, MrrResult);
          for (Device = 0; Device < DeviceMax; Device++) {
            Mr8.Data8 = MrrResult[Device];
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\tDevice[%u]= 0x%02X - LP5%c %s %uGb\n", Device, MrrResult[Device],
              (Mr8.Bits.Type == 1) ? 'X' : ' ',
              (Mr8.Bits.IoWidth == 1) ? "x8" : "x16",
              (Mr8.Bits.Density < ARRAY_COUNT (DensityMap)) ? DensityMap[Mr8.Bits.Density] : 0
              );
          }
#endif // #ifdef MRC_DEBUG_PRINT
        } else { // DDR5
#ifdef MRC_DEBUG_PRINT
          DimmOut = &Outputs->Controller[Controller].Channel[Channel].Dimm[RANK_TO_DIMM_NUMBER (Rank)];
          // Channel Width (ddr5 = 32 Bytes) / SdramWidth (x8 or x16)
          DeviceMax = DimmOut->PrimaryBusWidth / DimmOut->SdramWidth;
          MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR0, MrrResult);
          MrcIssueMrr (MrcData, Controller, Channel, Rank, mrMR1, MrrResult2);
          for (Device = 0; Device < DeviceMax; Device++) {
            if (DimmOut->SdramWidth == 16) {
              Index = Device * 2;
            } else {
              Index = Device;
            }
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\tDevice[%u]: MR0=0x%02X MR1=0x%02X\n", Device, MrrResult[Index], MrrResult2[Index]);
          }
#endif // #ifdef MRC_DEBUG_PRINT
        }
      } // for Rank
    } // for Channel
  } // for Controller

  MrcWait (MrcData, 100 * MRC_TIMER_1NS);
}
