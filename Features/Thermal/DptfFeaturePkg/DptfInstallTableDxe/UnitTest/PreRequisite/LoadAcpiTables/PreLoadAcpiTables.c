/** @file
    PreRequisite functions of the LoadAcpiTables function.
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
#include "PreLoadAcpiTables.h"
#include "PrivateMock.h"
#include <Library/MemoryAllocationLib.h>

/**
  Set up services for LoadAcpiTables function.
  @retval     EFI_SUCCESS                 Environment set up success.
**/
EFI_STATUS
EFIAPI
SetupLoadAcpiTablesEnvironment (
  )
{
  EFI_STATUS                     Status;
  EFI_ACPI_COMMON_HEADER         *DptfTable;
  UINTN                          DptfBufferSize;
  VOID                           *HandleBuffer;

  //
  // Load DptfTable from build file
  //
  will_return (MockAcpiTableFile, "Build/DptfFeaturePkg/HostTest/NOOPT_VS2019/X64/DptfFeaturePkg/AcpiTables/DptfAcpiTables/OUTPUT/Dptf.aml");
  Status = MockAcpiTableFile((UINT8**) &DptfTable, &DptfBufferSize);
  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  //
  // Mock boot service
  //
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  if (gBS == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  gBS->LocateProtocol = MockBootServiceLocateProtocol;
  gBS->LocateHandleBuffer = MockBootServiceLocateHandleBuffer;
  gBS->HandleProtocol = MockBootServiceHandleProtocol;
  gBS->FreePool = MockBootServiceFreePool;

  //
  // Mock protocol gEfiFirmwareVolume2ProtocolGuid
  //
  gFwVol = AllocateZeroPool (sizeof (EFI_FIRMWARE_VOLUME2_PROTOCOL));
  if (gFwVol == NULL) {
    FreePool (gBS);
    return EFI_OUT_OF_RESOURCES;
  }
  gFwVol->ReadFile = MockFwVol2ReadFile;
  gFwVol->ReadSection = MockFwVol2ReadSection;

  //
  // Mock protocol gEfiAcpiTableProtocolGuid
  //
  gAcpiTableProtocol = AllocateZeroPool (sizeof (EFI_ACPI_TABLE_PROTOCOL));
  if (gAcpiTableProtocol == NULL) {
    FreePool (gBS);
    FreePool (gFwVol);
    return EFI_OUT_OF_RESOURCES;
  }
  gAcpiTableProtocol->InstallAcpiTable = MockInstallAcpiTable;

  // Mock function for gBS->LocateHandleBuffer
  HandleBuffer = AllocateZeroPool (0x1000);
  if (HandleBuffer == NULL) {
    FreePool (gBS);
    FreePool (gFwVol);
    FreePool (gAcpiTableProtocol);
    return EFI_OUT_OF_RESOURCES;
  }
  expect_value (MockBootServiceLocateHandleBuffer, SearchType, ByProtocol);
  expect_value (MockBootServiceLocateHandleBuffer, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  expect_value (MockBootServiceLocateHandleBuffer, SearchKey, NULL);
  expect_any (MockBootServiceLocateHandleBuffer, NumberHandles);
  expect_any (MockBootServiceLocateHandleBuffer, Buffer);
  will_return (MockBootServiceLocateHandleBuffer, 0xFF);
  will_return (MockBootServiceLocateHandleBuffer, HandleBuffer);
  will_return (MockBootServiceLocateHandleBuffer, EFI_SUCCESS);

  // Mock function for gBS->HandleProtocol
  expect_any (MockBootServiceHandleProtocol, UserHandle);
  expect_value (MockBootServiceHandleProtocol, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  expect_any (MockBootServiceHandleProtocol, Interface);
  will_return (MockBootServiceHandleProtocol, gFwVol);
  will_return (MockBootServiceHandleProtocol, EFI_SUCCESS);

  // Mock function for FwVol->ReadFile
  expect_value (MockFwVol2ReadFile, This, gFwVol);
  expect_value (MockFwVol2ReadFile, NameGuid, &gDptfAcpiTableStorageGuid);
  expect_any (MockFwVol2ReadFile, Buffer);
  expect_any (MockFwVol2ReadFile, BufferSize);
  expect_any (MockFwVol2ReadFile, FoundType);
  expect_any (MockFwVol2ReadFile, FileAttributes);
  expect_any (MockFwVol2ReadFile, AuthenticationStatus);
  will_return (MockFwVol2ReadFile, EFI_SUCCESS);

  // Mock function for gBS->LocateProtocol
  expect_value (MockBootServiceLocateProtocol, Protocol, &gEfiAcpiTableProtocolGuid);
  expect_value (MockBootServiceLocateProtocol, Registration, NULL);
  expect_any (MockBootServiceLocateProtocol, Interface);
  will_return (MockBootServiceLocateProtocol, gAcpiTableProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);

  // Mock function for FwVol->ReadSection
  expect_value (MockFwVol2ReadSection, This, gFwVol);
  expect_value (MockFwVol2ReadSection, NameGuid, &gDptfAcpiTableStorageGuid);
  expect_any (MockFwVol2ReadSection, SectionType);
  expect_any (MockFwVol2ReadSection, SectionInstance);
  expect_any (MockFwVol2ReadSection, Buffer);
  expect_any (MockFwVol2ReadSection, BufferSize);
  expect_any (MockFwVol2ReadSection, AuthenticationStatus);
  will_return (MockFwVol2ReadSection, DptfTable);
  will_return (MockFwVol2ReadSection, DptfBufferSize);
  will_return (MockFwVol2ReadSection, EFI_SUCCESS);

  // Mock function for AcpiTable->InstallAcpiTable
  expect_value (MockInstallAcpiTable, This, gAcpiTableProtocol);
  expect_any (MockInstallAcpiTable, AcpiTableBuffer);
  expect_any (MockInstallAcpiTable, AcpiTableBufferSize);
  expect_any (MockInstallAcpiTable, TableKey);
  will_return (MockInstallAcpiTable, &gSsdtTargetTable);
  will_return (MockInstallAcpiTable, EFI_SUCCESS);

  // Mock function for gBS->FreePool
  will_return (MockBootServiceFreePool, EFI_SUCCESS);

  // Mock function for FwVol->ReadSection
  expect_value (MockFwVol2ReadSection, This, gFwVol);
  expect_value (MockFwVol2ReadSection, NameGuid, &gDptfAcpiTableStorageGuid);
  expect_any (MockFwVol2ReadSection, SectionType);
  expect_any (MockFwVol2ReadSection, SectionInstance);
  expect_any (MockFwVol2ReadSection, Buffer);
  expect_any (MockFwVol2ReadSection, BufferSize);
  expect_any (MockFwVol2ReadSection, AuthenticationStatus);
  will_return (MockFwVol2ReadSection, NULL);
  will_return (MockFwVol2ReadSection, 0);
  will_return (MockFwVol2ReadSection, EFI_NOT_FOUND);


  return EFI_SUCCESS;
}

/**
  Set up the environment and execute LoadAcpiTables function.
  @param[in]  Context                                The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED                       Set up success.
  @retval     UNIT_TEST_ERROR_PREREQUISITE_NOT_MET   Fail to set up environment.
**/
UNIT_TEST_STATUS
EFIAPI
PreLoadAcpiTables (
  IN UNIT_TEST_CONTEXT        Context
  )
{
  EFI_STATUS  Status;

  Status = SetupLoadAcpiTablesEnvironment ();
  if (Status != EFI_SUCCESS) {
    return UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
  }

  //
  // Running platform function
  //
  gReturnResult = LoadAcpiTables ();

  return UNIT_TEST_PASSED;
}
