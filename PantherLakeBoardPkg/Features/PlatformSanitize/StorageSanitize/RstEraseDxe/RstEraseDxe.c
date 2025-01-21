/**@file
  RST Storage Erase Driver.

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

#include "RstEraseDxe.h"

VOID  *mNvmePassthruRegistration  = NULL;
VOID  *mSataPassthruRegistration  = NULL;

/**
  Check if the given device under VMD controlled

  @param[in] DevicePath  Device path.

  @retval TRUE   Device is under VMD controlled.
  @retval FALSE  Device is not under VMD controlled.

**/
BOOLEAN
IsVmdControlled (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *DevicePathNode;
  PCI_DEVICE_PATH           *PciDevicePath;

  DevicePathNode = DevicePath;
  while (!IsDevicePathEndType (DevicePathNode)) {
    if ((DevicePathNode->Type == HARDWARE_DEVICE_PATH) &&
        (DevicePathNode->SubType == HW_PCI_DP))
    {
      PciDevicePath = (PCI_DEVICE_PATH *)DevicePathNode;
      if ((PciDevicePath->Device == GetVmdDevNumber ()) &&
        (PciDevicePath->Function == GetVmdFuncNumber ())) {
        return TRUE;
      }
    }
    DevicePathNode = NextDevicePathNode (DevicePathNode);
  }

  return FALSE;
}

/**
  Callback function to start storage erase.

  @param[in] Event    Event whose notification function is being invoked
  @param[in] Context  Pointer to the notification function's context
**/
VOID
EFIAPI
NvmePassthruCallBack (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                          Status;
  UINTN                               BufferSize;
  EFI_HANDLE                          Handle;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL  *NvmePassthru;
  EFI_DEVICE_PATH_PROTOCOL            *DevicePath;
  RST_ERASE_NVME_PRIVATE              *RstNvmeErasePrivate;
  LIST_ENTRY              *Node;
  UINT32                  Count;
  RstNvmeErasePrivate = NULL;
  BufferSize = sizeof (EFI_HANDLE);
  Status = gBS->LocateHandle (
                  ByRegisterNotify,
                  NULL,
                  mNvmePassthruRegistration,
                  &BufferSize,
                  &Handle
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Get NvmePassThruProtocol interface
  //
  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiNvmExpressPassThruProtocolGuid,
                  (VOID **)&NvmePassthru
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Check if it is under VMD controlled
  //
  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **)&DevicePath
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  if (!IsVmdControlled (DevicePath)) {
    return;
  }

  //
  // Enumerate all the Nvme device under VMD controlled
  //
  RstNvmeErasePrivate = AllocateZeroPool (sizeof (RST_ERASE_NVME_PRIVATE));
  if (RstNvmeErasePrivate == NULL) {
    return;
  }
  RstNvmeErasePrivate->Signature = RST_ERASE_DRIVER_NVME_SIGNATURE;
  RstNvmeErasePrivate->NvmePassthru = NvmePassthru;
  EnumerateNvmeDevices (RstNvmeErasePrivate, Handle);

  //
  // Locate the device structure
  //
  for (Node = GetFirstNode (&RstNvmeErasePrivate->DeviceList), Count = 0;
       !IsNull (&RstNvmeErasePrivate->DeviceList, Node);
       Node = GetNextNode (&RstNvmeErasePrivate->DeviceList, Node)) {
    Count ++;
  }
  DEBUG ((DEBUG_INFO, "%a - Count = %d\n", __FUNCTION__, Count));

  //
  // Install RST erase protocols
  //
  RstNvmeErasePrivate->EraseProtocol.Revision               = RST_ERASE_PROTOCOL_REVISION;
  RstNvmeErasePrivate->EraseProtocol.EraseLengthGranularity = 1;
  RstNvmeErasePrivate->EraseProtocol.Erase                  = NvmeErase;
  RstNvmeErasePrivate->EraseProtocol.GetEraseResult         = GetNvmeEraseResult;
  RstNvmeErasePrivate->EraseProtocol.GetDeviceList          = GetNvmeDeviceList;
  RstNvmeErasePrivate->EraseProtocol.GetEstimateTime        = GetNvmeEstimateTime;

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gRstNvmeEraseProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &RstNvmeErasePrivate->EraseProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a: Failed to install the RST erase Protocol! Status = %r\n", __FUNCTION__, Status));
  }

  gBS->CloseEvent (Event);
}

