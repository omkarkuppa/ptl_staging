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

#include "version.h"

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

#define COHEN_CLUSTER
#define COHEN_USAGE
#include <SmartAmp-Clusters.asl>
#include <SmartAmp-Usage.asl>
#undef COHEN_USAGE
#undef COHEN_CLUSTER


#ifdef COHEN_DISABLE_REF_STREAM
# define _SMART_AMP_REFERENCE_STREAM_FEATURE_BITS   0
#else
# define _SMART_AMP_REFERENCE_STREAM_FEATURE_BITS   (1 << 5) | (1 << 6)
#endif

#ifdef DISABLE_US_STREAM
# define COHEN_RENDER_STREAM_ENTITY_ID_LIST         0x8, 0x6, 0x4, 0x10, 0x1, 0x7, 0x9, 0xA, 0xB, 0x2, 0xC,
#else
# define COHEN_RENDER_STREAM_ENTITY_ID_LIST         0x8, 0x6, 0x4, 0x10, 0x1, 0x7, 0x9, 0x18, 0xA, 0xB, 0x2, 0xC,
#endif

#define COHEN_US_STREAM_ENTITY_ID_LIST              0x13, 0x14, 0x15, 0x16, 0x17,

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
        //
        // NOTE:
        //  The MSFT Class driver does not parse mipi-sdca-function-topology-features.
        //  MSFT recommends not including it in DisCo until the Class driver has added
        //  support for it.
        //
        //Package(2) { "mipi-sdca-function-topology-features",
        //    _SMART_AMP_REFERENCE_STREAM_FEATURE_BITS
        //},
        Package(2) { "mipi-sdca-control-list", (CTL_E0_FUNCTION_VERSION | CTL_E0_FUNCTION_ID | CTL_E0_FUNCTION_MANUFACTURER_ID |
                                                CTL_E0_FUNCTION_TYPE | CTL_E0_FUNCTION_SDCA_VERSION | CTL_E0_COMMIT_GROUP_MASK | CTL_E0_FUNCTION_STATUS |
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
#ifndef DISABLE_US_STREAM
            COHEN_US_STREAM_ENTITY_ID_LIST
#endif
            },
        },
        Package(2) { "mipi-sdca-function-busy-max-delay", 5000},
        // Clusters
        CLUSTER_ID_LIST_SMART_AMP,
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
        Package(2) { "mipi-sdca-control-0x10-subproperties", "C010"},
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
        Package(2) { "mipi-sdca-entity-id-0x10-subproperties", "E010"},     // MFPU 21
        Package(2) { "mipi-sdca-entity-id-0x11-subproperties", "E011"},     // UDMPU 25
        Package(2) { "mipi-sdca-entity-id-0x12-subproperties", "E012"},     // OT 25

        Package(2) { "mipi-sdca-entity-id-0x13-subproperties", "E013"},     // IT 26
        Package(2) { "mipi-sdca-entity-id-0x14-subproperties", "E014"},     // CS 26
        Package(2) { "mipi-sdca-entity-id-0x15-subproperties", "E015"},     // PPU 26
        Package(2) { "mipi-sdca-entity-id-0x16-subproperties", "E016"},     // MFPU 26
        Package(2) { "mipi-sdca-entity-id-0x17-subproperties", "E017"},     // FU 26
        Package(2) { "mipi-sdca-entity-id-0x18-subproperties", "E018"},     // MU 26
        // Clusters
        CLUSTER_INIT_JAMERSON,
        // Vendor-specific
        Package(2) { "mipi-sdca-function-expansion-subproperties", "EXT0"},
    },
    //
    // For WIN10 compatibility, the Buffer Data Extension cannot be empty.
    //
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        // Class driver will print warnings if these are not defined.
        // To not pollute the class driver logs, simply do not define these if we are not using them.
        Package(2) { "mipi-sdca-function-initialization-table", "BUF0"},
        //Package(2) { "msft-stream-start-table", "BUF1"},
        //Package(2) { "msft-stream-stop-table", "BUF2"},
    }
}) // End _DSD

#undef _SMART_AMP_REFERENCE_STREAM_FEATURE_BITS

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
        Package(2) { "01fa-release-version", RELEASE_VERSION },
        Package(2) { "01fa-ssid-ex", 0x7 },
        //
        // This property is used to select HWKWS FW.
        // Since the audio function performing the FDL is selected at random,
        // all audio functions involved with FDL must have it defined.
        Package(2) {"01fa-xu-features", (FEATURE_ENABLE_HWKWS | FEATURE_ENABLE_WT | FEATURE_ENABLE_KNCK | FEATURE_NO_FUN_STS |
                                         FEATURE_CS42L43_AMP_NO_VOL_MUTE_R_COND)},
