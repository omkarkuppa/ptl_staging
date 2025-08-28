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

/*
 *  Main flags that defines the rest of the topology
 */
#define DISABLE_AEC                 1
#define CODEC_NO_MIC                1
#define UAJ_RENDER_192KHZ_DEFAULT   1
#define UAJ_RENDER_192KHZ           1

#include "cirrus_config_AEP.h"
#include "common.h"

DefinitionBlock ("SndwDevTopologySt10Ssdt.aml", "SSDT", 2, "INTEL", "St10Ssdt", 0x1000) {
    // references to existing objects
    External(\_SB, DeviceObj)
    External(\_SB.PC00.HDAS.IDA.SNDW, DeviceObj)

    /*Addition for Cirrus includes*/
    #include <Cirrus_Topology_8xAMP_1xCODEC.asl>
    /*End of addition for Cirrus includes*/
}
