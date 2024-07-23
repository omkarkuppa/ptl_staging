/** @file
  Nhlt driver for Intel platforms.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#include <Uefi.h>
#include <Uefi/UefiMultiPhase.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Nhlt.h>
#include <NhltConfiguration.h>
#include <Library/NhltLib.h>
#include <Library/GetNhltConfigurationLib.h>
#include <Uefi/UefiBaseType.h>
#include <Protocol/AcpiTable.h>
#include <Pi/PiFirmwareFile.h>
#include <Library/PcdLib.h>
#include <Library/DxeServicesLib.h>

//
// This guids should be used by Platform/Board Code for Nhlt binary assignment
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_GUID* mNhltConfigurationGuids[] = {
  &gNhltFileConf0BinGuid,
  &gNhltFileConf1BinGuid,
  &gNhltFileConf2BinGuid,
  &gNhltFileConf3BinGuid,
  &gNhltFileConf4BinGuid,
  &gNhltFileConf5BinGuid,
  &gNhltFileConf6BinGuid,
  &gNhltFileConf7BinGuid,
  &gNhltFileConf8BinGuid,
  &gNhltFileConf9BinGuid,
  &gNhltFileConf10BinGuid,
  &gNhltFileConf11BinGuid,
  &gNhltFileConf12BinGuid,
  &gNhltFileConf13BinGuid,
  &gNhltFileConf14BinGuid,
  &gNhltFileConf15BinGuid,
  &gNhltFileConf15BinGuid,
  &gNhltFileConf16BinGuid,
  &gNhltFileConf17BinGuid,
  &gNhltFileConf18BinGuid,
  &gNhltFileConf19BinGuid,
  &gNhltFileConf20BinGuid,
  &gNhltFileConf21BinGuid,
  &gNhltFileConf22BinGuid,
  &gNhltFileConf23BinGuid,
  &gNhltFileConf24BinGuid,
  &gNhltFileConf25BinGuid,
  &gNhltFileConf27BinGuid,
  &gNhltFileConf28BinGuid,
  &gNhltFileConf29BinGuid,
  &gNhltFileConf30BinGuid,
  &gNhltFileConf31BinGuid,
  &gNhltFileConf32BinGuid,
  &gNhltFileConf33BinGuid,
  &gNhltFileConf34BinGuid,
  &gNhltFileConf35BinGuid,
  &gNhltFileConf36BinGuid,
  &gNhltFileConf37BinGuid,
  &gNhltFileConf38BinGuid,
  &gNhltFileConf39BinGuid
};

GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mNhltOedConfiguration[] = { 0xEFBEADDE };
GLOBAL_REMOVE_IF_UNREFERENCED UINT32 gNhltOedConfigurationSize = sizeof (mNhltOedConfiguration);

/**
  Get Nhlt binary table from FV

  @param[in]  NhltTableGuid   Nhlt table guid.
  @param[out] TableSize       Table size.

  @retval                         Pointer to NHLT configuration which is part of final Nhlt table.
**/
NHLT_ACPI_TABLE*
NhltGetTable (
  IN EFI_GUID                    *NhltTableGuid,
  OUT UINTN                      *TableSize
  )
{
  EFI_STATUS                     Status;
  NHLT_ACPI_TABLE                *NhltTable;

  NhltTable = NULL;
  *TableSize = 0;

  Status = GetSectionFromFv (
             NhltTableGuid,
             EFI_SECTION_RAW,
             0,
             (VOID **) &NhltTable,
             TableSize
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "NHLT table binary does not exist in FVs.\n"));
    return NULL;
  }

  DEBUG ((DEBUG_INFO, "NHLT binary exist in FV. Guid: %g, Size = %d.\n", *NhltTableGuid, *TableSize));

  if ((NhltTable->Header.Signature == NHLT_ACPI_TABLE_SIGNATURE)) {
      return NhltTable;
  }

  DEBUG ((DEBUG_ERROR, "NHLT has bad signature.\n"));
  gBS->FreePool (NhltTable);
  return NULL;
}

