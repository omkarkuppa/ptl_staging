/** @file
  CrashLog DXE driver

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
  Advanced Configuration and Power Interface (ACPI) Specification, version 6.5
**/

#include "CrashLogDxe.h"

EFI_GUID gCrashLogRecordIdGuid = CRASHLOG_RECORDID_GUID;

/**
  Set BootErrorRegionLength in Bert table.

  @param[in] Bert  APEI BERT Instance.

**/
VOID
SetBootErrRegionLen (
  IN EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  *Bert
  )
{
  EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE  *ErrStsBlk;

  if (Bert == NULL || Bert->BootErrorRegion == 0) {
    return ;
  }

  ErrStsBlk = (EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE *)(UINTN)Bert->BootErrorRegion;
  Bert->BootErrorRegionLength = ErrStsBlk->DataLength + sizeof (EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE);
  DEBUG ((DEBUG_INFO, " BootErrorRegionLength =0x%x, ErrorDataEntryCount =%d\n",
    Bert->BootErrorRegionLength,
    ErrStsBlk->BlockStatus.ErrorDataEntryCount)
  );
}

/**
  Install ACPI table for APEI BERT

  @param[in] AcpiTbl  Acpi table protocol instance.
  @param[in] Bert     APEI BERT Instance.

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
InstallApeiBertTables (
  IN EFI_ACPI_TABLE_PROTOCOL                      *AcpiTbl,
  IN EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  *Bert
  )
{
  EFI_STATUS                                   Status;
  UINTN                                        TableKey;

  if ((AcpiTbl == NULL) || (Bert == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  SetBootErrRegionLen (Bert);

  DEBUG ((DEBUG_INFO, "APEI Support. Install BERT Table \n"));
  TableKey = 0;
  Status = AcpiTbl->InstallAcpiTable (
                      AcpiTbl,
                      Bert,
                      Bert->Header.Length,
                      &TableKey
                      );

  DEBUG ((DEBUG_INFO, "Install ACPI table for APEI BERT Status: %r\n", Status));

  return Status;
}

/**

  Update Platform & OEM ID's in Whea ACPI tables

  @param[in,out] TableHeader  BERT Table Header Pointer

**/
VOID
UpdateAcpiTableIds (
  IN OUT EFI_ACPI_DESCRIPTION_HEADER  *TableHeader
  )
{
  //
  // Update the OEMID, OEM revision, OEM TABLE ID, Creator ID and Creator revision
  //
  *(UINT32 *)(TableHeader->OemId) = 'I' + ('N' << 8) + ('T' << 16) + ('E' << 24);
  *(UINT16 *)(TableHeader->OemId + 4) = 'L' + (' ' << 8);

  TableHeader->OemTableId = EFI_ACPI_OEM_TABLE_ID;
  TableHeader->OemRevision = EFI_WHEA_OEM_REVISION;
  TableHeader->CreatorId = EFI_ACPI_CREATOR_ID;
  TableHeader->CreatorRevision = EFI_ACPI_CREATOR_REVISION;
}

