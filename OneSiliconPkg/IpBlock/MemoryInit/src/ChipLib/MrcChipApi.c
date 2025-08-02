/** @file
  .

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#include "MrcHalRegisterAccess.h"
#include "MrcCommon.h"
#include "MrcChipApi.h"
#include "MrcMemoryApi.h"
#include "MrcDdr5.h"
#include "MrcDdr5Registers.h"
#include "MrcGeneral.h"
#include "MrcCpgcApi.h"
#include "MrcDdrIoDefines.h"
#include "MrcSchedulerParameters.h"
#include "BlueTestDataEngine.h"
#include "MrcDdrIoUtils.h"
#include "MrcChipApiPrivate.h"  // for WrappedMrcProgramMrsFsm()
#include "CMrcStatsTracker.h"  // for MrcStatsStartTimer()
#include "MrcMcApi.h"

#define GEN_MRS_FSM_FLAT_TIME_MAX          (127) // BITS[6:0]
#define GEN_MRS_FSM_TIME_SCALAR_x16        (16)
#define GEN_MRS_FSM_TIME_SCALAR_x32        (32)
#define GEN_MRS_FSM_TIME_SCALAR_x64        (64)
#define GEN_MRS_FSM_TIME_SCALAR_x128       (128)
#define GEN_MRS_FSM_TIME_SCALAR_BIT_x16    (0x080)
#define GEN_MRS_FSM_TIME_SCALAR_BIT_x32    (0x100)
#define GEN_MRS_FSM_TIME_SCALAR_BIT_x64    (0x180)
#define GEN_MRS_FSM_TIME_SCALAR_BIT_x128   (0x200)

/**
  This function initializes all the necessary registers for basic training by
  MrcSetNormalMode (): Setting CPGC Mode.
  Cpgc20Init (): Activating and initializing CPGC Engine, set McChBitMask and CpgcChAssign variables
  MrcCkeOnProgramming (): Enables CKE_On for CPGC usage

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess
**/
MrcStatus
MrcPreTraining (
  IN MrcParameters *const MrcData
  )
{
  const MrcInput    *Inputs;
  MrcDebug          *Debug;
  MrcOutput         *Outputs;

  Inputs        = &MrcData->Inputs;
  Outputs       = &MrcData->Outputs;
  Debug         = &Outputs->Debug;

  MrcSetNormalMode (MrcData, FALSE); // Go to CPGC mode
  Outputs->CpgcModeLocked = TRUE; // Locking CPGC mode until NormalMode executed in MRC flow
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "CPGC Mode locked\n");

  TestDataEngineInit (MrcData);

  if (Inputs->BootMode != bmWarm) {
    MrcCkeOnProgramming(MrcData);
  }

  return mrcSuccess;
}

/**
  This function uses MC0_CH0_CR_MRS_CR_MAILBOX Register to Access Generic MRS FSM Control, Timing or Value registers.

  @param[in]     MrcData            - Pointer to MRC global data.
  @param[in]     Controller         - Handle to the controller
  @param[in]     McCh               - Handle to channel within the memory controller
  @param[in]     Opcode             - 0 - Read register or 1 - Write register
  @param[in]     SelectionType      - Select type of MRS register to access: 00 - control registers or 01 - storage registers or 10 - timing registers
  @param[in]     SelectionIndex     - Select which register index to access in each of the MRS registers types: control, data and timing.
  @param[in]     Data               - In case field 'Opcode' is set to Write, on run, data in this field will be writen to the register mentioned in fields 'Sel_Type' and 'Sel_Index'

  @retval mrcSuccess, or mrcTimeout if FSM is not responding
**/
MrcStatus
GenericMrsFsmMailboxAccess (
  IN MrcParameters        *MrcData,
  IN UINT32               Controller,
  IN UINT32               McCh,
  IN GmfMailboxOpcode     Opcode,
  IN GmfMailboxType       SelectionType,
  IN UINT32               SelectionIndex,
  IN OUT UINT32           *Data
  )
{
  MrcStatus           Status;
  MrcOutput           *Outputs;
  MrcDebug            *Debug;
  const MRC_FUNCTION  *MrcCall;
  const MrcInput      *Inputs;
  UINT64              Timeout;
  UINT32              Offset;
  BOOLEAN             Busy;
  MC0_CH0_CR_MRS_CR_MAILBOX_STRUCT       GenMailboxStruct;

  Status  = mrcSuccess;
  Outputs = &MrcData->Outputs;
  Inputs  = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;
  Debug   = &Outputs->Debug;

  Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_MRS_CR_MAILBOX_REG, MC1_CH0_CR_MRS_CR_MAILBOX_REG, Controller, MC0_CH1_CR_MRS_CR_MAILBOX_REG, McCh);
  GenMailboxStruct.Data = 0;
  GenMailboxStruct.Bits.Run       = 1;
  GenMailboxStruct.Bits.Opcode    = Opcode;
  GenMailboxStruct.Bits.Sel_Type  = SelectionType;
  GenMailboxStruct.Bits.Sel_Index = SelectionIndex;
  GenMailboxStruct.Bits.CR_Data   = *Data;

  MrcWriteCR64 (MrcData, Offset, GenMailboxStruct.Data);

  // Poll until MC0_CH0_CR_MRS_CR_MAILBOX.Run goes low
  Timeout = MrcCall->MrcGetCpuTime () + MRC_WAIT_TIMEOUT; // 10 seconds timeout
  Busy = FALSE;

  do {
    GenMailboxStruct.Data = MrcReadCR64 (MrcData, Offset);
    Busy = (GenMailboxStruct.Bits.Run == 1);
    if (Inputs->ExtInputs.Ptr->SimicsFlag == 1) {
      Busy = FALSE;
    }
  } while (Busy && (MrcCall->MrcGetCpuTime () < Timeout));

  // Check for Timeout
  if (Busy) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s MRS_CR_MAILBOX TIMEOUT.\n", gErrString);
    Status = mrcTimeout;
  }

  if (Opcode == GmfMailboxRead) {
    *Data = GenMailboxStruct.Bits.CR_Data;
  }

  return Status;
}