/**
  Callback function to start storage erase.

  @param[in] Event    Event whose notification function is being invoked
  @param[in] Context  Pointer to the notification function's context
**/
VOID
EFIAPI
SataPassthruCallBack (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                          Status;
  EFI_ATA_PASS_THRU_PROTOCOL          *AtaPassthru;
  UINTN                               BufferSize;
  EFI_HANDLE                          Handle;
  EFI_DEVICE_PATH_PROTOCOL            *DevicePath;
  RST_ERASE_SATA_PRIVATE              *RstSataErasePrivate;
  RstSataErasePrivate = NULL;

  BufferSize = sizeof (EFI_HANDLE);
  Status = gBS->LocateHandle (
                  ByRegisterNotify,
                  NULL,
                  mSataPassthruRegistration,
                  &BufferSize,
                  &Handle
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Get NvmePassThruProtocol interface
  //
  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiAtaPassThruProtocolGuid,
                  (VOID **)&AtaPassthru
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Check if it is under VMD controlled
  //
  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **)&DevicePath
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  if (!IsVmdControlled (DevicePath)) {
    return;
  }

  //
  // Enumerate all the SATA device under VMD controlled
  //
  RstSataErasePrivate = AllocateZeroPool (sizeof (RST_ERASE_SATA_PRIVATE));
  if (RstSataErasePrivate == NULL) {
    return;
  }
  RstSataErasePrivate->Signature = RST_ERASE_DRIVER_SATA_SIGNATURE;
  RstSataErasePrivate->AtaPassthru = AtaPassthru;
  EnumerateSataDevices (RstSataErasePrivate, Handle);

  //
  // Install RST erase protocols
  //
  RstSataErasePrivate->EraseProtocol.Revision               = RST_ERASE_PROTOCOL_REVISION;
  RstSataErasePrivate->EraseProtocol.EraseLengthGranularity = 1;
  RstSataErasePrivate->EraseProtocol.Erase                  = SataErase;
  RstSataErasePrivate->EraseProtocol.GetEraseResult         = GetSataEraseResult;
  RstSataErasePrivate->EraseProtocol.GetDeviceList          = GetSataDeviceList;
  RstSataErasePrivate->EraseProtocol.GetEstimateTime        = GetSataEstimateTime;

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gRstSataEraseProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &RstSataErasePrivate->EraseProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a: Failed to install the RST erase Protocol! Status = %r\n", __FUNCTION__, Status));
  }
  gBS->CloseEvent (Event);
}

/**
  Main entry for this driver.

  @param[in] ImageHandle  Image Handle this driver.
  @param[in] SystemTable  Pointer to SystemTable.

  @retval EFI_SUCCESS  This function always complete successfully.
**/
EFI_STATUS
EFIAPI
RstStorageEraseEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE*  SystemTable
  )
{
  //
  // register callback function will be triggered by AtaPassthru protocol
  //
  EfiCreateProtocolNotifyEvent (
    &gEfiAtaPassThruProtocolGuid,
    TPL_CALLBACK,
    SataPassthruCallBack,
    NULL,
    &mSataPassthruRegistration
    );


  EfiCreateProtocolNotifyEvent (
    &gEfiNvmExpressPassThruProtocolGuid,
    TPL_CALLBACK,
    NvmePassthruCallBack,
    NULL,
    &mNvmePassthruRegistration
    );

  return EFI_SUCCESS;
}
