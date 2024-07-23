/**@file
  Unit test case of the I2C Device Common Driver

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

@par Specification Reference:
**/

#include "PreGetDevTablePointer.h"

#define PRE_FUNCTION_NAME     "PreGetDevTablePointer"
#define PRE_FUNCTION_VERSION  "0.1"

/**
  Set up services for GetDevTablePointer function.

  @retval EFI_SUCCESS  Set up success.

**/
EFI_STATUS
EFIAPI
SetupGetDevTablePointerEnvironment (
  VOID
  )
{
  EFI_STATUS                     Status;
  EFI_ACPI_COMMON_HEADER         *CurrentTable;
  UINTN                          SsdtBufferSize;
  UINTN                          NumberOfHandles = 1;
  EFI_FIRMWARE_VOLUME2_PROTOCOL  *FwVol2;
  UINTN                          Size = 0;
  UINT64                         PcdData = SIGNATURE_64 ('I','2','C','_','D','E','V','T');
  EFI_EVENT                      EndOfDxeEvent;

  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  UT_ASSERT_NOT_NULL (gBS);

  //
  // Mock I2C ACPI table
  //
  will_return (MockI2cAcpiTableFile, "Build/I2cFeaturePkg/HostTest/NOOPT_VS2019/X64/I2cFeaturePkg/I2cTouchAcpiDxe/I2cTouchCommon/UnitTest/I2cCommonHost/OUTPUT/I2cFeaturePkg/I2cTouchAcpiDxe/I2cTouchCommon/I2cSsdt.aml");
  Status = MockI2cAcpiTableFile ((UINT8**) &CurrentTable, &SsdtBufferSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "MockI2cAcpiTableFile Fail!!\n"));
    UT_ASSERT_FALSE (TRUE);
  }

  //
  // Mock boot service LocateProtocol
  //
  gBS->LocateProtocol = MockBootServiceLocateProtocol;

  mAcpiTable = AllocateZeroPool (sizeof (EFI_ACPI_TABLE_PROTOCOL));
  UT_ASSERT_NOT_NULL (mAcpiTable);

  expect_value (MockBootServiceLocateProtocol, Protocol, &gEfiAcpiTableProtocolGuid);
  will_return (MockBootServiceLocateProtocol, mAcpiTable);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);

  //
  // Mock boot service LocateHandleBuffer
  //
  gBS->LocateHandleBuffer = MockBootServiceLocateHandleBuffer;

  expect_value (MockBootServiceLocateHandleBuffer, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  will_return (MockBootServiceLocateHandleBuffer, EFI_SUCCESS);
  will_return (MockBootServiceLocateHandleBuffer, NumberOfHandles);

  //
  // Mock boot service HandleProtocol
  //
  gBS->HandleProtocol = MockBootServiceHandleProtocol;

  FwVol2 = AllocateZeroPool (sizeof (EFI_FIRMWARE_VOLUME2_PROTOCOL));
  UT_ASSERT_NOT_NULL (FwVol2);

  expect_value (MockBootServiceHandleProtocol, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  will_return (MockBootServiceHandleProtocol, FwVol2);
  will_return (MockBootServiceHandleProtocol, EFI_SUCCESS);

  //
  // Mock the FwVol2->ReadFile
  //
  FwVol2->ReadFile = MockFwVol2ReadFile;
  will_return (MockFwVol2ReadFile, Size);
  will_return (MockFwVol2ReadFile, EFI_SUCCESS);

  //
  // Mock the FwVol2->ReadSection
  //
  FwVol2->ReadSection = MockFwVol2ReadSection;

  expect_value (MockFwVol2ReadSection, SectionType, EFI_SECTION_RAW);
  will_return (MockFwVol2ReadSection, CurrentTable);
  will_return (MockFwVol2ReadSection, SsdtBufferSize);
  will_return (MockFwVol2ReadSection, EFI_SUCCESS);

  //
  // Mock PcdGet64
  //
  expect_value (LibPcdGet64, TokenNumber, _PCD_TOKEN_PcdI2cDevTableName);
  will_return (LibPcdGet64, PcdData);

  //
  // Mock boot service CreateEventEx
  //
  gBS->CreateEventEx = MockBootServiceCreateEventEx;

  EndOfDxeEvent = AllocateZeroPool (sizeof (EFI_EVENT));
  UT_ASSERT_NOT_NULL (EndOfDxeEvent);

  expect_value (MockBootServiceCreateEventEx, Type, EVT_NOTIFY_SIGNAL);
  expect_value (MockBootServiceCreateEventEx, NotifyTpl, TPL_CALLBACK);
  expect_value (MockBootServiceCreateEventEx, NotifyFunction, InstallI2cTable);
  expect_value (MockBootServiceCreateEventEx, EventGroup, &gEfiEndOfDxeEventGroupGuid);
  will_return (MockBootServiceCreateEventEx, &EndOfDxeEvent);
  will_return (MockBootServiceCreateEventEx, EFI_SUCCESS);

  return EFI_SUCCESS;
}

/**
  Set up the environment and execute GetDevTablePointer function.

  @param[in] Context                            The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Fail to set up environment.

**/
UNIT_TEST_STATUS
EFIAPI
PreGetDevTablePointer (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "[PreRequisite] %a v%a\n", PRE_FUNCTION_NAME, PRE_FUNCTION_VERSION));

  Status = SetupGetDevTablePointerEnvironment ();
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //
  // Execute test function with above mock environment
  //
  GetDevTablePointer ();

  return UNIT_TEST_PASSED;
}