/**
  This function configures the Generic MRS FSM (Gmf) timing registers on the
  input Controller McCh based on the pre-defined GmfTimingIndex timing assignments.
  These timings are different per technology and are statically assigned a timing
  register field index.

  @param[in]     MrcData            - Pointer to MRC global data.
  @param[in]     Controller         - Handle to the controller
  @param[in]     McCh               - Handle to channel within the memory controller

  @retval mrcSuccess if successful.
  @retval mrcFail if the timing registers are full and the requested Delay could not be allocated.
**/
MrcStatus
ConfigGenMrsFsmTiming  (
  IN     MrcParameters *MrcData,
  IN     UINT32        Controller,
  IN     UINT32        McCh
  )
{
  MrcStatus        Status;
  MrcInput         *Inputs;
  MrcOutput        *Outputs;
  UINT32           Data;
  UINT16           MrDelay;
  UINT32           TimingSetIdx;
  UINT32           TimingIdx;
  UINT32           ScaledTimeMax;
  UINT16           TimingEncode;
  BOOLEAN          IsLpddr5;
  MRC_EXT_INPUTS_TYPE *ExtInputs;
  GENERIC_MRS_FSM_TIMING_STORAGE_STRUCT GenMrsFsmTiming;

  Inputs    = &MrcData->Inputs;
  ExtInputs = Inputs->ExtInputs.Ptr;
  Outputs   = &MrcData->Outputs;
  IsLpddr5  = Outputs->IsLpddr5;
  Data      = 0;

  // Configure all of the Generic MRS FSM Timing registers on the current Controller, McCh
  for (TimingSetIdx = GmfTimingIndex0; TimingSetIdx < GmfTimingIndexMax; TimingSetIdx++) {
    Status = MrcGetGmfDelayTiming (MrcData, TimingSetIdx, &MrDelay);
    if (Status != mrcSuccess) {
      return Status;
    }

    if (IsLpddr5) {
      // Need to scale from tCK to WCK
      MrDelay *= 4;
    }

    ScaledTimeMax = (GEN_MRS_FSM_FLAT_TIME_MAX * GEN_MRS_FSM_TIME_SCALAR_x128);
    if (MrDelay > ScaledTimeMax) {
      // Check if the timing value is larger than what the register supports.
      return mrcWrongInputParameter;
    } else if (MrDelay > (GEN_MRS_FSM_FLAT_TIME_MAX * GEN_MRS_FSM_TIME_SCALAR_x64)) {
      // Round to the nearest multiple of the scalar based value and set the bit to enable the scalar
      TimingEncode = ((UINT16)DIVIDECEIL(MrDelay, GEN_MRS_FSM_TIME_SCALAR_x128)) | GEN_MRS_FSM_TIME_SCALAR_BIT_x128;
    } else if (MrDelay > (GEN_MRS_FSM_FLAT_TIME_MAX * GEN_MRS_FSM_TIME_SCALAR_x32)) {
      // Round to the nearest multiple of the scalar based value and set the bit to enable the scalar
      TimingEncode = ((UINT16) DIVIDECEIL (MrDelay, GEN_MRS_FSM_TIME_SCALAR_x64)) | GEN_MRS_FSM_TIME_SCALAR_BIT_x64;
    } else if (MrDelay > (GEN_MRS_FSM_FLAT_TIME_MAX * GEN_MRS_FSM_TIME_SCALAR_x16)) {
      // Round to the nearest multiple of the scalar based value and set the bit to enable the scalar
      TimingEncode = ((UINT16) DIVIDECEIL (MrDelay, GEN_MRS_FSM_TIME_SCALAR_x32)) | GEN_MRS_FSM_TIME_SCALAR_BIT_x32;
    } else if (MrDelay > GEN_MRS_FSM_FLAT_TIME_MAX) {
      // Round to the nearest multiple of the scalar based value and set the bit to enable the scalar
      TimingEncode = ((UINT16) DIVIDECEIL (MrDelay, GEN_MRS_FSM_TIME_SCALAR_x16)) | GEN_MRS_FSM_TIME_SCALAR_BIT_x16;
    } else {
      TimingEncode = MrDelay;
    }

    if (IsLpddr5 && ExtInputs->IsWckIdleExitEnabled && !Inputs->IsDdrIoMbA0) {
      if (Outputs->Frequency >= f8533) {
        if (TimingSetIdx == GmfTimingIndex9) {
          TimingEncode = 0xFF;
        }
      }
    }

    // Calculate the register offset
    // Up to 12 entries, 3 in each register
    if (TimingSetIdx < GMF_TIMING_FIELDS_PER_REGISTER) {
      TimingIdx = 0;
    } else if (TimingSetIdx < (2 * GMF_TIMING_FIELDS_PER_REGISTER)) {
      TimingIdx = 1;
    } else if (TimingSetIdx < (3 * GMF_TIMING_FIELDS_PER_REGISTER)) {
      TimingIdx = 2;
    } else {
      TimingIdx = 3;
    }
    GenericMrsFsmMailboxAccess (MrcData, Controller, McCh, GmfMailboxRead, GmfMailboxTypeTiming, TimingIdx, &Data);
    GenMrsFsmTiming.Data = Data;

    switch (TimingSetIdx % GMF_TIMING_FIELDS_PER_REGISTER) {
      case 0:
        GenMrsFsmTiming.Bits.TIMING_FIELD_0 = TimingEncode;
        break;

      case 1:
        GenMrsFsmTiming.Bits.TIMING_FIELD_1 = TimingEncode;
        break;

      case 2:
        GenMrsFsmTiming.Bits.TIMING_FIELD_2 = TimingEncode;
        break;

      default:
        return mrcFail;
    }

    Data = GenMrsFsmTiming.Data;
    GenericMrsFsmMailboxAccess (MrcData, Controller, McCh, GmfMailboxWrite, GmfMailboxTypeTiming, TimingIdx, &Data);
  } // for TimingSetIdx

  return mrcSuccess;
}

