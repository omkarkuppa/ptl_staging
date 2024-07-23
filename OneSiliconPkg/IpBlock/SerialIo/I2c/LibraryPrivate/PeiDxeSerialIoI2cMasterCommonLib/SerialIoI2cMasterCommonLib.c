/** @file
 Implement the SerialIo I2C Master Common Lib

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

#include <Uefi.h>
#include <Base.h>
#include <IndustryStandard/Pci.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SerialIoAccessLib.h>
#include <Library/SerialIoI2cMasterCommonLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SerialIoI2cPrivateLib.h>
#include <Library/SerialIoPrivateLib.h>
#include <Register/SerialIoRegs.h>
#include <Register/SerialIoI2cRegs.h>
#include <Protocol/I2cMaster.h>
#include <Pi/PiI2c.h>

/**
  Prepare I2c controller for use: enable its mmio range, put in D0, get out of reset
  Verifies I2C Line SDA and SCL states

  @param[in] Context - driver context

  @retval EFI_SUCCESS         Controller prepared
  @retval EFI_DEVICE_ERROR    SCL and/or SDA lines are not pulled up
**/
EFI_STATUS
PrepareController (
  I2C_MASTER_CONTEXT  *Context
  )
{
  SERIAL_IO_I2C_GENERAL               I2cGeneralConfigRegister;
  SERIAL_IO_PWRMG_CONTROL_AND_STATUS  I2cPwrMgmControlStsRegAndData;
  SERIAL_IO_HOST_CONTROL_RESET        I2cHostControllerResetReg;

  I2cPwrMgmControlStsRegAndData.Data32               = (UINT32) ~0x0; // This is AND Value make sure bits are cleared/set properly.
  I2cPwrMgmControlStsRegAndData.Fields.PwrState      = 0x0; // D0 State

  PciSegmentOr16 (Context->ConfigAddress + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
  PciSegmentAnd32 (Context->ConfigAddress + R_SERIAL_IO_CFG_PMECTRLSTATUS, I2cPwrMgmControlStsRegAndData.Data32);
  MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_GENERAL); // Dummy read after power bring up

  // I2C out of Reset
  I2cHostControllerResetReg.Data32 = MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_MEM_RESETS);
  I2cHostControllerResetReg.Fields.HostControlReset  = 0x3;  // Reset Released
  I2cHostControllerResetReg.Fields.DmaReset          = 0x1;  // Reset Released
  MmioWrite32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_MEM_RESETS, I2cHostControllerResetReg.Data32);

  //
  //  Check if I2C lines are pulled up
  //
  I2cGeneralConfigRegister.Data32 = MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_GENERAL);

  if (I2cGeneralConfigRegister.Fields.SdaRdPostDr | I2cGeneralConfigRegister.Fields.SclRdPostDr) {
    MmioWrite32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_ENABLE, 0);
    // Clear any errors/signal gliches during boot time
    MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_CLR_TX_ABRT);
    MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_CLR_INTR);
    return EFI_SUCCESS;
  }
  DEBUG (( DEBUG_ERROR, "I2C PrepareController Failed SDA or SCL Line is not pulled up.\n"));
  return EFI_DEVICE_ERROR;
}

/**
  Determine the state of the I2C controller

  @param[in] Context - driver context

  @retval TRUE              The I2C controller is active
  @retval FALSE             The I2C controller is idle
**/
BOOLEAN
IsHardwareActive (
  I2C_MASTER_CONTEXT  *Context
  )
{
  SERIAL_IO_I2C_MEM_STATUS   I2cStatusRegister;

  I2cStatusRegister.Data32 = (UINT32) 0x00;

  I2cStatusRegister.Data32 = MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_STATUS);

  return ((BOOLEAN) I2cStatusRegister.Fields.IcStatusAct);
}

