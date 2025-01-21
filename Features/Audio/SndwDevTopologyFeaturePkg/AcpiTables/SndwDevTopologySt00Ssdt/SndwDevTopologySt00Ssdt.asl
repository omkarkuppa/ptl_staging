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

DefinitionBlock ("SndwDevTopologySt00Ssdt.aml", "SSDT", 2, "INTEL", "St00Ssdt", 0x1000) {

  External(\_SB.PC00.HDAS.IDA.SNDW, DeviceObj)

  Scope (\_SB.PC00.HDAS.IDA.SNDW) {
    Device(SWD0) {
      Name (_ADR, 0x000030025D071101)
      Include ("SndwLegacyCodec.asl")
    }
  }
}
