/** @file
  DCI Mailbox IO protocol .

  Abstraction of a DCI Mailbox device. Targeted at  EXI DCI Mailbox communication.

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

#ifndef __DCI_IO_PROTOCOL_H__
#define __DCI_IO_PROTOCOL_H__

typedef struct _EFI_DCI_IO_PROTOCOL EFI_DCI_IO_PROTOCOL;
//
// Backward-compatible with EFI1.1.
//
typedef EFI_DCI_IO_PROTOCOL  DCI_IO_INTERFACE;

//
// DCI IO Member Functions
//
/**
  Reset the DCI device.

  @param  This              Protocol instance pointer.

  @retval EFI_SUCCESS       The device was reset.
  @retval EFI_DEVICE_ERROR  The DCI device could not be reset.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_DCI_RESET)(
  IN EFI_DCI_IO_PROTOCOL       *This
  );

/**
  Sets Attributes for DCI mailbox device.

  @param  This             Protocol instance pointer.
  @param  ReveiveFifoDepth The requested depth of the FIFO on the receive side of the
                           DCI interface. A ReceiveFifoDepth value of 0 will use
                           the device's default FIFO depth.
  @param  Timeout          The requested time out for a single character in microseconds.
                           This timeout applies to both the transmit and receive side of the
                           interface. A Timeout value of 0 will use the device's default time
                           out value.


  @retval EFI_SUCCESS           The device was reset.
  @retval EFI_INVALID_PARAMETER One or more attributes has an unsupported value.
  @retval EFI_DEVICE_ERROR      The DCI device is not functioning correctly.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_DCI_SET_ATTRIBUTES)(
  IN EFI_DCI_IO_PROTOCOL            *This,
   IN UINT32                        ReceiveFifoDepth,
  IN UINT32                         Timeout
  );

/**
  Set the control bits on a DCI device

  @param  This             Protocol instance pointer.
  @param  Control          Set the bits of Control that are settable.

  @retval EFI_SUCCESS      The new control bits were set on the DCI device.
  @retval EFI_UNSUPPORTED  The DCI device does not support this operation.
  @retval EFI_DEVICE_ERROR The DCI device is not functioning correctly.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_DCI_SET_CONTROL_BITS)(
  IN EFI_DCI_IO_PROTOCOL            *This,
  IN UINT32                         Control
  );

/**
  Retrieves the status of the control bits on a DCI device

  @param  This              Protocol instance pointer.
  @param  Control           A pointer to return the current Control signals from the DCI device.

  @retval EFI_SUCCESS       The control bits were read from the DCI device.
  @retval EFI_DEVICE_ERROR  The DCI device is not functioning correctly.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_DCI_GET_CONTROL_BITS)(
  IN EFI_DCI_IO_PROTOCOL            *This,
  OUT UINT32                        *Control
  );

/**
  Writes data to a DCI mailbox device.

  @param  This              Protocol instance pointer.
  @param  BufferSize        On input, the size of the Buffer. On output, the amount of
                            data actually written.
  @param  Buffer            The buffer of data to write

  @retval EFI_SUCCESS       The data was written.
  @retval EFI_DEVICE_ERROR  The device reported an error.
  @retval EFI_TIMEOUT       The data write was stopped due to a timeout.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_DCI_WRITE)(
  IN EFI_DCI_IO_PROTOCOL            *This,
  IN OUT UINTN                      *BufferSize,
  IN VOID                           *Buffer
  );

/**
  Read data from a DCI device.

  @param  This              Protocol instance pointer.
  @param  BufferSize        On input, the size of the Buffer. On output, the amount of
                            data returned in Buffer.
  @param  Buffer            The buffer to return the data into.

  @retval EFI_SUCCESS       The data was read.
  @retval EFI_DEVICE_ERROR  The device reported an error.
  @retval EFI_TIMEOUT       The data write was stopped due to a timeout.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_DCI_READ)(
  IN EFI_DCI_IO_PROTOCOL            *This,
  IN OUT UINTN                      *BufferSize,
  OUT VOID                          *Buffer
  );

/**
  @par Data Structure Description:
  The data values in DCI_IO_MODE are read-only and are updated by the code
  that produces the EFI_DCI_IO_PROTOCOL member functions.

  @param ControlMask
  A mask for the Control bits that the device supports. The device
  must always support the Input Buffer Empty control bit.

  @param TimeOut
  If applicable, the number of microseconds to wait before timing out
  a Read or Write operation.


  @param ReceiveFifoDepth
  The number of characters the device will buffer on input

**/
typedef struct {
  UINT32  ControlMask;
  //
  // current Attributes
  //
  UINT32  Timeout;
  UINT32  ReceiveFifoDepth;
} EFI_DCI_IO_MODE;

#define EFI_DCI_IO_PROTOCOL_REVISION    0x00010000
#define EFI_DCI_IO_PROTOCOL_REVISION1p1 0x00010001
#define DCI_IO_INTERFACE_REVISION  EFI_DCI_IO_PROTOCOL_REVISION

//
// The DCI I/O protocol is used to communicate with EXI DCI Mailbox devices.
//
struct _EFI_DCI_IO_PROTOCOL {
  ///
  /// The revision to which the EFI_DCI_IO_PROTOCOL adheres. All future revisions
  /// must be backwards compatible. If a future version is not backwards compatible,
  /// it is not the same GUID.
  ///
  UINT32                   Revision;
  EFI_DCI_RESET            Reset;
  EFI_DCI_SET_ATTRIBUTES   SetAttributes;
  EFI_DCI_SET_CONTROL_BITS SetControl;
  EFI_DCI_GET_CONTROL_BITS GetControl;
  EFI_DCI_WRITE            Write;
  EFI_DCI_READ             Read;
  ///
  /// Pointer to DCI_IO_MODE data.
  ///
  EFI_DCI_IO_MODE          *Mode;
  ///
  /// Pointer to a GUID identifying the device connected to the DCI port.
  /// This field is NULL when the protocol is installed by the DCI port
  /// driver and may be populated by a platform driver for a DCI port
  /// with a known device attached. The field will remain NULL if there is
  /// no platform DCI device identification information available.
  ///
  CONST EFI_GUID           *DeviceTypeGuid;
};

extern EFI_GUID gEfiDciIoProtocolGuid;

#endif
