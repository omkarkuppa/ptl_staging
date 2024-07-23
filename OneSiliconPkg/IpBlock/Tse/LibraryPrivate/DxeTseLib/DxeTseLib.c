/** @file
  TSE driver for initializing ACPI tables

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

@par Specification
**/

#include <PiDxe.h>
#include <Protocol/AcpiTable.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/IoLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/HostBridgeDataLib.h>
#include <Defines/HostBridgeDefines.h>
#include <Library/AslUpdateLib.h>
#include <Library/TseInfoLib.h>
#include <Register/TseIocceRegs.h>
#include <Register/TseIocRegs.h>
#include <Protocol/TseNvsArea.h>
#include "TseAcpiTables.h"

///
/// Global Variables
///
GLOBAL_REMOVE_IF_UNREFERENCED TSE_NVS_AREA_PROTOCOL mTseNvsAreaProtocol;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_TSE_ACPI_DESCRIPTION_TABLE mTseAcpiTemplate;

/**
  Prints TSE ACPI Table.

  @param[in] *TseTable The TSE ACPI address to print

  @retval None
**/
VOID
TseAcpiTableDump (
  IN EFI_TSE_ACPI_DESCRIPTION_TABLE            *TseTable
  )
{
  UINT8 EngineIndex;
  EFI_TSE_ACPI_CAPABILITY_STRUCTURE *CapabilityStructurePtr = NULL;

  if (TseTable == NULL)
  {
    DEBUG ((DEBUG_INFO, "TSE Table is null and cannot be printed. \n"));
    return;
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DEBUG ((DEBUG_INFO, "--- TSE ACPI Table Dump ---\n"));

  DEBUG ((DEBUG_INFO, "sizeof EFI_TSE_ACPI_DESCRIPTION_TABLE  = %d B\n", sizeof (EFI_TSE_ACPI_DESCRIPTION_TABLE)));

  DEBUG ((DEBUG_INFO, " TSE_ACPI_TABLE Header.Length          = 0x%08x\n", TseTable->Header.Length));
  DEBUG ((DEBUG_INFO, " TSE_ACPI_TABLE Header.Signature       = 0x%08x\n", TseTable->Header.Signature));
  DEBUG ((DEBUG_INFO, " TSE_ACPI_TABLE Header.Revision        = 0x%02x\n", TseTable->Header.Revision));
  DEBUG ((DEBUG_INFO, " TSE_ACPI_TABLE Header.Checksum        = 0x%02x\n", TseTable->Header.Checksum));
  DEBUG ((DEBUG_INFO, " TSE_ACPI_TABLE Header.OemId           = %a\n",     TseTable->Header.OemId));
  DEBUG ((DEBUG_INFO, " TSE_ACPI_TABLE Header.OemTableId      = 0x%lx\n",  TseTable->Header.OemTableId));
  DEBUG ((DEBUG_INFO, " TSE_ACPI_TABLE Header.OemRevision     = 0x%08x\n", TseTable->Header.OemRevision));
  DEBUG ((DEBUG_INFO, " TSE_ACPI_TABLE Header.CreatorId       = 0x%08x\n", TseTable->Header.CreatorId));
  DEBUG ((DEBUG_INFO, " TSE_ACPI_TABLE Header.CreatorRevision = 0x%08x\n", TseTable->Header.CreatorRevision));
  DEBUG ((DEBUG_INFO, " TSE_ACPI_TABLE NumberOfEngines        = 0x%08x\n", TseTable->NumberOfEngines));
  DEBUG ((DEBUG_INFO, " TSE_ACPI_TABLE Flags                  = 0x%08x\n", TseTable->Flags));
  DEBUG ((DEBUG_INFO, "\n"));

  for (EngineIndex = 0; EngineIndex < mTseAcpiTemplate.NumberOfEngines; EngineIndex++) {
    CapabilityStructurePtr = &(TseTable->CapabilityStructures[EngineIndex]);
    DEBUG ((DEBUG_INFO, "CapabilitiesStructure[%d]\n", EngineIndex));
    DEBUG ((DEBUG_INFO, "  ->Type                    = 0x%08x\n", CapabilityStructurePtr->Type));
    DEBUG ((DEBUG_INFO, "  ->Flags                   = 0x%08x\n", CapabilityStructurePtr->Flags));
    DEBUG ((DEBUG_INFO, "  ->Revision                = 0x%08x\n", CapabilityStructurePtr->Revision));
    DEBUG ((DEBUG_INFO, "  ->NumberOfDeviceScopes    = 0x%08x\n", CapabilityStructurePtr->NumberOfDeviceScopes));
    DEBUG ((DEBUG_INFO, "  ->KeySourcesSupported     = 0x%08x\n", CapabilityStructurePtr->KeySourcesSupported));
    DEBUG ((DEBUG_INFO, "  ->EncryptionAlgoSupported = 0x%08x\n", CapabilityStructurePtr->EncryptionAlgoSupported));
    DEBUG ((DEBUG_INFO, "  ->MaxKeysSupported        = 0x%08x\n", CapabilityStructurePtr->MaxKeysSupported));
    DEBUG ((DEBUG_INFO, "  ->MaxBdfSupported         = 0x%08x\n", CapabilityStructurePtr->MaxBdfSupported));
    DEBUG ((DEBUG_INFO, "  ->PageSizesSupported      = 0x%08x\n", CapabilityStructurePtr->PageSizesSupported));
    DEBUG ((DEBUG_INFO, "  ->LbaGranularity          = 0x%08x\n", CapabilityStructurePtr->LbaGranularity));
    DEBUG ((DEBUG_INFO, "  ->DataTransferAlignment   = 0x%08x\n", CapabilityStructurePtr->DataTransferAlignment));
    DEBUG ((DEBUG_INFO, "  ->CryptoTweakSize         = 0x%08x\n", CapabilityStructurePtr->CryptoTweakSize));
    DEBUG ((DEBUG_INFO, "  ->IocceMmioBase           = 0x%016llx\n", CapabilityStructurePtr->IocceMmioBase));
    DEBUG ((DEBUG_INFO, "  ->IocceMmioLimit          = 0x%016llx\n", CapabilityStructurePtr->IocceMmioLimit));
    DEBUG ((DEBUG_INFO, "  ->IocMmioBase             = 0x%016llx\n", CapabilityStructurePtr->IocMmioBase));
    DEBUG ((DEBUG_INFO, "  ->IocMmioLimit            = 0x%016llx\n", CapabilityStructurePtr->IocMmioLimit));
  }

  DEBUG ((DEBUG_INFO, "----------------------------------------------------------------------\n"));
}

/**
  Retrieves address of TSE table from XSDT/RSDT and prints the table

  @retval EFI_SUCCESS     If TSE table was successfully located
  @retval EFI_NOT_FOUND   If TSE table was not found
**/
EFI_STATUS
TseLocateAndDumpAcpiTable (
  VOID
  )
{
  EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER  *Rsdp;
  EFI_TSE_ACPI_DESCRIPTION_TABLE                *TseTable;
  EFI_ACPI_DESCRIPTION_HEADER                   *TableAddress;
  UINTN                                         TableEntrySize;
  UINTN                                         Index;
  EFI_STATUS                                    Status;

  Rsdp  = NULL;
  TableAddress = NULL;
  TseTable  = NULL;
  TableEntrySize = 0;

  ///
  /// Find the AcpiSupport protocol returns RSDP (or RSD PTR) address.
  ///
  Status = EfiGetSystemConfigurationTable (&gEfiAcpiTableGuid, (VOID *) &Rsdp);

  if (EFI_ERROR (Status) || (Rsdp == NULL)) {
    DEBUG ((DEBUG_ERROR, "EFI_ERROR or Rsdp == NULL\n"));
    return Status;
  }

  // Check for XSDT
  TableAddress = (EFI_ACPI_DESCRIPTION_HEADER *) (UINTN) Rsdp->XsdtAddress;
  TableEntrySize = sizeof (UINT64);
  if (TableAddress == NULL || TableAddress->Signature != EFI_ACPI_6_5_EXTENDED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) {
    // If XSDT has not been found, check for RSDT
    TableAddress = (EFI_ACPI_DESCRIPTION_HEADER *) (UINTN) Rsdp->RsdtAddress;
    TableEntrySize = sizeof (UINT32);
    if (TableAddress == NULL || TableAddress->Signature != EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) {
      DEBUG ((DEBUG_ERROR, "XSDT/RSDT == NULL or wrong signature\n"));
      return EFI_NOT_FOUND;
    }
  }

  // Parse selected table XSDT (64-bit entries) or RSDT (32-bit entries)
  for (Index = sizeof (EFI_ACPI_DESCRIPTION_HEADER); Index < TableAddress->Length; Index = Index + TableEntrySize) {

    if (TableEntrySize == sizeof(UINT32)) {
      TseTable = (EFI_TSE_ACPI_DESCRIPTION_TABLE *) (UINTN) *(UINT32 *)((UINT8 *) TableAddress + Index);
    } else {
      TseTable = (EFI_TSE_ACPI_DESCRIPTION_TABLE *) (UINTN) *(UINT64 *)((UINT8 *) TableAddress + Index);
    }

    if (TseTable->Header.Signature == EFI_TSE_ACPI_DESCRIPTION_TABLE_SIGNATURE) {
      DEBUG ((DEBUG_INFO, "Found TseTableTable Address = 0x%016lx\n", TseTable));
      TseAcpiTableDump (TseTable);
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/**
   Create and Publish TSE ACPI table

   @retval   EFI_SUCCESS     The TSE ACPI table is published successfully.
   @retval   Others          The TSE ACPI table is not published.

**/
EFI_STATUS
CreateAndPublishTseReportingStructure (
  VOID
  )
{
  EFI_STATUS               Status;
  EFI_ACPI_TABLE_PROTOCOL  *AcpiTable;
  UINTN                    TableKey;
  UINT8                    EngineIndex;
  UINT64                   MchBar = 0;

  TSE_IOCCE_CAPABILITY_MCHBAR_STRUCT TseIocceCapability;
  EFI_TSE_ACPI_CAPABILITY_STRUCTURE  *CapabilityStructurePtr = NULL;

  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);

  ZeroMem (&TseIocceCapability, sizeof (TSE_IOCCE_CAPABILITY_MCHBAR_STRUCT));
  ZeroMem (&mTseAcpiTemplate, sizeof (mTseAcpiTemplate));

  mTseAcpiTemplate.Header.Length = sizeof (EFI_TSE_ACPI_DESCRIPTION_TABLE);

  mTseAcpiTemplate.Header.Signature       = EFI_TSE_ACPI_DESCRIPTION_TABLE_SIGNATURE;
  mTseAcpiTemplate.Header.Revision        = EFI_TSE_ACPI_DESCRIPTION_TABLE_REVISION;

  CopyMem (
    mTseAcpiTemplate.Header.OemId,
    PcdGetPtr (PcdAcpiDefaultOemId),
    sizeof (mTseAcpiTemplate.Header.OemId));

  mTseAcpiTemplate.Header.OemTableId      = PcdGet64 (PcdAcpiDefaultOemTableId);
  mTseAcpiTemplate.Header.OemRevision     = PcdGet32 (PcdAcpiDefaultOemRevision);
  mTseAcpiTemplate.Header.CreatorId       = PcdGet32 (PcdAcpiDefaultCreatorId);
  mTseAcpiTemplate.Header.CreatorRevision = PcdGet32 (PcdAcpiDefaultCreatorRevision);

  mTseAcpiTemplate.NumberOfEngines        = FixedPcdGet8 (PcdTseEngines);
  mTseAcpiTemplate.Flags                  = EFI_TSE_ACPI_DESCRIPTION_TABLE_FLAGS;

  //
  // Read capabilities from IOCCE
  //
  TseIocceCapability.Data = MmioRead64 (MchBar + R_IOCCE_TSE_BASE + B_IOCCE_TSE_IOCCE_CAPABILITY);

  for (EngineIndex = 0; EngineIndex < mTseAcpiTemplate.NumberOfEngines; EngineIndex++) {
    CapabilityStructurePtr = &(mTseAcpiTemplate.CapabilityStructures[EngineIndex]);
    ZeroMem (CapabilityStructurePtr, sizeof (EFI_TSE_ACPI_CAPABILITY_STRUCTURE));

    CapabilityStructurePtr->Type                         = V_IOCCE_TSE_IOCCE_CAPABILITY_TYPE;
    CapabilityStructurePtr->Flags                        = V_IOCCE_TSE_IOCCE_CAPABILITY_FLAGS_PCI_ALL;
    CapabilityStructurePtr->Revision                     = PcdGet8 (PcdTseCapabilityReportingStructureRevision);
    CapabilityStructurePtr->NumberOfDeviceScopes         = PcdGet8 (PcdTseNumberOfDeviceScopes);
    CapabilityStructurePtr->KeySourcesSupported          = GetTseKeySources ();
    CapabilityStructurePtr->EncryptionAlgoSupported      = GetTseSupportedAlgorithms ();
    CapabilityStructurePtr->MaxKeysSupported             = GetTseMaxKeys ();
    CapabilityStructurePtr->MaxBdfSupported              = (UINT8) TseIocceCapability.Bits.TSE_MAX_BDF_Supported;
    CapabilityStructurePtr->PageSizesSupported           = (UINT8) TseIocceCapability.Bits.PageSizeSupported;
    CapabilityStructurePtr->LbaGranularity               = (UINT8) TseIocceCapability.Bits.LBA_Granularity;
    CapabilityStructurePtr->DataTransferAlignment        = (UINT8) TseIocceCapability.Bits.Data_Transfer_Alignment;
    CapabilityStructurePtr->CryptoTweakSize              = (UINT8) FixedPcdGet8 (PcdTseTweakSize);

    CapabilityStructurePtr->IocceMmioBase                = MchBar + R_IOCCE_TSE_BASE;
    CapabilityStructurePtr->IocceMmioLimit               = MchBar + R_IOCCE_TSE_LIMIT;
    CapabilityStructurePtr->IocMmioBase                  = MchBar + R_IOC_TSE_BASE;
    CapabilityStructurePtr->IocMmioLimit                 = MchBar + R_IOC_TSE_LIMIT;

    //
    // No implemented support for device scopes
    //
    CapabilityStructurePtr->Length                       = sizeof (EFI_TSE_ACPI_CAPABILITY_STRUCTURE);
  }

  //
  // Update Table Checksum prior to publish.
  //
  mTseAcpiTemplate.Header.Checksum = CalculateCheckSum8 ((UINT8 *)&mTseAcpiTemplate, mTseAcpiTemplate.Header.Length);

  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **)&AcpiTable);

  if ((EFI_ERROR (Status)) || (AcpiTable == NULL)) {
    DEBUG ((DEBUG_ERROR, "[TSE] Could not locate gEfiAcpiTableProtocolGuid!\n"));
    ASSERT_EFI_ERROR (Status);
  } else {
    //
    // Install ACPI table
    //
    TableKey = 0;
    Status = AcpiTable->InstallAcpiTable (
                          AcpiTable,
                          &mTseAcpiTemplate,
                          mTseAcpiTemplate.Header.Length,
                          &TableKey
                          );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[TSE] Could not install TSE ACPI table!\n"));
      ASSERT_EFI_ERROR (Status);
    }
  }

  return Status;
}

