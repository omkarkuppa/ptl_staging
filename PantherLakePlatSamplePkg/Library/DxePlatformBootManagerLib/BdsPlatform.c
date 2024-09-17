/** @file
  This file include all platform action which can be customized by IBV/OEM.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2004 Intel Corporation.

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

#include "BdsPlatform.h"
#include <Library/UserPasswordUiLib.h>
#include <Protocol/ReportStatusCodeHandler.h>
#include "AmtSupport.h"
#include "FastBootSupport.h"
#include "String.h"
#include <Guid/EventGroup.h>
#include <Protocol/DxeSmmReadyToLock.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Library/TcgPhysicalPresenceLib.h>
#include <Setup/MeSetup.h>
#include <Library/EcMiscLib.h>
#include <Library/BaseLib.h>

#include <Protocol/BlockIo.h>
#include "PlatformBoardId.h"
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PchInfoLib.h>
#include <Guid/EventGroup.h>
#include <Guid/ImageAuthentication.h>
#include <BootStateLib.h>
#include <Library/PciHostBridgeLib.h>

#include <Library/ResiliencySupportLib.h>
#include <Library/Tcg2PhysicalPresenceLib.h> // TCG2 implementation
#include <Protocol/FastBootException.h>

#include <Protocol/ResetNotification.h>
#include <Library/BiosIdLib.h>
#include <Library/PmcLib.h>

#include <Setup.h>
#include <Library/UserPasswordLib.h>
#ifdef _MSC_VER
#pragma optimize("g", off)
#endif

#include <AttemptUsbFirst.h>
#include <Library/HobLib.h>
#include <Protocol/UsbIo.h>
#include <Guid/PlatformGlobalVariable.h>

#include <Library/SerialPortLib.h>
#if FixedPcdGetBool (PcdTerminalOverDebugEnable) == 1
#include <Protocol/TerminalOverCommon.h>
#endif
#include <PlatformBoardConfig.h>
#include <Register/PchRegs.h>
#include <MeBiosPayloadHob.h>
#include <Library/DxeAsfLib.h>
#include <Library/DxeAmtSupportLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/PchPciBdfLib.h>
#include <ChassisIntrDetHob.h>
#if FixedPcdGetBool (PcdCapsuleEnable) == 1
#include <Guid/SysFwUpdateProgress.h>
#endif
#include <Library/PlatformWdtLib.h>
#include <CnvVfrSetupMenuHii.h>
#include <IGpuConfig.h>

#define BdsDispatchExternalOpromDisable 0x0
#define BdsDispatchExternalOpromStorage 0x1
#define BdsDispatchExternalOpromOther   0x2
#define BdsDispatchExternalOpromBoth    0x3
#define INTEL_VENDOR_ID    0x8086
#define EFI_NETWORK_DISABLED 0
#define EFI_NETWORK_ALL      1
#define EFI_NETWORK_WIFI     2
#define EFI_NETWORK_ONBOARD_NIC  3
#if FixedPcdGetBool (PcdVirtualKeyboardEnable) == 1
extern PLATFORM_VIRTUAL_KEYBOARD_I2C_DEVICE_PATH  gVirtualKeyboardDevicePath;
#endif
extern UINTN                                      mBootMenuOptionNumber;

EFI_RSC_HANDLER_PROTOCOL      *mRscHandlerProtocol     = NULL;

GLOBAL_REMOVE_IF_UNREFERENCED SA_SETUP                      mSaSetup;
GLOBAL_REMOVE_IF_UNREFERENCED ME_SETUP                      mMeSetup;
GLOBAL_REMOVE_IF_UNREFERENCED SETUP_DATA                    mSystemConfiguration;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_BOOT_MODE                 gBootMode;

GLOBAL_REMOVE_IF_UNREFERENCED EFI_GUID  *mLibTerminalType[] = {
  &gEfiPcAnsiGuid,
  &gEfiVT100Guid,
  &gEfiVT100PlusGuid,
  &gEfiVTUTF8Guid
};

extern BOOLEAN                      mAnyKeypressed;
extern BOOLEAN                      gPPRequireUIConfirm;

#define BIOS_COLOR_CODING_BAR_HEIGHT  40

//
// BDS Platform Functions
//

EFI_GUID  mAmtConsoleVariableGuid = {
  0xd9aaf1e5, 0xcd14, 0x4312, { 0x9c, 0xa4, 0x85, 0xc3, 0xa, 0xde, 0x43, 0xe8 }
};

//
// For password status
//
extern EFI_GUID gSystemAccessGuid;

typedef  VOID (*GEN_GBT_XML)(VOID *XmlCliProto);
typedef struct _XML_CLI_PROTO {
  GEN_GBT_XML   GenerateGbtXml;
} XML_CLI_PROTO;


/**
  This function is responsible to generate GBT Xml during Platform Runtime
  If this method is not called then XmlCli will not be full operational and feature
  would not have any Setup variable information xml.

 * @return VOID
 */
VOID
GenGbtXml (
  VOID
  )
{
  EFI_STATUS     Status;
  XML_CLI_PROTO  *XmlCliProto;
  EFI_GUID       XmlCliProtocolGuid  = {
    0xe3e49b8d, 0x1987, 0x48d0, { 0x9a, 0x1, 0xed, 0xa1, 0x79, 0xca, 0xb, 0xd6 }
  };  // This GUID is similar to definition in XmlCliFeaturePkg
  Status = gBS->LocateProtocol ( &XmlCliProtocolGuid, NULL, (VOID **)&XmlCliProto );
  if (!EFI_ERROR (Status)) {
    XmlCliProto->GenerateGbtXml((VOID*)XmlCliProto);
  }
}


/**
  Update gPlatformConsole to ConIn variable
**/
VOID
UpdatePlatformConsoleToConInVairable (
  VOID
  )
{
  UINTN       Index;
  for (Index = 0; gPlatformConsole[Index].DevicePath != NULL; Index++) {
    //
    // Update the console variable with the connect type
    //
    if ((gPlatformConsole[Index].ConnectType & CONSOLE_IN) == CONSOLE_IN) {

      if (CompareMem (gPlatformConsole[Index].DevicePath, &gKeyboardDevicePath, GetDevicePathSize ((EFI_DEVICE_PATH_PROTOCOL *) &gKeyboardDevicePath) - END_DEVICE_PATH_LENGTH) == 0){
        //
        // Only Update PS2 Device Path while PS2 Keyboard Connected.
        //
        if(IsPs2KeyboardConnected()){
          EfiBootManagerUpdateConsoleVariable (ConIn, gPlatformConsole[Index].DevicePath, NULL);
        }
      }else{
        EfiBootManagerUpdateConsoleVariable (ConIn, gPlatformConsole[Index].DevicePath, NULL);
      }
    }
    if ((gPlatformConsole[Index].ConnectType & CONSOLE_OUT) == CONSOLE_OUT) {
      EfiBootManagerUpdateConsoleVariable (ConOut, gPlatformConsole[Index].DevicePath, NULL);
    }
    if ((gPlatformConsole[Index].ConnectType & STD_ERROR) == STD_ERROR) {
      EfiBootManagerUpdateConsoleVariable (ErrOut, gPlatformConsole[Index].DevicePath, NULL);
    }
  }
}

/**
   This function prints the string corresponding to the device's path into the serial output.

   @param Name                        A string to pre-append to the device path being dumped.
   @param DevicePath                  A pointer to the device path structure.
**/
VOID
DumpDevicePath (
  IN CHAR16           *Name,
  IN EFI_DEVICE_PATH  *DevicePath
  )
{
  CHAR16 *Str;

  Str = ConvertDevicePathToText (DevicePath, TRUE, TRUE);
  if (Str != NULL) {
    DEBUG ((DEBUG_INFO, "%s: %s\n", Name, Str));
    FreePool (Str);
  } else {
    DEBUG ((DEBUG_INFO, "%s: NULL\n", Name));
  }
}

/**
  Enable power button SMI and 4 second power button shutdown.

**/
VOID
PlatformEnablePowerButton (
  VOID
  )
{
  //
  // Enable Ec power button handler (4 seconds pressing)
  //
  EcEnablePowerButton ();

  //
  // Enable power button SMI
  //
  PmcEnablePowerButtonSmi ();
}

/**
  PowerButton call back function for Reset notification.

  @param[in] ResetType            UEFI defined reset type.
  @param[in] ResetStatus          The status code for the reset.
  @param[in] DataSize             The size of ResetData in bytes.
  @param[in] ResetData            Optional element used to introduce a platform specific reset.
                                  The exact type of the reset is defined by the EFI_GUID that follows
                                  the Null-terminated Unicode string.
**/
VOID
EFIAPI
PowerButtonResetCallback (
  IN EFI_RESET_TYPE           ResetType,
  IN EFI_STATUS               ResetStatus,
  IN UINTN                    DataSize,
  IN VOID                     *ResetData OPTIONAL
  )
{
  //
  // In case EC 4 seconds power button is still disabled after reset
  //
  PlatformEnablePowerButton ();
}

/**
  Disable power button SMI and 4 second power button shutdown.
**/
VOID
PlatformDisablePowerButton (
  VOID
  )
{
  EFI_STATUS                               Status;
  STATIC EFI_RESET_NOTIFICATION_PROTOCOL   *ResetNotify = NULL;

  //
  // Disable power button SMI
  //
  PmcDisablePowerButtonSmi ();

  //
  // Disable EC power button handler (4 seconds pressing)
  //
  EcDisablePowerButton ();

  if (ResetNotify != NULL) {
    Status = gBS->LocateProtocol (&gEfiResetNotificationProtocolGuid, NULL, (VOID **) &ResetNotify);
    if (!EFI_ERROR (Status)) {
      Status = ResetNotify->RegisterResetNotify (ResetNotify, PowerButtonResetCallback);
      DEBUG ((DEBUG_INFO, "PowerButtonResetCallback() installed\n"));
    }
  }
}

/**
  The handle on the path we get might be not the display device.
  We must check it.

  @todo fix the parameters

  @retval  TRUE         PCI class type is VGA.
  @retval  FALSE        PCI class type isn't VGA.
**/
BOOLEAN
IsVgaHandle (
  IN EFI_HANDLE Handle
  )
{
  EFI_PCI_IO_PROTOCOL *PciIo;
  PCI_TYPE00          Pci;
  EFI_STATUS          Status;

  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiPciIoProtocolGuid,
                  (VOID **)&PciIo
                  );

  if (!EFI_ERROR (Status)) {
    Status = PciIo->Pci.Read (
                          PciIo,
                          EfiPciIoWidthUint32,
                          0,
                          sizeof (Pci) / sizeof (UINT32),
                          &Pci
                          );

    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "  PCI CLASS CODE    = 0x%x\n", Pci.Hdr.ClassCode [2]));
      DEBUG ((DEBUG_INFO, "  PCI SUBCLASS CODE = 0x%x\n", Pci.Hdr.ClassCode [1]));

      if (IS_PCI_VGA (&Pci) || IS_PCI_OLD_VGA (&Pci)) {
        DEBUG ((DEBUG_INFO, "  \nPCI VGA Device Found\n"));
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**
  Generic function to update the console variable.
  Please refer to FastBootSupport.c for how to use it.

  @param VariableName    - The name of the variable to be updated
  @param AgentGuid       - The Agent GUID
  @param ProcessVariable - The function pointer to update the variable
                           NULL means to restore to the original value
**/
VOID
UpdateEfiGlobalVariable (
  CHAR16           *VariableName,
  EFI_GUID         *AgentGuid,
  PROCESS_VARIABLE ProcessVariable
  )
{
  EFI_STATUS  Status;
  CHAR16      BackupVariableName[20];
  CHAR16      FlagVariableName[20];
  VOID        *Variable;
  VOID        *BackupVariable;
  VOID        *NewVariable;
  UINTN       VariableSize;
  UINTN       BackupVariableSize;
  UINTN       NewVariableSize;
  BOOLEAN     Flag;
  BOOLEAN     *FlagVariable;

  ASSERT (StrLen (VariableName) <= 13);
  UnicodeSPrint (BackupVariableName, sizeof (BackupVariableName), L"%sBackup", VariableName);
  UnicodeSPrint (FlagVariableName, sizeof (FlagVariableName), L"%sModify", VariableName);

  GetVariable2 (VariableName,       &gEfiGlobalVariableGuid, (VOID **) &Variable,       &VariableSize);
  GetVariable2 (BackupVariableName, AgentGuid,               (VOID **) &BackupVariable, &BackupVariableSize);
  GetVariable2 (FlagVariableName,   AgentGuid,               (VOID **) &FlagVariable,   NULL);
  if (ProcessVariable != NULL) {
    if (FlagVariable == NULL) {
      //
      // Last boot is normal boot
      // Set flag
      // BackupVariable <- Variable
      // Variable       <- ProcessVariable (Variable)
      //
      Flag   = TRUE;
      Status = gRT->SetVariable (
                      FlagVariableName,
                      AgentGuid,
                      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                      sizeof (Flag),
                      &Flag
                      );
      ASSERT_EFI_ERROR (Status);

      Status = gRT->SetVariable (
                      BackupVariableName,
                      AgentGuid,
                      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                      VariableSize,
                      Variable
                      );
      ASSERT ((Status == EFI_SUCCESS) || (Status == EFI_NOT_FOUND));

      NewVariable     = Variable;
      NewVariableSize = VariableSize;
      ProcessVariable (&NewVariable, &NewVariableSize);

      Status = gRT->SetVariable (
                      VariableName,
                      &gEfiGlobalVariableGuid,
                      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                      NewVariableSize,
                      NewVariable
                      );
      ASSERT ((Status == EFI_SUCCESS) || (Status == EFI_NOT_FOUND));

      if ((NewVariable != NULL) && (NewVariable != Variable)) {
        FreePool (NewVariable);
      }
    } else { // LastBootIsModifiedPtr != NULL
      //
      // Last Boot is modified boot
      //
      if (StrCmp (VariableName, L"ConIn") == 0 && CompareGuid (AgentGuid, &mAmtConsoleVariableGuid)) {
        // This means that BIOS will need to execute keyboard lock continuously
        // keep FlagVariable to TRUE in mAmtConsoleVariableGuid
        // Then save the newer ConIn to BackupVariable and clear it in GlobalVariable
        //
        // Set flag
        // BackupVariable <- Variable
        // Variable       <- ProcessVariable (Variable)
        //
        Flag   = TRUE;
        Status = gRT->SetVariable (
                        FlagVariableName,
                        AgentGuid,
                        EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                        sizeof (Flag),
                        &Flag
                        );
        ASSERT_EFI_ERROR (Status);

        Status = gRT->SetVariable (
                        BackupVariableName,
                        AgentGuid,
                        EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                        VariableSize,
                        Variable
                        );
        ASSERT ((Status == EFI_SUCCESS) || (Status == EFI_NOT_FOUND));

        NewVariable     = Variable;
        NewVariableSize = VariableSize;
        ProcessVariable (&NewVariable, &NewVariableSize);

        Status = gRT->SetVariable (
                        VariableName,
                        &gEfiGlobalVariableGuid,
                        EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                        NewVariableSize,
                        NewVariable
                        );
        ASSERT ((Status == EFI_SUCCESS) || (Status == EFI_NOT_FOUND));

        if ((NewVariable != NULL) && (NewVariable != Variable)) {
          FreePool (NewVariable);
        }
      }
    }
  } else {
    if (FlagVariable != NULL) {
      //
      // Last boot is modified boot
      // Clear LastBootIsModified flag
      // Variable       <- BackupVariable
      // BackupVariable <- NULL
      //
      Status = gRT->SetVariable (
                      FlagVariableName,
                      AgentGuid,
                      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                      0,
                      NULL
                      );
      ASSERT_EFI_ERROR (Status);

      Status = gRT->SetVariable (
                      VariableName,
                      &gEfiGlobalVariableGuid,
                      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                      BackupVariableSize,
                      BackupVariable
                      );
      ASSERT ((Status == EFI_SUCCESS) || (Status == EFI_NOT_FOUND));

      Status = gRT->SetVariable (
                      BackupVariableName,
                      AgentGuid,
                      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                      0,
                      NULL
                      );
      ASSERT ((Status == EFI_SUCCESS) || (Status == EFI_NOT_FOUND));
    } else { // LastBootIsModifiedPtr == NULL
      //
      // Last boot is normal boot
      //
    }
  }

  if (Variable != NULL) {
    FreePool (Variable);
  }

  if (BackupVariable != NULL) {
    FreePool (BackupVariable);
  }

  if (FlagVariable != NULL) {
    FreePool (FlagVariable);
  }
}

VOID
SignalAllDriversConnected (
  VOID
  )
{
  EFI_HANDLE                 Handle;
  EFI_STATUS                 Status;

  //
  // Inform other code that all drivers have been connected.
  //
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gBdsAllDriversConnectedProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
}

/**
  An empty function to pass error checking of CreateEventEx ().

  This empty function ensures that EVT_NOTIFY_SIGNAL_ALL is error
  checked correctly since it is now mapped into CreateEventEx() in UEFI 2.0.

  @param  Event                 Event whose notification function is being invoked.
  @param  Context               The pointer to the notification function's context,
                                which is implementation-dependent.
**/
VOID
EFIAPI
InternalBdsEmptyCallbackFuntion (
  IN EFI_EVENT                Event,
  IN VOID                     *Context
  )
{
  return;
}

VOID
ExitPmAuth (
  VOID
  )
{
  EFI_HANDLE                 Handle;
  EFI_STATUS                 Status;
  EFI_EVENT                  EndOfDxeEvent;

  DEBUG((DEBUG_INFO,"ExitPmAuth ()- Start\n"));

  //
  // Since PI1.2.1, we need signal EndOfDxe as ExitPmAuth
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  InternalBdsEmptyCallbackFuntion,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);
  gBS->SignalEvent (EndOfDxeEvent);
  gBS->CloseEvent (EndOfDxeEvent);
  DEBUG((DEBUG_INFO,"All EndOfDxe callbacks have returned successfully\n"));

  //
  // NOTE: We need install DxeSmmReadyToLock directly here because many boot script is added via ExitPmAuth/EndOfDxe callback.
  // If we install them at same callback, these boot script will be rejected because BootScript Driver runs first to lock them done.
  // So we separate them to be 2 different events, ExitPmAuth is last chance to let platform add boot script. DxeSmmReadyToLock will
  // make boot script save driver lock down the interface.
  //
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gEfiDxeSmmReadyToLockProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
  DEBUG((DEBUG_INFO,"ExitPmAuth ()- End\n"));
}

