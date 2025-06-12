/** @file
  Copies the memory related timing and configuration information into the
  Compatible BIOS data (BDAT) table.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#include "MrcBdatPrivate.h"
#include "MrcBdat.h"

#include "CMrcTypes.h"
#include "CMrcApi.h"
#include "MrcGeneral.h"  // for MrcIsSaGvEnabled()
#include "MrcSagv.h"
#include "BlueMrcSpdProcessingApi.h"  // for GetDimmCrc()
#include "CMcAddress.h"
#include "CPlatformData.h"  // for PLATFORM_DATA
#ifndef MRC_MINIBIOS_BUILD
#include "CMemoryInit.h"
#else
#include "ConfigBlock.h"
#include "MrcOemPlatform.h"
#endif

// Index of signals in the RankResultType0/ByteResultType/LaneResultType
#define RESULTS_RX_DQ_INDEX          0
#define RESULTS_TX_DQ_INDEX          1
#define RESULTS_RX_VREF_INDEX        2
#define RESULTS_TX_VREF_INDEX        3

// Index of signals in the RankResultType1
#define RESULTS_CMD_INDEX            0
#define RESULTS_CMD_VREF_INDEX       1

// Index of signals in the RankResultType3
#define RESULTS_RECEN_INDEX          0
#define RESULTS_WRLVL_INDEX          1

#define LOW_INDEX                    0
#define HIGH_INDEX                   1

#define IO_LEVEL_DDR_LEVEL           1

#define GetCrc16                     GetDimmCrc

// Fill Bdat function pointer
typedef VOID  (*MRC_FILL_BDAT_STRUCT_FUNC)  (MrcParameters *const MrcData, VOID *RmtDataBuffer, UINT32 RmtDataBufferSize, BOOLEAN MsgPrint);

/**
  Print out the RMT column data.

  @param[in]      MrcData       - Include all MRC global data.
  @param[in]      RmtRows       - The RMT column result.
  @param[in]      ElementNumber - The index of the results.
**/
VOID
PrintBdatResult (
  IN MrcParameters  *const MrcData,
  IN RMT_RESULT_COLUMNS *RmtRows,
  IN UINT32             ElementNumber
  )
{
  MrcDebug           *Debug;
  RMT_RESULT_COLUMNS *RmtRow;
  UINT8 i;
  UINT8 j;

  RmtRow = &(RmtRows[ElementNumber]);
  Debug  = &MrcData->Outputs.Debug;

  MRC_DEBUG_MSG (
    Debug,
    MSG_LEVEL_NOTE,
    "i: %d, Ch: %d Rank: %d Type: %d Lane: %d - [",
    ElementNumber,
    RmtRow->Header.Channel,
    RmtRow->Header.RankA,
    RmtRow->Header.ResultType,
    RmtRow->Header.Lane
    );

  for (i = 0; i < 4; i++) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "[");
    for (j = 0; j < 2; j++) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %d", RmtRow->Margin[i][j]);
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "]");
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "]\n");
}

/**
  Determines the number of rows for the RMT Results.

  @param[in] MrcData       - Include all MRC global data.

  @return  The number of RMT Result Rows
**/
UINT32
GetNumberRowsBdatResult (
  IN MrcParameters *const MrcData
  )
{
  const MrcOutput     *Outputs;
  const MrcChannelOut *ChannelOut;          // Pointer to the channel output portion of the MRC global data area.
  const MrcDimmOut    *DimmOut;             // Pointer to the DIMM output portion of the MRC global data area.
  UINT32 NumberRows;
  UINT8  BdatTestType;
  UINT8  Controller;
  UINT8  Channel;
  UINT8  Dimm;
  UINT8  Rank;
  UINT8  Byte;
  UINT8  DqCnt;

  NumberRows  = 0;
  Outputs     = &MrcData->Outputs;
  DqCnt       = (Outputs->IsLpddr) ? Outputs->MaxDqBits : (Outputs->EccSupport) ? Outputs->MaxDqBits : (Outputs->MaxDqBits - NUM_ECC_BITS);
  BdatTestType = MrcData->Inputs.BdatTestType;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (Outputs->Controller[Controller].Status == CONTROLLER_PRESENT) {
      for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
        if (Outputs->Controller[Controller].Channel[Channel].Status == CHANNEL_PRESENT) {
          ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
          for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
            DimmOut = &ChannelOut->Dimm[Dimm];
            if (DimmOut->Status == DIMM_PRESENT) {
              for (Rank = 0; (Rank < MAX_RANK_IN_DIMM) && (Rank < DimmOut->RankInDimm); Rank++) {
                for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
                  if (BdatTestType == Margin2DType) {
                    NumberRows += MAX_2D_EYE_OFFSETS * 2;
                  } else {
                    // per rank RankResultType0  - RxDqs, TxDq, RxVref and TxVref
                    // per rank RankResultType1  - Cmd, Cmd Vref
                    // per rank RankResultType1  - RecEN, Wrlvl
                    NumberRows += 3;
                    // per bit margin LaneResultType
                    if (BdatTestType == RankMarginToolPerBitType) {
                      NumberRows += DqCnt;
                    }
                  }
                } // Byte
              } // Rank
            } // if Dimm present
          } // Dimm
        } // if Channel present
      } // Channel
    } // if Controller present
  } // Controller

  return NumberRows;
}

