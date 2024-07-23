/** @file
  This file implements an interface between the MRC and the different
  versions of CPGC.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#include "MrcCpgcOffsets.h"
#include "MrcCommon.h"
#include "MrcCpgcApi.h"
#include "Cpgc20.h"
#include "Cpgc20TestCtl.h"
#include "Cpgc20Patterns.h"
#include "MrcLpddr5.h"

///
/// External Function Definitions
///

/**
  This function activates and initializes the CPGC Engine.
  The McChBitMask and CpgcChAssign variables will be initialized
  based on the populated channels.

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess
**/
MrcStatus
Cpgc20Init (
  IN  MrcParameters *const  MrcData
)
{
  MrcOutput         *Outputs;
  MrcControllerOut  *ControllerOut;
  MrcChannelOut     *ChannelOut;
  UINT32            ChannelLoopIncrement;
  UINT32            Channel;
  UINT32            IpChannel;
  UINT32            Controller;
  UINT32            Offset;
  UINT8             ChannelMask;
  UINT32            MaskShift;
  BOOLEAN           IsLpddr;
  BOOLEAN           IsDdr5;
  MC0_REQ0_CR_CPGC_SEQ_CFG_A_STRUCT   CpgcSeqCfgA;

  Outputs              = &MrcData->Outputs;
  IsLpddr              = Outputs->IsLpddr;
  IsDdr5               = Outputs->IsDdr5;
  ChannelLoopIncrement = (IsLpddr) ? 2 : 1;

  // Assign channel to its serial number
  // Activate CPGC Engines on populated channels and subchannels.
  CpgcSeqCfgA.Data = 0;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (!MrcControllerExist (MrcData, Controller)) {
      continue;
    }

    ControllerOut = &Outputs->Controller[Controller];
    CpgcSeqCfgA.Data = 0;
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel += ChannelLoopIncrement) {
      if (!MrcChannelExist (MrcData, Controller, Channel)) {
        continue;
      }
      ChannelMask = (IsLpddr) ? ((ControllerOut->ValidChBitMask >> Channel) & 0x3) : 0x1;
      MaskShift   = (IsDdr5) ? (Channel * Outputs->MaxChannels) : Channel;
      ChannelOut  = &ControllerOut->Channel[Channel];
      IpChannel   = LP_IP_CH (IsLpddr, Channel);

      CpgcSeqCfgA.Bits.CHANNEL_ASSIGN      = (ChannelMask << MaskShift);
      ChannelOut->CpgcChAssign             = (UINT8) (IsDdr5 ? (1 << Channel) : CpgcSeqCfgA.Bits.CHANNEL_ASSIGN);
      CpgcSeqCfgA.Bits.INITIALIZATION_MODE = CPGC20_ACTIVE_MODE;
      CpgcSeqCfgA.Bits.GLOBAL_START_BIND = MrcData->Save.Data.CpgcGlobalStart;
      Offset = MrcGetCpgcSeqCfgOffset (Controller, Channel / ChannelLoopIncrement);
      MrcWriteCR (MrcData, Offset, CpgcSeqCfgA.Data);

      if (IsLpddr) {
        Cpgc20GetSetBankSequence (MrcData, Controller, Channel, B2BL2p, MAX_CPGC_B2B_BANKS, MRC_SET);
        Cpgc20LpBankAddrSwizzleUpdate (MrcData, Controller, Channel);
      } else {
        Cpgc20GetSetBankSequence (MrcData, Controller, Channel, BurstIdleBurstDdr5G4L2p , MAX_CPGC_RECEN_BG_BANKS, MRC_SET);
      }

      Cpgc20SetRequestDataSize (MrcData, Controller, IpChannel, 1);
    } // Channel
  } // Controller

  return mrcSuccess;
}

/**
  This function programs the Error Mask for Cacheline and UI comparisons.
  A value of 1 enables error checking.

  @param[in]  MrcData       - Pointer to global MRC data.
  @param[in]  McChBitMask   - Controller / Channel Mask to program
  @param[in]  CachelineMask - Bit mask of cachelines to enable.
  @param[in]  ChunkMask     - Bit mask of chunks to enable.

  @retval MrcStatus - mrcSuccess
**/
MrcStatus
MrcSetChunkAndClErrMsk (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 McChBitMask,
  IN  UINT32                CachelineMask,
  IN  UINT32                ChunkMask
  )
{
  Cpgc20SetChunkClErrMsk (MrcData, McChBitMask, CachelineMask, ChunkMask);

  return mrcSuccess;
}

