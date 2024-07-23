/** @file
    This file describes the contents of the ACPI TSE reporting structure

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _TSE_ACPI_H_
#define _TSE_ACPI_H_

///
/// Include standard ACPI table definitions
///
#include <IndustryStandard/Acpi.h>
#include <Base.h>

#define EFI_TSE_ACPI_DESCRIPTION_TABLE_SUPPORTED_ENGINES FixedPcdGet8 (PcdTseEngines)

#pragma pack(1)

///
/// Definition for TSE PCI Path
///
typedef struct {
  UINT8    Device;
  UINT8    Function;
} EFI_TSE_ACPI_DEVICE_PCI_PATH;


#define TSE_DEVICE_TYPE_SPECIFIC_PCI_ROOT_PORT    0x1
#define TSE_DEVICE_TYPE_PCI_SUBHIERARCHY          0x2

///
/// TSE Device Scope Structure
///
typedef struct {
  UINT8                         Type;
  UINT8                         Length;
  UINT16                        Reserved;
  UINT16                        Reserved2;
  UINT8                         StartBusNumber;
  EFI_TSE_ACPI_DEVICE_PCI_PATH  *Path;
} EFI_TSE_ACPI_DEVICE_SCOPE_STRUCTURE;

///
/// TSE Capability Reporting
///
typedef struct {
  UINT16                              Type;
  UINT16                              Length;
  UINT8                               Flags;
  UINT8                               Revision : 4;
  UINT8                               NumberOfDeviceScopes : 4;
  UINT16                              SegmentNumber;
  UINT8                               KeySourcesSupported;
  UINT16                              EncryptionAlgoSupported;
  UINT16                              MaxKeysSupported;
  UINT8                               MaxBdfSupported;
  UINT8                               PageSizesSupported;
  UINT8                               LbaGranularity;
  UINT8                               DataTransferAlignment;
  UINT8                               CryptoTweakSize;
  UINT64                              IocceMmioBase;
  UINT64                              IocceMmioLimit;
  UINT64                              IocMmioBase;
  UINT64                              IocMmioLimit;
} EFI_TSE_ACPI_CAPABILITY_STRUCTURE;

#define EFI_TSE_ACPI_DESCRIPTION_TABLE_SIGNATURE         SIGNATURE_32 ('I', 'E', 'R', 'S')
#define EFI_TSE_ACPI_DESCRIPTION_TABLE_REVISION          1
#define EFI_TSE_ACPI_DESCRIPTION_TABLE_FLAGS             0x0

///
///  TSE Description Table
///
typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER       Header;
  UINT8                             NumberOfEngines;
  UINT8                             Flags;
  UINT8                             Reserved2[10];
  EFI_TSE_ACPI_CAPABILITY_STRUCTURE CapabilityStructures[EFI_TSE_ACPI_DESCRIPTION_TABLE_SUPPORTED_ENGINES];
} EFI_TSE_ACPI_DESCRIPTION_TABLE;

#pragma pack()

#endif // _TSE_ACPI_H_
