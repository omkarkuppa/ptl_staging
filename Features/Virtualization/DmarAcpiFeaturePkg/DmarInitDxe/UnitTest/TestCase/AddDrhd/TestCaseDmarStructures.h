/** @file

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

#ifndef _UT_DMAR_STRUCTURES_H_
#define _UT_DMAR_STRUCTURES_H_

#include <IndustryStandard/DmaRemappingReportingTable.h>

typedef struct {
  EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER DeviceScopeStructureHeader;
  EFI_ACPI_DMAR_PCI_PATH                      PciPath; // device, function
} EFI_ACPI_DEV_SCOPE_STRUCTURE;

typedef struct {
  EFI_ACPI_DMAR_DRHD_HEADER     DrhdHeader;
  EFI_ACPI_DEV_SCOPE_STRUCTURE  DeviceScope[2];
} EFI_ACPI_DRHD_ENGINE1_STRUCT;

typedef struct {
  EFI_ACPI_DMAR_DRHD_HEADER     DrhdHeader;
  EFI_ACPI_DEV_SCOPE_STRUCTURE  DeviceScope[2];
} EFI_ACPI_DRHD_ENGINE2_STRUCT;

typedef struct {
  EFI_ACPI_DRHD_ENGINE1_STRUCT  Drhd1;
  EFI_ACPI_DRHD_ENGINE2_STRUCT  Drhd2;
} UNIT_TEST_DRHD_TABLE_STRUCTURE;

#endif // _UT_DMAR_STRUCTURES_H_
