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


        #include <SndwDevTopologySt05Ssdt/CS42L43/CS42L43_DSD.asl>

#ifdef CODEC1_AMP
        Device (AF01)
        {
            Name (_ADR, 0x1) // SDCA Function Number = 0x1

            #include <SndwDevTopologySt05Ssdt/CS42L43/Cohen_Amp.asl>

# ifdef COHEN_BRIDGE
#  ifdef SIDECAR_GPIO_SPEAKER_SELECT
            Name (GPIA, ResourceTemplate () {
                SIDECAR_GPIO_RESOURCE_DESCRIPTOR
            })
            Method (_CRS, 0x0, NotSerialized) {
                Return (GPIA)
            }
#  endif // SIDECAR_GPIO_SPEAKER_SELECT

#  ifdef SIDECAR_VARIABLE_SPEAKER_SELECT
            // _INI method: Set 01fa-spk-id-val to the value specified in
            // SIDECAR_VARIABLE_SPEAKER_SELECT_NAME. This can either be defined
            // as the four-character constant of a field within an OpRegion (for
            // instance, of the corresponding BIOS variable set by the ODM), or,
            // for testing purposes, a constant value.
            //
            // Examples:
            //   - Source the value from a field in an OpRegion named 'SIDV'
            //   #define SIDECAR_VARIABLE_SPEAKER_SELECT_NAME     SIDV
            //   - Use 1 as the hardcoded value for the property
            //   #define SIDECAR_VARIABLE_SPEAKER_SELECT_NAME     0x01
            Method (_INI, 0x0, NotSerialized) {
                Local1 = SIDECAR_VARIABLE_SPEAKER_SELECT_NAME
                //            +-- reference to the value part of the '01fa-spk-id-val' property package
                //            |     +-- First package inside EXT0 inner package
                //            |     |   (should be the '01fa-spk-id-val' package)
                //            |     |             +-- inner package within EXT0
                //            |     |             |
                //            |     |             v--------------------v
                //            |     v----------------------------------------v
                //            v---------------------------------------------------v
                Store(Local1, Index(DerefOf(Index(DerefOf(Index(EXT0, 1)), 0)), 1))
            }
#  endif // SIDECAR_VARIABLE_SPEAKER_SELECT
# endif // COHEN_BRIDGE
        }
#endif // CODEC1_AMP

#ifdef CODEC1_MIC
        Device (AF02)
        {
            Name (_ADR, 0x2) // SDCA Function Number = 0x2

//# ifdef MIC_GEOMETRY_OVERLOAD file not included in the drop
//            #include <SndwDevTopologySt05Ssdt/Mic_Geometry_Overload.asl>
//# endif
            #include <SndwDevTopologySt05Ssdt/CS42L43/Cohen_SimpleMic.asl>

# ifdef COHEN_BRIDGE
#  ifdef SIDECAR_GPIO_SPEAKER_SELECT
            Name (GPIA, ResourceTemplate () {
                SIDECAR_GPIO_RESOURCE_DESCRIPTOR
            })
            Method (_CRS, 0x0, NotSerialized) {
                Return (GPIA)
            }
#  endif // SIDECAR_GPIO_SPEAKER_SELECT

#  ifdef SIDECAR_VARIABLE_SPEAKER_SELECT
            // _INI method: Set 01fa-spk-id-val to the value specified in
            // SIDECAR_VARIABLE_SPEAKER_SELECT_NAME. This can either be defined
            // as the four-character constant of a field within an OpRegion (for
            // instance, of the corresponding BIOS variable set by the ODM), or,
            // for testing purposes, a constant value.
            //
            // Examples:
            //   - Source the value from a field in an OpRegion named 'SIDV'
            //   #define SIDECAR_VARIABLE_SPEAKER_SELECT_NAME     SIDV
            //   - Use 1 as the hardcoded value for the property
            //   #define SIDECAR_VARIABLE_SPEAKER_SELECT_NAME     0x01
            Method (_INI, 0x0, NotSerialized) {
                Local1 = SIDECAR_VARIABLE_SPEAKER_SELECT_NAME
                //            +-- reference to the value part of the '01fa-spk-id-val' property package
                //            |     +-- First package inside EXT0 inner package
                //            |     |   (should be the '01fa-spk-id-val' package)
                //            |     |             +-- inner package within EXT0
                //            |     |             |
                //            |     |             v--------------------v
                //            |     v----------------------------------------v
                //            v---------------------------------------------------v
                Store(Local1, Index(DerefOf(Index(DerefOf(Index(EXT0, 1)), 0)), 1))
            }
#  endif // SIDECAR_VARIABLE_SPEAKER_SELECT
# endif // COHEN_BRIDGE
        }
#endif // CODEC1_MIC

#ifdef CODEC1_UAJ
        Device (AF03)
        {
            Name (_ADR, 0x3) // SDCA Hierarchical Function Number = 0x3

            #include <SndwDevTopologySt05Ssdt/CS42L43/Cohen_UAJ.asl>
            #include <SndwDevTopologySt05Ssdt/CS42L43/Cohen_UAJ_capture_enable.asl>

# ifdef COHEN_BRIDGE
#  ifdef SIDECAR_GPIO_SPEAKER_SELECT
            Name (GPIA, ResourceTemplate () {
                SIDECAR_GPIO_RESOURCE_DESCRIPTOR
            })
            Method (_CRS, 0x0, NotSerialized) {
                Return (GPIA)
            }
#  endif // SIDECAR_GPIO_SPEAKER_SELECT

#  ifdef SIDECAR_VARIABLE_SPEAKER_SELECT
            // _INI method: Set 01fa-spk-id-val to the value specified in
            // SIDECAR_VARIABLE_SPEAKER_SELECT_NAME. This can either be defined
            // as the four-character constant of a field within an OpRegion (for
            // instance, of the corresponding BIOS variable set by the ODM), or,
            // for testing purposes, a constant value.
            //
            // Examples:
            //   - Source the value from a field in an OpRegion named 'SIDV'
            //   #define SIDECAR_VARIABLE_SPEAKER_SELECT_NAME     SIDV
            //   - Use 1 as the hardcoded value for the property
            //   #define SIDECAR_VARIABLE_SPEAKER_SELECT_NAME     0x01
            Method (_INI, 0x0, NotSerialized) {
                Local1 = SIDECAR_VARIABLE_SPEAKER_SELECT_NAME
                //            +-- reference to the value part of the '01fa-spk-id-val' property package
                //            |     +-- First package inside EXT0 inner package
                //            |     |   (should be the '01fa-spk-id-val' package)
                //            |     |             +-- inner package within EXT0
                //            |     |             |
                //            |     |             v--------------------v
                //            |     v----------------------------------------v
                //            v---------------------------------------------------v
                Store(Local1, Index(DerefOf(Index(DerefOf(Index(EXT0, 1)), 0)), 1))
            }
#  endif // SIDECAR_VARIABLE_SPEAKER_SELECT
# endif // COHEN_BRIDGE
        }

        Device (AF04)
        {
            Name (_ADR, 0x4) // SDCA Hierarchical Function Number = 0x4

            #include <SndwDevTopologySt05Ssdt/CS42L43/CS_HID_headset_buttons.asl>
        }
#endif // CODEC1_UAJ
