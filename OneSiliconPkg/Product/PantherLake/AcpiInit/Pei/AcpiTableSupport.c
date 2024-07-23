/** @file
  ACPI table support APIs for ACPI PEIM

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

#include "AcpiInit.h"

//
// The maximum number of tables this driver supports
//
#define SILICON_ACPI_MAX_NUM_TABLES  20

GLOBAL_REMOVE_IF_UNREFERENCED  UINTN    mSiliconAcpiMaxNumTables = SILICON_ACPI_MAX_NUM_TABLES;
/**
  This function calculates and updates an UINT8 checksum.
  @param  Buffer          Pointer to buffer to checksum
  @param  Size            Number of bytes to checksum
  @param  ChecksumOffset  Offset to place the checksum result in
  @return EFI_SUCCESS             The function completed successfully.
**/
EFI_STATUS
AcpiPlatformChecksum (
  IN VOID   *Buffer,
  IN UINTN  Size,
  IN UINTN  ChecksumOffset
  )
{
  UINT8  Sum;
  UINT8  *Ptr;

  Sum = 0;
  //
  // Initialize pointer
  //
  Ptr = Buffer;

  //
  // set checksum to 0 first
  //
  Ptr[ChecksumOffset] = 0;

  //
  // add all content of buffer
  //
  while ((Size--) != 0) {
    Sum = (UINT8)(Sum + (*Ptr++));
  }

  //
  // set checksum
  //
  Ptr                 = Buffer;
  Ptr[ChecksumOffset] = (UINT8)(0xff - Sum + 1);

  return EFI_SUCCESS;
}

/**
  Checksum all versions of the common tables, RSDP, RSDT, XSDT.
  @param  AcpiTableInstance  Protocol instance private data.
  @return EFI_SUCCESS        The function completed successfully.
**/
EFI_STATUS
ChecksumCommonTables (
  IN OUT SILICON_ACPI_TABLE_INSTANCE  *AcpiTableInstance
  )
{
  //
  // RSDP ACPI 1.0 checksum for 2.0/3.0 table.  This is only the first 20 bytes of the structure
  //
  AcpiPlatformChecksum (
    AcpiTableInstance->Rsdp,
    sizeof (EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER),
    OFFSET_OF (
      EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER,
      Checksum
      )
    );

  //
  // RSDP ACPI 2.0/3.0 checksum, this is the entire table
  //
  AcpiPlatformChecksum (
    AcpiTableInstance->Rsdp,
    sizeof (EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER),
    OFFSET_OF (
      EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER,
      ExtendedChecksum
      )
    );

  //
  // XSDT checksum
  //
  AcpiPlatformChecksum (
    AcpiTableInstance->Xsdt,
    AcpiTableInstance->Xsdt->Length,
    OFFSET_OF (
      EFI_ACPI_DESCRIPTION_HEADER,
      Checksum
      )
    );

  return EFI_SUCCESS;
}


/**
  If the number of APCI tables exceeds the preallocated max table number, enlarge the table buffer.
  @param  AcpiTableInstance       ACPI table protocol instance data structure.
  @return EFI_SUCCESS             reallocate the table beffer successfully.
  @return EFI_OUT_OF_RESOURCES    Unable to allocate required resources.
**/
EFI_STATUS
ReallocateAcpiTableBuffer (
  IN SILICON_ACPI_TABLE_INSTANCE  *AcpiTableInstance
  )
{
  UINTN                    NewMaxTableNumber;
  UINTN                    TotalSize;
  UINT8                    *Pointer;
  EFI_PHYSICAL_ADDRESS     PageAddress;
  SILICON_ACPI_TABLE_INSTANCE  PrivateData;
  EFI_STATUS               Status;
  UINT64                   CurrentData;

  CopyMem (&PrivateData, AcpiTableInstance, sizeof (SILICON_ACPI_TABLE_INSTANCE));
  //
  // Enlarge the max table number from mSiliconAcpiMaxNumTables to mSiliconAcpiMaxNumTables + SILICON_ACPI_MAX_NUM_TABLES
  //
  NewMaxTableNumber = mSiliconAcpiMaxNumTables + SILICON_ACPI_MAX_NUM_TABLES;
  //
  // Calculate TotalSize and allocate buffers.
  //
  TotalSize = sizeof (EFI_ACPI_DESCRIPTION_HEADER) +         // for ACPI 2.0/3.0 XSDT
              NewMaxTableNumber * sizeof (UINT64);

  PageAddress = 0xFFFFFFFF;
  Status      = PeiServicesAllocatePages (
                       EfiACPIReclaimMemory,
                       EFI_SIZE_TO_PAGES (TotalSize),
                       &PageAddress
                       );

  if (EFI_ERROR (Status)) {
    return EFI_OUT_OF_RESOURCES;
  }

  Pointer = (UINT8 *)(UINTN)PageAddress;

  ZeroMem (Pointer, TotalSize);

  AcpiTableInstance->Xsdt = (EFI_ACPI_DESCRIPTION_HEADER *)Pointer;

  CurrentData = (UINT64)(UINTN)AcpiTableInstance->Xsdt;
  CopyMem (&AcpiTableInstance->Rsdp->XsdtAddress, &CurrentData, sizeof (UINT64));

  CopyMem (AcpiTableInstance->Xsdt, PrivateData.Xsdt, (sizeof (EFI_ACPI_DESCRIPTION_HEADER) + mSiliconAcpiMaxNumTables * sizeof (UINT64)));

  //
  // Calculate orignal ACPI table buffer size
  //
  TotalSize = sizeof (EFI_ACPI_DESCRIPTION_HEADER) +         // for ACPI 2.0/3.0 XSDT
              mSiliconAcpiMaxNumTables * sizeof (UINT64);

  PeiServicesFreePages (
          (EFI_PHYSICAL_ADDRESS)(UINTN)PrivateData.Xsdt,
          EFI_SIZE_TO_PAGES (TotalSize)
          );

  //
  // Update the Max ACPI table number
  //
  mSiliconAcpiMaxNumTables = NewMaxTableNumber;

  //
  return EFI_SUCCESS;
}

