/** @file
  PCIe drives ACPI RTD3 support for VMD RAID

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

ACPI_DEBUG_EXTERNAL_REFERENCE

//
// SSDT imports
//
External (VLON, MethodObj)
External (VLOF, MethodObj)
External (VD3A, MethodObj)
External (\RTD3, FieldUnitObj) // Global RTD3 Support in BIOS setup menu
External (\STD3, FieldUnitObj) /// 0: D3 disable. 1: D3Hot support for Storages. 2: D3Cold support for Storages.
External (VV0S)
External (VV0D)
External (VV1S)
External (VV1D)
External (VV2S)
External (VV2D)
External (VV3S)
External (VV3D)

Device (VOL0) {
  Name (_ADR,0x0080FFFF)  // VMD RAID Volume 0

  If (LAnd (CondRefOf (\RTD3), CondRefOf (\STD3))) {
    If (LAnd (LNotEqual (\RTD3, 0), LEqual (\STD3, 0x02))) { // Is Global RTD3 support and RTD3 support for storage enabled in BIOS Setup?
      Method (_PR0) {
        Return (Package () {V0PR})
      }

      Method (_PR3) {
        Return (Package () {V0PR})
      }
    }
  }

  If (LAnd (CondRefOf (\RTD3), CondRefOf (\STD3))) {
    If (LAnd (LNotEqual (\RTD3, 0), LEqual (\STD3, 0x02))) { // Is Global RTD3 support and RTD3 support for storage enabled in BIOS Setup?
      PowerResource (V0PR, 4, 0) { // Power Resource Aggregate for RAID volume 0
        Name (_STA, 0x01)

        Method (_ON, 0) {
          ADBG ("VMD VOL0._ON")
          If (VD3A) {
            ^^^VLON (VV0S,VV0D)
            Store (0x1, _STA)
          } Else {
            ADBG ("Global RTD3 lock")
          }
        }

        Method (_OFF, 0) {
          ADBG ("VMD VOL0._OFF")
          If (VD3A) {
            ^^^VLOF (VV0S,VV0D)
            Store (0x0, _STA)
          } Else {
            ADBG ("Global RTD3 lock")
          }
        }
      }
    }
  }
}

Device (VOL1) {
  Name (_ADR,0x0081FFFF)  // VMD RAID Volume 1

  If (LAnd (CondRefOf (\RTD3), CondRefOf (\STD3))) {
    If (LAnd (LNotEqual (\RTD3, 0), LEqual (\STD3, 0x02))) { // Is Global RTD3 support and RTD3 support for storage enabled in BIOS Setup?
      Method (_PR0) {
        Return (Package () {V1PR})
      }

      Method (_PR3) {
        Return (Package () {V1PR})
      }
    }
  }

  If (LAnd (CondRefOf (\RTD3), CondRefOf (\STD3))) {
    If (LAnd (LNotEqual (\RTD3, 0), LEqual (\STD3, 0x02))) { // Is Global RTD3 support and RTD3 support for storage enabled in BIOS Setup?
      PowerResource (V1PR, 4, 0) { // Power Resource Aggregate for RAID volume 1
        Name (_STA, 0x01)

        Method (_ON, 0) {
          ADBG ("VMD VOL1._ON")
          If (VD3A) {
            ^^^VLON (VV1S,VV1D)
            Store (0x1, _STA)
          } Else {
            ADBG ("Global RTD3 lock")
          }
        }

        Method (_OFF, 0) {
          ADBG ("VMD VOL1._OFF")
          If (VD3A) {
            ^^^VLOF (VV1S,VV1D)
            Store (0x0, _STA)
          } Else {
            ADBG ("Global RTD3 lock")
          }
        }
      }
    }
  }
}

Device (VOL2) {
  Name (_ADR,0x0082FFFF)  // VMD RAID Volume 2

  If (LAnd (CondRefOf (\RTD3), CondRefOf (\STD3))) {
    If (LAnd (LNotEqual (\RTD3, 0), LEqual (\STD3, 0x02))) { // Is Global RTD3 support and RTD3 support for storage enabled in BIOS Setup?
      Method (_PR0) {
        Return (Package () {V2PR})
      }

      Method (_PR3) {
        Return (Package () {V2PR})
      }
    }
  }

  If (LAnd (CondRefOf (\RTD3), CondRefOf (\STD3))) {
    If (LAnd (LNotEqual (\RTD3, 0), LEqual (\STD3, 0x02))) { // Is Global RTD3 support and RTD3 support for storage enabled in BIOS Setup?
      PowerResource (V2PR, 4, 0) { // Power Resource Aggregate for RAID volume 2
        Name (_STA, 0x01)

        Method (_ON, 0) {
          ADBG ("VMD VOL2._ON")
          If (VD3A) {
            ^^^VLON (VV2S,VV2D)
            Store (0x1, _STA)
          } Else {
            ADBG ("Global RTD3 lock")
          }
        }

        Method (_OFF, 0) {
          ADBG ("VMD VOL2._OFF")
          If (VD3A) {
            ^^^VLOF (VV2S,VV2D)
            Store (0x0, _STA)
          } Else {
            ADBG ("Global RTD3 lock")
          }
        }
      }
    }
  }
}

Device (VOL3) {
  Name (_ADR,0x0083FFFF)  // VMD RAID Volume 4

  If (LAnd (CondRefOf (\RTD3), CondRefOf (\STD3))) {
    If (LAnd (LNotEqual (\RTD3, 0), LEqual (\STD3, 0x02))) { // Is Global RTD3 support and RTD3 support for storage enabled in BIOS Setup?
      Method (_PR0) {
        Return (Package () {V3PR})
      }

      Method (_PR3) {
        Return (Package () {V3PR})
      }
    }
  }

  If (LAnd (CondRefOf (\RTD3), CondRefOf (\STD3))) {
    If (LAnd (LNotEqual (\RTD3, 0), LEqual (\STD3, 0x02))) { // Is Global RTD3 support and RTD3 support for storage enabled in BIOS Setup?
      PowerResource (V3PR, 4, 0) { // Power Resource Aggregate for RAID volume 3
        Name (_STA, 0x01)

        Method (_ON, 0) {
          ADBG ("VMD VOL3._ON")
          If (VD3A) {
            ^^^VLON (VV3S,VV3D)
            Store (0x1, _STA)
          } Else {
            ADBG ("Global RTD3 lock")
          }
        }

        Method (_OFF, 0) {
          ADBG ("VMD VOL3._OFF")
          If (VD3A) {
            ^^^VLOF (VV3S,VV3D)
            Store (0x0, _STA)
          } Else {
            ADBG ("Global RTD3 lock")
          }
        }
      }
    }
  }
}
