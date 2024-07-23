/** @file
  SMM Smbus Driver Entry

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2009 Intel Corporation.

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
#include "SmbusSmm.h"

SMBUS_INSTANCE  *mSmbusContext;
/**
  Execute an SMBUS operation

  @param[in] This                 The protocol instance
  @param[in] DeviceAddress        The address of the SMBUS device
  @param[in] Command              The SMBUS command
  @param[in] Operation            Which SMBus protocol will be issued
  @param[in] PecCheck             If Packet Error Code Checking is to be used
  @param[in, out] Length          Length of data
  @param[in, out] Buffer          Data buffer

  @retval EFI_SUCCESS             The SMBUS operation is successful
  @retval Other Values            Something error occurred
**/
EFI_STATUS
EFIAPI
SmbusExecute (
  IN      CONST EFI_SMBUS_HC_PROTOCOL   *This,
  IN      EFI_SMBUS_DEVICE_ADDRESS      DeviceAddress,
  IN      EFI_SMBUS_DEVICE_COMMAND      Command,
  IN      EFI_SMBUS_OPERATION           Operation,
  IN      BOOLEAN                       PecCheck,
  IN OUT  UINTN                         *Length,
  IN OUT  VOID                          *Buffer
  )
{
  InitializeSmbusRegisters ();

  return SmbusExec (
           DeviceAddress,
           Command,
           Operation,
           PecCheck,
           Length,
           Buffer
           );
}

/**
  Initialize Smbus SMM service.

  @retval EFI_SUCCESS             Service initializes successfully
  @retval Other values            Some error occurred
**/
EFI_STATUS
EFIAPI
InitializeSmbusSmm (
  VOID
  )
{
  EFI_STATUS  Status;

  ///
  /// Locate SMM Base Protocol
  ///

  Status = gMmst->MmAllocatePool (EfiRuntimeServicesData, sizeof (SMBUS_INSTANCE), (VOID **) &mSmbusContext);

  if (EFI_ERROR (Status)) {
    return Status;
  } else {
    ZeroMem (mSmbusContext, sizeof (SMBUS_INSTANCE));

    mSmbusContext->Signature                  = PCH_SMBUS_PRIVATE_DATA_SIGNATURE;
    mSmbusContext->SmbusController.Execute    = SmbusExecute;
    mSmbusContext->SmbusController.ArpDevice  = SmbusArpDevice;
    mSmbusContext->SmbusController.GetArpMap  = SmbusGetArpMap;
    mSmbusContext->SmbusController.Notify     = SmbusNotify;

    ///
    /// Install the SMBUS interface
    ///
    Status = gMmst->MmInstallProtocolInterface (
                      &mSmbusContext->Handle,
                      &gEfiSmmSmbusProtocolGuid,
                      EFI_NATIVE_INTERFACE,
                      &mSmbusContext->SmbusController
                      );
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}

/**
  Uninstall Smbus SMM service.

  @retval EFI_SUCCESS             Service is uninstalled successfully.
  @retval Other Values            Something error occurred.

**/
EFI_STATUS
EFIAPI
UninstallSmbusSmm (
  VOID
  )
{
  EFI_STATUS  Status;

  if (mSmbusContext != NULL) {
    if (mSmbusContext->Handle != NULL) {
      //
      // Uninstall the SMBUS interface
      //
      Status = gMmst->MmUninstallProtocolInterface (
                        mSmbusContext->Handle,
                        &gEfiSmmSmbusProtocolGuid,
                        &mSmbusContext->SmbusController
                        );
      ASSERT_EFI_ERROR (Status);
    }
    gMmst->MmFreePool (mSmbusContext);
  }

  return EFI_SUCCESS;
}