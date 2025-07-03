/** @file
  Header file for BDS Platform specific code

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

#ifndef _BDS_PLATFORM_H
#define _BDS_PLATFORM_H

#include <PiDxe.h>
#include <Protocol/DevicePath.h>
#include <Protocol/SimpleNetwork.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/LoadFile.h>
#include <Protocol/PciIo.h>
#include <Protocol/CpuIo2.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/DiskInfo.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/UgaDraw.h>
#include <Protocol/GenericMemoryTest.h>
#include <Protocol/DevicePathToText.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/EsrtManagement.h>

#include <Guid/MemoryTypeInformation.h>
#include <Guid/GlobalVariable.h>
#include <Guid/MemoryOverwriteControl.h>
#include <Guid/FileInfo.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/PlatformBootManagerLib.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/PrintLib.h>
#include <Library/HiiLib.h>
#include <Library/BootLogoLib.h>
#include <Library/CapsuleLib.h>
#include <Library/PerformanceLib.h>
#include <Library/EcMiscLib.h>
#include <Library/PciHostBridgeLib.h>

#include <Register/EspiRegs.h>
#include <Register/PchPcieRpRegs.h>

#include <IndustryStandard/Pci30.h>
#include <IndustryStandard/PciCodeId.h>

#include <SetupVariable.h>
#include <PlatformBoardType.h>

///
/// ConnectType
///
#define CONSOLE_OUT 0x00000001
#define STD_ERROR   0x00000002
#define CONSOLE_IN  0x00000004
#define CONSOLE_ALL (CONSOLE_OUT | CONSOLE_IN | STD_ERROR)

extern EFI_GUID                  gUefiShellFileGuid;
extern EFI_GUID                  gSetupEnterGuid;
extern EFI_BOOT_MODE             gBootMode;
extern SA_SETUP                  mSaSetup;
extern SETUP_DATA                mSystemConfiguration;
extern BOOLEAN                   mDecompressFvOptional;
extern EFI_DEVICE_PATH_PROTOCOL  *gPlatformConnectSequence[];

#define gPciRootBridge \
  { \
    { \
      ACPI_DEVICE_PATH, \
      ACPI_DP, \
      { \
        (UINT8) (sizeof (ACPI_HID_DEVICE_PATH)), \
        (UINT8) ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8) \
      }, \
    }, \
    EISA_PNP_ID (0x0A03), \
    0 \
  }

#define gEndEntire \
  { \
    END_DEVICE_PATH_TYPE, END_ENTIRE_DEVICE_PATH_SUBTYPE, { END_DEVICE_PATH_LENGTH, 0 } \
  }

typedef struct {
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  UINTN                     ConnectType;
} BDS_CONSOLE_CONNECT_ENTRY;

//
// Platform Root Bridge
//
typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_ROOT_BRIDGE_DEVICE_PATH;

//
// Below is the platform console device path
//
typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           IsaBridge;
  ACPI_HID_DEVICE_PATH      Keyboard;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_KEYBOARD_DEVICE_PATH;

#if FixedPcdGetBool (PcdVirtualKeyboardEnable) == 1
typedef struct {
  ACPI_EXTENDED_HID_DEVICE_PATH      I2cController;
  UINT8                              HidStr[8];
  UINT8                              UidStr[1];
  UINT8                              CidStr[8];
} PLATFORM_VIRTUAL_KEYBOARD_I2C_CONTROLLER_DEVICE_PATH;

typedef struct {
  ACPI_EXTENDED_HID_DEVICE_PATH      I2cDevice;
  UINT8                              HidStr[13];
  UINT8                              UidStr[1];
  UINT8                              CidStr[13];
} PLATFORM_VIRTUAL_KEYBOARD_I2C_DEVICE_DEVICE_PATH;

typedef struct {
  PLATFORM_VIRTUAL_KEYBOARD_I2C_CONTROLLER_DEVICE_PATH      I2cController;
  PLATFORM_VIRTUAL_KEYBOARD_I2C_DEVICE_DEVICE_PATH          I2cDevice;
  EFI_DEVICE_PATH_PROTOCOL                                  End;
} PLATFORM_VIRTUAL_KEYBOARD_I2C_DEVICE_PATH;
#endif

typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           IsaBridge;
  ACPI_HID_DEVICE_PATH      IsaSerial;
  UART_DEVICE_PATH          Uart;
  VENDOR_DEVICE_PATH        TerminalType;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_ISA_SERIAL_DEVICE_PATH;

typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           PciDevice;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_ONBOARD_CONTROLLER_DEVICE_PATH;

typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           PciDevice;
  USB_DEVICE_PATH           UsbDevice;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_ONBOARD_USB_DEVICE_PATH;

typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           Pci0Device;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_PEG_ROOT_CONTROLLER_DEVICE_PATH;

typedef struct {
  ACPI_HID_DEVICE_PATH      PciRootBridge;
  PCI_DEVICE_PATH           PciBridge;
  PCI_DEVICE_PATH           PciDevice;
  EFI_DEVICE_PATH_PROTOCOL  End;
} PLATFORM_PCI_CONTROLLER_DEVICE_PATH;

#define CLASS_HID           3
#define SUBCLASS_BOOT       1
#define PROTOCOL_KEYBOARD   1

typedef struct {
  USB_CLASS_DEVICE_PATH           UsbClass;
  EFI_DEVICE_PATH_PROTOCOL        End;
} USB_CLASS_FORMAT_DEVICE_PATH;

extern BDS_CONSOLE_CONNECT_ENTRY                 gPlatformConsole[];
extern PLATFORM_ISA_SERIAL_DEVICE_PATH           gSerialDevicePath;
extern PLATFORM_ONBOARD_CONTROLLER_DEVICE_PATH   gPlatformIGDDevice;
extern PLATFORM_ONBOARD_CONTROLLER_DEVICE_PATH   gPlatformOnboardXhciDevice;
extern PLATFORM_ONBOARD_CONTROLLER_DEVICE_PATH   gPlatformTcssXhciDevice;
extern PLATFORM_ONBOARD_CONTROLLER_DEVICE_PATH   gPlatformPchPcieRootController;
extern PLATFORM_PEG_ROOT_CONTROLLER_DEVICE_PATH  gPlatformPegRootController;
extern PLATFORM_KEYBOARD_DEVICE_PATH             gKeyboardDevicePath;
extern USB_CLASS_FORMAT_DEVICE_PATH              gUsbClassKeyboardDevicePath;
extern USB_CLASS_FORMAT_DEVICE_PATH              gUsbClassMassStorageDevice;
extern PLATFORM_ONBOARD_CONTROLLER_DEVICE_PATH   gPlatformOnboardVmdDevice;


//
// Platform BDS Functions
//
typedef
VOID
(*PROCESS_VARIABLE) (
  VOID  **Variable,
  UINTN *VariableSize
  );

/**
  Update gPlatformConsole to ConIn variable
**/
VOID
UpdatePlatformConsoleToConInVairable (
  VOID
  );

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
  );

