#ifndef __MrcRegisterPtlAxxx_h__
#define __MrcRegisterPtlAxxx_h__

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


#define DDRDATA_SBMEM_CR_DDRCRVCCCLK_WP0_REG                           (0x0000A020)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM_CR_DDRCRVCCCLK_WP1_REG                           (0x0000A024)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM_CR_DDRCRVCCCLK_WP2_REG                           (0x0000A028)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM_CR_DDRCRVCCCLK_WP3_REG                           (0x0000A02C)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM_CR_DDRCRLVRAUTOTRIM_REG                          (0x0000A030)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_REG

#define DDRDATA_SBMEM_CR_DDRCRDLLCBTUNE0_REG                           (0x0000A034)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG

#define DDRDATA_SBMEM_CR_DDRCRDLLCBTUNE1_REG                           (0x0000A038)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_REG

#define DDRDATA_SBMEM_CR_DDRCRTXDLLCBTUNE0_REG                         (0x0000A03C)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG

#define DDRDATA_SBMEM_CR_DDRCRTXDLLCBTUNE1_REG                         (0x0000A040)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_REG

#define DDRDATA_SBMEM_CR_PM_MISC_CTRL_REG                              (0x0000A044)
//Duplicate of DDRDATA_SBMEM0_CR_PM_MISC_CTRL_REG

#define DDRDATA_SBMEM_CR_DDRCRVCCIOG_WP0_REG                           (0x0000A048)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM_CR_DDRCRVCCIOG_WP1_REG                           (0x0000A04C)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM_CR_DDRCRVCCIOG_WP2_REG                           (0x0000A050)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM_CR_DDRCRVCCIOG_WP3_REG                           (0x0000A054)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM_SB_DEBUG_REG                                     (0x0000A058)
//Duplicate of MCMISCSSAUG_SB_DEBUG_REG

#define DDRDATA_SBMEM_CR_DDRCRVSSHI_REG                                (0x0000A05C)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVSSHI_REG

#define DDRDATA_SBMEM_CR_SPARE_REG                                     (0x0000A060)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DDRDATA_SBMEM_SB_DEBUG_1_0_0_MCHBAR_REG                        (0x0000A064)
//Duplicate of DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_REG

#define DDRCCC_SBMEM_CR_DDRCRVCCCLK_WP0_REG                            (0x0000A0A0)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRCCC_SBMEM_CR_DDRCRVCCCLK_WP1_REG                            (0x0000A0A4)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRCCC_SBMEM_CR_DDRCRVCCCLK_WP2_REG                            (0x0000A0A8)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRCCC_SBMEM_CR_DDRCRVCCCLK_WP3_REG                            (0x0000A0AC)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRCCC_SBMEM_CR_DDRCRLVRAUTOTRIM_REG                           (0x0000A0B0)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_REG

#define DDRCCC_SBMEM_CR_DDRCRTXDLLCBTUNE0_REG                          (0x0000A0B4)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG

#define DDRCCC_SBMEM_CR_DDRCRTXDLLCBTUNE1_REG                          (0x0000A0B8)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_REG

#define DDRCCC_SBMEM_CR_DDRCRLPMODE4CTRL0_REG                          (0x0000A0BC)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_REG

#define DDRCCC_SBMEM_CR_PM_MISC_CTRL_REG                               (0x0000A0C0)
//Duplicate of DDRCCC_SBMEM0_CR_PM_MISC_CTRL_REG

#define DDRCCC_SBMEM_CR_DDRCRVCCIOG_WP0_REG                            (0x0000A0C4)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRCCC_SBMEM_CR_DDRCRVCCIOG_WP1_REG                            (0x0000A0C8)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRCCC_SBMEM_CR_DDRCRVCCIOG_WP2_REG                            (0x0000A0CC)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRCCC_SBMEM_CR_DDRCRVCCIOG_WP3_REG                            (0x0000A0D0)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRCCC_SBMEM_SB_DEBUG_REG                                      (0x0000A0D4)
//Duplicate of MCMISCSSAUG_SB_DEBUG_REG

#define DDRCCC_SBMEM_CR_DDRCRVSSHI_REG                                 (0x0000A0D8)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVSSHI_REG

#define DDRCCC_SBMEM_CR_SPARE_REG                                      (0x0000A0DC)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DUNIT0_CR_MEMCFG_REG                                           (0x0000AF80)
#define DUNIT0_CR_MEMCFG_DEF                                           (0x00000400)
#define DUNIT0_CR_MEMCFG_VOLATILE_BITFIELDS_MSK                        (0x00000000)

  #define DUNIT0_CR_MEMCFG_RANK_EN_OFF                                 ( 0)
  #define DUNIT0_CR_MEMCFG_RANK_EN_WID                                 ( 4)
  #define DUNIT0_CR_MEMCFG_RANK_EN_MSK                                 (0x0000000F)
  #define DUNIT0_CR_MEMCFG_RANK_EN_MIN                                 (0)
  #define DUNIT0_CR_MEMCFG_RANK_EN_MAX                                 (15) // 0x0000000F
  #define DUNIT0_CR_MEMCFG_RANK_EN_DEF                                 (0x00000000)
  #define DUNIT0_CR_MEMCFG_RANK_EN_HSH                                 (0x0400AF80)

  #define DUNIT0_CR_MEMCFG_DRAM_TYPE_OFF                               ( 4)
  #define DUNIT0_CR_MEMCFG_DRAM_TYPE_WID                               ( 4)
  #define DUNIT0_CR_MEMCFG_DRAM_TYPE_MSK                               (0x000000F0)
  #define DUNIT0_CR_MEMCFG_DRAM_TYPE_MIN                               (0)
  #define DUNIT0_CR_MEMCFG_DRAM_TYPE_MAX                               (15) // 0x0000000F
  #define DUNIT0_CR_MEMCFG_DRAM_TYPE_DEF                               (0x00000000)
  #define DUNIT0_CR_MEMCFG_DRAM_TYPE_HSH                               (0x0408AF80)

  #define DUNIT0_CR_MEMCFG_RSVD9_8_OFF                                 ( 8)
  #define DUNIT0_CR_MEMCFG_RSVD9_8_WID                                 ( 2)
  #define DUNIT0_CR_MEMCFG_RSVD9_8_MSK                                 (0x00000300)
  #define DUNIT0_CR_MEMCFG_RSVD9_8_MIN                                 (0)
  #define DUNIT0_CR_MEMCFG_RSVD9_8_MAX                                 (3) // 0x00000003
  #define DUNIT0_CR_MEMCFG_RSVD9_8_DEF                                 (0x00000000)
  #define DUNIT0_CR_MEMCFG_RSVD9_8_HSH                                 (0x0210AF80)

  #define DUNIT0_CR_MEMCFG_CHAN_WIDTH_OFF                              (10)
  #define DUNIT0_CR_MEMCFG_CHAN_WIDTH_WID                              ( 2)
  #define DUNIT0_CR_MEMCFG_CHAN_WIDTH_MSK                              (0x00000C00)
  #define DUNIT0_CR_MEMCFG_CHAN_WIDTH_MIN                              (0)
  #define DUNIT0_CR_MEMCFG_CHAN_WIDTH_MAX                              (3) // 0x00000003
  #define DUNIT0_CR_MEMCFG_CHAN_WIDTH_DEF                              (0x00000001)
  #define DUNIT0_CR_MEMCFG_CHAN_WIDTH_HSH                              (0x0214AF80)

  #define DUNIT0_CR_MEMCFG_RSVD12_OFF                                  (12)
  #define DUNIT0_CR_MEMCFG_RSVD12_WID                                  ( 1)
  #define DUNIT0_CR_MEMCFG_RSVD12_MSK                                  (0x00001000)
  #define DUNIT0_CR_MEMCFG_RSVD12_MIN                                  (0)
  #define DUNIT0_CR_MEMCFG_RSVD12_MAX                                  (1) // 0x00000001
  #define DUNIT0_CR_MEMCFG_RSVD12_DEF                                  (0x00000000)
  #define DUNIT0_CR_MEMCFG_RSVD12_HSH                                  (0x0118AF80)

  #define DUNIT0_CR_MEMCFG_DEVICE_WIDTH_OFF                            (13)
  #define DUNIT0_CR_MEMCFG_DEVICE_WIDTH_WID                            ( 2)
  #define DUNIT0_CR_MEMCFG_DEVICE_WIDTH_MSK                            (0x00006000)
  #define DUNIT0_CR_MEMCFG_DEVICE_WIDTH_MIN                            (0)
  #define DUNIT0_CR_MEMCFG_DEVICE_WIDTH_MAX                            (3) // 0x00000003
  #define DUNIT0_CR_MEMCFG_DEVICE_WIDTH_DEF                            (0x00000000)
  #define DUNIT0_CR_MEMCFG_DEVICE_WIDTH_HSH                            (0x021AAF80)

  #define DUNIT0_CR_MEMCFG_RSVD20_15_OFF                               (15)
  #define DUNIT0_CR_MEMCFG_RSVD20_15_WID                               ( 6)
  #define DUNIT0_CR_MEMCFG_RSVD20_15_MSK                               (0x001F8000)
  #define DUNIT0_CR_MEMCFG_RSVD20_15_MIN                               (0)
  #define DUNIT0_CR_MEMCFG_RSVD20_15_MAX                               (63) // 0x0000003F
  #define DUNIT0_CR_MEMCFG_RSVD20_15_DEF                               (0x00000000)
  #define DUNIT0_CR_MEMCFG_RSVD20_15_HSH                               (0x061EAF80)

  #define DUNIT0_CR_MEMCFG_BK_ORG_OFF                                  (21)
  #define DUNIT0_CR_MEMCFG_BK_ORG_WID                                  ( 2)
  #define DUNIT0_CR_MEMCFG_BK_ORG_MSK                                  (0x00600000)
  #define DUNIT0_CR_MEMCFG_BK_ORG_MIN                                  (0)
  #define DUNIT0_CR_MEMCFG_BK_ORG_MAX                                  (3) // 0x00000003
  #define DUNIT0_CR_MEMCFG_BK_ORG_DEF                                  (0x00000000)
  #define DUNIT0_CR_MEMCFG_BK_ORG_HSH                                  (0x022AAF80)

  #define DUNIT0_CR_MEMCFG_RSVD23_OFF                                  (23)
  #define DUNIT0_CR_MEMCFG_RSVD23_WID                                  ( 1)
  #define DUNIT0_CR_MEMCFG_RSVD23_MSK                                  (0x00800000)
  #define DUNIT0_CR_MEMCFG_RSVD23_MIN                                  (0)
  #define DUNIT0_CR_MEMCFG_RSVD23_MAX                                  (1) // 0x00000001
  #define DUNIT0_CR_MEMCFG_RSVD23_DEF                                  (0x00000000)
  #define DUNIT0_CR_MEMCFG_RSVD23_HSH                                  (0x012EAF80)

  #define DUNIT0_CR_MEMCFG_BL_OFF                                      (24)
  #define DUNIT0_CR_MEMCFG_BL_WID                                      ( 1)
  #define DUNIT0_CR_MEMCFG_BL_MSK                                      (0x01000000)
  #define DUNIT0_CR_MEMCFG_BL_MIN                                      (0)
  #define DUNIT0_CR_MEMCFG_BL_MAX                                      (1) // 0x00000001
  #define DUNIT0_CR_MEMCFG_BL_DEF                                      (0x00000000)
  #define DUNIT0_CR_MEMCFG_BL_HSH                                      (0x0130AF80)

  #define DUNIT0_CR_MEMCFG_RSVD30_25_OFF                               (25)
  #define DUNIT0_CR_MEMCFG_RSVD30_25_WID                               ( 6)
  #define DUNIT0_CR_MEMCFG_RSVD30_25_MSK                               (0x7E000000)
  #define DUNIT0_CR_MEMCFG_RSVD30_25_MIN                               (0)
  #define DUNIT0_CR_MEMCFG_RSVD30_25_MAX                               (63) // 0x0000003F
  #define DUNIT0_CR_MEMCFG_RSVD30_25_DEF                               (0x00000000)
  #define DUNIT0_CR_MEMCFG_RSVD30_25_HSH                               (0x0632AF80)

  #define DUNIT0_CR_MEMCFG_ECC_EN_OFF                                  (31)
  #define DUNIT0_CR_MEMCFG_ECC_EN_WID                                  ( 1)
  #define DUNIT0_CR_MEMCFG_ECC_EN_MSK                                  (0x80000000)
  #define DUNIT0_CR_MEMCFG_ECC_EN_MIN                                  (0)
  #define DUNIT0_CR_MEMCFG_ECC_EN_MAX                                  (1) // 0x00000001
  #define DUNIT0_CR_MEMCFG_ECC_EN_DEF                                  (0x00000000)
  #define DUNIT0_CR_MEMCFG_ECC_EN_HSH                                  (0x013EAF80)

