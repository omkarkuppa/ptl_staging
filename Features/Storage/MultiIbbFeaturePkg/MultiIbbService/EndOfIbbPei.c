/** @file
  Source code file for callback function at the end of every IBB

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include <Base.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>

EFI_PEI_PPI_DESCRIPTOR gEndOfIbbSignalPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfIbbSignalPpiGuid,
  NULL
};

/**
  Trigger the callback function at the end of the current Ibb.

  @param[in] FileHandle   Handle of the file being invoked.
  @param[in] PeiServices  Describes the list of possible PEI Services.

  @retval    EFI_SUCCESS  Install gEndOfIbbSignalPpi successfully
**/
EFI_STATUS
EFIAPI
EndOfIbbEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
)
{
  EFI_STATUS                 Status;

  DEBUG ((DEBUG_INFO, "Enter EndOfIbb entry point.\n"));

  // Install gEndOfIbbSignalPpi
  Status = PeiServicesInstallPpi (&gEndOfIbbSignalPpi);
  ASSERT_EFI_ERROR (Status);

  return Status;
}