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

#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/PeiAceInitLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/SerialIoUartSocLib.h>
#include <Protocol/SerialIo.h>
#include <Ppi/SiPolicy.h>
#include <Register/LpssUartRegs.h>
#include <Library/LpssUartLib.h>
#include <LpssSpiHandle.h>
#include <LpssUartConfig.h>
#include <LpssUartPriv.h>

#define LPSS_UART_SOURCE_CLOCK   100000000
#define LPSS_UART_MAX_BAUDRATE   6000000

#define LPSS_UART_TIMEOUT_DELAY_INTERVAL 10 //1000 when set as BASE

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
} LPSS_UART_FCR;

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
} LPSS_UART_LCR;

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
} LPSS_UART_MCR;

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
} LPSS_UART_LSR;

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
} LPSS_UART_USR;

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
} LPSS_UART_CLOCK;

/**
  Polls a serial device to see if there is any data waiting or pending for read.
  If there is data pending, then TRUE is returned.
  If there is no data, then FALSE is returned.

  @param[in]  MmioBaseAddress     MMIO Base address

  @retval TRUE             Data is waiting to read from the serial device.
  @retval FALSE            There is no data waiting to read from the serial device.
**/
BOOLEAN
EFIAPI
LpssUartPolling (
  IN UINTN            MmioBaseAddress
  )
{
  LPSS_UART_LSR        Lsr;

  //
  // Read the serial port status
  //
  Lsr.Data = LpssUartReadRegister (MmioBaseAddress, R_LPSS_UART_MEM_LSR);
  return (BOOLEAN) Lsr.Fields.DR;
}

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
  )
{
  UART_ACCESS_MODE    AccessMode;

  if (MmioRead32 (MmioBaseAddress + R_LPSS_UART_MEM_CTR) == UART_COMPONENT_IDENTIFICATION_CODE) {
    AccessMode = AccessMode32bit;
  } else {
    AccessMode = AccessMode8bit;
  }

  if (AccessMode == AccessMode32bit) {
    MmioWrite32 (MmioBaseAddress + Offset, Data);
  } else {
    MmioWrite8 (MmioBaseAddress + Offset / 4, Data);
  }
}

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
  )
{
  UART_ACCESS_MODE    AccessMode;

  if (MmioRead32 (MmioBaseAddress + R_LPSS_UART_MEM_CTR) == UART_COMPONENT_IDENTIFICATION_CODE) {
    AccessMode = AccessMode32bit;
  } else {
    AccessMode = AccessMode8bit;
  }

  if (AccessMode == AccessMode32bit) {
    return (UINT8) (0xFF & MmioRead32 (MmioBaseAddress + Offset));
  }
  return MmioRead8 (MmioBaseAddress + Offset / 4);
}

/**
  Enables/Disables Auto Flow Control

  @param[in]  MmioBaseAddress     MMIO Base address
  @param[in]  AutoFlow            TRUE - Enable, FALSE - Disable
**/
VOID
LpssUartSetAutoFlow (
  IN UINTN                  MmioBaseAddress,
  IN BOOLEAN                AutoFlow
  )
{
  LPSS_UART_MCR  Mcr;

  Mcr.Data = LpssUartReadRegister (MmioBaseAddress, R_LPSS_UART_MEM_MCR);
  Mcr.Fields.RTS  = 1;
  Mcr.Fields.AFCE = (AutoFlow ? 1 : 0);
  LpssUartWriteRegister (MmioBaseAddress, R_LPSS_UART_MEM_MCR, Mcr.Data);
}

/**
  Returns Auto Flow Control state

  @param[in]  MmioBaseAddress     MMIO Base address

  @retval     TRUE                AutoFlow Enabled
  @retval     FALSE               AutoFlow Disabled
**/
BOOLEAN
LpssUartGetAutoFlow (
  IN UINTN                  MmioBaseAddress
  )
{
  LPSS_UART_MCR       Mcr;

  Mcr.Data = LpssUartReadRegister (MmioBaseAddress, R_LPSS_UART_MEM_MCR);
  return (BOOLEAN) Mcr.Fields.AFCE;
}


