/**@file
  Storage erase driver

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

#include "StorageErase.h"

STORAGE_ERASE_DRIVER    mStorageEraseDriver;
BOOLEAN                 mRegisterRstDevice = FALSE;
STORAGE_ERASE_COMPLETE  mCompleteFunction = NULL;
UINT8 mSanitizeOrder[SANITIZE_COUNT] = {
  SANITIZE_BLOCK_ERASE,
  SANITIZE_CRYPTO_ERASE,
  SANITIZE_OVR_ERASE,
  SANITIZE_CLEAR
};

//
// Globals
//
EFI_DRIVER_BINDING_PROTOCOL gStorageEraseDriverBinding = {
  StorageEraseBindingSupported,
  StorageEraseBindingStart,
  StorageEraseBindingStop,
  0x1b,
  NULL,
  NULL
};

/**
  Enumerate the NVMe / SATA device from RST, add the device to the list.

  @param[in] DeviceType  Device type, NVMe or SATA
**/
VOID
RegisterRstDevice (
  IN DEVICE_TYPE  DeviceType
  )
{
  EFI_STATUS  Status;
  UINT32      DeviceNumber;
  VOID        *DeviceList;
  UINT32      Index;
  EFI_HANDLE  Handle;
  EFI_HANDLE  ParentHandle;
  UINT32      CommandId;
  PLATFORM_ERASE_DEVICE  *Dev;

  Status = HandleRstDevice (DeviceType, &DeviceNumber, &DeviceList);
  if (!EFI_ERROR (Status) && (DeviceList != NULL)) {
    for (Index = 0; Index < DeviceNumber; Index++) {
      Status = GetRstDevice (DeviceList, Index, &Handle, &ParentHandle, &CommandId);
      if (EFI_ERROR (Status)) {
        continue;
      }
      Dev = AllocateZeroPool (sizeof (PLATFORM_ERASE_DEVICE));
      if (Dev == NULL) {
        continue;
      }
      Dev->Signature = PLATFORM_ERASE_DEVICE_SIGNATURE;
      Dev->Handle = Handle;
      Dev->CmdId.Value = CommandId;
      Dev->VmdControlled |= DEVICE_IN_VMD_CONTROLLED;
      Dev->DeviceType = DeviceType;
      Dev->Selected = 1;
      Dev->ParentHandle = ParentHandle;

      //
      // Get device information (Name, SN)
      //
      GetRstEraseInfo (Dev->CmdId, Dev->DeviceType, &Dev->EraseInfo);

      //
      // Add device to the Platform Erase device list
      //
      InsertTailList (&mStorageEraseDriver.PlatformEraseDeviceList, &Dev->Link);
      DEBUG ((DEBUG_INFO, "AddRstDeviceToPlatformEraseList - Name: %a\n", Dev->EraseInfo.ModelName));
      DEBUG ((DEBUG_INFO, "                                  type: %d, Selected: %d, IsVmdDevice: %d\n",
                                                             Dev->DeviceType, Dev->Selected, Dev->VmdControlled));
    }
    if (DeviceList != NULL) {
      FreePool (DeviceList);
    }
  }
}

/**
  Init UI title and clear screen

**/
VOID
InitSanitizeUi (
  VOID
  )
{
  InitUiLib ();
  SetScreenTitle ();
  ClearScreen ();
}

/**
  Storage is frozen, reset system to power cycle storage.

  @retval EFI_SUCCESS  system reset.
  @retval Others       system not reset.
**/
EFI_STATUS
ResetSystemForStoragePowerCycle (
  VOID
  )
{
  LIST_ENTRY             *Node;
  PLATFORM_ERASE_DEVICE  *TmpDev;
  BOOLEAN                G3Status;

  G3Status = FALSE;
  for (Node = GetFirstNode (&mStorageEraseDriver.PlatformEraseDeviceList);
       !IsNull (&mStorageEraseDriver.PlatformEraseDeviceList, Node);
       Node = GetNextNode (&mStorageEraseDriver.PlatformEraseDeviceList, Node)) {

    TmpDev = PLATFORM_ERASE_DEVICE_FROM_LINK (Node);
    if (!TmpDev->Selected) {
      continue;
    }
    G3Status = CheckG3Status (TmpDev);
    if (G3Status) {
      DEBUG ((DEBUG_INFO, "storage %a need power reset.\n", TmpDev->EraseInfo.ModelName));
      OutputStringAtNext (L"!!G3 cycle is required to restore storage device security state.", EFI_BACKGROUND_BLACK | EFI_WHITE);
      gRT->ResetSystem (EfiResetShutdown, EFI_SUCCESS, 0, NULL);
      return EFI_SUCCESS;
    }
  }
  return EFI_UNSUPPORTED;
}

