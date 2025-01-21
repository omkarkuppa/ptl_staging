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

# define CLUSTER_IT_21    Package(2) {"mipi-sdca-control-0x10-subproperties", "CI21"}
# define CLUSTER_IT_26    Package(2) {"mipi-sdca-control-0x10-subproperties", "CI26"}
# define CLUSTER_IT_29    Package(2) {"mipi-sdca-control-0x10-subproperties", "CI26"}
# define CLUSTER_MFPU_26  Package(2) {"mipi-sdca-control-0x10-subproperties", "CI26"}
# define CLUSTER_UDMPU_23 Package(2) {"mipi-sdca-control-0x10-subproperties", "CI23"}
# define CLUSTER_UDMPU_25 Package(2) {"mipi-sdca-control-0x10-subproperties", "CI25"}

# define CLUSTER_ID_LIST_JAMERSON Package (2) {"mipi-sdca-cluster-id-list", Package() {0x1, 0x21, 0x23, 0x25, 0x26, 0x29, 0xF0} }

// This is one really long line, since the preprocessor will not except line extenders '\'.
# define CLUSTER_INIT_JAMERSON    Package (2) {"mipi-sdca-cluster-id-0x1-subproperties",  "CL29"}, Package (2) {"mipi-sdca-cluster-id-0x21-subproperties", "CL21"}, Package (2) {"mipi-sdca-cluster-id-0x23-subproperties", "CL23"}, Package (2) {"mipi-sdca-cluster-id-0x25-subproperties", "CL25"}, Package (2) {"mipi-sdca-cluster-id-0x26-subproperties", "CL26"}, Package (2) {"mipi-sdca-cluster-id-0x29-subproperties", "CL29"}, Package (2) {"mipi-sdca-cluster-id-0xF0-subproperties", "CLF0"}

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
#ifndef CHANNEL_STEREO
Name(CLF0, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-channel-count", 1 },
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
#ifdef CHANNEL_LEFT
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHR1" },
#else
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHR2" },
#endif
    }
})
#else
// Switch the left and right channels.
Name(CLF0, Package()
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
#endif

// +------------------------------------+
// | Default Posture for Jamerson/Cohen |
// +------------------------------------+
Name(PMAP, Package()
{
    0x0B, 0x00,    // 12 32 bit entries per row
    0x01, 0x00,    // 1 row
    0x00, 0x00, 0x00, 0x00, 0x67, 0x01, 0x00, 0x00,  // Pitch Min, Max (0, 359)
    0x00, 0x00, 0x00, 0x00, 0x67, 0x01, 0x00, 0x00,  // Roll Min, Max (0, 359)
    0x00, 0x00, 0x00, 0x00, 0x67, 0x01, 0x00, 0x00,  // Yaw Min, Max (0, 359)
    0x00, 0x00, 0x00, 0x00, 0x67, 0x01, 0x00, 0x00,  // Hinge Angle Min, Max (0, 359)
    0x00, 0x00, 0x00, 0x00,                          // System Extension Value
    0x01, 0x00, 0x00, 0x00,                          // Posture number 1
    0xF0, 0x00, 0x00, 0x00                           // Cluster id 0xF0 which is the output of PPU21
})

#endif // defined ENABLE_CLUSTERS
