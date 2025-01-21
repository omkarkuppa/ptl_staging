/** @file
  LPIT table install and update driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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
#include <Uefi.h>
#include <Pi/PiFirmwareVolume.h>
#include <Pi/PiFirmwareFile.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/FirmwareVolume2.h>
#include <Lpit.h>

/**
  Install the LPIT table.

  @param[in] Table         Point to LPIT table.

  @retval EFI_SUCCESS      Install table success.
  @retval Other            The function encountered an error and could not complete process.
**/
EFI_STATUS
InstallTable (
  IN  EFI_ACPI_DESCRIPTION_HEADER *Table
  )
{
  EFI_STATUS              Status;
  EFI_ACPI_TABLE_PROTOCOL *mAcpiTable;
  UINTN                   TableKey;

  mAcpiTable = NULL;
  TableKey   = 0;

  //
  // Locate ACPI Table protocol.
  //
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &mAcpiTable);

  //
  //  Install table.
  //
  if (!EFI_ERROR (Status)) {
    Status = mAcpiTable->InstallAcpiTable (mAcpiTable, Table, Table->Length, &TableKey);
  }

  FreePool (Table);

  return Status;
}

/**
  This function will update several fields of LPI state entry in LPIT
  @param[in] Table      The table to update
  @param[in] Index      The entry index
  @param[in] Flags      The ACPI_LPI_STATE_FLAGS
  @param[in] Address    Residency counter address
  @param[in] Frequency  Residency counter frequency
**/
VOID
UpdLpiStat (
  IN ACPI_LOW_POWER_IDLE_TABLE *Table,
  IN UINT8                     Index,
  IN ACPI_LPI_STATE_FLAGS      Flags,
  IN UINT64                    Address,
  IN UINT64                    Frequency
  )
{
  if (!Table)
    return;

  Table->LpiStates[Index].Flags = Flags;
  Table->LpiStates[Index].ResidencyCounter.Address = Address;
  Table->LpiStates[Index].ResidencyCounterFrequency = Frequency;
}

/**
  Get PCD and update the values.

  @param[in, out] LpitTable     Point to Lpit table.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval Other                 The function encountered an error and could not complete process.
**/
EFI_STATUS
UpdateLpitTable (
  IN OUT EFI_ACPI_DESCRIPTION_HEADER *LpitTable
  )
{
  UINT8                LpitStateEntries = 0;
  UINT64               Address = 0;
  UINT64               ResidencyCounterFrequency = 0;
  ACPI_LPI_STATE_FLAGS Flags = {{ 0x0, 0x0, 0x0 }}; // Clear LPI State- Disabled bit

  CopyMem (LpitTable->OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (LpitTable->OemId));

  //
  // Set through patchable PCD value
  //
  LpitTable->OemTableId  = PcdGet64 (PcdAcpiDefaultOemTableId);
  LpitTable->OemRevision = PcdGet32(PcdAcpiDefaultOemRevision);

  //
  // Update creator information
  //
  LpitTable->CreatorId        = PcdGet32 (PcdAcpiDefaultCreatorId);
  LpitTable->CreatorRevision  = PcdGet32 (PcdAcpiDefaultCreatorRevision);

  LpitStateEntries = (UINT8)(((EFI_ACPI_DESCRIPTION_HEADER *)LpitTable)->Length - sizeof (EFI_ACPI_DESCRIPTION_HEADER)) / sizeof (ACPI_LPI_NATIVE_CSTATE_DESCRIPTOR);

  if (LpitStateEntries != 0) {
    LPI_COUNTER_VALID_STS Lpi_Counter;
    //
    // PcdLpitResidencyCounter : 1 - C10 Counter, 3 - SLP_S0 Based, 7 - ATX Shutdown/PS_ON
    // Expectation is
    // if "C10" is set in BIOS setup , only C10 table should be valid
    // if "SLP S0" is set in BIOS setup, both C10 and SLP S0 should be valid
    // if "ATX Shutdown/PS_ON" is set in BIOS setup, both C10 ,SLP S0, PSON should be valid
    //
    Lpi_Counter.Data8 = PcdGet8 (PcdLpitResidencyCounter);

    if (Lpi_Counter.Bits.C10) {
      Address = (UINT64)PcdGet32 (PcdMsrPc10Rcntr);
      ResidencyCounterFrequency = 0; //Counter runs at TSC frequency
      UpdLpiStat ((ACPI_LOW_POWER_IDLE_TABLE *)LpitTable, 0, Flags, Address, ResidencyCounterFrequency);
    }

    if (Lpi_Counter.Bits.Slp_S0) {
      //
      // Read PWRM Base Address to fill in Residency counter Address Space
      //
      Address = PcdGet64 (PcdPmcPwrmBase) + PcdGet32 (PcdPmcSlpS0Residency);
      ResidencyCounterFrequency = 8197;  //Counter runs at 122us granularity which implies 10KHz frequency (8197Hz)
      UpdLpiStat ((ACPI_LOW_POWER_IDLE_TABLE *)LpitTable, 1, Flags, Address, ResidencyCounterFrequency);
    }

    if (Lpi_Counter.Bits.Ps_On) {
      //
      // Read PWRM Base Address to fill in Residency counter Address Space
      //
      Address = PcdGet64 (PcdPmcPwrmBase) + PcdGet32 (PcdPmcPsOnResidency);
      ResidencyCounterFrequency = 10000; //Counter runs at 100us granularity which implies 10KHz frequency (10000Hz)
      UpdLpiStat ((ACPI_LOW_POWER_IDLE_TABLE *)LpitTable, 2, Flags, Address, ResidencyCounterFrequency);
    }
  }

  return EFI_SUCCESS;
}

