/** @file
  SPD processing header file.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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
#ifndef MrcSpdProcessingDefs_h_
#define MrcSpdProcessingDefs_h_

#include "CMrcInterface.h"  // for MrcParameters

#pragma pack (push, 1)

///
/// Local definitions
///
#define CRC_SEED                  0
#define CRC_XOR_MASK              0x1021
#define TREFIMIN_LPDDR            3904000   /// Average periodic refresh interval, in picoseconds (3.904 us for LPDDR5)
#define TREFIMIN_DDR5             1950000   /// Average periodic refresh interval, in picoseconds (1.95 us for DDR5)
#define TREFIMULTIPLIER           1000      /// tREFI value defined in XMP 1.3 spec is actually in thousands of MTB units.

/// DDR5 tRRD_L min timing in picoseconds
#define MRC_DDR5_TRRD_L_6400_MIN        (5000)
#define MRC_DDR5_TRRD_L_6800_MIN        (4705)
#define MRC_DDR5_TRRD_L_7200_MIN        (4444)
#define MRC_DDR5_TRRD_L_7600_MIN        (4210)
#define MRC_DDR5_TRRD_L_8000_8400_MIN   (4000)
#define MRC_DDR5_TRRD_L_8800_MIN        (3863)

// DDR5 tFAW min timing in picoseconds
#define MRC_DDR5_TFAW_1K_3200_MIN       (20000)
#define MRC_DDR5_TFAW_2K_3200_MIN       (25000)
#define MRC_DDR5_TFAW_1K_3600_MIN       (17777)
#define MRC_DDR5_TFAW_2K_3600_MIN       (22222)
#define MRC_DDR5_TFAW_1K_4000_MIN       (16000)
#define MRC_DDR5_TFAW_2K_4000_MIN       (20000)
#define MRC_DDR5_TFAW_1K_4400_MIN       (14545)
#define MRC_DDR5_TFAW_2K_4400_MIN       (18181)
#define MRC_DDR5_TFAW_1K_4800_MIN       (13333)
#define MRC_DDR5_TFAW_2K_4800_MIN       (16666)
#define MRC_DDR5_TFAW_1K_5200_MIN       (12307)
#define MRC_DDR5_TFAW_2K_5200_MIN       (15384)
#define MRC_DDR5_TFAW_1K_5600_MIN       (11428)
#define MRC_DDR5_TFAW_2K_5600_MIN       (14285)
#define MRC_DDR5_TFAW_1K_6000_MIN       (10666)
#define MRC_DDR5_TFAW_2K_6000_MIN       (13333)
#define MRC_DDR5_TFAW_1K_6400_MIN       (10000)
#define MRC_DDR5_TFAW_2K_6400_MIN       (12500)
#define MRC_DDR5_TFAW_1K_6800_MIN       (9411)
#define MRC_DDR5_TFAW_2K_6800_MIN       (11764)
#define MRC_DDR5_TFAW_1K_7200_MIN       (8888)
#define MRC_DDR5_TFAW_2K_7200_MIN       (11111)
#define MRC_DDR5_TFAW_1K_7600_MIN       (8421)
#define MRC_DDR5_TFAW_2K_7600_MIN       (10526)
#define MRC_DDR5_TFAW_1K_8000_MIN       (8000)
#define MRC_DDR5_TFAW_2K_8000_MIN       (10000)
#define MRC_DDR5_TFAW_1K_8400_MIN       (7619)
#define MRC_DDR5_TFAW_2K_8400_MIN       (9523)
#define MRC_DDR5_TFAW_1K_8800_MIN       (7272)
#define MRC_DDR5_TFAW_2K_8800_MIN       (9090)

#define MRC_FREQUENCY_MTB_OFFSET  1000000
#define MRC_FREQUENCY_FTB_OFFSET  1000

///
/// SPD field definitions
///
#define MRC_SPD_DDR5_SDRAM_TYPE_NUMBER      (0x12)
#define MRC_SPD_LPDDR5_SDRAM_TYPE_NUMBER    (0x13)
#define MRC_SPD_LPDDR5X_SDRAM_TYPE_NUMBER   (0x15)
#define MRC_SPD_LPDDR_CL_SUPPORTED_MASK     (0x157DFF)

#define MRC_SPD_RDIMM_TYPE_NUMBER           (0x01)
#define MRC_SPD_UDIMM_TYPE_NUMBER           (0x02)
#define MRC_SPD_SODIMM_TYPE_NUMBER          (0x03)

#define MRC_SPD_LPDDR_SDRAM_BANK_4          (0x00)
#define MRC_SPD_LPDDR_SDRAM_BANK_8          (0x01)
#define MRC_SPD_LPDDR_SDRAM_BANK_16         (0x02)

#define MRC_SPD_DDR5_SDRAM_BANK_1           (0x00)
#define MRC_SPD_DDR5_SDRAM_BANK_2           (0x01)
#define MRC_SPD_DDR5_SDRAM_BANK_4           (0x02)

#define MRC_SPD_LPDDR5_SDRAM_BANK_GROUPS_1  (0x00)
#define MRC_SPD_LPDDR5_SDRAM_BANK_GROUPS_2  (0x01)
#define MRC_SPD_LPDDR5_SDRAM_BANK_GROUPS_4  (0x02)

#define MRC_SPD_DDR5_SDRAM_BANK_GROUPS_1    (0x00)
#define MRC_SPD_DDR5_SDRAM_BANK_GROUPS_2    (0x01)
#define MRC_SPD_DDR5_SDRAM_BANK_GROUPS_4    (0x02)
#define MRC_SPD_DDR5_SDRAM_BANK_GROUPS_8    (0x03)

#define MRC_SPD_SDRAM_DENSITY_256Mb         (0x00)
#define MRC_SPD_SDRAM_DENSITY_512Mb         (0x01)
#define MRC_SPD_SDRAM_DENSITY_1Gb           (0x02)
#define MRC_SPD_SDRAM_DENSITY_2Gb           (0x03)
#define MRC_SPD_SDRAM_DENSITY_4Gb           (0x04)
#define MRC_SPD_SDRAM_DENSITY_8Gb           (0x05)
#define MRC_SPD_SDRAM_DENSITY_16Gb          (0x06)
#define MRC_SPD_SDRAM_DENSITY_32Gb          (0x07)

#define MRC_SPD_LPDDR_SDRAM_DENSITY_12Gb    (0x08)
#define MRC_SPD_LPDDR_SDRAM_DENSITY_24Gb    (0x09)
#define MRC_SPD_LPDDR_SDRAM_DENSITY_3Gb     (0x0A)
#define MRC_SPD_LPDDR_SDRAM_DENSITY_6Gb     (0x0B)
#define MRC_SPD_LPDDR_SDRAM_DENSITY_18Gb    (0x0C)

#define MRC_SPD5_SDRAM_DENSITY_NONE         (0x00)
#define MRC_SPD5_SDRAM_DENSITY_4Gb          (0x01)
#define MRC_SPD5_SDRAM_DENSITY_8Gb          (0x02)
#define MRC_SPD5_SDRAM_DENSITY_12Gb         (0x03)
#define MRC_SPD5_SDRAM_DENSITY_16Gb         (0x04)
#define MRC_SPD5_SDRAM_DENSITY_24Gb         (0x05)
#define MRC_SPD5_SDRAM_DENSITY_32Gb         (0x06)
#define MRC_SPD5_SDRAM_DENSITY_48Gb         (0x07)
#define MRC_SPD5_SDRAM_DENSITY_64Gb         (0x08)

#define MRC_SPD_SDRAM_ROW_12                (0x00)
#define MRC_SPD_SDRAM_ROW_13                (0x01)
#define MRC_SPD_SDRAM_ROW_14                (0x02)
#define MRC_SPD_SDRAM_ROW_15                (0x03)
#define MRC_SPD_SDRAM_ROW_16                (0x04)
#define MRC_SPD_SDRAM_ROW_17                (0x05)
#define MRC_SPD_SDRAM_ROW_18                (0x06)

#define MRC_SPD5_SDRAM_ROW_16               (0x00)
#define MRC_SPD5_SDRAM_ROW_17               (0x01)
#define MRC_SPD5_SDRAM_ROW_18               (0x02)

#define MRC_SPD_SDRAM_COLUMN_9              (0x00)
#define MRC_SPD_SDRAM_COLUMN_10             (0x01)
#define MRC_SPD_SDRAM_COLUMN_11             (0x02)
#define MRC_SPD_SDRAM_COLUMN_12             (0x03)

#define MRC_SPD5_SDRAM_COLUMN_10            (0x00)
#define MRC_SPD5_SDRAM_COLUMN_11            (0x02)

#define MRC_SPD_SDRAM_DEVICE_WIDTH_4        (0x00)
#define MRC_SPD_SDRAM_DEVICE_WIDTH_8        (0x01)
#define MRC_SPD_SDRAM_DEVICE_WIDTH_16       (0x02)
#define MRC_SPD_SDRAM_DEVICE_WIDTH_32       (0x03)

#define MRC_SPD_PRIMARY_BUS_WIDTH_8         (0x00)
#define MRC_SPD_PRIMARY_BUS_WIDTH_16        (0x01)
#define MRC_SPD_PRIMARY_BUS_WIDTH_32        (0x02)
#define MRC_SPD_PRIMARY_BUS_WIDTH_64        (0x03)

#define MRC_SPD_BUS_WIDTH_EXTENSION_0       (0x00)
#define MRC_SPD_BUS_WIDTH_EXTENSION_8       (0x01)

#define MRC_SPD5_BUS_WIDTH_EXTENSION_0      (0x00)
#define MRC_SPD5_BUS_WIDTH_EXTENSION_4      (0x01)
#define MRC_SPD5_BUS_WIDTH_EXTENSION_8      (0x02)

#define MRC_SPD_CH_PER_SDRAM_PKG_1          (0x0)
#define MRC_SPD_CH_PER_SDRAM_PKG_2          (0x1)
#define MRC_SPD_CH_PER_SDRAM_PKG_4          (0x2)
#define MRC_SPD_CH_PER_SDRAM_PKG_RSVD       (0x3)

#define MRC_SPD_CL_SUPPORTED_MASK           (0x7FFF)

#define MRC_SPD5_DEVICE_INSTALLED           (0x1)

#define XMP_VDD_INTEGER                     (1000)
#define XMP_VDD_INCREMENT                   (50)
#define XMP_VDD_INCREMENT_2                 (10)
#define XMP_VDD_MIN_POSSIBLE                (1000)
#define XMP_VDD_MAX_POSSIBLE                (2000)
#define XMP3_VDD_MIN_POSSIBLE               (1000)
#define XMP3_VDD_MAX_POSSIBLE               (2950)

#define MRC_TMAC_UNTESTED                   (0x0)
#define MRC_TMAC_700K                       (0x1)
#define MRC_TMAC_600K                       (0x2)
#define MRC_TMAC_500K                       (0x3)
#define MRC_TMAC_400K                       (0x4)
#define MRC_TMAC_300K                       (0x5)
#define MRC_TMAC_200K                       (0x6)
#define MRC_TMAC_UNLIMITED                  (0x8)

#define MRC_SPD_REF_RAW_CARD_SIZE           (5)
#define MRC_SPD_REF_RAW_CARD_EXT_OFFSET     (7)

#define XMP_ID_STRING                       (0x4A0C)

#define MICRON_DRAM_ID                      (0x2C80)
#define SAMSUNG_DRAM_ID                     (0xCE80)
#define SKHYNIX_DRAM_ID                     (0xAD80)

// MTB in units of 0.125ns
#define NS_TO_MTB_FACTOR                    (8)

typedef enum {
  MrcDensity256Mb = MRC_SPD_SDRAM_DENSITY_256Mb,
  MrcDensity512Mb = MRC_SPD_SDRAM_DENSITY_512Mb,
  MrcDensity1Gb   = MRC_SPD_SDRAM_DENSITY_1Gb,
  MrcDensity2Gb   = MRC_SPD_SDRAM_DENSITY_2Gb,
  MrcDensity4Gb   = MRC_SPD_SDRAM_DENSITY_4Gb,
  MrcDensity8Gb   = MRC_SPD_SDRAM_DENSITY_8Gb,
  MrcDensity16Gb  = MRC_SPD_SDRAM_DENSITY_16Gb,
  MrcDensity32Gb  = MRC_SPD_SDRAM_DENSITY_32Gb,
  MrcDensity12Gb  = MRC_SPD_LPDDR_SDRAM_DENSITY_12Gb,
  MrcDensity24Gb  = MRC_SPD_LPDDR_SDRAM_DENSITY_24Gb,
  MrcDensity3Gb   = MRC_SPD_LPDDR_SDRAM_DENSITY_3Gb,
  MrcDensity6Gb   = MRC_SPD_LPDDR_SDRAM_DENSITY_6Gb,
  MrcDensity18Gb  = MRC_SPD_LPDDR_SDRAM_DENSITY_18Gb,
  MrcDensity48Gb  = 13,
  MrcDensity64Gb  = 14,
  MrcDensityMax
} MRC_SDRAM_DENSITY;

typedef struct {
  UINT32        tCK;
  MrcFrequency  Frequency;
} TRangeTable;

typedef struct {
  BOOLEAN (*mrc_task) (MrcParameters * const MrcData);
} SpdTimeCallTable;

typedef struct {
  BOOLEAN (*mrc_task) (MrcParameters * const MrcData, const MrcSpd * const Spd, MrcDimmOut * const DimmOut);
} SpdRecogCallTable;

extern const UINT32 SdramCapacityTable[MrcDensityMax];

// Check if density is power of 2
#ifndef IS_CAPACITY_POWER2_DENSITY_INDEX_CHECK
#define IS_CAPACITY_POWER2_DENSITY_INDEX_CHECK(DensityIndex)      (BOOLEAN)((SdramCapacityTable[DensityIndex] & (SdramCapacityTable[DensityIndex] - 1)) == 0)
#endif

#pragma pack (pop)
#endif // MrcSpdProcessingnDefs_h_
