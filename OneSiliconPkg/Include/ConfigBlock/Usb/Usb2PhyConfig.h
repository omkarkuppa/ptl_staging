/** @file
  USB2 PHY configuration policy

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#ifndef _USB2_PHY_CONFIG_H_
#define _USB2_PHY_CONFIG_H_

#include <UsbConfig.h>

#define USB2_PHY_CONFIG_REVISION 1
extern EFI_GUID gUsb2PhyConfigGuid;

#pragma pack (push,1)

/**
  This structure configures per USB2 AFE settings.
  It allows to setup the port electrical parameters.
**/
typedef struct {
/** Per Port HS Preemphasis Bias (PERPORTPETXISET)
  000b - 0mV
  001b - 11.25mV
  010b - 16.9mV
  011b - 28.15mV
  100b - 28.15mV
  101b - 39.35mV
  110b - 45mV
  111b - 56.3mV
**/
  UINT8   Petxiset;
/** Per Port HS Transmitter Bias (PERPORTTXISET)
  000b - 0mV
  001b - 11.25mV
  010b - 16.9mV
  011b - 28.15mV
  100b - 28.15mV
  101b - 39.35mV
  110b - 45mV
  111b - 56.3mV
**/
  UINT8   Txiset;
/**
  Per Port HS Transmitter Emphasis (IUSBTXEMPHASISEN)
  00b - Emphasis OFF
  01b - De-emphasis ON
  10b - Pre-emphasis ON
  11b - Pre-emphasis & De-emphasis ON
**/
  UINT8   Predeemp;
/**
  Per Port Half Bit Pre-emphasis (PERPORTTXPEHALF)
  1b - half-bit pre-emphasis
  0b - full-bit pre-emphasis
**/
  UINT8   Pehalfbit;
/**
  Semi Flexi Pre/DE-emphasis (reg_predeemp_semiflexi_en):
  1: Choose the predeemp setting (increase the DC amplitude)
  0: Choose the predeemp setting (decrease the DC amplitude)
**/
  UINT8   PredeempSemiflexEn;
  UINT8   ReservedBytes0[3];      // Reserved bytes
} USB2_PHY_PARAMETERS;

/**
  This structure holds info on how to tune electrical parameters of USB2 ports based on board layout

  <b>Revision 1</b>:
  - Initial version.

**/
typedef struct {
  CONFIG_BLOCK_HEADER     Header;                   ///< Config Block Header
  /**
    This structure configures per USB2 port physical settings.
    It allows to setup the port location and port length, and configures the port strength accordingly.
    Changing this policy values from default ones may require disabling USB2 PHY Sus Well Power Gating
    through Usb2PhySusPgEnable on PCH-LP
  **/
  USB2_PHY_PARAMETERS          Port[MAX_USB2_PORTS];
} USB2_PHY_CONFIG;

#pragma pack (pop)

#endif // _USB2_PHY_CONFIG_H_