/**
  Return the size of the BDAT Schema for the specified BDAT Test Type.

  @param[in] MrcData           - Include all MRC global data.

  @return  The size of the BDAT Schema.
**/
UINT32
GetBdatSchemaSize (
  IN MrcParameters *const MrcData
  )
{
  UINT32 BdatSchemaSize;

  if (MrcData->Inputs.BdatTestType == Margin2DType) {
    BdatSchemaSize = sizeof (RESULTS_DATA_HDR) + sizeof (MARGIN_2D_RESULT_METADATA) + (sizeof (MARGIN_2D_RESULT_COLUMNS) * GetNumberRowsBdatResult (MrcData));
  } else {
    BdatSchemaSize = sizeof (RESULTS_DATA_HDR) + sizeof (RMT_RESULT_METADATA) + (sizeof (RMT_RESULT_COLUMNS) * GetNumberRowsBdatResult (MrcData));
  }

  return BdatSchemaSize;
}

/**
  This function fills the RMT schema 6 which defined in the BDAT spec.
  The schema include meta data and columns data sections.

  @param[in, out] MrcData           - Include all MRC global data.
  @param[out]     RmtDataBuffer     - Pointer to data block to fill up the RMT structure.
  @param[in]      RmtDataBufferSize - Size of the RMT data block.
  @param[in]      MsgPrint          - Show debug prints
**/
VOID
FillRmtBdatStructure (
  IN OUT MrcParameters *const MrcData,
  OUT   VOID           *RmtDataBuffer,
  IN    UINT32          RmtDataBufferSize,
  IN    BOOLEAN         MsgPrint
  )
{
  const MrcInput      *Inputs;
  const MRC_FUNCTION  *MrcCall;
  const MrcChannelOut *ChannelOut;       // Pointer to the channel output portion of the MRC global data area.
  const MrcDimmOut    *DimmOut;          // Pointer to the DIMM output portion of the MRC global data area.
  MrcOutput           *Outputs;
  MrcDebug            *Debug;
  MrcDebugMsgLevel    DebugLevel;
  BASE_RMT_RESULT     *RmtResults;
  RMT_RESULT_COLUMNS  *RmtRow;
  INT32 MaxNumberRows;
  INT32 ResultElementCount;
  UINT8 Controller;
  UINT8 Byte;
  UINT8 Channel;
  UINT8 Dimm;
  UINT8 Rank;
  UINT8 RankInChannel;     // Rank number in a channel.
  UINT8 Bit;
  UINT8 Lane;
  UINT8 Gear;
  const MrcDqTimeMargin   *RxDqMargin;      // Pointer to the output portion of the MRC global data area.
  const MrcDqTimeMargin   *TxDqMargin;      // Pointer to the output portion of the MRC global data area.
  const MrcDqVrefMargin   *RxVrefDqMargin;  // Pointer to the output portion of the MRC global data area.
  const MrcDqVrefMargin   *TxVrefDqMargin;  // Pointer to the output portion of the MRC global data area.
  const MrcCommandMargin  *CmdMargin;
  const MrcRecvEnMargin   *RecvEnMargin;
  const MrcWrLevelMargin  *WrLvlMargin;
  const MRC_EXT_INPUTS_TYPE  *ExtInputs;
  MrcVersion              Version;

  Outputs = &MrcData->Outputs;
  Inputs  = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;
  Debug   = &Outputs->Debug;
  ExtInputs = Inputs->ExtInputs.Ptr;
  DebugLevel = MsgPrint ? MSG_LEVEL_NOTE : MSG_LEVEL_NONE;
  Gear = Outputs->GearMode ? 4 : 2,
  MrcVersionGet (MrcData, &Version);

  RmtResults = (BASE_RMT_RESULT*) RmtDataBuffer;
  MrcCall->MrcSetMem ((UINT8*) RmtDataBuffer, RmtDataBufferSize, 0);
  MrcCall->MrcCopyMem ((UINT8*) (&RmtResults->ResultsHeader.MdBlock.MetadataType), (UINT8*) (&gRmtResultMetadataGuid), sizeof (RmtResults->ResultsHeader.MdBlock.MetadataType));
  RmtResults->ResultsHeader.MdBlock.MetadataSize = sizeof (RMT_RESULT_METADATA);
  MrcCall->MrcCopyMem ((UINT8*) (&RmtResults->ResultsHeader.RsBlock.ResultType), (UINT8*) (&gRmtResultColumnsGuid), sizeof (RmtResults->ResultsHeader.RsBlock.ResultType));
  RmtResults->ResultsHeader.RsBlock.ResultElementSize = sizeof (RMT_RESULT_COLUMNS);
  MaxNumberRows = RmtDataBufferSize - sizeof (RESULTS_DATA_HDR) - sizeof (RMT_RESULT_METADATA);
  MaxNumberRows = MaxNumberRows / sizeof (RMT_RESULT_COLUMNS);

  RmtResults->ResultsHeader.RsBlock.ResultCapacity = MaxNumberRows;
  ResultElementCount = 0;
  RmtResults->ResultsHeader.RsBlock.ResultElementCount = ResultElementCount;
  RmtResults->Metadata.EnableCtlAllMargin       = 0;
  RmtResults->Metadata.SinglesBurstLength       = 128;  //  NumCL = 128;
  RmtResults->Metadata.SinglesLoopCount         = (ExtInputs->RMTLoopCount == 0) ? 17 : ExtInputs->RMTLoopCount;  //   DqLoopCount  = 17; CmdLoopCount = 17;
  RmtResults->Metadata.TurnaroundsBurstLength   = 0;
  RmtResults->Metadata.TurnaroundsLoopCount     = 0;
  RmtResults->Metadata.ScramblerOverrideMode    = DontTouchScrambler;
  RmtResults->Metadata.PiStepUnit[0]            = 0;
  RmtResults->Metadata.RxVrefStepUnit[0]        = 0;
  RmtResults->Metadata.TxVrefStepUnit[0][0]     = 0;
  RmtResults->Metadata.CmdVrefStepUnit[0][0]    = 0;
  RmtResults->Metadata.MajorVer                 = Version.Major;
  RmtResults->Metadata.MinorVer                 = Version.Minor;
  RmtResults->Metadata.RevVer                   = Version.Rev;
  RmtResults->Metadata.BuildVer                 = Version.Build;
  RmtResults->Metadata.Frequency                = Outputs->Frequency;
  RmtResults->Metadata.Gear                     = Gear;
  RmtRow = &(RmtResults->Rows[0]);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (Outputs->Controller[Controller].Status == CONTROLLER_PRESENT) {
      for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
        if (Outputs->Controller[Controller].Channel[Channel].Status == CHANNEL_PRESENT) {
          ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
          for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
            DimmOut = &ChannelOut->Dimm[Dimm];
            if (DimmOut->Status == DIMM_PRESENT) {
              for (Rank = 0; (Rank < MAX_RANK_IN_DIMM) && (Rank < DimmOut->RankInDimm); Rank++) {
                RankInChannel = (Dimm * MAX_RANK_IN_DIMM) + Rank;
                // The next line is telling Coverity to ignore the possible array overflow in RxDqMargin, etc. caused by relying on Outputs->SdramCount
                // coverity[overflow:SUPPRESS]
                for (Byte = 0; Byte < Outputs->SdramCount; Byte++) {
                  MRC_DEBUG_MSG (Debug, DebugLevel, "Ch: %u, D: %u R: %u B: %u \n", Channel, Dimm, Rank, Byte);
                  // Per rank RankResultType0  - RxDqs, TxDq, RxVref and TxVref
                  RmtRow[ResultElementCount].Header.Controller = Controller;
                  RmtRow[ResultElementCount].Header.Channel    = Channel;
                  RmtRow[ResultElementCount].Header.DimmA      = Dimm;
                  RmtRow[ResultElementCount].Header.RankA      = Rank;
                  RmtRow[ResultElementCount].Header.Lane       = Byte;
                  RmtRow[ResultElementCount].Header.IoLevel    = IO_LEVEL_DDR_LEVEL;
                  RmtRow[ResultElementCount].Header.ResultType = RankResultType0; //RxDqsdelay, TxDqDelay, RxVref, TxVref

                  RmtRow[ResultElementCount].Margin[RESULTS_RX_DQ_INDEX][LOW_INDEX]    = 0xFF;
                  RmtRow[ResultElementCount].Margin[RESULTS_RX_DQ_INDEX][HIGH_INDEX]   = 0xFF;
                  RmtRow[ResultElementCount].Margin[RESULTS_TX_DQ_INDEX][LOW_INDEX]    = 0xFF;
                  RmtRow[ResultElementCount].Margin[RESULTS_TX_DQ_INDEX][HIGH_INDEX]   = 0xFF;
                  RmtRow[ResultElementCount].Margin[RESULTS_RX_VREF_INDEX][LOW_INDEX]  = 0xFF;
                  RmtRow[ResultElementCount].Margin[RESULTS_RX_VREF_INDEX][HIGH_INDEX] = 0xFF;
                  RmtRow[ResultElementCount].Margin[RESULTS_TX_VREF_INDEX][LOW_INDEX]  = 0xFF;
                  RmtRow[ResultElementCount].Margin[RESULTS_TX_VREF_INDEX][HIGH_INDEX] = 0xFF;

                  // Rank margin results are the min value of all DQ lanes.
                  for (Bit = 0; Bit < MAX_BITS; Bit++) {
                    if (!MrcBitExist (Controller, Byte, Bit)) {
                      continue; // DDR5 ECC byte has only 4 bits DQ
                    }
                    RxDqMargin              = &ChannelOut->RxDqPb[RankInChannel][Byte][Bit];
                    TxDqMargin              = &ChannelOut->TxDqPb[RankInChannel][Byte][Bit];
                    RxVrefDqMargin          = &ChannelOut->RxDqVrefPb[RankInChannel][Byte][Bit];
                    TxVrefDqMargin          = &ChannelOut->TxDqVrefPb[RankInChannel][Byte][Bit];

                    if (RxDqMargin->Left < RmtRow[ResultElementCount].Margin[RESULTS_RX_DQ_INDEX][LOW_INDEX]) {
                      RmtRow[ResultElementCount].Margin[RESULTS_RX_DQ_INDEX][LOW_INDEX] = RxDqMargin->Left;
                    }
                    if (RxDqMargin->Right < RmtRow[ResultElementCount].Margin[RESULTS_RX_DQ_INDEX][HIGH_INDEX]) {
                      RmtRow[ResultElementCount].Margin[RESULTS_RX_DQ_INDEX][HIGH_INDEX] = RxDqMargin->Right;
                    }
                    if (TxDqMargin->Left < RmtRow[ResultElementCount].Margin[RESULTS_TX_DQ_INDEX][LOW_INDEX]) {
                      RmtRow[ResultElementCount].Margin[RESULTS_TX_DQ_INDEX][LOW_INDEX] = TxDqMargin->Left;
                    }
                    if (TxDqMargin->Right < RmtRow[ResultElementCount].Margin[RESULTS_TX_DQ_INDEX][HIGH_INDEX]) {
                      RmtRow[ResultElementCount].Margin[RESULTS_TX_DQ_INDEX][HIGH_INDEX] = TxDqMargin->Right;
                    }
                    if (RxVrefDqMargin->Low < RmtRow[ResultElementCount].Margin[RESULTS_RX_VREF_INDEX][LOW_INDEX]) {
                      RmtRow[ResultElementCount].Margin[RESULTS_RX_VREF_INDEX][LOW_INDEX] = RxVrefDqMargin->Low;
                    }
                    if (RxVrefDqMargin->High < RmtRow[ResultElementCount].Margin[RESULTS_RX_VREF_INDEX][HIGH_INDEX]) {
                      RmtRow[ResultElementCount].Margin[RESULTS_RX_VREF_INDEX][HIGH_INDEX] = RxVrefDqMargin->High;
                    }
                    if (TxVrefDqMargin->Low < RmtRow[ResultElementCount].Margin[RESULTS_TX_VREF_INDEX][LOW_INDEX]) {
                      RmtRow[ResultElementCount].Margin[RESULTS_TX_VREF_INDEX][LOW_INDEX] = TxVrefDqMargin->Low;
                    }
                    if (TxVrefDqMargin->High < RmtRow[ResultElementCount].Margin[RESULTS_TX_VREF_INDEX][HIGH_INDEX]) {
                      RmtRow[ResultElementCount].Margin[RESULTS_TX_VREF_INDEX][HIGH_INDEX] = TxVrefDqMargin->High;
                    }
                  } // Bit loop
                  if (MsgPrint) {
                    PrintBdatResult (MrcData, RmtRow, ResultElementCount);
                  }
                  ResultElementCount += 1;
                  RmtResults->ResultsHeader.RsBlock.ResultElementCount = ResultElementCount;
                  if (ResultElementCount > MaxNumberRows) {
                    MRC_DEBUG_MSG (Debug, DebugLevel, "%s Out of space after per rank DQ\n", gErrString);
                    return;
                  }
                  // Per rank RankResultType1  - CmdT, CmdV
                  CmdMargin = &ChannelOut->Command[RankInChannel];
                  RmtRow[ResultElementCount].Header.Controller = Controller;
                  RmtRow[ResultElementCount].Header.Channel    = Channel;
                  RmtRow[ResultElementCount].Header.DimmA      = Dimm;
                  RmtRow[ResultElementCount].Header.RankA      = Rank;
                  RmtRow[ResultElementCount].Header.Lane       = Byte;
                  RmtRow[ResultElementCount].Header.IoLevel    = IO_LEVEL_DDR_LEVEL;
                  RmtRow[ResultElementCount].Header.ResultType = RankResultType1; // CmdAll, CmdVref, CtlAll
                  RmtRow[ResultElementCount].Margin[RESULTS_CMD_INDEX][LOW_INDEX]        = CmdMargin->Left;
                  RmtRow[ResultElementCount].Margin[RESULTS_CMD_INDEX][HIGH_INDEX]       = CmdMargin->Right;
                  RmtRow[ResultElementCount].Margin[RESULTS_CMD_VREF_INDEX][LOW_INDEX]   = CmdMargin->Low;
                  RmtRow[ResultElementCount].Margin[RESULTS_CMD_VREF_INDEX][HIGH_INDEX]  = CmdMargin->High;
                  if (MsgPrint) {
                    PrintBdatResult (MrcData, RmtRow, ResultElementCount);
                  }
                  ResultElementCount += 1;
                  RmtResults->ResultsHeader.RsBlock.ResultElementCount = ResultElementCount;
                  if (ResultElementCount > MaxNumberRows) {
                    MRC_DEBUG_MSG (Debug, DebugLevel, "%s Out of space after per rank CMD\n", gErrString);
                    return;
                  }

                  // Per rank RankResultType1  - RecEn, Wrlvl
                  RecvEnMargin = &ChannelOut->ReceiveEnable[RankInChannel];
                  WrLvlMargin  = &ChannelOut->WriteLevel[RankInChannel];
                  RmtRow[ResultElementCount].Header.Controller = Controller;
                  RmtRow[ResultElementCount].Header.Channel    = Channel;
                  RmtRow[ResultElementCount].Header.DimmA      = Dimm;
                  RmtRow[ResultElementCount].Header.RankA      = Rank;
                  RmtRow[ResultElementCount].Header.Lane       = Byte;
                  RmtRow[ResultElementCount].Header.IoLevel    = IO_LEVEL_DDR_LEVEL;
                  RmtRow[ResultElementCount].Header.ResultType = RankResultType3; //RecEn and WrLvl
                  RmtRow[ResultElementCount].Margin[RESULTS_RECEN_INDEX][LOW_INDEX]  = RecvEnMargin->Left;
                  RmtRow[ResultElementCount].Margin[RESULTS_RECEN_INDEX][HIGH_INDEX] = RecvEnMargin->Right;
                  RmtRow[ResultElementCount].Margin[RESULTS_WRLVL_INDEX][LOW_INDEX]  = WrLvlMargin->Left;
                  RmtRow[ResultElementCount].Margin[RESULTS_WRLVL_INDEX][HIGH_INDEX] = WrLvlMargin->Right;
                  if (MsgPrint) {
                    PrintBdatResult (MrcData, RmtRow, ResultElementCount);
                  }
                  ResultElementCount += 1;
                  RmtResults->ResultsHeader.RsBlock.ResultElementCount = ResultElementCount;
                  if (ResultElementCount > MaxNumberRows) {
                    MRC_DEBUG_MSG (Debug, DebugLevel, "%s Out of space after per rank RcvEn/WrLvl\n", gErrString);
                    return;
                  }

                  if (Inputs->BdatTestType == RankMarginToolPerBitType) {
                    for (Bit = 0; Bit < MAX_BITS; Bit++) {
                      if (!MrcBitExist (Controller, Byte, Bit)) {
                        continue; // DDR5 ECC byte has only 4 bits DQ
                      }
                      Lane = (Byte * MAX_BITS);
                      Lane += (Byte == MRC_DDR5_ECC_BYTE) ? (Bit % MRC_DDR5_ECC_MAX_BITS) : Bit;
                      MRC_DEBUG_MSG (Debug, DebugLevel, "Ch: %d, D: %d R: %d L: %d\n", Channel, Dimm, Rank, Lane);
                      // per lane LaneResultType  - RxDqs, TxDq, RxVref and TxVref
                      RmtRow[ResultElementCount].Header.Controller = Controller;
                      RmtRow[ResultElementCount].Header.Channel    = Channel;
                      RmtRow[ResultElementCount].Header.DimmA      = Dimm;
                      RmtRow[ResultElementCount].Header.RankA      = Rank;
                      RmtRow[ResultElementCount].Header.Lane       = Lane;
                      RmtRow[ResultElementCount].Header.IoLevel    = IO_LEVEL_DDR_LEVEL;
                      RmtRow[ResultElementCount].Header.ResultType = LaneResultType; //RxDqsdelay, TxDqDelay, RxVref, TxVref

                      RxDqMargin              = &ChannelOut->RxDqPb[RankInChannel][Byte][Bit];
                      TxDqMargin              = &ChannelOut->TxDqPb[RankInChannel][Byte][Bit];
                      RxVrefDqMargin          = &ChannelOut->RxDqVrefPb[RankInChannel][Byte][Bit];
                      TxVrefDqMargin          = &ChannelOut->TxDqVrefPb[RankInChannel][Byte][Bit];

                      RmtRow[ResultElementCount].Margin[RESULTS_RX_DQ_INDEX][LOW_INDEX]    = RxDqMargin->Left;
                      RmtRow[ResultElementCount].Margin[RESULTS_RX_DQ_INDEX][HIGH_INDEX]   = RxDqMargin->Right;
                      RmtRow[ResultElementCount].Margin[RESULTS_TX_DQ_INDEX][LOW_INDEX]    = TxDqMargin->Left;
                      RmtRow[ResultElementCount].Margin[RESULTS_TX_DQ_INDEX][HIGH_INDEX]   = TxDqMargin->Right;
                      RmtRow[ResultElementCount].Margin[RESULTS_RX_VREF_INDEX][LOW_INDEX]  = RxVrefDqMargin->Low;
                      RmtRow[ResultElementCount].Margin[RESULTS_RX_VREF_INDEX][HIGH_INDEX] = RxVrefDqMargin->High;
                      RmtRow[ResultElementCount].Margin[RESULTS_TX_VREF_INDEX][LOW_INDEX]  = TxVrefDqMargin->Low;
                      RmtRow[ResultElementCount].Margin[RESULTS_TX_VREF_INDEX][HIGH_INDEX] = TxVrefDqMargin->High;

                      if (MsgPrint) {
                        PrintBdatResult(MrcData, RmtRow, ResultElementCount);
                      }
                      ResultElementCount += 1;
                      RmtResults->ResultsHeader.RsBlock.ResultElementCount = ResultElementCount;
                      if (ResultElementCount > MaxNumberRows) {
                        MRC_DEBUG_MSG (Debug, DebugLevel, "%s Out of space after per bit DQ\n", gErrString);
                        return;
                      }
                    } // Bit
                  } // RankMarginToolPerBitType
                } // Byte
              } // Rank
            } // if Dimm present
          } // Dimm
        } // if Channel present
      } // Channel
    } // if Controller present
  } // Controller

  RmtResults->Metadata.ResultEleCount = (UINT16) ResultElementCount;
} // FillRmtBdatStructure

