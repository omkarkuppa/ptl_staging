/** @file
  SoundWire topology definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

DefinitionBlock ("SndwDevTopologySt07Ssdt.aml", "SSDT", 2, "INTEL", "St07Ssdt", 0x1000) {

  External (\_SB.PC00.HDAS.IDA.SNDW, DeviceObj)

  Scope (\_SB)
  {
    Device (AUDC)
    {
      Name (_HID, "ACPI0018")
      Include ("AudioComposition.asl")
    } //End AUDC
  }

  Scope (\_SB.PC00.HDAS.IDA.SNDW)
  {
    // SoundWire Device (SWDA) on Link #3 (Realtek ALC712VB : UID_00)
    // _ADR: Link_3 Bit[51:48]=3 | ALC1320_ID Bit[47:0]=30025D071201 | UID_00 Bit[43:40]=0
    Device (SWDA)
    {
      Name (_ADR, 0x000330025D071201)
      Include ("Peripheral_712VB.asl")
    }
  }
}
