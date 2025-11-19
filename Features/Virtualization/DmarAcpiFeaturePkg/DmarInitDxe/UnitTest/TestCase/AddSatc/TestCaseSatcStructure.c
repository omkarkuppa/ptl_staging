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

#include "TestCaseDmarSatc.h"

UNIT_TEST_SATC_TABLE_STRUCTURE UnitTestDmarSatcTable = {
  {
    //
    // EFI_ACPI_DMAR_SATC_HEADER
    //
    {
      {
        0x5,
        sizeof (EFI_ACPI_SATC1_STRUCT)
      },
      1,
      0x00,
      0x0000,
    },
    {
      {
        {1,
        sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE),
        0,
        0,
        0,
        0xA},
        {5, 0}
      },
      {
        {1,
        sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE),
        0,
        0,
        0,
        0xA},
        {6, 0}
      },
      {
        {1,
        sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE),
        0,
        0,
        0,
        0xA},
        {7, 0}
      },
    }
  },
  {
    //
    // EFI_ACPI_DMAR_SATC_HEADER
    //
    {
      {
        0x5,
        sizeof (EFI_ACPI_SATC2_STRUCT)
      },
      0,
      0x00,
      0x0001,
    },
    {
      {
        {1,
        sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE),
        0,
        0,
        0,
        0},
        {0xA, 0}
      },
      {
        {1,
        sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE),
        0,
        0,
        0,
        0},
        {0xC, 1}
      },
    }
  },
  {
    //
    // EFI_ACPI_DMAR_SATC_HEADER
    //
    {
      {
        0x5,
        sizeof (EFI_ACPI_SATC3_STRUCT)
      },
      1,
      0x00,
      0x0001,
    },
    {
      {
        {1,
        sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE),
        0,
        0,
        0,
        0},
        {0xF, 0}
      },
    }
  },
};