#define DUNIT0_CR_DTR0_REG                                             (0x0000AF90)
#define DUNIT0_CR_DTR0_DEF                                             (0x08C00C30)
#define DUNIT0_CR_DTR0_VOLATILE_BITFIELDS_MSK                          (0x00000000)

  #define DUNIT0_CR_DTR0_TREFI_OFF                                     ( 0)
  #define DUNIT0_CR_DTR0_TREFI_WID                                     (17)
  #define DUNIT0_CR_DTR0_TREFI_MSK                                     (0x0001FFFF)
  #define DUNIT0_CR_DTR0_TREFI_MIN                                     (0)
  #define DUNIT0_CR_DTR0_TREFI_MAX                                     (131071) // 0x0001FFFF
  #define DUNIT0_CR_DTR0_TREFI_DEF                                     (0x00000C30)
  #define DUNIT0_CR_DTR0_TREFI_HSH                                     (0x1100AF90)

  #define DUNIT0_CR_DTR0_RSVD18_17_OFF                                 (17)
  #define DUNIT0_CR_DTR0_RSVD18_17_WID                                 ( 2)
  #define DUNIT0_CR_DTR0_RSVD18_17_MSK                                 (0x00060000)
  #define DUNIT0_CR_DTR0_RSVD18_17_MIN                                 (0)
  #define DUNIT0_CR_DTR0_RSVD18_17_MAX                                 (3) // 0x00000003
  #define DUNIT0_CR_DTR0_RSVD18_17_DEF                                 (0x00000000)
  #define DUNIT0_CR_DTR0_RSVD18_17_HSH                                 (0x0222AF90)

  #define DUNIT0_CR_DTR0_TRFCAB_OFF                                    (19)
  #define DUNIT0_CR_DTR0_TRFCAB_WID                                    (12)
  #define DUNIT0_CR_DTR0_TRFCAB_MSK                                    (0x7FF80000)
  #define DUNIT0_CR_DTR0_TRFCAB_MIN                                    (0)
  #define DUNIT0_CR_DTR0_TRFCAB_MAX                                    (4095) // 0x00000FFF
  #define DUNIT0_CR_DTR0_TRFCAB_DEF                                    (0x00000118)
  #define DUNIT0_CR_DTR0_TRFCAB_HSH                                    (0x0C26AF90)

  #define DUNIT0_CR_DTR0_RSVD31_OFF                                    (31)
  #define DUNIT0_CR_DTR0_RSVD31_WID                                    ( 1)
  #define DUNIT0_CR_DTR0_RSVD31_MSK                                    (0x80000000)
  #define DUNIT0_CR_DTR0_RSVD31_MIN                                    (0)
  #define DUNIT0_CR_DTR0_RSVD31_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_DTR0_RSVD31_DEF                                    (0x00000000)
  #define DUNIT0_CR_DTR0_RSVD31_HSH                                    (0x013EAF90)

#define DUNIT0_CR_DTR1_REG                                             (0x0000AF94)
#define DUNIT0_CR_DTR1_DEF                                             (0x00000000)
#define DUNIT0_CR_DTR1_VOLATILE_BITFIELDS_MSK                          (0x00000000)

  #define DUNIT0_CR_DTR1_WL_OFF                                        ( 0)
  #define DUNIT0_CR_DTR1_WL_WID                                        ( 8)
  #define DUNIT0_CR_DTR1_WL_MSK                                        (0x000000FF)
  #define DUNIT0_CR_DTR1_WL_MIN                                        (0)
  #define DUNIT0_CR_DTR1_WL_MAX                                        (255) // 0x000000FF
  #define DUNIT0_CR_DTR1_WL_DEF                                        (0x00000000)
  #define DUNIT0_CR_DTR1_WL_HSH                                        (0x0800AF94)

  #define DUNIT0_CR_DTR1_RSVD15_8_OFF                                  ( 8)
  #define DUNIT0_CR_DTR1_RSVD15_8_WID                                  ( 8)
  #define DUNIT0_CR_DTR1_RSVD15_8_MSK                                  (0x0000FF00)
  #define DUNIT0_CR_DTR1_RSVD15_8_MIN                                  (0)
  #define DUNIT0_CR_DTR1_RSVD15_8_MAX                                  (255) // 0x000000FF
  #define DUNIT0_CR_DTR1_RSVD15_8_DEF                                  (0x00000000)
  #define DUNIT0_CR_DTR1_RSVD15_8_HSH                                  (0x0810AF94)

  #define DUNIT0_CR_DTR1_TRCD_OFF                                      (16)
  #define DUNIT0_CR_DTR1_TRCD_WID                                      ( 7)
  #define DUNIT0_CR_DTR1_TRCD_MSK                                      (0x007F0000)
  #define DUNIT0_CR_DTR1_TRCD_MIN                                      (0)
  #define DUNIT0_CR_DTR1_TRCD_MAX                                      (127) // 0x0000007F
  #define DUNIT0_CR_DTR1_TRCD_DEF                                      (0x00000000)
  #define DUNIT0_CR_DTR1_TRCD_HSH                                      (0x0720AF94)

  #define DUNIT0_CR_DTR1_RSVD31_23_OFF                                 (23)
  #define DUNIT0_CR_DTR1_RSVD31_23_WID                                 ( 9)
  #define DUNIT0_CR_DTR1_RSVD31_23_MSK                                 (0xFF800000)
  #define DUNIT0_CR_DTR1_RSVD31_23_MIN                                 (0)
  #define DUNIT0_CR_DTR1_RSVD31_23_MAX                                 (511) // 0x000001FF
  #define DUNIT0_CR_DTR1_RSVD31_23_DEF                                 (0x00000000)
  #define DUNIT0_CR_DTR1_RSVD31_23_HSH                                 (0x092EAF94)

#define DUNIT0_CR_DTR2_REG                                             (0x0000AF98)
#define DUNIT0_CR_DTR2_DEF                                             (0x00000000)
#define DUNIT0_CR_DTR2_VOLATILE_BITFIELDS_MSK                          (0x00000000)

  #define DUNIT0_CR_DTR2_TRPPB_OFF                                     ( 0)
  #define DUNIT0_CR_DTR2_TRPPB_WID                                     ( 7)
  #define DUNIT0_CR_DTR2_TRPPB_MSK                                     (0x0000007F)
  #define DUNIT0_CR_DTR2_TRPPB_MIN                                     (0)
  #define DUNIT0_CR_DTR2_TRPPB_MAX                                     (127) // 0x0000007F
  #define DUNIT0_CR_DTR2_TRPPB_DEF                                     (0x00000000)
  #define DUNIT0_CR_DTR2_TRPPB_HSH                                     (0x0700AF98)

  #define DUNIT0_CR_DTR2_RSVD7_OFF                                     ( 7)
  #define DUNIT0_CR_DTR2_RSVD7_WID                                     ( 1)
  #define DUNIT0_CR_DTR2_RSVD7_MSK                                     (0x00000080)
  #define DUNIT0_CR_DTR2_RSVD7_MIN                                     (0)
  #define DUNIT0_CR_DTR2_RSVD7_MAX                                     (1) // 0x00000001
  #define DUNIT0_CR_DTR2_RSVD7_DEF                                     (0x00000000)
  #define DUNIT0_CR_DTR2_RSVD7_HSH                                     (0x010EAF98)

  #define DUNIT0_CR_DTR2_TRPAB_OFF                                     ( 8)
  #define DUNIT0_CR_DTR2_TRPAB_WID                                     ( 7)
  #define DUNIT0_CR_DTR2_TRPAB_MSK                                     (0x00007F00)
  #define DUNIT0_CR_DTR2_TRPAB_MIN                                     (0)
  #define DUNIT0_CR_DTR2_TRPAB_MAX                                     (127) // 0x0000007F
  #define DUNIT0_CR_DTR2_TRPAB_DEF                                     (0x00000000)
  #define DUNIT0_CR_DTR2_TRPAB_HSH                                     (0x0710AF98)

  #define DUNIT0_CR_DTR2_RSVD15_OFF                                    (15)
  #define DUNIT0_CR_DTR2_RSVD15_WID                                    ( 1)
  #define DUNIT0_CR_DTR2_RSVD15_MSK                                    (0x00008000)
  #define DUNIT0_CR_DTR2_RSVD15_MIN                                    (0)
  #define DUNIT0_CR_DTR2_RSVD15_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_DTR2_RSVD15_DEF                                    (0x00000000)
  #define DUNIT0_CR_DTR2_RSVD15_HSH                                    (0x011EAF98)

  #define DUNIT0_CR_DTR2_TRAS_OFF                                      (16)
  #define DUNIT0_CR_DTR2_TRAS_WID                                      ( 8)
  #define DUNIT0_CR_DTR2_TRAS_MSK                                      (0x00FF0000)
  #define DUNIT0_CR_DTR2_TRAS_MIN                                      (0)
  #define DUNIT0_CR_DTR2_TRAS_MAX                                      (255) // 0x000000FF
  #define DUNIT0_CR_DTR2_TRAS_DEF                                      (0x00000000)
  #define DUNIT0_CR_DTR2_TRAS_HSH                                      (0x0820AF98)

  #define DUNIT0_CR_DTR2_RSVD30_24_OFF                                 (24)
  #define DUNIT0_CR_DTR2_RSVD30_24_WID                                 ( 7)
  #define DUNIT0_CR_DTR2_RSVD30_24_MSK                                 (0x7F000000)
  #define DUNIT0_CR_DTR2_RSVD30_24_MIN                                 (0)
  #define DUNIT0_CR_DTR2_RSVD30_24_MAX                                 (127) // 0x0000007F
  #define DUNIT0_CR_DTR2_RSVD30_24_DEF                                 (0x00000000)
  #define DUNIT0_CR_DTR2_RSVD30_24_HSH                                 (0x0730AF98)

  #define DUNIT0_CR_DTR2_AUTO_PRE_EN_OFF                               (31)
  #define DUNIT0_CR_DTR2_AUTO_PRE_EN_WID                               ( 1)
  #define DUNIT0_CR_DTR2_AUTO_PRE_EN_MSK                               (0x80000000)
  #define DUNIT0_CR_DTR2_AUTO_PRE_EN_MIN                               (0)
  #define DUNIT0_CR_DTR2_AUTO_PRE_EN_MAX                               (1) // 0x00000001
  #define DUNIT0_CR_DTR2_AUTO_PRE_EN_DEF                               (0x00000000)
  #define DUNIT0_CR_DTR2_AUTO_PRE_EN_HSH                               (0x013EAF98)

#define DUNIT0_CR_DTR3_REG                                             (0x0000AF9C)
#define DUNIT0_CR_DTR3_DEF                                             (0x00000000)
#define DUNIT0_CR_DTR3_VOLATILE_BITFIELDS_MSK                          (0x00000000)

  #define DUNIT0_CR_DTR3_TRRD_OFF                                      ( 0)
  #define DUNIT0_CR_DTR3_TRRD_WID                                      ( 6)
  #define DUNIT0_CR_DTR3_TRRD_MSK                                      (0x0000003F)
  #define DUNIT0_CR_DTR3_TRRD_MIN                                      (0)
  #define DUNIT0_CR_DTR3_TRRD_MAX                                      (63) // 0x0000003F
  #define DUNIT0_CR_DTR3_TRRD_DEF                                      (0x00000000)
  #define DUNIT0_CR_DTR3_TRRD_HSH                                      (0x0600AF9C)

  #define DUNIT0_CR_DTR3_RSVD23_6_OFF                                  ( 6)
  #define DUNIT0_CR_DTR3_RSVD23_6_WID                                  (18)
  #define DUNIT0_CR_DTR3_RSVD23_6_MSK                                  (0x00FFFFC0)
  #define DUNIT0_CR_DTR3_RSVD23_6_MIN                                  (0)
  #define DUNIT0_CR_DTR3_RSVD23_6_MAX                                  (262143) // 0x0003FFFF
  #define DUNIT0_CR_DTR3_RSVD23_6_DEF                                  (0x00000000)
  #define DUNIT0_CR_DTR3_RSVD23_6_HSH                                  (0x120CAF9C)

  #define DUNIT0_CR_DTR3_TCMD_OFF                                      (24)
  #define DUNIT0_CR_DTR3_TCMD_WID                                      ( 2)
  #define DUNIT0_CR_DTR3_TCMD_MSK                                      (0x03000000)
  #define DUNIT0_CR_DTR3_TCMD_MIN                                      (0)
  #define DUNIT0_CR_DTR3_TCMD_MAX                                      (3) // 0x00000003
  #define DUNIT0_CR_DTR3_TCMD_DEF                                      (0x00000000)
  #define DUNIT0_CR_DTR3_TCMD_HSH                                      (0x0230AF9C)

  #define DUNIT0_CR_DTR3_DFI_2N_MODE_CTRL_OFF                          (26)
  #define DUNIT0_CR_DTR3_DFI_2N_MODE_CTRL_WID                          ( 1)
  #define DUNIT0_CR_DTR3_DFI_2N_MODE_CTRL_MSK                          (0x04000000)
  #define DUNIT0_CR_DTR3_DFI_2N_MODE_CTRL_MIN                          (0)
  #define DUNIT0_CR_DTR3_DFI_2N_MODE_CTRL_MAX                          (1) // 0x00000001
  #define DUNIT0_CR_DTR3_DFI_2N_MODE_CTRL_DEF                          (0x00000000)
  #define DUNIT0_CR_DTR3_DFI_2N_MODE_CTRL_HSH                          (0x0134AF9C)

  #define DUNIT0_CR_DTR3_RSVD31_27_OFF                                 (27)
  #define DUNIT0_CR_DTR3_RSVD31_27_WID                                 ( 5)
  #define DUNIT0_CR_DTR3_RSVD31_27_MSK                                 (0xF8000000)
  #define DUNIT0_CR_DTR3_RSVD31_27_MIN                                 (0)
  #define DUNIT0_CR_DTR3_RSVD31_27_MAX                                 (31) // 0x0000001F
  #define DUNIT0_CR_DTR3_RSVD31_27_DEF                                 (0x00000000)
  #define DUNIT0_CR_DTR3_RSVD31_27_HSH                                 (0x0536AF9C)

