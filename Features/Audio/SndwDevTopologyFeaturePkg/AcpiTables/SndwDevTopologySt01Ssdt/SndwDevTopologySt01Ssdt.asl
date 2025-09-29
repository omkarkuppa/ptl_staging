/** @file
  SoundWire topology definitions

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

DefinitionBlock ("SndwDevTopologySt01Ssdt.aml", "SSDT", 2, "INTEL", "St01Ssdt", 0x1000) {

  External (\_SB.PC00.HDAS.IDA.SNDW, DeviceObj)

  Scope (\_SB)
  {
    Device (AUDC) {
      Name(_HID, "ACPI0018")
      Include("AudioComposition_722.asl")
    }
  }

  Scope (\_SB.PC00.HDAS.IDA.SNDW) {
    // SoundWire Device (SWD0) on Link #3 (Realtek ALC722 : UID_00)
    // _ADR: Link_3 Bit[51:48]=3 | ALC722_ID Bit[47:0]=30025d072201 | UID_00 Bit[43:40]=0
    Device (SWD0)
    {
      Name (_ADR, 0x000330025d072201)
      Include ("Peripheral_722.asl")
    }
  }
}