/**
  This function programs the Error Mask for Data and ECC bit lanes.
  A value of 1 enables error checking for that bit.

  @param[in]  MrcData       - Pointer to global MRC data.
  @param[in]  McChBitMask   - Controller / Channel Mask to program
  @param[in]  DataMask      - Bit mask of Data bits to check.
  @param[in]  EccMask       - Bit mask of ECC bits to check.

  @retval MrcStatus - mrcSuccess.
**/
MrcStatus
MrcSetDataAndEccErrMsk (
  IN  MrcParameters *const  MrcData,
  IN  UINT8                 McChBitMask,
  IN  UINT64                DataMask,
  IN  UINT8                 EccMask
  )
{
  MrcStatus     Status;
  UINT64_STRUCT SubChErrMsk;
  UINT64        DataValue;
  UINT8         EccValue;

  Status = mrcSuccess;
  // Using bitwise complement for Masks as the register fields are enabled when 0 and disabled when 1
  DataValue = ~DataMask;
  EccValue = ~EccMask;

  // We set the entire x64 channel.
  SubChErrMsk.Data = DataValue;
  Cpgc20SetDataErrMsk (MrcData, McChBitMask, 0x1, SubChErrMsk.Data32.Low);
  Cpgc20SetDataErrMsk (MrcData, McChBitMask, 0x2, SubChErrMsk.Data32.High);
  if (MrcData->Outputs.EccSupport) {
    Cpgc20SetEccErrMsk (MrcData, EccValue);
  }

  return Status;
}

/**
  This function programs the test loop count.

  @param[in]  MrcData      - Pointer to MRC global data.
  @param[in]  McChBitMask  - Controller / Channel Mask to program
  @param[in]  LoopCount    - Number of sequence iterations. 0 means infinite test.

  @retval MrcStatus - mrcSuccess.
**/
MrcStatus
MrcSetLoopcount (
  IN MrcParameters *const  MrcData,
  IN UINT8                 McChBitMask,
  IN UINT32                LoopCount
  )
{
  Cpgc20SetLoopCount (MrcData, McChBitMask, &LoopCount);

  return mrcSuccess;
}

/**
  This function programs the test engine stop condition.

  @param[in]  MrcData         - Pointer to MRC global data.
  @param[in]  StopType        - MRC_TEST_STOP_TYPE: NSOE, NTHSOE, ABGSOE, ALSOE.
  @param[in]  NumberOfErrors  - Number of errors required to stop the test when StopType is NTHSOE.

  @retval MrcStatus - mrcSuccess.
**/
MrcStatus
MrcSetupTestErrCtl (
  IN  MrcParameters       *const  MrcData,
  IN  MRC_TEST_STOP_TYPE          StopType,
  IN  UINT32                      NumberOfErrors
  )
{
  MrcStatus Status;

  Status = mrcSuccess;
  MrcData->Outputs.ReutStopType = StopType;

  Cpgc20SetupTestErrCtl (MrcData, StopType, NumberOfErrors);

  return Status;
}

/**
  This function will Setup REUT Error Counters to count errors for specified type.

  @param[in]  MrcData           - Include all MRC global data.
  @param[in]  CounterPointer    - Specifies in register which Counter to setup. Valid for ErrControl values:
                                  ErrCounterCtlPerLane, ErrCounterCtlPerByte, ErrCounterCtlPerNibble, or ErrCounterCtlPerUI.
  @param[in]  ErrControlSel     - Specifies which type of error counter read will be executed.
  @param[in]  CounterUI         - Specifies which UI will be considered when counting errors.
                                    00 - All UI; 01 - Even UI; 10 - Odd UI; 11 - Particular UI (COUNTER_CONTROL_SEL = ErrCounterCtlPerUI)

  @retval mrcWrongInputParameter if ErrControlSel is an incorrect value, otherwise mrcSuccess.
**/
MrcStatus
MrcSetupErrCounterCtl (
  IN  MrcParameters *const      MrcData,
  IN  UINT8                     CounterPointer,
  IN  MRC_ERR_COUNTER_CTL_TYPE  ErrControlSel,
  IN  UINT8                     CounterUI
  )
{
  MrcStatus Status;

  switch (ErrControlSel) {
    case ErrCounterCtlAllLanes:
      Status = Cpgc20SetupErrCounterCtl (MrcData, 0, ErrControlSel, 0, 0, 0);
      break;

    case ErrCounterCtlPerByte:
//  case ErrCounterCtlPerNibble:
    case ErrCounterCtlPerLane:
    case ErrCounterCtlPerUI:
      Status = Cpgc20SetupErrCounterCtl (MrcData, CounterPointer, ErrControlSel, CounterPointer, 1, CounterUI);
      break;

    default:
      Status = mrcWrongInputParameter;
      break;
  }

  return Status;
}

