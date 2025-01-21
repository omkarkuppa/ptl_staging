/** @file
  Serial IO UART Private Lib.
  All function in this library is available for PEI, DXE, and SMM,

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

#ifndef _SERIAL_IO_UART_PRIVATE_LIB_H_
#define _SERIAL_IO_UART_PRIVATE_LIB_H_

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/SerialIoAccessLib.h>
#include <Library/TimerLib.h>
#include <Library/SerialIoPrivateLib.h>
#include <Register/PchRegs.h>
#include <Register/SerialIoRegs.h>
#include <Register/SerialIoUartRegs.h>
#include <IndustryStandard/Pci30.h>
#include <Protocol/SerialIo.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PchInfoLib.h>

#define SOURCE_CLOCK   100000000
#define MAX_BAUDRATE   6000000

#define SERIALIO_UART_TIMEOUT_DELAY_INTERVAL 10 //1000 when set as BASE

typedef enum {
  AccessMode8bit,
  AccessMode32bit
} UART_ACCESS_MODE;

// 0x02 Interrupt Control Register.
typedef union {
  struct {
    UINT8  IID    : 4;    //[RO][0:3] Interrupt ID. This indicates the highest priority pending interrupt which can be one of the following types:
                          //        0000 = modem status
                          //        0001 = no interrupt pending
                          //        0010 = THR empty
                          //        0100 = received data available
                          //        0110 = receiver line status
                          //        0111 = busy detect
                          //        1100 = character timeout
    UINT8  Reserved : 2;  //[N/A][4:5]
    UINT8  FIFOSE : 2;    //[RO][6:7] FIFOs Enabled. This is used to indicate whether the FIFOs are enabled or disabled.
                          //        00 = disabled
                          //        11 = enabled
  } Fields;
  UINT8 Data;
} SERIAL_IO_UART_IIR;

// 0x02 FIFO Control Register
typedef union {
  struct {
    UINT8  FIFOE  : 1;    //[WO][0] Data Length Select.00 = 5 bits 01 = 6 bits 10 = 7 bits 11 = 8 bits
    UINT8  RFIFOR : 1;    //[WO][1] Number of stop bits 0 = 1 stop bit 1 = 1.5 stop bits when DLS (LCR[1:0]) is zero, else 2 stop bit
    UINT8  XFIFOR : 1;    //[WO][2] Parity Enable
    UINT8  DMAM   : 1;    //[WO][3] Even and odd parity, when parity is enabled (PEN set to one)
    UINT8  TET    : 2;    //[WO][4:5] TX Empty Trigger.
    UINT8  RT     : 2;    //[WO][6:7] RCVR Trigger. This is used to select the trigger level in the receiver FIFO at which the
                          //        Received Data Available Interrupt is generated.
  } Fields;
  UINT8 Data;
} SERIAL_IO_UART_FCR;

// 0x03 Line Control Register
typedef union {
  struct {
    UINT8  DLS   : 2;    //[RW][0:1] Data Length Select.00 = 5 bits 01 = 6 bits 10 = 7 bits 11 = 8 bits
    UINT8  STOP  : 1;    //[RW][2] Number of stop bits 0 = 1 stop bit 1 = 1.5 stop bits when DLS (LCR[1:0]) is zero, else 2 stop bit
    UINT8  PEN   : 1;    //[RW][3] Parity Enable
    UINT8  EPS   : 1;    //[RW][4] Even and odd parity, when parity is enabled (PEN set to one)
    UINT8  Reserved  : 1;//[N/A][5]
    UINT8  BREAK : 1;    //[RW][6] This is used to cause a break condition to be transmitted to the receiving device.
    UINT8  DLAB  : 1;    //[RW][7] Divisor Latch Access Bit used to enable reading and writing of the
                         //      Divisor Latch register(DLL and DLH) to set the baud rate
  } Fields;
  UINT8 Data;
} SERIAL_IO_UART_LCR;

// 0x04 Modem Control Register
typedef union {
  struct {
    UINT8  Reserved0  : 1;    //[N/A][0]
    UINT8  RTS        : 1;    //[RW][1] Request to Send. When Auto RTS Flow Control is not enabled (MCR[5] set to zero), the rts_n
                              //      signal is set low by programming MCR[1] (RTS) to a high. In Auto Flow Control,
                              //      (MCR[5] set to one) and FIFOs enable (FCR[0] set to one), the rts_n output is
                              //      controlled in the same way, but is also gated with the receiver FIFO threshold
                              //      trigger (rts_n is inactive high when above the threshold). The rts_n signal is deasserted when MCR[1] is set low.
    UINT8  Reserved1  : 2;    //[N/A][2:3]
    UINT8  LOOPBACK   : 1;    //[RW][4] LoopBack Bit. This is used to put the UART into a diagnostic mode.
    UINT8  AFCE       : 1;    //[RW][5] Auto Flow Control Enable. When FIFOs are enabled and the Auto Flow Control Enable (AFCE) bit is set.
                              //        The bit is used to help for flow control using external IO pins with the pairing device.
                              // 0 = Auto Flow Control Mode disabled
                              // 1 = Auto Flow Control Mode enabled
    UINT8  Reserved2  : 2;    //[N/A][6:7]
  } Fields;
  UINT8 Data;
} SERIAL_IO_UART_MCR;

// 0x05 Line Status Register
typedef union {
  struct {
    UINT8  DR   : 1;    //[RW][0] Data Ready. sed to indicate that the receiver contains at least one character in the RBR or the receiver FIFO
                        //                 0 = no data ready
                        //                 1 = data ready
                        //      This bit is cleared when the RBR is read in non-FIFO mode, or when the receiver
                        //      FIFO is empty, in FIFO mode.
    UINT8  OE   : 1;    //[RW][1] Overrun error
    UINT8  PE   : 1;    //[RW][2] Parity Error bit
    UINT8  FE   : 1;    //[RW][3] Framing Error bit
    UINT8  BI   : 1;    //[RW][4] Break Interrupt bit
    UINT8  THRE : 1;    //[RW][5] Transmit Holding Register Empty bit
    UINT8  TEMT : 1;    //[RW][6] Transmitter Empty bit
    UINT8  RFE  : 1;    //[RW][7] Receiver FIFO Error bit
  } Fields;
  UINT8 Data;
} SERIAL_IO_UART_LSR;

// 0x1F UART Status Register
typedef union {
  struct {
    UINT8  BUSY : 1;    //[RO][0] N/A
    UINT8  TFNF : 1;    //[RO][1] Transmit FIFO Not Full. This is used to indicate that the transmit FIFO in not full.
                        //      0 = Transmit FIFO is full
                        //      1 = Transmit FIFO is not full
                        //      This bit is cleared when the TX FIFO is full
    UINT8  TFE  : 1;    //[RO][2] Transmit FIFO Empty. This is used to indicate that the transmit FIFO is completely empty.
                        //      0 = Transmit FIFO is not empty
                        //      1 = Transmit FIFO is empty
                        //      This bit is cleared when the TX FIFO is no longer empty.
    UINT8  RFNE : 1;    //[RO][3] Receive FIFO Not Empty. This is used to indicate that the receive FIFO contains one or more entries.
                        //      0 = Receive FIFO is empty
                        //      1 = Receive FIFO is not empty
                        //      This bit is cleared when the RX FIFO is empty
    UINT8  RFF  : 1;    //[RO][4] Receive FIFO Full. This is used to indicate that the receive FIFO is completely full.
                        //      0 = Receive FIFO not full
                        //      1 = Receive FIFO Full
                        //      This bit is cleared when the RX FIFO is no longer full
    UINT8  Reserved : 3;//[N/A][5:7]
  } Fields;
  UINT8 Data;
} SERIAL_IO_UART_USR;

// 0x200 PRV_CLOCK_PARAMS - m/n divider control register
typedef union {
  struct {
    UINT32 CLK_EN     : 1;     //[RW][0] 0 - No clock Update, 1 - Clock gets updated. Note: For new M and N values to be used by the divider, the
                               //      clk_update bit should be set to 0 & then set to 1.
    UINT32 M          : 15;    //[RW][1:15] M over N divider logic that creates the CLK_OUT. Used to generate the input clk to the UART.
    UINT32 N          : 15;    //[RW][16:31] M over N divider logic that creates the CLK_OUT. Used to generate the input clk to the UART.
    UINT32 CLK_UPDATE : 1;     //[RW][32]  0 - Clock disabled 1 - Clock Enabled.
  } Fields;
  UINT32 Data;
} SERIAL_IO_UART_CLOCK;


/**
  Program BaudRate along with adjusted Clock M/N divider

  @param[in]  BaseAddress      Base Address Regiser offset (BAR0)
  @param[in]  AccessMode       8bit/32bit register layout
  @param[in]  BaudRate         Baud Rate

**/
VOID
SerialIoUartProgramBaudRate (
  IN UINT64                BaseAddress,
  IN UART_ACCESS_MODE      AccessMode,
  IN UINT64                BaudRate
  );

