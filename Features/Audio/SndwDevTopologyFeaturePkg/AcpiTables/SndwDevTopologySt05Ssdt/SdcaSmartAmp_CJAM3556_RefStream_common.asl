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

#ifdef EXCLUDE_FU_21_VOLUME_CONTROL
# define FEATURE_CS35L56_AMP_NO_VOL_MUTE_R_COND  0x00000000
#else
// Class driver creates AMP mute and volume elements.
// XU opts out from creating AMP mute and volume elements.
# define FEATURE_CS35L56_AMP_NO_VOL_MUTE_R_COND  0x00000020
#endif

#ifndef CS35L56_FU_21_VOL_MIN
// Min = 0x00009A00 (-102.0 dB) in Q7.8 format
# define CS35L56_FU_21_VOL_MIN 0x00, 0x9A, 0x00, 0x00
#endif

#ifndef CS35L56_FU_21_VOL_MAX
// Max = 0x00000C00 (+12.0 dB) in Q7.8 format
# define CS35L56_FU_21_VOL_MAX 0x00, 0x0C, 0x00, 0x00
#endif

#ifndef CS35L56_FU_21_VOL_FIXED_Q7_8
// Fixed = 0x00000C00 (+12 dB) in Q7.8 format
# define CS35L56_FU_21_VOL_FIXED_Q7_8 0x00000C00
#endif

#ifndef CS35L56_FU_21_VOL_STEP
// Step = 0x00000020 (0.125 dB per step) in Q7.8 format
# define CS35L56_FU_21_VOL_STEP 0x20, 0x00, 0x00, 0x00
#endif

#ifndef CS35L56_FU_21_VOL_DEFAULT
// Default = 0x0 (0 dB) in Q7.8 format
# define CS35L56_FU_21_VOL_DEFAULT 0x0
#endif

#ifndef CS35L56_FU_23_VOL_MIN
// Min = 0x00009A00 (-102.0 dB) in Q7.8 format
# define CS35L56_FU_23_VOL_MIN 0x00, 0x9A, 0x00, 0x00
#endif

#ifndef CS35L56_FU_23_VOL_MAX
// Max = 0x00000C00 (+12.0 dB) in Q7.8 format
# define CS35L56_FU_23_VOL_MAX 0x00, 0x0C, 0x00, 0x00
#endif

#ifndef CS35L56_FU_23_VOL_STEP
// Step = 0x00000020 (0.125 dB per step) in Q7.8 format
# define CS35L56_FU_23_VOL_STEP 0x20, 0x00, 0x00, 0x00
#endif

#ifndef CS35L56_FU_23_VOL_DEFAULT
// Default = 0x0 (0 dB) in Q7.8 format
# define CS35L56_FU_23_VOL_DEFAULT 0x0
#endif

#include <SmartAmp-Clusters.asl>
#include <SmartAmp-Usage.asl>


#ifdef JAMERSON_DISABLE_REF_STREAM
# define _SMART_AMP_REFERENCE_STREAM_FEATURE_BITS   0
#else
# define _SMART_AMP_REFERENCE_STREAM_FEATURE_BITS   (1 << 5) | (1 << 6)
#endif

#ifdef JAMERSON_DISABLE_US_STREAM
# define _SMART_AMP_US_STREAM_FEATURE_BITS          0
#else
# define _SMART_AMP_US_STREAM_FEATURE_BITS          (1 << 2)
#endif

#define JAMERSON_RENDER_STREAM_ENTITY_ID_LIST         0x3, 0x2, 0x4, 0x1A, 0x8, 0x9, 0xA, 0xB, 0xE, 0xD, 0x6, 0xC,
#define JAMERSON_VOLUME_STREAM_ENTITY_ID_LIST_VOLUME  0x05,
#define JAMERSON_REF_STREAM_ENTITY_ID_LIST            0xF, 0x10,
#define JAMERSON_US_STREAM_ENTITY_ID_LIST             0x16, 0x15, 0x17, 0x18, 0x19,
#define JAMERSON_SPEAKER_SENSE_ENTITY_ID_LIST         0x13, 0x14, 0x11