/**
  Updates WriteOperation and WritePosition, two variables that determine
  which part of Request is being committed to I2C bus.
  This iterates over both Read and Write operations from a request, because
  things that need to be written to WriteFifo are both I2c bus writes
  and I2c bus reads (the command to perform bus read needs to be put into Write Fifo)

  @param[in] Context - driver context
**/
VOID
UpdateWritePosition (
  I2C_MASTER_CONTEXT *Context
  )
{
  if (Context->Request->Operation[Context->WriteOp].Flags & I2C_FLAG_READ) {
    Context->ExpectedReadPos++;
  }
  if ( Context->WritePos == Context->Request->Operation[Context->WriteOp].LengthInBytes - 1) {
    Context->WritePos = 0;
    Context->WriteOp ++;
  } else {
    Context->WritePos ++;
  }
}

/*
  FindReadOp checks if current Operation is of Read type. If so, returns.
  If not, increases ReadOp until it finds one or goes beyond Request's OperationCount

  @param[in] Context - driver context
*/
VOID
FindReadOp (
  I2C_MASTER_CONTEXT *Context
  )
{
  while (Context->ReadOp < Context->Request->OperationCount &&
         !(Context->Request->Operation[Context->ReadOp].Flags & I2C_FLAG_READ)) {
    Context->ReadOp++;
  }
}

/**
  Updates ReadOperation and ReadPosition, two variables that determine
  which part of Request is being filled with data incoming from I2C reads.
  This iterates only over Read operations from a request.

  @param[in] Context - driver context
**/
VOID
UpdateReadPosition (
  I2C_MASTER_CONTEXT *Context
  )
{
  if ( Context->ReadPos == Context->Request->Operation[Context->ReadOp].LengthInBytes - 1) {
    Context->ReadPos = 0;
    Context->ReadOp ++;
    FindReadOp (Context);
  } else {
    Context->ReadPos ++;
  }
}