/**
  This function returns the Bit Group Error status results.

  @param[in]  MrcData     - Include all MRC global data.
  @param[in]  Controller  - Desired controller to get ErrStatus
  @param[in]  Channel     - Desired channel to get ErrStatus
  @param[out] Status      - Pointer to array where the lane error status values will be stored.

  @retval Nothing.
**/
void
MrcGetBitGroupErrStatus (
  IN  MrcParameters   *const  MrcData,
  IN  UINT32                  Controller,
  IN  UINT32                  Channel,
  OUT UINT8                   *Status
  )
{
  Cpgc20GetBitGroupErrStatus (MrcData, Controller, Channel, Status);
}

/**
  This function returns the Error status results of the specified Error Type.

  @param[in]  MrcData     - Include all MRC global data.
  @param[in]  Controller  - Desired Controller.
  @param[in]  Channel     - Desired Channel.
  @param[in]  Param       - Specifies which type of error status read will be executed.
  @param[out] Buffer      - Pointer to buffer which register values will be read into.
                              Error status bits will be returned starting with bit zero. Logical shifting will not be handled by this function.

  @retval Returns mrcWrongInputParameter if Param value is not supported by this function, otherwise mrcSuccess.
**/
MrcStatus
MrcGetMiscErrStatus (
  IN  MrcParameters   *const  MrcData,
  IN  UINT32                  Controller,
  IN  UINT32                  Channel,
  IN  MRC_ERR_STATUS_TYPE     Param,
  OUT UINT64          *const  Buffer
  )
{
  MrcStatus Status;

  switch (Param) {
    case ByteGroupErrStatus:
    case EccLaneErrStatus:
    case ChunkErrStatus:
    case RankErrStatus:
    case NthErrStatus:
    case NthErrOverflow:
    case AlertErrStatus:
    case WdbRdChunkNumStatus:
      Status = Cpgc20GetErrEccChunkRankByteStatus (MrcData, Controller, Channel, Param, Buffer);
      break;

    default:
      Status = mrcWrongInputParameter;
      break;
  }

  return Status;
}

/**
  This function sets or returns the Logical-to-Physical mapping of Banks.  The index to the array
  is the logical address, and the value at that index is the physical address.  This function
  operates on a linear definition of Banks, even though there may be a hierarchy as BankGroup->Bank.
  For system with X Bank Groups and Y Banks per group, the Banks are indexed in the array as:
  (X * Y + Z) where X is the bank group, Y is the total number of banks per group, and Z is the
  target bank belonging to bank group X.

  Example:
    Bank Group 3, Bank 5, 8 Banks per Bank Group -> Index position (3 * 8) + 5 == 29.

  The function will act upon the array bounded by the param Length.
  The Function always starts at Bank 0.

  @param[in]      MrcData     - Pointer to MRC global data.
  @param[in]      Controller  - Desired Controller.
  @param[in]      Channel     - Desired Channel.
  @param[in,out]  L2pBankList - Array of logical-to-physical Bank Mapping.
  @param[in]      Length      - Array length of L2pBankList buffer.
  @param[in]      IsGet       - Boolean; if TRUE, request is a Get, otherwise Set.

  @retval MrcStatus.
**/
MrcStatus
Cpgc20GetSetBankSequence (
  IN  MrcParameters *const      MrcData,
  IN  UINT32                    Controller,
  IN  UINT32                    Channel,
  IN  MRC_BG_BANK_PAIR  *const  L2pBankList,
  IN  UINT32                    Length,
  IN  BOOLEAN                   IsGet
  )
{
  MrcStatus Status;
  UINT8     BankList[CPGC20_MAX_BANKS_PER_CHANNEL];
  UINT8     Index;
  UINT8     BgOffset;
  UINT8     BankMask;

  Status  = mrcSuccess;

  if (Length > CPGC20_MAX_BANKS_PER_CHANNEL) {
    Status = mrcWrongInputParameter;
  } else {
    BgOffset = CPGC20_BANK_GROUP_FIELD_OFFSET_DDR5;
    BankMask = (1 << BgOffset) - 1;
    if (!IsGet) {
      for (Index = 0; Index < Length; Index++) {
        BankList[Index] = (L2pBankList[Index].BankGroup << BgOffset) | (L2pBankList[Index].Bank);
      }
    }
    Cpgc20GetSetBankMap (MrcData, Controller, Channel, BankList, Length, IsGet);
    if (IsGet) {
      for (Index = 0; Index < Length; Index++) {
        L2pBankList[Index].BankGroup = BankList[Index] >> BgOffset;
        L2pBankList[Index].Bank = BankList[Index] & (BankMask);
      }
    }
  }

  return Status;
}


