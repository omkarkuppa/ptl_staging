/** @file
  Defined the platform specific device path which will be used by
  platform Bbd to perform the platform policy connect.

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
#include <PchPcieRpInfo.h>
#include <Defines/PcdPchBdfAssignment.h>
#if FixedPcdGetBool (PcdTerminalOverDebugEnable) == 1
#include <Protocol/TerminalOverCommon.h>
#endif

//
// Platform specific keyboard device path
//
GLOBAL_REMOVE_IF_UNREFERENCED PLATFORM_KEYBOARD_DEVICE_PATH     gKeyboardDevicePath = {
  gPciRootBridge,
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_PCI_DP,
      {
        (UINT8) (sizeof (PCI_DEVICE_PATH)),
        (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8)
      }
    },
    PCI_FUNCTION_NUMBER_PCH_LPC,
    PCI_DEVICE_NUMBER_PCH_LPC
  },
  {
    {
      ACPI_DEVICE_PATH,
      ACPI_DP,
      {
        (UINT8) (sizeof (ACPI_HID_DEVICE_PATH)),
        (UINT8) ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8)
      }
    },
    EISA_PNP_ID(0x0303),
    0
  },
  gEndEntire
};

#if FixedPcdGetBool (PcdVirtualKeyboardEnable) == 1
GLOBAL_REMOVE_IF_UNREFERENCED PLATFORM_VIRTUAL_KEYBOARD_I2C_DEVICE_PATH gVirtualKeyboardDevicePath = {
  {
    {
      {
        ACPI_DEVICE_PATH,
        ACPI_EXTENDED_DP,
        { 33, 0 }
      },
      0,  // HID
      2,  // UID
      0   // CID
    },
    {'I', 'N', 'T', '3', '3', 'C', '3', 0},
    {0},
    {'I', 'N', 'T', '3', '3', 'C', '3', 0}
  },
  {
    {
      {
        ACPI_DEVICE_PATH,
        ACPI_EXTENDED_DP,
        { 43, 0 }
      },
      0,  // HID
      1,  // UID
      0   // CID
    },
    {'I', '2', 'C', '0', '1',  '\\', 'T', 'P', 'A', 'N', 'E', 'L', 0},
    {0},
    {'I', '2', 'C', '0', '1',  '\\', 'T', 'P', 'A', 'N', 'E', 'L', 0}
  },
  gEndEntire
};
#endif

//
// Platform specific serial device path
//
GLOBAL_REMOVE_IF_UNREFERENCED PLATFORM_ISA_SERIAL_DEVICE_PATH   gSerialDevicePath = {
  gPciRootBridge,
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_PCI_DP,
      {
        (UINT8) (sizeof (PCI_DEVICE_PATH)),
        (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8)
      }
    },
    PCI_FUNCTION_NUMBER_PCH_LPC,
    PCI_DEVICE_NUMBER_PCH_LPC
  },
  {
    {
      ACPI_DEVICE_PATH,
      ACPI_DP,
      {
        (UINT8) (sizeof (ACPI_HID_DEVICE_PATH)),
        (UINT8) ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8)
      }
    },
    EISA_PNP_ID(0x0501),
    0
  },
  {
    {
      MESSAGING_DEVICE_PATH,
      MSG_UART_DP,
      {
        (UINT8) (sizeof (UART_DEVICE_PATH)),
        (UINT8) ((sizeof (UART_DEVICE_PATH)) >> 8)
      }
    },
    0,
    FixedPcdGet32 (PcdSerialBaudRate),
    8,
    1,
    1
  },
  {
    {
      MESSAGING_DEVICE_PATH,
      MSG_VENDOR_DP,
      {
        (UINT8) (sizeof (VENDOR_DEVICE_PATH)),
        (UINT8) ((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    DEVICE_PATH_MESSAGING_PC_ANSI
  },
  gEndEntire
};

GLOBAL_REMOVE_IF_UNREFERENCED USB_CLASS_FORMAT_DEVICE_PATH gUsbClassBtKeyboardDevicePath = {
  {
    {
      MESSAGING_DEVICE_PATH,
      MSG_USB_CLASS_DP,
      {
        (UINT8) (sizeof (USB_CLASS_DEVICE_PATH)),
        (UINT8) ((sizeof (USB_CLASS_DEVICE_PATH)) >> 8)
      }
    },
    0xffff,  // VendorId
    0xffff,  // ProductId
    0xE0,    // DeviceClass    - Wireless Controller Class
    0x1,     // DeviceSubClass - RF Controller
    0x1      // DeviceProtocol - Bluetooth Primary Controller
  },
  gEndEntire
};

GLOBAL_REMOVE_IF_UNREFERENCED USB_CLASS_FORMAT_DEVICE_PATH gUsbClassKeyboardDevicePath = {
  {
    {
      MESSAGING_DEVICE_PATH,
      MSG_USB_CLASS_DP,
      {
        (UINT8) (sizeof (USB_CLASS_DEVICE_PATH)),
        (UINT8) ((sizeof (USB_CLASS_DEVICE_PATH)) >> 8)
      }
    },
    0xffff,           // VendorId
    0xffff,           // ProductId
    CLASS_HID,        // DeviceClass
    SUBCLASS_BOOT,    // DeviceSubClass
    PROTOCOL_KEYBOARD // DeviceProtocol
  },
  gEndEntire
};

//
// Onboard VGA controller device path
//
GLOBAL_REMOVE_IF_UNREFERENCED PLATFORM_ONBOARD_CONTROLLER_DEVICE_PATH         gPlatformIGDDevice = {
  gPciRootBridge,
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_PCI_DP,
      {
        (UINT8) (sizeof (PCI_DEVICE_PATH)),
        (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8)
      }
    },
    0x0,
    0x2
  },
  gEndEntire
};

//
// Predefined platform default console device path
//
GLOBAL_REMOVE_IF_UNREFERENCED BDS_CONSOLE_CONNECT_ENTRY         gPlatformConsole[] = {
  //
  // Match PCCG current bios, remove console from ConIn&ConOut
  //
  // {
  //   (EFI_DEVICE_PATH_PROTOCOL *) &gSerialDevicePath,
  //   (CONSOLE_OUT | CONSOLE_IN)
  // },
  {
    (EFI_DEVICE_PATH_PROTOCOL *) &gKeyboardDevicePath,
    CONSOLE_IN
  },
  {
    (EFI_DEVICE_PATH_PROTOCOL *) &gUsbClassKeyboardDevicePath,
    CONSOLE_IN
  },
  {
    (EFI_DEVICE_PATH_PROTOCOL *) &gUsbClassBtKeyboardDevicePath,
    CONSOLE_IN
  },
  {
    NULL,
    0
  }
};
#if FixedPcdGetBool (PcdTerminalOverDebugEnable) == 1
GLOBAL_REMOVE_IF_UNREFERENCED PLATFORM_DCIIO_DEVICE_PATH  XhciDciDevicePath = {
  gPciRootBridge,
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_PCI_DP,
      {
        (UINT8)(sizeof(PCI_DEVICE_PATH)),
        (UINT8)((sizeof(PCI_DEVICE_PATH)) >> 8)
      }
    },
    PCI_FUNCTION_NUMBER_PCH_XHCI,
    PCI_DEVICE_NUMBER_PCH_XHCI
  },

  {
    {
      MESSAGING_DEVICE_PATH,
      MSG_DCI_MB_DP,
      {
        (UINT8)(sizeof(DCI_MB_PATH)),
        (UINT8)((sizeof(DCI_MB_PATH)) >> 8)
      }
    },
    0,
  },
  {
    {
      MESSAGING_DEVICE_PATH,
      MSG_VENDOR_DP,
      {
        (UINT8)(sizeof(VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof(VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    DEVICE_PATH_MESSAGING_PC_ANSI
  },
  gEndEntire
};
#endif
//
// XHCI Controller Device Path (0x14, 0x00)
//
GLOBAL_REMOVE_IF_UNREFERENCED PLATFORM_ONBOARD_CONTROLLER_DEVICE_PATH  gPlatformOnboardXhciDevice = {
  gPciRootBridge,
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_PCI_DP,
      {
        (UINT8) (sizeof (PCI_DEVICE_PATH)),
        (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8)
      }
    },
    PCI_FUNCTION_NUMBER_PCH_XHCI,
    PCI_DEVICE_NUMBER_PCH_XHCI
  },
  gEndEntire
};

//
// TCSS XHCI Controller Device Path (0x0D, 0x00)
//
GLOBAL_REMOVE_IF_UNREFERENCED PLATFORM_ONBOARD_CONTROLLER_DEVICE_PATH  gPlatformTcssXhciDevice = {
  gPciRootBridge,
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_PCI_DP,
      {
        (UINT8) (sizeof (PCI_DEVICE_PATH)),
        (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8)
      }
    },
    0x0,
    0xD
  },
  gEndEntire
};

GLOBAL_REMOVE_IF_UNREFERENCED USB_CLASS_FORMAT_DEVICE_PATH gUsbClassMassStorageDevice = {
  {
    {
      MESSAGING_DEVICE_PATH,
      MSG_USB_CLASS_DP,
      {
        (UINT8) (sizeof (USB_CLASS_DEVICE_PATH)),
        (UINT8) ((sizeof (USB_CLASS_DEVICE_PATH)) >> 8)
      }
    },
    0xffff,  // VendorId
    0xffff,  // ProductId
    0x08,    // DeviceClass    - USB Mass Storage Class
    0x06,    // DeviceSubClass - SCSI Transparent Command Set
    0xff     // DeviceProtocol - Match any Device Protocol
  },
  gEndEntire
};

#if (FixedPcdGetBool(PcdVmdEnable) == 1)
GLOBAL_REMOVE_IF_UNREFERENCED PLATFORM_ONBOARD_CONTROLLER_DEVICE_PATH  gPlatformOnboardVmdDevice = {
  gPciRootBridge,
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_PCI_DP,
      {
        (UINT8) (sizeof (PCI_DEVICE_PATH)),
        (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8)
      }
    },
    0x0,
    0xE
  },
  gEndEntire
};
#endif

//
// Predefined platform connect sequence
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_DEVICE_PATH_PROTOCOL          *gPlatformConnectSequence[] = {
  //
  // Add VMD controller so that VMD devices can work
  //
#if (FixedPcdGetBool(PcdVmdEnable) == 1)
  (EFI_DEVICE_PATH_PROTOCOL *) &gPlatformOnboardVmdDevice,
#endif
  //
  // Add USB so that USB devices can work
  //
  (EFI_DEVICE_PATH_PROTOCOL *) &gPlatformOnboardXhciDevice,

  (EFI_DEVICE_PATH_PROTOCOL *) &gPlatformTcssXhciDevice,

  NULL
};

/**
  Update on board XHCI device path to meet silicon configuration
**/
VOID
UpdateOnboardXhciDevicePath (
  VOID
  )
{
  EFI_STATUS                   Status;
  PCI_ROOT_BRIDGE              *PciRootBridge;
  UINTN                        PciRootBridgeCount;
  UINTN                        HandleNum;
  UINTN                        Index;
  EFI_HANDLE                   *PciIoHandles;
  EFI_PCI_IO_PROTOCOL          *PciIo;
  EFI_DEVICE_PATH_PROTOCOL     *XhciDP;
  UINTN                        Segment;
  UINTN                        Bus;
  UINTN                        Device;
  UINTN                        Function;
  UINT8                        ClassCode[3];

  PciIoHandles       = NULL;
  PciRootBridge      = NULL;
  PciRootBridgeCount = 0;
  XhciDP             = NULL;

  DEBUG ((DEBUG_INFO, "%a enter\n", __FUNCTION__));

  PciRootBridge = PciHostBridgeGetRootBridges (&PciRootBridgeCount);
  if (PciRootBridge == NULL || PciRootBridgeCount == 0) {
    return;
  }

  if (PciRootBridgeCount == 1) {
    DEBUG ((DEBUG_INFO, "%a exit with only 1 root bridge, XHCI device path update is not required\n", __FUNCTION__));
    PciHostBridgeFreeRootBridges (PciRootBridge, PciRootBridgeCount);
    FreePool (PciRootBridge);
    return;
  }

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &HandleNum,
                  &PciIoHandles
                  );
  if (EFI_ERROR (Status)) {
    PciHostBridgeFreeRootBridges (PciRootBridge, PciRootBridgeCount);
    FreePool (PciRootBridge);
    return;
  }

  for (Index = 0; Index < HandleNum; Index++) {
    Status = gBS->HandleProtocol (PciIoHandles[Index], &gEfiPciIoProtocolGuid, (VOID **) &PciIo);
    if (EFI_ERROR (Status)) {
      continue;
    }

    Status = PciIo->GetLocation (PciIo, &Segment, &Bus, &Device, &Function);
    if (EFI_ERROR (Status)) {
      continue;
    }

    if (!IsInternalBus (PciRootBridge, PciRootBridgeCount, Bus) ||
        Device != PCI_DEVICE_NUMBER_PCH_XHCI ||
        Function != PCI_FUNCTION_NUMBER_PCH_XHCI) {
      continue;
    }

    Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint8, PCI_CLASSCODE_OFFSET, sizeof (ClassCode), ClassCode);
    if (EFI_ERROR (Status)) {
      continue;
    }

    if (ClassCode[1] == PCI_CLASS_SERIAL_USB && ClassCode[2] == PCI_CLASS_SERIAL) {
      XhciDP = DevicePathFromHandle (PciIoHandles[Index]);
      break;
    }
  }

  if (XhciDP == NULL) {
    DEBUG ((DEBUG_INFO, "Can't find onboard XHCI controller device path, %a exit\n", __FUNCTION__));
  } else {
    //
    // Update gPlatformOnboardXhciDevice content
    //
    CopyMem (&gPlatformOnboardXhciDevice, XhciDP, GetDevicePathSize (XhciDP));
    DEBUG ((DEBUG_INFO, "gPlatformOnboardXhciDevice update to = %s\n", ConvertDevicePathToText ((EFI_DEVICE_PATH_PROTOCOL *) &gPlatformOnboardXhciDevice, FALSE, FALSE)));
  }

  PciHostBridgeFreeRootBridges (PciRootBridge, PciRootBridgeCount);
  FreePool (PciRootBridge);
  FreePool (PciIoHandles);
  return;
}
