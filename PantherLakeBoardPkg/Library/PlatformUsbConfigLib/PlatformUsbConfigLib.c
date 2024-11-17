/** @file
  Platform USB Config Library

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

#include <Uefi.h>
#include <Pi/PiBootMode.h>
#include <Pi/PiMultiPhase.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PlatformUsbConfigLib.h>
#include <Ucsi.h>

/**
  Build USB Connector HOB

  @retval EFI_SUCCESS                  USB Connector HOD is built successfully.
  @retval EFI_NOT_FOUND                PcdUsbConnectorTable is not available.
  @retval EFI_INVALID_PARAMETER        ConnectorCount is over the max supported number by platform.
**/
EFI_STATUS
EFIAPI
BuildUsbConnectorHob (
  VOID
  )
{
  UINT8                       ConnectorCount;
  USB_CONNECTOR_BOARD_CONFIG  *UsbConnectorBoardConfig;
  USB_CONNECTOR_HOB_DATA      UsbConnectorHobData;
  VOID                        *Hob;

  ConnectorCount = 0;
  UsbConnectorBoardConfig = (USB_CONNECTOR_BOARD_CONFIG *) PcdGet64 (PcdUsbConnectorTable);

  if (UsbConnectorBoardConfig == NULL) {
    DEBUG((DEBUG_ERROR, "%a: PcdUsbConnectorTable is not available!!\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  for (; *(UINT32 *) UsbConnectorBoardConfig != 0; UsbConnectorBoardConfig++) {
    ConnectorCount++;
  }

  if (ConnectorCount > MAX_USB_CONNECTOR_NUMBER) {
    DEBUG((DEBUG_ERROR, "Number of USB connectors is over the max supported number by platform!!\n"));
    return EFI_INVALID_PARAMETER;
  }

  DEBUG((DEBUG_INFO, "Number of USB connectors: %d\n", ConnectorCount));
  UsbConnectorHobData.NumberOfUsbConnectors = ConnectorCount;
  CopyMem (UsbConnectorHobData.UsbConnectorBoardConfig, (VOID *) PcdGet64 (PcdUsbConnectorTable), sizeof (USB_CONNECTOR_BOARD_CONFIG) * ConnectorCount);

  DEBUG((DEBUG_INFO, "%a: Creating HOB...\n", __FUNCTION__));
  Hob = BuildGuidDataHob (
          &gUsbConnectorHobGuid,
          &UsbConnectorHobData,
          (sizeof (USB_CONNECTOR_BOARD_CONFIG) * ConnectorCount) + sizeof (UsbConnectorHobData.NumberOfUsbConnectors)
          );
  ASSERT (Hob != NULL);

  return EFI_SUCCESS;
}

/**
  Get USB Connector HOB Data Pointer

  @retval USB_CONNECTOR_HOB_DATA *    USB Connector HOB Data Pointer
**/
USB_CONNECTOR_HOB_DATA *
EFIAPI
GetUsbConnectorHobData (
  VOID
  )
{
  VOID  *Hob;

  Hob = GetFirstGuidHob (&gUsbConnectorHobGuid);
  if (Hob == NULL) {
    DEBUG((DEBUG_ERROR, "%a: UsbConnectorHob Not Found!!\n", __FUNCTION__));
    return NULL;
  }

  return (USB_CONNECTOR_HOB_DATA *) GET_GUID_HOB_DATA (Hob);
}

/**
  Build USBC Connector HOB

  @retval EFI_SUCCESS                  USBC Connector HOD is built successfully.
  @retval EFI_NOT_FOUND                PcdUsbCConnectorTable is not available.
  @retval EFI_INVALID_PARAMETER        ConnectorCount is over the max supported number by platform.
**/
EFI_STATUS
EFIAPI
BuildUsbCConnectorHob (
  VOID
  )
{
  UINT8                       ConnectorCount;
  USBC_CONNECTOR_BOARD_CONFIG *UsbCConnectorBoardConfig;
  USBC_CONNECTOR_HOB_DATA     UsbCConnectorHobData;
  USB_CONNECTOR_HOB_DATA      *UsbConnectorHobDataPtr;
  VOID                        *Hob;

  ConnectorCount = 0;
  UsbCConnectorBoardConfig = (USBC_CONNECTOR_BOARD_CONFIG *) PcdGet64 (PcdUsbCConnectorTable);
  UsbConnectorHobDataPtr   = GetUsbConnectorHobData ();

  if (UsbConnectorHobDataPtr == NULL) {
    DEBUG((DEBUG_ERROR, "%a: UsbConnectorHobData is not available!!\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  if (UsbCConnectorBoardConfig == NULL) {
    DEBUG((DEBUG_ERROR, "%a: PcdUsbCConnectorTable is not available!!\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  for (; *(UINT32 *) UsbCConnectorBoardConfig != 0; UsbCConnectorBoardConfig++) {
    ConnectorCount++;
  }

  if (ConnectorCount > MAX_USBC_CONNECTOR_NUMBER) {
    DEBUG((DEBUG_ERROR, "Number of USBC connectors is over the max supported number by platform!!\n"));
    return EFI_INVALID_PARAMETER;
  }

  if (ConnectorCount > UsbConnectorHobDataPtr->NumberOfUsbConnectors) {
    DEBUG((DEBUG_WARN, "Number of USBC connectors should less or equal to number of USB connectors!!\n"));
    ConnectorCount = UsbConnectorHobDataPtr->NumberOfUsbConnectors;
  }

  DEBUG((DEBUG_INFO, "Number of USBC connectors: %d\n", ConnectorCount));
  UsbCConnectorHobData.NumberOfUsbCConnectors = ConnectorCount;
  CopyMem (UsbCConnectorHobData.UsbCConnectorBoardConfig, (VOID *) PcdGet64 (PcdUsbCConnectorTable), sizeof (USBC_CONNECTOR_BOARD_CONFIG) * ConnectorCount);

  DEBUG((DEBUG_INFO, "%a: Creating HOB...\n", __FUNCTION__));
  Hob = BuildGuidDataHob (
          &gUsbCConnectorHobGuid,
          &UsbCConnectorHobData,
          (sizeof (USBC_CONNECTOR_BOARD_CONFIG) * ConnectorCount) +
          sizeof (UsbCConnectorHobData.NumberOfUsbCConnectors) +
          sizeof (UsbCConnectorHobData.NumberOfConnectableUsbCConnectors) +
          sizeof (UsbCConnectorHobData.ConnectableUsbCConnectorsMap)
          );
  ASSERT (Hob != NULL);
  UpdateNumOfConnectableUsbCConnectorsAndMap ();

  return EFI_SUCCESS;
}

/**
  Get USBC Connector HOB Data Pointer

  @retval USBC_CONNECTOR_HOB_DATA *    USBC Connector HOB Data Pointer
**/
USBC_CONNECTOR_HOB_DATA *
EFIAPI
GetUsbCConnectorHobData (
  VOID
  )
{
  VOID  *Hob;

  Hob = GetFirstGuidHob (&gUsbCConnectorHobGuid);
  if (Hob == NULL) {
    DEBUG((DEBUG_ERROR, "%a: UsbCConnectorHob Not Found!!\n", __FUNCTION__));
    return NULL;
  }

  return (USBC_CONNECTOR_HOB_DATA *) GET_GUID_HOB_DATA (Hob);
}

/**
  Get number of connectable USBC connectors on board.

  @retval UINT8                        Number of activated USBC connectors on board
**/
UINT8
EFIAPI
GetNumOfConnectableUsbCConnectors (
  VOID
  )
{
  USBC_CONNECTOR_HOB_DATA *UsbCConnectorHobDataPtr;

  UsbCConnectorHobDataPtr = GetUsbCConnectorHobData ();

  if (UsbCConnectorHobDataPtr != NULL) {
    return UsbCConnectorHobDataPtr->NumberOfConnectableUsbCConnectors;
  }

  return 0;
}

/**
  Get connectable USBC connectors map on board.

  @retval UINT16                       Activated USBC connectors map on board
**/
UINT16
EFIAPI
GetConnectableUsbCConnectorsMap (
  VOID
  )
{
  USBC_CONNECTOR_HOB_DATA *UsbCConnectorHobDataPtr;

  UsbCConnectorHobDataPtr = GetUsbCConnectorHobData ();

  if (UsbCConnectorHobDataPtr != NULL) {
    return UsbCConnectorHobDataPtr->ConnectableUsbCConnectorsMap;
  }

  return 0;
}

/**
  Update number of connectable USBC connectors on board.

  @retval EFI_SUCCESS                  Number of activated USBC connectors is set successfully.
  @retval EFI_NOT_FOUND                UsbCConnectorHobDataPtr is not available.
**/
EFI_STATUS
EFIAPI
UpdateNumOfConnectableUsbCConnectorsAndMap (
  VOID
  )
{
  UINT8                   ConnectorIndex;
  UINT8                   NumOfConnectableUsbCConnectors;
  USB_CONNECTOR_HOB_DATA  *UsbConnectorHobDataPtr;
  USBC_CONNECTOR_HOB_DATA *UsbCConnectorHobDataPtr;

  NumOfConnectableUsbCConnectors = 0;
  UsbConnectorHobDataPtr  = GetUsbConnectorHobData ();
  UsbCConnectorHobDataPtr = GetUsbCConnectorHobData ();

  if (UsbConnectorHobDataPtr == NULL || UsbCConnectorHobDataPtr == NULL) {
    return EFI_NOT_FOUND;
  }

  UsbCConnectorHobDataPtr->ConnectableUsbCConnectorsMap = 0;
  for (ConnectorIndex = 0; ConnectorIndex < UsbCConnectorHobDataPtr->NumberOfUsbCConnectors; ConnectorIndex++) {
    if (UsbConnectorHobDataPtr->UsbConnectorBoardConfig[ConnectorIndex].ConnectorConnectable == CONNECTABLE &&
      UsbConnectorHobDataPtr->UsbConnectorBoardConfig[ConnectorIndex].ConnectorType >= USB_TYPE_C_USB2_ONLY &&
      UsbConnectorHobDataPtr->UsbConnectorBoardConfig[ConnectorIndex].ConnectorType <= USB_TYPE_C_WITHOUT_SWITCH) {
      NumOfConnectableUsbCConnectors++;
      UsbCConnectorHobDataPtr->ConnectableUsbCConnectorsMap |= (UINT16)(1 << ConnectorIndex);
    }
  }
  UsbCConnectorHobDataPtr->NumberOfConnectableUsbCConnectors = NumOfConnectableUsbCConnectors;
  return EFI_SUCCESS;
}