VOID
ConnectRootBridge (
  BOOLEAN Recursive
  )
{
  UINTN                            RootBridgeHandleCount;
  EFI_HANDLE                       *RootBridgeHandleBuffer;
  UINTN                            RootBridgeIndex;

  RootBridgeHandleCount = 0;
  gBS->LocateHandleBuffer (
         ByProtocol,
         &gEfiPciRootBridgeIoProtocolGuid,
         NULL,
         &RootBridgeHandleCount,
         &RootBridgeHandleBuffer
         );
  for (RootBridgeIndex = 0; ((RootBridgeIndex < RootBridgeHandleCount) && (RootBridgeHandleBuffer != NULL)); RootBridgeIndex++) {
    gBS->ConnectController (RootBridgeHandleBuffer[RootBridgeIndex], NULL, NULL, Recursive);
  }
  FreePool (RootBridgeHandleBuffer);
}

BOOLEAN
IsGopDevicePath (
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  while (!IsDevicePathEndType (DevicePath)) {
    if (DevicePathType (DevicePath) == ACPI_DEVICE_PATH &&
        DevicePathSubType (DevicePath) == ACPI_ADR_DP) {
      return TRUE;
    }
    DevicePath = NextDevicePathNode (DevicePath);
  }
  return FALSE;
}

VOID
InitConInActVariable (
  VOID
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *ConInDevPath;
  UINTN                     ConInSize;
  GetEfiGlobalVariable2 (L"ConIn", (VOID **) &ConInDevPath, &ConInSize);
  if (ConInDevPath != NULL) {
    gRT->SetVariable (
           L"ConInAct",
           &gPlatformGlobalVariableGuid,
           EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
           ConInSize,
           ConInDevPath
           );
    FreePool (ConInDevPath);
  }
}

/**
  Function compares a device path data structure to that of all the nodes of a
  second device path instance.

  @param  Multi                 A pointer to a multi-instance device path data
                                structure.
  @param  Single                A pointer to a single-instance device path data
                                structure.

  @retval TRUE                  If the Single device path is contained within Multi device path.
  @retval FALSE                 The Single device path is not match within Multi device path.

**/
BOOLEAN
PlatformMatchDevicePaths (
  IN  EFI_DEVICE_PATH_PROTOCOL  *Multi,
  IN  EFI_DEVICE_PATH_PROTOCOL  *Single
  )
{
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePathInst;
  UINTN                     Size;

  if (Multi == NULL || Single  == NULL) {
    return FALSE;
  }

  DevicePath     = Multi;
  DevicePathInst = GetNextDevicePathInstance (&DevicePath, &Size);

  //
  // Search for the match of 'Single' in 'Multi'
  //
  while (DevicePathInst != NULL) {
    //
    // If the single device path is found in multiple device paths,
    // return success
    //
    if (CompareMem (Single, DevicePathInst, Size) == 0) {
      FreePool (DevicePathInst);
      return TRUE;
    }

    FreePool (DevicePathInst);
    DevicePathInst = GetNextDevicePathInstance (&DevicePath, &Size);
  }

  return FALSE;
}

/**
  Update the ConIn variable if Ps2 keyboard is connected.
**/
VOID
EnumPs2Keyboard (
  VOID
  )
{
  UINTN                     DevicePathSize;
  UINTN                     NewDevPathSize;
  UINTN                     Size;
  EFI_DEVICE_PATH_PROTOCOL  *VarConIn;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePathInstance;
  EFI_DEVICE_PATH_PROTOCOL  *Next;
  EFI_DEVICE_PATH_PROTOCOL  *NewDevicePath;
  EFI_DEVICE_PATH_PROTOCOL  *VarConInAct;
  BOOLEAN                   Ps2Keyboard;
  BOOLEAN                   Ps2KeyboardActive;

  Ps2Keyboard = FALSE;
  Ps2KeyboardActive = FALSE;
  GetEfiGlobalVariable2 (L"ConIn",    (VOID **) &VarConIn,    &Size);
  if ((VarConIn == NULL) || (Size < sizeof (EFI_DEVICE_PATH_PROTOCOL))) {
    return;
  }
  GetVariable2 (L"ConInAct", &gPlatformGlobalVariableGuid, (VOID **) &VarConInAct, &Size);
  if ((VarConInAct == NULL) || (Size < sizeof (EFI_DEVICE_PATH_PROTOCOL))) {
    return;
  }

  //
  // If ConIn variable is empty, need to enumerate PS/2 keyboard device path
  //
  do {
    DevicePathInstance = GetNextDevicePathInstance (
                           &VarConIn,
                           &DevicePathSize
                           );

    if (DevicePathInstance == NULL) {
      //
      // The instance is NULL, it means the VarConIn is null, escape the DO loop,
      // and need to add PS/2 keyboard dev path.
      //
      break;
    }

    Next = DevicePathInstance;
    while (!IsDevicePathEndType(Next)) {
      //
      // Checking the device path to see the PS/2 keyboard existance.
      //
      if ((Next->Type    == ACPI_DEVICE_PATH) &&
          (Next->SubType == ACPI_DP         ) &&
          (((ACPI_HID_DEVICE_PATH *) Next)->HID == EISA_PNP_ID (0x0303))) {
        //
        // PS/2 keyboard already exists.
        //
        DEBUG ((DEBUG_INFO, "[EnumPs2Keyboard] PS2 keyboard path exists\n"));
        Ps2Keyboard = TRUE;
        break;
      }
      Next = NextDevicePathNode (Next);
    } // while (!IsDevicePathEndType(Next));

    if (DevicePathInstance != NULL) {
      FreePool (DevicePathInstance);
    }
  } while (VarConIn != NULL);

  //
  // PS/2 keyboard device path does not exist, so try detecting ps2 keyboard
  // and add-in its device path.
  //
  if (!Ps2Keyboard) {
    DEBUG ((DEBUG_INFO, "[EnumPs2Keyboard] Adding detected PS2 keyboard to ConIn.\n"));
    Ps2KeyboardActive = PlatformMatchDevicePaths (
                          VarConInAct,
                          (EFI_DEVICE_PATH_PROTOCOL *) &gKeyboardDevicePath
                          );

    if (IsPs2KeyboardConnected() && Ps2KeyboardActive) {
      NewDevicePath = AppendDevicePathInstance (
                        VarConIn,
                        (EFI_DEVICE_PATH_PROTOCOL *) &gKeyboardDevicePath
                        );
      NewDevPathSize = DevicePathSize + sizeof (gKeyboardDevicePath);

      gRT->SetVariable (
             L"ConIn",
             &gEfiGlobalVariableGuid,
             EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
             NewDevPathSize,
             NewDevicePath
             );
    }
  }

  if (VarConIn != NULL) {
    FreePool (VarConIn);
  }
}

/**
  Update the ConIn variable with USB Keyboard device path,if its not already exists in ConIn
**/
VOID
EnumUsbKeyboard (
  VOID
  )
{
  UINTN                     DevicePathSize;
  EFI_DEVICE_PATH_PROTOCOL  *VarConIn;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePathInstance;
  EFI_DEVICE_PATH_PROTOCOL  *Next;
  BOOLEAN                   UsbKeyboard;

  UsbKeyboard = FALSE;
  GetEfiGlobalVariable2 (L"ConIn", (VOID **) &VarConIn, NULL);

  //
  // If ConIn variable is empty, need to enumerate USB keyboard device path
  //
  do {
    DevicePathInstance = GetNextDevicePathInstance (
                           &VarConIn,
                           &DevicePathSize
                           );

    if (DevicePathInstance == NULL) {
      //
      // The instance is NULL, it means the VarConIn is null, escape the DO loop,
      // and need to add USB keyboard dev path.
      //
      break;
    }

    Next = DevicePathInstance;
    while (!IsDevicePathEndType(Next)) {
      //
      // Checking the device path to see the USB keyboard existance.
      //
      if ((Next->Type    == MESSAGING_DEVICE_PATH) &&
          (Next->SubType == MSG_USB_CLASS_DP) &&
          (((USB_CLASS_DEVICE_PATH *) Next)->DeviceClass == CLASS_HID) &&
          (((USB_CLASS_DEVICE_PATH *) Next)->DeviceSubClass == SUBCLASS_BOOT) &&
          (((USB_CLASS_DEVICE_PATH *) Next)->DeviceProtocol == PROTOCOL_KEYBOARD)) {
        DEBUG ((DEBUG_INFO, "[EnumUsbKeyboard] USB keyboard path exists\n"));
        UsbKeyboard = TRUE;

        break;
      }
      Next = NextDevicePathNode (Next);
    } // while (!IsDevicePathEndType(Next));

    if (DevicePathInstance != NULL) {
      FreePool (DevicePathInstance);
    }
  } while (VarConIn != NULL);

  //
  //  USB keyboard device path does not exist, So add it to the ConIn
  //
  if (!UsbKeyboard) {
    DEBUG ((DEBUG_INFO, "[EnumUsbKeyboard] Adding USB keyboard device path to ConIn.\n"));
    EfiBootManagerUpdateConsoleVariable (ConIn, (EFI_DEVICE_PATH_PROTOCOL *) &gUsbClassKeyboardDevicePath, NULL);
  }

  if (VarConIn != NULL) {
    FreePool (VarConIn);
  }
}

/**
  Check whether the device path node is ISA Serial Node.

  @param Acpi           Device path node to be checked

  @retval TRUE          It's ISA Serial Node.
  @retval FALSE         It's NOT ISA Serial Node.

**/
BOOLEAN
IsIsaSerialNode (
  IN ACPI_HID_DEVICE_PATH *Acpi
  )
{
  return (BOOLEAN) (
      (DevicePathType (Acpi) == ACPI_DEVICE_PATH) &&
      (DevicePathSubType (Acpi) == ACPI_DP) &&
      (ReadUnaligned32 (&Acpi->HID) == EISA_PNP_ID (0x0501))
      );
}

