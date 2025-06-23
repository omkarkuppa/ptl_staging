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

#ifndef ENABLE_CLUSTERS

# define CLUSTER_IT_21
# define CLUSTER_IT_26
# define CLUSTER_IT_29
# define CLUSTER_MFPU_26
# define CLUSTER_UDMPU_23
# define CLUSTER_UDMPU_25

#define CLUSTER_ID_LIST_JAMERSON
#define CLUSTER_INIT_JAMERSON

#else

//
// Let's assume that initially 4 AMP channels relationships are available unless 2 are selected.
//
# define POSTURE_CLUSTER_FOUR_REL

# ifdef CHANNEL_RIGHT
#  undef POSTURE_CLUSTER_FOUR_REL
# endif

# ifdef CHANNEL_LEFT
#  undef POSTURE_CLUSTER_FOUR_REL
# endif

# define CLUSTER_IT_21    Package(2) {"mipi-sdca-control-0x10-subproperties", "CI21"}
# define CLUSTER_IT_26    Package(2) {"mipi-sdca-control-0x10-subproperties", "CI26"}
# define CLUSTER_IT_29    Package(2) {"mipi-sdca-control-0x10-subproperties", "CI26"}
# define CLUSTER_MFPU_26  Package(2) {"mipi-sdca-control-0x10-subproperties", "CI26"}
# define CLUSTER_UDMPU_23 Package(2) {"mipi-sdca-control-0x10-subproperties", "CI23"}
# define CLUSTER_UDMPU_25 Package(2) {"mipi-sdca-control-0x10-subproperties", "CI25"}

# ifdef POSTURE_CLUSTER_FOUR_REL
#  define CLUSTER_ID_LIST_JAMERSON Package (2) {"mipi-sdca-cluster-id-list", Package() {0x1, 0x21, 0x23, 0x25, 0x26, 0x29, 0xF0, 0xF1, 0xF2, 0xF3} }
# else
#  define CLUSTER_ID_LIST_JAMERSON Package (2) {"mipi-sdca-cluster-id-list", Package() {0x1, 0x21, 0x23, 0x25, 0x26, 0x29, 0xF0, 0xF1} }
# endif

// This is one really long line, since the preprocessor will not except line extenders '\'.
# ifdef POSTURE_CLUSTER_FOUR_REL
#  define CLUSTER_INIT_JAMERSON    Package (2) {"mipi-sdca-cluster-id-0x1-subproperties",  "CL29"}, Package (2) {"mipi-sdca-cluster-id-0x21-subproperties", "CL21"}, Package (2) {"mipi-sdca-cluster-id-0x23-subproperties", "CL23"}, Package (2) {"mipi-sdca-cluster-id-0x25-subproperties", "CL25"}, Package (2) {"mipi-sdca-cluster-id-0x26-subproperties", "CL26"}, Package (2) {"mipi-sdca-cluster-id-0x29-subproperties", "CL29"}, Package (2) {"mipi-sdca-cluster-id-0xF0-subproperties", "CLF0"}, Package (2) {"mipi-sdca-cluster-id-0xF1-subproperties", "CLF1"}, Package (2) {"mipi-sdca-cluster-id-0xF2-subproperties", "CLF2"}, Package (2) {"mipi-sdca-cluster-id-0xF3-subproperties", "CLF3"}
# else
#  define CLUSTER_INIT_JAMERSON    Package (2) {"mipi-sdca-cluster-id-0x1-subproperties",  "CL29"}, Package (2) {"mipi-sdca-cluster-id-0x21-subproperties", "CL21"}, Package (2) {"mipi-sdca-cluster-id-0x23-subproperties", "CL23"}, Package (2) {"mipi-sdca-cluster-id-0x25-subproperties", "CL25"}, Package (2) {"mipi-sdca-cluster-id-0x26-subproperties", "CL26"}, Package (2) {"mipi-sdca-cluster-id-0x29-subproperties", "CL29"}, Package (2) {"mipi-sdca-cluster-id-0xF0-subproperties", "CLF0"}, Package (2) {"mipi-sdca-cluster-id-0xF1-subproperties", "CLF1"}
# endif

