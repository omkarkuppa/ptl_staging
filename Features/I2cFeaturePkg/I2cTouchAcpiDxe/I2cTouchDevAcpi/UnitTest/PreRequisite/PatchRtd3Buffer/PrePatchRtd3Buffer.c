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

#include "PrePatchRtd3Buffer.h"

#define PRE_FUNCTION_NAME     "PrePatchRtd3Buffer"
#define PRE_FUNCTION_VERSION  "0.1"

EFI_ACPI_DESCRIPTION_HEADER  *gI2cRtd3AcpiTable = NULL;
I2C_ACPI_DEV                 gMockI2cControllerInfo[MAX_I2C_ROOTPORT_ENTRY_SUPPORT];


/**
  Set up services for PatchRtd3Buffer function.

  @retval EFI_SUCCESS  Set up success.

**/
EFI_STATUS
EFIAPI
SetupPatchRtd3BufferEnvironment (
  VOID
  )
{
  EFI_STATUS  Status;
  UINTN       SsdtBufferSize;
  UINT8       I2cIndex;

  //
  // Mock I2C ACPI table
  //
  will_return (MockI2cAcpiTableFile, "Build/I2cFeaturePkg/HostTest/NOOPT_VS2019/X64/I2cFeaturePkg/I2cTouchAcpiDxe/I2cTouchCommon/UnitTest/I2cCommonHost/OUTPUT/I2cFeaturePkg/I2cTouchAcpiDxe/I2cTouchCommon/I2cSsdt.aml");
  Status = MockI2cAcpiTableFile ((UINT8**) &gI2cRtd3AcpiTable, &SsdtBufferSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "MockI2cAcpiTableFile Fail!!\n"));
    UT_ASSERT_FALSE (TRUE);
  }

  for (I2cIndex = 0; I2cIndex < MAX_I2C_ROOTPORT_ENTRY_SUPPORT; I2cIndex++) {
    //
    // Initial gMockI2cControllerInfo content.
    //
    gMockI2cControllerInfo[I2cIndex].I2cControllerEnabled                           = 0x01;
    gMockI2cControllerInfo[I2cIndex].I2cDeviceType                                  = 0x03;
    gMockI2cControllerInfo[I2cIndex].TouchPadInfo.TouchPadInterruptGpio             = 0xFFFFFFFF;
    gMockI2cControllerInfo[I2cIndex].TouchPadInfo.TouchPadInterruptGpioPolarity     = TRUE;
    gMockI2cControllerInfo[I2cIndex].TouchPanelInfo.TouchPanelInterruptGpio         = 0xFFFFFFFF;
    gMockI2cControllerInfo[I2cIndex].TouchPanelInfo.TouchPanelInterruptGpioPolarity = TRUE;
    gMockI2cControllerInfo[I2cIndex].TouchPanelInfo.TouchpanelPwrGpio               = 0xFFFFFFFF;
    gMockI2cControllerInfo[I2cIndex].TouchPanelInfo.TouchpanelPwrGpioPolarity       = TRUE;
    gMockI2cControllerInfo[I2cIndex].TouchPanelInfo.TouchPanelResetGpio             = 0xFFFFFFFF;
    gMockI2cControllerInfo[I2cIndex].TouchPanelInfo.TouchPanelResetGpioPolarity     = TRUE;
  }

  return EFI_SUCCESS;
}

/**
  Set up the environment and execute PatchRtd3Buffer function.

  @param[in] Context                            The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Fail to set up environment.

**/
UNIT_TEST_STATUS
EFIAPI
PrePatchRtd3Buffer (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;
  UINT8       Count = MAX_I2C_ROOTPORT_ENTRY_SUPPORT;

  DEBUG ((DEBUG_INFO, "[PreRequisite] %a v%a\n", PRE_FUNCTION_NAME, PRE_FUNCTION_VERSION));

  Status = SetupPatchRtd3BufferEnvironment ();
  UT_ASSERT_NOT_EFI_ERROR (Status);

  //
  // Execute test function with above mock environment
  //
  PatchRtd3Buffer (gI2cRtd3AcpiTable, gMockI2cControllerInfo, Count);

  return UNIT_TEST_PASSED;
}

