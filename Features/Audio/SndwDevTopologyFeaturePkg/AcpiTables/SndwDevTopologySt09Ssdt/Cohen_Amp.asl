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

#ifdef EXCLUDE_FU_21_VOLUME_CONTROL
# define FEATURE_CS42L43_AMP_NO_VOL_MUTE_R_COND 0x00000000
#else
// Class driver creates AMP mute and volume elements.
// XU opts out from creating AMP mute and volume elements.
# define FEATURE_CS42L43_AMP_NO_VOL_MUTE_R_COND 0x00000020
#endif

#ifndef CS42L43_FU_21_VOL_MIN
// Min = 0x0000C000 (-64.0 dB) in Q7.8 format
# define CS42L43_FU_21_VOL_MIN 0x00, 0xC0, 0x00, 0x00
#endif

#ifndef CS42L43_FU_21_VOL_MAX
// Max = 0x00000000 (0.0 dB) in Q7.8 format
# define CS42L43_FU_21_VOL_MAX     0x00, 0x00, 0x00, 0x00
#endif

#ifndef CS42L43_FU_21_VOL_FIXED_Q7_8
// Fixed = 0x00000000 (0.0 dB) in Q7.8 format
# define CS42L43_FU_21_VOL_FIXED_Q7_8 0x00000000
#endif

#ifndef CS42L43_FU_21_VOL_STEP
// Step = 0x00000008 (1/32 dB per step) in Q7.8 format
# define CS42L43_FU_21_VOL_STEP 0x08, 0x00, 0x00, 0x00
#endif

#ifndef CS42L43_FU_21_VOL_DEFAULT
// Default = 0x0 (0 dB) in Q7.8 format
# define CS42L43_FU_21_VOL_DEFAULT 0x0
#endif

#include <SmartAmp-Clusters.asl>
#include <SmartAmp-Usage.asl>


#ifdef COHEN_DISABLE_REF_STREAM
# define _SMART_AMP_REFERENCE_STREAM_FEATURE_BITS   0
#else
# define _SMART_AMP_REFERENCE_STREAM_FEATURE_BITS   (1 << 5) | (1 << 6)
#endif

#define COHEN_RENDER_STREAM_ENTITY_ID_LIST         0x8, 0x6, 0x4, 0x10, 0x1, 0x7, 0x9, 0xA, 0xB, 0x2, 0xC,
#define COHEN_VOLUME_STREAM_ENTITY_ID_LIST_VOLUME  0x05,
#define COHEN_REF_STREAM_ENTITY_ID_LIST            0x11, 0x12,

// COHEN_BRIDGE_SIDECAR_REGISTER_ACCESS_ADDRESS_SELECTOR
#define CS42L43_AMP_E0_CTL_SIDECAR_REG_ADDR (1 << 0x30)

// COHEN_BRIDGE_SIDECAR_REGISTER_ACCESS_VALUE_SELECTOR
#define CS42L43_AMP_E0_CTL_SIDECAR_REG_VAL  (1 << 0x31)


