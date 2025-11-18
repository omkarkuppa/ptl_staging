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


//-----------------
// Configuration
//-----------------

#if _NUM_AMP_AMPS > 0
# ifdef CODEC1_UID
#  ifdef CODEC1_AMP
#   ifdef CODEC1_MIC
#    ifdef CODEC1_UAJ
//    3 Endpoints: Aggregated Speaker, Headphones, LineOut
#     define ENDPOINT_CNT  3
#     define EP00_PROPERTIES Package (2) {"acpi-acd-endpoint-0-properties", "EP00"},
#     define EP01_PROPERTIES
#     define EP02_PROPERTIES Package (2) {"acpi-acd-endpoint-1-properties", "EP02"},
#     define EP03_PROPERTIES Package (2) {"acpi-acd-endpoint-2-properties", "EP03"},
#     define EP04_PROPERTIES
#     define EP05_PROPERTIES
#     define EP06_PROPERTIES
#     define EP07_PROPERTIES
#    else // !CODEC1_UAJ
//    1 Endpoints: Aggregated Speaker
#     define ENDPOINT_CNT  1
#     define EP00_PROPERTIES Package (2) {"acpi-acd-endpoint-0-properties", "EP00"},
#     define EP01_PROPERTIES
#     define EP02_PROPERTIES
#     define EP03_PROPERTIES
#     define EP04_PROPERTIES
#     define EP05_PROPERTIES
#     define EP06_PROPERTIES
#     define EP07_PROPERTIES
#    endif // CODEC1_UAJ
#   else // !CODEC1_MIC
#    ifdef CODEC1_UAJ
//    3 Endpoints: Aggregated Speaker, LineOut, HeadsetOutput
#     define ENDPOINT_CNT  3
#     define EP00_PROPERTIES Package (2) {"acpi-acd-endpoint-0-properties", "EP00"},
#     define EP01_PROPERTIES
#     define EP02_PROPERTIES Package (2) {"acpi-acd-endpoint-1-properties", "EP02"},
#     define EP03_PROPERTIES Package (2) {"acpi-acd-endpoint-2-properties", "EP03"},
#     define EP04_PROPERTIES
#     define EP05_PROPERTIES
#     define EP06_PROPERTIES
#     define EP07_PROPERTIES
#    else // !CODEC1_UAJ
//    1 Endpoints: Aggregated Speaker
#     define ENDPOINT_CNT  1
#     define EP00_PROPERTIES Package (2) {"acpi-acd-endpoint-0-properties", "EP00"},
#     define EP01_PROPERTIES
#     define EP02_PROPERTIES
#     define EP03_PROPERTIES
#     define EP04_PROPERTIES
#     define EP05_PROPERTIES
#     define EP06_PROPERTIES
#     define EP07_PROPERTIES
#    endif // CODEC1_UAJ
#   endif // CODEC1_MIC
#  else // !CODEC1_AMP
#   ifdef CODEC1_MIC
#    ifdef CODEC1_UAJ
//    3 Endpoints: Aggregated Speaker, Headphones, LineOut
#     define ENDPOINT_CNT  3
#     define EP00_PROPERTIES Package (2) {"acpi-acd-endpoint-0-properties", "EP00"},
#     define EP01_PROPERTIES
#     define EP02_PROPERTIES Package (2) {"acpi-acd-endpoint-1-properties", "EP02"},
#     define EP03_PROPERTIES Package (2) {"acpi-acd-endpoint-2-properties", "EP03"},
#     define EP04_PROPERTIES
#     define EP05_PROPERTIES
#     define EP06_PROPERTIES
#     define EP07_PROPERTIES
#    else // !CODEC1_UAJ
//    1 Endpoints: Aggregated Speaker
#     define ENDPOINT_CNT  1
#     define EP00_PROPERTIES Package (2) {"acpi-acd-endpoint-0-properties", "EP00"},
#     define EP01_PROPERTIES
#     define EP02_PROPERTIES
#     define EP03_PROPERTIES
#     define EP04_PROPERTIES
#     define EP05_PROPERTIES
#     define EP06_PROPERTIES
#     define EP07_PROPERTIES
#    endif // CODEC1_UAJ
#   else // !CODEC1_MIC
#    ifdef CODEC1_UAJ
//    3 Endpoints: Aggregated Speaker, Headphones, LineOut
#     define ENDPOINT_CNT  3
#     define EP00_PROPERTIES Package (2) {"acpi-acd-endpoint-0-properties", "EP00"},
#     define EP01_PROPERTIES
#     define EP02_PROPERTIES Package (2) {"acpi-acd-endpoint-1-properties", "EP02"},
#     define EP03_PROPERTIES Package (2) {"acpi-acd-endpoint-2-properties", "EP03"},
#     define EP04_PROPERTIES
#     define EP05_PROPERTIES
#     define EP06_PROPERTIES
#     define EP07_PROPERTIES
#    else // !CODEC1_UAJ
//    1 Endpoints: Aggregated Speaker
#     define ENDPOINT_CNT  1
#     define EP00_PROPERTIES Package (2) {"acpi-acd-endpoint-0-properties", "EP00"},
#     define EP01_PROPERTIES
#     define EP02_PROPERTIES
#     define EP03_PROPERTIES
#     define EP04_PROPERTIES
#     define EP05_PROPERTIES
#     define EP06_PROPERTIES
#     define EP07_PROPERTIES
#    endif // CODEC1_UAJ
#   endif // CODEC1_MIC
#  endif // CODEC1_AMP
# else // !CODEC1_UID
// 1 Endpoints: Aggregated Speaker
#  define ENDPOINT_CNT  1
#  define EP00_PROPERTIES Package (2) {"acpi-acd-endpoint-0-properties", "EP00"},
#  define EP01_PROPERTIES
#  define EP02_PROPERTIES
#  define EP03_PROPERTIES
#  define EP04_PROPERTIES
#  define EP05_PROPERTIES
#  define EP06_PROPERTIES
#  define EP07_PROPERTIES
# endif // CODEC1_UID
#else // !(_NUM_AMP_AMPS > 0)
# ifdef CODEC1_UID
#  ifdef CODEC1_AMP
#   ifdef CODEC1_MIC
#    ifdef CODEC1_UAJ
//    3 Endpoints: Aggregated Speaker, Headphones, LineOut
#     define ENDPOINT_CNT  3
#     define EP00_PROPERTIES Package (2) {"acpi-acd-endpoint-0-properties", "EP00"},
#     define EP01_PROPERTIES
#     define EP02_PROPERTIES Package (2) {"acpi-acd-endpoint-1-properties", "EP02"},
#     define EP03_PROPERTIES Package (2) {"acpi-acd-endpoint-2-properties", "EP03"},
#     define EP04_PROPERTIES
#     define EP05_PROPERTIES
#     define EP06_PROPERTIES
#     define EP07_PROPERTIES
#    else // !CODEC1_UAJ
//    1 Endpoints: Aggregated Speaker
#     define ENDPOINT_CNT  1
#     define EP00_PROPERTIES Package (2) {"acpi-acd-endpoint-0-properties", "EP00"},
#     define EP01_PROPERTIES
#     define EP02_PROPERTIES
#     define EP03_PROPERTIES
#     define EP04_PROPERTIES
#     define EP05_PROPERTIES
#     define EP06_PROPERTIES
#     define EP07_PROPERTIES
#    endif // CODEC1_UAJ
#   else // !CODEC1_MIC
#    ifdef CODEC1_UAJ
//    3 Endpoints: Aggregated Speaker, Headphones, LineOut
#     define ENDPOINT_CNT  3
#     define EP00_PROPERTIES Package (2) {"acpi-acd-endpoint-0-properties", "EP00"},
#     define EP01_PROPERTIES
#     define EP02_PROPERTIES Package (2) {"acpi-acd-endpoint-1-properties", "EP02"},
#     define EP03_PROPERTIES Package (2) {"acpi-acd-endpoint-2-properties", "EP03"},
#     define EP04_PROPERTIES
#     define EP05_PROPERTIES
#     define EP06_PROPERTIES
#     define EP07_PROPERTIES
#    else // !CODEC1_UAJ
//    1 Endpoints: Aggregated Speaker
#     define ENDPOINT_CNT  1
#     define EP00_PROPERTIES Package (2) {"acpi-acd-endpoint-0-properties", "EP00"},
#     define EP01_PROPERTIES
#     define EP02_PROPERTIES
#     define EP03_PROPERTIES
#     define EP04_PROPERTIES
#     define EP05_PROPERTIES
#     define EP06_PROPERTIES
#     define EP07_PROPERTIES
#    endif // CODEC1_UAJ
#   endif // CODEC1_MIC
#  else // !CODEC1_AMP
#   ifdef CODEC1_MIC
#    ifdef CODEC1_UAJ
//    2 Endpoints: Headphones, LineOut
#     define ENDPOINT_CNT  2
#     define EP00_PROPERTIES
#     define EP01_PROPERTIES
#     define EP02_PROPERTIES Package (2) {"acpi-acd-endpoint-0-properties", "EP02"},
#     define EP03_PROPERTIES Package (2) {"acpi-acd-endpoint-1-properties", "EP03"},
#     define EP04_PROPERTIES
#     define EP05_PROPERTIES
#     define EP06_PROPERTIES
#     define EP07_PROPERTIES
#    else // !CODEC1_UAJ
//    0 Endpoints:
#     define ENDPOINT_CNT  0
#     define EP00_PROPERTIES
#     define EP01_PROPERTIES
#     define EP02_PROPERTIES
#     define EP03_PROPERTIES
#     define EP04_PROPERTIES
#     define EP05_PROPERTIES
#     define EP06_PROPERTIES
#     define EP07_PROPERTIES
#    endif // CODEC1_UAJ
#   else // !CODEC1_MIC
#    ifdef CODEC1_UAJ
//    2 Endpoints: Headphones, LineOut
#     define ENDPOINT_CNT  2
#     define EP00_PROPERTIES
#     define EP01_PROPERTIES
#     define EP02_PROPERTIES Package (2) {"acpi-acd-endpoint-0-properties", "EP02"},
#     define EP03_PROPERTIES Package (2) {"acpi-acd-endpoint-1-properties", "EP03"},
#     define EP04_PROPERTIES
#     define EP05_PROPERTIES
#     define EP06_PROPERTIES
#     define EP07_PROPERTIES
#    else // !CODEC1_UAJ
//    0 Endpoints:
#     define ENDPOINT_CNT  0
#     define EP00_PROPERTIES
#     define EP01_PROPERTIES
#     define EP02_PROPERTIES
#     define EP03_PROPERTIES
#     define EP04_PROPERTIES
#     define EP05_PROPERTIES
#     define EP06_PROPERTIES
#     define EP07_PROPERTIES
#     error "No endpoints available!"
#    endif // CODEC1_UAJ
#   endif // CODEC1_MIC
#  endif // CODEC1_AMP
# else // !CODEC1_UID
// 0 Endpoints:
#  define ENDPOINT_CNT  0
#  define EP00_PROPERTIES
#  define EP01_PROPERTIES
#  define EP02_PROPERTIES
#  define EP03_PROPERTIES
#  define EP04_PROPERTIES
#  define EP05_PROPERTIES
#  define EP06_PROPERTIES
#  define EP07_PROPERTIES
#  error "No endpoints available!"
# endif // CODEC1_UID
#endif // _NUM_AMP_AMPS > 0

