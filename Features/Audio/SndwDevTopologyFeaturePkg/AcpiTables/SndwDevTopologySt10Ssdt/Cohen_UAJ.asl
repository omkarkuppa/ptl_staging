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

#ifdef EXCLUDE_FU_36_VOLUME_CONTROL
# define FEATURE_CS42L43_UAJ_NO_VOL_MUTE_C_COND  0x00000000
#else // !EXCLUDE_FU_36_VOLUME_CONTROL
// Class driver creates UAJ MIC mute and volume elements.
// XU opts out from creating UAJ MIC mute and volume elements.
# define FEATURE_CS42L43_UAJ_NO_VOL_MUTE_C_COND  0x00000010
#endif // EXCLUDE_FU_36_VOLUME_CONTROL

#ifdef EXCLUDE_FU_41_VOLUME_CONTROL
# define FEATURE_CS42L43_UAJ_NO_VOL_MUTE_R_COND  0x00000000
#else
// Class driver creates UAJ SPK mute and volume elements.
// XU opts out from creating UAJ SPK mute and volume elements.
# define FEATURE_CS42L43_UAJ_NO_VOL_MUTE_R_COND  0x00000020
#endif

#ifndef CS42L43_FU_36_VOL_MIN
// Min = 0x0000CE00 (-64.0 dB) in Q7.8 format
# define CS42L43_FU_36_VOL_MIN 0x00, 0xC0, 0x00, 0x00
#endif

#ifndef CS42L43_FU_36_VOL_MAX
// Max = 0x00001F80 (+31.5 dB) in Q7.8 format
# define CS42L43_FU_36_VOL_MAX 0x80, 0x1F, 0x00, 0x00
#endif

#ifndef CS42L43_FU_36_VOL_STEP
// Step = 0x00000008 (1/32 dB per step) in Q7.8 format
# define CS42L43_FU_36_VOL_STEP 0x08, 0x00, 0x00, 0x00
#endif

#ifndef CS42L43_FU_36_VOL_DEFAULT
// FU36 Default = 0x0 (0 dB) in Q7.8 format
# define CS42L43_FU_36_VOL_DEFAULT 0x0
#endif

#ifndef CS42L43_FU_36_VOL_FIXED_Q7_8
// FU36 Fixed = 0x0 (0 dB) in Q7.8 format
# define CS42L43_FU_36_VOL_FIXED_Q7_8 0x0
#endif

#ifndef CS42L43_FU_41_VOL_MIN
// Min = 0x00008E00 (-114 dB) in Q7.8 format
# define CS42L43_FU_41_VOL_MIN 0x00, 0x8E, 0x00, 0x00
#endif

#ifndef CS42L43_FU_41_VOL_MAX
// Max = 0x00000000 (0.0 dB) in Q7.8 format
# define CS42L43_FU_41_VOL_MAX 0x00, 0x00, 0x00, 0x00
#endif

#ifndef CS42L43_FU_41_VOL_FIXED_Q7_8
// Fixed = 0x00000000 (0.0 dB) in Q7.8 format
# define CS42L43_FU_41_VOL_FIXED_Q7_8 0x0
#endif

#ifndef CS42L43_FU_41_VOL_STEP
// Step = 0x00000008 (1/32 dB per step) in Q7.8 format
# define CS42L43_FU_41_VOL_STEP 0x08, 0x00, 0x00, 0x00
#endif

#ifndef CS42L43_FU_41_VOL_DEFAULT
// Default = 0x0 (0 dB) in Q7.8 format
# define CS42L43_FU_41_VOL_DEFAULT 0x0
#endif

#ifndef CS42L43_IT_31_MIC_BIAS_DEFAULT
// MIC-on-Jack default MIC Bias = 2.75V
// HS MIC default MIC Bias = 2.75V
# define CS42L43_IT_31_MIC_BIAS_DEFAULT 0x05
#endif

// Cohen GE35 Load Detection
#define CS42L43_GE35_CTL_LOAD_DET   (1 << 0x31)

// Cohen GE35 ASP Output
#define CS42L43_GE35_CTL_ASP_OUTPUT (1 << 0x32)

#include <UAJ-Clusters.asl>
#include <UAJ-Usage.asl>

#ifdef EXCLUDE_FU_36_VOLUME_CONTROL
// Remove FU_36
# define COHEN_UAJ_CAPTURE_STREAM_ENTITY_ID_LIST    0x2, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0x11, 0x1F, 0x18, 0x1A, 0x1B
#else   // !EXCLUDE_FU_36_VOLUME_CONTROL
// Define FU_36
# define COHEN_UAJ_CAPTURE_STREAM_ENTITY_ID_LIST    0x2, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0x11, 0x1F, 0x18, 0x19, 0x1A, 0x1B
#endif  // EXCLUDE_FU_36_VOLUME_CONTROL

#ifdef EXCLUDE_FU_41_VOLUME_CONTROL
// Remove FU_41
# define COHEN_UAJ_RENDER_STREAM_ENTITY_ID_LIST     0x5, 0x4, 0x6, 0xE, 0xF, 0x1, 0x3, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17
#else   // !EXCLUDE_FU_41_VOLUME_CONTROL
// Define FU_41
# define COHEN_UAJ_RENDER_STREAM_ENTITY_ID_LIST     0x5, 0x4, 0x6, 0xE, 0xF, 0x1, 0x10, 0x3, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17
#endif  // EXCLUDE_FU_41_VOLUME_CONTROL

// COHEN_BRIDGE_SIDECAR_REGISTER_ACCESS_ADDRESS_SELECTOR
#define CS42L43_UAJ_E0_CTL_SIDECAR_REG_ADDR (1 << 0x30)

