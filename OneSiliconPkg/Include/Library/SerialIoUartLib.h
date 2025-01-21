/** @file
  Header file for Serial IO UART Lib implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#ifndef _SERIAL_IO_UART_LIB_H_
#define _SERIAL_IO_UART_LIB_H_

#include <Protocol/SerialIo.h>

/**
  Polls a serial device to see if there is any data waiting or pending for read.
  If there is data pending, then TRUE is returned.
  If there is no data, then FALSE is returned.

  @param[in]  PciCfgBase     PCI Config Base address
  @param[in]  *UartNumber    Optional: Selects Serial IO UART device.
                             If not provided, PciCfgBase is used instead.

  @retval TRUE             Data is waiting to read from the serial device.
  @retval FALSE            There is no data waiting to read from the serial device.
**/
BOOLEAN
EFIAPI
SerialIoUartPolling (
  IN UINT64  PciCfgBase,
  IN UINT8   *UartNumber     OPTIONAL
  );

/**
  Write data to serial device.

  If the buffer is NULL, then return 0;
  if NumberOfBytes is zero, then return 0.

  @param[in]  PciCfgBase       PCI Config Base address
  @param[in]  *UartNumber      Optional: Selects Serial IO UART device.
                               If not provided, PciCfgBase is used instead.
  @param[in]  Buffer           Data pointer
  @param[in]  NumberOfBytes    Number of output bytes which are cached in Buffer.

  @retval                      Actual number of bytes written to serial device.
**/
UINTN
EFIAPI
SerialIoUartWrite (
  IN UINT64           PciCfgBase,
  IN UINT8            *UartNumber,     OPTIONAL
  IN UINT8            *Buffer,
  IN UINTN            NumberOfBytes
  );

/**
  Read data from serial device and save the data in buffer.

  If the buffer is NULL, then return 0;
  if NumberOfBytes is 0, then return 0.

  @param[in]   PciCfgBase           PCI Config Base address
  @param[in]   *UartNumber          Optional: Selects Serial IO UART device.
                                    If not provided, PciCfgBase is used instead.
  @param[out]  Buffer               Pointer to the buffer that will contain the return data
  @param[in]   NumberOfBytes        Number of input bytes to read
  @param[in]   WaitUntilBufferFull  TRUE  - function waits until whole buffer is filled.
                                    FALSE - function returns as soon as no new characters are available.

  @retval                           Actual number of bytes raed to serial device.
**/
UINTN
EFIAPI
SerialIoUartRead (
  IN UINT64            PciCfgBase,
  IN UINT8             *UartNumber,     OPTIONAL
  OUT UINT8            *Buffer,
  IN  UINTN            NumberOfBytes,
  IN  BOOLEAN          WaitUntilBufferFull
  );


/**
  Sets the baud rate, parity, data bits, flow control and stop bits on a serial device.

  @param[in]  PciCfgBase   PCI Config Base address
  @param[in]  *UartNumber  Optional: Selects Serial IO UART device.
                           If not provided, PciCfgBase is used instead.
  @param[in]  BaudRate     The requested baud rate. A BaudRate value of 0 will use the the
                           device's default interface speed.
  @param[in]  Parity       The type of parity to use on this serial device. A Parity value of
                           DefaultParity will use the device's default parity value.
  @param[in]  DataBits     The number of data bits to use on the serial device. A DataBits
                           value of 0 will use the device's default data bit setting.
  @param[in]  StopBits     The number of stop bits to use on this serial device. A StopBits
                           value of DefaultStopBits will use the device's default number of
                           stop bits.
  @param[in]  AutoFlow     Enable/Disable Auto Flow Control
**/
VOID
SerialIoUartSetAttributes (
  IN UINT64              PciCfgBase,
  IN UINT8               *UartNumber,     OPTIONAL
  IN UINT64              BaudRate,
  IN EFI_PARITY_TYPE     Parity,
  IN UINT8               DataBits,
  IN EFI_STOP_BITS_TYPE  StopBits,
  IN BOOLEAN             AutoFlow
  );

/**
  Dummy Read required after setting/clearing 16550 8bit mode

  @param[in]  PciCfgBase   PCI Config Base address
  @param[in]  *UartNumber  Optional: Selects Serial IO UART device.
                           If not provided, PciCfgBase is used instead.
**/
VOID
SerialIoUartDummyRead (
  IN UINT64           PciCfgBase,
  IN UINT8            *UartNumber     OPTIONAL
  );

/**
  Gets UART out of reset

  @param[in]  BaseAddress    Base address of UART MMIO space
**/
VOID
SerialIoUartGetOutOfReset (
  IN UINT64       MmioBaseAddress
  );

/**
  Return whether UART is in PCI mode or not

  @param[in]  PciCfgBase   PCI Config Base address
  @param[in]  *UartNumber  Optional: Selects Serial IO UART device.
                           If not provided, PciCfgBase is used instead.

  @retval                  TRUE  - UART is in PCI mode
  @retval                  FALSE - UART is in Hidden mode or disabled
**/
BOOLEAN
IsSerialIoUartInPciMode (
  IN UINT64           PciCfgBase,
  IN UINT8            *UartNumber     OPTIONAL
  );

/**
  Assign MMIO Resource in PCI Mode.

  @param[in]  PciCfgBase        PCI Config Base address
  @param[in]  *UartNumber       Optional: Selects Serial IO UART device.
                                If not provided, PciCfgBase is used instead.
  @param[in]  MmioBaseAddress   Base address of UART MMIO space

  @retval EFI_UNSUPPORTED       SerialIo UART isn't in PCI mode.
  @retval EFI_SUCCESS           Finish to set SerialIo UART MMIO in PCI mode.
**/
EFI_STATUS
SerialIoUartSetMmioInPciMode (
  IN UINT64           PciCfgBase,
  IN UINT8            *UartNumber,     OPTIONAL
  IN UINT64           MmioBaseAddress
  );

#endif // _SERIAL_IO_UART_LIB_H_
