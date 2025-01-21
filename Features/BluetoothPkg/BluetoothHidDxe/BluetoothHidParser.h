/** @file
  The header file of Bluetooth HID Parser.

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
#ifndef _BLUETOOTH_HID_PAESER_H_
#define _BLUETOOTH_HID_PAESER_H_

#include "BluetoothHidDxe.h"

#define BT_HID_REPORT_MAP_LEN 512

/**
  Report Map Parser Function for the Report Mde Operation of Bluetooth HID devices

  @param[in]  BtHidDev Pointer to Bluetooth HID Device Structure.

**/
VOID
BtHidParseReportMap (
  IN  BT_HID_DEV                  *BtHidDev
  );

/**
  Fetches the Head of the Link List having the report format structures
  @param[in]  This        A pointer to the BT_HID_DEV_FROM_HID_PROTOCOL
                          instance.
  @param[out] Head Pointer to Report format linked list.

  @retval  EFI_SUCCESS  The function completes successfully.

**/
EFI_STATUS
EFIAPI
BtHidGetReportFormatList (
  IN EDKII_HID_PROTOCOL *This,
  OUT LIST_ENTRY        **Head
  );

#endif

