/** @file
  This file is used as a driver to all inter-agent communications from Green MRC.
  It includes all the functions that the Green MRC needs to send/receive data.

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
//
// Include files
//
#include "MrcBlueGreenCommunication.h"
#include "MrcUcManagement.h"
#include "MrcUcPayloadData.h"
#include "BlueMrcAgentCommunication.h"
#include "MrcPmaApiCrossProj.h"
#include "MrcGeneral.h"
#include "MrcSagv.h"
#include "MrcCommon.h"
#include "DecompressLib.h"

#ifndef FULL_HEADLESS

/**
  Soft-reset the UC.

  @param[in] MrcData - Include all MRC global data.

  @retval passes through BlueMrcXtensaSoftReset's return value.
**/
MrcStatus
MrcUcReset (
  IN MrcParameters *const MrcData
  )
{
  MrcStatus Status;
  UCSS_SRAM_CR_DDRUCSS_CR_UCSS_ECC_CTRL_STRUCT UcssEccCtrl;

  Status = BlueMrcXtensaSoftReset (MrcData);

  if (MrcData->Inputs.IsXtensaEccDisabled) {
    // Disable UCSS ECC for NE and FE SRAM
    UcssEccCtrl.Data = MrcReadCR (MrcData,  UCSS_SRAM_CR_DDRUCSS_CR_UCSS_ECC_CTRL_REG);
    UcssEccCtrl.Bits.fesram_ecc_en   = 0;
    UcssEccCtrl.Bits.nesram_ecc_en   = 0;
    UcssEccCtrl.Bits.tracemem_ecc_en = 0;
    MrcWriteCR (MrcData, UCSS_SRAM_CR_DDRUCSS_CR_UCSS_ECC_CTRL_REG, UcssEccCtrl.Data);
  }

  return Status;
}