/**
  Program Stop Bits

  @param[in]  BaseAddress      Base Address Regiser offset (BAR0)
  @param[in]  AccessMode       8bit/32bit register layout
  @param[in]  StopBits         Stop Bits

**/
VOID
SerialIoUartProgramStopBits (
  IN UINT64                 BaseAddress,
  IN UART_ACCESS_MODE       AccessMode,
  IN EFI_STOP_BITS_TYPE     StopBits
  );

/**
  Program Parity

  @param[in]  BaseAddress      Base Address Regiser offset (BAR0)
  @param[in]  AccessMode       8bit/32bit register layout
  @param[in]  Parity           Parity
**/
VOID
SerialIoUartProgramParity (
  IN UINT64                 BaseAddress,
  IN UART_ACCESS_MODE       AccessMode,
  IN EFI_PARITY_TYPE        Parity
  );

/**
  Program FIFO Enabled

  @param[in]  BaseAddress      Base Address Regiser offset (BAR0)
  @param[in]  AccessMode       8bit/32bit register layout
**/
EFI_STATUS
SerialIoUartProgramFifoEnable (
  IN UINT64               BaseAddress,
  IN UART_ACCESS_MODE     AccessMode
);

/**
  Enables/Disables Auto Flow Control

  @param[in]  BaseAddress      UART Base Address
  @param[in]  AccessMode       8bit/32bit register layout
  @param[in]  AutoFlow         TRUE - Enable, FALSE - Disable

**/
VOID
SerialIoUartProgramAutoFlow (
  UINT64                 BaseAddress,
  UART_ACCESS_MODE       AccessMode,
  IN BOOLEAN             AutoFlow
  );

