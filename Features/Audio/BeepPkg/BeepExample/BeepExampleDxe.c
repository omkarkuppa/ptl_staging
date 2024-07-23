/** @file
  Example of using Beep Protocol

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
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/BeepProtocol.h>

/**
  This function is the entry point of the Beep example driver in the DXE phase.
  The driver  illustrates  an example of consuming and using Beep Protocol.

  @param  ImageHandle       The firmware allocated handle for the EFI image.
  @param  SystemTable       A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The function completed successfully.
  @retval EFI_NOT_FOUND     Beep Protocol has not been found.
  @retval EFI_UNSUPPORTED   Failed using the Beep interface.
**/
EFI_STATUS
EFIAPI
DxeBeepExampleEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS             Status;
  BEEP_PROTOCOL          *BeepProtocol;

  DEBUG ((DEBUG_INFO, "%a () Start.\n", __FUNCTION__));

  ///
  ///  Locate the Codec Beep Protocol.
  ///
  Status = gBS->LocateProtocol (
                  &gBeepProtocolGuid,
                  NULL,
                  (VOID **) &BeepProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): Failed to locate Protocol - %r\n", __FUNCTION__, Status));
    return EFI_NOT_FOUND;
  }

  Status = BeepProtocol->Enable (BeepProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): Failed to enable the interface - %r\n", __FUNCTION__, Status));
    return EFI_UNSUPPORTED;
  }

  Status = BeepProtocol->Beep (BeepProtocol, 250, 0, 2, 1000, 500);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): Failed to beep on the desired codec - %r\n", __FUNCTION__, Status));
    return EFI_UNSUPPORTED;
  }

  Status = BeepProtocol->Disable (BeepProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): Failed to disable the interface - %r\n", __FUNCTION__, Status));
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));

  return Status;
}
