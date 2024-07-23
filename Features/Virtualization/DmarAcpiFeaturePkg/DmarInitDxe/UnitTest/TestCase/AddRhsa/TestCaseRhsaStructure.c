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

#include "TestCaseDmarRhsa.h"

UNIT_TEST_RHSA_TABLE_STRUCTURE UnitTestDmarRhsaTable = {
  //
  // EFI_ACPI_DMAR_RHSA_HEADER
  //
  {
    {
      {
        0x3,
        sizeof (EFI_ACPI_DMAR_RHSA_HEADER)
      },
      {0, 0, 0, 0},
      0xDEADBEEF0000,
      2,
    },
    //
    // EFI_ACPI_DMAR_RHSA_HEADER
    //
    {
      {
        0x3,
        sizeof (EFI_ACPI_DMAR_RHSA_HEADER)
      },
      {0, 0, 0, 0},
      0xDEADBEF00000,
      1,
    },
    //
    // EFI_ACPI_DMAR_RHSA_HEADER
    //
    {
      {
        0x3,
        sizeof (EFI_ACPI_DMAR_RHSA_HEADER)
      },
      {0, 0, 0, 0},
      0xDEADBEF11000,
      0,
    }
  }
};