/**
  This function fills the Margin2D.
  The schema include meta data and columns data sections.

  @param[in, out] MrcData           - Include all MRC global data.
  @param[out]     DataBuffer     - Pointer to data block to fill up the Margin2D result structure.
  @param[in]      DataBufferSize - Size of the Margin2D results data block.
  @param[in]      MsgPrint          - Show debug prints
**/
VOID
FillMargin2DBdatStructure (
  IN OUT MrcParameters *const MrcData,
  OUT   VOID           *DataBuffer,
  IN    UINT32          DataBufferSize,
  IN    BOOLEAN         MsgPrint
  )
{
  const MrcInput      *Inputs;
  const MRC_FUNCTION  *MrcCall;
  const MrcChannelOut *ChannelOut;       // Pointer to the channel output portion of the MRC global data area.
  const MrcDimmOut    *DimmOut;          // Pointer to the DIMM output portion of the MRC global data area.
  static const INT16  VrefPointsConst[] = {0, -6, -12, -18, 6, 12, 18};
  MrcOutput           *Outputs;
  MrcDebug            *Debug;
  MrcDebugMsgLevel    DebugLevel;
  BASE_MARGIN_2D_RESULT *Margin2DResults;
  MARGIN_2D_RESULT_COLUMNS  *Margin2DRow;
  INT32 MaxNumberRows;
  INT32 ResultElementCount;
  UINT8 Controller;
  UINT8 Channel;
  UINT8 Dimm;
  UINT8 Rank;
  UINT8 RankInChannel;     // Rank number in a channel.
  MARGIN_2D_EYE_TYPE EyeTypeIndex;
  UINT8 OuterOffsetIndex;
  MrcVersion Version;

  Outputs = &MrcData->Outputs;
  Inputs  = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;
  Debug   = &Outputs->Debug;
  DebugLevel = MsgPrint ? MSG_LEVEL_NOTE : MSG_LEVEL_NONE;
  MrcVersionGet (MrcData, &Version);

  Margin2DResults = (BASE_MARGIN_2D_RESULT*) DataBuffer;
  MrcCall->MrcSetMem ((UINT8*) DataBuffer, DataBufferSize, 0);
  MrcCall->MrcCopyMem ((UINT8*) (&Margin2DResults->ResultsHeader.MdBlock.MetadataType), (UINT8*) (&gMargin2DResultMetadataGuid), sizeof (Margin2DResults->ResultsHeader.MdBlock.MetadataType));
  Margin2DResults->ResultsHeader.MdBlock.MetadataSize = sizeof (MARGIN_2D_RESULT_METADATA);
  MrcCall->MrcCopyMem ((UINT8*) (&Margin2DResults->ResultsHeader.RsBlock.ResultType), (UINT8*) (&gMargin2DResultColumnsGuid), sizeof (Margin2DResults->ResultsHeader.RsBlock.ResultType));
  Margin2DResults->ResultsHeader.RsBlock.ResultElementSize = sizeof (MARGIN_2D_RESULT_COLUMNS);
  MaxNumberRows = DataBufferSize - sizeof (RESULTS_DATA_HDR) - sizeof (MARGIN_2D_RESULT_METADATA);
  MaxNumberRows = MaxNumberRows / sizeof (MARGIN_2D_RESULT_COLUMNS);
  Margin2DResults->ResultsHeader.RsBlock.ResultCapacity = MaxNumberRows;
  ResultElementCount = 0;
  Margin2DResults->ResultsHeader.RsBlock.ResultElementCount = ResultElementCount;
  Margin2DResults->Metadata.BurstLength       = Outputs->NumCL;
  Margin2DResults->Metadata.LoopCount         = 15;
  Margin2DResults->Metadata.MajorVer          = Version.Major;
  Margin2DResults->Metadata.MinorVer          = Version.Minor;
  Margin2DResults->Metadata.RevVer            = Version.Rev;
  Margin2DResults->Metadata.BuildVer          = Version.Build;

  Margin2DRow = &(Margin2DResults->Rows[0]);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (Outputs->Controller[Controller].Status == CONTROLLER_PRESENT) {
      for (Channel = 0; Channel < Outputs->MaxChannels; Channel++) {
        if (Outputs->Controller[Controller].Channel[Channel].Status == CHANNEL_PRESENT) {
          ChannelOut = &Outputs->Controller[Controller].Channel[Channel];
          for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
            DimmOut = &ChannelOut->Dimm[Dimm];
            if (DimmOut->Status == DIMM_PRESENT) {
              for (Rank = 0; (Rank < MAX_RANK_IN_DIMM) && (Rank < DimmOut->RankInDimm); Rank++) {
                RankInChannel = (Dimm * MAX_RANK_IN_DIMM) + Rank;
                for (EyeTypeIndex = RxDqsRxVrefEyeType; EyeTypeIndex <= TxDqTxVrefEyeType; EyeTypeIndex++) {
                  for (OuterOffsetIndex = 0; OuterOffsetIndex < MAX_2D_EYE_OFFSETS; OuterOffsetIndex++){
                    Margin2DRow[ResultElementCount].Controller           = Controller;
                    Margin2DRow[ResultElementCount].Channel              = Channel;
                    Margin2DRow[ResultElementCount].Dimm                 = Dimm;
                    Margin2DRow[ResultElementCount].Rank                 = Rank;
                    Margin2DRow[ResultElementCount].Lane                 = 0; // We don't collect per lane margin2d data yet
                    Margin2DRow[ResultElementCount].EyeType              = (UINT8) EyeTypeIndex;
                    Margin2DRow[ResultElementCount].OuterMarginParamType = (UINT8) VotageMarginParamType;
                    Margin2DRow[ResultElementCount].OuterOffset          = VrefPointsConst[OuterOffsetIndex];
                    Margin2DRow[ResultElementCount].LeftMargin           = Outputs->Margin2DResult[EyeTypeIndex][RankInChannel][Channel][OuterOffsetIndex][1];
                    Margin2DRow[ResultElementCount].RightMargin          = Outputs->Margin2DResult[EyeTypeIndex][RankInChannel][Channel][OuterOffsetIndex][0];
                    MRC_DEBUG_MSG (Debug, DebugLevel, "ElementCount:%d Controller:%d Channel:%d Dimm:%d Rank:%d lane:%d EyeType:%d OuterMarginParamType:%d OuterOffset:%d LeftMargin:%d RightMargin:%d \n",
                      ResultElementCount,
                      Margin2DRow[ResultElementCount].Controller,
                      Margin2DRow[ResultElementCount].Channel,
                      Margin2DRow[ResultElementCount].Dimm,
                      Margin2DRow[ResultElementCount].Rank,
                      Margin2DRow[ResultElementCount].Lane,
                      Margin2DRow[ResultElementCount].EyeType,
                      Margin2DRow[ResultElementCount].OuterMarginParamType,
                      Margin2DRow[ResultElementCount].OuterOffset,
                      Margin2DRow[ResultElementCount].LeftMargin,
                      Margin2DRow[ResultElementCount].RightMargin
                      );
                    ResultElementCount += 1;
                    Margin2DResults->ResultsHeader.RsBlock.ResultElementCount = ResultElementCount;
                    if (ResultElementCount > MaxNumberRows) {
                      MRC_DEBUG_MSG (Debug, DebugLevel, "BDAT: Not enough space for Margin2D results after filling row:%d.\n", (ResultElementCount - 1));
                      return;
                    }
                  } // OuterOffset
                } // EyeType
              } // Rank
            } // if Dimm present
          } // Dimm
        } // if Channel present
      } // Channel
    } // if Controller present
  } // Controller

  Margin2DResults->Metadata.ResultEleCount = (UINT16) ResultElementCount;
} // FillMargin2DBdatStructure

