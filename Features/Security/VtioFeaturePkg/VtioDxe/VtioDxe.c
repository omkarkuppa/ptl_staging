/** @file
 Entry point of VTIO DXE driver.

 This driver will install the VTIO ACPI table.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchPciBdfLib.h>
#include <IndustryStandard/Pci22.h>
#include <Guid/EventGroup.h>
#include <VtioSetup.h>
#include <VtioDxe.h>
#include <Uefi.h>

extern EFI_BOOT_SERVICES         *gBS;
extern EFI_RUNTIME_SERVICES      *gRT;
extern EFI_HANDLE                gImageHandle;

typedef enum {
  UINT_TYPE_8bit = 0,
  UINT_TYPE_16bit,
  UINT_TYPE_64bit
}VTIO_DATA_TYPE;

EFI_ACPI_6_5_SECURE_DEVICES_TABLE_HEADER mAcpiTemplate = {
  {
    EFI_ACPI_6_5_SECURE_DEVICES_TABLE_SIGNATURE,
    0,//sizeof (EFI_VTIO_ACPI_TABLE) - will be updated dynamically
    EFI_ACPI_6_5_SECURE_DEVICES_TABLE_REVISION,
    //
    // Compiler initializes the remaining bytes to 0
    // These fields should be filled in in production
    //
  }
};

SECURE_CONTROLLER SecureController[NUMBER_OF_SECURE_CONTROLLERS-1] = {
  {
    //
    // Secure Controller 1 Struct - CIO2 controller
    //
    {
      {
      0x01,     // Type
      0x00,     // Flags
      sizeof (SECURE_CONTROLLER),  // Length - To be updated before publishing ACPI table
      },
      0x0000,   // PCI Segment Number
      0x0000,   // Start Bus Number
      0x0010,   // PCI Path Offset
      0x0002,   // PCI Path Length
      0x0012,   // Vendor Specific Data Offset
      sizeof (CAP_STRUCT)  // Vendor Specific Data Length
    },
    0x0314,     // PCI Path
    {
      0x0100,   // Vendor Specific Data Version
      0x0000,   // Controller capability - TGL HW does not have support for BDF, MSI separation and HW crypto capability
      0xFF,     // Alternate Function Number
      0x00,     // Reserved
      0x0000,   // Reserved 1
      0x0002,   // Number of sensors
      0x0002,   // Sensor Entry Size
      //
      // CIO2 controller Sensor Info
      //
      {
        {0x0001},  // Sensor Info - User facing RGB Camera
        {0x0089},  // Sensor Info - User facing IR Camera
    }}
  },

  {
    //
    // Secure Controller 2 Struct - ISP controller
    //
    {
      {
      0x01,     // Type
      0x00,     // Flags
      sizeof (SECURE_CONTROLLER),  // Length - To be updated before publishing ACPI table
      },
      0x0000,   // PCI Segment Number
      0x0000,   // Start Bus Number
      0x0010,   // PCI Path Offset
      0x0002,   // PCI Path Length
      0x0012,   // Vendor Specific Data Offset
      sizeof (CAP_STRUCT)  // Vendor Specific Data Length
    },
    0x0005,     // PCI Path
    {
      0x0100,   // Vendor Specific Data Version
      0x0000,   // Controller capability - TGL HW does not have support for BDF, MSI separation and HW crypto capability
      0xFF,     // Alternate Function Number
      0x00,     // Reserved
      0x0000,   // Reserved 1
      0x0002,   // Number of sensors
      0x0002,   // Sensor Entry Size
      //
      // ISP controller Sensor Info
      //
      {
        {0x0001},  // Sensor Info - User facing RGB Camera
        {0x0085},  // Sensor Info - User facing IR Camera
    }}
  },

  {
    //
    // Secure Controller 3 Struct - HECI controller
    //
    {
      {
      0x01,     // Type
      0x00,     // Flags
      sizeof (SECURE_CONTROLLER) - 4,  // Length - To be updated before publishing ACPI table
      },
      0x0000,   // PCI Segment Number
      0x0000,   // Start Bus Number
      0x0010,   // PCI Path Offset
      0x0002,   // PCI Path Length
      0x0012,   // Vendor Specific Data Offset
      sizeof (CAP_STRUCT) - 4  // Vendor Specific Data Length
    },
    0x0016,     // PCI Path
    {
      0x0100,   // Vendor Specific Data Version
      0x0000,   // Controller capability - TGL HW does not have support for BDF, MSI separation and HW crypto capability
      0xFF,     // Alternate Function Number
      0x00,     // Reserved
      0x0000,   // Reserved 1
      0x0000,   // Number of sensors
      0x0002,   // Sensor Entry Size
      //
      // HECI controller Sensor Info
      //
      {
        {0x0000},  // Dummy
        {0x0000},  // Dummy
    }}
  },

  {
    //
    // Secure Controller 4 Struct - SPI1 controller
    //
    {
      {
      0x01,     // Type
      0x00,     // Flags
      sizeof (SECURE_CONTROLLER) - 2,  // Length - To be updated before publishing ACPI table
      },
      0x0000,   // PCI Segment Number
      0x0000,   // Start Bus Number
      0x0010,   // PCI Path Offset
      0x0002,   // PCI Path Length
      0x0012,   // Vendor Specific Data Offset
      sizeof (CAP_STRUCT) - 2  // Vendor Specific Data Length
    },
    0x021E,     // PCI Path
    {
      0x0100,   // Vendor Specific Data Version
      0x0000,   // Controller capability - TGL HW does not have support for BDF, MSI separation and HW crypto capability
      0xFF,     // Alternate Function Number
      0x00,     // Reserved
      0x0000,   // Reserved 1
      0x0001,   // Number of sensors
      0x0002,   // Sensor Entry Size
      //
      // SPI1 controller Sensor Info
      //
      {
        {0x0001},  // Sensor
        {0x0000},  // Dummy
    }}
  },

  {
    //
    // Secure Controller 5 Struct - SPI2 controller
    //
    {
      {
      0x01,     // Type
      0x00,     // Flags
      sizeof (SECURE_CONTROLLER) - 2,  // Length - To be updated before publishing ACPI table
      },
      0x0000,   // PCI Segment Number
      0x0000,   // Start Bus Number
      0x0010,   // PCI Path Offset
      0x0002,   // PCI Path Length
      0x0012,   // Vendor Specific Data Offset
      sizeof (CAP_STRUCT) - 2  // Vendor Specific Data Length
    },
    0x031E,     // PCI Path
    {
      0x0100,   // Vendor Specific Data Version
      0x0000,   // Controller capability - TGL HW does not have support for BDF, MSI separation and HW crypto capability
      0xFF,     // Alternate Function Number
      0x00,     // Reserved
      0x0000,   // Reserved 1
      0x0001,   // Number of sensors
      0x0002,   // Sensor Entry Size
      //
      // SPI2 controller Sensor Info
      //
      {
        {0x0001},  // Sensor Info
        {0x0000},  // Dummy
    }}
  }
};

XHCI_SECURE_CONTROLLER XHCISecureController = {
  //
  // Secure Controller 6 Struct - XHCI controller
  //
  {
    {
    0x01,    // Type
    0x00,    // Flags
    0x007D,  // Length - This includes the length of Descriptor. To be updated before publishing ACPI table
    },
    0x0000,  // PCI Segment Number
    0x0000,  // Start Bus Number
    0x0010,  // PCI Path Offset
    0x0002,  // PCI Path Length
    0x0012,  // Vendor Specific Data Offset
    0x006B   // Vendor Specific Data Length. To be updated before publishing ACPI table
  },
  0x0014,  // PCI Path
};

XHCI_VENDOR_DEVICE_DESC XHCIVendorDescriptor = {
  //
  // XHCI Vendor Device Descriptor Struct
  //
  0x80,    // Type
  0x000A,  // Length - To be updated before publishing ACPI table
  0x0000,  // Controller Capability
  0xFF,    // Alternate function number - To be updated from Vendor-Specific Extended Capability (VSEC)
  0x0000,  // Reserved1
  0x0000,  // Reserved2
};

XHCI_SECURE_DEVICE_DESC RGBDescriptor = {
  //
  // XHCI Secure Device Descriptor Struct - RGB
  //
  0x00,       // Type
  0x0051,     // Length
  0x02,       // Revision
  0x00,       // Attributes
  0x00,       // Root port Number
  {
    0x0000,   // Vendor ID
    0x0000,   // Device ID
    0x0000,   // Revision
    0x00      // Interface Number
  },
  {
    0x0E,     // Class code
    0x01,     // Sub Class
    0x01,     // Protocol
    0x0034,   // ACPI Path string offset
    0x001D,   // ACPI Path string name
    {
      0x0,    // Firmware Hash [63:0]
      0x0,    // Firmware Hash [127:64]
      0x0,    // Firmware Hash [191:128]
      0x0,    // Firmware Hash [255:192]
    },
  "\\_SB.PC00.XHCI.RHUB.HS00.CRGB"  // ACPI Path name
  }
};

XHCI_SECURE_DEVICE_DESC IRDescriptor = {
  //
  // XHCI Secure Device Descriptor Struct - IR
  //
  0x00,      // Type
  0x0050,    // Length
  0x02,      // Revision
  0x00,      // Attributes
  0x01,      // Root port Number
  {
    0x0000,  // Vendor ID
    0x0000,  // Device ID
    0x0000,  // Revision
    0x00     // Interface Number
  },
  {
    0x0E,    // Class code
    0x01,    // Sub Class
    0x01,    // Protocol
    0x0034,  // ACPI Path string offset
    0x001C,  // ACPI Path string name
    {
      0x0,   // Firmware Hash [63:0]
      0x0,   // Firmware Hash [127:64]
      0x0,   // Firmware Hash [191:128]
      0x0,   // Firmware Hash [255:192]
    },
  "\\_SB.PC00.XHCI.RHUB.HS01.CIR"  // ACPI Path name
  }
};

SDEV_USB_DEVICE_STRUCT USBCameraRGB = {
  //
  // USB Device Struct for RGB Camera
  //
  {
    {
      0x00,    // Type
      0x00,    // Flags
      0x002D,  // Length - To be updated before publishing ACPI table
    },
    0x0010,  // Device Identifier Offset
    0x001D,  // Device Identifier Length
    0x0010,  // Vendor Specific Information Offset
    0x0000,  // Vendor Specific Information Length
    0x0000,  // Secure Access Components Offset
    0x0000   // Secure Access Components Length
  },
  "\\_SB.PC00.XHCI.RHUB.HS00.CRGB",  // Device Identifier
};

SDEV_USB_DEVICE_STRUCT USBCameraIR = {
  //
  // USB Device Struct for IR Camera
  //
  {
    {
      0x00,    // Type
      0x00,    // Flags
      0x002C,  // Length - To be updated before publishing ACPI table
    },
    0x0010,  // Device Identifier Offset
    0x001C,  // Device Identifier Length
    0x0010,  // Vendor Specific Information Offset
    0x0000,  // Vendor Specific Information Length
    0x0000,  // Secure Access Components Offset
    0x0000   // Secure Access Components Length
  },
  "\\_SB.PC00.XHCI.RHUB.HS01.CIR",  // Device Identifier
};

/**
  Update Secure Controller

  @param[in] SecureControllerIndex - Index of Secure Controller
  @param[in] NumberOfSensors       - Number of sensors for Secure Controller
  @param[in] Flags                 - Flags of Secure Controller
  @param[in] SensorEntry1          - Sensor information for first sensor
  @param[in] SensorEntry2          - Sensor information for second sensor
**/
VOID
UpdateSecureController (
  IN UINT16 SecureControllerIndex,
  IN UINT16 NumberOfSensors,
  IN UINT8  Flags,
  IN UINT16 SensorEntry1,
  IN UINT16 SensorEntry2
  )
{
  UINT16                         SensorEntryLengthUpdate;

  SensorEntryLengthUpdate      = 0;

  SecureController[SecureControllerIndex].CapabilityStruct.NumberofSensors = NumberOfSensors;
  SensorEntryLengthUpdate = (SecureController[SecureControllerIndex].CapabilityStruct.SensorEntrySize *
    (MAXIMUM_SENSOR_ENTRIES - SecureController[SecureControllerIndex].CapabilityStruct.NumberofSensors));
  SecureController[SecureControllerIndex].PcieDevice.Header.Length = sizeof (SECURE_CONTROLLER) - SensorEntryLengthUpdate;
  SecureController[SecureControllerIndex].PcieDevice.VendorSpecificDataLength = sizeof (CAP_STRUCT) - SensorEntryLengthUpdate;
  SecureController[SecureControllerIndex].PcieDevice.Header.Flags = Flags;
  SecureController[SecureControllerIndex].CapabilityStruct.SensorArray[0].SensorInfo = SensorEntry1;
  SecureController[SecureControllerIndex].CapabilityStruct.SensorArray[1].SensorInfo = SensorEntry2;

  DEBUG ((DEBUG_INFO, "Update Secure Controller %x\n", SecureControllerIndex));
  DEBUG ((DEBUG_INFO, "  Number of Sensors        = %x\n", SecureController[SecureControllerIndex].CapabilityStruct.NumberofSensors));
  DEBUG ((DEBUG_INFO, "  Flags                    = %x\n", SecureController[SecureControllerIndex].PcieDevice.Header.Flags));
  DEBUG ((DEBUG_INFO, "  Sensor Info              = %x, %x\n", SecureController[SecureControllerIndex].CapabilityStruct.SensorArray[0].SensorInfo,
                                                               SecureController[SecureControllerIndex].CapabilityStruct.SensorArray[1].SensorInfo));
  DEBUG ((DEBUG_INFO, "  Length                   = %x\n", SecureController[SecureControllerIndex].PcieDevice.Header.Length));
  DEBUG ((DEBUG_INFO, "  VendorSpecificDataLength   = %x\n", SecureController[SecureControllerIndex].PcieDevice.VendorSpecificDataLength));

  return;
}

