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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <Uefi.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include "PrivateMock.h"

EFI_STATUS
MockGetSystemInfo (
  IN  VTD_TOPOLOGY_PROTOCOL  *This,
  OUT SYSTEM_INFO            *SystemInfo
  )
{
  check_expected_ptr (This);

  SystemInfo->HostAddressWidth = mock_type (UINT8);
  SystemInfo->NumaSupported = mock_type (BOOLEAN);
  SystemInfo->TotalSegments = mock_type (UINT16);

  return mock_type (EFI_STATUS);
}

EFI_STATUS
MockGetNumberOfVtdEngines (
  IN     VTD_TOPOLOGY_PROTOCOL  *This,
     OUT UINT32                 *NumberOfVtdEngines
  )
{
  check_expected_ptr (This);

  *NumberOfVtdEngines = mock_type (UINT32);

  return mock_type (EFI_STATUS);
}

EFI_STATUS
MockGetDevices (
  IN     VTD_TOPOLOGY_PROTOCOL   *This,
  IN     UINT32                  VtdEngineId,
     OUT DEVICE_INFO             **DevicesList,
     OUT UINT32                  *ListLength
  )
{
  check_expected_ptr (This);
  check_expected (VtdEngineId);

  if (DevicesList != NULL) {
    *DevicesList = mock_ptr_type (VOID*);
  }
  *ListLength = mock_type (UINT32);

  return mock_type (EFI_STATUS);
}

EFI_STATUS
MockGetVtdEngineInfo (
  IN     VTD_TOPOLOGY_PROTOCOL  *This,
  IN     UINT32                 VtdEngineId,
     OUT VTD_ENGINE_INFO        *Info
  )
{
  check_expected_ptr (This);
  check_expected (VtdEngineId);

  if (Info == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Info->BaseAddress = mock_type (UINT64);
  Info->BarSize = mock_type (UINT32);
  Info->Segment = mock_type (UINT16);
  Info->ProximityDomain = mock_type (UINT32);
  Info->IncludePciAll = mock_type (BOOLEAN);

  return EFI_SUCCESS;
}

EFI_STATUS
MockGetPciDevicesWithAtc (
  IN     VTD_TOPOLOGY_PROTOCOL  *This,
  IN     UINT32                 VtdEngineId,
     OUT DEVICE_INFO            **DeviceList,
     OUT UINT32                 *ListLength
  )
{
  check_expected_ptr (This);
  check_expected (VtdEngineId);

  if (DeviceList != NULL) {
    *DeviceList = mock_ptr_type (VOID*);
  }
  *ListLength = mock_type (UINT32);

  return mock_type (EFI_STATUS);
}

EFI_STATUS
MockGetDevicesWithProperties (
  IN     VTD_TOPOLOGY_PROTOCOL  *This,
  IN     UINT32                 VtdEngineId,
     OUT DEVICE_INFO            **DeviceList,
     OUT UINT32                 *ListLength
  )
{
  check_expected_ptr (This);
  check_expected (VtdEngineId);

  if (DeviceList != NULL) {
    *DeviceList = mock_ptr_type (VOID*);
  }
  *ListLength = mock_type (UINT32);

  return mock_type (EFI_STATUS);
}

EFI_STATUS
MockGetNumberOfReservedMemoryRegions (
  IN  VTD_TOPOLOGY_PROTOCOL  *This,
  OUT UINT32                 *NumberOfReservedMemoryRegions
  )
{
  check_expected_ptr (This);

  *NumberOfReservedMemoryRegions = mock_type (UINT32);

  return mock_type (EFI_STATUS);
}

EFI_STATUS
MockGetReservedMemoryRegionInfo (
  IN  VTD_TOPOLOGY_PROTOCOL        *This,
  IN  UINT32                       ReservedMemoryRegionId,
  OUT RESERVED_MEMORY_REGION_INFO  *Info,
  OUT DEVICE_INFO                  **DeviceList   OPTIONAL,
  OUT UINT32                       *ListLength
  )
{
  EFI_STATUS Status;

  check_expected_ptr (This);
  check_expected (ReservedMemoryRegionId);

  Status = mock_type (EFI_STATUS);

  if (Status != EFI_SUCCESS) {
    return Status;
  }

  Info->PciSegment = mock_type (UINT16);
  Info->BaseAddress = mock_type (UINT64);
  Info->LimitAddress = mock_type (UINT64);

  if (DeviceList != NULL) {
    *DeviceList = mock_ptr_type (VOID*);
  }
  *ListLength = mock_type (UINT32);

  return Status;
}

EFI_STATUS
MockOemGetNumberOfReservedMemoryRegions (
  IN  OEM_DMAR_PROTOCOL      *This,
  OUT UINT32                 *NumberOfReservedMemoryRegions
  )
{
  check_expected_ptr (This);

  *NumberOfReservedMemoryRegions = mock_type (UINT32);

  return mock_type (EFI_STATUS);
}

EFI_STATUS
MockOemGetReservedMemoryRegionInfo (
  IN  OEM_DMAR_PROTOCOL            *This,
  IN  UINT32                       ReservedMemoryRegionId,
  OUT RESERVED_MEMORY_REGION_INFO  *Info,
  OUT DEVICE_INFO                  **DeviceList   OPTIONAL,
  OUT UINT32                       *ListLength
  )
{
  EFI_STATUS Status;

  check_expected_ptr (This);
  check_expected (ReservedMemoryRegionId);

  Status = mock_type (EFI_STATUS);

  if (Status != EFI_SUCCESS) {
    return Status;
  }

  Info->PciSegment = mock_type (UINT16);
  Info->BaseAddress = mock_type (UINT64);
  Info->LimitAddress = mock_type (UINT64);

  if (DeviceList != NULL) {
    *DeviceList = mock_ptr_type (VOID*);
  }
  *ListLength = mock_type (UINT32);

  return Status;
}


EFI_STATUS
MockGetRootPortsSupportingAts (
  IN  VTD_TOPOLOGY_PROTOCOL  *This,
  IN  UINT16                 SegmentId,
  OUT UINT8                  *Flags,
  OUT DEVICE_INFO            **DeviceList,
  OUT UINT32                 *ListLength
  )
{
  EFI_STATUS Status;

  check_expected_ptr (This);
  check_expected (SegmentId);

  Status = mock_type (EFI_STATUS);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  if (Flags != NULL) {
    *Flags = mock_type (UINT8);
  }

  if (DeviceList != NULL) {
    *DeviceList = mock_ptr_type (VOID*);
  }
  *ListLength = mock_type (UINT32);

  return Status;
}

UINT64
EFIAPI
MmioRead64 (
  IN      UINTN  Address
  )
{
  return mock_type (UINT64);
}