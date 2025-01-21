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
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include "TestCaseGetPciDevId.h"
#include <I2cPlatformDxe.h>
#include <BootServicesMock.h>
#include "PrivateMock/PrivateMock.h"
#include "TestCase/GetSocSeries/TestCaseGetSocSeries.h"
#include <Library/UefiBootServicesTableLib.h>
/**
  Get PCI Device ID from device pointed to by handle.
  If valid DeviceID can't be retrieved, 0xFFFF is returned

  @param[in] Handle   device handle

  retval     PCI Device ID
**/
extern
UINT16
GetPciDevId (
  EFI_HANDLE Handle
  );
/**
  Mock GetPciDevId for I2c Platform Dxe.

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
UnitTestGetPciDevId (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINT32                          DevId;
  EFI_PCI_IO_PROTOCOL             *PciIo;
  EFI_HANDLE                      ControllerHandle;
  SOC_SERIES                      SocSeries;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));
  ControllerHandle = NULL;
  //
  // Mock boot service OpenProtocol and CloseProtocol
  //
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  gBS->OpenProtocol = MockBootServiceOpenProtocol;
  gBS->CloseProtocol = MockBootServiceCloseProtocol;
  //
  // Mock MockGetBusFrequency output parameter Value
  //
  PciIo = AllocateZeroPool (sizeof (EFI_PCI_IO_PROTOCOL));
  PciIo->Pci.Read = MockPciIoConfigRead;
  //
  // Test Function "GetPciDevId"
  // Mock Data
  //  1. Locate gEfiPciIoProtocolGuid  success
  //  2. PciIo->Pci.Read success
  //
  // Expected Result
  // EFI_SUCCESS: 1.if gEfiPciIoProtocolGuid found, and access Pci.Read successfully.
  //

  //
  // Test1 Locate gEfiPciIoProtocolGuid unsuccessful.
  //

  //
  // Mock OpenProtocol gEfiPciIoProtocolGuid: EFI_UNSUPPORTED.
  //
  expect_value (MockBootServiceOpenProtocol, Protocol, &gEfiPciIoProtocolGuid);         // Assume GetPciDevId will locate gEfiPciIoProtocolGuid
  will_return (MockBootServiceOpenProtocol, EFI_UNSUPPORTED);                           // Mock return Status = EFI_UNSUPPORTED
  will_return (MockBootServiceOpenProtocol, PciIo);                                     // Mock gBS->OpenProtocol to have MockPciIoConfigRead

  DevId = GetPciDevId (ControllerHandle);
  UT_ASSERT_EQUAL (DevId, 0xFFFF);

  //
  // Test2 Locate gEfiPciIoProtocolGuid successful.
  //

  //
  // Mock OpenProtocol gEfiPciIoProtocolGuid: EFI_SUCCESS.
  //
  expect_value (MockBootServiceOpenProtocol, Protocol, &gEfiPciIoProtocolGuid);         // Assume GetPciDevId will locate gEfiPciIoProtocolGuid
  will_return (MockBootServiceOpenProtocol, EFI_SUCCESS);                               // Mock return Status = EFI_SUCCESS
  will_return (MockBootServiceOpenProtocol, PciIo);                                     // Mock gBS->OpenProtocol to have MockPciIoConfigRead

  //
  // Mock MockPciIoConfigRead output parameter Value
  //
  expect_value (MockPciIoConfigRead, Offset, PCI_VENDOR_ID_OFFSET);

  SocSeries = GetSocSeries ();
  if (SocSeries == SOC_PTL_PCD_P) {
    will_return (MockPciIoConfigRead, PTL_PCD_P_LPSS_I2C_CFG_I2C0_DEVICE_ID);
  } else if (SocSeries == SOC_PTL_PCD_H) {
    will_return (MockPciIoConfigRead, PTL_PCD_H_LPSS_I2C_CFG_I2C0_DEVICE_ID);
  }
  will_return (MockPciIoConfigRead, EFI_SUCCESS);
  //
  // Mock CloseProtocol
  //
  will_return (MockBootServiceCloseProtocol, EFI_SUCCESS);                              // Mock return Status = EFI_SUCCESS
  DevId = GetPciDevId (ControllerHandle);
  if (SocSeries == SOC_PTL_PCD_P) {
    UT_ASSERT_EQUAL (DevId, PTL_PCD_P_LPSS_I2C_CFG_I2C0_DEVICE_ID);
  } else if (SocSeries == SOC_PTL_PCD_H) {
    UT_ASSERT_EQUAL (DevId, PTL_PCD_H_LPSS_I2C_CFG_I2C0_DEVICE_ID);
  }

  //
  // Destroy Mock Service
  //
  FreePool (PciIo);
  FreePool (gBS);
  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
  return UNIT_TEST_PASSED;
}
