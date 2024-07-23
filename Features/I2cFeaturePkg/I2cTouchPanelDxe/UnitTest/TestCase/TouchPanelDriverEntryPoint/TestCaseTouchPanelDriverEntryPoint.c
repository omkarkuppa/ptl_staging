/**@file
    Unit test case of the I2c TouchPanel Dxe driver.

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
#include "TestCaseTouchPanelDriverEntryPoint.h"
#include <I2cTouchPanel.h>
#include <Library/UefiBootServicesTableLib.h>
#include <BootServicesMock.h>

/**
  This is the declaration of an EFI image entry point. This entry point is
  the same for UEFI Applications, UEFI OS Loaders, and UEFI Drivers including
  both device drivers and bus drivers.

  @param  ImageHandle           The firmware allocated handle for the UEFI image.
  @param  SystemTable           A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.
**/
extern
EFI_STATUS
EFIAPI
TouchPanelDriverEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  );

UNIT_TEST_STATUS
EFIAPI
UnitTestTouchPanelDriverEntryPoint (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                      Status;
  EFI_DRIVER_BINDING_PROTOCOL     **DriverBinding;
  PROTOCOLS_LIST                  *ProtocolList;
  UINT8                           Counter;

  //
  // Mock boot services.
  //
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS->InstallMultipleProtocolInterfaces = MockBootServiceInstallMultipleProtocolInterfaces;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  DriverBinding = AllocateZeroPool (sizeof (EFI_DRIVER_BINDING_PROTOCOL));
  //
  // Test 1: Check TOUCH_DRIVER_VERSION, If TOUCH_DRIVER_VERSION same with EFI_DRIVER_BINDING_PROTOCOL,
  //         then return EFI_SUCCESS.
  //
  ProtocolList = NULL;
  will_return (EfiLibInstallDriverBindingComponentName2, DriverBinding);
  will_return (MockBootServiceInstallMultipleProtocolInterfaces, &ProtocolList);
  will_return (MockBootServiceInstallMultipleProtocolInterfaces, &Counter);
  will_return (MockBootServiceInstallMultipleProtocolInterfaces, EFI_SUCCESS);
  Status = TouchPanelDriverEntryPoint (NULL, NULL);
  // Check Status is EFI_SUCCESS.
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  // Check ProtocolList include gEfiDriverSupportedEfiVersionProtocolGuid.
  UT_ASSERT_EQUAL (CompareGuid (ProtocolList[0].Protocol, &gEfiDriverSupportedEfiVersionProtocolGuid), TRUE);
  // Check (**DriverBinding).Version is TOUCH_DRIVER_VERSION.
  UT_ASSERT_EQUAL ((**DriverBinding).Version, TOUCH_DRIVER_VERSION);

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));
  //
  // Destroy Mock Service
  //
  FreePool (DriverBinding);
  FreePool (ProtocolList);
  return UNIT_TEST_PASSED;
}