/**
  This function adds an ACPI table to the table list.  It will detect FACS and
  allocate the correct type of memory and properly align the table.

  @param  AcpiTableInstance         Instance of the protocol.
  @param  Table                     Table to add.
  @param  Checksum                  Does the table require checksumming.
  @param  Version                   The version of the list to add the table to.
  @param  IsFromHob                 True, if add Apci Table from Hob List.
  @param  Handle                    Pointer for returning the handle.

  @return EFI_SUCCESS               The function completed successfully.
  @return EFI_OUT_OF_RESOURCES      Could not allocate a required resource.
  @retval EFI_ACCESS_DENIED         The table signature matches a table already
                                    present in the system and platform policy
                                    does not allow duplicate tables of this type.

**/
EFI_STATUS
AddTableToList (
  IN SILICON_ACPI_TABLE_INSTANCE  *AcpiTableInstance,
  IN VOID                     *Table,
  IN BOOLEAN                  Checksum,
  IN EFI_ACPI_TABLE_VERSION   Version,
  IN BOOLEAN                  IsFromHob,
  OUT UINTN                   *Handle
  )
{
  EFI_STATUS            Status;
  SILICON_ACPI_TABLE_LIST   *CurrentTableList;
  UINT32                CurrentTableSignature;
  UINT32                CurrentTableSize;
  VOID                  *CurrentXsdtEntry;
  EFI_PHYSICAL_ADDRESS  AllocPhysAddress;
  UINT64                Buffer64;
  BOOLEAN               AddToXsdt;

  //
  // Check for invalid input parameters
  //
  ASSERT (AcpiTableInstance);
  ASSERT (Table);
  ASSERT (Handle);

  //
  // Init locals
  //
  AddToXsdt = TRUE;

  //
  // Create a new list entry
  //
  AllocPhysAddress                 = 0xFFFFFFFF;
  Status = PeiServicesAllocatePages (
                    EfiBootServicesData,
                    EFI_SIZE_TO_PAGES (sizeof (SILICON_ACPI_TABLE_LIST)),
                    &AllocPhysAddress
                    );
  ASSERT_EFI_ERROR(Status);
  CurrentTableList =  (SILICON_ACPI_TABLE_LIST *)(UINTN)AllocPhysAddress;

  //
  // Determine table type and size
  //
  CurrentTableSignature = ((EFI_ACPI_COMMON_HEADER *)Table)->Signature;
  CurrentTableSize      = ((EFI_ACPI_COMMON_HEADER *)Table)->Length;

  //
  // Allocate a buffer for the table.
  //
  AllocPhysAddress                 = 0xFFFFFFFF;
  CurrentTableList->TableSize      = CurrentTableSize;
  CurrentTableList->PoolAllocation = FALSE;


  //
  // Allocation memory type depends on the type of the table
  //
  if ((CurrentTableSignature == EFI_ACPI_6_5_FIRMWARE_ACPI_CONTROL_STRUCTURE_SIGNATURE) ||
      (CurrentTableSignature == EFI_ACPI_6_5_UEFI_ACPI_DATA_TABLE_SIGNATURE))
  {
    //
    // Allocate memory for the FACS.  This structure must be aligned
    // on a 64 byte boundary and must be ACPI NVS memory.
    // Using AllocatePages should ensure that it is always aligned.
    // Do not change signature for new ACPI version because they are same.
    //
    // UEFI table also need to be in ACPI NVS memory, because some data field
    // could be updated by OS present agent. For example, BufferPtrAddress in
    // SMM communication ACPI table.
    //
    ASSERT ((EFI_PAGE_SIZE % 64) == 0);

    Status = PeiServicesAllocatePages (
                    EfiACPIMemoryNVS,
                    EFI_SIZE_TO_PAGES (CurrentTableList->TableSize),
                    &AllocPhysAddress
                    );

  } else {
    //
    // All other tables are ACPI reclaim memory, no alignment requirements.
    //
    Status = PeiServicesAllocatePages (
                      EfiACPIReclaimMemory,
                      EFI_SIZE_TO_PAGES (CurrentTableList->TableSize),
                      &AllocPhysAddress);
  }

  ASSERT_EFI_ERROR(Status);

  CurrentTableList->Table = (EFI_ACPI_COMMON_HEADER *)(UINTN)AllocPhysAddress;

  //
  // Initialize the table contents
  //
  CurrentTableList->Signature = SILICON_ACPI_TABLE_LIST_SIGNATURE;
  CopyMem (CurrentTableList->Table, Table, CurrentTableSize);
  CurrentTableList->Handle  = AcpiTableInstance->CurrentHandle++;
  *Handle                   = CurrentTableList->Handle;
  CurrentTableList->Version = Version;

  //
  // Update internal pointers if this is a required table.  If it is a required
  // table and a table of that type already exists, return an error.
  //
  // Calculate the checksum if the table is not FACS.
  //
  switch (CurrentTableSignature) {
    case EFI_ACPI_6_5_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE:
      //
      // Check that the table has not been previously added.
      //
      if (AcpiTableInstance->Fadt != NULL)
      {
        PeiServicesFreePages((EFI_PHYSICAL_ADDRESS)CurrentTableList->Table, EFI_SIZE_TO_PAGES (CurrentTableList->TableSize));
        PeiServicesFreePages((EFI_PHYSICAL_ADDRESS)CurrentTableList, EFI_SIZE_TO_PAGES (sizeof (SILICON_ACPI_TABLE_LIST)));
        return EFI_ACCESS_DENIED;
      }

      //
      // We don't add the FADT in the standard way because some
      // OS expect the FADT to be early in the table list.
      // So we always add it as the first element in the list.
      //
      AddToXsdt = FALSE;

      //
      // Save a pointer to the table
      //
      AcpiTableInstance->Fadt = (EFI_ACPI_6_5_FIXED_ACPI_DESCRIPTION_TABLE *)CurrentTableList->Table;
      //

      // Update pointers in FADT.  If tables don't exist this will put NULL pointers there.
      // Note: If the FIRMWARE_CTRL is non-zero, then X_FIRMWARE_CTRL must be zero, and
      // vice-versa.
      //
      if ((UINT64)(UINTN)AcpiTableInstance->Facs < BASE_4GB) {
        AcpiTableInstance->Fadt->FirmwareCtrl = (UINT32)(UINTN)AcpiTableInstance->Facs;
        ZeroMem (&AcpiTableInstance->Fadt->XFirmwareCtrl, sizeof (UINT64));
      } else {
        Buffer64 = (UINT64)(UINTN)AcpiTableInstance->Facs;
        CopyMem (
          &AcpiTableInstance->Fadt->XFirmwareCtrl,
          &Buffer64,
          sizeof (UINT64)
          );
        AcpiTableInstance->Fadt->FirmwareCtrl = 0;
      }
      if ((UINT64)(UINTN)AcpiTableInstance->Dsdt < BASE_4GB) {
        AcpiTableInstance->Fadt->Dsdt = (UINT32)(UINTN)AcpiTableInstance->Dsdt;
        Buffer64 = AcpiTableInstance->Fadt->Dsdt;
      } else {
        AcpiTableInstance->Fadt->Dsdt = 0;
        Buffer64 = (UINT64)(UINTN)AcpiTableInstance->Dsdt;
      }
      CopyMem (&AcpiTableInstance->Fadt->XDsdt, &Buffer64, sizeof (UINT64));

      //
      // Checksum the table
      //
      if (Checksum) {
        AcpiPlatformChecksum (
          CurrentTableList->Table,
          CurrentTableList->Table->Length,
          OFFSET_OF (
            EFI_ACPI_DESCRIPTION_HEADER,
            Checksum
            )
          );
      }

      //
      // XSDT OEM information is updated to match the FADT OEM information
      //
      CopyMem (
        AcpiTableInstance->Rsdp->OemId,
        AcpiTableInstance->Fadt->Header.OemId,
        6
        );
      CopyMem (
        AcpiTableInstance->Xsdt->OemId,
        AcpiTableInstance->Fadt->Header.OemId,
        6
        );
      CopyMem (
        &AcpiTableInstance->Xsdt->OemTableId,
        &AcpiTableInstance->Fadt->Header.OemTableId,
        sizeof (UINT64)
        );
      AcpiTableInstance->Xsdt->OemRevision = AcpiTableInstance->Fadt->Header.OemRevision;
      //
      // Add FADT as the first entry
      //
      CurrentXsdtEntry = (VOID *)((UINT8 *)AcpiTableInstance->Xsdt + sizeof (EFI_ACPI_DESCRIPTION_HEADER));
      Buffer64 = (UINT64)(UINTN)AcpiTableInstance->Fadt;
      CopyMem (
        CurrentXsdtEntry,
        &Buffer64,
        sizeof (UINT64)
        );
      break;

    case EFI_ACPI_6_5_FIRMWARE_ACPI_CONTROL_STRUCTURE_SIGNATURE:
      //
      // Check that the table has not been previously added.
      //
      if (AcpiTableInstance->Facs != NULL)
      {
        PeiServicesFreePages((EFI_PHYSICAL_ADDRESS)CurrentTableList->Table, EFI_SIZE_TO_PAGES (CurrentTableList->TableSize));
        PeiServicesFreePages((EFI_PHYSICAL_ADDRESS)CurrentTableList, EFI_SIZE_TO_PAGES (sizeof (SILICON_ACPI_TABLE_LIST)));
        return EFI_ACCESS_DENIED;
      }

      //
      // FACS is referenced by FADT and is not part of RSDT
      //
      AddToXsdt = FALSE;

      //
      // Save a pointer to the table
      //
      AcpiTableInstance->Facs = (EFI_ACPI_6_5_FIRMWARE_ACPI_CONTROL_STRUCTURE *)CurrentTableList->Table;

      //
      // If FADT already exists, update table pointers.
      //
      if (AcpiTableInstance->Fadt != NULL) {
        //
        // Note: If the FIRMWARE_CTRL is non-zero, then X_FIRMWARE_CTRL must be zero, and
        // vice-versa.
        //
        if ((UINT64)(UINTN)AcpiTableInstance->Facs < BASE_4GB) {
          AcpiTableInstance->Fadt->FirmwareCtrl = (UINT32)(UINTN)AcpiTableInstance->Facs;
          ZeroMem (&AcpiTableInstance->Fadt->XFirmwareCtrl, sizeof (UINT64));
        } else {
          Buffer64 = (UINT64)(UINTN)AcpiTableInstance->Facs;
          CopyMem (
            &AcpiTableInstance->Fadt->XFirmwareCtrl,
            &Buffer64,
            sizeof (UINT64)
            );
          AcpiTableInstance->Fadt->FirmwareCtrl = 0;
        }

        //
        // Checksum FADT table
        //
        AcpiPlatformChecksum (
          AcpiTableInstance->Fadt,
          AcpiTableInstance->Fadt->Header.Length,
          OFFSET_OF (
            EFI_ACPI_DESCRIPTION_HEADER,
            Checksum
            )
          );
      }
      break;

    case EFI_ACPI_6_5_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE:
      //
      // Check that the table has not been previously added.
      //
      if (AcpiTableInstance->Dsdt != NULL)
      {
        PeiServicesFreePages((EFI_PHYSICAL_ADDRESS)CurrentTableList->Table, EFI_SIZE_TO_PAGES (CurrentTableList->TableSize));
        PeiServicesFreePages((EFI_PHYSICAL_ADDRESS)CurrentTableList, EFI_SIZE_TO_PAGES (sizeof (SILICON_ACPI_TABLE_LIST)));
        return EFI_ACCESS_DENIED;
      }

      //
      // DSDT is referenced by FADT and is not part of RSDT
      //
      AddToXsdt = FALSE;

      //
      // Save a pointer to the table
      //
      AcpiTableInstance->Dsdt = (EFI_ACPI_DESCRIPTION_HEADER *)CurrentTableList->Table;

      //
      // If FADT already exists, update table pointers.
      //
      if (AcpiTableInstance->Fadt != NULL) {
        if ((UINT64)(UINTN)AcpiTableInstance->Dsdt < BASE_4GB) {
          AcpiTableInstance->Fadt->Dsdt = (UINT32)(UINTN)AcpiTableInstance->Dsdt;
          Buffer64 = AcpiTableInstance->Fadt->Dsdt;
        } else {
          AcpiTableInstance->Fadt->Dsdt = 0;
          Buffer64                       = (UINT64)(UINTN)AcpiTableInstance->Dsdt;
        }
        CopyMem (&AcpiTableInstance->Fadt->XDsdt, &Buffer64, sizeof (UINT64));

        //
        // Checksum FADT table
        //
        AcpiPlatformChecksum (
          AcpiTableInstance->Fadt,
          AcpiTableInstance->Fadt->Header.Length,
          OFFSET_OF (
            EFI_ACPI_DESCRIPTION_HEADER,
            Checksum
            )
          );
      }

      //
      // Checksum the table
      //
      if (Checksum) {
        AcpiPlatformChecksum (
          CurrentTableList->Table,
          CurrentTableList->Table->Length,
          OFFSET_OF (
            EFI_ACPI_DESCRIPTION_HEADER,
            Checksum
            )
          );
      }
      break;

    default:
      //
      // Checksum the table
      //
      if (Checksum) {
        AcpiPlatformChecksum (
          CurrentTableList->Table,
          CurrentTableList->Table->Length,
          OFFSET_OF (
            EFI_ACPI_DESCRIPTION_HEADER,
            Checksum
            )
          );
      }
      break;
  }

  //
  // Add the table to the current list of tables
  //
  InsertTailList (&AcpiTableInstance->TableList, &CurrentTableList->Link);

  if (AddToXsdt) {
    //
    // If the table number exceed the gEfiAcpiMaxNumTables, enlarge the table buffer
    //
    if (AcpiTableInstance->NumberOfTableEntries >= mSiliconAcpiMaxNumTables) {
      Status = ReallocateAcpiTableBuffer (AcpiTableInstance);
      ASSERT_EFI_ERROR (Status);
    }

    //
    // This pointer must not be directly dereferenced as the XSDT entries may not
    // be 64 bit aligned resulting in a possible fault.  Use CopyMem to update.
    //
    CurrentXsdtEntry = (VOID *)
                        (
                        (UINT8 *)AcpiTableInstance->Xsdt +
                        sizeof (EFI_ACPI_DESCRIPTION_HEADER) +
                        AcpiTableInstance->NumberOfTableEntries *
                        sizeof (UINT64)
                        );

    //
    // Add entry to XSDT, XSDT expects 64 bit pointers, but
    // the table pointers in XSDT are not aligned on 8 byte boundary.
    //
    Buffer64 = (UINT64)(UINTN)CurrentTableList->Table;
    CopyMem (
      CurrentXsdtEntry,
      &Buffer64,
      sizeof (UINT64)
      );

    //
    // Update length
    //
    AcpiTableInstance->Xsdt->Length = AcpiTableInstance->Xsdt->Length + sizeof (UINT64);

    AcpiTableInstance->NumberOfTableEntries++;
  }

  return EFI_SUCCESS;
}

