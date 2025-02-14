/** @file
  Touch Host Controller Protocol for Thc feature

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#ifndef _THC_PROTOCOL_H_
#define _THC_PROTOCOL_H_

typedef struct _THC_PROTOCOL THC_PROTOCOL;

#define THC_PROTOCOL_REVISION 2

/**
  Performs GetFeature function as described in Human Interface Device specification.

  @param[in]        This       Pointer to instance of protocol.
  @param[in]        Length     Size of the buffer.
  @param[in] [Out]  Buffer     On input, contains ReportId in 1st byte. On output, filled with Feature data from external device.
                               The caller is responsible for freeing the buffer that is allocated by callee for Buffer.
  @param[in]        Timeout    0 - No timeout, do not wait for response
                         1 or higher - will wait for that amount of time and copy response results to the same buffer.

  @retval EFI_NOT_READY         THC is not ready
  @retval EFI_ALREADY_STARTED   HID transaction is still active
  @retval EFI_NOT_AVAILABLE_YET THC read transaction is ongoing
  @retval EFI_TIMEOUT           a) Response did not come in time OR
                                b) DMA transaction did not finish in time
  @retval EFI_BUFFER_TOO_SMALL  THC DMA buffer is unable to fit that much data
  @retval EFI_SUCCESS           Get feature completed
**/
typedef
EFI_STATUS
(EFIAPI *HID_GET_FEATURE) (
  IN THC_PROTOCOL   *This,
  IN UINT32         Length,
  IN OUT UINT8      *Buffer,
  IN UINTN          Timeout
  );

/**
  Performs SetFeature function as described in Human Interface Device specification.
  @param[in]        This       Pointer to instance of protocol.
  @param[in]        Length     Size of the buffer.
  @param[in] [Out]  Buffer     On input, contains data to be sent to host.
  @param[in]        Timeout    0 - No timeout, do not wait for response
                         1 or higher - will wait for that amount of time and copy response results to the same buffer.

  @retval EFI_NOT_READY         THC is not ready
  @retval EFI_ALREADY_STARTED   HID transaction is still active
  @retval EFI_NOT_AVAILABLE_YET THC read transaction is ongoing
  @retval EFI_TIMEOUT           a) Response did not come in time OR
                                b) DMA transaction did not finish in time
  @retval EFI_BUFFER_TOO_SMALL  THC DMA buffer is unable to fit that much data
  @retval EFI_SUCCESS           Set feature completed
**/
typedef
EFI_STATUS
(EFIAPI *HID_SET_FEATURE) (
  IN THC_PROTOCOL   *This,
  IN UINT32         Length,
  IN OUT UINT8      *Buffer,
  IN UINTN          Timeout
  );

/**
  Performs Get input report function as described in Human Interface Device specification.
  @param[in]        This       Pointer to instance of protocol.
  @param[in]        Length     Size of the buffer.
  @param[in] [Out]  Buffer     On input, contains ReportId in 1st byte. On output, filled with Feature data from external device.
                               The caller is responsible for freeing the buffer that is allocated by callee for Buffer.
  @param[in]        Timeout    0 - No timeout, do not wait for response
                               1 or higher - will wait for that amount of time and copy response results to the same buffer.
  @retval EFI_NOT_READY         THC is not ready
  @retval EFI_ALREADY_STARTED   HID transaction is still active
  @retval EFI_NOT_AVAILABLE_YET THC read transaction is ongoing
  @retval EFI_TIMEOUT           a) Response did not come in time OR
                                b) DMA transaction did not finish in time
  @retval EFI_BUFFER_TOO_SMALL  THC DMA buffer is unable to fit that much data
  @retval EFI_SUCCESS           Set feature completed
**/
typedef
EFI_STATUS
(EFIAPI *HID_GET_INPUT_REPORT) (
  IN THC_PROTOCOL   *This,
  IN UINT32         Length,
  IN OUT UINT8      *Buffer,
  IN UINTN          Timeout
  );


/**
  Performs SetOutput report function as described in Human Interface Device specification.
  @param[in]        This       Pointer to instance of protocol.
  @param[in]        Length     Size of the buffer.
  @param[in] [Out]  Buffer     On input, contains data to be sent to host.
  @param[in]        Timeout    0 - No timeout, do not wait for response
                               1 or higher - will wait for that amount of time and copy response results to the same buffer.
  @retval EFI_NOT_READY         THC is not ready
  @retval EFI_ALREADY_STARTED   HID transaction is still active
  @retval EFI_NOT_AVAILABLE_YET THC read transaction is ongoing
  @retval EFI_TIMEOUT           a) Response did not come in time OR
                                b) DMA transaction did not finish in time
  @retval EFI_BUFFER_TOO_SMALL  THC DMA buffer is unable to fit that much data
  @retval EFI_SUCCESS           Set feature completed
**/
typedef
EFI_STATUS
(EFIAPI *HID_SET_OUTPUT_REPORT) (
  IN THC_PROTOCOL   *This,
  IN UINT32         Length,
  IN OUT UINT8      *Buffer,
  IN UINTN          Timeout
  );

