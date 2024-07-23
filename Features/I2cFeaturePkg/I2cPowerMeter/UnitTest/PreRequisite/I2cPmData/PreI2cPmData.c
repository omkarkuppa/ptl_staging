/**@file
  PreRequisite functions of the I2cPmData function.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#include "PreI2cPmData.h"
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <BootServicesMock.h>


EFI_ACPI_DESCRIPTION_HEADER   *gIC2PmDataTable = NULL;
EFI_ACPI_DESCRIPTION_HEADER   *gVerifyTable = NULL;
EFI_ACPI_TABLE_PROTOCOL       *gAcpiTableProtocol = NULL;
EFI_FIRMWARE_VOLUME2_PROTOCOL *gFwVol = NULL;
UINTN                          gVerifyTableSize;

POWER_METER_PKG               *PowerMtrData = NULL;
UINT8                         PowerMeterEnable;

/**
  Set up services for I2cPmData function.

  @retval EFI_SUCCESS  Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupI2cPmDataEnvironment (
  VOID
  )
{
  EFI_STATUS                     Status;
  UINTN                          I2cPmDataBufferSize;

  //
  // Input parameter initialization
  //
  UT_ASSERT_TRUE (GenerateRandomPowerMtrData (&PowerMtrData));
  UT_ASSERT_TRUE (RandomData (&PowerMeterEnable, sizeof (UINT8), FALSE));

  //
  // Load Pmax AcpiTables from building file.
  //
  will_return (MockI2cPmDataAcpiTableFile, "Build/I2cFeaturePkg/HostTest/NOOPT_VS2019/X64/I2cFeaturePkg/I2cPowerMeter/I2cPowerMeterAcpiTable/OUTPUT/I2cPowerMeter.aml");
  Status = MockI2cPmDataAcpiTableFile ((UINT8**) &gIC2PmDataTable, &I2cPmDataBufferSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Can't mock I2cPm ACPI table!!\n"));
    return Status;
  }
  //
  // Mock boot service
  //
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  if (gBS == NULL) {
    DEBUG ((DEBUG_ERROR, "Can't locate memory for mock gBS!!\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  gBS->LocateProtocol     = MockBootServiceLocateProtocol;
  gBS->LocateHandleBuffer = MockBootServiceLocateHandleBuffer;
  gBS->HandleProtocol     = MockBootServiceHandleProtocol;

  //
  // Mock protocol gEfiFirmwareVolume2ProtocolGuid
  //
  gFwVol = AllocateZeroPool (sizeof (EFI_FIRMWARE_VOLUME2_PROTOCOL));
  if (gFwVol == NULL) {
    DEBUG ((DEBUG_ERROR, "Can't locate memory for mock Firmware Volume 2 protocol.\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  gFwVol->ReadFile    = MockFwVol2ReadFile;
  gFwVol->ReadSection = MockFwVol2ReadSection;

  //
  // Mock AcpiTableProtocol
  //
  gAcpiTableProtocol = AllocateZeroPool (sizeof (EFI_ACPI_TABLE_PROTOCOL));
  if (gAcpiTableProtocol == NULL) {
    DEBUG ((DEBUG_ERROR, "Can't locate memory for ACPI protocol!!\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  gAcpiTableProtocol->InstallAcpiTable = MockInstallAcpiTable;

  //
  // Mock function for gBS->LocateProtocol
  //
  expect_value (MockBootServiceLocateProtocol, Protocol, &gEfiAcpiTableProtocolGuid);
  will_return (MockBootServiceLocateProtocol, gAcpiTableProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);

  //
  // Mock function for gBS->LocateHandleBuffer
  //
  expect_value (MockBootServiceLocateHandleBuffer, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  will_return (MockBootServiceLocateHandleBuffer, EFI_SUCCESS);
  will_return (MockBootServiceLocateHandleBuffer, 0xFF);

  //
  // Mock function for gBS->HandleProtocol
  //
  expect_value (MockBootServiceHandleProtocol, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  will_return (MockBootServiceHandleProtocol, gFwVol);
  will_return (MockBootServiceHandleProtocol, EFI_SUCCESS);

  //
  // Mock function for FwVol->ReadFile
  //
  expect_value (MockFwVol2ReadFile, This, gFwVol);
  expect_value (MockFwVol2ReadFile, NameGuid, &gEfiCallerIdGuid);
  expect_any (MockFwVol2ReadFile, Buffer);
  expect_any (MockFwVol2ReadFile, BufferSize);
  expect_any (MockFwVol2ReadFile, FoundType);
  expect_any (MockFwVol2ReadFile, FileAttributes);
  expect_any (MockFwVol2ReadFile, AuthenticationStatus);
  will_return (MockFwVol2ReadFile, EFI_SUCCESS);

  //
  // Mock function for FwVol->ReadSection
  //
  expect_value (MockFwVol2ReadSection, This, gFwVol);
  expect_value (MockFwVol2ReadSection, NameGuid, &gEfiCallerIdGuid);
  expect_any (MockFwVol2ReadSection, SectionType);
  expect_any (MockFwVol2ReadSection, SectionInstance);
  expect_any (MockFwVol2ReadSection, Buffer);
  expect_any (MockFwVol2ReadSection, BufferSize);
  expect_any (MockFwVol2ReadSection, AuthenticationStatus);
  will_return (MockFwVol2ReadSection, gIC2PmDataTable);
  will_return (MockFwVol2ReadSection, I2cPmDataBufferSize);
  will_return (MockFwVol2ReadSection, EFI_SUCCESS);

  //
  // Mock function for AcpiTable->InstallAcpiTable
  //
  will_return (MockInstallAcpiTable, &gVerifyTable);

  return Status;
}

/**
  Set up the environment and execute I2cPmData function.

  @param[in] Context                            The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreI2cPmData (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;

  Status = SetupI2cPmDataEnvironment ();
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //
  // Running platform function
  //
  Status = I2cPmData (PowerMtrData, PowerMeterEnable);

  return UNIT_TEST_PASSED;
}