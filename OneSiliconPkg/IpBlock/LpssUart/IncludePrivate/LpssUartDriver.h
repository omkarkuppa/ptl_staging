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

#ifndef _LPSS_UART_DRIVER_H_
#define _LPSS_UART_DRIVER_H_

#include <Protocol/SerialIo.h>
#include <Protocol/DriverBinding.h>
#include <Library/UefiLib.h>

#define LPSS_UART_DEV_SIGNATURE    SIGNATURE_32 ('L', 'U', 'R', 'T')
#define LPSS_UART_NAME             L"LPSS UART Driver Controller #%d"
#define LPSS_UART_DEV_FROM_THIS(a) CR (a, LPSS_UART_DEV, SerialIoProtocol, LPSS_UART_DEV_SIGNATURE)

typedef struct {
  ACPI_EXTENDED_HID_DEVICE_PATH   AcpiExtendedPath;
  CHAR8                           HidString[8];
  UINTN                           FixedMmio;
  UINT16                          DeviceId;
  UINT8                           UartIndex;
} LPSS_UART_ACPI_EXTENDED_DEVICE_PATH;

typedef struct {
  UINTN                     Signature;
  EFI_HANDLE                Handle;
  EFI_SERIAL_IO_PROTOCOL    SerialIoProtocol;
  EFI_SERIAL_IO_MODE        SerialIoMode;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL  *ParentDevicePath;
  UART_DEVICE_PATH          UartDevicePath;
  UINTN                     FixedMmio;
  BOOLEAN                   AutoFlow;
  EFI_UNICODE_STRING_TABLE  *ControllerNameTable;
} LPSS_UART_DEV;

typedef struct {
  UINTN                            Signature;
  EFI_SERIAL_IO_PROTOCOL           SerialIoAccess;
  UINTN                            FixedMmioBaseAddress;
  UINTN                            FixedPciCfgAddress;
  EFI_SERIAL_IO_MODE               SerialIoMode;
} LPSS_UART_DEV_INFO;

#define LPSS_UART_DEV_INFO_FROM_SERIAL_IO_PROTOCOL(a) CR (a, LPSS_UART_DEV_INFO, SerialIoAccess, LPSS_UART_DEV_SIGNATURE)

#endif // _LPSS_UART_DRIVER_H_
