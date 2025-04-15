/** @file
  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include "PciIoLibInternal.h"

/**
  Provides the PCI controller-specific addresses needed to access system memory.
  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Operation             Indicates if the bus master is going to read or write to system memory.
  @param  HostAddress           The system memory address to map to the PCI controller.
  @param  NumberOfBytes         On input the number of bytes to map. On output the number of bytes
                                that were mapped.
  @param  DeviceAddress         The resulting map address for the bus master PCI controller to use to
                                access the hosts HostAddress.
  @param  Mapping               A resulting value to pass to Unmap().
  @retval EFI_SUCCESS           The range was mapped for the returned NumberOfBytes.
  @retval EFI_UNSUPPORTED       The HostAddress cannot be mapped as a common buffer.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_DEVICE_ERROR      The system hardware could not map the requested address.
**/
EFI_STATUS
EFIAPI
PciBusMap (
  IN EFI_PCI_IO_PROTOCOL                *This,
  IN     EFI_PCI_IO_PROTOCOL_OPERATION  Operation,
  IN     VOID                           *HostAddress,
  IN OUT UINTN                          *NumberOfBytes,
  OUT    EFI_PHYSICAL_ADDRESS           *DeviceAddress,
  OUT    VOID                           **Mapping
  )
{

  if (HostAddress == NULL || NumberOfBytes == NULL || Mapping == NULL || DeviceAddress == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Operation == -1 || Operation >= EfiPciIoOperationMaximum) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_UNSUPPORTED;
}

/**
  Completes the Map() operation and releases any corresponding resources.
  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Mapping               The mapping value returned from Map().
  @retval EFI_SUCCESS           The range was unmapped.
  @retval EFI_DEVICE_ERROR      The data was not committed to the target system memory.
**/
EFI_STATUS
EFIAPI
PciBusUnmap (
  IN EFI_PCI_IO_PROTOCOL           *This,
  IN  VOID                         *Mapping
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Allocates pages that are suitable for an EfiPciIoOperationBusMasterCommonBuffer
  or EfiPciOperationBusMasterCommonBuffer64 mapping.
  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Type                  This parameter is not used and must be ignored.
  @param  MemoryType            The type of memory to allocate, EfiBootServicesData or
                                EfiRuntimeServicesData.
  @param  Pages                 The number of pages to allocate.
  @param  HostAddress           A pointer to store the base system memory address of the
                                allocated range.
  @param  Attributes            The requested bit mask of attributes for the allocated range.
  @retval EFI_SUCCESS           The requested memory pages were allocated.
  @retval EFI_UNSUPPORTED       Attributes is unsupported. The only legal attribute bits are
                                MEMORY_WRITE_COMBINE, MEMORY_CACHED and DUAL_ADDRESS_CYCLE.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The memory pages could not be allocated.
**/
EFI_STATUS
EFIAPI
PciBusAllocateBuffer (
  IN EFI_PCI_IO_PROTOCOL           *This,
  IN  EFI_ALLOCATE_TYPE            Type,
  IN  EFI_MEMORY_TYPE              MemoryType,
  IN  UINTN                        Pages,
  OUT VOID                         **HostAddress,
  IN  UINT64                       Attributes
  )
{

  if ((Attributes & (~(EFI_PCI_ATTRIBUTE_MEMORY_WRITE_COMBINE | EFI_PCI_ATTRIBUTE_MEMORY_CACHED))) != 0) {
    return EFI_UNSUPPORTED;
  }

  return EFI_INVALID_PARAMETER;
}

/**
  Frees memory that was allocated with AllocateBuffer().
  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Pages                 The number of pages to free.
  @param  HostAddress           The base system memory address of the allocated range.
  @retval EFI_SUCCESS           The requested memory pages were freed.
  @retval EFI_INVALID_PARAMETER The memory range specified by HostAddress and Pages
                                was not allocated with AllocateBuffer().
**/
EFI_STATUS
EFIAPI
PciBusFreeBuffer (
  IN EFI_PCI_IO_PROTOCOL           *This,
  IN  UINTN                        Pages,
  IN  VOID                         *HostAddress
  )
{
  return EFI_UNSUPPORTED;
}