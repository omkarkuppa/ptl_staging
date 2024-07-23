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

#include <Library/SerialIoUartPrivateLib.h>
#include <Library/SerialIoUartLib.h>
#include <Library/SerialIoAccessLib.h>
#include <Library/SerialIoPrivateLib.h>
#include <Library/SerialIoUartSocLib.h>

/**
  Enables UART in Hidden mode

  @param[in]      PciCfgBase     PCI Config Base address
  @param[in]      *UartNumber    Optional: Selects Serial IO UART device.
                                 If not provided, PciCfgBase is used instead.
  @param[in/out]  BaseAddress    Base address of UART MMIO space

  @retval   TRUE             UART has BAR assigned
  @retval   FALSE            Unable to access UART's MMIO
**/
BOOLEAN
STATIC
SerialIoUartEnableHidden (
  IN     UINT64      PciCfgBase,
  IN     UINT8       *UartNumber,     OPTIONAL
  IN OUT UINT64      *BaseAddress
  )
{
  SERIAL_IO_PWRMG_CONTROL_AND_STATUS  UartPwrMgmControlStsReg;

  if (UartNumber != NULL) {
    PciCfgBase = GetSerialIoUartFixedPciCfgAddress (*UartNumber);
  }
  *BaseAddress = GetSerialIoBar (PciCfgBase);
  if ((*BaseAddress == 0xFFFFFFFFFFFFF000) || (*BaseAddress == 0x0)) { //Device is still not enabled, or there is no BAR assigned
    return FALSE;
  }

  UartPwrMgmControlStsReg.Data32 = MmioRead32 ((UINTN) PciCfgBase + R_SERIAL_IO_CFG_PMECTRLSTATUS);

  if (UartPwrMgmControlStsReg.Fields.PwrState != 0x0) { //Check PG only
    UartPwrMgmControlStsReg.Fields.PwrState = 0x0; // D0 State
    MmioAndThenOr8 ((UINTN) PciCfgBase + R_SERIAL_IO_CFG_PMECTRLSTATUS, (UINT8) UartPwrMgmControlStsReg.Data32, 0x0); // Place device in D0
    MmioRead8 ((UINTN) PciCfgBase + R_SERIAL_IO_CFG_PMECTRLSTATUS); // Dummy Read after modifying PME
    SerialIoUartGetOutOfReset (*BaseAddress);
  }
  return TRUE;
}