/**
  Update xHCI Secure Controller

  @param[in] DeviceIndex           - Index of xHCI Secure Controller device
  @param[in] Flags                 - Flags of xHCI Secure Controller device
  @param[in] NumberOfDevices       - Number of xHCI Secure Controller devices
  @param[in] DeviceAttributes      - Attributes of xHCI Secure Controller device
  @param[in] RootPortNumber        - Root Port Number of xHCI Secure Controller device
  @param[in] VendorId              - Vendor ID of xHCI Secure Controller device
  @param[in] ProductId             - Product ID of xHCI Secure Controller device
  @param[in] Revision              - Revision of xHCI Secure Controller device
  @param[in] InterfaceNumber       - Interface Number of xHCI Secure Controller device
  @param[in] Class                 - Class of xHCI Secure Controller device
  @param[in] SubClass              - Subclass of xHCI Secure Controller device
  @param[in] Protocol              - Protocol of xHCI Secure Controller device
  @param[in] AcpiPathStringOffset  - ACPI Path String Offset of xHCI Secure Controller device
  @param[in] AcpiPathStringLength  - ACPI Path String Length of xHCI Secure Controller device
  @param[in] FirmwareHashDevice1   - Firmware Hash of xHCI Secure Controller device
  @param[in] FirmwareHashDevice2   - Firmware Hash of xHCI Secure Controller device
  @param[in] AcpiPathNameDevice1   - ACPI Path Name of xHCI Secure Controller device
  @param[in] AcpiPathNameDevice2   - ACPI Path Name of xHCI Secure Controller device
**/
VOID
UpdateXHCISecureController (
  IN UINT16  DeviceIndex,
  IN UINT8   NumberOfDevices,
  IN UINT8   Flags,
  IN UINT8   DeviceAttributes,
  IN UINT8   RootPortNumber,
  IN UINT16  VendorId,
  IN UINT16  ProductId,
  IN UINT16  Revision,
  IN UINT8   InterfaceNumber,
  IN UINT8   Class,
  IN UINT8   SubClass,
  IN UINT8   Protocol,
  IN UINT16  AcpiPathStringOffset,
  IN UINT16  AcpiPathStringLength,
  IN UINT64  FirmwareHashDevice1[],
  IN UINT64  FirmwareHashDevice2[],
  IN UINT16  AcpiPathNameDevice1[],
  IN UINT16  AcpiPathNameDevice2[]
  )
{
  UINT16                       Index;
  UINT8                        PathNameTemp[MAXIMUM_PATH_NAME_SIZE];
  UINT16                       DescriptorLengthTemp;
  XHCI_SECURE_DEVICE_DESC      *TempXhciDevice;
  SDEV_USB_DEVICE_STRUCT       *TempUsbDevice;

  Index                = 0;
  DescriptorLengthTemp = 0;
  ZeroMem (PathNameTemp, sizeof (PathNameTemp)/sizeof (UINT8));
  TempXhciDevice       = NULL;
  TempUsbDevice        = NULL;

  ///
  /// Select xHCI Secure Device
  ///
  if (DeviceIndex == 0) {
    TempXhciDevice = &RGBDescriptor;
    TempUsbDevice  = &USBCameraRGB;
  } else if (DeviceIndex == 1) {
    TempXhciDevice = &IRDescriptor;
    TempUsbDevice  = &USBCameraIR;
  } else {
    DEBUG ((DEBUG_ERROR, "Invalid xHCI Secure Device - %x\n", DeviceIndex));
    return;
  }

  ///
  /// Update Flag field for Secure Device Struture
  ///
  TempUsbDevice->AcpiDevice.Header.Flags                  = Flags;

  DEBUG ((DEBUG_INFO, "Update xHCI Secure Controller %x\n", DeviceIndex));
  if (DeviceIndex < NumberOfDevices) {
    DescriptorLengthTemp                              = TempXhciDevice->Length;
    TempXhciDevice->Length                            = (TempXhciDevice->Length - TempXhciDevice->USBCidStruct.AcpiPathStringLength + AcpiPathStringLength);
    TempXhciDevice->Attributes                        = DeviceAttributes;
    TempXhciDevice->RootPortNumber                    = RootPortNumber;
    TempXhciDevice->USBHidStruct.VendorID             = VendorId;
    TempXhciDevice->USBHidStruct.DeviceID             = ProductId;
    TempXhciDevice->USBHidStruct.Revision             = Revision;
    TempXhciDevice->USBHidStruct.InterfaceNumber      = InterfaceNumber;
    TempXhciDevice->USBCidStruct.Class                = Class;
    TempXhciDevice->USBCidStruct.SubClass             = SubClass;
    TempXhciDevice->USBCidStruct.Protocol             = Protocol;
    TempXhciDevice->USBCidStruct.AcpiPathStringOffset = AcpiPathStringOffset;
    TempXhciDevice->USBCidStruct.AcpiPathStringLength = AcpiPathStringLength;
    if (DeviceIndex == 0) {
      TempXhciDevice->USBCidStruct.FirmwareHash[0]    = FirmwareHashDevice1[0];
      TempXhciDevice->USBCidStruct.FirmwareHash[1]    = FirmwareHashDevice1[1];
      TempXhciDevice->USBCidStruct.FirmwareHash[2]    = FirmwareHashDevice1[2];
      TempXhciDevice->USBCidStruct.FirmwareHash[3]    = FirmwareHashDevice1[3];
    } else if (DeviceIndex == 1) {
      TempXhciDevice->USBCidStruct.FirmwareHash[0]    = FirmwareHashDevice2[0];
      TempXhciDevice->USBCidStruct.FirmwareHash[1]    = FirmwareHashDevice2[1];
      TempXhciDevice->USBCidStruct.FirmwareHash[2]    = FirmwareHashDevice2[2];
      TempXhciDevice->USBCidStruct.FirmwareHash[3]    = FirmwareHashDevice2[3];
    }

    ///
    /// Update USB CID Structure if ACPI Path Name has been modified
    ///
    if ((DeviceIndex == 0 && AcpiPathNameDevice1[0] != 0x0) || (DeviceIndex == 1 && AcpiPathNameDevice2[0] != 0x0)) {
      DEBUG ((DEBUG_INFO, "Current AcpiPathName\n"));
      for (Index = 0; Index < MAXIMUM_PATH_NAME_SIZE; Index++) {
        DEBUG ((DEBUG_INFO, "0x%02x ", TempXhciDevice->USBCidStruct.AcpiPathName[Index]));
      }
      DEBUG ((DEBUG_INFO, "\n"));

      ///
      /// Zero out current ACPI Path Name
      ///
      ZeroMem (TempXhciDevice->USBCidStruct.AcpiPathName, sizeof (TempXhciDevice->USBCidStruct.AcpiPathName)/sizeof (UINT8));

      ///
      /// Copy new ACPI Path Name to a temporary variable
      ///
      for (Index = 0; Index < AcpiPathStringLength; Index++) {
        if (DeviceIndex == 0) {
          PathNameTemp[Index] = (UINT8) AcpiPathNameDevice1[Index];
        } else if (DeviceIndex == 1) {
          PathNameTemp[Index] = (UINT8) AcpiPathNameDevice2[Index];
        }
      }

      ///
      /// Verify ACPI Path Name Length is correct
      /// Length is determined by the index of the NULL terminator
      ///
      Index = 0;
      while (PathNameTemp[Index] != 0x0) {
        Index++;
      }

      ///
      /// Update Descriptor Length and AcpiPathStringLength if needed
      ///
      if (AcpiPathStringLength != (Index + 1)) {
        DEBUG ((DEBUG_INFO, "Index %x contains NULL terminator\n", Index));
        DEBUG ((DEBUG_WARN, "AcpiPathStringLength is incorrect. Should be set to : %x \n", Index + 1));
        AcpiPathStringLength = Index + 1;

        TempXhciDevice->Length = (DescriptorLengthTemp - TempXhciDevice->USBCidStruct.AcpiPathStringLength + AcpiPathStringLength);
        DEBUG ((DEBUG_INFO, "Descriptor Length Updated: %x\n", TempXhciDevice->Length));
        TempXhciDevice->USBCidStruct.AcpiPathStringLength = AcpiPathStringLength;
        DEBUG ((DEBUG_INFO, "AcpiPathStringLength Updated: %x\n", TempXhciDevice->USBCidStruct.AcpiPathStringLength));
      }

      ///
      /// Update ACPI Path Name with new value
      ///
      CopyMem (TempXhciDevice->USBCidStruct.AcpiPathName, PathNameTemp, sizeof (PathNameTemp)/sizeof (UINT8));
      DEBUG ((DEBUG_INFO, "Updated AcpiPathName\n"));
      for (Index = 0; Index < MAXIMUM_PATH_NAME_SIZE; Index++) {
        DEBUG ((DEBUG_INFO, "0x%02x ", TempXhciDevice->USBCidStruct.AcpiPathName[Index]));
      }
      DEBUG ((DEBUG_INFO, "\n"));

      ///
      /// Update the corresponding ACPI Controller Description
      ///
      TempUsbDevice->AcpiDevice.Header.Length                 = TempUsbDevice->AcpiDevice.Header.Length - TempUsbDevice->AcpiDevice.DeviceIdentifierLength + TempXhciDevice->USBCidStruct.AcpiPathStringLength;
      TempUsbDevice->AcpiDevice.DeviceIdentifierLength = TempXhciDevice->USBCidStruct.AcpiPathStringLength;
      ZeroMem (TempUsbDevice->AcpiPathName, sizeof (TempUsbDevice->AcpiPathName)/sizeof (UINT8));
      CopyMem (TempUsbDevice->AcpiPathName, TempXhciDevice->USBCidStruct.AcpiPathName, sizeof (TempXhciDevice->USBCidStruct.AcpiPathName)/sizeof (UINT8));
    }

    DEBUG ((DEBUG_INFO, "  Flags                                 = %x\n", TempUsbDevice->AcpiDevice.Header.Flags));
    DEBUG ((DEBUG_INFO, "  Length                                = %x\n", TempXhciDevice->Length));
    DEBUG ((DEBUG_INFO, "  Attributes                            = %x\n", TempXhciDevice->Attributes));
    DEBUG ((DEBUG_INFO, "  RootPortNumber                        = %x\n", TempXhciDevice->RootPortNumber));
    DEBUG ((DEBUG_INFO, "  USB Hardware ID - VendorID            = %x\n", TempXhciDevice->USBHidStruct.VendorID));
    DEBUG ((DEBUG_INFO, "  USB Hardware ID - Device ID           = %x\n", TempXhciDevice->USBHidStruct.DeviceID));
    DEBUG ((DEBUG_INFO, "  USB Hardware ID - Revision            = %x\n", TempXhciDevice->USBHidStruct.Revision));
    DEBUG ((DEBUG_INFO, "  USB Hardware ID - InterfaceNumber     = %x\n", TempXhciDevice->USBHidStruct.InterfaceNumber));
    DEBUG ((DEBUG_INFO, "  USB Compat ID - Class                 = %x\n", TempXhciDevice->USBCidStruct.Class));
    DEBUG ((DEBUG_INFO, "  USB Compat ID - SubClass              = %x\n", TempXhciDevice->USBCidStruct.SubClass));
    DEBUG ((DEBUG_INFO, "  USB Compat ID - Protocol              = %x\n", TempXhciDevice->USBCidStruct.Protocol));
    DEBUG ((DEBUG_INFO, "  USB Compat ID - AcpiPathStringOffset  = %x\n", TempXhciDevice->USBCidStruct.AcpiPathStringOffset));
    DEBUG ((DEBUG_INFO, "  USB Compat ID - FW Hash [255:192]     = %016llx\n", TempXhciDevice->USBCidStruct.FirmwareHash[3]));
    DEBUG ((DEBUG_INFO, "  USB Compat ID - FW Hash [191:128]     = %016llx\n", TempXhciDevice->USBCidStruct.FirmwareHash[2]));
    DEBUG ((DEBUG_INFO, "  USB Compat ID - FW Hash [127:64]      = %016llx\n", TempXhciDevice->USBCidStruct.FirmwareHash[1]));
    DEBUG ((DEBUG_INFO, "  USB Compat ID - FW Hash [63:0]        = %016llx\n", TempXhciDevice->USBCidStruct.FirmwareHash[0]));
    DEBUG ((DEBUG_INFO, "  USB Compat ID - AcpiPathStringLength  = %x\n", TempXhciDevice->USBCidStruct.AcpiPathStringLength));
    DEBUG ((DEBUG_INFO, "  USB Compat ID - AcpiPathName (Hex)    = "));

    for (Index = 0; Index < TempXhciDevice->USBCidStruct.AcpiPathStringLength; Index++) {
      DEBUG ((DEBUG_INFO, "0x%02x ", TempXhciDevice->USBCidStruct.AcpiPathName[Index]));
    }
    DEBUG ((DEBUG_INFO, "\n"));

    DEBUG ((DEBUG_INFO, "  USB Compat ID - AcpiPathName (String) = "));
    for (Index = 0; Index < TempXhciDevice->USBCidStruct.AcpiPathStringLength; Index++) {
      DEBUG ((DEBUG_INFO, "%c", TempXhciDevice->USBCidStruct.AcpiPathName[Index]));
    }
    DEBUG ((DEBUG_INFO, "\n"));
  } else {
    TempXhciDevice->Length = 0;
    DEBUG ((DEBUG_INFO, "  Length                               = %x\n", TempXhciDevice->Length));
    TempUsbDevice->AcpiDevice.Header.Length = 0;
  }

  return;
}

