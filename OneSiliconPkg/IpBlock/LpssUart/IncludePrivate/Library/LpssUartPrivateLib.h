/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef _LPSS_UART_PRIVATE_LIB_H_
#define _LPSS_UART_PRIVATE_LIB_H_

#include <LpssUartPriv.h>
#include <Protocol/SerialIo.h>

/**
  Register access helper. Depending on SerialIO UART mode,
  its registers are aligned to 1 or 4 bytes and have 8 or 32bit size

  @param[in]  AccessMode          Selects between 8bit access to 1-byte aligned registers or 32bit access to 4-byte algined
  @param[in]  MmioBaseAddress     MMIO Base address
  @param[in]  Offset              Register offset in 8bit mode
  @param[in]  Data                Data to be written
**/
VOID
LpssUartWriteRegister (
  IN UINTN            MmioBaseAddress,
  IN UINTN            Offset,
  IN UINT8            Data
  );

/**
  Register access helper. Depending on SerialIO UART mode,
  its registers are aligned to 1 or 4 bytes and have 8 or 32bit size

  @param[in]  AccessMode          Selects between 8bit access to 1-byte aligned registers or 32bit access to 4-byte algined
  @param[in]  MmioBaseAddress     MMIO Base address
  @param[in]  Offset              Register offset in 8bit mode

  @retval                         retrieved register value, always 8bit regardless of access mode
**/
UINT8
LpssUartReadRegister (
  IN UINTN            MmioBaseAddress,
  IN UINTN            Offset
  );

/**
  Enables/Disables Auto Flow Control

  @param[in]  MmioBaseAddress     MMIO Base address
  @param[in]  AutoFlow            TRUE - Enable, FALSE - Disable
**/
VOID
LpssUartSetAutoFlow (
  IN UINTN                  MmioBaseAddress,
  IN BOOLEAN                AutoFlow
  );

/**
  Returns Auto Flow Control state

  @param[in]  MmioBaseAddress     MMIO Base address

  @retval     TRUE                AutoFlow Enabled
  @retval     FALSE               AutoFlow Disabled
**/
BOOLEAN
LpssUartGetAutoFlow (
  IN UINTN                  MmioBaseAddress
  );

/**
  Sets Stop Bits

  @param[in]  MmioBaseAddress     MMIO Base address

  @param[in]  StopBits            Stop Bits
**/
VOID
LpssUartSetStopBits (
  IN UINTN                  MmioBaseAddress,
  IN EFI_STOP_BITS_TYPE     StopBits
  );

/**
  Returns Stop Bits

  @param[in]  MmioBaseAddress     MMIO Base address

  @retval     StopBits            Default/One/OneFive/Two
**/
EFI_STOP_BITS_TYPE
LpssUartGetStopBits (
  IN UINTN                  MmioBaseAddress
  );

/**
  Sets Data Bits

  @param[in]  MmioBaseAddress     MMIO Base address
  @param[in]  DataBits            Data Bits 5/6/7/8
**/
VOID
LpssUartSetDataBits (
  IN UINTN                  MmioBaseAddress,
  IN UINT8                  DataBits
  );

/**
  Returns Data Bits

  @param[in]  MmioBaseAddress     MMIO Base address

  @retval     DataBits            5/6/7/8
**/
UINT8
LpssUartGetDataBits (
  IN UINTN                  MmioBaseAddress
  );

/**
  Sets Parity

  @param[in]  MmioBaseAddress     MMIO Base address
  @param[in]  Parity              Parity
**/
VOID
LpssUartSetParity (
  IN UINTN                  MmioBaseAddress,
  IN EFI_PARITY_TYPE        Parity
  );

/**
  Returns Parity

  @param[in]  MmioBaseAddress     MMIO Base address

  @retval     Parity              Even/Odd/None
**/
EFI_PARITY_TYPE
LpssUartGetParity (
  IN UINTN                  MmioBaseAddress
  );

/**
  Sets BaudRate along with adjusted Clock M/N divider

  @param[in]  MmioBaseAddress     MMIO Base address
  @param[in]  BaudRate            Baud Rate
**/
VOID
LpssUartSetBaudRate (
  IN UINTN                  MmioBaseAddress,
  IN UINT64                 BaudRate
  );

/**
  Returns current BaudRate

  @param[in]  MmioBaseAddress     MMIO Base address
  @retval     BaudRate            Baud Rate
**/
UINT64
LpssUartGetBaudRate (
  IN UINTN                  MmioBaseAddress
  );

/**
  Enables Fifo

  @param[in]  MmioBaseAddress     MMIO Base address
**/
VOID
LpssUartFifoEnable (
  IN UINTN                  MmioBaseAddress
  );

/**
  Sets the baud rate, parity, data bits, flow control and stop bits on a serial device.

  @param[in]  MmioBaseAddress     MMIO Base address

  @param[in]  BaudRate            The requested baud rate. A BaudRate value of 0 will use the the
                                  device's default interface speed.
  @param[in]  Parity              The type of parity to use on this serial device. A Parity value of
                                  DefaultParity will use the device's default parity value.
  @param[in]  DataBits            The number of data bits to use on the serial device. A DataBits
                                  value of 0 will use the device's default data bit setting.
  @param[in]  StopBits            The number of stop bits to use on this serial device. A StopBits
                                  value of DefaultStopBits will use the device's default number of
                                  stop bits.
  @param[in]  AutoFlow            Enable/Disable Auto Flow Control
**/
VOID
LpssUartSetAttributes (
  IN UINTN                  MmioBaseAddress,
  IN UINT64                 BaudRate,
  IN EFI_PARITY_TYPE        Parity,
  IN UINT8                  DataBits,
  IN EFI_STOP_BITS_TYPE     StopBits,
  IN BOOLEAN                AutoFlow
  );

/**
  Write data to serial device.

  If the buffer is NULL, then return 0;
  if NumberOfBytes is zero, then return 0.

  @param[in]  MmioBaseAddress       MMIO Base address
  @param[in]  Buffer                Data pointer
  @param[in]  NumberOfBytes         Number of output bytes which are cached in Buffer.

  @retval                           Actual number of bytes written to serial device.
**/
UINTN
EFIAPI
LpssUartWrite (
  IN UINTN            MmioBaseAddress,
  IN UINT8            *Buffer,
  IN UINTN            NumberOfBytes
  );


/**
  Read data from serial device and save the data in buffer.

  If the buffer is NULL, then return 0;
  if NumberOfBytes is 0, then return 0.

  @param[in]   MmioBaseAddress      MMIO Base address
  @param[out]  Buffer               Pointer to the buffer that will contain the return data
  @param[in]   NumberOfBytes        Number of input bytes to read
  @param[in]   WaitUntilBufferFull  TRUE  - function waits until whole buffer is filled.
                                    FALSE - function returns as soon as no new characters are available..

  @retval                           Actual number of bytes raed to serial device.
**/
UINTN
EFIAPI
LpssUartRead (
  IN  UINTN            MmioBaseAddress,
  OUT UINT8            *Buffer,
  IN  UINTN            NumberOfBytes,
  IN  BOOLEAN          WaitUntilBufferFull
  );

#endif