/**
  Test whether DevicePath is a valid Terminal


  @param DevicePath      DevicePath to be checked

  @retval  TRUE         If DevicePath point to a Terminal.
  @retval  FALSE        If DevicePath does not point to a Terminal.

**/
BOOLEAN
IsTerminalDevicePath (
  IN  EFI_DEVICE_PATH_PROTOCOL *DevicePath
  )
{
  BOOLEAN                   IsTerminal;
  EFI_DEVICE_PATH_PROTOCOL  *Node;
  VENDOR_DEVICE_PATH        *Vendor;
  UART_DEVICE_PATH          *Uart;
  ACPI_HID_DEVICE_PATH      *Acpi;

  IsTerminal = FALSE;

  Uart   = NULL;
  Vendor = NULL;
  Acpi   = NULL;
  for (Node = DevicePath; !IsDevicePathEnd (Node); Node = NextDevicePathNode (Node)) {
    //
    // Vendor points to the node before the End node
    //
    Vendor = (VENDOR_DEVICE_PATH *) Node;

    if ((DevicePathType (Node) == MESSAGING_DEVICE_PATH) && (DevicePathSubType (Node) == MSG_UART_DP)) {
      Uart = (UART_DEVICE_PATH *) Node;
    }

    if (Uart == NULL) {
      //
      // Acpi points to the node before the UART node
      //
      Acpi = (ACPI_HID_DEVICE_PATH *) Node;
    }
  }

  if (Vendor == NULL ||
      DevicePathType (Vendor) != MESSAGING_DEVICE_PATH ||
      DevicePathSubType (Vendor) != MSG_VENDOR_DP ||
      Uart == NULL) {
    return FALSE;
  }

  //
  // There are four kinds of Terminal types
  // check to see whether this devicepath
  // is one of that type
  //
  if (CompareGuid (&Vendor->Guid, mLibTerminalType[0]) ||
      CompareGuid (&Vendor->Guid, mLibTerminalType[1]) ||
      CompareGuid (&Vendor->Guid, mLibTerminalType[2]) ||
      CompareGuid (&Vendor->Guid, mLibTerminalType[3])) {
    IsTerminal  = TRUE;
  } else {
    IsTerminal = FALSE;
  }

  if (!IsTerminal) {
    return FALSE;
  }

  if ((Acpi != NULL) && IsIsaSerialNode (Acpi)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Return whether the device is trusted console.

  @param Device  The device to be tested.

  @retval TRUE   The device can be trusted.
  @retval FALSE  The device cannot be trusted.
**/
BOOLEAN
IsTrustedConsole (
  IN CONSOLE_TYPE              ConsoleType,
  IN EFI_DEVICE_PATH_PROTOCOL  *Device
  )
{
  VOID                      *TrustedConsoleDevicepath;
  EFI_DEVICE_PATH_PROTOCOL  *TempDevicePath;
  EFI_DEVICE_PATH_PROTOCOL  *Instance;
  UINTN                     Size;
  EFI_DEVICE_PATH_PROTOCOL  *ConsoleDevice;
  EFI_DEVICE_PATH_PROTOCOL  *SolDevicePath;
  EFI_DEVICE_PATH_PROTOCOL  *SolInstance;

  if (Device == NULL) {
    return FALSE;
  }

  ConsoleDevice = DuplicateDevicePath (Device);

  //
  // Add dummy check to prevent from complaining of Klockwork
  //
  if (ConsoleDevice == NULL) {
    return FALSE;
  }

  if (AsfIsSolEnabled ()) {
    SolDevicePath = (EFI_DEVICE_PATH_PROTOCOL *) AmtGetSolDevicePath ();
    if (SolDevicePath != NULL) {
      SolInstance = GetNextDevicePathInstance (&SolDevicePath, &Size);
      if (CompareMem (ConsoleDevice, SolInstance, GetDevicePathSize (SolInstance)) == 0) {
        //
        // SOL should be considered as Trusted console in/out
        //
        return TRUE;
      }
    }
  }

  TrustedConsoleDevicepath = NULL;

  switch (ConsoleType) {
  case ConIn:
    TrustedConsoleDevicepath = PcdGetPtr (PcdTrustedConsoleInputDevicePath);
    break;
  case ConOut:
    //
    // Check GOP and remove last node
    //
    TempDevicePath = ConsoleDevice;
    while (!IsDevicePathEndType (TempDevicePath)) {
      if (DevicePathType (TempDevicePath) == ACPI_DEVICE_PATH &&
          DevicePathSubType (TempDevicePath) == ACPI_ADR_DP) {
        SetDevicePathEndNode (TempDevicePath);
        break;
      }
      TempDevicePath = NextDevicePathNode (TempDevicePath);
    }

    TrustedConsoleDevicepath = PcdGetPtr (PcdTrustedConsoleOutputDevicePath);
    break;
  default:
    ASSERT(FALSE);
    break;
  }

  TempDevicePath = TrustedConsoleDevicepath;
  do {
    Instance = GetNextDevicePathInstance (&TempDevicePath, &Size);
    if (Instance == NULL) {
      break;
    }

    if (CompareMem (ConsoleDevice, Instance, Size - END_DEVICE_PATH_LENGTH) == 0) {
      FreePool (Instance);
      FreePool (ConsoleDevice);
      return TRUE;
    }

    FreePool (Instance);
  } while (TempDevicePath != NULL);

  FreePool (ConsoleDevice);

  return FALSE;
}

BOOLEAN
IsUsbShortForm (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  if ((DevicePathType (DevicePath) == MESSAGING_DEVICE_PATH) &&
      ((DevicePathSubType (DevicePath) == MSG_USB_CLASS_DP) ||
       (DevicePathSubType (DevicePath) == MSG_USB_WWID_DP)) ) {
    return TRUE;
  }

  return FALSE;
}

/**
  Connect the USB short form device path.

  @param DevicePath   USB short form device path

  @retval EFI_SUCCESS           Successfully connected the USB device
  @retval EFI_NOT_FOUND         Cannot connect the USB device
  @retval EFI_INVALID_PARAMETER The device path is invalid.
**/
EFI_STATUS
ConnectUsbShortFormDevicePath (
  IN EFI_DEVICE_PATH_PROTOCOL   *DevicePath
  )
{
  EFI_STATUS                            Status;
  EFI_HANDLE                            *Handles;
  UINTN                                 HandleCount;
  UINTN                                 Index;
  EFI_PCI_IO_PROTOCOL                   *PciIo;
  UINT8                                 Class[3];
  BOOLEAN                               AtLeastOneConnected;

  Handles = NULL;
  HandleCount = 0;
  //
  // Check the passed in parameters
  //
  if (DevicePath == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (!IsUsbShortForm (DevicePath)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Find the usb host controller firstly, then connect with the remaining device path
  //
  AtLeastOneConnected = FALSE;
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &Handles
                  );

  if ((EFI_ERROR (Status)) || (Handles == NULL) || (HandleCount == 0)) {
    return Status;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    Handles[Index],
                    &gEfiPciIoProtocolGuid,
                    (VOID **) &PciIo
                    );

    if (!EFI_ERROR (Status)) {
      //
      // Check whether the Pci device is the wanted usb host controller
      //
      Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint8, 0x09, 3, &Class);
      if (!EFI_ERROR (Status) &&
          ((PCI_CLASS_SERIAL == Class[2]) && (PCI_CLASS_SERIAL_USB == Class[1]))
         ) {
        Status = gBS->ConnectController (
                        Handles[Index],
                        NULL,
                        DevicePath,
                        FALSE
                        );
        if (!EFI_ERROR(Status)) {
          AtLeastOneConnected = TRUE;
        }
      }
    }
  }
  FreePool (Handles);
  return AtLeastOneConnected ? EFI_SUCCESS : EFI_NOT_FOUND;
}

/**
  The function connects the trusted consoles and returns whether both,
  a trusted input and output consoles are present.

  @retval TRUE   The device can be trusted.
  @retval FALSE  The device cannot be trusted.

**/
BOOLEAN
ConnectTrustedConsole (
  VOID
  )
{
  BOOLEAN                      TrustedConOutPresent;
  BOOLEAN                      TrustedConInPresent;
  EFI_DEVICE_PATH_PROTOCOL     *Consoles;
  EFI_DEVICE_PATH_PROTOCOL     *TempDevicePath;
  EFI_DEVICE_PATH_PROTOCOL     *Instance;
  EFI_DEVICE_PATH_PROTOCOL     *Next;
  UINTN                        Size;
  UINTN                        Index;
  EFI_HANDLE                   Handle;
  EFI_STATUS                   Status;
  CHAR16                       *ConsoleVar[] = {L"ConIn", L"ConOut"};

  TrustedConOutPresent = FALSE;
  TrustedConInPresent  = FALSE;

  DumpDevicePath (L"TrustedConsoleIn", PcdGetPtr (PcdTrustedConsoleInputDevicePath));
  DumpDevicePath (L"TrustedConsoleOut", PcdGetPtr (PcdTrustedConsoleOutputDevicePath));

  for (Index = 0; Index < sizeof (ConsoleVar) / sizeof (ConsoleVar[0]); Index++) {

    GetEfiGlobalVariable2 (ConsoleVar[Index], (VOID **)&Consoles, NULL);

    TempDevicePath = Consoles;
    do {
      Instance = GetNextDevicePathInstance (&TempDevicePath, &Size);
      if (Instance == NULL) {
        break;
      }
      if (IsTrustedConsole (Index, Instance)) {
        //
        // Flag when either a input or output trusted console is present.
        //
        switch (Index) {
        case ConIn:
          TrustedConInPresent = TRUE;
          DEBUG ((DEBUG_INFO, "[TCG] PP: Trusted input console detected...\n"));
          break;
        case ConOut:
          DEBUG ((DEBUG_INFO, "[TCG] PP: Trusted output console detected...\n"));
          TrustedConOutPresent = TRUE;
          break;
        default:
          break;
        }

        if (IsUsbShortForm (Instance)) {
          ConnectUsbShortFormDevicePath (Instance);
        } else {
          for (Next = Instance; !IsDevicePathEnd (Next); Next = NextDevicePathNode (Next)) {
            if (DevicePathType (Next) == ACPI_DEVICE_PATH && DevicePathSubType (Next) == ACPI_ADR_DP) {
              break;
            } else if (DevicePathType (Next) == HARDWARE_DEVICE_PATH &&
                       DevicePathSubType (Next) == HW_CONTROLLER_DP &&
                       DevicePathType (NextDevicePathNode (Next)) == ACPI_DEVICE_PATH &&
                       DevicePathSubType (NextDevicePathNode (Next)) == ACPI_ADR_DP
                       ) {
              break;
            }
          }
          if (!IsDevicePathEnd (Next)) {
            SetDevicePathEndNode (Next);
            Status = EfiBootManagerConnectDevicePath (Instance, &Handle);
            if (!EFI_ERROR (Status)) {
              gBS->ConnectController (Handle, NULL, NULL, TRUE);
            }
          } else {
            Status = EfiBootManagerConnectDevicePath (Instance, NULL);
            if (EFI_ERROR (Status)) {
              DEBUG ((DEBUG_ERROR, "EfiBootManagerConnectDevicePath - %r\n", Status));
            }
          }
        }
      }
      FreePool (Instance);
    } while (TempDevicePath != NULL);

    if (Consoles != NULL) {
      FreePool (Consoles);
    }
  }

  if (TrustedConInPresent && TrustedConOutPresent) {
    AmtConsoleReady ();
  }

  return (TrustedConInPresent && TrustedConOutPresent);
}

/**
  The function connects the trusted consoles and then call the PP processing library interface.

  @param Device  Configured primary display device.
**/
VOID
ProcessTcgPp (
  IN EFI_DEVICE_PATH_PROTOCOL  *OutputConsoleDevice
  )
{
  BOOLEAN TrustedConsolesPresent = FALSE;
  gPPRequireUIConfirm |= Tcg2PhysicalPresenceLibNeedUserConfirm();

  if (gPPRequireUIConfirm) {
    DEBUG ((DEBUG_ERROR, "[TCG] PP: Physical presence UI confirmation required...\n"));
    TrustedConsolesPresent = ConnectTrustedConsole ();

    if (TrustedConsolesPresent == FALSE) {
      DEBUG ((DEBUG_ERROR, "[TCG] PP: No trusted display device present to perform UI confirmation, deferring TCG physical presence processing...\n"));
      return;
    }

    //
    // Verify the configured primary display is the trusted console output.
    //
    if (!IsTrustedConsole (ConOut, OutputConsoleDevice)) {
      DEBUG ((DEBUG_ERROR, "[TCG] PP: Primary display is not the configured trusted output console! Physical Presence is not supported in this configuration, deferring TCG physical presence processing...\n"));
      return;
    }
  }

  //
  // Process TCG Physical Presence request just after the trusted console is ready
  //
  TcgPhysicalPresenceLibProcessRequest ();
  Tcg2PhysicalPresenceLibProcessRequest (NULL); // TCG2 implementation
}

/**
  The function checks whether the given PciBusNumber an internal bus.

  @param[in]  PciRootBridge          A poiner to Pci root bridge list.
  @param[in]  PciRootBridgeCount     Number of the items in PciRootBridge list.
  @param[in]  PciBusNumber           PCI bus number to be checked.

  @retval TRUE          PciBusNumber is an internal bus.
  @retval FALSE         PciBusNumber is not an internal bus.

**/
BOOLEAN
IsInternalBus (
  IN PCI_ROOT_BRIDGE    *PciRootBridge,
  IN UINTN              PciRootBridgeCount,
  IN UINT64             PciBusNumber
  )
{
  UINTN                         Index;

  //
  //  The case when failing to get PciRootBridge info
  //
  if ((PciRootBridge == NULL) || (PciRootBridgeCount == 0)) {
    if (PciBusNumber == 0) {
      return TRUE;
    }

    return FALSE;
  }

  for (Index = 0; Index < PciRootBridgeCount; Index++) {
    if (PciBusNumber == PciRootBridge[Index].Bus.Base) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  The function connects the trusted storages.

**/
VOID
ConnectTrustedStorage (
  VOID
  )
{
  EFI_STATUS                    Status;
  UINTN                         Index;
  UINTN                         PciIoHandleCount;
  EFI_HANDLE                    *PciIoHandleBuffer;
  EFI_PCI_IO_PROTOCOL           *PciIo;
  UINT8                         ClassCode[3];
  UINTN                         Segment;
  UINTN                         Bus;
  UINTN                         Device;
  UINTN                         Function;
  EFI_EVENT                     BdsDeviceConnectEvent;
  PCI_ROOT_BRIDGE               *PciRootBridge;
  UINTN                         PciRootBridgeCount;

  PciRootBridge = PciHostBridgeGetRootBridges (&PciRootBridgeCount);

  //
  // Connect Nvm Express controller.
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &PciIoHandleCount,
                  &PciIoHandleBuffer
                  );
  if (!EFI_ERROR (Status) &&
      (PciIoHandleBuffer != NULL) &&
      (PciIoHandleCount != 0)) {
    //
    // Create event to notify the device is connecting
    //
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    InternalBdsEmptyCallbackFuntion,
                    NULL,
                    &gPsStorageDeviceConnectNotifyGuid,
                    &BdsDeviceConnectEvent
                    );
    ASSERT_EFI_ERROR (Status);
    if (!EFI_ERROR (Status)) {
      gBS->SignalEvent (BdsDeviceConnectEvent);
      gBS->CloseEvent (BdsDeviceConnectEvent);
    }

    for (Index = 0; Index < PciIoHandleCount; Index++) {
      Status = gBS->HandleProtocol (
                      PciIoHandleBuffer[Index],
                      &gEfiPciIoProtocolGuid,
                      (VOID **) &PciIo
                      );
      ASSERT_EFI_ERROR (Status);
      //
      // Now further check the PCI header: Base class (offset 0x0B) and Sub Class (offset 0x0A).
      // This controller should be a Nvm Express controller.
      //
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
      if ((ClassCode[0] == 0x02) &&
          (ClassCode[1] == 0x08) &&
          (ClassCode[2] == PCI_CLASS_MASS_STORAGE)) {
        gBS->ConnectController (PciIoHandleBuffer[Index], NULL, NULL, TRUE);
      }

      //
      // Connect integrated SATA controller
      //
      if (ClassCode[2] == PCI_CLASS_MASS_STORAGE &&
          (ClassCode[1] == PCI_CLASS_MASS_STORAGE_SATADPA ||
           ClassCode[1] == PCI_CLASS_MASS_STORAGE_RAID)) {
        PciIo->GetLocation (PciIo, &Segment, &Bus, &Device, &Function);
        if (IsInternalBus (PciRootBridge, PciRootBridgeCount, Bus)) {
          gBS->ConnectController (PciIoHandleBuffer[Index], NULL, NULL, TRUE);
        }
      }
    }

    gBS->FreePool (PciIoHandleBuffer);
  }

  if (PciRootBridge != NULL) {
    PciHostBridgeFreeRootBridges (PciRootBridge, PciRootBridgeCount);
    FreePool (PciRootBridge);
  }
}

BOOLEAN
IsMorBitSet (
  VOID
  )
{
  UINTN                     MorControl;
  EFI_STATUS                Status;
  UINTN                     DataSize;

  //
  // Check if the MOR bit is set.
  //
  DataSize = sizeof (MorControl);
  Status = gRT->GetVariable (
                  MEMORY_OVERWRITE_REQUEST_VARIABLE_NAME,
                  &gEfiMemoryOverwriteControlDataGuid,
                  NULL,
                  &DataSize,
                  &MorControl
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, " PlatformBootMangerLib: gEfiMemoryOverwriteControlDataGuid doesn't exist!!***\n"));
    MorControl = 0;
  } else {
    DEBUG ((DEBUG_INFO, " PlatformBootMangerLib: Get the gEfiMemoryOverwriteControlDataGuid = %x!!***\n", MorControl));
  }

  return (BOOLEAN) (MorControl & 0x01);
}

/**
  The function connects the trusted storage to perform TPerReset.
**/
VOID
ProcessTcgMor (
  VOID
  )
{
  if (IsMorBitSet ()) {
    ConnectTrustedConsole ();
    ConnectTrustedStorage ();
  }
}

/**
  The function support device password feature.

**/
VOID
SupportDevicePassword (
  VOID
  )
{
  if (mSystemConfiguration.DevicePasswordSupport) {
    ConnectTrustedConsole ();
    ConnectTrustedStorage ();
  }
}

/**
  Finds the first external VGA controller.

  @retval The handle to the first VGA controller that was found, or NULL if
          there is no VGA controllers.
**/
EFI_HANDLE
GetExternalVideoController (
  VOID
  )
{
  EFI_STATUS                    Status;
  UINTN                         HandleIndex;
  EFI_HANDLE                    *HandleBuffer;
  UINTN                         HandleCount;
  PCI_TYPE00                    Pci;
  EFI_PCI_IO_PROTOCOL           *PciIo;
  EFI_HANDLE                    VideoController;
  UINTN                         Segment;
  UINTN                         Bus;
  UINTN                         Device;
  UINTN                         Function;

  VideoController = NULL;
  //
  // Get all handles that which contain an instance of PCI_IO_PROTOCOL
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  //
  // For all PCI Devices
  //
  for (HandleIndex = 0; (HandleIndex < HandleCount) && (VideoController == NULL); HandleIndex++) {
    Status = gBS->HandleProtocol (HandleBuffer[HandleIndex], &gEfiPciIoProtocolGuid, (VOID **) &PciIo);
    if (EFI_ERROR (Status)) {
      continue;
    }
    //
    // Check if this device is a VGA Controller
    //
    Status = PciIo->Pci.Read (
                    PciIo,
                    EfiPciIoWidthUint32,
                    0,
                    sizeof (Pci) / sizeof (UINT32),
                    &Pci
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }
    Status = PciIo->GetLocation (PciIo, &Segment, &Bus, &Device, &Function);
    if (EFI_ERROR (Status)) {
      continue;
    }
    //
    // All external video controllers are located on bus != 0 on our systems. The only
    // video controller on bus == 0 is integrated video controller(IGD).
    //
    if (Bus != 0 && (IS_PCI_VGA (&Pci) || IS_PCI_OLD_VGA (&Pci))) {
      DEBUG ((DEBUG_INFO, "Found VGA\n"));
      VideoController = HandleBuffer[HandleIndex];
      break;
    }
  }
  FreePool (HandleBuffer);
  return VideoController;
}

#ifdef _MSC_VER
#pragma optimize("g", off)
#endif

/**
  Check if current BootCurrent variable is internal shell boot option.

  @retval  TRUE         BootCurrent is internal shell.
  @retval  FALSE        BootCurrent is not internal shell.
**/
BOOLEAN
BootCurrentIsInternalShell (
  VOID
  )
{
  UINTN                         VarSize;
  UINT16                        BootCurrent;
  CHAR16                        BootOptionName[16];
  UINT8                         *BootOption;
  UINT8                         *Ptr;
  BOOLEAN                       Result;
  EFI_STATUS                    Status;
  EFI_DEVICE_PATH_PROTOCOL      *TempDevicePath;
  EFI_DEVICE_PATH_PROTOCOL      *LastDeviceNode;
  EFI_GUID                      *GuidPoint;

  BootOption     = NULL;
  Result         = FALSE;

  //
  // Get BootCurrent variable
  //
  VarSize = sizeof (UINT16);
  Status = gRT->GetVariable (
                  L"BootCurrent",
                  &gEfiGlobalVariableGuid,
                  NULL,
                  &VarSize,
                  &BootCurrent
                  );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  //
  // Create boot option Bootxxxx from BootCurrent
  //
  UnicodeSPrint (BootOptionName, sizeof(BootOptionName), L"Boot%04X", BootCurrent);

  GetEfiGlobalVariable2 (BootOptionName, (VOID **) &BootOption, &VarSize);
  if (BootOption == NULL || VarSize == 0) {
    return FALSE;
  }

  Ptr = BootOption;
  Ptr += sizeof (UINT32);
  Ptr += sizeof (UINT16);
  Ptr += StrSize ((CHAR16 *) Ptr);
  TempDevicePath = (EFI_DEVICE_PATH_PROTOCOL *) Ptr;
  LastDeviceNode = TempDevicePath;
  while (!IsDevicePathEnd (TempDevicePath)) {
    LastDeviceNode = TempDevicePath;
    TempDevicePath = NextDevicePathNode (TempDevicePath);
  }
  GuidPoint = EfiGetNameGuidFromFwVolDevicePathNode (
                (MEDIA_FW_VOL_FILEPATH_DEVICE_PATH *) LastDeviceNode
                );
  if ((GuidPoint != NULL) && (CompareGuid (GuidPoint, &gUefiShellFileGuid))) {
    //
    // if this option is internal shell, return TRUE
    //
    Result = TRUE;
  }

  if (BootOption != NULL) {
    FreePool (BootOption);
    BootOption = NULL;
  }

  return Result;
}