#define DUNIT0_CR_DTR4_REG                                             (0x0000AFA0)
#define DUNIT0_CR_DTR4_DEF                                             (0x00000000)
#define DUNIT0_CR_DTR4_VOLATILE_BITFIELDS_MSK                          (0x00000000)

  #define DUNIT0_CR_DTR4_TRTP_OFF                                      ( 0)
  #define DUNIT0_CR_DTR4_TRTP_WID                                      ( 8)
  #define DUNIT0_CR_DTR4_TRTP_MSK                                      (0x000000FF)
  #define DUNIT0_CR_DTR4_TRTP_MIN                                      (0)
  #define DUNIT0_CR_DTR4_TRTP_MAX                                      (255) // 0x000000FF
  #define DUNIT0_CR_DTR4_TRTP_DEF                                      (0x00000000)
  #define DUNIT0_CR_DTR4_TRTP_HSH                                      (0x0800AFA0)

  #define DUNIT0_CR_DTR4_RSVD9_8_OFF                                   ( 8)
  #define DUNIT0_CR_DTR4_RSVD9_8_WID                                   ( 2)
  #define DUNIT0_CR_DTR4_RSVD9_8_MSK                                   (0x00000300)
  #define DUNIT0_CR_DTR4_RSVD9_8_MIN                                   (0)
  #define DUNIT0_CR_DTR4_RSVD9_8_MAX                                   (3) // 0x00000003
  #define DUNIT0_CR_DTR4_RSVD9_8_DEF                                   (0x00000000)
  #define DUNIT0_CR_DTR4_RSVD9_8_HSH                                   (0x0210AFA0)

  #define DUNIT0_CR_DTR4_TWTP_OFF                                      (10)
  #define DUNIT0_CR_DTR4_TWTP_WID                                      ( 8)
  #define DUNIT0_CR_DTR4_TWTP_MSK                                      (0x0003FC00)
  #define DUNIT0_CR_DTR4_TWTP_MIN                                      (0)
  #define DUNIT0_CR_DTR4_TWTP_MAX                                      (255) // 0x000000FF
  #define DUNIT0_CR_DTR4_TWTP_DEF                                      (0x00000000)
  #define DUNIT0_CR_DTR4_TWTP_HSH                                      (0x0814AFA0)

  #define DUNIT0_CR_DTR4_RSVD31_18_OFF                                 (18)
  #define DUNIT0_CR_DTR4_RSVD31_18_WID                                 (14)
  #define DUNIT0_CR_DTR4_RSVD31_18_MSK                                 (0xFFFC0000)
  #define DUNIT0_CR_DTR4_RSVD31_18_MIN                                 (0)
  #define DUNIT0_CR_DTR4_RSVD31_18_MAX                                 (16383) // 0x00003FFF
  #define DUNIT0_CR_DTR4_RSVD31_18_DEF                                 (0x00000000)
  #define DUNIT0_CR_DTR4_RSVD31_18_HSH                                 (0x0E24AFA0)

#define DUNIT0_CR_DTR5_REG                                             (0x0000AFA4)
#define DUNIT0_CR_DTR5_DEF                                             (0x10480000)
#define DUNIT0_CR_DTR5_VOLATILE_BITFIELDS_MSK                          (0x00000000)

  #define DUNIT0_CR_DTR5_TRRSR_OFF                                     ( 0)
  #define DUNIT0_CR_DTR5_TRRSR_WID                                     ( 5)
  #define DUNIT0_CR_DTR5_TRRSR_MSK                                     (0x0000001F)
  #define DUNIT0_CR_DTR5_TRRSR_MIN                                     (0)
  #define DUNIT0_CR_DTR5_TRRSR_MAX                                     (31) // 0x0000001F
  #define DUNIT0_CR_DTR5_TRRSR_DEF                                     (0x00000000)
  #define DUNIT0_CR_DTR5_TRRSR_HSH                                     (0x0500AFA4)

  #define DUNIT0_CR_DTR5_RSVD5_OFF                                     ( 5)
  #define DUNIT0_CR_DTR5_RSVD5_WID                                     ( 1)
  #define DUNIT0_CR_DTR5_RSVD5_MSK                                     (0x00000020)
  #define DUNIT0_CR_DTR5_RSVD5_MIN                                     (0)
  #define DUNIT0_CR_DTR5_RSVD5_MAX                                     (1) // 0x00000001
  #define DUNIT0_CR_DTR5_RSVD5_DEF                                     (0x00000000)
  #define DUNIT0_CR_DTR5_RSVD5_HSH                                     (0x010AAFA4)

  #define DUNIT0_CR_DTR5_TRWSR_OFF                                     ( 6)
  #define DUNIT0_CR_DTR5_TRWSR_WID                                     ( 7)
  #define DUNIT0_CR_DTR5_TRWSR_MSK                                     (0x00001FC0)
  #define DUNIT0_CR_DTR5_TRWSR_MIN                                     (0)
  #define DUNIT0_CR_DTR5_TRWSR_MAX                                     (127) // 0x0000007F
  #define DUNIT0_CR_DTR5_TRWSR_DEF                                     (0x00000000)
  #define DUNIT0_CR_DTR5_TRWSR_HSH                                     (0x070CAFA4)

  #define DUNIT0_CR_DTR5_RSVD15_13_OFF                                 (13)
  #define DUNIT0_CR_DTR5_RSVD15_13_WID                                 ( 3)
  #define DUNIT0_CR_DTR5_RSVD15_13_MSK                                 (0x0000E000)
  #define DUNIT0_CR_DTR5_RSVD15_13_MIN                                 (0)
  #define DUNIT0_CR_DTR5_RSVD15_13_MAX                                 (7) // 0x00000007
  #define DUNIT0_CR_DTR5_RSVD15_13_DEF                                 (0x00000000)
  #define DUNIT0_CR_DTR5_RSVD15_13_HSH                                 (0x031AAFA4)

  #define DUNIT0_CR_DTR5_TWWSR_OFF                                     (16)
  #define DUNIT0_CR_DTR5_TWWSR_WID                                     ( 5)
  #define DUNIT0_CR_DTR5_TWWSR_MSK                                     (0x001F0000)
  #define DUNIT0_CR_DTR5_TWWSR_MIN                                     (0)
  #define DUNIT0_CR_DTR5_TWWSR_MAX                                     (31) // 0x0000001F
  #define DUNIT0_CR_DTR5_TWWSR_DEF                                     (0x00000008)
  #define DUNIT0_CR_DTR5_TWWSR_HSH                                     (0x0520AFA4)

  #define DUNIT0_CR_DTR5_RSVD21_OFF                                    (21)
  #define DUNIT0_CR_DTR5_RSVD21_WID                                    ( 1)
  #define DUNIT0_CR_DTR5_RSVD21_MSK                                    (0x00200000)
  #define DUNIT0_CR_DTR5_RSVD21_MIN                                    (0)
  #define DUNIT0_CR_DTR5_RSVD21_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_DTR5_RSVD21_DEF                                    (0x00000000)
  #define DUNIT0_CR_DTR5_RSVD21_HSH                                    (0x012AAFA4)

  #define DUNIT0_CR_DTR5_TWRSR_OFF                                     (22)
  #define DUNIT0_CR_DTR5_TWRSR_WID                                     ( 8)
  #define DUNIT0_CR_DTR5_TWRSR_MSK                                     (0x3FC00000)
  #define DUNIT0_CR_DTR5_TWRSR_MIN                                     (0)
  #define DUNIT0_CR_DTR5_TWRSR_MAX                                     (255) // 0x000000FF
  #define DUNIT0_CR_DTR5_TWRSR_DEF                                     (0x00000041)
  #define DUNIT0_CR_DTR5_TWRSR_HSH                                     (0x082CAFA4)

  #define DUNIT0_CR_DTR5_RSVD31_30_OFF                                 (30)
  #define DUNIT0_CR_DTR5_RSVD31_30_WID                                 ( 2)
  #define DUNIT0_CR_DTR5_RSVD31_30_MSK                                 (0xC0000000)
  #define DUNIT0_CR_DTR5_RSVD31_30_MIN                                 (0)
  #define DUNIT0_CR_DTR5_RSVD31_30_MAX                                 (3) // 0x00000003
  #define DUNIT0_CR_DTR5_RSVD31_30_DEF                                 (0x00000000)
  #define DUNIT0_CR_DTR5_RSVD31_30_HSH                                 (0x023CAFA4)

#define DUNIT0_CR_DTR6_REG                                             (0x0000AFA8)
#define DUNIT0_CR_DTR6_DEF                                             (0x00000000)
#define DUNIT0_CR_DTR6_VOLATILE_BITFIELDS_MSK                          (0x00000000)

  #define DUNIT0_CR_DTR6_TRRSR_L_OFF                                   ( 0)
  #define DUNIT0_CR_DTR6_TRRSR_L_WID                                   ( 6)
  #define DUNIT0_CR_DTR6_TRRSR_L_MSK                                   (0x0000003F)
  #define DUNIT0_CR_DTR6_TRRSR_L_MIN                                   (0)
  #define DUNIT0_CR_DTR6_TRRSR_L_MAX                                   (63) // 0x0000003F
  #define DUNIT0_CR_DTR6_TRRSR_L_DEF                                   (0x00000000)
  #define DUNIT0_CR_DTR6_TRRSR_L_HSH                                   (0x0600AFA8)

  #define DUNIT0_CR_DTR6_RSVD11_6_OFF                                  ( 6)
  #define DUNIT0_CR_DTR6_RSVD11_6_WID                                  ( 6)
  #define DUNIT0_CR_DTR6_RSVD11_6_MSK                                  (0x00000FC0)
  #define DUNIT0_CR_DTR6_RSVD11_6_MIN                                  (0)
  #define DUNIT0_CR_DTR6_RSVD11_6_MAX                                  (63) // 0x0000003F
  #define DUNIT0_CR_DTR6_RSVD11_6_DEF                                  (0x00000000)
  #define DUNIT0_CR_DTR6_RSVD11_6_HSH                                  (0x060CAFA8)

  #define DUNIT0_CR_DTR6_TWWSR_L_OFF                                   (12)
  #define DUNIT0_CR_DTR6_TWWSR_L_WID                                   ( 8)
  #define DUNIT0_CR_DTR6_TWWSR_L_MSK                                   (0x000FF000)
  #define DUNIT0_CR_DTR6_TWWSR_L_MIN                                   (0)
  #define DUNIT0_CR_DTR6_TWWSR_L_MAX                                   (255) // 0x000000FF
  #define DUNIT0_CR_DTR6_TWWSR_L_DEF                                   (0x00000000)
  #define DUNIT0_CR_DTR6_TWWSR_L_HSH                                   (0x0818AFA8)

  #define DUNIT0_CR_DTR6_RSVD21_20_OFF                                 (20)
  #define DUNIT0_CR_DTR6_RSVD21_20_WID                                 ( 2)
  #define DUNIT0_CR_DTR6_RSVD21_20_MSK                                 (0x00300000)
  #define DUNIT0_CR_DTR6_RSVD21_20_MIN                                 (0)
  #define DUNIT0_CR_DTR6_RSVD21_20_MAX                                 (3) // 0x00000003
  #define DUNIT0_CR_DTR6_RSVD21_20_DEF                                 (0x00000000)
  #define DUNIT0_CR_DTR6_RSVD21_20_HSH                                 (0x0228AFA8)

  #define DUNIT0_CR_DTR6_TWRSR_L_OFF                                   (22)
  #define DUNIT0_CR_DTR6_TWRSR_L_WID                                   ( 8)
  #define DUNIT0_CR_DTR6_TWRSR_L_MSK                                   (0x3FC00000)
  #define DUNIT0_CR_DTR6_TWRSR_L_MIN                                   (0)
  #define DUNIT0_CR_DTR6_TWRSR_L_MAX                                   (255) // 0x000000FF
  #define DUNIT0_CR_DTR6_TWRSR_L_DEF                                   (0x00000000)
  #define DUNIT0_CR_DTR6_TWRSR_L_HSH                                   (0x082CAFA8)

  #define DUNIT0_CR_DTR6_RSVD31_30_OFF                                 (30)
  #define DUNIT0_CR_DTR6_RSVD31_30_WID                                 ( 2)
  #define DUNIT0_CR_DTR6_RSVD31_30_MSK                                 (0xC0000000)
  #define DUNIT0_CR_DTR6_RSVD31_30_MIN                                 (0)
  #define DUNIT0_CR_DTR6_RSVD31_30_MAX                                 (3) // 0x00000003
  #define DUNIT0_CR_DTR6_RSVD31_30_DEF                                 (0x00000000)
  #define DUNIT0_CR_DTR6_RSVD31_30_HSH                                 (0x023CAFA8)