/**
  This function returns the Error Counter and Raster Repo status.

  @param[in]  MrcData       - Include all MRC global data.
  @param[in]  Controller    - Desired Controller.
  @param[in]  Channel       - Desired Channel.
  @param[in]  Byte          - Desired Byte.
  @param[in]  EvenOddUIErr  - Selects which Raster Repo register to be read to get Even/Odd UI error status for all lanes.
  @param[in]  RasterRepoStatus        - Pointer to buffer where counter status will be held.

  @retval RasterRepoStatus.
**/
void
MrcGetRasterRepoStatus (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  UINT32                Channel,
  IN  UINT8                 Byte,
  IN  UINT8                 EvenOddUI,
  OUT UINT64         *const RasterRepoStatus
)
{
  const MRC_FUNCTION  *MrcCall;
  MrcInput            *Inputs;
  UINT32              Offset;
  UINT64              Mask;
  MC0_CH0_CR_CPGC2_RASTER_REPO_CONTENT_ECC_0_STRUCT  Cpgc2RasterRepoContentEcc;

  Inputs  = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;
  Mask    = 0x00000000000000FF;

  if (EvenOddUI == 2) {
    Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_CPGC2_RASTER_REPO_CONTENT_ECC_0_REG, MC1_CH0_CR_CPGC2_RASTER_REPO_CONTENT_ECC_0_REG, Controller, MC0_CH1_CR_CPGC2_RASTER_REPO_CONTENT_ECC_0_REG, Channel);
  } else {
    Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_CPGC2_RASTER_REPO_CONTENT_ECC_1_REG, MC1_CH0_CR_CPGC2_RASTER_REPO_CONTENT_ECC_1_REG, Controller, MC0_CH1_CR_CPGC2_RASTER_REPO_CONTENT_ECC_1_REG, Channel);
  }
  Cpgc2RasterRepoContentEcc.Data = MrcReadCR64 (MrcData, Offset);
  if (Byte < 4) { // lp5 and ddr5
    *RasterRepoStatus = Cpgc2RasterRepoContentEcc.Bits.CONTENTLow & ((UINT32) MrcCall->MrcLeftShift64 (Mask,(8 * Byte)));
  } else if (Byte == (MAX_SDRAM_IN_DIMM + 1)) {
#ifdef DDR5_SUPPORT
    *RasterRepoStatus = Cpgc2RasterRepoContentEcc.Data;
#endif
  }
}

/**
  This function will enable or disable CPGC engines on all channels.
  @todo: Move to GetSet infrastructure.

  @param[in] MrcData    - Global MRC data structure
  @param[in] ActiveMode - If true, enable CPGC engines. If false, set to idle mode.

  @retval - Nothing
**/
void
MrcSetCpgcInitMode (
  IN MrcParameters *const MrcData,
  IN BOOLEAN              ActiveMode
  )
{
  MrcOutput   *Outputs;
  UINT32      Offset;
  UINT32      Controller;
  UINT32      Channel;
  UINT32      IpChannel;
  BOOLEAN     IsLpddr;
  MC0_REQ0_CR_CPGC_SEQ_CFG_A_STRUCT CpgcSeqCfgA;

  Outputs = &MrcData->Outputs;
  IsLpddr = Outputs->IsLpddr;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
      if ((MrcChannelExist (MrcData, Controller, Channel)) && (!(IS_MC_SUB_CH (IsLpddr, Channel)))) {
        IpChannel = LP_IP_CH (IsLpddr, Channel);
        Offset = MrcGetCpgcSeqCfgOffset (Controller, IpChannel);
        CpgcSeqCfgA.Data =  MrcReadCR (MrcData, Offset);
        CpgcSeqCfgA.Bits.INITIALIZATION_MODE = ActiveMode ? CPGC20_ACTIVE_MODE : CPGC20_IDLE_MODE;
        MrcWriteCR (MrcData, Offset, CpgcSeqCfgA.Data);
      }
    }
  }
}