/**
  This function configures the Generic MRS FSM shadow registers as specified by the
  input parameters. This function will handle programming of the control
  registers and will program the appropriate MR value register type:
  UlxUly: GENERIC_MRS_FSM_PER_RANK_VALUES is used when MrPerRank == TRUE
              GENERIC_MRS_FSM_SHADOW_VALUES is used when MrPerRank == FALSE
  DtHalo: GENERIC_MRS_FSM_STORAGE_VALUES is used

  @param[in]     MrcData       - Pointer to MRC global data.
  @param[in]     Controller    - Handle to the controller
  @param[in]     McCh          - Handle to channel within the memory controller
  @param[in]     GenMrRef      - Pointer to an entry from the MrData array belonging to the ControlRegIdx values to be set.
  @param[in]     MrPerRank     - The DRAM Mode Register requires unique values for each rank.
  @param[in]     ControlRegIdx - The MrData array entry to use when configuring the shadow registers.
  @param[in]     MrData        - Pointer to an array of MR data to configure the MRS FSM with.
  @param[in,out] StorageIdx    - StorageIdx points to the next available GENERIC_MRS_FSM_STORAGE_VALUES
                                 byte.

  @retval mrcSuccess if successful.
  @retval mrcFail if the shadow registers are out of free entries.
**/
MrcStatus
ConfigGenMrsFsmValue (
  IN     MrcParameters           *MrcData,
  IN     UINT32                  Controller,
  IN     UINT32                  McCh,
  IN     MRC_GEN_MRS_FSM_MR_TYPE *GenMrRef,
  IN     BOOLEAN                 MrPerRank,
  IN     UINT8                   ControlRegIdx,
  IN     MRC_GEN_MRS_FSM_MR_TYPE MrData[MAX_CONTROLLER][MAX_CHANNEL][MAX_RANK_IN_CHANNEL][MAX_MR_GEN_FSM],
  IN OUT UINT8                   *StorageIdx
  )
{
  const MRC_FUNCTION  *MrcCall;
  MrcDebug  *Debug;
  MrcOutput *Outputs;
  UINT32    ControlRegNum;
  UINT32    Channel;
  UINT32    MaxSubCh;
  UINT32    SubCh;
  UINT32    Data;
  UINT32    Rank;
  UINT8     ShadowFieldIdx;
  UINT8     MrPdaData[MAX_PDA_MR_IN_CHANNEL];
  UINT8     NumMrData;
  UINT8     mrDataIndex;
  BOOLEAN   IsLpddr;
  MrcStatus Status;
  MRC_GEN_MRS_FSM_MR_TYPE                *GenMrPtr;
  GENERIC_MRS_FSM_CONTROL_STRUCT         GenMrsFsmCtl;
  GENERIC_MRS_FSM_STORAGE_VALUES_STRUCT  GenMrsFsmChVal;

  Outputs  = &MrcData->Outputs;
  Debug    = &Outputs->Debug;
  IsLpddr  = Outputs->IsLpddr;
  MaxSubCh = (IsLpddr) ? MAX_SUB_CHANNEL : 1;
  Status   = mrcSuccess;
  MrcCall  = MrcData->Inputs.Call.Func;

  MrcCall->MrcSetMem ((UINT8 *) MrPdaData, sizeof (MrPdaData), 0);
  NumMrData = 0;
  // Configure control register for this MR.
  GenMrsFsmCtl.Data = 0;
  if (GenMrRef->PdaMr) {
    GenMrsFsmCtl.Bits.PER_RANK = 1;
    GenMrsFsmCtl.Bits.PER_DEVICE = 1;
  } else if (MrPerRank) {
    GenMrsFsmCtl.Bits.PER_RANK = 1;
  }
  GenMrsFsmCtl.Bits.COMMAND_TYPE = GenMrRef->CmdType;
  GenMrsFsmCtl.Bits.ADDRESS = GenMrRef->MrAddr;
  GenMrsFsmCtl.Bits.ACTIVE = GenMrRef->Valid;
  GenMrsFsmCtl.Bits.TIMING_VALUE_POINTER = GenMrRef->DelayIndex;
  GenMrsFsmCtl.Bits.GENERIC_MRS_STORAGE_POINTER = *StorageIdx;
  GenMrsFsmCtl.Bits.FSP_CONTROL = (GenMrRef->FspWrToggle ? 1 : (GenMrRef->FspOpToggle ? 2 : 0));

  Data = GenMrsFsmCtl.Data;
  GenericMrsFsmMailboxAccess (MrcData, Controller, McCh, GmfMailboxWrite, GmfMailboxTypeControl, ControlRegIdx, &Data);

  // Lp5 should always fall into first condition since PDA is not supported on Lp5
  if ((!GenMrRef->PdaMr) || IsLpddr) {
    // Configure MR Value registers. case of PerRank or Per Ch MR
    for (SubCh = 0; SubCh < MaxSubCh; SubCh++) {
      Channel = (McCh * MaxSubCh) + SubCh;
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
          continue;
        }

        GenMrPtr = &MrData[Controller][Channel][Rank][ControlRegIdx];
        if (!GenMrPtr->Valid) {
          continue;
        }

        // Per Rank configurations may span multiple registers, including registers
        // where previous byte indexes are occupied by non per-rank values. To prevent
        // overwriting existing entries, do a read-modify-write for each new value.
        // Crossing register boundaries will be automatically handled by incrementing
        // StorageIdx for each new value written.
        MRC_DEBUG_ASSERT ((*StorageIdx < GEN_MRS_FSM_BYTE_MAX), Debug, "Number of byte storage instances has exceeded the max supported by the MC\n");
        ControlRegNum = (*StorageIdx) / 4;

        GenericMrsFsmMailboxAccess (MrcData, Controller, McCh, GmfMailboxRead, GmfMailboxTypeStorage, ControlRegNum, &Data);
        GenMrsFsmChVal.Data = Data;

        // The Registers in a dual rank configuration must be stored in the following order
        // assuming n is the byte address specified in GENERIC_MRS_STORAGE_POINTER.
        // n   = Sub-channel 0, Rank 0
        // n+1 = Sub-channel 0, Rank 1
        // n+2 = Sub-channel 1, Rank 0
        // n+3 = Sub-channel 1, Rank 1
        ShadowFieldIdx = (*StorageIdx) % 4;
        switch (ShadowFieldIdx) {
        case 0:
          GenMrsFsmChVal.Bits.SHADOW_0 = GenMrPtr->MrData;
          break;

        case 1:
          GenMrsFsmChVal.Bits.SHADOW_1 = GenMrPtr->MrData;
          break;

        case 2:
          GenMrsFsmChVal.Bits.SHADOW_2 = GenMrPtr->MrData;
          break;

        case 3:
          GenMrsFsmChVal.Bits.SHADOW_3 = GenMrPtr->MrData;
          break;
        }
        Data = GenMrsFsmChVal.Data;
        GenericMrsFsmMailboxAccess (MrcData, Controller, McCh, GmfMailboxWrite, GmfMailboxTypeStorage, ControlRegNum, &Data);

        (*StorageIdx)++;
        // Only a single byte field is needed for non per-rank MrData array entries.
        if (!MrPerRank) {
          break;
        }
      } // Rank
        // Only a single byte field is needed for non per-rank MrData array entries.
      if (!MrPerRank) {
        break;
      }
    } // SubCh
  } else if (GenMrRef->Valid) {
    // DDR5 Only, since PDA is only supported DDR5
    // Configure MR Value registers. case of PDA or Per Ch MR
    // First Get the data for active Ranks and Dev
    Status = MrFillPdaMrsDataDdr5 (MrcData, Controller, McCh, GenMrRef->MrAddr, MrPdaData, &NumMrData);
    // check boundaries
    MRC_DEBUG_ASSERT ((Status == mrcSuccess), Debug, "Failed to  MrFillPdaMrsData Controller %d, Channel= %d, MrAddress = %d \n", Controller, McCh, GenMrRef->MrAddr);
    MRC_DEBUG_ASSERT (((*StorageIdx + NumMrData)< GEN_MRS_FSM_BYTE_MAX), Debug, "Number of byte storage instances has exceeded the max supported by the MC\n");

    // configure the MrPdaData  vals in the appropriate GENERIC_MRS_FSM_STORAGE reg according to *StorageIdx
    for (mrDataIndex=0; mrDataIndex < NumMrData ;) {
      ShadowFieldIdx = (*StorageIdx) % 4;
      ControlRegNum = (*StorageIdx) / 4;

      if (ShadowFieldIdx) {
        // Register is partially configured Need Read Modify write
        GenericMrsFsmMailboxAccess (MrcData, Controller, McCh, GmfMailboxRead, GmfMailboxTypeStorage, ControlRegNum, &Data);
        GenMrsFsmChVal.Data = Data;

        while (ShadowFieldIdx && (mrDataIndex < NumMrData)) {
          GenMrsFsmChVal.Data8[ShadowFieldIdx] = MrPdaData[mrDataIndex];
          (*StorageIdx)++;
          mrDataIndex++;
          ShadowFieldIdx = (*StorageIdx) % 4;
        }
      } else {
        // In beginning of the Register, no need to read
        GenMrsFsmChVal.Data = 0;
        while ((ShadowFieldIdx < 4) && (mrDataIndex < NumMrData)) { // fill up to 4 bytes or we finished the data
          GenMrsFsmChVal.Data8[ShadowFieldIdx] = MrPdaData[mrDataIndex];
          (*StorageIdx)++;
          mrDataIndex++;
          ShadowFieldIdx++;
        }
      }
      Data = GenMrsFsmChVal.Data;
      GenericMrsFsmMailboxAccess (MrcData, Controller, McCh, GmfMailboxWrite, GmfMailboxTypeStorage, ControlRegNum, &Data);
    }
  } // else if

  return Status;
}

