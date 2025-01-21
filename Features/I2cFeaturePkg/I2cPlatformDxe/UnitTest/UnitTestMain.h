/**@file
    Unit test main of the I2c Platform Dxe driver.

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
// IsSerialIoI2cDeviceIdCp Unit Test.
//
#include <TestCase/IsSerialIoI2cDeviceIdCp/TestCaseIsSerialIoI2cDeviceIdCp.h>
//
// SetConfiguration Unit Test.
//
#include <TestCase/SetConfiguration/TestCaseSetConfiguration.h>
//
// GetPciDevId Unit Test.
//
#include <TestCase/GetPciDevId/TestCaseGetPciDevId.h>
//
// GetBusFrequency Unit Test.
//
#include <TestCase/GetBusFrequency/TestCaseGetBusFrequency.h>
//
// Enumerate Unit Test.
//
#include <TestCase/Enumerate/TestCaseEnumerate.h>
//
// I2cPlatformDriverBindingSupported Unit Test.
//
#include <TestCase/I2cPlatformDriverBindingSupported/TestCaseI2cPlatformDriverBindingSupported.h>
//
// I2cPlatformDriverBindingStart Unit Test.
//
#include <TestCase/I2cPlatformDriverBindingStart/TestCaseI2cPlatformDriverBindingStart.h>
//
// I2cPlatformDriverBindingStop Unit Test.
//
#include <TestCase/I2cPlatformDriverBindingStop/TestCaseI2cPlatformDriverBindingStop.h>
//
// I2cPlatformEntryPoint Unit Test.
//
#include <TestCase/I2cPlatformEntryPoint/TestCaseI2cPlatformEntryPoint.h>

#endif
