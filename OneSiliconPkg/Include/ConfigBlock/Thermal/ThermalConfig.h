/** @file
  Thermal policy

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#ifndef _THERMAL_CONFIG_H_
#define _THERMAL_CONFIG_H_

#define THERMAL_CONFIG_REVISION 1
extern EFI_GUID gThermalConfigGuid;

#pragma pack (push,1)

/**
  This structure lists PCH supported throttling register setting for custimization.
  When the SuggestedSetting is enabled, the customized values are ignored.
**/
typedef struct {
  UINT32 T0Level                  :  9; ///< Custimized T0Level value. If SuggestedSetting is used, this setting is ignored.
  UINT32 T1Level                  :  9; ///< Custimized T1Level value. If SuggestedSetting is used, this setting is ignored.
  UINT32 T2Level                  :  9; ///< Custimized T2Level value. If SuggestedSetting is used, this setting is ignored.
  UINT32 TTEnable                 :  1; ///< Enable the thermal throttle function. If SuggestedSetting is used, this settings is ignored.
  /**
    When set to 1 and the programmed GPIO pin is a 1, then PMSync state 13 will force at least T2 state.
    If SuggestedSetting is used, this setting is ignored.
  **/
  UINT32 TTState13Enable          :  1;
  /**
    When set to 1, this entire register (TL) is locked and remains locked until the next platform reset.
    If SuggestedSetting is used, this setting is ignored.
  **/
  UINT32 TTLock                   :  1;
  UINT32 SuggestedSetting         :  1; ///< 0: Disable; <b>1: Enable</b> suggested representative values.
  UINT32 RsvdBits0                :  1; ///< Reserved bits.
  UINT32 Rsvd0;                      ///< Reserved bytes
} THERMAL_THROTTLE_LEVELS;

//
// Supported Thermal Sensor Target Width
//
typedef enum {
  DmiThermSensWidthX1  = 0,
  DmiThermSensWidthX2  = 1,
  DmiThermSensWidthX4  = 2,
  DmiThermSensWidthX8  = 3,
  DmiThermSensWidthX16 = 4
} DMI_THERMAL_SENSOR_TARGET_WIDTH;

/**
  This structure allows to customize DMI HW Autonomous Width Control for Thermal and Mechanical spec design.
  When the SuggestedSetting is enabled, the customized values are ignored.
  Look at DMI_THERMAL_SENSOR_TARGET_WIDTH for possible values
**/
typedef struct {
  UINT32  DmiTsawEn               :  1; ///< DMI Thermal Sensor Autonomous Width Enable
  UINT32  SuggestedSetting        :  1; ///< 0: Disable; <b>1: Enable</b> suggested representative values
  UINT32  RsvdBits0               :  6; ///< Reserved bits
  UINT32  TS0TW                   :  3; ///< Thermal Sensor 0 Target Width (<b>DmiThermSensWidthx8</b>)
  UINT32  TS1TW                   :  3; ///< Thermal Sensor 1 Target Width (<b>DmiThermSensWidthx4</b>)
  UINT32  TS2TW                   :  3; ///< Thermal Sensor 2 Target Width (<b>DmiThermSensWidthx2</b>)
  UINT32  TS3TW                   :  3; ///< Thermal Sensor 3 Target Width (<b>DmiThermSensWidthx1</b>)
  UINT32  RsvdBits1               : 12; ///< Reserved bits
} DMI_HW_WIDTH_CONTROL;

/**
  The THERMAL_CONFIG block describes the expected configuration of the Thermal IP block.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header
  UINT32  PchHotEnable            :  1; ///< Enable PCHHOT# pin assertion when temperature is higher than PchHotLevel. 0: <b>Disabled<b>, 1: Enabled.
  UINT32  RsvdBits0               : 31;
  /**
    This field decides the settings of Thermal throttling. When the Suggested Setting
    is enabled, PCH RC will use the suggested representative values.
  **/
  THERMAL_THROTTLE_LEVELS   TTLevels;
  /**
    This field decides the settings of DMI throttling. When the Suggested Setting
    is enabled, PCH RC will use the suggested representative values.
  **/
  DMI_HW_WIDTH_CONTROL      DmiHaAWC;
  /**
    The recommendation is the same as Cat Trip point.
    This field decides the temperature, default is <b>120</b>.
    Temperature value used for PCHHOT# pin assertion based on 2s complement format
    - 0x001 positive 1'C
    - 0x000 0'C
    - 0x1FF negative 1'C
    - 0x1D8 negative 40'C
    - and so on
  **/
  UINT16                    PchHotLevel;
  UINT8                     Rsvd0[6];
} THERMAL_CONFIG;

#pragma pack (pop)

#endif // _THERMAL_CONFIG_H_