/**
  Constructs SPECIFIC_CONFIG structure for OED configuration.

  @param[in][out] **NhltTable     Pointer to Nhlt table.
  @param[in]      NhltTableSize   Nhlt table size.


  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_BAD_BUFFER_SIZE     Not enough resources to allocate NHLT
  @retval EFI_NOT_FOUND           NHLT structure is corrupted
  @retval EFI_OUT_OF_RESOURCES    Nhlt Oed configuration has bad size
**/
EFI_STATUS
NhltOedConfigConstructor (
  IN OUT NHLT_ACPI_TABLE        *NhltTable,
  IN UINTN                      TableSize
  )
{
  SPECIFIC_CONFIG *OedConfig;

  if (NhltTable == NULL) {
    DEBUG ((DEBUG_ERROR, "NhltOedConfigConstructor: Nhlt table not exists.\n"));
    return EFI_INVALID_PARAMETER;
  }

  OedConfig = GetNhltOedConfig (NhltTable, TableSize);
  if (OedConfig == NULL) {
    DEBUG ((DEBUG_ERROR, "NhltOedConfigConstructor: OED configuration not exists.\n"));
    return EFI_NOT_FOUND;
  }

  OedConfig->CapabilitiesSize = gNhltOedConfigurationSize;
  CopyMem (OedConfig->Capabilities, (UINT8*) mNhltOedConfiguration, gNhltOedConfigurationSize);

  return EFI_SUCCESS;
}

/**
  Add Nhlt Endpoints configuration to final NHLT table.

  @param[in]      *NhltConfiguration        Pointer to Nhlt configuration.
  @param[in][out] **NhltTable               Pointer to Nhlt table.
  @param[in]      NhltTableSize             Nhlt table size.
  @param[in]      NhltLinkTypeConfiguration Nhlt Link Type configuration table settings

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_BAD_BUFFER_SIZE     Not enough resources to allocate NHLT
  @retval EFI_NOT_FOUND           Configuration was not found
  @retval EFI_OUT_OF_RESOURCES    Nhlt endpoint has bad size
**/
EFI_STATUS
NhltEndpointsConstructor (
  IN     NHLT_CONFIGURATION                           *NhltConfiguration,
  IN OUT NHLT_ACPI_TABLE                              *NhltTable,
  IN     UINTN                                        NhltTableSize
  )
{
  UINTN                  Index;
  UINT8                  EndpointIndex;
  NHLT_ACPI_TABLE        *NhltTableSource;
  UINTN                  NhltTableSourceSize;
  ENDPOINT_DESCRIPTOR    *NhltEndpoint;

  NhltTableSourceSize = 0;

  if (NhltConfiguration == NULL) {
    DEBUG ((DEBUG_ERROR, "NhltEndpointsConstructor: NhltConfiguration has NULL pointer.\n"));
    return EFI_INVALID_PARAMETER;
  }

  for (Index = 0; Index < sizeof (NhltConfiguration->NhltConfigurationEnabled); Index++) {
    if (NhltConfiguration->NhltConfigurationEnabled[Index] == TRUE) {
      DEBUG ((DEBUG_INFO, "Conf#%d enabled. ", Index));

      NhltTableSource = NhltGetTable (mNhltConfigurationGuids[Index], &NhltTableSourceSize);
      if (NhltTableSource == NULL) {
        DEBUG ((DEBUG_ERROR, "NhltEndpointsConstructor: NhltTableSource has NULL pointer.\n"));
        continue;
     }

      for (EndpointIndex = 0; EndpointIndex < NhltTableSource->EndpointCount; EndpointIndex++) {
        NhltEndpoint = GetNhltEndpoint (NhltTableSource, EndpointIndex, NhltTableSourceSize);

        if (NhltEndpoint == NULL) {
          DEBUG ((DEBUG_ERROR, "NhltEndpointsConstructor: Endpoint number %d not found in Nhlt.\n", EndpointIndex));
          return EFI_NOT_FOUND;
        }
        CopyMem (GetNhltEndpoint (NhltTable, NhltTable->EndpointCount++, NhltTableSize), NhltEndpoint, NhltEndpoint->EndpointDescriptorLength);
      }
    }
  }
  return EFI_SUCCESS;
}

