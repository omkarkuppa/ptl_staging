/** @file
  Intel ACPI Sample Code for CnvFeaturePkg Enabling Implementation

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

#if FixedPcdGetBool (PcdIntelWifiDsmFunc1) == 1
#define IDF1  0x02
#else
#define IDF1  0x00
#endif
#if FixedPcdGetBool (PcdIntelWifiDsmFunc2) == 1
#define IDF2  0x04
#else
#define IDF2  0x00
#endif
#if FixedPcdGetBool (PcdIntelWifiDsmFunc4) == 1
#define IDF4  0x10
#else
#define IDF4  0x00
#endif
#if FixedPcdGetBool (PcdIntelWifiDsmFunc5) == 1
#define IDF5  0x20
#else
#define IDF5  0x00
#endif

#define INTEL_WIFI_DSM_SUPPORTED_FUNC_BYTE0  (0x01 | IDF1 | IDF2 | IDF4 | IDF5)
#define INTEL_WIFI_DSM_SUPPORTED_FUNC_BYTE1  0

// ----------------------------------------------------
//  Intel Proprietary WiFi Feature Enabling Solution
// ----------------------------------------------------
//
// _DSM (Device Specific Method) for Intel WiFi features
//
// This optional object is a control method that enables devices to provide device specific control
// functions that are consumed by the device driver.
//
// Arguments: (4)
//   Arg0 - A Buffer containing a UUID
//   Arg1 - An Integer containing the Revision ID
//   Arg2 - An Integer containing the Function Index
//   Arg3 - A Package that contains function-specific arguments
// Return Value:
//   If Function Index = 0, a Buffer containing a function index bit-field which has set of supported function indexes.
//   Otherwise, the return feature value based on set of supported function type which depends on the UUID and revision ID.
//
Method (IFUN, 4, Serialized) {
  //
  // Function 0 : Query Function/Get supported functions (based on revision)
  //              For Revision 0,
  //              -- Return value would contain one bit for each function index. Starting with zero.
  //              -- Bit 0 indicates whether there is support for any functions other than function 0 for the specified UUID and Revision ID.
  //              -- If set to zero, no functions are supported.
  //
  If (LEqual (Arg2, Zero)) {
    // Revision 0
    If (LEqual (Arg1, Zero)) { // The current revision is 0
      //
      // Below are functions defined.
      // Bit0    - Indicates whether its support for any other functions
      // Bit1    - vPro information
      // Bit2    - Dynamic PLDR
      // Bit3    - RFI enablement; Project after LNL, reserved
      // Bit4    - CNVd PLDR Timer Control
      // Bit31:5 - Reserved for future options
      //
      Name (IDSF, Buffer (2) {0, 0})
      Store (INTEL_WIFI_DSM_SUPPORTED_FUNC_BYTE0, Index (IDSF, 0))
      Store (INTEL_WIFI_DSM_SUPPORTED_FUNC_BYTE1, Index (IDSF, 1))
      Return (IDSF)
    } Else {
      Return (Buffer () {0}) // No supported functions.
    }
  }
  #if FixedPcdGetBool (PcdIntelWifiDsmFunc1) == 1
  Include ("IntelWifiDsmFunc1.asl")
  #endif
  #if FixedPcdGetBool (PcdIntelWifiDsmFunc2) == 1
  Include ("IntelWifiDsmFunc2.asl")
  #endif
  #if FixedPcdGetBool (PcdIntelWifiDsmFunc4) == 1
  Include ("IntelWifiDsmFunc4.asl")
  #endif
  #if FixedPcdGetBool (PcdIntelWifiDsmFunc5) == 1
  Include ("IntelWifiDsmFunc5.asl")
  #endif
  // No other supported functions, but normal return.
  Return (0)
}

//
// Checks if Intel WiFi specific _DSM is called
//
// @retval    1 if true, 0 otherwise
//
Method (IDSM, 1, Serialized) {
  If (LEqual (Arg0, ToUUID ("7266172C-220B-4B29-814F-75E4DD26B5FD"))) {
    return (1)
  }
  return (0)
}