Name(_DSD, Package()
{
    // Device properties UUID
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package() {
        Package(2) { "mipi-sdw-sw-interface-revision", 0x00020001 },  // Revision 2.1
        Package(2) { "mipi-sdca-function-topology-features",
            _SMART_AMP_REFERENCE_STREAM_FEATURE_BITS
        },
        Package(2) { "mipi-sdca-control-list", (CTL_E0_FUNCTION_VERSION | CTL_E0_FUNCTION_ID | CTL_E0_FUNCTION_MANUFACTURER_ID |
                                                CTL_E0_FUNCTION_TYPE | CTL_E0_FUNCTION_SDCA_VERSION | CTL_E0_COMMIT_GROUP_MASK | CTL_E0_FUNCTION_STATUS_COND |
                                                CTL_E0_DEVICE_MANUFACTURER_ID | CTL_E0_DEVICE_PART_ID | CTL_E0_DEVICE_VERSION | CTL_E0_DEVICE_SDCA_VERSION |
                                                CS42L43_AMP_E0_CTL_SIDECAR_REG_ADDR | CS42L43_AMP_E0_CTL_SIDECAR_REG_VAL)},
        Package(2) { "mipi-sdca-entity-id-list", Package() {
            COHEN_RENDER_STREAM_ENTITY_ID_LIST
#ifndef EXCLUDE_FU_21_VOLUME_CONTROL
            COHEN_VOLUME_STREAM_ENTITY_ID_LIST_VOLUME
#endif
#ifndef COHEN_DISABLE_REF_STREAM
            COHEN_REF_STREAM_ENTITY_ID_LIST
#endif
            },
        },
        Package(2) { "mipi-sdca-function-busy-max-delay", 5000},
        // Clusters
        CLUSTER_ID_LIST_JAMERSON,
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        // Function-level
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C001"},
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
        Package(2) { "mipi-sdca-entity-id-0x1-subproperties", "E001"},      // XU 22
        Package(2) { "mipi-sdca-entity-id-0x2-subproperties", "E002"},      // PDE 22
        // TG_23 currently not used
        // Package(2) { "mipi-sdca-entity-id-0x3-subproperties", "E003"},   // TG 23
        Package(2) { "mipi-sdca-entity-id-0x4-subproperties", "E004"},      // PPU 21
#ifndef EXCLUDE_FU_21_VOLUME_CONTROL
        Package(2) { "mipi-sdca-entity-id-0x5-subproperties", "E005"},      // FU 21
#endif
        Package(2) { "mipi-sdca-entity-id-0x6-subproperties", "E006"},      // IT 21
        Package(2) { "mipi-sdca-entity-id-0x7-subproperties", "E007"},      // IT 29
        Package(2) { "mipi-sdca-entity-id-0x8-subproperties", "E008"},      // CS 21
        Package(2) { "mipi-sdca-entity-id-0x9-subproperties", "E009"},      // SAPU 29
        Package(2) { "mipi-sdca-entity-id-0xA-subproperties", "E00A"},      // UDMPU 23
        Package(2) { "mipi-sdca-entity-id-0xB-subproperties", "E00B"},      // FU 23
        Package(2) { "mipi-sdca-entity-id-0xC-subproperties", "E00C"},      // OT 23
        // IT_199 currently not used
        // Package(2) { "mipi-sdca-entity-id-0xD-subproperties", "E00D"},   // IT 199
        // SPE_199 currently not used
        // Package(2) { "mipi-sdca-entity-id-0xE-subproperties", "E00E"},   // SPE 199
        // OT_199 currently not used
        // Package(2) { "mipi-sdca-entity-id-0xF-subproperties", "E00F"},   // OT 199
        Package(2) { "mipi-sdca-entity-id-0x10-subproperties", "E010"},     // MFPU 21
        Package(2) { "mipi-sdca-entity-id-0x11-subproperties", "E011"},     // UDMPU 25
        Package(2) { "mipi-sdca-entity-id-0x12-subproperties", "E012"},     // OT 25
        // Clusters
        CLUSTER_INIT_JAMERSON,
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
        //Package(2) { "msft-stream-start-table", "BUF1"},
        //Package(2) { "msft-stream-stop-table", "BUF2"},
    }
#endif
}) // End _DSD

#undef _SMART_AMP_REFERENCE_STREAM_FEATURE_BITS


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
                                         FEATURE_CS42L43_AMP_NO_VOL_MUTE_R_COND)},
    }
}) // End EXT0


// +====================================+
// |    Function level controls         |
// +====================================+

// Function-level control definitions
Name(C001, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // Function_Commit_Group_Mask: Class, RW
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS },
        Package(2) { "mipi-sdca-control-access-mode", CAM_READ_WRITE },
        Package(2) { "mipi-sdca-control-default-value", 0x3E }, // Commit groups 1-5
    }
}) // End C001

Name(C004, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // Function_SDCA_Version: Class, DC = 1.0
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS },
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC },
        Package(2) { "mipi-sdca-control-dc-value", CTL_E0_FUNCTION_SDCA_VERSION_VAL },
    }
}) // End C004

Name(C005, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Function_Type: Class, DC = 0x1 (Smart Amp)
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x1},
    }
}) // End C005

Name(C006, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Function_Manufacturer_Id: Class, DC = 0x01FA
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x01FA},
    }
}) // End C006

Name(C007, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Function_Id: Class, DC = 0x4243
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
        // Function_Version: Class, DC = 0x1
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
        Package(2) { "mipi-sdca-control-interrupt-position", 6},
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


// ClusterID 1 definition
Name(CID1, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-channel-count", 2 },
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-channel-1-subproperties", "CH1P"},
        Package(2) { "mipi-sdca-channel-2-subproperties", "CH2P"},
    }
}) // End CID1


