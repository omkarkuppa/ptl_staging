/** @file
  Serial IO Private I2c Lib implementation PantherLake specific.
  All functions in this library are available in PEI,
  But do not support UEFI RUNTIME environment call.

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/SerialIoI2cSocLib.h>
#include <SerialIoDevices.h>
#include <Ppi/SiPolicy.h>

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
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Initialize the I2c controller

  @param[in, out] I2cHandle       The SerialIo I2C Handle instance
  @param[in]      SiPolicy         The SI Policy PPI instance
  @param[in]      SerialIoConfig   SerialIo Config Block
**/
VOID
SerialIoI2cSocInit (
  IN OUT SERIAL_IO_I2C_HANDLE   *I2cHandle,
  IN     SI_POLICY_PPI          *SiPolicy,
  IN     SERIAL_IO_CONFIG       *SerialIoConfig
  )
{
  return;
}