/**
  This function checks if the input MrAddr value is present in the MrPerRankLookup array.

  @param[in] MrAddr - MR Address to lookup in MrPerRankLookup
  @param[in] MrPerRankLookup - mrEndOfSequence terminated array specifying which MR addresses
  require per-rank configuration.

  @retval TRUE if MrAddr value exists in MrPerRankLookup array
          FALSE if MrPerRankLookup is NULL
  @retval FALSE in all other cases
**/
BOOLEAN
IsMrPerRank (
  IN MrcModeRegister       MrAddr,
  IN const MrcModeRegister *MrPerRankLookup
)
{
  UINT32 Index;
  BOOLEAN MrPerRank;
  MrPerRank = FALSE;

  if (MrPerRankLookup != NULL) {
    for (Index = 0; MrPerRankLookup[Index] != mrEndOfSequence; Index++) {
      if (MrPerRankLookup[Index] == MrAddr) {
        MrPerRank = TRUE;
        break;
      }
    }
  }

  return MrPerRank;
}

/**
  This function configures the Generic MRS FSM shadow registers based on the MrData inputs.
  It will determine if it needs to use the per-rank feature if the MR value differs across ranks.

  @param[in] MrcData - Pointer to MRC global data.
  @param[in] MrData  - Pointer to an array of MR data to configure the MRS FSM with.
  @param[in] SagvConfig - TRUE if Configure the Generic FSM for SAGV transition
  @param[out] MrPerRankLookup - Optional mrEndOfSequence terminated array specifying MR addresses that must
  be sent per-rank, all other registers use the same data for all ranks on the
  same channel. If this pointer is NULL, then only MRs with different values in
  each rank are configued as per-rank Generic MRS FSM entries.

  @retval mrcSuccess if successful.
  @retval mrcFail if MrData pointer is null, the timing or per-rank registers are out of free entries.
**/
MrcStatus
MrcGenMrsFsmConfig (
  IN  MrcParameters *MrcData,
  IN  MRC_GEN_MRS_FSM_MR_TYPE MrData[MAX_CONTROLLER][MAX_CHANNEL][MAX_RANK_IN_CHANNEL][MAX_MR_GEN_FSM],
  IN  BOOLEAN                 SagvConfig,
  OUT const MrcModeRegister   *MrPerRankLookup OPTIONAL
  )
{
  const MRC_FUNCTION  *MrcCall;
  const MrcInput      *Inputs;
  MRC_EXT_INPUTS_TYPE *ExtInputs;
  MrcDebug  *Debug;
  MrcOutput *Outputs;
  MrcStatus Status;
  UINT32    Controller;
  UINT32    Channel;
  UINT32    McCh;
  UINT32    MaxSubCh;
  UINT32    Offset;
  UINT32    Rank;
  UINT32    SubCh;
  UINT16    InitData;
  UINT8     ControlRegIdx;
  UINT8     ValidControlRegIndex;
  UINT8     StorageIdx;
  UINT8     SaveStorageIdx;
  UINT8     FspOpSwitchIdx;
  BOOLEAN   SagvBreakPoint;
  BOOLEAN   IsDdr5;
  BOOLEAN   IsLpddr;
  BOOLEAN   MrPerRank;
  BOOLEAN   DoZqCal;
  BOOLEAN   GlobalDevLoopEnable;
  BOOLEAN   EnablePdaSelectAll;
  MRC_GEN_MRS_FSM_MR_TYPE *GenMrPtr;
  MC0_CH0_CR_MRS_FSM_CONTROL_STRUCT MrsFsmCtl;
  BOOLEAN   RestoreStorageIdx;
  BOOLEAN   IsDimmDfeMrBit0;
  UINT8     DfeMrTap;
  UINT8     DfeMrTapStorageIdx[DDR5_DFE_TAP_MR_DQ_OFFSET];
  UINT32    IpChannel;

  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;
  Inputs = &MrcData->Inputs;
  ExtInputs = Inputs->ExtInputs.Ptr;
  MrcCall = Inputs->Call.Func;
  IsLpddr = Outputs->IsLpddr;
  IsDdr5  = Outputs->IsDdr5;
  MaxSubCh = (IsLpddr) ? MAX_SUB_CHANNEL : 1;
  GenMrPtr = NULL;
  SagvBreakPoint = FALSE;
  FspOpSwitchIdx = 0;
  // Configuring the FSM for SAGV should not enable ZqCal
  DoZqCal = SagvConfig ? FALSE : TRUE;
  GlobalDevLoopEnable = FALSE;
  RestoreStorageIdx = FALSE;

  if (Outputs->IsDvfsqEnabled) {
    DoZqCal = FALSE;
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    if (!MrcControllerExist (MrcData, Controller)) {
      // Skip unpopulated controllers.
      continue;
    }
    // Determine if the the MR value is the same across Sub-Channels and Ranks
    for (McCh = 0; McCh < MAX_CHANNEL_SHARE_REGS; McCh++) {
      // Configure the timing registers for the current MC Channel.
      Status = ConfigGenMrsFsmTiming (
                MrcData,
                Controller,
                McCh
                );
      if (Status != mrcSuccess) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Generic FSM Timing", gErrString);
        if (Status == mrcWrongInputParameter) {
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Delay Overflow");
        }
        MRC_DEBUG_MSG (
          Debug,
          MSG_LEVEL_ERROR,
          " - Mc: %u, IpCh: %u\n",
          Controller, McCh);
        return Status;
      }

      StorageIdx = 0;
      SaveStorageIdx = 0;
      EnablePdaSelectAll = FALSE;
      IsDimmDfeMrBit0 = FALSE;
      MrcCall->MrcSetMem (DfeMrTapStorageIdx, ARRAY_COUNT(DfeMrTapStorageIdx), 0);
      MRC_MC_IP_DEBUG_MSG (Debug, MSG_LEVEL_ALGO, "Mc%d.IpCh%d\n\tStorageIdx = %d\n", Controller, McCh, StorageIdx);
      ValidControlRegIndex = 0;
      for (ControlRegIdx = 0; ControlRegIdx < MAX_MR_GEN_FSM; ControlRegIdx++) {
        MrPerRank = FALSE;
        InitData  = MRC_UINT16_MAX;
        // Gate SubCh and Rank loops with MrPerRank == FALSE.
        // Once we find that they are different per rank; break out and start the register configuration
        for (SubCh = 0; ((SubCh < MaxSubCh) && (!MrPerRank)); SubCh++) {
          Channel = (McCh * MaxSubCh) + SubCh;
          for (Rank = 0; ((Rank < MAX_RANK_IN_CHANNEL) && (!MrPerRank)); Rank++) {
            if (!MrcRankExist (MrcData, Controller, Channel, Rank)) {
              continue;
            }

            GenMrPtr = &MrData[Controller][Channel][Rank][ControlRegIdx];
            if (!GenMrPtr->Valid) {
              continue;
            }

            // Base Case
            if (InitData == MRC_UINT16_MAX) {
              InitData = GenMrPtr->MrData;
            } else if ((MrPerRankLookup == NULL) && (InitData != GenMrPtr->MrData)) {
              // If MrData values differ between ranks then configure as per-rank
              MrPerRank = TRUE;
            } else if ((MrPerRankLookup != NULL)) {

              if (IsMrPerRank (GenMrPtr->MrAddr, MrPerRankLookup)) {
                MrPerRank = TRUE;
              }

              if ((GenMrPtr->MrAddr > MRC_UINT8_MAX) && (IsDdr5)) {
                GenMrPtr->MrAddr = 0;
                MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "mpcMR GenMrPtr->MrAddr changes to 0\n");
              }
            }

            if ((GenMrPtr->CmdType == GmfCmdMpc) && (GenMrPtr->MrData == DDR5_MPC_ZQCAL_LATCH)) {
              // Turn off automatic ZQCAL when issued as part of the input sequence
              DoZqCal = FALSE;
            }
          } // Rank
        } // SubCh

        // If InitData is not another value besides MRC_UINT16_MAX, then this ControlRegIdx
        // did not have a valid entry in the array for the subch's or ranks.  Skip this index.
        if (InitData == MRC_UINT16_MAX) {
          continue;
        }

        if (GenMrPtr == NULL) {
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "GenMrPtr is Null\n");
          return mrcFail;
        }
        if ((GenMrPtr->CmdType == GmfCmdMpc) && (GenMrPtr->MrData == DDR5_MPC_PDA_SELECT_ID(0xf)) && (EnablePdaSelectAll == FALSE) ) {
          // Ignore PDA Select all if there is no PDA command in the sequence
          continue;
        }

        // In DDR5 DIMM DFE there is MR per bit for each TAP (TAP 1-4)
        // For example Tap1 has {mrMR129, mrMR137, mrMR145, mrMR153, mrMR161, mrMR169, mrMR177, mrMR185} for x8 device
        // All the bits have the same DFE value (it's not trained per bit), but we still need to configure MR's for all the bits.
        // In order to save space in the Generic FSM storage we point to Bit 0 storage for all the bits (0-7)
        if (IsDdr5) {
          RestoreStorageIdx = FALSE;
          if (Ddr5IsDimmDfeMr (GenMrPtr->MrAddr)) {
            DfeMrTap = Ddr5GetDimmDfeTapStorage (GenMrPtr->MrAddr, &IsDimmDfeMrBit0);
            if (IsDimmDfeMrBit0) { // First Bit MR in this TAP
              DfeMrTapStorageIdx[DfeMrTap] = StorageIdx;
            } else {
              RestoreStorageIdx = TRUE;
              SaveStorageIdx = StorageIdx;
              StorageIdx = DfeMrTapStorageIdx[DfeMrTap];
            }
          }
        }
        Status = ConfigGenMrsFsmValue (
                  MrcData,
                  Controller,
                  McCh,
                  GenMrPtr,
                  MrPerRank,
                  ValidControlRegIndex,
                  MrData,
                  &StorageIdx
                  );
        if (Status != mrcSuccess) {
          return Status;
        }
        if (IsDdr5 && RestoreStorageIdx) {
          StorageIdx = SaveStorageIdx;
        }
        ValidControlRegIndex++;  // Increment the Actual
        if (GenMrPtr->FreqSwitchPoint) {
          SagvBreakPoint = TRUE;
          FspOpSwitchIdx = ControlRegIdx;
        }
        // Check if there is at least one PDA Mr to handel in the sequence
        if (GenMrPtr->PdaMr) {
          GlobalDevLoopEnable = TRUE;
          EnablePdaSelectAll  = TRUE;
        }
      } // ControlRegIdx
    } // McCh
  } // Controller

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      if ((!MrcChannelExist(MrcData, Controller, Channel)) || IS_MC_SUB_CH (IsLpddr, Channel)) {
        // For LPDDR5, only program register on even channels.
        continue;
      }
      IpChannel = LP_IP_CH (IsLpddr, Channel);
      Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_MRS_FSM_CONTROL_REG, MC1_CH0_CR_MRS_FSM_CONTROL_REG, Controller, MC0_CH1_CR_MRS_FSM_CONTROL_REG, IpChannel);
      // Do read, modify, write to preserve default register fields that are config independent
      MrsFsmCtl.Data = MrcReadCR (MrcData, Offset);
      // Configure the MRS_FSM to use the Generic version
      MrsFsmCtl.Bits.generic_mrs_fsm_device_loop_enable = GlobalDevLoopEnable;
      MrsFsmCtl.Bits.send_preall = (IsLpddr) ? 0 : (SagvConfig ? 1 : 0);
      //MrsFsmCtl.Bits.do_pda_for_ecc = Outputs->EccSupport ? 1 : 0;
      if (SagvBreakPoint) {
        MrsFsmCtl.Bits.split_transition = 1;
        MrsFsmCtl.Bits.lpddr_switch_fsp = 1;
        MrsFsmCtl.Bits.generic_mrs_fsm_breakpoint_address = FspOpSwitchIdx;
        MrsFsmCtl.Bits.do_dq_osc_start = (ExtInputs->TrainingEnables3.WRTRETRAIN) ? 1 : 0;
        if (IsLpddr) {
          DoZqCal = FALSE;
        }
      } else {
        MrsFsmCtl.Bits.split_transition = 0;
        MrsFsmCtl.Bits.lpddr_switch_fsp = 0;
        MrsFsmCtl.Bits.generic_mrs_fsm_breakpoint_address = MC0_CH0_CR_MRS_FSM_CONTROL_generic_mrs_fsm_breakpoint_address_DEF;
        MrsFsmCtl.Bits.do_dq_osc_start = 0;
      }
      if (DoZqCal) {
        MrsFsmCtl.Bits.do_zqcl = 1;
      } else {
        MrsFsmCtl.Bits.do_zqcl = 0;
      }
      MrcWriteCR (MrcData, Offset, MrsFsmCtl.Data);
    }
  }
  return mrcSuccess;
}

