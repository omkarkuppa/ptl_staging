/** @file
  Intel ACPI Sample Code for OEM WiFi Configuration

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

#if FixedPcdGetBool (PcdOemWifiDsmFunc11) == 1
#define ODFB  0x08
#else
#define ODFB  0x00
#endif
#define OEM_WIFI_DSM_SUPPORTED_FUNC_BYTE0  (0x01 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00)
#define OEM_WIFI_DSM_SUPPORTED_FUNC_BYTE1  (0x00 | 0x00 | 0x00 | ODFB)

// --------------------------
//  OEM WiFi Configuration
// --------------------------
//
// _DSM (Device Specific Method) for OEM WiFi configuration
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
Method (OFUN, 4, Serialized) {
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
      // Bit0     - Indicates whether its support for any other functions
      // Bit1:10  - Be replaced by UEFI variable - UefiCnvWlanGeneralCfg
      // Bit11    - DLVR RFI Mitigation and DDR RFI Mitigation
      // Bit31:12 - Reserved for future options
      //
      Name (ODSF, Buffer (2) {0, 0})
      Store (OEM_WIFI_DSM_SUPPORTED_FUNC_BYTE0, Index (ODSF, 0))
      Store (OEM_WIFI_DSM_SUPPORTED_FUNC_BYTE1, Index (ODSF, 1))
      Return (ODSF)
    } Else {
      Return (Buffer () {0}) // No supported functions.
    }
  }
  #if FixedPcdGetBool (PcdOemWifiDsmFunc11) == 1
  Include ("OemWifiDsmFunc11.asl")
  #endif
  // No other supported functions, but normal return.
  Return (0)
}

//
// Checks if OEM WiFi specific _DSM is called
//
// @retval    1 if true, 0 otherwise
//
Method (ODSM, 1, Serialized) {
  If (LEqual (Arg0, ToUUID ("F21202BF-8F78-4DC6-A5B3-1F738E285ADE"))) {
    return (1)
  }
  return (0)
}