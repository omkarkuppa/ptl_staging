/** @file
  Unit test main of the Multi-IBB Service driver.

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

#include "CleanUp/MultiIbbServiceEntryPoint/CleanUpMultiIbbServiceEntryPoint.h"
#include "CleanUp/AdjustInitNemRange/CleanUpAdjustInitNemRange.h"
#include "CleanUp/CreateSha384Hash/CleanUpCreateSha384Hash.h"
#include "CleanUp/FvIntegrityCheck/CleanUpFvIntegrityCheck.h"
#include "CleanUp/MultiIbbCacheMissDebugCheck/CleanUpMultiIbbCacheMissDebugCheck.h"
#include "CleanUp/IbbCacheabilityType/CleanUpIbbCacheabilityType.h"
#include "CleanUp/EndOfIbbNotifyCallback/CleanUpEndOfIbbNotifyCallback.h"
#include "CleanUp/MemoryDiscoveredPpiNotifyCallback/CleanUpMemoryDiscoveredPpiNotifyCallback.h"

#include "PreRequisite/MultiIbbServiceEntryPoint/PreMultiIbbServiceEntryPoint.h"
#include "PreRequisite/AdjustInitNemRange/PreAdjustInitNemRange.h"
#include "PreRequisite/CreateSha384Hash/PreCreateSha384Hash.h"
#include "PreRequisite/FvIntegrityCheck/PreFvIntegrityCheck.h"
#include "PreRequisite/MultiIbbCacheMissDebugCheck/PreMultiIbbCacheMissDebugCheck.h"
#include "PreRequisite/IbbCacheabilityType/PreIbbCacheabilityType.h"
#include "PreRequisite/EndOfIbbNotifyCallback/PreEndOfIbbNotifyCallback.h"
#include "PreRequisite/MemoryDiscoveredPpiNotifyCallback/PreMemoryDiscoveredPpiNotifyCallback.h"

#include "TestCase/MultiIbbServiceEntryPoint/TestCaseMultiIbbServiceEntryPoint.h"
#include "TestCase/AdjustInitNemRange/TestCaseAdjustInitNemRange.h"
#include "TestCase/CreateSha384Hash/TestCaseCreateSha384Hash.h"
#include "TestCase/FvIntegrityCheck/TestCaseFvIntegrityCheck.h"
#include "TestCase/MultiIbbCacheMissDebugCheck/TestCaseMultiIbbCacheMissDebugCheck.h"
#include "TestCase/IbbCacheabilityType/TestCaseIbbCacheabilityType.h"
#include "TestCase/EndOfIbbNotifyCallback/TestCaseEndOfIbbNotifyCallback.h"
#include "TestCase/MemoryDiscoveredPpiNotifyCallback/TestCaseMemoryDiscoveredPpiNotifyCallback.h"