#ifdef GLOBAL_MUTE_LED_MIC_GPIO_NUM
        package(2) {"01fa-global-mute-led-mic-mute-gpio", GLOBAL_MUTE_LED_MIC_GPIO_NUM },
#endif
#ifdef GLOBAL_MUTE_LED_SPK_GPIO_NUM
        package(2) {"01fa-global-mute-led-spk-mute-gpio", GLOBAL_MUTE_LED_SPK_GPIO_NUM },
#endif
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

Name(C010, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Class, RW1C, Function_Status
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_RW1C},
        Package(2) { "mipi-sdca-control-interrupt-position", COHEN_SDCA_AMP_FUNC_STATUS_INT},
    }
}) // End C010

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
        Package(2) { "mipi-sdca-control-0x10-subproperties", "CI21"}, // Cluster Index
        Package(2) { "mipi-sdca-control-0x11-subproperties", "C111"}, // DataPortSelector
        Package(2) { "mipi-sdca-terminal-clock-connection", "E008" }, // CS_21
    },
#if CTL_E0_FUNCTION_SDCA_VERSION_VAL < 0x10
//
// NOTE:
//  "mipi-sdca-terminal-dp-numbers" has been removed from DisCo for SoundWire 2.1 spec so they should no longer be present in the ASL file.
//  "mipi-sdca-terminal-dp-numbers" has been deprecated, and 1.0 Conformant devices will report DP numbers as a range for DataPort_Selector control.
//
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-terminal-dp-numbers", "DPM1"},
    }
#endif
}) // End E006

Name(C108, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Latency: Class, DC = 0x4 (dummy value)
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x4},
    }
}) // End C108

// DataPortSelector for IT_21
Name(C111, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // DataPortSelector: Class, DC, *index* = 1 (see {DP_Index, DP_Num} map in DPM3)
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 1},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-control-range", "DPM3"},
    }
}) // End C111

#if CTL_E0_FUNCTION_SDCA_VERSION_VAL < 0x10
// DataPortMap1
Name(DPM1, Buffer() {
    0x1, COHEN_SPEAKER_RENDER_DATA_PORT // {DP_Index, DP_Num}
}) // End DPM1
#endif