/**
  Sets Stop Bits

  @param[in]  MmioBaseAddress     MMIO Base address

  @param[in]  StopBits            Stop Bits
**/
VOID
LpssUartSetStopBits (
  IN UINTN                  MmioBaseAddress,
  IN EFI_STOP_BITS_TYPE     StopBits
  )
{
  LPSS_UART_LCR             Lcr;

  if (StopBits > TwoStopBits) {
    return;
  }
  if (StopBits == DefaultStopBits) {
    StopBits = (EFI_STOP_BITS_TYPE) PcdGet8 (PcdUartDefaultStopBits);
  }

  Lcr.Data = LpssUartReadRegister (MmioBaseAddress, R_LPSS_UART_MEM_LCR);
  switch (StopBits) {
    default:
    case OneStopBit:
      Lcr.Fields.STOP = 0;
      break;
    case OneFiveStopBits:
    case TwoStopBits:
      Lcr.Fields.STOP = 1;
      break;
  }
  LpssUartWriteRegister (MmioBaseAddress, R_LPSS_UART_MEM_LCR, Lcr.Data);
}

/**
  Returns Stop Bits

  @param[in]  MmioBaseAddress     MMIO Base address

  @retval     StopBits            Default/One/OneFive/Two
**/
EFI_STOP_BITS_TYPE
LpssUartGetStopBits (
  IN UINTN             MmioBaseAddress
  )
{
  LPSS_UART_LCR       Lcr;

  Lcr.Data = LpssUartReadRegister (MmioBaseAddress, R_LPSS_UART_MEM_LCR);

  if (Lcr.Fields.STOP == 1) {
    if (Lcr.Fields.DLS == 0) { // DLS = 5 bits
      return OneFiveStopBits;
    }
    return TwoStopBits;
  }

  return OneStopBit;
}

/**
  Sets Data Bits

  @param[in]  MmioBaseAddress     MMIO Base address
  @param[in]  DataBits            Data Bits 5/6/7/8
**/
VOID
LpssUartSetDataBits (
  IN UINTN                  MmioBaseAddress,
  IN UINT8                  DataBits
  )
{
  LPSS_UART_LCR     Lcr;

  if (DataBits == 0) {
    DataBits = PcdGet8 (PcdUartDefaultDataBits);
  }

  Lcr.Data = LpssUartReadRegister (MmioBaseAddress, R_LPSS_UART_MEM_LCR);

  if ((DataBits < 5) || (DataBits > 8)) {
    return;
  }
  // Map 5..8 to 0..3
  Lcr.Fields.DLS = (UINT8) (DataBits - (UINT8) 5);
  LpssUartWriteRegister (MmioBaseAddress, R_LPSS_UART_MEM_LCR, Lcr.Data);
}

/**
  Returns Data Bits

  @param[in]  MmioBaseAddress     MMIO Base address

  @retval     DataBits            5/6/7/8
**/
UINT8
LpssUartGetDataBits (
  IN UINTN                 MmioBaseAddress
  )
{
  LPSS_UART_LCR  Lcr;

  Lcr.Data = LpssUartReadRegister (MmioBaseAddress, R_LPSS_UART_MEM_LCR);

  return (Lcr.Fields.DLS + (UINT8) 5);
}

/**
  Sets Parity

  @param[in]  MmioBaseAddress     MMIO Base address
  @param[in]  Parity              Parity
**/
VOID
LpssUartSetParity (
  IN UINTN                  MmioBaseAddress,
  IN EFI_PARITY_TYPE        Parity
  )
{
  LPSS_UART_LCR       Lcr;

  if (Parity == DefaultParity) {
    Parity = (EFI_PARITY_TYPE) PcdGet8 (PcdUartDefaultParity);
  }
  if (Parity > OddParity) { // Not supported
    return;
  }

  Lcr.Data = LpssUartReadRegister (MmioBaseAddress, R_LPSS_UART_MEM_LCR);

  switch (Parity) {
    default:
    case NoParity:
      Lcr.Fields.PEN = 0;
      Lcr.Fields.EPS = 0;
      break;
    case EvenParity:
      Lcr.Fields.PEN = 1;
      Lcr.Fields.EPS = 0;
      break;
    case OddParity:
      Lcr.Fields.PEN = 1;
      Lcr.Fields.EPS = 1;
      break;
  }

  LpssUartWriteRegister (MmioBaseAddress, R_LPSS_UART_MEM_LCR, Lcr.Data);
}