/**
  Enables UART regardless of the current mode 8/32bit
  - Enables MSE
  - Places device in D0
  - Gets Controller out of Reset

  This function will not assign new BAR.

  @param[in]      PciCfgBase     PCI Config Base address
  @param[in]      *UartNumber    Optional: Selects Serial IO UART device.
                                 If not provided, PciCfgBase is used instead.
  @param[in/out]  BaseAddress    Base address of UART MMIO space
  @param[in/out]  AccessMode     Selects between 8bit access to 1-byte aligned registers or 32bit access to 4-byte algined

  @retval   TRUE             UART is Enabled, Out Of reset, MMIO is available
  @retval   FALSE            Unable to access UART's MMIO
**/
BOOLEAN
SerialIoUartEnable (
  IN UINT64                PciCfgBase,
  IN UINT8                 *UartNumber,     OPTIONAL
  IN OUT UINT64            *BaseAddress,
  IN OUT UART_ACCESS_MODE  *AccessMode
  );

/**
  Register access helper. Depending on SerialIO UART mode,
  its registers are aligned to 1 or 4 bytes and have 8 or 32bit size

  @param[in]  AccessMode     Selects between 8bit access to 1-byte aligned registers or 32bit access to 4-byte algined
  @param[in]  BaseAddress    Base address of UART MMIO space
  @param[in]  Offset         Register offset in 8bit mode

  @retval                    retrieved register value, always 8bit regardless of access mode
**/
UINT8
SerialIoUartReadRegister (
  IN UART_ACCESS_MODE AccessMode,
  IN UINT64           BaseAddress,
  IN UINTN            Offset
  );

