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

#include "PreInitializeMipiCam.h"

#define PRE_FUNCTION_NAME     "PreInitializeMipiCam"
#define PRE_FUNCTION_VERSION  "0.1"

VOID
DumpMipiCamNvsAreaProtocol (
  VOID
  )
{
  UINTN  Index;

  DEBUG ((DEBUG_INFO, "----- Dump mMipiCamNvsAreaProtocol Start -----\n"));

  //
  // MipiCam sensor configuration
  //
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic0: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic0));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic1: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic1));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic2: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic2));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic3: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic3));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic4: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic4));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic5: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamControlLogic5));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0Enabled: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0Enabled));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1Enabled: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1Enabled));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2Enabled: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2Enabled));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3Enabled: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3Enabled));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4Enabled: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4Enabled));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5Enabled: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5Enabled));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLanesClkDiv: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLanesClkDiv));

  //
  // MipiCam Control Logic 0 specific
  //
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Type: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Type));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_CrdVersion: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_CrdVersion));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_InputClock: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_InputClock));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioPinsEnabled: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioPinsEnabled));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_I2cBus: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_I2cBus));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_I2cAddress: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_I2cAddress));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Pld: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Pld));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled1FlashMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled1FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled1TorchMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled1TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled2FlashMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled2FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled2TorchMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled2TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled1Type: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled1Type));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled2Type: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_Wled2Type));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_PchClockSource: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_PchClockSource));

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioGroupPadNumber[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioGroupPadNumber[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioComNumber[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioComNumber[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioGroupNumber[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioGroupNumber[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioFunction[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioFunction[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioActiveValue[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioActiveValue[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioInitialValue[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic0_GpioInitialValue[Index]));
  }

  //
  // MipiCam Control Logic 1 specific
  //
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Type: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Type));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_CrdVersion: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_CrdVersion));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_InputClock: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_InputClock));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioPinsEnabled: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioPinsEnabled));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_I2cBus: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_I2cBus));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_I2cAddress: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_I2cAddress));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Pld: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Pld));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled1FlashMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled1FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled1TorchMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled1TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled2FlashMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled2FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled2TorchMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled2TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled1Type: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled1Type));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled2Type: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_Wled2Type));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_PchClockSource: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_PchClockSource));

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioGroupPadNumber[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioGroupPadNumber[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioComNumber[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioComNumber[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioGroupNumber[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioGroupNumber[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioFunction[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioFunction[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioActiveValue[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioActiveValue[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioInitialValue[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic1_GpioInitialValue[Index]));
  }

  //
  // MipiCam Control Logic 2 specific
  //
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Type: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Type));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_CrdVersion: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_CrdVersion));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_InputClock: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_InputClock));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioPinsEnabled: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioPinsEnabled));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_I2cBus: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_I2cBus));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_I2cAddress: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_I2cAddress));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Pld: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Pld));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled1FlashMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled1FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled1TorchMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled1TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled2FlashMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled2FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled2TorchMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled2TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled1Type: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled1Type));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled2Type: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_Wled2Type));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_PchClockSource: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_PchClockSource));

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioGroupPadNumber[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioGroupPadNumber[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioComNumber[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioComNumber[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioGroupNumber[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioGroupNumber[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioFunction[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioFunction[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioActiveValue[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioActiveValue[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioInitialValue[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic2_GpioInitialValue[Index]));
  }

  //
  // MipiCam Control Logic 3 specific
  //
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Type: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Type));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_CrdVersion: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_CrdVersion));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_InputClock: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_InputClock));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioPinsEnabled: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioPinsEnabled));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_I2cBus: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_I2cBus));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_I2cAddress: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_I2cAddress));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Pld: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Pld));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled1FlashMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled1FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled1TorchMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled1TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled2FlashMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled2FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled2TorchMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled2TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled1Type: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled1Type));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled2Type: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_Wled2Type));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_PchClockSource: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_PchClockSource));

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioGroupPadNumber[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioGroupPadNumber[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioComNumber[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioComNumber[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioGroupNumber[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioGroupNumber[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioFunction[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioFunction[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioActiveValue[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioActiveValue[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioInitialValue[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic3_GpioInitialValue[Index]));
  }

  //
  // MipiCam Control Logic 4 specific
  //
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Type: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Type));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_CrdVersion: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_CrdVersion));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_InputClock: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_InputClock));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioPinsEnabled: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioPinsEnabled));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_I2cBus: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_I2cBus));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_I2cAddress: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_I2cAddress));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Pld: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Pld));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled1FlashMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled1FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled1TorchMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled1TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled2FlashMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled2FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled2TorchMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled2TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled1Type: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled1Type));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled2Type: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_Wled2Type));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_PchClockSource: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_PchClockSource));

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioGroupPadNumber[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioGroupPadNumber[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioComNumber[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioComNumber[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioGroupNumber[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioGroupNumber[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioFunction[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioFunction[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioActiveValue[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioActiveValue[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioInitialValue[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic4_GpioInitialValue[Index]));
  }

  //
  // MipiCam Control Logic 5 specific
  //
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Type: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Type));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_CrdVersion: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_CrdVersion));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_InputClock: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_InputClock));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioPinsEnabled: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioPinsEnabled));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_I2cBus: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_I2cBus));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_I2cAddress: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_I2cAddress));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Pld: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Pld));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled1FlashMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled1FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled1TorchMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled1TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled2FlashMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled2FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled2TorchMaxCurrent: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled2TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled1Type: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled1Type));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled2Type: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_Wled2Type));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_PchClockSource: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_PchClockSource));

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioGroupPadNumber[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioGroupPadNumber[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioComNumber[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioComNumber[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioGroupNumber[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioGroupNumber[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioFunction[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioFunction[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioActiveValue[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioActiveValue[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioInitialValue[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamCtrlLogic5_GpioInitialValue[Index]));
  }

  //
  // MipiCam Link0 specific
  //
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0SensorModel: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0SensorModel));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0Pld: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0Pld));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0ModuleName: %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0ModuleName));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0UserHid: %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0UserHid));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0I2cDevicesEnabled: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0I2cDevicesEnabled));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0I2cBus: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0I2cBus));

  //
  // MipiCam Link0 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0I2cAddrDev[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamLink0I2cAddrDev[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0I2cDeviceType[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamLink0I2cDeviceType[Index]));
  }
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_Version: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_Version));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_CrdVersion: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_CrdVersion));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_LinkUsed: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_LinkUsed));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_LaneUsed: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_LaneUsed));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_EepromType: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_EepromType));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_VcmType: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_VcmType));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_Mclk: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_Mclk));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_ControlLogic: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_ControlLogic));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_FlashSupport: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_FlashSupport));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_PrivacyLed: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_PrivacyLed));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_Degree: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_Degree));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_PmicPosition: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_PmicPosition));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_VoltageRail: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_VoltageRail));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_PhyConfiguration: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_PhyConfiguration));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_LaneConfiguration: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_LaneConfiguration));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0_FlashDriverSelection: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0_FlashDriverSelection));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_FlashID: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink0DD_FlashID));

  //
  // MipiCam Link1 specific
  //
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1SensorModel: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1SensorModel));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1Pld: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1Pld));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1ModuleName: %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1ModuleName));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1UserHid: %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1UserHid));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1I2cDevicesEnabled: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1I2cDevicesEnabled));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1I2cBus: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1I2cBus));

  //
  // MipiCam Link1 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1I2cAddrDev[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamLink1I2cAddrDev[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1I2cDeviceType[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamLink1I2cDeviceType[Index]));
  }
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_Version: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_Version));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_CrdVersion: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_CrdVersion));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_LinkUsed: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_LinkUsed));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_LaneUsed: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_LaneUsed));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_EepromType: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_EepromType));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_VcmType: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_VcmType));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_Mclk: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_Mclk));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_ControlLogic: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_ControlLogic));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_FlashSupport: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_FlashSupport));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_PrivacyLed: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_PrivacyLed));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_Degree: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_Degree));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_PmicPosition: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_PmicPosition));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_VoltageRail: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_VoltageRail));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_PhyConfiguration: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_PhyConfiguration));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_LaneConfiguration: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_LaneConfiguration));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1_FlashDriverSelection: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1_FlashDriverSelection));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_FlashID: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink1DD_FlashID));

  //
  // MipiCam Link2 specific
  //
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2SensorModel: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2SensorModel));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2Pld: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2Pld));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2ModuleName: %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2ModuleName));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2UserHid: %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2UserHid));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2I2cDevicesEnabled: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2I2cDevicesEnabled));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2I2cBus: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2I2cBus));

  //
  // MipiCam Link2 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2I2cAddrDev[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamLink2I2cAddrDev[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2I2cDeviceType[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamLink2I2cDeviceType[Index]));
  }
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_Version: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_Version));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_CrdVersion: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_CrdVersion));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_LinkUsed: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_LinkUsed));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_LaneUsed: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_LaneUsed));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_EepromType: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_EepromType));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_VcmType: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_VcmType));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_Mclk: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_Mclk));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_ControlLogic: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_ControlLogic));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_FlashSupport: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_FlashSupport));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_PrivacyLed: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_PrivacyLed));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_Degree: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_Degree));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_PmicPosition: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_PmicPosition));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_VoltageRail: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_VoltageRail));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_PhyConfiguration: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_PhyConfiguration));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_LaneConfiguration: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_LaneConfiguration));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2_FlashDriverSelection: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2_FlashDriverSelection));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_FlashID: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink2DD_FlashID));

  //
  // MipiCam Link3 specific
  //
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3SensorModel: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3SensorModel));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3Pld: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3Pld));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3ModuleName: %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3ModuleName));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3UserHid: %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3UserHid));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3I2cDevicesEnabled: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3I2cDevicesEnabled));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3I2cBus: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3I2cBus));

  //
  // MipiCam Link3 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3I2cAddrDev[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamLink3I2cAddrDev[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3I2cDeviceType[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamLink3I2cDeviceType[Index]));
  }
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_Version: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_Version));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_CrdVersion: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_CrdVersion));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_LinkUsed: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_LinkUsed));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_LaneUsed: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_LaneUsed));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_EepromType: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_EepromType));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_VcmType: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_VcmType));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_Mclk: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_Mclk));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_ControlLogic: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_ControlLogic));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_FlashSupport: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_FlashSupport));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_PrivacyLed: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_PrivacyLed));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_Degree: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_Degree));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_PmicPosition: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_PmicPosition));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_VoltageRail: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_VoltageRail));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_PhyConfiguration: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_PhyConfiguration));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_LaneConfiguration: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_LaneConfiguration));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3_FlashDriverSelection: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3_FlashDriverSelection));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_FlashID: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink3DD_FlashID));

  //
  // MipiCam Link4 specific
  //
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4SensorModel: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4SensorModel));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4Pld: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4Pld));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4ModuleName: %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4ModuleName));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4UserHid: %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4UserHid));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4I2cDevicesEnabled: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4I2cDevicesEnabled));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4I2cBus: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4I2cBus));

  //
  // MipiCam Link4 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4I2cAddrDev[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamLink4I2cAddrDev[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4I2cDeviceType[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamLink4I2cDeviceType[Index]));
  }
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_Version: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_Version));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_CrdVersion: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_CrdVersion));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_LinkUsed: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_LinkUsed));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_LaneUsed: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_LaneUsed));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_EepromType: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_EepromType));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_VcmType: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_VcmType));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_Mclk: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_Mclk));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_ControlLogic: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_ControlLogic));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_FlashSupport: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_FlashSupport));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_PrivacyLed: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_PrivacyLed));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_Degree: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_Degree));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_PmicPosition: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_PmicPosition));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_VoltageRail: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_VoltageRail));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_PhyConfiguration: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_PhyConfiguration));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_LaneConfiguration: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_LaneConfiguration));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4_FlashDriverSelection: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4_FlashDriverSelection));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_FlashID: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink4DD_FlashID));

  //
  // MipiCam Link5 specific
  //
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5SensorModel: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5SensorModel));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5Pld: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5Pld));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5ModuleName: %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5ModuleName));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5UserHid: %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5UserHid));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5I2cDevicesEnabled: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5I2cDevicesEnabled));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5I2cBus: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5I2cBus));

  //
  // MipiCam Link5 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5I2cAddrDev[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamLink5I2cAddrDev[Index]));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5I2cDeviceType[%d]: 0x%x\n", Index, mMipiCamNvsAreaProtocol.Area->MipiCamLink5I2cDeviceType[Index]));
  }
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_Version: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_Version));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_CrdVersion: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_CrdVersion));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_LinkUsed: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_LinkUsed));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_LaneUsed: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_LaneUsed));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_EepromType: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_EepromType));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_VcmType: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_VcmType));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_Mclk: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_Mclk));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_ControlLogic: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_ControlLogic));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_FlashSupport: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_FlashSupport));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_PrivacyLed: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_PrivacyLed));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_Degree: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_Degree));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_PmicPosition: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_PmicPosition));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_VoltageRail: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_VoltageRail));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_PhyConfiguration: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_PhyConfiguration));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_LaneConfiguration: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_LaneConfiguration));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5_FlashDriverSelection: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5_FlashDriverSelection));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_FlashID: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamLink5DD_FlashID));

  //
  // MipiCam Flash0 specific
  //
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash0Model:       0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash0Model));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash0UserHid:       %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash0UserHid));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash0Mode:        0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash0Mode));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash0ModuleName:    %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash0ModuleName));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash0I2cBus:      0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash0I2cBus));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash0I2cAddrDev:  0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash0I2cAddrDev));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioSupport: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioSupport));
  if (mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioSupport == 1) {
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioGroupPadNumber: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioGroupPadNumber));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioComNumber:      0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioComNumber));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioGroupNumber:    0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioGroupNumber));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioActiveValue:    0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioActiveValue));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioInitialValue:   0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioInitialValue));
  }
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash0OperatingMode:        0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash0OperatingMode));


  //
  // MipiCam Flash1 specific
  //
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash1Model:       0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash1Model));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash1UserHid:       %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash1UserHid));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash1Mode:        0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash1Mode));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash1ModuleName:    %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash1ModuleName));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash1I2cBus:      0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash1I2cBus));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash1I2cAddrDev:  0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash1I2cAddrDev));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioSupport: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioSupport));
  if (mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioSupport == 1) {
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioGroupPadNumber: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioGroupPadNumber));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioComNumber:      0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioComNumber));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioGroupNumber:    0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioGroupNumber));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioActiveValue:    0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioActiveValue));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioInitialValue:   0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash1GpioInitialValue));
  }
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash1OperatingMode:        0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash1OperatingMode));


  //
  // MipiCam Flash2 specific
  //
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash2Model:       0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash2Model));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash2UserHid:       %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash2UserHid));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash2Mode:        0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash2Mode));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash2ModuleName:    %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash2ModuleName));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash2I2cBus:      0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash2I2cBus));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash2I2cAddrDev:  0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash2I2cAddrDev));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioSupport: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioSupport));
  if (mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioSupport == 1) {
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioGroupPadNumber: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioGroupPadNumber));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioComNumber:      0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioComNumber));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioGroupNumber:    0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioGroupNumber));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioActiveValue:    0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioActiveValue));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioInitialValue:   0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash2GpioInitialValue));
  }
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash2OperatingMode:        0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash2OperatingMode));


  //
  // MipiCam Flash3 specific
  //
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash3Model:       0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash3Model));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash3UserHid:       %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash3UserHid));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash3Mode:        0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash3Mode));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash3ModuleName:    %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash3ModuleName));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash3I2cBus:      0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash3I2cBus));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash3I2cAddrDev:  0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash3I2cAddrDev));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioSupport: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioSupport));
  if (mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioSupport == 1) {
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioGroupPadNumber: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioGroupPadNumber));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioComNumber:      0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioComNumber));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioGroupNumber:    0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioGroupNumber));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioActiveValue:    0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioActiveValue));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioInitialValue:   0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash3GpioInitialValue));
  }
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash3OperatingMode:        0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash3OperatingMode));

  //
  // MipiCam Flash4 specific
  //
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash4Model:       0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash4Model));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash4UserHid:       %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash4UserHid));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash4Mode:        0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash4Mode));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash4ModuleName:    %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash4ModuleName));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash4I2cBus:      0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash4I2cBus));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash4I2cAddrDev:  0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash4I2cAddrDev));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioSupport: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioSupport));
  if (mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioSupport == 1) {
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioGroupPadNumber: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioGroupPadNumber));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioComNumber:      0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioComNumber));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioGroupNumber:    0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioGroupNumber));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioActiveValue:    0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioActiveValue));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioInitialValue:   0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash4GpioInitialValue));
  }
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash4OperatingMode:        0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash4OperatingMode));

  //
  // MipiCam Flash5 specific
  //
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash5Model:       0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash5Model));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash5UserHid:       %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash5UserHid));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash5Mode:        0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash5Mode));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash5ModuleName:    %a\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash5ModuleName));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash5I2cBus:      0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash5I2cBus));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash5I2cAddrDev:  0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash5I2cAddrDev));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash0GpioSupport: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash5GpioSupport));
  if (mMipiCamNvsAreaProtocol.Area->MipiCamFlash5GpioSupport == 1) {
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash5GpioGroupPadNumber: 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash5GpioGroupPadNumber));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash5GpioGroupNumber:    0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash5GpioGroupNumber));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash5GpioComNumber:      0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash5GpioComNumber));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash5GpioActiveValue:    0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash5GpioActiveValue));
    DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash5GpioInitialValue:   0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash5GpioInitialValue));
  }
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamFlash5OperatingMode:        0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamFlash5OperatingMode));

  //
  // MipiCam Hob Data
  //
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamPlatformId:                 0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamPlatformId));
  DEBUG ((DEBUG_INFO, "mMipiCamNvsAreaProtocol.Area->MipiCamEmulationConfig:            0x%x\n", mMipiCamNvsAreaProtocol.Area->MipiCamEmulationConfig));

  DEBUG ((DEBUG_INFO, "----- Dump mMipiCamNvsAreaProtocol End -----\n"));
}

