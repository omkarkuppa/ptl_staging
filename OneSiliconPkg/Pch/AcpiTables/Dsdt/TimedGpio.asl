/**@file
  ACPI definition for Timed GPIO controller

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
  //----------------------------
  //  Timed GPIO
  //----------------------------
#include <Register/PmcRegs.h>

Scope (\_SB) {
  Device (TGI0) {
    Name (_HID, "INTC1023")
    Name (_STR, Unicode ("Timed GPIO 1"))
    Method (_STA, 0x0, NotSerialized) {
      if (LEqual (TGEA, 0x01)) {
        Return (0x0F)
      } else {
        Return (0x00)
      }
    }
    Method (_CRS, 0x0, NotSerialized) {
      Name (TGRA,ResourceTemplate () {
        Memory32Fixed (ReadWrite, 0x0,0x38, TGA1)
      })
      CreateDWordField (TGRA,TGA1._BAS,TMH1)
      Store ( Add (PWRM,R_PMC_PWRM_TGPIOCTL0), TMH1)
      Return (TGRA)
    }
  } // END Device (TGI0)

  Device (TGI1) {
    Name (_HID, "INTC1024")
    Name (_STR, Unicode ("Timed GPIO 2"))
    Method (_STA, 0x0, NotSerialized) {
      if (LEqual (TGEB, 0x01)) {
        Return (0x0F)
      } else {
        Return (0x00)
      }
    }
    Method (_CRS, 0x0, NotSerialized) {
      Name (TGRB,ResourceTemplate () {
        Memory32Fixed (ReadWrite, 0x0,0x38, TGA2)
      })
      CreateDWordField (TGRB,TGA2._BAS,TMH2)
      Store ( Add (PWRM,R_PMC_PWRM_TGPIOCTL1), TMH2)
      Return (TGRB)
    }
  }// END Device (TGI1)
}// END Scope