Name(_DSD, Package()
{ // Function Descriptor
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) {"mipi-sdw-sw-interface-revision", 0x00020001},  // Revision 2.1
        Package(2) {"mipi-sdca-function-topology-features",
            _SMART_AMP_REFERENCE_STREAM_FEATURE_BITS |
            _SMART_AMP_US_STREAM_FEATURE_BITS
        },
        Package(2) {"mipi-sdca-control-list",  (CTL_E0_FUNCTION_VERSION | CTL_E0_FUNCTION_ID | CTL_E0_FUNCTION_MANUFACTURER_ID |
                                                CTL_E0_FUNCTION_TYPE | CTL_E0_FUNCTION_SDCA_VERSION | CTL_E0_FUNCTION_STATUS_COND |
                                                CTL_E0_DEVICE_MANUFACTURER_ID | CTL_E0_DEVICE_PART_ID | CTL_E0_DEVICE_VERSION | CTL_E0_DEVICE_SDCA_VERSION
                                                )},  // Function level controls
        Package(2) {"mipi-sdca-entity-id-list", Package() {
            JAMERSON_RENDER_STREAM_ENTITY_ID_LIST
#ifndef EXCLUDE_FU_21_VOLUME_CONTROL
            JAMERSON_VOLUME_STREAM_ENTITY_ID_LIST_VOLUME
#endif
#ifndef JAMERSON_DISABLE_REF_STREAM
            JAMERSON_REF_STREAM_ENTITY_ID_LIST
#endif
#ifndef JAMERSON_DISABLE_US_STREAM
            JAMERSON_US_STREAM_ENTITY_ID_LIST
#endif
            // JAMERSON_SPEAKER_SENSE_ENTITY_ID_LIST
            },
        },
        CLUSTER_ID_LIST_JAMERSON,
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) {"mipi-sdca-control-0x4-subproperties", "C004"},
        Package(2) {"mipi-sdca-control-0x5-subproperties", "C005"},
        Package(2) {"mipi-sdca-control-0x6-subproperties", "C006"},
        Package(2) {"mipi-sdca-control-0x7-subproperties", "C007"},
        Package(2) {"mipi-sdca-control-0x8-subproperties", "C008"},
#ifndef EXCLUDE_FUN_STS
        Package(2) { "mipi-sdca-control-0x10-subproperties", "C010"},
#endif
        Package(2) { "mipi-sdca-control-0x2C-subproperties", "C02C"},
        Package(2) { "mipi-sdca-control-0x2D-subproperties", "C02D"},
        Package(2) { "mipi-sdca-control-0x2E-subproperties", "C02E"},
        Package(2) { "mipi-sdca-control-0x2F-subproperties", "C02F"},

        Package(2) {"mipi-sdca-entity-id-0x3-subproperties", "E003"},       // IT 21
        Package(2) {"mipi-sdca-entity-id-0x2-subproperties", "E002"},       // CS 21
        Package(2) {"mipi-sdca-entity-id-0x4-subproperties", "E004"},       // PPU 21
#ifndef EXCLUDE_FU_21_VOLUME_CONTROL
        Package(2) {"mipi-sdca-entity-id-0x5-subproperties", "E005"},       // FU 21
#endif
        Package(2) {"mipi-sdca-entity-id-0x1A-subproperties", "E01A"},      // MFPU 21
        Package(2) {"mipi-sdca-entity-id-0x8-subproperties", "E008"},       // XU 22
        Package(2) {"mipi-sdca-entity-id-0x9-subproperties", "E009"},       // SAPU 29
        Package(2) {"mipi-sdca-entity-id-0x16-subproperties", "E016"},      // IT 26
        Package(2) {"mipi-sdca-entity-id-0x15-subproperties", "E015"},      // CS 26
        Package(2) {"mipi-sdca-entity-id-0x17-subproperties", "E017"},      // PPU 26
        Package(2) {"mipi-sdca-entity-id-0x18-subproperties", "E018"},      // MFPU 26
        Package(2) {"mipi-sdca-entity-id-0xA-subproperties", "E00A"},       // MU 26
        Package(2) {"mipi-sdca-entity-id-0xB-subproperties", "E00B"},       // UDMPU 23
        Package(2) {"mipi-sdca-entity-id-0xE-subproperties", "E00E"},       // OT 23
        Package(2) {"mipi-sdca-entity-id-0xD-subproperties", "E00D"},       // PDE 23
        Package(2) {"mipi-sdca-entity-id-0xF-subproperties", "E00F"},       // UDMPU 25
        Package(2) {"mipi-sdca-entity-id-0x10-subproperties", "E010"},      // OT 25
        Package(2) {"mipi-sdca-entity-id-0x6-subproperties", "E006"},       // IT 29
        Package(2) {"mipi-sdca-entity-id-0x13-subproperties", "E013"},      // XU 24
        Package(2) {"mipi-sdca-entity-id-0x14-subproperties", "E014"},      // OT 24
        Package(2) {"mipi-sdca-entity-id-0x11-subproperties", "E011"},      // CS 24
        Package(2) {"mipi-sdca-entity-id-0xC-subproperties", "E00C"},       // FU 23
        Package(2) {"mipi-sdca-entity-id-0x19-subproperties", "E019"},      // FU 26
        // Vendor-specific
        Package(2) {"mipi-sdca-function-expansion-subproperties", "EXT0"},
        // Clusters
        CLUSTER_INIT_JAMERSON,
    },
}) //End _DSD