#define DUNIT0_CR_DTR7_REG                                             (0x0000AFAC)
#define DUNIT0_CR_DTR7_DEF                                             (0x00000000)
#define DUNIT0_CR_DTR7_VOLATILE_BITFIELDS_MSK                          (0x00000000)

  #define DUNIT0_CR_DTR7_TWMWSB_OFF                                    ( 0)
  #define DUNIT0_CR_DTR7_TWMWSB_WID                                    ( 8)
  #define DUNIT0_CR_DTR7_TWMWSB_MSK                                    (0x000000FF)
  #define DUNIT0_CR_DTR7_TWMWSB_MIN                                    (0)
  #define DUNIT0_CR_DTR7_TWMWSB_MAX                                    (255) // 0x000000FF
  #define DUNIT0_CR_DTR7_TWMWSB_DEF                                    (0x00000000)
  #define DUNIT0_CR_DTR7_TWMWSB_HSH                                    (0x0800AFAC)

  #define DUNIT0_CR_DTR7_RSVD15_8_OFF                                  ( 8)
  #define DUNIT0_CR_DTR7_RSVD15_8_WID                                  ( 8)
  #define DUNIT0_CR_DTR7_RSVD15_8_MSK                                  (0x0000FF00)
  #define DUNIT0_CR_DTR7_RSVD15_8_MIN                                  (0)
  #define DUNIT0_CR_DTR7_RSVD15_8_MAX                                  (255) // 0x000000FF
  #define DUNIT0_CR_DTR7_RSVD15_8_DEF                                  (0x00000000)
  #define DUNIT0_CR_DTR7_RSVD15_8_HSH                                  (0x0810AFAC)

  #define DUNIT0_CR_DTR7_TRPRE_OFF                                     (16)
  #define DUNIT0_CR_DTR7_TRPRE_WID                                     ( 3)
  #define DUNIT0_CR_DTR7_TRPRE_MSK                                     (0x00070000)
  #define DUNIT0_CR_DTR7_TRPRE_MIN                                     (0)
  #define DUNIT0_CR_DTR7_TRPRE_MAX                                     (7) // 0x00000007
  #define DUNIT0_CR_DTR7_TRPRE_DEF                                     (0x00000000)
  #define DUNIT0_CR_DTR7_TRPRE_HSH                                     (0x0320AFAC)

  #define DUNIT0_CR_DTR7_RSVD19_OFF                                    (19)
  #define DUNIT0_CR_DTR7_RSVD19_WID                                    ( 1)
  #define DUNIT0_CR_DTR7_RSVD19_MSK                                    (0x00080000)
  #define DUNIT0_CR_DTR7_RSVD19_MIN                                    (0)
  #define DUNIT0_CR_DTR7_RSVD19_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_DTR7_RSVD19_DEF                                    (0x00000000)
  #define DUNIT0_CR_DTR7_RSVD19_HSH                                    (0x0126AFAC)

  #define DUNIT0_CR_DTR7_TWPRE_OFF                                     (20)
  #define DUNIT0_CR_DTR7_TWPRE_WID                                     ( 3)
  #define DUNIT0_CR_DTR7_TWPRE_MSK                                     (0x00700000)
  #define DUNIT0_CR_DTR7_TWPRE_MIN                                     (0)
  #define DUNIT0_CR_DTR7_TWPRE_MAX                                     (7) // 0x00000007
  #define DUNIT0_CR_DTR7_TWPRE_DEF                                     (0x00000000)
  #define DUNIT0_CR_DTR7_TWPRE_HSH                                     (0x0328AFAC)

  #define DUNIT0_CR_DTR7_RSVD23_OFF                                    (23)
  #define DUNIT0_CR_DTR7_RSVD23_WID                                    ( 1)
  #define DUNIT0_CR_DTR7_RSVD23_MSK                                    (0x00800000)
  #define DUNIT0_CR_DTR7_RSVD23_MIN                                    (0)
  #define DUNIT0_CR_DTR7_RSVD23_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_DTR7_RSVD23_DEF                                    (0x00000000)
  #define DUNIT0_CR_DTR7_RSVD23_HSH                                    (0x012EAFAC)

  #define DUNIT0_CR_DTR7_TRPST_OFF                                     (24)
  #define DUNIT0_CR_DTR7_TRPST_WID                                     ( 2)
  #define DUNIT0_CR_DTR7_TRPST_MSK                                     (0x03000000)
  #define DUNIT0_CR_DTR7_TRPST_MIN                                     (0)
  #define DUNIT0_CR_DTR7_TRPST_MAX                                     (3) // 0x00000003
  #define DUNIT0_CR_DTR7_TRPST_DEF                                     (0x00000000)
  #define DUNIT0_CR_DTR7_TRPST_HSH                                     (0x0230AFAC)

  #define DUNIT0_CR_DTR7_TWPST_OFF                                     (26)
  #define DUNIT0_CR_DTR7_TWPST_WID                                     ( 2)
  #define DUNIT0_CR_DTR7_TWPST_MSK                                     (0x0C000000)
  #define DUNIT0_CR_DTR7_TWPST_MIN                                     (0)
  #define DUNIT0_CR_DTR7_TWPST_MAX                                     (3) // 0x00000003
  #define DUNIT0_CR_DTR7_TWPST_DEF                                     (0x00000000)
  #define DUNIT0_CR_DTR7_TWPST_HSH                                     (0x0234AFAC)

  #define DUNIT0_CR_DTR7_RSVD28_OFF                                    (28)
  #define DUNIT0_CR_DTR7_RSVD28_WID                                    ( 1)
  #define DUNIT0_CR_DTR7_RSVD28_MSK                                    (0x10000000)
  #define DUNIT0_CR_DTR7_RSVD28_MIN                                    (0)
  #define DUNIT0_CR_DTR7_RSVD28_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_DTR7_RSVD28_DEF                                    (0x00000000)
  #define DUNIT0_CR_DTR7_RSVD28_HSH                                    (0x0138AFAC)

  #define DUNIT0_CR_DTR7_RAND_TAT_OFF                                  (29)
  #define DUNIT0_CR_DTR7_RAND_TAT_WID                                  ( 3)
  #define DUNIT0_CR_DTR7_RAND_TAT_MSK                                  (0xE0000000)
  #define DUNIT0_CR_DTR7_RAND_TAT_MIN                                  (0)
  #define DUNIT0_CR_DTR7_RAND_TAT_MAX                                  (7) // 0x00000007
  #define DUNIT0_CR_DTR7_RAND_TAT_DEF                                  (0x00000000)
  #define DUNIT0_CR_DTR7_RAND_TAT_HSH                                  (0x033AAFAC)

#define DUNIT0_CR_DTR8_REG                                             (0x0000AFB0)
#define DUNIT0_CR_DTR8_DEF                                             (0x00000000)
#define DUNIT0_CR_DTR8_VOLATILE_BITFIELDS_MSK                          (0x00000000)

  #define DUNIT0_CR_DTR8_TRRDR_OFF                                     ( 0)
  #define DUNIT0_CR_DTR8_TRRDR_WID                                     ( 6)
  #define DUNIT0_CR_DTR8_TRRDR_MSK                                     (0x0000003F)
  #define DUNIT0_CR_DTR8_TRRDR_MIN                                     (0)
  #define DUNIT0_CR_DTR8_TRRDR_MAX                                     (63) // 0x0000003F
  #define DUNIT0_CR_DTR8_TRRDR_DEF                                     (0x00000000)
  #define DUNIT0_CR_DTR8_TRRDR_HSH                                     (0x0600AFB0)

  #define DUNIT0_CR_DTR8_RSVD6_OFF                                     ( 6)
  #define DUNIT0_CR_DTR8_RSVD6_WID                                     ( 1)
  #define DUNIT0_CR_DTR8_RSVD6_MSK                                     (0x00000040)
  #define DUNIT0_CR_DTR8_RSVD6_MIN                                     (0)
  #define DUNIT0_CR_DTR8_RSVD6_MAX                                     (1) // 0x00000001
  #define DUNIT0_CR_DTR8_RSVD6_DEF                                     (0x00000000)
  #define DUNIT0_CR_DTR8_RSVD6_HSH                                     (0x010CAFB0)

  #define DUNIT0_CR_DTR8_TRWDR_OFF                                     ( 7)
  #define DUNIT0_CR_DTR8_TRWDR_WID                                     ( 7)
  #define DUNIT0_CR_DTR8_TRWDR_MSK                                     (0x00003F80)
  #define DUNIT0_CR_DTR8_TRWDR_MIN                                     (0)
  #define DUNIT0_CR_DTR8_TRWDR_MAX                                     (127) // 0x0000007F
  #define DUNIT0_CR_DTR8_TRWDR_DEF                                     (0x00000000)
  #define DUNIT0_CR_DTR8_TRWDR_HSH                                     (0x070EAFB0)

  #define DUNIT0_CR_DTR8_RSVD15_14_OFF                                 (14)
  #define DUNIT0_CR_DTR8_RSVD15_14_WID                                 ( 2)
  #define DUNIT0_CR_DTR8_RSVD15_14_MSK                                 (0x0000C000)
  #define DUNIT0_CR_DTR8_RSVD15_14_MIN                                 (0)
  #define DUNIT0_CR_DTR8_RSVD15_14_MAX                                 (3) // 0x00000003
  #define DUNIT0_CR_DTR8_RSVD15_14_DEF                                 (0x00000000)
  #define DUNIT0_CR_DTR8_RSVD15_14_HSH                                 (0x021CAFB0)

  #define DUNIT0_CR_DTR8_TWWDR_OFF                                     (16)
  #define DUNIT0_CR_DTR8_TWWDR_WID                                     ( 7)
  #define DUNIT0_CR_DTR8_TWWDR_MSK                                     (0x007F0000)
  #define DUNIT0_CR_DTR8_TWWDR_MIN                                     (0)
  #define DUNIT0_CR_DTR8_TWWDR_MAX                                     (127) // 0x0000007F
  #define DUNIT0_CR_DTR8_TWWDR_DEF                                     (0x00000000)
  #define DUNIT0_CR_DTR8_TWWDR_HSH                                     (0x0720AFB0)

  #define DUNIT0_CR_DTR8_RSVD23_OFF                                    (23)
  #define DUNIT0_CR_DTR8_RSVD23_WID                                    ( 1)
  #define DUNIT0_CR_DTR8_RSVD23_MSK                                    (0x00800000)
  #define DUNIT0_CR_DTR8_RSVD23_MIN                                    (0)
  #define DUNIT0_CR_DTR8_RSVD23_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_DTR8_RSVD23_DEF                                    (0x00000000)
  #define DUNIT0_CR_DTR8_RSVD23_HSH                                    (0x012EAFB0)

  #define DUNIT0_CR_DTR8_TWRDR_OFF                                     (24)
  #define DUNIT0_CR_DTR8_TWRDR_WID                                     ( 6)
  #define DUNIT0_CR_DTR8_TWRDR_MSK                                     (0x3F000000)
  #define DUNIT0_CR_DTR8_TWRDR_MIN                                     (0)
  #define DUNIT0_CR_DTR8_TWRDR_MAX                                     (63) // 0x0000003F
  #define DUNIT0_CR_DTR8_TWRDR_DEF                                     (0x00000000)
  #define DUNIT0_CR_DTR8_TWRDR_HSH                                     (0x0630AFB0)

  #define DUNIT0_CR_DTR8_RSVD31_30_OFF                                 (30)
  #define DUNIT0_CR_DTR8_RSVD31_30_WID                                 ( 2)
  #define DUNIT0_CR_DTR8_RSVD31_30_MSK                                 (0xC0000000)
  #define DUNIT0_CR_DTR8_RSVD31_30_MIN                                 (0)
  #define DUNIT0_CR_DTR8_RSVD31_30_MAX                                 (3) // 0x00000003
  #define DUNIT0_CR_DTR8_RSVD31_30_DEF                                 (0x00000000)
  #define DUNIT0_CR_DTR8_RSVD31_30_HSH                                 (0x023CAFB0)

#define DUNIT0_CR_DTR9_REG                                             (0x0000AFB4)
#define DUNIT0_CR_DTR9_DEF                                             (0x00000000)
#define DUNIT0_CR_DTR9_VOLATILE_BITFIELDS_MSK                          (0x00000000)

  #define DUNIT0_CR_DTR9_TPREMRBLK_OFF                                 ( 0)
  #define DUNIT0_CR_DTR9_TPREMRBLK_WID                                 ( 9)
  #define DUNIT0_CR_DTR9_TPREMRBLK_MSK                                 (0x000001FF)
  #define DUNIT0_CR_DTR9_TPREMRBLK_MIN                                 (0)
  #define DUNIT0_CR_DTR9_TPREMRBLK_MAX                                 (511) // 0x000001FF
  #define DUNIT0_CR_DTR9_TPREMRBLK_DEF                                 (0x00000000)
  #define DUNIT0_CR_DTR9_TPREMRBLK_HSH                                 (0x0900AFB4)

  #define DUNIT0_CR_DTR9_TPSTMRRBLK_OFF                                ( 9)
  #define DUNIT0_CR_DTR9_TPSTMRRBLK_WID                                ( 7)
  #define DUNIT0_CR_DTR9_TPSTMRRBLK_MSK                                (0x0000FE00)
  #define DUNIT0_CR_DTR9_TPSTMRRBLK_MIN                                (0)
  #define DUNIT0_CR_DTR9_TPSTMRRBLK_MAX                                (127) // 0x0000007F
  #define DUNIT0_CR_DTR9_TPSTMRRBLK_DEF                                (0x00000000)
  #define DUNIT0_CR_DTR9_TPSTMRRBLK_HSH                                (0x0712AFB4)

  #define DUNIT0_CR_DTR9_RSVD16_OFF                                    (16)
  #define DUNIT0_CR_DTR9_RSVD16_WID                                    ( 1)
  #define DUNIT0_CR_DTR9_RSVD16_MSK                                    (0x00010000)
  #define DUNIT0_CR_DTR9_RSVD16_MIN                                    (0)
  #define DUNIT0_CR_DTR9_RSVD16_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_DTR9_RSVD16_DEF                                    (0x00000000)
  #define DUNIT0_CR_DTR9_RSVD16_HSH                                    (0x0120AFB4)

  #define DUNIT0_CR_DTR9_TPSTMRWBLK_OFF                                (17)
  #define DUNIT0_CR_DTR9_TPSTMRWBLK_WID                                ( 6)
  #define DUNIT0_CR_DTR9_TPSTMRWBLK_MSK                                (0x007E0000)
  #define DUNIT0_CR_DTR9_TPSTMRWBLK_MIN                                (0)
  #define DUNIT0_CR_DTR9_TPSTMRWBLK_MAX                                (63) // 0x0000003F
  #define DUNIT0_CR_DTR9_TPSTMRWBLK_DEF                                (0x00000000)
  #define DUNIT0_CR_DTR9_TPSTMRWBLK_HSH                                (0x0622AFB4)

  #define DUNIT0_CR_DTR9_RSVD23_OFF                                    (23)
  #define DUNIT0_CR_DTR9_RSVD23_WID                                    ( 1)
  #define DUNIT0_CR_DTR9_RSVD23_MSK                                    (0x00800000)
  #define DUNIT0_CR_DTR9_RSVD23_MIN                                    (0)
  #define DUNIT0_CR_DTR9_RSVD23_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_DTR9_RSVD23_DEF                                    (0x00000000)
  #define DUNIT0_CR_DTR9_RSVD23_HSH                                    (0x012EAFB4)

  #define DUNIT0_CR_DTR9_TMRR_OFF                                      (24)
  #define DUNIT0_CR_DTR9_TMRR_WID                                      ( 7)
  #define DUNIT0_CR_DTR9_TMRR_MSK                                      (0x7F000000)
  #define DUNIT0_CR_DTR9_TMRR_MIN                                      (0)
  #define DUNIT0_CR_DTR9_TMRR_MAX                                      (127) // 0x0000007F
  #define DUNIT0_CR_DTR9_TMRR_DEF                                      (0x00000000)
  #define DUNIT0_CR_DTR9_TMRR_HSH                                      (0x0730AFB4)

  #define DUNIT0_CR_DTR9_RSVD31_OFF                                    (31)
  #define DUNIT0_CR_DTR9_RSVD31_WID                                    ( 1)
  #define DUNIT0_CR_DTR9_RSVD31_MSK                                    (0x80000000)
  #define DUNIT0_CR_DTR9_RSVD31_MIN                                    (0)
  #define DUNIT0_CR_DTR9_RSVD31_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_DTR9_RSVD31_DEF                                    (0x00000000)
  #define DUNIT0_CR_DTR9_RSVD31_HSH                                    (0x013EAFB4)

