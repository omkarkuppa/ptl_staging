/** @file
  I3C config defines.

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

#ifndef _I3C_CONFIG_H_
#define _I3C_CONFIG_H_

#define LPSS_I3C_CONFIG_REVISION 1
extern EFI_GUID gLpssI3cConfigGuid;
#pragma pack (push,1)

#include <ConfigBlock.h>
#include <Library/PchLimits.h>

/**
  Available working modes for LPSS I3C Host Controller

       0: I3cDisabled;
          - Device is placed in D3
          - Gpio configuration is skipped
          - Device will be disabled in PSF
    <b>1: I3cPci</b>;
          - Gpio pin configuration in native mode for each assigned pin
            SDA and SCL
          - Device will be enabled in PSF
          - Only Bar 0 will be enabled
       2: I3cPhantom;
          - Dedicated mode for I3C1, controls if GPIOs are enabled
**/
typedef enum {
  I3cDisabled,
  I3cPci,
  I3cPhantom
} I3C_MODE;

/**
  I3C GPIO settings
**/
typedef struct {
  UINT32   PinMux;           /// GPIO signals pin muxing settings.
  UINT32   PadTermination;   /// GPIO Pads Internal Termination.
} I3C_MUX_GPIO_CONFIG;

/**
  The LPSS_I3C_DEVICE_CONFIG provides the configurations for Serial IO I3C controller
**/
typedef struct {
  UINT8                Mode;          ///< <b<I3cPci <b> see I3C_MODE
  UINT8                Reserved[3];   ///< Reserved bytes
  I3C_MUX_GPIO_CONFIG  Sda;
  I3C_MUX_GPIO_CONFIG  Scl;
  I3C_MUX_GPIO_CONFIG  SclFb;
} LPSS_I3C_DEVICE_CONFIG;

/**
  The LPSS_I3C_CONFIG block provides the configurations to set the Serial IO controllers

  <b>Revision 1:</b>
  - Inital version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER     Header;                                              ///< Config Block Header
  LPSS_I3C_DEVICE_CONFIG  I3cDeviceConfig[PCH_MAX_SERIALIO_I3C_CONTROLLERS];   ///< I3C Device Configuration
} LPSS_I3C_CONFIG;

#pragma pack (pop)

#endif // _I3C_CONFIG_H_
