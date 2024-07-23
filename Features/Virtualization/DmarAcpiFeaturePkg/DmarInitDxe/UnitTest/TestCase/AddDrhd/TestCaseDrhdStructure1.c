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

#include "TestCaseDmarStructures.h"

UNIT_TEST_DRHD_TABLE_STRUCTURE UnitTest1DmarTable = {
  {
    {
      {
        0,
        sizeof (EFI_ACPI_DRHD_ENGINE1_STRUCT),
      },
      0,
      4,
      0,
      0xDEADBEEF0000,
    },
    {
      {
        {1,
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
      },
    }
  },
  {
    {
      {
        0,
        sizeof (EFI_ACPI_DRHD_ENGINE2_STRUCT)
      },
      1,
      0,
      0,
      0xDEADBEEF1000,
    },
    {
      {
        {
          3,
          sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE),
          0,
          0,
          2,
          0
        },
        {0x15, 0x01}
      },
      {
        {
          4,
          sizeof (EFI_ACPI_DEV_SCOPE_STRUCTURE),
          0,
          0,
          0,
          0
        },
        {0x15, 0x02}
      }
    }
  }
};