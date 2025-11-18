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


// v1.24 - Add support for SDCA class FDL.
// v1.23 - SDCA 1.0 conformance DisCo fixes.
// v1.22 - Removed EXCLUDE_FUN_STS conditional compilation.
// v1.21 - Added support to allow fixed unmute value on FU_23 and FU_26
// v1.20 - Disabled Jamerson AMP FDL and Protection Mode interrupts handling at the class level.
// v1.19 - Removed FU_23 and FU_26 volume controls. Fixed Mute access layer as Platform.
// v1.18 - Posture support.
// v1.17 - Added FU21 fixed mute support.
// v1.16 - Moved downgrading Function SDCA Version to common.h file.
// v1.15 - Redesigned preventing XU from using DisCo defined controls (Volume, Mute, Function_status).
// v1.14 - Replaced CS35L56_FU_21_VOL_MAX_Q7_8 with CS35L56_FU_21_VOL_FIXED_Q7_8.
// v1.13 - Set Function_SDCA_Version to 1.0.
//         Defined missing required E0 controls.
// v1.12 - Changed FU_21 to be opt-out rather than opt-in.
// v1.11 - Included FU_21 and a fixed render volume if required.
// v1.10 - Added support for Function_Status bit handling by the class driver.
// v1.9  -  Re-introduce Cluster and Usage support
// v1.8  - Removed Sense Capture stream (OT24) from topology.
// v1.7  - Remove Cluster and Usage until they are supported by Microsoft
// v1.6  -  Defined macros specifying volume range, volume default value, and volume fixed value.
// v1.5  - Removed XU22 FDL UMP controls.
//       - Removed reserved control numbers from the control list in XU22.
// v1.4  - Fixed FU 21 volume range.
// v1.3  - Updated to class driver Drop 1 from Workshop 4.0
// v1.2  - Removed stream start, stop buffer contents
// v1.1  - Removed FU 21 [Entity ID 0x5] (MFPU 21 [Entity ID 0x1A] is now connected to PPU 21 [Entity ID 0x4])
//       - Added FU 23 [Entity ID C] ( OT 23 [Entity ID 0xE] is now connected to FU 23 [Entity ID 0xC] which is connected to UDMPU 23 [Entity ID 0xB])
//       - Added FU 26 [Entity ID 0x19] ( MU 26 [Entity ID 0xA] Pin 2 is now connected to FU 26 [Entity ID 0x19] which is connected to MFPU 26 [Entity ID 0x18])
//       - OT 24 [Entity ID 0x14 ] is now connected to DP 3
//       - IT 26 [Entity ID 0x16 ] is now connected to DP 2
//
// Conditionals:
//   EXCLUDE_FU_21_VOLUME_CONTROL - Excludes FU_21 from the topology.
//   FU_21_FIXED_RENDER_VOLUME    - When set, and EXCLUDE_FU_21_VOLUME_CONTROL is not set, it fixes the FU 21 volume to CS35L56_FU_21_VOL_FIXED_Q7_8
//                                  which will force the drive stack to include a volume control APO.
//   FU_21_FIXED_RENDER_MUTE      - When set, and EXCLUDE_FU_21_VOLUME_CONTROL is not set, it fixes the
//                                  FU 21 mute to unmuted state.
//   FU_23_FIXED_RENDER_MUTE      - When set, it fixes the FU 23 mute control to unmuted state
//   FU_26_FIXED_RENDER_MUTE      - When set, it fixes the FU 26 mute control to unmuted state
//
// +-------+  +--------+  +-------+  +---------+
// | IT_21 +->| PPU_21 +->| FU_21 +->| MFPU_21 +--+                                                                       +--------+
// +-------+  +--------+  +-------+  +---------+  |                                                                       | PDE_23 |
//     ^                                          |  +-------+    +---------+    +-------+                  +-------+   +-+-----+  |
//     |                                          +->|       +--->|         +--->|       |    +----------+  |       +-->| OT_23 +--+
// +---+---+                                         | XU_22 |    | SAPU_29 |    | MU_26 +--->| UDMPU_23 +->| FU_23 |   +-------+
// | CS_21 |                       +---------------->|       | +->|         | +->|       |    +----------+  |       +-+
// +-------+                       |                 +-------+ |  +---------+ |  +-------+                  +-------+ |   +----------+  +-------+
//                                 |                           |              |                                       +-->| UDMPU_25 +->| OT_25 |
//                                 +---------------------------+              |                                           +----------+  +-------+
//                                 |                                          |                                           Reference stream path
//                       +-------+ |  +-------+   +-------+                   |
//                       | IT_29 +-+->| XU_24 +-->| OT_24 |                   |
//                       +-------+    +-------+   +-------+                   |
//                               Speaker sense path   ^                       |
//                                                    |                       |
//                                                +---+---+                   |
//                                                | CS_24 |                   |
//                                                +-------+                   |
//                                                                            |
//                                                                            |
//                         +-------+  +--------+  +----------+  +-------+     |
//                         | IT_26 +->| PPU_26 +->|  MFPU_26 +->| FU_26 +-----+
//                         +-------+  +--------+  +----------+  +-------+
//                             ^             Ultrasonic render path
//                             |
//                         +---+---+
//                         | CS_26 |
//                         +-------+
//


