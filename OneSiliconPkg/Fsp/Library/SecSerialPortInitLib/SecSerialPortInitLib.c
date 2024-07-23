/** @file
  SEC Serial Port initialization library functions with no library constructor/destructor

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#include "SecSerialPortInitLib.h"
#include <FspEas.h>
#include <FspUpd.h>
#include <FsptUpd.h>
#include <Library/GetFsptApiParameter.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PrintLib.h>
#include <Register/TcoRegs.h>

#define MAX_BAUD_RATE     115200

//
// UART register offsets and bitfields
//
#define RXBUF_OFFSET      0x00
#define TXBUF_OFFSET      0x00
#define BAUD_LOW_OFFSET   0x00
#define BAUD_HIGH_OFFSET  0x01
#define IER_OFFSET        0x01
#define LCR_SHADOW_OFFSET 0x01
#define FCR_SHADOW_OFFSET 0x02
#define IR_CONTROL_OFFSET 0x02
#define FCR_OFFSET        0x02
#define EIR_OFFSET        0x02
#define IIR_OFFSET        0x02
#define BSR_OFFSET        0x03
#define LCR_OFFSET        0x03
#define MCR_OFFSET        0x04
#define LSR_OFFSET        0x05
#define MSR_OFFSET        0x06

//
// ---------------------------------------------
// UART Register Bit Defines
// ---------------------------------------------
//
#define FCR_FIFOE    BIT0
#define FCR_FIFO64   BIT5
#define LSR_TXRDY    0x20
#define LSR_RXDA     0x01
#define DLAB         BIT7
#define MCR_DTR      BIT0
#define MCR_RTS      BIT1
#define MCR_OUT1     BIT2
#define MCR_OUT2     BIT3
#define MCR_LOOPBACK BIT4
#define MSR_CTS      BIT4
#define MSR_DSR      BIT5
#define IER_ERDAI    BIT0
#define IER_ETHREI   BIT1
#define IER_ELSI     BIT2
#define IER_EMSI     BIT3
#define IIR_IPEND    BIT0
#define IIR_INTID    (BIT1 | BIT2 | BIT3)
#define IIR_FIFO_EN  (BIT6 | BIT7)

/**
  Read one byte from a Serial Port Register.

  @param[in] Offset             The offset to read.

  @retval                       The value read.

**/
UINT8
SecSerialPortReadRegister (
  UINTN  Offset
  )
{
  return IoRead8 ((UINTN) PcdGet64 (PcdFspSerialRegisterBase) + Offset);
}

/**
  Write one byte to a Serial Port Register.

  @param[in] Offset             The offset to write.
  @param[in] Value              The value to write.

  @retval                       The value written.

**/
UINT8
SecSerialPortWriteRegister (
  UINTN  Offset,
  UINT8  Value
  )
{
  return IoWrite8 ((UINTN) PcdGet64 (PcdFspSerialRegisterBase) + Offset, Value);
}