// COHEN_BRIDGE_SIDECAR_REGISTER_ACCESS_VALUE_SELECTOR
#define CS42L43_UAJ_E0_CTL_SIDECAR_REG_VAL  (1 << 0x31)

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
        //Package(2) { "mipi-sdca-function-topology-features", 0x0},
        Package(2) { "mipi-sdca-control-list", (CTL_E0_FUNCTION_VERSION | CTL_E0_FUNCTION_ID | CTL_E0_FUNCTION_MANUFACTURER_ID |
                                                CTL_E0_FUNCTION_TYPE | CTL_E0_FUNCTION_SDCA_VERSION | CTL_E0_FUNCTION_STATUS |
                                                CTL_E0_DEVICE_MANUFACTURER_ID | CTL_E0_DEVICE_PART_ID | CTL_E0_DEVICE_VERSION | CTL_E0_DEVICE_SDCA_VERSION |
                                                CS42L43_UAJ_E0_CTL_SIDECAR_REG_ADDR | CS42L43_UAJ_E0_CTL_SIDECAR_REG_VAL
                                                )},
        Package(2) { "mipi-sdca-entity-id-list", Package() { COHEN_UAJ_CAPTURE_STREAM_ENTITY_ID_LIST,
                                                             COHEN_UAJ_RENDER_STREAM_ENTITY_ID_LIST} },

        Package(2) { "mipi-sdca-function-busy-max-delay", 5000},
        // Clusters
        CLUSTER_ID_LIST_UAJ,
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
        Package(2) { "mipi-sdca-control-0x10-subproperties", "C010"},
        Package(2) { "mipi-sdca-control-0x2C-subproperties", "C02C"},
        Package(2) { "mipi-sdca-control-0x2D-subproperties", "C02D"},
        Package(2) { "mipi-sdca-control-0x2E-subproperties", "C02E"},
        Package(2) { "mipi-sdca-control-0x2F-subproperties", "C02F"},
        Package(2) { "mipi-sdca-control-0x30-subproperties", "C030"},       // COHEN_BRIDGE_SIDECAR_REGISTER_ACCESS_ADDRESS_SELECTOR
        Package(2) { "mipi-sdca-control-0x31-subproperties", "C031"},       // COHEN_BRIDGE_SIDECAR_REGISTER_ACCESS_VALUE_SELECTOR

        // Entity-level
        Package(2) { "mipi-sdca-entity-id-0x2-subproperties", "E002"},  // PDE_34
        Package(2) { "mipi-sdca-entity-id-0x3-subproperties", "E003"},  // PDE_47
        Package(2) { "mipi-sdca-entity-id-0x7-subproperties", "E007"},  // IT_31
        Package(2) { "mipi-sdca-entity-id-0x9-subproperties", "E009"},  // IT_32
        Package(2) { "mipi-sdca-entity-id-0xB-subproperties", "E00B"},  // IT_33
        Package(2) { "mipi-sdca-entity-id-0x4-subproperties", "E004"},  // IT_41
        Package(2) { "mipi-sdca-entity-id-0x8-subproperties", "E008"},  // FU_31
        Package(2) { "mipi-sdca-entity-id-0xA-subproperties", "E00A"},  // FU_32
        Package(2) { "mipi-sdca-entity-id-0xC-subproperties", "E00C"},  // FU_33
#ifndef EXCLUDE_FU_36_VOLUME_CONTROL
        Package(2) { "mipi-sdca-entity-id-0x19-subproperties", "E019"}, // FU_36
#endif  // !EXCLUDE_FU_36_VOLUME_CONTROL
        Package(2) { "mipi-sdca-entity-id-0xE-subproperties", "E00E"},  // FU_35
#ifndef EXCLUDE_FU_41_VOLUME_CONTROL
        Package(2) { "mipi-sdca-entity-id-0x10-subproperties", "E010"}, // FU_41
#endif  // !EXCLUDE_FU_41_VOLUME_CONTROL
        Package(2) { "mipi-sdca-entity-id-0x11-subproperties", "E011"}, // GE_35
        Package(2) { "mipi-sdca-entity-id-0x1-subproperties", "E001"},  // XU_42
        Package(2) { "mipi-sdca-entity-id-0x1F-subproperties", "E01F"}, // XU_36
        Package(2) { "mipi-sdca-entity-id-0x18-subproperties", "E018"}, // MFPU_36
        Package(2) { "mipi-sdca-entity-id-0x6-subproperties", "E006"},  // MFPU_41
        Package(2) { "mipi-sdca-entity-id-0xF-subproperties", "E00F"},  // MU_35
        Package(2) { "mipi-sdca-entity-id-0x1A-subproperties", "E01A"}, // CS_36
        Package(2) { "mipi-sdca-entity-id-0x5-subproperties", "E005"},  // CS_41
        Package(2) { "mipi-sdca-entity-id-0x1B-subproperties", "E01B"}, // OT_36
        Package(2) { "mipi-sdca-entity-id-0x13-subproperties", "E013"}, // OT_43
        Package(2) { "mipi-sdca-entity-id-0x15-subproperties", "E015"}, // OT_44
        Package(2) { "mipi-sdca-entity-id-0x17-subproperties", "E017"}, // OT_45
        Package(2) { "mipi-sdca-entity-id-0xD-subproperties", "E00D"},  // SU_35
        Package(2) { "mipi-sdca-entity-id-0x12-subproperties", "E012"}, // SU_43
        Package(2) { "mipi-sdca-entity-id-0x14-subproperties", "E014"}, // SU_44
        Package(2) { "mipi-sdca-entity-id-0x16-subproperties", "E016"}, // SU_45
        // Clusters
        CLUSTER_INIT_UAJ,
        // Vendor-specific
        Package(2) { "mipi-sdca-function-expansion-subproperties", "EXT0"},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        // Class driver will print warnings if these are not defined.
        // To not pollute the class driver logs, simply do not define these if we are not using them.
        Package(2) { "mipi-sdca-function-initialization-table", "BUF0"},
    }
}) // End _DSD




// +====================================+
// |    Function level controls         |
// +====================================+


//
// BUF0 package moved to a separate file.
//

//
// EXT0 package moved to a separate file.
//

// Function-level control definitions
Name(C004, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // Class, DC, Function_SDCA_Version = 1.0
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS },
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC },
        Package(2) { "mipi-sdca-control-dc-value", CTL_E0_FUNCTION_SDCA_VERSION_VAL },
    }
}) // End C004

Name(C005, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // Class, DC, Function_Type = 0x6 (Universal Audio Jack (UAJ))
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x06},   // UAJ
    }
}) // End C005

