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

#include "CommonCohenOnly.h"

// GPIO for speaker selection is not used on the RVP.
#undef JAMERSON_GPIO

// Note that if COHEN_ONLY is defined, the Jamerson
// SoundWire link and unit IDs are not to prevent their functions from
// being included in the DSDT as the Jamersons are not on the SoundWire
// bus in that particular use case.
#ifdef AIC_V2
// See Table 88 in the MIPI soundwire spec and Table 3 in the DisCo spec for data on the address.
# ifndef COHEN_ONLY
// UID_1, SNDW_2
#  define JAMERSON_1_LID              2
#  define JAMERSON_1_UID              1
#  define JAMERSON_1_LR_CHANNEL_MASK  0x01
// UID_2, SNDW_2
#  define JAMERSON_2_LID              2
#  define JAMERSON_2_UID              2
#  define JAMERSON_2_LR_CHANNEL_MASK  0x02
// UID_4, SNDW_1
#  define JAMERSON_3_LID              1
#  define JAMERSON_3_UID              4
#  define JAMERSON_3_LR_CHANNEL_MASK  0x10
// UID_5, SNDW_1
#  define JAMERSON_4_LID              1
#  define JAMERSON_4_UID              5
#  define JAMERSON_4_LR_CHANNEL_MASK  0x08
#  ifdef SIX_SPEAKERS
//  Modify channel masks for Jamerson_3&4
#   undef JAMERSON_3_LR_CHANNEL_MASK
#   undef JAMERSON_4_LR_CHANNEL_MASK
#   define JAMERSON_3_LR_CHANNEL_MASK  0x10
#   define JAMERSON_4_LR_CHANNEL_MASK  0x08
//  UID_0, SNDW_2
#   define JAMERSON_5_LID              2
#   define JAMERSON_5_UID              0
#   define JAMERSON_5_LR_CHANNEL_MASK  0x04
//  UID_3, SNDW_1
#   define JAMERSON_6_LID              1
#   define JAMERSON_6_UID              3
#   define JAMERSON_6_LR_CHANNEL_MASK  0x20
#  endif // SIX_SPEAKERS
# endif  // COHEN_ONLY
// UID_0, SNDW_0
# define COHEN_1_LID                 3
# define COHEN_1_UID                 0
#else   // !AIC_V2
# ifndef COHEN_ONLY
// UID_0, SNDW_2
#  define JAMERSON_1_LID              2
#  define JAMERSON_1_UID              0
#  define JAMERSON_1_LR_CHANNEL_MASK  0x01
// UID_1, SNDW_2
#  define JAMERSON_2_LID              2
#  define JAMERSON_2_UID              1
#  define JAMERSON_2_LR_CHANNEL_MASK  0x02
// UID_4, SNDW_3
#  define JAMERSON_3_LID              3
#  define JAMERSON_3_UID              4
#  define JAMERSON_3_LR_CHANNEL_MASK  0x04
// UID_5, SNDW_3
#  define JAMERSON_4_LID              3
#  define JAMERSON_4_UID              5
#  define JAMERSON_4_LR_CHANNEL_MASK  0x08
#  ifdef SIX_SPEAKERS
//  Modify channel masks for Jamerson_3&4
#   undef JAMERSON_3_LR_CHANNEL_MASK
#   undef JAMERSON_4_LR_CHANNEL_MASK
#   define JAMERSON_3_LR_CHANNEL_MASK  0x08
#   define JAMERSON_4_LR_CHANNEL_MASK  0x10
//  UID_2, SNDW_2
#   define JAMERSON_5_LID              2
#   define JAMERSON_5_UID              2
#   define JAMERSON_5_LR_CHANNEL_MASK  0x04
//  UID_3, SNDW_3
#   define JAMERSON_6_LID              3
#   define JAMERSON_6_UID              6
#   define JAMERSON_6_LR_CHANNEL_MASK  0x20
#  endif // SIX_SPEAKERS
# endif  // COHEN_ONLY
// UID_0, SNDW_0
# define COHEN_1_LID                 3
# define COHEN_1_UID                 0
#endif  // AIC_V2

