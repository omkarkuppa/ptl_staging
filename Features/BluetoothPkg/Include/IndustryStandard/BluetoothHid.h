/** @file
  Definitions for Bluetooth HID layer.

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

#ifndef __BLUETOOTH_HID_H__
#define __BLUETOOTH_HID_H__

#pragma pack(1)

//
// Human Interface Device
//

typedef enum {
  BluetoothHidTransactionHandshake = 0x0,
  BluetoothHidTransactionHidControl = 0x1,
  BluetoothHidTransactionGetReport = 0x4,
  BluetoothHidTransactionSetReport = 0x5,
  BluetoothHidTransactionGetProtocol = 0x6,
  BluetoothHidTransactionSetProtocol = 0x7,
  BluetoothHidTransactionGetIdle = 0x8,
  BluetoothHidTransactionSetIdle = 0x9,
  BluetoothHidTransactionData = 0xA,
  BluetoothHidTransactionDataContinue = 0xB,
} BLUETOOTH_HID_TRANSACTION_TYPE;

typedef enum {
  BluetoothHidBootProtocol = 0x0,
  BluetoothHidReportProtocol = 0x1,
} BLUETOOTH_HID_PROTOCOL;

typedef enum {
  BluetoothHidInputReport = 0x1,
  BluetoothHidOutputReport = 0x2,
  BluetoothHidFeatureReport = 0x3,
} BLUETOOTH_HID_REPORT_TYPE;

typedef enum {
  BluetoothHidReportIdKeyboard = 0x1,
  BluetoothHidReportIdMouse = 0x2,
} BLUETOOTH_HID_REPORT_ID;

typedef struct {
  UINT8    TransactionType:4;
  UINT8    Parameter:4;
} BLUETOOTH_HID_TRANSACTION_HEADER;

typedef struct {
  BLUETOOTH_HID_TRANSACTION_HEADER  Header;
  UINT8                             ReportId;
  UINT16                            BufferSize;
} BLUETOOTH_HID_TRANSACTION_GET_REPORT;

typedef struct {
  BLUETOOTH_HID_TRANSACTION_HEADER  Header;
  UINT8                             ReportId;
  UINT16                            BufferSize;
//UINT8                             Buffer[];
} BLUETOOTH_HID_TRANSACTION_SET_REPORT;

typedef struct {
  BLUETOOTH_HID_TRANSACTION_HEADER  Header;
} BLUETOOTH_HID_TRANSACTION_GET_PROTOCOL;

typedef struct {
  BLUETOOTH_HID_TRANSACTION_HEADER  Header;
} BLUETOOTH_HID_TRANSACTION_SET_PROTOCOL;

#pragma pack()

#endif