/**
  This function cleans the MRS FSM valid control on all Controllers Channels.
  @param[in] MrcData - Pointer to MRC global data.
  @param[in] MrData  - Pointer to an array of MR data to configure the MRS FSM with.
  @param[in] CleanAll - If set to TRUE, MrData values will be ignored and all Control Registers will be cleared

  @retval mrcFail if clean failed.
  @retval mrcSuccess otherwise.
**/
MrcStatus
MrcGenMrsFsmClean (
  IN  MrcParameters *MrcData,
  IN  MRC_GEN_MRS_FSM_MR_TYPE MrData[MAX_CONTROLLER][MAX_CHANNEL][MAX_RANK_IN_CHANNEL][MAX_MR_GEN_FSM],
  IN  BOOLEAN       CleanAll
  )
{
  MrcOutput *Outputs;
  UINT32    Controller;
  UINT32    Channel;
  UINT32    Rank;
  UINT32    McCh;
  UINT8     MaxChannel;
  UINT8     MrsFsmControlRegIdx;
  UINT32    Data = 0;
  BOOLEAN   IsCleanRequired;
  MRC_GEN_MRS_FSM_MR_TYPE                               *GenMrPtr;

  Outputs = &MrcData->Outputs;
  MaxChannel = Outputs->MaxChannels;

  // Run on  all FSM's
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if (MrcChannelExist (MrcData, Controller, Channel)) {
        for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
          if (MrcRankExist (MrcData, Controller, Channel, Rank)) {
            McCh = LP_IP_CH (Outputs->IsLpddr, Channel);
            for (MrsFsmControlRegIdx = 0; MrsFsmControlRegIdx < MAX_MR_GEN_FSM; MrsFsmControlRegIdx++) {
              IsCleanRequired = FALSE;
              if (MrData != NULL) {
                GenMrPtr = &MrData[Controller][Channel][Rank][MrsFsmControlRegIdx];
                if (GenMrPtr->Valid || CleanAll) {
                  IsCleanRequired = TRUE;
                }
              } else {
                // If the input MrData buffer is NULL, then we will clean all registers
                IsCleanRequired = TRUE;
              }

              if (IsCleanRequired) {
                GenericMrsFsmMailboxAccess (MrcData, Controller, McCh, GmfMailboxWrite, GmfMailboxTypeControl, MrsFsmControlRegIdx, &Data);
              }
            }
            // The first active rank in the channel is enough , the FSM works per channel
            break;
          }
        }
        // If we're LPDDR and we are on Channel 0 or 2, increment channel additionally so we move to the next MC Channel.
        if (Outputs->IsLpddr && ((Channel % 2) == 0)) {
          Channel++;
        }
      }
    }
  }
  return mrcSuccess;
}