#undef _SMART_AMP_REFERENCE_STREAM_FEATURE_BITS
#undef _SMART_AMP_US_STREAM_FEATURE_BITS

Name(EXT0, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-function-expansion-subsystem-id", 0 },  // MIPI required, but not used by MSFT
        Package(2) { "01fa-chip-id", 0x3556 },
        Package(2) { "01fa-ssid-ex", 0x7 },
        Package(2) { "01fa-xu-features", (FEATURE_ENABLE_HWKWS | FEATURE_ENABLE_WT | FEATURE_ENABLE_KNCK | FEATURE_NO_FUN_STS |
                                          FEATURE_CS35L56_AMP_NO_VOL_MUTE_R_COND)},
    }
}) //End EXT0

// TODO: Add channel definition for ultrasound, which is purpose 6 and potentially echo reference.


// +====================================+
// |    Function level controls         |
// +====================================+

// Function-level control definitions
Name(C004, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Class, DC, Function_SDCA_Version = 1.0
        Package(2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package(2) {"mipi-sdca-control-dc-value", CTL_E0_FUNCTION_SDCA_VERSION_VAL},
    }
}) //End C004

Name(C005, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Class, DC, Function_Type = 0x1 (Smart-Amp)
        Package(2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package(2) {"mipi-sdca-control-dc-value", 0x1},
    }
}) //End C005

Name(C006, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Function_Manufacturer_Id = 0x01FA
        Package(2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package(2) {"mipi-sdca-control-dc-value", 0x01FA},
    }
}) //End C006

Name(C007, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Function_Id = 0x3556
        Package(2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package(2) {"mipi-sdca-control-dc-value", 0x3556},
    }
}) //End C007

Name(C008, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Class, DC, Function_Version = 0x1
        Package(2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package(2) {"mipi-sdca-control-dc-value", 0x1},
    }
}) //End C008

#ifndef EXCLUDE_FUN_STS
Name(C010, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Class, RW1C, Function_Status
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_RW1C},
        Package(2) { "mipi-sdca-control-interrupt-position", 1},    // CS35L56 rev B0.
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
        // Device_Part_ID: Class, DC = 0x3556
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x3556},
    }
}) // End C02D


Name(C02E, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Device_Version: Class, DC = 0xB0
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0xB0},
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


// +====================================+
// |        Entity Definitions          |
// +====================================+

// +------------------------------------+
// |                IT_21               |
// +------------------------------------+
Name(E003, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) {"mipi-sdca-entity-type", 0x2},
        Package(2) {"mipi-sdca-entity-label", "IT 21"},
        Package(2) {"mipi-sdca-terminal-type", 0x0101}, // Generic Sink DataPort
        Package(2) {"mipi-sdca-control-list", CTL_IT_DATAPORT_SELECTOR | CTL_IT_CLUSTERINDEX | CTL_IT_USAGE},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        USAGE_IT_21,
        CLUSTER_IT_21,
        Package(2) {"mipi-sdca-control-0x11-subproperties", "C021"},
        Package(2) {"mipi-sdca-terminal-clock-connection", "E002"},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-terminal-dp-numbers", "BUF3"},
    }
}) //End E003

Name(BUF3, Buffer()
{
    0x1, JAMERSON_SPEAKER_RENDER_DATA_PORT, // DP_Index: 0x1 DP_Num: 0x1
}) //End BUF3

Name(C021, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    { // DataPortIndex, Class, DC
        Package(2) {"mipi-sdca-control-access-layer", 4},
        Package(2) {"mipi-sdca-control-access-mode", 5},
        Package(2) {"mipi-sdca-control-dc-value", 1},
    }
}) //End C021


