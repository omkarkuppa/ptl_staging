/**@file
    Unit test main of the I2c Touch Panel Dxe driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#ifndef _UNIT_TEST_MAIN_H_
#define _UNIT_TEST_MAIN_H_
//
// NewRequestPacket Unit Test.
//
#include <TestCase/NewRequestPacket/TestcaseNewRequestPacket.h>
//
// DeleteRequestPacket Unit Test.
//
#include <TestCase/DeleteRequestPacket/TestcaseDeleteRequestPacket.h>
//
// TouchPanelDriverAbsolutePointerGetState Unit Test.
//
#include <TestCase/TouchPanelDriverAbsolutePointerGetState/TestCaseTouchPanelDriverAbsolutePointerGetState.h>
//
// TouchPanelDriverUnload Unit Test.
//
#include <TestCase/TouchPanelDriverUnload/TestCaseTouchPanelDriverUnload.h>
//
// TouchPanelDriverEntryPoint Unit Test.
//
#include <TestCase/TouchPanelDriverEntryPoint/TestCaseTouchPanelDriverEntryPoint.h>
//
// TouchPanelDriverDriverBindingSupported Unit Test.
//
#include <TestCase/TouchPanelDriverDriverBindingSupported/TestCaseTouchPanelDriverDriverBindingSupported.h>
//
// TouchPanelDriverDriverBindingStart Unit Test.
//
#include <TestCase/TouchPanelDriverDriverBindingStart/TestCaseTouchPanelDriverDriverBindingStart.h>
//
// TouchPanelDriverDriverBindingStop Unit Test.
//
#include <TestCase/TouchPanelDriverDriverBindingStop/TestCaseTouchPanelDriverDriverBindingStop.h>
//
// TouchPanelDriverAbsolutePointerReset Unit Test.
//
#include <TestCase/TouchPanelDriverAbsolutePointerReset/TestCaseTouchPanelDriverAbsolutePointerReset.h>
//
// GetLength Unit Test.
//
#include <TestCase/GetLength/TestCaseGetLength.h>
//
// HidDescriptorReceivedEvent Unit Test.
//
#include <TestCase/HidDescriptorReceivedEvent/TestCaseHidDescriptorReceivedEvent.h>
//
// GetHidDescriptor Unit Test.
//
#include <TestCase/GetHidDescriptor/TestCaseGetHidDescriptor.h>
//
// SetHidPower Unit Test.
//
#include <TestCase/SetHidPower/TestCaseSetHidPower.h>
//
// SetHidReset Unit Test.
//
#include <TestCase/SetHidReset/TestCaseSetHidReset.h>
//
// GetTouchCoordinates Unit Test.
//
#include <TestCase/GetTouchCoordinates/TestCaseGetTouchCoordinates.h>
//
// CheckDataAvailableCallback Unit Test.
//
#include <TestCase/CheckDataAvailableCallback/TestCaseCheckDataAvailableCallback.h>
//
// TouchDevInit Unit Test.
//
#include <TestCase/TouchDevInit/TestCaseTouchDevInit.h>
//
// GetToken Unit Test.
//
#include <TestCase/GetToken/TestCaseGetToken.h>
//
// ExportReport Unit Test.
//
#include <TestCase/ExportReport/TestCaseExportReport.h>
//
// UpdateStack Unit Test.
//
#include <TestCase/UpdateStack/TestCaseUpdateStack.h>
//
// AccessBit Unit Test.
//
#include <TestCase/AccessBit/TestCaseAccessBit.h>
//
// ShowReportTable Unit Test.
//
#include <TestCase/ShowReportTable/TestCaseShowReportTable.h>
//
// SetInterruptFrequency Unit Test.
//
#include <TestCase/SetInterruptFrequency/TestCaseSetInterruptFrequency.h>
//
// ReadInputReportDescriptor Unit Test.
//
#include <TestCase/ReadInputReportDescriptor/TestCaseReadInputReportDescriptor.h>
//
// ParseInput Unit Test.
//
#include <TestCase/ParseInput/TestCaseParseInput.h>
//
// TouchPanelDriverComponentNameGetDriverName Unit Test.
//
#include <TestCase/TouchPanelDriverComponentNameGetDriverName/TestCaseTouchPanelDriverComponentNameGetDriverName.h>
//
// TouchPanelDriverComponentNameGetControllerName Unit Test.
//
#include <TestCase/TPLDriverComponentNameGetControllerName/TestCaseTouchPanelDriverComponentNameGetControllerName.h>

#endif