/**
  This function executes the MRS FSM and waits for the FSM to complete.
  If the FSM does not complete after 10 seconds, it will return an error message.

  @param[in] MrcData - Pointer to MRC global data.

  @retval mrcFail if the FSM is not idle.
  @retval mrcSuccess otherwise.
**/
MrcStatus
MrcGenMrsFsmRun (
  IN  MrcParameters *MrcData
  )
{
  const MRC_FUNCTION  *MrcCall;
  MrcOutput *Outputs;
  UINT64    Timeout;
  UINT32    Controller;
  UINT32    Channel;
  UINT32    IpCh;
  UINT32    Offset;
  UINT8     MaxChannel;
  BOOLEAN   IsLpddr;
  BOOLEAN   Flag;
  MC0_CH0_CR_MRS_FSM_RUN_STRUCT MrsFsmRun;

  Outputs = &MrcData->Outputs;
  MrcCall = MrcData->Inputs.Call.Func;
  IsLpddr = Outputs->IsLpddr;
  MaxChannel = Outputs->MaxChannels;

  // Start all FSM's in parallel
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if (MrcChannelExist (MrcData, Controller, Channel)) {
        IpCh = LP_IP_CH (IsLpddr, Channel);
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_MRS_FSM_RUN_REG, MC1_CH0_CR_MRS_FSM_RUN_REG, Controller, MC0_CH1_CR_MRS_FSM_RUN_REG, IpCh);

        MrsFsmRun.Data = 0;
        MrsFsmRun.Bits.Run = 1;
        MrcWriteCR (MrcData, Offset, MrsFsmRun.Data);

        // If we're LPDDR and we are on Channel 0 or 2, increment channel additionally so we move to the next MC Channel.
        if (IsLpddr && ((Channel % 2) == 0)) {
          Channel++;
        }
      }
    }
  }

  // Check that all FSM's are done
  Timeout = MrcCall->MrcGetCpuTime () + MRC_WAIT_TIMEOUT; // 10 seconds timeout
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      if (MrcChannelExist (MrcData, Controller, Channel)) {
        IpCh = LP_IP_CH (IsLpddr, Channel);
        Offset = OFFSET_CALC_MC_CH (MC0_CH0_CR_MRS_FSM_RUN_REG, MC1_CH0_CR_MRS_FSM_RUN_REG, Controller, MC0_CH1_CR_MRS_FSM_RUN_REG, IpCh);

        do {
          MrsFsmRun.Data = MrcReadCR (MrcData, Offset);
          Flag = (MrsFsmRun.Bits.Run == 1);
        } while (Flag && (MrcCall->MrcGetCpuTime () < Timeout));
        if (Flag) {
          MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_ERROR, "Generic FSM is not idle\n");
          return mrcFail;
        }

        // If we're LPDDR and we are on Channel 0 or 2, increment channel additionally so we move to the next MC Channel.
        if (IsLpddr && ((Channel % 2) == 0)) {
          Channel++;
        }
      }
    }
  }

  return mrcSuccess;
}


/**
  This function configures the DDRCRCMDBUSTRAIN register to values for normal mode.

  @param[in]  MrcData - Pointer to global MRC data.

  @retval none.
**/
VOID
MrcSetWritePreamble (
  IN  MrcParameters *const  MrcData
  )
{
  INT64             NumMaskedPulses;
  INT64             NumOfPulses;

  NumMaskedPulses = (MrcData->Outputs.IsDdr5 ? 1 : 0);
  NumOfPulses = 3;

  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocDqsMaskPulseCnt, WriteToCache, &NumMaskedPulses);
  MrcGetSetChStrb (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_SDRAM_IN_DIMM, GsmIocDqsPulseCnt,     WriteToCache, &NumOfPulses);
  MrcFlushRegisterCachedData (MrcData);
}

/**
  Returns the currently configured DRAM Command Intput Rate NMode.

  @param[in] MrcData    - Include all MRC global data.

  @retval 1 = 1N Mode
  @retval 2 = 2N Mode
**/
UINT32
MrcGetNMode (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput         *Outputs;

  Outputs         = &MrcData->Outputs;

  return Outputs->Timing[MrcData->Inputs.ExtInputs.Ptr->MemoryProfile].NMode;
}

