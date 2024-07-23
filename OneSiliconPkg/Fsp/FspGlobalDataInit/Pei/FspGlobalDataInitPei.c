/** @file

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

@par Specification Reference:
**/

#include <PiPei.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/PeimEntryPoint.h>
#include <FspGlobalData.h>
#include <Library/FspCommonLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <FspsUpd.h>

/**
  This function is called after PEI core discover memory and finish migration.

  @param[in] PeiServices    Pointer to PEI Services Table.
  @param[in] NotifyDesc     Pointer to the descriptor for the Notification event that
                            caused this function to execute.
  @param[in] Ppi            Pointer to the PPI data associated with this function.

  @retval EFI_STATUS        Always return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
PeiMemoryDiscoveredNotify (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

EFI_PEI_NOTIFY_DESCRIPTOR mPeiMemoryDiscoveredNotifyDesc = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  PeiMemoryDiscoveredNotify
};

/**
  Main entry

  @param[in] FileHandle        Handle of the file being invoked.
  @param[in] PeiServices       Pointer to PEI Services table.

  @retval EFI_SUCCESS          Install function successfully.
  @retval EFI_OUT_OF_RESOURCES Insufficient resources to create buffer.

**/
EFI_STATUS
EFIAPI
FspGlobalDataInitEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS              Status;
  FSP_GLOBAL_DATA         *FspData;
  EFI_PEI_PPI_DESCRIPTOR  *PpiDescriptor;
  VOID                    *FsptUpdLocationPpi;

  Status = EFI_SUCCESS;
  FsptUpdLocationPpi = NULL;
  FspData = GetFspGlobalDataPointer ();

  if ((UINTN)FspData == 0x00 || (UINTN)FspData == 0xFFFFFFFF) {
    //
    // Re-initialize Global Data as it was not present/initialized earlier.
    //
    FspData = NULL;
    FspData = (FSP_GLOBAL_DATA *) AllocateZeroPool (sizeof (FSP_GLOBAL_DATA));
    if (FspData == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    FspData->Signature      = FSP_GLOBAL_DATA_SIGNATURE;
    FspData->Version        = FSP_GLOBAL_DATA_VERSION;
    FspData->PerfIdx        = 2;
    FspData->PerfSig        = FSP_PERFORMANCE_DATA_SIGNATURE;

    //
    // In API mode FspGlobalData must have be initialized in FspSecCore, so here must be DISPATCH mode.
    //
    FspData->FspMode = FSP_IN_DISPATCH_MODE;

    Status = (*PeiServices)->LocatePpi (
                               PeiServices,
                               &gFsptUpdLocationPpiGuid,
                               0,
                               NULL,
                               (VOID **) &FsptUpdLocationPpi
                               );
    if (!EFI_ERROR (Status)) {
      FspData->TempRamInitUpdPtr = FsptUpdLocationPpi;
    }

    SetFspGlobalDataPointer (FspData);
    //
    // Ensure the global data pointer is valid
    //
    ASSERT (GetFspGlobalDataPointer () == FspData);
  }

  if (FspData->FspMode == FSP_IN_DISPATCH_MODE) { // FSP_MODE_CHECK
    PpiDescriptor = AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
    if (PpiDescriptor == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    if (PpiDescriptor != NULL) {
      PpiDescriptor->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
      PpiDescriptor->Ppi   = (VOID *) FspData;
      PpiDescriptor->Guid  = &gFspGlobalDataInitPpiGuid;

      Status = PeiServicesInstallPpi (PpiDescriptor);
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
      }
    }
    Status = PeiServicesNotifyPpi (&mPeiMemoryDiscoveredNotifyDesc);
    ASSERT_EFI_ERROR (Status);
  }
  return Status;
}

/**
  This function is called after PEI core discover memory and finish migration.

  @param[in] PeiServices    Pointer to PEI Services Table.
  @param[in] NotifyDesc     Pointer to the descriptor for the Notification event that
                            caused this function to execute.
  @param[in] Ppi            Pointer to the PPI data associated with this function.

  @retval EFI_STATUS        Always return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
PeiMemoryDiscoveredNotify (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS              Status;
  VOID                    *FspGlobalDataInitPpi;
  FSP_GLOBAL_DATA         *FspData;

  FspData            = NULL;

  Status = PeiServicesLocatePpi (
             &gFspGlobalDataInitPpiGuid,
             0,
             NULL,
             (VOID **) &FspGlobalDataInitPpi
             );

  if (!EFI_ERROR (Status)) {
    FspData = (FSP_GLOBAL_DATA *) FspGlobalDataInitPpi;
    SetFspGlobalDataPointer (FspData);
  }

  return EFI_SUCCESS;
}
