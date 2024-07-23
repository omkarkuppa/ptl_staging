/** @file
  Serial IO UART Lib implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

STATIC EFI_EVENT      mSerialIoUartLibExitBootServicesEvent;
STATIC BOOLEAN        mSerialIoUartLibAtRuntime = FALSE;

/**
  Set mSerialIoUartLibAtRuntime flag as TRUE after ExitBootServices.

  @param[in]  Event   The Event that is being processed.
  @param[in]  Context The Event Context.

**/
STATIC
VOID
EFIAPI
SerialIoUartLibExitBootServicesEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  mSerialIoUartLibAtRuntime = TRUE;
}

/**
  The constructor function registers a callback for the ExitBootServices event.

  @param[in]  ImageHandle   The firmware allocated handle for the EFI image.
  @param[in]  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The operation completed successfully.
  @retval other         Either the serial port failed to initialize or the
                        ExitBootServices event callback registration failed.
**/
EFI_STATUS
EFIAPI
DxeRuntimeSerialIoUartLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return SystemTable->BootServices->CreateEvent (EVT_SIGNAL_EXIT_BOOT_SERVICES,
                                      TPL_NOTIFY, SerialIoUartLibExitBootServicesEvent, NULL,
                                      &mSerialIoUartLibExitBootServicesEvent);
}

/**
  If a runtime driver exits with an error, it must call this routine
  to free the allocated resource before the exiting.

  @param[in]  ImageHandle   The firmware allocated handle for the EFI image.
  @param[in]  SystemTable   A pointer to the EFI System Table.

  @retval     EFI_SUCCESS       The Runtime Driver Lib shutdown successfully.
  @retval     EFI_UNSUPPORTED   Runtime Driver lib was not initialized.
**/
EFI_STATUS
EFIAPI
DxeRuntimeSerialIoUartLibDestructor  (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return SystemTable->BootServices->CloseEvent (mSerialIoUartLibExitBootServicesEvent);
}

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
  )
{
  UART_ACCESS_MODE     AccessMode;
  UINT64               BaseAddress;
  SERIAL_IO_UART_LSR   Lsr;

  if (UartNumber != NULL && *UartNumber >= GetMaxUartInterfacesNum ()) {
    return FALSE;
  }

  if (mSerialIoUartLibAtRuntime) {
    return FALSE;
  }

  if (!SerialIoUartEnable (PciCfgBase, UartNumber, &BaseAddress, &AccessMode)) {
    return FALSE;
  }

  //
  // Read the serial port status
  //
  Lsr.Data = SerialIoUartReadRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_LSR);
  return (BOOLEAN) Lsr.Fields.DR;
}

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
  IN UINTN            PciCfgBase,
  IN UINT8            *UartNumber,     OPTIONAL
  IN UINT8            *Buffer,
  IN UINTN            NumberOfBytes
  )
{
  BOOLEAN           UseTimeout;
  UINT32            Index;
  UINTN             Timeout;
  UART_ACCESS_MODE  AccessMode;
  UINT64            BaseAddress;

  if (UartNumber != NULL && *UartNumber >= GetMaxUartInterfacesNum ()) {
    return 0;
  }

  if (mSerialIoUartLibAtRuntime) {
    return 0;
  }

  if (!SerialIoUartEnable (PciCfgBase, UartNumber, &BaseAddress, &AccessMode)) {
    return 0;
  }
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
          if (SerialIoUartGetAutoFlow (PciCfgBase, UartNumber) == FALSE) {
            return 0;
          }
          //
          // Disable AutoFlow Control if data did not come out of the FIFO in given time
          //
          SerialIoUartSetAutoFlow (PciCfgBase, UartNumber, FALSE);
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
  )
{
  UINTN                BytesReceived;
  UINT64               BaseAddress;
  UART_ACCESS_MODE     AccessMode;
  SERIAL_IO_UART_LSR   Lsr;
  UINT8                Byte;

  if (UartNumber != NULL && *UartNumber >= GetMaxUartInterfacesNum ()) {
    return 0;
  }

  if (mSerialIoUartLibAtRuntime) {
    return 0;
  }

  if (!SerialIoUartEnable (PciCfgBase, UartNumber, &BaseAddress, &AccessMode)) {
    return 0;
  }
  if ((NULL == Buffer) || (SerialIoUartReadRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_USR) == 0xFF)) {
    return 0;
  }

  BytesReceived = 0;

  while (BytesReceived != NumberOfBytes) {
    Lsr.Data = SerialIoUartReadRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_LSR);
    //
    // If there is data in the RX buffer, read it.
    //
    if (Lsr.Fields.DR != 0) {
      Byte = SerialIoUartReadRegister (AccessMode, BaseAddress, R_SERIAL_IO_UART_MEM_RBR);
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
  )
{

  if (UartNumber != NULL && *UartNumber >= GetMaxUartInterfacesNum ()) {
    return;
  }

  if (mSerialIoUartLibAtRuntime) {
    return;
  }

  SerialIoUartFifoEnable  (PciCfgBase, UartNumber);
  SerialIoUartSetBaudRate (PciCfgBase, UartNumber, BaudRate);
  SerialIoUartSetParity   (PciCfgBase, UartNumber, Parity);
  SerialIoUartSetDataBits (PciCfgBase, UartNumber, DataBits);
  SerialIoUartSetStopBits (PciCfgBase, UartNumber, StopBits);
  SerialIoUartSetAutoFlow (PciCfgBase, UartNumber, AutoFlow);
}

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
  )
{
  UINT64  BaseAddress;

  if (UartNumber != NULL && *UartNumber >= GetMaxUartInterfacesNum ()) {
    return;
  }

  if (mSerialIoUartLibAtRuntime) {
    return;
  }
  if (UartNumber != NULL) {
    PciCfgBase  = (UINTN) GetSerialIoUartPciCfg (*UartNumber);
  }
  BaseAddress = GetSerialIoBar (PciCfgBase);

  MmioRead8 ((UINTN) BaseAddress + R_SERIAL_IO_UART_MEM_CTR);
}