/**
  Installs an ACPI table into the XSDT.
  Note that the ACPI table should be checksumed before installing it.
  Otherwise it will assert.
  @param  This                 Protocol instance pointer.
  @param  AcpiTableBuffer      A pointer to a buffer containing the ACPI table to be installed.
  @param  AcpiTableBufferSize  Specifies the size, in bytes, of the AcpiTableBuffer buffer.
  @param  TableKey             Reurns a key to refer to the ACPI table.
  @return EFI_SUCCESS            The table was successfully inserted.
  @return EFI_INVALID_PARAMETER  Either AcpiTableBuffer is NULL, TableKey is NULL, or AcpiTableBufferSize
                                 and the size field embedded in the ACPI table pointed to by AcpiTableBuffer
                                 are not in sync.
  @return EFI_OUT_OF_RESOURCES   Insufficient resources exist to complete the request.
  @retval EFI_ACCESS_DENIED      The table signature matches a table already
                                 present in the system and platform policy
                                 does not allow duplicate tables of this type.
**/
EFI_STATUS
EFIAPI
InstallAcpiTable (
  IN   EFI_PEI_SERVICES         **PeiServices,
  IN   ACPI_TABLE_PPI           *This,
  IN   VOID                     *AcpiTableBuffer,
  IN   UINTN                    AcpiTableBufferSize,
  OUT  UINTN                    *TableKey
  )
{
  SILICON_ACPI_TABLE_INSTANCE  *AcpiTableInstance;
  EFI_STATUS               Status;
  EFI_ACPI_TABLE_VERSION   Version;

  //
  // Check for invalid input parameters
  //
  if (  (AcpiTableBuffer == NULL) || (TableKey == NULL)
     || (((EFI_ACPI_DESCRIPTION_HEADER *)AcpiTableBuffer)->Length != AcpiTableBufferSize))
  {
    return EFI_INVALID_PARAMETER;
  }

  Version = PcdGet32 (PcdAcpiExposedTableVersions);

  //
  // Get the instance of the ACPI table protocol
  //
  AcpiTableInstance = SILICON_ACPI_TABLE_INSTANCE_FROM_THIS (This);

  //
  // Install the ACPI table
  //
  *TableKey = 0;
  Status = AddTableToList (
            AcpiTableInstance,
            AcpiTableBuffer,
            TRUE,
            Version,
            FALSE,
            TableKey
            );

  ChecksumCommonTables(AcpiTableInstance);

  return Status;
}