/**
  This function will change video resolution and text mode
  for internal shell when internal shell is launched.

  @param   None.

  @retval  EFI_SUCCESS  Mode is changed successfully.
  @retval  Others       Mode failed to changed.
**/
EFI_STATUS
EFIAPI
ChangeModeForInternalShell (
  VOID
  )
{
  EFI_GRAPHICS_OUTPUT_PROTOCOL          *GraphicsOutput;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL       *SimpleTextOut;
  UINTN                                 SizeOfInfo;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  *Info;
  UINT32                                MaxGopMode;
  UINT32                                MaxTextMode;
  UINT32                                ModeNumber;
  UINTN                                 HandleCount;
  EFI_HANDLE                            *HandleBuffer;
  EFI_STATUS                            Status;
  UINTN                                 Index;
  UINTN                                 CurrentColumn;
  UINTN                                 CurrentRow;
  UINT32                                ShellModeColumn;
  UINT32                                ShellModeRow;
  UINT32                                ShellHorizontalResolution;
  UINT32                                ShellVerticalResolution;

  ShellHorizontalResolution = PcdGet32(PcdSetupVideoHorizontalResolution);
  ShellVerticalResolution = PcdGet32(PcdSetupVideoVerticalResolution);
  ShellModeColumn = PcdGet32(PcdSetupConOutColumn);
  ShellModeRow = PcdGet32(PcdSetupConOutRow);

  Status = gBS->HandleProtocol (
                  gST->ConsoleOutHandle,
                  &gEfiGraphicsOutputProtocolGuid,
                  (VOID**)&GraphicsOutput
                  );
  if (EFI_ERROR (Status)) {
      Status = gBS->LocateProtocol (&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&GraphicsOutput);
      if (EFI_ERROR (Status)) {
          GraphicsOutput = NULL;
      }
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
    return EFI_UNSUPPORTED;
  }

  MaxGopMode  = GraphicsOutput->Mode->MaxMode;
  MaxTextMode = SimpleTextOut->Mode->MaxMode;

  //
  // 1. If current video resolution is same with new video resolution,
  //    video resolution need not be changed.
  //    1.1. If current text mode is same with new text mode, text mode need not be change.
  //    1.2. If current text mode is different with new text mode, text mode need be change to new text mode.
  // 2. If current video resolution is different with new video resolution, we need restart whole console drivers.
  //
  for (ModeNumber = 0; ModeNumber < MaxGopMode; ModeNumber++) {
    Status = GraphicsOutput->QueryMode (
                       GraphicsOutput,
                       ModeNumber,
                       &SizeOfInfo,
                       &Info
                       );
    if (!EFI_ERROR (Status)) {
      if ((Info->HorizontalResolution == ShellHorizontalResolution) &&
          (Info->VerticalResolution == ShellVerticalResolution)) {
        if ((GraphicsOutput->Mode->Info->HorizontalResolution == ShellHorizontalResolution) &&
            (GraphicsOutput->Mode->Info->VerticalResolution == ShellVerticalResolution)) {
          //
          // If current video resolution is same with new resolution,
          // then check if current text mode is same with new text mode.
          //
          Status = SimpleTextOut->QueryMode (SimpleTextOut, SimpleTextOut->Mode->Mode, &CurrentColumn, &CurrentRow);
          ASSERT_EFI_ERROR (Status);
          if (CurrentColumn == ShellModeColumn && CurrentRow == ShellModeRow) {
            //
            // Current text mode is same with new text mode, text mode need not be change.
            //
            FreePool (Info);
            return EFI_SUCCESS;
          } else {
            //
            // Current text mode is different with new text mode, text mode need be change to new text mode.
            //
            for (Index = 0; Index < MaxTextMode; Index++) {
              Status = SimpleTextOut->QueryMode (SimpleTextOut, Index, &CurrentColumn, &CurrentRow);
              if (!EFI_ERROR(Status)) {
                if ((CurrentColumn == ShellModeColumn) && (CurrentRow == ShellModeRow)) {
                  //
                  // New text mode is supported, set it.
                  //
                  Status = SimpleTextOut->SetMode (SimpleTextOut, Index);
                  ASSERT_EFI_ERROR (Status);
                  //
                  // Update text mode PCD.
                  //
                  PcdSet32S (PcdConOutColumn, ShellModeColumn);
                  PcdSet32S (PcdConOutRow, ShellModeRow);
                  FreePool (Info);
                  return EFI_SUCCESS;
                }
              }
            }
            if (Index == MaxTextMode) {
              //
              // If new text mode is not supported, return error.
              //
              FreePool (Info);
              return EFI_UNSUPPORTED;
            }
          }
        } else {
          FreePool (Info);
          //
          // If current video resolution is not same with the new one, set new video resolution.
          // In this case, the driver which produces simple text out need be restarted.
          //
          Status = GraphicsOutput->SetMode (GraphicsOutput, ModeNumber);
          if (!EFI_ERROR (Status)) {
            //
            // Set PCD to restart GraphicsConsole and Consplitter to change video resolution
            // and produce new text mode based on new resolution.
            //
            PcdSet32S (PcdVideoHorizontalResolution, ShellHorizontalResolution);
            PcdSet32S (PcdVideoVerticalResolution, ShellVerticalResolution);
            PcdSet32S (PcdConOutColumn, ShellModeColumn);
            PcdSet32S (PcdConOutRow, ShellModeRow);

            Status = gBS->LocateHandleBuffer (
                             ByProtocol,
                             &gEfiSimpleTextOutProtocolGuid,
                             NULL,
                             &HandleCount,
                             &HandleBuffer
                             );
            if (!EFI_ERROR (Status)) {
              for (Index = 0; Index < HandleCount; Index++) {
                gBS->DisconnectController (HandleBuffer[Index], NULL, NULL);
              }
              for (Index = 0; Index < HandleCount; Index++) {
                gBS->ConnectController (HandleBuffer[Index], NULL, NULL, TRUE);
              }
              if (HandleBuffer != NULL) {
                FreePool (HandleBuffer);
              }
              break;
            }
          }
        }
      }
      FreePool (Info);
    }
  }

  if (ModeNumber == MaxGopMode) {
    //
    // If the new resolution is not supported, return error.
    //
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  Connect on board USB controller.

  @retval None.
**/
VOID
ConnectUsbController (
  VOID
  )
{
  EFI_STATUS               Status;
  EFI_HANDLE               DeviceHandle;

  Status = EfiBootManagerConnectDevicePath ((EFI_DEVICE_PATH_PROTOCOL *) &gPlatformOnboardXhciDevice, &DeviceHandle);

  if (!EFI_ERROR (Status)) {
    Status = gBS->ConnectController (DeviceHandle, NULL, NULL, TRUE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "ConnectController (OnBoardUsb) - %r\n", Status));
      return;
    }
  }

  gDS->Dispatch ();
  return;
}

/**
  Connect on TCSS XHCI controller.

  @retval None.
**/
VOID
ConnectTcssXhciController (
  VOID
  )
{
  EFI_STATUS               Status;
  EFI_HANDLE               DeviceHandle;

  Status = EfiBootManagerConnectDevicePath ((EFI_DEVICE_PATH_PROTOCOL *) &gPlatformTcssXhciDevice, &DeviceHandle);

  if (!EFI_ERROR (Status)) {
    Status = gBS->ConnectController (DeviceHandle, NULL, NULL, TRUE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "ConnectController (TCSS XHCI) - %r\n", Status));
      return;
    }
  }

  gDS->Dispatch ();
}

/**
  Delete progress bar and Function key prompts from the top and bottom of the screen
  before the OS boot loader gets invoked.

  @retval None
**/
VOID
EFIAPI
ClearBootProgressBar (
  VOID
  )
{
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL         Black = { 0, 0, 0, 0 };
  EFI_GRAPHICS_OUTPUT_PROTOCOL          *GraphicsOutput;
  EFI_STATUS                            Status;
  UINT32                                DisplayWidth;
  UINT32                                DisplayHeight;
  UINT32                                UpperBandHeight;

  GraphicsOutput = NULL;
  DisplayWidth   = 0;
  DisplayHeight  = 0;
  UpperBandHeight = 150;

  Status = gBS->HandleProtocol (
                  gST->ConsoleOutHandle,
                  &gEfiGraphicsOutputProtocolGuid,
                  (VOID**)&GraphicsOutput
                  );
  if (EFI_ERROR (Status)) {
    GraphicsOutput = NULL;
  }

  if (GraphicsOutput != NULL) {
    DisplayWidth  = GraphicsOutput->Mode->Info->HorizontalResolution;
    DisplayHeight = GraphicsOutput->Mode->Info->VerticalResolution;

    ///
    /// Keep Upper Band cut-off screen Height lesser for FHD and lower resolution panels.
    ///
    if (DisplayWidth <= 1920) {
      UpperBandHeight = 75;
    }
    /// Delete contents on the upper band of the screen
    ///
    Status = GraphicsOutput->Blt (
                              GraphicsOutput,
                              &Black,
                              EfiBltVideoFill,
                              0,
                              0,
                              0,    ///< X
                              0,    ///< Y
                              DisplayWidth,
                              UpperBandHeight,  ///< Height
                              0
                              );
    if (EFI_ERROR (Status)) {
      return;
    }
    ///
    /// Delete contents on the lower band of the screen
    ///
    Status = GraphicsOutput->Blt (
                              GraphicsOutput,
                              &Black,
                              EfiBltVideoFill,
                              0,
                              0,
                              0,                  ///< X
                              DisplayHeight - 75, ///< Y
                              DisplayWidth,       ///< Width
                              75,                 ///< Height
                              0
                              );
  }
}

/**
  Draw the color code for this BIOS

  @retval None
**/
VOID
EFIAPI
DrawBiosColorCode (
  VOID
  )
{
  BIOS_ID_IMAGE                         BiosIdImage;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL         DrawColor = { 0, 0, 0, 0 };
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL         Black     = { 0, 0, 0, 0 };
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL         *TempBitmap;
  EFI_GRAPHICS_OUTPUT_PROTOCOL          *GraphicsOutput;
  EFI_STATUS                            Status;
  UINTN                                 DrawAttributes;
  UINTN                                 OriginalAttributes;
  UINTN                                 Columns;
  UINTN                                 Rows;
  UINTN                                 TextLength;
  UINTN                                 LeftPadding;
  UINTN                                 RightPadding;
  UINTN                                 Index;
  UINT32                                DisplayWidth;
  UINT32                                X;
  UINT32                                Y;
  UINT32                                OriginalCursorRow;
  UINT32                                OriginalCursorColumn;
  CHAR16                                ColorCode;
  BOOLEAN                               DrawTextBar;
  BOOLEAN                               DrawColorBar;
  CHAR16                                *DrawText;

  DrawText       = L"";
  DrawTextBar    = FALSE;
  DrawColorBar   = FALSE;
  DrawAttributes = EFI_TEXT_ATTR (EFI_LIGHTGRAY, EFI_BLACK);

  Status = GetBiosId (&BiosIdImage);
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Determine which color to draw
  //
  ColorCode = BiosIdImage.BiosIdString.VersionMajor[0];
  switch (ColorCode) {
    case 'X':
    case 'x':
      DrawTextBar     = TRUE;
      DrawText        = L"TEST BIOS - NO BUGs UNTIL REPRO ON BLUE BUILD";
      DrawAttributes  = EFI_TEXT_ATTR (EFI_WHITE, EFI_BLACK);
      DrawColorBar    = TRUE;
      DrawColor.Blue  = 4;
      DrawColor.Green = 66;
      DrawColor.Red   = 132;
      break;
    case 'Y':
    case 'y':
      DrawTextBar     = TRUE;
      DrawText        = L"PASSED DAILY AUTOMATED TESTING - NO BUGs UNTIL REPRO ON BLUE BUILD";
      DrawAttributes  = EFI_TEXT_ATTR (EFI_WHITE, EFI_BLACK);
      DrawColorBar    = TRUE;
      DrawColor.Blue  = 0;
      DrawColor.Green = 165;
      DrawColor.Red   = 255;
      break;
    case 'P':
    case 'p':
      DrawTextBar     = TRUE;
      DrawText        = L"ON DEMAND BIOS BUILD - NO BUGs UNTIL REPRO ON BLUE BUILD";
      DrawAttributes  = EFI_TEXT_ATTR (EFI_WHITE, EFI_BLACK);
      DrawColorBar    = TRUE;
      DrawColor.Blue  = 128;
      DrawColor.Green = 0;
      DrawColor.Red   = 128;
      break;
    case 'B':
    case 'b':
      DrawTextBar     = TRUE;
      DrawText        = L"PASSED WEEKLY BAT";
      DrawAttributes  = EFI_TEXT_ATTR (EFI_WHITE, EFI_BLACK);
      DrawColorBar    = TRUE;
      DrawColor.Blue  = 197;
      DrawColor.Green = 113;
      DrawColor.Red   = 0;
      break;
    default:
      break;
  }

  //
  // Draw Text if needed
  //
  if (DrawTextBar) {
    Status = gST->ConOut->QueryMode (
                            gST->ConOut,
                            gST->ConOut->Mode->Mode,
                            &Columns,
                            &Rows
                            );
    if (EFI_ERROR (Status)) {
      return;
    }
    OriginalCursorColumn = gST->ConOut->Mode->CursorColumn;
    OriginalCursorRow    = gST->ConOut->Mode->CursorRow;
    Status = gST->ConOut->SetCursorPosition (gST->ConOut, 0, 0);
    if (EFI_ERROR (Status)) {
      return;
    }
    //
    // Set attributes to draw the colored bar
    //
    OriginalAttributes = gST->ConOut->Mode->Attribute;
    Status             = gST->ConOut->SetAttribute (gST->ConOut, DrawAttributes);
    if (EFI_ERROR (Status)) {
      gST->ConOut->SetCursorPosition (
                    gST->ConOut,
                    OriginalCursorColumn,
                    OriginalCursorRow
                    );
      return;
    }
    //
    // Draw the left padding on the text
    //
    TextLength   = StrLen (DrawText);
    LeftPadding  = (Columns / 2) - (TextLength / 2);
    RightPadding = Columns - TextLength - LeftPadding;
    for (Index = 0; Index < LeftPadding; Index++) {
      Print (L" ");
    }
    //
    // Draw the text itself
    //
    Print (DrawText);
    //
    // Draw the right padding
    //
    for (Index = 0; Index < RightPadding; Index++) {
      Print (L" ");
    }
    //
    // Restore the original attributes and cursor position
    //
    gST->ConOut->SetAttribute (gST->ConOut, OriginalAttributes);
    gST->ConOut->SetCursorPosition (
                   gST->ConOut,
                   OriginalCursorColumn,
                   OriginalCursorRow
                   );
  }

  //
  // Draw Color Bar if needed
  //
  if (DrawColorBar) {
    GraphicsOutput = NULL;
    DisplayWidth   = 0;

    Status = gBS->HandleProtocol (
                    gST->ConsoleOutHandle,
                    &gEfiGraphicsOutputProtocolGuid,
                    (VOID**)&GraphicsOutput
                    );
    if (EFI_ERROR (Status)) {
      GraphicsOutput = NULL;
    }
    if (GraphicsOutput != NULL) {
      DisplayWidth  = GraphicsOutput->Mode->Info->HorizontalResolution;
      //
      // If we drew text earlier, then we need to copy the text to a bitmap,
      // add the desired background color and copy the result back to the framebuffer
      // If we didn't draw text, then we can just draw the background directly
      //
      TempBitmap    = AllocatePool (
                        sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL) * (DisplayWidth * BIOS_COLOR_CODING_BAR_HEIGHT)
                        );
      if (TempBitmap == NULL) {
        return;
      }
      Status        = GraphicsOutput->Blt (
                                        GraphicsOutput,
                                        TempBitmap,
                                        EfiBltVideoToBltBuffer,
                                        0,
                                        0,
                                        0,
                                        0,
                                        DisplayWidth,
                                        BIOS_COLOR_CODING_BAR_HEIGHT,
                                        sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL) * DisplayWidth
                                        );
      if (EFI_ERROR (Status)) {
        FreePool (TempBitmap);
        return;
      }
      for (Y = 0; Y < BIOS_COLOR_CODING_BAR_HEIGHT; Y++) {
        for (X = 0; X < DisplayWidth; X++) {
          if (CompareMem (& (TempBitmap[X + (Y * DisplayWidth)]), & (Black), sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)) == 0) {
            TempBitmap[X + (Y * DisplayWidth)] = DrawColor;
          }
        }
      }
      Status        = GraphicsOutput->Blt (
                                        GraphicsOutput,
                                        TempBitmap,
                                        EfiBltBufferToVideo,
                                        0,
                                        0,
                                        0,
                                        0,
                                        DisplayWidth,
                                        BIOS_COLOR_CODING_BAR_HEIGHT,
                                        sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL) * DisplayWidth
                                        );
      FreePool (TempBitmap);
      if (EFI_ERROR (Status)) {
        return;
      }
    }
  }
}