// +-----------------------+
// | Clusters for IT29     |
// | Streaming Sense Input |
// +-----------------------+
Name(CL29, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-channel-count", 2},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package ()
    {
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHS1" },
       Package (2) { "mipi-sdca-channel-2-subproperties", "CHS2" },
    }
})

Name(CHS1, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 1 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_GENERIC_LEFT },
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_SENSE_V1 },
    }
})

Name(CHS2, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 2 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_GENERIC_LEFT },
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_SENSE_V2 },
    }
})

Name(CI29, Package()
{
    // Class, DC, ClusterIndex = 0x31
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS },
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC },
        Package(2) { "mipi-sdca-control-dc-value", 0x31 },
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "CM29" },
    }
})

// By rule #3 in section 5.2.6.10 (Version 0.9 r02) OT24 must point to a cluster index
// in the range from 0x01 to 0x0F which is an alias of IT29.
Name(CI24, Package()
{
    // Class, DC, ClusterIndex = 0x1
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS },
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC },
        Package(2) { "mipi-sdca-control-dc-value", 0x01 },
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "CM29" },
    }
})

// ClusterMap: Defines {ClusterIndex, Cluster_ID} relationships
Name(CM29, Buffer()
{
    0x02, 0x00,    // Range type 0x0002
    0x02, 0x00,    // 2 rows
    0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,    // ClusterIndex 01 --> ClusterID 1
    0x31, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00     // ClusterIndex 31 --> ClusterID 29
})

// +-------------------+
// | Clusters for IT21 |
// | Amp Render Stream |
// +-------------------+
// TODO: Check and see if this should be single channel for left and right.
Name(CL21, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-channel-count", 2 },
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHR1" },
       Package (2) { "mipi-sdca-channel-2-subproperties", "CHR2" },
    }
})

Name(CHR1, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 1 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_GENERIC_LEFT },
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_GENERIC_AUDIO },
    }
})

Name(CHR2, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 2 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_GENERIC_RIGHT },
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_GENERIC_AUDIO} ,
    }
})

Name(CI21, Package()
{
    // Class, DC, ClusterIndex = 0x1
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS },
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC },
        Package(2) { "mipi-sdca-control-dc-value", 0x01 },
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "CM21" },
    }
})

 // ClusterMap: Defines {ClusterIndex, Cluster_ID} relationships
Name(CM21, Buffer()
{
    0x02, 0x00,    // Range type 0x0002
    0x01, 0x00,    // 2 rows
    0x01, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00    // ClusterIndex 01 --> ClusterID 21
}) // End CMP1

// +-------------------+
// | Clusters for IT26 |
// | Amp Render Stream |
// +-------------------+
Name(CL26, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-channel-count", 1 },
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package ()
    {
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHR4" },
    }
})

Name(CHR4, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 1 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_GENERIC_MONO },
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_ULTRASOUND },
    }
})

Name(CI26, Package()
{
    // Class, DC, ClusterIndex = 0x1
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS },
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC },
        Package(2) { "mipi-sdca-control-dc-value", 0x01 },
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "CM26" },
    }
})

 // ClusterMap: Defines {ClusterIndex, Cluster_ID} relationships
Name(CM26, Buffer()
{
    0x02, 0x00,    // Range type 0x0002
    0x01, 0x00,    // 2 rows
    0x01, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00    // ClusterIndex 01 --> ClusterID 26
}) // End CMP1

// +------------------------+
// |  Clusters for UDMDP23  |
// | Amp Transducer Mapping |
// +------------------------+
Name(CL23, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-channel-count", 1 },
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package ()
    {
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHR6" },
    }
})

Name(CHR6, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 0 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_PRIMARY_TRANSDUCER },
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_GENERIC_AUDIO },
    }
})

