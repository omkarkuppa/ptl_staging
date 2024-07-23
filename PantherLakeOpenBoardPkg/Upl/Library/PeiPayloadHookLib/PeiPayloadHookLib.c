/** @file
  Library instance to list all DynamicEx PCD PPL consumes.
  No real functionality.

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

#include <PiPei.h>
#include <Uefi.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciHostBridgeLib.h>
#include <Protocol/DevicePath.h>
#include <UniversalPayload/SerialPortInfo.h>
#include <UniversalPayload/PciRootBridges.h>
#include <UniversalPayload/SmbiosTable.h>
#include <UniversalPayload/AcpiTable.h>
#include <Guid/AcpiBoardInfoGuid.h>
#include <Guid/DebugPrintErrorLevel.h>
#include <IndustryStandard/SmBios.h>
#include <IndustryStandard/Acpi.h>

/**
  Create initialize ACPI RSDP that include minmum XSDT

  @retval EFI_SUCCESS             The function completes successfully
  @retval EFI_OUT_OF_RESOURCES    if not able to get resources.
**/
EFI_STATUS
CreateUniversalPayloadAcpiTable (
  VOID
  )
{
  EFI_STATUS                                              Status;
  EFI_PHYSICAL_ADDRESS                                    RsdpAddress;
  EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER            *Rsdp;
  EFI_ACPI_DESCRIPTION_HEADER                             *Xsdt;
  UNIVERSAL_PAYLOAD_ACPI_TABLE                            *PlatformAcpiTable;

  Status             = EFI_SUCCESS;
  Rsdp               = NULL;
  Xsdt               = NULL;
  PlatformAcpiTable  = NULL;
  RsdpAddress        = 0;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  //
  // Memory Allocation for ACPI Tables.
  //
  Status = PeiServicesAllocatePages (
             EfiACPIReclaimMemory,
             EFI_SIZE_TO_PAGES (
               sizeof (EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER) + // RSDP
               sizeof (EFI_ACPI_DESCRIPTION_HEADER)),                  // XSDT
             &RsdpAddress
             );
  if (EFI_ERROR (Status) || RsdpAddress == 0) {
    ASSERT_EFI_ERROR (Status);
    ASSERT (RsdpAddress != 0);
    return Status;
  }

  ZeroMem (
    (VOID *) (UINTN) RsdpAddress,
    sizeof (EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER) +
    sizeof (EFI_ACPI_DESCRIPTION_HEADER)
    );

  Rsdp = (EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER *) (UINTN) (RsdpAddress);
  Xsdt = (EFI_ACPI_DESCRIPTION_HEADER *) (Rsdp + 1);

  //
  // Update the Values for RSDP.
  //
  CopyMem (&Rsdp->OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (Rsdp->OemId));
  SetMem (Rsdp->Reserved, sizeof (Rsdp->Reserved), EFI_ACPI_RESERVED_BYTE);
  Rsdp->Signature        = EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER_SIGNATURE;
  Rsdp->Length           = sizeof (EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER);
  Rsdp->Revision         = EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER_REVISION;
  Rsdp->XsdtAddress      = (UINT64)(UINTN)Xsdt;
  Rsdp->Checksum         = CalculateCheckSum8 ((UINT8 *)Rsdp, 20);
  Rsdp->ExtendedChecksum = CalculateCheckSum8 ((UINT8 *)Rsdp, Rsdp->Length);

  //
  // Update the values for XSDT.
  //
  CopyMem (&Xsdt->OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (Xsdt->OemId));
  Xsdt->Signature       = EFI_ACPI_6_5_EXTENDED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE;
  Xsdt->Length          = sizeof (EFI_ACPI_DESCRIPTION_HEADER);
  Xsdt->OemTableId      = PcdGet64 (PcdAcpiDefaultOemTableId);
  Xsdt->OemRevision     = PcdGet32 (PcdAcpiDefaultOemRevision);
  Xsdt->CreatorId       = PcdGet32 (PcdAcpiDefaultCreatorId);
  Xsdt->CreatorRevision = PcdGet32 (PcdAcpiDefaultCreatorRevision);
  Xsdt->Revision        = EFI_ACPI_6_5_EXTENDED_SYSTEM_DESCRIPTION_TABLE_REVISION;
  Xsdt->Checksum        = CalculateCheckSum8 ((UINT8 *)Xsdt, Xsdt->Length);

  //
  // Update gUniversalPayloadAcpiTableGuid HOB with RSDP.
  //
  PlatformAcpiTable = BuildGuidHob (&gUniversalPayloadAcpiTableGuid, sizeof (UNIVERSAL_PAYLOAD_ACPI_TABLE));
  if (PlatformAcpiTable == NULL) {
    ASSERT (PlatformAcpiTable != NULL);
    Status = EFI_OUT_OF_RESOURCES;
    return Status;
  }
  PlatformAcpiTable->Rsdp            = (EFI_PHYSICAL_ADDRESS)(UINTN)Rsdp;
  PlatformAcpiTable->Header.Revision = UNIVERSAL_PAYLOAD_ACPI_TABLE_REVISION;
  PlatformAcpiTable->Header.Length   = sizeof (UNIVERSAL_PAYLOAD_ACPI_TABLE);

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));
  return Status;
}

