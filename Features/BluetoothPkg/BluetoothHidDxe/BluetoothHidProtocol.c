/** @file
  This file implements the interfaces defined in EDKII_HID_PROTOCOL.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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

#include "BluetoothHidDxe.h"

typedef struct {
  VOID            *Command;
  UINTN           CommandSize;
  UINT8           StopResponseCode;
  VOID            *Payload;
  UINTN           PayloadSize;
} BLUETOOTH_L2CAP_COMMAND_STRUCTURE;

UINT8  gHid1[] = {0xa2, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
UINT8  gHid2[] = {0xa2, 0x02, 0x00, 0x00, 0x00, 0x00};
UINT8  gHid3[] = {0xa2, 0x03, 0x00};
UINT8  gHid4[] = {0xa2, 0x04, 0x00};
UINT8  gHid5[] = {0xa2, 0x05, 0x00};
UINT8  gHid6[] = {0xa2, 0xff, 0x00};
UINT8  gHidC7[] = {0x41, 0x03};
UINT8  gHidC8[] = {0x41, 0x03};

UINT8  mHidr1[] = {0x41, 0x03};

BLUETOOTH_L2CAP_COMMAND_STRUCTURE   mCommandHidInit3[] = {
  {&gHid1,  sizeof(gHid1),},
  {&gHid2,  sizeof(gHid2),},
  {&gHid3,  sizeof(gHid3),},
  {&gHid4,  sizeof(gHid4),},
  {&gHid5,  sizeof(gHid5),},
  {&gHid6,  sizeof(gHid6),},
};

BLUETOOTH_L2CAP_COMMAND_STRUCTURE   mCommandHidInit4[] = {
  {&gHidC7,  sizeof(gHidC7),},
  {&gHidC8,  sizeof(gHidC8),},
};

STATIC volatile BOOLEAN RequestOngoing = FALSE;
STATIC VOID *ResponseData = NULL;
STATIC UINTN ResponseDataLength= 0;

/**
  Retrieves the information of a Bluetooth human interface device.

  @param[in]   This     A pointer to the EDKII_HID_PROTOCOL instance.
  @param[out]  HidInfo  A pointer to the EDKII_HID_INFO structure.

  @retval  EFI_SUCCESS  The information of a Bluetooth human interface device
                        has been retrieved.

**/
EFI_STATUS
EFIAPI
BtHidGetDeviceInfo (
  IN  EDKII_HID_PROTOCOL    *This,
  OUT EDKII_HID_INFO        *HidInfo
  )
{
  BT_HID_DEV       *BtHidDev;
  LIST_ENTRY       *Link;
  HID_REPORT_FMT   *ReportFormatList;

  BtHidDev = BT_HID_DEV_FROM_HID_PROTOCOL (This);

  Link  = GetFirstNode (&BtHidDev->ReportFormatList);
  while (!IsNull (&BtHidDev->ReportFormatList, Link)) {
    ReportFormatList = ITEM_FROM_LINK(Link);

    if ( ReportFormatList->UsagePage == BT_HID_BUTTON_USAGE_PAGE && ReportFormatList->Usage == BT_HID_POINTER_USAGE ) {
#ifdef UEFI_COMBO_DEVICE_SUPPORTED
      HidInfo->HidType |= HidTypeMouse;
#else
      HidInfo->HidType = HidTypeMouse;
      break;
#endif
    } else if ( ReportFormatList->UsagePage == BT_HID_KEYBOARD_USAGE_PAGE && ReportFormatList->Usage == BT_HID_KEYBOARD_USAGE ) {
#ifdef UEFI_COMBO_DEVICE_SUPPORTED
      HidInfo->HidType |= HidTypeKeyBoard;
#else
      HidInfo->HidType = HidTypeKeyBoard;
#endif
    }

    Link = GetNextNode (&ReportFormatList->Link, Link);
  }

  return EFI_SUCCESS;
}

