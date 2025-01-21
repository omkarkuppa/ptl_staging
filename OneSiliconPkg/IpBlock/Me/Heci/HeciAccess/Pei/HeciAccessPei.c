/** @file
  HECI Access protocol driver for use in Pei

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#include <Base.h>
#include <Uefi.h>
#include <IndustryStandard/Pci22.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PerformanceLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/HobLib.h>
#include <Library/PmcLib.h>
#include <Library/HeciAccessCoreLib.h>
#include <Library/HeciUtilsLib.h>
#include <Ppi/HeciAccessPpi.h>
#include <Ppi/MemoryDiscovered.h>
#include <HeciDeviceConfig.h>

#define HECI_ACCESS_DEBUG   "[HECI Access]"

EFI_STATUS
EFIAPI
ReinstallHeciAccessPpis (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

static EFI_PEI_NOTIFY_DESCRIPTOR  mReinitializeHeciAccessNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  ReinstallHeciAccessPpis
  };

/**
  Get next HECI Access

  @param[in] This     Pointer to HECI Access protocol instance from which the next instance should be returned.

  @retval Pointer to the next HeciAccess protocol instance
**/
HECI_ACCESS *
EFIAPI
GetNextHeciAccess (
  IN HECI_ACCESS    *This
  )
{
  return GetNextPpi (This, &gHeciAccessPpiGuid);
}

