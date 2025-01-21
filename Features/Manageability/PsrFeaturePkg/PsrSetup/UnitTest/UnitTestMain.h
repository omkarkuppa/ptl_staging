/** @file
  Unit test main of the Psr Setup driver.

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

#include "CleanUp/PsrSetupInit/CleanUpPsrSetupInit.h"
#include "CleanUp/UpdatePsrCapabilitiesInfo/CleanUpUpdatePsrCapabilitiesInfo.h"
#include "CleanUp/GetPsrEventSource/CleanUpGetPsrEventSource.h"
#include "CleanUp/GetPsrEventStatus/CleanUpGetPsrEventStatus.h"
#include "CleanUp/GetPsrEventAction/CleanUpGetPsrEventAction.h"
#include "CleanUp/GetPsrEventName/CleanUpGetPsrEventName.h"
#include "CleanUp/GetPsrLogState/CleanUpGetPsrLogState.h"
#include "CleanUp/CountOfLeapYearsInRange/CleanUpCountOfLeapYearsInRange.h"
#include "CleanUp/UnixTimeStampToDate/CleanUpUnixTimeStampToDate.h"
#include "CleanUp/ConvertToString/CleanUpConvertToString.h"
#include "CleanUp/StrTrim/CleanUpStrTrim.h"
#include "CleanUp/PsrGetInfo/CleanUpPsrGetInfo.h"

#include "PreRequisite/PsrSetupInit/PrePsrSetupInit.h"
#include "PreRequisite/UpdatePsrCapabilitiesInfo/PreUpdatePsrCapabilitiesInfo.h"
#include "PreRequisite/GetPsrEventSource/PreGetPsrEventSource.h"
#include "PreRequisite/GetPsrEventStatus/PreGetPsrEventStatus.h"
#include "PreRequisite/GetPsrEventAction/PreGetPsrEventAction.h"
#include "PreRequisite/GetPsrEventName/PreGetPsrEventName.h"
#include "PreRequisite/GetPsrLogState/PreGetPsrLogState.h"
#include "PreRequisite/CountOfLeapYearsInRange/PreCountOfLeapYearsInRange.h"
#include "PreRequisite/UnixTimeStampToDate/PreUnixTimeStampToDate.h"
#include "PreRequisite/ConvertToString/PreConvertToString.h"
#include "PreRequisite/StrTrim/PreStrTrim.h"
#include "PreRequisite/PsrGetInfo/PrePsrGetInfo.h"

#include "TestCase/PsrSetupInit/TestCasePsrSetupInit.h"
#include "TestCase/UpdatePsrCapabilitiesInfo/TestCaseUpdatePsrCapabilitiesInfo.h"
#include "TestCase/GetPsrEventSource/TestCaseGetPsrEventSource.h"
#include "TestCase/GetPsrEventStatus/TestCaseGetPsrEventStatus.h"
#include "TestCase/GetPsrEventAction/TestCaseGetPsrEventAction.h"
#include "TestCase/GetPsrEventName/TestCaseGetPsrEventName.h"
#include "TestCase/GetPsrLogState/TestCaseGetPsrLogState.h"
#include "TestCase/CountOfLeapYearsInRange/TestCaseCountOfLeapYearsInRange.h"
#include "TestCase/UnixTimeStampToDate/TestCaseUnixTimeStampToDate.h"
#include "TestCase/ConvertToString/TestCaseConvertToString.h"
#include "TestCase/StrTrim/TestCaseStrTrim.h"
#include "TestCase/PsrGetInfo/TestCasePsrGetInfo.h"
