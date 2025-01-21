/** @file
  Functions for creating DMAR ACPI table

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

#include <Base.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <IndustryStandard/DmaRemappingReportingTable.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Protocol/VtdTopologyProtocol.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/OemDmarProtocol.h>
#include <IndustryStandard/Vtd.h>
#include "Dmar.h"

/**
  Add DMA Remapping Hardware Unit Definition (DRHD) structures to DMAR ACPI table for all VT-d engines in a system.

  This function adds DRHD structures along with device scope entries to DMAR ACPI table.
  It utilizes VTD_TOPOLOGY_PROTOCOL which is one of input parameters.

  @attention This function requires that GetNumberOfVtdEngines(), GetVtdEngineInfo(), GetDevices()
             APIs from VTD_TOPOLOGY_PROTOCOL are implemented.

  @attention This function is responsible for updating the size of DMAR table within DMAR header.

  @param[in, out]  Dmar                   Pointer to DMAR table to which DRHD needs to be added
  @param[in]       VtdTopologyProtocol    Pointer to VT-d info protocol

  @retval EFI_SUCCESS            The function succeeded.
  @retval EFI_INVALID_PARAMETER  Invalid input parameter.
  @retval EFI_COMPROMISED_DATA   DMAR ACPI header is compromised.
**/
EFI_STATUS
AddDrhd (
  IN OUT EFI_ACPI_DMAR_HEADER   *Dmar,
  IN     VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol
  )
{
  EFI_STATUS                                   Status;
  UINT32                                       NumberOfVtdEngines;
  UINT32                                       Length;
  UINT32                                       ListLength;
  EFI_ACPI_DMAR_DRHD_HEADER                    *Drhd;
  UINT32                                       VtdEngineId;
  UINT32                                       Index;
  VTD_ENGINE_INFO                              VtdEngineInfo;
  EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER  *DeviceScope;
  EFI_ACPI_DMAR_PCI_PATH                       *PciPath;
  DEVICE_INFO                                  *DeviceList;

  DEBUG ((DEBUG_VERBOSE, "%a(): enter\n", __FUNCTION__));

  if ((Dmar == NULL) || (VtdTopologyProtocol == NULL)) {
    DEBUG ((DEBUG_ERROR, "%a(): invalid input parameter\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  if (Dmar->Header.Signature != EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "%a(): invalid DMAR signature in ACPI header\n", __FUNCTION__));
    return EFI_COMPROMISED_DATA;
  }

  if (Dmar->Header.Length < sizeof (EFI_ACPI_DMAR_HEADER)) {
    DEBUG ((DEBUG_ERROR, "%a(): invalid length in ACPI header\n", __FUNCTION__));
    return EFI_COMPROMISED_DATA;
  }

  Status = VtdTopologyProtocol->GetNumberOfVtdEngines (VtdTopologyProtocol, &NumberOfVtdEngines);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): GetNumberOfVtdEngines() failed. Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Length = 0;
  Drhd   = (EFI_ACPI_DMAR_DRHD_HEADER *)((UINT8 *)Dmar + Dmar->Header.Length);

  for (VtdEngineId = 0; VtdEngineId < NumberOfVtdEngines; VtdEngineId++) {
    Status = VtdTopologyProtocol->GetVtdEngineInfo (VtdTopologyProtocol, VtdEngineId, &VtdEngineInfo);
    if (EFI_ERROR (Status)) {
      continue;
    }

    //
    // Create DRHD header
    //
    Drhd->Header.Type         = EFI_ACPI_DMAR_TYPE_DRHD;
    Drhd->Header.Length       = sizeof (EFI_ACPI_DMAR_DRHD_HEADER);
    Drhd->Flags               = VtdEngineInfo.IncludePciAll ? EFI_ACPI_DMAR_DRHD_FLAGS_INCLUDE_PCI_ALL : 0;
    Drhd->SegmentNumber       = VtdEngineInfo.Segment;
    Drhd->RegisterBaseAddress = VtdEngineInfo.BaseAddress;
    Drhd->Size                = (UINT8)HighBitSet32 (VtdEngineInfo.BarSize / SIZE_4KB);

    DEBUG ((
      DEBUG_INFO,
      "[DRHD%02d] segment: %d, base address: 0x%016lX, flags: %x\n",
      VtdEngineId,
      Drhd->SegmentNumber,
      Drhd->RegisterBaseAddress,
      Drhd->Flags
      ));

    DeviceList = NULL;

    //
    // Iterate through all devices which are within a scope of corresponding VT-d engine
    //
    Status = VtdTopologyProtocol->GetDevices (
                                    VtdTopologyProtocol,
                                    VtdEngineId,
                                    &DeviceList,
                                    &ListLength
                                    );
    if (!EFI_ERROR (Status)) {
      for (Index = 0; Index < ListLength; Index++) {
        DeviceScope = (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER *)((UINT8 *)Drhd + Drhd->Header.Length);

        //
        // For DRHD with INCLUDE_PCI_ALL flag set, only IOAPIC and MSI_CAPABLE_HPET device types may be
        // reported within a device scope of this DRHD structure
        //
        if (VtdEngineInfo.IncludePciAll) {
          if (!((DeviceList[Index].Type == IoApicDevice) || (DeviceList[Index].Type == HpetDevice))) {
            continue;
          }
        }

        DeviceScope->Type = DeviceList[Index].Type;

        //
        // EnumarationId is only valid for IOAPIC, MSI_CAPABLE_HPET and ACPI_NAMESPACE_DEVICE device types
        //
        if ((DeviceScope->Type == IoApicDevice) ||
            (DeviceScope->Type == HpetDevice) ||
            (DeviceScope->Type == AcpiNamespaceDevice))
        {
          DeviceScope->EnumerationId = DeviceList[Index].EnumarationId;
        } else {
          DeviceScope->EnumerationId = 0;
        }

        DeviceScope->Length         = sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER);
        DeviceScope->StartBusNumber = DeviceList[Index].Bdf.Bus;

        PciPath           = (EFI_ACPI_DMAR_PCI_PATH *)((UINT8 *)DeviceScope + DeviceScope->Length);
        PciPath->Device   = DeviceList[Index].Bdf.Device;
        PciPath->Function = DeviceList[Index].Bdf.Function;

        if ((DeviceScope->Type == IoApicDevice) ||
            (DeviceScope->Type == HpetDevice))
        {
          DEBUG ((
            DEBUG_INFO,
            "Add %a device [%02X|%02X|%02X], EnumerationId: 0x%x\n",
            (DeviceScope->Type == IoApicDevice) ? "IOAPIC" : "HPET",
            DeviceScope->StartBusNumber,
            PciPath->Device,
            PciPath->Function,
            DeviceScope->EnumerationId
            ));
        } else if ((DeviceScope->Type == PciEndpointDevice) ||
                   (DeviceScope->Type == PciBridge))
        {
          DEBUG ((
            DEBUG_INFO,
            "Add PCI %a [%02X|%02X|%02X]\n",
            (DeviceScope->Type == PciBridge) ? "bridge" : "endpoint",
            DeviceScope->StartBusNumber,
            PciPath->Device,
            PciPath->Function
            ));
        }

        DeviceScope->Length += sizeof (EFI_ACPI_DMAR_PCI_PATH);
        Drhd->Header.Length += DeviceScope->Length;
      }

      if (DeviceList != NULL) {
        FreePool (DeviceList);
      }
    }

    Length += Drhd->Header.Length;
    Drhd    = (EFI_ACPI_DMAR_DRHD_HEADER *)((UINT8 *)Drhd + Drhd->Header.Length);
  }

  Dmar->Header.Length += Length;

  return EFI_SUCCESS;
}