/**
  Enables UART in Pci mode

  @param[in]      PciCfgBase     PCI Config Base address
  @param[in]      *UartNumber    Optional: Selects Serial IO UART device.
                                 If not provided, PciCfgBase is used instead.
  @param[in/out]  BaseAddress    Base address of UART MMIO space

  @retval   TRUE             UART has BAR assigned
  @retval   FALSE            Unable to access UART's MMIO
**/
BOOLEAN
STATIC
SerialIoUartEnablePci (
  IN UINT64          PciCfgBase,
  IN UINT8           *UartNumber,     OPTIONAL
  IN OUT UINT64      *BaseAddress
  )
{
  SERIAL_IO_PWRMG_CONTROL_AND_STATUS  UartPwrMgmControlStsReg;

  if (UartNumber != NULL) {
    PciCfgBase = LpssUartPciCfgBase (*UartNumber);
  }
  *BaseAddress = GetSerialIoBar (PciCfgBase);
  if ((*BaseAddress == 0xFFFFFFFFFFFFF000) || (*BaseAddress == 0x0)) { //Device is still not enabled, or there is no BAR assigned
    return FALSE;
  }
  if ((PciSegmentRead8 (PciCfgBase + PCI_COMMAND_OFFSET) & EFI_PCI_COMMAND_MEMORY_SPACE) != EFI_PCI_COMMAND_MEMORY_SPACE) {
    PciSegmentOr8 (PciCfgBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE); // Enable MSE

    UartPwrMgmControlStsReg.Data32 = PciSegmentRead32 (PciCfgBase + R_SERIAL_IO_CFG_PMECTRLSTATUS);
    UartPwrMgmControlStsReg.Fields.PwrState = 0x0; // D0 State
    PciSegmentWrite32 (PciCfgBase + R_SERIAL_IO_CFG_PMECTRLSTATUS, UartPwrMgmControlStsReg.Data32);
    PciSegmentRead8 (PciCfgBase + R_SERIAL_IO_CFG_PMECTRLSTATUS); // Dummy Read after modifying PME

    SerialIoUartGetOutOfReset (*BaseAddress);
  }
  return TRUE;
}

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
  )
{
  UINT8  TempUartNumber;

  if (UartNumber != NULL && *UartNumber >= GetMaxUartInterfacesNum ()) {
    return FALSE;
  }

  if (PcdGetBool (PcdSerialIoUartLibSkipMmioCheck)) {
    if (UartNumber != NULL) {
      PciCfgBase = GetSerialIoUartFixedPciCfgAddress (*UartNumber);
    }
    *BaseAddress = (MmioRead32 ((UINTN) PciCfgBase + PCI_BASE_ADDRESSREG_OFFSET) & 0xFFFFF000) + LShiftU64 (MmioRead32 ((UINTN) PciCfgBase + PCI_BASE_ADDRESSREG_OFFSET + 4), 32);
  } else {
    if (UartNumber == NULL) {
      TempUartNumber = GetSerialIoUartNumber (PciCfgBase);
    } else {
      TempUartNumber = *UartNumber;
    }
    if (TempUartNumber < 0xFF && IsSerialIoUartHidden (TempUartNumber)) {
      if (!SerialIoUartEnableHidden (PciCfgBase, &TempUartNumber, BaseAddress)) {
        return FALSE;
      }
    } else {
      if (!SerialIoUartEnablePci (PciCfgBase, UartNumber, BaseAddress)) {
        return FALSE;
      }
    }
  }

  if (MmioRead32 ((UINTN) *BaseAddress + R_SERIAL_IO_UART_MEM_CTR) == UART_COMPONENT_IDENTIFICATION_CODE) {
    *AccessMode = AccessMode32bit;
  } else {
    *AccessMode = AccessMode8bit;
  }

  return TRUE;
}

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
  )
{
  if (AccessMode == AccessMode32bit) {
    MmioWrite32 ((UINTN) BaseAddress + Offset, Data);
  } else {
    MmioWrite8 ((UINTN) BaseAddress + Offset/4, Data);
  }
}

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
  )
{
  if (AccessMode == AccessMode32bit) {
    return (UINT8) (0xFF & MmioRead32 ((UINTN) BaseAddress + Offset));
  }
  return MmioRead8 ((UINTN) BaseAddress + Offset/4);
}

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
  )
{
  SERIAL_IO_UART_USR  Usr;
  Usr.Data = SerialIoUartReadRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_USR);
  return (BOOLEAN) Usr.Fields.TFNF;
}

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
  )
{
  if (SerialIoUartIsTxFifoNotFull (AccessMode, BaseAddress)) {
    SerialIoUartWriteRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_THR, Data);
    return TRUE;
  }
  return FALSE;
}