/**
  Provide device descriptor to caller.
  @param[in]      This       Pointer to instance of protocol.
  @param[in]      Length     Size of the buffer.
  @param[in][out] Buffer     On output, filled with descriptor data for external device.
                             The caller is responsible for freeing the buffer that is allocated by callee for Buffer.
  @param[in]      Timeout    0 - No timeout, do not wait for response
                             1 or higher - will wait for that amount of time and copy response results to the same buffer.
  @retval EFI_NOT_READY         THC is not ready
  @retval EFI_ALREADY_STARTED   HID transaction is still active
  @retval EFI_NOT_AVAILABLE_YET THC read transaction is ongoing
  @retval EFI_TIMEOUT           a) Response did not come in time OR
                                b) DMA transaction did not finish in time
  @retval EFI_BUFFER_TOO_SMALL  THC DMA buffer is unable to fit that much data
  @retval EFI_SUCCESS           Set feature completed
**/
typedef
EFI_STATUS
(EFIAPI *HID_GET_DEVICE_DESCRIPTOR) (
  IN THC_PROTOCOL   *This,
  IN UINT32         Length,
  IN OUT UINT8      *Buffer,
  IN UINTN          Timeout
  );

/**
  Performs SetPowerState function as described in Human Interface Device specification.
  @param[in]        This       Pointer to instance of protocol.
  @param[in]        Length     Size of the buffer.
  @param[in] [Out]  Buffer     On input, contains data to be sent to host.
  @param[in]        PowerState Power State.
  @retval EFI_NOT_READY         THC is not ready
  @retval EFI_ALREADY_STARTED   HID transaction is still active
  @retval EFI_NOT_AVAILABLE_YET THC read transaction is ongoing
  @retval EFI_TIMEOUT           a) Response did not come in time OR
                                b) DMA transaction did not finish in time
  @retval EFI_BUFFER_TOO_SMALL  THC DMA buffer is unable to fit that much data
  @retval EFI_SUCCESS           Set feature completed
**/
typedef
EFI_STATUS
(EFIAPI *HID_SET_POWER_STATE) (
  IN THC_PROTOCOL   *This,
  IN UINT32         Length,
  IN OUT UINT8      *Buffer,
  IN UINT8          PowerState
  );

/**
  Performs perform reset function as described in Human Interface Device specification.
  @param[in]        This       Pointer to instance of protocol.
  @param[in]        Length     Size of the buffer.
  @param[in][Out]   Buffer     On input, contains data to be sent to host.
  @param[in]        Timeout    0 - No timeout, do not wait for response
                         1 or higher - will wait for that amount of time and copy response results to the same buffer.
  @retval EFI_NOT_READY         THC is not ready
  @retval EFI_ALREADY_STARTED   HID transaction is still active
  @retval EFI_NOT_AVAILABLE_YET THC read transaction is ongoing
  @retval EFI_TIMEOUT           a) Response did not come in time OR
                                b) DMA transaction did not finish in time
  @retval EFI_BUFFER_TOO_SMALL  THC DMA buffer is unable to fit that much data
  @retval EFI_SUCCESS           Set feature completed
**/
typedef
EFI_STATUS
(EFIAPI *HID_PERFORM_RESET) (
  IN THC_PROTOCOL   *This,
  IN UINT32         Length,
  IN OUT UINT8      *Buffer,
  IN UINTN          Timeout
  );

/**
  Provides Input HID report descriptor to caller.
  @param[in]      This       Pointer to instance of protocol.
  @param[in]      Length     Size of the buffer.
  @param[in][out] Buffer     On output, filled with descriptor data for external device.
                             The caller is responsible for freeing the buffer that is allocated by callee for Buffer.
  @param[in]      Timeout    0 - No timeout, do not wait for response
                             1 or higher - will wait for that amount of time and copy response results to the same buffer.
	@@ -70,11 +229,11 @@ EFI_STATUS
**/
typedef
EFI_STATUS
(EFIAPI *HID_GET_REPORT_DESCRIPTOR) (
  IN THC_PROTOCOL   *This,
  IN UINT32         Length,
  IN OUT UINT8      *Buffer,
  IN UINTN          Timeout
  );

/**
  Enables end point device.
  - Reads all Touch Panels devices registers
  - Sets TouchEnable

  @param[in]  This       Pointer to instance of protocol.

  @retval EFI_SUCCESS          Enabling completed
  @retval EFI_DEVICE_ERROR     TSSDONE not set or ERR set
  @retval EFI_TIMEOUT          Timeout
**/
typedef
EFI_STATUS
(EFIAPI *HID_ENABLE_AFTER_RESET) (
  IN THC_PROTOCOL  *This
  );

/**
  Protocol used by Thc
  Once installed allows to detect if it is already running on given device
  Also used for obtaining Thc context

 @note
  Designed to be backward compatible with previous versions.

**/
struct _THC_PROTOCOL {
  UINT8                      Revision;
  /**
    Devices Status:
      EFI_NOT_READY    - Thc initialization is still in progress
      EFI_DEVICE_ERROR - Thc Initialization failed
      EFI_SUCCESS      - Thc is functional
  **/
  EFI_STATUS                  DeviceStatus;
  HID_GET_FEATURE             HidGetFeature;
  HID_SET_FEATURE             HidSetFeature;
  HID_ENABLE_AFTER_RESET      HidEnableAfterReset;
  HID_GET_INPUT_REPORT        HidGetInputReport;
  HID_SET_OUTPUT_REPORT       HidSetOutputReport;
  HID_GET_DEVICE_DESCRIPTOR   HidGetDeviceDescriptor;
  HID_SET_POWER_STATE         HidSetPowerState;
  HID_PERFORM_RESET           HidPerformReset;
  HID_GET_REPORT_DESCRIPTOR   HidGetReportDescriptor;
};

extern EFI_GUID gThcProtocolGuid;

#endif // _THC_PROTOCOL_H_