// +------------------------------------+
// |                CS_21               |
// +------------------------------------+
Name(E002, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) {"mipi-sdca-entity-type", 0x0b},
        Package(2) {"mipi-sdca-entity-label", "CS 21"},
        Package(2) {"mipi-sdca-cs-type", 0},
        Package(2) {"mipi-sdca-control-list", CTL_CS_SAMPLERATEINDEX},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) {"mipi-sdca-control-0x10-subproperties", "C023"},
    }
}) //End E002

Name(C022, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    { // Clock_Valid, Class, RO
        Package(2) {"mipi-sdca-control-access-layer", 4},
        Package(2) {"mipi-sdca-control-access-mode", 3},
    }
}) //End C022

Name(C023, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    { // Sample_Freq_Index, Class, R/W
        Package(2) {"mipi-sdca-control-access-layer", 0x4},
        Package(2) {"mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package(2) {"mipi-sdca-control-default-value", 0x3},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "BUF5"},
    }
}) //End C023

// Indexes must match the values listed in the data sheet for GLOBAL_FS.
Name(BUF5, Buffer()
{
    0x02, 0x00,  // Range type 0x0002 (Pairs)
#ifndef JAMERSON_96K
    0x01, 0x00,  // Count of ranges = 0x1
#else
    0x02, 0x00,  // Count of ranges = 0x2
#endif
    0x03, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00, 0x00, // SampleRateIndex = 0x00000003, SampleRate = 48000 (0x0000BB80)
#ifdef JAMERSON_96K
    0x04, 0x00, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, // SampleRateIndex = 0x00000004, SampleRate = 96000 (0x00017700)
#endif
    //0x0B, 0x00, 0x00, 0x00, 0x44, 0xAC, 0x00, 0x00, // SampleRateIndex = 0x0000000B, SampleRate = 44100 (0x0000AC44)
    //0x0C, 0x00, 0x00, 0x00, 0x88, 0x58, 0x01, 0x00, // SampleRateIndex = 0x0000000B, SampleRate = 88200 (0x00015888)
    //0x11, 0x00, 0x00, 0x00, 0x40, 0x1F, 0x00, 0x00, // SampleRateIndex = 0x0000000B, SampleRate = 8000  (0x00001F40)
    //0x12, 0x00, 0x00, 0x00, 0x80, 0x3E, 0x00, 0x00, // SampleRateIndex = 0x0000000B, SampleRate = 16000 (0x00003E80)
}) //End BUF5


// +------------------------------------+
// |               PPU_21               |
// +------------------------------------+
Name(E004, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x25},
        Package (2) {"mipi-sdca-entity-label", "PPU 21"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", CTL_PPU_POSTURENUMBER},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-control-0x10-subproperties", "C024"},
        Package (2) {"mipi-sdca-input-pin-1", "E003"},
    }
}) //End E004

Name(C024, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 1},
        Package(2) {"mipi-sdca-control-dc-value", 1},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "PMAP"},
    }
}) //End C024


#ifndef EXCLUDE_FU_21_VOLUME_CONTROL
// +------------------------------------+
// |                FU_21               |
// +------------------------------------+
Name(E005, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) {"mipi-sdca-entity-type", 0x07},
        Package(2) {"mipi-sdca-entity-label", "FU 21"},
        Package(2) {"mipi-sdca-control-list", CTL_FU_VOLUME | CTL_FU_MUTE},
        Package(2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) {"mipi-sdca-input-pin-1", "E004"},
        Package(2) {"mipi-sdca-control-0x1-subproperties", "C025"},
        Package(2) {"mipi-sdca-control-0x2-subproperties", "C026"},
    }
}) //End E005

Name(C025, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {  // Mute, User, RW/Dual
        Package(2) {"mipi-sdca-control-access-layer", 1},
        Package(2) {"mipi-sdca-control-access-mode", 1},
        Package(2) {"mipi-sdca-control-cn-list", 0x2},  // Control Numbers = {1}
#ifdef FU_21_FIXED_RENDER_MUTE
        Package(2) {"mipi-sdca-control-fixed-value", 0},   // unmuted
#endif
    }
}) //End C025

Name(C026, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {  // Volume, User, RW/Dual
        Package(2) {"mipi-sdca-control-access-layer", 1},
        Package(2) {"mipi-sdca-control-access-mode", 1},
        Package(2) {"mipi-sdca-control-cn-list", 0x2},  // Control Numbers = {1}
#ifdef FU_21_FIXED_RENDER_VOLUME
        Package(2) {"mipi-sdca-control-fixed-value", CS35L56_FU_21_VOL_FIXED_Q7_8},
#else
        Package(2) {"mipi-sdca-control-default-value", CS35L56_FU_21_VOL_DEFAULT},
#endif
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "BUF4"},
    }
}) //End C026

