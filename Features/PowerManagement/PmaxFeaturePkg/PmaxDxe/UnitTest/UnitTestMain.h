/**@file

  Unit test main of the Pmax Dxe driver.

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

//
// Include below for FindPmaxTable.
//
#include "PreRequisite/FindPmaxTable/PreFindPmaxTable.h"
#include "TestCase/FindPmaxTable/TestCaseFindPmaxTable.h"
#include "CleanUp/FindPmaxTable/CleanUpFindPmaxTable.h"
//
// Include below for UpdatePmaxTable.
//
#include "PreRequisite/UpdatePmaxTable/PreUpdatePmaxTable.h"
#include "TestCase/UpdatePmaxTable/TestCaseUpdatePmaxTable.h"
#include "CleanUp/UpdatePmaxTable/CleanUpUpdatePmaxTable.h"
//
// Include below for InstallTable.
//
#include "PreRequisite/InstallTable/PreInstallTable.h"
#include "TestCase/InstallTable/TestCaseInstallTable.h"
#include "CleanUp/InstallTable/CleanUpInstallTable.h"
//
// Include below for GetStrLength.
//
#include "TestCase/GetStrLength/TestCaseGetStrLength.h"
//
// Include below for InstallPmaxTable.
//
#include "TestCase/InstallPmaxTable/TestCaseInstallPmaxTable.h"