/**
@brief
  Get next avaliable BDat Hob

  @param[in, out] MrcData - Constant pointer to the Mrc data structure which conatins the BDAT structure to fill.
  @param[out] HobOut - Next avaliable BDat Hob

  @retval mrcSuccess if the Hob is created successfully, otherwise mrcFail.
**/

static MrcStatus
MrcGetNextBdatHob (
  IN OUT MrcParameters             *const MrcData,
  OUT BDAT_MEMORY_HOB_STRUCT       **HobOut
)
{
  MrcInput   *Inputs;
  MrcOutput  *Outputs;
  MrcDebug   *Debug;
  PLATFORM_DATA *PlatformData;
  UINT8      Index;
  MRC_BDAT_SCHEMA_LIST_HOB           *BdatSchemaListPointer;
  BDAT_MEMORY_HOB_STRUCT             *BdatMemoryHob;

  Inputs         = &MrcData->Inputs;
  Outputs        = &MrcData->Outputs;
  Debug          = &Outputs->Debug;
  PlatformData   = Inputs->PlatformDataPtr.Pointer;
  BdatSchemaListPointer = PlatformData->BdatSchemasHob.Pointer;
  BdatMemoryHob  = NULL;

  // first find the first empty spot in the BdatMemoryHob
  for (Index = 0; Index < MAX_SCHEMA_LIST_LENGTH; Index++) {
    if (PlatformData->BdatMemoryHob[Index].Pointer == NULL) {
      BdatMemoryHob = &PlatformData->BdatMemoryHob[Index];
      break;
    }
  } // for Index

  // Fail case when getting pre-filled or allocated HoBs
  if ((Index >= MAX_SCHEMA_LIST_LENGTH) || (BdatSchemaListPointer == NULL) || BdatMemoryHob == NULL) {
    if (Index == MAX_SCHEMA_LIST_LENGTH) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "No more room left in BDAT Schema List\n");
    } else if (BdatSchemaListPointer == NULL) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "BDAT Schema HOB Pointer is NULL!\n");
    } else if (BdatMemoryHob == NULL) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "BDAT Memory HOB is NULL!\n");
    }

    return mrcFail;
  }
  *HobOut = BdatMemoryHob;
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Next BDAT Memory HOB at %d\n", Index);
  return mrcSuccess;
}

