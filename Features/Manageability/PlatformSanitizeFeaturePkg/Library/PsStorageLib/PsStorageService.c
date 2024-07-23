/**@file
  Platform Erase Library.

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

#include <IndustryStandard/Pci.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PsStorageService.h>
#include <Protocol/PciIo.h>
#include <Protocol/PlatformSpecificResetHandler.h>
#include <Protocol/StorageSanitizeNotifyProtocol.h>
#include <Guid/PlatformSanitizeVariable.h>
#include <Library/SanitizeUiLib.h>

#define PCI_CLASS_MASS_STORAGE_NVM     0x08  // mass storage sub-class non-volatile memory.
#define PCI_IF_NVMHCI                  0x02  // mass storage programming interface NVMHCI.

GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN mResetSystemHook;

/**
  Get Devices from STORAGE_ERASE_DEVICE_VARIABLE and build device list for erase.

  @param[in] Variable           The pointer to the STORAGE_ERASE_DEVICE_VARIABLE which contain the device not selected to erase.
  @param[in] DeviceList         The old device list.

  @retval ERASE_DEVICE_LIST     Return the new device list or NULL.
**/
ERASE_DEVICE_LIST *
BuildDeviceItemFromVariable (
  IN STORAGE_ERASE_DEVICE_VARIABLE  *Variable,
  IN ERASE_DEVICE_LIST              *DeviceList
  )
{
  ERASE_DEVICE_LIST              *TempDeviceList;
  ERASE_DEVICE_INFO              *DeviceInfo;
  UINT32                         TotalSize;
  UINT32                         DeviceSize;

  if ((Variable == NULL) || (DeviceList == NULL)) {
    return NULL;
  }
  TotalSize = DeviceList->TotalSize;
  DeviceSize = (UINT32)(sizeof (ERASE_DEVICE_INFO));
  TempDeviceList = AllocateZeroPool (TotalSize + DeviceSize);
  if (TempDeviceList == NULL) {
    return NULL;
  }
  CopyMem (TempDeviceList, DeviceList, TotalSize);
  FreePool (DeviceList);
  DeviceList = NULL;

  DeviceInfo = (ERASE_DEVICE_INFO *)((UINTN)TempDeviceList + TotalSize);
  DeviceInfo->NameSize = Variable->NameLength;
  DeviceInfo->SnSize = Variable->SnLength;
  StrCpyS (DeviceInfo->Name, MN_MAX_LEN, Variable->Name);
  CopyMem (DeviceInfo->Sn, Variable->Sn, SN_MAX_LEN);
  TempDeviceList->TotalSize = TotalSize + DeviceSize;
  TempDeviceList->Selected = 0;
  return TempDeviceList;
}

/**
  Insert the password to the device list for erasing.

  @param[in] DeviceList         The old device list.
  @param[in] Password           The password should insert to the device list.

  @retval ERASE_DEVICE_LIST     Return the new device list or NULL.
**/
ERASE_DEVICE_LIST *
BuildDeviceItemWithPassword (
  IN ERASE_DEVICE_LIST  *DeviceList,
  IN CHAR8              *Password
  )
{
  ERASE_DEVICE_LIST              *TempDeviceList;
  ERASE_DEVICE_INFO              *DeviceInfo;
  UINT32                         TotalSize;
  UINT32                         DeviceSize;

  if ((Password != NULL) && (AsciiStrLen (Password) != 0)) {
    DEBUG ((DEBUG_INFO, "%a: Set Storage Erase password .\n", __FUNCTION__));
    DeviceSize = (UINT32)(sizeof (ERASE_DEVICE_INFO));
    if (DeviceList != NULL) {
      TotalSize = DeviceList->TotalSize;
    } else {
      TotalSize = sizeof (ERASE_DEVICE_LIST);
    }
    TempDeviceList = AllocateZeroPool (TotalSize + DeviceSize);
    if (TempDeviceList != NULL) {
      if (DeviceList != NULL) {
        CopyMem (TempDeviceList, DeviceList, TotalSize);
        FreePool (DeviceList);
        DeviceList = NULL;
      }
      DeviceInfo = (ERASE_DEVICE_INFO *)((UINTN)TempDeviceList + TotalSize);
      CopyMem (DeviceInfo->Password, Password, PASSWORD_MAX_LENGTH);
      TempDeviceList->TotalSize = TotalSize + DeviceSize;
      TempDeviceList->Selected = 0;
      return TempDeviceList;
    }
  }
  return NULL;
}

