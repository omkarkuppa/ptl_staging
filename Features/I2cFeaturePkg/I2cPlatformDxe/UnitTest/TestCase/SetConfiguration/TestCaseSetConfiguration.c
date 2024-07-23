/**@file
    Unit test case of the I2c Platform Dxe driver.

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
#include "TestCaseSetConfiguration.h"
#include <I2cPlatformDxe.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include "PrivateMock/PrivateMock.h"
#include <BootServicesMock.h>
#include <Library/MemoryAllocationLib.h>
/**
  Set bus configuration
  Since our platforms support only a single configuration per i2c bus,
  the only thing this function needs to do is to configure i2c central for correct bus speed

  @param[in]      This                     Pointer to an EFI_I2C_BUS_CONFIGURATION_MANAGEMENT_PROTOCOL structure.
  @param[in]      I2cBusConfiguration      Index of an I2C bus configuration. All values in the range of zero to N-1 are valid where N is the total number of I2C bus configurations for an I2C bus.
  @param[in]      Event                    Event to signal when the transaction is complete
  @param[in]      I2cStatus                Buffer to receive the transaction status.

  @retval EFI_SUCCESS              The asynchronous bus configuration request was successfully started when Event is not NULL.
  @retval EFI_SUCCESS              The bus configuration request completed successfully when Event is NULL.
  @retval EFI_DEVICE_ERROR         The bus configuration failed.
  @retval EFI_DEVICE_ERROR         Invalid I2cBusConfiguration value
**/
extern
EFI_STATUS
EFIAPI
SetConfiguration (
  IN CONST EFI_I2C_BUS_CONFIGURATION_MANAGEMENT_PROTOCOL *This,
  IN UINTN                                               I2cBusConfiguration,
  IN EFI_EVENT                                           Event      OPTIONAL,
  IN EFI_STATUS                                          *I2cStatus OPTIONAL
  );
/**
  GetBusFrequency
  For now, this function simply returns 100kHz because we only use one bus,
  single configuration, with a single device and it accepts just that speed

  @param[in]      This                     A pointer to the EFI_I2C_ENUMERATE_PROTOCOL instance.
  @param[in]      I2cBusConfiguration      A pointer to the I2cBusConfiguration
  @param[out]     BusClockHertz            Pointer is updated with right Serial I2C controller frequency

  @retval EFI_SUCCESS              BusClockHertz is returned with freq value
  @retval EFI_NOT_FOUND            End of device list
**/
extern
EFI_STATUS
EFIAPI
GetBusFrequency (
  IN CONST EFI_I2C_ENUMERATE_PROTOCOL *This,
  IN UINTN                            I2cBusConfiguration,
  OUT UINTN                           *BusClockHertz
  );
