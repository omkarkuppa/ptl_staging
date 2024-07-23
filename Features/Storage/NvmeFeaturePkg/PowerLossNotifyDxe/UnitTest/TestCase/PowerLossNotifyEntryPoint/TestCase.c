/**@file
    Unit test cases of PowerLossNotifyEntryPoint.

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

#include "PowerLossNotifyEntryPointMock.h"

extern VOID *mNvmePassthruRegistration;
/**
  Unit test for PowerLossNotifyEntryPoint.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestPlnEntryPoint_1 (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS         Status;
  EFI_BOOT_SERVICES  *mBS;

  //
  // Mock boot service LocateProtocol
  //
  mBS = BootServiceConstructorMock ();
  mBS->LocateProtocol = MockBootServiceLocateProtocol;

  expect_value(MockBootServiceLocateProtocol, Protocol, &gPowerLossNotifySettingProtocolGuid);
  will_return (MockBootServiceLocateProtocol, NULL);
  will_return (MockBootServiceLocateProtocol, EFI_NOT_FOUND);

  Status = PowerLossNotifyEntryPoint (NULL, NULL);
  BootServiceDestructorMock ();

  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);
  return UNIT_TEST_PASSED;
}

/**
  Unit test for PowerLossNotifyEntryPoint.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestPlnEntryPoint_2 (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS         Status;
  EFI_BOOT_SERVICES  *mBS;
  POWER_LOSS_NOTIFY_SETTING_PROTOCOL  *PlnProtocol;

  //
  // Mock POWER_LOSS_NOTIFY_SETTING_PROTOCOL
  //
  PlnProtocol = AllocateZeroPool (sizeof (POWER_LOSS_NOTIFY_SETTING_PROTOCOL));
  PlnProtocol->GetPlnSetting    = MockPlnSettingProtocol_GetPlnSetting;
  PlnProtocol->SetPlnController = MockPlnSettingProtocol_SetPlnController;

  //
  // Mock boot service LocateProtocol
  //
  mBS = BootServiceConstructorMock ();
  mBS->LocateProtocol = MockBootServiceLocateProtocol;

  expect_value(MockBootServiceLocateProtocol, Protocol, &gPowerLossNotifySettingProtocolGuid);
  will_return (MockBootServiceLocateProtocol, PlnProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);

  Status = PowerLossNotifyEntryPoint (NULL, NULL);
  BootServiceDestructorMock ();

  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  return UNIT_TEST_PASSED;
}

/**
  Unit test for PowerLossNotifyEntryPoint.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestPlnEntryPoint_3 (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS         Status;
  EFI_BOOT_SERVICES  *mBS;
  POWER_LOSS_NOTIFY_SETTING_PROTOCOL  *PlnProtocol;

  //
  // Mock POWER_LOSS_NOTIFY_SETTING_PROTOCOL
  //
  PlnProtocol = AllocateZeroPool (sizeof (POWER_LOSS_NOTIFY_SETTING_PROTOCOL));
  PlnProtocol->GetPlnSetting    = MockPlnSettingProtocol_GetPlnSetting;
  PlnProtocol->SetPlnController = MockPlnSettingProtocol_SetPlnController;
  gPlnSetting = PLN_ENABLE;

  //
  // Mock boot service LocateProtocol
  //
  mBS = BootServiceConstructorMock ();
  mBS->LocateProtocol = MockBootServiceLocateProtocol;

  expect_value(MockBootServiceLocateProtocol, Protocol, &gPowerLossNotifySettingProtocolGuid);
  will_return (MockBootServiceLocateProtocol, PlnProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);

  Status = PowerLossNotifyEntryPoint (NULL, NULL);
  BootServiceDestructorMock ();

  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_NOT_NULL (mNvmePassthruRegistration);
  FreePool(mNvmePassthruRegistration);
  return UNIT_TEST_PASSED;
}
