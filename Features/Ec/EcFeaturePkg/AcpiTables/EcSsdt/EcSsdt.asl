/** @file
  ACPI EC SSDT table

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
#ifndef PARENT_OF_LPCB
#define PARENT_OF_LPCB PC00
#endif

DefinitionBlock (
  "ECSSDT.aml",
  "SSDT",
  0x02,
  "EcRef",
  "EcSsdt ",
  0x0
  )
{
  External (ADBG, MethodObj)
  #define ADBG(x) \
  If (CondRefOf(ADBG)) {\
    ADBG(x) \
  }
  External (\_SB.ASRT, MethodObj)
  ADBG ("[EC SSDT][AcpiTableEntry]")
  External (\_SB.PARENT_OF_LPCB.LPCB, DeviceObj)
  External (\_SB.HIDD, DeviceObj)

  Include ("EcNvs.asl")

  Scope (\_SB.PARENT_OF_LPCB.LPCB) {
    Include ("EcDevice.asl")
  }

  //
  // System Bus
  //
  Scope (\_SB) {
    Include ("SBMethod/PowerButtonMethod.asl")
    Include ("SBDevices/PowerButton.asl")
    Include ("SBDevices/ACDevice.asl")
    Include ("SBDevices/Pg3d.asl")
  }

  Scope (\_SB.HIDD) {
    Include ("HID/HidMethod.asl")
  }

  Include ("HIDWakeDSM.asl")
  ADBG ("[EC SSDT][AcpiTableExit]")
} // End EC SSDT