Name(CI23, Package()
{
    // Class, DC, ClusterIndex = 0x1
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS },
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC },
        Package(2) { "mipi-sdca-control-dc-value", 0x01 },
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "CM23" },
    }
})

 // ClusterMap: Defines {ClusterIndex, Cluster_ID} relationships
Name(CM23, Buffer()
{
    0x02, 0x00,    // Range type 0x0002
    0x01, 0x00,    // 2 rows
    0x01, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00    // ClusterIndex 01 --> ClusterID 23
}) // End CMP1

// +----------------------+
// | Clusters for UDMDP25 |
// | Amp Reference Stream |
// +----------------------+
Name(CL25, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-channel-count", 1 },
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package ()
    {
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHE1" },
    }
})

Name(CHE1, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 101 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_ECHO_REF_1 },
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_REFERENCE },
    }
})

Name(CI25, Package()
{
    // Class, DC, ClusterIndex = 0x1
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS },
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC },
        Package(2) { "mipi-sdca-control-dc-value", 0x01 },
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "CM25" },
    }
})

 // ClusterMap: Defines {ClusterIndex, Cluster_ID} relationships
Name(CM25, Buffer()
{
    0x02, 0x00,    // Range type 0x0002
    0x01, 0x00,    // 2 rows
    0x01, 0x00, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00    // ClusterIndex 01 --> ClusterID 25
}) // End CMP1

// +--------------------------+
// | Clusters for Posture L/R |
// +--------------------------+

# ifdef COHEN_CLUSTER

Name(CH_L, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 1 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_EQUIPMENT_LEFT },
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_GENERIC_AUDIO },
    }
})

Name(CH_R, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 2 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_EQUIPMENT_RIGHT },
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_GENERIC_AUDIO} ,
    }
})

//
// Stereo channel, normal, LEFT-RIGHT
//
Name(CLF0, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-channel-count", 2 },
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
       Package (2) { "mipi-sdca-channel-1-subproperties", "CH_L" },
       Package (2) { "mipi-sdca-channel-2-subproperties", "CH_R" },
    }
})

//
// Stereo channel, swapped, RIGHT-LEFT
//
Name(CLF1, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-channel-count", 2 },
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
       Package (2) { "mipi-sdca-channel-1-subproperties", "CH_R" },
       Package (2) { "mipi-sdca-channel-2-subproperties", "CH_L" },
    }
})

// +------------------------------------+
// | Default Posture for Cohen          |
// +------------------------------------+
Name(PMAP, Buffer()
{
    0x0b, 0x00,  // Range type 0x0b (Elevens)
#  ifdef ENABLE_POSTURE
    0x02, 0x00,  // Count of ranges = 0x2
#  else
    0x01, 0x00,  // Count of ranges = 0x1
#  endif

    // Entry 0
    0x00, 0x00, 0x00, 0x00,  // Pitch Min 0
    0x67, 0x01, 0x00, 0x00,  // Pitch Max 359
    0x00, 0x00, 0x00, 0x00,  // Roll Min
    0x00, 0x00, 0x00, 0x00,  // Roll Max (Roll 0,0 => NotRotated
    0x00, 0x00, 0x00, 0x00,  // Yaw Min 0
    0x67, 0x01, 0x00, 0x00,  // Yaw Max 359
    0x00, 0x00, 0x00, 0x00,  // Hinge Angle Min 0
    0x67, 0x01, 0x00, 0x00,  // Hinge Angle Max 359
    0x00, 0x00, 0x00, 0x00,  // System Extension 0
    0x00, 0x00, 0x00, 0x00,  // Posture Number = 0
    0xF0, 0x00, 0x00, 0x00,  // Cluster ID = 0xF0

#  ifdef ENABLE_POSTURE
    // Entry 1
    0x00, 0x00, 0x00, 0x00,  // Pitch Min 0
    0x67, 0x01, 0x00, 0x00,  // Pitch Max 359
    0xb4, 0x00, 0x00, 0x00,  // Roll Min
    0xb4, 0x00, 0x00, 0x00,  // Roll Max (Roll 180,180 => Rotated180CounterClockwise
    0x00, 0x00, 0x00, 0x00,  // Yaw Min 0
    0x67, 0x01, 0x00, 0x00,  // Yaw Max 359
    0x00, 0x00, 0x00, 0x00,  // Hinge Angle Min 0
    0x67, 0x01, 0x00, 0x00,  // Hinge Angle Max 359
    0x00, 0x00, 0x00, 0x00,  // System Extension 0
    0x01, 0x00, 0x00, 0x00,  // Posture Number = 1
    0xF1, 0x00, 0x00, 0x00,  // Cluster ID = 0xF1
#  endif
})