/**
  Function to change the Display Resolution to 1024x768 for KVM session.
**/
VOID
EFIAPI
ChangeResolutionForKvm (
  VOID
  )
{
  EFI_STATUS                            Status;
  EFI_GRAPHICS_OUTPUT_PROTOCOL          *GraphicsOutput;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  *Info;
  UINTN                                 SizeOfInfo;
  UINT32                                MaxGopMode;
  UINT32                                ModeNumber;
  UINT32                                HorizontalResforKvm;
  UINT32                                VerticalResforKvm;
  UINTN                                 HandleCount;
  EFI_HANDLE                            *HandleBuffer;
  UINTN                                 Index;


  HorizontalResforKvm = 1024;
  VerticalResforKvm   = 768;

  Status = gBS->HandleProtocol (
                  gST->ConsoleOutHandle,
                  &gEfiGraphicsOutputProtocolGuid,
                  (VOID**)&GraphicsOutput
                  );
  if (EFI_ERROR (Status)) {
    GraphicsOutput = NULL;
  }

  if (GraphicsOutput == NULL) {
    return ;
  }

  MaxGopMode  = GraphicsOutput->Mode->MaxMode;

  for (ModeNumber = 0; ModeNumber < MaxGopMode; ModeNumber++) {
    Status = GraphicsOutput->QueryMode (GraphicsOutput, ModeNumber, &SizeOfInfo, &Info);
    if (!EFI_ERROR (Status)) {
      if ((Info->HorizontalResolution == HorizontalResforKvm) &&
          (Info->VerticalResolution == VerticalResforKvm)) {
        ///
        /// If Current resolution is not 1024x768, Set it.
        ///
        if ((GraphicsOutput->Mode->Info->HorizontalResolution != HorizontalResforKvm) &&
            (GraphicsOutput->Mode->Info->VerticalResolution != VerticalResforKvm)) {
          Status = GraphicsOutput->SetMode (GraphicsOutput, ModeNumber);
          if (EFI_ERROR (Status)) {
              return ;
          }
          //
          // Set PCD to Inform GraphicsConsole to change video resolution.
          // Set PCD to Inform Consplitter to change text mode.
          //
          Status = PcdSet32S (PcdVideoHorizontalResolution, HorizontalResforKvm);
          ASSERT_EFI_ERROR (Status);
          Status = PcdSet32S (PcdVideoVerticalResolution, VerticalResforKvm);
          ASSERT_EFI_ERROR (Status);

          //
          // Video mode is changed, so restart graphics console driver and higher level driver.
          // Reconnect graphics console driver and higher level driver.
          // Locate all the handles with GOP protocol and reconnect it.
          //
          Status = gBS->LocateHandleBuffer (
                          ByProtocol,
                          &gEfiSimpleTextOutProtocolGuid,
                          NULL,
                          &HandleCount,
                          &HandleBuffer
                          );
          if (!EFI_ERROR (Status)) {
            for (Index = 0; Index < HandleCount; Index++) {
              gBS->DisconnectController (HandleBuffer[Index], NULL, NULL);
            }
            for (Index = 0; Index < HandleCount; Index++) {
              gBS->ConnectController (HandleBuffer[Index], NULL, NULL, TRUE);
            }
            if (HandleBuffer != NULL) {
              FreePool (HandleBuffer);
            }
          }
        }
      }
    }
  }
}

/**
  This function will change video resolution and text mode
  for internal shell when internal shell is launched.
  @param  Event   Pointer to this event
  @param  Context Event handler private data
  @retval None.
**/
VOID
EFIAPI
NotifyChangeModeForInternalShell (
  IN  EFI_EVENT       Event,
  IN  VOID            *Context
  )
{
  EFI_STATUS               Status;
  VOID                     *Interface;

  Status = gBS->LocateProtocol (&gEfiGraphicsOutputProtocolGuid, NULL, &Interface);
  if (EFI_ERROR (Status)) {
    return;
  }

  ChangeModeForInternalShell ();

  gBS->CloseEvent (Event);
}

/**
  ReadyToBoot callback to set video and text mode for internal shell boot.
  That will not connect USB controller while CSM and FastBoot are disabled, we need to connect them
  before booting to Shell for showing USB devices in Shell.

  When FastBoot is enabled and Windows Console is the chosen Console behavior, input devices will not be connected
  by default. Hence, when booting to EFI shell, connecting input consoles are required.

  @param  Event   Pointer to this event
  @param  Context Event handler private data

  @retval None.
**/
VOID
EFIAPI
OnReadyToBootCallBack (
  IN  EFI_EVENT                 Event,
  IN  VOID                      *Context
  )
{
  VOID                         *MsgRegistration;
  EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput;
  EFI_STATUS                   Status;
  //
  // Initialize GraphicsOutput as NULL.
  //
  GraphicsOutput = NULL;

//  EFI_HANDLE  FwVolHandle;  @todo Align BdsPlatform (DxePlatformBootManagerLib) with the Minimum Platform FV map
#if FixedPcdGetBool(PcdCapsuleEnable) == 1
  EFI_HOB_GUID_TYPE        *GuidHob;

  GuidHob = NULL;
  GuidHob = GetFirstGuidHob (&gSysFwUpdateProgressGuid);
  if ((GuidHob != NULL) && \
      (((SYSTEM_FIRMWARE_UPDATE_PROGRESS *) GET_GUID_HOB_DATA (GuidHob))->Component == UpdatingBios)) {
    //
    //  We set this flag in order to skip ProcessFirmwareVolume
    //
    mDecompressFvUefiBoot = TRUE;
  }
#endif
  if (BootCurrentIsInternalShell ()) {
    if (!mDecompressFvUefiBoot) {
//  @todo Align BdsPlatform (DxePlatformBootManagerLib) with the Minimum Platform FV map
//        gDS->ProcessFirmwareVolume (
//          (VOID *) (UINTN)PcdGet32(PcdFlashFvUefiBootBase),
//          PcdGet32(PcdFlashFvUefiBootSize),
//          &FwVolHandle
//          );

//        gDS->Dispatch ();
      mDecompressFvUefiBoot = TRUE;
//        DEBUG ((DEBUG_INFO, "Current boot is shell, decompress FvUefiBoot\n"));
    }

    Status = gBS->HandleProtocol (
                    gST->ConsoleOutHandle,
                    &gEfiGraphicsOutputProtocolGuid,
                    (VOID**)&GraphicsOutput
                    );

    if (EFI_ERROR (Status)) {
        Status = gBS->LocateProtocol (&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&GraphicsOutput);
        if (EFI_ERROR (Status)) {
            GraphicsOutput = NULL;
            EfiCreateProtocolNotifyEvent (
              &gEfiGraphicsOutputProtocolGuid,
              TPL_CALLBACK,
              NotifyChangeModeForInternalShell,
              NULL,
              &MsgRegistration
              );
        }
    }
    if (GraphicsOutput != NULL) {
      ChangeModeForInternalShell ();
    }


    UpdatePlatformConsoleToConInVairable ();
    //
    // Connect all devices no matter fastboot enable or not while entering shell.
    //
    EfiBootManagerConnectAll ();

  } else {
    if (GetDisplayBootMode () == NORMAL_BOOT) {
      ClearBootProgressBar ();
      DrawBiosColorCode ();
    }
  }

  if (AmtIsKvmSessionEnabled ()) {
    DEBUG ((DEBUG_INFO, "KVM session is active, changing resolution to 1024x768 during Ready to boot call back.\n"));
    ChangeResolutionForKvm ();
  }
}

VOID
EFIAPI
UnloadDriver (
  IN EFI_GUID    *DriverGuid,
  IN EFI_STRING  String
  )
{
  EFI_STATUS                          Status;
  UINTN                               Index;
  UINTN                               DriverImageHandleCount;
  EFI_HANDLE                          *DriverImageHandleBuffer;
  EFI_LOADED_IMAGE_PROTOCOL           *LoadedImage;
  EFI_GUID                            *NameGuid;
  EFI_DEVICE_PATH_PROTOCOL            *TempDevicePath;
  EFI_DEVICE_PATH_PROTOCOL            *LastDeviceNode;

  DriverImageHandleCount = 0;
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiLoadedImageProtocolGuid,
                  NULL,
                  &DriverImageHandleCount,
                  &DriverImageHandleBuffer
                  );
  ASSERT_EFI_ERROR (Status);

  for (Index = 0; Index < DriverImageHandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    DriverImageHandleBuffer[Index],
                    &gEfiLoadedImageProtocolGuid,
                    (VOID *) &LoadedImage
                    );
    if (LoadedImage->FilePath == NULL) {
      continue;
    }

    TempDevicePath = LoadedImage->FilePath;
    LastDeviceNode = TempDevicePath;
    while (!IsDevicePathEnd (TempDevicePath)) {
      LastDeviceNode = TempDevicePath;
      TempDevicePath = NextDevicePathNode (TempDevicePath);
    }
    NameGuid = EfiGetNameGuidFromFwVolDevicePathNode (
                 (MEDIA_FW_VOL_FILEPATH_DEVICE_PATH *) LastDeviceNode
                 );
    if ((NameGuid != NULL) && (CompareGuid (NameGuid, DriverGuid))) {
      Status = gBS->UnloadImage (DriverImageHandleBuffer[Index]);
      ASSERT_EFI_ERROR (Status);
      DEBUG ((DEBUG_ERROR, "Unload %s\n", String));
      break;
    }
  }
  if (DriverImageHandleBuffer != NULL) {
    FreePool (DriverImageHandleBuffer);
  }
}

/**
  Password authentication handler to check password
  before entering setup.

  @param  CodeType      Indicates the type of status code being reported.  Type EFI_STATUS_CODE_TYPE is defined in "Related Definitions" below.

  @param  Value         Describes the current status of a hardware or software entity.
                        This included information about the class and subclass that is used to classify the entity
                        as well as an operation.  For progress codes, the operation is the current activity.
                        For error codes, it is the exception.  For debug codes, it is not defined at this time.
                        Type EFI_STATUS_CODE_VALUE is defined in "Related Definitions" below.
                        Specific values are discussed in the Intel? Platform Innovation Framework for EFI Status Code Specification.

  @param  Instance      The enumeration of a hardware or software entity within the system.
                        A system may contain multiple entities that match a class/subclass pairing.
                        The instance differentiates between them.  An instance of 0 indicates that instance information is unavailable,
                        not meaningful, or not relevant.  Valid instance numbers start with 1.


  @param  CallerId      This optional parameter may be used to identify the caller.
                        This parameter allows the status code driver to apply different rules to different callers.
                        Type EFI_GUID is defined in InstallProtocolInterface() in the UEFI 2.0 Specification.


  @param  Data          This optional parameter may be used to pass additional data

  @retval EFI_SUCCESS             Status code is what we expected.
  @retval EFI_UNSUPPORTED         Status code not supported.

**/
EFI_STATUS
EFIAPI
PasswordAuthenticationHandler (
  IN EFI_STATUS_CODE_TYPE     CodeType,
  IN EFI_STATUS_CODE_VALUE    Value,
  IN UINT32                   Instance,
  IN EFI_GUID                 *CallerId, OPTIONAL
  IN EFI_STATUS_CODE_DATA     *Data      OPTIONAL
  )
{
  if (((CodeType & EFI_STATUS_CODE_TYPE_MASK) == EFI_PROGRESS_CODE ) &&
       (Value == (EFI_SOFTWARE_DXE_BS_DRIVER | EFI_SW_PC_USER_SETUP))) {
    return UiDoPasswordAuthentication ();
  } else {
    return EFI_UNSUPPORTED;
  }
}


/**
  Unregister password authentication handler.

  @param  Event         Event whose notification function is being invoked.
  @param  Context       Pointer to the notification function's context, which is
                        always zero in current implementation.

**/
VOID
EFIAPI
UnregisterPasswordAuthenticationHandler (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  mRscHandlerProtocol->Unregister (PasswordAuthenticationHandler);
}

/**
  Register password authentication handler to check password
  before entering setup.

**/
VOID
RegisterPasswordAuthenticationHandler (
  )
{
  EFI_STATUS          Status;
  EFI_EVENT           ExitBootServicesEvent;
  EFI_PASSWORD_STATUS PwdData;
  SYSTEM_ACCESS       SystemData;

  //
  // Locate report status code protocol.
  //
  Status = gBS->LocateProtocol (
                  &gEfiRscHandlerProtocolGuid,
                  NULL,
                  (VOID **) &mRscHandlerProtocol
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Register the callback function for ReportStatusCode() notification.
  //
  mRscHandlerProtocol->Register (PasswordAuthenticationHandler, TPL_HIGH_LEVEL);

  //
  // Unregister boot time report status code listener at ExitBootService Event.
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  UnregisterPasswordAuthenticationHandler,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &ExitBootServicesEvent
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Set NeedReVerify policy to FALSE.
  //
  Status = UiSetPasswordVerificationPolicy (FALSE);
  ASSERT_EFI_ERROR (Status);

  //
  // Check if password is set
  //
  ZeroMem (&PwdData, sizeof(PwdData));
  if(IsPasswordInstalled ()) {
    PwdData.AdminName = ADMIN_PW_SET;
  } else {
    PwdData.AdminName = ADMIN_PW_CLEAR;
  }

  //
  // Update password status variable
  //
  Status = gRT->SetVariable (
                  L"PasswordStatus",
                  &gSystemAccessGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                  sizeof (PwdData),
                  &PwdData
                );

  //
  // No password installed is same as admin access since only admin password is supported
  //
  ZeroMem (&SystemData, sizeof(SystemData));
  SystemData.Access = SYSTEM_PASSWORD_ADMIN;
  Status = gRT->SetVariable (
                  L"SystemAccess",
                  &gSystemAccessGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                  sizeof (SystemData),
                  &SystemData
                );
}

/**
  Update Serial In/Out Uart Console Environment Variable
**/

VOID
SerialIoUartUpdateConsoleVariable (
  VOID
  )
{
  EFI_STATUS                      Status;
  EFI_DEVICE_PATH_PROTOCOL        *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL        *EndPath;
  UINTN                           HandleIndex;
  EFI_HANDLE                      *HandleBuffer;
  UINTN                           HandleCount;
  EFI_HANDLE                      UartController;

  DEBUG ((DEBUG_INFO, "SerialIoUartUpdateConsoleVariable\n"));

  UartController = NULL;

  //
  // Get all handles which contain an instance of EFI_DEVICE_PATH_PROTOCOL
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiDevicePathProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  for (HandleIndex = 0; (HandleIndex < HandleCount) && (UartController == NULL); HandleIndex++) {
    DevicePath = DevicePathFromHandle (HandleBuffer[HandleIndex]);
    if (DevicePath == NULL) {
      continue;
    }
    EndPath = DevicePath;
    while (!IsDevicePathEnd (EndPath)) {
      DevicePath = EndPath;
      EndPath    = NextDevicePathNode (EndPath);
    }

    if (DevicePath->Type == ACPI_DEVICE_PATH && DevicePath->SubType == ACPI_EXTENDED_DP) {
      if (AsciiStrnCmp ("UART", (CHAR8*)( (ACPI_EXTENDED_HID_DEVICE_PATH*)DevicePath + 1), 4) == 0) {
        UartController = HandleBuffer[HandleIndex];
        //
        // Found UART Controller
        // Connect to create new handle with appended Device Path and Serial IO Protocol support
        //
        gBS->ConnectController (UartController, NULL, NULL, TRUE);
      }
    }
  }

  // @todo add support for more controllers
  //
  // No Uart devices found
  //
  if (UartController == NULL) {
    return;
  }

  //
  // Locate New Handle for previously created (...)/Uart(DEFAULT,DEFAULT,D,D)
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiDevicePathProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Connect new handle to create another Device Path with PcAnsi SimpleTextIn/Out support
  //
  gBS->ConnectController (HandleBuffer[HandleCount-1], NULL, NULL, TRUE);

  //
  // Locate new Handle for /VenPcAnsi
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiDevicePathProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  DevicePath = DevicePathFromHandle (HandleBuffer[HandleCount-1]);

  //
  // Update Console In/Out Dev Setup Variable - Enable by default
  //
  EfiBootManagerUpdateConsoleVariable (ConInDev, DevicePath, NULL);
  EfiBootManagerUpdateConsoleVariable (ConOutDev, DevicePath, NULL);
  //
  // Update Console In/Out Environment Variable.
  //
  EfiBootManagerUpdateConsoleVariable (ConIn, DevicePath, NULL);
  EfiBootManagerUpdateConsoleVariable (ConOut, DevicePath, NULL);
  FreePool (HandleBuffer);
}


/**
   This function will delete additional igfx's device path in ConOut variable when external video controller
   was set as primary display and internal graphics was forced to enable.
   @param VarConOut    A pointer to ConOut variable.
   @param IgfxDp       A pointer to igfx device path.
**/
STATIC
VOID
CheckPrimaryDisplay (
  IN EFI_DEVICE_PATH_PROTOCOL  *VarConOut,
  IN EFI_DEVICE_PATH_PROTOCOL  *IgfxDp
  )
{
  UINTN                     IgfxDpSize;
  UINTN                     DevicePathInstSize;
  EFI_DEVICE_PATH_PROTOCOL  *VarConOutDp;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePathInst;
  EFI_STATUS                Status;

  DEBUG ((DEBUG_ERROR, "%a Start\n", __FUNCTION__));

  VarConOutDp    = VarConOut;
  DevicePathInst = GetNextDevicePathInstance (&VarConOutDp, &DevicePathInstSize);
  IgfxDpSize     = GetDevicePathSize (IgfxDp);
  //
  // Abandon END device path node.
  //
  IgfxDpSize    -= sizeof (EFI_DEVICE_PATH_PROTOCOL);

  //
  // Go through ConOut with loop.
  //
  while (DevicePathInst != NULL) {
    //
    // Compare each device path instance in ConOut variable with igfx device path.
    //
    if (CompareMem (IgfxDp, DevicePathInst, IgfxDpSize) == 0) {
      //
      // Delete igfx device path instance in ConOut.
      //
      Status = EfiBootManagerUpdateConsoleVariable (ConOut, NULL, DevicePathInst);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "EfiBootManagerUpdateConsoleVariable() with status: %r\n", Status));
        return;
      }

      break;
    }

    FreePool (DevicePathInst);
    DevicePathInst = GetNextDevicePathInstance (&VarConOutDp, &DevicePathInstSize);
  }

  if (DevicePathInst != NULL) {
    FreePool (DevicePathInst);
  }

  DEBUG ((DEBUG_ERROR, "%a End\n", __FUNCTION__));
}


