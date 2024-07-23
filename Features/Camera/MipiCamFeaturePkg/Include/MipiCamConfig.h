/** @file
  Header file for Mipi Camera Config Variable

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

#ifndef _MIPICAM_CONFIG_H_
#define _MIPICAM_CONFIG_H_

#define MIPICAM_CONFIG_REVISION       1
#define MIPICAM_CONFIG_VARIABLE_NAME  L"MipiCamConfig"

#define MIPICAM_I2C_DEVICES_COUNT         12
#define MIPICAM_GPIO_COUNT                6
#define MIPICAM_MODULE_NAME_LENGTH        16
#define MIPICAM_HID_MIN_LENGTH            6
#define MIPICAM_HID_MAX_LENGTH            8
#define MIPICAM_HID_LENGTH                9
#define MIPICAM_CUSTOMIZE_DEVICE_COUNT    3
#define MIPICAM_CUSTOMIZE_DEVICE1         0x0D
#define MIPICAM_CUSTOMIZE_DEVICE2         0x0E
#define MIPICAM_CUSTOMIZE_DEVICE3         0x0F

#pragma pack (1)
typedef struct {
  //
  // MipiCam sensor configuration menu
  //
  UINT8   MipiCam_ControlLogic0;             // None/discrete/PMIC
  UINT8   MipiCam_ControlLogic1;             // None/discrete/PMIC
  UINT8   MipiCam_ControlLogic2;             // None/discrete/PMIC
  UINT8   MipiCam_ControlLogic3;             // None/discrete/PMIC
  UINT8   MipiCam_ControlLogic4;             // None/discrete/PMIC
  UINT8   MipiCam_ControlLogic5;             // None/discrete/PMIC

  UINT8   MipiCam_Link0;                     // Enabled / Disabled
  UINT8   MipiCam_Link1;                     // Enabled / Disabled
  UINT8   MipiCam_Link2;                     // Enabled / Disabled
  UINT8   MipiCam_Link3;                     // Enabled / Disabled
  UINT8   MipiCam_Link4;                     // Enabled / Disabled
  UINT8   MipiCam_Link5;                     // Enabled / Disabled

  UINT8   MipiCam4Enable;                    // Enable 5th camera
  UINT8   MipiCam5Enable;                    // Enable 6th camera

  UINT8   MipiCam_LanesClkDiv;

  UINT8   MipiCam_ControlLogic0_Type;
  UINT8   MipiCam_ControlLogic0_CrdVersion;
  UINT32  MipiCam_ControlLogic0_InputClock;
  UINT8   MipiCam_ControlLogic0_PchClockSource;
  UINT8   MipiCam_ControlLogic0_GpioPinsEnabled;
  UINT8   MipiCam_ControlLogic0_I2cChannel;
  UINT16  MipiCam_ControlLogic0_I2cAddress;
  UINT8   MipiCam_ControlLogic0_Pld;
  UINT8   MipiCam_ControlLogic0_Wled1FlashMaxCurrent;
  UINT8   MipiCam_ControlLogic0_Wled1TorchMaxCurrent;
  UINT8   MipiCam_ControlLogic0_Wled2FlashMaxCurrent;
  UINT8   MipiCam_ControlLogic0_Wled2TorchMaxCurrent;
  UINT8   MipiCam_ControlLogic0_Wled1Type;
  UINT8   MipiCam_ControlLogic0_Wled2Type;
  UINT8   MipiCam_ControlLogic0_GpioGroupPadNumber[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic0_GpioGroupNumber[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic0_GpioComNumber[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic0_GpioFunction[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic0_GpioFunction_User[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic0_GpioActiveValue[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic0_GpioInitialValue[MIPICAM_GPIO_COUNT];

  UINT8   MipiCam_ControlLogic1_Type;
  UINT8   MipiCam_ControlLogic1_CrdVersion;
  UINT32  MipiCam_ControlLogic1_InputClock;
  UINT8   MipiCam_ControlLogic1_PchClockSource;
  UINT8   MipiCam_ControlLogic1_GpioPinsEnabled;
  UINT8   MipiCam_ControlLogic1_I2cChannel;
  UINT16  MipiCam_ControlLogic1_I2cAddress;
  UINT8   MipiCam_ControlLogic1_Pld;
  UINT8   MipiCam_ControlLogic1_Wled1FlashMaxCurrent;
  UINT8   MipiCam_ControlLogic1_Wled1TorchMaxCurrent;
  UINT8   MipiCam_ControlLogic1_Wled2FlashMaxCurrent;
  UINT8   MipiCam_ControlLogic1_Wled2TorchMaxCurrent;
  UINT8   MipiCam_ControlLogic1_Wled1Type;
  UINT8   MipiCam_ControlLogic1_Wled2Type;
  UINT8   MipiCam_ControlLogic1_GpioGroupPadNumber[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic1_GpioGroupNumber[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic1_GpioComNumber[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic1_GpioFunction[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic1_GpioFunction_User[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic1_GpioActiveValue[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic1_GpioInitialValue[MIPICAM_GPIO_COUNT];

  UINT8   MipiCam_ControlLogic2_Type;
  UINT8   MipiCam_ControlLogic2_CrdVersion;
  UINT32  MipiCam_ControlLogic2_InputClock;
  UINT8   MipiCam_ControlLogic2_PchClockSource;
  UINT8   MipiCam_ControlLogic2_GpioPinsEnabled;
  UINT8   MipiCam_ControlLogic2_I2cChannel;
  UINT16  MipiCam_ControlLogic2_I2cAddress;
  UINT8   MipiCam_ControlLogic2_Pld;
  UINT8   MipiCam_ControlLogic2_Wled1FlashMaxCurrent;
  UINT8   MipiCam_ControlLogic2_Wled1TorchMaxCurrent;
  UINT8   MipiCam_ControlLogic2_Wled2FlashMaxCurrent;
  UINT8   MipiCam_ControlLogic2_Wled2TorchMaxCurrent;
  UINT8   MipiCam_ControlLogic2_Wled1Type;
  UINT8   MipiCam_ControlLogic2_Wled2Type;
  UINT8   MipiCam_ControlLogic2_GpioGroupPadNumber[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic2_GpioGroupNumber[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic2_GpioComNumber[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic2_GpioFunction[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic2_GpioFunction_User[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic2_GpioActiveValue[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic2_GpioInitialValue[MIPICAM_GPIO_COUNT];

  UINT8   MipiCam_ControlLogic3_Type;
  UINT8   MipiCam_ControlLogic3_CrdVersion;
  UINT32  MipiCam_ControlLogic3_InputClock;
  UINT8   MipiCam_ControlLogic3_PchClockSource;
  UINT8   MipiCam_ControlLogic3_GpioPinsEnabled;
  UINT8   MipiCam_ControlLogic3_I2cChannel;
  UINT16  MipiCam_ControlLogic3_I2cAddress;
  UINT8   MipiCam_ControlLogic3_Pld;
  UINT8   MipiCam_ControlLogic3_Wled1FlashMaxCurrent;
  UINT8   MipiCam_ControlLogic3_Wled1TorchMaxCurrent;
  UINT8   MipiCam_ControlLogic3_Wled2FlashMaxCurrent;
  UINT8   MipiCam_ControlLogic3_Wled2TorchMaxCurrent;
  UINT8   MipiCam_ControlLogic3_Wled1Type;
  UINT8   MipiCam_ControlLogic3_Wled2Type;
  UINT8   MipiCam_ControlLogic3_GpioGroupPadNumber[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic3_GpioGroupNumber[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic3_GpioComNumber[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic3_GpioFunction[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic3_GpioFunction_User[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic3_GpioActiveValue[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic3_GpioInitialValue[MIPICAM_GPIO_COUNT];

  UINT8   MipiCam_ControlLogic4_Type;
  UINT8   MipiCam_ControlLogic4_CrdVersion;
  UINT32  MipiCam_ControlLogic4_InputClock;
  UINT8   MipiCam_ControlLogic4_PchClockSource;
  UINT8   MipiCam_ControlLogic4_GpioPinsEnabled;
  UINT8   MipiCam_ControlLogic4_I2cChannel;
  UINT16  MipiCam_ControlLogic4_I2cAddress;
  UINT8   MipiCam_ControlLogic4_Pld;
  UINT8   MipiCam_ControlLogic4_Wled1FlashMaxCurrent;
  UINT8   MipiCam_ControlLogic4_Wled1TorchMaxCurrent;
  UINT8   MipiCam_ControlLogic4_Wled2FlashMaxCurrent;
  UINT8   MipiCam_ControlLogic4_Wled2TorchMaxCurrent;
  UINT8   MipiCam_ControlLogic4_Wled1Type;
  UINT8   MipiCam_ControlLogic4_Wled2Type;
  UINT8   MipiCam_ControlLogic4_GpioGroupPadNumber[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic4_GpioGroupNumber[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic4_GpioComNumber[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic4_GpioFunction[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic4_GpioFunction_User[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic4_GpioActiveValue[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic4_GpioInitialValue[MIPICAM_GPIO_COUNT];

  UINT8   MipiCam_ControlLogic5_Type;
  UINT8   MipiCam_ControlLogic5_CrdVersion;
  UINT32  MipiCam_ControlLogic5_InputClock;
  UINT8   MipiCam_ControlLogic5_PchClockSource;
  UINT8   MipiCam_ControlLogic5_GpioPinsEnabled;
  UINT8   MipiCam_ControlLogic5_I2cChannel;
  UINT16  MipiCam_ControlLogic5_I2cAddress;
  UINT8   MipiCam_ControlLogic5_Pld;
  UINT8   MipiCam_ControlLogic5_Wled1FlashMaxCurrent;
  UINT8   MipiCam_ControlLogic5_Wled1TorchMaxCurrent;
  UINT8   MipiCam_ControlLogic5_Wled2FlashMaxCurrent;
  UINT8   MipiCam_ControlLogic5_Wled2TorchMaxCurrent;
  UINT8   MipiCam_ControlLogic5_Wled1Type;
  UINT8   MipiCam_ControlLogic5_Wled2Type;
  UINT8   MipiCam_ControlLogic5_GpioGroupPadNumber[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic5_GpioGroupNumber[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic5_GpioComNumber[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic5_GpioFunction[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic5_GpioFunction_User[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic5_GpioActiveValue[MIPICAM_GPIO_COUNT];
  UINT8   MipiCam_ControlLogic5_GpioInitialValue[MIPICAM_GPIO_COUNT];

  UINT8   MipiCam_Link0_SensorModel;         // IMX135 / OV5693 / IMX179 / OV8858 / OV2740-IVCAM / IMX208 / OV9728 / IMX188 / User0
  UINT16  MipiCam_Link0_UserHid[MIPICAM_HID_LENGTH];
  UINT8   MipiCam_Link0_CameraPhysicalLocation;
  UINT16  MipiCam_Link0_ModuleName[MIPICAM_MODULE_NAME_LENGTH];
  UINT8   MipiCam_Link0_I2cDevicesEnabled;   // number of I2C devices defined for this link
  UINT8   MipiCam_Link0_I2cChannel;
  UINT16  MipiCam_Link0_I2cAddress[MIPICAM_I2C_DEVICES_COUNT];
  UINT8   MipiCam_Link0_I2cDeviceType[MIPICAM_I2C_DEVICES_COUNT];
  UINT8   MipiCam_Link0_DriverData_Version;
  UINT8   MipiCam_Link0_DriverData_CrdVersion;
  UINT8   MipiCam_Link0_DriverData_LinkUsed;
  UINT8   MipiCam_Link0_DriverData_LaneUsed;
  UINT8   MipiCam_Link0_DriverData_EepromType;
  UINT8   MipiCam_Link0_DriverData_EepromTypeCustom;
  UINT8   MipiCam_Link0_DriverData_VcmType;
  UINT8   MipiCam_Link0_DriverData_VcmTypeCustom;
  UINT8   MipiCam_Link0_DriverData_CustomData[4];
  UINT32  MipiCam_Link0_DriverData_Mclk;
  UINT8   MipiCam_Link0_DriverData_ControlLogic;
  UINT8   MipiCam_Link0_DriverData_FlashSupport;
  UINT8   MipiCam_Link0_DriverData_PrivacyLed;
  UINT8   MipiCam_Link0_DriverData_Degree;
  UINT8   MipiCam_Link0_DriverData_PmicPosition;
  UINT8   MipiCam_Link0_DriverData_VoltageRail;
  UINT8   MipiCam_Link0_DriverData_PhyConfiguration;
  UINT8   MipiCam_Link0_DriverData_LaneConfiguration;
  UINT8   MipiCam_Link0_FlashDriverSelection;
  UINT8   MipiCam_Link0_DriverData_FlashID;

  UINT8   MipiCam_Link1_SensorModel;         // IMX135 / OV5693 / IMX179 / OV8858 / OV2740-IVCAM / IMX208 / OV9728 / IMX188 / User1
  UINT16  MipiCam_Link1_UserHid[MIPICAM_HID_LENGTH];
  UINT8   MipiCam_Link1_CameraPhysicalLocation;
  UINT16  MipiCam_Link1_ModuleName[MIPICAM_MODULE_NAME_LENGTH];
  UINT8   MipiCam_Link1_I2cDevicesEnabled;   // number of I2C devices defined for this link
  UINT8   MipiCam_Link1_I2cChannel;
  UINT16  MipiCam_Link1_I2cAddress[MIPICAM_I2C_DEVICES_COUNT];
  UINT8   MipiCam_Link1_I2cDeviceType[MIPICAM_I2C_DEVICES_COUNT];
  UINT8   MipiCam_Link1_DriverData_Version;
  UINT8   MipiCam_Link1_DriverData_LinkUsed;
  UINT8   MipiCam_Link1_DriverData_CrdVersion;
  UINT8   MipiCam_Link1_DriverData_LaneUsed;
  UINT8   MipiCam_Link1_DriverData_EepromType;
  UINT8   MipiCam_Link1_DriverData_EepromTypeCustom;
  UINT8   MipiCam_Link1_DriverData_VcmType;
  UINT8   MipiCam_Link1_DriverData_VcmTypeCustom;
  UINT8   MipiCam_Link1_DriverData_CustomData[4];
  UINT32  MipiCam_Link1_DriverData_Mclk;
  UINT8   MipiCam_Link1_DriverData_ControlLogic;
  UINT8   MipiCam_Link1_DriverData_FlashSupport;
  UINT8   MipiCam_Link1_DriverData_PrivacyLed;
  UINT8   MipiCam_Link1_DriverData_Degree;
  UINT8   MipiCam_Link1_DriverData_PmicPosition;
  UINT8   MipiCam_Link1_DriverData_VoltageRail;
  UINT8   MipiCam_Link1_DriverData_PhyConfiguration;
  UINT8   MipiCam_Link1_DriverData_LaneConfiguration;
  UINT8   MipiCam_Link1_FlashDriverSelection;
  UINT8   MipiCam_Link1_DriverData_FlashID;

  UINT8   MipiCam_Link2_SensorModel;         // IMX135 / OV5693 / IMX179 / OV8858 / OV2740-IVCAM / IMX208 / OV9728 / IMX188 / User2
  UINT16  MipiCam_Link2_UserHid[MIPICAM_HID_LENGTH];
  UINT8   MipiCam_Link2_CameraPhysicalLocation;
  UINT16  MipiCam_Link2_ModuleName[MIPICAM_MODULE_NAME_LENGTH];
  UINT8   MipiCam_Link2_I2cDevicesEnabled;   // number of I2C devices defined for this link
  UINT8   MipiCam_Link2_I2cChannel;
  UINT16  MipiCam_Link2_I2cAddress[MIPICAM_I2C_DEVICES_COUNT];
  UINT8   MipiCam_Link2_I2cDeviceType[MIPICAM_I2C_DEVICES_COUNT];
  UINT8   MipiCam_Link2_DriverData_Version;
  UINT8   MipiCam_Link2_DriverData_CrdVersion;
  UINT8   MipiCam_Link2_DriverData_LinkUsed;
  UINT8   MipiCam_Link2_DriverData_LaneUsed;
  UINT8   MipiCam_Link2_DriverData_EepromType;
  UINT8   MipiCam_Link2_DriverData_EepromTypeCustom;
  UINT8   MipiCam_Link2_DriverData_VcmType;
  UINT8   MipiCam_Link2_DriverData_VcmTypeCustom;
  UINT8   MipiCam_Link2_DriverData_CustomData[4];
  UINT32  MipiCam_Link2_DriverData_Mclk;
  UINT8   MipiCam_Link2_DriverData_ControlLogic;
  UINT8   MipiCam_Link2_DriverData_FlashSupport;
  UINT8   MipiCam_Link2_DriverData_PrivacyLed;
  UINT8   MipiCam_Link2_DriverData_Degree;
  UINT8   MipiCam_Link2_DriverData_PmicPosition;
  UINT8   MipiCam_Link2_DriverData_VoltageRail;
  UINT8   MipiCam_Link2_DriverData_PhyConfiguration;
  UINT8   MipiCam_Link2_DriverData_LaneConfiguration;
  UINT8   MipiCam_Link2_FlashDriverSelection;
  UINT8   MipiCam_Link2_DriverData_FlashID;

  UINT8   MipiCam_Link3_SensorModel;         // IMX135 / OV5693 / IMX179 / OV8858 / OV2740-IVCAM / IMX208 / OV9728 / IMX188 / User3
  UINT16  MipiCam_Link3_UserHid[MIPICAM_HID_LENGTH];
  UINT8   MipiCam_Link3_CameraPhysicalLocation;
  UINT16  MipiCam_Link3_ModuleName[MIPICAM_MODULE_NAME_LENGTH];
  UINT8   MipiCam_Link3_I2cDevicesEnabled;   // number of I2C devices defined for this link
  UINT8   MipiCam_Link3_I2cChannel;
  UINT16  MipiCam_Link3_I2cAddress[MIPICAM_I2C_DEVICES_COUNT];
  UINT8   MipiCam_Link3_I2cDeviceType[MIPICAM_I2C_DEVICES_COUNT];
  UINT8   MipiCam_Link3_DriverData_Version;
  UINT8   MipiCam_Link3_DriverData_CrdVersion;
  UINT8   MipiCam_Link3_DriverData_LinkUsed;
  UINT8   MipiCam_Link3_DriverData_LaneUsed;
  UINT8   MipiCam_Link3_DriverData_EepromType;
  UINT8   MipiCam_Link3_DriverData_EepromTypeCustom;
  UINT8   MipiCam_Link3_DriverData_VcmType;
  UINT8   MipiCam_Link3_DriverData_VcmTypeCustom;
  UINT8   MipiCam_Link3_DriverData_CustomData[4];
  UINT32  MipiCam_Link3_DriverData_Mclk;
  UINT8   MipiCam_Link3_DriverData_ControlLogic;
  UINT8   MipiCam_Link3_DriverData_FlashSupport;
  UINT8   MipiCam_Link3_DriverData_PrivacyLed;
  UINT8   MipiCam_Link3_DriverData_Degree;
  UINT8   MipiCam_Link3_DriverData_PmicPosition;
  UINT8   MipiCam_Link3_DriverData_VoltageRail;
  UINT8   MipiCam_Link3_DriverData_PhyConfiguration;
  UINT8   MipiCam_Link3_DriverData_LaneConfiguration;
  UINT8   MipiCam_Link3_FlashDriverSelection;
  UINT8   MipiCam_Link3_DriverData_FlashID;

  UINT8   MipiCam_Link4_SensorModel;         // IMX135 / OV5693 / IMX179 / OV8858 / OV2740-IVCAM / IMX208 / OV9728 / IMX188 / User3
  UINT16  MipiCam_Link4_UserHid[MIPICAM_HID_LENGTH];
  UINT8   MipiCam_Link4_CameraPhysicalLocation;
  UINT16  MipiCam_Link4_ModuleName[MIPICAM_MODULE_NAME_LENGTH];
  UINT8   MipiCam_Link4_I2cDevicesEnabled;   // number of I2C devices defined for this link
  UINT8   MipiCam_Link4_I2cChannel;
  UINT16  MipiCam_Link4_I2cAddress[MIPICAM_I2C_DEVICES_COUNT];
  UINT8   MipiCam_Link4_I2cDeviceType[MIPICAM_I2C_DEVICES_COUNT];
  UINT8   MipiCam_Link4_DriverData_Version;
  UINT8   MipiCam_Link4_DriverData_CrdVersion;
  UINT8   MipiCam_Link4_DriverData_LinkUsed;
  UINT8   MipiCam_Link4_DriverData_LaneUsed;
  UINT8   MipiCam_Link4_DriverData_EepromType;
  UINT8   MipiCam_Link4_DriverData_EepromTypeCustom;
  UINT8   MipiCam_Link4_DriverData_VcmType;
  UINT8   MipiCam_Link4_DriverData_VcmTypeCustom;
  UINT8   MipiCam_Link4_DriverData_CustomData[4];
  UINT32  MipiCam_Link4_DriverData_Mclk;
  UINT8   MipiCam_Link4_DriverData_ControlLogic;
  UINT8   MipiCam_Link4_DriverData_FlashSupport;
  UINT8   MipiCam_Link4_DriverData_PrivacyLed;
  UINT8   MipiCam_Link4_DriverData_Degree;
  UINT8   MipiCam_Link4_DriverData_PmicPosition;
  UINT8   MipiCam_Link4_DriverData_VoltageRail;
  UINT8   MipiCam_Link4_DriverData_PhyConfiguration;
  UINT8   MipiCam_Link4_DriverData_LaneConfiguration;
  UINT8   MipiCam_Link4_FlashDriverSelection;
  UINT8   MipiCam_Link4_DriverData_FlashID;

  UINT8   MipiCam_Link5_SensorModel;         // IMX135 / OV5693 / IMX179 / OV8858 / OV2740-IVCAM / IMX208 / OV9728 / IMX188 / User3
  UINT16  MipiCam_Link5_UserHid[MIPICAM_HID_LENGTH];
  UINT8   MipiCam_Link5_CameraPhysicalLocation;
  UINT16  MipiCam_Link5_ModuleName[MIPICAM_MODULE_NAME_LENGTH];
  UINT8   MipiCam_Link5_I2cDevicesEnabled;   // number of I2C devices defined for this link
  UINT8   MipiCam_Link5_I2cChannel;
  UINT16  MipiCam_Link5_I2cAddress[MIPICAM_I2C_DEVICES_COUNT];
  UINT8   MipiCam_Link5_I2cDeviceType[MIPICAM_I2C_DEVICES_COUNT];
  UINT8   MipiCam_Link5_DriverData_Version;
  UINT8   MipiCam_Link5_DriverData_CrdVersion;
  UINT8   MipiCam_Link5_DriverData_LinkUsed;
  UINT8   MipiCam_Link5_DriverData_LaneUsed;
  UINT8   MipiCam_Link5_DriverData_EepromType;
  UINT8   MipiCam_Link5_DriverData_EepromTypeCustom;
  UINT8   MipiCam_Link5_DriverData_VcmType;
  UINT8   MipiCam_Link5_DriverData_VcmTypeCustom;
  UINT8   MipiCam_Link5_DriverData_CustomData[4];
  UINT32  MipiCam_Link5_DriverData_Mclk;
  UINT8   MipiCam_Link5_DriverData_ControlLogic;
  UINT8   MipiCam_Link5_DriverData_FlashSupport;
  UINT8   MipiCam_Link5_DriverData_PrivacyLed;
  UINT8   MipiCam_Link5_DriverData_Degree;
  UINT8   MipiCam_Link5_DriverData_PmicPosition;
  UINT8   MipiCam_Link5_DriverData_VoltageRail;
  UINT8   MipiCam_Link5_DriverData_PhyConfiguration;
  UINT8   MipiCam_Link5_DriverData_LaneConfiguration;
  UINT8   MipiCam_Link5_FlashDriverSelection;
  UINT8   MipiCam_Link5_DriverData_FlashID;

  UINT8   MipiCam_Flash0_Model;                                                    // LM3643 / Internal PMIC - WRC
  UINT8   MipiCam_Flash0_Mode;
  UINT16  MipiCam_Flash0_UserHid[MIPICAM_HID_LENGTH];
  UINT16  MipiCam_Flash0_ModuleName[MIPICAM_MODULE_NAME_LENGTH];
  UINT8   MipiCam_Flash0_I2cChannel;
  UINT16  MipiCam_Flash0_I2cAddress;
  UINT8   MipiCam_Flash0_GpioGroupPadNumber;
  UINT16  MipiCam_Flash0_GpioGroupNumber;
  UINT16  MipiCam_Flash0_GpioComNumber;
  UINT8   MipiCam_Flash0_GpioActiveValue;
  UINT8   MipiCam_Flash0_GpioInitialValue;
  UINT8   MipiCam_Flash0_OperatingMode;

  UINT8   MipiCam_Flash1_Model;                                                    // LM3643 / Internal PMIC - WRC
  UINT8   MipiCam_Flash1_Mode;
  UINT16  MipiCam_Flash1_UserHid[MIPICAM_HID_LENGTH];
  UINT16  MipiCam_Flash1_ModuleName[MIPICAM_MODULE_NAME_LENGTH];
  UINT8   MipiCam_Flash1_I2cChannel;
  UINT16  MipiCam_Flash1_I2cAddress;
  UINT8   MipiCam_Flash1_GpioGroupPadNumber;
  UINT16  MipiCam_Flash1_GpioGroupNumber;
  UINT16  MipiCam_Flash1_GpioComNumber;
  UINT8   MipiCam_Flash1_GpioActiveValue;
  UINT8   MipiCam_Flash1_GpioInitialValue;
  UINT8   MipiCam_Flash1_OperatingMode;

  UINT8   MipiCam_Flash2_Model;                                                    // LM3643 / Internal PMIC - WRC
  UINT8   MipiCam_Flash2_Mode;
  UINT16  MipiCam_Flash2_UserHid[MIPICAM_HID_LENGTH];
  UINT16  MipiCam_Flash2_ModuleName[MIPICAM_MODULE_NAME_LENGTH];
  UINT8   MipiCam_Flash2_I2cChannel;
  UINT16  MipiCam_Flash2_I2cAddress;
  UINT8   MipiCam_Flash2_GpioGroupPadNumber;
  UINT16  MipiCam_Flash2_GpioGroupNumber;
  UINT16  MipiCam_Flash2_GpioComNumber;
  UINT8   MipiCam_Flash2_GpioActiveValue;
  UINT8   MipiCam_Flash2_GpioInitialValue;
  UINT8   MipiCam_Flash2_OperatingMode;

  UINT8   MipiCam_Flash3_Model;                                                    // LM3643 / Internal PMIC - WRC
  UINT8   MipiCam_Flash3_Mode;
  UINT16  MipiCam_Flash3_UserHid[MIPICAM_HID_LENGTH];
  UINT16  MipiCam_Flash3_ModuleName[MIPICAM_MODULE_NAME_LENGTH];
  UINT8   MipiCam_Flash3_I2cChannel;
  UINT16  MipiCam_Flash3_I2cAddress;
  UINT8   MipiCam_Flash3_GpioGroupPadNumber;
  UINT16  MipiCam_Flash3_GpioGroupNumber;
  UINT16  MipiCam_Flash3_GpioComNumber;
  UINT8   MipiCam_Flash3_GpioActiveValue;
  UINT8   MipiCam_Flash3_GpioInitialValue;
  UINT8   MipiCam_Flash3_OperatingMode;

  UINT8   MipiCam_Flash4_Model;                                                    // LM3643 / Internal PMIC - WRC
  UINT8   MipiCam_Flash4_Mode;
  UINT16  MipiCam_Flash4_UserHid[MIPICAM_HID_LENGTH];
  UINT16  MipiCam_Flash4_ModuleName[MIPICAM_MODULE_NAME_LENGTH];
  UINT8   MipiCam_Flash4_I2cChannel;
  UINT16  MipiCam_Flash4_I2cAddress;
  UINT8   MipiCam_Flash4_GpioGroupPadNumber;
  UINT16  MipiCam_Flash4_GpioGroupNumber;
  UINT16  MipiCam_Flash4_GpioComNumber;
  UINT8   MipiCam_Flash4_GpioActiveValue;
  UINT8   MipiCam_Flash4_GpioInitialValue;
  UINT8   MipiCam_Flash4_OperatingMode;

  UINT8   MipiCam_Flash5_Model;                                                    // LM3643 / Internal PMIC - WRC
  UINT8   MipiCam_Flash5_Mode;
  UINT16  MipiCam_Flash5_UserHid[MIPICAM_HID_LENGTH];
  UINT16  MipiCam_Flash5_ModuleName[MIPICAM_MODULE_NAME_LENGTH];
  UINT8   MipiCam_Flash5_I2cChannel;
  UINT16  MipiCam_Flash5_I2cAddress;
  UINT8   MipiCam_Flash5_GpioGroupPadNumber;
  UINT16  MipiCam_Flash5_GpioComNumber;
  UINT16  MipiCam_Flash5_GpioGroupNumber;
  UINT8   MipiCam_Flash5_GpioActiveValue;
  UINT8   MipiCam_Flash5_GpioInitialValue;
  UINT8   MipiCam_Flash5_OperatingMode;
  UINT8   MipiCam_Flash0_GpioSupport;
  UINT8   MipiCam_Flash1_GpioSupport;
  UINT8   MipiCam_Flash2_GpioSupport;
  UINT8   MipiCam_Flash3_GpioSupport;
  UINT8   MipiCam_Flash4_GpioSupport;
  UINT8   MipiCam_Flash5_GpioSupport;
  UINT8   MipiCam_Link0_Customize_Id[MIPICAM_CUSTOMIZE_DEVICE_COUNT];
  UINT8   MipiCam_Link1_Customize_Id[MIPICAM_CUSTOMIZE_DEVICE_COUNT];
  UINT8   MipiCam_Link2_Customize_Id[MIPICAM_CUSTOMIZE_DEVICE_COUNT];
  UINT8   MipiCam_Link3_Customize_Id[MIPICAM_CUSTOMIZE_DEVICE_COUNT];
  UINT8   MipiCam_Link4_Customize_Id[MIPICAM_CUSTOMIZE_DEVICE_COUNT];
  UINT8   MipiCam_Link5_Customize_Id[MIPICAM_CUSTOMIZE_DEVICE_COUNT];
} MIPICAM_CONFIG;
#pragma pack ()

extern MIPICAM_CONFIG    mMipiCamSetupData;

#define MIPICAM_CONFIG_VARSTORE\
    efivarstore MIPICAM_CONFIG,\
        attribute = 0x7, name = MipiCamConfig, guid = gMipiCamConfigVariableGuid;

#endif // _MIPICAM_CONFIG_H_