/**
  Check whether the size is enough to insert record.

  @param[in] BertTable  BERT table instance.
  @param[in] DataSize   data size.

  @retval EFI_SUCCESS            Size check pass.
  @retval EFI_INVALID_PARAMETER  Parameters is not valid.
  @retval EFI_OUT_OF_RESOURCES   Memory is not enough to insert new record.

**/
EFI_STATUS
CheckBootErrRegionSize (
  IN EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  *BertTable,
  IN UINT32                                       DataSize
  )
{
  EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE  *ErrStsBlk;

  if ((BertTable == NULL) ||
      (BertTable->BootErrorRegion == 0) ||
      (DataSize == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  ErrStsBlk = (EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE *)(UINTN)BertTable->BootErrorRegion;
  if ((ErrStsBlk->DataLength >= BertTable->BootErrorRegionLength) ||
    (ErrStsBlk->DataLength + DataSize > BertTable->BootErrorRegionLength)) {
    return EFI_OUT_OF_RESOURCES;
  }

  return EFI_SUCCESS;
}

/**

  Add a new Firmware CrashLog Entry to existing Error Status Block.

  @param[in] ErrStsBlk                        BERT GENERIC_ERROR_STATUS instance.
  @param[in,out] FirmwareGenericErrorAddr     Pointer to this FirmwareGenericError entry address, updated on return.
  @param[in,out] FirmwareCrashLogPayloadAddr  Pointer to Firmware CrashLog Entry payload address, updated on return.
  @param[in] EntrySize                        Firmware CrashLog Entry payload size.
  @param[in] EntryVersion                     Firmware CrashLog Entry version.

**/
VOID
AddFirmwareErrorEntry (
  IN     EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE  *ErrStsBlk,
  IN OUT FIRMWARE_GENERIC_ERROR                       **FirmwareGenericErrorAddr,
  IN OUT UINT8                                        **FirmwareCrashLogPayloadAddr,
  IN     UINT32                                       EntrySize,
  IN     EFI_GUID                                     RecordIdGuid
  )
{
  FIRMWARE_GENERIC_ERROR  *FirmwareGenericError = NULL;

  FirmwareGenericError = (FIRMWARE_GENERIC_ERROR *)((UINT8 *)(VOID *)ErrStsBlk + ErrStsBlk->DataLength + sizeof (EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE));
  ZeroMem (FirmwareGenericError, sizeof (FIRMWARE_GENERIC_ERROR));

  CopyMem (&FirmwareGenericError->GenericErrorDataEntry.SectionType, &gEfiFirmwareErrorSectionGuid, sizeof (EFI_GUID));
  FirmwareGenericError->GenericErrorDataEntry.ErrorSeverity = EFI_ACPI_6_5_ERROR_SEVERITY_FATAL;
  FirmwareGenericError->GenericErrorDataEntry.Revision = EFI_ACPI_6_5_GENERIC_ERROR_DATA_ENTRY_REVISION;
  FirmwareGenericError->GenericErrorDataEntry.ValidationBits = 0;
  FirmwareGenericError->GenericErrorDataEntry.ErrorDataLength = EntrySize + sizeof (EFI_FIRMWARE_ERROR_DATA);
  FirmwareGenericError->EfiFirmwareErrorData.ErrorType = 2; //FW CrashLog Error Record
  FirmwareGenericError->EfiFirmwareErrorData.Revision = EFI_2_7_FIRMWARE_ERROR_RECORD_REVISION;
  FirmwareGenericError->EfiFirmwareErrorData.RecordIdGuid = RecordIdGuid;

  ErrStsBlk->DataLength += FirmwareGenericError->GenericErrorDataEntry.ErrorDataLength + sizeof (EFI_ACPI_6_5_GENERIC_ERROR_DATA_ENTRY_STRUCTURE);
  ErrStsBlk->BlockStatus.ErrorDataEntryCount++;

  *FirmwareGenericErrorAddr = FirmwareGenericError;
  *FirmwareCrashLogPayloadAddr = (UINT8 *)(VOID *)FirmwareGenericError + sizeof (FIRMWARE_GENERIC_ERROR);
}

/**
  Insert data to the APEI BERT GENERIC_ERROR_STATUS structure.

  @param[in] BertTable  BERT table instance
  @param[in] DataAddr   The addess of data
  @param[in] DataSize   data size

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
InsertGenericErrorData (
  IN  EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  *BertTable,
  IN  UINT64                                       DataAddr,
  IN  UINT32                                       DataSize
  )
{
  EFI_STATUS                                   Status;
  FIRMWARE_GENERIC_ERROR                       *GenericErrorData;
  UINT8                                        *PayloadAddr;
  EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE  *ErrStsBlk;

  GenericErrorData = NULL;
  PayloadAddr      = NULL;

  Status = CheckBootErrRegionSize (BertTable, DataSize);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ErrStsBlk = (EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE *)(UINTN)BertTable->BootErrorRegion;
  AddFirmwareErrorEntry (ErrStsBlk, &GenericErrorData, &PayloadAddr, DataSize, gCrashLogRecordIdGuid);
  CopyMem (PayloadAddr, (VOID *)(UINTN)DataAddr, DataSize);
  DEBUG ((DEBUG_INFO, "Add data to GEDE, GEDEentry= 0x%x, PayloadAddr = 0x%x, PayloadSize= %d bytes\n", GenericErrorData, PayloadAddr, DataSize));

  return EFI_SUCCESS;
}

/**
  Initialize ACPI descriptor header.

  @param[in] BertPtr  pointer of boot error record table header.

**/
VOID
InitAcpiDescHeader (
  IN EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  *BertPtr
  )
{
  if (BertPtr != NULL) {
    BertPtr->Header.Signature = EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_SIGNATURE;
    BertPtr->Header.Length    = sizeof (EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER);
    BertPtr->Header.Revision  = EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_REVISION;
    BertPtr->Header.Checksum  = 0;
    UpdateAcpiTableIds (&(BertPtr->Header));
  }
}

/**
  Initialize boot error status block header.

  @param[in] ErrStsBlk  pointer of generic error status structure.

**/
VOID
InitErrStatusHeader (
  IN EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE  *ErrStsBlk
  )
{
  if (ErrStsBlk != NULL) {
    ErrStsBlk->RawDataOffset = 0;
    ErrStsBlk->RawDataLength = 0;
    ErrStsBlk->ErrorSeverity = EFI_ACPI_6_5_ERROR_SEVERITY_FATAL;
    ErrStsBlk->BlockStatus.ErrorDataEntryCount = 0;
    ErrStsBlk->BlockStatus.MultipleUncorrectableErrors = 1;
    ErrStsBlk->BlockStatus.UncorrectableErrorValid = 1;
    ErrStsBlk->DataLength = 0;
  }
}

/**
  Initialize boot error status block.

  @param[in] BertPtr  pointer of boot error record table header.
  @param[in] Size     Total size of all records.
  @param[in] Count    Count of crashlog records.

  @retval EFI_SUCCESS            The function completes successfully
  @retval EFI_INVALID_PARAMETER  Parameters is not valid.
  @retval EFI_OUT_OF_RESOURCES   Memory is not enough.
**/
EFI_STATUS
InitErrorStatusBlk (
  IN EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  *BertPtr,
  IN UINT32                                       Size,
  IN UINT32                                       Count
  )
{
  UINT32  BootErrorRegionSize;

  if (BertPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  BootErrorRegionSize = sizeof (EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE) + sizeof (FIRMWARE_GENERIC_ERROR) * Count + Size;
  BertPtr->BootErrorRegionLength = BootErrorRegionSize;
  BertPtr->BootErrorRegion = (UINT64) AllocateReservedZeroPool ((UINTN) BootErrorRegionSize);

  DEBUG ((DEBUG_INFO, "BertPtr->BootErrorRegion     = %x\n", (UINTN)BertPtr->BootErrorRegion));
  DEBUG ((DEBUG_INFO, "BertPtr->BootErrorRegionLength = %x\n", BootErrorRegionSize));

  if (BertPtr->BootErrorRegion == 0) {
    DEBUG ((DEBUG_ERROR, "Unable to Allocate Memory for Boot Error Record Data\n"));
    FreePool (BertPtr);
    return EFI_OUT_OF_RESOURCES;
  }

  InitErrStatusHeader ((EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE *)(UINTN)BertPtr->BootErrorRegion);

  return EFI_SUCCESS;
}

/**
  Initialize the APEI BERT EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER structure.

  @param[in,out] BertTable  point of the BERT EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER instance.
  @param[in] Size           Total size of all records.
  @param[in] Count          Count of crashlog record.

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
InitBertTblHeader (
  IN OUT EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  **BertTable,
  IN     UINT32                                       Size,
  IN     UINT32                                       Count
  )
{
  EFI_STATUS                                   Status;
  EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  *BertPtr;

  //
  // Allocate memory for BERT table
  //
  BertPtr = (EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER *)AllocateZeroPool (sizeof (EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER));

  if (BertPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "Unable to Allocate Memory for BERT Record Creation\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  InitAcpiDescHeader (BertPtr);

  Status = InitErrorStatusBlk (BertPtr, Size, Count);

  *BertTable = BertPtr;

  return Status;
}

/**
  Dump bert for debug.

  @param[in] Bert  point of the BERT EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER instance.

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
DumpBert (
  IN EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  *Bert
  )
{
  UINT32                  Index;
  UINT32                  *DataPtr;
  UINT32                  Size;
  FIRMWARE_GENERIC_ERROR  *GEDE;

  if (Bert == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "============================================================\n"));
  DEBUG ((DEBUG_INFO, "|---------------------CrashLog Bert dump-------------------|\n"));
  DEBUG ((DEBUG_INFO, "============================================================\n"));
  DEBUG ((DEBUG_INFO, "|--------+++--------------Bert header--------------+++-----|\n"));
  DEBUG ((DEBUG_INFO, "============================================================"));

  DataPtr = (UINT32 *)(VOID *)Bert;
  for (Index = 0; Index < sizeof (EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER); Index += 4, DataPtr ++) {
    if ((Index % 4) == 0) {
     DEBUG ((DEBUG_INFO, "\n"));
    }
    DEBUG ((DEBUG_INFO, "0x%08x   ", *DataPtr));
  }
  DEBUG ((DEBUG_INFO, "\n============================================================\n"));
  DEBUG ((DEBUG_INFO, "|--------+++-----------Boot Error region-----------+++-----|\n"));
  DEBUG ((DEBUG_INFO, "============================================================"));

  DataPtr = (UINT32 *)(UINTN)Bert->BootErrorRegion;
  for (Index = 0; Index < sizeof (EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE); Index += 4, DataPtr ++) {
    if ((Index % 4) == 0) {
      DEBUG ((DEBUG_INFO, "\n"));
    }
    DEBUG ((DEBUG_INFO, "0x%08x   ", *DataPtr));
  }

  DEBUG ((DEBUG_INFO, "\n============================================================\n"));
  DEBUG ((DEBUG_INFO, "|--------+++--------------GEDE entry---------------+++-----|\n"));
  DEBUG ((DEBUG_INFO, "============================================================"));

  GEDE = (FIRMWARE_GENERIC_ERROR *)(UINTN)(Bert->BootErrorRegion + sizeof (EFI_ACPI_6_5_GENERIC_ERROR_STATUS_STRUCTURE));
  DataPtr = (UINT32 *) GEDE;
  Size = GEDE->GenericErrorDataEntry.ErrorDataLength + sizeof (EFI_ACPI_6_5_GENERIC_ERROR_DATA_ENTRY_STRUCTURE);
  for (Index = 0; Index < Size; Index += 4, DataPtr ++) {
    if ((Index % 4) == 0) {
      DEBUG ((DEBUG_INFO, "\n"));
    }
    DEBUG ((DEBUG_INFO, "0x%08x   ", *DataPtr));
  }

  DEBUG ((DEBUG_INFO, "\n============================================================\n"));

  return EFI_SUCCESS;
}
