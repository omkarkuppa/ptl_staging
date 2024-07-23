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

#ifndef _DMAR_H_
#define _DMAR_H_

#include <Base.h>
#include <Uefi.h>
#include <Protocol/VtdTopologyProtocol.h>
#include <Protocol/OemDmarProtocol.h>

//
// "DMAR" DMAR Description Table Signature
//
#define EFI_ACPI_DMAR_DESCRIPTION_TABLE_SIGNATURE  0x52414d44

/**
  Add DRHD structures to DMAR ACPI table for all VT-d engines in a system.

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
  );

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
  );

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
  );

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
  );

/**
  Add SATC structures to DMAR ACPI table for all VT-d engines in a system.

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
  );

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
  );

/**
  Calculate the maximum possible size of DMAR ACPI table.

  This functions calculates the possible maximum size of DMAR ACPI table based
  on information obtained using the APIs defined in VtdTopologyProtocol.

  @param[in] VtdTopologyProtocol          Pointer to VtdTopologyProtocol.

  @retval    The maximum possible size of DMAR ACPI table.
**/
UINT32
CalculateDmarLength (
  IN VTD_TOPOLOGY_PROTOCOL  *VtdTopologyProtocol,
  IN OEM_DMAR_PROTOCOL      *OemDmarProtocol
  );

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
  );

#endif // _DMAR_H_