/**
  Once main memory has been initialized, locate and re-initialize HECI Access

  @param[in] PeiServices        General purpose services available to every PEIM.
  @param[in] NotifyDescriptor   Information about the notify event.
  @param[in] Ppi                Context
**/
EFI_STATUS
EFIAPI
ReinstallHeciAccessPpis (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                Status;
  EFI_PEI_PPI_DESCRIPTOR    *HeciAccessPpi;
  EFI_PEI_PPI_DESCRIPTOR    *HeciAccessOldPpi;
  HECI_ACCESS_PRIVATE       *HeciAccess;
  HECI_ACCESS_PRIVATE       *HeciAccessOld;
  UINTN                     PpiIndex;
  VOID                      *MemoryDiscoveredPpi;

  DEBUG ((DEBUG_INFO, "%a %a () - Start\n", HECI_ACCESS_DEBUG, __FUNCTION__));

  Status = PeiServicesLocatePpi (
             &gEfiPeiMemoryDiscoveredPpiGuid,
             0,
             NULL,
             (VOID **) &MemoryDiscoveredPpi
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a Called before memory initialization - Exit\n", HECI_ACCESS_DEBUG));
    return EFI_ABORTED;
  }

  PpiIndex = 0;
  Status = PeiServicesLocatePpi (
             &gHeciAccessPpiGuid,
             PpiIndex,
             &HeciAccessOldPpi,
             (VOID **) &HeciAccessOld
             );
  while (!EFI_ERROR (Status)) {
    HeciAccess = AllocateZeroPool (sizeof (HECI_ACCESS_PRIVATE));
    if (HeciAccess == NULL) {
      DEBUG ((DEBUG_ERROR, "%a Memory allocation failed!\n", HECI_ACCESS_DEBUG));
      break;
    }

    CopyMem (HeciAccess, HeciAccessOld, sizeof (HECI_ACCESS_PRIVATE));
    SetHeciAccessFunctions (HeciAccess);

    HeciAccessPpi = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
    if (HeciAccessPpi == NULL) {
      DEBUG ((DEBUG_ERROR, "%a Memory allocation failed!\n", HECI_ACCESS_DEBUG));
      FreePool (HeciAccess);
      break;
    }

    HeciAccessPpi->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
    HeciAccessPpi->Guid  = &gHeciAccessPpiGuid;
    HeciAccessPpi->Ppi   = HeciAccess;

    Status = PeiServicesReInstallPpi (HeciAccessOldPpi, HeciAccessPpi);
    if (EFI_ERROR (Status)) {
      FreePool (HeciAccess);
      FreePool (HeciAccessPpi);
      DEBUG ((DEBUG_ERROR, "%a PPI reinstallation failed!\n", HECI_ACCESS_DEBUG));
      break;
    }

    DEBUG ((DEBUG_INFO, "%a Reinstalled HECI Access %d PPI\n", HECI_ACCESS_DEBUG, PpiIndex));

    PpiIndex++;
    Status = PeiServicesLocatePpi (
               &gHeciAccessPpiGuid,
               PpiIndex,
               &HeciAccessOldPpi,
               (VOID **) &HeciAccessOld
               );
  }

  DEBUG ((DEBUG_INFO, "%a Reinstalled %d PPIs\n", HECI_ACCESS_DEBUG, PpiIndex));
  DEBUG ((DEBUG_INFO, "%a %a () - Exit\n", HECI_ACCESS_DEBUG, __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  Install HECI Access PPIs

  @retval EFI_SUCCESS       Successfully initialized PPI
  @retval EFI_ABORTED       There are no HECI devices configuration
**/
EFI_STATUS
InitializeHeciAccess (
  VOID
  )
{
  EFI_STATUS                Status;
  EFI_HOB_GUID_TYPE         *GuidHob;
  HECI_DEVICE_CONFIG        *HeciDeviceConfig;
  HECI_ACCESS_PRIVATE       *HeciAccess;
  EFI_PEI_PPI_DESCRIPTOR    *HeciAccessPpi;
  UINT8                     InstalledHeciAccessCount;

  DEBUG ((DEBUG_INFO, "%a %a () - Start\n", HECI_ACCESS_DEBUG, __FUNCTION__));

  Status = EFI_NOT_FOUND;
  InstalledHeciAccessCount = 0;

  GuidHob = GetFirstGuidHob (&gHeciDeviceConfigHobGuid);
  if (GuidHob == NULL) {
    DEBUG ((DEBUG_ERROR, "No HECI devices configuration found!\n"));
    return EFI_ABORTED;
  }

  //
  // Install HECI Access PPI for each HECI device
  //
  while (GuidHob != NULL) {
    HeciDeviceConfig = (HECI_DEVICE_CONFIG *)(GET_GUID_HOB_DATA (GuidHob));

    DEBUG ((
      DEBUG_INFO,
      "%a Install Heci Access for HECI device at location [%d:%d:%d:%d]\n",
      HECI_ACCESS_DEBUG,
      HeciDeviceConfig->PciSegment,
      HeciDeviceConfig->PciBus,
      HeciDeviceConfig->PciDevice,
      HeciDeviceConfig->PciFunction));
    HeciAccess = AllocateZeroPool (sizeof (HECI_ACCESS_PRIVATE));
    if (HeciAccess == NULL) {
      DEBUG ((DEBUG_ERROR, "%a Memory allocation failed!\n", HECI_ACCESS_DEBUG));
      return EFI_OUT_OF_RESOURCES;
    }

    Status = HeciSetNewDevice (HeciAccess, HeciDeviceConfig);
    if (!EFI_ERROR (Status)) {
      HeciAccessPpi = (EFI_PEI_PPI_DESCRIPTOR*) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
      if (HeciAccessPpi == NULL) {
        DEBUG ((DEBUG_ERROR, "%a Memory allocation failed!\n", HECI_ACCESS_DEBUG));
        FreePool (HeciAccess);
        return EFI_OUT_OF_RESOURCES;
      }

      HeciAccessPpi->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
      HeciAccessPpi->Guid  = &gHeciAccessPpiGuid;
      HeciAccessPpi->Ppi   = HeciAccess;

      Status = PeiServicesInstallPpi (HeciAccessPpi);
      if (!EFI_ERROR (Status)) {
        InstalledHeciAccessCount++;
        DEBUG ((
          DEBUG_INFO,
          "%a Heci Access %d has been installed\n",
          HECI_ACCESS_DEBUG,
          InstalledHeciAccessCount
        ));
      } else {
        DEBUG ((DEBUG_ERROR, "%a PPI installation failed!\n", HECI_ACCESS_DEBUG));
        FreePool (HeciAccess);
        FreePool (HeciAccessPpi);
      }
    } else {
      FreePool (HeciAccess);
    }

    GuidHob = GET_NEXT_HOB (GuidHob);
    GuidHob = GetNextGuidHob (&gHeciDeviceConfigHobGuid, GuidHob);
  }

  DEBUG ((DEBUG_INFO, "%a Installed %d PPIs\n", HECI_ACCESS_DEBUG, InstalledHeciAccessCount));
  DEBUG ((DEBUG_INFO, "%a %a () - End\n", HECI_ACCESS_DEBUG, __FUNCTION__));

  return Status;
}

/**
  HECI driver entry point used to initialize support for the HECI device.

  @param[in] ImageHandle          Standard entry point parameter.
  @param[in] SystemTable          Standard entry point parameter.

  @retval EFI_SUCCESS             HeciAccess installed successfully
**/
EFI_STATUS
EFIAPI
HeciAccessEntryPoint (
  IN       EFI_PEI_FILE_HANDLE   FileHandle,
  IN CONST EFI_PEI_SERVICES      **PeiServices
  )
{
  EFI_STATUS    Status;
  VOID          *Ppi;

  DEBUG ((DEBUG_INFO, "%a %a () - Start\n", HECI_ACCESS_DEBUG, __FUNCTION__));

  Status = PeiServicesLocatePpi (
             &gHeciAccessPpiGuid,
             0,
             NULL,
             (VOID **) &Ppi
             );
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a %a () - End. HeciAccess already installed.\n", HECI_ACCESS_DEBUG, __FUNCTION__));
    return EFI_SUCCESS;
  }

  if (PmcIsDwrBootMode ()) {
    DEBUG ((DEBUG_WARN, "%a DWR detected - Exit\n", HECI_ACCESS_DEBUG));
    return EFI_SUCCESS;
  }

  Status = InitializeHeciAccess ();
  if (!EFI_ERROR (Status)) {
    //
    // Register a callback to reinitialize HeciAccess after memory discovered
    //
    Status = PeiServicesNotifyPpi (&mReinitializeHeciAccessNotifyList);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "%a Notify registration failed! Status: %r.\n", HECI_ACCESS_DEBUG, Status));
      return Status;
    }
  } else {
    DEBUG ((DEBUG_ERROR, "%a Initialization failed! Status: %r\n", HECI_ACCESS_DEBUG, Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "%a %a () - Exit: %r.\n", HECI_ACCESS_DEBUG, __FUNCTION__, Status));

  return Status;
}