Name(C006, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // Class, DC, Function_Manufacturer_Id = 0x01FA
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

Name(C010, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Class, RW1C, Function_Status
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_RW1C},
        Package(2) { "mipi-sdca-control-interrupt-position", COHEN_SDCA_UAJ_FUNC_STATUS_INT},
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


#if CTL_E0_FUNCTION_SDCA_VERSION_VAL < 0x10
// DataPortIndex MAP1
Name(MAP1, Buffer() {
    0x1, 0x6, // {DP_Index, DP_Num} (IT_41)
}) // End MAP1

// DataPortIndex MAP2
Name(MAP2, Buffer() {
    0x2, 0x2, // {DP_Index, DP_Num} (OT_36)
}) // End MAP2
#endif



// +====================================+
// |        Entity Definitions          |
// +====================================+



// +------------------------------------+
// |                PDE_34              |
// +------------------------------------+
Name(E002, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x11 },
        Package(2) { "mipi-sdca-entity-label", "PDE 34"},
        Package(2) { "mipi-sdca-control-list", CTL_PDE_ACTUAL_PS | CTL_PDE_REQUESTED_PS},
        Package(2)
        {
            "mipi-sdca-powerdomain-managed-list",
            Package() {0x7, 0x9, 0xB}, // Entities IT_31, IT_32, IT_33 managed by this PDE.
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
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_RO},
    }
}) // End APS0




// +------------------------------------+
// |                PDE_47              |
// +------------------------------------+
Name(E003, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x11 },
        Package(2) { "mipi-sdca-entity-label", "PDE 47"},
        Package(2) { "mipi-sdca-control-list", CTL_PDE_ACTUAL_PS | CTL_PDE_REQUESTED_PS},
        Package(2)
        {
            "mipi-sdca-powerdomain-managed-list",
            Package() {0x13, 0x15, 0x17}, // Entities OT_43, OT_44, OT_45 managed by this PDE.
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
        Package(2) { "mipi-sdca-control-0x1-subproperties", "RPS0" }, // Reuse. Requested PS
        Package(2) { "mipi-sdca-control-0x10-subproperties", "APS0"}, // Reuse. Actual PS
    }
}) // End E003




// +------------------------------------+
// |                IT_31               |
// |         Stereo Mic On Jack         |
// |     Transducer Input Terminal      |
// +------------------------------------+
Name(E007, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x02},
        Package(2) { "mipi-sdca-entity-label", "IT 31"},
        Package(2) { "mipi-sdca-terminal-type", 0x6A0}, // Mic-on-jack
        // SDCA 1.0 spec, Table 80 Enumeration Values for DisCo Property: Terminal Connector Type
        Package(2) { "mipi-sdca-terminal-connector-type", 0x02},    // 3.5 mm Jack
        Package(2) { "mipi-sdca-terminal-reference-number", 0x01},
        Package(2) { "mipi-sdca-control-list", CTL_IT_CLUSTERINDEX | CTL_IT_LATENCY | CTL_IT_USAGE | CTL_IT_MIC_BIAS},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x3-subproperties", "C103"},
        USAGE_IT_31,
        Package(2) { "mipi-sdca-control-0x8-subproperties", "C108"},
        CLUSTER_IT_31,
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-terminal-reference-description", "RDES"},
    },
}) // End E007

Name(RDES, Buffer()
{
    0x00, 0x01,             // Version
    0x00, 0x00, 0x00, 0x00, // Color (RGB): Black
    0x04, 0x00,             // Gen Location: Right
    0x00, 0x00,             // Geo Location: On Primary Chassis
}) //End RDES

// Mic_Bias Buffer
Name(B103, Buffer()
{
    0x01, 0x00,  // Range type 0x0001
    0x07, 0x00,  // NumRows = 0x7
    0x00, 0x00, 0x00, 0x00,        // HiZ
    0x01, 0x00, 0x00, 0x00,        // GND
    0x02, 0x00, 0x00, 0x00,        // 1.2V
    0x03, 0x00, 0x00, 0x00,        // 1.8V
    0x04, 0x00, 0x00, 0x00,        // 2.5V
    0x05, 0x00, 0x00, 0x00,        // 2.75V
    0xF0, 0x00, 0x00, 0x00,        // 2.0V
}) // End B103


Name(C103, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {    // Class, RW, Mic_Bias
        Package (2) {"mipi-sdca-control-access-layer", CAL_CLASS},
        Package (2) {"mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package (2) {"mipi-sdca-control-deferrable", 1},
        Package (2) {"mipi-sdca-control-default-value", CS42L43_IT_31_MIC_BIAS_DEFAULT},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-control-range", "B103"},
    }
}) // End C103

Name(C108, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // Class, DC, Latency = 0x4 (dummy value)
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x4},
    }
}) // End C108



// +------------------------------------+
// |                IT_32               |
// |            Line In Stereo          |
// |     Transducer Input Terminal      |
// +------------------------------------+
Name(E009, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x02},
        Package(2) { "mipi-sdca-entity-label", "IT 32"},
        Package(2) { "mipi-sdca-terminal-type", 0x680}, // Line-In Stereo
        // SDCA 1.0 spec, Table 80 Enumeration Values for DisCo Property: Terminal Connector Type
        Package(2) { "mipi-sdca-terminal-connector-type", 0x02},    // 3.5 mm Jack
        Package(2) { "mipi-sdca-terminal-reference-number", 0x01},
        Package(2) { "mipi-sdca-control-list", CTL_IT_CLUSTERINDEX | CTL_IT_LATENCY | CTL_IT_USAGE | CTL_IT_MIC_BIAS},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x3-subproperties", "C103"},  // Reuse
        USAGE_IT_32,
        Package(2) { "mipi-sdca-control-0x8-subproperties", "C108"},  // Reuse
        CLUSTER_IT_32,
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-terminal-reference-description", "RDES"},
    },
}) // End E009



// +------------------------------------+
// |                IT_33               |
// |    Headeset (Mono) Mic On Jack     |
// |     Transducer Input Terminal      |
// +------------------------------------+
Name(E00B, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x02},
        Package(2) { "mipi-sdca-entity-label", "IT 33"},
        Package(2) { "mipi-sdca-terminal-type", 0x6D0}, // Headset Mic-on-jack
        // SDCA 1.0 spec, Table 80 Enumeration Values for DisCo Property: Terminal Connector Type
        Package(2) { "mipi-sdca-terminal-connector-type", 0x02},    // 3.5 mm Jack
        Package(2) { "mipi-sdca-terminal-reference-number", 0x01},
        Package(2) { "mipi-sdca-control-list", CTL_IT_CLUSTERINDEX | CTL_IT_LATENCY | CTL_IT_USAGE | CTL_IT_MIC_BIAS},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x3-subproperties", "C103"},  // Reuse
        USAGE_IT_33,
        Package(2) { "mipi-sdca-control-0x8-subproperties", "C108"},  // Reuse
        CLUSTER_IT_33,
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-terminal-reference-description", "RDES"},
    },
}) // End E00B