/**
@brief
  Fill common BDat structure
  @param[in, out] MrcData - Constant pointer to the Mrc data structure which conatins the BDAT structure to fill.
  @param[in, Out] BdatMemoryHob - BDat Hob to fill
  @param[in] BdatSchemaSize - BDat Hob size
  @param[in] ResultsGuid - BDat Hob guid
  @retval mrcSuccess if the Hob is created successfully, otherwise mrcFail.
**/
static MrcStatus
MrcFillCommonBdatStruct (
  IN OUT MrcParameters             *const MrcData,
  IN OUT BDAT_MEMORY_HOB_STRUCT    *BdatMemoryHob,
  IN UINT32                         BdatSchemaSize,
  IN EFI_GUID                      *ResultsGuid
)
{
  MrcStatus           Status;
  MrcInput            *Inputs;
  MrcOutput           *Outputs;
  MrcDebug            *Debug;
  PLATFORM_DATA       *PlatformData;
  UINT16              SchemaIndex;
  MRC_BDAT_SCHEMA_LIST_HOB          *BdatSchemaListPointer;
  BDAT_MEMORY_DATA_HOB              *BdatMemoryHobPointer;
  MRC_BDAT_SCHEMA_HEADER_STRUCTURE  *BdatSchemaHdrPtr;

  Inputs         = &MrcData->Inputs;
  Outputs        = &MrcData->Outputs;
  Debug          = &Outputs->Debug;
  PlatformData   = Inputs->PlatformDataPtr.Pointer;
  BdatSchemaListPointer = PlatformData->BdatSchemasHob.Pointer;

  SchemaIndex = BdatSchemaListPointer->SchemaHobCount;
  if (SchemaIndex >= MAX_SCHEMA_LIST_LENGTH) {  // Needed to pass Coverity tainted variable check
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "SchemaIndex >= MAX_SCHEMA_LIST_LENGTH\n");
    return mrcFail;
  }

  Status = MrcGetBdatMemoryHob (
            (VOID **)&BdatMemoryHob->Pointer,
            BdatSchemaSize,
            ResultsGuid
            );
  if (Status != mrcSuccess) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Can't create HOB\n");
    return mrcFail;
  }

  // Increment the Schema Count by 1 to account for current schema being included
  BdatSchemaListPointer->SchemaHobCount = SchemaIndex + 1;
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "SchemaHobCount: %d\n", BdatSchemaListPointer->SchemaHobCount);

  BdatMemoryHobPointer = BdatMemoryHob->Pointer;
  BdatSchemaHdrPtr = (MRC_BDAT_SCHEMA_HEADER_STRUCTURE *) &BdatMemoryHobPointer->MemorySchema;

  // Copy the GUID ID into schema ID region
  MrcUpdateBdatHobIds (MrcData, &(BdatSchemaListPointer->SchemaHobGuids[SchemaIndex]),
                       &BdatSchemaHdrPtr->SchemaId, ResultsGuid);
  BdatSchemaHdrPtr->DataSize = BdatSchemaSize;
  // CRC for the schema header section only
  GetCrc16 ((UINT8 *) BdatSchemaHdrPtr, sizeof (MRC_BDAT_SCHEMA_HEADER_STRUCTURE), &BdatSchemaHdrPtr->Crc16);

  return Status;
}

