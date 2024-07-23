/** @file
  Intel ACPI Sample Code for WWAN Flash Reset Method

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

If (LEqual (Arg0, 0)) {
  ADBG ("Start WWAN Warm Reset")
} Else {
  ADBG ("Start WWAN Cold Reset")
}
// 1. Perform First Half of Warm/Cold Reset Flow
If (CondRefOf (WWAN_PCIE_ROOT_PORT.PXSX.FHRF)) {
  WWAN_PCIE_ROOT_PORT.PXSX.FHRF (Arg0)
}
Sleep (Arg1)
ADBG (Concatenate ("WFRT Delay1  ", ToDecimalString (Arg1)))
Notify (WWAN_PCIE_ROOT_PORT.PXSX, 1)
// 2. Perform Second Half of Warm/Cold Reset (WWAN ON) Flow
If (CondRefOf (WWAN_PCIE_ROOT_PORT.PXSX.SHRF)) {
  WWAN_PCIE_ROOT_PORT.PXSX.SHRF ()
}
Sleep (Arg2)
ADBG (Concatenate ("WFRT Delay2  ", ToDecimalString (Arg2)))
Notify (WWAN_PCIE_ROOT_PORT.PXSX, 1)
ADBG ("End WWAN Warm Reset")
// 3. Release Reset mutex
Release (\WWMT)
Store (0, Local0)
ADBG ("WWAN Warm Reset Mutex: Release Done")
If (CondRefOf (WWAN_PCIE_ROOT_PORT.PXSX.DOSV)) {
  WWAN_PCIE_ROOT_PORT.PXSX.DOSV ()              // Workaround for OEM only
}
// 4. Optional - Sleep for Secondary Bus Reset Delay.
// Sleep(0)