// DataPortMap
Name(DPM3, Buffer() {
    0x10, 0x00,                 // Range type 0x0010
    0x04, 0x00,                 // NumRows = 4
    // DP_Index_A
    0xFF, 0x00, 0x00, 0x00,     // 0: not used
    0x05, 0x00, 0x00, 0x00,     // 1: DP5
    0x07, 0x00, 0x00, 0x00,     // 2: DP7
    0xFF, 0x00, 0x00, 0x00,     // 3: not used
    0xFF, 0x00, 0x00, 0x00,     // 4: not used
    0xFF, 0x00, 0x00, 0x00,     // 5: not used
    0xFF, 0x00, 0x00, 0x00,     // 6: not used
    0xFF, 0x00, 0x00, 0x00,     // 7: not used
    0xFF, 0x00, 0x00, 0x00,     // 8: not used
    0xFF, 0x00, 0x00, 0x00,     // 9: not used
    0xFF, 0x00, 0x00, 0x00,     // 10: not used
    0xFF, 0x00, 0x00, 0x00,     // 11: not used
    0xFF, 0x00, 0x00, 0x00,     // 12: not used
    0xFF, 0x00, 0x00, 0x00,     // 13: not used
    0xFF, 0x00, 0x00, 0x00,     // 14: not used
    0xFF, 0x00, 0x00, 0x00,     // 15: not used
    // DP_Index_B
    0xFF, 0x00, 0x00, 0x00,     // 0: not used
    0xFF, 0x00, 0x00, 0x00,     // 1: not used
    0xFF, 0x00, 0x00, 0x00,     // 2: not used
    0xFF, 0x00, 0x00, 0x00,     // 3: not used
    0xFF, 0x00, 0x00, 0x00,     // 4: not used
    0xFF, 0x00, 0x00, 0x00,     // 5: not used
    0xFF, 0x00, 0x00, 0x00,     // 6: not used
    0xFF, 0x00, 0x00, 0x00,     // 7: not used
    0xFF, 0x00, 0x00, 0x00,     // 8: not used
    0xFF, 0x00, 0x00, 0x00,     // 9: not used
    0xFF, 0x00, 0x00, 0x00,     // 10: not used
    0xFF, 0x00, 0x00, 0x00,     // 11: not used
    0xFF, 0x00, 0x00, 0x00,     // 12: not used
    0xFF, 0x00, 0x00, 0x00,     // 13: not used
    0xFF, 0x00, 0x00, 0x00,     // 14: not used
    0xFF, 0x00, 0x00, 0x00,     // 15: not used
    // DP_Index_C
    0xFF, 0x00, 0x00, 0x00,     // 0: not used
    0xFF, 0x00, 0x00, 0x00,     // 1: not used
    0xFF, 0x00, 0x00, 0x00,     // 2: not used
    0xFF, 0x00, 0x00, 0x00,     // 3: not used
    0xFF, 0x00, 0x00, 0x00,     // 4: not used
    0xFF, 0x00, 0x00, 0x00,     // 5: not used
    0xFF, 0x00, 0x00, 0x00,     // 6: not used
    0xFF, 0x00, 0x00, 0x00,     // 7: not used
    0xFF, 0x00, 0x00, 0x00,     // 8: not used
    0xFF, 0x00, 0x00, 0x00,     // 9: not used
    0xFF, 0x00, 0x00, 0x00,     // 10: not used
    0xFF, 0x00, 0x00, 0x00,     // 11: not used
    0xFF, 0x00, 0x00, 0x00,     // 12: not used
    0xFF, 0x00, 0x00, 0x00,     // 13: not used
    0xFF, 0x00, 0x00, 0x00,     // 14: not used
    0xFF, 0x00, 0x00, 0x00,     // 15: not used
    // DP_Index_D
    0xFF, 0x00, 0x00, 0x00,     // 0: not used
    0xFF, 0x00, 0x00, 0x00,     // 1: not used
    0xFF, 0x00, 0x00, 0x00,     // 2: not used
    0xFF, 0x00, 0x00, 0x00,     // 3: not used
    0xFF, 0x00, 0x00, 0x00,     // 4: not used
    0xFF, 0x00, 0x00, 0x00,     // 5: not used
    0xFF, 0x00, 0x00, 0x00,     // 6: not used
    0xFF, 0x00, 0x00, 0x00,     // 7: not used
    0xFF, 0x00, 0x00, 0x00,     // 8: not used
    0xFF, 0x00, 0x00, 0x00,     // 9: not used
    0xFF, 0x00, 0x00, 0x00,     // 10: not used
    0xFF, 0x00, 0x00, 0x00,     // 11: not used
    0xFF, 0x00, 0x00, 0x00,     // 12: not used
    0xFF, 0x00, 0x00, 0x00,     // 13: not used
    0xFF, 0x00, 0x00, 0x00,     // 14: not used
    0xFF, 0x00, 0x00, 0x00,     // 15: not used
}) // End DPM3


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
         Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
         Package (2) {"mipi-sdca-control-access-mode", CAM_DUAL},
         Package (2) {"mipi-sdca-control-deferrable", 1},
         Package (2) {"mipi-sdca-control-default-value", 0},    // Posture Number = 0, no rotation
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
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_READ_WRITE},
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
        Package(2) { "mipi-sdca-control-access-layer", CAL_USER},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DUAL},
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
        Package(2) { "mipi-sdca-control-access-layer", CAL_USER},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DUAL},
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
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
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
        Package(2) { "mipi-sdca-control-list", CTL_XU_VERSION | CTL_XU_ID | CTL_XU_BYPASS | CTL_XU_IMPDEF_GPIO},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C901"}, // Bypass
        Package(2) { "mipi-sdca-control-0x7-subproperties", "C907"}, // XU_ID
        Package(2) { "mipi-sdca-control-0x8-subproperties", "C908"}, // XU_Version
        Package(2) { "mipi-sdca-control-0x30-subproperties", "C930"}, // GPIO
        Package(2) { "mipi-sdca-input-pin-1", "E010"}, // Input Pin 1 connected to MFPU_21
        Package(2) { "mipi-sdca-input-pin-2", "E007"}, // Input Pin 2 connected to IT_29
    }
}) // End E001

Name(C901, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Bypass, Class, RW.
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package(2) { "mipi-sdca-control-deferrable", 1},
    }
}) // End C901

Name(C907, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // XU_ID: Class, DC = 1
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 1},
    }
}) // End C907

