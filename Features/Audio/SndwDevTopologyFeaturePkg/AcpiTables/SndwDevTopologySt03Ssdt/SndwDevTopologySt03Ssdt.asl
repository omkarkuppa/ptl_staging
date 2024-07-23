/** @file
  SoundWire topology definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

DefinitionBlock ("SndwDevTopologySt03Ssdt.aml", "SSDT", 2, "INTEL", "St03Ssdt", 0x1000) {

  External(\_SB.PC00.HDAS.IDA.SNDW, DeviceObj)

  Scope (\_SB.PC00.HDAS.IDA.SNDW)
  {
    // SoundWire Device #0 on Link #3 (Realtek ALC713-VB UAJ/HID/MIC)
    // _ADR: (V_HDA_SNDW_LINK_ID_3 | V_HDA_SNDW_DID_REALTEK_ALC713VB | UID:0)
    Device(SWD0)
    {
      Name (_ADR, 0x000330025D071301)
      Include ("Peripheral_713VB.asl")
    }

    // SoundWire Device #1 on Link #1 (Realtek ALC1320 AMP1 for aggregated left speakers)
    // _ADR: (V_HDA_SNDW_LINK_ID_1 | V_HDA_SNDW_DID_REALTEK_ALC1320 | UID:0)
    Device(SWD1)
    {
      Name (_ADR, 0x000130025D132001)
      Include ("Peripheral_1320_SPK1.asl")
    }

    // SoundWire Device #2 on Link #2 (Realtek ALC1320 AMP2 for aggregated right speakers)
    // _ADR: (V_HDA_SNDW_LINK_ID_2 | V_HDA_SNDW_DID_REALTEK_ALC1320 | UID:2)
    Device(SWD2)
    {
      Name (_ADR, 0x000232025D132001)
      Include ("Peripheral_1320_SPK2.asl")
    }
  }
}
