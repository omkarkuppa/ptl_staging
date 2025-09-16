/** @file
  Sndw Audio Initialization Library for controller initialization

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#include <Base.h>
#include <Uefi.h>
#include <IndustryStandard/Pci30.h>
#include <Protocol/PciIo.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiDxeAceCommonLib.h>
#include <Library/PeiDxeSndwInitLib.h>
#include <Register/AceRegs.h>
#include <Register/SndwCadenceRegs.h>
#include <SndwMipiCmd.h>
#include <SndwAccess.h>

#define SNDW_MAX_RETRY_NUMBER         10

GLOBAL_REMOVE_IF_UNREFERENCED SNDW_COMMAND mMipiSndwReadPeripheralIdsCmds[] = {
  {
    .TxRead = {
      .OpCode = SndwCmdRead,
      .RegAddr = R_PCP_DevId_0,
    }
  },
  {
    .TxRead = {
      .OpCode = SndwCmdRead,
      .RegAddr = R_PCP_DevId_1,
    }
  },
  {
    .TxRead = {
      .OpCode = SndwCmdRead,
      .RegAddr = R_PCP_DevId_2,
    }
  },
  {
    .TxRead = {
      .OpCode = SndwCmdRead,
      .RegAddr = R_PCP_DevId_3,
    }
  },
  {
    .TxRead = {
      .OpCode = SndwCmdRead,
      .RegAddr = R_PCP_DevId_4,
    }
  },
  {
    .TxRead = {
      .OpCode = SndwCmdRead,
      .RegAddr = R_PCP_DevId_5,
    }
  }
};

/**
  Print response message from Sndw codec

  @param[in] RxCommands           Pointer to Sndw message
**/
STATIC
VOID
SndwPrintRxMsg (
  IN  SNDW_COMMAND*               RxCommands
  )
{
  DEBUG ((DEBUG_INFO, "Sndw Rx Response\n"));
  DEBUG ((DEBUG_INFO, "OpCode: %d\n",      RxCommands->Rx.OpCode));
  DEBUG ((DEBUG_INFO, "Data  : 0x%X\n",    RxCommands->Rx.Data));
  DEBUG ((DEBUG_INFO, "Ack   : %d\n",      RxCommands->Rx.Ack));
  DEBUG ((DEBUG_INFO, "Nak   : %d\n",      RxCommands->Rx.Nak));
}


/**
  Print message for Sndw codec

  @param[in] TxCommands           Pointer to Sndw message
**/
STATIC
VOID
SndwPrintTxMsg (
  IN  SNDW_COMMAND*               TxCommands
  )
{
  DEBUG ((DEBUG_INFO, "Sndw Tx Commands\n"));
  DEBUG ((DEBUG_INFO, "SspTag                : %d\n",     TxCommands->Tx.SspTag));
  DEBUG ((DEBUG_INFO, "OpCode                : %d\n",     TxCommands->Tx.OpCode));
  DEBUG ((DEBUG_INFO, "Peripheral Address    : 0x%X\n",   TxCommands->Tx.DeviceAddress));
  switch (TxCommands->Tx.OpCode) {
    case SndwCmdRead:
      DEBUG ((DEBUG_INFO, "Register Address : 0x%X\n", TxCommands->TxRead.RegAddr));
      break;
    case SndwCmdWrite:
      DEBUG ((DEBUG_INFO, "Register Address : 0x%X\n", TxCommands->TxWrite.RegAddr));
      DEBUG ((DEBUG_INFO, "Register Data    : 0x%X\n", TxCommands->TxWrite.RegData));
      break;
    default:
      DEBUG ((DEBUG_INFO, "End of command unavailable.\n"));
      break;
  }
}

/**
  Function operating on Sndw Fifo for sending message to codecs.

  @param[in]  SndwControllerSpace Sndw controller memory space address
  @param[in]  TxCommands          Pointer to send messages
  @param[in]  NumOfMessages       Size of messages to send
**/
VOID
Send (
  IN  UINTN                       SndwControllerSpace,
  IN  SNDW_COMMAND                *TxCommands,
  IN  UINTN                       NumOfMessages
  )
{
  UINTN                           Index;
  UINT32                          FifoStatusFree;
  UINTN                           TxIndex;
  UINT32                          Val;

  TxIndex = 0;

  FifoStatusFree = (UINT32) ((MmioRead32 ((UINTN) SndwControllerSpace + R_SNDW_MEM_FIFOSTAT) & B_SNDW_MEM_FIFOSTAT_FREE) >> N_SNDW_MEM_FIFOSTAT_FREE);

  for (Index = 0; Index < FifoStatusFree; ++Index) {
    if (TxIndex >= NumOfMessages) {
      break;
    }
    Val = TxCommands[TxIndex++].Command;
    SndwPrintTxMsg ((SNDW_COMMAND *) &Val);
    MmioWrite32 ((UINTN) SndwControllerSpace + R_SNDW_MEM_COMMAND, Val);
  }
}

/**
  Function operating on Sndw Fifo for receiving messages from codecs.

  @param[in]  SndwControllerSpace Sndw controller memory space address
  @param[out] RxCommands          Pointer to pointer to received messages.
                                  Memory pool is allocated in this function,
                                  but the caller is responsible for
                                  freeing the memory with FreePool.
  @param[out] RxSize              Size of received messages
**/
VOID
Receive (
  IN  UINTN                       SndwControllerSpace,
  OUT SNDW_COMMAND                **RxCommands,
  OUT UINTN*                      RxSize
  )
{
  UINTN                           Index;
  UINT32                          FifoStatusAvailable;
  UINTN                           RxIndex;
  UINT32                          Val;

  RxIndex = 0;

  FifoStatusAvailable = (UINT32) ((MmioRead32 ((UINTN) SndwControllerSpace + R_SNDW_MEM_FIFOSTAT) & B_SNDW_MEM_FIFOSTAT_AVAIL) >> N_SNDW_MEM_FIFOSTAT_AVAIL);

  *RxCommands = (SNDW_COMMAND *) AllocateZeroPool ((sizeof (SNDW_COMMAND)) * FifoStatusAvailable);
  if (*RxCommands == NULL) {
    DEBUG ((DEBUG_ERROR, "Memory allocation failed.\n"));
    return;
  }

  for (Index = 0; Index < FifoStatusAvailable; ++Index) {

    Val = (UINT32) MmioRead32 ((UINTN) SndwControllerSpace + R_SNDW_MEM_COMMAND);
    SndwPrintRxMsg ((SNDW_COMMAND *) &Val);
    (*RxCommands)[RxIndex++].Command = Val;
  }

  *RxSize = RxIndex;

  //
  // Clear interrupt flag
  //
  MmioOr32 ((UINTN) SndwControllerSpace + R_SNDW_MEM_INTMASK, B_SNDW_MEM_INTMASK_RXNE);
}