/**
  Platform Bds init. Include the platform firmware vendor, revision
  and so crc check.
**/
VOID
EFIAPI
PlatformBootManagerBeforeConsole (
  VOID
  )
{
  EFI_STATUS                          Status;
  EFI_DEVICE_PATH_PROTOCOL            *VarConOut;
  EFI_DEVICE_PATH_PROTOCOL            *VarConIn;
  EFI_DEVICE_PATH_PROTOCOL            *TempDevicePath;
  EFI_DEVICE_PATH_PROTOCOL            *Instance;
  EFI_DEVICE_PATH_PROTOCOL            *Next;
  EFI_HANDLE                          VideoHandle;
  BOOLEAN                             IsIgd;
  EFI_EVENT                           Event;
  EFI_GUID                            *TerminalGuid;
  UINT8                               TerminalType;
  UINTN                               InstanceSize;
  CNV_VFR_CONFIG_SETUP                CnvSetup;
  EFI_HANDLE                          ConsoleHandle;
  UINTN                               DataSize;
  ATTEMPT_USB_FIRST_HOTKEY_INFO       *pUsbFirstInfoHob;
  ATTEMPT_USB_FIRST_HOTKEY_INFO       AttemptUsbFirstHotkeyInfo;
  ATTEMPT_USB_FIRST_VARIABLE          AttemptUsbFirstEfiVarInfo;
  ATTEMPT_USB_FIRST_RUNTIME_VARIABLE  AttemptUsbFirstRuntimeVarInfo;
  EFI_HOB_GUID_TYPE                   *GuidHob;
  PCH_SETUP                           PchSetup;
  ESRT_MANAGEMENT_PROTOCOL            *EsrtManagement;
  EFI_BOOT_MODE                       BootMode;
  EFI_EVENT                           BeforeEndOfDxeEvent;

  ZeroMem (
    &AttemptUsbFirstHotkeyInfo,
    sizeof (ATTEMPT_USB_FIRST_HOTKEY_INFO)
    );

  ZeroMem (
    &AttemptUsbFirstEfiVarInfo,
    sizeof (ATTEMPT_USB_FIRST_VARIABLE)
    );

  ZeroMem (
    &AttemptUsbFirstRuntimeVarInfo,
    sizeof (ATTEMPT_USB_FIRST_RUNTIME_VARIABLE)
    );

  Status = EFI_SUCCESS;

  //
  // Register password authentication handler to check password
  // before entering setup.
  //
  RegisterPasswordAuthenticationHandler ();

  //
  // Get Hotkey statue of AttemptUsbFirst from HOB
  //
  GuidHob = GetFirstGuidHob (&gAttemptUsbFirstHotkeyInfoHobGuid);
  if (GuidHob) {
    pUsbFirstInfoHob = (ATTEMPT_USB_FIRST_HOTKEY_INFO*) GET_GUID_HOB_DATA (GuidHob);
    CopyMem (&AttemptUsbFirstHotkeyInfo, pUsbFirstInfoHob, sizeof (ATTEMPT_USB_FIRST_HOTKEY_INFO));
  }
  //
  //Send StateAfterG3 Status to EC
  //
  DataSize = sizeof (PCH_SETUP);
  Status = gRT->GetVariable (
                  (CHAR16 *) L"PchSetup",
                  &gPchSetupVariableGuid,
                  (UINT32 *) NULL,
                  &DataSize,
                  &PchSetup
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  EcSetG3State (PchSetup.StateAfterG3);

  //
  // Get 'AttemptUSBFirst' EFI variable
  //
  DataSize = sizeof (ATTEMPT_USB_FIRST_VARIABLE);
  Status = gRT->GetVariable (
                  L"AttemptUSBFirst",
                  &gOsVendorGlobalVariableGuid,
                  NULL,
                  &DataSize,
                  &AttemptUsbFirstEfiVarInfo
                  );
  if (Status == EFI_NOT_FOUND) {
    Status = gRT->SetVariable (
                    L"AttemptUSBFirst",
                    &gOsVendorGlobalVariableGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                    DataSize,
                    &AttemptUsbFirstEfiVarInfo
                    );
  }

  //
  // Save status of AttemptUsbFirst to volatile variable
  //
  AttemptUsbFirstRuntimeVarInfo.UsbFirstEnable = AttemptUsbFirstHotkeyInfo.HotkeyTriggered | AttemptUsbFirstEfiVarInfo.UsbBootPrior;
  DataSize = sizeof (ATTEMPT_USB_FIRST_RUNTIME_VARIABLE);
  Status = gRT->SetVariable (
                  L"AttemptUSBFirstRuntime",
                  &gAttemptUsbFirstRuntimeVarInfoGuid,
                  EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                  DataSize,
                  &AttemptUsbFirstRuntimeVarInfo
                  );

  FastBootGenericVariablesInit ();

  //
  // Append Usb Keyboard short form DevicePath into "ConInDev"
  //
  EfiBootManagerUpdateConsoleVariable (
    ConInDev,
    (EFI_DEVICE_PATH_PROTOCOL *) &gUsbClassKeyboardDevicePath,
    NULL
    );

  //
  // Create event to set proper video resolution and text mode for internal shell.
  //
  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             OnReadyToBootCallBack,
             NULL,
             &Event
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Connect Root Bridge to make PCI BAR resource allocated and all PciIo created
  //
  PERF_START_EX(NULL,"EventRec", NULL, AsmReadTsc(), 0x7010);
  ConnectRootBridge (FALSE);
  PERF_END_EX(NULL,"EventRec", NULL, AsmReadTsc(), 0x7011);

  //
  // Update on board XHCI device path to connect the correct controller.
  //
  UpdateOnboardXhciDevicePath ();

  if (AmtIsKvmSessionEnabled ()) {
    DEBUG ((DEBUG_INFO, "KVM session is active, changing resolution to 1024x768.\n"));
    PcdSet32S (PcdVideoHorizontalResolution, 1024);
    PcdSet32S (PcdVideoVerticalResolution, 768);
  }

  IsIgd  = FALSE;
  switch (mSaSetup.PrimaryDisplay) {

    case DISPLAY_AUTO:
      //
      // Add PCIe to ConOut if it exists
      //
      VideoHandle = GetExternalVideoController ();
      if (VideoHandle != NULL) {
        DEBUG ((DEBUG_INFO, "[PlatformBds] Video controller on PCIe port as primary display.\n"));
        break;
      }

      //
      // Falls to case 1 IGD case if PCH PCIe doesn't exist
      //
    case DISPLAY_IGD:
    case DISPLAY_HG:
      //
      // Add IGD to ConOut
      //

      TempDevicePath = (EFI_DEVICE_PATH_PROTOCOL *) &gPlatformIGDDevice;
      Status = gBS->LocateDevicePath (&gEfiPciIoProtocolGuid, &TempDevicePath, &VideoHandle);
      if (!EFI_ERROR (Status) &&
        IsDevicePathEnd (TempDevicePath) &&
        IsVgaHandle (VideoHandle)) {
        IsIgd = TRUE;
        DEBUG ((DEBUG_INFO, "[PlatformBds] IGD video controller as primary display.\n"));
        break;
      }


    default:
      VideoHandle = NULL;
      DEBUG ((DEBUG_ERROR, "[PlatformBds] No video controller!\n"));
      break;
  }

  if (VideoHandle != NULL) {
    if (IsIgd == TRUE) {
      //
      // Connect the GOP driver
      //
      gBS->ConnectController (VideoHandle, NULL, NULL, TRUE);
    }

    //
    // Update ConOut variable according to the PrimaryDisplay setting
    //
    TempDevicePath = EfiBootManagerGetGopDevicePath (VideoHandle);
    if (TempDevicePath != NULL) {
      EfiBootManagerUpdateConsoleVariable (ConOut, TempDevicePath, NULL);
      FreePool (TempDevicePath);
    }

    if (IsIgd == TRUE) {
      //
      // Necessary for ConPlatform and ConSplitter driver to start up again after ConOut is updated.
      //
      gBS->ConnectController (VideoHandle, NULL, NULL, TRUE);
    }
  }

  //
  // Add platform string package
  //
  InitializeStringSupport ();

  //
  // Fill ConIn/ConOut in Full Configuration boot mode
  //
  DEBUG ((DEBUG_INFO, "PlatformBootManagerInit - %x\n", gBootMode));

  if (gBootMode == BOOT_WITH_FULL_CONFIGURATION ||
      gBootMode == BOOT_WITH_DEFAULT_SETTINGS ||
      gBootMode == BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS ||
      gBootMode == BOOT_IN_RECOVERY_MODE) {

    GetEfiGlobalVariable2 (L"ConOut", (VOID **) &VarConOut, NULL);   if (VarConOut != NULL) { FreePool (VarConOut); }
    GetEfiGlobalVariable2 (L"ConIn", (VOID **) &VarConIn, NULL);    if (VarConIn  != NULL) { FreePool (VarConIn);  }

    if (VarConOut == NULL || VarConIn == NULL) {
      //
      // Only fill ConIn/ConOut when ConIn/ConOut is empty because we may drop to Full Configuration boot mode in non-first boot
      //
      SerialIoUartUpdateConsoleVariable ();
      if (PcdGetBool(PcdComPortAttributes0IsEnabled) && (!FastBootEnabled ()
          )){
        //
        // Update Terminal Type.
        //
        TerminalGuid = &gEfiPcAnsiGuid;
        TerminalType = PcdGet8 (PcdDefaultTerminalType);
        if (TerminalType < 4) {
          TerminalGuid = mLibTerminalType[TerminalType];
        }
        CopyMem (&gSerialDevicePath.TerminalType.Guid, TerminalGuid, sizeof (EFI_GUID));
        EfiBootManagerUpdateConsoleVariable (ConIn, (EFI_DEVICE_PATH_PROTOCOL *)&gSerialDevicePath, NULL);
        EfiBootManagerUpdateConsoleVariable (ConOut, (EFI_DEVICE_PATH_PROTOCOL *)&gSerialDevicePath, NULL);
      }
      UpdatePlatformConsoleToConInVairable ();
      //
      // Back up ConIn varaible to ConInAct
      //
      InitConInActVariable ();
    }
    else {
      if (gBootMode == BOOT_WITH_DEFAULT_SETTINGS) {
        //
        // Get default Terminal Type.
        //
        TerminalGuid = &gEfiPcAnsiGuid;
        TerminalType = PcdGet8 (PcdDefaultTerminalType);
        if (TerminalType < 4) {
          TerminalGuid = mLibTerminalType[TerminalType];
        }

        GetEfiGlobalVariable2 (L"ConIn", (VOID **) &VarConIn, NULL);
        Instance      = GetNextDevicePathInstance (&VarConIn, &InstanceSize);
        InstanceSize -= END_DEVICE_PATH_LENGTH;

        while (Instance != NULL) {
          Next = Instance;
          while (!IsDevicePathEndType (Next)) {
            Next = NextDevicePathNode (Next);
            if (DevicePathType (Next) == MESSAGING_DEVICE_PATH && DevicePathSubType (Next) == MSG_VENDOR_DP) {
              //
              // Restoring default serial device path
              //
              EfiBootManagerUpdateConsoleVariable (ConIn, NULL, Instance);
              EfiBootManagerUpdateConsoleVariable (ConOut, NULL, Instance);

              if (PcdGetBool(PcdComPortAttributes0IsEnabled) && (!FastBootEnabled ()
                  )){
                CopyMem (&((VENDOR_DEVICE_PATH *)Next)->Guid, TerminalGuid, sizeof (EFI_GUID));
                EfiBootManagerUpdateConsoleVariable (ConIn, Instance, NULL);
                EfiBootManagerUpdateConsoleVariable (ConOut, Instance, NULL);
              }
            }
          }
          FreePool(Instance);
          Instance      = GetNextDevicePathInstance (&VarConIn, &InstanceSize);
          InstanceSize -= END_DEVICE_PATH_LENGTH;
        }
      }
    }
  }

//
// Provide Virtual UART support based on TerminalOverDebug setup option.
//
#if FixedPcdGetBool (PcdTerminalOverDebugEnable) == 1
  if (mSystemConfiguration.TerminalOverDebug == 1) {
    DEBUG ((DEBUG_ERROR, "TerminalOverDebug support is enabled.\n"));
    EfiBootManagerUpdateConsoleVariable (ConIn, (EFI_DEVICE_PATH_PROTOCOL *)&XhciDciDevicePath, NULL);
    EfiBootManagerUpdateConsoleVariable (ConOut, (EFI_DEVICE_PATH_PROTOCOL *)&XhciDciDevicePath, NULL);
  } else {
    DEBUG ((DEBUG_ERROR, "TerminalOverDebug support is disabled..\n"));
    //
    // Remove Virtual UART from ConIn Variable
    //
    EfiBootManagerUpdateConsoleVariable (ConIn, NULL, (EFI_DEVICE_PATH_PROTOCOL *) &XhciDciDevicePath);
    EfiBootManagerUpdateConsoleVariable (ConOut, NULL, (EFI_DEVICE_PATH_PROTOCOL *)&XhciDciDevicePath);
  }
#endif

#if FixedPcdGetBool (PcdVirtualKeyboardEnable) == 1
  if ((mSystemConfiguration.VirtualKeyboard == 1)) {  // If virtual keyboard is enabled add it to ConIn else unload the driver and remove from ConIn

    //
    // Add Virtual Keyboard to ConIn Variable
    //
    EfiBootManagerUpdateConsoleVariable (ConIn, (EFI_DEVICE_PATH_PROTOCOL *) &gVirtualKeyboardDevicePath, NULL);
  } else {
    DEBUG ((DEBUG_ERROR, "Virtual Keyboard is disabled.\n"));

    //
    // Remove Virtual Keyboard from ConIn Variable
    //
    EfiBootManagerUpdateConsoleVariable (ConIn, NULL, (EFI_DEVICE_PATH_PROTOCOL *) &gVirtualKeyboardDevicePath);

    //
    // Unload Virtual Keyboard driver
    //
    UnloadDriver (&gVirtualKeyboardDriverImageGuid, L"Virtual Keyboard");
  }
#endif

  if (PcdGetBool(PcdComPortAttributes0IsEnabled) && (!FastBootEnabled ()
      )){
    RaiseFastBootException (ExceptionType2, ConsoleDeviceChanged);
  } else {
    FastBootUpdateConsoleVariable ();
  }

  if (FastBootEnabled ()) {
    //
    // If ConIn device is changed, raise FastBoot exception.
    //
    if (EFI_ERROR (FastBootEnumConInDevice ())) {
      RaiseFastBootException (ExceptionType2, ConsoleDeviceChanged);
    }
  } else {
    EnumPs2Keyboard ();
    EnumUsbKeyboard ();
  }
  //
  // Handle AMT lock keyboard feature
  // Handle AMT SOL feature
  //
  AmtInitializePlatformForAmtSupport ();
  AmtUpdateConsoleVariable ();

  //
  // Unload EFI network driver if it is disabled in setup
  //
  if ((mSystemConfiguration.EfiNetworkSupport == 2) || (mSystemConfiguration.EfiNetworkSupport == 0)) {
    UnloadDriver (&gUndiDriverBinGuid, L"EFI Network Driver");
  }

  //
  // Unload WLAN driver if it is disabled in setup
  //
  if ((mSystemConfiguration.EfiNetworkSupport == 3) || (mSystemConfiguration.EfiNetworkSupport == 0)) {
    UnloadDriver (&gWlanDriverBinGuid, L"Intel WLAN Driver");
  }

  //
  // Unload BLE driver if it is disabled in setup
  //
  DataSize = sizeof (CNV_VFR_CONFIG_SETUP);
  Status = gRT->GetVariable (
                  L"CnvSetup",
                  &gCnvFeatureSetupGuid,
                  NULL,
                  &DataSize,
                  &CnvSetup
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }
  if (CnvSetup.PrebootBleEnable == 0) {
    UnloadDriver (&gBluetoothHciImageGuid, L"Intel BT Hci Driver");
  }

  //
  // This handler is to avoid both igfx and external video controller were set as primary display
  // when external video controller was choosed to be a primary display as well as Internal Graphics was forced to enabled.
  // This handler should be prior to any connect device path and connect controller routine.
  //
  if ((mSaSetup.InternalGraphics == 1) && (IsIgd == FALSE)) {
    Status = GetEfiGlobalVariable2 (L"ConOut", (VOID **)&VarConOut, NULL);
    if (!EFI_ERROR (Status)) {
      CheckPrimaryDisplay (VarConOut, (EFI_DEVICE_PATH_PROTOCOL *) &gPlatformIGDDevice);
      if (VarConOut != NULL) {
        FreePool (VarConOut);
      }
    }
  }

  if (FeaturePcdGet (PcdTpm2Enable) == TRUE) {
    //
    // If the MOR bit is set, connect SATA controller to make sure all hard disks have been connected.
    //
    ProcessTcgMor ();

    TempDevicePath = EfiBootManagerGetGopDevicePath (VideoHandle);
    //
    // Process TPM Physical Presence commands
    //
    ProcessTcgPp (TempDevicePath);
  }

  SupportDevicePassword ();

  //
  // Dynamically register hot key: Only F2 when AmtForceSetup, otherwise, F2/F7/Enter
  //
  RegisterDefaultBootOption ();
  RegisterStaticHotkey ();
  RegisterDynamicHotkey ();

  Status = gBS->LocateProtocol (&gEsrtManagementProtocolGuid, NULL, (VOID **) &EsrtManagement);
  if (EFI_ERROR (Status)) {
    EsrtManagement = NULL;
  }

  BootMode = GetBootModeHob ();
  switch (BootMode) {
  case BOOT_ON_FLASH_UPDATE:
    BootLogoEnableLogo ();
    PlatformDisablePowerButton (); // In case of accidental shutdown during FW update process.
    DEBUG ((DEBUG_INFO, "ProcessCapsules Before EndOfDxe ......\n"));

    //
    // Simply disarm wdt to avoid reboot during capsule update
    //
    Status = DisarmPlatformWdt ();
    DEBUG ((DEBUG_INFO, "Disarm ec wdt with status: %d\n", Status));

    //
    // ProcessCapsules only when there is a capsule hob detected.
    // Otherwise system will reset in ProcessCapsules() and ResiliencyCallBackFunction() would never be reached.
    //
    if (GetFirstHob (EFI_HOB_TYPE_UEFI_CAPSULE) != NULL) {

      PlatformDisablePowerButton (); // In case of accidental shutdown during FW update process.
      DEBUG ((DEBUG_INFO, "ProcessCapsules Before EndOfDxe ......\n"));

      //
      // Start to process Capsules in memory.
      //
      Status = ProcessCapsules ();
      DEBUG ((DEBUG_INFO, "ProcessCapsules %r\n", Status));
      PlatformEnablePowerButton ();

    }

    break;
  case BOOT_IN_RECOVERY_MODE:
    break;
  case BOOT_ASSUMING_NO_CONFIGURATION_CHANGES:
  case BOOT_WITH_MINIMAL_CONFIGURATION:
  case BOOT_ON_S4_RESUME:
    if (EsrtManagement != NULL) {
      //
      // Lock ESRT cache repository before EndofDxe if ESRT sync is not needed
      //
      EsrtManagement->LockEsrtRepository ();
    }
    break;
  default:
    //
    // Require to sync ESRT from FMP in a new boot
    //
    if (EsrtManagement != NULL) {
      EsrtManagement->SyncEsrtFmp ();
    }
    break;
  }

  if (GetBiosResiliencyType () != NOT_SUPPORT_RECOVERY) {
    DEBUG ((DEBUG_INFO, "Signal BeforeEndOfDxe Event to Handle Resiliency Syncup/Rollback\n"));
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    InternalBdsEmptyCallbackFuntion,
                    NULL,
                    &gPlatformBeforeEndOfDxeEventGroupGuid,
                    &BeforeEndOfDxeEvent
                    );
    ASSERT_EFI_ERROR (Status);
    gBS->SignalEvent (BeforeEndOfDxeEvent);
    gBS->CloseEvent (BeforeEndOfDxeEvent);
  }

  if (GetFirstGuidHob (&gSkipBiosLockForSysFwUpdateGuid) != NULL) {
    DEBUG ((DEBUG_INFO, "BIOS region is not protected, force to reset system before EndOfDxe\n"));
    gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
  }

  //
  // If BootMode is not BOOT_ON_FLASH_UPDATE, memory should already be cleaned by MRC via CleanMemory policy.
  //
  if (BootMode == BOOT_ON_FLASH_UPDATE) {
    //
    // We are here if not all capsules are processed, or there is
    // failure or reset is not required in ProcessCapsules() above.
    //
    if ((FeaturePcdGet (PcdTpm2Enable) == TRUE) && IsMorBitSet ()) {
      //
      // If MOR is set, there maybe secret in memory (not cleared).
      // Reset to avoid the potential secret exposure after EndOfDxe.
      //
      gRT->ResetSystem (EfiResetCold, EFI_SUCCESS, 0, NULL);
    }
  }

  PERF_START_EX (NULL,"EventRec", NULL, AsmReadTsc (), 0x7020);
  ExitPmAuth ();
  PERF_END_EX (NULL,"EventRec", NULL, AsmReadTsc (), 0x7021);

  //
  // Dispatch the deferred 3rd party images.
  //
  EfiBootManagerDispatchDeferredImages ();

  //
  // Restart ASF BIOS Watchdog Timer after third party option ROM is complete.
  //
  AmtStartWatchdog ();

  // External EFI graphic image will be deferred.
  // If it's IGPU scenario, the ConnectController should have been done earlier.
  // So here we need to handle non-IGPU scenario.
  //
  if ((VideoHandle != NULL) && (IsIgd == FALSE)) {
    //
    // Connect the GOP driver
    //
    gBS->ConnectController (VideoHandle, NULL, NULL, TRUE);

    //
    // Update ConOut variable according to the PrimaryDisplay setting
    //
    TempDevicePath = EfiBootManagerGetGopDevicePath (VideoHandle);
    if (TempDevicePath != NULL) {
      EfiBootManagerUpdateConsoleVariable (ConOut, TempDevicePath, NULL);
      FreePool (TempDevicePath);
    }

    //
    // Necessary for ConPlatform and ConSplitter driver to start up again after ConOut is updated.
    //
    gBS->ConnectController (VideoHandle, NULL, NULL, TRUE);
  }

  if ((FeaturePcdGet (PcdTpm2Enable) == TRUE) &&
      ((TcgPhysicalPresenceLibNeedUserConfirm() == TRUE) ||
       (Tcg2PhysicalPresenceLibNeedUserConfirm() == TRUE))) {
    Print (L"\nTPM Physical Presence prompt deferred due to incompatible configuration.\n");
    Print (L"Please check your video and keyboard hardware configuration.\n");
    Print (L"System will continue to resume shortly...\n");

    DEBUG ((DEBUG_ERROR, "[TCG] PP: TPM Physical Presence prompt deferred due to incompatible configuration.\n"));
    DEBUG ((DEBUG_ERROR, "Please check your video and keyboard hardware configuration.\n"));
    DEBUG ((DEBUG_ERROR, "System will continue to resume shortly...\n"));

    //
    // delay for 10 seconds that is sufficient time to include
    // video monitor slowness
    //
    MicroSecondDelay (10000000);
    gST->ConOut->ClearScreen (gST->ConOut);
  }

  //
  // Register Amt hotkeys here to avoid signalling ReadyToBoot before ExitPmAuth
  //
  AmtRegisterHotKey ();

  //
  // Inform other drivers that console is connected.
  //
  ConsoleHandle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &ConsoleHandle,
                  &gConsoleOutConnectedProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);


}