/**
  This function finds the table specified by the handle and returns a pointer to it.
  If the handle is not found, EFI_NOT_FOUND is returned and the contents of Table are
  undefined.

  @param  Handle      Table to find.
  @param  TableList   Table list to search
  @param  Table       Pointer to table found.

  @return EFI_SUCCESS    The function completed successfully.
  @return EFI_NOT_FOUND  No table found matching the handle specified.

**/
EFI_STATUS
FindTableByHandle (
  IN UINTN                 Handle,
  IN LIST_ENTRY            *TableList,
  OUT SILICON_ACPI_TABLE_LIST  **Table
  )
{
  LIST_ENTRY           *CurrentLink;
  SILICON_ACPI_TABLE_LIST  *CurrentTable;

  //
  // Check for invalid input parameters
  //
  ASSERT (Table);

  //
  // Find the table
  //
  CurrentLink = TableList->ForwardLink;

  while (CurrentLink != TableList) {
    CurrentTable = SILICON_ACPI_TABLE_LIST_FROM_LINK (CurrentLink);
    if (CurrentTable->Handle == Handle) {
      //
      // Found handle, so return this table.
      //
      *Table = CurrentTable;
      return EFI_SUCCESS;
    }

    CurrentLink = CurrentLink->ForwardLink;
  }

  //
  // Table not found
  //
  return EFI_NOT_FOUND;
}

