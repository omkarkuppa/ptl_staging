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
#include "TestCaseTouchPanelDriverUnload.h"
#include <Library/DebugLib.h>
#include <I2cTouchPanel.h>
#include <Library/UefiBootServicesTableLib.h>
#include "PrivateMock/PrivateMock.h"
#include <BootServicesMock.h>

/**
  Unloads an image.

  @param  ImageHandle           Handle that identifies the image to be unloaded.

  @retval EFI_SUCCESS           The image has been unloaded.
  @retval EFI_INVALID_PARAMETER ImageHandle is not a valid image handle.

**/
extern
EFI_STATUS
EFIAPI
TouchPanelDriverUnload (
  IN EFI_HANDLE  ImageHandle
  );
/**
  Mock of TouchPanelDriverAbsolutePointerGetState

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
UnitTestTouchPanelDriverUnload (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINTN              BufferSize;
  EFI_STATUS         Status;
  EFI_HANDLE         ImageHandle;
  PROTOCOLS_LIST     *pProtocolList1;
  PROTOCOLS_LIST     *pProtocolList2;
  UINT8              Counter1;
  UINT8              Counter2;
  UINT32             *GuidString;
  UINT32             *DriverBindingProtocolGuidArray;
  UINT32             *ComponentNameProtocolGuidArray;
  UINT32             *ComponentName2ProtocolGuidArray;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  //
  // Mock boot service LocateHandleBuffer. DisconnectController and UninstallMultipleProtocolInterfaces
  //
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS->LocateHandleBuffer = MockBootServiceLocateHandleBuffer;
  gBS->UninstallMultipleProtocolInterfaces = MockBootServiceUninstallMultipleProtocolInterfaces;
  gBS->DisconnectController = MockBootServiceDisconnectController;
  //
  // Test1: LocateHandleBuffer was failed. TouchPanelDriverUnload return EFI_NOT_FOUND.
  // Expected Result: EFI_NOT_FOUND
  //
  BufferSize = 0;
  ImageHandle = AllocateZeroPool (sizeof (EFI_HANDLE));
  will_return (MockBootServiceLocateHandleBuffer, EFI_NOT_FOUND);
  will_return (MockBootServiceLocateHandleBuffer, BufferSize);
  Status = TouchPanelDriverUnload (ImageHandle);
  UT_ASSERT_EQUAL (Status, EFI_NOT_FOUND);
  //
  // Test2: LocateHandleBuffer was succeed but DisconnectController was failed. TouchPanelDriverUnload return EFI_INVALID_PARAMETER.
  // Expected Result: EFI_INVALID_PARAMETER
  //
  BufferSize = 1;
  will_return (MockBootServiceLocateHandleBuffer, EFI_SUCCESS);
  will_return (MockBootServiceLocateHandleBuffer, BufferSize);
  will_return (MockBootServiceDisconnectController, EFI_INVALID_PARAMETER);
  Status = TouchPanelDriverUnload (ImageHandle);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);
  //
  // Test3: first UninstallMultipleProtocolInterfaces was failed. TouchPanelDriverUnload return EFI_INVALID_PARAMETER.
  // Expected Result: EFI_INVALID_PARAMETER
  //
  pProtocolList1 = NULL;
  will_return (MockBootServiceLocateHandleBuffer, EFI_SUCCESS);
  will_return (MockBootServiceLocateHandleBuffer, BufferSize);
  will_return (MockBootServiceDisconnectController, EFI_SUCCESS);
  will_return (MockBootServiceUninstallMultipleProtocolInterfaces, &pProtocolList1);
  will_return (MockBootServiceUninstallMultipleProtocolInterfaces, &Counter1);
  will_return (MockBootServiceUninstallMultipleProtocolInterfaces, EFI_INVALID_PARAMETER);
  Status = TouchPanelDriverUnload (ImageHandle);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);
  //
  // Test3-1: first UninstallMultipleProtocolInterfaces was failed. TouchPanelDriverUnload return EFI_INVALID_PARAMETER.
  // Expected Result: EFI_INVALID_PARAMETER
  //
  pProtocolList2 = NULL;
  will_return (MockBootServiceLocateHandleBuffer, EFI_SUCCESS);
  will_return (MockBootServiceLocateHandleBuffer, BufferSize);
  will_return (MockBootServiceDisconnectController, EFI_SUCCESS);
  will_return (MockBootServiceUninstallMultipleProtocolInterfaces, &pProtocolList1);
  will_return (MockBootServiceUninstallMultipleProtocolInterfaces, &Counter1);
  will_return (MockBootServiceUninstallMultipleProtocolInterfaces, EFI_SUCCESS);
  will_return (MockBootServiceUninstallMultipleProtocolInterfaces, &pProtocolList2);
  will_return (MockBootServiceUninstallMultipleProtocolInterfaces, &Counter2);
  will_return (MockBootServiceUninstallMultipleProtocolInterfaces, EFI_INVALID_PARAMETER);
  Status = TouchPanelDriverUnload (ImageHandle);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);
  //
  // Test3-2: Second UninstallMultipleProtocolInterfaces was failed. TouchPanelDriverUnload return EFI_SUCCESS.
  // Expected Result: EFI_SUCCESS
  //
  BufferSize = 1;
  pProtocolList1 = NULL;
  pProtocolList2 = NULL;
  will_return (MockBootServiceLocateHandleBuffer, EFI_SUCCESS);
  will_return (MockBootServiceLocateHandleBuffer, BufferSize);
  will_return (MockBootServiceDisconnectController, EFI_SUCCESS);
  will_return (MockBootServiceUninstallMultipleProtocolInterfaces, &pProtocolList1);
  will_return (MockBootServiceUninstallMultipleProtocolInterfaces, &Counter1);
  will_return (MockBootServiceUninstallMultipleProtocolInterfaces, EFI_SUCCESS);
  will_return (MockBootServiceUninstallMultipleProtocolInterfaces, &pProtocolList2);
  will_return (MockBootServiceUninstallMultipleProtocolInterfaces, &Counter2);
  will_return (MockBootServiceUninstallMultipleProtocolInterfaces, EFI_SUCCESS);
  Status = TouchPanelDriverUnload (ImageHandle);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_EQUAL (pProtocolList1[0].Protocol, &gEfiDriverSupportedEfiVersionProtocolGuid);
  // Guid list is transformed to array.
  Status = GuidListToString (pProtocolList2, 3, &GuidString);
  // gEfiDriverBindingProtocolGuid is transformed to array.
  Status = GuidToString (&gEfiDriverBindingProtocolGuid, &DriverBindingProtocolGuidArray);
  // gEfiComponentNameProtocolGuid is transformed to array.
  Status = GuidToString (&gEfiComponentNameProtocolGuid, &ComponentNameProtocolGuidArray);
  // gEfiComponentName2ProtocolGuid is transformed to array.
  Status = GuidToString (&gEfiComponentName2ProtocolGuid,&ComponentName2ProtocolGuidArray);
  //
  // To search whether gEfiDriverBindingProtocolGuid.gEfiComponentNameProtocolGuid and gEfiComponentName2ProtocolGuid
  //is included in Protocol list which parse UninstallMultipleProtocolInterfaces from TouchPanelDriverUnload().
  //
  UT_ASSERT_EQUAL (GuidStringCmp (GuidString, DriverBindingProtocolGuidArray), TRUE);
  UT_ASSERT_EQUAL (GuidStringCmp (GuidString, ComponentNameProtocolGuidArray), TRUE);
  UT_ASSERT_EQUAL (GuidStringCmp (GuidString, ComponentName2ProtocolGuidArray), TRUE);

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));
  FreePool (gBS);
  FreePool (GuidString);
  FreePool (DriverBindingProtocolGuidArray);
  FreePool (ComponentNameProtocolGuidArray);
  FreePool (ComponentName2ProtocolGuidArray);
  FreePool (pProtocolList1);
  FreePool (pProtocolList2);
  return UNIT_TEST_PASSED;
}