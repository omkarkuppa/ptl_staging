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
#include <Defines/PchReservedResources.h>


typedef struct {
  UINT32 Bar0;
  UINT32 Bar1;
} LPSS_CONTROLLER_DESCRIPTOR;

GLOBAL_REMOVE_IF_UNREFERENCED LPSS_CONTROLLER_DESCRIPTOR mPtlPcdLpssUartFixedOffset [] = {
  {PCH_SERIAL_IO_BASE_ADDRESS + 0xC000,   PCH_SERIAL_IO_BASE_ADDRESS + 0xD000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0xE000,   PCH_SERIAL_IO_BASE_ADDRESS + 0xF000},
  {PCH_SERIAL_IO_BASE_ADDRESS + 0x10000,  PCH_SERIAL_IO_BASE_ADDRESS + 0x11000}
};

/**
  Gets Fixed Base Address used for BAR0

  @param[in] UartNumber              Serial IO device UART number

  @retval                            Config control offset
**/
UINT32
GetLpssUartFixedBar0 (
  IN UINT8       UartNumber
  )
{
if (UartNumber < ARRAY_SIZE (mPtlPcdLpssUartFixedOffset)) {
    return mPtlPcdLpssUartFixedOffset[UartNumber].Bar0;
  }
  return 0x0;
}

/**
  Gets Fixed Address used for Pci Config Space manipulation

  @param[in] UartNumber              Serial IO device UART number

  @retval                            Pci Config Address
**/
UINT32
GetLpssUartFixedBar1 (
  IN UINT8       UartNumber
  )
{
  if (UartNumber < ARRAY_SIZE (mPtlPcdLpssUartFixedOffset)) {
    return mPtlPcdLpssUartFixedOffset[UartNumber].Bar1;
  }
  return 0x0;
}