# endif  // COHEN_CLUSTER

# ifdef JAMERSON_CLUSTER

#  ifdef POSTURE_CLUSTER_FOUR_REL

Name(CHTL, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 1 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_EQUIPMENT_TOP_LEFT },
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_GENERIC_AUDIO },
    }
})

Name(CHBL, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 1 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_EQUIPMENT_BOTTOM_LEFT },
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_GENERIC_AUDIO },
    }
})

Name(CHTR, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 2 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_EQUIPMENT_TOP_RIGHT },
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_GENERIC_AUDIO} ,
    }
})

Name(CHBR, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 2 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_EQUIPMENT_BOTTOM_RIGHT },
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_GENERIC_AUDIO} ,
    }
})

//
//             rotation ->
//   +-----------------+
//   |       Oo        |
//   +-----------------+
//   | TL           TR |
//   |                 |
//   |                 |
//   |                 |
//   | BL           BR |
//   +-----------------+
// <- rotation
//
// Rotations are in 90deg steps, clockwise
//

//
// Signle channel, no rotation.
//
Name(CLF0, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-channel-count", 1 },
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package ()
    {
#   ifdef CHANNEL_TOP_LEFT
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHTL" },
#   else      // !CHANNEL_TOP_LEFT
#    ifdef CHANNEL_BOTTOM_LEFT
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHBL" },
#    else     // !CHANNEL_BOTTOM_LEFT
#     ifdef CHANNEL_TOP_RIGHT
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHTR" },
#     else    // !CHANNEL_TOP_RIGHT
#      ifdef CHANNEL_BOTTOM_RIGHT
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHBR" },
#      else   // !CHANNEL_BOTTOM_RIGHT
#       error "No channel relationship defined"
#      endif  // CHANNEL_BOTTOM_RIGHT
#     endif   // CHANNEL_TOP_RIGHT
#    endif    // CHANNEL_BOTTOM_LEFT
#   endif     // CHANNEL_TOP_LEFT
    }
})

//
// Single channel, rotated 90.
//
Name(CLF1, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-channel-count", 1 },
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
#   ifdef CHANNEL_TOP_LEFT
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHTR" },
#   else      // !CHANNEL_TOP_LEFT
#    ifdef CHANNEL_BOTTOM_LEFT
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHTL" },
#    else     // !CHANNEL_BOTTOM_LEFT
#     ifdef CHANNEL_TOP_RIGHT
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHBR" },
#     else    // !CHANNEL_TOP_RIGHT
#      ifdef CHANNEL_BOTTOM_RIGHT
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHBL" },
#      else   // !CHANNEL_BOTTOM_RIGHT
#       error "No channel relationship defined"
#      endif  // CHANNEL_BOTTOM_RIGHT
#     endif   // CHANNEL_TOP_RIGHT
#    endif    // CHANNEL_BOTTOM_LEFT
#   endif     // CHANNEL_TOP_LEFT
    }
})

