/** @file
  USB4 host router service libary header.

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

#ifndef _USB4_HR_SRV_LIB_H_
#define _USB4_HR_SRV_LIB_H_

#include <Uefi.h>
#include <Protocol/PciIo.h>
#include <Library/Usb4Common.h>
#include <Library/Usb4HrPci.h>
#include <Library/Usb4HiCoreDefs.h>

/**
  Allocates pages that are suitable for NHI DMA access.

  @param[in]  HrContext     - USB4 host router context for DMA buffer allocation.
  @param[in]  BufPages      - The buffer size to be allocated.
  @param[out] HostAddress   - Pointer of pointer to the system memory address of the allocated DMA buffer.
  @param[out] DeviceAddress - Pointer to the resulting map address for the bus master PCI controller.
  @param[out] Mapping       - A resulting value to pass to Unmap().

  @retval EFI_SUCCESS           - The requested memory pages were allocated.
  @retval EFI_OUT_OF_RESOURCES  - Buffer allocation failure.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
HrSrvDmaBufAlloc (
  IN  USB4_HR_CONTEXT     HrContext,
  IN  UINT32              BufPages,
  OUT VOID                **HostAddress,
  OUT PHYSICAL_ADDRESS    *DeviceAddress,
  OUT VOID                **Mapping
  );

/**
  Free the DMA buffer that is allocated by HrSrvDmaBufAlloc.

  @param[in] HrContext   - USB4 host router context.
  @param[in] HostAddress - The system memory address of DMA buffer to free.
  @param[in] BufPages    - The number of pages to free.
  @param[in] Mapping     - The mapping value returned from Map().
**/
VOID
HrSrvDmaBufFree (
  IN USB4_HR_CONTEXT     HrContext,
  IN VOID                *HostAddress,
  IN UINT32              BufPages,
  IN VOID                *Mapping
  );

/**
  Read MMIO data from Host Router NHI device via BAR0.

  @param[in]  HrContext - Host router context for MMIO read.
  @param[in]  Offset    - The offset to start the memory operation.
  @param[in]  Count     - The number in DWORDs of memory operations to perform.
  @param[out] Buffer    - Data buffer for MMIO read.

  @retval EFI_SUCCESS           - MMIO data read success.
  @retval EFI_NOT_READY         - MMIO space is not assigned to NHI device.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
HrSrvMmioRead (
  IN  USB4_HR_CONTEXT    HrContext,
  IN  UINT32             Offset,
  IN  UINT32             Count,
  OUT VOID               *Buffer
  );

/**
  Write MMIO data to Host Router NHI device via BAR0.

  @param[in]  HrContext - Host router context for MMIO write.
  @param[in]  Offset    - The offset to start the memory operation.
  @param[in]  Count     - The number in DWORDs of memory operations to perform.
  @param[in]  Buffer    - Data buffer for MMIO write.

  @retval EFI_SUCCESS           - MMIO data write success.
  @retval EFI_NOT_READY         - MMIO space is not assigned to NHI device.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
HrSrvMmioWrite (
  IN  USB4_HR_CONTEXT    HrContext,
  IN  UINT32             Offset,
  IN  UINT32             Count,
  IN  VOID               *Buffer
  );

/**
  Dump PCI registers of the given device address

  @param[in] DevAddr - PCI Device Address
**/
VOID
HrSrvDumpPciDevRegs (
  IN UINT64 DevAddr
  );

/**
  Dump MMIO data of the specified MMIO base

  @param[in] MmioBase - Base address of MMIO to be dumped
  @param[in] DwCount  - Count of DWORDs to be dumped
**/
VOID
HrSrvMmioDump (
  IN PHYSICAL_ADDRESS MmioBase,
  IN UINT32           DwCount
  );

