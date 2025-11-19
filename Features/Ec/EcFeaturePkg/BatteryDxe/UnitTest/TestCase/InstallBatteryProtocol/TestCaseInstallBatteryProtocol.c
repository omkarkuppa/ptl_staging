/**@file
    Unit test case of the Battery Dxe driver.

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

#include <Uefi.h>
#include <EcCommands.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeBatteryLib.h>
#include <Protocol/Battery.h>
#include <Protocol/EcNvsArea.h>

#include "BootServicesMock.h"
#include "TestCaseGetBatteryInformation.h"

extern
EFI_STATUS
EFIAPI
InstallBatteryProtocol (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  );

/**
  Unit test for InstallBatteryProtocol

  @param[in]  Context    [Optional] An optional parameter that enables:
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
UnitTestInstallBatteryProtocol (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_SYSTEM_TABLE      *SystemTable;
  EFI_STATUS            Status;
  EFI_HANDLE            Handle;
  EFI_BATTERY_PROTOCOL  *BatteryProtocol;
  EC_NVS_AREA_PROTOCOL  *EcNvsAreaProtocol;

  DEBUG ((DEBUG_INFO, "%a Start Test\n", __FUNCTION__));

  //
  // Test function InstallBatteryProtocol with input parameter using NULL case
  //
  // Expect Result:
  //   - EFI_INVALID_PARAMETER: have error handle the NULL input parameter
  //
  Status = InstallBatteryProtocol (NULL, NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);


  //
  // Test function InstallBatteryProtocol with input parameter using Mock case
  //
  // Expect Result:
  //   - EFI_SUCCESS: Install battery Protocol successfully
  //

  //
  // Mock Dummy System Table and Dummy Handle
  //
  SystemTable = AllocateZeroPool (sizeof (EFI_SYSTEM_TABLE));
  Handle      = AllocateZeroPool (sizeof (EFI_HANDLE));

  //
  // Mock gBS->LocateProtocol
  // Mock gBS->InstallProtocolInterface
  //
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS->LocateProtocol = MockBootServiceLocateProtocol;
  gBS->InstallProtocolInterface = MockBootServiceInstallProtocolInterface;

  //
  // Mock gEcNvsAreaProtocolGuid
  //
  EcNvsAreaProtocol = AllocateZeroPool (sizeof (EC_NVS_AREA_PROTOCOL));
  EcNvsAreaProtocol->Area = AllocateZeroPool (sizeof (EC_NVS_AREA));

  //
  // Setting Mock environment with PcdBatterySupport = 0
  //
  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdBatterySupport);
  will_return (LibPcdGet8, 0);

  Status = InstallBatteryProtocol (Handle, SystemTable);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);

  //
  // Setting Mock environment with PcdBatterySupport = 1
  //
  expect_value (LibPcdGet8, TokenNumber, _PCD_TOKEN_PcdBatterySupport);
  will_return (LibPcdGet8, 1);

  expect_value (MockBootServiceInstallProtocolInterface, Protocol, &gBatteryProtocolGuid);
  expect_value (MockBootServiceInstallProtocolInterface, InterfaceType, EFI_NATIVE_INTERFACE);
  will_return (MockBootServiceInstallProtocolInterface, &BatteryProtocol);
  will_return (MockBootServiceInstallProtocolInterface, EFI_SUCCESS);

  Status = InstallBatteryProtocol (Handle, SystemTable);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_EQUAL (BatteryProtocol->Revision, EFI_BATTERY_PROTOCOL_REVISION);

  //
  // Destroy Mock Data
  //
  FreePool (gBS);
  FreePool (EcNvsAreaProtocol->Area);
  FreePool (EcNvsAreaProtocol);
  FreePool (SystemTable);
  FreePool (Handle);

  DEBUG ((DEBUG_INFO, "%a End Test\n", __FUNCTION__));

  return UNIT_TEST_PASSED;
}