/**
  This function clears the raster repo to get a new error
  status on the next RunIOTest.

  @param[in]  MrcData     - Pointer to MRC global data.
  @param[in]  Controller  - Controller to work on
  @param[in]  Channel     - Channel to work on

  @retval Nothing
**/
VOID
MrcCpgcClearRasterRepo (
  IN MrcParameters *const  MrcData,
  IN UINT32                Controller,
  IN UINT32                Channel
  )
{
  UINT32  Offset;
  MC0_CH0_CR_CPGC2_RASTER_REPO_CONFIG_STRUCT  Cpgc2RasterRepoCfg;

  Offset = OFFSET_CALC_MC_CH (
            MC0_CH0_CR_CPGC2_RASTER_REPO_CONFIG_REG,
            MC1_CH0_CR_CPGC2_RASTER_REPO_CONFIG_REG,
            Controller,
            MC0_CH1_CR_CPGC2_RASTER_REPO_CONFIG_REG,
            Channel);
  Cpgc2RasterRepoCfg.Data = MrcReadCR (MrcData, Offset);
  Cpgc2RasterRepoCfg.Bits.RasterRepoClear = 1; // Clear raster repo to get new error status before RunIOTest
  MrcWriteCR (MrcData, Offset, Cpgc2RasterRepoCfg.Data);
}

/**
  This function immediately clear all error registers and error status
  except for the Raster Repository.

  @param[in] MrcData     - The MRC global data.
  @param[in] McChBitMask - Controllers and channels to work on.

  @retval none
**/
void
Cpgc20ClearErrors (
  IN MrcParameters *const MrcData,
  IN  UINT8               McChBitMask
  )
{
  MC0_REQ0_CR_CPGC_SEQ_CTL_STRUCT  CpgcSeqCtl;

  CpgcSeqCtl.Data = 0;
  CpgcSeqCtl.Bits.CLEAR_ERRORS = 1;
  Cpgc20ControlRegWrite (MrcData, McChBitMask, CpgcSeqCtl);
}

/**
  This function configures initial CPGC credits before running any CPGC traffic.

  @param[in]  MrcData       - Pointer to MRC global data.
  @param[in]  Controller    - Controller to work on
  @param[in]  IsCpgcActive  - FALSE for Normal mode, TRUE for CPGC mode

  @retval Nothing.
**/
VOID
MrcCpgcConfigInitialCredits (
  IN  MrcParameters *const  MrcData,
  IN  UINT32                Controller,
  IN  BOOLEAN               IsCpgcActive
  )
{
  UINT32  Offset;
  MC0_SHARED_CR_CPGC2_CREDIT_CFG_STRUCT  Cpgc20Credits;

  Offset = OFFSET_CALC_CH (MC0_SHARED_CR_CPGC2_CREDIT_CFG_REG, MC1_SHARED_CR_CPGC2_CREDIT_CFG_REG, Controller);
  Cpgc20Credits.Data = MrcReadCR (MrcData, Offset);

  if (IsCpgcActive) {
    Cpgc20Credits.Bits.RD_CPL_CREDITS_INIT = RDCPL_CREDITS_DEFAULT;
    Cpgc20Credits.Bits.CREDITS_CONFIG_DONE = 1;
  } else {
    Cpgc20Credits.Bits.CREDITS_CONFIG_DONE = 0;
  }

  MrcWriteCR (MrcData, Offset, Cpgc20Credits.Data);
}

