/** @file
  Internal file to support Foxville device.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#ifndef __FOXVILLE_DEVICE_H__
#define __FOXVILLE_DEVICE_H__

//
// EDK-II Foundation.
//
#include <Uefi.h>

/**
  Check if the device handle is supported.

  @param[in]  DeviceHandlePtr  Pointer to the device handle.

  @retval  FALSE  Device handle is not supported.
  @retval  TRUE   Device handle is supported.

**/
BOOLEAN
EFIAPI
IsDeviceHandleSupported (
  IN EFI_HANDLE  *DeviceHandlePtr
  );

/**
  Get the supported Foxville device handle.

  @param[out]  DeviceHandlePtr  Pointer to the device handle.

  @retval  EFI_SUCCESS            Succeed to get the supported Foxville device handle.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_NOT_FOUND          Not found the supported Foxville device handle.
  @retval  Others                 Failed to get the supported Foxville device handle.

**/
EFI_STATUS
EFIAPI
GetFoxvilleDeviceHandle (
  OUT EFI_HANDLE  *DeviceHandlePtr
  );

/**
  Check if the device ID is blank flash.

  @note If the device owned the blank flash, there would not have valid NVM.
        The device ID would be changed after the update.

  @param[in]  DeviceId  The device ID to be evaluated.

  @retval  FALSE  The device ID is not indicated as blank flash.
  @retval  TRUE   The device ID is indicated as blank flash.

**/
BOOLEAN
EFIAPI
IsBlankFlashDeviceId (
  IN UINT16  DeviceId
  );

/**
  Set the required Foxville PCI attributes.

  @note This function would set the attributes below,
          (1) Enable device MMIO.
          (2) Enable device port IO.
          (3) Enable device bus mastering.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to set the PCI attributes.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to set the PCI attributes.

**/
EFI_STATUS
EFIAPI
SetFoxvillePciAttributes (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  );

/**
  Initial the Foxville device information.

  @param[in]   PciIoProtocolPtr       Pointer to the PCI IO protocol instance.
  @param[out]  FoxvilleDeviceInfoPtr  Pointer to the Foxville device information.

  @retval  EFI_SUCCESS            Succeed to initialize the Foxville device information.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to initialize the Foxville device information.

**/
EFI_STATUS
EFIAPI
InitFoxvilleDeviceInfo (
  IN     EFI_PCI_IO_PROTOCOL   *PciIoProtocolPtr,
     OUT FOXVILLE_DEVICE_INFO  *FoxvilleDeviceInfoPtr
  );

#endif
