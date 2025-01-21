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
#include "BdatAccessHandler.h"

static EFI_ACPI_TABLE_PROTOCOL  *AcpiTable;

/**
  Entry point of the Bdat Access Handler.

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
  )
{
  EFI_STATUS                      Status;
  VOID                            *HobList;

  ///
  /// Get the start of the HOBs.
  ///
  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, &HobList);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }
  ///
  /// Locate ACPI table protocol
  ///
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);
  ASSERT_EFI_ERROR (Status);
  ASSERT (AcpiTable != NULL);
  if ((AcpiTable == NULL) || EFI_ERROR (Status)) {
    return EFI_OUT_OF_RESOURCES;
  }
  CreateBdatTable (HobList, AcpiTable);
  ///
  /// ASSERT_EFI_ERROR (Status);
  ///
  return Status;
}
