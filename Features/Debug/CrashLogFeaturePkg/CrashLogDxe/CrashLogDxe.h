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

#ifndef _CRASH_LOG_DXE_H_
#define _CRASH_LOG_DXE_H_

//
// Statements that include other header files
//
#include <Uefi.h>
#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <IndustryStandard/Acpi.h>
#include <Protocol/AcpiTable.h>
#include <Library/HobLib.h>
#include <Guid/Cper.h>

#include <Guid/CrashLogHob.h>
#include <BertTable.h>

//
// Crashlog Dxe driver private structure
//
typedef struct {
  EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER    *Bert;
  EFI_ACPI_TABLE_PROTOCOL                        *AcpiTblProtocol;
  CRASHLOG_HOB_DATA                              **HobDataPtr;
  UINT32                                         HobCount;
  UINT32                                         PayloadSize;
  UINT32                                         PayloadCount;
} CRASHLOG_DXE_PRIVATE_DATA;


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
  );

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
  );

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
  );

/**
  Dump bert for debug.

  @param[in] Bert  point of the BERT EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER instance.

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
DumpBert (
  IN EFI_ACPI_6_5_BOOT_ERROR_RECORD_TABLE_HEADER  *Bert
  );

#endif