Name(BUF4, Buffer()
{
    0x03, 0x00,  // Range type 0x0003 (Triples)
    0x01, 0x00,  // Count of ranges = 0x1
    CS35L56_FU_21_VOL_MIN,
    CS35L56_FU_21_VOL_MAX,
    CS35L56_FU_21_VOL_STEP,
}) //End BUF4
#endif  // EXCLUDE_FU_21_VOLUME_CONTROL


// +------------------------------------+
// |                MFPU_21             |
// +------------------------------------+
Name(E01A, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
        Package (2) {"mipi-sdca-entity-type", 0x22},
        Package (2) {"mipi-sdca-entity-label", "MFPU 21"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", CTL_MFPU_BYPASS},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package ()
    {
#ifdef EXCLUDE_FU_21_VOLUME_CONTROL
        Package (2) {"mipi-sdca-input-pin-1", "E004"}, // Connect to PPU_21 (skip FU_21)
#else
        Package (2) {"mipi-sdca-input-pin-1", "E005"},
#endif
        Package (2) {"mipi-sdca-control-0x1-subproperties", "C027"},
    }
}) //End E01A

Name(C027, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 1},
    }
}) //End C027


// +------------------------------------+
// |                XU_22               |
// +------------------------------------+
Name(E008, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0xa},
        Package (2) {"mipi-sdca-entity-label", "XU 22"},
        Package (2) {"mipi-sdca-input-pin-list", 0x6},
        //Package (2) {"mipi-sdca-control-list", 0x7D0182},
        Package (2) {"mipi-sdca-control-list", CTL_XU_VERSION | CTL_XU_ID | CTL_XU_BYPASS},
        Package (2) {"mipi-sdca-firmware-dl-mode", 0},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-2", "E006"},
        Package (2) {"mipi-sdca-input-pin-1", "E01A"},
        Package (2) {"mipi-sdca-control-0x1-subproperties", "C028"},
        Package (2) {"mipi-sdca-control-0x7-subproperties", "C029"},
        Package (2) {"mipi-sdca-control-0x8-subproperties", "C030"},
        Package (2) {"mipi-sdca-control-0x9-subproperties", "C031"},
        Package (2) {"mipi-sdca-control-0xA-subproperties", "C032"},
        Package (2) {"mipi-sdca-control-0x10-subproperties", "C033"},
        Package (2) {"mipi-sdca-control-0x12-subproperties", "C034"},
        Package (2) {"mipi-sdca-control-0x13-subproperties", "C035"},
        Package (2) {"mipi-sdca-control-0x14-subproperties", "C036"},
        Package (2) {"mipi-sdca-control-0x15-subproperties", "C037"},
        Package (2) {"mipi-sdca-control-0x16-subproperties", "C038"},
    }
}) //End E008

Name(C028, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 0},
        Package (2) {"mipi-sdca-control-default-value", 0},
    }
}) //End C028

Name(C029, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 2}, // XU ID
    }
}) //End C029

Name(C030, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 1}, // XU Version
    }
}) //End C030

Name(C031, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0},
    }
}) //End C031

Name(C032, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0},
    }
}) //End C032

Name(C033, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-interrupt-position", 1},
        Package (2) {"mipi-sdca-control-access-mode", 0},
        Package (2) {"mipi-sdca-control-default-value", 1},
    }
}) //End C033

Name(C034, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 0},
        Package (2) {"mipi-sdca-control-default-value", 0},
    }
}) //End C034

Name(C035, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 0},
        Package (2) {"mipi-sdca-control-default-value", 0},
    }
}) //End C035

Name(C036, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 0},
        Package (2) {"mipi-sdca-control-default-value", 0},
    }
}) //End C036

Name(C037, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 3},
    }
}) //End C037

Name(C038, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 0},
        Package (2) {"mipi-sdca-control-default-value", 0},
    }
}) //End C038

