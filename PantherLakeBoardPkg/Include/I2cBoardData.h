/**@file
  I2C Board Driver
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
#define MAX_I2C_ROOTPORT_ENTRY_SUPPORT                       8
#define TOUCH_PAD_TYPE                                       0x1
#define TOUCH_PANEL_TYPE                                     0x2
#define TOUCH_PAD_AND_PANEL_SUPPORT                          0x3

typedef struct {
  //
  // Touch Panel Power Gpio Pad
  //
  UINT32   TouchPanelPowerGpioPad;
  //
  // Touch Panel Power Pin Gpio Polarity
  // True :Active High
  // False:Active Low
  //
  BOOLEAN  TouchPanelPowerGpioPolarity;
  //
  // Touch Panel Reset Gpio Pad Polarity
  //
  UINT32   TouchPanelResetGpioPad;
  //
  // Touch Panel Reset Gpio Polarity
  // True : Active High
  // False: Active LOw
  BOOLEAN  TouchPanelResetGpioPolarity;
  //
  // Touch Panel Reset Gpio Pad Polarity
  //
  UINT32   TouchPanelInterruptGpioPad;
  //
  // Touch Panel Interrupt Gpio Polarity
  // True : Active High
  // False: Active LOw
  BOOLEAN  TouchPanelInterruptGpioPolarity;

  UINT8  Reserved;
} VPD_TOUCH_PANEL_TABLE;

typedef struct {
  //
  // Touch Pad Power Gpio Pad
  //
  UINT32   TouchPadInterruptGpioPad;
  //
  // Touch Pad Power Pin Gpio Polarity
  // True :Active High
  // False:Active Low
  //
  BOOLEAN  TouchPadInterruptGpioPolarity;
  //
  // Reserved
  //
  UINT8  Reserved[3];
} VPD_TOUCH_PAD_TABLE;

typedef struct {
  //
  // I2C Rp Index
  //
  UINT8    I2CRpIndex;
  //
  // Touch panel Gpio Pin
  //
  VPD_TOUCH_PANEL_TABLE  TouchPanelGpioPin;
  //
  // Touch Pad Gpio Pin
  //
  VPD_TOUCH_PAD_TABLE    TouchPadGpioPin;
  //
  // Reserved
  //
  UINT8  Reserved[3];
} VPD_I2C_RP_TABLE;


typedef struct {
  UINT8                        Count;
  VPD_I2C_RP_TABLE             I2CRpInfoTable [MAX_I2C_ROOTPORT_ENTRY_SUPPORT];
  UINT8                        Reserved[3];
} VPD_I2C_RP_TABLE_PACKAGE;