#if FixedPcdGetBool (PcdDmarRmrrSupport) == 1

/**
  Add one Reserved Memory Region Reporting (RMRR) structure to DMAR ACPI table for BIOS allocated reserved memory
  region which may be DMA target.

  This function adds one RMRR structure along with device scope entries to DMAR ACPI table.

  @param[in, out]  Rmrr                     Pointer to location where RMRR structure will be added.
  @param[in]       RmrrId                   RMRR Id.
  @param[in]       ReservedMemoryRegionInfo Info about BIOS allocated reserved memory region.
  @param[in]       DeviceList               The list of DEVICE_INFO entries.
  @param[in]       ListLength               The number of elements in the DeviceList.

  @retval EFI_SUCCESS            The function succeeded.
  @retval EFI_INVALID_PARAMETER  Invalid input parameter.
**/
EFI_STATUS
AddRmrrStructure (
  IN OUT EFI_ACPI_DMAR_RMRR_HEADER    *Rmrr,
  IN     UINT32                       RmrrId,
  IN     RESERVED_MEMORY_REGION_INFO  *ReservedMemoryRegionInfo,
  IN     DEVICE_INFO                  *DeviceList,
  IN     UINT32                       ListLength
  )
{
  UINT32                                       Index;
  EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER  *DeviceScope;
  EFI_ACPI_DMAR_PCI_PATH                       *PciPath;

  if ((Rmrr == NULL) ||
      (ReservedMemoryRegionInfo == NULL) ||
      (DeviceList == NULL) ||
      (ListLength == 0))
  {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Create RMRR header
  //
  Rmrr->Header.Type                      = EFI_ACPI_DMAR_TYPE_RMRR;
  Rmrr->Header.Length                    = sizeof (EFI_ACPI_DMAR_RMRR_HEADER);
  Rmrr->SegmentNumber                    = ReservedMemoryRegionInfo->PciSegment;
  Rmrr->ReservedMemoryRegionBaseAddress  = ReservedMemoryRegionInfo->BaseAddress;
  Rmrr->ReservedMemoryRegionLimitAddress = ReservedMemoryRegionInfo->LimitAddress;

  DEBUG ((
    DEBUG_INFO,
    "[RMRR%02d] segment: %d, base address: 0x%016lX, limit address: 0x%016lX\n",
    RmrrId,
    Rmrr->SegmentNumber,
    Rmrr->ReservedMemoryRegionBaseAddress,
    Rmrr->ReservedMemoryRegionLimitAddress
    ));

  for (Index = 0; Index < ListLength; Index++) {
    DeviceScope = (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER *)((UINT8 *)Rmrr + Rmrr->Header.Length);

    DeviceScope->Type           = DeviceList[Index].Type;
    DeviceScope->EnumerationId  = 0;
    DeviceScope->Length         = sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER);
    DeviceScope->StartBusNumber = DeviceList[Index].Bdf.Bus;

    PciPath           = (EFI_ACPI_DMAR_PCI_PATH *)((UINT8 *)DeviceScope + DeviceScope->Length);
    PciPath->Device   = DeviceList[Index].Bdf.Device;
    PciPath->Function = DeviceList[Index].Bdf.Function;

    if ((DeviceScope->Type == PciEndpointDevice) ||
        (DeviceScope->Type == PciBridge))
    {
      DEBUG ((
        DEBUG_INFO,
        "Add PCI %a [%02X|%02X|%02X]\n",
        (DeviceScope->Type == PciBridge) ? "bridge" : "endpoint",
        DeviceScope->StartBusNumber,
        PciPath->Device,
        PciPath->Function
        ));
    }

    DeviceScope->Length += sizeof (EFI_ACPI_DMAR_PCI_PATH);
    Rmrr->Header.Length += DeviceScope->Length;
  }

  return EFI_SUCCESS;
}