/**
  Initialize PCI bridge device

  @param[in] DevAddr        - PCI Device address of the target bridge
  @param[in] MemoryBase     - Base address of MMIO range for Memory Base and Memory Limit register
  @param[in] PrimaryBus     - Primary bus number
  @param[in] SecondaryBus   - Secondary bus number
  @param[in] SubordinateBus - Subordinate bus number

  @retval EFI_SUCCESS   - Initialize PCI bridge success
  @retval EFI_NOT_FOUND - Device is not present
**/
EFI_STATUS
HrSrvPciBridgeInit (
  IN UINT64  DevAddr,
  IN UINT16  MemoryBase,
  IN UINT8   PrimaryBus,
  IN UINT8   SecondaryBus,
  IN UINT8   SubordinateBus
  );

/**
  Un-Initialize PCI bridge device

  @param[in] DevAddr  - PCI Device address of the target bridge

  @retval EFI_SUCCESS   - Un-Initialize PCI bridge success
  @retval EFI_NOT_FOUND - Device is not present
**/
EFI_STATUS
HrSrvPciBridgeUninit (
  IN UINT64  DevAddr
  );

/**
  Query PCI bridge information for the given bridge BDF.

  @param[in, out] PciBridge      - Pointer to PCI bridge information structure.

  @retval EFI_SUCCESS           - Query PCI bridge information successfully.
  @retval EFI_NOT_FOUND         - Device is not present.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
HrSrvPciBridgeQuery (
  IN OUT PCI_BRIDGE_INFO *PciBridge
  );

/**
  Initialize PCI configuration space of PCI device for MMIO access and DMA transfer.

  @param[in] DevAddr    - PCI device address
  @param[in] MmioAddr   - MMIO base address to be used by PCI device

  @retval EFI_SUCCESS   - Initialize NHI PCI configuration space successfully.
  @retval EFI_NOT_FOUND - Device is not present.
  @retval EFI_UNSUPPORTED - Fail to initialize NHI PCI configuration space.
**/
EFI_STATUS
HrSrvPciDevInit (
  IN UINT64              DevAddr,
  IN PHYSICAL_ADDRESS    MmioAddr
  );

/**
  Un-Initialize PCI configuration space of PCI device to clear MMIO and disable DMA transfer.

  @param[in] DevAddr   - PCI device address of PCI device

  @retval EFI_SUCCESS     - Un-Initialize PCI configuration space success
  @retval EFI_NOT_FOUND   - Device is not present.
  @retval EFI_UNSUPPORTED - Fail to un-initialize NHI PCI configuration space.
**/
EFI_STATUS
HrSrvPciDevUninit (
  IN UINT64 DevAddr
  );

/**
  Query MMIO base address of PCI device.

  @param[in]  DevBdf   - PCI device location.
  @param[out] MmioAddr - Pointer to MMIO base address.

  @retval EFI_SUCCESS           - Query MMIO base address success.
  @retval EFI_UNSUPPORTED       - Fail to query MMIO base address.
  @retval EFI_NOT_READY         - MMIO is not assigned.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
HrSrvPciQueryMmio (
  IN  UINT64              DevAddr,
  OUT PHYSICAL_ADDRESS    *MmioAddr
  );

/**
  Query PCI I/O protocol of PCI device.

  @param[in]  Bus           - Bus of PCI location.
  @param[in]  Dev           - Device of PCI location.
  @param[in]  Func          - Function of PCI location.
  @param[out] PciIoProtocol - Pointer of pointer to the associated PCI I/O protocol.

  @retval EFI_SUCCESS           - Query PCI I/O protocol success.
  @retval EFI_NOT_FOUND         - Can't find PCI I/O protocol for the given BDF.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
HrSrvPciQueryIoProtocol (
  IN  UINT8                  Bus,
  IN  UINT8                  Dev,
  IN  UINT8                  Func,
  OUT EFI_PCI_IO_PROTOCOL    **PciIoProtocol
  );

/**
  Detect USB4 host router at the specified PCI device address.

  @param[in] DevAddr - PCI device address of USB4 host router to be checked.

  @retval EFI_SUCCESS   - USB4 host router is detected.
  @retval EFI_NOT_FOUND - USB4 host router is not found.
**/
EFI_STATUS
HrSrvPciDetectUsb4Hr (
  IN UINT64    DevAddr
  );
#endif