// ClusterID_1, Channel_1 Subproperties
Name(CH1P, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // ChannelCount(n1), n1 x {Channel_ID, Relationship, Purpose}
        Package(2) { "mipi-sdca-cluster-channel-id", 1 }, // *** ?
        Package(2) { "mipi-sdca-cluster-channel-purpose", 0x01 }, // Generic Audio
        Package(2) { "mipi-sdca-cluster-channel-relationship", 0x51 }, // Mic
    }
}) // CH1P

Name(CH2P, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // ChannelCount(n1), n1 x {Channel_ID, Relationship, Purpose}
        Package(2) { "mipi-sdca-cluster-channel-id", 2 }, // *** ?
        Package(2) { "mipi-sdca-cluster-channel-purpose", 0x01 }, // Generic Audio
        Package(2) { "mipi-sdca-cluster-channel-relationship", 0x51 }, // Mic
    }
}) // CH2P


// ClusterMap: Defines {ClusterIndex, Cluster_ID} relationships
Name(CMAP, Buffer()
{
    0x02, 0x00,    // Range type 0x0002
    0x01, 0x00, // 1 row
    0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,    // ClusterIndex 1 --> ClusterID 1
}) // End CMAP


// +====================================+
// |        Entity Definitions          |
// +====================================+

// +------------------------------------+
// |                IT_21               |
// |        Speaker Render Stream       |
// |      Streaming Input Terminal      |
// +------------------------------------+
Name(E006, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x02},
        Package(2) { "mipi-sdca-entity-label", "IT 21"},
        Package(2) { "mipi-sdca-terminal-type", 0x101},  // Speaker render stream. Generic Source / Sink Data Port
        Package(2) { "mipi-sdca-control-list", CTL_IT_DATAPORT_SELECTOR | CTL_IT_CLUSTERINDEX | CTL_IT_LATENCY | CTL_IT_USAGE},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        USAGE_IT_21,
        Package(2) { "mipi-sdca-control-0x8-subproperties", "C108"},  // Reuse
        Package (2) { "mipi-sdca-control-0x10-subproperties", "CI21"}, // Cluster Index
        Package(2) { "mipi-sdca-control-0x11-subproperties", "C111"}, // DataPortSelector
        Package(2) { "mipi-sdca-terminal-clock-connection", "E008" }, // CS_21
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-terminal-dp-numbers", "DPM1"},
    }
}) // End E006

Name(C108, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Latency: Class, DC = 0x4 (dummy value)
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 0x4},
    }
}) // End C108

// DataPortSelector for IT_21
Name(C111, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // DataPortSelector: Class, DC, *index* = 1 (see {DP_Index, DP_Num} map in DPM1)
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 1},
    }
}) // End C111

// DataPortMap1
Name(DPM1, Buffer() {
    0x1, COHEN_SPEAKER_RENDER_DATA_PORT // {DP_Index, DP_Num}
}) // End DPM1


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
        Package (2) {"mipi-sdca-input-pin-1", "E006"}, // Input Pin 1 connected to IT_21
    }
}) // End E004

Name(C024, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
         // PostureNumber: Class, Dual = 0
         Package (2) {"mipi-sdca-control-access-layer", 4},
         Package (2) {"mipi-sdca-control-access-mode", 1},
         Package (2) {"mipi-sdca-control-deferrable", 1},
         Package (2) {"mipi-sdca-control-dc-value", 1},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "PMAP"},
    }
}) // End C024


// +------------------------------------+
// |                CS_21               |
// +------------------------------------+
Name(E008, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0xB},
        Package(2) { "mipi-sdca-entity-label", "CS 21"},
        Package(2) { "mipi-sdca-cs-type", 0}, // External
        Package(2) { "mipi-sdca-control-list", CTL_CS_SAMPLERATEINDEX},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x10-subproperties", "C210"},
    }
}) // End E008

Name(C210, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // SampleRateIndex: Class, RW
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 0},
        Package(2) { "mipi-sdca-control-deferrable", 1},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "B210"},
    }
}) // End C210

// {SampleRateIndex, SampleRate} mapping.
Name(B210, Buffer()
{
    0x02, 0x00,  // Range type 0x0002 (Pairs)
    0x01, 0x00,  // Count of ranges = 0x2
    0x00, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00, 0x00, // SampleRateIndex = 0x00000000, SampleRate = 0x0000BB80 (48000)
    //0x01, 0x00, 0x00, 0x00, 0x44, 0xAC, 0x00, 0x00, // SampleRateIndex = 0x00000001, SampleRate = 0x0000AC44 (44100)
    //0x02, 0x00, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, // SampleRateIndex = 0x00000002, SampleRate = 96000 (0x00017700)
    //0x03, 0x00, 0x00, 0x00, 0x80, 0x3e, 0x00, 0x00, // SampleRateIndex = 0x00000003, SampleRate = 16000 (0x00003e80)
}) // End B210


