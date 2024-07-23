/** @file

  Parts of the SMM/MM implementation that are specific to traditional MM

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

#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmMemLib.h>
#include "Variable.h"

/**
  This function checks if the buffer is valid per processor architecture and
  does not overlap with SMRAM.

  @param Buffer The buffer start address to be checked.
  @param Length The buffer length to be checked.

  @retval TRUE  This buffer is valid per processor architecture and does not
                overlap with SMRAM.
  @retval FALSE This buffer is not valid per processor architecture or overlaps
                with SMRAM.
**/
BOOLEAN
VariableSmmIsBufferOutsideSmmValid (
  IN EFI_PHYSICAL_ADDRESS  Buffer,
  IN UINT64                Length
  )
{
  return SmmIsBufferOutsideSmmValid (Buffer, Length);
}

/**
  Notify the system that the SMM variable driver is ready.
**/
VOID
VariableNotifySmmReady (
  VOID
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  Handle;

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gEfiSmmVariableProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
}

/**
  Notify the system that the SMM variable write driver is ready.
**/
VOID
VariableNotifySmmWriteReady (
  VOID
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  Handle;

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gSmmVariableWriteGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
}

/**
  Variable service MM driver entry point

  @param[in] ImageHandle    A handle for the image that is initializing this
                            driver
  @param[in] SystemTable    A pointer to the EFI system table

  @retval EFI_SUCCESS       Variable service successfully initialized.
**/
EFI_STATUS
EFIAPI
VariableServiceInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return MmVariableServiceInitialize ();
}

/**
  Whether the TCG or TCG2 protocols are installed in the UEFI protocol database.
  This information is used by the MorLock code to infer whether an existing
  MOR variable is legitimate or not.

  @retval TRUE  Either the TCG or TCG2 protocol is installed in the UEFI
                protocol database
  @retval FALSE Neither the TCG nor the TCG2 protocol is installed in the UEFI
                protocol database
**/
BOOLEAN
VariableHaveTcgProtocols (
  VOID
  )
{
  EFI_STATUS  Status;
  VOID        *Interface;

  Status = gBS->LocateProtocol (
                  &gEfiTcg2ProtocolGuid,
                  NULL,                     // Registration
                  &Interface
                  );
  if (!EFI_ERROR (Status)) {
    return TRUE;
  }

  Status = gBS->LocateProtocol (
                  &gEfiTcgProtocolGuid,
                  NULL,                     // Registration
                  &Interface
                  );
  return !EFI_ERROR (Status);
}
