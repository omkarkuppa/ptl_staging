/** @file

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

#include <PiDxe.h>
#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PerformanceLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/HeciTransportCoreLib.h>
#include <Library/HeciUtilsLib.h>
#include <Ppi/HeciTransportPpi.h>
#include <Ppi/HeciAccessPpi.h>
#include <Ppi/MemoryDiscovered.h>

#define HECI_TRANSPORT_DEBUG    "[HECI Transport]"

EFI_STATUS
EFIAPI
InitializeHeciTransportCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

static EFI_PEI_NOTIFY_DESCRIPTOR  mInitializeHeciTransportPpiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gHeciAccessPpiGuid,
  InitializeHeciTransportCallback
};

/**
  Install HECI Transport PPI and link it to given HECI Access

  @param PeiServices              General purpose services available to every PEIM.
  @param NotifyDescriptor         Information about the notify event.
  @param Ppi                      Context

  @retval EFI_SUCCESS             Successfully initialized PPI
  @retval EFI_ABORTED             No need to install/reinstall PPI
  @retval EFI_NOT_FOUND           Unable to perform initialization
  @retval EFI_OUT_OF_RESOURCES    Unable to allocate memory
  @retval EFI_INVALID_PARAMETER   Pointer to Ppi is NULL
**/
EFI_STATUS
EFIAPI
InitializeHeciTransportCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                     Status;
  EFI_PEI_PPI_DESCRIPTOR         *HeciTransportPpi;
  EFI_PEI_PPI_DESCRIPTOR         *HeciTransportOldPpi;
  HECI_TRANSPORT_PRIVATE         *HeciTransportPrivate;
  HECI_TRANSPORT_PRIVATE         *HeciTransportPrivateOld;
  HECI_ACCESS                    *HeciAccess;
  UINTN                          HeciTransportInstance;
  BOOLEAN                        Reinstall;

  Reinstall = FALSE;

  DEBUG ((DEBUG_INFO, "%a %a () - Start\n", HECI_TRANSPORT_DEBUG, __FUNCTION__));

  if (Ppi == NULL) {
    DEBUG ((DEBUG_ERROR, "%a HeciAccess is NULL!\n", HECI_TRANSPORT_DEBUG));
    return EFI_INVALID_PARAMETER;
  }

  HeciAccess = (HECI_ACCESS *) Ppi;

  HeciTransportPpi = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (HeciTransportPpi == NULL) {
    DEBUG ((DEBUG_ERROR, "%a Memory allocation failed!\n", HECI_TRANSPORT_DEBUG));
    return EFI_OUT_OF_RESOURCES;
  }

  HeciTransportPrivate = (HECI_TRANSPORT_PRIVATE *) AllocateZeroPool (sizeof (HECI_TRANSPORT_PRIVATE));
  if (HeciTransportPrivate == NULL) {
    DEBUG ((DEBUG_ERROR, "%a Memory allocation failed!\n", HECI_TRANSPORT_DEBUG));
    FreePool (HeciTransportPpi);
    return EFI_OUT_OF_RESOURCES;
  }

  HeciTransportPrivateOld = (HECI_TRANSPORT_PRIVATE *) GetHeciTransportForHeciAccess (HeciAccess, &HeciTransportInstance);

  // If the HeciTransport was found, we should reinstall it
  if (HeciTransportPrivateOld != NULL) {
    Reinstall = TRUE;
  }

  if (Reinstall) {
    CopyMem (HeciTransportPrivate, HeciTransportPrivateOld, sizeof (HECI_TRANSPORT_PRIVATE));
    SetHeciTransportFunctions (HeciTransportPrivate);
    HeciTransportPrivate->HeciAccess = HeciAccess;
  } else {
    Status = HeciInitializeNewTransportInstance (HeciTransportPrivate, HeciAccess);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a Initialization failed!\n", HECI_TRANSPORT_DEBUG));
      FreePool (HeciTransportPpi);
      FreePool (HeciTransportPrivate);
      return Status;
    }

    HeciTransportPrivate->HeciAccessIndex = GetPpiInstanceIndex ((VOID *) HeciAccess, &gHeciAccessPpiGuid);
  }

  // Create new HeciTransport PPI
  HeciTransportPpi->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  HeciTransportPpi->Guid  = &gHeciTransportPpiGuid;
  HeciTransportPpi->Ppi   = HeciTransportPrivate;

  if (Reinstall) {
    Status = PeiServicesLocatePpi (
                &gHeciTransportPpiGuid,
                HeciTransportPrivate->HeciAccessIndex,
                &HeciTransportOldPpi,
                (VOID **) &HeciTransportPrivateOld
                );
    if (!EFI_ERROR (Status)) {
      Status = PeiServicesReInstallPpi (HeciTransportOldPpi, HeciTransportPpi);
    } else {
      DEBUG ((DEBUG_ERROR, "%a PPI with given index could not be found!\n", HECI_TRANSPORT_DEBUG));
      FreePool (HeciTransportPpi);
      FreePool (HeciTransportPrivate);
      return Status;
    }
  } else {
    Status = PeiServicesInstallPpi (HeciTransportPpi);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a PPI installation failed!\n", HECI_TRANSPORT_DEBUG));
      FreePool (HeciTransportPpi);
      FreePool (HeciTransportPrivate);
      return Status;
    }
  }

  DEBUG ((
    DEBUG_INFO,
    "%a HeciTransport %ainstalled (for HeciAccess instance: %d)\n",
    HECI_TRANSPORT_DEBUG,
    Reinstall ? "re" : "",
    HeciTransportPrivate->HeciAccessIndex
    ));
  DEBUG ((DEBUG_INFO, "%a %a () - End\n", HECI_TRANSPORT_DEBUG, __FUNCTION__));

  return Status;
}

/**
  HECI driver entry point used to initialize support for the HECI device.

  @param[in] ImageHandle          Standard entry point parameter.
  @param[in] SystemTable          Standard entry point parameter.

  @retval EFI_SUCCESS             Callback registered successfully
  @retval EFI_INVALID_PARAMETER   Invalid parameter passed to PeiServicesNotify
  @retval EFI_OUT_OF_RESOURCES    There is no additional space in the PPI database
**/
EFI_STATUS
EFIAPI
HeciTransportEntryPoint (
  IN       EFI_PEI_FILE_HANDLE   FileHandle,
  IN CONST EFI_PEI_SERVICES      **PeiServices
  )
{
  EFI_STATUS    Status;
  VOID          *Ppi;

  DEBUG ((DEBUG_INFO, "%a %a () - Start\n", HECI_TRANSPORT_DEBUG, __FUNCTION__));

  Status = PeiServicesLocatePpi (
             &gHeciTransportPpiGuid,
             0,
             NULL,
             (VOID **) &Ppi
             );
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a %a () - End. HeciTransport already installed.\n", HECI_TRANSPORT_DEBUG, __FUNCTION__));
    return EFI_SUCCESS;
  }

  //
  // Register a callback to initialize/reinitialize HeciTransport
  //
  Status = PeiServicesNotifyPpi (&mInitializeHeciTransportPpiNotifyList);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a Notify registration failed! Status: %r\n", HECI_TRANSPORT_DEBUG, Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "%a %a () - Exit: %r\n", HECI_TRANSPORT_DEBUG, __FUNCTION__, Status));

  return EFI_SUCCESS;
}
