/** @file
  Serial Io I3C SoC library. Library allow to configure I3C controller
  All function in this library is available for PEI.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _SERIAL_IO_I3C_SOC_LIB_H_
#define _SERIAL_IO_I3C_SOC_LIB_H_

#include <I3cCtrl.h>
#include <ConfigBlock.h>
#include <SerialIoConfig.h>
#include <Ppi/SiPolicy.h>

#pragma pack (1)

/**
  Internal I3C policy options
**/
typedef struct {
  UINT8      InterruptPin[PCH_MAX_SERIALIO_I3C_CONTROLLERS];
  UINT8      Irq[PCH_MAX_SERIALIO_I3C_CONTROLLERS];
  BOOLEAN    IsPhantomController[PCH_MAX_SERIALIO_I3C_CONTROLLERS]; // If TRUE device does not map as I3C PCI controller
} SERIAL_IO_I3C_PRIVATE_CONFIG;

/**
  Configures GPIO for Serial IO I3C Controller

  @param[in] I3cNumber         I3C Number
  @param[in] I3cDeviceConfig   SerialIo I3C Config
**/
typedef
VOID
(*SERIAL_IO_I3C_GPIO_ENABLE) (
  IN UINT8                  I3cNumber,
  IN SERIAL_IO_I3C_CONFIG   *I3cDeviceConfig
  );

/**
  I3C callbacks
  List of function pointers that can be passed to the IP Block driver
**/
typedef struct {
  SERIAL_IO_I3C_GPIO_ENABLE                SerialIoI3cGpioEnable;
} SERIAL_IO_I3C_CALLBACK;

/**
  I3C controller structure
  Stores all data necessary to initialize Serial Io I3C IP block
**/

typedef struct {
  SERIAL_IO_I3C_CONTROLLER       Controller[PCH_MAX_SERIALIO_I3C_CONTROLLERS];
  SERIAL_IO_I3C_CONFIG           I3cConfig[PCH_MAX_SERIALIO_I3C_CONTROLLERS];
  SERIAL_IO_I3C_PRIVATE_CONFIG   PrivateConfig;
  SERIAL_IO_I3C_CALLBACK         Callback;
  UINT8                          I3cCount;
  UINT8                          ControllerId;
} SERIAL_IO_I3C_HANDLE;
#pragma pack ()

/**
  Build SerialIo I3C Handle with all Callbacks and Soc Private Config

  @param[in, out] I3cHandle        The SerialIo I3C Handle instance
  @param[in]      I3cDeviceConfig  SerialIo I3C Config
  @param[in]      I3cNumber        I3C Number

  @retval     EFI_SUCCESS           Completed successfully
              EFI_UNSUPPORTED       SOC is not supported
**/
VOID
SerialIoI3cBuildHandle (
  IN OUT SERIAL_IO_I3C_HANDLE    *I3cHandle,
  IN     SERIAL_IO_I3C_CONFIG    *I3cDeviceConfig,
  IN     UINT8                   SerialIoI3cNumber
  );

/**
  Initialize the I3c controller

  @param[in, out] I3cHandle        The SerialIo I3C Handle instance
  @param[in]      SiPolicy         The SI Policy PPI instance
  @param[in]      SerialIoConfig   SerialIo Config Block
**/
VOID
SerialIoI3cSocInit (
  IN OUT SERIAL_IO_I3C_HANDLE   *I3cHandle,
  IN     SI_POLICY_PPI          *SiPolicy,
  IN     SERIAL_IO_CONFIG       *SerialIoConfig
  );

#endif //_SERIAL_IO_I3C_SOC_LIB_H_