// +------------------------------------+
// |                IT_41               |
// |       Generic Sink Data Port       |
// |      Streaming Input Terminal      |
// +------------------------------------+
Name(E004, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x02},
        Package(2) { "mipi-sdca-entity-label", "IT 41"},
        Package(2) { "mipi-sdca-terminal-type", 0x101}, // Generic Source / Sink Data Port
        Package(2) { "mipi-sdca-control-list", CTL_IT_DATAPORT_SELECTOR | CTL_IT_CLUSTERINDEX | CTL_IT_LATENCY | CTL_IT_USAGE},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        USAGE_IT_41,
        Package(2) { "mipi-sdca-control-0x8-subproperties", "C108"},  // Reuse
        CLUSTER_IT_41,
        Package(2) { "mipi-sdca-control-0x11-subproperties", "C111"}, // DataPort_Selector
        Package(2) { "mipi-sdca-terminal-clock-connection", "E005" }, // CS_41
    },

#if CTL_E0_FUNCTION_SDCA_VERSION_VAL < 0x10
//
// NOTE:
//  "mipi-sdca-terminal-dp-numbers" has been removed from DisCo for SoundWire 2.1 spec so they should no longer be present in the ASL file.
//  "mipi-sdca-terminal-dp-numbers" has been deprecated, and 1.0-conformant devices will report DP numbers as a range for DataPort_Selector control.
//
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-terminal-dp-numbers", "MAP1"},
    }
#endif
}) // End E004

// DataPortSelector for IT_41
Name(C111, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // DataPortSelector *index* = 1 (see {DP_Index, DP_Num} map in DMAP), Class, DC
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 1},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-control-range", "DPM0"},
    }
}) // End C111

// DataPortMap0
Name(DPM0, Buffer() {
    0x10, 0x00,                 // Range type 0x0010
    0x04, 0x00,                 // NumRows = 4
    // DP_Index_A
    0xFF, 0x00, 0x00, 0x00,     // 0: not used
    0x06, 0x00, 0x00, 0x00,     // 1: DP6
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
// |                CS_41               |
// +------------------------------------+
Name(E005, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0xB},
        Package(2) { "mipi-sdca-entity-label", "CS 41"},
        Package(2) { "mipi-sdca-cs-type", 0}, // External
        Package(2) { "mipi-sdca-control-list", CTL_CS_SAMPLERATEINDEX},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x10-subproperties", "C210"},
    }
}) // End E005

Name(C210, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // Class, RW, SampleRateIndex
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package(2) { "mipi-sdca-control-deferrable", 1},
#ifdef UAJ_RENDER_192KHZ_DEFAULT
        Package(2) { "mipi-sdca-control-default-value", 1}, // 192kHz
#endif
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "B210"},
    }
}) // End C210

// {SampleRateIndex, SampleRate} mapping.
// Cohen cannot support 48kHz, 96kHz and 192kHz sampling rates at the same time.
// Configure for 48kHz, or 48kHz and 96kHz, or 48kHz and 96kHz and 192kHz
// The Sample Rate list:
//     SR 1 (Index 0) = 48 kHz
//     SR 2 (Index 1) = 192 kHz
//     SR 3 (Index 2) = 96 kHz
//     SR 4 (Index 3) = 16 kHz

#ifdef UAJ_RENDER_192KHZ
Name(B210, Buffer()
{
    0x02, 0x00,  // Range type 0x0002 (Pairs)
    0x03, 0x00,  // Count of ranges = 0x3
# ifdef UAJ_RENDER_192KHZ_DEFAULT
    0x01, 0x00, 0x00, 0x00, 0x00, 0xEE, 0x02, 0x00, // SampleRateIndex = 0x00000001, SampleRate = 192000 (0x0002ee00)
    0x02, 0x00, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, // SampleRateIndex = 0x00000002, SampleRate = 96000 (0x00017700)
    0x00, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00, 0x00, // SampleRateIndex = 0x00000000, SampleRate = 48000 (0x0000BB80)
# else  // !UAJ_RENDER_192KHZ_DEFAULT
    0x00, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00, 0x00, // SampleRateIndex = 0x00000000, SampleRate = 48000 (0x0000BB80)
    0x02, 0x00, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, // SampleRateIndex = 0x00000002, SampleRate = 96000 (0x00017700)
    0x01, 0x00, 0x00, 0x00, 0x00, 0xEE, 0x02, 0x00, // SampleRateIndex = 0x00000001, SampleRate = 192000 (0x0002ee00)
# endif // UAJ_RENDER_192KHZ_DEFAULT
}) // End B210
#else
# ifdef UAJ_RENDER_96KHZ
Name(B210, Buffer()
{
    0x02, 0x00,  // Range type 0x0002 (Pairs)
    0x02, 0x00,  // Count of ranges = 0x2
    0x00, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00, 0x00, // SampleRateIndex = 0x00000000, SampleRate = 48000 (0x0000BB80)
    //0x01, 0x00, 0x00, 0x00, 0x44, 0xAC, 0x00, 0x00, // SampleRateIndex = 0x00000001, SampleRate = 44100 (0x0000AC44)
    0x02, 0x00, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, // SampleRateIndex = 0x00000002, SampleRate = 96000 (0x00017700)
    //0x03, 0x00, 0x00, 0x00, 0x80, 0x3e, 0x00, 0x00, // SampleRateIndex = 0x00000003, SampleRate = 16000 (0x00003e80)
}) // End B210
# else
Name(B210, Buffer()
{
    0x02, 0x00,  // Range type 0x0002 (Pairs)
    0x01, 0x00,  // Count of ranges = 0x1
    0x00, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00, 0x00, // SampleRateIndex = 0x00000000, SampleRate = 0x0000BB80 (48000)
}) // End B210
# endif
#endif


// +------------------------------------+
// |                CS_36               |
// +------------------------------------+
Name(E01A, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0xB},
        Package(2) { "mipi-sdca-entity-label", "CS 36"},
        Package(2) { "mipi-sdca-cs-type", 0}, // External
        Package(2) { "mipi-sdca-control-list", CTL_CS_SAMPLERATEINDEX},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x10-subproperties", "C211"},
    }
}) // End E01A