/**
  Starts the data transmission for a Bluetooth human interface device.

  @param[in]  This                        A pointer to the EDKII_HID_PROTOCOL
                                          interface.
  @param[in]  HidControlCallback          The callback function for data
                                          transferred via control transfer.
  @param[in]  HidControlCallbackContext   Data passed into the control transfer
                                          callback function.
  @param[in]  HidInterruptCallback        The callback function for data
                                          transferred via interrupt transfer.
  @param[in]  HidInterrupCallbackContext  Data passed into the interrupt
                                          transfer callback function.
  @param[in]  HidInfo                     EDKII_HID_INFO structure to inform
                                          interrupt is for HID Mouse or Keyboard.
                                          Applcable only if UEFI_COMBO_DEVICE_SUPPORTED
                                          is defined.

  @retval  EFI_SUCCESS  The operation succeeds.

**/
EFI_STATUS
EFIAPI
BtHidStart (
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
  )
{
  BT_HID_DEV                      *BtHidDev;

  BtHidDev    = BT_HID_DEV_FROM_HID_PROTOCOL (This);

#ifdef UEFI_COMBO_DEVICE_SUPPORTED
  if (HidInfo.HidType & HidTypeMouse) {
    BtHidDev->HidMouseInterruptCallback        = HidInterruptCallback;
    BtHidDev->HidMouseInterruptCallbackContext = HidInterrupCallbackContext;
  } else if (HidInfo.HidType & HidTypeKeyBoard) {
    BtHidDev->HidKeyboardInterruptCallback        = HidInterruptCallback;
    BtHidDev->HidKeyboardInterruptCallbackContext = HidInterrupCallbackContext;
  }
#else
  BtHidDev->HidInterruptCallback        = HidInterruptCallback;
  BtHidDev->HidInterruptCallbackContext = HidInterrupCallbackContext;
#endif

  return EFI_SUCCESS;
}

/**
  Stops the data transmission for a Bluetooth human interface device.

  @param[in]  This  A pointer to the EDKII_HID_PROTOCOL instance.

  @retval  EFI_UNSUPPORTED  The operation is not supported.

**/
EFI_STATUS
EFIAPI
BtHidStop (
  IN EDKII_HID_PROTOCOL            *This
  )
{
  BT_HID_DEV                       *BtHidDev;

  BtHidDev = BT_HID_DEV_FROM_HID_PROTOCOL (This);

#ifdef UEFI_COMBO_DEVICE_SUPPORTED
  BtHidDev->HidMouseInterruptCallback        = NULL;
  BtHidDev->HidMouseInterruptCallbackContext = NULL;

  BtHidDev->HidKeyboardInterruptCallback        = NULL;
  BtHidDev->HidKeyboardInterruptCallbackContext = NULL;
#else
  BtHidDev->HidInterruptCallback = NULL;
  BtHidDev->HidInterruptCallbackContext = NULL;
#endif

  return EFI_UNSUPPORTED;
}

/**
  The callback function to a read attribute request.

  @param[in]  This        A pointer to the EFI_BLUETOOTH_ATTRIBUTE_PROTOCOL
                          instance.
  @param[in]  Data        Data received. The first byte is the attribute
                          opcode, followed by opcode specific fields.
                          See Bluetooth specification, Vol 3, Part F, Attribute
                          Protocol. It might be a normal RESPONSE message, or
                          ERROR RESPONSE message.
  @param[in]  DataLength  The length of Data in bytes.
  @param[in]  Context     The context passed from the callback registration
                          request.

  @retval  EFI_SUCCESS  The callback function completes successfully.

**/
EFI_STATUS
EFIAPI
ReadCallback (
  IN EFI_BLUETOOTH_ATTRIBUTE_PROTOCOL *This,
  IN VOID                             *Data,
  IN UINTN                            DataLength,
  IN VOID                             *Context
  )
{
  UINT8  *Buffer;
  UINT8  OpCode;
  UINT16 Handle;
  UINTN  CopyLength;

  Buffer = Data;
  OpCode = *Buffer;
  Handle = Context != NULL ? *((UINT16*)Context) : 0;

  DEBUG ((EFI_D_INFO, "Got opcode 0x%02x for handle 0x%04x\n", OpCode, Handle));

  Buffer++;
  DataLength--;
  CopyLength = MIN(DataLength, ResponseDataLength);

  switch (OpCode) {
    case BluetoothAttOpReadResponse:
      CopyMem (ResponseData, Buffer, CopyLength);
      break;
    case BluetoothAttOpReadByTypeResponse:
    case BluetoothAttOpReadMultipleResponse:
    case BluetoothAttOpErrorResponse:
    default:
      DEBUG ((EFI_D_ERROR, "Invalid opcode\n"));
  }

  RequestOngoing = FALSE;

  return EFI_SUCCESS;
}