/**
  Write data to serial device.

  If the buffer is NULL, then return 0;
  if NumberOfBytes is zero, then return 0.

  @param[in]  BaseAddress      Base Address Regiser offset (BAR0)
  @param[in]  AccessMode       8bit/32bit register layout
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
  )
{
  BOOLEAN           UseTimeout;
  UINT32            Index;
  UINTN             Timeout;

  if ((NULL == Buffer) || (SerialIoUartReadRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_USR) == 0xFF)) {
    return 0;
  }

  UseTimeout = TRUE;

  //
  // If Timeout is equal to 0, then timeout is disabled
  //
  Timeout = PcdGet32 (PcdSerialIoUartTimeOut);
  if (Timeout == 0) {
    UseTimeout = FALSE;
  }
  
  for (Index = 0; Index < NumberOfBytes; Index++) {
    while (!SerialIoUartTransmit (AccessMode, BaseAddress, Buffer[Index])) {
      if (UseTimeout == TRUE) {
        //
        // Unsuccessful write so check if timeout has expired, if not,
        // stall for a bit, increment time elapsed, and try again
        //
        if (Timeout == 0) {
          // If Auto Flow is already disabled, and timeout was reached return
          if (SerialIoUartReadAutoFlow (BaseAddress, AccessMode) == FALSE) {
            return 0;
          }
          //
          // Disable AutoFlow Control if data did not come out of the FIFO in given time
          //
          SerialIoUartProgramAutoFlow (BaseAddress, AccessMode, FALSE);
          Timeout = PcdGet32 (PcdSerialIoUartTimeOut);
        }
        MicroSecondDelay (SERIALIO_UART_TIMEOUT_DELAY_INTERVAL);
        Timeout -= SERIALIO_UART_TIMEOUT_DELAY_INTERVAL;
      }
    }
  }

  return NumberOfBytes;
}

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
  )
{
  SERIAL_IO_UART_CLOCK  Clock;
  SERIAL_IO_UART_LCR    Lcr;
  UINT64                ComputedDivisor;
  UINT32                ClockRate;
  UINT16                ClockOffset;

  ClockOffset = R_SERIAL_IO_UART_MEM_CLOCKS;

  if (AccessMode == AccessMode8bit) {
    ClockOffset = ClockOffset / 4;
  }

  //
  // Default 115200 settings
  //
  ComputedDivisor = 1;
  ClockRate       = 1843200;

  //
  // Clock defaults
  //
  Clock.Data              = MmioRead32 ((UINTN) BaseAddress + ClockOffset);
  Clock.Fields.N          = V_SERIAL_IO_MEM_PPR_CLK_N_DIV;
  Clock.Fields.M          = V_SERIAL_IO_MEM_PPR_CLK_M_DIV;
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
    ClockRate = SOURCE_CLOCK;
    //  M = ((N * 16) * BaudRate) / ClockRate;
    Clock.Fields.M = (UINT16) DivU64x64Remainder (MultU64x32 (BaudRate, Clock.Fields.N << (UINT32) 4), ClockRate, NULL);
  }

  //
  // Update Clock
  //
  MmioWrite32 ((UINTN) BaseAddress + ClockOffset, Clock.Data);

  //
  // Disable Clock update
  //
  Clock.Data = MmioRead32 ((UINTN) BaseAddress + ClockOffset);
  Clock.Fields.CLK_UPDATE = 0;
  MmioWrite32 ((UINTN) BaseAddress + ClockOffset, Clock.Data);

  //
  // Open Divisor Latch
  //
  Lcr.Data = SerialIoUartReadRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_LCR);
  Lcr.Fields.DLAB = 1;
  SerialIoUartWriteRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_LCR, Lcr.Data);
  //
  // Update Baud Rate
  //
  SerialIoUartWriteRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_DLH, (UINT8)(RShiftU64 (ComputedDivisor & 0xFF00, 8)));
  SerialIoUartWriteRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_DLL, (UINT8)(ComputedDivisor & 0xFF));
  //
  // Close Divisor Latch
  //
  Lcr.Data = SerialIoUartReadRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_LCR);
  Lcr.Fields.DLAB = 0;
  SerialIoUartWriteRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_LCR, Lcr.Data);
}

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
  )
{
  UART_ACCESS_MODE      AccessMode;
  UINT64                BaseAddress;

  if (!SerialIoUartEnable (PciCfgBase, UartNumber, &BaseAddress, &AccessMode)) {
    return;
  }

  //
  //  Set default Baudrate based on Pcd
  //
  if (BaudRate == 0) {
    BaudRate = PcdGet64 (PcdUartDefaultBaudRate);
  }

  if (BaudRate > MAX_BAUDRATE) {
    return;
  }

  SerialIoUartProgramBaudRate (BaseAddress, AccessMode, BaudRate);
}

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
  )
{
  UART_ACCESS_MODE      AccessMode;
  UINT64                BaseAddress;
  SERIAL_IO_UART_CLOCK  Clock;
  SERIAL_IO_UART_LCR    Lcr;
  UINT64                Divisor;
  UINT32                ClockRate;
  UINT16                ClockOffset;
  UINT64                Remainder = 0;

  Divisor     = 1;
  ClockRate   = 1843200;
  ClockOffset = R_SERIAL_IO_UART_MEM_CLOCKS;;

  if (!SerialIoUartEnable (PciCfgBase, UartNumber, &BaseAddress, &AccessMode)) {
    return 0;
  }

  if (AccessMode == AccessMode8bit) {
    ClockOffset = ClockOffset / 4;
  }
  //
  // Open Divisor Latch
  //
  Lcr.Data = SerialIoUartReadRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_LCR);
  Lcr.Fields.DLAB = 1;
  SerialIoUartWriteRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_LCR, Lcr.Data);
  //
  // Read BAUD_HIGH and BUAD_LOW
  //
  Divisor = ((SerialIoUartReadRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_DLH) << 8)
               | ((SerialIoUartReadRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_DLL) & 0xFF)));
  //
  // Close Divisor Latch
  //
  Lcr.Data = SerialIoUartReadRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_LCR);
  Lcr.Fields.DLAB = 0;
  SerialIoUartWriteRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_LCR, Lcr.Data);

  if (Divisor == 0) {
    return 0;
  }

  if (Divisor > 1) {
    return DivU64x64Remainder (ClockRate, LShiftU64 (Divisor, 4), &Remainder);
  }

  Clock.Data = MmioRead32 ((UINTN) BaseAddress + ClockOffset);
  ClockRate = SOURCE_CLOCK;

  // BaudRate = (SourceClock * M)/(N * 16);
  return DivU64x32 (MultU64x32 (ClockRate, Clock.Fields.M), Clock.Fields.N << 4);
}

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
  )
{
  SERIAL_IO_UART_LCR  Lcr;

  Lcr.Data = SerialIoUartReadRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_LCR);
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
  SerialIoUartWriteRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_LCR, Lcr.Data);
}

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
  )
{
  UART_ACCESS_MODE    AccessMode;
  UINT64              BaseAddress;

  if (!SerialIoUartEnable (PciCfgBase, UartNumber, &BaseAddress, &AccessMode)) {
    return;
  }

  if (StopBits > TwoStopBits) {
    return;
  }
  if (StopBits == DefaultStopBits) {
    StopBits = (EFI_STOP_BITS_TYPE) PcdGet8 (PcdUartDefaultStopBits);
  }

  SerialIoUartProgramStopBits (BaseAddress, AccessMode, StopBits);
}

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
  )
{
  UART_ACCESS_MODE    AccessMode;
  UINT64              BaseAddress;
  SERIAL_IO_UART_LCR  Lcr;

  if (!SerialIoUartEnable (PciCfgBase, UartNumber, &BaseAddress, &AccessMode)) {
    return DefaultStopBits;
  }

  Lcr.Data = SerialIoUartReadRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_LCR);

  if (Lcr.Fields.STOP == 1) {
    if (Lcr.Fields.DLS == 0) { // DLS = 5 bits
      return OneFiveStopBits;
    }
    return TwoStopBits;
  }

  return OneStopBit;
}

/**
  Program Data Bits

  @param[in]  BaseAddress      Base Address Regiser offset (BAR0)
  @param[in]  AccessMode       8bit/32bit register layout
  @param[in]  DataBits         Data Bits 5/6/7/8

**/
VOID
SerialIoUartProgramDataBits (
  IN UINT64              BaseAddress,
  IN UART_ACCESS_MODE    AccessMode,
  IN UINT8               DataBits
  )
{
  SERIAL_IO_UART_LCR  Lcr;

  Lcr.Data = SerialIoUartReadRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_LCR);

  if ((DataBits < 5) || (DataBits > 8)) {
    return;
  }
  // Map 5..8 to 0..3
  Lcr.Fields.DLS = (UINT8) (DataBits - (UINT8) 5);
  SerialIoUartWriteRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_LCR, Lcr.Data);
}

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
  )
{
  UART_ACCESS_MODE    AccessMode;
  UINT64              BaseAddress;

  if (!SerialIoUartEnable (PciCfgBase, UartNumber, &BaseAddress, &AccessMode)) {
    return;
  }

  if (DataBits == 0) {
    DataBits = PcdGet8 (PcdUartDefaultDataBits);
  }

  SerialIoUartProgramDataBits (BaseAddress, AccessMode, DataBits);
}

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
  )
{
  UART_ACCESS_MODE    AccessMode;
  UINT64              BaseAddress;
  SERIAL_IO_UART_LCR  Lcr;

  if (!SerialIoUartEnable (PciCfgBase, UartNumber, &BaseAddress, &AccessMode)) {
    return 0;
  }
  Lcr.Data = SerialIoUartReadRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_LCR);

  return (Lcr.Fields.DLS + (UINT8) 5);
}

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
  )
{
  SERIAL_IO_UART_LCR  Lcr;

  Lcr.Data = SerialIoUartReadRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_LCR);

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

  SerialIoUartWriteRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_LCR, Lcr.Data);
}

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
  )
{
  UART_ACCESS_MODE    AccessMode;
  UINT64              BaseAddress;

  if (!SerialIoUartEnable (PciCfgBase, UartNumber, &BaseAddress, &AccessMode)) {
    return;
  }

  if (Parity == DefaultParity) {
    Parity = (EFI_PARITY_TYPE) PcdGet8 (PcdUartDefaultParity);
  }
  if (Parity > OddParity) { // Not supported
    return;
  }

  SerialIoUartProgramParity (BaseAddress, AccessMode, Parity);
}

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
  )
{
  UART_ACCESS_MODE    AccessMode;
  UINT64              BaseAddress;
  SERIAL_IO_UART_LCR  Lcr;

  if (!SerialIoUartEnable (PciCfgBase, UartNumber, &BaseAddress, &AccessMode)) {
    return DefaultParity;
  }

  Lcr.Data = SerialIoUartReadRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_LCR);

  if ((Lcr.Fields.PEN == 1) && (Lcr.Fields.EPS == 1)) {
    return OddParity;
  } else if ((Lcr.Fields.PEN == 1) && (Lcr.Fields.EPS == 0)) {
    return EvenParity;
  }
  return NoParity; // PEN =0/EPS = 0
}