/**
  Gets UART out of reset

  @param[in]  BaseAddress    Base address of UART MMIO space
**/
VOID
SerialIoUartGetOutOfReset (
  IN UINT64            MmioBaseAddress
  )
{
  UINT16                         ResetOffset;
  SERIAL_IO_HOST_CONTROL_RESET   UartHostControllerResetReg;

  UartHostControllerResetReg.Data32                   = 0x0;
  UartHostControllerResetReg.Fields.HostControlReset  = 0x3;  // Reset Released
  UartHostControllerResetReg.Fields.DmaReset          = 0x1;  // Reset Released

  if (mSerialIoUartLibAtRuntime) {
    return;
  }

  ResetOffset = R_SERIAL_IO_MEM_RESETS;

  if (MmioRead32 ((UINTN) (MmioBaseAddress + R_SERIAL_IO_UART_MEM_CTR)) != UART_COMPONENT_IDENTIFICATION_CODE) {
    ResetOffset = R_SERIAL_IO_UART_MEM_RESETS/4;  // Requires 8BIT Offset.
  }
  MmioOr8 ((UINTN) (MmioBaseAddress + ResetOffset), (UINT8) UartHostControllerResetReg.Data32);
}

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
  IN UINT64                PciCfgBase,
  IN UINT8                 *UartNumber     OPTIONAL
  )
{
  if (UartNumber != NULL && *UartNumber >= GetMaxUartInterfacesNum ()) {
    return FALSE;
  }

  if (UartNumber != NULL) {
    PciCfgBase = LpssUartPciCfgBase (*UartNumber);
  }

  if (PciSegmentRead16 (PciCfgBase) != MAX_UINT16) {
    return TRUE;
  } else {
    return FALSE;
  }
}

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
  )
{
  if (UartNumber != NULL && *UartNumber >= GetMaxUartInterfacesNum ()) {
    return EFI_UNSUPPORTED;
  }

  if (mSerialIoUartLibAtRuntime) {
    return EFI_UNSUPPORTED;
  }

  if (!IsSerialIoUartInPciMode (PciCfgBase, UartNumber)) {
    return EFI_UNSUPPORTED;
  }

  if (UartNumber != NULL) {
    PciCfgBase  = (UINTN) GetSerialIoUartPciCfg (*UartNumber);
  }

  SerialIoSetD3 (PciCfgBase);
  SerialIoDisableMse (PciCfgBase);

  //
  // Assign MMIO Base Address
  //
  PciSegmentWrite32 (PciCfgBase + PCI_BASE_ADDRESSREG_OFFSET, (UINT32) MmioBaseAddress);
  if (PciSegmentRead32 (PciCfgBase + PCI_BASE_ADDRESSREG_OFFSET) & BIT2) {
    PciSegmentWrite32 (PciCfgBase + PCI_BASE_ADDRESSREG_OFFSET + 4, (UINT32) RShiftU64 (MmioBaseAddress, 32));
  }

  SerialIoSetD0 (PciCfgBase);
  SerialIoEnableMse (PciCfgBase, FALSE);

  return EFI_SUCCESS;
}