/**
  Load the Green MRC payload into the UC's memory.

  @param[in] MrcData - Include all MRC global data.

  @retval mrcFail if no Green MRC binary, mrcSuccess otherwise.
**/
MrcStatus
MrcUcLoadGreenCode (
  IN MrcParameters *const MrcData
  )
{
  const MrcInput  *Inputs;
  MrcStatus       Status;
  MrcOutput       *Outputs;
  MrcDebug        *Debug;
  BOOLEAN         IsSaGvAndNotFirst;
  UINT32          Index;
  UINT32          StartSection;
  UINT32          EndSection;
  UINTN           Payload;
  UINT8           DecompBuffer[DECOMP_BUFFER_SIZE];
  UINT32          SectionIndex;
  UINT32          TotalIndex;
  UINT8           *Chunk;
  UINT32          ChunkSize;
  UINT32          SizeToWrite;
  UINT32          BufferIndex;
  BOOLEAN         ClearBuffer;
  // BOOLEAN         SkipLoad;
  UINTN           BytesWritten;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  IsSaGvAndNotFirst = MrcIsSaGvEnabled (MrcData) && (Outputs->SaGvPoint != Outputs->SaGvFirst);

  Status = mrcSuccess;
  if (!IsSaGvAndNotFirst) {
    Status = BlueMrcClearXtensaSram (MrcData);
    if (Status == mrcSuccess) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "SRAM preload done\n");
    } else {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s SRAM preload timeout!\n", gErrString);
      return Status;
    }
  }
  Payload = ((UINTN) Inputs->UcPayloadAddress) + sizeof (MRC_PAYLOAD_HEADER);
  if (Inputs->BootMode == bmCold) {
    StartSection = 0;
    EndSection = MRC_GREEN_PAYLOAD_END;
  } else {
    StartSection = MRC_FAST_PAYLOAD_START;
    EndSection = MRC_FAST_PAYLOAD_END;
  }

  BufferIndex = 0;
  // SkipLoad = FALSE;
  ClearBuffer = FALSE;
  ChunkSize = *(UINT32*)(Payload);
  Chunk = ((UINT8*)Payload) + sizeof(UINT32); // First chunk is at start, skip its size
  TotalIndex = 0;
  if (MRC_UNCOMPRESSED_CHUNK_SIZE == 0) { // Use original code for uncompressed payload
    for (Index = StartSection; Index < EndSection; Index++) {
      if (IsSaGvAndNotFirst && (gUcPayloadSections[Index].Address < XTENSA_FESRAM_BASE)) {
        // No need to download NE SRAM on subsequent SAGV points
        // continue;
      }
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Loading Green MRC segment %2d to SRAM address 0x%08x size 0x%08x\n", Index, gUcPayloadSections[Index].Address, gUcPayloadSections[Index].Size);
      BlueMrcWriteUcData (
        MrcData,
        (UINT32 *) (UINTN) (Payload + gUcPayloadSections[Index].Offset),
        gUcPayloadSections[Index].Address, // Function will convert from Xtensa address space into UCSS address space
        gUcPayloadSections[Index].Size);
    }

    // check errors reported to UCSS during FW download
    return BlueMrcCheckXtStatusRegs(MrcData);
  }

  for (Index = StartSection; Index < EndSection; Index++) { // Compressed payload requires decompressing chunks
    if (IsSaGvAndNotFirst && (gUcPayloadSections[Index].Address < XTENSA_FESRAM_BASE)) {
      // No need to download NE SRAM on subsequent SAGV points, but decompression is in-order
      // SkipLoad = TRUE;
    } else {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Loading Green MRC segment %2d to SRAM address 0x%08x size 0x%08x, payload offset 0x%08x\n", Index, gUcPayloadSections[Index].Address, gUcPayloadSections[Index].Size, gUcPayloadSections[Index].Offset);
    }

    SectionIndex = 0;
    while (SectionIndex < gUcPayloadSections[Index].Size) { // While section end not reached
      if (BufferIndex == 0) { // If no remaining data, decomp a chunk
        if (MrcDecompress (Chunk, ChunkSize, DecompBuffer, DECOMP_BUFFER_SIZE, &BytesWritten) != 0) {
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Green MRC binary decompression failure!\n");
          return mrcFail;
        }
        Chunk = Chunk + ChunkSize + sizeof(UINT32); // Get next chunk
        ChunkSize = *(UINT32*)(Chunk - sizeof(UINT32)); // Get next size
      }
      if (gUcPayloadSections[Index].Offset > TotalIndex) {
        BufferIndex += gUcPayloadSections[Index].Offset - TotalIndex; // Skip ahead
        TotalIndex = gUcPayloadSections[Index].Offset;
      }
      SizeToWrite = gUcPayloadSections[Index].Size - SectionIndex;
      if (SizeToWrite >= ((MRC_UNCOMPRESSED_CHUNK_SIZE) - BufferIndex)) { // If section uses rest of buffer {
        SizeToWrite = (MRC_UNCOMPRESSED_CHUNK_SIZE) - BufferIndex; // Don't go past chunk
        ClearBuffer = TRUE;
      }

      // if (SkipLoad) {
      //   SkipLoad = FALSE; // Make sure to check next 
      // } else {
      BlueMrcWriteUcData ( // Write from decomp buffer
        MrcData,
        (UINT32 *) ((DecompBuffer + BufferIndex)),
        gUcPayloadSections[Index].Address + SectionIndex, // Function will convert from Xtensa address space into UCSS address space
        SizeToWrite);
      // }

      SectionIndex = SectionIndex + SizeToWrite;
      TotalIndex = TotalIndex + SizeToWrite;
      if (ClearBuffer) {
        BufferIndex = 0; // Used up whole buffer
        ClearBuffer = FALSE;
      } else {
        BufferIndex = BufferIndex + SizeToWrite; // Buffer remains, move forward
      }
    }
  }

  // check errors reported to UCSS during FW download
  return BlueMrcCheckXtStatusRegs(MrcData);
}