/*
    Handy Entity ID map
    -------------------
    CS_21         2
    IT_21         3
    PPU_21        4
    FU_21         5
    IT_29         6
    XU_22         8
    SAPU_29       9
    MU_26         10 (0xA)
    UDMPU_23      11 (0xB)
    FU_23         12 (0xC)
    PDE_23        13 (0xD)
    OT_23         14 (0xE)
    UDMPU_25      15 (0xF)      // only used when reference stream is enabled
    OT_25         16 (0x10)     // only used when reference stream is enabled
    CS_24         17 (0x11)     // only used when speaker sense is enabled
    XU_24         19 (0x13)     // only used when speaker sense is enabled
    OT_24         20 (0x14)     // only used when ultrasonic stream is enabled
    CS_26         21 (0x15)     // only used when ultrasonic stream is enabled
    IT_26         22 (0x16)     // only used when ultrasonic stream is enabled
    PPU_26        23 (0x17)     // only used when ultrasonic stream is enabled
    MFPU_26       24 (0x18)     // only used when ultrasonic stream is enabled
    FU_26         25 (0x19)     // currently not used
    MFPU_21       26 (0x1A)
*/

#include "SndwDevTopologySt05Ssdt/version.h"

#ifdef EXCLUDE_FU_21_VOLUME_CONTROL
# define FEATURE_CS35L56_AMP_NO_VOL_MUTE_R_COND  0x00000000
#else
// Class driver creates AMP mute and volume elements.
// XU opts out from creating AMP mute and volume elements.
# define FEATURE_CS35L56_AMP_NO_VOL_MUTE_R_COND  0x00000020
#endif

// NOTE: Volume ranges MUST be identical for all FUs.
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
// Step = 0x00000008 (0.03125 dB per step) in Q7.8 format
# define CS35L56_FU_21_VOL_STEP 0x08, 0x00, 0x00, 0x00
#endif

#ifndef CS35L56_FU_21_VOL_DEFAULT
// Default = 0x0 (0 dB) in Q7.8 format
# define CS35L56_FU_21_VOL_DEFAULT 0x0
#endif


#define JAMERSON_CLUSTER
#define JAMERSON_USAGE
#include <SndwDevTopologySt05Ssdt/SmartAmp-Clusters.asl>
#include <SndwDevTopologySt05Ssdt/SmartAmp-Usage.asl>
#undef JAMERSON_USAGE
#undef JAMERSON_CLUSTER


#ifdef JAMERSON_DISABLE_REF_STREAM
# define _SMART_AMP_REFERENCE_STREAM_FEATURE_BITS   0
#else
# define _SMART_AMP_REFERENCE_STREAM_FEATURE_BITS   (1 << 5) | (1 << 6)
#endif