/**
  Perform the memory test base on the memory test intensive level,
  and update the memory resource.

  @param  Level         The memory test intensive level.

  @retval EFI_STATUS    Success test all the system memory and update
                        the memory resource

**/
EFI_STATUS
MemoryTest (
  IN EXTENDMEM_COVERAGE_LEVEL Level
  );

VOID
ConnectSequence (
  IN EFI_BOOT_MODE                      BootMode
  );

/**
  This function locks platform flash that is not allowed to be updated during normal boot path.
  The flash layout is platform specific.

**/
VOID
LockNonUpdatableFlash (
  VOID
  );

INTN
EFIAPI
CompareBootOption (
  CONST VOID  *Left,
  CONST VOID  *Right
  );

/**
  Print the boot prompt.
**/
VOID
PrintBootPrompt (
  VOID
  );

VOID
RegisterStaticHotkey (
  VOID
  );

VOID
RegisterDynamicHotkey (
  VOID
  );

VOID
RegisterDefaultBootOption (
  VOID
  );

VOID
BootUi (
  VOID
  );

VOID
PlatformBootManagerAttemptUsbBoot (
  VOID
  );

BOOLEAN
IsShellInBootList (
  VOID
  );

EFI_STATUS
EFIAPI
ChangeModeForInternalShell (
  VOID
  );

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
  );

/**
  Update on board XHCI device path to meet silicon configuration
**/
VOID
UpdateOnboardXhciDevicePath (
  VOID
  );

/**
  This function enables FvApp which contains EFI Shell.

  @retval   EFI_SUCCESS Successfully enable FvApp
**/
EFI_STATUS
EnableFvApp (
    VOID
);

/**
  This function process FvApp which contains EFI Shell.

  @retval   EFI_SUCCESS          Successfully FvApp has been processed.
  @retval   EFI_UNSUPPORTED      FvApp has not been enabled so fails to be processed.
**/
EFI_STATUS
ProcessFvApp (
    VOID
);
#endif
