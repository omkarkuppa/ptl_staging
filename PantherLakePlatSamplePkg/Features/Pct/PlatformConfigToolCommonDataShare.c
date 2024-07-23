/** @file
 Common Data Share for Platform Configuration Data Tool Init.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#ifndef _PLATFORM_CONFIG_TOOL_COMMON_DATA_SHARE_H_
#define _PLATFORM_CONFIG_TOOL_COMMON_DATA_SHARE_H_

#include "PlatformConfigToolInit.h"
#include "PlatformConfigToolStructures.h"
#include  <Pins/GpioPinsVer2Lp.h>
#include  <Pins/GpioPinsVer2H.h>

//
// Commom File Names
//
CHAR8 *PctConfigFileNames[] = {
  "GpioPreMem",
  "GpioPostMem",
  "AcpiConfig"
};

//
// Guid File Names
//
EFI_GUID  *PctConfigFileGuids[] = {
            &gPctPreMemFileNameGuid,
            &gPctPostMemFileNameGuid,
            &gPctAcpiFileNameGuid
};

UINTN  PctConfigFileCount = sizeof (PctConfigFileGuids) / sizeof (EFI_GUID *);
//
// Data Type Guids
//
PCT_CONFIG_DATA_TYPE_GUID  PctConfigDataTypeGuids[] = {
                             {PctPreMemGpio, &gPctPreMemGpioDataTypeGuid},
                             {PctPostMemGpio, &gPctPostMemGpioDataTypeGuid},
                             {PctAcpi, &gPctAcpiDataTypeGuid}
};

UINTN PctConfigDataTypeGuidsCount = sizeof (PctConfigDataTypeGuids) / sizeof (PCT_CONFIG_DATA_TYPE_GUID);

static GPIO_PIN_STRING ChipsetId [] = {
  {GPIO_VER2_H_CHIPSET_ID,  "GPIO_VER2_H_GROUP_"},
  {GPIO_VER2_LP_CHIPSET_ID, "GPIO_VER2_LP_GROUP_"}
};

static GPIO_PIN_STRING Group [] = {
  {GPIO_VER2_H_GROUP_GPP_A,    "GPP_A"},
  {GPIO_VER2_H_GROUP_GPP_R,    "GPP_R"},
  {GPIO_VER2_H_GROUP_GPP_B,    "GPP_B"},
  {GPIO_VER2_H_GROUP_VGPIO_0,  "VGPIO_0"},
  {GPIO_VER2_H_GROUP_GPP_D,    "GPP_D"},
  {GPIO_VER2_H_GROUP_GPP_C,    "GPP_C"},
  {GPIO_VER2_H_GROUP_GPP_S,    "GPP_S"},
  {GPIO_VER2_H_GROUP_GPP_G,    "GPP_G"},
  {GPIO_VER2_H_GROUP_VGPIO,    "VGPIO"},
  {GPIO_VER2_H_GROUP_GPD,      "GPD"},
  {GPIO_VER2_H_GROUP_GPP_E,    "GPP_E"},
  {GPIO_VER2_H_GROUP_GPP_F,    "GPP_F"},
  {GPIO_VER2_H_GROUP_VGPIO_3,  "VGPIO_3"},
  {GPIO_VER2_H_GROUP_GPP_H,    "GPP_H"},
  {GPIO_VER2_H_GROUP_GPP_J,    "GPP_J"},
  {GPIO_VER2_H_GROUP_GPP_K,    "GPP_K"},
  {GPIO_VER2_H_GROUP_GPP_I,    "GPP_I"},
  {GPIO_VER2_H_GROUP_JTAG,     "JTAG"},
  {GPIO_VER2_H_GROUP_CPU,      "CPU"},
  {GPIO_VER2_LP_GROUP_GPP_B,   "GPP_B"},
  {GPIO_VER2_LP_GROUP_GPP_T,   "GPP_T"},
  {GPIO_VER2_LP_GROUP_GPP_A,   "GPP_A"},
  {GPIO_VER2_LP_GROUP_GPP_R,   "GPP_R"},
  {GPIO_VER2_LP_GROUP_SPI,     "SPI"},
  {GPIO_VER2_LP_GROUP_GPD,     "GPD"},
  {GPIO_VER2_LP_GROUP_GPP_S,   "GPP_S"},
  {GPIO_VER2_LP_GROUP_GPP_H,   "GPP_H"},
  {GPIO_VER2_LP_GROUP_GPP_D,   "GPP_D"},
  {GPIO_VER2_LP_GROUP_GPP_U,   "GPP_U"},
  {GPIO_VER2_LP_GROUP_VGPIO,   "VGPIO"},
  {GPIO_VER2_LP_GROUP_GPP_C,   "GPP_C"},
  {GPIO_VER2_LP_GROUP_GPP_F,   "GPP_F"},
  {GPIO_VER2_LP_GROUP_HVCMOS,  "HVCMOS"},
  {GPIO_VER2_LP_GROUP_GPP_E,   "GPP_E"},
  {GPIO_VER2_LP_GROUP_JTAG,    "JTAG"},
  {GPIO_VER2_LP_GROUP_CPU,     "CPU"},
  {GPIO_VER2_LP_GROUP_VGPIO_3, "VGPIO_3"}
};

static GPIO_PIN_STRING GpioPadMode [] = {
  {GpioV2PadModeHardwareDefault, "Default"},
  {GpioV2PadModeGpio, "Gpio"},
  {GpioV2PadModeNative1, "Native1"},
  {GpioV2PadModeNative2, "Native2"},
  {GpioV2PadModeNative3, "Native3"},
  {GpioV2PadModeNative4, "Native4"},
  {GpioV2PadModeNative5, "Native5"},
  {GpioV2PadModeNative6, "Native6"},
  {GpioV2PadModeNative6, "Native7"}
};

static GPIO_PIN_STRING GpioPadOwner [] = {
  {GpioV2HostOwnDefault, "Default"},
  {GpioV2HostOwnAcpi, "Acpi"},
  {GpioV2HostOwnGpio, "Gpio"}
};

static GPIO_PIN_STRING GpioDirection [] = {
  {GpioV2DirDefault, "Default"},
  {GpioV2DirInOut, "InOut"},
  {GpioV2DirInInvOut, "InInvOut"},
  {GpioV2DirIn, "In"},
  {GpioV2DirInInv, "InInv"},
  {GpioV2DirOut, "Out"},
  {GpioV2DirNone, "None"}
};

static GPIO_PIN_STRING GpioOutputState [] = {
  {GpioV2StateDefault, "Default"},
  {GpioV2StateLow, "Low"},
  {GpioV2StateHigh, "High"}
};

static GPIO_PIN_STRING GpioInterruptConfiguration [] = {
  {GpioV2IntDefault, "Default"},
  {GpioV2IntDis, "Dis"},
  {GpioV2IntNmi, "Nmi"},
  {GpioV2IntSmi, "Smi"},
  {GpioV2IntSci, "Sci"},
  {GpioV2IntApic, "Apic"},
  {GpioV2IntLevel, "Level"},
  {GpioV2IntEdge, "Edge"},
  {GpioV2IntLvlEdgDis, "LvlEdgDis"},
  {GpioV2IntBothEdge, "BothEdge"}
};

static GPIO_PIN_STRING GpioResetConfiguration [] = {
  {GpioV2ResetDefault, "Default"},
  {GpioV2ResetResume, "Resume"},
  {GpioV2ResetHostDeep, "HostDeep"},
  {GpioV2ResetHost, "Platform"},
  {GpioV2DswReset, "Dsw"}
};

static GPIO_PIN_STRING GpioElectricalConfiguration [] = {
  {GpioV2TermDefault, "Default"},
  {GpioV2TermNone, "None"},
  {GpioV2TermWpd5K, "Wpd5K"},
  {GpioV2TermWpd20K, "Wpd20K"},
  {GpioV2TermWpu1K, "Wpu1K"},
  {GpioV2TermWpu2K, "Wpu2K"},
  {GpioV2TermWpu5K, "Wpu5K"},
  {GpioV2TermWpu20K, "Wpu20K"},
  {GpioV2TermWpu1K2K, "Wpu1K2K"},
  {GpioV2TermNative, "Native"},
};

static GPIO_PIN_STRING GpioLockConfiguration [] = {
  {GpioV2LockHardwareDefault, "Default"},
  {GpioV2Unlock, "PadConfigUn"},
  {GpioV2Lock, "PadConfig"}
};

GPIO_PIN_STRING GpioOtherConfiguration [] = {
  {GpioV2RxRaw1Default, "Default"},
  {GpioV2RxRaw1Dis, "Dis"},
  {GpioV2RxRaw1En, "En"}
};

GPIO_PIN_STRING *GpioConfiguration [] = {
  ChipsetId,
  Group,
  GpioPadMode,
  GpioPadOwner,
  GpioDirection,
  GpioOutputState,
  GpioInterruptConfiguration,
  GpioResetConfiguration,
  GpioElectricalConfiguration,
  GpioLockConfiguration,
  GpioOtherConfiguration
};

UINTN DebugOutputGpioCount [] = {
  sizeof (ChipsetId) / sizeof (GPIO_PIN_STRING),
  sizeof (Group) / sizeof (GPIO_PIN_STRING),
  sizeof (GpioPadMode) / sizeof (GPIO_PIN_STRING),
  sizeof (GpioPadOwner) / sizeof (GPIO_PIN_STRING),
  sizeof (GpioDirection) / sizeof (GPIO_PIN_STRING),
  sizeof (GpioOutputState) / sizeof (GPIO_PIN_STRING),
  sizeof (GpioInterruptConfiguration) / sizeof (GPIO_PIN_STRING),
  sizeof (GpioResetConfiguration) / sizeof (GPIO_PIN_STRING),
  sizeof (GpioElectricalConfiguration) / sizeof (GPIO_PIN_STRING),
  sizeof (GpioLockConfiguration) / sizeof (GPIO_PIN_STRING),
  sizeof (GpioOtherConfiguration) / sizeof (GPIO_PIN_STRING)
};

#endif // _PLATFORM_CONFIG_DATA_SHARE_H_
