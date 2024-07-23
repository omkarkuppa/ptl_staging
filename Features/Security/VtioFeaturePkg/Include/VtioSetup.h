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

#ifndef _VTIO_SETUP_H_
#define _VTIO_SETUP_H_

#define VTIO_FORMSET_GUID                         { 0xb318a3fb, 0xc98c, 0x43f4, { 0x86, 0x55, 0xc7, 0x61, 0x33, 0xac, 0xde, 0x44 } }

#define PLATFORM_VTIO_FORM_ID                     5420
#define KEY_SdevXhciAcpiPathNameDevice1           5436
#define KEY_SdevXhciAcpiPathNameDevice2           5437
#define PLATFORM_VTIO_SETUP_DATA_ID               5575

#ifndef AUTO_ID
#define AUTO_ID(x)                                x
#endif

#define VTIO_CFG_VARIABLE_NAME                    L"VtioCfg"
#define DEVICE_PATH_NAME1                         L"\\_SB.PC00.XHCI.RHUB.HS00.CRGB"
#define DEVICE_PATH_NAME2                         L"\\_SB.PC00.XHCI.RHUB.HS01.CIR"

#define VTIO_SETUP_REVISION                       1

#pragma pack(push, 1)

/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
**/
typedef struct {
  UINT8                               Revision;
  UINT8                               VtioSupport;
  UINT8                               SdevCio2Entry;
  UINT8                               SdevIspEntry;
  UINT8                               SdevHeciEntry;
  UINT8                               SdevSpi1Entry;
  UINT8                               SdevSpi2Entry;
  UINT8                               SdevXhciEntry;
  UINT16                              SdevNumberOfSensors[5];
  UINT16                              SdevSensorEntry1[5];
  UINT16                              SdevSensorEntry2[5];
  UINT8                               SdevXhciNumberOfDevices;
  UINT8                               SdevXhciDeviceAttributes[2];
  UINT8                               SdevXhciRootPortNumber[2];
  UINT16                              SdevXhciVendorId[2];
  UINT16                              SdevXhciProductId[2];
  UINT16                              SdevXhciRevision[2];
  UINT8                               SdevXhciInterfaceNumber[2];
  UINT8                               SdevXhciClass[2];
  UINT8                               SdevXhciSubClass[2];
  UINT8                               SdevXhciProtocol[2];
  UINT16                              SdevXhciAcpiPathStringOffset[2];
  UINT16                              SdevXhciAcpiPathStringLength[2];
  UINT64                              SdevXhciFirmwareHashDevice1[4];
  UINT64                              SdevXhciFirmwareHashDevice2[4];
  UINT16                              SdevXhciAcpiPathNameDevice1[49]; // 48 characters + NULL termination
  UINT16                              SdevXhciAcpiPathNameDevice2[49]; // 48 characters + NULL termination
  UINT8                               SdevFlags[5];
  UINT8                               SdevXhciFlags;
  UINT8                               Reserved[3];                     // Padding added for DWORD alignment
} VTIO_SETUP_DATA_STRUCT;

#pragma pack(pop)

#endif