/**
  Create Nhlt header which are compliant with ACPI standards and Intel ACPI tables naming.

  @param[in][out] **NhltTable               Pointer to Nhlt table.
  @param[in]      NhltTableSize             Nhlt table size.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_BAD_BUFFER_SIZE     Not enough resources to allocate NHLT
**/
EFI_STATUS
NhltHeaderConstructor (
  IN OUT NHLT_ACPI_TABLE                          *NhltTable,
  IN UINT32                                       NhltTableSize
  )
{
  EFI_ACPI_DESCRIPTION_HEADER                     *NhltHeader;

  NhltHeader = &(NhltTable->Header);

  // Header
  NhltHeader->Signature = NHLT_ACPI_TABLE_SIGNATURE;
  NhltHeader->Length          = NhltTableSize;
  NhltHeader->Revision        = 0x0;
  NhltHeader->Checksum        = 0x0;
  NhltHeader->OemTableId      = PcdGet64 (PcdAcpiDefaultOemTableId);
  NhltHeader->OemRevision     = PcdGet32 (PcdAcpiDefaultOemRevision);
  NhltHeader->CreatorId       = PcdGet32 (PcdAcpiDefaultCreatorId);
  NhltHeader->CreatorRevision = PcdGet32 (PcdAcpiDefaultCreatorRevision);
  CopyMem (NhltHeader->OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (NhltHeader->OemId));
  return EFI_SUCCESS;
}

/**
  Based on Nhlt configuration function calculate Nhlt table size

  @param[in]      *NhltConfiguration        Pointer to Nhlt configuration.

  @retval Nhlt table size including Header, Endpoints configurations and OED configurations.
**/
UINT32
NhltCalculateTableSize (
  IN NHLT_CONFIGURATION              *NhltConfiguration
  )
{
  UINTN                  Index;
  UINT8                  EndpointIndex;
  NHLT_ACPI_TABLE        *NhltTableSource;
  ENDPOINT_DESCRIPTOR    *NhltEndpoint;
  UINTN                  NhltTableSourceSize;
  UINT32                 NhltTableSize;

  NhltTableSourceSize = 0;

  if (NhltConfiguration == NULL) {
    DEBUG ((DEBUG_ERROR, "NhltCalculateTableSize: NhltConfiguration has NULL pointer.\n"));
    return 0;
  }

  // ACPI Header size
  NhltTableSize = (UINT32) sizeof (EFI_ACPI_DESCRIPTION_HEADER);
  // EndpointCount size
  NhltTableSize += (UINT32) sizeof (UINT8);

  for (Index = 0; Index < sizeof (NhltConfiguration->NhltConfigurationEnabled); Index++) {
    if (NhltConfiguration->NhltConfigurationEnabled[Index] == TRUE) {
      NhltTableSource = NhltGetTable (mNhltConfigurationGuids[Index], &NhltTableSourceSize);
      if (NhltTableSource == NULL) {
        DEBUG ((DEBUG_ERROR, "NhltCalculateTableSize: NhltTableSource has NULL pointer.\n"));
        continue;
      }

      for (EndpointIndex = 0; EndpointIndex < NhltTableSource->EndpointCount; EndpointIndex++) {
        NhltEndpoint = GetNhltEndpoint (NhltTableSource, EndpointIndex, NhltTableSourceSize);
        if (NhltEndpoint == NULL) {
          DEBUG ((DEBUG_ERROR, "NhltCalculateTableSize: NhltEndpoint has NULL pointer.\n"));
        } else {
          // Endpoint size
          NhltTableSize += (UINT32) NhltEndpoint->EndpointDescriptorLength;
        }
      }
    }
  }

  // OED Configuration header size and OED Configuration size
  NhltTableSize += (UINT32) (sizeof (UINT32) + gNhltOedConfigurationSize);

  DEBUG ((DEBUG_INFO, "NhltCalculateTableSize: Required size: %d bytes.\n", NhltTableSize));

  return NhltTableSize;
}