//
// Platform specific acpi-acd-device-namestring
//
#define JAMERSON_1_AMP_DEV_NAME "\\_SB.PC00.HDAS.IDA.SNDW.SWD2.AF01"
#define JAMERSON_2_AMP_DEV_NAME "\\_SB.PC00.HDAS.IDA.SNDW.SWD3.AF01"
#define JAMERSON_3_AMP_DEV_NAME "\\_SB.PC00.HDAS.IDA.SNDW.SWD4.AF01"
#define JAMERSON_4_AMP_DEV_NAME "\\_SB.PC00.HDAS.IDA.SNDW.SWD5.AF01"
#define JAMERSON_5_AMP_DEV_NAME "\\_SB.PC00.HDAS.IDA.SNDW.SWD7.AF01"
#define JAMERSON_6_AMP_DEV_NAME "\\_SB.PC00.HDAS.IDA.SNDW.SWD8.AF01"
#define COHEN_1_AMP_DEV_NAME    "\\_SB.PC00.HDAS.IDA.SNDW.SWD6.AF01"
#define COHEN_1_MIC_DEV_NAME    "\\_SB.PC00.HDAS.IDA.SNDW.SWD6.AF02"
#define COHEN_1_UAJ_DEV_NAME    "\\_SB.PC00.HDAS.IDA.SNDW.SWD6.AF03"
#define COHEN_1_HID_DEV_NAME    "\\_SB.PC00.HDAS.IDA.SNDW.SWD6.AF04"

// Uncomment or define one of these on the command line to to enable Cohen
// headset 96K or 192K playback.
//define UAJ_RENDER_96KHZ
//define UAJ_RENDER_192KHZ

//============================================================
//
// Audio endpoint gain setting
// Must match the INF file volume settings when combining or
// switching between class created and/or XU created volume
// nodes.
//

//
// HS MIC
// CS42L43_FU_36_VOL_DEFAULT =          0x0000(0dB)
// CS42L43_FU_36_VOL_MIN =              0xC000(-64dB)
// CS42L43_FU_36_VOL_MAX =              0x1E80(+30.5dB)
// CS42L43_FU_36_VOL_STEP = 0           0x8 (1/32dB)
// CS42L43_FU_36_VOL_FIXED_Q7_8 =       0x0 (0dB)
#define CS42L43_FU_36_VOL_DEFAULT       0x0
#define CS42L43_FU_36_VOL_MIN           0x00, 0xC0, 0x00, 0x00
#define CS42L43_FU_36_VOL_MAX           0x80, 0x1E, 0x00, 0x00
#define CS42L43_FU_36_VOL_STEP          0x08, 0x00, 0x00, 0x00
#define CS42L43_FU_36_VOL_FIXED_Q7_8    0x0

//
// HS Render
// CS42L43_FU_41_VOL_DEFAULT =          0xEC00(-20dB)
// CS42L43_FU_41_VOL_MIN =              0x8E00(-114dB)
// CS42L43_FU_41_VOL_MAX =              0x0(0dB)
// CS42L43_FU_41_VOL_STEP =             0x8 (1/32dB)
// CS42L43_FU_41_VOL_FIXED_Q7_8 =       0x0(0dB)
#define CS42L43_FU_41_VOL_DEFAULT       0xEC00
#define CS42L43_FU_41_VOL_MIN           0x00, 0x8E, 0x00, 0x00
#define CS42L43_FU_41_VOL_MAX           0x00, 0x00, 0x00, 0x00
#define CS42L43_FU_41_VOL_STEP          0x08, 0x00, 0x00, 0x00
#define CS42L43_FU_41_VOL_FIXED_Q7_8    0x0