Name(E009, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x24},
        Package (2) {"mipi-sdca-entity-label", "SAPU 29"},
        Package (2) {"mipi-sdca-input-pin-list", 0x6},
        Package (2) {"mipi-sdca-control-list", CTL_SAPU_PROTECTION_STATUS | CTL_SAPU_PROTECTION_MODE},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-2", "E006"},
        Package (2) {"mipi-sdca-input-pin-1", "E008"},
        Package (2) {"mipi-sdca-control-0x10-subproperties", "C039"},
        Package (2) {"mipi-sdca-control-0x11-subproperties", "C040"},
    }
}) //End E009

Name(C039, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 3},
        Package (2) {"mipi-sdca-control-interrupt-position", 10}, //TODO - Check with Jon
    }
}) //End C039

Name(C040, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 3},
    }
}) //End C040


// +------------------------------------+
// |                IT_26               |
// +------------------------------------+
Name(E016, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x2},
        Package (2) {"mipi-sdca-entity-label", "IT 26"},
        Package (2) {"mipi-sdca-control-list", CTL_IT_DATAPORT_SELECTOR | CTL_IT_CLUSTERINDEX | CTL_IT_USAGE},
        Package (2) {"mipi-sdca-terminal-type", 0x180},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        USAGE_IT_26,
        CLUSTER_IT_26,
        Package (2) {"mipi-sdca-control-0x11-subproperties", "C042"},
        Package (2) {"mipi-sdca-terminal-clock-connection", "E015"},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package() {
        Package (2) {"mipi-sdca-terminal-dp-numbers", "BUF7"},
    }
}) //End E016

Name(BUF7, Buffer() {
    0x1, JAMERSON_US_RENDER_DATA_PORT, // DP_Index, DP_Num
}) //End BUF7

Name(C042, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 2},
    }
}) //End C042


// +------------------------------------+
// |                CS_26               |
// +------------------------------------+
Name(E015, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0xb},
        Package (2) {"mipi-sdca-entity-label", "CS 26"},
        Package (2) {"mipi-sdca-cs-type", 0},
        Package (2) {"mipi-sdca-control-list", CTL_CS_SAMPLERATEINDEX},
        },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-control-0x10-subproperties", "C043"},
    }
}) //End E015

Name(C043, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 0x4},
        Package(2) {"mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package (2) {"mipi-sdca-control-default-value", 0x3},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "BUF9"},
    }
}) //End C043

Name(BUF9, Buffer()
{
    0x02, 0x00,  // Range type 0x0002 (Pairs)
    0x01, 0x00,  // Count of ranges = 0x1
    0x03, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00, 0x00, // 0x00000003, 0x0000BB80 (48000)
}) //End BUF9


// +------------------------------------+
// |               PPU_26               |
// +------------------------------------+
Name(E017, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x25},
        Package (2) {"mipi-sdca-entity-label", "PPU 26"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", CTL_PPU_POSTURENUMBER},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-1", "E016"},
        Package (2) {"mipi-sdca-control-0x10-subproperties", "C044"},
    }
}) //End E017

Name(C044, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 1},
        Package (2) {"mipi-sdca-control-default-value", 0},
    }
}) //End C044


// +------------------------------------+
// |               MFPU_26              |
// +------------------------------------+
Name(E018, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x22},
        Package (2) {"mipi-sdca-entity-label", "MFPU 26"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", (CTL_MFPU_CLUSTER_INDEX | CTL_MFPU_ALGORITHM_ENABLE |
                                                CTL_MFPU_ALGORITHM_READY | CTL_MFPU_BYPASS)},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-1", "E017"},
        Package (2) {"mipi-sdca-control-0x1-subproperties", "C045"},
        Package (2) {"mipi-sdca-control-0x4-subproperties", "C046"},
        Package (2) {"mipi-sdca-control-0x5-subproperties", "C047"},
        CLUSTER_MFPU_26,
    }
}) //End E018

Name(C045, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0},
    }
}) //End C045

Name(C046, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0x283C},
    }
}) //End C046

Name(C047, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0x283C},
    }
}) //End C047


// +------------------------------------+
// |                MU_26               |
// +------------------------------------+
Name(E00A, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x5},
        Package (2) {"mipi-sdca-entity-label", "MU 26"},
#ifndef JAMERSON_DISABLE_US_STREAM
        Package (2) {"mipi-sdca-input-pin-list", 0x6},
#else
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
#endif
        Package (2) {"mipi-sdca-control-list", CTL_MU_MIXER},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
#ifndef JAMERSON_DISABLE_US_STREAM
        Package (2) {"mipi-sdca-input-pin-2", "E019"},
