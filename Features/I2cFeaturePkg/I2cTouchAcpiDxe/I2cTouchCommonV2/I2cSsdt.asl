/** @file
  I2C Rtd3 Ssdt table
  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.
**/

#include <I2cDevices.h>

DefinitionBlock (
    "Rtd3.aml",
    "SSDT",
    2,
    "Rtd3",
    "I2C_DEVT",
    0x1000
    )
{
  External (\ADBG,MethodObj)

  #define GPIO_PPI_PULL_DEFAULT 0x0
  #define GPIO_INT_MOD_LEVEL 0x0 // Interrupt is triggered in response to the signal being in either a high or low state.
  #define GPIO_INT_POL_ACTIVE_LOW      0x1 // This interrupt is sampled when the signal is low, or false.
  #define GPIO_INT_EXCLUSIVE           0x0 // This IO connection is used exclusively by one device, This interrupt is not capable of waking the system.
  #define GPIO_INT_EXCLUSIVE_AND_WAKE  0x2
  
  // Macros used for TLDT, TDDT
  #define TPLD_TYPE_INDEX (0)
  #define TPLD_BRIDGE_INDEX (1)
  #define TPLD_CONTROLLER_INDEX (2)
  #define TPLD_IRQ_MODE_INDEX (3)
  #define TPLD_BUS_ADR_INDEX (4)
  #define TPLD_BUS_SPEED_INDEX (5)
  #define TPLD_HID_ADR_INDEX (6)
  #define TPLD_IRQ_GPIO_INDEX (7)
  #define TPLD_IRQ_POLARITY_INDEX (8)
  #define TPLD_D3_SUPPORT_ENABLE_INDEX (9)
  #define TPLD_D3_DELAY_INDEX (10)

  // Macros used for TLD3 buffer
  #define TPL_POWER_GPIO_INDEX (0)
  #define TPL_POWER_POLARITY_INDEX (1)
  #define TPL_RESET_GPIO_INDEX (2)
  #define TPL_RESET_POLARITY_INDEX (3)

  #define NO_OF_ELT (10) //(No. Of elements - 1) in 1 Packet of Touch panel(TLDT)/pad(TDDT)

  Include("I2cDevices.asl")

  Scope(\_SB){

    //
    // Two instances of Touch paneL DaTa in TLDT pkg in below format.
    // Data in One pkg as below, in sequence.
    //
    // TouchPanelType (UINT8)
    // TouchPanelBridge (UINT8)
    // TouchPanelController (UINT8)
    // TouchPanelIrqMode (UINT8)
    // TouchPanelBusAddress (UINT8)
    // TouchPanelBusSpeed (UINT8)
    // TouchPanelHidAddress (UINT16)
    // TouchpanelIrqGpio (UINT32)
    // TouchpanelIrqGpioPolarity (UINT8)
    // TouchPanelD3ColdSupport (UINT8)
    // TouchPanelD3Delay (UINT16)
    //
    Name(TLDT,Package () {0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF, 0xFFFF, 0xFFFFFFFF, 0xFF, 0xFF, 0xFFFF,
                          0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF, 0xFFFF, 0xFFFFFFFF, 0xFF, 0xFF, 0xFFFF })

    //
    // Two instances of Touch PaneL D3 support Pins
    //
    // TPLPowerEnableGPIO UINT32
    // TPLPowerEnablePolarity UINT8
    // TPLResetGPIO   UINT32
    // TPLResetPolarity UINT8
    //
    Name(TLD3,Package () {0xFFFFFFFF, 0xFF, 0xFFFFFFFF, 0xFF,
                          0xFFFFFFFF, 0xFF, 0xFFFFFFFF, 0xFF})

    //
    // Three instances of Touch PaD DaTa in TDDT pkg.
    //
    // TouchPadType (UINT8)
    // TouchPadBridge (UINT8)
    // TouchPadController (UINT8)
    // TouchPadIrqMode (UINT8)
    // TouchPadBusAddress (UINT8)
    // TouchPadSpeed  (UINT8))
    // TouchPadHidAddress (UINT16)
    // TouchpadIrqGpio (UINT32)
    // TouchpanelIrqGpioPolarity (UINT8)
    // TouchPadD3ColdSupport (UINT8)
    // TouchPadD3Delay (UINT16)
    //
    Name(TDDT,Package () { 0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF, 0xFFFF, 0xFFFFFFFF, 0xFF, 0xFF, 0xFFFF,
                           0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF, 0xFFFF, 0xFFFFFFFF, 0xFF, 0xFF, 0xFFFF,
                           0xFF,0xFF,0xFF,0xFF, 0xFF, 0xFF, 0xFFFF, 0xFFFFFFFF, 0xFF, 0xFF, 0xFFFF })


    Include ("I2cTouchCommonMethods.asl")

    //
    // I2C Touch Panel Devices
    //
#define TPLD_INDEX (0)
      Device (TPL0) {
        Include("I2cTouchPanel.asl")
        Include ("I2cTouchPanelGenericRtd3.asl")
      }
#undef TPLD_INDEX

#define TPLD_INDEX (1)
      Device (TPL1) {
        Include("I2cTouchPanel.asl")
        Include ("I2cTouchPanelGenericRtd3.asl")
      }
#undef TPLD_INDEX

    //
    // I2C Touch Pad Devices
    //
#define TPLD_INDEX (0)
      Device (TPD0) {
        Include("I2cTouchPad.asl")
        Include ("I2cTouchPadGenericRtd3.asl")
      }
#undef TPLD_INDEX

#define TPLD_INDEX (1)
      Device (TPD1) {
        Include("I2cTouchPad.asl")
        Include ("I2cTouchPadGenericRtd3.asl")
      }
#undef TPLD_INDEX

#define TPLD_INDEX (2)
      Device (TPD2) {
        Include("I2cTouchPad.asl")
        Include ("I2cTouchPadGenericRtd3.asl")
      }
#undef TPLD_INDEX

  } // End of \_SB scope
}
