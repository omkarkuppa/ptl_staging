/** @file
  This code defines ACPI DMA Remapping table related definitions.
  See the System Agent BIOS specification for definition of the table.

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
#ifndef _DMA_REMAPPING_TABLE_H_
#define _DMA_REMAPPING_TABLE_H_

#include <Uefi.h>
#include <Base.h>
#include <IndustryStandard/DmaRemappingReportingTable.h>
#include <IndustryStandard/Acpi.h>
#include <TcssInfo.h>

#pragma pack(1)
///
/// DMAR table signature
///
#define EFI_ACPI_VTD_DMAR_TABLE_SIGNATURE   0x52414D44  ///< "DMAR"
#define EFI_ACPI_DMAR_TABLE_REVISION        1
#define EFI_ACPI_DRHD_ENGINE_HEADER_LENGTH  0x10

typedef struct {
  EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER DeviceScopeStructureHeader;
  EFI_ACPI_DMAR_PCI_PATH                      PciPath;     // device, function
} EFI_ACPI_DEV_SCOPE_STRUCTURE;

typedef struct {
  EFI_ACPI_DMAR_DRHD_HEADER     DrhdHeader;
  EFI_ACPI_DEV_SCOPE_STRUCTURE  DeviceScope[1];
} EFI_ACPI_DRHD_ENGINE1_STRUCT;

typedef struct {
  EFI_ACPI_DMAR_DRHD_HEADER     DrhdHeader;
  EFI_ACPI_DEV_SCOPE_STRUCTURE  DeviceScope[5];
} EFI_ACPI_DRHD_ENGINE2_STRUCT;

typedef struct {
  EFI_ACPI_DMAR_DRHD_HEADER     DrhdHeader;
  EFI_ACPI_DEV_SCOPE_STRUCTURE  DeviceScope[2];
} EFI_ACPI_DRHD_ENGINE3_STRUCT;

typedef struct {
  EFI_ACPI_DMAR_SATC_HEADER     SatcHeader;
  EFI_ACPI_DEV_SCOPE_STRUCTURE  DeviceScope[5];
} EFI_ACPI_SATC_STRUCT;

typedef struct {
  EFI_ACPI_DMAR_SIDP_HEADER     SidpHeader;
  EFI_ACPI_DEV_SCOPE_STRUCTURE  DeviceScope[4];
} EFI_ACPI_SIDP_STRUCT;

typedef struct {
  EFI_ACPI_DMAR_HEADER          DmarHeader;
  EFI_ACPI_DRHD_ENGINE1_STRUCT  DrhdEngine1;
  EFI_ACPI_DRHD_ENGINE2_STRUCT  DrhdEngine2;
  EFI_ACPI_DRHD_ENGINE3_STRUCT  DrhdEngine3;
  EFI_ACPI_SATC_STRUCT          Satc;
  EFI_ACPI_SIDP_STRUCT          Sidp;
} EFI_ACPI_DMAR_TABLE;

#pragma pack()

#endif // _DMA_REMAPPING_TABLE_H_
