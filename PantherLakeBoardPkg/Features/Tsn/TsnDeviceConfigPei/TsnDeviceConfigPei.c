/** @file
  TSN Device Configuration.

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

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/GpioV2WrapperLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/EndOfPeiPhase.h>
#include <SetupVariable.h>
#include <GpioV2Config.h>

/**
  TSN Device GPIO Configuration.

  @param PeiServices       An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation
  @param NotifyDescriptor  Address of the notification descriptor data structure.
  @param Ppi               Address of the PPI that was installed.

  @retval EFI_SUCCESS      Successfully update the Boot records.
**/
EFI_STATUS
EFIAPI
TsnDeviceGpioInitEndOfPei (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                       Status;
  PCH_SETUP                        PchSetup;
  UINTN                            VarSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *VariableServices;

  DEBUG ((DEBUG_INFO, "TsnDeviceGpioInit() Start\n"));

  //
  // Locate Setup variables
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "TsnGpioInit: PeiServicesLocatePpi failed\n"));
    return Status;
  }

  VarSize = sizeof (PCH_SETUP);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"PchSetup",
                               &gPchSetupVariableGuid,
                               NULL,
                               &VarSize,
                               &PchSetup
                               );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "TsnGpioInit: GetVariable (PchSetup) failed\n"));
    return Status;
  }

  if (PchSetup.PchTsnEnable [0] == 1 || PchSetup.PchTsnEnable [1] == 1 || PchSetup.PchTsnEnable [2] == 1 || PchSetup.PchTsnEnable [3] == 1 ) {
    DEBUG ((DEBUG_INFO, "TSN Device GpioInit\n"));
    Status = GpioV2ConfigurePads (
               (GPIOV2_INIT_CONFIG *) (UINTN) PcdGet32 (PcdBoardGpioTableTsnDevice),
               (UINT32) PcdGet16 (PcdBoardGpioTableTsnDeviceSize)
               );
    ASSERT_EFI_ERROR (Status);

    Status = GpioV2SetConflictCheckData (
               (GPIOV2_INIT_CONFIG *) (UINTN) PcdGet32 (PcdBoardGpioTableTsnDevice),
               (UINT32) PcdGet16 (PcdBoardGpioTableTsnDeviceSize)
               );
    ASSERT_EFI_ERROR (Status);
  } else {
    //
    // Disable Tsn Pcs if TsnEnable feature not available
    //
    PchSetup.TsnPcsEnabled = 0;
    Status = EFI_SUCCESS;
  }
  DEBUG ((DEBUG_INFO, "TsnDeviceGpioInit() End\n"));
  return Status;
}

static EFI_PEI_NOTIFY_DESCRIPTOR  mEndOfPeiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) TsnDeviceGpioInitEndOfPei
};

/**
  Main Entry Of TSN Device Configuration

  @param[in]  FileHandle   Handle of the file being invoked.
  @param[in]  PeiServices  Pointer to PEI Services table.

  @retval EFI_SUCCESS Install function successfully.

**/
EFI_STATUS
EFIAPI
TsnDeviceConfigPeiEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS     Status;

  //
  // Performing PlatformInitEndOfPei after EndOfPei PPI produced
  //
  Status = PeiServicesNotifyPpi (&mEndOfPeiNotifyList);

  return Status;
}
