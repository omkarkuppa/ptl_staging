/** @file
  Include for Serial Io Uart Driver.

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
#ifndef _SERIAL_IO_UART_DRIVER_H_
#define _SERIAL_IO_UART_DRIVER_H_

#include <Protocol/SerialIo.h>
#include <Protocol/DriverBinding.h>
#include <Library/UefiLib.h>

#define SERIAL_IO_UART_DEV_SIGNATURE    SIGNATURE_32 ('S', 'E', 'R', 'U')
#define SERIAL_IO_UART_NAME L"Serial IO UART Driver Controller #%d"
#define SERIAL_IO_UART_DEV_FROM_THIS(a) CR (a, SERIAL_IO_UART_DEV, SerialIoProtocol, SERIAL_IO_UART_DEV_SIGNATURE)

//
//

typedef struct {
  ACPI_EXTENDED_HID_DEVICE_PATH   AcpiExtendedPath;
  CHAR8                           HidString[8];
  UINT32                          PciCfgBase;
  UINT16                          DeviceId;
  UINT8                           UartIndex;
} SERIAL_IO_UART_ACPI_EXTENDED_DEVICE_PATH;

typedef struct {
  UINTN                     Signature;
  EFI_HANDLE                Handle;
  EFI_SERIAL_IO_PROTOCOL    SerialIoProtocol;
  EFI_SERIAL_IO_MODE        SerialIoMode;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL  *ParentDevicePath;
  UART_DEVICE_PATH          UartDevicePath;
  UINT32                    PciCfgBase;
  UINT8                     UartIndex;
  BOOLEAN                   AutoFlow;
  EFI_UNICODE_STRING_TABLE  *ControllerNameTable;
} SERIAL_IO_UART_DEV;

#endif // _SERIAL_IO_UART_DRIVER_H_
