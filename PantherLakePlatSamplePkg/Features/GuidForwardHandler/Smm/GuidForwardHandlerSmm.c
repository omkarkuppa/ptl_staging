/** @file
  Platform PPV GUID forward code implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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

#include "GuidForwardHandlerSmm.h"

EFI_SMM_RSC_HANDLER_PROTOCOL  *mRscHandlerProtocol       = NULL;



/**
  Entry point of SMM Guid Forward Driver.

  This function is the entry point of SMM Status Code Driver.

  @param[in]  ImageHandle   The firmware allocated handle for the EFI image.
  @param[in]  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.

**/
EFI_STATUS
EFIAPI
GuidForwardHandlerSmmEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                Status;

  if (!(PcdGetBool (PcdGuidForward))) {
    return EFI_SUCCESS;
  }

  Status = gSmst->SmmLocateProtocol (
                    &gEfiSmmRscHandlerProtocolGuid,
                    NULL,
                    (VOID **) &mRscHandlerProtocol
                    );
  ASSERT_EFI_ERROR (Status);

  mRscHandlerProtocol->Register (GuidForwardWorker);

  return EFI_SUCCESS;
}

