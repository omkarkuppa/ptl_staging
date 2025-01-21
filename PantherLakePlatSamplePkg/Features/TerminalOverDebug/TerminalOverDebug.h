/** @file
  Header file for Terminal Over Debug Driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef _TERMINAL_OVER_DEBUG_H_
#define _TERMINAL_OVER_DEBUG_H_

#include <Uefi.h>

#include <IndustryStandard/Pci.h>
#include <Protocol/PciIo.h>
#include <Protocol/TerminalOverDebugIO.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>
#include <Library/DevicePathLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>
#include <Protocol/SerialIo.h>
#include <Protocol/TerminalOverCommon.h>

//
// Driver Binding Externs
//
extern EFI_DRIVER_BINDING_PROTOCOL  gDciControllerDriver;
extern EFI_COMPONENT_NAME_PROTOCOL  gDciMailboxComponentName;
extern EFI_COMPONENT_NAME2_PROTOCOL gDciMailboxComponentName2;

//
// Internal Data Structures
//
#define TIMEOUT_STALL_INTERVAL  10

#pragma pack(1)
///
/// PcdPciSerialParameters contains zero or more instances of the below structure.
/// If a PCI device contains multiple UARTs, PcdPciSerialParameters needs to contain
/// two instances of the below structure, with the VendorId and DeviceId equals to the
/// device ID and vendor ID of the device. If the PCI device uses the first two BARs
/// to support multiple UARTs, BarIndex of first instance equals to 0 and BarIndex of
/// second one equals to 1; if the PCI device uses the first BAR to support multiple
/// UARTs, BarIndex of both instance equals to 0 and Offset of first instance equals
/// to 0 while Offset of second one equals to some value bigger or equal to 8.
/// For certain UART whose register needs to be accessed in DWORD aligned address,
/// RegisterStride equals to 4.
///
typedef struct {
  UINT16  VendorId;          ///< Vendor ID to match the PCI device.  The value 0xFFFF terminates the list of entries.
  UINT16  DeviceId;          ///< Device ID to match the PCI device for DCI Mailbox
  UINT32  ClockRate;         ///< DCI clock rate.  Set to 0 for default clock rate of 1843200 Hz
  UINT64  Offset;            ///< The byte offset into to the BAR
  UINT8   BarIndex;          ///< Which BAR to get the DCI Mailbox base address
  UINT8   RegisterStride;    ///< DCI register stride in bytes.  Set to 0 for default register stride of 1 byte.
  UINT16  ReceiveFifoDepth;  ///< DCI receive FIFO depth in bytes. Set to 0 for a default FIFO depth of 16 bytes.
  UINT16  TransmitFifoDepth; ///< DCI transmit FIFO depth in bytes. Set to 0 for a default FIFO depth of 16 bytes.
  UINT8   Reserved[2];
} DCI_MB_PARAMETER;
#pragma pack()

#define DCI_MAX_FIFO_SIZE 17       ///< Actual FIFO size is 16. FIFO based on circular wastes one unit.
typedef struct {
  UINT16  Head;                    ///< Head pointer of the FIFO. Empty when (Head == Tail).
  UINT16  Tail;                    ///< Tail pointer of the FIFO. Full when ((Tail + 1) % SERIAL_MAX_FIFO_SIZE == Head).
  UINT8   Data[DCI_MAX_FIFO_SIZE]; ///< Store the FIFO data.
} DCI_DEV_FIFO;

#define DCI_MB_SIGNATURE    SIGNATURE_32 ('d', 'c', 'i', 'm')
typedef struct {
  UINT32                        Signature;
  EFI_HANDLE                    Handle;
  EFI_DCI_IO_PROTOCOL           DciIo;
  EFI_DCI_IO_MODE               DciMode;
  EFI_DEVICE_PATH_PROTOCOL      *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL      *ParentDevicePath;
  DCI_MB_PATH                   DciMailboxPath;
  EFI_PHYSICAL_ADDRESS          BaseAddress;            ///< UART base address
  BOOLEAN                       MmioAccess;             ///< TRUE for MMIO, FALSE for IO
  UINT8                         RegisterStride;         ///< UART Register Stride
  DCI_DEV_FIFO                  Receive;                ///< The FIFO used to store received data
  DCI_DEV_FIFO                  Transmit;               ///< The FIFO used to store to-transmit data
  EFI_UNICODE_STRING_TABLE      *ControllerNameTable;
  BOOLEAN                       ContainsControllerNode; ///< TRUE if the device produced contains Controller node
  UINT32                        Instance;
} DCI_MB;

//
// DCI Mailbox Driver Defaults
//
#define DCI_MB_DEFAULT_TIMEOUT             1000000

#pragma pack(1)

#pragma pack()

//
// Prototypes
// Driver model protocol interface
//
/**
  Check to see if this driver supports the given controller

  @param  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param  Controller           The handle of the controller to test.
  @param  RemainingDevicePath  A pointer to the remaining portion of a device path.

  @return EFI_SUCCESS          This driver can support the given controller

**/
EFI_STATUS
EFIAPI
DciControllerDriverSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  );

