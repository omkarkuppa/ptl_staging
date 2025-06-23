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

# define CLUSTER_IT_11
# define CLUSTER_MFPU_13
# define CLUSTER_MFPU_113
# define CLUSTER_CRU_14
# define CLUSTER_MFPU_14

# define CLUSTER_ID_LIST_DMIC
# define CLUSTER_INIT_DMIC

#else

# define CLUSTER_IT_11    Package(2) {"mipi-sdca-control-0x10-subproperties", "CI11"}
# define CLUSTER_MFPU_13  Package(2) {"mipi-sdca-control-0x10-subproperties", "CI13"}
# define CLUSTER_MFPU_113 Package(2) {"mipi-sdca-control-0x10-subproperties", "CI21"}
# define CLUSTER_CRU_14   Package(2) {"mipi-sdca-control-0x10-subproperties", "CI14"}
# define CLUSTER_MFPU_14  Package(2) {"mipi-sdca-control-0x10-subproperties", "CI26"}

# define CLUSTER_ID_LIST_DMIC Package(2) {"mipi-sdca-cluster-id-list", Package() {0x11, 0x14, 0x23, 0x26, 0xF0} }

// This is one really long line, since the preprocessor will not except line extenders '\'.
# define CLUSTER_INIT_DMIC    Package(2) {"mipi-sdca-cluster-id-0x11-subproperties",  "CL11"},  Package(2) {"mipi-sdca-cluster-id-0x14-subproperties",  "CL14"},  Package(2) {"mipi-sdca-cluster-id-0x23-subproperties",  "CL23"},  Package(2) {"mipi-sdca-cluster-id-0x26-subproperties",  "CL26"},  Package (2) {"mipi-sdca-cluster-id-0xF0-subproperties", "CLF0"}

// +-----------------------+
// | Clusters for IT11     |
// | Mic Transducer Input  |
// +-----------------------+
Name(CL11, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-channel-count", 2},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package ()
    {
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHC1" },
       Package (2) { "mipi-sdca-channel-2-subproperties", "CHC2" },
    }
})

Name(CHC1, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 1 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_ARRAY },
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_GENERIC_AUDIO },
    }
})

Name(CHC2, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 2 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_ARRAY },
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_GENERIC_AUDIO },
    }
})

Name(CI11, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS },
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC },
        Package(2) { "mipi-sdca-control-dc-value", 0x1 },
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "CM11" },
    }
})

Name(CM11, Buffer()
{
    0x02, 0x00,    // Range type 0x0002
    0x01, 0x00,    // 1 rows
    0x01, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00     // ClusterIndex 1 --> ClusterID 11
})

// +-----------------------+
// | Clusters for MFPU113  |
// | Capture direct Input  |
// +-----------------------+
Name(CL23, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-channel-count", 2},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package ()
    {
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHC3" },
       Package (2) { "mipi-sdca-channel-2-subproperties", "CHC4" },
    }
})

Name(CHC3, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 1 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_MIC },
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_GENERIC_AUDIO },
    }
})

Name(CHC4, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 2 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_MIC },  // This is 0x53, but Table 40 states 0x51.
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_GENERIC_AUDIO },
    }
})

Name(CI21, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS },
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC },
        Package(2) { "mipi-sdca-control-dc-value", 0x1 },
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "CM21" },
    }
})

Name(CM21, Buffer()
{
    0x02, 0x00,    // Range type 0x0002
    0x01, 0x00,    // 1 rows
    0x01, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00     // ClusterIndex 1 --> ClusterID 21
})

// +---------------------+
// | Clusters for MFPU13 |
// |  Secondary Mic PCM  |
// +---------------------+
Name(CI13, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS },
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC },
        Package(2) { "mipi-sdca-control-dc-value", 0x1 },
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        // Point to MFPU113 since it is the same.
        Package(2) { "mipi-sdca-control-range", "CM21" },
    }
})

// +-----------------------+
// |  Clusters for CRU14   |
// |  UltraSound Capture   |
// +-----------------------+
Name(CL14, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-channel-count", 2},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package ()
    {
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHC5" },
       Package (2) { "mipi-sdca-channel-2-subproperties", "CHC6" },
    }
})

Name(CHC5, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 1 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_MIC },
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_ULTRASOUND },
    }
})

Name(CHC6, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 2 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_MIC },
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_ULTRASOUND },
    }
})

Name(CI14, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-control-access-layer", CAL_PLATFORM },
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC },
        Package(2) { "mipi-sdca-control-dc-value", 0x1 },
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "CM14" },
    }
})

Name(CM14, Buffer()
{
    0x02, 0x00,    // Range type 0x0002
    0x01, 0x00,    // 1 rows
    0x01, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00     // ClusterIndex 1 --> ClusterID 14
})

// +-----------------------+
// |  Clusters for MFPU14  |
// |  UltraSound Capture   |
// +-----------------------+
Name(CL26, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-channel-count", 2 },
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package ()
    {
       Package (2) { "mipi-sdca-channel-1-subproperties", "CHC7" },
       Package (2) { "mipi-sdca-channel-2-subproperties", "CHC8" },
    }
})

Name(CHC7, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 1 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_MIC },
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_ULTRASOUND },
    }
})

Name(CHC8, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 2 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_MIC },
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
    0x01, 0x00,    // 1 row
    0x01, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00    // ClusterIndex 01 --> ClusterID 26
}) // End CMP1

// +--------------------------+
// | Clusters for Posture L/R |
// +--------------------------+

//
// Stereo LEFT-RIGHT
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

Name(CH_L, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 1 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_GENERIC_LEFT },
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_GENERIC_AUDIO },
    }
})

Name(CH_R, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package ()
    {
       Package (2) { "mipi-sdca-cluster-channel-id", 2 },
       Package (2) { "mipi-sdca-cluster-channel-relationship", CHR_GENERIC_RIGHT },
       Package (2) { "mipi-sdca-cluster-channel-purpose", CHP_GENERIC_AUDIO} ,
    }
})

// +------------------------------------+
// | Default Posture for Cohen          |
// +------------------------------------+
Name(PMAP, Buffer()
{
    0x0B, 0x00,    // 12 32 bit entries per row
    0x01, 0x00,    // 1 row
    0x00, 0x00, 0x00, 0x00, 0x67, 0x01, 0x00, 0x00,  // Pitch Min, Max (0, 359)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // Roll Min, Max (0, 0) => NotRotated
    0x00, 0x00, 0x00, 0x00, 0x67, 0x01, 0x00, 0x00,  // Yaw Min, Max (0, 359)
    0x00, 0x00, 0x00, 0x00, 0x67, 0x01, 0x00, 0x00,  // Hinge Angle Min, Max (0, 359)
    0x00, 0x00, 0x00, 0x00,                          // System Extension Value
    0x00, 0x00, 0x00, 0x00,                          // Posture number 0
    0xF0, 0x00, 0x00, 0x00                           // Cluster id 0xF0 which is the output of PPU11
})

#endif // defined ENABLE_CLUSTERS
