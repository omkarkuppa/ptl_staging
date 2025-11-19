/**@file
    Unit test CleanUp functionality.

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
#ifndef _MOCK_CLEAN_UP_H_
#define _MOCK_CLEAN_UP_H_

#include <Uefi.h>
#include <Library/UnitTestLib.h>
#include <Library/MemoryAllocationLib.h>

/**
  Unit-Test Cleanup (after) function pointer.

  @param[in]  Context    cleanup the precondition parameter.

**/
VOID
UnitTestCleanup (
  IN UNIT_TEST_CONTEXT  Context
  );

#endif /*_MOCK_CLEAN_UP_H_*/
