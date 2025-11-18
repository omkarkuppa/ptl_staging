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


        #include <SndwDevTopologySt05Ssdt/CS42L45/CS42L45_DSD.asl>

#ifdef CODEC1_MIC
        Device (AF02)
        {
            Name (_ADR, 0x2) // SDCA Function Number = 0x2

# ifdef MIC_GEOMETRY_OVERLOAD
            #include <SndwDevTopologySt05Ssdt/Mic_Geometry_Overload.asl>
# endif
            #include <SndwDevTopologySt05Ssdt/CS42L45/Phife_SimpleMic.asl>
        }
#endif // CODEC1_MIC

#ifdef CODEC1_UAJ
        Device (AF03)
        {
            Name (_ADR, 0x3) // SDCA Hierarchical Function Number = 0x3

            #include <SndwDevTopologySt05Ssdt/CS42L45/Phife_UAJ.asl>
            #include <SndwDevTopologySt05Ssdt/CS42L45/Phife_UAJ_capture_enable.asl>
        }

        Device (AF04)
        {
            Name (_ADR, 0x4) // SDCA Hierarchical Function Number = 0x4

            #include <SndwDevTopologySt05Ssdt/CS42L45/Phife_HID_headset_buttons.asl>
        }
#endif // CODEC1_UAJ