/**
  ValidateRequest checks if Request is valid and can be started

  @param[in] Context - driver context
  @param[in] RequestPacket

  @retval EFI_SUCCESS           Request is valid and can be started
  @retval EFI_ALREADY_STARTED   The controller is busy with another transfer
  @retval EFI_BAD_BUFFER_SIZE   Transfer size too big
  @retval EFI_INVALID_PARAMETER RequestPacket is NULL, invalid Operation flags
  @retval EFI_UNSUPPORTED       "ping" operation attempted (0-byte transfer, address byte not followed by any data)
**/
EFI_STATUS
ValidateRequest (
  I2C_MASTER_CONTEXT           *Context,
  CONST EFI_I2C_REQUEST_PACKET *RequestPacket
  )
{
  UINTN TotalSize;
  UINTN Operation;
  UINTN OperationSize;

  if (Context->TransferInProgress) {
    return EFI_ALREADY_STARTED;
  }
  if (RequestPacket == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (RequestPacket->OperationCount == 0) {
    // 0-size operations ("pings") not supported
    return EFI_UNSUPPORTED;
  }

  TotalSize = 0;

  for (Operation=0; Operation < RequestPacket->OperationCount; Operation++) {
    // unsupported flags?
    if ((RequestPacket->Operation[Operation].Flags & (~I2C_FLAG_READ)) != 0) {
      return EFI_INVALID_PARAMETER;
    }
    OperationSize = RequestPacket->Operation[Operation].LengthInBytes;
    // 0-size operations ("pings") not supported
    if (OperationSize == 0) {
      return EFI_UNSUPPORTED;
    }
    TotalSize += OperationSize;
    // read operation too big?
    if (RequestPacket->Operation[Operation].Flags & I2C_FLAG_READ) {
      if (OperationSize > Context->Capabilities.MaximumReceiveBytes) {
        return EFI_BAD_BUFFER_SIZE;
      }
      // write operation too big?
    } else {
      if (OperationSize > Context->Capabilities.MaximumTransmitBytes) {
        return EFI_BAD_BUFFER_SIZE;
      }
    }
  }
  // total request too big?
  if (TotalSize > Context->Capabilities.MaximumTotalBytes) {
    return EFI_BAD_BUFFER_SIZE;
  }

  return EFI_SUCCESS;
}

/**
  IsLastFromRequest checks if WritePos and WriteOp point to the last byte of the request

  @param[in] Context - driver context

  @retval Boolean
**/
BOOLEAN
IsLastFromRequest (
  I2C_MASTER_CONTEXT *Context
  )
{
  if ((Context->WriteOp == Context->Request->OperationCount - 1) &&
      (Context->WritePos == Context->Request->Operation[Context->WriteOp].LengthInBytes - 1)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  IsFirstFromOperation checks if WritePos and WriteOp point to the first byte of an operation

  @param[in] Context - driver context

  @retval Boolean
**/
BOOLEAN
IsFirstFromOperation (
  I2C_MASTER_CONTEXT *Context
  )
{
  if ((Context->WriteOp != 0) && (Context->WritePos == 0)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  InitializeTransfer checks if HW is ready to accept new transfer.
  If so, sets up TargetAddress

  @param[in] Context - driver context

  @retval Status
**/
EFI_STATUS
InitializeTransfer (
  I2C_MASTER_CONTEXT           *Context,
  UINTN                        TargetAddress,
  CONST EFI_I2C_REQUEST_PACKET *RequestPacket
  )
{
  UINT32                              Attempts = 10000;
  SERIAL_IO_I2C_MEM_TAR               I2cTargetAddrRegister;
  SERIAL_IO_I2C_MEM_STATUS            I2cStatusRegister;
  SERIAL_IO_I2C_MEM_ENABLE            I2cEnableRegister;
  SERIAL_IO_I2C_MEM_ENABLE_STATUS     I2cEnableStsRegister;

  I2cTargetAddrRegister.Data32 = (UINT32) 0x00;
  I2cStatusRegister.Data32     = (UINT32) 0x00;
  I2cEnableRegister.Data32     = (UINT32) 0x00;
  I2cEnableStsRegister.Data32  = (UINT32) 0x00;

  Context->Request = (EFI_I2C_REQUEST_PACKET*) RequestPacket;
  Context->TransferStatus = EFI_SUCCESS;
  Context->WriteOp = 0;
  Context->WritePos = 0;
  Context->ReadOp = 0;
  Context->ExpectedReadPos = 0;
  FindReadOp (Context);
  Context->ReadPos = 0;

  if (MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_ENABLE) != 0) {
    DEBUG (( DEBUG_ERROR, "Address change was attempted while a transfer was underway!\n"));
    return EFI_DEVICE_ERROR;
  }

  I2cTargetAddrRegister.Fields.IcTar = TargetAddress & 0x3FF;
  if (TargetAddress & I2C_ADDRESSING_10_BIT) {
    I2cTargetAddrRegister.Fields.Ic10bHost = 1;     //10 - bit addressing by default 0 7 bit addressing
  }
  MmioWrite32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_TAR, I2cTargetAddrRegister.Data32);

  I2cEnableRegister.Fields.Enable = 1;
  MmioWrite32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_ENABLE, I2cEnableRegister.Data32);

  // Clear FIFO
  I2cStatusRegister.Data32 = MmioRead32 ((UINTN)Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_STATUS);
  while (I2cStatusRegister.Fields.RFNE) {
    MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_DATA_CMD);
    I2cStatusRegister.Data32 = MmioRead32 ((UINTN)Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_STATUS);
  }
  //clear errors
  MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_CLR_TX_ABRT);
  MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_CLR_INTR);

  I2cEnableStsRegister.Data32 = MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_ENABLE_STATUS);

  while (!(I2cEnableStsRegister.Fields.IcEn)) {
    //should never happen, but just to make sure BIOS doesn't hang in infinite loop...
    MicroSecondDelay (1);
    if (--Attempts == 0) {
      MmioWrite32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_ENABLE, 0);
      DEBUG (( DEBUG_ERROR, "I2cHost InitializeTransfer Timeout\n"));
      return EFI_DEVICE_ERROR;
    }
    I2cEnableStsRegister.Data32 = MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_ENABLE_STATUS);
  }
  Context->TransferInProgress = TRUE;
  return EFI_SUCCESS;
}

