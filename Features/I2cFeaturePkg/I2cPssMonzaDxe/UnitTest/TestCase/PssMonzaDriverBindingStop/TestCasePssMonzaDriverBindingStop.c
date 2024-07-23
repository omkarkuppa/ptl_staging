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
#include "TestCasePssMonzaDriverBindingStop.h"
#include <Protocol/DriverBinding.h>
#include <Library/MemoryAllocationLib.h>
#include <BootServicesMock.h>
#include <Library/UefiBootServicesTableLib.h>


/**
  Stops a device controller or a bus controller.

  The Stop() function is designed to be invoked from the EFI boot service DisconnectController().
  As a result, much of the error checking on the parameters to Stop() has been moved
  into this common boot service. It is legal to call Stop() from other locations,
  but the following calling restrictions must be followed, or the system behavior will not be deterministic.
  1. ControllerHandle must be a valid EFI_HANDLE that was used on a previous call to this
     same driver's Start() function.
  2. The first NumberOfChildren handles of ChildHandleBuffer must all be a valid
     EFI_HANDLE. In addition, all of these handles must have been created in this driver's
     Start() function, and the Start() function must have called OpenProtocol() on
     ControllerHandle with an Attribute of EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER.

  @param[in]  This              A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle  A handle to the device being stopped. The handle must
                                support a bus specific I/O protocol for the driver
                                to use to stop the device.
  @param[in]  NumberOfChildren  The number of child device handles in ChildHandleBuffer.
  @param[in]  ChildHandleBuffer An array of child handles to be freed. May be NULL
                                if NumberOfChildren is 0.

  @retval EFI_SUCCESS           The device was stopped.
  @retval EFI_DEVICE_ERROR      The device could not be stopped due to a device error.

**/
extern
EFI_STATUS
EFIAPI
PssMonzaDriverBindingStop (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN UINTN                        NumberOfChildren,
  IN EFI_HANDLE                   *ChildHandleBuffer OPTIONAL
  );

/**
  Mock PssMonzaDriverBindingStop

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
UnitTestPssMonzaDriverBindingStop (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS                           Status;
  EFI_DRIVER_BINDING_PROTOCOL          DriverBinding;
  UINT8                                Counter;
  PROTOCOLS_LIST                       *ProtocolList;
  UINTN                                NumberOfChildren;
  PSS_MONZA_PRIVATE_CONTEXT            *PssMonzaContext;

  NumberOfChildren =0;
  ProtocolList = NULL;
  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));
  //
  // Mock boot services, initial PssProtocol and PSS_MONZA_PRIVATE_CONTEXT.
  //
  PssMonzaContext = AllocateZeroPool (sizeof (PSS_MONZA_PRIVATE_CONTEXT));
  PssMonzaContext->Signature = PSS_MONZA_SIGNATURE;
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS->OpenProtocol = MockBootServiceOpenProtocol;
  gBS->CloseProtocol = MockBootServiceCloseProtocol;
  gBS->UninstallMultipleProtocolInterfaces = MockBootServiceUninstallMultipleProtocolInterfaces;

  //
  // Test1: Mock handle doesn't support gI2cPssProtocolGuid.
  // Expected Result: EFI_SUCCESS. but system will return: PSS Monza driver still in use.
  //
  expect_value (MockBootServiceOpenProtocol, Protocol, &gI2cPssProtocolGuid);
  will_return (MockBootServiceOpenProtocol, EFI_UNSUPPORTED);
  will_return (MockBootServiceOpenProtocol, &(PssMonzaContext->PssProtocol));

  Status = PssMonzaDriverBindingStop (&DriverBinding, NULL, NumberOfChildren, NULL);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  //
  // Test2: Mock handle support gI2cPssProtocolGuid but failed to unload gI2cPssProtocolGuid with handle.
  // Expected Result: EFI_INVALID_PARAMETER.
  //
  expect_value (MockBootServiceOpenProtocol, Protocol, &gI2cPssProtocolGuid);
  will_return (MockBootServiceOpenProtocol, EFI_SUCCESS);
  will_return (MockBootServiceOpenProtocol, &(PssMonzaContext->PssProtocol));
  will_return (MockBootServiceCloseProtocol, EFI_SUCCESS);
  //
  // Mock UninstallMultipleProtocolInterfaces. All protocols will store in ProtocolList.
  // but protocol interface could not be uninstalled.
  //
  will_return (MockBootServiceUninstallMultipleProtocolInterfaces, &ProtocolList);
  will_return (MockBootServiceUninstallMultipleProtocolInterfaces, &Counter);
  will_return (MockBootServiceUninstallMultipleProtocolInterfaces, EFI_INVALID_PARAMETER);

  Status = PssMonzaDriverBindingStop (&DriverBinding, NULL, NumberOfChildren, NULL);
  UT_ASSERT_EQUAL (Status, EFI_INVALID_PARAMETER);
  //
  // Test3: Mock handle support gI2cPssProtocolGuid and the protocol interfaces were removed successful.
  // Expected Result: EFI_SUCCESS.
  //
  expect_value (MockBootServiceOpenProtocol, Protocol, &gI2cPssProtocolGuid);
  will_return (MockBootServiceOpenProtocol, EFI_SUCCESS);
  will_return (MockBootServiceOpenProtocol, &(PssMonzaContext->PssProtocol));
  will_return (MockBootServiceCloseProtocol, EFI_SUCCESS);
  //
  // Mock UninstallMultipleProtocolInterfaces. All protocols will store in ProtocolList.
  // Then check protocols that we expected by CompareGuid().
  //
  will_return (MockBootServiceUninstallMultipleProtocolInterfaces, &ProtocolList);
  will_return (MockBootServiceUninstallMultipleProtocolInterfaces, &Counter);
  will_return (MockBootServiceUninstallMultipleProtocolInterfaces, EFI_SUCCESS);
  will_return (MockBootServiceCloseProtocol, EFI_SUCCESS);
  //
  // Mock FreeUnicodeStringTable to return EFI_SUCCESS.
  //
  will_return (FreeUnicodeStringTable, EFI_SUCCESS);

  Status = PssMonzaDriverBindingStop (&DriverBinding, NULL, NumberOfChildren, NULL);
  // Check ProtocolList include gI2cPssProtocolGuid.
  UT_ASSERT_EQUAL (CompareGuid (ProtocolList[0].Protocol, &gI2cPssProtocolGuid), TRUE);
  // Check Status is EFI_SUCCESS.
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));
  //
  // Destroy Mock Service
  //
  FreePool (gBS);
  return UNIT_TEST_PASSED;

}