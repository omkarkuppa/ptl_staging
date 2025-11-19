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

#include "TestCasePatchRtd3Buffer.h"

#define UNIT_TEST_FUNCTION_NAME     "UnitTestPatchRtd3Buffer"
#define UNIT_TEST_FUNCTION_VERSION  "0.1"

/**
  Check the result of test case with PatchRtd3Buffer.

  @param[in] Context                   The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED             The result is correct.
  @retval UNIT_TEST_ERROR_TEST_FAILED  The result is wrong.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestPatchRtd3Buffer (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINT8   *I2cStartPointer;
  UINT8   *I2cEndPointer;
  UINT32  *Signature;
  UINT8   I2cIndex;
  UINT8   Count = MAX_I2C_ROOTPORT_ENTRY_SUPPORT;

  DEBUG ((DEBUG_INFO, "[Test Case] %a v%a\n", UNIT_TEST_FUNCTION_NAME, UNIT_TEST_FUNCTION_VERSION));

  I2cStartPointer = (UINT8 *) gI2cRtd3AcpiTable;
  I2cEndPointer = I2cStartPointer + gI2cRtd3AcpiTable->Length;
  for (; I2cStartPointer <= I2cEndPointer; I2cStartPointer++) {
    Signature = (UINT32 *) (I2cStartPointer + 1);
    if ((*I2cStartPointer == AML_NAME_OP) && (*Signature == SIGNATURE_32 ('I','2','B','U'))) {
      I2cStartPointer = I2cStartPointer + I2C_BUFFER_START_LOCATION;
      for (I2cIndex =0; I2cIndex < Count ; I2cIndex ++) {
        if (I2cIndex == gMockI2cControllerInfo[I2cIndex].I2cControllerTouchIndex ) {
          UT_ASSERT_EQUAL (*( (UINT8  *) (I2cStartPointer + ((I2cIndex * I2C_BUFFER_SIZE) + I2C_ENABLE_BYTE_OFFSET))),                     gMockI2cControllerInfo[I2cIndex].I2cControllerEnabled);
          UT_ASSERT_EQUAL (*( (UINT8  *) (I2cStartPointer + ((I2cIndex * I2C_BUFFER_SIZE) + END_POINT_PRESENT_BYTE_OFFSET))),              gMockI2cControllerInfo[I2cIndex].I2cDeviceType);
          UT_ASSERT_EQUAL (*( (UINT32 *) (I2cStartPointer + ((I2cIndex * I2C_BUFFER_SIZE) + TOUCH_PAD_INTERRUPT_GPIO_DWORD_OFFSET))),      gMockI2cControllerInfo[I2cIndex].TouchPadInfo.TouchPadInterruptGpio);
          UT_ASSERT_EQUAL (*( (UINT8  *) (I2cStartPointer + ((I2cIndex * I2C_BUFFER_SIZE) + TOUCH_PAD_INTERRUPT_POLARITY_BYTE_OFFSET))),   gMockI2cControllerInfo[I2cIndex].TouchPadInfo.TouchPadInterruptGpioPolarity);
          UT_ASSERT_EQUAL (*( (UINT32 *) (I2cStartPointer + ((I2cIndex * I2C_BUFFER_SIZE) + TOUCH_PANEL_INTERRUPT_GPIO_DWORD_OFFSET))),    gMockI2cControllerInfo[I2cIndex].TouchPanelInfo.TouchPanelInterruptGpio);
          UT_ASSERT_EQUAL (*( (UINT8  *) (I2cStartPointer + ((I2cIndex * I2C_BUFFER_SIZE) + TOUCH_PANEL_INTERRUPT_POLARITY_BYTE_OFFSET))), gMockI2cControllerInfo[I2cIndex].TouchPanelInfo.TouchPanelInterruptGpioPolarity);
          UT_ASSERT_EQUAL (*( (UINT32 *) (I2cStartPointer + ((I2cIndex * I2C_BUFFER_SIZE) + TOUCH_PANEL_POWER_GPIO_DWORD_OFFSET))),        gMockI2cControllerInfo[I2cIndex].TouchPanelInfo.TouchpanelPwrGpio);
          UT_ASSERT_EQUAL (*( (UINT8  *) (I2cStartPointer + ((I2cIndex * I2C_BUFFER_SIZE) + TOUCH_PANEL_POWER_POLARITY_BYTE_OFFSET))),     gMockI2cControllerInfo[I2cIndex].TouchPanelInfo.TouchpanelPwrGpioPolarity);
          UT_ASSERT_EQUAL (*( (UINT32 *) (I2cStartPointer + ((I2cIndex * I2C_BUFFER_SIZE) + TOUCH_PANEL_RESET_GPIO_DWORD_OFFSET))),        gMockI2cControllerInfo[I2cIndex].TouchPanelInfo.TouchPanelResetGpio);
          UT_ASSERT_EQUAL (*( (UINT8  *) (I2cStartPointer + ((I2cIndex * I2C_BUFFER_SIZE) + TOUCH_PANEL_RESET_POLARITY_BYTE_OFFSET))),     gMockI2cControllerInfo[I2cIndex].TouchPanelInfo.TouchPanelResetGpioPolarity);
        }
      }
    }
  }

  DEBUG ((DEBUG_INFO, "Testing result is pass.\n"));

  return UNIT_TEST_PASSED;
}