VOID
DumpMipiCamSetupData (
  VOID
  )
{
  UINTN  Index;

  DEBUG ((DEBUG_INFO, "----- Dump gMipiCamSetupData Start -----\n"));

  //
  // MipiCam sensor configuration
  //
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic0: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic0));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic1: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic1));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic2: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic2));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic3: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic3));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic4: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic4));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic5: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic5));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0: 0x%x\n", gMipiCamSetupData->MipiCam_Link0));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1: 0x%x\n", gMipiCamSetupData->MipiCam_Link1));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2: 0x%x\n", gMipiCamSetupData->MipiCam_Link2));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3: 0x%x\n", gMipiCamSetupData->MipiCam_Link3));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4: 0x%x\n", gMipiCamSetupData->MipiCam_Link4));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5: 0x%x\n", gMipiCamSetupData->MipiCam_Link5));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_LanesClkDiv: 0x%x\n", gMipiCamSetupData->MipiCam_LanesClkDiv));

  //
  // MipiCam Control Logic 0 specific
  //
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic0_Type: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic0_Type));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic0_CrdVersion: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic0_CrdVersion));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic0_InputClock: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic0_InputClock));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic0_GpioPinsEnabled: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic0_GpioPinsEnabled));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic0_I2cChannel: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic0_I2cChannel));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic0_I2cAddress: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic0_I2cAddress));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic0_Pld: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic0_Pld));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic0_Wled1FlashMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic0_Wled1FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic0_Wled1TorchMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic0_Wled1TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic0_Wled2FlashMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic0_Wled2FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic0_Wled2TorchMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic0_Wled2TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic0_Wled1Type: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic0_Wled1Type));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic0_Wled2Type: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic0_Wled2Type));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic0_PchClockSource: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic0_PchClockSource));

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic0_GpioGroupPadNumber[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic0_GpioGroupPadNumber[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic0_GpioComNumber[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic0_GpioComNumber[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic0_GpioGroupNumber[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic0_GpioGroupNumber[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic0_GpioFunction[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic0_GpioFunction[Index]));
    if(gMipiCamSetupData->MipiCam_ControlLogic0_GpioFunction[Index] == 0xFF) {
      DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic0_GpioFunction_User[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic0_GpioFunction_User[Index]));
    }
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic0_GpioActiveValue[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic0_GpioActiveValue[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic0_GpioInitialValue[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic0_GpioInitialValue[Index]));
  }

  //
  // MipiCam Control Logic 1 specific
  //
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic1_Type: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic1_Type));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic1_CrdVersion: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic1_CrdVersion));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic1_InputClock: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic1_InputClock));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic1_GpioPinsEnabled: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic1_GpioPinsEnabled));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic1_I2cChannel: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic1_I2cChannel));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic1_I2cAddress: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic1_I2cAddress));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic1_Pld: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic1_Pld));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic1_Wled1FlashMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic1_Wled1FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic1_Wled1TorchMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic1_Wled1TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic1_Wled2FlashMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic1_Wled2FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic1_Wled2TorchMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic1_Wled2TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic1_Wled1Type: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic1_Wled1Type));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic1_Wled2Type: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic1_Wled2Type));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic1_PchClockSource: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic1_PchClockSource));

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic1_GpioGroupPadNumber[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic1_GpioGroupPadNumber[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic1_GpioComNumber[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic1_GpioComNumber[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic1_GpioGroupNumber[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic1_GpioGroupNumber[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic1_GpioFunction[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic1_GpioFunction[Index]));
    if(gMipiCamSetupData->MipiCam_ControlLogic1_GpioFunction[Index] == 0xFF) {
      DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic1_GpioFunction_User[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic1_GpioFunction_User[Index]));
    }
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic1_GpioActiveValue[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic1_GpioActiveValue[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic1_GpioInitialValue[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic1_GpioInitialValue[Index]));
  }

  //
  // MipiCam Control Logic 2 specific
  //
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic2_Type: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic2_Type));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic2_CrdVersion: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic2_CrdVersion));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic2_InputClock: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic2_InputClock));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic2_GpioPinsEnabled: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic2_GpioPinsEnabled));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic2_I2cChannel: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic2_I2cChannel));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic2_I2cAddress: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic2_I2cAddress));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic2_Pld: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic2_Pld));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic2_Wled1FlashMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic2_Wled1FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic2_Wled1TorchMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic2_Wled1TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic2_Wled2FlashMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic2_Wled2FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic2_Wled2TorchMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic2_Wled2TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic2_Wled1Type: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic2_Wled1Type));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic2_Wled2Type: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic2_Wled2Type));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic2_PchClockSource: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic2_PchClockSource));

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic2_GpioGroupPadNumber[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic2_GpioGroupPadNumber[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic2_GpioComNumber[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic2_GpioComNumber[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic2_GpioGroupNumber[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic2_GpioGroupNumber[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic2_GpioFunction[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic2_GpioFunction[Index]));
    if(gMipiCamSetupData->MipiCam_ControlLogic2_GpioFunction[Index] == 0xFF) {
      DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic2_GpioFunction_User[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic2_GpioFunction_User[Index]));
    }
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic2_GpioActiveValue[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic2_GpioActiveValue[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic2_GpioInitialValue[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic2_GpioInitialValue[Index]));
  }

  //
  // MipiCam Control Logic 3 specific
  //
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic3_Type: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic3_Type));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic3_CrdVersion: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic3_CrdVersion));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic3_InputClock: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic3_InputClock));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic3_GpioPinsEnabled: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic3_GpioPinsEnabled));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic3_I2cChannel: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic3_I2cChannel));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic3_I2cAddress: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic3_I2cAddress));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic3_Pld: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic3_Pld));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic3_Wled1FlashMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic3_Wled1FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic3_Wled1TorchMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic3_Wled1TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic3_Wled2FlashMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic3_Wled2FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic3_Wled2TorchMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic3_Wled2TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic3_Wled1Type: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic3_Wled1Type));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic3_Wled2Type: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic3_Wled2Type));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic3_PchClockSource: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic3_PchClockSource));

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic3_GpioGroupPadNumber[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic3_GpioGroupPadNumber[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic3_GpioComNumber[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic3_GpioComNumber[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic3_GpioGroupNumber[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic3_GpioGroupNumber[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic3_GpioFunction[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic3_GpioFunction[Index]));
    if(gMipiCamSetupData->MipiCam_ControlLogic3_GpioFunction[Index] == 0xFF) {
      DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic3_GpioFunction_User[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic3_GpioFunction_User[Index]));
    }
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic3_GpioActiveValue[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic3_GpioActiveValue[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic3_GpioInitialValue[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic3_GpioInitialValue[Index]));
  }

  //
  // MipiCam Control Logic 4 specific
  //
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic4_Type: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic4_Type));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic4_CrdVersion: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic4_CrdVersion));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic4_InputClock: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic4_InputClock));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic4_GpioPinsEnabled: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic4_GpioPinsEnabled));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic4_I2cChannel: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic4_I2cChannel));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic4_I2cAddress: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic4_I2cAddress));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic4_Pld: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic4_Pld));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic4_Wled1FlashMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic4_Wled1FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic4_Wled1TorchMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic4_Wled1TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic4_Wled2FlashMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic4_Wled2FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic4_Wled2TorchMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic4_Wled2TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic4_Wled1Type: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic4_Wled1Type));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic4_Wled2Type: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic4_Wled2Type));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic4_PchClockSource: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic4_PchClockSource));

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic4_GpioGroupPadNumber[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic4_GpioGroupPadNumber[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic4_GpioComNumber[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic4_GpioComNumber[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic4_GpioGroupNumber[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic4_GpioGroupNumber[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic4_GpioFunction[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic4_GpioFunction[Index]));
    if(gMipiCamSetupData->MipiCam_ControlLogic4_GpioFunction[Index] == 0xFF) {
      DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic4_GpioFunction_User[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic4_GpioFunction_User[Index]));
    }
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic4_GpioActiveValue[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic4_GpioActiveValue[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic4_GpioInitialValue[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic4_GpioInitialValue[Index]));
  }

  //
  // MipiCam Control Logic 5 specific
  //
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic5_Type: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic5_Type));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic5_CrdVersion: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic5_CrdVersion));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic5_InputClock: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic5_InputClock));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic5_GpioPinsEnabled: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic5_GpioPinsEnabled));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic5_I2cChannel: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic5_I2cChannel));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic5_I2cAddress: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic5_I2cAddress));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic5_Pld: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic5_Pld));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic5_Wled1FlashMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic5_Wled1FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic5_Wled1TorchMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic5_Wled1TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic5_Wled2FlashMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic5_Wled2FlashMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic5_Wled2TorchMaxCurrent: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic5_Wled2TorchMaxCurrent));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic5_Wled1Type: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic5_Wled1Type));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic5_Wled2Type: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic5_Wled2Type));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic5_PchClockSource: 0x%x\n", gMipiCamSetupData->MipiCam_ControlLogic5_PchClockSource));

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic5_GpioGroupPadNumber[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic5_GpioGroupPadNumber[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic5_GpioComNumber[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic5_GpioComNumber[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic5_GpioGroupNumber[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic5_GpioGroupNumber[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic5_GpioFunction[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic5_GpioFunction[Index]));
    if(gMipiCamSetupData->MipiCam_ControlLogic5_GpioFunction[Index] == 0xFF) {
      DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic5_GpioFunction_User[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic5_GpioFunction_User[Index]));
    }
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic5_GpioActiveValue[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic5_GpioActiveValue[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_ControlLogic5_GpioInitialValue[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_ControlLogic5_GpioInitialValue[Index]));
  }

  //
  // MipiCam Link0 specific
  //
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_SensorModel: 0x%x\n", gMipiCamSetupData->MipiCam_Link0_SensorModel));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_CameraPhysicalLocation: 0x%x\n", gMipiCamSetupData->MipiCam_Link0_CameraPhysicalLocation));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_ModuleName: %s\n", gMipiCamSetupData->MipiCam_Link0_ModuleName));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_UserHid: %s\n", gMipiCamSetupData->MipiCam_Link0_UserHid));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_I2cDevicesEnabled: 0x%x\n", gMipiCamSetupData->MipiCam_Link0_I2cDevicesEnabled));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_I2cChannel: 0x%x\n", gMipiCamSetupData->MipiCam_Link0_I2cChannel));

  //
  // MipiCam Link0 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_I2cAddress[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_Link0_I2cAddress[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_I2cDeviceType[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_Link0_I2cDeviceType[Index]));
  }
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_DriverData_CrdVersion: 0x%x\n", gMipiCamSetupData->MipiCam_Link0_DriverData_CrdVersion));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_DriverData_LinkUsed: 0x%x\n", gMipiCamSetupData->MipiCam_Link0_DriverData_LinkUsed));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_DriverData_LaneUsed: 0x%x\n", gMipiCamSetupData->MipiCam_Link0_DriverData_LaneUsed));
  if (gMipiCamSetupData->MipiCam_Link0_DriverData_EepromType == 0xFF) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_DriverData_EepromTypeCustom: 0x%x\n", gMipiCamSetupData->MipiCam_Link0_DriverData_EepromTypeCustom));
  } else {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_DriverData_EepromType: 0x%x\n", gMipiCamSetupData->MipiCam_Link0_DriverData_EepromType));
  }
  if (gMipiCamSetupData->MipiCam_Link0_DriverData_VcmType == 0xFF) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_DriverData_VcmTypeCustom: 0x%x\n", gMipiCamSetupData->MipiCam_Link0_DriverData_VcmTypeCustom));
  } else {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_DriverData_VcmType: 0x%x\n", gMipiCamSetupData->MipiCam_Link0_DriverData_VcmType));
  }
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_DriverData_Mclk: 0x%x\n", gMipiCamSetupData->MipiCam_Link0_DriverData_Mclk));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_DriverData_ControlLogic: 0x%x\n", gMipiCamSetupData->MipiCam_Link0_DriverData_ControlLogic));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_DriverData_FlashSupport: 0x%x\n", gMipiCamSetupData->MipiCam_Link0_DriverData_FlashSupport));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_DriverData_PrivacyLed: 0x%x\n", gMipiCamSetupData->MipiCam_Link0_DriverData_PrivacyLed));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_DriverData_Degree: 0x%x\n", gMipiCamSetupData->MipiCam_Link0_DriverData_Degree));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_DriverData_PmicPosition: 0x%x\n", gMipiCamSetupData->MipiCam_Link0_DriverData_PmicPosition));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_DriverData_VoltageRail: 0x%x\n", gMipiCamSetupData->MipiCam_Link0_DriverData_VoltageRail));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_DriverData_PhyConfiguration: 0x%x\n", gMipiCamSetupData->MipiCam_Link0_DriverData_PhyConfiguration));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_DriverData_LaneConfiguration: 0x%x\n", gMipiCamSetupData->MipiCam_Link0_DriverData_LaneConfiguration));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_FlashDriverSelection: 0x%x\n", gMipiCamSetupData->MipiCam_Link0_FlashDriverSelection));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link0_DriverData_FlashID: 0x%x\n", gMipiCamSetupData->MipiCam_Link0_DriverData_FlashID));

  //
  // MipiCam Link1 specific
  //
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_SensorModel: 0x%x\n", gMipiCamSetupData->MipiCam_Link1_SensorModel));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_CameraPhysicalLocation: 0x%x\n", gMipiCamSetupData->MipiCam_Link1_CameraPhysicalLocation));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_ModuleName: %s\n", gMipiCamSetupData->MipiCam_Link1_ModuleName));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_UserHid: %s\n", gMipiCamSetupData->MipiCam_Link1_UserHid));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_I2cDevicesEnabled: 0x%x\n", gMipiCamSetupData->MipiCam_Link1_I2cDevicesEnabled));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_I2cChannel: 0x%x\n", gMipiCamSetupData->MipiCam_Link1_I2cChannel));

  //
  // MipiCam Link1 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_I2cAddress[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_Link1_I2cAddress[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_I2cDeviceType[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_Link1_I2cDeviceType[Index]));
  }
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_DriverData_CrdVersion: 0x%x\n", gMipiCamSetupData->MipiCam_Link1_DriverData_CrdVersion));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_DriverData_LinkUsed: 0x%x\n", gMipiCamSetupData->MipiCam_Link1_DriverData_LinkUsed));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_DriverData_LaneUsed: 0x%x\n", gMipiCamSetupData->MipiCam_Link1_DriverData_LaneUsed));
  if (gMipiCamSetupData->MipiCam_Link1_DriverData_EepromType == 0xFF) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_DriverData_EepromTypeCustom: 0x%x\n", gMipiCamSetupData->MipiCam_Link1_DriverData_EepromTypeCustom));
  } else {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_DriverData_EepromType: 0x%x\n", gMipiCamSetupData->MipiCam_Link1_DriverData_EepromType));
  }
  if (gMipiCamSetupData->MipiCam_Link1_DriverData_VcmType == 0xFF) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_DriverData_VcmTypeCustom: 0x%x\n", gMipiCamSetupData->MipiCam_Link1_DriverData_VcmTypeCustom));
  } else {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_DriverData_VcmType: 0x%x\n", gMipiCamSetupData->MipiCam_Link1_DriverData_VcmType));
  }
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_DriverData_Mclk: 0x%x\n", gMipiCamSetupData->MipiCam_Link1_DriverData_Mclk));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_DriverData_ControlLogic: 0x%x\n", gMipiCamSetupData->MipiCam_Link1_DriverData_ControlLogic));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_DriverData_FlashSupport: 0x%x\n", gMipiCamSetupData->MipiCam_Link1_DriverData_FlashSupport));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_DriverData_PrivacyLed: 0x%x\n", gMipiCamSetupData->MipiCam_Link1_DriverData_PrivacyLed));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_DriverData_Degree: 0x%x\n", gMipiCamSetupData->MipiCam_Link1_DriverData_Degree));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_DriverData_PmicPosition: 0x%x\n", gMipiCamSetupData->MipiCam_Link1_DriverData_PmicPosition));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_DriverData_VoltageRail: 0x%x\n", gMipiCamSetupData->MipiCam_Link1_DriverData_VoltageRail));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_DriverData_PhyConfiguration: 0x%x\n", gMipiCamSetupData->MipiCam_Link1_DriverData_PhyConfiguration));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_DriverData_LaneConfiguration: 0x%x\n", gMipiCamSetupData->MipiCam_Link1_DriverData_LaneConfiguration));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_FlashDriverSelection: 0x%x\n", gMipiCamSetupData->MipiCam_Link1_FlashDriverSelection));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link1_DriverData_FlashID: 0x%x\n", gMipiCamSetupData->MipiCam_Link1_DriverData_FlashID));

  //
  // MipiCam Link2 specific
  //
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_SensorModel: 0x%x\n", gMipiCamSetupData->MipiCam_Link2_SensorModel));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_CameraPhysicalLocation: 0x%x\n", gMipiCamSetupData->MipiCam_Link2_CameraPhysicalLocation));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_ModuleName: %s\n", gMipiCamSetupData->MipiCam_Link2_ModuleName));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_UserHid: %s\n", gMipiCamSetupData->MipiCam_Link2_UserHid));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_I2cDevicesEnabled: 0x%x\n", gMipiCamSetupData->MipiCam_Link2_I2cDevicesEnabled));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_I2cChannel: 0x%x\n", gMipiCamSetupData->MipiCam_Link2_I2cChannel));

  //
  // MipiCam Link2 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_I2cAddress[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_Link2_I2cAddress[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_I2cDeviceType[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_Link2_I2cDeviceType[Index]));
  }
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_DriverData_CrdVersion: 0x%x\n", gMipiCamSetupData->MipiCam_Link2_DriverData_CrdVersion));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_DriverData_LinkUsed: 0x%x\n", gMipiCamSetupData->MipiCam_Link2_DriverData_LinkUsed));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_DriverData_LaneUsed: 0x%x\n", gMipiCamSetupData->MipiCam_Link2_DriverData_LaneUsed));
  if (gMipiCamSetupData->MipiCam_Link2_DriverData_EepromType == 0xFF) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_DriverData_EepromTypeCustom: 0x%x\n", gMipiCamSetupData->MipiCam_Link2_DriverData_EepromTypeCustom));
  } else {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_DriverData_EepromType: 0x%x\n", gMipiCamSetupData->MipiCam_Link2_DriverData_EepromType));
  }
  if (gMipiCamSetupData->MipiCam_Link2_DriverData_VcmType == 0xFF) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_DriverData_VcmTypeCustom: 0x%x\n", gMipiCamSetupData->MipiCam_Link2_DriverData_VcmTypeCustom));
  } else {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_DriverData_VcmType: 0x%x\n", gMipiCamSetupData->MipiCam_Link2_DriverData_VcmType));
  }
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_DriverData_Mclk: 0x%x\n", gMipiCamSetupData->MipiCam_Link2_DriverData_Mclk));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_DriverData_ControlLogic: 0x%x\n", gMipiCamSetupData->MipiCam_Link2_DriverData_ControlLogic));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_DriverData_FlashSupport: 0x%x\n", gMipiCamSetupData->MipiCam_Link2_DriverData_FlashSupport));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_DriverData_PrivacyLed: 0x%x\n", gMipiCamSetupData->MipiCam_Link2_DriverData_PrivacyLed));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_DriverData_Degree: 0x%x\n", gMipiCamSetupData->MipiCam_Link2_DriverData_Degree));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_DriverData_PmicPosition: 0x%x\n", gMipiCamSetupData->MipiCam_Link2_DriverData_PmicPosition));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_DriverData_VoltageRail: 0x%x\n", gMipiCamSetupData->MipiCam_Link2_DriverData_VoltageRail));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_DriverData_PhyConfiguration: 0x%x\n", gMipiCamSetupData->MipiCam_Link2_DriverData_PhyConfiguration));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_DriverData_LaneConfiguration: 0x%x\n", gMipiCamSetupData->MipiCam_Link2_DriverData_LaneConfiguration));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_FlashDriverSelection: 0x%x\n", gMipiCamSetupData->MipiCam_Link2_FlashDriverSelection));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link2_DriverData_FlashID: 0x%x\n", gMipiCamSetupData->MipiCam_Link2_DriverData_FlashID));

  //
  // MipiCam Link3 specific
  //
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_SensorModel: 0x%x\n", gMipiCamSetupData->MipiCam_Link3_SensorModel));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_CameraPhysicalLocation: 0x%x\n", gMipiCamSetupData->MipiCam_Link3_CameraPhysicalLocation));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_ModuleName: %s\n", gMipiCamSetupData->MipiCam_Link3_ModuleName));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_UserHid: %s\n", gMipiCamSetupData->MipiCam_Link3_UserHid));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_I2cDevicesEnabled: 0x%x\n", gMipiCamSetupData->MipiCam_Link3_I2cDevicesEnabled));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_I2cChannel: 0x%x\n", gMipiCamSetupData->MipiCam_Link3_I2cChannel));

  //
  // MipiCam Link3 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_I2cAddress[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_Link3_I2cAddress[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_I2cDeviceType[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_Link3_I2cDeviceType[Index]));
  }
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_DriverData_CrdVersion: 0x%x\n", gMipiCamSetupData->MipiCam_Link3_DriverData_CrdVersion));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_DriverData_LinkUsed: 0x%x\n", gMipiCamSetupData->MipiCam_Link3_DriverData_LinkUsed));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_DriverData_LaneUsed: 0x%x\n", gMipiCamSetupData->MipiCam_Link3_DriverData_LaneUsed));
  if (gMipiCamSetupData->MipiCam_Link3_DriverData_EepromType == 0xFF) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_DriverData_EepromTypeCustom: 0x%x\n", gMipiCamSetupData->MipiCam_Link3_DriverData_EepromTypeCustom));
  } else {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_DriverData_EepromType: 0x%x\n", gMipiCamSetupData->MipiCam_Link3_DriverData_EepromType));
  }
  if (gMipiCamSetupData->MipiCam_Link3_DriverData_VcmType == 0xFF) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_DriverData_VcmTypeCustom: 0x%x\n", gMipiCamSetupData->MipiCam_Link3_DriverData_VcmTypeCustom));
  } else {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_DriverData_VcmType: 0x%x\n", gMipiCamSetupData->MipiCam_Link3_DriverData_VcmType));
  }
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_DriverData_Mclk: 0x%x\n", gMipiCamSetupData->MipiCam_Link3_DriverData_Mclk));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_DriverData_ControlLogic: 0x%x\n", gMipiCamSetupData->MipiCam_Link3_DriverData_ControlLogic));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_DriverData_FlashSupport: 0x%x\n", gMipiCamSetupData->MipiCam_Link3_DriverData_FlashSupport));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_DriverData_PrivacyLed: 0x%x\n", gMipiCamSetupData->MipiCam_Link3_DriverData_PrivacyLed));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_DriverData_Degree: 0x%x\n", gMipiCamSetupData->MipiCam_Link3_DriverData_Degree));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_DriverData_PmicPosition: 0x%x\n", gMipiCamSetupData->MipiCam_Link3_DriverData_PmicPosition));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_DriverData_VoltageRail: 0x%x\n", gMipiCamSetupData->MipiCam_Link3_DriverData_VoltageRail));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_DriverData_PhyConfiguration: 0x%x\n", gMipiCamSetupData->MipiCam_Link3_DriverData_PhyConfiguration));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_DriverData_LaneConfiguration: 0x%x\n", gMipiCamSetupData->MipiCam_Link3_DriverData_LaneConfiguration));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_FlashDriverSelection: 0x%x\n", gMipiCamSetupData->MipiCam_Link3_FlashDriverSelection));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link3_DriverData_FlashID: 0x%x\n", gMipiCamSetupData->MipiCam_Link3_DriverData_FlashID));

  //
  // MipiCam Link4 specific
  //
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_SensorModel: 0x%x\n", gMipiCamSetupData->MipiCam_Link4_SensorModel));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_CameraPhysicalLocation: 0x%x\n", gMipiCamSetupData->MipiCam_Link4_CameraPhysicalLocation));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_ModuleName: %s\n", gMipiCamSetupData->MipiCam_Link4_ModuleName));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_UserHid: %s\n", gMipiCamSetupData->MipiCam_Link4_UserHid));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_I2cDevicesEnabled: 0x%x\n", gMipiCamSetupData->MipiCam_Link4_I2cDevicesEnabled));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_I2cChannel: 0x%x\n", gMipiCamSetupData->MipiCam_Link4_I2cChannel));

  //
  // MipiCam Link4 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_I2cAddress[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_Link4_I2cAddress[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_I2cDeviceType[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_Link4_I2cDeviceType[Index]));
  }
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_DriverData_CrdVersion: 0x%x\n", gMipiCamSetupData->MipiCam_Link4_DriverData_CrdVersion));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_DriverData_LinkUsed: 0x%x\n", gMipiCamSetupData->MipiCam_Link4_DriverData_LinkUsed));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_DriverData_LaneUsed: 0x%x\n", gMipiCamSetupData->MipiCam_Link4_DriverData_LaneUsed));
  if (gMipiCamSetupData->MipiCam_Link4_DriverData_EepromType == 0xFF) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_DriverData_EepromTypeCustom: 0x%x\n", gMipiCamSetupData->MipiCam_Link4_DriverData_EepromTypeCustom));
  } else {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_DriverData_EepromType: 0x%x\n", gMipiCamSetupData->MipiCam_Link4_DriverData_EepromType));
  }
  if (gMipiCamSetupData->MipiCam_Link4_DriverData_VcmType == 0xFF) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_DriverData_VcmTypeCustom: 0x%x\n", gMipiCamSetupData->MipiCam_Link4_DriverData_VcmTypeCustom));
  } else {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_DriverData_VcmType: 0x%x\n", gMipiCamSetupData->MipiCam_Link4_DriverData_VcmType));
  }
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_DriverData_Mclk: 0x%x\n", gMipiCamSetupData->MipiCam_Link4_DriverData_Mclk));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_DriverData_ControlLogic: 0x%x\n", gMipiCamSetupData->MipiCam_Link4_DriverData_ControlLogic));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_DriverData_FlashSupport: 0x%x\n", gMipiCamSetupData->MipiCam_Link4_DriverData_FlashSupport));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_DriverData_PrivacyLed: 0x%x\n", gMipiCamSetupData->MipiCam_Link4_DriverData_PrivacyLed));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_DriverData_Degree: 0x%x\n", gMipiCamSetupData->MipiCam_Link4_DriverData_Degree));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_DriverData_PmicPosition: 0x%x\n", gMipiCamSetupData->MipiCam_Link4_DriverData_PmicPosition));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_DriverData_VoltageRail: 0x%x\n", gMipiCamSetupData->MipiCam_Link4_DriverData_VoltageRail));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_DriverData_PhyConfiguration: 0x%x\n", gMipiCamSetupData->MipiCam_Link4_DriverData_PhyConfiguration));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_DriverData_LaneConfiguration: 0x%x\n", gMipiCamSetupData->MipiCam_Link4_DriverData_LaneConfiguration));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_FlashDriverSelection: 0x%x\n", gMipiCamSetupData->MipiCam_Link4_FlashDriverSelection));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link4_DriverData_FlashID: 0x%x\n", gMipiCamSetupData->MipiCam_Link4_DriverData_FlashID));

  //
  // MipiCam Link5 specific
  //
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_SensorModel: 0x%x\n", gMipiCamSetupData->MipiCam_Link5_SensorModel));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_CameraPhysicalLocation: 0x%x\n", gMipiCamSetupData->MipiCam_Link5_CameraPhysicalLocation));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_ModuleName: %s\n", gMipiCamSetupData->MipiCam_Link5_ModuleName));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_UserHid: %s\n", gMipiCamSetupData->MipiCam_Link5_UserHid));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_I2cDevicesEnabled: 0x%x\n", gMipiCamSetupData->MipiCam_Link5_I2cDevicesEnabled));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_I2cChannel: 0x%x\n", gMipiCamSetupData->MipiCam_Link5_I2cChannel));

  //
  // MipiCam Link5 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_I2cAddress[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_Link5_I2cAddress[Index]));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_I2cDeviceType[%d]: 0x%x\n", Index, gMipiCamSetupData->MipiCam_Link5_I2cDeviceType[Index]));
  }
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_DriverData_CrdVersion: 0x%x\n", gMipiCamSetupData->MipiCam_Link5_DriverData_CrdVersion));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_DriverData_LinkUsed: 0x%x\n", gMipiCamSetupData->MipiCam_Link5_DriverData_LinkUsed));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_DriverData_LaneUsed: 0x%x\n", gMipiCamSetupData->MipiCam_Link5_DriverData_LaneUsed));
  if (gMipiCamSetupData->MipiCam_Link5_DriverData_EepromType == 0xFF) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_DriverData_EepromTypeCustom: 0x%x\n", gMipiCamSetupData->MipiCam_Link5_DriverData_EepromTypeCustom));
  } else {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_DriverData_EepromType: 0x%x\n", gMipiCamSetupData->MipiCam_Link5_DriverData_EepromType));
  }
  if (gMipiCamSetupData->MipiCam_Link5_DriverData_VcmType == 0xFF) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_DriverData_VcmTypeCustom: 0x%x\n", gMipiCamSetupData->MipiCam_Link5_DriverData_VcmTypeCustom));
  } else {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_DriverData_VcmType: 0x%x\n", gMipiCamSetupData->MipiCam_Link5_DriverData_VcmType));
  }
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_DriverData_Mclk: 0x%x\n", gMipiCamSetupData->MipiCam_Link5_DriverData_Mclk));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_DriverData_ControlLogic: 0x%x\n", gMipiCamSetupData->MipiCam_Link5_DriverData_ControlLogic));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_DriverData_FlashSupport: 0x%x\n", gMipiCamSetupData->MipiCam_Link5_DriverData_FlashSupport));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_DriverData_PrivacyLed: 0x%x\n", gMipiCamSetupData->MipiCam_Link5_DriverData_PrivacyLed));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_DriverData_Degree: 0x%x\n", gMipiCamSetupData->MipiCam_Link5_DriverData_Degree));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_DriverData_PmicPosition: 0x%x\n", gMipiCamSetupData->MipiCam_Link5_DriverData_PmicPosition));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_DriverData_VoltageRail: 0x%x\n", gMipiCamSetupData->MipiCam_Link5_DriverData_VoltageRail));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_DriverData_PhyConfiguration: 0x%x\n", gMipiCamSetupData->MipiCam_Link5_DriverData_PhyConfiguration));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_DriverData_LaneConfiguration: 0x%x\n", gMipiCamSetupData->MipiCam_Link5_DriverData_LaneConfiguration));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_FlashDriverSelection: 0x%x\n", gMipiCamSetupData->MipiCam_Link5_FlashDriverSelection));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Link5_DriverData_FlashID: 0x%x\n", gMipiCamSetupData->MipiCam_Link5_DriverData_FlashID));

  //
  // MipiCam Flash0 specific
  //
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash0_Model:       0x%x\n", gMipiCamSetupData->MipiCam_Flash0_Model));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash0_UserHid:       %s\n", gMipiCamSetupData->MipiCam_Flash0_UserHid));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash0_ModuleName:    %s\n", gMipiCamSetupData->MipiCam_Flash0_ModuleName));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash0_I2cChannel:  0x%x\n", gMipiCamSetupData->MipiCam_Flash0_I2cChannel));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash0_I2cAddress:  0x%x\n", gMipiCamSetupData->MipiCam_Flash0_I2cAddress));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash0_GpioSupport: 0x%x\n", gMipiCamSetupData->MipiCam_Flash0_GpioSupport));
  if (gMipiCamSetupData->MipiCam_Flash0_GpioSupport == 1) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash0_GpioGroupPadNumber: 0x%x\n", gMipiCamSetupData->MipiCam_Flash0_GpioGroupPadNumber));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash0_GpioComNumber:      0x%x\n", gMipiCamSetupData->MipiCam_Flash0_GpioComNumber));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash0_GpioGroupNumber:    0x%x\n", gMipiCamSetupData->MipiCam_Flash0_GpioGroupNumber));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash0_GpioActiveValue:    0x%x\n", gMipiCamSetupData->MipiCam_Flash0_GpioActiveValue));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash0_GpioInitialValue:   0x%x\n", gMipiCamSetupData->MipiCam_Flash0_GpioInitialValue));
  }
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash0_OperatingMode:        0x%x\n", gMipiCamSetupData->MipiCam_Flash0_OperatingMode));

  //
  // MipiCam Flash1 specific
  //
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash1_Model:       0x%x\n", gMipiCamSetupData->MipiCam_Flash1_Model));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash1_UserHid:       %s\n", gMipiCamSetupData->MipiCam_Flash1_UserHid));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash1_ModuleName:    %s\n", gMipiCamSetupData->MipiCam_Flash1_ModuleName));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash1_I2cChannel:  0x%x\n", gMipiCamSetupData->MipiCam_Flash1_I2cChannel));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash1_I2cAddress:  0x%x\n", gMipiCamSetupData->MipiCam_Flash1_I2cAddress));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash1_GpioSupport: 0x%x\n", gMipiCamSetupData->MipiCam_Flash1_GpioSupport));
  if (gMipiCamSetupData->MipiCam_Flash1_GpioSupport == 1) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash1_GpioGroupPadNumber: 0x%x\n", gMipiCamSetupData->MipiCam_Flash1_GpioGroupPadNumber));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash1_GpioComNumber:      0x%x\n", gMipiCamSetupData->MipiCam_Flash1_GpioComNumber));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash1_GpioGroupNumber:    0x%x\n", gMipiCamSetupData->MipiCam_Flash1_GpioGroupNumber));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash1_GpioActiveValue:    0x%x\n", gMipiCamSetupData->MipiCam_Flash1_GpioActiveValue));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash1_GpioInitialValue:   0x%x\n", gMipiCamSetupData->MipiCam_Flash1_GpioInitialValue));
  }
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash1_OperatingMode:        0x%x\n", gMipiCamSetupData->MipiCam_Flash1_OperatingMode));

  //
  // MipiCam Flash2 specific
  //
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash2_Model:       0x%x\n", gMipiCamSetupData->MipiCam_Flash2_Model));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash2_UserHid:       %s\n", gMipiCamSetupData->MipiCam_Flash2_UserHid));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash2_ModuleName:    %s\n", gMipiCamSetupData->MipiCam_Flash2_ModuleName));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash2_I2cChannel:  0x%x\n", gMipiCamSetupData->MipiCam_Flash2_I2cChannel));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash2_I2cAddress:  0x%x\n", gMipiCamSetupData->MipiCam_Flash2_I2cAddress));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash2_GpioSupport: 0x%x\n", gMipiCamSetupData->MipiCam_Flash2_GpioSupport));
  if (gMipiCamSetupData->MipiCam_Flash2_GpioSupport == 1) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash2_GpioGroupPadNumber: 0x%x\n", gMipiCamSetupData->MipiCam_Flash2_GpioGroupPadNumber));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash2_GpioComNumber:      0x%x\n", gMipiCamSetupData->MipiCam_Flash2_GpioComNumber));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash2_GpioGroupNumber:    0x%x\n", gMipiCamSetupData->MipiCam_Flash2_GpioGroupNumber));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash2_GpioActiveValue:    0x%x\n", gMipiCamSetupData->MipiCam_Flash2_GpioActiveValue));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash2_GpioInitialValue:   0x%x\n", gMipiCamSetupData->MipiCam_Flash2_GpioInitialValue));
  }
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash2_OperatingMode:        0x%x\n", gMipiCamSetupData->MipiCam_Flash2_OperatingMode));

  //
  // MipiCam Flash3 specific
  //
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash3_Model:       0x%x\n", gMipiCamSetupData->MipiCam_Flash3_Model));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash3_UserHid:       %s\n", gMipiCamSetupData->MipiCam_Flash3_UserHid));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash3_ModuleName:    %s\n", gMipiCamSetupData->MipiCam_Flash3_ModuleName));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash3_I2cChannel:  0x%x\n", gMipiCamSetupData->MipiCam_Flash3_I2cChannel));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash3_I2cAddress:  0x%x\n", gMipiCamSetupData->MipiCam_Flash3_I2cAddress));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash3_GpioSupport: 0x%x\n", gMipiCamSetupData->MipiCam_Flash3_GpioSupport));
  if (gMipiCamSetupData->MipiCam_Flash3_GpioSupport == 1) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash3_GpioGroupPadNumber: 0x%x\n", gMipiCamSetupData->MipiCam_Flash3_GpioGroupPadNumber));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash3_GpioComNumber:      0x%x\n", gMipiCamSetupData->MipiCam_Flash3_GpioComNumber));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash3_GpioGroupNumber:    0x%x\n", gMipiCamSetupData->MipiCam_Flash3_GpioGroupNumber));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash3_GpioActiveValue:    0x%x\n", gMipiCamSetupData->MipiCam_Flash3_GpioActiveValue));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash3_GpioInitialValue:   0x%x\n", gMipiCamSetupData->MipiCam_Flash3_GpioInitialValue));
  }
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash3_OperatingMode:        0x%x\n", gMipiCamSetupData->MipiCam_Flash3_OperatingMode));

  //
  // MipiCam Flash4 specific
  //
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash4_Model:       0x%x\n", gMipiCamSetupData->MipiCam_Flash4_Model));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash4_UserHid:       %s\n", gMipiCamSetupData->MipiCam_Flash4_UserHid));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash4_ModuleName:    %s\n", gMipiCamSetupData->MipiCam_Flash4_ModuleName));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash4_I2cChannel:  0x%x\n", gMipiCamSetupData->MipiCam_Flash4_I2cChannel));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash4_I2cAddress:  0x%x\n", gMipiCamSetupData->MipiCam_Flash4_I2cAddress));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash4_GpioSupport: 0x%x\n", gMipiCamSetupData->MipiCam_Flash4_GpioSupport));
  if (gMipiCamSetupData->MipiCam_Flash4_GpioSupport == 1) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash4_GpioGroupPadNumber: 0x%x\n", gMipiCamSetupData->MipiCam_Flash4_GpioGroupPadNumber));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash4_GpioComNumber:      0x%x\n", gMipiCamSetupData->MipiCam_Flash4_GpioComNumber));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash4_GpioGroupNumber:    0x%x\n", gMipiCamSetupData->MipiCam_Flash4_GpioGroupNumber));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash4_GpioActiveValue:    0x%x\n", gMipiCamSetupData->MipiCam_Flash4_GpioActiveValue));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash4_GpioInitialValue:   0x%x\n", gMipiCamSetupData->MipiCam_Flash4_GpioInitialValue));
  }
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash4_OperatingMode:        0x%x\n", gMipiCamSetupData->MipiCam_Flash4_OperatingMode));

  //
  // MipiCam Flash5 specific
  //
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash5_Model:       0x%x\n", gMipiCamSetupData->MipiCam_Flash5_Model));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash5_UserHid:       %s\n", gMipiCamSetupData->MipiCam_Flash5_UserHid));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash5_ModuleName:    %s\n", gMipiCamSetupData->MipiCam_Flash5_ModuleName));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash5_I2cChannel:  0x%x\n", gMipiCamSetupData->MipiCam_Flash5_I2cChannel));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash5_I2cAddress:  0x%x\n", gMipiCamSetupData->MipiCam_Flash5_I2cAddress));
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash5_GpioSupport: 0x%x\n", gMipiCamSetupData->MipiCam_Flash5_GpioSupport));
  if (gMipiCamSetupData->MipiCam_Flash5_GpioSupport == 1) {
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash5_GpioGroupPadNumber: 0x%x\n", gMipiCamSetupData->MipiCam_Flash5_GpioGroupPadNumber));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash5_GpioGroupNumber:    0x%x\n", gMipiCamSetupData->MipiCam_Flash5_GpioGroupNumber));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash5_GpioComNumber:      0x%x\n", gMipiCamSetupData->MipiCam_Flash5_GpioComNumber));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash5_GpioActiveValue:    0x%x\n", gMipiCamSetupData->MipiCam_Flash5_GpioActiveValue));
    DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash5_GpioInitialValue:   0x%x\n", gMipiCamSetupData->MipiCam_Flash5_GpioInitialValue));
  }
  DEBUG ((DEBUG_INFO, "gMipiCamSetupData->MipiCam_Flash5_OperatingMode:        0x%x\n", gMipiCamSetupData->MipiCam_Flash5_OperatingMode));

  DEBUG ((DEBUG_INFO, "----- Dump gMipiCamSetupData End -----\n"));
}

