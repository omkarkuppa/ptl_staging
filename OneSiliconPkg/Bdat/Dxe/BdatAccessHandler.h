/** @file
  This code makes the BIOS Data structure available via standard ACPI mechanisms.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#ifndef _BDAT_ACCESS_HANDLER_H_
#define _BDAT_ACCESS_HANDLER_H_
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Guid/HobList.h>
#include <IndustryStandard/Acpi30.h>
#include <Protocol/AcpiTable.h>
#include <BdatFactory.h>

#include <Library/PciSegmentLib.h>
#include <Protocol/BdatAccess.h>
///
/// Ensure proper structure formats
///
#pragma pack(push, 1)

//
//
#define MAX_SCHEMA_LIST_LENGTH (10)

/**
  Not strictly part of the BDAT spec.  This HOB is used to identify which HOBs
  contain data that should be copied into the final BDAT structure as a schema
**/
typedef struct {
  EFI_HOB_GUID_TYPE           HobGuidType;
/**
  SchemaHobCount : Number of HOBs that exist which contain data that should be
                   copied to the BDAT structure.
**/
  UINT16                      SchemaHobCount;
  UINT16                      Reserved;
/**
  SchemaHobGuids : Array of GUIDs which identify HOBs that contain data
                   that should be copied into the BDAT structure.  The contents
                   of the HOB contains a schema.  The GUID for the HOB is the
                   same as the SchemaId GUID.
**/
  EFI_GUID                    SchemaHobGuids[MAX_SCHEMA_LIST_LENGTH];
} BDAT_SCHEMA_LIST_HOB;

/**
  Entry point of the Bdat table factory.

  @param[in] HobList       : A pointer to the HOB list
  @param[in] AcpiTable     : A pointer to ACPI table

  @retval EFI_SUCCESS:              Driver initialized successfully
  @exception EFI_UNSUPPORTED:       A needed driver was not located
  @retval EFI_OUT_OF_RESOURCES:     Could not allocate needed resources
**/
EFI_STATUS
CreateBdatTable (
  IN VOID                            *HobList,
  IN EFI_ACPI_TABLE_PROTOCOL         *AcpiTable
  );


/**
  <b>Entry point of the Bdat Access Handler</b>
  - <b>Introduction</b> \n
    This DXE driver will install an ACPI table which presents BDAT structure provided by MRC.

  - @pre
    - _EFI_ACPI_TABLE_PROTOCOL

  - @result
    Creates the Compatible BIOS data (BDAT) table in the ACPI tables and copies the memory related timing and configuration information into the table.

  @param[in] ImageHandle   EFI_HANDLE: A handle for the image that is initializing this driver
  @param[in] SystemTable   EFI_SYSTEM_TABLE: A pointer to the EFI system table

  @retval EFI_SUCCESS:              Driver initialized successfully
  @exception EFI_UNSUPPORTED:       A needed driver was not located
  @retval EFI_OUT_OF_RESOURCES:     Could not allocate needed resources
**/
EFI_STATUS
EFIAPI
BdatAccessHandler (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  );

///
/// This is copied from Include\Acpi.h
///
#define CREATOR_ID_INTEL  0x4C544E49  /// "LTNI""INTL"(Intel)
#define CREATOR_REV_INTEL 0x20090903

#pragma pack(pop)

#endif /// _BDAT_ACCESS_HANDLER_H_