/**
  This function removes a basic table from XSDT.
  For Acpi 1.0 tables, pass in the Rsdt.
  For Acpi 2.0 tables, pass in both Rsdt and Xsdt.

  @param  Table                 Pointer to table found.
  @param  NumberOfTableEntries  Current number of table entries in the XSDT
  @param  Rsdt                  Pointer to the RSDT to remove from
  @param  Xsdt                  Pointer to the Xsdt to remove from

  @return EFI_SUCCESS            The function completed successfully.
  @return EFI_INVALID_PARAMETER  The table was not found in both Rsdt and Xsdt.

**/
EFI_STATUS
RemoveTableFromRsdt (
  IN OUT SILICON_ACPI_TABLE_LIST          *Table,
  IN OUT UINTN                        *NumberOfTableEntries,
  IN OUT EFI_ACPI_DESCRIPTION_HEADER  *Rsdt OPTIONAL,
  IN OUT EFI_ACPI_DESCRIPTION_HEADER  *Xsdt OPTIONAL
  )
{
  VOID    *CurrentXsdtEntry;
  UINT64  CurrentTablePointer64;
  UINTN   Index;

  //
  // Check for invalid input parameters
  //
  ASSERT (Table);
  ASSERT (NumberOfTableEntries);
  ASSERT (Xsdt);

  //
  // Find the table entry in the RSDT and XSDT
  //
  for (Index = 0; Index < *NumberOfTableEntries; Index++) {

    if (Xsdt != NULL) {
      //
      // This pointer must not be directly dereferenced as the XSDT entries may not
      // be 64 bit aligned resulting in a possible fault.  Use CopyMem to update.
      //
      CurrentXsdtEntry = (VOID *)((UINT8 *)Xsdt + sizeof (EFI_ACPI_DESCRIPTION_HEADER) + Index * sizeof (UINT64));

      //
      // Read the entry value out of the XSDT
      //
      CopyMem (&CurrentTablePointer64, CurrentXsdtEntry, sizeof (UINT64));
    } else {
      //
      // Initialize to NULL
      //
      CurrentXsdtEntry      = 0;
      CurrentTablePointer64 = 0;
    }

    //
    // Check if we have found the corresponding entry in XSDT
    //
    if ((Xsdt == NULL) || (CurrentTablePointer64 == (UINT64)(UINTN)Table->Table))
    {
      if (Xsdt != NULL) {
        CopyMem (CurrentXsdtEntry, ((UINT64 *)CurrentXsdtEntry) + 1, (*NumberOfTableEntries - Index) * sizeof (UINT64));
        Xsdt->Length = Xsdt->Length - sizeof (UINT64);
      }

      break;
    } else if (Index + 1 == *NumberOfTableEntries) {
      //
      // At the last entry, and table not found
      //
      return EFI_INVALID_PARAMETER;
    }
  }

  if (Xsdt != NULL) {
    AcpiPlatformChecksum (
      Xsdt,
      Xsdt->Length,
      OFFSET_OF (
        EFI_ACPI_DESCRIPTION_HEADER,
        Checksum
        )
      );
  }

  //
  // Decrement the number of tables
  //
  (*NumberOfTableEntries)--;

  return EFI_SUCCESS;
}