/**
  Submit a request to a Bluetooth human interface device over the Bluetooth
  L2CAP layer.

  @param[in]      This     A pointer to the EFI_BLUETOOTH_ATTRIBUTE_PROTOCOL
                           instance.
  @param[in]      Request  A pointer to the EDKII_HID_REQUEST_TYPE structure that
                           specifies the type of the request.
  @param[in]      Value    Value to set for the Bluetooth human interface
                           device.
  @param[in]      Length   The length of Data in bytes.
  @param[in,out]  Data     A pointer to the buffer for data transmission.

  @retval  EFI_UNSUPPORTED       The type of request is not supported.
  @retval  EFI_OUT_OF_RESOURCES  The operation fails due to memory allocation
                                 failure.
  @retval  Other                 A value from other functions.

**/
EFI_STATUS
EFIAPI
BtHidSendRequest (
  IN EDKII_HID_PROTOCOL      *This,
  IN EDKII_HID_REQUEST_TYPE  Request,
  IN UINT16                  Value,
  IN UINT16                  Length,
  IN OUT VOID                *Data
  )
{
  BT_HID_DEV                             *BtHidDev;
  EFI_BLUETOOTH_ATTRIBUTE_PROTOCOL       *BluetoothAttribute;
  EFI_STATUS                             Status;
  LIST_ENTRY                             *Link;
  HID_REPORT_INFO                        *ReportInfo;
  HID_REPORT_FMT                         *ReportItem;
  UINT8                                  LedReportId;
#ifdef UEFI_COMBO_DEVICE_SUPPORTED
  UINT8                                  MouseReportId;
  UINT8                                  KeyboardReportId;
#endif
  UINT8                                  *Buffer;

  BtHidDev           = BT_HID_DEV_FROM_HID_PROTOCOL (This);
  BluetoothAttribute = BtHidDev->BluetoothAttribute;
  LedReportId        = 0;
#ifdef UEFI_COMBO_DEVICE_SUPPORTED
  MouseReportId      = 0;
  KeyboardReportId   = 0;
#endif

  //DEBUG ((EFI_D_INFO, "HidSendRequest!\n"));
  Status = EFI_SUCCESS;
  switch (Request) {
  case EdkiiHidRequestSetReport:
    if (BtHidDev->LEDReportHandle > 0) {
      Status = BluetoothAttributeWrite(BluetoothAttribute, BtHidDev->LEDReportHandle + 1, EfiBluetoothAttributeWriteCommand, Length, Data, NULL, NULL);
    }
    break;
  case EdkiiHidRequestGetReportMap:
    RequestOngoing = TRUE;
    ResponseData = Data;
    ResponseDataLength = Length;
    Status = BluetoothAttributeRead(BluetoothAttribute, BtHidDev->ReportMapHandle + 1, 0, 0, ReadCallback, &BtHidDev->ReportMapHandle);
    do {} while (RequestOngoing);
    break;
  case EdkiiHidRequestGetReportReferDesc:
    Link  = GetFirstNode (&BtHidDev->ReportFormatList);
    while (!IsNull (&BtHidDev->ReportFormatList, Link)) {
      ReportItem = ITEM_FROM_LINK(Link);

      if (ReportItem->UsagePage == BT_HID_LED_USAGE_PAGE) {
        LedReportId = ReportItem->ReportId;
        break;
      }

      Link  = GetNextNode (&BtHidDev->ReportFormatList, Link);
    }
#ifdef UEFI_COMBO_DEVICE_SUPPORTED
    // Searching for Mouse Report
    Link  = GetFirstNode (&BtHidDev->ReportFormatList);
    while (!IsNull (&BtHidDev->ReportFormatList, Link)) {
      ReportItem = ITEM_FROM_LINK(Link);

      if ((ReportItem->UsagePage == BT_HID_BUTTON_USAGE_PAGE) && (ReportItem->Usage == BT_HID_POINTER_USAGE)) {
        MouseReportId = ReportItem->ReportId;
        break;
      }

      Link  = GetNextNode (&BtHidDev->ReportFormatList, Link);
    }

    // Searching for Keyboard Report
    Link  = GetFirstNode (&BtHidDev->ReportFormatList);
    while (!IsNull (&BtHidDev->ReportFormatList, Link)) {
      ReportItem = ITEM_FROM_LINK(Link);

      if ((ReportItem->UsagePage == BT_HID_KEYBOARD_USAGE_PAGE) && (ReportItem->Usage == BT_HID_KEYBOARD_USAGE)) {
        KeyboardReportId = ReportItem->ReportId;
        break;
      }

      Link  = GetNextNode (&BtHidDev->ReportFormatList, Link);
    }
#endif
    Link  = GetFirstNode (&BtHidDev->HidReportInfo);
    while (!IsNull (&BtHidDev->HidReportInfo, Link)) {
      ReportInfo = BT_HID_REPORT_INFO_FROM_LINK(Link);
      RequestOngoing = TRUE;
      ResponseDataLength = 10;
      ResponseData = AllocateZeroPool(ResponseDataLength);
      if (ResponseData == NULL) {
        return EFI_OUT_OF_RESOURCES;
      }
      Status = BluetoothAttributeRead(BluetoothAttribute, ReportInfo->ReportReferDescHandle, 0, 0, ReadCallback, &ReportInfo->ReportHandle);
      do {} while (RequestOngoing);
      Buffer = ResponseData;
      ReportInfo->ReportID = Buffer[0];
      ReportInfo->ReportType = Buffer[1];

      // If the Output report is for setting the LEDs, set the handle correctly
      if ((ReportInfo->ReportType == HID_REPORT_TYPE_OUTPUT) && (ReportInfo->ReportID == LedReportId)) {
        BtHidDev->LEDReportHandle = ReportInfo->ReportHandle;
      }
#ifdef UEFI_COMBO_DEVICE_SUPPORTED
      // Setting Keyboard and/or Mouse Report Handle
      if ((ReportInfo->ReportType == HID_REPORT_TYPE_INPUT) && (ReportInfo->ReportID == MouseReportId)) {
        BtHidDev->MouseReportHandle = ReportInfo->ReportHandle;
      } else if ((ReportInfo->ReportType == HID_REPORT_TYPE_INPUT) && (ReportInfo->ReportID == KeyboardReportId)) {
        BtHidDev->KeyboardReportHandle = ReportInfo->ReportHandle;
      }
#endif
      FreePool(ResponseData);
      Link  = GetNextNode (&BtHidDev->HidReportInfo, Link);
    }
    break;
  case EdkiiHidRequestGetReport:
  case EdkiiHidRequestGetIdle:
  case EdkiiHidRequestSetIdle:
  default:
    return EFI_UNSUPPORTED;
  }

  return Status;
}


