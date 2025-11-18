/** @file
  ACPI Support for PCIe Hot Plug with RealTek SD Card Reader

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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
  External (PCIE_SD_CARD_SCOPE.PXSX, DeviceObj)

  Method (ADSD, 0) {
    ADBG ("Hot Plug Support Enable for SD Card Reader and SD Host Controller")
    Return (
      Package () {
        // HotPlugSupportInD3 UUID
        ToUUID ("6211E2C0-58A3-4AF3-90E1-927A4E0C55A4"),
        Package () {
          Package (2) {"HotPlugSupportInD3", 1},
        },

        // ExternalFacingPort UUID
        ToUUID ("EFCC06CC-73AC-4BC3-BFF0-76143807C389"),
        Package () {
          Package (2) {"ExternalFacingPort", 1},
          Package (2) {"UID", SD_CARD_UID},  // assign different value to SD_CARD_UID for each RP
        }
      }
    )
  }

  Scope (PCIE_SD_CARD_SCOPE.PXSX) {
    Method (_RMV, 0) {
      //
      // The _RMV method indicates if the device is removable
      // (1 = removable, 0 = non-removable).
      //
      ADBG ("_RMV method called for SD Host Controller")
      Return (0x01)         // Removable
    }
  }