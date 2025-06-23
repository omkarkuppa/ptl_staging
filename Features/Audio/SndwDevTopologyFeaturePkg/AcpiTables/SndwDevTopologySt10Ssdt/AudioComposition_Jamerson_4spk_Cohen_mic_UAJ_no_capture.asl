/** @file
  SoundWire topology definitions

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

//
// Fall back acpi-acd-device-namestring
//
#ifndef COHEN_1_UAJ2_DEV_NAME
# define COHEN_1_UAJ2_DEV_NAME "\\_SB.PC00.HDAS.IDA.SNDW.SWD6.AF3B"
#endif

Name(_DSD, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-interface-revision", 0x00000001},    // 0.1 Spec Version Bits 31-16: (Upper word) Major version number. Bits 15-0: (Lower word) Minor version number
       Package (2) {"acpi-acd-endpoint-count", 3},    // 8 Endpoints: Aggregated Speaker, Headphones, LineOut
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"acpi-acd-endpoint-0-properties", "EP00"},     // Configuration for Aggregated Speaker
       Package (2) {"acpi-acd-endpoint-1-properties", "EP02"},     // Configuration for Headphones
       Package (2) {"acpi-acd-endpoint-2-properties", "EP03"},     // Configuration for LineOut
    }
}) //End _DSD


Name(CC21, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", COHEN_1_UAJ2_DEV_NAME},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0x13},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x06c0},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC02"},    // Acx specific properties
    }
}) //End CC21


Name(CC31, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", COHEN_1_UAJ2_DEV_NAME},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0x15},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x0690},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC02"},    // Acx specific properties
    }
}) //End CC31


Name(CC41, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", COHEN_1_UAJ2_DEV_NAME},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0x17},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x06d0},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC02"},    // Acx specific properties
    }
}) //End CC41


Name(CC51, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", COHEN_1_UAJ2_DEV_NAME},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0x7},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x06a0},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC02"},    // Acx specific properties
    }
}) //End CC51


Name(CC61, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", COHEN_1_UAJ2_DEV_NAME},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0x9},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x0680},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC02"},    // Acx specific properties
    }
}) //End CC61


Name(CC71, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", COHEN_1_UAJ2_DEV_NAME},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0xB},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x06d0},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC02"},    // Acx specific properties
    }
}) //End CC71