/**
  Backup the device which need erase and verify at last.
**/
VOID
BackupDeviceForVerify (
  VOID
  )
{
  LIST_ENTRY             *Node;
  PLATFORM_ERASE_DEVICE  *TmpDev;

  mStorageEraseDriver.NumOfVerifyPostpone = 0;
  ZeroMem(mStorageEraseDriver.VerifyInfo, MAX_DEVICE_SUPPORT * sizeof (VERIFY_INFO));

  for (Node = GetFirstNode (&mStorageEraseDriver.PlatformEraseDeviceList);
        !IsNull (&mStorageEraseDriver.PlatformEraseDeviceList, Node);
        Node = GetNextNode (&mStorageEraseDriver.PlatformEraseDeviceList, Node)) {

    TmpDev = PLATFORM_ERASE_DEVICE_FROM_LINK (Node);
    if (!TmpDev->Selected) {
      continue;
    }
    if (VerifySpecificDevice (TmpDev)) {
      mStorageEraseDriver.VerifyInfo[mStorageEraseDriver.NumOfVerifyPostpone].Dev = TmpDev;
      mStorageEraseDriver.NumOfVerifyPostpone++;
    }
  }
}
/**
  Check if there is output console connected.

  @retval TRUE/FALSE  output console connected/not connected.

**/
BOOLEAN
HasOutputConsoleConnected (
  VOID
  )
{
  EFI_GRAPHICS_OUTPUT_PROTOCOL          *GraphicsOutput;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL       *SimpleTextOut;
  EFI_STATUS                            Status;

  //
  // Get current video resolution and text mode
  //
  Status = gBS->HandleProtocol (
                  gST->ConsoleOutHandle,
                  &gEfiGraphicsOutputProtocolGuid,
                  (VOID**)&GraphicsOutput
                  );
  if (EFI_ERROR (Status)) {
    GraphicsOutput = NULL;
  }

  Status = gBS->HandleProtocol (
                  gST->ConsoleOutHandle,
                  &gEfiSimpleTextOutProtocolGuid,
                  (VOID**)&SimpleTextOut
                  );
  if (EFI_ERROR (Status)) {
    SimpleTextOut = NULL;
  }

  if ((GraphicsOutput == NULL) || (SimpleTextOut == NULL)) {
    return FALSE;
  }

  return TRUE;
}

/**
  Remove the Platform erase device from the internal list

  @param[in] Dev  Platform erase device

**/
VOID
RemoveDevice (
  IN PLATFORM_ERASE_DEVICE  *Dev
  )
{
  RemoveEntryList (&Dev->Link);

  FreePool (Dev);
}

/**
  Stop this Controller.

  @param[in] Handle  The device with given handle needed to be stopped.

  @retval EFI_SUCCESS  Successful.

**/
EFI_STATUS
DriverStopDevice (
  IN EFI_HANDLE  Handle
  )
{
  PLATFORM_ERASE_DEVICE  *Dev;
  LIST_ENTRY             *Node;

  //
  // Locate the platform erase device with given handle
  //
  for (Node = GetFirstNode (&mStorageEraseDriver.PlatformEraseDeviceList);
       !IsNull (&mStorageEraseDriver.PlatformEraseDeviceList, Node);
       Node = GetNextNode (&mStorageEraseDriver.PlatformEraseDeviceList, Node)) {
    Dev = PLATFORM_ERASE_DEVICE_FROM_LINK (Node);
    if (Dev->Handle == Handle) {
      RemoveDevice (Dev);
    }
  }

  return EFI_SUCCESS;
}

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
  Build the device list from variable for storage erase.

  @retval ERASE_DEVICE_LIST     Return the device list or NULL.
**/
ERASE_DEVICE_LIST *
EFIAPI
BuildEraseDeviceList (
  VOID
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
             &gPsStorageEraseVariableGuid,
             (VOID **)&Variable,
             &VariableSize
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PS: %a - Fail to get %S variable (%r)\n", __FUNCTION__, DEVICE_VARIABLE_NAME, Status));
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

  return DeviceList;
}

/**
  Get password from device list.

  @param[in] DeviceInfo  Pointer the ERASE_DEVICE_INFO list.
  @param[in] TotalSize   Total size of ERASE_DEVICE_INFO list.
  @param[out] Password   Password get from device list.

**/
VOID
GetPasswordFromDeviceList (
  IN  ERASE_DEVICE_INFO      *DeviceInfo,
  IN  UINT32                 TotalSize,
  OUT CHAR8                  Password[PASSWORD_MAX_LENGTH + 1]
  )
{
  UINT32                 TempSize;
  ERASE_DEVICE_INFO      *TempDevice;

  TempSize = TotalSize;
  TempDevice = DeviceInfo;
  ZeroMem(Password, PASSWORD_MAX_LENGTH + 1);
  if (DeviceInfo == NULL) {
    return;
  }

  while ((TempSize > 0) && (TempDevice != NULL)) {
    if (TempSize < sizeof (ERASE_DEVICE_INFO)) {
      break;
    }
    if ((TempDevice->SnSize == 0) && (AsciiStrLen (TempDevice->Password) != 0)) {
      CopyMem (Password, TempDevice->Password, PASSWORD_MAX_LENGTH);
      break;
    }
    TempSize -= sizeof (ERASE_DEVICE_INFO);
    TempDevice ++;
  }
}

/**
  Get the pointer to ERASE_DEVICE_INFO if device is in the list.

  @param[in] DestinationDevice  Destination device need update.
  @param[in] DeviceListInfo     Pointer to the ERASE_DEVICE_INFO list.
  @param[in] TotalSize          Total size of ERASE_DEVICE_INFO list.

  @retval ERASE_DEVICE_INFO     Device in the device list, return the pointer to the ERASE_DEVICE_INFO.
  @retval NULL                  Device is not in the device list.
**/
ERASE_DEVICE_INFO *
GetDeviceInfoFromList (
  IN OUT PLATFORM_ERASE_DEVICE  *DestinationDevice,
  IN ERASE_DEVICE_INFO          *DeviceListInfo,
  IN UINT32                     TotalSize
  )
{
  UINT32                 TempSize;
  ERASE_DEVICE_INFO      *TempDevice;

  TempSize = TotalSize;
  TempDevice = DeviceListInfo;
  if ((DestinationDevice == NULL) || (DeviceListInfo == NULL)) {
    return NULL;
  }

  while ((TempSize > 0) && (TempDevice != NULL)) {
    if (TempSize < sizeof (ERASE_DEVICE_INFO)) {
      break;
    }
    if ((TempDevice->SnSize > 0) && CompareMem (DestinationDevice->EraseInfo.SerialNumber, TempDevice->Sn, TempDevice->SnSize) == 0) {
      DEBUG ((DEBUG_INFO, "Sn(%a) is in the device list.\n", TempDevice->Sn));
      return TempDevice;
    }
    TempSize -= sizeof (ERASE_DEVICE_INFO);
    TempDevice ++;
  }
  return NULL;
}