/**
  Function sends one message through the Sndw interface and waits
  for the corresponding ACK message.

  @param[in]  SndwControllerSpace Sndw controller memory space address
  @param[in]  TxCommands          Message to send
  @param[out] RxCommands          Pointer to receiving message

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_TIMEOUT             Sending command has failed
  @retval EFI_DEVICE_ERROR        Response command has failed
**/
EFI_STATUS
SendwAck (
  IN  UINTN                       SndwControllerSpace,
  IN  SNDW_COMMAND                TxCommand,
  OUT SNDW_COMMAND*               RxCommand OPTIONAL
  )
{
  EFI_STATUS                      Status;
  SNDW_COMMAND                    TmpRxCommand;

  Send (SndwControllerSpace, &TxCommand, 1);

  Status = StatusPolling (
             (UINTN) SndwControllerSpace + R_SNDW_MEM_STAT,
             B_SNDW_MEM_STAT_RXNE,
             (UINT32) B_SNDW_MEM_STAT_RXNE_FIFO_EMPTY
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Sending command failed. No response from codec. Status = %r\n", Status));
    return Status;
  }

  TmpRxCommand.Command = (UINT32) MmioRead32 ((UINTN) SndwControllerSpace + R_SNDW_MEM_COMMAND);
  SndwPrintRxMsg ((SNDW_COMMAND *) &TmpRxCommand);

  if (RxCommand != NULL) {
    CopyMem (RxCommand, &TmpRxCommand, sizeof (SNDW_COMMAND));
  }

  return EFI_SUCCESS;
}


/**
  Function asks connected codec about identification information.

  @param[in]  SndwControllerSpace      Sndw controller memory space address
  @param[in]  SndwPeripheralAddress    Peripheral address
  @param[in]  SndwCodecId              Pointer to Sndw codec information structure

  @retval EFI_SUCCESS                  The function completed successfully
  @retval EFI_TIMEOUT                  Not able to read ids
  @retval EFI_DEVICE_ERROR             Responses from codec are incorrect
**/
EFI_STATUS
ReadPeripheralId (
  IN UINTN                             SndwControllerSpace,
  IN UINTN                             SndwPeripheralAddress,
  OUT SNDW_CODEC_ID                    *SndwCodecId
  )
{
  EFI_STATUS                           Status;
  SNDW_COMMAND*                        RxCommands;
  UINTN                                FifoStatusAvailable;
  UINTN                                RetryNumber;
  UINTN                                RxSize;
  UINTN                                Index;
  UINT32                               LoopTime;
  BOOLEAN                              Ack;

  Status      = EFI_SUCCESS;
  RetryNumber = 0;
  RxSize      = 0;
  RxCommands  = NULL;
  Ack         = TRUE;

  for (Index = 0; Index < ARRAY_SIZE (mMipiSndwReadPeripheralIdsCmds); Index++) {
    mMipiSndwReadPeripheralIdsCmds[Index].TxRead.DeviceAddress = (UINT32) SndwPeripheralAddress;
  }

  do {
    if (RetryNumber > SNDW_MAX_RETRY_NUMBER) {
      Status = EFI_DEVICE_ERROR;
      break;
    }

    Send (SndwControllerSpace, (SNDW_COMMAND *) &mMipiSndwReadPeripheralIdsCmds, ARRAY_SIZE (mMipiSndwReadPeripheralIdsCmds));

    for (LoopTime = 0; LoopTime < HDA_MAX_LOOP_TIME; LoopTime++) {
      FifoStatusAvailable = ((MmioRead32 ((UINTN) SndwControllerSpace + R_SNDW_MEM_FIFOSTAT) & B_SNDW_MEM_FIFOSTAT_AVAIL) >> N_SNDW_MEM_FIFOSTAT_AVAIL);
      if (ARRAY_SIZE (mMipiSndwReadPeripheralIdsCmds) == FifoStatusAvailable) {
        break;
      } else {
        MicroSecondDelay (HDA_WAIT_PERIOD);
      }
    }

    if (LoopTime >= HDA_MAX_LOOP_TIME) {
      RetryNumber++;
      continue;
    }

    Receive (SndwControllerSpace, &RxCommands, &RxSize);

    if (RxCommands == NULL) {
      Status = EFI_DEVICE_ERROR;
      continue;
    }

    Ack = TRUE;

    for (Index = 0; Index < RxSize; Index++) {
      if (RxCommands[Index].Rx.Ack == 0) {
        Ack = FALSE;
        RetryNumber++;
        break;
      }

      SndwCodecId->Data[Index] = (UINT8) RxCommands[Index].Rx.Data;
    }

    FreePool (RxCommands);
  } while (Ack == FALSE);


  return Status;
}


/**
  Function initializes Sndw controller for enumerating connected codecs

  @param[in]  SndwControllerMmioAddress       Sndw controller memory space address

  @retval EFI_SUCCESS                         Sndw controller initialization completed successfully
  @retval EFI_TIMEOUT                         Polling FIFO status timed out
**/
EFI_STATUS
SndwControllerInit (
  IN UINTN                                    SndwControllerMmioAddress
  )
{
  EFI_STATUS Status;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  MmioWrite32 ((UINTN) SndwControllerMmioAddress + R_SNDW_MCP_B0_CLOCK_CTRL, (UINT32) V_SNDW_MCP_CLOCK_DIVIDER_BY_4);
  MmioWrite32 ((UINTN) SndwControllerMmioAddress + R_SNDW_MCP_B1_CLOCK_CTRL, (UINT32) V_SNDW_MCP_CLOCK_DIVIDER_BY_4);
  //
  // Program MCP_Config.CMDMode = 0, to enable PIO mode for Command TX FIFO, and Response RXFIFO.
  // Internally the FIFOs will be accessed by AHB Port.
  //
  MmioAnd32 ((UINTN) SndwControllerMmioAddress + R_SNDW_MEM_CONFIG, (UINT32) ~B_SNDW_MEM_CONFIG_MODE_AHB);

  //
  // Soundwire controller enumeration: Note** if >1 devices are being connected to the link, enumeration flow is a must.
  // Write 0 to MCP_Config.OperationMode to bring the controller into normal mode
  //
  MmioAnd32 ((UINTN) SndwControllerMmioAddress + R_SNDW_MEM_CONFIG, (UINT32) ~B_SNDW_MEM_CONFIG_OM_NORMAL);

  //
  // Write 1 to MCP_ConfigUpdate to update controller settings
  //
  MmioWrite32 ((UINTN) SndwControllerMmioAddress + R_SNDW_MEM_CONFIGUPDATE, (UINT32) B_SNDW_MEM_CONFIGUPDATE_UPDATE_DONE);

  ///
  /// Waiting for FIFO to be ready to send MCP message
  ///
  Status = StatusPolling (
            (UINTN) (SndwControllerMmioAddress + R_SNDW_MEM_STAT),
            B_SNDW_MEM_STAT_TXE,
            B_SNDW_MEM_STAT_TXE_FIFO_EMPTY);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Sndw controller not ready to send commands to codecs.\n"));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));

  return EFI_SUCCESS;
}