/**
  Writes data to the serial device only if Fifo is ready for it

  @param[in]  AccessMode       8bit/32bit register layout
  @param[in]  BaseAddress      Base Address Regiser offset (BAR0)
  @param[in]  Data             Data to transmit

  @retval TRUE             Data was sent out
          FALSE            No data transmitted
**/
BOOLEAN
SerialIoUartTransmit (
  IN UART_ACCESS_MODE  AccessMode,
  IN UINT64            BaseAddress,
  IN UINT8             Data
  );

/**
  Sets BaudRate along with adjusted Clock M/N divider

  @param[in]  PciCfgBase     PCI Config Base address
  @param[in]  *UartNumber    Optional: Selects Serial IO UART device.
                             If not provided, PciCfgBase is used instead.
  @param[in]  BaudRate         Baud Rate

**/
VOID
SerialIoUartSetBaudRate (
  IN UINT64  PciCfgBase,
  IN UINT8   *UartNumber,     OPTIONAL
  IN UINT64  BaudRate
  );

/**
  Returns current BaudRate

  @param[in]  PciCfgBase     PCI Config Base address
  @param[in]  *UartNumber    Optional: Selects Serial IO UART device.
                             If not provided, PciCfgBase is used instead.

  @retval     BaudRate         Baud Rate
**/
UINT64
SerialIoUartGetBaudRate (
  IN UINT64  PciCfgBase,
  IN UINT8   *UartNumber     OPTIONAL
  );

/**
  Sets Stop Bits

  @param[in]  PciCfgBase     PCI Config Base address
  @param[in]  *UartNumber    Optional: Selects Serial IO UART device.
                             If not provided, PciCfgBase is used instead.
  @param[in]  StopBits       Stop Bits

**/
VOID
SerialIoUartSetStopBits (
  IN UINT64              PciCfgBase,
  IN UINT8               *UartNumber,     OPTIONAL
  IN EFI_STOP_BITS_TYPE  StopBits
  );

/**
  Returns Stop Bits

  @param[in]  PciCfgBase     PCI Config Base address
  @param[in]  *UartNumber    Optional: Selects Serial IO UART device.
                             If not provided, PciCfgBase is used instead.

  @retval     StopBits         Default/One/OneFive/Two
**/
EFI_STOP_BITS_TYPE
SerialIoUartGetStopBits (
  IN UINT64  PciCfgBase,
  IN UINT8   *UartNumber     OPTIONAL
  );

/**
  Program Data Bits

  @param[in]  BaseAddress      UART Base Address
  @param[in]  AccessMode       8bit/32bit register layout
  @param[in]  DataBits       Data Bits 5/6/7/8

**/
VOID
SerialIoUartProgramDataBits (
  IN UINT64            BaseAddress,
  IN UART_ACCESS_MODE  AccessMode,
  IN UINT8             DataBits
  );

/**
  Sets Data Bits

  @param[in]  PciCfgBase     PCI Config Base address
  @param[in]  *UartNumber    Optional: Selects Serial IO UART device.
                             If not provided, PciCfgBase is used instead.
  @param[in]  DataBits       Data Bits 5/6/7/8

**/
VOID
SerialIoUartSetDataBits (
  IN UINT64  PciCfgBase,
  IN UINT8   *UartNumber,     OPTIONAL
  IN UINT8   DataBits
  );

/**
  Returns Data Bits

  @param[in]  PciCfgBase     PCI Config Base address
  @param[in]  *UartNumber    Optional: Selects Serial IO UART device.
                             If not provided, PciCfgBase is used instead.

  @retval     DataBits         5/6/7/8
**/
UINT8
SerialIoUartGetDataBits (
  IN UINT64  PciCfgBase,
  IN UINT8   *UartNumber     OPTIONAL
  );

/**
  Sets Parity

  @param[in]  PciCfgBase     PCI Config Base address
  @param[in]  *UartNumber    Optional: Selects Serial IO UART device.
                             If not provided, PciCfgBase is used instead.
  @param[in]  Parity         Parity
**/
VOID
SerialIoUartSetParity (
  IN UINT64              PciCfgBase,
  IN UINT8              *UartNumber,     OPTIONAL
  IN EFI_PARITY_TYPE     Parity
  );

/**
  Returns Parity

  @param[in]  PciCfgBase     PCI Config Base address
  @param[in]  *UartNumber    Optional: Selects Serial IO UART device.
                             If not provided, PciCfgBase is used instead.

  @retval     Parity           Even/Odd/None
**/
EFI_PARITY_TYPE
SerialIoUartGetParity (
  IN UINT64  PciCfgBase,
  IN UINT8   *UartNumber     OPTIONAL
  );

