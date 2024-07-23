/**@file
  Unit test case of the I2c Touch Device Acpi Driver

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

#include "PrePatchI2cDevicesBuffer.h"

#define PRE_FUNCTION_NAME     "PrePatchI2cDevicesBuffer"
#define PRE_FUNCTION_VERSION  "0.1"

EFI_ACPI_DESCRIPTION_HEADER  *gI2cDevicesAcpiTable = NULL;
I2C_DEVICES                  *gMockI2cTPTD = NULL;

/**
  Set up services for PatchI2cDevicesBuffer function.

  @retval EFI_SUCCESS  Set up success.

**/
EFI_STATUS
EFIAPI
SetupPatchI2cDevicesBufferEnvironment (
  VOID
  )
{
  EFI_STATUS  Status;
  UINTN       SsdtBufferSize;

  //
  // Mock I2C ACPI table
  //
  will_return (MockI2cAcpiTableFile, "Build/I2cFeaturePkg/HostTest/NOOPT_VS2019/X64/I2cFeaturePkg/I2cTouchAcpiDxe/I2cTouchCommon/UnitTest/I2cCommonHost/OUTPUT/I2cFeaturePkg/I2cTouchAcpiDxe/I2cTouchCommon/I2cSsdt.aml");
  Status = MockI2cAcpiTableFile ((UINT8**) &gI2cDevicesAcpiTable, &SsdtBufferSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "MockI2cAcpiTableFile Fail!!\n"));
    UT_ASSERT_FALSE (TRUE);
  }

  gMockI2cTPTD = AllocateZeroPool (sizeof (I2C_DEVICES));
  UT_ASSERT_NOT_NULL (gMockI2cTPTD);

  //
  // Initial gMockI2cTPTD content.
  //
  gMockI2cTPTD->TouchPanelNvsVar.TouchPanelType       = 0x07;
  gMockI2cTPTD->TouchPanelNvsVar.TouchPanelIrqMode    = 0x01;
  gMockI2cTPTD->TouchPanelNvsVar.TouchPanelBusAddress = 0x7F;
  gMockI2cTPTD->TouchPanelNvsVar.TouchPanelSpeed      = 0x02;
  gMockI2cTPTD->TouchPadNvsVar.TouchPadType           = 0x06;
  gMockI2cTPTD->TouchPadNvsVar.TouchPadIrqMode        = 0x01;
  gMockI2cTPTD->TouchPadNvsVar.TouchPadBusAddress     = 0x7F;
  gMockI2cTPTD->TouchPadNvsVar.TouchPadSpeed          = 0x02;
  gMockI2cTPTD->TouchPanelNvsVar.TouchPanelHidAddress = 0xFFFF;
  gMockI2cTPTD->TouchPadNvsVar.TouchPadHidAddress     = 0xFFFF;
  gMockI2cTPTD->TouchPadNvsVar.TouchpadIrqGpio        = 0xFFFFFFFF;
  gMockI2cTPTD->TouchPanelNvsVar.TouchpanelIrqGpio    = 0xFFFFFFFF;
  gMockI2cTPTD->TouchPanelNvsVar.Touchpanel0IrqGpio   = 0xFFFFFFFF;

  return EFI_SUCCESS;
}

/**
  Set up the environment and execute PatchI2cDevicesBuffer function.

  @param[in] Context                            The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Fail to set up environment.

**/
UNIT_TEST_STATUS
EFIAPI
PrePatchI2cDevicesBuffer (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "[PreRequisite] %a v%a\n", PRE_FUNCTION_NAME, PRE_FUNCTION_VERSION));

  Status = SetupPatchI2cDevicesBufferEnvironment ();
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //
  // Execute test function with above mock environment
  //
  PatchI2cDevicesBuffer (gI2cDevicesAcpiTable, gMockI2cTPTD);

  return UNIT_TEST_PASSED;
}