/**
  Returns Parity

  @param[in]  MmioBaseAddress     MMIO Base address

  @retval     Parity              Even/Odd/None
**/
EFI_PARITY_TYPE
LpssUartGetParity (
  IN UINTN                  MmioBaseAddress
  )
{
  LPSS_UART_LCR  Lcr;

  Lcr.Data = LpssUartReadRegister (MmioBaseAddress, R_LPSS_UART_MEM_LCR);

  if ((Lcr.Fields.PEN == 1) && (Lcr.Fields.EPS == 1)) {
    return OddParity;
  } else if ((Lcr.Fields.PEN == 1) && (Lcr.Fields.EPS == 0)) {
    return EvenParity;
  }
  return NoParity; // PEN =0/EPS = 0
}

/**
  Sets BaudRate along with adjusted Clock M/N divider

  @param[in]  MmioBaseAddress     MMIO Base address
  @param[in]  BaudRate            Baud Rate
**/
VOID
LpssUartSetBaudRate (
  IN UINTN                  MmioBaseAddress,
  IN UINT64                 BaudRate
  )
{
  UART_ACCESS_MODE      AccessMode;
  LPSS_UART_CLOCK       Clock;
  LPSS_UART_LCR         Lcr;
  UINT64                ComputedDivisor;
  UINT32                ClockRate;
  UINT16                ClockOffset;

  if (MmioRead32 (MmioBaseAddress + R_LPSS_UART_MEM_CTR) == UART_COMPONENT_IDENTIFICATION_CODE) {
    AccessMode = AccessMode32bit;
  } else {
    AccessMode = AccessMode8bit;
  }

  ClockOffset = R_LPSS_UART_MEM_CLOCKS;

  if (AccessMode == AccessMode8bit) {
    ClockOffset = ClockOffset / 4;
  }

  //
  // Default 115200 settings
  //
  ComputedDivisor = 1;
  ClockRate       = 1843200;

  //
  //  Set default Baudrate based on Pcd
  //
  if (BaudRate == 0) {
    BaudRate = (UINT32) PcdGet64 (PcdUartDefaultBaudRate);
  }

  if (BaudRate > LPSS_UART_MAX_BAUDRATE) {
    return;
  }

  //
  // Clock defaults
  //
  Clock.Data              = MmioRead32 (MmioBaseAddress + ClockOffset);
  Clock.Fields.N          = V_LPSS_UART_MEM_PPR_CLK_N_DIV;
  Clock.Fields.M          = V_LPSS_UART_MEM_PPR_CLK_M_DIV;
  Clock.Fields.CLK_EN     = 1;
  Clock.Fields.CLK_UPDATE = 1;

  //
  //  Low Speed mode
  //
  if (BaudRate < 115200) {
    //
    // Compute divisor use to program the baud rate using a round determination
    // Divisor = ClockRate / 16 / BaudRate = ClockRate / (16 * BaudRate)
    //         = ClockRate / (BaudRate << 4)
    //
    ComputedDivisor = DivU64x64Remainder (ClockRate, LShiftU64 (BaudRate, 4), NULL);
    //
    // If the computed divisor is 0, then use a computed divisor of 1, which will select
    // the maximum supported baud rate.
    //
    if (ComputedDivisor == 0) {
      ComputedDivisor = 1;
    }
    //
    // If the computed divisor is larger than the maximum value that can be programmed
    // into the UART, then the requested baud rate can not be supported.
    //
    if (ComputedDivisor > MAX_UINT16) {
      return;
    }
  }

  //
  //  High Speed mode
  //
  if (BaudRate > 115200) {
    Clock.Fields.N = 32767;
    ClockRate = LPSS_UART_SOURCE_CLOCK;
    //  M = ((N * 16) * BaudRate) / ClockRate;
    Clock.Fields.M = (UINT16) DivU64x64Remainder (MultU64x32 (BaudRate, Clock.Fields.N << (UINT32) 4), ClockRate, NULL);
  }

  //
  // Update Clock
  //
  MmioWrite32 (MmioBaseAddress + ClockOffset, Clock.Data);

  //
  // Disable Clock update
  //
  Clock.Data = MmioRead32 (MmioBaseAddress + ClockOffset);
  Clock.Fields.CLK_UPDATE = 0;
  MmioWrite32 (MmioBaseAddress + ClockOffset, Clock.Data);

  //
  // Open Divisor Latch
  //
  Lcr.Data = LpssUartReadRegister (MmioBaseAddress, R_LPSS_UART_MEM_LCR);
  Lcr.Fields.DLAB = 1;
  LpssUartWriteRegister (MmioBaseAddress, R_LPSS_UART_MEM_LCR, Lcr.Data);
  //
  // Update Baud Rate
  //
  LpssUartWriteRegister (MmioBaseAddress, R_LPSS_UART_MEM_DLH, (UINT8) (RShiftU64 (ComputedDivisor & 0xFF00, 8)));
  LpssUartWriteRegister (MmioBaseAddress, R_LPSS_UART_MEM_DLL, (UINT8) (ComputedDivisor & 0xFF));
  //
  // Close Divisor Latch
  //
  Lcr.Data = LpssUartReadRegister (MmioBaseAddress, R_LPSS_UART_MEM_LCR);
  Lcr.Fields.DLAB = 0;
  LpssUartWriteRegister (MmioBaseAddress, R_LPSS_UART_MEM_LCR, Lcr.Data);
}