/**
  Configure the MC to issue multicycle CS_n MPC commands.
  The DRAM must be configured separately by either setting
  DDR5 MR2.OP[4] = 0 or by resetting the DRAM.

  @param[in] MrcData    - Include all MRC global data.

  @retval None
**/
void
EnableMcMulticycleCs (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput *Outputs;
  UINT32    Gear;
  UINT8     Controller;
  UINT8     Channel;
  UINT32    Offset;
  UINT32    NMode;
  INT64     MultiCycCmd;
  MC0_CH0_CR_TC_MPC_STRUCT TcMpc;

  Outputs           = &MrcData->Outputs;
  Gear              = Outputs->GearMode ? 4 : 2;
  NMode             = MrcGetNMode (MrcData);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      if (MrcChannelExist (MrcData, Controller, Channel)) {
        Offset = OFFSET_CALC_MC_CH(MC0_CH0_CR_TC_MPC_REG, MC1_CH0_CR_TC_MPC_REG, Controller, MC0_CH1_CR_TC_MPC_REG, Channel);
        TcMpc.Data = 0;
        TcMpc.Bits.tmpc_setup  = (UINT32) MrcGetDividedValueTck (MrcData, ((NMode == 2) ? (DDR5_tMC_MPC_SETUP_MIN + Gear): DDR5_tMC_MPC_SETUP_MIN));
        TcMpc.Bits.tmpc_hold   = (UINT32) MrcGetDividedValueTck (MrcData, DDR5_tMC_MPC_HOLD_MIN);
        TcMpc.Bits.tmpc_cs     = (UINT32) MrcGetDividedValueTck (MrcData, DDR5_tMPC_CS);
        MultiCycCmd             = 1;
        MrcWriteCR (MrcData, Offset, TcMpc.Data);
        MrcGetSetMcCh (MrcData, Controller, Channel, GsmMccMultiCycCmd, WriteCached, &MultiCycCmd);
      }
    } // Channel
  } // Controller
}

/**
  Configure the MC and DRAM for single cycle CS_n MPC commands.
  An MRW is issued to the DRAM to configure DDR5 MR2[4] = 1.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] McOnly     - TRUE: program MC side only; FALSE: program both MC and DRAM.

  @retval None
**/
VOID
DisableMcMulticycleCs (
  IN MrcParameters *const MrcData,
  IN const BOOLEAN        McOnly
  )
{
  MrcOutput         *Outputs;
  MrcControllerOut  *ControllerOut;
  MrcChannelOut     *ChannelOut;
  UINT32            Controller;
  UINT32            Channel;
  UINT32            Rank;
  UINT8             *MrPtr;
  UINT32            MrIndex;
  INT64             GetSetDis;
  DDR5_MODE_REGISTER_2_TYPE Mr2;
  UINT32        Dimm;
  UINT32        DimmRank;

  Outputs = &MrcData->Outputs;
  GetSetDis = 0;

  // Set the MC register
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GsmMccMultiCycCmd, WriteCached, &GetSetDis);

  if (McOnly) { // No need to program the DRAM side
    return;
  }

  MrIndex = mrIndexMR2;

  // Set the DRAM Mode Register
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    ControllerOut = &Outputs->Controller[Controller];
    for (Channel = 0; (Channel < Outputs->MaxChannels); Channel++) {
      ChannelOut = &ControllerOut->Channel[Channel];
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!(MrcRankExist (MrcData, Controller, Channel, Rank))) {
          continue;
        }
        Dimm = RANK_TO_DIMM_NUMBER (Rank);
        DimmRank = Rank % MAX_RANK_IN_DIMM;
        MrPtr = ChannelOut->Dimm[Dimm].Rank[DimmRank].MR;
        Mr2.Data8 = MrPtr[MrIndex];
        Mr2.Bits.CsAssertionDuration = 1; // Only a single cycle of CS assertion supported for MPC and VrefCA commands
        MrPtr[MrIndex] = Mr2.Data8;
        // Issue MRW to DDR5 MR2
        MrcIssueMrw (
          MrcData,
          Controller,
          Channel,
          Rank,
          mrMR2,
          Mr2.Data8,
          MRC_PRINTS_ON   // DebugPrint
          );
      }
    }
  }
}

/**
  Use MRS Burst FSM to program all relevant MR registers in one quick operation.
  Can only be used on ULT/ULX (up to 2 ranks, no ECC).
  Runs on all populated controllers, channels, and ranks.

  @param[in]  MrcData - Include all MRC global data.

  @retval mrcSuccess    - MRS FSM was executed successfully
  @retval mrcDeviceBusy - timed out waiting for MRH or MRS FSM
**/
MrcStatus
MrcProgramMrsFsm (
  IN MrcParameters *const MrcData
  )
{
  MrcStatus Status;
  MRC_STATISTIC Timer = MRC_MRS_TIME;

  MrcStatsStartTimer (MrcData, Timer);
  Status = WrappedMrcProgramMrsFsm (MrcData);
  MrcStatsEndTimer (MrcData, Timer);

  return Status;
}

/**
  Use MRS Burst FSM to program all relevant MR registers in one quick operation.
  Can only be used on ULT/ULX (up to 2 ranks, no ECC).
  Runs on all populated controllers, channels, and ranks.

  @param[in]  MrcData - Include all MRC global data.

  @retval mrcSuccess    - MRS FSM was executed successfully
  @retval mrcDeviceBusy - timed out waiting for MRH or MRS FSM
**/
MrcStatus
WrappedMrcProgramMrsFsm (
  IN MrcParameters *const MrcData
  )
{
  const MrcInput      *Inputs;
  MrcDebug            *Debug;
  const MRC_FUNCTION  *MrcCall;
  MrcOutput           *Outputs;
  MrcStatus           Status;
  UINT64              Timeout;
  UINT32              OffsetMrCommand;
  UINT32              OffsetMrsFsm;
  UINT32              Channel;
  UINT32              Controller;
  UINT32              IpChannel;
  BOOLEAN             Busy;
  BOOLEAN             IsLpddr;
  MC0_CH0_CR_DDR_MR_COMMAND_STRUCT  MrCommand;
  MC0_CH0_CR_MRS_FSM_RUN_STRUCT     MrsFsm;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  MrcCall = Inputs->Call.Func;
  Debug   = &Outputs->Debug;
  Status  = mrcSuccess;
  IsLpddr = Outputs->IsLpddr;

  // Make sure both MRH and MRS FSM are not busy
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      if ((!MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (IsLpddr, Channel)) {
        continue;
      }
      IpChannel = LP_IP_CH (IsLpddr, Channel);
      OffsetMrCommand = OFFSET_CALC_MC_CH (MC0_CH0_CR_DDR_MR_COMMAND_REG, MC1_CH0_CR_DDR_MR_COMMAND_REG, Controller, MC0_CH1_CR_DDR_MR_COMMAND_REG, IpChannel);
      OffsetMrsFsm = MrcGetMrsRunFsmOffset (MrcData, Controller, IpChannel);

      Timeout = MrcCall->MrcGetCpuTime () + MRC_WAIT_TIMEOUT;   // 10 seconds timeout
      do {
        MrCommand.Data = MrcReadCR (MrcData, OffsetMrCommand);
        MrsFsm.Data    = MrcReadCR (MrcData, OffsetMrsFsm);
        Busy = (MrCommand.Bits.Busy == 1) || (MrsFsm.Bits.Run == 1);
      } while (Busy && (MrcCall->MrcGetCpuTime () < Timeout));
      if (Busy) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Timed out waiting for previous MRH command to finish!\n");
        return mrcDeviceBusy;
      }
    } // Channel
  } // Controller

  // Start MRS FSM on both channels in parallel
  MrsFsm.Data = 0;
  MrsFsm.Bits.Run = 1;

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      if ((MrcChannelExist (MrcData, Controller, Channel)) && (!IS_MC_SUB_CH (IsLpddr, Channel))) {
        IpChannel = LP_IP_CH (IsLpddr, Channel);
        OffsetMrsFsm = MrcGetMrsRunFsmOffset (MrcData, Controller, IpChannel);
        MrcWriteCR (MrcData, OffsetMrsFsm, MrsFsm.Data);
      }
    } // Channel
  } // Controller

  // Poll for completion
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      if ((!MrcChannelExist (MrcData, Controller, Channel)) || IS_MC_SUB_CH (IsLpddr, Channel)) {
        continue;
      }
      IpChannel = LP_IP_CH (IsLpddr, Channel);
      OffsetMrsFsm = MrcGetMrsRunFsmOffset (MrcData, Controller, IpChannel);

      Timeout = MrcCall->MrcGetCpuTime () + MRC_WAIT_TIMEOUT;   // 10 seconds timeout
      do {
        MrsFsm.Data = MrcReadCR (MrcData, OffsetMrsFsm);
        Busy = (MrsFsm.Bits.Run == 1);
      } while (Busy && (MrcCall->MrcGetCpuTime () < Timeout));
      if (Busy) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Timed out waiting for MRS FSM finish!\n");
        return mrcDeviceBusy;
      }
    } // Channel
  } // Controller

  return Status;
}