/**
  Load the MRC data structure used by Green MRC into the UC's memory.

  @param[in] MrcData - Include all MRC global data.

  @retval passes through BlueMrcWriteUcData's return value, or mrcFail if payload is too large.
**/
MrcStatus
MrcUcLoadGreenData (
  IN MrcParameters *const MrcData
  )
{
  MrcStatus    Status;
  MrcOutput    *Outputs;
  UINT32       DataSize;
  BOOLEAN      IsSaGvAndNotFirst;
  MRC_EXT_INPUTS_TYPE *ExtInputs;

  Outputs     = &MrcData->Outputs;
  ExtInputs   = MrcData->Inputs.ExtInputs.Ptr;

  IsSaGvAndNotFirst = MrcIsSaGvEnabled (MrcData) && (Outputs->SaGvPoint != Outputs->SaGvFirst);

  // Clear the Cache
  MrcFlushRegisterCachedData (MrcData);
  MrcInvalidateRegisterCachedData (MrcData);

  // Last part of MrcData (which is part of MrcSave) is not needed in Green MRC, hence do not load it
  DataSize = sizeof (MrcParameters) - (sizeof (MrcSaveData) - OFFSET_OF (MrcSaveData, RegSaveCommon));

  // Function will convert destination address from Xtensa address space into UCSS address space
  MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "Loading MrcData to SRAM address 0x%08x size 0x%08x\n", gUcMrcDataAddress, DataSize);
  if (IsSaGvAndNotFirst) {
    // No zero preload, hence cannot skip zeroes
    Status = BlueMrcWriteUcData (MrcData, (UINT32 *) MrcData, gUcMrcDataAddress, DataSize);
  } else {
    Status = BlueMrcWriteUcDataSkipZeroes (MrcData, (UINT32 *) MrcData, gUcMrcDataAddress, DataSize);
  }

  MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "Loading ExtInputs to SRAM address 0x%08x size 0x%" PRIxPTR "\n", gUcExtInputsAddress, sizeof (MRC_EXT_INPUTS_TYPE));
  if (IsSaGvAndNotFirst) {
    // No zero preload, hence cannot skip zeroes
    Status |= BlueMrcWriteUcData (MrcData, (UINT32 *) ExtInputs, gUcExtInputsAddress, sizeof (MRC_EXT_INPUTS_TYPE));
  } else {
    Status |= BlueMrcWriteUcDataSkipZeroes (MrcData, (UINT32 *) ExtInputs, gUcExtInputsAddress, sizeof (MRC_EXT_INPUTS_TYPE));
  }

  if (Status != mrcSuccess) {
    return Status;
  }
  // check errors reported to UCSS during Data Write
  return BlueMrcCheckXtStatusRegs (MrcData);
}

