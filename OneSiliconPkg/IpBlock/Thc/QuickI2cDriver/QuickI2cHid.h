/** @file
  Touch Host Controller Human Interface Device API

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

@par Specification Reference:
**/

#ifndef _THC_HID_H_
#define _THC_HID_H_

#include "QuickI2cPrivate.h"

/**
  Performs SetFeature function as described in Human Interface Device specification.

  @param[in]  This       Pointer to instance of protocol.
  @param[in]  Length     Size of the buffer.
  @param[in]  Buffer      On input, contains data to be sent to host.
  @param[in]  Timeout    0 - No timeout, do not wait for response
                         1 or higher - will wait for that amount of time and copy response results to the same buffer.

  @retval EFI_NOT_READY         THC is not ready
  @retval EFI_ALREADY_STARTED   HID transaction is still active
  @retval EFI_NOT_AVAILABLE_YET THC read transaction is ongoing
  @retval EFI_TIMEOUT           a) Response did not come in time OR
                                b) DMA transaction did not finish in time
  @retval EFI_BUFFER_TOO_SMALL  THC DMA buffer is unable to fit that much data
  @retval EFI_SUCCESS           Set feature completed
**/
EFI_STATUS
EFIAPI
QuickI2cHidSetFeature (
  IN THC_PROTOCOL   *This,
  IN UINT32         Length,
  IN OUT UINT8      *Buffer,
  IN UINTN          Timeout
  );

/**
  Performs GetFeature function as described in Human Interface Device specification.

  @param[in]  This       Pointer to instance of protocol.
  @param[in]  Length     Size of the buffer.
  @param[in]  Buffer     On input, contains ReportId in 1st byte. On output, filled with Feature data from external device.
  @param[in]  Timeout    0 - No timeout, do not wait for response
                         1 or higher - will wait for that amount of time and copy response results to the same buffer.

  @retval EFI_NOT_READY         THC is not ready
  @retval EFI_ALREADY_STARTED   HID transaction is still active
  @retval EFI_NOT_AVAILABLE_YET THC read transaction is ongoing
  @retval EFI_TIMEOUT           a) Response did not come in time OR
                                b) DMA transaction did not finish in time
  @retval EFI_BUFFER_TOO_SMALL  THC DMA buffer is unable to fit that much data
  @retval EFI_SUCCESS           Get feature completed
**/
EFI_STATUS
EFIAPI
QuickI2cHidGetFeature (
  IN THC_PROTOCOL   *This,
  IN UINT32         Length,
  IN OUT UINT8      *Buffer,
  IN UINTN          Timeout
  );

/**
  Performs SetPowerState function as described in Human Interface Device specification.
  @param[in]  This       Pointer to instance of protocol.
  @param[in]  Length     Size of the buffer.
  @param[out] Buffer     On input, contains data to be sent to host.
  @param[in]  PowerState Power State.
  @retval EFI_NOT_READY         THC is not ready
  @retval EFI_ALREADY_STARTED   HID transaction is still active
  @retval EFI_NOT_AVAILABLE_YET THC read transaction is ongoing
  @retval EFI_TIMEOUT           a) Response did not come in time OR
                                b) DMA transaction did not finish in time
  @retval EFI_BUFFER_TOO_SMALL  THC DMA buffer is unable to fit that much data
  @retval EFI_SUCCESS           Set feature completed
**/
EFI_STATUS
EFIAPI
QuickI2cHidSetPowerState (
  IN THC_PROTOCOL   *This,
  IN UINT32         Length,
  IN OUT UINT8      *Buffer,
  IN UINT8          PowerState
  );

/**
  Performs perform reset function as described in Human Interface Device specification.
  @param[in]  This       Pointer to instance of protocol.
  @param[in]  Length     Size of the buffer.
  @param[out] Buffer     On input, contains data to be sent to host.
  @param[in]  Timeout    0 - No timeout, do not wait for response
                         1 or higher - will wait for that amount of time and copy response results to the same buffer.
  @retval EFI_NOT_READY         THC is not ready
  @retval EFI_ALREADY_STARTED   HID transaction is still active
  @retval EFI_NOT_AVAILABLE_YET THC read transaction is ongoing
  @retval EFI_TIMEOUT           a) Response did not come in time OR
                                b) DMA transaction did not finish in time
  @retval EFI_BUFFER_TOO_SMALL  THC DMA buffer is unable to fit that much data
  @retval EFI_SUCCESS           Set feature completed
**/
EFI_STATUS
EFIAPI
QuickI2cHidPerformReset (
  IN THC_PROTOCOL   *This,
  IN UINT32         Length,
  IN OUT UINT8      *Buffer,
  IN UINTN          Timeout
  );

/**
  Performs Get input report function as described in Human Interface Device specification.
  @param[in]  This       Pointer to instance of protocol.
  @param[in]  Length     Size of the buffer.
  @param[in]  Buffer     On input, contains ReportId in 1st byte. On output, filled with Feature data from external device.
                         The caller is responsible for freeing the buffer that is allocated by callee for HidPacket.ReportBuffer.
  @param[in]  Timeout    0 - No timeout, do not wait for response
                         1 or higher - will wait for that amount of time and copy response results to the same buffer.
  @retval EFI_NOT_READY         THC is not ready
  @retval EFI_ALREADY_STARTED   HID transaction is still active
  @retval EFI_NOT_AVAILABLE_YET THC read transaction is ongoing
  @retval EFI_TIMEOUT           a) Response did not come in time OR
                                b) DMA transaction did not finish in time
  @retval EFI_BUFFER_TOO_SMALL  THC DMA buffer is unable to fit that much data
  @retval EFI_SUCCESS           Set feature completed
**/
EFI_STATUS
EFIAPI
QuickI2cHidGetInputReport (
  IN THC_PROTOCOL   *This,
  IN UINT32         Length,
  IN OUT UINT8      *Buffer,
  IN UINTN          Timeout
  );