/**
  Update the selected status and password for platform erase device.

  @param[in] HostPassword  HostPassword to erase storage.

**/
VOID
UpdateDeviceInfo (
  IN CHAR8  *HostPassword
  )
{
  ERASE_DEVICE_LIST      *DeviceList;
  LIST_ENTRY             *Node;
  PLATFORM_ERASE_DEVICE  *Dev;
  UINT32                 TotalSize;
  ERASE_DEVICE_INFO      *DeviceInfo;
  ERASE_DEVICE_INFO      *TempDevice;
  UINT8                  Selected;

  DEBUG ((DEBUG_INFO, "PS: %a - enter\n", __FUNCTION__));

  DeviceList = BuildEraseDeviceList (); // Build the device list from variable.
  if (DeviceList != NULL) {
    TotalSize = DeviceList->TotalSize;
    Selected = DeviceList->Selected;
    if (TotalSize > 0) {
      TotalSize -= sizeof (ERASE_DEVICE_LIST);
      DeviceInfo = (ERASE_DEVICE_INFO *)((UINTN)DeviceList + sizeof (ERASE_DEVICE_LIST));
      //
      // Update the selected status and password from variable.
      //
      for (Node = GetFirstNode (&mStorageEraseDriver.PlatformEraseDeviceList);
        !IsNull (&mStorageEraseDriver.PlatformEraseDeviceList, Node);
        Node = GetNextNode (&mStorageEraseDriver.PlatformEraseDeviceList, Node)) {
        Dev = PLATFORM_ERASE_DEVICE_FROM_LINK (Node);
        TempDevice = GetDeviceInfoFromList (Dev, DeviceInfo, TotalSize);
        if (TempDevice == NULL) { // De-select device is not in the erase device list
          if (Selected) {
            Dev->Selected = 0;
          } else {
            Dev->Selected = 1;
            if ((AsciiStrLen (HostPassword) != 0)) {
              CopyMem (Dev->Password, HostPassword, PASSWORD_MAX_LENGTH);
            }
          }
        } else {
          Dev->Selected = Selected;
          if (Selected && (AsciiStrLen (HostPassword) != 0)) {
            CopyMem (Dev->Password, HostPassword, PASSWORD_MAX_LENGTH);
          }
        }
      }
    }
  }

  //
  // Update Host password for selected devices.
  //
  if ((HostPassword != NULL) && (AsciiStrLen (HostPassword) != 0)) {
    for (Node = GetFirstNode (&mStorageEraseDriver.PlatformEraseDeviceList);
      !IsNull (&mStorageEraseDriver.PlatformEraseDeviceList, Node);
      Node = GetNextNode (&mStorageEraseDriver.PlatformEraseDeviceList, Node)) {
      Dev = PLATFORM_ERASE_DEVICE_FROM_LINK (Node);
      if (Dev->Selected) {
        CopyMem (Dev->Password, HostPassword, PASSWORD_MAX_LENGTH);
      }
    }
  }
}

/**
  Update the sanitize order from variable.

**/
VOID
UpdateSanitizeOrder (
  VOID
  )
{
  EFI_STATUS                     Status;
  PLATFORM_ERASE_VARIABLE        *Variable;
  UINTN                          VariableSize;
  UINT32                         VariableAttr;

  VariableSize   = 0;
  Variable       = NULL;
  VariableAttr   = 0;

  Status = GetVariable3 (
             PLATFORM_ERASE_VARIABLE_NAME,
             &gPsPlatformEraseVariableGuid,
             (VOID **) &Variable,
             &VariableSize,
             &VariableAttr
             );
  if (EFI_ERROR (Status) || Variable == NULL) {
    return;
  }

  DEBUG ((DEBUG_INFO, "Update order from variable : %S \n", PLATFORM_ERASE_VARIABLE_NAME));
  CopyMem (&mSanitizeOrder, &Variable->SanitizeOrder, sizeof (Variable->SanitizeOrder));
}

/**
  Get the device information from selected platform device and add the information to the device list.

  @param[in] Device          platform device.
  @param[in] OldDeviceList   old device list.

  @retval ERASE_DEVICE_LIST  Successfully build new item to the device list, return the pointer.
  @retval NULL               No change for the old Device list.
**/
ERASE_DEVICE_LIST *
BuildListFromSelectedDevice (
  IN PLATFORM_ERASE_DEVICE *Device,
  IN ERASE_DEVICE_LIST     *OldDeviceList
 )
{
  ERASE_DEVICE_LIST     *TempDeviceList;
  ERASE_DEVICE_INFO     *DeviceInfo;
  UINT32                TotalSize;
  UINT32                DeviceSize;

  if ((Device == NULL) || (Device->Selected == 0) || (OldDeviceList == NULL)) {
    return NULL;
  }

  TotalSize = OldDeviceList->TotalSize;
  DeviceSize = (UINT32)(sizeof (ERASE_DEVICE_INFO));
  TempDeviceList = AllocateZeroPool (TotalSize + DeviceSize);
  if (TempDeviceList == NULL) {
    return NULL;
  }
  CopyMem (TempDeviceList, OldDeviceList, TotalSize);
  FreePool (OldDeviceList);
  OldDeviceList = NULL;

  DeviceInfo = (ERASE_DEVICE_INFO *)((UINTN)TempDeviceList + TotalSize);
  DeviceInfo->NameSize = (UINT32)(AsciiStrLen (Device->EraseInfo.ModelName)) * sizeof(CHAR16);
  DeviceInfo->SnSize = (UINT32)AsciiStrLen (Device->EraseInfo.SerialNumber);
  UnicodeSPrintAsciiFormat(DeviceInfo->Name, MN_MAX_LEN * sizeof (CHAR16), "%a", Device->EraseInfo.ModelName);
  CopyMem (DeviceInfo->Sn, Device->EraseInfo.SerialNumber, SN_MAX_LEN);
  DeviceInfo->Result = Device->Result;
  DEBUG ((DEBUG_INFO, "%a : enter Name(%d): %S, Sn(%d): %a\n", __FUNCTION__, DeviceInfo->NameSize, DeviceInfo->Name, DeviceInfo->SnSize, DeviceInfo->Sn));
  TempDeviceList->TotalSize = TotalSize + DeviceSize;
  return TempDeviceList;
}

