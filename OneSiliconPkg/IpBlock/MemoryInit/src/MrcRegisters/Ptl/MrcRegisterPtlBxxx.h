#ifndef __MrcRegisterPtlBxxx_h__
#define __MrcRegisterPtlBxxx_h__

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


#define DUNIT0_CR_D_CR_SR0_REG                                         (0x0000B000)
#define DUNIT0_CR_D_CR_SR0_DEF                                         (0x00000020)
#define DUNIT0_CR_D_CR_SR0_VOLATILE_BITFIELDS_MSK                      (0x60000000)

  #define DUNIT0_CR_D_CR_SR0_SRE_DLY_OFF                               ( 0)
  #define DUNIT0_CR_D_CR_SR0_SRE_DLY_WID                               (16)
  #define DUNIT0_CR_D_CR_SR0_SRE_DLY_MSK                               (0x0000FFFF)
  #define DUNIT0_CR_D_CR_SR0_SRE_DLY_MIN                               (0)
  #define DUNIT0_CR_D_CR_SR0_SRE_DLY_MAX                               (65535) // 0x0000FFFF
  #define DUNIT0_CR_D_CR_SR0_SRE_DLY_DEF                               (0x00000020)
  #define DUNIT0_CR_D_CR_SR0_SRE_DLY_HSH                               (0x1000B000)

  #define DUNIT0_CR_D_CR_SR0_RSVD18_16_OFF                             (16)
  #define DUNIT0_CR_D_CR_SR0_RSVD18_16_WID                             ( 3)
  #define DUNIT0_CR_D_CR_SR0_RSVD18_16_MSK                             (0x00070000)
  #define DUNIT0_CR_D_CR_SR0_RSVD18_16_MIN                             (0)
  #define DUNIT0_CR_D_CR_SR0_RSVD18_16_MAX                             (7) // 0x00000007
  #define DUNIT0_CR_D_CR_SR0_RSVD18_16_DEF                             (0x00000000)
  #define DUNIT0_CR_D_CR_SR0_RSVD18_16_HSH                             (0x0320B000)

  #define DUNIT0_CR_D_CR_SR0_DYN_SR_EN_OFF                             (19)
  #define DUNIT0_CR_D_CR_SR0_DYN_SR_EN_WID                             ( 1)
  #define DUNIT0_CR_D_CR_SR0_DYN_SR_EN_MSK                             (0x00080000)
  #define DUNIT0_CR_D_CR_SR0_DYN_SR_EN_MIN                             (0)
  #define DUNIT0_CR_D_CR_SR0_DYN_SR_EN_MAX                             (1) // 0x00000001
  #define DUNIT0_CR_D_CR_SR0_DYN_SR_EN_DEF                             (0x00000000)
  #define DUNIT0_CR_D_CR_SR0_DYN_SR_EN_HSH                             (0x0126B000)

  #define DUNIT0_CR_D_CR_SR0_RSVD24_20_OFF                             (20)
  #define DUNIT0_CR_D_CR_SR0_RSVD24_20_WID                             ( 5)
  #define DUNIT0_CR_D_CR_SR0_RSVD24_20_MSK                             (0x01F00000)
  #define DUNIT0_CR_D_CR_SR0_RSVD24_20_MIN                             (0)
  #define DUNIT0_CR_D_CR_SR0_RSVD24_20_MAX                             (31) // 0x0000001F
  #define DUNIT0_CR_D_CR_SR0_RSVD24_20_DEF                             (0x00000000)
  #define DUNIT0_CR_D_CR_SR0_RSVD24_20_HSH                             (0x0528B000)

  #define DUNIT0_CR_D_CR_SR0_PHY_IC_QUAL_EN_OFF                        (25)
  #define DUNIT0_CR_D_CR_SR0_PHY_IC_QUAL_EN_WID                        ( 1)
  #define DUNIT0_CR_D_CR_SR0_PHY_IC_QUAL_EN_MSK                        (0x02000000)
  #define DUNIT0_CR_D_CR_SR0_PHY_IC_QUAL_EN_MIN                        (0)
  #define DUNIT0_CR_D_CR_SR0_PHY_IC_QUAL_EN_MAX                        (1) // 0x00000001
  #define DUNIT0_CR_D_CR_SR0_PHY_IC_QUAL_EN_DEF                        (0x00000000)
  #define DUNIT0_CR_D_CR_SR0_PHY_IC_QUAL_EN_HSH                        (0x0132B000)

  #define DUNIT0_CR_D_CR_SR0_DFI_INIT_START_CTRL_OFF                   (26)
  #define DUNIT0_CR_D_CR_SR0_DFI_INIT_START_CTRL_WID                   ( 2)
  #define DUNIT0_CR_D_CR_SR0_DFI_INIT_START_CTRL_MSK                   (0x0C000000)
  #define DUNIT0_CR_D_CR_SR0_DFI_INIT_START_CTRL_MIN                   (0)
  #define DUNIT0_CR_D_CR_SR0_DFI_INIT_START_CTRL_MAX                   (3) // 0x00000003
  #define DUNIT0_CR_D_CR_SR0_DFI_INIT_START_CTRL_DEF                   (0x00000000)
  #define DUNIT0_CR_D_CR_SR0_DFI_INIT_START_CTRL_HSH                   (0x0234B000)

  #define DUNIT0_CR_D_CR_SR0_SRE_WFC_OFF                               (28)
  #define DUNIT0_CR_D_CR_SR0_SRE_WFC_WID                               ( 1)
  #define DUNIT0_CR_D_CR_SR0_SRE_WFC_MSK                               (0x10000000)
  #define DUNIT0_CR_D_CR_SR0_SRE_WFC_MIN                               (0)
  #define DUNIT0_CR_D_CR_SR0_SRE_WFC_MAX                               (1) // 0x00000001
  #define DUNIT0_CR_D_CR_SR0_SRE_WFC_DEF                               (0x00000000)
  #define DUNIT0_CR_D_CR_SR0_SRE_WFC_HSH                               (0x0138B000)

  #define DUNIT0_CR_D_CR_SR0_MSRE_OFF                                  (29)
  #define DUNIT0_CR_D_CR_SR0_MSRE_WID                                  ( 1)
  #define DUNIT0_CR_D_CR_SR0_MSRE_MSK                                  (0x20000000)
  #define DUNIT0_CR_D_CR_SR0_MSRE_MIN                                  (0)
  #define DUNIT0_CR_D_CR_SR0_MSRE_MAX                                  (1) // 0x00000001
  #define DUNIT0_CR_D_CR_SR0_MSRE_DEF                                  (0x00000000)
  #define DUNIT0_CR_D_CR_SR0_MSRE_HSH                                  (0x01BAB000)

  #define DUNIT0_CR_D_CR_SR0_MSRX_OFF                                  (30)
  #define DUNIT0_CR_D_CR_SR0_MSRX_WID                                  ( 1)
  #define DUNIT0_CR_D_CR_SR0_MSRX_MSK                                  (0x40000000)
  #define DUNIT0_CR_D_CR_SR0_MSRX_MIN                                  (0)
  #define DUNIT0_CR_D_CR_SR0_MSRX_MAX                                  (1) // 0x00000001
  #define DUNIT0_CR_D_CR_SR0_MSRX_DEF                                  (0x00000000)
  #define DUNIT0_CR_D_CR_SR0_MSRX_HSH                                  (0x01BCB000)

  #define DUNIT0_CR_D_CR_SR0_SRE_WO_PD_OFF                             (31)
  #define DUNIT0_CR_D_CR_SR0_SRE_WO_PD_WID                             ( 1)
  #define DUNIT0_CR_D_CR_SR0_SRE_WO_PD_MSK                             (0x80000000)
  #define DUNIT0_CR_D_CR_SR0_SRE_WO_PD_MIN                             (0)
  #define DUNIT0_CR_D_CR_SR0_SRE_WO_PD_MAX                             (1) // 0x00000001
  #define DUNIT0_CR_D_CR_SR0_SRE_WO_PD_DEF                             (0x00000000)
  #define DUNIT0_CR_D_CR_SR0_SRE_WO_PD_HSH                             (0x013EB000)

#define DUNIT0_CR_D_CR_LPCTRL0_REG                                     (0x0000B004)
#define DUNIT0_CR_D_CR_LPCTRL0_DEF                                     (0x04000000)
#define DUNIT0_CR_D_CR_LPCTRL0_VOLATILE_BITFIELDS_MSK                  (0x00000000)

  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE0P5_WAKEUP_TIME_OFF             ( 0)
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE0P5_WAKEUP_TIME_WID             ( 4)
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE0P5_WAKEUP_TIME_MSK             (0x0000000F)
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE0P5_WAKEUP_TIME_MIN             (0)
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE0P5_WAKEUP_TIME_MAX             (15) // 0x0000000F
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE0P5_WAKEUP_TIME_DEF             (0x00000000)
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE0P5_WAKEUP_TIME_HSH             (0x0400B004)

  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE2_WAKEUP_TIME_OFF               ( 4)
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE2_WAKEUP_TIME_WID               ( 4)
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE2_WAKEUP_TIME_MSK               (0x000000F0)
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE2_WAKEUP_TIME_MIN               (0)
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE2_WAKEUP_TIME_MAX               (15) // 0x0000000F
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE2_WAKEUP_TIME_DEF               (0x00000000)
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE2_WAKEUP_TIME_HSH               (0x0408B004)

  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE3_WAKEUP_TIME_OFF               ( 8)
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE3_WAKEUP_TIME_WID               ( 4)
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE3_WAKEUP_TIME_MSK               (0x00000F00)
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE3_WAKEUP_TIME_MIN               (0)
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE3_WAKEUP_TIME_MAX               (15) // 0x0000000F
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE3_WAKEUP_TIME_DEF               (0x00000000)
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE3_WAKEUP_TIME_HSH               (0x0410B004)

  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE4_WAKEUP_TIME_OFF               (12)
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE4_WAKEUP_TIME_WID               ( 4)
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE4_WAKEUP_TIME_MSK               (0x0000F000)
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE4_WAKEUP_TIME_MIN               (0)
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE4_WAKEUP_TIME_MAX               (15) // 0x0000000F
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE4_WAKEUP_TIME_DEF               (0x00000000)
  #define DUNIT0_CR_D_CR_LPCTRL0_LPMODE4_WAKEUP_TIME_HSH               (0x0418B004)

  #define DUNIT0_CR_D_CR_LPCTRL0_TOGGLE_SR_WAKEUP_TIME_OFF             (16)
  #define DUNIT0_CR_D_CR_LPCTRL0_TOGGLE_SR_WAKEUP_TIME_WID             ( 1)
  #define DUNIT0_CR_D_CR_LPCTRL0_TOGGLE_SR_WAKEUP_TIME_MSK             (0x00010000)
  #define DUNIT0_CR_D_CR_LPCTRL0_TOGGLE_SR_WAKEUP_TIME_MIN             (0)
  #define DUNIT0_CR_D_CR_LPCTRL0_TOGGLE_SR_WAKEUP_TIME_MAX             (1) // 0x00000001
  #define DUNIT0_CR_D_CR_LPCTRL0_TOGGLE_SR_WAKEUP_TIME_DEF             (0x00000000)
  #define DUNIT0_CR_D_CR_LPCTRL0_TOGGLE_SR_WAKEUP_TIME_HSH             (0x0120B004)

  #define DUNIT0_CR_D_CR_LPCTRL0_RSVD23_17_OFF                         (17)
  #define DUNIT0_CR_D_CR_LPCTRL0_RSVD23_17_WID                         ( 7)
  #define DUNIT0_CR_D_CR_LPCTRL0_RSVD23_17_MSK                         (0x00FE0000)
  #define DUNIT0_CR_D_CR_LPCTRL0_RSVD23_17_MIN                         (0)
  #define DUNIT0_CR_D_CR_LPCTRL0_RSVD23_17_MAX                         (127) // 0x0000007F
  #define DUNIT0_CR_D_CR_LPCTRL0_RSVD23_17_DEF                         (0x00000000)
  #define DUNIT0_CR_D_CR_LPCTRL0_RSVD23_17_HSH                         (0x0722B004)

  #define DUNIT0_CR_D_CR_LPCTRL0_LP_REQ_TMO_DLY_OFF                    (24)
  #define DUNIT0_CR_D_CR_LPCTRL0_LP_REQ_TMO_DLY_WID                    ( 8)
  #define DUNIT0_CR_D_CR_LPCTRL0_LP_REQ_TMO_DLY_MSK                    (0xFF000000)
  #define DUNIT0_CR_D_CR_LPCTRL0_LP_REQ_TMO_DLY_MIN                    (0)
  #define DUNIT0_CR_D_CR_LPCTRL0_LP_REQ_TMO_DLY_MAX                    (255) // 0x000000FF
  #define DUNIT0_CR_D_CR_LPCTRL0_LP_REQ_TMO_DLY_DEF                    (0x00000004)
  #define DUNIT0_CR_D_CR_LPCTRL0_LP_REQ_TMO_DLY_HSH                    (0x0830B004)

#define DUNIT0_CR_D_CR_LPCTRL1_REG                                     (0x0000B008)
#define DUNIT0_CR_D_CR_LPCTRL1_DEF                                     (0x00000000)
#define DUNIT0_CR_D_CR_LPCTRL1_VOLATILE_BITFIELDS_MSK                  (0x00000000)

  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE0P5_MIN_RES_OFF                 ( 0)
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE0P5_MIN_RES_WID                 ( 8)
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE0P5_MIN_RES_MSK                 (0x000000FF)
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE0P5_MIN_RES_MIN                 (0)
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE0P5_MIN_RES_MAX                 (255) // 0x000000FF
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE0P5_MIN_RES_DEF                 (0x00000000)
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE0P5_MIN_RES_HSH                 (0x0800B008)

  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE2_MIN_RES_OFF                   ( 8)
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE2_MIN_RES_WID                   ( 8)
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE2_MIN_RES_MSK                   (0x0000FF00)
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE2_MIN_RES_MIN                   (0)
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE2_MIN_RES_MAX                   (255) // 0x000000FF
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE2_MIN_RES_DEF                   (0x00000000)
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE2_MIN_RES_HSH                   (0x0810B008)

  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE3_MIN_RES_OFF                   (16)
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE3_MIN_RES_WID                   ( 8)
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE3_MIN_RES_MSK                   (0x00FF0000)
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE3_MIN_RES_MIN                   (0)
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE3_MIN_RES_MAX                   (255) // 0x000000FF
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE3_MIN_RES_DEF                   (0x00000000)
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE3_MIN_RES_HSH                   (0x0820B008)

  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE4_MIN_RES_OFF                   (24)
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE4_MIN_RES_WID                   ( 8)
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE4_MIN_RES_MSK                   (0xFF000000)
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE4_MIN_RES_MIN                   (0)
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE4_MIN_RES_MAX                   (255) // 0x000000FF
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE4_MIN_RES_DEF                   (0x00000000)
  #define DUNIT0_CR_D_CR_LPCTRL1_LPMODE4_MIN_RES_HSH                   (0x0830B008)

#define DUNIT0_CR_D_CR_REF_REG                                         (0x0000B00C)
#define DUNIT0_CR_D_CR_REF_DEF                                         (0x00000034)
#define DUNIT0_CR_D_CR_REF_VOLATILE_BITFIELDS_MSK                      (0xFC000000)

  #define DUNIT0_CR_D_CR_REF_REF_WM_OFF                                ( 0)
  #define DUNIT0_CR_D_CR_REF_REF_WM_WID                                ( 3)
  #define DUNIT0_CR_D_CR_REF_REF_WM_MSK                                (0x00000007)
  #define DUNIT0_CR_D_CR_REF_REF_WM_MIN                                (0)
  #define DUNIT0_CR_D_CR_REF_REF_WM_MAX                                (7) // 0x00000007
  #define DUNIT0_CR_D_CR_REF_REF_WM_DEF                                (0x00000004)
  #define DUNIT0_CR_D_CR_REF_REF_WM_HSH                                (0x0300B00C)

  #define DUNIT0_CR_D_CR_REF_RSVD3_OFF                                 ( 3)
  #define DUNIT0_CR_D_CR_REF_RSVD3_WID                                 ( 1)
  #define DUNIT0_CR_D_CR_REF_RSVD3_MSK                                 (0x00000008)
  #define DUNIT0_CR_D_CR_REF_RSVD3_MIN                                 (0)
  #define DUNIT0_CR_D_CR_REF_RSVD3_MAX                                 (1) // 0x00000001
  #define DUNIT0_CR_D_CR_REF_RSVD3_DEF                                 (0x00000000)
  #define DUNIT0_CR_D_CR_REF_RSVD3_HSH                                 (0x0106B00C)

  #define DUNIT0_CR_D_CR_REF_REF_RATE_OFF                              ( 4)
  #define DUNIT0_CR_D_CR_REF_REF_RATE_WID                              ( 3)
  #define DUNIT0_CR_D_CR_REF_REF_RATE_MSK                              (0x00000070)
  #define DUNIT0_CR_D_CR_REF_REF_RATE_MIN                              (0)
  #define DUNIT0_CR_D_CR_REF_REF_RATE_MAX                              (7) // 0x00000007
  #define DUNIT0_CR_D_CR_REF_REF_RATE_DEF                              (0x00000003)
  #define DUNIT0_CR_D_CR_REF_REF_RATE_HSH                              (0x0308B00C)

  #define DUNIT0_CR_D_CR_REF_RSVD25_7_OFF                              ( 7)
  #define DUNIT0_CR_D_CR_REF_RSVD25_7_WID                              (19)
  #define DUNIT0_CR_D_CR_REF_RSVD25_7_MSK                              (0x03FFFF80)
  #define DUNIT0_CR_D_CR_REF_RSVD25_7_MIN                              (0)
  #define DUNIT0_CR_D_CR_REF_RSVD25_7_MAX                              (524287) // 0x0007FFFF
  #define DUNIT0_CR_D_CR_REF_RSVD25_7_DEF                              (0x00000000)
  #define DUNIT0_CR_D_CR_REF_RSVD25_7_HSH                              (0x130EB00C)

  #define DUNIT0_CR_D_CR_REF_MREF_OFF                                  (26)
  #define DUNIT0_CR_D_CR_REF_MREF_WID                                  ( 1)
  #define DUNIT0_CR_D_CR_REF_MREF_MSK                                  (0x04000000)
  #define DUNIT0_CR_D_CR_REF_MREF_MIN                                  (0)
  #define DUNIT0_CR_D_CR_REF_MREF_MAX                                  (1) // 0x00000001
  #define DUNIT0_CR_D_CR_REF_MREF_DEF                                  (0x00000000)
  #define DUNIT0_CR_D_CR_REF_MREF_HSH                                  (0x01B4B00C)

  #define DUNIT0_CR_D_CR_REF_FLUSH_REF_OFF                             (27)
  #define DUNIT0_CR_D_CR_REF_FLUSH_REF_WID                             ( 1)
  #define DUNIT0_CR_D_CR_REF_FLUSH_REF_MSK                             (0x08000000)
  #define DUNIT0_CR_D_CR_REF_FLUSH_REF_MIN                             (0)
  #define DUNIT0_CR_D_CR_REF_FLUSH_REF_MAX                             (1) // 0x00000001
  #define DUNIT0_CR_D_CR_REF_FLUSH_REF_DEF                             (0x00000000)
  #define DUNIT0_CR_D_CR_REF_FLUSH_REF_HSH                             (0x01B6B00C)

  #define DUNIT0_CR_D_CR_REF_POSTPONED_REFS_OFF                        (28)
  #define DUNIT0_CR_D_CR_REF_POSTPONED_REFS_WID                        ( 4)
  #define DUNIT0_CR_D_CR_REF_POSTPONED_REFS_MSK                        (0xF0000000)
  #define DUNIT0_CR_D_CR_REF_POSTPONED_REFS_MIN                        (0)
  #define DUNIT0_CR_D_CR_REF_POSTPONED_REFS_MAX                        (15) // 0x0000000F
  #define DUNIT0_CR_D_CR_REF_POSTPONED_REFS_DEF                        (0x00000000)
  #define DUNIT0_CR_D_CR_REF_POSTPONED_REFS_HSH                        (0x04B8B00C)

#define DUNIT0_CR_D_CR_ZQCAL_REG                                       (0x0000B010)
#define DUNIT0_CR_D_CR_ZQCAL_DEF                                       (0x000007D0)
#define DUNIT0_CR_D_CR_ZQCAL_VOLATILE_BITFIELDS_MSK                    (0x00080000)

  #define DUNIT0_CR_D_CR_ZQCAL_ZQCAL_INTV_OFF                          ( 0)
  #define DUNIT0_CR_D_CR_ZQCAL_ZQCAL_INTV_WID                          (14)
  #define DUNIT0_CR_D_CR_ZQCAL_ZQCAL_INTV_MSK                          (0x00003FFF)
  #define DUNIT0_CR_D_CR_ZQCAL_ZQCAL_INTV_MIN                          (0)
  #define DUNIT0_CR_D_CR_ZQCAL_ZQCAL_INTV_MAX                          (16383) // 0x00003FFF
  #define DUNIT0_CR_D_CR_ZQCAL_ZQCAL_INTV_DEF                          (0x000007D0)
  #define DUNIT0_CR_D_CR_ZQCAL_ZQCAL_INTV_HSH                          (0x0E00B010)

  #define DUNIT0_CR_D_CR_ZQCAL_RSVD15_14_OFF                           (14)
  #define DUNIT0_CR_D_CR_ZQCAL_RSVD15_14_WID                           ( 2)
  #define DUNIT0_CR_D_CR_ZQCAL_RSVD15_14_MSK                           (0x0000C000)
  #define DUNIT0_CR_D_CR_ZQCAL_RSVD15_14_MIN                           (0)
  #define DUNIT0_CR_D_CR_ZQCAL_RSVD15_14_MAX                           (3) // 0x00000003
  #define DUNIT0_CR_D_CR_ZQCAL_RSVD15_14_DEF                           (0x00000000)
  #define DUNIT0_CR_D_CR_ZQCAL_RSVD15_14_HSH                           (0x021CB010)

  #define DUNIT0_CR_D_CR_ZQCAL_ZQCAL_EN_OFF                            (16)
  #define DUNIT0_CR_D_CR_ZQCAL_ZQCAL_EN_WID                            ( 1)
  #define DUNIT0_CR_D_CR_ZQCAL_ZQCAL_EN_MSK                            (0x00010000)
  #define DUNIT0_CR_D_CR_ZQCAL_ZQCAL_EN_MIN                            (0)
  #define DUNIT0_CR_D_CR_ZQCAL_ZQCAL_EN_MAX                            (1) // 0x00000001
  #define DUNIT0_CR_D_CR_ZQCAL_ZQCAL_EN_DEF                            (0x00000000)
  #define DUNIT0_CR_D_CR_ZQCAL_ZQCAL_EN_HSH                            (0x0120B010)

  #define DUNIT0_CR_D_CR_ZQCAL_ZQCAL_MODE_OFF                          (17)
  #define DUNIT0_CR_D_CR_ZQCAL_ZQCAL_MODE_WID                          ( 1)
  #define DUNIT0_CR_D_CR_ZQCAL_ZQCAL_MODE_MSK                          (0x00020000)
  #define DUNIT0_CR_D_CR_ZQCAL_ZQCAL_MODE_MIN                          (0)
  #define DUNIT0_CR_D_CR_ZQCAL_ZQCAL_MODE_MAX                          (1) // 0x00000001
  #define DUNIT0_CR_D_CR_ZQCAL_ZQCAL_MODE_DEF                          (0x00000000)
  #define DUNIT0_CR_D_CR_ZQCAL_ZQCAL_MODE_HSH                          (0x0122B010)

  #define DUNIT0_CR_D_CR_ZQCAL_RSVD18_OFF                              (18)
  #define DUNIT0_CR_D_CR_ZQCAL_RSVD18_WID                              ( 1)
  #define DUNIT0_CR_D_CR_ZQCAL_RSVD18_MSK                              (0x00040000)
  #define DUNIT0_CR_D_CR_ZQCAL_RSVD18_MIN                              (0)
  #define DUNIT0_CR_D_CR_ZQCAL_RSVD18_MAX                              (1) // 0x00000001
  #define DUNIT0_CR_D_CR_ZQCAL_RSVD18_DEF                              (0x00000000)
  #define DUNIT0_CR_D_CR_ZQCAL_RSVD18_HSH                              (0x0124B010)

  #define DUNIT0_CR_D_CR_ZQCAL_MZQCAL_START_OFF                        (19)
  #define DUNIT0_CR_D_CR_ZQCAL_MZQCAL_START_WID                        ( 1)
  #define DUNIT0_CR_D_CR_ZQCAL_MZQCAL_START_MSK                        (0x00080000)
  #define DUNIT0_CR_D_CR_ZQCAL_MZQCAL_START_MIN                        (0)
  #define DUNIT0_CR_D_CR_ZQCAL_MZQCAL_START_MAX                        (1) // 0x00000001
  #define DUNIT0_CR_D_CR_ZQCAL_MZQCAL_START_DEF                        (0x00000000)
  #define DUNIT0_CR_D_CR_ZQCAL_MZQCAL_START_HSH                        (0x01A6B010)

  #define DUNIT0_CR_D_CR_ZQCAL_RSVD31_20_OFF                           (20)
  #define DUNIT0_CR_D_CR_ZQCAL_RSVD31_20_WID                           (12)
  #define DUNIT0_CR_D_CR_ZQCAL_RSVD31_20_MSK                           (0xFFF00000)
  #define DUNIT0_CR_D_CR_ZQCAL_RSVD31_20_MIN                           (0)
  #define DUNIT0_CR_D_CR_ZQCAL_RSVD31_20_MAX                           (4095) // 0x00000FFF
  #define DUNIT0_CR_D_CR_ZQCAL_RSVD31_20_DEF                           (0x00000000)
  #define DUNIT0_CR_D_CR_ZQCAL_RSVD31_20_HSH                           (0x0C28B010)

#define DUNIT0_CR_D_CR_DQS2DQ_CAL_REG                                  (0x0000B014)
#define DUNIT0_CR_D_CR_DQS2DQ_CAL_DEF                                  (0x00280014)
#define DUNIT0_CR_D_CR_DQS2DQ_CAL_VOLATILE_BITFIELDS_MSK               (0x10000000)

  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_DQS2DQ_CAL_INTV_OFF                ( 0)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_DQS2DQ_CAL_INTV_WID                (14)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_DQS2DQ_CAL_INTV_MSK                (0x00003FFF)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_DQS2DQ_CAL_INTV_MIN                (0)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_DQS2DQ_CAL_INTV_MAX                (16383) // 0x00003FFF
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_DQS2DQ_CAL_INTV_DEF                (0x00000014)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_DQS2DQ_CAL_INTV_HSH                (0x0E00B014)

  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_RSVD16_14_OFF                      (14)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_RSVD16_14_WID                      ( 3)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_RSVD16_14_MSK                      (0x0001C000)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_RSVD16_14_MIN                      (0)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_RSVD16_14_MAX                      (7) // 0x00000007
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_RSVD16_14_DEF                      (0x00000000)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_RSVD16_14_HSH                      (0x031CB014)

  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_DQS2DQ_CAL_OSC_RT_OFF              (17)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_DQS2DQ_CAL_OSC_RT_WID              (10)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_DQS2DQ_CAL_OSC_RT_MSK              (0x07FE0000)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_DQS2DQ_CAL_OSC_RT_MIN              (0)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_DQS2DQ_CAL_OSC_RT_MAX              (1023) // 0x000003FF
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_DQS2DQ_CAL_OSC_RT_DEF              (0x00000014)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_DQS2DQ_CAL_OSC_RT_HSH              (0x0A22B014)

  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_DQS2DQ_CAL_EN_OFF                  (27)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_DQS2DQ_CAL_EN_WID                  ( 1)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_DQS2DQ_CAL_EN_MSK                  (0x08000000)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_DQS2DQ_CAL_EN_MIN                  (0)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_DQS2DQ_CAL_EN_MAX                  (1) // 0x00000001
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_DQS2DQ_CAL_EN_DEF                  (0x00000000)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_DQS2DQ_CAL_EN_HSH                  (0x0136B014)

  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_MDQS2DQ_CAL_START_OFF              (28)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_MDQS2DQ_CAL_START_WID              ( 1)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_MDQS2DQ_CAL_START_MSK              (0x10000000)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_MDQS2DQ_CAL_START_MIN              (0)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_MDQS2DQ_CAL_START_MAX              (1) // 0x00000001
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_MDQS2DQ_CAL_START_DEF              (0x00000000)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_MDQS2DQ_CAL_START_HSH              (0x01B8B014)

  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_RSVD31_29_OFF                      (29)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_RSVD31_29_WID                      ( 3)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_RSVD31_29_MSK                      (0xE0000000)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_RSVD31_29_MIN                      (0)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_RSVD31_29_MAX                      (7) // 0x00000007
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_RSVD31_29_DEF                      (0x00000000)
  #define DUNIT0_CR_D_CR_DQS2DQ_CAL_RSVD31_29_HSH                      (0x033AB014)

#define DUNIT0_CR_D_CR_WCK2DQO_CAL_REG                                 (0x0000B018)
#define DUNIT0_CR_D_CR_WCK2DQO_CAL_DEF                                 (0x00280014)
#define DUNIT0_CR_D_CR_WCK2DQO_CAL_VOLATILE_BITFIELDS_MSK              (0x10000000)

  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_WCK2DQO_CAL_INTV_OFF              ( 0)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_WCK2DQO_CAL_INTV_WID              (14)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_WCK2DQO_CAL_INTV_MSK              (0x00003FFF)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_WCK2DQO_CAL_INTV_MIN              (0)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_WCK2DQO_CAL_INTV_MAX              (16383) // 0x00003FFF
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_WCK2DQO_CAL_INTV_DEF              (0x00000014)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_WCK2DQO_CAL_INTV_HSH              (0x0E00B018)

  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_RSVD16_14_OFF                     (14)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_RSVD16_14_WID                     ( 3)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_RSVD16_14_MSK                     (0x0001C000)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_RSVD16_14_MIN                     (0)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_RSVD16_14_MAX                     (7) // 0x00000007
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_RSVD16_14_DEF                     (0x00000000)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_RSVD16_14_HSH                     (0x031CB018)

  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_WCK2DQO_CAL_OSC_RT_OFF            (17)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_WCK2DQO_CAL_OSC_RT_WID            (10)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_WCK2DQO_CAL_OSC_RT_MSK            (0x07FE0000)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_WCK2DQO_CAL_OSC_RT_MIN            (0)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_WCK2DQO_CAL_OSC_RT_MAX            (1023) // 0x000003FF
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_WCK2DQO_CAL_OSC_RT_DEF            (0x00000014)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_WCK2DQO_CAL_OSC_RT_HSH            (0x0A22B018)

  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_WCK2DQO_CAL_EN_OFF                (27)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_WCK2DQO_CAL_EN_WID                ( 1)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_WCK2DQO_CAL_EN_MSK                (0x08000000)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_WCK2DQO_CAL_EN_MIN                (0)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_WCK2DQO_CAL_EN_MAX                (1) // 0x00000001
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_WCK2DQO_CAL_EN_DEF                (0x00000000)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_WCK2DQO_CAL_EN_HSH                (0x0136B018)

  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_MWCK2DQO_CAL_START_OFF            (28)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_MWCK2DQO_CAL_START_WID            ( 1)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_MWCK2DQO_CAL_START_MSK            (0x10000000)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_MWCK2DQO_CAL_START_MIN            (0)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_MWCK2DQO_CAL_START_MAX            (1) // 0x00000001
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_MWCK2DQO_CAL_START_DEF            (0x00000000)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_MWCK2DQO_CAL_START_HSH            (0x01B8B018)

  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_RSVD31_29_OFF                     (29)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_RSVD31_29_WID                     ( 3)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_RSVD31_29_MSK                     (0xE0000000)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_RSVD31_29_MIN                     (0)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_RSVD31_29_MAX                     (7) // 0x00000007
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_RSVD31_29_DEF                     (0x00000000)
  #define DUNIT0_CR_D_CR_WCK2DQO_CAL_RSVD31_29_HSH                     (0x033AB018)

#define DUNIT0_CR_D_CR_TRAIN_REG                                       (0x0000B020)
#define DUNIT0_CR_D_CR_TRAIN_DEF                                       (0x00000000)
#define DUNIT0_CR_D_CR_TRAIN_VOLATILE_BITFIELDS_MSK                    (0x00000000)

  #define DUNIT0_CR_D_CR_TRAIN_CBT_OFF                                 ( 0)
  #define DUNIT0_CR_D_CR_TRAIN_CBT_WID                                 ( 1)
  #define DUNIT0_CR_D_CR_TRAIN_CBT_MSK                                 (0x00000001)
  #define DUNIT0_CR_D_CR_TRAIN_CBT_MIN                                 (0)
  #define DUNIT0_CR_D_CR_TRAIN_CBT_MAX                                 (1) // 0x00000001
  #define DUNIT0_CR_D_CR_TRAIN_CBT_DEF                                 (0x00000000)
  #define DUNIT0_CR_D_CR_TRAIN_CBT_HSH                                 (0x0100B020)

  #define DUNIT0_CR_D_CR_TRAIN_RCVEN_CAL_OFF                           ( 1)
  #define DUNIT0_CR_D_CR_TRAIN_RCVEN_CAL_WID                           ( 1)
  #define DUNIT0_CR_D_CR_TRAIN_RCVEN_CAL_MSK                           (0x00000002)
  #define DUNIT0_CR_D_CR_TRAIN_RCVEN_CAL_MIN                           (0)
  #define DUNIT0_CR_D_CR_TRAIN_RCVEN_CAL_MAX                           (1) // 0x00000001
  #define DUNIT0_CR_D_CR_TRAIN_RCVEN_CAL_DEF                           (0x00000000)
  #define DUNIT0_CR_D_CR_TRAIN_RCVEN_CAL_HSH                           (0x0102B020)

  #define DUNIT0_CR_D_CR_TRAIN_RD_DQCAL_OFF                            ( 2)
  #define DUNIT0_CR_D_CR_TRAIN_RD_DQCAL_WID                            ( 1)
  #define DUNIT0_CR_D_CR_TRAIN_RD_DQCAL_MSK                            (0x00000004)
  #define DUNIT0_CR_D_CR_TRAIN_RD_DQCAL_MIN                            (0)
  #define DUNIT0_CR_D_CR_TRAIN_RD_DQCAL_MAX                            (1) // 0x00000001
  #define DUNIT0_CR_D_CR_TRAIN_RD_DQCAL_DEF                            (0x00000000)
  #define DUNIT0_CR_D_CR_TRAIN_RD_DQCAL_HSH                            (0x0104B020)

  #define DUNIT0_CR_D_CR_TRAIN_WR_LEVELING_OFF                         ( 3)
  #define DUNIT0_CR_D_CR_TRAIN_WR_LEVELING_WID                         ( 1)
  #define DUNIT0_CR_D_CR_TRAIN_WR_LEVELING_MSK                         (0x00000008)
  #define DUNIT0_CR_D_CR_TRAIN_WR_LEVELING_MIN                         (0)
  #define DUNIT0_CR_D_CR_TRAIN_WR_LEVELING_MAX                         (1) // 0x00000001
  #define DUNIT0_CR_D_CR_TRAIN_WR_LEVELING_DEF                         (0x00000000)
  #define DUNIT0_CR_D_CR_TRAIN_WR_LEVELING_HSH                         (0x0106B020)

  #define DUNIT0_CR_D_CR_TRAIN_PDA_OFF                                 ( 4)
  #define DUNIT0_CR_D_CR_TRAIN_PDA_WID                                 ( 1)
  #define DUNIT0_CR_D_CR_TRAIN_PDA_MSK                                 (0x00000010)
  #define DUNIT0_CR_D_CR_TRAIN_PDA_MIN                                 (0)
  #define DUNIT0_CR_D_CR_TRAIN_PDA_MAX                                 (1) // 0x00000001
  #define DUNIT0_CR_D_CR_TRAIN_PDA_DEF                                 (0x00000000)
  #define DUNIT0_CR_D_CR_TRAIN_PDA_HSH                                 (0x0108B020)

  #define DUNIT0_CR_D_CR_TRAIN_DQS2DQ_FIFO_CAL_OFF                     ( 5)
  #define DUNIT0_CR_D_CR_TRAIN_DQS2DQ_FIFO_CAL_WID                     ( 1)
  #define DUNIT0_CR_D_CR_TRAIN_DQS2DQ_FIFO_CAL_MSK                     (0x00000020)
  #define DUNIT0_CR_D_CR_TRAIN_DQS2DQ_FIFO_CAL_MIN                     (0)
  #define DUNIT0_CR_D_CR_TRAIN_DQS2DQ_FIFO_CAL_MAX                     (1) // 0x00000001
  #define DUNIT0_CR_D_CR_TRAIN_DQS2DQ_FIFO_CAL_DEF                     (0x00000000)
  #define DUNIT0_CR_D_CR_TRAIN_DQS2DQ_FIFO_CAL_HSH                     (0x010AB020)

  #define DUNIT0_CR_D_CR_TRAIN_CBT_PH_OFF                              ( 6)
  #define DUNIT0_CR_D_CR_TRAIN_CBT_PH_WID                              ( 1)
  #define DUNIT0_CR_D_CR_TRAIN_CBT_PH_MSK                              (0x00000040)
  #define DUNIT0_CR_D_CR_TRAIN_CBT_PH_MIN                              (0)
  #define DUNIT0_CR_D_CR_TRAIN_CBT_PH_MAX                              (1) // 0x00000001
  #define DUNIT0_CR_D_CR_TRAIN_CBT_PH_DEF                              (0x00000000)
  #define DUNIT0_CR_D_CR_TRAIN_CBT_PH_HSH                              (0x010CB020)

  #define DUNIT0_CR_D_CR_TRAIN_DQ_LOOPBACK_OFF                         ( 7)
  #define DUNIT0_CR_D_CR_TRAIN_DQ_LOOPBACK_WID                         ( 1)
  #define DUNIT0_CR_D_CR_TRAIN_DQ_LOOPBACK_MSK                         (0x00000080)
  #define DUNIT0_CR_D_CR_TRAIN_DQ_LOOPBACK_MIN                         (0)
  #define DUNIT0_CR_D_CR_TRAIN_DQ_LOOPBACK_MAX                         (1) // 0x00000001
  #define DUNIT0_CR_D_CR_TRAIN_DQ_LOOPBACK_DEF                         (0x00000000)
  #define DUNIT0_CR_D_CR_TRAIN_DQ_LOOPBACK_HSH                         (0x010EB020)

  #define DUNIT0_CR_D_CR_TRAIN_RSVD23_8_OFF                            ( 8)
  #define DUNIT0_CR_D_CR_TRAIN_RSVD23_8_WID                            (16)
  #define DUNIT0_CR_D_CR_TRAIN_RSVD23_8_MSK                            (0x00FFFF00)
  #define DUNIT0_CR_D_CR_TRAIN_RSVD23_8_MIN                            (0)
  #define DUNIT0_CR_D_CR_TRAIN_RSVD23_8_MAX                            (65535) // 0x0000FFFF
  #define DUNIT0_CR_D_CR_TRAIN_RSVD23_8_DEF                            (0x00000000)
  #define DUNIT0_CR_D_CR_TRAIN_RSVD23_8_HSH                            (0x1010B020)

  #define DUNIT0_CR_D_CR_TRAIN_BLOCK_ACT_OFF                           (24)
  #define DUNIT0_CR_D_CR_TRAIN_BLOCK_ACT_WID                           ( 1)
  #define DUNIT0_CR_D_CR_TRAIN_BLOCK_ACT_MSK                           (0x01000000)
  #define DUNIT0_CR_D_CR_TRAIN_BLOCK_ACT_MIN                           (0)
  #define DUNIT0_CR_D_CR_TRAIN_BLOCK_ACT_MAX                           (1) // 0x00000001
  #define DUNIT0_CR_D_CR_TRAIN_BLOCK_ACT_DEF                           (0x00000000)
  #define DUNIT0_CR_D_CR_TRAIN_BLOCK_ACT_HSH                           (0x0130B020)

  #define DUNIT0_CR_D_CR_TRAIN_BLOCK_WRDATA_OFF                        (25)
  #define DUNIT0_CR_D_CR_TRAIN_BLOCK_WRDATA_WID                        ( 1)
  #define DUNIT0_CR_D_CR_TRAIN_BLOCK_WRDATA_MSK                        (0x02000000)
  #define DUNIT0_CR_D_CR_TRAIN_BLOCK_WRDATA_MIN                        (0)
  #define DUNIT0_CR_D_CR_TRAIN_BLOCK_WRDATA_MAX                        (1) // 0x00000001
  #define DUNIT0_CR_D_CR_TRAIN_BLOCK_WRDATA_DEF                        (0x00000000)
  #define DUNIT0_CR_D_CR_TRAIN_BLOCK_WRDATA_HSH                        (0x0132B020)

  #define DUNIT0_CR_D_CR_TRAIN_BLOCK_RDDATA_OFF                        (26)
  #define DUNIT0_CR_D_CR_TRAIN_BLOCK_RDDATA_WID                        ( 4)
  #define DUNIT0_CR_D_CR_TRAIN_BLOCK_RDDATA_MSK                        (0x3C000000)
  #define DUNIT0_CR_D_CR_TRAIN_BLOCK_RDDATA_MIN                        (0)
  #define DUNIT0_CR_D_CR_TRAIN_BLOCK_RDDATA_MAX                        (15) // 0x0000000F
  #define DUNIT0_CR_D_CR_TRAIN_BLOCK_RDDATA_DEF                        (0x00000000)
  #define DUNIT0_CR_D_CR_TRAIN_BLOCK_RDDATA_HSH                        (0x0434B020)

  #define DUNIT0_CR_D_CR_TRAIN_RSVD30_OFF                              (30)
  #define DUNIT0_CR_D_CR_TRAIN_RSVD30_WID                              ( 1)
  #define DUNIT0_CR_D_CR_TRAIN_RSVD30_MSK                              (0x40000000)
  #define DUNIT0_CR_D_CR_TRAIN_RSVD30_MIN                              (0)
  #define DUNIT0_CR_D_CR_TRAIN_RSVD30_MAX                              (1) // 0x00000001
  #define DUNIT0_CR_D_CR_TRAIN_RSVD30_DEF                              (0x00000000)
  #define DUNIT0_CR_D_CR_TRAIN_RSVD30_HSH                              (0x013CB020)

  #define DUNIT0_CR_D_CR_TRAIN_CAPTURE_FB_DATA_OFF                     (31)
  #define DUNIT0_CR_D_CR_TRAIN_CAPTURE_FB_DATA_WID                     ( 1)
  #define DUNIT0_CR_D_CR_TRAIN_CAPTURE_FB_DATA_MSK                     (0x80000000)
  #define DUNIT0_CR_D_CR_TRAIN_CAPTURE_FB_DATA_MIN                     (0)
  #define DUNIT0_CR_D_CR_TRAIN_CAPTURE_FB_DATA_MAX                     (1) // 0x00000001
  #define DUNIT0_CR_D_CR_TRAIN_CAPTURE_FB_DATA_DEF                     (0x00000000)
  #define DUNIT0_CR_D_CR_TRAIN_CAPTURE_FB_DATA_HSH                     (0x013EB020)

#define DUNIT0_CR_D_CR_TRAIN_FB_DATA0_REG                              (0x0000B024)
#define DUNIT0_CR_D_CR_TRAIN_FB_DATA0_DEF                              (0x00000000)
#define DUNIT0_CR_D_CR_TRAIN_FB_DATA0_VOLATILE_BITFIELDS_MSK           (0xFFFFFFFF)

  #define DUNIT0_CR_D_CR_TRAIN_FB_DATA0_TRAIN_FB_DATA0_OFF             ( 0)
  #define DUNIT0_CR_D_CR_TRAIN_FB_DATA0_TRAIN_FB_DATA0_WID             (32)
  #define DUNIT0_CR_D_CR_TRAIN_FB_DATA0_TRAIN_FB_DATA0_MSK             (0xFFFFFFFF)
  #define DUNIT0_CR_D_CR_TRAIN_FB_DATA0_TRAIN_FB_DATA0_MIN             (0)
  #define DUNIT0_CR_D_CR_TRAIN_FB_DATA0_TRAIN_FB_DATA0_MAX             (4294967295) // 0xFFFFFFFF
  #define DUNIT0_CR_D_CR_TRAIN_FB_DATA0_TRAIN_FB_DATA0_DEF             (0x00000000)
  #define DUNIT0_CR_D_CR_TRAIN_FB_DATA0_TRAIN_FB_DATA0_HSH             (0x2080B024)

#define DUNIT0_CR_D_CR_TRAIN_FB_DATA1_REG                              (0x0000B028)
#define DUNIT0_CR_D_CR_TRAIN_FB_DATA1_DEF                              (0x00000000)
#define DUNIT0_CR_D_CR_TRAIN_FB_DATA1_VOLATILE_BITFIELDS_MSK           (0xFFFFFFFF)

  #define DUNIT0_CR_D_CR_TRAIN_FB_DATA1_TRAIN_FB_DATA1_OFF             ( 0)
  #define DUNIT0_CR_D_CR_TRAIN_FB_DATA1_TRAIN_FB_DATA1_WID             (32)
  #define DUNIT0_CR_D_CR_TRAIN_FB_DATA1_TRAIN_FB_DATA1_MSK             (0xFFFFFFFF)
  #define DUNIT0_CR_D_CR_TRAIN_FB_DATA1_TRAIN_FB_DATA1_MIN             (0)
  #define DUNIT0_CR_D_CR_TRAIN_FB_DATA1_TRAIN_FB_DATA1_MAX             (4294967295) // 0xFFFFFFFF
  #define DUNIT0_CR_D_CR_TRAIN_FB_DATA1_TRAIN_FB_DATA1_DEF             (0x00000000)
  #define DUNIT0_CR_D_CR_TRAIN_FB_DATA1_TRAIN_FB_DATA1_HSH             (0x2080B028)

#define DUNIT0_CR_D_CR_MCTRL_REG                                       (0x0000B02C)
#define DUNIT0_CR_D_CR_MCTRL_DEF                                       (0x00000000)
#define DUNIT0_CR_D_CR_MCTRL_VOLATILE_BITFIELDS_MSK                    (0x0F000000)

  #define DUNIT0_CR_D_CR_MCTRL_RSVD9_0_OFF                             ( 0)
  #define DUNIT0_CR_D_CR_MCTRL_RSVD9_0_WID                             (10)
  #define DUNIT0_CR_D_CR_MCTRL_RSVD9_0_MSK                             (0x000003FF)
  #define DUNIT0_CR_D_CR_MCTRL_RSVD9_0_MIN                             (0)
  #define DUNIT0_CR_D_CR_MCTRL_RSVD9_0_MAX                             (1023) // 0x000003FF
  #define DUNIT0_CR_D_CR_MCTRL_RSVD9_0_DEF                             (0x00000000)
  #define DUNIT0_CR_D_CR_MCTRL_RSVD9_0_HSH                             (0x0A00B02C)

  #define DUNIT0_CR_D_CR_MCTRL_CS_MODE_OFF                             (10)
  #define DUNIT0_CR_D_CR_MCTRL_CS_MODE_WID                             ( 2)
  #define DUNIT0_CR_D_CR_MCTRL_CS_MODE_MSK                             (0x00000C00)
  #define DUNIT0_CR_D_CR_MCTRL_CS_MODE_MIN                             (0)
  #define DUNIT0_CR_D_CR_MCTRL_CS_MODE_MAX                             (3) // 0x00000003
  #define DUNIT0_CR_D_CR_MCTRL_CS_MODE_DEF                             (0x00000000)
  #define DUNIT0_CR_D_CR_MCTRL_CS_MODE_HSH                             (0x0214B02C)

  #define DUNIT0_CR_D_CR_MCTRL_CS_VAL_OFF                              (12)
  #define DUNIT0_CR_D_CR_MCTRL_CS_VAL_WID                              ( 4)
  #define DUNIT0_CR_D_CR_MCTRL_CS_VAL_MSK                              (0x0000F000)
  #define DUNIT0_CR_D_CR_MCTRL_CS_VAL_MIN                              (0)
  #define DUNIT0_CR_D_CR_MCTRL_CS_VAL_MAX                              (15) // 0x0000000F
  #define DUNIT0_CR_D_CR_MCTRL_CS_VAL_DEF                              (0x00000000)
  #define DUNIT0_CR_D_CR_MCTRL_CS_VAL_HSH                              (0x0418B02C)

  #define DUNIT0_CR_D_CR_MCTRL_RSVD23_16_OFF                           (16)
  #define DUNIT0_CR_D_CR_MCTRL_RSVD23_16_WID                           ( 8)
  #define DUNIT0_CR_D_CR_MCTRL_RSVD23_16_MSK                           (0x00FF0000)
  #define DUNIT0_CR_D_CR_MCTRL_RSVD23_16_MIN                           (0)
  #define DUNIT0_CR_D_CR_MCTRL_RSVD23_16_MAX                           (255) // 0x000000FF
  #define DUNIT0_CR_D_CR_MCTRL_RSVD23_16_DEF                           (0x00000000)
  #define DUNIT0_CR_D_CR_MCTRL_RSVD23_16_HSH                           (0x0820B02C)

  #define DUNIT0_CR_D_CR_MCTRL_TURN_WCK_ON_OFF                         (24)
  #define DUNIT0_CR_D_CR_MCTRL_TURN_WCK_ON_WID                         ( 1)
  #define DUNIT0_CR_D_CR_MCTRL_TURN_WCK_ON_MSK                         (0x01000000)
  #define DUNIT0_CR_D_CR_MCTRL_TURN_WCK_ON_MIN                         (0)
  #define DUNIT0_CR_D_CR_MCTRL_TURN_WCK_ON_MAX                         (1) // 0x00000001
  #define DUNIT0_CR_D_CR_MCTRL_TURN_WCK_ON_DEF                         (0x00000000)
  #define DUNIT0_CR_D_CR_MCTRL_TURN_WCK_ON_HSH                         (0x01B0B02C)

  #define DUNIT0_CR_D_CR_MCTRL_TURN_WCK_OFF_OFF                        (25)
  #define DUNIT0_CR_D_CR_MCTRL_TURN_WCK_OFF_WID                        ( 1)
  #define DUNIT0_CR_D_CR_MCTRL_TURN_WCK_OFF_MSK                        (0x02000000)
  #define DUNIT0_CR_D_CR_MCTRL_TURN_WCK_OFF_MIN                        (0)
  #define DUNIT0_CR_D_CR_MCTRL_TURN_WCK_OFF_MAX                        (1) // 0x00000001
  #define DUNIT0_CR_D_CR_MCTRL_TURN_WCK_OFF_DEF                        (0x00000000)
  #define DUNIT0_CR_D_CR_MCTRL_TURN_WCK_OFF_HSH                        (0x01B2B02C)

  #define DUNIT0_CR_D_CR_MCTRL_ASSERT_DFI_WRDATA_EN_OFF                (26)
  #define DUNIT0_CR_D_CR_MCTRL_ASSERT_DFI_WRDATA_EN_WID                ( 1)
  #define DUNIT0_CR_D_CR_MCTRL_ASSERT_DFI_WRDATA_EN_MSK                (0x04000000)
  #define DUNIT0_CR_D_CR_MCTRL_ASSERT_DFI_WRDATA_EN_MIN                (0)
  #define DUNIT0_CR_D_CR_MCTRL_ASSERT_DFI_WRDATA_EN_MAX                (1) // 0x00000001
  #define DUNIT0_CR_D_CR_MCTRL_ASSERT_DFI_WRDATA_EN_DEF                (0x00000000)
  #define DUNIT0_CR_D_CR_MCTRL_ASSERT_DFI_WRDATA_EN_HSH                (0x01B4B02C)

  #define DUNIT0_CR_D_CR_MCTRL_ASSERT_DFI_RDDATA_EN_OFF                (27)
  #define DUNIT0_CR_D_CR_MCTRL_ASSERT_DFI_RDDATA_EN_WID                ( 1)
  #define DUNIT0_CR_D_CR_MCTRL_ASSERT_DFI_RDDATA_EN_MSK                (0x08000000)
  #define DUNIT0_CR_D_CR_MCTRL_ASSERT_DFI_RDDATA_EN_MIN                (0)
  #define DUNIT0_CR_D_CR_MCTRL_ASSERT_DFI_RDDATA_EN_MAX                (1) // 0x00000001
  #define DUNIT0_CR_D_CR_MCTRL_ASSERT_DFI_RDDATA_EN_DEF                (0x00000000)
  #define DUNIT0_CR_D_CR_MCTRL_ASSERT_DFI_RDDATA_EN_HSH                (0x01B6B02C)

  #define DUNIT0_CR_D_CR_MCTRL_SET_DFI_XDATA_CS_OFF                    (28)
  #define DUNIT0_CR_D_CR_MCTRL_SET_DFI_XDATA_CS_WID                    ( 4)
  #define DUNIT0_CR_D_CR_MCTRL_SET_DFI_XDATA_CS_MSK                    (0xF0000000)
  #define DUNIT0_CR_D_CR_MCTRL_SET_DFI_XDATA_CS_MIN                    (0)
  #define DUNIT0_CR_D_CR_MCTRL_SET_DFI_XDATA_CS_MAX                    (15) // 0x0000000F
  #define DUNIT0_CR_D_CR_MCTRL_SET_DFI_XDATA_CS_DEF                    (0x00000000)
  #define DUNIT0_CR_D_CR_MCTRL_SET_DFI_XDATA_CS_HSH                    (0x0438B02C)

#define DUNIT0_CR_D_CR_DRAM_CMDS_REG                                   (0x0000B030)
#define DUNIT0_CR_D_CR_DRAM_CMDS_DEF                                   (0x00000100)
#define DUNIT0_CR_D_CR_DRAM_CMDS_VOLATILE_BITFIELDS_MSK                (0x00000000)

  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD3_0_OFF                         ( 0)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD3_0_WID                         ( 4)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD3_0_MSK                         (0x0000000F)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD3_0_MIN                         (0)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD3_0_MAX                         (15) // 0x0000000F
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD3_0_DEF                         (0x00000000)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD3_0_HSH                         (0x0400B030)

  #define DUNIT0_CR_D_CR_DRAM_CMDS_CS_MASK_OFF                         ( 4)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_CS_MASK_WID                         ( 2)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_CS_MASK_MSK                         (0x00000030)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_CS_MASK_MIN                         (0)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_CS_MASK_MAX                         (3) // 0x00000003
  #define DUNIT0_CR_D_CR_DRAM_CMDS_CS_MASK_DEF                         (0x00000000)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_CS_MASK_HSH                         (0x0208B030)

  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD7_6_OFF                         ( 6)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD7_6_WID                         ( 2)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD7_6_MSK                         (0x000000C0)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD7_6_MIN                         (0)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD7_6_MAX                         (3) // 0x00000003
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD7_6_DEF                         (0x00000000)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD7_6_HSH                         (0x020CB030)

  #define DUNIT0_CR_D_CR_DRAM_CMDS_MULTI_CYCLE_CS_OFF                  ( 8)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_MULTI_CYCLE_CS_WID                  ( 1)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_MULTI_CYCLE_CS_MSK                  (0x00000100)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_MULTI_CYCLE_CS_MIN                  (0)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_MULTI_CYCLE_CS_MAX                  (1) // 0x00000001
  #define DUNIT0_CR_D_CR_DRAM_CMDS_MULTI_CYCLE_CS_DEF                  (0x00000001)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_MULTI_CYCLE_CS_HSH                  (0x0110B030)

  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD15_9_OFF                        ( 9)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD15_9_WID                        ( 7)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD15_9_MSK                        (0x0000FE00)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD15_9_MIN                        (0)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD15_9_MAX                        (127) // 0x0000007F
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD15_9_DEF                        (0x00000000)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD15_9_HSH                        (0x0712B030)

  #define DUNIT0_CR_D_CR_DRAM_CMDS_CA_MASK_OFF                         (16)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_CA_MASK_WID                         ( 4)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_CA_MASK_MSK                         (0x000F0000)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_CA_MASK_MIN                         (0)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_CA_MASK_MAX                         (15) // 0x0000000F
  #define DUNIT0_CR_D_CR_DRAM_CMDS_CA_MASK_DEF                         (0x00000000)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_CA_MASK_HSH                         (0x0420B030)

  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD31_20_OFF                       (20)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD31_20_WID                       (12)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD31_20_MSK                       (0xFFF00000)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD31_20_MIN                       (0)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD31_20_MAX                       (4095) // 0x00000FFF
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD31_20_DEF                       (0x00000000)
  #define DUNIT0_CR_D_CR_DRAM_CMDS_RSVD31_20_HSH                       (0x0C28B030)

#define DUNIT0_CR_D_CR_DRAM_CMDS1_REG                                  (0x0000B034)
#define DUNIT0_CR_D_CR_DRAM_CMDS1_DEF                                  (0x00000000)
#define DUNIT0_CR_D_CR_DRAM_CMDS1_VOLATILE_BITFIELDS_MSK               (0x00000001)

  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_CMD_EN_OFF                     ( 0)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_CMD_EN_WID                     ( 1)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_CMD_EN_MSK                     (0x00000001)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_CMD_EN_MIN                     (0)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_CMD_EN_MAX                     (1) // 0x00000001
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_CMD_EN_DEF                     (0x00000000)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_CMD_EN_HSH                     (0x0180B034)

  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_NONSTOP_OFF                    ( 1)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_NONSTOP_WID                    ( 1)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_NONSTOP_MSK                    (0x00000002)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_NONSTOP_MIN                    (0)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_NONSTOP_MAX                    (1) // 0x00000001
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_NONSTOP_DEF                    (0x00000000)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_NONSTOP_HSH                    (0x0102B034)

  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_MPAUSE_OFF                     ( 2)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_MPAUSE_WID                     ( 1)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_MPAUSE_MSK                     (0x00000004)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_MPAUSE_MIN                     (0)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_MPAUSE_MAX                     (1) // 0x00000001
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_MPAUSE_DEF                     (0x00000000)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_MPAUSE_HSH                     (0x0104B034)

  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RSVD7_3_OFF                        ( 3)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RSVD7_3_WID                        ( 5)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RSVD7_3_MSK                        (0x000000F8)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RSVD7_3_MIN                        (0)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RSVD7_3_MAX                        (31) // 0x0000001F
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RSVD7_3_DEF                        (0x00000000)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RSVD7_3_HSH                        (0x0506B034)

  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_CMD_DLY_OFF                    ( 8)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_CMD_DLY_WID                    ( 8)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_CMD_DLY_MSK                    (0x0000FF00)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_CMD_DLY_MIN                    (0)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_CMD_DLY_MAX                    (255) // 0x000000FF
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_CMD_DLY_DEF                    (0x00000000)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_CMD_DLY_HSH                    (0x0810B034)

  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_CMD_CNT_OFF                    (16)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_CMD_CNT_WID                    (16)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_CMD_CNT_MSK                    (0xFFFF0000)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_CMD_CNT_MIN                    (0)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_CMD_CNT_MAX                    (65535) // 0x0000FFFF
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_CMD_CNT_DEF                    (0x00000000)
  #define DUNIT0_CR_D_CR_DRAM_CMDS1_RPT_CMD_CNT_HSH                    (0x1020B034)

#define DUNIT0_CR_D_CR_DRAM_CMD_REG                                    (0x0000B03C)
#define DUNIT0_CR_D_CR_DRAM_CMD_DEF                                    (0x00000000)
#define DUNIT0_CR_D_CR_DRAM_CMD_VOLATILE_BITFIELDS_MSK                 (0x00000000)

  #define DUNIT0_CR_D_CR_DRAM_CMD_DATA_OFF                             ( 0)
  #define DUNIT0_CR_D_CR_DRAM_CMD_DATA_WID                             (28)
  #define DUNIT0_CR_D_CR_DRAM_CMD_DATA_MSK                             (0x0FFFFFFF)
  #define DUNIT0_CR_D_CR_DRAM_CMD_DATA_MIN                             (0)
  #define DUNIT0_CR_D_CR_DRAM_CMD_DATA_MAX                             (268435455) // 0x0FFFFFFF
  #define DUNIT0_CR_D_CR_DRAM_CMD_DATA_DEF                             (0x00000000)
  #define DUNIT0_CR_D_CR_DRAM_CMD_DATA_HSH                             (0x1C00B03C)

  #define DUNIT0_CR_D_CR_DRAM_CMD_CS_OFF                               (28)
  #define DUNIT0_CR_D_CR_DRAM_CMD_CS_WID                               ( 4)
  #define DUNIT0_CR_D_CR_DRAM_CMD_CS_MSK                               (0xF0000000)
  #define DUNIT0_CR_D_CR_DRAM_CMD_CS_MIN                               (0)
  #define DUNIT0_CR_D_CR_DRAM_CMD_CS_MAX                               (15) // 0x0000000F
  #define DUNIT0_CR_D_CR_DRAM_CMD_CS_DEF                               (0x00000000)
  #define DUNIT0_CR_D_CR_DRAM_CMD_CS_HSH                               (0x0438B03C)

#define DUNIT0_CR_D_CR_PS_DRAM_CMD_REG                                 (0x0000B040)
#define DUNIT0_CR_D_CR_PS_DRAM_CMD_DEF                                 (0x00000000)
#define DUNIT0_CR_D_CR_PS_DRAM_CMD_VOLATILE_BITFIELDS_MSK              (0xF0000000)

  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_OP_OFF                            ( 0)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_OP_WID                            ( 8)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_OP_MSK                            (0x000000FF)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_OP_MIN                            (0)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_OP_MAX                            (255) // 0x000000FF
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_OP_DEF                            (0x00000000)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_OP_HSH                            (0x0800B040)

  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_MA_OFF                            ( 8)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_MA_WID                            ( 8)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_MA_MSK                            (0x0000FF00)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_MA_MIN                            (0)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_MA_MAX                            (255) // 0x000000FF
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_MA_DEF                            (0x00000000)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_MA_HSH                            (0x0810B040)

  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_RSVD19_16_OFF                     (16)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_RSVD19_16_WID                     ( 4)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_RSVD19_16_MSK                     (0x000F0000)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_RSVD19_16_MIN                     (0)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_RSVD19_16_MAX                     (15) // 0x0000000F
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_RSVD19_16_DEF                     (0x00000000)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_RSVD19_16_HSH                     (0x0420B040)

  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_CMD_OFF                           (20)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_CMD_WID                           ( 2)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_CMD_MSK                           (0x00300000)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_CMD_MIN                           (0)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_CMD_MAX                           (3) // 0x00000003
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_CMD_DEF                           (0x00000000)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_CMD_HSH                           (0x0228B040)

  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_RSVD23_22_OFF                     (22)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_RSVD23_22_WID                     ( 2)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_RSVD23_22_MSK                     (0x00C00000)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_RSVD23_22_MIN                     (0)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_RSVD23_22_MAX                     (3) // 0x00000003
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_RSVD23_22_DEF                     (0x00000000)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_RSVD23_22_HSH                     (0x022CB040)

  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_CS_OFF                            (24)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_CS_WID                            ( 4)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_CS_MSK                            (0x0F000000)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_CS_MIN                            (0)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_CS_MAX                            (15) // 0x0000000F
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_CS_DEF                            (0x00000000)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_CS_HSH                            (0x0430B040)

  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_FIFO_LEVEL_OFF                    (28)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_FIFO_LEVEL_WID                    ( 4)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_FIFO_LEVEL_MSK                    (0xF0000000)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_FIFO_LEVEL_MIN                    (0)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_FIFO_LEVEL_MAX                    (15) // 0x0000000F
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_FIFO_LEVEL_DEF                    (0x00000000)
  #define DUNIT0_CR_D_CR_PS_DRAM_CMD_FIFO_LEVEL_HSH                    (0x04B8B040)

#define DUNIT0_CR_D_CR_MRRD0_REG                                       (0x0000B080)
#define DUNIT0_CR_D_CR_MRRD0_DEF                                       (0x00000000)
#define DUNIT0_CR_D_CR_MRRD0_VOLATILE_BITFIELDS_MSK                    (0xFFFFFFFF)

  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA0_OFF                           ( 0)
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA0_WID                           ( 8)
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA0_MSK                           (0x000000FF)
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA0_MIN                           (0)
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA0_MAX                           (255) // 0x000000FF
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA0_DEF                           (0x00000000)
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA0_HSH                           (0x0880B080)

  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA1_OFF                           ( 8)
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA1_WID                           ( 8)
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA1_MSK                           (0x0000FF00)
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA1_MIN                           (0)
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA1_MAX                           (255) // 0x000000FF
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA1_DEF                           (0x00000000)
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA1_HSH                           (0x0890B080)

  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA2_OFF                           (16)
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA2_WID                           ( 8)
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA2_MSK                           (0x00FF0000)
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA2_MIN                           (0)
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA2_MAX                           (255) // 0x000000FF
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA2_DEF                           (0x00000000)
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA2_HSH                           (0x08A0B080)

  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA3_OFF                           (24)
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA3_WID                           ( 8)
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA3_MSK                           (0xFF000000)
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA3_MIN                           (0)
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA3_MAX                           (255) // 0x000000FF
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA3_DEF                           (0x00000000)
  #define DUNIT0_CR_D_CR_MRRD0_MRR_DATA3_HSH                           (0x08B0B080)

#define DUNIT0_CR_D_CR_MRRD1_REG                                       (0x0000B084)
#define DUNIT0_CR_D_CR_MRRD1_DEF                                       (0x00000000)
#define DUNIT0_CR_D_CR_MRRD1_VOLATILE_BITFIELDS_MSK                    (0xFFFFFFFF)

  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA4_OFF                           ( 0)
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA4_WID                           ( 8)
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA4_MSK                           (0x000000FF)
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA4_MIN                           (0)
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA4_MAX                           (255) // 0x000000FF
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA4_DEF                           (0x00000000)
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA4_HSH                           (0x0880B084)

  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA5_OFF                           ( 8)
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA5_WID                           ( 8)
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA5_MSK                           (0x0000FF00)
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA5_MIN                           (0)
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA5_MAX                           (255) // 0x000000FF
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA5_DEF                           (0x00000000)
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA5_HSH                           (0x0890B084)

  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA6_OFF                           (16)
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA6_WID                           ( 8)
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA6_MSK                           (0x00FF0000)
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA6_MIN                           (0)
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA6_MAX                           (255) // 0x000000FF
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA6_DEF                           (0x00000000)
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA6_HSH                           (0x08A0B084)

  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA7_OFF                           (24)
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA7_WID                           ( 8)
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA7_MSK                           (0xFF000000)
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA7_MIN                           (0)
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA7_MAX                           (255) // 0x000000FF
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA7_DEF                           (0x00000000)
  #define DUNIT0_CR_D_CR_MRRD1_MRR_DATA7_HSH                           (0x08B0B084)

#define DUNIT0_CR_D_CR_MRRD2_REG                                       (0x0000B088)
#define DUNIT0_CR_D_CR_MRRD2_DEF                                       (0x00000000)
#define DUNIT0_CR_D_CR_MRRD2_VOLATILE_BITFIELDS_MSK                    (0x0000FFFF)

  #define DUNIT0_CR_D_CR_MRRD2_MRR_DATA8_OFF                           ( 0)
  #define DUNIT0_CR_D_CR_MRRD2_MRR_DATA8_WID                           ( 8)
  #define DUNIT0_CR_D_CR_MRRD2_MRR_DATA8_MSK                           (0x000000FF)
  #define DUNIT0_CR_D_CR_MRRD2_MRR_DATA8_MIN                           (0)
  #define DUNIT0_CR_D_CR_MRRD2_MRR_DATA8_MAX                           (255) // 0x000000FF
  #define DUNIT0_CR_D_CR_MRRD2_MRR_DATA8_DEF                           (0x00000000)
  #define DUNIT0_CR_D_CR_MRRD2_MRR_DATA8_HSH                           (0x0880B088)

  #define DUNIT0_CR_D_CR_MRRD2_MRR_DATA9_OFF                           ( 8)
  #define DUNIT0_CR_D_CR_MRRD2_MRR_DATA9_WID                           ( 8)
  #define DUNIT0_CR_D_CR_MRRD2_MRR_DATA9_MSK                           (0x0000FF00)
  #define DUNIT0_CR_D_CR_MRRD2_MRR_DATA9_MIN                           (0)
  #define DUNIT0_CR_D_CR_MRRD2_MRR_DATA9_MAX                           (255) // 0x000000FF
  #define DUNIT0_CR_D_CR_MRRD2_MRR_DATA9_DEF                           (0x00000000)
  #define DUNIT0_CR_D_CR_MRRD2_MRR_DATA9_HSH                           (0x0890B088)

  #define DUNIT0_CR_D_CR_MRRD2_RSVD31_16_OFF                           (16)
  #define DUNIT0_CR_D_CR_MRRD2_RSVD31_16_WID                           (16)
  #define DUNIT0_CR_D_CR_MRRD2_RSVD31_16_MSK                           (0xFFFF0000)
  #define DUNIT0_CR_D_CR_MRRD2_RSVD31_16_MIN                           (0)
  #define DUNIT0_CR_D_CR_MRRD2_RSVD31_16_MAX                           (65535) // 0x0000FFFF
  #define DUNIT0_CR_D_CR_MRRD2_RSVD31_16_DEF                           (0x00000000)
  #define DUNIT0_CR_D_CR_MRRD2_RSVD31_16_HSH                           (0x1020B088)

#define DUNIT0_CR_D_CR_DOC_REG                                         (0x0000B090)
#define DUNIT0_CR_D_CR_DOC_DEF                                         (0x00000020)
#define DUNIT0_CR_D_CR_DOC_VOLATILE_BITFIELDS_MSK                      (0x00000000)

  #define DUNIT0_CR_D_CR_DOC_RSVD1_0_OFF                               ( 0)
  #define DUNIT0_CR_D_CR_DOC_RSVD1_0_WID                               ( 2)
  #define DUNIT0_CR_D_CR_DOC_RSVD1_0_MSK                               (0x00000003)
  #define DUNIT0_CR_D_CR_DOC_RSVD1_0_MIN                               (0)
  #define DUNIT0_CR_D_CR_DOC_RSVD1_0_MAX                               (3) // 0x00000003
  #define DUNIT0_CR_D_CR_DOC_RSVD1_0_DEF                               (0x00000000)
  #define DUNIT0_CR_D_CR_DOC_RSVD1_0_HSH                               (0x0200B090)

  #define DUNIT0_CR_D_CR_DOC_MNT_EN_OFF                                ( 2)
  #define DUNIT0_CR_D_CR_DOC_MNT_EN_WID                                ( 1)
  #define DUNIT0_CR_D_CR_DOC_MNT_EN_MSK                                (0x00000004)
  #define DUNIT0_CR_D_CR_DOC_MNT_EN_MIN                                (0)
  #define DUNIT0_CR_D_CR_DOC_MNT_EN_MAX                                (1) // 0x00000001
  #define DUNIT0_CR_D_CR_DOC_MNT_EN_DEF                                (0x00000000)
  #define DUNIT0_CR_D_CR_DOC_MNT_EN_HSH                                (0x0104B090)

  #define DUNIT0_CR_D_CR_DOC_RSVD4_3_OFF                               ( 3)
  #define DUNIT0_CR_D_CR_DOC_RSVD4_3_WID                               ( 2)
  #define DUNIT0_CR_D_CR_DOC_RSVD4_3_MSK                               (0x00000018)
  #define DUNIT0_CR_D_CR_DOC_RSVD4_3_MIN                               (0)
  #define DUNIT0_CR_D_CR_DOC_RSVD4_3_MAX                               (3) // 0x00000003
  #define DUNIT0_CR_D_CR_DOC_RSVD4_3_DEF                               (0x00000000)
  #define DUNIT0_CR_D_CR_DOC_RSVD4_3_HSH                               (0x0206B090)

  #define DUNIT0_CR_D_CR_DOC_DFI_DDR5_2N_CS_POS_OFF                    ( 5)
  #define DUNIT0_CR_D_CR_DOC_DFI_DDR5_2N_CS_POS_WID                    ( 1)
  #define DUNIT0_CR_D_CR_DOC_DFI_DDR5_2N_CS_POS_MSK                    (0x00000020)
  #define DUNIT0_CR_D_CR_DOC_DFI_DDR5_2N_CS_POS_MIN                    (0)
  #define DUNIT0_CR_D_CR_DOC_DFI_DDR5_2N_CS_POS_MAX                    (1) // 0x00000001
  #define DUNIT0_CR_D_CR_DOC_DFI_DDR5_2N_CS_POS_DEF                    (0x00000001)
  #define DUNIT0_CR_D_CR_DOC_DFI_DDR5_2N_CS_POS_HSH                    (0x010AB090)

  #define DUNIT0_CR_D_CR_DOC_EARLY_DFI_CA_VALID_OFF                    ( 6)
  #define DUNIT0_CR_D_CR_DOC_EARLY_DFI_CA_VALID_WID                    ( 1)
  #define DUNIT0_CR_D_CR_DOC_EARLY_DFI_CA_VALID_MSK                    (0x00000040)
  #define DUNIT0_CR_D_CR_DOC_EARLY_DFI_CA_VALID_MIN                    (0)
  #define DUNIT0_CR_D_CR_DOC_EARLY_DFI_CA_VALID_MAX                    (1) // 0x00000001
  #define DUNIT0_CR_D_CR_DOC_EARLY_DFI_CA_VALID_DEF                    (0x00000000)
  #define DUNIT0_CR_D_CR_DOC_EARLY_DFI_CA_VALID_HSH                    (0x010CB090)

  #define DUNIT0_CR_D_CR_DOC_RSVD27_7_OFF                              ( 7)
  #define DUNIT0_CR_D_CR_DOC_RSVD27_7_WID                              (21)
  #define DUNIT0_CR_D_CR_DOC_RSVD27_7_MSK                              (0x0FFFFF80)
  #define DUNIT0_CR_D_CR_DOC_RSVD27_7_MIN                              (0)
  #define DUNIT0_CR_D_CR_DOC_RSVD27_7_MAX                              (2097151) // 0x001FFFFF
  #define DUNIT0_CR_D_CR_DOC_RSVD27_7_DEF                              (0x00000000)
  #define DUNIT0_CR_D_CR_DOC_RSVD27_7_HSH                              (0x150EB090)

  #define DUNIT0_CR_D_CR_DOC_WR_DBI_EN_OFF                             (28)
  #define DUNIT0_CR_D_CR_DOC_WR_DBI_EN_WID                             ( 1)
  #define DUNIT0_CR_D_CR_DOC_WR_DBI_EN_MSK                             (0x10000000)
  #define DUNIT0_CR_D_CR_DOC_WR_DBI_EN_MIN                             (0)
  #define DUNIT0_CR_D_CR_DOC_WR_DBI_EN_MAX                             (1) // 0x00000001
  #define DUNIT0_CR_D_CR_DOC_WR_DBI_EN_DEF                             (0x00000000)
  #define DUNIT0_CR_D_CR_DOC_WR_DBI_EN_HSH                             (0x0138B090)

  #define DUNIT0_CR_D_CR_DOC_RD_DBI_EN_OFF                             (29)
  #define DUNIT0_CR_D_CR_DOC_RD_DBI_EN_WID                             ( 1)
  #define DUNIT0_CR_D_CR_DOC_RD_DBI_EN_MSK                             (0x20000000)
  #define DUNIT0_CR_D_CR_DOC_RD_DBI_EN_MIN                             (0)
  #define DUNIT0_CR_D_CR_DOC_RD_DBI_EN_MAX                             (1) // 0x00000001
  #define DUNIT0_CR_D_CR_DOC_RD_DBI_EN_DEF                             (0x00000000)
  #define DUNIT0_CR_D_CR_DOC_RD_DBI_EN_HSH                             (0x013AB090)

  #define DUNIT0_CR_D_CR_DOC_RSVD30_OFF                                (30)
  #define DUNIT0_CR_D_CR_DOC_RSVD30_WID                                ( 1)
  #define DUNIT0_CR_D_CR_DOC_RSVD30_MSK                                (0x40000000)
  #define DUNIT0_CR_D_CR_DOC_RSVD30_MIN                                (0)
  #define DUNIT0_CR_D_CR_DOC_RSVD30_MAX                                (1) // 0x00000001
  #define DUNIT0_CR_D_CR_DOC_RSVD30_DEF                                (0x00000000)
  #define DUNIT0_CR_D_CR_DOC_RSVD30_HSH                                (0x013CB090)

  #define DUNIT0_CR_D_CR_DOC_DUNIT_INIT_DN_OFF                         (31)
  #define DUNIT0_CR_D_CR_DOC_DUNIT_INIT_DN_WID                         ( 1)
  #define DUNIT0_CR_D_CR_DOC_DUNIT_INIT_DN_MSK                         (0x80000000)
  #define DUNIT0_CR_D_CR_DOC_DUNIT_INIT_DN_MIN                         (0)
  #define DUNIT0_CR_D_CR_DOC_DUNIT_INIT_DN_MAX                         (1) // 0x00000001
  #define DUNIT0_CR_D_CR_DOC_DUNIT_INIT_DN_DEF                         (0x00000000)
  #define DUNIT0_CR_D_CR_DOC_DUNIT_INIT_DN_HSH                         (0x013EB090)

#define DUNIT0_CR_D_CR_STATUS_REG                                      (0x0000B094)
#define DUNIT0_CR_D_CR_STATUS_DEF                                      (0x00000060)
#define DUNIT0_CR_D_CR_STATUS_VOLATILE_BITFIELDS_MSK                   (0xF7710F7B)

  #define DUNIT0_CR_D_CR_STATUS_IN_PD_OFF                              ( 0)
  #define DUNIT0_CR_D_CR_STATUS_IN_PD_WID                              ( 1)
  #define DUNIT0_CR_D_CR_STATUS_IN_PD_MSK                              (0x00000001)
  #define DUNIT0_CR_D_CR_STATUS_IN_PD_MIN                              (0)
  #define DUNIT0_CR_D_CR_STATUS_IN_PD_MAX                              (1) // 0x00000001
  #define DUNIT0_CR_D_CR_STATUS_IN_PD_DEF                              (0x00000000)
  #define DUNIT0_CR_D_CR_STATUS_IN_PD_HSH                              (0x0180B094)

  #define DUNIT0_CR_D_CR_STATUS_IN_SR_OFF                              ( 1)
  #define DUNIT0_CR_D_CR_STATUS_IN_SR_WID                              ( 1)
  #define DUNIT0_CR_D_CR_STATUS_IN_SR_MSK                              (0x00000002)
  #define DUNIT0_CR_D_CR_STATUS_IN_SR_MIN                              (0)
  #define DUNIT0_CR_D_CR_STATUS_IN_SR_MAX                              (1) // 0x00000001
  #define DUNIT0_CR_D_CR_STATUS_IN_SR_DEF                              (0x00000000)
  #define DUNIT0_CR_D_CR_STATUS_IN_SR_HSH                              (0x0182B094)

  #define DUNIT0_CR_D_CR_STATUS_RSVD2_OFF                              ( 2)
  #define DUNIT0_CR_D_CR_STATUS_RSVD2_WID                              ( 1)
  #define DUNIT0_CR_D_CR_STATUS_RSVD2_MSK                              (0x00000004)
  #define DUNIT0_CR_D_CR_STATUS_RSVD2_MIN                              (0)
  #define DUNIT0_CR_D_CR_STATUS_RSVD2_MAX                              (1) // 0x00000001
  #define DUNIT0_CR_D_CR_STATUS_RSVD2_DEF                              (0x00000000)
  #define DUNIT0_CR_D_CR_STATUS_RSVD2_HSH                              (0x0104B094)

  #define DUNIT0_CR_D_CR_STATUS_ALL_BANKS_CLOSED_OFF                   ( 3)
  #define DUNIT0_CR_D_CR_STATUS_ALL_BANKS_CLOSED_WID                   ( 1)
  #define DUNIT0_CR_D_CR_STATUS_ALL_BANKS_CLOSED_MSK                   (0x00000008)
  #define DUNIT0_CR_D_CR_STATUS_ALL_BANKS_CLOSED_MIN                   (0)
  #define DUNIT0_CR_D_CR_STATUS_ALL_BANKS_CLOSED_MAX                   (1) // 0x00000001
  #define DUNIT0_CR_D_CR_STATUS_ALL_BANKS_CLOSED_DEF                   (0x00000000)
  #define DUNIT0_CR_D_CR_STATUS_ALL_BANKS_CLOSED_HSH                   (0x0186B094)

  #define DUNIT0_CR_D_CR_STATUS_ALL_QUEUES_EMPTY_OFF                   ( 4)
  #define DUNIT0_CR_D_CR_STATUS_ALL_QUEUES_EMPTY_WID                   ( 1)
  #define DUNIT0_CR_D_CR_STATUS_ALL_QUEUES_EMPTY_MSK                   (0x00000010)
  #define DUNIT0_CR_D_CR_STATUS_ALL_QUEUES_EMPTY_MIN                   (0)
  #define DUNIT0_CR_D_CR_STATUS_ALL_QUEUES_EMPTY_MAX                   (1) // 0x00000001
  #define DUNIT0_CR_D_CR_STATUS_ALL_QUEUES_EMPTY_DEF                   (0x00000000)
  #define DUNIT0_CR_D_CR_STATUS_ALL_QUEUES_EMPTY_HSH                   (0x0188B094)

  #define DUNIT0_CR_D_CR_STATUS_CMDQ_EMPTY_OFF                         ( 5)
  #define DUNIT0_CR_D_CR_STATUS_CMDQ_EMPTY_WID                         ( 1)
  #define DUNIT0_CR_D_CR_STATUS_CMDQ_EMPTY_MSK                         (0x00000020)
  #define DUNIT0_CR_D_CR_STATUS_CMDQ_EMPTY_MIN                         (0)
  #define DUNIT0_CR_D_CR_STATUS_CMDQ_EMPTY_MAX                         (1) // 0x00000001
  #define DUNIT0_CR_D_CR_STATUS_CMDQ_EMPTY_DEF                         (0x00000001)
  #define DUNIT0_CR_D_CR_STATUS_CMDQ_EMPTY_HSH                         (0x018AB094)

  #define DUNIT0_CR_D_CR_STATUS_WDB_EMPTY_OFF                          ( 6)
  #define DUNIT0_CR_D_CR_STATUS_WDB_EMPTY_WID                          ( 1)
  #define DUNIT0_CR_D_CR_STATUS_WDB_EMPTY_MSK                          (0x00000040)
  #define DUNIT0_CR_D_CR_STATUS_WDB_EMPTY_MIN                          (0)
  #define DUNIT0_CR_D_CR_STATUS_WDB_EMPTY_MAX                          (1) // 0x00000001
  #define DUNIT0_CR_D_CR_STATUS_WDB_EMPTY_DEF                          (0x00000001)
  #define DUNIT0_CR_D_CR_STATUS_WDB_EMPTY_HSH                          (0x018CB094)

  #define DUNIT0_CR_D_CR_STATUS_RSVD7_OFF                              ( 7)
  #define DUNIT0_CR_D_CR_STATUS_RSVD7_WID                              ( 1)
  #define DUNIT0_CR_D_CR_STATUS_RSVD7_MSK                              (0x00000080)
  #define DUNIT0_CR_D_CR_STATUS_RSVD7_MIN                              (0)
  #define DUNIT0_CR_D_CR_STATUS_RSVD7_MAX                              (1) // 0x00000001
  #define DUNIT0_CR_D_CR_STATUS_RSVD7_DEF                              (0x00000000)
  #define DUNIT0_CR_D_CR_STATUS_RSVD7_HSH                              (0x010EB094)

  #define DUNIT0_CR_D_CR_STATUS_RD_IN_PROGRESS_OFF                     ( 8)
  #define DUNIT0_CR_D_CR_STATUS_RD_IN_PROGRESS_WID                     ( 4)
  #define DUNIT0_CR_D_CR_STATUS_RD_IN_PROGRESS_MSK                     (0x00000F00)
  #define DUNIT0_CR_D_CR_STATUS_RD_IN_PROGRESS_MIN                     (0)
  #define DUNIT0_CR_D_CR_STATUS_RD_IN_PROGRESS_MAX                     (15) // 0x0000000F
  #define DUNIT0_CR_D_CR_STATUS_RD_IN_PROGRESS_DEF                     (0x00000000)
  #define DUNIT0_CR_D_CR_STATUS_RD_IN_PROGRESS_HSH                     (0x0490B094)

  #define DUNIT0_CR_D_CR_STATUS_RSVD15_12_OFF                          (12)
  #define DUNIT0_CR_D_CR_STATUS_RSVD15_12_WID                          ( 4)
  #define DUNIT0_CR_D_CR_STATUS_RSVD15_12_MSK                          (0x0000F000)
  #define DUNIT0_CR_D_CR_STATUS_RSVD15_12_MIN                          (0)
  #define DUNIT0_CR_D_CR_STATUS_RSVD15_12_MAX                          (15) // 0x0000000F
  #define DUNIT0_CR_D_CR_STATUS_RSVD15_12_DEF                          (0x00000000)
  #define DUNIT0_CR_D_CR_STATUS_RSVD15_12_HSH                          (0x0418B094)

  #define DUNIT0_CR_D_CR_STATUS_WCK_ON_OFF                             (16)
  #define DUNIT0_CR_D_CR_STATUS_WCK_ON_WID                             ( 1)
  #define DUNIT0_CR_D_CR_STATUS_WCK_ON_MSK                             (0x00010000)
  #define DUNIT0_CR_D_CR_STATUS_WCK_ON_MIN                             (0)
  #define DUNIT0_CR_D_CR_STATUS_WCK_ON_MAX                             (1) // 0x00000001
  #define DUNIT0_CR_D_CR_STATUS_WCK_ON_DEF                             (0x00000000)
  #define DUNIT0_CR_D_CR_STATUS_WCK_ON_HSH                             (0x01A0B094)

  #define DUNIT0_CR_D_CR_STATUS_RSVD19_17_OFF                          (17)
  #define DUNIT0_CR_D_CR_STATUS_RSVD19_17_WID                          ( 3)
  #define DUNIT0_CR_D_CR_STATUS_RSVD19_17_MSK                          (0x000E0000)
  #define DUNIT0_CR_D_CR_STATUS_RSVD19_17_MIN                          (0)
  #define DUNIT0_CR_D_CR_STATUS_RSVD19_17_MAX                          (7) // 0x00000007
  #define DUNIT0_CR_D_CR_STATUS_RSVD19_17_DEF                          (0x00000000)
  #define DUNIT0_CR_D_CR_STATUS_RSVD19_17_HSH                          (0x0322B094)

  #define DUNIT0_CR_D_CR_STATUS_MEM_REQ_OFF                            (20)
  #define DUNIT0_CR_D_CR_STATUS_MEM_REQ_WID                            ( 1)
  #define DUNIT0_CR_D_CR_STATUS_MEM_REQ_MSK                            (0x00100000)
  #define DUNIT0_CR_D_CR_STATUS_MEM_REQ_MIN                            (0)
  #define DUNIT0_CR_D_CR_STATUS_MEM_REQ_MAX                            (1) // 0x00000001
  #define DUNIT0_CR_D_CR_STATUS_MEM_REQ_DEF                            (0x00000000)
  #define DUNIT0_CR_D_CR_STATUS_MEM_REQ_HSH                            (0x01A8B094)

  #define DUNIT0_CR_D_CR_STATUS_REF_REQ_OFF                            (21)
  #define DUNIT0_CR_D_CR_STATUS_REF_REQ_WID                            ( 1)
  #define DUNIT0_CR_D_CR_STATUS_REF_REQ_MSK                            (0x00200000)
  #define DUNIT0_CR_D_CR_STATUS_REF_REQ_MIN                            (0)
  #define DUNIT0_CR_D_CR_STATUS_REF_REQ_MAX                            (1) // 0x00000001
  #define DUNIT0_CR_D_CR_STATUS_REF_REQ_DEF                            (0x00000000)
  #define DUNIT0_CR_D_CR_STATUS_REF_REQ_HSH                            (0x01AAB094)

  #define DUNIT0_CR_D_CR_STATUS_ZQCAL_REQ_OFF                          (22)
  #define DUNIT0_CR_D_CR_STATUS_ZQCAL_REQ_WID                          ( 1)
  #define DUNIT0_CR_D_CR_STATUS_ZQCAL_REQ_MSK                          (0x00400000)
  #define DUNIT0_CR_D_CR_STATUS_ZQCAL_REQ_MIN                          (0)
  #define DUNIT0_CR_D_CR_STATUS_ZQCAL_REQ_MAX                          (1) // 0x00000001
  #define DUNIT0_CR_D_CR_STATUS_ZQCAL_REQ_DEF                          (0x00000000)
  #define DUNIT0_CR_D_CR_STATUS_ZQCAL_REQ_HSH                          (0x01ACB094)

  #define DUNIT0_CR_D_CR_STATUS_RSVD23_OFF                             (23)
  #define DUNIT0_CR_D_CR_STATUS_RSVD23_WID                             ( 1)
  #define DUNIT0_CR_D_CR_STATUS_RSVD23_MSK                             (0x00800000)
  #define DUNIT0_CR_D_CR_STATUS_RSVD23_MIN                             (0)
  #define DUNIT0_CR_D_CR_STATUS_RSVD23_MAX                             (1) // 0x00000001
  #define DUNIT0_CR_D_CR_STATUS_RSVD23_DEF                             (0x00000000)
  #define DUNIT0_CR_D_CR_STATUS_RSVD23_HSH                             (0x012EB094)

  #define DUNIT0_CR_D_CR_STATUS_DQS2DQ_CAL_REQ_OFF                     (24)
  #define DUNIT0_CR_D_CR_STATUS_DQS2DQ_CAL_REQ_WID                     ( 1)
  #define DUNIT0_CR_D_CR_STATUS_DQS2DQ_CAL_REQ_MSK                     (0x01000000)
  #define DUNIT0_CR_D_CR_STATUS_DQS2DQ_CAL_REQ_MIN                     (0)
  #define DUNIT0_CR_D_CR_STATUS_DQS2DQ_CAL_REQ_MAX                     (1) // 0x00000001
  #define DUNIT0_CR_D_CR_STATUS_DQS2DQ_CAL_REQ_DEF                     (0x00000000)
  #define DUNIT0_CR_D_CR_STATUS_DQS2DQ_CAL_REQ_HSH                     (0x01B0B094)

  #define DUNIT0_CR_D_CR_STATUS_WCK2DQO_CAL_REQ_OFF                    (25)
  #define DUNIT0_CR_D_CR_STATUS_WCK2DQO_CAL_REQ_WID                    ( 1)
  #define DUNIT0_CR_D_CR_STATUS_WCK2DQO_CAL_REQ_MSK                    (0x02000000)
  #define DUNIT0_CR_D_CR_STATUS_WCK2DQO_CAL_REQ_MIN                    (0)
  #define DUNIT0_CR_D_CR_STATUS_WCK2DQO_CAL_REQ_MAX                    (1) // 0x00000001
  #define DUNIT0_CR_D_CR_STATUS_WCK2DQO_CAL_REQ_DEF                    (0x00000000)
  #define DUNIT0_CR_D_CR_STATUS_WCK2DQO_CAL_REQ_HSH                    (0x01B2B094)

  #define DUNIT0_CR_D_CR_STATUS_PHYUPD_REQ_OFF                         (26)
  #define DUNIT0_CR_D_CR_STATUS_PHYUPD_REQ_WID                         ( 1)
  #define DUNIT0_CR_D_CR_STATUS_PHYUPD_REQ_MSK                         (0x04000000)
  #define DUNIT0_CR_D_CR_STATUS_PHYUPD_REQ_MIN                         (0)
  #define DUNIT0_CR_D_CR_STATUS_PHYUPD_REQ_MAX                         (1) // 0x00000001
  #define DUNIT0_CR_D_CR_STATUS_PHYUPD_REQ_DEF                         (0x00000000)
  #define DUNIT0_CR_D_CR_STATUS_PHYUPD_REQ_HSH                         (0x01B4B094)

  #define DUNIT0_CR_D_CR_STATUS_RSVD27_OFF                             (27)
  #define DUNIT0_CR_D_CR_STATUS_RSVD27_WID                             ( 1)
  #define DUNIT0_CR_D_CR_STATUS_RSVD27_MSK                             (0x08000000)
  #define DUNIT0_CR_D_CR_STATUS_RSVD27_MIN                             (0)
  #define DUNIT0_CR_D_CR_STATUS_RSVD27_MAX                             (1) // 0x00000001
  #define DUNIT0_CR_D_CR_STATUS_RSVD27_DEF                             (0x00000000)
  #define DUNIT0_CR_D_CR_STATUS_RSVD27_HSH                             (0x0136B094)

  #define DUNIT0_CR_D_CR_STATUS_PHY_INIT_COMPLETE_OFF                  (28)
  #define DUNIT0_CR_D_CR_STATUS_PHY_INIT_COMPLETE_WID                  ( 4)
  #define DUNIT0_CR_D_CR_STATUS_PHY_INIT_COMPLETE_MSK                  (0xF0000000)
  #define DUNIT0_CR_D_CR_STATUS_PHY_INIT_COMPLETE_MIN                  (0)
  #define DUNIT0_CR_D_CR_STATUS_PHY_INIT_COMPLETE_MAX                  (15) // 0x0000000F
  #define DUNIT0_CR_D_CR_STATUS_PHY_INIT_COMPLETE_DEF                  (0x00000000)
  #define DUNIT0_CR_D_CR_STATUS_PHY_INIT_COMPLETE_HSH                  (0x04B8B094)

#define DUNIT0_CR_D_CR_BANK_STATUS0_REG                                (0x0000B0A0)
#define DUNIT0_CR_D_CR_BANK_STATUS0_DEF                                (0x00000000)
#define DUNIT0_CR_D_CR_BANK_STATUS0_VOLATILE_BITFIELDS_MSK             (0xFFFFFFFF)

  #define DUNIT0_CR_D_CR_BANK_STATUS0_R0_BANK_STATUS_OFF               ( 0)
  #define DUNIT0_CR_D_CR_BANK_STATUS0_R0_BANK_STATUS_WID               (32)
  #define DUNIT0_CR_D_CR_BANK_STATUS0_R0_BANK_STATUS_MSK               (0xFFFFFFFF)
  #define DUNIT0_CR_D_CR_BANK_STATUS0_R0_BANK_STATUS_MIN               (0)
  #define DUNIT0_CR_D_CR_BANK_STATUS0_R0_BANK_STATUS_MAX               (4294967295) // 0xFFFFFFFF
  #define DUNIT0_CR_D_CR_BANK_STATUS0_R0_BANK_STATUS_DEF               (0x00000000)
  #define DUNIT0_CR_D_CR_BANK_STATUS0_R0_BANK_STATUS_HSH               (0x2080B0A0)

#define DUNIT0_CR_D_CR_BANK_STATUS1_REG                                (0x0000B0A4)
#define DUNIT0_CR_D_CR_BANK_STATUS1_DEF                                (0x00000000)
#define DUNIT0_CR_D_CR_BANK_STATUS1_VOLATILE_BITFIELDS_MSK             (0xFFFFFFFF)

  #define DUNIT0_CR_D_CR_BANK_STATUS1_R1_BANK_STATUS_OFF               ( 0)
  #define DUNIT0_CR_D_CR_BANK_STATUS1_R1_BANK_STATUS_WID               (32)
  #define DUNIT0_CR_D_CR_BANK_STATUS1_R1_BANK_STATUS_MSK               (0xFFFFFFFF)
  #define DUNIT0_CR_D_CR_BANK_STATUS1_R1_BANK_STATUS_MIN               (0)
  #define DUNIT0_CR_D_CR_BANK_STATUS1_R1_BANK_STATUS_MAX               (4294967295) // 0xFFFFFFFF
  #define DUNIT0_CR_D_CR_BANK_STATUS1_R1_BANK_STATUS_DEF               (0x00000000)
  #define DUNIT0_CR_D_CR_BANK_STATUS1_R1_BANK_STATUS_HSH               (0x2080B0A4)

#define DUNIT0_CR_D_CR_BANK_STATUS2_REG                                (0x0000B0A8)
#define DUNIT0_CR_D_CR_BANK_STATUS2_DEF                                (0x00000000)
#define DUNIT0_CR_D_CR_BANK_STATUS2_VOLATILE_BITFIELDS_MSK             (0xFFFFFFFF)

  #define DUNIT0_CR_D_CR_BANK_STATUS2_R2_BANK_STATUS_OFF               ( 0)
  #define DUNIT0_CR_D_CR_BANK_STATUS2_R2_BANK_STATUS_WID               (32)
  #define DUNIT0_CR_D_CR_BANK_STATUS2_R2_BANK_STATUS_MSK               (0xFFFFFFFF)
  #define DUNIT0_CR_D_CR_BANK_STATUS2_R2_BANK_STATUS_MIN               (0)
  #define DUNIT0_CR_D_CR_BANK_STATUS2_R2_BANK_STATUS_MAX               (4294967295) // 0xFFFFFFFF
  #define DUNIT0_CR_D_CR_BANK_STATUS2_R2_BANK_STATUS_DEF               (0x00000000)
  #define DUNIT0_CR_D_CR_BANK_STATUS2_R2_BANK_STATUS_HSH               (0x2080B0A8)

#define DUNIT0_CR_D_CR_BANK_STATUS3_REG                                (0x0000B0AC)
#define DUNIT0_CR_D_CR_BANK_STATUS3_DEF                                (0x00000000)
#define DUNIT0_CR_D_CR_BANK_STATUS3_VOLATILE_BITFIELDS_MSK             (0xFFFFFFFF)

  #define DUNIT0_CR_D_CR_BANK_STATUS3_R3_BANK_STATUS_OFF               ( 0)
  #define DUNIT0_CR_D_CR_BANK_STATUS3_R3_BANK_STATUS_WID               (32)
  #define DUNIT0_CR_D_CR_BANK_STATUS3_R3_BANK_STATUS_MSK               (0xFFFFFFFF)
  #define DUNIT0_CR_D_CR_BANK_STATUS3_R3_BANK_STATUS_MIN               (0)
  #define DUNIT0_CR_D_CR_BANK_STATUS3_R3_BANK_STATUS_MAX               (4294967295) // 0xFFFFFFFF
  #define DUNIT0_CR_D_CR_BANK_STATUS3_R3_BANK_STATUS_DEF               (0x00000000)
  #define DUNIT0_CR_D_CR_BANK_STATUS3_R3_BANK_STATUS_HSH               (0x2080B0AC)

#define DUNIT0_CR_D_CR_MISC0_REG                                       (0x0000B0B4)
#define DUNIT0_CR_D_CR_MISC0_DEF                                       (0x01000000)
#define DUNIT0_CR_D_CR_MISC0_VOLATILE_BITFIELDS_MSK                    (0x00000B00)

  #define DUNIT0_CR_D_CR_MISC0_DCMDQ_RST_OFF                           ( 0)
  #define DUNIT0_CR_D_CR_MISC0_DCMDQ_RST_WID                           ( 1)
  #define DUNIT0_CR_D_CR_MISC0_DCMDQ_RST_MSK                           (0x00000001)
  #define DUNIT0_CR_D_CR_MISC0_DCMDQ_RST_MIN                           (0)
  #define DUNIT0_CR_D_CR_MISC0_DCMDQ_RST_MAX                           (1) // 0x00000001
  #define DUNIT0_CR_D_CR_MISC0_DCMDQ_RST_DEF                           (0x00000000)
  #define DUNIT0_CR_D_CR_MISC0_DCMDQ_RST_HSH                           (0x0100B0B4)

  #define DUNIT0_CR_D_CR_MISC0_DTMR_RST_OFF                            ( 1)
  #define DUNIT0_CR_D_CR_MISC0_DTMR_RST_WID                            ( 1)
  #define DUNIT0_CR_D_CR_MISC0_DTMR_RST_MSK                            (0x00000002)
  #define DUNIT0_CR_D_CR_MISC0_DTMR_RST_MIN                            (0)
  #define DUNIT0_CR_D_CR_MISC0_DTMR_RST_MAX                            (1) // 0x00000001
  #define DUNIT0_CR_D_CR_MISC0_DTMR_RST_DEF                            (0x00000000)
  #define DUNIT0_CR_D_CR_MISC0_DTMR_RST_HSH                            (0x0102B0B4)

  #define DUNIT0_CR_D_CR_MISC0_DWRBUF_RST_OFF                          ( 2)
  #define DUNIT0_CR_D_CR_MISC0_DWRBUF_RST_WID                          ( 1)
  #define DUNIT0_CR_D_CR_MISC0_DWRBUF_RST_MSK                          (0x00000004)
  #define DUNIT0_CR_D_CR_MISC0_DWRBUF_RST_MIN                          (0)
  #define DUNIT0_CR_D_CR_MISC0_DWRBUF_RST_MAX                          (1) // 0x00000001
  #define DUNIT0_CR_D_CR_MISC0_DWRBUF_RST_DEF                          (0x00000000)
  #define DUNIT0_CR_D_CR_MISC0_DWRBUF_RST_HSH                          (0x0104B0B4)

  #define DUNIT0_CR_D_CR_MISC0_DRDBUF_RST_OFF                          ( 3)
  #define DUNIT0_CR_D_CR_MISC0_DRDBUF_RST_WID                          ( 1)
  #define DUNIT0_CR_D_CR_MISC0_DRDBUF_RST_MSK                          (0x00000008)
  #define DUNIT0_CR_D_CR_MISC0_DRDBUF_RST_MIN                          (0)
  #define DUNIT0_CR_D_CR_MISC0_DRDBUF_RST_MAX                          (1) // 0x00000001
  #define DUNIT0_CR_D_CR_MISC0_DRDBUF_RST_DEF                          (0x00000000)
  #define DUNIT0_CR_D_CR_MISC0_DRDBUF_RST_HSH                          (0x0106B0B4)

  #define DUNIT0_CR_D_CR_MISC0_RSVD4_OFF                               ( 4)
  #define DUNIT0_CR_D_CR_MISC0_RSVD4_WID                               ( 1)
  #define DUNIT0_CR_D_CR_MISC0_RSVD4_MSK                               (0x00000010)
  #define DUNIT0_CR_D_CR_MISC0_RSVD4_MIN                               (0)
  #define DUNIT0_CR_D_CR_MISC0_RSVD4_MAX                               (1) // 0x00000001
  #define DUNIT0_CR_D_CR_MISC0_RSVD4_DEF                               (0x00000000)
  #define DUNIT0_CR_D_CR_MISC0_RSVD4_HSH                               (0x0108B0B4)

  #define DUNIT0_CR_D_CR_MISC0_MTG_RST_OFF                             ( 5)
  #define DUNIT0_CR_D_CR_MISC0_MTG_RST_WID                             ( 1)
  #define DUNIT0_CR_D_CR_MISC0_MTG_RST_MSK                             (0x00000020)
  #define DUNIT0_CR_D_CR_MISC0_MTG_RST_MIN                             (0)
  #define DUNIT0_CR_D_CR_MISC0_MTG_RST_MAX                             (1) // 0x00000001
  #define DUNIT0_CR_D_CR_MISC0_MTG_RST_DEF                             (0x00000000)
  #define DUNIT0_CR_D_CR_MISC0_MTG_RST_HSH                             (0x010AB0B4)

  #define DUNIT0_CR_D_CR_MISC0_RSVD7_6_OFF                             ( 6)
  #define DUNIT0_CR_D_CR_MISC0_RSVD7_6_WID                             ( 2)
  #define DUNIT0_CR_D_CR_MISC0_RSVD7_6_MSK                             (0x000000C0)
  #define DUNIT0_CR_D_CR_MISC0_RSVD7_6_MIN                             (0)
  #define DUNIT0_CR_D_CR_MISC0_RSVD7_6_MAX                             (3) // 0x00000003
  #define DUNIT0_CR_D_CR_MISC0_RSVD7_6_DEF                             (0x00000000)
  #define DUNIT0_CR_D_CR_MISC0_RSVD7_6_HSH                             (0x020CB0B4)

  #define DUNIT0_CR_D_CR_MISC0_CLR_BANK_STATUS_OFF                     ( 8)
  #define DUNIT0_CR_D_CR_MISC0_CLR_BANK_STATUS_WID                     ( 1)
  #define DUNIT0_CR_D_CR_MISC0_CLR_BANK_STATUS_MSK                     (0x00000100)
  #define DUNIT0_CR_D_CR_MISC0_CLR_BANK_STATUS_MIN                     (0)
  #define DUNIT0_CR_D_CR_MISC0_CLR_BANK_STATUS_MAX                     (1) // 0x00000001
  #define DUNIT0_CR_D_CR_MISC0_CLR_BANK_STATUS_DEF                     (0x00000000)
  #define DUNIT0_CR_D_CR_MISC0_CLR_BANK_STATUS_HSH                     (0x0190B0B4)

  #define DUNIT0_CR_D_CR_MISC0_CLR_MRR_DATA_OFF                        ( 9)
  #define DUNIT0_CR_D_CR_MISC0_CLR_MRR_DATA_WID                        ( 1)
  #define DUNIT0_CR_D_CR_MISC0_CLR_MRR_DATA_MSK                        (0x00000200)
  #define DUNIT0_CR_D_CR_MISC0_CLR_MRR_DATA_MIN                        (0)
  #define DUNIT0_CR_D_CR_MISC0_CLR_MRR_DATA_MAX                        (1) // 0x00000001
  #define DUNIT0_CR_D_CR_MISC0_CLR_MRR_DATA_DEF                        (0x00000000)
  #define DUNIT0_CR_D_CR_MISC0_CLR_MRR_DATA_HSH                        (0x0192B0B4)

  #define DUNIT0_CR_D_CR_MISC0_RSVD10_OFF                              (10)
  #define DUNIT0_CR_D_CR_MISC0_RSVD10_WID                              ( 1)
  #define DUNIT0_CR_D_CR_MISC0_RSVD10_MSK                              (0x00000400)
  #define DUNIT0_CR_D_CR_MISC0_RSVD10_MIN                              (0)
  #define DUNIT0_CR_D_CR_MISC0_RSVD10_MAX                              (1) // 0x00000001
  #define DUNIT0_CR_D_CR_MISC0_RSVD10_DEF                              (0x00000000)
  #define DUNIT0_CR_D_CR_MISC0_RSVD10_HSH                              (0x0114B0B4)

  #define DUNIT0_CR_D_CR_MISC0_CLR_TRAIN_FB_DATA_OFF                   (11)
  #define DUNIT0_CR_D_CR_MISC0_CLR_TRAIN_FB_DATA_WID                   ( 1)
  #define DUNIT0_CR_D_CR_MISC0_CLR_TRAIN_FB_DATA_MSK                   (0x00000800)
  #define DUNIT0_CR_D_CR_MISC0_CLR_TRAIN_FB_DATA_MIN                   (0)
  #define DUNIT0_CR_D_CR_MISC0_CLR_TRAIN_FB_DATA_MAX                   (1) // 0x00000001
  #define DUNIT0_CR_D_CR_MISC0_CLR_TRAIN_FB_DATA_DEF                   (0x00000000)
  #define DUNIT0_CR_D_CR_MISC0_CLR_TRAIN_FB_DATA_HSH                   (0x0196B0B4)

  #define DUNIT0_CR_D_CR_MISC0_RSVD15_12_OFF                           (12)
  #define DUNIT0_CR_D_CR_MISC0_RSVD15_12_WID                           ( 4)
  #define DUNIT0_CR_D_CR_MISC0_RSVD15_12_MSK                           (0x0000F000)
  #define DUNIT0_CR_D_CR_MISC0_RSVD15_12_MIN                           (0)
  #define DUNIT0_CR_D_CR_MISC0_RSVD15_12_MAX                           (15) // 0x0000000F
  #define DUNIT0_CR_D_CR_MISC0_RSVD15_12_DEF                           (0x00000000)
  #define DUNIT0_CR_D_CR_MISC0_RSVD15_12_HSH                           (0x0418B0B4)

  #define DUNIT0_CR_D_CR_MISC0_IGNORE_PHYUPD_REQ_OFF                   (16)
  #define DUNIT0_CR_D_CR_MISC0_IGNORE_PHYUPD_REQ_WID                   ( 1)
  #define DUNIT0_CR_D_CR_MISC0_IGNORE_PHYUPD_REQ_MSK                   (0x00010000)
  #define DUNIT0_CR_D_CR_MISC0_IGNORE_PHYUPD_REQ_MIN                   (0)
  #define DUNIT0_CR_D_CR_MISC0_IGNORE_PHYUPD_REQ_MAX                   (1) // 0x00000001
  #define DUNIT0_CR_D_CR_MISC0_IGNORE_PHYUPD_REQ_DEF                   (0x00000000)
  #define DUNIT0_CR_D_CR_MISC0_IGNORE_PHYUPD_REQ_HSH                   (0x0120B0B4)

  #define DUNIT0_CR_D_CR_MISC0_RSVD18_17_OFF                           (17)
  #define DUNIT0_CR_D_CR_MISC0_RSVD18_17_WID                           ( 2)
  #define DUNIT0_CR_D_CR_MISC0_RSVD18_17_MSK                           (0x00060000)
  #define DUNIT0_CR_D_CR_MISC0_RSVD18_17_MIN                           (0)
  #define DUNIT0_CR_D_CR_MISC0_RSVD18_17_MAX                           (3) // 0x00000003
  #define DUNIT0_CR_D_CR_MISC0_RSVD18_17_DEF                           (0x00000000)
  #define DUNIT0_CR_D_CR_MISC0_RSVD18_17_HSH                           (0x0222B0B4)

  #define DUNIT0_CR_D_CR_MISC0_DUNIT2DRAM_PROP_DLY_OFF                 (19)
  #define DUNIT0_CR_D_CR_MISC0_DUNIT2DRAM_PROP_DLY_WID                 ( 8)
  #define DUNIT0_CR_D_CR_MISC0_DUNIT2DRAM_PROP_DLY_MSK                 (0x07F80000)
  #define DUNIT0_CR_D_CR_MISC0_DUNIT2DRAM_PROP_DLY_MIN                 (0)
  #define DUNIT0_CR_D_CR_MISC0_DUNIT2DRAM_PROP_DLY_MAX                 (255) // 0x000000FF
  #define DUNIT0_CR_D_CR_MISC0_DUNIT2DRAM_PROP_DLY_DEF                 (0x00000020)
  #define DUNIT0_CR_D_CR_MISC0_DUNIT2DRAM_PROP_DLY_HSH                 (0x0826B0B4)

  #define DUNIT0_CR_D_CR_MISC0_RSVD28_27_OFF                           (27)
  #define DUNIT0_CR_D_CR_MISC0_RSVD28_27_WID                           ( 2)
  #define DUNIT0_CR_D_CR_MISC0_RSVD28_27_MSK                           (0x18000000)
  #define DUNIT0_CR_D_CR_MISC0_RSVD28_27_MIN                           (0)
  #define DUNIT0_CR_D_CR_MISC0_RSVD28_27_MAX                           (3) // 0x00000003
  #define DUNIT0_CR_D_CR_MISC0_RSVD28_27_DEF                           (0x00000000)
  #define DUNIT0_CR_D_CR_MISC0_RSVD28_27_HSH                           (0x0236B0B4)

  #define DUNIT0_CR_D_CR_MISC0_EVEN_PHASE_CMD_OFF                      (29)
  #define DUNIT0_CR_D_CR_MISC0_EVEN_PHASE_CMD_WID                      ( 1)
  #define DUNIT0_CR_D_CR_MISC0_EVEN_PHASE_CMD_MSK                      (0x20000000)
  #define DUNIT0_CR_D_CR_MISC0_EVEN_PHASE_CMD_MIN                      (0)
  #define DUNIT0_CR_D_CR_MISC0_EVEN_PHASE_CMD_MAX                      (1) // 0x00000001
  #define DUNIT0_CR_D_CR_MISC0_EVEN_PHASE_CMD_DEF                      (0x00000000)
  #define DUNIT0_CR_D_CR_MISC0_EVEN_PHASE_CMD_HSH                      (0x013AB0B4)

  #define DUNIT0_CR_D_CR_MISC0_RSVD30_OFF                              (30)
  #define DUNIT0_CR_D_CR_MISC0_RSVD30_WID                              ( 1)
  #define DUNIT0_CR_D_CR_MISC0_RSVD30_MSK                              (0x40000000)
  #define DUNIT0_CR_D_CR_MISC0_RSVD30_MIN                              (0)
  #define DUNIT0_CR_D_CR_MISC0_RSVD30_MAX                              (1) // 0x00000001
  #define DUNIT0_CR_D_CR_MISC0_RSVD30_DEF                              (0x00000000)
  #define DUNIT0_CR_D_CR_MISC0_RSVD30_HSH                              (0x013CB0B4)

  #define DUNIT0_CR_D_CR_MISC0_BG_SWAP_DIS_OFF                         (31)
  #define DUNIT0_CR_D_CR_MISC0_BG_SWAP_DIS_WID                         ( 1)
  #define DUNIT0_CR_D_CR_MISC0_BG_SWAP_DIS_MSK                         (0x80000000)
  #define DUNIT0_CR_D_CR_MISC0_BG_SWAP_DIS_MIN                         (0)
  #define DUNIT0_CR_D_CR_MISC0_BG_SWAP_DIS_MAX                         (1) // 0x00000001
  #define DUNIT0_CR_D_CR_MISC0_BG_SWAP_DIS_DEF                         (0x00000000)
  #define DUNIT0_CR_D_CR_MISC0_BG_SWAP_DIS_HSH                         (0x013EB0B4)

#define DUNIT0_CR_D_CR_BONUS0_REG                                      (0x0000B0C0)
#define DUNIT0_CR_D_CR_BONUS0_DEF                                      (0x00000000)
#define DUNIT0_CR_D_CR_BONUS0_VOLATILE_BITFIELDS_MSK                   (0x00000000)

  #define DUNIT0_CR_D_CR_BONUS0_BONUS0_OFF                             ( 0)
  #define DUNIT0_CR_D_CR_BONUS0_BONUS0_WID                             (32)
  #define DUNIT0_CR_D_CR_BONUS0_BONUS0_MSK                             (0xFFFFFFFF)
  #define DUNIT0_CR_D_CR_BONUS0_BONUS0_MIN                             (0)
  #define DUNIT0_CR_D_CR_BONUS0_BONUS0_MAX                             (4294967295) // 0xFFFFFFFF
  #define DUNIT0_CR_D_CR_BONUS0_BONUS0_DEF                             (0x00000000)
  #define DUNIT0_CR_D_CR_BONUS0_BONUS0_HSH                             (0x2000B0C0)

#define DUNIT0_CR_D_CR_BONUS1_REG                                      (0x0000B0C4)
#define DUNIT0_CR_D_CR_BONUS1_DEF                                      (0x00000000)
#define DUNIT0_CR_D_CR_BONUS1_VOLATILE_BITFIELDS_MSK                   (0x00000000)

  #define DUNIT0_CR_D_CR_BONUS1_BONUS1_OFF                             ( 0)
  #define DUNIT0_CR_D_CR_BONUS1_BONUS1_WID                             (32)
  #define DUNIT0_CR_D_CR_BONUS1_BONUS1_MSK                             (0xFFFFFFFF)
  #define DUNIT0_CR_D_CR_BONUS1_BONUS1_MIN                             (0)
  #define DUNIT0_CR_D_CR_BONUS1_BONUS1_MAX                             (4294967295) // 0xFFFFFFFF
  #define DUNIT0_CR_D_CR_BONUS1_BONUS1_DEF                             (0x00000000)
  #define DUNIT0_CR_D_CR_BONUS1_BONUS1_HSH                             (0x2000B0C4)

#define DUNIT0_CR_D_CR_DGB_REG                                         (0x0000B0C8)
#define DUNIT0_CR_D_CR_DGB_DEF                                         (0x00000012)
#define DUNIT0_CR_D_CR_DGB_VOLATILE_BITFIELDS_MSK                      (0x00000000)

  #define DUNIT0_CR_D_CR_DGB_DGB_DPHASE_SEL_OFF                        ( 0)
  #define DUNIT0_CR_D_CR_DGB_DGB_DPHASE_SEL_WID                        ( 2)
  #define DUNIT0_CR_D_CR_DGB_DGB_DPHASE_SEL_MSK                        (0x00000003)
  #define DUNIT0_CR_D_CR_DGB_DGB_DPHASE_SEL_MIN                        (0)
  #define DUNIT0_CR_D_CR_DGB_DGB_DPHASE_SEL_MAX                        (3) // 0x00000003
  #define DUNIT0_CR_D_CR_DGB_DGB_DPHASE_SEL_DEF                        (0x00000002)
  #define DUNIT0_CR_D_CR_DGB_DGB_DPHASE_SEL_HSH                        (0x0200B0C8)

  #define DUNIT0_CR_D_CR_DGB_DGB_D2XCLK_MODE_OFF                       ( 2)
  #define DUNIT0_CR_D_CR_DGB_DGB_D2XCLK_MODE_WID                       ( 1)
  #define DUNIT0_CR_D_CR_DGB_DGB_D2XCLK_MODE_MSK                       (0x00000004)
  #define DUNIT0_CR_D_CR_DGB_DGB_D2XCLK_MODE_MIN                       (0)
  #define DUNIT0_CR_D_CR_DGB_DGB_D2XCLK_MODE_MAX                       (1) // 0x00000001
  #define DUNIT0_CR_D_CR_DGB_DGB_D2XCLK_MODE_DEF                       (0x00000000)
  #define DUNIT0_CR_D_CR_DGB_DGB_D2XCLK_MODE_HSH                       (0x0104B0C8)

  #define DUNIT0_CR_D_CR_DGB_DGB_TX_1XCLK_ALIGN_OFF                    ( 3)
  #define DUNIT0_CR_D_CR_DGB_DGB_TX_1XCLK_ALIGN_WID                    ( 1)
  #define DUNIT0_CR_D_CR_DGB_DGB_TX_1XCLK_ALIGN_MSK                    (0x00000008)
  #define DUNIT0_CR_D_CR_DGB_DGB_TX_1XCLK_ALIGN_MIN                    (0)
  #define DUNIT0_CR_D_CR_DGB_DGB_TX_1XCLK_ALIGN_MAX                    (1) // 0x00000001
  #define DUNIT0_CR_D_CR_DGB_DGB_TX_1XCLK_ALIGN_DEF                    (0x00000000)
  #define DUNIT0_CR_D_CR_DGB_DGB_TX_1XCLK_ALIGN_HSH                    (0x0106B0C8)

  #define DUNIT0_CR_D_CR_DGB_DGB_PPHASE_SEL_OFF                        ( 4)
  #define DUNIT0_CR_D_CR_DGB_DGB_PPHASE_SEL_WID                        ( 2)
  #define DUNIT0_CR_D_CR_DGB_DGB_PPHASE_SEL_MSK                        (0x00000030)
  #define DUNIT0_CR_D_CR_DGB_DGB_PPHASE_SEL_MIN                        (0)
  #define DUNIT0_CR_D_CR_DGB_DGB_PPHASE_SEL_MAX                        (3) // 0x00000003
  #define DUNIT0_CR_D_CR_DGB_DGB_PPHASE_SEL_DEF                        (0x00000001)
  #define DUNIT0_CR_D_CR_DGB_DGB_PPHASE_SEL_HSH                        (0x0208B0C8)

  #define DUNIT0_CR_D_CR_DGB_DGB_P2XCLK_MODE_OFF                       ( 6)
  #define DUNIT0_CR_D_CR_DGB_DGB_P2XCLK_MODE_WID                       ( 1)
  #define DUNIT0_CR_D_CR_DGB_DGB_P2XCLK_MODE_MSK                       (0x00000040)
  #define DUNIT0_CR_D_CR_DGB_DGB_P2XCLK_MODE_MIN                       (0)
  #define DUNIT0_CR_D_CR_DGB_DGB_P2XCLK_MODE_MAX                       (1) // 0x00000001
  #define DUNIT0_CR_D_CR_DGB_DGB_P2XCLK_MODE_DEF                       (0x00000000)
  #define DUNIT0_CR_D_CR_DGB_DGB_P2XCLK_MODE_HSH                       (0x010CB0C8)

  #define DUNIT0_CR_D_CR_DGB_DGB_RX_1XCLK_ALIGN_OFF                    ( 7)
  #define DUNIT0_CR_D_CR_DGB_DGB_RX_1XCLK_ALIGN_WID                    ( 1)
  #define DUNIT0_CR_D_CR_DGB_DGB_RX_1XCLK_ALIGN_MSK                    (0x00000080)
  #define DUNIT0_CR_D_CR_DGB_DGB_RX_1XCLK_ALIGN_MIN                    (0)
  #define DUNIT0_CR_D_CR_DGB_DGB_RX_1XCLK_ALIGN_MAX                    (1) // 0x00000001
  #define DUNIT0_CR_D_CR_DGB_DGB_RX_1XCLK_ALIGN_DEF                    (0x00000000)
  #define DUNIT0_CR_D_CR_DGB_DGB_RX_1XCLK_ALIGN_HSH                    (0x010EB0C8)

  #define DUNIT0_CR_D_CR_DGB_DGB_TX_WPTR_INIT_VALUE_OFF                ( 8)
  #define DUNIT0_CR_D_CR_DGB_DGB_TX_WPTR_INIT_VALUE_WID                ( 4)
  #define DUNIT0_CR_D_CR_DGB_DGB_TX_WPTR_INIT_VALUE_MSK                (0x00000F00)
  #define DUNIT0_CR_D_CR_DGB_DGB_TX_WPTR_INIT_VALUE_MIN                (0)
  #define DUNIT0_CR_D_CR_DGB_DGB_TX_WPTR_INIT_VALUE_MAX                (15) // 0x0000000F
  #define DUNIT0_CR_D_CR_DGB_DGB_TX_WPTR_INIT_VALUE_DEF                (0x00000000)
  #define DUNIT0_CR_D_CR_DGB_DGB_TX_WPTR_INIT_VALUE_HSH                (0x0410B0C8)

  #define DUNIT0_CR_D_CR_DGB_RSVD12_OFF                                (12)
  #define DUNIT0_CR_D_CR_DGB_RSVD12_WID                                ( 1)
  #define DUNIT0_CR_D_CR_DGB_RSVD12_MSK                                (0x00001000)
  #define DUNIT0_CR_D_CR_DGB_RSVD12_MIN                                (0)
  #define DUNIT0_CR_D_CR_DGB_RSVD12_MAX                                (1) // 0x00000001
  #define DUNIT0_CR_D_CR_DGB_RSVD12_DEF                                (0x00000000)
  #define DUNIT0_CR_D_CR_DGB_RSVD12_HSH                                (0x0118B0C8)

  #define DUNIT0_CR_D_CR_DGB_DGB_TX_RPTR_INIT_VALUE_OFF                (13)
  #define DUNIT0_CR_D_CR_DGB_DGB_TX_RPTR_INIT_VALUE_WID                ( 4)
  #define DUNIT0_CR_D_CR_DGB_DGB_TX_RPTR_INIT_VALUE_MSK                (0x0001E000)
  #define DUNIT0_CR_D_CR_DGB_DGB_TX_RPTR_INIT_VALUE_MIN                (0)
  #define DUNIT0_CR_D_CR_DGB_DGB_TX_RPTR_INIT_VALUE_MAX                (15) // 0x0000000F
  #define DUNIT0_CR_D_CR_DGB_DGB_TX_RPTR_INIT_VALUE_DEF                (0x00000000)
  #define DUNIT0_CR_D_CR_DGB_DGB_TX_RPTR_INIT_VALUE_HSH                (0x041AB0C8)

  #define DUNIT0_CR_D_CR_DGB_RSVD17_OFF                                (17)
  #define DUNIT0_CR_D_CR_DGB_RSVD17_WID                                ( 1)
  #define DUNIT0_CR_D_CR_DGB_RSVD17_MSK                                (0x00020000)
  #define DUNIT0_CR_D_CR_DGB_RSVD17_MIN                                (0)
  #define DUNIT0_CR_D_CR_DGB_RSVD17_MAX                                (1) // 0x00000001
  #define DUNIT0_CR_D_CR_DGB_RSVD17_DEF                                (0x00000000)
  #define DUNIT0_CR_D_CR_DGB_RSVD17_HSH                                (0x0122B0C8)

  #define DUNIT0_CR_D_CR_DGB_DGB_RX_WPTR_INIT_VALUE_OFF                (18)
  #define DUNIT0_CR_D_CR_DGB_DGB_RX_WPTR_INIT_VALUE_WID                ( 4)
  #define DUNIT0_CR_D_CR_DGB_DGB_RX_WPTR_INIT_VALUE_MSK                (0x003C0000)
  #define DUNIT0_CR_D_CR_DGB_DGB_RX_WPTR_INIT_VALUE_MIN                (0)
  #define DUNIT0_CR_D_CR_DGB_DGB_RX_WPTR_INIT_VALUE_MAX                (15) // 0x0000000F
  #define DUNIT0_CR_D_CR_DGB_DGB_RX_WPTR_INIT_VALUE_DEF                (0x00000000)
  #define DUNIT0_CR_D_CR_DGB_DGB_RX_WPTR_INIT_VALUE_HSH                (0x0424B0C8)

  #define DUNIT0_CR_D_CR_DGB_RSVD22_OFF                                (22)
  #define DUNIT0_CR_D_CR_DGB_RSVD22_WID                                ( 1)
  #define DUNIT0_CR_D_CR_DGB_RSVD22_MSK                                (0x00400000)
  #define DUNIT0_CR_D_CR_DGB_RSVD22_MIN                                (0)
  #define DUNIT0_CR_D_CR_DGB_RSVD22_MAX                                (1) // 0x00000001
  #define DUNIT0_CR_D_CR_DGB_RSVD22_DEF                                (0x00000000)
  #define DUNIT0_CR_D_CR_DGB_RSVD22_HSH                                (0x012CB0C8)

  #define DUNIT0_CR_D_CR_DGB_DGB_RX_RPTR_INIT_VALUE_OFF                (23)
  #define DUNIT0_CR_D_CR_DGB_DGB_RX_RPTR_INIT_VALUE_WID                ( 4)
  #define DUNIT0_CR_D_CR_DGB_DGB_RX_RPTR_INIT_VALUE_MSK                (0x07800000)
  #define DUNIT0_CR_D_CR_DGB_DGB_RX_RPTR_INIT_VALUE_MIN                (0)
  #define DUNIT0_CR_D_CR_DGB_DGB_RX_RPTR_INIT_VALUE_MAX                (15) // 0x0000000F
  #define DUNIT0_CR_D_CR_DGB_DGB_RX_RPTR_INIT_VALUE_DEF                (0x00000000)
  #define DUNIT0_CR_D_CR_DGB_DGB_RX_RPTR_INIT_VALUE_HSH                (0x042EB0C8)

  #define DUNIT0_CR_D_CR_DGB_RSVD30_27_OFF                             (27)
  #define DUNIT0_CR_D_CR_DGB_RSVD30_27_WID                             ( 4)
  #define DUNIT0_CR_D_CR_DGB_RSVD30_27_MSK                             (0x78000000)
  #define DUNIT0_CR_D_CR_DGB_RSVD30_27_MIN                             (0)
  #define DUNIT0_CR_D_CR_DGB_RSVD30_27_MAX                             (15) // 0x0000000F
  #define DUNIT0_CR_D_CR_DGB_RSVD30_27_DEF                             (0x00000000)
  #define DUNIT0_CR_D_CR_DGB_RSVD30_27_HSH                             (0x0436B0C8)

  #define DUNIT0_CR_D_CR_DGB_DGB_FIFO_PTR_EN_OFF                       (31)
  #define DUNIT0_CR_D_CR_DGB_DGB_FIFO_PTR_EN_WID                       ( 1)
  #define DUNIT0_CR_D_CR_DGB_DGB_FIFO_PTR_EN_MSK                       (0x80000000)
  #define DUNIT0_CR_D_CR_DGB_DGB_FIFO_PTR_EN_MIN                       (0)
  #define DUNIT0_CR_D_CR_DGB_DGB_FIFO_PTR_EN_MAX                       (1) // 0x00000001
  #define DUNIT0_CR_D_CR_DGB_DGB_FIFO_PTR_EN_DEF                       (0x00000000)
  #define DUNIT0_CR_D_CR_DGB_DGB_FIFO_PTR_EN_HSH                       (0x013EB0C8)

#define DUNIT0_CR_CATG_CTRL0_REG                                       (0x0000B110)
#define DUNIT0_CR_CATG_CTRL0_DEF                                       (0x00000000)
#define DUNIT0_CR_CATG_CTRL0_VOLATILE_BITFIELDS_MSK                    (0x00000001)

  #define DUNIT0_CR_CATG_CTRL0_START_OFF                               ( 0)
  #define DUNIT0_CR_CATG_CTRL0_START_WID                               ( 1)
  #define DUNIT0_CR_CATG_CTRL0_START_MSK                               (0x00000001)
  #define DUNIT0_CR_CATG_CTRL0_START_MIN                               (0)
  #define DUNIT0_CR_CATG_CTRL0_START_MAX                               (1) // 0x00000001
  #define DUNIT0_CR_CATG_CTRL0_START_DEF                               (0x00000000)
  #define DUNIT0_CR_CATG_CTRL0_START_HSH                               (0x0180B110)

  #define DUNIT0_CR_CATG_CTRL0_NONSTOP_OFF                             ( 1)
  #define DUNIT0_CR_CATG_CTRL0_NONSTOP_WID                             ( 1)
  #define DUNIT0_CR_CATG_CTRL0_NONSTOP_MSK                             (0x00000002)
  #define DUNIT0_CR_CATG_CTRL0_NONSTOP_MIN                             (0)
  #define DUNIT0_CR_CATG_CTRL0_NONSTOP_MAX                             (1) // 0x00000001
  #define DUNIT0_CR_CATG_CTRL0_NONSTOP_DEF                             (0x00000000)
  #define DUNIT0_CR_CATG_CTRL0_NONSTOP_HSH                             (0x0102B110)

  #define DUNIT0_CR_CATG_CTRL0_CATG_MODE_OFF                           ( 2)
  #define DUNIT0_CR_CATG_CTRL0_CATG_MODE_WID                           ( 3)
  #define DUNIT0_CR_CATG_CTRL0_CATG_MODE_MSK                           (0x0000001C)
  #define DUNIT0_CR_CATG_CTRL0_CATG_MODE_MIN                           (0)
  #define DUNIT0_CR_CATG_CTRL0_CATG_MODE_MAX                           (7) // 0x00000007
  #define DUNIT0_CR_CATG_CTRL0_CATG_MODE_DEF                           (0x00000000)
  #define DUNIT0_CR_CATG_CTRL0_CATG_MODE_HSH                           (0x0304B110)

  #define DUNIT0_CR_CATG_CTRL0_RSVD7_5_OFF                             ( 5)
  #define DUNIT0_CR_CATG_CTRL0_RSVD7_5_WID                             ( 3)
  #define DUNIT0_CR_CATG_CTRL0_RSVD7_5_MSK                             (0x000000E0)
  #define DUNIT0_CR_CATG_CTRL0_RSVD7_5_MIN                             (0)
  #define DUNIT0_CR_CATG_CTRL0_RSVD7_5_MAX                             (7) // 0x00000007
  #define DUNIT0_CR_CATG_CTRL0_RSVD7_5_DEF                             (0x00000000)
  #define DUNIT0_CR_CATG_CTRL0_RSVD7_5_HSH                             (0x030AB110)

  #define DUNIT0_CR_CATG_CTRL0_SEL_PERIOD_OFF                          ( 8)
  #define DUNIT0_CR_CATG_CTRL0_SEL_PERIOD_WID                          ( 8)
  #define DUNIT0_CR_CATG_CTRL0_SEL_PERIOD_MSK                          (0x0000FF00)
  #define DUNIT0_CR_CATG_CTRL0_SEL_PERIOD_MIN                          (0)
  #define DUNIT0_CR_CATG_CTRL0_SEL_PERIOD_MAX                          (255) // 0x000000FF
  #define DUNIT0_CR_CATG_CTRL0_SEL_PERIOD_DEF                          (0x00000000)
  #define DUNIT0_CR_CATG_CTRL0_SEL_PERIOD_HSH                          (0x0810B110)

  #define DUNIT0_CR_CATG_CTRL0_SEL_CS_OFF                              (16)
  #define DUNIT0_CR_CATG_CTRL0_SEL_CS_WID                              ( 4)
  #define DUNIT0_CR_CATG_CTRL0_SEL_CS_MSK                              (0x000F0000)
  #define DUNIT0_CR_CATG_CTRL0_SEL_CS_MIN                              (0)
  #define DUNIT0_CR_CATG_CTRL0_SEL_CS_MAX                              (15) // 0x0000000F
  #define DUNIT0_CR_CATG_CTRL0_SEL_CS_DEF                              (0x00000000)
  #define DUNIT0_CR_CATG_CTRL0_SEL_CS_HSH                              (0x0420B110)

  #define DUNIT0_CR_CATG_CTRL0_SEL_PHASE_OFF                           (20)
  #define DUNIT0_CR_CATG_CTRL0_SEL_PHASE_WID                           ( 2)
  #define DUNIT0_CR_CATG_CTRL0_SEL_PHASE_MSK                           (0x00300000)
  #define DUNIT0_CR_CATG_CTRL0_SEL_PHASE_MIN                           (0)
  #define DUNIT0_CR_CATG_CTRL0_SEL_PHASE_MAX                           (3) // 0x00000003
  #define DUNIT0_CR_CATG_CTRL0_SEL_PHASE_DEF                           (0x00000000)
  #define DUNIT0_CR_CATG_CTRL0_SEL_PHASE_HSH                           (0x0228B110)

  #define DUNIT0_CR_CATG_CTRL0_RSVD31_22_OFF                           (22)
  #define DUNIT0_CR_CATG_CTRL0_RSVD31_22_WID                           (10)
  #define DUNIT0_CR_CATG_CTRL0_RSVD31_22_MSK                           (0xFFC00000)
  #define DUNIT0_CR_CATG_CTRL0_RSVD31_22_MIN                           (0)
  #define DUNIT0_CR_CATG_CTRL0_RSVD31_22_MAX                           (1023) // 0x000003FF
  #define DUNIT0_CR_CATG_CTRL0_RSVD31_22_DEF                           (0x00000000)
  #define DUNIT0_CR_CATG_CTRL0_RSVD31_22_HSH                           (0x0A2CB110)

#define DUNIT0_CR_CATG_CTRL1_REG                                       (0x0000B114)
#define DUNIT0_CR_CATG_CTRL1_DEF                                       (0x00000000)
#define DUNIT0_CR_CATG_CTRL1_VOLATILE_BITFIELDS_MSK                    (0x00000000)

  #define DUNIT0_CR_CATG_CTRL1_CAPAT_ROTATE_EN_OFF                     ( 0)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_ROTATE_EN_WID                     ( 1)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_ROTATE_EN_MSK                     (0x00000001)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_ROTATE_EN_MIN                     (0)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_ROTATE_EN_MAX                     (1) // 0x00000001
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_ROTATE_EN_DEF                     (0x00000000)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_ROTATE_EN_HSH                     (0x0100B114)

  #define DUNIT0_CR_CATG_CTRL1_RST_CAPAT_ON_ROTATE_OFF                 ( 1)
  #define DUNIT0_CR_CATG_CTRL1_RST_CAPAT_ON_ROTATE_WID                 ( 1)
  #define DUNIT0_CR_CATG_CTRL1_RST_CAPAT_ON_ROTATE_MSK                 (0x00000002)
  #define DUNIT0_CR_CATG_CTRL1_RST_CAPAT_ON_ROTATE_MIN                 (0)
  #define DUNIT0_CR_CATG_CTRL1_RST_CAPAT_ON_ROTATE_MAX                 (1) // 0x00000001
  #define DUNIT0_CR_CATG_CTRL1_RST_CAPAT_ON_ROTATE_DEF                 (0x00000000)
  #define DUNIT0_CR_CATG_CTRL1_RST_CAPAT_ON_ROTATE_HSH                 (0x0102B114)

  #define DUNIT0_CR_CATG_CTRL1_VICTIM_CAPAT_SEL_OFF                    ( 2)
  #define DUNIT0_CR_CATG_CTRL1_VICTIM_CAPAT_SEL_WID                    ( 1)
  #define DUNIT0_CR_CATG_CTRL1_VICTIM_CAPAT_SEL_MSK                    (0x00000004)
  #define DUNIT0_CR_CATG_CTRL1_VICTIM_CAPAT_SEL_MIN                    (0)
  #define DUNIT0_CR_CATG_CTRL1_VICTIM_CAPAT_SEL_MAX                    (1) // 0x00000001
  #define DUNIT0_CR_CATG_CTRL1_VICTIM_CAPAT_SEL_DEF                    (0x00000000)
  #define DUNIT0_CR_CATG_CTRL1_VICTIM_CAPAT_SEL_HSH                    (0x0104B114)

  #define DUNIT0_CR_CATG_CTRL1_AGGRESSOR_CAPAT_SEL_OFF                 ( 3)
  #define DUNIT0_CR_CATG_CTRL1_AGGRESSOR_CAPAT_SEL_WID                 ( 1)
  #define DUNIT0_CR_CATG_CTRL1_AGGRESSOR_CAPAT_SEL_MSK                 (0x00000008)
  #define DUNIT0_CR_CATG_CTRL1_AGGRESSOR_CAPAT_SEL_MIN                 (0)
  #define DUNIT0_CR_CATG_CTRL1_AGGRESSOR_CAPAT_SEL_MAX                 (1) // 0x00000001
  #define DUNIT0_CR_CATG_CTRL1_AGGRESSOR_CAPAT_SEL_DEF                 (0x00000000)
  #define DUNIT0_CR_CATG_CTRL1_AGGRESSOR_CAPAT_SEL_HSH                 (0x0106B114)

  #define DUNIT0_CR_CATG_CTRL1_CAPAT_ROTATE_RATE_OFF                   ( 4)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_ROTATE_RATE_WID                   (16)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_ROTATE_RATE_MSK                   (0x000FFFF0)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_ROTATE_RATE_MIN                   (0)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_ROTATE_RATE_MAX                   (65535) // 0x0000FFFF
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_ROTATE_RATE_DEF                   (0x00000000)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_ROTATE_RATE_HSH                   (0x1008B114)

  #define DUNIT0_CR_CATG_CTRL1_RSVD23_20_OFF                           (20)
  #define DUNIT0_CR_CATG_CTRL1_RSVD23_20_WID                           ( 4)
  #define DUNIT0_CR_CATG_CTRL1_RSVD23_20_MSK                           (0x00F00000)
  #define DUNIT0_CR_CATG_CTRL1_RSVD23_20_MIN                           (0)
  #define DUNIT0_CR_CATG_CTRL1_RSVD23_20_MAX                           (15) // 0x0000000F
  #define DUNIT0_CR_CATG_CTRL1_RSVD23_20_DEF                           (0x00000000)
  #define DUNIT0_CR_CATG_CTRL1_RSVD23_20_HSH                           (0x0428B114)

  #define DUNIT0_CR_CATG_CTRL1_CAPAT_PARITY_EN_OFF                     (24)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_PARITY_EN_WID                     ( 1)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_PARITY_EN_MSK                     (0x01000000)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_PARITY_EN_MIN                     (0)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_PARITY_EN_MAX                     (1) // 0x00000001
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_PARITY_EN_DEF                     (0x00000000)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_PARITY_EN_HSH                     (0x0130B114)

  #define DUNIT0_CR_CATG_CTRL1_CAPAT_PARITY_TYPE_OFF                   (25)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_PARITY_TYPE_WID                   ( 1)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_PARITY_TYPE_MSK                   (0x02000000)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_PARITY_TYPE_MIN                   (0)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_PARITY_TYPE_MAX                   (1) // 0x00000001
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_PARITY_TYPE_DEF                   (0x00000000)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_PARITY_TYPE_HSH                   (0x0132B114)

  #define DUNIT0_CR_CATG_CTRL1_CALC_PARITY_WITH_CA13_OFF               (26)
  #define DUNIT0_CR_CATG_CTRL1_CALC_PARITY_WITH_CA13_WID               ( 1)
  #define DUNIT0_CR_CATG_CTRL1_CALC_PARITY_WITH_CA13_MSK               (0x04000000)
  #define DUNIT0_CR_CATG_CTRL1_CALC_PARITY_WITH_CA13_MIN               (0)
  #define DUNIT0_CR_CATG_CTRL1_CALC_PARITY_WITH_CA13_MAX               (1) // 0x00000001
  #define DUNIT0_CR_CATG_CTRL1_CALC_PARITY_WITH_CA13_DEF               (0x00000000)
  #define DUNIT0_CR_CATG_CTRL1_CALC_PARITY_WITH_CA13_HSH               (0x0134B114)

  #define DUNIT0_CR_CATG_CTRL1_CAPAT_PARITY_PIN_OFF                    (27)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_PARITY_PIN_WID                    ( 3)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_PARITY_PIN_MSK                    (0x38000000)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_PARITY_PIN_MIN                    (0)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_PARITY_PIN_MAX                    (7) // 0x00000007
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_PARITY_PIN_DEF                    (0x00000000)
  #define DUNIT0_CR_CATG_CTRL1_CAPAT_PARITY_PIN_HSH                    (0x0336B114)

  #define DUNIT0_CR_CATG_CTRL1_RSVD31_30_OFF                           (30)
  #define DUNIT0_CR_CATG_CTRL1_RSVD31_30_WID                           ( 2)
  #define DUNIT0_CR_CATG_CTRL1_RSVD31_30_MSK                           (0xC0000000)
  #define DUNIT0_CR_CATG_CTRL1_RSVD31_30_MIN                           (0)
  #define DUNIT0_CR_CATG_CTRL1_RSVD31_30_MAX                           (3) // 0x00000003
  #define DUNIT0_CR_CATG_CTRL1_RSVD31_30_DEF                           (0x00000000)
  #define DUNIT0_CR_CATG_CTRL1_RSVD31_30_HSH                           (0x023CB114)

#define DUNIT0_CR_CATG_CTRL2_REG                                       (0x0000B118)
#define DUNIT0_CR_CATG_CTRL2_DEF                                       (0x00000000)
#define DUNIT0_CR_CATG_CTRL2_VOLATILE_BITFIELDS_MSK                    (0x00000000)

  #define DUNIT0_CR_CATG_CTRL2_CATG_RUNTIME_OFF                        ( 0)
  #define DUNIT0_CR_CATG_CTRL2_CATG_RUNTIME_WID                        (32)
  #define DUNIT0_CR_CATG_CTRL2_CATG_RUNTIME_MSK                        (0xFFFFFFFF)
  #define DUNIT0_CR_CATG_CTRL2_CATG_RUNTIME_MIN                        (0)
  #define DUNIT0_CR_CATG_CTRL2_CATG_RUNTIME_MAX                        (4294967295) // 0xFFFFFFFF
  #define DUNIT0_CR_CATG_CTRL2_CATG_RUNTIME_DEF                        (0x00000000)
  #define DUNIT0_CR_CATG_CTRL2_CATG_RUNTIME_HSH                        (0x2000B118)

#define DUNIT0_CR_CATG_CAPPATSEL_REG                                   (0x0000B11C)
#define DUNIT0_CR_CATG_CAPPATSEL_DEF                                   (0x00000000)
#define DUNIT0_CR_CATG_CAPPATSEL_VOLATILE_BITFIELDS_MSK                (0x00000000)

  #define DUNIT0_CR_CATG_CAPPATSEL_CA_SEL_PPAT_OFF                     ( 0)
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_SEL_PPAT_WID                     (14)
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_SEL_PPAT_MSK                     (0x00003FFF)
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_SEL_PPAT_MIN                     (0)
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_SEL_PPAT_MAX                     (16383) // 0x00003FFF
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_SEL_PPAT_DEF                     (0x00000000)
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_SEL_PPAT_HSH                     (0x0E00B11C)

  #define DUNIT0_CR_CATG_CAPPATSEL_RSVD14_OFF                          (14)
  #define DUNIT0_CR_CATG_CAPPATSEL_RSVD14_WID                          ( 1)
  #define DUNIT0_CR_CATG_CAPPATSEL_RSVD14_MSK                          (0x00004000)
  #define DUNIT0_CR_CATG_CAPPATSEL_RSVD14_MIN                          (0)
  #define DUNIT0_CR_CATG_CAPPATSEL_RSVD14_MAX                          (1) // 0x00000001
  #define DUNIT0_CR_CATG_CAPPATSEL_RSVD14_DEF                          (0x00000000)
  #define DUNIT0_CR_CATG_CAPPATSEL_RSVD14_HSH                          (0x011CB11C)

  #define DUNIT0_CR_CATG_CAPPATSEL_CA_SEL_PPAT_EN_OFF                  (15)
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_SEL_PPAT_EN_WID                  ( 1)
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_SEL_PPAT_EN_MSK                  (0x00008000)
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_SEL_PPAT_EN_MIN                  (0)
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_SEL_PPAT_EN_MAX                  (1) // 0x00000001
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_SEL_PPAT_EN_DEF                  (0x00000000)
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_SEL_PPAT_EN_HSH                  (0x011EB11C)

  #define DUNIT0_CR_CATG_CAPPATSEL_CA_DSEL_PPAT_OFF                    (16)
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_DSEL_PPAT_WID                    (14)
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_DSEL_PPAT_MSK                    (0x3FFF0000)
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_DSEL_PPAT_MIN                    (0)
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_DSEL_PPAT_MAX                    (16383) // 0x00003FFF
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_DSEL_PPAT_DEF                    (0x00000000)
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_DSEL_PPAT_HSH                    (0x0E20B11C)

  #define DUNIT0_CR_CATG_CAPPATSEL_RSVD30_OFF                          (30)
  #define DUNIT0_CR_CATG_CAPPATSEL_RSVD30_WID                          ( 1)
  #define DUNIT0_CR_CATG_CAPPATSEL_RSVD30_MSK                          (0x40000000)
  #define DUNIT0_CR_CATG_CAPPATSEL_RSVD30_MIN                          (0)
  #define DUNIT0_CR_CATG_CAPPATSEL_RSVD30_MAX                          (1) // 0x00000001
  #define DUNIT0_CR_CATG_CAPPATSEL_RSVD30_DEF                          (0x00000000)
  #define DUNIT0_CR_CATG_CAPPATSEL_RSVD30_HSH                          (0x013CB11C)

  #define DUNIT0_CR_CATG_CAPPATSEL_CA_DSEL_PPAT_EN_OFF                 (31)
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_DSEL_PPAT_EN_WID                 ( 1)
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_DSEL_PPAT_EN_MSK                 (0x80000000)
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_DSEL_PPAT_EN_MIN                 (0)
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_DSEL_PPAT_EN_MAX                 (1) // 0x00000001
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_DSEL_PPAT_EN_DEF                 (0x00000000)
  #define DUNIT0_CR_CATG_CAPPATSEL_CA_DSEL_PPAT_EN_HSH                 (0x013EB11C)

#define DUNIT0_CR_CATG_CAPATSEL_REG                                    (0x0000B120)
#define DUNIT0_CR_CATG_CAPATSEL_DEF                                    (0x04543210)
#define DUNIT0_CR_CATG_CAPATSEL_VOLATILE_BITFIELDS_MSK                 (0x00000000)

  #define DUNIT0_CR_CATG_CAPATSEL_CA0_PATSEL_OFF                       ( 0)
  #define DUNIT0_CR_CATG_CAPATSEL_CA0_PATSEL_WID                       ( 3)
  #define DUNIT0_CR_CATG_CAPATSEL_CA0_PATSEL_MSK                       (0x00000007)
  #define DUNIT0_CR_CATG_CAPATSEL_CA0_PATSEL_MIN                       (0)
  #define DUNIT0_CR_CATG_CAPATSEL_CA0_PATSEL_MAX                       (7) // 0x00000007
  #define DUNIT0_CR_CATG_CAPATSEL_CA0_PATSEL_DEF                       (0x00000000)
  #define DUNIT0_CR_CATG_CAPATSEL_CA0_PATSEL_HSH                       (0x0300B120)

  #define DUNIT0_CR_CATG_CAPATSEL_RSVD3_OFF                            ( 3)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD3_WID                            ( 1)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD3_MSK                            (0x00000008)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD3_MIN                            (0)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD3_MAX                            (1) // 0x00000001
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD3_DEF                            (0x00000000)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD3_HSH                            (0x0106B120)

  #define DUNIT0_CR_CATG_CAPATSEL_CA1_PATSEL_OFF                       ( 4)
  #define DUNIT0_CR_CATG_CAPATSEL_CA1_PATSEL_WID                       ( 3)
  #define DUNIT0_CR_CATG_CAPATSEL_CA1_PATSEL_MSK                       (0x00000070)
  #define DUNIT0_CR_CATG_CAPATSEL_CA1_PATSEL_MIN                       (0)
  #define DUNIT0_CR_CATG_CAPATSEL_CA1_PATSEL_MAX                       (7) // 0x00000007
  #define DUNIT0_CR_CATG_CAPATSEL_CA1_PATSEL_DEF                       (0x00000001)
  #define DUNIT0_CR_CATG_CAPATSEL_CA1_PATSEL_HSH                       (0x0308B120)

  #define DUNIT0_CR_CATG_CAPATSEL_RSVD7_OFF                            ( 7)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD7_WID                            ( 1)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD7_MSK                            (0x00000080)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD7_MIN                            (0)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD7_MAX                            (1) // 0x00000001
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD7_DEF                            (0x00000000)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD7_HSH                            (0x010EB120)

  #define DUNIT0_CR_CATG_CAPATSEL_CA2_PATSEL_OFF                       ( 8)
  #define DUNIT0_CR_CATG_CAPATSEL_CA2_PATSEL_WID                       ( 3)
  #define DUNIT0_CR_CATG_CAPATSEL_CA2_PATSEL_MSK                       (0x00000700)
  #define DUNIT0_CR_CATG_CAPATSEL_CA2_PATSEL_MIN                       (0)
  #define DUNIT0_CR_CATG_CAPATSEL_CA2_PATSEL_MAX                       (7) // 0x00000007
  #define DUNIT0_CR_CATG_CAPATSEL_CA2_PATSEL_DEF                       (0x00000002)
  #define DUNIT0_CR_CATG_CAPATSEL_CA2_PATSEL_HSH                       (0x0310B120)

  #define DUNIT0_CR_CATG_CAPATSEL_RSVD11_OFF                           (11)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD11_WID                           ( 1)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD11_MSK                           (0x00000800)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD11_MIN                           (0)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD11_MAX                           (1) // 0x00000001
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD11_DEF                           (0x00000000)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD11_HSH                           (0x0116B120)

  #define DUNIT0_CR_CATG_CAPATSEL_CA3_PATSEL_OFF                       (12)
  #define DUNIT0_CR_CATG_CAPATSEL_CA3_PATSEL_WID                       ( 3)
  #define DUNIT0_CR_CATG_CAPATSEL_CA3_PATSEL_MSK                       (0x00007000)
  #define DUNIT0_CR_CATG_CAPATSEL_CA3_PATSEL_MIN                       (0)
  #define DUNIT0_CR_CATG_CAPATSEL_CA3_PATSEL_MAX                       (7) // 0x00000007
  #define DUNIT0_CR_CATG_CAPATSEL_CA3_PATSEL_DEF                       (0x00000003)
  #define DUNIT0_CR_CATG_CAPATSEL_CA3_PATSEL_HSH                       (0x0318B120)

  #define DUNIT0_CR_CATG_CAPATSEL_RSVD15_OFF                           (15)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD15_WID                           ( 1)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD15_MSK                           (0x00008000)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD15_MIN                           (0)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD15_MAX                           (1) // 0x00000001
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD15_DEF                           (0x00000000)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD15_HSH                           (0x011EB120)

  #define DUNIT0_CR_CATG_CAPATSEL_CA4_PATSEL_OFF                       (16)
  #define DUNIT0_CR_CATG_CAPATSEL_CA4_PATSEL_WID                       ( 3)
  #define DUNIT0_CR_CATG_CAPATSEL_CA4_PATSEL_MSK                       (0x00070000)
  #define DUNIT0_CR_CATG_CAPATSEL_CA4_PATSEL_MIN                       (0)
  #define DUNIT0_CR_CATG_CAPATSEL_CA4_PATSEL_MAX                       (7) // 0x00000007
  #define DUNIT0_CR_CATG_CAPATSEL_CA4_PATSEL_DEF                       (0x00000004)
  #define DUNIT0_CR_CATG_CAPATSEL_CA4_PATSEL_HSH                       (0x0320B120)

  #define DUNIT0_CR_CATG_CAPATSEL_RSVD19_OFF                           (19)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD19_WID                           ( 1)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD19_MSK                           (0x00080000)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD19_MIN                           (0)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD19_MAX                           (1) // 0x00000001
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD19_DEF                           (0x00000000)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD19_HSH                           (0x0126B120)

  #define DUNIT0_CR_CATG_CAPATSEL_CA5_PATSEL_OFF                       (20)
  #define DUNIT0_CR_CATG_CAPATSEL_CA5_PATSEL_WID                       ( 3)
  #define DUNIT0_CR_CATG_CAPATSEL_CA5_PATSEL_MSK                       (0x00700000)
  #define DUNIT0_CR_CATG_CAPATSEL_CA5_PATSEL_MIN                       (0)
  #define DUNIT0_CR_CATG_CAPATSEL_CA5_PATSEL_MAX                       (7) // 0x00000007
  #define DUNIT0_CR_CATG_CAPATSEL_CA5_PATSEL_DEF                       (0x00000005)
  #define DUNIT0_CR_CATG_CAPATSEL_CA5_PATSEL_HSH                       (0x0328B120)

  #define DUNIT0_CR_CATG_CAPATSEL_RSVD23_OFF                           (23)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD23_WID                           ( 1)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD23_MSK                           (0x00800000)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD23_MIN                           (0)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD23_MAX                           (1) // 0x00000001
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD23_DEF                           (0x00000000)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD23_HSH                           (0x012EB120)

  #define DUNIT0_CR_CATG_CAPATSEL_CA6_PATSEL_OFF                       (24)
  #define DUNIT0_CR_CATG_CAPATSEL_CA6_PATSEL_WID                       ( 3)
  #define DUNIT0_CR_CATG_CAPATSEL_CA6_PATSEL_MSK                       (0x07000000)
  #define DUNIT0_CR_CATG_CAPATSEL_CA6_PATSEL_MIN                       (0)
  #define DUNIT0_CR_CATG_CAPATSEL_CA6_PATSEL_MAX                       (7) // 0x00000007
  #define DUNIT0_CR_CATG_CAPATSEL_CA6_PATSEL_DEF                       (0x00000004)
  #define DUNIT0_CR_CATG_CAPATSEL_CA6_PATSEL_HSH                       (0x0330B120)

  #define DUNIT0_CR_CATG_CAPATSEL_RSVD31_27_OFF                        (27)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD31_27_WID                        ( 5)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD31_27_MSK                        (0xF8000000)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD31_27_MIN                        (0)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD31_27_MAX                        (31) // 0x0000001F
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD31_27_DEF                        (0x00000000)
  #define DUNIT0_CR_CATG_CAPATSEL_RSVD31_27_HSH                        (0x0536B120)

#define DUNIT0_CR_CATG_CSPATSEL_REG                                    (0x0000B124)
#define DUNIT0_CR_CATG_CSPATSEL_DEF                                    (0x00003210)
#define DUNIT0_CR_CATG_CSPATSEL_VOLATILE_BITFIELDS_MSK                 (0x00000000)

  #define DUNIT0_CR_CATG_CSPATSEL_CS0_PATSEL_OFF                       ( 0)
  #define DUNIT0_CR_CATG_CSPATSEL_CS0_PATSEL_WID                       ( 3)
  #define DUNIT0_CR_CATG_CSPATSEL_CS0_PATSEL_MSK                       (0x00000007)
  #define DUNIT0_CR_CATG_CSPATSEL_CS0_PATSEL_MIN                       (0)
  #define DUNIT0_CR_CATG_CSPATSEL_CS0_PATSEL_MAX                       (7) // 0x00000007
  #define DUNIT0_CR_CATG_CSPATSEL_CS0_PATSEL_DEF                       (0x00000000)
  #define DUNIT0_CR_CATG_CSPATSEL_CS0_PATSEL_HSH                       (0x0300B124)

  #define DUNIT0_CR_CATG_CSPATSEL_RSVD3_OFF                            ( 3)
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD3_WID                            ( 1)
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD3_MSK                            (0x00000008)
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD3_MIN                            (0)
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD3_MAX                            (1) // 0x00000001
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD3_DEF                            (0x00000000)
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD3_HSH                            (0x0106B124)

  #define DUNIT0_CR_CATG_CSPATSEL_CS1_PATSEL_OFF                       ( 4)
  #define DUNIT0_CR_CATG_CSPATSEL_CS1_PATSEL_WID                       ( 3)
  #define DUNIT0_CR_CATG_CSPATSEL_CS1_PATSEL_MSK                       (0x00000070)
  #define DUNIT0_CR_CATG_CSPATSEL_CS1_PATSEL_MIN                       (0)
  #define DUNIT0_CR_CATG_CSPATSEL_CS1_PATSEL_MAX                       (7) // 0x00000007
  #define DUNIT0_CR_CATG_CSPATSEL_CS1_PATSEL_DEF                       (0x00000001)
  #define DUNIT0_CR_CATG_CSPATSEL_CS1_PATSEL_HSH                       (0x0308B124)

  #define DUNIT0_CR_CATG_CSPATSEL_RSVD7_OFF                            ( 7)
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD7_WID                            ( 1)
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD7_MSK                            (0x00000080)
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD7_MIN                            (0)
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD7_MAX                            (1) // 0x00000001
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD7_DEF                            (0x00000000)
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD7_HSH                            (0x010EB124)

  #define DUNIT0_CR_CATG_CSPATSEL_CS2_PATSEL_OFF                       ( 8)
  #define DUNIT0_CR_CATG_CSPATSEL_CS2_PATSEL_WID                       ( 3)
  #define DUNIT0_CR_CATG_CSPATSEL_CS2_PATSEL_MSK                       (0x00000700)
  #define DUNIT0_CR_CATG_CSPATSEL_CS2_PATSEL_MIN                       (0)
  #define DUNIT0_CR_CATG_CSPATSEL_CS2_PATSEL_MAX                       (7) // 0x00000007
  #define DUNIT0_CR_CATG_CSPATSEL_CS2_PATSEL_DEF                       (0x00000002)
  #define DUNIT0_CR_CATG_CSPATSEL_CS2_PATSEL_HSH                       (0x0310B124)

  #define DUNIT0_CR_CATG_CSPATSEL_RSVD11_OFF                           (11)
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD11_WID                           ( 1)
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD11_MSK                           (0x00000800)
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD11_MIN                           (0)
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD11_MAX                           (1) // 0x00000001
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD11_DEF                           (0x00000000)
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD11_HSH                           (0x0116B124)

  #define DUNIT0_CR_CATG_CSPATSEL_CS3_PATSEL_OFF                       (12)
  #define DUNIT0_CR_CATG_CSPATSEL_CS3_PATSEL_WID                       ( 3)
  #define DUNIT0_CR_CATG_CSPATSEL_CS3_PATSEL_MSK                       (0x00007000)
  #define DUNIT0_CR_CATG_CSPATSEL_CS3_PATSEL_MIN                       (0)
  #define DUNIT0_CR_CATG_CSPATSEL_CS3_PATSEL_MAX                       (7) // 0x00000007
  #define DUNIT0_CR_CATG_CSPATSEL_CS3_PATSEL_DEF                       (0x00000003)
  #define DUNIT0_CR_CATG_CSPATSEL_CS3_PATSEL_HSH                       (0x0318B124)

  #define DUNIT0_CR_CATG_CSPATSEL_RSVD31_15_OFF                        (15)
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD31_15_WID                        (17)
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD31_15_MSK                        (0xFFFF8000)
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD31_15_MIN                        (0)
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD31_15_MAX                        (131071) // 0x0001FFFF
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD31_15_DEF                        (0x00000000)
  #define DUNIT0_CR_CATG_CSPATSEL_RSVD31_15_HSH                        (0x111EB124)

#define DUNIT0_CR_CATG_SSP0_0_REG                                      (0x0000B128)
#define DUNIT0_CR_CATG_SSP0_0_DEF                                      (0x11111111)
#define DUNIT0_CR_CATG_SSP0_0_VOLATILE_BITFIELDS_MSK                   (0x00000000)

  #define DUNIT0_CR_CATG_SSP0_0_SERIAL_PAT0_0_OFF                      ( 0)
  #define DUNIT0_CR_CATG_SSP0_0_SERIAL_PAT0_0_WID                      (32)
  #define DUNIT0_CR_CATG_SSP0_0_SERIAL_PAT0_0_MSK                      (0xFFFFFFFF)
  #define DUNIT0_CR_CATG_SSP0_0_SERIAL_PAT0_0_MIN                      (0)
  #define DUNIT0_CR_CATG_SSP0_0_SERIAL_PAT0_0_MAX                      (4294967295) // 0xFFFFFFFF
  #define DUNIT0_CR_CATG_SSP0_0_SERIAL_PAT0_0_DEF                      (0x11111111)
  #define DUNIT0_CR_CATG_SSP0_0_SERIAL_PAT0_0_HSH                      (0x2000B128)

#define DUNIT0_CR_CATG_SSP0_1_REG                                      (0x0000B12C)
#define DUNIT0_CR_CATG_SSP0_1_DEF                                      (0x11111111)
#define DUNIT0_CR_CATG_SSP0_1_VOLATILE_BITFIELDS_MSK                   (0x00000000)

  #define DUNIT0_CR_CATG_SSP0_1_SERIAL_PAT0_1_OFF                      ( 0)
  #define DUNIT0_CR_CATG_SSP0_1_SERIAL_PAT0_1_WID                      (32)
  #define DUNIT0_CR_CATG_SSP0_1_SERIAL_PAT0_1_MSK                      (0xFFFFFFFF)
  #define DUNIT0_CR_CATG_SSP0_1_SERIAL_PAT0_1_MIN                      (0)
  #define DUNIT0_CR_CATG_SSP0_1_SERIAL_PAT0_1_MAX                      (4294967295) // 0xFFFFFFFF
  #define DUNIT0_CR_CATG_SSP0_1_SERIAL_PAT0_1_DEF                      (0x11111111)
  #define DUNIT0_CR_CATG_SSP0_1_SERIAL_PAT0_1_HSH                      (0x2000B12C)

#define DUNIT0_CR_CATG_SSP1_0_REG                                      (0x0000B130)
#define DUNIT0_CR_CATG_SSP1_0_DEF                                      (0x55555555)
#define DUNIT0_CR_CATG_SSP1_0_VOLATILE_BITFIELDS_MSK                   (0x00000000)

  #define DUNIT0_CR_CATG_SSP1_0_SERIAL_PAT1_0_OFF                      ( 0)
  #define DUNIT0_CR_CATG_SSP1_0_SERIAL_PAT1_0_WID                      (32)
  #define DUNIT0_CR_CATG_SSP1_0_SERIAL_PAT1_0_MSK                      (0xFFFFFFFF)
  #define DUNIT0_CR_CATG_SSP1_0_SERIAL_PAT1_0_MIN                      (0)
  #define DUNIT0_CR_CATG_SSP1_0_SERIAL_PAT1_0_MAX                      (4294967295) // 0xFFFFFFFF
  #define DUNIT0_CR_CATG_SSP1_0_SERIAL_PAT1_0_DEF                      (0x55555555)
  #define DUNIT0_CR_CATG_SSP1_0_SERIAL_PAT1_0_HSH                      (0x2000B130)

#define DUNIT0_CR_CATG_SSP1_1_REG                                      (0x0000B134)
#define DUNIT0_CR_CATG_SSP1_1_DEF                                      (0x55555555)
#define DUNIT0_CR_CATG_SSP1_1_VOLATILE_BITFIELDS_MSK                   (0x00000000)

  #define DUNIT0_CR_CATG_SSP1_1_SERIAL_PAT1_1_OFF                      ( 0)
  #define DUNIT0_CR_CATG_SSP1_1_SERIAL_PAT1_1_WID                      (32)
  #define DUNIT0_CR_CATG_SSP1_1_SERIAL_PAT1_1_MSK                      (0xFFFFFFFF)
  #define DUNIT0_CR_CATG_SSP1_1_SERIAL_PAT1_1_MIN                      (0)
  #define DUNIT0_CR_CATG_SSP1_1_SERIAL_PAT1_1_MAX                      (4294967295) // 0xFFFFFFFF
  #define DUNIT0_CR_CATG_SSP1_1_SERIAL_PAT1_1_DEF                      (0x55555555)
  #define DUNIT0_CR_CATG_SSP1_1_SERIAL_PAT1_1_HSH                      (0x2000B134)

#define DUNIT0_CR_CATG_SSP2_0_REG                                      (0x0000B138)
#define DUNIT0_CR_CATG_SSP2_0_DEF                                      (0x33333333)
#define DUNIT0_CR_CATG_SSP2_0_VOLATILE_BITFIELDS_MSK                   (0x00000000)

  #define DUNIT0_CR_CATG_SSP2_0_SERIAL_PAT2_0_OFF                      ( 0)
  #define DUNIT0_CR_CATG_SSP2_0_SERIAL_PAT2_0_WID                      (32)
  #define DUNIT0_CR_CATG_SSP2_0_SERIAL_PAT2_0_MSK                      (0xFFFFFFFF)
  #define DUNIT0_CR_CATG_SSP2_0_SERIAL_PAT2_0_MIN                      (0)
  #define DUNIT0_CR_CATG_SSP2_0_SERIAL_PAT2_0_MAX                      (4294967295) // 0xFFFFFFFF
  #define DUNIT0_CR_CATG_SSP2_0_SERIAL_PAT2_0_DEF                      (0x33333333)
  #define DUNIT0_CR_CATG_SSP2_0_SERIAL_PAT2_0_HSH                      (0x2000B138)

#define DUNIT0_CR_CATG_SSP2_1_REG                                      (0x0000B13C)
#define DUNIT0_CR_CATG_SSP2_1_DEF                                      (0x33333333)
#define DUNIT0_CR_CATG_SSP2_1_VOLATILE_BITFIELDS_MSK                   (0x00000000)

  #define DUNIT0_CR_CATG_SSP2_1_SERIAL_PAT2_1_OFF                      ( 0)
  #define DUNIT0_CR_CATG_SSP2_1_SERIAL_PAT2_1_WID                      (32)
  #define DUNIT0_CR_CATG_SSP2_1_SERIAL_PAT2_1_MSK                      (0xFFFFFFFF)
  #define DUNIT0_CR_CATG_SSP2_1_SERIAL_PAT2_1_MIN                      (0)
  #define DUNIT0_CR_CATG_SSP2_1_SERIAL_PAT2_1_MAX                      (4294967295) // 0xFFFFFFFF
  #define DUNIT0_CR_CATG_SSP2_1_SERIAL_PAT2_1_DEF                      (0x33333333)
  #define DUNIT0_CR_CATG_SSP2_1_SERIAL_PAT2_1_HSH                      (0x2000B13C)

#define DUNIT0_CR_CATG_SSP3_0_REG                                      (0x0000B140)
#define DUNIT0_CR_CATG_SSP3_0_DEF                                      (0x0F0F0F0F)
#define DUNIT0_CR_CATG_SSP3_0_VOLATILE_BITFIELDS_MSK                   (0x00000000)

  #define DUNIT0_CR_CATG_SSP3_0_SERIAL_PAT3_0_OFF                      ( 0)
  #define DUNIT0_CR_CATG_SSP3_0_SERIAL_PAT3_0_WID                      (32)
  #define DUNIT0_CR_CATG_SSP3_0_SERIAL_PAT3_0_MSK                      (0xFFFFFFFF)
  #define DUNIT0_CR_CATG_SSP3_0_SERIAL_PAT3_0_MIN                      (0)
  #define DUNIT0_CR_CATG_SSP3_0_SERIAL_PAT3_0_MAX                      (4294967295) // 0xFFFFFFFF
  #define DUNIT0_CR_CATG_SSP3_0_SERIAL_PAT3_0_DEF                      (0x0F0F0F0F)
  #define DUNIT0_CR_CATG_SSP3_0_SERIAL_PAT3_0_HSH                      (0x2000B140)

#define DUNIT0_CR_CATG_SSP3_1_REG                                      (0x0000B144)
#define DUNIT0_CR_CATG_SSP3_1_DEF                                      (0x0F0F0F0F)
#define DUNIT0_CR_CATG_SSP3_1_VOLATILE_BITFIELDS_MSK                   (0x00000000)

  #define DUNIT0_CR_CATG_SSP3_1_SERIAL_PAT3_1_OFF                      ( 0)
  #define DUNIT0_CR_CATG_SSP3_1_SERIAL_PAT3_1_WID                      (32)
  #define DUNIT0_CR_CATG_SSP3_1_SERIAL_PAT3_1_MSK                      (0xFFFFFFFF)
  #define DUNIT0_CR_CATG_SSP3_1_SERIAL_PAT3_1_MIN                      (0)
  #define DUNIT0_CR_CATG_SSP3_1_SERIAL_PAT3_1_MAX                      (4294967295) // 0xFFFFFFFF
  #define DUNIT0_CR_CATG_SSP3_1_SERIAL_PAT3_1_DEF                      (0x0F0F0F0F)
  #define DUNIT0_CR_CATG_SSP3_1_SERIAL_PAT3_1_HSH                      (0x2000B144)

#define DUNIT0_CR_CATG_SSPL_REG                                        (0x0000B148)
#define DUNIT0_CR_CATG_SSPL_DEF                                        (0x00003333)
#define DUNIT0_CR_CATG_SSPL_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT0_LEN_OFF                      ( 0)
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT0_LEN_WID                      ( 2)
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT0_LEN_MSK                      (0x00000003)
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT0_LEN_MIN                      (0)
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT0_LEN_MAX                      (3) // 0x00000003
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT0_LEN_DEF                      (0x00000003)
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT0_LEN_HSH                      (0x0200B148)

  #define DUNIT0_CR_CATG_SSPL_RSVD3_2_OFF                              ( 2)
  #define DUNIT0_CR_CATG_SSPL_RSVD3_2_WID                              ( 2)
  #define DUNIT0_CR_CATG_SSPL_RSVD3_2_MSK                              (0x0000000C)
  #define DUNIT0_CR_CATG_SSPL_RSVD3_2_MIN                              (0)
  #define DUNIT0_CR_CATG_SSPL_RSVD3_2_MAX                              (3) // 0x00000003
  #define DUNIT0_CR_CATG_SSPL_RSVD3_2_DEF                              (0x00000000)
  #define DUNIT0_CR_CATG_SSPL_RSVD3_2_HSH                              (0x0204B148)

  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT1_LEN_OFF                      ( 4)
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT1_LEN_WID                      ( 2)
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT1_LEN_MSK                      (0x00000030)
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT1_LEN_MIN                      (0)
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT1_LEN_MAX                      (3) // 0x00000003
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT1_LEN_DEF                      (0x00000003)
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT1_LEN_HSH                      (0x0208B148)

  #define DUNIT0_CR_CATG_SSPL_RSVD7_6_OFF                              ( 6)
  #define DUNIT0_CR_CATG_SSPL_RSVD7_6_WID                              ( 2)
  #define DUNIT0_CR_CATG_SSPL_RSVD7_6_MSK                              (0x000000C0)
  #define DUNIT0_CR_CATG_SSPL_RSVD7_6_MIN                              (0)
  #define DUNIT0_CR_CATG_SSPL_RSVD7_6_MAX                              (3) // 0x00000003
  #define DUNIT0_CR_CATG_SSPL_RSVD7_6_DEF                              (0x00000000)
  #define DUNIT0_CR_CATG_SSPL_RSVD7_6_HSH                              (0x020CB148)

  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT2_LEN_OFF                      ( 8)
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT2_LEN_WID                      ( 2)
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT2_LEN_MSK                      (0x00000300)
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT2_LEN_MIN                      (0)
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT2_LEN_MAX                      (3) // 0x00000003
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT2_LEN_DEF                      (0x00000003)
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT2_LEN_HSH                      (0x0210B148)

  #define DUNIT0_CR_CATG_SSPL_RSVD11_10_OFF                            (10)
  #define DUNIT0_CR_CATG_SSPL_RSVD11_10_WID                            ( 2)
  #define DUNIT0_CR_CATG_SSPL_RSVD11_10_MSK                            (0x00000C00)
  #define DUNIT0_CR_CATG_SSPL_RSVD11_10_MIN                            (0)
  #define DUNIT0_CR_CATG_SSPL_RSVD11_10_MAX                            (3) // 0x00000003
  #define DUNIT0_CR_CATG_SSPL_RSVD11_10_DEF                            (0x00000000)
  #define DUNIT0_CR_CATG_SSPL_RSVD11_10_HSH                            (0x0214B148)

  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT3_LEN_OFF                      (12)
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT3_LEN_WID                      ( 2)
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT3_LEN_MSK                      (0x00003000)
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT3_LEN_MIN                      (0)
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT3_LEN_MAX                      (3) // 0x00000003
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT3_LEN_DEF                      (0x00000003)
  #define DUNIT0_CR_CATG_SSPL_SERIAL_PAT3_LEN_HSH                      (0x0218B148)

  #define DUNIT0_CR_CATG_SSPL_RSVD31_14_OFF                            (14)
  #define DUNIT0_CR_CATG_SSPL_RSVD31_14_WID                            (18)
  #define DUNIT0_CR_CATG_SSPL_RSVD31_14_MSK                            (0xFFFFC000)
  #define DUNIT0_CR_CATG_SSPL_RSVD31_14_MIN                            (0)
  #define DUNIT0_CR_CATG_SSPL_RSVD31_14_MAX                            (262143) // 0x0003FFFF
  #define DUNIT0_CR_CATG_SSPL_RSVD31_14_DEF                            (0x00000000)
  #define DUNIT0_CR_CATG_SSPL_RSVD31_14_HSH                            (0x121CB148)

#define DUNIT0_CR_CATG_LFSR_POLY_REG                                   (0x0000B14C)
#define DUNIT0_CR_CATG_LFSR_POLY_DEF                                   (0x00000000)
#define DUNIT0_CR_CATG_LFSR_POLY_VOLATILE_BITFIELDS_MSK                (0x00000000)

  #define DUNIT0_CR_CATG_LFSR_POLY_LFSR0_POLY_OFF                      ( 0)
  #define DUNIT0_CR_CATG_LFSR_POLY_LFSR0_POLY_WID                      ( 3)
  #define DUNIT0_CR_CATG_LFSR_POLY_LFSR0_POLY_MSK                      (0x00000007)
  #define DUNIT0_CR_CATG_LFSR_POLY_LFSR0_POLY_MIN                      (0)
  #define DUNIT0_CR_CATG_LFSR_POLY_LFSR0_POLY_MAX                      (7) // 0x00000007
  #define DUNIT0_CR_CATG_LFSR_POLY_LFSR0_POLY_DEF                      (0x00000000)
  #define DUNIT0_CR_CATG_LFSR_POLY_LFSR0_POLY_HSH                      (0x0300B14C)

  #define DUNIT0_CR_CATG_LFSR_POLY_RSVD3_OFF                           ( 3)
  #define DUNIT0_CR_CATG_LFSR_POLY_RSVD3_WID                           ( 1)
  #define DUNIT0_CR_CATG_LFSR_POLY_RSVD3_MSK                           (0x00000008)
  #define DUNIT0_CR_CATG_LFSR_POLY_RSVD3_MIN                           (0)
  #define DUNIT0_CR_CATG_LFSR_POLY_RSVD3_MAX                           (1) // 0x00000001
  #define DUNIT0_CR_CATG_LFSR_POLY_RSVD3_DEF                           (0x00000000)
  #define DUNIT0_CR_CATG_LFSR_POLY_RSVD3_HSH                           (0x0106B14C)

  #define DUNIT0_CR_CATG_LFSR_POLY_LFSR1_POLY_OFF                      ( 4)
  #define DUNIT0_CR_CATG_LFSR_POLY_LFSR1_POLY_WID                      ( 3)
  #define DUNIT0_CR_CATG_LFSR_POLY_LFSR1_POLY_MSK                      (0x00000070)
  #define DUNIT0_CR_CATG_LFSR_POLY_LFSR1_POLY_MIN                      (0)
  #define DUNIT0_CR_CATG_LFSR_POLY_LFSR1_POLY_MAX                      (7) // 0x00000007
  #define DUNIT0_CR_CATG_LFSR_POLY_LFSR1_POLY_DEF                      (0x00000000)
  #define DUNIT0_CR_CATG_LFSR_POLY_LFSR1_POLY_HSH                      (0x0308B14C)

  #define DUNIT0_CR_CATG_LFSR_POLY_RSVD31_7_OFF                        ( 7)
  #define DUNIT0_CR_CATG_LFSR_POLY_RSVD31_7_WID                        (25)
  #define DUNIT0_CR_CATG_LFSR_POLY_RSVD31_7_MSK                        (0xFFFFFF80)
  #define DUNIT0_CR_CATG_LFSR_POLY_RSVD31_7_MIN                        (0)
  #define DUNIT0_CR_CATG_LFSR_POLY_RSVD31_7_MAX                        (33554431) // 0x01FFFFFF
  #define DUNIT0_CR_CATG_LFSR_POLY_RSVD31_7_DEF                        (0x00000000)
  #define DUNIT0_CR_CATG_LFSR_POLY_RSVD31_7_HSH                        (0x190EB14C)

#define DUNIT0_CR_CATG_LFSR0_SEED_REG                                  (0x0000B150)
#define DUNIT0_CR_CATG_LFSR0_SEED_DEF                                  (0x00000001)
#define DUNIT0_CR_CATG_LFSR0_SEED_VOLATILE_BITFIELDS_MSK               (0x00000000)

  #define DUNIT0_CR_CATG_LFSR0_SEED_LFSR0_SEED_OFF                     ( 0)
  #define DUNIT0_CR_CATG_LFSR0_SEED_LFSR0_SEED_WID                     (32)
  #define DUNIT0_CR_CATG_LFSR0_SEED_LFSR0_SEED_MSK                     (0xFFFFFFFF)
  #define DUNIT0_CR_CATG_LFSR0_SEED_LFSR0_SEED_MIN                     (0)
  #define DUNIT0_CR_CATG_LFSR0_SEED_LFSR0_SEED_MAX                     (4294967295) // 0xFFFFFFFF
  #define DUNIT0_CR_CATG_LFSR0_SEED_LFSR0_SEED_DEF                     (0x00000001)
  #define DUNIT0_CR_CATG_LFSR0_SEED_LFSR0_SEED_HSH                     (0x2000B150)

#define DUNIT0_CR_CATG_LFSR1_SEED_REG                                  (0x0000B154)
#define DUNIT0_CR_CATG_LFSR1_SEED_DEF                                  (0x00000002)
#define DUNIT0_CR_CATG_LFSR1_SEED_VOLATILE_BITFIELDS_MSK               (0x00000000)

  #define DUNIT0_CR_CATG_LFSR1_SEED_LFSR1_SEED_OFF                     ( 0)
  #define DUNIT0_CR_CATG_LFSR1_SEED_LFSR1_SEED_WID                     (32)
  #define DUNIT0_CR_CATG_LFSR1_SEED_LFSR1_SEED_MSK                     (0xFFFFFFFF)
  #define DUNIT0_CR_CATG_LFSR1_SEED_LFSR1_SEED_MIN                     (0)
  #define DUNIT0_CR_CATG_LFSR1_SEED_LFSR1_SEED_MAX                     (4294967295) // 0xFFFFFFFF
  #define DUNIT0_CR_CATG_LFSR1_SEED_LFSR1_SEED_DEF                     (0x00000002)
  #define DUNIT0_CR_CATG_LFSR1_SEED_LFSR1_SEED_HSH                     (0x2000B154)

#define DUNIT0_CR_CATG_CA_DCINVC_REG                                   (0x0000B158)
#define DUNIT0_CR_CATG_CA_DCINVC_DEF                                   (0x00000000)
#define DUNIT0_CR_CATG_CA_DCINVC_VOLATILE_BITFIELDS_MSK                (0x00000000)

  #define DUNIT0_CR_CATG_CA_DCINVC_CA_DC_OFF                           ( 0)
  #define DUNIT0_CR_CATG_CA_DCINVC_CA_DC_WID                           (14)
  #define DUNIT0_CR_CATG_CA_DCINVC_CA_DC_MSK                           (0x00003FFF)
  #define DUNIT0_CR_CATG_CA_DCINVC_CA_DC_MIN                           (0)
  #define DUNIT0_CR_CATG_CA_DCINVC_CA_DC_MAX                           (16383) // 0x00003FFF
  #define DUNIT0_CR_CATG_CA_DCINVC_CA_DC_DEF                           (0x00000000)
  #define DUNIT0_CR_CATG_CA_DCINVC_CA_DC_HSH                           (0x0E00B158)

  #define DUNIT0_CR_CATG_CA_DCINVC_RSVD15_14_OFF                       (14)
  #define DUNIT0_CR_CATG_CA_DCINVC_RSVD15_14_WID                       ( 2)
  #define DUNIT0_CR_CATG_CA_DCINVC_RSVD15_14_MSK                       (0x0000C000)
  #define DUNIT0_CR_CATG_CA_DCINVC_RSVD15_14_MIN                       (0)
  #define DUNIT0_CR_CATG_CA_DCINVC_RSVD15_14_MAX                       (3) // 0x00000003
  #define DUNIT0_CR_CATG_CA_DCINVC_RSVD15_14_DEF                       (0x00000000)
  #define DUNIT0_CR_CATG_CA_DCINVC_RSVD15_14_HSH                       (0x021CB158)

  #define DUNIT0_CR_CATG_CA_DCINVC_CA_INV_OFF                          (16)
  #define DUNIT0_CR_CATG_CA_DCINVC_CA_INV_WID                          (14)
  #define DUNIT0_CR_CATG_CA_DCINVC_CA_INV_MSK                          (0x3FFF0000)
  #define DUNIT0_CR_CATG_CA_DCINVC_CA_INV_MIN                          (0)
  #define DUNIT0_CR_CATG_CA_DCINVC_CA_INV_MAX                          (16383) // 0x00003FFF
  #define DUNIT0_CR_CATG_CA_DCINVC_CA_INV_DEF                          (0x00000000)
  #define DUNIT0_CR_CATG_CA_DCINVC_CA_INV_HSH                          (0x0E20B158)

  #define DUNIT0_CR_CATG_CA_DCINVC_RSVD31_30_OFF                       (30)
  #define DUNIT0_CR_CATG_CA_DCINVC_RSVD31_30_WID                       ( 2)
  #define DUNIT0_CR_CATG_CA_DCINVC_RSVD31_30_MSK                       (0xC0000000)
  #define DUNIT0_CR_CATG_CA_DCINVC_RSVD31_30_MIN                       (0)
  #define DUNIT0_CR_CATG_CA_DCINVC_RSVD31_30_MAX                       (3) // 0x00000003
  #define DUNIT0_CR_CATG_CA_DCINVC_RSVD31_30_DEF                       (0x00000000)
  #define DUNIT0_CR_CATG_CA_DCINVC_RSVD31_30_HSH                       (0x023CB158)

#define DUNIT0_CR_CATG_CS_DCINVC_REG                                   (0x0000B15C)
#define DUNIT0_CR_CATG_CS_DCINVC_DEF                                   (0x00000000)
#define DUNIT0_CR_CATG_CS_DCINVC_VOLATILE_BITFIELDS_MSK                (0x00000000)

  #define DUNIT0_CR_CATG_CS_DCINVC_CS_DC_OFF                           ( 0)
  #define DUNIT0_CR_CATG_CS_DCINVC_CS_DC_WID                           ( 4)
  #define DUNIT0_CR_CATG_CS_DCINVC_CS_DC_MSK                           (0x0000000F)
  #define DUNIT0_CR_CATG_CS_DCINVC_CS_DC_MIN                           (0)
  #define DUNIT0_CR_CATG_CS_DCINVC_CS_DC_MAX                           (15) // 0x0000000F
  #define DUNIT0_CR_CATG_CS_DCINVC_CS_DC_DEF                           (0x00000000)
  #define DUNIT0_CR_CATG_CS_DCINVC_CS_DC_HSH                           (0x0400B15C)

  #define DUNIT0_CR_CATG_CS_DCINVC_CS_INV_OFF                          ( 4)
  #define DUNIT0_CR_CATG_CS_DCINVC_CS_INV_WID                          ( 4)
  #define DUNIT0_CR_CATG_CS_DCINVC_CS_INV_MSK                          (0x000000F0)
  #define DUNIT0_CR_CATG_CS_DCINVC_CS_INV_MIN                          (0)
  #define DUNIT0_CR_CATG_CS_DCINVC_CS_INV_MAX                          (15) // 0x0000000F
  #define DUNIT0_CR_CATG_CS_DCINVC_CS_INV_DEF                          (0x00000000)
  #define DUNIT0_CR_CATG_CS_DCINVC_CS_INV_HSH                          (0x0408B15C)

  #define DUNIT0_CR_CATG_CS_DCINVC_RSVD31_8_OFF                        ( 8)
  #define DUNIT0_CR_CATG_CS_DCINVC_RSVD31_8_WID                        (24)
  #define DUNIT0_CR_CATG_CS_DCINVC_RSVD31_8_MSK                        (0xFFFFFF00)
  #define DUNIT0_CR_CATG_CS_DCINVC_RSVD31_8_MIN                        (0)
  #define DUNIT0_CR_CATG_CS_DCINVC_RSVD31_8_MAX                        (16777215) // 0x00FFFFFF
  #define DUNIT0_CR_CATG_CS_DCINVC_RSVD31_8_DEF                        (0x00000000)
  #define DUNIT0_CR_CATG_CS_DCINVC_RSVD31_8_HSH                        (0x1810B15C)

#define MTG0_CR_MTG_MAIN_CTRL_REG                                      (0x0000B180)
#define MTG0_CR_MTG_MAIN_CTRL_DEF                                      (0x00000100)
#define MTG0_CR_MTG_MAIN_CTRL_VOLATILE_BITFIELDS_MSK                   (0x00000061)

  #define MTG0_CR_MTG_MAIN_CTRL_START_OFF                              ( 0)
  #define MTG0_CR_MTG_MAIN_CTRL_START_WID                              ( 1)
  #define MTG0_CR_MTG_MAIN_CTRL_START_MSK                              (0x00000001)
  #define MTG0_CR_MTG_MAIN_CTRL_START_MIN                              (0)
  #define MTG0_CR_MTG_MAIN_CTRL_START_MAX                              (1) // 0x00000001
  #define MTG0_CR_MTG_MAIN_CTRL_START_DEF                              (0x00000000)
  #define MTG0_CR_MTG_MAIN_CTRL_START_HSH                              (0x0180B180)

  #define MTG0_CR_MTG_MAIN_CTRL_NONSTOP_OFF                            ( 1)
  #define MTG0_CR_MTG_MAIN_CTRL_NONSTOP_WID                            ( 1)
  #define MTG0_CR_MTG_MAIN_CTRL_NONSTOP_MSK                            (0x00000002)
  #define MTG0_CR_MTG_MAIN_CTRL_NONSTOP_MIN                            (0)
  #define MTG0_CR_MTG_MAIN_CTRL_NONSTOP_MAX                            (1) // 0x00000001
  #define MTG0_CR_MTG_MAIN_CTRL_NONSTOP_DEF                            (0x00000000)
  #define MTG0_CR_MTG_MAIN_CTRL_NONSTOP_HSH                            (0x0102B180)

  #define MTG0_CR_MTG_MAIN_CTRL_MANUAL_PAUSE_OFF                       ( 2)
  #define MTG0_CR_MTG_MAIN_CTRL_MANUAL_PAUSE_WID                       ( 1)
  #define MTG0_CR_MTG_MAIN_CTRL_MANUAL_PAUSE_MSK                       (0x00000004)
  #define MTG0_CR_MTG_MAIN_CTRL_MANUAL_PAUSE_MIN                       (0)
  #define MTG0_CR_MTG_MAIN_CTRL_MANUAL_PAUSE_MAX                       (1) // 0x00000001
  #define MTG0_CR_MTG_MAIN_CTRL_MANUAL_PAUSE_DEF                       (0x00000000)
  #define MTG0_CR_MTG_MAIN_CTRL_MANUAL_PAUSE_HSH                       (0x0104B180)

  #define MTG0_CR_MTG_MAIN_CTRL_RSVD3_OFF                              ( 3)
  #define MTG0_CR_MTG_MAIN_CTRL_RSVD3_WID                              ( 1)
  #define MTG0_CR_MTG_MAIN_CTRL_RSVD3_MSK                              (0x00000008)
  #define MTG0_CR_MTG_MAIN_CTRL_RSVD3_MIN                              (0)
  #define MTG0_CR_MTG_MAIN_CTRL_RSVD3_MAX                              (1) // 0x00000001
  #define MTG0_CR_MTG_MAIN_CTRL_RSVD3_DEF                              (0x00000000)
  #define MTG0_CR_MTG_MAIN_CTRL_RSVD3_HSH                              (0x0106B180)

  #define MTG0_CR_MTG_MAIN_CTRL_SINGLE_RD_MODE_OFF                     ( 4)
  #define MTG0_CR_MTG_MAIN_CTRL_SINGLE_RD_MODE_WID                     ( 1)
  #define MTG0_CR_MTG_MAIN_CTRL_SINGLE_RD_MODE_MSK                     (0x00000010)
  #define MTG0_CR_MTG_MAIN_CTRL_SINGLE_RD_MODE_MIN                     (0)
  #define MTG0_CR_MTG_MAIN_CTRL_SINGLE_RD_MODE_MAX                     (1) // 0x00000001
  #define MTG0_CR_MTG_MAIN_CTRL_SINGLE_RD_MODE_DEF                     (0x00000000)
  #define MTG0_CR_MTG_MAIN_CTRL_SINGLE_RD_MODE_HSH                     (0x0108B180)

  #define MTG0_CR_MTG_MAIN_CTRL_SINGLE_RD_PAUSED_OFF                   ( 5)
  #define MTG0_CR_MTG_MAIN_CTRL_SINGLE_RD_PAUSED_WID                   ( 1)
  #define MTG0_CR_MTG_MAIN_CTRL_SINGLE_RD_PAUSED_MSK                   (0x00000020)
  #define MTG0_CR_MTG_MAIN_CTRL_SINGLE_RD_PAUSED_MIN                   (0)
  #define MTG0_CR_MTG_MAIN_CTRL_SINGLE_RD_PAUSED_MAX                   (1) // 0x00000001
  #define MTG0_CR_MTG_MAIN_CTRL_SINGLE_RD_PAUSED_DEF                   (0x00000000)
  #define MTG0_CR_MTG_MAIN_CTRL_SINGLE_RD_PAUSED_HSH                   (0x018AB180)

  #define MTG0_CR_MTG_MAIN_CTRL_SINGLE_RD_CONTINUE_OFF                 ( 6)
  #define MTG0_CR_MTG_MAIN_CTRL_SINGLE_RD_CONTINUE_WID                 ( 1)
  #define MTG0_CR_MTG_MAIN_CTRL_SINGLE_RD_CONTINUE_MSK                 (0x00000040)
  #define MTG0_CR_MTG_MAIN_CTRL_SINGLE_RD_CONTINUE_MIN                 (0)
  #define MTG0_CR_MTG_MAIN_CTRL_SINGLE_RD_CONTINUE_MAX                 (1) // 0x00000001
  #define MTG0_CR_MTG_MAIN_CTRL_SINGLE_RD_CONTINUE_DEF                 (0x00000000)
  #define MTG0_CR_MTG_MAIN_CTRL_SINGLE_RD_CONTINUE_HSH                 (0x018CB180)

  #define MTG0_CR_MTG_MAIN_CTRL_RSVD7_OFF                              ( 7)
  #define MTG0_CR_MTG_MAIN_CTRL_RSVD7_WID                              ( 1)
  #define MTG0_CR_MTG_MAIN_CTRL_RSVD7_MSK                              (0x00000080)
  #define MTG0_CR_MTG_MAIN_CTRL_RSVD7_MIN                              (0)
  #define MTG0_CR_MTG_MAIN_CTRL_RSVD7_MAX                              (1) // 0x00000001
  #define MTG0_CR_MTG_MAIN_CTRL_RSVD7_DEF                              (0x00000000)
  #define MTG0_CR_MTG_MAIN_CTRL_RSVD7_HSH                              (0x010EB180)

  #define MTG0_CR_MTG_MAIN_CTRL_REQ_SIZE_OFF                           ( 8)
  #define MTG0_CR_MTG_MAIN_CTRL_REQ_SIZE_WID                           ( 1)
  #define MTG0_CR_MTG_MAIN_CTRL_REQ_SIZE_MSK                           (0x00000100)
  #define MTG0_CR_MTG_MAIN_CTRL_REQ_SIZE_MIN                           (0)
  #define MTG0_CR_MTG_MAIN_CTRL_REQ_SIZE_MAX                           (1) // 0x00000001
  #define MTG0_CR_MTG_MAIN_CTRL_REQ_SIZE_DEF                           (0x00000001)
  #define MTG0_CR_MTG_MAIN_CTRL_REQ_SIZE_HSH                           (0x0110B180)

  #define MTG0_CR_MTG_MAIN_CTRL_RSVD31_9_OFF                           ( 9)
  #define MTG0_CR_MTG_MAIN_CTRL_RSVD31_9_WID                           (23)
  #define MTG0_CR_MTG_MAIN_CTRL_RSVD31_9_MSK                           (0xFFFFFE00)
  #define MTG0_CR_MTG_MAIN_CTRL_RSVD31_9_MIN                           (0)
  #define MTG0_CR_MTG_MAIN_CTRL_RSVD31_9_MAX                           (8388607) // 0x007FFFFF
  #define MTG0_CR_MTG_MAIN_CTRL_RSVD31_9_DEF                           (0x00000000)
  #define MTG0_CR_MTG_MAIN_CTRL_RSVD31_9_HSH                           (0x1712B180)

#define MTG0_CR_MTG_CE0_REG                                            (0x0000B190)
#define MTG0_CR_MTG_CE0_DEF                                            (0x0000A000)
#define MTG0_CR_MTG_CE0_VOLATILE_BITFIELDS_MSK                         (0x00000000)

  #define MTG0_CR_MTG_CE0_CMD_CNT_OFF                                  ( 0)
  #define MTG0_CR_MTG_CE0_CMD_CNT_WID                                  (10)
  #define MTG0_CR_MTG_CE0_CMD_CNT_MSK                                  (0x000003FF)
  #define MTG0_CR_MTG_CE0_CMD_CNT_MIN                                  (0)
  #define MTG0_CR_MTG_CE0_CMD_CNT_MAX                                  (1023) // 0x000003FF
  #define MTG0_CR_MTG_CE0_CMD_CNT_DEF                                  (0x00000000)
  #define MTG0_CR_MTG_CE0_CMD_CNT_HSH                                  (0x0A00B190)

  #define MTG0_CR_MTG_CE0_RSVD11_10_OFF                                (10)
  #define MTG0_CR_MTG_CE0_RSVD11_10_WID                                ( 2)
  #define MTG0_CR_MTG_CE0_RSVD11_10_MSK                                (0x00000C00)
  #define MTG0_CR_MTG_CE0_RSVD11_10_MIN                                (0)
  #define MTG0_CR_MTG_CE0_RSVD11_10_MAX                                (3) // 0x00000003
  #define MTG0_CR_MTG_CE0_RSVD11_10_DEF                                (0x00000000)
  #define MTG0_CR_MTG_CE0_RSVD11_10_HSH                                (0x0214B190)

  #define MTG0_CR_MTG_CE0_CMD_TYPE_OFF                                 (12)
  #define MTG0_CR_MTG_CE0_CMD_TYPE_WID                                 ( 3)
  #define MTG0_CR_MTG_CE0_CMD_TYPE_MSK                                 (0x00007000)
  #define MTG0_CR_MTG_CE0_CMD_TYPE_MIN                                 (0)
  #define MTG0_CR_MTG_CE0_CMD_TYPE_MAX                                 (7) // 0x00000007
  #define MTG0_CR_MTG_CE0_CMD_TYPE_DEF                                 (0x00000002)
  #define MTG0_CR_MTG_CE0_CMD_TYPE_HSH                                 (0x0318B190)

  #define MTG0_CR_MTG_CE0_LAST_ENTRY_OFF                               (15)
  #define MTG0_CR_MTG_CE0_LAST_ENTRY_WID                               ( 1)
  #define MTG0_CR_MTG_CE0_LAST_ENTRY_MSK                               (0x00008000)
  #define MTG0_CR_MTG_CE0_LAST_ENTRY_MIN                               (0)
  #define MTG0_CR_MTG_CE0_LAST_ENTRY_MAX                               (1) // 0x00000001
  #define MTG0_CR_MTG_CE0_LAST_ENTRY_DEF                               (0x00000001)
  #define MTG0_CR_MTG_CE0_LAST_ENTRY_HSH                               (0x011EB190)

  #define MTG0_CR_MTG_CE0_WDATA_TYPE_OFF                               (16)
  #define MTG0_CR_MTG_CE0_WDATA_TYPE_WID                               ( 2)
  #define MTG0_CR_MTG_CE0_WDATA_TYPE_MSK                               (0x00030000)
  #define MTG0_CR_MTG_CE0_WDATA_TYPE_MIN                               (0)
  #define MTG0_CR_MTG_CE0_WDATA_TYPE_MAX                               (3) // 0x00000003
  #define MTG0_CR_MTG_CE0_WDATA_TYPE_DEF                               (0x00000000)
  #define MTG0_CR_MTG_CE0_WDATA_TYPE_HSH                               (0x0220B190)

  #define MTG0_CR_MTG_CE0_RDATA_TYPE_OFF                               (18)
  #define MTG0_CR_MTG_CE0_RDATA_TYPE_WID                               ( 2)
  #define MTG0_CR_MTG_CE0_RDATA_TYPE_MSK                               (0x000C0000)
  #define MTG0_CR_MTG_CE0_RDATA_TYPE_MIN                               (0)
  #define MTG0_CR_MTG_CE0_RDATA_TYPE_MAX                               (3) // 0x00000003
  #define MTG0_CR_MTG_CE0_RDATA_TYPE_DEF                               (0x00000000)
  #define MTG0_CR_MTG_CE0_RDATA_TYPE_HSH                               (0x0224B190)

  #define MTG0_CR_MTG_CE0_ROW_OFFSET_OFF                               (20)
  #define MTG0_CR_MTG_CE0_ROW_OFFSET_WID                               ( 4)
  #define MTG0_CR_MTG_CE0_ROW_OFFSET_MSK                               (0x00F00000)
  #define MTG0_CR_MTG_CE0_ROW_OFFSET_MIN                               (0)
  #define MTG0_CR_MTG_CE0_ROW_OFFSET_MAX                               (15) // 0x0000000F
  #define MTG0_CR_MTG_CE0_ROW_OFFSET_DEF                               (0x00000000)
  #define MTG0_CR_MTG_CE0_ROW_OFFSET_HSH                               (0x0428B190)

  #define MTG0_CR_MTG_CE0_ERRCHK_DIS_OFF                               (24)
  #define MTG0_CR_MTG_CE0_ERRCHK_DIS_WID                               ( 1)
  #define MTG0_CR_MTG_CE0_ERRCHK_DIS_MSK                               (0x01000000)
  #define MTG0_CR_MTG_CE0_ERRCHK_DIS_MIN                               (0)
  #define MTG0_CR_MTG_CE0_ERRCHK_DIS_MAX                               (1) // 0x00000001
  #define MTG0_CR_MTG_CE0_ERRCHK_DIS_DEF                               (0x00000000)
  #define MTG0_CR_MTG_CE0_ERRCHK_DIS_HSH                               (0x0130B190)

  #define MTG0_CR_MTG_CE0_RSVD27_25_OFF                                (25)
  #define MTG0_CR_MTG_CE0_RSVD27_25_WID                                ( 3)
  #define MTG0_CR_MTG_CE0_RSVD27_25_MSK                                (0x0E000000)
  #define MTG0_CR_MTG_CE0_RSVD27_25_MIN                                (0)
  #define MTG0_CR_MTG_CE0_RSVD27_25_MAX                                (7) // 0x00000007
  #define MTG0_CR_MTG_CE0_RSVD27_25_DEF                                (0x00000000)
  #define MTG0_CR_MTG_CE0_RSVD27_25_HSH                                (0x0332B190)

  #define MTG0_CR_MTG_CE0_ADDR_MODE_OFF                                (28)
  #define MTG0_CR_MTG_CE0_ADDR_MODE_WID                                ( 4)
  #define MTG0_CR_MTG_CE0_ADDR_MODE_MSK                                (0xF0000000)
  #define MTG0_CR_MTG_CE0_ADDR_MODE_MIN                                (0)
  #define MTG0_CR_MTG_CE0_ADDR_MODE_MAX                                (15) // 0x0000000F
  #define MTG0_CR_MTG_CE0_ADDR_MODE_DEF                                (0x00000000)
  #define MTG0_CR_MTG_CE0_ADDR_MODE_HSH                                (0x0438B190)

#define MTG0_CR_MTG_CE1_REG                                            (0x0000B194)
#define MTG0_CR_MTG_CE1_DEF                                            (0x00002000)
#define MTG0_CR_MTG_CE1_VOLATILE_BITFIELDS_MSK                         (0x00000000)

  #define MTG0_CR_MTG_CE1_CMD_CNT_OFF                                  ( 0)
  #define MTG0_CR_MTG_CE1_CMD_CNT_WID                                  (10)
  #define MTG0_CR_MTG_CE1_CMD_CNT_MSK                                  (0x000003FF)
  #define MTG0_CR_MTG_CE1_CMD_CNT_MIN                                  (0)
  #define MTG0_CR_MTG_CE1_CMD_CNT_MAX                                  (1023) // 0x000003FF
  #define MTG0_CR_MTG_CE1_CMD_CNT_DEF                                  (0x00000000)
  #define MTG0_CR_MTG_CE1_CMD_CNT_HSH                                  (0x0A00B194)

  #define MTG0_CR_MTG_CE1_RSVD11_10_OFF                                (10)
  #define MTG0_CR_MTG_CE1_RSVD11_10_WID                                ( 2)
  #define MTG0_CR_MTG_CE1_RSVD11_10_MSK                                (0x00000C00)
  #define MTG0_CR_MTG_CE1_RSVD11_10_MIN                                (0)
  #define MTG0_CR_MTG_CE1_RSVD11_10_MAX                                (3) // 0x00000003
  #define MTG0_CR_MTG_CE1_RSVD11_10_DEF                                (0x00000000)
  #define MTG0_CR_MTG_CE1_RSVD11_10_HSH                                (0x0214B194)

  #define MTG0_CR_MTG_CE1_CMD_TYPE_OFF                                 (12)
  #define MTG0_CR_MTG_CE1_CMD_TYPE_WID                                 ( 3)
  #define MTG0_CR_MTG_CE1_CMD_TYPE_MSK                                 (0x00007000)
  #define MTG0_CR_MTG_CE1_CMD_TYPE_MIN                                 (0)
  #define MTG0_CR_MTG_CE1_CMD_TYPE_MAX                                 (7) // 0x00000007
  #define MTG0_CR_MTG_CE1_CMD_TYPE_DEF                                 (0x00000002)
  #define MTG0_CR_MTG_CE1_CMD_TYPE_HSH                                 (0x0318B194)

  #define MTG0_CR_MTG_CE1_LAST_ENTRY_OFF                               (15)
  #define MTG0_CR_MTG_CE1_LAST_ENTRY_WID                               ( 1)
  #define MTG0_CR_MTG_CE1_LAST_ENTRY_MSK                               (0x00008000)
  #define MTG0_CR_MTG_CE1_LAST_ENTRY_MIN                               (0)
  #define MTG0_CR_MTG_CE1_LAST_ENTRY_MAX                               (1) // 0x00000001
  #define MTG0_CR_MTG_CE1_LAST_ENTRY_DEF                               (0x00000000)
  #define MTG0_CR_MTG_CE1_LAST_ENTRY_HSH                               (0x011EB194)

  #define MTG0_CR_MTG_CE1_WDATA_TYPE_OFF                               (16)
  #define MTG0_CR_MTG_CE1_WDATA_TYPE_WID                               ( 2)
  #define MTG0_CR_MTG_CE1_WDATA_TYPE_MSK                               (0x00030000)
  #define MTG0_CR_MTG_CE1_WDATA_TYPE_MIN                               (0)
  #define MTG0_CR_MTG_CE1_WDATA_TYPE_MAX                               (3) // 0x00000003
  #define MTG0_CR_MTG_CE1_WDATA_TYPE_DEF                               (0x00000000)
  #define MTG0_CR_MTG_CE1_WDATA_TYPE_HSH                               (0x0220B194)

  #define MTG0_CR_MTG_CE1_RDATA_TYPE_OFF                               (18)
  #define MTG0_CR_MTG_CE1_RDATA_TYPE_WID                               ( 2)
  #define MTG0_CR_MTG_CE1_RDATA_TYPE_MSK                               (0x000C0000)
  #define MTG0_CR_MTG_CE1_RDATA_TYPE_MIN                               (0)
  #define MTG0_CR_MTG_CE1_RDATA_TYPE_MAX                               (3) // 0x00000003
  #define MTG0_CR_MTG_CE1_RDATA_TYPE_DEF                               (0x00000000)
  #define MTG0_CR_MTG_CE1_RDATA_TYPE_HSH                               (0x0224B194)

  #define MTG0_CR_MTG_CE1_ROW_OFFSET_OFF                               (20)
  #define MTG0_CR_MTG_CE1_ROW_OFFSET_WID                               ( 4)
  #define MTG0_CR_MTG_CE1_ROW_OFFSET_MSK                               (0x00F00000)
  #define MTG0_CR_MTG_CE1_ROW_OFFSET_MIN                               (0)
  #define MTG0_CR_MTG_CE1_ROW_OFFSET_MAX                               (15) // 0x0000000F
  #define MTG0_CR_MTG_CE1_ROW_OFFSET_DEF                               (0x00000000)
  #define MTG0_CR_MTG_CE1_ROW_OFFSET_HSH                               (0x0428B194)

  #define MTG0_CR_MTG_CE1_ERRCHK_DIS_OFF                               (24)
  #define MTG0_CR_MTG_CE1_ERRCHK_DIS_WID                               ( 1)
  #define MTG0_CR_MTG_CE1_ERRCHK_DIS_MSK                               (0x01000000)
  #define MTG0_CR_MTG_CE1_ERRCHK_DIS_MIN                               (0)
  #define MTG0_CR_MTG_CE1_ERRCHK_DIS_MAX                               (1) // 0x00000001
  #define MTG0_CR_MTG_CE1_ERRCHK_DIS_DEF                               (0x00000000)
  #define MTG0_CR_MTG_CE1_ERRCHK_DIS_HSH                               (0x0130B194)

  #define MTG0_CR_MTG_CE1_RSVD27_25_OFF                                (25)
  #define MTG0_CR_MTG_CE1_RSVD27_25_WID                                ( 3)
  #define MTG0_CR_MTG_CE1_RSVD27_25_MSK                                (0x0E000000)
  #define MTG0_CR_MTG_CE1_RSVD27_25_MIN                                (0)
  #define MTG0_CR_MTG_CE1_RSVD27_25_MAX                                (7) // 0x00000007
  #define MTG0_CR_MTG_CE1_RSVD27_25_DEF                                (0x00000000)
  #define MTG0_CR_MTG_CE1_RSVD27_25_HSH                                (0x0332B194)

  #define MTG0_CR_MTG_CE1_ADDR_MODE_OFF                                (28)
  #define MTG0_CR_MTG_CE1_ADDR_MODE_WID                                ( 4)
  #define MTG0_CR_MTG_CE1_ADDR_MODE_MSK                                (0xF0000000)
  #define MTG0_CR_MTG_CE1_ADDR_MODE_MIN                                (0)
  #define MTG0_CR_MTG_CE1_ADDR_MODE_MAX                                (15) // 0x0000000F
  #define MTG0_CR_MTG_CE1_ADDR_MODE_DEF                                (0x00000000)
  #define MTG0_CR_MTG_CE1_ADDR_MODE_HSH                                (0x0438B194)

#define MTG0_CR_MTG_CE2_REG                                            (0x0000B198)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG0_CR_MTG_CE3_REG                                            (0x0000B19C)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG0_CR_MTG_CE4_REG                                            (0x0000B1A0)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG0_CR_MTG_CE5_REG                                            (0x0000B1A4)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG0_CR_MTG_CE6_REG                                            (0x0000B1A8)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG0_CR_MTG_CE7_REG                                            (0x0000B1AC)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG0_CR_MTG_CE8_REG                                            (0x0000B1B0)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG0_CR_MTG_CE9_REG                                            (0x0000B1B4)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG0_CR_MTG_CE10_REG                                           (0x0000B1B8)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG0_CR_MTG_CE11_REG                                           (0x0000B1BC)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG0_CR_MTG_CE12_REG                                           (0x0000B1C0)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG0_CR_MTG_CE13_REG                                           (0x0000B1C4)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG0_CR_MTG_CE14_REG                                           (0x0000B1C8)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG0_CR_MTG_CE15_REG                                           (0x0000B1CC)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG0_CR_MTG_CDLY_REG                                           (0x0000B1E0)
#define MTG0_CR_MTG_CDLY_DEF                                           (0x00000000)
#define MTG0_CR_MTG_CDLY_VOLATILE_BITFIELDS_MSK                        (0x00000000)

  #define MTG0_CR_MTG_CDLY_CMD_ENTRY_DLY_OFF                           ( 0)
  #define MTG0_CR_MTG_CDLY_CMD_ENTRY_DLY_WID                           (16)
  #define MTG0_CR_MTG_CDLY_CMD_ENTRY_DLY_MSK                           (0x0000FFFF)
  #define MTG0_CR_MTG_CDLY_CMD_ENTRY_DLY_MIN                           (0)
  #define MTG0_CR_MTG_CDLY_CMD_ENTRY_DLY_MAX                           (65535) // 0x0000FFFF
  #define MTG0_CR_MTG_CDLY_CMD_ENTRY_DLY_DEF                           (0x00000000)
  #define MTG0_CR_MTG_CDLY_CMD_ENTRY_DLY_HSH                           (0x1000B1E0)

  #define MTG0_CR_MTG_CDLY_CMD_LOOP_DLY_OFF                            (16)
  #define MTG0_CR_MTG_CDLY_CMD_LOOP_DLY_WID                            (16)
  #define MTG0_CR_MTG_CDLY_CMD_LOOP_DLY_MSK                            (0xFFFF0000)
  #define MTG0_CR_MTG_CDLY_CMD_LOOP_DLY_MIN                            (0)
  #define MTG0_CR_MTG_CDLY_CMD_LOOP_DLY_MAX                            (65535) // 0x0000FFFF
  #define MTG0_CR_MTG_CDLY_CMD_LOOP_DLY_DEF                            (0x00000000)
  #define MTG0_CR_MTG_CDLY_CMD_LOOP_DLY_HSH                            (0x1020B1E0)

#define MTG0_CR_MTG_CLC_REG                                            (0x0000B1E4)
#define MTG0_CR_MTG_CLC_DEF                                            (0x00000000)
#define MTG0_CR_MTG_CLC_VOLATILE_BITFIELDS_MSK                         (0x00000000)

  #define MTG0_CR_MTG_CLC_CMD_LOOP_CNT_OFF                             ( 0)
  #define MTG0_CR_MTG_CLC_CMD_LOOP_CNT_WID                             (32)
  #define MTG0_CR_MTG_CLC_CMD_LOOP_CNT_MSK                             (0xFFFFFFFF)
  #define MTG0_CR_MTG_CLC_CMD_LOOP_CNT_MIN                             (0)
  #define MTG0_CR_MTG_CLC_CMD_LOOP_CNT_MAX                             (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_CLC_CMD_LOOP_CNT_DEF                             (0x00000000)
  #define MTG0_CR_MTG_CLC_CMD_LOOP_CNT_HSH                             (0x2000B1E4)

#define MTG0_CR_MTG_CRANDC_REG                                         (0x0000B1E8)
#define MTG0_CR_MTG_CRANDC_DEF                                         (0x00140000)
#define MTG0_CR_MTG_CRANDC_VOLATILE_BITFIELDS_MSK                      (0x00000000)

  #define MTG0_CR_MTG_CRANDC_CMD_RAND_MODE_OFF                         ( 0)
  #define MTG0_CR_MTG_CRANDC_CMD_RAND_MODE_WID                         ( 2)
  #define MTG0_CR_MTG_CRANDC_CMD_RAND_MODE_MSK                         (0x00000003)
  #define MTG0_CR_MTG_CRANDC_CMD_RAND_MODE_MIN                         (0)
  #define MTG0_CR_MTG_CRANDC_CMD_RAND_MODE_MAX                         (3) // 0x00000003
  #define MTG0_CR_MTG_CRANDC_CMD_RAND_MODE_DEF                         (0x00000000)
  #define MTG0_CR_MTG_CRANDC_CMD_RAND_MODE_HSH                         (0x0200B1E8)

  #define MTG0_CR_MTG_CRANDC_RSVD15_2_OFF                              ( 2)
  #define MTG0_CR_MTG_CRANDC_RSVD15_2_WID                              (14)
  #define MTG0_CR_MTG_CRANDC_RSVD15_2_MSK                              (0x0000FFFC)
  #define MTG0_CR_MTG_CRANDC_RSVD15_2_MIN                              (0)
  #define MTG0_CR_MTG_CRANDC_RSVD15_2_MAX                              (16383) // 0x00003FFF
  #define MTG0_CR_MTG_CRANDC_RSVD15_2_DEF                              (0x00000000)
  #define MTG0_CR_MTG_CRANDC_RSVD15_2_HSH                              (0x0E04B1E8)

  #define MTG0_CR_MTG_CRANDC_CMD_RAND_WIDTH_OFF                        (16)
  #define MTG0_CR_MTG_CRANDC_CMD_RAND_WIDTH_WID                        ( 4)
  #define MTG0_CR_MTG_CRANDC_CMD_RAND_WIDTH_MSK                        (0x000F0000)
  #define MTG0_CR_MTG_CRANDC_CMD_RAND_WIDTH_MIN                        (0)
  #define MTG0_CR_MTG_CRANDC_CMD_RAND_WIDTH_MAX                        (15) // 0x0000000F
  #define MTG0_CR_MTG_CRANDC_CMD_RAND_WIDTH_DEF                        (0x00000004)
  #define MTG0_CR_MTG_CRANDC_CMD_RAND_WIDTH_HSH                        (0x0420B1E8)

  #define MTG0_CR_MTG_CRANDC_CMD_RAND_SEED_OFF                         (20)
  #define MTG0_CR_MTG_CRANDC_CMD_RAND_SEED_WID                         (10)
  #define MTG0_CR_MTG_CRANDC_CMD_RAND_SEED_MSK                         (0x3FF00000)
  #define MTG0_CR_MTG_CRANDC_CMD_RAND_SEED_MIN                         (0)
  #define MTG0_CR_MTG_CRANDC_CMD_RAND_SEED_MAX                         (1023) // 0x000003FF
  #define MTG0_CR_MTG_CRANDC_CMD_RAND_SEED_DEF                         (0x00000001)
  #define MTG0_CR_MTG_CRANDC_CMD_RAND_SEED_HSH                         (0x0A28B1E8)

  #define MTG0_CR_MTG_CRANDC_RSVD31_30_OFF                             (30)
  #define MTG0_CR_MTG_CRANDC_RSVD31_30_WID                             ( 2)
  #define MTG0_CR_MTG_CRANDC_RSVD31_30_MSK                             (0xC0000000)
  #define MTG0_CR_MTG_CRANDC_RSVD31_30_MIN                             (0)
  #define MTG0_CR_MTG_CRANDC_RSVD31_30_MAX                             (3) // 0x00000003
  #define MTG0_CR_MTG_CRANDC_RSVD31_30_DEF                             (0x00000000)
  #define MTG0_CR_MTG_CRANDC_RSVD31_30_HSH                             (0x023CB1E8)

#define MTG0_CR_MTG_MWRC_REG                                           (0x0000B1EC)
#define MTG0_CR_MTG_MWRC_DEF                                           (0x00000010)
#define MTG0_CR_MTG_MWRC_VOLATILE_BITFIELDS_MSK                        (0x00000000)

  #define MTG0_CR_MTG_MWRC_MWR_MODE_OFF                                ( 0)
  #define MTG0_CR_MTG_MWRC_MWR_MODE_WID                                ( 2)
  #define MTG0_CR_MTG_MWRC_MWR_MODE_MSK                                (0x00000003)
  #define MTG0_CR_MTG_MWRC_MWR_MODE_MIN                                (0)
  #define MTG0_CR_MTG_MWRC_MWR_MODE_MAX                                (3) // 0x00000003
  #define MTG0_CR_MTG_MWRC_MWR_MODE_DEF                                (0x00000000)
  #define MTG0_CR_MTG_MWRC_MWR_MODE_HSH                                (0x0200B1EC)

  #define MTG0_CR_MTG_MWRC_RSVD3_2_OFF                                 ( 2)
  #define MTG0_CR_MTG_MWRC_RSVD3_2_WID                                 ( 2)
  #define MTG0_CR_MTG_MWRC_RSVD3_2_MSK                                 (0x0000000C)
  #define MTG0_CR_MTG_MWRC_RSVD3_2_MIN                                 (0)
  #define MTG0_CR_MTG_MWRC_RSVD3_2_MAX                                 (3) // 0x00000003
  #define MTG0_CR_MTG_MWRC_RSVD3_2_DEF                                 (0x00000000)
  #define MTG0_CR_MTG_MWRC_RSVD3_2_HSH                                 (0x0204B1EC)

  #define MTG0_CR_MTG_MWRC_MWR_RAND_SEED_OFF                           ( 4)
  #define MTG0_CR_MTG_MWRC_MWR_RAND_SEED_WID                           (10)
  #define MTG0_CR_MTG_MWRC_MWR_RAND_SEED_MSK                           (0x00003FF0)
  #define MTG0_CR_MTG_MWRC_MWR_RAND_SEED_MIN                           (0)
  #define MTG0_CR_MTG_MWRC_MWR_RAND_SEED_MAX                           (1023) // 0x000003FF
  #define MTG0_CR_MTG_MWRC_MWR_RAND_SEED_DEF                           (0x00000001)
  #define MTG0_CR_MTG_MWRC_MWR_RAND_SEED_HSH                           (0x0A08B1EC)

  #define MTG0_CR_MTG_MWRC_RSVD31_14_OFF                               (14)
  #define MTG0_CR_MTG_MWRC_RSVD31_14_WID                               (18)
  #define MTG0_CR_MTG_MWRC_RSVD31_14_MSK                               (0xFFFFC000)
  #define MTG0_CR_MTG_MWRC_RSVD31_14_MIN                               (0)
  #define MTG0_CR_MTG_MWRC_RSVD31_14_MAX                               (262143) // 0x0003FFFF
  #define MTG0_CR_MTG_MWRC_RSVD31_14_DEF                               (0x00000000)
  #define MTG0_CR_MTG_MWRC_RSVD31_14_HSH                               (0x121CB1EC)

#define MTG0_CR_MTG_MWRO_REG                                           (0x0000B1F0)
#define MTG0_CR_MTG_MWRO_DEF                                           (0x00000006)
#define MTG0_CR_MTG_MWRO_VOLATILE_BITFIELDS_MSK                        (0x00000000)

  #define MTG0_CR_MTG_MWRO_MWR_ORDER_OFF                               ( 0)
  #define MTG0_CR_MTG_MWRO_MWR_ORDER_WID                               (32)
  #define MTG0_CR_MTG_MWRO_MWR_ORDER_MSK                               (0xFFFFFFFF)
  #define MTG0_CR_MTG_MWRO_MWR_ORDER_MIN                               (0)
  #define MTG0_CR_MTG_MWRO_MWR_ORDER_MAX                               (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_MWRO_MWR_ORDER_DEF                               (0x00000006)
  #define MTG0_CR_MTG_MWRO_MWR_ORDER_HSH                               (0x2000B1F0)

#define MTG0_CR_MTG_ARMIN0_REG                                         (0x0000B200)
#define MTG0_CR_MTG_ARMIN0_DEF                                         (0x00000000)
#define MTG0_CR_MTG_ARMIN0_VOLATILE_BITFIELDS_MSK                      (0x00000000)

  #define MTG0_CR_MTG_ARMIN0_RANK_MIN_ADDR_OFF                         ( 0)
  #define MTG0_CR_MTG_ARMIN0_RANK_MIN_ADDR_WID                         ( 2)
  #define MTG0_CR_MTG_ARMIN0_RANK_MIN_ADDR_MSK                         (0x00000003)
  #define MTG0_CR_MTG_ARMIN0_RANK_MIN_ADDR_MIN                         (0)
  #define MTG0_CR_MTG_ARMIN0_RANK_MIN_ADDR_MAX                         (3) // 0x00000003
  #define MTG0_CR_MTG_ARMIN0_RANK_MIN_ADDR_DEF                         (0x00000000)
  #define MTG0_CR_MTG_ARMIN0_RANK_MIN_ADDR_HSH                         (0x0200B200)

  #define MTG0_CR_MTG_ARMIN0_RSVD3_2_OFF                               ( 2)
  #define MTG0_CR_MTG_ARMIN0_RSVD3_2_WID                               ( 2)
  #define MTG0_CR_MTG_ARMIN0_RSVD3_2_MSK                               (0x0000000C)
  #define MTG0_CR_MTG_ARMIN0_RSVD3_2_MIN                               (0)
  #define MTG0_CR_MTG_ARMIN0_RSVD3_2_MAX                               (3) // 0x00000003
  #define MTG0_CR_MTG_ARMIN0_RSVD3_2_DEF                               (0x00000000)
  #define MTG0_CR_MTG_ARMIN0_RSVD3_2_HSH                               (0x0204B200)

  #define MTG0_CR_MTG_ARMIN0_BANK_MIN_ADDR_OFF                         ( 4)
  #define MTG0_CR_MTG_ARMIN0_BANK_MIN_ADDR_WID                         ( 5)
  #define MTG0_CR_MTG_ARMIN0_BANK_MIN_ADDR_MSK                         (0x000001F0)
  #define MTG0_CR_MTG_ARMIN0_BANK_MIN_ADDR_MIN                         (0)
  #define MTG0_CR_MTG_ARMIN0_BANK_MIN_ADDR_MAX                         (31) // 0x0000001F
  #define MTG0_CR_MTG_ARMIN0_BANK_MIN_ADDR_DEF                         (0x00000000)
  #define MTG0_CR_MTG_ARMIN0_BANK_MIN_ADDR_HSH                         (0x0508B200)

  #define MTG0_CR_MTG_ARMIN0_RSVD10_9_OFF                              ( 9)
  #define MTG0_CR_MTG_ARMIN0_RSVD10_9_WID                              ( 2)
  #define MTG0_CR_MTG_ARMIN0_RSVD10_9_MSK                              (0x00000600)
  #define MTG0_CR_MTG_ARMIN0_RSVD10_9_MIN                              (0)
  #define MTG0_CR_MTG_ARMIN0_RSVD10_9_MAX                              (3) // 0x00000003
  #define MTG0_CR_MTG_ARMIN0_RSVD10_9_DEF                              (0x00000000)
  #define MTG0_CR_MTG_ARMIN0_RSVD10_9_HSH                              (0x0212B200)

  #define MTG0_CR_MTG_ARMIN0_COL_MIN_ADDR_OFF                          (11)
  #define MTG0_CR_MTG_ARMIN0_COL_MIN_ADDR_WID                          (10)
  #define MTG0_CR_MTG_ARMIN0_COL_MIN_ADDR_MSK                          (0x001FF800)
  #define MTG0_CR_MTG_ARMIN0_COL_MIN_ADDR_MIN                          (0)
  #define MTG0_CR_MTG_ARMIN0_COL_MIN_ADDR_MAX                          (1023) // 0x000003FF
  #define MTG0_CR_MTG_ARMIN0_COL_MIN_ADDR_DEF                          (0x00000000)
  #define MTG0_CR_MTG_ARMIN0_COL_MIN_ADDR_HSH                          (0x0A16B200)

  #define MTG0_CR_MTG_ARMIN0_RSVD23_21_OFF                             (21)
  #define MTG0_CR_MTG_ARMIN0_RSVD23_21_WID                             ( 3)
  #define MTG0_CR_MTG_ARMIN0_RSVD23_21_MSK                             (0x00E00000)
  #define MTG0_CR_MTG_ARMIN0_RSVD23_21_MIN                             (0)
  #define MTG0_CR_MTG_ARMIN0_RSVD23_21_MAX                             (7) // 0x00000007
  #define MTG0_CR_MTG_ARMIN0_RSVD23_21_DEF                             (0x00000000)
  #define MTG0_CR_MTG_ARMIN0_RSVD23_21_HSH                             (0x032AB200)

  #define MTG0_CR_MTG_ARMIN0_LRANK_MIN_ADDR_OFF                        (24)
  #define MTG0_CR_MTG_ARMIN0_LRANK_MIN_ADDR_WID                        ( 4)
  #define MTG0_CR_MTG_ARMIN0_LRANK_MIN_ADDR_MSK                        (0x0F000000)
  #define MTG0_CR_MTG_ARMIN0_LRANK_MIN_ADDR_MIN                        (0)
  #define MTG0_CR_MTG_ARMIN0_LRANK_MIN_ADDR_MAX                        (15) // 0x0000000F
  #define MTG0_CR_MTG_ARMIN0_LRANK_MIN_ADDR_DEF                        (0x00000000)
  #define MTG0_CR_MTG_ARMIN0_LRANK_MIN_ADDR_HSH                        (0x0430B200)

  #define MTG0_CR_MTG_ARMIN0_RSVD31_28_OFF                             (28)
  #define MTG0_CR_MTG_ARMIN0_RSVD31_28_WID                             ( 4)
  #define MTG0_CR_MTG_ARMIN0_RSVD31_28_MSK                             (0xF0000000)
  #define MTG0_CR_MTG_ARMIN0_RSVD31_28_MIN                             (0)
  #define MTG0_CR_MTG_ARMIN0_RSVD31_28_MAX                             (15) // 0x0000000F
  #define MTG0_CR_MTG_ARMIN0_RSVD31_28_DEF                             (0x00000000)
  #define MTG0_CR_MTG_ARMIN0_RSVD31_28_HSH                             (0x0438B200)

#define MTG0_CR_MTG_ARMIN1_REG                                         (0x0000B204)
#define MTG0_CR_MTG_ARMIN1_DEF                                         (0x00000000)
#define MTG0_CR_MTG_ARMIN1_VOLATILE_BITFIELDS_MSK                      (0x00000000)

  #define MTG0_CR_MTG_ARMIN1_ROW_MIN_ADDR_OFF                          ( 0)
  #define MTG0_CR_MTG_ARMIN1_ROW_MIN_ADDR_WID                          (18)
  #define MTG0_CR_MTG_ARMIN1_ROW_MIN_ADDR_MSK                          (0x0003FFFF)
  #define MTG0_CR_MTG_ARMIN1_ROW_MIN_ADDR_MIN                          (0)
  #define MTG0_CR_MTG_ARMIN1_ROW_MIN_ADDR_MAX                          (262143) // 0x0003FFFF
  #define MTG0_CR_MTG_ARMIN1_ROW_MIN_ADDR_DEF                          (0x00000000)
  #define MTG0_CR_MTG_ARMIN1_ROW_MIN_ADDR_HSH                          (0x1200B204)

  #define MTG0_CR_MTG_ARMIN1_RSVD31_18_OFF                             (18)
  #define MTG0_CR_MTG_ARMIN1_RSVD31_18_WID                             (14)
  #define MTG0_CR_MTG_ARMIN1_RSVD31_18_MSK                             (0xFFFC0000)
  #define MTG0_CR_MTG_ARMIN1_RSVD31_18_MIN                             (0)
  #define MTG0_CR_MTG_ARMIN1_RSVD31_18_MAX                             (16383) // 0x00003FFF
  #define MTG0_CR_MTG_ARMIN1_RSVD31_18_DEF                             (0x00000000)
  #define MTG0_CR_MTG_ARMIN1_RSVD31_18_HSH                             (0x0E24B204)

#define MTG0_CR_MTG_ARMAX0_REG                                         (0x0000B208)
#define MTG0_CR_MTG_ARMAX0_DEF                                         (0x0301F873)
#define MTG0_CR_MTG_ARMAX0_VOLATILE_BITFIELDS_MSK                      (0x00000000)

  #define MTG0_CR_MTG_ARMAX0_RANK_MAX_ADDR_OFF                         ( 0)
  #define MTG0_CR_MTG_ARMAX0_RANK_MAX_ADDR_WID                         ( 2)
  #define MTG0_CR_MTG_ARMAX0_RANK_MAX_ADDR_MSK                         (0x00000003)
  #define MTG0_CR_MTG_ARMAX0_RANK_MAX_ADDR_MIN                         (0)
  #define MTG0_CR_MTG_ARMAX0_RANK_MAX_ADDR_MAX                         (3) // 0x00000003
  #define MTG0_CR_MTG_ARMAX0_RANK_MAX_ADDR_DEF                         (0x00000003)
  #define MTG0_CR_MTG_ARMAX0_RANK_MAX_ADDR_HSH                         (0x0200B208)

  #define MTG0_CR_MTG_ARMAX0_RSVD3_2_OFF                               ( 2)
  #define MTG0_CR_MTG_ARMAX0_RSVD3_2_WID                               ( 2)
  #define MTG0_CR_MTG_ARMAX0_RSVD3_2_MSK                               (0x0000000C)
  #define MTG0_CR_MTG_ARMAX0_RSVD3_2_MIN                               (0)
  #define MTG0_CR_MTG_ARMAX0_RSVD3_2_MAX                               (3) // 0x00000003
  #define MTG0_CR_MTG_ARMAX0_RSVD3_2_DEF                               (0x00000000)
  #define MTG0_CR_MTG_ARMAX0_RSVD3_2_HSH                               (0x0204B208)

  #define MTG0_CR_MTG_ARMAX0_BANK_MAX_ADDR_OFF                         ( 4)
  #define MTG0_CR_MTG_ARMAX0_BANK_MAX_ADDR_WID                         ( 5)
  #define MTG0_CR_MTG_ARMAX0_BANK_MAX_ADDR_MSK                         (0x000001F0)
  #define MTG0_CR_MTG_ARMAX0_BANK_MAX_ADDR_MIN                         (0)
  #define MTG0_CR_MTG_ARMAX0_BANK_MAX_ADDR_MAX                         (31) // 0x0000001F
  #define MTG0_CR_MTG_ARMAX0_BANK_MAX_ADDR_DEF                         (0x00000007)
  #define MTG0_CR_MTG_ARMAX0_BANK_MAX_ADDR_HSH                         (0x0508B208)

  #define MTG0_CR_MTG_ARMAX0_RSVD10_9_OFF                              ( 9)
  #define MTG0_CR_MTG_ARMAX0_RSVD10_9_WID                              ( 2)
  #define MTG0_CR_MTG_ARMAX0_RSVD10_9_MSK                              (0x00000600)
  #define MTG0_CR_MTG_ARMAX0_RSVD10_9_MIN                              (0)
  #define MTG0_CR_MTG_ARMAX0_RSVD10_9_MAX                              (3) // 0x00000003
  #define MTG0_CR_MTG_ARMAX0_RSVD10_9_DEF                              (0x00000000)
  #define MTG0_CR_MTG_ARMAX0_RSVD10_9_HSH                              (0x0212B208)

  #define MTG0_CR_MTG_ARMAX0_COL_MAX_ADDR_OFF                          (11)
  #define MTG0_CR_MTG_ARMAX0_COL_MAX_ADDR_WID                          (10)
  #define MTG0_CR_MTG_ARMAX0_COL_MAX_ADDR_MSK                          (0x001FF800)
  #define MTG0_CR_MTG_ARMAX0_COL_MAX_ADDR_MIN                          (0)
  #define MTG0_CR_MTG_ARMAX0_COL_MAX_ADDR_MAX                          (1023) // 0x000003FF
  #define MTG0_CR_MTG_ARMAX0_COL_MAX_ADDR_DEF                          (0x0000003F)
  #define MTG0_CR_MTG_ARMAX0_COL_MAX_ADDR_HSH                          (0x0A16B208)

  #define MTG0_CR_MTG_ARMAX0_RSVD23_21_OFF                             (21)
  #define MTG0_CR_MTG_ARMAX0_RSVD23_21_WID                             ( 3)
  #define MTG0_CR_MTG_ARMAX0_RSVD23_21_MSK                             (0x00E00000)
  #define MTG0_CR_MTG_ARMAX0_RSVD23_21_MIN                             (0)
  #define MTG0_CR_MTG_ARMAX0_RSVD23_21_MAX                             (7) // 0x00000007
  #define MTG0_CR_MTG_ARMAX0_RSVD23_21_DEF                             (0x00000000)
  #define MTG0_CR_MTG_ARMAX0_RSVD23_21_HSH                             (0x032AB208)

  #define MTG0_CR_MTG_ARMAX0_LRANK_MAX_ADDR_OFF                        (24)
  #define MTG0_CR_MTG_ARMAX0_LRANK_MAX_ADDR_WID                        ( 4)
  #define MTG0_CR_MTG_ARMAX0_LRANK_MAX_ADDR_MSK                        (0x0F000000)
  #define MTG0_CR_MTG_ARMAX0_LRANK_MAX_ADDR_MIN                        (0)
  #define MTG0_CR_MTG_ARMAX0_LRANK_MAX_ADDR_MAX                        (15) // 0x0000000F
  #define MTG0_CR_MTG_ARMAX0_LRANK_MAX_ADDR_DEF                        (0x00000003)
  #define MTG0_CR_MTG_ARMAX0_LRANK_MAX_ADDR_HSH                        (0x0430B208)

  #define MTG0_CR_MTG_ARMAX0_RSVD31_28_OFF                             (28)
  #define MTG0_CR_MTG_ARMAX0_RSVD31_28_WID                             ( 4)
  #define MTG0_CR_MTG_ARMAX0_RSVD31_28_MSK                             (0xF0000000)
  #define MTG0_CR_MTG_ARMAX0_RSVD31_28_MIN                             (0)
  #define MTG0_CR_MTG_ARMAX0_RSVD31_28_MAX                             (15) // 0x0000000F
  #define MTG0_CR_MTG_ARMAX0_RSVD31_28_DEF                             (0x00000000)
  #define MTG0_CR_MTG_ARMAX0_RSVD31_28_HSH                             (0x0438B208)

#define MTG0_CR_MTG_ARMAX1_REG                                         (0x0000B20C)
#define MTG0_CR_MTG_ARMAX1_DEF                                         (0x00001FFF)
#define MTG0_CR_MTG_ARMAX1_VOLATILE_BITFIELDS_MSK                      (0x00000000)

  #define MTG0_CR_MTG_ARMAX1_ROW_MAX_ADDR_OFF                          ( 0)
  #define MTG0_CR_MTG_ARMAX1_ROW_MAX_ADDR_WID                          (18)
  #define MTG0_CR_MTG_ARMAX1_ROW_MAX_ADDR_MSK                          (0x0003FFFF)
  #define MTG0_CR_MTG_ARMAX1_ROW_MAX_ADDR_MIN                          (0)
  #define MTG0_CR_MTG_ARMAX1_ROW_MAX_ADDR_MAX                          (262143) // 0x0003FFFF
  #define MTG0_CR_MTG_ARMAX1_ROW_MAX_ADDR_DEF                          (0x00001FFF)
  #define MTG0_CR_MTG_ARMAX1_ROW_MAX_ADDR_HSH                          (0x1200B20C)

  #define MTG0_CR_MTG_ARMAX1_RSVD31_18_OFF                             (18)
  #define MTG0_CR_MTG_ARMAX1_RSVD31_18_WID                             (14)
  #define MTG0_CR_MTG_ARMAX1_RSVD31_18_MSK                             (0xFFFC0000)
  #define MTG0_CR_MTG_ARMAX1_RSVD31_18_MIN                             (0)
  #define MTG0_CR_MTG_ARMAX1_RSVD31_18_MAX                             (16383) // 0x00003FFF
  #define MTG0_CR_MTG_ARMAX1_RSVD31_18_DEF                             (0x00000000)
  #define MTG0_CR_MTG_ARMAX1_RSVD31_18_HSH                             (0x0E24B20C)

#define MTG0_CR_MTG_L2PRM_REG                                          (0x0000B210)
#define MTG0_CR_MTG_L2PRM_DEF                                          (0x00E4E4E4)
#define MTG0_CR_MTG_L2PRM_VOLATILE_BITFIELDS_MSK                       (0x00000000)

  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP0_OFF                          ( 0)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP0_WID                          ( 2)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP0_MSK                          (0x00000003)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP0_MIN                          (0)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP0_MAX                          (3) // 0x00000003
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP0_DEF                          (0x00000000)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP0_HSH                          (0x0200B210)

  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP1_OFF                          ( 2)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP1_WID                          ( 2)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP1_MSK                          (0x0000000C)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP1_MIN                          (0)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP1_MAX                          (3) // 0x00000003
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP1_DEF                          (0x00000001)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP1_HSH                          (0x0204B210)

  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP2_OFF                          ( 4)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP2_WID                          ( 2)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP2_MSK                          (0x00000030)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP2_MIN                          (0)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP2_MAX                          (3) // 0x00000003
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP2_DEF                          (0x00000002)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP2_HSH                          (0x0208B210)

  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP3_OFF                          ( 6)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP3_WID                          ( 2)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP3_MSK                          (0x000000C0)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP3_MIN                          (0)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP3_MAX                          (3) // 0x00000003
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP3_DEF                          (0x00000003)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP3_HSH                          (0x020CB210)

  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP4_OFF                          ( 8)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP4_WID                          ( 2)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP4_MSK                          (0x00000300)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP4_MIN                          (0)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP4_MAX                          (3) // 0x00000003
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP4_DEF                          (0x00000000)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP4_HSH                          (0x0210B210)

  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP5_OFF                          (10)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP5_WID                          ( 2)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP5_MSK                          (0x00000C00)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP5_MIN                          (0)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP5_MAX                          (3) // 0x00000003
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP5_DEF                          (0x00000001)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP5_HSH                          (0x0214B210)

  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP6_OFF                          (12)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP6_WID                          ( 2)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP6_MSK                          (0x00003000)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP6_MIN                          (0)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP6_MAX                          (3) // 0x00000003
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP6_DEF                          (0x00000002)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP6_HSH                          (0x0218B210)

  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP7_OFF                          (14)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP7_WID                          ( 2)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP7_MSK                          (0x0000C000)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP7_MIN                          (0)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP7_MAX                          (3) // 0x00000003
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP7_DEF                          (0x00000003)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP7_HSH                          (0x021CB210)

  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP8_OFF                          (16)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP8_WID                          ( 2)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP8_MSK                          (0x00030000)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP8_MIN                          (0)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP8_MAX                          (3) // 0x00000003
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP8_DEF                          (0x00000000)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP8_HSH                          (0x0220B210)

  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP9_OFF                          (18)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP9_WID                          ( 2)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP9_MSK                          (0x000C0000)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP9_MIN                          (0)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP9_MAX                          (3) // 0x00000003
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP9_DEF                          (0x00000001)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP9_HSH                          (0x0224B210)

  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP10_OFF                         (20)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP10_WID                         ( 2)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP10_MSK                         (0x00300000)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP10_MIN                         (0)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP10_MAX                         (3) // 0x00000003
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP10_DEF                         (0x00000002)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP10_HSH                         (0x0228B210)

  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP11_OFF                         (22)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP11_WID                         ( 2)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP11_MSK                         (0x00C00000)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP11_MIN                         (0)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP11_MAX                         (3) // 0x00000003
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP11_DEF                         (0x00000003)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP11_HSH                         (0x022CB210)

  #define MTG0_CR_MTG_L2PRM_RSVD30_24_OFF                              (24)
  #define MTG0_CR_MTG_L2PRM_RSVD30_24_WID                              ( 7)
  #define MTG0_CR_MTG_L2PRM_RSVD30_24_MSK                              (0x7F000000)
  #define MTG0_CR_MTG_L2PRM_RSVD30_24_MIN                              (0)
  #define MTG0_CR_MTG_L2PRM_RSVD30_24_MAX                              (127) // 0x0000007F
  #define MTG0_CR_MTG_L2PRM_RSVD30_24_DEF                              (0x00000000)
  #define MTG0_CR_MTG_L2PRM_RSVD30_24_HSH                              (0x0730B210)

  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP_EN_OFF                        (31)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP_EN_WID                        ( 1)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP_EN_MSK                        (0x80000000)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP_EN_MIN                        (0)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP_EN_MAX                        (1) // 0x00000001
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP_EN_DEF                        (0x00000000)
  #define MTG0_CR_MTG_L2PRM_L2P_RANK_MAP_EN_HSH                        (0x013EB210)

#define MTG0_CR_MTG_AO_REG                                             (0x0000B220)
#define MTG0_CR_MTG_AO_DEF                                             (0x00000000)
#define MTG0_CR_MTG_AO_VOLATILE_BITFIELDS_MSK                          (0x00000000)

  #define MTG0_CR_MTG_AO_ADDR_ORDER_MODE_OFF                           ( 0)
  #define MTG0_CR_MTG_AO_ADDR_ORDER_MODE_WID                           ( 1)
  #define MTG0_CR_MTG_AO_ADDR_ORDER_MODE_MSK                           (0x00000001)
  #define MTG0_CR_MTG_AO_ADDR_ORDER_MODE_MIN                           (0)
  #define MTG0_CR_MTG_AO_ADDR_ORDER_MODE_MAX                           (1) // 0x00000001
  #define MTG0_CR_MTG_AO_ADDR_ORDER_MODE_DEF                           (0x00000000)
  #define MTG0_CR_MTG_AO_ADDR_ORDER_MODE_HSH                           (0x0100B220)

  #define MTG0_CR_MTG_AO_RSVD3_1_OFF                                   ( 1)
  #define MTG0_CR_MTG_AO_RSVD3_1_WID                                   ( 3)
  #define MTG0_CR_MTG_AO_RSVD3_1_MSK                                   (0x0000000E)
  #define MTG0_CR_MTG_AO_RSVD3_1_MIN                                   (0)
  #define MTG0_CR_MTG_AO_RSVD3_1_MAX                                   (7) // 0x00000007
  #define MTG0_CR_MTG_AO_RSVD3_1_DEF                                   (0x00000000)
  #define MTG0_CR_MTG_AO_RSVD3_1_HSH                                   (0x0302B220)

  #define MTG0_CR_MTG_AO_ADDR_ORDER_OFF                                ( 4)
  #define MTG0_CR_MTG_AO_ADDR_ORDER_WID                                ( 4)
  #define MTG0_CR_MTG_AO_ADDR_ORDER_MSK                                (0x000000F0)
  #define MTG0_CR_MTG_AO_ADDR_ORDER_MIN                                (0)
  #define MTG0_CR_MTG_AO_ADDR_ORDER_MAX                                (15) // 0x0000000F
  #define MTG0_CR_MTG_AO_ADDR_ORDER_DEF                                (0x00000000)
  #define MTG0_CR_MTG_AO_ADDR_ORDER_HSH                                (0x0408B220)

  #define MTG0_CR_MTG_AO_BANK_INC_ORDER_OFF                            ( 8)
  #define MTG0_CR_MTG_AO_BANK_INC_ORDER_WID                            ( 3)
  #define MTG0_CR_MTG_AO_BANK_INC_ORDER_MSK                            (0x00000700)
  #define MTG0_CR_MTG_AO_BANK_INC_ORDER_MIN                            (0)
  #define MTG0_CR_MTG_AO_BANK_INC_ORDER_MAX                            (7) // 0x00000007
  #define MTG0_CR_MTG_AO_BANK_INC_ORDER_DEF                            (0x00000000)
  #define MTG0_CR_MTG_AO_BANK_INC_ORDER_HSH                            (0x0310B220)

  #define MTG0_CR_MTG_AO_RSVD11_OFF                                    (11)
  #define MTG0_CR_MTG_AO_RSVD11_WID                                    ( 1)
  #define MTG0_CR_MTG_AO_RSVD11_MSK                                    (0x00000800)
  #define MTG0_CR_MTG_AO_RSVD11_MIN                                    (0)
  #define MTG0_CR_MTG_AO_RSVD11_MAX                                    (1) // 0x00000001
  #define MTG0_CR_MTG_AO_RSVD11_DEF                                    (0x00000000)
  #define MTG0_CR_MTG_AO_RSVD11_HSH                                    (0x0116B220)

  #define MTG0_CR_MTG_AO_RANK_INC_OFF                                  (12)
  #define MTG0_CR_MTG_AO_RANK_INC_WID                                  ( 1)
  #define MTG0_CR_MTG_AO_RANK_INC_MSK                                  (0x00001000)
  #define MTG0_CR_MTG_AO_RANK_INC_MIN                                  (0)
  #define MTG0_CR_MTG_AO_RANK_INC_MAX                                  (1) // 0x00000001
  #define MTG0_CR_MTG_AO_RANK_INC_DEF                                  (0x00000000)
  #define MTG0_CR_MTG_AO_RANK_INC_HSH                                  (0x0118B220)

  #define MTG0_CR_MTG_AO_RSVD13_OFF                                    (13)
  #define MTG0_CR_MTG_AO_RSVD13_WID                                    ( 1)
  #define MTG0_CR_MTG_AO_RSVD13_MSK                                    (0x00002000)
  #define MTG0_CR_MTG_AO_RSVD13_MIN                                    (0)
  #define MTG0_CR_MTG_AO_RSVD13_MAX                                    (1) // 0x00000001
  #define MTG0_CR_MTG_AO_RSVD13_DEF                                    (0x00000000)
  #define MTG0_CR_MTG_AO_RSVD13_HSH                                    (0x011AB220)

  #define MTG0_CR_MTG_AO_BANK_INC_OFF                                  (14)
  #define MTG0_CR_MTG_AO_BANK_INC_WID                                  ( 2)
  #define MTG0_CR_MTG_AO_BANK_INC_MSK                                  (0x0000C000)
  #define MTG0_CR_MTG_AO_BANK_INC_MIN                                  (0)
  #define MTG0_CR_MTG_AO_BANK_INC_MAX                                  (3) // 0x00000003
  #define MTG0_CR_MTG_AO_BANK_INC_DEF                                  (0x00000000)
  #define MTG0_CR_MTG_AO_BANK_INC_HSH                                  (0x021CB220)

  #define MTG0_CR_MTG_AO_RSVD16_OFF                                    (16)
  #define MTG0_CR_MTG_AO_RSVD16_WID                                    ( 1)
  #define MTG0_CR_MTG_AO_RSVD16_MSK                                    (0x00010000)
  #define MTG0_CR_MTG_AO_RSVD16_MIN                                    (0)
  #define MTG0_CR_MTG_AO_RSVD16_MAX                                    (1) // 0x00000001
  #define MTG0_CR_MTG_AO_RSVD16_DEF                                    (0x00000000)
  #define MTG0_CR_MTG_AO_RSVD16_HSH                                    (0x0120B220)

  #define MTG0_CR_MTG_AO_ROW_INC_OFF                                   (17)
  #define MTG0_CR_MTG_AO_ROW_INC_WID                                   ( 4)
  #define MTG0_CR_MTG_AO_ROW_INC_MSK                                   (0x001E0000)
  #define MTG0_CR_MTG_AO_ROW_INC_MIN                                   (0)
  #define MTG0_CR_MTG_AO_ROW_INC_MAX                                   (15) // 0x0000000F
  #define MTG0_CR_MTG_AO_ROW_INC_DEF                                   (0x00000000)
  #define MTG0_CR_MTG_AO_ROW_INC_HSH                                   (0x0422B220)

  #define MTG0_CR_MTG_AO_RSVD21_OFF                                    (21)
  #define MTG0_CR_MTG_AO_RSVD21_WID                                    ( 1)
  #define MTG0_CR_MTG_AO_RSVD21_MSK                                    (0x00200000)
  #define MTG0_CR_MTG_AO_RSVD21_MIN                                    (0)
  #define MTG0_CR_MTG_AO_RSVD21_MAX                                    (1) // 0x00000001
  #define MTG0_CR_MTG_AO_RSVD21_DEF                                    (0x00000000)
  #define MTG0_CR_MTG_AO_RSVD21_HSH                                    (0x012AB220)

  #define MTG0_CR_MTG_AO_COL_INC_OFF                                   (22)
  #define MTG0_CR_MTG_AO_COL_INC_WID                                   ( 3)
  #define MTG0_CR_MTG_AO_COL_INC_MSK                                   (0x01C00000)
  #define MTG0_CR_MTG_AO_COL_INC_MIN                                   (0)
  #define MTG0_CR_MTG_AO_COL_INC_MAX                                   (7) // 0x00000007
  #define MTG0_CR_MTG_AO_COL_INC_DEF                                   (0x00000000)
  #define MTG0_CR_MTG_AO_COL_INC_HSH                                   (0x032CB220)

  #define MTG0_CR_MTG_AO_RSVD27_25_OFF                                 (25)
  #define MTG0_CR_MTG_AO_RSVD27_25_WID                                 ( 3)
  #define MTG0_CR_MTG_AO_RSVD27_25_MSK                                 (0x0E000000)
  #define MTG0_CR_MTG_AO_RSVD27_25_MIN                                 (0)
  #define MTG0_CR_MTG_AO_RSVD27_25_MAX                                 (7) // 0x00000007
  #define MTG0_CR_MTG_AO_RSVD27_25_DEF                                 (0x00000000)
  #define MTG0_CR_MTG_AO_RSVD27_25_HSH                                 (0x0332B220)

  #define MTG0_CR_MTG_AO_ALWAYS_INC_RANK_ADDR_OFF                      (28)
  #define MTG0_CR_MTG_AO_ALWAYS_INC_RANK_ADDR_WID                      ( 1)
  #define MTG0_CR_MTG_AO_ALWAYS_INC_RANK_ADDR_MSK                      (0x10000000)
  #define MTG0_CR_MTG_AO_ALWAYS_INC_RANK_ADDR_MIN                      (0)
  #define MTG0_CR_MTG_AO_ALWAYS_INC_RANK_ADDR_MAX                      (1) // 0x00000001
  #define MTG0_CR_MTG_AO_ALWAYS_INC_RANK_ADDR_DEF                      (0x00000000)
  #define MTG0_CR_MTG_AO_ALWAYS_INC_RANK_ADDR_HSH                      (0x0138B220)

  #define MTG0_CR_MTG_AO_ALWAYS_INC_BANK_ADDR_OFF                      (29)
  #define MTG0_CR_MTG_AO_ALWAYS_INC_BANK_ADDR_WID                      ( 1)
  #define MTG0_CR_MTG_AO_ALWAYS_INC_BANK_ADDR_MSK                      (0x20000000)
  #define MTG0_CR_MTG_AO_ALWAYS_INC_BANK_ADDR_MIN                      (0)
  #define MTG0_CR_MTG_AO_ALWAYS_INC_BANK_ADDR_MAX                      (1) // 0x00000001
  #define MTG0_CR_MTG_AO_ALWAYS_INC_BANK_ADDR_DEF                      (0x00000000)
  #define MTG0_CR_MTG_AO_ALWAYS_INC_BANK_ADDR_HSH                      (0x013AB220)

  #define MTG0_CR_MTG_AO_ALWAYS_INC_ROW_ADDR_OFF                       (30)
  #define MTG0_CR_MTG_AO_ALWAYS_INC_ROW_ADDR_WID                       ( 1)
  #define MTG0_CR_MTG_AO_ALWAYS_INC_ROW_ADDR_MSK                       (0x40000000)
  #define MTG0_CR_MTG_AO_ALWAYS_INC_ROW_ADDR_MIN                       (0)
  #define MTG0_CR_MTG_AO_ALWAYS_INC_ROW_ADDR_MAX                       (1) // 0x00000001
  #define MTG0_CR_MTG_AO_ALWAYS_INC_ROW_ADDR_DEF                       (0x00000000)
  #define MTG0_CR_MTG_AO_ALWAYS_INC_ROW_ADDR_HSH                       (0x013CB220)

  #define MTG0_CR_MTG_AO_ALWAYS_INC_COL_ADDR_OFF                       (31)
  #define MTG0_CR_MTG_AO_ALWAYS_INC_COL_ADDR_WID                       ( 1)
  #define MTG0_CR_MTG_AO_ALWAYS_INC_COL_ADDR_MSK                       (0x80000000)
  #define MTG0_CR_MTG_AO_ALWAYS_INC_COL_ADDR_MIN                       (0)
  #define MTG0_CR_MTG_AO_ALWAYS_INC_COL_ADDR_MAX                       (1) // 0x00000001
  #define MTG0_CR_MTG_AO_ALWAYS_INC_COL_ADDR_DEF                       (0x00000000)
  #define MTG0_CR_MTG_AO_ALWAYS_INC_COL_ADDR_HSH                       (0x013EB220)

#define MTG0_CR_MTG_ARC_REG                                            (0x0000B224)
#define MTG0_CR_MTG_ARC_DEF                                            (0x0C71A000)
#define MTG0_CR_MTG_ARC_VOLATILE_BITFIELDS_MSK                         (0x00000000)

  #define MTG0_CR_MTG_ARC_ADDR_RAND_MODE_OFF                           ( 0)
  #define MTG0_CR_MTG_ARC_ADDR_RAND_MODE_WID                           ( 1)
  #define MTG0_CR_MTG_ARC_ADDR_RAND_MODE_MSK                           (0x00000001)
  #define MTG0_CR_MTG_ARC_ADDR_RAND_MODE_MIN                           (0)
  #define MTG0_CR_MTG_ARC_ADDR_RAND_MODE_MAX                           (1) // 0x00000001
  #define MTG0_CR_MTG_ARC_ADDR_RAND_MODE_DEF                           (0x00000000)
  #define MTG0_CR_MTG_ARC_ADDR_RAND_MODE_HSH                           (0x0100B224)

  #define MTG0_CR_MTG_ARC_RSVD4_1_OFF                                  ( 1)
  #define MTG0_CR_MTG_ARC_RSVD4_1_WID                                  ( 4)
  #define MTG0_CR_MTG_ARC_RSVD4_1_MSK                                  (0x0000001E)
  #define MTG0_CR_MTG_ARC_RSVD4_1_MIN                                  (0)
  #define MTG0_CR_MTG_ARC_RSVD4_1_MAX                                  (15) // 0x0000000F
  #define MTG0_CR_MTG_ARC_RSVD4_1_DEF                                  (0x00000000)
  #define MTG0_CR_MTG_ARC_RSVD4_1_HSH                                  (0x0402B224)

  #define MTG0_CR_MTG_ARC_BANK_RAND_EN_OFF                             ( 5)
  #define MTG0_CR_MTG_ARC_BANK_RAND_EN_WID                             ( 1)
  #define MTG0_CR_MTG_ARC_BANK_RAND_EN_MSK                             (0x00000020)
  #define MTG0_CR_MTG_ARC_BANK_RAND_EN_MIN                             (0)
  #define MTG0_CR_MTG_ARC_BANK_RAND_EN_MAX                             (1) // 0x00000001
  #define MTG0_CR_MTG_ARC_BANK_RAND_EN_DEF                             (0x00000000)
  #define MTG0_CR_MTG_ARC_BANK_RAND_EN_HSH                             (0x010AB224)

  #define MTG0_CR_MTG_ARC_ROW_RAND_EN_OFF                              ( 6)
  #define MTG0_CR_MTG_ARC_ROW_RAND_EN_WID                              ( 1)
  #define MTG0_CR_MTG_ARC_ROW_RAND_EN_MSK                              (0x00000040)
  #define MTG0_CR_MTG_ARC_ROW_RAND_EN_MIN                              (0)
  #define MTG0_CR_MTG_ARC_ROW_RAND_EN_MAX                              (1) // 0x00000001
  #define MTG0_CR_MTG_ARC_ROW_RAND_EN_DEF                              (0x00000000)
  #define MTG0_CR_MTG_ARC_ROW_RAND_EN_HSH                              (0x010CB224)

  #define MTG0_CR_MTG_ARC_COL_RAND_EN_OFF                              ( 7)
  #define MTG0_CR_MTG_ARC_COL_RAND_EN_WID                              ( 1)
  #define MTG0_CR_MTG_ARC_COL_RAND_EN_MSK                              (0x00000080)
  #define MTG0_CR_MTG_ARC_COL_RAND_EN_MIN                              (0)
  #define MTG0_CR_MTG_ARC_COL_RAND_EN_MAX                              (1) // 0x00000001
  #define MTG0_CR_MTG_ARC_COL_RAND_EN_DEF                              (0x00000000)
  #define MTG0_CR_MTG_ARC_COL_RAND_EN_HSH                              (0x010EB224)

  #define MTG0_CR_MTG_ARC_RSVD11_8_OFF                                 ( 8)
  #define MTG0_CR_MTG_ARC_RSVD11_8_WID                                 ( 4)
  #define MTG0_CR_MTG_ARC_RSVD11_8_MSK                                 (0x00000F00)
  #define MTG0_CR_MTG_ARC_RSVD11_8_MIN                                 (0)
  #define MTG0_CR_MTG_ARC_RSVD11_8_MAX                                 (15) // 0x0000000F
  #define MTG0_CR_MTG_ARC_RSVD11_8_DEF                                 (0x00000000)
  #define MTG0_CR_MTG_ARC_RSVD11_8_HSH                                 (0x0410B224)

  #define MTG0_CR_MTG_ARC_RAND_RANK_WIDTH_OFF                          (12)
  #define MTG0_CR_MTG_ARC_RAND_RANK_WIDTH_WID                          ( 2)
  #define MTG0_CR_MTG_ARC_RAND_RANK_WIDTH_MSK                          (0x00003000)
  #define MTG0_CR_MTG_ARC_RAND_RANK_WIDTH_MIN                          (0)
  #define MTG0_CR_MTG_ARC_RAND_RANK_WIDTH_MAX                          (3) // 0x00000003
  #define MTG0_CR_MTG_ARC_RAND_RANK_WIDTH_DEF                          (0x00000002)
  #define MTG0_CR_MTG_ARC_RAND_RANK_WIDTH_HSH                          (0x0218B224)

  #define MTG0_CR_MTG_ARC_RSVD14_OFF                                   (14)
  #define MTG0_CR_MTG_ARC_RSVD14_WID                                   ( 1)
  #define MTG0_CR_MTG_ARC_RSVD14_MSK                                   (0x00004000)
  #define MTG0_CR_MTG_ARC_RSVD14_MIN                                   (0)
  #define MTG0_CR_MTG_ARC_RSVD14_MAX                                   (1) // 0x00000001
  #define MTG0_CR_MTG_ARC_RSVD14_DEF                                   (0x00000000)
  #define MTG0_CR_MTG_ARC_RSVD14_HSH                                   (0x011CB224)

  #define MTG0_CR_MTG_ARC_RAND_BANK_WIDTH_OFF                          (15)
  #define MTG0_CR_MTG_ARC_RAND_BANK_WIDTH_WID                          ( 3)
  #define MTG0_CR_MTG_ARC_RAND_BANK_WIDTH_MSK                          (0x00038000)
  #define MTG0_CR_MTG_ARC_RAND_BANK_WIDTH_MIN                          (0)
  #define MTG0_CR_MTG_ARC_RAND_BANK_WIDTH_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_ARC_RAND_BANK_WIDTH_DEF                          (0x00000003)
  #define MTG0_CR_MTG_ARC_RAND_BANK_WIDTH_HSH                          (0x031EB224)

  #define MTG0_CR_MTG_ARC_RSVD18_OFF                                   (18)
  #define MTG0_CR_MTG_ARC_RSVD18_WID                                   ( 1)
  #define MTG0_CR_MTG_ARC_RSVD18_MSK                                   (0x00040000)
  #define MTG0_CR_MTG_ARC_RSVD18_MIN                                   (0)
  #define MTG0_CR_MTG_ARC_RSVD18_MAX                                   (1) // 0x00000001
  #define MTG0_CR_MTG_ARC_RSVD18_DEF                                   (0x00000000)
  #define MTG0_CR_MTG_ARC_RSVD18_HSH                                   (0x0124B224)

  #define MTG0_CR_MTG_ARC_RAND_ROW_WIDTH_OFF                           (19)
  #define MTG0_CR_MTG_ARC_RAND_ROW_WIDTH_WID                           ( 5)
  #define MTG0_CR_MTG_ARC_RAND_ROW_WIDTH_MSK                           (0x00F80000)
  #define MTG0_CR_MTG_ARC_RAND_ROW_WIDTH_MIN                           (0)
  #define MTG0_CR_MTG_ARC_RAND_ROW_WIDTH_MAX                           (31) // 0x0000001F
  #define MTG0_CR_MTG_ARC_RAND_ROW_WIDTH_DEF                           (0x0000000E)
  #define MTG0_CR_MTG_ARC_RAND_ROW_WIDTH_HSH                           (0x0526B224)

  #define MTG0_CR_MTG_ARC_RSVD24_OFF                                   (24)
  #define MTG0_CR_MTG_ARC_RSVD24_WID                                   ( 1)
  #define MTG0_CR_MTG_ARC_RSVD24_MSK                                   (0x01000000)
  #define MTG0_CR_MTG_ARC_RSVD24_MIN                                   (0)
  #define MTG0_CR_MTG_ARC_RSVD24_MAX                                   (1) // 0x00000001
  #define MTG0_CR_MTG_ARC_RSVD24_DEF                                   (0x00000000)
  #define MTG0_CR_MTG_ARC_RSVD24_HSH                                   (0x0130B224)

  #define MTG0_CR_MTG_ARC_RAND_COL_WIDTH_OFF                           (25)
  #define MTG0_CR_MTG_ARC_RAND_COL_WIDTH_WID                           ( 4)
  #define MTG0_CR_MTG_ARC_RAND_COL_WIDTH_MSK                           (0x1E000000)
  #define MTG0_CR_MTG_ARC_RAND_COL_WIDTH_MIN                           (0)
  #define MTG0_CR_MTG_ARC_RAND_COL_WIDTH_MAX                           (15) // 0x0000000F
  #define MTG0_CR_MTG_ARC_RAND_COL_WIDTH_DEF                           (0x00000006)
  #define MTG0_CR_MTG_ARC_RAND_COL_WIDTH_HSH                           (0x0432B224)

  #define MTG0_CR_MTG_ARC_RSVD31_29_OFF                                (29)
  #define MTG0_CR_MTG_ARC_RSVD31_29_WID                                ( 3)
  #define MTG0_CR_MTG_ARC_RSVD31_29_MSK                                (0xE0000000)
  #define MTG0_CR_MTG_ARC_RSVD31_29_MIN                                (0)
  #define MTG0_CR_MTG_ARC_RSVD31_29_MAX                                (7) // 0x00000007
  #define MTG0_CR_MTG_ARC_RSVD31_29_DEF                                (0x00000000)
  #define MTG0_CR_MTG_ARC_RSVD31_29_HSH                                (0x033AB224)

#define MTG0_CR_MTG_ARS0_REG                                           (0x0000B228)
#define MTG0_CR_MTG_ARS0_DEF                                           (0x0000C100)
#define MTG0_CR_MTG_ARS0_VOLATILE_BITFIELDS_MSK                        (0x00000000)

  #define MTG0_CR_MTG_ARS0_RSVD6_0_OFF                                 ( 0)
  #define MTG0_CR_MTG_ARS0_RSVD6_0_WID                                 ( 7)
  #define MTG0_CR_MTG_ARS0_RSVD6_0_MSK                                 (0x0000007F)
  #define MTG0_CR_MTG_ARS0_RSVD6_0_MIN                                 (0)
  #define MTG0_CR_MTG_ARS0_RSVD6_0_MAX                                 (127) // 0x0000007F
  #define MTG0_CR_MTG_ARS0_RSVD6_0_DEF                                 (0x00000000)
  #define MTG0_CR_MTG_ARS0_RSVD6_0_HSH                                 (0x0700B228)

  #define MTG0_CR_MTG_ARS0_BANK_RAND_SEED_OFF                          ( 7)
  #define MTG0_CR_MTG_ARS0_BANK_RAND_SEED_WID                          ( 5)
  #define MTG0_CR_MTG_ARS0_BANK_RAND_SEED_MSK                          (0x00000F80)
  #define MTG0_CR_MTG_ARS0_BANK_RAND_SEED_MIN                          (0)
  #define MTG0_CR_MTG_ARS0_BANK_RAND_SEED_MAX                          (31) // 0x0000001F
  #define MTG0_CR_MTG_ARS0_BANK_RAND_SEED_DEF                          (0x00000002)
  #define MTG0_CR_MTG_ARS0_BANK_RAND_SEED_HSH                          (0x050EB228)

  #define MTG0_CR_MTG_ARS0_RSVD13_12_OFF                               (12)
  #define MTG0_CR_MTG_ARS0_RSVD13_12_WID                               ( 2)
  #define MTG0_CR_MTG_ARS0_RSVD13_12_MSK                               (0x00003000)
  #define MTG0_CR_MTG_ARS0_RSVD13_12_MIN                               (0)
  #define MTG0_CR_MTG_ARS0_RSVD13_12_MAX                               (3) // 0x00000003
  #define MTG0_CR_MTG_ARS0_RSVD13_12_DEF                               (0x00000000)
  #define MTG0_CR_MTG_ARS0_RSVD13_12_HSH                               (0x0218B228)

  #define MTG0_CR_MTG_ARS0_COL_RAND_SEED_OFF                           (14)
  #define MTG0_CR_MTG_ARS0_COL_RAND_SEED_WID                           (10)
  #define MTG0_CR_MTG_ARS0_COL_RAND_SEED_MSK                           (0x00FFC000)
  #define MTG0_CR_MTG_ARS0_COL_RAND_SEED_MIN                           (0)
  #define MTG0_CR_MTG_ARS0_COL_RAND_SEED_MAX                           (1023) // 0x000003FF
  #define MTG0_CR_MTG_ARS0_COL_RAND_SEED_DEF                           (0x00000003)
  #define MTG0_CR_MTG_ARS0_COL_RAND_SEED_HSH                           (0x0A1CB228)

  #define MTG0_CR_MTG_ARS0_RSVD31_24_OFF                               (24)
  #define MTG0_CR_MTG_ARS0_RSVD31_24_WID                               ( 8)
  #define MTG0_CR_MTG_ARS0_RSVD31_24_MSK                               (0xFF000000)
  #define MTG0_CR_MTG_ARS0_RSVD31_24_MIN                               (0)
  #define MTG0_CR_MTG_ARS0_RSVD31_24_MAX                               (255) // 0x000000FF
  #define MTG0_CR_MTG_ARS0_RSVD31_24_DEF                               (0x00000000)
  #define MTG0_CR_MTG_ARS0_RSVD31_24_HSH                               (0x0830B228)

#define MTG0_CR_MTG_ARS1_REG                                           (0x0000B22C)
#define MTG0_CR_MTG_ARS1_DEF                                           (0x00000004)
#define MTG0_CR_MTG_ARS1_VOLATILE_BITFIELDS_MSK                        (0x00000000)

  #define MTG0_CR_MTG_ARS1_ROW_RAND_SEED_OFF                           ( 0)
  #define MTG0_CR_MTG_ARS1_ROW_RAND_SEED_WID                           (18)
  #define MTG0_CR_MTG_ARS1_ROW_RAND_SEED_MSK                           (0x0003FFFF)
  #define MTG0_CR_MTG_ARS1_ROW_RAND_SEED_MIN                           (0)
  #define MTG0_CR_MTG_ARS1_ROW_RAND_SEED_MAX                           (262143) // 0x0003FFFF
  #define MTG0_CR_MTG_ARS1_ROW_RAND_SEED_DEF                           (0x00000004)
  #define MTG0_CR_MTG_ARS1_ROW_RAND_SEED_HSH                           (0x1200B22C)

  #define MTG0_CR_MTG_ARS1_RSVD31_18_OFF                               (18)
  #define MTG0_CR_MTG_ARS1_RSVD31_18_WID                               (14)
  #define MTG0_CR_MTG_ARS1_RSVD31_18_MSK                               (0xFFFC0000)
  #define MTG0_CR_MTG_ARS1_RSVD31_18_MIN                               (0)
  #define MTG0_CR_MTG_ARS1_RSVD31_18_MAX                               (16383) // 0x00003FFF
  #define MTG0_CR_MTG_ARS1_RSVD31_18_DEF                               (0x00000000)
  #define MTG0_CR_MTG_ARS1_RSVD31_18_HSH                               (0x0E24B22C)

#define MTG0_CR_MTG_RADC_REG                                           (0x0000B230)
#define MTG0_CR_MTG_RADC_DEF                                           (0x00000000)
#define MTG0_CR_MTG_RADC_VOLATILE_BITFIELDS_MSK                        (0x00000000)

  #define MTG0_CR_MTG_RADC_RST_ADDR_PCE_OFF                            ( 0)
  #define MTG0_CR_MTG_RADC_RST_ADDR_PCE_WID                            ( 1)
  #define MTG0_CR_MTG_RADC_RST_ADDR_PCE_MSK                            (0x00000001)
  #define MTG0_CR_MTG_RADC_RST_ADDR_PCE_MIN                            (0)
  #define MTG0_CR_MTG_RADC_RST_ADDR_PCE_MAX                            (1) // 0x00000001
  #define MTG0_CR_MTG_RADC_RST_ADDR_PCE_DEF                            (0x00000000)
  #define MTG0_CR_MTG_RADC_RST_ADDR_PCE_HSH                            (0x0100B230)

  #define MTG0_CR_MTG_RADC_RST_DQPAT_PCE_OFF                           ( 1)
  #define MTG0_CR_MTG_RADC_RST_DQPAT_PCE_WID                           ( 1)
  #define MTG0_CR_MTG_RADC_RST_DQPAT_PCE_MSK                           (0x00000002)
  #define MTG0_CR_MTG_RADC_RST_DQPAT_PCE_MIN                           (0)
  #define MTG0_CR_MTG_RADC_RST_DQPAT_PCE_MAX                           (1) // 0x00000001
  #define MTG0_CR_MTG_RADC_RST_DQPAT_PCE_DEF                           (0x00000000)
  #define MTG0_CR_MTG_RADC_RST_DQPAT_PCE_HSH                           (0x0102B230)

  #define MTG0_CR_MTG_RADC_RST_DMPAT_PCE_OFF                           ( 2)
  #define MTG0_CR_MTG_RADC_RST_DMPAT_PCE_WID                           ( 1)
  #define MTG0_CR_MTG_RADC_RST_DMPAT_PCE_MSK                           (0x00000004)
  #define MTG0_CR_MTG_RADC_RST_DMPAT_PCE_MIN                           (0)
  #define MTG0_CR_MTG_RADC_RST_DMPAT_PCE_MAX                           (1) // 0x00000001
  #define MTG0_CR_MTG_RADC_RST_DMPAT_PCE_DEF                           (0x00000000)
  #define MTG0_CR_MTG_RADC_RST_DMPAT_PCE_HSH                           (0x0104B230)

  #define MTG0_CR_MTG_RADC_RSVD11_3_OFF                                ( 3)
  #define MTG0_CR_MTG_RADC_RSVD11_3_WID                                ( 9)
  #define MTG0_CR_MTG_RADC_RSVD11_3_MSK                                (0x00000FF8)
  #define MTG0_CR_MTG_RADC_RSVD11_3_MIN                                (0)
  #define MTG0_CR_MTG_RADC_RSVD11_3_MAX                                (511) // 0x000001FF
  #define MTG0_CR_MTG_RADC_RSVD11_3_DEF                                (0x00000000)
  #define MTG0_CR_MTG_RADC_RSVD11_3_HSH                                (0x0906B230)

  #define MTG0_CR_MTG_RADC_RST_ADDR_EN_OFF                             (12)
  #define MTG0_CR_MTG_RADC_RST_ADDR_EN_WID                             ( 1)
  #define MTG0_CR_MTG_RADC_RST_ADDR_EN_MSK                             (0x00001000)
  #define MTG0_CR_MTG_RADC_RST_ADDR_EN_MIN                             (0)
  #define MTG0_CR_MTG_RADC_RST_ADDR_EN_MAX                             (1) // 0x00000001
  #define MTG0_CR_MTG_RADC_RST_ADDR_EN_DEF                             (0x00000000)
  #define MTG0_CR_MTG_RADC_RST_ADDR_EN_HSH                             (0x0118B230)

  #define MTG0_CR_MTG_RADC_RST_DQPAT_EN_OFF                            (13)
  #define MTG0_CR_MTG_RADC_RST_DQPAT_EN_WID                            ( 1)
  #define MTG0_CR_MTG_RADC_RST_DQPAT_EN_MSK                            (0x00002000)
  #define MTG0_CR_MTG_RADC_RST_DQPAT_EN_MIN                            (0)
  #define MTG0_CR_MTG_RADC_RST_DQPAT_EN_MAX                            (1) // 0x00000001
  #define MTG0_CR_MTG_RADC_RST_DQPAT_EN_DEF                            (0x00000000)
  #define MTG0_CR_MTG_RADC_RST_DQPAT_EN_HSH                            (0x011AB230)

  #define MTG0_CR_MTG_RADC_RST_DMPAT_EN_OFF                            (14)
  #define MTG0_CR_MTG_RADC_RST_DMPAT_EN_WID                            ( 1)
  #define MTG0_CR_MTG_RADC_RST_DMPAT_EN_MSK                            (0x00004000)
  #define MTG0_CR_MTG_RADC_RST_DMPAT_EN_MIN                            (0)
  #define MTG0_CR_MTG_RADC_RST_DMPAT_EN_MAX                            (1) // 0x00000001
  #define MTG0_CR_MTG_RADC_RST_DMPAT_EN_DEF                            (0x00000000)
  #define MTG0_CR_MTG_RADC_RST_DMPAT_EN_HSH                            (0x011CB230)

  #define MTG0_CR_MTG_RADC_RSVD15_OFF                                  (15)
  #define MTG0_CR_MTG_RADC_RSVD15_WID                                  ( 1)
  #define MTG0_CR_MTG_RADC_RSVD15_MSK                                  (0x00008000)
  #define MTG0_CR_MTG_RADC_RSVD15_MIN                                  (0)
  #define MTG0_CR_MTG_RADC_RSVD15_MAX                                  (1) // 0x00000001
  #define MTG0_CR_MTG_RADC_RSVD15_DEF                                  (0x00000000)
  #define MTG0_CR_MTG_RADC_RSVD15_HSH                                  (0x011EB230)

  #define MTG0_CR_MTG_RADC_RST_ADDR_DPAT_THRES_OFF                     (16)
  #define MTG0_CR_MTG_RADC_RST_ADDR_DPAT_THRES_WID                     (10)
  #define MTG0_CR_MTG_RADC_RST_ADDR_DPAT_THRES_MSK                     (0x03FF0000)
  #define MTG0_CR_MTG_RADC_RST_ADDR_DPAT_THRES_MIN                     (0)
  #define MTG0_CR_MTG_RADC_RST_ADDR_DPAT_THRES_MAX                     (1023) // 0x000003FF
  #define MTG0_CR_MTG_RADC_RST_ADDR_DPAT_THRES_DEF                     (0x00000000)
  #define MTG0_CR_MTG_RADC_RST_ADDR_DPAT_THRES_HSH                     (0x0A20B230)

  #define MTG0_CR_MTG_RADC_RSVD31_26_OFF                               (26)
  #define MTG0_CR_MTG_RADC_RSVD31_26_WID                               ( 6)
  #define MTG0_CR_MTG_RADC_RSVD31_26_MSK                               (0xFC000000)
  #define MTG0_CR_MTG_RADC_RSVD31_26_MIN                               (0)
  #define MTG0_CR_MTG_RADC_RSVD31_26_MAX                               (63) // 0x0000003F
  #define MTG0_CR_MTG_RADC_RSVD31_26_DEF                               (0x00000000)
  #define MTG0_CR_MTG_RADC_RSVD31_26_HSH                               (0x0634B230)

#define MTG0_CR_MTG_DQPATSEL0_REG                                      (0x0000B250)
#define MTG0_CR_MTG_DQPATSEL0_DEF                                      (0x76543210)
#define MTG0_CR_MTG_DQPATSEL0_VOLATILE_BITFIELDS_MSK                   (0x00000000)

  #define MTG0_CR_MTG_DQPATSEL0_DQ0_PATSEL_OFF                         ( 0)
  #define MTG0_CR_MTG_DQPATSEL0_DQ0_PATSEL_WID                         ( 3)
  #define MTG0_CR_MTG_DQPATSEL0_DQ0_PATSEL_MSK                         (0x00000007)
  #define MTG0_CR_MTG_DQPATSEL0_DQ0_PATSEL_MIN                         (0)
  #define MTG0_CR_MTG_DQPATSEL0_DQ0_PATSEL_MAX                         (7) // 0x00000007
  #define MTG0_CR_MTG_DQPATSEL0_DQ0_PATSEL_DEF                         (0x00000000)
  #define MTG0_CR_MTG_DQPATSEL0_DQ0_PATSEL_HSH                         (0x0300B250)

  #define MTG0_CR_MTG_DQPATSEL0_RSVD3_OFF                              ( 3)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD3_WID                              ( 1)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD3_MSK                              (0x00000008)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD3_MIN                              (0)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD3_MAX                              (1) // 0x00000001
  #define MTG0_CR_MTG_DQPATSEL0_RSVD3_DEF                              (0x00000000)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD3_HSH                              (0x0106B250)

  #define MTG0_CR_MTG_DQPATSEL0_DQ1_PATSEL_OFF                         ( 4)
  #define MTG0_CR_MTG_DQPATSEL0_DQ1_PATSEL_WID                         ( 3)
  #define MTG0_CR_MTG_DQPATSEL0_DQ1_PATSEL_MSK                         (0x00000070)
  #define MTG0_CR_MTG_DQPATSEL0_DQ1_PATSEL_MIN                         (0)
  #define MTG0_CR_MTG_DQPATSEL0_DQ1_PATSEL_MAX                         (7) // 0x00000007
  #define MTG0_CR_MTG_DQPATSEL0_DQ1_PATSEL_DEF                         (0x00000001)
  #define MTG0_CR_MTG_DQPATSEL0_DQ1_PATSEL_HSH                         (0x0308B250)

  #define MTG0_CR_MTG_DQPATSEL0_RSVD7_OFF                              ( 7)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD7_WID                              ( 1)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD7_MSK                              (0x00000080)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD7_MIN                              (0)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD7_MAX                              (1) // 0x00000001
  #define MTG0_CR_MTG_DQPATSEL0_RSVD7_DEF                              (0x00000000)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD7_HSH                              (0x010EB250)

  #define MTG0_CR_MTG_DQPATSEL0_DQ2_PATSEL_OFF                         ( 8)
  #define MTG0_CR_MTG_DQPATSEL0_DQ2_PATSEL_WID                         ( 3)
  #define MTG0_CR_MTG_DQPATSEL0_DQ2_PATSEL_MSK                         (0x00000700)
  #define MTG0_CR_MTG_DQPATSEL0_DQ2_PATSEL_MIN                         (0)
  #define MTG0_CR_MTG_DQPATSEL0_DQ2_PATSEL_MAX                         (7) // 0x00000007
  #define MTG0_CR_MTG_DQPATSEL0_DQ2_PATSEL_DEF                         (0x00000002)
  #define MTG0_CR_MTG_DQPATSEL0_DQ2_PATSEL_HSH                         (0x0310B250)

  #define MTG0_CR_MTG_DQPATSEL0_RSVD11_OFF                             (11)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD11_WID                             ( 1)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD11_MSK                             (0x00000800)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD11_MIN                             (0)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD11_MAX                             (1) // 0x00000001
  #define MTG0_CR_MTG_DQPATSEL0_RSVD11_DEF                             (0x00000000)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD11_HSH                             (0x0116B250)

  #define MTG0_CR_MTG_DQPATSEL0_DQ3_PATSEL_OFF                         (12)
  #define MTG0_CR_MTG_DQPATSEL0_DQ3_PATSEL_WID                         ( 3)
  #define MTG0_CR_MTG_DQPATSEL0_DQ3_PATSEL_MSK                         (0x00007000)
  #define MTG0_CR_MTG_DQPATSEL0_DQ3_PATSEL_MIN                         (0)
  #define MTG0_CR_MTG_DQPATSEL0_DQ3_PATSEL_MAX                         (7) // 0x00000007
  #define MTG0_CR_MTG_DQPATSEL0_DQ3_PATSEL_DEF                         (0x00000003)
  #define MTG0_CR_MTG_DQPATSEL0_DQ3_PATSEL_HSH                         (0x0318B250)

  #define MTG0_CR_MTG_DQPATSEL0_RSVD15_OFF                             (15)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD15_WID                             ( 1)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD15_MSK                             (0x00008000)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD15_MIN                             (0)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD15_MAX                             (1) // 0x00000001
  #define MTG0_CR_MTG_DQPATSEL0_RSVD15_DEF                             (0x00000000)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD15_HSH                             (0x011EB250)

  #define MTG0_CR_MTG_DQPATSEL0_DQ4_PATSEL_OFF                         (16)
  #define MTG0_CR_MTG_DQPATSEL0_DQ4_PATSEL_WID                         ( 3)
  #define MTG0_CR_MTG_DQPATSEL0_DQ4_PATSEL_MSK                         (0x00070000)
  #define MTG0_CR_MTG_DQPATSEL0_DQ4_PATSEL_MIN                         (0)
  #define MTG0_CR_MTG_DQPATSEL0_DQ4_PATSEL_MAX                         (7) // 0x00000007
  #define MTG0_CR_MTG_DQPATSEL0_DQ4_PATSEL_DEF                         (0x00000004)
  #define MTG0_CR_MTG_DQPATSEL0_DQ4_PATSEL_HSH                         (0x0320B250)

  #define MTG0_CR_MTG_DQPATSEL0_RSVD19_OFF                             (19)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD19_WID                             ( 1)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD19_MSK                             (0x00080000)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD19_MIN                             (0)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD19_MAX                             (1) // 0x00000001
  #define MTG0_CR_MTG_DQPATSEL0_RSVD19_DEF                             (0x00000000)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD19_HSH                             (0x0126B250)

  #define MTG0_CR_MTG_DQPATSEL0_DQ5_PATSEL_OFF                         (20)
  #define MTG0_CR_MTG_DQPATSEL0_DQ5_PATSEL_WID                         ( 3)
  #define MTG0_CR_MTG_DQPATSEL0_DQ5_PATSEL_MSK                         (0x00700000)
  #define MTG0_CR_MTG_DQPATSEL0_DQ5_PATSEL_MIN                         (0)
  #define MTG0_CR_MTG_DQPATSEL0_DQ5_PATSEL_MAX                         (7) // 0x00000007
  #define MTG0_CR_MTG_DQPATSEL0_DQ5_PATSEL_DEF                         (0x00000005)
  #define MTG0_CR_MTG_DQPATSEL0_DQ5_PATSEL_HSH                         (0x0328B250)

  #define MTG0_CR_MTG_DQPATSEL0_RSVD23_OFF                             (23)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD23_WID                             ( 1)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD23_MSK                             (0x00800000)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD23_MIN                             (0)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD23_MAX                             (1) // 0x00000001
  #define MTG0_CR_MTG_DQPATSEL0_RSVD23_DEF                             (0x00000000)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD23_HSH                             (0x012EB250)

  #define MTG0_CR_MTG_DQPATSEL0_DQ6_PATSEL_OFF                         (24)
  #define MTG0_CR_MTG_DQPATSEL0_DQ6_PATSEL_WID                         ( 3)
  #define MTG0_CR_MTG_DQPATSEL0_DQ6_PATSEL_MSK                         (0x07000000)
  #define MTG0_CR_MTG_DQPATSEL0_DQ6_PATSEL_MIN                         (0)
  #define MTG0_CR_MTG_DQPATSEL0_DQ6_PATSEL_MAX                         (7) // 0x00000007
  #define MTG0_CR_MTG_DQPATSEL0_DQ6_PATSEL_DEF                         (0x00000006)
  #define MTG0_CR_MTG_DQPATSEL0_DQ6_PATSEL_HSH                         (0x0330B250)

  #define MTG0_CR_MTG_DQPATSEL0_RSVD27_OFF                             (27)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD27_WID                             ( 1)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD27_MSK                             (0x08000000)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD27_MIN                             (0)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD27_MAX                             (1) // 0x00000001
  #define MTG0_CR_MTG_DQPATSEL0_RSVD27_DEF                             (0x00000000)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD27_HSH                             (0x0136B250)

  #define MTG0_CR_MTG_DQPATSEL0_DQ7_PATSEL_OFF                         (28)
  #define MTG0_CR_MTG_DQPATSEL0_DQ7_PATSEL_WID                         ( 3)
  #define MTG0_CR_MTG_DQPATSEL0_DQ7_PATSEL_MSK                         (0x70000000)
  #define MTG0_CR_MTG_DQPATSEL0_DQ7_PATSEL_MIN                         (0)
  #define MTG0_CR_MTG_DQPATSEL0_DQ7_PATSEL_MAX                         (7) // 0x00000007
  #define MTG0_CR_MTG_DQPATSEL0_DQ7_PATSEL_DEF                         (0x00000007)
  #define MTG0_CR_MTG_DQPATSEL0_DQ7_PATSEL_HSH                         (0x0338B250)

  #define MTG0_CR_MTG_DQPATSEL0_RSVD31_OFF                             (31)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD31_WID                             ( 1)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD31_MSK                             (0x80000000)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD31_MIN                             (0)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD31_MAX                             (1) // 0x00000001
  #define MTG0_CR_MTG_DQPATSEL0_RSVD31_DEF                             (0x00000000)
  #define MTG0_CR_MTG_DQPATSEL0_RSVD31_HSH                             (0x013EB250)

#define MTG0_CR_MTG_DQPATSEL1_REG                                      (0x0000B254)
#define MTG0_CR_MTG_DQPATSEL1_DEF                                      (0x76543210)
#define MTG0_CR_MTG_DQPATSEL1_VOLATILE_BITFIELDS_MSK                   (0x00000000)

  #define MTG0_CR_MTG_DQPATSEL1_DQ8_PATSEL_OFF                         ( 0)
  #define MTG0_CR_MTG_DQPATSEL1_DQ8_PATSEL_WID                         ( 3)
  #define MTG0_CR_MTG_DQPATSEL1_DQ8_PATSEL_MSK                         (0x00000007)
  #define MTG0_CR_MTG_DQPATSEL1_DQ8_PATSEL_MIN                         (0)
  #define MTG0_CR_MTG_DQPATSEL1_DQ8_PATSEL_MAX                         (7) // 0x00000007
  #define MTG0_CR_MTG_DQPATSEL1_DQ8_PATSEL_DEF                         (0x00000000)
  #define MTG0_CR_MTG_DQPATSEL1_DQ8_PATSEL_HSH                         (0x0300B254)

  #define MTG0_CR_MTG_DQPATSEL1_RSVD3_OFF                              ( 3)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD3_WID                              ( 1)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD3_MSK                              (0x00000008)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD3_MIN                              (0)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD3_MAX                              (1) // 0x00000001
  #define MTG0_CR_MTG_DQPATSEL1_RSVD3_DEF                              (0x00000000)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD3_HSH                              (0x0106B254)

  #define MTG0_CR_MTG_DQPATSEL1_DQ9_PATSEL_OFF                         ( 4)
  #define MTG0_CR_MTG_DQPATSEL1_DQ9_PATSEL_WID                         ( 3)
  #define MTG0_CR_MTG_DQPATSEL1_DQ9_PATSEL_MSK                         (0x00000070)
  #define MTG0_CR_MTG_DQPATSEL1_DQ9_PATSEL_MIN                         (0)
  #define MTG0_CR_MTG_DQPATSEL1_DQ9_PATSEL_MAX                         (7) // 0x00000007
  #define MTG0_CR_MTG_DQPATSEL1_DQ9_PATSEL_DEF                         (0x00000001)
  #define MTG0_CR_MTG_DQPATSEL1_DQ9_PATSEL_HSH                         (0x0308B254)

  #define MTG0_CR_MTG_DQPATSEL1_RSVD7_OFF                              ( 7)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD7_WID                              ( 1)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD7_MSK                              (0x00000080)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD7_MIN                              (0)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD7_MAX                              (1) // 0x00000001
  #define MTG0_CR_MTG_DQPATSEL1_RSVD7_DEF                              (0x00000000)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD7_HSH                              (0x010EB254)

  #define MTG0_CR_MTG_DQPATSEL1_DQ10_PATSEL_OFF                        ( 8)
  #define MTG0_CR_MTG_DQPATSEL1_DQ10_PATSEL_WID                        ( 3)
  #define MTG0_CR_MTG_DQPATSEL1_DQ10_PATSEL_MSK                        (0x00000700)
  #define MTG0_CR_MTG_DQPATSEL1_DQ10_PATSEL_MIN                        (0)
  #define MTG0_CR_MTG_DQPATSEL1_DQ10_PATSEL_MAX                        (7) // 0x00000007
  #define MTG0_CR_MTG_DQPATSEL1_DQ10_PATSEL_DEF                        (0x00000002)
  #define MTG0_CR_MTG_DQPATSEL1_DQ10_PATSEL_HSH                        (0x0310B254)

  #define MTG0_CR_MTG_DQPATSEL1_RSVD11_OFF                             (11)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD11_WID                             ( 1)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD11_MSK                             (0x00000800)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD11_MIN                             (0)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD11_MAX                             (1) // 0x00000001
  #define MTG0_CR_MTG_DQPATSEL1_RSVD11_DEF                             (0x00000000)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD11_HSH                             (0x0116B254)

  #define MTG0_CR_MTG_DQPATSEL1_DQ11_PATSEL_OFF                        (12)
  #define MTG0_CR_MTG_DQPATSEL1_DQ11_PATSEL_WID                        ( 3)
  #define MTG0_CR_MTG_DQPATSEL1_DQ11_PATSEL_MSK                        (0x00007000)
  #define MTG0_CR_MTG_DQPATSEL1_DQ11_PATSEL_MIN                        (0)
  #define MTG0_CR_MTG_DQPATSEL1_DQ11_PATSEL_MAX                        (7) // 0x00000007
  #define MTG0_CR_MTG_DQPATSEL1_DQ11_PATSEL_DEF                        (0x00000003)
  #define MTG0_CR_MTG_DQPATSEL1_DQ11_PATSEL_HSH                        (0x0318B254)

  #define MTG0_CR_MTG_DQPATSEL1_RSVD15_OFF                             (15)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD15_WID                             ( 1)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD15_MSK                             (0x00008000)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD15_MIN                             (0)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD15_MAX                             (1) // 0x00000001
  #define MTG0_CR_MTG_DQPATSEL1_RSVD15_DEF                             (0x00000000)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD15_HSH                             (0x011EB254)

  #define MTG0_CR_MTG_DQPATSEL1_DQ12_PATSEL_OFF                        (16)
  #define MTG0_CR_MTG_DQPATSEL1_DQ12_PATSEL_WID                        ( 3)
  #define MTG0_CR_MTG_DQPATSEL1_DQ12_PATSEL_MSK                        (0x00070000)
  #define MTG0_CR_MTG_DQPATSEL1_DQ12_PATSEL_MIN                        (0)
  #define MTG0_CR_MTG_DQPATSEL1_DQ12_PATSEL_MAX                        (7) // 0x00000007
  #define MTG0_CR_MTG_DQPATSEL1_DQ12_PATSEL_DEF                        (0x00000004)
  #define MTG0_CR_MTG_DQPATSEL1_DQ12_PATSEL_HSH                        (0x0320B254)

  #define MTG0_CR_MTG_DQPATSEL1_RSVD19_OFF                             (19)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD19_WID                             ( 1)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD19_MSK                             (0x00080000)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD19_MIN                             (0)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD19_MAX                             (1) // 0x00000001
  #define MTG0_CR_MTG_DQPATSEL1_RSVD19_DEF                             (0x00000000)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD19_HSH                             (0x0126B254)

  #define MTG0_CR_MTG_DQPATSEL1_DQ13_PATSEL_OFF                        (20)
  #define MTG0_CR_MTG_DQPATSEL1_DQ13_PATSEL_WID                        ( 3)
  #define MTG0_CR_MTG_DQPATSEL1_DQ13_PATSEL_MSK                        (0x00700000)
  #define MTG0_CR_MTG_DQPATSEL1_DQ13_PATSEL_MIN                        (0)
  #define MTG0_CR_MTG_DQPATSEL1_DQ13_PATSEL_MAX                        (7) // 0x00000007
  #define MTG0_CR_MTG_DQPATSEL1_DQ13_PATSEL_DEF                        (0x00000005)
  #define MTG0_CR_MTG_DQPATSEL1_DQ13_PATSEL_HSH                        (0x0328B254)

  #define MTG0_CR_MTG_DQPATSEL1_RSVD23_OFF                             (23)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD23_WID                             ( 1)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD23_MSK                             (0x00800000)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD23_MIN                             (0)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD23_MAX                             (1) // 0x00000001
  #define MTG0_CR_MTG_DQPATSEL1_RSVD23_DEF                             (0x00000000)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD23_HSH                             (0x012EB254)

  #define MTG0_CR_MTG_DQPATSEL1_DQ14_PATSEL_OFF                        (24)
  #define MTG0_CR_MTG_DQPATSEL1_DQ14_PATSEL_WID                        ( 3)
  #define MTG0_CR_MTG_DQPATSEL1_DQ14_PATSEL_MSK                        (0x07000000)
  #define MTG0_CR_MTG_DQPATSEL1_DQ14_PATSEL_MIN                        (0)
  #define MTG0_CR_MTG_DQPATSEL1_DQ14_PATSEL_MAX                        (7) // 0x00000007
  #define MTG0_CR_MTG_DQPATSEL1_DQ14_PATSEL_DEF                        (0x00000006)
  #define MTG0_CR_MTG_DQPATSEL1_DQ14_PATSEL_HSH                        (0x0330B254)

  #define MTG0_CR_MTG_DQPATSEL1_RSVD27_OFF                             (27)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD27_WID                             ( 1)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD27_MSK                             (0x08000000)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD27_MIN                             (0)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD27_MAX                             (1) // 0x00000001
  #define MTG0_CR_MTG_DQPATSEL1_RSVD27_DEF                             (0x00000000)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD27_HSH                             (0x0136B254)

  #define MTG0_CR_MTG_DQPATSEL1_DQ15_PATSEL_OFF                        (28)
  #define MTG0_CR_MTG_DQPATSEL1_DQ15_PATSEL_WID                        ( 3)
  #define MTG0_CR_MTG_DQPATSEL1_DQ15_PATSEL_MSK                        (0x70000000)
  #define MTG0_CR_MTG_DQPATSEL1_DQ15_PATSEL_MIN                        (0)
  #define MTG0_CR_MTG_DQPATSEL1_DQ15_PATSEL_MAX                        (7) // 0x00000007
  #define MTG0_CR_MTG_DQPATSEL1_DQ15_PATSEL_DEF                        (0x00000007)
  #define MTG0_CR_MTG_DQPATSEL1_DQ15_PATSEL_HSH                        (0x0338B254)

  #define MTG0_CR_MTG_DQPATSEL1_RSVD31_OFF                             (31)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD31_WID                             ( 1)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD31_MSK                             (0x80000000)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD31_MIN                             (0)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD31_MAX                             (1) // 0x00000001
  #define MTG0_CR_MTG_DQPATSEL1_RSVD31_DEF                             (0x00000000)
  #define MTG0_CR_MTG_DQPATSEL1_RSVD31_HSH                             (0x013EB254)

#define MTG0_CR_MTG_UPDQBM_REG                                         (0x0000B258)
#define MTG0_CR_MTG_UPDQBM_DEF                                         (0x00000008)
#define MTG0_CR_MTG_UPDQBM_VOLATILE_BITFIELDS_MSK                      (0x00000000)

  #define MTG0_CR_MTG_UPDQBM_RSVD1_0_OFF                               ( 0)
  #define MTG0_CR_MTG_UPDQBM_RSVD1_0_WID                               ( 2)
  #define MTG0_CR_MTG_UPDQBM_RSVD1_0_MSK                               (0x00000003)
  #define MTG0_CR_MTG_UPDQBM_RSVD1_0_MIN                               (0)
  #define MTG0_CR_MTG_UPDQBM_RSVD1_0_MAX                               (3) // 0x00000003
  #define MTG0_CR_MTG_UPDQBM_RSVD1_0_DEF                               (0x00000000)
  #define MTG0_CR_MTG_UPDQBM_RSVD1_0_HSH                               (0x0200B258)

  #define MTG0_CR_MTG_UPDQBM_DQ_BYTE2_MAP_OFF                          ( 2)
  #define MTG0_CR_MTG_UPDQBM_DQ_BYTE2_MAP_WID                          ( 1)
  #define MTG0_CR_MTG_UPDQBM_DQ_BYTE2_MAP_MSK                          (0x00000004)
  #define MTG0_CR_MTG_UPDQBM_DQ_BYTE2_MAP_MIN                          (0)
  #define MTG0_CR_MTG_UPDQBM_DQ_BYTE2_MAP_MAX                          (1) // 0x00000001
  #define MTG0_CR_MTG_UPDQBM_DQ_BYTE2_MAP_DEF                          (0x00000000)
  #define MTG0_CR_MTG_UPDQBM_DQ_BYTE2_MAP_HSH                          (0x0104B258)

  #define MTG0_CR_MTG_UPDQBM_DQ_BYTE3_MAP_OFF                          ( 3)
  #define MTG0_CR_MTG_UPDQBM_DQ_BYTE3_MAP_WID                          ( 1)
  #define MTG0_CR_MTG_UPDQBM_DQ_BYTE3_MAP_MSK                          (0x00000008)
  #define MTG0_CR_MTG_UPDQBM_DQ_BYTE3_MAP_MIN                          (0)
  #define MTG0_CR_MTG_UPDQBM_DQ_BYTE3_MAP_MAX                          (1) // 0x00000001
  #define MTG0_CR_MTG_UPDQBM_DQ_BYTE3_MAP_DEF                          (0x00000001)
  #define MTG0_CR_MTG_UPDQBM_DQ_BYTE3_MAP_HSH                          (0x0106B258)

  #define MTG0_CR_MTG_UPDQBM_RSVD7_4_OFF                               ( 4)
  #define MTG0_CR_MTG_UPDQBM_RSVD7_4_WID                               ( 4)
  #define MTG0_CR_MTG_UPDQBM_RSVD7_4_MSK                               (0x000000F0)
  #define MTG0_CR_MTG_UPDQBM_RSVD7_4_MIN                               (0)
  #define MTG0_CR_MTG_UPDQBM_RSVD7_4_MAX                               (15) // 0x0000000F
  #define MTG0_CR_MTG_UPDQBM_RSVD7_4_DEF                               (0x00000000)
  #define MTG0_CR_MTG_UPDQBM_RSVD7_4_HSH                               (0x0408B258)

  #define MTG0_CR_MTG_UPDQBM_ECC_DQ_BYTE_MAP_OFF                       ( 8)
  #define MTG0_CR_MTG_UPDQBM_ECC_DQ_BYTE_MAP_WID                       ( 1)
  #define MTG0_CR_MTG_UPDQBM_ECC_DQ_BYTE_MAP_MSK                       (0x00000100)
  #define MTG0_CR_MTG_UPDQBM_ECC_DQ_BYTE_MAP_MIN                       (0)
  #define MTG0_CR_MTG_UPDQBM_ECC_DQ_BYTE_MAP_MAX                       (1) // 0x00000001
  #define MTG0_CR_MTG_UPDQBM_ECC_DQ_BYTE_MAP_DEF                       (0x00000000)
  #define MTG0_CR_MTG_UPDQBM_ECC_DQ_BYTE_MAP_HSH                       (0x0110B258)

  #define MTG0_CR_MTG_UPDQBM_RSVD31_9_OFF                              ( 9)
  #define MTG0_CR_MTG_UPDQBM_RSVD31_9_WID                              (23)
  #define MTG0_CR_MTG_UPDQBM_RSVD31_9_MSK                              (0xFFFFFE00)
  #define MTG0_CR_MTG_UPDQBM_RSVD31_9_MIN                              (0)
  #define MTG0_CR_MTG_UPDQBM_RSVD31_9_MAX                              (8388607) // 0x007FFFFF
  #define MTG0_CR_MTG_UPDQBM_RSVD31_9_DEF                              (0x00000000)
  #define MTG0_CR_MTG_UPDQBM_RSVD31_9_HSH                              (0x1712B258)

#define MTG0_CR_MTG_DMPATSEL_REG                                       (0x0000B25C)
#define MTG0_CR_MTG_DMPATSEL_DEF                                       (0x00003210)
#define MTG0_CR_MTG_DMPATSEL_VOLATILE_BITFIELDS_MSK                    (0x00000000)

  #define MTG0_CR_MTG_DMPATSEL_DM0_PATSEL_OFF                          ( 0)
  #define MTG0_CR_MTG_DMPATSEL_DM0_PATSEL_WID                          ( 2)
  #define MTG0_CR_MTG_DMPATSEL_DM0_PATSEL_MSK                          (0x00000003)
  #define MTG0_CR_MTG_DMPATSEL_DM0_PATSEL_MIN                          (0)
  #define MTG0_CR_MTG_DMPATSEL_DM0_PATSEL_MAX                          (3) // 0x00000003
  #define MTG0_CR_MTG_DMPATSEL_DM0_PATSEL_DEF                          (0x00000000)
  #define MTG0_CR_MTG_DMPATSEL_DM0_PATSEL_HSH                          (0x0200B25C)

  #define MTG0_CR_MTG_DMPATSEL_RSVD3_2_OFF                             ( 2)
  #define MTG0_CR_MTG_DMPATSEL_RSVD3_2_WID                             ( 2)
  #define MTG0_CR_MTG_DMPATSEL_RSVD3_2_MSK                             (0x0000000C)
  #define MTG0_CR_MTG_DMPATSEL_RSVD3_2_MIN                             (0)
  #define MTG0_CR_MTG_DMPATSEL_RSVD3_2_MAX                             (3) // 0x00000003
  #define MTG0_CR_MTG_DMPATSEL_RSVD3_2_DEF                             (0x00000000)
  #define MTG0_CR_MTG_DMPATSEL_RSVD3_2_HSH                             (0x0204B25C)

  #define MTG0_CR_MTG_DMPATSEL_DM1_PATSEL_OFF                          ( 4)
  #define MTG0_CR_MTG_DMPATSEL_DM1_PATSEL_WID                          ( 2)
  #define MTG0_CR_MTG_DMPATSEL_DM1_PATSEL_MSK                          (0x00000030)
  #define MTG0_CR_MTG_DMPATSEL_DM1_PATSEL_MIN                          (0)
  #define MTG0_CR_MTG_DMPATSEL_DM1_PATSEL_MAX                          (3) // 0x00000003
  #define MTG0_CR_MTG_DMPATSEL_DM1_PATSEL_DEF                          (0x00000001)
  #define MTG0_CR_MTG_DMPATSEL_DM1_PATSEL_HSH                          (0x0208B25C)

  #define MTG0_CR_MTG_DMPATSEL_RSVD7_6_OFF                             ( 6)
  #define MTG0_CR_MTG_DMPATSEL_RSVD7_6_WID                             ( 2)
  #define MTG0_CR_MTG_DMPATSEL_RSVD7_6_MSK                             (0x000000C0)
  #define MTG0_CR_MTG_DMPATSEL_RSVD7_6_MIN                             (0)
  #define MTG0_CR_MTG_DMPATSEL_RSVD7_6_MAX                             (3) // 0x00000003
  #define MTG0_CR_MTG_DMPATSEL_RSVD7_6_DEF                             (0x00000000)
  #define MTG0_CR_MTG_DMPATSEL_RSVD7_6_HSH                             (0x020CB25C)

  #define MTG0_CR_MTG_DMPATSEL_DM2_PATSEL_OFF                          ( 8)
  #define MTG0_CR_MTG_DMPATSEL_DM2_PATSEL_WID                          ( 2)
  #define MTG0_CR_MTG_DMPATSEL_DM2_PATSEL_MSK                          (0x00000300)
  #define MTG0_CR_MTG_DMPATSEL_DM2_PATSEL_MIN                          (0)
  #define MTG0_CR_MTG_DMPATSEL_DM2_PATSEL_MAX                          (3) // 0x00000003
  #define MTG0_CR_MTG_DMPATSEL_DM2_PATSEL_DEF                          (0x00000002)
  #define MTG0_CR_MTG_DMPATSEL_DM2_PATSEL_HSH                          (0x0210B25C)

  #define MTG0_CR_MTG_DMPATSEL_RSVD11_10_OFF                           (10)
  #define MTG0_CR_MTG_DMPATSEL_RSVD11_10_WID                           ( 2)
  #define MTG0_CR_MTG_DMPATSEL_RSVD11_10_MSK                           (0x00000C00)
  #define MTG0_CR_MTG_DMPATSEL_RSVD11_10_MIN                           (0)
  #define MTG0_CR_MTG_DMPATSEL_RSVD11_10_MAX                           (3) // 0x00000003
  #define MTG0_CR_MTG_DMPATSEL_RSVD11_10_DEF                           (0x00000000)
  #define MTG0_CR_MTG_DMPATSEL_RSVD11_10_HSH                           (0x0214B25C)

  #define MTG0_CR_MTG_DMPATSEL_DM3_PATSEL_OFF                          (12)
  #define MTG0_CR_MTG_DMPATSEL_DM3_PATSEL_WID                          ( 2)
  #define MTG0_CR_MTG_DMPATSEL_DM3_PATSEL_MSK                          (0x00003000)
  #define MTG0_CR_MTG_DMPATSEL_DM3_PATSEL_MIN                          (0)
  #define MTG0_CR_MTG_DMPATSEL_DM3_PATSEL_MAX                          (3) // 0x00000003
  #define MTG0_CR_MTG_DMPATSEL_DM3_PATSEL_DEF                          (0x00000003)
  #define MTG0_CR_MTG_DMPATSEL_DM3_PATSEL_HSH                          (0x0218B25C)

  #define MTG0_CR_MTG_DMPATSEL_RSVD31_14_OFF                           (14)
  #define MTG0_CR_MTG_DMPATSEL_RSVD31_14_WID                           (18)
  #define MTG0_CR_MTG_DMPATSEL_RSVD31_14_MSK                           (0xFFFFC000)
  #define MTG0_CR_MTG_DMPATSEL_RSVD31_14_MIN                           (0)
  #define MTG0_CR_MTG_DMPATSEL_RSVD31_14_MAX                           (262143) // 0x0003FFFF
  #define MTG0_CR_MTG_DMPATSEL_RSVD31_14_DEF                           (0x00000000)
  #define MTG0_CR_MTG_DMPATSEL_RSVD31_14_HSH                           (0x121CB25C)

#define MTG0_CR_MTG_SDQP0_0_REG                                        (0x0000B260)
#define MTG0_CR_MTG_SDQP0_0_DEF                                        (0x00FFFF00)
#define MTG0_CR_MTG_SDQP0_0_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define MTG0_CR_MTG_SDQP0_0_SERIAL_DQPAT0_0_OFF                      ( 0)
  #define MTG0_CR_MTG_SDQP0_0_SERIAL_DQPAT0_0_WID                      (32)
  #define MTG0_CR_MTG_SDQP0_0_SERIAL_DQPAT0_0_MSK                      (0xFFFFFFFF)
  #define MTG0_CR_MTG_SDQP0_0_SERIAL_DQPAT0_0_MIN                      (0)
  #define MTG0_CR_MTG_SDQP0_0_SERIAL_DQPAT0_0_MAX                      (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_SDQP0_0_SERIAL_DQPAT0_0_DEF                      (0x00FFFF00)
  #define MTG0_CR_MTG_SDQP0_0_SERIAL_DQPAT0_0_HSH                      (0x2000B260)

#define MTG0_CR_MTG_SDQP0_1_REG                                        (0x0000B264)
#define MTG0_CR_MTG_SDQP0_1_DEF                                        (0xFFAA00AA)
#define MTG0_CR_MTG_SDQP0_1_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define MTG0_CR_MTG_SDQP0_1_SERIAL_DQPAT0_1_OFF                      ( 0)
  #define MTG0_CR_MTG_SDQP0_1_SERIAL_DQPAT0_1_WID                      (32)
  #define MTG0_CR_MTG_SDQP0_1_SERIAL_DQPAT0_1_MSK                      (0xFFFFFFFF)
  #define MTG0_CR_MTG_SDQP0_1_SERIAL_DQPAT0_1_MIN                      (0)
  #define MTG0_CR_MTG_SDQP0_1_SERIAL_DQPAT0_1_MAX                      (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_SDQP0_1_SERIAL_DQPAT0_1_DEF                      (0xFFAA00AA)
  #define MTG0_CR_MTG_SDQP0_1_SERIAL_DQPAT0_1_HSH                      (0x2000B264)

#define MTG0_CR_MTG_SDQP1_0_REG                                        (0x0000B268)
#define MTG0_CR_MTG_SDQP1_0_DEF                                        (0x0F0FF0F0)
#define MTG0_CR_MTG_SDQP1_0_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define MTG0_CR_MTG_SDQP1_0_SERIAL_DQPAT1_0_OFF                      ( 0)
  #define MTG0_CR_MTG_SDQP1_0_SERIAL_DQPAT1_0_WID                      (32)
  #define MTG0_CR_MTG_SDQP1_0_SERIAL_DQPAT1_0_MSK                      (0xFFFFFFFF)
  #define MTG0_CR_MTG_SDQP1_0_SERIAL_DQPAT1_0_MIN                      (0)
  #define MTG0_CR_MTG_SDQP1_0_SERIAL_DQPAT1_0_MAX                      (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_SDQP1_0_SERIAL_DQPAT1_0_DEF                      (0x0F0FF0F0)
  #define MTG0_CR_MTG_SDQP1_0_SERIAL_DQPAT1_0_HSH                      (0x2000B268)

#define MTG0_CR_MTG_SDQP1_1_REG                                        (0x0000B26C)
#define MTG0_CR_MTG_SDQP1_1_DEF                                        (0xD755AA00)
#define MTG0_CR_MTG_SDQP1_1_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define MTG0_CR_MTG_SDQP1_1_SERIAL_DQPAT1_1_OFF                      ( 0)
  #define MTG0_CR_MTG_SDQP1_1_SERIAL_DQPAT1_1_WID                      (32)
  #define MTG0_CR_MTG_SDQP1_1_SERIAL_DQPAT1_1_MSK                      (0xFFFFFFFF)
  #define MTG0_CR_MTG_SDQP1_1_SERIAL_DQPAT1_1_MIN                      (0)
  #define MTG0_CR_MTG_SDQP1_1_SERIAL_DQPAT1_1_MAX                      (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_SDQP1_1_SERIAL_DQPAT1_1_DEF                      (0xD755AA00)
  #define MTG0_CR_MTG_SDQP1_1_SERIAL_DQPAT1_1_HSH                      (0x2000B26C)

#define MTG0_CR_MTG_SDQP2_0_REG                                        (0x0000B270)
#define MTG0_CR_MTG_SDQP2_0_DEF                                        (0x3333CCCC)
#define MTG0_CR_MTG_SDQP2_0_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define MTG0_CR_MTG_SDQP2_0_SERIAL_DQPAT2_0_OFF                      ( 0)
  #define MTG0_CR_MTG_SDQP2_0_SERIAL_DQPAT2_0_WID                      (32)
  #define MTG0_CR_MTG_SDQP2_0_SERIAL_DQPAT2_0_MSK                      (0xFFFFFFFF)
  #define MTG0_CR_MTG_SDQP2_0_SERIAL_DQPAT2_0_MIN                      (0)
  #define MTG0_CR_MTG_SDQP2_0_SERIAL_DQPAT2_0_MAX                      (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_SDQP2_0_SERIAL_DQPAT2_0_DEF                      (0x3333CCCC)
  #define MTG0_CR_MTG_SDQP2_0_SERIAL_DQPAT2_0_HSH                      (0x2000B270)

#define MTG0_CR_MTG_SDQP2_1_REG                                        (0x0000B274)
#define MTG0_CR_MTG_SDQP2_1_DEF                                        (0x6FAAFF00)
#define MTG0_CR_MTG_SDQP2_1_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define MTG0_CR_MTG_SDQP2_1_SERIAL_DQPAT2_1_OFF                      ( 0)
  #define MTG0_CR_MTG_SDQP2_1_SERIAL_DQPAT2_1_WID                      (32)
  #define MTG0_CR_MTG_SDQP2_1_SERIAL_DQPAT2_1_MSK                      (0xFFFFFFFF)
  #define MTG0_CR_MTG_SDQP2_1_SERIAL_DQPAT2_1_MIN                      (0)
  #define MTG0_CR_MTG_SDQP2_1_SERIAL_DQPAT2_1_MAX                      (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_SDQP2_1_SERIAL_DQPAT2_1_DEF                      (0x6FAAFF00)
  #define MTG0_CR_MTG_SDQP2_1_SERIAL_DQPAT2_1_HSH                      (0x2000B274)

#define MTG0_CR_MTG_SDQP3_0_REG                                        (0x0000B278)
#define MTG0_CR_MTG_SDQP3_0_DEF                                        (0x5555AAAA)
#define MTG0_CR_MTG_SDQP3_0_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define MTG0_CR_MTG_SDQP3_0_SERIAL_DQPAT3_0_OFF                      ( 0)
  #define MTG0_CR_MTG_SDQP3_0_SERIAL_DQPAT3_0_WID                      (32)
  #define MTG0_CR_MTG_SDQP3_0_SERIAL_DQPAT3_0_MSK                      (0xFFFFFFFF)
  #define MTG0_CR_MTG_SDQP3_0_SERIAL_DQPAT3_0_MIN                      (0)
  #define MTG0_CR_MTG_SDQP3_0_SERIAL_DQPAT3_0_MAX                      (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_SDQP3_0_SERIAL_DQPAT3_0_DEF                      (0x5555AAAA)
  #define MTG0_CR_MTG_SDQP3_0_SERIAL_DQPAT3_0_HSH                      (0x2000B278)

#define MTG0_CR_MTG_SDQP3_1_REG                                        (0x0000B27C)
#define MTG0_CR_MTG_SDQP3_1_DEF                                        (0x9955FF55)
#define MTG0_CR_MTG_SDQP3_1_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define MTG0_CR_MTG_SDQP3_1_SERIAL_DQPAT3_1_OFF                      ( 0)
  #define MTG0_CR_MTG_SDQP3_1_SERIAL_DQPAT3_1_WID                      (32)
  #define MTG0_CR_MTG_SDQP3_1_SERIAL_DQPAT3_1_MSK                      (0xFFFFFFFF)
  #define MTG0_CR_MTG_SDQP3_1_SERIAL_DQPAT3_1_MIN                      (0)
  #define MTG0_CR_MTG_SDQP3_1_SERIAL_DQPAT3_1_MAX                      (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_SDQP3_1_SERIAL_DQPAT3_1_DEF                      (0x9955FF55)
  #define MTG0_CR_MTG_SDQP3_1_SERIAL_DQPAT3_1_HSH                      (0x2000B27C)

#define MTG0_CR_MTG_SDMP0_0_REG                                        (0x0000B280)
#define MTG0_CR_MTG_SDMP0_0_DEF                                        (0x00FFFF00)
#define MTG0_CR_MTG_SDMP0_0_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define MTG0_CR_MTG_SDMP0_0_SERIAL_DMPAT0_0_OFF                      ( 0)
  #define MTG0_CR_MTG_SDMP0_0_SERIAL_DMPAT0_0_WID                      (32)
  #define MTG0_CR_MTG_SDMP0_0_SERIAL_DMPAT0_0_MSK                      (0xFFFFFFFF)
  #define MTG0_CR_MTG_SDMP0_0_SERIAL_DMPAT0_0_MIN                      (0)
  #define MTG0_CR_MTG_SDMP0_0_SERIAL_DMPAT0_0_MAX                      (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_SDMP0_0_SERIAL_DMPAT0_0_DEF                      (0x00FFFF00)
  #define MTG0_CR_MTG_SDMP0_0_SERIAL_DMPAT0_0_HSH                      (0x2000B280)

#define MTG0_CR_MTG_SDMP0_1_REG                                        (0x0000B284)
#define MTG0_CR_MTG_SDMP0_1_DEF                                        (0xFFAA00AA)
#define MTG0_CR_MTG_SDMP0_1_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define MTG0_CR_MTG_SDMP0_1_SERIAL_DMPAT0_1_OFF                      ( 0)
  #define MTG0_CR_MTG_SDMP0_1_SERIAL_DMPAT0_1_WID                      (32)
  #define MTG0_CR_MTG_SDMP0_1_SERIAL_DMPAT0_1_MSK                      (0xFFFFFFFF)
  #define MTG0_CR_MTG_SDMP0_1_SERIAL_DMPAT0_1_MIN                      (0)
  #define MTG0_CR_MTG_SDMP0_1_SERIAL_DMPAT0_1_MAX                      (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_SDMP0_1_SERIAL_DMPAT0_1_DEF                      (0xFFAA00AA)
  #define MTG0_CR_MTG_SDMP0_1_SERIAL_DMPAT0_1_HSH                      (0x2000B284)

#define MTG0_CR_MTG_SDMP1_0_REG                                        (0x0000B288)
#define MTG0_CR_MTG_SDMP1_0_DEF                                        (0x0F0FF0F0)
#define MTG0_CR_MTG_SDMP1_0_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define MTG0_CR_MTG_SDMP1_0_SERIAL_DMPAT1_0_OFF                      ( 0)
  #define MTG0_CR_MTG_SDMP1_0_SERIAL_DMPAT1_0_WID                      (32)
  #define MTG0_CR_MTG_SDMP1_0_SERIAL_DMPAT1_0_MSK                      (0xFFFFFFFF)
  #define MTG0_CR_MTG_SDMP1_0_SERIAL_DMPAT1_0_MIN                      (0)
  #define MTG0_CR_MTG_SDMP1_0_SERIAL_DMPAT1_0_MAX                      (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_SDMP1_0_SERIAL_DMPAT1_0_DEF                      (0x0F0FF0F0)
  #define MTG0_CR_MTG_SDMP1_0_SERIAL_DMPAT1_0_HSH                      (0x2000B288)

#define MTG0_CR_MTG_SDMP1_1_REG                                        (0x0000B28C)
#define MTG0_CR_MTG_SDMP1_1_DEF                                        (0xD755AA00)
#define MTG0_CR_MTG_SDMP1_1_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define MTG0_CR_MTG_SDMP1_1_SERIAL_DMPAT1_1_OFF                      ( 0)
  #define MTG0_CR_MTG_SDMP1_1_SERIAL_DMPAT1_1_WID                      (32)
  #define MTG0_CR_MTG_SDMP1_1_SERIAL_DMPAT1_1_MSK                      (0xFFFFFFFF)
  #define MTG0_CR_MTG_SDMP1_1_SERIAL_DMPAT1_1_MIN                      (0)
  #define MTG0_CR_MTG_SDMP1_1_SERIAL_DMPAT1_1_MAX                      (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_SDMP1_1_SERIAL_DMPAT1_1_DEF                      (0xD755AA00)
  #define MTG0_CR_MTG_SDMP1_1_SERIAL_DMPAT1_1_HSH                      (0x2000B28C)

#define MTG0_CR_MTG_SDPL_REG                                           (0x0000B290)
#define MTG0_CR_MTG_SDPL_DEF                                           (0x00333333)
#define MTG0_CR_MTG_SDPL_VOLATILE_BITFIELDS_MSK                        (0x00000000)

  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT0_LEN_OFF                       ( 0)
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT0_LEN_WID                       ( 2)
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT0_LEN_MSK                       (0x00000003)
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT0_LEN_MIN                       (0)
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT0_LEN_MAX                       (3) // 0x00000003
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT0_LEN_DEF                       (0x00000003)
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT0_LEN_HSH                       (0x0200B290)

  #define MTG0_CR_MTG_SDPL_RSVD3_2_OFF                                 ( 2)
  #define MTG0_CR_MTG_SDPL_RSVD3_2_WID                                 ( 2)
  #define MTG0_CR_MTG_SDPL_RSVD3_2_MSK                                 (0x0000000C)
  #define MTG0_CR_MTG_SDPL_RSVD3_2_MIN                                 (0)
  #define MTG0_CR_MTG_SDPL_RSVD3_2_MAX                                 (3) // 0x00000003
  #define MTG0_CR_MTG_SDPL_RSVD3_2_DEF                                 (0x00000000)
  #define MTG0_CR_MTG_SDPL_RSVD3_2_HSH                                 (0x0204B290)

  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT1_LEN_OFF                       ( 4)
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT1_LEN_WID                       ( 2)
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT1_LEN_MSK                       (0x00000030)
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT1_LEN_MIN                       (0)
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT1_LEN_MAX                       (3) // 0x00000003
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT1_LEN_DEF                       (0x00000003)
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT1_LEN_HSH                       (0x0208B290)

  #define MTG0_CR_MTG_SDPL_RSVD7_6_OFF                                 ( 6)
  #define MTG0_CR_MTG_SDPL_RSVD7_6_WID                                 ( 2)
  #define MTG0_CR_MTG_SDPL_RSVD7_6_MSK                                 (0x000000C0)
  #define MTG0_CR_MTG_SDPL_RSVD7_6_MIN                                 (0)
  #define MTG0_CR_MTG_SDPL_RSVD7_6_MAX                                 (3) // 0x00000003
  #define MTG0_CR_MTG_SDPL_RSVD7_6_DEF                                 (0x00000000)
  #define MTG0_CR_MTG_SDPL_RSVD7_6_HSH                                 (0x020CB290)

  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT2_LEN_OFF                       ( 8)
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT2_LEN_WID                       ( 2)
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT2_LEN_MSK                       (0x00000300)
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT2_LEN_MIN                       (0)
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT2_LEN_MAX                       (3) // 0x00000003
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT2_LEN_DEF                       (0x00000003)
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT2_LEN_HSH                       (0x0210B290)

  #define MTG0_CR_MTG_SDPL_RSVD11_10_OFF                               (10)
  #define MTG0_CR_MTG_SDPL_RSVD11_10_WID                               ( 2)
  #define MTG0_CR_MTG_SDPL_RSVD11_10_MSK                               (0x00000C00)
  #define MTG0_CR_MTG_SDPL_RSVD11_10_MIN                               (0)
  #define MTG0_CR_MTG_SDPL_RSVD11_10_MAX                               (3) // 0x00000003
  #define MTG0_CR_MTG_SDPL_RSVD11_10_DEF                               (0x00000000)
  #define MTG0_CR_MTG_SDPL_RSVD11_10_HSH                               (0x0214B290)

  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT3_LEN_OFF                       (12)
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT3_LEN_WID                       ( 2)
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT3_LEN_MSK                       (0x00003000)
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT3_LEN_MIN                       (0)
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT3_LEN_MAX                       (3) // 0x00000003
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT3_LEN_DEF                       (0x00000003)
  #define MTG0_CR_MTG_SDPL_SERIAL_DQPAT3_LEN_HSH                       (0x0218B290)

  #define MTG0_CR_MTG_SDPL_RSVD15_14_OFF                               (14)
  #define MTG0_CR_MTG_SDPL_RSVD15_14_WID                               ( 2)
  #define MTG0_CR_MTG_SDPL_RSVD15_14_MSK                               (0x0000C000)
  #define MTG0_CR_MTG_SDPL_RSVD15_14_MIN                               (0)
  #define MTG0_CR_MTG_SDPL_RSVD15_14_MAX                               (3) // 0x00000003
  #define MTG0_CR_MTG_SDPL_RSVD15_14_DEF                               (0x00000000)
  #define MTG0_CR_MTG_SDPL_RSVD15_14_HSH                               (0x021CB290)

  #define MTG0_CR_MTG_SDPL_SERIAL_DMPAT0_LEN_OFF                       (16)
  #define MTG0_CR_MTG_SDPL_SERIAL_DMPAT0_LEN_WID                       ( 2)
  #define MTG0_CR_MTG_SDPL_SERIAL_DMPAT0_LEN_MSK                       (0x00030000)
  #define MTG0_CR_MTG_SDPL_SERIAL_DMPAT0_LEN_MIN                       (0)
  #define MTG0_CR_MTG_SDPL_SERIAL_DMPAT0_LEN_MAX                       (3) // 0x00000003
  #define MTG0_CR_MTG_SDPL_SERIAL_DMPAT0_LEN_DEF                       (0x00000003)
  #define MTG0_CR_MTG_SDPL_SERIAL_DMPAT0_LEN_HSH                       (0x0220B290)

  #define MTG0_CR_MTG_SDPL_RSVD19_18_OFF                               (18)
  #define MTG0_CR_MTG_SDPL_RSVD19_18_WID                               ( 2)
  #define MTG0_CR_MTG_SDPL_RSVD19_18_MSK                               (0x000C0000)
  #define MTG0_CR_MTG_SDPL_RSVD19_18_MIN                               (0)
  #define MTG0_CR_MTG_SDPL_RSVD19_18_MAX                               (3) // 0x00000003
  #define MTG0_CR_MTG_SDPL_RSVD19_18_DEF                               (0x00000000)
  #define MTG0_CR_MTG_SDPL_RSVD19_18_HSH                               (0x0224B290)

  #define MTG0_CR_MTG_SDPL_SERIAL_DMPAT1_LEN_OFF                       (20)
  #define MTG0_CR_MTG_SDPL_SERIAL_DMPAT1_LEN_WID                       ( 2)
  #define MTG0_CR_MTG_SDPL_SERIAL_DMPAT1_LEN_MSK                       (0x00300000)
  #define MTG0_CR_MTG_SDPL_SERIAL_DMPAT1_LEN_MIN                       (0)
  #define MTG0_CR_MTG_SDPL_SERIAL_DMPAT1_LEN_MAX                       (3) // 0x00000003
  #define MTG0_CR_MTG_SDPL_SERIAL_DMPAT1_LEN_DEF                       (0x00000003)
  #define MTG0_CR_MTG_SDPL_SERIAL_DMPAT1_LEN_HSH                       (0x0228B290)

  #define MTG0_CR_MTG_SDPL_RSVD31_22_OFF                               (22)
  #define MTG0_CR_MTG_SDPL_RSVD31_22_WID                               (10)
  #define MTG0_CR_MTG_SDPL_RSVD31_22_MSK                               (0xFFC00000)
  #define MTG0_CR_MTG_SDPL_RSVD31_22_MIN                               (0)
  #define MTG0_CR_MTG_SDPL_RSVD31_22_MAX                               (1023) // 0x000003FF
  #define MTG0_CR_MTG_SDPL_RSVD31_22_DEF                               (0x00000000)
  #define MTG0_CR_MTG_SDPL_RSVD31_22_HSH                               (0x0A2CB290)

#define MTG0_CR_MTG_DLFSR_POLY_REG                                     (0x0000B294)
#define MTG0_CR_MTG_DLFSR_POLY_DEF                                     (0x00110000)
#define MTG0_CR_MTG_DLFSR_POLY_VOLATILE_BITFIELDS_MSK                  (0x00000000)

  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR0_POLY_OFF                     ( 0)
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR0_POLY_WID                     ( 3)
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR0_POLY_MSK                     (0x00000007)
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR0_POLY_MIN                     (0)
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR0_POLY_MAX                     (7) // 0x00000007
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR0_POLY_DEF                     (0x00000000)
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR0_POLY_HSH                     (0x0300B294)

  #define MTG0_CR_MTG_DLFSR_POLY_RSVD3_OFF                             ( 3)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD3_WID                             ( 1)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD3_MSK                             (0x00000008)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD3_MIN                             (0)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD3_MAX                             (1) // 0x00000001
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD3_DEF                             (0x00000000)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD3_HSH                             (0x0106B294)

  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR1_POLY_OFF                     ( 4)
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR1_POLY_WID                     ( 3)
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR1_POLY_MSK                     (0x00000070)
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR1_POLY_MIN                     (0)
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR1_POLY_MAX                     (7) // 0x00000007
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR1_POLY_DEF                     (0x00000000)
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR1_POLY_HSH                     (0x0308B294)

  #define MTG0_CR_MTG_DLFSR_POLY_RSVD7_OFF                             ( 7)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD7_WID                             ( 1)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD7_MSK                             (0x00000080)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD7_MIN                             (0)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD7_MAX                             (1) // 0x00000001
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD7_DEF                             (0x00000000)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD7_HSH                             (0x010EB294)

  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR2_POLY_OFF                     ( 8)
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR2_POLY_WID                     ( 3)
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR2_POLY_MSK                     (0x00000700)
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR2_POLY_MIN                     (0)
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR2_POLY_MAX                     (7) // 0x00000007
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR2_POLY_DEF                     (0x00000000)
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR2_POLY_HSH                     (0x0310B294)

  #define MTG0_CR_MTG_DLFSR_POLY_RSVD11_OFF                            (11)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD11_WID                            ( 1)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD11_MSK                            (0x00000800)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD11_MIN                            (0)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD11_MAX                            (1) // 0x00000001
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD11_DEF                            (0x00000000)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD11_HSH                            (0x0116B294)

  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR3_POLY_OFF                     (12)
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR3_POLY_WID                     ( 3)
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR3_POLY_MSK                     (0x00007000)
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR3_POLY_MIN                     (0)
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR3_POLY_MAX                     (7) // 0x00000007
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR3_POLY_DEF                     (0x00000000)
  #define MTG0_CR_MTG_DLFSR_POLY_DQ_LFSR3_POLY_HSH                     (0x0318B294)

  #define MTG0_CR_MTG_DLFSR_POLY_RSVD15_OFF                            (15)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD15_WID                            ( 1)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD15_MSK                            (0x00008000)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD15_MIN                            (0)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD15_MAX                            (1) // 0x00000001
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD15_DEF                            (0x00000000)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD15_HSH                            (0x011EB294)

  #define MTG0_CR_MTG_DLFSR_POLY_DM_LFSR0_POLY_OFF                     (16)
  #define MTG0_CR_MTG_DLFSR_POLY_DM_LFSR0_POLY_WID                     ( 3)
  #define MTG0_CR_MTG_DLFSR_POLY_DM_LFSR0_POLY_MSK                     (0x00070000)
  #define MTG0_CR_MTG_DLFSR_POLY_DM_LFSR0_POLY_MIN                     (0)
  #define MTG0_CR_MTG_DLFSR_POLY_DM_LFSR0_POLY_MAX                     (7) // 0x00000007
  #define MTG0_CR_MTG_DLFSR_POLY_DM_LFSR0_POLY_DEF                     (0x00000001)
  #define MTG0_CR_MTG_DLFSR_POLY_DM_LFSR0_POLY_HSH                     (0x0320B294)

  #define MTG0_CR_MTG_DLFSR_POLY_RSVD19_OFF                            (19)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD19_WID                            ( 1)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD19_MSK                            (0x00080000)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD19_MIN                            (0)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD19_MAX                            (1) // 0x00000001
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD19_DEF                            (0x00000000)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD19_HSH                            (0x0126B294)

  #define MTG0_CR_MTG_DLFSR_POLY_DM_LFSR1_POLY_OFF                     (20)
  #define MTG0_CR_MTG_DLFSR_POLY_DM_LFSR1_POLY_WID                     ( 3)
  #define MTG0_CR_MTG_DLFSR_POLY_DM_LFSR1_POLY_MSK                     (0x00700000)
  #define MTG0_CR_MTG_DLFSR_POLY_DM_LFSR1_POLY_MIN                     (0)
  #define MTG0_CR_MTG_DLFSR_POLY_DM_LFSR1_POLY_MAX                     (7) // 0x00000007
  #define MTG0_CR_MTG_DLFSR_POLY_DM_LFSR1_POLY_DEF                     (0x00000001)
  #define MTG0_CR_MTG_DLFSR_POLY_DM_LFSR1_POLY_HSH                     (0x0328B294)

  #define MTG0_CR_MTG_DLFSR_POLY_RSVD31_23_OFF                         (23)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD31_23_WID                         ( 9)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD31_23_MSK                         (0xFF800000)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD31_23_MIN                         (0)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD31_23_MAX                         (511) // 0x000001FF
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD31_23_DEF                         (0x00000000)
  #define MTG0_CR_MTG_DLFSR_POLY_RSVD31_23_HSH                         (0x092EB294)

#define MTG0_CR_MTG_DLFSRX_POLY_REG                                    (0x0000B298)
#define MTG0_CR_MTG_DLFSRX_POLY_DEF                                    (0x000000B8)
#define MTG0_CR_MTG_DLFSRX_POLY_VOLATILE_BITFIELDS_MSK                 (0x00000000)

  #define MTG0_CR_MTG_DLFSRX_POLY_DQM_LFSRX_POLY_OFF                   ( 0)
  #define MTG0_CR_MTG_DLFSRX_POLY_DQM_LFSRX_POLY_WID                   (32)
  #define MTG0_CR_MTG_DLFSRX_POLY_DQM_LFSRX_POLY_MSK                   (0xFFFFFFFF)
  #define MTG0_CR_MTG_DLFSRX_POLY_DQM_LFSRX_POLY_MIN                   (0)
  #define MTG0_CR_MTG_DLFSRX_POLY_DQM_LFSRX_POLY_MAX                   (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_DLFSRX_POLY_DQM_LFSRX_POLY_DEF                   (0x000000B8)
  #define MTG0_CR_MTG_DLFSRX_POLY_DQM_LFSRX_POLY_HSH                   (0x2000B298)

#define MTG0_CR_MTG_DQLFSR0_SEED_REG                                   (0x0000B2B0)
#define MTG0_CR_MTG_DQLFSR0_SEED_DEF                                   (0x00000001)
#define MTG0_CR_MTG_DQLFSR0_SEED_VOLATILE_BITFIELDS_MSK                (0x00000000)

  #define MTG0_CR_MTG_DQLFSR0_SEED_DQ_LFSR0_SEED_OFF                   ( 0)
  #define MTG0_CR_MTG_DQLFSR0_SEED_DQ_LFSR0_SEED_WID                   (32)
  #define MTG0_CR_MTG_DQLFSR0_SEED_DQ_LFSR0_SEED_MSK                   (0xFFFFFFFF)
  #define MTG0_CR_MTG_DQLFSR0_SEED_DQ_LFSR0_SEED_MIN                   (0)
  #define MTG0_CR_MTG_DQLFSR0_SEED_DQ_LFSR0_SEED_MAX                   (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_DQLFSR0_SEED_DQ_LFSR0_SEED_DEF                   (0x00000001)
  #define MTG0_CR_MTG_DQLFSR0_SEED_DQ_LFSR0_SEED_HSH                   (0x2000B2B0)

#define MTG0_CR_MTG_DQLFSR1_SEED_REG                                   (0x0000B2B4)
#define MTG0_CR_MTG_DQLFSR1_SEED_DEF                                   (0x00000002)
#define MTG0_CR_MTG_DQLFSR1_SEED_VOLATILE_BITFIELDS_MSK                (0x00000000)

  #define MTG0_CR_MTG_DQLFSR1_SEED_DQ_LFSR1_SEED_OFF                   ( 0)
  #define MTG0_CR_MTG_DQLFSR1_SEED_DQ_LFSR1_SEED_WID                   (32)
  #define MTG0_CR_MTG_DQLFSR1_SEED_DQ_LFSR1_SEED_MSK                   (0xFFFFFFFF)
  #define MTG0_CR_MTG_DQLFSR1_SEED_DQ_LFSR1_SEED_MIN                   (0)
  #define MTG0_CR_MTG_DQLFSR1_SEED_DQ_LFSR1_SEED_MAX                   (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_DQLFSR1_SEED_DQ_LFSR1_SEED_DEF                   (0x00000002)
  #define MTG0_CR_MTG_DQLFSR1_SEED_DQ_LFSR1_SEED_HSH                   (0x2000B2B4)

#define MTG0_CR_MTG_DQLFSR2_SEED_REG                                   (0x0000B2B8)
#define MTG0_CR_MTG_DQLFSR2_SEED_DEF                                   (0x00000003)
#define MTG0_CR_MTG_DQLFSR2_SEED_VOLATILE_BITFIELDS_MSK                (0x00000000)

  #define MTG0_CR_MTG_DQLFSR2_SEED_DQ_LFSR2_SEED_OFF                   ( 0)
  #define MTG0_CR_MTG_DQLFSR2_SEED_DQ_LFSR2_SEED_WID                   (32)
  #define MTG0_CR_MTG_DQLFSR2_SEED_DQ_LFSR2_SEED_MSK                   (0xFFFFFFFF)
  #define MTG0_CR_MTG_DQLFSR2_SEED_DQ_LFSR2_SEED_MIN                   (0)
  #define MTG0_CR_MTG_DQLFSR2_SEED_DQ_LFSR2_SEED_MAX                   (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_DQLFSR2_SEED_DQ_LFSR2_SEED_DEF                   (0x00000003)
  #define MTG0_CR_MTG_DQLFSR2_SEED_DQ_LFSR2_SEED_HSH                   (0x2000B2B8)

#define MTG0_CR_MTG_DQLFSR3_SEED_REG                                   (0x0000B2BC)
#define MTG0_CR_MTG_DQLFSR3_SEED_DEF                                   (0x00000004)
#define MTG0_CR_MTG_DQLFSR3_SEED_VOLATILE_BITFIELDS_MSK                (0x00000000)

  #define MTG0_CR_MTG_DQLFSR3_SEED_DQ_LFSR3_SEED_OFF                   ( 0)
  #define MTG0_CR_MTG_DQLFSR3_SEED_DQ_LFSR3_SEED_WID                   (32)
  #define MTG0_CR_MTG_DQLFSR3_SEED_DQ_LFSR3_SEED_MSK                   (0xFFFFFFFF)
  #define MTG0_CR_MTG_DQLFSR3_SEED_DQ_LFSR3_SEED_MIN                   (0)
  #define MTG0_CR_MTG_DQLFSR3_SEED_DQ_LFSR3_SEED_MAX                   (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_DQLFSR3_SEED_DQ_LFSR3_SEED_DEF                   (0x00000004)
  #define MTG0_CR_MTG_DQLFSR3_SEED_DQ_LFSR3_SEED_HSH                   (0x2000B2BC)

#define MTG0_CR_MTG_DMLFSR0_SEED_REG                                   (0x0000B2C0)
#define MTG0_CR_MTG_DMLFSR0_SEED_DEF                                   (0x00000005)
#define MTG0_CR_MTG_DMLFSR0_SEED_VOLATILE_BITFIELDS_MSK                (0x00000000)

  #define MTG0_CR_MTG_DMLFSR0_SEED_DM_LFSR0_SEED_OFF                   ( 0)
  #define MTG0_CR_MTG_DMLFSR0_SEED_DM_LFSR0_SEED_WID                   (32)
  #define MTG0_CR_MTG_DMLFSR0_SEED_DM_LFSR0_SEED_MSK                   (0xFFFFFFFF)
  #define MTG0_CR_MTG_DMLFSR0_SEED_DM_LFSR0_SEED_MIN                   (0)
  #define MTG0_CR_MTG_DMLFSR0_SEED_DM_LFSR0_SEED_MAX                   (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_DMLFSR0_SEED_DM_LFSR0_SEED_DEF                   (0x00000005)
  #define MTG0_CR_MTG_DMLFSR0_SEED_DM_LFSR0_SEED_HSH                   (0x2000B2C0)

#define MTG0_CR_MTG_DMLFSR1_SEED_REG                                   (0x0000B2C4)
#define MTG0_CR_MTG_DMLFSR1_SEED_DEF                                   (0x00000006)
#define MTG0_CR_MTG_DMLFSR1_SEED_VOLATILE_BITFIELDS_MSK                (0x00000000)

  #define MTG0_CR_MTG_DMLFSR1_SEED_DM_LFSR1_SEED_OFF                   ( 0)
  #define MTG0_CR_MTG_DMLFSR1_SEED_DM_LFSR1_SEED_WID                   (32)
  #define MTG0_CR_MTG_DMLFSR1_SEED_DM_LFSR1_SEED_MSK                   (0xFFFFFFFF)
  #define MTG0_CR_MTG_DMLFSR1_SEED_DM_LFSR1_SEED_MIN                   (0)
  #define MTG0_CR_MTG_DMLFSR1_SEED_DM_LFSR1_SEED_MAX                   (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_DMLFSR1_SEED_DM_LFSR1_SEED_DEF                   (0x00000006)
  #define MTG0_CR_MTG_DMLFSR1_SEED_DM_LFSR1_SEED_HSH                   (0x2000B2C4)

#define MTG0_CR_MTG_DQC_REG                                            (0x0000B2D0)
#define MTG0_CR_MTG_DQC_DEF                                            (0x00F00000)
#define MTG0_CR_MTG_DQC_VOLATILE_BITFIELDS_MSK                         (0x00000000)

  #define MTG0_CR_MTG_DQC_DQPAT_MODE_OFF                               ( 0)
  #define MTG0_CR_MTG_DQC_DQPAT_MODE_WID                               ( 4)
  #define MTG0_CR_MTG_DQC_DQPAT_MODE_MSK                               (0x0000000F)
  #define MTG0_CR_MTG_DQC_DQPAT_MODE_MIN                               (0)
  #define MTG0_CR_MTG_DQC_DQPAT_MODE_MAX                               (15) // 0x0000000F
  #define MTG0_CR_MTG_DQC_DQPAT_MODE_DEF                               (0x00000000)
  #define MTG0_CR_MTG_DQC_DQPAT_MODE_HSH                               (0x0400B2D0)

  #define MTG0_CR_MTG_DQC_DQPAT_RATE_OFF                               ( 4)
  #define MTG0_CR_MTG_DQC_DQPAT_RATE_WID                               ( 8)
  #define MTG0_CR_MTG_DQC_DQPAT_RATE_MSK                               (0x00000FF0)
  #define MTG0_CR_MTG_DQC_DQPAT_RATE_MIN                               (0)
  #define MTG0_CR_MTG_DQC_DQPAT_RATE_MAX                               (255) // 0x000000FF
  #define MTG0_CR_MTG_DQC_DQPAT_RATE_DEF                               (0x00000000)
  #define MTG0_CR_MTG_DQC_DQPAT_RATE_HSH                               (0x0808B2D0)

  #define MTG0_CR_MTG_DQC_VICTIM0_DQPAT_SEL_OFF                        (12)
  #define MTG0_CR_MTG_DQC_VICTIM0_DQPAT_SEL_WID                        ( 1)
  #define MTG0_CR_MTG_DQC_VICTIM0_DQPAT_SEL_MSK                        (0x00001000)
  #define MTG0_CR_MTG_DQC_VICTIM0_DQPAT_SEL_MIN                        (0)
  #define MTG0_CR_MTG_DQC_VICTIM0_DQPAT_SEL_MAX                        (1) // 0x00000001
  #define MTG0_CR_MTG_DQC_VICTIM0_DQPAT_SEL_DEF                        (0x00000000)
  #define MTG0_CR_MTG_DQC_VICTIM0_DQPAT_SEL_HSH                        (0x0118B2D0)

  #define MTG0_CR_MTG_DQC_VICTIM1_DQPAT_SEL_OFF                        (13)
  #define MTG0_CR_MTG_DQC_VICTIM1_DQPAT_SEL_WID                        ( 1)
  #define MTG0_CR_MTG_DQC_VICTIM1_DQPAT_SEL_MSK                        (0x00002000)
  #define MTG0_CR_MTG_DQC_VICTIM1_DQPAT_SEL_MIN                        (0)
  #define MTG0_CR_MTG_DQC_VICTIM1_DQPAT_SEL_MAX                        (1) // 0x00000001
  #define MTG0_CR_MTG_DQC_VICTIM1_DQPAT_SEL_DEF                        (0x00000000)
  #define MTG0_CR_MTG_DQC_VICTIM1_DQPAT_SEL_HSH                        (0x011AB2D0)

  #define MTG0_CR_MTG_DQC_AGGRESSOR0_DQPAT_SEL_OFF                     (14)
  #define MTG0_CR_MTG_DQC_AGGRESSOR0_DQPAT_SEL_WID                     ( 1)
  #define MTG0_CR_MTG_DQC_AGGRESSOR0_DQPAT_SEL_MSK                     (0x00004000)
  #define MTG0_CR_MTG_DQC_AGGRESSOR0_DQPAT_SEL_MIN                     (0)
  #define MTG0_CR_MTG_DQC_AGGRESSOR0_DQPAT_SEL_MAX                     (1) // 0x00000001
  #define MTG0_CR_MTG_DQC_AGGRESSOR0_DQPAT_SEL_DEF                     (0x00000000)
  #define MTG0_CR_MTG_DQC_AGGRESSOR0_DQPAT_SEL_HSH                     (0x011CB2D0)

  #define MTG0_CR_MTG_DQC_AGGRESSOR1_DQPAT_SEL_OFF                     (15)
  #define MTG0_CR_MTG_DQC_AGGRESSOR1_DQPAT_SEL_WID                     ( 1)
  #define MTG0_CR_MTG_DQC_AGGRESSOR1_DQPAT_SEL_MSK                     (0x00008000)
  #define MTG0_CR_MTG_DQC_AGGRESSOR1_DQPAT_SEL_MIN                     (0)
  #define MTG0_CR_MTG_DQC_AGGRESSOR1_DQPAT_SEL_MAX                     (1) // 0x00000001
  #define MTG0_CR_MTG_DQC_AGGRESSOR1_DQPAT_SEL_DEF                     (0x00000000)
  #define MTG0_CR_MTG_DQC_AGGRESSOR1_DQPAT_SEL_HSH                     (0x011EB2D0)

  #define MTG0_CR_MTG_DQC_RST_DQPAT_ON_ROTATE_OFF                      (16)
  #define MTG0_CR_MTG_DQC_RST_DQPAT_ON_ROTATE_WID                      ( 1)
  #define MTG0_CR_MTG_DQC_RST_DQPAT_ON_ROTATE_MSK                      (0x00010000)
  #define MTG0_CR_MTG_DQC_RST_DQPAT_ON_ROTATE_MIN                      (0)
  #define MTG0_CR_MTG_DQC_RST_DQPAT_ON_ROTATE_MAX                      (1) // 0x00000001
  #define MTG0_CR_MTG_DQC_RST_DQPAT_ON_ROTATE_DEF                      (0x00000000)
  #define MTG0_CR_MTG_DQC_RST_DQPAT_ON_ROTATE_HSH                      (0x0120B2D0)

  #define MTG0_CR_MTG_DQC_RSVD18_17_OFF                                (17)
  #define MTG0_CR_MTG_DQC_RSVD18_17_WID                                ( 2)
  #define MTG0_CR_MTG_DQC_RSVD18_17_MSK                                (0x00060000)
  #define MTG0_CR_MTG_DQC_RSVD18_17_MIN                                (0)
  #define MTG0_CR_MTG_DQC_RSVD18_17_MAX                                (3) // 0x00000003
  #define MTG0_CR_MTG_DQC_RSVD18_17_DEF                                (0x00000000)
  #define MTG0_CR_MTG_DQC_RSVD18_17_HSH                                (0x0222B2D0)

  #define MTG0_CR_MTG_DQC_STATIC_DMPAT_EN_OFF                          (19)
  #define MTG0_CR_MTG_DQC_STATIC_DMPAT_EN_WID                          ( 1)
  #define MTG0_CR_MTG_DQC_STATIC_DMPAT_EN_MSK                          (0x00080000)
  #define MTG0_CR_MTG_DQC_STATIC_DMPAT_EN_MIN                          (0)
  #define MTG0_CR_MTG_DQC_STATIC_DMPAT_EN_MAX                          (1) // 0x00000001
  #define MTG0_CR_MTG_DQC_STATIC_DMPAT_EN_DEF                          (0x00000000)
  #define MTG0_CR_MTG_DQC_STATIC_DMPAT_EN_HSH                          (0x0126B2D0)

  #define MTG0_CR_MTG_DQC_SERIAL_CHAR_EN_OFF                           (20)
  #define MTG0_CR_MTG_DQC_SERIAL_CHAR_EN_WID                           ( 4)
  #define MTG0_CR_MTG_DQC_SERIAL_CHAR_EN_MSK                           (0x00F00000)
  #define MTG0_CR_MTG_DQC_SERIAL_CHAR_EN_MIN                           (0)
  #define MTG0_CR_MTG_DQC_SERIAL_CHAR_EN_MAX                           (15) // 0x0000000F
  #define MTG0_CR_MTG_DQC_SERIAL_CHAR_EN_DEF                           (0x0000000F)
  #define MTG0_CR_MTG_DQC_SERIAL_CHAR_EN_HSH                           (0x0428B2D0)

  #define MTG0_CR_MTG_DQC_RSVD27_24_OFF                                (24)
  #define MTG0_CR_MTG_DQC_RSVD27_24_WID                                ( 4)
  #define MTG0_CR_MTG_DQC_RSVD27_24_MSK                                (0x0F000000)
  #define MTG0_CR_MTG_DQC_RSVD27_24_MIN                                (0)
  #define MTG0_CR_MTG_DQC_RSVD27_24_MAX                                (15) // 0x0000000F
  #define MTG0_CR_MTG_DQC_RSVD27_24_DEF                                (0x00000000)
  #define MTG0_CR_MTG_DQC_RSVD27_24_HSH                                (0x0430B2D0)

  #define MTG0_CR_MTG_DQC_DMPAT_AS_ECCPAT_OFF                          (28)
  #define MTG0_CR_MTG_DQC_DMPAT_AS_ECCPAT_WID                          ( 1)
  #define MTG0_CR_MTG_DQC_DMPAT_AS_ECCPAT_MSK                          (0x10000000)
  #define MTG0_CR_MTG_DQC_DMPAT_AS_ECCPAT_MIN                          (0)
  #define MTG0_CR_MTG_DQC_DMPAT_AS_ECCPAT_MAX                          (1) // 0x00000001
  #define MTG0_CR_MTG_DQC_DMPAT_AS_ECCPAT_DEF                          (0x00000000)
  #define MTG0_CR_MTG_DQC_DMPAT_AS_ECCPAT_HSH                          (0x0138B2D0)

  #define MTG0_CR_MTG_DQC_ECC_UI_OV_OFF                                (29)
  #define MTG0_CR_MTG_DQC_ECC_UI_OV_WID                                ( 2)
  #define MTG0_CR_MTG_DQC_ECC_UI_OV_MSK                                (0x60000000)
  #define MTG0_CR_MTG_DQC_ECC_UI_OV_MIN                                (0)
  #define MTG0_CR_MTG_DQC_ECC_UI_OV_MAX                                (3) // 0x00000003
  #define MTG0_CR_MTG_DQC_ECC_UI_OV_DEF                                (0x00000000)
  #define MTG0_CR_MTG_DQC_ECC_UI_OV_HSH                                (0x023AB2D0)

  #define MTG0_CR_MTG_DQC_RSVD31_OFF                                   (31)
  #define MTG0_CR_MTG_DQC_RSVD31_WID                                   ( 1)
  #define MTG0_CR_MTG_DQC_RSVD31_MSK                                   (0x80000000)
  #define MTG0_CR_MTG_DQC_RSVD31_MIN                                   (0)
  #define MTG0_CR_MTG_DQC_RSVD31_MAX                                   (1) // 0x00000001
  #define MTG0_CR_MTG_DQC_RSVD31_DEF                                   (0x00000000)
  #define MTG0_CR_MTG_DQC_RSVD31_HSH                                   (0x013EB2D0)

#define MTG0_CR_MTG_DQDCC0_REG                                         (0x0000B2D4)
#define MTG0_CR_MTG_DQDCC0_DEF                                         (0x00000000)
#define MTG0_CR_MTG_DQDCC0_VOLATILE_BITFIELDS_MSK                      (0x00000000)

  #define MTG0_CR_MTG_DQDCC0_DQ_DC_0_OFF                               ( 0)
  #define MTG0_CR_MTG_DQDCC0_DQ_DC_0_WID                               (32)
  #define MTG0_CR_MTG_DQDCC0_DQ_DC_0_MSK                               (0xFFFFFFFF)
  #define MTG0_CR_MTG_DQDCC0_DQ_DC_0_MIN                               (0)
  #define MTG0_CR_MTG_DQDCC0_DQ_DC_0_MAX                               (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_DQDCC0_DQ_DC_0_DEF                               (0x00000000)
  #define MTG0_CR_MTG_DQDCC0_DQ_DC_0_HSH                               (0x2000B2D4)

#define MTG0_CR_MTG_DQDCC1_REG                                         (0x0000B2D8)
#define MTG0_CR_MTG_DQDCC1_DEF                                         (0x00000000)
#define MTG0_CR_MTG_DQDCC1_VOLATILE_BITFIELDS_MSK                      (0x00000000)

  #define MTG0_CR_MTG_DQDCC1_DQ_DC_1_OFF                               ( 0)
  #define MTG0_CR_MTG_DQDCC1_DQ_DC_1_WID                               ( 8)
  #define MTG0_CR_MTG_DQDCC1_DQ_DC_1_MSK                               (0x000000FF)
  #define MTG0_CR_MTG_DQDCC1_DQ_DC_1_MIN                               (0)
  #define MTG0_CR_MTG_DQDCC1_DQ_DC_1_MAX                               (255) // 0x000000FF
  #define MTG0_CR_MTG_DQDCC1_DQ_DC_1_DEF                               (0x00000000)
  #define MTG0_CR_MTG_DQDCC1_DQ_DC_1_HSH                               (0x0800B2D8)

  #define MTG0_CR_MTG_DQDCC1_RSVD31_8_OFF                              ( 8)
  #define MTG0_CR_MTG_DQDCC1_RSVD31_8_WID                              (24)
  #define MTG0_CR_MTG_DQDCC1_RSVD31_8_MSK                              (0xFFFFFF00)
  #define MTG0_CR_MTG_DQDCC1_RSVD31_8_MIN                              (0)
  #define MTG0_CR_MTG_DQDCC1_RSVD31_8_MAX                              (16777215) // 0x00FFFFFF
  #define MTG0_CR_MTG_DQDCC1_RSVD31_8_DEF                              (0x00000000)
  #define MTG0_CR_MTG_DQDCC1_RSVD31_8_HSH                              (0x1810B2D8)

#define MTG0_CR_MTG_DQINVC0_REG                                        (0x0000B2DC)
#define MTG0_CR_MTG_DQINVC0_DEF                                        (0x00000000)
#define MTG0_CR_MTG_DQINVC0_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define MTG0_CR_MTG_DQINVC0_DQ_INV_0_OFF                             ( 0)
  #define MTG0_CR_MTG_DQINVC0_DQ_INV_0_WID                             (32)
  #define MTG0_CR_MTG_DQINVC0_DQ_INV_0_MSK                             (0xFFFFFFFF)
  #define MTG0_CR_MTG_DQINVC0_DQ_INV_0_MIN                             (0)
  #define MTG0_CR_MTG_DQINVC0_DQ_INV_0_MAX                             (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_DQINVC0_DQ_INV_0_DEF                             (0x00000000)
  #define MTG0_CR_MTG_DQINVC0_DQ_INV_0_HSH                             (0x2000B2DC)

#define MTG0_CR_MTG_DQINVC1_REG                                        (0x0000B2E0)
#define MTG0_CR_MTG_DQINVC1_DEF                                        (0x00000000)
#define MTG0_CR_MTG_DQINVC1_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define MTG0_CR_MTG_DQINVC1_DQ_INV_1_OFF                             ( 0)
  #define MTG0_CR_MTG_DQINVC1_DQ_INV_1_WID                             ( 8)
  #define MTG0_CR_MTG_DQINVC1_DQ_INV_1_MSK                             (0x000000FF)
  #define MTG0_CR_MTG_DQINVC1_DQ_INV_1_MIN                             (0)
  #define MTG0_CR_MTG_DQINVC1_DQ_INV_1_MAX                             (255) // 0x000000FF
  #define MTG0_CR_MTG_DQINVC1_DQ_INV_1_DEF                             (0x00000000)
  #define MTG0_CR_MTG_DQINVC1_DQ_INV_1_HSH                             (0x0800B2E0)

  #define MTG0_CR_MTG_DQINVC1_RSVD31_8_OFF                             ( 8)
  #define MTG0_CR_MTG_DQINVC1_RSVD31_8_WID                             (24)
  #define MTG0_CR_MTG_DQINVC1_RSVD31_8_MSK                             (0xFFFFFF00)
  #define MTG0_CR_MTG_DQINVC1_RSVD31_8_MIN                             (0)
  #define MTG0_CR_MTG_DQINVC1_RSVD31_8_MAX                             (16777215) // 0x00FFFFFF
  #define MTG0_CR_MTG_DQINVC1_RSVD31_8_DEF                             (0x00000000)
  #define MTG0_CR_MTG_DQINVC1_RSVD31_8_HSH                             (0x1810B2E0)

#define MTG0_CR_MTG_DMDCINVC_REG                                       (0x0000B2E4)
#define MTG0_CR_MTG_DMDCINVC_DEF                                       (0x00000000)
#define MTG0_CR_MTG_DMDCINVC_VOLATILE_BITFIELDS_MSK                    (0x00000000)

  #define MTG0_CR_MTG_DMDCINVC_DM_DC_OFF                               ( 0)
  #define MTG0_CR_MTG_DMDCINVC_DM_DC_WID                               ( 4)
  #define MTG0_CR_MTG_DMDCINVC_DM_DC_MSK                               (0x0000000F)
  #define MTG0_CR_MTG_DMDCINVC_DM_DC_MIN                               (0)
  #define MTG0_CR_MTG_DMDCINVC_DM_DC_MAX                               (15) // 0x0000000F
  #define MTG0_CR_MTG_DMDCINVC_DM_DC_DEF                               (0x00000000)
  #define MTG0_CR_MTG_DMDCINVC_DM_DC_HSH                               (0x0400B2E4)

  #define MTG0_CR_MTG_DMDCINVC_RSVD15_4_OFF                            ( 4)
  #define MTG0_CR_MTG_DMDCINVC_RSVD15_4_WID                            (12)
  #define MTG0_CR_MTG_DMDCINVC_RSVD15_4_MSK                            (0x0000FFF0)
  #define MTG0_CR_MTG_DMDCINVC_RSVD15_4_MIN                            (0)
  #define MTG0_CR_MTG_DMDCINVC_RSVD15_4_MAX                            (4095) // 0x00000FFF
  #define MTG0_CR_MTG_DMDCINVC_RSVD15_4_DEF                            (0x00000000)
  #define MTG0_CR_MTG_DMDCINVC_RSVD15_4_HSH                            (0x0C08B2E4)

  #define MTG0_CR_MTG_DMDCINVC_DM_INV_OFF                              (16)
  #define MTG0_CR_MTG_DMDCINVC_DM_INV_WID                              ( 4)
  #define MTG0_CR_MTG_DMDCINVC_DM_INV_MSK                              (0x000F0000)
  #define MTG0_CR_MTG_DMDCINVC_DM_INV_MIN                              (0)
  #define MTG0_CR_MTG_DMDCINVC_DM_INV_MAX                              (15) // 0x0000000F
  #define MTG0_CR_MTG_DMDCINVC_DM_INV_DEF                              (0x00000000)
  #define MTG0_CR_MTG_DMDCINVC_DM_INV_HSH                              (0x0420B2E4)

  #define MTG0_CR_MTG_DMDCINVC_RSVD31_20_OFF                           (20)
  #define MTG0_CR_MTG_DMDCINVC_RSVD31_20_WID                           (12)
  #define MTG0_CR_MTG_DMDCINVC_RSVD31_20_MSK                           (0xFFF00000)
  #define MTG0_CR_MTG_DMDCINVC_RSVD31_20_MIN                           (0)
  #define MTG0_CR_MTG_DMDCINVC_RSVD31_20_MAX                           (4095) // 0x00000FFF
  #define MTG0_CR_MTG_DMDCINVC_RSVD31_20_DEF                           (0x00000000)
  #define MTG0_CR_MTG_DMDCINVC_RSVD31_20_HSH                           (0x0C28B2E4)

#define MTG0_CR_MTG_DBAC_REG                                           (0x0000B350)
#define MTG0_CR_MTG_DBAC_DEF                                           (0x00000000)
#define MTG0_CR_MTG_DBAC_VOLATILE_BITFIELDS_MSK                        (0x07DF0000)

  #define MTG0_CR_MTG_DBAC_CHAN_SEL_OFF                                ( 0)
  #define MTG0_CR_MTG_DBAC_CHAN_SEL_WID                                ( 2)
  #define MTG0_CR_MTG_DBAC_CHAN_SEL_MSK                                (0x00000003)
  #define MTG0_CR_MTG_DBAC_CHAN_SEL_MIN                                (0)
  #define MTG0_CR_MTG_DBAC_CHAN_SEL_MAX                                (3) // 0x00000003
  #define MTG0_CR_MTG_DBAC_CHAN_SEL_DEF                                (0x00000000)
  #define MTG0_CR_MTG_DBAC_CHAN_SEL_HSH                                (0x0200B350)

  #define MTG0_CR_MTG_DBAC_DATABUF_SEL_OFF                             ( 2)
  #define MTG0_CR_MTG_DBAC_DATABUF_SEL_WID                             ( 2)
  #define MTG0_CR_MTG_DBAC_DATABUF_SEL_MSK                             (0x0000000C)
  #define MTG0_CR_MTG_DBAC_DATABUF_SEL_MIN                             (0)
  #define MTG0_CR_MTG_DBAC_DATABUF_SEL_MAX                             (3) // 0x00000003
  #define MTG0_CR_MTG_DBAC_DATABUF_SEL_DEF                             (0x00000000)
  #define MTG0_CR_MTG_DBAC_DATABUF_SEL_HSH                             (0x0204B350)

  #define MTG0_CR_MTG_DBAC_DATABUF_INIT_WPTR_VAL_OFF                   ( 4)
  #define MTG0_CR_MTG_DBAC_DATABUF_INIT_WPTR_VAL_WID                   ( 5)
  #define MTG0_CR_MTG_DBAC_DATABUF_INIT_WPTR_VAL_MSK                   (0x000001F0)
  #define MTG0_CR_MTG_DBAC_DATABUF_INIT_WPTR_VAL_MIN                   (0)
  #define MTG0_CR_MTG_DBAC_DATABUF_INIT_WPTR_VAL_MAX                   (31) // 0x0000001F
  #define MTG0_CR_MTG_DBAC_DATABUF_INIT_WPTR_VAL_DEF                   (0x00000000)
  #define MTG0_CR_MTG_DBAC_DATABUF_INIT_WPTR_VAL_HSH                   (0x0508B350)

  #define MTG0_CR_MTG_DBAC_RSVD9_OFF                                   ( 9)
  #define MTG0_CR_MTG_DBAC_RSVD9_WID                                   ( 1)
  #define MTG0_CR_MTG_DBAC_RSVD9_MSK                                   (0x00000200)
  #define MTG0_CR_MTG_DBAC_RSVD9_MIN                                   (0)
  #define MTG0_CR_MTG_DBAC_RSVD9_MAX                                   (1) // 0x00000001
  #define MTG0_CR_MTG_DBAC_RSVD9_DEF                                   (0x00000000)
  #define MTG0_CR_MTG_DBAC_RSVD9_HSH                                   (0x0112B350)

  #define MTG0_CR_MTG_DBAC_DATABUF_INIT_RPTR_VAL_OFF                   (10)
  #define MTG0_CR_MTG_DBAC_DATABUF_INIT_RPTR_VAL_WID                   ( 5)
  #define MTG0_CR_MTG_DBAC_DATABUF_INIT_RPTR_VAL_MSK                   (0x00007C00)
  #define MTG0_CR_MTG_DBAC_DATABUF_INIT_RPTR_VAL_MIN                   (0)
  #define MTG0_CR_MTG_DBAC_DATABUF_INIT_RPTR_VAL_MAX                   (31) // 0x0000001F
  #define MTG0_CR_MTG_DBAC_DATABUF_INIT_RPTR_VAL_DEF                   (0x00000000)
  #define MTG0_CR_MTG_DBAC_DATABUF_INIT_RPTR_VAL_HSH                   (0x0514B350)

  #define MTG0_CR_MTG_DBAC_RSVD15_OFF                                  (15)
  #define MTG0_CR_MTG_DBAC_RSVD15_WID                                  ( 1)
  #define MTG0_CR_MTG_DBAC_RSVD15_MSK                                  (0x00008000)
  #define MTG0_CR_MTG_DBAC_RSVD15_MIN                                  (0)
  #define MTG0_CR_MTG_DBAC_RSVD15_MAX                                  (1) // 0x00000001
  #define MTG0_CR_MTG_DBAC_RSVD15_DEF                                  (0x00000000)
  #define MTG0_CR_MTG_DBAC_RSVD15_HSH                                  (0x011EB350)

  #define MTG0_CR_MTG_DBAC_DATABUF_CUR_WPTR_VAL_OFF                    (16)
  #define MTG0_CR_MTG_DBAC_DATABUF_CUR_WPTR_VAL_WID                    ( 5)
  #define MTG0_CR_MTG_DBAC_DATABUF_CUR_WPTR_VAL_MSK                    (0x001F0000)
  #define MTG0_CR_MTG_DBAC_DATABUF_CUR_WPTR_VAL_MIN                    (0)
  #define MTG0_CR_MTG_DBAC_DATABUF_CUR_WPTR_VAL_MAX                    (31) // 0x0000001F
  #define MTG0_CR_MTG_DBAC_DATABUF_CUR_WPTR_VAL_DEF                    (0x00000000)
  #define MTG0_CR_MTG_DBAC_DATABUF_CUR_WPTR_VAL_HSH                    (0x05A0B350)

  #define MTG0_CR_MTG_DBAC_RSVD21_OFF                                  (21)
  #define MTG0_CR_MTG_DBAC_RSVD21_WID                                  ( 1)
  #define MTG0_CR_MTG_DBAC_RSVD21_MSK                                  (0x00200000)
  #define MTG0_CR_MTG_DBAC_RSVD21_MIN                                  (0)
  #define MTG0_CR_MTG_DBAC_RSVD21_MAX                                  (1) // 0x00000001
  #define MTG0_CR_MTG_DBAC_RSVD21_DEF                                  (0x00000000)
  #define MTG0_CR_MTG_DBAC_RSVD21_HSH                                  (0x012AB350)

  #define MTG0_CR_MTG_DBAC_DATABUF_CUR_RPTR_VAL_OFF                    (22)
  #define MTG0_CR_MTG_DBAC_DATABUF_CUR_RPTR_VAL_WID                    ( 5)
  #define MTG0_CR_MTG_DBAC_DATABUF_CUR_RPTR_VAL_MSK                    (0x07C00000)
  #define MTG0_CR_MTG_DBAC_DATABUF_CUR_RPTR_VAL_MIN                    (0)
  #define MTG0_CR_MTG_DBAC_DATABUF_CUR_RPTR_VAL_MAX                    (31) // 0x0000001F
  #define MTG0_CR_MTG_DBAC_DATABUF_CUR_RPTR_VAL_DEF                    (0x00000000)
  #define MTG0_CR_MTG_DBAC_DATABUF_CUR_RPTR_VAL_HSH                    (0x05ACB350)

  #define MTG0_CR_MTG_DBAC_RSVD31_27_OFF                               (27)
  #define MTG0_CR_MTG_DBAC_RSVD31_27_WID                               ( 5)
  #define MTG0_CR_MTG_DBAC_RSVD31_27_MSK                               (0xF8000000)
  #define MTG0_CR_MTG_DBAC_RSVD31_27_MIN                               (0)
  #define MTG0_CR_MTG_DBAC_RSVD31_27_MAX                               (31) // 0x0000001F
  #define MTG0_CR_MTG_DBAC_RSVD31_27_DEF                               (0x00000000)
  #define MTG0_CR_MTG_DBAC_RSVD31_27_HSH                               (0x0536B350)

#define MTG0_CR_MTG_DBA_REG                                            (0x0000B354)
#define MTG0_CR_MTG_DBA_DEF                                            (0x00000000)
#define MTG0_CR_MTG_DBA_VOLATILE_BITFIELDS_MSK                         (0xFFFFFFFF)

  #define MTG0_CR_MTG_DBA_DATABUF_ACC_OFF                              ( 0)
  #define MTG0_CR_MTG_DBA_DATABUF_ACC_WID                              (32)
  #define MTG0_CR_MTG_DBA_DATABUF_ACC_MSK                              (0xFFFFFFFF)
  #define MTG0_CR_MTG_DBA_DATABUF_ACC_MIN                              (0)
  #define MTG0_CR_MTG_DBA_DATABUF_ACC_MAX                              (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_DBA_DATABUF_ACC_DEF                              (0x00000000)
  #define MTG0_CR_MTG_DBA_DATABUF_ACC_HSH                              (0x2080B354)

#define MTG0_CR_MTG_LRA0_REG                                           (0x0000B358)
#define MTG0_CR_MTG_LRA0_DEF                                           (0x00000000)
#define MTG0_CR_MTG_LRA0_VOLATILE_BITFIELDS_MSK                        (0x001FF9F3)

  #define MTG0_CR_MTG_LRA0_LR_RANK_ADDR_OFF                            ( 0)
  #define MTG0_CR_MTG_LRA0_LR_RANK_ADDR_WID                            ( 2)
  #define MTG0_CR_MTG_LRA0_LR_RANK_ADDR_MSK                            (0x00000003)
  #define MTG0_CR_MTG_LRA0_LR_RANK_ADDR_MIN                            (0)
  #define MTG0_CR_MTG_LRA0_LR_RANK_ADDR_MAX                            (3) // 0x00000003
  #define MTG0_CR_MTG_LRA0_LR_RANK_ADDR_DEF                            (0x00000000)
  #define MTG0_CR_MTG_LRA0_LR_RANK_ADDR_HSH                            (0x0280B358)

  #define MTG0_CR_MTG_LRA0_RSVD3_2_OFF                                 ( 2)
  #define MTG0_CR_MTG_LRA0_RSVD3_2_WID                                 ( 2)
  #define MTG0_CR_MTG_LRA0_RSVD3_2_MSK                                 (0x0000000C)
  #define MTG0_CR_MTG_LRA0_RSVD3_2_MIN                                 (0)
  #define MTG0_CR_MTG_LRA0_RSVD3_2_MAX                                 (3) // 0x00000003
  #define MTG0_CR_MTG_LRA0_RSVD3_2_DEF                                 (0x00000000)
  #define MTG0_CR_MTG_LRA0_RSVD3_2_HSH                                 (0x0204B358)

  #define MTG0_CR_MTG_LRA0_LR_BANK_ADDR_OFF                            ( 4)
  #define MTG0_CR_MTG_LRA0_LR_BANK_ADDR_WID                            ( 5)
  #define MTG0_CR_MTG_LRA0_LR_BANK_ADDR_MSK                            (0x000001F0)
  #define MTG0_CR_MTG_LRA0_LR_BANK_ADDR_MIN                            (0)
  #define MTG0_CR_MTG_LRA0_LR_BANK_ADDR_MAX                            (31) // 0x0000001F
  #define MTG0_CR_MTG_LRA0_LR_BANK_ADDR_DEF                            (0x00000000)
  #define MTG0_CR_MTG_LRA0_LR_BANK_ADDR_HSH                            (0x0588B358)

  #define MTG0_CR_MTG_LRA0_RSVD10_9_OFF                                ( 9)
  #define MTG0_CR_MTG_LRA0_RSVD10_9_WID                                ( 2)
  #define MTG0_CR_MTG_LRA0_RSVD10_9_MSK                                (0x00000600)
  #define MTG0_CR_MTG_LRA0_RSVD10_9_MIN                                (0)
  #define MTG0_CR_MTG_LRA0_RSVD10_9_MAX                                (3) // 0x00000003
  #define MTG0_CR_MTG_LRA0_RSVD10_9_DEF                                (0x00000000)
  #define MTG0_CR_MTG_LRA0_RSVD10_9_HSH                                (0x0212B358)

  #define MTG0_CR_MTG_LRA0_LR_COL_ADDR_OFF                             (11)
  #define MTG0_CR_MTG_LRA0_LR_COL_ADDR_WID                             (10)
  #define MTG0_CR_MTG_LRA0_LR_COL_ADDR_MSK                             (0x001FF800)
  #define MTG0_CR_MTG_LRA0_LR_COL_ADDR_MIN                             (0)
  #define MTG0_CR_MTG_LRA0_LR_COL_ADDR_MAX                             (1023) // 0x000003FF
  #define MTG0_CR_MTG_LRA0_LR_COL_ADDR_DEF                             (0x00000000)
  #define MTG0_CR_MTG_LRA0_LR_COL_ADDR_HSH                             (0x0A96B358)

  #define MTG0_CR_MTG_LRA0_RSVD31_21_OFF                               (21)
  #define MTG0_CR_MTG_LRA0_RSVD31_21_WID                               (11)
  #define MTG0_CR_MTG_LRA0_RSVD31_21_MSK                               (0xFFE00000)
  #define MTG0_CR_MTG_LRA0_RSVD31_21_MIN                               (0)
  #define MTG0_CR_MTG_LRA0_RSVD31_21_MAX                               (2047) // 0x000007FF
  #define MTG0_CR_MTG_LRA0_RSVD31_21_DEF                               (0x00000000)
  #define MTG0_CR_MTG_LRA0_RSVD31_21_HSH                               (0x0B2AB358)

#define MTG0_CR_MTG_LRA1_REG                                           (0x0000B35C)
#define MTG0_CR_MTG_LRA1_DEF                                           (0x00000000)
#define MTG0_CR_MTG_LRA1_VOLATILE_BITFIELDS_MSK                        (0x0003FFFF)

  #define MTG0_CR_MTG_LRA1_LR_ROW_ADDR_OFF                             ( 0)
  #define MTG0_CR_MTG_LRA1_LR_ROW_ADDR_WID                             (18)
  #define MTG0_CR_MTG_LRA1_LR_ROW_ADDR_MSK                             (0x0003FFFF)
  #define MTG0_CR_MTG_LRA1_LR_ROW_ADDR_MIN                             (0)
  #define MTG0_CR_MTG_LRA1_LR_ROW_ADDR_MAX                             (262143) // 0x0003FFFF
  #define MTG0_CR_MTG_LRA1_LR_ROW_ADDR_DEF                             (0x00000000)
  #define MTG0_CR_MTG_LRA1_LR_ROW_ADDR_HSH                             (0x1280B35C)

  #define MTG0_CR_MTG_LRA1_RSVD31_18_OFF                               (18)
  #define MTG0_CR_MTG_LRA1_RSVD31_18_WID                               (14)
  #define MTG0_CR_MTG_LRA1_RSVD31_18_MSK                               (0xFFFC0000)
  #define MTG0_CR_MTG_LRA1_RSVD31_18_MIN                               (0)
  #define MTG0_CR_MTG_LRA1_RSVD31_18_MAX                               (16383) // 0x00003FFF
  #define MTG0_CR_MTG_LRA1_RSVD31_18_DEF                               (0x00000000)
  #define MTG0_CR_MTG_LRA1_RSVD31_18_HSH                               (0x0E24B35C)

#define MTG0_CR_MTG_GEN_ERRCFG_REG                                     (0x0000B360)
#define MTG0_CR_MTG_GEN_ERRCFG_DEF                                     (0x00000001)
#define MTG0_CR_MTG_GEN_ERRCFG_VOLATILE_BITFIELDS_MSK                  (0xFFF1F000)

  #define MTG0_CR_MTG_GEN_ERRCFG_ERR_CHK_EN_OFF                        ( 0)
  #define MTG0_CR_MTG_GEN_ERRCFG_ERR_CHK_EN_WID                        ( 4)
  #define MTG0_CR_MTG_GEN_ERRCFG_ERR_CHK_EN_MSK                        (0x0000000F)
  #define MTG0_CR_MTG_GEN_ERRCFG_ERR_CHK_EN_MIN                        (0)
  #define MTG0_CR_MTG_GEN_ERRCFG_ERR_CHK_EN_MAX                        (15) // 0x0000000F
  #define MTG0_CR_MTG_GEN_ERRCFG_ERR_CHK_EN_DEF                        (0x00000001)
  #define MTG0_CR_MTG_GEN_ERRCFG_ERR_CHK_EN_HSH                        (0x0400B360)

  #define MTG0_CR_MTG_GEN_ERRCFG_ERR_CAPT_MODE_OFF                     ( 4)
  #define MTG0_CR_MTG_GEN_ERRCFG_ERR_CAPT_MODE_WID                     ( 4)
  #define MTG0_CR_MTG_GEN_ERRCFG_ERR_CAPT_MODE_MSK                     (0x000000F0)
  #define MTG0_CR_MTG_GEN_ERRCFG_ERR_CAPT_MODE_MIN                     (0)
  #define MTG0_CR_MTG_GEN_ERRCFG_ERR_CAPT_MODE_MAX                     (15) // 0x0000000F
  #define MTG0_CR_MTG_GEN_ERRCFG_ERR_CAPT_MODE_DEF                     (0x00000000)
  #define MTG0_CR_MTG_GEN_ERRCFG_ERR_CAPT_MODE_HSH                     (0x0408B360)

  #define MTG0_CR_MTG_GEN_ERRCFG_CHK_DM_BYTE_OFF                       ( 8)
  #define MTG0_CR_MTG_GEN_ERRCFG_CHK_DM_BYTE_WID                       ( 1)
  #define MTG0_CR_MTG_GEN_ERRCFG_CHK_DM_BYTE_MSK                       (0x00000100)
  #define MTG0_CR_MTG_GEN_ERRCFG_CHK_DM_BYTE_MIN                       (0)
  #define MTG0_CR_MTG_GEN_ERRCFG_CHK_DM_BYTE_MAX                       (1) // 0x00000001
  #define MTG0_CR_MTG_GEN_ERRCFG_CHK_DM_BYTE_DEF                       (0x00000000)
  #define MTG0_CR_MTG_GEN_ERRCFG_CHK_DM_BYTE_HSH                       (0x0110B360)

  #define MTG0_CR_MTG_GEN_ERRCFG_DBI_ERR_CHK_EN_OFF                    ( 9)
  #define MTG0_CR_MTG_GEN_ERRCFG_DBI_ERR_CHK_EN_WID                    ( 1)
  #define MTG0_CR_MTG_GEN_ERRCFG_DBI_ERR_CHK_EN_MSK                    (0x00000200)
  #define MTG0_CR_MTG_GEN_ERRCFG_DBI_ERR_CHK_EN_MIN                    (0)
  #define MTG0_CR_MTG_GEN_ERRCFG_DBI_ERR_CHK_EN_MAX                    (1) // 0x00000001
  #define MTG0_CR_MTG_GEN_ERRCFG_DBI_ERR_CHK_EN_DEF                    (0x00000000)
  #define MTG0_CR_MTG_GEN_ERRCFG_DBI_ERR_CHK_EN_HSH                    (0x0112B360)

  #define MTG0_CR_MTG_GEN_ERRCFG_RSVD11_10_OFF                         (10)
  #define MTG0_CR_MTG_GEN_ERRCFG_RSVD11_10_WID                         ( 2)
  #define MTG0_CR_MTG_GEN_ERRCFG_RSVD11_10_MSK                         (0x00000C00)
  #define MTG0_CR_MTG_GEN_ERRCFG_RSVD11_10_MIN                         (0)
  #define MTG0_CR_MTG_GEN_ERRCFG_RSVD11_10_MAX                         (3) // 0x00000003
  #define MTG0_CR_MTG_GEN_ERRCFG_RSVD11_10_DEF                         (0x00000000)
  #define MTG0_CR_MTG_GEN_ERRCFG_RSVD11_10_HSH                         (0x0214B360)

  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_ALL_ERRS_OFF                      (12)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_ALL_ERRS_WID                      ( 4)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_ALL_ERRS_MSK                      (0x0000F000)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_ALL_ERRS_MIN                      (0)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_ALL_ERRS_MAX                      (15) // 0x0000000F
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_ALL_ERRS_DEF                      (0x00000000)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_ALL_ERRS_HSH                      (0x0498B360)

  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_WR_DATABUF_OFF                    (16)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_WR_DATABUF_WID                    ( 1)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_WR_DATABUF_MSK                    (0x00010000)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_WR_DATABUF_MIN                    (0)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_WR_DATABUF_MAX                    (1) // 0x00000001
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_WR_DATABUF_DEF                    (0x00000000)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_WR_DATABUF_HSH                    (0x01A0B360)

  #define MTG0_CR_MTG_GEN_ERRCFG_RSVD19_17_OFF                         (17)
  #define MTG0_CR_MTG_GEN_ERRCFG_RSVD19_17_WID                         ( 3)
  #define MTG0_CR_MTG_GEN_ERRCFG_RSVD19_17_MSK                         (0x000E0000)
  #define MTG0_CR_MTG_GEN_ERRCFG_RSVD19_17_MIN                         (0)
  #define MTG0_CR_MTG_GEN_ERRCFG_RSVD19_17_MAX                         (7) // 0x00000007
  #define MTG0_CR_MTG_GEN_ERRCFG_RSVD19_17_DEF                         (0x00000000)
  #define MTG0_CR_MTG_GEN_ERRCFG_RSVD19_17_HSH                         (0x0322B360)

  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_EXP_DATABUF_OFF                   (20)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_EXP_DATABUF_WID                   ( 4)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_EXP_DATABUF_MSK                   (0x00F00000)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_EXP_DATABUF_MIN                   (0)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_EXP_DATABUF_MAX                   (15) // 0x0000000F
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_EXP_DATABUF_DEF                   (0x00000000)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_EXP_DATABUF_HSH                   (0x04A8B360)

  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_LR_DATABUF_OFF                    (24)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_LR_DATABUF_WID                    ( 4)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_LR_DATABUF_MSK                    (0x0F000000)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_LR_DATABUF_MIN                    (0)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_LR_DATABUF_MAX                    (15) // 0x0000000F
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_LR_DATABUF_DEF                    (0x00000000)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_LR_DATABUF_HSH                    (0x04B0B360)

  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_LR_ADDRBUF_OFF                    (28)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_LR_ADDRBUF_WID                    ( 4)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_LR_ADDRBUF_MSK                    (0xF0000000)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_LR_ADDRBUF_MIN                    (0)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_LR_ADDRBUF_MAX                    (15) // 0x0000000F
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_LR_ADDRBUF_DEF                    (0x00000000)
  #define MTG0_CR_MTG_GEN_ERRCFG_CLR_LR_ADDRBUF_HSH                    (0x04B8B360)

#define MTG0_CR_MTG_GEN_ERR_REG                                        (0x0000B368)
#define MTG0_CR_MTG_GEN_ERR_DEF                                        (0x00000000)
#define MTG0_CR_MTG_GEN_ERR_VOLATILE_BITFIELDS_MSK                     (0xFFFFFFFF)

  #define MTG0_CR_MTG_GEN_ERR_GENERAL_ERR_OFF                          ( 0)
  #define MTG0_CR_MTG_GEN_ERR_GENERAL_ERR_WID                          ( 4)
  #define MTG0_CR_MTG_GEN_ERR_GENERAL_ERR_MSK                          (0x0000000F)
  #define MTG0_CR_MTG_GEN_ERR_GENERAL_ERR_MIN                          (0)
  #define MTG0_CR_MTG_GEN_ERR_GENERAL_ERR_MAX                          (15) // 0x0000000F
  #define MTG0_CR_MTG_GEN_ERR_GENERAL_ERR_DEF                          (0x00000000)
  #define MTG0_CR_MTG_GEN_ERR_GENERAL_ERR_HSH                          (0x0480B368)

  #define MTG0_CR_MTG_GEN_ERR_ALL_DQ_ERR_OFF                           ( 4)
  #define MTG0_CR_MTG_GEN_ERR_ALL_DQ_ERR_WID                           ( 4)
  #define MTG0_CR_MTG_GEN_ERR_ALL_DQ_ERR_MSK                           (0x000000F0)
  #define MTG0_CR_MTG_GEN_ERR_ALL_DQ_ERR_MIN                           (0)
  #define MTG0_CR_MTG_GEN_ERR_ALL_DQ_ERR_MAX                           (15) // 0x0000000F
  #define MTG0_CR_MTG_GEN_ERR_ALL_DQ_ERR_DEF                           (0x00000000)
  #define MTG0_CR_MTG_GEN_ERR_ALL_DQ_ERR_HSH                           (0x0488B368)

  #define MTG0_CR_MTG_GEN_ERR_STOPPED_ON_ERR_OFF                       ( 8)
  #define MTG0_CR_MTG_GEN_ERR_STOPPED_ON_ERR_WID                       ( 4)
  #define MTG0_CR_MTG_GEN_ERR_STOPPED_ON_ERR_MSK                       (0x00000F00)
  #define MTG0_CR_MTG_GEN_ERR_STOPPED_ON_ERR_MIN                       (0)
  #define MTG0_CR_MTG_GEN_ERR_STOPPED_ON_ERR_MAX                       (15) // 0x0000000F
  #define MTG0_CR_MTG_GEN_ERR_STOPPED_ON_ERR_DEF                       (0x00000000)
  #define MTG0_CR_MTG_GEN_ERR_STOPPED_ON_ERR_HSH                       (0x0490B368)

  #define MTG0_CR_MTG_GEN_ERR_PAUSED_ON_ERR_OFF                        (12)
  #define MTG0_CR_MTG_GEN_ERR_PAUSED_ON_ERR_WID                        ( 4)
  #define MTG0_CR_MTG_GEN_ERR_PAUSED_ON_ERR_MSK                        (0x0000F000)
  #define MTG0_CR_MTG_GEN_ERR_PAUSED_ON_ERR_MIN                        (0)
  #define MTG0_CR_MTG_GEN_ERR_PAUSED_ON_ERR_MAX                        (15) // 0x0000000F
  #define MTG0_CR_MTG_GEN_ERR_PAUSED_ON_ERR_DEF                        (0x00000000)
  #define MTG0_CR_MTG_GEN_ERR_PAUSED_ON_ERR_HSH                        (0x0498B368)

  #define MTG0_CR_MTG_GEN_ERR_CH0_RANK_ERR_OFF                         (16)
  #define MTG0_CR_MTG_GEN_ERR_CH0_RANK_ERR_WID                         ( 4)
  #define MTG0_CR_MTG_GEN_ERR_CH0_RANK_ERR_MSK                         (0x000F0000)
  #define MTG0_CR_MTG_GEN_ERR_CH0_RANK_ERR_MIN                         (0)
  #define MTG0_CR_MTG_GEN_ERR_CH0_RANK_ERR_MAX                         (15) // 0x0000000F
  #define MTG0_CR_MTG_GEN_ERR_CH0_RANK_ERR_DEF                         (0x00000000)
  #define MTG0_CR_MTG_GEN_ERR_CH0_RANK_ERR_HSH                         (0x04A0B368)

  #define MTG0_CR_MTG_GEN_ERR_CH1_RANK_ERR_OFF                         (20)
  #define MTG0_CR_MTG_GEN_ERR_CH1_RANK_ERR_WID                         ( 4)
  #define MTG0_CR_MTG_GEN_ERR_CH1_RANK_ERR_MSK                         (0x00F00000)
  #define MTG0_CR_MTG_GEN_ERR_CH1_RANK_ERR_MIN                         (0)
  #define MTG0_CR_MTG_GEN_ERR_CH1_RANK_ERR_MAX                         (15) // 0x0000000F
  #define MTG0_CR_MTG_GEN_ERR_CH1_RANK_ERR_DEF                         (0x00000000)
  #define MTG0_CR_MTG_GEN_ERR_CH1_RANK_ERR_HSH                         (0x04A8B368)

  #define MTG0_CR_MTG_GEN_ERR_CH2_RANK_ERR_OFF                         (24)
  #define MTG0_CR_MTG_GEN_ERR_CH2_RANK_ERR_WID                         ( 4)
  #define MTG0_CR_MTG_GEN_ERR_CH2_RANK_ERR_MSK                         (0x0F000000)
  #define MTG0_CR_MTG_GEN_ERR_CH2_RANK_ERR_MIN                         (0)
  #define MTG0_CR_MTG_GEN_ERR_CH2_RANK_ERR_MAX                         (15) // 0x0000000F
  #define MTG0_CR_MTG_GEN_ERR_CH2_RANK_ERR_DEF                         (0x00000000)
  #define MTG0_CR_MTG_GEN_ERR_CH2_RANK_ERR_HSH                         (0x04B0B368)

  #define MTG0_CR_MTG_GEN_ERR_CH3_RANK_ERR_OFF                         (28)
  #define MTG0_CR_MTG_GEN_ERR_CH3_RANK_ERR_WID                         ( 4)
  #define MTG0_CR_MTG_GEN_ERR_CH3_RANK_ERR_MSK                         (0xF0000000)
  #define MTG0_CR_MTG_GEN_ERR_CH3_RANK_ERR_MIN                         (0)
  #define MTG0_CR_MTG_GEN_ERR_CH3_RANK_ERR_MAX                         (15) // 0x0000000F
  #define MTG0_CR_MTG_GEN_ERR_CH3_RANK_ERR_DEF                         (0x00000000)
  #define MTG0_CR_MTG_GEN_ERR_CH3_RANK_ERR_HSH                         (0x04B8B368)

#define MTG0_CR_MTG_BYTE_LANE_ERR_REG                                  (0x0000B36C)
#define MTG0_CR_MTG_BYTE_LANE_ERR_DEF                                  (0x00000000)
#define MTG0_CR_MTG_BYTE_LANE_ERR_VOLATILE_BITFIELDS_MSK               (0x1F1F1F1F)

  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH0_BYTE_LANE_ERR_OFF              ( 0)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH0_BYTE_LANE_ERR_WID              ( 5)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH0_BYTE_LANE_ERR_MSK              (0x0000001F)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH0_BYTE_LANE_ERR_MIN              (0)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH0_BYTE_LANE_ERR_MAX              (31) // 0x0000001F
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH0_BYTE_LANE_ERR_DEF              (0x00000000)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH0_BYTE_LANE_ERR_HSH              (0x0580B36C)

  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD7_5_OFF                        ( 5)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD7_5_WID                        ( 3)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD7_5_MSK                        (0x000000E0)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD7_5_MIN                        (0)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD7_5_MAX                        (7) // 0x00000007
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD7_5_DEF                        (0x00000000)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD7_5_HSH                        (0x030AB36C)

  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH1_BYTE_LANE_ERR_OFF              ( 8)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH1_BYTE_LANE_ERR_WID              ( 5)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH1_BYTE_LANE_ERR_MSK              (0x00001F00)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH1_BYTE_LANE_ERR_MIN              (0)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH1_BYTE_LANE_ERR_MAX              (31) // 0x0000001F
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH1_BYTE_LANE_ERR_DEF              (0x00000000)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH1_BYTE_LANE_ERR_HSH              (0x0590B36C)

  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD15_13_OFF                      (13)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD15_13_WID                      ( 3)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD15_13_MSK                      (0x0000E000)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD15_13_MIN                      (0)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD15_13_MAX                      (7) // 0x00000007
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD15_13_DEF                      (0x00000000)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD15_13_HSH                      (0x031AB36C)

  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH2_BYTE_LANE_ERR_OFF              (16)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH2_BYTE_LANE_ERR_WID              ( 5)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH2_BYTE_LANE_ERR_MSK              (0x001F0000)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH2_BYTE_LANE_ERR_MIN              (0)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH2_BYTE_LANE_ERR_MAX              (31) // 0x0000001F
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH2_BYTE_LANE_ERR_DEF              (0x00000000)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH2_BYTE_LANE_ERR_HSH              (0x05A0B36C)

  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD23_21_OFF                      (21)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD23_21_WID                      ( 3)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD23_21_MSK                      (0x00E00000)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD23_21_MIN                      (0)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD23_21_MAX                      (7) // 0x00000007
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD23_21_DEF                      (0x00000000)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD23_21_HSH                      (0x032AB36C)

  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH3_BYTE_LANE_ERR_OFF              (24)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH3_BYTE_LANE_ERR_WID              ( 5)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH3_BYTE_LANE_ERR_MSK              (0x1F000000)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH3_BYTE_LANE_ERR_MIN              (0)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH3_BYTE_LANE_ERR_MAX              (31) // 0x0000001F
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH3_BYTE_LANE_ERR_DEF              (0x00000000)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_CH3_BYTE_LANE_ERR_HSH              (0x05B0B36C)

  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD31_29_OFF                      (29)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD31_29_WID                      ( 3)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD31_29_MSK                      (0xE0000000)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD31_29_MIN                      (0)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD31_29_MAX                      (7) // 0x00000007
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD31_29_DEF                      (0x00000000)
  #define MTG0_CR_MTG_BYTE_LANE_ERR_RSVD31_29_HSH                      (0x033AB36C)

#define MTG0_CR_MTG_MRD_ERR_REG                                        (0x0000B370)
#define MTG0_CR_MTG_MRD_ERR_DEF                                        (0x00000064)
#define MTG0_CR_MTG_MRD_ERR_VOLATILE_BITFIELDS_MSK                     (0xF0000000)

  #define MTG0_CR_MTG_MRD_ERR_MRD_TMO_DLY_OFF                          ( 0)
  #define MTG0_CR_MTG_MRD_ERR_MRD_TMO_DLY_WID                          (24)
  #define MTG0_CR_MTG_MRD_ERR_MRD_TMO_DLY_MSK                          (0x00FFFFFF)
  #define MTG0_CR_MTG_MRD_ERR_MRD_TMO_DLY_MIN                          (0)
  #define MTG0_CR_MTG_MRD_ERR_MRD_TMO_DLY_MAX                          (16777215) // 0x00FFFFFF
  #define MTG0_CR_MTG_MRD_ERR_MRD_TMO_DLY_DEF                          (0x00000064)
  #define MTG0_CR_MTG_MRD_ERR_MRD_TMO_DLY_HSH                          (0x1800B370)

  #define MTG0_CR_MTG_MRD_ERR_RSVD27_24_OFF                            (24)
  #define MTG0_CR_MTG_MRD_ERR_RSVD27_24_WID                            ( 4)
  #define MTG0_CR_MTG_MRD_ERR_RSVD27_24_MSK                            (0x0F000000)
  #define MTG0_CR_MTG_MRD_ERR_RSVD27_24_MIN                            (0)
  #define MTG0_CR_MTG_MRD_ERR_RSVD27_24_MAX                            (15) // 0x0000000F
  #define MTG0_CR_MTG_MRD_ERR_RSVD27_24_DEF                            (0x00000000)
  #define MTG0_CR_MTG_MRD_ERR_RSVD27_24_HSH                            (0x0430B370)

  #define MTG0_CR_MTG_MRD_ERR_MRD_ERR_OFF                              (28)
  #define MTG0_CR_MTG_MRD_ERR_MRD_ERR_WID                              ( 4)
  #define MTG0_CR_MTG_MRD_ERR_MRD_ERR_MSK                              (0xF0000000)
  #define MTG0_CR_MTG_MRD_ERR_MRD_ERR_MIN                              (0)
  #define MTG0_CR_MTG_MRD_ERR_MRD_ERR_MAX                              (15) // 0x0000000F
  #define MTG0_CR_MTG_MRD_ERR_MRD_ERR_DEF                              (0x00000000)
  #define MTG0_CR_MTG_MRD_ERR_MRD_ERR_HSH                              (0x04B8B370)

#define MTG0_CR_MTG_CH0_ERRCFG_REG                                     (0x0000B380)
#define MTG0_CR_MTG_CH0_ERRCFG_DEF                                     (0x00000000)
#define MTG0_CR_MTG_CH0_ERRCFG_VOLATILE_BITFIELDS_MSK                  (0x00000000)

  #define MTG0_CR_MTG_CH0_ERRCFG_STOP_ON_ALL_DQ_ERR_OFF                ( 0)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOP_ON_ALL_DQ_ERR_WID                ( 1)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOP_ON_ALL_DQ_ERR_MSK                (0x00000001)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOP_ON_ALL_DQ_ERR_MIN                (0)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOP_ON_ALL_DQ_ERR_MAX                (1) // 0x00000001
  #define MTG0_CR_MTG_CH0_ERRCFG_STOP_ON_ALL_DQ_ERR_DEF                (0x00000000)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOP_ON_ALL_DQ_ERR_HSH                (0x0100B380)

  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ON_ERR_EN_OFF                ( 1)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ON_ERR_EN_WID                ( 1)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ON_ERR_EN_MSK                (0x00000002)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ON_ERR_EN_MIN                (0)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ON_ERR_EN_MAX                (1) // 0x00000001
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ON_ERR_EN_DEF                (0x00000000)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ON_ERR_EN_HSH                (0x0102B380)

  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ON_ERR_OFF                   ( 2)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ON_ERR_WID                   ( 1)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ON_ERR_MSK                   (0x00000004)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ON_ERR_MIN                   (0)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ON_ERR_MAX                   (1) // 0x00000001
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ON_ERR_DEF                   (0x00000000)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ON_ERR_HSH                   (0x0104B380)

  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ERR_CNTR_SEL_OFF             ( 3)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ERR_CNTR_SEL_WID             ( 1)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ERR_CNTR_SEL_MSK             (0x00000008)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ERR_CNTR_SEL_MIN             (0)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ERR_CNTR_SEL_MAX             (1) // 0x00000001
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ERR_CNTR_SEL_DEF             (0x00000000)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ERR_CNTR_SEL_HSH             (0x0106B380)

  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ON_ALL_BEC_OFF               ( 4)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ON_ALL_BEC_WID               ( 2)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ON_ALL_BEC_MSK               (0x00000030)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ON_ALL_BEC_MIN               (0)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ON_ALL_BEC_MAX               (3) // 0x00000003
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ON_ALL_BEC_DEF               (0x00000000)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_ON_ALL_BEC_HSH               (0x0208B380)

  #define MTG0_CR_MTG_CH0_ERRCFG_RSVD7_6_OFF                           ( 6)
  #define MTG0_CR_MTG_CH0_ERRCFG_RSVD7_6_WID                           ( 2)
  #define MTG0_CR_MTG_CH0_ERRCFG_RSVD7_6_MSK                           (0x000000C0)
  #define MTG0_CR_MTG_CH0_ERRCFG_RSVD7_6_MIN                           (0)
  #define MTG0_CR_MTG_CH0_ERRCFG_RSVD7_6_MAX                           (3) // 0x00000003
  #define MTG0_CR_MTG_CH0_ERRCFG_RSVD7_6_DEF                           (0x00000000)
  #define MTG0_CR_MTG_CH0_ERRCFG_RSVD7_6_HSH                           (0x020CB380)

  #define MTG0_CR_MTG_CH0_ERRCFG_IGNORE_N_READS_OFF                    ( 8)
  #define MTG0_CR_MTG_CH0_ERRCFG_IGNORE_N_READS_WID                    ( 8)
  #define MTG0_CR_MTG_CH0_ERRCFG_IGNORE_N_READS_MSK                    (0x0000FF00)
  #define MTG0_CR_MTG_CH0_ERRCFG_IGNORE_N_READS_MIN                    (0)
  #define MTG0_CR_MTG_CH0_ERRCFG_IGNORE_N_READS_MAX                    (255) // 0x000000FF
  #define MTG0_CR_MTG_CH0_ERRCFG_IGNORE_N_READS_DEF                    (0x00000000)
  #define MTG0_CR_MTG_CH0_ERRCFG_IGNORE_N_READS_HSH                    (0x0810B380)

  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_THRES_OFF                    (16)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_THRES_WID                    (16)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_THRES_MSK                    (0xFFFF0000)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_THRES_MIN                    (0)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_THRES_MAX                    (65535) // 0x0000FFFF
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_THRES_DEF                    (0x00000000)
  #define MTG0_CR_MTG_CH0_ERRCFG_STOPAUSE_THRES_HSH                    (0x1020B380)

#define MTG0_CR_MTG_CH0_ERRCNT0_REG                                    (0x0000B384)
#define MTG0_CR_MTG_CH0_ERRCNT0_DEF                                    (0x80000000)
#define MTG0_CR_MTG_CH0_ERRCNT0_VOLATILE_BITFIELDS_MSK                 (0x0000FFFF)

  #define MTG0_CR_MTG_CH0_ERRCNT0_ERR_CNT_OFF                          ( 0)
  #define MTG0_CR_MTG_CH0_ERRCNT0_ERR_CNT_WID                          (16)
  #define MTG0_CR_MTG_CH0_ERRCNT0_ERR_CNT_MSK                          (0x0000FFFF)
  #define MTG0_CR_MTG_CH0_ERRCNT0_ERR_CNT_MIN                          (0)
  #define MTG0_CR_MTG_CH0_ERRCNT0_ERR_CNT_MAX                          (65535) // 0x0000FFFF
  #define MTG0_CR_MTG_CH0_ERRCNT0_ERR_CNT_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_ERRCNT0_ERR_CNT_HSH                          (0x1080B384)

  #define MTG0_CR_MTG_CH0_ERRCNT0_RSVD27_16_OFF                        (16)
  #define MTG0_CR_MTG_CH0_ERRCNT0_RSVD27_16_WID                        (12)
  #define MTG0_CR_MTG_CH0_ERRCNT0_RSVD27_16_MSK                        (0x0FFF0000)
  #define MTG0_CR_MTG_CH0_ERRCNT0_RSVD27_16_MIN                        (0)
  #define MTG0_CR_MTG_CH0_ERRCNT0_RSVD27_16_MAX                        (4095) // 0x00000FFF
  #define MTG0_CR_MTG_CH0_ERRCNT0_RSVD27_16_DEF                        (0x00000000)
  #define MTG0_CR_MTG_CH0_ERRCNT0_RSVD27_16_HSH                        (0x0C20B384)

  #define MTG0_CR_MTG_CH0_ERRCNT0_ERR_CNT_TYPE_OFF                     (28)
  #define MTG0_CR_MTG_CH0_ERRCNT0_ERR_CNT_TYPE_WID                     ( 3)
  #define MTG0_CR_MTG_CH0_ERRCNT0_ERR_CNT_TYPE_MSK                     (0x70000000)
  #define MTG0_CR_MTG_CH0_ERRCNT0_ERR_CNT_TYPE_MIN                     (0)
  #define MTG0_CR_MTG_CH0_ERRCNT0_ERR_CNT_TYPE_MAX                     (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_ERRCNT0_ERR_CNT_TYPE_DEF                     (0x00000000)
  #define MTG0_CR_MTG_CH0_ERRCNT0_ERR_CNT_TYPE_HSH                     (0x0338B384)

  #define MTG0_CR_MTG_CH0_ERRCNT0_ERR_CNT_EN_OFF                       (31)
  #define MTG0_CR_MTG_CH0_ERRCNT0_ERR_CNT_EN_WID                       ( 1)
  #define MTG0_CR_MTG_CH0_ERRCNT0_ERR_CNT_EN_MSK                       (0x80000000)
  #define MTG0_CR_MTG_CH0_ERRCNT0_ERR_CNT_EN_MIN                       (0)
  #define MTG0_CR_MTG_CH0_ERRCNT0_ERR_CNT_EN_MAX                       (1) // 0x00000001
  #define MTG0_CR_MTG_CH0_ERRCNT0_ERR_CNT_EN_DEF                       (0x00000001)
  #define MTG0_CR_MTG_CH0_ERRCNT0_ERR_CNT_EN_HSH                       (0x013EB384)

#define MTG0_CR_MTG_CH0_ERRCNT1_REG                                    (0x0000B388)
#define MTG0_CR_MTG_CH0_ERRCNT1_DEF                                    (0x00000000)
#define MTG0_CR_MTG_CH0_ERRCNT1_VOLATILE_BITFIELDS_MSK                 (0x0000FFFF)

  #define MTG0_CR_MTG_CH0_ERRCNT1_ERR_CNT_OFF                          ( 0)
  #define MTG0_CR_MTG_CH0_ERRCNT1_ERR_CNT_WID                          (16)
  #define MTG0_CR_MTG_CH0_ERRCNT1_ERR_CNT_MSK                          (0x0000FFFF)
  #define MTG0_CR_MTG_CH0_ERRCNT1_ERR_CNT_MIN                          (0)
  #define MTG0_CR_MTG_CH0_ERRCNT1_ERR_CNT_MAX                          (65535) // 0x0000FFFF
  #define MTG0_CR_MTG_CH0_ERRCNT1_ERR_CNT_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_ERRCNT1_ERR_CNT_HSH                          (0x1080B388)

  #define MTG0_CR_MTG_CH0_ERRCNT1_RSVD27_16_OFF                        (16)
  #define MTG0_CR_MTG_CH0_ERRCNT1_RSVD27_16_WID                        (12)
  #define MTG0_CR_MTG_CH0_ERRCNT1_RSVD27_16_MSK                        (0x0FFF0000)
  #define MTG0_CR_MTG_CH0_ERRCNT1_RSVD27_16_MIN                        (0)
  #define MTG0_CR_MTG_CH0_ERRCNT1_RSVD27_16_MAX                        (4095) // 0x00000FFF
  #define MTG0_CR_MTG_CH0_ERRCNT1_RSVD27_16_DEF                        (0x00000000)
  #define MTG0_CR_MTG_CH0_ERRCNT1_RSVD27_16_HSH                        (0x0C20B388)

  #define MTG0_CR_MTG_CH0_ERRCNT1_ERR_CNT_TYPE_OFF                     (28)
  #define MTG0_CR_MTG_CH0_ERRCNT1_ERR_CNT_TYPE_WID                     ( 3)
  #define MTG0_CR_MTG_CH0_ERRCNT1_ERR_CNT_TYPE_MSK                     (0x70000000)
  #define MTG0_CR_MTG_CH0_ERRCNT1_ERR_CNT_TYPE_MIN                     (0)
  #define MTG0_CR_MTG_CH0_ERRCNT1_ERR_CNT_TYPE_MAX                     (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_ERRCNT1_ERR_CNT_TYPE_DEF                     (0x00000000)
  #define MTG0_CR_MTG_CH0_ERRCNT1_ERR_CNT_TYPE_HSH                     (0x0338B388)

  #define MTG0_CR_MTG_CH0_ERRCNT1_ERR_CNT_EN_OFF                       (31)
  #define MTG0_CR_MTG_CH0_ERRCNT1_ERR_CNT_EN_WID                       ( 1)
  #define MTG0_CR_MTG_CH0_ERRCNT1_ERR_CNT_EN_MSK                       (0x80000000)
  #define MTG0_CR_MTG_CH0_ERRCNT1_ERR_CNT_EN_MIN                       (0)
  #define MTG0_CR_MTG_CH0_ERRCNT1_ERR_CNT_EN_MAX                       (1) // 0x00000001
  #define MTG0_CR_MTG_CH0_ERRCNT1_ERR_CNT_EN_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_ERRCNT1_ERR_CNT_EN_HSH                       (0x013EB388)

#define MTG0_CR_MTG_CH0_BEM0_REG                                       (0x0000B390)
#define MTG0_CR_MTG_CH0_BEM0_DEF                                       (0x00000000)
#define MTG0_CR_MTG_CH0_BEM0_VOLATILE_BITFIELDS_MSK                    (0x00000000)

  #define MTG0_CR_MTG_CH0_BEM0_DATA_BYTE_ERR_MASK_0_OFF                ( 0)
  #define MTG0_CR_MTG_CH0_BEM0_DATA_BYTE_ERR_MASK_0_WID                (32)
  #define MTG0_CR_MTG_CH0_BEM0_DATA_BYTE_ERR_MASK_0_MSK                (0xFFFFFFFF)
  #define MTG0_CR_MTG_CH0_BEM0_DATA_BYTE_ERR_MASK_0_MIN                (0)
  #define MTG0_CR_MTG_CH0_BEM0_DATA_BYTE_ERR_MASK_0_MAX                (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_CH0_BEM0_DATA_BYTE_ERR_MASK_0_DEF                (0x00000000)
  #define MTG0_CR_MTG_CH0_BEM0_DATA_BYTE_ERR_MASK_0_HSH                (0x2000B390)

#define MTG0_CR_MTG_CH0_BEM1_REG                                       (0x0000B394)
#define MTG0_CR_MTG_CH0_BEM1_DEF                                       (0x00000000)
#define MTG0_CR_MTG_CH0_BEM1_VOLATILE_BITFIELDS_MSK                    (0x00000000)

  #define MTG0_CR_MTG_CH0_BEM1_DATA_BYTE_ERR_MASK_1_OFF                ( 0)
  #define MTG0_CR_MTG_CH0_BEM1_DATA_BYTE_ERR_MASK_1_WID                (32)
  #define MTG0_CR_MTG_CH0_BEM1_DATA_BYTE_ERR_MASK_1_MSK                (0xFFFFFFFF)
  #define MTG0_CR_MTG_CH0_BEM1_DATA_BYTE_ERR_MASK_1_MIN                (0)
  #define MTG0_CR_MTG_CH0_BEM1_DATA_BYTE_ERR_MASK_1_MAX                (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_CH0_BEM1_DATA_BYTE_ERR_MASK_1_DEF                (0x00000000)
  #define MTG0_CR_MTG_CH0_BEM1_DATA_BYTE_ERR_MASK_1_HSH                (0x2000B394)

#define MTG0_CR_MTG_CH0_BEM2_REG                                       (0x0000B398)
#define MTG0_CR_MTG_CH0_BEM2_DEF                                       (0x00000000)
#define MTG0_CR_MTG_CH0_BEM2_VOLATILE_BITFIELDS_MSK                    (0x00000000)

  #define MTG0_CR_MTG_CH0_BEM2_ECC_BYTE_ERR_MASK_OFF                   ( 0)
  #define MTG0_CR_MTG_CH0_BEM2_ECC_BYTE_ERR_MASK_WID                   (16)
  #define MTG0_CR_MTG_CH0_BEM2_ECC_BYTE_ERR_MASK_MSK                   (0x0000FFFF)
  #define MTG0_CR_MTG_CH0_BEM2_ECC_BYTE_ERR_MASK_MIN                   (0)
  #define MTG0_CR_MTG_CH0_BEM2_ECC_BYTE_ERR_MASK_MAX                   (65535) // 0x0000FFFF
  #define MTG0_CR_MTG_CH0_BEM2_ECC_BYTE_ERR_MASK_DEF                   (0x00000000)
  #define MTG0_CR_MTG_CH0_BEM2_ECC_BYTE_ERR_MASK_HSH                   (0x1000B398)

  #define MTG0_CR_MTG_CH0_BEM2_RSVD31_16_OFF                           (16)
  #define MTG0_CR_MTG_CH0_BEM2_RSVD31_16_WID                           (16)
  #define MTG0_CR_MTG_CH0_BEM2_RSVD31_16_MSK                           (0xFFFF0000)
  #define MTG0_CR_MTG_CH0_BEM2_RSVD31_16_MIN                           (0)
  #define MTG0_CR_MTG_CH0_BEM2_RSVD31_16_MAX                           (65535) // 0x0000FFFF
  #define MTG0_CR_MTG_CH0_BEM2_RSVD31_16_DEF                           (0x00000000)
  #define MTG0_CR_MTG_CH0_BEM2_RSVD31_16_HSH                           (0x1020B398)

#define MTG0_CR_MTG_CH0_UIEM_REG                                       (0x0000B39C)
#define MTG0_CR_MTG_CH0_UIEM_DEF                                       (0x00000000)
#define MTG0_CR_MTG_CH0_UIEM_VOLATILE_BITFIELDS_MSK                    (0x00000000)

  #define MTG0_CR_MTG_CH0_UIEM_UI_ERR_MASK_OFF                         ( 0)
  #define MTG0_CR_MTG_CH0_UIEM_UI_ERR_MASK_WID                         (32)
  #define MTG0_CR_MTG_CH0_UIEM_UI_ERR_MASK_MSK                         (0xFFFFFFFF)
  #define MTG0_CR_MTG_CH0_UIEM_UI_ERR_MASK_MIN                         (0)
  #define MTG0_CR_MTG_CH0_UIEM_UI_ERR_MASK_MAX                         (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_CH0_UIEM_UI_ERR_MASK_DEF                         (0x00000000)
  #define MTG0_CR_MTG_CH0_UIEM_UI_ERR_MASK_HSH                         (0x2000B39C)

#define MTG0_CR_MTG_CH0_LEM0_REG                                       (0x0000B3A0)
#define MTG0_CR_MTG_CH0_LEM0_DEF                                       (0x00000000)
#define MTG0_CR_MTG_CH0_LEM0_VOLATILE_BITFIELDS_MSK                    (0x00000000)

  #define MTG0_CR_MTG_CH0_LEM0_DQ_LANE_ERR_MASK_0_OFF                  ( 0)
  #define MTG0_CR_MTG_CH0_LEM0_DQ_LANE_ERR_MASK_0_WID                  (32)
  #define MTG0_CR_MTG_CH0_LEM0_DQ_LANE_ERR_MASK_0_MSK                  (0xFFFFFFFF)
  #define MTG0_CR_MTG_CH0_LEM0_DQ_LANE_ERR_MASK_0_MIN                  (0)
  #define MTG0_CR_MTG_CH0_LEM0_DQ_LANE_ERR_MASK_0_MAX                  (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_CH0_LEM0_DQ_LANE_ERR_MASK_0_DEF                  (0x00000000)
  #define MTG0_CR_MTG_CH0_LEM0_DQ_LANE_ERR_MASK_0_HSH                  (0x2000B3A0)

#define MTG0_CR_MTG_CH0_LEM1_REG                                       (0x0000B3A4)
#define MTG0_CR_MTG_CH0_LEM1_DEF                                       (0x00000000)
#define MTG0_CR_MTG_CH0_LEM1_VOLATILE_BITFIELDS_MSK                    (0x00000000)

  #define MTG0_CR_MTG_CH0_LEM1_DQ_LANE_ERR_MASK_1_OFF                  ( 0)
  #define MTG0_CR_MTG_CH0_LEM1_DQ_LANE_ERR_MASK_1_WID                  ( 8)
  #define MTG0_CR_MTG_CH0_LEM1_DQ_LANE_ERR_MASK_1_MSK                  (0x000000FF)
  #define MTG0_CR_MTG_CH0_LEM1_DQ_LANE_ERR_MASK_1_MIN                  (0)
  #define MTG0_CR_MTG_CH0_LEM1_DQ_LANE_ERR_MASK_1_MAX                  (255) // 0x000000FF
  #define MTG0_CR_MTG_CH0_LEM1_DQ_LANE_ERR_MASK_1_DEF                  (0x00000000)
  #define MTG0_CR_MTG_CH0_LEM1_DQ_LANE_ERR_MASK_1_HSH                  (0x0800B3A4)

  #define MTG0_CR_MTG_CH0_LEM1_RSVD31_8_OFF                            ( 8)
  #define MTG0_CR_MTG_CH0_LEM1_RSVD31_8_WID                            (24)
  #define MTG0_CR_MTG_CH0_LEM1_RSVD31_8_MSK                            (0xFFFFFF00)
  #define MTG0_CR_MTG_CH0_LEM1_RSVD31_8_MIN                            (0)
  #define MTG0_CR_MTG_CH0_LEM1_RSVD31_8_MAX                            (16777215) // 0x00FFFFFF
  #define MTG0_CR_MTG_CH0_LEM1_RSVD31_8_DEF                            (0x00000000)
  #define MTG0_CR_MTG_CH0_LEM1_RSVD31_8_HSH                            (0x1810B3A4)

#define MTG0_CR_MTG_CH0_CLEM_REG                                       (0x0000B3A8)
#define MTG0_CR_MTG_CH0_CLEM_DEF                                       (0x00000000)
#define MTG0_CR_MTG_CH0_CLEM_VOLATILE_BITFIELDS_MSK                    (0x00000000)

  #define MTG0_CR_MTG_CH0_CLEM_CACHELINE_ERR_MASK_OFF                  ( 0)
  #define MTG0_CR_MTG_CH0_CLEM_CACHELINE_ERR_MASK_WID                  (32)
  #define MTG0_CR_MTG_CH0_CLEM_CACHELINE_ERR_MASK_MSK                  (0xFFFFFFFF)
  #define MTG0_CR_MTG_CH0_CLEM_CACHELINE_ERR_MASK_MIN                  (0)
  #define MTG0_CR_MTG_CH0_CLEM_CACHELINE_ERR_MASK_MAX                  (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_CH0_CLEM_CACHELINE_ERR_MASK_DEF                  (0x00000000)
  #define MTG0_CR_MTG_CH0_CLEM_CACHELINE_ERR_MASK_HSH                  (0x2000B3A8)

#define MTG0_CR_MTG_CH0_BYTE_ERR0_REG                                  (0x0000B3B0)
#define MTG0_CR_MTG_CH0_BYTE_ERR0_DEF                                  (0x00000000)
#define MTG0_CR_MTG_CH0_BYTE_ERR0_VOLATILE_BITFIELDS_MSK               (0xFFFFFFFF)

  #define MTG0_CR_MTG_CH0_BYTE_ERR0_DATA_BYTE_ERR_0_OFF                ( 0)
  #define MTG0_CR_MTG_CH0_BYTE_ERR0_DATA_BYTE_ERR_0_WID                (32)
  #define MTG0_CR_MTG_CH0_BYTE_ERR0_DATA_BYTE_ERR_0_MSK                (0xFFFFFFFF)
  #define MTG0_CR_MTG_CH0_BYTE_ERR0_DATA_BYTE_ERR_0_MIN                (0)
  #define MTG0_CR_MTG_CH0_BYTE_ERR0_DATA_BYTE_ERR_0_MAX                (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_CH0_BYTE_ERR0_DATA_BYTE_ERR_0_DEF                (0x00000000)
  #define MTG0_CR_MTG_CH0_BYTE_ERR0_DATA_BYTE_ERR_0_HSH                (0x2080B3B0)

#define MTG0_CR_MTG_CH0_BYTE_ERR1_REG                                  (0x0000B3B4)
#define MTG0_CR_MTG_CH0_BYTE_ERR1_DEF                                  (0x00000000)
#define MTG0_CR_MTG_CH0_BYTE_ERR1_VOLATILE_BITFIELDS_MSK               (0xFFFFFFFF)

  #define MTG0_CR_MTG_CH0_BYTE_ERR1_DATA_BYTE_ERR_1_OFF                ( 0)
  #define MTG0_CR_MTG_CH0_BYTE_ERR1_DATA_BYTE_ERR_1_WID                (32)
  #define MTG0_CR_MTG_CH0_BYTE_ERR1_DATA_BYTE_ERR_1_MSK                (0xFFFFFFFF)
  #define MTG0_CR_MTG_CH0_BYTE_ERR1_DATA_BYTE_ERR_1_MIN                (0)
  #define MTG0_CR_MTG_CH0_BYTE_ERR1_DATA_BYTE_ERR_1_MAX                (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_CH0_BYTE_ERR1_DATA_BYTE_ERR_1_DEF                (0x00000000)
  #define MTG0_CR_MTG_CH0_BYTE_ERR1_DATA_BYTE_ERR_1_HSH                (0x2080B3B4)

#define MTG0_CR_MTG_CH0_BYTE_ERR2_REG                                  (0x0000B3B8)
#define MTG0_CR_MTG_CH0_BYTE_ERR2_DEF                                  (0x00000000)
#define MTG0_CR_MTG_CH0_BYTE_ERR2_VOLATILE_BITFIELDS_MSK               (0x0000FFFF)

  #define MTG0_CR_MTG_CH0_BYTE_ERR2_ECC_BYTE_ERR_OFF                   ( 0)
  #define MTG0_CR_MTG_CH0_BYTE_ERR2_ECC_BYTE_ERR_WID                   (16)
  #define MTG0_CR_MTG_CH0_BYTE_ERR2_ECC_BYTE_ERR_MSK                   (0x0000FFFF)
  #define MTG0_CR_MTG_CH0_BYTE_ERR2_ECC_BYTE_ERR_MIN                   (0)
  #define MTG0_CR_MTG_CH0_BYTE_ERR2_ECC_BYTE_ERR_MAX                   (65535) // 0x0000FFFF
  #define MTG0_CR_MTG_CH0_BYTE_ERR2_ECC_BYTE_ERR_DEF                   (0x00000000)
  #define MTG0_CR_MTG_CH0_BYTE_ERR2_ECC_BYTE_ERR_HSH                   (0x1080B3B8)

  #define MTG0_CR_MTG_CH0_BYTE_ERR2_RSVD31_16_OFF                      (16)
  #define MTG0_CR_MTG_CH0_BYTE_ERR2_RSVD31_16_WID                      (16)
  #define MTG0_CR_MTG_CH0_BYTE_ERR2_RSVD31_16_MSK                      (0xFFFF0000)
  #define MTG0_CR_MTG_CH0_BYTE_ERR2_RSVD31_16_MIN                      (0)
  #define MTG0_CR_MTG_CH0_BYTE_ERR2_RSVD31_16_MAX                      (65535) // 0x0000FFFF
  #define MTG0_CR_MTG_CH0_BYTE_ERR2_RSVD31_16_DEF                      (0x00000000)
  #define MTG0_CR_MTG_CH0_BYTE_ERR2_RSVD31_16_HSH                      (0x1020B3B8)

#define MTG0_CR_MTG_CH0_UI_ERR_REG                                     (0x0000B3BC)
#define MTG0_CR_MTG_CH0_UI_ERR_DEF                                     (0x00000000)
#define MTG0_CR_MTG_CH0_UI_ERR_VOLATILE_BITFIELDS_MSK                  (0xFFFFFFFF)

  #define MTG0_CR_MTG_CH0_UI_ERR_UI_ERR_OFF                            ( 0)
  #define MTG0_CR_MTG_CH0_UI_ERR_UI_ERR_WID                            (32)
  #define MTG0_CR_MTG_CH0_UI_ERR_UI_ERR_MSK                            (0xFFFFFFFF)
  #define MTG0_CR_MTG_CH0_UI_ERR_UI_ERR_MIN                            (0)
  #define MTG0_CR_MTG_CH0_UI_ERR_UI_ERR_MAX                            (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_CH0_UI_ERR_UI_ERR_DEF                            (0x00000000)
  #define MTG0_CR_MTG_CH0_UI_ERR_UI_ERR_HSH                            (0x2080B3BC)

#define MTG0_CR_MTG_CH0_EUILANE_ERR0_REG                               (0x0000B3C0)
#define MTG0_CR_MTG_CH0_EUILANE_ERR0_DEF                               (0x00000000)
#define MTG0_CR_MTG_CH0_EUILANE_ERR0_VOLATILE_BITFIELDS_MSK            (0xFFFFFFFF)

  #define MTG0_CR_MTG_CH0_EUILANE_ERR0_EVEN_UI_DQ_LANE_ERR_0_OFF       ( 0)
  #define MTG0_CR_MTG_CH0_EUILANE_ERR0_EVEN_UI_DQ_LANE_ERR_0_WID       (32)
  #define MTG0_CR_MTG_CH0_EUILANE_ERR0_EVEN_UI_DQ_LANE_ERR_0_MSK       (0xFFFFFFFF)
  #define MTG0_CR_MTG_CH0_EUILANE_ERR0_EVEN_UI_DQ_LANE_ERR_0_MIN       (0)
  #define MTG0_CR_MTG_CH0_EUILANE_ERR0_EVEN_UI_DQ_LANE_ERR_0_MAX       (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_CH0_EUILANE_ERR0_EVEN_UI_DQ_LANE_ERR_0_DEF       (0x00000000)
  #define MTG0_CR_MTG_CH0_EUILANE_ERR0_EVEN_UI_DQ_LANE_ERR_0_HSH       (0x2080B3C0)

#define MTG0_CR_MTG_CH0_EUILANE_ERR1_REG                               (0x0000B3C4)
#define MTG0_CR_MTG_CH0_EUILANE_ERR1_DEF                               (0x00000000)
#define MTG0_CR_MTG_CH0_EUILANE_ERR1_VOLATILE_BITFIELDS_MSK            (0x000000FF)

  #define MTG0_CR_MTG_CH0_EUILANE_ERR1_EVEN_UI_DQ_LANE_ERR_1_OFF       ( 0)
  #define MTG0_CR_MTG_CH0_EUILANE_ERR1_EVEN_UI_DQ_LANE_ERR_1_WID       ( 8)
  #define MTG0_CR_MTG_CH0_EUILANE_ERR1_EVEN_UI_DQ_LANE_ERR_1_MSK       (0x000000FF)
  #define MTG0_CR_MTG_CH0_EUILANE_ERR1_EVEN_UI_DQ_LANE_ERR_1_MIN       (0)
  #define MTG0_CR_MTG_CH0_EUILANE_ERR1_EVEN_UI_DQ_LANE_ERR_1_MAX       (255) // 0x000000FF
  #define MTG0_CR_MTG_CH0_EUILANE_ERR1_EVEN_UI_DQ_LANE_ERR_1_DEF       (0x00000000)
  #define MTG0_CR_MTG_CH0_EUILANE_ERR1_EVEN_UI_DQ_LANE_ERR_1_HSH       (0x0880B3C4)

  #define MTG0_CR_MTG_CH0_EUILANE_ERR1_RSVD31_8_OFF                    ( 8)
  #define MTG0_CR_MTG_CH0_EUILANE_ERR1_RSVD31_8_WID                    (24)
  #define MTG0_CR_MTG_CH0_EUILANE_ERR1_RSVD31_8_MSK                    (0xFFFFFF00)
  #define MTG0_CR_MTG_CH0_EUILANE_ERR1_RSVD31_8_MIN                    (0)
  #define MTG0_CR_MTG_CH0_EUILANE_ERR1_RSVD31_8_MAX                    (16777215) // 0x00FFFFFF
  #define MTG0_CR_MTG_CH0_EUILANE_ERR1_RSVD31_8_DEF                    (0x00000000)
  #define MTG0_CR_MTG_CH0_EUILANE_ERR1_RSVD31_8_HSH                    (0x1810B3C4)

#define MTG0_CR_MTG_CH0_OUILANE_ERR0_REG                               (0x0000B3C8)
#define MTG0_CR_MTG_CH0_OUILANE_ERR0_DEF                               (0x00000000)
#define MTG0_CR_MTG_CH0_OUILANE_ERR0_VOLATILE_BITFIELDS_MSK            (0xFFFFFFFF)

  #define MTG0_CR_MTG_CH0_OUILANE_ERR0_ODD_UI_DQ_LANE_ERR_0_OFF        ( 0)
  #define MTG0_CR_MTG_CH0_OUILANE_ERR0_ODD_UI_DQ_LANE_ERR_0_WID        (32)
  #define MTG0_CR_MTG_CH0_OUILANE_ERR0_ODD_UI_DQ_LANE_ERR_0_MSK        (0xFFFFFFFF)
  #define MTG0_CR_MTG_CH0_OUILANE_ERR0_ODD_UI_DQ_LANE_ERR_0_MIN        (0)
  #define MTG0_CR_MTG_CH0_OUILANE_ERR0_ODD_UI_DQ_LANE_ERR_0_MAX        (4294967295) // 0xFFFFFFFF
  #define MTG0_CR_MTG_CH0_OUILANE_ERR0_ODD_UI_DQ_LANE_ERR_0_DEF        (0x00000000)
  #define MTG0_CR_MTG_CH0_OUILANE_ERR0_ODD_UI_DQ_LANE_ERR_0_HSH        (0x2080B3C8)

#define MTG0_CR_MTG_CH0_OUILANE_ERR1_REG                               (0x0000B3CC)
#define MTG0_CR_MTG_CH0_OUILANE_ERR1_DEF                               (0x00000000)
#define MTG0_CR_MTG_CH0_OUILANE_ERR1_VOLATILE_BITFIELDS_MSK            (0x000000FF)

  #define MTG0_CR_MTG_CH0_OUILANE_ERR1_ODD_UI_DQ_LANE_ERR_1_OFF        ( 0)
  #define MTG0_CR_MTG_CH0_OUILANE_ERR1_ODD_UI_DQ_LANE_ERR_1_WID        ( 8)
  #define MTG0_CR_MTG_CH0_OUILANE_ERR1_ODD_UI_DQ_LANE_ERR_1_MSK        (0x000000FF)
  #define MTG0_CR_MTG_CH0_OUILANE_ERR1_ODD_UI_DQ_LANE_ERR_1_MIN        (0)
  #define MTG0_CR_MTG_CH0_OUILANE_ERR1_ODD_UI_DQ_LANE_ERR_1_MAX        (255) // 0x000000FF
  #define MTG0_CR_MTG_CH0_OUILANE_ERR1_ODD_UI_DQ_LANE_ERR_1_DEF        (0x00000000)
  #define MTG0_CR_MTG_CH0_OUILANE_ERR1_ODD_UI_DQ_LANE_ERR_1_HSH        (0x0880B3CC)

  #define MTG0_CR_MTG_CH0_OUILANE_ERR1_RSVD31_8_OFF                    ( 8)
  #define MTG0_CR_MTG_CH0_OUILANE_ERR1_RSVD31_8_WID                    (24)
  #define MTG0_CR_MTG_CH0_OUILANE_ERR1_RSVD31_8_MSK                    (0xFFFFFF00)
  #define MTG0_CR_MTG_CH0_OUILANE_ERR1_RSVD31_8_MIN                    (0)
  #define MTG0_CR_MTG_CH0_OUILANE_ERR1_RSVD31_8_MAX                    (16777215) // 0x00FFFFFF
  #define MTG0_CR_MTG_CH0_OUILANE_ERR1_RSVD31_8_DEF                    (0x00000000)
  #define MTG0_CR_MTG_CH0_OUILANE_ERR1_RSVD31_8_HSH                    (0x1810B3CC)

#define MTG0_CR_MTG_CH0_BLEC0_REG                                      (0x0000B3D0)
#define MTG0_CR_MTG_CH0_BLEC0_DEF                                      (0x00000000)
#define MTG0_CR_MTG_CH0_BLEC0_VOLATILE_BITFIELDS_MSK                   (0xFFFFFFFF)

  #define MTG0_CR_MTG_CH0_BLEC0_BYTE_LANE0_ERR_CNT_OFF                 ( 0)
  #define MTG0_CR_MTG_CH0_BLEC0_BYTE_LANE0_ERR_CNT_WID                 (16)
  #define MTG0_CR_MTG_CH0_BLEC0_BYTE_LANE0_ERR_CNT_MSK                 (0x0000FFFF)
  #define MTG0_CR_MTG_CH0_BLEC0_BYTE_LANE0_ERR_CNT_MIN                 (0)
  #define MTG0_CR_MTG_CH0_BLEC0_BYTE_LANE0_ERR_CNT_MAX                 (65535) // 0x0000FFFF
  #define MTG0_CR_MTG_CH0_BLEC0_BYTE_LANE0_ERR_CNT_DEF                 (0x00000000)
  #define MTG0_CR_MTG_CH0_BLEC0_BYTE_LANE0_ERR_CNT_HSH                 (0x1080B3D0)

  #define MTG0_CR_MTG_CH0_BLEC0_BYTE_LANE1_ERR_CNT_OFF                 (16)
  #define MTG0_CR_MTG_CH0_BLEC0_BYTE_LANE1_ERR_CNT_WID                 (16)
  #define MTG0_CR_MTG_CH0_BLEC0_BYTE_LANE1_ERR_CNT_MSK                 (0xFFFF0000)
  #define MTG0_CR_MTG_CH0_BLEC0_BYTE_LANE1_ERR_CNT_MIN                 (0)
  #define MTG0_CR_MTG_CH0_BLEC0_BYTE_LANE1_ERR_CNT_MAX                 (65535) // 0x0000FFFF
  #define MTG0_CR_MTG_CH0_BLEC0_BYTE_LANE1_ERR_CNT_DEF                 (0x00000000)
  #define MTG0_CR_MTG_CH0_BLEC0_BYTE_LANE1_ERR_CNT_HSH                 (0x10A0B3D0)

#define MTG0_CR_MTG_CH0_BLEC1_REG                                      (0x0000B3D4)
#define MTG0_CR_MTG_CH0_BLEC1_DEF                                      (0x00000000)
#define MTG0_CR_MTG_CH0_BLEC1_VOLATILE_BITFIELDS_MSK                   (0xFFFFFFFF)

  #define MTG0_CR_MTG_CH0_BLEC1_BYTE_LANE2_ERR_CNT_OFF                 ( 0)
  #define MTG0_CR_MTG_CH0_BLEC1_BYTE_LANE2_ERR_CNT_WID                 (16)
  #define MTG0_CR_MTG_CH0_BLEC1_BYTE_LANE2_ERR_CNT_MSK                 (0x0000FFFF)
  #define MTG0_CR_MTG_CH0_BLEC1_BYTE_LANE2_ERR_CNT_MIN                 (0)
  #define MTG0_CR_MTG_CH0_BLEC1_BYTE_LANE2_ERR_CNT_MAX                 (65535) // 0x0000FFFF
  #define MTG0_CR_MTG_CH0_BLEC1_BYTE_LANE2_ERR_CNT_DEF                 (0x00000000)
  #define MTG0_CR_MTG_CH0_BLEC1_BYTE_LANE2_ERR_CNT_HSH                 (0x1080B3D4)

  #define MTG0_CR_MTG_CH0_BLEC1_BYTE_LANE3_ERR_CNT_OFF                 (16)
  #define MTG0_CR_MTG_CH0_BLEC1_BYTE_LANE3_ERR_CNT_WID                 (16)
  #define MTG0_CR_MTG_CH0_BLEC1_BYTE_LANE3_ERR_CNT_MSK                 (0xFFFF0000)
  #define MTG0_CR_MTG_CH0_BLEC1_BYTE_LANE3_ERR_CNT_MIN                 (0)
  #define MTG0_CR_MTG_CH0_BLEC1_BYTE_LANE3_ERR_CNT_MAX                 (65535) // 0x0000FFFF
  #define MTG0_CR_MTG_CH0_BLEC1_BYTE_LANE3_ERR_CNT_DEF                 (0x00000000)
  #define MTG0_CR_MTG_CH0_BLEC1_BYTE_LANE3_ERR_CNT_HSH                 (0x10A0B3D4)

#define MTG0_CR_MTG_CH0_BLEC2_REG                                      (0x0000B3D8)
#define MTG0_CR_MTG_CH0_BLEC2_DEF                                      (0x00000000)
#define MTG0_CR_MTG_CH0_BLEC2_VOLATILE_BITFIELDS_MSK                   (0x0000FFFF)

  #define MTG0_CR_MTG_CH0_BLEC2_BYTE_LANE4_ERR_CNT_OFF                 ( 0)
  #define MTG0_CR_MTG_CH0_BLEC2_BYTE_LANE4_ERR_CNT_WID                 (16)
  #define MTG0_CR_MTG_CH0_BLEC2_BYTE_LANE4_ERR_CNT_MSK                 (0x0000FFFF)
  #define MTG0_CR_MTG_CH0_BLEC2_BYTE_LANE4_ERR_CNT_MIN                 (0)
  #define MTG0_CR_MTG_CH0_BLEC2_BYTE_LANE4_ERR_CNT_MAX                 (65535) // 0x0000FFFF
  #define MTG0_CR_MTG_CH0_BLEC2_BYTE_LANE4_ERR_CNT_DEF                 (0x00000000)
  #define MTG0_CR_MTG_CH0_BLEC2_BYTE_LANE4_ERR_CNT_HSH                 (0x1080B3D8)

  #define MTG0_CR_MTG_CH0_BLEC2_BYTE_LANE_ERR_CNTR_EN_OFF              (16)
  #define MTG0_CR_MTG_CH0_BLEC2_BYTE_LANE_ERR_CNTR_EN_WID              ( 5)
  #define MTG0_CR_MTG_CH0_BLEC2_BYTE_LANE_ERR_CNTR_EN_MSK              (0x001F0000)
  #define MTG0_CR_MTG_CH0_BLEC2_BYTE_LANE_ERR_CNTR_EN_MIN              (0)
  #define MTG0_CR_MTG_CH0_BLEC2_BYTE_LANE_ERR_CNTR_EN_MAX              (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_BLEC2_BYTE_LANE_ERR_CNTR_EN_DEF              (0x00000000)
  #define MTG0_CR_MTG_CH0_BLEC2_BYTE_LANE_ERR_CNTR_EN_HSH              (0x0520B3D8)

  #define MTG0_CR_MTG_CH0_BLEC2_RSVD31_21_OFF                          (21)
  #define MTG0_CR_MTG_CH0_BLEC2_RSVD31_21_WID                          (11)
  #define MTG0_CR_MTG_CH0_BLEC2_RSVD31_21_MSK                          (0xFFE00000)
  #define MTG0_CR_MTG_CH0_BLEC2_RSVD31_21_MIN                          (0)
  #define MTG0_CR_MTG_CH0_BLEC2_RSVD31_21_MAX                          (2047) // 0x000007FF
  #define MTG0_CR_MTG_CH0_BLEC2_RSVD31_21_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_BLEC2_RSVD31_21_HSH                          (0x0B2AB3D8)

#define MTG0_CR_MTG_CH0_DQEC0_REG                                      (0x0000B3E0)
#define MTG0_CR_MTG_CH0_DQEC0_DEF                                      (0x00000000)
#define MTG0_CR_MTG_CH0_DQEC0_VOLATILE_BITFIELDS_MSK                   (0x1F1F1F1F)

  #define MTG0_CR_MTG_CH0_DQEC0_DQ0_ERR_CNT_OFF                        ( 0)
  #define MTG0_CR_MTG_CH0_DQEC0_DQ0_ERR_CNT_WID                        ( 5)
  #define MTG0_CR_MTG_CH0_DQEC0_DQ0_ERR_CNT_MSK                        (0x0000001F)
  #define MTG0_CR_MTG_CH0_DQEC0_DQ0_ERR_CNT_MIN                        (0)
  #define MTG0_CR_MTG_CH0_DQEC0_DQ0_ERR_CNT_MAX                        (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC0_DQ0_ERR_CNT_DEF                        (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC0_DQ0_ERR_CNT_HSH                        (0x0580B3E0)

  #define MTG0_CR_MTG_CH0_DQEC0_RSVD7_5_OFF                            ( 5)
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD7_5_WID                            ( 3)
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD7_5_MSK                            (0x000000E0)
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD7_5_MIN                            (0)
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD7_5_MAX                            (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD7_5_DEF                            (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD7_5_HSH                            (0x030AB3E0)

  #define MTG0_CR_MTG_CH0_DQEC0_DQ1_ERR_CNT_OFF                        ( 8)
  #define MTG0_CR_MTG_CH0_DQEC0_DQ1_ERR_CNT_WID                        ( 5)
  #define MTG0_CR_MTG_CH0_DQEC0_DQ1_ERR_CNT_MSK                        (0x00001F00)
  #define MTG0_CR_MTG_CH0_DQEC0_DQ1_ERR_CNT_MIN                        (0)
  #define MTG0_CR_MTG_CH0_DQEC0_DQ1_ERR_CNT_MAX                        (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC0_DQ1_ERR_CNT_DEF                        (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC0_DQ1_ERR_CNT_HSH                        (0x0590B3E0)

  #define MTG0_CR_MTG_CH0_DQEC0_RSVD15_13_OFF                          (13)
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD15_13_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD15_13_MSK                          (0x0000E000)
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD15_13_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD15_13_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD15_13_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD15_13_HSH                          (0x031AB3E0)

  #define MTG0_CR_MTG_CH0_DQEC0_DQ2_ERR_CNT_OFF                        (16)
  #define MTG0_CR_MTG_CH0_DQEC0_DQ2_ERR_CNT_WID                        ( 5)
  #define MTG0_CR_MTG_CH0_DQEC0_DQ2_ERR_CNT_MSK                        (0x001F0000)
  #define MTG0_CR_MTG_CH0_DQEC0_DQ2_ERR_CNT_MIN                        (0)
  #define MTG0_CR_MTG_CH0_DQEC0_DQ2_ERR_CNT_MAX                        (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC0_DQ2_ERR_CNT_DEF                        (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC0_DQ2_ERR_CNT_HSH                        (0x05A0B3E0)

  #define MTG0_CR_MTG_CH0_DQEC0_RSVD23_21_OFF                          (21)
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD23_21_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD23_21_MSK                          (0x00E00000)
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD23_21_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD23_21_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD23_21_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD23_21_HSH                          (0x032AB3E0)

  #define MTG0_CR_MTG_CH0_DQEC0_DQ3_ERR_CNT_OFF                        (24)
  #define MTG0_CR_MTG_CH0_DQEC0_DQ3_ERR_CNT_WID                        ( 5)
  #define MTG0_CR_MTG_CH0_DQEC0_DQ3_ERR_CNT_MSK                        (0x1F000000)
  #define MTG0_CR_MTG_CH0_DQEC0_DQ3_ERR_CNT_MIN                        (0)
  #define MTG0_CR_MTG_CH0_DQEC0_DQ3_ERR_CNT_MAX                        (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC0_DQ3_ERR_CNT_DEF                        (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC0_DQ3_ERR_CNT_HSH                        (0x05B0B3E0)

  #define MTG0_CR_MTG_CH0_DQEC0_RSVD31_29_OFF                          (29)
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD31_29_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD31_29_MSK                          (0xE0000000)
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD31_29_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD31_29_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD31_29_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC0_RSVD31_29_HSH                          (0x033AB3E0)

#define MTG0_CR_MTG_CH0_DQEC1_REG                                      (0x0000B3E4)
#define MTG0_CR_MTG_CH0_DQEC1_DEF                                      (0x00000000)
#define MTG0_CR_MTG_CH0_DQEC1_VOLATILE_BITFIELDS_MSK                   (0x1F1F1F1F)

  #define MTG0_CR_MTG_CH0_DQEC1_DQ4_ERR_CNT_OFF                        ( 0)
  #define MTG0_CR_MTG_CH0_DQEC1_DQ4_ERR_CNT_WID                        ( 5)
  #define MTG0_CR_MTG_CH0_DQEC1_DQ4_ERR_CNT_MSK                        (0x0000001F)
  #define MTG0_CR_MTG_CH0_DQEC1_DQ4_ERR_CNT_MIN                        (0)
  #define MTG0_CR_MTG_CH0_DQEC1_DQ4_ERR_CNT_MAX                        (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC1_DQ4_ERR_CNT_DEF                        (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC1_DQ4_ERR_CNT_HSH                        (0x0580B3E4)

  #define MTG0_CR_MTG_CH0_DQEC1_RSVD7_5_OFF                            ( 5)
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD7_5_WID                            ( 3)
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD7_5_MSK                            (0x000000E0)
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD7_5_MIN                            (0)
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD7_5_MAX                            (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD7_5_DEF                            (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD7_5_HSH                            (0x030AB3E4)

  #define MTG0_CR_MTG_CH0_DQEC1_DQ5_ERR_CNT_OFF                        ( 8)
  #define MTG0_CR_MTG_CH0_DQEC1_DQ5_ERR_CNT_WID                        ( 5)
  #define MTG0_CR_MTG_CH0_DQEC1_DQ5_ERR_CNT_MSK                        (0x00001F00)
  #define MTG0_CR_MTG_CH0_DQEC1_DQ5_ERR_CNT_MIN                        (0)
  #define MTG0_CR_MTG_CH0_DQEC1_DQ5_ERR_CNT_MAX                        (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC1_DQ5_ERR_CNT_DEF                        (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC1_DQ5_ERR_CNT_HSH                        (0x0590B3E4)

  #define MTG0_CR_MTG_CH0_DQEC1_RSVD15_13_OFF                          (13)
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD15_13_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD15_13_MSK                          (0x0000E000)
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD15_13_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD15_13_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD15_13_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD15_13_HSH                          (0x031AB3E4)

  #define MTG0_CR_MTG_CH0_DQEC1_DQ6_ERR_CNT_OFF                        (16)
  #define MTG0_CR_MTG_CH0_DQEC1_DQ6_ERR_CNT_WID                        ( 5)
  #define MTG0_CR_MTG_CH0_DQEC1_DQ6_ERR_CNT_MSK                        (0x001F0000)
  #define MTG0_CR_MTG_CH0_DQEC1_DQ6_ERR_CNT_MIN                        (0)
  #define MTG0_CR_MTG_CH0_DQEC1_DQ6_ERR_CNT_MAX                        (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC1_DQ6_ERR_CNT_DEF                        (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC1_DQ6_ERR_CNT_HSH                        (0x05A0B3E4)

  #define MTG0_CR_MTG_CH0_DQEC1_RSVD23_21_OFF                          (21)
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD23_21_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD23_21_MSK                          (0x00E00000)
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD23_21_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD23_21_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD23_21_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD23_21_HSH                          (0x032AB3E4)

  #define MTG0_CR_MTG_CH0_DQEC1_DQ7_ERR_CNT_OFF                        (24)
  #define MTG0_CR_MTG_CH0_DQEC1_DQ7_ERR_CNT_WID                        ( 5)
  #define MTG0_CR_MTG_CH0_DQEC1_DQ7_ERR_CNT_MSK                        (0x1F000000)
  #define MTG0_CR_MTG_CH0_DQEC1_DQ7_ERR_CNT_MIN                        (0)
  #define MTG0_CR_MTG_CH0_DQEC1_DQ7_ERR_CNT_MAX                        (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC1_DQ7_ERR_CNT_DEF                        (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC1_DQ7_ERR_CNT_HSH                        (0x05B0B3E4)

  #define MTG0_CR_MTG_CH0_DQEC1_RSVD31_29_OFF                          (29)
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD31_29_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD31_29_MSK                          (0xE0000000)
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD31_29_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD31_29_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD31_29_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC1_RSVD31_29_HSH                          (0x033AB3E4)

#define MTG0_CR_MTG_CH0_DQEC2_REG                                      (0x0000B3E8)
#define MTG0_CR_MTG_CH0_DQEC2_DEF                                      (0x00000000)
#define MTG0_CR_MTG_CH0_DQEC2_VOLATILE_BITFIELDS_MSK                   (0x1F1F1F1F)

  #define MTG0_CR_MTG_CH0_DQEC2_DQ8_ERR_CNT_OFF                        ( 0)
  #define MTG0_CR_MTG_CH0_DQEC2_DQ8_ERR_CNT_WID                        ( 5)
  #define MTG0_CR_MTG_CH0_DQEC2_DQ8_ERR_CNT_MSK                        (0x0000001F)
  #define MTG0_CR_MTG_CH0_DQEC2_DQ8_ERR_CNT_MIN                        (0)
  #define MTG0_CR_MTG_CH0_DQEC2_DQ8_ERR_CNT_MAX                        (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC2_DQ8_ERR_CNT_DEF                        (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC2_DQ8_ERR_CNT_HSH                        (0x0580B3E8)

  #define MTG0_CR_MTG_CH0_DQEC2_RSVD7_5_OFF                            ( 5)
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD7_5_WID                            ( 3)
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD7_5_MSK                            (0x000000E0)
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD7_5_MIN                            (0)
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD7_5_MAX                            (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD7_5_DEF                            (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD7_5_HSH                            (0x030AB3E8)

  #define MTG0_CR_MTG_CH0_DQEC2_DQ9_ERR_CNT_OFF                        ( 8)
  #define MTG0_CR_MTG_CH0_DQEC2_DQ9_ERR_CNT_WID                        ( 5)
  #define MTG0_CR_MTG_CH0_DQEC2_DQ9_ERR_CNT_MSK                        (0x00001F00)
  #define MTG0_CR_MTG_CH0_DQEC2_DQ9_ERR_CNT_MIN                        (0)
  #define MTG0_CR_MTG_CH0_DQEC2_DQ9_ERR_CNT_MAX                        (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC2_DQ9_ERR_CNT_DEF                        (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC2_DQ9_ERR_CNT_HSH                        (0x0590B3E8)

  #define MTG0_CR_MTG_CH0_DQEC2_RSVD15_13_OFF                          (13)
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD15_13_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD15_13_MSK                          (0x0000E000)
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD15_13_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD15_13_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD15_13_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD15_13_HSH                          (0x031AB3E8)

  #define MTG0_CR_MTG_CH0_DQEC2_DQ10_ERR_CNT_OFF                       (16)
  #define MTG0_CR_MTG_CH0_DQEC2_DQ10_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC2_DQ10_ERR_CNT_MSK                       (0x001F0000)
  #define MTG0_CR_MTG_CH0_DQEC2_DQ10_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC2_DQ10_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC2_DQ10_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC2_DQ10_ERR_CNT_HSH                       (0x05A0B3E8)

  #define MTG0_CR_MTG_CH0_DQEC2_RSVD23_21_OFF                          (21)
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD23_21_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD23_21_MSK                          (0x00E00000)
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD23_21_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD23_21_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD23_21_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD23_21_HSH                          (0x032AB3E8)

  #define MTG0_CR_MTG_CH0_DQEC2_DQ11_ERR_CNT_OFF                       (24)
  #define MTG0_CR_MTG_CH0_DQEC2_DQ11_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC2_DQ11_ERR_CNT_MSK                       (0x1F000000)
  #define MTG0_CR_MTG_CH0_DQEC2_DQ11_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC2_DQ11_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC2_DQ11_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC2_DQ11_ERR_CNT_HSH                       (0x05B0B3E8)

  #define MTG0_CR_MTG_CH0_DQEC2_RSVD31_29_OFF                          (29)
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD31_29_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD31_29_MSK                          (0xE0000000)
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD31_29_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD31_29_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD31_29_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC2_RSVD31_29_HSH                          (0x033AB3E8)

#define MTG0_CR_MTG_CH0_DQEC3_REG                                      (0x0000B3EC)
#define MTG0_CR_MTG_CH0_DQEC3_DEF                                      (0x00000000)
#define MTG0_CR_MTG_CH0_DQEC3_VOLATILE_BITFIELDS_MSK                   (0x1F1F1F1F)

  #define MTG0_CR_MTG_CH0_DQEC3_DQ12_ERR_CNT_OFF                       ( 0)
  #define MTG0_CR_MTG_CH0_DQEC3_DQ12_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC3_DQ12_ERR_CNT_MSK                       (0x0000001F)
  #define MTG0_CR_MTG_CH0_DQEC3_DQ12_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC3_DQ12_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC3_DQ12_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC3_DQ12_ERR_CNT_HSH                       (0x0580B3EC)

  #define MTG0_CR_MTG_CH0_DQEC3_RSVD7_5_OFF                            ( 5)
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD7_5_WID                            ( 3)
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD7_5_MSK                            (0x000000E0)
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD7_5_MIN                            (0)
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD7_5_MAX                            (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD7_5_DEF                            (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD7_5_HSH                            (0x030AB3EC)

  #define MTG0_CR_MTG_CH0_DQEC3_DQ13_ERR_CNT_OFF                       ( 8)
  #define MTG0_CR_MTG_CH0_DQEC3_DQ13_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC3_DQ13_ERR_CNT_MSK                       (0x00001F00)
  #define MTG0_CR_MTG_CH0_DQEC3_DQ13_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC3_DQ13_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC3_DQ13_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC3_DQ13_ERR_CNT_HSH                       (0x0590B3EC)

  #define MTG0_CR_MTG_CH0_DQEC3_RSVD15_13_OFF                          (13)
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD15_13_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD15_13_MSK                          (0x0000E000)
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD15_13_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD15_13_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD15_13_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD15_13_HSH                          (0x031AB3EC)

  #define MTG0_CR_MTG_CH0_DQEC3_DQ14_ERR_CNT_OFF                       (16)
  #define MTG0_CR_MTG_CH0_DQEC3_DQ14_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC3_DQ14_ERR_CNT_MSK                       (0x001F0000)
  #define MTG0_CR_MTG_CH0_DQEC3_DQ14_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC3_DQ14_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC3_DQ14_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC3_DQ14_ERR_CNT_HSH                       (0x05A0B3EC)

  #define MTG0_CR_MTG_CH0_DQEC3_RSVD23_21_OFF                          (21)
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD23_21_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD23_21_MSK                          (0x00E00000)
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD23_21_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD23_21_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD23_21_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD23_21_HSH                          (0x032AB3EC)

  #define MTG0_CR_MTG_CH0_DQEC3_DQ15_ERR_CNT_OFF                       (24)
  #define MTG0_CR_MTG_CH0_DQEC3_DQ15_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC3_DQ15_ERR_CNT_MSK                       (0x1F000000)
  #define MTG0_CR_MTG_CH0_DQEC3_DQ15_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC3_DQ15_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC3_DQ15_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC3_DQ15_ERR_CNT_HSH                       (0x05B0B3EC)

  #define MTG0_CR_MTG_CH0_DQEC3_RSVD31_29_OFF                          (29)
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD31_29_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD31_29_MSK                          (0xE0000000)
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD31_29_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD31_29_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD31_29_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC3_RSVD31_29_HSH                          (0x033AB3EC)

#define MTG0_CR_MTG_CH0_DQEC4_REG                                      (0x0000B3F0)
#define MTG0_CR_MTG_CH0_DQEC4_DEF                                      (0x00000000)
#define MTG0_CR_MTG_CH0_DQEC4_VOLATILE_BITFIELDS_MSK                   (0x1F1F1F1F)

  #define MTG0_CR_MTG_CH0_DQEC4_DQ16_ERR_CNT_OFF                       ( 0)
  #define MTG0_CR_MTG_CH0_DQEC4_DQ16_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC4_DQ16_ERR_CNT_MSK                       (0x0000001F)
  #define MTG0_CR_MTG_CH0_DQEC4_DQ16_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC4_DQ16_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC4_DQ16_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC4_DQ16_ERR_CNT_HSH                       (0x0580B3F0)

  #define MTG0_CR_MTG_CH0_DQEC4_RSVD7_5_OFF                            ( 5)
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD7_5_WID                            ( 3)
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD7_5_MSK                            (0x000000E0)
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD7_5_MIN                            (0)
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD7_5_MAX                            (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD7_5_DEF                            (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD7_5_HSH                            (0x030AB3F0)

  #define MTG0_CR_MTG_CH0_DQEC4_DQ17_ERR_CNT_OFF                       ( 8)
  #define MTG0_CR_MTG_CH0_DQEC4_DQ17_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC4_DQ17_ERR_CNT_MSK                       (0x00001F00)
  #define MTG0_CR_MTG_CH0_DQEC4_DQ17_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC4_DQ17_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC4_DQ17_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC4_DQ17_ERR_CNT_HSH                       (0x0590B3F0)

  #define MTG0_CR_MTG_CH0_DQEC4_RSVD15_13_OFF                          (13)
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD15_13_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD15_13_MSK                          (0x0000E000)
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD15_13_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD15_13_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD15_13_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD15_13_HSH                          (0x031AB3F0)

  #define MTG0_CR_MTG_CH0_DQEC4_DQ18_ERR_CNT_OFF                       (16)
  #define MTG0_CR_MTG_CH0_DQEC4_DQ18_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC4_DQ18_ERR_CNT_MSK                       (0x001F0000)
  #define MTG0_CR_MTG_CH0_DQEC4_DQ18_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC4_DQ18_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC4_DQ18_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC4_DQ18_ERR_CNT_HSH                       (0x05A0B3F0)

  #define MTG0_CR_MTG_CH0_DQEC4_RSVD23_21_OFF                          (21)
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD23_21_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD23_21_MSK                          (0x00E00000)
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD23_21_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD23_21_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD23_21_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD23_21_HSH                          (0x032AB3F0)

  #define MTG0_CR_MTG_CH0_DQEC4_DQ19_ERR_CNT_OFF                       (24)
  #define MTG0_CR_MTG_CH0_DQEC4_DQ19_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC4_DQ19_ERR_CNT_MSK                       (0x1F000000)
  #define MTG0_CR_MTG_CH0_DQEC4_DQ19_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC4_DQ19_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC4_DQ19_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC4_DQ19_ERR_CNT_HSH                       (0x05B0B3F0)

  #define MTG0_CR_MTG_CH0_DQEC4_RSVD31_29_OFF                          (29)
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD31_29_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD31_29_MSK                          (0xE0000000)
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD31_29_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD31_29_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD31_29_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC4_RSVD31_29_HSH                          (0x033AB3F0)

#define MTG0_CR_MTG_CH0_DQEC5_REG                                      (0x0000B3F4)
#define MTG0_CR_MTG_CH0_DQEC5_DEF                                      (0x00000000)
#define MTG0_CR_MTG_CH0_DQEC5_VOLATILE_BITFIELDS_MSK                   (0x1F1F1F1F)

  #define MTG0_CR_MTG_CH0_DQEC5_DQ20_ERR_CNT_OFF                       ( 0)
  #define MTG0_CR_MTG_CH0_DQEC5_DQ20_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC5_DQ20_ERR_CNT_MSK                       (0x0000001F)
  #define MTG0_CR_MTG_CH0_DQEC5_DQ20_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC5_DQ20_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC5_DQ20_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC5_DQ20_ERR_CNT_HSH                       (0x0580B3F4)

  #define MTG0_CR_MTG_CH0_DQEC5_RSVD7_5_OFF                            ( 5)
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD7_5_WID                            ( 3)
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD7_5_MSK                            (0x000000E0)
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD7_5_MIN                            (0)
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD7_5_MAX                            (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD7_5_DEF                            (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD7_5_HSH                            (0x030AB3F4)

  #define MTG0_CR_MTG_CH0_DQEC5_DQ21_ERR_CNT_OFF                       ( 8)
  #define MTG0_CR_MTG_CH0_DQEC5_DQ21_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC5_DQ21_ERR_CNT_MSK                       (0x00001F00)
  #define MTG0_CR_MTG_CH0_DQEC5_DQ21_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC5_DQ21_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC5_DQ21_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC5_DQ21_ERR_CNT_HSH                       (0x0590B3F4)

  #define MTG0_CR_MTG_CH0_DQEC5_RSVD15_13_OFF                          (13)
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD15_13_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD15_13_MSK                          (0x0000E000)
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD15_13_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD15_13_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD15_13_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD15_13_HSH                          (0x031AB3F4)

  #define MTG0_CR_MTG_CH0_DQEC5_DQ22_ERR_CNT_OFF                       (16)
  #define MTG0_CR_MTG_CH0_DQEC5_DQ22_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC5_DQ22_ERR_CNT_MSK                       (0x001F0000)
  #define MTG0_CR_MTG_CH0_DQEC5_DQ22_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC5_DQ22_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC5_DQ22_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC5_DQ22_ERR_CNT_HSH                       (0x05A0B3F4)

  #define MTG0_CR_MTG_CH0_DQEC5_RSVD23_21_OFF                          (21)
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD23_21_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD23_21_MSK                          (0x00E00000)
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD23_21_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD23_21_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD23_21_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD23_21_HSH                          (0x032AB3F4)

  #define MTG0_CR_MTG_CH0_DQEC5_DQ23_ERR_CNT_OFF                       (24)
  #define MTG0_CR_MTG_CH0_DQEC5_DQ23_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC5_DQ23_ERR_CNT_MSK                       (0x1F000000)
  #define MTG0_CR_MTG_CH0_DQEC5_DQ23_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC5_DQ23_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC5_DQ23_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC5_DQ23_ERR_CNT_HSH                       (0x05B0B3F4)

  #define MTG0_CR_MTG_CH0_DQEC5_RSVD31_29_OFF                          (29)
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD31_29_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD31_29_MSK                          (0xE0000000)
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD31_29_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD31_29_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD31_29_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC5_RSVD31_29_HSH                          (0x033AB3F4)

#define MTG0_CR_MTG_CH0_DQEC6_REG                                      (0x0000B3F8)
#define MTG0_CR_MTG_CH0_DQEC6_DEF                                      (0x00000000)
#define MTG0_CR_MTG_CH0_DQEC6_VOLATILE_BITFIELDS_MSK                   (0x1F1F1F1F)

  #define MTG0_CR_MTG_CH0_DQEC6_DQ24_ERR_CNT_OFF                       ( 0)
  #define MTG0_CR_MTG_CH0_DQEC6_DQ24_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC6_DQ24_ERR_CNT_MSK                       (0x0000001F)
  #define MTG0_CR_MTG_CH0_DQEC6_DQ24_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC6_DQ24_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC6_DQ24_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC6_DQ24_ERR_CNT_HSH                       (0x0580B3F8)

  #define MTG0_CR_MTG_CH0_DQEC6_RSVD7_5_OFF                            ( 5)
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD7_5_WID                            ( 3)
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD7_5_MSK                            (0x000000E0)
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD7_5_MIN                            (0)
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD7_5_MAX                            (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD7_5_DEF                            (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD7_5_HSH                            (0x030AB3F8)

  #define MTG0_CR_MTG_CH0_DQEC6_DQ25_ERR_CNT_OFF                       ( 8)
  #define MTG0_CR_MTG_CH0_DQEC6_DQ25_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC6_DQ25_ERR_CNT_MSK                       (0x00001F00)
  #define MTG0_CR_MTG_CH0_DQEC6_DQ25_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC6_DQ25_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC6_DQ25_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC6_DQ25_ERR_CNT_HSH                       (0x0590B3F8)

  #define MTG0_CR_MTG_CH0_DQEC6_RSVD15_13_OFF                          (13)
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD15_13_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD15_13_MSK                          (0x0000E000)
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD15_13_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD15_13_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD15_13_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD15_13_HSH                          (0x031AB3F8)

  #define MTG0_CR_MTG_CH0_DQEC6_DQ26_ERR_CNT_OFF                       (16)
  #define MTG0_CR_MTG_CH0_DQEC6_DQ26_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC6_DQ26_ERR_CNT_MSK                       (0x001F0000)
  #define MTG0_CR_MTG_CH0_DQEC6_DQ26_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC6_DQ26_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC6_DQ26_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC6_DQ26_ERR_CNT_HSH                       (0x05A0B3F8)

  #define MTG0_CR_MTG_CH0_DQEC6_RSVD23_21_OFF                          (21)
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD23_21_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD23_21_MSK                          (0x00E00000)
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD23_21_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD23_21_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD23_21_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD23_21_HSH                          (0x032AB3F8)

  #define MTG0_CR_MTG_CH0_DQEC6_DQ27_ERR_CNT_OFF                       (24)
  #define MTG0_CR_MTG_CH0_DQEC6_DQ27_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC6_DQ27_ERR_CNT_MSK                       (0x1F000000)
  #define MTG0_CR_MTG_CH0_DQEC6_DQ27_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC6_DQ27_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC6_DQ27_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC6_DQ27_ERR_CNT_HSH                       (0x05B0B3F8)

  #define MTG0_CR_MTG_CH0_DQEC6_RSVD31_29_OFF                          (29)
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD31_29_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD31_29_MSK                          (0xE0000000)
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD31_29_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD31_29_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD31_29_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC6_RSVD31_29_HSH                          (0x033AB3F8)

#define MTG0_CR_MTG_CH0_DQEC7_REG                                      (0x0000B3FC)
#define MTG0_CR_MTG_CH0_DQEC7_DEF                                      (0x00000000)
#define MTG0_CR_MTG_CH0_DQEC7_VOLATILE_BITFIELDS_MSK                   (0x1F1F1F1F)

  #define MTG0_CR_MTG_CH0_DQEC7_DQ28_ERR_CNT_OFF                       ( 0)
  #define MTG0_CR_MTG_CH0_DQEC7_DQ28_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC7_DQ28_ERR_CNT_MSK                       (0x0000001F)
  #define MTG0_CR_MTG_CH0_DQEC7_DQ28_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC7_DQ28_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC7_DQ28_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC7_DQ28_ERR_CNT_HSH                       (0x0580B3FC)

  #define MTG0_CR_MTG_CH0_DQEC7_RSVD7_5_OFF                            ( 5)
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD7_5_WID                            ( 3)
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD7_5_MSK                            (0x000000E0)
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD7_5_MIN                            (0)
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD7_5_MAX                            (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD7_5_DEF                            (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD7_5_HSH                            (0x030AB3FC)

  #define MTG0_CR_MTG_CH0_DQEC7_DQ29_ERR_CNT_OFF                       ( 8)
  #define MTG0_CR_MTG_CH0_DQEC7_DQ29_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC7_DQ29_ERR_CNT_MSK                       (0x00001F00)
  #define MTG0_CR_MTG_CH0_DQEC7_DQ29_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC7_DQ29_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC7_DQ29_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC7_DQ29_ERR_CNT_HSH                       (0x0590B3FC)

  #define MTG0_CR_MTG_CH0_DQEC7_RSVD15_13_OFF                          (13)
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD15_13_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD15_13_MSK                          (0x0000E000)
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD15_13_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD15_13_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD15_13_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD15_13_HSH                          (0x031AB3FC)

  #define MTG0_CR_MTG_CH0_DQEC7_DQ30_ERR_CNT_OFF                       (16)
  #define MTG0_CR_MTG_CH0_DQEC7_DQ30_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC7_DQ30_ERR_CNT_MSK                       (0x001F0000)
  #define MTG0_CR_MTG_CH0_DQEC7_DQ30_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC7_DQ30_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC7_DQ30_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC7_DQ30_ERR_CNT_HSH                       (0x05A0B3FC)

  #define MTG0_CR_MTG_CH0_DQEC7_RSVD23_21_OFF                          (21)
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD23_21_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD23_21_MSK                          (0x00E00000)
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD23_21_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD23_21_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD23_21_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD23_21_HSH                          (0x032AB3FC)

  #define MTG0_CR_MTG_CH0_DQEC7_DQ31_ERR_CNT_OFF                       (24)
  #define MTG0_CR_MTG_CH0_DQEC7_DQ31_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC7_DQ31_ERR_CNT_MSK                       (0x1F000000)
  #define MTG0_CR_MTG_CH0_DQEC7_DQ31_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC7_DQ31_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC7_DQ31_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC7_DQ31_ERR_CNT_HSH                       (0x05B0B3FC)

  #define MTG0_CR_MTG_CH0_DQEC7_RSVD31_29_OFF                          (29)
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD31_29_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD31_29_MSK                          (0xE0000000)
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD31_29_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD31_29_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD31_29_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC7_RSVD31_29_HSH                          (0x033AB3FC)

#define MTG0_CR_MTG_CH0_DQEC8_REG                                      (0x0000B400)
#define MTG0_CR_MTG_CH0_DQEC8_DEF                                      (0x00000000)
#define MTG0_CR_MTG_CH0_DQEC8_VOLATILE_BITFIELDS_MSK                   (0x1F1F1F1F)

  #define MTG0_CR_MTG_CH0_DQEC8_DQ32_ERR_CNT_OFF                       ( 0)
  #define MTG0_CR_MTG_CH0_DQEC8_DQ32_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC8_DQ32_ERR_CNT_MSK                       (0x0000001F)
  #define MTG0_CR_MTG_CH0_DQEC8_DQ32_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC8_DQ32_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC8_DQ32_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC8_DQ32_ERR_CNT_HSH                       (0x0580B400)

  #define MTG0_CR_MTG_CH0_DQEC8_RSVD7_5_OFF                            ( 5)
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD7_5_WID                            ( 3)
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD7_5_MSK                            (0x000000E0)
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD7_5_MIN                            (0)
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD7_5_MAX                            (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD7_5_DEF                            (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD7_5_HSH                            (0x030AB400)

  #define MTG0_CR_MTG_CH0_DQEC8_DQ33_ERR_CNT_OFF                       ( 8)
  #define MTG0_CR_MTG_CH0_DQEC8_DQ33_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC8_DQ33_ERR_CNT_MSK                       (0x00001F00)
  #define MTG0_CR_MTG_CH0_DQEC8_DQ33_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC8_DQ33_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC8_DQ33_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC8_DQ33_ERR_CNT_HSH                       (0x0590B400)

  #define MTG0_CR_MTG_CH0_DQEC8_RSVD15_13_OFF                          (13)
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD15_13_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD15_13_MSK                          (0x0000E000)
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD15_13_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD15_13_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD15_13_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD15_13_HSH                          (0x031AB400)

  #define MTG0_CR_MTG_CH0_DQEC8_DQ34_ERR_CNT_OFF                       (16)
  #define MTG0_CR_MTG_CH0_DQEC8_DQ34_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC8_DQ34_ERR_CNT_MSK                       (0x001F0000)
  #define MTG0_CR_MTG_CH0_DQEC8_DQ34_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC8_DQ34_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC8_DQ34_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC8_DQ34_ERR_CNT_HSH                       (0x05A0B400)

  #define MTG0_CR_MTG_CH0_DQEC8_RSVD23_21_OFF                          (21)
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD23_21_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD23_21_MSK                          (0x00E00000)
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD23_21_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD23_21_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD23_21_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD23_21_HSH                          (0x032AB400)

  #define MTG0_CR_MTG_CH0_DQEC8_DQ35_ERR_CNT_OFF                       (24)
  #define MTG0_CR_MTG_CH0_DQEC8_DQ35_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC8_DQ35_ERR_CNT_MSK                       (0x1F000000)
  #define MTG0_CR_MTG_CH0_DQEC8_DQ35_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC8_DQ35_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC8_DQ35_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC8_DQ35_ERR_CNT_HSH                       (0x05B0B400)

  #define MTG0_CR_MTG_CH0_DQEC8_RSVD31_29_OFF                          (29)
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD31_29_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD31_29_MSK                          (0xE0000000)
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD31_29_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD31_29_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD31_29_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC8_RSVD31_29_HSH                          (0x033AB400)

#define MTG0_CR_MTG_CH0_DQEC9_REG                                      (0x0000B404)
#define MTG0_CR_MTG_CH0_DQEC9_DEF                                      (0x00000000)
#define MTG0_CR_MTG_CH0_DQEC9_VOLATILE_BITFIELDS_MSK                   (0x1F1F1F1F)

  #define MTG0_CR_MTG_CH0_DQEC9_DQ36_ERR_CNT_OFF                       ( 0)
  #define MTG0_CR_MTG_CH0_DQEC9_DQ36_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC9_DQ36_ERR_CNT_MSK                       (0x0000001F)
  #define MTG0_CR_MTG_CH0_DQEC9_DQ36_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC9_DQ36_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC9_DQ36_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC9_DQ36_ERR_CNT_HSH                       (0x0580B404)

  #define MTG0_CR_MTG_CH0_DQEC9_RSVD7_5_OFF                            ( 5)
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD7_5_WID                            ( 3)
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD7_5_MSK                            (0x000000E0)
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD7_5_MIN                            (0)
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD7_5_MAX                            (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD7_5_DEF                            (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD7_5_HSH                            (0x030AB404)

  #define MTG0_CR_MTG_CH0_DQEC9_DQ37_ERR_CNT_OFF                       ( 8)
  #define MTG0_CR_MTG_CH0_DQEC9_DQ37_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC9_DQ37_ERR_CNT_MSK                       (0x00001F00)
  #define MTG0_CR_MTG_CH0_DQEC9_DQ37_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC9_DQ37_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC9_DQ37_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC9_DQ37_ERR_CNT_HSH                       (0x0590B404)

  #define MTG0_CR_MTG_CH0_DQEC9_RSVD15_13_OFF                          (13)
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD15_13_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD15_13_MSK                          (0x0000E000)
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD15_13_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD15_13_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD15_13_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD15_13_HSH                          (0x031AB404)

  #define MTG0_CR_MTG_CH0_DQEC9_DQ38_ERR_CNT_OFF                       (16)
  #define MTG0_CR_MTG_CH0_DQEC9_DQ38_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC9_DQ38_ERR_CNT_MSK                       (0x001F0000)
  #define MTG0_CR_MTG_CH0_DQEC9_DQ38_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC9_DQ38_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC9_DQ38_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC9_DQ38_ERR_CNT_HSH                       (0x05A0B404)

  #define MTG0_CR_MTG_CH0_DQEC9_RSVD23_21_OFF                          (21)
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD23_21_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD23_21_MSK                          (0x00E00000)
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD23_21_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD23_21_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD23_21_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD23_21_HSH                          (0x032AB404)

  #define MTG0_CR_MTG_CH0_DQEC9_DQ39_ERR_CNT_OFF                       (24)
  #define MTG0_CR_MTG_CH0_DQEC9_DQ39_ERR_CNT_WID                       ( 5)
  #define MTG0_CR_MTG_CH0_DQEC9_DQ39_ERR_CNT_MSK                       (0x1F000000)
  #define MTG0_CR_MTG_CH0_DQEC9_DQ39_ERR_CNT_MIN                       (0)
  #define MTG0_CR_MTG_CH0_DQEC9_DQ39_ERR_CNT_MAX                       (31) // 0x0000001F
  #define MTG0_CR_MTG_CH0_DQEC9_DQ39_ERR_CNT_DEF                       (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC9_DQ39_ERR_CNT_HSH                       (0x05B0B404)

  #define MTG0_CR_MTG_CH0_DQEC9_RSVD31_29_OFF                          (29)
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD31_29_WID                          ( 3)
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD31_29_MSK                          (0xE0000000)
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD31_29_MIN                          (0)
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD31_29_MAX                          (7) // 0x00000007
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD31_29_DEF                          (0x00000000)
  #define MTG0_CR_MTG_CH0_DQEC9_RSVD31_29_HSH                          (0x033AB404)

#define MTG0_CR_MTG_CH1_ERRCFG_REG                                     (0x0000B480)
//Duplicate of MTG0_CR_MTG_CH0_ERRCFG_REG

#define MTG0_CR_MTG_CH1_ERRCNT0_REG                                    (0x0000B484)
//Duplicate of MTG0_CR_MTG_CH0_ERRCNT0_REG

#define MTG0_CR_MTG_CH1_ERRCNT1_REG                                    (0x0000B488)
//Duplicate of MTG0_CR_MTG_CH0_ERRCNT1_REG

#define MTG0_CR_MTG_CH1_BEM0_REG                                       (0x0000B490)
//Duplicate of MTG0_CR_MTG_CH0_BEM0_REG

#define MTG0_CR_MTG_CH1_BEM1_REG                                       (0x0000B494)
//Duplicate of MTG0_CR_MTG_CH0_BEM1_REG

#define MTG0_CR_MTG_CH1_BEM2_REG                                       (0x0000B498)
//Duplicate of MTG0_CR_MTG_CH0_BEM2_REG

#define MTG0_CR_MTG_CH1_UIEM_REG                                       (0x0000B49C)
//Duplicate of MTG0_CR_MTG_CH0_UIEM_REG

#define MTG0_CR_MTG_CH1_LEM0_REG                                       (0x0000B4A0)
//Duplicate of MTG0_CR_MTG_CH0_LEM0_REG

#define MTG0_CR_MTG_CH1_LEM1_REG                                       (0x0000B4A4)
//Duplicate of MTG0_CR_MTG_CH0_LEM1_REG

#define MTG0_CR_MTG_CH1_CLEM_REG                                       (0x0000B4A8)
//Duplicate of MTG0_CR_MTG_CH0_CLEM_REG

#define MTG0_CR_MTG_CH1_BYTE_ERR0_REG                                  (0x0000B4B0)
//Duplicate of MTG0_CR_MTG_CH0_BYTE_ERR0_REG

#define MTG0_CR_MTG_CH1_BYTE_ERR1_REG                                  (0x0000B4B4)
//Duplicate of MTG0_CR_MTG_CH0_BYTE_ERR1_REG

#define MTG0_CR_MTG_CH1_BYTE_ERR2_REG                                  (0x0000B4B8)
//Duplicate of MTG0_CR_MTG_CH0_BYTE_ERR2_REG

#define MTG0_CR_MTG_CH1_UI_ERR_REG                                     (0x0000B4BC)
//Duplicate of MTG0_CR_MTG_CH0_UI_ERR_REG

#define MTG0_CR_MTG_CH1_EUILANE_ERR0_REG                               (0x0000B4C0)
//Duplicate of MTG0_CR_MTG_CH0_EUILANE_ERR0_REG

#define MTG0_CR_MTG_CH1_EUILANE_ERR1_REG                               (0x0000B4C4)
//Duplicate of MTG0_CR_MTG_CH0_EUILANE_ERR1_REG

#define MTG0_CR_MTG_CH1_OUILANE_ERR0_REG                               (0x0000B4C8)
//Duplicate of MTG0_CR_MTG_CH0_OUILANE_ERR0_REG

#define MTG0_CR_MTG_CH1_OUILANE_ERR1_REG                               (0x0000B4CC)
//Duplicate of MTG0_CR_MTG_CH0_OUILANE_ERR1_REG

#define MTG0_CR_MTG_CH1_BLEC0_REG                                      (0x0000B4D0)
//Duplicate of MTG0_CR_MTG_CH0_BLEC0_REG

#define MTG0_CR_MTG_CH1_BLEC1_REG                                      (0x0000B4D4)
//Duplicate of MTG0_CR_MTG_CH0_BLEC1_REG

#define MTG0_CR_MTG_CH1_BLEC2_REG                                      (0x0000B4D8)
//Duplicate of MTG0_CR_MTG_CH0_BLEC2_REG

#define MTG0_CR_MTG_CH1_DQEC0_REG                                      (0x0000B4E0)
//Duplicate of MTG0_CR_MTG_CH0_DQEC0_REG

#define MTG0_CR_MTG_CH1_DQEC1_REG                                      (0x0000B4E4)
//Duplicate of MTG0_CR_MTG_CH0_DQEC1_REG

#define MTG0_CR_MTG_CH1_DQEC2_REG                                      (0x0000B4E8)
//Duplicate of MTG0_CR_MTG_CH0_DQEC2_REG

#define MTG0_CR_MTG_CH1_DQEC3_REG                                      (0x0000B4EC)
//Duplicate of MTG0_CR_MTG_CH0_DQEC3_REG

#define MTG0_CR_MTG_CH1_DQEC4_REG                                      (0x0000B4F0)
//Duplicate of MTG0_CR_MTG_CH0_DQEC4_REG

#define MTG0_CR_MTG_CH1_DQEC5_REG                                      (0x0000B4F4)
//Duplicate of MTG0_CR_MTG_CH0_DQEC5_REG

#define MTG0_CR_MTG_CH1_DQEC6_REG                                      (0x0000B4F8)
//Duplicate of MTG0_CR_MTG_CH0_DQEC6_REG

#define MTG0_CR_MTG_CH1_DQEC7_REG                                      (0x0000B4FC)
//Duplicate of MTG0_CR_MTG_CH0_DQEC7_REG

#define MTG0_CR_MTG_CH1_DQEC8_REG                                      (0x0000B500)
//Duplicate of MTG0_CR_MTG_CH0_DQEC8_REG

#define MTG0_CR_MTG_CH1_DQEC9_REG                                      (0x0000B504)
//Duplicate of MTG0_CR_MTG_CH0_DQEC9_REG

#define MTG0_CR_MTG_CH2_ERRCFG_REG                                     (0x0000B580)
//Duplicate of MTG0_CR_MTG_CH0_ERRCFG_REG

#define MTG0_CR_MTG_CH2_ERRCNT0_REG                                    (0x0000B584)
//Duplicate of MTG0_CR_MTG_CH0_ERRCNT0_REG

#define MTG0_CR_MTG_CH2_ERRCNT1_REG                                    (0x0000B588)
//Duplicate of MTG0_CR_MTG_CH0_ERRCNT1_REG

#define MTG0_CR_MTG_CH2_BEM0_REG                                       (0x0000B590)
//Duplicate of MTG0_CR_MTG_CH0_BEM0_REG

#define MTG0_CR_MTG_CH2_BEM1_REG                                       (0x0000B594)
//Duplicate of MTG0_CR_MTG_CH0_BEM1_REG

#define MTG0_CR_MTG_CH2_BEM2_REG                                       (0x0000B598)
//Duplicate of MTG0_CR_MTG_CH0_BEM2_REG

#define MTG0_CR_MTG_CH2_UIEM_REG                                       (0x0000B59C)
//Duplicate of MTG0_CR_MTG_CH0_UIEM_REG

#define MTG0_CR_MTG_CH2_LEM0_REG                                       (0x0000B5A0)
//Duplicate of MTG0_CR_MTG_CH0_LEM0_REG

#define MTG0_CR_MTG_CH2_LEM1_REG                                       (0x0000B5A4)
//Duplicate of MTG0_CR_MTG_CH0_LEM1_REG

#define MTG0_CR_MTG_CH2_CLEM_REG                                       (0x0000B5A8)
//Duplicate of MTG0_CR_MTG_CH0_CLEM_REG

#define MTG0_CR_MTG_CH2_BYTE_ERR0_REG                                  (0x0000B5B0)
//Duplicate of MTG0_CR_MTG_CH0_BYTE_ERR0_REG

#define MTG0_CR_MTG_CH2_BYTE_ERR1_REG                                  (0x0000B5B4)
//Duplicate of MTG0_CR_MTG_CH0_BYTE_ERR1_REG

#define MTG0_CR_MTG_CH2_BYTE_ERR2_REG                                  (0x0000B5B8)
//Duplicate of MTG0_CR_MTG_CH0_BYTE_ERR2_REG

#define MTG0_CR_MTG_CH2_UI_ERR_REG                                     (0x0000B5BC)
//Duplicate of MTG0_CR_MTG_CH0_UI_ERR_REG

#define MTG0_CR_MTG_CH2_EUILANE_ERR0_REG                               (0x0000B5C0)
//Duplicate of MTG0_CR_MTG_CH0_EUILANE_ERR0_REG

#define MTG0_CR_MTG_CH2_EUILANE_ERR1_REG                               (0x0000B5C4)
//Duplicate of MTG0_CR_MTG_CH0_EUILANE_ERR1_REG

#define MTG0_CR_MTG_CH2_OUILANE_ERR0_REG                               (0x0000B5C8)
//Duplicate of MTG0_CR_MTG_CH0_OUILANE_ERR0_REG

#define MTG0_CR_MTG_CH2_OUILANE_ERR1_REG                               (0x0000B5CC)
//Duplicate of MTG0_CR_MTG_CH0_OUILANE_ERR1_REG

#define MTG0_CR_MTG_CH2_BLEC0_REG                                      (0x0000B5D0)
//Duplicate of MTG0_CR_MTG_CH0_BLEC0_REG

#define MTG0_CR_MTG_CH2_BLEC1_REG                                      (0x0000B5D4)
//Duplicate of MTG0_CR_MTG_CH0_BLEC1_REG

#define MTG0_CR_MTG_CH2_BLEC2_REG                                      (0x0000B5D8)
//Duplicate of MTG0_CR_MTG_CH0_BLEC2_REG

#define MTG0_CR_MTG_CH2_DQEC0_REG                                      (0x0000B5E0)
//Duplicate of MTG0_CR_MTG_CH0_DQEC0_REG

#define MTG0_CR_MTG_CH2_DQEC1_REG                                      (0x0000B5E4)
//Duplicate of MTG0_CR_MTG_CH0_DQEC1_REG

#define MTG0_CR_MTG_CH2_DQEC2_REG                                      (0x0000B5E8)
//Duplicate of MTG0_CR_MTG_CH0_DQEC2_REG

#define MTG0_CR_MTG_CH2_DQEC3_REG                                      (0x0000B5EC)
//Duplicate of MTG0_CR_MTG_CH0_DQEC3_REG

#define MTG0_CR_MTG_CH2_DQEC4_REG                                      (0x0000B5F0)
//Duplicate of MTG0_CR_MTG_CH0_DQEC4_REG

#define MTG0_CR_MTG_CH2_DQEC5_REG                                      (0x0000B5F4)
//Duplicate of MTG0_CR_MTG_CH0_DQEC5_REG

#define MTG0_CR_MTG_CH2_DQEC6_REG                                      (0x0000B5F8)
//Duplicate of MTG0_CR_MTG_CH0_DQEC6_REG

#define MTG0_CR_MTG_CH2_DQEC7_REG                                      (0x0000B5FC)
//Duplicate of MTG0_CR_MTG_CH0_DQEC7_REG

#define MTG0_CR_MTG_CH2_DQEC8_REG                                      (0x0000B600)
//Duplicate of MTG0_CR_MTG_CH0_DQEC8_REG

#define MTG0_CR_MTG_CH2_DQEC9_REG                                      (0x0000B604)
//Duplicate of MTG0_CR_MTG_CH0_DQEC9_REG

#define MTG0_CR_MTG_CH3_ERRCFG_REG                                     (0x0000B680)
//Duplicate of MTG0_CR_MTG_CH0_ERRCFG_REG

#define MTG0_CR_MTG_CH3_ERRCNT0_REG                                    (0x0000B684)
//Duplicate of MTG0_CR_MTG_CH0_ERRCNT0_REG

#define MTG0_CR_MTG_CH3_ERRCNT1_REG                                    (0x0000B688)
//Duplicate of MTG0_CR_MTG_CH0_ERRCNT1_REG

#define MTG0_CR_MTG_CH3_BEM0_REG                                       (0x0000B690)
//Duplicate of MTG0_CR_MTG_CH0_BEM0_REG

#define MTG0_CR_MTG_CH3_BEM1_REG                                       (0x0000B694)
//Duplicate of MTG0_CR_MTG_CH0_BEM1_REG

#define MTG0_CR_MTG_CH3_BEM2_REG                                       (0x0000B698)
//Duplicate of MTG0_CR_MTG_CH0_BEM2_REG

#define MTG0_CR_MTG_CH3_UIEM_REG                                       (0x0000B69C)
//Duplicate of MTG0_CR_MTG_CH0_UIEM_REG

#define MTG0_CR_MTG_CH3_LEM0_REG                                       (0x0000B6A0)
//Duplicate of MTG0_CR_MTG_CH0_LEM0_REG

#define MTG0_CR_MTG_CH3_LEM1_REG                                       (0x0000B6A4)
//Duplicate of MTG0_CR_MTG_CH0_LEM1_REG

#define MTG0_CR_MTG_CH3_CLEM_REG                                       (0x0000B6A8)
//Duplicate of MTG0_CR_MTG_CH0_CLEM_REG

#define MTG0_CR_MTG_CH3_BYTE_ERR0_REG                                  (0x0000B6B0)
//Duplicate of MTG0_CR_MTG_CH0_BYTE_ERR0_REG

#define MTG0_CR_MTG_CH3_BYTE_ERR1_REG                                  (0x0000B6B4)
//Duplicate of MTG0_CR_MTG_CH0_BYTE_ERR1_REG

#define MTG0_CR_MTG_CH3_BYTE_ERR2_REG                                  (0x0000B6B8)
//Duplicate of MTG0_CR_MTG_CH0_BYTE_ERR2_REG

#define MTG0_CR_MTG_CH3_UI_ERR_REG                                     (0x0000B6BC)
//Duplicate of MTG0_CR_MTG_CH0_UI_ERR_REG

#define MTG0_CR_MTG_CH3_EUILANE_ERR0_REG                               (0x0000B6C0)
//Duplicate of MTG0_CR_MTG_CH0_EUILANE_ERR0_REG

#define MTG0_CR_MTG_CH3_EUILANE_ERR1_REG                               (0x0000B6C4)
//Duplicate of MTG0_CR_MTG_CH0_EUILANE_ERR1_REG

#define MTG0_CR_MTG_CH3_OUILANE_ERR0_REG                               (0x0000B6C8)
//Duplicate of MTG0_CR_MTG_CH0_OUILANE_ERR0_REG

#define MTG0_CR_MTG_CH3_OUILANE_ERR1_REG                               (0x0000B6CC)
//Duplicate of MTG0_CR_MTG_CH0_OUILANE_ERR1_REG

#define MTG0_CR_MTG_CH3_BLEC0_REG                                      (0x0000B6D0)
//Duplicate of MTG0_CR_MTG_CH0_BLEC0_REG

#define MTG0_CR_MTG_CH3_BLEC1_REG                                      (0x0000B6D4)
//Duplicate of MTG0_CR_MTG_CH0_BLEC1_REG

#define MTG0_CR_MTG_CH3_BLEC2_REG                                      (0x0000B6D8)
//Duplicate of MTG0_CR_MTG_CH0_BLEC2_REG

#define MTG0_CR_MTG_CH3_DQEC0_REG                                      (0x0000B6E0)
//Duplicate of MTG0_CR_MTG_CH0_DQEC0_REG

#define MTG0_CR_MTG_CH3_DQEC1_REG                                      (0x0000B6E4)
//Duplicate of MTG0_CR_MTG_CH0_DQEC1_REG

#define MTG0_CR_MTG_CH3_DQEC2_REG                                      (0x0000B6E8)
//Duplicate of MTG0_CR_MTG_CH0_DQEC2_REG

#define MTG0_CR_MTG_CH3_DQEC3_REG                                      (0x0000B6EC)
//Duplicate of MTG0_CR_MTG_CH0_DQEC3_REG

#define MTG0_CR_MTG_CH3_DQEC4_REG                                      (0x0000B6F0)
//Duplicate of MTG0_CR_MTG_CH0_DQEC4_REG

#define MTG0_CR_MTG_CH3_DQEC5_REG                                      (0x0000B6F4)
//Duplicate of MTG0_CR_MTG_CH0_DQEC5_REG

#define MTG0_CR_MTG_CH3_DQEC6_REG                                      (0x0000B6F8)
//Duplicate of MTG0_CR_MTG_CH0_DQEC6_REG

#define MTG0_CR_MTG_CH3_DQEC7_REG                                      (0x0000B6FC)
//Duplicate of MTG0_CR_MTG_CH0_DQEC7_REG

#define MTG0_CR_MTG_CH3_DQEC8_REG                                      (0x0000B700)
//Duplicate of MTG0_CR_MTG_CH0_DQEC8_REG

#define MTG0_CR_MTG_CH3_DQEC9_REG                                      (0x0000B704)
//Duplicate of MTG0_CR_MTG_CH0_DQEC9_REG

#define DUNIT1_CR_D_CR_MEMCFG_REG                                      (0x0000B780)
//Duplicate of DUNIT0_CR_MEMCFG_REG

#define DUNIT1_CR_D_CR_DTR0_REG                                        (0x0000B790)
//Duplicate of DUNIT0_CR_DTR0_REG

#define DUNIT1_CR_D_CR_DTR1_REG                                        (0x0000B794)
//Duplicate of DUNIT0_CR_DTR1_REG

#define DUNIT1_CR_D_CR_DTR2_REG                                        (0x0000B798)
//Duplicate of DUNIT0_CR_DTR2_REG

#define DUNIT1_CR_D_CR_DTR3_REG                                        (0x0000B79C)
//Duplicate of DUNIT0_CR_DTR3_REG

#define DUNIT1_CR_D_CR_DTR4_REG                                        (0x0000B7A0)
//Duplicate of DUNIT0_CR_DTR4_REG

#define DUNIT1_CR_D_CR_DTR5_REG                                        (0x0000B7A4)
//Duplicate of DUNIT0_CR_DTR5_REG

#define DUNIT1_CR_D_CR_DTR6_REG                                        (0x0000B7A8)
//Duplicate of DUNIT0_CR_DTR6_REG

#define DUNIT1_CR_D_CR_DTR7_REG                                        (0x0000B7AC)
//Duplicate of DUNIT0_CR_DTR7_REG

#define DUNIT1_CR_D_CR_DTR8_REG                                        (0x0000B7B0)
//Duplicate of DUNIT0_CR_DTR8_REG

#define DUNIT1_CR_D_CR_DTR9_REG                                        (0x0000B7B4)
//Duplicate of DUNIT0_CR_DTR9_REG

#define DUNIT1_CR_D_CR_DTR10_REG                                       (0x0000B7B8)
//Duplicate of DUNIT0_CR_DTR10_REG

#define DUNIT1_CR_D_CR_DTR11_REG                                       (0x0000B7BC)
//Duplicate of DUNIT0_CR_DTR11_REG

#define DUNIT1_CR_D_CR_DTR12_REG                                       (0x0000B7C0)
//Duplicate of DUNIT0_CR_DTR12_REG

#define DUNIT1_CR_D_CR_DTR13_REG                                       (0x0000B7C4)
//Duplicate of DUNIT0_CR_DTR13_REG

#define DUNIT1_CR_D_CR_DTR14_REG                                       (0x0000B7C8)
//Duplicate of DUNIT0_CR_DTR14_REG

#define DUNIT1_CR_D_CR_DTR15_REG                                       (0x0000B7CC)
//Duplicate of DUNIT0_CR_DTR15_REG

#define DUNIT1_CR_D_CR_DTR16_REG                                       (0x0000B7D0)
//Duplicate of DUNIT0_CR_DTR16_REG

#define DUNIT1_CR_D_CR_DTR17_REG                                       (0x0000B7D4)
//Duplicate of DUNIT0_CR_DTR17_REG

#define DUNIT1_CR_D_CR_ODT1_REG                                        (0x0000B7E4)
//Duplicate of DUNIT0_CR_ODT1_REG

#define DUNIT1_CR_D_CR_PD0_REG                                         (0x0000B7F0)
//Duplicate of DUNIT0_CR_PD0_REG

#define DUNIT1_CR_D_CR_PD2_REG                                         (0x0000B7F8)
//Duplicate of DUNIT0_CR_PD2_REG

#define DUNIT1_CR_D_CR_SR0_REG                                         (0x0000B800)
//Duplicate of DUNIT0_CR_D_CR_SR0_REG

#define DUNIT1_CR_D_CR_LPCTRL0_REG                                     (0x0000B804)
//Duplicate of DUNIT0_CR_D_CR_LPCTRL0_REG

#define DUNIT1_CR_D_CR_LPCTRL1_REG                                     (0x0000B808)
//Duplicate of DUNIT0_CR_D_CR_LPCTRL1_REG

#define DUNIT1_CR_D_CR_REF_REG                                         (0x0000B80C)
//Duplicate of DUNIT0_CR_D_CR_REF_REG

#define DUNIT1_CR_D_CR_ZQCAL_REG                                       (0x0000B810)
//Duplicate of DUNIT0_CR_D_CR_ZQCAL_REG

#define DUNIT1_CR_D_CR_DQS2DQ_CAL_REG                                  (0x0000B814)
//Duplicate of DUNIT0_CR_D_CR_DQS2DQ_CAL_REG

#define DUNIT1_CR_D_CR_WCK2DQO_CAL_REG                                 (0x0000B818)
//Duplicate of DUNIT0_CR_D_CR_WCK2DQO_CAL_REG

#define DUNIT1_CR_D_CR_TRAIN_REG                                       (0x0000B820)
//Duplicate of DUNIT0_CR_D_CR_TRAIN_REG

#define DUNIT1_CR_D_CR_TRAIN_FB_DATA0_REG                              (0x0000B824)
//Duplicate of DUNIT0_CR_D_CR_TRAIN_FB_DATA0_REG

#define DUNIT1_CR_D_CR_TRAIN_FB_DATA1_REG                              (0x0000B828)
//Duplicate of DUNIT0_CR_D_CR_TRAIN_FB_DATA1_REG

#define DUNIT1_CR_D_CR_MCTRL_REG                                       (0x0000B82C)
//Duplicate of DUNIT0_CR_D_CR_MCTRL_REG

#define DUNIT1_CR_D_CR_DRAM_CMDS_REG                                   (0x0000B830)
//Duplicate of DUNIT0_CR_D_CR_DRAM_CMDS_REG

#define DUNIT1_CR_D_CR_DRAM_CMDS1_REG                                  (0x0000B834)
//Duplicate of DUNIT0_CR_D_CR_DRAM_CMDS1_REG

#define DUNIT1_CR_D_CR_DRAM_CMD_REG                                    (0x0000B83C)
//Duplicate of DUNIT0_CR_D_CR_DRAM_CMD_REG

#define DUNIT1_CR_D_CR_PS_DRAM_CMD_REG                                 (0x0000B840)
//Duplicate of DUNIT0_CR_D_CR_PS_DRAM_CMD_REG

#define DUNIT1_CR_D_CR_MRRD0_REG                                       (0x0000B880)
//Duplicate of DUNIT0_CR_D_CR_MRRD0_REG

#define DUNIT1_CR_D_CR_MRRD1_REG                                       (0x0000B884)
//Duplicate of DUNIT0_CR_D_CR_MRRD1_REG

#define DUNIT1_CR_D_CR_MRRD2_REG                                       (0x0000B888)
//Duplicate of DUNIT0_CR_D_CR_MRRD2_REG

#define DUNIT1_CR_D_CR_DOC_REG                                         (0x0000B890)
//Duplicate of DUNIT0_CR_D_CR_DOC_REG

#define DUNIT1_CR_D_CR_STATUS_REG                                      (0x0000B894)
//Duplicate of DUNIT0_CR_D_CR_STATUS_REG

#define DUNIT1_CR_D_CR_BANK_STATUS0_REG                                (0x0000B8A0)
//Duplicate of DUNIT0_CR_D_CR_BANK_STATUS0_REG

#define DUNIT1_CR_D_CR_BANK_STATUS1_REG                                (0x0000B8A4)
//Duplicate of DUNIT0_CR_D_CR_BANK_STATUS1_REG

#define DUNIT1_CR_D_CR_BANK_STATUS2_REG                                (0x0000B8A8)
//Duplicate of DUNIT0_CR_D_CR_BANK_STATUS2_REG

#define DUNIT1_CR_D_CR_BANK_STATUS3_REG                                (0x0000B8AC)
//Duplicate of DUNIT0_CR_D_CR_BANK_STATUS3_REG

#define DUNIT1_CR_D_CR_MISC0_REG                                       (0x0000B8B4)
//Duplicate of DUNIT0_CR_D_CR_MISC0_REG

#define DUNIT1_CR_D_CR_BONUS0_REG                                      (0x0000B8C0)
//Duplicate of DUNIT0_CR_D_CR_BONUS0_REG

#define DUNIT1_CR_D_CR_BONUS1_REG                                      (0x0000B8C4)
//Duplicate of DUNIT0_CR_D_CR_BONUS1_REG

#define DUNIT1_CR_D_CR_DGB_REG                                         (0x0000B8C8)
//Duplicate of DUNIT0_CR_D_CR_DGB_REG

#define DUNIT1_CR_CATG_CTRL0_REG                                       (0x0000B910)
//Duplicate of DUNIT0_CR_CATG_CTRL0_REG

#define DUNIT1_CR_CATG_CTRL1_REG                                       (0x0000B914)
//Duplicate of DUNIT0_CR_CATG_CTRL1_REG

#define DUNIT1_CR_CATG_CTRL2_REG                                       (0x0000B918)
//Duplicate of DUNIT0_CR_CATG_CTRL2_REG

#define DUNIT1_CR_CATG_CAPPATSEL_REG                                   (0x0000B91C)
//Duplicate of DUNIT0_CR_CATG_CAPPATSEL_REG

#define DUNIT1_CR_CATG_CAPATSEL_REG                                    (0x0000B920)
//Duplicate of DUNIT0_CR_CATG_CAPATSEL_REG

#define DUNIT1_CR_CATG_CSPATSEL_REG                                    (0x0000B924)
//Duplicate of DUNIT0_CR_CATG_CSPATSEL_REG

#define DUNIT1_CR_CATG_SSP0_0_REG                                      (0x0000B928)
//Duplicate of DUNIT0_CR_CATG_SSP0_0_REG

#define DUNIT1_CR_CATG_SSP0_1_REG                                      (0x0000B92C)
//Duplicate of DUNIT0_CR_CATG_SSP0_1_REG

#define DUNIT1_CR_CATG_SSP1_0_REG                                      (0x0000B930)
//Duplicate of DUNIT0_CR_CATG_SSP1_0_REG

#define DUNIT1_CR_CATG_SSP1_1_REG                                      (0x0000B934)
//Duplicate of DUNIT0_CR_CATG_SSP1_1_REG

#define DUNIT1_CR_CATG_SSP2_0_REG                                      (0x0000B938)
//Duplicate of DUNIT0_CR_CATG_SSP2_0_REG

#define DUNIT1_CR_CATG_SSP2_1_REG                                      (0x0000B93C)
//Duplicate of DUNIT0_CR_CATG_SSP2_1_REG

#define DUNIT1_CR_CATG_SSP3_0_REG                                      (0x0000B940)
//Duplicate of DUNIT0_CR_CATG_SSP3_0_REG

#define DUNIT1_CR_CATG_SSP3_1_REG                                      (0x0000B944)
//Duplicate of DUNIT0_CR_CATG_SSP3_1_REG

#define DUNIT1_CR_CATG_SSPL_REG                                        (0x0000B948)
//Duplicate of DUNIT0_CR_CATG_SSPL_REG

#define DUNIT1_CR_CATG_LFSR_POLY_REG                                   (0x0000B94C)
//Duplicate of DUNIT0_CR_CATG_LFSR_POLY_REG

#define DUNIT1_CR_CATG_LFSR0_SEED_REG                                  (0x0000B950)
//Duplicate of DUNIT0_CR_CATG_LFSR0_SEED_REG

#define DUNIT1_CR_CATG_LFSR1_SEED_REG                                  (0x0000B954)
//Duplicate of DUNIT0_CR_CATG_LFSR1_SEED_REG

#define DUNIT1_CR_CATG_CA_DCINVC_REG                                   (0x0000B958)
//Duplicate of DUNIT0_CR_CATG_CA_DCINVC_REG

#define DUNIT1_CR_CATG_CS_DCINVC_REG                                   (0x0000B95C)
//Duplicate of DUNIT0_CR_CATG_CS_DCINVC_REG

#define MTG1_CR_MTG_MAIN_CTRL_REG                                      (0x0000B980)
//Duplicate of MTG0_CR_MTG_MAIN_CTRL_REG

#define MTG1_CR_MTG_CE0_REG                                            (0x0000B990)
//Duplicate of MTG0_CR_MTG_CE0_REG

#define MTG1_CR_MTG_CE1_REG                                            (0x0000B994)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG1_CR_MTG_CE2_REG                                            (0x0000B998)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG1_CR_MTG_CE3_REG                                            (0x0000B99C)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG1_CR_MTG_CE4_REG                                            (0x0000B9A0)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG1_CR_MTG_CE5_REG                                            (0x0000B9A4)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG1_CR_MTG_CE6_REG                                            (0x0000B9A8)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG1_CR_MTG_CE7_REG                                            (0x0000B9AC)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG1_CR_MTG_CE8_REG                                            (0x0000B9B0)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG1_CR_MTG_CE9_REG                                            (0x0000B9B4)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG1_CR_MTG_CE10_REG                                           (0x0000B9B8)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG1_CR_MTG_CE11_REG                                           (0x0000B9BC)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG1_CR_MTG_CE12_REG                                           (0x0000B9C0)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG1_CR_MTG_CE13_REG                                           (0x0000B9C4)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG1_CR_MTG_CE14_REG                                           (0x0000B9C8)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG1_CR_MTG_CE15_REG                                           (0x0000B9CC)
//Duplicate of MTG0_CR_MTG_CE1_REG

#define MTG1_CR_MTG_CDLY_REG                                           (0x0000B9E0)
//Duplicate of MTG0_CR_MTG_CDLY_REG

#define MTG1_CR_MTG_CLC_REG                                            (0x0000B9E4)
//Duplicate of MTG0_CR_MTG_CLC_REG

#define MTG1_CR_MTG_CRANDC_REG                                         (0x0000B9E8)
//Duplicate of MTG0_CR_MTG_CRANDC_REG

#define MTG1_CR_MTG_MWRC_REG                                           (0x0000B9EC)
//Duplicate of MTG0_CR_MTG_MWRC_REG

#define MTG1_CR_MTG_MWRO_REG                                           (0x0000B9F0)
//Duplicate of MTG0_CR_MTG_MWRO_REG

#define MTG1_CR_MTG_ARMIN0_REG                                         (0x0000BA00)
//Duplicate of MTG0_CR_MTG_ARMIN0_REG

#define MTG1_CR_MTG_ARMIN1_REG                                         (0x0000BA04)
//Duplicate of MTG0_CR_MTG_ARMIN1_REG

#define MTG1_CR_MTG_ARMAX0_REG                                         (0x0000BA08)
//Duplicate of MTG0_CR_MTG_ARMAX0_REG

#define MTG1_CR_MTG_ARMAX1_REG                                         (0x0000BA0C)
//Duplicate of MTG0_CR_MTG_ARMAX1_REG

#define MTG1_CR_MTG_L2PRM_REG                                          (0x0000BA10)
//Duplicate of MTG0_CR_MTG_L2PRM_REG

#define MTG1_CR_MTG_AO_REG                                             (0x0000BA20)
//Duplicate of MTG0_CR_MTG_AO_REG

#define MTG1_CR_MTG_ARC_REG                                            (0x0000BA24)
//Duplicate of MTG0_CR_MTG_ARC_REG

#define MTG1_CR_MTG_ARS0_REG                                           (0x0000BA28)
//Duplicate of MTG0_CR_MTG_ARS0_REG

#define MTG1_CR_MTG_ARS1_REG                                           (0x0000BA2C)
//Duplicate of MTG0_CR_MTG_ARS1_REG

#define MTG1_CR_MTG_RADC_REG                                           (0x0000BA30)
//Duplicate of MTG0_CR_MTG_RADC_REG

#define MTG1_CR_MTG_DQPATSEL0_REG                                      (0x0000BA50)
//Duplicate of MTG0_CR_MTG_DQPATSEL0_REG

#define MTG1_CR_MTG_DQPATSEL1_REG                                      (0x0000BA54)
//Duplicate of MTG0_CR_MTG_DQPATSEL1_REG

#define MTG1_CR_MTG_UPDQBM_REG                                         (0x0000BA58)
//Duplicate of MTG0_CR_MTG_UPDQBM_REG

#define MTG1_CR_MTG_DMPATSEL_REG                                       (0x0000BA5C)
//Duplicate of MTG0_CR_MTG_DMPATSEL_REG

#define MTG1_CR_MTG_SDQP0_0_REG                                        (0x0000BA60)
//Duplicate of MTG0_CR_MTG_SDQP0_0_REG

#define MTG1_CR_MTG_SDQP0_1_REG                                        (0x0000BA64)
//Duplicate of MTG0_CR_MTG_SDQP0_1_REG

#define MTG1_CR_MTG_SDQP1_0_REG                                        (0x0000BA68)
//Duplicate of MTG0_CR_MTG_SDQP1_0_REG

#define MTG1_CR_MTG_SDQP1_1_REG                                        (0x0000BA6C)
//Duplicate of MTG0_CR_MTG_SDQP1_1_REG

#define MTG1_CR_MTG_SDQP2_0_REG                                        (0x0000BA70)
//Duplicate of MTG0_CR_MTG_SDQP2_0_REG

#define MTG1_CR_MTG_SDQP2_1_REG                                        (0x0000BA74)
//Duplicate of MTG0_CR_MTG_SDQP2_1_REG

#define MTG1_CR_MTG_SDQP3_0_REG                                        (0x0000BA78)
//Duplicate of MTG0_CR_MTG_SDQP3_0_REG

#define MTG1_CR_MTG_SDQP3_1_REG                                        (0x0000BA7C)
//Duplicate of MTG0_CR_MTG_SDQP3_1_REG

#define MTG1_CR_MTG_SDMP0_0_REG                                        (0x0000BA80)
//Duplicate of MTG0_CR_MTG_SDMP0_0_REG

#define MTG1_CR_MTG_SDMP0_1_REG                                        (0x0000BA84)
//Duplicate of MTG0_CR_MTG_SDMP0_1_REG

#define MTG1_CR_MTG_SDMP1_0_REG                                        (0x0000BA88)
//Duplicate of MTG0_CR_MTG_SDMP1_0_REG

#define MTG1_CR_MTG_SDMP1_1_REG                                        (0x0000BA8C)
//Duplicate of MTG0_CR_MTG_SDMP1_1_REG

#define MTG1_CR_MTG_SDPL_REG                                           (0x0000BA90)
//Duplicate of MTG0_CR_MTG_SDPL_REG

#define MTG1_CR_MTG_DLFSR_POLY_REG                                     (0x0000BA94)
//Duplicate of MTG0_CR_MTG_DLFSR_POLY_REG

#define MTG1_CR_MTG_DLFSRX_POLY_REG                                    (0x0000BA98)
//Duplicate of MTG0_CR_MTG_DLFSRX_POLY_REG

#define MTG1_CR_MTG_DQLFSR0_SEED_REG                                   (0x0000BAB0)
//Duplicate of MTG0_CR_MTG_DQLFSR0_SEED_REG

#define MTG1_CR_MTG_DQLFSR1_SEED_REG                                   (0x0000BAB4)
//Duplicate of MTG0_CR_MTG_DQLFSR1_SEED_REG

#define MTG1_CR_MTG_DQLFSR2_SEED_REG                                   (0x0000BAB8)
//Duplicate of MTG0_CR_MTG_DQLFSR2_SEED_REG

#define MTG1_CR_MTG_DQLFSR3_SEED_REG                                   (0x0000BABC)
//Duplicate of MTG0_CR_MTG_DQLFSR3_SEED_REG

#define MTG1_CR_MTG_DMLFSR0_SEED_REG                                   (0x0000BAC0)
//Duplicate of MTG0_CR_MTG_DMLFSR0_SEED_REG

#define MTG1_CR_MTG_DMLFSR1_SEED_REG                                   (0x0000BAC4)
//Duplicate of MTG0_CR_MTG_DMLFSR1_SEED_REG

#define MTG1_CR_MTG_DQC_REG                                            (0x0000BAD0)
//Duplicate of MTG0_CR_MTG_DQC_REG

#define MTG1_CR_MTG_DQDCC0_REG                                         (0x0000BAD4)
//Duplicate of MTG0_CR_MTG_DQDCC0_REG

#define MTG1_CR_MTG_DQDCC1_REG                                         (0x0000BAD8)
//Duplicate of MTG0_CR_MTG_DQDCC1_REG

#define MTG1_CR_MTG_DQINVC0_REG                                        (0x0000BADC)
//Duplicate of MTG0_CR_MTG_DQINVC0_REG

#define MTG1_CR_MTG_DQINVC1_REG                                        (0x0000BAE0)
//Duplicate of MTG0_CR_MTG_DQINVC1_REG

#define MTG1_CR_MTG_DMDCINVC_REG                                       (0x0000BAE4)
//Duplicate of MTG0_CR_MTG_DMDCINVC_REG

#define MTG1_CR_MTG_DBAC_REG                                           (0x0000BB50)
//Duplicate of MTG0_CR_MTG_DBAC_REG

#define MTG1_CR_MTG_DBA_REG                                            (0x0000BB54)
//Duplicate of MTG0_CR_MTG_DBA_REG

#define MTG1_CR_MTG_LRA0_REG                                           (0x0000BB58)
//Duplicate of MTG0_CR_MTG_LRA0_REG

#define MTG1_CR_MTG_LRA1_REG                                           (0x0000BB5C)
//Duplicate of MTG0_CR_MTG_LRA1_REG

#define MTG1_CR_MTG_GEN_ERRCFG_REG                                     (0x0000BB60)
//Duplicate of MTG0_CR_MTG_GEN_ERRCFG_REG

#define MTG1_CR_MTG_GEN_ERR_REG                                        (0x0000BB68)
//Duplicate of MTG0_CR_MTG_GEN_ERR_REG

#define MTG1_CR_MTG_BYTE_LANE_ERR_REG                                  (0x0000BB6C)
//Duplicate of MTG0_CR_MTG_BYTE_LANE_ERR_REG

#define MTG1_CR_MTG_MRD_ERR_REG                                        (0x0000BB70)
//Duplicate of MTG0_CR_MTG_MRD_ERR_REG

#define MTG1_CR_MTG_CH0_ERRCFG_REG                                     (0x0000BB80)
//Duplicate of MTG0_CR_MTG_CH0_ERRCFG_REG

#define MTG1_CR_MTG_CH0_ERRCNT0_REG                                    (0x0000BB84)
//Duplicate of MTG0_CR_MTG_CH0_ERRCNT0_REG

#define MTG1_CR_MTG_CH0_ERRCNT1_REG                                    (0x0000BB88)
//Duplicate of MTG0_CR_MTG_CH0_ERRCNT1_REG

#define MTG1_CR_MTG_CH0_BEM0_REG                                       (0x0000BB90)
//Duplicate of MTG0_CR_MTG_CH0_BEM0_REG

#define MTG1_CR_MTG_CH0_BEM1_REG                                       (0x0000BB94)
//Duplicate of MTG0_CR_MTG_CH0_BEM1_REG

#define MTG1_CR_MTG_CH0_BEM2_REG                                       (0x0000BB98)
//Duplicate of MTG0_CR_MTG_CH0_BEM2_REG

#define MTG1_CR_MTG_CH0_UIEM_REG                                       (0x0000BB9C)
//Duplicate of MTG0_CR_MTG_CH0_UIEM_REG

#define MTG1_CR_MTG_CH0_LEM0_REG                                       (0x0000BBA0)
//Duplicate of MTG0_CR_MTG_CH0_LEM0_REG

#define MTG1_CR_MTG_CH0_LEM1_REG                                       (0x0000BBA4)
//Duplicate of MTG0_CR_MTG_CH0_LEM1_REG

#define MTG1_CR_MTG_CH0_CLEM_REG                                       (0x0000BBA8)
//Duplicate of MTG0_CR_MTG_CH0_CLEM_REG

#define MTG1_CR_MTG_CH0_BYTE_ERR0_REG                                  (0x0000BBB0)
//Duplicate of MTG0_CR_MTG_CH0_BYTE_ERR0_REG

#define MTG1_CR_MTG_CH0_BYTE_ERR1_REG                                  (0x0000BBB4)
//Duplicate of MTG0_CR_MTG_CH0_BYTE_ERR1_REG

#define MTG1_CR_MTG_CH0_BYTE_ERR2_REG                                  (0x0000BBB8)
//Duplicate of MTG0_CR_MTG_CH0_BYTE_ERR2_REG

#define MTG1_CR_MTG_CH0_UI_ERR_REG                                     (0x0000BBBC)
//Duplicate of MTG0_CR_MTG_CH0_UI_ERR_REG

#define MTG1_CR_MTG_CH0_EUILANE_ERR0_REG                               (0x0000BBC0)
//Duplicate of MTG0_CR_MTG_CH0_EUILANE_ERR0_REG

#define MTG1_CR_MTG_CH0_EUILANE_ERR1_REG                               (0x0000BBC4)
//Duplicate of MTG0_CR_MTG_CH0_EUILANE_ERR1_REG

#define MTG1_CR_MTG_CH0_OUILANE_ERR0_REG                               (0x0000BBC8)
//Duplicate of MTG0_CR_MTG_CH0_OUILANE_ERR0_REG

#define MTG1_CR_MTG_CH0_OUILANE_ERR1_REG                               (0x0000BBCC)
//Duplicate of MTG0_CR_MTG_CH0_OUILANE_ERR1_REG

#define MTG1_CR_MTG_CH0_BLEC0_REG                                      (0x0000BBD0)
//Duplicate of MTG0_CR_MTG_CH0_BLEC0_REG

#define MTG1_CR_MTG_CH0_BLEC1_REG                                      (0x0000BBD4)
//Duplicate of MTG0_CR_MTG_CH0_BLEC1_REG

#define MTG1_CR_MTG_CH0_BLEC2_REG                                      (0x0000BBD8)
//Duplicate of MTG0_CR_MTG_CH0_BLEC2_REG

#define MTG1_CR_MTG_CH0_DQEC0_REG                                      (0x0000BBE0)
//Duplicate of MTG0_CR_MTG_CH0_DQEC0_REG

#define MTG1_CR_MTG_CH0_DQEC1_REG                                      (0x0000BBE4)
//Duplicate of MTG0_CR_MTG_CH0_DQEC1_REG

#define MTG1_CR_MTG_CH0_DQEC2_REG                                      (0x0000BBE8)
//Duplicate of MTG0_CR_MTG_CH0_DQEC2_REG

#define MTG1_CR_MTG_CH0_DQEC3_REG                                      (0x0000BBEC)
//Duplicate of MTG0_CR_MTG_CH0_DQEC3_REG

#define MTG1_CR_MTG_CH0_DQEC4_REG                                      (0x0000BBF0)
//Duplicate of MTG0_CR_MTG_CH0_DQEC4_REG

#define MTG1_CR_MTG_CH0_DQEC5_REG                                      (0x0000BBF4)
//Duplicate of MTG0_CR_MTG_CH0_DQEC5_REG

#define MTG1_CR_MTG_CH0_DQEC6_REG                                      (0x0000BBF8)
//Duplicate of MTG0_CR_MTG_CH0_DQEC6_REG

#define MTG1_CR_MTG_CH0_DQEC7_REG                                      (0x0000BBFC)
//Duplicate of MTG0_CR_MTG_CH0_DQEC7_REG

#define MTG1_CR_MTG_CH0_DQEC8_REG                                      (0x0000BC00)
//Duplicate of MTG0_CR_MTG_CH0_DQEC8_REG

#define MTG1_CR_MTG_CH0_DQEC9_REG                                      (0x0000BC04)
//Duplicate of MTG0_CR_MTG_CH0_DQEC9_REG

#define MTG1_CR_MTG_CH1_ERRCFG_REG                                     (0x0000BC80)
//Duplicate of MTG0_CR_MTG_CH0_ERRCFG_REG

#define MTG1_CR_MTG_CH1_ERRCNT0_REG                                    (0x0000BC84)
//Duplicate of MTG0_CR_MTG_CH0_ERRCNT0_REG

#define MTG1_CR_MTG_CH1_ERRCNT1_REG                                    (0x0000BC88)
//Duplicate of MTG0_CR_MTG_CH0_ERRCNT1_REG

#define MTG1_CR_MTG_CH1_BEM0_REG                                       (0x0000BC90)
//Duplicate of MTG0_CR_MTG_CH0_BEM0_REG

#define MTG1_CR_MTG_CH1_BEM1_REG                                       (0x0000BC94)
//Duplicate of MTG0_CR_MTG_CH0_BEM1_REG

#define MTG1_CR_MTG_CH1_BEM2_REG                                       (0x0000BC98)
//Duplicate of MTG0_CR_MTG_CH0_BEM2_REG

#define MTG1_CR_MTG_CH1_UIEM_REG                                       (0x0000BC9C)
//Duplicate of MTG0_CR_MTG_CH0_UIEM_REG

#define MTG1_CR_MTG_CH1_LEM0_REG                                       (0x0000BCA0)
//Duplicate of MTG0_CR_MTG_CH0_LEM0_REG

#define MTG1_CR_MTG_CH1_LEM1_REG                                       (0x0000BCA4)
//Duplicate of MTG0_CR_MTG_CH0_LEM1_REG

#define MTG1_CR_MTG_CH1_CLEM_REG                                       (0x0000BCA8)
//Duplicate of MTG0_CR_MTG_CH0_CLEM_REG

#define MTG1_CR_MTG_CH1_BYTE_ERR0_REG                                  (0x0000BCB0)
//Duplicate of MTG0_CR_MTG_CH0_BYTE_ERR0_REG

#define MTG1_CR_MTG_CH1_BYTE_ERR1_REG                                  (0x0000BCB4)
//Duplicate of MTG0_CR_MTG_CH0_BYTE_ERR1_REG

#define MTG1_CR_MTG_CH1_BYTE_ERR2_REG                                  (0x0000BCB8)
//Duplicate of MTG0_CR_MTG_CH0_BYTE_ERR2_REG

#define MTG1_CR_MTG_CH1_UI_ERR_REG                                     (0x0000BCBC)
//Duplicate of MTG0_CR_MTG_CH0_UI_ERR_REG

#define MTG1_CR_MTG_CH1_EUILANE_ERR0_REG                               (0x0000BCC0)
//Duplicate of MTG0_CR_MTG_CH0_EUILANE_ERR0_REG

#define MTG1_CR_MTG_CH1_EUILANE_ERR1_REG                               (0x0000BCC4)
//Duplicate of MTG0_CR_MTG_CH0_EUILANE_ERR1_REG

#define MTG1_CR_MTG_CH1_OUILANE_ERR0_REG                               (0x0000BCC8)
//Duplicate of MTG0_CR_MTG_CH0_OUILANE_ERR0_REG

#define MTG1_CR_MTG_CH1_OUILANE_ERR1_REG                               (0x0000BCCC)
//Duplicate of MTG0_CR_MTG_CH0_OUILANE_ERR1_REG

#define MTG1_CR_MTG_CH1_BLEC0_REG                                      (0x0000BCD0)
//Duplicate of MTG0_CR_MTG_CH0_BLEC0_REG

#define MTG1_CR_MTG_CH1_BLEC1_REG                                      (0x0000BCD4)
//Duplicate of MTG0_CR_MTG_CH0_BLEC1_REG

#define MTG1_CR_MTG_CH1_BLEC2_REG                                      (0x0000BCD8)
//Duplicate of MTG0_CR_MTG_CH0_BLEC2_REG

#define MTG1_CR_MTG_CH1_DQEC0_REG                                      (0x0000BCE0)
//Duplicate of MTG0_CR_MTG_CH0_DQEC0_REG

#define MTG1_CR_MTG_CH1_DQEC1_REG                                      (0x0000BCE4)
//Duplicate of MTG0_CR_MTG_CH0_DQEC1_REG

#define MTG1_CR_MTG_CH1_DQEC2_REG                                      (0x0000BCE8)
//Duplicate of MTG0_CR_MTG_CH0_DQEC2_REG

#define MTG1_CR_MTG_CH1_DQEC3_REG                                      (0x0000BCEC)
//Duplicate of MTG0_CR_MTG_CH0_DQEC3_REG

#define MTG1_CR_MTG_CH1_DQEC4_REG                                      (0x0000BCF0)
//Duplicate of MTG0_CR_MTG_CH0_DQEC4_REG

#define MTG1_CR_MTG_CH1_DQEC5_REG                                      (0x0000BCF4)
//Duplicate of MTG0_CR_MTG_CH0_DQEC5_REG

#define MTG1_CR_MTG_CH1_DQEC6_REG                                      (0x0000BCF8)
//Duplicate of MTG0_CR_MTG_CH0_DQEC6_REG

#define MTG1_CR_MTG_CH1_DQEC7_REG                                      (0x0000BCFC)
//Duplicate of MTG0_CR_MTG_CH0_DQEC7_REG

#define MTG1_CR_MTG_CH1_DQEC8_REG                                      (0x0000BD00)
//Duplicate of MTG0_CR_MTG_CH0_DQEC8_REG

#define MTG1_CR_MTG_CH1_DQEC9_REG                                      (0x0000BD04)
//Duplicate of MTG0_CR_MTG_CH0_DQEC9_REG

#define MTG1_CR_MTG_CH2_ERRCFG_REG                                     (0x0000BD80)
//Duplicate of MTG0_CR_MTG_CH0_ERRCFG_REG

#define MTG1_CR_MTG_CH2_ERRCNT0_REG                                    (0x0000BD84)
//Duplicate of MTG0_CR_MTG_CH0_ERRCNT0_REG

#define MTG1_CR_MTG_CH2_ERRCNT1_REG                                    (0x0000BD88)
//Duplicate of MTG0_CR_MTG_CH0_ERRCNT1_REG

#define MTG1_CR_MTG_CH2_BEM0_REG                                       (0x0000BD90)
//Duplicate of MTG0_CR_MTG_CH0_BEM0_REG

#define MTG1_CR_MTG_CH2_BEM1_REG                                       (0x0000BD94)
//Duplicate of MTG0_CR_MTG_CH0_BEM1_REG

#define MTG1_CR_MTG_CH2_BEM2_REG                                       (0x0000BD98)
//Duplicate of MTG0_CR_MTG_CH0_BEM2_REG

#define MTG1_CR_MTG_CH2_UIEM_REG                                       (0x0000BD9C)
//Duplicate of MTG0_CR_MTG_CH0_UIEM_REG

#define MTG1_CR_MTG_CH2_LEM0_REG                                       (0x0000BDA0)
//Duplicate of MTG0_CR_MTG_CH0_LEM0_REG

#define MTG1_CR_MTG_CH2_LEM1_REG                                       (0x0000BDA4)
//Duplicate of MTG0_CR_MTG_CH0_LEM1_REG

#define MTG1_CR_MTG_CH2_CLEM_REG                                       (0x0000BDA8)
//Duplicate of MTG0_CR_MTG_CH0_CLEM_REG

#define MTG1_CR_MTG_CH2_BYTE_ERR0_REG                                  (0x0000BDB0)
//Duplicate of MTG0_CR_MTG_CH0_BYTE_ERR0_REG

#define MTG1_CR_MTG_CH2_BYTE_ERR1_REG                                  (0x0000BDB4)
//Duplicate of MTG0_CR_MTG_CH0_BYTE_ERR1_REG

#define MTG1_CR_MTG_CH2_BYTE_ERR2_REG                                  (0x0000BDB8)
//Duplicate of MTG0_CR_MTG_CH0_BYTE_ERR2_REG

#define MTG1_CR_MTG_CH2_UI_ERR_REG                                     (0x0000BDBC)
//Duplicate of MTG0_CR_MTG_CH0_UI_ERR_REG

#define MTG1_CR_MTG_CH2_EUILANE_ERR0_REG                               (0x0000BDC0)
//Duplicate of MTG0_CR_MTG_CH0_EUILANE_ERR0_REG

#define MTG1_CR_MTG_CH2_EUILANE_ERR1_REG                               (0x0000BDC4)
//Duplicate of MTG0_CR_MTG_CH0_EUILANE_ERR1_REG

#define MTG1_CR_MTG_CH2_OUILANE_ERR0_REG                               (0x0000BDC8)
//Duplicate of MTG0_CR_MTG_CH0_OUILANE_ERR0_REG

#define MTG1_CR_MTG_CH2_OUILANE_ERR1_REG                               (0x0000BDCC)
//Duplicate of MTG0_CR_MTG_CH0_OUILANE_ERR1_REG

#define MTG1_CR_MTG_CH2_BLEC0_REG                                      (0x0000BDD0)
//Duplicate of MTG0_CR_MTG_CH0_BLEC0_REG

#define MTG1_CR_MTG_CH2_BLEC1_REG                                      (0x0000BDD4)
//Duplicate of MTG0_CR_MTG_CH0_BLEC1_REG

#define MTG1_CR_MTG_CH2_BLEC2_REG                                      (0x0000BDD8)
//Duplicate of MTG0_CR_MTG_CH0_BLEC2_REG

#define MTG1_CR_MTG_CH2_DQEC0_REG                                      (0x0000BDE0)
//Duplicate of MTG0_CR_MTG_CH0_DQEC0_REG

#define MTG1_CR_MTG_CH2_DQEC1_REG                                      (0x0000BDE4)
//Duplicate of MTG0_CR_MTG_CH0_DQEC1_REG

#define MTG1_CR_MTG_CH2_DQEC2_REG                                      (0x0000BDE8)
//Duplicate of MTG0_CR_MTG_CH0_DQEC2_REG

#define MTG1_CR_MTG_CH2_DQEC3_REG                                      (0x0000BDEC)
//Duplicate of MTG0_CR_MTG_CH0_DQEC3_REG

#define MTG1_CR_MTG_CH2_DQEC4_REG                                      (0x0000BDF0)
//Duplicate of MTG0_CR_MTG_CH0_DQEC4_REG

#define MTG1_CR_MTG_CH2_DQEC5_REG                                      (0x0000BDF4)
//Duplicate of MTG0_CR_MTG_CH0_DQEC5_REG

#define MTG1_CR_MTG_CH2_DQEC6_REG                                      (0x0000BDF8)
//Duplicate of MTG0_CR_MTG_CH0_DQEC6_REG

#define MTG1_CR_MTG_CH2_DQEC7_REG                                      (0x0000BDFC)
//Duplicate of MTG0_CR_MTG_CH0_DQEC7_REG

#define MTG1_CR_MTG_CH2_DQEC8_REG                                      (0x0000BE00)
//Duplicate of MTG0_CR_MTG_CH0_DQEC8_REG

#define MTG1_CR_MTG_CH2_DQEC9_REG                                      (0x0000BE04)
//Duplicate of MTG0_CR_MTG_CH0_DQEC9_REG

#define MTG1_CR_MTG_CH3_ERRCFG_REG                                     (0x0000BE80)
//Duplicate of MTG0_CR_MTG_CH0_ERRCFG_REG

#define MTG1_CR_MTG_CH3_ERRCNT0_REG                                    (0x0000BE84)
//Duplicate of MTG0_CR_MTG_CH0_ERRCNT0_REG

#define MTG1_CR_MTG_CH3_ERRCNT1_REG                                    (0x0000BE88)
//Duplicate of MTG0_CR_MTG_CH0_ERRCNT1_REG

#define MTG1_CR_MTG_CH3_BEM0_REG                                       (0x0000BE90)
//Duplicate of MTG0_CR_MTG_CH0_BEM0_REG

#define MTG1_CR_MTG_CH3_BEM1_REG                                       (0x0000BE94)
//Duplicate of MTG0_CR_MTG_CH0_BEM1_REG

#define MTG1_CR_MTG_CH3_BEM2_REG                                       (0x0000BE98)
//Duplicate of MTG0_CR_MTG_CH0_BEM2_REG

#define MTG1_CR_MTG_CH3_UIEM_REG                                       (0x0000BE9C)
//Duplicate of MTG0_CR_MTG_CH0_UIEM_REG

#define MTG1_CR_MTG_CH3_LEM0_REG                                       (0x0000BEA0)
//Duplicate of MTG0_CR_MTG_CH0_LEM0_REG

#define MTG1_CR_MTG_CH3_LEM1_REG                                       (0x0000BEA4)
//Duplicate of MTG0_CR_MTG_CH0_LEM1_REG

#define MTG1_CR_MTG_CH3_CLEM_REG                                       (0x0000BEA8)
//Duplicate of MTG0_CR_MTG_CH0_CLEM_REG

#define MTG1_CR_MTG_CH3_BYTE_ERR0_REG                                  (0x0000BEB0)
//Duplicate of MTG0_CR_MTG_CH0_BYTE_ERR0_REG

#define MTG1_CR_MTG_CH3_BYTE_ERR1_REG                                  (0x0000BEB4)
//Duplicate of MTG0_CR_MTG_CH0_BYTE_ERR1_REG

#define MTG1_CR_MTG_CH3_BYTE_ERR2_REG                                  (0x0000BEB8)
//Duplicate of MTG0_CR_MTG_CH0_BYTE_ERR2_REG

#define MTG1_CR_MTG_CH3_UI_ERR_REG                                     (0x0000BEBC)
//Duplicate of MTG0_CR_MTG_CH0_UI_ERR_REG

#define MTG1_CR_MTG_CH3_EUILANE_ERR0_REG                               (0x0000BEC0)
//Duplicate of MTG0_CR_MTG_CH0_EUILANE_ERR0_REG

#define MTG1_CR_MTG_CH3_EUILANE_ERR1_REG                               (0x0000BEC4)
//Duplicate of MTG0_CR_MTG_CH0_EUILANE_ERR1_REG

#define MTG1_CR_MTG_CH3_OUILANE_ERR0_REG                               (0x0000BEC8)
//Duplicate of MTG0_CR_MTG_CH0_OUILANE_ERR0_REG

#define MTG1_CR_MTG_CH3_OUILANE_ERR1_REG                               (0x0000BECC)
//Duplicate of MTG0_CR_MTG_CH0_OUILANE_ERR1_REG

#define MTG1_CR_MTG_CH3_BLEC0_REG                                      (0x0000BED0)
//Duplicate of MTG0_CR_MTG_CH0_BLEC0_REG

#define MTG1_CR_MTG_CH3_BLEC1_REG                                      (0x0000BED4)
//Duplicate of MTG0_CR_MTG_CH0_BLEC1_REG

#define MTG1_CR_MTG_CH3_BLEC2_REG                                      (0x0000BED8)
//Duplicate of MTG0_CR_MTG_CH0_BLEC2_REG

#define MTG1_CR_MTG_CH3_DQEC0_REG                                      (0x0000BEE0)
//Duplicate of MTG0_CR_MTG_CH0_DQEC0_REG

#define MTG1_CR_MTG_CH3_DQEC1_REG                                      (0x0000BEE4)
//Duplicate of MTG0_CR_MTG_CH0_DQEC1_REG

#define MTG1_CR_MTG_CH3_DQEC2_REG                                      (0x0000BEE8)
//Duplicate of MTG0_CR_MTG_CH0_DQEC2_REG

#define MTG1_CR_MTG_CH3_DQEC3_REG                                      (0x0000BEEC)
//Duplicate of MTG0_CR_MTG_CH0_DQEC3_REG

#define MTG1_CR_MTG_CH3_DQEC4_REG                                      (0x0000BEF0)
//Duplicate of MTG0_CR_MTG_CH0_DQEC4_REG

#define MTG1_CR_MTG_CH3_DQEC5_REG                                      (0x0000BEF4)
//Duplicate of MTG0_CR_MTG_CH0_DQEC5_REG

#define MTG1_CR_MTG_CH3_DQEC6_REG                                      (0x0000BEF8)
//Duplicate of MTG0_CR_MTG_CH0_DQEC6_REG

#define MTG1_CR_MTG_CH3_DQEC7_REG                                      (0x0000BEFC)
//Duplicate of MTG0_CR_MTG_CH0_DQEC7_REG

#define MTG1_CR_MTG_CH3_DQEC8_REG                                      (0x0000BF00)
//Duplicate of MTG0_CR_MTG_CH0_DQEC8_REG

#define MTG1_CR_MTG_CH3_DQEC9_REG                                      (0x0000BF04)
//Duplicate of MTG0_CR_MTG_CH0_DQEC9_REG

#define MPTUSS_CR_MSS_CTRL0_REG                                        (0x0000BF80)
#define MPTUSS_CR_MSS_CTRL0_DEF                                        (0x00000000)
#define MPTUSS_CR_MSS_CTRL0_VOLATILE_BITFIELDS_MSK                     (0x00000001)

  #define MPTUSS_CR_MSS_CTRL0_GLOBAL_MPTU_START_OFF                    ( 0)
  #define MPTUSS_CR_MSS_CTRL0_GLOBAL_MPTU_START_WID                    ( 1)
  #define MPTUSS_CR_MSS_CTRL0_GLOBAL_MPTU_START_MSK                    (0x00000001)
  #define MPTUSS_CR_MSS_CTRL0_GLOBAL_MPTU_START_MIN                    (0)
  #define MPTUSS_CR_MSS_CTRL0_GLOBAL_MPTU_START_MAX                    (1) // 0x00000001
  #define MPTUSS_CR_MSS_CTRL0_GLOBAL_MPTU_START_DEF                    (0x00000000)
  #define MPTUSS_CR_MSS_CTRL0_GLOBAL_MPTU_START_HSH                    (0x0180BF80)

  #define MPTUSS_CR_MSS_CTRL0_GLOBAL_MPTU_START_DIS_OFF                ( 1)
  #define MPTUSS_CR_MSS_CTRL0_GLOBAL_MPTU_START_DIS_WID                ( 1)
  #define MPTUSS_CR_MSS_CTRL0_GLOBAL_MPTU_START_DIS_MSK                (0x00000002)
  #define MPTUSS_CR_MSS_CTRL0_GLOBAL_MPTU_START_DIS_MIN                (0)
  #define MPTUSS_CR_MSS_CTRL0_GLOBAL_MPTU_START_DIS_MAX                (1) // 0x00000001
  #define MPTUSS_CR_MSS_CTRL0_GLOBAL_MPTU_START_DIS_DEF                (0x00000000)
  #define MPTUSS_CR_MSS_CTRL0_GLOBAL_MPTU_START_DIS_HSH                (0x0102BF80)

  #define MPTUSS_CR_MSS_CTRL0_RSVD15_2_OFF                             ( 2)
  #define MPTUSS_CR_MSS_CTRL0_RSVD15_2_WID                             (14)
  #define MPTUSS_CR_MSS_CTRL0_RSVD15_2_MSK                             (0x0000FFFC)
  #define MPTUSS_CR_MSS_CTRL0_RSVD15_2_MIN                             (0)
  #define MPTUSS_CR_MSS_CTRL0_RSVD15_2_MAX                             (16383) // 0x00003FFF
  #define MPTUSS_CR_MSS_CTRL0_RSVD15_2_DEF                             (0x00000000)
  #define MPTUSS_CR_MSS_CTRL0_RSVD15_2_HSH                             (0x0E04BF80)

  #define MPTUSS_CR_MSS_CTRL0_MPTU0_START_DLY_OFF                      (16)
  #define MPTUSS_CR_MSS_CTRL0_MPTU0_START_DLY_WID                      ( 8)
  #define MPTUSS_CR_MSS_CTRL0_MPTU0_START_DLY_MSK                      (0x00FF0000)
  #define MPTUSS_CR_MSS_CTRL0_MPTU0_START_DLY_MIN                      (0)
  #define MPTUSS_CR_MSS_CTRL0_MPTU0_START_DLY_MAX                      (255) // 0x000000FF
  #define MPTUSS_CR_MSS_CTRL0_MPTU0_START_DLY_DEF                      (0x00000000)
  #define MPTUSS_CR_MSS_CTRL0_MPTU0_START_DLY_HSH                      (0x0820BF80)

  #define MPTUSS_CR_MSS_CTRL0_MPTU1_START_DLY_OFF                      (24)
  #define MPTUSS_CR_MSS_CTRL0_MPTU1_START_DLY_WID                      ( 8)
  #define MPTUSS_CR_MSS_CTRL0_MPTU1_START_DLY_MSK                      (0xFF000000)
  #define MPTUSS_CR_MSS_CTRL0_MPTU1_START_DLY_MIN                      (0)
  #define MPTUSS_CR_MSS_CTRL0_MPTU1_START_DLY_MAX                      (255) // 0x000000FF
  #define MPTUSS_CR_MSS_CTRL0_MPTU1_START_DLY_DEF                      (0x00000000)
  #define MPTUSS_CR_MSS_CTRL0_MPTU1_START_DLY_HSH                      (0x0830BF80)

#define MPTUSS_CR_MSS_CTRL1_REG                                        (0x0000BF84)
#define MPTUSS_CR_MSS_CTRL1_DEF                                        (0xFFFFFF01)
#define MPTUSS_CR_MSS_CTRL1_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define MPTUSS_CR_MSS_CTRL1_CHAN_WIDTH_OFF                           ( 0)
  #define MPTUSS_CR_MSS_CTRL1_CHAN_WIDTH_WID                           ( 2)
  #define MPTUSS_CR_MSS_CTRL1_CHAN_WIDTH_MSK                           (0x00000003)
  #define MPTUSS_CR_MSS_CTRL1_CHAN_WIDTH_MIN                           (0)
  #define MPTUSS_CR_MSS_CTRL1_CHAN_WIDTH_MAX                           (3) // 0x00000003
  #define MPTUSS_CR_MSS_CTRL1_CHAN_WIDTH_DEF                           (0x00000001)
  #define MPTUSS_CR_MSS_CTRL1_CHAN_WIDTH_HSH                           (0x0200BF84)

  #define MPTUSS_CR_MSS_CTRL1_RSVD4_2_OFF                              ( 2)
  #define MPTUSS_CR_MSS_CTRL1_RSVD4_2_WID                              ( 3)
  #define MPTUSS_CR_MSS_CTRL1_RSVD4_2_MSK                              (0x0000001C)
  #define MPTUSS_CR_MSS_CTRL1_RSVD4_2_MIN                              (0)
  #define MPTUSS_CR_MSS_CTRL1_RSVD4_2_MAX                              (7) // 0x00000007
  #define MPTUSS_CR_MSS_CTRL1_RSVD4_2_DEF                              (0x00000000)
  #define MPTUSS_CR_MSS_CTRL1_RSVD4_2_HSH                              (0x0304BF84)

  #define MPTUSS_CR_MSS_CTRL1_DFI_WCK_CTRL_OFF                         ( 5)
  #define MPTUSS_CR_MSS_CTRL1_DFI_WCK_CTRL_WID                         ( 1)
  #define MPTUSS_CR_MSS_CTRL1_DFI_WCK_CTRL_MSK                         (0x00000020)
  #define MPTUSS_CR_MSS_CTRL1_DFI_WCK_CTRL_MIN                         (0)
  #define MPTUSS_CR_MSS_CTRL1_DFI_WCK_CTRL_MAX                         (1) // 0x00000001
  #define MPTUSS_CR_MSS_CTRL1_DFI_WCK_CTRL_DEF                         (0x00000000)
  #define MPTUSS_CR_MSS_CTRL1_DFI_WCK_CTRL_HSH                         (0x010ABF84)

  #define MPTUSS_CR_MSS_CTRL1_DFI_DRAM_CLK_DIS_CTRL_OFF                ( 6)
  #define MPTUSS_CR_MSS_CTRL1_DFI_DRAM_CLK_DIS_CTRL_WID                ( 1)
  #define MPTUSS_CR_MSS_CTRL1_DFI_DRAM_CLK_DIS_CTRL_MSK                (0x00000040)
  #define MPTUSS_CR_MSS_CTRL1_DFI_DRAM_CLK_DIS_CTRL_MIN                (0)
  #define MPTUSS_CR_MSS_CTRL1_DFI_DRAM_CLK_DIS_CTRL_MAX                (1) // 0x00000001
  #define MPTUSS_CR_MSS_CTRL1_DFI_DRAM_CLK_DIS_CTRL_DEF                (0x00000000)
  #define MPTUSS_CR_MSS_CTRL1_DFI_DRAM_CLK_DIS_CTRL_HSH                (0x010CBF84)

  #define MPTUSS_CR_MSS_CTRL1_DFI_INIT_START_SEL_OFF                   ( 7)
  #define MPTUSS_CR_MSS_CTRL1_DFI_INIT_START_SEL_WID                   ( 1)
  #define MPTUSS_CR_MSS_CTRL1_DFI_INIT_START_SEL_MSK                   (0x00000080)
  #define MPTUSS_CR_MSS_CTRL1_DFI_INIT_START_SEL_MIN                   (0)
  #define MPTUSS_CR_MSS_CTRL1_DFI_INIT_START_SEL_MAX                   (1) // 0x00000001
  #define MPTUSS_CR_MSS_CTRL1_DFI_INIT_START_SEL_DEF                   (0x00000000)
  #define MPTUSS_CR_MSS_CTRL1_DFI_INIT_START_SEL_HSH                   (0x010EBF84)

  #define MPTUSS_CR_MSS_CTRL1_CHAN_EN_OFF                              ( 8)
  #define MPTUSS_CR_MSS_CTRL1_CHAN_EN_WID                              ( 8)
  #define MPTUSS_CR_MSS_CTRL1_CHAN_EN_MSK                              (0x0000FF00)
  #define MPTUSS_CR_MSS_CTRL1_CHAN_EN_MIN                              (0)
  #define MPTUSS_CR_MSS_CTRL1_CHAN_EN_MAX                              (255) // 0x000000FF
  #define MPTUSS_CR_MSS_CTRL1_CHAN_EN_DEF                              (0x000000FF)
  #define MPTUSS_CR_MSS_CTRL1_CHAN_EN_HSH                              (0x0810BF84)

  #define MPTUSS_CR_MSS_CTRL1_DFI_PHYUPD_EN_OFF                        (16)
  #define MPTUSS_CR_MSS_CTRL1_DFI_PHYUPD_EN_WID                        ( 8)
  #define MPTUSS_CR_MSS_CTRL1_DFI_PHYUPD_EN_MSK                        (0x00FF0000)
  #define MPTUSS_CR_MSS_CTRL1_DFI_PHYUPD_EN_MIN                        (0)
  #define MPTUSS_CR_MSS_CTRL1_DFI_PHYUPD_EN_MAX                        (255) // 0x000000FF
  #define MPTUSS_CR_MSS_CTRL1_DFI_PHYUPD_EN_DEF                        (0x000000FF)
  #define MPTUSS_CR_MSS_CTRL1_DFI_PHYUPD_EN_HSH                        (0x0820BF84)

  #define MPTUSS_CR_MSS_CTRL1_DFI_LP_CTRL_EN_OFF                       (24)
  #define MPTUSS_CR_MSS_CTRL1_DFI_LP_CTRL_EN_WID                       ( 8)
  #define MPTUSS_CR_MSS_CTRL1_DFI_LP_CTRL_EN_MSK                       (0xFF000000)
  #define MPTUSS_CR_MSS_CTRL1_DFI_LP_CTRL_EN_MIN                       (0)
  #define MPTUSS_CR_MSS_CTRL1_DFI_LP_CTRL_EN_MAX                       (255) // 0x000000FF
  #define MPTUSS_CR_MSS_CTRL1_DFI_LP_CTRL_EN_DEF                       (0x000000FF)
  #define MPTUSS_CR_MSS_CTRL1_DFI_LP_CTRL_EN_HSH                       (0x0830BF84)

#define MPTUSS_CR_MSS_CTRL2_REG                                        (0x0000BF88)
#define MPTUSS_CR_MSS_CTRL2_DEF                                        (0x44440000)
#define MPTUSS_CR_MSS_CTRL2_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define MPTUSS_CR_MSS_CTRL2_RSVD15_0_OFF                             ( 0)
  #define MPTUSS_CR_MSS_CTRL2_RSVD15_0_WID                             (16)
  #define MPTUSS_CR_MSS_CTRL2_RSVD15_0_MSK                             (0x0000FFFF)
  #define MPTUSS_CR_MSS_CTRL2_RSVD15_0_MIN                             (0)
  #define MPTUSS_CR_MSS_CTRL2_RSVD15_0_MAX                             (65535) // 0x0000FFFF
  #define MPTUSS_CR_MSS_CTRL2_RSVD15_0_DEF                             (0x00000000)
  #define MPTUSS_CR_MSS_CTRL2_RSVD15_0_HSH                             (0x1000BF88)

  #define MPTUSS_CR_MSS_CTRL2_TX_CH0_MPTU_MAP_OFF                      (16)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH0_MPTU_MAP_WID                      ( 2)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH0_MPTU_MAP_MSK                      (0x00030000)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH0_MPTU_MAP_MIN                      (0)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH0_MPTU_MAP_MAX                      (3) // 0x00000003
  #define MPTUSS_CR_MSS_CTRL2_TX_CH0_MPTU_MAP_DEF                      (0x00000000)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH0_MPTU_MAP_HSH                      (0x0220BF88)

  #define MPTUSS_CR_MSS_CTRL2_TX_CH1_MPTU_MAP_OFF                      (18)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH1_MPTU_MAP_WID                      ( 2)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH1_MPTU_MAP_MSK                      (0x000C0000)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH1_MPTU_MAP_MIN                      (0)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH1_MPTU_MAP_MAX                      (3) // 0x00000003
  #define MPTUSS_CR_MSS_CTRL2_TX_CH1_MPTU_MAP_DEF                      (0x00000001)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH1_MPTU_MAP_HSH                      (0x0224BF88)

  #define MPTUSS_CR_MSS_CTRL2_TX_CH2_MPTU_MAP_OFF                      (20)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH2_MPTU_MAP_WID                      ( 2)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH2_MPTU_MAP_MSK                      (0x00300000)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH2_MPTU_MAP_MIN                      (0)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH2_MPTU_MAP_MAX                      (3) // 0x00000003
  #define MPTUSS_CR_MSS_CTRL2_TX_CH2_MPTU_MAP_DEF                      (0x00000000)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH2_MPTU_MAP_HSH                      (0x0228BF88)

  #define MPTUSS_CR_MSS_CTRL2_TX_CH3_MPTU_MAP_OFF                      (22)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH3_MPTU_MAP_WID                      ( 2)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH3_MPTU_MAP_MSK                      (0x00C00000)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH3_MPTU_MAP_MIN                      (0)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH3_MPTU_MAP_MAX                      (3) // 0x00000003
  #define MPTUSS_CR_MSS_CTRL2_TX_CH3_MPTU_MAP_DEF                      (0x00000001)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH3_MPTU_MAP_HSH                      (0x022CBF88)

  #define MPTUSS_CR_MSS_CTRL2_TX_CH4_MPTU_MAP_OFF                      (24)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH4_MPTU_MAP_WID                      ( 2)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH4_MPTU_MAP_MSK                      (0x03000000)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH4_MPTU_MAP_MIN                      (0)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH4_MPTU_MAP_MAX                      (3) // 0x00000003
  #define MPTUSS_CR_MSS_CTRL2_TX_CH4_MPTU_MAP_DEF                      (0x00000000)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH4_MPTU_MAP_HSH                      (0x0230BF88)

  #define MPTUSS_CR_MSS_CTRL2_TX_CH5_MPTU_MAP_OFF                      (26)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH5_MPTU_MAP_WID                      ( 2)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH5_MPTU_MAP_MSK                      (0x0C000000)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH5_MPTU_MAP_MIN                      (0)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH5_MPTU_MAP_MAX                      (3) // 0x00000003
  #define MPTUSS_CR_MSS_CTRL2_TX_CH5_MPTU_MAP_DEF                      (0x00000001)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH5_MPTU_MAP_HSH                      (0x0234BF88)

  #define MPTUSS_CR_MSS_CTRL2_TX_CH6_MPTU_MAP_OFF                      (28)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH6_MPTU_MAP_WID                      ( 2)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH6_MPTU_MAP_MSK                      (0x30000000)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH6_MPTU_MAP_MIN                      (0)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH6_MPTU_MAP_MAX                      (3) // 0x00000003
  #define MPTUSS_CR_MSS_CTRL2_TX_CH6_MPTU_MAP_DEF                      (0x00000000)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH6_MPTU_MAP_HSH                      (0x0238BF88)

  #define MPTUSS_CR_MSS_CTRL2_TX_CH7_MPTU_MAP_OFF                      (30)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH7_MPTU_MAP_WID                      ( 2)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH7_MPTU_MAP_MSK                      (0xC0000000)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH7_MPTU_MAP_MIN                      (0)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH7_MPTU_MAP_MAX                      (3) // 0x00000003
  #define MPTUSS_CR_MSS_CTRL2_TX_CH7_MPTU_MAP_DEF                      (0x00000001)
  #define MPTUSS_CR_MSS_CTRL2_TX_CH7_MPTU_MAP_HSH                      (0x023CBF88)

#define MPTUSS_CR_MSS_CTRL3_REG                                        (0x0000BF8C)
#define MPTUSS_CR_MSS_CTRL3_DEF                                        (0xFB73D951)
#define MPTUSS_CR_MSS_CTRL3_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH0_RX_EN_OFF                      ( 0)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH0_RX_EN_WID                      ( 1)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH0_RX_EN_MSK                      (0x00000001)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH0_RX_EN_MIN                      (0)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH0_RX_EN_MAX                      (1) // 0x00000001
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH0_RX_EN_DEF                      (0x00000001)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH0_RX_EN_HSH                      (0x0100BF8C)

  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH0_RX_MAP_OFF                     ( 1)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH0_RX_MAP_WID                     ( 3)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH0_RX_MAP_MSK                     (0x0000000E)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH0_RX_MAP_MIN                     (0)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH0_RX_MAP_MAX                     (7) // 0x00000007
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH0_RX_MAP_DEF                     (0x00000000)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH0_RX_MAP_HSH                     (0x0302BF8C)

  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH1_RX_EN_OFF                      ( 4)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH1_RX_EN_WID                      ( 1)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH1_RX_EN_MSK                      (0x00000010)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH1_RX_EN_MIN                      (0)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH1_RX_EN_MAX                      (1) // 0x00000001
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH1_RX_EN_DEF                      (0x00000001)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH1_RX_EN_HSH                      (0x0108BF8C)

  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH1_RX_MAP_OFF                     ( 5)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH1_RX_MAP_WID                     ( 3)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH1_RX_MAP_MSK                     (0x000000E0)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH1_RX_MAP_MIN                     (0)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH1_RX_MAP_MAX                     (7) // 0x00000007
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH1_RX_MAP_DEF                     (0x00000002)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH1_RX_MAP_HSH                     (0x030ABF8C)

  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH2_RX_EN_OFF                      ( 8)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH2_RX_EN_WID                      ( 1)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH2_RX_EN_MSK                      (0x00000100)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH2_RX_EN_MIN                      (0)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH2_RX_EN_MAX                      (1) // 0x00000001
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH2_RX_EN_DEF                      (0x00000001)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH2_RX_EN_HSH                      (0x0110BF8C)

  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH2_RX_MAP_OFF                     ( 9)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH2_RX_MAP_WID                     ( 3)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH2_RX_MAP_MSK                     (0x00000E00)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH2_RX_MAP_MIN                     (0)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH2_RX_MAP_MAX                     (7) // 0x00000007
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH2_RX_MAP_DEF                     (0x00000004)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH2_RX_MAP_HSH                     (0x0312BF8C)

  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH3_RX_EN_OFF                      (12)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH3_RX_EN_WID                      ( 1)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH3_RX_EN_MSK                      (0x00001000)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH3_RX_EN_MIN                      (0)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH3_RX_EN_MAX                      (1) // 0x00000001
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH3_RX_EN_DEF                      (0x00000001)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH3_RX_EN_HSH                      (0x0118BF8C)

  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH3_RX_MAP_OFF                     (13)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH3_RX_MAP_WID                     ( 3)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH3_RX_MAP_MSK                     (0x0000E000)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH3_RX_MAP_MIN                     (0)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH3_RX_MAP_MAX                     (7) // 0x00000007
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH3_RX_MAP_DEF                     (0x00000006)
  #define MPTUSS_CR_MSS_CTRL3_MPTU0_CH3_RX_MAP_HSH                     (0x031ABF8C)

  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH0_RX_EN_OFF                      (16)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH0_RX_EN_WID                      ( 1)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH0_RX_EN_MSK                      (0x00010000)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH0_RX_EN_MIN                      (0)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH0_RX_EN_MAX                      (1) // 0x00000001
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH0_RX_EN_DEF                      (0x00000001)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH0_RX_EN_HSH                      (0x0120BF8C)

  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH0_RX_MAP_OFF                     (17)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH0_RX_MAP_WID                     ( 3)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH0_RX_MAP_MSK                     (0x000E0000)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH0_RX_MAP_MIN                     (0)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH0_RX_MAP_MAX                     (7) // 0x00000007
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH0_RX_MAP_DEF                     (0x00000001)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH0_RX_MAP_HSH                     (0x0322BF8C)

  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH1_RX_EN_OFF                      (20)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH1_RX_EN_WID                      ( 1)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH1_RX_EN_MSK                      (0x00100000)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH1_RX_EN_MIN                      (0)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH1_RX_EN_MAX                      (1) // 0x00000001
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH1_RX_EN_DEF                      (0x00000001)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH1_RX_EN_HSH                      (0x0128BF8C)

  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH1_RX_MAP_OFF                     (21)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH1_RX_MAP_WID                     ( 3)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH1_RX_MAP_MSK                     (0x00E00000)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH1_RX_MAP_MIN                     (0)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH1_RX_MAP_MAX                     (7) // 0x00000007
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH1_RX_MAP_DEF                     (0x00000003)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH1_RX_MAP_HSH                     (0x032ABF8C)

  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH2_RX_EN_OFF                      (24)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH2_RX_EN_WID                      ( 1)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH2_RX_EN_MSK                      (0x01000000)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH2_RX_EN_MIN                      (0)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH2_RX_EN_MAX                      (1) // 0x00000001
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH2_RX_EN_DEF                      (0x00000001)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH2_RX_EN_HSH                      (0x0130BF8C)

  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH2_RX_MAP_OFF                     (25)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH2_RX_MAP_WID                     ( 3)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH2_RX_MAP_MSK                     (0x0E000000)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH2_RX_MAP_MIN                     (0)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH2_RX_MAP_MAX                     (7) // 0x00000007
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH2_RX_MAP_DEF                     (0x00000005)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH2_RX_MAP_HSH                     (0x0332BF8C)

  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH3_RX_EN_OFF                      (28)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH3_RX_EN_WID                      ( 1)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH3_RX_EN_MSK                      (0x10000000)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH3_RX_EN_MIN                      (0)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH3_RX_EN_MAX                      (1) // 0x00000001
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH3_RX_EN_DEF                      (0x00000001)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH3_RX_EN_HSH                      (0x0138BF8C)

  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH3_RX_MAP_OFF                     (29)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH3_RX_MAP_WID                     ( 3)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH3_RX_MAP_MSK                     (0xE0000000)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH3_RX_MAP_MIN                     (0)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH3_RX_MAP_MAX                     (7) // 0x00000007
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH3_RX_MAP_DEF                     (0x00000007)
  #define MPTUSS_CR_MSS_CTRL3_MPTU1_CH3_RX_MAP_HSH                     (0x033ABF8C)



#pragma pack(pop)
#endif /* __MrcRegisterPtlBxxx_h__ */