/**
  Update xHCI VTIO Alternate Function Vector from Vendor-Specific Extended Capability (VSEC)
**/
VOID
UpdateXhciAlternateFunctionVector (
  )
{
  UINT64              DeviceBase;
  UINT64              CapabilityAddress;

  DeviceBase        = 0;
  CapabilityAddress = 0;

  DEBUG ((DEBUG_ERROR, "Update xHCI VTIO Alternate Function Vector\n"));

  //
  // Find base address of the device
  //
  DeviceBase = PchXhciPciCfgBase ();
  if (PciSegmentRead16 (DeviceBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_ERROR, "xHCI PCI device is disabled\n"));
    return;
  }

  //
  // VTIO PCI Capability Address
  //
  CapabilityAddress = DeviceBase + R_VTIO_PCI_CAPABILITY;

  //
  // Verify PCI Standard Capability ID is for Vendor Specific Extended Capability
  //
  if (PciSegmentRead8 (CapabilityAddress + R_PCI_CAPABILITY_ID_OFFSET) != V_VSEC_CAPABILITY_ID) {
    DEBUG ((DEBUG_INFO, "VSEC is not supported. CapabilityAddress = 0x%x\n", CapabilityAddress));
    return;
  }

  //
  // Verify PCI Standard Capability ID is for Vendor Specific Extended Capability
  //
  if (PciSegmentRead16 (CapabilityAddress + R_VTIO_DVSEC_ID_OFFSET) != V_VTIO_DVSEC_ID) {
    DEBUG ((DEBUG_INFO, "VTIO DVSEC ID is not supported. CapabilityAddress = 0x%x\n", CapabilityAddress));
    return;
  }

  //
  // Update VTIO Alternate Function Vector
  //
  XHCIVendorDescriptor.AlternateFunctionNumber = PciSegmentRead8 (CapabilityAddress + R_VTIO_ALT_FUNCTION_OFFSET);
  DEBUG ((DEBUG_INFO, "  xHCI Vendor Descriptor - Alternate Function Number = 0x%x\n", XHCIVendorDescriptor.AlternateFunctionNumber));
}

