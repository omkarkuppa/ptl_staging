/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#define HDAS_PATH                      \_SB.PC00.HDAS
//
// High Definition Audio Controller - Device 31, Function 3
//
Device (HDAS)
{
  Name (_ADR, 0x001F0003)

  // D0 Method for HD-A Controller
  Method (_PS0, 0, Serialized)
  {
    ADBG ("HD-A Ctrlr D0")

    \_SB.S023 (0, 1)
    //
    // Call platform HDAS PS0 method if present
    //
    If (CondRefOf (\_SB.PC00.HDAS.PS0X)) {
      \_SB.PC00.HDAS.PS0X ()
    }
  }

  // D3 Method for HD-A Controller
  Method (_PS3, 0, Serialized)
  {
    ADBG ("HD-A Ctrlr D3")

    \_SB.S023 (0, 0)
    //
    // Call platform HDAS PS3 method if present
    //
    If (CondRefOf (\_SB.PC00.HDAS.PS3X)) {
      \_SB.PC00.HDAS.PS3X ()
    }
  }

  Method (_PRW, 0) {
    If (CondRefOf (GP1E)){
      If (LNotEqual (GP1E, 0)) {
        Return (GPRW (GPE1_ACE_PME_B0, 4))
      }
    }
    Return (GPRW (GPE_PME_B0, 4)) // can wakeup from S4 state
  }

  include ("Hda.asl")
  include ("HdaIda.asl")
}

Scope (HDAS.IDA) {
  //
  // High Definition Audio - SoundWire Controller
  //
  include ("HdaSoundWireCtrl.asl")

  If (LNotEqual (UAOE,0)) {
    //
    // High Definition Audio - USB Audio Offload
    //
    include ("HdaUsbAudioOffload.asl")
  }
} // END Device (HDAS)

#undef HDAS_PATH