// +------------------------------------+
// |                FU_21               |
// +------------------------------------+
Name(E005, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x7},
        Package(2) { "mipi-sdca-entity-label", "FU 21"},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
        Package(2) { "mipi-sdca-control-list", CTL_FU_VOLUME | CTL_FU_MUTE},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C601"}, // Mute
        Package(2) { "mipi-sdca-control-0x2-subproperties", "C602"}, // Volume
        Package(2) { "mipi-sdca-input-pin-1", "E004"}, // Input Pin 1 connected to PPU_21
    }
}) // End E005

// FU_21 Volume map
Name(VMAP, Buffer()
{
    0x03, 0x00,  // Range type 0x0003 (Triples)
    0x01, 0x00,  // Count of ranges = 0x1
    CS42L43_FU_21_VOL_MIN,
    CS42L43_FU_21_VOL_MAX,
    CS42L43_FU_21_VOL_STEP,
}) // End VMAP

Name(C601, Package()
{
    // Mute
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Mute: User, Dual, Control Numbers = {1,2}
        Package(2) { "mipi-sdca-control-access-layer", 1},
        Package(2) { "mipi-sdca-control-access-mode", 1},
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-cn-list", 0x6 }, // 0x1, 0x2
#ifdef FU_21_FIXED_RENDER_MUTE
        Package(2) { "mipi-sdca-control-fixed-value", 0},   // unmuted
#endif
    }
}) // End C601

Name(C602, Package()
{
    // Volume
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Channel Volume: User, Dual, Control Numbers = {1,2}
        Package(2) { "mipi-sdca-control-access-layer", 1},
        Package(2) { "mipi-sdca-control-access-mode", 1},
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-cn-list", 0x6 }, // 0x1, 0x2
#ifdef FU_21_FIXED_RENDER_VOLUME
        Package(2) { "mipi-sdca-control-fixed-value", CS42L43_FU_21_VOL_FIXED_Q7_8},
#else
        Package(2) { "mipi-sdca-control-default-value", CS42L43_FU_21_VOL_DEFAULT},
#endif
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "VMAP"},
    }
}) // End C602



// +------------------------------------+
// |                MFPU_21             |
// +------------------------------------+
Name(E010, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x22},
        Package(2) { "mipi-sdca-entity-label", "MFPU 21"},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
        Package(2) { "mipi-sdca-control-list", CTL_MFPU_BYPASS},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C501"},
#ifdef EXCLUDE_FU_21_VOLUME_CONTROL
        Package(2) { "mipi-sdca-input-pin-1", "E004"}, // Input Pin 1 connected to PPU_21 (skip FU_21)
#else
        Package(2) { "mipi-sdca-input-pin-1", "E005"}, // Input Pin 1 connected to FU_21
#endif
    }
}) // End E010

Name(C501, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Bypass: Class, DC = 1
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 0x1},
    }
}) // End C501


// +------------------------------------+
// |                XU_22               |
// +------------------------------------+
Name(E001, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x0A},
        Package(2) { "mipi-sdca-entity-label", "XU 22"},
        Package(2) { "mipi-sdca-input-pin-list", 0x6}, // Pin1 and Pin2 connected
        Package(2) { "mipi-sdca-control-list", CTL_XU_VERSION | CTL_XU_ID | CTL_XU_BYPASS},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C901"}, // Bypass
        Package(2) { "mipi-sdca-control-0x7-subproperties", "C907"}, // XU_ID
        Package(2) { "mipi-sdca-control-0x8-subproperties", "C908"}, // XU_Version
        Package(2) { "mipi-sdca-input-pin-1", "E010"}, // Input Pin 1 connected to MFPU_21
        Package(2) { "mipi-sdca-input-pin-2", "E007"}, // Input Pin 2 connected to IT_29
    }
}) // End E001

Name(C901, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Bypass, Class, RW.
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 0},
        Package(2) { "mipi-sdca-control-deferrable", 1},
    }
}) // End C901

Name(C907, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // XU_ID: Class, DC = 1
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 1},
    }
}) // End C907