#ifdef DISABLE_US_STREAM
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
        //
        // NOTE:
        //  The MSFT Class driver does not parse mipi-sdca-function-topology-features.
        //  MSFT recommends not including it in DisCo until the Class driver has added
        //  support for it.
        //
        //Package(2) {"mipi-sdca-function-topology-features",
        //    _SMART_AMP_REFERENCE_STREAM_FEATURE_BITS |
        //    _SMART_AMP_US_STREAM_FEATURE_BITS
        //},
        Package(2) {"mipi-sdca-control-list",  (CTL_E0_FUNCTION_VERSION | CTL_E0_FUNCTION_ID | CTL_E0_FUNCTION_MANUFACTURER_ID |
                                                CTL_E0_FUNCTION_TYPE | CTL_E0_FUNCTION_SDCA_VERSION | CTL_E0_FUNCTION_STATUS |
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
#ifndef DISABLE_US_STREAM
            JAMERSON_US_STREAM_ENTITY_ID_LIST
#endif
            // JAMERSON_SPEAKER_SENSE_ENTITY_ID_LIST
            },
        },
        CLUSTER_ID_LIST_SMART_AMP,
#ifdef  SDCA_CLASS_FDL_CS35L56
        Package () { "mipi-sdca-file-set-id-list", Package () {1}},
        Package () { "mipi-sdca-file-set-id-0x1",
      Package () {0x01FA, 0x35560001, 0x03800000,    // PM
                                    0x01FA, 0x35560002, 0x02000000,    // XM
                                    0x01FA, 0x35560003, 0x02c004ac}},  // YM
#endif //  SDCA_CLASS_FDL_CS35L56
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) {"mipi-sdca-control-0x4-subproperties", "C004"},
        Package(2) {"mipi-sdca-control-0x5-subproperties", "C005"},
        Package(2) {"mipi-sdca-control-0x6-subproperties", "C006"},
        Package(2) {"mipi-sdca-control-0x7-subproperties", "C007"},
        Package(2) {"mipi-sdca-control-0x8-subproperties", "C008"},
        Package(2) { "mipi-sdca-control-0x10-subproperties", "C010"},
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
}) // End _DSD

#undef _SMART_AMP_REFERENCE_STREAM_FEATURE_BITS
#undef _SMART_AMP_US_STREAM_FEATURE_BITS

Name(EXT0, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
#ifdef AMP_VARIABLE_SPEAKER_SELECT
        // NOTE: 01fa-spk-id-val must ALWAYS be the first package inside the
        // inner package of EXT0. It will be updated at ACPI initialization time
        // by the _INI method in the SDCA function ACPI driver node for this
        // device.
        Package(2) {"01fa-spk-id-val", 0}, // value to be set by _INI function
#endif
        Package(2) { "mipi-sdca-function-expansion-subsystem-id", 0 },  // MIPI required, but not used by MSFT
        Package(2) { "01fa-chip-id", 0x3556 },
        Package(2) { "01fa-release-version", RELEASE_VERSION },
        Package(2) { "01fa-ssid-ex", 0x7 },
        Package(2) { "01fa-xu-features", (FEATURE_ENABLE_HWKWS | FEATURE_ENABLE_WT | FEATURE_ENABLE_KNCK | FEATURE_NO_FUN_STS |
                                          FEATURE_CS35L56_AMP_NO_VOL_MUTE_R_COND | FEATURE_DISABLE_FDL_CS35L56)},
    }
}) // End EXT0

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
}) // End C004

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
}) // End C005

Name(C006, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Function_Manufacturer_Id = 0x01FA
        Package(2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package(2) {"mipi-sdca-control-dc-value", 0x01FA},
    }
}) // End C006

Name(C007, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, DC, Function_Id = 0x3556
        Package(2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package(2) {"mipi-sdca-control-dc-value", 0x3556},
    }
}) // End C007

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
}) // End C008