/**
  This function removes a table and frees any associated memory.

  @param  AcpiTableInstance  Instance of the protocol.
  @param  Version            Version(s) to delete.
  @param  Table              Pointer to table found.

  @return EFI_SUCCESS  The function completed successfully.

**/
EFI_STATUS
DeleteTable (
  IN SILICON_ACPI_TABLE_INSTANCE  *AcpiTableInstance,
  IN EFI_ACPI_TABLE_VERSION   Version,
  IN OUT SILICON_ACPI_TABLE_LIST  *Table
  )
{
  UINT32   CurrentTableSignature;
  BOOLEAN  RemoveFromRsdt;

  //
  // Check for invalid input parameters
  //
  ASSERT (AcpiTableInstance);
  ASSERT (Table);

  //
  // Init locals
  //
  RemoveFromRsdt = TRUE;
  //
  // Check for Table->Table
  //
  if ((Table == NULL) || (Table->Table == NULL)) {
    return EFI_UNSUPPORTED;
  }
  ASSERT (Table->Table != NULL);
  CurrentTableSignature = ((EFI_ACPI_COMMON_HEADER *)Table->Table)->Signature;

  //
  // Basic tasks to accomplish delete are:
  //   Determine removal requirements (in XSDT or not)
  //   Remove entry from RSDT/XSDT
  //   Remove any table references to the table
  //   If no one is using the table
  //      Free the table (removing pointers from private data and tables)
  //      Remove from list
  //      Free list structure
  //
  //
  // Determine if this table is in the RSDT or XSDT
  //
  if ((CurrentTableSignature == EFI_ACPI_6_5_FIRMWARE_ACPI_CONTROL_STRUCTURE_SIGNATURE) ||
      (CurrentTableSignature == EFI_ACPI_6_5_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) ||
      (CurrentTableSignature == EFI_ACPI_6_5_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE)
      )
  {
    RemoveFromRsdt = FALSE;
  }

  //
  // We don't remove the FADT in the standard way because some
  // OS expect the FADT to be early in the table list.
  // So we always put it as the first element in the list.
  //
  if (CurrentTableSignature == EFI_ACPI_6_5_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE) {
    RemoveFromRsdt = FALSE;
  }

  //
  // Remove the table from RSDT and XSDT
  //
  if (Table->Table != NULL) {
    //
    // Remove from Rsdt and Xsdt.  We don't care about the return value
    // because it is acceptable for the table to not exist in Xsdt.
    // We didn't add some tables so we don't remove them.
    //
    if (RemoveFromRsdt) {
      RemoveTableFromRsdt (
        Table,
        &AcpiTableInstance->NumberOfTableEntries,
        NULL,
        AcpiTableInstance->Xsdt
        );
    }
    //
    // Free the table, clean up any dependent tables and our private data pointers.
    //
    switch (Table->Table->Signature) {
      case EFI_ACPI_6_5_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE:
        AcpiTableInstance->Fadt = NULL;
        break;

      case EFI_ACPI_6_5_FIRMWARE_ACPI_CONTROL_STRUCTURE_SIGNATURE:
        AcpiTableInstance->Facs = NULL;
        //
        // Update FADT table pointers
        //
        if (AcpiTableInstance->Fadt != NULL) {
          AcpiTableInstance->Fadt->FirmwareCtrl = 0;
          ZeroMem (&AcpiTableInstance->Fadt->XFirmwareCtrl, sizeof (UINT64));

          //
          // Checksum table
          //
          AcpiPlatformChecksum (
            AcpiTableInstance->Fadt,
            AcpiTableInstance->Fadt->Header.Length,
            OFFSET_OF (
              EFI_ACPI_DESCRIPTION_HEADER,
              Checksum
              )
            );
        }
        break;

      case EFI_ACPI_6_5_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE:
        AcpiTableInstance->Dsdt = NULL;

        //
        // Update FADT table pointers
        //
        if (AcpiTableInstance->Fadt != NULL) {
          AcpiTableInstance->Fadt->Dsdt = 0;
          ZeroMem (&AcpiTableInstance->Fadt->XDsdt, sizeof (UINT64));

          //
          // Checksum table
          //
          AcpiPlatformChecksum (
            AcpiTableInstance->Fadt,
            AcpiTableInstance->Fadt->Header.Length,
            OFFSET_OF (
              EFI_ACPI_DESCRIPTION_HEADER,
              Checksum
              )
            );
        }
        break;

      default:
        //
        // Do nothing
        //
        break;
    }
  }

  //
  // Free the Table
  //
  PeiServicesFreePages((EFI_PHYSICAL_ADDRESS)Table->Table, EFI_SIZE_TO_PAGES (Table->TableSize));
  RemoveEntryList (&(Table->Link));
  PeiServicesFreePages((EFI_PHYSICAL_ADDRESS)Table, EFI_SIZE_TO_PAGES (sizeof (SILICON_ACPI_TABLE_LIST)));
  //
  // Done
  //
  return EFI_SUCCESS;
}


