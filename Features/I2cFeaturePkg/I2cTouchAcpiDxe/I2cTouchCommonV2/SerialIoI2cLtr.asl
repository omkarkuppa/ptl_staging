/** @file
  ACPI DSDT table for Serial Io I2C LTR settings

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

// _DSD: Device-Specific Data
//
//  - I2C LTR UUID: f87a6d23-2884-4fe4-a55f-633d9e339ce1
//  Designated for I2C Driver to configure its Latency Tolerance Reporting
//  - idle latency tolerance
//  - active latency for each Speed SS/FM/HS
//
//  Example:
//    "idle-latency-tolerance", 0xBB8
//    0xBB8 -> 3000 us = 3ms of IDLE LTR
//  Value of 0xFFFF will cause no LTR reporting/driver default used
//
Name(_DSD, Package () {
  ToUUID ("f87a6d23-2884-4fe4-a55f-633d9e339ce1"), //I2C LTR reporting UUID
  Package () {
    Package (0x02) {
        "idle-latency-tolerance",
        0xFFFF
    },
    Package (0x02) {
        "SS-active-latency-tolerance",
        0xFFFF
    },
    Package (0x02) {
        "FM-active-latency-tolerance",
        0xFFFF
    },
    Package (0x02) {
        "FMP-active-latency-tolerance",
        0xFFFF
    }
  }
})