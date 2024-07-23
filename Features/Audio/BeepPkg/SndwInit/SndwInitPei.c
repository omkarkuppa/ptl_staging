/** @file
  This PEIM driver configures Sndw controllers, performs codecs enumeration process,
  allows to send/receive messages to/from them.

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

@par specification reference:
**/

#include <Base.h>
#include <Uefi.h>
#include <Ppi/PciDevice.h>
#include <IndustryStandard/Pci22.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiDxeSndwInitLib.h>
#include <Library/PeiDxeAceCommonLib.h>
#include <Ppi/SndwAccessPpi.h>

EFI_STATUS
EFIAPI
HdaPciDevicePpiInstallationCallback (
  IN EFI_PEI_SERVICES            **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN VOID                        *Ppi
  );

EFI_PEI_NOTIFY_DESCRIPTOR  mPciDevicePpiNotify = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEdkiiPeiPciDevicePpiGuid,
  HdaPciDevicePpiInstallationCallback
};

/**
  Checks if a newly installed PPI points to a valid HDA device and installs a HDA_PPI for it.

  @param[in]  PeiServices        Pointer to PeiServices
  @param[in]  NotifyDescriptor   Pointer to notify descriptor
  @param[in]  Ppi                Installed PPI

  @return Status of the check
**/
EFI_STATUS
EFIAPI
HdaPciDevicePpiInstallationCallback (
  IN EFI_PEI_SERVICES            **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN VOID                        *Ppi
  )
{
  EFI_STATUS                     Status;
  SNDW_ACCESS_CONTEXT            *SndwAccessContext;
  EFI_PEI_PPI_DESCRIPTOR         *SndwAccessPpi;
  EDKII_PCI_DEVICE_PPI           *PciDevice;

  PciDevice = (EDKII_PCI_DEVICE_PPI*) Ppi;

  if (!IsHdaDeviceSupported (&PciDevice->PciIo)) {
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "%a () Start.\n", __FUNCTION__));

  SndwAccessContext = AllocateZeroPool (sizeof (SNDW_ACCESS_CONTEXT));
  if (SndwAccessContext == NULL) {
    DEBUG ((DEBUG_INFO, "%a () - End. Memory pool allocation error.\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  SndwAccessContext->Signature                = SNDW_ACCESS_SIGNATURE;
  SndwAccessContext->SndwAccess.Enable        = SndwAccessEnable;
  SndwAccessContext->SndwAccess.Disable       = SndwAccessDisable;
  SndwAccessContext->SndwAccess.Send          = SndwSend;
  SndwAccessContext->SndwAccess.Receive       = SndwReceive;
  SndwAccessContext->SndwAccess.SendWithAck   = SndwSendWithAck;
  SndwAccessContext->SndwAccess.GetFirstCodec = SndwGetFirstCodec;
  SndwAccessContext->SndwAccess.GetNextCodec  = SndwGetNextCodec;
  SndwAccessContext->CodecListHead            = NULL;
  SndwAccessContext->EnableCounter            = 0;
  SndwAccessContext->PciIo                    = &PciDevice->PciIo;

  //
  // Install on a new handle
  //
  SndwAccessPpi = (EFI_PEI_PPI_DESCRIPTOR*) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (SndwAccessPpi == NULL) {
    DEBUG ((DEBUG_INFO, "%a () - End. Memory pool allocation error.\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }
  SndwAccessPpi->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  SndwAccessPpi->Guid  = &gSndwAccessPpiGuid;
  SndwAccessPpi->Ppi   = &SndwAccessContext->SndwAccess;

  Status = PeiServicesInstallPpi (SndwAccessPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(): Failed to install PPI - %r\n", __FUNCTION__, Status));
    FreePool (SndwAccessContext);
    FreePool (SndwAccessPpi);
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));

  return Status;
}

/**
  This function is the entry point of this PEI Sndw Init Driver.

  @param  FileHandle             Handle of the file being invoked.
  @param  PeiServices            Describes the list of possible PEI Services.

  @retval EFI_SUCCESS            The driver is successfully initialized.
  @retval EFI_UNSUPPORTED        Not able to find HDA Device.
  @retval Others                 Can't initialize the driver.
**/
EFI_STATUS
EFIAPI
PeiSndwInitEntryPoint (
  IN EFI_PEI_FILE_HANDLE         FileHandle,
  IN CONST EFI_PEI_SERVICES      **PeiServices
  )
{
  DEBUG ((DEBUG_INFO, "%a - Start ()\n", __FUNCTION__));

  PeiServicesNotifyPpi (&mPciDevicePpiNotify);

  DEBUG ((DEBUG_INFO, "%a - End ()\n", __FUNCTION__));

  return EFI_SUCCESS;
}