Name(C211, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // Class, RW, SampleRateIndex
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-default-value", 0}, // 48kHz
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "B211"},
    }
}) // End C211

// {SampleRateIndex, SampleRate} mapping.
Name(B211, Buffer()
{
#ifdef UAJ_CAPTURE_96KHZ
    0x02, 0x00,  // Range type 0x0002 (Pairs)
    0x02, 0x00,  // Count of ranges = 0x2
    0x00, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00, 0x00, // SampleRateIndex = 0x00000000, SampleRate = 48000 (0x0000BB80)
    //0x01, 0x00, 0x00, 0x00, 0x44, 0xAC, 0x00, 0x00, // SampleRateIndex = 0x00000001, SampleRate = 44100 (0x0000AC44)
    0x02, 0x00, 0x00, 0x00, 0x00, 0x77, 0x01, 0x00, // SampleRateIndex = 0x00000002, SampleRate = 96000 (0x00017700)
    //0x03, 0x00, 0x00, 0x00, 0x80, 0x3e, 0x00, 0x00, // SampleRateIndex = 0x00000003, SampleRate = 16000 (0x00003e80)
#else
    0x02, 0x00,  // Range type 0x0002 (Pairs)
    0x01, 0x00,  // Count of ranges = 0x1
    0x00, 0x00, 0x00, 0x00, 0x80, 0xBB, 0x00, 0x00, // SampleRateIndex = 0x00000000, SampleRate = 0x0000BB80 (48000)
#endif
}) // End B211


Name(C30B, Package()
{
    // Gain
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // Extension, RW, Control Numbers = 2
        Package(2) { "mipi-sdca-control-access-layer", CAL_EXTENSION},
        Package(2) { "mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-cn-list", 0x6}, // Channel Gain, Control Numbers = {1,2}
    }
}) // End C30B



// +------------------------------------+
// |                FU_31               |
// +------------------------------------+
Name(E008, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x7},
        Package(2) { "mipi-sdca-entity-label", "FU 31"},
        Package(2) { "mipi-sdca-control-list", CTL_FU_CHANNELGAIN},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0xB-subproperties", "C30B"},  // Gain
        Package(2) { "mipi-sdca-input-pin-1", "E007"}, // Input Pin 1 connected to IT_31
    }
}) // End E008



Name(C40B, Package()
{
    // Gain
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // Extension, RW, Control Numbers = 0
        Package(2) { "mipi-sdca-control-access-layer", CAL_EXTENSION},
        Package(2) { "mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-cn-list", 0x1}, // Master Gain, Control Numbers = {0}
    }
}) // End C40B


// +------------------------------------+
// |                FU_32               |
// +------------------------------------+

Name(E00A, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x7},
        Package(2) { "mipi-sdca-entity-label", "FU 32"},
        Package(2) { "mipi-sdca-control-list", CTL_FU_CHANNELGAIN},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0xB-subproperties", "C40B"},
        Package(2) { "mipi-sdca-input-pin-1", "E009"}, // Input Pin 1 connected to IT_32
    }
}) // End E00A



// +------------------------------------+
// |                FU_33               |
// +------------------------------------+

Name(E00C, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x7},
        Package(2) { "mipi-sdca-entity-label", "FU 33"},
        Package(2) { "mipi-sdca-control-list", CTL_FU_CHANNELGAIN},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0xB-subproperties", "C40B"},  // Reuse
        Package(2) { "mipi-sdca-input-pin-1", "E00B"}, // Input Pin 1 connected to IT_33
    }
}) // End E00C



// +------------------------------------+
// |                FU_35               |
// +------------------------------------+

Name(E00E, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x7},
        Package(2) { "mipi-sdca-entity-label", "FU 35"},
        Package(2) { "mipi-sdca-control-list", CTL_FU_LATENCY | CTL_FU_VOLUME | CTL_FU_MUTE},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C301"}, // Mute
        Package(2) { "mipi-sdca-control-0x2-subproperties", "C302"}, // Volume
        Package(2) { "mipi-sdca-control-0x10-subproperties", "C310"}, // Latency
        Package(2) { "mipi-sdca-input-pin-1", "E00D"}, // Input Pin 1 connected to SU_35
    }
}) // End E00E


Name(B302, Buffer()
{
    0x03, 0x00,  // Range type 0x0003 (Triples)
    0x01, 0x00,  // Count of ranges = 0x1
    0x00, 0xC0, 0x00, 0x00,  // Min = 0x0000C000 (-64 dB)
    0x80, 0x1F, 0x00, 0x00,  // Max = 0x00001F80 (31.5 dB)
    0x80, 0x00, 0x00, 0x00,  // Step = 0x00000080 (0.5 dB per step)
}) // End B302


Name(C301, Package()
{
    // Mute
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // Class, Dual, Control Numbers = {1,2}
        Package(2) { "mipi-sdca-control-access-layer", CAL_USER},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DUAL},
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-cn-list", 0x6},
        Package(2) { "mipi-sdca-control-fixed-value", 1},   // muted
    }
}) // End C301

Name(C302, Package()
{
    // Volume
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, Dual, Control Numbers = {1,2}
        Package(2) { "mipi-sdca-control-access-layer", CAL_USER},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DUAL},
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-cn-list", 0x6},
        Package(2) { "mipi-sdca-control-fixed-value", 0x0},   // 0dB
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "B702"},    // must match FU36 volume range
    }
}) // End C302

Name(C310, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // Class, DC, Latency = 0x8
        Package(2) { "mipi-sdca-control-access-layer", CAL_USER},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x8}, // Dummy value for test
    }
}) // End C310


#ifndef EXCLUDE_FU_41_VOLUME_CONTROL
// +------------------------------------+
// |                FU_41               |
// +------------------------------------+

Name(E010, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x7},
        Package(2) { "mipi-sdca-entity-label", "FU 41"},
        Package(2) { "mipi-sdca-control-list", CTL_FU_LATENCY | CTL_FU_VOLUME | CTL_FU_MUTE},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C601"}, // Mute
        Package(2) { "mipi-sdca-control-0x2-subproperties", "C602"}, // Volume
        Package(2) { "mipi-sdca-control-0x10-subproperties", "C610"}, // Latency
        Package(2) { "mipi-sdca-input-pin-1", "E001"}, // Input Pin 1 connected to XU_42
    }
}) // End E010