/**
  Mock SetConfiguration for I2c Platform Dxe.

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
UnitTestSetConfiguration (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_I2C_MASTER_PROTOCOL                              *CentralProtocol;
  I2C_PLATFORM_CONTEXT                                 *Private;
  I2C_FREQUENCY_DATA_TABLE                             *I2cFrequencyTable;
  UINTN                                                BusClockHertz;
  UINT8                                                Index;
  EFI_STATUS                                           Status;
  UINTN                                                I2cBusConfiguration;
  UINTN                                                *Frequency;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));
  //
  // Mock UpdateI2cBusFrequency(), this function updates Bus speed for each controller.
  //
  I2cBusConfiguration = 0;
  I2cFrequencyTable = AllocateZeroPool (sizeof (I2C_FREQUENCY_DATA_TABLE));
  I2cFrequencyTable->Count = MAX_I2C_CONTROLLERS_NUM;
  BusClockHertz = 100 * 1000;
  Frequency = AllocateZeroPool (sizeof (UINTN));
  for (Index = 0; Index < I2cFrequencyTable->Count; Index ++) {
    I2cFrequencyTable->I2cFrequency[Index] = BusClockHertz;
  }

  //
  // Mock boot service OpenProtocol and CloseProtocol
  //
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS->OpenProtocol = MockBootServiceOpenProtocol;
  gBS->CloseProtocol = MockBootServiceCloseProtocol;

  //
  // Mock I2C_PLATFORM_CONTEXT Structure
  //
  Private = AllocateZeroPool (sizeof (I2C_PLATFORM_CONTEXT));
  Private->Signature = I2C_PLATFORM_SIGNATURE;
  Private->Enumerate.GetBusFrequency = GetBusFrequency;

  //
  // Test Function "SetConfiguration"
  // Mock Data
  //  1. Locate gEfiI2cEnumerateProtocolGuid and gEfiI2cMasterProtocolGuid success
  //  2. EnumerateProtocol->GetBusFrequency and CentralProtocol->SetBusFrequency success
  //
  // Expected Result
  // EFI_SUCCESS: 1.if gEfiI2cEnumerateProtocolGuid found, and access GetBusFrequency successfully.
  //              2.if gEfiI2cMasterProtocolGuid found, and access SetBusFrequency successfully.
  //

  //
  // Mock EFI_I2C_ENUMERATE_PROTOCOL
  //
  CentralProtocol = AllocateZeroPool (sizeof (EFI_I2C_MASTER_PROTOCOL));
  CentralProtocol->SetBusFrequency = MockSetBusFrequency;

  //
  // Test1 Locate gEfiI2cEnumerateProtocolGuid unsuccessful.
  //

  //
  // Mock OpenProtocol gEfiI2cEnumerateProtocolGuid: EFI_UNSUPPORTED
  //
  expect_value (MockBootServiceOpenProtocol, Protocol, &gEfiI2cEnumerateProtocolGuid);  // Assume SetConfiguration will locate gEfiI2cEnumerateProtocolGuid
  will_return (MockBootServiceOpenProtocol, EFI_UNSUPPORTED);                           // Mock return Status = EFI_UNSUPPORTED
  will_return (MockBootServiceOpenProtocol, Private);                                   // Mock gBS->OpenProtocol to have GetBusFrequency
  Status = SetConfiguration ( &(Private->BusConfiguration), I2cBusConfiguration, NULL, NULL);

  UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);

  //
  // Test2 Locate gEfiI2cEnumerateProtocolGuid successful but gEfiI2cMasterProtocolGuid unsuccessful.
  //
  for (Index = 0; Index < I2cFrequencyTable->Count; Index++) {
    //
    // Mock OpenProtocol gEfiI2cEnumerateProtocolGuid: EFI_SUCCESS
    //
    expect_value (MockBootServiceOpenProtocol, Protocol, &gEfiI2cEnumerateProtocolGuid);  // Assume SetConfiguration will locate gEfiI2cEnumerateProtocolGuid
    will_return (MockBootServiceOpenProtocol, EFI_SUCCESS);                               // Mock return Status = EFI_SUCCESS
    will_return (MockBootServiceOpenProtocol, &(Private->Enumerate));                     // Mock gBS->OpenProtocol to have GetBusFrequency

    //
    // Mock PcdI2cFrequencyDataMap.
    //
    expect_value (LibPcdGetExPtr, TokenNumber, _PCD_TOKEN_gI2cFeaturePkgTokenSpaceGuid_PcdI2cFrequencyDataMap);
    will_return (LibPcdGetExPtr, I2cFrequencyTable);
    //
    // Mock CloseProtocol
    //
    will_return (MockBootServiceCloseProtocol, EFI_SUCCESS);                              // Mock return Status = EFI_SUCCESS
    //
    // Mock OpenProtocol gEfiI2cMasterProtocolGuid: EFI_UNSUPPORTED
    //
    expect_value (MockBootServiceOpenProtocol, Protocol, &gEfiI2cMasterProtocolGuid);     // Assume SetConfiguration will locate gEfiI2cMasterProtocolGuid
    will_return (MockBootServiceOpenProtocol, EFI_UNSUPPORTED);                           // Mock return Status = EFI_UNSUPPORTED
    will_return (MockBootServiceOpenProtocol, CentralProtocol);                           // Mock gBS->OpenProtocol to have SetBusFrequency
    Status = SetConfiguration ( &(Private->BusConfiguration), I2cBusConfiguration, NULL, NULL);
    UT_ASSERT_EQUAL (Status, EFI_UNSUPPORTED);
  }

  //
  // Test3 Locate gEfiI2cEnumerateProtocolGuid and gEfiI2cMasterProtocolGuid successful.
  //
  for (Index = 0; Index < I2cFrequencyTable->Count; Index++) {
    //
    // Mock OpenProtocol gEfiI2cEnumerateProtocolGuid: EFI_SUCCESS
    //
    expect_value (MockBootServiceOpenProtocol, Protocol, &gEfiI2cEnumerateProtocolGuid);  // Assume SetConfiguration will locate gEfiI2cEnumerateProtocolGuid
    will_return (MockBootServiceOpenProtocol, EFI_SUCCESS);                               // Mock return Status = EFI_SUCCESS
    will_return (MockBootServiceOpenProtocol, &(Private->Enumerate));                     // Mock gBS->OpenProtocol to have GetBusFrequency

    //
    // Mock PcdI2cFrequencyDataMap.
    //
    expect_value (LibPcdGetExPtr, TokenNumber, _PCD_TOKEN_gI2cFeaturePkgTokenSpaceGuid_PcdI2cFrequencyDataMap);
    will_return (LibPcdGetExPtr, I2cFrequencyTable);
    will_return (MockBootServiceCloseProtocol, EFI_SUCCESS);                              // Mock return Status = EFI_SUCCESS

    //
    // Mock OpenProtocol gEfiI2cEnumerateProtocolGuid: EFI_SUCCESS
    //
    expect_value (MockBootServiceOpenProtocol, Protocol, &gEfiI2cMasterProtocolGuid);     // Assume SetConfiguration will locate gEfiI2cMasterProtocolGuid
    will_return (MockBootServiceOpenProtocol, EFI_SUCCESS);                               // Mock return Status = EFI_SUCCESS
    will_return (MockBootServiceOpenProtocol, CentralProtocol);                           // Mock gBS->OpenProtocol to have SetBusFrequency

    //
    // Mock MockSetBusFrequency output parameter Value
    //
    will_return (MockSetBusFrequency, Frequency);
    will_return (MockSetBusFrequency, EFI_SUCCESS);
    will_return (MockBootServiceCloseProtocol, EFI_SUCCESS);                              // Mock return Status = EFI_SUCCESS
    Status = SetConfiguration ( &(Private->BusConfiguration), I2cBusConfiguration, NULL, NULL);
    UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  }

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));

  //
  // Destroy Mock Service
  //
  FreePool (I2cFrequencyTable);
  FreePool (gBS);
  FreePool (Private);
  FreePool (CentralProtocol);
  FreePool (Frequency);
  return UNIT_TEST_PASSED;
}
