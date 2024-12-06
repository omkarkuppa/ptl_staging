/** @file
  SoundWire topology definitions

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

#ifdef EXCLUDE_FU_113_VOLUME_CONTROL
# define FEATURE_CS42L43_DMIC_NO_VOL_MUTE_C_COND  0x00000000
#else
// Class driver creates DMIC mute and volume elements.
// XU opts out from creating DMIC mute and volume elements.
# define FEATURE_CS42L43_DMIC_NO_VOL_MUTE_C_COND  0x00000010
#endif

#ifndef CS42L43_FU_113_VOL_MIN
// Min = 0x0000CE00 (-64.0 dB) in Q7.8 format
# define CS42L43_FU_113_VOL_MIN 0x00, 0xC0, 0x00, 0x00
#endif

#ifndef CS42L43_FU_113_VOL_MAX
// Max = 0x00001F80 (+31.5 dB) in Q7.8 format
# define CS42L43_FU_113_VOL_MAX 0x80, 0x1F, 0x00, 0x00
#endif

#ifndef CS42L43_FU_113_VOL_FIXED_Q7_8
// FU113 Fixed = 0x00001F80 (+31.5 dB) in Q7.8 format
# define CS42L43_FU_113_VOL_FIXED_Q7_8 0x1F80
#endif

#ifndef CS42L43_FU_113_VOL_STEP
// Step = 0x00000008 (1/32 dB per step) in Q7.8 format
# define CS42L43_FU_113_VOL_STEP 0x08, 0x00, 0x00, 0x00
#endif

#ifndef CS42L43_FU_113_VOL_DEFAULT
// Default = 0x0 (0 dB) in Q7.8 format
# define CS42L43_FU_113_VOL_DEFAULT 0x0
#endif

#ifndef CS42L43_FU_113_VOL_FIXED
// Fixed = 0x0 (0 dB) in Q7.8 format
# define CS42L43_FU_113_VOL_FIXED 0x0
#endif

#ifndef CS42L43_FU_13_VOL_MIN
// Min = 0x0000CE00 (-64.0 dB) in Q7.8 format
# define CS42L43_FU_13_VOL_MIN 0x00, 0xC0, 0x00, 0x00
#endif

#ifndef CS42L43_FU_13_VOL_MAX
// Max = 0x00001F80 (+31.5 dB) in Q7.8 format
# define CS42L43_FU_13_VOL_MAX 0x80, 0x1F, 0x00, 0x00
#endif

#ifndef CS42L43_FU_13_VOL_STEP
// Step = 0x00000008 (1/32 dB per step) in Q7.8 format
# define CS42L43_FU_13_VOL_STEP 0x08, 0x00, 0x00, 0x00
#endif

#ifndef CS42L43_FU_13_VOL_DEFAULT
// Default = 0x0 (0 dB) in Q7.8 format
# define CS42L43_FU_13_VOL_DEFAULT 0x0
#endif

#ifndef CS42L43_FU_13_VOL_FIXED
// Fixed = 0x0 (0 dB) in Q7.8 format
# define CS42L43_FU_13_VOL_FIXED 0x0
#endif


// For details of these values, see Documentation AN0580R1
#ifndef CS42L43_FU_14_VOL_MIN
// Min = 0x00009C00 (-100.0 dB) in Q7.8 format
# define CS42L43_FU_14_VOL_MIN 0x00, 0x9C, 0x00, 0x00
#endif

#ifndef CS42L43_FU_14_VOL_MAX
// Max = 0x00000000 (0.0 dB) in Q7.8 format
# define CS42L43_FU_14_VOL_MAX 0x00, 0x00, 0x00, 0x00
#endif

#ifndef CS42L43_FU_14_VOL_STEP
// Step = 0x00000040 (0.25 dB per step) in Q7.8 format
# define CS42L43_FU_14_VOL_STEP 0x40, 0x00, 0x00, 0x00
#endif

#ifndef CS42L43_FU_14_VOL_DEFAULT
// Default = 0x0 (0 dB) in Q7.8 format
# define CS42L43_FU_14_VOL_DEFAULT 0x0
#endif

#include <SmartMic-Clusters.asl>
#include <SmartMic-Usage.asl>

//
// Microphone array geometry
// (KSAUDIO_MIC_ARRAY_GEOMETRY struct as a buffer KSPROPERTY_AUDIO_MIC_ARRAY_GEOMETRY - Windows drivers | Microsoft Learn)
// It is possible that this was overloaded before this file was included.  In this case MIC_GEOMETRY_OVERLOAD must be
// defined to keep this table from being included.  It is intentionally at the top of this file so that it dis-assembles into
// the same location when it is overloaded.
#ifndef MIC_GEOMETRY_OVERLOAD
Name(MGEO, Buffer()
{
    0x00, 0x01,     // usVersion = 0x0100
    0x00, 0x00,     // usMicArrayType = KSMICARRAY_MICTYPE_OMNIDIRECTIONAL
    0x52, 0xe1,     // wVerticalAngleBegin = -7854 = -(pi * 10000)/4 [0.0001 rad]
    0xae, 0x1e,     // wVerticalAngleEnd = 7854 = (pi * 10000)/4 [0.0001 rad]
    0x00, 0x00,     // wHorizontalAngleBegin = 0 [0.0001 rad]
    0x00, 0x00,     // wHorizontalAngleEnd = 0 [0.0001 rad]
    0x64, 0x00,     // usFrequencyBandLo = 100 [Hz]
    0x40, 0x1f,     // usFrequencyBandHi = 8000 [Hz]
    0x02, 0x00,     // usNumberOfMicrophones = 2
                    // KsMicCoord[0]
        0x02, 0x00, // usType = KSMICARRAY_MICTYPE_CARDIOID
        0x00, 0x00, // wXCoord = 0 [mm]
        0xe7, 0xff, // wYCoord = -25 [mm]
        0x00, 0x00, // wZCoord = 0 [mm]
        0x00, 0x00, // wVerticalAngle = 0 [0.0001 rad]
        0x00, 0x00, // wHorizontalAngle = 0 [0.0001 rad]
                    // KsMicCoord[1]
        0x02, 0x00, // usType = KSMICARRAY_MICTYPE_CARDIOID
        0x00, 0x00, // wXCoord = 0 [mm]
        0x19, 0x00, // wYCoord = 25 [mm]
        0x00, 0x00, // wZCoord = 0 [mm]
        0x00, 0x00, // wVerticalAngle = 0 [0.0001 rad]
        0x00, 0x00  // wHorizontalAngle = 0 [0.0001 rad]
}) //End MGEO
#endif

#ifdef EXCLUDE_FU_113_VOLUME_CONTROL
// Remove FU113 and FU13
# define COHEN_MAIN_PCM_MIC_STREAM_ENTITY_ID_LIST         0x4, 0x5, 0x2, 0x6, 0x1, 0x7, 0x9, 0xA
# define COHEN_SECONDARY_PCM_MIC_STREAM_ENTITY_ID_LIST    0x11, 0x13, 0x14, 0x15
#else   // !EXCLUDE_FU_113_VOLUME_CONTROL
// Define FU113 and FU13
# define COHEN_MAIN_PCM_MIC_STREAM_ENTITY_ID_LIST         0x4, 0x5, 0x2, 0x6, 0x1, 0x7, 0x8, 0x9, 0xA
# define COHEN_SECONDARY_PCM_MIC_STREAM_ENTITY_ID_LIST    0x11, 0x12, 0x13, 0x14, 0x15
#endif  // EXCLUDE_FU_113_VOLUME_CONTROL

#define COHEN_ULTRASONIC_PCM_STREAM_ENTITY_ID_LIST       0x1A, 0x16, 0x17, 0x1B, 0x18, 0x19

#ifdef HWKWS
# define COHEN_TOPOLOGY_FEATURES_BITS                    (1 << 21)
#else
# ifdef COHEN_ENABLE_US_CAPTURE_STREAM
#  define COHEN_TOPOLOGY_FEATURES_BITS                   (1 << 17)
# else
#  define COHEN_TOPOLOGY_FEATURES_BITS                   0
# endif // COHEN_ENABLE_US_CAPTURE_STREAM
#endif // HWKWS

// COHEN_BRIDGE_SIDECAR_REGISTER_ACCESS_ADDRESS_SELECTOR
#define CS42L43_MIC_E0_CTL_SIDECAR_REG_ADDR (1 << 0x30)

// COHEN_BRIDGE_SIDECAR_REGISTER_ACCESS_VALUE_SELECTOR
#define CS42L43_MIC_E0_CTL_SIDECAR_REG_VAL  (1 << 0x31)

Name(_DSD, Package()
{
    // Device properties UUID
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package() {
        Package(2) { "mipi-sdw-sw-interface-revision", 0x00020001 },  // Revision 2.1
        Package(2) { "mipi-sdca-function-topology-features", COHEN_TOPOLOGY_FEATURES_BITS },
        Package(2) { "mipi-sdca-control-list", (CTL_E0_FUNCTION_VERSION | CTL_E0_FUNCTION_ID | CTL_E0_FUNCTION_MANUFACTURER_ID |
                                                CTL_E0_FUNCTION_TYPE | CTL_E0_FUNCTION_SDCA_VERSION | CTL_E0_FUNCTION_STATUS_COND |
                                                CTL_E0_DEVICE_MANUFACTURER_ID | CTL_E0_DEVICE_PART_ID | CTL_E0_DEVICE_VERSION | CTL_E0_DEVICE_SDCA_VERSION |
                                                CS42L43_MIC_E0_CTL_SIDECAR_REG_ADDR | CS42L43_MIC_E0_CTL_SIDECAR_REG_VAL
                                                )},
#ifdef HWKWS
        // Traversing from IT --> OT: IT11, CS11, PDE11, FU11, XU12, MFPU113, FU113, CS113, OT113, MFPU13, FU13, SMPU13, CS13, OT13.
        Package(2) { "mipi-sdca-entity-id-list", Package() { COHEN_MAIN_PCM_MIC_STREAM_ENTITY_ID_LIST, COHEN_SECONDARY_PCM_MIC_STREAM_ENTITY_ID_LIST } },
# ifdef COHEN_ENABLE_US_CAPTURE_STREAM
#  error "Cannot enable UltraSound capture stream when HWKWS is enabled!"
# endif
#else   // !HWKWS
# ifdef COHEN_ENABLE_US_CAPTURE_STREAM
        // Traversing from IT --> OT: IT11, CS11, PDE11, FU11, XU12, MFPU113, FU113, CS113, OT113, CRU14, FU14, SMPU14, MFPU14, CS14, OT14.
        Package(2) { "mipi-sdca-entity-id-list", Package() { COHEN_MAIN_PCM_MIC_STREAM_ENTITY_ID_LIST, COHEN_ULTRASONIC_PCM_STREAM_ENTITY_ID_LIST } },
# else  // !COHEN_ENABLE_US_CAPTURE_STREAM
        // Traversing from IT --> OT: IT11, CS11, PDE11, FU11, XU12, MFPU113, FU113, CS113, OT113.
        Package(2) { "mipi-sdca-entity-id-list", Package() { COHEN_MAIN_PCM_MIC_STREAM_ENTITY_ID_LIST } },
# endif // COHEN_ENABLE_US_CAPTURE_STREAM
#endif // HWKWS
        Package(2) { "mipi-sdca-function-busy-max-delay", 5000},
        // Clusters
        CLUSTER_ID_LIST_DMIC,
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        // Function-level
        Package(2) { "mipi-sdca-control-0x4-subproperties", "C004"},
        Package(2) { "mipi-sdca-control-0x5-subproperties", "C005"},
        Package(2) { "mipi-sdca-control-0x6-subproperties", "C006"},
        Package(2) { "mipi-sdca-control-0x7-subproperties", "C007"},
        Package(2) { "mipi-sdca-control-0x8-subproperties", "C008"},
#ifndef EXCLUDE_FUN_STS
        Package(2) { "mipi-sdca-control-0x10-subproperties", "C010"},
#endif
        Package(2) { "mipi-sdca-control-0x2C-subproperties", "C02C"},
        Package(2) { "mipi-sdca-control-0x2D-subproperties", "C02D"},
        Package(2) { "mipi-sdca-control-0x2E-subproperties", "C02E"},
        Package(2) { "mipi-sdca-control-0x2F-subproperties", "C02F"},
        Package(2) { "mipi-sdca-control-0x30-subproperties", "C030"},       // COHEN_BRIDGE_SIDECAR_REGISTER_ACCESS_ADDRESS_SELECTOR
        Package(2) { "mipi-sdca-control-0x31-subproperties", "C031"},       // COHEN_BRIDGE_SIDECAR_REGISTER_ACCESS_VALUE_SELECTOR

        // Entity-level
        Package(2) { "mipi-sdca-entity-id-0x4-subproperties", "E004"},  // IT11
        Package(2) { "mipi-sdca-entity-id-0x5-subproperties", "E005"},  // CS11
        Package(2) { "mipi-sdca-entity-id-0x2-subproperties", "E002"},  // PDE11
        Package(2) { "mipi-sdca-entity-id-0x6-subproperties", "E006"},  // FU11
        Package(2) { "mipi-sdca-entity-id-0x1-subproperties", "E001"},  // XU12
        Package(2) { "mipi-sdca-entity-id-0x7-subproperties", "E007"},  // MFPU113
#ifndef EXCLUDE_FU_113_VOLUME_CONTROL
        Package(2) { "mipi-sdca-entity-id-0x8-subproperties", "E008"},  // FU113
#endif  // !EXCLUDE_FU_113_VOLUME_CONTROL
        Package(2) { "mipi-sdca-entity-id-0x9-subproperties", "E009"},  // CS113
        Package(2) { "mipi-sdca-entity-id-0xA-subproperties", "E00A"},  // OT113
        Package(2) { "mipi-sdca-entity-id-0x11-subproperties", "E011"}, // MFPU13
#ifndef EXCLUDE_FU_113_VOLUME_CONTROL
        Package(2) { "mipi-sdca-entity-id-0x12-subproperties", "E012"}, // FU13
#endif  // !EXCLUDE_FU_113_VOLUME_CONTROL
        Package(2) { "mipi-sdca-entity-id-0x13-subproperties", "E013"}, // SMPU13
        Package(2) { "mipi-sdca-entity-id-0x14-subproperties", "E014"}, // CS13
        Package(2) { "mipi-sdca-entity-id-0x15-subproperties", "E015"}, // OT13
        Package(2) { "mipi-sdca-entity-id-0x1A-subproperties", "E01A"}, // CRU14
        Package(2) { "mipi-sdca-entity-id-0x16-subproperties", "E016"}, // FU14
        Package(2) { "mipi-sdca-entity-id-0x17-subproperties", "E017"}, // SMPU14
        Package(2) { "mipi-sdca-entity-id-0x1B-subproperties", "E01B"}, // MFPU14
        Package(2) { "mipi-sdca-entity-id-0x18-subproperties", "E018"}, // CS14
        Package(2) { "mipi-sdca-entity-id-0x19-subproperties", "E019"}, // OT14
        // Clusters
        CLUSTER_INIT_DMIC,
        // Vendor-specific
        Package(2) { "mipi-sdca-function-expansion-subproperties", "EXT0"},
    },
    //
    // For WIN10 compatibility, the Buffer Data Extension cannot be empty.
    //
#ifndef EXCLUDE_FUN_STS
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        // Class driver will print warnings if these are not defined.
        // To not pollute the class driver logs, simply do not define these if we are not using them.
        Package(2) { "mipi-sdca-function-initialization-table", "BUF0"},
    }
#endif
}) // End _DSD


#ifndef EXCLUDE_FUN_STS
// Function initialization-table
Name(BUF0, Buffer()
{
    //
    // FDL config
    //
    0x00, 0xff, 0x11, 0x00, 0x01,   // 0x0011FF00 = 0x1 (FILE_SET_5)
    0x01, 0xff, 0x11, 0x00, 0x00,
    0x02, 0xff, 0x11, 0x00, 0x00,
    0x03, 0xff, 0x11, 0x00, 0x00,

    0x04, 0xff, 0x11, 0x00, 0x01,   // 0x0011FF04 = 0x1 (FILE_SET_6)
    0x05, 0xff, 0x11, 0x00, 0x00,
    0x06, 0xff, 0x11, 0x00, 0x00,
    0x07, 0xff, 0x11, 0x00, 0x00,

    0x0C, 0x40, 0x11, 0x00, COHEN_NEED_CONFIGS_VAL_0C,  // 0x0011400C = 800000xx (NEED_CONFIGS)
    0x0D, 0x40, 0x11, 0x00, 0x00,
    0x0E, 0x40, 0x11, 0x00, 0x00,
    0x0F, 0x40, 0x11, 0x00, 0x80,

    0x04, 0x40, 0x11, 0x00, 0x00,   // 0x00114004 = 00014800 (PATCH_START)
    0x05, 0x40, 0x11, 0x00, 0x48,
    0x06, 0x40, 0x11, 0x00, 0x01,
    0x07, 0x40, 0x11, 0x00, 0x00,

    0xF8, 0xFB, 0x11, 0x00, 0x00,   // 0x0011FBF8 = 00014800(FW_PATCH)
    0xF9, 0xFB, 0x11, 0x00, 0x48,
    0xFA, 0xFB, 0x11, 0x00, 0x01,
    0xFB, 0xFB, 0x11, 0x00, 0x00,
    //
    // FDL config ends here
    //
}) // End BUF0
#endif


// +------------------------------------+
// |    Function level controls         |
// +------------------------------------+

Name(EXT0, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
#ifdef COHEN_BRIDGE
# ifdef SIDECAR_VARIABLE_SPEAKER_SELECT
        // NOTE: 01fa-spk-id-val must ALWAYS be the first package inside the
        // inner package of EXT0. It will be updated at ACPI initialization time
        // by the _INI method in the SDCA function ACPI driver node for this
        // device.
        Package(2) {"01fa-spk-id-val", 0}, // value to be set by _INI function
# endif
        #include <Sidecar_Cohen_Tweeter_Jamerson_Woofer.asl>
#endif
        Package(2) { "mipi-sdca-function-expansion-subsystem-id", 0 },  // MIPI required, but not used by MSFT
        Package(2) { "01fa-chip-id", 0x4243 },
        Package(2) { "01fa-ssid-ex", 0x7 },
        //
        // This property is used to select HWKWS FW.
        // Since the audio function performing the FDL is selected at random,
        // all audio functions involved with FDL must have it defined.
        Package(2) {"01fa-xu-features", (FEATURE_ENABLE_HWKWS | FEATURE_ENABLE_WT | FEATURE_ENABLE_KNCK | FEATURE_NO_FUN_STS |
                                         FEATURE_CS42L43_DMIC_NO_VOL_MUTE_C_COND)},
    }
}) // End EXT0


// Function-level control definitions
Name(C004, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Function_SDCA_Version = 1.0
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS },
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC },
        Package(2) { "mipi-sdca-control-dc-value", CTL_E0_FUNCTION_SDCA_VERSION_VAL },
    }
}) // End C004

Name(C005, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Function_Type = 0x3 (Smart Mic)
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x03}
    }
}) // End C005

Name(C006, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Function_Manufacturer_Id = 0x01FA
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x01FA},
    }
}) // End C006

Name(C007, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Function_Id = 0x4243
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x4243},
    }
}) // End C007

Name(C008, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Shows up as VER in HWID
        // Class, DC, Function_Version = 0x1
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x1},
    }
}) // End C008

#ifndef EXCLUDE_FUN_STS
Name(C010, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Class, RW1C, Function_Status
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_RW1C},
        Package(2) { "mipi-sdca-control-interrupt-position", 7},
    }
}) // End C010
#endif


Name(C02C, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Device_Manufacturer_ID: Class, DC = 0x01FA
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x01FA},
    }
}) // End C02C


Name(C02D, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Device_Part_ID: Class, DC = 0x4243
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x4243},
    }
}) // End C02D


Name(C02E, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Device_Version: Class, DC = 0xA1
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0xA1},
    }
}) // End C02E


Name(C02F, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Device_SDCA_Version: Class, DC = 0x10
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", CTL_E0_DEVICE_SDCA_VERSION_VAL},
    }
}) // End C02F


Name(C030, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Extension/Platform, RW, COHEN_BRIDGE_SIDECAR_REGISTER_ACCESS_ADDRESS_SELECTOR
        Package(2) { "mipi-sdca-control-access-layer", CAL_EXTENSION | CAL_PLATFORM},
        Package(2) { "mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-cn-list", 0x6 }, // 0x1, 0x2
    }
}) // End C030


Name(C031, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Extension/Platform, RW, COHEN_BRIDGE_SIDECAR_REGISTER_ACCESS_VALUE_SELECTOR
        Package(2) { "mipi-sdca-control-access-layer", CAL_EXTENSION | CAL_PLATFORM},
        Package(2) { "mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-cn-list", 0x6 }, // 0x1, 0x2
    }
}) // End C031


// +------------------------------------+
// |                IT 11               |
// |     Mic Array Transducer Input     |
// |     Trasnducer Input Terminal      |
// +------------------------------------+
Name(E004, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x02},
        Package(2) { "mipi-sdca-entity-label", "IT 11"},
        Package(2) { "mipi-sdca-terminal-type", 0x205}, // Microphone Array Transducer Input
        Package(2) { "mipi-sdca-control-list", CTL_IT_CLUSTERINDEX | CTL_IT_LATENCY | CTL_IT_USAGE},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        USAGE_IT_11,
        Package(2) { "mipi-sdca-control-0x8-subproperties", "C108"},
        CLUSTER_IT_11,
        Package(2) { "mipi-sdca-terminal-clock-connection", "E005" }, // CS 11
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package() {
        Package (2) {"mipi-sdca-microphone-array-geometry", "MGEO"},
        Package (2) {"mipi-sdca-microphone-array-sensitivity", "MSEN"},
        Package (2) {"mipi-sdca-microphone-array-snr", "MSNR"},
    }
}) // End E004

//
// Microphone sensitivity
// (16.16 fixed point value KSPROPERTY\_AUDIO\_MIC\_SENSITIVITY - Windows drivers | Microsoft Learn)
Name(MSEN, Buffer()
{
    0x00, 0x01,             // Version
    0x02, 0x00,             // NumberOfMicrophones
    0x00, 0x00, 0xe6, 0xFF, // Mic 1 Sensitivity, Knowles KAS-700-0145, sensitivity= -26.0 dB (+/-1 dB)
    0x00, 0x00, 0xe6, 0xFF, // Mic 2 Sensitivity, Knowles KAS-700-0145, sensitivity= -26.0 dB (+/-1 dB)
}) //End MSEN

//
// Microphone snr
// (16.16 fixed point value KSPROPERTY\_AUDIO\_MIC\_SNR - Windows drivers | Microsoft Learn)
Name(MSNR, Buffer()
{
    0x00, 0x01,             // Version
    0x02, 0x00,             // NumberOfMicrophones
    0x00, 0x80, 0x40, 0x00, // Mic 1 SNR, Knowles KAS-700-0145, SNR= 64.5 dB
    0x00, 0x80, 0x40, 0x00, // Mic 2 SNR, Knowles KAS-700-0145, SNR= 64.5 dB
}) //End MSNR

Name(C108, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Latency = 0x4 (dummy value)
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 0x4},
    }
}) // End C108


// +------------------------------------+
// |                CS 11               |
// +------------------------------------+
Name(E005, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0xB},
        Package(2) { "mipi-sdca-entity-label", "CS 11"},
        Package(2) { "mipi-sdca-cs-type", 0},
        Package(2) { "mipi-sdca-control-list", CTL_CS_CLOCK_VALID},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x2-subproperties", "C202"},
    }
}) // End E005


Name(C202, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, RO, Clock_Valid
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 3},
        Package(2) { "mipi-sdca-control-deferrable", 1},
    }
}) // End C202



// +------------------------------------+
// |                PDE 11              |
// +------------------------------------+
Name(E002, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x11 },
        Package(2) { "mipi-sdca-entity-label", "PDE 11"},
        Package(2) { "mipi-sdca-control-list", CTL_PDE_ACTUAL_PS | CTL_PDE_REQUESTED_PS},
        Package(2)
        {
            "mipi-sdca-powerdomain-managed-list",
            Package() {0x4}, // Entity ID 0x4 (IT11) managed by this PDE.
        },
        Package(2)
        {
            "mipi-sdca-powerdomain-transition-typical-delay",
            Package()
            {
                3, 0, 500000,  // From_PS, To_PS, Delay in microseconds
                0, 3, 500000,
            },
        },
        Package(2)
        {
            "mipi-sdca-powerdomain-transition-max-delay",
            Package()
            {
                3, 0, 800000,  // From_PS, To_PS, Delay in microseconds
                0, 3, 800000,
            },
        },
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "RPS0" }, // Requested PS
        Package(2) { "mipi-sdca-control-0x10-subproperties", "APS0"}, // Actual PS
    }
}) // End E002

Name(RPS0, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Requested_PS, Class, R/W
        Package(2) { "mipi-sdca-control-access-layer", 4 },
        Package(2) { "mipi-sdca-control-access-mode", 0},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "PDER"},
    }
}) // End RPS0


Name(PDER, Buffer()
{
    0x01, 0x00,  // Range type 0x0001
    0x02, 0x00,  // Count of ranges = 0x2
    0x00, 0x00, 0x00, 0x00, // 0 --> PS0
    0x03, 0x00, 0x00, 0x00, // 3 --> PS3
}) // End PDER

Name(APS0, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {  // Actual_PS, Class, RO
        Package(2) { "mipi-sdca-control-access-layer", 4 },
        Package(2) { "mipi-sdca-control-access-mode", 3},
    }
}) // End APS0



// +------------------------------------+
// |                FU 11               |
// +------------------------------------+
Name(E006, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x7},
        Package(2) { "mipi-sdca-entity-label", "FU 11"},
        Package(2) { "mipi-sdca-control-list", 0},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        //Package(2) { "mipi-sdca-control-0x1-subproperties", "C801"}, // Mute
        //Package(2) { "mipi-sdca-control-0xB-subproperties", "C80B"}, // Master Gain
        //Package(2) { "mipi-sdca-control-0x10-subproperties", "C810"}, // Latency
        Package(2) { "mipi-sdca-input-pin-1", "E004"}, // Input Pin 1 connected to IT 11
    }
}) // End E006

Name(C801, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Platform, R/W
        Package(2) { "mipi-sdca-control-access-layer", 8}, // Platform
        Package(2) { "mipi-sdca-control-access-mode", 0},  // R/W
    }
}) // End C801

Name(C80B, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Platform, R/W
        Package(2) { "mipi-sdca-control-access-layer", 8}, // Platform
        Package(2) { "mipi-sdca-control-access-mode", 0},  // R/W
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-cn-list", 0x7}, // Control Numbers = {0,1,2}
    }
}) // End C80B

Name(C810, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Platform, DisCo Const, Latency = 0xFF
        Package(2) { "mipi-sdca-control-access-layer", 8}, // Platform
        Package(2) { "mipi-sdca-control-access-mode", 5},  // DisCo Const
        Package(2) { "mipi-sdca-control-dc-value", 0xFF},
    }
}) // End C810



// +------------------------------------+
// |                XU 12               |
// +------------------------------------+
Name(E001, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x0A},
        Package(2) { "mipi-sdca-entity-label", "XU 12"},
        Package(2) { "mipi-sdca-input-pin-list", 0x2}, // Pin1 has a connection
        Package(2) { "mipi-sdca-control-list", CTL_XU_VERSION | CTL_XU_ID | CTL_XU_BYPASS},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C301"}, // Bypass
        Package(2) { "mipi-sdca-control-0x7-subproperties", "C307"}, // XU_ID
        Package(2) { "mipi-sdca-control-0x8-subproperties", "C308"}, // XU_Version
        Package(2) { "mipi-sdca-input-pin-1", "E006"}, // Input Pin 1 connected to FU 11
    }
}) // End E001

Name(C301, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Bypass, Class, RW.
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 0},
        Package(2) { "mipi-sdca-control-deferrable", 1},
    }
}) // End C301

Name(C307, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, XU_ID
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 1},
    }
}) // End C307

Name(C308, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, XU_Version
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 0x10},
    }
}) // End C308



// +------------------------------------+
// |            MFPU 113                |
// +------------------------------------+
Name(E007, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x22},
        Package(2) { "mipi-sdca-entity-label", "MFPU 113"},
        Package(2) { "mipi-sdca-control-list", CTL_MFPU_CLUSTER_INDEX | CTL_MFPU_BYPASS},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C401"}, // Bypass
        CLUSTER_MFPU_113,
        Package(2) { "mipi-sdca-input-pin-1", "E001"}, // Input Pin 1 connected to XU 12
    }
}) // End E007

Name(C401, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Bypass, Class, DC. Defining as 'Null' MFPU (DC:1)
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 0x1},
    }
}) // End C401


#ifndef EXCLUDE_FU_113_VOLUME_CONTROL
// +------------------------------------+
// |                FU 113              |
// +------------------------------------+
Name(E008, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x7},
        Package(2) { "mipi-sdca-entity-label", "FU 113"},
        Package(2) { "mipi-sdca-control-list", CTL_FU_LATENCY | CTL_FU_VOLUME | CTL_FU_MUTE},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C501"}, // Mute
        Package(2) { "mipi-sdca-control-0x2-subproperties", "C502"}, // Volume
        Package(2) { "mipi-sdca-control-0x10-subproperties", "C510"}, // Latency
        Package(2) { "mipi-sdca-input-pin-1", "E007"}, // Input Pin 1 connected to MFPU 113
    }
}) // End E008

Name(C501, Package()
{
    // Mute
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, Dual, Control Numbers = {1,2}
        Package(2) { "mipi-sdca-control-access-layer", CAL_USER},   // User, but spec says Class
        Package(2) { "mipi-sdca-control-access-mode", CAM_DUAL},    // Dual R/W
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-cn-list", 0x6},   // Control Numbers = {1,2}
#ifdef FU_113_FIXED_CAPTURE_MUTE
        Package(2) { "mipi-sdca-control-fixed-value", 0},   // unmuted
#endif
    }
}) // End C501

Name(C502, Package()
{
    // Volume
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, Dual, Control Numbers = {1,2}
        Package(2) { "mipi-sdca-control-access-layer", CAL_USER},   // User, but spec says Class
        Package(2) { "mipi-sdca-control-access-mode", CAM_DUAL},    // Dual R/W
#ifdef FU_113_FIXED_CAPTURE_VOLUME
        Package(2) { "mipi-sdca-control-fixed-value", CS42L43_FU_113_VOL_FIXED_Q7_8},
#else
        Package(2) { "mipi-sdca-control-default-value", CS42L43_FU_113_VOL_DEFAULT},
#endif
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-cn-list", 0x6}, // Control Numbers = {1,2}
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "BUF4"},
    }
}) // End C502

Name(BUF4, Buffer()
{
    0x03, 0x00,  // Range type 0x0003 (Triples)
    0x01, 0x00,  // Count of ranges = 0x1
    CS42L43_FU_113_VOL_MIN,
    CS42L43_FU_113_VOL_MAX,
    CS42L43_FU_113_VOL_STEP,
}) // End BUF4

Name(C510, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Latency = 0x8
        Package(2) { "mipi-sdca-control-access-layer", CAL_USER},   // User, but spec says Class
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x8}, // Dummy value for test
    }
}) // End C510
#endif  // !EXCLUDE_FU_113_VOLUME_CONTROL


// +------------------------------------+
// |                CS 113              |
// +------------------------------------+
Name(E009, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0xB},
        Package(2) { "mipi-sdca-entity-label", "CS 113"},
        Package(2) { "mipi-sdca-cs-type", 0},
        Package(2) { "mipi-sdca-control-list", CTL_CS_SAMPLERATEINDEX},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x10-subproperties", "C610"},
    }
}) // End E009

Name(C610, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, RW, SampleRateIndex
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 0},
        Package(2) { "mipi-sdca-control-deferrable", 1},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "BUF5"},
    }
}) // End C610

// {SampleRateIndex, SampleRate} mapping.
Name(BUF5, Buffer()
{
    0x02, 0x00,  // Range type 0x0002 (Pairs)
    0x01, 0x00,  // Count of ranges = 0x1;
    0x00, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00, 0x00, // SampleRateIndex = 0x00000000, SampleRate = 48000 (0x0000BB80)
    //0x01, 0x00, 0x00, 0x00, 0x44, 0xAC, 0x00, 0x00, // SampleRateIndex = 0x00000001, SampleRate = 44100 (0x0000AC44)
    //0x02, 0x00, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, // SampleRateIndex = 0x00000002, SampleRate = 96000 (0x00017700)
    //0x03, 0x00, 0x00, 0x00, 0x80, 0x3e, 0x00, 0x00, // SampleRateIndex = 0x00000003, SampleRate = 16000 (0x00003e80)
}) // End BUF5



// +------------------------------------+
// |                OT 113              |
// |     Capture Direct PCM Stream      |
// |     Streaming Output Terminal      |
// +------------------------------------+
Name(E00A, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x03},
        Package(2) { "mipi-sdca-entity-label", "OT 113"},
        Package(2) { "mipi-sdca-terminal-type", 0x181}, // Raw PCM Mic Source Data Port
        Package(2) { "mipi-sdca-input-pin-list", 0x2}, // Pin1 has a connection
        Package(2) { "mipi-sdca-control-list", CTL_OT_DATAPORT_SELECTOR | CTL_OT_LATENCY | CTL_OT_USAGE},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
#ifdef EXCLUDE_FU_113_VOLUME_CONTROL
        Package(2) { "mipi-sdca-input-pin-1", "E007"}, // Input Pin 1 connected to MFPU 113
#else
        Package(2) { "mipi-sdca-input-pin-1", "E008"}, // Input Pin 1 connected to FU 113
#endif  // EXCLUDE_FU_113_VOLUME_CONTROL
        USAGE_OT_113,
        Package(2) { "mipi-sdca-control-0x8-subproperties", "C708"}, // Latency
        Package(2) { "mipi-sdca-control-0x11-subproperties", "C711"}, // DataPortSelector
        Package(2) { "mipi-sdca-terminal-clock-connection", "E009" }, // CS 113
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-terminal-dp-numbers", "BUF6"},
    }
}) // End E00A

Name(BUF6, Buffer() {
    0x1, 0x1, // {DP_Index, DP_Num} (DMIC)
}) // End BUF6


Name(C708, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Latency = 0xA (dummy value)
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 0xA},
    }
}) // End C708

Name(C711, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // DataPort Selector *index* = 1 (see {DP_Index, DP_Num} map), Class, DC
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 1},
    }
}) // End C711


// +------------------------------------+
// |            MFPU 13                |
// +------------------------------------+
Name(E011, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x22},
        Package(2) { "mipi-sdca-entity-label", "MFPU 13"},
        Package(2) { "mipi-sdca-control-list", CTL_MFPU_CLUSTER_INDEX | CTL_MFPU_BYPASS},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C901"},    // Bypass
        CLUSTER_MFPU_13,
        Package(2) { "mipi-sdca-input-pin-1", "E001"}, // Input Pin 1 connected to XU 12
    }
}) // End E011

Name(C901, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Bypass, Class, DC. Defining as 'Null' MFPU (DC:1)
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 0x1},
    }
}) // End C901


#ifndef EXCLUDE_FU_113_VOLUME_CONTROL
// +------------------------------------+
// |                FU 13               |
// +------------------------------------+
Name(E012, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x7},
        Package(2) { "mipi-sdca-entity-label", "FU 13"},
        Package(2) { "mipi-sdca-control-list", CTL_FU_LATENCY | CTL_FU_VOLUME | CTL_FU_MUTE},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "CA01"},    // Mute
        Package(2) { "mipi-sdca-control-0x2-subproperties", "CA02"},    // Channel Volume
        Package(2) { "mipi-sdca-control-0x10-subproperties", "CA10"},   // Latency
        Package(2) { "mipi-sdca-input-pin-1", "E011"}, // Input Pin 1 connected to MFPU 13
    }
}) // End E012

Name(CA01, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, R/W, fixed - not acted upon by the class driver
        Package(2) { "mipi-sdca-control-access-layer", CAL_USER},       // User, but spec says Class
        Package(2) { "mipi-sdca-control-access-mode", CAM_READ_WRITE},  // R/W
        Package(2) { "mipi-sdca-control-deferrable", 1},
#ifdef FU_113_FIXED_CAPTURE_MUTE
        Package(2) { "mipi-sdca-control-fixed-value", 0},   // unmuted
#else
        Package(2) { "mipi-sdca-control-default-value", 0x1}, // muted
#endif
        Package(2) { "mipi-sdca-control-cn-list", 0x6},   // Control Numbers = {1,2}
    }
}) // End CA01

Name(CA02, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, R/W, fixed - not acted upon by the class driver
        Package(2) { "mipi-sdca-control-access-layer", CAL_USER},       // User, but spec says Class
        Package(2) { "mipi-sdca-control-access-mode", CAM_READ_WRITE},  // R/W
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-fixed-value", CS42L43_FU_13_VOL_FIXED},
        Package(2) { "mipi-sdca-control-cn-list", 0x6},   // Control Numbers = {1,2}
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "BUFA"},
    }
}) // End CA02

Name(BUFA, Buffer()
{
    0x03, 0x00,  // Range type 0x0003 (Triples)
    0x01, 0x00,  // Count of ranges = 0x1
    CS42L43_FU_13_VOL_MIN,
    CS42L43_FU_13_VOL_MAX,
    CS42L43_FU_13_VOL_STEP,
}) // End BUFA

Name(CA10, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DisCo Const, Latency = 0xFF
        Package(2) { "mipi-sdca-control-access-layer", CAL_USER},   // User, but spec says Class
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},      // DisCo Const
        Package(2) { "mipi-sdca-control-dc-value", 0xFF},
    }
}) // End CA10
#endif  // !EXCLUDE_FU_113_VOLUME_CONTROL


// +------------------------------------+
// |             SMPU 13                |
// +------------------------------------+
Name(E013, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x23},
        Package(2) { "mipi-sdca-entity-label", "SMPU 13"},
        Package(2) { "mipi-sdca-control-list", (CTL_SMPU_HIST_MESSAGELENGTH | CTL_SMPU_HIST_MESSAGEOFFSET | CTL_SMPU_HIST_CURRENTOWNER |
                                                CTL_SMPU_TRIGGER_READY | CTL_SMPU_HIST_ERROR | CTL_SMPU_HIST_BUFFER_PREAMBLE |
                                                CTL_SMPU_HIST_BUFFER_MODE | CTL_SMPU_TRIGGER_STATUS | CTL_SMPU_TRIGGER_ENABLE)}, // Simple History Buffer mode Saurin: Changed 1A to 1B
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x10-subproperties", "CB10"}, // Trigger_Enable
        Package(2) { "mipi-sdca-control-0x11-subproperties", "CB11"}, // Trigger_Status
        Package(2) { "mipi-sdca-control-0x12-subproperties", "CB12"}, // Hist_Buffer_Mode
        Package(2) { "mipi-sdca-control-0x13-subproperties", "CB13"}, // Hist_Buffer_Preamble
        Package(2) { "mipi-sdca-control-0x14-subproperties", "CB14"}, // Hist_Error
        Package(2) { "mipi-sdca-control-0x16-subproperties", "CB16"}, // Trigger_Ready
        Package(2) { "mipi-sdca-control-0x18-subproperties", "CB18"}, // Hist_CurrentOwner
        Package(2) { "mipi-sdca-control-0x1A-subproperties", "CB1A"}, // Hist_MessageOffset
        Package(2) { "mipi-sdca-control-0x1B-subproperties", "CB1B"}, // Hist_MessageLength
#ifdef EXCLUDE_FU_113_VOLUME_CONTROL
        Package(2) { "mipi-sdca-input-pin-1", "E011"}, // Input Pin 1 connected to MFPU 13
#else
        Package(2) { "mipi-sdca-input-pin-1", "E012"}, // Input Pin 1 connected to FU 13
#endif  // EXCLUDE_FU_113_VOLUME_CONTROL
    }
}) // End E013

Name(CB10, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, RW, Trigger_Enable
        Package(2) { "mipi-sdca-control-access-layer", 4}, // Class
        Package(2) { "mipi-sdca-control-access-mode", 0},  // RW
        Package(2) { "mipi-sdca-control-deferrable", 1},
    }
}) // End CB10

Name(CB11, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, RW1C, Trigger_Status
        Package(2) { "mipi-sdca-control-access-layer", 4}, // Class
        Package(2) { "mipi-sdca-control-access-mode", 2},  // RW1C
        Package(2) { "mipi-sdca-control-deferrable", 1},
    }
}) // End CB11

Name(CB12, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Hist_Buffer_Mode = SHB
        Package(2) { "mipi-sdca-control-access-layer", 4}, // Class
        Package(2) { "mipi-sdca-control-access-mode", 5},  // DC
        //Package(2) { "mipi-sdca-control-dc-value", 0x3 },    // No History Buffer (NHB)
        Package(2) { "mipi-sdca-control-dc-value", 0x0 },    // Simple History Buffer (SHB)
    }
}) // End CB12

Name(CB13, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Hist_Buffer_Preamble = 0
        Package(2) { "mipi-sdca-control-access-layer", 4}, // Class
        Package(2) { "mipi-sdca-control-access-mode", 5},  // DC
        Package(2) { "mipi-sdca-control-dc-value", 0 },
    }
}) // End CB13

Name(CB14, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, RO, Hist_Error
        Package(2) { "mipi-sdca-control-access-layer", 4}, // Class
        Package(2) { "mipi-sdca-control-access-mode", 3},  // RO
    }
}) // End CB14

Name(CB16, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Trigger_Ready = 0
        Package(2) { "mipi-sdca-control-access-layer", 4}, // Class
        Package(2) { "mipi-sdca-control-access-mode", 5},  // DC
        Package(2) { "mipi-sdca-control-dc-value", 0 },
    }
}) // End CB16

Name(CB18, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, RW1S, Hist_CurrentOwner
        Package(2) { "mipi-sdca-control-access-layer", 4}, // Class
        Package(2) { "mipi-sdca-control-access-mode", 4},  // RW1S
    }
}) // End CB18

Name(CB1A, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Hist_MessageOffset = 0
        Package(2) { "mipi-sdca-control-access-layer", 4}, // Class
        Package(2) { "mipi-sdca-control-access-mode", 5},  // DC
        Package(2) { "mipi-sdca-control-dc-value", 0 },
    }
}) // End CB1A

Name(CB1B, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Hist_MessageLength = 0
        Package(2) { "mipi-sdca-control-access-layer", 4}, // Class
        Package(2) { "mipi-sdca-control-access-mode", 5},  // DC
        Package(2) { "mipi-sdca-control-dc-value", 0 },
    }
}) // End CB1B


// +------------------------------------+
// |                CS 13               |
// +------------------------------------+
Name(E014, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0xB},
        Package(2) { "mipi-sdca-entity-label", "CS 13"},
        Package(2) { "mipi-sdca-cs-type", 0},
        Package(2) { "mipi-sdca-control-list", CTL_CS_SAMPLERATEINDEX},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x10-subproperties", "CC10"},
    }
}) // End E014

Name(CC10, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, RW, SampleRateIndex
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 0},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "BUF7"},
    }
}) // End CC10

// {SampleRateIndex, SampleRate} mapping.
Name(BUF7, Buffer()
{
    0x02, 0x00,  // Range type 0x0002 (Pairs)
    0x01, 0x00,  // Count of ranges = 0x1;
    //0x00, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00, 0x00, // SampleRateIndex = 0x00000000, SampleRate = 48000 (0x0000BB80)
    //0x01, 0x00, 0x00, 0x00, 0x44, 0xAC, 0x00, 0x00, // SampleRateIndex = 0x00000001, SampleRate = 44100 (0x0000AC44)
    //0x02, 0x00, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, // SampleRateIndex = 0x00000002, SampleRate = 96000 (0x00017700)
    0x03, 0x00, 0x00, 0x00, 0x80, 0x3e, 0x00, 0x00, // SampleRateIndex = 0x00000003, SampleRate = 16000 (0x00003e80)
}) // End BUF7


// +------------------------------------+
// |                OT 13               |
// |      Secondary PCM Mic Stream      |
// |     Streaming Output Terminal      |
// +------------------------------------+
Name(E015, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x03},
        Package(2) { "mipi-sdca-entity-label", "OT 13"},
        Package(2) { "mipi-sdca-terminal-type", 0x185}, // Buffered PCM Mic Source Data Port
        Package(2) { "mipi-sdca-input-pin-list", 0x2},  // Pin 1 has a connection
        Package(2) { "mipi-sdca-control-list", CTL_OT_DATAPORT_SELECTOR | CTL_OT_LATENCY | CTL_OT_USAGE},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-input-pin-1", "E013"}, // SMPU 13
        USAGE_OT_13,
        Package(2) { "mipi-sdca-control-0x8-subproperties", "CD08"},    // Latency
        Package(2) { "mipi-sdca-control-0x11-subproperties", "CD11"},   // DataPortSelector
        Package(2) { "mipi-sdca-terminal-clock-connection", "E014" }, // CS 13
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-terminal-dp-numbers", "BUF8"},
    }
}) // End E015

Name(BUF8, Buffer() {
    0x1, COHEN_SECONDARY_STREAM_DATA_PORT, // {DP_Index, DP_Num}
}) // End BUF8

Name(CD08, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Latency = 0xA (dummy value)
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 0xA},
    }
}) // End CD08

Name(CD11, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // DataPort Selector *index* = 1 (see {DP_Index, DP_Num} map), Class, DC
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 1},
    }
}) // End CD11


// +------------------------------------+
// |               CRU 14               |
// +------------------------------------+
Name(E01A, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x20},
        Package(2) { "mipi-sdca-entity-label", "CRU 14"},
        Package(2) { "mipi-sdca-control-list", CTL_CRU_CLUSTERINDEX},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        CLUSTER_CRU_14,
        Package(2) { "mipi-sdca-input-pin-1", "E001"}, // Input Pin 1 connected to XU 12
    }
}) // End E01A


// +------------------------------------+
// |                FU 14               |
// +------------------------------------+
Name(E016, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x7},
        Package(2) { "mipi-sdca-entity-label", "FU 14"},
        Package(2) { "mipi-sdca-control-list", CTL_FU_VOLUME | CTL_FU_MUTE},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "CF01"},    // Mute
        Package(2) { "mipi-sdca-control-0x2-subproperties", "CF02"},    // Channel Volume
        Package(2) { "mipi-sdca-input-pin-1", "E01A"}, // Input Pin 1 connected to CRU 14
    }
}) // End E016

Name(CF01, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // User, Dual, Control Numbers = {1,2}
        Package(2) { "mipi-sdca-control-access-layer", 1}, // User
        Package(2) { "mipi-sdca-control-access-mode", 1},  // Dual R/W
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-fixed-value", 0 }, // Unmuted
        Package(2) { "mipi-sdca-control-cn-list", 0x6},    // Control Numbers = {1,2}
    }
}) // End CF01

Name(CF02, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // User, Dual, Control Numbers = {1,2}
        Package(2) { "mipi-sdca-control-access-layer", 4}, // Class
        Package(2) { "mipi-sdca-control-access-mode", 1},  // Dual R/W
        Package(2) {"mipi-sdca-control-default-value", CS42L43_FU_14_VOL_DEFAULT},
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-cn-list", 0x6}, // Control Numbers = {1,2}
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "BUFC"},
    }
}) // End CF02

Name(BUFC, Buffer()
{
    0x03, 0x00,  // Range type 0x0003 (Triples)
    0x01, 0x00,  // Count of ranges = 0x1
    CS42L43_FU_14_VOL_MIN,
    CS42L43_FU_14_VOL_MAX,
    CS42L43_FU_14_VOL_STEP,
}) // End BUFC


// +------------------------------------+
// |             SMPU 14                |
// +------------------------------------+
Name(E017, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x23},
        Package(2) { "mipi-sdca-entity-label", "SMPU 14"},
        Package(2) { "mipi-sdca-control-list", (CTL_SMPU_TRIGGER_READY | CTL_SMPU_HIST_BUFFER_MODE |
                                                CTL_SMPU_TRIGGER_STATUS | CTL_SMPU_TRIGGER_ENABLE)},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x10-subproperties", "CG10"}, // Trigger_Enable
        Package(2) { "mipi-sdca-control-0x11-subproperties", "CG11"}, // Trigger_Status
        Package(2) { "mipi-sdca-control-0x12-subproperties", "CG12"}, // Hist_Buffer_Mode
        Package(2) { "mipi-sdca-control-0x16-subproperties", "CG16"}, // Trigger_Ready
        Package(2) { "mipi-sdca-input-pin-1", "E016"}, // Input Pin 1 connected to FU 14
    }
}) // End E017

Name(CG10, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, RW, Trigger_Enable
        Package(2) { "mipi-sdca-control-access-layer", 4}, // Class
        Package(2) { "mipi-sdca-control-access-mode", 0},  // RW
        Package(2) { "mipi-sdca-control-deferrable", 1},
    }
}) // End CG10

Name(CG11, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, RW1C, Trigger_Status
        Package(2) { "mipi-sdca-control-access-layer", 4}, // Class
        Package(2) { "mipi-sdca-control-access-mode", 0},  // RW
        Package(2) { "mipi-sdca-control-deferrable", 1},
    }
}) // End CG11

Name(CG12, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Hist_Buffer_Mode = SHB
        Package(2) { "mipi-sdca-control-access-layer", 4}, // Class
        Package(2) { "mipi-sdca-control-access-mode", 5},  // DC
        Package(2) { "mipi-sdca-control-dc-value", 0x0 },  // Simple History Buffer (SHB)
    }
}) // End CG12

Name(CG16, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Trigger_Ready = 0
        Package(2) { "mipi-sdca-control-access-layer", 4}, // Class
        Package(2) { "mipi-sdca-control-access-mode", 5},  // DC
        Package(2) { "mipi-sdca-control-dc-value", 0 },
    }
}) // End CG16


// +------------------------------------+
// |              MFPU 14               |
// +------------------------------------+
Name(E01B, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x22},
        Package(2) { "mipi-sdca-entity-label", "MFPU 14"},
        Package(2) { "mipi-sdca-control-list", CTL_MFPU_CLUSTER_INDEX | CTL_MFPU_BYPASS},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "CH01"}, // Bypass
        CLUSTER_MFPU_14,
        Package(2) { "mipi-sdca-input-pin-1", "E017"}, // Input Pin 1 connected to SMPU 14
    }
}) // End E01B

Name(CH01, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Bypass, Class, DC. Defining as 'Null' MFPU (DC:1)
        Package(2) { "mipi-sdca-control-access-layer", 4}, // Class
        Package(2) { "mipi-sdca-control-access-mode", 5},  // DC
        Package(2) { "mipi-sdca-control-dc-value", 0x1},
    }
}) // End CH01


// +------------------------------------+
// |                CS 14               |
// +------------------------------------+
Name(E018, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0xB},
        Package(2) { "mipi-sdca-entity-label", "CS 14"},
        Package(2) { "mipi-sdca-cs-type", 0},
        Package(2) { "mipi-sdca-control-list", CTL_CS_CLOCK_VALID | CTL_CS_SAMPLERATEINDEX},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x2-subproperties", "CI02"},
        Package(2) { "mipi-sdca-control-0x10-subproperties", "CI10"},
    }
}) // End E014

Name(CI02, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Platform, DC, Clock_Valid
        Package(2) { "mipi-sdca-control-access-layer", 8}, // Platform
        Package(2) { "mipi-sdca-control-access-mode", 5},  // DC
        Package(2) { "mipi-sdca-control-dc-value", 0x1},
    }
}) // End C202

Name(CI10, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, SampleRateIndex
        Package(2) { "mipi-sdca-control-access-layer", 4}, // Class
        Package(2) { "mipi-sdca-control-access-mode", 0},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "BUFD"},
    }
}) // End CI10

// {SampleRateIndex, SampleRate} mapping.
Name(BUFD, Buffer()
{
    0x02, 0x00,  // Range type 0x0002 (Pairs)
    0x01, 0x00,  // Count of ranges = 0x1;

#ifdef COHEN_US_CAPTURE_96KHZ
    //0x00, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00, 0x00, // SampleRateIndex = 0x00000000, SampleRate = 48000 (0x0000BB80)
    //0x01, 0x00, 0x00, 0x00, 0x44, 0xAC, 0x00, 0x00, // SampleRateIndex = 0x00000001, SampleRate = 44100 (0x0000AC44)
    0x02, 0x00, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, // SampleRateIndex = 0x00000002, SampleRate = 96000 (0x00017700)
    //0x03, 0x00, 0x00, 0x00, 0x80, 0x3e, 0x00, 0x00, // SampleRateIndex = 0x00000003, SampleRate = 16000 (0x00003e80)
#else // Default = 48kHz
    0x00, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00, 0x00, // SampleRateIndex = 0x00000000, SampleRate = 48000 (0x0000BB80)
    //0x01, 0x00, 0x00, 0x00, 0x44, 0xAC, 0x00, 0x00, // SampleRateIndex = 0x00000001, SampleRate = 44100 (0x0000AC44)
    //0x02, 0x00, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, // SampleRateIndex = 0x00000002, SampleRate = 96000 (0x00017700)
    //0x03, 0x00, 0x00, 0x00, 0x80, 0x3e, 0x00, 0x00, // SampleRateIndex = 0x00000003, SampleRate = 16000 (0x00003e80)
#endif //COHEN_US_CAPTURE_96KHZ
}) // End BUFD


// +------------------------------------+
// |                OT 14               |
// |         UltraSound Capture         |
// |     Streaming Output Terminal      |
// +------------------------------------+
Name(E019, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x03},
        Package(2) { "mipi-sdca-entity-label", "OT 14"},
        Package(2) { "mipi-sdca-terminal-type", 0x180}, // Ultrasound Source/Sink Data Port
        Package(2) { "mipi-sdca-input-pin-list", 0x2},  // Pin 1 has a connection
        Package(2) { "mipi-sdca-control-list", CTL_OT_DATAPORT_SELECTOR | CTL_OT_LATENCY | CTL_OT_USAGE},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-input-pin-1", "E01B"}, // MFPU 14
        USAGE_OT_14,
        Package(2) { "mipi-sdca-control-0x8-subproperties", "CJ08"},   // Latency
        Package(2) { "mipi-sdca-control-0x11-subproperties", "CJ11"},   // DataPortSelector
        Package(2) { "mipi-sdca-terminal-clock-connection", "E018" },   // CS 14
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-terminal-dp-numbers", "BUFE"},
    }
}) // End E019

Name(BUFE, Buffer() {
    0x1, COHEN_ULS_CAPTURE_DATA_PORT, // {DP_Index, DP_Num}
}) // End BUFE

Name(CJ08, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Latency = 0xA (dummy value)
        Package(2) { "mipi-sdca-control-access-layer", 4}, // Class
        Package(2) { "mipi-sdca-control-access-mode", 5},  // DC
        Package(2) { "mipi-sdca-control-dc-value", 0xA},
    }
}) // End CJ08

Name(CJ11, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // DataPort Selector *index* = 1 (see {DP_Index, DP_Num} map), Class, DC
        Package(2) { "mipi-sdca-control-access-layer", 4}, // Class
        Package(2) { "mipi-sdca-control-access-mode", 5},  // DC
        Package(2) { "mipi-sdca-control-dc-value", 1},
    }
}) // End CJ11

//
// EOF
// Leave blank new line at the end of the file or iasl.exe will hang.
//
