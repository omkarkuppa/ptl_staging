/** @file
  Example of using Beep PPI

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

#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/BeepPpi.h>

/**
  This function is the entry point of the Beep example driver in the PEI phase.
  The driver  illustrates  an example of consuming and using Beep PPI.

  @param  FileHandle             Handle of the file being invoked.
  @param  PeiServices            Describes the list of possible PEI Services.

  @retval EFI_SUCCESS            The driver is successfully initialized.
  @retval EFI_NOT_FOUND          Beep PPI has not been found.
  @retval EFI_UNSUPPORTED        Failed using the Beep interface.
**/
EFI_STATUS
EFIAPI
PeiBeepExampleEntryPoint (
  IN EFI_PEI_FILE_HANDLE       FileHandle,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
{
  EFI_STATUS        Status;
  BEEP_PPI          *BeepPpi;

  DEBUG ((DEBUG_INFO, "%a () Start.\n", __FUNCTION__));

  ///
  ///  Locate required PPIs.
  ///
  Status = PeiServicesLocatePpi (&gBeepPpiGuid, 0, NULL, (VOID **) &BeepPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): Failed to locate PPI - %r\n", __FUNCTION__, Status));
    return EFI_NOT_FOUND;
  }

  Status = BeepPpi->Enable (BeepPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): Failed to enable the interface - %r\n", __FUNCTION__, Status));
    return EFI_UNSUPPORTED;
  }

  Status = BeepPpi->Beep (BeepPpi, 250, 0, 2, 1000, 500);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): Failed to beep on the desired codec - %r\n", __FUNCTION__, Status));
    return EFI_UNSUPPORTED;
  }

  Status = BeepPpi->Disable (BeepPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): Failed to disable the interface - %r\n", __FUNCTION__, Status));
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));

  return Status;
}
