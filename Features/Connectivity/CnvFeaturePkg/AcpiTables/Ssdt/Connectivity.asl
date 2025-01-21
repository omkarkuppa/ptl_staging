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
External (CBTC, IntObj)
External (CBTI, IntObj)
External (CRFP, IntObj)
External (CWFC, IntObj)

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
#if FixedPcdGetBool (PcdWifiDsmSupport) == 1
        Include ("WifiDsmWrapper.asl")
#endif
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
#if FixedPcdGetBool (PcdBtDsmSupport) == 1
            Include ("BtDsm.asl")
#endif
#if FixedPcdGetBool (PcdBtAudioOffloadSupport) == 1
            Include ("BtAudioOffload.asl")
#endif
            Include ("GuidLockIndicator.asl")
          }
        }
      }
    }
  }
}