/**
  Dump Array Contents

  @param[in] ArrPtr Pointer to the Array containing 16bit element.
  @param[in] Length Length of the Array.
  @param[in] Type   enum datatype.
**/
VOID
DebugArrayDmp (
  IN VOID *ArrPtr,
  IN UINT16 Length,
  IN VTIO_DATA_TYPE Type
  )
{
  switch (Type)
  {
    case UINT_TYPE_8bit:
    for (UINT16 i = 0; i < Length; i++)
    {
      DEBUG ((DEBUG_INFO, "%016llx ", ((UINT8 *)ArrPtr)[i]));
    }
      break;
    case UINT_TYPE_16bit:
    for (UINT16 i = 0; i < Length; i++)
    {
      DEBUG ((DEBUG_INFO, "%016llx ", ((UINT16 *)ArrPtr)[i]));
    }
      break;
    case UINT_TYPE_64bit:
    for (UINT16 i = 0; i < Length; i++)
    {
      DEBUG ((DEBUG_INFO, "%016llx ", ((UINT64 *)ArrPtr)[i]));
    }
      break;
    default:
      DEBUG ((DEBUG_ERROR, "Data Type Not Supported"));
      break;
  }
  DEBUG ((DEBUG_INFO, "\n"));
}

/**
  This function is used for printing VTIO setup data passed from VtioSetup driver

  @param[in]  VTIO_SETUP_DATA_STRUCT*   Pointer to VTIO_SETUP_DATA_STRUCT
**/
VOID
ShowConfig (
  IN VTIO_SETUP_DATA_STRUCT *Cfg
  )
{
  DEBUG ((DEBUG_INFO, "VtioSupport= %x\n",Cfg->VtioSupport));
  DEBUG ((DEBUG_INFO, "SdevCio2Entry= %x\n",Cfg->SdevCio2Entry));
  DEBUG ((DEBUG_INFO, "SdevIspEntry= %x\n",Cfg->SdevIspEntry));
  DEBUG ((DEBUG_INFO, "SdevHeciEntry= %x\n",Cfg->SdevHeciEntry));
  DEBUG ((DEBUG_INFO, "SdevSpi1Entry= %x\n",Cfg->SdevSpi1Entry));
  DEBUG ((DEBUG_INFO, "SdevSpi2Entry= %x\n",Cfg->SdevSpi2Entry));
  DEBUG ((DEBUG_INFO, "SdevXhciEntry= %x\n",Cfg->SdevXhciEntry));
  DEBUG ((DEBUG_INFO, "SdevNumberOfSensors= "));
  DebugArrayDmp (Cfg->SdevNumberOfSensors, 5, UINT_TYPE_16bit);
  DEBUG ((DEBUG_INFO, "SdevSensorEntry1= "));
  DebugArrayDmp (Cfg->SdevSensorEntry1, 5, UINT_TYPE_16bit);
  DEBUG ((DEBUG_INFO, "SdevSensorEntry2= "));
  DebugArrayDmp (Cfg->SdevSensorEntry2, 5, UINT_TYPE_16bit);
  DEBUG ((DEBUG_INFO, "SdevXhciNumberOfDevices= %x\n",Cfg->SdevXhciNumberOfDevices));
  DEBUG ((DEBUG_INFO, "SdevXhciDeviceAttributes= "));
  DebugArrayDmp (Cfg->SdevXhciDeviceAttributes, 2, UINT_TYPE_8bit);
  DEBUG ((DEBUG_INFO, "SdevXhciRootPortNumber= "));
  DebugArrayDmp (Cfg->SdevXhciRootPortNumber, 2, UINT_TYPE_8bit);
  DEBUG ((DEBUG_INFO, "SdevXhciVendorId= "));
  DebugArrayDmp (Cfg->SdevXhciVendorId, 2, UINT_TYPE_16bit);
  DEBUG ((DEBUG_INFO, "SdevXhciProductId= "));
  DebugArrayDmp (Cfg->SdevXhciProductId, 2, UINT_TYPE_16bit);
  DEBUG ((DEBUG_INFO, "SdevXhciRevision= "));
  DebugArrayDmp (Cfg->SdevXhciRevision, 2, UINT_TYPE_16bit);
  DEBUG ((DEBUG_INFO, "SdevXhciInterfaceNumber= "));
  DebugArrayDmp (Cfg->SdevXhciInterfaceNumber, 2, UINT_TYPE_8bit);
  DEBUG ((DEBUG_INFO, "SdevXhciClass= "));
  DebugArrayDmp (Cfg->SdevXhciClass, 2, UINT_TYPE_8bit);
  DEBUG ((DEBUG_INFO, "SdevXhciSubClass= "));
  DebugArrayDmp (Cfg->SdevXhciSubClass, 2, UINT_TYPE_8bit);
  DEBUG ((DEBUG_INFO, "SdevXhciProtocol= "));
  DebugArrayDmp (Cfg->SdevXhciProtocol, 2, UINT_TYPE_8bit);
  DEBUG ((DEBUG_INFO, "SdevXhciAcpiPathStringOffset= "));
  DebugArrayDmp (Cfg->SdevXhciAcpiPathStringOffset, 2, UINT_TYPE_16bit);
  DEBUG ((DEBUG_INFO, "SdevXhciAcpiPathStringLength= "));
  DebugArrayDmp (Cfg->SdevXhciAcpiPathStringLength, 2, UINT_TYPE_16bit);
  DEBUG ((DEBUG_INFO, "SdevXhciFirmwareHashDevice1= "));
  DebugArrayDmp (Cfg->SdevXhciFirmwareHashDevice1, 4, UINT_TYPE_64bit);
  DEBUG ((DEBUG_INFO, "SdevXhciFirmwareHashDevice2= "));
  DebugArrayDmp (Cfg->SdevXhciFirmwareHashDevice2, 4, UINT_TYPE_64bit);
  DEBUG ((DEBUG_INFO, "SdevXhciAcpiPathNameDevice1= %s\n",Cfg->SdevXhciAcpiPathNameDevice1));
  DEBUG ((DEBUG_INFO, "SdevXhciAcpiPathNameDevice2= %s\n",Cfg->SdevXhciAcpiPathNameDevice2));
  DEBUG ((DEBUG_INFO, "SdevFlags= "));
  DebugArrayDmp (Cfg->SdevFlags, 5, UINT_TYPE_8bit);
  DEBUG ((DEBUG_INFO, "SdevXhciFlags= %x\n",Cfg->SdevXhciFlags));
}