/**
  Connect with predefined platform connect sequence,
  the OEM/IBV can customize with their own connect sequence.

  @param[in] BootMode          Boot mode of this boot.
**/
VOID
ConnectSequence (
  IN EFI_BOOT_MODE         BootMode
  )
{
  UINTN                    Index;
  EFI_HANDLE               DeviceHandle;
  EFI_STATUS               Status;

  //
  // Here we can get the customized platform connect sequence
  // Notes: we can connect with new variable which record the
  // last time boots connect device path sequence
  //
  for (Index = 0; gPlatformConnectSequence[Index] != NULL; Index++) {
    if ((gPlatformConnectSequence[Index] == (EFI_DEVICE_PATH_PROTOCOL *) &gPlatformOnboardXhciDevice) ||
        (gPlatformConnectSequence[Index] == (EFI_DEVICE_PATH_PROTOCOL *) &gPlatformTcssXhciDevice)) {
      if ((BootMode == BOOT_ASSUMING_NO_CONFIGURATION_CHANGES && !MeClientIsCorporate ()) ||
          (BootMode == BOOT_WITH_MINIMAL_CONFIGURATION) ||
          (BootMode == BOOT_ON_S4_RESUME)) {
        //
        // If local boot mode variable indicates fast boot or S4 resume, skip connecting to USB controller.
        // Skip connecting to USB controller also when booting with no configuration changes unless it is
        // Corporate SKU where USB Provisioning must be possible.
        // Local boot mode variable is passed to this function based on various conditions checked before.
        //
        continue;
      }
    }

    //
    // Build the platform boot option
    //
    Status = EfiBootManagerConnectDevicePath (gPlatformConnectSequence[Index], &DeviceHandle);
    if (gPlatformConnectSequence[Index] == (EFI_DEVICE_PATH_PROTOCOL *) &gPlatformOnboardXhciDevice) {
      if (!EFI_ERROR (Status)) {
        gBS->ConnectController (DeviceHandle, NULL, NULL, TRUE);
      }
    }
    if (gPlatformConnectSequence[Index] == (EFI_DEVICE_PATH_PROTOCOL *) &gPlatformTcssXhciDevice) {
      if (!EFI_ERROR (Status)) {
        gBS->ConnectController (DeviceHandle, NULL, NULL, TRUE);
      }
    }
#if (FixedPcdGetBool(PcdVmdEnable) == 1)
    if (gPlatformConnectSequence[Index] == (EFI_DEVICE_PATH_PROTOCOL *) &gPlatformOnboardVmdDevice) {
      if (mSaSetup.VmdEnable == 1 && ((!EFI_ERROR (Status) || (Status == EFI_NOT_FOUND)))) {
        gBS->ConnectController (DeviceHandle, NULL, NULL, TRUE);
      }
    }
#endif
  }

  //
  // Dispatch again since Hybrid Graphics driver depends on PCI_IO protocol
  //
  gDS->Dispatch ();
}

/**
  Perform the platform diagnostic, such like test memory. OEM/IBV also
  can customize this function to support specific platform diagnostic.

  @param MemoryTestLevel  - The memory test intensive level
  @param QuietBoot        - Indicate if need to enable the quiet boot
**/
VOID
Diagnostics (
  IN EXTENDMEM_COVERAGE_LEVEL    MemoryTestLevel,
  IN BOOLEAN                     QuietBoot
  )
{
  EFI_STATUS  Status;

  //
  // Here we can decide if we need to show
  // the diagnostics screen
  // Notes: this quiet boot code should be remove
  // from the graphic lib
  //
  if (GetDisplayBootMode () != NORMAL_BOOT) {
    gST->ConOut->EnableCursor (gST->ConOut, FALSE);
  }

  //
  // Perform system diagnostic
  //
  Status = MemoryTest (MemoryTestLevel);
  if (EFI_ERROR (Status)) {
    BootLogoDisableLogo ();
  }
  return ;
}

/**
  The function is to consider the boot order which is not in our expectation.
  In the case that we need to re-sort the boot option.

  @retval  TRUE         Need to sort Boot Option.
  @retval  FALSE        Don't need to sort Boot Option.
**/
BOOLEAN
IsNeedSortBootOption (
  VOID
  )
{
  EFI_BOOT_MANAGER_LOAD_OPTION  *BootOptions;
  UINTN                         BootOptionCount;

  BootOptions = EfiBootManagerGetLoadOptions (&BootOptionCount, LoadOptionTypeBoot);

  //
  // If setup is the first priority in boot option, we need to sort boot option.
  //
  if ((BootOptionCount > 1) &&
      (StrnCmp (BootOptions->Description, L"Enter Setup", StrLen (L"Enter Setup"))) == 0) {
    return TRUE;
  }

  return FALSE;
}

/**
  Checks if given network adapter is required for boot.

  @param[in] PciIo      PciIo protocol connected with given device
  @param[in] PciConfig  Config space of the device
**/
STATIC
BOOLEAN
IsPciNetworkAdapterRequiredForBoot (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN PCI_TYPE00           *PciConfig
  )
{
  UINTN    Segment;
  UINTN    Bus;
  UINTN    Device;
  UINTN    Function;

  if (mSystemConfiguration.EfiNetworkSupport == EFI_NETWORK_DISABLED) {
    return FALSE;
  }

  //
  // Note that EFI_NETWORK_ALL = Intel wifi + integrated GbE + External GbE
  //
  if (PciConfig->Hdr.VendorId == INTEL_VENDOR_ID &&
      PciConfig->Hdr.ClassCode[1] == PCI_CLASS_NETWORK_OTHER) {
    if (mSystemConfiguration.EfiNetworkSupport == EFI_NETWORK_ALL || mSystemConfiguration.EfiNetworkSupport == EFI_NETWORK_WIFI) {
      return TRUE;
    } else {
      return FALSE;
    }
  } else if (PciConfig->Hdr.ClassCode[1] == PCI_CLASS_NETWORK_ETHERNET) {
    PciIo->GetLocation (PciIo, &Segment, &Bus, &Device, &Function);
    if (mSystemConfiguration.EfiNetworkSupport == EFI_NETWORK_ALL || mSystemConfiguration.EfiNetworkSupport == EFI_NETWORK_ONBOARD_NIC) {
      return TRUE;
    } else {
      return FALSE;
    }
  } else {
    return FALSE;
  }
}