Name(C010, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Class, RW1C, Function_Status
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_RW1C},
        Package(2) { "mipi-sdca-control-interrupt-position", JAMERSON_B_SDCA_FUNC_STATUS_INT},    // CS35L56 rev B0.
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
        Package(2) {"mipi-sdca-control-0x11-subproperties", "C021"}, // DataPort_Selector
        Package(2) {"mipi-sdca-terminal-clock-connection", "E002"},
    },
#if CTL_E0_FUNCTION_SDCA_VERSION_VAL < 0x10
//
// NOTE:
//  "mipi-sdca-terminal-dp-numbers" has been removed from DisCo for SoundWire 2.1 spec so they should no longer be present in the ASL file.
//  "mipi-sdca-terminal-dp-numbers" has been deprecated, and 1.0 Conformant devices will report DP numbers as a range for DataPort_Selector control.
//
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-terminal-dp-numbers", "BUF3"},
    }
#endif
}) // End E003

#if CTL_E0_FUNCTION_SDCA_VERSION_VAL < 0x10
Name(BUF3, Buffer()
{
    0x1, JAMERSON_SPEAKER_RENDER_DATA_PORT, // DP_Index: 0x1 DP_Num: 0x1
}) // End BUF3
#endif

Name(C021, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    { // DataPortIndex, Class, DC
        Package(2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package(2) {"mipi-sdca-control-dc-value", 1},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-control-range", "DPM0"},
    }
}) // End C021

// DataPortMap
Name(DPM0, Buffer() {
    0x10, 0x00,                 // Range type 0x0010
    0x04, 0x00,                 // NumRows = 4
    // DP_Index_A
    0xFF, 0x00, 0x00, 0x00,     // 0: not used
    0x01, 0x00, 0x00, 0x00,     // 1: DP1
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
}) // End DPM0


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
}) // End E002

Name(C022, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    { // Clock_Valid, Class, RO
        Package(2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) {"mipi-sdca-control-access-mode", CAM_RO},
    }
}) // End C022

Name(C023, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    { // Sample_Freq_Index, Class, R/W
        Package(2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) {"mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package(2) {"mipi-sdca-control-default-value", 0x3},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "BUF5"},
    }
}) // End C023

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
}) // End BUF5


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
}) // End E004

Name(C024, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DUAL},
        Package (2) {"mipi-sdca-control-default-value", 0}, // Posture Number = 0, no rotation
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "PMAP"},
    }
}) // End C024


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
}) // End E005

Name(C025, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {  // Mute, User, RW/Dual
        Package(2) {"mipi-sdca-control-access-layer", CAL_USER},
        Package(2) {"mipi-sdca-control-access-mode", CAM_DUAL},
        Package(2) {"mipi-sdca-control-cn-list", 0x2},  // Control Numbers = {1}
#ifdef FU_21_FIXED_RENDER_MUTE
        Package(2) {"mipi-sdca-control-fixed-value", 0},   // unmuted
#endif
    }
}) // End C025

Name(C026, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {  // Volume, User, RW/Dual
        Package(2) {"mipi-sdca-control-access-layer", CAL_USER},
        Package(2) {"mipi-sdca-control-access-mode", CAM_DUAL},
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
}) // End C026

Name(BUF4, Buffer()
{
    0x03, 0x00,  // Range type 0x0003 (Triples)
    0x01, 0x00,  // Count of ranges = 0x1
    CS35L56_FU_21_VOL_MIN,
    CS35L56_FU_21_VOL_MAX,
    CS35L56_FU_21_VOL_STEP,
}) // End BUF4
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
}) // End E01A

Name(C027, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 1},
    }
}) // End C027


// +------------------------------------+
// |                XU_22               |
// +------------------------------------+
Name(E008, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0xa},
        Package (2) {"mipi-sdca-entity-label", "XU 22"},
        Package (2) {"mipi-sdca-input-pin-list", 0x6},
