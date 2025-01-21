/** @file
  VTIO DXE header file.

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

#ifndef _VTIO_DXE_H_
#define _VTIO_DXE_H_

#include <IndustryStandard/Acpi63.h>
#include <Protocol/AcpiTable.h>

///
/// Secure Devices table for VTIO - Exposes the Secure controllers - XHCI and MIPI to OS via this table
///
#define NUMBER_OF_SECURE_CONTROLLERS  6
#define SECURE_CONTROLLER_INDEX_CIO2  0
#define SECURE_CONTROLLER_INDEX_ISP   1
#define SECURE_CONTROLLER_INDEX_HECI  2
#define SECURE_CONTROLLER_INDEX_SPI1  3
#define SECURE_CONTROLLER_INDEX_SPI2  4
#define SECURE_CONTROLLER_INDEX_HXCI  5

#define MAXIMUM_SENSOR_ENTRIES        2
#define MAXIMUM_XHCI_DEVICES          2
#define MAXIMUM_PATH_NAME_SIZE        49

#define EKpub_HANDLE 0x81010001

#define R_VTIO_PCI_CAPABILITY         0xB0          // VTIO PCI Capability
#define R_PCI_CAPABILITY_ID_OFFSET    0x00          // PCI Capability ID
#define V_VSEC_CAPABILITY_ID          0x09          // Vendor Specific Capability

#define R_VTIO_DVSEC_ID_OFFSET        0x08          // DVSEC ID
#define V_VTIO_DVSEC_ID               0x0009        // VTIO DVSEC ID

#define R_VTIO_ALT_FUNCTION_OFFSET    0x0A          // VTIO Alternate Function Vector

//
// Key value for HMAC-SHA-256 to generate E
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8 mHmacSha256Key[8] = {
  0xde, 0xad, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef
};

#pragma pack(push, 1)

typedef struct {
  UINT16  SensorInfo;
} SENSOR_INFO;

typedef struct {
  UINT16                 Version;
  UINT16                 ControllerCap;
  UINT8                  AFN; // Alternate Function Number
  UINT8                  Rsvd;
  UINT16                 Rsvd1;
  UINT16                 NumberofSensors;
  UINT16                 SensorEntrySize;
  SENSOR_INFO            SensorArray[2];
} CAP_STRUCT;

typedef struct {
  EFI_ACPI_6_5_SDEV_STRUCTURE_PCIE_ENDPOINT_DEVICE  PcieDevice;
  UINT16                                            PciPath;
  CAP_STRUCT                                        CapabilityStruct; // Capability Structure
} SECURE_CONTROLLER;

typedef struct {
  UINT16                VendorID;
  UINT16                DeviceID;
  UINT16                Revision;
  UINT8                 InterfaceNumber;
} USB_HID_STRUCT;

typedef struct {
  UINT8                Class;
  UINT8                SubClass;
  UINT8                Protocol;
  UINT16               AcpiPathStringOffset;
  UINT16               AcpiPathStringLength;
  UINT64               FirmwareHash[4];
  UINT8                AcpiPathName[MAXIMUM_PATH_NAME_SIZE];
} USB_CID_STRUCT;

typedef struct {
  UINT8                  Type;
  UINT16                 Length;
  UINT8                  Revision;
  UINT8                  Attributes;
  UINT8                  RootPortNumber;
  USB_HID_STRUCT         USBHidStruct;
  USB_CID_STRUCT         USBCidStruct;
} XHCI_SECURE_DEVICE_DESC;

typedef struct {
  UINT8                  Type;
  UINT16                 Length;
  UINT16                 ControllerCapability;
  UINT8                  AlternateFunctionNumber;
  UINT16                 Reserved1;
  UINT16                 Reserved2;
} XHCI_VENDOR_DEVICE_DESC;

typedef struct {
  EFI_ACPI_6_5_SDEV_STRUCTURE_ACPI_NAMESPACE_DEVICE    AcpiDevice;
  UINT8                                                AcpiPathName[MAXIMUM_PATH_NAME_SIZE];
} SDEV_USB_DEVICE_STRUCT;


typedef struct {
  EFI_ACPI_6_5_SDEV_STRUCTURE_PCIE_ENDPOINT_DEVICE    PcieDevice;
  UINT16                                              PciPath;
} XHCI_SECURE_CONTROLLER;

typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER         Header;
  SECURE_CONTROLLER               SecureController[NUMBER_OF_SECURE_CONTROLLERS];
} VTIO_ACPI_TABLE;

#pragma pack(pop)

/**
  Main entry for this driver.

  @param ImageHandle     Image handle this driver.
  @param SystemTable     Pointer to SystemTable.

  @retval EFI_SUCESS     This function always complete successfully.
**/
EFI_STATUS
EFIAPI
VtioDxeEntryPoint (
  IN EFI_HANDLE,
  IN EFI_SYSTEM_TABLE*
  );

#endif