#define DUNIT0_CR_DTR10_REG                                            (0x0000AFB8)
#define DUNIT0_CR_DTR10_DEF                                            (0x00000000)
#define DUNIT0_CR_DTR10_VOLATILE_BITFIELDS_MSK                         (0x00000000)

  #define DUNIT0_CR_DTR10_TRDPDEN_OFF                                  ( 0)
  #define DUNIT0_CR_DTR10_TRDPDEN_WID                                  ( 9)
  #define DUNIT0_CR_DTR10_TRDPDEN_MSK                                  (0x000001FF)
  #define DUNIT0_CR_DTR10_TRDPDEN_MIN                                  (0)
  #define DUNIT0_CR_DTR10_TRDPDEN_MAX                                  (511) // 0x000001FF
  #define DUNIT0_CR_DTR10_TRDPDEN_DEF                                  (0x00000000)
  #define DUNIT0_CR_DTR10_TRDPDEN_HSH                                  (0x0900AFB8)

  #define DUNIT0_CR_DTR10_RSVD9_OFF                                    ( 9)
  #define DUNIT0_CR_DTR10_RSVD9_WID                                    ( 1)
  #define DUNIT0_CR_DTR10_RSVD9_MSK                                    (0x00000200)
  #define DUNIT0_CR_DTR10_RSVD9_MIN                                    (0)
  #define DUNIT0_CR_DTR10_RSVD9_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_DTR10_RSVD9_DEF                                    (0x00000000)
  #define DUNIT0_CR_DTR10_RSVD9_HSH                                    (0x0112AFB8)

  #define DUNIT0_CR_DTR10_TWRPDEN_OFF                                  (10)
  #define DUNIT0_CR_DTR10_TWRPDEN_WID                                  ( 9)
  #define DUNIT0_CR_DTR10_TWRPDEN_MSK                                  (0x0007FC00)
  #define DUNIT0_CR_DTR10_TWRPDEN_MIN                                  (0)
  #define DUNIT0_CR_DTR10_TWRPDEN_MAX                                  (511) // 0x000001FF
  #define DUNIT0_CR_DTR10_TWRPDEN_DEF                                  (0x00000000)
  #define DUNIT0_CR_DTR10_TWRPDEN_HSH                                  (0x0914AFB8)

  #define DUNIT0_CR_DTR10_RSVD31_19_OFF                                (19)
  #define DUNIT0_CR_DTR10_RSVD31_19_WID                                (13)
  #define DUNIT0_CR_DTR10_RSVD31_19_MSK                                (0xFFF80000)
  #define DUNIT0_CR_DTR10_RSVD31_19_MIN                                (0)
  #define DUNIT0_CR_DTR10_RSVD31_19_MAX                                (8191) // 0x00001FFF
  #define DUNIT0_CR_DTR10_RSVD31_19_DEF                                (0x00000000)
  #define DUNIT0_CR_DTR10_RSVD31_19_HSH                                (0x0D26AFB8)

#define DUNIT0_CR_DTR11_REG                                            (0x0000AFBC)
#define DUNIT0_CR_DTR11_DEF                                            (0x02809606)
#define DUNIT0_CR_DTR11_VOLATILE_BITFIELDS_MSK                         (0x00000000)

  #define DUNIT0_CR_DTR11_TXP_OFF                                      ( 0)
  #define DUNIT0_CR_DTR11_TXP_WID                                      ( 7)
  #define DUNIT0_CR_DTR11_TXP_MSK                                      (0x0000007F)
  #define DUNIT0_CR_DTR11_TXP_MIN                                      (0)
  #define DUNIT0_CR_DTR11_TXP_MAX                                      (127) // 0x0000007F
  #define DUNIT0_CR_DTR11_TXP_DEF                                      (0x00000006)
  #define DUNIT0_CR_DTR11_TXP_HSH                                      (0x0700AFBC)

  #define DUNIT0_CR_DTR11_RSVD7_OFF                                    ( 7)
  #define DUNIT0_CR_DTR11_RSVD7_WID                                    ( 1)
  #define DUNIT0_CR_DTR11_RSVD7_MSK                                    (0x00000080)
  #define DUNIT0_CR_DTR11_RSVD7_MIN                                    (0)
  #define DUNIT0_CR_DTR11_RSVD7_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_DTR11_RSVD7_DEF                                    (0x00000000)
  #define DUNIT0_CR_DTR11_RSVD7_HSH                                    (0x010EAFBC)

  #define DUNIT0_CR_DTR11_TXSR_OFF                                     ( 8)
  #define DUNIT0_CR_DTR11_TXSR_WID                                     (13)
  #define DUNIT0_CR_DTR11_TXSR_MSK                                     (0x001FFF00)
  #define DUNIT0_CR_DTR11_TXSR_MIN                                     (0)
  #define DUNIT0_CR_DTR11_TXSR_MAX                                     (8191) // 0x00001FFF
  #define DUNIT0_CR_DTR11_TXSR_DEF                                     (0x00000096)
  #define DUNIT0_CR_DTR11_TXSR_HSH                                     (0x0D10AFBC)

  #define DUNIT0_CR_DTR11_RSVD21_OFF                                   (21)
  #define DUNIT0_CR_DTR11_RSVD21_WID                                   ( 1)
  #define DUNIT0_CR_DTR11_RSVD21_MSK                                   (0x00200000)
  #define DUNIT0_CR_DTR11_RSVD21_MIN                                   (0)
  #define DUNIT0_CR_DTR11_RSVD21_MAX                                   (1) // 0x00000001
  #define DUNIT0_CR_DTR11_RSVD21_DEF                                   (0x00000000)
  #define DUNIT0_CR_DTR11_RSVD21_HSH                                   (0x012AAFBC)

  #define DUNIT0_CR_DTR11_TXSDLL_OFF                                   (22)
  #define DUNIT0_CR_DTR11_TXSDLL_WID                                   ( 7)
  #define DUNIT0_CR_DTR11_TXSDLL_MSK                                   (0x1FC00000)
  #define DUNIT0_CR_DTR11_TXSDLL_MIN                                   (0)
  #define DUNIT0_CR_DTR11_TXSDLL_MAX                                   (127) // 0x0000007F
  #define DUNIT0_CR_DTR11_TXSDLL_DEF                                   (0x0000000A)
  #define DUNIT0_CR_DTR11_TXSDLL_HSH                                   (0x072CAFBC)

  #define DUNIT0_CR_DTR11_RSVD31_29_OFF                                (29)
  #define DUNIT0_CR_DTR11_RSVD31_29_WID                                ( 3)
  #define DUNIT0_CR_DTR11_RSVD31_29_MSK                                (0xE0000000)
  #define DUNIT0_CR_DTR11_RSVD31_29_MIN                                (0)
  #define DUNIT0_CR_DTR11_RSVD31_29_MAX                                (7) // 0x00000007
  #define DUNIT0_CR_DTR11_RSVD31_29_DEF                                (0x00000000)
  #define DUNIT0_CR_DTR11_RSVD31_29_HSH                                (0x033AAFBC)

#define DUNIT0_CR_DTR12_REG                                            (0x0000AFC0)
#define DUNIT0_CR_DTR12_DEF                                            (0x0306060C)
#define DUNIT0_CR_DTR12_VOLATILE_BITFIELDS_MSK                         (0x00000000)

  #define DUNIT0_CR_DTR12_TSR_OFF                                      ( 0)
  #define DUNIT0_CR_DTR12_TSR_WID                                      ( 7)
  #define DUNIT0_CR_DTR12_TSR_MSK                                      (0x0000007F)
  #define DUNIT0_CR_DTR12_TSR_MIN                                      (0)
  #define DUNIT0_CR_DTR12_TSR_MAX                                      (127) // 0x0000007F
  #define DUNIT0_CR_DTR12_TSR_DEF                                      (0x0000000C)
  #define DUNIT0_CR_DTR12_TSR_HSH                                      (0x0700AFC0)

  #define DUNIT0_CR_DTR12_RSVD7_OFF                                    ( 7)
  #define DUNIT0_CR_DTR12_RSVD7_WID                                    ( 1)
  #define DUNIT0_CR_DTR12_RSVD7_MSK                                    (0x00000080)
  #define DUNIT0_CR_DTR12_RSVD7_MIN                                    (0)
  #define DUNIT0_CR_DTR12_RSVD7_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_DTR12_RSVD7_DEF                                    (0x00000000)
  #define DUNIT0_CR_DTR12_RSVD7_HSH                                    (0x010EAFC0)

  #define DUNIT0_CR_DTR12_TCKE_OFF                                     ( 8)
  #define DUNIT0_CR_DTR12_TCKE_WID                                     ( 7)
  #define DUNIT0_CR_DTR12_TCKE_MSK                                     (0x00007F00)
  #define DUNIT0_CR_DTR12_TCKE_MIN                                     (0)
  #define DUNIT0_CR_DTR12_TCKE_MAX                                     (127) // 0x0000007F
  #define DUNIT0_CR_DTR12_TCKE_DEF                                     (0x00000006)
  #define DUNIT0_CR_DTR12_TCKE_HSH                                     (0x0710AFC0)

  #define DUNIT0_CR_DTR12_RSVD15_OFF                                   (15)
  #define DUNIT0_CR_DTR12_RSVD15_WID                                   ( 1)
  #define DUNIT0_CR_DTR12_RSVD15_MSK                                   (0x00008000)
  #define DUNIT0_CR_DTR12_RSVD15_MIN                                   (0)
  #define DUNIT0_CR_DTR12_RSVD15_MAX                                   (1) // 0x00000001
  #define DUNIT0_CR_DTR12_RSVD15_DEF                                   (0x00000000)
  #define DUNIT0_CR_DTR12_RSVD15_HSH                                   (0x011EAFC0)

  #define DUNIT0_CR_DTR12_TCKCKEL_OFF                                  (16)
  #define DUNIT0_CR_DTR12_TCKCKEL_WID                                  ( 6)
  #define DUNIT0_CR_DTR12_TCKCKEL_MSK                                  (0x003F0000)
  #define DUNIT0_CR_DTR12_TCKCKEL_MIN                                  (0)
  #define DUNIT0_CR_DTR12_TCKCKEL_MAX                                  (63) // 0x0000003F
  #define DUNIT0_CR_DTR12_TCKCKEL_DEF                                  (0x00000006)
  #define DUNIT0_CR_DTR12_TCKCKEL_HSH                                  (0x0620AFC0)

  #define DUNIT0_CR_DTR12_RSVD23_22_OFF                                (22)
  #define DUNIT0_CR_DTR12_RSVD23_22_WID                                ( 2)
  #define DUNIT0_CR_DTR12_RSVD23_22_MSK                                (0x00C00000)
  #define DUNIT0_CR_DTR12_RSVD23_22_MIN                                (0)
  #define DUNIT0_CR_DTR12_RSVD23_22_MAX                                (3) // 0x00000003
  #define DUNIT0_CR_DTR12_RSVD23_22_DEF                                (0x00000000)
  #define DUNIT0_CR_DTR12_RSVD23_22_HSH                                (0x022CAFC0)

  #define DUNIT0_CR_DTR12_TCKCKEH_OFF                                  (24)
  #define DUNIT0_CR_DTR12_TCKCKEH_WID                                  ( 6)
  #define DUNIT0_CR_DTR12_TCKCKEH_MSK                                  (0x3F000000)
  #define DUNIT0_CR_DTR12_TCKCKEH_MIN                                  (0)
  #define DUNIT0_CR_DTR12_TCKCKEH_MAX                                  (63) // 0x0000003F
  #define DUNIT0_CR_DTR12_TCKCKEH_DEF                                  (0x00000003)
  #define DUNIT0_CR_DTR12_TCKCKEH_HSH                                  (0x0630AFC0)

  #define DUNIT0_CR_DTR12_RSVD31_30_OFF                                (30)
  #define DUNIT0_CR_DTR12_RSVD31_30_WID                                ( 2)
  #define DUNIT0_CR_DTR12_RSVD31_30_MSK                                (0xC0000000)
  #define DUNIT0_CR_DTR12_RSVD31_30_MIN                                (0)
  #define DUNIT0_CR_DTR12_RSVD31_30_MAX                                (3) // 0x00000003
  #define DUNIT0_CR_DTR12_RSVD31_30_DEF                                (0x00000000)
  #define DUNIT0_CR_DTR12_RSVD31_30_HSH                                (0x023CAFC0)