#ifdef  SDCA_CLASS_FDL_CS35L56
        Package (2) { "mipi-sdca-control-list", CTL_XU_VERSION | CTL_XU_ID | CTL_XU_BYPASS | CTL_XU_FDL_CURRENTOWNER | CTL_XU_FDL_MESSAGEOFFSET | CTL_XU_FDL_MESSAGELENGTH | CTL_XU_FDL_STATUS | CTL_XU_FDL_SET_INDEX | CTL_XU_FDL_HOST_REQUEST},
        Package (2) { "mipi-sdca-RxUMP-ownership-transition-max-delay", 10000},
#else
        Package (2) { "mipi-sdca-control-list", CTL_XU_VERSION | CTL_XU_ID | CTL_XU_BYPASS},
#endif //  SDCA_CLASS_FDL_CS35L56
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
#ifdef  SDCA_CLASS_FDL_CS35L56
        Package (2) {"mipi-sdca-control-0x10-subproperties", "C033"},
        Package (2) {"mipi-sdca-control-0x12-subproperties", "C034"},
        Package (2) {"mipi-sdca-control-0x13-subproperties", "C035"},
        Package (2) {"mipi-sdca-control-0x14-subproperties", "C036"},
        Package (2) {"mipi-sdca-control-0x15-subproperties", "C037"},
#endif //  SDCA_CLASS_FDL_CS35L56
        Package (2) {"mipi-sdca-control-0x16-subproperties", "C038"},
    }
}) // End E008

Name(C028, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package (2) {"mipi-sdca-control-default-value", 0},
    }
}) // End C028

Name(C029, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 2}, // XU ID
    }
}) // End C029

Name(C030, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 1}, // XU Version
    }
}) // End C030

Name(C031, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0},
    }
}) // End C031

Name(C032, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0},
    }
}) // End C032
#ifdef  SDCA_CLASS_FDL_CS35L56
Name(C033, Package() {  // XU_FDL_Current_Owner
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-interrupt-position", JAMERSON_B_SDCA_FDL_CURRENT_OWNER_INT},    // CS35L56 rev B0.
        Package (2) {"mipi-sdca-control-access-mode", CAM_RW1S},
        Package (2) {"mipi-sdca-control-default-value", 1},
    }
}) // End C033

Name(C034, Package() {  // XU_FDL_MESSAGEOFFSET
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-control-range","FMO2"}
    },
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package (2) {"mipi-sdca-control-default-value", 0},
    }
}) //End C034

Name(FMO2, Buffer() {
    0x03, 0x00, // Range type 0x0003 (Triples)
    0x01, 0x00, // Count of ranges = 0x1
    0x00, 0x80, 0x00, 0x00, // UMP Buffer Start Address: 0x8000
    0x00, 0x00, 0x00, 0x04, // UMP Buffer Length: 64mb
    0x00, 0x00, 0x00, 0x00, // UMP Mode: 0 = Direct, 1 = Indirect
})

Name(C035, Package() {  // XU_FDL_MessageLength
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package (2) {"mipi-sdca-control-default-value", 0},
    }
}) // End C035

Name(C036, Package() {  // XU_FDL_Status
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package (2) {"mipi-sdca-control-default-value", 0},
    }
}) // End C036

Name(C037, Package() {  // XU_FDL_SET_INDEX
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-control-range","FSI2"}
    },
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_RO},
    }
}) // End C037

Name(FSI2, Buffer()
{
    0x02, 0x00, // Range type 0x0002 (Doubles)
    0x01, 0x00, // Count of ranges = 0x1
    0x00, 0x00, 0x00, 0x00, // First Set Index: 0x0
    0x01, 0x00, 0x00, 0x00, // First File Set ID: 0x1
})
#endif // End  SDCA_CLASS_FDL_CS35L56

Name(C038, Package() {  // XU_Host_Request
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package (2) {"mipi-sdca-control-default-value", 0},
    }
}) // End C038

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
}) // End E009

