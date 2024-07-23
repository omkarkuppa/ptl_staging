/**@file
  Unit test main of the I2c Touch Device Acpi Driver

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

#ifndef _I2C_TOUCH_DEV_ACPI_UNIT_TEST_MAIN_H_
#define _I2C_TOUCH_DEV_ACPI_UNIT_TEST_MAIN_H_

//
// PatchI2cDevicesBuffer
//
#include "PreRequisite/PatchI2cDevicesBuffer/PrePatchI2cDevicesBuffer.h"
#include "TestCase/PatchI2cDevicesBuffer/TestCasePatchI2cDevicesBuffer.h"
#include "CleanUp/PatchI2cDevicesBuffer/CleanUpPatchI2cDevicesBuffer.h"

//
// PatchRtd3Buffer
//
#include "PreRequisite/PatchRtd3Buffer/PrePatchRtd3Buffer.h"
#include "TestCase/PatchRtd3Buffer/TestCasePatchRtd3Buffer.h"
#include "CleanUp/PatchRtd3Buffer/CleanUpPatchRtd3Buffer.h"

//
// I2CBoardData
//
#include "PreRequisite/I2CBoardData/PreI2CBoardData.h"
#include "TestCase/I2CBoardData/TestCaseI2CBoardData.h"
#include "CleanUp/I2CBoardData/CleanUpI2CBoardData.h"

//
// I2cDevPatchEntry
//
#include "PreRequisite/I2cDevPatchEntry/PreI2cDevPatchEntry.h"
#include "TestCase/I2cDevPatchEntry/TestCaseI2cDevPatchEntry.h"
#include "CleanUp/I2cDevPatchEntry/CleanUpI2cDevPatchEntry.h"

#endif  // _I2C_TOUCH_DEV_ACPI_UNIT_TEST_MAIN_H_
