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
#include <Library/SerialIoUartDebugPropertyPcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Pi/PiBootMode.h>
#include <Pi/PiHob.h>
#include <Setup.h>

GLOBAL_REMOVE_IF_UNREFERENCED SERIAL_IO_UART_ATTRIBUTES mSerialIoAttributes;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN                   mDataReady = FALSE;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                     mDebugControllerNumber = 0xFF;

/**
  Note: HOB_LIB macros duplicate definition due to DebugLib being present in HobLib and Constructor cycle
**/
#define DEBUG_GET_HOB_LENGTH(HobStart) \
  ((*(EFI_HOB_GENERIC_HEADER **)&(HobStart))->HobLength)
#define DEBUG_GET_HOB_TYPE(HobStart) \
  ((*(EFI_HOB_GENERIC_HEADER **)&(HobStart))->HobType)
#define DEBUG_GET_NEXT_HOB(HobStart) \
  (VOID *)(*(UINT8 **)&(HobStart) + DEBUG_GET_HOB_LENGTH (HobStart))
#define DEBUG_END_OF_HOB_LIST(HobStart)  (DEBUG_GET_HOB_TYPE (HobStart) == (UINT16)EFI_HOB_TYPE_END_OF_HOB_LIST)
#define DEBUG_GET_GUID_HOB_DATA(HobStart) \
  (VOID *)(*(UINT8 **)&(HobStart) + sizeof (EFI_HOB_GUID_TYPE))

/**
  Note: GetNextHob duplicate definition due to DebugLib being present in HobLib and Constructor cycle

  Returns the next instance of a HOB type from the starting HOB.

  This function searches the first instance of a HOB type from the starting HOB pointer.
  If there does not exist such HOB type from the starting HOB pointer, it will return NULL.
  In contrast with macro GET_NEXT_HOB(), this function does not skip the starting HOB pointer
  unconditionally: it returns HobStart back if HobStart itself meets the requirement;
  caller is required to use GET_NEXT_HOB() if it wishes to skip current HobStart.

  If HobStart is NULL, then ASSERT().

  @param[in]  Type          The HOB type to return.
  @param[in]  HobStart      The starting HOB pointer to search from.

  @return The next instance of a HOB type from the starting HOB.

**/
STATIC
VOID *
DebugGetNextHob (
  IN UINT16                 Type,
  IN CONST VOID             *HobStart
  )
{
  EFI_PEI_HOB_POINTERS  Hob;
  Hob.Raw = (UINT8 *) HobStart;
  while (!DEBUG_END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == Type) {
      return Hob.Raw;
    }
    Hob.Raw = DEBUG_GET_NEXT_HOB (Hob);
  }
  return NULL;
}

/**
  The constructor function initializes the Serial Port Parameter Library

  @param[in]  ImageHandle  The firmware allocated handle for the EFI image.
  @param[in]  SystemTable  A pointer to the EFI System Table.

  @retval EFI_SUCCESS      The constructor always returns EFI_SUCCESS.
**/
EFI_STATUS
EFIAPI
DxeSmmSerialIoUartDebugPropertyLibConstructor  (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS             Status;
  EFI_PEI_HOB_POINTERS   GuidHob;
  UINTN                  Index;
  VOID                   *DebugHobList;
  DEBUG_CONFIG_DATA_HOB  *DebugConfigData;

  Status = EFI_NOT_READY;

  if (SystemTable != NULL) {
    DebugConfigData = NULL;
    DebugHobList = NULL;

    for (Index = 0; Index < SystemTable->NumberOfTableEntries; Index++) {
      if (CompareGuid (&gEfiHobListGuid, &(SystemTable->ConfigurationTable[Index].VendorGuid))) {
        DebugHobList = SystemTable->ConfigurationTable[Index].VendorTable;
        Status = EFI_SUCCESS;
      }
    }

    if (DebugHobList == NULL) {
      return Status;
    }

    GuidHob.Raw = (UINT8 *) DebugHobList;
    Status = EFI_NOT_FOUND;
    while ((GuidHob.Raw = DebugGetNextHob (EFI_HOB_TYPE_GUID_EXTENSION, GuidHob.Raw)) != NULL) {
      if (CompareGuid (&gDebugConfigHobGuid, &GuidHob.Guid->Name)) {
        Status = EFI_SUCCESS;
        break;
      }
      GuidHob.Raw = DEBUG_GET_NEXT_HOB (GuidHob);
    }

    if (Status == EFI_SUCCESS) {
      DebugConfigData = (DEBUG_CONFIG_DATA_HOB *) DEBUG_GET_GUID_HOB_DATA (GuidHob);
      if (DebugConfigData != NULL) {
        mDebugControllerNumber = DebugConfigData->SerialIoUartDebugControllerNumber;
        mSerialIoAttributes.BaudRate = DebugConfigData->SerialIoUartDebugBaudRate;
        mSerialIoAttributes.DataBits = DebugConfigData->SerialIoUartDebugDataBits;
        mSerialIoAttributes.StopBits = DebugConfigData->SerialIoUartDebugStopBits;
        mSerialIoAttributes.Parity   = DebugConfigData->SerialIoUartDebugParity;
        mSerialIoAttributes.AutoFlow = DebugConfigData->SerialIoUartDebugFlowControl;
        mDataReady = TRUE;
      }
    }
  }

  return Status;
}


/**
  Returns UART's attributes

  @param[in,out]  UartAttributes   Uart Attributes
**/
VOID
SerialIoUartDebugGetAttributes (
  IN OUT SERIAL_IO_UART_ATTRIBUTES *UartAttributes
  )
{
  if (mDataReady == TRUE) {
    UartAttributes->BaudRate = mSerialIoAttributes.BaudRate;
    UartAttributes->DataBits = mSerialIoAttributes.DataBits;
    UartAttributes->StopBits = mSerialIoAttributes.StopBits;
    UartAttributes->Parity   = mSerialIoAttributes.Parity;
    UartAttributes->AutoFlow = mSerialIoAttributes.AutoFlow;
    return;
  }

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
  if (mDataReady == TRUE) {
    return mDebugControllerNumber;
  }

  return SerialIoUartDebugPcdGetControllerNumber ();
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