/**
  Get information for all selected devices.

  @param[in] DeviceList  Output the pointer to the ERASE_DEVICE_LIST.

  @retval EFI_SUCCESS            Successfully get all selected devices.
  @retval EFI_NOT_FOUND          No device found or selected.
  @retval EFI_OUT_OF_RESOURCES   Memory is not enough.
**/
EFI_STATUS
EFIAPI
GetDeviceList (
  OUT VOID     **DeviceList
  )
{
  LIST_ENTRY            *Node;
  UINTN                 Count;
  PLATFORM_ERASE_DEVICE *Dev;
  ERASE_DEVICE_LIST     *TempDeviceList;
  ERASE_DEVICE_LIST     *OldDeviceList;

  if (DeviceList == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Count         = 0;
  *DeviceList   = NULL;
  OldDeviceList = AllocateZeroPool (sizeof (ERASE_DEVICE_LIST));
  if (OldDeviceList == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  OldDeviceList->TotalSize = sizeof (ERASE_DEVICE_LIST);

  for (Node = GetFirstNode (&mStorageEraseDriver.PlatformEraseDeviceList);
       !IsNull (&mStorageEraseDriver.PlatformEraseDeviceList, Node);
       Node = GetNextNode (&mStorageEraseDriver.PlatformEraseDeviceList, Node)) {
    Dev = PLATFORM_ERASE_DEVICE_FROM_LINK (Node);
    TempDeviceList = BuildListFromSelectedDevice (Dev, OldDeviceList);
    if (TempDeviceList != NULL) {
      OldDeviceList = TempDeviceList;
      Count ++;
    }
  }

  if (Count > 0) {
    *DeviceList = OldDeviceList;
  } else {
    if (OldDeviceList != NULL) {
      FreePool (OldDeviceList);
    }
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}

/**
  Get all devices erase result.

  @retval EFI_SUCCESS       All selected devices are erased successfully.
  @retval EFI_NOT_FOUND     No device found or selected.
  @retval EFI_DEVICE_ERROR  Some of the selected devices are failed to erase.
**/
EFI_STATUS
StorageEraseResult (
  VOID
  )
{
  UINT8                  Result;
  LIST_ENTRY             *Node;
  PLATFORM_ERASE_DEVICE  *Dev;

  Result = 0;

  for (Node = GetFirstNode (&mStorageEraseDriver.PlatformEraseDeviceList);
    !IsNull (&mStorageEraseDriver.PlatformEraseDeviceList, Node);
    Node = GetNextNode (&mStorageEraseDriver.PlatformEraseDeviceList, Node)) {

    Dev = PLATFORM_ERASE_DEVICE_FROM_LINK (Node);
    Result |= Dev->Result;
  }

  if (Result == ERASE_RESULT_SUCCESS) {
    return EFI_SUCCESS;
  } else if (Result == 0) {
    return EFI_NOT_FOUND;
  }
  return EFI_DEVICE_ERROR;
}

/**
  Send erase result to USB or callback functionality.

  @retval EFI_SUCCESS       All selected devices are erased successfully.
  @retval Other             Failure.
**/
EFI_STATUS
SendEraseResult (
  IN  STORAGE_ERASE_COMPLETE CompleteFunction,
  IN  BOOLEAN                PerformWriteToUsb
  )
{
  EFI_STATUS           Status;
  EFI_STATUS           Result;
  ERASE_DOCUMENTATION  *ReportList;
  ERASE_DEVICE_LIST    *DeviceList;

  DeviceList = NULL;
  ReportList = NULL;
  Result = StorageEraseResult ();
  //
  // Documentation
  //
  Status = GetStorageEraseReport ();
  if (!EFI_ERROR (Status)) {
    ReportList = (ERASE_DOCUMENTATION *)AllocateZeroPool (sizeof (ERASE_DOCUMENTATION) * mStorageEraseDriver.NumOfDocumentation);
    if (ReportList != NULL) {
      CopyMem (ReportList, mStorageEraseDriver.EraseDocumentation, sizeof (ERASE_DOCUMENTATION) * mStorageEraseDriver.NumOfDocumentation);
    }
    if (PerformWriteToUsb) {
      WriteToUsb ();
    }
  }
  if (CompleteFunction != NULL) {
    GetDeviceList ((void **) &DeviceList);
    CompleteFunction (Result, mStorageEraseDriver.NumOfDocumentation, ReportList, DeviceList);
    CompleteFunction = NULL;
  }
  return Result;
}
/**
  Erase the specific device and verify it.

  @param[in] HasOutputConsole The output console connected or not.

  @retval EFI_SUCCESS     Storage erase is successful.
  @retval Others          failed to erase all devices.
**/
EFI_STATUS
EraseAndVerifySpecificDevice (
  IN BOOLEAN                HasOutputConsole
  )
{
  EFI_STATUS             Status;
  EFI_STATUS             Result;
  UINT8                  VerifyNumber;
  VERIFY_INFO            *VerifyInfo;
  PLATFORM_ERASE_DEVICE  *TmpDev;
  BOOLEAN                PostponeErase;
  UINT64                 Time;

  Result = EFI_SUCCESS;
  TmpDev = NULL;
  PostponeErase = FALSE;

  VerifyNumber = mStorageEraseDriver.NumOfVerifyPostpone;
  while (VerifyNumber > 0) {
    VerifyInfo = &mStorageEraseDriver.VerifyInfo[VerifyNumber-1];
    if (VerifyInfo->Dev != NULL) {
      TmpDev = VerifyInfo->Dev;
      TmpDev->Erased = TRUE;

      //
      // show message on the screen
      //
      if (HasOutputConsole) {
        PrintEraseMessage (TmpDev->EraseInfo.ModelName, TmpDev->DeviceType, ERASE_MESSAGE);
      }

      //
      // Unlock the device when the device is locked.
      //
      Status = PerformUnlockStorage (TmpDev, TmpDev->Password, HasOutputConsole);
      if (!EFI_ERROR (Status)) {
        //
        // Backup Lba if it is not clear device
        //
        if (mSanitizeOrder[0] != SANITIZE_CLEAR) {
          if (VerifyInfo->BackupLba == NULL) {
            if (HasOutputConsole) {
              Time = GetVerifyTime (TmpDev);
              AppendEraseTime (Time);
            }
            BackupLba (VerifyInfo);
          }
        }

        //
        // Erase device
        //
        Status = PerformDeviceErase (TmpDev, TmpDev->Password, HasOutputConsole);
        Result |= Status;
        if (HasOutputConsole && (TmpDev->Result == ERASE_RESULT_SUCCESS)) {
          UpdateEraseMessage (TmpDev->EraseInfo.ModelName, TmpDev->DeviceType, VERIFICATION_MESSAGE);
          Time = GetVerifyTime (TmpDev);
          AppendEraseTime (Time);
        }
        //
        // Verify the data
        // if it is clear, check if it is clear to zero
        // if it is purge, compare with the backup lba
        //
        if (mSanitizeOrder[0] != SANITIZE_CLEAR) {
          VerifyBackupLba (VerifyInfo);
        } else {
          VerifyEraseResult (TmpDev);
        }
        mStorageEraseDriver.VerifyInfo[VerifyNumber-1].Dev = NULL;
      } else if (Status == EFI_NOT_READY) {
        PostponeErase = TRUE;
      } else {
        Result |= Status;
        mStorageEraseDriver.VerifyInfo[VerifyNumber-1].Dev = NULL;
      }

      //
      // Show erase result on the screen
      //
      if (HasOutputConsole && TmpDev->Erased) {
        UpdateEraseMessage (TmpDev->EraseInfo.ModelName, TmpDev->DeviceType, ResultString (TmpDev->Result, TRUE));
      }
    }
    VerifyNumber--;
  }

  if (PostponeErase) {
    return EFI_NOT_READY;
  }
  return Result;
}

/**
  Function to start storage erase.

  @param[in] PerformVerify    Enable/Disable verification flow.
  @param[in] HasOutputConsole The output console connected or not.

  @retval EFI_SUCCESS     Storage erase is successful.
  @retval Others          failed to erase all devices.
**/
EFI_STATUS
PerformStorageErase (
  IN BOOLEAN  PerformVerify,
  IN BOOLEAN  HasOutputConsole
  )
{
  EFI_STATUS             Status;
  LIST_ENTRY             *Node;
  PLATFORM_ERASE_DEVICE  *TmpDev;
  EFI_STATUS             Result;
  BOOLEAN                PostponeErase;
  UINT64                 Time;

  DEBUG ((DEBUG_INFO, "PS: %a - enter\n", __FUNCTION__));

  PostponeErase = FALSE;
  Result = EFI_SUCCESS;

  //
  // Traverse all the device in the PlatformEraseDeviceList
  // 1. check if it support security feature
  //   a. if yes, perform unlock flow and disable security feature process
  // 2. check if verification is supported
  //   a. if yes, perform pre-sample flow
  // 3. erase storage
  // 4. check if verification is supported
  //   a. if yes, perform pre-sample flow
  //
  for (Node = GetFirstNode (&mStorageEraseDriver.PlatformEraseDeviceList);
       !IsNull (&mStorageEraseDriver.PlatformEraseDeviceList, Node);
       Node = GetNextNode (&mStorageEraseDriver.PlatformEraseDeviceList, Node)) {

    TmpDev = PLATFORM_ERASE_DEVICE_FROM_LINK (Node);
    if (!TmpDev->Selected || TmpDev->Erased) {
      continue;
    }

    if (PerformVerify) {
      if (VerifySpecificDevice (TmpDev)) {
        continue;
      }
    }

    Status = EFI_UNSUPPORTED;
    TmpDev->Erased = TRUE;
    DEBUG ((DEBUG_INFO, "PS: storage erase on %a - start\n", TmpDev->EraseInfo.ModelName));
    //
    // show message on the screen
    //
    if (HasOutputConsole) {
      PrintEraseMessage (TmpDev->EraseInfo.ModelName, TmpDev->DeviceType, ERASE_MESSAGE);
    }

    //
    // Unlock the device when the device is locked.
    //
    Status = PerformUnlockStorage (TmpDev, TmpDev->Password, HasOutputConsole);
    if (Status == EFI_NOT_READY) {
      PostponeErase = TRUE;
    } else if (!EFI_ERROR (Status)) {
      //
      // Perform erase
      //
      Status = PerformDeviceErase (TmpDev, TmpDev->Password, HasOutputConsole);
      Result |= Status;
      //
      // Verification
      //
      if (PerformVerify && (TmpDev->Result == ERASE_RESULT_SUCCESS)) {
        if (HasOutputConsole) {
          UpdateEraseMessage (TmpDev->EraseInfo.ModelName, TmpDev->DeviceType, VERIFICATION_MESSAGE);
          Time = GetVerifyTime (TmpDev);
          AppendEraseTime (Time);
        }
        VerifyEraseResult (TmpDev);
      }
    } else {
      Result |= Status;
    }
    //
    // Show erase result on the screen
    //
    if (HasOutputConsole && TmpDev->Erased) {
      UpdateEraseMessage (TmpDev->EraseInfo.ModelName, TmpDev->DeviceType, ResultString (TmpDev->Result, PerformVerify));
    }
    DEBUG ((DEBUG_INFO, "PS: storage erase on %a - end, Status(%r)\n", TmpDev->EraseInfo.ModelName, Status));
  }

  if (PostponeErase) {
    return EFI_NOT_READY;
  } else {
    if (PerformVerify) {
      //
      // Erase the specific device and verify it
      //
      Result = EraseAndVerifySpecificDevice (HasOutputConsole);
    }
  }
  return Result;
}

/**
  Refresh the UI with the erase result done before endOfDxe.

  @param[in] PerformVerify    Enable/Disable verification flow.

**/
VOID
RefreshUiWithEraseResult (
  IN BOOLEAN  PerformVerify
  )
{
  LIST_ENTRY             *Node;
  PLATFORM_ERASE_DEVICE  *TmpDev;

  for (Node = GetFirstNode (&mStorageEraseDriver.PlatformEraseDeviceList);
       !IsNull (&mStorageEraseDriver.PlatformEraseDeviceList, Node);
       Node = GetNextNode (&mStorageEraseDriver.PlatformEraseDeviceList, Node)) {

    TmpDev = PLATFORM_ERASE_DEVICE_FROM_LINK (Node);
    if (!TmpDev->Selected || !TmpDev->Erased) {
      continue;
    }

    PrintEraseMessage (TmpDev->EraseInfo.ModelName, TmpDev->DeviceType, ResultString(TmpDev->Result, PerformVerify));
  }
}

/**
  Register callback function to display messages.

  @param[in]  Event         The Event this notify function registered to.
  @param[in]  Context       Pointer to the context data registered to the Event.
**/
VOID
EFIAPI
StorageEraseOutputConsoleConnected (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  BOOLEAN                          PerformVerify;

  DEBUG ((DEBUG_INFO, "%a : enter.\n", __FUNCTION__));

  PerformVerify  = mStorageEraseDriver.PerformVerify;

  if (!HasOutputConsoleConnected ()) {
    return;
  }

  gBS->CloseEvent (Event);
  //
  // Init UI and refresh UI with erase result performed before endOfDxe
  //
  InitSanitizeUi ();
  RefreshUiWithEraseResult (PerformVerify);

  PerformStorageErase (PerformVerify, TRUE);
  SendEraseResult (mCompleteFunction, mStorageEraseDriver.PerformUsbReport);
}

/**
  Function to start storage erase.

  @param[in] Configuration     Configuration of erase operation.
  @param[in] HostPassword      Host password for storage unlock.
  @param[in] CompleteFunction  Callback function when erase completed.

  @retval EFI_SUCCESS     Storage erase is successful.
  @retval Others          failed to erase all devices.
**/
EFI_STATUS
EFIAPI
StorageEraseExec (
  IN  ERASE_CONFIGURATION    Configuration,
  IN  CHAR8                  *HostPassword,
  IN  STORAGE_ERASE_COMPLETE CompleteFunction
  )
{
  EFI_STATUS             Result;
  BOOLEAN                HasOutputConsole;
  VOID                   *Registration;
  EFI_EVENT              Event;

  DEBUG ((DEBUG_INFO, "%a : enter\n", __FUNCTION__));

  Registration = NULL;
  Event        = NULL;
  mStorageEraseDriver.PerformVerify     = (Configuration.b.Verification == 1) ? TRUE : FALSE;
  mStorageEraseDriver.PerformUsbReport  = (Configuration.b.ReportToUsb == 1) ? TRUE : FALSE;
  Result                                = EFI_SUCCESS;
  HasOutputConsole                      = FALSE;

  HasOutputConsole = HasOutputConsoleConnected ();
  DEBUG ((DEBUG_INFO, "%a : HasOutputConsole %d\n", __FUNCTION__, HasOutputConsole));
  //
  // Register RST devices
  //
  if (!mRegisterRstDevice) {
    RegisterRstDevice (NVME);
    RegisterRstDevice (SATA);
    mRegisterRstDevice = TRUE;
  }

  if (HasOutputConsole) {
    InitSanitizeUi ();
  }

  UpdateDeviceInfo (HostPassword);
  UpdateSanitizeOrder ();
  Result = ResetSystemForStoragePowerCycle ();
  if (!EFI_ERROR (Result)) {
    return EFI_ABORTED;
  }

  if (mStorageEraseDriver.PerformVerify) {
    BackupDeviceForVerify ();
  }
  Result = PerformStorageErase (mStorageEraseDriver.PerformVerify, HasOutputConsole);

  if ((Result == EFI_NOT_READY) || !HasOutputConsole) {
    //
    // Register callback function for output console connected.
    //
    mCompleteFunction = CompleteFunction;
    DEBUG ((DEBUG_INFO, "%a: postpone storage erase.\n", __FUNCTION__));
    Event = RegisterNotifyForConsoleOutConnected (StorageEraseOutputConsoleConnected, &Registration);
    if ((Event == NULL) || (Registration == NULL)) {
      Result = SendEraseResult (CompleteFunction, FALSE);
    }
  } else {
    Result = SendEraseResult (CompleteFunction, mStorageEraseDriver.PerformUsbReport);
  }

  return Result;
}

/**
  Notification function of EFI_END_OF_DXE_EVENT_GROUP_GUID event group.

  This is a notification function registered on EFI_END_OF_DXE_EVENT_GROUP_GUID event group.

  @param[in]  Event    Event whose notification function is being invoked.
  @param[in]  Context  Pointer to the notification function's context.

**/
VOID
EFIAPI
EndOfDxeEventNotify (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  if (!mRegisterRstDevice) {
    RegisterRstDevice (NVME);
    RegisterRstDevice (SATA);
    mRegisterRstDevice = TRUE;
  }

  gBS->CloseEvent (Event);
}

/**
  Main entry for this driver.

  @param[in] ImageHandle     Image Handle this driver.
  @param[in] SystemTable     Pointer to SystemTable.

  @retval EFI_SUCCESS    This function always complete successfully.
**/
EFI_STATUS
EFIAPI
StorageSanitizeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE*  SystemTable
  )
{
  EFI_STATUS                     Status;
  EFI_EVENT                      EndOfDxeEvent;
  EFI_HANDLE                     Handle;
  STORAGE_ERASE_NOTIFY_PROTOCOL  *StorageEraseNotify;

  DEBUG ((DEBUG_INFO, "%a : enter\n", __FUNCTION__));

  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &gStorageEraseDriverBinding,
             ImageHandle,
             &gStorageEraseComponentName,
             &gStorageEraseComponentName2
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Install Driver Binding protocols failed\n"));
    return Status;
  }

  //
  // Initialize Driver private data
  //
  ZeroMem (&mStorageEraseDriver, sizeof (STORAGE_ERASE_DRIVER));
  mStorageEraseDriver.Handle = ImageHandle;
  mStorageEraseDriver.Signature = STORAGE_ERASE_DRIVER_SIGNATURE;
  InitializeListHead (&mStorageEraseDriver.PlatformEraseDeviceList);
  InitializeListHead (&mStorageEraseDriver.DocInfoList);

  //
  // register callback function will be triggered by sanitize function
  //
  StorageEraseNotify = AllocateZeroPool (sizeof (STORAGE_ERASE_NOTIFY_PROTOCOL));
  if (StorageEraseNotify == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
  } else {
    StorageEraseNotify->Revision = STORAGE_ERASE_NOTIFY_PROTOCOL_REVISION;
    StorageEraseNotify->StorageErase = StorageEraseExec;
    StorageEraseNotify->GetDevice = GetDeviceList;

    Handle = NULL;
    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gStorageSanitizeNotifyProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    StorageEraseNotify
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "Install storage erase notify protocol failed(%r).\n", Status));
    }
  }

  //
  // Register callback function will be triggered when end of DXE
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  EndOfDxeEventNotify,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Tests to see if this driver supports a given controller.

  @param[in]  This                  A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle      The Handle of the controller to test. This Handle
                                    must support a protocol interface that supplies
                                    an I/O abstraction to the driver.
  @param[in]  RemainingDevicePath  This parameter is ignored.

  @retval EFI_SUCCESS               The device contains required protocols
  @retval EFI_ALREADY_STARTED       The device specified by ControllerHandle and
                                    RemainingDevicePath is already being managed by the driver
                                    specified by This.
  @retval EFI_ACCESS_DENIED         The device specified by ControllerHandle and
                                    RemainingDevicePath is already being managed by a different
                                    driver or an application that requires exclusive access.
                                    Currently not implemented.
  @retval EFI_UNSUPPORTED           The device does not contain requires protocols

