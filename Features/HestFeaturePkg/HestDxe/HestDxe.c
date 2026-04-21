/** @file
  HEST DXE driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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
#include <Guid/Cper.h>
#include <Library/PcdLib.h>

//
// Maximum buffer size for WHEA ACPI tables
//
#define MAX_HEST_SIZE  0x1000

///
/// PCI Express Device AER Structure Definition
///
EFI_ACPI_6_5_PCI_EXPRESS_DEVICE_AER_STRUCTURE  PcieDeviceAerSource = {
  EFI_ACPI_6_5_PCI_EXPRESS_DEVICE_AER, // UINT16  Type;
  0,                                   // UINT16  SourceId;
  { 0, 0 },                            // UINT8   Reserved0[2];
  3,                                   // UINT8   Flags; Bit[0]-FIRMWARE_FIRST Bit[1]-GLOBAL
  0,                                   // UINT8   Enabled;
  1,                                   // UINT32  NumberOfRecordsToPreAllocate;
  1,                                   // UINT32  MaxSectionsPerRecord;
  0,                                   // UINT32  Bus;
  0,                                   // UINT16  Device;
  0,                                   // UINT16  Function;
  7,                                   // UINT16  DeviceControl;
  { 0, 0 },                            // UINT8   Reserved1[2];
  0,                                   // UINT32  UncorrectableErrorMask;
  0x62030,                             // UINT32  UncorrectableErrorSeverity;
  0x8000,                              // UINT32  CorrectableErrorMask;
  0,                                   // UINT32  AdvancedErrorCapabilitiesAndControl;
};

/**
  This routine is to add error source in HEST table. And return reference buffer address.


  @param  [in]  ErrorSource -- point to error source
  @param  [in]  InstallACPIFlag -- TRUE: install HEST ACPI table; FALSE: don't install.
  @param  [out] BufferAddress  -- buffer address in reserved area for the specific error source.

  @return none.
**/
VOID
AddHESTErrorSource (
  IN  VOID                                                 *ErrorSource,
  IN  UINT32                                               Size,
  IN  OUT EFI_ACPI_6_5_HARDWARE_ERROR_SOURCE_TABLE_HEADER  *HestPtr
  )
{
  EFI_ACPI_6_5_GENERIC_HARDWARE_ERROR_SOURCE_VERSION_2_STRUCTURE  *ErrorDest;

  if ((ErrorSource != NULL) && (Size != 0) && (HestPtr != NULL)) {
    if ((HestPtr->Header.Length + Size) > MAX_HEST_SIZE) {
      DEBUG ((DEBUG_ERROR, "[HEST] HEST table size is too small to add new source!\n"));
      return;
    }

    //
    // point to end of current table.
    //
    ErrorDest = (EFI_ACPI_6_5_GENERIC_HARDWARE_ERROR_SOURCE_VERSION_2_STRUCTURE *)(VOID *)((UINT8 *)HestPtr + HestPtr->Header.Length);
    CopyMem (ErrorDest, ErrorSource, Size);

    HestPtr->Header.Length += Size;
    ErrorDest->SourceId     = (UINT16)HestPtr->ErrorSourceCount;
    HestPtr->ErrorSourceCount++;
    DEBUG ((DEBUG_INFO, "[HEST] Current source counter:%d  table length:%x \n", HestPtr->ErrorSourceCount, HestPtr->Header.Length));
  }
}

/**
  Entry point of the HEST Dxe driver.

  @param[in] ImageHandle  EFI_HANDLE: A handle for the image that is initializing this driver
  @param[in] SystemTable  EFI_SYSTEM_TABLE: A pointer to the EFI system table

  @retval  EFI_SUCCESS:           Driver initialized successfully
           EFI_LOAD_ERROR:        Failed to Initialize or to Load the driver
           EFI_OUT_OF_RESOURCES:  Could not allocate needed resources

**/
EFI_STATUS
EFIAPI
HestDxeEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                                       Status;
  EFI_ACPI_6_5_HARDWARE_ERROR_SOURCE_TABLE_HEADER  *HestPtr;
  EFI_ACPI_TABLE_PROTOCOL                          *AcpiTable = NULL;
  UINTN                                            TableKey;

  DEBUG ((DEBUG_INFO, "%a() - enter\n", __FUNCTION__));

  HestPtr = (EFI_ACPI_6_5_HARDWARE_ERROR_SOURCE_TABLE_HEADER *)AllocateZeroPool (MAX_HEST_SIZE);

  if (HestPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "Unable to Allocate Memory for HEST Record Creation\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  HestPtr->Header.Signature = EFI_ACPI_6_5_HARDWARE_ERROR_SOURCE_TABLE_SIGNATURE;
  HestPtr->Header.Length    = sizeof (EFI_ACPI_6_5_HARDWARE_ERROR_SOURCE_TABLE_HEADER);
  HestPtr->Header.Revision  = EFI_ACPI_6_5_HARDWARE_ERROR_SOURCE_TABLE_REVISION;
  HestPtr->Header.Checksum  = 0;
  HestPtr->ErrorSourceCount = 0;

  CopyMem (
           HestPtr->Header.OemId,
           PcdGetPtr (PcdAcpiDefaultOemId),
           sizeof (HestPtr->Header.OemId)
           );

  HestPtr->Header.OemTableId      = PcdGet64 (PcdAcpiDefaultOemTableId);
  HestPtr->Header.OemRevision     = PcdGet32 (PcdAcpiDefaultOemRevision);
  HestPtr->Header.CreatorId       = PcdGet32 (PcdAcpiDefaultCreatorId);
  HestPtr->Header.CreatorRevision = PcdGet32 (PcdAcpiDefaultCreatorRevision);

  AddHESTErrorSource (&PcieDeviceAerSource, sizeof (EFI_ACPI_6_5_PCI_EXPRESS_DEVICE_AER_STRUCTURE), HestPtr);

  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **)&AcpiTable);
  if (AcpiTable == NULL) {
    return EFI_NOT_FOUND;
  }

  Status = AcpiTable->InstallAcpiTable (
                                        AcpiTable,
                                        &HestPtr->Header,
                                        HestPtr->Header.Length,
                                        &TableKey
                                        );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "%a() - exit\n", __FUNCTION__));

  return EFI_SUCCESS;
}