#define DUNIT0_CR_DTR13_REG                                            (0x0000AFC4)
#define DUNIT0_CR_DTR13_DEF                                            (0x00000603)
#define DUNIT0_CR_DTR13_VOLATILE_BITFIELDS_MSK                         (0x00000000)

  #define DUNIT0_CR_DTR13_TCSH_OFF                                     ( 0)
  #define DUNIT0_CR_DTR13_TCSH_WID                                     ( 7)
  #define DUNIT0_CR_DTR13_TCSH_MSK                                     (0x0000007F)
  #define DUNIT0_CR_DTR13_TCSH_MIN                                     (0)
  #define DUNIT0_CR_DTR13_TCSH_MAX                                     (127) // 0x0000007F
  #define DUNIT0_CR_DTR13_TCSH_DEF                                     (0x00000003)
  #define DUNIT0_CR_DTR13_TCSH_HSH                                     (0x0700AFC4)

  #define DUNIT0_CR_DTR13_RSVD8_7_OFF                                  ( 7)
  #define DUNIT0_CR_DTR13_RSVD8_7_WID                                  ( 2)
  #define DUNIT0_CR_DTR13_RSVD8_7_MSK                                  (0x00000180)
  #define DUNIT0_CR_DTR13_RSVD8_7_MIN                                  (0)
  #define DUNIT0_CR_DTR13_RSVD8_7_MAX                                  (3) // 0x00000003
  #define DUNIT0_CR_DTR13_RSVD8_7_DEF                                  (0x00000000)
  #define DUNIT0_CR_DTR13_RSVD8_7_HSH                                  (0x020EAFC4)

  #define DUNIT0_CR_DTR13_TCSL_OFF                                     ( 9)
  #define DUNIT0_CR_DTR13_TCSL_WID                                     ( 7)
  #define DUNIT0_CR_DTR13_TCSL_MSK                                     (0x0000FE00)
  #define DUNIT0_CR_DTR13_TCSL_MIN                                     (0)
  #define DUNIT0_CR_DTR13_TCSL_MAX                                     (127) // 0x0000007F
  #define DUNIT0_CR_DTR13_TCSL_DEF                                     (0x00000003)
  #define DUNIT0_CR_DTR13_TCSL_HSH                                     (0x0712AFC4)

  #define DUNIT0_CR_DTR13_RSVD31_16_OFF                                (16)
  #define DUNIT0_CR_DTR13_RSVD31_16_WID                                (16)
  #define DUNIT0_CR_DTR13_RSVD31_16_MSK                                (0xFFFF0000)
  #define DUNIT0_CR_DTR13_RSVD31_16_MIN                                (0)
  #define DUNIT0_CR_DTR13_RSVD31_16_MAX                                (65535) // 0x0000FFFF
  #define DUNIT0_CR_DTR13_RSVD31_16_DEF                                (0x00000000)
  #define DUNIT0_CR_DTR13_RSVD31_16_HSH                                (0x1020AFC4)

#define DUNIT0_CR_DTR14_REG                                            (0x0000AFC8)
#define DUNIT0_CR_DTR14_DEF                                            (0x00000000)
#define DUNIT0_CR_DTR14_VOLATILE_BITFIELDS_MSK                         (0x00000000)

  #define DUNIT0_CR_DTR14_RSVD6_0_OFF                                  ( 0)
  #define DUNIT0_CR_DTR14_RSVD6_0_WID                                  ( 7)
  #define DUNIT0_CR_DTR14_RSVD6_0_MSK                                  (0x0000007F)
  #define DUNIT0_CR_DTR14_RSVD6_0_MIN                                  (0)
  #define DUNIT0_CR_DTR14_RSVD6_0_MAX                                  (127) // 0x0000007F
  #define DUNIT0_CR_DTR14_RSVD6_0_DEF                                  (0x00000000)
  #define DUNIT0_CR_DTR14_RSVD6_0_HSH                                  (0x0700AFC8)

  #define DUNIT0_CR_DTR14_TZQCAL_OFF                                   ( 7)
  #define DUNIT0_CR_DTR14_TZQCAL_WID                                   (14)
  #define DUNIT0_CR_DTR14_TZQCAL_MSK                                   (0x001FFF80)
  #define DUNIT0_CR_DTR14_TZQCAL_MIN                                   (0)
  #define DUNIT0_CR_DTR14_TZQCAL_MAX                                   (16383) // 0x00003FFF
  #define DUNIT0_CR_DTR14_TZQCAL_DEF                                   (0x00000000)
  #define DUNIT0_CR_DTR14_TZQCAL_HSH                                   (0x0E0EAFC8)

  #define DUNIT0_CR_DTR14_RSVD21_OFF                                   (21)
  #define DUNIT0_CR_DTR14_RSVD21_WID                                   ( 1)
  #define DUNIT0_CR_DTR14_RSVD21_MSK                                   (0x00200000)
  #define DUNIT0_CR_DTR14_RSVD21_MIN                                   (0)
  #define DUNIT0_CR_DTR14_RSVD21_MAX                                   (1) // 0x00000001
  #define DUNIT0_CR_DTR14_RSVD21_DEF                                   (0x00000000)
  #define DUNIT0_CR_DTR14_RSVD21_HSH                                   (0x012AAFC8)

  #define DUNIT0_CR_DTR14_TZQLAT_OFF                                   (22)
  #define DUNIT0_CR_DTR14_TZQLAT_WID                                   ( 9)
  #define DUNIT0_CR_DTR14_TZQLAT_MSK                                   (0x7FC00000)
  #define DUNIT0_CR_DTR14_TZQLAT_MIN                                   (0)
  #define DUNIT0_CR_DTR14_TZQLAT_MAX                                   (511) // 0x000001FF
  #define DUNIT0_CR_DTR14_TZQLAT_DEF                                   (0x00000000)
  #define DUNIT0_CR_DTR14_TZQLAT_HSH                                   (0x092CAFC8)

  #define DUNIT0_CR_DTR14_RSVD31_OFF                                   (31)
  #define DUNIT0_CR_DTR14_RSVD31_WID                                   ( 1)
  #define DUNIT0_CR_DTR14_RSVD31_MSK                                   (0x80000000)
  #define DUNIT0_CR_DTR14_RSVD31_MIN                                   (0)
  #define DUNIT0_CR_DTR14_RSVD31_MAX                                   (1) // 0x00000001
  #define DUNIT0_CR_DTR14_RSVD31_DEF                                   (0x00000000)
  #define DUNIT0_CR_DTR14_RSVD31_HSH                                   (0x013EAFC8)

#define DUNIT0_CR_DTR15_REG                                            (0x0000AFCC)
#define DUNIT0_CR_DTR15_DEF                                            (0x00410411)
#define DUNIT0_CR_DTR15_VOLATILE_BITFIELDS_MSK                         (0x00000000)

  #define DUNIT0_CR_DTR15_TWCKENL_FS_OFF                               ( 0)
  #define DUNIT0_CR_DTR15_TWCKENL_FS_WID                               ( 3)
  #define DUNIT0_CR_DTR15_TWCKENL_FS_MSK                               (0x00000007)
  #define DUNIT0_CR_DTR15_TWCKENL_FS_MIN                               (0)
  #define DUNIT0_CR_DTR15_TWCKENL_FS_MAX                               (7) // 0x00000007
  #define DUNIT0_CR_DTR15_TWCKENL_FS_DEF                               (0x00000001)
  #define DUNIT0_CR_DTR15_TWCKENL_FS_HSH                               (0x0300AFCC)

  #define DUNIT0_CR_DTR15_RSVD3_OFF                                    ( 3)
  #define DUNIT0_CR_DTR15_RSVD3_WID                                    ( 1)
  #define DUNIT0_CR_DTR15_RSVD3_MSK                                    (0x00000008)
  #define DUNIT0_CR_DTR15_RSVD3_MIN                                    (0)
  #define DUNIT0_CR_DTR15_RSVD3_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_DTR15_RSVD3_DEF                                    (0x00000000)
  #define DUNIT0_CR_DTR15_RSVD3_HSH                                    (0x0106AFCC)

  #define DUNIT0_CR_DTR15_TWCKENL_RD_OFF                               ( 4)
  #define DUNIT0_CR_DTR15_TWCKENL_RD_WID                               ( 5)
  #define DUNIT0_CR_DTR15_TWCKENL_RD_MSK                               (0x000001F0)
  #define DUNIT0_CR_DTR15_TWCKENL_RD_MIN                               (0)
  #define DUNIT0_CR_DTR15_TWCKENL_RD_MAX                               (31) // 0x0000001F
  #define DUNIT0_CR_DTR15_TWCKENL_RD_DEF                               (0x00000001)
  #define DUNIT0_CR_DTR15_TWCKENL_RD_HSH                               (0x0508AFCC)

  #define DUNIT0_CR_DTR15_RSVD9_OFF                                    ( 9)
  #define DUNIT0_CR_DTR15_RSVD9_WID                                    ( 1)
  #define DUNIT0_CR_DTR15_RSVD9_MSK                                    (0x00000200)
  #define DUNIT0_CR_DTR15_RSVD9_MIN                                    (0)
  #define DUNIT0_CR_DTR15_RSVD9_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_DTR15_RSVD9_DEF                                    (0x00000000)
  #define DUNIT0_CR_DTR15_RSVD9_HSH                                    (0x0112AFCC)

  #define DUNIT0_CR_DTR15_TWCKENL_WR_OFF                               (10)
  #define DUNIT0_CR_DTR15_TWCKENL_WR_WID                               ( 5)
  #define DUNIT0_CR_DTR15_TWCKENL_WR_MSK                               (0x00007C00)
  #define DUNIT0_CR_DTR15_TWCKENL_WR_MIN                               (0)
  #define DUNIT0_CR_DTR15_TWCKENL_WR_MAX                               (31) // 0x0000001F
  #define DUNIT0_CR_DTR15_TWCKENL_WR_DEF                               (0x00000001)
  #define DUNIT0_CR_DTR15_TWCKENL_WR_HSH                               (0x0514AFCC)

  #define DUNIT0_CR_DTR15_RSVD15_OFF                                   (15)
  #define DUNIT0_CR_DTR15_RSVD15_WID                                   ( 1)
  #define DUNIT0_CR_DTR15_RSVD15_MSK                                   (0x00008000)
  #define DUNIT0_CR_DTR15_RSVD15_MIN                                   (0)
  #define DUNIT0_CR_DTR15_RSVD15_MAX                                   (1) // 0x00000001
  #define DUNIT0_CR_DTR15_RSVD15_DEF                                   (0x00000000)
  #define DUNIT0_CR_DTR15_RSVD15_HSH                                   (0x011EAFCC)

  #define DUNIT0_CR_DTR15_TWCKPRE_STATIC_OFF                           (16)
  #define DUNIT0_CR_DTR15_TWCKPRE_STATIC_WID                           ( 4)
  #define DUNIT0_CR_DTR15_TWCKPRE_STATIC_MSK                           (0x000F0000)
  #define DUNIT0_CR_DTR15_TWCKPRE_STATIC_MIN                           (0)
  #define DUNIT0_CR_DTR15_TWCKPRE_STATIC_MAX                           (15) // 0x0000000F
  #define DUNIT0_CR_DTR15_TWCKPRE_STATIC_DEF                           (0x00000001)
  #define DUNIT0_CR_DTR15_TWCKPRE_STATIC_HSH                           (0x0420AFCC)

  #define DUNIT0_CR_DTR15_RSVD20_OFF                                   (20)
  #define DUNIT0_CR_DTR15_RSVD20_WID                                   ( 1)
  #define DUNIT0_CR_DTR15_RSVD20_MSK                                   (0x00100000)
  #define DUNIT0_CR_DTR15_RSVD20_MIN                                   (0)
  #define DUNIT0_CR_DTR15_RSVD20_MAX                                   (1) // 0x00000001
  #define DUNIT0_CR_DTR15_RSVD20_DEF                                   (0x00000000)
  #define DUNIT0_CR_DTR15_RSVD20_HSH                                   (0x0128AFCC)

  #define DUNIT0_CR_DTR15_TWCKSTOP_OFF                                 (21)
  #define DUNIT0_CR_DTR15_TWCKSTOP_WID                                 ( 4)
  #define DUNIT0_CR_DTR15_TWCKSTOP_MSK                                 (0x01E00000)
  #define DUNIT0_CR_DTR15_TWCKSTOP_MIN                                 (0)
  #define DUNIT0_CR_DTR15_TWCKSTOP_MAX                                 (15) // 0x0000000F
  #define DUNIT0_CR_DTR15_TWCKSTOP_DEF                                 (0x00000002)
  #define DUNIT0_CR_DTR15_TWCKSTOP_HSH                                 (0x042AAFCC)

  #define DUNIT0_CR_DTR15_RSVD30_25_OFF                                (25)
  #define DUNIT0_CR_DTR15_RSVD30_25_WID                                ( 6)
  #define DUNIT0_CR_DTR15_RSVD30_25_MSK                                (0x7E000000)
  #define DUNIT0_CR_DTR15_RSVD30_25_MIN                                (0)
  #define DUNIT0_CR_DTR15_RSVD30_25_MAX                                (63) // 0x0000003F
  #define DUNIT0_CR_DTR15_RSVD30_25_DEF                                (0x00000000)
  #define DUNIT0_CR_DTR15_RSVD30_25_HSH                                (0x0632AFCC)

  #define DUNIT0_CR_DTR15_WCK_CTRL_OFF                                 (31)
  #define DUNIT0_CR_DTR15_WCK_CTRL_WID                                 ( 1)
  #define DUNIT0_CR_DTR15_WCK_CTRL_MSK                                 (0x80000000)
  #define DUNIT0_CR_DTR15_WCK_CTRL_MIN                                 (0)
  #define DUNIT0_CR_DTR15_WCK_CTRL_MAX                                 (1) // 0x00000001
  #define DUNIT0_CR_DTR15_WCK_CTRL_DEF                                 (0x00000000)
  #define DUNIT0_CR_DTR15_WCK_CTRL_HSH                                 (0x013EAFCC)

