/** @file
  I2C config defines.

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

#ifndef _I2C_CONFIG_H_
#define _I2C_CONFIG_H_

#define LPSS_I2C_CONFIG_REVISION 1
extern EFI_GUID gLpssI2cConfigGuid;
#pragma pack (push,1)

#include <ConfigBlock.h>
#include <Library/PchLimits.h>

/**
  Available working modes for Lpss I2C Host Controller

       0: LpssI2cDisabled;
          - Device is placed in D3
          - Gpio configuration is skipped
          - PSF:
          !important! If given device is Function 0 and other higher functions on given device
                      are enabled, PSF disabling is skipped. PSF default will remain and device PCI CFG Space will still be visible.
                      This is needed to allow PCI enumerator access functions above 0 in a multifunction device.
    <b>1: LpssI2cPci;</b>
          - Gpio pin configuration in native mode for each assigned pin
          - Device will be enabled in PSF
          - Only BAR0 will be enabled
**/
typedef enum {
  LpssI2cDisabled,
  LpssI2cPci
} LPSS_I2C_I2C_MODE;

/**
  I2C signals pin muxing settings. If signal can be enable only on a single pin
  then this parameter is ignored by RC. Refer to GPIO_*_MUXING_SERIALIO_I2Cx_* in GpioPins*.h
  for supported settings on a given platform
**/
typedef struct {
  UINT32   Sda;   ///< SDA Pin mux configuration. Refer to GPIO_*_MUXING_SERIALIO_I2Cx_SDA_*
  UINT32   Scl;   ///< SCL Pin mux configuration. Refer to GPIO_*_MUXING_SERIALIO_I2Cx_SCL_*
} LPSS_I2C_PIN_MUX;

/**
  Serial IO I2C Controller Configuration
**/
typedef struct {
  UINT8        Mode;        /// <b>LpssI2cPci <b> see LPSS_I2C_I2C_MODE
  /**
    I2C Pads Internal Termination.
    For more information please see Platform Design Guide.
    Supported values (check GPIO_ELECTRICAL_CONFIG for reference):
    <b>GpioTermNone: No termination</b>,
    GpioTermWpu1K: 1kOhm weak pull-up,
    GpioTermWpu5K: 5kOhm weak pull-up,
    GpioTermWpu20K: 20kOhm weak pull-up
  **/
  UINT8             PadTermination;
  UINT8             Reserved[2];
  LPSS_I2C_PIN_MUX  PinMux;      ///< I2C pin mux configuration
} LPSS_I2C_CONTROLLER_CONFIG;
/**
  The LPSS_I2C_CONFIG block provides the configurations to set the Serial IO controllers

  <b>Revision 1:</b>
  - Inital version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER            Header;                                              ///< Config Block Header
  LPSS_I2C_CONTROLLER_CONFIG     I2cDeviceConfig[PCH_MAX_SERIALIO_I2C_CONTROLLERS];   ///< I2C Device Configuration
} LPSS_I2C_CONFIG;

#pragma pack (pop)

#endif // _I2C_CONFIG_H_