/**
  Write data to serial device.

  If the buffer is NULL, then return 0;
  if NumberOfBytes is zero, then return 0.

  @param  Buffer           Point of data buffer which need to be writed.
  @param  NumberOfBytes    Number of output bytes which are cached in Buffer.

  @retval 0                Write data failed.
  @retval >0               Actual number of bytes writed to serial device.

**/
UINTN
EFIAPI
SecUartDebugOut (
  IN UINT8     *Buffer,
  IN UINTN     NumberOfBytes
  )
{
  UINTN Result;
  UINTN FifoSize;
  UINTN Index;

  if (NULL == Buffer) {
    return 0;
  }

  //
  // Compute the maximum size of the Tx FIFO
  //
  FifoSize = 1;
  if ((PcdGet8 (PcdFspSerialFifoControl) & FCR_FIFOE) != 0) {
    if ((PcdGet8 (PcdFspSerialFifoControl) & FCR_FIFO64) == 0) {
      FifoSize = 16;
    } else {
      FifoSize = 64;
    }
  }

  Result = NumberOfBytes;

  while (NumberOfBytes != 0) {
    //
    // Wait for the serial port to be ready, to make sure both the transmit FIFO
    // and shift register empty.
    //
    while ((SecSerialPortReadRegister (LSR_OFFSET) & LSR_TXRDY) == 0);

    //
    // Fill then entire Tx FIFO
    //
    for (Index = 0; Index < FifoSize && NumberOfBytes != 0; Index++, NumberOfBytes--, Buffer++) {
      if (PcdGetBool (PcdFspSerialUseHardwareFlowControl)) {
        if (PcdGetBool (PcdFspSerialDetectCable)) {
          //
          // Wait for both DSR and CTS to be set
          //   DSR is set if a cable is connected.
          //   CTS is set if it is ok to transmit data
          //
          //   DSR  CTS  Description                               Action
          //   ===  ===  ========================================  ========
          //    0    0   No cable connected.                       Wait
          //    0    1   No cable connected.                       Wait
          //    1    0   Cable connected, but not clear to send.   Wait
          //    1    1   Cable connected, and clear to send.       Transmit
          //
          while ((SecSerialPortReadRegister (MSR_OFFSET) & (MSR_DSR | MSR_CTS)) != (MSR_DSR | MSR_CTS));
        } else {
          //
          // Wait for both DSR and CTS to be set OR for DSR to be clear.
          //   DSR is set if a cable is connected.
          //   CTS is set if it is ok to transmit data
          //
          //   DSR  CTS  Description                               Action
          //   ===  ===  ========================================  ========
          //    0    0   No cable connected.                       Transmit
          //    0    1   No cable connected.                       Transmit
          //    1    0   Cable connected, but not clear to send.   Wait
          //    1    1   Cable connected, and clar to send.        Transmit
          //
          while ((SecSerialPortReadRegister (MSR_OFFSET) & (MSR_DSR | MSR_CTS)) == (MSR_DSR));
        }
      }

      //
      // Write byte to the transmit buffer.
      //
      SecSerialPortWriteRegister (TXBUF_OFFSET, *Buffer);
    }
  }

  return Result;
}

/**
  Initialize Serial Port.

  The Baud Rate Divisor registers are programmed and the LCR
  is used to configure the communications format. Hard coded
  UART config comes from globals in DebugSerialPlatform lib.

  @retval RETURN_SUCCESS           The serial device was initialized.
  @retval RETURN_ALREADY_STARTED   The serial device was initialized at before.

**/
RETURN_STATUS
EFIAPI
SecUartInitialize (
  VOID
  )
{
  UINTN          Divisor;
  UINTN          TempDivisor;
  UINT8          Ier;
  UINT8          Iir;
  UINT8          Lcr;
  UINT8          Mcr;
  UINT8          Lsr;
  CHAR8          UartInitBuf[64];

  //
  // Calculate divisor for baud generator
  //
  Divisor = MAX_BAUD_RATE / PcdGet32 (PcdFspSerialBaudRate);

  //
  // Check whether the serial port was already initialized
  //
  SecSerialPortWriteRegister (LCR_OFFSET, (UINT8)(SecSerialPortReadRegister (LCR_OFFSET) | DLAB));
  TempDivisor = (UINTN)SecSerialPortReadRegister (BAUD_HIGH_OFFSET);
  TempDivisor = TempDivisor << 8;
  TempDivisor |= (UINTN)SecSerialPortReadRegister (BAUD_LOW_OFFSET);
  SecSerialPortWriteRegister (LCR_OFFSET, (UINT8)(SecSerialPortReadRegister (LCR_OFFSET) & ~DLAB));

  Ier = SecSerialPortReadRegister (IER_OFFSET);
  Iir = SecSerialPortReadRegister (IIR_OFFSET);
  Lcr = SecSerialPortReadRegister (LCR_OFFSET);
  Mcr = SecSerialPortReadRegister (MCR_OFFSET);
  Lsr = SecSerialPortReadRegister (LSR_OFFSET);

  if ((Ier & (IER_ERDAI | IER_ETHREI | IER_ELSI | IER_EMSI)) == 0 &&
      (Iir & (IIR_IPEND | IIR_INTID | IIR_FIFO_EN)) == (IIR_IPEND | IIR_FIFO_EN) &&
      (Lcr & 0x3F) == (PcdGet8 (PcdFspSerialLineControl) & 0x3F) &&
      (Mcr & (MCR_OUT1 | MCR_OUT2 | MCR_LOOPBACK)) == 0 &&
      TempDivisor == Divisor) {
    AsciiSPrint (UartInitBuf, sizeof (UartInitBuf), "Serial reg: IER:%02x IIR:%02x LCR:%02x MCR:%02x LSR:%02x\n", Ier, Iir, Lcr, Mcr, Lsr);
    SecUartDebugOut ((UINT8 *)UartInitBuf, AsciiStrLen(UartInitBuf));
    AsciiSPrint (UartInitBuf, sizeof (UartInitBuf), "Serial initialization is skipped\n");
    SecUartDebugOut ((UINT8 *)UartInitBuf, AsciiStrLen(UartInitBuf));
    return RETURN_ALREADY_STARTED;
  }

  // Disable the interrupt
  SecSerialPortWriteRegister (IER_OFFSET, 0x00);

  //
  // Set communications format
  //
  SecSerialPortWriteRegister (LCR_OFFSET, DLAB);

  //
  // Configure baud rate
  //
  SecSerialPortWriteRegister (BAUD_HIGH_OFFSET, (UINT8) (Divisor >> 8));
  SecSerialPortWriteRegister (BAUD_LOW_OFFSET, (UINT8) (Divisor & 0xff));

  //
  // Switch back to bank 0
  //
  SecSerialPortWriteRegister (LCR_OFFSET, (UINT8)(PcdGet8 (PcdFspSerialLineControl) & 0x3F));

  //
  // Disable FIFOs
  //
  SecSerialPortWriteRegister (FCR_OFFSET, 0x00);
  //
  // Put Modem Control Register(MCR) into its reset state of 0x00.
  //
  SecSerialPortWriteRegister (MCR_OFFSET, 0x00);

  //
  // Enable and reset FIFOs
  // Strip reserved bits from PcdSerialFifoControl
  //
  SecSerialPortWriteRegister (FCR_OFFSET, (UINT8)(PcdGet8 (PcdFspSerialFifoControl) & 0x27));

  AsciiSPrint (UartInitBuf, sizeof (UartInitBuf), "Serial reg: IER:%02x IIR:%02x LCR:%02x MCR:%02x LSR:%02x\n", Ier, Iir, Lcr, Mcr, Lsr);
  SecUartDebugOut ((UINT8 *)UartInitBuf, AsciiStrLen(UartInitBuf));
  AsciiSPrint (UartInitBuf, sizeof (UartInitBuf), "Serial initialization is successful\n");
  SecUartDebugOut ((UINT8 *)UartInitBuf, AsciiStrLen(UartInitBuf));

  return RETURN_SUCCESS;
}