/**
  Function checks an access to Sndw controller

  @param[in] SndwControllerSpaceAddress    Sndw controller memory space address

  @retval TRUE                             Sndw Controller is ready
  @retval FALSE                            Sndw Controller is not ready
**/
BOOLEAN
SndwIoAccessReady (
  IN UINTN   SndwControllerSpaceAddress
  )
{
  if (MmioRead32 ((UINTN) (SndwControllerSpaceAddress + R_SNDW_MEM_CONFIG)) == 0xFFFFFFFF) {
    return FALSE;
  }
  return TRUE;
}


/**
  Function reads and returns Sndw codecs status

  @param[in]  SndwControllerSpaceAddress    Sndw controller memory space address
  @param[in]  PeripheralsStatus             Pointer to variable where codecs status should be written

  @retval EFI_SUCCESS                       Sndw codecs status read successfully
  @retval EFI_TIMEOUT                       No codecs detected
**/
EFI_STATUS
SndwReadCodecsStatus (
  IN  UINTN         SndwControllerSpaceAddress,
  OUT UINT32        *PeripheralsStatus
)
{
  UINT32            LoopTime;

  for (LoopTime = 0; LoopTime < HDA_MAX_LOOP_TIME; LoopTime++) {
    *PeripheralsStatus = MmioRead32 ((UINTN) (SndwControllerSpaceAddress + R_SNDW_MEM_PERIPHERALSTAT));
    if (*PeripheralsStatus != 0) {
      break;
    } else {
      MicroSecondDelay (HDA_WAIT_PERIOD);
    }
  }

  if (LoopTime >= HDA_MAX_LOOP_TIME) {
    return EFI_TIMEOUT;
  }
  return EFI_SUCCESS;
}


/**
  Function prints information about given Sndw Codec

  @param[in]  SndwCodecInfo    Pointer to Sndw codec information
**/
VOID
PrintCodecInfo (
  IN SNDW_CODEC_INFO           *SndwCodecInfo
  )
{
  DEBUG ((DEBUG_INFO, "Link Number:      %d\n", SndwCodecInfo->SndwLinkIndex));
  DEBUG ((DEBUG_INFO, "Peripheral Index: %d\n", SndwCodecInfo->PeripheralIndex));
  DEBUG ((DEBUG_INFO, "Codec ID:\n"));
  DEBUG ((DEBUG_INFO, "  UniqueId          [0x50]: 0x%02x\n", SndwCodecInfo->CodecId.Encoding.UniqueId));
  DEBUG ((DEBUG_INFO, "  Version           [0x50]: 0x%02x\n", SndwCodecInfo->CodecId.Encoding.Version));
  DEBUG ((DEBUG_INFO, "  ManufacturerID[0] [0x51]: 0x%02x\n", SndwCodecInfo->CodecId.Encoding.ManufacturerID[0]));
  DEBUG ((DEBUG_INFO, "  ManufacturerID[1] [0x52]: 0x%02x\n", SndwCodecInfo->CodecId.Encoding.ManufacturerID[1]));
  DEBUG ((DEBUG_INFO, "  PartId[0]         [0x53]: 0x%02x\n", SndwCodecInfo->CodecId.Encoding.PartId[0]));
  DEBUG ((DEBUG_INFO, "  PartId[1]         [0x54]: 0x%02x\n", SndwCodecInfo->CodecId.Encoding.PartId[1]));
  DEBUG ((DEBUG_INFO, "  Class             [0x55]: 0x%02x\n", SndwCodecInfo->CodecId.Encoding.Class));
}

/**
  Function prints information about all enumerated codecs

  @param[in]  SndwAccess    Pointer to Sndw access
**/
VOID
PrintAllCodecsInfo (
  SNDW_ACCESS               *SndwAccess
  )
{
  SNDW_CODEC_INFO           *CodecInfo;
  SNDW_CODEC_INFO           *NextCodecInfo;

  SndwAccess->GetFirstCodec (SndwAccess, &NextCodecInfo);

  while (NextCodecInfo != NULL) {
    DEBUG ((DEBUG_INFO, "------- SNDW CODEC -------\n"));

    DEBUG ((DEBUG_INFO, "SndwLinkIndex:        %d\n", NextCodecInfo->SndwLinkIndex));
    DEBUG ((DEBUG_INFO, "Peripheral Index:     %d\n", NextCodecInfo->PeripheralIndex));
    DEBUG ((DEBUG_INFO, "Codec ID:\n"));
    DEBUG ((DEBUG_INFO, "  UniqueId:           0x%02x\n", NextCodecInfo->CodecId.Encoding.UniqueId));
    DEBUG ((DEBUG_INFO, "  Version:            0x%02x\n", NextCodecInfo->CodecId.Encoding.Version));
    DEBUG ((DEBUG_INFO, "  ManufacturerID[0]:  0x%02x\n", NextCodecInfo->CodecId.Encoding.ManufacturerID[0]));
    DEBUG ((DEBUG_INFO, "  ManufacturerID[1]:  0x%02x\n", NextCodecInfo->CodecId.Encoding.ManufacturerID[1]));
    DEBUG ((DEBUG_INFO, "  PartId[0]:          0x%02x\n", NextCodecInfo->CodecId.Encoding.PartId[0]));
    DEBUG ((DEBUG_INFO, "  PartId[1]:          0x%02x\n", NextCodecInfo->CodecId.Encoding.PartId[1]));
    DEBUG ((DEBUG_INFO, "  Class:              0x%02x\n", NextCodecInfo->CodecId.Encoding.Class));

    DEBUG ((DEBUG_INFO, "----------------------\n"));

    CodecInfo = NextCodecInfo;
    NextCodecInfo = NULL;
    SndwAccess->GetNextCodec (SndwAccess, CodecInfo, &NextCodecInfo);
    FreePool (CodecInfo);
  }

  return;
}

/**
  Function sets Sndw shim registers to perform controller initialization.

  @param[in]  HdaBar              Memory map address of Dsp
  @param[in]  SndwLinkIndex       Index of Sndw link whose registers should be set

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Dsp memory space is not enabled
**/
VOID
SetSndwShimRegisters (
  IN UINTN    HdaBar,
  IN UINT32   SndwLinkIndex
  )
{
  ///
  /// SW enables Integration Glue to enforce IO AC timing in preparation for Manager IP being put into "Normal" Operation
  /// Write Shim register SNDWxACTMCTL, bit DACTQE=1b
  ///
  MmioOr16 ((UINTN) (HdaBar + R_HDA_MEM2_SNDW_SNDWxACTMCTL (SndwLinkIndex)), B_HDA_MEM2_SNDW_SNDWxACTMCTL_DACTQE);

  ///
  /// SW releases Override allowing Manager IP control
  /// Write Shim register SNDWxIOCTL, bit MIF=1b
  ///
  MmioOr16 ((UINTN) (HdaBar + R_HDA_MEM2_SNDW_SNDWxIOCTL (SndwLinkIndex)), B_HDA_MEM2_SNDW_SNDWxIOCTL_MIF);
}


