/** @file
  Cmos Map header file.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/
#ifndef _CMOSMAP_H_
#define _CMOSMAP_H_

//
// GENERAL USAGE GUIDELINES
//

/**
  CMOS 0x00 - 0x0F are used by RTC. defined in ICHx spec
  CMOS 0x10 - 0x7F are reserved for board specific use by Intel
  CMOS 0x80 - 0xFF are reserved for OEM use only
*/

#define CmosIo_70 0x70
#define CmosIo_71 0x71
#define CmosIo_72 0x72
#define CmosIo_73 0x73

//
// PLATFORM SPECIFIC USAGE
//
#define FIT_REC_TXT_POLICY_TYPE_A 0x2A

#define FIT_REC_TXT_POLICY_TYPE_A 0x2A

#define RTC_ADDRESS_CENTURY       0x32

#define CPU_HT_POLICY             0x50
#define CPU_HT_POLICY_ENABLED     0x01

#define TPM_POLICY                0x60
#define TPM_POLICY_ENABLED        0x01
#define CMOS_TBTHR_PRESENT_ON_RESUME    0x58

#define CMOS_CPV_STATE                  0x6A

#define CMOS_TXT_POWERDOWN_REQUEST      0x2B
#define CMOS_TXT_REG                    0x6D
#define CMOS_TXT_REG1                   0x7F
#define CMOS_PLATFORM_RESET_OS          0x80
#define CMOS_CPU_RATIO_OFFSET           0x92
#define CMOS_ICH_PORT80_OFFSET          0x97
#define CMOS_OC_S3_SCRATCHPAD           0x98
#define CMOS_OC_SEND_BCLK_RAMP_MSG      0x99

#define CMOS_DATA_PORT  0x71
#define CMOS_ADDR_PORT  0x70
#define CMOS_BAD_REG    0xe

#define CMOS_MAXRATIO_CONFIG_REG            0xEF
#define CMOS_FFS_NV_HASH00_DATA_REG         0x91
#define CMOS_FFS_NV_HASH01_DATA_REG         0x81
#define CMOS_FFS_NV_HASH02_DATA_REG         0x82
#define CMOS_FFS_NV_HASH03_DATA_REG         0x83
#define CMOS_FFS_NV_HASH04_DATA_REG         0x84
#define CMOS_FFS_NV_HASH05_DATA_REG         0x85
#define CMOS_FFS_NV_HASH06_DATA_REG         0x86
#define CMOS_FFS_NV_HASH07_DATA_REG         0x87
#define CMOS_FFS_NV_HASH08_DATA_REG         0x88
#define CMOS_FFS_NV_HASH09_DATA_REG         0x89
#define CMOS_FFS_NV_HASH10_DATA_REG         0x8a
#define CMOS_FFS_NV_HASH11_DATA_REG         0x8b
#define CMOS_FFS_NV_HASH12_DATA_REG         0x8c
#define CMOS_FFS_NV_HASH13_DATA_REG         0x8d
#define CMOS_FFS_NV_HASH14_DATA_REG         0x8e
#define CMOS_FFS_NV_HASH15_DATA_REG         0x8f
#define CMOS_MEM_TEST_INFO_OFFSET           0x94

#define CMOS_FFS_NV_HASH16_DATA_REG         0xd0
#define CMOS_FFS_NV_HASH17_DATA_REG         0xd1
#define CMOS_FFS_NV_HASH18_DATA_REG         0xd2
#define CMOS_FFS_NV_HASH19_DATA_REG         0xd3
#define CMOS_FFS_NV_HASH20_DATA_REG         0xd4
#define CMOS_FFS_NV_HASH21_DATA_REG         0xd5
#define CMOS_FFS_NV_HASH22_DATA_REG         0xd6
#define CMOS_FFS_NV_HASH23_DATA_REG         0xd7
#define CMOS_FFS_NV_HASH24_DATA_REG         0xd8
#define CMOS_FFS_NV_HASH25_DATA_REG         0xd9
#define CMOS_FFS_NV_HASH26_DATA_REG         0xda
#define CMOS_FFS_NV_HASH27_DATA_REG         0xdb
#define CMOS_FFS_NV_HASH28_DATA_REG         0xdc
#define CMOS_FFS_NV_HASH29_DATA_REG         0xdd
#define CMOS_FFS_NV_HASH30_DATA_REG         0xde
#define CMOS_FFS_NV_HASH31_DATA_REG         0xdf

