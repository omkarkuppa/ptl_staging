/** @file
  Implementation of BaseSerialIoUartDebugHelperLib.

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SerialIoAccessLib.h>
#include <Library/SerialIoUartLib.h>
#include <Library/SerialIoUartDebugPropertyLib.h>

/**
  Initialize SerialIo UART for debug.

**/
VOID
EFIAPI
SerialIoUartDebugInit (
  VOID
  )
{
  UINT64                    PciCfgBase;
  UINT64                    Bar0;
  SERIAL_IO_UART_ATTRIBUTES UartAttributes;
  UINT8                     *UartNumber;
  UINT8                      UartNumFromPcd;
  EFI_STATUS                Status;

  UartNumber = NULL;
  UartNumFromPcd = SerialIoUartDebugGetControllerNumber ();

  if (UartNumFromPcd == 0xFF) {
    PciCfgBase = SerialIoUartDebugGetDefaultPciCfgBase ();
  } else {
    UartNumber = &UartNumFromPcd;
    PciCfgBase = GetSerialIoUartPciCfg (*UartNumber);
  }
  Bar0       = GetSerialIoBar (PciCfgBase);
  SetMem (&UartAttributes, sizeof (SERIAL_IO_UART_ATTRIBUTES), 0);
  SerialIoUartDebugGetAttributes (&UartAttributes);

  //
  // Check BAR0 whether is assigned value in either Hidden or PCI Mode.
  //
  if ((Bar0 !=  0x0) && (Bar0 != 0xFFFFF000)) {
    SerialIoUartSetAttributes (
      (UINTN) PciCfgBase,
      UartNumber,
      UartAttributes.BaudRate,
      UartAttributes.Parity,
      UartAttributes.DataBits,
      UartAttributes.StopBits,
      UartAttributes.AutoFlow
      );
    return;
  }

  //
  // Assign MMIO for HS-UART used in PCI Mode.
  //
  if (!IsSerialIoUartInPciMode ((UINTN) PciCfgBase, UartNumber)) {
    return;
  }
  Bar0 = SerialIoUartDebugGetPciDefaultMmioBase ();
  Status = SerialIoUartSetMmioInPciMode ((UINTN) PciCfgBase, UartNumber, Bar0);
  if (Status == EFI_UNSUPPORTED) {
    return;
  }

  //
  // UART Reset
  //
  SerialIoUartGetOutOfReset (Bar0);

  //
  // Initialize UART Attributes
  //
  SerialIoUartSetAttributes (
    (UINTN) PciCfgBase,
    UartNumber,
    UartAttributes.BaudRate,
    UartAttributes.Parity,
    UartAttributes.DataBits,
    UartAttributes.StopBits,
    UartAttributes.AutoFlow
    );
}

/**
  Write data to serial device.

  If the buffer is NULL, then return 0;
  if NumberOfBytes is zero, then return 0.

  @param[in]  Buffer           Data pointer
  @param[in]  NumberOfBytes    Number of output bytes which are cached in Buffer.

  @retval                      Actual number of bytes written to serial device.
**/
UINTN
EFIAPI
SerialIoUartDebugWrite (
  IN UINT8            *Buffer,
  IN UINTN            NumberOfBytes
  )
{
  UINT8  UartIndex;

  UartIndex = SerialIoUartDebugGetControllerNumber ();
  if (UartIndex == 0xFF) {
    return SerialIoUartWrite ((UINTN) SerialIoUartDebugGetDefaultPciCfgBase (), NULL, Buffer, NumberOfBytes);
  } else {
    return SerialIoUartWrite (0, &UartIndex, Buffer, NumberOfBytes);
  }
}
