/**@file
 PMAX Devices ACPI table

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

@par Specification
**/
#include <PmaxDevDef.h>

DefinitionBlock (
  "PMAX.aml",
  "SSDT",
  0x02,
  "PmaxDv",
  "Pmax_Dev",
  0x1
)
{
  External(\PXAC, IntObj)
  External(\PXUC, IntObj)
  External(\PXWC, IntObj)
  External(\PXFD, IntObj)
  External(\PXDC, IntObj)

  Scope(\_SB) {
  //------------------------
  // PMAX device description
  // Note: This is a sample table describing device peak power values for ADL platform for the PMAX driver to consume
  //------------------------
    Device (PMAX)
    {
      Name(_HID, "INT3533")

      Name(RTKS, DEFAULT_REALTEK_CODEC_DEVICE_STRING)
      Name(RTK0, DEFAULT_REALTEK_CODEC_DEVICE_D0_PEAK_POWER)
      Name(RTKX, DEFAULT_REALTEK_CODEC_DEVICE_DX_PEAK_POWER)

      Name(WFCS, DEFAULT_WF_CAMERA_STRING)
      Name(WFC0, DEFAULT_WF_CAMERA_D0_PEAK_POWER)
      Name(WFCX, DEFAULT_WF_CAMERA_DX_PEAK_POWER)

      Name(UFCS, DEFAULT_UF_CAMERA_STRING)
      Name(UFC0, DEFAULT_UF_CAMERA_D0_PEAK_POWER)
      Name(UFCX, DEFAULT_UF_CAMERA_DX_PEAK_POWER)

      Name(FLDS, DEFAULT_FLASH_DEVICE_STRING)
      Name(FLD0, DEFAULT_FLASH_DEVICE_D0_PEAK_POWER)
      Name(FLDX, DEFAULT_FLASH_DEVICE_DX_PEAK_POWER)

      //
      // Package and names above are place holders, will be updated in boot time.
      //
      Name(DSMP, Package() {
        Package() {
          "",                       // Realtek codec device
          Package() {
            Package() {0, 0xFFFF},  // D0 peak power in mW
            Package() {1, 0xFFFF}   // Dx peak power in mW
          }
        },
        Package() {
          "",                       // WF Camera
          Package() {
            Package() {0, 0xFFFF},  // D0 peak power in mW without accounting for flash
            Package() {1, 0xFFFF}   // Dx peak power in mW
          }
        },
        Package() {
          "",                       // UF Camera
          Package() {
            Package() {0, 0xFFFF},  // D0 peak power in mW without accounting for flash
            Package() {1, 0xFFFF}   // Dx peak power in mW
          }
        },
        Package() {
          "",                       // Flash device
          Package() {
            Package() {0, 0xFFFF},  // D0 peak power in mW
            Package() {1, 0xFFFF}   // Dx peak power in mW
          }
        }
      })

      Method (_DSM, 4, NotSerialized) {   // _DSM: Device-Specific Method
        If (LEqual(Arg0,ToUUID("2256364F-A3A9-4c9a-BCEC-A34A34A57144"))) {
          //
          // Update device name and peak power values.
          //
          Store(RTKS, Index (DeRefOf (Index (DSMP, 0)), 0))
          Store(RTK0, Index (DeRefOf (Index (DeRefOf (Index (DeRefOf (Index (DSMP, 0)), 1)), 0)), 1))
          Store(RTKX, Index (DeRefOf (Index (DeRefOf (Index (DeRefOf (Index (DSMP, 0)), 1)), 1)), 1))

          Store(WFCS, Index (DeRefOf (Index (DSMP, 1)), 0))
          Store(WFC0, Index (DeRefOf (Index (DeRefOf (Index (DeRefOf (Index (DSMP, 1)), 1)), 0)), 1))
          Store(WFCX, Index (DeRefOf (Index (DeRefOf (Index (DeRefOf (Index (DSMP, 1)), 1)), 1)), 1))

          Store(UFCS, Index (DeRefOf (Index (DSMP, 2)), 0))
          Store(UFC0, Index (DeRefOf (Index (DeRefOf (Index (DeRefOf (Index (DSMP, 2)), 1)), 0)), 1))
          Store(UFCX, Index (DeRefOf (Index (DeRefOf (Index (DeRefOf (Index (DSMP, 2)), 1)), 1)), 1))

          Store(FLDS, Index (DeRefOf (Index (DSMP, 3)), 0))
          Store(FLD0, Index (DeRefOf (Index (DeRefOf (Index (DeRefOf (Index (DSMP, 3)), 1)), 0)), 1))
          Store(FLDX, Index (DeRefOf (Index (DeRefOf (Index (DeRefOf (Index (DSMP, 3)), 1)), 1)), 1))

          If (LEqual (Arg1, Zero)) {
            If (LEqual (Arg2, 0)) {
              //- Standard query - A bitmask of functions supported
              //- Supports functions 0-1
              Return ( Buffer() {0x3})
            }
            If (LEqual (Arg2, 1)) {
              // Config PMAX Audio Codec
              If (PXAC == 1) {
                Store(Package(){Package() {0, 0},Package() {1, 0}},Index (DeRefOf (Index (DSMP, 0)), 1)) // Clear D0 peak power in mW
              }
              // Config PMAX WF Camera
              If (PXWC == 1) {
                Store(Package(){Package() {0, 0},Package() {1, 0}},Index (DeRefOf (Index (DSMP, 1)), 1)) // Clear D0 peak power in mW
              }
              // Config PMAX UF Camera
              If (PXUC == 1) {
                Store(Package(){Package() {0, 0},Package() {1, 0}},Index (DeRefOf (Index (DSMP, 2)), 1)) // Clear D0 peak power in mW
              }
              // Config PMAX Flash device
              If (PXFD == 1) {
                Store(Package(){Package() {0, 0},Package() {1, 0}},Index (DeRefOf (Index (DSMP, 3)), 1)) // Clear D0 peak power in mW
              }
              // Power plane request
              // Return a list of packages with device power information
              Return (DSMP)
            }
          }
          Return (0)
        }
        Return (0)
      }

      Method (_STA) {
        If (PXDC == 0) {
          Return (0x0)
        } else {
          Return (0xF)
        }
      }

    }
  }
}