/**
  Build the device list from variable for storage erase.

  @param[in] Password        The password should insert to the device list.

  @retval ERASE_DEVICE_LIST     Return the device list or NULL.
**/
ERASE_DEVICE_LIST *
EFIAPI
BuildEraseDeviceList (
  IN CHAR8   *Password
  )
{
  STORAGE_ERASE_DEVICE_VARIABLE  *TempVariable;
  STORAGE_ERASE_DEVICE_VARIABLE  *Variable;
  UINTN                          VariableSize;
  EFI_STATUS                     Status;
  ERASE_DEVICE_LIST              *DeviceList;
  ERASE_DEVICE_LIST              *TempDeviceList;

  Variable = NULL;
  VariableSize = 0;
  DeviceList = NULL;
  TempDeviceList = NULL;

  Status = GetVariable2 (
             DEVICE_VARIABLE_NAME,
             &gPsPlatformEraseVariableGuid,
             (VOID **)&Variable,
             &VariableSize
             );
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  if ((VariableSize >= sizeof (STORAGE_ERASE_DEVICE_VARIABLE)) && (Variable != NULL)) {
    DeviceList = AllocateZeroPool (sizeof (ERASE_DEVICE_LIST));
    if (DeviceList == NULL) {
      FreePool (Variable);
      return NULL;
    }
    DeviceList->TotalSize = sizeof (ERASE_DEVICE_LIST);
    TempVariable = Variable;
    while ((TempVariable != NULL) &&
          (VariableSize >= sizeof (STORAGE_ERASE_DEVICE_VARIABLE))) {
      TempDeviceList = BuildDeviceItemFromVariable (TempVariable, DeviceList);
      if (TempDeviceList == NULL) {
        break;
      }
      DeviceList = TempDeviceList;
      VariableSize -= sizeof (STORAGE_ERASE_DEVICE_VARIABLE);
      TempVariable ++;
    }
    FreePool (Variable);
  }
  if ((Password != NULL) && AsciiStrLen (Password) != 0) {
    TempDeviceList = BuildDeviceItemWithPassword (DeviceList, Password);
    if (TempDeviceList != NULL) {
      DeviceList = TempDeviceList;
    }
  }
  return DeviceList;
}

/**
  Due to platform security concern, only trusted storages are connected.
  This function is to connect all storages so they can be found and erased.
**/
VOID
ConnectAllStorages (
  VOID
  )
{
  EFI_STATUS                    Status;
  UINTN                         Index;
  UINTN                         PciIoHandleCount;
  EFI_HANDLE                    *PciIoHandleBuffer;
  EFI_PCI_IO_PROTOCOL           *PciIo;
  UINT8                         ClassCode[3];

  DEBUG ((DEBUG_INFO, "Platform Erase :: %a\n", __FUNCTION__));

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &PciIoHandleCount,
                  &PciIoHandleBuffer
                  );
  if (EFI_ERROR (Status) || (PciIoHandleBuffer == NULL) || (PciIoHandleCount == 0)) {
    return;
  }

  for (Index = 0; Index < PciIoHandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    PciIoHandleBuffer[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID **) &PciIo
                    );
    ASSERT_EFI_ERROR (Status);

    Status = PciIo->Pci.Read (
                          PciIo,
                          EfiPciIoWidthUint8,
                          PCI_CLASSCODE_OFFSET,
                          sizeof (ClassCode),
                          ClassCode
                          );
    ASSERT_EFI_ERROR (Status);
    //
    // Examine Nvm Express controller PCI Configuration table fields
    //
    if ((ClassCode[0] == PCI_IF_NVMHCI) &&
        (ClassCode[1] == PCI_CLASS_MASS_STORAGE_NVM) &&
        (ClassCode[2] == PCI_CLASS_MASS_STORAGE)) {
      gBS->ConnectController (PciIoHandleBuffer[Index], NULL, NULL, TRUE);
    }

    //
    // Connect SATA controller
    //
    if (ClassCode[2] == PCI_CLASS_MASS_STORAGE &&
        (ClassCode[1] == PCI_CLASS_MASS_STORAGE_SATADPA || ClassCode[1] == PCI_CLASS_MASS_STORAGE_RAID)) {
      gBS->ConnectController (PciIoHandleBuffer[Index], NULL, NULL, TRUE);
    }

    //
    // Connect UFS controller
    //
    if ((ClassCode[2] == PCI_CLASS_MASS_STORAGE) &&
        (ClassCode[1] == 0x09)) {
      gBS->ConnectController (PciIoHandleBuffer[Index], NULL, NULL, TRUE);
    }
  }
  gBS->FreePool (PciIoHandleBuffer);
}


