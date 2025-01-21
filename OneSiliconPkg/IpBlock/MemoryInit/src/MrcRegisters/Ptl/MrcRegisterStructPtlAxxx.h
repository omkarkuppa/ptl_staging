#ifndef __MrcRegisterStructPtlAxxx_h__
#define __MrcRegisterStructPtlAxxx_h__

/**
@file
  This file was automatically generated. Modify at your own risk.
  Note that no error checking is done in these functions so ensure that the correct values are passed.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#pragma pack(push, 1)


typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM_CR_DDRCRVCCCLK_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM_CR_DDRCRVCCCLK_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM_CR_DDRCRVCCCLK_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRDATA_SBMEM_CR_DDRCRVCCCLK_WP3_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_STRUCT DDRDATA_SBMEM_CR_DDRCRLVRAUTOTRIM_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT DDRDATA_SBMEM_CR_DDRCRDLLCBTUNE0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_STRUCT DDRDATA_SBMEM_CR_DDRCRDLLCBTUNE1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT DDRDATA_SBMEM_CR_DDRCRTXDLLCBTUNE0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_STRUCT DDRDATA_SBMEM_CR_DDRCRTXDLLCBTUNE1_STRUCT;

typedef DDRDATA_SBMEM0_CR_PM_MISC_CTRL_STRUCT DDRDATA_SBMEM_CR_PM_MISC_CTRL_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM_CR_DDRCRVCCIOG_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM_CR_DDRCRVCCIOG_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM_CR_DDRCRVCCIOG_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRDATA_SBMEM_CR_DDRCRVCCIOG_WP3_STRUCT;

typedef MCMISCSSAUG_SB_DEBUG_STRUCT DDRDATA_SBMEM_SB_DEBUG_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVSSHI_STRUCT DDRDATA_SBMEM_CR_DDRCRVSSHI_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRDATA_SBMEM_CR_SPARE_STRUCT;

typedef DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_STRUCT DDRDATA_SBMEM_SB_DEBUG_1_0_0_MCHBAR_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRCCC_SBMEM_CR_DDRCRVCCCLK_WP0_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRCCC_SBMEM_CR_DDRCRVCCCLK_WP1_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRCCC_SBMEM_CR_DDRCRVCCCLK_WP2_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_STRUCT DDRCCC_SBMEM_CR_DDRCRVCCCLK_WP3_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_STRUCT DDRCCC_SBMEM_CR_DDRCRLVRAUTOTRIM_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_STRUCT DDRCCC_SBMEM_CR_DDRCRTXDLLCBTUNE0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_STRUCT DDRCCC_SBMEM_CR_DDRCRTXDLLCBTUNE1_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_STRUCT DDRCCC_SBMEM_CR_DDRCRLPMODE4CTRL0_STRUCT;

typedef DDRCCC_SBMEM0_CR_PM_MISC_CTRL_STRUCT DDRCCC_SBMEM_CR_PM_MISC_CTRL_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRCCC_SBMEM_CR_DDRCRVCCIOG_WP0_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRCCC_SBMEM_CR_DDRCRVCCIOG_WP1_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRCCC_SBMEM_CR_DDRCRVCCIOG_WP2_STRUCT;

typedef DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_STRUCT DDRCCC_SBMEM_CR_DDRCRVCCIOG_WP3_STRUCT;

typedef MCMISCSSAUG_SB_DEBUG_STRUCT DDRCCC_SBMEM_SB_DEBUG_STRUCT;

typedef DDRCCC_SBMEM0_CR_DDRCRVSSHI_STRUCT DDRCCC_SBMEM_CR_DDRCRVSSHI_STRUCT;

typedef DATA0CH0_CR_SPARE_STRUCT DDRCCC_SBMEM_CR_SPARE_STRUCT;
typedef union {
  struct {
    UINT32 RANK_EN                                 :  4;  // Bits 3:0
    UINT32 DRAM_TYPE                               :  4;  // Bits 7:4
    UINT32 RSVD9_8                                 :  2;  // Bits 9:8
    UINT32 CHAN_WIDTH                              :  2;  // Bits 11:10
    UINT32 RSVD12                                  :  1;  // Bits 12:12
    UINT32 DEVICE_WIDTH                            :  2;  // Bits 14:13
    UINT32 RSVD20_15                               :  6;  // Bits 20:15
    UINT32 BK_ORG                                  :  2;  // Bits 22:21
    UINT32 RSVD23                                  :  1;  // Bits 23:23
    UINT32 BL                                      :  1;  // Bits 24:24
    UINT32 RSVD30_25                               :  6;  // Bits 30:25
    UINT32 ECC_EN                                  :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_MEMCFG_STRUCT;
typedef union {
  struct {
    UINT32 TREFI                                   :  17;  // Bits 16:0
    UINT32 RSVD18_17                               :  2;  // Bits 18:17
    UINT32 TRFCAB                                  :  12;  // Bits 30:19
    UINT32 RSVD31                                  :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_DTR0_STRUCT;
typedef union {
  struct {
    UINT32 WL                                      :  8;  // Bits 7:0
    UINT32 RSVD15_8                                :  8;  // Bits 15:8
    UINT32 TRCD                                    :  7;  // Bits 22:16
    UINT32 RSVD31_23                               :  9;  // Bits 31:23
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_DTR1_STRUCT;
typedef union {
  struct {
    UINT32 TRPPB                                   :  7;  // Bits 6:0
    UINT32 RSVD7                                   :  1;  // Bits 7:7
    UINT32 TRPAB                                   :  7;  // Bits 14:8
    UINT32 RSVD15                                  :  1;  // Bits 15:15
    UINT32 TRAS                                    :  8;  // Bits 23:16
    UINT32 RSVD30_24                               :  7;  // Bits 30:24
    UINT32 AUTO_PRE_EN                             :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_DTR2_STRUCT;
typedef union {
  struct {
    UINT32 TRRD                                    :  6;  // Bits 5:0
    UINT32 RSVD23_6                                :  18;  // Bits 23:6
    UINT32 TCMD                                    :  2;  // Bits 25:24
    UINT32 DFI_2N_MODE_CTRL                        :  1;  // Bits 26:26
    UINT32 RSVD31_27                               :  5;  // Bits 31:27
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_DTR3_STRUCT;
typedef union {
  struct {
    UINT32 TRTP                                    :  8;  // Bits 7:0
    UINT32 RSVD9_8                                 :  2;  // Bits 9:8
    UINT32 TWTP                                    :  8;  // Bits 17:10
    UINT32 RSVD31_18                               :  14;  // Bits 31:18
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_DTR4_STRUCT;
typedef union {
  struct {
    UINT32 TRRSR                                   :  5;  // Bits 4:0
    UINT32 RSVD5                                   :  1;  // Bits 5:5
    UINT32 TRWSR                                   :  7;  // Bits 12:6
    UINT32 RSVD15_13                               :  3;  // Bits 15:13
    UINT32 TWWSR                                   :  5;  // Bits 20:16
    UINT32 RSVD21                                  :  1;  // Bits 21:21
    UINT32 TWRSR                                   :  8;  // Bits 29:22
    UINT32 RSVD31_30                               :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_DTR5_STRUCT;
typedef union {
  struct {
    UINT32 TRRSR_L                                 :  6;  // Bits 5:0
    UINT32 RSVD11_6                                :  6;  // Bits 11:6
    UINT32 TWWSR_L                                 :  8;  // Bits 19:12
    UINT32 RSVD21_20                               :  2;  // Bits 21:20
    UINT32 TWRSR_L                                 :  8;  // Bits 29:22
    UINT32 RSVD31_30                               :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_DTR6_STRUCT;
typedef union {
  struct {
    UINT32 TWMWSB                                  :  8;  // Bits 7:0
    UINT32 RSVD15_8                                :  8;  // Bits 15:8
    UINT32 TRPRE                                   :  3;  // Bits 18:16
    UINT32 RSVD19                                  :  1;  // Bits 19:19
    UINT32 TWPRE                                   :  3;  // Bits 22:20
    UINT32 RSVD23                                  :  1;  // Bits 23:23
    UINT32 TRPST                                   :  2;  // Bits 25:24
    UINT32 TWPST                                   :  2;  // Bits 27:26
    UINT32 RSVD28                                  :  1;  // Bits 28:28
    UINT32 RAND_TAT                                :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_DTR7_STRUCT;
typedef union {
  struct {
    UINT32 TRRDR                                   :  6;  // Bits 5:0
    UINT32 RSVD6                                   :  1;  // Bits 6:6
    UINT32 TRWDR                                   :  7;  // Bits 13:7
    UINT32 RSVD15_14                               :  2;  // Bits 15:14
    UINT32 TWWDR                                   :  7;  // Bits 22:16
    UINT32 RSVD23                                  :  1;  // Bits 23:23
    UINT32 TWRDR                                   :  6;  // Bits 29:24
    UINT32 RSVD31_30                               :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_DTR8_STRUCT;
typedef union {
  struct {
    UINT32 TPREMRBLK                               :  9;  // Bits 8:0
    UINT32 TPSTMRRBLK                              :  7;  // Bits 15:9
    UINT32 RSVD16                                  :  1;  // Bits 16:16
    UINT32 TPSTMRWBLK                              :  6;  // Bits 22:17
    UINT32 RSVD23                                  :  1;  // Bits 23:23
    UINT32 TMRR                                    :  7;  // Bits 30:24
    UINT32 RSVD31                                  :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_DTR9_STRUCT;
typedef union {
  struct {
    UINT32 TRDPDEN                                 :  9;  // Bits 8:0
    UINT32 RSVD9                                   :  1;  // Bits 9:9
    UINT32 TWRPDEN                                 :  9;  // Bits 18:10
    UINT32 RSVD31_19                               :  13;  // Bits 31:19
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_DTR10_STRUCT;
typedef union {
  struct {
    UINT32 TXP                                     :  7;  // Bits 6:0
    UINT32 RSVD7                                   :  1;  // Bits 7:7
    UINT32 TXSR                                    :  13;  // Bits 20:8
    UINT32 RSVD21                                  :  1;  // Bits 21:21
    UINT32 TXSDLL                                  :  7;  // Bits 28:22
    UINT32 RSVD31_29                               :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_DTR11_STRUCT;
typedef union {
  struct {
    UINT32 TSR                                     :  7;  // Bits 6:0
    UINT32 RSVD7                                   :  1;  // Bits 7:7
    UINT32 TCKE                                    :  7;  // Bits 14:8
    UINT32 RSVD15                                  :  1;  // Bits 15:15
    UINT32 TCKCKEL                                 :  6;  // Bits 21:16
    UINT32 RSVD23_22                               :  2;  // Bits 23:22
    UINT32 TCKCKEH                                 :  6;  // Bits 29:24
    UINT32 RSVD31_30                               :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_DTR12_STRUCT;
typedef union {
  struct {
    UINT32 TCSH                                    :  7;  // Bits 6:0
    UINT32 RSVD8_7                                 :  2;  // Bits 8:7
    UINT32 TCSL                                    :  7;  // Bits 15:9
    UINT32 RSVD31_16                               :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_DTR13_STRUCT;
typedef union {
  struct {
    UINT32 RSVD6_0                                 :  7;  // Bits 6:0
    UINT32 TZQCAL                                  :  14;  // Bits 20:7
    UINT32 RSVD21                                  :  1;  // Bits 21:21
    UINT32 TZQLAT                                  :  9;  // Bits 30:22
    UINT32 RSVD31                                  :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_DTR14_STRUCT;
typedef union {
  struct {
    UINT32 TWCKENL_FS                              :  3;  // Bits 2:0
    UINT32 RSVD3                                   :  1;  // Bits 3:3
    UINT32 TWCKENL_RD                              :  5;  // Bits 8:4
    UINT32 RSVD9                                   :  1;  // Bits 9:9
    UINT32 TWCKENL_WR                              :  5;  // Bits 14:10
    UINT32 RSVD15                                  :  1;  // Bits 15:15
    UINT32 TWCKPRE_STATIC                          :  4;  // Bits 19:16
    UINT32 RSVD20                                  :  1;  // Bits 20:20
    UINT32 TWCKSTOP                                :  4;  // Bits 24:21
    UINT32 RSVD30_25                               :  6;  // Bits 30:25
    UINT32 WCK_CTRL                                :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_DTR15_STRUCT;
typedef union {
  struct {
    UINT32 RD2WCKOFF                               :  6;  // Bits 5:0
    UINT32 RSVD6                                   :  1;  // Bits 6:6
    UINT32 WR2WCKOFF                               :  6;  // Bits 12:7
    UINT32 RSVD31_13                               :  19;  // Bits 31:13
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_DTR16_STRUCT;
typedef union {
  struct {
    UINT32 TRRDD                                   :  6;  // Bits 5:0
    UINT32 RSVD6                                   :  1;  // Bits 6:6
    UINT32 TRWDD                                   :  7;  // Bits 13:7
    UINT32 RSVD15_14                               :  2;  // Bits 15:14
    UINT32 TWWDD                                   :  7;  // Bits 22:16
    UINT32 RSVD23                                  :  1;  // Bits 23:23
    UINT32 TWRDD                                   :  6;  // Bits 29:24
    UINT32 RSVD31_30                               :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_DTR17_STRUCT;
typedef union {
  struct {
    UINT32 R0_WR_ODT_CTL                           :  3;  // Bits 2:0
    UINT32 RSVD3                                   :  1;  // Bits 3:3
    UINT32 R1_WR_ODT_CTL                           :  3;  // Bits 6:4
    UINT32 RSVD7                                   :  1;  // Bits 7:7
    UINT32 R2_WR_ODT_CTL                           :  3;  // Bits 10:8
    UINT32 RSVD11                                  :  1;  // Bits 11:11
    UINT32 R3_WR_ODT_CTL                           :  3;  // Bits 14:12
    UINT32 RSVD15                                  :  1;  // Bits 15:15
    UINT32 R0_RD_ODT_CTL                           :  3;  // Bits 18:16
    UINT32 RSVD19                                  :  1;  // Bits 19:19
    UINT32 R1_RD_ODT_CTL                           :  3;  // Bits 22:20
    UINT32 RSVD23                                  :  1;  // Bits 23:23
    UINT32 R2_RD_ODT_CTL                           :  3;  // Bits 26:24
    UINT32 RSVD27                                  :  1;  // Bits 27:27
    UINT32 R3_RD_ODT_CTL                           :  3;  // Bits 30:28
    UINT32 RSVD31                                  :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_ODT1_STRUCT;
typedef union {
  struct {
    UINT32 PDE_DLY                                 :  10;  // Bits 9:0
    UINT32 RSVD27_10                               :  18;  // Bits 27:10
    UINT32 PREA_BEFORE_PDE                         :  1;  // Bits 28:28
    UINT32 MPDE                                    :  1;  // Bits 29:29
    UINT32 MPDX                                    :  1;  // Bits 30:30
    UINT32 PD_DIS                                  :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_PD0_STRUCT;
typedef union {
  struct {
    UINT32 STOP_CK                                 :  1;  // Bits 0:0
    UINT32 RSVD2_1                                 :  2;  // Bits 2:1
    UINT32 TRI_CA                                  :  2;  // Bits 4:3
    UINT32 RSVD5                                   :  1;  // Bits 5:5
    UINT32 FORCE_CKSTOP                            :  1;  // Bits 6:6
    UINT32 CKSTOP_DLY                              :  3;  // Bits 9:7
    UINT32 RSVD31_10                               :  22;  // Bits 31:10
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DUNIT0_CR_PD2_STRUCT;



#pragma pack(pop)
#endif /* __MrcRegisterStructPtlAxxx_h__ */