/**
  Function gets Sndw links out of reset

  @param[in] SndwLinkIndex        Soundwire link index

  @retval EFI_SUCCESS             Sndw link is out of reset
  @retval EFI_TIMEOUT             Polling CPA bit timed out, Sndw link is not out of reset
**/
EFI_STATUS
GetSndwLinkOutOfReset (
  IN UINTN     HdaBar,
  IN UINTN     SndwLinkIndex
  )
{
  EFI_STATUS   Status;

  DEBUG ((DEBUG_INFO, "Sndw#%d: set SPA, waiting for CPA.\n", SndwLinkIndex));
  ///
  /// Set SNDWLCTL.SPA = 1 to get SoundWire IP power up and out of reset.
  ///
  MmioOr32 ((UINTN) (HdaBar + R_HDA_MEM2_SNDW_SNDWLCTL), B_HDA_MEM2_SNDW_SNDWLCTL_SPA (SndwLinkIndex));

  ///
  /// Wait until SNDWLCTL.CPA = 1 to confirm SoundWire IP is alive.
  ///
  Status = StatusPolling (
             (UINTN) (HdaBar + R_HDA_MEM2_SNDW_SNDWLCTL),
             B_HDA_MEM2_SNDW_SNDWLCTL_CPA (SndwLinkIndex),
             B_HDA_MEM2_SNDW_SNDWLCTL_CPA (SndwLinkIndex)
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Sndw#%d stalled!\n", SndwLinkIndex));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "Sndw#%d: CPA is set, controller out of reset. Status = %r\n", SndwLinkIndex, Status));
  return EFI_SUCCESS;
}

/**
  Function put Sndw IP controller in reset state.

  @param[in] SndwLinkIndex        Soundwire link index

  @retval EFI_SUCCESS             The controller was successfully reset.
  @retval EFI_TIMEOUT             Could not reset the controller, polling CPA bit timed out.
**/
EFI_STATUS
ResetSndwLink (
  IN UINTN     HdaBar,
  IN UINTN     SndwLinkIndex
  )
{
  EFI_STATUS   Status;

  DEBUG ((DEBUG_INFO, "Sndw#%d: set SPA to 0, waiting for CPA.\n", SndwLinkIndex));
  ///
  /// Set SNDWLCTL.SPA = 0 to get SoundWire Controllet in reset state.
  ///
  MmioAnd32 ((UINTN) (HdaBar + R_HDA_MEM2_SNDW_SNDWLCTL), (UINT32)~B_HDA_MEM2_SNDW_SNDWLCTL_SPA (SndwLinkIndex));

  ///
  /// Wait until SNDWLCTL.CPA = 0 to confirm SoundWire controller reset state.
  ///
  Status = StatusPolling (
             (UINTN) (HdaBar + R_HDA_MEM2_SNDW_SNDWLCTL),
             (UINT32) B_HDA_MEM2_SNDW_SNDWLCTL_CPA (SndwLinkIndex),
             (UINT32) 0
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Sndw#%d stalled!\n", SndwLinkIndex));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "Sndw#%d: CPA is set to 0, controller in reset state. Status = %r\n", SndwLinkIndex, Status));
  return EFI_SUCCESS;
}

/**
  Function allows to define Sndw controller mmio address.

  @param[in]  HdaBar                      Memory map address of Dsp
  @param[in]  SndwLinkIndex               Number of Sndw link
  @param[out] *SndwControllerMmioAddress  Pointer to memory space address of Sndw controller

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Dsp memory space is not enabled
  @retval EFI_DEVICE_ERROR        Not able to get Sndw controller mmio address
**/
EFI_STATUS
GetLinkControllerMmioAddress (
  IN  UINTN   HdaBar,
  IN  UINTN   SndwLinkIndex,
  OUT UINTN   *SndwControllerMmioAddress
  )
{
  if (HdaBar == 0) {
    return EFI_INVALID_PARAMETER;
  }

  *SndwControllerMmioAddress = (UINTN) (MmioRead32 ((UINTN) (HdaBar + R_HDA_MEM2_SNDW_SNDWIPPTR)) & B_HDA_MEM2_SNDW_SNDWIPPTR_PRT) + SndwLinkIndex * R_HDA_MEM2_SNDW_OFFSETS;
  if ((*SndwControllerMmioAddress == 0) || (*SndwControllerMmioAddress == (UINTN) ~0)) {
    return EFI_DEVICE_ERROR;
  }
  return EFI_SUCCESS;
}


/**
  Function reads and returns number of supported Sndw links.

  @param[in]  HdaBar                        Memory map address of Dsp
  @param[out] *NumberOfSupportedSndwLinks   Pointer to variable where number of supported Sndw links should be written

  @retval EFI_SUCCESS                       The function completed successfully
  @retval EFI_INVALID_PARAMETER             Dsp memory space is not enabled
**/
EFI_STATUS
GetNumberOfSupportedSndwLinks (
  IN  UINTN   HdaBar,
  OUT UINT32  *NumberOfSupportedSndwLinks
  )
{
  if (HdaBar == 0) {
    return EFI_INVALID_PARAMETER;
  }

  *NumberOfSupportedSndwLinks = MmioRead32 ((UINTN) (HdaBar + R_HDA_MEM2_SNDW_SNDWLCAP)) & B_HDA_MEM2_SNDW_SNDWLCAP_LCOUNT;
  *NumberOfSupportedSndwLinks >>= N_HDA_MEM2_SNDW_SNDWLCAP_LCOUNT;

  return EFI_SUCCESS;
}


/**
  Function initializes Soundwire controllers

  @param[in, out] NumberOfSupportedSndwLinks  Pointer to number of supported Sndw links
  @param[in]      HdaBar                      DSP base address

  @retval EFI_SUCCESS
  @retval EFI_DEVICE_ERROR
**/
EFI_STATUS
InitializeSndwControllers (
  IN OUT UINT32  *NumberOfSupportedSndwLinks,
  IN     UINTN    HdaBar
  )
{
  EFI_STATUS  Status;
  UINTN       SndwControllerMmioOffset;
  UINT32      SndwLinkIndex;

  ///
  /// Read number of supported links
  ///
  Status = GetNumberOfSupportedSndwLinks (HdaBar, NumberOfSupportedSndwLinks);
  DEBUG ((DEBUG_INFO, "Number of Supported Sndw Links %d\n", *NumberOfSupportedSndwLinks));

  for (SndwLinkIndex = 0; SndwLinkIndex < *NumberOfSupportedSndwLinks; SndwLinkIndex++) {
    Status = GetSndwLinkOutOfReset (HdaBar, SndwLinkIndex);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
      return EFI_DEVICE_ERROR;
    }

    SetSndwShimRegisters (HdaBar, SndwLinkIndex);

    ///
    /// Read address of SndwX controller
    ///
    GetLinkControllerMmioAddress (HdaBar, SndwLinkIndex, &SndwControllerMmioOffset);
    DEBUG ((DEBUG_INFO, "Sndw#%d: Controller mmio address: 0x%X.\n", SndwLinkIndex, HdaBar + SndwControllerMmioOffset));

    ///
    /// Check access to SndwX controller
    ///
    if (SndwIoAccessReady (HdaBar + SndwControllerMmioOffset) == FALSE) {
      DEBUG ((DEBUG_INFO, "No access to Sndw#%d Controller registers! Aborting", SndwLinkIndex));
      return EFI_DEVICE_ERROR;
    }

    ///
    /// Initialize SndwX controller
    ///
    Status = SndwControllerInit (HdaBar + SndwControllerMmioOffset);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
      return EFI_DEVICE_ERROR;
    } else {
      DEBUG ((DEBUG_INFO, "Sndw#%d controller ready to send commands to codecs.\n", SndwLinkIndex));
    }
  }

  return EFI_SUCCESS;
}

