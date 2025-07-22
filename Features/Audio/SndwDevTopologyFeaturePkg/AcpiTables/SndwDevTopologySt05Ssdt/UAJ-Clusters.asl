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

# define CLUSTER_IT_31
# define CLUSTER_IT_32
# define CLUSTER_IT_33
# define CLUSTER_IT_41

# define CLUSTER_ID_LIST_UAJ
# define CLUSTER_INIT_UAJ

#else

# define CLUSTER_IT_31 Package(2) {"mipi-sdca-control-0x10-subproperties", "CI31"}
# define CLUSTER_IT_32 Package(2) {"mipi-sdca-control-0x10-subproperties", "CI31"}
# define CLUSTER_IT_33 Package(2) {"mipi-sdca-control-0x10-subproperties", "CI33"}
# define CLUSTER_IT_41 Package(2) {"mipi-sdca-control-0x10-subproperties", "CI41"}


# define CLUSTER_ID_LIST_UAJ Package(2) {"mipi-sdca-cluster-id-list", Package() {0x31, 0x33, 0x41} }

// This is one really long line, since the preprocessor will not except line extenders '\'.
# define CLUSTER_INIT_UAJ    Package(2) {"mipi-sdca-cluster-id-0x31-subproperties", "CL31"},  Package(2) {"mipi-sdca-cluster-id-0x33-subproperties", "CL33"},  Package(2) {"mipi-sdca-cluster-id-0x41-subproperties", "CL41"}

// +--------------------+
// | Clusters for IT31  |
// | Stereo Jack Inputs |
// +--------------------+
Name(CL31, Package()
{
    // ClusterIndex: Cluster Library Cluster ID 2
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-channel-count", 2},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
       Package (2) {"mipi-sdca-channel-1-subproperties", "CHC1"},
       Package (2) {"mipi-sdca-channel-2-subproperties", "CHC2"},
    }
})

Name(CHC1, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
       Package (2) {"mipi-sdca-cluster-channel-id", 1},
       Package (2) {"mipi-sdca-cluster-channel-purpose", CHP_GENERIC_AUDIO},
       Package (2) {"mipi-sdca-cluster-channel-relationship", CHR_GENERIC_LEFT},
    }
})

Name(CHC2, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
       Package (2) {"mipi-sdca-cluster-channel-id", 2},
       Package (2) {"mipi-sdca-cluster-channel-purpose", CHP_GENERIC_AUDIO},
       Package (2) {"mipi-sdca-cluster-channel-relationship", CHR_GENERIC_RIGHT},
    }
})

// Stereo Mic Cluster Index
Name(CI31, Package()
{
    // Class, DC, ClusterIndex = 0x2
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 1},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "CM31"},
    }
  })

Name(CM31, Buffer()
{
    0x02, 0x00,    // Range type 0x0002
    0x01, 0x00,    // 1 rows
    0x01, 0x00, 0x00, 0x00, 0x31, 0x00, 0x00, 0x00,    // ClusterIndex 1 --> ClusterID 31
})

// +---------------------+
// |  Clusters for IT33  |
// | Headset Mic On Jack |
// +---------------------+
Name(CL33, Package()
{
    // ClusterIndex: Cluster Library Cluster ID 3
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-channel-count", 2},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
       Package (2) {"mipi-sdca-channel-1-subproperties", "CHC3"},
       Package (2) {"mipi-sdca-channel-2-subproperties", "CHC4"},
    }
})

Name(CHC3, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
       Package (2) {"mipi-sdca-cluster-channel-id", 1},
       Package (2) {"mipi-sdca-cluster-channel-purpose", CHP_GENERIC_AUDIO},
       Package (2) {"mipi-sdca-cluster-channel-relationship", CHR_GENERIC_MONO},
    }
})

Name(CHC4, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
       Package (2) {"mipi-sdca-cluster-channel-id", 2},
       Package (2) {"mipi-sdca-cluster-channel-purpose", CHP_GENERIC_AUDIO},
       Package (2) {"mipi-sdca-cluster-channel-relationship", CHR_GENERIC_MONO},
    }
})

Name(CI33, Package()
{
    // Class, DC, ClusterIndex = 0x3
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
        Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
        Package(2) { "mipi-sdca-control-dc-value", 1},
    },
    ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
    Package()
    {
        Package(2) { "mipi-sdca-control-range", "CM33"},
    }
})

Name(CM33, Buffer()
{
    0x02, 0x00,    // Range type 0x0002
    0x01, 0x00,    // 1 row
    0x01, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00,    // ClusterIndex 1 --> ClusterID 3
})

// +---------------------------+
// |    Clusters for IT41      |
// | UAJ Output Render Streams |
// +---------------------------+
Name(CL41, Package()
{
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package()
    {
        Package(2) { "mipi-sdca-channel-count", 2},
    },
    ToUUID("dbb8e3e6-5886-4ba6-8795-1319f52a966b"),
    Package () {
       Package (2) {"mipi-sdca-channel-1-subproperties", "CHR1"},
       Package (2) {"mipi-sdca-channel-2-subproperties", "CHR2"},
    }
}) //End C041

Name(CHR1, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
       Package (2) {"mipi-sdca-cluster-channel-id", 1},
       Package (2) {"mipi-sdca-cluster-channel-purpose", CHP_GENERIC_AUDIO},
       Package (2) {"mipi-sdca-cluster-channel-relationship", CHR_GENERIC_LEFT},
    }
})

Name(CHR2, Package() {
    ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
    Package () {
       Package (2) {"mipi-sdca-cluster-channel-id", 2},
       Package (2) {"mipi-sdca-cluster-channel-purpose", CHP_GENERIC_AUDIO},
       Package (2) {"mipi-sdca-cluster-channel-relationship", CHR_GENERIC_RIGHT},
    }
})

Name(CI41, Package()
{
   ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
   Package()
   {
       Package(2) { "mipi-sdca-control-access-layer", CAL_CLASS},
       Package(2) { "mipi-sdca-control-access-mode", CAM_DC},
       Package(2) { "mipi-sdca-control-dc-value", 1},
   },
   ToUUID("edb12dd0-363d-4085-a3d2-49522ca160c4"),
   Package()
   {
       Package(2) { "mipi-sdca-control-range", "CM41"},
   }
})

Name(CM41, Buffer()
{
    0x02, 0x00,    // Range type 0x0002
    0x01, 0x00,    // 1 row
    0x01, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00,    // ClusterIndex 1 --> ClusterID 41
})

#endif // defined ENABLE_CLUSTERS
