/** @file
  Serial IO Private Uart Lib implementation MeteorLake specific.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PcdInfoLib.h>
#include <Defines/PchReservedResources.h>

/**
  Gets Fixed Base Address used for BAR0

  @param[in] UartNumber              Serial IO device UART number

  @retval                            Config control offset
**/
UINT32
GetSerialIoUartFixedMmioAddress (
  IN UINT8       UartNumber
  )
{
  return 0x0;
}

/**
  Gets Fixed Address used for Pci Config Space manipulation

  @param[in] UartNumber              Serial IO device UART number

  @retval                            Pci Config Address
**/
UINT32
GetSerialIoUartFixedPciCfgAddress (
  IN UINT8       UartNumber
  )
{
  return 0x0;
}

/**
  Gets Serial IO Uart Number based on its PciCfgSpace adrress

  @param[in]  PciCfgBase     PCI Config Base address

  @retval    UartNumber      Serial IO device UART number
                             0xFF is returned if no match was found
**/
UINT8
GetSerialIoUartNumber (
  IN UINT64            PciCfgBase
  )
{
  return 0xFF;
}

/**
  Gets Uarts Device Id

  @param[in] UartNumbe               Serial IO device UART number

  @retval                            Device Id
**/
UINT16
GetSerialIoUartDeviceId (
  IN UINT8       UartNumber
  )
{
  return 0xFFFF;
}
