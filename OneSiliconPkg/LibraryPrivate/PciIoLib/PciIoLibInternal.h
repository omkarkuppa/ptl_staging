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

#include <Base.h>
#include <Uefi.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/PciHostBridgeResourceAllocation.h>
#include <Ppi/PciDevice.h>
#include <Library/PciHostBridgeLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciExpressHelpersLib.h>
#include <Library/PcieHelperLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>
#include <Library/DevicePathLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseLib.h>
#include <Ppi/IoMmu.h>
#include <IndustryStandard/Acpi.h>
#include <Library/PciIoLib.h>

#ifndef _PCI_IO_LIB_INTERNAL_H_
#define _PCI_IO_LIB_INTERNAL_H_

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
  );

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
  );

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
  );

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
  );


/**
  Reads from the memory space of a PCI controller. Returns either when the polling exit criteria is
  satisfied or after a defined duration.

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Width                 Signifies the width of the memory or I/O operations.
  @param  BarIndex              The BAR index of the standard PCI Configuration header to use as the
                                base address for the memory operation to perform.
  @param  Offset                The offset within the selected BAR to start the memory operation.
  @param  Mask                  Mask used for the polling criteria.
  @param  Value                 The comparison value used for the polling exit criteria.
  @param  Delay                 The number of 100 ns units to poll.
  @param  Result                Pointer to the last value read from the memory location.
  @retval EFI_SUCCESS           The last data returned from the access matched the poll exit criteria.
  @retval EFI_UNSUPPORTED       BarIndex not valid for this PCI controller.
  @retval EFI_UNSUPPORTED       Offset is not valid for the BarIndex of this PCI controller.
  @retval EFI_TIMEOUT           Delay expired before a match occurred.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
**/
EFI_STATUS
EFIAPI
PciBusPollMem (
  IN EFI_PCI_IO_PROTOCOL           *This,
  IN  EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN  UINT8                        BarIndex,
  IN  UINT64                       Offset,
  IN  UINT64                       Mask,
  IN  UINT64                       Value,
  IN  UINT64                       Delay,
  OUT UINT64                       *Result
  );

  /**
  Reads from the I/O space of a PCI controller. Returns either when the polling exit criteria is
  satisfied or after a defined duration.

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Width                 Signifies the width of the memory or I/O operations.
  @param  BarIndex              The BAR index of the standard PCI Configuration header to use as the
                                base address for the memory operation to perform.
  @param  Offset                The offset within the selected BAR to start the memory operation.
  @param  Mask                  Mask used for the polling criteria.
  @param  Value                 The comparison value used for the polling exit criteria.
  @param  Delay                 The number of 100 ns units to poll.
  @param  Result                Pointer to the last value read from the memory location.
  @retval EFI_SUCCESS           The last data returned from the access matched the poll exit criteria.
  @retval EFI_UNSUPPORTED       BarIndex not valid for this PCI controller.
  @retval EFI_UNSUPPORTED       Offset is not valid for the BarIndex of this PCI controller.
  @retval EFI_TIMEOUT           Delay expired before a match occurred.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
**/
EFI_STATUS
EFIAPI
PciBusPollIo (
  IN EFI_PCI_IO_PROTOCOL           *This,
  IN  EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN  UINT8                        BarIndex,
  IN  UINT64                       Offset,
  IN  UINT64                       Mask,
  IN  UINT64                       Value,
  IN  UINT64                       Delay,
  OUT UINT64                       *Result
  );

/**
  Enables a PCI driver to copy one region of PCI memory space to another region of PCI
  memory space.

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @param  Width                 Signifies the width of the memory operations.
  @param  DestBarIndex          The BAR index in the standard PCI Configuration header to use as the
                                base address for the memory operation to perform.
  @param  DestOffset            The destination offset within the BAR specified by DestBarIndex to
                                start the memory writes for the copy operation.
  @param  SrcBarIndex           The BAR index in the standard PCI Configuration header to use as the
                                base address for the memory operation to perform.
  @param  SrcOffset             The source offset within the BAR specified by SrcBarIndex to start
                                the memory reads for the copy operation.
  @param  Count                 The number of memory operations to perform. Bytes moved is Width
                                size * Count, starting at DestOffset and SrcOffset.
  @retval EFI_SUCCESS           The data was copied from one memory region to another memory region.
  @retval EFI_UNSUPPORTED       DestBarIndex not valid for this PCI controller.
  @retval EFI_UNSUPPORTED       SrcBarIndex not valid for this PCI controller.
  @retval EFI_UNSUPPORTED       The address range specified by DestOffset, Width, and Count
                                is not valid for the PCI BAR specified by DestBarIndex.
  @retval EFI_UNSUPPORTED       The address range specified by SrcOffset, Width, and Count is
                                not valid for the PCI BAR specified by SrcBarIndex.
  @retval EFI_INVALID_PARAMETER Width is invalid.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
**/
EFI_STATUS
EFIAPI
PciBusCopyMem (
  IN EFI_PCI_IO_PROTOCOL              *This,
  IN     EFI_PCI_IO_PROTOCOL_WIDTH    Width,
  IN     UINT8                        DestBarIndex,
  IN     UINT64                       DestOffset,
  IN     UINT8                        SrcBarIndex,
  IN     UINT64                       SrcOffset,
  IN     UINTN                        Count
  );

/**
  Flushes all PCI posted write transactions from a PCI host bridge to system memory.

  @param  This                  A pointer to the EFI_PCI_IO_PROTOCOL instance.
  @retval EFI_SUCCESS           The PCI posted write transactions were flushed from the PCI host
                                bridge to system memory.
  @retval EFI_DEVICE_ERROR      The PCI posted write transactions were not flushed from the PCI
                                host bridge due to a hardware error.
**/
EFI_STATUS
EFIAPI
PciBusFlush (
  IN EFI_PCI_IO_PROTOCOL  *This
  );



#endif
