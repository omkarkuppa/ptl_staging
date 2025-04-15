/** @file
  Internal file to access Foxville NVM.

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
//
// Self-Module Foundation.
//
#include "Misc/FoxvilleNvmAccess.h"
#include "Misc/FoxvilleRegAccess.h"

/**
  Read the value from assigned address of the word via EERD register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]   Address           Address of the word in read.
  @param[out]  DataPtr           Pointer to return the value read from.

  @retval  EFI_SUCCESS            Succeed to read the value from NVM.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  EFI_TIMEOUT            Operation is time out.
  @retval  Others                 Failed to read the value from NVM.

**/
EFI_STATUS
EFIAPI
ReadNvmEerdUint16 (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN     UINT16               Address,
     OUT UINT16               *DataPtr
  )
{
  EFI_STATUS  Status;

  if ((PciIoProtocolPtr == NULL) || (DataPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = WriteEerd (PciIoProtocolPtr, Address);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Status = PollEerdReadDone (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    return EFI_TIMEOUT;
  }

  Status = ReadEerd (PciIoProtocolPtr, DataPtr);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Write the value to assigned address of the word via EEWR register.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Address           Address of the word in write.
  @param[in]  Data              Data used to write into assigned address.

  @retval  EFI_SUCCESS            Succeed to write the value to NVM.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  EFI_TIMEOUT            Operation is time out.
  @retval  Others                 Failed to write the value to NVM.

**/
EFI_STATUS
EFIAPI
WriteNvmEewrUint16 (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT16               Address,
  IN UINT16               Data
  )
{
  EFI_STATUS  Status;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = WriteEewr (PciIoProtocolPtr, Address, Data);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Status = PollEewrWriteDone (PciIoProtocolPtr);
  if (EFI_ERROR (Status)) {
    return EFI_TIMEOUT;
  }

  return EFI_SUCCESS;
}