/**
  The callback function to the SetNotification request.

  @param[in]  This        A pointer to the EFI_BLUETOOTH_ATTRIBUTE_PROTOCOL
                          instance.
  @param[in]  Data        Data received. The first byte is the attribute
                          opcode, followed by opcode specific fields.
                          See Bluetooth specification, Vol 3, Part F, Attribute
                          Protocol. It might be a normal RESPONSE message, or
                          ERROR RESPONSE message.
  @param[in]  DataLength  The length of Data in bytes.
  @param[in]  Context     The context passed from the callback registration
                          request.

  @retval  EFI_SUCCESS            The callback function completes successfully.
  @retval  EFI_INVALID_PARAMETER  The input parameters are invalid.

**/
EFI_STATUS
EFIAPI
NotificationCallback (
  IN EFI_BLUETOOTH_ATTRIBUTE_PROTOCOL *This,
  IN VOID                             *Data,
  IN UINTN                            DataLength,
  IN VOID                             *Context
  )
{
  UINT8           *Buffer;
  UINT8           OpCode;
  BT_HID_DEV      *BtHidDev;
  LIST_ENTRY      *Link;
  HID_REPORT_INFO *ReportInfo;
  UINT16          ReportHandle;

  //
  // Within this function, the first 3 bytes of 'Data' will be explicitly
  // consumed. Byte 0 is the opcode, bytes 1-2 are used for report handle.
  //
  if ((Data == NULL) || (DataLength < 3)) {
    return EFI_INVALID_PARAMETER;
  }

  Buffer    = Data;
  OpCode    = *Buffer;
  BtHidDev  = (BT_HID_DEV *)Context;

  Buffer++;
  DataLength--;

  ReportHandle = *(UINT16 *)Buffer;

  switch (OpCode) {
    case BluetoothAttOpHandleValueNotification:

      /* Zero filling the byte since it will not be used further */
      Buffer[0] = 0;

      /* The buffer that we plan to pass to the callback needs
       * to have the report id appended before the buffer. We are using the lower byte of the opcode field to store this value.
       * Thus, Advancing the buffer pointer by 1 and decrementing the length by 1.
       */
      Buffer += 1;
      DataLength -= 1;
      Link  = GetFirstNode (&BtHidDev->HidReportInfo);
      // Populate the report ID correctly and proceed
      while (!IsNull (&BtHidDev->HidReportInfo, Link)) {
        ReportInfo = BT_HID_REPORT_INFO_FROM_LINK(Link);

        if ( ReportInfo->ReportHandle + 1 == ReportHandle ) {
          Buffer[0] = ReportInfo->ReportID;
          break;
        }

        Link  = GetNextNode (&BtHidDev->HidReportInfo, Link);
      }

#ifdef UEFI_COMBO_DEVICE_SUPPORTED
      if ((BtHidDev->MouseReportHandle + 1) == ReportHandle) {
        if (BtHidDev->HidMouseInterruptCallback) {
          BtHidDev->HidMouseInterruptCallback(Buffer, DataLength, BtHidDev->HidMouseInterruptCallbackContext);
        }
      } else if ((BtHidDev->KeyboardReportHandle + 1) == ReportHandle) {
        if (BtHidDev->HidKeyboardInterruptCallback) {
          BtHidDev->HidKeyboardInterruptCallback(Buffer, DataLength, BtHidDev->HidKeyboardInterruptCallbackContext);
        }
      } else {
        if (BtHidDev->HidMouseInterruptCallback) {
          BtHidDev->HidMouseInterruptCallback(Buffer, DataLength, BtHidDev->HidMouseInterruptCallbackContext);
        }
        if (BtHidDev->HidKeyboardInterruptCallback) {
          BtHidDev->HidKeyboardInterruptCallback(Buffer, DataLength, BtHidDev->HidKeyboardInterruptCallbackContext);
        }
      }
#else
      BtHidDev->HidInterruptCallback(Buffer, DataLength, BtHidDev->HidInterruptCallbackContext);
#endif
      break;
    case BluetoothAttOpHandleValueIndication:
      break;
  }

  return EFI_SUCCESS;
}