//
// Single channel, rotated 180.
//
Name(CLF2, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-channel-count", 1 },
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
#   ifdef CHANNEL_TOP_LEFT
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHBR" },
#   else      // !CHANNEL_TOP_LEFT
#    ifdef CHANNEL_BOTTOM_LEFT
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHTR" },
#    else     // !CHANNEL_BOTTOM_LEFT
#     ifdef CHANNEL_TOP_RIGHT
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHBL" },
#     else    // !CHANNEL_TOP_RIGHT
#      ifdef CHANNEL_BOTTOM_RIGHT
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHTL" },
#      else   // !CHANNEL_BOTTOM_RIGHT
#       error "No channel relationship defined"
#      endif  // CHANNEL_BOTTOM_RIGHT
#     endif   // CHANNEL_TOP_RIGHT
#    endif    // CHANNEL_BOTTOM_LEFT
#   endif     // CHANNEL_TOP_LEFT
    }
})

//
// Single channel, rotated 270.
//
Name(CLF3, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-channel-count", 1 },
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
#   ifdef CHANNEL_TOP_LEFT
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHBL" },
#   else      // !CHANNEL_TOP_LEFT
#    ifdef CHANNEL_BOTTOM_LEFT
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHBR" },
#    else     // !CHANNEL_BOTTOM_LEFT
#     ifdef CHANNEL_TOP_RIGHT
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHTL" },
#     else    // !CHANNEL_TOP_RIGHT
#      ifdef CHANNEL_BOTTOM_RIGHT
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHTR" },
#      else   // !CHANNEL_BOTTOM_RIGHT
#       error "No channel relationship defined"
#      endif  // CHANNEL_BOTTOM_RIGHT
#     endif   // CHANNEL_TOP_RIGHT
#    endif    // CHANNEL_BOTTOM_LEFT
#   endif     // CHANNEL_TOP_LEFT
    }
})