//
// DMIC
// CS42L43_FU_113_VOL_DEFAULT =         0xE600(-26dB)
// CS42L43_FU_113_VOL_MIN =             0xC000(-64dB)
// CS42L43_FU_113_VOL_MAX =             0x1E80(30.5dB)
// CS42L43_FU_113_VOL_STEP =            0x8 (1/32dB)
// CS42L43_FU_113_VOL_FIXED_Q7_8 =      0xFE00 (-2dB)
#define CS42L43_FU_113_VOL_DEFAULT      0xE600
#define CS42L43_FU_113_VOL_MIN          0x00, 0xC0, 0x00, 0x00
#define CS42L43_FU_113_VOL_MAX          0x80, 0x1E, 0x00, 0x00
#define CS42L43_FU_113_VOL_STEP         0x08, 0x00, 0x00, 0x00
#define CS42L43_FU_113_VOL_FIXED_Q7_8   0xFE00
#define CS42L43_FU_113_VOL_FIXED        0xFE00  // left for legacy support

//
// DMIC HWKWS
// CS42L43_FU_13_VOL_DEFAULT =          0x0A00(10dB)
// CS42L43_FU_13_VOL_MIN =              0xC000(-64dB)
// CS42L43_FU_13_VOL_MAX =              0x0A00(10dB)
// CS42L43_FU_13_VOL_STEP =             0x8 (1/32dB)
// CS42L43_FU_13_VOL_FIXED_Q7_8 =       0xFE00 (-2dB)
#define CS42L43_FU_13_VOL_DEFAULT       0x0A00
#define CS42L43_FU_13_VOL_MIN           0x00, 0xC0, 0x00, 0x00
#define CS42L43_FU_13_VOL_MAX           0x00, 0x0A, 0x00, 0x00
#define CS42L43_FU_13_VOL_STEP          0x08, 0x00, 0x00, 0x00
#define CS42L43_FU_13_VOL_FIXED         0xFE00

//
// Cohen AMP
// CS42L43_FU_21_VOL_DEFAULT =          0x0(0dB)
// CS42L43_FU_21_VOL_MIN =              0xC000 (-64dB)
// CS42L43_FU_21_VOL_MAX =              0x0 (0.0dB)
// CS42L43_FU_21_VOL_STEP =             0x8 (1/32dB)
// CS42L43_FU_21_VOL_FIXED_Q7_8 =       0x0 (0dB)
#define CS42L43_FU_21_VOL_DEFAULT       0x0
#define CS42L43_FU_21_VOL_MIN           0x00, 0xC0, 0x00, 0x00
#define CS42L43_FU_21_VOL_MAX           0x00, 0x00, 0x00, 0x00
#define CS42L43_FU_21_VOL_STEP          0x08, 0x00, 0x00, 0x00
#define CS42L43_FU_21_VOL_FIXED_Q7_8    0x0

//
// Jamerson AMP
// CS35L56_FU_21_VOL_DEFAULT =          0x0(0dB)
// CS35L56_FU_21_VOL_MIN =              0x9800 (-102dB)
// CS35L56_FU_21_VOL_MAX =              0x0C00 (12dB)
// CS35L56_FU_21_VOL_STEP =             0x8 (1/32dB)
// CS35L56_FU_21_VOL_FIXED_Q7_8 =       0x0 (0dB)
#define CS35L56_FU_21_VOL_DEFAULT       0x0
#define CS35L56_FU_21_VOL_MIN           0x00, 0x98, 0x00, 0x00
#define CS35L56_FU_21_VOL_MAX           0x00, 0x0C, 0x00, 0x00
#define CS35L56_FU_21_VOL_STEP          0x08, 0x00, 0x00, 0x00
#define CS35L56_FU_21_VOL_FIXED_Q7_8    0x0

DefinitionBlock ("SndwDevTopologySt09Ssdt.aml", "SSDT", 2, "INTEL", "St09Ssdt", 0x1000) {
    // references to existing objects
    External(\_SB, DeviceObj)
    External(\_SB.PC00.HDAS.IDA.SNDW, DeviceObj)

#ifdef SIX_SPEAKERS
    #include <Cirrus_Topology_6_Jamerson_1_Cohen.asl>
#else
    #include <Cirrus_Topology_4_Jamerson_1_Cohen.asl>
#endif
    /*End of addition for Cirrus includes*/
}