**/
EFI_STATUS
EFIAPI
StorageEraseBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL*  This,
  IN EFI_HANDLE                    Controller,
  IN EFI_DEVICE_PATH_PROTOCOL*     RemainingDevicePath
  )
{
  EFI_STATUS                      Status;
  EFI_DEVICE_PATH_PROTOCOL        *DevicePath;

  //
  // Open EFI_BLOCK_IO_PROTOCOL to check if protocol is installed on the handle
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiBlockIoProtocolGuid,
                  NULL,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Get EFI_DEVICE_PATH_PROTOCOL on the controller
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **)&DevicePath,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR(Status)) {
    return Status;
  }
  //
  // Check if erase protocol exists
  //
  if (!IsEraseSupported (Controller, DevicePath)) {
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  Enables Storage erase Management on a supported device if available.

  The start function is designed to be called after the Opal UEFI Driver has confirmed the
  "controller", which is a child Handle, contains the EF_STORAGE_SECURITY_COMMAND protocols.
  This function will complete the other necessary checks, such as verifying the device supports
  the correct version of Opal.  Upon verification, it will add the device to the
  Opal HII list in order to expose Opal management options.

  @param[in] This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in] ControllerHandle     The Handle of the controller to start. This Handle
                                  must support a protocol interface that supplies
                                  an I/O abstraction to the driver.
  @param[in] RemainingDevicePath  A pointer to the remaining portion of a device path.  This
                                  parameter is ignored by device drivers, and is optional for bus
                                  drivers. For a bus driver, if this parameter is NULL, then handles
                                  for all the children of Controller are created by this driver.
                                  If this parameter is not NULL and the first Device Path Node is
                                  not the End of Device Path Node, then only the Handle for the
                                  child device specified by the first Device Path Node of
                                  RemainingDevicePath is created by this driver.
                                  If the first Device Path Node of RemainingDevicePath is
                                  the End of Device Path Node, no child Handle is created by this
                                  driver.

  @retval EFI_SUCCESS           Storage erase management was enabled.
  @retval EFI_DEVICE_ERROR      The device could not be started due to a device error.Currently not implemented.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.
  @retval Others                The driver failed to start the device.