/**
  Execute the Green MRC payload currently loaded in the UC.

  @param[in] MrcData - Include all MRC global data.

  @retval Failed Green MRC task result, if any, mrcFail if Blue/Green comm issue, mrcSuccess otherwise.
**/
MrcStatus
MrcUcExecuteGreen (
  IN MrcParameters *const MrcData
  )
{
  const MRC_FUNCTION        *MrcCall;
  MrcDebug                  *Debug;
  MrcOutput                 *Outputs;
  MrcStatus                 Status;
  MrcStatus                 HaltStatus;
  MrcStatus                 UpdateMrcDataStatus;
  MrcBlueGreenCommunication CommBuffer;
  UINT64                    Timeout;
  BOOLEAN                   GreenDone;
  MrcPostCode               PostCode;
  MrcPrintBuf               PrintBuf;
  INT32                     Temperature;
  UINT32                    Address;
  RETURN_STATUS             SmbusStatus;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  MrcCall = MrcData->Inputs.Call.Func;
  PostCode = 0;

  MrcCall->MrcSetMem ((UINT8 *)&CommBuffer, sizeof (CommBuffer), 0);

  CommBuffer.Misc.Bits.ExecutingCommand = Outputs->IsInterpreterCommand;
    // Load the initial CMOS breakpoint.
    CommBuffer.TaskControl.PostCodes.Stop = (MrcCall->MrcRtcCmos (MRC_POST_CODE_HIGH_BYTE_ADDR) << 8) | MrcCall->MrcRtcCmos (MRC_POST_CODE_LOW_BYTE_ADDR);

  BlueMrcWriteUcCommBuffer (MrcData, &CommBuffer);
  Status = BlueMrcSetXtensaFwDownloadDone (MrcData);
  if (Status != mrcSuccess) {
    return Status;
  }
  Status = BlueMrcSetXtensaHardwareHaltState (MrcData, MRC_XTENSA_HW_UNHALT);
  if (Status != mrcSuccess) {
    return Status;
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Xtensa is running\n");

  // restore debug level for prints on Blue side for interpreter flow
  if (CommBuffer.Misc.Bits.ExecutingCommand) {
    MrcData->Outputs.Debug.Level = Outputs->InterpreterOrigDbgLvl;
  }

  Timeout = MrcCall->MrcGetCpuTime() + MRC_GREEN_TIMEOUT * 10;
  //
  // Loop until timeout expires or status is done/error,
  // checking and printing log data while we wait.
  //
  do {
    MrcWait (MrcData, MRC_BLUE_WAIT_CYCLE * MRC_TIMER_1NS);

    if (MrcData->Inputs.IsCrBasedCommunication) {
      MrcCall->MrcSetMem ((UINT8 *) &CommBuffer, sizeof (CommBuffer), 0);
      PrintBuf.Data = MrcReadCR (MrcData, MRC_PRINT_BUF_CR_REG);
      if (PrintBuf.Bits.Size == MRC_CR_BASED_MRC_DONE) {
        CommBuffer.Misc.Bits.GreenDone = TRUE;
        CommBuffer.GreenStatus = PrintBuf.Bits.Byte0;
        MrcWriteCR (MrcData, MRC_PRINT_BUF_CR_REG, 0);    // Clean out the communication CR for the next SAGV point
      } else if (PrintBuf.Bits.Size == MRC_CR_BASED_HIT_ENTER) {
        CommBuffer.Misc.Bits.DoHitEnter = TRUE;
      } else {
        CommBuffer.LogSize = (UINT16) PrintBuf.Bits.Size;
      }
    } else {
      // Read just the buffer header first, to see the LogSize. No need to fetch the whole LogBuffer.
      BlueMrcReadUcData (MrcData, gUcCommBufferAddress, (UINT32 *) (UINTN) &CommBuffer, MRC_BLUE_GREEN_COMM_CONTROLS_SIZE);
    }
    GreenDone = (BOOLEAN) CommBuffer.Misc.Bits.GreenDone;

    // Check if Green sent any log and print it
    if (CommBuffer.LogSize > 0) {
      BlueMrcPrintGreenLog (MrcData, CommBuffer.LogSize);
    }
    // Call DebugHook if PostCode has changed
    if (CommBuffer.TaskControl.PostCodes.Start != PostCode) {
      PostCode = CommBuffer.TaskControl.PostCodes.Start;
      // MrcDebugHook will update the CommBuffer's StopPostCode for the next CMOS breakpoint if the current breakpoint was triggered
      MrcCall->MrcDebugHook (MrcData, PostCode);
    }
    if (CommBuffer.Misc.Bits.DoToggleVoltage) {
      // Toggle VDDQ for DVFSQ
      Outputs->VccddqVoltage = (Outputs->VccddqVoltage == VDD_0_50) ? VDD_0_30 : VDD_0_50;
      MrcSendPmMessage (MrcData, PM_MESSAGE_0_VOLTAGE_ONLY, PM_MSG_MC);

      // Pass control back to Green
      CommBuffer.Misc.Bits.DoToggleVoltage = FALSE;
      BlueMrcWriteUcCommBuffer (MrcData, &CommBuffer);
    }
    if (CommBuffer.Misc.Bits.DoReadTemperature) {
      // Read PHY temperature
      MrcReadTemperature (MrcData, &Temperature);

      // Pass control back to Green
      CommBuffer.Misc.Bits.DoReadTemperature = FALSE;
      CommBuffer.Misc.Bits.PhyTemperature = Temperature;
      BlueMrcWriteUcCommBuffer (MrcData, &CommBuffer);
    }
    if (CommBuffer.SmbusTransaction.Bits.RequestType != MrcNoPendingSmbusRequest) {
      // Perform Smbus transaction
      Address = CommBuffer.SmbusTransaction.Bits.Address;
      if (CommBuffer.SmbusTransaction.Bits.RequestType == MrcSmbusWriteRequest) {
        MrcCall->MrcSmbusWrite8 (Address, (UINT8) CommBuffer.SmbusTransaction.Bits.Value, &SmbusStatus);
      } else {
        CommBuffer.SmbusTransaction.Bits.Value = MrcCall->MrcSmbusRead8 (Address, &SmbusStatus);
      }

      // Pass control back to Green
      CommBuffer.SmbusTransaction.Bits.Status = (SmbusStatus == mrcSuccess) ? MrcSmbusRequestStatusPass : MrcSmbusRequestStatusFail;
      CommBuffer.SmbusTransaction.Bits.RequestType = MrcNoPendingSmbusRequest;
      BlueMrcWriteUcCommBuffer (MrcData, &CommBuffer);
    }
#ifdef MRC_DEBUG_PRINT
    if (CommBuffer.Misc.Bits.DoHitEnter) {
      MrcHitEnter (MrcData, "Blue MRC: Do MrcHitEnter from Green");

      // Pass control back to Green
      CommBuffer.Misc.Bits.DoHitEnter = FALSE;
      BlueMrcWriteUcCommBuffer (MrcData, &CommBuffer);
      if (MrcData->Inputs.IsCrBasedCommunication) {
        MrcWriteCR (MrcData, MRC_PRINT_BUF_CR_REG, 0);
      }
    }
#endif // MRC_DEBUG_PRINT
    // check errors reported to UCSS during Code execution
    Status = BlueMrcCheckXtStatusRegs(MrcData);
    if (Status != mrcSuccess) {
      return Status;
    }
  } while (!GreenDone && (MrcCall->MrcGetCpuTime() <= Timeout));
  Status = (MrcStatus) CommBuffer.GreenStatus;

  if (!GreenDone) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Green MRC Timeout\n", gErrString);
    Status = mrcTimeout;
  }
  HaltStatus = BlueMrcSetXtensaHardwareHaltState (MrcData, MRC_XTENSA_HW_HALT);
  if (HaltStatus != mrcSuccess) {
    return HaltStatus;
  }

  UpdateMrcDataStatus = MrcUpdateDataFromUc (MrcData);
  if (UpdateMrcDataStatus != mrcSuccess) {
    return UpdateMrcDataStatus;
  }

  return Status;
}

