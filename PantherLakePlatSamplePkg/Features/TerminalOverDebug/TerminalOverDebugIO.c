/** @file
  IO implementation for DCI mailbox.

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

@par Specification
**/

#include "TerminalOverDebug.h"
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <DciConfig.h>
#include <Library/BaseDciInfoLib.h>

//
// Define a union for byte access for double word.
//
 typedef union {
  UINT32        DwData;
  unsigned char ByteData[4];
} ONE_PACKET;


//
// Interface Functions
//
/**
  Reset DCI IO device.

  @param This                Pointer to EFI_DCI_IO_PROTOCOL

  @retval EFI_SUCCESS        Reset successfully
  @retval EFI_DEVICE_ERROR   Failed to reset

**/
EFI_STATUS
EFIAPI
DciMbReset(
  IN EFI_DCI_IO_PROTOCOL           *This
  )
{
  return EFI_SUCCESS;
}

/**
  Set new attributes to a serial device.

  @param  This                       Pointer to EFI_SERIAL_IO_PROTOCOL
  @param  ReceiveFifoDepth          The depth of receive FIFO buffer
  @param  Timeout                   The request timeout for a single char

  @retval  EFI_SUCCESS              The new attributes were set
  @retval  EFI_INVALID_PARAMETERS   One or more attributes have an unsupported value
  @retval  EFI_UNSUPPORTED          Data Bits can not set to 5 or 6
  @retval  EFI_DEVICE_ERROR         The serial device is not functioning correctly (no return)

**/
EFI_STATUS
EFIAPI
DciMbSetAttributes (
  IN EFI_DCI_IO_PROTOCOL            *This,
  IN UINT32                         ReceiveFifoDepth,
  IN UINT32                         Timeout
 )
{
  return EFI_SUCCESS;
}

/**
  Set Control Bits.

  @param This              Pointer to EFI_SERIAL_IO_PROTOCOL
  @param Control           Control bits that can be settable

  @retval EFI_SUCCESS       New Control bits were set successfully
  @retval EFI_UNSUPPORTED   The Control bits wanted to set are not supported

**/
EFI_STATUS
EFIAPI
DciMbSetControl (
  IN EFI_DCI_IO_PROTOCOL            *This,
  IN UINT32                         Control
  )
{
  return EFI_SUCCESS;
}

/**
  Get Control Bits.

  @param This          Pointer to EFI_SERIAL_IO_PROTOCOL
  @param Control       Control signals of the serial device

  @retval EFI_SUCCESS   Get Control signals successfully

**/
EFI_STATUS
EFIAPI
DciMbGetControl (
  IN EFI_DCI_IO_PROTOCOL            *This,
  OUT UINT32                        *Control
  )
{
  return EFI_SUCCESS;
}

/**
  Write byte by byte to the TerminalOverDebug interface (ExIDci Tx Mailbox).

  @param This                Pointer to EFI_SERIAL_IO_PROTOCOL
  @param  BufferSize         On input the size of Buffer, on output the amount of
                             data actually written
  @param  Buffer             The buffer of data to write

  @retval EFI_SUCCESS        The data were written successfully
  @retval EFI_DEVICE_ERROR   The device reported an error
  @retval EFI_TIMEOUT        The write operation was stopped due to timeout

**/
EFI_STATUS
EFIAPI
DciMbWrite (
  IN EFI_DCI_IO_PROTOCOL         *This,
  IN OUT UINTN                   *BufferSize,
  IN VOID                        *Buffer
  )
{
  EFI_TPL                        Tpl;
  BOOLEAN                        RaiseTplNeeded = TRUE;

  if (*BufferSize == 0 || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //Raise TPL to prevent recursion from EFI timer interrupts.
  if (EfiGetCurrentTpl() > TPL_NOTIFY) {
    RaiseTplNeeded = FALSE;
  }

  if (RaiseTplNeeded) {
    Tpl = gBS->RaiseTPL(TPL_NOTIFY);
  }

  MmioWrite8 (GetDciMailboxAddr (Emb2), *(UINT8 *) Buffer);

  if (RaiseTplNeeded) {
    gBS->RestoreTPL(Tpl);
  }
  // 50us delay, for the Host to get the data from the ExIDci Mailbox.
  MicroSecondDelay (50);

  return EFI_SUCCESS;       // Returns success after putting the data to mailbox.
}

/**
  Read byte by byte from the TerminalOverDebug interface (ExIDci Rx Mailbox).

  @param This               Pointer to EFI_SERIAL_IO_PROTOCOL
  @param BufferSize         On input the size of Buffer, on output the amount of
                            data returned in buffer
  @param Buffer             The buffer to return the data into

  @retval EFI_SUCCESS        The data were read successfully
  @retval EFI_DEVICE_ERROR   The device reported an error
  @retval EFI_TIMEOUT        The read operation was stopped due to timeout

**/
EFI_STATUS
EFIAPI
DciMbRead (
  IN EFI_DCI_IO_PROTOCOL          *This,
  IN OUT UINTN                    *BufferSize,
  OUT VOID                        *Buffer
  )
{
  EFI_TPL                          Tpl;
  ONE_PACKET                       RcvData;
  UINT64                           DciMailboxAddr = GetDciMailboxAddr (Emb1);
  BOOLEAN                          RaiseTplNeeded = TRUE;
  *BufferSize = 1;

  //Raise TPL to prevent recursion from EFI timer interrupts
  if (EfiGetCurrentTpl() > TPL_NOTIFY) {
    RaiseTplNeeded = FALSE;
  }

  if (RaiseTplNeeded) {
    Tpl = gBS->RaiseTPL(TPL_NOTIFY);
  }

  // Read from DCI Rx MailBox
  RcvData.DwData =  MmioRead32(DciMailboxAddr);
  if( RcvData.ByteData[0] > 0 ){
      *(UINT8 *) Buffer = RcvData.ByteData[1];
      MmioWrite32(DciMailboxAddr,0); // Writing 0 indicating BIOS read the data
  }

  if (RaiseTplNeeded) {
    gBS->RestoreTPL(Tpl);
  }
  return EFI_SUCCESS;
}