Name(C908, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // XU_Version: Class, DC = 0x10
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x10},
    }
}) // End C908

Name(C930, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // GPIO, Extension, RW.
        Package(2) { "mipi-sdca-control-access-layer", CAL_EXTENSION},
        Package(2) { "mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-cn-list", 0x7}, // Control Numbers = {0,1,2}
    }
}) // End C930


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
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_RO},
        Package (2) { "mipi-sdca-control-deferrable", 1},
    }
}) // End C039

Name(C040, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // Protection_Status: Class, RO
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_RO},
        Package (2) { "mipi-sdca-control-deferrable", 1},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-control-range", "BMT1"},
    }
}) // End C040

// Protection_Status BitIndex Mapping Table (BMT)
Name(BMT1, Buffer() {
    0x03, 0x00,             // Range type 0x0003
    0x08, 0x00,             // NumRows = 8
    // Bit 0: SAPU_FIRMWARE_MISSING = 0x1
    0x00, 0x00, 0x00, 0x00, // Bit Number = 0
    0x00, 0x00, 0x00, 0x00, // Behavior Set ID = 0 -> CBN = 1010
    0x00, 0x00, 0x00, 0x00, // String Number = 0 -> "Firmware Missing"
    // Bit 1: SAPU_MISSING_OPAQUE_SET = 0x2
    0x01, 0x00, 0x00, 0x00, // Bit Number = 1
    0x01, 0x00, 0x00, 0x00, // Behavior Set ID = 1 -> CBN = 1080
    0x01, 0x00, 0x00, 0x00, // String Number = 1 -> "Opaque Set Missing"
    // Bit 2: SAPU_PROTECT_LITE_FAULT = 0x4
    0x02, 0x00, 0x00, 0x00, // Bit Number = 2
    0x02, 0x00, 0x00, 0x00, // Behavior Set ID = 2 -> CBN = 1060
    0x02, 0x00, 0x00, 0x00, // String Number = 2 -> "Internal Condition"
    // Bit 3: SAPU_SUPPLY_FAULT = 0x8
    0x03, 0x00, 0x00, 0x00, // Bit Number = 3
    0x03, 0x00, 0x00, 0x00, // Behavior Set ID = 3 -> CBN = 1100
    0x03, 0x00, 0x00, 0x00, // String Number = 3 -> "Catastrophic Fault"
    // Bit 4: SAPU_INTERNAL_FAULT = 0x10
    0x04, 0x00, 0x00, 0x00, // Bit Number = 4
    0x02, 0x00, 0x00, 0x00, // Behavior Set ID = 2 -> CBN = 1060
    0x02, 0x00, 0x00, 0x00, // String Number = 2 -> "Internal Condition"
    // Bit 5: SAPU_TRANSDUCER_FAULT = 0x20
    0x05, 0x00, 0x00, 0x00, // Bit Number = 5
    0x04, 0x00, 0x00, 0x00, // Behavior Set ID = 4 -> CBN = 1070
    0x04, 0x00, 0x00, 0x00, // String Number = 4 -> "External Condition"
    // Bit 6: SAPU_CRC_FAULT = 0x40
    0x06, 0x00, 0x00, 0x00, // Bit Number = 6
    0x03, 0x00, 0x00, 0x00, // Behavior Set ID = 3 -> CBN = 1100
    0x03, 0x00, 0x00, 0x00, // String Number = 3 -> "Catastrophic Fault"
    // Bit 7: SAPU_THERM_WARNING = 0x80
    0x07, 0x00, 0x00, 0x00, // Bit Number = 7
    0x02, 0x00, 0x00, 0x00, // Behavior Set ID = 2 -> CBN = 1060
    0x02, 0x00, 0x00, 0x00, // String Number = 2 -> "Internal Condition"
}) // End BMT1

//
// NOTE:
//  The MSFT class driver does not parse mipi-sdca-behavior-set-id-list,
//  mipi-sdca-behavior-set-id-0x<n>-subproperties, and mipi-sdca-string-table.
//  MSFT recommends not including them in DisCo until the Class driver has added
//  support for them.
//



// +------------------------------------+
// |                MU_26               |
// +------------------------------------+
Name(E018, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x5},
        Package (2) {"mipi-sdca-entity-label", "MU 26"},
#ifdef DISABLE_US_STREAM
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
#else
        Package (2) {"mipi-sdca-input-pin-list", 0x6},
#endif
        Package (2) {"mipi-sdca-control-list", CTL_MU_MIXER},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-1", "E009"},
