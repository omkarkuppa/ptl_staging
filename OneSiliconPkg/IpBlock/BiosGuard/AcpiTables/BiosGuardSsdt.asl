/** @file
  Intel Bios Guard SSDT ACPI Code.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#define TRAP_TYPE_BGD     0x04  // BIOS Guard

DefinitionBlock (
  "BIGSSDT.aml",
  "SSDT",
  0x02,
  "Intel",
  "BigSsdt",
  0x3000
  )
{
  External (\SMIF)
  External (\_SB.PC00, DeviceObj)

  Scope (\_SB)
  {
    //
    // Define BIOS Guard NVS Area operation region.
    //

    OperationRegion(BGNV,SystemMemory,0xFFFF0000,0xAA55)
    Field(BGNV,AnyAcc,Lock,Preserve)
    {
      Offset(0),      BGMA, 64, // Offset(0),     BIOS Guard Memory Address for Tool Interface
      Offset(8),      BGMS, 8,  // Offset(8),     BIOS Guard Memory Size for Tool Interface
      Offset(9),      BGIA, 8,  // Offset(9),     BIOS Guard SW SMI value for Tool Interface
    }
  }

  Scope(\_SB.PC00) {
    Method (PTMA) {
      Return (\_SB.BGMA)
    }

    Method (PTMS) {
      Return (\_SB.BGMS)
    }

    Method (PTIA) {
      Return (\_SB.BGIA)
    }
  }
} // End of Definition Block