/**
  Hook system reset to display requesting G3 cycle message.

  When PcdSkipHddPasswordPrompt / PcdSkipOpalPasswordPrompt are set and SSD is in the unlocked status,
  system is forced to shutdown due to security concerns without any message.
  This function is to display message to request user to perform G3 cycle.

  @param[in]  ResetType         The type of reset to perform.
  @param[in]  ResetStatus       The status code for the reset.
  @param[in]  DataSize          The size, in bytes, of ResetData.
  @param[in]  ResetData         For a ResetType of EfiResetCold, EfiResetWarm, or
                                EfiResetShutdown the data buffer starts with a Null-terminated
                                string, optionally followed by additional binary data.
                                The string is a description that the caller may use to further
                                indicate the reason for the system reset.
                                For a ResetType of EfiResetPlatformSpecific the data buffer
                                also starts with a Null-terminated string that is followed
                                by an EFI_GUID that describes the specific type of reset to perform.
**/
VOID
EFIAPI
PlatformEraseResetSystem (
  IN EFI_RESET_TYPE           ResetType,
  IN EFI_STATUS               ResetStatus,
  IN UINTN                    DataSize,
  IN VOID                     *ResetData OPTIONAL
  )
{
  DEBUG ((DEBUG_INFO, "Platform Erase :: %a()\n", __FUNCTION__));

  if (!mResetSystemHook) {
    return;
  }

  InitUiLib ();
  ClearScreen ();
  DEBUG ((DEBUG_INFO, "Platform Erase :: Request user to perform G3 cycle \n"));
  OutputStringAtNext (L"!!G3 cycle is required to restore storage device security state.", EFI_BACKGROUND_BLACK | EFI_WHITE);
  CpuDeadLoop ();
  return;
}


/**
  Hook the system reset to properly handle SSD security state.

  @param[in]  Event     Event whose notification function is being invoked
  @param[in]  Context   Pointer to the notification function's context
**/
VOID
EFIAPI
PlatformEraseOnResetHook (
  IN EFI_EVENT                      Event,
  IN VOID                           *Context
  )
{
  EFI_STATUS                                     Status;
  EDKII_PLATFORM_SPECIFIC_RESET_HANDLER_PROTOCOL *ResetHandler;

  Status = gBS->LocateProtocol (
                  &gEdkiiPlatformSpecificResetHandlerProtocolGuid,
                  NULL,
                  (VOID **) &ResetHandler
                  );

  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Platform Erase :: %a()\n", __FUNCTION__));
    ResetHandler->RegisterResetNotify (ResetHandler, PlatformEraseResetSystem);
    gBS->CloseEvent (Event);
  }
}

/**
  Attempts to erase storage devices.

  @param[in]   EraseConfiguration  Configuration for erase operation
  @param[in]   DeviceList          The device list for erasing
  @param[in]   CompleteFunction    Callback function when erase completed.

  @retval      EFI_SUCCESS         Erase device succeed
  @retval      EFI_UNSUPPORTED     The device is not supported
  @retval      EFI_ACCESS_DENIED   User has entered wrong password too many times
  @retval      EFI_ABORTED         The device is supported, but the system
                                   has failed to erase it
**/
EFI_STATUS
PerformSsdErase (
  IN ERASE_CONFIGURATION    EraseConfiguration,
  IN ERASE_DEVICE_LIST      *DeviceList,
  IN STORAGE_ERASE_COMPLETE CompleteFunction
  )
{
  EFI_STATUS                     Status;
  EFI_TPL                        OldTpl;
  VOID                           *Registration;
  STORAGE_ERASE_NOTIFY_PROTOCOL  *StorageEraseNotify;

  DEBUG ((DEBUG_INFO, "Platform Erase :: %a\n", __FUNCTION__));

  OldTpl = EfiGetCurrentTpl ();
  gBS->RestoreTPL (TPL_APPLICATION);

  //
  // Power off SSD is required to restore its security state.
  // There are two scenarios we need to power off SSD:
  // 1. When PcdSkipHddPasswordPrompt / PcdSkipOpalPasswordPrompt are set and SSD is in the unlocked status,
  //    system is forced shutdown for security concern.
  // 2. When ATA SSD's SecurityStatus.Frozen is set.
  //
  // Due to board design, SSD may not power off during system reset/shutdown,
  // So we register a reset hook to use GPIO power pins to power off SSD and perform warm reset.
  // If these power pins are not assigned, G3 cycle is still mandatory.
  //
  EfiCreateProtocolNotifyEvent (
    &gEdkiiPlatformSpecificResetHandlerProtocolGuid,
    TPL_CALLBACK,
    PlatformEraseOnResetHook,
    NULL,
    &Registration
    );
  //
  // When connecting storages, system might be forced shutdown for above scenario #1 without any message.
  // Set mResetSystemHook TRUE as a indicator that we should perform the hook in PlatformEraseResetSystem (),
  // and set it FALSE after storage connection done to prevent any unexpected message displayed when reset system.
  //
  mResetSystemHook = TRUE;
  ConnectAllStorages ();
  mResetSystemHook = FALSE;

  //
  // Install platform erase notify protocol
  //
  Status = gBS->LocateProtocol (
                  &gStorageSanitizeNotifyProtocolGuid,
                  NULL,
                  (VOID **) &StorageEraseNotify
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "locate storage erase notify protocol failed(%r).\n", Status));
    return Status;
  }
  Status = StorageEraseNotify->StorageErase (
                                 EraseConfiguration,
                                 DeviceList,
                                 (STORAGE_ERASE_COMPLETE) CompleteFunction
                                 );

  gBS->RaiseTPL (OldTpl);
  return EFI_SUCCESS;
}