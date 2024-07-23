/** @file
  Intel ACPI Sample Code for Wireless Connectivity Configuration Device

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

#include <SmbiosPlatformInfoDefinition.h>

Name (DEVM, 0xFF) // Device Mode

Scope(\_SB)
{
  //
  // Wireless Connectivity Configuration Device
  //
  Device (WCCD) {
    Name (_HID, "INTC1092")
    Name (_STR, Unicode ("Wireless Connectivity Configuration Device"))

    //
    // Initialize device mode
    //
    Method (_INI) {
      Store (0, DEVM) // Default: Device Mode 0
    }

    // _STA (Status)
    //
    // This object returns the current status of a device.
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //   An Integer containing a device status bitmap:
    //    Bit 0 - Set if the device is present.
    //    Bit 1 - Set if the device is enabled and decoding its resources.
    //    Bit 2 - Set if the device should be shown in the UI.
    //    Bit 3 - Set if the device is functioning properly (cleared if device failed its diagnostics).
    //    Bit 4 - Set if the battery is present.
    //    Bits 5-31 - Reserved (must be cleared).
    //
    Method (_STA) {
      If (LAnd (LNotEqual (WWEN, 0), LEqual (WCEN, 1))){
        Return (0x0F)
      } Else {
        Return (0x00)
      }
    }

    Name (WCS0, Package () // WWAN_CFG_SET 0 for FCC
    {
      0x00,                // Regulatory 0: FCC
      Package ()           // WWAN_DEVICE_MODE_INFO for Device Mode 0
      {
        0x00,              // Device Mode 0
        0x00000001,        // Band Table Index
        0x00000002,        // Antenna Table Index
        0x00000003,        // SAR Table Index
      },
      Package ()           // WWAN_DEVICE_MODE_INFO for Device Mode 1
      {
        0x01,              // Device Mode 1
        0x00000002,        // Band Table Index
        0x00000003,        // Antenna Table Index
        0x00000001,        // SAR Table Index
      },
      Package ()           // WWAN_DEVICE_MODE_INFO for Device Mode 2
      {
        0x02,              // Device Mode 2
        0x00000003,        // Band Table Index
        0x00000001,        // Antenna Table Index
        0x00000002,        // SAR Table Index
      }
    })

    Name (WCS1, Package () // WWAN_CFG_SET 1 for CE
    {
      0x01,                // Regulatory 1: CE
      Package ()           // WWAN_DEVICE_MODE_INFO for Device Mode 0
      {
        0x00,              // Device Mode 0
        0x00000002,        // Band Table Index
        0x00000003,        // Antenna Table Index
        0x00000001,        // SAR Table Index
      },
      Package ()           // WWAN_DEVICE_MODE_INFO for Device Mode 1
      {
        0x01,              // Device Mode 1
        0x00000003,        // Band Table Index
        0x00000001,        // Antenna Table Index
        0x00000002,        // SAR Table Index
      },
      Package ()           // WWAN_DEVICE_MODE_INFO for Device Mode 2
      {
        0x02,              // Device Mode 2
        0x00000001,        // Band Table Index
        0x00000002,        // Antenna Table Index
        0x00000003,        // SAR Table Index
      }
    })

    Name (WCS2, Package () // WWAN_CFG_SET 2 for ISED
    {
      0x02,                // Regulatory 2: ISED
      Package ()           // WWAN_DEVICE_MODE_INFO for Device Mode 0
      {
        0x00,              // Device Mode 0
        0x00000003,        // Band Table Index
        0x00000001,        // Antenna Table Index
        0x00000002,        // SAR Table Index
      },
      Package ()           // WWAN_DEVICE_MODE_INFO for Device Mode 1
      {
        0x01,              // Device Mode 1
        0x00000001,        // Band Table Index
        0x00000002,        // Antenna Table Index
        0x00000003,        // SAR Table Index
      },
      Package ()           // WWAN_DEVICE_MODE_INFO for Device Mode 2
      {
        0x02,              // Device Mode 2
        0x00000002,        // Band Table Index
        0x00000003,        // Antenna Table Index
        0x00000001,        // SAR Table Index
      }
    })

    //
    // WWAN Device Mode Configuration Table
    // The contain of table is just for Intel RVP. The table values can be modified
    // accordingly to OEM/Customer product.
    //
    Name (WDMC, Package () // WWAN_DEVICE_MODE_CONFIGURATION
    {
      0,                   // Revision, [0-255]
      NUM_OF_DEV_MODE,     // Number of Device Mode
      Package ()           // WWAN_CFG_SET
      {
        0x00,              // Regulatory 0: FCC, 1: CE, 2: ISED
        Package ()         // WWAN_DEVICE_MODE_INFO 0
        {
          0x00,            // Device Mode 0
          0x00000000,      // Band Table Index
          0x00000000,      // Antenna Table Index
          0x00000000,      // SAR Table Index
        },
        Package ()         // WWAN_DEVICE_MODE_INFO 1
        {
          0x00,            // Device Mode 1
          0x00000000,      // Band Table Index
          0x00000000,      // Antenna Table Index
          0x00000000,      // SAR Table Index
        },
        Package ()         // WWAN_DEVICE_MODE_INFO 2
        {
          0x00,            // Device Mode 2
          0x00000000,      // Band Table Index
          0x00000000,      // Antenna Table Index
          0x00000000,      // SAR Table Index
        }
      }
    })

    //----------------------------------------------------------
    //  Wireless Connectivity Configuration Device _DSM Method
    //----------------------------------------------------------
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
    Method (_DSM, 0x4, Serialized, 0, {IntObj, BuffObj}, {BuffObj, IntObj, IntObj, PkgObj}) {
      If (LEqual (Arg0, ToUUID ("82737E72-3A33-4C45-A9C7-57C0411A5F13"))) {
        //
        // Function 0 : Query Function/Get supported functions (based on revision)
        //              For Revision 0,
        //              -- Return value would contain one bit for each function index. Starting with zero.
        //              -- Bit 0 indicates whether there is support for any functions other than function 0 for the specified UUID and Revision ID.
        //              -- If set to zero, no functions are supported.
        //
        If(LEqual (Arg2, 0)) {
          //
          // Revision 0
          //
          If(LEqual (Arg1, 0)) {
            //
            // Below are functions defined.
            // Bit0    - Indicates whether its support for any other functions
            // Bit1    - Get Device Mode
            // Bit2    - Get WWAN Device Mode Configuration Table
            // Bit31:3 - Reserved for future options
            Store (0x07, Local0)
            Store (0x80000007, Local0)
            Return (Local0)
          } Else {
            Return (0x0) // No supported functions.
          }
        }

        //
        // Function 1 : Get Device Mode
        //   Input: None
        //   Return: DEVM -> Device Mode
        //
        If (LEqual (Arg2, 1)) {
          ADBG (Concatenate ("Device Mode ", ToDecimalString (DEVM)))
          Return (DEVM)
        }

        //
        // Function 2 : Get WWAN Device Mode Configuration Table
        //   Input: Arg3 -> The requested regulatory
        //   Return: WDMC for the requested regulatory
        //
        If (LEqual (Arg2, 2)) {
          ADBG (Concatenate ("Get WDMC Table with WCS", ToHexString (Arg3)))
          //
          // Update the corresponding WWAN CFG SET table for the requested regulatory
          //
          If (LEqual (Arg3, 0)) {          // 0: FCC
            Store (WCS0, Index (WDMC, 2))
          } ElseIf (LEqual (Arg3, 1)) {    // 1: CE
            Store (WCS1, Index (WDMC, 2))
          } ElseIf (LEqual (Arg3, 2)) {    // 2: ISED
            Store (WCS2, Index (WDMC, 2))
          } Else {
            //
            // Unsupported regulatory is detected!!
            //
            ADBG ("Unsupported regulatory is detected!!")
          }
          Return (WDMC)
        }

        //
        // Function 31 : Set Device Mode
        //
        If (LEqual (Arg2, 31)) {
          ADBG (Concatenate ("Set Device Mode ", ToDecimalString (Arg3)))
          Store (Arg3, DEVM)
          Notify (\_SB.WCCD, 0x80)
          Return (0x0)
        }
        //
        // No other supported functions, but normal return.
        //
        Return (0x0)
      } Else {
        //
        // Guid mismatch
        //
        Return (0x0)
      }
    }
  } // End of Device (WCCD)
}