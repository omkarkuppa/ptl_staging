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

#include "TestCaseDmarAtsr.h"

UNIT_TEST_ATSR_TABLE_STRUCTURE UnitTestDmarAtsrTable = {
  //
  // EFI_ACPI_ATSR_SEGMENT0_STRUCT
  //
  {
    {
      {
        0x2,
        sizeof (EFI_ACPI_ATSR_SEGMENT0_STRUCT)
      },
      1,
      0,
      0,
    }
  },
  //
  // EFI_ACPI_ATSR_SEGMENT1_STRUCT
  //
  {
    {
      {
        0x2,
        sizeof (EFI_ACPI_ATSR_SEGMENT1_STRUCT)
      },
      0,
      0,
      1,
    },
    {
      {
        {2,
        sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE),
        0,
        0,
        0,
        0},
        {2, 0}
      },
      {
        {2,
        sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE),
        0,
        0,
        0,
        0},
        {11, 0}
      }
    }
  },
};