/**
  Copy the Green MRC's MrcData updates back to Blue MRC.

  @param[in] MrcData - Include all MRC global data.

  @retval mrcSuccess.
**/
MrcStatus
MrcUpdateDataFromUc (
  IN MrcParameters *const MrcData
  )
{
  MrcSaveData   *Save;
  MrcOutput     *Output;
  MrcInput      *Inputs;
  MrcVersion    *Version;
  UINT64        *TscTimeBase;
  UINT32        SaveOffset;
  UINT32        VersionOffset;
  UINT32        InputOffset;
  UINT32        SaveSizeToCopy;
  UINT32        OutputSizeToCopy;
  UINT32        InputSizeToCopy;

  Save        = &MrcData->Save.Data;
  Output      = &MrcData->Outputs;
  Inputs      = &MrcData->Inputs;
  Version     = &Output->Version;
  TscTimeBase = &Inputs->TscTimeBase;

  SaveOffset      = gUcMrcDataAddress  + OFFSET_OF (MrcParameters, Save.Data);
  VersionOffset   = gUcMrcDataAddress  + OFFSET_OF (MrcParameters, Outputs) + OFFSET_OF (MrcOutput, Version);
  InputOffset     = gUcMrcDataAddress  + OFFSET_OF (MrcParameters, Inputs) + OFFSET_OF (MrcInput, TscTimeBase);

  SaveSizeToCopy   = OFFSET_OF (MrcSaveData, RegSaveCommon); // Last part of MrcSave is not used in Green MRC, hence do not copy it (RegSaveCommon, SaGvRegSave, Controller and SaGvOutputs)
  OutputSizeToCopy = OFFSET_OF (MrcOutput, RegisterCache) - OFFSET_OF (MrcOutput, Version); // Skipped past debug member, Stop at register cache
  InputSizeToCopy  = OFFSET_OF (MrcInput, Call) - sizeof (Inputs->ExtInputs); // Do not copy the ExtInputs pointer, Stop at Inputs->Call

  BlueMrcReadUcData (MrcData, SaveOffset,      (UINT32 *) Save,        SaveSizeToCopy);
  BlueMrcReadUcData (MrcData, VersionOffset,   (UINT32 *) Version,     OutputSizeToCopy);
  BlueMrcReadUcData (MrcData, InputOffset,     (UINT32 *) TscTimeBase, InputSizeToCopy);
  MrcInvalidateRegisterCachedData (MrcData);

  // check errors reported to UCSS during Data Read
  return BlueMrcCheckXtStatusRegs(MrcData);
}

#endif // FULL_HEADLESS