/**
@brief
  Fill the compatible data structure BDAT with the information provided by
  the memory initialization RMT code.

  @param[in, out] MrcData - Constant pointer to the Mrc data structure which conatins the BDAT structure to fill.

  @retval mrcSuccess if the Hob is created successfully, otherwise mrcFail.
**/
static
MrcStatus
MrcFillRmtBdatStructure (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput      *Inputs;
  MrcOutput           *Outputs;
  MrcDebug            *Debug;
  MrcStatus  Status;
  UINT32     BdatSchemaSize;
  UINT8      BdatSchemaType;
  BDAT_MEMORY_HOB_STRUCT           *BdatMemoryHob;
  BDAT_MEMORY_DATA_HOB             *BdatMemoryHobPointer;
  MRC_FILL_BDAT_STRUCT_FUNC        FillBdatStructure;
  EFI_GUID     *ResultsGuid;
#ifdef MRC_DEBUG_PRINT
  const char   *RMTSchemaString      = "Rmt Schema 6B";
  const char   *Margin2DSchemaString = "Margin 2D Schema";
  const char   *SchemaTypeString;
#endif // MRC_DEBUG_PRINT

  Outputs        = &MrcData->Outputs;
  Debug          = &Outputs->Debug;
  Inputs         = &MrcData->Inputs;
  BdatSchemaType = Inputs->BdatTestType;
  BdatMemoryHob  = NULL;

  if (Inputs->BootMode != bmCold) {
    // Currently, RMT runs only at ColdBoot
    return mrcSuccess;
  }

  if (MrcIsSaGvEnabled(MrcData)) {
    switch (Outputs->SaGvPoint) {
        case MrcSaGvPoint0:
        default:
          ResultsGuid = &gSsaBiosResultsGuid;
          break;
        case MrcSaGvPoint1:
          ResultsGuid = &gSsaBiosResults1Guid;
          break;
        case MrcSaGvPoint2:
          ResultsGuid = &gSsaBiosResults2Guid;
          break;
        case MrcSaGvPoint3:
          ResultsGuid = &gSsaBiosResults3Guid;
          break;
    }
  } else {  // Default behavior is to set gSsaBiosResultsGuid
    ResultsGuid = &gSsaBiosResultsGuid;
  }

#ifdef MRC_DEBUG_PRINT
  // Determine the Schema String name
  if (BdatSchemaType == Margin2DType) {
    SchemaTypeString = Margin2DSchemaString;
  } else {
    SchemaTypeString = RMTSchemaString;
  }
#endif // MRC_DEBUG_PRINT

  // Assign the correct Function Pointer to fill the BDAT Data
  if (BdatSchemaType == Margin2DType) {
    FillBdatStructure = &FillMargin2DBdatStructure;
  } else {
    FillBdatStructure = &FillRmtBdatStructure;
  }

  Status = MrcGetNextBdatHob(MrcData, &BdatMemoryHob);

  if (Status != mrcSuccess) {
    return Status;
  }

  // Create a HOB to store the BDAT Schema
  BdatSchemaSize = GetBdatSchemaSize (MrcData);
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Filling/Saving %s structure of size %d\n", SchemaTypeString, BdatSchemaSize);

  Status = MrcFillCommonBdatStruct(MrcData, BdatMemoryHob, BdatSchemaSize, ResultsGuid);

  if (Status != mrcSuccess) {
    return Status;
  }

  BdatMemoryHobPointer = BdatMemoryHob->Pointer;
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s HOB at %ph\n", SchemaTypeString, BdatMemoryHob);

  // Fill up the RMT results
  FillBdatStructure (
      MrcData,
      ((UINT8 *) &BdatMemoryHobPointer->MemorySchema) + sizeof (MRC_BDAT_SCHEMA_HEADER_STRUCTURE),
      BdatSchemaSize,
      TRUE
      );

  // Write scratchpad register into the MCHBAR space with address of the ACPI BDAT structure.
  MrcWriteCR (MrcData, 0x5430, (UINT32) (UINTN) BdatMemoryHobPointer);  // NCDECS_CR_SCRATCHPAD_NCU_2_REG

  return mrcSuccess;
}