Name(C908, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // XU_Version: Class, DC = 0x10
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 0x10},
    }
}) // End C908


// +------------------------------------+
// |               SAPU_29              |
// +------------------------------------+
Name(E009, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x24},
        Package (2) {"mipi-sdca-entity-label", "SAPU 29"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", CTL_SAPU_PROTECTION_STATUS | CTL_SAPU_PROTECTION_MODE},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-control-0x10-subproperties", "C039"},
        Package (2) {"mipi-sdca-control-0x11-subproperties", "C040"},
        Package (2) {"mipi-sdca-input-pin-1", "E001"}, // Input Pin 1 connected to XU_22
    }
}) // End E009

Name(C039, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // Protection_Mode: Class, RO
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 3},
        Package (2) { "mipi-sdca-control-deferrable", 1},
    }
}) // End C039

Name(C040, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // Protection_Status: Class, RO
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 3},
        Package (2) { "mipi-sdca-control-deferrable", 1},
    }
}) // End C040


// +------------------------------------+
// |               UDMPU_23             |
// +------------------------------------+
Name(E00A, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x21},
        Package (2) {"mipi-sdca-entity-label", "UDMPU 23"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", CTL_UDMPU_CLUSTERINDEX},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-control-0x10-subproperties", "CI23"},
        Package (2) {"mipi-sdca-input-pin-1", "E009"}, // Input Pin 1 connected to SAPU_29
    }
}) // End E00A


// +------------------------------------+
// |                FU_23               |
// +------------------------------------+
Name(E00B, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x7},
        Package(2) { "mipi-sdca-entity-label", "FU 23"},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
        Package(2) { "mipi-sdca-control-list", CTL_FU_CHANNELGAIN},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0xb-subproperties", "C400"},
        Package(2) { "mipi-sdca-input-pin-1", "E00A"}, // Input Pin 1 connected to UDMPU_23
    }
}) // End E00B

Name(C400, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // ### Changed to Application because with Mic topo occasionally the write to the Mute control #2 would fail. This should be revisited.
        // Channel Gain: Application, RW = 0
        Package(2) { "mipi-sdca-control-access-layer", 2}, // Class ***Platform***
        Package(2) { "mipi-sdca-control-access-mode", 0}, // RW
        Package(2) { "mipi-sdca-control-cn-list", 0x1 } // Master Gain
    }
}) // End C400


// +------------------------------------+
// |               UDMPU_25             |
// +------------------------------------+
Name(E011, Package() {
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
        Package (2) {"mipi-sdca-control-0x10-subproperties", "CI25"},
        Package (2) {"mipi-sdca-control-0x11-subproperties", "C053"},
        Package (2) {"mipi-sdca-control-0x12-subproperties", "C054"},
        Package (2) {"mipi-sdca-input-pin-1", "E00B"}, // Input Pin 1 connected to FU_23
    }
}) // End E011

Name(C053, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // Acoustic_Energy_Level_Monitor: Platform, DC = 0
        Package (2) {"mipi-sdca-control-access-layer", 8},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0},
    }
}) // End C053

Name(C054, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // Ultrasound_Level_Report: Platform, DC = 0
        Package (2) {"mipi-sdca-control-access-layer", 8},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0},
    }
}) // End C054


// +------------------------------------+
// |                OT_23               |
// |     Primary Transducer Output      |
// |     Transducer Output Terminal     |
// +------------------------------------+
Name(E00C, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x03},
        Package(2) { "mipi-sdca-entity-label", "OT 23"},
        // ATR: Ideally, this should be 0x382 (Primary Tweeter Speaker Transducer),
        // but specifying this value instead of 0x380 causes the audio functions
        // to fail to initialize.
        Package(2) { "mipi-sdca-terminal-type", 0x0380},
        Package(2) { "mipi-sdca-input-pin-list", 0x2},
        Package(2) { "mipi-sdca-control-list", CTL_OT_LATENCY | CTL_OT_USAGE},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        USAGE_OT_23,
        Package(2) { "mipi-sdca-control-0x8-subproperties", "CB08"}, // Latency
        Package(2) { "mipi-sdca-input-pin-1", "E00B"}, // Input Pin 1 connected to FU_23
    }
}) // End E00C

Name(CB08, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Latency: Class, DC = 0xA (dummy value)
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 5},
        Package(2) { "mipi-sdca-control-dc-value", 0xA},
    }
}) // End CB08


