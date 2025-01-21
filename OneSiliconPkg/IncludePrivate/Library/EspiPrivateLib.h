/** @file
  Header file for PchEspiLib.
  All function in this library is available for PEI, DXE, and SMM,

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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

#ifndef _ESPI_LIB_H_
#define _ESPI_LIB_H_

#include <RegisterAccess.h>

//
// Enum for eSPI device ids
//
typedef enum {
  EspiDeviceCS0 = 0,
  EspiDeviceCS1,
  EspiDeviceCS2,
  EspiDeviceCS3,
  EspiDeviceMax
} ESPI_DEVICE_ID;

/**
  Checks if given eSPI device is supported

  @param[in]  DeviceId        Id of device to check
  @param[in]  EspiPcrAccess   eSPI Sideband registers access protocol

  @retval TRUE      There's second device
  @retval FALSE     There's no second device
**/
BOOLEAN
IsEspiDeviceSupported (
  IN ESPI_DEVICE_ID  DeviceId,
  IN REGISTER_ACCESS *EspiPcrAccess
  );

/**
  Checks in device General Capabilities register if it supports channel with requested number

  @param[in]  DeviceId        Id of device to check
  @param[in]  ChannelNumber   Number of channel of which to check
  @param[in]  EspiPcrAccess   eSPI Sideband registers access protocol

  @retval TRUE      Channel with requested number is supported by device
  @retval FALSE     Channel with requested number is not supported by device
**/
BOOLEAN
IsEspiDeviceChannelSupported (
  IN  ESPI_DEVICE_ID  DeviceId,
  IN  UINT8           ChannelNumber,
  IN  REGISTER_ACCESS *EspiPcrAccess
  );

/**
  Is eSPI enabled in strap.

  @param[in] EspiPcrAccess        eSPI Sideband registers access protocol

  @retval TRUE          Espi is enabled in strap
  @retval FALSE         Espi is disabled in strap
**/
BOOLEAN
IsEspiEnabled (
  IN REGISTER_ACCESS *EspiPcrAccess
  );

/**
  Get configuration from eSPI device

  @param[in]  DeviceId       eSPI device ID
  @param[in]  DeviceAddress  Device Configuration Register Address
  @param[in]  EspiPcrAccess  eSPI Sideband registers access protocol
  @param[out] OutData        Configuration data read

  @retval EFI_SUCCESS           Operation succeed
  @retval EFI_INVALID_PARAMETER device ID is not supported
  @retval EFI_INVALID_PARAMETER device ID is not supported or DeviceId 1 is used in PCH_LP
  @retval EFI_INVALID_PARAMETER device configuration register address exceed maximum allowed
  @retval EFI_INVALID_PARAMETER device configuration register address is not DWord aligned
  @retval EFI_DEVICE_ERROR      Error in SCRS during polling stage of operation
**/
EFI_STATUS
PchEspiDeviceGetConfig (
  IN  ESPI_DEVICE_ID  DeviceId,
  IN  UINT32          DeviceAddress,
  IN  REGISTER_ACCESS *EspiPcrAccess,
  OUT UINT32          *OutData
  );

/**
  Set eSPI device configuration

  Note: A Set_Configuration must always be followed by a Get_Configuration in order to ensure
  that the internal state of the eSPI-MC is consistent with the device register settings.

  @param[in]  DeviceId       eSPI device ID
  @param[in]  DeviceAddress  device Configuration Register Address
  @param[in]  EspiPcrAccess  eSPI Sideband registers access protocol
  @param[in]  InData         Configuration data to write

  @retval EFI_SUCCESS           Operation succeed
  @retval EFI_INVALID_PARAMETER device ID is not supported or DeviceId 1 is used in PCH_LP
  @retval EFI_INVALID_PARAMETER device configuration register address exceed maximum allowed
  @retval EFI_INVALID_PARAMETER device configuration register address is not DWord aligned
  @retval EFI_ACCESS_DENIED     eSPI device write to address range 0 to 0x7FF has been locked
  @retval EFI_DEVICE_ERROR      Error in SCRS during polling stage of operation
**/
EFI_STATUS
PchEspiDeviceSetConfig (
  IN  ESPI_DEVICE_ID  DeviceId,
  IN  UINT32          DeviceAddress,
  IN  REGISTER_ACCESS *EspiPcrAccess,
  IN  UINT32          InData
  );

/**
  Get status from eSPI device

  @param[in]  DeviceId       eSPI device ID
  @param[in]  EspiPcrAccess  eSPI Sideband registers access protocol
  @param[out] OutData        Configuration data read

  @retval EFI_SUCCESS           Operation succeed
  @retval EFI_INVALID_PARAMETER device ID is not supported or DeviceId 1 is used in PCH_LP
  @retval EFI_DEVICE_ERROR      Error in SCRS during polling stage of operation
**/
EFI_STATUS
PchEspiDeviceGetStatus (
  IN  ESPI_DEVICE_ID  DeviceId,
  IN  REGISTER_ACCESS *EspiPcrAccess,
  OUT UINT16          *OutData
  );

/**
  eSPI device in-band reset

  @param[in]  DeviceId       eSPI device ID
  @param[in]  EspiPcrAccess  eSPI Sideband registers access protocol

  @retval EFI_SUCCESS           Operation succeed
  @retval EFI_INVALID_PARAMETER device ID is not supported or DeviceId 1 is used in PCH_LP
  @retval EFI_DEVICE_ERROR      Error in SCRS during polling stage of operation
**/
EFI_STATUS
PchEspiDeviceInBandReset (
  IN  ESPI_DEVICE_ID  DeviceId,
  IN  REGISTER_ACCESS *EspiPcrAccess
  );

/**
  eSPI device channel reset helper function

  @param[in]  DeviceId          eSPI device ID
  @param[in]  ChannelNumber     Number of channel to reset
  @param[in]  EspiPcrAccess     eSPI Sideband registers access protocol

  @retval     EFI_SUCCESS       Operation succeeded
  @retval     EFI_UNSUPPORTED   device doesn't support that channel or invalid number specified
  @retval     EFI_TIMEOUT       Operation has timeouted
**/
EFI_STATUS
PchEspiDeviceChannelReset (
  IN  ESPI_DEVICE_ID  DeviceId,
  IN  UINT8           ChannelNumber,
  IN  REGISTER_ACCESS *EspiPcrAccess
  );

/**
  Disable EISS (Enable InSMM.STS)
**/
VOID
EspiDisableEiss (
  VOID
  );

/**
  Returns whether eSPI peripheral channel is available and ready on device $0

  @param[in]  EspiPcrAccess     eSPI Sideband registers access protocol

  @retval     TRUE              eSPI Peripheral channel for device #0 is
                                enabled and ready
  @retval     FALSE             eSPI Peripheral channel for device #0 is either
                                enabled or not ready
**/
BOOLEAN
IsEspiCs0PeripheralChannelAvailable(
  IN  REGISTER_ACCESS   *EspiPcrAccess
  );

#endif // _ESPI_LIB_H_