Name(_DSD, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
        Package (2) {"acpi-acd-interface-revision", 0x00000001},    // 0.1 Spec Version Bits 31-16: (Upper word) Major version number. Bits 15-0: (Lower word) Minor version number
        Package (2) {"acpi-acd-endpoint-count", ENDPOINT_CNT},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
        EP00_PROPERTIES
        EP01_PROPERTIES
        EP02_PROPERTIES
        EP03_PROPERTIES
        EP04_PROPERTIES
        EP05_PROPERTIES
        EP06_PROPERTIES
        EP07_PROPERTIES
    }
}) //End _DSD

#ifdef CODEC1_UID
# ifdef CODEC1_UAJ
Name(CC21, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),    // Device Properties UUID
    Package () {
       Package (2) {"acpi-acd-device-namestring", CODEC1_UAJ_DEV_NAME_B},
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
       Package (2) {"acpi-acd-device-namestring", CODEC1_UAJ_DEV_NAME_B},
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
       Package (2) {"acpi-acd-device-namestring", CODEC1_UAJ_DEV_NAME_B},
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
       Package (2) {"acpi-acd-device-namestring", CODEC1_UAJ_DEV_NAME_B},
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
       Package (2) {"acpi-acd-device-namestring", CODEC1_UAJ_DEV_NAME_B},
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
       Package (2) {"acpi-acd-device-namestring", CODEC1_UAJ_DEV_NAME_B},
       Package (2) {"acpi-acd-device-type", 1},    // 0: Generic, 1: SoundWire
       Package (2) {"acpi-acd-sdca-terminal-id", 0xB},    // Entity id of the Analog terminal used for this endpoint
       Package (2) {"acpi-acd-sdca-terminal-type", 0x06d0},    // Sdca Terminal Type based on Sdca Version implemented by Audio Function
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),    // Hierarchical Data Extension UUID
    Package () {
       Package (2) {"msft-acx-properties", "AC02"},    // Acx specific properties
    }
}) //End CC71
# endif // CODEC1_UAJ
#endif


//
// Cleanup
//
#ifdef _NUM_AMP_AMPS
# undef _NUM_AMP_AMPS
#endif

#ifdef _NUM_CODEC_AMPS
# undef _NUM_CODEC_AMPS
#endif

#ifdef ENDPOINT_CNT
# undef ENDPOINT_CNT
#endif

#ifdef EP00_PROPERTIES
# undef EP00_PROPERTIES
#endif

#ifdef EP01_PROPERTIES
# undef EP01_PROPERTIES
#endif

#ifdef EP02_PROPERTIES
# undef EP02_PROPERTIES
#endif

#ifdef EP03_PROPERTIES
# undef EP03_PROPERTIES
#endif

#ifdef EP04_PROPERTIES
# undef EP04_PROPERTIES
#endif

#ifdef EP05_PROPERTIES
# undef EP05_PROPERTIES
#endif

#ifdef EP06_PROPERTIES
# undef EP06_PROPERTIES
#endif

#ifdef EP07_PROPERTIES
# undef EP07_PROPERTIES
#endif