Name (PMAP, Buffer ()
{
    0x0b, 0x00,  // Range type 0x0b (Elevens)
#   ifdef ENABLE_POSTURE
    0x05, 0x00,  // Count of ranges = 0x5
#   else
    0x01, 0x00,  // Count of ranges = 0x1
#   endif

    // Entry 0
    0x00, 0x00, 0x00, 0x00,  // Pitch Min 0
    0x67, 0x01, 0x00, 0x00,  // Pitch Max 359
    0x00, 0x00, 0x00, 0x00,  // Roll Min
    0x00, 0x00, 0x00, 0x00,  // Roll Max (Roll 0,0 => NotRotated
    0x00, 0x00, 0x00, 0x00,  // Yaw Min 0
    0x67, 0x01, 0x00, 0x00,  // Yaw Max 359
    0x00, 0x00, 0x00, 0x00,  // Hinge Angle Min 0
    0x67, 0x01, 0x00, 0x00,  // Hinge Angle Max 359
    0x00, 0x00, 0x00, 0x00,  // System Extension 0
    0x00, 0x00, 0x00, 0x00,  // Posture Number = 0
    0xF0, 0x00, 0x00, 0x00,  // Cluster ID = 0xF0

#   ifdef ENABLE_POSTURE
    // Entry 1
    0x00, 0x00, 0x00, 0x00,  // Pitch Min 0
    0x67, 0x01, 0x00, 0x00,  // Pitch Max 359
    0x5a, 0x00, 0x00, 0x00,  // Roll Min
    0x5a, 0x00, 0x00, 0x00,  // Roll Max (Roll 90,90 => Rotated270CounterClockwise
    0x00, 0x00, 0x00, 0x00,  // Yaw Min 0
    0x67, 0x01, 0x00, 0x00,  // Yaw Max 359
    0x00, 0x00, 0x00, 0x00,  // Hinge Angle Min 0
    0x67, 0x01, 0x00, 0x00,  // Hinge Angle Max 359
    0x00, 0x00, 0x00, 0x00,  // System Extension 0
    0x02, 0x00, 0x00, 0x00,  // Posture Number = 2
    0xF1, 0x00, 0x00, 0x00,  // Cluster ID = 0xF1

    // Entry 2
    0x00, 0x00, 0x00, 0x00,  // Pitch Min 0
    0x67, 0x01, 0x00, 0x00,  // Pitch Max 359
    0xb4, 0x00, 0x00, 0x00,  // Roll Min
    0xb4, 0x00, 0x00, 0x00,  // Roll Max (Roll 180,180 => Rotated180CounterClockwise
    0x00, 0x00, 0x00, 0x00,  // Yaw Min 0
    0x67, 0x01, 0x00, 0x00,  // Yaw Max 359
    0x00, 0x00, 0x00, 0x00,  // Hinge Angle Min 0
    0x67, 0x01, 0x00, 0x00,  // Hinge Angle Max 359
    0x00, 0x00, 0x00, 0x00,  // System Extension 0
    0x01, 0x00, 0x00, 0x00,  // Posture Number = 1
    0xF2, 0x00, 0x00, 0x00,  // Cluster ID = 0xF2

    // Entry 3
    0x00, 0x00, 0x00, 0x00,  // Pitch Min 0
    0x67, 0x01, 0x00, 0x00,  // Pitch Max 359
    0x0e, 0x01, 0x00, 0x00,  // Roll Min
    0x0e, 0x01, 0x00, 0x00,  // Roll Max (Roll 270,270 => Rotated90CounterClockwise
    0x00, 0x00, 0x00, 0x00,  // Yaw Min 0
    0x67, 0x01, 0x00, 0x00,  // Yaw Max 359
    0x00, 0x00, 0x00, 0x00,  // Hinge Angle Min 0
    0x67, 0x01, 0x00, 0x00,  // Hinge Angle Max 359
    0x00, 0x00, 0x00, 0x00,  // System Extension 0
    0x03, 0x00, 0x00, 0x00,  // Posture Number = 3
    0xF3, 0x00, 0x00, 0x00,  // Cluster ID = 0xF3

    // Entry 4
    0x00, 0x00, 0x00, 0x00,  // Pitch Min 0
    0x67, 0x01, 0x00, 0x00,  // Pitch Max 359
    0x00, 0x00, 0x00, 0x00,  // Roll Min 0
    0x67, 0x01, 0x00, 0x00,  // Roll Max 359
    0x00, 0x00, 0x00, 0x00,  // Yaw Min 0
    0x67, 0x01, 0x00, 0x00,  // Yaw Max 359
    0x00, 0x00, 0x00, 0x00,  // Hinge Angle Min 0
    0x00, 0x00, 0x00, 0x00,  // Hinge Angle Max 0 (Hinge 0,0 => LidClosed)
    0x00, 0x00, 0x00, 0x00,  // System Extension 0
    0x04, 0x00, 0x00, 0x00,  // Posture Number = 4
    0xF0, 0x00, 0x00, 0x00,  // Cluster ID = 0xF0
#   endif
})

#  else  // !POSTURE_CLUSTER_FOUR_REL

Name(CH_L, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 1 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_EQUIPMENT_LEFT },
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_GENERIC_AUDIO },
    }
})

Name(CH_R, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 2 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_EQUIPMENT_RIGHT },
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_GENERIC_AUDIO} ,
    }
})

//
// Signle channel, no rotation.
//
Name(CLF0, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-channel-count", 1 },
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package ()
    {
#   ifdef CHANNEL_LEFT
       Package (2) { "mipi-sdca-channel-1-subproperties", "CH_L" },
#   else     // !CHANNEL_LEFT
#    ifdef CHANNEL_RIGHT
       Package (2) { "mipi-sdca-channel-1-subproperties", "CH_R" },
#    else     // !CHANNEL_RIGHT
#     error "No channel relationship defined"
#    endif    // CHANNEL_RIGHT
#   endif    // CHANNEL_LEFT
    }
})