/**
  Create PCI Root Bridge Info Hob that will be used by Universal Payload

  @retval EFI_SUCCESS             The function completes successfully
  @retval EFI_OUT_OF_RESOURCES    Could not allocate HOB.
**/
EFI_STATUS
CreateUniversalPayloadPciRootBridge (
  VOID
  )
{
  EFI_STATUS                          Status;
  UNIVERSAL_PAYLOAD_PCI_ROOT_BRIDGES  *PciRootBridgeInfo;
  PCI_ROOT_BRIDGE                     *PlatformPciRootBridges;
  UINTN                               RootBridgeCount;
  UINTN                               HobDataSize;
  UINT8                               Index;

  Status                 = EFI_SUCCESS;
  PlatformPciRootBridges = NULL;
  PciRootBridgeInfo      = NULL;
  RootBridgeCount        = 0;
  HobDataSize            = 0;
  Index                  = 0;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  PlatformPciRootBridges = PciHostBridgeGetRootBridges (&RootBridgeCount);
  DEBUG ((DEBUG_INFO, "RootBridgeCount = %d\n", RootBridgeCount));

  HobDataSize = sizeof (UNIVERSAL_PAYLOAD_PCI_ROOT_BRIDGES) + RootBridgeCount * sizeof (UNIVERSAL_PAYLOAD_PCI_ROOT_BRIDGE);
  PciRootBridgeInfo = BuildGuidHob (&gUniversalPayloadPciRootBridgeInfoGuid, HobDataSize);
  if (PciRootBridgeInfo == NULL) {
    ASSERT (PciRootBridgeInfo != NULL);
    Status = EFI_OUT_OF_RESOURCES;
    return Status;
  }
  ZeroMem (PciRootBridgeInfo, HobDataSize);

  PciRootBridgeInfo->Count            = (UINT8) RootBridgeCount;
  PciRootBridgeInfo->Header.Length    = sizeof (UNIVERSAL_PAYLOAD_PCI_ROOT_BRIDGES);
  PciRootBridgeInfo->Header.Revision  = UNIVERSAL_PAYLOAD_PCI_ROOT_BRIDGES_REVISION;
  PciRootBridgeInfo->ResourceAssigned = FALSE;

  if (PlatformPciRootBridges != NULL && RootBridgeCount !=0) {
    while (Index < RootBridgeCount) {
      PciRootBridgeInfo->RootBridge[Index].Bus.Base = PlatformPciRootBridges[Index].Bus.Base;
      PciRootBridgeInfo->RootBridge[Index].Bus.Limit = PlatformPciRootBridges[Index].Bus.Limit;

      PciRootBridgeInfo->RootBridge[Index].Io.Base = PlatformPciRootBridges[Index].Io.Base;
      PciRootBridgeInfo->RootBridge[Index].Io.Limit = PlatformPciRootBridges[Index].Io.Limit;

      PciRootBridgeInfo->RootBridge[Index].Mem.Base = PlatformPciRootBridges[Index].Mem.Base;
      PciRootBridgeInfo->RootBridge[Index].Mem.Limit = PlatformPciRootBridges[Index].Mem.Limit;

      PciRootBridgeInfo->RootBridge[Index].MemAbove4G.Base = PlatformPciRootBridges[Index].MemAbove4G.Base;
      PciRootBridgeInfo->RootBridge[Index].MemAbove4G.Limit = PlatformPciRootBridges[Index].MemAbove4G.Limit;

      PciRootBridgeInfo->RootBridge[Index].PMem.Base = PlatformPciRootBridges[Index].PMem.Base;
      PciRootBridgeInfo->RootBridge[Index].PMem.Limit = PlatformPciRootBridges[Index].PMem.Limit;

      PciRootBridgeInfo->RootBridge[Index].PMemAbove4G.Base = PlatformPciRootBridges[Index].PMemAbove4G.Base;
      PciRootBridgeInfo->RootBridge[Index].PMemAbove4G.Limit = PlatformPciRootBridges[Index].PMemAbove4G.Limit;

      PciRootBridgeInfo->RootBridge[Index].Segment = PlatformPciRootBridges[Index].Segment;
      PciRootBridgeInfo->RootBridge[Index].Supports = PlatformPciRootBridges[Index].Supports;
      PciRootBridgeInfo->RootBridge[Index].Attributes = PlatformPciRootBridges[Index].Attributes;
      PciRootBridgeInfo->RootBridge[Index].DmaAbove4G = PlatformPciRootBridges[Index].DmaAbove4G;
      PciRootBridgeInfo->RootBridge[Index].NoExtendedConfigSpace = PlatformPciRootBridges[Index].NoExtendedConfigSpace;
      PciRootBridgeInfo->ResourceAssigned = PlatformPciRootBridges[Index].ResourceAssigned;
      PciRootBridgeInfo->RootBridge[Index].AllocationAttributes = PlatformPciRootBridges[Index].AllocationAttributes;
      PciRootBridgeInfo->RootBridge[Index].UID = Index;
      PciRootBridgeInfo->RootBridge[Index].HID = EISA_PNP_ID (0x0A03);

      Index++;
    }
    FreePool (PlatformPciRootBridges);
  }

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
  return Status;
}