**/
EFI_STATUS
EFIAPI
StorageEraseBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL*  This,
  IN EFI_HANDLE                    Controller,
  IN EFI_DEVICE_PATH_PROTOCOL*     RemainingDevicePath
  )
{
  EFI_STATUS              Status;
  LIST_ENTRY              *Node;
  PLATFORM_ERASE_DEVICE   *Dev;
  EFI_DEVICE_PATH_PROTOCOL *DevicePath;

  DEBUG ((DEBUG_INFO, "%a : enter\n", __FUNCTION__));
  DevicePath = NULL;

  //
  // Check if device with given handle already exist in the device list
  //
  for (Node = GetFirstNode (&mStorageEraseDriver.PlatformEraseDeviceList);
       !IsNull (&mStorageEraseDriver.PlatformEraseDeviceList, Node);
       Node = GetNextNode (&mStorageEraseDriver.PlatformEraseDeviceList, Node)) {
    Dev = PLATFORM_ERASE_DEVICE_FROM_LINK (Node);
    if (Dev->Handle == Controller) {
      DEBUG ((DEBUG_INFO, "device is already in the list. \n"));
      return EFI_SUCCESS;
    }
  }

  //
  // Create internal device for tracking.  This allows all disks to be tracked
  // by same HII form
  //
  Dev = AllocateZeroPool (sizeof (PLATFORM_ERASE_DEVICE));
  if (Dev == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Get EFI_DEVICE_PATH_PROTOCOL on the controller
  //
  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiDevicePathProtocolGuid,
                  (VOID **)&DevicePath,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Device path protocol is not installed on device handle. \n"));
    goto Done;
  }

  //
  // Update device type from device path
  //
  Dev->DeviceType = GetDeviceType (DevicePath);
  UpdateDeviceCmdId (DevicePath, Dev);

  //
  // Initialize platform erase device structure
  //
  Dev->Signature = PLATFORM_ERASE_DEVICE_SIGNATURE;
  Dev->Handle = Controller;
  Dev->Selected = 1;

  //
  // Get device information (Name, SN)
  //
  GetEraseInfo (Dev->Handle, Dev->DeviceType, &Dev->EraseInfo);

  //
  // Add device to the Platform Erase device list
  //
  InsertTailList (&mStorageEraseDriver.PlatformEraseDeviceList, &Dev->Link);
  DEBUG ((DEBUG_INFO, "AddDeviceToPlatformEraseList - Name: %a\n", Dev->EraseInfo.ModelName));
  DEBUG ((DEBUG_INFO, "                               type: %d, Selected: %d, IsVmdDevice: %d, CmdId: %x\n",
                                                      Dev->DeviceType, Dev->Selected, Dev->VmdControlled, Dev->CmdId.Value));

  return EFI_SUCCESS;

Done:

  if (Dev != NULL) {
    FreePool (Dev);
  }

  return EFI_DEVICE_ERROR;
}