Name(B602, Buffer()
{
    0x03, 0x00,  // Range type 0x0003 (Triples)
    0x01, 0x00,  // Count of ranges = 0x1
    CS42L43_FU_41_VOL_MIN,
    CS42L43_FU_41_VOL_MAX,
    CS42L43_FU_41_VOL_STEP,
}) // End B602


Name(C601, Package()
{
    // Mute
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // User, Dual, Control Numbers = {1,2}
        Package(2) { "mipi-sdca-control-access-layer", CAL_USER},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DUAL},
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-cn-list", 0x6},
#ifdef FU_41_FIXED_RENDER_MUTE
        Package(2) { "mipi-sdca-control-fixed-value", 0},   // unmuted
#endif
    }
}) // End C601

Name(C602, Package()
{
    // Volume
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // User, Dual, Control Numbers = {1,2}
        Package(2) { "mipi-sdca-control-access-layer", CAL_USER},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DUAL},
#ifdef FU_41_FIXED_RENDER_VOLUME
        Package(2) { "mipi-sdca-control-fixed-value", CS42L43_FU_41_VOL_FIXED_Q7_8},
#else
        Package(2) { "mipi-sdca-control-default-value", CS42L43_FU_41_VOL_DEFAULT},
#endif
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-cn-list", 0x6},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "B602"},
    }
}) // End C602

Name(C610, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // Class, DC, Latency = 0x8
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x8}, // Dummy value for test
    }
}) // End C610
#endif


#ifndef EXCLUDE_FU_36_VOLUME_CONTROL
// +------------------------------------+
// |                FU_36               |
// +------------------------------------+

Name(E019, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x7},
        Package(2) { "mipi-sdca-entity-label", "FU 36"},
        Package(2) { "mipi-sdca-control-list", CTL_FU_LATENCY | CTL_FU_VOLUME | CTL_FU_MUTE},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C701"}, // Mute
        Package(2) { "mipi-sdca-control-0x2-subproperties", "C702"}, // Volume
        Package(2) { "mipi-sdca-control-0x10-subproperties", "C710"}, // Latency
        Package(2) { "mipi-sdca-input-pin-1", "E018"}, // Input Pin 1 connected to MFPU_36
    }
}) // End E019

Name(C701, Package()
{
    // Mute
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // User, R/W, Control Numbers = {1,2}
        Package(2) { "mipi-sdca-control-access-layer", CAL_USER},
        Package(2) { "mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-cn-list", 0x6},
#ifdef FU_36_FIXED_CAPTURE_MUTE
        Package(2) { "mipi-sdca-control-fixed-value", 0},   // unmuted
#endif
    }
}) // End C701

Name(C702, Package()
{
    // Volume
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // User, R/W, Control Numbers = {1,2}
        Package(2) { "mipi-sdca-control-access-layer", CAL_USER},
        Package(2) { "mipi-sdca-control-access-mode", CAM_READ_WRITE},
#ifdef FU_36_FIXED_CAPTURE_VOLUME
        Package(2) { "mipi-sdca-control-fixed-value", CS42L43_FU_36_VOL_FIXED_Q7_8},
#else
        Package(2) { "mipi-sdca-control-default-value", CS42L43_FU_36_VOL_DEFAULT},
#endif
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-cn-list", 0x6},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "B702"},
    }
}) // End C702

Name(C710, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // User, DC, Latency = 0x8
        Package(2) { "mipi-sdca-control-access-layer", CAL_USER},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x8}, // Dummy value for test
    }
}) // End C710
#endif  // !EXCLUDE_FU_36_VOLUME_CONTROL

Name(B702, Buffer()
{
    0x03, 0x00,  // Range type 0x0003 (Triples)
    0x01, 0x00,  // Count of ranges = 0x1
    CS42L43_FU_36_VOL_MIN,
    CS42L43_FU_36_VOL_MAX,
    CS42L43_FU_36_VOL_STEP,
}) // End B702


// +------------------------------------+
// |                SU_35               |
// +------------------------------------+
Name(E00D, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x06},
        Package(2) { "mipi-sdca-entity-label", "SU 35"},
        Package(2) { "mipi-sdca-control-list", CTL_SU_SELECTOR},
        Package(2) { "mipi-sdca-input-pin-list", 0xE }, // Input Pins 1-3 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C401"},
        Package(2) { "mipi-sdca-input-pin-1", "E008"}, // Input Pin 1 connected to FU_31
        Package(2) { "mipi-sdca-input-pin-2", "E00A"}, // Input Pin 2 connected to FU_32
        Package(2) { "mipi-sdca-input-pin-3", "E00C"}, // Input Pin 3 connected to FU_33
    }
}) // End E00D


Name(C401, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {   // Device, RO, Selector
        Package (2) {"mipi-sdca-control-access-layer", CAL_DEVICE},
        Package (2) {"mipi-sdca-control-access-mode", CAM_RO},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-control-range", "B401"},
    }
}) // End C401

// Selector: list of pins
Name(B401, Buffer() {
    0x01, 0x00,             // Range type 0x0001
    0x04, 0x00,             // NumRows = 4
    // Input Pin 0
    0x00, 0x00, 0x00, 0x00, // 0
    // Input Pin 1
    0x01, 0x00, 0x00, 0x00, // 1
    // Input Pin 2
    0x02, 0x00, 0x00, 0x00, // 2
    // Input Pin 3
    0x03, 0x00, 0x00, 0x00, // 3
}) // End B401


// +------------------------------------+
// |                SU_43               |
// +------------------------------------+
Name(E012, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x06},
        Package(2) { "mipi-sdca-entity-label", "SU 43"},
        Package(2) { "mipi-sdca-control-list", CTL_SU_SELECTOR},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C401"}, // Reuse
#ifdef EXCLUDE_FU_41_VOLUME_CONTROL
        Package(2) { "mipi-sdca-input-pin-1", "E001"}, // Input Pin 1 connected to XU_42
#else
        Package(2) { "mipi-sdca-input-pin-1", "E010"}, // Input Pin 1 connected to FU_41
#endif
    }
}) // End E012



// +------------------------------------+
// |                SU_44               |
// +------------------------------------+
Name(E014, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x06},
        Package(2) { "mipi-sdca-entity-label", "SU 44"},
        Package(2) { "mipi-sdca-control-list", CTL_SU_SELECTOR},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C401"}, // Reuse
#ifdef EXCLUDE_FU_41_VOLUME_CONTROL
        Package(2) { "mipi-sdca-input-pin-1", "E001"}, // Input Pin 1 connected to XU_42