/**
  Create Serial Port Info Hob that will be used by Universal Payload

  @retval EFI_SUCCESS             The function completes successfully
  @retval EFI_OUT_OF_RESOURCES    Could not allocate HOB.
**/
EFI_STATUS
CreateUniversalPayloadSerialPortInfo (
  VOID
  )
{
  UNIVERSAL_PAYLOAD_SERIAL_PORT_INFO   *Serial;
  EFI_STATUS                           Status;

  Status  = EFI_SUCCESS;
  Serial  = NULL;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  Serial = BuildGuidHob (&gUniversalPayloadSerialPortInfoGuid, sizeof (UNIVERSAL_PAYLOAD_SERIAL_PORT_INFO));
  if (Serial == NULL) {
    ASSERT (Serial != NULL);
    Status = EFI_OUT_OF_RESOURCES;
    return Status;
  }

  Serial->BaudRate        = PcdGet32 (PcdSerialBaudRate);
  Serial->RegisterBase    = PcdGet64 (PcdSerialRegisterBase);
  Serial->RegisterStride  = (UINT8) PcdGet32 (PcdSerialRegisterStride);
  Serial->Header.Revision = UNIVERSAL_PAYLOAD_SERIAL_PORT_INFO_REVISION;
  Serial->Header.Length   = sizeof (UNIVERSAL_PAYLOAD_SERIAL_PORT_INFO);
  Serial->UseMmio         = PcdGetBool (PcdSerialUseMmio);

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));

  return Status;
}