/*
  Clean up Hw activity and errors
  Return status to Request's submitter and signal the event that tells
  it that the request is complete
  Clear up Sw context to allow new request to start

  @param[in] Context - driver context
*/
VOID
FinishTransfer (
  I2C_MASTER_CONTEXT *Context
  )
{
  UINT32                              Attempts = 10000;
  SERIAL_IO_I2C_MEM_ENABLE_STATUS     I2cEnableStsRegister;

  I2cEnableStsRegister.Data32  = (UINT32) 0x00;

  MmioWrite32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_ENABLE, 0);

  while (1) {
    I2cEnableStsRegister.Data32 = MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_ENABLE_STATUS);

    if (I2cEnableStsRegister.Fields.IcEn) {
      //try for 1ms; should take no more than 100us even at slowest bus speed, but just to make sure BIOS doesn't hang in infinite loop
      MicroSecondDelay (1);
      if (--Attempts == 0) {
        ASSERT (FALSE);
        break;
      }
    } else {
      break;
    }
  }
  //
  //clear errors
  //
  MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_CLR_TX_ABRT);
  MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_CLR_INTR);

  //
  //clean up context data
  //
  Context->TransferInProgress = FALSE;
}
/**
  WriteFifo writes to I2c controller's transmit Fifo. Data written to Fifo could be
  - data bytes to be written to an i2c target
  - read requests that trigger i2c bus reads
  First transfer from each operation adds Restart bit which triggers Restart condition on bus
  Last transfer from the whole Request adds Stop bit which triggers Stop condtion on bus
  Driver keeps track of which parts of Request were already committed to hardware using
  pointer consisting of WritePosition and WriteOperation variables. This pointer is updated
  every time data byte/read request is committed to FIFO
  WriteFifo executes while there's anything more to write and the write fifo isn't full

  @param[in] Context - driver context
**/
VOID
WriteFifo (
  I2C_MASTER_CONTEXT *Context
  )
{
  UINT8                      RxFifoGuard;
  SERIAL_IO_I2C_MEM_STATUS   I2cStatusRegister;
  SERIAL_IO_I2C_MEM_DATA_CMD I2cDataCommandRegister;

  I2cStatusRegister.Data32       = (UINT32) 0x00;
  I2cDataCommandRegister.Data32  = (UINT32) 0x00;

  RxFifoGuard = 0;

  I2cStatusRegister.Data32 = MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_STATUS);
  while (I2cStatusRegister.Fields.TFNF) {
    if (Context->WriteOp >= Context->Request->OperationCount ) {
      return; // request complete, nothing more to write
    }

    //
    // Check for TX FIFO overrun
    //
    if ((MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_TXFLR) & 0x1FF) >= 63) {
      DEBUG (( DEBUG_ERROR, "R_SERIAL_IO_I2C_MEM_IC_TXFLR 0x%X\n", MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_TXFLR)));
      return;
    }

    if (Context->Request->Operation[Context->WriteOp].Flags & I2C_FLAG_READ) {
      if ((MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_RXFLR) & 0x1FF) >= 63) {
        DEBUG (( DEBUG_ERROR, "R_SERIAL_IO_I2C_MEM_IC_RXFLR 0x%X\n", MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_RXFLR)));
        DEBUG (( DEBUG_WARN, "WritePos %d\n", Context->WritePos));
        return;
      }
      //
      // Issue read commands until RX FIFO is full
      //
      I2cStatusRegister.Data32 = MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_STATUS);
      if (I2cStatusRegister.Fields.RFF) {
        DEBUG (( DEBUG_ERROR, "RFF\n"));
        return;
      }
      //
      // SW protection to avoid RX FIFO overrun
      //
      if (RxFifoGuard == 63) {
        return;
      }
      I2cDataCommandRegister.Data32 = 0x0;      // Clear all fields
      I2cDataCommandRegister.Fields.Cmd = 1;    // Read
      RxFifoGuard++;
    } else {
      I2cDataCommandRegister.Data32 = Context->Request->Operation[Context->WriteOp].Buffer[Context->WritePos];
    }
    if (IsLastFromRequest (Context)) {
      I2cDataCommandRegister.Fields.Stop = 1;
    }
    if (IsFirstFromOperation (Context)) {
      I2cDataCommandRegister.Fields.Restart = 1;
      if (Context->Request->Operation[Context->WriteOp].Flags & I2C_FLAG_READ) {
        MmioWrite32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_DATA_CMD, (UINT32) I2cDataCommandRegister.Data32);
        I2cDataCommandRegister.Fields.Restart = 0;
      }
    }
    MmioWrite32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_DATA_CMD, I2cDataCommandRegister.Data32);
    UpdateWritePosition (Context);
    I2cStatusRegister.Data32 = MmioRead32((UINTN)Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_STATUS);
  }
}