#else
        Package(2) { "mipi-sdca-input-pin-1", "E010"}, // Input Pin 1 connected to FU_41
#endif
    }
}) // End E014



// +------------------------------------+
// |                SU_45               |
// +------------------------------------+
Name(E016, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x06},
        Package(2) { "mipi-sdca-entity-label", "SU 45"},
        Package(2) { "mipi-sdca-control-list", CTL_SU_SELECTOR},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C401"}, // Reuse
#ifdef EXCLUDE_FU_41_VOLUME_CONTROL
        Package(2) { "mipi-sdca-input-pin-1", "E001"}, // Input Pin 1 connected to XU_42
#else
        Package(2) { "mipi-sdca-input-pin-1", "E010"}, // Input Pin 1 connected to FU_41
#endif
    }
}) // End E016



// +------------------------------------+
// |                MFPU_41             |
// +------------------------------------+
Name(E006, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x22},
        Package(2) { "mipi-sdca-entity-label", "MFPU 41"},
        Package(2) { "mipi-sdca-control-list", CTL_MFPU_BYPASS},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C501"}, // Bypass
        Package(2) { "mipi-sdca-input-pin-1", "E004"}, // Input Pin 1 connected to IT_41
    }
}) // End E006

Name(C501, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // Bypass, Class, DC. Defining as 'Null' MFPU (DC:1)
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x1},
    }
}) // End C501



// +------------------------------------+
// |                XU_36               |
// +------------------------------------+
Name(E01F, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x0A},
        Package(2) { "mipi-sdca-entity-label", "XU 36"},
        Package(2) { "mipi-sdca-control-list", CTL_XU_BYPASS | CTL_XU_IMPDEF_GPIO},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "CF01"}, // Bypass
        Package(2) { "mipi-sdca-control-0x30-subproperties", "CF30"}, // GPIO
        Package(2) { "mipi-sdca-input-pin-1", "E00D"}, // Input Pin 1 connected to SU_35
    }
}) // End E01F


Name(CF01, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Bypass, Class, RW.
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package(2) { "mipi-sdca-control-deferrable", 1},
    }
}) // End CF01


Name(CF30, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // GPIO, Extension, RW.
        Package(2) { "mipi-sdca-control-access-layer", CAL_EXTENSION},
        Package(2) { "mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-cn-list", 0x7}, // Control Numbers = {0,1,2}
    }
}) // End CF30



// +------------------------------------+
// |                MFPU_36             |
// +------------------------------------+
Name(E018, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x22},
        Package(2) { "mipi-sdca-entity-label", "MFPU 36"},
        Package(2) { "mipi-sdca-control-list", CTL_MFPU_BYPASS},
        Package(2) { "mipi-sdca-input-pin-list", 0x2 }, // Input Pin 1 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C501"}, // Reuse
        Package(2) { "mipi-sdca-input-pin-1", "E01F"}, // Input Pin 1 connected to XU_36
    }
}) // End E018



// +------------------------------------+
// |                MU_35               |
// +------------------------------------+
Name(E00F, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x5},
        Package(2) { "mipi-sdca-entity-label", "MU 35"},
        Package(2) { "mipi-sdca-control-list", CTL_MU_LATENCY | CTL_MU_MIXER},
        Package(2) { "mipi-sdca-input-pin-list", 0x6 }, // Input Pins 1, 2 connected
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C801"},
        Package(2) { "mipi-sdca-control-0x6-subproperties", "C806"},
        Package(2) { "mipi-sdca-input-pin-1", "E006"}, // Input Pin 1 connected to MFPU_41
        Package(2) { "mipi-sdca-input-pin-2", "E00E"}, // Input Pin 2 connected to FU_35
    }
}) // End E00F


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
}) // End C801


// Mixer: unity gain
Name(B801, Buffer() {
    0x01, 0x00,             // Range type 0x0001
    0x01, 0x00,             // NumRows = 1
    0x00, 0x00, 0x00, 0x00, // 0.0dB
}) // End B801


Name(C806, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // Class, DC, Latency = 0x8
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x8}, // Dummy value for test
    }
}) // End C806



// +------------------------------------+
// |                XU_42               |
// +------------------------------------+
Name(E001, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x0A},
        Package(2) { "mipi-sdca-entity-label", "XU 42"},
        Package(2) { "mipi-sdca-input-pin-list", 0x2}, // Pin1 has a connection
        Package(2) { "mipi-sdca-control-list", CTL_XU_VERSION | CTL_XU_ID | CTL_XU_BYPASS},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        Package(2) { "mipi-sdca-control-0x1-subproperties", "C901"}, // Bypass
        Package(2) { "mipi-sdca-control-0x7-subproperties", "C907"}, // XU_ID
        Package(2) { "mipi-sdca-control-0x8-subproperties", "C908"}, // XU_Version
        Package(2) { "mipi-sdca-input-pin-1", "E00F"}, // Input Pin 1 connected to MU_35
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
    {    // Class, DC, XU_ID
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 1},
    }
}) // End C907

Name(C908, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // Class, DC, XU_Version
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0x10},
    }
}) // End C908



// +------------------------------------+
// |                OT_36               |
// |       Generic Sink Dataport        |
// |     Streaming Output Terminal      |
// +------------------------------------+
Name(E01B, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x03},
        Package(2) { "mipi-sdca-entity-label", "OT 36"},
        Package(2) { "mipi-sdca-terminal-type", 0x101}, // Generic Source Data Port
        Package(2) { "mipi-sdca-input-pin-list", 0x2}, // Pin1 has a connection
        Package(2) { "mipi-sdca-control-list", CTL_OT_DATAPORT_SELECTOR | CTL_OT_LATENCY | CTL_OT_USAGE},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        USAGE_OT_36,
        Package(2) { "mipi-sdca-control-0x8-subproperties", "CA08"}, // Latency
        Package(2) { "mipi-sdca-control-0x11-subproperties", "CA11"}, // DataPort_Selector
        Package(2) { "mipi-sdca-terminal-clock-connection", "E01A" }, // CS_36
#ifdef EXCLUDE_FU_36_VOLUME_CONTROL
        Package(2) { "mipi-sdca-input-pin-1", "E018"}, // Input Pin 1 connected to MFPU_36
