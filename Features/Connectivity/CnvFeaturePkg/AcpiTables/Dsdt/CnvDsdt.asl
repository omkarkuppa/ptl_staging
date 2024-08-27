/** @file
  Intel ACPI Sample Code for CNV in DSDT

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

External (\_SB.GGOV, MethodObj)
External (\_SB.SGOV, MethodObj)
External (GBTK)
//
// WiFi/BT Mutex
//
Mutex (CNMT, 0)
Name (RDLY, 160)
Name (BRMT, 0)

#if FixedPcdGetBool (PcdCnvAcpiTables) == 1
#if FixedPcdGetBool (PcdDynamicSarSupport) == 1
Include ("HidDynamicSarDsm.asl")
Include ("WifiDynamicSar.asl")
#endif
#endif

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
    If (CondRefOf (GBTK)) {
      If (LNotEqual (GBTK, 0)) {
        If (CondRefOf (\_SB.SGOV)) {
          \_SB.SGOV (GBTK, Arg0)
        }
      }
    } Else {
      ADBG ("Set M.2 BT RF-Kill pin failied")
    }
  }

  //
  // Get value of M.2 BT RF-Kill (W_DISABLE2#) pin
  //
  Method (GBTR, 0)
  {
    If (CondRefOf (\_SB.GGOV)) {
      If (CondRefOf (GBTK)) {
        Return (\_SB.GGOV (GBTK))
      } Else {
        ADBG ("Get M.2 BT RF-Kill pin failied")
        Return (0)
      }
    } Else {
      ADBG ("Get M.2 BT RF-Kill pin status failied")
      Return (0)
    }
  }
}