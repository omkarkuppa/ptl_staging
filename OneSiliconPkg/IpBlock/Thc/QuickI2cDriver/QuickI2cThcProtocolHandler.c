/** @file
  Touch Host Controller Human Interface Device API

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#include "QuickI2cPrivate.h"
#include "QuickI2cDriver.h"

/**
  Performs Sanity Checks before write transfer starts
  @param[in]  QuickI2cDev            Context of QuickI2c device
  @param[in]  Length            Size of the buffer
  @retval EFI_NOT_READY         THC is not ready
  @retval EFI_ALREADY_STARTED   HID transaction is still active
  @retval EFI_NOT_AVAILABLE_YET THC read transaction is ongoing
  @retval EFI_SUCCESS           All checks passed
**/
EFI_STATUS
STATIC
HidSanityCheck (
  IN QUICK_I2C_DEV        *QuickI2cDev,
  IN UINT32               Length
  )
{
  if (QuickI2cDev->ThcProtocol.DeviceStatus != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "ThcProtocol HidSanityCheck Error QuickI2c Status: %r\n", QuickI2cDev->ThcProtocol.DeviceStatus));
    return EFI_NOT_READY;
  }

  if (QuickI2cDev->HidActive == TRUE) {
    DEBUG ((DEBUG_ERROR, "ThcProtocol HidSanityCheck Error HID transaction is still active: %r\n", EFI_ALREADY_STARTED));
    return EFI_ALREADY_STARTED;
  }

  if (QuickI2cDev->ReadDone == FALSE) {
    DEBUG ((DEBUG_ERROR, "ThcProtocol HidSanityCheck Error THC is still recieving data: %r\n", EFI_NOT_AVAILABLE_YET));
    return EFI_NOT_AVAILABLE_YET;
  }

  return EFI_SUCCESS;
}

/**
  Performs GetFeature function as described in Human Interface Device specification.
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
  @retval EFI_SUCCESS           Get feature completed
**/
EFI_STATUS
EFIAPI
QuickI2cHidGetFeature (
  IN THC_PROTOCOL   *This,
  IN UINT32         Length,
  IN OUT UINT8      *Buffer,
  IN UINTN          Timeout
  )
{
  EFI_STATUS        Status;
  QUICK_I2C_DEV     *QuickI2cDev;
  HID_PACKET        *HidPacket;

  HidPacket   = (HID_PACKET*) Buffer;

  if (This == NULL || HidPacket->ReportId == 0) {
    DEBUG ((DEBUG_ERROR, "QuickI2cHidGetFeature: Invalid input parameters\n"));
    return EFI_INVALID_PARAMETER;
  }

  QuickI2cDev = QUICK_I2C_CONTEXT_FROM_THC_PROTOCOL (This);

  DEBUG ((DEBUG_INFO, "%a: Entry () \n", __FUNCTION__));
  //
  // Sanity checks before transfer
  //
  Status = HidSanityCheck (QuickI2cDev, Length);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cHidGetFeature: HidSanityCheck returned Status- %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cHidGetFeature: HidSanityCheck ERROR Status %r\n", Status));
    return Status;
  }

  Status = QuickI2cGetFeature (QuickI2cDev, HidPacket, QuickI2cReportTypeFeature, Timeout);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cHidGetFeature: QuickI2cGetFeature returned Status- %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cHidGetFeature: QuickI2cGetFeature ERROR Status %r\n", Status));
    return Status;
  }

  return Status;
}