#else   // !EXCLUDE_FU_36_VOLUME_CONTROL
        Package(2) { "mipi-sdca-input-pin-1", "E019"}, // Input Pin 1 connected to FU_36
#endif  // EXCLUDE_FU_36_VOLUME_CONTROL
    },
#if CTL_E0_FUNCTION_SDCA_VERSION_VAL < 0x10
//
// NOTE:
//  "mipi-sdca-terminal-dp-numbers" has been removed from DisCo for SoundWire 2.1 spec so they should no longer be present in the ASL file.
//  "mipi-sdca-terminal-dp-numbers" has been deprecated, and 1.0-conformant devices will report DP numbers as a range for DataPort_Selector control.
//
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-terminal-dp-numbers", "MAP2"},
    }
#endif
}) // End E01B


Name(CA08, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // Class, DC, Latency = 0xA (dummy value)
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0xA},
    }
}) // End CA08

Name(CA11, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // DataPort Selector *index* = 2 (see {DP_Index, DP_Num} map in DMAP), Class, DC
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 2},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-control-range", "DPM1"},
    }
}) // End CA11

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
// |                OT_43               |
// |       Headphone Output Jack        |
// |     Transducer Output Terminal     |
// +------------------------------------+
Name(E013, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x03},
        Package(2) { "mipi-sdca-entity-label", "OT 43"},
        Package(2) { "mipi-sdca-terminal-type", 0x6C0}, // Headphone-on-Jack (HP)
        // SDCA 1.0 spec, Table 80 Enumeration Values for DisCo Property: Terminal Connector Type
        Package(2) { "mipi-sdca-terminal-connector-type", 0x02},    // 3.5 mm Jack
        Package(2) { "mipi-sdca-terminal-reference-number", 0x01},
        Package(2) { "mipi-sdca-input-pin-list", 0x2}, // Pin1 has a connection
        Package(2) { "mipi-sdca-control-list", CTL_OT_LATENCY | CTL_OT_USAGE},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        USAGE_OT_43,
        Package(2) { "mipi-sdca-control-0x8-subproperties", "CB08"}, // Latency
        Package(2) { "mipi-sdca-input-pin-1", "E012"}, // SU_43
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-terminal-reference-description", "RDES"},
    },
}) // End E013

Name(CB08, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // Class, DC, Latency = 0xA (dummy value)
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0xA},
    }
}) // End CB08



// +------------------------------------+
// |                OT_44               |
// |           Line Out Stereo          |
// |     Transducer Output Terminal     |
// +------------------------------------+
Name(E015, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x03},
        Package(2) { "mipi-sdca-entity-label", "OT 44"},
        Package(2) { "mipi-sdca-terminal-type", 0x690}, // LineOut Stereo
        // SDCA 1.0 spec, Table 80 Enumeration Values for DisCo Property: Terminal Connector Type
        Package(2) { "mipi-sdca-terminal-connector-type", 0x02},    // 3.5 mm Jack
        Package(2) { "mipi-sdca-terminal-reference-number", 0x01},
        Package(2) { "mipi-sdca-input-pin-list", 0x2}, // Pin1 has a connection
        Package(2) { "mipi-sdca-control-list", CTL_OT_LATENCY | CTL_OT_USAGE},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        USAGE_OT_44,
        Package(2) { "mipi-sdca-control-0x8-subproperties", "CC08"}, // Latency
        Package(2) { "mipi-sdca-input-pin-1", "E014"}, // SU_44
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-terminal-reference-description", "RDES"},
    },
}) // End E015


Name(CC08, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // Class, DC, Latency = 0xA (dummy value)
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0xA},
    }
}) // End CC08



// +------------------------------------+
// |                OT_45               |
// |       Headset Output On Jack       |
// |     Transducer Output Terminal     |
// +------------------------------------+
Name(E017, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-entity-type", 0x03},
        Package(2) { "mipi-sdca-entity-label", "OT 45"},
        Package(2) { "mipi-sdca-terminal-type", 0x6D0}, // Headset-output-on-Jack (HS)
        // SDCA 1.0 spec, Table 80 Enumeration Values for DisCo Property: Terminal Connector Type
        Package(2) { "mipi-sdca-terminal-connector-type", 0x02},    // 3.5 mm Jack
        Package(2) { "mipi-sdca-terminal-reference-number", 0x01},
        Package(2) { "mipi-sdca-input-pin-list", 0x2}, // Pin1 has a connection
        Package(2) { "mipi-sdca-control-list", CTL_OT_LATENCY | CTL_OT_USAGE},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package()
    {
        USAGE_OT_45,
        Package(2) { "mipi-sdca-control-0x8-subproperties", "CD08"}, // Latency
        Package(2) { "mipi-sdca-input-pin-1", "E016"}, // SU_45
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) {"mipi-sdca-terminal-reference-description", "RDES"},
    },
}) // End E017


Name(CD08, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // Class, DC, Latency = 0xA (dummy value)
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 0xA},
    }
}) // End CD08



// +------------------------------------+
// |                GE_35               |
// +------------------------------------+

//
// E011 package moved to a separate file
//

Name(CE01, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {    // Class, RW
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_READ_WRITE},
        Package(2) { "mipi-sdca-control-deferrable", 1},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package () {
        Package (2) {"mipi-sdca-control-range", "BMT2"},
    }
}) // End CE01


Name(CE02, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {   // Class, RO
        // Interrupt bit position = 11
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-interrupt-position", COHEN_SDCA_UAJ_JACK_MODE_UPDATE_INT},
        Package(2) { "mipi-sdca-control-access-mode", CAM_RO},
        Package(2) { "mipi-sdca-control-deferrable", 1},
    }
}) // End CE02

Name(CE31, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Extension/Platform, RW, Load Detection
        Package(2) { "mipi-sdca-control-access-layer", CAL_EXTENSION | CAL_PLATFORM},
        Package(2) { "mipi-sdca-control-access-mode", CAM_READ_WRITE},
        //Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-cn-list", 0x1 }, // 0x0
    }
}) // End CE31

Name(CE32, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        // Extension/Platform, ASP Output
        Package(2) { "mipi-sdca-control-access-layer", CAL_EXTENSION | CAL_PLATFORM},
        Package(2) { "mipi-sdca-control-access-mode", CAM_READ_WRITE},
        //Package(2) { "mipi-sdca-control-deferrable", 1},
        Package(2) { "mipi-sdca-control-cn-list", 0x1 }, // 0x0
    }
}) // End CE32
