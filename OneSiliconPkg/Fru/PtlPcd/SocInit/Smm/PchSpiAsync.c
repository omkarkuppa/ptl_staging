/** @file
  PCH SPI Async SMI handler.

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
#include "PchInitSmm.h"

///
/// Global variables
///
GLOBAL_REMOVE_IF_UNREFERENCED PCH_SMI_DISPATCH_PROTOCOL     *mPchSmiDispatchProtocol;

/**
  This hardware SMI handler will be run every time the flash write/earse happens.

  @param[in] DispatchHandle       Not used

**/
VOID
EFIAPI
PchSpiAsyncCallback (
  IN  EFI_HANDLE                              DispatchHandle
  )
{
  //
  // Dummy SMI handler
  //
}

/**
  This fuction install SPI ASYNC SMI handler.

  @retval EFI_SUCCESS             Initialization complete.
**/
EFI_STATUS
EFIAPI
InstallPchSpiAsyncSmiHandler (
  VOID
  )
{
  EFI_STATUS              Status;
  EFI_HANDLE              Handle;

  DEBUG ((DEBUG_INFO, "InstallPchSpiAsyncSmiHandler()\n"));

  ///
  /// Get the PCH SMM dispatch protocol
  ///
  mPchSmiDispatchProtocol = NULL;
  Status = gSmst->SmmLocateProtocol (&gPchSmiDispatchProtocolGuid, NULL, (VOID **) &mPchSmiDispatchProtocol);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Register an SpiAsync callback function
  ///
  Handle = NULL;
  Status = mPchSmiDispatchProtocol->SpiAsyncRegister (
                                      mPchSmiDispatchProtocol,
                                      PchSpiAsyncCallback,
                                      &Handle
                                      );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

