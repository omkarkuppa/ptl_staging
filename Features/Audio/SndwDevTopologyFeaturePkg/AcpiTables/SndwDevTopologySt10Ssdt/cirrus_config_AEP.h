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

#ifndef _CIRRUS_CONFIG_AEP_H_
#define _CIRRUS_CONFIG_AEP_H_

// SDCA version 1.0
#define CTL_E0_FUNCTION_SDCA_VERSION_VAL        0x10
#define CTL_E0_DEVICE_SDCA_VERSION_VAL          0x10

// SDCA interface version 1.0
#define MIPI_SDW_SDCA_INTERFACE_REVISION_VAL    0x1000

//
// AMPn_LR_CHANNEL_MASK bit map
//
// Channels | Bit 0 | Bit 1 | Bit 2 | Bit 3 | Bit 4 | Bit 5
//----------+-------+-------+-------+-------+-------+-------
//    2     | Left  | Right | N/A   | N/A   | N/A   | N/A
//    4     | Left  | Left  | Right | Right | N/A   | N/A
//    6     | Left  | Left  | Left  | Right | Right | Right
//


#ifndef COHEN_ONLY
//
// Board has only 4 AMPs.
//

// AMP1, LW: UID_3
#  define AMP1_LID              2
#  define AMP1_UID              3
#  define AMP1_LR_CHANNEL_MASK  0x01
#  define AMP1_LEFT
#  define AMP1_MANUFACTURER_ID  0x01FA
#  define AMP1_PART_ID          0x3556
#  define AMP1_FUNC_ID          0x3556
#  define AMP1_AMP_DEV_NAME     "\\_SB.PC00.HDAS.IDA.SNDW.SWD2.AF01"

// AMP2, RW: UID_0
#  define AMP2_LID              2
#  define AMP2_UID              0
#  define AMP2_LR_CHANNEL_MASK  0x02
#  define AMP2_RIGHT
#  define AMP2_MANUFACTURER_ID  0x01FA
#  define AMP2_PART_ID          0x3556
#  define AMP2_FUNC_ID          0x3556
#  define AMP2_AMP_DEV_NAME     "\\_SB.PC00.HDAS.IDA.SNDW.SWD3.AF01"

// AMP3, RT: UID_4
#  define AMP3_LID              1
#  define AMP3_UID              4
#  define AMP3_LR_CHANNEL_MASK  0x08
#  define AMP3_RIGHT
#  define AMP3_MANUFACTURER_ID  0x01FA
#  define AMP3_PART_ID          0x3556
#  define AMP3_FUNC_ID          0x3556
#  define AMP3_AMP_DEV_NAME     "\\_SB.PC00.HDAS.IDA.SNDW.SWD4.AF01"

// AMP4, LT: UID_0
#  define AMP4_LID              1
#  define AMP4_UID              5
#  define AMP4_LR_CHANNEL_MASK  0x04
#  define AMP4_LEFT
#  define AMP4_MANUFACTURER_ID  0x01FA
#  define AMP4_PART_ID          0x3556
#  define AMP4_FUNC_ID          0x3556
#  define AMP4_AMP_DEV_NAME     "\\_SB.PC00.HDAS.IDA.SNDW.SWD5.AF01"

#endif // COHEN_ONLY

// UID_0, SNDW3
#define CODEC1_LID              3
#define CODEC1_UID              0
#define CODEC1_MANUFACTURER_ID  0x01FA
#define CODEC1_PART_ID          0x4243
#define CODEC1_FUNC_ID          0x4243
#define CODEC1_AMP_DEV_NAME     "\\_SB.PC00.HDAS.IDA.SNDW.SWD6.AF01"
#define CODEC1_MIC_DEV_NAME     "\\_SB.PC00.HDAS.IDA.SNDW.SWD6.AF02"
#define CODEC1_UAJ_DEV_NAME     "\\_SB.PC00.HDAS.IDA.SNDW.SWD6.AF03"
#ifdef COHEN_AMP
# define CODEC1_AMP
#endif // COHEN_AMP
#ifndef CODEC_NO_MIC
# define CODEC1_MIC
#endif
#define CODEC1_UAJ

//
// Platform specific acpi-acd-device-namestring
//
#define DSP_ACPI_ACD_DEVICE_NAMESTRING  "\\_SB.PC00.HDAS.IDA.ISSW"

//============================================================
// Number of DMIC attached to Cohen
#define CS42L43_IT11_NUM_OF_MIC         2

//============================================================
//
// Audio endpoint gain setting
// Must match the INF file volume settings when combining or
// switching between class created and/or XU created volume
// nodes.
//

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

//
// Cohen HS MIC
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
// Cohen HS Render
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
// Cohen DMIC HWKWS
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

#endif //* _CIRRUS_CONFIG_AEP_H_ *//