#define CMOS_FFS_NV_CRITBATT_OVERRIDE_REG   0xFB
#define CMOS_FFS_NV_FLAG_REG                0xBB
#define CMOS_FFS_NV_CONFIG_REG              0x51
#define CMOS_FFS_SCRAMBLER_SEED_REG0        0x41  // 16 bits
#define CMOS_FFS_SCRAMBLER_SEED_REG1        0x42  // 16 bits
#define CMOS_SERIAL_BAUD_RATE_REG           0x43
#define CMOS_SERIAL_BAUD_RATE_1_REG         0x44
#define CMOS_SERIAL_BAUD_RATE_2_REG         0x45
#define CMOS_SERIAL_BAUD_RATE_3_REG         0x46

#define CMOS_BOOT_REGISTER_REG              0x47

//
// USB4 platform settings
//
#define CMOS_USB4_CM_MODE_REG               0xF2
#define CMOS_USB4_CM_MODE_DEFAULT_VALUE     1

//
// Post Code value to be break at
//
#define CMOS_POST_CODE_BREAK_REG            0x48
#define CMOS_POST_CODE_BREAK_1_REG          0x49
#define CMOS_POST_CODE_BREAK_2_REG          0x4A
#define CMOS_POST_CODE_BREAK_3_REG          0x4B

//
// Debug Mask saved in CMOS
//
#define CMOS_DEBUG_PRINT_LEVEL_REG          0x4C
#define CMOS_DEBUG_PRINT_LEVEL_1_REG        0x4D
#define CMOS_DEBUG_PRINT_LEVEL_2_REG        0x4E
#define CMOS_DEBUG_PRINT_LEVEL_3_REG        0x4F

//
// USB debug port config
//
#define USB_DEBUG_PORT_BUS_REG              0x51
#define USB_DEBUG_PORT_DEVICE_REG           0x52
#define USB_DEBUG_PORT_FUNCTION_REG         0x53
#define USB_DEBUG_PORT_TEMP_RAM_REG         0x54
#define USB_DEBUG_PORT_TEMP_RAM_1_REG       0x55
#define USB_DEBUG_PORT_TEMP_RAM_2_REG       0x56
#define USB_DEBUG_PORT_TEMP_RAM_3_REG       0x57

//
// USB3 debug port config
//
#define USB3_DEBUG_PORT_BUS_REG             0x59
#define USB3_DEBUG_PORT_DEVICE_REG          0x5A
#define USB3_DEBUG_PORT_FUNCTION_REG        0x5B
#define USB3_DEBUG_XHC_MODE                 0x5C

#define CMOS_DEBUG_PRINT_ENDPOINTS          0x5E

#define CMOS_FAST_BOOT_REG                  0x6B

//
// LT related
//
#define OEM_SSP_SV_LT_CMOS_VALID_FLAG_REG   0xA0
#define OEM_SSP_SV_LT_CMOS_VALID_FLAG_1_REG 0xA1
#define OEM_SSP_SV_LT_CMOS_VALID_FLAG_2_REG 0xA2
#define OEM_SSP_SV_LT_CMOS_VALID_FLAG_3_REG 0xA3

#define OEM_SSP_SV_LT_MEMORY_LOCKED_REG     0xA4
#define OEM_SSP_SV_LT_MEMORY_LOCKED_1_REG   0xA5
#define OEM_SSP_SV_LT_MEMORY_LOCKED_2_REG   0xA6
#define OEM_SSP_SV_LT_MEMORY_LOCKED_3_REG   0xA7

#define OEM_SSP_SV_LT_STS_REG     0xA8
#define OEM_SSP_SV_LT_STS_1_REG   0xA9
#define OEM_SSP_SV_LT_STS_2_REG   0xAA
#define OEM_SSP_SV_LT_STS_3_REG   0xAB

#define OEM_SSP_SV_LT_ESTS_REG    0xAC
#define OEM_SSP_SV_LT_ESTS_1_REG  0xAD
#define OEM_SSP_SV_LT_ESTS_2_REG  0xAE
#define OEM_SSP_SV_LT_ESTS_3_REG  0xAF

#define OEM_SSP_SV_LT_CRASH0_REG    0xB0
#define OEM_SSP_SV_LT_CRASH0_1_REG  0xB1
#define OEM_SSP_SV_LT_CRASH0_2_REG  0xB2
#define OEM_SSP_SV_LT_CRASH0_3_REG  0xB3

