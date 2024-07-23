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

  External(\_SB.PC00.HDAS.IDA.SNDW, DeviceObj)

    Scope (\_SB)
    {
        Device(AUDC) {
            Name(_HID, "ACPI0018")
            Include("AggregationAudioComposition.asl")
        } //End AUDC
    }

    Scope (\_SB.PC00.HDAS.IDA.SNDW)
    {
        // SoundWire Device #0 on Controller #0 (Realtek ALC711)
        // _ADR: (V_HDA_SNDW_LINK_ID_0 | V_HDA_SNDW_DID_REALTEK_ALC711)
        Device(SWD0)
        {
            Name (_ADR, 0x000030025d071101)
            Include ("Peripheral_711.asl")
        }

        // SoundWire Device #1 on Controller #3 (Realtek ALC1316 UID:1)
        // _ADR: (V_HDA_SNDW_LINK_ID_3 | V_HDA_SNDW_DID_REALTEK_ALC1316)
        Device(SWD1)
        {
            Name (_ADR, 0x000331025d131601)
            Include ("Peripheral_1316_SPK1.asl")
        }

        // SoundWire Device #2 on Controller #2 (Realtek ALC1316 UID:0)
        // _ADR: (V_HDA_SNDW_LINK_ID_2 | V_HDA_SNDW_DID_REALTEK_ALC1316)
        Device(SWD2)
        {
            Name (_ADR, 0x000230025d131601)
            Include ("Peripheral_1316_SPK2.asl")
        }

        // SoundWire Device #3 on Controller #1 (Realtek ALC714)
        // _ADR: (V_HDA_SNDW_LINK_ID_1 | V_HDA_SNDW_DID_REALTEK_ALC714)
        Device(SWD3)
        {
            Name (_ADR, 0x000130025d071401)
            Include ("Peripheral_714.asl")
        }
    }
}