/** @file
  ACPI RTD3 support for VMD remapped

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
// Power resource for NVMx (remapped) device.

//
// SSDT imports
//
External (VPON, MethodObj)
External (VPOF, MethodObj)
External (VR3D, MethodObj)
External (_ADR, MethodObj)
External (\STD3, FieldUnitObj)  // 0: D3 disable. 1: D3Hot support for Storages. 2: D3Cold support for Storages.

//
// The deepest D-state supported by this device in the S0 system sleeping state where the device can wake itself,
// "4" represents D3cold.
//
Method (_S0W, 0x0, NotSerialized)
{
  If (CondRefOf (\STD3)) {
    If (LEqual (\STD3, 0x02)) {
      ADBG (Concatenate ("_S0W - Storage D3 Status : ", ToHexString (\STD3)))
      Return (0x4)
    }
    Return (0x3)
  }
  Return (0x3)
}

If (CondRefOf (\STD3)) {
  If (LEqual (\STD3, 0x02)) { // Is RTD3 support for storage enabled in Bios Setup?
    Method (_PR0) {
      Return (Package(){NVPR})
    }

    Method (_PR3) {
      Return (Package(){NVPR})
    }
  }
}

If (CondRefOf (\STD3)) {
  If (LEqual (\STD3, 0x02)) { // Is RTD3 support for storage enabled in Bios Setup?
    PowerResource (NVPR, 4, 0) { // Power Resource for remapped devices
      Name (_STA, 0x01)

      Method (_ON, 0, Serialized) {
        //
        // If VMD RAID support has been added check if RTD3 isn't blocked - VR3D method
        // If RTD3 is blocked return.
        //
        If (LNot (VR3D())) {
          ADBG (Concatenate ("VMD PCIe ONr:", ToHexString(_ADR ())))
          VPON ()
          Store (0x1, _STA);
        }
      }

      Method (_OFF, 0, Serialized) {
        //
        // If VMD RAID support has been added check if RTD3 isn't blocked - VR3D method
        // If RTD3 is blocked return.
        //
        If (LNot (VR3D ())) {
          ADBG (Concatenate ("VMD PCIe OFFr:", ToHexString (_ADR ())))
          VPOF ()
          Store (0x0, _STA);
        }
      }
    }
  }
}