/**
  Registers or unregisters a server initiated message on a characteristic value
  on remote server.

  @param[in]  This   A pointer to the EDKII_HID_PROTOCOL instance.
  @param[in]  Value  Type of notification for server initiated attribute
                     protocol. A value of zero means unregister the server
                     initiated callback.

  @retval  EFI_UNSUPPORTED  The operation is not supported
  @retval  Other            A value from other functions.

**/
EFI_STATUS
EFIAPI
BtHidSetNotification (
  IN EDKII_HID_PROTOCOL    *This,
  IN UINT8                 Value
  )
{
  BT_HID_DEV                                 *BtHidDev;
  EFI_BLUETOOTH_ATTRIBUTE_PROTOCOL           *BluetoothAttribute;
  EFI_STATUS                                 Status;
  EFI_BLUETOOTH_ATTRIBUTE_CALLBACK_PARAMETER NotifyParam;
  LIST_ENTRY                                 *Link;
  HID_REPORT_INFO                            *ReportInfo;

  BtHidDev    = BT_HID_DEV_FROM_HID_PROTOCOL (This);
  BluetoothAttribute = BtHidDev->BluetoothAttribute;
  Status      = EFI_UNSUPPORTED;

  if (BluetoothAttribute == NULL) {
    return EFI_UNSUPPORTED;
  }

  //DEBUG ((EFI_D_INFO, "SetNotification!\n"));

  NotifyParam.AttributeOpCode = Value;

  Link  = GetFirstNode (&BtHidDev->HidReportInfo);
  while (!IsNull (&BtHidDev->HidReportInfo, Link)) {
    ReportInfo = BT_HID_REPORT_INFO_FROM_LINK(Link);
    // Register for all Input Reports
    if (ReportInfo->ReportType == HID_REPORT_TYPE_INPUT) {
      NotifyParam.Parameter.Notification.AttributeHandle = ReportInfo->ReportHandle;
      if (Value != 0) {
#ifdef UEFI_COMBO_DEVICE_SUPPORTED
        Status = EFI_SUCCESS;
        // Avoiding Enabling Notification if already enabled
        if (ReportInfo->IsNotificationEnabled == FALSE) {
          Status = BluetoothAttribute->RegisterForServerNotification(BluetoothAttribute, &NotifyParam, NotificationCallback, BtHidDev);
          ReportInfo->IsNotificationEnabled = TRUE;
        }
#else
        Status = BluetoothAttribute->RegisterForServerNotification(BluetoothAttribute, &NotifyParam, NotificationCallback, BtHidDev);
#endif
      } else {
        Status = BluetoothAttribute->RegisterForServerNotification(BluetoothAttribute, &NotifyParam, 0, 0);
#ifdef UEFI_COMBO_DEVICE_SUPPORTED
        ReportInfo->IsNotificationEnabled = FALSE;
#endif
      }
    }
    Link  = GetNextNode (&BtHidDev->HidReportInfo, Link);
  }

  DEBUG ((EFI_D_INFO, "RegisterForServerNotification - %r\n", Status));

  return Status;
}


