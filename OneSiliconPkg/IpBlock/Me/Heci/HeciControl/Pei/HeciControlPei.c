/** @file
  HECI Control protocol driver (Pei)

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
#include <Library/PciSegmentLib.h>
#include <Library/HeciControlCoreLib.h>
#include <Library/HeciUtilsLib.h>
#include <Ppi/HeciTransportPpi.h>
#include <Ppi/HeciControlPpi.h>
#include <Ppi/MemoryDiscovered.h>
#include <IntelRcStatusCode.h>
#include <Register/HeciRegs.h>
#include <IndustryStandard/Pci22.h>
#include <MeDefines.h>

#define HECI_CONTROL_DEBUG    "[HECI Control PEI]"

EFI_STATUS
EFIAPI
HeciControlReinstallCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

static EFI_PEI_NOTIFY_DESCRIPTOR  mReinstallHeciControlOnMemoryDiscovered = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  HeciControlReinstallCallback
};

/**
  HECI Control driver entry point used to initialize support for the HECI device.

  @param[in] ImageHandle          Standard entry point parameter.
  @param[in] SystemTable          Standard entry point parameter.

  @retval EFI_SUCCESS             HECI Control initialized successfully
  @retval EFI_OUT_OF_RESOURCES    Can't allocate PPI data
  @retval EFI_ABORTED             HECI Control initialization aborted
**/
EFI_STATUS
EFIAPI
HeciControlEntryPoint (
  IN       EFI_PEI_FILE_HANDLE   FileHandle,
  IN CONST EFI_PEI_SERVICES      **PeiServices
  )
{
  EFI_STATUS                     Status;
  HECI_CONTROL_PRIVATE           *HeciControl;
  EFI_PEI_PPI_DESCRIPTOR         *HeciControlPpi;
  HECI_TRANSPORT                 *HeciTransport;

  DEBUG ((DEBUG_INFO, "%a %a () - Start\n", HECI_CONTROL_DEBUG, __FUNCTION__));

  Status          = EFI_SUCCESS;
  HeciControl     = NULL;
  HeciControlPpi  = NULL;
  HeciTransport   = NULL;

  Status = PeiServicesLocatePpi (
             &gHeciControlPpiGuid,
             0,
             NULL,
             (VOID **) &HeciControl
             );

  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a () - End. HECI Control already installed\n", __FUNCTION__));
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "ME-BIOS: HECI Control PPI Entry.\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_HECI_CONTROL_PPI_ENTRY); //PostCode (0xE03)

  Status = PeiServicesLocatePpi (
             &gHeciTransportPpiGuid,
             0,
             NULL,
             (VOID **) &HeciTransport
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ME-BIOS: HECI Control PPI Exit - No HECI Transport Installed.\n"));
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_HECI_CONTROL_PPI_EXIT_ERR_HECIDEV); //PostCode (0xE83)
    DEBUG ((DEBUG_INFO, "There are no HECI Transport installed, skip HECI Control installation\n"));
    return EFI_ABORTED;
  }

  HeciControlPpi = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (HeciControlPpi == NULL) {
    DEBUG ((DEBUG_ERROR, "ME-BIOS: HECI Control PPI Exit - Error by HECI device error.\n"));
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_HECI_CONTROL_PPI_EXIT_ERR_HECIDEV); //PostCode (0xE83)
    DEBUG ((DEBUG_ERROR, "%a () - Memory allocation failed!\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  HeciControl = AllocateZeroPool (sizeof (HECI_CONTROL_PRIVATE));
  if (HeciControl == NULL) {
    DEBUG ((DEBUG_ERROR, "ME-BIOS: HECI Control PPI Exit - Error by HECI device error.\n"));
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_HECI_CONTROL_PPI_EXIT_ERR_HECIDEV); //PostCode (0xE83)
    DEBUG ((DEBUG_ERROR, "%a () - Memory allocation failed!\n", __FUNCTION__));
    FreePool (HeciControlPpi);
    return EFI_OUT_OF_RESOURCES;
  }

  SetHeciControlFunctions (HeciControl);

  HeciControlPpi->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  HeciControlPpi->Guid  = &gHeciControlPpiGuid;
  HeciControlPpi->Ppi   = HeciControl;

  Status = PeiServicesInstallPpi (HeciControlPpi);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ME-BIOS: HECI Control PPI Exit - Error by install HeciControlPpi fail, Status: %r\n", Status));
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_HECI_CONTROL_PPI_EXIT_ERR_INSTALLPPIFAIL); //PostCode (0xEA3)
    DEBUG ((DEBUG_ERROR, "%a () - HeciControll PPI installation failed!\n", __FUNCTION__));
    FreePool (HeciControlPpi);
    FreePool (HeciControl);
    return EFI_ABORTED;
  }

  //
  // HECI1 and HECI2/3 are loaded in different ways.
  // HECI1 is handled by CSE BUP but HECI2/3 are handled later in the CSE driver.
  // In this phase, only HECI1 can be initialized.
  // HECI2 and HECI3 will be initialized by HeciControlReinstallCallback () in Memory Discovered.
  //
  HeciControlInitalizeSpecificCommunicationDevice (HeciControl, HECI1);

  Status = PeiServicesNotifyPpi (&mReinstallHeciControlOnMemoryDiscovered);
  if (EFI_ERROR (Status)) {
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_HECI_CONTROL_PPI_EXIT_ERR_INSTALLPPIFAIL); //PostCode (0xEA3)
    DEBUG ((DEBUG_ERROR, "%a HeciControll PPI installation failed!\n", HECI_CONTROL_DEBUG));
    DEBUG ((DEBUG_ERROR, "%a Callback installation failed!\n", HECI_CONTROL_DEBUG));
    FreePool (HeciControlPpi);
    FreePool (HeciControl);
    return EFI_ABORTED;
  }

  DEBUG ((DEBUG_INFO, "ME-BIOS: HECI Control PPI Exit - Success.\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_HECI_CONTROL_PPI_EXIT_SUCCESS); //PostCode (0xE23)
  DEBUG ((DEBUG_INFO, "%a %a () - End\n", HECI_CONTROL_DEBUG, __FUNCTION__));

  return Status;
}

/**
  Reinitialize HECI Control on MemoryDiscovered

  @param PeiServices            General purpose services available to every PEIM.
  @param NotifyDescriptor       Information about the notify event.
  @param Ppi                    Context

  @retval EFI_SUCCESS           Successfully initialized PPI
  @retval EFI_NOT_FOUND         Unable to perform initialization
  @retval EFI_OUT_OF_RESOURCES  Not enough memory to initialize
**/
EFI_STATUS
EFIAPI
HeciControlReinstallCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                Status;
  EFI_PEI_PPI_DESCRIPTOR    *HeciControlOldPpi;
  EFI_PEI_PPI_DESCRIPTOR    *HeciControlPpi;
  HECI_CONTROL_PRIVATE      *HeciControlOld;
  HECI_CONTROL_PRIVATE      *HeciControl;

  DEBUG ((DEBUG_INFO, "%a %a () - Start\n", HECI_CONTROL_DEBUG, __FUNCTION__));

  Status = PeiServicesLocatePpi (
             &gHeciControlPpiGuid,
             0,
             &HeciControlOldPpi,
             (VOID **) &HeciControlOld
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a %a () - HECI Control is not installed!\n", HECI_CONTROL_DEBUG, __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  HeciControl = AllocateZeroPool (sizeof (HECI_CONTROL_PRIVATE));
  if (HeciControl == NULL) {
    DEBUG ((DEBUG_ERROR, "%a Memory allocation failed!\n", HECI_CONTROL_DEBUG));
    return EFI_OUT_OF_RESOURCES;
  }

  CopyMem (HeciControl, HeciControlOld, sizeof (HECI_CONTROL_PRIVATE));
  SetHeciControlFunctions (HeciControl);
  HeciControlInitalizeAllCommunicationDevices (HeciControl);

  HeciControlPpi = (EFI_PEI_PPI_DESCRIPTOR*) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (HeciControlPpi == NULL) {
    DEBUG ((DEBUG_ERROR, "%a Memory allocation failed!\n", HECI_CONTROL_DEBUG));
    FreePool (HeciControl);
    return EFI_OUT_OF_RESOURCES;
  }

  HeciControlPpi->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  HeciControlPpi->Guid  = &gHeciControlPpiGuid;
  HeciControlPpi->Ppi   = HeciControl;

  Status = PeiServicesReInstallPpi (HeciControlOldPpi, HeciControlPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a HeciControll PPI reinstallation failed!\n", HECI_CONTROL_DEBUG));
    FreePool (HeciControl);
    FreePool (HeciControlPpi);
    return EFI_ABORTED;
  }

  DEBUG ((DEBUG_INFO, "%a () - Reinstalled HECI Control PPI\n", HECI_CONTROL_DEBUG));
  DEBUG ((DEBUG_INFO, "%a %a () - End\n", HECI_CONTROL_DEBUG, __FUNCTION__));

  return EFI_SUCCESS;
}
