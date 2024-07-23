/** @file
  Silicon Acpi Module

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

EFI_PEI_PPI_DESCRIPTOR mAcpiTablePpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gAcpiTablePpiGuid,
  NULL
};

/**
  Silicon ACPI initialization entry point
  @param[in]  FileHandle         Handle of the file being invoked.
  @param[in]  PeiServices        Describes the list of possible PEI services.
  @retval     EFI_SUCCESS        The function completes successfully.
**/
EFI_STATUS
EFIAPI
InitializeAcpi (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiService
  )
{
  EFI_STATUS                    Status;
  EFI_PHYSICAL_ADDRESS          PageAdress;
  SILICON_ACPI_TABLE_INSTANCE  *SiliconAcpiTableInstance;
  PageAdress = 0xFFFFFFFF;

  Status = PeiServicesAllocatePages (
                  EfiACPIReclaimMemory,
                  EFI_SIZE_TO_PAGES (sizeof (SILICON_ACPI_TABLE_INSTANCE)),
                  &PageAdress
                  );

  SiliconAcpiTableInstance = (SILICON_ACPI_TABLE_INSTANCE*)(UINTN)PageAdress;
  ZeroMem(SiliconAcpiTableInstance, sizeof (SILICON_ACPI_TABLE_INSTANCE));

  SiliconAcpiTableInstance->Signature = SILICON_ACPI_TABLE_SIGNATURE;

  Status = InitializeAcpiTableInstance (SiliconAcpiTableInstance);

  if (EFI_ERROR (Status)) {
    PeiServicesFreePages (
              (EFI_PHYSICAL_ADDRESS)(UINTN)SiliconAcpiTableInstance,
              EFI_SIZE_TO_PAGES (sizeof (SILICON_ACPI_TABLE_INSTANCE))
              );
  }

  mAcpiTablePpi.Ppi = (void*)(&SiliconAcpiTableInstance->AcpiTablePpi);
  Status = PeiServicesInstallPpi (&mAcpiTablePpi);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
