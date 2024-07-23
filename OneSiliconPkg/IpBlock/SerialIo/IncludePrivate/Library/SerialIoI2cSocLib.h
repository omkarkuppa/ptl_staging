/** @file
  Serial Io I2C SoC library. Library allow to configure I2C controller
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

#ifndef _SERIAL_IO_I2C_SOC_LIB_H_
#define _SERIAL_IO_I2C_SOC_LIB_H_

#include <I2cCtrl.h>
#include <Library/PchLimits.h>
#include <ConfigBlock.h>
#include <SerialIoConfig.h>
#include <Ppi/SiPolicy.h>

#pragma pack (1)

/**
  Internal I2C policy options
**/
typedef struct {
  UINT8      InterruptPin[PCH_MAX_SERIALIO_I2C_CONTROLLERS];
  UINT8      Irq[PCH_MAX_SERIALIO_I2C_CONTROLLERS];
} SERIAL_IO_I2C_PRIVATE_CONFIG;

/**
  Configures GPIO for Serial IO I2C Controller

  @param[in] I2cNumber         I2C Number
  @param[in] I2cDeviceConfig   SerialIo I2C Config
**/
typedef
VOID
(EFIAPI *SERIAL_IO_I2C_GPIO_ENABLE) (
  IN UINT8                  I2cNumber,
  IN SERIAL_IO_I2C_CONFIG   *I2cDeviceConfig
  );

/**
  I2C callbacks
  List of function pointers that can be passed to the IP Block driver
**/
typedef struct {
  SERIAL_IO_I2C_GPIO_ENABLE                SerialIoI2cGpioEnable;
} SERIAL_IO_I2C_CALLBACK;

/**
  I2C controller structure
  Stores all data necessary to initialize Serial Io I2C IP block
**/

typedef struct {
  SERIAL_IO_I2C_CONTROLLER       Controller[PCH_MAX_SERIALIO_I2C_CONTROLLERS];
  SERIAL_IO_I2C_CONFIG           I2cConfig[PCH_MAX_SERIALIO_I2C_CONTROLLERS];
  SERIAL_IO_I2C_PRIVATE_CONFIG   PrivateConfig;
  SERIAL_IO_I2C_CALLBACK         Callback;
  UINT8                          I2cCount;
  UINT8                          ControllerId;
} SERIAL_IO_I2C_HANDLE;
#pragma pack ()

/**
  Build SerialIo I2C Handle with all Callbacks and Soc Private Config

  @param[in, out] I2cHandle        The SerialIo I2C Handle instance
  @param[in]      I2cDeviceConfig  SerialIo I2C Config
  @param[in]      PciCfgBase       PCI Config Base address
  @param[in]      I2cNumber        I2C Number

  @retval     EFI_SUCCESS           Completed successfully
              EFI_UNSUPPORTED       SOC is not supported
**/
EFI_STATUS
SerialIoI2cBuildHandle (
  IN OUT SERIAL_IO_I2C_HANDLE    *I2cHandle,
  IN     SERIAL_IO_I2C_CONFIG    *I2cDeviceConfig,
  IN     UINT64                  I2cPciCfgBase,
  IN     UINT8                   *I2cNumber     OPTIONAL
  );

/**
  Initialize the I2c controller

  @param[in, out] I2cHandle        The SerialIo I2C Handle instance
  @param[in]      SiPolicy         The SI Policy PPI instance
  @param[in]      SerialIoConfig   SerialIo Config Block
**/
VOID
SerialIoI2cSocInit (
  IN OUT SERIAL_IO_I2C_HANDLE   *I2cHandle,
  IN     SI_POLICY_PPI          *SiPolicy,
  IN     SERIAL_IO_CONFIG       *SerialIoConfig
  );

/**
  Gets Serial IO I2c Number based on its PciCfgSpace address

  @param[in]  PciCfgBase     PCI Config Base address

  @retval    I2cNumber      Serial IO device I2C number
                             0xFF is returned if no match was found
**/
UINT8
GetSerialIoI2cNumber (
  IN UINT64            PciCfgBase
  );

/**
  Gets I2cs Device Id

  @param[in] I2cNumbe               Serial IO device I2C number

  @retval                            Device Id
**/
UINT16
GetSerialIoI2cDeviceId (
  IN UINT8       I2cNumber
  );

#endif //_SERIAL_IO_I2C_SOC_LIB_H_