/**
  Checks if PCI device is required for boot. The decision whether device
  is required is based on device class code and platform policy.

  @param[in] PciDevHandle  Handle that supports PCI IO protocol

  @retval TRUE  Driver should be connected to device
  @retval FALSE Driver should not be connected to device
**/
STATIC
BOOLEAN
IsPciDeviceRequiredForBoot (
  IN EFI_HANDLE  PciDevHandle
  )
{
  EFI_STATUS           Status;
  PCI_TYPE00           PciConfig;
  EFI_PCI_IO_PROTOCOL  *PciIo;

  Status = gBS->HandleProtocol (
                  PciDevHandle,
                  &gEfiPciIoProtocolGuid,
                  (VOID *) &PciIo
                  );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  Status = PciIo->Pci.Read (
                        PciIo,
                        EfiPciIoWidthUint8,
                        0,
                        sizeof (PciConfig),
                        &PciConfig
                        );
  //
  // Every mass storage device is a potential boot medium,
  // connect them all.
  //
  if (PciConfig.Hdr.ClassCode[2] == PCI_CLASS_MASS_STORAGE) {
    return TRUE;
  //
  // Connect SD peripherals as they might have storage device connected.
  //
  } else if (PciConfig.Hdr.ClassCode[2] == PCI_CLASS_SYSTEM_PERIPHERAL &&
             PciConfig.Hdr.ClassCode[1] == 5) { // 5 = SD Host controller
    return TRUE;
  } else if (PciConfig.Hdr.ClassCode[2] == PCI_CLASS_NETWORK) {
    return IsPciNetworkAdapterRequiredForBoot (PciIo, &PciConfig);
  }
  return FALSE;
}

/**
  Connects drivers to required PCI devices.
  This function assumes that PciBus driver has
  already been connected to all host bridges and all
  PciIo handles are present in the system.
**/
STATIC
VOID
ConnectDriversToRequiredPciControllers (
  VOID
  )
{
  EFI_STATUS  Status;
  UINTN       HandleCount;
  EFI_HANDLE  *HandleBuffer;
  UINT32      Index;

  DEBUG ((DEBUG_INFO, "Connecting required PCI drivers\n"));

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    if (IsPciDeviceRequiredForBoot (HandleBuffer[Index])) {
      gBS->ConnectController (HandleBuffer[Index], NULL, NULL, TRUE);
    }
  }

  FreePool (HandleBuffer);
}

/**
  This function is called from the core after console devices have been
  connected.
**/
VOID
EFIAPI
PlatformBootManagerAfterConsole (
  VOID
  )
{
  EFI_BOOT_MODE                 LocalBootMode;
  BOOLEAN                       BootState;
  BOOLEAN                       IsFirstBoot;
  EFI_STATUS                    Status;
  BOOLEAN                       UefiShellEnabled;
  ESRT_MANAGEMENT_PROTOCOL      *EsrtManagement;

  IsFirstBoot = FALSE;
  UefiShellEnabled = PcdGetBool (PcdUefiShellEnable);
  //
  // Check BootState variable, NULL means it's the first boot after reflashing
  //
  BootState = IsBootStatePresent();
  if (!BootState) {
    IsFirstBoot = TRUE;
  }

  //
  // Use a DynamicHii type pcd to save the boot status, which is used to
  // control configuration mode, such as FULL/MINIMAL/NO_CHANGES configuration.
  //
  if (IsFirstBoot) {
    SetBootState ();
  }
  //
  // If EFI Shell is NOT in Boot List, then Delete BootState variable,
  // this would enforce next boot to FullConfiguration which bring EFI shell back to Boot Order.
  if (UefiShellEnabled && IsShellInBootList() == FALSE) {
    UnsetBootState();
  }

  //
  // After Console is available, run AMT specific action
  //
  AmtConsoleReady ();

  //
  // Get current Boot Mode
  //
  LocalBootMode = gBootMode;
  DEBUG ((DEBUG_INFO, "Local bootmode before FastBoot update - %x\n", LocalBootMode));
  FastBootUpdateBootMode (&LocalBootMode);

  //
  // Fast Boot does the necessary patch for boot option.
  //
  FastBootBootOptionPatch (LocalBootMode);
  DEBUG ((DEBUG_INFO, "Local bootmode before Amt update - %x\n", LocalBootMode));
  AmtUpdateBootMode (&LocalBootMode);
  DEBUG ((DEBUG_INFO, "Current local bootmode - %x\n", LocalBootMode));
  Status = gBS->LocateProtocol(&gEsrtManagementProtocolGuid, NULL, (VOID **)&EsrtManagement);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "BdsPlatform: Failed to locate EsrtManagementProtocol\n"));
    EsrtManagement = NULL;
  }

  //
  // Go the different platform policy with different boot mode
  // Notes: this part code can be change with the table policy
  //
  switch (LocalBootMode) {

  case BOOT_ASSUMING_NO_CONFIGURATION_CHANGES:
  case BOOT_WITH_MINIMAL_CONFIGURATION:
  case BOOT_ON_S4_RESUME:
    //
    // gST->ConOut is hooked to disable UI if Fast Boot is enabled
    //
    FastBootHookConOut();

    //
    // Memory test
    //
    Status = MemoryTest((EXTENDMEM_COVERAGE_LEVEL) PcdGet8 (PcdPlatformMemoryCheck));
    PERF_START_EX(NULL,"EventRec", NULL, AsmReadTsc(), 0x7050);
    ConnectDriversToRequiredPciControllers ();
    //
    // Perform some platform specific connect sequence
    //
    ConnectSequence (LocalBootMode);
    PERF_END_EX(NULL,"EventRec", NULL, AsmReadTsc(), 0x7051);

    SignalAllDriversConnected ();

    //
    // Restore gST->ConOut
    //
    FastBootRestoreConOut();
    break;

  case BOOT_ON_FLASH_UPDATE:
    //
    // Boot with the specific configuration
    //
    if (FeaturePcdGet (PcdSupportUpdateCapsuleReset)) {

      Diagnostics (EXTENSIVE, FALSE);
      EfiBootManagerConnectAll ();

      SignalAllDriversConnected ();

      if (EsrtManagement != NULL) {
        EsrtManagement->SyncEsrtFmp ();
      }

      PlatformDisablePowerButton (); // In case of accidental shutdown during FW update process.
      DEBUG ((DEBUG_INFO, "ProcessCapsules After ConnectAll......\n"));
      ProcessCapsules();
      DEBUG ((DEBUG_INFO, "ProcessCapsules Done\n"));
      PlatformEnablePowerButton ();
    } else {
      ASSERT (FeaturePcdGet (PcdSupportUpdateCapsuleReset));
    }
    break;

  case BOOT_IN_RECOVERY_MODE:
    //
    // In recovery mode, just connect platform console
    // and show up the front page
    //
    Diagnostics (EXTENSIVE, FALSE);
    EfiBootManagerConnectAll ();

    SignalAllDriversConnected ();
    //
    // In recovery boot mode, we still enter to the
    // frong page now
    //
    break;

  case BOOT_WITH_FULL_CONFIGURATION:
  case BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS:
  case BOOT_WITH_DEFAULT_SETTINGS:
  default:
    Diagnostics ((EXTENDMEM_COVERAGE_LEVEL) PcdGet8 (PcdPlatformMemoryCheck), (BOOLEAN) (GetDisplayBootMode () == QUIET_BOOT));

    ConnectDriversToRequiredPciControllers ();
    //
    // Perform some platform specific connect sequence
    //
    ConnectSequence (LocalBootMode);
    SignalAllDriversConnected ();

    //
    // Only in Full Configuration boot mode we do the enumeration of boot device
    //

    EfiBootManagerRefreshAllBootOption ();

    //
    // Sync ESRT Cache from FMP Instance on demand after Connect All
    //
    if (EsrtManagement != NULL) {
      EsrtManagement->SyncEsrtFmp ();
    }

    if ((BootState == FALSE) || (IsNeedSortBootOption())) {
      EfiBootManagerSortLoadOptionVariable (LoadOptionTypeBoot, CompareBootOption);
    }
    //
    // PXE boot option may appear after boot option enumeration
    //
    RegisterDynamicHotkey ();
    break;
  }

  //
  // Display Logo
  //
  BootLogoEnableLogo ();
  DrawBiosColorCode ();

  if (!PcdGetBool (PcdConInConnectOnDemand)) {
    PrintBootPrompt ();
  }

  AmtBdsBoot ();

  if (PcdGetBool (PcdForceSerialAfterConsoleReady)) {
    SerialPortInitialize ();
  }

  GenGbtXml();


  // Check if needing to perform a recovery boot
  AmtPerformOneClickRecoveryBoot ();

  PlatformBootManagerAttemptUsbBoot ();
  FastBootUpdateTimeoutVariable ();
}

/**
  This function locks the block

  @param CpuIo           A instance of EFI_CPU_IO2_PROTOCOL.
  @param Base            The base address flash region to be locked.

**/
VOID
BdsLockFv (
  IN EFI_CPU_IO2_PROTOCOL         *CpuIo,
  IN EFI_PHYSICAL_ADDRESS         Base
  )
{
  EFI_FV_BLOCK_MAP_ENTRY      *BlockMap;
  EFI_FIRMWARE_VOLUME_HEADER  *FvHeader;
  EFI_PHYSICAL_ADDRESS        BaseAddress;
  UINT8                       Data;
  UINT32                      BlockLength;
  UINTN                       Index;

  BaseAddress = Base - 0x400000 + 2;
  FvHeader    = (EFI_FIRMWARE_VOLUME_HEADER *) ((UINTN) (Base));
  BlockMap    = &(FvHeader->BlockMap[0]);

  while ((BlockMap->NumBlocks != 0) && (BlockMap->Length != 0)) {
    BlockLength = BlockMap->Length;
    for (Index = 0; Index < BlockMap->NumBlocks; Index++) {
      CpuIo->Mem.Read (
                  CpuIo,
                  EfiCpuIoWidthUint8,
                  BaseAddress,
                  1,
                  &Data
                  );
      Data = (UINT8) (Data | 0x3);
      CpuIo->Mem.Write (
                  CpuIo,
                  EfiCpuIoWidthUint8,
                  BaseAddress,
                  1,
                  &Data
                  );
      BaseAddress += BlockLength;
    }

    BlockMap++;
  }
}

VOID
LockNonUpdatableFlash (
  VOID
  )
{
  EFI_STATUS                Status;
  EFI_CPU_IO2_PROTOCOL      *CpuIo;
  EFI_PHYSICAL_ADDRESS      Base[4];
  UINTN                     Index;

// @todo The set of PCDs for FlashFV has been changed in MinPlatform, the relevant code need to be redesign.
// @todo This function is not calling by anyone, leave it for now.
  Base[0] = 0x00;//(EFI_PHYSICAL_ADDRESS) PcdGet32 (PcdFlashFvMainBase);
  Base[1] = 0x00;//(EFI_PHYSICAL_ADDRESS) PcdGet32 (PcdFlashFvRecoveryBase);
  Base[2] = 0x00;//(EFI_PHYSICAL_ADDRESS) PcdGet32 (PcdFlashFvRecovery2Base);
  Base[3] = 0x00;//(EFI_PHYSICAL_ADDRESS) PcdGet32 (PcdFlashFvRecovery3Base);

  Status = gBS->LocateProtocol (&gEfiCpuIo2ProtocolGuid, NULL, (VOID **) &CpuIo);
  ASSERT_EFI_ERROR (Status);

  for (Index = 0; Index < sizeof (Base) / sizeof (Base[0]); Index++) {
    if (Base[Index] > 0) {
      BdsLockFv (CpuIo, Base[Index]);
    }
  }
}

/**
  Read data value from 'AttemptUSBFirstRuntime' volatile variable
  to determine the feature of 'Attempt USB First' is enabled or not.

  @retval TRUE            - 'Attempt USB First' feature is enabled
  @retval FALSE           - 'Attempt USB First' feature is disabled
**/
BOOLEAN
IsAttemptUSBFirstEnable (
  VOID
  )
{
  EFI_STATUS                         Status;
  UINTN                              DataSize;
  ATTEMPT_USB_FIRST_RUNTIME_VARIABLE AttemptUsbFirstRuntimeVarInfo;

  ZeroMem (
    &AttemptUsbFirstRuntimeVarInfo,
    sizeof (ATTEMPT_USB_FIRST_RUNTIME_VARIABLE)
    );

  //
  // Get 'AttemptUSBFirstRuntime' variable
  //
  DataSize = sizeof (ATTEMPT_USB_FIRST_RUNTIME_VARIABLE);
  Status = gRT->GetVariable (
                  L"AttemptUSBFirstRuntime",
                  &gAttemptUsbFirstRuntimeVarInfoGuid,
                  NULL,
                  &DataSize,
                  &AttemptUsbFirstRuntimeVarInfo
                  );
  if(!EFI_ERROR(Status) && AttemptUsbFirstRuntimeVarInfo.UsbFirstEnable) {
    return TRUE;
  }

  return FALSE;
}

/**
  Add and Set AttmptUsbFirst as first bootable device if ENABLE
**/
VOID
PlatformBootManagerAttemptUsbBoot (
  VOID
  )
{
  EFI_STATUS                   Status;
  EFI_BOOT_MANAGER_LOAD_OPTION UsbBootOption;
  EFI_BOOT_MANAGER_LOAD_OPTION *BootOptions;
  UINTN                        BootOptionCount;
  UINTN                        Index;
  BOOLEAN                      AttemptUSBFirst;

  //
  // Specific boot for AttemptUSBFirst feature
  //
  if(IsAttemptUSBFirstEnable()) {
    DEBUG ((DEBUG_INFO, "PlatformBootManagerAttemptUsbBoot : AttemptUSBFirst is enabled. \n"));
    AttemptUSBFirst = FALSE;
    BootOptions = EfiBootManagerGetLoadOptions (&BootOptionCount, LoadOptionTypeBoot);
    for (Index = 0; Index < BootOptionCount; Index++) {
      if ((StrnCmp (BootOptions[Index].Description, L"AttemptUSBFirst Boot", StrLen (L"AttemptUSBFirst Boot"))) == 0) {
        DEBUG ((DEBUG_INFO, "PlatformBootManagerAttemptUsbBoot : AttemptUSBFirst Boot signature is found. The location is %x\n", Index));
        if (Index == 0) {
          AttemptUSBFirst = TRUE;
        }
        break;
      }
    }
    EfiBootManagerFreeLoadOptions (BootOptions, BootOptionCount);
    if (AttemptUSBFirst == FALSE) {
      DEBUG ((DEBUG_INFO, "PlatformBootManagerAttemptUsbBoot : AttemptUSBFirst Boot signature is NOT found at the top of boot order. Re-init Boot order\n"));
      Status = EfiBootManagerInitializeLoadOption (
                 &UsbBootOption,
                 LoadOptionNumberUnassigned,
                 LoadOptionTypeBoot,
                 LOAD_OPTION_CATEGORY_BOOT | LOAD_OPTION_ACTIVE | LOAD_OPTION_HIDDEN,
                 L"AttemptUSBFirst Boot",
                 (EFI_DEVICE_PATH_PROTOCOL *) &gUsbClassMassStorageDevice,
                 NULL,
                 0
                 );
      ASSERT_EFI_ERROR (Status);
      if (!EFI_ERROR(Status)) {
        EfiBootManagerAddLoadOptionVariable (&UsbBootOption, 0);
      }
    }
    DEBUG ((DEBUG_INFO, "PlatformBootManagerAttemptUsbBoot : Initialize USB bootable devices. (ConnectUsbController)\n"));
    ConnectUsbController ();
    ConnectTcssXhciController ();
  } else {
    DEBUG ((DEBUG_INFO, "PlatformBootManagerAttemptUsbBoot : AttemptUSBFirst is NOT enabled. \n"));
    BootOptions = EfiBootManagerGetLoadOptions (&BootOptionCount, LoadOptionTypeBoot);
    for (Index = 0; Index < BootOptionCount; Index++) {
      if ((StrnCmp (BootOptions[Index].Description, L"AttemptUSBFirst Boot", StrLen (L"AttemptUSBFirst Boot"))) == 0) {
        EfiBootManagerDeleteLoadOptionVariable (BootOptions[Index].OptionNumber, LoadOptionTypeBoot);
        break;
      }
    }
    EfiBootManagerFreeLoadOptions (BootOptions, BootOptionCount);
  }
}

/**
  The function is called when no boot option could be launched,
  including platform recovery options and options pointing to applications
  built into firmware volumes.

  If this function returns, BDS attempts to enter an infinite loop.
**/
VOID
EFIAPI
PlatformBootManagerUnableToBoot (
  VOID
  )
{
  EFI_STATUS                   Status;
  EFI_BOOT_MANAGER_LOAD_OPTION BootDeviceList;
  CHAR16                       OptionName[sizeof ("Boot####")];

  if (mBootMenuOptionNumber == LoadOptionNumberUnassigned) {
    return;
  }
  UnicodeSPrint (OptionName, sizeof (OptionName), L"Boot%04x", mBootMenuOptionNumber);
  Status = EfiBootManagerVariableToLoadOption (OptionName, &BootDeviceList);
  ASSERT_EFI_ERROR (Status);

  for (;;) {
    EfiBootManagerBoot (&BootDeviceList);
  }
}
