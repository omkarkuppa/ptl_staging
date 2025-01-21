/** @file
  The public header file of EDKII_HID_PROTOCOL protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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

#ifndef _HID_H_
#define _HID_H_

// HID definition in USB.h
//#include <IndustryStandard/Usb.h>

#undef UEFI_COMBO_DEVICE_SUPPORTED
#define UEFI_COMBO_DEVICE_SUPPORTED

#define EDKII_HID_PROTOCOL_GUID \
  { 0xc9520968, 0x20df, 0x40a9, 0x94, 0xf3, 0x58, 0x5b, 0x3f, 0x18, 0xa9, 0x9 }

typedef struct _EDKII_HID_PROTOCOL EDKII_HID_PROTOCOL;

// Use as Bit values if UEFI_COMBO_DEVICE_SUPPORTED is defined
// Todo: in future in case of enhancement of new entries please use as
// 0x04, 0x08, 0x10, 0x20, 0x40, 0x80.
typedef enum {
  HidTypeMax = 0x00,
  HidTypeMouse = 0x01,
  HidTypeKeyBoard = 0x02
} HID_DEVICE_TYPE;

typedef
EFI_STATUS
(EFIAPI *EDKII_HID_ASYNC_FUNC_CALLBACK) (
  IN VOID                           *Data,
  IN UINTN                          DataLength,
  IN VOID                           *Context
  );

typedef struct {
  HID_DEVICE_TYPE   HidType;
  // TBD
} EDKII_HID_INFO;

typedef
EFI_STATUS
(EFIAPI *EDKII_HID_GET_DEVICE_INFO)(
  IN EDKII_HID_PROTOCOL      *This,
  OUT EDKII_HID_INFO         *HidInfo
  );

typedef
EFI_STATUS
(EFIAPI *EDKII_HID_SERVICE_CALLBACK) (
  IN VOID                           *Data,
  IN UINTN                          DataLength,
  IN VOID                           *Context
  );

typedef
EFI_STATUS
(EFIAPI *EDKII_HID_START)(
  IN EDKII_HID_PROTOCOL           *This,
  IN EDKII_HID_SERVICE_CALLBACK   HidControlCallback,
  IN VOID                         *HidControlCallbackContext,
  IN EDKII_HID_SERVICE_CALLBACK   HidInterruptCallback,
#ifdef UEFI_COMBO_DEVICE_SUPPORTED
  IN VOID                         *HidInterrupCallbackContext,
  IN EDKII_HID_INFO               HidInfo
#else
  IN VOID                         *HidInterrupCallbackContext
#endif
  );

typedef
EFI_STATUS
(EFIAPI *EDKII_HID_STOP)(
  IN EDKII_HID_PROTOCOL              *This
  );

typedef enum {
  EdkiiHidRequestGetReport = 0x01,
  EdkiiHidRequestGetIdle = 0x02,
  EdkiiHidRequestGetReportMap = 0x04,
  EdkiiHidRequestGetReportReferDesc = 0x05,
  EdkiiHidRequestSetReport = 0x09,
  EdkiiHidRequestSetIdle = 0x0A,
} EDKII_HID_REQUEST_TYPE;

typedef
EFI_STATUS
(EFIAPI *EDKII_HID_SEND_REQUEST)(
  IN EDKII_HID_PROTOCOL      *This,
  IN EDKII_HID_REQUEST_TYPE  Request,
  IN UINT16                Value,
  IN UINT16                Length,
  IN OUT VOID              *Data
  );

typedef
EFI_STATUS
(EFIAPI *EDKII_HID_SET_NOTIFICATION)(
  IN EDKII_HID_PROTOCOL      *This,
  IN UINT8                 Value
  );

#define ITEM_SIGNATURE SIGNATURE_32 ('b', 't', 'r', 'i')
#define MAX_KEYCODE_BUFF_SIZE 10

#define BT_HID_ITEM_TYPE_VARIABLE 1
#define BT_HID_ITEM_TYPE_ARRAY 0
#define BT_HID_ITEM_TYPE_INPUT 0x8
#define BT_HID_KEYBOARD_USAGE_PAGE 0x7
#define BT_HID_KEYBOARD_USAGE 0x6
#define BT_HID_LED_USAGE_PAGE 0x08
#define BT_HID_POINTER_USAGE 0x1
#define BT_HID_BUTTON_USAGE_PAGE 0x9
#define BT_HID_MOUSE_USAGE 0x2

typedef struct item{
  UINTN Signature;
  UINT8 DatConst      :1;
  UINT8 ArrVar        :1;
  UINT8 AbsRel        :1;
  UINT8 Wrap          :1;
  UINT8 Linear        :1;
  UINT8 Preferred     :1;
  UINT8 NullPosState  :1;
  UINT8 Reserved      :1;
  UINT32 Usage;
  UINT32 UsagePage;
  UINT32 UsageMin;
  UINT32 UsageMax;
  UINT32 LogicalMin;
  UINT32 LogicalMax;
  UINT32 ReportCount;
  UINT32 ReportSize;
  UINT8  ReportId;
  UINT8  Type;
  LIST_ENTRY Link;
} HID_REPORT_FMT;

#define ITEM_FROM_LINK(a) \
    CR(a, HID_REPORT_FMT, Link, ITEM_SIGNATURE)

typedef
EFI_STATUS
(EFIAPI *EDKII_HID_GET_ITEM_LIST)(
  IN EDKII_HID_PROTOCOL    *This,
  OUT LIST_ENTRY           **Head
  );

typedef struct _EDKII_HID_PROTOCOL {
  EDKII_HID_GET_DEVICE_INFO          GetDeviceInfo;

  //
  // Start/Stop
  //
  EDKII_HID_START                    Start;
  EDKII_HID_STOP                     Stop;

  //
  // Send
  //
  EDKII_HID_SEND_REQUEST             SendRequest;
  EDKII_HID_SET_NOTIFICATION         SetNotification;
  EDKII_HID_GET_ITEM_LIST            GetReportFormatList;
} EDKII_HID_PROTOCOL;

extern EFI_GUID gEfiHidProtocolGuid;

#endif

