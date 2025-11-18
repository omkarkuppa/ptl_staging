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


// v1.01 - Extracted peripherals.
// v1.00 - Created from UpX TGL.
//
// abb 5.9.22 - Added MS Audio compositor driver to _SB Scope
Scope (_SB)
{
    Device (AUDC) // MS Audio compositor driver
    {
        Name (_HID, "ACPI0018") // INF in MS audio compositor driver looks for this HWID (UEFI-defined for audio comp.)
        // #include is used so that the preprocessor is run on the contents of the file.
        #include <SndwDevTopologySt05Ssdt/AudioComposition_Jamerson_4spk_Cohen_mic_UAJ_all_endpoints.asl>
        #include <SndwDevTopologySt05Ssdt/AudioComposition_Jamerson_6spk_Cohen_mic_UAJ.asl>
    }
}

Scope (_SB.PC00.HDAS.IDA.SNDW)
{
#ifdef JAMERSON_1_LID
    // Jamerson 1 (UID_0, SNDW_0)
    Device (SWD2)
    {
        Name (_ADR, ((0x000<<52)|(JAMERSON_1_LID << 48)|(3<<44)|(JAMERSON_1_UID << 40)|0x01FA355601) )  // _ADR: Address

        #define CHANNEL_LEFT
        #include <SndwDevTopologySt05Ssdt/CS35L56/CS35L56.asl>
        #undef CHANNEL_LEFT
    }
#endif
#ifdef JAMERSON_2_LID
    // Jamerson 2 (UID_1, SNDW_0)
    Device (SWD3)
    {
        Name (_ADR, ((0x000<<52)|( JAMERSON_2_LID << 48 )|(3<<44)|( JAMERSON_2_UID << 40 )|0x01FA355601) )  // _ADR: Address

        #define CHANNEL_LEFT
        #include <SndwDevTopologySt05Ssdt/CS35L56/CS35L56.asl>
        #undef CHANNEL_LEFT
    }
#endif
#ifdef JAMERSON_3_LID
    // Jamerson 3 (UID_4, SNDW_1)
    Device (SWD4)
    {
        Name (_ADR, ((0x000<<52)|( JAMERSON_3_LID << 48 )|(3<<44)|( JAMERSON_3_UID << 40 )|0x01FA355601) )  // _ADR: Address

        #define CHANNEL_RIGHT
        #include <SndwDevTopologySt05Ssdt/CS35L56/CS35L56.asl>
        #undef CHANNEL_RIGHT
    }
#endif
#ifdef JAMERSON_4_LID
    // Jamerson 4 (UID_5, SNDW_1)
    Device (SWD5)
    {
        Name (_ADR, ((0x000<<52)|( JAMERSON_4_LID << 48 )|(3<<44)|( JAMERSON_4_UID << 40 )|0x01FA355601) )  // _ADR: Address

        #define CHANNEL_RIGHT
        #include <SndwDevTopologySt05Ssdt/CS35L56/CS35L56.asl>
        #undef CHANNEL_RIGHT
    }
#endif

#ifdef JAMERSON_5_LID
    // Jamerson 5 (UID_2, SNDW_0)
    Device (SWD7)
    {
        Name (_ADR, ((0x000<<52)|( JAMERSON_5_LID << 48 )|(3<<44)|( JAMERSON_5_UID << 40 )|0x01FA355601) )  // _ADR: Address

        #define CHANNEL_LEFT
        #include <SndwDevTopologySt05Ssdt/CS35L56/CS35L56.asl>
        #undef CHANNEL_LEFT
    }
#endif
#ifdef JAMERSON_6_LID
    // Jamerson 6 (UID_3, SNDW_1)
    Device (SWD8)
    {
        Name (_ADR, ((0x000<<52)|( JAMERSON_6_LID << 48 )|(3<<44)|( JAMERSON_6_UID << 40 )|0x01FA355601) )  // _ADR: Address

        #define CHANNEL_RIGHT
        #include <SndwDevTopologySt05Ssdt/CS35L56/CS35L56.asl>
        #undef CHANNEL_RIGHT
    }
#endif
#ifdef COHEN_1_LID
    // Cohen UAJ [UID_0, SNDW_3]
    Device (SWD6)
    {
        Name (_ADR, ((0x000<<52)|( COHEN_1_LID << 48 )|(3<<44)|( COHEN_1_UID << 40 )|0x01FA424301) )  // _ADR: Address

        #define CHANNEL_LEFT
        #define CHANNEL_RIGHT
        #include <SndwDevTopologySt05Ssdt/CS42L43/CS42L43.asl>
        #undef CHANNEL_LEFT
        #undef CHANNEL_RIGHT
    }
#endif
}
