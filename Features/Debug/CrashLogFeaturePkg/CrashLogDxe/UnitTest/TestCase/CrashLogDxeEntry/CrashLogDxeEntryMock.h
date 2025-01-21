/**@file
    Unit test case mock header for CrashLogDxeEntry.

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
#ifndef _CRASHLOG_DXE_ENTRY_MOCK_H_
#define _CRASHLOG_DXE_ENTRY_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Library/UefiBootServicesTableLib.h>

/**
  Unit-Test Prerequisite Function pointer.

  @param[in]  Context    An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case prerequisites
                                                 are met.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.

**/
UNIT_TEST_STATUS
EFIAPI
PreCrashLogDxeEntry (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Unit-Test CrashLogDxeEntry CleanUp Function. Clean the input for mock

  @param[in]  Context    Input context for mock.

**/
VOID
EFIAPI
CleanUpCrashLogDxeEntry (
  IN UNIT_TEST_CONTEXT  Context
  );

/**
  Entry point of the CrashLog Dxe driver.

  @param[in] ImageHandle  EFI_HANDLE: A handle for the image that is initializing this driver
  @param[in] SystemTable  EFI_SYSTEM_TABLE: A pointer to the EFI system table

  @retval  EFI_SUCCESS:           Driver initialized successfully
           EFI_LOAD_ERROR:        Failed to Initialize or to Load the driver
           EFI_OUT_OF_RESOURCES:  Could not allocate needed resources

**/
EFI_STATUS
EFIAPI
CrashLogDxeEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

/**
  Unit test for CrashLogDxeEntry.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestCrashLogDxeEntry (
  IN UNIT_TEST_CONTEXT  Context
  );
#endif /*_CRASHLOG_DXE_ENTRY_MOCK_H_*/
