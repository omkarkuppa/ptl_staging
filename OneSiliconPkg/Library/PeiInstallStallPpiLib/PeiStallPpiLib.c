/** @file
  Library to install StallPpi.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#include <Ppi/Stall.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/PeiServicesLib.h>

#define PEI_STALL_RESOLUTION   1

/**
  This function provides a blocking stall for reset at least the given number of microseconds
  stipulated in the final argument.

  @param  PeiServices General purpose services available to every PEIM.
  @param  this Pointer to the local data for the interface.
  @param  Microseconds number of microseconds for which to stall.

  @retval  EFI_SUCCESS the function provided at least the required stall.
**/
EFI_STATUS
EFIAPI
Stall (
  IN CONST EFI_PEI_SERVICES   **PeiServices,
  IN CONST EFI_PEI_STALL_PPI  *This,
  IN UINTN                    Microseconds
  );


EFI_PEI_STALL_PPI   mStallPpi = {
  PEI_STALL_RESOLUTION,
  Stall
};

EFI_PEI_PPI_DESCRIPTOR    mPeiInstallStallPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiStallPpiGuid,
  &mStallPpi
};

EFI_STATUS
EFIAPI
Stall (
  IN CONST EFI_PEI_SERVICES   **PeiServices,
  IN CONST EFI_PEI_STALL_PPI  *This,
  IN UINTN                    Microseconds
  )
{
  MicroSecondDelay (Microseconds);
  return EFI_SUCCESS;
}

/**
  This function will install the StallPpi.

  @retval EFI_SUCCESS if StallPpi is installed successfully.
**/
EFI_STATUS
EFIAPI
InstallStallPpi (
  VOID
  )
{
  EFI_STATUS   Status;

  DEBUG((DEBUG_INFO, "Installing StallPpi \n"));

  Status = PeiServicesInstallPpi (&mPeiInstallStallPpi);
  ASSERT_EFI_ERROR (Status);

  return Status;
}