//
// Single channel, rotated 180.
//
Name(CLF1, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-channel-count", 1 },
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package ()
    {
#   ifdef CHANNEL_LEFT
       Package (2) { "mipi-sdca-channel-1-subproperties", "CH_R" },
#   else     // !CHANNEL_LEFT
#    ifdef CHANNEL_RIGHT
       Package (2) { "mipi-sdca-channel-1-subproperties", "CH_L" },
#    else     // !CHANNEL_RIGHT
#     error "No channel relationship defined"
#    endif    // CHANNEL_RIGHT
#   endif    // CHANNEL_LEFT
    }
})

// +------------------------------------+
// | Default Posture for Jamerson       |
// +------------------------------------+
Name(PMAP, Buffer()
{
    0x0b, 0x00,  // Range type 0x0b (Elevens)
#   ifdef ENABLE_POSTURE
    0x03, 0x00,  // Count of ranges = 0x3
#   else
    0x01, 0x00,  // Count of ranges = 0x1
#   endif

    // Entry 0
    0x00, 0x00, 0x00, 0x00,  // Pitch Min 0
    0x67, 0x01, 0x00, 0x00,  // Pitch Max 359
    0x00, 0x00, 0x00, 0x00,  // Roll Min
    0x00, 0x00, 0x00, 0x00,  // Roll Max (Roll 0,0 => NotRotated
    0x00, 0x00, 0x00, 0x00,  // Yaw Min 0
    0x67, 0x01, 0x00, 0x00,  // Yaw Max 359
    0x00, 0x00, 0x00, 0x00,  // Hinge Angle Min 0
    0x67, 0x01, 0x00, 0x00,  // Hinge Angle Max 359
    0x00, 0x00, 0x00, 0x00,  // System Extension 0
    0x00, 0x00, 0x00, 0x00,  // Posture Number = 0
    0xF0, 0x00, 0x00, 0x00,  // Cluster ID = 0xF0: regular channel

#   ifdef ENABLE_POSTURE
    // Entry 1
    0x00, 0x00, 0x00, 0x00,  // Pitch Min 0
    0x67, 0x01, 0x00, 0x00,  // Pitch Max 359
    0xb4, 0x00, 0x00, 0x00,  // Roll Min
    0xb4, 0x00, 0x00, 0x00,  // Roll Max (Roll 180,180 => Rotated180CounterClockwise
    0x00, 0x00, 0x00, 0x00,  // Yaw Min 0
    0x67, 0x01, 0x00, 0x00,  // Yaw Max 359
    0x00, 0x00, 0x00, 0x00,  // Hinge Angle Min 0
    0x67, 0x01, 0x00, 0x00,  // Hinge Angle Max 359
    0x00, 0x00, 0x00, 0x00,  // System Extension 0
    0x01, 0x00, 0x00, 0x00,  // Posture Number = 1
    0xF1, 0x00, 0x00, 0x00,  // Cluster ID = 0xF1: swapped channel

    // Entry 2
    0x00, 0x00, 0x00, 0x00,  // Pitch Min 0
    0x67, 0x01, 0x00, 0x00,  // Pitch Max 359
    0x00, 0x00, 0x00, 0x00,  // Roll Min 0
    0x67, 0x01, 0x00, 0x00,  // Roll Max 359
    0x00, 0x00, 0x00, 0x00,  // Yaw Min 0
    0x67, 0x01, 0x00, 0x00,  // Yaw Max 359
    0x00, 0x00, 0x00, 0x00,  // Hinge Angle Min 0
    0x00, 0x00, 0x00, 0x00,  // Hinge Angle Max 0 (Hinge 0,0 => LidClosed)
    0x00, 0x00, 0x00, 0x00,  // System Extension 0
    0x04, 0x00, 0x00, 0x00,  // Posture Number = 4
    0xF0, 0x00, 0x00, 0x00,  // Cluster ID = 0xF0: regular channel
#   endif
})

#  endif // POSTURE_CLUSTER_FOUR_REL

# endif  // JAMERSON_CLUSTER

#endif // defined ENABLE_CLUSTERS