/**
  This function handles publishing of VTIO ACPI table at the end of DXE

  @param[in]  Event
  @param[in]  *Context
**/
VOID
EFIAPI
VtioInitialize (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS                     Status;
  BOOLEAN                        EnableControllerEntry[NUMBER_OF_SECURE_CONTROLLERS];
  UINT16                         ControllerNumber;
  EFI_ACPI_TABLE_PROTOCOL        *AcpiTable;
  UINTN                          TableKey;
  UINT8                          *VtioTable;
  UINT8                          *TempVtioTable;
  UINT32                         Length;
  UINT16                         XhciDeviceNumber;
  VTIO_SETUP_DATA_STRUCT         *VtioSetupData = NULL;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  VtioSetupData = (VTIO_SETUP_DATA_STRUCT*) PcdGet64 (PcdVtioSetupDataPtr);

  if (VtioSetupData == NULL) {
    DEBUG ((DEBUG_ERROR, "VTIO: Setup data passed from PcdVtioSetupDataPtr is NULL\n"));
    return;
  }

  if (!VtioSetupData->VtioSupport) {
    DEBUG ((DEBUG_ERROR, "VTIO: VTIO is disabled in BIOS setup - just return\n"));
    return;
  }

  ShowConfig (VtioSetupData);

  EnableControllerEntry[SECURE_CONTROLLER_INDEX_CIO2] = (BOOLEAN) VtioSetupData->SdevCio2Entry;
  EnableControllerEntry[SECURE_CONTROLLER_INDEX_ISP]  = (BOOLEAN) VtioSetupData->SdevIspEntry;
  EnableControllerEntry[SECURE_CONTROLLER_INDEX_HECI] = (BOOLEAN) VtioSetupData->SdevHeciEntry;
  EnableControllerEntry[SECURE_CONTROLLER_INDEX_SPI1] = (BOOLEAN) VtioSetupData->SdevSpi1Entry;
  EnableControllerEntry[SECURE_CONTROLLER_INDEX_SPI2] = (BOOLEAN) VtioSetupData->SdevSpi2Entry;
  EnableControllerEntry[SECURE_CONTROLLER_INDEX_HXCI] = (BOOLEAN) VtioSetupData->SdevXhciEntry;

  DEBUG ((DEBUG_INFO, "VtioSetupData->VtioSupport   = %x\n", VtioSetupData->VtioSupport));
  DEBUG ((DEBUG_INFO, "VtioSetupData->SdevCio2Entry = %x\n", VtioSetupData->SdevCio2Entry));
  DEBUG ((DEBUG_INFO, "VtioSetupData->SdevIspEntry  = %x\n", VtioSetupData->SdevIspEntry));
  DEBUG ((DEBUG_INFO, "VtioSetupData->SdevHeciEntry = %x\n", VtioSetupData->SdevHeciEntry));
  DEBUG ((DEBUG_INFO, "VtioSetupData->SdevSpi1Entry = %x\n", VtioSetupData->SdevSpi1Entry));
  DEBUG ((DEBUG_INFO, "VtioSetupData->SdevSpi2Entry = %x\n", VtioSetupData->SdevSpi2Entry));
  DEBUG ((DEBUG_INFO, "VtioSetupData->SdevXhciEntry = %x\n", VtioSetupData->SdevXhciEntry));

  Length = sizeof (EFI_ACPI_DESCRIPTION_HEADER);
  //
  // Exclude XHCI controller since it has a different structure
  //
  for (ControllerNumber = 0; ControllerNumber < NUMBER_OF_SECURE_CONTROLLERS-1; ControllerNumber++) {
    if (EnableControllerEntry[ControllerNumber]) {
      UpdateSecureController (ControllerNumber,
                                VtioSetupData->SdevNumberOfSensors[ControllerNumber],
                                VtioSetupData->SdevFlags[ControllerNumber],
                                VtioSetupData->SdevSensorEntry1[ControllerNumber],
                                VtioSetupData->SdevSensorEntry2[ControllerNumber]
                              );
      Length += SecureController[ControllerNumber].PcieDevice.Header.Length;
    }
  }

  //
  // Update xHCI Secure Controllers if enabled in BIOS setup
  //
  if (EnableControllerEntry[(SECURE_CONTROLLER_INDEX_HXCI)]) {
    UpdateXhciAlternateFunctionVector ();

    for (XhciDeviceNumber = 0; XhciDeviceNumber < MAXIMUM_XHCI_DEVICES; XhciDeviceNumber++) {
      UpdateXHCISecureController (
        XhciDeviceNumber,
        VtioSetupData->SdevXhciNumberOfDevices,
        VtioSetupData->SdevXhciFlags,
        VtioSetupData->SdevXhciDeviceAttributes[XhciDeviceNumber],
        VtioSetupData->SdevXhciRootPortNumber[XhciDeviceNumber],
        VtioSetupData->SdevXhciVendorId[XhciDeviceNumber],
        VtioSetupData->SdevXhciProductId[XhciDeviceNumber],
        VtioSetupData->SdevXhciRevision[XhciDeviceNumber],
        VtioSetupData->SdevXhciInterfaceNumber[XhciDeviceNumber],
        VtioSetupData->SdevXhciClass[XhciDeviceNumber],
        VtioSetupData->SdevXhciSubClass[XhciDeviceNumber],
        VtioSetupData->SdevXhciProtocol[XhciDeviceNumber],
        VtioSetupData->SdevXhciAcpiPathStringOffset[XhciDeviceNumber],
        VtioSetupData->SdevXhciAcpiPathStringLength[XhciDeviceNumber],
        VtioSetupData->SdevXhciFirmwareHashDevice1,
        VtioSetupData->SdevXhciFirmwareHashDevice2,
        VtioSetupData->SdevXhciAcpiPathNameDevice1,
        VtioSetupData->SdevXhciAcpiPathNameDevice2
        );

      XHCISecureController.PcieDevice.Header.Flags = VtioSetupData->SdevXhciFlags;

      XHCISecureController.PcieDevice.Header.Length = (sizeof (XHCISecureController) +
                                      XHCIVendorDescriptor.Length +
                                      RGBDescriptor.Length +
                                      IRDescriptor.Length
                                    );

      XHCISecureController.PcieDevice.VendorSpecificDataLength = (XHCIVendorDescriptor.Length +
                                                      RGBDescriptor.Length +
                                                      IRDescriptor.Length
                                                    );
    }

    //
    // Add the length of all XHCI structures
    //
    Length += (sizeof (XHCI_SECURE_CONTROLLER) +
                XHCIVendorDescriptor.Length +
                RGBDescriptor.Length +
                IRDescriptor.Length +
                USBCameraRGB.AcpiDevice.Header.Length +
                USBCameraIR.AcpiDevice.Header.Length
              );
  }

  VtioTable = AllocatePool (Length);

  if (VtioTable == NULL) {
    DEBUG ((DEBUG_ERROR, "VTIO: EFI out of resources\n"));
    return;
  }

  TempVtioTable = VtioTable;

  CopyMem (TempVtioTable, &mAcpiTemplate, sizeof (EFI_ACPI_DESCRIPTION_HEADER));
  TempVtioTable = TempVtioTable + sizeof (EFI_ACPI_DESCRIPTION_HEADER);
  for (ControllerNumber = 0; ControllerNumber < NUMBER_OF_SECURE_CONTROLLERS-1; ControllerNumber++) {
    if (EnableControllerEntry[ControllerNumber]){
      CopyMem (TempVtioTable, &SecureController[ControllerNumber], SecureController[ControllerNumber].PcieDevice.Header.Length);
      TempVtioTable = TempVtioTable + SecureController[ControllerNumber].PcieDevice.Header.Length;
    }
  }

  //
  // Copy all XHCI structures if it is enabled in BIOS setup
  //
  if (EnableControllerEntry[(SECURE_CONTROLLER_INDEX_HXCI)]) {
    CopyMem (TempVtioTable, &XHCISecureController, sizeof (XHCI_SECURE_CONTROLLER));
    TempVtioTable = TempVtioTable + sizeof (XHCI_SECURE_CONTROLLER);

    CopyMem (TempVtioTable, &XHCIVendorDescriptor, XHCIVendorDescriptor.Length);
    TempVtioTable = TempVtioTable + XHCIVendorDescriptor.Length;

    CopyMem (TempVtioTable, &RGBDescriptor, RGBDescriptor.Length);
    TempVtioTable = TempVtioTable + RGBDescriptor.Length;

    CopyMem (TempVtioTable, &IRDescriptor, IRDescriptor.Length);
    TempVtioTable = TempVtioTable + IRDescriptor.Length;

    CopyMem (TempVtioTable, &USBCameraRGB, USBCameraRGB.AcpiDevice.Header.Length);
    TempVtioTable = TempVtioTable + USBCameraRGB.AcpiDevice.Header.Length;

    CopyMem (TempVtioTable, &USBCameraIR, USBCameraIR.AcpiDevice.Header.Length);
    TempVtioTable = TempVtioTable + USBCameraIR.AcpiDevice.Header.Length;
  }

  ///
  /// Construct ACPI table
  ///
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **)&AcpiTable);
  ASSERT_EFI_ERROR (Status);

  ((EFI_ACPI_DESCRIPTION_HEADER *)(UINTN)(VtioTable))->Length = Length;

  Status = AcpiTable->InstallAcpiTable (
                        AcpiTable,
                        VtioTable,
                        ((EFI_ACPI_DESCRIPTION_HEADER *)VtioTable)->Length,
                        &TableKey
                        );
  ASSERT_EFI_ERROR (Status);
  if (VtioTable != NULL) {
    gBS->FreePool (VtioTable);
  }

  DEBUG ((DEBUG_INFO, "VTIO: Successfully installed VTIO SDEV table\n"));
  return;
}

/**
  Main entry for this driver.

  @param ImageHandle     Image handle this driver.
  @param SystemTable     Pointer to SystemTable.

  @retval EFI_SUCESS     This function always complete successfully.
**/
EFI_STATUS
EFIAPI
VtioDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                     Status;
  EFI_EVENT                      EndOfDxeEvent;

  DEBUG ((DEBUG_INFO, "VtioDxeEntryPoint\n"));

  if (!PcdGetBool (PcdIsVtdEnabled)) {
    DEBUG ((DEBUG_ERROR, "VTIO: VTD is not enabled in BIOS menu. Returning...\n"));
    return EFI_SUCCESS;
  }

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  VtioInitialize,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "VTIO: Error in creating event. Status = %d\n", Status));
    return Status;
  }

  return EFI_SUCCESS;
}