#endif
        Package (2) {"mipi-sdca-input-pin-1", "E009"},
        Package (2) {"mipi-sdca-control-0x1-subproperties", "C049"},
    }
}) //End E00A

Name(C049, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 0x8},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0x1},
    }
}) //End C049


// +------------------------------------+
// |               UDMPU_23             |
// +------------------------------------+
Name(E00B, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x21},
        Package (2) {"mipi-sdca-entity-label", "UDMPU 23"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", CTL_UDMPU_CLUSTERINDEX},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-1", "E00A"},
        CLUSTER_UDMPU_23,
    }
}) //End E00B


// +------------------------------------+
// |                OT_23               |
// +------------------------------------+
Name(E00E, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) {"mipi-sdca-entity-type", 0x03},
        Package(2) {"mipi-sdca-entity-label", "OT 23"},
        Package(2) {"mipi-sdca-terminal-type", 0x0380},// Primary Full Speaker
        Package(2) {"mipi-sdca-input-pin-list", 0x2},
        Package(2) {"mipi-sdca-control-list", CTL_OT_USAGE},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) {"mipi-sdca-input-pin-1", "E00C"},
        Package(2) {"mipi-sdca-terminal-transducer-count", 1},
        USAGE_OT_23,
    }
}) //End E00E


// +------------------------------------+
// |                PDE_23              |
// +------------------------------------+
Name(E00D, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) {"mipi-sdca-entity-type", 0x11},
        Package(2) {"mipi-sdca-entity-label", "PDE 23"},
        Package(2)
        {
            "mipi-sdca-powerdomain-managed-list", Package() {0xE},
        },
        Package(2) {"mipi-sdca-control-list", CTL_PDE_ACTUAL_PS | CTL_PDE_REQUESTED_PS},
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
        Package(2) {"mipi-sdca-control-0x1-subproperties", "RPS0" },
        Package(2) {"mipi-sdca-control-0x10-subproperties", "APS0"},
    }
}) //End E00D

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
         Package(2) {"mipi-sdca-control-access-mode", 3},
    }
}) //End APS0


// +------------------------------------+
// |               UDMPU_25             |
// +------------------------------------+
Name(E00F, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x21},
        Package (2) {"mipi-sdca-entity-label", "UDMPU 25"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", (CTL_UDMPU_ULTRASOUND_LOOP_GAIN | CTL_UDMPU_ACOUSTIC_ENERGY_LEVEL_MONITOR |
                                                CTL_UDMPU_CLUSTERINDEX)},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-1", "E00C"},
        CLUSTER_UDMPU_25,
        Package (2) {"mipi-sdca-control-0x11-subproperties", "C053"},
        Package (2) {"mipi-sdca-control-0x12-subproperties", "C054"},
    }
}) //End E00F

Name(C053, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 8},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0},
    }
}) //End C053

Name(C054, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 8},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0},
    }
}) //End C054

// +------------------------------------+
// |                OT_25               |
// +------------------------------------+
Name(E010, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x3},
        Package (2) {"mipi-sdca-entity-label", "OT 25"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", CTL_OT_MATCHING_GUID | CTL_OT_DATAPORT_SELECTOR | CTL_OT_USAGE},
        Package (2) {"mipi-sdca-terminal-type", 0x188},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-1", "E00F"},
        Package (2) {"mipi-sdca-terminal-clock-connection", "E002"},
        USAGE_OT_25,
        Package (2) {"mipi-sdca-control-0x11-subproperties", "C056"},
        Package (2) {"mipi-sdca-control-0x12-subproperties", "C057"},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-terminal-dp-numbers", "BUF6"},
    }
}) //End E010

Name(BUF6, Buffer() {
    0x1, JAMERSON_REF_STREAM_DATA_PORT, // DP_Index, DP_Num
}) //End BUF6

Name(C056, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 0x4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0x4},
    }
}) //End C056

Name(C057, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 0x4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0x0},
    }
}) //End C057


// +------------------------------------+
// |                IT_29               |
// +------------------------------------+
Name(E006, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x2},
        Package (2) {"mipi-sdca-entity-label", "IT 29"},
        Package (2) {"mipi-sdca-terminal-type", 0x280},
        Package (2) {"mipi-sdca-control-list", CTL_IT_CLUSTERINDEX | CTL_IT_USAGE},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        USAGE_IT_29,
        CLUSTER_IT_29
    },
}) //End E006


