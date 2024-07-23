/** @file
  Intel ACPI Sample Code for connectivity modules

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

@par Specification
**/

External (\_SB.RP_PATH1.CNVW, DeviceObj)
External (\_SB.RP_PATH1.BTPC, DeviceObj)
External (\_SB.GGOV, MethodObj)
External (\_SB.SGOV, MethodObj)
External (CBTC, IntObj)
External (CBTI, IntObj)
External (CRFP, IntObj)
External (CWFC, IntObj)

Scope (\_SB)
{
  //
  // Set M.2 BT RF-Kill (W_DISABLE2#) pin
  //
  Method (BTRK, 0x1, Serialized)
  {
    //
    // Arg0 - Value to W_DISABLE2#
    //
    If (LNotEqual (GBTK, 0)) {
      If (CondRefOf (\_SB.SGOV)) {
        \_SB.SGOV (GBTK, Arg0)
      }
    }
  }

  //
  // Get value of M.2 BT RF-Kill (W_DISABLE2#) pin
  //
  Method (GBTR, 0)
  {
    If (CondRefOf (\_SB.GGOV)) {
      Return (\_SB.GGOV (GBTK))
    } Else {
      ADBG ("Get M.2 BT RF-Kill pin status failied")
      Return (0)
    }
  }
}

//
// Report thermal & regulatory methods
//
If (LAnd (CondRefOf (CRFP), LEqual (CRFP, 1))) {
  If (LAnd (CondRefOf (CWFC), LEqual (CWFC, 1))) {
    //
    // CNVi Wi-Fi core is enabled.
    //
    If (CondRefOf (\_SB.RP_PATH1.CNVW)) {
      Scope (\_SB.RP_PATH1.CNVW) {
        Include ("WifiDsmWrapper.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
  If (LAnd (CondRefOf (CBTC), CondRefOf (CBTI))) {
    If (LEqual (CBTC, 1)) {
      //
      // CNVi BT core is enabled.
      //
      If (LEqual (CBTI, 2)) {
        //
        // BT interface is PCI
        //
        If (CondRefOf (\_SB.RP_PATH1.BTPC)) {
          Scope (\_SB.RP_PATH1.BTPC) {
            Include ("BtDsm.asl")
            Include ("BtAudioOffload.asl")
            Include ("GuidLockIndicator.asl")
          }
        }
      }
    }
  }
}