/**
  Function enumerates connected Soundwire codecs

  @param[in] SndwAccessContext            Pointer to Sndw access context
  @param[in] NumberOfSupportedSndwLinks   Number of supported Sndw links
  @param[in] HdaBar                       DSP base address

  @retval EFI_SUCCESS
  @retval EFI_DEVICE_ERROR
**/
EFI_STATUS
EnumerateSndwCodecs (
  IN SNDW_ACCESS_CONTEXT  *SndwAccessContext,
  IN UINT32               NumberOfSupportedSndwLinks,
  IN UINTN                HdaBar
  )
{
  EFI_STATUS        Status;
  CODEC_LIST_ENTRY  *CodecListEntry;
  SNDW_CODEC_ID     CodecId;
  SNDW_COMMAND      RxCommand;
  LIST_ENTRY        *CodecListHead;
  UINTN             SndwControllerMmioOffset;
  UINT32            SndwLinkIndex;
  UINT32            PeripheralIndex;
  UINT32            PeripheralsStatus;
  SNDW_COMMAND      TxCommand;

  CodecListHead = NULL;

  ///
  /// Controller after initialization sending ping message automatically
  ///
  MicroSecondDelay (HDA_WAIT_PERIOD * 100);

  for (SndwLinkIndex = 0; SndwLinkIndex < NumberOfSupportedSndwLinks; SndwLinkIndex++) {
    //
    // Read address of SndwX controller
    //
    Status = GetLinkControllerMmioAddress (HdaBar, SndwLinkIndex, &SndwControllerMmioOffset);
    if (EFI_ERROR (Status)) {
      continue;
    }
    DEBUG ((DEBUG_INFO, "Sndw#%d controller mmio address: 0x%X.\n", SndwLinkIndex, HdaBar + SndwControllerMmioOffset));

    for (PeripheralIndex = 1; PeripheralIndex <= SNDW_MAX_PERIPHERAL_NUMBER; PeripheralIndex++) {
      ///
      /// Codecs enumeration process, check peripherals status
      ///
      Status = SndwReadCodecsStatus (HdaBar + SndwControllerMmioOffset, &PeripheralsStatus);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Sndw#%d controller did not detect any codecs on the peripherials.\n", SndwLinkIndex));
        break;
      }

      DEBUG ((DEBUG_INFO, "Sndw#%d peripheral status: 0x%X.\n", SndwLinkIndex, PeripheralsStatus));
      if ((PeripheralsStatus & B_SNDW_MEM_PERIPHERALSTAT_STATUS (0)) == SndwPeripheralNotPresent) {
        DEBUG ((DEBUG_ERROR, "Enumeration complete on Sndw#%d controller.\n", SndwLinkIndex));
        break;
      } else if ((PeripheralsStatus & B_SNDW_MEM_PERIPHERALSTAT_STATUS (0)) == SndwPeripheralAttachedOk) {
        DEBUG ((DEBUG_INFO, "Peripheral#%d attached correctly.\n", 0));

        ///
        /// Read if a codec is connected to the Sndw bus
        /// Every peripheral starts with DeviceAddress '0' at the start of the enumeration
        ///
        Status = ReadPeripheralId (HdaBar + SndwControllerMmioOffset, 0, &CodecId);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_INFO, "Peripheral#%d on Sndw#%d not responding, skipping in the enumeration.\n", 0, SndwLinkIndex));
          continue;
        }

        ///
        /// Waiting a few us after sending init msg
        ///
        MicroSecondDelay (HDA_WAIT_PERIOD * 100);

        ///
        /// Set Codec Address
        ///
        TxCommand.TxWrite.OpCode        = SndwCmdWrite;
        TxCommand.TxWrite.RegAddr       = R_PCP_DevNumber;
        TxCommand.TxWrite.DeviceAddress = 0;
        TxCommand.TxWrite.RegData       = PeripheralIndex;
        SendwAck (HdaBar + SndwControllerMmioOffset, TxCommand, &RxCommand);

        ///
        /// Waiting a few us after sending init msg
        ///
        MicroSecondDelay (HDA_WAIT_PERIOD * 100);

        CodecListEntry = AllocatePool (sizeof (CODEC_LIST_ENTRY));

        if (CodecListEntry == NULL) {
          return EFI_OUT_OF_RESOURCES;
        }

        CodecListEntry->CodecInfo.SndwLinkIndex   = SndwLinkIndex;
        CodecListEntry->CodecInfo.PeripheralIndex = PeripheralIndex;
        CodecListEntry->SndwControllerMmioOffset  = SndwControllerMmioOffset;

        CopyMem (CodecListEntry->CodecInfo.CodecId.Data, CodecId.Data, sizeof (SNDW_CODEC_ID));

        PrintCodecInfo (&(CodecListEntry->CodecInfo));

        if (CodecListHead == NULL) {
          SndwAccessContext->CodecListHead = CodecListEntry;
          CodecListHead = &(CodecListEntry->ListEntry);
          InitializeListHead (CodecListHead);
        }
        else {
          InsertTailList (CodecListHead, &(CodecListEntry->ListEntry));
        }
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  Function finds given codec in the list of enumerated codecs and returns information about it.

  @param[in] SndwAccessContext            Pointer to Sndw access context
  @param[in] SndwCodecInfo                Pointer to Sndw codec information
  @param[in] PeripheralIndex              Pointer to peripherial index of given codec
  @param[in] SndwControllerMmioOffset     Pointer to MMIO address for given codec

  @retval EFI_SUCCESS
  @retval EFI_DEVICE_ERROR