/**
  This function finds and removes the table specified by the handle.

  @param  AcpiTableInstance  Instance of the protocol.
  @param  Version            Bitmask of which versions to remove.
  @param  Handle             Table to remove.

  @return EFI_SUCCESS    The function completed successfully.
  @return EFI_ABORTED    An error occurred.
  @return EFI_NOT_FOUND  Handle not found in table list.

**/
EFI_STATUS
RemoveTableFromList (
  IN SILICON_ACPI_TABLE_INSTANCE  *AcpiTableInstance,
  IN EFI_ACPI_TABLE_VERSION   Version,
  IN UINTN                    Handle
  )
{
  SILICON_ACPI_TABLE_LIST  *Table;
  EFI_STATUS           Status;

  Table = (SILICON_ACPI_TABLE_LIST *)NULL;

  //
  // Check for invalid input parameters
  //
  ASSERT (AcpiTableInstance);

  //
  // Find the table
  //
  Status = FindTableByHandle (
             Handle,
             &AcpiTableInstance->TableList,
             &Table
             );
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  //
  // Remove the table
  //
  Status = DeleteTable (AcpiTableInstance, Version, Table);
  if (EFI_ERROR (Status)) {
    return EFI_ABORTED;
  }

  //
  // Completed successfully
  //
  return EFI_SUCCESS;
}


/**
  Removes an ACPI table from the RSDT/XSDT.
  @param  This      Protocol instance pointer.
  @param  TableKey  Specifies the table to uninstall.  The key was returned from InstallAcpiTable().
  @return EFI_SUCCESS    The table was successfully uninstalled.
  @return EFI_NOT_FOUND  TableKey does not refer to a valid key for a table entry.
**/
EFI_STATUS
EFIAPI
UninstallAcpiTable (
   IN  EFI_PEI_SERVICES        **PeiServices,
   IN  ACPI_TABLE_PPI           *This,
   IN  UINTN                    TableKey
   )
{
  SILICON_ACPI_TABLE_INSTANCE  *AcpiTableInstance;
  EFI_STATUS               Status;
  EFI_ACPI_TABLE_VERSION   Version;

  //
  // Get the instance of the ACPI table protocol
  //
  AcpiTableInstance = SILICON_ACPI_TABLE_INSTANCE_FROM_THIS (This);

  Version = PcdGet32 (PcdAcpiExposedTableVersions);

  //
  // Uninstall the ACPI table
  //
  Status = RemoveTableFromList (
             AcpiTableInstance,
             Version,
             TableKey
             );

  ChecksumCommonTables(AcpiTableInstance);

  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  } else {
    return EFI_SUCCESS;
  }
}

/**
  Returns a requested ACPI table.

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
EFI_STATUS
EFIAPI GetAcpiTable (
  IN  EFI_PEI_SERVICES         **PeiServices,
  IN  ACPI_TABLE_PPI           *This,
  IN  UINTN                    Index,
  OUT VOID                     **Table,
  OUT UINT32                   *Version,
  OUT UINTN                    *TableKey
  )
{
  SILICON_ACPI_TABLE_INSTANCE  *AcpiTableInstance;
  UINTN                TableIndex;
  LIST_ENTRY           *CurrentLink;
  LIST_ENTRY           *StartLink;
  SILICON_ACPI_TABLE_LIST  *CurrentTable;

  //
  // Get the instance of the ACPI table protocol
  //
  AcpiTableInstance = SILICON_ACPI_TABLE_INSTANCE_FROM_THIS (This);
  //
  // Find the table
  //
  StartLink   = &AcpiTableInstance->TableList;
  CurrentLink = StartLink->ForwardLink;
  TableIndex  = 0;

  while (CurrentLink != StartLink) {
    if (TableIndex == Index) {
      break;
    }

    //
    // Next one
    //
    CurrentLink = CurrentLink->ForwardLink;
    TableIndex++;
  }

  if ((TableIndex != Index) || (CurrentLink == StartLink)) {
    return EFI_NOT_FOUND;
  }

  //
  // Get handle and version
  //
  CurrentTable = SILICON_ACPI_TABLE_LIST_FROM_LINK (CurrentLink);
  *TableKey    = CurrentTable->Handle;
  *Version     = CurrentTable->Version;
  *Table       = (VOID*) CurrentTable->Table;

  return EFI_SUCCESS;
}

/**
  Intilializes the ACPI table Instance and builds the Hob Structure
  @param[in]  SiliconAcpiTableInstance      Pointer to Silicon Acpi Instance Structure.
  @retval     EFI_SUCCESS                   The function completes successfully.
**/