/**
  Start to management the controller passed in

  @param  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param  Controller           The handle of the controller to test.
  @param  RemainingDevicePath  A pointer to the remaining portion of a device path.

  @return EFI_SUCCESS          Driver is started successfully
**/
EFI_STATUS
EFIAPI
DciControllerDriverStart (
  IN EFI_DRIVER_BINDING_PROTOCOL    *This,
  IN EFI_HANDLE                     Controller,
  IN EFI_DEVICE_PATH_PROTOCOL       *RemainingDevicePath
  );

/**
  Disconnect this driver with the controller, uninstall related protocol instance

  @param  This                  A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param  Controller            The handle of the controller to test.
  @param  NumberOfChildren      Number of child device.
  @param  ChildHandleBuffer     A pointer to the remaining portion of a device path.

  @retval EFI_SUCCESS           Operation successfully
  @retval EFI_DEVICE_ERROR      Cannot stop the driver successfully

**/
EFI_STATUS
EFIAPI
DciControllerDriverStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL   *This,
  IN  EFI_HANDLE                    Controller,
  IN  UINTN                         NumberOfChildren,
  IN  EFI_HANDLE                    *ChildHandleBuffer
  );

//
// DCI Mailbox I/O Protocol Interface
//
/**
  Reset serial device.

  @param This               Pointer to EFI_SERIAL_IO_PROTOCOL

  @retval EFI_SUCCESS        Reset successfully
  @retval EFI_DEVICE_ERROR   Failed to reset

**/
EFI_STATUS
EFIAPI
DciMbReset (
  IN EFI_DCI_IO_PROTOCOL           *This
  );

/**
  Set new attributes to a serial device.

  @param  This                     Pointer to EFI_SERIAL_IO_PROTOCOL
  @param  ReceiveFifoDepth         The depth of receive FIFO buffer
  @param  Timeout                  The request timeout for a single char

  @retval  EFI_SUCCESS              The new attributes were set
  @retval  EFI_INVALID_PARAMETERS   One or more attributes have an unsupported value
  @retval  EFI_UNSUPPORTED          Data Bits can not set to 5 or 6
  @retval  EFI_DEVICE_ERROR         The serial device is not functioning correctly (no return)

**/
EFI_STATUS
EFIAPI
DciMbSetAttributes (
  IN EFI_DCI_IO_PROTOCOL            *This,
  IN UINT32                         ReceiveFifoDepth,
  IN UINT32                         Timeout
 );

/**
  Set Control Bits.

  @param This              Pointer to EFI_SERIAL_IO_PROTOCOL
  @param Control           Control bits that can be settable

  @retval EFI_SUCCESS       New Control bits were set successfully
  @retval EFI_UNSUPPORTED   The Control bits wanted to set are not supported

**/
EFI_STATUS
EFIAPI
DciMbSetControl (
  IN EFI_DCI_IO_PROTOCOL            *This,
  IN UINT32                         Control
  );

/**
  Get Control Bits.

  @param This          Pointer to EFI_SERIAL_IO_PROTOCOL
  @param Control       Control signals of the serial device

  @retval EFI_SUCCESS   Get Control signals successfully

**/
EFI_STATUS
EFIAPI
DciMbGetControl (
  IN EFI_DCI_IO_PROTOCOL            *This,
  OUT UINT32                        *Control
  );

/**
  Write the specified number of bytes to serial device.

  @param This                Pointer to EFI_SERIAL_IO_PROTOCOL
  @param  BufferSize         On input the size of Buffer, on output the amount of
                             data actually written
  @param  Buffer             The buffer of data to write

  @retval EFI_SUCCESS        The data were written successfully
  @retval EFI_DEVICE_ERROR   The device reported an error
  @retval EFI_TIMEOUT        The write operation was stopped due to timeout

**/
EFI_STATUS
EFIAPI
DciMbWrite (
  IN EFI_DCI_IO_PROTOCOL            *This,
  IN OUT UINTN                      *BufferSize,
  IN VOID                           *Buffer
  );

/**
  Read the specified number of bytes from serial device.

  @param This               Pointer to EFI_SERIAL_IO_PROTOCOL
  @param BufferSize         On input the size of Buffer, on output the amount of
                            data returned in buffer
  @param Buffer             The buffer to return the data into

  @retval EFI_SUCCESS        The data were read successfully
  @retval EFI_DEVICE_ERROR   The device reported an error
  @retval EFI_TIMEOUT        The read operation was stopped due to timeout

**/
EFI_STATUS
EFIAPI
DciMbRead (
  IN EFI_DCI_IO_PROTOCOL             *This,
  IN OUT UINTN                       *BufferSize,
  OUT VOID                           *Buffer
  );

#endif