VOID
InitializeMipiCamSetupData (
  VOID
  )
{
  UINTN  Index;

  //
  // MipiCam sensor configuration
  //
  gMipiCamSetupData->MipiCam_ControlLogic0 = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic1 = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic2 = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic3 = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic4 = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic5 = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link0         = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link1         = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link2         = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link3         = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link4         = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link5         = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_LanesClkDiv   = (UINT8) rand ();

  //
  // MipiCam Control Logic 0 specific
  //
  gMipiCamSetupData->MipiCam_ControlLogic0_Type                 = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic0_CrdVersion           = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic0_InputClock           = ((UINT32) rand () << 16) + (UINT32) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic0_GpioPinsEnabled      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic0_I2cChannel           = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic0_I2cAddress           = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic0_Pld                  = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic0_Wled1FlashMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic0_Wled1TorchMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic0_Wled2FlashMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic0_Wled2TorchMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic0_Wled1Type            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic0_Wled2Type            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic0_PchClockSource       = (UINT8) rand ();

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    gMipiCamSetupData->MipiCam_ControlLogic0_GpioGroupPadNumber[Index] = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic0_GpioComNumber[Index]      = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic0_GpioGroupNumber[Index]    = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic0_GpioFunction[Index]       = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic0_GpioFunction_User[Index]  = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic0_GpioActiveValue[Index]    = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic0_GpioInitialValue[Index]   = (UINT8) rand ();
  }

  //
  // MipiCam Control Logic 1 specific
  //
  gMipiCamSetupData->MipiCam_ControlLogic1_Type                 = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic1_CrdVersion           = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic1_InputClock           = ((UINT32) rand () << 16) + (UINT32) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic1_GpioPinsEnabled      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic1_I2cChannel           = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic1_I2cAddress           = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic1_Pld                  = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic1_Wled1FlashMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic1_Wled1TorchMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic1_Wled2FlashMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic1_Wled2TorchMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic1_Wled1Type            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic1_Wled2Type            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic1_PchClockSource       = (UINT8) rand ();

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    gMipiCamSetupData->MipiCam_ControlLogic1_GpioGroupPadNumber[Index] = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic1_GpioComNumber[Index]      = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic1_GpioGroupNumber[Index]    = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic1_GpioFunction[Index]       = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic1_GpioFunction_User[Index]  = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic1_GpioActiveValue[Index]    = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic1_GpioInitialValue[Index]   = (UINT8) rand ();
  }

  //
  // MipiCam Control Logic 2 specific
  //
  gMipiCamSetupData->MipiCam_ControlLogic2_Type                 = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic2_CrdVersion           = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic2_InputClock           = ((UINT32) rand () << 16) + (UINT32) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic2_GpioPinsEnabled      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic2_I2cChannel           = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic2_I2cAddress           = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic2_Pld                  = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic2_Wled1FlashMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic2_Wled1TorchMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic2_Wled2FlashMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic2_Wled2TorchMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic2_Wled1Type            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic2_Wled2Type            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic2_PchClockSource       = (UINT8) rand ();

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    gMipiCamSetupData->MipiCam_ControlLogic2_GpioGroupPadNumber[Index] = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic2_GpioComNumber[Index]      = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic2_GpioGroupNumber[Index]    = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic2_GpioFunction[Index]       = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic2_GpioFunction_User[Index]  = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic2_GpioActiveValue[Index]    = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic2_GpioInitialValue[Index]   = (UINT8) rand ();
  }

  //
  // MipiCam Control Logic 3 specific
  //
  gMipiCamSetupData->MipiCam_ControlLogic3_Type                 = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic3_CrdVersion           = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic3_InputClock           = ((UINT32) rand () << 16) + (UINT32) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic3_GpioPinsEnabled      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic3_I2cChannel           = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic3_I2cAddress           = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic3_Pld                  = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic3_Wled1FlashMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic3_Wled1TorchMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic3_Wled2FlashMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic3_Wled2TorchMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic3_Wled1Type            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic3_Wled2Type            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic3_PchClockSource       = (UINT8) rand ();

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    gMipiCamSetupData->MipiCam_ControlLogic3_GpioGroupPadNumber[Index] = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic3_GpioComNumber[Index]      = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic3_GpioGroupNumber[Index]    = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic3_GpioFunction[Index]       = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic3_GpioFunction_User[Index]  = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic3_GpioActiveValue[Index]    = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic3_GpioInitialValue[Index]   = (UINT8) rand ();
  }

  //
  // MipiCam Control Logic 4 specific
  //
  gMipiCamSetupData->MipiCam_ControlLogic4_Type                 = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic4_CrdVersion           = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic4_InputClock           = ((UINT32) rand () << 16) + (UINT32) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic4_GpioPinsEnabled      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic4_I2cChannel           = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic4_I2cAddress           = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic4_Pld                  = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic4_Wled1FlashMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic4_Wled1TorchMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic4_Wled2FlashMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic4_Wled2TorchMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic4_Wled1Type            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic4_Wled2Type            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic4_PchClockSource       = (UINT8) rand ();

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    gMipiCamSetupData->MipiCam_ControlLogic4_GpioGroupPadNumber[Index] = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic4_GpioComNumber[Index]      = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic4_GpioGroupNumber[Index]    = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic4_GpioFunction[Index]       = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic4_GpioFunction_User[Index]  = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic4_GpioActiveValue[Index]    = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic4_GpioInitialValue[Index]   = (UINT8) rand ();
  }

  //
  // MipiCam Control Logic 5 specific
  //
  gMipiCamSetupData->MipiCam_ControlLogic5_Type                 = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic5_CrdVersion           = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic5_InputClock           = ((UINT32) rand () << 16) + (UINT32) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic5_GpioPinsEnabled      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic5_I2cChannel           = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic5_I2cAddress           = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic5_Pld                  = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic5_Wled1FlashMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic5_Wled1TorchMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic5_Wled2FlashMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic5_Wled2TorchMaxCurrent = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic5_Wled1Type            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic5_Wled2Type            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_ControlLogic5_PchClockSource       = (UINT8) rand ();

  for (Index = 0; Index < MIPICAM_GPIO_COUNT; Index++) {
    gMipiCamSetupData->MipiCam_ControlLogic5_GpioGroupPadNumber[Index] = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic5_GpioComNumber[Index]      = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic5_GpioGroupNumber[Index]    = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic5_GpioFunction[Index]       = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic5_GpioFunction_User[Index]  = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic5_GpioActiveValue[Index]    = (UINT8) rand ();
    gMipiCamSetupData->MipiCam_ControlLogic5_GpioInitialValue[Index]   = (UINT8) rand ();
  }

  //
  // MipiCam Link0 specific
  //
  gMipiCamSetupData->MipiCam_Link0_SensorModel            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link0_CameraPhysicalLocation = (UINT8) rand ();
  //
  // ToDo: Replace the CopyMem with StrCpyS, after the alignment issue fix.
  //
  CopyMem (gMipiCamSetupData->MipiCam_Link0_ModuleName, L"A12N08B", sizeof (L"A12N08B"));
  CopyMem (gMipiCamSetupData->MipiCam_Link0_UserHid, L"Link0Hid", sizeof (L"Link0Hid"));
  gMipiCamSetupData->MipiCam_Link0_I2cDevicesEnabled = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link0_I2cChannel        = (UINT8) rand ();

  //
  // MipiCam Link0 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    gMipiCamSetupData->MipiCam_Link0_I2cAddress[Index]    = (UINT16) rand ();
    gMipiCamSetupData->MipiCam_Link0_I2cDeviceType[Index] = (UINT8) rand ();
  }
  for (Index = 0; Index < MIPICAM_CUSTOMIZE_DEVICE_COUNT; Index++) {
    gMipiCamSetupData->MipiCam_Link0_Customize_Id[Index]  = (UINT8) rand ();
  }
  gMipiCamSetupData->MipiCam_Link0_DriverData_CrdVersion = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link0_DriverData_LinkUsed   = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link0_DriverData_LaneUsed   = (UINT8) rand ();
  if (gMipiCamSetupData->MipiCam_Link0_DriverData_EepromType == 0xFF) {
    gMipiCamSetupData->MipiCam_Link0_DriverData_EepromTypeCustom = (UINT8) rand ();
  } else {
    gMipiCamSetupData->MipiCam_Link0_DriverData_EepromType = (UINT8) rand ();
  }
  if (gMipiCamSetupData->MipiCam_Link0_DriverData_VcmType == 0xFF) {
    gMipiCamSetupData->MipiCam_Link0_DriverData_VcmTypeCustom = (UINT8) rand ();
  } else {
    gMipiCamSetupData->MipiCam_Link0_DriverData_VcmType = (UINT8) rand ();
  }
  gMipiCamSetupData->MipiCam_Link0_DriverData_Mclk              = ((UINT32) rand () << 16) + (UINT32) rand ();
  gMipiCamSetupData->MipiCam_Link0_DriverData_ControlLogic      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link0_DriverData_FlashSupport      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link0_DriverData_PrivacyLed        = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link0_DriverData_Degree            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link0_DriverData_PmicPosition      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link0_DriverData_VoltageRail       = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link0_DriverData_PhyConfiguration  = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link0_DriverData_LaneConfiguration = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link0_FlashDriverSelection         = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link0_DriverData_FlashID           = (UINT8) rand ();

  //
  // MipiCam Link1 specific
  //
  gMipiCamSetupData->MipiCam_Link1_SensorModel            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link1_CameraPhysicalLocation = (UINT8) rand ();
  //
  // ToDo: Replace the CopyMem with StrCpyS, after the alignment issue fix.
  //
  CopyMem (gMipiCamSetupData->MipiCam_Link1_ModuleName, L"56B6", sizeof (L"56B6"));
  CopyMem (gMipiCamSetupData->MipiCam_Link1_UserHid, L"Link1Hid", sizeof (L"Link1Hid"));
  gMipiCamSetupData->MipiCam_Link1_I2cDevicesEnabled = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link1_I2cChannel        = (UINT8) rand ();

  //
  // MipiCam Link1 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    gMipiCamSetupData->MipiCam_Link1_I2cAddress[Index]    = (UINT16) rand ();
    gMipiCamSetupData->MipiCam_Link1_I2cDeviceType[Index] = (UINT8) rand ();
  }
  for (Index = 0; Index < MIPICAM_CUSTOMIZE_DEVICE_COUNT; Index++) {
    gMipiCamSetupData->MipiCam_Link1_Customize_Id[Index]  = (UINT8) rand ();
  }
  gMipiCamSetupData->MipiCam_Link1_DriverData_CrdVersion = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link1_DriverData_LinkUsed   = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link1_DriverData_LaneUsed   = (UINT8) rand ();
  if (gMipiCamSetupData->MipiCam_Link1_DriverData_EepromType == 0xFF) {
    gMipiCamSetupData->MipiCam_Link1_DriverData_EepromTypeCustom = (UINT8) rand ();
  } else {
    gMipiCamSetupData->MipiCam_Link1_DriverData_EepromType = (UINT8) rand ();
  }
  if (gMipiCamSetupData->MipiCam_Link1_DriverData_VcmType == 0xFF) {
    gMipiCamSetupData->MipiCam_Link1_DriverData_VcmTypeCustom = (UINT8) rand ();
  } else {
    gMipiCamSetupData->MipiCam_Link1_DriverData_VcmType = (UINT8) rand ();
  }
  gMipiCamSetupData->MipiCam_Link1_DriverData_Mclk              = ((UINT32) rand () << 16) + (UINT32) rand ();
  gMipiCamSetupData->MipiCam_Link1_DriverData_ControlLogic      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link1_DriverData_FlashSupport      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link1_DriverData_PrivacyLed        = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link1_DriverData_Degree            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link1_DriverData_PmicPosition      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link1_DriverData_VoltageRail       = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link1_DriverData_PhyConfiguration  = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link1_DriverData_LaneConfiguration = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link1_FlashDriverSelection         = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link1_DriverData_FlashID           = (UINT8) rand ();

  //
  // MipiCam Link2 specific
  //
  gMipiCamSetupData->MipiCam_Link2_SensorModel            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link2_CameraPhysicalLocation = (UINT8) rand ();
  //
  // ToDo: Replace the CopyMem with StrCpyS, after the alignment issue fix.
  //
  CopyMem (gMipiCamSetupData->MipiCam_Link2_ModuleName, L"A12N08BU", sizeof (L"A12N08BU"));
  CopyMem (gMipiCamSetupData->MipiCam_Link2_UserHid, L"Link2Hid", sizeof (L"Link2Hid"));
  gMipiCamSetupData->MipiCam_Link2_I2cDevicesEnabled = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link2_I2cChannel        = (UINT8) rand ();

  //
  // MipiCam Link2 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    gMipiCamSetupData->MipiCam_Link2_I2cAddress[Index]    = (UINT16) rand ();
    gMipiCamSetupData->MipiCam_Link2_I2cDeviceType[Index] = (UINT8) rand ();
  }
  for (Index = 0; Index < MIPICAM_CUSTOMIZE_DEVICE_COUNT; Index++) {
    gMipiCamSetupData->MipiCam_Link2_Customize_Id[Index]  = (UINT8) rand ();
  }
  gMipiCamSetupData->MipiCam_Link2_DriverData_CrdVersion = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link2_DriverData_LinkUsed   = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link2_DriverData_LaneUsed   = (UINT8) rand ();
  if (gMipiCamSetupData->MipiCam_Link2_DriverData_EepromType == 0xFF) {
    gMipiCamSetupData->MipiCam_Link2_DriverData_EepromTypeCustom = (UINT8) rand ();
  } else {
    gMipiCamSetupData->MipiCam_Link2_DriverData_EepromType = (UINT8) rand ();
  }
  if (gMipiCamSetupData->MipiCam_Link2_DriverData_VcmType == 0xFF) {
    gMipiCamSetupData->MipiCam_Link2_DriverData_VcmTypeCustom = (UINT8) rand ();
  } else {
    gMipiCamSetupData->MipiCam_Link2_DriverData_VcmType = (UINT8) rand ();
  }
  gMipiCamSetupData->MipiCam_Link2_DriverData_Mclk              = ((UINT32) rand () << 16) + (UINT32) rand ();
  gMipiCamSetupData->MipiCam_Link2_DriverData_ControlLogic      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link2_DriverData_FlashSupport      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link2_DriverData_PrivacyLed        = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link2_DriverData_Degree            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link2_DriverData_PmicPosition      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link2_DriverData_VoltageRail       = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link2_DriverData_PhyConfiguration  = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link2_DriverData_LaneConfiguration = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link2_FlashDriverSelection         = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link2_DriverData_FlashID           = (UINT8) rand ();

  //
  // MipiCam Link3 specific
  //
  gMipiCamSetupData->MipiCam_Link3_SensorModel            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link3_CameraPhysicalLocation = (UINT8) rand ();
  //
  // ToDo: Replace the CopyMem with StrCpyS, after the alignment issue fix.
  //
  CopyMem (gMipiCamSetupData->MipiCam_Link3_ModuleName, L"Link3ModuleName", sizeof (L"Link3ModuleName"));
  CopyMem (gMipiCamSetupData->MipiCam_Link3_UserHid, L"Link3Hid", sizeof (L"Link3Hid"));
  gMipiCamSetupData->MipiCam_Link3_I2cDevicesEnabled = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link3_I2cChannel        = (UINT8) rand ();

  //
  // MipiCam Link3 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    gMipiCamSetupData->MipiCam_Link3_I2cAddress[Index]    = (UINT16) rand ();
    gMipiCamSetupData->MipiCam_Link3_I2cDeviceType[Index] = (UINT8) rand ();
  }
  for (Index = 0; Index < MIPICAM_CUSTOMIZE_DEVICE_COUNT; Index++) {
    gMipiCamSetupData->MipiCam_Link3_Customize_Id[Index]  = (UINT8) rand ();
  }
  gMipiCamSetupData->MipiCam_Link3_DriverData_CrdVersion = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link3_DriverData_LinkUsed   = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link3_DriverData_LaneUsed   = (UINT8) rand ();
  if (gMipiCamSetupData->MipiCam_Link3_DriverData_EepromType == 0xFF) {
    gMipiCamSetupData->MipiCam_Link3_DriverData_EepromTypeCustom = (UINT8) rand ();
  } else {
    gMipiCamSetupData->MipiCam_Link3_DriverData_EepromType = (UINT8) rand ();
  }
  if (gMipiCamSetupData->MipiCam_Link3_DriverData_VcmType == 0xFF) {
    gMipiCamSetupData->MipiCam_Link3_DriverData_VcmTypeCustom = (UINT8) rand ();
  } else {
    gMipiCamSetupData->MipiCam_Link3_DriverData_VcmType = (UINT8) rand ();
  }
  gMipiCamSetupData->MipiCam_Link3_DriverData_Mclk              = ((UINT32) rand () << 16) + (UINT32) rand ();
  gMipiCamSetupData->MipiCam_Link3_DriverData_ControlLogic      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link3_DriverData_FlashSupport      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link3_DriverData_PrivacyLed        = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link3_DriverData_Degree            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link3_DriverData_PmicPosition      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link3_DriverData_VoltageRail       = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link3_DriverData_PhyConfiguration  = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link3_DriverData_LaneConfiguration = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link3_FlashDriverSelection         = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link3_DriverData_FlashID           = (UINT8) rand ();

  //
  // MipiCam Link4 specific
  //
  gMipiCamSetupData->MipiCam_Link4_SensorModel            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link4_CameraPhysicalLocation = (UINT8) rand ();
  //
  // ToDo: Replace the CopyMem with StrCpyS, after the alignment issue fix.
  //
  CopyMem (gMipiCamSetupData->MipiCam_Link4_ModuleName, L"Link4ModuleName", sizeof (L"Link4ModuleName"));
  CopyMem (gMipiCamSetupData->MipiCam_Link4_UserHid, L"Link4Hid", sizeof (L"Link4Hid"));
  gMipiCamSetupData->MipiCam_Link4_I2cDevicesEnabled = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link4_I2cChannel        = (UINT8) rand ();

  //
  // MipiCam Link4 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    gMipiCamSetupData->MipiCam_Link4_I2cAddress[Index]    = (UINT16) rand ();
    gMipiCamSetupData->MipiCam_Link4_I2cDeviceType[Index] = (UINT8) rand ();
  }
  for (Index = 0; Index < MIPICAM_CUSTOMIZE_DEVICE_COUNT; Index++) {
    gMipiCamSetupData->MipiCam_Link4_Customize_Id[Index]  = (UINT8) rand ();
  }
  gMipiCamSetupData->MipiCam_Link4_DriverData_CrdVersion = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link4_DriverData_LinkUsed   = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link4_DriverData_LaneUsed   = (UINT8) rand ();
  if (gMipiCamSetupData->MipiCam_Link1_DriverData_EepromType == 0xFF) {
    gMipiCamSetupData->MipiCam_Link4_DriverData_EepromTypeCustom = (UINT8) rand ();
  } else {
    gMipiCamSetupData->MipiCam_Link4_DriverData_EepromType = (UINT8) rand ();
  }
  if (gMipiCamSetupData->MipiCam_Link1_DriverData_VcmType == 0xFF) {
    gMipiCamSetupData->MipiCam_Link4_DriverData_VcmTypeCustom = (UINT8) rand ();
  } else {
    gMipiCamSetupData->MipiCam_Link4_DriverData_VcmType = (UINT8) rand ();
  }
  gMipiCamSetupData->MipiCam_Link4_DriverData_Mclk              = ((UINT32) rand () << 16) + (UINT32) rand ();
  gMipiCamSetupData->MipiCam_Link4_DriverData_ControlLogic      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link4_DriverData_FlashSupport      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link4_DriverData_PrivacyLed        = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link4_DriverData_Degree            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link4_DriverData_PmicPosition      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link4_DriverData_VoltageRail       = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link4_DriverData_PhyConfiguration  = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link4_DriverData_LaneConfiguration = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link4_FlashDriverSelection         = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link4_DriverData_FlashID           = (UINT8) rand ();

  //
  // MipiCam Link5 specific
  //
  gMipiCamSetupData->MipiCam_Link5_SensorModel            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link5_CameraPhysicalLocation = (UINT8) rand ();
  //
  // ToDo: Replace the CopyMem with StrCpyS, after the alignment issue fix.
  //
  CopyMem (gMipiCamSetupData->MipiCam_Link5_ModuleName, L"Link5ModuleName", sizeof (L"Link5ModuleName"));
  CopyMem (gMipiCamSetupData->MipiCam_Link5_UserHid, L"Link5Hid", sizeof (L"Link5Hid"));
  gMipiCamSetupData->MipiCam_Link5_I2cDevicesEnabled = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link5_I2cChannel        = (UINT8) rand ();

  //
  // MipiCam Link5 I2C Devices
  //
  for (Index = 0; Index < MIPICAM_I2C_DEVICES_COUNT; Index++) {
    gMipiCamSetupData->MipiCam_Link5_I2cAddress[Index]    = (UINT16) rand ();
    gMipiCamSetupData->MipiCam_Link5_I2cDeviceType[Index] = (UINT8) rand ();
  }
  for (Index = 0; Index < MIPICAM_CUSTOMIZE_DEVICE_COUNT; Index++) {
    gMipiCamSetupData->MipiCam_Link5_Customize_Id[Index]  = (UINT8) rand ();
  }
  gMipiCamSetupData->MipiCam_Link5_DriverData_CrdVersion = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link5_DriverData_LinkUsed   = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link5_DriverData_LaneUsed   = (UINT8) rand ();
  if (gMipiCamSetupData->MipiCam_Link5_DriverData_EepromType == 0xFF) {
    gMipiCamSetupData->MipiCam_Link5_DriverData_EepromTypeCustom = (UINT8) rand ();
  } else {
    gMipiCamSetupData->MipiCam_Link5_DriverData_EepromType = (UINT8) rand ();
  }
  if (gMipiCamSetupData->MipiCam_Link5_DriverData_VcmType == 0xFF) {
    gMipiCamSetupData->MipiCam_Link5_DriverData_VcmTypeCustom = (UINT8) rand ();
  } else {
    gMipiCamSetupData->MipiCam_Link5_DriverData_VcmType = (UINT8) rand ();
  }
  gMipiCamSetupData->MipiCam_Link5_DriverData_Mclk              = ((UINT32) rand () << 16) + (UINT32) rand ();
  gMipiCamSetupData->MipiCam_Link5_DriverData_ControlLogic      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link5_DriverData_FlashSupport      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link5_DriverData_PrivacyLed        = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link5_DriverData_Degree            = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link5_DriverData_PmicPosition      = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link5_DriverData_VoltageRail       = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link5_DriverData_PhyConfiguration  = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link5_DriverData_LaneConfiguration = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link5_FlashDriverSelection         = (UINT8) rand ();
  gMipiCamSetupData->MipiCam_Link5_DriverData_FlashID           = (UINT8) rand ();

  //
  // MipiCam Flash0 specific
  //
  gMipiCamSetupData->MipiCam_Flash0_Model = (UINT8) rand ();
  //
  // ToDo: Replace the CopyMem with StrCpyS, after the alignment issue fix.
  //
  CopyMem (gMipiCamSetupData->MipiCam_Flash0_UserHid, L"Fs0_Hid", sizeof (L"Fs0_Hid"));
  gMipiCamSetupData->MipiCam_Flash0_Mode = (UINT8) rand ();
  //
  // ToDo: Replace the CopyMem with StrCpyS, after the alignment issue fix.
  //
  CopyMem (gMipiCamSetupData->MipiCam_Flash0_ModuleName, L"Fs00_ModuleName", sizeof (L"Fs00_ModuleName"));
  gMipiCamSetupData->MipiCam_Flash0_I2cChannel         = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash0_I2cAddress         = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_Flash0_GpioSupport        = (UINT8)  TRUE;
  gMipiCamSetupData->MipiCam_Flash0_GpioGroupPadNumber = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash0_GpioComNumber      = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_Flash0_GpioGroupNumber    = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_Flash0_GpioActiveValue    = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash0_GpioInitialValue   = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash0_OperatingMode      = (UINT8)  rand ();

  //
  // MipiCam Flash1 specific
  //
  gMipiCamSetupData->MipiCam_Flash1_Model = (UINT8) rand ();
  //
  // ToDo: Replace the CopyMem with StrCpyS, after the alignment issue fix.
  //
  CopyMem (gMipiCamSetupData->MipiCam_Flash1_UserHid, L"Fs1_Hid", sizeof (L"Fs1_Hid"));
  gMipiCamSetupData->MipiCam_Flash1_Mode = (UINT8) rand ();
  //
  // ToDo: Replace the CopyMem with StrCpyS, after the alignment issue fix.
  //
  CopyMem (gMipiCamSetupData->MipiCam_Flash1_ModuleName, L"Fs01_ModuleName", sizeof (L"Fs01_ModuleName"));
  gMipiCamSetupData->MipiCam_Flash1_I2cChannel         = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash1_I2cAddress         = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_Flash1_GpioSupport        = (UINT8)  TRUE;
  gMipiCamSetupData->MipiCam_Flash1_GpioGroupPadNumber = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash1_GpioComNumber      = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_Flash1_GpioGroupNumber    = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_Flash1_GpioActiveValue    = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash1_GpioInitialValue   = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash1_OperatingMode      = (UINT8)  rand ();

  //
  // MipiCam Flash2 specific
  //
  gMipiCamSetupData->MipiCam_Flash2_Model = (UINT8) rand ();
  //
  // ToDo: Replace the CopyMem with StrCpyS, after the alignment issue fix.
  //
  CopyMem (gMipiCamSetupData->MipiCam_Flash2_UserHid, L"Fs2_Hid", sizeof (L"Fs2_Hid"));
  gMipiCamSetupData->MipiCam_Flash2_Mode = (UINT8) rand ();
  //
  // ToDo: Replace the CopyMem with StrCpyS, after the alignment issue fix.
  //
  CopyMem (gMipiCamSetupData->MipiCam_Flash2_ModuleName, L"Fs02_ModuleName", sizeof (L"Fs02_ModuleName"));
  gMipiCamSetupData->MipiCam_Flash2_I2cChannel         = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash2_I2cAddress         = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_Flash2_GpioSupport        = (UINT8)  TRUE;
  gMipiCamSetupData->MipiCam_Flash2_GpioGroupPadNumber = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash2_GpioComNumber      = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_Flash2_GpioGroupNumber    = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_Flash2_GpioActiveValue    = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash2_GpioInitialValue   = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash2_OperatingMode      = (UINT8)  rand ();

  //
  // MipiCam Flash3 specific
  //
  gMipiCamSetupData->MipiCam_Flash3_Model = (UINT8) rand ();
  //
  // ToDo: Replace the CopyMem with StrCpyS, after the alignment issue fix.
  //
  CopyMem (gMipiCamSetupData->MipiCam_Flash3_UserHid, L"Fs3_Hid", sizeof (L"Fs3_Hid"));
  gMipiCamSetupData->MipiCam_Flash3_Mode = (UINT8) rand ();
  //
  // ToDo: Replace the CopyMem with StrCpyS, after the alignment issue fix.
  //
  CopyMem (gMipiCamSetupData->MipiCam_Flash3_ModuleName, L"Fs03_ModuleName", sizeof (L"Fs03_ModuleName"));
  gMipiCamSetupData->MipiCam_Flash3_I2cChannel         = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash3_I2cAddress         = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_Flash3_GpioSupport        = (UINT8)  TRUE;
  gMipiCamSetupData->MipiCam_Flash3_GpioGroupPadNumber = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash3_GpioComNumber      = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_Flash3_GpioGroupNumber    = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_Flash3_GpioActiveValue    = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash3_GpioInitialValue   = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash3_OperatingMode      = (UINT8)  rand ();

  //
  // MipiCam Flash4 specific
  //
  gMipiCamSetupData->MipiCam_Flash4_Model = (UINT8) rand ();
  //
  // ToDo: Replace the CopyMem with StrCpyS, after the alignment issue fix.
  //
  CopyMem (gMipiCamSetupData->MipiCam_Flash4_UserHid, L"Fs4_Hid", sizeof (L"Fs4_Hid"));
  gMipiCamSetupData->MipiCam_Flash4_Mode = (UINT8) rand ();
  //
  // ToDo: Replace the CopyMem with StrCpyS, after the alignment issue fix.
  //
  CopyMem (gMipiCamSetupData->MipiCam_Flash4_ModuleName, L"Fs04_ModuleName", sizeof (L"Fs04_ModuleName"));
  gMipiCamSetupData->MipiCam_Flash4_I2cChannel         = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash4_I2cAddress         = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_Flash4_GpioSupport        = (UINT8)  TRUE;
  gMipiCamSetupData->MipiCam_Flash4_GpioGroupPadNumber = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash4_GpioComNumber      = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_Flash4_GpioGroupNumber    = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_Flash4_GpioActiveValue    = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash4_GpioInitialValue   = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash4_OperatingMode      = (UINT8)  rand ();

  //
  // MipiCam Flash5 specific
  //
  gMipiCamSetupData->MipiCam_Flash5_Model = (UINT8) rand ();
  //
  // ToDo: Replace the CopyMem with StrCpyS, after the alignment issue fix.
  //
  CopyMem (gMipiCamSetupData->MipiCam_Flash5_UserHid, L"Fs5_Hid", sizeof (L"Fs5_Hid"));
  gMipiCamSetupData->MipiCam_Flash5_Mode = (UINT8) rand ();
  //
  // ToDo: Replace the CopyMem with StrCpyS, after the alignment issue fix.
  //
  CopyMem (gMipiCamSetupData->MipiCam_Flash5_ModuleName, L"Fs05_ModuleName", sizeof (L"Fs05_ModuleName"));
  gMipiCamSetupData->MipiCam_Flash5_I2cChannel         = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash5_I2cAddress         = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_Flash5_GpioSupport        = (UINT8)  TRUE;
  gMipiCamSetupData->MipiCam_Flash5_GpioGroupPadNumber = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash5_GpioGroupNumber    = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_Flash5_GpioComNumber      = (UINT16) rand ();
  gMipiCamSetupData->MipiCam_Flash5_GpioActiveValue    = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash5_GpioInitialValue   = (UINT8)  rand ();
  gMipiCamSetupData->MipiCam_Flash5_OperatingMode      = (UINT8)  rand ();
}