#ifndef DISABLE_US_STREAM
        Package (2) {"mipi-sdca-input-pin-2", "E017"},
#endif
        Package (2) {"mipi-sdca-control-0x1-subproperties", "C801"},
    }
}) // End E00A


Name(C801, Package()
{
    // Mixer
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // Platform, DC, Control Numbers = {0,1}
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x0000},  // Q7.8
        Package(2) { "mipi-sdca-control-cn-list", 0x3},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-control-range", "B801"},
    }
}) // End C049


// Mixer: unity gain
Name(B801, Buffer() {
    0x01, 0x00,             // Range type 0x0001
    0x01, 0x00,             // NumRows = 1
    0x00, 0x00, 0x00, 0x00, // 0.0dB
}) // End B801


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
#ifdef DISABLE_US_STREAM
        Package (2) {"mipi-sdca-input-pin-1", "E009"}, // Input Pin 1 connected to SAPU_29, skipping MU_26
#else
        Package (2) {"mipi-sdca-input-pin-1", "E018"}, // Input Pin 1 connected to MU_26
#endif
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
        Package(2) { "mipi-sdca-control-list", CTL_FU_MUTE | CTL_FU_CHANNELGAIN},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C02A"},
        Package(2) { "mipi-sdca-control-0xB-subproperties", "C400"},
        Package(2) { "mipi-sdca-input-pin-1", "E00A"}, // Input Pin 1 connected to UDMPU_23
    }
}) // End E00B

Name(C400, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Channel Gain: Platform, RW
        Package(2) { "mipi-sdca-control-access-layer", CAL_PLATFORM},
        Package(2) { "mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-cn-list", 0x6 } // 0x1, 0x2
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
        Package (2) {"mipi-sdca-control-access-layer", CAL_PLATFORM},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0},
    }
}) // End C053

Name(C054, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // Ultrasound_Level_Report: Platform, DC = 0
        Package (2) {"mipi-sdca-control-access-layer", CAL_PLATFORM},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
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
        Package(2) { "mipi-sdca-terminal-connector-type", 0xFF},    // Other connector type
        Package(2) { "mipi-sdca-terminal-transducer-count", 2},
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
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
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
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_READ_WRITE},
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
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_RO},
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
#if CTL_E0_FUNCTION_SDCA_VERSION_VAL < 0x10
//
// NOTE:
//  "mipi-sdca-terminal-dp-numbers" has been removed from DisCo for SoundWire 2.1 spec so they should no longer be present in the ASL file.
//  "mipi-sdca-terminal-dp-numbers" has been deprecated, and 1.0 Conformant devices will report DP numbers as a range for DataPort_Selector control.
//
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) { "mipi-sdca-terminal-dp-numbers", "DPM2"},
    }
#endif
}) // End E012

#if CTL_E0_FUNCTION_SDCA_VERSION_VAL < 0x10
// DataPortMap2
Name(DPM2, Buffer() {
    0x2, COHEN_REF_STREAM_DATA_PORT, // {DP_Index, DP_Num}
}) // End DPM2
#endif


Name(C056, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // DataPort Selector: Class, DC = 2
        Package (2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package (2) { "mipi-sdca-control-dc-value", 0x2},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-control-range", "DPM4"},
    }
}) // End C056

