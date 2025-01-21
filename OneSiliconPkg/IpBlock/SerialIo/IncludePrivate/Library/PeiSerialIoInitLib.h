/** @file
  Initializes Serial IO Controllers.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
#ifndef _PEI_SERIALIO_INIT_LIB_H_
#define _PEI_SERIALIO_INIT_LIB_H_

#include <Ppi/SiPolicy.h>
#include <Library/SerialIoUartSocLib.h>
#include <Library/SerialIoI2cSocLib.h>
#include <Library/SerialIoI3cSocLib.h>
#include <Library/SerialIoSpiSocLib.h>

/**
  Configures Serial IO Controllers

  @param[in] UartHandle        The SerialIo UART Handle instance
  @param[in] I2cHandle         The SerialIo I2C Handle instance
  @param[in] I3cHandle         The SerialIo I3C Handle instance
  @param[in] SpiHandle         The SerialIo SPI Handle instance
  @param[in] SiPolicy
**/
VOID
SerialIoInit (
  IN OUT SERIAL_IO_UART_HANDLE   *UartHandle,
  IN OUT SERIAL_IO_I2C_HANDLE    *I2cHandle,
  IN OUT SERIAL_IO_I3C_HANDLE    *I3cHandle,
  IN OUT SERIAL_IO_SPI_HANDLE    *SpiHandle,
  IN     SI_POLICY_PPI           *SiPolicy
  );

/**
  Configures Serial IO Function 0 Disabled Controllers

  @param[in] UartHandle       The SerialIo UART Handle instance
  @param[in] I2cHandle        The SerialIo I2C Handle instance
  @param[in] SpiHandle        The SerialIo SPI Handle instance
  @param[in] I3cHandle        The SerialIo I3C Handle instance
**/
VOID
SerialIoFunction0Disable (
  IN SERIAL_IO_UART_HANDLE   *UartHandle,
  IN SERIAL_IO_I2C_HANDLE    *I2cHandle,
  IN SERIAL_IO_SPI_HANDLE    *SpiHandle,
  IN SERIAL_IO_I3C_HANDLE    *I3cHandle
  );

/**
  Configures Serial IO Uart Controller

  @param[in] UartHandle        The SerialIo UART Handle instance
  @param[in] PsfDisable        Applies only for SerialIoUartDisabled devices.
                               TRUE  - Device will be disabled in PSF, and will no longer enumerate on PCI.
                               FALSE - PSF configuration is left unmodified.
**/
VOID
SerialIoUartConfiguration (
  IN SERIAL_IO_UART_HANDLE  *UartHandle,
  IN BOOLEAN                PsfDisable
  );

/**
  Configures Serial IO Spi Controller

  @param[in] SpiHandle         The SerialIo SPI Handle instance
  @param[in] PsfDisable        Applies only for SerialIoSpiDisabled devices.
                               TRUE  - Device will be disabled in PSF, and will no longer enumerate on PCI.
                               FALSE - PSF configuration is left unmodified.
**/
VOID
SerialIoSpiConfiguration (
  IN SERIAL_IO_SPI_HANDLE   *SpiHandle,
  IN BOOLEAN                PsfDisable
  );

/**
  Configures Serial I2c IO Controller

  @param[in] I2cHandle         The SerialIo I2C Handle instance
  @param[in] PsfDisable        Applies only for SerialIoI2cDisabled devices.
                               TRUE  - Device will be disabled in PSF, and will no longer enumerate on PCI.
                               FALSE - PSF configuration is left unmodified.
**/
VOID
SerialIoI2cConfiguration (
  IN SERIAL_IO_I2C_HANDLE   *I2cHandle,
  IN BOOLEAN                PsfDisable
  );

/**
  Configures Serial IO Uart Controllers

  @param[in] UartHandle        The SerialIo UART Handle instance
  @param[in] SiPolicy          The SI Policy PPI instance
**/
VOID
SerialIoUartInit (
  IN SERIAL_IO_UART_HANDLE   *UartHandle
  );

/**
  Configures Serial IO I2c Controllers

  @param[in] I2cHandle        The SerialIo I2C Handle instance
**/
VOID
SerialIoI2cInit (
  IN SERIAL_IO_I2C_HANDLE   *I2cHandle
  );

/**
  Configures Serial IO I3C Controllers

  @param[in] I3cHandle        The SerialIo I3C Handle instance
**/
VOID
SerialIoI3cInit (
  IN SERIAL_IO_I3C_HANDLE   *I3cHandle
  );

/**
  Configures Serial IO SPI Controllers

  @param[in] SpiHandle        The SerialIo SPI Handle instance
**/
VOID
SerialIoSpiInit (
  IN SERIAL_IO_SPI_HANDLE   *SpiHandle
  );

/**
  Performs Early disable for Serial Io I2C Controller

  @param[in] SerialAccess         The SerialIo I2C Register Access

**/
VOID
SerialIoI2cEarlyDisable (
  IN REGISTER_ACCESS   *SerialIoAccess
  );


/**
  Configures Serial IO Uart Function 0 Disabled Controllers

  @param[in] UartHandle        The SerialIo UART Handle instance
**/
VOID
SerialIoUartFunction0Disable (
  IN SERIAL_IO_UART_HANDLE   *UartHandle
  );

/**
  Configures Serial IO Spi Function 0 Disabled Controllers

  @param[in] SpiHandle        The SerialIo SPI Handle instance
**/
VOID
SerialIoSpiFunction0Disable (
  IN SERIAL_IO_SPI_HANDLE   *SpiHandle
  );

/**
  Configures Serial IO I2c Function 0 Disabled Controllers

  @param[in] I2cHandle        The SerialIo I2C Handle instance
**/
VOID
SerialIoI2cFunction0Disable (
  IN SERIAL_IO_I2C_HANDLE   *I2cHandle
  );

/**
  Configures Serial IO I3c Function 0 Disabled Controllers

  @param[in] I3cHandle        The SerialIo I3C Handle instance
**/
VOID
SerialIoI3cFunction0Disable (
  IN SERIAL_IO_I3C_HANDLE   *I3cHandle
  );

#endif // _PEI_SERIALIO_INIT_LIB_H_