**/
EFI_STATUS
GetCodecAddressFromCodecInfo (
  IN SNDW_ACCESS_CONTEXT    *SndwAccessContext,
  IN SNDW_CODEC_INFO        *SndwCodecInfo,
  IN OUT UINT32             *PeripheralIndex,
  IN OUT UINTN              *SndwControllerMmioOffset
  )
{
  CODEC_LIST_ENTRY          *CurrentSndwListEntry;
  LIST_ENTRY                *ListEntryHead;
  LIST_ENTRY                *CurrentListEntry;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  CurrentSndwListEntry = SndwAccessContext->CodecListHead;
  CurrentListEntry = &(CurrentSndwListEntry->ListEntry);
  ListEntryHead = CurrentListEntry;

  while (CurrentListEntry != NULL) {
    CurrentSndwListEntry = CODEC_LIST_ENTRY_FROM_LIST_ENTRY (CurrentListEntry);

    if (CompareMem (CurrentSndwListEntry->CodecInfo.CodecId.Data, SndwCodecInfo->CodecId.Data, sizeof (SNDW_CODEC_ID)) == 0) {
        SndwCodecInfo->SndwLinkIndex = CurrentSndwListEntry->CodecInfo.SndwLinkIndex;
        *PeripheralIndex = CurrentSndwListEntry->CodecInfo.PeripheralIndex;
        *SndwControllerMmioOffset = CurrentSndwListEntry->SndwControllerMmioOffset;
        DEBUG ((DEBUG_INFO, "Codec Found: PeripheralIndex: 0x%x, SndwControllerMmioOffset: 0x%x\n", *PeripheralIndex, *SndwControllerMmioOffset));
        return EFI_SUCCESS;
      }

    CurrentListEntry = GetNextNode (ListEntryHead, CurrentListEntry);
    if (CurrentListEntry == ListEntryHead) {
      break;
    }
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, EFI_NOT_FOUND));

  return EFI_NOT_FOUND;
}

/**
  Function operating on Sndw Fifo buffer for sending messages to codecs.

  @param[in]  This                Pointer to Sndw Access PPI/Protocol
  @param[in]  SndwCodecInfo       Sndw codec information
  @param[in]  TxCommands          Pointer to send messages
  @param[in]  TxSize              Size of messages to send

  @retval EFI_SUCCESS             Message sent successfully
  @retval EFI_NOT_FOUND           Given codec not found
**/
EFI_STATUS
EFIAPI
SndwSend (
  IN  CONST SNDW_ACCESS    *This,
  IN  SNDW_CODEC_INFO      SndwCodecInfo,
  IN  SNDW_COMMAND         *TxCommand,
  IN  UINTN                TxSize
  )
{
  EFI_STATUS               Status;
  UINTN                    SndwControllerMmioOffset;
  UINTN                    Index;
  UINTN                    HdaBar;
  UINT32                   PeripheralIndex;
  BOOLEAN                  IsTemporaryBar;
  SNDW_ACCESS_CONTEXT      *SndwAccessContext;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  SndwAccessContext = SNDW_ACCESS_CONTEXT_FROM_SNDW_ACCESS_PPI_PROTOCOL (This);

  Status = EnableHdaMmioAccess (SndwAccessContext->PciIo, &HdaBar, &IsTemporaryBar);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EnableHdaMmioAccess () failed! Status = %r\n", Status));
    return Status;
  }

  Status = GetCodecAddressFromCodecInfo (SndwAccessContext, &SndwCodecInfo, &PeripheralIndex, &SndwControllerMmioOffset);
  if (EFI_ERROR (Status)) {
    DisableHdaDspMmioAccess (SndwAccessContext->PciIo, IsTemporaryBar);
    DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
    return EFI_NOT_FOUND;
  }

  //
  // Update DeviceAddress in case of a write to one peripheral
  //
  for (Index = 0; Index < TxSize; Index++) {
    if (TxCommand[Index].TxWrite.DeviceAddress <= SNDW_MAX_PERIPHERAL_NUMBER) {
      TxCommand[Index].TxWrite.DeviceAddress = PeripheralIndex;
    }
  }
  Send (HdaBar + SndwControllerMmioOffset, TxCommand, TxSize);

  DisableHdaDspMmioAccess (SndwAccessContext->PciIo, IsTemporaryBar);

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
  return EFI_SUCCESS;
}


/**
  Function sends one message through the Sndw interface and waits
  for the corresponding ACK message.

  @param[in]  This                Pointer to Sndw Access PPI/Protocol
  @param[in]  SndwCodecInfo       Sndw codec information
  @param[in]  TxCommands          Pointer to sending message
  @param[out] RxCommands          Pointer to receiving message

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_NOT_FOUND           Given codec not found
  @retval EFI_TIMEOUT             Sending command has failed
  @retval EFI_DEVICE_ERROR        Response command has failed
**/
EFI_STATUS
EFIAPI
SndwSendWithAck (
  IN  CONST SNDW_ACCESS    *This,
  IN  SNDW_CODEC_INFO      SndwCodecInfo,
  IN  SNDW_COMMAND         TxCommand,
  OUT SNDW_COMMAND*        RxCommand OPTIONAL
  )
{
  EFI_STATUS               Status;
  UINTN                    SndwControllerMmioOffset;
  UINTN                    HdaBar;
  UINT32                   PeripheralIndex;
  BOOLEAN                  IsTemporaryBar;
  SNDW_ACCESS_CONTEXT      *SndwAccessContext;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  SndwAccessContext = SNDW_ACCESS_CONTEXT_FROM_SNDW_ACCESS_PPI_PROTOCOL (This);

  Status = EnableHdaMmioAccess (SndwAccessContext->PciIo, &HdaBar, &IsTemporaryBar);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EnableHdaMmioAccess () failed! Status = %r\n", Status));
    return Status;
  }

  Status = GetCodecAddressFromCodecInfo (SndwAccessContext, &SndwCodecInfo, &PeripheralIndex, &SndwControllerMmioOffset);
  if (EFI_ERROR (Status)) {
    DisableHdaDspMmioAccess (SndwAccessContext->PciIo, IsTemporaryBar);
    DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
    return EFI_NOT_FOUND;
  }

  DEBUG ((DEBUG_INFO, "PeripheralIndex: %d\n", PeripheralIndex));
  DEBUG ((DEBUG_INFO, "Sndw%d: Controller mmio address: 0x%X.\n", SndwCodecInfo.SndwLinkIndex, HdaBar + SndwControllerMmioOffset));

  //
  // Update DeviceAddress in case of a write to one peripheral
  //
  if (TxCommand.TxWrite.DeviceAddress <= SNDW_MAX_PERIPHERAL_NUMBER) {
    TxCommand.TxWrite.DeviceAddress = PeripheralIndex;
  }

  SendwAck (HdaBar + SndwControllerMmioOffset, TxCommand, RxCommand);

  DisableHdaDspMmioAccess (SndwAccessContext->PciIo, IsTemporaryBar);

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
  return EFI_SUCCESS;
}