/**
  Returns current BaudRate

  @param[in]  MmioBaseAddress     MMIO Base address
  @retval     BaudRate            Baud Rate
**/
UINT64
LpssUartGetBaudRate (
  IN UINTN                  MmioBaseAddress
  )
{
  UART_ACCESS_MODE          AccessMode;
  LPSS_UART_CLOCK           Clock;
  LPSS_UART_LCR             Lcr;
  UINT64                    Divisor;
  UINT32                    ClockRate;
  UINT16                    ClockOffset;
  UINT64                    Remainder = 0;

  Divisor     = 1;
  ClockRate   = 1843200;
  ClockOffset = R_LPSS_UART_MEM_CLOCKS;

  if (MmioRead32 (MmioBaseAddress + R_LPSS_UART_MEM_CTR) == UART_COMPONENT_IDENTIFICATION_CODE) {
    AccessMode = AccessMode32bit;
  } else {
    AccessMode = AccessMode8bit;
  }

  if (AccessMode == AccessMode8bit) {
    ClockOffset = ClockOffset / 4;
  }
  //
  // Open Divisor Latch
  //
  Lcr.Data = LpssUartReadRegister (MmioBaseAddress, R_LPSS_UART_MEM_LCR);
  Lcr.Fields.DLAB = 1;
  LpssUartWriteRegister (MmioBaseAddress, R_LPSS_UART_MEM_LCR, Lcr.Data);
  //
  // Read BAUD_HIGH and BUAD_LOW
  //
  Divisor = ((LpssUartReadRegister (MmioBaseAddress, R_LPSS_UART_MEM_DLH) << 8)
               | ((LpssUartReadRegister (MmioBaseAddress, R_LPSS_UART_MEM_DLL) & 0xFF)));
  //
  // Close Divisor Latch
  //
  Lcr.Data = LpssUartReadRegister (MmioBaseAddress, R_LPSS_UART_MEM_LCR);
  Lcr.Fields.DLAB = 0;
  LpssUartWriteRegister (MmioBaseAddress, R_LPSS_UART_MEM_LCR, Lcr.Data);

  if (Divisor == 0) {
    return 0;
  }

  if (Divisor > 1) {
    return (UINT32) DivU64x64Remainder (ClockRate, LShiftU64 (Divisor, 4), &Remainder);
  }

  Clock.Data = MmioRead32 (MmioBaseAddress + ClockOffset);
  ClockRate = LPSS_UART_SOURCE_CLOCK;

  // BaudRate = (SourceClock * M)/(N * 16);
  return (UINT32) DivU64x32 (MultU64x32 (ClockRate, Clock.Fields.M), Clock.Fields.N << 4);
}

/**
  Enables Fifo

  @param[in]  MmioBaseAddress     MMIO Base address
**/
VOID
LpssUartFifoEnable (
  IN UINTN                  MmioBaseAddress
  )
{
  LPSS_UART_FCR     Fcr;

  Fcr.Data = LpssUartReadRegister (MmioBaseAddress, R_LPSS_UART_MEM_FCR);
  Fcr.Fields.FIFOE = 1;
  LpssUartWriteRegister (MmioBaseAddress, R_LPSS_UART_MEM_FCR, Fcr.Data);
}

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
  )
{

  LpssUartFifoEnable  (MmioBaseAddress);
  LpssUartSetBaudRate (MmioBaseAddress, BaudRate);
  LpssUartSetParity   (MmioBaseAddress, Parity);
  LpssUartSetDataBits (MmioBaseAddress, DataBits);
  LpssUartSetStopBits (MmioBaseAddress, StopBits);
  LpssUartSetAutoFlow (MmioBaseAddress, AutoFlow);
}