// DataPortMap
Name(DPM4, Buffer() {
    0x10, 0x00,                 // Range type 0x0010
    0x04, 0x00,                 // NumRows = 4
    // DP_Index_A
    0xFF, 0x00, 0x00, 0x00,     // 0: not used
    0xFF, 0x00, 0x00, 0x00,     // 1: not used
    0x03, 0x00, 0x00, 0x00,     // 2: DP3
    0xFF, 0x00, 0x00, 0x00,     // 3: not used
    0xFF, 0x00, 0x00, 0x00,     // 4: not used
    0xFF, 0x00, 0x00, 0x00,     // 5: not used
    0xFF, 0x00, 0x00, 0x00,     // 6: not used
    0xFF, 0x00, 0x00, 0x00,     // 7: not used
    0xFF, 0x00, 0x00, 0x00,     // 8: not used
    0xFF, 0x00, 0x00, 0x00,     // 9: not used
    0xFF, 0x00, 0x00, 0x00,     // 10: not used
    0xFF, 0x00, 0x00, 0x00,     // 11: not used
    0xFF, 0x00, 0x00, 0x00,     // 12: not used
    0xFF, 0x00, 0x00, 0x00,     // 13: not used
    0xFF, 0x00, 0x00, 0x00,     // 14: not used
    0xFF, 0x00, 0x00, 0x00,     // 15: not used
    // DP_Index_B
    0xFF, 0x00, 0x00, 0x00,     // 0: not used
    0xFF, 0x00, 0x00, 0x00,     // 1: not used
    0xFF, 0x00, 0x00, 0x00,     // 2: not used
    0xFF, 0x00, 0x00, 0x00,     // 3: not used
    0xFF, 0x00, 0x00, 0x00,     // 4: not used
    0xFF, 0x00, 0x00, 0x00,     // 5: not used
    0xFF, 0x00, 0x00, 0x00,     // 6: not used
    0xFF, 0x00, 0x00, 0x00,     // 7: not used
    0xFF, 0x00, 0x00, 0x00,     // 8: not used
    0xFF, 0x00, 0x00, 0x00,     // 9: not used
    0xFF, 0x00, 0x00, 0x00,     // 10: not used
    0xFF, 0x00, 0x00, 0x00,     // 11: not used
    0xFF, 0x00, 0x00, 0x00,     // 12: not used
    0xFF, 0x00, 0x00, 0x00,     // 13: not used
    0xFF, 0x00, 0x00, 0x00,     // 14: not used
    0xFF, 0x00, 0x00, 0x00,     // 15: not used
    // DP_Index_C
    0xFF, 0x00, 0x00, 0x00,     // 0: not used
    0xFF, 0x00, 0x00, 0x00,     // 1: not used
    0xFF, 0x00, 0x00, 0x00,     // 2: not used
    0xFF, 0x00, 0x00, 0x00,     // 3: not used
    0xFF, 0x00, 0x00, 0x00,     // 4: not used
    0xFF, 0x00, 0x00, 0x00,     // 5: not used
    0xFF, 0x00, 0x00, 0x00,     // 6: not used
    0xFF, 0x00, 0x00, 0x00,     // 7: not used
    0xFF, 0x00, 0x00, 0x00,     // 8: not used
    0xFF, 0x00, 0x00, 0x00,     // 9: not used
    0xFF, 0x00, 0x00, 0x00,     // 10: not used
    0xFF, 0x00, 0x00, 0x00,     // 11: not used
    0xFF, 0x00, 0x00, 0x00,     // 12: not used
    0xFF, 0x00, 0x00, 0x00,     // 13: not used
    0xFF, 0x00, 0x00, 0x00,     // 14: not used
    0xFF, 0x00, 0x00, 0x00,     // 15: not used
    // DP_Index_D
    0xFF, 0x00, 0x00, 0x00,     // 0: not used
    0xFF, 0x00, 0x00, 0x00,     // 1: not used
    0xFF, 0x00, 0x00, 0x00,     // 2: not used
    0xFF, 0x00, 0x00, 0x00,     // 3: not used
    0xFF, 0x00, 0x00, 0x00,     // 4: not used
    0xFF, 0x00, 0x00, 0x00,     // 5: not used
    0xFF, 0x00, 0x00, 0x00,     // 6: not used
    0xFF, 0x00, 0x00, 0x00,     // 7: not used
    0xFF, 0x00, 0x00, 0x00,     // 8: not used
    0xFF, 0x00, 0x00, 0x00,     // 9: not used
    0xFF, 0x00, 0x00, 0x00,     // 10: not used
    0xFF, 0x00, 0x00, 0x00,     // 11: not used
    0xFF, 0x00, 0x00, 0x00,     // 12: not used
    0xFF, 0x00, 0x00, 0x00,     // 13: not used
    0xFF, 0x00, 0x00, 0x00,     // 14: not used
    0xFF, 0x00, 0x00, 0x00,     // 15: not used
}) // End DPM4

Name(C057, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // Matching_GUID: Class, DC = 0
        Package (2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) { "mipi-sdca-control-access-mode", CAM_DC},
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
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
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
        Package (2) {"mipi-sdca-control-access-layer", CAL_PLATFORM},
        Package (2) {"mipi-sdca-control-access-mode", CAM_READ_WRITE},
    }
}) // End C300
#endif