/**
  Function operating on Sndw Fifo for receiving messages from codecs.

  @param[in]  This                Pointer to Sndw Access PPI/Protocol
  @param[in]  SndwCodecInfo       Sndw codec information
  @param[out] RxCommands          Pointer to pointer to received messages.
                                  Memory pool is allocated in this function,
                                  but the caller is responsible for
                                  freeing the memory with FreePool.
  @param[out] RxSize              Size of received messages

  @retval EFI_SUCCESS             Message received successfully
  @retval EFI_NOT_FOUND           Given codec not found
  @retval EFI_OUT_OF_RESOURCES    Memory allocation for response failed
**/
EFI_STATUS
EFIAPI
SndwReceive (
  IN  CONST SNDW_ACCESS           *This,
  IN  SNDW_CODEC_INFO             SndwCodecInfo,
  OUT SNDW_COMMAND                **RxCommands,
  OUT UINTN*                      RxSize
  )
{
  EFI_STATUS                      Status;
  UINTN                           SndwControllerMmioOffset;
  UINTN                           HdaBar;
  BOOLEAN                         IsTemporaryBar;
  SNDW_ACCESS_CONTEXT             *SndwAccessContext;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  SndwAccessContext = SNDW_ACCESS_CONTEXT_FROM_SNDW_ACCESS_PPI_PROTOCOL (This);

  Status = EnableHdaMmioAccess (SndwAccessContext->PciIo, &HdaBar, &IsTemporaryBar);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EnableHdaMmioAccess () failed! Status = %r\n", Status));
    return Status;
  }

  Status = GetCodecAddressFromCodecInfo (SndwAccessContext, &SndwCodecInfo, NULL, &SndwControllerMmioOffset);
  if (EFI_ERROR (Status)) {
    DisableHdaDspMmioAccess (SndwAccessContext->PciIo, IsTemporaryBar);
    DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
    return EFI_NOT_FOUND;
  }

  Receive (HdaBar + SndwControllerMmioOffset, RxCommands, RxSize);

  DisableHdaDspMmioAccess (SndwAccessContext->PciIo, IsTemporaryBar);

  if (*RxCommands == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
  return EFI_SUCCESS;
}


/**
  Function disables Soundwire interface

  @param[in] This            Pointer to Sndw Access PPI

  @retval EFI_SUCCESS
  @retval EFI_DEVICE_ERROR
**/
EFI_STATUS
EFIAPI
SndwAccessDisable (
  IN CONST SNDW_ACCESS       *This
  )
{
  UINTN                      HdaBar;
  BOOLEAN                    IsTemporaryBar;
  SNDW_ACCESS_CONTEXT        *SndwAccessContext;
  EFI_STATUS                 Status;
  UINT32                     SndwLinkIndex;
  UINT32                     NumberOfSupportedSndwLinks;
  UINTN                      SndwControllerMmioOffset;

  SndwAccessContext = SNDW_ACCESS_CONTEXT_FROM_SNDW_ACCESS_PPI_PROTOCOL (This);

  if (SndwAccessContext->EnableCounter <= 0) {
    DEBUG ((DEBUG_ERROR, "SndwAccess already disabled.\n"));
    return EFI_SUCCESS;
  }
  SndwAccessContext->EnableCounter -= 1;
  if (SndwAccessContext->EnableCounter > 0) {
    DEBUG ((DEBUG_ERROR, "SndwAccess still being used.\n"));
    return EFI_SUCCESS;
  }

  Status = EnableHdaMmioAccess (SndwAccessContext->PciIo, &HdaBar, &IsTemporaryBar);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EnableHdaMmioAccess () failed! Status = %r\n", Status));
    return Status;
  }

  MmioAnd32 ((HdaBar + R_ACE_MEM_PPCTL), (UINT32) ~B_ACE_MEM_PPCTL_GPROCEN);
  DEBUG ((DEBUG_INFO, "PPCTL.GPROCEN = 0x%x\n", !!(MmioRead32 (HdaBar + R_ACE_MEM_PPCTL) & B_ACE_MEM_PPCTL_GPROCEN)));

  Status = GetNumberOfSupportedSndwLinks (HdaBar, &NumberOfSupportedSndwLinks);
  DEBUG ((DEBUG_INFO, "Number of Supported Sndw Links %d\n", NumberOfSupportedSndwLinks));

  for (SndwLinkIndex = 0; SndwLinkIndex < NumberOfSupportedSndwLinks; SndwLinkIndex++) {
    GetLinkControllerMmioAddress (HdaBar, SndwLinkIndex, &SndwControllerMmioOffset);
    DEBUG ((DEBUG_INFO, "Sndw#%d: Controller mmio address: 0x%X.\n", SndwLinkIndex, HdaBar + SndwControllerMmioOffset));

    MmioAnd32 ((UINTN) SndwControllerMmioOffset + R_SNDW_MEM_CONFIG, (UINT32) ~B_SNDW_MEM_CONFIG_OM_NORMAL);
    MmioOr32 ((UINTN) SndwControllerMmioOffset + R_SNDW_MEM_CONFIG, (UINT32) V_SNDW_MEM_CONFIG_OM_RESET_VALUE);

    // Clear Shim register SNDWxACTMCTL, bit DACTQE=0b
    MmioAnd16 ((UINTN) (HdaBar + R_HDA_MEM2_SNDW_SNDWxACTMCTL (SndwLinkIndex)), (UINT16) ~B_HDA_MEM2_SNDW_SNDWxACTMCTL_DACTQE);
    // Clear Shim register SNDWxIOCTL, bit MIF=0b
    MmioAnd16 ((UINTN) (HdaBar + R_HDA_MEM2_SNDW_SNDWxIOCTL (SndwLinkIndex)), (UINT16) ~B_HDA_MEM2_SNDW_SNDWxIOCTL_MIF);

    //
    // Write 1 to MCP_ConfigUpdate to update controller settings
    //
    MmioWrite32 ((UINTN) SndwControllerMmioOffset + R_SNDW_MEM_CONFIGUPDATE, (UINT32) B_SNDW_MEM_CONFIGUPDATE_UPDATE_DONE);

    Status = ResetSndwLink (HdaBar, SndwLinkIndex);
    if (EFI_ERROR (Status)) {
      break;
    }
  }

  DisableHdaDspMmioAccess (SndwAccessContext->PciIo, IsTemporaryBar);

  return Status;
}

