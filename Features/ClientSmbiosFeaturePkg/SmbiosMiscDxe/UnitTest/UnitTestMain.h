/** @file
  Unit test case of the Smbios Misc Dxe driver.

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

#include "PreRequisite/AddSmbiosT24Callback/PreAddSmbiosT24Callback.h"
#include "PreRequisite/AddSmbiosT14Callback/PreAddSmbiosT14Callback.h"
#include "PreRequisite/AddSmbiosRecord/PreAddSmbiosRecord.h"
#include "PreRequisite/UpdateMiscSmbios/PreUpdateMiscSmbios.h"
#include "PreRequisite/UpdateMiscSmbiosCallback/PreUpdateMiscSmbiosCallback.h"
#include "PreRequisite/SmbiosMiscEntryPoint/PreSmbiosMiscEntryPoint.h"
#include "PreRequisite/MiscCommonSmbiosFunction/PreMiscCommonSmbiosFunction.h"

#include "TestCase/AddSmbiosT24Callback/TestCaseAddSmbiosT24Callback.h"
#include "TestCase/AddSmbiosT14Callback/TestCaseAddSmbiosT14Callback.h"
#include "TestCase/AddSmbiosRecord/TestCaseAddSmbiosRecord.h"
#include "TestCase/UpdateMiscSmbios/TestCaseUpdateMiscSmbios.h"
#include "TestCase/UpdateMiscSmbiosCallback/TestCaseUpdateMiscSmbiosCallback.h"
#include "TestCase/SmbiosStrInit/TestCaseSmbiosStrInit.h"
#include "TestCase/SmbiosStringsUpdate/TestCaseSmbiosStringsUpdate.h"
#include "TestCase/SmbiosMiscEntryPoint/TestCaseSmbiosMiscEntryPoint.h"
#include "TestCase/MiscCommonSmbiosFunction/TestCaseMiscCommonSmbiosFunction.h"

#include "CleanUp/AddSmbiosT24Callback/CleanUpAddSmbiosT24Callback.h"
#include "CleanUp/AddSmbiosT14Callback/CleanUpAddSmbiosT14Callback.h"
#include "CleanUp/AddSmbiosRecord/CleanUpAddSmbiosRecord.h"
#include "CleanUp/UpdateMiscSmbios/CleanUpUpdateMiscSmbios.h"
#include "CleanUp/UpdateMiscSmbiosCallback/CleanUpUpdateMiscSmbiosCallback.h"
#include "CleanUp/SmbiosMiscEntryPoint/CleanUpSmbiosMiscEntryPoint.h"
#include "CleanUp/MiscCommonSmbiosFunction/CleanUpMiscCommonSmbiosFunction.h"