/**
  Program FIFO Enabled

  @param[in]  BaseAddress      Base Address Regiser offset (BAR0)
  @param[in]  AccessMode       8bit/32bit register layout
**/
EFI_STATUS
SerialIoUartProgramFifoEnable (
  IN UINT64               BaseAddress,
  IN UART_ACCESS_MODE     AccessMode
)
{
  SERIAL_IO_UART_FCR   Fcr;

  // FIFO enabled
  Fcr.Data = SerialIoUartReadRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_FCR);
  Fcr.Fields.FIFOE = 1;
  SerialIoUartWriteRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_FCR, Fcr.Data);

  return EFI_SUCCESS;
}

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
  )
{
  UART_ACCESS_MODE     AccessMode;
  UINT64               BaseAddress;

  if (!SerialIoUartEnable (PciCfgBase, UartNumber, &BaseAddress, &AccessMode)) {
    return;
  }

  SerialIoUartProgramFifoEnable (BaseAddress, AccessMode);
}

/**
  Returns FIFOE state

  @param[in]  UartNumber       Selects Serial IO UART device

  @retval     TRUE             Fifo Enabled
  @retval     FALSE            Fifo Disabled
**/
BOOLEAN
SerialIoUartIsFifoEnabled (
  IN UINT64  PciCfgBase,
  IN UINT8   *UartNumber     OPTIONAL
  )
{
  UART_ACCESS_MODE    AccessMode;
  UINT64              BaseAddress;
  SERIAL_IO_UART_FCR  Fcr;

  if (!SerialIoUartEnable (PciCfgBase, UartNumber, &BaseAddress, &AccessMode)) {
    return FALSE;
  }

  Fcr.Data = SerialIoUartReadRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_FCR);
  return (BOOLEAN) Fcr.Fields.FIFOE;
}

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
  )
{
  SERIAL_IO_UART_MCR  Mcr;

  Mcr.Data = SerialIoUartReadRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_MCR);
  Mcr.Fields.RTS  = 1;
  Mcr.Fields.AFCE = (AutoFlow ? 1 : 0);
  SerialIoUartWriteRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_MCR, Mcr.Data);
}