/**
  Performs SetOutput report function as described in Human Interface Device specification.
  @param[in]  This       Pointer to instance of protocol.
  @param[in]  Length     Size of the buffer.
  @param[out] Buffer     On input, contains data to be sent to host.
  @param[in]  Timeout    0 - No timeout, do not wait for response
                         1 or higher - will wait for that amount of time and copy response results to the same buffer.
  @retval EFI_NOT_READY         THC is not ready
  @retval EFI_ALREADY_STARTED   HID transaction is still active
  @retval EFI_NOT_AVAILABLE_YET THC read transaction is ongoing
  @retval EFI_TIMEOUT           a) Response did not come in time OR
                                b) DMA transaction did not finish in time
  @retval EFI_BUFFER_TOO_SMALL  THC DMA buffer is unable to fit that much data
  @retval EFI_SUCCESS           Set feature completed
**/
EFI_STATUS
EFIAPI
QuickI2cHidSetOutputReport (
  IN THC_PROTOCOL   *This,
  IN UINT32         Length,
  IN OUT UINT8      *Buffer,
  IN UINTN          Timeout
  );

/**
  Provide device descriptor to caller.
  @param[in]  This       Pointer to instance of protocol.
  @param[in]  Length     Size of the buffer.
  @param[out] Buffer     On output, filled with descriptor data for external device.
                         The caller is responsible for freeing the buffer that is allocated by callee for HidPacket.ReportBuffer.
  @param[in]  Timeout    0 - No timeout, do not wait for response
                         1 or higher - will wait for that amount of time and copy response results to the same buffer.
  @retval EFI_NOT_READY         THC is not ready
  @retval EFI_ALREADY_STARTED   HID transaction is still active
  @retval EFI_NOT_AVAILABLE_YET THC read transaction is ongoing
  @retval EFI_TIMEOUT           a) Response did not come in time OR
                                b) DMA transaction did not finish in time
  @retval EFI_BUFFER_TOO_SMALL  THC DMA buffer is unable to fit that much data
  @retval EFI_SUCCESS           Set feature completed
**/
EFI_STATUS
EFIAPI
QuickI2cHidGetDeviceDescriptor (
  IN THC_PROTOCOL   *This,
  IN UINT32         Length,
  IN OUT UINT8      *Buffer,
  IN UINTN          Timeout
  );

/**
  Provides Input HID report to caller.
  @param[in]  This       Pointer to instance of protocol.
  @param[in]  Length     Size of the buffer.
  @param[out] Buffer     On output, filled with descriptor data for external device.
                         The caller is responsible for freeing the buffer that is allocated by callee for HidPacket.ReportBuffer.
  @param[in]  Timeout    0 - No timeout, do not wait for response
                         1 or higher - will wait for that amount of time and copy response results to the same buffer.
  @retval EFI_NOT_READY         THC is not ready
  @retval EFI_ALREADY_STARTED   HID transaction is still active
  @retval EFI_NOT_AVAILABLE_YET THC read transaction is ongoing
  @retval EFI_TIMEOUT           a) Response did not come in time OR
                                b) DMA transaction did not finish in time
  @retval EFI_BUFFER_TOO_SMALL  THC DMA buffer is unable to fit that much data
  @retval EFI_SUCCESS           Set feature completed
**/
EFI_STATUS
EFIAPI
QuickI2cGetHidReportDescriptor (
  IN THC_PROTOCOL   *This,
  IN UINT32         Length,
  IN OUT UINT8      *Buffer,
  IN UINTN          Timeout
  );

/**
  Enables end point device.
  - Reads all Touch Panels devices registers
  - Sets TouchEnable

  @param[in]  This             Pointer to instance of protocol.

  @retval EFI_SUCCESS          Enabling completed
  @retval EFI_DEVICE_ERROR     TSSDONE not set or ERR set
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
EFIAPI
QuickI2cHidEnableAfterReset (
  IN THC_PROTOCOL  *This
  );

/**
  Parses HID Descriptor and creates Report Tables

  @param[in]  QuickI2cDev       Context of QuickI2c device
  @param[in]  Descriptor        Pointer to the descriptor
  @param[in]  DescriptorLength  Size of the descriptor to parse
**/
VOID
HidParseDescriptor (
  IN QUICK_I2C_DEV   *QuickI2cDev,
  IN UINT8           *Descriptor,
  IN UINT32          DescriptorLength
  );

/**
  This function uses dictionaries to parse incoming InputReport and convert it into X/Y coordinates plus Button info.

  @param[in]  ReportTable             Report Table with all supported HID reports.
  @param[in]  InputStream             Pointer to the HID report.
  @param[out] Output                  Result X/Y/B data.
  @param[out] MouseTouchOutput        Result relative X/Y data.
  @param[in]  MinMax                  X/Y Min and Max data.
  @param[in]  HidSolutionFlag         Flag for HID protocol.
  @param[in]  UsageDevice             Device Usage

  @retval EFI_SUCCESS       Parsing completed.
  @retval EFI_NOT_FOUND     Corresponding Report ID was not found in the Report Table.
**/
EFI_STATUS
HidParseInput (
  IN HID_INPUT_REPORT_TABLE     ReportTable,
  IN UINT8                      *InputStream,
  IN OUT HID_TOUCH_OUTPUT       *Output,
  IN OUT HID_REL_TOUCH_OUTPUT   *MouseTouchOutput,
  IN HID_XY_BOUNDARY            *MinMax,
  IN UINT8                      HidSolutionFlag,
  IN OUT UINT8                  *UsageDevice
  );

#endif // _THC_HID_H_