#define OEM_SSP_FREE_REG_B4  0xB4
#define OEM_SSP_FREE_REG_B5  0xB5
#define OEM_SSP_FREE_REG_B6  0xB6
#define OEM_SSP_FREE_REG_B7  0xB7

#define OEM_SSP_FREE_REG_B8  0xB8
#define OEM_SSP_FREE_REG_B9  0xB9
#define OEM_SSP_FREE_REG_BA  0xBA
#define OEM_SSP_FREE_REG_BB  0xBB

#define OEM_SSP_FREE_REG_BC  0xBC
#define OEM_SSP_FREE_REG_BD  0xBD
#define OEM_SSP_FREE_REG_BE  0xBE
#define OEM_SSP_FREE_REG_BF  0xBF

#define SSP_PCH_SV_CMOS_REG_C0  0xC0
#define SSP_PCH_SV_CMOS_REG_C1  0xC1
#define SSP_PCH_SV_CMOS_REG_C2  0xC2
#define SSP_PCH_SV_CMOS_REG_C3  0xC3
#define SSP_PCH_SV_CMOS_REG_C4  0xC4
#define SSP_PCH_SV_CMOS_REG_C5  0xC5
#define SSP_PCH_SV_CMOS_REG_C6  0xC6
#define SSP_PCH_SV_CMOS_REG_C7  0xC7
#define SSP_PCH_SV_CMOS_REG_C8  0xC8
#define SSP_PCH_SV_CMOS_REG_C9  0xC9
#define SSP_PCH_SV_CMOS_REG_CA  0xCA
#define SSP_PCH_SV_CMOS_REG_CB  0xCB

#define SSP_XMLCLI_SIG_CMOS_REG_CC  0xCC
#define SSP_XMLCLI_SIG_CMOS_REG_CD  0xCD
#define SSP_XMLCLI_SIG_CMOS_REG_CE  0xCE
#define SSP_XMLCLI_SIG_CMOS_REG_CF  0xCF

#define SSP_PCH_SV_CMOS_REG_E0  0xE0


#define SSP_PCH_BIOS_CMOS_REG_D0  0xD0
#define SSP_PCH_BIOS_CMOS_REG_D1  0xD1
#define SSP_PCH_BIOS_CMOS_REG_D2  0xD2
#define SSP_PCH_BIOS_CMOS_REG_D3  0xD3
#define SSP_PCH_BIOS_CMOS_REG_D4  0xD4
#define SSP_PCH_BIOS_CMOS_REG_D5  0xD5
#define SSP_PCH_BIOS_CMOS_REG_D6  0xD6
#define SSP_PCH_BIOS_CMOS_REG_D7  0xD7
#define SSP_PCH_BIOS_CMOS_REG_D8  0xD8
#define SSP_PCH_BIOS_CMOS_REG_D9  0xD9
#define SSP_PCH_BIOS_CMOS_REG_DA  0xDA
#define SSP_PCH_BIOS_CMOS_REG_DB  0xDB
#define SSP_PCH_BIOS_CMOS_REG_DC  0xDC
#define SSP_PCH_BIOS_CMOS_REG_DD  0xDD
#define SSP_PCH_BIOS_CMOS_REG_DE  0xDE
#define SSP_PCH_BIOS_CMOS_REG_DF  0xDF

//
// Shared mailbox address 20 bits (in 4K alignment [31:12])
//
#define OEM_DRAM_SHARED_MAILBOX_REG_F0  0xF0
#define OEM_DRAM_SHARED_MAILBOX_REG_F1  0xF1

#define OEM_SSP_FREE_REG_F2  0xF2
#define OEM_SSP_FREE_REG_F3  0xF3
#define OEM_SSP_FREE_REG_F4  0xF4
#define OEM_SSP_FREE_REG_F5  0xF5
#define OEM_SSP_FREE_REG_F6  0xF6
#define OEM_SSP_FREE_REG_F7  0xF7

#define Q_SV_RESERVED_REG_F9      0xF9
#define Q_SV_CHECKSUM_LSB_REG     0x6E
#define Q_SV_CHECKSUM_MSB_REG     0x6F
#define Q_SV_NOT_IN_USE_REG       0x91

#define DEFAULT_VALUE         0

#endif
