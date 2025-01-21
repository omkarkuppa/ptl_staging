/** @file
  Source file for FSP S3 Notify  PEI module

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#include "FspS3Notify.h"
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

STATIC EFI_PEI_NOTIFY_DESCRIPTOR mNotifyDesc = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  FspS3NotifyOnEndOfPei
};

EFI_PEI_PPI_DESCRIPTOR      mReadyToInstallPeiPostPciEnumerationPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiReadyToInstallPciEnumerationCompleteProtocolGuid,
  NULL
};

EFI_PEI_PPI_DESCRIPTOR      mPeiPostPciEnumerationPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPciEnumerationCompleteProtocolGuid,
  NULL
};

EFI_PEI_PPI_DESCRIPTOR      mReadyToInstallPeiReadyToBootPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiReadyToInstallEventReadyToBootGuid,
  NULL
};

EFI_PEI_PPI_DESCRIPTOR      mPeiReadyToBootPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEventReadyToBootGuid,
  NULL
};

EFI_PEI_PPI_DESCRIPTOR      mReadyToInstallPeiEndOfFirmwarePpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gFspReadyToInstallEventEndOfFirmwareGuid,
  NULL
};

EFI_PEI_PPI_DESCRIPTOR      mPeiEndOfFirmwarePpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gFspEventEndOfFirmwareGuid,
  NULL
};

EFI_PEI_PPI_DESCRIPTOR      mEndOfFspS3NotifyPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEndOfFspS3NotifyGuid,
  NULL
};

/**
  FSP S3 Notify PEI module entry point

  @param[in]  FileHandle           Not used.
  @param[in]  PeiServices          General purpose services available to every PEIM.

  @retval     EFI_SUCCESS          The function completes successfully
  @retval     EFI_OUT_OF_RESOURCES Insufficient resources to create database
**/
EFI_STATUS
EFIAPI
FspS3NotifyEntryPoint (
  IN       EFI_PEI_FILE_HANDLE    FileHandle,
  IN CONST EFI_PEI_SERVICES       **PeiServices
  )
{
  EFI_STATUS             Status = EFI_SUCCESS;

  EFI_BOOT_MODE          BootMode;

  DEBUG((DEBUG_INFO, "FSP S3 Notify Entry\n"));

  Status = PeiServicesGetBootMode (&BootMode);
  if (BootMode == BOOT_ON_S3_RESUME) {
    //
    // End of SiInit notification event
    //
    DEBUG((DEBUG_INFO, "BOOT_ON_S3_RESUME\n"));
    Status = PeiServicesNotifyPpi (&mNotifyDesc);
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

/**
  Fix up pointers since they are located in real memory now.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The memory discovered PPI.  Not used.

  @retval EFI_SUCCESS             The function completed successfully.
**/
EFI_STATUS
EFIAPI
FspS3NotifyOnEndOfPei (
  IN EFI_PEI_SERVICES    **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDescriptor,
  IN VOID                      *Ppi
  )
{
  EFI_STATUS             Status;

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FspS3NotifyOnEndOfPei Entry\n"));

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP Post PCI Enumeration ...\n"));
  PeiServicesInstallPpi (&mReadyToInstallPeiPostPciEnumerationPpi);

  PeiServicesInstallPpi (&mPeiPostPciEnumerationPpi);

  DEBUG ((DEBUG_INFO| DEBUG_INIT, "FSP Ready To Boot ...\n"));
  PeiServicesInstallPpi (&mReadyToInstallPeiReadyToBootPpi);

  PeiServicesInstallPpi (&mPeiReadyToBootPpi);

  DEBUG ((DEBUG_INFO| DEBUG_INIT, "FSP End of Firmware ...\n"));
  PeiServicesInstallPpi (&mReadyToInstallPeiEndOfFirmwarePpi);

  PeiServicesInstallPpi (&mPeiEndOfFirmwarePpi);

  DEBUG ((DEBUG_INFO| DEBUG_INIT, "FSP S3 Notify End ...\n"));
  PeiServicesInstallPpi (&mEndOfFspS3NotifyPpi);

  return Status;
}
