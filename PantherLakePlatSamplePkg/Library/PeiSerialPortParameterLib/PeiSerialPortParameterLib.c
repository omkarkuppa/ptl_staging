/** @file
  Pei Serial Port Parameter library instance.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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

#include <PiPei.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/SerialPortParameterLib.h>
#include <Setup.h>

/**
  Returns the serial port baud rate.

  @return  Baud rate of serial port.

**/
UINT32
EFIAPI
GetSerialPortBaudRate (
  VOID
  )
{
  EFI_PEI_HOB_POINTERS         Hob;
  DEBUG_CONFIG_DATA_HOB        *DebugConfigData;
  UINT32                       SerialPortBaudRate;

  //
  // Use PeiServicesGetHobList() to check HobList is ready or not.
  //
  PeiServicesGetHobList ((VOID **) &Hob.Raw);
  if (Hob.Raw == NULL) {
    return PcdGet8(PcdStatusCodeFlags);
  }

  DebugConfigData = NULL;
  SerialPortBaudRate = 0;

  Hob.Guid = GetFirstGuidHob (&gDebugConfigHobGuid);
  if (Hob.Guid != NULL) {
    DebugConfigData = (DEBUG_CONFIG_DATA_HOB *) GET_GUID_HOB_DATA (Hob.Guid);
    if (DebugConfigData != NULL) {
      switch (DebugConfigData->SerialDebugBaudRate) {
      case 3:
        SerialPortBaudRate = 9600;
        break;
      case 4:
        SerialPortBaudRate = 19200;
        break;
      case 6:
        SerialPortBaudRate = 57600;
        break;
      default:
        SerialPortBaudRate = 115200;
      }
      return SerialPortBaudRate;
    }
  }

  return PcdGet32 (PcdSerialBaudRate);
}

/**
  Sets the serial port baud rate value.

  @param[in] BaudRate     Baud rate value to be set.

  @retval  TRUE           The baud rate of serial port was sucessfully set.
  @retval  FALSE          The baud rate of serial port could not be set.

**/
BOOLEAN
EFIAPI
SetSerialPortBaudRate (
  IN UINT32        BaudRate
  )
{
  return FALSE;
}

/**
  Returns enabled debug interfaces.

  @retval  Enabled debug interfaces bitmask.
**/
UINT8
GetDebugInterface (
  VOID
  )
{
  EFI_PEI_HOB_POINTERS         Hob;
  DEBUG_CONFIG_DATA_HOB        *DebugConfigData;
  UINT8                        DebugInterfaces;

  //
  // Use PeiServicesGetHobList() to check HobList is ready or not.
  //
  PeiServicesGetHobList ((VOID **) &Hob.Raw);
  if (Hob.Raw == NULL) {
    return PcdGet8(PcdStatusCodeFlags);
  }

  DebugConfigData = NULL;
  DebugInterfaces = 0;

  Hob.Guid = GetFirstGuidHob (&gDebugConfigHobGuid);
  if (Hob.Guid != NULL) {
    DebugConfigData = (DEBUG_CONFIG_DATA_HOB *) GET_GUID_HOB_DATA (Hob.Guid);
    if (DebugConfigData != NULL) {
      if (DebugConfigData->RamDebugInterface)      { DebugInterfaces |= STATUS_CODE_USE_RAM; }
      if (DebugConfigData->UartDebugInterface)     { DebugInterfaces |= STATUS_CODE_USE_ISA_SERIAL; }
      if (DebugConfigData->Usb3DebugInterface)     { DebugInterfaces |= STATUS_CODE_USE_USB3; }
      if (DebugConfigData->SerialIoDebugInterface) { DebugInterfaces |= STATUS_CODE_USE_SERIALIO; }
      if (DebugConfigData->TraceHubDebugInterface) { DebugInterfaces |= STATUS_CODE_USE_TRACEHUB; }
      return DebugInterfaces;
    }
  }

  return PcdGet8(PcdStatusCodeFlags);
}

/**
  Sets debug interfaces.

  @param[in] DebugInterface  Debug interfaces to be set.
**/
VOID
SetDebugInterface (
  IN UINT8 DebugInterface
  )
{
  return;
}
