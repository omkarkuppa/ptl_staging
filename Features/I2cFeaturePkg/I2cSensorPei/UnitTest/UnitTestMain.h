/**@file
    Unit test main of the I2C Sensor PEIM.

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

@par Specification
**/
#ifndef _UNIT_TEST_MAIN_H_
#define _UNIT_TEST_MAIN_H_
//
// I2cSensorPeiEntry Unit Test.
//
#include <TestCase/I2cSensorPeiEntry/TestCaseI2cSensorPeiEntry.h>
//
// ReadBmi323Orientation Unit Test.
//
#include <TestCase/ReadBmi323Orientation/TestCaseReadBmi323Orientation.h>
//
// ReadLsm6dsoxOrientation Unit Test.
//
#include <TestCase/ReadLsm6dsoxOrientation/TestCaseReadLsm6dsoxOrientation.h>
//
// ReadSensorData Unit Test.
//
#include <TestCase/ReadSensorData/TestCaseReadSensorData.h>
//
// WriteSensor16 Unit Test.
//
#include <TestCase/WriteSensor16/TestCaseWriteSensor16.h>
//
// WriteSensorByte Unit Test.
//
#include <TestCase/WriteSensorByte/TestCaseWriteSensorByte.h>

#endif