/**
  This function sets up DRAM into CS Gear Down Mode

  @param[in, out] MrcData - Include all MRC global data.

  @retval mrcSuccess if DRAM set up correctly, otherwise mrcFail.
**/
MrcStatus
Ddr5EnableCsGearDown (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcStatus  Status;
  MrcOutput  *Outputs;
  INT64   MultiCycCmd;
  UINT32  FirstController;
  UINT32  FirstChannel;
  UINT32  tCK;
  UINT32  tMRD;
  UINT32  tSYNC_GEAR;
  UINT32  tCMD_GEAR;

  Outputs = &MrcData->Outputs;
  tCK = Outputs->MemoryClock;
  tMRD = MrcGetTmod (MrcData, tCK);
  FirstController = Outputs->FirstPopController;
  FirstChannel = Outputs->Controller[FirstController].FirstPopCh;
  MrcGetSetMcCh (MrcData, FirstController, FirstChannel, GsmMccMultiCycCmd, ReadFromCache, &MultiCycCmd);

  tSYNC_GEAR = (tMRD + 4) * tCK;
  tSYNC_GEAR = DIVIDECEIL (tSYNC_GEAR, FEMTOSECONDS_PER_NANOSECOND);

  tCMD_GEAR = tMRD * tCK;
  tCMD_GEAR = DIVIDECEIL (tCMD_GEAR, FEMTOSECONDS_PER_NANOSECOND);

  Status = MrcIssueMpc (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_RANK_IN_CHANNEL, DDR5_MPC_CS_GEARDOWN_ENABLE, TRUE);
  if (Status != mrcSuccess) {
    return Status;
  }

  // Only change MC register to send 2-cycle NOP
  DisableMcMulticycleCs (MrcData, TRUE);

  // Wait tSYNC_GEAR
  MrcWait (MrcData, tSYNC_GEAR);

  Status = MrcIssueNop (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_RANK_IN_CHANNEL, TRUE);
  if (Status != mrcSuccess) {
    return Status;
  }

  // Wait tCMD_GEAR
  MrcWait (MrcData, tCMD_GEAR);

  if (MultiCycCmd != 0) {
    EnableMcMulticycleCs (MrcData);
  }

  return Status;
}

/**
  This function sets up MC, DDRIO and DRAM registers into CA 2N or 1N mode

  @param[in, out] MrcData - Include all MRC global data.
  @param[in]     IsEnable - Whether it is 2N (MRC_ENABLE) or 1N (MRC_DISABLE).

  @retval mrcSuccess setting Ca2N in the MC, DDRIO, and DRAM is successful
**/
MrcStatus
SetCa2N (
  IN OUT MrcParameters *const MrcData,
  IN     BOOLEAN              IsEnable
  )
{
  MrcStatus  Status;
  UINT32     MpcOpCode;
  INT64      GetSetVal;
  BOOLEAN    IsPrint;
  UINT8      Group;

  IsPrint = TRUE;
  MpcOpCode = (IsEnable) ? DDR5_MPC_SET_2N_COMMAND_TIMING : DDR5_MPC_SET_1N_COMMAND_TIMING;

  Status = MrcIssueMpc (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MAX_RANK_IN_CHANNEL, MpcOpCode, IsPrint);

  GetSetVal = (IsEnable) ? 1 : 0;
  MrcGetSetNoScope (MrcData, GsmIocDdr52NMode, WriteCached, &GetSetVal);

  for (Group = 0; Group < MRC_DDR5_CMD_GRP_MAX; Group++) {
    GetSetVal = (IsEnable) ? (-64) : 64;
    MrcGetSetCcc (MrcData, MAX_CONTROLLER, MAX_CHANNEL, MRC_IGNORE_ARG, Group, CmdGrpPi, ForceWriteOffsetCached, &GetSetVal);
  }

  McCa2NEnableOrDisable (MrcData, IsEnable);

  return Status;
}

/**
  Configure the DRAM for multi cycle CS_n MPC commands.
  An MRW is issued to the DRAM to configure DDR5 MR2[4] = 0.

  @param[in] MrcData    - Include all MRC global data.
**/
VOID
EnableDdr5MulticycleCs (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput         *Outputs;
  MrcControllerOut  *ControllerOut;
  MrcChannelOut     *ChannelOut;
  UINT32            Controller;
  UINT32            Channel;
  UINT32            Rank;
  UINT32            Dimm;
  UINT32            DimmRank;
  DDR5_MODE_REGISTER_2_TYPE *Mr2Ptr;

  Outputs = &MrcData->Outputs;

  // Set the DRAM Mode Register
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    ControllerOut = &Outputs->Controller[Controller];
    for (Channel = 0; (Channel < Outputs->MaxChannels); Channel++) {
      ChannelOut = &ControllerOut->Channel[Channel];
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        if (!(MrcRankExist (MrcData, Controller, Channel, Rank))) {
          continue;
        }
        Dimm = RANK_TO_DIMM_NUMBER (Rank);
        DimmRank = Rank % MAX_RANK_IN_DIMM;
        Mr2Ptr = (DDR5_MODE_REGISTER_2_TYPE *) &ChannelOut->Dimm[Dimm].Rank[DimmRank].MR[mrIndexMR2];
        Mr2Ptr->Bits.CsAssertionDuration = 0; // Multi cycle of CS assertion supported for MPC and VrefCA commands
        // Issue MRW to DDR5 MR2
        MrcIssueMrw (
          MrcData,
          Controller,
          Channel,
          Rank,
          mrMR2,
          (UINT16) Mr2Ptr->Data8,
          MRC_PRINTS_ON   // DebugPrint
          );
      }
    }
  }
}

/**
  This function sets workpoint data for PHY.

  @param[in, out] MrcData - MRC global data.

  @retval MrcStatus - mrcSuccess if workpoint data is set correctly, otherwise an error status.
**/
MrcStatus
SetWorkPointData (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcSetWorkPointDataForPhy(MrcData);
  return MrcSetWorkpointCR(MrcData);
}