/**
   Patch TSE Nvs Area region.

   @retval EFI_SUCCESS    initialized successfully
   @retval EFI_NOT_FOUND  Nvs Area operation region is not found
**/
EFI_STATUS
PatchTseNvsAreaAddress (
  VOID
  )
{
  EFI_STATUS                            Status;
  UINT32                                Address;
  UINT16                                Length;
  TSE_NVS_AREA_PROTOCOL                 *TseNvsAreaProtocol;

  Status = gBS->LocateProtocol (&gTseNvsAreaProtocolGuid, NULL, (VOID **) &TseNvsAreaProtocol);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[TSE] Could not locate TseNvsAreaProtocol!\n"));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Address = (UINT32) (UINTN)  TseNvsAreaProtocol->Area;
  Length  = (UINT16) sizeof (TSE_NVS_AREA);
  DEBUG ((DEBUG_INFO, "Patch TSE NvsAreaAddress: TSE NVS Address %x Length %x\n", Address, Length));

  Status  = UpdateNameAslCode (SIGNATURE_32 ('T','S','N','B'), &Address, sizeof (Address));
  ASSERT_EFI_ERROR (Status);
  Status  = UpdateNameAslCode (SIGNATURE_32 ('T','S','N','L'), &Length, sizeof (Length));
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

/**
   Install TSE Global NVS protocol

   @param[in]  ImageHandle  Image Handle.

   @retval   EFI_SUCCESS     The TSE ACPI table is published successfully.
   @retval   Others          The TSE ACPI table is not published.
**/
EFI_STATUS
InstallTseNvsProtocol (
  IN EFI_HANDLE         ImageHandle
  )
{
  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  MemoryAddress;

  MemoryAddress = SIZE_4GB - 1;

  ///
  /// Install TSE Global protocol
  ///
  DEBUG ((DEBUG_INFO, "Install TSE NVS protocol\n"));

  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiACPIMemoryNVS,
                  EFI_SIZE_TO_PAGES (sizeof (TSE_NVS_AREA)),
                  &MemoryAddress);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[TSE] Could not allocate TSE NVS area!\n"));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  mTseNvsAreaProtocol.Area = (TSE_NVS_AREA *) (UINTN) MemoryAddress;

  ZeroMem ((VOID *) mTseNvsAreaProtocol.Area, sizeof (TSE_NVS_AREA));

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gTseNvsAreaProtocolGuid,
                  &mTseNvsAreaProtocol,
                  NULL
                  );

  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  Update TSE NVS area.

  This function will update the TSE NVS Area based on the provided BIOS TSE policy value.
  The TseEnable field controls whether the ACPI methods are exposed.

  @param[in]  TseEnabled BIOS TSE policy state

  @retval EFI_SUCCESS     The entry point is executed successfully.
  @retval Others          Some error occurs when executing this entry point.