/**
  Enables/Disables Auto Flow Control

  @param[in]  UartNumber       Selects Serial IO UART device
  @param[in]  AutoFlow         TRUE - Enable, FALSE - Disable

**/
VOID
SerialIoUartSetAutoFlow (
  IN UINT64              PciCfgBase,
  IN UINT8               *UartNumber,     OPTIONAL
  IN BOOLEAN             AutoFlow
  )
{
  UART_ACCESS_MODE    AccessMode;
  UINT64              BaseAddress;

  if (!SerialIoUartEnable (PciCfgBase, UartNumber, &BaseAddress, &AccessMode)) {
    return;
  }

  SerialIoUartProgramAutoFlow (BaseAddress, AccessMode, AutoFlow);
}

/**
  Returns Auto Flow Control state

  @param[in]  UartNumber       Selects Serial IO UART device

  @retval     TRUE             AutoFlow Enabled
  @retval     FALSE            AutoFlow Disabled
**/
BOOLEAN
SerialIoUartReadAutoFlow (
  UINT64              BaseAddress,
  UART_ACCESS_MODE    AccessMode
  )
{
  SERIAL_IO_UART_MCR  Mcr;

  Mcr.Data = SerialIoUartReadRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_MCR);
  return (BOOLEAN) Mcr.Fields.AFCE;
}

/**
  Returns Auto Flow Control state

  @param[in]  UartNumber       Selects Serial IO UART device

  @retval     TRUE             AutoFlow Enabled
  @retval     FALSE            AutoFlow Disabled
**/
BOOLEAN
SerialIoUartGetAutoFlow (
  IN UINT64  PciCfgBase,
  IN UINT8   *UartNumber     OPTIONAL
  )
{
  UART_ACCESS_MODE    AccessMode;
  UINT64              BaseAddress;

  if (!SerialIoUartEnable (PciCfgBase, UartNumber, &BaseAddress, &AccessMode)) {
    return FALSE;
  }

  return (BOOLEAN) SerialIoUartReadAutoFlow (BaseAddress, AccessMode);
}