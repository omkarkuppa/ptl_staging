/** @file
  Intel ACPI Reference Code for _DSM BT Feature Enabling Implementation

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

// --------------------------------------------------
//  Intel Proprietary BT Feature Enabling Solution
// --------------------------------------------------
//
// _DSM (Device Specific Method)
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

#if FixedPcdGetBool (PcdIntelBtDsmFunc1) == 1
#define BDF1  0x02
#else
#define BDF1  0x00
#endif
#if FixedPcdGetBool (PcdIntelBtDsmFunc2) == 1
#define BDF2  0x04
#else
#define BDF2  0x00
#endif
#if FixedPcdGetBool (PcdIntelBtDsmFunc3) == 1
#define BDF3  0x08
#else
#define BDF3  0x00
#endif
#if FixedPcdGetBool (PcdIntelBtDsmFunc4) == 1
#define BDF4  0x10
#else
#define BDF4  0x00
#endif
#if FixedPcdGetBool (PcdIntelBtDsmFunc5) == 1
#define BDF5  0x20
#else
#define BDF5  0x00
#endif
#if FixedPcdGetBool (PcdIntelBtDsmFunc6) == 1
#define BDF6  0x40
#else
#define BDF6  0x00
#endif
#if FixedPcdGetBool (PcdIntelBtDsmFunc7) == 1
#define BDF7  0x80
#else
#define BDF7  0x00
#endif
#define INTEL_BT_DSM_SUPPORTED_FUNC_BYTE0  (0x01 | BDF1 | BDF2 | BDF3| BDF4| BDF5| BDF6| BDF7)

#if FixedPcdGetBool (PcdIntelBtTileDsmFunc1) == 1
#define TDF1  0x02
#else
#define TDF1  0x00
#endif
#define INTEL_BT_TILE_DSM_SUPPORTED_FUNC_BYTE0  (0x01 | TDF1)

Method (_DSM, 0x4, Serialized)
{
  If (LEqual (Arg0, ToUUID ("AA10F4E0-81AC-4233-ABF6-3B2AC50E28D9"))) {
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
        // Bit1    - Set BT reset timing
        // Bit2    - Get BT reset timing
        // Bit3    - BT PLDR Configure
        // Bit4    - Get Reset Method Type
        // Bit31:5 - Reserved for future options
        Name (BDSF, Buffer (1) {0})
        Store (INTEL_BT_DSM_SUPPORTED_FUNC_BYTE0, Index (BDSF, 0))
        Return (BDSF)
      } Else {
        Return (Buffer () {0}) // No supported functions.
      }
    }
    #if FixedPcdGetBool (PcdIntelBtDsmFunc1) == 1
    Include ("IntelBtDsmFunc1.asl")
    #endif
    #if FixedPcdGetBool (PcdIntelBtDsmFunc2) == 1
    Include ("IntelBtDsmFunc2.asl")
    #endif
    #if FixedPcdGetBool (PcdIntelBtDsmFunc3) == 1
    Include ("IntelBtDsmFunc3.asl")
    #endif
    #if FixedPcdGetBool (PcdIntelBtDsmFunc4) == 1
    Include ("IntelBtDsmFunc4.asl")
    #endif
    #if FixedPcdGetBool (PcdIntelBtDsmFunc5) == 1
    Include ("IntelBtDsmFunc5.asl")
    #endif
    #if FixedPcdGetBool (PcdIntelBtDsmFunc7) == 1
    Include ("IntelBtDsmFunc7.asl")
    #endif
    #if FixedPcdGetBool (PcdIntelBtDsmFunc6) == 1
    Include ("IntelBtDsmFunc6.asl")
    #endif
    // No other supported functions, but normal return.
    Return (0)
  } ElseIf (LEqual (Arg0, ToUUID ("2D19D3E1-5708-4696-BD5B-2C3DBAE2D6A9"))) {
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
        // Bit1    - Set Tile Activation
        // Bit31:2 - Reserved for future options
        Name (TDSF, Buffer (1) {0})
        Store (INTEL_BT_TILE_DSM_SUPPORTED_FUNC_BYTE0, Index (TDSF, 0))
        Return (TDSF)
      } Else {
        Return (Buffer () {0}) // No supported functions.
      }
    }
    #if FixedPcdGetBool (PcdIntelBtTileDsmFunc1) == 1
    Include ("IntelBtTileDsmFunc1.asl")
    #endif
    // No other supported functions, but normal return.
    Return (0)
  } Else {
    Return (Buffer () {0}) // Guid mismatch
  }
}