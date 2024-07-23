/** @file
  Serial Io UART SoC library. Library allow to configure UART controller
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

#ifndef _SERIAL_IO_UART_SOC_LIB_H_
#define _SERIAL_IO_UART_SOC_LIB_H_

#include <UartCtrl.h>
#include <Library/PchLimits.h>
#include <ConfigBlock.h>
#include <SerialIoConfig.h>
#include <Ppi/SiPolicy.h>

#pragma pack (1)

/**
  Internal UART policy options
**/
typedef struct {
  BOOLEAN    DisablePowerGating;
  UINT8      InterruptPin[PCH_MAX_SERIALIO_UART_CONTROLLERS];
  UINT8      Irq[PCH_MAX_SERIALIO_UART_CONTROLLERS];
} SERIAL_IO_UART_PRIVATE_CONFIG;

/**
  Configures GPIO for Serial IO UART Controller

  @param[in] UartNumber         UART Number
  @param[in] UartDeviceConfig   SerialIo UART Config
**/
typedef
VOID
(EFIAPI *SERIAL_IO_UART_GPIO_ENABLE) (
  IN UINT8                  UartNumber,
  IN SERIAL_IO_UART_CONFIG  *UartDeviceConfig
  );

/**
  Configures GPIO IO Standby for Serial IO UART Controller

  @param[in] UartNumber         UART Number
  @param[in] UartDeviceConfig   SerialIo UART Config
**/
typedef
VOID
(*SERIAL_IO_UART_GPIO_IO_STANDBY_CONFIGURE) (
  IN UINT8                   UartNumber,
  IN SERIAL_IO_UART_CONFIG   *UartDeviceConfig
  );

/**
  Disables UART controller PG capabilites

  @param[in]  UartController     Pointer to UART Controller structure
**/
typedef
VOID
(*SERIAL_IO_UART_DISABLE_PG) (
  IN  SERIAL_IO_UART_CONTROLLER    *UartController
  );

/**
  UART callbacks
  List of function pointers that can be passed to the IP Block driver
**/
typedef struct {
  SERIAL_IO_UART_GPIO_ENABLE                SerialIoUartGpioEnable;
  SERIAL_IO_UART_GPIO_IO_STANDBY_CONFIGURE  SerialIoUartGpioIoStandbyConfigure;
} SERIAL_IO_UART_CALLBACK;

/**
  UART controller structure
  Stores all data necessary to initialize Serial Io UART IP block
**/

typedef struct {
  SERIAL_IO_UART_CONTROLLER       Controller[PCH_MAX_SERIALIO_UART_CONTROLLERS];
  SERIAL_IO_UART_CONFIG           UartConfig[PCH_MAX_SERIALIO_UART_CONTROLLERS];
  SERIAL_IO_UART_PRIVATE_CONFIG   PrivateConfig;
  SERIAL_IO_UART_CALLBACK         Callback;
  UINT8                           UartCount;
  UINT8                           ControllerId;
} SERIAL_IO_UART_HANDLE;
#pragma pack ()

/**
  Build SerialIo UART Handle with all Callbacks and Soc Private Config

  @param[in, out] UartHandle        The SerialIo UART Handle instance
  @param[in]      UartDeviceConfig  SerialIo UART Config
  @param[in]      PciCfgBase        PCI Config Base address
  @param[in]      UartNumber        UART Number

  @retval     EFI_SUCCESS           Completed successfully
              EFI_UNSUPPORTED       SOC is not supported
**/
EFI_STATUS
SerialIoUartBuildHandle (
  IN OUT SERIAL_IO_UART_HANDLE   *UartHandle,
  IN     SERIAL_IO_UART_CONFIG   *UartDeviceConfig,
  IN     UINT64                  UartPciCfgBase,
  IN     UINT8                   *UartNumber     OPTIONAL
  );

/**
  Initialize the Uart controller

  @param[in, out] UartHandle       The SerialIo UART Handle instance
  @param[in]      SiPolicy         The SI Policy PPI instance
  @param[in]      SerialIoConfig   SerialIo Config Block
**/
VOID
SerialIoUartSocInit (
  IN OUT SERIAL_IO_UART_HANDLE   *UartHandle,
  IN     SI_POLICY_PPI           *SiPolicy,
  IN     SERIAL_IO_CONFIG        *SerialIoConfig
  );

/**
  Gets Serial IO Uart Number based on its PciCfgSpace adrress

  @param[in]  PciCfgBase     PCI Config Base address

  @retval    UartNumber      Serial IO device UART number
                             0xFF is returned if no match was found
**/
UINT8
GetSerialIoUartNumber (
  IN UINT64            PciCfgBase
  );

/**
  Gets Uarts Device Id

  @param[in] UartNumbe               Serial IO device UART number

  @retval                            Device Id
**/
UINT16
GetSerialIoUartDeviceId (
  IN UINT8       UartNumber
  );

/**
  Gets Fixed Base Address used for BAR0

  @param[in] UartNumber              Serial IO device UART number

  @retval                            Config control offset
**/
UINT32
GetSerialIoUartFixedMmioAddress (
  IN UINT8       UartNumber
  );

/**
  Gets Fixed Address used for Pci Config Space manipulation

  @param[in] UartNumber              Serial IO device UART number

  @retval                            Pci Config Address
**/
UINT32
GetSerialIoUartFixedPciCfgAddress (
  IN UINT8       UartNumber
  );

#endif //_SERIAL_IO_UART_SOC_LIB_H_