// +------------------------------------+
// |                IT_26               |
// |      Ultrasound Render Stream      |
// |            Sink Data Port          |
// +------------------------------------+
Name(E013, Package() {
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
        Package (2) {"mipi-sdca-control-0x11-subproperties", "C042"}, // DataPort_Selector
        Package (2) {"mipi-sdca-terminal-clock-connection", "E014"},
    },
#if CTL_E0_FUNCTION_SDCA_VERSION_VAL < 0x10
//
// NOTE:
//  "mipi-sdca-terminal-dp-numbers" has been removed from DisCo for SoundWire 2.1 spec so they should no longer be present in the ASL file.
//  "mipi-sdca-terminal-dp-numbers" has been deprecated, and 1.0 Conformant devices will report DP numbers as a range for DataPort_Selector control.
//
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) { "mipi-sdca-terminal-dp-numbers", "DPM7"}, // Legacy map
    }
#endif
}) // End E013

Name(C042, Package() {
    // DataPortSelector: Class, DC, *index* = 1 (see {DP_Index, DP_Num} map in DPM8)
    // NOTE: Potentially could change to DPM3, index 2
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 1},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-control-range", "DPM8"},
    }
}) // End C042

#if CTL_E0_FUNCTION_SDCA_VERSION_VAL < 0x10
// DataPortMap7
Name(DPM7, Buffer() {
    0x1, 7, // {DP_Index, DP_Num}
}) // End DPM7
#endif

// DataPortMap 8
Name(DPM8, Buffer() {
    0x10, 0x00,                 // Range type 0x0010
    0x04, 0x00,                 // NumRows = 4
    // DP_Index_A
    0xFF, 0x00, 0x00, 0x00,     // 0: not used
    0x07, 0x00, 0x00, 0x00,     // 1: DP7
    0xFF, 0x00, 0x00, 0x00,     // 2: not used
    0xFF, 0x00, 0x00, 0x00,     // 3: not used
    0xFF, 0x00, 0x00, 0x00,     // 4: not used
    0xFF, 0x00, 0x00, 0x00,     // 5: not used
    0xFF, 0x00, 0x00, 0x00,     // 6: not used
    0xFF, 0x00, 0x00, 0x00,     // 7: not used
    0xFF, 0x00, 0x00, 0x00,     // 8: not used
    0xFF, 0x00, 0x00, 0x00,     // 9: not used
    0xFF, 0x00, 0x00, 0x00,     // 10: not used
    0xFF, 0x00, 0x00, 0x00,     // 11: not used
    0xFF, 0x00, 0x00, 0x00,     // 12: not used
    0xFF, 0x00, 0x00, 0x00,     // 13: not used
    0xFF, 0x00, 0x00, 0x00,     // 14: not used
    0xFF, 0x00, 0x00, 0x00,     // 15: not used
    // DP_Index_B
    0xFF, 0x00, 0x00, 0x00,     // 0: not used
    0xFF, 0x00, 0x00, 0x00,     // 1: not used
    0xFF, 0x00, 0x00, 0x00,     // 2: not used
    0xFF, 0x00, 0x00, 0x00,     // 3: not used
    0xFF, 0x00, 0x00, 0x00,     // 4: not used
    0xFF, 0x00, 0x00, 0x00,     // 5: not used
    0xFF, 0x00, 0x00, 0x00,     // 6: not used
    0xFF, 0x00, 0x00, 0x00,     // 7: not used
    0xFF, 0x00, 0x00, 0x00,     // 8: not used
    0xFF, 0x00, 0x00, 0x00,     // 9: not used
    0xFF, 0x00, 0x00, 0x00,     // 10: not used
    0xFF, 0x00, 0x00, 0x00,     // 11: not used
    0xFF, 0x00, 0x00, 0x00,     // 12: not used
    0xFF, 0x00, 0x00, 0x00,     // 13: not used
    0xFF, 0x00, 0x00, 0x00,     // 14: not used
    0xFF, 0x00, 0x00, 0x00,     // 15: not used
    // DP_Index_C
    0xFF, 0x00, 0x00, 0x00,     // 0: not used
    0xFF, 0x00, 0x00, 0x00,     // 1: not used
    0xFF, 0x00, 0x00, 0x00,     // 2: not used
    0xFF, 0x00, 0x00, 0x00,     // 3: not used
    0xFF, 0x00, 0x00, 0x00,     // 4: not used
    0xFF, 0x00, 0x00, 0x00,     // 5: not used
    0xFF, 0x00, 0x00, 0x00,     // 6: not used
    0xFF, 0x00, 0x00, 0x00,     // 7: not used
    0xFF, 0x00, 0x00, 0x00,     // 8: not used
    0xFF, 0x00, 0x00, 0x00,     // 9: not used
    0xFF, 0x00, 0x00, 0x00,     // 10: not used
    0xFF, 0x00, 0x00, 0x00,     // 11: not used
    0xFF, 0x00, 0x00, 0x00,     // 12: not used
    0xFF, 0x00, 0x00, 0x00,     // 13: not used
    0xFF, 0x00, 0x00, 0x00,     // 14: not used
    0xFF, 0x00, 0x00, 0x00,     // 15: not used
    // DP_Index_D
    0xFF, 0x00, 0x00, 0x00,     // 0: not used
    0xFF, 0x00, 0x00, 0x00,     // 1: not used
    0xFF, 0x00, 0x00, 0x00,     // 2: not used
    0xFF, 0x00, 0x00, 0x00,     // 3: not used
    0xFF, 0x00, 0x00, 0x00,     // 4: not used
    0xFF, 0x00, 0x00, 0x00,     // 5: not used
    0xFF, 0x00, 0x00, 0x00,     // 6: not used
    0xFF, 0x00, 0x00, 0x00,     // 7: not used
    0xFF, 0x00, 0x00, 0x00,     // 8: not used
    0xFF, 0x00, 0x00, 0x00,     // 9: not used
    0xFF, 0x00, 0x00, 0x00,     // 10: not used
    0xFF, 0x00, 0x00, 0x00,     // 11: not used
    0xFF, 0x00, 0x00, 0x00,     // 12: not used
    0xFF, 0x00, 0x00, 0x00,     // 13: not used
    0xFF, 0x00, 0x00, 0x00,     // 14: not used
    0xFF, 0x00, 0x00, 0x00,     // 15: not used
}) // End DPM8



