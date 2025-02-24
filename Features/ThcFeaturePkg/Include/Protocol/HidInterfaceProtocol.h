/** @file
  HID Protocol Interface

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

#ifndef HID_INTERFACE_PROTOCOL_H_
#define HID_INTERFACE_PROTOCOL_H_

#include <Uefi.h> // Ensure this header is included for standard types

extern EFI_GUID gHidProtocolGuid;
typedef struct _HID_INTERFACE_PROTOCOL HID_INTERFACE_PROTOCOL;

#define HID_INTERFACE_PROTOCOL_VERSION   1
#define MAX_HID_COLLECTION               256
#define ELAN_HID_SKIP_LENGTH             16
#define ELAN_HID_MAX_LENGTH              32
#define CUSTOM_REPORT_ID                 0xFF00
#define THQA_FEATURE_REPORT              0xC5
#define RELATIVE_COORDINATES_MAX_Y       1024
#define RELATIVE_COORDINATES_MAX_X       1024

#define USE_PAGE       0x01
#define DIGITIZER      0x0D
#define DESKTOP        0x01
#define USAGE          0x02
#define MOUSE          0x02
#define TOUCHPANEL     0x04
#define TOUCHPPAD      0x05
#define TIP_SWITCH     0x42
#define TOUCH_VALID    0x47
#define BUTTON         0x09
#define X_AXIS         0x30
#define Y_AXIS         0x31
#define REPORT_ID      0x21
#define LOG_MIN        0x05
#define LOG_MAX        0x09
#define REP_SIZE       0x1d
#define REP_COUNT      0x25
#define INPUT          0x20
#define OUTPUT         0x24
#define COLLECTION     0x28
#define APPLICATION    0x01
#define LOGICAL        0x02
#define END_COLLECTION 0x30

// Define flags for HID input descriptor
#define HID_FLAG_DATA               0x01
#define HID_FLAG_VARIABLE           0x02
#define HID_FLAG_RELATIVE           0x04
#define HID_FLAG_WRAP               0x08
#define HID_FLAG_LINEAR             0x10
#define HID_FLAG_PREFERRED_STATE    0x20
#define HID_FLAG_NO_NULL_POSITION   0x40

typedef enum {
  WacomHidProtocol      = 1,
  ElanHidProtocol       = 2,
} PANEL_HID_INTERFACE_PROTOCOL;

typedef enum {
  None          = 0,
  Relative      = 1,
  Absolute      = 2,
  BothRelAbs    = 3
} THC_HID_REL_ABS_SUPPORT;

typedef struct {
  UINT32 ID;
  UINT32 Value;
} HID_REPORT_DESCRIPTOR_TOKEN;

typedef struct {
  UINTN ReportId;
  UINTN MaxX;
  UINTN StartX;
  UINTN StopX;
  UINTN MaxY;
  UINTN StartY;
  UINTN StopY;
  UINTN Button;
} HID_REPORT_DESCRIPTOR;

typedef struct {
  UINT32 ID;
  UINT32 Value;
} HID_TOKEN;

typedef struct {
  UINT32 B;
  UINT32 X;
  UINT32 Y;
} HID_TOUCH_OUTPUT;

typedef struct {
  UINT8 B;
  INT32 X;
  INT32 Y;
} HID_REL_TOUCH_OUTPUT;

// Placeholder definitions for missing types
typedef struct {
  UINT8 FeatureReportAvailable;
  UINT8 ReportId;
} HID_GET_REPORT_FORMAT;

typedef struct {
  UINT32 MinX;
  UINT32 MaxX;
  UINT32 MinY;
  UINT32 MaxY;
} HID_XY_BOUNDARY;

// Dictionary for parsing incoming data from touchpanel
typedef struct {
  UINT32  LogMaxX;    // max and min values for X and Y axes
  UINT32  LogMinX;
  UINT32  LogMaxY;
  UINT32  LogMinY;
  UINT32  BitStartB;  // bit positions of Buttons state /X coordinates / Y coordinates in input stream
  UINT32  BitStopB;   // bit positions are calculated in pure input data (after cutting Length and reportID)
  UINT32  BitStartX;
  UINT32  BitStopX;
  UINT32  BitsInputX;
  UINT32  BitStartY;
  UINT32  BitStopY;
  UINT32  BitsInputY;
  UINT32  BitsTotal;  // total number of bits with declared purpose
  BOOLEAN ValidCollection;
} HID_INPUT_REPORT_COLLECTION;

// Dictionary for parsing incoming data from touchpanel
typedef struct {
  UINT32  Id;  // ReportID. 0 means panel doesn't send ReportIDs, and it also means there is only 1 format
  UINT32  CollectionCount;
  UINT32  UsageTouchPanel;
  UINT32  UsageTouchPad;
  UINT32  UsageMouse;
  HID_INPUT_REPORT_COLLECTION  Collection[MAX_HID_COLLECTION];
} HID_INPUT_REPORT_FORMAT;

typedef struct {
  UINT32  GlobalUsage;
  UINT32  Usages;
  UINT32  UsageB;
  UINT32  UsageX;
  UINT32  UsageY;
  UINT32  LogMax;
  UINT32  LogMin;
  UINT32  ReportSize;
  UINT32  ReportCount;
  UINT32  GlobalUsageOnStack;
  UINT32  ReportSizeOnStack;
  UINT32  ReportCountOnStack;
  UINT32  UsageTouchPanel;
  UINT32  UsageTouchPad;
  UINT32  UsageMouse;
  HID_INPUT_REPORT_FORMAT  TempReport;
} HID_PARSER_STACK;

// Set of dictionaries for parsing incoming data from touchpanel
typedef struct {
  UINT32                    Quantity;   // number of different report formats
  UINT32                    TouchPanel;
  UINT32                    TouchPad;
  UINT32                    Mouse;
  HID_INPUT_REPORT_FORMAT   *Report;    // pointer to array of dictionaries for report formats
} HID_INPUT_REPORT_TABLE;

/**
  Performs SetFeature function as described in Human Interface Device specification.

  @param[in]  This       Pointer to instance of protocol.
  @param[in]  Length     Size of buffer.
  @param[in]  Buffer     On input, contains ReportId in 1st byte. On output, filled with Feature data from external device.
  @param[in]  Timeout    0 - No timeout, do not wait for response
                         1 or higher - will wait for that amount of time and copy response results to the same buffer.

  @retval EFI_NOT_READY         Touch Interface is not ready
  @retval EFI_ALREADY_STARTED   HID transaction is still active
  @retval EFI_NOT_AVAILABLE_YET Touch Interface read transaction is ongoing
  @retval EFI_TIMEOUT           a) Response did not come in time OR
                                b) DMA transaction did not finish in time
  @retval EFI_BUFFER_TOO_SMALL  Touch Interface DMA buffer is unable to fit that much data
  @retval EFI_SUCCESS           Set feature completed
**/
typedef
EFI_STATUS
(EFIAPI *HID_PARSE_DESCRIPTOR) (
  IN HID_INTERFACE_PROTOCOL *This,
  IN HID_INPUT_REPORT_TABLE *InputReportTable,
  IN HID_GET_REPORT_FORMAT  *ReportPacket,
  IN UINT8                  *Descriptor,
  IN UINT32                 DescriptorLength
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
typedef
EFI_STATUS
(EFIAPI *HID_PARSE_INPUT) (
  IN HID_INTERFACE_PROTOCOL     *This,
  IN HID_INPUT_REPORT_TABLE     ReportTable,
  IN UINT8                      *InputStream,
  IN OUT HID_TOUCH_OUTPUT       *Output,
  IN OUT HID_REL_TOUCH_OUTPUT   *MouseTouchOutput,
  IN HID_XY_BOUNDARY            *MinMax,
  IN UINT8                      HidSolutionFlag,
  IN OUT UINT8                  *UsageDevice
);

/**
  Protocol used by Touch Interface
  Once installed allows to detect if it is already running on given device
  Also used for obtaining Touch Interface context

 @note
  Designed to be backward compatible with previous versions.

**/
struct _HID_INTERFACE_PROTOCOL {
  UINT8                 Version;
  HID_PARSE_DESCRIPTOR  ParseDescriptor;
  HID_PARSE_INPUT       ParseInput;
};

#endif // HID_INTERFACE_PROTOCOL_H_