/**
  Enables Fifo

  @param[in]  PciCfgBase     PCI Config Base address
  @param[in]  *UartNumber    Optional: Selects Serial IO UART device.
                             If not provided, PciCfgBase is used instead.
**/
VOID
SerialIoUartFifoEnable (
  IN UINT64  PciCfgBase,
  IN UINT8   *UartNumber     OPTIONAL
  );

/**
  Returns FIFOE state

  @param[in]  PciCfgBase     PCI Config Base address
  @param[in]  *UartNumber    Optional: Selects Serial IO UART device.
                             If not provided, PciCfgBase is used instead.
**/
BOOLEAN
SerialIoUartIsFifoEnabled (
  IN UINT64  PciCfgBase,
  IN UINT8   *UartNumber     OPTIONAL
  );

/**
  Returns Auto Flow Control state

  @param[in]  BaseAddress      UART Base Address
  @param[in]  AccessMode       8bit/32bit register layout

  @retval     TRUE             AutoFlow Enabled
  @retval     FALSE            AutoFlow Disabled
**/
BOOLEAN
SerialIoUartReadAutoFlow (
  UINT64              BaseAddress,
  UART_ACCESS_MODE    AccessMode
  );

/**
  Enables/Disables Auto Flow Control

  @param[in]  PciCfgBase     PCI Config Base address
  @param[in]  *UartNumber    Optional: Selects Serial IO UART device.
                             If not provided, PciCfgBase is used instead.
  @param[in]  AutoFlow       TRUE - Enable, FALSE - Disable
**/
VOID
SerialIoUartSetAutoFlow (
  IN UINT64              PciCfgBase,
  IN UINT8               *UartNumber,     OPTIONAL
  IN BOOLEAN             AutoFlow
  );

/**
  Returns Auto Flow Control state

  @param[in]  PciCfgBase     PCI Config Base address
  @param[in]  *UartNumber    Optional: Selects Serial IO UART device.
                             If not provided, PciCfgBase is used instead.
**/
BOOLEAN
SerialIoUartGetAutoFlow (
  IN UINT64  PciCfgBase,
  IN UINT8   *UartNumber     OPTIONAL
  );

/**
  Checks if Tx Fifo is not full

  @param[in]  AccessMode       8bit/32bit register layout
  @param[in]  BaseAddress      Base Address Regiser offset (BAR0)

  @retval TRUE                 Fifo is not full
          FALSE                Fifo is full
**/
BOOLEAN
SerialIoUartIsTxFifoNotFull (
  IN UART_ACCESS_MODE AccessMode,
  IN UINT64           BaseAddress
  );

/**
  Register access helper. Depending on SerialIO UART mode,
  its registers are aligned to 1 or 4 bytes and have 8 or 32bit size

  @param[in]  AccessMode     Selects between 8bit access to 1-byte aligned registers or 32bit access to 4-byte algined
  @param[in]  BaseAddress    Base address of UART MMIO space
  @param[in]  Offset         Register offset in 8bit mode
  @param[in]  Data           Data to be written
**/
VOID
SerialIoUartWriteRegister (
  IN UART_ACCESS_MODE AccessMode,
  IN UINT64           BaseAddress,
  IN UINTN            Offset,
  IN UINT8            Data
  );

  /**
  Write data to serial device.

  If the buffer is NULL, then return 0;
  if NumberOfBytes is zero, then return 0.

  @param[in]  AccessMode       Selects between 8bit access to 1-byte aligned registers or 32bit access to 4-byte algined
  @param[in]  BaseAddress      Base address of UART MMIO space
  @param[in]  Buffer           Data pointer
  @param[in]  NumberOfBytes    Number of output bytes which are cached in Buffer.

  @retval                      Actual number of bytes written to serial device.
**/
UINTN
EFIAPI
SerialIoUartWritePriv (
  IN UART_ACCESS_MODE  AccessMode,
  IN UINT64            BaseAddress,
  IN UINT8             *Buffer,
  IN UINTN             NumberOfBytes
  );

#endif
