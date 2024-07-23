/** @file
  Serial Io SPI SoC library. Library allow to configure SPI controller
  All functions in this library are available for PEI.

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

#ifndef _SERIAL_IO_SPI_SOC_LIB_H_
#define _SERIAL_IO_SPI_SOC_LIB_H_

#include <SpiCtrl.h>
#include <Library/PchLimits.h>
#include <ConfigBlock.h>
#include <SerialIoConfig.h>
#include <Ppi/SiPolicy.h>

#pragma pack (1)

/**
  Internal SPI policy options
**/
typedef struct {
  UINT8      InterruptPin[PCH_MAX_SERIALIO_SPI_CONTROLLERS];
  UINT8      Irq[PCH_MAX_SERIALIO_SPI_CONTROLLERS];
} SERIAL_IO_SPI_PRIVATE_CONFIG;

/**
  Configures GPIO for Serial IO SPI Controller

  @param[in] SpiNumber         SPI Number
  @param[in] SpiDeviceConfig   SerialIo SPI Config
**/
typedef
VOID
(EFIAPI *SERIAL_IO_SPI_GPIO_ENABLE) (
  IN UINT8                  SpiNumber,
  IN SERIAL_IO_SPI_CONFIG   *SpiDeviceConfig
  );

/**
  SPI callbacks
  List of function pointers that can be passed to the IP Block driver
**/
typedef struct {
  SERIAL_IO_SPI_GPIO_ENABLE                SerialIoSpiGpioEnable;
} SERIAL_IO_SPI_CALLBACK;

/**
  SPI controller structure
  Stores all data necessary to initialize Serial Io SPI IP block
**/
typedef struct {
  SERIAL_IO_SPI_CONTROLLER       Controller[PCH_MAX_SERIALIO_SPI_CONTROLLERS];
  SERIAL_IO_SPI_CONFIG           SpiConfig[PCH_MAX_SERIALIO_SPI_CONTROLLERS];
  SERIAL_IO_SPI_PRIVATE_CONFIG   PrivateConfig;
  SERIAL_IO_SPI_CALLBACK         Callback;
  UINT8                          SpiCount;
  UINT8                          ControllerId;
} SERIAL_IO_SPI_HANDLE;
#pragma pack ()

/**
  Build SerialIo SPI Handle with all Callbacks and Soc Private Config

  @param[in, out] SpiHandle        The SerialIo SPI Handle instance
  @param[in]      SpiDeviceConfig  SerialIo SPI Config
  @param[in]      PciCfgBase       PCI Config Base address
  @param[in]      SpiNumber        SPI Number

  @retval     EFI_SUCCESS           Completed successfully
              EFI_UNSUPPORTED       SOC is not supported
**/
EFI_STATUS
SerialIoSpiBuildHandle (
  IN OUT SERIAL_IO_SPI_HANDLE    *SpiHandle,
  IN     SERIAL_IO_SPI_CONFIG    *SpiDeviceConfig,
  IN     UINTN                   SpiPciCfgBase,
  IN     UINT8                   *SpiNumber     OPTIONAL
  );

/**
  Initialize the Spi controller

  @param[in, out] SpiHandle        The SerialIo SPI Handle instance
  @param[in]      SiPolicy         The SI Policy PPI instance
  @param[in]      SerialIoConfig   SerialIo Config Block
**/
VOID
SerialIoSpiSocInit (
  IN OUT SERIAL_IO_SPI_HANDLE   *SpiHandle,
  IN     SI_POLICY_PPI          *SiPolicy,
  IN     SERIAL_IO_CONFIG       *SerialIoConfig
  );

/**
  Gets Serial IO Spi Number based on its PciCfgSpace address

  @param[in]  PciCfgBase     PCI Config Base address

  @retval    SpiNumber      Serial IO device SPI number
                             0xFF is returned if no match was found
**/
UINT8
GetSerialIoSpiNumber (
  IN UINTN            PciCfgBase
  );

#endif //_SERIAL_IO_SPI_SOC_LIB_H_