Name(C039, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_RO},
        //Package (2) {"mipi-sdca-control-interrupt-position", JAMERSON_B_SDCA_PROTECTION_MODE_INT}, // CS35L56 rev B0.
    }
}) // End C039

Name(C040, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_RO},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-control-range", "BMT1"},
    }
}) // End C040

// Protection_Status BitIndex Mapping Table (BMT)
Name(BMT1, Buffer() {
    0x03, 0x00,             // Range type 0x0003
    0x06, 0x00,             // NumRows = 6
    // Bit 0: Firmware and/or parameters are missing
    0x00, 0x00, 0x00, 0x00, // Bit Number = 0
    0x00, 0x00, 0x00, 0x00, // Behavior Set ID = 0 -> CBN = 1010
    0x00, 0x00, 0x00, 0x00, // String Number = 0 -> "Firmware Missing"
    // Bit 1: Reserved
    // Bit 2: Speaker protection internal fault
    0x02, 0x00, 0x00, 0x00, // Bit Number = 2
    0x03, 0x00, 0x00, 0x00, // Behavior Set ID = 3 -> CBN = 1100
    0x03, 0x00, 0x00, 0x00, // String Number = 3 -> "Catastrophic Fault"
    // Bit 3: Power Supply Fault
    0x03, 0x00, 0x00, 0x00, // Bit Number = 3
    0x03, 0x00, 0x00, 0x00, // Behavior Set ID = 3 -> CBN = 1100
    0x03, 0x00, 0x00, 0x00, // String Number = 3 -> "Catastrophic Fault"
    // Bit 4: Hardware Internal Fault
    0x04, 0x00, 0x00, 0x00, // Bit Number = 4
    0x03, 0x00, 0x00, 0x00, // Behavior Set ID = 3 -> CBN = 1100
    0x03, 0x00, 0x00, 0x00, // String Number = 3 -> "Catastrophic Fault"
    // Bit 5: Transducer Fault
    0x05, 0x00, 0x00, 0x00, // Bit Number = 5
    0x04, 0x00, 0x00, 0x00, // Behavior Set ID = 4 -> CBN = 1070
    0x04, 0x00, 0x00, 0x00, // String Number = 4 -> "External Condition"
    // Bit 6: Parameter CRC Fault
    0x06, 0x00, 0x00, 0x00, // Bit Number = 6
    0x03, 0x00, 0x00, 0x00, // Behavior Set ID = 3 -> CBN = 1100
    0x03, 0x00, 0x00, 0x00, // String Number = 3 -> "Catastrophic Fault"
    // Bit 7: Reserved
}) // End BMT1

//
// NOTE:
//  The MSFT class driver does not parse mipi-sdca-behavior-set-id-list,
//  mipi-sdca-behavior-set-id-0x<n>-subproperties, and mipi-sdca-string-table.
//  MSFT recommends not including them in DisCo until the Class driver has added
//  support for them.
//


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
        Package (2) {"mipi-sdca-control-0x11-subproperties", "C042"}, // DataPort_Selector
        Package (2) {"mipi-sdca-terminal-clock-connection", "E015"},
    },
#if CTL_E0_FUNCTION_SDCA_VERSION_VAL < 0x10
//
// NOTE:
//  "mipi-sdca-terminal-dp-numbers" has been removed from DisCo for SoundWire 2.1 spec so they should no longer be present in the ASL file.
//  "mipi-sdca-terminal-dp-numbers" has been deprecated, and 1.0 Conformant devices will report DP numbers as a range for DataPort_Selector control.
//
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package() {
        Package (2) {"mipi-sdca-terminal-dp-numbers", "BUF7"},
    }
#endif
}) // End E016

#if CTL_E0_FUNCTION_SDCA_VERSION_VAL < 0x10
Name(BUF7, Buffer() {
    0x2, JAMERSON_US_RENDER_DATA_PORT, // DP_Index, DP_Num
}) //End BUF7
#endif