/**
  Create Acpi Board Info Hob that will be used by Universal Payload

  @retval EFI_SUCCESS             The function completes successfully
  @retval EFI_OUT_OF_RESOURCES    Could not allocate HOB.
**/
EFI_STATUS
CreateUniversalPayloadAcpiBoardInfo (
  VOID
  )
{
  ACPI_BOARD_INFO                 *AcpiBoardInfo;
  EFI_STATUS                      Status;

  Status         = EFI_SUCCESS;
  AcpiBoardInfo  = NULL;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  AcpiBoardInfo = BuildGuidHob (&gUefiAcpiBoardInfoGuid, sizeof (ACPI_BOARD_INFO));
  if (AcpiBoardInfo == NULL) {
    ASSERT (AcpiBoardInfo != NULL);
    Status = EFI_OUT_OF_RESOURCES;
    return Status;
  }

  AcpiBoardInfo->PcieBaseAddress = PcdGet64 (PcdPciExpressBaseAddress);
  AcpiBoardInfo->PcieBaseSize    = SIZE_256MB;
  AcpiBoardInfo->PmTimerRegBase  = FixedPcdGet32 (PcdAcpiIoPortBaseAddress) + FixedPcdGet32(PcdAcpiPm1TmrOffset);

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));

  return Status;
}

/**
  Create Debug Print Error Level Hob that will be used by Universal Payload

  @retval EFI_SUCCESS             The function completes successfully
  @retval EFI_OUT_OF_RESOURCES    Could not allocate HOB.
**/
EFI_STATUS
CreateUniversalPayloadDebugPrintErrorLevel (
  VOID
  )
{
  UEFI_PAYLOAD_DEBUG_PRINT_ERROR_LEVEL  *DebugPrintErrorLevel;
  EFI_STATUS                            Status;

  Status                = EFI_SUCCESS;
  DebugPrintErrorLevel  = NULL;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  DebugPrintErrorLevel = BuildGuidHob (&gEdkiiDebugPrintErrorLevelGuid, sizeof (UEFI_PAYLOAD_DEBUG_PRINT_ERROR_LEVEL));
  if (DebugPrintErrorLevel == NULL) {
    ASSERT (DebugPrintErrorLevel != NULL);
    Status = EFI_OUT_OF_RESOURCES;
    return Status;
  }

  DebugPrintErrorLevel->ErrorLevel      = GetDebugPrintErrorLevel ();
  DebugPrintErrorLevel->Header.Length   = sizeof (UEFI_PAYLOAD_DEBUG_PRINT_ERROR_LEVEL);
  DebugPrintErrorLevel->Header.Revision = UEFI_PAYLOAD_DEBUG_PRINT_ERROR_LEVEL_REVISION;

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));

  return Status;
}

/**
  Create Smbios30 Table Hob that will be used by Universal Payload

  @retval EFI_SUCCESS             The function completes successfully
  @retval EFI_OUT_OF_RESOURCES    Could not allocate HOB.
**/
EFI_STATUS
CreateUniversalPayloadSmbios30Table (
  VOID
  )
{
  UNIVERSAL_PAYLOAD_SMBIOS_TABLE   *SmbiosTable;
  SMBIOS_TABLE_3_0_ENTRY_POINT     Smbios30EntryPointStructureUplData;
  EFI_STATUS                       Status;

  Status       = EFI_SUCCESS;
  SmbiosTable  = NULL;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  ZeroMem (&Smbios30EntryPointStructureUplData, sizeof (SMBIOS_TABLE_3_0_ENTRY_POINT));
  CopyMem (&(Smbios30EntryPointStructureUplData.AnchorString), "_SM3_", sizeof ("_SM3_") - 1);
  Smbios30EntryPointStructureUplData.EntryPointLength            = sizeof (SMBIOS_TABLE_3_0_ENTRY_POINT);
  Smbios30EntryPointStructureUplData.EntryPointRevision          = 0x01;
  Smbios30EntryPointStructureUplData.MajorVersion                = (PcdGet16 (PcdSmbiosVersion) >> 8) & MAX_UINT8;
  Smbios30EntryPointStructureUplData.MinorVersion                = (PcdGet16 (PcdSmbiosVersion) & MAX_UINT8);
  Smbios30EntryPointStructureUplData.EntryPointStructureChecksum = CalculateCheckSum8 ((UINT8 *)&Smbios30EntryPointStructureUplData, Smbios30EntryPointStructureUplData.EntryPointLength);

  SmbiosTable = BuildGuidHob (&gUniversalPayloadSmbios3TableGuid, sizeof (UNIVERSAL_PAYLOAD_SMBIOS_TABLE) + sizeof (SMBIOS_TABLE_3_0_ENTRY_POINT));
  if (SmbiosTable == NULL) {
    ASSERT (SmbiosTable != NULL);
    Status = EFI_OUT_OF_RESOURCES;
    return Status;
  }

  SmbiosTable->Header.Revision  = UNIVERSAL_PAYLOAD_SMBIOS_TABLE_REVISION;
  SmbiosTable->Header.Length    = sizeof (UNIVERSAL_PAYLOAD_SMBIOS_TABLE) + sizeof (SMBIOS_TABLE_3_0_ENTRY_POINT);
  SmbiosTable->SmBiosEntryPoint = (EFI_PHYSICAL_ADDRESS)(UINTN)(&(SmbiosTable->SmBiosEntryPoint) + 1);
  CopyMem ((VOID *) (UINTN)SmbiosTable->SmBiosEntryPoint, &Smbios30EntryPointStructureUplData, sizeof (SMBIOS_TABLE_3_0_ENTRY_POINT));

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));

  return Status;
}