EFI_STATUS
InitializeAcpiTableInstance (
  SILICON_ACPI_TABLE_INSTANCE *SiliconAcpiTableInstance
  )
{

  EFI_STATUS                              Status;
  EFI_PHYSICAL_ADDRESS                    RsdpAddress;
  EFI_PHYSICAL_ADDRESS                    XsdtAddress;
  ACPI_SILICON_HOB                        *AcpiSiliconHob;
  Status                                  = EFI_SUCCESS;
  RsdpAddress                             = 0xFFFFFFFF;
  XsdtAddress                             = 0xFFFFFFFF;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  //
  // Check for invalid input parameters
  //
  ASSERT (SiliconAcpiTableInstance);

  SiliconAcpiTableInstance->AcpiTablePpi.InstallAcpiTable   = InstallAcpiTable;
  SiliconAcpiTableInstance->AcpiTablePpi.UninstallAcpiTable = UninstallAcpiTable;
  SiliconAcpiTableInstance->AcpiTablePpi.GetAcpiTable = GetAcpiTable;
  InitializeListHead (&SiliconAcpiTableInstance->TableList);
  SiliconAcpiTableInstance->CurrentHandle = 1;
  //
  // Memory Allocation for RSDP
  //
  Status = PeiServicesAllocatePages (
             EfiACPIReclaimMemory,
             EFI_SIZE_TO_PAGES (
             sizeof (EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER)), // RSDP
             &RsdpAddress
             );

  if (EFI_ERROR (Status) || RsdpAddress == 0) {
    ASSERT_EFI_ERROR (Status);
    ASSERT (RsdpAddress != 0);
    return Status;
  }

  ZeroMem (
    (VOID *) (UINTN) RsdpAddress,
    sizeof (EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER)
    );

  SiliconAcpiTableInstance->Rsdp = (EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER *) (UINTN) (RsdpAddress);

  //
  // Memory Allocation for XSDT
  //
  Status = PeiServicesAllocatePages (
             EfiACPIReclaimMemory,
             EFI_SIZE_TO_PAGES (
             sizeof (EFI_ACPI_DESCRIPTION_HEADER) +                 // XSDT
             mSiliconAcpiMaxNumTables * sizeof (UINT64)),           // Max Num Entries
             &XsdtAddress
             );
  if (EFI_ERROR (Status) || XsdtAddress == 0) {
    ASSERT_EFI_ERROR (Status);
    ASSERT (XsdtAddress != 0);
    return Status;
  }

  ZeroMem (
    (VOID *) (UINTN) XsdtAddress,
    sizeof (EFI_ACPI_DESCRIPTION_HEADER) +
    mSiliconAcpiMaxNumTables * sizeof (UINT64)
    );

  SiliconAcpiTableInstance->Xsdt = (EFI_ACPI_DESCRIPTION_HEADER *) (UINTN) (XsdtAddress);

  //
  // Update the Values for RSDP.
  //
  CopyMem (SiliconAcpiTableInstance->Rsdp->OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (SiliconAcpiTableInstance->Rsdp->OemId));
  SetMem (SiliconAcpiTableInstance->Rsdp->Reserved, sizeof (SiliconAcpiTableInstance->Rsdp->Reserved), EFI_ACPI_RESERVED_BYTE);
  SiliconAcpiTableInstance->Rsdp->Signature        = EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER_SIGNATURE;
  SiliconAcpiTableInstance->Rsdp->Length           = sizeof (EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER);
  SiliconAcpiTableInstance->Rsdp->Revision         = EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER_REVISION;
  SiliconAcpiTableInstance->Rsdp->XsdtAddress      = (UINT64)(UINTN)SiliconAcpiTableInstance->Xsdt;

  //
  // Update the values for XSDT.
  //
  CopyMem (SiliconAcpiTableInstance->Xsdt->OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (SiliconAcpiTableInstance->Xsdt->OemId));
  SiliconAcpiTableInstance->Xsdt->Signature       = EFI_ACPI_6_5_EXTENDED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE;
  SiliconAcpiTableInstance->Xsdt->Length          = sizeof (EFI_ACPI_DESCRIPTION_HEADER);
  SiliconAcpiTableInstance->Xsdt->OemTableId      = PcdGet64 (PcdAcpiDefaultOemTableId);
  SiliconAcpiTableInstance->Xsdt->OemRevision     = PcdGet32 (PcdAcpiDefaultOemRevision);
  SiliconAcpiTableInstance->Xsdt->CreatorId       = PcdGet32 (PcdAcpiDefaultCreatorId);
  SiliconAcpiTableInstance->Xsdt->CreatorRevision = PcdGet32 (PcdAcpiDefaultCreatorRevision);
  SiliconAcpiTableInstance->Xsdt->Revision        = EFI_ACPI_6_5_EXTENDED_SYSTEM_DESCRIPTION_TABLE_REVISION;

  //
  // Reserve the first entry for FADT
  //
  SiliconAcpiTableInstance->Xsdt->Length = SiliconAcpiTableInstance->Xsdt->Length + sizeof (UINT64);
  SiliconAcpiTableInstance->NumberOfTableEntries = 1;

  //
  // Check sum for RSDP and XSDT
  //
  ChecksumCommonTables(SiliconAcpiTableInstance);

  //
  // Creat ACPI Silicon Hob
  //
  AcpiSiliconHob = BuildGuidHob (&gAcpiTableHobGuid, sizeof (ACPI_SILICON_HOB));
  ASSERT ( AcpiSiliconHob!= NULL);

  AcpiSiliconHob->Rsdp = (EFI_PHYSICAL_ADDRESS)(UINT64) SiliconAcpiTableInstance->Rsdp;
  DEBUG ((DEBUG_INFO, "AcpiDataHob->Rsdp= 0x%x\n",AcpiSiliconHob->Rsdp));

  AcpiSiliconHob->MaxNumOfTable = &mSiliconAcpiMaxNumTables;
  //
  // Completed successfully
  //
  return Status;
}
