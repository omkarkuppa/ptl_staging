/** @file
  Header file for PlatformUsbConfigLib

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

@par Specification
**/

#ifndef _PLATFORM_USB_CONFIG_LIB_H_
#define _PLATFORM_USB_CONFIG_LIB_H_

#include <PlatformBoardConfig.h>
#include <Ucsi.h>

/**
  Build USB Connector HOB

  @retval EFI_SUCCESS                  USB Connector HOD is built successfully.
  @retval EFI_NOT_FOUND                VpdPcdUsbConnector is not available.
  @retval EFI_INVALID_PARAMETER        ConnectorCount is over the max supported number by platform.
**/
EFI_STATUS
EFIAPI
BuildUsbConnectorHob (
  VOID
  );

/**
  Get USB Connector HOB Data Pointer

  @retval USB_CONNECTOR_HOB_DATA *    USB Connector HOB Data Pointer
**/
USB_CONNECTOR_HOB_DATA *
EFIAPI
GetUsbConnectorHobData (
  VOID
  );

/**
  Build USBC Connector HOB

  @retval EFI_SUCCESS                  USBC Connector HOD is built successfully.
  @retval EFI_NOT_FOUND                VpdPcdUsbCConnector is not available.
  @retval EFI_INVALID_PARAMETER        ConnectorCount is over the max supported number by platform.
**/
EFI_STATUS
EFIAPI
BuildUsbCConnectorHob (
  VOID
  );

/**
  Get USBC Connector HOB Data Pointer

  @retval USBC_CONNECTOR_HOB_DATA *    USBC Connector HOB Data Pointer
**/
USBC_CONNECTOR_HOB_DATA *
EFIAPI
GetUsbCConnectorHobData (
  VOID
  );

/**
  Get number of connectable USBC connectors on board.

  @retval UINT8                        Number of activated USBC connectors on board
**/
UINT8
EFIAPI
GetNumOfConnectableUsbCConnectors (
  VOID
  );

/**
  Get connectable USBC connectors map on board.

  @retval UINT16                       Activated USBC connectors map on board
**/
UINT16
EFIAPI
GetConnectableUsbCConnectorsMap (
  VOID
  );

/**
  Update number of connectable USBC connectors and map on board.

  @retval EFI_SUCCESS                  Number of activated USBC connectors is set successfully.
  @retval EFI_NOT_FOUND                UsbCConnectorHobDataPtr is not available.
**/
EFI_STATUS
EFIAPI
UpdateNumOfConnectableUsbCConnectorsAndMap (
  VOID
  );

#endif // _PLATFORM_USB_CONFIG_LIB_H_