// +------------------------------------+
// |                PDE_23              |
// +------------------------------------+
Name(E002, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x11 },
        Package(2) { "mipi-sdca-entity-label", "PDE 23"},
        Package(2)
        {
            "mipi-sdca-powerdomain-managed-list",
            Package() {0xC}, // Entities OT_23 managed by this PDE.
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
        Package(2) { "mipi-sdca-control-list", CTL_PDE_ACTUAL_PS | CTL_PDE_REQUESTED_PS},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "RPS0" }, // Reuse. Requested PS
        Package(2) { "mipi-sdca-control-0x10-subproperties", "APS0"}, // Reuse. Actual PS
    }
}) // End E002

Name(RPS0, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Requested_PS, Class, R/W
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 0},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "PDER"},
    },
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
        Package(2) { "mipi-sdca-control-access-layer", 4},
        Package(2) { "mipi-sdca-control-access-mode", 3},
    }
}) // End APS0


// +------------------------------------+
// |                OT_25               |
// |      Reference Stream Output       |
// |     Streaming Output Terminal      |
// +------------------------------------+
Name(E012, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) { "mipi-sdca-entity-type", 0x3},
        Package (2) { "mipi-sdca-entity-label", "OT 25"},
        Package (2) { "mipi-sdca-input-pin-list", 0x2},
        Package (2) { "mipi-sdca-terminal-type", 0x188},
        Package (2) { "mipi-sdca-control-list", CTL_OT_MATCHING_GUID | CTL_OT_DATAPORT_SELECTOR | CTL_OT_USAGE},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        USAGE_OT_25,
        Package (2) { "mipi-sdca-control-0x11-subproperties", "C056"},
        Package (2) { "mipi-sdca-control-0x12-subproperties", "C057"},
        Package (2) { "mipi-sdca-terminal-clock-connection", "E008"}, // CS_21
        Package (2) { "mipi-sdca-input-pin-1", "E011"}, // Input Pin 1 connected to UDMPU_25
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) { "mipi-sdca-terminal-dp-numbers", "DPM2"},
    }
}) // End E012

// DataPortMap2
Name(DPM2, Buffer() {
    0x2, COHEN_REF_STREAM_DATA_PORT, // {DP_Index, DP_Num}
}) // End DPM2

Name(C056, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // DataPort Selector: Class, DC = 2
        Package (2) { "mipi-sdca-control-access-layer", 0x4},
        Package (2) { "mipi-sdca-control-access-mode", 5},
        Package (2) { "mipi-sdca-control-dc-value", 0x2},
    }
}) // End C056

Name(C057, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // Matching_GUID: Class, DC = 0
        Package (2) { "mipi-sdca-control-access-layer", 0x4},
        Package (2) { "mipi-sdca-control-access-mode", 5},
        Package (2) { "mipi-sdca-control-dc-value", 0},
    }
}) // End C057


// +------------------------------------+
// |                IT_29               |
// |      Speaker Sense Data Input      |
// |     Transducer Input Terminal      |
// +------------------------------------+
Name(E007, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x2},
        Package (2) {"mipi-sdca-entity-label", "IT 29"},
        Package (2) {"mipi-sdca-terminal-type", 0x280},
        Package (2) {"mipi-sdca-control-list", CTL_IT_CLUSTERINDEX | CTL_IT_LATENCY | CTL_IT_USAGE},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        USAGE_IT_29,
        Package (2) {"mipi-sdca-control-0x8-subproperties", "C059"},
        Package (2) {"mipi-sdca-control-0x10-subproperties", "CI29"},
    },
}) // End E007

Name(C059, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // Latency: Class, DC = 7
        Package (2) {"mipi-sdca-control-access-layer", 4},
        Package (2) {"mipi-sdca-control-access-mode", 5},
        Package (2) {"mipi-sdca-control-dc-value", 0x7}, // Dummy Value
    }
}) // End C059


#if TG_23
// +------------------------------------+
// |                TG_23               |
// +------------------------------------+
Name(E003, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x30},
        Package (2) {"mipi-sdca-entity-label", "TG 23"},
        Package (2) {"mipi-sdca-control-list", CTL_TG_TONE_DIVIDER},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-control-0x10-subproperties", "C300"},
    },
}) // End E003

Name(C300, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // Class_Divider: Platform, RW
        Package (2) {"mipi-sdca-control-access-layer", 8},
        Package (2) {"mipi-sdca-control-access-mode", 0},
    }
}) // End C300
#endif