/**
  Initialize Serial Port configuration, and print message once debug enabled.
**/
VOID
EFIAPI
SerialPortConfiguration (
  VOID
  )
{
  CHAR8             CarInitBuffer[32];
  UINT8             LpcUartDebugEnable;
  FSPT_UPD          *FsptUpd;

  LpcUartDebugEnable = 0;
  FsptUpd = (FSPT_UPD *)((UINTN) (SecGetFsptApiParameter ()));
  if (FsptUpd != NULL) {
    LpcUartDebugEnable = FsptUpd->FsptConfig.PcdLpcUartDebugEnable;
  }

  if (LpcUartDebugEnable == 1) {
    //
    // Halt the TCO timer as early as possible
    //
    IoWrite16 (PcdGet16 (PcdTcoBaseAddress) + R_TCO_IO_TCTL1, B_TCO_IO_TCTL1_TCO_TMR_HALT);
    //
    // Enable I/O decoding for COM1(3F8h-3FFh), COM2(2F8h-2FFh), I/O port 2Eh/2Fh, 4Eh/4Fh, 60h/64Fh and 62h/66h.
    //
    PchLpcIoDecodeRangesSet (PcdGet16 (PcdLpcIoDecodeRange));
    PchLpcIoEnableDecodingAndThenOr (0xFFFF, PcdGet16 (PcdLpcIoEnableDecoding));

    // This line add a little delay between enable encoding and init the serial port.
    // It is needed to wait for the UART registers be ready.
    AsciiSPrint (CarInitBuffer, sizeof (CarInitBuffer), "FSP-T: CAR Init\n");
    //
    // Initialize LPC UART for debug message
    //
    SecUartInitialize ();
    SecUartDebugOut ((UINT8 *)CarInitBuffer, AsciiStrLen(CarInitBuffer));
  }
}