/**
  ReadFifo reads from I2c controller's receive Fifo. It contains data retrieved
  from target device as a result of executing read transfers, which were
  triggered by putting read requests into Write Fifo. Retrieved data is copied into buffers
  pointed to by Request structure.
  Driver keeps track where to copy incoming data using pointer consisting of
  ReadPosition and ReadOperation variables. This pointer is updated
  every time data was retrieved from hardware
  ReadFifo executes while there's data available and receive buffers were not filled

  @param[in] Context - driver context
**/
VOID
ReadFifo (
  I2C_MASTER_CONTEXT *Context
  )
{
  UINT32                     Timeout;
  SERIAL_IO_I2C_MEM_STATUS   I2cStatusRegister;

  I2cStatusRegister.Data32 = (UINT32) 0x0;

  Timeout = 10000;

  while (Context->ReadPos < Context->ExpectedReadPos) {
    I2cStatusRegister.Data32 = MmioRead32 ((UINTN)Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_STATUS);

    while (I2cStatusRegister.Fields.RFNE) {
      Context->Request->Operation[Context->ReadOp].Buffer[Context->ReadPos] = (0xFF & MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_DATA_CMD));
      DEBUG (( DEBUG_INFO, "DATA: %x %d\n", Context->Request->Operation[Context->ReadOp].Buffer[Context->ReadPos],Context->ReadPos));
      UpdateReadPosition (Context);
      if ( Context->ReadOp >= Context->Request->OperationCount ) {
        DEBUG (( DEBUG_ERROR, "R %d\n", Context->ReadOp));
        DEBUG (( DEBUG_WARN, "R_SERIAL_IO_I2C_MEM_IC_RXFLR 0x%X\n", MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_RXFLR)));
        Context->ExpectedReadPos = 0;
        return;
      }
      I2cStatusRegister.Data32 = MmioRead32 ((UINTN)Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_STATUS);
    }
    MicroSecondDelay (1);
    Timeout--;
    if (Timeout == 0) {
      DEBUG (( DEBUG_ERROR, "I2C initiator ReadFifo Timeout\n"));
      DEBUG (( DEBUG_WARN, "ReadPos %d\n", Context->ReadPos));
      DEBUG (( DEBUG_WARN, "ExpectedReadPos %d\n", Context->ExpectedReadPos));
      FinishTransfer (Context);
      Context->TransferStatus = EFI_TIMEOUT;
      return;
    }
  }
}

