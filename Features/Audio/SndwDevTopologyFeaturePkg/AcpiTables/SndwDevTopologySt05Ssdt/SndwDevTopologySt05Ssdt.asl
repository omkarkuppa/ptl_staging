/** @file
  SoundWire topology definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include "common6ch.h"

// Note that if COHEN_ONLY is defined, the Jamerson
// SoundWire link and unit IDs are not to prevent their functions from
// being included in the DSDT as the Jamersons are not on the SoundWire
// bus in that particular use case.
#ifdef AIC_V2
// See Table 88 in the MIPI soundwire spec and Table 3 in the DisCo spec for data on the address.
# ifndef COHEN_ONLY
// UID_1, SNDW_3
#  define JAMERSON_1_LID              3
#  define JAMERSON_1_UID              1
#  define JAMERSON_1_LR_CHANNEL_MASK  0x01
// UID_2, SNDW_3
#  define JAMERSON_2_LID              3
#  define JAMERSON_2_UID              2
#  define JAMERSON_2_LR_CHANNEL_MASK  0x02
// UID_4, SNDW_2
#  define JAMERSON_3_LID              2
#  define JAMERSON_3_UID              4
#  define JAMERSON_3_LR_CHANNEL_MASK  0x08
// UID_5, SNDW_2
#  define JAMERSON_4_LID              2
#  define JAMERSON_4_UID              5
#  define JAMERSON_4_LR_CHANNEL_MASK  0x04
#  ifdef SIX_SPEAKERS
//  Modify channel masks for Jamerson_3&4
#   undef JAMERSON_3_LR_CHANNEL_MASK
#   undef JAMERSON_4_LR_CHANNEL_MASK
#   define JAMERSON_3_LR_CHANNEL_MASK  0x10
#   define JAMERSON_4_LR_CHANNEL_MASK  0x08
//  UID_0, SNDW_3
#   define JAMERSON_5_LID              3
#   define JAMERSON_5_UID              0
#   define JAMERSON_5_LR_CHANNEL_MASK  0x04
//  UID_3, SNDW_2
#   define JAMERSON_6_LID              2
#   define JAMERSON_6_UID              3
#   define JAMERSON_6_LR_CHANNEL_MASK  0x20
#  endif // SIX_SPEAKERS
# endif  // COHEN_ONLY
// UID_0, SNDW_0
# define COHEN_1_LID                 0
# define COHEN_1_UID                 0
#endif  // AIC_V2

DefinitionBlock ("SndwDevTopologySt05Ssdt.aml", "SSDT", 2, "INTEL", "St05Ssdt", 0x1003) {
    // references to existing objects
    External(\_SB, DeviceObj)
    External(\_SB.PC00.HDAS.IDA.SNDW, DeviceObj)
    External (SSAT, IntObj)

    Include ("Cirrus_Topology_6_Jamerson_1_Cohen.asl")
}
