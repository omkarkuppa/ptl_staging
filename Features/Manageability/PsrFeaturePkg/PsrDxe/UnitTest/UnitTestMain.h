/** @file
  Unit test main of the Psr Dxe driver.

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

#include "CleanUp/GetPsrData/CleanUpGetPsrData.h"

#include "PreRequisite/GetPsrData/PreGetPsrData.h"

#include "TestCase/IsPsrSupported/TestCaseIsPsrSupported.h"
#include "TestCase/NotifyPsrCapabilitySet/TestCaseNotifyPsrCapabilitySet.h"
#include "TestCase/GetPsrData/TestCaseGetPsrData.h"
#include "TestCase/NotifyChassisIntrusionDetection/TestCaseNotifyChassisIntrusionDetection.h"
#include "TestCase/NotifyCriticalTempShutdown/TestCaseNotifyCriticalTempShutdown.h"
#include "TestCase/ExportPsrToUsbStorage/TestCaseExportPsrToUsbStorage.h"
#include "TestCase/NotifyEventReport/TestCaseNotifyEventReport.h"