/**
  Stop this driver on Controller.

  @param[in] This               Protocol instance pointer.
  @param[in] Controller         Handle of device to stop driver on
  @param[in] NumberOfChildren   Number of Handles in ChildHandleBuffer. If number of
                                children is zero stop the entire bus driver.
  @param[in] ChildHandleBuffer  List of Child Handles to Stop.

  @retval EFI_SUCCESS  This driver is removed Controller.
  @retval other        This driver could not be removed from this device.

**/
EFI_STATUS
EFIAPI
StorageEraseBindingStop (
  IN EFI_DRIVER_BINDING_PROTOCOL*  This,
  IN EFI_HANDLE                    Controller,
  IN UINTN                         NumberOfChildren,
  IN EFI_HANDLE*                   ChildHandleBuffer
  )
{
  return DriverStopDevice (Controller);
}

/**
  Unloads UEFI Driver.  Very useful for debugging and testing.

  @param[in] ImageHandle  Image Handle this driver.

  @retval EFI_SUCCESS            This function always complete successfully.
  @retval EFI_INVALID_PARAMETER  The input ImageHandle is not valid.
**/
EFI_STATUS
EFIAPI
StorageSanitizeUnload (
  IN EFI_HANDLE  ImageHandle
  )
{
  LIST_ENTRY             *Node;
  EFI_STATUS             Status;
  PLATFORM_ERASE_DEVICE  *Dev;

  Status = EFI_SUCCESS;

  if (ImageHandle != gImageHandle) {
    return (EFI_INVALID_PARAMETER);
  }

  //
  // Check if device in list already
  //
  for (Node = GetFirstNode (&mStorageEraseDriver.PlatformEraseDeviceList);
       !IsNull (&mStorageEraseDriver.PlatformEraseDeviceList, Node);
       Node = GetNextNode (&mStorageEraseDriver.PlatformEraseDeviceList, Node)) {
    Dev = PLATFORM_ERASE_DEVICE_FROM_LINK (Node);
    RemoveDevice (Dev);
  }

  return Status;
}