/**
  Performs SetFeature function as described in Human Interface Device specification.
  @param[in]  This       Pointer to instance of protocol.
  @param[in]  Length     Size of the buffer.
  @param[in] Buffer      On input, contains data to be sent to host.
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
  )
{
  EFI_STATUS        Status;
  QUICK_I2C_DEV     *QuickI2cDev;
  HID_PACKET        *HidPacket;

  if (This == NULL) {
    DEBUG ((DEBUG_ERROR, "QuickI2cHidSetFeature: Invalid input parameters\n"));
    return EFI_INVALID_PARAMETER;
  }

  HidPacket   = (HID_PACKET*) Buffer;
  QuickI2cDev = QUICK_I2C_CONTEXT_FROM_THC_PROTOCOL (This);

  DEBUG ((DEBUG_INFO, "%a: Entry () \n", __FUNCTION__));
  //
  // Sanity checks before transfer
  //
  Status = HidSanityCheck (QuickI2cDev, HidPacket->ReportBufferLen);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cHidSetFeature: HidSanityCheck returned Status- %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cHidSetFeature: HidSanityCheck ERROR Status %r\n", Status));
    return Status;
  }

  Status = QuickI2cSetFeature (QuickI2cDev, HidPacket, QuickI2cReportTypeFeature, Timeout);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cHidSetFeature: QuickI2cSetFeature returned Status- %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cHidSetFeature: QuickI2cSetFeature ERROR Status %r\n", Status));
    return Status;
  }

  return Status;
}

/**
  Performs Set Power state function as described in Human Interface Device specification.
  @param[in]  This       Pointer to instance of protocol.
  @param[in]  Length     Size of the buffer.
  @param[in]  Buffer     On input, contains data to be sent to host.
  @param[in]  PowerState supported poser states
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
  )
{
  EFI_STATUS        Status;
  QUICK_I2C_DEV     *QuickI2cDev;
  HID_PACKET        *HidPacket;

  if (This == NULL) {
    DEBUG ((DEBUG_ERROR, "QuickI2cHidSetPowerState: Invalid input parameters\n"));
    return EFI_INVALID_PARAMETER;
  }
  HidPacket   = (HID_PACKET*) Buffer;
  QuickI2cDev = QUICK_I2C_CONTEXT_FROM_THC_PROTOCOL (This);

  DEBUG ((DEBUG_INFO, "%a: Entry () \n", __FUNCTION__));
  //
  // Sanity checks before transfer
  //
  Status = HidSanityCheck (QuickI2cDev, HidPacket->ReportBufferLen);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cHidSetPowerState: HidSanityCheck returned Status- %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cHidSetPowerState: HidSanityCheck ERROR Status %r\n", Status));
    return Status;
  }

  Status = QuickI2cSendSetPowerRequest (QuickI2cDev, (UINT8) PowerState);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cHidSetPowerState: QuickI2cSendSetPowerRequest Status - %r\n", Status);
    DEBUG ((DEBUG_WARN, "QuickI2cHidSetPowerState: QuickI2cSendSetPowerRequest: ERROR Status %r\n", Status));
    return Status;
  }

  return Status;
}

/**
  Performs PerformReset function as described in Human Interface Device specification.
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
  )
{
  EFI_STATUS        Status;
  QUICK_I2C_DEV     *QuickI2cDev;
  HID_PACKET        *HidPacket;

  if (This == NULL) {
    DEBUG ((DEBUG_ERROR, "QuickI2cHidPerformReset: Invalid input parameters\n"));
    return EFI_INVALID_PARAMETER;
  }

  HidPacket   = (HID_PACKET*) Buffer;
  QuickI2cDev = QUICK_I2C_CONTEXT_FROM_THC_PROTOCOL (This);

  DEBUG ((DEBUG_INFO, "%a: Entry () \n", __FUNCTION__));
  //
  // Sanity checks before transfer
  //
  Status = HidSanityCheck (QuickI2cDev, HidPacket->ReportBufferLen);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cHidPerformReset: HidSanityCheck returned Status- %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cHidPerformReset: HidSanityCheck ERROR Status %r\n", Status));
    return Status;
  }

  Status = QuickI2cSendResetRequest (QuickI2cDev, Timeout);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cHidPerformReset: QuickI2cSendResetRequest returned Status- %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cHidPerformReset: QuickI2cSendResetRequest: ERROR Status %r\n", Status));
    return Status;
  }

  return Status;
}

/**
  Performs Get Input Report function as described in Human Interface Device specification.
  @param[in]  This       Pointer to instance of protocol.
  @param[in]  Length     Size of the buffer.
  @param[out] Buffer     On output, contains data to be sent to external device.
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
  )
{
  EFI_STATUS        Status;
  QUICK_I2C_DEV     *QuickI2cDev;
  HID_PACKET        *HidPacket;

  HidPacket   = (HID_PACKET*) Buffer;
  if (This == NULL || HidPacket->ReportId == 0) {
    DEBUG ((DEBUG_ERROR, "QuickI2cHidGetInputReport: Invalid input parameters\n"));
    return EFI_INVALID_PARAMETER;
  }
  QuickI2cDev = QUICK_I2C_CONTEXT_FROM_THC_PROTOCOL (This);
  DEBUG ((DEBUG_INFO, "%a: Entry () \n", __FUNCTION__));

  //
  // Sanity checks before transfer
  //
  Status = HidSanityCheck (QuickI2cDev, HidPacket->ReportBufferLen);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cHidGetInputReport: HidSanityCheck returned Status- %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cHidGetInputReport: HidSanityCheck ERROR Status %r\n", Status));
    return Status;
  }

  Status = QuickI2cGetFeature (QuickI2cDev, HidPacket, QuickI2cReportTypeInput, Timeout);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cHidGetInputReport: QuickI2cGetFeature returned Status- %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cHidGetInputReport: QuickI2cGetFeature ERROR Status %r\n", Status));
    return Status;
  }

  return Status;
}

/**
  Performs Set output report function as described in Human Interface Device specification.
  @param[in]  This       Pointer to instance of protocol.
  @param[in]  Length     Size of the buffer.
  @param[out] Buffer     On output, contains data to be sent to external device.
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
  )
{
  EFI_STATUS        Status;
  QUICK_I2C_DEV     *QuickI2cDev;
  HID_PACKET        *HidPacket;

  if (This == NULL) {
    DEBUG ((DEBUG_ERROR, "QuickI2cHidSetOutputReport: Invalid input parameters\n"));
    return EFI_INVALID_PARAMETER;
  }
  HidPacket   = (HID_PACKET*) Buffer;
  QuickI2cDev = QUICK_I2C_CONTEXT_FROM_THC_PROTOCOL (This);

  DEBUG ((DEBUG_INFO, "%a: Entry () \n", __FUNCTION__));
  //
  // Sanity checks before transfer
  //
  Status = HidSanityCheck (QuickI2cDev, HidPacket->ReportBufferLen);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cHidSetOutputReport: HidSanityCheck returned Status- %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cHidSetOutputReport: HidSanityCheck ERROR Status %r\n", Status));
    return Status;
  }

  Status = QuickI2cSetFeature (QuickI2cDev, HidPacket, QuickI2cReportTypeOutput, Timeout);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cHidSetOutputReport: QuickI2cSetFeature returned Status- %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cHidSetOutputReport: QuickI2cSetFeature ERROR Status %r\n", Status));
    return Status;
  }

  return Status;
}

/**
  Performs get device descriptor function as described in Human Interface Device specification.
  @param[in]  This       Pointer to instance of protocol.
  @param[in]  Length     Size of the buffer.
  @param[out] Buffer     On output, contains data to be sent to external device.
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
  )
{
  EFI_STATUS        Status;
  QUICK_I2C_DEV     *QuickI2cDev;
  HID_PACKET        *HidPacket;

  if (This == NULL) {
    DEBUG ((DEBUG_ERROR, "QuickI2cHidGetDeviceDescriptor: Invalid input parameters\n"));
    return EFI_INVALID_PARAMETER;
  }

  HidPacket = (HID_PACKET*) Buffer;
  QuickI2cDev = QUICK_I2C_CONTEXT_FROM_THC_PROTOCOL (This);

  DEBUG ((DEBUG_INFO, "%a: Entry () \n", __FUNCTION__));
  //
  // Sanity checks before transfer
  //
  Status = HidSanityCheck (QuickI2cDev, HidPacket->ReportBufferLen);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cHidGetDeviceDescriptor: HidSanityCheck returned Status- %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cHidGetDeviceDescriptor: HidSanityCheck ERROR Status %r\n", Status));
    return Status;
  }

  HidPacket->ReportBuffer = AllocateZeroPool (sizeof (QUICK_I2C_DEVICE_DESCRIPTOR));
  if (HidPacket->ReportBuffer == NULL) {
    THC_LOCAL_DEBUG (L"QuickI2cHidGetDeviceDescriptor: AllocateZeroPool failed - %r\n", Status);
    DEBUG ((DEBUG_WARN, "QuickI2cHidGetDeviceDescriptor: AllocateZeroPool ERROR Status %r\n", Status));
    return EFI_OUT_OF_RESOURCES;
  }

  HidPacket->ReportBufferLen = sizeof (QUICK_I2C_DEVICE_DESCRIPTOR);
  CopyMem (HidPacket->ReportBuffer, &QuickI2cDev->DeviceDescriptor, sizeof (QUICK_I2C_DEVICE_DESCRIPTOR));

  return EFI_SUCCESS;
}

/**
  Performs get Hid report function as described in Human Interface Device specification.
  @param[in]  This       Pointer to instance of protocol.
  @param[in]  Length     Size of the buffer.
  @param[out] Buffer     On output, contains data to be sent to external device.
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
  )
{
  EFI_STATUS        Status;
  QUICK_I2C_DEV     *QuickI2cDev;
  HID_PACKET        *HidPacket;

  if (This == NULL) {
    DEBUG ((DEBUG_ERROR, "QuickI2cGetHidReportDescriptor: Invalid input parameters\n"));
    return EFI_INVALID_PARAMETER;
  }
  HidPacket = (HID_PACKET*) Buffer;
  QuickI2cDev = QUICK_I2C_CONTEXT_FROM_THC_PROTOCOL (This);

  DEBUG ((DEBUG_INFO, "%a: Entry () \n", __FUNCTION__));
  //
  // Sanity checks before transfer
  //
  Status = HidSanityCheck (QuickI2cDev, HidPacket->ReportBufferLen);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cGetHidReportDescriptor: HidSanityCheck returned Status- %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cGetHidReportDescriptor: HidSanityCheck ERROR Status %r\n", Status));
    return Status;
  }

  HidPacket->ReportBuffer = AllocateZeroPool (QuickI2cDev->HidInputReportSize);
  if (HidPacket->ReportBuffer == NULL) {
    THC_LOCAL_DEBUG (L"QuickI2cGetHidReportDescriptor: returned Status- %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cGetHidReportDescriptor: ERROR Status %r\n", Status));
    return EFI_BUFFER_TOO_SMALL;
  }

  CopyMem (HidPacket->ReportBuffer, QuickI2cDev->HidInputReportBuffer, QuickI2cDev->HidInputReportSize);
  HidPacket->ReportBufferLen = QuickI2cDev->HidInputReportSize;

  return EFI_SUCCESS;
}

/**
  Enables end point device.
  - Reads all Touch Panels devices registers
  - Sets TouchEnable
  @param[in]  QuickI2cDev      Context of QuickI2c device
  @retval EFI_SUCCESS          Enabling completed
  @retval EFI_DEVICE_ERROR     TSSDONE not set or ERR set
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
EFIAPI
QuickI2cHidEnableAfterReset (
  IN THC_PROTOCOL  *This
  )
{
  QUICK_I2C_DEV       *QuickI2cDev;
  EFI_STATUS          Status;

  QuickI2cDev = QUICK_I2C_CONTEXT_FROM_THC_PROTOCOL (This);

  Status = QuickI2cEnableAfterReset (QuickI2cDev);

  return Status;
}