/**
  Checks if Tx Fifo is not full

  @param[in]  AccessMode       8bit/32bit register layout
  @param[in]  MmioBaseAddress     MMIO Base address

  @retval TRUE                 Fifo is not full
          FALSE                Fifo is full
**/
BOOLEAN
STATIC
LpssUartIsTxFifoNotFull (
  IN UINTN             MmioBaseAddress
  )
{
  LPSS_UART_USR  Usr;

  Usr.Data = LpssUartReadRegister (MmioBaseAddress, R_LPSS_UART_MEM_USR);
  return (BOOLEAN) Usr.Fields.TFNF;
}

/**
  Writes data to the serial device only if Fifo is ready for it

  @param[in]  MmioBaseAddress  MMIO Base address
  @param[in]  AccessMode       8bit/32bit register layout
  @param[in]  Data             Data to transmit

  @retval TRUE                 Data was sent out
          FALSE                No data transmitted
**/
BOOLEAN
LpssUartTransmit (
  IN UINTN             MmioBaseAddress,
  IN UINT8             Data
  )
{
  if (LpssUartIsTxFifoNotFull (MmioBaseAddress)) {
    LpssUartWriteRegister (MmioBaseAddress, R_LPSS_UART_MEM_THR, Data);
    return TRUE;
  }
  return FALSE;
}

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
  )
{
  BOOLEAN           UseTimeout;
  UINT32            Index;
  UINTN             Timeout;

  if ((NULL == Buffer) || (LpssUartReadRegister (MmioBaseAddress, R_LPSS_UART_MEM_USR) == 0xFF)) {
     return 0;
  }

  UseTimeout = TRUE;

  //
  // If Timeout is equal to 0, then timeout is disabled
  //
  Timeout = PcdGet32 (PcdLpssUartTimeOut);
  if (Timeout == 0) {
     UseTimeout = FALSE;
  }
  for (Index = 0; Index < NumberOfBytes; Index++) {
    while (!LpssUartTransmit (MmioBaseAddress, Buffer[Index])) {
      if (UseTimeout == TRUE) {
        //
        // Unsuccessful write so check if timeout has expired, if not,
        // stall for a bit, increment time elapsed, and try again
        //
        if (Timeout == 0) {
          // If Auto Flow is already disabled, and timeout was reached return
          if (LpssUartGetAutoFlow (MmioBaseAddress) == FALSE) {
            return 0;
          }
          //
          // Disable AutoFlow Control if data did not come out of the FIFO in given time
          //
          LpssUartSetAutoFlow (MmioBaseAddress, FALSE);
          Timeout = PcdGet32 (PcdLpssUartTimeOut);
        }
        MicroSecondDelay (LPSS_UART_TIMEOUT_DELAY_INTERVAL);
        Timeout -= LPSS_UART_TIMEOUT_DELAY_INTERVAL;
      }
    }
  }
  return NumberOfBytes;
}

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
  )
{
  UINTN                BytesReceived;
  LPSS_UART_LSR        Lsr;
  UINT8                Byte;

  if ((NULL == Buffer) || (LpssUartReadRegister (MmioBaseAddress, R_LPSS_UART_MEM_USR) == 0xFF)) {
    return 0;
  }

  BytesReceived = 0;

  while (BytesReceived != NumberOfBytes) {
    Lsr.Data = LpssUartReadRegister (MmioBaseAddress, R_LPSS_UART_MEM_LSR);
    //
    // If there is data in the RX buffer, read it.
    //
    if (Lsr.Fields.DR != 0) {
      Byte = LpssUartReadRegister (MmioBaseAddress, R_LPSS_UART_MEM_RBR);
      //
      // Check if the break interrupt bit is set. If set, the byte read from the
      // RX buffer is invalid and should be ignored. If not set, copy the byte into
      // the receive buffer.
      //
      if (Lsr.Fields.BI == 0) {
        *Buffer = Byte;
        Buffer++;
        BytesReceived++;
      }
    } else {
      if (!WaitUntilBufferFull) {
        //
        // If there's no data and function shouldn't wait, exit early
        //
        return BytesReceived;
      }
    }
  }
  return BytesReceived;
}

/**
  Gets Fixed Base Address used for BAR0

  @param[in] UartNumber              LPSS device UART number

  @retval                            Config control offset
**/
UINT32
GetLpssUartFixedMmioAddress (
  IN UINT8       UartNumber
  )
{
  return GetLpssUartFixedBar0 (UartNumber);
}
