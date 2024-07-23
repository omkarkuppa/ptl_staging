/** @file
  Silicon Acpi Module header file

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

#ifndef _ACPI_INIT_H_
#define _ACPI_INIT_H_

#include <PiPei.h>
#include <IndustryStandard/Acpi10.h>
#include <IndustryStandard/Acpi65.h>
#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiSpec.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <IndustryStandard/Acpi.h>
#include <Ppi/AcpiTablePpi.h>

//
// Private Driver Data
//
//
// ACPI Table Linked List Signature.
//
#define SILICON_ACPI_TABLE_LIST_SIGNATURE  SIGNATURE_32 ('S', 'A', 'T', 'L')

typedef UINT32 EFI_ACPI_TABLE_VERSION;

//
// ACPI Table Linked List Entry definition.
//
//  Signature must be set to EFI_ACPI_TABLE_LIST_SIGNATURE
//  Link is the linked list data.
//  Version is the versions of the ACPI tables that this table belongs in.
//  Table is a pointer to the table.
//  TableSize is the size of the table
//  Handle is used to identify a particular table.
//  PoolAllocation carries the allocation type:
//    FALSE: Table points to EFI_SIZE_TO_PAGES(TableSize) pages allocated using
//           gBS->AllocatePages ()
//    TRUE:  Table points to TableSize bytes allocated using gBS->AllocatePool ()
//
typedef struct {
  UINT32                    Signature;
  LIST_ENTRY                Link;
  EFI_ACPI_TABLE_VERSION    Version;
  EFI_ACPI_COMMON_HEADER    *Table;
  UINTN                     TableSize;
  UINTN                     Handle;
  BOOLEAN                   PoolAllocation;
} SILICON_ACPI_TABLE_LIST;

//
// Containment record for ACPI Table linked list.
//
#define SILICON_ACPI_TABLE_LIST_FROM_LINK(_link)  CR (_link, SILICON_ACPI_TABLE_LIST, Link, SILICON_ACPI_TABLE_LIST_SIGNATURE)


//
// Protocol private structure definition
//
//
// ACPI support protocol instance signature definition.
//
#define SILICON_ACPI_TABLE_SIGNATURE  SIGNATURE_32 ('S', 'T', 'A', 'S')

//
// ACPI support protocol instance data structure
//
typedef struct {
  UINTN                                           Signature;
  EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER    *Rsdp;               // Pointer to RSD_PTR structure
  EFI_ACPI_DESCRIPTION_HEADER                     *Rsdt;               // Pointer to RSDT table header
  EFI_ACPI_DESCRIPTION_HEADER                     *Xsdt;                // Pointer to XSDT table header
  EFI_ACPI_6_5_FIXED_ACPI_DESCRIPTION_TABLE       *Fadt;               // Pointer to FADT table header
  EFI_ACPI_6_5_FIRMWARE_ACPI_CONTROL_STRUCTURE    *Facs;               // Pointer to FACS table header
  EFI_ACPI_DESCRIPTION_HEADER                     *Dsdt;               // Pointer to DSDT table header
  LIST_ENTRY                                      TableList;
  UINTN                                           NumberOfTableEntries; // Number of ACPI tables
  UINTN                                           CurrentHandle;
  ACPI_TABLE_PPI                                  AcpiTablePpi;
} SILICON_ACPI_TABLE_INSTANCE;

//
// ACPI Silicon Hob Structure
//
typedef struct {
  EFI_PHYSICAL_ADDRESS    Rsdp;
  UINTN                   *MaxNumOfTable;
} ACPI_SILICON_HOB;

//
// Intilializes the ACPI table Instance and builds the Hob Structure.
//
EFI_STATUS
InitializeAcpiTableInstance (
  SILICON_ACPI_TABLE_INSTANCE *AcpiTableInstance
  );

//
// ACPI table instance containing record macro
//
#define SILICON_ACPI_TABLE_INSTANCE_FROM_THIS(a) \
  CR (a, \
      SILICON_ACPI_TABLE_INSTANCE, \
      AcpiTablePpi, \
      SILICON_ACPI_TABLE_SIGNATURE \
     )

#endif