// +------------------------------------+
// |                CS_26               |
// +------------------------------------+
Name(E014, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0xB},
        Package (2) {"mipi-sdca-entity-label", "CS 26"},
        Package (2) {"mipi-sdca-cs-type", 0},
        Package (2) {"mipi-sdca-control-list", CTL_CS_SAMPLERATEINDEX},
        },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-control-0x10-subproperties", "C210"},
    }
}) // End E014


// +------------------------------------+
// |               PPU_26               |
// +------------------------------------+
Name(E015, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x25},
        Package (2) {"mipi-sdca-entity-label", "PPU 26"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", CTL_PPU_POSTURENUMBER},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-1", "E013"},
        Package (2) {"mipi-sdca-control-0x10-subproperties", "C024"},
    }
}) // End E015


// +------------------------------------+
// |               MFPU_26              |
// +------------------------------------+
Name(E016, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x22},
        Package (2) {"mipi-sdca-entity-label", "MFPU 26"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", (CTL_MFPU_CLUSTER_INDEX | CTL_MFPU_BYPASS)},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-1", "E015"},
        Package (2) {"mipi-sdca-control-0x1-subproperties", "C501"},    // Bypass
        CLUSTER_MFPU_26,
    }
}) // End E016


// +------------------------------------+
// |                FU_26               |
// +------------------------------------+
Name(E017, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) {"mipi-sdca-entity-type", 0x07},
        Package(2) {"mipi-sdca-entity-label", "FU 26"},
        Package(2) {"mipi-sdca-control-list", CTL_FU_MUTE},
        Package(2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) {"mipi-sdca-input-pin-1", "E016"},
        Package(2) {"mipi-sdca-control-0x1-subproperties", "C02B"},
    }
}) // End E017

Name(C02A, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {  // Mute, Platform, RW/Dual
        Package(2) {"mipi-sdca-control-access-layer", CAL_PLATFORM},
        Package(2) {"mipi-sdca-control-access-mode", CAM_DUAL},
        Package(2) {"mipi-sdca-control-cn-list", 0x2},  // Control Numbers = {1}
#ifdef FU_23_FIXED_RENDER_MUTE
        Package(2) {"mipi-sdca-control-fixed-value", 0}, // Unmuted
#endif
    }
}) // End C02A

Name(C02B, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Mute, Platform, RW/Dual
        Package(2) {"mipi-sdca-control-access-layer", CAL_PLATFORM},
        Package(2) {"mipi-sdca-control-access-mode", CAM_DUAL},
        Package(2) {"mipi-sdca-control-cn-list", 0x2},  // Control Numbers = {1}
#ifdef FU_26_FIXED_RENDER_MUTE
        Package(2) {"mipi-sdca-control-fixed-value", 0}, // Unmuted
#endif
    }
}) // End C02B
