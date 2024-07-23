/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#include <Protocol/SerialIo.h>
#include <Library/HobLib.h>
#include <Library/SerialIoUartDebugPropertyPcdLib.h>
#include <Setup.h>

/**
  Returns UART's attributes

  @param[in,out]  UartAttributes   Uart Attributes
**/
VOID
SerialIoUartDebugGetAttributes (
  IN OUT SERIAL_IO_UART_ATTRIBUTES *UartAttributes
  )
{
  EFI_HOB_GUID_TYPE            *GuidHob;
  DEBUG_CONFIG_DATA_HOB        *DebugConfigData;

  GuidHob = NULL;
  DebugConfigData = NULL;
  GuidHob = GetFirstGuidHob (&gDebugConfigHobGuid);
  if (GuidHob != NULL) {
    DebugConfigData = (DEBUG_CONFIG_DATA_HOB *) GET_GUID_HOB_DATA (GuidHob);
    if (DebugConfigData != NULL) {
      UartAttributes->BaudRate = DebugConfigData->SerialIoUartDebugBaudRate;
      UartAttributes->DataBits = DebugConfigData->SerialIoUartDebugDataBits;
      UartAttributes->StopBits = DebugConfigData->SerialIoUartDebugStopBits;
      UartAttributes->Parity   = DebugConfigData->SerialIoUartDebugParity;
      UartAttributes->AutoFlow = DebugConfigData->SerialIoUartDebugFlowControl;
      return;
    }
  }

  //
  // Unable to obtain data from the HOB, returning data from Pcd
  //
  SerialIoUartDebugPcdGetAttributes (UartAttributes);
}

/**
  Returns Serial Io UART Controller Number used in Debug mode

  @retval  ControllerNumber   UART Controller Number
**/
UINT8
SerialIoUartDebugGetControllerNumber (
  VOID
  )
{
  EFI_HOB_GUID_TYPE            *GuidHob;
  DEBUG_CONFIG_DATA_HOB        *DebugConfigData;
  STATIC UINT8                 ControllerNumber = 0xFF;

  if (ControllerNumber == 0xFF) {
    GuidHob = NULL;
    DebugConfigData = NULL;
    GuidHob = GetFirstGuidHob (&gDebugConfigHobGuid);
    if (GuidHob != NULL) {
      DebugConfigData = (DEBUG_CONFIG_DATA_HOB *) GET_GUID_HOB_DATA (GuidHob);
      if (DebugConfigData != NULL) {
        ControllerNumber = DebugConfigData->SerialIoUartDebugControllerNumber;
        return DebugConfigData->SerialIoUartDebugControllerNumber;
      }
    }
    //
    // Unable to obtain data from the HOB, returning data from PcdLib
    //
    return SerialIoUartDebugPcdGetControllerNumber ();
  }

  return ControllerNumber;
}

/**
  Returns Serial Io UART MMIO Base Address

  @retval  MMIO Base Address by default in PCI Mode
**/
UINT32
SerialIoUartDebugGetPciDefaultMmioBase (
  VOID
  )
{
  return SerialIoUartDebugPcdGetPciDefaultMmioBase ();
}

/**
  Returns Serial Io UART PCI Config Base

  @retval  PciCfgBase
**/
UINT32
SerialIoUartDebugGetDefaultPciCfgBase (
  VOID
  )
{
  return SerialIoUartDebugPcdGetDefaultPciCfgBase ();
}