Name(C042, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 2},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-control-range", "DPM1"},
    }
}) // End C042

// DataPortMap1
Name(DPM1, Buffer() {
    0x10, 0x00,                 // Range type 0x0010
    0x04, 0x00,                 // NumRows = 4
    // DP_Index_A
    0xFF, 0x00, 0x00, 0x00,     // 0: not used
    0xFF, 0x00, 0x00, 0x00,     // 1: not used
    0x02, 0x00, 0x00, 0x00,     // 2: DP2
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
}) // End DPM1


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
}) // End E015

Name(C043, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) {"mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package (2) {"mipi-sdca-control-default-value", 0x3},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "BUF9"},
    }
}) // End C043

Name(BUF9, Buffer()
{
    0x02, 0x00,  // Range type 0x0002 (Pairs)
    0x01, 0x00,  // Count of ranges = 0x1
    0x03, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00, 0x00, // 0x00000003, 0x0000BB80 (48000)
}) // End BUF9


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
}) // End E017

Name(C044, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DUAL},
        Package (2) {"mipi-sdca-control-default-value", 0}, // Posture Number = 0, no rotation
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "PMAP"},
    }
}) // End C044


// +------------------------------------+
// |               MFPU_26              |
// +------------------------------------+
Name(E018, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-entity-type", 0x22},
        Package (2) {"mipi-sdca-entity-label", "MFPU 26"},
        Package (2) {"mipi-sdca-input-pin-list", 0x2},
        Package (2) {"mipi-sdca-control-list", (CTL_MFPU_CLUSTER_INDEX | CTL_MFPU_BYPASS)},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        Package (2) {"mipi-sdca-input-pin-1", "E017"},
        Package (2) {"mipi-sdca-control-0x1-subproperties", "C045"},    // Bypass
        CLUSTER_MFPU_26,
    }
}) // End E018

Name(C045, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        // Bypass: Class, DC = 1
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0x1},    // NULL MFPU
    }
}) // End C045


// +------------------------------------+
// |                MU_26               |
// +------------------------------------+
Name(E00A, Package() {
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
        Package (2) {"mipi-sdca-input-pin-2", "E019"},
#endif
        Package (2) {"mipi-sdca-control-0x1-subproperties", "C049"},
    }
}) // End E00A

Name(C049, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_PLATFORM},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0x1},
    }
}) // End C049


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
}) // End E00B


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
        // SDCA 1.0 spec, Table 80 Enumeration Values for DisCo Property: Terminal Connector Type
        Package(2) {"mipi-sdca-terminal-connector-type", 0xFF},    // Other connector type
        Package(2) {"mipi-sdca-terminal-transducer-count", 1},
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
}) // End E00E


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
}) // End E00D

Name(RPS0, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Requested_PS, Class, R/W
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS },
        Package(2) { "mipi-sdca-control-access-mode", CAM_READ_WRITE},
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
         Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS },
         Package(2) {"mipi-sdca-control-access-mode", CAM_RO},
    }
}) // End APS0


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
}) // End E00F

Name(C053, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_PLATFORM},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0},
    }
}) // End C053

Name(C054, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_PLATFORM},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0},
    }
}) // End C054

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
        Package (2) {"mipi-sdca-control-0x11-subproperties", "C056"}, // DataPort_Selector
        Package (2) {"mipi-sdca-control-0x12-subproperties", "C057"},
    },
#if CTL_E0_FUNCTION_SDCA_VERSION_VAL < 0x10
//
// NOTE:
//  "mipi-sdca-terminal-dp-numbers" has been removed from DisCo for SoundWire 2.1 spec so they should no longer be present in the ASL file.
//  "mipi-sdca-terminal-dp-numbers" has been deprecated, and 1.0 Conformant devices will report DP numbers as a range for DataPort_Selector control.
//
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-terminal-dp-numbers", "BUF6"},
    }
