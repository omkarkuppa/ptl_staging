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

#include "TestCaseDmarRmrr.h"

UNIT_TEST_RMRR_TABLE_STRUCTURE UnitTestDmarRmrrTable = {
  {
    //
    // EFI_ACPI_DMAR_RMRR_HEADER
    //
    {
      {
        EFI_ACPI_DMAR_TYPE_RMRR,
        sizeof (EFI_ACPI_RMRR1_STRUCT)
      },
      0,
      0,
      0,
      0x1234FEAD0000,
      0x1234FEAEFFFF,
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
        {2,
        sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE),
        0,
        0,
        0,
        0xA},
        {6, 1}
      }
    }
  },
  {
    //
    // EFI_ACPI_DMAR_RMRR_HEADER
    //
    {
      {
        EFI_ACPI_DMAR_TYPE_RMRR,
        sizeof (EFI_ACPI_RMRR2_STRUCT)
      },
      0,
      0,
      1,
      0xBEAF02000000,
      0xBEAF05FFFFFF,
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
    }
  },
  {
    //
    // EFI_ACPI_DMAR_RMRR_HEADER
    //
    {
      {
        EFI_ACPI_DMAR_TYPE_RMRR,
        sizeof (EFI_ACPI_RMRR2_STRUCT)
      },
      0,
      0,
      1,
      0xA234FEAD0000,
      0xA234FEAEFFFF,
    },
    {
      {
        {1,
        sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE),
        0,
        0,
        0,
        0xB},
        {0xA, 0x1}
      },
    }
  }
};