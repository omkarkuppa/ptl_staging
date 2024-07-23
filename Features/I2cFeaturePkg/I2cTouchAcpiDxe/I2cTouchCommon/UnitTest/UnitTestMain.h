/**@file
  Unit test main of the I2C Device Common Driver

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

@par Specification Reference:
**/

#ifndef _I2C_TOUCH_COMMON_UNIT_TEST_MAIN_H_
#define _I2C_TOUCH_COMMON_UNIT_TEST_MAIN_H_

//
// InstallI2cTable
//
#include "PreRequisite/InstallI2cTable/PreInstallI2cTable.h"
#include "TestCase/InstallI2cTable/TestCaseInstallI2cTable.h"
#include "CleanUp/InstallI2cTable/CleanUpInstallI2cTable.h"

//
// GetDevTablePointer
//
#include "PreRequisite/GetDevTablePointer/PreGetDevTablePointer.h"
#include "TestCase/GetDevTablePointer/TestCaseGetDevTablePointer.h"
#include "CleanUp/GetDevTablePointer/CleanUpGetDevTablePointer.h"

//
// I2CTouchDevEntry
//
#include "PreRequisite/I2CTouchDevEntry/PreI2CTouchDevEntry.h"
#include "TestCase/I2CTouchDevEntry/TestCaseI2CTouchDevEntry.h"
#include "CleanUp/I2CTouchDevEntry/CleanUpI2CTouchDevEntry.h"

#endif  // _I2C_TOUCH_COMMON_UNIT_TEST_MAIN_H_
