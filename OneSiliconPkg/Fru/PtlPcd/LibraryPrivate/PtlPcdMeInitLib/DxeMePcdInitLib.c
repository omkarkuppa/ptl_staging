/** @file
  PTL PCD ME Init specific routines for DXE

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

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PmcLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/PmcLib.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <MeHandle.h>
#include "MePcdInitLib.h"

/**
  Initialize PCD ME handle structure

  @param[in] MeHandlePrivate        Pointer to the ME Private Handle
**/
STATIC
VOID
PtlPcdInitMeHandle (
  IN ME_HANDLE                      *MeHandlePrivate
  )
{
  MeHandlePrivate->HeciTempMmio                        = 0;
  MeHandlePrivate->PmcPwrmBase                         = PmcGetPwrmBase ();
  MeHandlePrivate->Callbacks->GetCsmeHeciSbAccess      = PtlPcdGetCsmeHeciSbAccess;
  MeHandlePrivate->Callbacks->GetCsmeHeciPciCfgBase    = PtlPcdGetCsmeHeciPciCfgBase;
  MeHandlePrivate->Callbacks->GetPseHeciPciCfgBase     = PtlPcdGetPseHeciPciCfgBase;
  //
  // SetCsmeHeciDeviceState / SetCsmeHeciInterruptPin not supported in DXE
  //
  MeHandlePrivate->Callbacks->SetCsmeHeciDeviceState   = NULL;
  MeHandlePrivate->Callbacks->SetCsmeHeciInterruptPin  = NULL;
}

/**
  Installs the ME (Management Engine) Handle Protocol.

  This function allocates memory for the ME_HANDLE structure and initializes it.
  It then allocates memory for the ME_CALLBACKS structure and attaches it to the
  ME_HANDLE. After initializing the ME_HANDLE with platform-specific details
  through the PtlPcdInitMeHandle function, it installs the ME_HANDLE protocol
  onto a new handle in the UEFI handle database.

  If any memory allocations fail, the function will assert and return early.
  If the protocol installation fails, it will log an error message with the
  status code.

  @retval VOID  This function does not return a value and has no return statement.
**/
VOID
InstallMeHandle (
  VOID
  )
{
  EFI_STATUS    Status;
  EFI_HANDLE    NewHandle;
  ME_HANDLE     *MeHandle;

  MeHandle = (ME_HANDLE *) AllocateZeroPool (sizeof (ME_HANDLE));
  if (MeHandle == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Allocating memory for MeHandle failed!\n", __FUNCTION__));
    ASSERT (FALSE);
    return;
  }

  MeHandle->Callbacks = (ME_CALLBACKS *) AllocateZeroPool (sizeof (ME_CALLBACKS));
  if (MeHandle->Callbacks == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Allocating memory for MeCallbacks failed!\n", __FUNCTION__));
    ASSERT (FALSE);
    return;
  }

  PtlPcdInitMeHandle (MeHandle);

  NewHandle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &NewHandle,
                  &gMeHandleProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  MeHandle
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: MeHandle Protocol installation failed! Status = %r\n", __FUNCTION__, Status));
  }
}

/**
  Retrieves the ME (Management Engine) Handle Protocol.

  @param[out] MeHandle            The pointer to a pointer to the ME_HANDLE structure that
                                  will be updated with the ME_HANDLE protocol instance.

  @retval EFI_SUCCESS             The ME_HANDLE protocol was located and returned successfully.
  @retval EFI_NOT_FOUND           The ME_HANDLE protocol could not be located.
  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval Other                   An error returned by the LocateProtocol function.
**/
EFI_STATUS
GetMeHandle (
  OUT ME_HANDLE    **MeHandle
  )
{
  EFI_STATUS    Status;

  Status = gBS->LocateProtocol (&gMeHandleProtocolGuid, NULL, (VOID **) MeHandle);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "%a () LocateProtocol failed! Status = %r\n", __FUNCTION__, Status));
    *MeHandle = NULL;
  }

  return Status;
}