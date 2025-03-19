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

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Protocol/PciIo.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
//
// Self-Module Foundation.
//
#include "Misc/FoxvilleRegAccess.h"
#include "Misc/FoxvilleSemaphore.h"

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
  )
{
  EFI_STATUS  Status;
  UINTN       Count;
  BOOLEAN     IsSet;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (Count = 0; Count < FOXVILLE_SW_SEMAPHORE_ACQUIRE_MAX_ATTEMPT; Count++) {
    Status = GetSwsmSmbi (PciIoProtocolPtr, &IsSet);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    if (IsSet) {
      return EFI_SUCCESS;
    }

    gBS->Stall (FOXVILLE_SW_SEMAPHORE_ACQUIRE_TIMEOUT);
    continue;
  }

  return EFI_TIMEOUT;
}

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
  )
{
  EFI_STATUS  Status;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ClearSwsmSmbi (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS  Status;
  UINTN       Count;
  BOOLEAN     IsSet;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (Count = 0; Count < FOXVILLE_FW_SEMAPHORE_ACQUIRE_MAX_ATTEMPT; Count++) {
    Status = SetSwsmSweSmbi (PciIoProtocolPtr);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    Status = GetSwsmSweSmbi (PciIoProtocolPtr, &IsSet);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    if (IsSet) {
      return EFI_SUCCESS;
    }

    gBS->Stall (FOXVILLE_FW_SEMAPHORE_ACQUIRE_TIMEOUT);
    continue;
  }

  return EFI_TIMEOUT;
}

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
  )
{
  EFI_STATUS  Status;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AcquireSwSemaphore (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get SW semaphore - %r", Status));
    return EFI_DEVICE_ERROR;
  }

  Status = AcquireFwSemaphore (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get FW semaphore - %r", Status));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS  Status;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReleaseSwSemaphore (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to clear SW semaphore - %r", Status));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Internal function to set the NVM access semaphore.

  @note In this function, a succeed attempt would be returned as EFI_SUCCESS.
        Caller needs to check the IsSetPtr to see if semaphore is set or not.
        Since others SW thread might take the ownership already.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  IsSetPtr          Pointer to return the semaphore is set or not.

  @retval  EFI_SUCCESS            Succeed to attempt set NVM access semaphore.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to attempt set NVM access semaphore.

**/
EFI_STATUS
SetNvmAccessSemaphore (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT BOOLEAN              *IsSetPtr
  )
{
  EFI_STATUS  Status;
  EFI_STATUS  ReturnStatus;
  BOOLEAN     IsAvailable;

  ReturnStatus = EFI_SUCCESS;
  IsAvailable  = FALSE;

  if ((PciIoProtocolPtr == NULL) || (IsSetPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *IsSetPtr = FALSE;

  Status = AcquireHwSemaphore (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to acquire HW semaphore.\n"));
    return EFI_DEVICE_ERROR;
  }

  Status = CheckSwFwSyncNvmResource (PciIoProtocolPtr, &IsAvailable);
  if (EFI_ERROR (Status)) {
    ReturnStatus = Status;
    DEBUG ((DEBUG_ERROR, "Failed to check the NVM access resource status.\n"));
    goto Exit;
  }

  if (!IsAvailable) {
    ReturnStatus = EFI_SUCCESS;
    DEBUG ((DEBUG_INFO, "NVM access resource is busy.\n"));
    goto Exit;
  }

  Status = SetSwFwSyncSwFlashSm (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    ReturnStatus = Status;
    DEBUG ((DEBUG_ERROR, "Failed to acquire the NVM access resource.\n"));
    goto Exit;
  }

  *IsSetPtr = TRUE;

Exit:
  Status = ReleaseHwSemaphore (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to release HW semaphore.\n"));
    return EFI_DEVICE_ERROR;
  }

  return ReturnStatus;
}

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
  )
{
  EFI_STATUS  Status;
  UINTN       Count;
  BOOLEAN     IsAcquired;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (Count = 0; Count < FOXVILLE_NVM_ACCESS_SEMAPHORE_ACQUIRE_MAX_ATTEMPT; Count++) {
    Status = SetNvmAccessSemaphore (PciIoProtocolPtr, &IsAcquired);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    if (IsAcquired) {
      return EFI_SUCCESS;
    }

    gBS->Stall (FOXVILLE_NVM_ACCESS_SEMAPHORE_ACQUIRE_TIMEOUT);
  }

  return EFI_TIMEOUT;
}

/**
  Internal function to clear the NVM access semaphore.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to clear NVM access semaphore.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_NOT_READY          Failed to acquire HW semaphore.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to clear NVM access semaphore.

**/
EFI_STATUS
ClearNvmAccessSemaphore (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  )
{
  EFI_STATUS  Status;
  EFI_STATUS  ReturnStatus;

  ReturnStatus = EFI_SUCCESS;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AcquireHwSemaphore (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Not ready to acquire HW semaphore.\n"));
    return EFI_NOT_READY;
  }

  Status = ClearSwFwSyncSwFlashSm (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    ReturnStatus = Status;
    DEBUG ((DEBUG_ERROR, "Failed to release NVM access resource.\n"));
    goto Exit;
  }

Exit:
  Status = ReleaseHwSemaphore (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to release HW semaphore.\n"));
    return EFI_DEVICE_ERROR;
  }

  return ReturnStatus;
}

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
  )
{
  EFI_STATUS  Status;
  UINTN       Count;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (Count = 0; Count < FOXVILLE_NVM_ACCESS_SEMAPHORE_RELEASE_MAX_ATTEMPT; Count++) {
    Status = ClearNvmAccessSemaphore (PciIoProtocolPtr);
    if (!EFI_ERROR (Status)) {
      return EFI_SUCCESS;
    } else if (Status != EFI_NOT_READY) {
      return EFI_DEVICE_ERROR;
    } else {
      continue;
    }
  }

  return EFI_TIMEOUT;
}