#define DUNIT0_CR_DTR16_REG                                            (0x0000AFD0)
#define DUNIT0_CR_DTR16_DEF                                            (0x00000000)
#define DUNIT0_CR_DTR16_VOLATILE_BITFIELDS_MSK                         (0x00000000)

  #define DUNIT0_CR_DTR16_RD2WCKOFF_OFF                                ( 0)
  #define DUNIT0_CR_DTR16_RD2WCKOFF_WID                                ( 6)
  #define DUNIT0_CR_DTR16_RD2WCKOFF_MSK                                (0x0000003F)
  #define DUNIT0_CR_DTR16_RD2WCKOFF_MIN                                (0)
  #define DUNIT0_CR_DTR16_RD2WCKOFF_MAX                                (63) // 0x0000003F
  #define DUNIT0_CR_DTR16_RD2WCKOFF_DEF                                (0x00000000)
  #define DUNIT0_CR_DTR16_RD2WCKOFF_HSH                                (0x0600AFD0)

  #define DUNIT0_CR_DTR16_RSVD6_OFF                                    ( 6)
  #define DUNIT0_CR_DTR16_RSVD6_WID                                    ( 1)
  #define DUNIT0_CR_DTR16_RSVD6_MSK                                    (0x00000040)
  #define DUNIT0_CR_DTR16_RSVD6_MIN                                    (0)
  #define DUNIT0_CR_DTR16_RSVD6_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_DTR16_RSVD6_DEF                                    (0x00000000)
  #define DUNIT0_CR_DTR16_RSVD6_HSH                                    (0x010CAFD0)

  #define DUNIT0_CR_DTR16_WR2WCKOFF_OFF                                ( 7)
  #define DUNIT0_CR_DTR16_WR2WCKOFF_WID                                ( 6)
  #define DUNIT0_CR_DTR16_WR2WCKOFF_MSK                                (0x00001F80)
  #define DUNIT0_CR_DTR16_WR2WCKOFF_MIN                                (0)
  #define DUNIT0_CR_DTR16_WR2WCKOFF_MAX                                (63) // 0x0000003F
  #define DUNIT0_CR_DTR16_WR2WCKOFF_DEF                                (0x00000000)
  #define DUNIT0_CR_DTR16_WR2WCKOFF_HSH                                (0x060EAFD0)

  #define DUNIT0_CR_DTR16_RSVD31_13_OFF                                (13)
  #define DUNIT0_CR_DTR16_RSVD31_13_WID                                (19)
  #define DUNIT0_CR_DTR16_RSVD31_13_MSK                                (0xFFFFE000)
  #define DUNIT0_CR_DTR16_RSVD31_13_MIN                                (0)
  #define DUNIT0_CR_DTR16_RSVD31_13_MAX                                (524287) // 0x0007FFFF
  #define DUNIT0_CR_DTR16_RSVD31_13_DEF                                (0x00000000)
  #define DUNIT0_CR_DTR16_RSVD31_13_HSH                                (0x131AAFD0)

#define DUNIT0_CR_DTR17_REG                                            (0x0000AFD4)
#define DUNIT0_CR_DTR17_DEF                                            (0x00000000)
#define DUNIT0_CR_DTR17_VOLATILE_BITFIELDS_MSK                         (0x00000000)

  #define DUNIT0_CR_DTR17_TRRDD_OFF                                    ( 0)
  #define DUNIT0_CR_DTR17_TRRDD_WID                                    ( 6)
  #define DUNIT0_CR_DTR17_TRRDD_MSK                                    (0x0000003F)
  #define DUNIT0_CR_DTR17_TRRDD_MIN                                    (0)
  #define DUNIT0_CR_DTR17_TRRDD_MAX                                    (63) // 0x0000003F
  #define DUNIT0_CR_DTR17_TRRDD_DEF                                    (0x00000000)
  #define DUNIT0_CR_DTR17_TRRDD_HSH                                    (0x0600AFD4)

  #define DUNIT0_CR_DTR17_RSVD6_OFF                                    ( 6)
  #define DUNIT0_CR_DTR17_RSVD6_WID                                    ( 1)
  #define DUNIT0_CR_DTR17_RSVD6_MSK                                    (0x00000040)
  #define DUNIT0_CR_DTR17_RSVD6_MIN                                    (0)
  #define DUNIT0_CR_DTR17_RSVD6_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_DTR17_RSVD6_DEF                                    (0x00000000)
  #define DUNIT0_CR_DTR17_RSVD6_HSH                                    (0x010CAFD4)

  #define DUNIT0_CR_DTR17_TRWDD_OFF                                    ( 7)
  #define DUNIT0_CR_DTR17_TRWDD_WID                                    ( 7)
  #define DUNIT0_CR_DTR17_TRWDD_MSK                                    (0x00003F80)
  #define DUNIT0_CR_DTR17_TRWDD_MIN                                    (0)
  #define DUNIT0_CR_DTR17_TRWDD_MAX                                    (127) // 0x0000007F
  #define DUNIT0_CR_DTR17_TRWDD_DEF                                    (0x00000000)
  #define DUNIT0_CR_DTR17_TRWDD_HSH                                    (0x070EAFD4)

  #define DUNIT0_CR_DTR17_RSVD15_14_OFF                                (14)
  #define DUNIT0_CR_DTR17_RSVD15_14_WID                                ( 2)
  #define DUNIT0_CR_DTR17_RSVD15_14_MSK                                (0x0000C000)
  #define DUNIT0_CR_DTR17_RSVD15_14_MIN                                (0)
  #define DUNIT0_CR_DTR17_RSVD15_14_MAX                                (3) // 0x00000003
  #define DUNIT0_CR_DTR17_RSVD15_14_DEF                                (0x00000000)
  #define DUNIT0_CR_DTR17_RSVD15_14_HSH                                (0x021CAFD4)

  #define DUNIT0_CR_DTR17_TWWDD_OFF                                    (16)
  #define DUNIT0_CR_DTR17_TWWDD_WID                                    ( 7)
  #define DUNIT0_CR_DTR17_TWWDD_MSK                                    (0x007F0000)
  #define DUNIT0_CR_DTR17_TWWDD_MIN                                    (0)
  #define DUNIT0_CR_DTR17_TWWDD_MAX                                    (127) // 0x0000007F
  #define DUNIT0_CR_DTR17_TWWDD_DEF                                    (0x00000000)
  #define DUNIT0_CR_DTR17_TWWDD_HSH                                    (0x0720AFD4)

  #define DUNIT0_CR_DTR17_RSVD23_OFF                                   (23)
  #define DUNIT0_CR_DTR17_RSVD23_WID                                   ( 1)
  #define DUNIT0_CR_DTR17_RSVD23_MSK                                   (0x00800000)
  #define DUNIT0_CR_DTR17_RSVD23_MIN                                   (0)
  #define DUNIT0_CR_DTR17_RSVD23_MAX                                   (1) // 0x00000001
  #define DUNIT0_CR_DTR17_RSVD23_DEF                                   (0x00000000)
  #define DUNIT0_CR_DTR17_RSVD23_HSH                                   (0x012EAFD4)

  #define DUNIT0_CR_DTR17_TWRDD_OFF                                    (24)
  #define DUNIT0_CR_DTR17_TWRDD_WID                                    ( 6)
  #define DUNIT0_CR_DTR17_TWRDD_MSK                                    (0x3F000000)
  #define DUNIT0_CR_DTR17_TWRDD_MIN                                    (0)
  #define DUNIT0_CR_DTR17_TWRDD_MAX                                    (63) // 0x0000003F
  #define DUNIT0_CR_DTR17_TWRDD_DEF                                    (0x00000000)
  #define DUNIT0_CR_DTR17_TWRDD_HSH                                    (0x0630AFD4)

  #define DUNIT0_CR_DTR17_RSVD31_30_OFF                                (30)
  #define DUNIT0_CR_DTR17_RSVD31_30_WID                                ( 2)
  #define DUNIT0_CR_DTR17_RSVD31_30_MSK                                (0xC0000000)
  #define DUNIT0_CR_DTR17_RSVD31_30_MIN                                (0)
  #define DUNIT0_CR_DTR17_RSVD31_30_MAX                                (3) // 0x00000003
  #define DUNIT0_CR_DTR17_RSVD31_30_DEF                                (0x00000000)
  #define DUNIT0_CR_DTR17_RSVD31_30_HSH                                (0x023CAFD4)

#define DUNIT0_CR_ODT1_REG                                             (0x0000AFE4)
#define DUNIT0_CR_ODT1_DEF                                             (0x00000000)
#define DUNIT0_CR_ODT1_VOLATILE_BITFIELDS_MSK                          (0x00000000)

  #define DUNIT0_CR_ODT1_R0_WR_ODT_CTL_OFF                             ( 0)
  #define DUNIT0_CR_ODT1_R0_WR_ODT_CTL_WID                             ( 3)
  #define DUNIT0_CR_ODT1_R0_WR_ODT_CTL_MSK                             (0x00000007)
  #define DUNIT0_CR_ODT1_R0_WR_ODT_CTL_MIN                             (0)
  #define DUNIT0_CR_ODT1_R0_WR_ODT_CTL_MAX                             (7) // 0x00000007
  #define DUNIT0_CR_ODT1_R0_WR_ODT_CTL_DEF                             (0x00000000)
  #define DUNIT0_CR_ODT1_R0_WR_ODT_CTL_HSH                             (0x0300AFE4)

  #define DUNIT0_CR_ODT1_RSVD3_OFF                                     ( 3)
  #define DUNIT0_CR_ODT1_RSVD3_WID                                     ( 1)
  #define DUNIT0_CR_ODT1_RSVD3_MSK                                     (0x00000008)
  #define DUNIT0_CR_ODT1_RSVD3_MIN                                     (0)
  #define DUNIT0_CR_ODT1_RSVD3_MAX                                     (1) // 0x00000001
  #define DUNIT0_CR_ODT1_RSVD3_DEF                                     (0x00000000)
  #define DUNIT0_CR_ODT1_RSVD3_HSH                                     (0x0106AFE4)

  #define DUNIT0_CR_ODT1_R1_WR_ODT_CTL_OFF                             ( 4)
  #define DUNIT0_CR_ODT1_R1_WR_ODT_CTL_WID                             ( 3)
  #define DUNIT0_CR_ODT1_R1_WR_ODT_CTL_MSK                             (0x00000070)
  #define DUNIT0_CR_ODT1_R1_WR_ODT_CTL_MIN                             (0)
  #define DUNIT0_CR_ODT1_R1_WR_ODT_CTL_MAX                             (7) // 0x00000007
  #define DUNIT0_CR_ODT1_R1_WR_ODT_CTL_DEF                             (0x00000000)
  #define DUNIT0_CR_ODT1_R1_WR_ODT_CTL_HSH                             (0x0308AFE4)

  #define DUNIT0_CR_ODT1_RSVD7_OFF                                     ( 7)
  #define DUNIT0_CR_ODT1_RSVD7_WID                                     ( 1)
  #define DUNIT0_CR_ODT1_RSVD7_MSK                                     (0x00000080)
  #define DUNIT0_CR_ODT1_RSVD7_MIN                                     (0)
  #define DUNIT0_CR_ODT1_RSVD7_MAX                                     (1) // 0x00000001
  #define DUNIT0_CR_ODT1_RSVD7_DEF                                     (0x00000000)
  #define DUNIT0_CR_ODT1_RSVD7_HSH                                     (0x010EAFE4)

  #define DUNIT0_CR_ODT1_R2_WR_ODT_CTL_OFF                             ( 8)
  #define DUNIT0_CR_ODT1_R2_WR_ODT_CTL_WID                             ( 3)
  #define DUNIT0_CR_ODT1_R2_WR_ODT_CTL_MSK                             (0x00000700)
  #define DUNIT0_CR_ODT1_R2_WR_ODT_CTL_MIN                             (0)
  #define DUNIT0_CR_ODT1_R2_WR_ODT_CTL_MAX                             (7) // 0x00000007
  #define DUNIT0_CR_ODT1_R2_WR_ODT_CTL_DEF                             (0x00000000)
  #define DUNIT0_CR_ODT1_R2_WR_ODT_CTL_HSH                             (0x0310AFE4)

  #define DUNIT0_CR_ODT1_RSVD11_OFF                                    (11)
  #define DUNIT0_CR_ODT1_RSVD11_WID                                    ( 1)
  #define DUNIT0_CR_ODT1_RSVD11_MSK                                    (0x00000800)
  #define DUNIT0_CR_ODT1_RSVD11_MIN                                    (0)
  #define DUNIT0_CR_ODT1_RSVD11_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_ODT1_RSVD11_DEF                                    (0x00000000)
  #define DUNIT0_CR_ODT1_RSVD11_HSH                                    (0x0116AFE4)

  #define DUNIT0_CR_ODT1_R3_WR_ODT_CTL_OFF                             (12)
  #define DUNIT0_CR_ODT1_R3_WR_ODT_CTL_WID                             ( 3)
  #define DUNIT0_CR_ODT1_R3_WR_ODT_CTL_MSK                             (0x00007000)
  #define DUNIT0_CR_ODT1_R3_WR_ODT_CTL_MIN                             (0)
  #define DUNIT0_CR_ODT1_R3_WR_ODT_CTL_MAX                             (7) // 0x00000007
  #define DUNIT0_CR_ODT1_R3_WR_ODT_CTL_DEF                             (0x00000000)
  #define DUNIT0_CR_ODT1_R3_WR_ODT_CTL_HSH                             (0x0318AFE4)

  #define DUNIT0_CR_ODT1_RSVD15_OFF                                    (15)
  #define DUNIT0_CR_ODT1_RSVD15_WID                                    ( 1)
  #define DUNIT0_CR_ODT1_RSVD15_MSK                                    (0x00008000)
  #define DUNIT0_CR_ODT1_RSVD15_MIN                                    (0)
  #define DUNIT0_CR_ODT1_RSVD15_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_ODT1_RSVD15_DEF                                    (0x00000000)
  #define DUNIT0_CR_ODT1_RSVD15_HSH                                    (0x011EAFE4)

  #define DUNIT0_CR_ODT1_R0_RD_ODT_CTL_OFF                             (16)
  #define DUNIT0_CR_ODT1_R0_RD_ODT_CTL_WID                             ( 3)
  #define DUNIT0_CR_ODT1_R0_RD_ODT_CTL_MSK                             (0x00070000)
  #define DUNIT0_CR_ODT1_R0_RD_ODT_CTL_MIN                             (0)
  #define DUNIT0_CR_ODT1_R0_RD_ODT_CTL_MAX                             (7) // 0x00000007
  #define DUNIT0_CR_ODT1_R0_RD_ODT_CTL_DEF                             (0x00000000)
  #define DUNIT0_CR_ODT1_R0_RD_ODT_CTL_HSH                             (0x0320AFE4)

  #define DUNIT0_CR_ODT1_RSVD19_OFF                                    (19)
  #define DUNIT0_CR_ODT1_RSVD19_WID                                    ( 1)
  #define DUNIT0_CR_ODT1_RSVD19_MSK                                    (0x00080000)
  #define DUNIT0_CR_ODT1_RSVD19_MIN                                    (0)
  #define DUNIT0_CR_ODT1_RSVD19_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_ODT1_RSVD19_DEF                                    (0x00000000)
  #define DUNIT0_CR_ODT1_RSVD19_HSH                                    (0x0126AFE4)

  #define DUNIT0_CR_ODT1_R1_RD_ODT_CTL_OFF                             (20)
  #define DUNIT0_CR_ODT1_R1_RD_ODT_CTL_WID                             ( 3)
  #define DUNIT0_CR_ODT1_R1_RD_ODT_CTL_MSK                             (0x00700000)
  #define DUNIT0_CR_ODT1_R1_RD_ODT_CTL_MIN                             (0)
  #define DUNIT0_CR_ODT1_R1_RD_ODT_CTL_MAX                             (7) // 0x00000007
  #define DUNIT0_CR_ODT1_R1_RD_ODT_CTL_DEF                             (0x00000000)
  #define DUNIT0_CR_ODT1_R1_RD_ODT_CTL_HSH                             (0x0328AFE4)

  #define DUNIT0_CR_ODT1_RSVD23_OFF                                    (23)
  #define DUNIT0_CR_ODT1_RSVD23_WID                                    ( 1)
  #define DUNIT0_CR_ODT1_RSVD23_MSK                                    (0x00800000)
  #define DUNIT0_CR_ODT1_RSVD23_MIN                                    (0)
  #define DUNIT0_CR_ODT1_RSVD23_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_ODT1_RSVD23_DEF                                    (0x00000000)
  #define DUNIT0_CR_ODT1_RSVD23_HSH                                    (0x012EAFE4)

  #define DUNIT0_CR_ODT1_R2_RD_ODT_CTL_OFF                             (24)
  #define DUNIT0_CR_ODT1_R2_RD_ODT_CTL_WID                             ( 3)
  #define DUNIT0_CR_ODT1_R2_RD_ODT_CTL_MSK                             (0x07000000)
  #define DUNIT0_CR_ODT1_R2_RD_ODT_CTL_MIN                             (0)
  #define DUNIT0_CR_ODT1_R2_RD_ODT_CTL_MAX                             (7) // 0x00000007
  #define DUNIT0_CR_ODT1_R2_RD_ODT_CTL_DEF                             (0x00000000)
  #define DUNIT0_CR_ODT1_R2_RD_ODT_CTL_HSH                             (0x0330AFE4)

  #define DUNIT0_CR_ODT1_RSVD27_OFF                                    (27)
  #define DUNIT0_CR_ODT1_RSVD27_WID                                    ( 1)
  #define DUNIT0_CR_ODT1_RSVD27_MSK                                    (0x08000000)
  #define DUNIT0_CR_ODT1_RSVD27_MIN                                    (0)
  #define DUNIT0_CR_ODT1_RSVD27_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_ODT1_RSVD27_DEF                                    (0x00000000)
  #define DUNIT0_CR_ODT1_RSVD27_HSH                                    (0x0136AFE4)

  #define DUNIT0_CR_ODT1_R3_RD_ODT_CTL_OFF                             (28)
  #define DUNIT0_CR_ODT1_R3_RD_ODT_CTL_WID                             ( 3)
  #define DUNIT0_CR_ODT1_R3_RD_ODT_CTL_MSK                             (0x70000000)
  #define DUNIT0_CR_ODT1_R3_RD_ODT_CTL_MIN                             (0)
  #define DUNIT0_CR_ODT1_R3_RD_ODT_CTL_MAX                             (7) // 0x00000007
  #define DUNIT0_CR_ODT1_R3_RD_ODT_CTL_DEF                             (0x00000000)
  #define DUNIT0_CR_ODT1_R3_RD_ODT_CTL_HSH                             (0x0338AFE4)

  #define DUNIT0_CR_ODT1_RSVD31_OFF                                    (31)
  #define DUNIT0_CR_ODT1_RSVD31_WID                                    ( 1)
  #define DUNIT0_CR_ODT1_RSVD31_MSK                                    (0x80000000)
  #define DUNIT0_CR_ODT1_RSVD31_MIN                                    (0)
  #define DUNIT0_CR_ODT1_RSVD31_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_ODT1_RSVD31_DEF                                    (0x00000000)
  #define DUNIT0_CR_ODT1_RSVD31_HSH                                    (0x013EAFE4)