**/
EFI_STATUS
UpdateTseNvs (
  IN BOOLEAN TsePolicy
  )
{
  TSE_NVS_AREA_PROTOCOL            *TseNvsAreaProtocol;
  EFI_STATUS                       Status;

  Status = gBS->LocateProtocol (
                  &gTseNvsAreaProtocolGuid,
                  NULL,
                  (VOID **) &TseNvsAreaProtocol
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[TSE] Could not update TSE NVS due to failed to locate TseNvsAreaProtocol.\n"));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  DEBUG ((DEBUG_INFO, "[TSE] Update TSE GNVS Area.\n"));
  TseNvsAreaProtocol->Area->TseEnable = TsePolicy;
  DEBUG ((DEBUG_INFO, "[TSE] TseNvsAreaProtocol.Area->TseEnabled = %x\n", TseNvsAreaProtocol->Area->TseEnable));

  return Status;
}

/**
   This function initializes and installs ACPI tables used for handling TSE.

   @param[in] ImageHandle  The firmware allocated handle for the EFI image.

   @retval EFI_SUCCESS     The entry point is executed successfully.
   @retval Others          Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
InitializeTseAcpi (
  IN EFI_HANDLE         ImageHandle
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  BOOLEAN TsePolicy;

  TsePolicy = IsTseEnabled ();

  if (TsePolicy == TRUE) {
    //
    // Create and Publish TSE ACPI Reporting Structure
    //
    Status = CreateAndPublishTseReportingStructure ();
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[TSE] Could not publish TSE reporting structure!\n"));
      ASSERT_EFI_ERROR (Status);
    }
    TseLocateAndDumpAcpiTable ();
  }

  //
  // Update ACPI NV register for exposing the TSE Device object and resources
  //
  UpdateTseNvs (TsePolicy);
  return Status;
}
