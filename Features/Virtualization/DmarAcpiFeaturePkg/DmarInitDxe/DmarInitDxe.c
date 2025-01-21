/** @file
  DXE driver for creating and publishing DMAR ACPI table

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
#include <IndustryStandard/Acpi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Protocol/VtdTopologyProtocol.h>
#include <Protocol/AcpiTable.h>
#include <Library/UefiBootServicesTableLib.h>
#include <IndustryStandard/DmaRemappingReportingTable.h>
#include "Dmar.h"

/**
  This callback is called after gEfiPciEnumerationCompleteProtocolGuid event. It's responsible for creating and
  publishing DMAR ACPI table.

  @param[in]  Event     The Event this notify function registered to.
  @param[in]  Context   Pointer to the context data registered to the Event.
**/
VOID
EFIAPI
DmarAcpiInit (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_ACPI_DMAR_HEADER     *Dmar;
  EFI_STATUS               Status;
  VTD_TOPOLOGY_PROTOCOL    *VtdTopologyProtocol;
  OEM_DMAR_PROTOCOL        *OemDmarProtocol;
  UINT32                   Length;
  EFI_ACPI_TABLE_PROTOCOL  *AcpiTable;
  UINTN                    AcpiTableHandle;
  VOID                     *Protocol;

  //
  // Check whether gEfiPciEnumerationCompleteProtocolGuid is installed, if not return
  //
  Status = gBS->LocateProtocol (
                  &gEfiPciEnumerationCompleteProtocolGuid,
                  NULL,
                  &Protocol
                  );

  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Locate the VT-d Info Protocol.
  //
  DEBUG ((DEBUG_INFO, "%a(): enter\n", __FUNCTION__));

  Status = gBS->LocateProtocol (
                  &gVtdTopologyProtocolGuid,
                  NULL,
                  (VOID **)&VtdTopologyProtocol
                  );

  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Locate the OEM DMAR Protocol (this protocol is optional)
  //
  Status = gBS->LocateProtocol (
                  &gOemDmarProtocolGuid,
                  NULL,
                  (VOID **)&OemDmarProtocol
                  );

  if (EFI_ERROR (Status)) {
    OemDmarProtocol = NULL;
  }

  //
  // Locate ACPI support protocol
  //
  Status = gBS->LocateProtocol (
                  &gEfiAcpiTableProtocolGuid,
                  NULL,
                  (VOID **)&AcpiTable
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Calculate a maximum possible size of DMAR ACPI table and allocate memory for it
  //
  Length = CalculateDmarLength (VtdTopologyProtocol, NULL);

  Dmar = AllocateZeroPool (Length);
  if (Dmar == NULL) {
    ASSERT (Dmar != NULL);
    return;
  }

  //
  // Fill DMAR header
  //
  Status = CreateHeader (Dmar, VtdTopologyProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "CreateHeader() failed. Status = %r\n", Status));
  }

  //
  // Add DMA Remapping Hardware Unit Definition (DRHD) Structure - type 0
  //
  Status = AddDrhd (Dmar, VtdTopologyProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "AddDrhd() failed. Status = %r\n", Status));
  }

#if FixedPcdGetBool (PcdDmarRmrrSupport) == 1
  //
  // Add Reserved Memory Region Reporting (RMRR) Structure - type 1
  //
  Status = AddRmrr (Dmar, VtdTopologyProtocol, OemDmarProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "AddRmrr() failed. Status = %r\n", Status));
  }
#endif

#if FixedPcdGetBool (PcdDmarAtsrSupport) == 1
  //
  // Add Root Port ATS Capability Reporting (ATSR) Structure - type 2
  //
  Status = AddAtsr (Dmar, VtdTopologyProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "AddAtsr() failed. Status = %r\n", Status));
  }
#endif

#if FixedPcdGetBool (PcdDmarRhsaSupport) == 1
  //
  // Add Remapping Hardware Static Affinity (RHSA) Reporting Structure - type 3
  //
  Status = AddRhsa (Dmar, VtdTopologyProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "AddRhsa() failed. Status = %r\n", Status));
  }
#endif

#if FixedPcdGetBool (PcdDmarSatcSupport) == 1
  //
  // Add SoC Integrated Address Translation Cache (SATC) Reporting Structure - type 5
  //
  Status = AddSatc (Dmar, VtdTopologyProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "AddSatc() failed. Status = %r\n", Status));
  }
#endif

#if FixedPcdGetBool (PcdDmarSidpSupport) == 1
  //
  // Add SoC Integrated Device Property (SIDP) Reporting Structure - type 6
  //
  Status = AddSidp (Dmar, VtdTopologyProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "AddSidp() failed. Status = %r\n", Status));
  }
#endif

  AcpiTableHandle = 0;

  if (Dmar != NULL) {
    Status = AcpiTable->InstallAcpiTable (
                          AcpiTable,
                          Dmar,
                          Dmar->Header.Length,
                          &AcpiTableHandle
                          );
    FreePool (Dmar);
  }

  gBS->CloseEvent (Event);
}

/**
  This function is the entry point of this DXE DMAR ACPI Init driver.

  @param  ImageHandle       The firmware allocated handle for the EFI image.
  @param  SystemTable       A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
DmarAcpiInitEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  VOID       *Registration;
  EFI_EVENT  Event;

  Event = NULL;

  DEBUG ((DEBUG_INFO, "%a(): enter\n", __FUNCTION__));

  Event = EfiCreateProtocolNotifyEvent (
            &gEfiPciEnumerationCompleteProtocolGuid,
            TPL_NOTIFY,
            DmarAcpiInit,
            NULL,
            &Registration
            );

  if (Event == NULL) {
    return EFI_ABORTED;
  }

  return EFI_SUCCESS;
}
