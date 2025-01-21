/** @file
  ACPI EC SSDT table to summary all the devices defined under H_EC scope.

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

Device (H_EC) {
  Name (ECAV, Zero)    // OS Bug Checks if EC OpRegion accessed before Embedded Controller Driver loaded
  Name (B0SC, 100)     // Battery 0 Stored Capacity. Used in VBHK and BAT0 device.
  Name (BNUM, 0)       // Number Of Batteries Present
  Name (ECUP, 1)      // EC State indicator: 1- Normal Mode 0- Low Power Mode

  Include ("EcRam.asl")
  Include ("EcBase.asl")
  Include ("EcBaseMethod.asl")
  Include ("EcLidStateApis.asl")
  Include ("EcUcsiApis.asl")
  Include ("EcDptfApis.asl")
  Include ("EcQevent.asl")

  //
  // Devices under H_EC scope
  //
  Include ("Devices/Als.asl")
  Include ("Devices/Bat0Virt.asl")
  Include ("Devices/Bat1Real.asl")
  Include ("Devices/Lid0.asl")
  Include ("Devices/WDT.asl")
  Include ("Devices/ButtonIndicator.asl")
  Include ("Devices/ConvertibleIndicator.asl")
  Include ("Devices/DockingIndicator.asl")
}