/**
  Add Reserved Memory Region Reporting (RMRR) structures to DMAR ACPI table for all BIOS allocated reserved memory
  regions which may be DMA targets.

  This function adds RMRR structures along with device scope entries to DMAR ACPI table.
  It utilizes VTD_TOPOLOGY_PROTOCOL which is one of input parameters.

  @attention This function requires that GetNumberOfReservedMemoryRegions(), GetReservedMemoryRegionInfo() APIs from
             VTD_TOPOLOGY_PROTOCOL are implemented.
  @attention This function is responsible for updating the size of DMAR table within DMAR header.

  @param[in, out]  Dmar                   Pointer to DMAR table to which RMRR needs to be added
  @param[in]       VtdTopologyProtocol    Pointer to VT-d info protocol

  @retval EFI_SUCCESS            The function succeeded.
  @retval EFI_INVALID_PARAMETER  Invalid input parameter.
  @retval EFI_COMPROMISED_DATA   DMAR ACPI header is compromised.
**/
EFI_STATUS
AddRmrr (
  IN OUT EFI_ACPI_DMAR_HEADER   *Dmar,
  IN     VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol,
  IN     OEM_DMAR_PROTOCOL      *OemDmarProtocol
  )
{
  EFI_STATUS                   Status;
  UINT32                       NumberOfReservedMemoryRegions;
  UINT32                       Length;
  EFI_ACPI_DMAR_RMRR_HEADER    *Rmrr;
  UINT32                       ReservedMemoryRegionId;
  RESERVED_MEMORY_REGION_INFO  ReservedMemoryRegionInfo;
  DEVICE_INFO                  *DeviceList;
  UINT32                       ListLength;
  UINT32                       RmrrId;

  DEBUG ((DEBUG_VERBOSE, "%a(): enter\n", __FUNCTION__));

  if (!PcdGetBool (PcdDmarRmrrEnable)) {
    DEBUG ((DEBUG_INFO, "%a(): disabled by platform\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  if ((Dmar == NULL) || (VtdTopologyProtocol == NULL)) {
    DEBUG ((DEBUG_ERROR, "%a(): invalid input parameter\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  if (Dmar->Header.Signature != EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "%a(): invalid DMAR signature in ACPI header\n", __FUNCTION__));
    return EFI_COMPROMISED_DATA;
  }

  if (Dmar->Header.Length < sizeof (EFI_ACPI_DMAR_HEADER)) {
    DEBUG ((DEBUG_ERROR, "%a(): invalid length in ACPI header\n", __FUNCTION__));
    return EFI_COMPROMISED_DATA;
  }

  Length = 0;
  Rmrr   = (EFI_ACPI_DMAR_RMRR_HEADER *)((UINT8 *)Dmar + Dmar->Header.Length);
  RmrrId = 0;

  Status = VtdTopologyProtocol->GetNumberOfReservedMemoryRegions (VtdTopologyProtocol, &NumberOfReservedMemoryRegions);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a(): VtdTopologyProtocol->GetNumberOfReservedMemoryRegions() failed. Status = %r\n", __FUNCTION__, Status));
  }

  //
  // Iterate through all Reserved Memory Regions reported by Silicon Code
  //
  if (!EFI_ERROR (Status)) {
    for (ReservedMemoryRegionId = 0; ReservedMemoryRegionId < NumberOfReservedMemoryRegions; ReservedMemoryRegionId++) {
      Status = VtdTopologyProtocol->GetReservedMemoryRegionInfo (
                                      VtdTopologyProtocol,
                                      ReservedMemoryRegionId,
                                      &ReservedMemoryRegionInfo,
                                      &DeviceList,
                                      &ListLength
                                      );
      if (EFI_ERROR (Status) || (ListLength == 0)) {
        continue;
      }

      //
      // Create one RMRR structure
      //
      Status = AddRmrrStructure (
                 Rmrr,
                 RmrrId,
                 &ReservedMemoryRegionInfo,
                 DeviceList,
                 ListLength
                 );

      if (DeviceList != NULL) {
        FreePool (DeviceList);
      }

      Length += Rmrr->Header.Length;
      Rmrr    = (EFI_ACPI_DMAR_RMRR_HEADER *)((UINT8 *)Rmrr + Rmrr->Header.Length);
      RmrrId++;
    }
  }

  //
  // Iterate through all Reserved Memory Regions reported by OEM
  //
  if (OemDmarProtocol != NULL) {
    Status = OemDmarProtocol->GetNumberOfReservedMemoryRegions (OemDmarProtocol, &NumberOfReservedMemoryRegions);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "%a(): OemDmarProtocol->GetNumberOfReservedMemoryRegions() failed. Status = %r\n", __FUNCTION__, Status));
    }

    //
    // Iterate through all Reserved Memory Regions reported by Platform/OEM
    //
    for (ReservedMemoryRegionId = 0; ReservedMemoryRegionId < NumberOfReservedMemoryRegions; ReservedMemoryRegionId++) {
      Status = OemDmarProtocol->GetReservedMemoryRegionInfo (
                                  OemDmarProtocol,
                                  ReservedMemoryRegionId,
                                  &ReservedMemoryRegionInfo,
                                  &DeviceList,
                                  &ListLength
                                  );
      if (EFI_ERROR (Status) || (ListLength == 0)) {
        continue;
      }

      //
      // Create one RMRR structure
      //
      Status = AddRmrrStructure (
                 Rmrr,
                 RmrrId,
                 &ReservedMemoryRegionInfo,
                 DeviceList,
                 ListLength
                 );

      if (DeviceList != NULL) {
        FreePool (DeviceList);
      }

      Length += Rmrr->Header.Length;
      Rmrr    = (EFI_ACPI_DMAR_RMRR_HEADER *)((UINT8 *)Rmrr + Rmrr->Header.Length);
      RmrrId++;
    }
  }

  Dmar->Header.Length += Length;

  return EFI_SUCCESS;
}

#endif

#if FixedPcdGetBool (PcdDmarAtsrSupport) == 1