#define DUNIT0_CR_PD0_REG                                              (0x0000AFF0)
#define DUNIT0_CR_PD0_DEF                                              (0x00000040)
#define DUNIT0_CR_PD0_VOLATILE_BITFIELDS_MSK                           (0x60000000)

  #define DUNIT0_CR_PD0_PDE_DLY_OFF                                    ( 0)
  #define DUNIT0_CR_PD0_PDE_DLY_WID                                    (10)
  #define DUNIT0_CR_PD0_PDE_DLY_MSK                                    (0x000003FF)
  #define DUNIT0_CR_PD0_PDE_DLY_MIN                                    (0)
  #define DUNIT0_CR_PD0_PDE_DLY_MAX                                    (1023) // 0x000003FF
  #define DUNIT0_CR_PD0_PDE_DLY_DEF                                    (0x00000040)
  #define DUNIT0_CR_PD0_PDE_DLY_HSH                                    (0x0A00AFF0)

  #define DUNIT0_CR_PD0_RSVD27_10_OFF                                  (10)
  #define DUNIT0_CR_PD0_RSVD27_10_WID                                  (18)
  #define DUNIT0_CR_PD0_RSVD27_10_MSK                                  (0x0FFFFC00)
  #define DUNIT0_CR_PD0_RSVD27_10_MIN                                  (0)
  #define DUNIT0_CR_PD0_RSVD27_10_MAX                                  (262143) // 0x0003FFFF
  #define DUNIT0_CR_PD0_RSVD27_10_DEF                                  (0x00000000)
  #define DUNIT0_CR_PD0_RSVD27_10_HSH                                  (0x1214AFF0)

  #define DUNIT0_CR_PD0_PREA_BEFORE_PDE_OFF                            (28)
  #define DUNIT0_CR_PD0_PREA_BEFORE_PDE_WID                            ( 1)
  #define DUNIT0_CR_PD0_PREA_BEFORE_PDE_MSK                            (0x10000000)
  #define DUNIT0_CR_PD0_PREA_BEFORE_PDE_MIN                            (0)
  #define DUNIT0_CR_PD0_PREA_BEFORE_PDE_MAX                            (1) // 0x00000001
  #define DUNIT0_CR_PD0_PREA_BEFORE_PDE_DEF                            (0x00000000)
  #define DUNIT0_CR_PD0_PREA_BEFORE_PDE_HSH                            (0x0138AFF0)

  #define DUNIT0_CR_PD0_MPDE_OFF                                       (29)
  #define DUNIT0_CR_PD0_MPDE_WID                                       ( 1)
  #define DUNIT0_CR_PD0_MPDE_MSK                                       (0x20000000)
  #define DUNIT0_CR_PD0_MPDE_MIN                                       (0)
  #define DUNIT0_CR_PD0_MPDE_MAX                                       (1) // 0x00000001
  #define DUNIT0_CR_PD0_MPDE_DEF                                       (0x00000000)
  #define DUNIT0_CR_PD0_MPDE_HSH                                       (0x01BAAFF0)

  #define DUNIT0_CR_PD0_MPDX_OFF                                       (30)
  #define DUNIT0_CR_PD0_MPDX_WID                                       ( 1)
  #define DUNIT0_CR_PD0_MPDX_MSK                                       (0x40000000)
  #define DUNIT0_CR_PD0_MPDX_MIN                                       (0)
  #define DUNIT0_CR_PD0_MPDX_MAX                                       (1) // 0x00000001
  #define DUNIT0_CR_PD0_MPDX_DEF                                       (0x00000000)
  #define DUNIT0_CR_PD0_MPDX_HSH                                       (0x01BCAFF0)

  #define DUNIT0_CR_PD0_PD_DIS_OFF                                     (31)
  #define DUNIT0_CR_PD0_PD_DIS_WID                                     ( 1)
  #define DUNIT0_CR_PD0_PD_DIS_MSK                                     (0x80000000)
  #define DUNIT0_CR_PD0_PD_DIS_MIN                                     (0)
  #define DUNIT0_CR_PD0_PD_DIS_MAX                                     (1) // 0x00000001
  #define DUNIT0_CR_PD0_PD_DIS_DEF                                     (0x00000000)
  #define DUNIT0_CR_PD0_PD_DIS_HSH                                     (0x013EAFF0)

#define DUNIT0_CR_PD2_REG                                              (0x0000AFF8)
#define DUNIT0_CR_PD2_DEF                                              (0x00000040)
#define DUNIT0_CR_PD2_VOLATILE_BITFIELDS_MSK                           (0x00000000)

  #define DUNIT0_CR_PD2_STOP_CK_OFF                                    ( 0)
  #define DUNIT0_CR_PD2_STOP_CK_WID                                    ( 1)
  #define DUNIT0_CR_PD2_STOP_CK_MSK                                    (0x00000001)
  #define DUNIT0_CR_PD2_STOP_CK_MIN                                    (0)
  #define DUNIT0_CR_PD2_STOP_CK_MAX                                    (1) // 0x00000001
  #define DUNIT0_CR_PD2_STOP_CK_DEF                                    (0x00000000)
  #define DUNIT0_CR_PD2_STOP_CK_HSH                                    (0x0100AFF8)

  #define DUNIT0_CR_PD2_RSVD2_1_OFF                                    ( 1)
  #define DUNIT0_CR_PD2_RSVD2_1_WID                                    ( 2)
  #define DUNIT0_CR_PD2_RSVD2_1_MSK                                    (0x00000006)
  #define DUNIT0_CR_PD2_RSVD2_1_MIN                                    (0)
  #define DUNIT0_CR_PD2_RSVD2_1_MAX                                    (3) // 0x00000003
  #define DUNIT0_CR_PD2_RSVD2_1_DEF                                    (0x00000000)
  #define DUNIT0_CR_PD2_RSVD2_1_HSH                                    (0x0202AFF8)

  #define DUNIT0_CR_PD2_TRI_CA_OFF                                     ( 3)
  #define DUNIT0_CR_PD2_TRI_CA_WID                                     ( 2)
  #define DUNIT0_CR_PD2_TRI_CA_MSK                                     (0x00000018)
  #define DUNIT0_CR_PD2_TRI_CA_MIN                                     (0)
  #define DUNIT0_CR_PD2_TRI_CA_MAX                                     (3) // 0x00000003
  #define DUNIT0_CR_PD2_TRI_CA_DEF                                     (0x00000000)
  #define DUNIT0_CR_PD2_TRI_CA_HSH                                     (0x0206AFF8)

  #define DUNIT0_CR_PD2_RSVD5_OFF                                      ( 5)
  #define DUNIT0_CR_PD2_RSVD5_WID                                      ( 1)
  #define DUNIT0_CR_PD2_RSVD5_MSK                                      (0x00000020)
  #define DUNIT0_CR_PD2_RSVD5_MIN                                      (0)
  #define DUNIT0_CR_PD2_RSVD5_MAX                                      (1) // 0x00000001
  #define DUNIT0_CR_PD2_RSVD5_DEF                                      (0x00000000)
  #define DUNIT0_CR_PD2_RSVD5_HSH                                      (0x010AAFF8)

  #define DUNIT0_CR_PD2_FORCE_CKSTOP_OFF                               ( 6)
  #define DUNIT0_CR_PD2_FORCE_CKSTOP_WID                               ( 1)
  #define DUNIT0_CR_PD2_FORCE_CKSTOP_MSK                               (0x00000040)
  #define DUNIT0_CR_PD2_FORCE_CKSTOP_MIN                               (0)
  #define DUNIT0_CR_PD2_FORCE_CKSTOP_MAX                               (1) // 0x00000001
  #define DUNIT0_CR_PD2_FORCE_CKSTOP_DEF                               (0x00000001)
  #define DUNIT0_CR_PD2_FORCE_CKSTOP_HSH                               (0x010CAFF8)

  #define DUNIT0_CR_PD2_CKSTOP_DLY_OFF                                 ( 7)
  #define DUNIT0_CR_PD2_CKSTOP_DLY_WID                                 ( 3)
  #define DUNIT0_CR_PD2_CKSTOP_DLY_MSK                                 (0x00000380)
  #define DUNIT0_CR_PD2_CKSTOP_DLY_MIN                                 (0)
  #define DUNIT0_CR_PD2_CKSTOP_DLY_MAX                                 (7) // 0x00000007
  #define DUNIT0_CR_PD2_CKSTOP_DLY_DEF                                 (0x00000000)
  #define DUNIT0_CR_PD2_CKSTOP_DLY_HSH                                 (0x030EAFF8)

  #define DUNIT0_CR_PD2_RSVD31_10_OFF                                  (10)
  #define DUNIT0_CR_PD2_RSVD31_10_WID                                  (22)
  #define DUNIT0_CR_PD2_RSVD31_10_MSK                                  (0xFFFFFC00)
  #define DUNIT0_CR_PD2_RSVD31_10_MIN                                  (0)
  #define DUNIT0_CR_PD2_RSVD31_10_MAX                                  (4194303) // 0x003FFFFF
  #define DUNIT0_CR_PD2_RSVD31_10_DEF                                  (0x00000000)
  #define DUNIT0_CR_PD2_RSVD31_10_HSH                                  (0x1614AFF8)



#pragma pack(pop)
#endif /* __MrcRegisterPtlAxxx_h__ */