/**
  Function enables Soundwire interface and enumerates attached codecs

  @param[in] This               Pointer to Sndw Access PPI/Protocol

  @retval EFI_SUCCESS
  @retval EFI_DEVICE_ERROR
**/
EFI_STATUS
EFIAPI
SndwAccessEnable (
  IN CONST SNDW_ACCESS          *This
  )
{
  EFI_STATUS                    Status;
  UINT32                        NumberOfSupportedSndwLinks;
  UINTN                         HdaBar;
  BOOLEAN                       IsTemporaryBar;
  SNDW_ACCESS_CONTEXT           *SndwAccessContext;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  SndwAccessContext = SNDW_ACCESS_CONTEXT_FROM_SNDW_ACCESS_PPI_PROTOCOL (This);

  SndwAccessContext->EnableCounter += 1;
  if (SndwAccessContext->EnableCounter != 1) {
    DEBUG ((DEBUG_ERROR, "SndwAccess already enabled.\n"));
    return EFI_SUCCESS;
  }

  Status = EnableHdaMmioAccess (SndwAccessContext->PciIo, &HdaBar, &IsTemporaryBar);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EnableHdaMmioAccess () failed! Status = %r\n", Status));
    return Status;
  }

  if (IsHdaControllerReset (HdaBar)) {
    DEBUG ((DEBUG_INFO, "HDA Controller is in reset state, getting out of reset...\n"));
    Status = AceSetControllerReset (HdaBar, FALSE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Un-reset High Definition Audio (CRST = 1) Time Out!\n"));
      return EFI_TIMEOUT;
    }
  }

  DEBUG ((DEBUG_INFO, "Set PPCTL.GPROCEN = 1 (Global Processing Enable - to enable ADSP for operation)\n"));
  MmioOr32 ((HdaBar + R_ACE_MEM_PPCTL), (UINT32) B_ACE_MEM_PPCTL_GPROCEN);

  Status = InitializeSndwControllers (&NumberOfSupportedSndwLinks, HdaBar);

  if (Status == EFI_SUCCESS) {
    Status = EnumerateSndwCodecs (SndwAccessContext, NumberOfSupportedSndwLinks, HdaBar);
  }

  DisableHdaDspMmioAccess (SndwAccessContext->PciIo, IsTemporaryBar);

  PrintAllCodecsInfo (&SndwAccessContext->SndwAccess);

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));

  return Status;
}


/**
  Function return first codec saved in codecs list.
  If it does not exist, function sets *CodecInfo to NULL.

  @param[in]  This                Pointer to Sndw Access PPI/Protocol
  @param[out] SndwCodecInfo       Pointer to pointer to Sndw codec information structure.
                                  The caller is responsible for freeing the memory.

  @retval EFI_SUCCESS             Codec found
  @retval EFI_NOT_FOUND           Codec not found
  @retval EFI_OUT_OF_RESOURCES    Memory allocation for codec info failed
**/
EFI_STATUS
EFIAPI
SndwGetFirstCodec (
  IN  CONST SNDW_ACCESS           *This,
  OUT SNDW_CODEC_INFO             **SndwCodecInfo
  )
{
  CODEC_LIST_ENTRY                *CurrentCodecListEntry;
  SNDW_ACCESS_CONTEXT             *SndwAccessContext;

  SndwAccessContext = SNDW_ACCESS_CONTEXT_FROM_SNDW_ACCESS_PPI_PROTOCOL (This);

  CurrentCodecListEntry = SndwAccessContext->CodecListHead;

  if (CurrentCodecListEntry != NULL) {
    *SndwCodecInfo = AllocatePool (sizeof (SNDW_CODEC_INFO));
    if (*SndwCodecInfo == NULL) {
      DEBUG ((DEBUG_INFO, "Couldn't allocate the memory for Sndw Codec Info!\n"));
      return EFI_OUT_OF_RESOURCES;
    }

    (*SndwCodecInfo)->SndwLinkIndex   = CurrentCodecListEntry->CodecInfo.SndwLinkIndex;
    (*SndwCodecInfo)->PeripheralIndex = CurrentCodecListEntry->CodecInfo.PeripheralIndex;
    CopyMem (&((*SndwCodecInfo)->CodecId.Data), &(CurrentCodecListEntry->CodecInfo.CodecId.Data), sizeof (SNDW_CODEC_ID));
  }
  else {
    *SndwCodecInfo = NULL;
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}


/**
  Function retrieves the next codec saved in codecs list that follows the given one.
  If it does not exist, function sets *NextCodecInfo to NULL.

  @param[in]  This                Pointer to Sndw Access PPI/Protocol
  @param[in]  SndwCodecInfo       Pointer to Sndw codec information structure.
  @param[out] NextCodecInfo       Pointer to pointer to Sndw codec information structure.
                                  The caller is responsible for freeing the memory.

  @retval EFI_SUCCESS             Codec found
  @retval EFI_NOT_FOUND           Codec not found
  @retval EFI_OUT_OF_RESOURCES    Memory allocation for codec info failed
**/
EFI_STATUS
EFIAPI
SndwGetNextCodec (
  IN   CONST SNDW_ACCESS          *This,
  IN   SNDW_CODEC_INFO            *SndwCodecInfo,
  OUT  SNDW_CODEC_INFO            **NextSndwCodecInfo
  )
{
  CODEC_LIST_ENTRY                *CurrentCodecListEntry;
  LIST_ENTRY                      *ListHead;
  LIST_ENTRY                      *CurrentListEntry;
  SNDW_ACCESS_CONTEXT             *SndwAccessContext;

  SndwAccessContext = SNDW_ACCESS_CONTEXT_FROM_SNDW_ACCESS_PPI_PROTOCOL (This);

  ListHead = &(SndwAccessContext->CodecListHead->ListEntry);
  CurrentListEntry = ListHead;

  while (TRUE) {
    CurrentCodecListEntry = CODEC_LIST_ENTRY_FROM_LIST_ENTRY (CurrentListEntry);

    if ((CurrentCodecListEntry->CodecInfo.SndwLinkIndex == SndwCodecInfo->SndwLinkIndex) &&
       (CompareMem (CurrentCodecListEntry->CodecInfo.CodecId.Data, SndwCodecInfo->CodecId.Data, sizeof (SNDW_CODEC_ID)) == 0)) {
      CurrentListEntry = GetNextNode (ListHead, CurrentListEntry);
      if (CurrentListEntry == ListHead) {
        *NextSndwCodecInfo = NULL;
        return EFI_NOT_FOUND;
      }

      CurrentCodecListEntry = CODEC_LIST_ENTRY_FROM_LIST_ENTRY (CurrentListEntry);

      *NextSndwCodecInfo = AllocatePool (sizeof (SNDW_CODEC_INFO));
      if (*NextSndwCodecInfo == NULL) {
        DEBUG ((DEBUG_INFO, "Couldn't allocate the memory for Sndw Codec Info!\n"));
        return EFI_OUT_OF_RESOURCES;
      }

      (*NextSndwCodecInfo)->SndwLinkIndex   = CurrentCodecListEntry->CodecInfo.SndwLinkIndex;
      (*NextSndwCodecInfo)->PeripheralIndex = CurrentCodecListEntry->CodecInfo.PeripheralIndex;
      CopyMem (&((*NextSndwCodecInfo)->CodecId.Data), &(CurrentCodecListEntry->CodecInfo.CodecId.Data), sizeof (SNDW_CODEC_ID));
      break;
    }

    CurrentListEntry = GetNextNode (ListHead, CurrentListEntry);
    if (CurrentListEntry == ListHead) {
      *NextSndwCodecInfo = NULL;
      return EFI_NOT_FOUND;
    }
  }

  return EFI_SUCCESS;
}
