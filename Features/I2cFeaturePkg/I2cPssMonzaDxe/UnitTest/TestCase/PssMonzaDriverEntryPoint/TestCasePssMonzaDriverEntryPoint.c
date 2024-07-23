/**@file
    Unit test case of the I2c Pss Monza Dxe driver.

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <I2cPssMonza.h>
#include "TestCasePssMonzaDriverEntryPoint.h"
#include <Protocol/DriverBinding.h>
#include <Library/MemoryAllocationLib.h>

/**
  This is the declaration of an EFI image entry point. This entry point is
  the same for UEFI Applications, UEFI OS Loaders, and UEFI Drivers including
  both device drivers and bus drivers.

  @param[in]  ImageHandle       The firmware allocated handle for the UEFI image.
  @param[in]  SystemTable       A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.
**/
extern
EFI_STATUS
EFIAPI
PssMonzaDriverEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

/**
  Mock PssMonzaDriverEntryPoint

  @param[in]  Context                   [Optional] An optional parameter that enables:
                                        1) test-case reuse with varied parameters and
                                        2) test-case re-entry for Target tests that need a
                                        reboot.  This parameter is a VOID* and it is the
                                        responsibility of the test author to ensure that the
                                        contents are well understood by all test cases that may
                                        consume it.
  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestPssMonzaDriverEntryPoint (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                      Status;
  EFI_DRIVER_BINDING_PROTOCOL     **DriverBinding;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  DriverBinding = AllocateZeroPool (sizeof (EFI_DRIVER_BINDING_PROTOCOL));
  //
  // Test 1: Check PSS_MONZA_VERSION, If PSS_MONZA_VERSION same with EFI_DRIVER_BINDING_PROTOCOL,
  //         then return EFI_SUCCESS.
  //
  will_return (EfiLibInstallDriverBindingComponentName2, DriverBinding);
  Status = PssMonzaDriverEntryPoint (NULL, NULL);
  // Check (**DriverBinding).Version is PSS_MONZA_VERSION.
  UT_ASSERT_EQUAL ((**DriverBinding).Version, PSS_MONZA_VERSION);
  // Check Status is EFI_SUCCESS.
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));
  //
  // Destroy Mock Service
  //
  FreePool (DriverBinding);
  return UNIT_TEST_PASSED;

}