/**
  Find LPIT Table.

  @param[out] LpitTable     Point to LPIT table.

  @retval EFI_SUCCESS       Find LPIT table successfully.
  @retval Other             The function encountered an error and could not complete process.
**/
EFI_STATUS
FindLpitTable (
  OUT EFI_ACPI_DESCRIPTION_HEADER **LpitTable
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    *HandleBuffer;
  UINTN                         NumberOfHandles;
  UINTN                         Index;
  INTN                          Instance;
  UINTN                         Size;
  UINT32                        FvStatus;
  EFI_FV_FILETYPE               FileType;
  EFI_FV_FILE_ATTRIBUTES        Attributes;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol;
  EFI_ACPI_COMMON_HEADER        *Table;

  Instance = 0;
  FwVol    = NULL;
  Table    = NULL;

  //
  // Locate FV protocol.
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );

  //
  // Look for FV with ACPI storage file
  //
  for (Index = 0; Index < NumberOfHandles; Index++) {
    //
    // Get the protocol on this handle
    //
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiFirmwareVolume2ProtocolGuid,
                    (VOID **) &FwVol
                    );
    if (FwVol == NULL) {
      return EFI_NOT_FOUND;
    }

    //
    // See if it has the ACPI storage file
    //
    Size      = 0;
    FvStatus  = 0;
    Status = FwVol->ReadFile (
                      FwVol,
                      &gEfiCallerIdGuid,
                      NULL,
                      &Size,
                      &FileType,
                      &Attributes,
                      &FvStatus
                      );
    if (!EFI_ERROR (Status)) {
      break;
    }
  }
  //
  // Free any allocated buffers
  //
  FreePool (HandleBuffer);

  //
  // Check that we found the data file
  //
  if (FwVol == NULL) {
    return EFI_NOT_FOUND;
  }

  //
  // Read tables from the storage file.
  //
  while (Status == EFI_SUCCESS) {
    //
    // Read the ACPI tables
    //
    Status = FwVol->ReadSection (
                      FwVol,
                      &gEfiCallerIdGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      (VOID **) &Table,
                      &Size,
                      &FvStatus
                      );
    if (!EFI_ERROR (Status)) {
      *LpitTable = (EFI_ACPI_DESCRIPTION_HEADER *) Table;

      if ((*LpitTable)->Signature == EFI_ACPI_6_5_LOW_POWER_IDLE_TABLE_STRUCTURE_SIGNATURE) {
        return Status;
      }

      //
      // Increase the instance
      //
      Instance++;
      Table = NULL;
    }
  }
  return Status;
}

/**
  LPIT table installation driver.

  @param[in] ImageHandle     Handle for this drivers loaded image protocol.
  @param[in] SystemTable     EFI system table.

  @retval EFI_SUCCESS        The driver installed without error.
  @retval Other              The driver encountered an error and could not complete installation of
                             the ACPI table.

**/
EFI_STATUS
EFIAPI
InstallLpitTableDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                  Status;
  EFI_ACPI_DESCRIPTION_HEADER *LpitTable;

  LpitTable = NULL;

  //
  // Find default LPIT table.
  //
  Status = FindLpitTable (&LpitTable);

  //
  // Update LPIT table.
  //
  if ( (!EFI_ERROR (Status)) && (LpitTable != NULL)) {
    Status = UpdateLpitTable (LpitTable);

    //
    // Install LPIT table.
    //
    if ( (!EFI_ERROR (Status))) {
      Status = InstallTable (LpitTable);
    }
  }

  return Status;
}
