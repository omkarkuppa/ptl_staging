/** @file
  Silicon Acpi PPI module

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

#ifndef _ACPI_TABLE_PPI_H_
#define _ACPI_TABLE_PPI_H_

#define ACPI_TABLE_PPI_GUID \
  { 0xffe06bdd, 0x6107, 0x46a6, { 0x7b, 0xb2, 0x5a, 0x9c, 0x7e, 0xc5, 0x27, 0x5c }}

typedef struct _ACPI_TABLE_PPI ACPI_TABLE_PPI;

/**
  The InstallAcpiTable() function allows a caller to install an
  ACPI table. When successful, the table will be linked by the
  RSDT/XSDT. AcpiTableBuffer specifies the table to be installed.
  InstallAcpiTable() will make a copy of the table and insert the
  copy into the RSDT/XSDT. InstallAcpiTable() must insert the new
  table at the end of the RSDT/XSDT. To prevent namespace
  collision, ACPI tables may be created using UEFI ACPI table
  format. If this protocol is used to install a table with a
  signature already present in the system, the new table will not
  replace the existing table. It is a platform implementation
  decision to add a new table with a signature matching an
  existing table or disallow duplicate table signatures and
  return EFI_ACCESS_DENIED. On successful output, TableKey is
  initialized with a unique key. Its value may be used in a
  subsequent call to UninstallAcpiTable to remove an ACPI table.
  If an EFI application is running at the time of this call, the
  relevant EFI_CONFIGURATION_TABLE pointer to the RSDT is no
  longer considered valid.
  @param PeiServices          General purpose services available to every PEIM.
  @param This                 A pointer to a ACPI_TABLE_PPI.
  @param AcpiTableBuffer      A pointer to a buffer containing the
                              ACPI table to be installed.
  @param AcpiTableBufferSize  Specifies the size, in bytes, of
                              the AcpiTableBuffer buffer.
  @param TableKey             Returns a key to refer to the ACPI table.
  @retval EFI_SUCCESS           The table was successfully inserted
  @retval EFI_INVALID_PARAMETER Either AcpiTableBuffer is NULL,
                                TableKey is NULL, or
                                AcpiTableBufferSize and the size
                                field embedded in the ACPI table
                                pointed to by AcpiTableBuffer
                                are not in sync.
  @retval EFI_OUT_OF_RESOURCES  Insufficient resources exist to
                                complete the request.
  @retval EFI_ACCESS_DENIED     The table signature matches a table already
                                present in the system and platform policy
                                does not allow duplicate tables of this type.
**/
typedef
EFI_STATUS
(EFIAPI *ACPI_TABLE_INSTALL_ACPI_TABLE)(
  IN   EFI_PEI_SERVICES              **PeiServices,
  IN   ACPI_TABLE_PPI                *This,
  IN   VOID                          *AcpiTableBuffer,
  IN   UINTN                         AcpiTableBufferSize,
  OUT  UINTN                         *TableKey
  );

/**
  The UninstallAcpiTable() function allows a caller to remove an
  ACPI table. The routine will remove its reference from the
  RSDT/XSDT. A table is referenced by the TableKey parameter
  returned from a prior call to InstallAcpiTable(). If an EFI
  application is running at the time of this call, the relevant
  EFI_CONFIGURATION_TABLE pointer to the RSDT is no longer
  considered valid.

  @param PeiServices            General purpose services available to every PEIM.
  @param This                   A pointer to a ACPI_TABLE_PPI.
  @param TableKey               Specifies the table to uninstall. The key was
                                returned from InstallAcpiTable().
  @retval EFI_SUCCESS           The table was successfully inserted
  @retval EFI_NOT_FOUND         TableKey does not refer to a valid key
                                for a table entry.
  @retval EFI_OUT_OF_RESOURCES  Insufficient resources exist to
                                complete the request.
**/
typedef
EFI_STATUS
(EFIAPI *ACPI_TABLE_UNINSTALL_ACPI_TABLE)(
  IN  EFI_PEI_SERVICES             **PeiServices,
  IN  ACPI_TABLE_PPI               *This,
  IN  UINTN                        TableKey
  );


/**
  Returns a requested ACPI table.

  The following structures are not considered elements in the list of
  ACPI tables:
  - Root System Description Pointer (RSD_PTR)
  - Root System Description Table (RSDT)
  - Extended System Description Table (XSDT)
  Version is updated with a bit map containing all the versions of ACPI of which the table is a
  member. For tables installed via the EFI_ACPI_TABLE_PROTOCOL.InstallAcpiTable() interface,
  the function returns the value of EFI_ACPI_STD_PROTOCOL.AcpiVersion.

  @param PeiServices               General purpose services available to every PEIM.
  @param This                      A pointer to a ACPI_TABLE_PPI.
  @param[in]    Index              The zero-based index of the table to retrieve.
  @param[out]   Table              Pointer for returning the table buffer.
  @param[out]   Version            On return, updated with the ACPI versions to which this table belongs. Type
                                   EFI_ACPI_TABLE_VERSION is defined in "Related Definitions" in the
                                   EFI_ACPI_SDT_PROTOCOL.
  @param[out]   TableKey           On return, points to the table key for the specified ACPI system definition table.
                                   This is identical to the table key used in the EFI_ACPI_TABLE_PROTOCOL.
                                   The TableKey can be passed to EFI_ACPI_TABLE_PROTOCOL.UninstallAcpiTable()
                                   to uninstall the table.
  @retval EFI_SUCCESS              The function completed successfully.
  @retval EFI_NOT_FOUND            The requested index is too large and a table was not found.
**/
typedef
EFI_STATUS
(EFIAPI *ACPI_TABLE_GET_ACPI_TABLE)(
  IN  EFI_PEI_SERVICES         **PeiServices,
  IN  ACPI_TABLE_PPI           *This,
  IN  UINTN                    Index,
  OUT VOID                     **Table,
  OUT UINT32                   *Version,
  OUT UINTN                    *TableKey
  );

///
/// The ACPI_TABLE_PPI provides the ability for a component
/// to install and uninstall ACPI tables from a platform.
///
struct _ACPI_TABLE_PPI {
  ACPI_TABLE_INSTALL_ACPI_TABLE      InstallAcpiTable;
  ACPI_TABLE_UNINSTALL_ACPI_TABLE    UninstallAcpiTable;
  ACPI_TABLE_GET_ACPI_TABLE          GetAcpiTable;
};

extern EFI_GUID  gAcpiTablePpiGuid;

#endif
