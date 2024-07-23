/** @file
  File to support Bdatlist
  Entry Point for Bdatlist EFI application

@copyright
 Copyright (c) 2024 Intel Corporation. All rights reserved
 This software and associated documentation (if any) is furnished
 under a license and may only be used or copied in accordance
 with the terms of the license. Except as permitted by the
 license, no part of this software or documentation may be
 reproduced, stored in a retrieval system, or transmitted in any
 form or by any means without the express written consent of
 Intel Corporation.
 This file contains an 'Intel Peripheral Driver' and is uniquely
 identified as "Intel Reference Module" and is licensed for Intel
 CPUs and chipsets under the terms of your license agreement with
 Intel or your vendor. This file may be modified by the user, subjEct
 to additional terms of the license agreement.

@par Specification Reference:
**/
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/ShellLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <IndustryStandard/Acpi30.h>
#include <Guid/Acpi.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Bdat4.h>
#include <CMrcRmtData.h>

#define MAJOR_VERSION   1
#define MINOR_VERSION   0

STATIC CONST SHELL_PARAM_ITEM BdatDumpParamList[] = {
  {L"-h", TypeFlag},
  {L"-H", TypeFlag},
  {L"-v", TypeFlag},
  {L"-V", TypeFlag},
  {NULL, TypeMax}
};

/**
   Display current version.
**/
static
VOID
ShowVersion (
  )
{
  ShellPrintEx (-1, -1, L"Bdatdump: Version %d.%02d\n", MAJOR_VERSION, MINOR_VERSION);
}

static
VOID
ShowHelp (
  )
{
  ShellPrintEx (-1, -1, L"Bdatdump\n");
}

/**
 * main entry of module
 *
 * @param ImageHandle
 * @param SystemTable
 *
 * @return EFI_STATUS EFIAPI
 */
EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  LIST_ENTRY                                    *ParamPackage;
  SHELL_STATUS                                  Status;
  UINT8                                         *Raw;
  UINT32                                        Index;
  EFI_CONFIGURATION_TABLE                       *Ect = SystemTable->ConfigurationTable;
  EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER  *Rsdp = NULL;
  EFI_ACPI_SDT_HEADER                           *Xsdt;
  EFI_ACPI_SDT_HEADER                           *Entry;
  UINT32                                        EntryCount;
  UINT64                                        *EntryPtr;
  UINT32                                        XsdtIndex;
  /*
  UINT32                                        *SchemaOffsetList;
  UINT32                                        SchemaListIndex;
  UINT8                                         *Schema;
  */
  BDAT_MEMORY_DATA_STRUCTURE                    *MemorySchema;
  BDAT_STRUCTURE                                *Bdat;
  BASE_PPR_RESULT                               *PprResult;
  PPR_RESULT_METADATA                           *PprResultMeta;
  PPR_RESULT_COLUMNS                            *PprResultColumns;

  //
  // initialize the shell lib (we must be in non-auto-init...)
  //
  Status = ShellInitialize ();
  ASSERT_EFI_ERROR (Status);

  ParamPackage = NULL;

  //
  // parse the command line
  //
  Status = ShellCommandLineParse (BdatDumpParamList, &ParamPackage, NULL, TRUE);
  if (EFI_ERROR (Status)) {
    Status = SHELL_INVALID_PARAMETER;
    goto Done;
  }

  //
  // check for "-?" help information, and for "-v" for version inforamtion.
  //
  if (ShellCommandLineGetFlag (ParamPackage, L"-h")  ||
      ShellCommandLineGetFlag (ParamPackage, L"-H")) {
    ShowHelp ();
    goto Done;
  }

  if (ShellCommandLineGetFlag (ParamPackage, L"-v") ||
             ShellCommandLineGetFlag (ParamPackage, L"-V")) {
    ShowVersion ();
    goto Done;
  }

  // List RMT BDAT info

  // List PPR BDAT info
 
  Raw = NULL;

  MemorySchema = NULL;
  // locate RSDP (Root System Description Pointer)
  for (Index = 0; (Index < SystemTable->NumberOfTableEntries) && (MemorySchema == NULL); Index++) {
    Ect = &SystemTable->ConfigurationTable[Index];
    if (!CompareGuid (&(Ect->VendorGuid), &gEfiAcpi20TableGuid)) {
      continue;
    }

    if (Ect->VendorTable == 0) {
      continue;
    }
    if (AsciiStrnCmp("RSD PTR ", (CHAR8 *)(Ect->VendorTable), 8)) {
      continue;
    }

    //Guid2String(GuidStr, &(SystemTable->ConfigurationTable[Index].VendorGuid));
    Rsdp = (EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER *)Ect->VendorTable;
    if (Rsdp->Revision < EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER_REVISION) {
      continue;
    }

    Xsdt = (EFI_ACPI_SDT_HEADER *)(VOID *)(Rsdp->XsdtAddress);
    if (AsciiStrnCmp("XSDT", (CHAR8 *)(VOID *)(&Xsdt->Signature), 4)) {
      continue;
    }

    EntryCount = (Xsdt->Length - sizeof (EFI_ACPI_SDT_HEADER)) / sizeof(UINT64);
    EntryPtr = (UINT64 *)(Xsdt + 1);

    for (XsdtIndex = 0; XsdtIndex < EntryCount; XsdtIndex++, EntryPtr++) {
      Entry = (EFI_ACPI_SDT_HEADER *)((UINTN)(*EntryPtr));
      if (EFI_BDAT_TABLE_SIGNATURE != Entry->Signature) {
        continue;
      }

      Raw = (VOID *) Entry;
      Raw = (VOID *) ((EFI_BDAT_ACPI_DESCRIPTION_TABLE *) Raw)->BdatGas.Address;
      Bdat = (BDAT_STRUCTURE *) Raw;
      //SchemaOffsetList = (UINT32*) (((UINTN) Raw) + sizeof (BDAT_STRUCTURE));

      if ((Bdat->Schemas.SchemaListLength == 0) || (Bdat->Schemas.SchemaListLength > 10000)) {
        // Avoid Coverity checker failure: [TAINTED_SCALAR] Using tainted variable "Bdat->Schemas.SchemaListLength" as a loop boundary.
        break;
      }

      /*
      for (SchemaListIndex = 0; SchemaListIndex < Bdat->Schemas.SchemaListLength; SchemaListIndex++) {
        Schema = (UINT8 *) (VOID *) ((SchemaOffsetList[SchemaListIndex]) + ((UINT64)(VOID *) Bdat));
        if (CompareGuid ((EFI_GUID *)&((BDAT_MEMORY_DATA_STRUCTURE *) Schema)->SchemaHeader.SchemaId, &gSsaBiosResultPprGuid)) {
          MemorySchema = (BDAT_MEMORY_DATA_STRUCTURE *) Schema;
          break;
        }
      }
      */
      break;
    }
  }

  if (MemorySchema == NULL) {
    ShellPrintEx ( -1, -1, L"Cannot find Post Package Repair Bdat info\n");
    goto Done;
  }

  PprResult         = (BASE_PPR_RESULT *) ((VOID *) &MemorySchema->RMT_RESULTS_WITH_META_COLUMNS);
  PprResultMeta     = &PprResult->Metadata;
  PprResultColumns  = PprResult->Rows;

  ShellPrintEx ( -1, -1, L"List Post Package Repair Bdat info:\n");
  ShellPrintEx ( -1, -1, L"MajorVer:%d MinorVer:%d\n", PprResultMeta->MajorVer, PprResultMeta->MinorVer);
  ShellPrintEx ( -1, -1, L"TotalRowFailuresDiscovered:%d\nTotalPprRowRepairsSuccessful:%d\nTotalPprRowRepairsAttemptCnt:%d\nTotalUncorrEctableDevices:%d\n", PprResultMeta->TotalRowFailuresDiscovered, PprResultMeta->TotalPprRowRepairsSuccessful, PprResultMeta->TotalPprRowRepairsAttemptCnt, PprResultMeta->TotalUncorrectableDevices);

  ShellPrintEx ( -1, -1, L"Error Row info:\n");
  for (Index = 0; Index < PprResultMeta->ResultEleCount; Index++) {
    ShellPrintEx ( -1, -1, L"\tController %d, Channel %d, Rank %d, BankGroup %d, Bank %02d, Row 0x%X, Repair %s, AttemptCount %d\n",
        PprResultColumns->Controller,
        PprResultColumns->Channel,
        PprResultColumns->Rank,
        PprResultColumns->BankGroup,
        PprResultColumns->Bank,
        PprResultColumns->Row,
        PprResultColumns->PprRowRepairsSuccessful == 1 ? L"succeeded" : L"failed",
        PprResultColumns->PprRowRepairsAttemptCnt);
    PprResultColumns++;
  }

Done:
  //
  // free the command line package
  // map to ShellCommandLineParse
  //
  if (ParamPackage != NULL) {
    ShellCommandLineFreeVarList (ParamPackage);
    ParamPackage = NULL;
  }

  return Status;
}

