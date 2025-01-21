/** @file
  Serial IO I2C Private Lib implementation MeteorLake specific.
  All function in this library is available for PEI, DXE, and SMM,
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
#include <Library/DebugLib.h>

/**
  Gets I2C Device Id

  @param[in] I2cNumber               Serial IO device I2C number

  @retval                            Device Id
**/
UINT16
GetSerialIoI2cDeviceId (
  IN UINT8       I2cNumber
  )
{
  return 0xFFFF;
}

/**
  Gets Serial IO I2c Number based on its PciCfgSpace address

  @param[in]  PciCfgBase     PCI Config Base address

  @retval    I2cNumber       Serial IO device I2C number
                             0xFF is returned if no match was found
**/
UINT8
GetSerialIoI2cNumber (
  IN UINT64            PciCfgBase
  )
{
  return 0xFF;
}