/** @file
  This file contains all dTBT on PCH ASL codes.

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

@par Specification
**/

//
// PcdPchDTbtRpSupport is 32 bit fixed PCD used to control which codes should be built for the associated dTBT RP.
// Bit == 1 means the associated PCH RP may be used as dTBT RP in platform
// If only one bit is set, then only the codes corresponding to that bit is built.
// Bit is indexed by the number part withn RP ACPI device name, but not real root port number or SLOT value.
// For example , RP03 is represented by Bit 3 of PcdPchDTbtRpSupport
// DTBT_RP_NAME_NUM should be set to the number part of RP ACPI device name before including BarlowRidgePch.asl
//

//
// PCH Root Port with ACPI name Root Port 0
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 0))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 0)))))
  {
    #define DTBT_RP_NAME_NUM    0
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 1
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 1))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 1)))))
  {
    #define DTBT_RP_NAME_NUM    1
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 2
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 2))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 2)))))
  {
    #define DTBT_RP_NAME_NUM    2
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 3
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 3))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 3)))))
  {
    #define DTBT_RP_NAME_NUM    3
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 4
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 4))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 4)))))
  {
    #define DTBT_RP_NAME_NUM    4
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 5
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 5))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 5)))))
  {
    #define DTBT_RP_NAME_NUM    5
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 6
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 6))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 6)))))
  {
    #define DTBT_RP_NAME_NUM    6
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 7
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 7))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 7)))))
  {
    #define DTBT_RP_NAME_NUM    7
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 8
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 8))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 8)))))
  {
    #define DTBT_RP_NAME_NUM    8
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 9
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 9))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 9)))))
  {
    #define DTBT_RP_NAME_NUM    9
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 10
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 10))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 10)))))
  {
    #define DTBT_RP_NAME_NUM    10
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 11
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 11))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 11)))))
  {
    #define DTBT_RP_NAME_NUM    11
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 12
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 12))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 12)))))
  {
    #define DTBT_RP_NAME_NUM    12
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 13
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 13))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 13)))))
  {
    #define DTBT_RP_NAME_NUM    13
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 14
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 14))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 14)))))
  {
    #define DTBT_RP_NAME_NUM    14
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 15
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 15))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 15)))))
  {
    #define DTBT_RP_NAME_NUM    15
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 16
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 16))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 16)))))
  {
    #define DTBT_RP_NAME_NUM    16
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 17
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 17))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 17)))))
  {
    #define DTBT_RP_NAME_NUM    17
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 18
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 18))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 18)))))
  {
    #define DTBT_RP_NAME_NUM    18
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 19
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 19))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 19)))))
  {
    #define DTBT_RP_NAME_NUM    19
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 20
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 20))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 20)))))
  {
    #define DTBT_RP_NAME_NUM    20
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 21
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 21))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 21)))))
  {
    #define DTBT_RP_NAME_NUM    21
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 22
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 22))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 22)))))
  {
    #define DTBT_RP_NAME_NUM    22
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 23
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 23))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 23)))))
  {
    #define DTBT_RP_NAME_NUM    23
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 24
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 24))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 24)))))
  {
    #define DTBT_RP_NAME_NUM    24
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 25
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 25))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 25)))))
  {
    #define DTBT_RP_NAME_NUM    25
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 26
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 26))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 26)))))
  {
    #define DTBT_RP_NAME_NUM    26
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 27
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 27))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 27)))))
  {
    #define DTBT_RP_NAME_NUM    27
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 28
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 28))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 28)))))
  {
    #define DTBT_RP_NAME_NUM    28
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 29
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 29))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 29)))))
  {
    #define DTBT_RP_NAME_NUM    29
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 30
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 30))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 30)))))
  {
    #define DTBT_RP_NAME_NUM    30
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif

//
// PCH Root Port with ACPI name Root Port 31
//
#if (FixedPcdGet32(PcdPchDTbtRpSupport)) != 0
  If (LOr (LAnd (LEqual (RPN0, 1), LAnd (LEqual (RPT0, PCIE_RP_TYPE_PCH), LEqual (RPI0, 31))),
           LAnd (LEqual (RPN1, 1), LAnd (LEqual (RPT1, PCIE_RP_TYPE_PCH), LEqual (RPI1, 31)))))
  {
    #define DTBT_RP_NAME_NUM    31
    Include ("BarlowRidgePch.asl")
    #undef DTBT_RP_NAME_NUM
  }
#endif