/**
  This function read the initial CPGC credits if IsSave is set and program new value.

  @param[in]      MrcData       - Pointer to global MRC data
  @param[in]      Controller    - Controller to work on
  @param[in]      IsSave        - Set to save initial CPGC credits
  @param[in,out]  RdCplCredits  - CPGC credits to program

  @retval MrcStatus - mrcFail if ISM is not in IDLE state
                    - mrcSuccess otherwise.
**/
MrcStatus
MrcCpgcModifyReadCredits (
  IN     MrcParameters* const  MrcData,
  IN     UINT32                Controller,
  IN     BOOLEAN               IsSave,
  IN OUT UINT32                *RdCplCredits
  )
{
  const MRC_FUNCTION  *MrcCall;
  UINT64              Timeout;
  BOOLEAN             Busy;
  MrcOutput           *Outputs;
  MrcDebug            *Debug;
  UINT32              Offset;
  UINT32              SetRdCplCredits;
  MC0_SHARED_CR_CPGC2_CREDIT_CFG_STRUCT  Cpgc20Credits;

  Outputs           = &MrcData->Outputs;
  Debug             = &Outputs->Debug;
  SetRdCplCredits   = *RdCplCredits;
  MrcCall           = MrcData->Inputs.Call.Func;
  Busy              = TRUE;
  Timeout           = MrcCall->MrcGetCpuTime () + MRC_WAIT_TIMEOUT;   // 10 seconds timeout

  // Make sure ISM is in IDLE before clearing CREDITS_CONFIG_DONE
  Offset = OFFSET_CALC_CH (MC0_SHARED_CR_CPGC2_CREDIT_CFG_REG, MC1_SHARED_CR_CPGC2_CREDIT_CFG_REG, Controller);
  do {
    Cpgc20Credits.Data = MrcReadCR (MrcData, Offset);
    Busy = (Cpgc20Credits.Bits.ISM_IDLE == 0);
  } while (Busy && (MrcCall->MrcGetCpuTime () < Timeout));
  if (Busy) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "%s: Unable to write RD_CPL_CREDITS_INIT due to ISM is not in IDLE state\n", gWarnString);
    return mrcFail;
  }

  if (IsSave) {
    *RdCplCredits = Cpgc20Credits.Bits.RD_CPL_CREDITS_INIT;
  }

  // Clear CREDITS_CONFIG_DONE before changing RD_CPL_CREDITS_INIT
  Cpgc20Credits.Bits.CREDITS_CONFIG_DONE = 0;
  MrcWriteCR (MrcData, Offset, Cpgc20Credits.Data);
  Cpgc20Credits.Bits.RD_CPL_CREDITS_INIT = SetRdCplCredits;
  Cpgc20Credits.Bits.CREDITS_CONFIG_DONE = 1;
  MrcWriteCR (MrcData, Offset, Cpgc20Credits.Data);

  return mrcSuccess;
}

/**
  This function return the CPGC ErrStatus Mask based on byte

  ECC bytes only has 4 bit, so return only lower nibble, else return 0xFF

  @param[in]  Byte - Byte to get ErrStatusMask
**/
UINT8
MrcCpgcErrStatusMask (
  UINT32 Byte
  )
{
  if (Byte == MRC_DDR5_ECC_BYTE) {
    return 0x0F;
  } else {
    return 0xFF;
  }
}

/**
  This function return the BitMask based on byte

  ECC byte only has 4 bit, so return bitmask of lower nibble

  @param[in]  Byte - Byte to get BitMask
  @param[in]  Bit  - Bit to get BitMask
**/
UINT8
MrcGetCpgcBitMask (
  UINT32 Byte,
  UINT32 Bit
  )
{
  UINT8    BitMask;

  if (Byte < MRC_DDR5_ECC_BYTE) {
    BitMask = MRC_BIT0 << Bit;
  } else {
    BitMask = MRC_BIT0 << (Bit % 4);
  }

  return BitMask;
}

/**
  This function acts as wrapper function and calls UpdateAddressForWholeRankExtended().
  @param[in]  MrcData           - Pointer to MRC global data.
  @param[out] CPGCAddressArray  - 2D Array of Structure that stores address related settings per channel.
  @param[in]  AddrDirection     - CPGC Address Direction
  @param[in]  McChMask          - Channel bit mask (can be different than Outputs->McChBitMask)
  @param[in]  Rank              - Rank to work on
**/
VOID
UpdateAddressForWholeRank (
    IN  MrcParameters *const            MrcData,
    OUT MRC_ADDRESS                     CPGCAddressArray[MAX_CONTROLLER][MAX_CHANNEL],
    IN  MRC_ADDRESS_INCREMENT_ORDER     AddrIncOrder,
    IN  UINT8                           McChMask,
    IN  UINT32                          Rank
  )
{
  UpdateAddressForWholeRankExtended (MrcData, CPGCAddressArray, AddrIncOrder, McChMask, Rank, NULL);
}