/**
  Create valid Nhlt structure including Nhlt header, Endpoints configuration and Oed configuration.

  @param[in]      *NhltConfiguration      Pointer to Nhlt configuration.
  @param[in][out] **NhltTable             Pointer to Nhlt table.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_BAD_BUFFER_SIZE     Not enough resources to allocate NHLT
  @retval EFI_OUT_OF_RESOURCES    Not enough memory to allocate NHLT
  @retval EFI_NOT_FOUND           NHLT structure is corrupted
**/
EFI_STATUS
NhltConstructor (
  IN     NHLT_CONFIGURATION                           *NhltConfiguration,
  IN OUT EFI_ACPI_DESCRIPTION_HEADER                  **NhltTable
  )
{
  EFI_STATUS             Status;
  NHLT_ACPI_TABLE        *TempNhltTable;
  UINT32                 NhltTableSize;

  NhltTableSize = NhltCalculateTableSize (NhltConfiguration);

  TempNhltTable = AllocateZeroPool (NhltTableSize);
  if (TempNhltTable == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Nhlt header should be added first because other parts of NHLT use Nhlt size value from header
  //
  NhltHeaderConstructor (TempNhltTable, NhltTableSize);

  Status = NhltEndpointsConstructor (NhltConfiguration, TempNhltTable, NhltTableSize);
  if (EFI_ERROR (Status)) {
      gBS->FreePool (TempNhltTable);
    return Status;
  }

  Status = NhltOedConfigConstructor (TempNhltTable, NhltTableSize);
  if (EFI_ERROR (Status)) {
    gBS->FreePool (TempNhltTable);
    return Status;
  }

  *NhltTable = (EFI_ACPI_DESCRIPTION_HEADER *) TempNhltTable;

  return EFI_SUCCESS;
}

/**
  Initialize and publish NHLT (Non-HDA Link Table), update NVS variables.

  @param[in] *NhltConfiguration        Pointer to Nhlt configuration.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_BAD_BUFFER_SIZE     Not enough resources to allocate NHLT
  @retval EFI_OUT_OF_RESOURCES    Not enough memory to allocate NHLT
  @retval EFI_INVALID_PARAMETER   NHLT structure is corrupted
**/
EFI_STATUS
NhltInstallTable (
  IN NHLT_CONFIGURATION              *NhltConfiguration
  )
{
  EFI_ACPI_DESCRIPTION_HEADER               *NhltTable;
  UINTN                                     AcpiTableKey;
  EFI_ACPI_TABLE_PROTOCOL                   *AcpiTableProtocol;
  EFI_STATUS                                Status;

  AcpiTableProtocol = NULL;
  NhltTable = NULL;
  AcpiTableKey = 0;

  DEBUG ((DEBUG_INFO, "%a () Start.\n", __FUNCTION__));

  Status = NhltConstructor (NhltConfiguration, &NhltTable);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Nhlt not created correctly - Status = %r\n", Status));
    return Status;
  }

  //
  // Locate ACPI support protocol
  //
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTableProtocol);
  if ( EFI_ERROR (Status) || AcpiTableProtocol == NULL) {
    gBS->FreePool (NhltTable);
    return EFI_UNSUPPORTED;
  }

  //
  // Publish NHLT ACPI table
  //
  Status = AcpiTableProtocol->InstallAcpiTable (
                                AcpiTableProtocol,
                                NhltTable,
                                NhltTable->Length,
                                &AcpiTableKey
                                );
  gBS->FreePool (NhltTable);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  DEBUG ((DEBUG_INFO, "%a () End. Status = %r.\n", __FUNCTION__, Status));
  return EFI_SUCCESS;
}

