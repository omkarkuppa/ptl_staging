/**@file
  Unit test case of the Mipi Camera Install Table Dxe driver.

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

#include "TestCaseInitializeMipiCam.h"

#define UNIT_TEST_FUNCTION_NAME     "UnitTestInitializeMipiCam"
#define UNIT_TEST_FUNCTION_VERSION  "0.1"

/**
  Check the result of test case with InitializeMipiCam.

  @param[in] Context                   The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED             The result is correct.
  @retval UNIT_TEST_ERROR_TEST_FAILED  The result is wrong.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestInitializeMipiCam (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINTN   Index;
  UINT16  MipiModuleName[MIPICAM_MODULE_NAME_LENGTH];
  UINT16  MipiHidName[MIPICAM_HID_LENGTH];

  DEBUG ((DEBUG_INFO, "[Test Case] %a v%a\n", UNIT_TEST_FUNCTION_NAME, UNIT_TEST_FUNCTION_VERSION));

  //
  // MipiCam sensor configuration
  //
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic0, gMipiCamSetupData->MipiCam_ControlLogic0);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic1, gMipiCamSetupData->MipiCam_ControlLogic1);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic2, gMipiCamSetupData->MipiCam_ControlLogic2);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic3, gMipiCamSetupData->MipiCam_ControlLogic3);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic4, gMipiCamSetupData->MipiCam_ControlLogic4);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic5, gMipiCamSetupData->MipiCam_ControlLogic5);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0Enabled, gMipiCamSetupData->MipiCam_Link0);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1Enabled, gMipiCamSetupData->MipiCam_Link1);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2Enabled, gMipiCamSetupData->MipiCam_Link2);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3Enabled, gMipiCamSetupData->MipiCam_Link3);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4Enabled, gMipiCamSetupData->MipiCam_Link4);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5Enabled, gMipiCamSetupData->MipiCam_Link5);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLanesClkDiv, gMipiCamSetupData->MipiCam_LanesClkDiv);

  //
  // MipiCam Control Logic 0 specific
  //
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Version, 1);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Type, gMipiCamSetupData->MipiCam_ControlLogic0_Type);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_CrdVersion, gMipiCamSetupData->MipiCam_ControlLogic0_CrdVersion);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_InputClock, gMipiCamSetupData->MipiCam_ControlLogic0_InputClock);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioPinsEnabled, gMipiCamSetupData->MipiCam_ControlLogic0_GpioPinsEnabled);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_I2cBus, gMipiCamSetupData->MipiCam_ControlLogic0_I2cChannel);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_I2cAddress, gMipiCamSetupData->MipiCam_ControlLogic0_I2cAddress);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Pld, gMipiCamSetupData->MipiCam_ControlLogic0_Pld);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled1FlashMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic0_Wled1FlashMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled1TorchMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic0_Wled1TorchMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled2FlashMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic0_Wled2FlashMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled2TorchMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic0_Wled2TorchMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled1Type, gMipiCamSetupData->MipiCam_ControlLogic0_Wled1Type);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled2Type, gMipiCamSetupData->MipiCam_ControlLogic0_Wled2Type);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_PchClockSource, gMipiCamSetupData->MipiCam_ControlLogic0_PchClockSource);

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    if (Index < gMipiCamSetupData->MipiCam_ControlLogic0_GpioPinsEnabled) {
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioGroupPadNumber[Index], gMipiCamSetupData->MipiCam_ControlLogic0_GpioGroupPadNumber[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioComNumber[Index], gMipiCamSetupData->MipiCam_ControlLogic0_GpioComNumber[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioGroupNumber[Index], gMipiCamSetupData->MipiCam_ControlLogic0_GpioGroupNumber[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioFunction[Index], gMipiCamSetupData->MipiCam_ControlLogic0_GpioFunction[Index]);
      if (gMipiCamSetupData->MipiCam_ControlLogic0_GpioFunction[Index] == 0xFF) {
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioFunction[Index], gMipiCamSetupData->MipiCam_ControlLogic0_GpioFunction_User[Index]);
      }
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioActiveValue[Index], gMipiCamSetupData->MipiCam_ControlLogic0_GpioActiveValue[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioInitialValue[Index], gMipiCamSetupData->MipiCam_ControlLogic0_GpioInitialValue[Index]);
    } else {
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioGroupPadNumber[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioGroupNumber[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioComNumber[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioFunction[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioActiveValue[Index], 1);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioInitialValue[Index], 0);
    }
  }

  //
  // MipiCam Control Logic 1 specific
  //
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Version, 1);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Type, gMipiCamSetupData->MipiCam_ControlLogic1_Type);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_CrdVersion, gMipiCamSetupData->MipiCam_ControlLogic1_CrdVersion);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_InputClock, gMipiCamSetupData->MipiCam_ControlLogic1_InputClock);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioPinsEnabled, gMipiCamSetupData->MipiCam_ControlLogic1_GpioPinsEnabled);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_I2cBus, gMipiCamSetupData->MipiCam_ControlLogic1_I2cChannel);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_I2cAddress, gMipiCamSetupData->MipiCam_ControlLogic1_I2cAddress);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Pld, gMipiCamSetupData->MipiCam_ControlLogic1_Pld);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled1FlashMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic1_Wled1FlashMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled1TorchMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic1_Wled1TorchMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled2FlashMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic1_Wled2FlashMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled2TorchMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic1_Wled2TorchMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled1Type, gMipiCamSetupData->MipiCam_ControlLogic1_Wled1Type);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled2Type, gMipiCamSetupData->MipiCam_ControlLogic1_Wled2Type);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_PchClockSource, gMipiCamSetupData->MipiCam_ControlLogic1_PchClockSource);

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    if (Index < gMipiCamSetupData->MipiCam_ControlLogic1_GpioPinsEnabled) {
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioGroupPadNumber[Index], gMipiCamSetupData->MipiCam_ControlLogic1_GpioGroupPadNumber[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioComNumber[Index], gMipiCamSetupData->MipiCam_ControlLogic1_GpioComNumber[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioGroupNumber[Index], gMipiCamSetupData->MipiCam_ControlLogic1_GpioGroupNumber[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioFunction[Index], gMipiCamSetupData->MipiCam_ControlLogic1_GpioFunction[Index]);
      if (gMipiCamSetupData->MipiCam_ControlLogic1_GpioFunction[Index] == 0xFF) {
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioFunction[Index], gMipiCamSetupData->MipiCam_ControlLogic1_GpioFunction_User[Index]);
      }
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioActiveValue[Index], gMipiCamSetupData->MipiCam_ControlLogic1_GpioActiveValue[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioInitialValue[Index], gMipiCamSetupData->MipiCam_ControlLogic1_GpioInitialValue[Index]);
    } else {
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioGroupPadNumber[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioGroupNumber[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioComNumber[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioFunction[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioActiveValue[Index], 1);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioInitialValue[Index], 0);
    }
  }

  //
  // MipiCam Control Logic 2 specific
  //
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Version, 1);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Type, gMipiCamSetupData->MipiCam_ControlLogic2_Type);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_CrdVersion, gMipiCamSetupData->MipiCam_ControlLogic2_CrdVersion);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_InputClock, gMipiCamSetupData->MipiCam_ControlLogic2_InputClock);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioPinsEnabled, gMipiCamSetupData->MipiCam_ControlLogic2_GpioPinsEnabled);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_I2cBus, gMipiCamSetupData->MipiCam_ControlLogic2_I2cChannel);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_I2cAddress, gMipiCamSetupData->MipiCam_ControlLogic2_I2cAddress);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Pld, gMipiCamSetupData->MipiCam_ControlLogic2_Pld);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled1FlashMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic2_Wled1FlashMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled1TorchMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic2_Wled1TorchMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled2FlashMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic2_Wled2FlashMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled2TorchMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic2_Wled2TorchMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled1Type, gMipiCamSetupData->MipiCam_ControlLogic2_Wled1Type);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled2Type, gMipiCamSetupData->MipiCam_ControlLogic2_Wled2Type);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_PchClockSource, gMipiCamSetupData->MipiCam_ControlLogic2_PchClockSource);

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    if (Index < gMipiCamSetupData->MipiCam_ControlLogic2_GpioPinsEnabled) {
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioGroupPadNumber[Index], gMipiCamSetupData->MipiCam_ControlLogic2_GpioGroupPadNumber[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioComNumber[Index], gMipiCamSetupData->MipiCam_ControlLogic2_GpioComNumber[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioGroupNumber[Index], gMipiCamSetupData->MipiCam_ControlLogic2_GpioGroupNumber[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioFunction[Index], gMipiCamSetupData->MipiCam_ControlLogic2_GpioFunction[Index]);
      if (gMipiCamSetupData->MipiCam_ControlLogic2_GpioFunction[Index] == 0xFF) {
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioFunction[Index], gMipiCamSetupData->MipiCam_ControlLogic2_GpioFunction_User[Index]);
      }
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioActiveValue[Index], gMipiCamSetupData->MipiCam_ControlLogic2_GpioActiveValue[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioInitialValue[Index], gMipiCamSetupData->MipiCam_ControlLogic2_GpioInitialValue[Index]);
    } else {
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioGroupPadNumber[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioGroupNumber[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioComNumber[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioFunction[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioActiveValue[Index], 1);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioInitialValue[Index], 0);
    }
  }

  //
  // MipiCam Control Logic 3 specific
  //
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Version, 1);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Type, gMipiCamSetupData->MipiCam_ControlLogic3_Type);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_CrdVersion, gMipiCamSetupData->MipiCam_ControlLogic3_CrdVersion);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_InputClock, gMipiCamSetupData->MipiCam_ControlLogic3_InputClock);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioPinsEnabled, gMipiCamSetupData->MipiCam_ControlLogic3_GpioPinsEnabled);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_I2cBus, gMipiCamSetupData->MipiCam_ControlLogic3_I2cChannel);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_I2cAddress, gMipiCamSetupData->MipiCam_ControlLogic3_I2cAddress);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Pld, gMipiCamSetupData->MipiCam_ControlLogic3_Pld);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled1FlashMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic3_Wled1FlashMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled1TorchMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic3_Wled1TorchMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled2FlashMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic3_Wled2FlashMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled2TorchMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic3_Wled2TorchMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled1Type, gMipiCamSetupData->MipiCam_ControlLogic3_Wled1Type);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled2Type, gMipiCamSetupData->MipiCam_ControlLogic3_Wled2Type);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_PchClockSource, gMipiCamSetupData->MipiCam_ControlLogic3_PchClockSource);

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    if (Index < gMipiCamSetupData->MipiCam_ControlLogic3_GpioPinsEnabled) {
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioGroupPadNumber[Index], gMipiCamSetupData->MipiCam_ControlLogic3_GpioGroupPadNumber[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioComNumber[Index], gMipiCamSetupData->MipiCam_ControlLogic3_GpioComNumber[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioGroupNumber[Index], gMipiCamSetupData->MipiCam_ControlLogic3_GpioGroupNumber[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioFunction[Index], gMipiCamSetupData->MipiCam_ControlLogic3_GpioFunction[Index]);
      if (gMipiCamSetupData->MipiCam_ControlLogic3_GpioFunction[Index] == 0xFF) {
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioFunction[Index], gMipiCamSetupData->MipiCam_ControlLogic3_GpioFunction_User[Index]);
      }
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioActiveValue[Index], gMipiCamSetupData->MipiCam_ControlLogic3_GpioActiveValue[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioInitialValue[Index], gMipiCamSetupData->MipiCam_ControlLogic3_GpioInitialValue[Index]);
    } else {
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioGroupPadNumber[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioGroupNumber[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioComNumber[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioFunction[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioActiveValue[Index], 1);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioInitialValue[Index], 0);
    }
  }

  //
  // MipiCam Control Logic 4 specific
  //
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Version, 1);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Type, gMipiCamSetupData->MipiCam_ControlLogic4_Type);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_CrdVersion, gMipiCamSetupData->MipiCam_ControlLogic4_CrdVersion);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_InputClock, gMipiCamSetupData->MipiCam_ControlLogic4_InputClock);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioPinsEnabled, gMipiCamSetupData->MipiCam_ControlLogic4_GpioPinsEnabled);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_I2cBus, gMipiCamSetupData->MipiCam_ControlLogic4_I2cChannel);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_I2cAddress, gMipiCamSetupData->MipiCam_ControlLogic4_I2cAddress);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Pld, gMipiCamSetupData->MipiCam_ControlLogic4_Pld);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled1FlashMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic4_Wled1FlashMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled1TorchMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic4_Wled1TorchMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled2FlashMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic4_Wled2FlashMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled2TorchMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic4_Wled2TorchMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled1Type, gMipiCamSetupData->MipiCam_ControlLogic4_Wled1Type);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled2Type, gMipiCamSetupData->MipiCam_ControlLogic4_Wled2Type);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_PchClockSource, gMipiCamSetupData->MipiCam_ControlLogic4_PchClockSource);

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    if (Index < gMipiCamSetupData->MipiCam_ControlLogic4_GpioPinsEnabled) {
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioGroupPadNumber[Index], gMipiCamSetupData->MipiCam_ControlLogic4_GpioGroupPadNumber[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioComNumber[Index], gMipiCamSetupData->MipiCam_ControlLogic4_GpioComNumber[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioGroupNumber[Index], gMipiCamSetupData->MipiCam_ControlLogic4_GpioGroupNumber[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioFunction[Index], gMipiCamSetupData->MipiCam_ControlLogic4_GpioFunction[Index]);
      if (gMipiCamSetupData->MipiCam_ControlLogic4_GpioFunction[Index] == 0xFF) {
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioFunction[Index], gMipiCamSetupData->MipiCam_ControlLogic4_GpioFunction_User[Index]);
      }
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioActiveValue[Index], gMipiCamSetupData->MipiCam_ControlLogic4_GpioActiveValue[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioInitialValue[Index], gMipiCamSetupData->MipiCam_ControlLogic4_GpioInitialValue[Index]);
    } else {
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioGroupPadNumber[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioGroupNumber[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioComNumber[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioFunction[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioActiveValue[Index], 1);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioInitialValue[Index], 0);
    }
  }

  //
  // MipiCam Control Logic 5 specific
  //
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Version, 1);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Type, gMipiCamSetupData->MipiCam_ControlLogic5_Type);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_CrdVersion, gMipiCamSetupData->MipiCam_ControlLogic5_CrdVersion);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_InputClock, gMipiCamSetupData->MipiCam_ControlLogic5_InputClock);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioPinsEnabled, gMipiCamSetupData->MipiCam_ControlLogic5_GpioPinsEnabled);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_I2cBus, gMipiCamSetupData->MipiCam_ControlLogic5_I2cChannel);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_I2cAddress, gMipiCamSetupData->MipiCam_ControlLogic5_I2cAddress);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Pld, gMipiCamSetupData->MipiCam_ControlLogic5_Pld);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled1FlashMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic5_Wled1FlashMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled1TorchMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic5_Wled1TorchMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled2FlashMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic5_Wled2FlashMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled2TorchMaxCurrent, gMipiCamSetupData->MipiCam_ControlLogic5_Wled2TorchMaxCurrent);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled1Type, gMipiCamSetupData->MipiCam_ControlLogic5_Wled1Type);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled2Type, gMipiCamSetupData->MipiCam_ControlLogic5_Wled2Type);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_PchClockSource, gMipiCamSetupData->MipiCam_ControlLogic5_PchClockSource);

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    if (Index < gMipiCamSetupData->MipiCam_ControlLogic5_GpioPinsEnabled) {
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioGroupPadNumber[Index], gMipiCamSetupData->MipiCam_ControlLogic5_GpioGroupPadNumber[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioComNumber[Index], gMipiCamSetupData->MipiCam_ControlLogic5_GpioComNumber[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioGroupNumber[Index], gMipiCamSetupData->MipiCam_ControlLogic5_GpioGroupNumber[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioFunction[Index], gMipiCamSetupData->MipiCam_ControlLogic5_GpioFunction[Index]);
      if (gMipiCamSetupData->MipiCam_ControlLogic5_GpioFunction[Index] == 0xFF) {
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioFunction[Index], gMipiCamSetupData->MipiCam_ControlLogic5_GpioFunction_User[Index]);
      }
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioActiveValue[Index], gMipiCamSetupData->MipiCam_ControlLogic5_GpioActiveValue[Index]);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioInitialValue[Index], gMipiCamSetupData->MipiCam_ControlLogic5_GpioInitialValue[Index]);
    } else {
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioGroupPadNumber[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioGroupNumber[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioComNumber[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioFunction[Index], 0);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioActiveValue[Index], 1);
      UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioInitialValue[Index], 0);
    }
  }

  //
  // MipiCam Link0 specific
  //
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0SensorModel, gMipiCamSetupData->MipiCam_Link0_SensorModel);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0Pld, gMipiCamSetupData->MipiCam_Link0_CameraPhysicalLocation);
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink0ModuleName, MipiModuleName, sizeof(MipiModuleName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Link0_ModuleName, MipiModuleName, sizeof(MipiModuleName));
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink0UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Link0_UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0I2cDevicesEnabled, gMipiCamSetupData->MipiCam_Link0_I2cDevicesEnabled);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0I2cBus, gMipiCamSetupData->MipiCam_Link0_I2cChannel);

  //
  // MipiCam Link0 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0I2cAddrDev[Index], gMipiCamSetupData->MipiCam_Link0_I2cAddress[Index]);
    switch (gMipiCamSetupData->MipiCam_Link0_I2cDeviceType[Index]) {
      case MIPICAM_CUSTOMIZE_DEVICE1:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link0_Customize_Id[0]);
        break;
      case MIPICAM_CUSTOMIZE_DEVICE2:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link0_Customize_Id[1]);
        break;
      case MIPICAM_CUSTOMIZE_DEVICE3:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link0_Customize_Id[2]);
        break;
      default:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link0_I2cDeviceType[Index]);
    }
  }

  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_Version, 1);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_CrdVersion, gMipiCamSetupData->MipiCam_Link0_DriverData_CrdVersion);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_LinkUsed, gMipiCamSetupData->MipiCam_Link0_DriverData_LinkUsed);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_LaneUsed, gMipiCamSetupData->MipiCam_Link0_DriverData_LaneUsed);
  if (gMipiCamSetupData->MipiCam_Link0_DriverData_EepromType == 0xFF) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_EepromType, gMipiCamSetupData->MipiCam_Link0_DriverData_EepromTypeCustom);
  } else {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_EepromType, gMipiCamSetupData->MipiCam_Link0_DriverData_EepromType);
  }
  if (gMipiCamSetupData->MipiCam_Link0_DriverData_VcmType == 0xFF) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_VcmType, gMipiCamSetupData->MipiCam_Link0_DriverData_VcmTypeCustom);
  } else {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_VcmType, gMipiCamSetupData->MipiCam_Link0_DriverData_VcmType);
  }
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_Mclk, gMipiCamSetupData->MipiCam_Link0_DriverData_Mclk);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_ControlLogic, gMipiCamSetupData->MipiCam_Link0_DriverData_ControlLogic);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_FlashSupport, gMipiCamSetupData->MipiCam_Link0_DriverData_FlashSupport);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_PrivacyLed, gMipiCamSetupData->MipiCam_Link0_DriverData_PrivacyLed);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_Degree, gMipiCamSetupData->MipiCam_Link0_DriverData_Degree);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_PmicPosition, gMipiCamSetupData->MipiCam_Link0_DriverData_PmicPosition);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_VoltageRail, gMipiCamSetupData->MipiCam_Link0_DriverData_VoltageRail);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_PhyConfiguration, gMipiCamSetupData->MipiCam_Link0_DriverData_PhyConfiguration);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_LaneConfiguration, gMipiCamSetupData->MipiCam_Link0_DriverData_LaneConfiguration);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0_FlashDriverSelection, gMipiCamSetupData->MipiCam_Link0_FlashDriverSelection);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_FlashID, gMipiCamSetupData->MipiCam_Link0_DriverData_FlashID);

  //
  // MipiCam Link1 specific
  //
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1SensorModel, gMipiCamSetupData->MipiCam_Link1_SensorModel);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1Pld, gMipiCamSetupData->MipiCam_Link1_CameraPhysicalLocation);
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink1ModuleName, MipiModuleName, sizeof(MipiModuleName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Link1_ModuleName, MipiModuleName, sizeof(MipiModuleName));
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink1UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Link1_UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1I2cDevicesEnabled, gMipiCamSetupData->MipiCam_Link1_I2cDevicesEnabled);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1I2cBus, gMipiCamSetupData->MipiCam_Link1_I2cChannel);

  //
  // MipiCam Link1 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1I2cAddrDev[Index], gMipiCamSetupData->MipiCam_Link1_I2cAddress[Index]);
    switch (gMipiCamSetupData->MipiCam_Link1_I2cDeviceType[Index]) {
      case MIPICAM_CUSTOMIZE_DEVICE1:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link1_Customize_Id[0]);
        break;
      case MIPICAM_CUSTOMIZE_DEVICE2:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link1_Customize_Id[1]);
        break;
      case MIPICAM_CUSTOMIZE_DEVICE3:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link1_Customize_Id[2]);
        break;
      default:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link1_I2cDeviceType[Index]);
    }
  }
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_Version, 1);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_CrdVersion, gMipiCamSetupData->MipiCam_Link1_DriverData_CrdVersion);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_LinkUsed, gMipiCamSetupData->MipiCam_Link1_DriverData_LinkUsed);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_LaneUsed, gMipiCamSetupData->MipiCam_Link1_DriverData_LaneUsed);
  if (gMipiCamSetupData->MipiCam_Link1_DriverData_EepromType == 0xFF) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_EepromType, gMipiCamSetupData->MipiCam_Link1_DriverData_EepromTypeCustom);
  } else {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_EepromType, gMipiCamSetupData->MipiCam_Link1_DriverData_EepromType);
  }
  if (gMipiCamSetupData->MipiCam_Link1_DriverData_VcmType == 0xFF) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_VcmType, gMipiCamSetupData->MipiCam_Link1_DriverData_VcmTypeCustom);
  } else {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_VcmType, gMipiCamSetupData->MipiCam_Link1_DriverData_VcmType);
  }
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_Mclk, gMipiCamSetupData->MipiCam_Link1_DriverData_Mclk);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_ControlLogic, gMipiCamSetupData->MipiCam_Link1_DriverData_ControlLogic);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_FlashSupport, gMipiCamSetupData->MipiCam_Link1_DriverData_FlashSupport);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_PrivacyLed, gMipiCamSetupData->MipiCam_Link1_DriverData_PrivacyLed);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_Degree, gMipiCamSetupData->MipiCam_Link1_DriverData_Degree);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_PmicPosition, gMipiCamSetupData->MipiCam_Link1_DriverData_PmicPosition);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_VoltageRail, gMipiCamSetupData->MipiCam_Link1_DriverData_VoltageRail);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_PhyConfiguration, gMipiCamSetupData->MipiCam_Link1_DriverData_PhyConfiguration);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_LaneConfiguration, gMipiCamSetupData->MipiCam_Link1_DriverData_LaneConfiguration);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1_FlashDriverSelection, gMipiCamSetupData->MipiCam_Link1_FlashDriverSelection);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_FlashID, gMipiCamSetupData->MipiCam_Link1_DriverData_FlashID);

  //
  // MipiCam Link2 specific
  //
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2SensorModel, gMipiCamSetupData->MipiCam_Link2_SensorModel);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2Pld, gMipiCamSetupData->MipiCam_Link2_CameraPhysicalLocation);
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink2ModuleName, MipiModuleName, sizeof(MipiModuleName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Link2_ModuleName, MipiModuleName, sizeof(MipiModuleName));
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink2UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Link2_UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2I2cDevicesEnabled, gMipiCamSetupData->MipiCam_Link2_I2cDevicesEnabled);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2I2cBus, gMipiCamSetupData->MipiCam_Link2_I2cChannel);

  //
  // MipiCam Link2 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2I2cAddrDev[Index], gMipiCamSetupData->MipiCam_Link2_I2cAddress[Index]);
    switch (gMipiCamSetupData->MipiCam_Link2_I2cDeviceType[Index]) {
      case MIPICAM_CUSTOMIZE_DEVICE1:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link2_Customize_Id[0]);
        break;
      case MIPICAM_CUSTOMIZE_DEVICE2:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link2_Customize_Id[1]);
        break;
      case MIPICAM_CUSTOMIZE_DEVICE3:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link2_Customize_Id[2]);
        break;
      default:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link2_I2cDeviceType[Index]);
    }
  }
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_Version, 1);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_CrdVersion, gMipiCamSetupData->MipiCam_Link2_DriverData_CrdVersion);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_LinkUsed, gMipiCamSetupData->MipiCam_Link2_DriverData_LinkUsed);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_LaneUsed, gMipiCamSetupData->MipiCam_Link2_DriverData_LaneUsed);
  if (gMipiCamSetupData->MipiCam_Link2_DriverData_EepromType == 0xFF) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_EepromType, gMipiCamSetupData->MipiCam_Link2_DriverData_EepromTypeCustom);
  } else {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_EepromType, gMipiCamSetupData->MipiCam_Link2_DriverData_EepromType);
  }
  if (gMipiCamSetupData->MipiCam_Link2_DriverData_VcmType == 0xFF) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_VcmType, gMipiCamSetupData->MipiCam_Link2_DriverData_VcmTypeCustom);
  } else {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_VcmType, gMipiCamSetupData->MipiCam_Link2_DriverData_VcmType);
  }
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_Mclk, gMipiCamSetupData->MipiCam_Link2_DriverData_Mclk);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_ControlLogic, gMipiCamSetupData->MipiCam_Link2_DriverData_ControlLogic);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_FlashSupport, gMipiCamSetupData->MipiCam_Link2_DriverData_FlashSupport);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_PrivacyLed, gMipiCamSetupData->MipiCam_Link2_DriverData_PrivacyLed);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_Degree, gMipiCamSetupData->MipiCam_Link2_DriverData_Degree);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_PmicPosition, gMipiCamSetupData->MipiCam_Link2_DriverData_PmicPosition);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_VoltageRail, gMipiCamSetupData->MipiCam_Link2_DriverData_VoltageRail);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_PhyConfiguration, gMipiCamSetupData->MipiCam_Link2_DriverData_PhyConfiguration);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_LaneConfiguration, gMipiCamSetupData->MipiCam_Link2_DriverData_LaneConfiguration);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2_FlashDriverSelection, gMipiCamSetupData->MipiCam_Link2_FlashDriverSelection);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_FlashID, gMipiCamSetupData->MipiCam_Link2_DriverData_FlashID);

  //
  // MipiCam Link3 specific
  //
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3SensorModel, gMipiCamSetupData->MipiCam_Link3_SensorModel);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3Pld, gMipiCamSetupData->MipiCam_Link3_CameraPhysicalLocation);
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink3ModuleName, MipiModuleName, sizeof(MipiModuleName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Link3_ModuleName, MipiModuleName, sizeof(MipiModuleName));
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink3UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Link3_UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3I2cDevicesEnabled, gMipiCamSetupData->MipiCam_Link3_I2cDevicesEnabled);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3I2cBus, gMipiCamSetupData->MipiCam_Link3_I2cChannel);

  //
  // MipiCam Link3 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3I2cAddrDev[Index], gMipiCamSetupData->MipiCam_Link3_I2cAddress[Index]);
    switch (gMipiCamSetupData->MipiCam_Link3_I2cDeviceType[Index]) {
      case MIPICAM_CUSTOMIZE_DEVICE1:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link3_Customize_Id[0]);
        break;
      case MIPICAM_CUSTOMIZE_DEVICE2:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link3_Customize_Id[1]);
        break;
      case MIPICAM_CUSTOMIZE_DEVICE3:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link3_Customize_Id[2]);
        break;
      default:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link3_I2cDeviceType[Index]);
    }
  }
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_Version, 1);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_CrdVersion, gMipiCamSetupData->MipiCam_Link3_DriverData_CrdVersion);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_LinkUsed, gMipiCamSetupData->MipiCam_Link3_DriverData_LinkUsed);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_LaneUsed, gMipiCamSetupData->MipiCam_Link3_DriverData_LaneUsed);
  if (gMipiCamSetupData->MipiCam_Link3_DriverData_EepromType == 0xFF) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_EepromType, gMipiCamSetupData->MipiCam_Link3_DriverData_EepromTypeCustom);
  } else {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_EepromType, gMipiCamSetupData->MipiCam_Link3_DriverData_EepromType);
  }
  if (gMipiCamSetupData->MipiCam_Link3_DriverData_VcmType == 0xFF) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_VcmType, gMipiCamSetupData->MipiCam_Link3_DriverData_VcmTypeCustom);
  } else {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_VcmType, gMipiCamSetupData->MipiCam_Link3_DriverData_VcmType);
  }
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_Mclk, gMipiCamSetupData->MipiCam_Link3_DriverData_Mclk);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_ControlLogic, gMipiCamSetupData->MipiCam_Link3_DriverData_ControlLogic);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_FlashSupport, gMipiCamSetupData->MipiCam_Link3_DriverData_FlashSupport);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_PrivacyLed, gMipiCamSetupData->MipiCam_Link3_DriverData_PrivacyLed);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_Degree, gMipiCamSetupData->MipiCam_Link3_DriverData_Degree);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_PmicPosition, gMipiCamSetupData->MipiCam_Link3_DriverData_PmicPosition);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_VoltageRail, gMipiCamSetupData->MipiCam_Link3_DriverData_VoltageRail);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_PhyConfiguration, gMipiCamSetupData->MipiCam_Link3_DriverData_PhyConfiguration);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_LaneConfiguration, gMipiCamSetupData->MipiCam_Link3_DriverData_LaneConfiguration);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3_FlashDriverSelection, gMipiCamSetupData->MipiCam_Link3_FlashDriverSelection);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_FlashID, gMipiCamSetupData->MipiCam_Link3_DriverData_FlashID);

  //
  // MipiCam Link4 specific
  //
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4SensorModel, gMipiCamSetupData->MipiCam_Link4_SensorModel);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4Pld, gMipiCamSetupData->MipiCam_Link4_CameraPhysicalLocation);
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink4ModuleName, MipiModuleName, sizeof(MipiModuleName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Link4_ModuleName, MipiModuleName, sizeof(MipiModuleName));
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink4UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Link4_UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4I2cDevicesEnabled, gMipiCamSetupData->MipiCam_Link4_I2cDevicesEnabled);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4I2cBus, gMipiCamSetupData->MipiCam_Link4_I2cChannel);

  //
  // MipiCam Link4 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4I2cAddrDev[Index], gMipiCamSetupData->MipiCam_Link4_I2cAddress[Index]);
    switch (gMipiCamSetupData->MipiCam_Link4_I2cDeviceType[Index]) {
      case MIPICAM_CUSTOMIZE_DEVICE1:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link4_Customize_Id[0]);
        break;
      case MIPICAM_CUSTOMIZE_DEVICE2:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link4_Customize_Id[1]);
        break;
      case MIPICAM_CUSTOMIZE_DEVICE3:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link4_Customize_Id[2]);
        break;
      default:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link4_I2cDeviceType[Index]);
    }
  }
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_Version, 1);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_CrdVersion, gMipiCamSetupData->MipiCam_Link4_DriverData_CrdVersion);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_LinkUsed, gMipiCamSetupData->MipiCam_Link4_DriverData_LinkUsed);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_LaneUsed, gMipiCamSetupData->MipiCam_Link4_DriverData_LaneUsed);
  if (gMipiCamSetupData->MipiCam_Link4_DriverData_EepromType == 0xFF) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_EepromType, gMipiCamSetupData->MipiCam_Link4_DriverData_EepromTypeCustom);
  } else {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_EepromType, gMipiCamSetupData->MipiCam_Link4_DriverData_EepromType);
  }
  if (gMipiCamSetupData->MipiCam_Link4_DriverData_VcmType == 0xFF) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_VcmType, gMipiCamSetupData->MipiCam_Link4_DriverData_VcmTypeCustom);
  } else {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_VcmType, gMipiCamSetupData->MipiCam_Link4_DriverData_VcmType);
  }
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_Mclk, gMipiCamSetupData->MipiCam_Link4_DriverData_Mclk);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_ControlLogic, gMipiCamSetupData->MipiCam_Link4_DriverData_ControlLogic);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_FlashSupport, gMipiCamSetupData->MipiCam_Link4_DriverData_FlashSupport);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_PrivacyLed, gMipiCamSetupData->MipiCam_Link4_DriverData_PrivacyLed);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_Degree, gMipiCamSetupData->MipiCam_Link4_DriverData_Degree);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_PmicPosition, gMipiCamSetupData->MipiCam_Link4_DriverData_PmicPosition);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_VoltageRail, gMipiCamSetupData->MipiCam_Link4_DriverData_VoltageRail);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_PhyConfiguration, gMipiCamSetupData->MipiCam_Link4_DriverData_PhyConfiguration);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_LaneConfiguration, gMipiCamSetupData->MipiCam_Link4_DriverData_LaneConfiguration);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4_FlashDriverSelection, gMipiCamSetupData->MipiCam_Link4_FlashDriverSelection);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_FlashID, gMipiCamSetupData->MipiCam_Link4_DriverData_FlashID);

  //
  // MipiCam Link5 specific
  //
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5SensorModel, gMipiCamSetupData->MipiCam_Link5_SensorModel);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5Pld, gMipiCamSetupData->MipiCam_Link5_CameraPhysicalLocation);
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink5ModuleName, MipiModuleName, sizeof(MipiModuleName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Link5_ModuleName, MipiModuleName, sizeof(MipiModuleName));
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamLink5UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Link5_UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5I2cDevicesEnabled, gMipiCamSetupData->MipiCam_Link5_I2cDevicesEnabled);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5I2cBus, gMipiCamSetupData->MipiCam_Link5_I2cChannel);

  //
  // MipiCam Link5 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5I2cAddrDev[Index], gMipiCamSetupData->MipiCam_Link5_I2cAddress[Index]);
    switch (gMipiCamSetupData->MipiCam_Link5_I2cDeviceType[Index]) {
      case MIPICAM_CUSTOMIZE_DEVICE1:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link5_Customize_Id[0]);
        break;
      case MIPICAM_CUSTOMIZE_DEVICE2:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link5_Customize_Id[1]);
        break;
      case MIPICAM_CUSTOMIZE_DEVICE3:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link5_Customize_Id[2]);
        break;
      default:
        UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5I2cDeviceType[Index], gMipiCamSetupData->MipiCam_Link5_I2cDeviceType[Index]);
    }
  }
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_Version, 1);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_CrdVersion, gMipiCamSetupData->MipiCam_Link5_DriverData_CrdVersion);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_LinkUsed, gMipiCamSetupData->MipiCam_Link5_DriverData_LinkUsed);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_LaneUsed, gMipiCamSetupData->MipiCam_Link5_DriverData_LaneUsed);
  if (gMipiCamSetupData->MipiCam_Link5_DriverData_EepromType == 0xFF) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_EepromType, gMipiCamSetupData->MipiCam_Link5_DriverData_EepromTypeCustom);
  } else {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_EepromType, gMipiCamSetupData->MipiCam_Link5_DriverData_EepromType);
  }
  if (gMipiCamSetupData->MipiCam_Link5_DriverData_VcmType == 0xFF) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_VcmType, gMipiCamSetupData->MipiCam_Link5_DriverData_VcmTypeCustom);
  } else {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_VcmType, gMipiCamSetupData->MipiCam_Link5_DriverData_VcmType);
  }
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_Mclk, gMipiCamSetupData->MipiCam_Link5_DriverData_Mclk);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_ControlLogic, gMipiCamSetupData->MipiCam_Link5_DriverData_ControlLogic);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_FlashSupport, gMipiCamSetupData->MipiCam_Link5_DriverData_FlashSupport);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_PrivacyLed, gMipiCamSetupData->MipiCam_Link5_DriverData_PrivacyLed);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_Degree, gMipiCamSetupData->MipiCam_Link5_DriverData_Degree);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_PmicPosition, gMipiCamSetupData->MipiCam_Link5_DriverData_PmicPosition);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_VoltageRail, gMipiCamSetupData->MipiCam_Link5_DriverData_VoltageRail);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_PhyConfiguration, gMipiCamSetupData->MipiCam_Link5_DriverData_PhyConfiguration);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_LaneConfiguration, gMipiCamSetupData->MipiCam_Link5_DriverData_LaneConfiguration);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5_FlashDriverSelection, gMipiCamSetupData->MipiCam_Link5_FlashDriverSelection);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_FlashID, gMipiCamSetupData->MipiCam_Link5_DriverData_FlashID);

  //
  // MipiCam Flash0 specific
  //
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash0Model, gMipiCamSetupData->MipiCam_Flash0_Model);
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash0UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Flash0_UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash0Mode, gMipiCamSetupData->MipiCam_Flash0_Mode);
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash0ModuleName, MipiModuleName, sizeof(MipiModuleName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Flash0_ModuleName, MipiModuleName, sizeof(MipiModuleName));
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash0I2cBus, gMipiCamSetupData->MipiCam_Flash0_I2cChannel);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash0I2cAddrDev, gMipiCamSetupData->MipiCam_Flash0_I2cAddress);
  if (gMipiCamSetupData->MipiCam_Flash0_GpioSupport == 1) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioGroupPadNumber, gMipiCamSetupData->MipiCam_Flash0_GpioGroupPadNumber);
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioComNumber, gMipiCamSetupData->MipiCam_Flash0_GpioComNumber);
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioGroupNumber, gMipiCamSetupData->MipiCam_Flash0_GpioGroupNumber);
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioActiveValue, gMipiCamSetupData->MipiCam_Flash0_GpioActiveValue);
  }
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash0OperatingMode, gMipiCamSetupData->MipiCam_Flash0_OperatingMode);

  //
  // MipiCam Flash1 specific
  //
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash1Model, gMipiCamSetupData->MipiCam_Flash1_Model);
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash1UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Flash1_UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash1Mode, gMipiCamSetupData->MipiCam_Flash1_Mode);
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash1ModuleName, MipiModuleName, sizeof(MipiModuleName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Flash1_ModuleName, MipiModuleName, sizeof(MipiModuleName));
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash1I2cBus, gMipiCamSetupData->MipiCam_Flash1_I2cChannel);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash1I2cAddrDev, gMipiCamSetupData->MipiCam_Flash1_I2cAddress);
  if (gMipiCamSetupData->MipiCam_Flash1_GpioSupport == 1) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioGroupPadNumber, gMipiCamSetupData->MipiCam_Flash1_GpioGroupPadNumber);
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioComNumber, gMipiCamSetupData->MipiCam_Flash1_GpioComNumber);
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioGroupNumber, gMipiCamSetupData->MipiCam_Flash1_GpioGroupNumber);
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioActiveValue, gMipiCamSetupData->MipiCam_Flash1_GpioActiveValue);
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioInitialValue, gMipiCamSetupData->MipiCam_Flash1_GpioInitialValue);
  }
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash1OperatingMode, gMipiCamSetupData->MipiCam_Flash1_OperatingMode);

  //
  // MipiCam Flash2 specific
  //
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash2Model, gMipiCamSetupData->MipiCam_Flash2_Model);
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash2UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Flash2_UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash2Mode, gMipiCamSetupData->MipiCam_Flash2_Mode);
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash2ModuleName, MipiModuleName, sizeof(MipiModuleName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Flash2_ModuleName, MipiModuleName, sizeof(MipiModuleName));
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash2I2cBus, gMipiCamSetupData->MipiCam_Flash2_I2cChannel);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash2I2cAddrDev, gMipiCamSetupData->MipiCam_Flash2_I2cAddress);
  if (gMipiCamSetupData->MipiCam_Flash2_GpioSupport == 1) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioGroupPadNumber, gMipiCamSetupData->MipiCam_Flash2_GpioGroupPadNumber);
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioComNumber, gMipiCamSetupData->MipiCam_Flash2_GpioComNumber);
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioGroupNumber, gMipiCamSetupData->MipiCam_Flash2_GpioGroupNumber);
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioActiveValue, gMipiCamSetupData->MipiCam_Flash2_GpioActiveValue);
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioInitialValue, gMipiCamSetupData->MipiCam_Flash2_GpioInitialValue);
  }
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash2OperatingMode, gMipiCamSetupData->MipiCam_Flash2_OperatingMode);

  //
  // MipiCam Flash3 specific
  //
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash3Model, gMipiCamSetupData->MipiCam_Flash3_Model);
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash3UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Flash3_UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash3Mode, gMipiCamSetupData->MipiCam_Flash3_Mode);
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash3ModuleName, MipiModuleName, sizeof(MipiModuleName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Flash3_ModuleName, MipiModuleName, sizeof(MipiModuleName));
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash3I2cBus, gMipiCamSetupData->MipiCam_Flash3_I2cChannel);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash3I2cAddrDev, gMipiCamSetupData->MipiCam_Flash3_I2cAddress);
  if (gMipiCamSetupData->MipiCam_Flash3_GpioSupport == 1) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioGroupPadNumber, gMipiCamSetupData->MipiCam_Flash3_GpioGroupPadNumber);
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioComNumber, gMipiCamSetupData->MipiCam_Flash3_GpioComNumber);
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioGroupNumber, gMipiCamSetupData->MipiCam_Flash3_GpioGroupNumber);
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioActiveValue, gMipiCamSetupData->MipiCam_Flash3_GpioActiveValue);
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioInitialValue, gMipiCamSetupData->MipiCam_Flash3_GpioInitialValue);
  }
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash3OperatingMode, gMipiCamSetupData->MipiCam_Flash3_OperatingMode);

  //
  // MipiCam Flash4 specific
  //
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash4Model, gMipiCamSetupData->MipiCam_Flash4_Model);
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash4UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Flash4_UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash4Mode, gMipiCamSetupData->MipiCam_Flash4_Mode);
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash4ModuleName, MipiModuleName, sizeof(MipiModuleName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Flash4_ModuleName, MipiModuleName, sizeof(MipiModuleName));
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash4I2cBus, gMipiCamSetupData->MipiCam_Flash4_I2cChannel);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash4I2cAddrDev, gMipiCamSetupData->MipiCam_Flash4_I2cAddress);
  if (gMipiCamSetupData->MipiCam_Flash4_GpioSupport == 1) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioGroupPadNumber, gMipiCamSetupData->MipiCam_Flash4_GpioGroupPadNumber);
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioComNumber, gMipiCamSetupData->MipiCam_Flash4_GpioComNumber);
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioGroupNumber, gMipiCamSetupData->MipiCam_Flash4_GpioGroupNumber);
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioActiveValue, gMipiCamSetupData->MipiCam_Flash4_GpioActiveValue);
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioInitialValue, gMipiCamSetupData->MipiCam_Flash4_GpioInitialValue);
  }
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash4OperatingMode, gMipiCamSetupData->MipiCam_Flash4_OperatingMode);

  //
  // MipiCam Flash5 specific
  //
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash5Model, gMipiCamSetupData->MipiCam_Flash5_Model);
  ZeroMem (MipiHidName, sizeof (MipiHidName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash5UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Flash5_UserHid, MipiHidName, sizeof(MipiHidName));
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash5Mode, gMipiCamSetupData->MipiCam_Flash5_Mode);
  ZeroMem (MipiModuleName, sizeof (MipiModuleName));
  AsciiStrToUnicodeStrS ((CHAR8 *) mMipiCamNvsAreaProtocol.Area->MipiCamFlash5ModuleName, MipiModuleName, sizeof(MipiModuleName));
  UT_ASSERT_MEM_EQUAL (gMipiCamSetupData->MipiCam_Flash5_ModuleName, MipiModuleName, sizeof(MipiModuleName));
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash5I2cBus, gMipiCamSetupData->MipiCam_Flash5_I2cChannel);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash5I2cAddrDev, gMipiCamSetupData->MipiCam_Flash5_I2cAddress);
  if (gMipiCamSetupData->MipiCam_Flash5_GpioSupport == 1) {
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash5GpioGroupPadNumber, gMipiCamSetupData->MipiCam_Flash5_GpioGroupPadNumber);
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash5GpioGroupNumber, gMipiCamSetupData->MipiCam_Flash5_GpioGroupNumber);
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash5GpioComNumber, gMipiCamSetupData->MipiCam_Flash5_GpioComNumber);
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash5GpioActiveValue, gMipiCamSetupData->MipiCam_Flash5_GpioActiveValue);
    UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash5GpioInitialValue, gMipiCamSetupData->MipiCam_Flash5_GpioInitialValue);
  }
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamFlash5OperatingMode, gMipiCamSetupData->MipiCam_Flash5_OperatingMode);

  //
  // MipiCam Config Hob specific
  //
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamPlatformId, gMipiCamConfigHob->PlatformId);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamEmulationConfig, gMipiCamConfigHob->EmulationConfig);
  UT_ASSERT_EQUAL (mMipiCamNvsAreaProtocol.Area->MipiCamChipsetId, gMipiCamConfigHob->PlatformChipId);

  return UNIT_TEST_PASSED;
}
