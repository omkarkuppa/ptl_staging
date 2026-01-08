/** @file
  ACPI PEP deep standby entry/exit.

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

//
// TCSS device would only put under the SOC side.
//
External (\_SB.PC00.LPCB.NTIR, MethodObj)
External (\_SB.PC02.LPCB.NTIR, MethodObj)
External (\GPRV, MethodObj)

//
//  PEP deep standby entry/exit feature.
//
//  Arguments: (1)
//    Arg0    - 0 exit CS
//              1 entry CS
//  Return Value:
//    None.
//
Method (PESB, 1, Serialized)
{
  If (LEqual (S0ID, 1)) {
    \GUAM (Arg0)
  }

  // Notify EC of CS entry/exit
  If (CondRefOf (\_SB.PC00.LPCB.NTIR)) {
    \_SB.PC00.LPCB.NTIR (Arg0)
  }
  If (CondRefOf (\_SB.PC02.LPCB.NTIR)) {
    \_SB.PC02.LPCB.NTIR (Arg0)
  }

  //
  // call method specific to CS platforms when the system is in a
  // standby state with very limited SW activities
  //
  If (CondRefOf (\GPRV)) {
    If (LEqual (Arg0, 1)) {
      ADBG ("[PEP] allow PS_ON!")
      \GPRV (3, 0)
    } Else {
      ADBG ("[PEP] veto PS_ON!")
      \GPRV (3, 1)
    }
  }
}
