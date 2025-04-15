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

#ifndef __FOXVILLE_NVM_ACCESS_H__
#define __FOXVILLE_NVM_ACCESS_H__

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Protocol/PciIo.h>

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
  );

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
  );

#endif