/**

  Create UPL FV that will be used by Universal Payload

**/
VOID
CreateUniversalPayloadFvUpl (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  DEBUG ((DEBUG_INFO, "Install FlashFvUpl - 0x%x, 0x%x\n", PcdGet32 (PcdFlashFvUplBase), PcdGet32 (PcdFlashFvUplSize)));
  PeiServicesInstallFvInfo2Ppi (
    &(((EFI_FIRMWARE_VOLUME_HEADER *) (UINTN) PcdGet32 (PcdFlashFvUplBase))->FileSystemGuid),
    (VOID *) (UINTN) PcdGet32 (PcdFlashFvUplBase),
    PcdGet32 (PcdFlashFvUplSize),
    NULL,
    NULL,
    0
    );

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));
}

/**
  Function to handle UPL at end of PEI

  @param[in]  PeiServices      Pointer to PEI Services Table.
  @param[in]  NotifyDesc       Pointer to the descriptor for the Notification event that
                               caused this function to execute.
  @param[in]  Ppi              Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
EFIAPI
UplEndOfPeiCallback (
  IN EFI_PEI_SERVICES                   **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDescriptor,
  IN VOID                               *Ppi
  )
{
  EFI_STATUS                 Status;

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  //
  // Create SerialPortInfo Hob.
  //
  Status = CreateUniversalPayloadSerialPortInfo ();
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Create AcpiBoardInfo Hob.
  //
  Status = CreateUniversalPayloadAcpiBoardInfo ();
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Create DebugPrintErrorLevel Hob.
  //
  Status = CreateUniversalPayloadDebugPrintErrorLevel ();
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Create SMBIOS 3.0 table Hob.
  //
  Status = CreateUniversalPayloadSmbios30Table ();
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Create Pci Root Bridge Hob.
  //
  Status = CreateUniversalPayloadPciRootBridge ();
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  //
  // Create ACPI Table Hob.
  //
  Status = CreateUniversalPayloadAcpiTable ();
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
  return Status;
}

EFI_PEI_NOTIFY_DESCRIPTOR  mUplEndOfPeiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  UplEndOfPeiCallback
};

/**
  Create Guid Hobs that will be used by Universal Payload

  @param[in] FileHandle        Handle of the file being invoked.
  @param[in] PeiServices       Pointer to PEI Services table.
**/
EFI_STATUS
EFIAPI
UplInitialization (
  IN       EFI_PEI_FILE_HANDLE FileHandle,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
{
  EFI_STATUS                  Status;

  Status = EFI_UNSUPPORTED;

  //
  // Create UPL FV
  //
  CreateUniversalPayloadFvUpl ();

  Status = PeiServicesNotifyPpi (&mUplEndOfPeiNotifyList);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