/**
  Add Root Port ATS Capability Reporting (ATSR) structures to DMAR ACPI table.

  This function adds ATSR structures along with device scope entries to DMAR ACPI table. It utilizes
  VTD_TOPOLOGY_PROTOCOL which is one of input parameters.

  @attention This function requires that GetSystemInfo(), GetRootPortsSupportingAts() APIs from VTD_TOPOLOGY_PROTOCOL
             are implemented.

  @attention This function is responsible for updating the size of DMAR table within DMAR header.

  @param[in, out]  Dmar                   Pointer to DMAR table to which RHSA needs to be added
  @param[in]       VtdTopologyProtocol    Pointer to VT-d info protocol

  @retval EFI_SUCCESS            The function succeeded.
  @retval EFI_INVALID_PARAMETER  Invalid input parameter.
  @retval EFI_COMPROMISED_DATA   DMAR ACPI header is compromised.
**/
EFI_STATUS
AddAtsr (
  IN OUT EFI_ACPI_DMAR_HEADER   *Dmar,
  IN     VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol
  )
{
  EFI_STATUS                                   Status;
  SYSTEM_INFO                                  SystemInfo;
  UINT16                                       SegmentId;
  UINT32                                       AtsrId;
  UINT32                                       Length;
  UINT32                                       AtsrLength;
  EFI_ACPI_DMAR_ATSR_HEADER                    *Atsr;
  UINT8                                        Flags;
  DEVICE_INFO                                  *DeviceList;
  UINT32                                       ListLength;
  UINT32                                       Index;
  EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER  *DeviceScope;
  EFI_ACPI_DMAR_PCI_PATH                       *PciPath;

  DEBUG ((DEBUG_VERBOSE, "%a(): enter\n", __FUNCTION__));

  if (!PcdGetBool (PcdDmarAtsrEnable)) {
    DEBUG ((DEBUG_INFO, "%a(): disabled by platform\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  if ((Dmar == NULL) || (VtdTopologyProtocol == NULL)) {
    DEBUG ((DEBUG_ERROR, "%a(): invalid input parameter\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  if (Dmar->Header.Signature != EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "%a(): invalid DMAR signature in ACPI header\n", __FUNCTION__));
    return EFI_COMPROMISED_DATA;
  }

  if (Dmar->Header.Length < sizeof (EFI_ACPI_DMAR_HEADER)) {
    DEBUG ((DEBUG_ERROR, "%a(): invalid length in ACPI header\n", __FUNCTION__));
    return EFI_COMPROMISED_DATA;
  }

  Status = VtdTopologyProtocol->GetSystemInfo (VtdTopologyProtocol, &SystemInfo);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): GetSystemInfo() failed. Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  AtsrId     = 0;
  Length     = 0;
  AtsrLength = 0;
  Atsr       = (EFI_ACPI_DMAR_ATSR_HEADER *)((UINT8 *)Dmar + Dmar->Header.Length);

  for (SegmentId = 0; SegmentId < SystemInfo.TotalSegments; SegmentId++) {
    Status = VtdTopologyProtocol->GetRootPortsSupportingAts (
                                    VtdTopologyProtocol,
                                    SegmentId,
                                    &Flags,
                                    &DeviceList,
                                    &ListLength
                                    );

    if (EFI_ERROR (Status)) {
      continue;
    }

    //
    // ATSR structure MUST have at least one root port reported in its device scope or ALL_PORTS MUST be set
    //
    if ((ListLength == 0) && !(Flags & EFI_ACPI_DMAR_ATSR_FLAGS_ALL_PORTS)) {
      continue;
    }

    //
    // Create ATSR header
    //
    Atsr->Header.Type   = EFI_ACPI_DMAR_TYPE_ATSR;
    Atsr->Header.Length = sizeof (EFI_ACPI_DMAR_ATSR_HEADER);
    Atsr->Flags         = Flags;
    Atsr->SegmentNumber = SegmentId;

    DEBUG ((
      DEBUG_INFO,
      "[ATSR%02d] segment: %d, flags: 0x%x\n",
      AtsrId,
      Atsr->SegmentNumber,
      Atsr->Flags
      ));
    AtsrId++;

    if (!(Atsr->Flags & EFI_ACPI_DMAR_ATSR_FLAGS_ALL_PORTS)) {
      for (Index = 0; Index < ListLength; Index++) {
        DeviceScope = (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER *)((UINT8 *)Atsr + Atsr->Header.Length);

        //
        // All Device Scope Entries in this structure must have a Device Scope Entry Type of 02h
        //
        if (DeviceList[Index].Type != PciBridge) {
          continue;
        }

        DeviceScope->Type           = DeviceList[Index].Type;
        DeviceScope->EnumerationId  = 0;
        DeviceScope->Length         = sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER);
        DeviceScope->StartBusNumber = DeviceList[Index].Bdf.Bus;

        PciPath           = (EFI_ACPI_DMAR_PCI_PATH *)((UINT8 *)DeviceScope + DeviceScope->Length);
        PciPath->Device   = DeviceList[Index].Bdf.Device;
        PciPath->Function = DeviceList[Index].Bdf.Function;

        DeviceScope->Length += sizeof (EFI_ACPI_DMAR_PCI_PATH);
        Atsr->Header.Length += DeviceScope->Length;

        DEBUG ((
          DEBUG_INFO,
          "Add Root Port [%02X|%02X|%02X]\n",
          DeviceScope->StartBusNumber,
          PciPath->Device,
          PciPath->Function
          ));
      }
    }

    if (DeviceList != NULL) {
      FreePool (DeviceList);
    }

    Length += Atsr->Header.Length;
    Atsr    = (EFI_ACPI_DMAR_ATSR_HEADER *)((UINT8 *)Atsr + Atsr->Header.Length);
  }

  Dmar->Header.Length += Length;

  return EFI_SUCCESS;
}

#endif

#if FixedPcdGetBool (PcdDmarRhsaSupport) == 1

/**
  Add Remapping Hardware Status Affinity (RHSA) structures to DMAR ACPI table.

  It's only applicable for platforms which support non-uniform memory (NUMA) when Remapping hardware units spans across
  nodes.

  It utilizes VTD_TOPOLOGY_PROTOCOL which is one of input parameters.

  @attention This function requires that GetSystemInfo(), GetNumberOfVtdEngines() and GetVtdEngineInfo() APIs from
             VTD_TOPOLOGY_PROTOCOL are implemented.
  @attention This function is responsible for updating the size of DMAR table within DMAR header.

  @param[in, out]  Dmar                   Pointer to DMAR table to which RHSA needs to be added
  @param[in]       VtdTopologyProtocol    Pointer to VT-d info protocol

  @retval EFI_SUCCESS            The function succeeded.
  @retval EFI_INVALID_PARAMETER  Invalid input parameter.
  @retval EFI_COMPROMISED_DATA   DMAR ACPI header is compromised.
**/
EFI_STATUS
AddRhsa (
  IN OUT EFI_ACPI_DMAR_HEADER   *Dmar,
  IN     VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol
  )
{
  EFI_STATUS                 Status;
  SYSTEM_INFO                SystemInfo;
  UINT32                     NumberOfVtdEngines;
  UINT32                     Length;
  EFI_ACPI_DMAR_RHSA_HEADER  *Rhsa;
  UINT32                     VtdEngineId;
  VTD_ENGINE_INFO            VtdEngineInfo;
  UINT32                     RhsaId;

  DEBUG ((DEBUG_VERBOSE, "%a(): enter\n", __FUNCTION__));

  if (!PcdGetBool (PcdDmarRhsaEnable)) {
    DEBUG ((DEBUG_INFO, "%a(): disabled by platform\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  if ((Dmar == NULL) || (VtdTopologyProtocol == NULL)) {
    DEBUG ((DEBUG_ERROR, "%a(): invalid input parameter\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  if (Dmar->Header.Signature != EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "%a(): invalid DMAR signature in ACPI header\n", __FUNCTION__));
    return EFI_COMPROMISED_DATA;
  }

  if (Dmar->Header.Length < sizeof (EFI_ACPI_DMAR_HEADER)) {
    DEBUG ((DEBUG_ERROR, "%a(): invalid length in ACPI header\n", __FUNCTION__));
    return EFI_COMPROMISED_DATA;
  }

  Status = VtdTopologyProtocol->GetSystemInfo (VtdTopologyProtocol, &SystemInfo);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): GetSystemInfo() failed. Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  if (!SystemInfo.NumaSupported) {
    DEBUG ((DEBUG_INFO, "%a(): NUMA unsupported\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  Status = VtdTopologyProtocol->GetNumberOfVtdEngines (VtdTopologyProtocol, &NumberOfVtdEngines);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): GetNumberOfVtdEngines() failed. Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  Length = 0;
  Rhsa   = (EFI_ACPI_DMAR_RHSA_HEADER *)((UINT8 *)Dmar + Dmar->Header.Length);
  RhsaId = 0;

  for (VtdEngineId = 0; VtdEngineId < NumberOfVtdEngines; VtdEngineId++) {
    Status = VtdTopologyProtocol->GetVtdEngineInfo (VtdTopologyProtocol, VtdEngineId, &VtdEngineInfo);
    if (EFI_ERROR (Status)) {
      continue;
    }

    //
    // Create RHSA header
    //
    Rhsa->Header.Type         = EFI_ACPI_DMAR_TYPE_RHSA;
    Rhsa->Header.Length       = sizeof (EFI_ACPI_DMAR_RHSA_HEADER);
    Rhsa->RegisterBaseAddress = VtdEngineInfo.BaseAddress;
    Rhsa->ProximityDomain     = VtdEngineInfo.ProximityDomain;

    DEBUG ((
      DEBUG_INFO,
      "[RHSA%02d] base address: 0x%016lX, proximity domain: 0x%x\n",
      RhsaId,
      Rhsa->RegisterBaseAddress,
      Rhsa->ProximityDomain
      ));
    RhsaId++;

    Length += Rhsa->Header.Length;
    Rhsa    = (EFI_ACPI_DMAR_RHSA_HEADER *)((UINT8 *)Rhsa + Rhsa->Header.Length);
  }

  Dmar->Header.Length += Length;

  return EFI_SUCCESS;
}

#endif

#if FixedPcdGetBool (PcdDmarSatcSupport) == 1

/**
  Add SoC Integrated Address Translation Cache (SATC) reporting structures to DMAR ACPI table.

  This function adds SATC structures along with device scope entries to DMAR ACPI table.
  It utilizes VTD_TOPOLOGY_PROTOCOL which is one of input parameters.

  @attention This function requires that GetNumberOfVtdEngines(), GetVtdEngineInfo(),
             GetPciDevicesWithAtc() APIs from VTD_TOPOLOGY_PROTOCOL are implemented.

  @attention This function is responsible for updating the size of DMAR table within DMAR header.

  @param[in, out]  Dmar                   Pointer to DMAR table to which SATC needs to be added
  @param[in]       VtdTopologyProtocol    Pointer to VT-d info protocol

  @retval EFI_SUCCESS            The function succeeded.
  @retval EFI_INVALID_PARAMETER  Invalid input parameter.
  @retval EFI_COMPROMISED_DATA   DMAR ACPI header is compromised.
**/
EFI_STATUS
AddSatc (
  IN OUT EFI_ACPI_DMAR_HEADER   *Dmar,
  IN     VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol
  )
{
  EFI_STATUS                                   Status;
  UINT32                                       NumberOfVtdEngines;
  UINT32                                       Length;
  UINT32                                       ListLength;
  EFI_ACPI_DMAR_SATC_HEADER                    *Satc;
  UINT32                                       VtdEngineId;
  VTD_ENGINE_INFO                              VtdEngineInfo;
  DEVICE_INFO                                  *DeviceList;
  UINT32                                       Index;
  EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER  *DeviceScope;
  EFI_ACPI_DMAR_PCI_PATH                       *PciPath;
  UINT32                                       SatcId;
  VTD_ECAP_REG                                 Ecap;
  UINT32                                       SatcLength;

  DEBUG ((DEBUG_VERBOSE, "%a(): enter\n", __FUNCTION__));

  if (!PcdGetBool (PcdDmarSatcEnable)) {
    DEBUG ((DEBUG_INFO, "%a(): disabled by platform\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  if ((Dmar == NULL) || (VtdTopologyProtocol == NULL)) {
    DEBUG ((DEBUG_ERROR, "%a(): invalid input parameter\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  if (Dmar->Header.Signature != EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "%a(): invalid DMAR signature in ACPI header\n", __FUNCTION__));
    return EFI_COMPROMISED_DATA;
  }

  if (Dmar->Header.Length < sizeof (EFI_ACPI_DMAR_HEADER)) {
    DEBUG ((DEBUG_ERROR, "%a(): invalid length in ACPI header\n", __FUNCTION__));
    return EFI_COMPROMISED_DATA;
  }

  Status = VtdTopologyProtocol->GetNumberOfVtdEngines (VtdTopologyProtocol, &NumberOfVtdEngines);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): GetNumberOfVtdEngines() failed. Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  SatcId     = 0;
  Length     = 0;
  SatcLength = 0;
  Satc       = (EFI_ACPI_DMAR_SATC_HEADER *)((UINT8 *)Dmar + Dmar->Header.Length);

  for (VtdEngineId = 0; VtdEngineId < NumberOfVtdEngines; VtdEngineId++) {
    Status = VtdTopologyProtocol->GetVtdEngineInfo (VtdTopologyProtocol, VtdEngineId, &VtdEngineInfo);
    if (EFI_ERROR (Status)) {
      continue;
    }

    //
    // Check if a specific VT-d engine support Device-TLBs
    //
    Ecap.Uint64 = MmioRead64 (VtdEngineInfo.BaseAddress + R_ECAP_REG);

    if (Ecap.Bits.DT == 0) {
      DEBUG ((DEBUG_WARN, "VT-d engine [%d] has no Device-TLB support, skipping...\n", VtdEngineId));
      continue;
    }

    //
    // Iterate through all PCI SoC integrated devices having ATC which are within a scope of corresponding VT-d engine
    //
    Status = VtdTopologyProtocol->GetPciDevicesWithAtc (
                                    VtdTopologyProtocol,
                                    VtdEngineId,
                                    &DeviceList,
                                    &ListLength
                                    );

    if (!EFI_ERROR (Status)) {
      for (Index = 0; Index < ListLength; Index++) {
        //
        // According to VT-d specification:
        // All device scope entries in SATC structure must have a device scope entry type of 01h (PCI endpoint device)
        //
        if (DeviceList[Index].Type != PciEndpointDevice) {
          continue;
        }

        //
        // Create a new SATC header when a segment or flags have changed
        //
        if ((Satc->SegmentNumber != VtdEngineInfo.Segment) ||
            (Satc->Flags != DeviceList[Index].Flags) ||
            (SatcId == 0))
        {
          //
          // Check if previous SATC structure has no empty device scope
          //
          if (Satc->Header.Length > sizeof (EFI_ACPI_DMAR_SATC_HEADER)) {
            Length    += Satc->Header.Length;
            Satc       = (EFI_ACPI_DMAR_SATC_HEADER *)((UINT8 *)Satc + Satc->Header.Length);
          }

          //
          // Initialize SATC header
          //
          Satc->Header.Type   = EFI_ACPI_DMAR_TYPE_SATC;
          Satc->Header.Length = sizeof (EFI_ACPI_DMAR_SATC_HEADER);
          Satc->SegmentNumber = VtdEngineInfo.Segment;
          Satc->Flags         = DeviceList[Index].Flags;

          DEBUG ((
            DEBUG_INFO,
            "[SATC%02d] segment: %d, flags: %x\n",
            SatcId,
            Satc->SegmentNumber,
            Satc->Flags
            ));
          SatcId++;
        }

        DeviceScope = (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER *)((UINT8 *)Satc + Satc->Header.Length);

        DeviceScope->Type           = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_PCI_ENDPOINT;
        DeviceScope->Length         = sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER);
        DeviceScope->Flags          = 0;
        DeviceScope->EnumerationId  = 0;
        DeviceScope->StartBusNumber = DeviceList[Index].Bdf.Bus,

        PciPath           = (EFI_ACPI_DMAR_PCI_PATH *)((UINT8 *)DeviceScope + DeviceScope->Length);
        PciPath->Device   = DeviceList[Index].Bdf.Device;
        PciPath->Function = DeviceList[Index].Bdf.Function;

        DEBUG ((
          DEBUG_INFO,
          "Add PCI SoC Integrated endpoint [%02X|%02X|%02X]\n",
          DeviceScope->StartBusNumber,
          PciPath->Device,
          PciPath->Function
          ));

        DeviceScope->Length += sizeof (EFI_ACPI_DMAR_PCI_PATH);
        Satc->Header.Length += DeviceScope->Length;
        SatcLength           = Satc->Header.Length;
      }

      if (DeviceList != NULL) {
        FreePool (DeviceList);
      }
    }
  }

  if (SatcLength > sizeof (EFI_ACPI_DMAR_SATC_HEADER)) {
    Length += Satc->Header.Length;
  }

  Dmar->Header.Length += Length;

  return EFI_SUCCESS;
}

#endif

#if FixedPcdGetBool (PcdDmarSidpSupport) == 1

/**
  Add SoC Integrated Device Property (SIDP) reporting structures to DMAR ACPI table.

  This function adds SIDP structures along with device scope entries to DMAR ACPI table.
  It utilizes VTD_TOPOLOGY_PROTOCOL which is one of input parameters.

  @attention This function requires that GetNumberOfVtdEngines(), GetVtdEngineInfo(),
             GetDevicesWithProperties() APIs from VTD_TOPOLOGY_PROTOCOL are implemented.

  @attention This function is responsible for updating the size of DMAR table within DMAR header.

  @param[in, out]  Dmar                   Pointer to DMAR table to which SIDP needs to be added
  @param[in]       VtdTopologyProtocol    Pointer to VT-d info protocol

  @retval EFI_SUCCESS            The function succeeded.
  @retval EFI_INVALID_PARAMETER  Invalid input parameter.
  @retval EFI_COMPROMISED_DATA   DMAR ACPI header is compromised.
**/
EFI_STATUS
AddSidp (
  IN OUT EFI_ACPI_DMAR_HEADER   *Dmar,
  IN     VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol
  )
{
  EFI_STATUS                                   Status;
  UINT32                                       NumberOfVtdEngines;
  UINT32                                       SidpId;
  UINT32                                       Length;
  EFI_ACPI_DMAR_SIDP_HEADER                    *Sidp;
  UINT32                                       VtdEngineId;
  VTD_ENGINE_INFO                              VtdEngineInfo;
  VTD_ECAP_REG                                 Ecap;
  DEVICE_INFO                                  *DeviceList;
  UINT32                                       ListLength;
  UINT32                                       Index;
  EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER  *DeviceScope;
  EFI_ACPI_DMAR_PCI_PATH                       *PciPath;
  UINT32                                       SidpLength;

  DEBUG ((DEBUG_VERBOSE, "%a(): enter\n", __FUNCTION__));

  if (!PcdGetBool (PcdDmarSidpEnable)) {
    DEBUG ((DEBUG_INFO, "%a(): disabled by platform\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  if ((Dmar == NULL) || (VtdTopologyProtocol == NULL)) {
    DEBUG ((DEBUG_ERROR, "%a(): invalid input parameter\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  if (Dmar->Header.Signature != EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "%a(): invalid DMAR signature in ACPI header\n", __FUNCTION__));
    return EFI_COMPROMISED_DATA;
  }

  if (Dmar->Header.Length < sizeof (EFI_ACPI_DMAR_HEADER)) {
    DEBUG ((DEBUG_ERROR, "%a(): invalid length in ACPI header\n", __FUNCTION__));
    return EFI_COMPROMISED_DATA;
  }

  Status = VtdTopologyProtocol->GetNumberOfVtdEngines (VtdTopologyProtocol, &NumberOfVtdEngines);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): GetNumberOfVtdEngines() failed. Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  SidpId     = 0;
  Length     = 0;
  SidpLength = 0;
  Sidp       = (EFI_ACPI_DMAR_SIDP_HEADER *)((UINT8 *)Dmar + Dmar->Header.Length);

  for (VtdEngineId = 0; VtdEngineId < NumberOfVtdEngines; VtdEngineId++) {
    Status = VtdTopologyProtocol->GetVtdEngineInfo (VtdTopologyProtocol, VtdEngineId, &VtdEngineInfo);
    if (EFI_ERROR (Status)) {
      continue;
    }

    //
    // Check if a specific VT-d engine support Device-TLBs
    //
    Ecap.Uint64 = MmioRead64 (VtdEngineInfo.BaseAddress + R_ECAP_REG);

    //
    // SIDP won't be published for devices which are under a VT-d engine which doesn't support Device-TLB
    //
    if (Ecap.Bits.DT == 0) {
      DEBUG ((DEBUG_WARN, "VT-d engine [%d] has no Device-TLB support, skipping...\n", VtdEngineId));
      continue;
    }

    //
    // Iterate through all SoC integrated devices having ATC which are within a scope of corresponding VT-d engine
    //
    Status = VtdTopologyProtocol->GetDevicesWithProperties (
                                    VtdTopologyProtocol,
                                    VtdEngineId,
                                    &DeviceList,
                                    &ListLength
                                    );

    if (!EFI_ERROR (Status)) {
      for (Index = 0; Index < ListLength; Index++) {
        //
        // All device scope entries in SATC entry must have a device scope entry type of 01h (PCI endpoint device)
        // or type 05h (ACPI device)
        //
        if ((DeviceList[Index].Type != PciEndpointDevice) &&
            (DeviceList[Index].Type != AcpiNamespaceDevice))
        {
          continue;
        }

        //
        // Create a new SIDP header when a segment or flags have changed
        //
        if ((Sidp->SegmentNumber != VtdEngineInfo.Segment) ||
            (SidpId == 0))
        {
          //
          // Check if previous SIDP structure has no empty device scope
          //
          if (Sidp->Header.Length > sizeof (EFI_ACPI_DMAR_SIDP_HEADER)) {
            Length    += Sidp->Header.Length;
            Sidp       = (EFI_ACPI_DMAR_SIDP_HEADER *)((UINT8 *)Sidp + Sidp->Header.Length);
          }

          //
          // Initialize SIDP header
          //
          Sidp->Header.Type   = EFI_ACPI_DMAR_TYPE_SIDP;
          Sidp->Header.Length = sizeof (EFI_ACPI_DMAR_SIDP_HEADER);
          Sidp->SegmentNumber = VtdEngineInfo.Segment;

          DEBUG ((
            DEBUG_INFO,
            "[SIDP%02d] segment: %d\n",
            SidpId,
            Sidp->SegmentNumber
            ));
          SidpId++;
        }

        DeviceScope = (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER *)((UINT8 *)Sidp + Sidp->Header.Length);

        DeviceScope->Type           = EFI_ACPI_DEVICE_SCOPE_ENTRY_TYPE_PCI_ENDPOINT;
        DeviceScope->Length         = sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER);
        DeviceScope->Flags          = DeviceList[Index].Flags;
        DeviceScope->EnumerationId  = 0;
        DeviceScope->StartBusNumber = DeviceList[Index].Bdf.Bus,

        PciPath           = (EFI_ACPI_DMAR_PCI_PATH *)((UINT8 *)DeviceScope + DeviceScope->Length);
        PciPath->Device   = DeviceList[Index].Bdf.Device;
        PciPath->Function = DeviceList[Index].Bdf.Function;

        DEBUG ((
          DEBUG_INFO,
          "Add PCI SoC Integrated endpoint [%02X|%02X|%02X], flags: 0x%x\n",
          DeviceScope->StartBusNumber,
          PciPath->Device,
          PciPath->Function,
          DeviceScope->Flags
          ));

        DeviceScope->Length += sizeof (EFI_ACPI_DMAR_PCI_PATH);
        Sidp->Header.Length += DeviceScope->Length;
        SidpLength           = Sidp->Header.Length;
      }

      if (DeviceList != NULL) {
        FreePool (DeviceList);
      }
    }
  }

  if (SidpLength > sizeof (EFI_ACPI_DMAR_SIDP_HEADER)) {
    Length += Sidp->Header.Length;
  }

  Dmar->Header.Length += Length;

  return EFI_SUCCESS;
}

#endif

/**
  Calculate the maximum possible size of DMAR ACPI table.

  This functions calculates the possible maximum size of DMAR ACPI table based on information obtained using the APIs
  defined in VtdTopologyProtocol and OemDmarProtocol.

  @param[in] VtdTopologyProtocol          Pointer to VtdTopologyProtocol.
  @param[in] OemDmarProtocol              Pointer to OemDmarProtocol.

  @retval    The maximum possible size of DMAR ACPI table.
**/
UINT32
CalculateDmarLength (
  IN VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol,
  IN OEM_DMAR_PROTOCOL      *OemDmarProtocol
  )
{
  EFI_STATUS  Status;
  UINT32      Length;
  UINT32      NumberOfVtdEngines;
  UINT32      VtdEngineId;
  UINT32      ListLength;
  UINT32      RhsaLength;
  UINT32      SidpLength;
  UINT32      SatcLength;
  UINT32      DrhdLength;
  UINT32      AtsrLength;
  UINT32      RmrrLength;

 #if FixedPcdGetBool (PcdDmarRmrrSupport) == 1
  UINT32                       NumberOfReservedMemoryRegions;
  UINT32                       ReservedMemoryRegionId;
  RESERVED_MEMORY_REGION_INFO  ReservedMemoryRegionInfo;
 #endif
 #if (FixedPcdGetBool (PcdDmarAtsrSupport) == 1) || (FixedPcdGetBool (PcdDmarRhsaSupport) == 1)
  SYSTEM_INFO  SystemInfo;
 #endif
 #if FixedPcdGetBool (PcdDmarAtsrSupport) == 1
  UINT16  SegmentId;
 #endif

  DrhdLength = 0;
  RmrrLength = 0;
  SatcLength = 0;
  SidpLength = 0;
  RhsaLength = 0;
  AtsrLength = 0;

  DEBUG ((DEBUG_VERBOSE, "%a(): enter\n", __FUNCTION__));

  if (VtdTopologyProtocol == NULL) {
    DEBUG ((DEBUG_ERROR, "%a(): input parameter invalid\n", __FUNCTION__));
    return 0;
  }

  Status = VtdTopologyProtocol->GetNumberOfVtdEngines (VtdTopologyProtocol, &NumberOfVtdEngines);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): GetNumberOfVtdEngines() failed. Status = %r\n", __FUNCTION__, Status));
    return 0;
  }

  //
  // Calculate a size of DRHD headers
  //
  DrhdLength = NumberOfVtdEngines * sizeof (EFI_ACPI_DMAR_DRHD_HEADER);

  //
  // Calculate the size of RHSA structures
  //
 #if FixedPcdGetBool (PcdDmarRhsaSupport) == 1
  if (PcdGetBool (PcdDmarRhsaEnable)) {
    Status = VtdTopologyProtocol->GetSystemInfo (VtdTopologyProtocol, &SystemInfo);
    if (!EFI_ERROR (Status) && SystemInfo.NumaSupported) {
      RhsaLength = NumberOfVtdEngines * sizeof (EFI_ACPI_DMAR_RHSA_HEADER);
    }
  }

 #endif

  for (VtdEngineId = 0; VtdEngineId < NumberOfVtdEngines; VtdEngineId++) {
    //
    // Find the maximum number of discoverable PCI devices within VT-d engine scope
    //
    Status = VtdTopologyProtocol->GetDevices (
                                    VtdTopologyProtocol,
                                    VtdEngineId,
                                    NULL,
                                    &ListLength
                                    );
    if (!EFI_ERROR (Status)) {
      DrhdLength += ListLength * (sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER) + sizeof (EFI_ACPI_DMAR_PCI_PATH));
    }

 #if FixedPcdGetBool (PcdDmarSatcSupport) == 1
    if (PcdGetBool (PcdDmarSatcEnable)) {
      //
      // Calculate the size of SATC structures
      //
      Status = VtdTopologyProtocol->GetPciDevicesWithAtc (
                                      VtdTopologyProtocol,
                                      VtdEngineId,
                                      NULL,
                                      &ListLength
                                      );
      if (!EFI_ERROR (Status) && (ListLength > 0)) {
        SatcLength += ListLength * (sizeof (EFI_ACPI_DMAR_SIDP_HEADER) + sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER) + sizeof (EFI_ACPI_DMAR_PCI_PATH));
      }
    }

 #endif

 #if FixedPcdGetBool (PcdDmarSidpSupport) == 1
    if (PcdGetBool (PcdDmarSidpEnable)) {
      //
      // Calculate the size of SIDP structures
      //
      Status = VtdTopologyProtocol->GetDevicesWithProperties (
                                      VtdTopologyProtocol,
                                      VtdEngineId,
                                      NULL,
                                      &ListLength
                                      );
      if (!EFI_ERROR (Status) && (ListLength > 0)) {
        SidpLength += sizeof (EFI_ACPI_DMAR_SIDP_HEADER) + ListLength * (sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER) + sizeof (EFI_ACPI_DMAR_PCI_PATH));
      }
    }

 #endif
  }

 #if FixedPcdGetBool (PcdDmarRmrrSupport) == 1
  if (PcdGetBool (PcdDmarRmrrEnable)) {
    //
    // Check if there are any Reserved Memory Regions which need to be reported in DMAR
    //
    Status = VtdTopologyProtocol->GetNumberOfReservedMemoryRegions (VtdTopologyProtocol, &NumberOfReservedMemoryRegions);
    if (!EFI_ERROR (Status)) {
      for (ReservedMemoryRegionId = 0; ReservedMemoryRegionId < NumberOfReservedMemoryRegions; ReservedMemoryRegionId++) {
        //
        // Calculate the size of RMRR structure
        //
        Status = VtdTopologyProtocol->GetReservedMemoryRegionInfo (
                                        VtdTopologyProtocol,
                                        ReservedMemoryRegionId,
                                        &ReservedMemoryRegionInfo,
                                        NULL,
                                        &ListLength
                                        );
        if (!EFI_ERROR (Status) && (ListLength > 0)) {
          RmrrLength += sizeof (EFI_ACPI_DMAR_RMRR_HEADER) + ListLength * (sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER) + sizeof (EFI_ACPI_DMAR_PCI_PATH));
        }
      }
    }

    //
    // Check if there are any Reserved Memory Regions reported by OEM which need to be reported in DMAR
    //
    if (OemDmarProtocol != NULL) {
      Status = OemDmarProtocol->GetNumberOfReservedMemoryRegions (OemDmarProtocol, &NumberOfReservedMemoryRegions);
      if (!EFI_ERROR (Status)) {
        for (ReservedMemoryRegionId = 0; ReservedMemoryRegionId < NumberOfReservedMemoryRegions; ReservedMemoryRegionId++) {
          //
          // Calculate the size of RMRR structure
          //
          Status = OemDmarProtocol->GetReservedMemoryRegionInfo (
                                          OemDmarProtocol,
                                          ReservedMemoryRegionId,
                                          &ReservedMemoryRegionInfo,
                                          NULL,
                                          &ListLength
                                          );
          if (!EFI_ERROR (Status) && (ListLength > 0)) {
            RmrrLength += sizeof (EFI_ACPI_DMAR_RMRR_HEADER) + ListLength * (sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER) + sizeof (EFI_ACPI_DMAR_PCI_PATH));
          }
        }
      }
    }
  }

 #endif

 #if FixedPcdGetBool (PcdDmarAtsrSupport) == 1
  if (PcdGetBool (PcdDmarAtsrEnable)) {
    Status = VtdTopologyProtocol->GetSystemInfo (VtdTopologyProtocol, &SystemInfo);
    if (!EFI_ERROR (Status)) {
      for (SegmentId = 0; SegmentId < SystemInfo.TotalSegments; SegmentId++) {
        //
        // Calculate the size of ATSR structures
        //
        Status = VtdTopologyProtocol->GetRootPortsSupportingAts (
                                        VtdTopologyProtocol,
                                        SegmentId,
                                        NULL,
                                        NULL,
                                        &ListLength
                                        );
        if (!EFI_ERROR (Status)) {
          AtsrLength += sizeof (EFI_ACPI_DMAR_ATSR_HEADER) + ListLength * (sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER) + sizeof (EFI_ACPI_DMAR_PCI_PATH));
        }
      }
    }
  }

 #endif

  Length = sizeof (EFI_ACPI_DMAR_HEADER) + DrhdLength + RmrrLength + RhsaLength + SatcLength + SidpLength + AtsrLength;
  DEBUG ((DEBUG_VERBOSE, "Length: %d\n", Length));

  return Length;
}

/**
  Fill DMAR ACPI header.

  The function is responsible for filling DMAR ACPI header.

  @param[in] Dmar                Pointer to a buffer where DMAR ACPI table is stored.
  @param[in] VtdTopologyProtocol Pointer to VtdTopologyProtocol.

  @retval EFI_SUCCESS            The function succeeded.
  @retval EFI_INVALID_PARAMETER  Invalid input parameter.
**/
EFI_STATUS
CreateHeader (
  IN OUT EFI_ACPI_DMAR_HEADER   *Dmar,
  IN     VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol
  )
{
  EFI_STATUS   Status;
  SYSTEM_INFO  SystemInfo;

  DEBUG ((DEBUG_VERBOSE, "%a(): enter\n", __FUNCTION__));

  if ((Dmar == NULL) || (VtdTopologyProtocol == NULL)) {
    DEBUG ((DEBUG_ERROR, "%a(): input parameter invalid\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  Status = VtdTopologyProtocol->GetSystemInfo (VtdTopologyProtocol, &SystemInfo);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): GetSystemInfo() failed. Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Fill ACPI header
  //
  Dmar->Header.Signature = EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE;
  Dmar->Header.Length    = sizeof (EFI_ACPI_DMAR_HEADER);
  Dmar->Header.Revision  = PcdGet8 (PcdDmarRevision);
  Dmar->Header.Checksum  = 0x0;
  CopyMem (Dmar->Header.OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (Dmar->Header.OemId));
  Dmar->Header.OemTableId      = PcdGet64 (PcdAcpiDefaultOemTableId);
  Dmar->Header.OemRevision     = PcdGet32 (PcdAcpiDefaultOemRevision);
  Dmar->Header.CreatorId       = PcdGet32 (PcdAcpiDefaultCreatorId);
  Dmar->Header.CreatorRevision = PcdGet32 (PcdAcpiDefaultCreatorRevision);

  //
  // Fill fields specific for DMAR ACPI header
  //
  Dmar->HostAddressWidth = SystemInfo.HostAddressWidth;
  Dmar->Flags            = PcdGet8 (PcdDmarFlags);

  return EFI_SUCCESS;
}