/**
  CheckErrors checks if there were any transfer errors.

  @param[in] Context - driver context
**/
VOID
CheckErrors (
  I2C_MASTER_CONTEXT *Context
  )
{
  SERIAL_IO_I2C_MEM_INTR_STAT       I2cIntStsRegister;
  SERIAL_IO_I2C_MEM_TX_ABRT_SOURCE  I2cTxAbortStsRegister;

  I2cIntStsRegister.Data32 = (UINT32) 0x0;
  I2cIntStsRegister.Data32 = (UINT32) 0x0;

  I2cIntStsRegister.Data32 = MmioRead32 ((UINTN)Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_INTR_STAT);

  if (!I2cIntStsRegister.Fields.RTxAbrt) {
    return;
  }
  I2cTxAbortStsRegister.Data32 = MmioRead32 ((UINTN)Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_TX_ABRT_SOURCE);
  if (I2cTxAbortStsRegister.Fields.Abrt7bAddrNoAck) {
    Context->TransferStatus = EFI_NO_RESPONSE;
  } else {
    Context->TransferStatus = EFI_DEVICE_ERROR;
  }
  DEBUG (( DEBUG_INFO, "I2c CheckErrors: %08x\n", MmioRead32 ((UINTN) Context->MmioAddress + R_SERIAL_IO_I2C_MEM_IC_TX_ABRT_SOURCE)));
}

/**
  Transfer is finished when all requested operations were placed in fifo,
  all read requests were filled and hardware is inactive
  The last part is necessary for write-only transfers where after
  placing all writes in fifo sw needs to wait until they flush down the bus

  @param[in] Context - driver context

  @retval Boolean
**/
BOOLEAN
IsTransferFinished (
  I2C_MASTER_CONTEXT *Context
  )
{
  if (( Context->WriteOp >= Context->Request->OperationCount ) &&
      ( Context->ReadOp >= Context->Request->OperationCount ) &&
      !IsHardwareActive (Context)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  PerformTransfer. For synchronous transfer this function is called in a loop
  and for asynchronous transfers, as a timer callback. It writes data and/or
  read requests to hadrware, copies read data to destination buffers. When
  transfer completes, it cleans up Sw context and Hw registers in preparation
  for new transfer

  @param[in] Context - driver context
**/
VOID
PerformTransfer (
  IN I2C_MASTER_CONTEXT *Context
  )
{
  if (!Context->TransferInProgress) {
    return;
  }
  if (EFI_ERROR (Context->TransferStatus) || IsTransferFinished (Context)) {
    FinishTransfer (Context);
    return;
  }
  WriteFifo (Context);
  ReadFifo (Context);
  CheckErrors (Context);
}

/**
  Reset the I2C controller

  @param[in] MmioAddress    Address of I2C controller

  @retval Status
**/
EFI_STATUS
I2cReset (
  IN UINT64  MmioAddress
  )
{
  SERIAL_IO_HOST_CONTROL_RESET        I2cHostControllerResetReg;

  I2cHostControllerResetReg.Data32  = 0x0;

  I2cHostControllerResetReg.Data32 = MmioRead32 ((UINTN) MmioAddress + R_SERIAL_IO_MEM_RESETS);
  I2cHostControllerResetReg.Fields.HostControlReset  = 0x0;  // Reset Asserted
  I2cHostControllerResetReg.Fields.DmaReset          = 0x0;  // Reset Asserted
  MmioWrite32 ((UINTN) MmioAddress + R_SERIAL_IO_MEM_RESETS, I2cHostControllerResetReg.Data32);

  I2cHostControllerResetReg.Data32 = MmioRead32 ((UINTN) MmioAddress + R_SERIAL_IO_MEM_RESETS);
  I2cHostControllerResetReg.Fields.HostControlReset  = 0x3;  // Reset Released
  I2cHostControllerResetReg.Fields.DmaReset          = 0x1;  // Reset Released
  MmioWrite32 ((UINTN)MmioAddress + R_SERIAL_IO_MEM_RESETS, I2cHostControllerResetReg.Data32);

  return EFI_SUCCESS;
}


