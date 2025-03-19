/** @file
  Internal file to handle Foxville semaphore.

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

#ifndef __FOXVILLE_SEMAPHORE_H__
#define __FOXVILLE_SEMAPHORE_H__

//
// EDK-II Foundation.
//
#include <Uefi.h>

#define FOXVILLE_SW_SEMAPHORE_ACQUIRE_TIMEOUT              50
#define FOXVILLE_SW_SEMAPHORE_ACQUIRE_MAX_ATTEMPT          2000
#define FOXVILLE_FW_SEMAPHORE_ACQUIRE_TIMEOUT              50
#define FOXVILLE_FW_SEMAPHORE_ACQUIRE_MAX_ATTEMPT          2000
#define FOXVILLE_NVM_ACCESS_SEMAPHORE_ACQUIRE_TIMEOUT      5000
#define FOXVILLE_NVM_ACCESS_SEMAPHORE_ACQUIRE_MAX_ATTEMPT  200
#define FOXVILLE_NVM_ACCESS_SEMAPHORE_RELEASE_MAX_ATTEMPT  (300 * 1000 * 1000)

/**
  Acquire the software semaphore.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to acquire SW semaphore.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  EFI_TIMEOUT            Operation is time out.
  @retval  Others                 Failed to acquire SW semaphore.

**/
EFI_STATUS
EFIAPI
AcquireSwSemaphore (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  );

/**
  Release the software semaphore.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to release SW semaphore.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to release SW semaphore.

**/
EFI_STATUS
EFIAPI
ReleaseSwSemaphore (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  );

/**
  Acquire the firmware semaphore.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to acquire FW semaphore.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  EFI_TIMEOUT            Operation is time out.
  @retval  Others                 Failed to acquire FW semaphore.

**/
EFI_STATUS
EFIAPI
AcquireFwSemaphore (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  );

/**
  Acquire the hardware semaphore.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to acquire HW semaphore.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to acquire HW semaphore.

**/
EFI_STATUS
EFIAPI
AcquireHwSemaphore (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  );

/**
  Release the hardware semaphore.

  @note If failed to release the semaphore, the device would fail in the OS.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to release HW semaphore.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to release HW semaphore.

**/
EFI_STATUS
EFIAPI
ReleaseHwSemaphore (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  );

/**
  Acquire the NVM access semaphore.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to acquire NVM access semaphore.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  EFI_TIMEOUT            Operation is time out.
  @retval  Others                 Failed to acquire NVM access semaphore.

**/
EFI_STATUS
EFIAPI
AcquireNvmAccessSemaphore (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  );

/**
  Release the NVM access semaphore.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to release NVM access semaphore.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  EFI_TIMEOUT            Operation is time out.
  @retval  Others                 Failed to release NVM access semaphore.

**/
EFI_STATUS
EFIAPI
ReleaseNvmAccessSemaphore (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  );

#endif