/**
  This function sets up ADDRESS_SIZE register per channel to the whole rank, Bank/Row/Col size is determined by DIMM.
  This is used in memory scrubbing and PPR.
  The register will be updated on all enabled CPGC engines.

  @param[in]  MrcData           - Pointer to MRC global data.
  @param[out] CPGCAddressArray  - 2D Array of Structure that stores address related settings per channel.
  @param[in]  AddrDirection     - CPGC Address Direction
  @param[in]  McChMask          - Channel bit mask (can be different than Outputs->McChBitMask)
  @param[in]  Rank              - Rank to work on
  @param[in]  CapNotPowerOf2    - Whether non-power of 2 capacity found per MC/CH

**/
VOID
UpdateAddressForWholeRankExtended (
  IN  MrcParameters *const            MrcData,
  OUT MRC_ADDRESS                     CPGCAddressArray[MAX_CONTROLLER][MAX_CHANNEL],
  IN  MRC_ADDRESS_INCREMENT_ORDER     AddrIncOrder,
  IN  UINT8                           McChMask,
  IN  UINT32                          Rank,
  IN  BOOLEAN                         CapNotPowerOf2[MAX_RANK_IN_CHANNEL][MAX_CONTROLLER][MAX_CHANNEL] OPTIONAL
  )
{
  MrcDebug          *Debug;
  MrcOutput         *Outputs;
  MrcControllerOut  *ControllerOut;
  MrcDimmOut        *DimmOut;
  UINT32            Controller;
  UINT32            Channel;
  UINT8             BankCount;
  UINT8             MaxChannel;
  UINT32            RowSizeBits;
  UINT32            ColSizeBits;
  BOOLEAN           IsLpddr;
  MRC_ADDRESS     *CpgcAddrPtr;

  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;
  MaxChannel = Outputs->MaxChannels;
  IsLpddr = Outputs->IsLpddr;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    ControllerOut = &Outputs->Controller[Controller];
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if ((MC_CH_MASK_CHECK (McChMask, Controller, Channel, MaxChannel) == 0) || (IS_MC_SUB_CH (IsLpddr, Channel))) {
        continue;
      }
      // get the Bank/Row/Col size from SPD for the whole rank
      DimmOut = &ControllerOut->Channel[Channel].Dimm[RANK_TO_DIMM_NUMBER (Rank)];
      BankCount = DimmOut->BankGroups * DimmOut->Banks;
      RowSizeBits = MrcLog2 (MrcData, DimmOut->RowSize) - 1;
      ColSizeBits = MrcLog2 (MrcData, DimmOut->ColumnSize) - 1;
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "UpdateAddressForWholeRank: Mc%d Ch%d R%d RowSizeBits = %d, ColSizeBits = %d, BankCount = %d\n",
        Controller, Channel, Rank, RowSizeBits, ColSizeBits, BankCount);

      // BL=16: column increment is 2^4 per CL --> 10 - 4 = 6
      // BL=32: column increment is 2^5 per CL --> 10 - 5 = 5
      ColSizeBits -= (IsLpddr ? 5 : 4);

      CpgcAddrPtr = &CPGCAddressArray[Controller][Channel];
      // Row and Col bit sizes in BlockSize and RegionSize registers are exponential. Bank and Rank are linear "+1".
      CpgcAddrPtr->AddrDirection = FAST_Y;
      CpgcAddrPtr->AddrIncOrder = AddrIncOrder;
      CpgcAddrPtr->BankSize = BankCount;
      CpgcAddrPtr->ColSizeBits = (UINT8)ColSizeBits;
      CpgcAddrPtr->ColStart = 0;
      CpgcAddrPtr->LastValidAddr = 0;
      if ((CapNotPowerOf2 != NULL) && CapNotPowerOf2[Rank][Controller][Channel]) {
        CpgcAddrPtr->RowSizeBits = (UINT8)(RowSizeBits - 2);
      } else {
        CpgcAddrPtr->RowSizeBits = (UINT8)RowSizeBits;
      }
      CpgcAddrPtr->RowStart = 0;
    }
  }
}
