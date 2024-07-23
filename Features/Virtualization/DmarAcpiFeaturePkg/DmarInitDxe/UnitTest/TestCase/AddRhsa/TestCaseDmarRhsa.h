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
  EFI_ACPI_DMAR_RHSA_HEADER  Rhsa[3];
} UNIT_TEST_RHSA_TABLE_STRUCTURE;

#endif // _UT_DMAR_STRUCTURES_H_