// +------------------------------------+
// |                XU_24               |
// +------------------------------------+
Name(E013, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0xA},
        Package (2) {"mipi-sdca-entity-label", "XU 24"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", CTL_XU_BYPASS},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-1", "E006"},
        Package (2) {"mipi-sdca-control-0x1-subproperties", "C060"},
    }
}) //End E013

Name(C060, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 0},
        Package (2) {"mipi-sdca-control-default-value", 0},
    }
}) //End C060


// +------------------------------------+
// |                OT_24               |
// +------------------------------------+
Name(E014, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x3},
        Package (2) {"mipi-sdca-entity-label", "OT 24"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", CTL_OT_MATCHING_GUID | CTL_OT_DATAPORT_SELECTOR | CTL_OT_USAGE},
        Package (2) {"mipi-sdca-terminal-type", 0x189},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-1", "E013"},
        Package (2) {"mipi-sdca-terminal-clock-connection", "E011"},
        USAGE_OT_24,
        Package (2) {"mipi-sdca-control-0x11-subproperties", "C062"},
        Package (2) {"mipi-sdca-control-0x12-subproperties", "C063"},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-terminal-dp-numbers", "BUFA"},
    }
}) //End E014

Name(BUFA, Buffer() {
    0x1, 0x3, // DP_Index, DP_Num
}) //End BUFA

Name(C062, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 0x4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0x3},
    }
}) //End C062

Name(C063, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", 0x4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0x0},
    }
}) //End C063


// +------------------------------------+
// |                CS_24               |
// +------------------------------------+
Name(E011, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0xb},
        Package (2) {"mipi-sdca-entity-label", "CS 24"},
        Package (2) {"mipi-sdca-cs-type", 0},
        Package (2) {"mipi-sdca-control-list", CTL_CS_SAMPLERATEINDEX},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-control-0x10-subproperties", "C064"},
    }
}) //End E011

Name(C064, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    { // Sample_Freq_Index, Class, R/W
        Package (2) {"mipi-sdca-control-access-layer", 0x4},
        Package(2) {"mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package (2) {"mipi-sdca-control-default-value", 0x3},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "BUF5"},
    }
}) //End C064




// +------------------------------------+
// |                FU_23               |
// +------------------------------------+
Name(E00C, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) {"mipi-sdca-entity-type", 0x07},
        Package(2) {"mipi-sdca-entity-label", "FU 23"},
        Package(2) {"mipi-sdca-control-list", CTL_FU_VOLUME | CTL_FU_MUTE},
        Package(2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) {"mipi-sdca-input-pin-1", "E00B"},
        Package(2) {"mipi-sdca-control-0x1-subproperties", "C02A"},
        Package(2) {"mipi-sdca-control-0x2-subproperties", "C02B"},
    }
}) //End E00C

Name(C02A, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {  // Mute, User, RW/Dual
        Package(2) {"mipi-sdca-control-access-layer", 1},
        Package(2) {"mipi-sdca-control-access-mode", 1},
        Package(2) {"mipi-sdca-control-cn-list", 0x6},
        Package(2) {"mipi-sdca-control-fixed-value", 0}, // Unmuted
    }
}) //End C02A

Name(C02B, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {  // Volume, Platform, RW/Dual
        Package(2) {"mipi-sdca-control-access-layer", 0x8},
        Package(2) {"mipi-sdca-control-access-mode", 1},
        Package(2) {"mipi-sdca-control-default-value", CS35L56_FU_23_VOL_DEFAULT},
        Package(2) {"mipi-sdca-control-cn-list", 0x6},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "BUFB"},
    }
}) //End C02B

Name(BUFB, Buffer()
{
    0x03, 0x00,  // Range type 0x0003 (Triples)
    0x01, 0x00,  // Count of ranges = 0x1
    CS35L56_FU_23_VOL_MIN,
    CS35L56_FU_23_VOL_MAX,
    CS35L56_FU_23_VOL_STEP,
}) //End BUFB


// +------------------------------------+
// |                FU_26               |
// +------------------------------------+
Name(E019, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) {"mipi-sdca-entity-type", 0x07},
        Package(2) {"mipi-sdca-entity-label", "FU 26"},
        Package(2) {"mipi-sdca-control-list", CTL_FU_VOLUME | CTL_FU_MUTE},
        Package(2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) {"mipi-sdca-input-pin-1", "E018"},
        Package(2) {"mipi-sdca-control-0x1-subproperties", "C02A"},
        Package(2) {"mipi-sdca-control-0x2-subproperties", "C02B"},
    }
}) //End E019