/**
  Retrieves address of NHLT table from XSDT/RSDT.

  @retval NHLT_ACPI_TABLE*  Pointer to NHLT table if found
  @retval NULL              NHLT could not be found
**/
EFI_STATUS
NhltLocateAndDumpAcpiTable (
  VOID
  )
{
  EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER  *Rsdp;
  EFI_ACPI_DESCRIPTION_HEADER                   *Xsdt;
  NHLT_ACPI_TABLE                               *Nhlt;
  UINTN                                         Index;
  UINT64                                        Data64;
  EFI_STATUS                                    Status;
  Rsdp  = NULL;
  Xsdt  = NULL;
  Nhlt  = NULL;

  ///
  /// Find the AcpiSupport protocol returns RSDP (or RSD PTR) address.
  ///
  Status = EfiGetSystemConfigurationTable (&gEfiAcpiTableGuid, (VOID *) &Rsdp);
  if (EFI_ERROR (Status) || (Rsdp == NULL)) {
    DEBUG ((DEBUG_ERROR, "EFI_ERROR or Rsdp == NULL\n"));
    return Status;
  }

  Xsdt = (EFI_ACPI_DESCRIPTION_HEADER *) (UINTN) Rsdp->XsdtAddress;
  if (Xsdt == NULL || Xsdt->Signature != EFI_ACPI_6_5_EXTENDED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) {
    // If XSDT has not been found, check RSDT
    Xsdt = (EFI_ACPI_DESCRIPTION_HEADER *) (UINTN) Rsdp->RsdtAddress;
    if (Xsdt == NULL || Xsdt->Signature != EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) {
      DEBUG ((DEBUG_ERROR, "XSDT/RSDT == NULL or wrong signature\n"));
      return EFI_NOT_FOUND;
    }
  }

  for (Index = sizeof (EFI_ACPI_DESCRIPTION_HEADER); Index < Xsdt->Length; Index = Index + sizeof (UINT64)) {
    Data64  = *(UINT64 *) ((UINT8 *) Xsdt + Index);
    Nhlt    = (NHLT_ACPI_TABLE *) (UINTN) Data64;
    if (Nhlt->Header.Signature == NHLT_ACPI_TABLE_SIGNATURE) {
      DEBUG ((DEBUG_INFO, "Found NhltTable signature N.H.L.T Address = 0x%016lx\n", Nhlt));
      DEBUG_CODE ( NhltAcpiTableDump (Nhlt); );
    }
  }

  if (Nhlt == NULL || Nhlt->Header.Signature != NHLT_ACPI_TABLE_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "Nhlt == NULL or wrong signature\n"));
    return EFI_NOT_FOUND;
  }

  DEBUG ((DEBUG_INFO, "Found NhltTable, Address = 0x%016lx\n", Nhlt));

  DEBUG_CODE ( NhltAcpiTableDump (Nhlt); );

  return EFI_SUCCESS;
}

/**
  Nhlt install table notification event handler.

  @param[in] Event    Event whose notification function is being invoked.
  @param[in] Context  Pointer to the notification function's context.
**/
VOID
EFIAPI
NhltInstallTableEventOnAcpiProtocol (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS              Status;
  NHLT_CONFIGURATION      NhltConfiguration;

  DEBUG ((DEBUG_INFO, "%a () Start.\n", __FUNCTION__));

  gBS->CloseEvent (Event);

  Status = GetNhltConfiguration (&NhltConfiguration);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to load NHLT configuration. Status = %r.\n", Status));
    DEBUG ((DEBUG_INFO, "%a () End. Status = %r.\n", __FUNCTION__, Status));
    return;
  }

  Status = NhltInstallTable (&NhltConfiguration);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to install NHLT table.\n"));
    DEBUG ((DEBUG_INFO, "%a () End. Status = %r.\n", __FUNCTION__, Status));
    return;
  }

  Status = NhltLocateAndDumpAcpiTable ();

  DEBUG ((DEBUG_INFO, "%a () End. Status = %r.\n", __FUNCTION__, Status));
}

/**
  Entry point for Nhlt Install driver for Intel platforms

  @param[in] ImageHandle  This image handle
  @param[in] SystemTable  System table pointer

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_ABORTED             Event not installed
**/
EFI_STATUS
EFIAPI
NhltInstallTableDriverEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  VOID       *Registration;
  EFI_EVENT  Event;

  Event = NULL;

  Event = EfiCreateProtocolNotifyEvent (
    &gEfiAcpiTableProtocolGuid,
    TPL_CALLBACK,
    NhltInstallTableEventOnAcpiProtocol,
    NULL,
    &Registration
    );
  if (Event == NULL) {
    return EFI_ABORTED;
  }

  return EFI_SUCCESS;
}