/**
  Set up services for InitializeMipiCam function.

  @retval EFI_SUCCESS  Set up success.

**/
EFI_STATUS
EFIAPI
SetupInitializeMipiCamEnvironment (
  VOID
  )
{
  EFI_STATUS                     Status;
  UINTN                          Pages;
  EFI_PHYSICAL_ADDRESS           Address;
  UINTN                          MipiCamConfigSize;
  EFI_HANDLE                     *HandleBuffer;
  EFI_FIRMWARE_VOLUME2_PROTOCOL  *FwVol2Protocol;
  EFI_ACPI_TABLE_PROTOCOL        *AcpiTableProtocol;
  EFI_ACPI_COMMON_HEADER         *SsdtTable;
  UINTN                          SsdtBufferSize;
  EFI_ACPI_COMMON_HEADER         *SsdtTargetTable;
  UINTN                          SsdtTargetBufferSize;

  //
  // Mock run time service GetVariable
  //
  MipiCamConfigSize = sizeof (MIPICAM_CONFIG);
  gMipiCamSetupData = AllocateZeroPool (sizeof (MIPICAM_CONFIG));
  if (gMipiCamSetupData == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  InitializeMipiCamSetupData ();

  gRT = AllocateZeroPool (sizeof (EFI_RUNTIME_SERVICES));
  if (gRT == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  gRT->GetVariable = MockRuntimeGetVariable;
  expect_value (MockRuntimeGetVariable, VariableName, MIPICAM_CONFIG_VARIABLE_NAME);
  expect_value (MockRuntimeGetVariable, VendorGuid, &gMipiCamConfigVariableGuid);
  will_return  (MockRuntimeGetVariable, MipiCamConfigSize);
  will_return  (MockRuntimeGetVariable, gMipiCamSetupData);
  will_return  (MockRuntimeGetVariable, EFI_SUCCESS);

  //
  // Mock boot service AllocatePages
  //
  Pages = EFI_SIZE_TO_PAGES (sizeof (MIPICAM_NVS_AREA));
  Address = (UINTN) AllocatePages (Pages);
  if (Address == 0) {
    return EFI_OUT_OF_RESOURCES;
  }

  SetMem ((VOID*) Address, sizeof (MIPICAM_NVS_AREA), 0xFF);

  mMipiCamNvsAreaProtocol.Area = (MIPICAM_NVS_AREA *)(UINTN) Address;

  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  if (gBS == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  gBS->AllocatePages = MockBootServiceAllocatePages;
  will_return (MockBootServiceAllocatePages, Address);
  will_return (MockBootServiceAllocatePages, Pages);
  will_return (MockBootServiceAllocatePages, EFI_SUCCESS);

  //
  // Mock GuidHob Data
  //
  MockMipiCamGuidHob ();

  expect_value (GetFirstGuidHob, Guid, (UINTN)&gMipiCamConfigHobGuid);
  will_return  (GetFirstGuidHob, gHobList);

  MockMipiCamGuidHobExt ();

  expect_value (GetFirstGuidHob, Guid, (UINTN)&gMipiCamConfigHobExtGuid);
  will_return  (GetFirstGuidHob, gHobExtList);

  //
  // LoadAcpiTables ()
  //
  will_return (MockMipiCamAcpiTableFile, "Build/Camera/MipiCamFeaturePkg/HostTest/NOOPT_VS2019/X64/MipiCamFeaturePkg/AcpiTables/MipiCamAcpiTables/OUTPUT/MipiCamSensors.aml");
  Status = MockMipiCamAcpiTableFile ((UINT8**) &SsdtTable, &SsdtBufferSize);
  if (EFI_ERROR (Status)) {
    return UNIT_TEST_ERROR_TEST_FAILED;
  }

  //
  // Mock boot service LocateHandleBuffer
  //
  HandleBuffer = AllocateZeroPool (3 * sizeof (EFI_HANDLE));
  if (HandleBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  gBS->LocateHandleBuffer = MockBootServiceLocateHandleBuffer;
  expect_value (MockBootServiceLocateHandleBuffer, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  will_return (MockBootServiceLocateHandleBuffer, 3);
  will_return (MockBootServiceLocateHandleBuffer, HandleBuffer);
  will_return (MockBootServiceLocateHandleBuffer, EFI_SUCCESS);

  //
  // Mock gEfiFirmwareVolume2ProtocolGuid
  //
  FwVol2Protocol = AllocateZeroPool (sizeof (EFI_FIRMWARE_VOLUME2_PROTOCOL));
  if (FwVol2Protocol == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Mock boot service HandleProtocol
  //
  gBS->HandleProtocol = MockBootServiceHandleProtocol;
  expect_value (MockBootServiceHandleProtocol, Protocol, &gEfiFirmwareVolume2ProtocolGuid);
  will_return (MockBootServiceHandleProtocol, FwVol2Protocol);
  will_return (MockBootServiceHandleProtocol, EFI_SUCCESS);

  //
  // Mock the FwVol->ReadFile
  //
  FwVol2Protocol->ReadFile = MockFwVol2ReadFile;
  will_return (MockFwVol2ReadFile, SsdtBufferSize);
  will_return (MockFwVol2ReadFile, EFI_SUCCESS);

  //
  // Mock gEfiAcpiTableProtocolGuid
  //
  AcpiTableProtocol = AllocateZeroPool (sizeof (EFI_ACPI_TABLE_PROTOCOL));
  if (AcpiTableProtocol == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Mock boot service LocateProtocol
  //
  gBS->LocateProtocol = MockBootServiceLocateProtocol;

  //
  // Mock for Mipi Camera SSDT needed protocol
  //
  expect_value (MockBootServiceLocateProtocol, Protocol, &gEfiAcpiTableProtocolGuid);
  will_return (MockBootServiceLocateProtocol, AcpiTableProtocol);
  will_return (MockBootServiceLocateProtocol, EFI_SUCCESS);

  //
  // Mock the FwVol->ReadFile
  //
  FwVol2Protocol->ReadSection = MockFwVol2ReadSection;
  //
  // First time return MipiCam SSDT table
  //
  expect_value (MockFwVol2ReadSection, SectionType, EFI_SECTION_RAW);
  will_return (MockFwVol2ReadSection, SsdtTable);
  will_return (MockFwVol2ReadSection, SsdtBufferSize);
  will_return (MockFwVol2ReadSection, EFI_SUCCESS);
  //
  // Second time return ACPI table not found
  //
  expect_value (MockFwVol2ReadSection, SectionType, EFI_SECTION_RAW);
  will_return (MockFwVol2ReadSection, SsdtTable);
  will_return (MockFwVol2ReadSection, SsdtBufferSize);
  will_return (MockFwVol2ReadSection, EFI_NOT_FOUND);

  //
  // Mock gEfiAcpiTableProtocolGuid
  //
  AcpiTableProtocol->InstallAcpiTable = MockInstallAcpiTable;
  will_return (MockInstallAcpiTable, &SsdtTargetTable);
  will_return (MockInstallAcpiTable, &SsdtTargetBufferSize);
  will_return (MockInstallAcpiTable, EFI_SUCCESS);

  return EFI_SUCCESS;
}

/**
  Set up the environment and execute InitializeMipiCam function.

  @param[in] Context                            The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED                      Set up success.
  @retval UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Fail to set up environment.

**/
UNIT_TEST_STATUS
EFIAPI
PreInitializeMipiCam (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "[PreRequisite] %a v%a\n", PRE_FUNCTION_NAME, PRE_FUNCTION_VERSION));

  Status = SetupInitializeMipiCamEnvironment ();
  if (EFI_ERROR (Status)) {
    return UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
  }
  //
  // Execute test function with above mock environment
  //
  InitializeMipiCam (NULL, NULL);

  DumpMipiCamNvsAreaProtocol ();

  DumpMipiCamSetupData ();

  return UNIT_TEST_PASSED;
}
