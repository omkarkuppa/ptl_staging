/** @file
  Intel ACPI Sample Code for WIST method Implementation

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

External (SVDC, IntObj)

#if FixedPcdGet32 (PcdSupportedWifi1) != 0
#define WIFI_VDID1  FixedPcdGet32 (PcdSupportedWifi1)
#endif
#if FixedPcdGet32 (PcdSupportedWifi2) != 0
#define WIFI_VDID2  FixedPcdGet32 (PcdSupportedWifi2)
#endif
#if FixedPcdGet32 (PcdSupportedWifi3) != 0
#define WIFI_VDID3  FixedPcdGet32 (PcdSupportedWifi3)
#endif
#if FixedPcdGet32 (PcdSupportedWifi4) != 0
#define WIFI_VDID4  FixedPcdGet32 (PcdSupportedWifi4)
#endif
#if FixedPcdGet32 (PcdSupportedWifi5) != 0
#define WIFI_VDID5  FixedPcdGet32 (PcdSupportedWifi5)
#endif
#if FixedPcdGet32 (PcdSupportedWifi6) != 0
#define WIFI_VDID6  FixedPcdGet32 (PcdSupportedWifi6)
#endif
#if FixedPcdGet32 (PcdSupportedWifi7) != 0
#define WIFI_VDID7  FixedPcdGet32 (PcdSupportedWifi7)
#endif
#if FixedPcdGet32 (PcdSupportedWifi8) != 0
#define WIFI_VDID8  FixedPcdGet32 (PcdSupportedWifi8)
#endif
#if FixedPcdGet32 (PcdSupportedWifi9) != 0
#define WIFI_VDID9  FixedPcdGet32 (PcdSupportedWifi9)
#endif
#if FixedPcdGet32 (PcdSupportedWifi10) != 0
#define WIFI_VDID10  FixedPcdGet32 (PcdSupportedWifi10)
#endif

// WIST (WiFi Device Presence Status)
//
// Check if a WiFi Device is present on the root port.
//
// Arguments: (0)
//   None
// Return Value:
//    0 - if a device is not present.
//    1 - if a device is present.
//
Method (WIST, 0, Serialized)
{
  // check Vendor-Device ID for supported devices
  If (CondRefOf (VDID)) {
    Switch (ToInteger (VDID)) {
      #if FixedPcdGet32 (PcdSupportedWifi1) != 0
      Case (WIFI_VDID1) {
        Return (1)
      }
      #endif
      #if FixedPcdGet32 (PcdSupportedWifi2) != 0
      Case (WIFI_VDID2) {
        Return (1)
      }
      #endif
      #if FixedPcdGet32 (PcdSupportedWifi3) != 0
      Case (WIFI_VDID3) {
        Return (1)
      }
      #endif
      #if FixedPcdGet32 (PcdSupportedWifi4) != 0
      Case (WIFI_VDID4) {
        Return (1)
      }
      #endif
      #if FixedPcdGet32 (PcdSupportedWifi5) != 0
      Case (WIFI_VDID5) {
        Return (1)
      }
      #endif
      #if FixedPcdGet32 (PcdSupportedWifi6) != 0
      Case (WIFI_VDID6) {
        Return (1)
      }
      #endif
      #if FixedPcdGet32 (PcdSupportedWifi7) != 0
      Case (WIFI_VDID7) {
        Return (1)
      }
      #endif
      #if FixedPcdGet32 (PcdSupportedWifi8) != 0
      Case (WIFI_VDID8) {
        Return (1)
      }
      #endif
      #if FixedPcdGet32 (PcdSupportedWifi9) != 0
      Case (WIFI_VDID9) {
        Return (1)
      }
      #endif
      #if FixedPcdGet32 (PcdSupportedWifi10) != 0
      Case (WIFI_VDID10) {
        Return (1)
      }
      #endif
      Default {
        If (LAnd (CondRefOf (SVDC), LEqual (SVDC, 1))) {     // Skip VDID check
          Return (1)
        }
        Return (0)                  // no supported device
      }
    }
  } Else {
    Return (0)
  }
}