/**
@brief
  get summary of Ppr fail count

  @param[in, out] MrcData - Constant pointer to the Mrc data structure which conatins the BDAT structure to fill.

  @retval summary of Ppr fail count
**/
UINT32
GetNumberPprFailResult (
  IN MrcParameters *const MrcData
)
{
  UINT32 Count;
  UINT8  Controller;
  UINT8  Channel;
  UINT8  Index;

  MrcOutput *Outputs;
  ROW_FAIL_RANGE *FailPtr;

  Outputs = &MrcData->Outputs;
  Count = 0;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      for (Index = 0; Index < MAX_FAIL_RANGE; Index++) {
        FailPtr = &Outputs->FailRange[Controller][Channel][Index];
        Count += MrcCountBitsEqOne(FailPtr->BankGroupMask);
      }
    }
  }
  return Count;
}

/**
@brief
  Fill the compatible data structure BDAT with the information provided by
  the memory initialization code.

  @param[in, out] MrcData - Constant pointer to the Mrc data structure which conatins the BDAT structure to fill.

  @retval mrcSuccess if the Hob is created successfully, otherwise mrcFail.
**/
MrcStatus
MrcFillBdatStructure (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput      *Inputs;
  MrcOutput           *Outputs;
  MrcDebug            *Debug;

  Outputs        = &MrcData->Outputs;
  Debug          = &Outputs->Debug;
  Inputs         = &MrcData->Inputs;

  // Early escape from function if BDAT is disabled
  if (!Inputs->BdatEnable) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "BDAT is disabled.\n");
    return mrcSuccess;
  }

  MrcFillRmtBdatStructure(MrcData);

  return mrcSuccess;
}

