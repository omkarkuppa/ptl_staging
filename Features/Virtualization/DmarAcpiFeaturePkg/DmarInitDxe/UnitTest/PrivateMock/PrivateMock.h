/** @file
  Mock functions

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

@par Specification Reference:
**/

#ifndef _PRIVATE_MOCK_H_
#define _PRIVATE_MOCK_H_

#include <Uefi.h>
#include <Protocol/VtdTopologyProtocol.h>
#include <Protocol/OemDmarProtocol.h>
#include <Library/UefiLib.h>

EFI_STATUS
MockGetSystemInfo (
  IN  VTD_TOPOLOGY_PROTOCOL  *This,
  OUT SYSTEM_INFO            *SystemInfo
  );

EFI_STATUS
MockGetNumberOfVtdEngines (
  IN     VTD_TOPOLOGY_PROTOCOL   *This,
     OUT UINT32                  *NumberOfVtdEngines
  );

EFI_STATUS
MockGetDevices (
  IN     VTD_TOPOLOGY_PROTOCOL   *This,
  IN     UINT32                  VtdEngineId,
     OUT DEVICE_INFO             **DeviceList,
     OUT UINT32                  *DataSize
  );

EFI_STATUS
MockGetPciDevicesWithAtc (
  IN     VTD_TOPOLOGY_PROTOCOL  *This,
  IN     UINT32                 VtdEngineId,
     OUT DEVICE_INFO            **DeviceList,
     OUT UINT32                 *ListLength
  );

EFI_STATUS
MockGetDevicesWithProperties (
  IN     VTD_TOPOLOGY_PROTOCOL  *This,
  IN     UINT32                 VtdEngineId,
     OUT DEVICE_INFO            **DeviceList,
     OUT UINT32                 *ListLength
  );

EFI_STATUS
MockGetVtdEngineInfo (
  IN     VTD_TOPOLOGY_PROTOCOL   *This,
  IN     UINT32                  VtdEngineId,
     OUT VTD_ENGINE_INFO         *Info
  );

EFI_STATUS
MockGetNumberOfReservedMemoryRegions (
  IN  VTD_TOPOLOGY_PROTOCOL  *This,
  OUT UINT32                 *NumberOfReservedMemoryRegions
  );

EFI_STATUS
MockGetReservedMemoryRegionInfo (
  IN  VTD_TOPOLOGY_PROTOCOL        *This,
  IN  UINT32                       ReservedMemoryRegionId,
  OUT RESERVED_MEMORY_REGION_INFO  *Info,
  OUT DEVICE_INFO                  **DeviceList   OPTIONAL,
  OUT UINT32                       *ListLength
  );

EFI_STATUS
MockGetRootPortsSupportingAts (
  IN  VTD_TOPOLOGY_PROTOCOL  *This,
  IN  UINT16                 SegmentId,
  OUT UINT8                  *Flags,
  OUT DEVICE_INFO            **DeviceList,
  OUT UINT32                 *ListLength
  );

EFI_STATUS
MockOemGetNumberOfReservedMemoryRegions (
  IN  OEM_DMAR_PROTOCOL      *This,
  OUT UINT32                 *NumberOfReservedMemoryRegions
  );

EFI_STATUS
MockOemGetReservedMemoryRegionInfo (
  IN  OEM_DMAR_PROTOCOL            *This,
  IN  UINT32                       ReservedMemoryRegionId,
  OUT RESERVED_MEMORY_REGION_INFO  *Info,
  OUT DEVICE_INFO                  **DeviceList   OPTIONAL,
  OUT UINT32                       *ListLength
  );

UINT64
EFIAPI
MmioRead64 (
  IN UINTN  Address
  );

#endif // _PRIVATE_MOCK_H_