#endif
}) // End E010

#if CTL_E0_FUNCTION_SDCA_VERSION_VAL < 0x10
Name(BUF6, Buffer() {
    0x4, JAMERSON_REF_STREAM_DATA_PORT, // DP_Index, DP_Num
}) //End BUF6
#endif

Name(C056, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0x4},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-control-range", "DPM2"},
    }
}) // End C056

// DataPortMap1
Name(DPM2, Buffer() {
    0x10, 0x00,                 // Range type 0x0010
    0x04, 0x00,                 // NumRows = 4
    // DP_Index_A
    0xFF, 0x00, 0x00, 0x00,     // 0: not used
    0xFF, 0x00, 0x00, 0x00,     // 1: not used
    0xFF, 0x00, 0x00, 0x00,     // 2: not used
    0xFF, 0x00, 0x00, 0x00,     // 3: not used
    0x04, 0x00, 0x00, 0x00,     // 4: DP4
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
}) // End DPM2

Name(C057, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0x0},
    }
}) // End C057


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
        // SDCA 1.0 spec, Table 80 Enumeration Values for DisCo Property: Terminal Connector Type
        Package (2) {"mipi-sdca-terminal-connector-type", 0xFF},    // Other connector type
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
        USAGE_IT_29,
        CLUSTER_IT_29
    },
}) // End E006


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
}) // End E013

Name(C060, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package (2) {"mipi-sdca-control-default-value", 0},
    }
}) // End C060


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
        Package (2) {"mipi-sdca-control-0x11-subproperties", "C062"}, // DataPort_Selector
        Package (2) {"mipi-sdca-control-0x12-subproperties", "C063"},
    },
#if CTL_E0_FUNCTION_SDCA_VERSION_VAL < 0x10
//
// NOTE:
//  "mipi-sdca-terminal-dp-numbers" has been removed from DisCo for SoundWire 2.1 spec so they should no longer be present in the ASL file.
//  "mipi-sdca-terminal-dp-numbers" has been deprecated, and 1.0 Conformant devices will report DP numbers as a range for DataPort_Selector control.
//
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-terminal-dp-numbers", "BUFA"},
    }
#endif
}) // End E014

#if CTL_E0_FUNCTION_SDCA_VERSION_VAL < 0x10
Name(BUFA, Buffer() {
    0x3, 0x3, // DP_Index, DP_Num
}) // End BUFA
#endif

Name(C062, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0x3},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-control-range", "DPM3"},
    }
}) // End C062

// DataPortMap1
Name(DPM3, Buffer() {
    0x10, 0x00,                 // Range type 0x0010
    0x04, 0x00,                 // NumRows = 4
    // DP_Index_A
    0xFF, 0x00, 0x00, 0x00,     // 0: not used
    0xFF, 0x00, 0x00, 0x00,     // 1: not used
    0xFF, 0x00, 0x00, 0x00,     // 2: not used
    0x03, 0x00, 0x00, 0x00,     // 3: DP3
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

Name(C063, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_DC},
        Package (2) {"mipi-sdca-control-dc-value", 0x0},
    }
}) // End C063


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
}) // End E011

Name(C064, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    { // Sample_Freq_Index, Class, R/W
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) {"mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package (2) {"mipi-sdca-control-default-value", 0x3},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "BUF5"},
    }
}) // End C064




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
        Package(2) {"mipi-sdca-control-list", CTL_FU_MUTE},
        Package(2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) {"mipi-sdca-input-pin-1", "E00B"},
        Package(2) {"mipi-sdca-control-0x1-subproperties", "C02A"},
    }
}) // End E00C

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
        Package(2) {"mipi-sdca-control-list", CTL_FU_MUTE},
        Package(2) {"mipi-sdca-input-pin-list", 0x2}, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) {"mipi-sdca-input-pin-1", "E018"},
        Package(2) {"mipi-sdca-control-0x1-subproperties", "C02B"},
    }
}) // End E019
