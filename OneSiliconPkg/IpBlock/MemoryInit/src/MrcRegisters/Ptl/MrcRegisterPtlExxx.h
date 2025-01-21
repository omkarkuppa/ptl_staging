#ifndef __MrcRegisterPtlExxx_h__
#define __MrcRegisterPtlExxx_h__
/** @file
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


#define MC0_CH0_CR_TC_PRE_REG                                          (0x0000E000)
#define MC0_CH0_CR_TC_PRE_DEF                                          (0x100380304060280A)
#define MC0_CH0_CR_TC_PRE_VOLATILE_BITFIELDS_MSK                       (0x0000000000000000)

  #define MC0_CH0_CR_TC_PRE_trppb_OFF                                  ( 0)
  #define MC0_CH0_CR_TC_PRE_trppb_WID                                  ( 8)
  #define MC0_CH0_CR_TC_PRE_trppb_MSK                                  (0x000000FF)
  #define MC0_CH0_CR_TC_PRE_trppb_MIN                                  (10)
  #define MC0_CH0_CR_TC_PRE_trppb_MAX                                  (76) // 0x0000004C
  #define MC0_CH0_CR_TC_PRE_trppb_DEF                                  (0x0000000A)
  #define MC0_CH0_CR_TC_PRE_trppb_HSH                                  (0x4800E000)

  #define MC0_CH0_CR_TC_PRE_trpab_OFF                                  (10)
  #define MC0_CH0_CR_TC_PRE_trpab_WID                                  ( 8)
  #define MC0_CH0_CR_TC_PRE_trpab_MSK                                  (0x0003FC00)
  #define MC0_CH0_CR_TC_PRE_trpab_MIN                                  (10)
  #define MC0_CH0_CR_TC_PRE_trpab_MAX                                  (92) // 0x0000005C
  #define MC0_CH0_CR_TC_PRE_trpab_DEF                                  (0x0000000A)
  #define MC0_CH0_CR_TC_PRE_trpab_HSH                                  (0x4814E000)

  #define MC0_CH0_CR_TC_PRE_trdpre_OFF                                 (20)
  #define MC0_CH0_CR_TC_PRE_trdpre_WID                                 ( 7)
  #define MC0_CH0_CR_TC_PRE_trdpre_MSK                                 (0x07F00000)
  #define MC0_CH0_CR_TC_PRE_trdpre_MIN                                 (4)
  #define MC0_CH0_CR_TC_PRE_trdpre_MAX                                 (35) // 0x00000023
  #define MC0_CH0_CR_TC_PRE_trdpre_DEF                                 (0x00000006)
  #define MC0_CH0_CR_TC_PRE_trdpre_HSH                                 (0x4728E000)

  #define MC0_CH0_CR_TC_PRE_tppd_OFF                                   (28)
  #define MC0_CH0_CR_TC_PRE_tppd_WID                                   ( 4)
  #define MC0_CH0_CR_TC_PRE_tppd_MSK                                   (0xF0000000)
  #define MC0_CH0_CR_TC_PRE_tppd_MIN                                   (4)
  #define MC0_CH0_CR_TC_PRE_tppd_MAX                                   (8) // 0x00000008
  #define MC0_CH0_CR_TC_PRE_tppd_DEF                                   (0x00000004)
  #define MC0_CH0_CR_TC_PRE_tppd_HSH                                   (0x4438E000)

  #define MC0_CH0_CR_TC_PRE_twrpre_OFF                                 (33)
  #define MC0_CH0_CR_TC_PRE_twrpre_WID                                 (10)
  #define MC0_CH0_CR_TC_PRE_twrpre_MSK                                 (0x000007FE00000000ULL)
  #define MC0_CH0_CR_TC_PRE_twrpre_MIN                                 (18)
  #define MC0_CH0_CR_TC_PRE_twrpre_MAX                                 (221) // 0x000000DD
  #define MC0_CH0_CR_TC_PRE_twrpre_DEF                                 (0x00000018)
  #define MC0_CH0_CR_TC_PRE_twrpre_HSH                                 (0x4A42E000)

  #define MC0_CH0_CR_TC_PRE_tras_OFF                                   (45)
  #define MC0_CH0_CR_TC_PRE_tras_WID                                   ( 9)
  #define MC0_CH0_CR_TC_PRE_tras_MSK                                   (0x003FE00000000000ULL)
  #define MC0_CH0_CR_TC_PRE_tras_MIN                                   (9)
  #define MC0_CH0_CR_TC_PRE_tras_MAX                                   (148) // 0x00000094
  #define MC0_CH0_CR_TC_PRE_tras_DEF                                   (0x0000001C)
  #define MC0_CH0_CR_TC_PRE_tras_HSH                                   (0x495AE000)

  #define MC0_CH0_CR_TC_PRE_nWR_diff_OFF                               (54)
  #define MC0_CH0_CR_TC_PRE_nWR_diff_WID                               ( 5)
  #define MC0_CH0_CR_TC_PRE_nWR_diff_MSK                               (0x07C0000000000000ULL)
  #define MC0_CH0_CR_TC_PRE_nWR_diff_MIN                               (0)
  #define MC0_CH0_CR_TC_PRE_nWR_diff_MAX                               (12) // 0x0000000C
  #define MC0_CH0_CR_TC_PRE_nWR_diff_DEF                               (0x00000000)
  #define MC0_CH0_CR_TC_PRE_nWR_diff_HSH                               (0x456CE000)

  #define MC0_CH0_CR_TC_PRE_derating_ext_OFF                           (59)
  #define MC0_CH0_CR_TC_PRE_derating_ext_WID                           ( 4)
  #define MC0_CH0_CR_TC_PRE_derating_ext_MSK                           (0x7800000000000000ULL)
  #define MC0_CH0_CR_TC_PRE_derating_ext_MIN                           (0)
  #define MC0_CH0_CR_TC_PRE_derating_ext_MAX                           (8) // 0x00000008
  #define MC0_CH0_CR_TC_PRE_derating_ext_DEF                           (0x00000002)
  #define MC0_CH0_CR_TC_PRE_derating_ext_HSH                           (0x4476E000)

#define MC0_CH0_CR_TC_RDRD_REG                                         (0x0000E00C)
#define MC0_CH0_CR_TC_RDRD_DEF                                         (0x04040404)
#define MC0_CH0_CR_TC_RDRD_VOLATILE_BITFIELDS_MSK                      (0x00000000)

  #define MC0_CH0_CR_TC_RDRD_tRDRD_sg_OFF                              ( 0)
  #define MC0_CH0_CR_TC_RDRD_tRDRD_sg_WID                              ( 7)
  #define MC0_CH0_CR_TC_RDRD_tRDRD_sg_MSK                              (0x0000007F)
  #define MC0_CH0_CR_TC_RDRD_tRDRD_sg_MIN                              (4)
  #define MC0_CH0_CR_TC_RDRD_tRDRD_sg_MAX                              (54) // 0x00000036
  #define MC0_CH0_CR_TC_RDRD_tRDRD_sg_DEF                              (0x00000004)
  #define MC0_CH0_CR_TC_RDRD_tRDRD_sg_HSH                              (0x0700E00C)

  #define MC0_CH0_CR_TC_RDRD_tRDRD_dg_OFF                              ( 8)
  #define MC0_CH0_CR_TC_RDRD_tRDRD_dg_WID                              ( 7)
  #define MC0_CH0_CR_TC_RDRD_tRDRD_dg_MSK                              (0x00007F00)
  #define MC0_CH0_CR_TC_RDRD_tRDRD_dg_MIN                              (4)
  #define MC0_CH0_CR_TC_RDRD_tRDRD_dg_MAX                              (54) // 0x00000036
  #define MC0_CH0_CR_TC_RDRD_tRDRD_dg_DEF                              (0x00000004)
  #define MC0_CH0_CR_TC_RDRD_tRDRD_dg_HSH                              (0x0710E00C)

  #define MC0_CH0_CR_TC_RDRD_tRDRD_dr_OFF                              (16)
  #define MC0_CH0_CR_TC_RDRD_tRDRD_dr_WID                              ( 8)
  #define MC0_CH0_CR_TC_RDRD_tRDRD_dr_MSK                              (0x00FF0000)
  #define MC0_CH0_CR_TC_RDRD_tRDRD_dr_MIN                              (4)
  #define MC0_CH0_CR_TC_RDRD_tRDRD_dr_MAX                              (54) // 0x00000036
  #define MC0_CH0_CR_TC_RDRD_tRDRD_dr_DEF                              (0x00000004)
  #define MC0_CH0_CR_TC_RDRD_tRDRD_dr_HSH                              (0x0820E00C)

  #define MC0_CH0_CR_TC_RDRD_tRDRD_dd_OFF                              (24)
  #define MC0_CH0_CR_TC_RDRD_tRDRD_dd_WID                              ( 8)
  #define MC0_CH0_CR_TC_RDRD_tRDRD_dd_MSK                              (0xFF000000)
  #define MC0_CH0_CR_TC_RDRD_tRDRD_dd_MIN                              (4)
  #define MC0_CH0_CR_TC_RDRD_tRDRD_dd_MAX                              (54) // 0x00000036
  #define MC0_CH0_CR_TC_RDRD_tRDRD_dd_DEF                              (0x00000004)
  #define MC0_CH0_CR_TC_RDRD_tRDRD_dd_HSH                              (0x0830E00C)

#define MC0_CH0_CR_TC_RDWR_REG                                         (0x0000E010)
#define MC0_CH0_CR_TC_RDWR_DEF                                         (0x04040404)
#define MC0_CH0_CR_TC_RDWR_VOLATILE_BITFIELDS_MSK                      (0x00000000)

  #define MC0_CH0_CR_TC_RDWR_tRDWR_sg_OFF                              ( 0)
  #define MC0_CH0_CR_TC_RDWR_tRDWR_sg_WID                              ( 8)
  #define MC0_CH0_CR_TC_RDWR_tRDWR_sg_MSK                              (0x000000FF)
  #define MC0_CH0_CR_TC_RDWR_tRDWR_sg_MIN                              (4)
  #define MC0_CH0_CR_TC_RDWR_tRDWR_sg_MAX                              (54) // 0x00000036
  #define MC0_CH0_CR_TC_RDWR_tRDWR_sg_DEF                              (0x00000004)
  #define MC0_CH0_CR_TC_RDWR_tRDWR_sg_HSH                              (0x0800E010)

  #define MC0_CH0_CR_TC_RDWR_tRDWR_dg_OFF                              ( 8)
  #define MC0_CH0_CR_TC_RDWR_tRDWR_dg_WID                              ( 8)
  #define MC0_CH0_CR_TC_RDWR_tRDWR_dg_MSK                              (0x0000FF00)
  #define MC0_CH0_CR_TC_RDWR_tRDWR_dg_MIN                              (4)
  #define MC0_CH0_CR_TC_RDWR_tRDWR_dg_MAX                              (54) // 0x00000036
  #define MC0_CH0_CR_TC_RDWR_tRDWR_dg_DEF                              (0x00000004)
  #define MC0_CH0_CR_TC_RDWR_tRDWR_dg_HSH                              (0x0810E010)

  #define MC0_CH0_CR_TC_RDWR_tRDWR_dr_OFF                              (16)
  #define MC0_CH0_CR_TC_RDWR_tRDWR_dr_WID                              ( 8)
  #define MC0_CH0_CR_TC_RDWR_tRDWR_dr_MSK                              (0x00FF0000)
  #define MC0_CH0_CR_TC_RDWR_tRDWR_dr_MIN                              (4)
  #define MC0_CH0_CR_TC_RDWR_tRDWR_dr_MAX                              (54) // 0x00000036
  #define MC0_CH0_CR_TC_RDWR_tRDWR_dr_DEF                              (0x00000004)
  #define MC0_CH0_CR_TC_RDWR_tRDWR_dr_HSH                              (0x0820E010)

  #define MC0_CH0_CR_TC_RDWR_tRDWR_dd_OFF                              (24)
  #define MC0_CH0_CR_TC_RDWR_tRDWR_dd_WID                              ( 8)
  #define MC0_CH0_CR_TC_RDWR_tRDWR_dd_MSK                              (0xFF000000)
  #define MC0_CH0_CR_TC_RDWR_tRDWR_dd_MIN                              (4)
  #define MC0_CH0_CR_TC_RDWR_tRDWR_dd_MAX                              (54) // 0x00000036
  #define MC0_CH0_CR_TC_RDWR_tRDWR_dd_DEF                              (0x00000004)
  #define MC0_CH0_CR_TC_RDWR_tRDWR_dd_HSH                              (0x0830E010)

#define MC0_CH0_CR_TC_WRRD_REG                                         (0x0000E014)
#define MC0_CH0_CR_TC_WRRD_DEF                                         (0x08100804)
#define MC0_CH0_CR_TC_WRRD_VOLATILE_BITFIELDS_MSK                      (0x00000000)

  #define MC0_CH0_CR_TC_WRRD_tWRRD_sg_OFF                              ( 0)
  #define MC0_CH0_CR_TC_WRRD_tWRRD_sg_WID                              ( 9)
  #define MC0_CH0_CR_TC_WRRD_tWRRD_sg_MSK                              (0x000001FF)
  #define MC0_CH0_CR_TC_WRRD_tWRRD_sg_MIN                              (4)
  #define MC0_CH0_CR_TC_WRRD_tWRRD_sg_MAX                              (145) // 0x00000091
  #define MC0_CH0_CR_TC_WRRD_tWRRD_sg_DEF                              (0x00000004)
  #define MC0_CH0_CR_TC_WRRD_tWRRD_sg_HSH                              (0x0900E014)

  #define MC0_CH0_CR_TC_WRRD_tWRRD_dg_OFF                              ( 9)
  #define MC0_CH0_CR_TC_WRRD_tWRRD_dg_WID                              ( 9)
  #define MC0_CH0_CR_TC_WRRD_tWRRD_dg_MSK                              (0x0003FE00)
  #define MC0_CH0_CR_TC_WRRD_tWRRD_dg_MIN                              (0)
  #define MC0_CH0_CR_TC_WRRD_tWRRD_dg_MAX                              (255) // 0x000000FF
  #define MC0_CH0_CR_TC_WRRD_tWRRD_dg_DEF                              (0x00000004)
  #define MC0_CH0_CR_TC_WRRD_tWRRD_dg_HSH                              (0x0912E014)

  #define MC0_CH0_CR_TC_WRRD_tWRRD_dr_OFF                              (18)
  #define MC0_CH0_CR_TC_WRRD_tWRRD_dr_WID                              ( 7)
  #define MC0_CH0_CR_TC_WRRD_tWRRD_dr_MSK                              (0x01FC0000)
  #define MC0_CH0_CR_TC_WRRD_tWRRD_dr_MIN                              (4)
  #define MC0_CH0_CR_TC_WRRD_tWRRD_dr_MAX                              (54) // 0x00000036
  #define MC0_CH0_CR_TC_WRRD_tWRRD_dr_DEF                              (0x00000004)
  #define MC0_CH0_CR_TC_WRRD_tWRRD_dr_HSH                              (0x0724E014)

  #define MC0_CH0_CR_TC_WRRD_tWRRD_dd_OFF                              (25)
  #define MC0_CH0_CR_TC_WRRD_tWRRD_dd_WID                              ( 7)
  #define MC0_CH0_CR_TC_WRRD_tWRRD_dd_MSK                              (0xFE000000)
  #define MC0_CH0_CR_TC_WRRD_tWRRD_dd_MIN                              (4)
  #define MC0_CH0_CR_TC_WRRD_tWRRD_dd_MAX                              (54) // 0x00000036
  #define MC0_CH0_CR_TC_WRRD_tWRRD_dd_DEF                              (0x00000004)
  #define MC0_CH0_CR_TC_WRRD_tWRRD_dd_HSH                              (0x0732E014)

#define MC0_CH0_CR_TC_WRWR_REG                                         (0x0000E018)
#define MC0_CH0_CR_TC_WRWR_DEF                                         (0x04040404)
#define MC0_CH0_CR_TC_WRWR_VOLATILE_BITFIELDS_MSK                      (0x00000000)

  #define MC0_CH0_CR_TC_WRWR_tWRWR_sg_OFF                              ( 0)
  #define MC0_CH0_CR_TC_WRWR_tWRWR_sg_WID                              ( 7)
  #define MC0_CH0_CR_TC_WRWR_tWRWR_sg_MSK                              (0x0000007F)
  #define MC0_CH0_CR_TC_WRWR_tWRWR_sg_MIN                              (4)
  #define MC0_CH0_CR_TC_WRWR_tWRWR_sg_MAX                              (92) // 0x0000005C
  #define MC0_CH0_CR_TC_WRWR_tWRWR_sg_DEF                              (0x00000004)
  #define MC0_CH0_CR_TC_WRWR_tWRWR_sg_HSH                              (0x0700E018)

  #define MC0_CH0_CR_TC_WRWR_tWRWR_dg_OFF                              ( 8)
  #define MC0_CH0_CR_TC_WRWR_tWRWR_dg_WID                              ( 7)
  #define MC0_CH0_CR_TC_WRWR_tWRWR_dg_MSK                              (0x00007F00)
  #define MC0_CH0_CR_TC_WRWR_tWRWR_dg_MIN                              (4)
  #define MC0_CH0_CR_TC_WRWR_tWRWR_dg_MAX                              (54) // 0x00000036
  #define MC0_CH0_CR_TC_WRWR_tWRWR_dg_DEF                              (0x00000004)
  #define MC0_CH0_CR_TC_WRWR_tWRWR_dg_HSH                              (0x0710E018)

  #define MC0_CH0_CR_TC_WRWR_tWRWR_dr_OFF                              (16)
  #define MC0_CH0_CR_TC_WRWR_tWRWR_dr_WID                              ( 7)
  #define MC0_CH0_CR_TC_WRWR_tWRWR_dr_MSK                              (0x007F0000)
  #define MC0_CH0_CR_TC_WRWR_tWRWR_dr_MIN                              (4)
  #define MC0_CH0_CR_TC_WRWR_tWRWR_dr_MAX                              (54) // 0x00000036
  #define MC0_CH0_CR_TC_WRWR_tWRWR_dr_DEF                              (0x00000004)
  #define MC0_CH0_CR_TC_WRWR_tWRWR_dr_HSH                              (0x0720E018)

  #define MC0_CH0_CR_TC_WRWR_tWRWR_dd_OFF                              (24)
  #define MC0_CH0_CR_TC_WRWR_tWRWR_dd_WID                              ( 8)
  #define MC0_CH0_CR_TC_WRWR_tWRWR_dd_MSK                              (0xFF000000)
  #define MC0_CH0_CR_TC_WRWR_tWRWR_dd_MIN                              (4)
  #define MC0_CH0_CR_TC_WRWR_tWRWR_dd_MAX                              (54) // 0x00000036
  #define MC0_CH0_CR_TC_WRWR_tWRWR_dd_DEF                              (0x00000004)
  #define MC0_CH0_CR_TC_WRWR_tWRWR_dd_HSH                              (0x0830E018)

#define MC0_CH0_CR_PM_ADAPTIVE_CKE_REG                                 (0x0000E01C)
#define MC0_CH0_CR_PM_ADAPTIVE_CKE_DEF                                 (0x00100010)
#define MC0_CH0_CR_PM_ADAPTIVE_CKE_VOLATILE_BITFIELDS_MSK              (0x00000000)

  #define MC0_CH0_CR_PM_ADAPTIVE_CKE_MIN_PDWN_IDLE_COUNTER_OFF         ( 0)
  #define MC0_CH0_CR_PM_ADAPTIVE_CKE_MIN_PDWN_IDLE_COUNTER_WID         (12)
  #define MC0_CH0_CR_PM_ADAPTIVE_CKE_MIN_PDWN_IDLE_COUNTER_MSK         (0x00000FFF)
  #define MC0_CH0_CR_PM_ADAPTIVE_CKE_MIN_PDWN_IDLE_COUNTER_MIN         (0)
  #define MC0_CH0_CR_PM_ADAPTIVE_CKE_MIN_PDWN_IDLE_COUNTER_MAX         (4095) // 0x00000FFF
  #define MC0_CH0_CR_PM_ADAPTIVE_CKE_MIN_PDWN_IDLE_COUNTER_DEF         (0x00000010)
  #define MC0_CH0_CR_PM_ADAPTIVE_CKE_MIN_PDWN_IDLE_COUNTER_HSH         (0x0C00E01C)

  #define MC0_CH0_CR_PM_ADAPTIVE_CKE_MAX_PDWN_IDLE_COUNTER_OFF         (12)
  #define MC0_CH0_CR_PM_ADAPTIVE_CKE_MAX_PDWN_IDLE_COUNTER_WID         (12)
  #define MC0_CH0_CR_PM_ADAPTIVE_CKE_MAX_PDWN_IDLE_COUNTER_MSK         (0x00FFF000)
  #define MC0_CH0_CR_PM_ADAPTIVE_CKE_MAX_PDWN_IDLE_COUNTER_MIN         (0)
  #define MC0_CH0_CR_PM_ADAPTIVE_CKE_MAX_PDWN_IDLE_COUNTER_MAX         (4095) // 0x00000FFF
  #define MC0_CH0_CR_PM_ADAPTIVE_CKE_MAX_PDWN_IDLE_COUNTER_DEF         (0x00000100)
  #define MC0_CH0_CR_PM_ADAPTIVE_CKE_MAX_PDWN_IDLE_COUNTER_HSH         (0x0C18E01C)

#define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_REG                            (0x0000E020)
#define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_DEF                            (0x19191919)
#define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_VOLATILE_BITFIELDS_MSK         (0x00000000)

  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_0_latency_OFF           ( 0)
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_0_latency_WID           ( 8)
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_0_latency_MSK           (0x000000FF)
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_0_latency_MIN           (19)
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_0_latency_MAX           (120) // 0x00000078
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_0_latency_DEF           (0x00000019)
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_0_latency_HSH           (0x0800E020)

  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_1_latency_OFF           ( 8)
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_1_latency_WID           ( 8)
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_1_latency_MSK           (0x0000FF00)
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_1_latency_MIN           (19)
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_1_latency_MAX           (120) // 0x00000078
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_1_latency_DEF           (0x00000019)
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_1_latency_HSH           (0x0810E020)

  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_2_latency_OFF           (16)
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_2_latency_WID           ( 8)
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_2_latency_MSK           (0x00FF0000)
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_2_latency_MIN           (19)
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_2_latency_MAX           (120) // 0x00000078
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_2_latency_DEF           (0x00000019)
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_2_latency_HSH           (0x0820E020)

  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_3_latency_OFF           (24)
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_3_latency_WID           ( 8)
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_3_latency_MSK           (0xFF000000)
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_3_latency_MIN           (19)
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_3_latency_MAX           (120) // 0x00000078
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_3_latency_DEF           (0x00000019)
  #define MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_Rank_3_latency_HSH           (0x0830E020)

#define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_REG                            (0x0000E024)
#define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_DEF                            (0x00000010)
#define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_VOLATILE_BITFIELDS_MSK         (0x00000000)

  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_TT_idle_counter_OFF          ( 0)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_TT_idle_counter_WID          ( 8)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_TT_idle_counter_MSK          (0x000000FF)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_TT_idle_counter_MIN          (0)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_TT_idle_counter_MAX          (255) // 0x000000FF
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_TT_idle_counter_DEF          (0x00000010)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_TT_idle_counter_HSH          (0x0800E024)

  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_Global_PD_OFF                ( 8)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_Global_PD_WID                ( 1)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_Global_PD_MSK                (0x00000100)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_Global_PD_MIN                (0)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_Global_PD_MAX                (1) // 0x00000001
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_Global_PD_DEF                (0x00000000)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_Global_PD_HSH                (0x0110E024)

  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_dis_cke_tt_OFF               ( 9)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_dis_cke_tt_WID               ( 1)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_dis_cke_tt_MSK               (0x00000200)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_dis_cke_tt_MIN               (0)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_dis_cke_tt_MAX               (1) // 0x00000001
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_dis_cke_tt_DEF               (0x00000000)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_dis_cke_tt_HSH               (0x0112E024)

  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_allow_opp_ref_below_write_threhold_OFF (10)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_allow_opp_ref_below_write_threhold_WID ( 1)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_allow_opp_ref_below_write_threhold_MSK (0x00000400)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_allow_opp_ref_below_write_threhold_MIN (0)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_allow_opp_ref_below_write_threhold_MAX (1) // 0x00000001
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_allow_opp_ref_below_write_threhold_DEF (0x00000000)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_allow_opp_ref_below_write_threhold_HSH (0x0114E024)

#define MC0_CH0_CR_SCHED_CBIT_REG                                      (0x0000E028)
#define MC0_CH0_CR_SCHED_CBIT_DEF                                      (0x00000000)
#define MC0_CH0_CR_SCHED_CBIT_VOLATILE_BITFIELDS_MSK                   (0x00000000)

  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_cas_OFF                        ( 0)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_cas_WID                        ( 1)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_cas_MSK                        (0x00000001)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_cas_MIN                        (0)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_cas_MAX                        (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_cas_DEF                        (0x00000000)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_cas_HSH                        (0x0100E028)

  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_is_cas_OFF                     ( 1)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_is_cas_WID                     ( 1)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_is_cas_MSK                     (0x00000002)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_is_cas_MIN                     (0)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_is_cas_MAX                     (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_is_cas_DEF                     (0x00000000)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_is_cas_HSH                     (0x0102E028)

  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_ras_OFF                        ( 2)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_ras_WID                        ( 1)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_ras_MSK                        (0x00000004)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_ras_MIN                        (0)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_ras_MAX                        (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_ras_DEF                        (0x00000000)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_ras_HSH                        (0x0104E028)

  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_is_ras_OFF                     ( 3)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_is_ras_WID                     ( 1)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_is_ras_MSK                     (0x00000008)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_is_ras_MIN                     (0)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_is_ras_MAX                     (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_is_ras_DEF                     (0x00000000)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_is_ras_HSH                     (0x0106E028)

  #define MC0_CH0_CR_SCHED_CBIT_dis_2c_byp_OFF                         ( 4)
  #define MC0_CH0_CR_SCHED_CBIT_dis_2c_byp_WID                         ( 1)
  #define MC0_CH0_CR_SCHED_CBIT_dis_2c_byp_MSK                         (0x00000010)
  #define MC0_CH0_CR_SCHED_CBIT_dis_2c_byp_MIN                         (0)
  #define MC0_CH0_CR_SCHED_CBIT_dis_2c_byp_MAX                         (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_CBIT_dis_2c_byp_DEF                         (0x00000000)
  #define MC0_CH0_CR_SCHED_CBIT_dis_2c_byp_HSH                         (0x0108E028)

  #define MC0_CH0_CR_SCHED_CBIT_dis_pt_it_OFF                          ( 6)
  #define MC0_CH0_CR_SCHED_CBIT_dis_pt_it_WID                          ( 1)
  #define MC0_CH0_CR_SCHED_CBIT_dis_pt_it_MSK                          (0x00000040)
  #define MC0_CH0_CR_SCHED_CBIT_dis_pt_it_MIN                          (0)
  #define MC0_CH0_CR_SCHED_CBIT_dis_pt_it_MAX                          (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_CBIT_dis_pt_it_DEF                          (0x00000000)
  #define MC0_CH0_CR_SCHED_CBIT_dis_pt_it_HSH                          (0x010CE028)

  #define MC0_CH0_CR_SCHED_CBIT_dis_prcnt_normal_OFF                   ( 7)
  #define MC0_CH0_CR_SCHED_CBIT_dis_prcnt_normal_WID                   ( 1)
  #define MC0_CH0_CR_SCHED_CBIT_dis_prcnt_normal_MSK                   (0x00000080)
  #define MC0_CH0_CR_SCHED_CBIT_dis_prcnt_normal_MIN                   (0)
  #define MC0_CH0_CR_SCHED_CBIT_dis_prcnt_normal_MAX                   (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_CBIT_dis_prcnt_normal_DEF                   (0x00000000)
  #define MC0_CH0_CR_SCHED_CBIT_dis_prcnt_normal_HSH                   (0x010EE028)

  #define MC0_CH0_CR_SCHED_CBIT_dis_prcnt_latency_tolerant_OFF         ( 8)
  #define MC0_CH0_CR_SCHED_CBIT_dis_prcnt_latency_tolerant_WID         ( 1)
  #define MC0_CH0_CR_SCHED_CBIT_dis_prcnt_latency_tolerant_MSK         (0x00000100)
  #define MC0_CH0_CR_SCHED_CBIT_dis_prcnt_latency_tolerant_MIN         (0)
  #define MC0_CH0_CR_SCHED_CBIT_dis_prcnt_latency_tolerant_MAX         (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_CBIT_dis_prcnt_latency_tolerant_DEF         (0x00000000)
  #define MC0_CH0_CR_SCHED_CBIT_dis_prcnt_latency_tolerant_HSH         (0x0110E028)

  #define MC0_CH0_CR_SCHED_CBIT_dis_blkr_ph_OFF                        ( 9)
  #define MC0_CH0_CR_SCHED_CBIT_dis_blkr_ph_WID                        ( 1)
  #define MC0_CH0_CR_SCHED_CBIT_dis_blkr_ph_MSK                        (0x00000200)
  #define MC0_CH0_CR_SCHED_CBIT_dis_blkr_ph_MIN                        (0)
  #define MC0_CH0_CR_SCHED_CBIT_dis_blkr_ph_MAX                        (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_CBIT_dis_blkr_ph_DEF                        (0x00000000)
  #define MC0_CH0_CR_SCHED_CBIT_dis_blkr_ph_HSH                        (0x0112E028)

  #define MC0_CH0_CR_SCHED_CBIT_dis_blkr_pe_OFF                        (10)
  #define MC0_CH0_CR_SCHED_CBIT_dis_blkr_pe_WID                        ( 1)
  #define MC0_CH0_CR_SCHED_CBIT_dis_blkr_pe_MSK                        (0x00000400)
  #define MC0_CH0_CR_SCHED_CBIT_dis_blkr_pe_MIN                        (0)
  #define MC0_CH0_CR_SCHED_CBIT_dis_blkr_pe_MAX                        (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_CBIT_dis_blkr_pe_DEF                        (0x00000000)
  #define MC0_CH0_CR_SCHED_CBIT_dis_blkr_pe_HSH                        (0x0114E028)

  #define MC0_CH0_CR_SCHED_CBIT_dis_blkr_pm_OFF                        (11)
  #define MC0_CH0_CR_SCHED_CBIT_dis_blkr_pm_WID                        ( 1)
  #define MC0_CH0_CR_SCHED_CBIT_dis_blkr_pm_MSK                        (0x00000800)
  #define MC0_CH0_CR_SCHED_CBIT_dis_blkr_pm_MIN                        (0)
  #define MC0_CH0_CR_SCHED_CBIT_dis_blkr_pm_MAX                        (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_CBIT_dis_blkr_pm_DEF                        (0x00000000)
  #define MC0_CH0_CR_SCHED_CBIT_dis_blkr_pm_HSH                        (0x0116E028)

  #define MC0_CH0_CR_SCHED_CBIT_OE_alw_off_OFF                         (13)
  #define MC0_CH0_CR_SCHED_CBIT_OE_alw_off_WID                         ( 1)
  #define MC0_CH0_CR_SCHED_CBIT_OE_alw_off_MSK                         (0x00002000)
  #define MC0_CH0_CR_SCHED_CBIT_OE_alw_off_MIN                         (0)
  #define MC0_CH0_CR_SCHED_CBIT_OE_alw_off_MAX                         (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_CBIT_OE_alw_off_DEF                         (0x00000000)
  #define MC0_CH0_CR_SCHED_CBIT_OE_alw_off_HSH                         (0x011AE028)

  #define MC0_CH0_CR_SCHED_CBIT_block_rpq_OFF                          (14)
  #define MC0_CH0_CR_SCHED_CBIT_block_rpq_WID                          ( 1)
  #define MC0_CH0_CR_SCHED_CBIT_block_rpq_MSK                          (0x00004000)
  #define MC0_CH0_CR_SCHED_CBIT_block_rpq_MIN                          (0)
  #define MC0_CH0_CR_SCHED_CBIT_block_rpq_MAX                          (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_CBIT_block_rpq_DEF                          (0x00000000)
  #define MC0_CH0_CR_SCHED_CBIT_block_rpq_HSH                          (0x011CE028)

  #define MC0_CH0_CR_SCHED_CBIT_block_ipq_OFF                          (15)
  #define MC0_CH0_CR_SCHED_CBIT_block_ipq_WID                          ( 1)
  #define MC0_CH0_CR_SCHED_CBIT_block_ipq_MSK                          (0x00008000)
  #define MC0_CH0_CR_SCHED_CBIT_block_ipq_MIN                          (0)
  #define MC0_CH0_CR_SCHED_CBIT_block_ipq_MAX                          (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_CBIT_block_ipq_DEF                          (0x00000000)
  #define MC0_CH0_CR_SCHED_CBIT_block_ipq_HSH                          (0x011EE028)

  #define MC0_CH0_CR_SCHED_CBIT_block_wpq_OFF                          (16)
  #define MC0_CH0_CR_SCHED_CBIT_block_wpq_WID                          ( 1)
  #define MC0_CH0_CR_SCHED_CBIT_block_wpq_MSK                          (0x00010000)
  #define MC0_CH0_CR_SCHED_CBIT_block_wpq_MIN                          (0)
  #define MC0_CH0_CR_SCHED_CBIT_block_wpq_MAX                          (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_CBIT_block_wpq_DEF                          (0x00000000)
  #define MC0_CH0_CR_SCHED_CBIT_block_wpq_HSH                          (0x0120E028)

  #define MC0_CH0_CR_SCHED_CBIT_dis_tt_OFF                             (18)
  #define MC0_CH0_CR_SCHED_CBIT_dis_tt_WID                             ( 1)
  #define MC0_CH0_CR_SCHED_CBIT_dis_tt_MSK                             (0x00040000)
  #define MC0_CH0_CR_SCHED_CBIT_dis_tt_MIN                             (0)
  #define MC0_CH0_CR_SCHED_CBIT_dis_tt_MAX                             (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_CBIT_dis_tt_DEF                             (0x00000000)
  #define MC0_CH0_CR_SCHED_CBIT_dis_tt_HSH                             (0x0124E028)

  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_ref_OFF                        (19)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_ref_WID                        ( 1)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_ref_MSK                        (0x00080000)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_ref_MIN                        (0)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_ref_MAX                        (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_ref_DEF                        (0x00000000)
  #define MC0_CH0_CR_SCHED_CBIT_dis_opp_ref_HSH                        (0x0126E028)

  #define MC0_CH0_CR_SCHED_CBIT_dis_starved_read_rank_block_OFF        (24)
  #define MC0_CH0_CR_SCHED_CBIT_dis_starved_read_rank_block_WID        ( 1)
  #define MC0_CH0_CR_SCHED_CBIT_dis_starved_read_rank_block_MSK        (0x01000000)
  #define MC0_CH0_CR_SCHED_CBIT_dis_starved_read_rank_block_MIN        (0)
  #define MC0_CH0_CR_SCHED_CBIT_dis_starved_read_rank_block_MAX        (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_CBIT_dis_starved_read_rank_block_DEF        (0x00000000)
  #define MC0_CH0_CR_SCHED_CBIT_dis_starved_read_rank_block_HSH        (0x0130E028)

  #define MC0_CH0_CR_SCHED_CBIT_find_first_allocation_OFF              (26)
  #define MC0_CH0_CR_SCHED_CBIT_find_first_allocation_WID              ( 1)
  #define MC0_CH0_CR_SCHED_CBIT_find_first_allocation_MSK              (0x04000000)
  #define MC0_CH0_CR_SCHED_CBIT_find_first_allocation_MIN              (0)
  #define MC0_CH0_CR_SCHED_CBIT_find_first_allocation_MAX              (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_CBIT_find_first_allocation_DEF              (0x00000000)
  #define MC0_CH0_CR_SCHED_CBIT_find_first_allocation_HSH              (0x0134E028)

  #define MC0_CH0_CR_SCHED_CBIT_dis_clk_gate_OFF                       (31)
  #define MC0_CH0_CR_SCHED_CBIT_dis_clk_gate_WID                       ( 1)
  #define MC0_CH0_CR_SCHED_CBIT_dis_clk_gate_MSK                       (0x80000000)
  #define MC0_CH0_CR_SCHED_CBIT_dis_clk_gate_MIN                       (0)
  #define MC0_CH0_CR_SCHED_CBIT_dis_clk_gate_MAX                       (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_CBIT_dis_clk_gate_DEF                       (0x00000000)
  #define MC0_CH0_CR_SCHED_CBIT_dis_clk_gate_HSH                       (0x013EE028)

#define MC0_CH0_CR_SCHED_SECOND_CBIT_REG                               (0x0000E02C)
#define MC0_CH0_CR_SCHED_SECOND_CBIT_DEF                               (0x02000200)
#define MC0_CH0_CR_SCHED_SECOND_CBIT_VOLATILE_BITFIELDS_MSK            (0x00000000)

  #define MC0_CH0_CR_SCHED_SECOND_CBIT_lpddr_sagv_issue_pdx_OFF        ( 0)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_lpddr_sagv_issue_pdx_WID        ( 1)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_lpddr_sagv_issue_pdx_MSK        (0x00000001)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_lpddr_sagv_issue_pdx_MIN        (0)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_lpddr_sagv_issue_pdx_MAX        (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_lpddr_sagv_issue_pdx_DEF        (0x00000000)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_lpddr_sagv_issue_pdx_HSH        (0x0100E02C)

  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_ck_tristate_OFF             ( 1)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_ck_tristate_WID             ( 1)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_ck_tristate_MSK             (0x00000002)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_ck_tristate_MIN             (0)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_ck_tristate_MAX             (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_ck_tristate_DEF             (0x00000000)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_ck_tristate_HSH             (0x0102E02C)

  #define MC0_CH0_CR_SCHED_SECOND_CBIT_revert_to_pre_gear4_cancel_OFF  ( 2)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_revert_to_pre_gear4_cancel_WID  ( 1)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_revert_to_pre_gear4_cancel_MSK  (0x00000004)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_revert_to_pre_gear4_cancel_MIN  (0)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_revert_to_pre_gear4_cancel_MAX  (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_revert_to_pre_gear4_cancel_DEF  (0x00000000)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_revert_to_pre_gear4_cancel_HSH  (0x0104E02C)

  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_bus_retain_OFF              ( 3)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_bus_retain_WID              ( 1)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_bus_retain_MSK              (0x00000008)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_bus_retain_MIN              (0)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_bus_retain_MAX              (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_bus_retain_DEF              (0x00000000)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_bus_retain_HSH              (0x0106E02C)

  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_async_odt_OFF               ( 4)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_async_odt_WID               ( 1)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_async_odt_MSK               (0x00000010)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_async_odt_MIN               (0)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_async_odt_MAX               (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_async_odt_DEF               (0x00000000)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_async_odt_HSH               (0x0108E02C)

  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_trace_valid_on_lpmode_OFF   ( 5)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_trace_valid_on_lpmode_WID   ( 1)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_trace_valid_on_lpmode_MSK   (0x00000020)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_trace_valid_on_lpmode_MIN   (0)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_trace_valid_on_lpmode_MAX   (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_trace_valid_on_lpmode_DEF   (0x00000000)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_trace_valid_on_lpmode_HSH   (0x010AE02C)

  #define MC0_CH0_CR_SCHED_SECOND_CBIT_disable_starved_prio_on_new_req_OFF ( 6)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_disable_starved_prio_on_new_req_WID ( 1)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_disable_starved_prio_on_new_req_MSK (0x00000040)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_disable_starved_prio_on_new_req_MIN (0)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_disable_starved_prio_on_new_req_MAX (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_disable_starved_prio_on_new_req_DEF (0x00000000)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_disable_starved_prio_on_new_req_HSH (0x010CE02C)

  #define MC0_CH0_CR_SCHED_SECOND_CBIT_lpddr_sagv_issue_pde_OFF        ( 7)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_lpddr_sagv_issue_pde_WID        ( 1)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_lpddr_sagv_issue_pde_MSK        (0x00000080)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_lpddr_sagv_issue_pde_MIN        (0)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_lpddr_sagv_issue_pde_MAX        (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_lpddr_sagv_issue_pde_DEF        (0x00000000)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_lpddr_sagv_issue_pde_HSH        (0x010EE02C)

  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_opp_ref_idle_delay_OFF      ( 8)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_opp_ref_idle_delay_WID      ( 1)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_opp_ref_idle_delay_MSK      (0x00000100)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_opp_ref_idle_delay_MIN      (0)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_opp_ref_idle_delay_MAX      (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_opp_ref_idle_delay_DEF      (0x00000000)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_opp_ref_idle_delay_HSH      (0x0110E02C)

  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_ignore_1st_trefi_OFF        ( 9)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_ignore_1st_trefi_WID        ( 1)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_ignore_1st_trefi_MSK        (0x00000200)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_ignore_1st_trefi_MIN        (0)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_ignore_1st_trefi_MAX        (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_ignore_1st_trefi_DEF        (0x00000001)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_ignore_1st_trefi_HSH        (0x0112E02C)

  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_new_wr_to_full_OFF          (20)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_new_wr_to_full_WID          ( 1)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_new_wr_to_full_MSK          (0x00100000)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_new_wr_to_full_MIN          (0)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_new_wr_to_full_MAX          (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_new_wr_to_full_DEF          (0x00000000)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_dis_new_wr_to_full_HSH          (0x0128E02C)

  #define MC0_CH0_CR_SCHED_SECOND_CBIT_cmd_bgf_always_on_OFF           (21)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_cmd_bgf_always_on_WID           ( 1)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_cmd_bgf_always_on_MSK           (0x00200000)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_cmd_bgf_always_on_MIN           (0)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_cmd_bgf_always_on_MAX           (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_cmd_bgf_always_on_DEF           (0x00000000)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_cmd_bgf_always_on_HSH           (0x012AE02C)

  #define MC0_CH0_CR_SCHED_SECOND_CBIT_do_temp_reading_after_drainless_SAGV_OFF (23)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_do_temp_reading_after_drainless_SAGV_WID ( 1)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_do_temp_reading_after_drainless_SAGV_MSK (0x00800000)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_do_temp_reading_after_drainless_SAGV_MIN (0)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_do_temp_reading_after_drainless_SAGV_MAX (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_do_temp_reading_after_drainless_SAGV_DEF (0x00000000)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_do_temp_reading_after_drainless_SAGV_HSH (0x012EE02C)

  #define MC0_CH0_CR_SCHED_SECOND_CBIT_ignore_refresh_between_SR_OFF   (24)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_ignore_refresh_between_SR_WID   ( 1)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_ignore_refresh_between_SR_MSK   (0x01000000)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_ignore_refresh_between_SR_MIN   (0)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_ignore_refresh_between_SR_MAX   (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_ignore_refresh_between_SR_DEF   (0x00000000)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_ignore_refresh_between_SR_HSH   (0x0130E02C)

  #define MC0_CH0_CR_SCHED_SECOND_CBIT_DisWrActThrottleOnAnyRead_OFF   (25)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_DisWrActThrottleOnAnyRead_WID   ( 1)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_DisWrActThrottleOnAnyRead_MSK   (0x02000000)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_DisWrActThrottleOnAnyRead_MIN   (0)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_DisWrActThrottleOnAnyRead_MAX   (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_DisWrActThrottleOnAnyRead_DEF   (0x00000001)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_DisWrActThrottleOnAnyRead_HSH   (0x0132E02C)

  #define MC0_CH0_CR_SCHED_SECOND_CBIT_disable_cas_clubbing_OFF        (29)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_disable_cas_clubbing_WID        ( 1)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_disable_cas_clubbing_MSK        (0x20000000)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_disable_cas_clubbing_MIN        (0)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_disable_cas_clubbing_MAX        (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_disable_cas_clubbing_DEF        (0x00000000)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_disable_cas_clubbing_HSH        (0x013AE02C)

  #define MC0_CH0_CR_SCHED_SECOND_CBIT_disable_starv2med_prio_on_new_req_OFF (31)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_disable_starv2med_prio_on_new_req_WID ( 1)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_disable_starv2med_prio_on_new_req_MSK (0x80000000)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_disable_starv2med_prio_on_new_req_MIN (0)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_disable_starv2med_prio_on_new_req_MAX (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_disable_starv2med_prio_on_new_req_DEF (0x00000000)
  #define MC0_CH0_CR_SCHED_SECOND_CBIT_disable_starv2med_prio_on_new_req_HSH (0x013EE02C)

#define MC0_CH0_CR_DFT_MISC_REG                                        (0x0000E030)
#define MC0_CH0_CR_DFT_MISC_DEF                                        (0x00000000)
#define MC0_CH0_CR_DFT_MISC_VOLATILE_BITFIELDS_MSK                     (0x00020000)

  #define MC0_CH0_CR_DFT_MISC_WDB_Block_En_OFF                         ( 1)
  #define MC0_CH0_CR_DFT_MISC_WDB_Block_En_WID                         ( 1)
  #define MC0_CH0_CR_DFT_MISC_WDB_Block_En_MSK                         (0x00000002)
  #define MC0_CH0_CR_DFT_MISC_WDB_Block_En_MIN                         (0)
  #define MC0_CH0_CR_DFT_MISC_WDB_Block_En_MAX                         (1) // 0x00000001
  #define MC0_CH0_CR_DFT_MISC_WDB_Block_En_DEF                         (0x00000000)
  #define MC0_CH0_CR_DFT_MISC_WDB_Block_En_HSH                         (0x0102E030)

  #define MC0_CH0_CR_DFT_MISC_isolate_data_return_OFF                  ( 4)
  #define MC0_CH0_CR_DFT_MISC_isolate_data_return_WID                  ( 1)
  #define MC0_CH0_CR_DFT_MISC_isolate_data_return_MSK                  (0x00000010)
  #define MC0_CH0_CR_DFT_MISC_isolate_data_return_MIN                  (0)
  #define MC0_CH0_CR_DFT_MISC_isolate_data_return_MAX                  (1) // 0x00000001
  #define MC0_CH0_CR_DFT_MISC_isolate_data_return_DEF                  (0x00000000)
  #define MC0_CH0_CR_DFT_MISC_isolate_data_return_HSH                  (0x0108E030)

  #define MC0_CH0_CR_DFT_MISC_dft_block_enable_OFF                     (17)
  #define MC0_CH0_CR_DFT_MISC_dft_block_enable_WID                     ( 1)
  #define MC0_CH0_CR_DFT_MISC_dft_block_enable_MSK                     (0x00020000)
  #define MC0_CH0_CR_DFT_MISC_dft_block_enable_MIN                     (0)
  #define MC0_CH0_CR_DFT_MISC_dft_block_enable_MAX                     (1) // 0x00000001
  #define MC0_CH0_CR_DFT_MISC_dft_block_enable_DEF                     (0x00000000)
  #define MC0_CH0_CR_DFT_MISC_dft_block_enable_HSH                     (0x01A2E030)

  #define MC0_CH0_CR_DFT_MISC_SAM_overloading_OFF                      (18)
  #define MC0_CH0_CR_DFT_MISC_SAM_overloading_WID                      ( 3)
  #define MC0_CH0_CR_DFT_MISC_SAM_overloading_MSK                      (0x001C0000)
  #define MC0_CH0_CR_DFT_MISC_SAM_overloading_MIN                      (0)
  #define MC0_CH0_CR_DFT_MISC_SAM_overloading_MAX                      (7) // 0x00000007
  #define MC0_CH0_CR_DFT_MISC_SAM_overloading_DEF                      (0x00000000)
  #define MC0_CH0_CR_DFT_MISC_SAM_overloading_HSH                      (0x0324E030)

  #define MC0_CH0_CR_DFT_MISC_ipq_wcdp_act_resources_OFF               (21)
  #define MC0_CH0_CR_DFT_MISC_ipq_wcdp_act_resources_WID               ( 4)
  #define MC0_CH0_CR_DFT_MISC_ipq_wcdp_act_resources_MSK               (0x01E00000)
  #define MC0_CH0_CR_DFT_MISC_ipq_wcdp_act_resources_MIN               (0)
  #define MC0_CH0_CR_DFT_MISC_ipq_wcdp_act_resources_MAX               (15) // 0x0000000F
  #define MC0_CH0_CR_DFT_MISC_ipq_wcdp_act_resources_DEF               (0x00000000)
  #define MC0_CH0_CR_DFT_MISC_ipq_wcdp_act_resources_HSH               (0x042AE030)

#define MC0_CH0_CR_SC_PCIT_REG                                         (0x0000E034)
#define MC0_CH0_CR_SC_PCIT_DEF                                         (0x04010040)
#define MC0_CH0_CR_SC_PCIT_VOLATILE_BITFIELDS_MSK                      (0x000FFFFF)

  #define MC0_CH0_CR_SC_PCIT_PCIT_SUBCH0_OFF                           ( 0)
  #define MC0_CH0_CR_SC_PCIT_PCIT_SUBCH0_WID                           (10)
  #define MC0_CH0_CR_SC_PCIT_PCIT_SUBCH0_MSK                           (0x000003FF)
  #define MC0_CH0_CR_SC_PCIT_PCIT_SUBCH0_MIN                           (0)
  #define MC0_CH0_CR_SC_PCIT_PCIT_SUBCH0_MAX                           (1023) // 0x000003FF
  #define MC0_CH0_CR_SC_PCIT_PCIT_SUBCH0_DEF                           (0x00000040)
  #define MC0_CH0_CR_SC_PCIT_PCIT_SUBCH0_HSH                           (0x0A80E034)

  #define MC0_CH0_CR_SC_PCIT_PCIT_SUBCH1_OFF                           (10)
  #define MC0_CH0_CR_SC_PCIT_PCIT_SUBCH1_WID                           (10)
  #define MC0_CH0_CR_SC_PCIT_PCIT_SUBCH1_MSK                           (0x000FFC00)
  #define MC0_CH0_CR_SC_PCIT_PCIT_SUBCH1_MIN                           (0)
  #define MC0_CH0_CR_SC_PCIT_PCIT_SUBCH1_MAX                           (1023) // 0x000003FF
  #define MC0_CH0_CR_SC_PCIT_PCIT_SUBCH1_DEF                           (0x00000040)
  #define MC0_CH0_CR_SC_PCIT_PCIT_SUBCH1_HSH                           (0x0A94E034)

  #define MC0_CH0_CR_SC_PCIT_PCIT_GT_OFF                               (20)
  #define MC0_CH0_CR_SC_PCIT_PCIT_GT_WID                               (10)
  #define MC0_CH0_CR_SC_PCIT_PCIT_GT_MSK                               (0x3FF00000)
  #define MC0_CH0_CR_SC_PCIT_PCIT_GT_MIN                               (0)
  #define MC0_CH0_CR_SC_PCIT_PCIT_GT_MAX                               (1023) // 0x000003FF
  #define MC0_CH0_CR_SC_PCIT_PCIT_GT_DEF                               (0x00000040)
  #define MC0_CH0_CR_SC_PCIT_PCIT_GT_HSH                               (0x0A28E034)

#define MC0_CH0_CR_ECC_DFT_REG                                         (0x0000E038)
#define MC0_CH0_CR_ECC_DFT_DEF                                         (0x00000000)
#define MC0_CH0_CR_ECC_DFT_VOLATILE_BITFIELDS_MSK                      (0x00000000)

  #define MC0_CH0_CR_ECC_DFT_ECC_Inject_OFF                            ( 0)
  #define MC0_CH0_CR_ECC_DFT_ECC_Inject_WID                            ( 3)
  #define MC0_CH0_CR_ECC_DFT_ECC_Inject_MSK                            (0x00000007)
  #define MC0_CH0_CR_ECC_DFT_ECC_Inject_MIN                            (0)
  #define MC0_CH0_CR_ECC_DFT_ECC_Inject_MAX                            (7) // 0x00000007
  #define MC0_CH0_CR_ECC_DFT_ECC_Inject_DEF                            (0x00000000)
  #define MC0_CH0_CR_ECC_DFT_ECC_Inject_HSH                            (0x0300E038)

  #define MC0_CH0_CR_ECC_DFT_ECC_correction_disable_OFF                ( 4)
  #define MC0_CH0_CR_ECC_DFT_ECC_correction_disable_WID                ( 1)
  #define MC0_CH0_CR_ECC_DFT_ECC_correction_disable_MSK                (0x00000010)
  #define MC0_CH0_CR_ECC_DFT_ECC_correction_disable_MIN                (0)
  #define MC0_CH0_CR_ECC_DFT_ECC_correction_disable_MAX                (1) // 0x00000001
  #define MC0_CH0_CR_ECC_DFT_ECC_correction_disable_DEF                (0x00000000)
  #define MC0_CH0_CR_ECC_DFT_ECC_correction_disable_HSH                (0x0108E038)

#define MC0_CH0_CR_ECC_DFT_UARCH_REG                                   (0x0000E03C)
#define MC0_CH0_CR_ECC_DFT_UARCH_DEF                                   (0x0000000C)
#define MC0_CH0_CR_ECC_DFT_UARCH_VOLATILE_BITFIELDS_MSK                (0x00000000)

  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_MCA_LOG_OFF                     ( 0)
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_MCA_LOG_WID                     ( 1)
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_MCA_LOG_MSK                     (0x00000001)
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_MCA_LOG_MIN                     (0)
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_MCA_LOG_MAX                     (1) // 0x00000001
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_MCA_LOG_DEF                     (0x00000000)
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_MCA_LOG_HSH                     (0x0100E03C)

  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_PCIE_ERR_OFF                    ( 1)
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_PCIE_ERR_WID                    ( 1)
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_PCIE_ERR_MSK                    (0x00000002)
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_PCIE_ERR_MIN                    (0)
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_PCIE_ERR_MAX                    (1) // 0x00000001
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_PCIE_ERR_DEF                    (0x00000000)
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_PCIE_ERR_HSH                    (0x0102E03C)

  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_RCH_POISON_OFF                  ( 2)
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_RCH_POISON_WID                  ( 1)
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_RCH_POISON_MSK                  (0x00000004)
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_RCH_POISON_MIN                  (0)
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_RCH_POISON_MAX                  (1) // 0x00000001
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_RCH_POISON_DEF                  (0x00000001)
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_RCH_POISON_HSH                  (0x0104E03C)

  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_RCH_ERROR_OFF                   ( 3)
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_RCH_ERROR_WID                   ( 1)
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_RCH_ERROR_MSK                   (0x00000008)
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_RCH_ERROR_MIN                   (0)
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_RCH_ERROR_MAX                   (1) // 0x00000001
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_RCH_ERROR_DEF                   (0x00000001)
  #define MC0_CH0_CR_ECC_DFT_UARCH_DIS_RCH_ERROR_HSH                   (0x0106E03C)

  #define MC0_CH0_CR_ECC_DFT_UARCH_ECC_trigger_OFF                     ( 4)
  #define MC0_CH0_CR_ECC_DFT_UARCH_ECC_trigger_WID                     ( 2)
  #define MC0_CH0_CR_ECC_DFT_UARCH_ECC_trigger_MSK                     (0x00000030)
  #define MC0_CH0_CR_ECC_DFT_UARCH_ECC_trigger_MIN                     (0)
  #define MC0_CH0_CR_ECC_DFT_UARCH_ECC_trigger_MAX                     (3) // 0x00000003
  #define MC0_CH0_CR_ECC_DFT_UARCH_ECC_trigger_DEF                     (0x00000000)
  #define MC0_CH0_CR_ECC_DFT_UARCH_ECC_trigger_HSH                     (0x0208E03C)

#define MC0_CH0_CR_PM_PDWN_CONFIG_REG                                  (0x0000E040)
#define MC0_CH0_CR_PM_PDWN_CONFIG_DEF                                  (0x0000800003040040)
#define MC0_CH0_CR_PM_PDWN_CONFIG_VOLATILE_BITFIELDS_MSK               (0x0000000000FFFFFF)

  #define MC0_CH0_CR_PM_PDWN_CONFIG_PDWN_idle_counter_subch0_OFF       ( 0)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_PDWN_idle_counter_subch0_WID       (12)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_PDWN_idle_counter_subch0_MSK       (0x00000FFF)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_PDWN_idle_counter_subch0_MIN       (0)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_PDWN_idle_counter_subch0_MAX       (4095) // 0x00000FFF
  #define MC0_CH0_CR_PM_PDWN_CONFIG_PDWN_idle_counter_subch0_DEF       (0x00000040)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_PDWN_idle_counter_subch0_HSH       (0x4C80E040)

  #define MC0_CH0_CR_PM_PDWN_CONFIG_PDWN_idle_counter_subch1_OFF       (12)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_PDWN_idle_counter_subch1_WID       (12)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_PDWN_idle_counter_subch1_MSK       (0x00FFF000)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_PDWN_idle_counter_subch1_MIN       (0)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_PDWN_idle_counter_subch1_MAX       (4095) // 0x00000FFF
  #define MC0_CH0_CR_PM_PDWN_CONFIG_PDWN_idle_counter_subch1_DEF       (0x00000040)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_PDWN_idle_counter_subch1_HSH       (0x4C98E040)

  #define MC0_CH0_CR_PM_PDWN_CONFIG_APD_OFF                            (24)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_APD_WID                            ( 1)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_APD_MSK                            (0x01000000)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_APD_MIN                            (0)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_APD_MAX                            (1) // 0x00000001
  #define MC0_CH0_CR_PM_PDWN_CONFIG_APD_DEF                            (0x00000001)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_APD_HSH                            (0x4130E040)

  #define MC0_CH0_CR_PM_PDWN_CONFIG_PPD_OFF                            (25)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_PPD_WID                            ( 1)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_PPD_MSK                            (0x02000000)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_PPD_MIN                            (0)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_PPD_MAX                            (1) // 0x00000001
  #define MC0_CH0_CR_PM_PDWN_CONFIG_PPD_DEF                            (0x00000001)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_PPD_HSH                            (0x4132E040)

  #define MC0_CH0_CR_PM_PDWN_CONFIG_write_threshold_OFF                (43)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_write_threshold_WID                ( 7)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_write_threshold_MSK                (0x0003F80000000000ULL)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_write_threshold_MIN                (0)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_write_threshold_MAX                (95) // 0x0000005F
  #define MC0_CH0_CR_PM_PDWN_CONFIG_write_threshold_DEF                (0x00000010)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_write_threshold_HSH                (0x4756E040)

  #define MC0_CH0_CR_PM_PDWN_CONFIG_lpmode_enable_OFF                  (50)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_lpmode_enable_WID                  ( 1)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_lpmode_enable_MSK                  (0x0004000000000000ULL)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_lpmode_enable_MIN                  (0)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_lpmode_enable_MAX                  (1) // 0x00000001
  #define MC0_CH0_CR_PM_PDWN_CONFIG_lpmode_enable_DEF                  (0x00000000)
  #define MC0_CH0_CR_PM_PDWN_CONFIG_lpmode_enable_HSH                  (0x4164E040)

#define MC0_CH0_CR_ECCERRLOG0_REG                                      (0x0000E048)
#define MC0_CH0_CR_ECCERRLOG0_DEF                                      (0x00000000)
#define MC0_CH0_CR_ECCERRLOG0_VOLATILE_BITFIELDS_MSK                   (0xFFFF000F)

  #define MC0_CH0_CR_ECCERRLOG0_CERRSTS_OFF                            ( 0)
  #define MC0_CH0_CR_ECCERRLOG0_CERRSTS_WID                            ( 1)
  #define MC0_CH0_CR_ECCERRLOG0_CERRSTS_MSK                            (0x00000001)
  #define MC0_CH0_CR_ECCERRLOG0_CERRSTS_MIN                            (0)
  #define MC0_CH0_CR_ECCERRLOG0_CERRSTS_MAX                            (1) // 0x00000001
  #define MC0_CH0_CR_ECCERRLOG0_CERRSTS_DEF                            (0x00000000)
  #define MC0_CH0_CR_ECCERRLOG0_CERRSTS_HSH                            (0x0180E048)

  #define MC0_CH0_CR_ECCERRLOG0_CERR_OVERFLOW_OFF                      ( 1)
  #define MC0_CH0_CR_ECCERRLOG0_CERR_OVERFLOW_WID                      ( 1)
  #define MC0_CH0_CR_ECCERRLOG0_CERR_OVERFLOW_MSK                      (0x00000002)
  #define MC0_CH0_CR_ECCERRLOG0_CERR_OVERFLOW_MIN                      (0)
  #define MC0_CH0_CR_ECCERRLOG0_CERR_OVERFLOW_MAX                      (1) // 0x00000001
  #define MC0_CH0_CR_ECCERRLOG0_CERR_OVERFLOW_DEF                      (0x00000000)
  #define MC0_CH0_CR_ECCERRLOG0_CERR_OVERFLOW_HSH                      (0x0182E048)

  #define MC0_CH0_CR_ECCERRLOG0_MERRSTS_OFF                            ( 2)
  #define MC0_CH0_CR_ECCERRLOG0_MERRSTS_WID                            ( 1)
  #define MC0_CH0_CR_ECCERRLOG0_MERRSTS_MSK                            (0x00000004)
  #define MC0_CH0_CR_ECCERRLOG0_MERRSTS_MIN                            (0)
  #define MC0_CH0_CR_ECCERRLOG0_MERRSTS_MAX                            (1) // 0x00000001
  #define MC0_CH0_CR_ECCERRLOG0_MERRSTS_DEF                            (0x00000000)
  #define MC0_CH0_CR_ECCERRLOG0_MERRSTS_HSH                            (0x0184E048)

  #define MC0_CH0_CR_ECCERRLOG0_MERR_OVERFLOW_OFF                      ( 3)
  #define MC0_CH0_CR_ECCERRLOG0_MERR_OVERFLOW_WID                      ( 1)
  #define MC0_CH0_CR_ECCERRLOG0_MERR_OVERFLOW_MSK                      (0x00000008)
  #define MC0_CH0_CR_ECCERRLOG0_MERR_OVERFLOW_MIN                      (0)
  #define MC0_CH0_CR_ECCERRLOG0_MERR_OVERFLOW_MAX                      (1) // 0x00000001
  #define MC0_CH0_CR_ECCERRLOG0_MERR_OVERFLOW_DEF                      (0x00000000)
  #define MC0_CH0_CR_ECCERRLOG0_MERR_OVERFLOW_HSH                      (0x0186E048)

  #define MC0_CH0_CR_ECCERRLOG0_ERRSYND_OFF                            (16)
  #define MC0_CH0_CR_ECCERRLOG0_ERRSYND_WID                            ( 8)
  #define MC0_CH0_CR_ECCERRLOG0_ERRSYND_MSK                            (0x00FF0000)
  #define MC0_CH0_CR_ECCERRLOG0_ERRSYND_MIN                            (0)
  #define MC0_CH0_CR_ECCERRLOG0_ERRSYND_MAX                            (255) // 0x000000FF
  #define MC0_CH0_CR_ECCERRLOG0_ERRSYND_DEF                            (0x00000000)
  #define MC0_CH0_CR_ECCERRLOG0_ERRSYND_HSH                            (0x08A0E048)

  #define MC0_CH0_CR_ECCERRLOG0_ERRCHUNK_OFF                           (24)
  #define MC0_CH0_CR_ECCERRLOG0_ERRCHUNK_WID                           ( 3)
  #define MC0_CH0_CR_ECCERRLOG0_ERRCHUNK_MSK                           (0x07000000)
  #define MC0_CH0_CR_ECCERRLOG0_ERRCHUNK_MIN                           (0)
  #define MC0_CH0_CR_ECCERRLOG0_ERRCHUNK_MAX                           (7) // 0x00000007
  #define MC0_CH0_CR_ECCERRLOG0_ERRCHUNK_DEF                           (0x00000000)
  #define MC0_CH0_CR_ECCERRLOG0_ERRCHUNK_HSH                           (0x03B0E048)

  #define MC0_CH0_CR_ECCERRLOG0_ERRRANK_OFF                            (27)
  #define MC0_CH0_CR_ECCERRLOG0_ERRRANK_WID                            ( 2)
  #define MC0_CH0_CR_ECCERRLOG0_ERRRANK_MSK                            (0x18000000)
  #define MC0_CH0_CR_ECCERRLOG0_ERRRANK_MIN                            (0)
  #define MC0_CH0_CR_ECCERRLOG0_ERRRANK_MAX                            (3) // 0x00000003
  #define MC0_CH0_CR_ECCERRLOG0_ERRRANK_DEF                            (0x00000000)
  #define MC0_CH0_CR_ECCERRLOG0_ERRRANK_HSH                            (0x02B6E048)

  #define MC0_CH0_CR_ECCERRLOG0_ERRBANK_OFF                            (29)
  #define MC0_CH0_CR_ECCERRLOG0_ERRBANK_WID                            ( 3)
  #define MC0_CH0_CR_ECCERRLOG0_ERRBANK_MSK                            (0xE0000000)
  #define MC0_CH0_CR_ECCERRLOG0_ERRBANK_MIN                            (0)
  #define MC0_CH0_CR_ECCERRLOG0_ERRBANK_MAX                            (7) // 0x00000007
  #define MC0_CH0_CR_ECCERRLOG0_ERRBANK_DEF                            (0x00000000)
  #define MC0_CH0_CR_ECCERRLOG0_ERRBANK_HSH                            (0x03BAE048)

#define MC0_CH0_CR_ECCERRLOG1_REG                                      (0x0000E04C)
#define MC0_CH0_CR_ECCERRLOG1_DEF                                      (0x00000000)
#define MC0_CH0_CR_ECCERRLOG1_VOLATILE_BITFIELDS_MSK                   (0x1FFFFFFF)

  #define MC0_CH0_CR_ECCERRLOG1_ERRROW_OFF                             ( 0)
  #define MC0_CH0_CR_ECCERRLOG1_ERRROW_WID                             (18)
  #define MC0_CH0_CR_ECCERRLOG1_ERRROW_MSK                             (0x0003FFFF)
  #define MC0_CH0_CR_ECCERRLOG1_ERRROW_MIN                             (0)
  #define MC0_CH0_CR_ECCERRLOG1_ERRROW_MAX                             (262143) // 0x0003FFFF
  #define MC0_CH0_CR_ECCERRLOG1_ERRROW_DEF                             (0x00000000)
  #define MC0_CH0_CR_ECCERRLOG1_ERRROW_HSH                             (0x1280E04C)

  #define MC0_CH0_CR_ECCERRLOG1_ERRCOL_OFF                             (18)
  #define MC0_CH0_CR_ECCERRLOG1_ERRCOL_WID                             ( 8)
  #define MC0_CH0_CR_ECCERRLOG1_ERRCOL_MSK                             (0x03FC0000)
  #define MC0_CH0_CR_ECCERRLOG1_ERRCOL_MIN                             (0)
  #define MC0_CH0_CR_ECCERRLOG1_ERRCOL_MAX                             (255) // 0x000000FF
  #define MC0_CH0_CR_ECCERRLOG1_ERRCOL_DEF                             (0x00000000)
  #define MC0_CH0_CR_ECCERRLOG1_ERRCOL_HSH                             (0x08A4E04C)

  #define MC0_CH0_CR_ECCERRLOG1_ERRBANKGROUP_OFF                       (26)
  #define MC0_CH0_CR_ECCERRLOG1_ERRBANKGROUP_WID                       ( 3)
  #define MC0_CH0_CR_ECCERRLOG1_ERRBANKGROUP_MSK                       (0x1C000000)
  #define MC0_CH0_CR_ECCERRLOG1_ERRBANKGROUP_MIN                       (0)
  #define MC0_CH0_CR_ECCERRLOG1_ERRBANKGROUP_MAX                       (7) // 0x00000007
  #define MC0_CH0_CR_ECCERRLOG1_ERRBANKGROUP_DEF                       (0x00000000)
  #define MC0_CH0_CR_ECCERRLOG1_ERRBANKGROUP_HSH                       (0x03B4E04C)

#define MC0_CH0_CR_TC_PWRDN_REG                                        (0x0000E050)
#define MC0_CH0_CR_TC_PWRDN_DEF                                        (0x0904110020204204)
#define MC0_CH0_CR_TC_PWRDN_VOLATILE_BITFIELDS_MSK                     (0x0000000000000000)

  #define MC0_CH0_CR_TC_PWRDN_tcke_OFF                                 ( 0)
  #define MC0_CH0_CR_TC_PWRDN_tcke_WID                                 ( 7)
  #define MC0_CH0_CR_TC_PWRDN_tcke_MSK                                 (0x0000007F)
  #define MC0_CH0_CR_TC_PWRDN_tcke_MIN                                 (4)
  #define MC0_CH0_CR_TC_PWRDN_tcke_MAX                                 (36) // 0x00000024
  #define MC0_CH0_CR_TC_PWRDN_tcke_DEF                                 (0x00000004)
  #define MC0_CH0_CR_TC_PWRDN_tcke_HSH                                 (0x4700E050)

  #define MC0_CH0_CR_TC_PWRDN_txp_OFF                                  ( 7)
  #define MC0_CH0_CR_TC_PWRDN_txp_WID                                  ( 7)
  #define MC0_CH0_CR_TC_PWRDN_txp_MSK                                  (0x00003F80)
  #define MC0_CH0_CR_TC_PWRDN_txp_MIN                                  (4)
  #define MC0_CH0_CR_TC_PWRDN_txp_MAX                                  (35) // 0x00000023
  #define MC0_CH0_CR_TC_PWRDN_txp_DEF                                  (0x00000004)
  #define MC0_CH0_CR_TC_PWRDN_txp_HSH                                  (0x470EE050)

  #define MC0_CH0_CR_TC_PWRDN_tcpded_OFF                               (14)
  #define MC0_CH0_CR_TC_PWRDN_tcpded_WID                               ( 5)
  #define MC0_CH0_CR_TC_PWRDN_tcpded_MSK                               (0x0007C000)
  #define MC0_CH0_CR_TC_PWRDN_tcpded_MIN                               (0)
  #define MC0_CH0_CR_TC_PWRDN_tcpded_MAX                               (23) // 0x00000017
  #define MC0_CH0_CR_TC_PWRDN_tcpded_DEF                               (0x00000001)
  #define MC0_CH0_CR_TC_PWRDN_tcpded_HSH                               (0x451CE050)

  #define MC0_CH0_CR_TC_PWRDN_trdpden_OFF                              (19)
  #define MC0_CH0_CR_TC_PWRDN_trdpden_WID                              ( 8)
  #define MC0_CH0_CR_TC_PWRDN_trdpden_MSK                              (0x07F80000)
  #define MC0_CH0_CR_TC_PWRDN_trdpden_MIN                              (4)
  #define MC0_CH0_CR_TC_PWRDN_trdpden_MAX                              (108) // 0x0000006C
  #define MC0_CH0_CR_TC_PWRDN_trdpden_DEF                              (0x00000004)
  #define MC0_CH0_CR_TC_PWRDN_trdpden_HSH                              (0x4826E050)

  #define MC0_CH0_CR_TC_PWRDN_twrpden_OFF                              (27)
  #define MC0_CH0_CR_TC_PWRDN_twrpden_WID                              (10)
  #define MC0_CH0_CR_TC_PWRDN_twrpden_MSK                              (0x0000001FF8000000ULL)
  #define MC0_CH0_CR_TC_PWRDN_twrpden_MIN                              (4)
  #define MC0_CH0_CR_TC_PWRDN_twrpden_MAX                              (221) // 0x000000DD
  #define MC0_CH0_CR_TC_PWRDN_twrpden_DEF                              (0x00000004)
  #define MC0_CH0_CR_TC_PWRDN_twrpden_HSH                              (0x4A36E050)

  #define MC0_CH0_CR_TC_PWRDN_tckckeh_OFF                              (37)
  #define MC0_CH0_CR_TC_PWRDN_tckckeh_WID                              ( 5)
  #define MC0_CH0_CR_TC_PWRDN_tckckeh_MSK                              (0x000003E000000000ULL)
  #define MC0_CH0_CR_TC_PWRDN_tckckeh_MIN                              (0)
  #define MC0_CH0_CR_TC_PWRDN_tckckeh_MAX                              (46) // 0x0000002E
  #define MC0_CH0_CR_TC_PWRDN_tckckeh_DEF                              (0x00000008)
  #define MC0_CH0_CR_TC_PWRDN_tckckeh_HSH                              (0x454AE050)

  #define MC0_CH0_CR_TC_PWRDN_tcsh_OFF                                 (42)
  #define MC0_CH0_CR_TC_PWRDN_tcsh_WID                                 ( 6)
  #define MC0_CH0_CR_TC_PWRDN_tcsh_MSK                                 (0x0000FC0000000000ULL)
  #define MC0_CH0_CR_TC_PWRDN_tcsh_MIN                                 (0)
  #define MC0_CH0_CR_TC_PWRDN_tcsh_MAX                                 (60) // 0x0000003C
  #define MC0_CH0_CR_TC_PWRDN_tcsh_DEF                                 (0x00000004)
  #define MC0_CH0_CR_TC_PWRDN_tcsh_HSH                                 (0x4654E050)

  #define MC0_CH0_CR_TC_PWRDN_tcsl_OFF                                 (48)
  #define MC0_CH0_CR_TC_PWRDN_tcsl_WID                                 ( 6)
  #define MC0_CH0_CR_TC_PWRDN_tcsl_MSK                                 (0x003F000000000000ULL)
  #define MC0_CH0_CR_TC_PWRDN_tcsl_MIN                                 (0)
  #define MC0_CH0_CR_TC_PWRDN_tcsl_MAX                                 (46) // 0x0000002E
  #define MC0_CH0_CR_TC_PWRDN_tcsl_DEF                                 (0x00000004)
  #define MC0_CH0_CR_TC_PWRDN_tcsl_HSH                                 (0x4660E050)

  #define MC0_CH0_CR_TC_PWRDN_tcacsh_OFF                               (54)
  #define MC0_CH0_CR_TC_PWRDN_tcacsh_WID                               ( 5)
  #define MC0_CH0_CR_TC_PWRDN_tcacsh_MSK                               (0x07C0000000000000ULL)
  #define MC0_CH0_CR_TC_PWRDN_tcacsh_MIN                               (0)
  #define MC0_CH0_CR_TC_PWRDN_tcacsh_MAX                               (46) // 0x0000002E
  #define MC0_CH0_CR_TC_PWRDN_tcacsh_DEF                               (0x00000004)
  #define MC0_CH0_CR_TC_PWRDN_tcacsh_HSH                               (0x456CE050)

  #define MC0_CH0_CR_TC_PWRDN_tprpden_OFF                              (59)
  #define MC0_CH0_CR_TC_PWRDN_tprpden_WID                              ( 5)
  #define MC0_CH0_CR_TC_PWRDN_tprpden_MSK                              (0xF800000000000000ULL)
  #define MC0_CH0_CR_TC_PWRDN_tprpden_MIN                              (2)
  #define MC0_CH0_CR_TC_PWRDN_tprpden_MAX                              (16) // 0x00000010
  #define MC0_CH0_CR_TC_PWRDN_tprpden_DEF                              (0x00000001)
  #define MC0_CH0_CR_TC_PWRDN_tprpden_HSH                              (0x4576E050)

#define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_HIGH_REG                    (0x0000E058)
#define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_HIGH_DEF                    (0x00000000)
#define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_HIGH_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_HIGH_WPQ_disable_OFF      ( 0)
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_HIGH_WPQ_disable_WID      (32)
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_HIGH_WPQ_disable_MSK      (0xFFFFFFFF)
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_HIGH_WPQ_disable_MIN      (0)
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_HIGH_WPQ_disable_MAX      (4294967295) // 0xFFFFFFFF
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_HIGH_WPQ_disable_DEF      (0x00000000)
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_HIGH_WPQ_disable_HSH      (0x2000E058)

#define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_IPQ_REG                         (0x0000E05C)
#define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_IPQ_DEF                         (0x00000000)
#define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_IPQ_VOLATILE_BITFIELDS_MSK      (0x00000000)

  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_IPQ_IPQ_disable_OFF           ( 0)
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_IPQ_IPQ_disable_WID           (16)
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_IPQ_IPQ_disable_MSK           (0x0000FFFF)
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_IPQ_IPQ_disable_MIN           (0)
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_IPQ_IPQ_disable_MAX           (65535) // 0x0000FFFF
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_IPQ_IPQ_disable_DEF           (0x00000000)
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_IPQ_IPQ_disable_HSH           (0x1000E05C)

#define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_LOW_REG                     (0x0000E060)
#define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_LOW_DEF                     (0x0000000000000000)
#define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_LOW_VOLATILE_BITFIELDS_MSK  (0x0000000000000000)

  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_LOW_WPQ_disable_OFF       ( 0)
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_LOW_WPQ_disable_WID       (64)
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_LOW_WPQ_disable_MSK       (0xFFFFFFFFFFFFFFFFULL)
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_LOW_WPQ_disable_MIN       (0)
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_LOW_WPQ_disable_MAX       (18446744073709551615ULL) // 0xFFFFFFFFFFFFFFFF
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_LOW_WPQ_disable_DEF       (0x00000000)
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_LOW_WPQ_disable_HSH       (0x4000E060)

#define MC0_CH0_CR_SC_WDBWM_REG                                        (0x0000E068)
#define MC0_CH0_CR_SC_WDBWM_DEF                                        (0x003F3020145C4858)
#define MC0_CH0_CR_SC_WDBWM_VOLATILE_BITFIELDS_MSK                     (0x0000000000000000)

  #define MC0_CH0_CR_SC_WDBWM_WMM_entry_wm_OFF                         ( 0)
  #define MC0_CH0_CR_SC_WDBWM_WMM_entry_wm_WID                         ( 7)
  #define MC0_CH0_CR_SC_WDBWM_WMM_entry_wm_MSK                         (0x0000007F)
  #define MC0_CH0_CR_SC_WDBWM_WMM_entry_wm_MIN                         (0)
  #define MC0_CH0_CR_SC_WDBWM_WMM_entry_wm_MAX                         (127) // 0x0000007F
  #define MC0_CH0_CR_SC_WDBWM_WMM_entry_wm_DEF                         (0x00000058)
  #define MC0_CH0_CR_SC_WDBWM_WMM_entry_wm_HSH                         (0x4700E068)

  #define MC0_CH0_CR_SC_WDBWM_WMM_exit_wm_OFF                          ( 8)
  #define MC0_CH0_CR_SC_WDBWM_WMM_exit_wm_WID                          ( 7)
  #define MC0_CH0_CR_SC_WDBWM_WMM_exit_wm_MSK                          (0x00007F00)
  #define MC0_CH0_CR_SC_WDBWM_WMM_exit_wm_MIN                          (0)
  #define MC0_CH0_CR_SC_WDBWM_WMM_exit_wm_MAX                          (127) // 0x0000007F
  #define MC0_CH0_CR_SC_WDBWM_WMM_exit_wm_DEF                          (0x00000048)
  #define MC0_CH0_CR_SC_WDBWM_WMM_exit_wm_HSH                          (0x4710E068)

  #define MC0_CH0_CR_SC_WDBWM_WIM_wm_OFF                               (16)
  #define MC0_CH0_CR_SC_WDBWM_WIM_wm_WID                               ( 7)
  #define MC0_CH0_CR_SC_WDBWM_WIM_wm_MSK                               (0x007F0000)
  #define MC0_CH0_CR_SC_WDBWM_WIM_wm_MIN                               (0)
  #define MC0_CH0_CR_SC_WDBWM_WIM_wm_MAX                               (127) // 0x0000007F
  #define MC0_CH0_CR_SC_WDBWM_WIM_wm_DEF                               (0x0000005C)
  #define MC0_CH0_CR_SC_WDBWM_WIM_wm_HSH                               (0x4720E068)

  #define MC0_CH0_CR_SC_WDBWM_write_isoc_cas_count_OFF                 (24)
  #define MC0_CH0_CR_SC_WDBWM_write_isoc_cas_count_WID                 ( 6)
  #define MC0_CH0_CR_SC_WDBWM_write_isoc_cas_count_MSK                 (0x3F000000)
  #define MC0_CH0_CR_SC_WDBWM_write_isoc_cas_count_MIN                 (0)
  #define MC0_CH0_CR_SC_WDBWM_write_isoc_cas_count_MAX                 (63) // 0x0000003F
  #define MC0_CH0_CR_SC_WDBWM_write_isoc_cas_count_DEF                 (0x00000014)
  #define MC0_CH0_CR_SC_WDBWM_write_isoc_cas_count_HSH                 (0x4630E068)

  #define MC0_CH0_CR_SC_WDBWM_read_cas_count_OFF                       (32)
  #define MC0_CH0_CR_SC_WDBWM_read_cas_count_WID                       ( 6)
  #define MC0_CH0_CR_SC_WDBWM_read_cas_count_MSK                       (0x0000003F00000000ULL)
  #define MC0_CH0_CR_SC_WDBWM_read_cas_count_MIN                       (0)
  #define MC0_CH0_CR_SC_WDBWM_read_cas_count_MAX                       (63) // 0x0000003F
  #define MC0_CH0_CR_SC_WDBWM_read_cas_count_DEF                       (0x00000020)
  #define MC0_CH0_CR_SC_WDBWM_read_cas_count_HSH                       (0x4640E068)

  #define MC0_CH0_CR_SC_WDBWM_write_cas_count_OFF                      (40)
  #define MC0_CH0_CR_SC_WDBWM_write_cas_count_WID                      ( 6)
  #define MC0_CH0_CR_SC_WDBWM_write_cas_count_MSK                      (0x00003F0000000000ULL)
  #define MC0_CH0_CR_SC_WDBWM_write_cas_count_MIN                      (0)
  #define MC0_CH0_CR_SC_WDBWM_write_cas_count_MAX                      (63) // 0x0000003F
  #define MC0_CH0_CR_SC_WDBWM_write_cas_count_DEF                      (0x00000030)
  #define MC0_CH0_CR_SC_WDBWM_write_cas_count_HSH                      (0x4650E068)

  #define MC0_CH0_CR_SC_WDBWM_write_cas_count_for_vc1_OFF              (48)
  #define MC0_CH0_CR_SC_WDBWM_write_cas_count_for_vc1_WID              ( 6)
  #define MC0_CH0_CR_SC_WDBWM_write_cas_count_for_vc1_MSK              (0x003F000000000000ULL)
  #define MC0_CH0_CR_SC_WDBWM_write_cas_count_for_vc1_MIN              (0)
  #define MC0_CH0_CR_SC_WDBWM_write_cas_count_for_vc1_MAX              (63) // 0x0000003F
  #define MC0_CH0_CR_SC_WDBWM_write_cas_count_for_vc1_DEF              (0x0000003F)
  #define MC0_CH0_CR_SC_WDBWM_write_cas_count_for_vc1_HSH              (0x4660E068)

#define MC0_CH0_CR_TC_CAS_REG                                          (0x0000E070)
#define MC0_CH0_CR_TC_CAS_DEF                                          (0x06050008)
#define MC0_CH0_CR_TC_CAS_VOLATILE_BITFIELDS_MSK                       (0x00000000)

  #define MC0_CH0_CR_TC_CAS_tccd_32_byte_cas_delta_OFF                 ( 0)
  #define MC0_CH0_CR_TC_CAS_tccd_32_byte_cas_delta_WID                 ( 6)
  #define MC0_CH0_CR_TC_CAS_tccd_32_byte_cas_delta_MSK                 (0x0000003F)
  #define MC0_CH0_CR_TC_CAS_tccd_32_byte_cas_delta_MIN                 (0)
  #define MC0_CH0_CR_TC_CAS_tccd_32_byte_cas_delta_MAX                 (63) // 0x0000003F
  #define MC0_CH0_CR_TC_CAS_tccd_32_byte_cas_delta_DEF                 (0x00000008)
  #define MC0_CH0_CR_TC_CAS_tccd_32_byte_cas_delta_HSH                 (0x0600E070)

  #define MC0_CH0_CR_TC_CAS_tCL_OFF                                    (16)
  #define MC0_CH0_CR_TC_CAS_tCL_WID                                    ( 7)
  #define MC0_CH0_CR_TC_CAS_tCL_MSK                                    (0x007F0000)
  #define MC0_CH0_CR_TC_CAS_tCL_MIN                                    (4)
  #define MC0_CH0_CR_TC_CAS_tCL_MAX                                    (76) // 0x0000004C
  #define MC0_CH0_CR_TC_CAS_tCL_DEF                                    (0x00000005)
  #define MC0_CH0_CR_TC_CAS_tCL_HSH                                    (0x0720E070)

  #define MC0_CH0_CR_TC_CAS_tCWL_OFF                                   (24)
  #define MC0_CH0_CR_TC_CAS_tCWL_WID                                   ( 8)
  #define MC0_CH0_CR_TC_CAS_tCWL_MSK                                   (0xFF000000)
  #define MC0_CH0_CR_TC_CAS_tCWL_MIN                                   (2)
  #define MC0_CH0_CR_TC_CAS_tCWL_MAX                                   (74) // 0x0000004A
  #define MC0_CH0_CR_TC_CAS_tCWL_DEF                                   (0x00000006)
  #define MC0_CH0_CR_TC_CAS_tCWL_HSH                                   (0x0830E070)

#define MC0_CH0_CR_MCSCHEDS_SPARE_REG                                  (0x0000E078)
#define MC0_CH0_CR_MCSCHEDS_SPARE_DEF                                  (0x00000000)
#define MC0_CH0_CR_MCSCHEDS_SPARE_VOLATILE_BITFIELDS_MSK               (0xFF000000)

  #define MC0_CH0_CR_MCSCHEDS_SPARE_Spare_RW_OFF                       ( 0)
  #define MC0_CH0_CR_MCSCHEDS_SPARE_Spare_RW_WID                       (16)
  #define MC0_CH0_CR_MCSCHEDS_SPARE_Spare_RW_MSK                       (0x0000FFFF)
  #define MC0_CH0_CR_MCSCHEDS_SPARE_Spare_RW_MIN                       (0)
  #define MC0_CH0_CR_MCSCHEDS_SPARE_Spare_RW_MAX                       (65535) // 0x0000FFFF
  #define MC0_CH0_CR_MCSCHEDS_SPARE_Spare_RW_DEF                       (0x00000000)
  #define MC0_CH0_CR_MCSCHEDS_SPARE_Spare_RW_HSH                       (0x1000E078)

  #define MC0_CH0_CR_MCSCHEDS_SPARE_dis_lpddr5_rdwr_interleaving_OFF   (16)
  #define MC0_CH0_CR_MCSCHEDS_SPARE_dis_lpddr5_rdwr_interleaving_WID   ( 1)
  #define MC0_CH0_CR_MCSCHEDS_SPARE_dis_lpddr5_rdwr_interleaving_MSK   (0x00010000)
  #define MC0_CH0_CR_MCSCHEDS_SPARE_dis_lpddr5_rdwr_interleaving_MIN   (0)
  #define MC0_CH0_CR_MCSCHEDS_SPARE_dis_lpddr5_rdwr_interleaving_MAX   (1) // 0x00000001
  #define MC0_CH0_CR_MCSCHEDS_SPARE_dis_lpddr5_rdwr_interleaving_DEF   (0x00000000)
  #define MC0_CH0_CR_MCSCHEDS_SPARE_dis_lpddr5_rdwr_interleaving_HSH   (0x0120E078)

  #define MC0_CH0_CR_MCSCHEDS_SPARE_cbit_sagv_lpmode_masking_behavior_OFF (17)
  #define MC0_CH0_CR_MCSCHEDS_SPARE_cbit_sagv_lpmode_masking_behavior_WID ( 1)
  #define MC0_CH0_CR_MCSCHEDS_SPARE_cbit_sagv_lpmode_masking_behavior_MSK (0x00020000)
  #define MC0_CH0_CR_MCSCHEDS_SPARE_cbit_sagv_lpmode_masking_behavior_MIN (0)
  #define MC0_CH0_CR_MCSCHEDS_SPARE_cbit_sagv_lpmode_masking_behavior_MAX (1) // 0x00000001
  #define MC0_CH0_CR_MCSCHEDS_SPARE_cbit_sagv_lpmode_masking_behavior_DEF (0x00000000)
  #define MC0_CH0_CR_MCSCHEDS_SPARE_cbit_sagv_lpmode_masking_behavior_HSH (0x0122E078)

  #define MC0_CH0_CR_MCSCHEDS_SPARE_Spare_RW_V_OFF                     (24)
  #define MC0_CH0_CR_MCSCHEDS_SPARE_Spare_RW_V_WID                     ( 8)
  #define MC0_CH0_CR_MCSCHEDS_SPARE_Spare_RW_V_MSK                     (0xFF000000)
  #define MC0_CH0_CR_MCSCHEDS_SPARE_Spare_RW_V_MIN                     (0)
  #define MC0_CH0_CR_MCSCHEDS_SPARE_Spare_RW_V_MAX                     (255) // 0x000000FF
  #define MC0_CH0_CR_MCSCHEDS_SPARE_Spare_RW_V_DEF                     (0x00000000)
  #define MC0_CH0_CR_MCSCHEDS_SPARE_Spare_RW_V_HSH                     (0x08B0E078)

#define MC0_CH0_CR_TC_MPC_REG                                          (0x0000E07C)
#define MC0_CH0_CR_TC_MPC_DEF                                          (0x00001063)
#define MC0_CH0_CR_TC_MPC_VOLATILE_BITFIELDS_MSK                       (0x00000000)

  #define MC0_CH0_CR_TC_MPC_tmpc_setup_OFF                             ( 0)
  #define MC0_CH0_CR_TC_MPC_tmpc_setup_WID                             ( 5)
  #define MC0_CH0_CR_TC_MPC_tmpc_setup_MSK                             (0x0000001F)
  #define MC0_CH0_CR_TC_MPC_tmpc_setup_MIN                             (0)
  #define MC0_CH0_CR_TC_MPC_tmpc_setup_MAX                             (31) // 0x0000001F
  #define MC0_CH0_CR_TC_MPC_tmpc_setup_DEF                             (0x00000003)
  #define MC0_CH0_CR_TC_MPC_tmpc_setup_HSH                             (0x0500E07C)

  #define MC0_CH0_CR_TC_MPC_tmpc_hold_OFF                              ( 5)
  #define MC0_CH0_CR_TC_MPC_tmpc_hold_WID                              ( 5)
  #define MC0_CH0_CR_TC_MPC_tmpc_hold_MSK                              (0x000003E0)
  #define MC0_CH0_CR_TC_MPC_tmpc_hold_MIN                              (0)
  #define MC0_CH0_CR_TC_MPC_tmpc_hold_MAX                              (31) // 0x0000001F
  #define MC0_CH0_CR_TC_MPC_tmpc_hold_DEF                              (0x00000003)
  #define MC0_CH0_CR_TC_MPC_tmpc_hold_HSH                              (0x050AE07C)

  #define MC0_CH0_CR_TC_MPC_tmpc_cs_OFF                                (10)
  #define MC0_CH0_CR_TC_MPC_tmpc_cs_WID                                ( 5)
  #define MC0_CH0_CR_TC_MPC_tmpc_cs_MSK                                (0x00007C00)
  #define MC0_CH0_CR_TC_MPC_tmpc_cs_MIN                                (0)
  #define MC0_CH0_CR_TC_MPC_tmpc_cs_MAX                                (31) // 0x0000001F
  #define MC0_CH0_CR_TC_MPC_tmpc_cs_DEF                                (0x00000004)
  #define MC0_CH0_CR_TC_MPC_tmpc_cs_HSH                                (0x0514E07C)

  #define MC0_CH0_CR_TC_MPC_tSTAB_OFF                                  (16)
  #define MC0_CH0_CR_TC_MPC_tSTAB_WID                                  (14)
  #define MC0_CH0_CR_TC_MPC_tSTAB_MSK                                  (0x3FFF0000)
  #define MC0_CH0_CR_TC_MPC_tSTAB_MIN                                  (0)
  #define MC0_CH0_CR_TC_MPC_tSTAB_MAX                                  (16383) // 0x00003FFF
  #define MC0_CH0_CR_TC_MPC_tSTAB_DEF                                  (0x00000000)
  #define MC0_CH0_CR_TC_MPC_tSTAB_HSH                                  (0x0E20E07C)

#define MC0_CH0_CR_SC_ODT_MATRIX_REG                                   (0x0000E080)
#define MC0_CH0_CR_SC_ODT_MATRIX_DEF                                   (0x00000000)
#define MC0_CH0_CR_SC_ODT_MATRIX_VOLATILE_BITFIELDS_MSK                (0x00000000)

  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_0_OFF                     ( 0)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_0_WID                     ( 4)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_0_MSK                     (0x0000000F)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_0_MIN                     (0)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_0_MAX                     (15) // 0x0000000F
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_0_DEF                     (0x00000000)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_0_HSH                     (0x0400E080)

  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_1_OFF                     ( 4)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_1_WID                     ( 4)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_1_MSK                     (0x000000F0)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_1_MIN                     (0)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_1_MAX                     (15) // 0x0000000F
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_1_DEF                     (0x00000000)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_1_HSH                     (0x0408E080)

  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_2_OFF                     ( 8)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_2_WID                     ( 4)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_2_MSK                     (0x00000F00)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_2_MIN                     (0)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_2_MAX                     (15) // 0x0000000F
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_2_DEF                     (0x00000000)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_2_HSH                     (0x0410E080)

  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_3_OFF                     (12)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_3_WID                     ( 4)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_3_MSK                     (0x0000F000)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_3_MIN                     (0)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_3_MAX                     (15) // 0x0000000F
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_3_DEF                     (0x00000000)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Read_Rank_3_HSH                     (0x0418E080)

  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_0_OFF                    (16)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_0_WID                    ( 4)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_0_MSK                    (0x000F0000)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_0_MIN                    (0)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_0_MAX                    (15) // 0x0000000F
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_0_DEF                    (0x00000000)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_0_HSH                    (0x0420E080)

  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_1_OFF                    (20)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_1_WID                    ( 4)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_1_MSK                    (0x00F00000)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_1_MIN                    (0)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_1_MAX                    (15) // 0x0000000F
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_1_DEF                    (0x00000000)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_1_HSH                    (0x0428E080)

  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_2_OFF                    (24)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_2_WID                    ( 4)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_2_MSK                    (0x0F000000)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_2_MIN                    (0)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_2_MAX                    (15) // 0x0000000F
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_2_DEF                    (0x00000000)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_2_HSH                    (0x0430E080)

  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_3_OFF                    (28)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_3_WID                    ( 4)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_3_MSK                    (0xF0000000)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_3_MIN                    (0)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_3_MAX                    (15) // 0x0000000F
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_3_DEF                    (0x00000000)
  #define MC0_CH0_CR_SC_ODT_MATRIX_Write_Rank_3_HSH                    (0x0438E080)

#define MC0_CH0_CR_DFT_BLOCK_REG                                       (0x0000E084)
#define MC0_CH0_CR_DFT_BLOCK_DEF                                       (0x00000000)
#define MC0_CH0_CR_DFT_BLOCK_VOLATILE_BITFIELDS_MSK                    (0x00000000)

  #define MC0_CH0_CR_DFT_BLOCK_dft_block_cycles_OFF                    ( 0)
  #define MC0_CH0_CR_DFT_BLOCK_dft_block_cycles_WID                    (16)
  #define MC0_CH0_CR_DFT_BLOCK_dft_block_cycles_MSK                    (0x0000FFFF)
  #define MC0_CH0_CR_DFT_BLOCK_dft_block_cycles_MIN                    (0)
  #define MC0_CH0_CR_DFT_BLOCK_dft_block_cycles_MAX                    (65535) // 0x0000FFFF
  #define MC0_CH0_CR_DFT_BLOCK_dft_block_cycles_DEF                    (0x00000000)
  #define MC0_CH0_CR_DFT_BLOCK_dft_block_cycles_HSH                    (0x1000E084)

  #define MC0_CH0_CR_DFT_BLOCK_dft_nonblock_cycles_OFF                 (16)
  #define MC0_CH0_CR_DFT_BLOCK_dft_nonblock_cycles_WID                 (16)
  #define MC0_CH0_CR_DFT_BLOCK_dft_nonblock_cycles_MSK                 (0xFFFF0000)
  #define MC0_CH0_CR_DFT_BLOCK_dft_nonblock_cycles_MIN                 (0)
  #define MC0_CH0_CR_DFT_BLOCK_dft_nonblock_cycles_MAX                 (65535) // 0x0000FFFF
  #define MC0_CH0_CR_DFT_BLOCK_dft_nonblock_cycles_DEF                 (0x00000000)
  #define MC0_CH0_CR_DFT_BLOCK_dft_nonblock_cycles_HSH                 (0x1020E084)

#define MC0_CH0_CR_SC_GS_CFG_REG                                       (0x0000E088)
#define MC0_CH0_CR_SC_GS_CFG_DEF                                       (0x0000C000)
#define MC0_CH0_CR_SC_GS_CFG_VOLATILE_BITFIELDS_MSK                    (0x00000000)

  #define MC0_CH0_CR_SC_GS_CFG_dram_technology_OFF                     ( 0)
  #define MC0_CH0_CR_SC_GS_CFG_dram_technology_WID                     ( 2)
  #define MC0_CH0_CR_SC_GS_CFG_dram_technology_MSK                     (0x00000003)
  #define MC0_CH0_CR_SC_GS_CFG_dram_technology_MIN                     (0)
  #define MC0_CH0_CR_SC_GS_CFG_dram_technology_MAX                     (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_dram_technology_DEF                     (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_dram_technology_HSH                     (0x0200E088)

  #define MC0_CH0_CR_SC_GS_CFG_cmd_stretch_OFF                         ( 2)
  #define MC0_CH0_CR_SC_GS_CFG_cmd_stretch_WID                         ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_cmd_stretch_MSK                         (0x00000004)
  #define MC0_CH0_CR_SC_GS_CFG_cmd_stretch_MIN                         (0)
  #define MC0_CH0_CR_SC_GS_CFG_cmd_stretch_MAX                         (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_cmd_stretch_DEF                         (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_cmd_stretch_HSH                         (0x0104E088)

  #define MC0_CH0_CR_SC_GS_CFG_address_mirror_OFF                      ( 3)
  #define MC0_CH0_CR_SC_GS_CFG_address_mirror_WID                      ( 4)
  #define MC0_CH0_CR_SC_GS_CFG_address_mirror_MSK                      (0x00000078)
  #define MC0_CH0_CR_SC_GS_CFG_address_mirror_MIN                      (0)
  #define MC0_CH0_CR_SC_GS_CFG_address_mirror_MAX                      (15) // 0x0000000F
  #define MC0_CH0_CR_SC_GS_CFG_address_mirror_DEF                      (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_address_mirror_HSH                      (0x0406E088)

  #define MC0_CH0_CR_SC_GS_CFG_frequency_point_OFF                     ( 7)
  #define MC0_CH0_CR_SC_GS_CFG_frequency_point_WID                     ( 2)
  #define MC0_CH0_CR_SC_GS_CFG_frequency_point_MSK                     (0x00000180)
  #define MC0_CH0_CR_SC_GS_CFG_frequency_point_MIN                     (0)
  #define MC0_CH0_CR_SC_GS_CFG_frequency_point_MAX                     (3) // 0x00000003
  #define MC0_CH0_CR_SC_GS_CFG_frequency_point_DEF                     (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_frequency_point_HSH                     (0x020EE088)

  #define MC0_CH0_CR_SC_GS_CFG_enable_odt_matrix_OFF                   ( 9)
  #define MC0_CH0_CR_SC_GS_CFG_enable_odt_matrix_WID                   ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_enable_odt_matrix_MSK                   (0x00000200)
  #define MC0_CH0_CR_SC_GS_CFG_enable_odt_matrix_MIN                   (0)
  #define MC0_CH0_CR_SC_GS_CFG_enable_odt_matrix_MAX                   (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_enable_odt_matrix_DEF                   (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_enable_odt_matrix_HSH                   (0x0112E088)

  #define MC0_CH0_CR_SC_GS_CFG_gear_mode_OFF                           (10)
  #define MC0_CH0_CR_SC_GS_CFG_gear_mode_WID                           ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_gear_mode_MSK                           (0x00000400)
  #define MC0_CH0_CR_SC_GS_CFG_gear_mode_MIN                           (0)
  #define MC0_CH0_CR_SC_GS_CFG_gear_mode_MAX                           (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_gear_mode_DEF                           (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_gear_mode_HSH                           (0x0114E088)

  #define MC0_CH0_CR_SC_GS_CFG_ddr_1dpc_split_ranks_on_subch_OFF       (11)
  #define MC0_CH0_CR_SC_GS_CFG_ddr_1dpc_split_ranks_on_subch_WID       ( 2)
  #define MC0_CH0_CR_SC_GS_CFG_ddr_1dpc_split_ranks_on_subch_MSK       (0x00001800)
  #define MC0_CH0_CR_SC_GS_CFG_ddr_1dpc_split_ranks_on_subch_MIN       (0)
  #define MC0_CH0_CR_SC_GS_CFG_ddr_1dpc_split_ranks_on_subch_MAX       (3) // 0x00000003
  #define MC0_CH0_CR_SC_GS_CFG_ddr_1dpc_split_ranks_on_subch_DEF       (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_ddr_1dpc_split_ranks_on_subch_HSH       (0x0216E088)

  #define MC0_CH0_CR_SC_GS_CFG_lpddr5_bg_mode_OFF                      (13)
  #define MC0_CH0_CR_SC_GS_CFG_lpddr5_bg_mode_WID                      ( 2)
  #define MC0_CH0_CR_SC_GS_CFG_lpddr5_bg_mode_MSK                      (0x00006000)
  #define MC0_CH0_CR_SC_GS_CFG_lpddr5_bg_mode_MIN                      (0)
  #define MC0_CH0_CR_SC_GS_CFG_lpddr5_bg_mode_MAX                      (3) // 0x00000003
  #define MC0_CH0_CR_SC_GS_CFG_lpddr5_bg_mode_DEF                      (0x00000002)
  #define MC0_CH0_CR_SC_GS_CFG_lpddr5_bg_mode_HSH                      (0x021AE088)

  #define MC0_CH0_CR_SC_GS_CFG_multicyccmd_OFF                         (15)
  #define MC0_CH0_CR_SC_GS_CFG_multicyccmd_WID                         ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_multicyccmd_MSK                         (0x00008000)
  #define MC0_CH0_CR_SC_GS_CFG_multicyccmd_MIN                         (0)
  #define MC0_CH0_CR_SC_GS_CFG_multicyccmd_MAX                         (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_multicyccmd_DEF                         (0x00000001)
  #define MC0_CH0_CR_SC_GS_CFG_multicyccmd_HSH                         (0x011EE088)

  #define MC0_CH0_CR_SC_GS_CFG_wckdifflowinidle_OFF                    (16)
  #define MC0_CH0_CR_SC_GS_CFG_wckdifflowinidle_WID                    ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_wckdifflowinidle_MSK                    (0x00010000)
  #define MC0_CH0_CR_SC_GS_CFG_wckdifflowinidle_MIN                    (0)
  #define MC0_CH0_CR_SC_GS_CFG_wckdifflowinidle_MAX                    (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_wckdifflowinidle_DEF                    (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_wckdifflowinidle_HSH                    (0x0120E088)

  #define MC0_CH0_CR_SC_GS_CFG_cpgc_in_order_OFF                       (17)
  #define MC0_CH0_CR_SC_GS_CFG_cpgc_in_order_WID                       ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_cpgc_in_order_MSK                       (0x00020000)
  #define MC0_CH0_CR_SC_GS_CFG_cpgc_in_order_MIN                       (0)
  #define MC0_CH0_CR_SC_GS_CFG_cpgc_in_order_MAX                       (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_cpgc_in_order_DEF                       (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_cpgc_in_order_HSH                       (0x0122E088)

  #define MC0_CH0_CR_SC_GS_CFG_cs_geardown_enable_OFF                  (18)
  #define MC0_CH0_CR_SC_GS_CFG_cs_geardown_enable_WID                  ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_cs_geardown_enable_MSK                  (0x00040000)
  #define MC0_CH0_CR_SC_GS_CFG_cs_geardown_enable_MIN                  (0)
  #define MC0_CH0_CR_SC_GS_CFG_cs_geardown_enable_MAX                  (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_cs_geardown_enable_DEF                  (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_cs_geardown_enable_HSH                  (0x0124E088)

  #define MC0_CH0_CR_SC_GS_CFG_DDR5_CKD_enable_OFF                     (19)
  #define MC0_CH0_CR_SC_GS_CFG_DDR5_CKD_enable_WID                     ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_DDR5_CKD_enable_MSK                     (0x00080000)
  #define MC0_CH0_CR_SC_GS_CFG_DDR5_CKD_enable_MIN                     (0)
  #define MC0_CH0_CR_SC_GS_CFG_DDR5_CKD_enable_MAX                     (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_DDR5_CKD_enable_DEF                     (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_DDR5_CKD_enable_HSH                     (0x0126E088)

#define MC0_CH0_CR_SC_GS_CFG_UARCH_REG                                 (0x0000E08C)
#define MC0_CH0_CR_SC_GS_CFG_UARCH_DEF                                 (0x00070000)
#define MC0_CH0_CR_SC_GS_CFG_UARCH_VOLATILE_BITFIELDS_MSK              (0x00000000)

  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_cs_tristate_OFF           ( 2)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_cs_tristate_WID           ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_cs_tristate_MSK           (0x00000004)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_cs_tristate_MIN           (0)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_cs_tristate_MAX           (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_cs_tristate_DEF           (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_cs_tristate_HSH           (0x0104E08C)

  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_tristate_OFF              ( 3)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_tristate_WID              ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_tristate_MSK              (0x00000008)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_tristate_MIN              (0)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_tristate_MAX              (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_tristate_DEF              (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_tristate_HSH              (0x0106E08C)

  #define MC0_CH0_CR_SC_GS_CFG_UARCH_no_gear4_param_divide_OFF         ( 4)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_no_gear4_param_divide_WID         ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_no_gear4_param_divide_MSK         (0x00000010)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_no_gear4_param_divide_MIN         (0)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_no_gear4_param_divide_MAX         (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_no_gear4_param_divide_DEF         (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_no_gear4_param_divide_HSH         (0x0108E08C)

  #define MC0_CH0_CR_SC_GS_CFG_UARCH_no_gear2_param_divide_OFF         ( 5)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_no_gear2_param_divide_WID         ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_no_gear2_param_divide_MSK         (0x00000020)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_no_gear2_param_divide_MIN         (0)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_no_gear2_param_divide_MAX         (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_no_gear2_param_divide_DEF         (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_no_gear2_param_divide_HSH         (0x010AE08C)

  #define MC0_CH0_CR_SC_GS_CFG_UARCH_pl_trace_only_cs_cmd_OFF          ( 6)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_pl_trace_only_cs_cmd_WID          ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_pl_trace_only_cs_cmd_MSK          (0x00000040)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_pl_trace_only_cs_cmd_MIN          (0)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_pl_trace_only_cs_cmd_MAX          (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_pl_trace_only_cs_cmd_DEF          (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_pl_trace_only_cs_cmd_HSH          (0x010CE08C)

  #define MC0_CH0_CR_SC_GS_CFG_UARCH_two_cycle_cke_idle_OFF            ( 7)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_two_cycle_cke_idle_WID            ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_two_cycle_cke_idle_MSK            (0x00000080)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_two_cycle_cke_idle_MIN            (0)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_two_cycle_cke_idle_MAX            (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_two_cycle_cke_idle_DEF            (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_two_cycle_cke_idle_HSH            (0x010EE08C)

  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_ca_tristate_OFF           ( 8)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_ca_tristate_WID           ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_ca_tristate_MSK           (0x00000100)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_ca_tristate_MIN           (0)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_ca_tristate_MAX           (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_ca_tristate_DEF           (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_ca_tristate_HSH           (0x0110E08C)

  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_ck_tristate_OFF           ( 9)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_ck_tristate_WID           ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_ck_tristate_MSK           (0x00000200)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_ck_tristate_MIN           (0)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_ck_tristate_MAX           (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_ck_tristate_DEF           (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_disable_ck_tristate_HSH           (0x0112E08C)

  #define MC0_CH0_CR_SC_GS_CFG_UARCH_pde_ca11_OFF                      (10)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_pde_ca11_WID                      ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_pde_ca11_MSK                      (0x00000400)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_pde_ca11_MIN                      (0)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_pde_ca11_MAX                      (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_pde_ca11_DEF                      (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_pde_ca11_HSH                      (0x0114E08C)

  #define MC0_CH0_CR_SC_GS_CFG_UARCH_force_pre_after_cas_OFF           (12)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_force_pre_after_cas_WID           ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_force_pre_after_cas_MSK           (0x00001000)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_force_pre_after_cas_MIN           (0)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_force_pre_after_cas_MAX           (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_force_pre_after_cas_DEF           (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_force_pre_after_cas_HSH           (0x0118E08C)

  #define MC0_CH0_CR_SC_GS_CFG_UARCH_block_cas_when_synccounter_finishes_OFF (13)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_block_cas_when_synccounter_finishes_WID ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_block_cas_when_synccounter_finishes_MSK (0x00002000)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_block_cas_when_synccounter_finishes_MIN (0)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_block_cas_when_synccounter_finishes_MAX (26) // 0x0000001A
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_block_cas_when_synccounter_finishes_DEF (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_block_cas_when_synccounter_finishes_HSH (0x011AE08C)

  #define MC0_CH0_CR_SC_GS_CFG_UARCH_WCK_MRR_SHORTER_BL_DIS_OFF        (14)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_WCK_MRR_SHORTER_BL_DIS_WID        ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_WCK_MRR_SHORTER_BL_DIS_MSK        (0x00004000)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_WCK_MRR_SHORTER_BL_DIS_MIN        (0)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_WCK_MRR_SHORTER_BL_DIS_MAX        (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_WCK_MRR_SHORTER_BL_DIS_DEF        (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_WCK_MRR_SHORTER_BL_DIS_HSH        (0x011CE08C)

  #define MC0_CH0_CR_SC_GS_CFG_UARCH_ALLOW_CAS_AFTER_WCKSTOP_OFF       (15)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_ALLOW_CAS_AFTER_WCKSTOP_WID       ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_ALLOW_CAS_AFTER_WCKSTOP_MSK       (0x00008000)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_ALLOW_CAS_AFTER_WCKSTOP_MIN       (0)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_ALLOW_CAS_AFTER_WCKSTOP_MAX       (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_ALLOW_CAS_AFTER_WCKSTOP_DEF       (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_ALLOW_CAS_AFTER_WCKSTOP_HSH       (0x011EE08C)

  #define MC0_CH0_CR_SC_GS_CFG_UARCH_ddr5_ckd_ckcke_enforce_tstab_OFF  (16)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_ddr5_ckd_ckcke_enforce_tstab_WID  ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_ddr5_ckd_ckcke_enforce_tstab_MSK  (0x00010000)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_ddr5_ckd_ckcke_enforce_tstab_MIN  (0)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_ddr5_ckd_ckcke_enforce_tstab_MAX  (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_ddr5_ckd_ckcke_enforce_tstab_DEF  (0x00000001)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_ddr5_ckd_ckcke_enforce_tstab_HSH  (0x0120E08C)

  #define MC0_CH0_CR_SC_GS_CFG_UARCH_roundtrip_spec_stall_free_entries_OFF (17)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_roundtrip_spec_stall_free_entries_WID ( 4)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_roundtrip_spec_stall_free_entries_MSK (0x001E0000)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_roundtrip_spec_stall_free_entries_MIN (0)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_roundtrip_spec_stall_free_entries_MAX (15) // 0x0000000F
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_roundtrip_spec_stall_free_entries_DEF (0x00000003)
  #define MC0_CH0_CR_SC_GS_CFG_UARCH_roundtrip_spec_stall_free_entries_HSH (0x0422E08C)

#define MC0_CH0_CR_SC_PH_THROTTLING_0_REG                              (0x0000E090)
#define MC0_CH0_CR_SC_PH_THROTTLING_0_DEF                              (0x030C0108)
#define MC0_CH0_CR_SC_PH_THROTTLING_0_VOLATILE_BITFIELDS_MSK           (0x00000000)

  #define MC0_CH0_CR_SC_PH_THROTTLING_0_loaded_same_rank_OFF           ( 0)
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_loaded_same_rank_WID           ( 6)
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_loaded_same_rank_MSK           (0x0000003F)
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_loaded_same_rank_MIN           (0)
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_loaded_same_rank_MAX           (63) // 0x0000003F
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_loaded_same_rank_DEF           (0x00000008)
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_loaded_same_rank_HSH           (0x0600E090)

  #define MC0_CH0_CR_SC_PH_THROTTLING_0_unloaded_same_rank_OFF         ( 8)
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_unloaded_same_rank_WID         ( 6)
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_unloaded_same_rank_MSK         (0x00003F00)
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_unloaded_same_rank_MIN         (0)
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_unloaded_same_rank_MAX         (63) // 0x0000003F
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_unloaded_same_rank_DEF         (0x00000001)
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_unloaded_same_rank_HSH         (0x0610E090)

  #define MC0_CH0_CR_SC_PH_THROTTLING_0_loaded_different_rank_OFF      (16)
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_loaded_different_rank_WID      ( 6)
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_loaded_different_rank_MSK      (0x003F0000)
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_loaded_different_rank_MIN      (0)
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_loaded_different_rank_MAX      (63) // 0x0000003F
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_loaded_different_rank_DEF      (0x0000000C)
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_loaded_different_rank_HSH      (0x0620E090)

  #define MC0_CH0_CR_SC_PH_THROTTLING_0_unloaded_different_rank_OFF    (24)
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_unloaded_different_rank_WID    ( 6)
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_unloaded_different_rank_MSK    (0x3F000000)
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_unloaded_different_rank_MIN    (0)
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_unloaded_different_rank_MAX    (63) // 0x0000003F
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_unloaded_different_rank_DEF    (0x00000003)
  #define MC0_CH0_CR_SC_PH_THROTTLING_0_unloaded_different_rank_HSH    (0x0630E090)

#define MC0_CH0_CR_SC_PH_THROTTLING_1_REG                              (0x0000E094)
#define MC0_CH0_CR_SC_PH_THROTTLING_1_DEF                              (0x02080004)
#define MC0_CH0_CR_SC_PH_THROTTLING_1_VOLATILE_BITFIELDS_MSK           (0x00000000)

  #define MC0_CH0_CR_SC_PH_THROTTLING_1_loaded_same_rank_OFF           ( 0)
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_loaded_same_rank_WID           ( 6)
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_loaded_same_rank_MSK           (0x0000003F)
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_loaded_same_rank_MIN           (0)
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_loaded_same_rank_MAX           (63) // 0x0000003F
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_loaded_same_rank_DEF           (0x00000004)
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_loaded_same_rank_HSH           (0x0600E094)

  #define MC0_CH0_CR_SC_PH_THROTTLING_1_unloaded_same_rank_OFF         ( 8)
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_unloaded_same_rank_WID         ( 6)
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_unloaded_same_rank_MSK         (0x00003F00)
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_unloaded_same_rank_MIN         (0)
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_unloaded_same_rank_MAX         (63) // 0x0000003F
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_unloaded_same_rank_DEF         (0x00000000)
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_unloaded_same_rank_HSH         (0x0610E094)

  #define MC0_CH0_CR_SC_PH_THROTTLING_1_loaded_different_rank_OFF      (16)
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_loaded_different_rank_WID      ( 6)
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_loaded_different_rank_MSK      (0x003F0000)
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_loaded_different_rank_MIN      (0)
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_loaded_different_rank_MAX      (63) // 0x0000003F
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_loaded_different_rank_DEF      (0x00000008)
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_loaded_different_rank_HSH      (0x0620E094)

  #define MC0_CH0_CR_SC_PH_THROTTLING_1_unloaded_different_rank_OFF    (24)
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_unloaded_different_rank_WID    ( 6)
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_unloaded_different_rank_MSK    (0x3F000000)
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_unloaded_different_rank_MIN    (0)
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_unloaded_different_rank_MAX    (63) // 0x0000003F
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_unloaded_different_rank_DEF    (0x00000002)
  #define MC0_CH0_CR_SC_PH_THROTTLING_1_unloaded_different_rank_HSH    (0x0630E094)

#define MC0_CH0_CR_SC_PH_THROTTLING_2_REG                              (0x0000E098)
#define MC0_CH0_CR_SC_PH_THROTTLING_2_DEF                              (0x00060002)
#define MC0_CH0_CR_SC_PH_THROTTLING_2_VOLATILE_BITFIELDS_MSK           (0x00000000)

  #define MC0_CH0_CR_SC_PH_THROTTLING_2_loaded_same_rank_OFF           ( 0)
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_loaded_same_rank_WID           ( 6)
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_loaded_same_rank_MSK           (0x0000003F)
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_loaded_same_rank_MIN           (0)
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_loaded_same_rank_MAX           (63) // 0x0000003F
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_loaded_same_rank_DEF           (0x00000002)
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_loaded_same_rank_HSH           (0x0600E098)

  #define MC0_CH0_CR_SC_PH_THROTTLING_2_unloaded_same_rank_OFF         ( 8)
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_unloaded_same_rank_WID         ( 6)
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_unloaded_same_rank_MSK         (0x00003F00)
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_unloaded_same_rank_MIN         (0)
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_unloaded_same_rank_MAX         (63) // 0x0000003F
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_unloaded_same_rank_DEF         (0x00000000)
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_unloaded_same_rank_HSH         (0x0610E098)

  #define MC0_CH0_CR_SC_PH_THROTTLING_2_loaded_different_rank_OFF      (16)
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_loaded_different_rank_WID      ( 6)
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_loaded_different_rank_MSK      (0x003F0000)
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_loaded_different_rank_MIN      (0)
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_loaded_different_rank_MAX      (63) // 0x0000003F
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_loaded_different_rank_DEF      (0x00000006)
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_loaded_different_rank_HSH      (0x0620E098)

  #define MC0_CH0_CR_SC_PH_THROTTLING_2_unloaded_different_rank_OFF    (24)
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_unloaded_different_rank_WID    ( 6)
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_unloaded_different_rank_MSK    (0x3F000000)
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_unloaded_different_rank_MIN    (0)
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_unloaded_different_rank_MAX    (63) // 0x0000003F
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_unloaded_different_rank_DEF    (0x00000000)
  #define MC0_CH0_CR_SC_PH_THROTTLING_2_unloaded_different_rank_HSH    (0x0630E098)

#define MC0_CH0_CR_SC_PH_THROTTLING_3_REG                              (0x0000E09C)
#define MC0_CH0_CR_SC_PH_THROTTLING_3_DEF                              (0x00000000)
#define MC0_CH0_CR_SC_PH_THROTTLING_3_VOLATILE_BITFIELDS_MSK           (0x00000000)

  #define MC0_CH0_CR_SC_PH_THROTTLING_3_loaded_same_rank_OFF           ( 0)
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_loaded_same_rank_WID           ( 6)
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_loaded_same_rank_MSK           (0x0000003F)
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_loaded_same_rank_MIN           (0)
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_loaded_same_rank_MAX           (63) // 0x0000003F
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_loaded_same_rank_DEF           (0x00000000)
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_loaded_same_rank_HSH           (0x0600E09C)

  #define MC0_CH0_CR_SC_PH_THROTTLING_3_unloaded_same_rank_OFF         ( 8)
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_unloaded_same_rank_WID         ( 6)
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_unloaded_same_rank_MSK         (0x00003F00)
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_unloaded_same_rank_MIN         (0)
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_unloaded_same_rank_MAX         (63) // 0x0000003F
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_unloaded_same_rank_DEF         (0x00000000)
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_unloaded_same_rank_HSH         (0x0610E09C)

  #define MC0_CH0_CR_SC_PH_THROTTLING_3_loaded_different_rank_OFF      (16)
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_loaded_different_rank_WID      ( 6)
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_loaded_different_rank_MSK      (0x003F0000)
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_loaded_different_rank_MIN      (0)
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_loaded_different_rank_MAX      (63) // 0x0000003F
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_loaded_different_rank_DEF      (0x00000000)
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_loaded_different_rank_HSH      (0x0620E09C)

  #define MC0_CH0_CR_SC_PH_THROTTLING_3_unloaded_different_rank_OFF    (24)
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_unloaded_different_rank_WID    ( 6)
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_unloaded_different_rank_MSK    (0x3F000000)
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_unloaded_different_rank_MIN    (0)
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_unloaded_different_rank_MAX    (63) // 0x0000003F
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_unloaded_different_rank_DEF    (0x00000000)
  #define MC0_CH0_CR_SC_PH_THROTTLING_3_unloaded_different_rank_HSH    (0x0630E09C)

#define MC0_CH0_CR_SC_WPQ_THRESHOLD_REG                                (0x0000E0A0)
#define MC0_CH0_CR_SC_WPQ_THRESHOLD_DEF                                (0xD0852285)
#define MC0_CH0_CR_SC_WPQ_THRESHOLD_VOLATILE_BITFIELDS_MSK             (0x00000000)

  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_low_wm_OFF                       ( 0)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_low_wm_WID                       ( 6)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_low_wm_MSK                       (0x0000003F)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_low_wm_MIN                       (0)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_low_wm_MAX                       (63) // 0x0000003F
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_low_wm_DEF                       (0x00000005)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_low_wm_HSH                       (0x0600E0A0)

  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_med_wm_OFF                       ( 6)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_med_wm_WID                       ( 6)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_med_wm_MSK                       (0x00000FC0)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_med_wm_MIN                       (0)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_med_wm_MAX                       (63) // 0x0000003F
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_med_wm_DEF                       (0x0000000A)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_med_wm_HSH                       (0x060CE0A0)

  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_high_wm_OFF                      (12)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_high_wm_WID                      ( 6)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_high_wm_MSK                      (0x0003F000)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_high_wm_MIN                      (0)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_high_wm_MAX                      (63) // 0x0000003F
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_high_wm_DEF                      (0x00000012)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_high_wm_HSH                      (0x0618E0A0)

  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_phs_allowed_under_low_wm_OFF     (18)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_phs_allowed_under_low_wm_WID     ( 4)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_phs_allowed_under_low_wm_MSK     (0x003C0000)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_phs_allowed_under_low_wm_MIN     (0)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_phs_allowed_under_low_wm_MAX     (15) // 0x0000000F
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_phs_allowed_under_low_wm_DEF     (0x00000001)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_phs_allowed_under_low_wm_HSH     (0x0424E0A0)

  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_phs_allowed_under_med_wm_OFF     (22)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_phs_allowed_under_med_wm_WID     ( 4)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_phs_allowed_under_med_wm_MSK     (0x03C00000)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_phs_allowed_under_med_wm_MIN     (0)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_phs_allowed_under_med_wm_MAX     (15) // 0x0000000F
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_phs_allowed_under_med_wm_DEF     (0x00000002)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_phs_allowed_under_med_wm_HSH     (0x042CE0A0)

  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_phs_allowed_under_high_wm_OFF    (26)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_phs_allowed_under_high_wm_WID    ( 4)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_phs_allowed_under_high_wm_MSK    (0x3C000000)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_phs_allowed_under_high_wm_MIN    (0)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_phs_allowed_under_high_wm_MAX    (15) // 0x0000000F
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_phs_allowed_under_high_wm_DEF    (0x00000004)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_phs_allowed_under_high_wm_HSH    (0x0434E0A0)

  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_read_ph_weight_OFF               (30)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_read_ph_weight_WID               ( 2)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_read_ph_weight_MSK               (0xC0000000)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_read_ph_weight_MIN               (0)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_read_ph_weight_MAX               (3) // 0x00000003
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_read_ph_weight_DEF               (0x00000003)
  #define MC0_CH0_CR_SC_WPQ_THRESHOLD_read_ph_weight_HSH               (0x023CE0A0)

#define MC0_CH0_CR_SC_PR_CNT_CONFIG_REG                                (0x0000E0A8)
#define MC0_CH0_CR_SC_PR_CNT_CONFIG_DEF                                (0x0280401000040040)
#define MC0_CH0_CR_SC_PR_CNT_CONFIG_VOLATILE_BITFIELDS_MSK             (0x0000000000000000)

  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_Normal_Timer_OFF                 ( 0)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_Normal_Timer_WID                 (10)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_Normal_Timer_MSK                 (0x000003FF)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_Normal_Timer_MIN                 (0)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_Normal_Timer_MAX                 (1023) // 0x000003FF
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_Normal_Timer_DEF                 (0x00000040)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_Normal_Timer_HSH                 (0x4A00E0A8)

  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_Latency_Tolerant_Timer_OFF       (10)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_Latency_Tolerant_Timer_WID       (10)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_Latency_Tolerant_Timer_MSK       (0x000FFC00)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_Latency_Tolerant_Timer_MIN       (0)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_Latency_Tolerant_Timer_MAX       (1023) // 0x000003FF
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_Latency_Tolerant_Timer_DEF       (0x00000100)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_Latency_Tolerant_Timer_HSH       (0x4A14E0A8)

  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_VC0_latency_guard_timer_x16_OFF  (20)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_VC0_latency_guard_timer_x16_WID  ( 9)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_VC0_latency_guard_timer_x16_MSK  (0x1FF00000)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_VC0_latency_guard_timer_x16_MIN  (0)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_VC0_latency_guard_timer_x16_MAX  (511) // 0x000001FF
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_VC0_latency_guard_timer_x16_DEF  (0x00000000)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_VC0_latency_guard_timer_x16_HSH  (0x4928E0A8)

  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_VC1_latency_guard_timer_x8_OFF   (32)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_VC1_latency_guard_timer_x8_WID   (11)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_VC1_latency_guard_timer_x8_MSK   (0x000007FF00000000ULL)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_VC1_latency_guard_timer_x8_MIN   (0)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_VC1_latency_guard_timer_x8_MAX   (2047) // 0x000007FF
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_VC1_latency_guard_timer_x8_DEF   (0x00000010)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_VC1_latency_guard_timer_x8_HSH   (0x4B40E0A8)

  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_VC1_Isoc_latency_guard_timer_x8_OFF (43)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_VC1_Isoc_latency_guard_timer_x8_WID (11)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_VC1_Isoc_latency_guard_timer_x8_MSK (0x003FF80000000000ULL)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_VC1_Isoc_latency_guard_timer_x8_MIN (0)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_VC1_Isoc_latency_guard_timer_x8_MAX (2047) // 0x000007FF
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_VC1_Isoc_latency_guard_timer_x8_DEF (0x00000008)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_VC1_Isoc_latency_guard_timer_x8_HSH (0x4B56E0A8)

  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_Latency_Sensitive_Timer_x16_OFF  (54)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_Latency_Sensitive_Timer_x16_WID  ( 9)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_Latency_Sensitive_Timer_x16_MSK  (0x7FC0000000000000ULL)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_Latency_Sensitive_Timer_x16_MIN  (0)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_Latency_Sensitive_Timer_x16_MAX  (511) // 0x000001FF
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_Latency_Sensitive_Timer_x16_DEF  (0x0000000A)
  #define MC0_CH0_CR_SC_PR_CNT_CONFIG_Latency_Sensitive_Timer_x16_HSH  (0x496CE0A8)

#define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_REG                        (0x0000E0B0)
#define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_DEF                        (0x0000000F)
#define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_VOLATILE_BITFIELDS_MSK     (0x00000000)

  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CKE_Override_OFF         ( 0)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CKE_Override_WID         ( 4)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CKE_Override_MSK         (0x0000000F)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CKE_Override_MIN         (0)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CKE_Override_MAX         (15) // 0x0000000F
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CKE_Override_DEF         (0x0000000F)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CKE_Override_HSH         (0x0400E0B0)

  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_0_OFF        ( 4)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_0_WID        ( 4)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_0_MSK        (0x000000F0)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_0_MIN        (0)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_0_MAX        (15) // 0x0000000F
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_0_DEF        (0x00000000)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_0_HSH        (0x0408E0B0)

  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_1_OFF        ( 8)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_1_WID        ( 4)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_1_MSK        (0x00000F00)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_1_MIN        (0)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_1_MAX        (15) // 0x0000000F
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_1_DEF        (0x00000000)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_1_HSH        (0x0410E0B0)

  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CKE_On_OFF               (16)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CKE_On_WID               ( 4)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CKE_On_MSK               (0x000F0000)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CKE_On_MIN               (0)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CKE_On_MAX               (15) // 0x0000000F
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CKE_On_DEF               (0x00000000)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CKE_On_HSH               (0x0420E0B0)

  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_Val_0_OFF    (20)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_Val_0_WID    ( 4)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_Val_0_MSK    (0x00F00000)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_Val_0_MIN    (0)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_Val_0_MAX    (15) // 0x0000000F
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_Val_0_DEF    (0x00000000)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_Val_0_HSH    (0x0428E0B0)

  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_Val_1_OFF    (24)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_Val_1_WID    ( 4)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_Val_1_MSK    (0x0F000000)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_Val_1_MIN    (0)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_Val_1_MAX    (15) // 0x0000000F
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_Val_1_DEF    (0x00000000)
  #define MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_CS_Override_Val_1_HSH    (0x0430E0B0)

#define MC0_CH0_CR_SC_GS_CFG_TRAINING_REG                              (0x0000E0B4)
#define MC0_CH0_CR_SC_GS_CFG_TRAINING_DEF                              (0x00000000)
#define MC0_CH0_CR_SC_GS_CFG_TRAINING_VOLATILE_BITFIELDS_MSK           (0x00000000)

  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_ignore_cke_OFF                 ( 8)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_ignore_cke_WID                 ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_ignore_cke_MSK                 (0x00000100)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_ignore_cke_MIN                 (0)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_ignore_cke_MAX                 (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_ignore_cke_DEF                 (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_ignore_cke_HSH                 (0x0110E0B4)

  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_mask_cs_OFF                    ( 9)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_mask_cs_WID                    ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_mask_cs_MSK                    (0x00000200)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_mask_cs_MIN                    (0)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_mask_cs_MAX                    (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_mask_cs_DEF                    (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_mask_cs_HSH                    (0x0112E0B4)

  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_disable_tristate_OFF           (11)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_disable_tristate_WID           ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_disable_tristate_MSK           (0x00000800)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_disable_tristate_MIN           (0)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_disable_tristate_MAX           (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_disable_tristate_DEF           (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_disable_tristate_HSH           (0x0116E0B4)

  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_Block_xarb_OFF                 (17)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_Block_xarb_WID                 ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_Block_xarb_MSK                 (0x00020000)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_Block_xarb_MIN                 (0)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_Block_xarb_MAX                 (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_Block_xarb_DEF                 (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_Block_xarb_HSH                 (0x0122E0B4)

  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_Block_cke_OFF                  (18)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_Block_cke_WID                  ( 1)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_Block_cke_MSK                  (0x00040000)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_Block_cke_MIN                  (0)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_Block_cke_MAX                  (1) // 0x00000001
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_Block_cke_DEF                  (0x00000000)
  #define MC0_CH0_CR_SC_GS_CFG_TRAINING_Block_cke_HSH                  (0x0124E0B4)

#define MC0_CH0_CR_SPID_LOW_POWER_CTL_REG                              (0x0000E0B8)
#define MC0_CH0_CR_SPID_LOW_POWER_CTL_DEF                              (0x00001004E0431188)
#define MC0_CH0_CR_SPID_LOW_POWER_CTL_VOLATILE_BITFIELDS_MSK           (0x0000000000000000)

  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_raise_cke_after_exit_latency_OFF ( 0)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_raise_cke_after_exit_latency_WID ( 1)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_raise_cke_after_exit_latency_MSK (0x00000001)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_raise_cke_after_exit_latency_MIN (0)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_raise_cke_after_exit_latency_MAX (1) // 0x00000001
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_raise_cke_after_exit_latency_DEF (0x00000000)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_raise_cke_after_exit_latency_HSH (0x4100E0B8)

  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_idle_length_OFF                ( 1)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_idle_length_WID                ( 4)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_idle_length_MSK                (0x0000001E)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_idle_length_MIN                (0)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_idle_length_MAX                (15) // 0x0000000F
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_idle_length_DEF                (0x00000004)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_idle_length_HSH                (0x4402E0B8)

  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_powerdown_latency_OFF          ( 5)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_powerdown_latency_WID          ( 5)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_powerdown_latency_MSK          (0x000003E0)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_powerdown_latency_MIN          (0)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_powerdown_latency_MAX          (31) // 0x0000001F
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_powerdown_latency_DEF          (0x0000000C)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_powerdown_latency_HSH          (0x450AE0B8)

  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_powerdown_length_OFF           (10)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_powerdown_length_WID           ( 4)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_powerdown_length_MSK           (0x00003C00)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_powerdown_length_MIN           (0)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_powerdown_length_MAX           (15) // 0x0000000F
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_powerdown_length_DEF           (0x00000004)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_powerdown_length_HSH           (0x4414E0B8)

  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_latency_OFF        (14)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_latency_WID        ( 6)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_latency_MSK        (0x000FC000)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_latency_MIN        (0)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_latency_MAX        (63) // 0x0000003F
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_latency_DEF        (0x0000000C)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_latency_HSH        (0x461CE0B8)

  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_length_OFF         (20)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_length_WID         ( 4)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_length_MSK         (0x00F00000)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_length_MIN         (0)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_length_MAX         (15) // 0x0000000F
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_length_DEF         (0x00000004)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_length_HSH         (0x4428E0B8)

  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_idle_enable_OFF                (29)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_idle_enable_WID                ( 1)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_idle_enable_MSK                (0x20000000)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_idle_enable_MIN                (0)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_idle_enable_MAX                (1) // 0x00000001
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_idle_enable_DEF                (0x00000001)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_idle_enable_HSH                (0x413AE0B8)

  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_powerdown_enable_OFF           (30)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_powerdown_enable_WID           ( 1)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_powerdown_enable_MSK           (0x40000000)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_powerdown_enable_MIN           (0)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_powerdown_enable_MAX           (1) // 0x00000001
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_powerdown_enable_DEF           (0x00000001)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_powerdown_enable_HSH           (0x413CE0B8)

  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_enable_OFF         (31)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_enable_WID         ( 1)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_enable_MSK         (0x80000000)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_enable_MIN         (0)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_enable_MAX         (1) // 0x00000001
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_enable_DEF         (0x00000001)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_selfrefresh_enable_HSH         (0x413EE0B8)

  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_active_length_OFF              (32)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_active_length_WID              ( 9)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_active_length_MSK              (0x000001FF00000000ULL)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_active_length_MIN              (0)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_active_length_MAX              (511) // 0x000001FF
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_active_length_DEF              (0x00000004)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_active_length_HSH              (0x4940E0B8)

  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_idle_latency_OFF               (41)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_idle_latency_WID               ( 4)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_idle_latency_MSK               (0x00001E0000000000ULL)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_idle_latency_MIN               (0)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_idle_latency_MAX               (15) // 0x0000000F
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_idle_latency_DEF               (0x00000008)
  #define MC0_CH0_CR_SPID_LOW_POWER_CTL_idle_latency_HSH               (0x4452E0B8)

#define MC0_CH0_CR_SCHED_THIRD_CBIT_REG                                (0x0000E0C0)
#define MC0_CH0_CR_SCHED_THIRD_CBIT_DEF                                (0x00600040)
#define MC0_CH0_CR_SCHED_THIRD_CBIT_VOLATILE_BITFIELDS_MSK             (0x00000000)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_isoc_block_relaxation_OFF    ( 0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_isoc_block_relaxation_WID    ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_isoc_block_relaxation_MSK    (0x00000001)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_isoc_block_relaxation_MIN    (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_isoc_block_relaxation_MAX    (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_isoc_block_relaxation_DEF    (0x00000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_isoc_block_relaxation_HSH    (0x0100E0C0)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_no_global_block_with_bgf_invalid_OFF ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_no_global_block_with_bgf_invalid_WID ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_no_global_block_with_bgf_invalid_MSK (0x00000002)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_no_global_block_with_bgf_invalid_MIN (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_no_global_block_with_bgf_invalid_MAX (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_no_global_block_with_bgf_invalid_DEF (0x00000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_no_global_block_with_bgf_invalid_HSH (0x0102E0C0)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_write_ph_blocks_read_pre_OFF     ( 3)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_write_ph_blocks_read_pre_WID     ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_write_ph_blocks_read_pre_MSK     (0x00000008)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_write_ph_blocks_read_pre_MIN     (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_write_ph_blocks_read_pre_MAX     (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_write_ph_blocks_read_pre_DEF     (0x00000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_write_ph_blocks_read_pre_HSH     (0x0106E0C0)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_row_press_dis_pcit_pre_OFF       ( 4)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_row_press_dis_pcit_pre_WID       ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_row_press_dis_pcit_pre_MSK       (0x00000010)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_row_press_dis_pcit_pre_MIN       (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_row_press_dis_pcit_pre_MAX       (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_row_press_dis_pcit_pre_DEF       (0x00000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_row_press_dis_pcit_pre_HSH       (0x0108E0C0)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_row_press_dis_autopre_new_req_miss_OFF ( 5)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_row_press_dis_autopre_new_req_miss_WID ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_row_press_dis_autopre_new_req_miss_MSK (0x00000020)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_row_press_dis_autopre_new_req_miss_MIN (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_row_press_dis_autopre_new_req_miss_MAX (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_row_press_dis_autopre_new_req_miss_DEF (0x00000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_row_press_dis_autopre_new_req_miss_HSH (0x010AE0C0)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_row_press_dis_vc1_autopre_OFF    ( 6)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_row_press_dis_vc1_autopre_WID    ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_row_press_dis_vc1_autopre_MSK    (0x00000040)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_row_press_dis_vc1_autopre_MIN    (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_row_press_dis_vc1_autopre_MAX    (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_row_press_dis_vc1_autopre_DEF    (0x00000001)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_row_press_dis_vc1_autopre_HSH    (0x010CE0C0)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_preempt_vc1_during_demote_OFF    (11)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_preempt_vc1_during_demote_WID    ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_preempt_vc1_during_demote_MSK    (0x00000800)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_preempt_vc1_during_demote_MIN    (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_preempt_vc1_during_demote_MAX    (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_preempt_vc1_during_demote_DEF    (0x00000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_preempt_vc1_during_demote_HSH    (0x0116E0C0)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_idle_ref_start_OFF           (12)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_idle_ref_start_WID           ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_idle_ref_start_MSK           (0x00001000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_idle_ref_start_MIN           (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_idle_ref_start_MAX           (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_idle_ref_start_DEF           (0x00000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_idle_ref_start_HSH           (0x0118E0C0)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_oldest_isoch_pre_over_ph_OFF (14)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_oldest_isoch_pre_over_ph_WID ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_oldest_isoch_pre_over_ph_MSK (0x00004000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_oldest_isoch_pre_over_ph_MIN (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_oldest_isoch_pre_over_ph_MAX (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_oldest_isoch_pre_over_ph_DEF (0x00000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_oldest_isoch_pre_over_ph_HSH (0x011CE0C0)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_cke_off_in_refresh_OFF       (16)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_cke_off_in_refresh_WID       ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_cke_off_in_refresh_MSK       (0x00010000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_cke_off_in_refresh_MIN       (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_cke_off_in_refresh_MAX       (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_cke_off_in_refresh_DEF       (0x00000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_cke_off_in_refresh_HSH       (0x0120E0C0)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_1st_cas_split_act_OFF        (17)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_1st_cas_split_act_WID        ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_1st_cas_split_act_MSK        (0x00020000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_1st_cas_split_act_MIN        (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_1st_cas_split_act_MAX        (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_1st_cas_split_act_DEF        (0x00000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_1st_cas_split_act_HSH        (0x0122E0C0)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_wdb_clk_gate_OFF             (18)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_wdb_clk_gate_WID             ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_wdb_clk_gate_MSK             (0x00040000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_wdb_clk_gate_MIN             (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_wdb_clk_gate_MAX             (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_wdb_clk_gate_DEF             (0x00000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_wdb_clk_gate_HSH             (0x0124E0C0)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_mnt_clk_gate_OFF             (19)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_mnt_clk_gate_WID             ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_mnt_clk_gate_MSK             (0x00080000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_mnt_clk_gate_MIN             (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_mnt_clk_gate_MAX             (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_mnt_clk_gate_DEF             (0x00000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_mnt_clk_gate_HSH             (0x0126E0C0)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_scheds_clk_gate_OFF          (20)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_scheds_clk_gate_WID          ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_scheds_clk_gate_MSK          (0x00100000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_scheds_clk_gate_MIN          (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_scheds_clk_gate_MAX          (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_scheds_clk_gate_DEF          (0x00000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_scheds_clk_gate_HSH          (0x0128E0C0)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_lpmode_on_sagv_OFF           (21)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_lpmode_on_sagv_WID           ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_lpmode_on_sagv_MSK           (0x00200000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_lpmode_on_sagv_MIN           (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_lpmode_on_sagv_MAX           (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_lpmode_on_sagv_DEF           (0x00000001)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_lpmode_on_sagv_HSH           (0x012AE0C0)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_lpmode_on_r1blockdrain_OFF   (22)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_lpmode_on_r1blockdrain_WID   ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_lpmode_on_r1blockdrain_MSK   (0x00400000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_lpmode_on_r1blockdrain_MIN   (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_lpmode_on_r1blockdrain_MAX   (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_lpmode_on_r1blockdrain_DEF   (0x00000001)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_lpmode_on_r1blockdrain_HSH   (0x012CE0C0)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_ignore_1st_zqcs_OFF          (23)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_ignore_1st_zqcs_WID          ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_ignore_1st_zqcs_MSK          (0x00800000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_ignore_1st_zqcs_MIN          (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_ignore_1st_zqcs_MAX          (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_ignore_1st_zqcs_DEF          (0x00000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_dis_ignore_1st_zqcs_HSH          (0x012EE0C0)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_ipq_ignore_write_ph_OFF          (24)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_ipq_ignore_write_ph_WID          ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_ipq_ignore_write_ph_MSK          (0x01000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_ipq_ignore_write_ph_MIN          (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_ipq_ignore_write_ph_MAX          (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_ipq_ignore_write_ph_DEF          (0x00000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_ipq_ignore_write_ph_HSH          (0x0130E0C0)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_cksynccounteroverride_OFF        (25)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_cksynccounteroverride_WID        ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_cksynccounteroverride_MSK        (0x02000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_cksynccounteroverride_MIN        (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_cksynccounteroverride_MAX        (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_cksynccounteroverride_DEF        (0x00000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_cksynccounteroverride_HSH        (0x0132E0C0)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_cksynccounteroverridevalue_OFF   (26)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_cksynccounteroverridevalue_WID   ( 2)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_cksynccounteroverridevalue_MSK   (0x0C000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_cksynccounteroverridevalue_MIN   (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_cksynccounteroverridevalue_MAX   (3) // 0x00000003
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_cksynccounteroverridevalue_DEF   (0x00000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_cksynccounteroverridevalue_HSH   (0x0234E0C0)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_asdynamicmodeinsertbubble_OFF    (28)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_asdynamicmodeinsertbubble_WID    ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_asdynamicmodeinsertbubble_MSK    (0x10000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_asdynamicmodeinsertbubble_MIN    (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_asdynamicmodeinsertbubble_MAX    (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_asdynamicmodeinsertbubble_DEF    (0x00000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_asdynamicmodeinsertbubble_HSH    (0x0138E0C0)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_oppsr_pending_wr_clkgate_dis_mcchannel_OFF (29)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_oppsr_pending_wr_clkgate_dis_mcchannel_WID ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_oppsr_pending_wr_clkgate_dis_mcchannel_MSK (0x20000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_oppsr_pending_wr_clkgate_dis_mcchannel_MIN (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_oppsr_pending_wr_clkgate_dis_mcchannel_MAX (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_oppsr_pending_wr_clkgate_dis_mcchannel_DEF (0x00000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_oppsr_pending_wr_clkgate_dis_mcchannel_HSH (0x013AE0C0)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_bgfwakeupsoon_dis_OFF            (30)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_bgfwakeupsoon_dis_WID            ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_bgfwakeupsoon_dis_MSK            (0x40000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_bgfwakeupsoon_dis_MIN            (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_bgfwakeupsoon_dis_MAX            (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_bgfwakeupsoon_dis_DEF            (0x00000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_bgfwakeupsoon_dis_HSH            (0x013CE0C0)

  #define MC0_CH0_CR_SCHED_THIRD_CBIT_disable_split_act_OFF            (31)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_disable_split_act_WID            ( 1)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_disable_split_act_MSK            (0x80000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_disable_split_act_MIN            (0)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_disable_split_act_MAX            (1) // 0x00000001
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_disable_split_act_DEF            (0x00000000)
  #define MC0_CH0_CR_SCHED_THIRD_CBIT_disable_split_act_HSH            (0x013EE0C0)

#define MC0_CH0_CR_DEADLOCK_BREAKER_REG                                (0x0000E0C4)
#define MC0_CH0_CR_DEADLOCK_BREAKER_DEF                                (0x00000000)
#define MC0_CH0_CR_DEADLOCK_BREAKER_VOLATILE_BITFIELDS_MSK             (0x00000000)

  #define MC0_CH0_CR_DEADLOCK_BREAKER_No_CAS_threshold_OFF             ( 0)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_No_CAS_threshold_WID             (16)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_No_CAS_threshold_MSK             (0x0000FFFF)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_No_CAS_threshold_MIN             (0)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_No_CAS_threshold_MAX             (65535) // 0x0000FFFF
  #define MC0_CH0_CR_DEADLOCK_BREAKER_No_CAS_threshold_DEF             (0x00000000)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_No_CAS_threshold_HSH             (0x1000E0C4)

  #define MC0_CH0_CR_DEADLOCK_BREAKER_No_CAS_threshold_en_OFF          (16)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_No_CAS_threshold_en_WID          ( 1)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_No_CAS_threshold_en_MSK          (0x00010000)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_No_CAS_threshold_en_MIN          (0)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_No_CAS_threshold_en_MAX          (1) // 0x00000001
  #define MC0_CH0_CR_DEADLOCK_BREAKER_No_CAS_threshold_en_DEF          (0x00000000)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_No_CAS_threshold_en_HSH          (0x0120E0C4)

  #define MC0_CH0_CR_DEADLOCK_BREAKER_Preemption_threshold_en_OFF      (17)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Preemption_threshold_en_WID      ( 1)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Preemption_threshold_en_MSK      (0x00020000)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Preemption_threshold_en_MIN      (0)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Preemption_threshold_en_MAX      (1) // 0x00000001
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Preemption_threshold_en_DEF      (0x00000000)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Preemption_threshold_en_HSH      (0x0122E0C4)

  #define MC0_CH0_CR_DEADLOCK_BREAKER_Stop_count_during_tt_OFF         (18)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Stop_count_during_tt_WID         ( 1)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Stop_count_during_tt_MSK         (0x00040000)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Stop_count_during_tt_MIN         (0)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Stop_count_during_tt_MAX         (1) // 0x00000001
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Stop_count_during_tt_DEF         (0x00000000)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Stop_count_during_tt_HSH         (0x0124E0C4)

  #define MC0_CH0_CR_DEADLOCK_BREAKER_Attempt_refresh_on_deadlock_OFF  (19)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Attempt_refresh_on_deadlock_WID  ( 1)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Attempt_refresh_on_deadlock_MSK  (0x00080000)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Attempt_refresh_on_deadlock_MIN  (0)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Attempt_refresh_on_deadlock_MAX  (1) // 0x00000001
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Attempt_refresh_on_deadlock_DEF  (0x00000000)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Attempt_refresh_on_deadlock_HSH  (0x0126E0C4)

  #define MC0_CH0_CR_DEADLOCK_BREAKER_MajorMode_force_duration_OFF     (20)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_MajorMode_force_duration_WID     ( 8)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_MajorMode_force_duration_MSK     (0x0FF00000)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_MajorMode_force_duration_MIN     (0)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_MajorMode_force_duration_MAX     (255) // 0x000000FF
  #define MC0_CH0_CR_DEADLOCK_BREAKER_MajorMode_force_duration_DEF     (0x00000000)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_MajorMode_force_duration_HSH     (0x0828E0C4)

  #define MC0_CH0_CR_DEADLOCK_BREAKER_MajorMode_force_en_OFF           (28)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_MajorMode_force_en_WID           ( 1)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_MajorMode_force_en_MSK           (0x10000000)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_MajorMode_force_en_MIN           (0)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_MajorMode_force_en_MAX           (1) // 0x00000001
  #define MC0_CH0_CR_DEADLOCK_BREAKER_MajorMode_force_en_DEF           (0x00000000)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_MajorMode_force_en_HSH           (0x0138E0C4)

  #define MC0_CH0_CR_DEADLOCK_BREAKER_Disable_blocking_rules_OFF       (29)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Disable_blocking_rules_WID       ( 1)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Disable_blocking_rules_MSK       (0x20000000)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Disable_blocking_rules_MIN       (0)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Disable_blocking_rules_MAX       (1) // 0x00000001
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Disable_blocking_rules_DEF       (0x00000000)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Disable_blocking_rules_HSH       (0x013AE0C4)

  #define MC0_CH0_CR_DEADLOCK_BREAKER_Rank_join_OFF                    (30)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Rank_join_WID                    ( 2)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Rank_join_MSK                    (0xC0000000)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Rank_join_MIN                    (0)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Rank_join_MAX                    (3) // 0x00000003
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Rank_join_DEF                    (0x00000000)
  #define MC0_CH0_CR_DEADLOCK_BREAKER_Rank_join_HSH                    (0x023CE0C4)

#define MC0_CH0_CR_MCSCHEDS_GLOBAL_DRIVER_GATE_CFG_REG                 (0x0000E0D0)
//Duplicate of MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_REG

#define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_REG                           (0x0000E0E0)
#define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_DEF                           (0x00002001E6491B37)
#define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VOLATILE_BITFIELDS_MSK        (0x0000000000000000)

  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_low_wm_allowed_preempt_priorities_OFF ( 0)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_low_wm_allowed_preempt_priorities_WID ( 3)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_low_wm_allowed_preempt_priorities_MSK (0x00000007)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_low_wm_allowed_preempt_priorities_MIN (0)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_low_wm_allowed_preempt_priorities_MAX (7) // 0x00000007
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_low_wm_allowed_preempt_priorities_DEF (0x00000007)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_low_wm_allowed_preempt_priorities_HSH (0x4300E0E0)

  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_med_wm_allowed_preempt_priorities_OFF ( 3)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_med_wm_allowed_preempt_priorities_WID ( 3)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_med_wm_allowed_preempt_priorities_MSK (0x00000038)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_med_wm_allowed_preempt_priorities_MIN (0)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_med_wm_allowed_preempt_priorities_MAX (7) // 0x00000007
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_med_wm_allowed_preempt_priorities_DEF (0x00000006)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_med_wm_allowed_preempt_priorities_HSH (0x4306E0E0)

  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_high_wm_allowed_preempt_priorities_OFF ( 6)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_high_wm_allowed_preempt_priorities_WID ( 3)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_high_wm_allowed_preempt_priorities_MSK (0x000001C0)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_high_wm_allowed_preempt_priorities_MIN (0)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_high_wm_allowed_preempt_priorities_MAX (7) // 0x00000007
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_high_wm_allowed_preempt_priorities_DEF (0x00000004)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_high_wm_allowed_preempt_priorities_HSH (0x430CE0E0)

  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_ph_block_pe_OFF             ( 9)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_ph_block_pe_WID             ( 1)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_ph_block_pe_MSK             (0x00000200)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_ph_block_pe_MIN             (0)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_ph_block_pe_MAX             (1) // 0x00000001
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_ph_block_pe_DEF             (0x00000001)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_ph_block_pe_HSH             (0x4112E0E0)

  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_blkr_effect_major_mode_OFF  (10)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_blkr_effect_major_mode_WID  ( 1)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_blkr_effect_major_mode_MSK  (0x00000400)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_blkr_effect_major_mode_MIN  (0)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_blkr_effect_major_mode_MAX  (1) // 0x00000001
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_blkr_effect_major_mode_DEF  (0x00000000)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_blkr_effect_major_mode_HSH  (0x4114E0E0)

  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_RIM_BW_optimization_OFF     (11)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_RIM_BW_optimization_WID     ( 1)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_RIM_BW_optimization_MSK     (0x00000800)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_RIM_BW_optimization_MIN     (0)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_RIM_BW_optimization_MAX     (1) // 0x00000001
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_RIM_BW_optimization_DEF     (0x00000001)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_RIM_BW_optimization_HSH     (0x4116E0E0)

  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_RIM_BW_prefer_VC1_OFF       (12)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_RIM_BW_prefer_VC1_WID       ( 1)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_RIM_BW_prefer_VC1_MSK       (0x00001000)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_RIM_BW_prefer_VC1_MIN       (0)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_RIM_BW_prefer_VC1_MAX       (1) // 0x00000001
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_RIM_BW_prefer_VC1_DEF       (0x00000001)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_RIM_BW_prefer_VC1_HSH       (0x4118E0E0)

  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_loaded_read_threshold_OFF   (13)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_loaded_read_threshold_WID   ( 6)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_loaded_read_threshold_MSK   (0x0007E000)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_loaded_read_threshold_MIN   (0)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_loaded_read_threshold_MAX   (32) // 0x00000020
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_loaded_read_threshold_DEF   (0x00000008)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_loaded_read_threshold_HSH   (0x461AE0E0)

  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_EN_OFF (19)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_EN_WID ( 1)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_EN_MSK (0x00080000)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_EN_MIN (0)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_EN_MAX (1) // 0x00000001
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_EN_DEF (0x00000001)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_EN_HSH (0x4126E0E0)

  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_COUNTER_RST_OFF (20)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_COUNTER_RST_WID ( 9)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_COUNTER_RST_MSK (0x1FF00000)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_COUNTER_RST_MIN (0)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_COUNTER_RST_MAX (511) // 0x000001FF
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_COUNTER_RST_DEF (0x00000064)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_COUNTER_RST_HSH (0x4928E0E0)

  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_BLOCK_ACT_OFF (29)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_BLOCK_ACT_WID ( 1)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_BLOCK_ACT_MSK (0x20000000)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_BLOCK_ACT_MIN (0)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_BLOCK_ACT_MAX (1) // 0x00000001
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_BLOCK_ACT_DEF (0x00000001)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_BLOCK_ACT_HSH (0x413AE0E0)

  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_BLOCK_PRE_OFF (30)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_BLOCK_PRE_WID ( 1)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_BLOCK_PRE_MSK (0x40000000)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_BLOCK_PRE_MIN (0)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_BLOCK_PRE_MAX (1) // 0x00000001
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_BLOCK_PRE_DEF (0x00000001)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_BLOCK_PRE_HSH (0x413CE0E0)

  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_BLOCK_PREEMPT_OFF (31)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_BLOCK_PREEMPT_WID ( 1)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_BLOCK_PREEMPT_MSK (0x80000000)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_BLOCK_PREEMPT_MIN (0)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_BLOCK_PREEMPT_MAX (1) // 0x00000001
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_BLOCK_PREEMPT_DEF (0x00000001)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC1_deadlock_by_stalled_VC0_handler_BLOCK_PREEMPT_HSH (0x413EE0E0)

  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC0_starvation_by_stalled_VC1_handler_EN_OFF (32)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC0_starvation_by_stalled_VC1_handler_EN_WID ( 1)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC0_starvation_by_stalled_VC1_handler_EN_MSK (0x0000000100000000ULL)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC0_starvation_by_stalled_VC1_handler_EN_MIN (0)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC0_starvation_by_stalled_VC1_handler_EN_MAX (1) // 0x00000001
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC0_starvation_by_stalled_VC1_handler_EN_DEF (0x00000001)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC0_starvation_by_stalled_VC1_handler_EN_HSH (0x4140E0E0)

  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC0_starvation_by_stalled_VC1_handler_CYCLES_OFF (40)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC0_starvation_by_stalled_VC1_handler_CYCLES_WID ( 8)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC0_starvation_by_stalled_VC1_handler_CYCLES_MSK (0x0000FF0000000000ULL)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC0_starvation_by_stalled_VC1_handler_CYCLES_MIN (0)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC0_starvation_by_stalled_VC1_handler_CYCLES_MAX (255) // 0x000000FF
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC0_starvation_by_stalled_VC1_handler_CYCLES_DEF (0x00000020)
  #define MC0_CH0_CR_SC_BLOCKING_RULES_CFG_VC0_starvation_by_stalled_VC1_handler_CYCLES_HSH (0x4850E0E0)

#define MC0_CH0_CR_PWM_DDR_SUBCH0_REQ_OCCUPANCY_COUNTER_REG            (0x0000E0E8)
#define MC0_CH0_CR_PWM_DDR_SUBCH0_REQ_OCCUPANCY_COUNTER_DEF            (0x0000000000000000)
#define MC0_CH0_CR_PWM_DDR_SUBCH0_REQ_OCCUPANCY_COUNTER_VOLATILE_BITFIELDS_MSK (0xFFFFFFFFFFFFFFFF)

  #define MC0_CH0_CR_PWM_DDR_SUBCH0_REQ_OCCUPANCY_COUNTER_REQ_OCCUPANCY_COUNTER_OFF ( 0)
  #define MC0_CH0_CR_PWM_DDR_SUBCH0_REQ_OCCUPANCY_COUNTER_REQ_OCCUPANCY_COUNTER_WID (64)
  #define MC0_CH0_CR_PWM_DDR_SUBCH0_REQ_OCCUPANCY_COUNTER_REQ_OCCUPANCY_COUNTER_MSK (0xFFFFFFFFFFFFFFFFULL)
  #define MC0_CH0_CR_PWM_DDR_SUBCH0_REQ_OCCUPANCY_COUNTER_REQ_OCCUPANCY_COUNTER_MIN (0)
  #define MC0_CH0_CR_PWM_DDR_SUBCH0_REQ_OCCUPANCY_COUNTER_REQ_OCCUPANCY_COUNTER_MAX (18446744073709551615ULL) // 0xFFFFFFFFFFFFFFFF
  #define MC0_CH0_CR_PWM_DDR_SUBCH0_REQ_OCCUPANCY_COUNTER_REQ_OCCUPANCY_COUNTER_DEF (0x00000000)
  #define MC0_CH0_CR_PWM_DDR_SUBCH0_REQ_OCCUPANCY_COUNTER_REQ_OCCUPANCY_COUNTER_HSH (0x4080E0E8)

#define MC0_CH0_CR_PWM_DDR_SUBCH1_REQ_OCCUPANCY_COUNTER_REG            (0x0000E0F0)
//Duplicate of MC0_CH0_CR_PWM_DDR_SUBCH0_REQ_OCCUPANCY_COUNTER_REG

#define MC0_CH0_CR_TR_RRDVALID_CTRL_REG                                (0x0000E100)
#define MC0_CH0_CR_TR_RRDVALID_CTRL_DEF                                (0x00000000)
#define MC0_CH0_CR_TR_RRDVALID_CTRL_VOLATILE_BITFIELDS_MSK             (0x00000B6D)

  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_0_trigger_OFF               ( 0)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_0_trigger_WID               ( 1)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_0_trigger_MSK               (0x00000001)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_0_trigger_MIN               (0)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_0_trigger_MAX               (1) // 0x00000001
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_0_trigger_DEF               (0x00000000)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_0_trigger_HSH               (0x0180E100)

  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_0_wr_en_OFF                 ( 1)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_0_wr_en_WID                 ( 1)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_0_wr_en_MSK                 (0x00000002)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_0_wr_en_MIN                 (0)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_0_wr_en_MAX                 (1) // 0x00000001
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_0_wr_en_DEF                 (0x00000000)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_0_wr_en_HSH                 (0x0102E100)

  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_0_overflow_OFF              ( 2)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_0_overflow_WID              ( 1)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_0_overflow_MSK              (0x00000004)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_0_overflow_MIN              (0)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_0_overflow_MAX              (1) // 0x00000001
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_0_overflow_DEF              (0x00000000)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_0_overflow_HSH              (0x0184E100)

  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_1_trigger_OFF               ( 3)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_1_trigger_WID               ( 1)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_1_trigger_MSK               (0x00000008)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_1_trigger_MIN               (0)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_1_trigger_MAX               (1) // 0x00000001
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_1_trigger_DEF               (0x00000000)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_1_trigger_HSH               (0x0186E100)

  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_1_wr_en_OFF                 ( 4)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_1_wr_en_WID                 ( 1)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_1_wr_en_MSK                 (0x00000010)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_1_wr_en_MIN                 (0)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_1_wr_en_MAX                 (1) // 0x00000001
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_1_wr_en_DEF                 (0x00000000)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_1_wr_en_HSH                 (0x0108E100)

  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_1_overflow_OFF              ( 5)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_1_overflow_WID              ( 1)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_1_overflow_MSK              (0x00000020)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_1_overflow_MIN              (0)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_1_overflow_MAX              (1) // 0x00000001
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_1_overflow_DEF              (0x00000000)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_1_overflow_HSH              (0x018AE100)

  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_2_trigger_OFF               ( 6)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_2_trigger_WID               ( 1)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_2_trigger_MSK               (0x00000040)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_2_trigger_MIN               (0)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_2_trigger_MAX               (1) // 0x00000001
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_2_trigger_DEF               (0x00000000)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_2_trigger_HSH               (0x018CE100)

  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_2_wr_en_OFF                 ( 7)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_2_wr_en_WID                 ( 1)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_2_wr_en_MSK                 (0x00000080)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_2_wr_en_MIN                 (0)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_2_wr_en_MAX                 (1) // 0x00000001
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_2_wr_en_DEF                 (0x00000000)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_2_wr_en_HSH                 (0x010EE100)

  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_2_overflow_OFF              ( 8)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_2_overflow_WID              ( 1)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_2_overflow_MSK              (0x00000100)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_2_overflow_MIN              (0)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_2_overflow_MAX              (1) // 0x00000001
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_2_overflow_DEF              (0x00000000)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_2_overflow_HSH              (0x0190E100)

  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_3_trigger_OFF               ( 9)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_3_trigger_WID               ( 1)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_3_trigger_MSK               (0x00000200)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_3_trigger_MIN               (0)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_3_trigger_MAX               (1) // 0x00000001
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_3_trigger_DEF               (0x00000000)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_3_trigger_HSH               (0x0192E100)

  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_3_wr_en_OFF                 (10)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_3_wr_en_WID                 ( 1)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_3_wr_en_MSK                 (0x00000400)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_3_wr_en_MIN                 (0)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_3_wr_en_MAX                 (1) // 0x00000001
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_3_wr_en_DEF                 (0x00000000)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_3_wr_en_HSH                 (0x0114E100)

  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_3_overflow_OFF              (11)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_3_overflow_WID              ( 1)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_3_overflow_MSK              (0x00000800)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_3_overflow_MIN              (0)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_3_overflow_MAX              (1) // 0x00000001
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_3_overflow_DEF              (0x00000000)
  #define MC0_CH0_CR_TR_RRDVALID_CTRL_Rank_3_overflow_HSH              (0x0196E100)

#define MC0_CH0_CR_TR_RRDVALID_DATA_REG                                (0x0000E104)
#define MC0_CH0_CR_TR_RRDVALID_DATA_DEF                                (0x00000000)
#define MC0_CH0_CR_TR_RRDVALID_DATA_VOLATILE_BITFIELDS_MSK             (0xFFFFFFFF)

  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_0_sign_OFF                  ( 0)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_0_sign_WID                  ( 1)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_0_sign_MSK                  (0x00000001)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_0_sign_MIN                  (0)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_0_sign_MAX                  (1) // 0x00000001
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_0_sign_DEF                  (0x00000000)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_0_sign_HSH                  (0x0180E104)

  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_0_value_OFF                 ( 1)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_0_value_WID                 ( 7)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_0_value_MSK                 (0x000000FE)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_0_value_MIN                 (0)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_0_value_MAX                 (127) // 0x0000007F
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_0_value_DEF                 (0x00000000)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_0_value_HSH                 (0x0782E104)

  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_1_sign_OFF                  ( 8)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_1_sign_WID                  ( 1)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_1_sign_MSK                  (0x00000100)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_1_sign_MIN                  (0)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_1_sign_MAX                  (1) // 0x00000001
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_1_sign_DEF                  (0x00000000)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_1_sign_HSH                  (0x0190E104)

  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_1_value_OFF                 ( 9)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_1_value_WID                 ( 7)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_1_value_MSK                 (0x0000FE00)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_1_value_MIN                 (0)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_1_value_MAX                 (127) // 0x0000007F
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_1_value_DEF                 (0x00000000)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_1_value_HSH                 (0x0792E104)

  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_2_sign_OFF                  (16)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_2_sign_WID                  ( 1)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_2_sign_MSK                  (0x00010000)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_2_sign_MIN                  (0)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_2_sign_MAX                  (1) // 0x00000001
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_2_sign_DEF                  (0x00000000)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_2_sign_HSH                  (0x01A0E104)

  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_2_value_OFF                 (17)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_2_value_WID                 ( 7)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_2_value_MSK                 (0x00FE0000)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_2_value_MIN                 (0)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_2_value_MAX                 (127) // 0x0000007F
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_2_value_DEF                 (0x00000000)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_2_value_HSH                 (0x07A2E104)

  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_3_sign_OFF                  (24)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_3_sign_WID                  ( 1)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_3_sign_MSK                  (0x01000000)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_3_sign_MIN                  (0)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_3_sign_MAX                  (1) // 0x00000001
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_3_sign_DEF                  (0x00000000)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_3_sign_HSH                  (0x01B0E104)

  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_3_value_OFF                 (25)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_3_value_WID                 ( 7)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_3_value_MSK                 (0xFE000000)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_3_value_MIN                 (0)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_3_value_MAX                 (127) // 0x0000007F
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_3_value_DEF                 (0x00000000)
  #define MC0_CH0_CR_TR_RRDVALID_DATA_rank_3_value_HSH                 (0x07B2E104)

#define MC0_CH0_CR_TC_WCK_REG                                          (0x0000E108)
#define MC0_CH0_CR_TC_WCK_DEF                                          (0x4000000000000000)
#define MC0_CH0_CR_TC_WCK_VOLATILE_BITFIELDS_MSK                       (0x0000000000000000)

  #define MC0_CH0_CR_TC_WCK_rd_wck_async_gap_OFF                       ( 0)
  #define MC0_CH0_CR_TC_WCK_rd_wck_async_gap_WID                       (10)
  #define MC0_CH0_CR_TC_WCK_rd_wck_async_gap_MSK                       (0x000003FF)
  #define MC0_CH0_CR_TC_WCK_rd_wck_async_gap_MIN                       (0)
  #define MC0_CH0_CR_TC_WCK_rd_wck_async_gap_MAX                       (511) // 0x000001FF
  #define MC0_CH0_CR_TC_WCK_rd_wck_async_gap_DEF                       (0x00000000)
  #define MC0_CH0_CR_TC_WCK_rd_wck_async_gap_HSH                       (0x4A00E108)

  #define MC0_CH0_CR_TC_WCK_wr_wck_async_gap_OFF                       (10)
  #define MC0_CH0_CR_TC_WCK_wr_wck_async_gap_WID                       (10)
  #define MC0_CH0_CR_TC_WCK_wr_wck_async_gap_MSK                       (0x000FFC00)
  #define MC0_CH0_CR_TC_WCK_wr_wck_async_gap_MIN                       (0)
  #define MC0_CH0_CR_TC_WCK_wr_wck_async_gap_MAX                       (511) // 0x000001FF
  #define MC0_CH0_CR_TC_WCK_wr_wck_async_gap_DEF                       (0x00000000)
  #define MC0_CH0_CR_TC_WCK_wr_wck_async_gap_HSH                       (0x4A14E108)

  #define MC0_CH0_CR_TC_WCK_tWCKSTOP_OFF                               (20)
  #define MC0_CH0_CR_TC_WCK_tWCKSTOP_WID                               ( 6)
  #define MC0_CH0_CR_TC_WCK_tWCKSTOP_MSK                               (0x03F00000)
  #define MC0_CH0_CR_TC_WCK_tWCKSTOP_MIN                               (0)
  #define MC0_CH0_CR_TC_WCK_tWCKSTOP_MAX                               (26) // 0x0000001A
  #define MC0_CH0_CR_TC_WCK_tWCKSTOP_DEF                               (0x00000000)
  #define MC0_CH0_CR_TC_WCK_tWCKSTOP_HSH                               (0x4628E108)

  #define MC0_CH0_CR_TC_WCK_twckoff_OFF                                (32)
  #define MC0_CH0_CR_TC_WCK_twckoff_WID                                ( 9)
  #define MC0_CH0_CR_TC_WCK_twckoff_MSK                                (0x000001FF00000000ULL)
  #define MC0_CH0_CR_TC_WCK_twckoff_MIN                                (0)
  #define MC0_CH0_CR_TC_WCK_twckoff_MAX                                (255) // 0x000000FF
  #define MC0_CH0_CR_TC_WCK_twckoff_DEF                                (0x00000000)
  #define MC0_CH0_CR_TC_WCK_twckoff_HSH                                (0x4940E108)

  #define MC0_CH0_CR_TC_WCK_casstop_additional_gap_OFF                 (41)
  #define MC0_CH0_CR_TC_WCK_casstop_additional_gap_WID                 ( 6)
  #define MC0_CH0_CR_TC_WCK_casstop_additional_gap_MSK                 (0x00007E0000000000ULL)
  #define MC0_CH0_CR_TC_WCK_casstop_additional_gap_MIN                 (0)
  #define MC0_CH0_CR_TC_WCK_casstop_additional_gap_MAX                 (31) // 0x0000001F
  #define MC0_CH0_CR_TC_WCK_casstop_additional_gap_DEF                 (0x00000000)
  #define MC0_CH0_CR_TC_WCK_casstop_additional_gap_HSH                 (0x4652E108)

  #define MC0_CH0_CR_TC_WCK_tCSLCK_OFF                                 (52)
  #define MC0_CH0_CR_TC_WCK_tCSLCK_WID                                 ( 5)
  #define MC0_CH0_CR_TC_WCK_tCSLCK_MSK                                 (0x01F0000000000000ULL)
  #define MC0_CH0_CR_TC_WCK_tCSLCK_MIN                                 (0)
  #define MC0_CH0_CR_TC_WCK_tCSLCK_MAX                                 (26) // 0x0000001A
  #define MC0_CH0_CR_TC_WCK_tCSLCK_DEF                                 (0x00000000)
  #define MC0_CH0_CR_TC_WCK_tCSLCK_HSH                                 (0x4568E108)

  #define MC0_CH0_CR_TC_WCK_tckfspx_OFF                                (58)
  #define MC0_CH0_CR_TC_WCK_tckfspx_WID                                ( 6)
  #define MC0_CH0_CR_TC_WCK_tckfspx_MSK                                (0xFC00000000000000ULL)
  #define MC0_CH0_CR_TC_WCK_tckfspx_MIN                                (16)
  #define MC0_CH0_CR_TC_WCK_tckfspx_MAX                                (32) // 0x00000020
  #define MC0_CH0_CR_TC_WCK_tckfspx_DEF                                (0x00000010)
  #define MC0_CH0_CR_TC_WCK_tckfspx_HSH                                (0x4674E108)

#define MC0_CH0_CR_WMM_READ_CONFIG_REG                                 (0x0000E110)
#define MC0_CH0_CR_WMM_READ_CONFIG_DEF                                 (0x00000046)
#define MC0_CH0_CR_WMM_READ_CONFIG_VOLATILE_BITFIELDS_MSK              (0x00000000)

  #define MC0_CH0_CR_WMM_READ_CONFIG_Dis_Opp_rd_OFF                    ( 0)
  #define MC0_CH0_CR_WMM_READ_CONFIG_Dis_Opp_rd_WID                    ( 1)
  #define MC0_CH0_CR_WMM_READ_CONFIG_Dis_Opp_rd_MSK                    (0x00000001)
  #define MC0_CH0_CR_WMM_READ_CONFIG_Dis_Opp_rd_MIN                    (0)
  #define MC0_CH0_CR_WMM_READ_CONFIG_Dis_Opp_rd_MAX                    (1) // 0x00000001
  #define MC0_CH0_CR_WMM_READ_CONFIG_Dis_Opp_rd_DEF                    (0x00000000)
  #define MC0_CH0_CR_WMM_READ_CONFIG_Dis_Opp_rd_HSH                    (0x0100E110)

  #define MC0_CH0_CR_WMM_READ_CONFIG_ACT_Enable_OFF                    ( 1)
  #define MC0_CH0_CR_WMM_READ_CONFIG_ACT_Enable_WID                    ( 1)
  #define MC0_CH0_CR_WMM_READ_CONFIG_ACT_Enable_MSK                    (0x00000002)
  #define MC0_CH0_CR_WMM_READ_CONFIG_ACT_Enable_MIN                    (0)
  #define MC0_CH0_CR_WMM_READ_CONFIG_ACT_Enable_MAX                    (1) // 0x00000001
  #define MC0_CH0_CR_WMM_READ_CONFIG_ACT_Enable_DEF                    (0x00000001)
  #define MC0_CH0_CR_WMM_READ_CONFIG_ACT_Enable_HSH                    (0x0102E110)

  #define MC0_CH0_CR_WMM_READ_CONFIG_PRE_Enable_OFF                    ( 2)
  #define MC0_CH0_CR_WMM_READ_CONFIG_PRE_Enable_WID                    ( 1)
  #define MC0_CH0_CR_WMM_READ_CONFIG_PRE_Enable_MSK                    (0x00000004)
  #define MC0_CH0_CR_WMM_READ_CONFIG_PRE_Enable_MIN                    (0)
  #define MC0_CH0_CR_WMM_READ_CONFIG_PRE_Enable_MAX                    (1) // 0x00000001
  #define MC0_CH0_CR_WMM_READ_CONFIG_PRE_Enable_DEF                    (0x00000001)
  #define MC0_CH0_CR_WMM_READ_CONFIG_PRE_Enable_HSH                    (0x0104E110)

  #define MC0_CH0_CR_WMM_READ_CONFIG_MAX_RPQ_CAS_OFF                   ( 3)
  #define MC0_CH0_CR_WMM_READ_CONFIG_MAX_RPQ_CAS_WID                   ( 4)
  #define MC0_CH0_CR_WMM_READ_CONFIG_MAX_RPQ_CAS_MSK                   (0x00000078)
  #define MC0_CH0_CR_WMM_READ_CONFIG_MAX_RPQ_CAS_MIN                   (0)
  #define MC0_CH0_CR_WMM_READ_CONFIG_MAX_RPQ_CAS_MAX                   (15) // 0x0000000F
  #define MC0_CH0_CR_WMM_READ_CONFIG_MAX_RPQ_CAS_DEF                   (0x00000008)
  #define MC0_CH0_CR_WMM_READ_CONFIG_MAX_RPQ_CAS_HSH                   (0x0406E110)

#define MC0_CH0_CR_MC2PHY_BGF_CTRL_REG                                 (0x0000E114)
#define MC0_CH0_CR_MC2PHY_BGF_CTRL_DEF                                 (0x11F20411)
#define MC0_CH0_CR_MC2PHY_BGF_CTRL_VOLATILE_BITFIELDS_MSK              (0x00006000)

  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_PtrSep_ispid_OFF                  ( 0)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_PtrSep_ispid_WID                  ( 2)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_PtrSep_ispid_MSK                  (0x00000003)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_PtrSep_ispid_MIN                  (0)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_PtrSep_ispid_MAX                  (3) // 0x00000003
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_PtrSep_ispid_DEF                  (0x00000001)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_PtrSep_ispid_HSH                  (0x0200E114)

  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRun_Ovrd_En_OFF                ( 2)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRun_Ovrd_En_WID                ( 1)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRun_Ovrd_En_MSK                (0x00000004)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRun_Ovrd_En_MIN                (0)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRun_Ovrd_En_MAX                (1) // 0x00000001
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRun_Ovrd_En_DEF                (0x00000000)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRun_Ovrd_En_HSH                (0x0104E114)

  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRun_Ovrd_Val_OFF               ( 3)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRun_Ovrd_Val_WID               ( 1)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRun_Ovrd_Val_MSK               (0x00000008)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRun_Ovrd_Val_MIN               (0)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRun_Ovrd_Val_MAX               (1) // 0x00000001
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRun_Ovrd_Val_DEF               (0x00000000)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRun_Ovrd_Val_HSH               (0x0106E114)

  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRun_Delay_OFF                  ( 4)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRun_Delay_WID                  ( 5)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRun_Delay_MSK                  (0x000001F0)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRun_Delay_MIN                  (0)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRun_Delay_MAX                  (31) // 0x0000001F
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRun_Delay_DEF                  (0x00000001)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRun_Delay_HSH                  (0x0508E114)

  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_clkStop_override_en_on_BGF_Off_OFF ( 9)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_clkStop_override_en_on_BGF_Off_WID ( 1)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_clkStop_override_en_on_BGF_Off_MSK (0x00000200)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_clkStop_override_en_on_BGF_Off_MIN (0)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_clkStop_override_en_on_BGF_Off_MAX (1) // 0x00000001
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_clkStop_override_en_on_BGF_Off_DEF (0x00000000)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_clkStop_override_en_on_BGF_Off_HSH (0x0112E114)

  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_clkStop_override_val_on_BGF_Off_OFF (10)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_clkStop_override_val_on_BGF_Off_WID ( 1)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_clkStop_override_val_on_BGF_Off_MSK (0x00000400)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_clkStop_override_val_on_BGF_Off_MIN (0)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_clkStop_override_val_on_BGF_Off_MAX (1) // 0x00000001
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_clkStop_override_val_on_BGF_Off_DEF (0x00000001)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_clkStop_override_val_on_BGF_Off_HSH (0x0114E114)

  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_Enable_osc_override_OFF           (11)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_Enable_osc_override_WID           ( 1)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_Enable_osc_override_MSK           (0x00000800)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_Enable_osc_override_MIN           (0)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_Enable_osc_override_MAX           (1) // 0x00000001
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_Enable_osc_override_DEF           (0x00000000)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_Enable_osc_override_HSH           (0x0116E114)

  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_Enable_osc_check_OFF              (12)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_Enable_osc_check_WID              ( 1)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_Enable_osc_check_MSK              (0x00001000)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_Enable_osc_check_MIN              (0)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_Enable_osc_check_MAX              (1) // 0x00000001
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_Enable_osc_check_DEF              (0x00000000)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_Enable_osc_check_HSH              (0x0118E114)

  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_MC_2_PHY_mismatch_OFF             (13)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_MC_2_PHY_mismatch_WID             ( 1)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_MC_2_PHY_mismatch_MSK             (0x00002000)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_MC_2_PHY_mismatch_MIN             (0)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_MC_2_PHY_mismatch_MAX             (1) // 0x00000001
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_MC_2_PHY_mismatch_DEF             (0x00000000)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_MC_2_PHY_mismatch_HSH             (0x019AE114)

  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_PHY_2_MC_mismatch_OFF             (14)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_PHY_2_MC_mismatch_WID             ( 1)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_PHY_2_MC_mismatch_MSK             (0x00004000)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_PHY_2_MC_mismatch_MIN             (0)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_PHY_2_MC_mismatch_MAX             (1) // 0x00000001
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_PHY_2_MC_mismatch_DEF             (0x00000000)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_PHY_2_MC_mismatch_HSH             (0x019CE114)

  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_force_off_cmd_bgf_valid_OFF       (15)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_force_off_cmd_bgf_valid_WID       ( 1)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_force_off_cmd_bgf_valid_MSK       (0x00008000)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_force_off_cmd_bgf_valid_MIN       (0)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_force_off_cmd_bgf_valid_MAX       (1) // 0x00000001
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_force_off_cmd_bgf_valid_DEF       (0x00000000)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_force_off_cmd_bgf_valid_HSH       (0x011EE114)

  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_PtrSep_ospid_OFF                  (17)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_PtrSep_ospid_WID                  ( 2)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_PtrSep_ospid_MSK                  (0x00060000)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_PtrSep_ospid_MIN                  (0)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_PtrSep_ospid_MAX                  (3) // 0x00000003
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_PtrSep_ospid_DEF                  (0x00000001)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_PtrSep_ospid_HSH                  (0x0222E114)

  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cstristate_override_en_on_BGF_Off_OFF (19)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cstristate_override_en_on_BGF_Off_WID ( 1)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cstristate_override_en_on_BGF_Off_MSK (0x00080000)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cstristate_override_en_on_BGF_Off_MIN (0)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cstristate_override_en_on_BGF_Off_MAX (1) // 0x00000001
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cstristate_override_en_on_BGF_Off_DEF (0x00000000)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cstristate_override_en_on_BGF_Off_HSH (0x0126E114)

  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cstristate_override_val_on_BGF_Off_OFF (20)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cstristate_override_val_on_BGF_Off_WID ( 1)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cstristate_override_val_on_BGF_Off_MSK (0x00100000)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cstristate_override_val_on_BGF_Off_MIN (0)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cstristate_override_val_on_BGF_Off_MAX (1) // 0x00000001
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cstristate_override_val_on_BGF_Off_DEF (0x00000001)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cstristate_override_val_on_BGF_Off_HSH (0x0128E114)

  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRst_Delay_OFF                  (21)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRst_Delay_WID                  ( 6)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRst_Delay_MSK                  (0x07E00000)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRst_Delay_MIN                  (0)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRst_Delay_MAX                  (63) // 0x0000003F
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRst_Delay_DEF                  (0x0000000F)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_BgfRst_Delay_HSH                  (0x062AE114)

  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cs_override_en_on_BGF_Off_OFF     (27)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cs_override_en_on_BGF_Off_WID     ( 1)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cs_override_en_on_BGF_Off_MSK     (0x08000000)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cs_override_en_on_BGF_Off_MIN     (0)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cs_override_en_on_BGF_Off_MAX     (1) // 0x00000001
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cs_override_en_on_BGF_Off_DEF     (0x00000000)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cs_override_en_on_BGF_Off_HSH     (0x0136E114)

  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cs_override_val_on_BGF_Off_OFF    (28)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cs_override_val_on_BGF_Off_WID    ( 4)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cs_override_val_on_BGF_Off_MSK    (0xF0000000)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cs_override_val_on_BGF_Off_MIN    (0)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cs_override_val_on_BGF_Off_MAX    (15) // 0x0000000F
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cs_override_val_on_BGF_Off_DEF    (0x00000001)
  #define MC0_CH0_CR_MC2PHY_BGF_CTRL_cs_override_val_on_BGF_Off_HSH    (0x0438E114)

#define MC0_CH0_CR_SC_ADAPTIVE_PCIT_REG                                (0x0000E118)
#define MC0_CH0_CR_SC_ADAPTIVE_PCIT_DEF                                (0x0001B20BD8008003)
#define MC0_CH0_CR_SC_ADAPTIVE_PCIT_VOLATILE_BITFIELDS_MSK             (0x0000000000000000)

  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_ADAPTIVE_PCIT_WINDOW_MULT_OFF    ( 0)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_ADAPTIVE_PCIT_WINDOW_MULT_WID    ( 3)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_ADAPTIVE_PCIT_WINDOW_MULT_MSK    (0x00000007)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_ADAPTIVE_PCIT_WINDOW_MULT_MIN    (0)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_ADAPTIVE_PCIT_WINDOW_MULT_MAX    (7) // 0x00000007
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_ADAPTIVE_PCIT_WINDOW_MULT_DEF    (0x00000003)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_ADAPTIVE_PCIT_WINDOW_MULT_HSH    (0x4300E118)

  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_MIN_PCIT_OFF                     (10)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_MIN_PCIT_WID                     (10)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_MIN_PCIT_MSK                     (0x000FFC00)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_MIN_PCIT_MIN                     (0)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_MIN_PCIT_MAX                     (1023) // 0x000003FF
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_MIN_PCIT_DEF                     (0x00000020)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_MIN_PCIT_HSH                     (0x4A14E118)

  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_MAX_PCIT_OFF                     (20)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_MAX_PCIT_WID                     (10)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_MAX_PCIT_MSK                     (0x3FF00000)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_MAX_PCIT_MIN                     (0)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_MAX_PCIT_MAX                     (1023) // 0x000003FF
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_MAX_PCIT_DEF                     (0x00000180)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_MAX_PCIT_HSH                     (0x4A28E118)

  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_CHANGE_RANGE_OFF                 (30)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_CHANGE_RANGE_WID                 ( 2)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_CHANGE_RANGE_MSK                 (0xC0000000)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_CHANGE_RANGE_MIN                 (0)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_CHANGE_RANGE_MAX                 (3) // 0x00000003
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_CHANGE_RANGE_DEF                 (0x00000003)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_CHANGE_RANGE_HSH                 (0x423CE118)

  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_EARLY_WEIGHT_OFF                 (32)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_EARLY_WEIGHT_WID                 ( 3)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_EARLY_WEIGHT_MSK                 (0x0000000700000000ULL)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_EARLY_WEIGHT_MIN                 (0)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_EARLY_WEIGHT_MAX                 (4) // 0x00000004
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_EARLY_WEIGHT_DEF                 (0x00000003)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_EARLY_WEIGHT_HSH                 (0x4340E118)

  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_LATE_WEIGHT_OFF                  (35)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_LATE_WEIGHT_WID                  ( 3)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_LATE_WEIGHT_MSK                  (0x0000003800000000ULL)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_LATE_WEIGHT_MIN                  (0)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_LATE_WEIGHT_MAX                  (4) // 0x00000004
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_LATE_WEIGHT_DEF                  (0x00000001)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_LATE_WEIGHT_HSH                  (0x4346E118)

  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_PH_WEIGHT_OFF                    (38)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_PH_WEIGHT_WID                    ( 3)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_PH_WEIGHT_MSK                    (0x000001C000000000ULL)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_PH_WEIGHT_MIN                    (0)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_PH_WEIGHT_MAX                    (4) // 0x00000004
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_PH_WEIGHT_DEF                    (0x00000000)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_PH_WEIGHT_HSH                    (0x434CE118)

  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_GOOD_PAGE_CLOSE_WEIGHT_OFF       (41)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_GOOD_PAGE_CLOSE_WEIGHT_WID       ( 3)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_GOOD_PAGE_CLOSE_WEIGHT_MSK       (0x00000E0000000000ULL)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_GOOD_PAGE_CLOSE_WEIGHT_MIN       (0)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_GOOD_PAGE_CLOSE_WEIGHT_MAX       (4) // 0x00000004
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_GOOD_PAGE_CLOSE_WEIGHT_DEF       (0x00000001)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_GOOD_PAGE_CLOSE_WEIGHT_HSH       (0x4352E118)

  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_TRANSACTION_FACTOR_OFF           (44)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_TRANSACTION_FACTOR_WID           ( 3)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_TRANSACTION_FACTOR_MSK           (0x0000700000000000ULL)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_TRANSACTION_FACTOR_MIN           (0)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_TRANSACTION_FACTOR_MAX           (4) // 0x00000004
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_TRANSACTION_FACTOR_DEF           (0x00000003)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_TRANSACTION_FACTOR_HSH           (0x4358E118)

  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_IGNORE_VC1_OFF                   (47)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_IGNORE_VC1_WID                   ( 1)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_IGNORE_VC1_MSK                   (0x0000800000000000ULL)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_IGNORE_VC1_MIN                   (0)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_IGNORE_VC1_MAX                   (1) // 0x00000001
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_IGNORE_VC1_DEF                   (0x00000001)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_IGNORE_VC1_HSH                   (0x415EE118)

  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_IGNORE_GT_OFF                    (48)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_IGNORE_GT_WID                    ( 1)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_IGNORE_GT_MSK                    (0x0001000000000000ULL)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_IGNORE_GT_MIN                    (0)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_IGNORE_GT_MAX                    (1) // 0x00000001
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_IGNORE_GT_DEF                    (0x00000001)
  #define MC0_CH0_CR_SC_ADAPTIVE_PCIT_IGNORE_GT_HSH                    (0x4160E118)

#define MC0_CH0_CR_ROWHAMMER_CTL_REG                                   (0x0000E120)
#define MC0_CH0_CR_ROWHAMMER_CTL_DEF                                   (0x1801600800040180)
#define MC0_CH0_CR_ROWHAMMER_CTL_VOLATILE_BITFIELDS_MSK                (0x0000000000000000)

  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_ENABLED_DIMM0_OFF                ( 0)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_ENABLED_DIMM0_WID                ( 1)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_ENABLED_DIMM0_MSK                (0x00000001)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_ENABLED_DIMM0_MIN                (0)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_ENABLED_DIMM0_MAX                (1) // 0x00000001
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_ENABLED_DIMM0_DEF                (0x00000000)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_ENABLED_DIMM0_HSH                (0x4100E120)

  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_ENABLED_DIMM1_OFF                ( 1)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_ENABLED_DIMM1_WID                ( 1)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_ENABLED_DIMM1_MSK                (0x00000002)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_ENABLED_DIMM1_MIN                (0)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_ENABLED_DIMM1_MAX                (1) // 0x00000001
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_ENABLED_DIMM1_DEF                (0x00000000)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_ENABLED_DIMM1_HSH                (0x4102E120)

  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_HIGH_WM_OFF                      ( 2)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_HIGH_WM_WID                      (12)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_HIGH_WM_MSK                      (0x00003FFC)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_HIGH_WM_MIN                      (0)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_HIGH_WM_MAX                      (4095) // 0x00000FFF
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_HIGH_WM_DEF                      (0x00000060)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_HIGH_WM_HSH                      (0x4C04E120)

  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_LOW_WM_OFF                       (14)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_LOW_WM_WID                       (11)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_LOW_WM_MSK                       (0x01FFC000)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_LOW_WM_MIN                       (0)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_LOW_WM_MAX                       (2047) // 0x000007FF
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_LOW_WM_DEF                       (0x00000010)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RH_LOW_WM_HSH                       (0x4B1CE120)

  #define MC0_CH0_CR_ROWHAMMER_CTL_NORMAL_REF_SUB_OFF                  (32)
  #define MC0_CH0_CR_ROWHAMMER_CTL_NORMAL_REF_SUB_WID                  (12)
  #define MC0_CH0_CR_ROWHAMMER_CTL_NORMAL_REF_SUB_MSK                  (0x00000FFF00000000ULL)
  #define MC0_CH0_CR_ROWHAMMER_CTL_NORMAL_REF_SUB_MIN                  (0)
  #define MC0_CH0_CR_ROWHAMMER_CTL_NORMAL_REF_SUB_MAX                  (4095) // 0x00000FFF
  #define MC0_CH0_CR_ROWHAMMER_CTL_NORMAL_REF_SUB_DEF                  (0x00000008)
  #define MC0_CH0_CR_ROWHAMMER_CTL_NORMAL_REF_SUB_HSH                  (0x4C40E120)

  #define MC0_CH0_CR_ROWHAMMER_CTL_REFM_SUB_OFF                        (44)
  #define MC0_CH0_CR_ROWHAMMER_CTL_REFM_SUB_WID                        (12)
  #define MC0_CH0_CR_ROWHAMMER_CTL_REFM_SUB_MSK                        (0x00FFF00000000000ULL)
  #define MC0_CH0_CR_ROWHAMMER_CTL_REFM_SUB_MIN                        (0)
  #define MC0_CH0_CR_ROWHAMMER_CTL_REFM_SUB_MAX                        (4095) // 0x00000FFF
  #define MC0_CH0_CR_ROWHAMMER_CTL_REFM_SUB_DEF                        (0x00000016)
  #define MC0_CH0_CR_ROWHAMMER_CTL_REFM_SUB_HSH                        (0x4C58E120)

  #define MC0_CH0_CR_ROWHAMMER_CTL_REFm_EN_OFF                         (59)
  #define MC0_CH0_CR_ROWHAMMER_CTL_REFm_EN_WID                         ( 1)
  #define MC0_CH0_CR_ROWHAMMER_CTL_REFm_EN_MSK                         (0x0800000000000000ULL)
  #define MC0_CH0_CR_ROWHAMMER_CTL_REFm_EN_MIN                         (0)
  #define MC0_CH0_CR_ROWHAMMER_CTL_REFm_EN_MAX                         (1) // 0x00000001
  #define MC0_CH0_CR_ROWHAMMER_CTL_REFm_EN_DEF                         (0x00000001)
  #define MC0_CH0_CR_ROWHAMMER_CTL_REFm_EN_HSH                         (0x4176E120)

  #define MC0_CH0_CR_ROWHAMMER_CTL_RIR_DEC_OFF                         (60)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RIR_DEC_WID                         ( 1)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RIR_DEC_MSK                         (0x1000000000000000ULL)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RIR_DEC_MIN                         (0)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RIR_DEC_MAX                         (1) // 0x00000001
  #define MC0_CH0_CR_ROWHAMMER_CTL_RIR_DEC_DEF                         (0x00000001)
  #define MC0_CH0_CR_ROWHAMMER_CTL_RIR_DEC_HSH                         (0x4178E120)

#define MC0_CH0_CR_MERGE_REQ_READS_PQ_REG                              (0x0000E128)
#define MC0_CH0_CR_MERGE_REQ_READS_PQ_DEF                              (0x000008E0)
#define MC0_CH0_CR_MERGE_REQ_READS_PQ_VOLATILE_BITFIELDS_MSK           (0x00000000)

  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_ipq_req_merge_OFF       ( 5)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_ipq_req_merge_WID       ( 1)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_ipq_req_merge_MSK       (0x00000020)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_ipq_req_merge_MIN       (0)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_ipq_req_merge_MAX       (1) // 0x00000001
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_ipq_req_merge_DEF       (0x00000001)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_ipq_req_merge_HSH       (0x010AE128)

  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_merge_req_vc1_low_prio_OFF ( 6)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_merge_req_vc1_low_prio_WID ( 1)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_merge_req_vc1_low_prio_MSK (0x00000040)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_merge_req_vc1_low_prio_MIN (0)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_merge_req_vc1_low_prio_MAX (1) // 0x00000001
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_merge_req_vc1_low_prio_DEF (0x00000001)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_merge_req_vc1_low_prio_HSH (0x010CE128)

  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_rpq_req_merge_OFF       ( 7)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_rpq_req_merge_WID       ( 1)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_rpq_req_merge_MSK       (0x00000080)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_rpq_req_merge_MIN       (0)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_rpq_req_merge_MAX       (1) // 0x00000001
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_rpq_req_merge_DEF       (0x00000001)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_rpq_req_merge_HSH       (0x010EE128)

  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_rpq_req_merge_reuse_OFF ( 8)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_rpq_req_merge_reuse_WID ( 1)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_rpq_req_merge_reuse_MSK (0x00000100)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_rpq_req_merge_reuse_MIN (0)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_rpq_req_merge_reuse_MAX (1) // 0x00000001
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_rpq_req_merge_reuse_DEF (0x00000000)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_rpq_req_merge_reuse_HSH (0x0110E128)

  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_ipq_req_merge_reuse_OFF ( 9)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_ipq_req_merge_reuse_WID ( 1)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_ipq_req_merge_reuse_MSK (0x00000200)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_ipq_req_merge_reuse_MIN (0)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_ipq_req_merge_reuse_MAX (1) // 0x00000001
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_ipq_req_merge_reuse_DEF (0x00000000)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_ipq_req_merge_reuse_HSH (0x0112E128)

  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_disable_dyn_rpq_req_merge_OFF  (10)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_disable_dyn_rpq_req_merge_WID  ( 1)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_disable_dyn_rpq_req_merge_MSK  (0x00000400)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_disable_dyn_rpq_req_merge_MIN  (0)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_disable_dyn_rpq_req_merge_MAX  (1) // 0x00000001
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_disable_dyn_rpq_req_merge_DEF  (0x00000000)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_disable_dyn_rpq_req_merge_HSH  (0x0114E128)

  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_merge_ipq_when_old_vc1_req_exist_OFF (11)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_merge_ipq_when_old_vc1_req_exist_WID ( 1)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_merge_ipq_when_old_vc1_req_exist_MSK (0x00000800)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_merge_ipq_when_old_vc1_req_exist_MIN (0)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_merge_ipq_when_old_vc1_req_exist_MAX (1) // 0x00000001
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_merge_ipq_when_old_vc1_req_exist_DEF (0x00000001)
  #define MC0_CH0_CR_MERGE_REQ_READS_PQ_enable_merge_ipq_when_old_vc1_req_exist_HSH (0x0116E128)

#define MC0_CH0_CR_DFI_CTL_REG                                         (0x0000E12C)
#define MC0_CH0_CR_DFI_CTL_DEF                                         (0x00000000)
#define MC0_CH0_CR_DFI_CTL_VOLATILE_BITFIELDS_MSK                      (0x00000000)

  #define MC0_CH0_CR_DFI_CTL_lp_ctrl_ack_enable_OFF                    ( 0)
  #define MC0_CH0_CR_DFI_CTL_lp_ctrl_ack_enable_WID                    ( 1)
  #define MC0_CH0_CR_DFI_CTL_lp_ctrl_ack_enable_MSK                    (0x00000001)
  #define MC0_CH0_CR_DFI_CTL_lp_ctrl_ack_enable_MIN                    (0)
  #define MC0_CH0_CR_DFI_CTL_lp_ctrl_ack_enable_MAX                    (1) // 0x00000001
  #define MC0_CH0_CR_DFI_CTL_lp_ctrl_ack_enable_DEF                    (0x00000000)
  #define MC0_CH0_CR_DFI_CTL_lp_ctrl_ack_enable_HSH                    (0x0100E12C)

#define MC0_CH0_CR_AUTO_PRE_CONTROL_REG                                (0x0000E130)
#define MC0_CH0_CR_AUTO_PRE_CONTROL_DEF                                (0x0000000000001421)
#define MC0_CH0_CR_AUTO_PRE_CONTROL_VOLATILE_BITFIELDS_MSK             (0x0000000000000000)

  #define MC0_CH0_CR_AUTO_PRE_CONTROL_AUTO_PRE_EN_OFF                  ( 0)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_AUTO_PRE_EN_WID                  ( 1)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_AUTO_PRE_EN_MSK                  (0x00000001)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_AUTO_PRE_EN_MIN                  (0)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_AUTO_PRE_EN_MAX                  (1) // 0x00000001
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_AUTO_PRE_EN_DEF                  (0x00000001)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_AUTO_PRE_EN_HSH                  (0x4100E130)

  #define MC0_CH0_CR_AUTO_PRE_CONTROL_RPQ_AP_PM_OFF                    ( 1)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_RPQ_AP_PM_WID                    ( 1)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_RPQ_AP_PM_MSK                    (0x00000002)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_RPQ_AP_PM_MIN                    (0)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_RPQ_AP_PM_MAX                    (1) // 0x00000001
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_RPQ_AP_PM_DEF                    (0x00000000)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_RPQ_AP_PM_HSH                    (0x4102E130)

  #define MC0_CH0_CR_AUTO_PRE_CONTROL_RPQ_AP_THRESHOLD_OFF             ( 2)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_RPQ_AP_THRESHOLD_WID             ( 6)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_RPQ_AP_THRESHOLD_MSK             (0x000000FC)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_RPQ_AP_THRESHOLD_MIN             (0)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_RPQ_AP_THRESHOLD_MAX             (63) // 0x0000003F
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_RPQ_AP_THRESHOLD_DEF             (0x00000008)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_RPQ_AP_THRESHOLD_HSH             (0x4604E130)

  #define MC0_CH0_CR_AUTO_PRE_CONTROL_WPQ_AP_THRESHOLD_OFF             ( 8)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_WPQ_AP_THRESHOLD_WID             ( 7)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_WPQ_AP_THRESHOLD_MSK             (0x00007F00)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_WPQ_AP_THRESHOLD_MIN             (0)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_WPQ_AP_THRESHOLD_MAX             (127) // 0x0000007F
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_WPQ_AP_THRESHOLD_DEF             (0x00000014)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_WPQ_AP_THRESHOLD_HSH             (0x4710E130)

  #define MC0_CH0_CR_AUTO_PRE_CONTROL_LPDDR5_AUTO_PRE_EN_OFF           (15)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_LPDDR5_AUTO_PRE_EN_WID           ( 1)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_LPDDR5_AUTO_PRE_EN_MSK           (0x00008000)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_LPDDR5_AUTO_PRE_EN_MIN           (0)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_LPDDR5_AUTO_PRE_EN_MAX           (1) // 0x00000001
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_LPDDR5_AUTO_PRE_EN_DEF           (0x00000000)
  #define MC0_CH0_CR_AUTO_PRE_CONTROL_LPDDR5_AUTO_PRE_EN_HSH           (0x411EE130)

#define MC0_CH0_CR_TC_ACT_REG                                          (0x0000E138)
#define MC0_CH0_CR_TC_ACT_DEF                                          (0x0000000802020810)
#define MC0_CH0_CR_TC_ACT_VOLATILE_BITFIELDS_MSK                       (0x0000000000000000)

  #define MC0_CH0_CR_TC_ACT_tFAW_OFF                                   ( 0)
  #define MC0_CH0_CR_TC_ACT_tFAW_WID                                   ( 9)
  #define MC0_CH0_CR_TC_ACT_tFAW_MSK                                   (0x000001FF)
  #define MC0_CH0_CR_TC_ACT_tFAW_MIN                                   (16)
  #define MC0_CH0_CR_TC_ACT_tFAW_MAX                                   (138) // 0x0000008A
  #define MC0_CH0_CR_TC_ACT_tFAW_DEF                                   (0x00000010)
  #define MC0_CH0_CR_TC_ACT_tFAW_HSH                                   (0x4900E138)

  #define MC0_CH0_CR_TC_ACT_tRRD_sg_OFF                                ( 9)
  #define MC0_CH0_CR_TC_ACT_tRRD_sg_WID                                ( 6)
  #define MC0_CH0_CR_TC_ACT_tRRD_sg_MSK                                (0x00007E00)
  #define MC0_CH0_CR_TC_ACT_tRRD_sg_MIN                                (4)
  #define MC0_CH0_CR_TC_ACT_tRRD_sg_MAX                                (32) // 0x00000020
  #define MC0_CH0_CR_TC_ACT_tRRD_sg_DEF                                (0x00000004)
  #define MC0_CH0_CR_TC_ACT_tRRD_sg_HSH                                (0x4612E138)

  #define MC0_CH0_CR_TC_ACT_tRRD_dg_OFF                                (15)
  #define MC0_CH0_CR_TC_ACT_tRRD_dg_WID                                ( 7)
  #define MC0_CH0_CR_TC_ACT_tRRD_dg_MSK                                (0x003F8000)
  #define MC0_CH0_CR_TC_ACT_tRRD_dg_MIN                                (4)
  #define MC0_CH0_CR_TC_ACT_tRRD_dg_MAX                                (32) // 0x00000020
  #define MC0_CH0_CR_TC_ACT_tRRD_dg_DEF                                (0x00000004)
  #define MC0_CH0_CR_TC_ACT_tRRD_dg_HSH                                (0x471EE138)

  #define MC0_CH0_CR_TC_ACT_tRCD_OFF                                   (22)
  #define MC0_CH0_CR_TC_ACT_tRCD_WID                                   ( 8)
  #define MC0_CH0_CR_TC_ACT_tRCD_MSK                                   (0x3FC00000)
  #define MC0_CH0_CR_TC_ACT_tRCD_MIN                                   (4)
  #define MC0_CH0_CR_TC_ACT_tRCD_MAX                                   (76) // 0x0000004C
  #define MC0_CH0_CR_TC_ACT_tRCD_DEF                                   (0x00000008)
  #define MC0_CH0_CR_TC_ACT_tRCD_HSH                                   (0x482CE138)

  #define MC0_CH0_CR_TC_ACT_tRCDW_OFF                                  (32)
  #define MC0_CH0_CR_TC_ACT_tRCDW_WID                                  ( 8)
  #define MC0_CH0_CR_TC_ACT_tRCDW_MSK                                  (0x000000FF00000000ULL)
  #define MC0_CH0_CR_TC_ACT_tRCDW_MIN                                  (4)
  #define MC0_CH0_CR_TC_ACT_tRCDW_MAX                                  (76) // 0x0000004C
  #define MC0_CH0_CR_TC_ACT_tRCDW_DEF                                  (0x00000008)
  #define MC0_CH0_CR_TC_ACT_tRCDW_HSH                                  (0x4840E138)

  #define MC0_CH0_CR_TC_ACT_RDA2ACT_OFF                                (40)
  #define MC0_CH0_CR_TC_ACT_RDA2ACT_WID                                ( 9)
  #define MC0_CH0_CR_TC_ACT_RDA2ACT_MSK                                (0x0001FF0000000000ULL)
  #define MC0_CH0_CR_TC_ACT_RDA2ACT_MIN                                (0)
  #define MC0_CH0_CR_TC_ACT_RDA2ACT_MAX                                (511) // 0x000001FF
  #define MC0_CH0_CR_TC_ACT_RDA2ACT_DEF                                (0x00000000)
  #define MC0_CH0_CR_TC_ACT_RDA2ACT_HSH                                (0x4950E138)

  #define MC0_CH0_CR_TC_ACT_WRA2ACT_OFF                                (49)
  #define MC0_CH0_CR_TC_ACT_WRA2ACT_WID                                (10)
  #define MC0_CH0_CR_TC_ACT_WRA2ACT_MSK                                (0x07FE000000000000ULL)
  #define MC0_CH0_CR_TC_ACT_WRA2ACT_MIN                                (0)
  #define MC0_CH0_CR_TC_ACT_WRA2ACT_MAX                                (1023) // 0x000003FF
  #define MC0_CH0_CR_TC_ACT_WRA2ACT_DEF                                (0x00000000)
  #define MC0_CH0_CR_TC_ACT_WRA2ACT_HSH                                (0x4A62E138)

#define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_RPQ_REG                         (0x0000E140)
#define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_RPQ_DEF                         (0x0000000000000000)
#define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_RPQ_VOLATILE_BITFIELDS_MSK      (0x0000000000000000)

  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_RPQ_RPQ_disable_OFF           ( 0)
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_RPQ_RPQ_disable_WID           (48)
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_RPQ_RPQ_disable_MSK           (0x0000FFFFFFFFFFFFULL)
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_RPQ_RPQ_disable_MIN           (0)
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_RPQ_RPQ_disable_MAX           (281474976710655ULL) // 0xFFFFFFFFFFFF
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_RPQ_RPQ_disable_DEF           (0x00000000)
  #define MC0_CH0_CR_QUEUE_ENTRY_DISABLE_RPQ_RPQ_disable_HSH           (0x7000E140)

#define MC0_CH0_CR_WCK_CONFIG_REG                                      (0x0000E148)
#define MC0_CH0_CR_WCK_CONFIG_DEF                                      (0x0000208000000000)
#define MC0_CH0_CR_WCK_CONFIG_VOLATILE_BITFIELDS_MSK                   (0x0000000000000000)

  #define MC0_CH0_CR_WCK_CONFIG_twck_en_rd_OFF                         ( 0)
  #define MC0_CH0_CR_WCK_CONFIG_twck_en_rd_WID                         ( 6)
  #define MC0_CH0_CR_WCK_CONFIG_twck_en_rd_MSK                         (0x0000003F)
  #define MC0_CH0_CR_WCK_CONFIG_twck_en_rd_MIN                         (0)
  #define MC0_CH0_CR_WCK_CONFIG_twck_en_rd_MAX                         (63) // 0x0000003F
  #define MC0_CH0_CR_WCK_CONFIG_twck_en_rd_DEF                         (0x00000000)
  #define MC0_CH0_CR_WCK_CONFIG_twck_en_rd_HSH                         (0x4600E148)

  #define MC0_CH0_CR_WCK_CONFIG_twck_toggle_rd_OFF                     ( 6)
  #define MC0_CH0_CR_WCK_CONFIG_twck_toggle_rd_WID                     ( 7)
  #define MC0_CH0_CR_WCK_CONFIG_twck_toggle_rd_MSK                     (0x00001FC0)
  #define MC0_CH0_CR_WCK_CONFIG_twck_toggle_rd_MIN                     (0)
  #define MC0_CH0_CR_WCK_CONFIG_twck_toggle_rd_MAX                     (127) // 0x0000007F
  #define MC0_CH0_CR_WCK_CONFIG_twck_toggle_rd_DEF                     (0x00000000)
  #define MC0_CH0_CR_WCK_CONFIG_twck_toggle_rd_HSH                     (0x470CE148)

  #define MC0_CH0_CR_WCK_CONFIG_twck_en_wr_OFF                         (13)
  #define MC0_CH0_CR_WCK_CONFIG_twck_en_wr_WID                         ( 6)
  #define MC0_CH0_CR_WCK_CONFIG_twck_en_wr_MSK                         (0x0007E000)
  #define MC0_CH0_CR_WCK_CONFIG_twck_en_wr_MIN                         (0)
  #define MC0_CH0_CR_WCK_CONFIG_twck_en_wr_MAX                         (63) // 0x0000003F
  #define MC0_CH0_CR_WCK_CONFIG_twck_en_wr_DEF                         (0x00000000)
  #define MC0_CH0_CR_WCK_CONFIG_twck_en_wr_HSH                         (0x461AE148)

  #define MC0_CH0_CR_WCK_CONFIG_twck_toggle_wr_OFF                     (19)
  #define MC0_CH0_CR_WCK_CONFIG_twck_toggle_wr_WID                     ( 7)
  #define MC0_CH0_CR_WCK_CONFIG_twck_toggle_wr_MSK                     (0x03F80000)
  #define MC0_CH0_CR_WCK_CONFIG_twck_toggle_wr_MIN                     (0)
  #define MC0_CH0_CR_WCK_CONFIG_twck_toggle_wr_MAX                     (127) // 0x0000007F
  #define MC0_CH0_CR_WCK_CONFIG_twck_toggle_wr_DEF                     (0x00000000)
  #define MC0_CH0_CR_WCK_CONFIG_twck_toggle_wr_HSH                     (0x4726E148)

  #define MC0_CH0_CR_WCK_CONFIG_twck_en_fs_OFF                         (26)
  #define MC0_CH0_CR_WCK_CONFIG_twck_en_fs_WID                         ( 5)
  #define MC0_CH0_CR_WCK_CONFIG_twck_en_fs_MSK                         (0x7C000000)
  #define MC0_CH0_CR_WCK_CONFIG_twck_en_fs_MIN                         (0)
  #define MC0_CH0_CR_WCK_CONFIG_twck_en_fs_MAX                         (31) // 0x0000001F
  #define MC0_CH0_CR_WCK_CONFIG_twck_en_fs_DEF                         (0x00000000)
  #define MC0_CH0_CR_WCK_CONFIG_twck_en_fs_HSH                         (0x4534E148)

  #define MC0_CH0_CR_WCK_CONFIG_twck_toggle_fs_OFF                     (32)
  #define MC0_CH0_CR_WCK_CONFIG_twck_toggle_fs_WID                     ( 5)
  #define MC0_CH0_CR_WCK_CONFIG_twck_toggle_fs_MSK                     (0x0000001F00000000ULL)
  #define MC0_CH0_CR_WCK_CONFIG_twck_toggle_fs_MIN                     (0)
  #define MC0_CH0_CR_WCK_CONFIG_twck_toggle_fs_MAX                     (31) // 0x0000001F
  #define MC0_CH0_CR_WCK_CONFIG_twck_toggle_fs_DEF                     (0x00000000)
  #define MC0_CH0_CR_WCK_CONFIG_twck_toggle_fs_HSH                     (0x4540E148)

  #define MC0_CH0_CR_WCK_CONFIG_twck_dfi_offset_OFF                    (37)
  #define MC0_CH0_CR_WCK_CONFIG_twck_dfi_offset_WID                    ( 3)
  #define MC0_CH0_CR_WCK_CONFIG_twck_dfi_offset_MSK                    (0x000000E000000000ULL)
  #define MC0_CH0_CR_WCK_CONFIG_twck_dfi_offset_MIN                    (0)
  #define MC0_CH0_CR_WCK_CONFIG_twck_dfi_offset_MAX                    (7) // 0x00000007
  #define MC0_CH0_CR_WCK_CONFIG_twck_dfi_offset_DEF                    (0x00000004)
  #define MC0_CH0_CR_WCK_CONFIG_twck_dfi_offset_HSH                    (0x434AE148)

  #define MC0_CH0_CR_WCK_CONFIG_twck_dfi_stop_offset_OFF               (40)
  #define MC0_CH0_CR_WCK_CONFIG_twck_dfi_stop_offset_WID               ( 3)
  #define MC0_CH0_CR_WCK_CONFIG_twck_dfi_stop_offset_MSK               (0x0000070000000000ULL)
  #define MC0_CH0_CR_WCK_CONFIG_twck_dfi_stop_offset_MIN               (0)
  #define MC0_CH0_CR_WCK_CONFIG_twck_dfi_stop_offset_MAX               (7) // 0x00000007
  #define MC0_CH0_CR_WCK_CONFIG_twck_dfi_stop_offset_DEF               (0x00000000)
  #define MC0_CH0_CR_WCK_CONFIG_twck_dfi_stop_offset_HSH               (0x4350E148)

  #define MC0_CH0_CR_WCK_CONFIG_LP5_WCK_MODE_OFF                       (43)
  #define MC0_CH0_CR_WCK_CONFIG_LP5_WCK_MODE_WID                       ( 2)
  #define MC0_CH0_CR_WCK_CONFIG_LP5_WCK_MODE_MSK                       (0x0000180000000000ULL)
  #define MC0_CH0_CR_WCK_CONFIG_LP5_WCK_MODE_MIN                       (0)
  #define MC0_CH0_CR_WCK_CONFIG_LP5_WCK_MODE_MAX                       (3) // 0x00000003
  #define MC0_CH0_CR_WCK_CONFIG_LP5_WCK_MODE_DEF                       (0x00000000)
  #define MC0_CH0_CR_WCK_CONFIG_LP5_WCK_MODE_HSH                       (0x4256E148)

  #define MC0_CH0_CR_WCK_CONFIG_WCK_FAST_MODE_OFF                      (45)
  #define MC0_CH0_CR_WCK_CONFIG_WCK_FAST_MODE_WID                      ( 1)
  #define MC0_CH0_CR_WCK_CONFIG_WCK_FAST_MODE_MSK                      (0x0000200000000000ULL)
  #define MC0_CH0_CR_WCK_CONFIG_WCK_FAST_MODE_MIN                      (0)
  #define MC0_CH0_CR_WCK_CONFIG_WCK_FAST_MODE_MAX                      (1) // 0x00000001
  #define MC0_CH0_CR_WCK_CONFIG_WCK_FAST_MODE_DEF                      (0x00000001)
  #define MC0_CH0_CR_WCK_CONFIG_WCK_FAST_MODE_HSH                      (0x415AE148)

#define MC0_CH0_CR_SC_WDBWM_UARCH_REG                                  (0x0000E158)
#define MC0_CH0_CR_SC_WDBWM_UARCH_DEF                                  (0x00000000)
#define MC0_CH0_CR_SC_WDBWM_UARCH_VOLATILE_BITFIELDS_MSK               (0x00000000)

  #define MC0_CH0_CR_SC_WDBWM_UARCH_En_Wr_in_WIM_during_TT_OFF         ( 0)
  #define MC0_CH0_CR_SC_WDBWM_UARCH_En_Wr_in_WIM_during_TT_WID         ( 1)
  #define MC0_CH0_CR_SC_WDBWM_UARCH_En_Wr_in_WIM_during_TT_MSK         (0x00000001)
  #define MC0_CH0_CR_SC_WDBWM_UARCH_En_Wr_in_WIM_during_TT_MIN         (0)
  #define MC0_CH0_CR_SC_WDBWM_UARCH_En_Wr_in_WIM_during_TT_MAX         (1) // 0x00000001
  #define MC0_CH0_CR_SC_WDBWM_UARCH_En_Wr_in_WIM_during_TT_DEF         (0x00000000)
  #define MC0_CH0_CR_SC_WDBWM_UARCH_En_Wr_in_WIM_during_TT_HSH         (0x0100E158)

#define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REG                             (0x0000E15C)
#define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_DEF                             (0x00000002)
#define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_VOLATILE_BITFIELDS_MSK          (0x00000000)

  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REFm_REF_wm_block_sel_OFF     ( 0)
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REFm_REF_wm_block_sel_WID     ( 1)
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REFm_REF_wm_block_sel_MSK     (0x00000001)
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REFm_REF_wm_block_sel_MIN     (0)
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REFm_REF_wm_block_sel_MAX     (1) // 0x00000001
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REFm_REF_wm_block_sel_DEF     (0x00000000)
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REFm_REF_wm_block_sel_HSH     (0x0100E15C)

  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REFab_before_REFm_on_IDLE_OFF ( 1)
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REFab_before_REFm_on_IDLE_WID ( 1)
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REFab_before_REFm_on_IDLE_MSK (0x00000002)
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REFab_before_REFm_on_IDLE_MIN (0)
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REFab_before_REFm_on_IDLE_MAX (1) // 0x00000001
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REFab_before_REFm_on_IDLE_DEF (0x00000001)
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REFab_before_REFm_on_IDLE_HSH (0x0102E15C)

  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_IGNORE_HIGH_WM_BLOCK_OFF      ( 2)
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_IGNORE_HIGH_WM_BLOCK_WID      ( 1)
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_IGNORE_HIGH_WM_BLOCK_MSK      (0x00000004)
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_IGNORE_HIGH_WM_BLOCK_MIN      (0)
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_IGNORE_HIGH_WM_BLOCK_MAX      (1) // 0x00000001
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_IGNORE_HIGH_WM_BLOCK_DEF      (0x00000000)
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_IGNORE_HIGH_WM_BLOCK_HSH      (0x0104E15C)

  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REFm_Disable_on_hot_OFF       ( 3)
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REFm_Disable_on_hot_WID       ( 1)
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REFm_Disable_on_hot_MSK       (0x00000008)
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REFm_Disable_on_hot_MIN       (0)
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REFm_Disable_on_hot_MAX       (1) // 0x00000001
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REFm_Disable_on_hot_DEF       (0x00000000)
  #define MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REFm_Disable_on_hot_HSH       (0x0106E15C)

#define MC0_CH0_CR_RH_TRR_CONTROL_REG                                  (0x0000E400)
#define MC0_CH0_CR_RH_TRR_CONTROL_DEF                                  (0x8C000000)
#define MC0_CH0_CR_RH_TRR_CONTROL_VOLATILE_BITFIELDS_MSK               (0x00000000)

  #define MC0_CH0_CR_RH_TRR_CONTROL_TRR_DRFM_Dimm_Enabled_OFF          ( 0)
  #define MC0_CH0_CR_RH_TRR_CONTROL_TRR_DRFM_Dimm_Enabled_WID          ( 2)
  #define MC0_CH0_CR_RH_TRR_CONTROL_TRR_DRFM_Dimm_Enabled_MSK          (0x00000003)
  #define MC0_CH0_CR_RH_TRR_CONTROL_TRR_DRFM_Dimm_Enabled_MIN          (0)
  #define MC0_CH0_CR_RH_TRR_CONTROL_TRR_DRFM_Dimm_Enabled_MAX          (3) // 0x00000003
  #define MC0_CH0_CR_RH_TRR_CONTROL_TRR_DRFM_Dimm_Enabled_DEF          (0x00000000)
  #define MC0_CH0_CR_RH_TRR_CONTROL_TRR_DRFM_Dimm_Enabled_HSH          (0x0200E400)

  #define MC0_CH0_CR_RH_TRR_CONTROL_DRFM_Enabled_OFF                   ( 2)
  #define MC0_CH0_CR_RH_TRR_CONTROL_DRFM_Enabled_WID                   ( 2)
  #define MC0_CH0_CR_RH_TRR_CONTROL_DRFM_Enabled_MSK                   (0x0000000C)
  #define MC0_CH0_CR_RH_TRR_CONTROL_DRFM_Enabled_MIN                   (0)
  #define MC0_CH0_CR_RH_TRR_CONTROL_DRFM_Enabled_MAX                   (3) // 0x00000003
  #define MC0_CH0_CR_RH_TRR_CONTROL_DRFM_Enabled_DEF                   (0x00000000)
  #define MC0_CH0_CR_RH_TRR_CONTROL_DRFM_Enabled_HSH                   (0x0204E400)

  #define MC0_CH0_CR_RH_TRR_CONTROL_LFSR_0_MASK_OFF                    ( 6)
  #define MC0_CH0_CR_RH_TRR_CONTROL_LFSR_0_MASK_WID                    ( 4)
  #define MC0_CH0_CR_RH_TRR_CONTROL_LFSR_0_MASK_MSK                    (0x000003C0)
  #define MC0_CH0_CR_RH_TRR_CONTROL_LFSR_0_MASK_MIN                    (0)
  #define MC0_CH0_CR_RH_TRR_CONTROL_LFSR_0_MASK_MAX                    (15) // 0x0000000F
  #define MC0_CH0_CR_RH_TRR_CONTROL_LFSR_0_MASK_DEF                    (0x00000000)
  #define MC0_CH0_CR_RH_TRR_CONTROL_LFSR_0_MASK_HSH                    (0x040CE400)

  #define MC0_CH0_CR_RH_TRR_CONTROL_LFSR_1_MASK_OFF                    (10)
  #define MC0_CH0_CR_RH_TRR_CONTROL_LFSR_1_MASK_WID                    ( 4)
  #define MC0_CH0_CR_RH_TRR_CONTROL_LFSR_1_MASK_MSK                    (0x00003C00)
  #define MC0_CH0_CR_RH_TRR_CONTROL_LFSR_1_MASK_MIN                    (0)
  #define MC0_CH0_CR_RH_TRR_CONTROL_LFSR_1_MASK_MAX                    (15) // 0x0000000F
  #define MC0_CH0_CR_RH_TRR_CONTROL_LFSR_1_MASK_DEF                    (0x00000000)
  #define MC0_CH0_CR_RH_TRR_CONTROL_LFSR_1_MASK_HSH                    (0x0414E400)

  #define MC0_CH0_CR_RH_TRR_CONTROL_MA1_Swizzling_OFF                  (14)
  #define MC0_CH0_CR_RH_TRR_CONTROL_MA1_Swizzling_WID                  ( 2)
  #define MC0_CH0_CR_RH_TRR_CONTROL_MA1_Swizzling_MSK                  (0x0000C000)
  #define MC0_CH0_CR_RH_TRR_CONTROL_MA1_Swizzling_MIN                  (0)
  #define MC0_CH0_CR_RH_TRR_CONTROL_MA1_Swizzling_MAX                  (3) // 0x00000003
  #define MC0_CH0_CR_RH_TRR_CONTROL_MA1_Swizzling_DEF                  (0x00000000)
  #define MC0_CH0_CR_RH_TRR_CONTROL_MA1_Swizzling_HSH                  (0x021CE400)

  #define MC0_CH0_CR_RH_TRR_CONTROL_MA2_Swizzling_OFF                  (16)
  #define MC0_CH0_CR_RH_TRR_CONTROL_MA2_Swizzling_WID                  ( 2)
  #define MC0_CH0_CR_RH_TRR_CONTROL_MA2_Swizzling_MSK                  (0x00030000)
  #define MC0_CH0_CR_RH_TRR_CONTROL_MA2_Swizzling_MIN                  (0)
  #define MC0_CH0_CR_RH_TRR_CONTROL_MA2_Swizzling_MAX                  (3) // 0x00000003
  #define MC0_CH0_CR_RH_TRR_CONTROL_MA2_Swizzling_DEF                  (0x00000000)
  #define MC0_CH0_CR_RH_TRR_CONTROL_MA2_Swizzling_HSH                  (0x0220E400)

  #define MC0_CH0_CR_RH_TRR_CONTROL_Micron_R_Swizzling_OFF             (18)
  #define MC0_CH0_CR_RH_TRR_CONTROL_Micron_R_Swizzling_WID             ( 2)
  #define MC0_CH0_CR_RH_TRR_CONTROL_Micron_R_Swizzling_MSK             (0x000C0000)
  #define MC0_CH0_CR_RH_TRR_CONTROL_Micron_R_Swizzling_MIN             (0)
  #define MC0_CH0_CR_RH_TRR_CONTROL_Micron_R_Swizzling_MAX             (3) // 0x00000003
  #define MC0_CH0_CR_RH_TRR_CONTROL_Micron_R_Swizzling_DEF             (0x00000000)
  #define MC0_CH0_CR_RH_TRR_CONTROL_Micron_R_Swizzling_HSH             (0x0224E400)

  #define MC0_CH0_CR_RH_TRR_CONTROL_Micron_F_Swizzling_OFF             (20)
  #define MC0_CH0_CR_RH_TRR_CONTROL_Micron_F_Swizzling_WID             ( 2)
  #define MC0_CH0_CR_RH_TRR_CONTROL_Micron_F_Swizzling_MSK             (0x00300000)
  #define MC0_CH0_CR_RH_TRR_CONTROL_Micron_F_Swizzling_MIN             (0)
  #define MC0_CH0_CR_RH_TRR_CONTROL_Micron_F_Swizzling_MAX             (3) // 0x00000003
  #define MC0_CH0_CR_RH_TRR_CONTROL_Micron_F_Swizzling_DEF             (0x00000000)
  #define MC0_CH0_CR_RH_TRR_CONTROL_Micron_F_Swizzling_HSH             (0x0228E400)

  #define MC0_CH0_CR_RH_TRR_CONTROL_BRC_OFF                            (24)
  #define MC0_CH0_CR_RH_TRR_CONTROL_BRC_WID                            ( 2)
  #define MC0_CH0_CR_RH_TRR_CONTROL_BRC_MSK                            (0x03000000)
  #define MC0_CH0_CR_RH_TRR_CONTROL_BRC_MIN                            (0)
  #define MC0_CH0_CR_RH_TRR_CONTROL_BRC_MAX                            (3) // 0x00000003
  #define MC0_CH0_CR_RH_TRR_CONTROL_BRC_DEF                            (0x00000000)
  #define MC0_CH0_CR_RH_TRR_CONTROL_BRC_HSH                            (0x0230E400)

  #define MC0_CH0_CR_RH_TRR_CONTROL_Blast_Radius_LFSR_MASK_OFF         (26)
  #define MC0_CH0_CR_RH_TRR_CONTROL_Blast_Radius_LFSR_MASK_WID         ( 3)
  #define MC0_CH0_CR_RH_TRR_CONTROL_Blast_Radius_LFSR_MASK_MSK         (0x1C000000)
  #define MC0_CH0_CR_RH_TRR_CONTROL_Blast_Radius_LFSR_MASK_MIN         (0)
  #define MC0_CH0_CR_RH_TRR_CONTROL_Blast_Radius_LFSR_MASK_MAX         (7) // 0x00000007
  #define MC0_CH0_CR_RH_TRR_CONTROL_Blast_Radius_LFSR_MASK_DEF         (0x00000003)
  #define MC0_CH0_CR_RH_TRR_CONTROL_Blast_Radius_LFSR_MASK_HSH         (0x0334E400)

  #define MC0_CH0_CR_RH_TRR_CONTROL_DDR5_DRFM_LFSR_MASK_OFF            (29)
  #define MC0_CH0_CR_RH_TRR_CONTROL_DDR5_DRFM_LFSR_MASK_WID            ( 3)
  #define MC0_CH0_CR_RH_TRR_CONTROL_DDR5_DRFM_LFSR_MASK_MSK            (0xE0000000)
  #define MC0_CH0_CR_RH_TRR_CONTROL_DDR5_DRFM_LFSR_MASK_MIN            (0)
  #define MC0_CH0_CR_RH_TRR_CONTROL_DDR5_DRFM_LFSR_MASK_MAX            (7) // 0x00000007
  #define MC0_CH0_CR_RH_TRR_CONTROL_DDR5_DRFM_LFSR_MASK_DEF            (0x00000004)
  #define MC0_CH0_CR_RH_TRR_CONTROL_DDR5_DRFM_LFSR_MASK_HSH            (0x033AE400)

#define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_REG                       (0x0000E404)
#define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_DEF                       (0x0000000F)
#define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_VOLATILE_BITFIELDS_MSK    (0x00000000)

  #define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_Refresh_Rank_Mask_OFF   ( 0)
  #define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_Refresh_Rank_Mask_WID   ( 4)
  #define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_Refresh_Rank_Mask_MSK   (0x0000000F)
  #define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_Refresh_Rank_Mask_MIN   (0)
  #define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_Refresh_Rank_Mask_MAX   (15) // 0x0000000F
  #define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_Refresh_Rank_Mask_DEF   (0x0000000F)
  #define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_Refresh_Rank_Mask_HSH   (0x0400E404)

  #define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_RefZQ_En_Start_Test_Sync_OFF ( 8)
  #define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_RefZQ_En_Start_Test_Sync_WID ( 1)
  #define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_RefZQ_En_Start_Test_Sync_MSK (0x00000100)
  #define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_RefZQ_En_Start_Test_Sync_MIN (0)
  #define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_RefZQ_En_Start_Test_Sync_MAX (1) // 0x00000001
  #define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_RefZQ_En_Start_Test_Sync_DEF (0x00000000)
  #define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_RefZQ_En_Start_Test_Sync_HSH (0x0110E404)

  #define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_Panic_Refresh_Only_OFF  (31)
  #define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_Panic_Refresh_Only_WID  ( 1)
  #define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_Panic_Refresh_Only_MSK  (0x80000000)
  #define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_Panic_Refresh_Only_MIN  (0)
  #define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_Panic_Refresh_Only_MAX  (1) // 0x00000001
  #define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_Panic_Refresh_Only_DEF  (0x00000000)
  #define MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_Panic_Refresh_Only_HSH  (0x013EE404)

#define MC0_CH0_CR_REUT_CH_MISC_ZQ_CTRL_REG                            (0x0000E408)
#define MC0_CH0_CR_REUT_CH_MISC_ZQ_CTRL_DEF                            (0x00000000)
#define MC0_CH0_CR_REUT_CH_MISC_ZQ_CTRL_VOLATILE_BITFIELDS_MSK         (0x00000000)

  #define MC0_CH0_CR_REUT_CH_MISC_ZQ_CTRL_ZQ_Rank_Mask_OFF             ( 0)
  #define MC0_CH0_CR_REUT_CH_MISC_ZQ_CTRL_ZQ_Rank_Mask_WID             ( 4)
  #define MC0_CH0_CR_REUT_CH_MISC_ZQ_CTRL_ZQ_Rank_Mask_MSK             (0x0000000F)
  #define MC0_CH0_CR_REUT_CH_MISC_ZQ_CTRL_ZQ_Rank_Mask_MIN             (0)
  #define MC0_CH0_CR_REUT_CH_MISC_ZQ_CTRL_ZQ_Rank_Mask_MAX             (15) // 0x0000000F
  #define MC0_CH0_CR_REUT_CH_MISC_ZQ_CTRL_ZQ_Rank_Mask_DEF             (0x00000000)
  #define MC0_CH0_CR_REUT_CH_MISC_ZQ_CTRL_ZQ_Rank_Mask_HSH             (0x0400E408)

  #define MC0_CH0_CR_REUT_CH_MISC_ZQ_CTRL_Always_Do_ZQ_OFF             (31)
  #define MC0_CH0_CR_REUT_CH_MISC_ZQ_CTRL_Always_Do_ZQ_WID             ( 1)
  #define MC0_CH0_CR_REUT_CH_MISC_ZQ_CTRL_Always_Do_ZQ_MSK             (0x80000000)
  #define MC0_CH0_CR_REUT_CH_MISC_ZQ_CTRL_Always_Do_ZQ_MIN             (0)
  #define MC0_CH0_CR_REUT_CH_MISC_ZQ_CTRL_Always_Do_ZQ_MAX             (1) // 0x00000001
  #define MC0_CH0_CR_REUT_CH_MISC_ZQ_CTRL_Always_Do_ZQ_DEF             (0x00000000)
  #define MC0_CH0_CR_REUT_CH_MISC_ZQ_CTRL_Always_Do_ZQ_HSH             (0x013EE408)

#define MC0_CH0_CR_TC_REFM_REG                                         (0x0000E40C)
#define MC0_CH0_CR_TC_REFM_DEF                                         (0x0001E03C)
#define MC0_CH0_CR_TC_REFM_VOLATILE_BITFIELDS_MSK                      (0x00000000)

  #define MC0_CH0_CR_TC_REFM_tRFM_OFF                                  ( 0)
  #define MC0_CH0_CR_TC_REFM_tRFM_WID                                  (11)
  #define MC0_CH0_CR_TC_REFM_tRFM_MSK                                  (0x000007FF)
  #define MC0_CH0_CR_TC_REFM_tRFM_MIN                                  (0)
  #define MC0_CH0_CR_TC_REFM_tRFM_MAX                                  (1023) // 0x000003FF
  #define MC0_CH0_CR_TC_REFM_tRFM_DEF                                  (0x0000003C)
  #define MC0_CH0_CR_TC_REFM_tRFM_HSH                                  (0x0B00E40C)

  #define MC0_CH0_CR_TC_REFM_tDRFM_OFF                                 (11)
  #define MC0_CH0_CR_TC_REFM_tDRFM_WID                                 (13)
  #define MC0_CH0_CR_TC_REFM_tDRFM_MSK                                 (0x00FFF800)
  #define MC0_CH0_CR_TC_REFM_tDRFM_MIN                                 (0)
  #define MC0_CH0_CR_TC_REFM_tDRFM_MAX                                 (2) // 0x00000002
  #define MC0_CH0_CR_TC_REFM_tDRFM_DEF                                 (0x0000003C)
  #define MC0_CH0_CR_TC_REFM_tDRFM_HSH                                 (0x0D16E40C)

#define MC0_CH0_CR_RH_TRR_CONTROL_UARCH_REG                            (0x0000E410)
#define MC0_CH0_CR_RH_TRR_CONTROL_UARCH_DEF                            (0x00000024)
#define MC0_CH0_CR_RH_TRR_CONTROL_UARCH_VOLATILE_BITFIELDS_MSK         (0x00000000)

  #define MC0_CH0_CR_RH_TRR_CONTROL_UARCH_BlockState_Delay_OFF         ( 0)
  #define MC0_CH0_CR_RH_TRR_CONTROL_UARCH_BlockState_Delay_WID         ( 4)
  #define MC0_CH0_CR_RH_TRR_CONTROL_UARCH_BlockState_Delay_MSK         (0x0000000F)
  #define MC0_CH0_CR_RH_TRR_CONTROL_UARCH_BlockState_Delay_MIN         (0)
  #define MC0_CH0_CR_RH_TRR_CONTROL_UARCH_BlockState_Delay_MAX         (15) // 0x0000000F
  #define MC0_CH0_CR_RH_TRR_CONTROL_UARCH_BlockState_Delay_DEF         (0x00000004)
  #define MC0_CH0_CR_RH_TRR_CONTROL_UARCH_BlockState_Delay_HSH         (0x0400E410)

  #define MC0_CH0_CR_RH_TRR_CONTROL_UARCH_Final_FSM_Delay_OFF          ( 4)
  #define MC0_CH0_CR_RH_TRR_CONTROL_UARCH_Final_FSM_Delay_WID          ( 3)
  #define MC0_CH0_CR_RH_TRR_CONTROL_UARCH_Final_FSM_Delay_MSK          (0x00000070)
  #define MC0_CH0_CR_RH_TRR_CONTROL_UARCH_Final_FSM_Delay_MIN          (0)
  #define MC0_CH0_CR_RH_TRR_CONTROL_UARCH_Final_FSM_Delay_MAX          (7) // 0x00000007
  #define MC0_CH0_CR_RH_TRR_CONTROL_UARCH_Final_FSM_Delay_DEF          (0x00000002)
  #define MC0_CH0_CR_RH_TRR_CONTROL_UARCH_Final_FSM_Delay_HSH          (0x0308E410)

#define MC0_CH0_CR_DDR_MR_COMMAND_REG                                  (0x0000E414)
#define MC0_CH0_CR_DDR_MR_COMMAND_DEF                                  (0x00000000)
#define MC0_CH0_CR_DDR_MR_COMMAND_VOLATILE_BITFIELDS_MSK               (0x80000000)

  #define MC0_CH0_CR_DDR_MR_COMMAND_Address_OFF                        ( 0)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Address_WID                        ( 8)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Address_MSK                        (0x000000FF)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Address_MIN                        (0)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Address_MAX                        (255) // 0x000000FF
  #define MC0_CH0_CR_DDR_MR_COMMAND_Address_DEF                        (0x00000000)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Address_HSH                        (0x0800E414)

  #define MC0_CH0_CR_DDR_MR_COMMAND_Data_OFF                           ( 8)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Data_WID                           ( 8)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Data_MSK                           (0x0000FF00)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Data_MIN                           (0)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Data_MAX                           (255) // 0x000000FF
  #define MC0_CH0_CR_DDR_MR_COMMAND_Data_DEF                           (0x00000000)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Data_HSH                           (0x0810E414)

  #define MC0_CH0_CR_DDR_MR_COMMAND_Rank_OFF                           (16)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Rank_WID                           ( 4)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Rank_MSK                           (0x000F0000)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Rank_MIN                           (0)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Rank_MAX                           (15) // 0x0000000F
  #define MC0_CH0_CR_DDR_MR_COMMAND_Rank_DEF                           (0x00000000)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Rank_HSH                           (0x0420E414)

  #define MC0_CH0_CR_DDR_MR_COMMAND_Command_OFF                        (20)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Command_WID                        ( 3)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Command_MSK                        (0x00700000)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Command_MIN                        (0)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Command_MAX                        (7) // 0x00000007
  #define MC0_CH0_CR_DDR_MR_COMMAND_Command_DEF                        (0x00000000)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Command_HSH                        (0x0328E414)

  #define MC0_CH0_CR_DDR_MR_COMMAND_SPID_cmd_type_OFF                  (23)
  #define MC0_CH0_CR_DDR_MR_COMMAND_SPID_cmd_type_WID                  ( 3)
  #define MC0_CH0_CR_DDR_MR_COMMAND_SPID_cmd_type_MSK                  (0x03800000)
  #define MC0_CH0_CR_DDR_MR_COMMAND_SPID_cmd_type_MIN                  (0)
  #define MC0_CH0_CR_DDR_MR_COMMAND_SPID_cmd_type_MAX                  (7) // 0x00000007
  #define MC0_CH0_CR_DDR_MR_COMMAND_SPID_cmd_type_DEF                  (0x00000000)
  #define MC0_CH0_CR_DDR_MR_COMMAND_SPID_cmd_type_HSH                  (0x032EE414)

  #define MC0_CH0_CR_DDR_MR_COMMAND_Assume_idle_OFF                    (30)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Assume_idle_WID                    ( 1)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Assume_idle_MSK                    (0x40000000)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Assume_idle_MIN                    (0)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Assume_idle_MAX                    (1) // 0x00000001
  #define MC0_CH0_CR_DDR_MR_COMMAND_Assume_idle_DEF                    (0x00000000)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Assume_idle_HSH                    (0x013CE414)

  #define MC0_CH0_CR_DDR_MR_COMMAND_Busy_OFF                           (31)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Busy_WID                           ( 1)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Busy_MSK                           (0x80000000)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Busy_MIN                           (0)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Busy_MAX                           (1) // 0x00000001
  #define MC0_CH0_CR_DDR_MR_COMMAND_Busy_DEF                           (0x00000000)
  #define MC0_CH0_CR_DDR_MR_COMMAND_Busy_HSH                           (0x01BEE414)

#define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_REG                          (0x0000E418)
#define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_DEF                          (0x00000000)
#define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_VOLATILE_BITFIELDS_MSK       (0xFFFFFFFF)

  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R0_D0_OFF                  ( 0)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R0_D0_WID                  ( 8)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R0_D0_MSK                  (0x000000FF)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R0_D0_MIN                  (0)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R0_D0_MAX                  (255) // 0x000000FF
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R0_D0_DEF                  (0x00000000)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R0_D0_HSH                  (0x0880E418)

  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R0_D1_OFF                  ( 8)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R0_D1_WID                  ( 8)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R0_D1_MSK                  (0x0000FF00)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R0_D1_MIN                  (0)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R0_D1_MAX                  (255) // 0x000000FF
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R0_D1_DEF                  (0x00000000)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R0_D1_HSH                  (0x0890E418)

  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R1_D0_OFF                  (16)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R1_D0_WID                  ( 8)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R1_D0_MSK                  (0x00FF0000)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R1_D0_MIN                  (0)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R1_D0_MAX                  (255) // 0x000000FF
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R1_D0_DEF                  (0x00000000)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R1_D0_HSH                  (0x08A0E418)

  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R1_D1_OFF                  (24)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R1_D1_WID                  ( 8)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R1_D1_MSK                  (0xFF000000)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R1_D1_MIN                  (0)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R1_D1_MAX                  (255) // 0x000000FF
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R1_D1_DEF                  (0x00000000)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_R1_D1_HSH                  (0x08B0E418)

#define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_REG                         (0x0000E41C)
#define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_DEF                         (0x00000000)
#define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_VOLATILE_BITFIELDS_MSK      (0xFFFFFFFF)

  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R0_D2_OFF                 ( 0)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R0_D2_WID                 ( 8)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R0_D2_MSK                 (0x000000FF)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R0_D2_MIN                 (0)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R0_D2_MAX                 (255) // 0x000000FF
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R0_D2_DEF                 (0x00000000)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R0_D2_HSH                 (0x0880E41C)

  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R0_D3_OFF                 ( 8)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R0_D3_WID                 ( 8)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R0_D3_MSK                 (0x0000FF00)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R0_D3_MIN                 (0)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R0_D3_MAX                 (255) // 0x000000FF
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R0_D3_DEF                 (0x00000000)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R0_D3_HSH                 (0x0890E41C)

  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R1_D2_OFF                 (16)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R1_D2_WID                 ( 8)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R1_D2_MSK                 (0x00FF0000)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R1_D2_MIN                 (0)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R1_D2_MAX                 (255) // 0x000000FF
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R1_D2_DEF                 (0x00000000)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R1_D2_HSH                 (0x08A0E41C)

  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R1_D3_OFF                 (24)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R1_D3_WID                 ( 8)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R1_D3_MSK                 (0xFF000000)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R1_D3_MIN                 (0)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R1_D3_MAX                 (255) // 0x000000FF
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R1_D3_DEF                 (0x00000000)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_R1_D3_HSH                 (0x08B0E41C)

#define MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_REG                          (0x0000E420)
#define MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_DEF                          (0x00000000)
#define MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_VOLATILE_BITFIELDS_MSK       (0x0000FFFF)

  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_R0_ECC_OFF                 ( 0)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_R0_ECC_WID                 ( 8)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_R0_ECC_MSK                 (0x000000FF)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_R0_ECC_MIN                 (0)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_R0_ECC_MAX                 (255) // 0x000000FF
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_R0_ECC_DEF                 (0x00000000)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_R0_ECC_HSH                 (0x0880E420)

  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_R1_ECC_OFF                 ( 8)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_R1_ECC_WID                 ( 8)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_R1_ECC_MSK                 (0x0000FF00)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_R1_ECC_MIN                 (0)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_R1_ECC_MAX                 (255) // 0x000000FF
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_R1_ECC_DEF                 (0x00000000)
  #define MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_R1_ECC_HSH                 (0x0890E420)

#define MC0_CH0_CR_RANK_TEMPERATURE_REG                                (0x0000E424)
#define MC0_CH0_CR_RANK_TEMPERATURE_DEF                                (0x09090909)
#define MC0_CH0_CR_RANK_TEMPERATURE_VOLATILE_BITFIELDS_MSK             (0xFFFFFFFF)

  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH0_RANK0_TEMP_OFF              ( 0)
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH0_RANK0_TEMP_WID              ( 8)
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH0_RANK0_TEMP_MSK              (0x000000FF)
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH0_RANK0_TEMP_MIN              (0)
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH0_RANK0_TEMP_MAX              (255) // 0x000000FF
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH0_RANK0_TEMP_DEF              (0x00000009)
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH0_RANK0_TEMP_HSH              (0x0880E424)

  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH0_RANK1_TEMP_OFF              ( 8)
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH0_RANK1_TEMP_WID              ( 8)
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH0_RANK1_TEMP_MSK              (0x0000FF00)
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH0_RANK1_TEMP_MIN              (0)
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH0_RANK1_TEMP_MAX              (255) // 0x000000FF
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH0_RANK1_TEMP_DEF              (0x00000009)
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH0_RANK1_TEMP_HSH              (0x0890E424)

  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH1_RANK0_TEMP_OFF              (16)
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH1_RANK0_TEMP_WID              ( 8)
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH1_RANK0_TEMP_MSK              (0x00FF0000)
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH1_RANK0_TEMP_MIN              (0)
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH1_RANK0_TEMP_MAX              (255) // 0x000000FF
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH1_RANK0_TEMP_DEF              (0x00000009)
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH1_RANK0_TEMP_HSH              (0x08A0E424)

  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH1_RANK1_TEMP_OFF              (24)
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH1_RANK1_TEMP_WID              ( 8)
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH1_RANK1_TEMP_MSK              (0xFF000000)
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH1_RANK1_TEMP_MIN              (0)
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH1_RANK1_TEMP_MAX              (255) // 0x000000FF
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH1_RANK1_TEMP_DEF              (0x00000009)
  #define MC0_CH0_CR_RANK_TEMPERATURE_SCH1_RANK1_TEMP_HSH              (0x08B0E424)

#define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_REG                          (0x0000E428)
#define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_DEF                          (0x00003800)
#define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_VOLATILE_BITFIELDS_MSK       (0x00000000)

  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_DISABLE_DRAM_TS_OFF        ( 7)
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_DISABLE_DRAM_TS_WID        ( 1)
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_DISABLE_DRAM_TS_MSK        (0x00000080)
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_DISABLE_DRAM_TS_MIN        (0)
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_DISABLE_DRAM_TS_MAX        (1) // 0x00000001
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_DISABLE_DRAM_TS_DEF        (0x00000000)
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_DISABLE_DRAM_TS_HSH        (0x010EE428)

  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MIN_REF_RATE_OFF           ( 8)
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MIN_REF_RATE_WID           ( 3)
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MIN_REF_RATE_MSK           (0x00000700)
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MIN_REF_RATE_MIN           (0)
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MIN_REF_RATE_MAX           (7) // 0x00000007
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MIN_REF_RATE_DEF           (0x00000000)
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MIN_REF_RATE_HSH           (0x0310E428)

  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MAX_REF_RATE_OFF           (11)
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MAX_REF_RATE_WID           ( 3)
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MAX_REF_RATE_MSK           (0x00003800)
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MAX_REF_RATE_MIN           (0)
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MAX_REF_RATE_MAX           (7) // 0x00000007
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MAX_REF_RATE_DEF           (0x00000007)
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MAX_REF_RATE_HSH           (0x0316E428)

  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MR4_PERIOD_OFF             (14)
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MR4_PERIOD_WID             (16)
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MR4_PERIOD_MSK             (0x3FFFC000)
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MR4_PERIOD_MIN             (0)
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MR4_PERIOD_MAX             (65535) // 0x0000FFFF
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MR4_PERIOD_DEF             (0x00000000)
  #define MC0_CH0_CR_PM_CONFIG_THERM_STATUS_MR4_PERIOD_HSH             (0x101CE428)

#define MC0_CH0_CR_MC_RDB_CREDITS_REG                                  (0x0000E42C)
#define MC0_CH0_CR_MC_RDB_CREDITS_DEF                                  (0x00181830)
#define MC0_CH0_CR_MC_RDB_CREDITS_VOLATILE_BITFIELDS_MSK               (0x00000000)

  #define MC0_CH0_CR_MC_RDB_CREDITS_Total_reads_entries_OFF            ( 0)
  #define MC0_CH0_CR_MC_RDB_CREDITS_Total_reads_entries_WID            ( 6)
  #define MC0_CH0_CR_MC_RDB_CREDITS_Total_reads_entries_MSK            (0x0000003F)
  #define MC0_CH0_CR_MC_RDB_CREDITS_Total_reads_entries_MIN            (2)
  #define MC0_CH0_CR_MC_RDB_CREDITS_Total_reads_entries_MAX            (48) // 0x00000030
  #define MC0_CH0_CR_MC_RDB_CREDITS_Total_reads_entries_DEF            (0x00000030)
  #define MC0_CH0_CR_MC_RDB_CREDITS_Total_reads_entries_HSH            (0x0600E42C)

  #define MC0_CH0_CR_MC_RDB_CREDITS_VC0_reads_entries_OFF              ( 8)
  #define MC0_CH0_CR_MC_RDB_CREDITS_VC0_reads_entries_WID              ( 6)
  #define MC0_CH0_CR_MC_RDB_CREDITS_VC0_reads_entries_MSK              (0x00003F00)
  #define MC0_CH0_CR_MC_RDB_CREDITS_VC0_reads_entries_MIN              (1)
  #define MC0_CH0_CR_MC_RDB_CREDITS_VC0_reads_entries_MAX              (48) // 0x00000030
  #define MC0_CH0_CR_MC_RDB_CREDITS_VC0_reads_entries_DEF              (0x00000018)
  #define MC0_CH0_CR_MC_RDB_CREDITS_VC0_reads_entries_HSH              (0x0610E42C)

  #define MC0_CH0_CR_MC_RDB_CREDITS_VC1_reads_entries_OFF              (16)
  #define MC0_CH0_CR_MC_RDB_CREDITS_VC1_reads_entries_WID              ( 6)
  #define MC0_CH0_CR_MC_RDB_CREDITS_VC1_reads_entries_MSK              (0x003F0000)
  #define MC0_CH0_CR_MC_RDB_CREDITS_VC1_reads_entries_MIN              (1)
  #define MC0_CH0_CR_MC_RDB_CREDITS_VC1_reads_entries_MAX              (48) // 0x00000030
  #define MC0_CH0_CR_MC_RDB_CREDITS_VC1_reads_entries_DEF              (0x00000018)
  #define MC0_CH0_CR_MC_RDB_CREDITS_VC1_reads_entries_HSH              (0x0620E42C)

#define MC0_CH0_CR_RH_TRR_ADDRESS_REG                                  (0x0000E430)
#define MC0_CH0_CR_RH_TRR_ADDRESS_DEF                                  (0x00000000)
#define MC0_CH0_CR_RH_TRR_ADDRESS_VOLATILE_BITFIELDS_MSK               (0x0FFFFFFF)

  #define MC0_CH0_CR_RH_TRR_ADDRESS_Valid_OFF                          ( 0)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Valid_WID                          ( 1)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Valid_MSK                          (0x00000001)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Valid_MIN                          (0)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Valid_MAX                          (1) // 0x00000001
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Valid_DEF                          (0x00000000)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Valid_HSH                          (0x0180E430)

  #define MC0_CH0_CR_RH_TRR_ADDRESS_Rank_OFF                           ( 1)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Rank_WID                           ( 2)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Rank_MSK                           (0x00000006)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Rank_MIN                           (0)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Rank_MAX                           (3) // 0x00000003
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Rank_DEF                           (0x00000000)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Rank_HSH                           (0x0282E430)

  #define MC0_CH0_CR_RH_TRR_ADDRESS_Group_OFF                          ( 3)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Group_WID                          ( 2)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Group_MSK                          (0x00000018)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Group_MIN                          (0)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Group_MAX                          (3) // 0x00000003
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Group_DEF                          (0x00000000)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Group_HSH                          (0x0286E430)

  #define MC0_CH0_CR_RH_TRR_ADDRESS_Bank_OFF                           ( 5)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Bank_WID                           ( 3)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Bank_MSK                           (0x000000E0)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Bank_MIN                           (0)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Bank_MAX                           (7) // 0x00000007
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Bank_DEF                           (0x00000000)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Bank_HSH                           (0x038AE430)

  #define MC0_CH0_CR_RH_TRR_ADDRESS_Row_OFF                            ( 8)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Row_WID                            (18)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Row_MSK                            (0x03FFFF00)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Row_MIN                            (0)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Row_MAX                            (262143) // 0x0003FFFF
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Row_DEF                            (0x00000000)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Row_HSH                            (0x1290E430)

  #define MC0_CH0_CR_RH_TRR_ADDRESS_Do_Blast_Radius_OFF                (26)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Do_Blast_Radius_WID                ( 1)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Do_Blast_Radius_MSK                (0x04000000)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Do_Blast_Radius_MIN                (0)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Do_Blast_Radius_MAX                (1) // 0x00000001
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Do_Blast_Radius_DEF                (0x00000000)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Do_Blast_Radius_HSH                (0x01B4E430)

  #define MC0_CH0_CR_RH_TRR_ADDRESS_Do_DDR5_DRFM_OFF                   (27)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Do_DDR5_DRFM_WID                   ( 1)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Do_DDR5_DRFM_MSK                   (0x08000000)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Do_DDR5_DRFM_MIN                   (0)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Do_DDR5_DRFM_MAX                   (1) // 0x00000001
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Do_DDR5_DRFM_DEF                   (0x00000000)
  #define MC0_CH0_CR_RH_TRR_ADDRESS_Do_DDR5_DRFM_HSH                   (0x01B6E430)

#define MC0_CH0_CR_MC_INIT_STATE_UARCH_REG                             (0x0000E434)
#define MC0_CH0_CR_MC_INIT_STATE_UARCH_DEF                             (0x00000000)
#define MC0_CH0_CR_MC_INIT_STATE_UARCH_VOLATILE_BITFIELDS_MSK          (0x00002800)

  #define MC0_CH0_CR_MC_INIT_STATE_UARCH_Reset_refresh_debt_OFF        (11)
  #define MC0_CH0_CR_MC_INIT_STATE_UARCH_Reset_refresh_debt_WID        ( 1)
  #define MC0_CH0_CR_MC_INIT_STATE_UARCH_Reset_refresh_debt_MSK        (0x00000800)
  #define MC0_CH0_CR_MC_INIT_STATE_UARCH_Reset_refresh_debt_MIN        (0)
  #define MC0_CH0_CR_MC_INIT_STATE_UARCH_Reset_refresh_debt_MAX        (1) // 0x00000001
  #define MC0_CH0_CR_MC_INIT_STATE_UARCH_Reset_refresh_debt_DEF        (0x00000000)
  #define MC0_CH0_CR_MC_INIT_STATE_UARCH_Reset_refresh_debt_HSH        (0x0196E434)

  #define MC0_CH0_CR_MC_INIT_STATE_UARCH_VRCG_DVFSQ_enabled_OFF        (13)
  #define MC0_CH0_CR_MC_INIT_STATE_UARCH_VRCG_DVFSQ_enabled_WID        ( 1)
  #define MC0_CH0_CR_MC_INIT_STATE_UARCH_VRCG_DVFSQ_enabled_MSK        (0x00002000)
  #define MC0_CH0_CR_MC_INIT_STATE_UARCH_VRCG_DVFSQ_enabled_MIN        (0)
  #define MC0_CH0_CR_MC_INIT_STATE_UARCH_VRCG_DVFSQ_enabled_MAX        (1) // 0x00000001
  #define MC0_CH0_CR_MC_INIT_STATE_UARCH_VRCG_DVFSQ_enabled_DEF        (0x00000000)
  #define MC0_CH0_CR_MC_INIT_STATE_UARCH_VRCG_DVFSQ_enabled_HSH        (0x019AE434)

#define MC0_CH0_CR_RFP_REG                                             (0x0000E438)
#define MC0_CH0_CR_RFP_DEF                                             (0x00050976)
#define MC0_CH0_CR_RFP_VOLATILE_BITFIELDS_MSK                          (0x00000000)

  #define MC0_CH0_CR_RFP_Refresh_HP_WM_OFF                             ( 0)
  #define MC0_CH0_CR_RFP_Refresh_HP_WM_WID                             ( 4)
  #define MC0_CH0_CR_RFP_Refresh_HP_WM_MSK                             (0x0000000F)
  #define MC0_CH0_CR_RFP_Refresh_HP_WM_MIN                             (0)
  #define MC0_CH0_CR_RFP_Refresh_HP_WM_MAX                             (15) // 0x0000000F
  #define MC0_CH0_CR_RFP_Refresh_HP_WM_DEF                             (0x00000006)
  #define MC0_CH0_CR_RFP_Refresh_HP_WM_HSH                             (0x0400E438)

  #define MC0_CH0_CR_RFP_Refresh_panic_wm_OFF                          ( 4)
  #define MC0_CH0_CR_RFP_Refresh_panic_wm_WID                          ( 4)
  #define MC0_CH0_CR_RFP_Refresh_panic_wm_MSK                          (0x000000F0)
  #define MC0_CH0_CR_RFP_Refresh_panic_wm_MIN                          (0)
  #define MC0_CH0_CR_RFP_Refresh_panic_wm_MAX                          (15) // 0x0000000F
  #define MC0_CH0_CR_RFP_Refresh_panic_wm_DEF                          (0x00000007)
  #define MC0_CH0_CR_RFP_Refresh_panic_wm_HSH                          (0x0408E438)

  #define MC0_CH0_CR_RFP_Wakeup_on_HP_WM_OFF                           ( 8)
  #define MC0_CH0_CR_RFP_Wakeup_on_HP_WM_WID                           ( 1)
  #define MC0_CH0_CR_RFP_Wakeup_on_HP_WM_MSK                           (0x00000100)
  #define MC0_CH0_CR_RFP_Wakeup_on_HP_WM_MIN                           (0)
  #define MC0_CH0_CR_RFP_Wakeup_on_HP_WM_MAX                           (1) // 0x00000001
  #define MC0_CH0_CR_RFP_Wakeup_on_HP_WM_DEF                           (0x00000001)
  #define MC0_CH0_CR_RFP_Wakeup_on_HP_WM_HSH                           (0x0110E438)

  #define MC0_CH0_CR_RFP_PBR_Disable_OFF                               ( 9)
  #define MC0_CH0_CR_RFP_PBR_Disable_WID                               ( 1)
  #define MC0_CH0_CR_RFP_PBR_Disable_MSK                               (0x00000200)
  #define MC0_CH0_CR_RFP_PBR_Disable_MIN                               (0)
  #define MC0_CH0_CR_RFP_PBR_Disable_MAX                               (1) // 0x00000001
  #define MC0_CH0_CR_RFP_PBR_Disable_DEF                               (0x00000000)
  #define MC0_CH0_CR_RFP_PBR_Disable_HSH                               (0x0112E438)

  #define MC0_CH0_CR_RFP_PBR_Exit_on_Idle_Cnt_OFF                      (10)
  #define MC0_CH0_CR_RFP_PBR_Exit_on_Idle_Cnt_WID                      ( 6)
  #define MC0_CH0_CR_RFP_PBR_Exit_on_Idle_Cnt_MSK                      (0x0000FC00)
  #define MC0_CH0_CR_RFP_PBR_Exit_on_Idle_Cnt_MIN                      (0)
  #define MC0_CH0_CR_RFP_PBR_Exit_on_Idle_Cnt_MAX                      (63) // 0x0000003F
  #define MC0_CH0_CR_RFP_PBR_Exit_on_Idle_Cnt_DEF                      (0x00000002)
  #define MC0_CH0_CR_RFP_PBR_Exit_on_Idle_Cnt_HSH                      (0x0614E438)

  #define MC0_CH0_CR_RFP_Refresh_ABR_release_OFF                       (16)
  #define MC0_CH0_CR_RFP_Refresh_ABR_release_WID                       ( 4)
  #define MC0_CH0_CR_RFP_Refresh_ABR_release_MSK                       (0x000F0000)
  #define MC0_CH0_CR_RFP_Refresh_ABR_release_MIN                       (0)
  #define MC0_CH0_CR_RFP_Refresh_ABR_release_MAX                       (15) // 0x0000000F
  #define MC0_CH0_CR_RFP_Refresh_ABR_release_DEF                       (0x00000005)
  #define MC0_CH0_CR_RFP_Refresh_ABR_release_HSH                       (0x0420E438)

  #define MC0_CH0_CR_RFP_DDR5_RIR_OFF                                  (20)
  #define MC0_CH0_CR_RFP_DDR5_RIR_WID                                  ( 1)
  #define MC0_CH0_CR_RFP_DDR5_RIR_MSK                                  (0x00100000)
  #define MC0_CH0_CR_RFP_DDR5_RIR_MIN                                  (0)
  #define MC0_CH0_CR_RFP_DDR5_RIR_MAX                                  (1) // 0x00000001
  #define MC0_CH0_CR_RFP_DDR5_RIR_DEF                                  (0x00000000)
  #define MC0_CH0_CR_RFP_DDR5_RIR_HSH                                  (0x0128E438)

#define MC0_CH0_CR_RFP_UARCH_REG                                       (0x0000E440)
#define MC0_CH0_CR_RFP_UARCH_DEF                                       (0x00002C0F)
#define MC0_CH0_CR_RFP_UARCH_VOLATILE_BITFIELDS_MSK                    (0x00000000)

  #define MC0_CH0_CR_RFP_UARCH_OREF_RI_OFF                             ( 0)
  #define MC0_CH0_CR_RFP_UARCH_OREF_RI_WID                             ( 8)
  #define MC0_CH0_CR_RFP_UARCH_OREF_RI_MSK                             (0x000000FF)
  #define MC0_CH0_CR_RFP_UARCH_OREF_RI_MIN                             (0)
  #define MC0_CH0_CR_RFP_UARCH_OREF_RI_MAX                             (255) // 0x000000FF
  #define MC0_CH0_CR_RFP_UARCH_OREF_RI_DEF                             (0x0000000F)
  #define MC0_CH0_CR_RFP_UARCH_OREF_RI_HSH                             (0x0800E440)

  #define MC0_CH0_CR_RFP_UARCH_CounttREFIWhileRefEnOff_OFF             ( 8)
  #define MC0_CH0_CR_RFP_UARCH_CounttREFIWhileRefEnOff_WID             ( 1)
  #define MC0_CH0_CR_RFP_UARCH_CounttREFIWhileRefEnOff_MSK             (0x00000100)
  #define MC0_CH0_CR_RFP_UARCH_CounttREFIWhileRefEnOff_MIN             (0)
  #define MC0_CH0_CR_RFP_UARCH_CounttREFIWhileRefEnOff_MAX             (1) // 0x00000001
  #define MC0_CH0_CR_RFP_UARCH_CounttREFIWhileRefEnOff_DEF             (0x00000000)
  #define MC0_CH0_CR_RFP_UARCH_CounttREFIWhileRefEnOff_HSH             (0x0110E440)

  #define MC0_CH0_CR_RFP_UARCH_SRX_Ref_Debits_OFF                      ( 9)
  #define MC0_CH0_CR_RFP_UARCH_SRX_Ref_Debits_WID                      ( 2)
  #define MC0_CH0_CR_RFP_UARCH_SRX_Ref_Debits_MSK                      (0x00000600)
  #define MC0_CH0_CR_RFP_UARCH_SRX_Ref_Debits_MIN                      (0)
  #define MC0_CH0_CR_RFP_UARCH_SRX_Ref_Debits_MAX                      (3) // 0x00000003
  #define MC0_CH0_CR_RFP_UARCH_SRX_Ref_Debits_DEF                      (0x00000002)
  #define MC0_CH0_CR_RFP_UARCH_SRX_Ref_Debits_HSH                      (0x0212E440)

  #define MC0_CH0_CR_RFP_UARCH_RAISE_BLK_WAIT_OFF                      (11)
  #define MC0_CH0_CR_RFP_UARCH_RAISE_BLK_WAIT_WID                      ( 4)
  #define MC0_CH0_CR_RFP_UARCH_RAISE_BLK_WAIT_MSK                      (0x00007800)
  #define MC0_CH0_CR_RFP_UARCH_RAISE_BLK_WAIT_MIN                      (0)
  #define MC0_CH0_CR_RFP_UARCH_RAISE_BLK_WAIT_MAX                      (15) // 0x0000000F
  #define MC0_CH0_CR_RFP_UARCH_RAISE_BLK_WAIT_DEF                      (0x00000005)
  #define MC0_CH0_CR_RFP_UARCH_RAISE_BLK_WAIT_HSH                      (0x0416E440)

#define MC0_CH0_CR_MC_REFRESH_STAGGER_REG                              (0x0000E444)
#define MC0_CH0_CR_MC_REFRESH_STAGGER_DEF                              (0x00000000)
#define MC0_CH0_CR_MC_REFRESH_STAGGER_VOLATILE_BITFIELDS_MSK           (0x00000000)

  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Ref_Interval_OFF               ( 0)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Ref_Interval_WID               (13)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Ref_Interval_MSK               (0x00001FFF)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Ref_Interval_MIN               (0)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Ref_Interval_MAX               (4095) // 0x00000FFF
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Ref_Interval_DEF               (0x00000000)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Ref_Interval_HSH               (0x0D00E444)

  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Disable_Stolen_Refresh_OFF     (13)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Disable_Stolen_Refresh_WID     ( 1)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Disable_Stolen_Refresh_MSK     (0x00002000)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Disable_Stolen_Refresh_MIN     (0)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Disable_Stolen_Refresh_MAX     (1) // 0x00000001
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Disable_Stolen_Refresh_DEF     (0x00000000)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Disable_Stolen_Refresh_HSH     (0x011AE444)

  #define MC0_CH0_CR_MC_REFRESH_STAGGER_En_Ref_Type_Display_OFF        (14)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_En_Ref_Type_Display_WID        ( 1)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_En_Ref_Type_Display_MSK        (0x00004000)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_En_Ref_Type_Display_MIN        (0)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_En_Ref_Type_Display_MAX        (1) // 0x00000001
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_En_Ref_Type_Display_DEF        (0x00000000)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_En_Ref_Type_Display_HSH        (0x011CE444)

  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Ref_Stagger_En_OFF             (15)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Ref_Stagger_En_WID             ( 1)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Ref_Stagger_En_MSK             (0x00008000)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Ref_Stagger_En_MIN             (0)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Ref_Stagger_En_MAX             (1) // 0x00000001
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Ref_Stagger_En_DEF             (0x00000000)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Ref_Stagger_En_HSH             (0x011EE444)

  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Ref_Stagger_Mode_OFF           (16)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Ref_Stagger_Mode_WID           ( 1)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Ref_Stagger_Mode_MSK           (0x00010000)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Ref_Stagger_Mode_MIN           (0)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Ref_Stagger_Mode_MAX           (1) // 0x00000001
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Ref_Stagger_Mode_DEF           (0x00000000)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Ref_Stagger_Mode_HSH           (0x0120E444)

  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Trefipulse_Stagger_Disable_OFF (17)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Trefipulse_Stagger_Disable_WID ( 1)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Trefipulse_Stagger_Disable_MSK (0x00020000)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Trefipulse_Stagger_Disable_MIN (0)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Trefipulse_Stagger_Disable_MAX (1) // 0x00000001
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Trefipulse_Stagger_Disable_DEF (0x00000000)
  #define MC0_CH0_CR_MC_REFRESH_STAGGER_Trefipulse_Stagger_Disable_HSH (0x0122E444)

#define MC0_CH0_CR_TC_ZQCAL_REG                                        (0x0000E448)
#define MC0_CH0_CR_TC_ZQCAL_DEF                                        (0x03200040)
#define MC0_CH0_CR_TC_ZQCAL_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define MC0_CH0_CR_TC_ZQCAL_tZQLATCH_OFF                             ( 0)
  #define MC0_CH0_CR_TC_ZQCAL_tZQLATCH_WID                             (11)
  #define MC0_CH0_CR_TC_ZQCAL_tZQLATCH_MSK                             (0x000007FF)
  #define MC0_CH0_CR_TC_ZQCAL_tZQLATCH_MIN                             (0)
  #define MC0_CH0_CR_TC_ZQCAL_tZQLATCH_MAX                             (1023) // 0x000003FF
  #define MC0_CH0_CR_TC_ZQCAL_tZQLATCH_DEF                             (0x00000040)
  #define MC0_CH0_CR_TC_ZQCAL_tZQLATCH_HSH                             (0x0B00E448)

  #define MC0_CH0_CR_TC_ZQCAL_tZQCAL_OFF                               (16)
  #define MC0_CH0_CR_TC_ZQCAL_tZQCAL_WID                               (13)
  #define MC0_CH0_CR_TC_ZQCAL_tZQCAL_MSK                               (0x1FFF0000)
  #define MC0_CH0_CR_TC_ZQCAL_tZQCAL_MIN                               (0)
  #define MC0_CH0_CR_TC_ZQCAL_tZQCAL_MAX                               (4095) // 0x00000FFF
  #define MC0_CH0_CR_TC_ZQCAL_tZQCAL_DEF                               (0x00000320)
  #define MC0_CH0_CR_TC_ZQCAL_tZQCAL_HSH                               (0x0D20E448)

#define MC0_CH0_CR_ZQCAL_CONTROL_REG                                   (0x0000E44C)
#define MC0_CH0_CR_ZQCAL_CONTROL_DEF                                   (0x00000080)
#define MC0_CH0_CR_ZQCAL_CONTROL_VOLATILE_BITFIELDS_MSK                (0x00000000)

  #define MC0_CH0_CR_ZQCAL_CONTROL_ZQCAL_period_OFF                    ( 0)
  #define MC0_CH0_CR_ZQCAL_CONTROL_ZQCAL_period_WID                    (10)
  #define MC0_CH0_CR_ZQCAL_CONTROL_ZQCAL_period_MSK                    (0x000003FF)
  #define MC0_CH0_CR_ZQCAL_CONTROL_ZQCAL_period_MIN                    (0)
  #define MC0_CH0_CR_ZQCAL_CONTROL_ZQCAL_period_MAX                    (1023) // 0x000003FF
  #define MC0_CH0_CR_ZQCAL_CONTROL_ZQCAL_period_DEF                    (0x00000080)
  #define MC0_CH0_CR_ZQCAL_CONTROL_ZQCAL_period_HSH                    (0x0A00E44C)

  #define MC0_CH0_CR_ZQCAL_CONTROL_serialize_zq_OFF                    (22)
  #define MC0_CH0_CR_ZQCAL_CONTROL_serialize_zq_WID                    ( 1)
  #define MC0_CH0_CR_ZQCAL_CONTROL_serialize_zq_MSK                    (0x00400000)
  #define MC0_CH0_CR_ZQCAL_CONTROL_serialize_zq_MIN                    (0)
  #define MC0_CH0_CR_ZQCAL_CONTROL_serialize_zq_MAX                    (1) // 0x00000001
  #define MC0_CH0_CR_ZQCAL_CONTROL_serialize_zq_DEF                    (0x00000000)
  #define MC0_CH0_CR_ZQCAL_CONTROL_serialize_zq_HSH                    (0x012CE44C)

  #define MC0_CH0_CR_ZQCAL_CONTROL_serial_zq_between_sub_ch_OFF        (28)
  #define MC0_CH0_CR_ZQCAL_CONTROL_serial_zq_between_sub_ch_WID        ( 1)
  #define MC0_CH0_CR_ZQCAL_CONTROL_serial_zq_between_sub_ch_MSK        (0x10000000)
  #define MC0_CH0_CR_ZQCAL_CONTROL_serial_zq_between_sub_ch_MIN        (0)
  #define MC0_CH0_CR_ZQCAL_CONTROL_serial_zq_between_sub_ch_MAX        (1) // 0x00000001
  #define MC0_CH0_CR_ZQCAL_CONTROL_serial_zq_between_sub_ch_DEF        (0x00000000)
  #define MC0_CH0_CR_ZQCAL_CONTROL_serial_zq_between_sub_ch_HSH        (0x0138E44C)

#define MC0_CH0_CR_SCH1_DDR_MR_RESULT_LOW_REG                          (0x0000E450)
//Duplicate of MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_REG

#define MC0_CH0_CR_MC_INIT_STATE_REG                                   (0x0000E454)
#define MC0_CH0_CR_MC_INIT_STATE_DEF                                   (0x0000000F)
#define MC0_CH0_CR_MC_INIT_STATE_VOLATILE_BITFIELDS_MSK                (0x00001700)

  #define MC0_CH0_CR_MC_INIT_STATE_Rank_occupancy_OFF                  ( 0)
  #define MC0_CH0_CR_MC_INIT_STATE_Rank_occupancy_WID                  ( 4)
  #define MC0_CH0_CR_MC_INIT_STATE_Rank_occupancy_MSK                  (0x0000000F)
  #define MC0_CH0_CR_MC_INIT_STATE_Rank_occupancy_MIN                  (0)
  #define MC0_CH0_CR_MC_INIT_STATE_Rank_occupancy_MAX                  (15) // 0x0000000F
  #define MC0_CH0_CR_MC_INIT_STATE_Rank_occupancy_DEF                  (0x0000000F)
  #define MC0_CH0_CR_MC_INIT_STATE_Rank_occupancy_HSH                  (0x0400E454)

  #define MC0_CH0_CR_MC_INIT_STATE_SRX_reset_OFF                       ( 8)
  #define MC0_CH0_CR_MC_INIT_STATE_SRX_reset_WID                       ( 1)
  #define MC0_CH0_CR_MC_INIT_STATE_SRX_reset_MSK                       (0x00000100)
  #define MC0_CH0_CR_MC_INIT_STATE_SRX_reset_MIN                       (0)
  #define MC0_CH0_CR_MC_INIT_STATE_SRX_reset_MAX                       (1) // 0x00000001
  #define MC0_CH0_CR_MC_INIT_STATE_SRX_reset_DEF                       (0x00000000)
  #define MC0_CH0_CR_MC_INIT_STATE_SRX_reset_HSH                       (0x0190E454)

  #define MC0_CH0_CR_MC_INIT_STATE_LPDDR_current_FSP_OFF               ( 9)
  #define MC0_CH0_CR_MC_INIT_STATE_LPDDR_current_FSP_WID               ( 1)
  #define MC0_CH0_CR_MC_INIT_STATE_LPDDR_current_FSP_MSK               (0x00000200)
  #define MC0_CH0_CR_MC_INIT_STATE_LPDDR_current_FSP_MIN               (0)
  #define MC0_CH0_CR_MC_INIT_STATE_LPDDR_current_FSP_MAX               (1) // 0x00000001
  #define MC0_CH0_CR_MC_INIT_STATE_LPDDR_current_FSP_DEF               (0x00000000)
  #define MC0_CH0_CR_MC_INIT_STATE_LPDDR_current_FSP_HSH               (0x0192E454)

  #define MC0_CH0_CR_MC_INIT_STATE_LPDDR_current_FSP_tracking_OFF      (10)
  #define MC0_CH0_CR_MC_INIT_STATE_LPDDR_current_FSP_tracking_WID      ( 1)
  #define MC0_CH0_CR_MC_INIT_STATE_LPDDR_current_FSP_tracking_MSK      (0x00000400)
  #define MC0_CH0_CR_MC_INIT_STATE_LPDDR_current_FSP_tracking_MIN      (0)
  #define MC0_CH0_CR_MC_INIT_STATE_LPDDR_current_FSP_tracking_MAX      (1) // 0x00000001
  #define MC0_CH0_CR_MC_INIT_STATE_LPDDR_current_FSP_tracking_DEF      (0x00000000)
  #define MC0_CH0_CR_MC_INIT_STATE_LPDDR_current_FSP_tracking_HSH      (0x0194E454)

  #define MC0_CH0_CR_MC_INIT_STATE_Deep_SRX_reset_OFF                  (12)
  #define MC0_CH0_CR_MC_INIT_STATE_Deep_SRX_reset_WID                  ( 1)
  #define MC0_CH0_CR_MC_INIT_STATE_Deep_SRX_reset_MSK                  (0x00001000)
  #define MC0_CH0_CR_MC_INIT_STATE_Deep_SRX_reset_MIN                  (0)
  #define MC0_CH0_CR_MC_INIT_STATE_Deep_SRX_reset_MAX                  (1) // 0x00000001
  #define MC0_CH0_CR_MC_INIT_STATE_Deep_SRX_reset_DEF                  (0x00000000)
  #define MC0_CH0_CR_MC_INIT_STATE_Deep_SRX_reset_HSH                  (0x0198E454)

#define MC0_CH0_CR_WDB_VISA_SEL_REG                                    (0x0000E458)
#define MC0_CH0_CR_WDB_VISA_SEL_DEF                                    (0x00001C00)
#define MC0_CH0_CR_WDB_VISA_SEL_VOLATILE_BITFIELDS_MSK                 (0x00000000)

  #define MC0_CH0_CR_WDB_VISA_SEL_VISAByteSel_OFF                      ( 0)
  #define MC0_CH0_CR_WDB_VISA_SEL_VISAByteSel_WID                      ( 3)
  #define MC0_CH0_CR_WDB_VISA_SEL_VISAByteSel_MSK                      (0x00000007)
  #define MC0_CH0_CR_WDB_VISA_SEL_VISAByteSel_MIN                      (0)
  #define MC0_CH0_CR_WDB_VISA_SEL_VISAByteSel_MAX                      (7) // 0x00000007
  #define MC0_CH0_CR_WDB_VISA_SEL_VISAByteSel_DEF                      (0x00000000)
  #define MC0_CH0_CR_WDB_VISA_SEL_VISAByteSel_HSH                      (0x0300E458)

  #define MC0_CH0_CR_WDB_VISA_SEL_RefFSMRankSel_OFF                    ( 8)
  #define MC0_CH0_CR_WDB_VISA_SEL_RefFSMRankSel_WID                    ( 2)
  #define MC0_CH0_CR_WDB_VISA_SEL_RefFSMRankSel_MSK                    (0x00000300)
  #define MC0_CH0_CR_WDB_VISA_SEL_RefFSMRankSel_MIN                    (0)
  #define MC0_CH0_CR_WDB_VISA_SEL_RefFSMRankSel_MAX                    (3) // 0x00000003
  #define MC0_CH0_CR_WDB_VISA_SEL_RefFSMRankSel_DEF                    (0x00000000)
  #define MC0_CH0_CR_WDB_VISA_SEL_RefFSMRankSel_HSH                    (0x0210E458)

  #define MC0_CH0_CR_WDB_VISA_SEL_Ref_counterSel_OFF                   (10)
  #define MC0_CH0_CR_WDB_VISA_SEL_Ref_counterSel_WID                   ( 4)
  #define MC0_CH0_CR_WDB_VISA_SEL_Ref_counterSel_MSK                   (0x00003C00)
  #define MC0_CH0_CR_WDB_VISA_SEL_Ref_counterSel_MIN                   (0)
  #define MC0_CH0_CR_WDB_VISA_SEL_Ref_counterSel_MAX                   (15) // 0x0000000F
  #define MC0_CH0_CR_WDB_VISA_SEL_Ref_counterSel_DEF                   (0x00000007)
  #define MC0_CH0_CR_WDB_VISA_SEL_Ref_counterSel_HSH                   (0x0414E458)

  #define MC0_CH0_CR_WDB_VISA_SEL_Enable_Ref_Status_Sample_OFF         (14)
  #define MC0_CH0_CR_WDB_VISA_SEL_Enable_Ref_Status_Sample_WID         ( 1)
  #define MC0_CH0_CR_WDB_VISA_SEL_Enable_Ref_Status_Sample_MSK         (0x00004000)
  #define MC0_CH0_CR_WDB_VISA_SEL_Enable_Ref_Status_Sample_MIN         (0)
  #define MC0_CH0_CR_WDB_VISA_SEL_Enable_Ref_Status_Sample_MAX         (1) // 0x00000001
  #define MC0_CH0_CR_WDB_VISA_SEL_Enable_Ref_Status_Sample_DEF         (0x00000000)
  #define MC0_CH0_CR_WDB_VISA_SEL_Enable_Ref_Status_Sample_HSH         (0x011CE458)

  #define MC0_CH0_CR_WDB_VISA_SEL_RHFSMRankSel_OFF                     (15)
  #define MC0_CH0_CR_WDB_VISA_SEL_RHFSMRankSel_WID                     ( 2)
  #define MC0_CH0_CR_WDB_VISA_SEL_RHFSMRankSel_MSK                     (0x00018000)
  #define MC0_CH0_CR_WDB_VISA_SEL_RHFSMRankSel_MIN                     (0)
  #define MC0_CH0_CR_WDB_VISA_SEL_RHFSMRankSel_MAX                     (3) // 0x00000003
  #define MC0_CH0_CR_WDB_VISA_SEL_RHFSMRankSel_DEF                     (0x00000000)
  #define MC0_CH0_CR_WDB_VISA_SEL_RHFSMRankSel_HSH                     (0x021EE458)

#define MC0_CH0_CR_REF_FSM_STATUS_REG                                  (0x0000E460)
#define MC0_CH0_CR_REF_FSM_STATUS_DEF                                  (0x0000000000000000)
#define MC0_CH0_CR_REF_FSM_STATUS_VOLATILE_BITFIELDS_MSK               (0x000000FF01FFFFFF)

  #define MC0_CH0_CR_REF_FSM_STATUS_RD_RETRAIN_INTERVAL_FSM_status_OFF ( 0)
  #define MC0_CH0_CR_REF_FSM_STATUS_RD_RETRAIN_INTERVAL_FSM_status_WID ( 4)
  #define MC0_CH0_CR_REF_FSM_STATUS_RD_RETRAIN_INTERVAL_FSM_status_MSK (0x0000000F)
  #define MC0_CH0_CR_REF_FSM_STATUS_RD_RETRAIN_INTERVAL_FSM_status_MIN (0)
  #define MC0_CH0_CR_REF_FSM_STATUS_RD_RETRAIN_INTERVAL_FSM_status_MAX (15) // 0x0000000F
  #define MC0_CH0_CR_REF_FSM_STATUS_RD_RETRAIN_INTERVAL_FSM_status_DEF (0x00000000)
  #define MC0_CH0_CR_REF_FSM_STATUS_RD_RETRAIN_INTERVAL_FSM_status_HSH (0x4480E460)

  #define MC0_CH0_CR_REF_FSM_STATUS_WR_RETRAIN_INTERVAL_FSM_status_OFF ( 4)
  #define MC0_CH0_CR_REF_FSM_STATUS_WR_RETRAIN_INTERVAL_FSM_status_WID ( 4)
  #define MC0_CH0_CR_REF_FSM_STATUS_WR_RETRAIN_INTERVAL_FSM_status_MSK (0x000000F0)
  #define MC0_CH0_CR_REF_FSM_STATUS_WR_RETRAIN_INTERVAL_FSM_status_MIN (0)
  #define MC0_CH0_CR_REF_FSM_STATUS_WR_RETRAIN_INTERVAL_FSM_status_MAX (15) // 0x0000000F
  #define MC0_CH0_CR_REF_FSM_STATUS_WR_RETRAIN_INTERVAL_FSM_status_DEF (0x00000000)
  #define MC0_CH0_CR_REF_FSM_STATUS_WR_RETRAIN_INTERVAL_FSM_status_HSH (0x4488E460)

  #define MC0_CH0_CR_REF_FSM_STATUS_TEMP_RD_FSM_status_OFF             ( 8)
  #define MC0_CH0_CR_REF_FSM_STATUS_TEMP_RD_FSM_status_WID             ( 3)
  #define MC0_CH0_CR_REF_FSM_STATUS_TEMP_RD_FSM_status_MSK             (0x00000700)
  #define MC0_CH0_CR_REF_FSM_STATUS_TEMP_RD_FSM_status_MIN             (0)
  #define MC0_CH0_CR_REF_FSM_STATUS_TEMP_RD_FSM_status_MAX             (7) // 0x00000007
  #define MC0_CH0_CR_REF_FSM_STATUS_TEMP_RD_FSM_status_DEF             (0x00000000)
  #define MC0_CH0_CR_REF_FSM_STATUS_TEMP_RD_FSM_status_HSH             (0x4390E460)

  #define MC0_CH0_CR_REF_FSM_STATUS_ZQCS_FSM_status_OFF                (11)
  #define MC0_CH0_CR_REF_FSM_STATUS_ZQCS_FSM_status_WID                ( 3)
  #define MC0_CH0_CR_REF_FSM_STATUS_ZQCS_FSM_status_MSK                (0x00003800)
  #define MC0_CH0_CR_REF_FSM_STATUS_ZQCS_FSM_status_MIN                (0)
  #define MC0_CH0_CR_REF_FSM_STATUS_ZQCS_FSM_status_MAX                (7) // 0x00000007
  #define MC0_CH0_CR_REF_FSM_STATUS_ZQCS_FSM_status_DEF                (0x00000000)
  #define MC0_CH0_CR_REF_FSM_STATUS_ZQCS_FSM_status_HSH                (0x4396E460)

  #define MC0_CH0_CR_REF_FSM_STATUS_EXE_REF_FSM_status_OFF             (14)
  #define MC0_CH0_CR_REF_FSM_STATUS_EXE_REF_FSM_status_WID             ( 3)
  #define MC0_CH0_CR_REF_FSM_STATUS_EXE_REF_FSM_status_MSK             (0x0001C000)
  #define MC0_CH0_CR_REF_FSM_STATUS_EXE_REF_FSM_status_MIN             (0)
  #define MC0_CH0_CR_REF_FSM_STATUS_EXE_REF_FSM_status_MAX             (7) // 0x00000007
  #define MC0_CH0_CR_REF_FSM_STATUS_EXE_REF_FSM_status_DEF             (0x00000000)
  #define MC0_CH0_CR_REF_FSM_STATUS_EXE_REF_FSM_status_HSH             (0x439CE460)

  #define MC0_CH0_CR_REF_FSM_STATUS_MAIN_REF_FSM_status_OFF            (17)
  #define MC0_CH0_CR_REF_FSM_STATUS_MAIN_REF_FSM_status_WID            ( 4)
  #define MC0_CH0_CR_REF_FSM_STATUS_MAIN_REF_FSM_status_MSK            (0x001E0000)
  #define MC0_CH0_CR_REF_FSM_STATUS_MAIN_REF_FSM_status_MIN            (0)
  #define MC0_CH0_CR_REF_FSM_STATUS_MAIN_REF_FSM_status_MAX            (15) // 0x0000000F
  #define MC0_CH0_CR_REF_FSM_STATUS_MAIN_REF_FSM_status_DEF            (0x00000000)
  #define MC0_CH0_CR_REF_FSM_STATUS_MAIN_REF_FSM_status_HSH            (0x44A2E460)

  #define MC0_CH0_CR_REF_FSM_STATUS_MAIN_SR_FSM_status_OFF             (21)
  #define MC0_CH0_CR_REF_FSM_STATUS_MAIN_SR_FSM_status_WID             ( 4)
  #define MC0_CH0_CR_REF_FSM_STATUS_MAIN_SR_FSM_status_MSK             (0x01E00000)
  #define MC0_CH0_CR_REF_FSM_STATUS_MAIN_SR_FSM_status_MIN             (0)
  #define MC0_CH0_CR_REF_FSM_STATUS_MAIN_SR_FSM_status_MAX             (15) // 0x0000000F
  #define MC0_CH0_CR_REF_FSM_STATUS_MAIN_SR_FSM_status_DEF             (0x00000000)
  #define MC0_CH0_CR_REF_FSM_STATUS_MAIN_SR_FSM_status_HSH             (0x44AAE460)

  #define MC0_CH0_CR_REF_FSM_STATUS_counter_status_OFF                 (32)
  #define MC0_CH0_CR_REF_FSM_STATUS_counter_status_WID                 ( 8)
  #define MC0_CH0_CR_REF_FSM_STATUS_counter_status_MSK                 (0x000000FF00000000ULL)
  #define MC0_CH0_CR_REF_FSM_STATUS_counter_status_MIN                 (0)
  #define MC0_CH0_CR_REF_FSM_STATUS_counter_status_MAX                 (255) // 0x000000FF
  #define MC0_CH0_CR_REF_FSM_STATUS_counter_status_DEF                 (0x00000000)
  #define MC0_CH0_CR_REF_FSM_STATUS_counter_status_HSH                 (0x48C0E460)

#define MC0_CH0_CR_PM_DIMM_ACT_ENERGY_REG                              (0x0000E468)
#define MC0_CH0_CR_PM_DIMM_ACT_ENERGY_DEF                              (0x00000000)
#define MC0_CH0_CR_PM_DIMM_ACT_ENERGY_VOLATILE_BITFIELDS_MSK           (0x00000000)

  #define MC0_CH0_CR_PM_DIMM_ACT_ENERGY_DIMM0_ACT_ENERGY_OFF           ( 0)
  #define MC0_CH0_CR_PM_DIMM_ACT_ENERGY_DIMM0_ACT_ENERGY_WID           ( 8)
  #define MC0_CH0_CR_PM_DIMM_ACT_ENERGY_DIMM0_ACT_ENERGY_MSK           (0x000000FF)
  #define MC0_CH0_CR_PM_DIMM_ACT_ENERGY_DIMM0_ACT_ENERGY_MIN           (0)
  #define MC0_CH0_CR_PM_DIMM_ACT_ENERGY_DIMM0_ACT_ENERGY_MAX           (255) // 0x000000FF
  #define MC0_CH0_CR_PM_DIMM_ACT_ENERGY_DIMM0_ACT_ENERGY_DEF           (0x00000000)
  #define MC0_CH0_CR_PM_DIMM_ACT_ENERGY_DIMM0_ACT_ENERGY_HSH           (0x0800E468)

  #define MC0_CH0_CR_PM_DIMM_ACT_ENERGY_DIMM1_ACT_ENERGY_OFF           ( 8)
  #define MC0_CH0_CR_PM_DIMM_ACT_ENERGY_DIMM1_ACT_ENERGY_WID           ( 8)
  #define MC0_CH0_CR_PM_DIMM_ACT_ENERGY_DIMM1_ACT_ENERGY_MSK           (0x0000FF00)
  #define MC0_CH0_CR_PM_DIMM_ACT_ENERGY_DIMM1_ACT_ENERGY_MIN           (0)
  #define MC0_CH0_CR_PM_DIMM_ACT_ENERGY_DIMM1_ACT_ENERGY_MAX           (255) // 0x000000FF
  #define MC0_CH0_CR_PM_DIMM_ACT_ENERGY_DIMM1_ACT_ENERGY_DEF           (0x00000000)
  #define MC0_CH0_CR_PM_DIMM_ACT_ENERGY_DIMM1_ACT_ENERGY_HSH           (0x0810E468)

#define MC0_CH0_CR_PM_DIMM_RD_ENERGY_REG                               (0x0000E46C)
#define MC0_CH0_CR_PM_DIMM_RD_ENERGY_DEF                               (0x00080101)
#define MC0_CH0_CR_PM_DIMM_RD_ENERGY_VOLATILE_BITFIELDS_MSK            (0x00000000)

  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_dimm0_rd_energy_OFF             ( 0)
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_dimm0_rd_energy_WID             ( 8)
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_dimm0_rd_energy_MSK             (0x000000FF)
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_dimm0_rd_energy_MIN             (0)
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_dimm0_rd_energy_MAX             (255) // 0x000000FF
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_dimm0_rd_energy_DEF             (0x00000001)
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_dimm0_rd_energy_HSH             (0x0800E46C)

  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_dimm1_rd_energy_OFF             ( 8)
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_dimm1_rd_energy_WID             ( 8)
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_dimm1_rd_energy_MSK             (0x0000FF00)
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_dimm1_rd_energy_MIN             (0)
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_dimm1_rd_energy_MAX             (255) // 0x000000FF
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_dimm1_rd_energy_DEF             (0x00000001)
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_dimm1_rd_energy_HSH             (0x0810E46C)

  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_read_vc0_rapl_limit_OFF         (16)
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_read_vc0_rapl_limit_WID         ( 5)
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_read_vc0_rapl_limit_MSK         (0x001F0000)
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_read_vc0_rapl_limit_MIN         (0)
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_read_vc0_rapl_limit_MAX         (16) // 0x00000010
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_read_vc0_rapl_limit_DEF         (0x00000008)
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_read_vc0_rapl_limit_HSH         (0x0520E46C)

  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_dis_vc1_rd_energy_OFF           (21)
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_dis_vc1_rd_energy_WID           ( 1)
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_dis_vc1_rd_energy_MSK           (0x00200000)
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_dis_vc1_rd_energy_MIN           (0)
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_dis_vc1_rd_energy_MAX           (1) // 0x00000001
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_dis_vc1_rd_energy_DEF           (0x00000000)
  #define MC0_CH0_CR_PM_DIMM_RD_ENERGY_dis_vc1_rd_energy_HSH           (0x012AE46C)

#define MC0_CH0_CR_PM_DIMM_WR_ENERGY_REG                               (0x0000E470)
#define MC0_CH0_CR_PM_DIMM_WR_ENERGY_DEF                               (0x01080101)
#define MC0_CH0_CR_PM_DIMM_WR_ENERGY_VOLATILE_BITFIELDS_MSK            (0x00000000)

  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_dimm0_wr_energy_OFF             ( 0)
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_dimm0_wr_energy_WID             ( 8)
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_dimm0_wr_energy_MSK             (0x000000FF)
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_dimm0_wr_energy_MIN             (0)
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_dimm0_wr_energy_MAX             (255) // 0x000000FF
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_dimm0_wr_energy_DEF             (0x00000001)
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_dimm0_wr_energy_HSH             (0x0800E470)

  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_dimm1_wr_energy_OFF             ( 8)
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_dimm1_wr_energy_WID             ( 8)
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_dimm1_wr_energy_MSK             (0x0000FF00)
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_dimm1_wr_energy_MIN             (0)
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_dimm1_wr_energy_MAX             (255) // 0x000000FF
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_dimm1_wr_energy_DEF             (0x00000001)
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_dimm1_wr_energy_HSH             (0x0810E470)

  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_write_rapl_limit_OFF            (16)
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_write_rapl_limit_WID            ( 5)
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_write_rapl_limit_MSK            (0x001F0000)
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_write_rapl_limit_MIN            (0)
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_write_rapl_limit_MAX            (24) // 0x00000018
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_write_rapl_limit_DEF            (0x00000008)
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_write_rapl_limit_HSH            (0x0520E470)

  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_write_rapl_limit_vc1priority_OFF (21)
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_write_rapl_limit_vc1priority_WID ( 5)
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_write_rapl_limit_vc1priority_MSK (0x03E00000)
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_write_rapl_limit_vc1priority_MIN (0)
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_write_rapl_limit_vc1priority_MAX (24) // 0x00000018
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_write_rapl_limit_vc1priority_DEF (0x00000008)
  #define MC0_CH0_CR_PM_DIMM_WR_ENERGY_write_rapl_limit_vc1priority_HSH (0x052AE470)

#define MC0_CH0_CR_PM_SREF_CONTROL_REG                                 (0x0000E474)
#define MC0_CH0_CR_PM_SREF_CONTROL_DEF                                 (0x00001B7F)
#define MC0_CH0_CR_PM_SREF_CONTROL_VOLATILE_BITFIELDS_MSK              (0x00000000)

  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_ZQCAL_OFF                 ( 0)
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_ZQCAL_WID                 ( 1)
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_ZQCAL_MSK                 (0x00000001)
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_ZQCAL_MIN                 (0)
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_ZQCAL_MAX                 (1) // 0x00000001
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_ZQCAL_DEF                 (0x00000001)
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_ZQCAL_HSH                 (0x0100E474)

  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_TEMP_READ_OFF             ( 1)
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_TEMP_READ_WID             ( 1)
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_TEMP_READ_MSK             (0x00000002)
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_TEMP_READ_MIN             (0)
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_TEMP_READ_MAX             (1) // 0x00000001
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_TEMP_READ_DEF             (0x00000001)
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_TEMP_READ_HSH             (0x0102E474)

  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_RD_RETRAINING_OFF         ( 2)
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_RD_RETRAINING_WID         ( 1)
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_RD_RETRAINING_MSK         (0x00000004)
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_RD_RETRAINING_MIN         (0)
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_RD_RETRAINING_MAX         (1) // 0x00000001
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_RD_RETRAINING_DEF         (0x00000001)
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_RD_RETRAINING_HSH         (0x0104E474)

  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_WR_RETRAINING_OFF         ( 3)
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_WR_RETRAINING_WID         ( 1)
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_WR_RETRAINING_MSK         (0x00000008)
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_WR_RETRAINING_MIN         (0)
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_WR_RETRAINING_MAX         (1) // 0x00000001
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_WR_RETRAINING_DEF         (0x00000001)
  #define MC0_CH0_CR_PM_SREF_CONTROL_WAKE_ON_WR_RETRAINING_HSH         (0x0106E474)

  #define MC0_CH0_CR_PM_SREF_CONTROL_BLOCK_WR_UNTIL_RETRAIN_DONE_OFF   ( 4)
  #define MC0_CH0_CR_PM_SREF_CONTROL_BLOCK_WR_UNTIL_RETRAIN_DONE_WID   ( 1)
  #define MC0_CH0_CR_PM_SREF_CONTROL_BLOCK_WR_UNTIL_RETRAIN_DONE_MSK   (0x00000010)
  #define MC0_CH0_CR_PM_SREF_CONTROL_BLOCK_WR_UNTIL_RETRAIN_DONE_MIN   (0)
  #define MC0_CH0_CR_PM_SREF_CONTROL_BLOCK_WR_UNTIL_RETRAIN_DONE_MAX   (1) // 0x00000001
  #define MC0_CH0_CR_PM_SREF_CONTROL_BLOCK_WR_UNTIL_RETRAIN_DONE_DEF   (0x00000001)
  #define MC0_CH0_CR_PM_SREF_CONTROL_BLOCK_WR_UNTIL_RETRAIN_DONE_HSH   (0x0108E474)

  #define MC0_CH0_CR_PM_SREF_CONTROL_BLOCK_RD_UNTIL_RETRAIN_DONE_OFF   ( 5)
  #define MC0_CH0_CR_PM_SREF_CONTROL_BLOCK_RD_UNTIL_RETRAIN_DONE_WID   ( 1)
  #define MC0_CH0_CR_PM_SREF_CONTROL_BLOCK_RD_UNTIL_RETRAIN_DONE_MSK   (0x00000020)
  #define MC0_CH0_CR_PM_SREF_CONTROL_BLOCK_RD_UNTIL_RETRAIN_DONE_MIN   (0)
  #define MC0_CH0_CR_PM_SREF_CONTROL_BLOCK_RD_UNTIL_RETRAIN_DONE_MAX   (1) // 0x00000001
  #define MC0_CH0_CR_PM_SREF_CONTROL_BLOCK_RD_UNTIL_RETRAIN_DONE_DEF   (0x00000001)
  #define MC0_CH0_CR_PM_SREF_CONTROL_BLOCK_RD_UNTIL_RETRAIN_DONE_HSH   (0x010AE474)

  #define MC0_CH0_CR_PM_SREF_CONTROL_BLOCK_UNTIL_ZQ_DONE_OFF           ( 6)
  #define MC0_CH0_CR_PM_SREF_CONTROL_BLOCK_UNTIL_ZQ_DONE_WID           ( 1)
  #define MC0_CH0_CR_PM_SREF_CONTROL_BLOCK_UNTIL_ZQ_DONE_MSK           (0x00000040)
  #define MC0_CH0_CR_PM_SREF_CONTROL_BLOCK_UNTIL_ZQ_DONE_MIN           (0)
  #define MC0_CH0_CR_PM_SREF_CONTROL_BLOCK_UNTIL_ZQ_DONE_MAX           (1) // 0x00000001
  #define MC0_CH0_CR_PM_SREF_CONTROL_BLOCK_UNTIL_ZQ_DONE_DEF           (0x00000001)
  #define MC0_CH0_CR_PM_SREF_CONTROL_BLOCK_UNTIL_ZQ_DONE_HSH           (0x010CE474)

  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_ZQCAL_OFF                  ( 8)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_ZQCAL_WID                  ( 1)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_ZQCAL_MSK                  (0x00000100)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_ZQCAL_MIN                  (0)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_ZQCAL_MAX                  (1) // 0x00000001
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_ZQCAL_DEF                  (0x00000001)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_ZQCAL_HSH                  (0x0110E474)

  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_TEMP_READ_OFF              ( 9)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_TEMP_READ_WID              ( 1)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_TEMP_READ_MSK              (0x00000200)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_TEMP_READ_MIN              (0)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_TEMP_READ_MAX              (1) // 0x00000001
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_TEMP_READ_DEF              (0x00000001)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_TEMP_READ_HSH              (0x0112E474)

  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_RD_RETRAINING_OFF          (10)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_RD_RETRAINING_WID          ( 1)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_RD_RETRAINING_MSK          (0x00000400)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_RD_RETRAINING_MIN          (0)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_RD_RETRAINING_MAX          (1) // 0x00000001
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_RD_RETRAINING_DEF          (0x00000000)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_RD_RETRAINING_HSH          (0x0114E474)

  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_WR_RETRAINING_OFF          (11)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_WR_RETRAINING_WID          ( 1)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_WR_RETRAINING_MSK          (0x00000800)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_WR_RETRAINING_MIN          (0)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_WR_RETRAINING_MAX          (1) // 0x00000001
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_WR_RETRAINING_DEF          (0x00000001)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_SRX_WR_RETRAINING_HSH          (0x0116E474)

  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_PWRDN_IN_SR_OFF                (12)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_PWRDN_IN_SR_WID                ( 1)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_PWRDN_IN_SR_MSK                (0x00001000)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_PWRDN_IN_SR_MIN                (0)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_PWRDN_IN_SR_MAX                (1) // 0x00000001
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_PWRDN_IN_SR_DEF                (0x00000001)
  #define MC0_CH0_CR_PM_SREF_CONTROL_EN_PWRDN_IN_SR_HSH                (0x0118E474)

  #define MC0_CH0_CR_PM_SREF_CONTROL_force_sre_with_freq_change_OFF    (13)
  #define MC0_CH0_CR_PM_SREF_CONTROL_force_sre_with_freq_change_WID    ( 1)
  #define MC0_CH0_CR_PM_SREF_CONTROL_force_sre_with_freq_change_MSK    (0x00002000)
  #define MC0_CH0_CR_PM_SREF_CONTROL_force_sre_with_freq_change_MIN    (0)
  #define MC0_CH0_CR_PM_SREF_CONTROL_force_sre_with_freq_change_MAX    (1) // 0x00000001
  #define MC0_CH0_CR_PM_SREF_CONTROL_force_sre_with_freq_change_DEF    (0x00000000)
  #define MC0_CH0_CR_PM_SREF_CONTROL_force_sre_with_freq_change_HSH    (0x011AE474)

#define MC0_CH0_CR_SC_WR_DELAY_REG                                     (0x0000E478)
#define MC0_CH0_CR_SC_WR_DELAY_DEF                                     (0x00000002)
#define MC0_CH0_CR_SC_WR_DELAY_VOLATILE_BITFIELDS_MSK                  (0x00000000)

  #define MC0_CH0_CR_SC_WR_DELAY_Dec_tCWL_OFF                          ( 0)
  #define MC0_CH0_CR_SC_WR_DELAY_Dec_tCWL_WID                          ( 6)
  #define MC0_CH0_CR_SC_WR_DELAY_Dec_tCWL_MSK                          (0x0000003F)
  #define MC0_CH0_CR_SC_WR_DELAY_Dec_tCWL_MIN                          (0)
  #define MC0_CH0_CR_SC_WR_DELAY_Dec_tCWL_MAX                          (63) // 0x0000003F
  #define MC0_CH0_CR_SC_WR_DELAY_Dec_tCWL_DEF                          (0x00000002)
  #define MC0_CH0_CR_SC_WR_DELAY_Dec_tCWL_HSH                          (0x0600E478)

  #define MC0_CH0_CR_SC_WR_DELAY_Add_tCWL_OFF                          ( 6)
  #define MC0_CH0_CR_SC_WR_DELAY_Add_tCWL_WID                          ( 6)
  #define MC0_CH0_CR_SC_WR_DELAY_Add_tCWL_MSK                          (0x00000FC0)
  #define MC0_CH0_CR_SC_WR_DELAY_Add_tCWL_MIN                          (0)
  #define MC0_CH0_CR_SC_WR_DELAY_Add_tCWL_MAX                          (63) // 0x0000003F
  #define MC0_CH0_CR_SC_WR_DELAY_Add_tCWL_DEF                          (0x00000000)
  #define MC0_CH0_CR_SC_WR_DELAY_Add_tCWL_HSH                          (0x060CE478)

#define MC0_CH0_CR_READ_RETURN_DFT_REG                                 (0x0000E47C)
#define MC0_CH0_CR_READ_RETURN_DFT_DEF                                 (0x00000000)
#define MC0_CH0_CR_READ_RETURN_DFT_VOLATILE_BITFIELDS_MSK              (0x00000000)

  #define MC0_CH0_CR_READ_RETURN_DFT_ECC_OFF                           ( 0)
  #define MC0_CH0_CR_READ_RETURN_DFT_ECC_WID                           ( 8)
  #define MC0_CH0_CR_READ_RETURN_DFT_ECC_MSK                           (0x000000FF)
  #define MC0_CH0_CR_READ_RETURN_DFT_ECC_MIN                           (0)
  #define MC0_CH0_CR_READ_RETURN_DFT_ECC_MAX                           (255) // 0x000000FF
  #define MC0_CH0_CR_READ_RETURN_DFT_ECC_DEF                           (0x00000000)
  #define MC0_CH0_CR_READ_RETURN_DFT_ECC_HSH                           (0x0800E47C)

  #define MC0_CH0_CR_READ_RETURN_DFT_RRD_DFT_Mode_OFF                  ( 8)
  #define MC0_CH0_CR_READ_RETURN_DFT_RRD_DFT_Mode_WID                  ( 2)
  #define MC0_CH0_CR_READ_RETURN_DFT_RRD_DFT_Mode_MSK                  (0x00000300)
  #define MC0_CH0_CR_READ_RETURN_DFT_RRD_DFT_Mode_MIN                  (0)
  #define MC0_CH0_CR_READ_RETURN_DFT_RRD_DFT_Mode_MAX                  (3) // 0x00000003
  #define MC0_CH0_CR_READ_RETURN_DFT_RRD_DFT_Mode_DEF                  (0x00000000)
  #define MC0_CH0_CR_READ_RETURN_DFT_RRD_DFT_Mode_HSH                  (0x0210E47C)

  #define MC0_CH0_CR_READ_RETURN_DFT_LFSR_Seed_Index_OFF               (10)
  #define MC0_CH0_CR_READ_RETURN_DFT_LFSR_Seed_Index_WID               ( 5)
  #define MC0_CH0_CR_READ_RETURN_DFT_LFSR_Seed_Index_MSK               (0x00007C00)
  #define MC0_CH0_CR_READ_RETURN_DFT_LFSR_Seed_Index_MIN               (0)
  #define MC0_CH0_CR_READ_RETURN_DFT_LFSR_Seed_Index_MAX               (31) // 0x0000001F
  #define MC0_CH0_CR_READ_RETURN_DFT_LFSR_Seed_Index_DEF               (0x00000000)
  #define MC0_CH0_CR_READ_RETURN_DFT_LFSR_Seed_Index_HSH               (0x0514E47C)

  #define MC0_CH0_CR_READ_RETURN_DFT_Inversion_Mode_OFF                (15)
  #define MC0_CH0_CR_READ_RETURN_DFT_Inversion_Mode_WID                ( 1)
  #define MC0_CH0_CR_READ_RETURN_DFT_Inversion_Mode_MSK                (0x00008000)
  #define MC0_CH0_CR_READ_RETURN_DFT_Inversion_Mode_MIN                (0)
  #define MC0_CH0_CR_READ_RETURN_DFT_Inversion_Mode_MAX                (1) // 0x00000001
  #define MC0_CH0_CR_READ_RETURN_DFT_Inversion_Mode_DEF                (0x00000000)
  #define MC0_CH0_CR_READ_RETURN_DFT_Inversion_Mode_HSH                (0x011EE47C)

  #define MC0_CH0_CR_READ_RETURN_DFT_AddressBased_OFF                  (16)
  #define MC0_CH0_CR_READ_RETURN_DFT_AddressBased_WID                  ( 1)
  #define MC0_CH0_CR_READ_RETURN_DFT_AddressBased_MSK                  (0x00010000)
  #define MC0_CH0_CR_READ_RETURN_DFT_AddressBased_MIN                  (0)
  #define MC0_CH0_CR_READ_RETURN_DFT_AddressBased_MAX                  (1) // 0x00000001
  #define MC0_CH0_CR_READ_RETURN_DFT_AddressBased_DEF                  (0x00000000)
  #define MC0_CH0_CR_READ_RETURN_DFT_AddressBased_HSH                  (0x0120E47C)

  #define MC0_CH0_CR_READ_RETURN_DFT_AddressBasedRRD_mcsel_OFF         (17)
  #define MC0_CH0_CR_READ_RETURN_DFT_AddressBasedRRD_mcsel_WID         ( 1)
  #define MC0_CH0_CR_READ_RETURN_DFT_AddressBasedRRD_mcsel_MSK         (0x00020000)
  #define MC0_CH0_CR_READ_RETURN_DFT_AddressBasedRRD_mcsel_MIN         (0)
  #define MC0_CH0_CR_READ_RETURN_DFT_AddressBasedRRD_mcsel_MAX         (1) // 0x00000001
  #define MC0_CH0_CR_READ_RETURN_DFT_AddressBasedRRD_mcsel_DEF         (0x00000000)
  #define MC0_CH0_CR_READ_RETURN_DFT_AddressBasedRRD_mcsel_HSH         (0x0122E47C)

  #define MC0_CH0_CR_READ_RETURN_DFT_WDAR_OFF                          (18)
  #define MC0_CH0_CR_READ_RETURN_DFT_WDAR_WID                          ( 1)
  #define MC0_CH0_CR_READ_RETURN_DFT_WDAR_MSK                          (0x00040000)
  #define MC0_CH0_CR_READ_RETURN_DFT_WDAR_MIN                          (0)
  #define MC0_CH0_CR_READ_RETURN_DFT_WDAR_MAX                          (1) // 0x00000001
  #define MC0_CH0_CR_READ_RETURN_DFT_WDAR_DEF                          (0x00000000)
  #define MC0_CH0_CR_READ_RETURN_DFT_WDAR_HSH                          (0x0124E47C)

#define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_REG                              (0x0000E480)
#define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_DEF                              (0x23444688)
#define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_VOLATILE_BITFIELDS_MSK           (0x00000000)

  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_0_OFF                      ( 0)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_0_WID                      ( 3)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_0_MSK                      (0x00000007)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_0_MIN                      (0)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_0_MAX                      (7) // 0x00000007
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_0_DEF                      (0x00000000)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_0_HSH                      (0x0300E480)

  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_1_OFF                      ( 3)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_1_WID                      ( 3)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_1_MSK                      (0x00000038)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_1_MIN                      (0)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_1_MAX                      (7) // 0x00000007
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_1_DEF                      (0x00000001)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_1_HSH                      (0x0306E480)

  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_2_OFF                      ( 6)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_2_WID                      ( 3)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_2_MSK                      (0x000001C0)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_2_MIN                      (0)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_2_MAX                      (7) // 0x00000007
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_2_DEF                      (0x00000002)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_2_HSH                      (0x030CE480)

  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_3_OFF                      ( 9)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_3_WID                      ( 3)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_3_MSK                      (0x00000E00)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_3_MIN                      (0)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_3_MAX                      (7) // 0x00000007
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_3_DEF                      (0x00000003)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_3_HSH                      (0x0312E480)

  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_4_OFF                      (12)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_4_WID                      ( 3)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_4_MSK                      (0x00007000)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_4_MIN                      (0)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_4_MAX                      (7) // 0x00000007
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_4_DEF                      (0x00000004)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_4_HSH                      (0x0318E480)

  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_8_OFF                      (15)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_8_WID                      ( 3)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_8_MSK                      (0x00038000)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_8_MIN                      (0)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_8_MAX                      (7) // 0x00000007
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_8_DEF                      (0x00000000)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_8_HSH                      (0x031EE480)

  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_9_OFF                      (18)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_9_WID                      ( 3)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_9_MSK                      (0x001C0000)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_9_MIN                      (0)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_9_MAX                      (7) // 0x00000007
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_9_DEF                      (0x00000001)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_9_HSH                      (0x0324E480)

  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_10_OFF                     (21)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_10_WID                     ( 3)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_10_MSK                     (0x00E00000)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_10_MIN                     (0)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_10_MAX                     (7) // 0x00000007
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_10_DEF                     (0x00000002)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_10_HSH                     (0x032AE480)

  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_11_OFF                     (24)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_11_WID                     ( 3)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_11_MSK                     (0x07000000)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_11_MIN                     (0)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_11_MAX                     (7) // 0x00000007
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_11_DEF                     (0x00000003)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_11_HSH                     (0x0330E480)

  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_12_OFF                     (27)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_12_WID                     ( 3)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_12_MSK                     (0x38000000)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_12_MIN                     (0)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_12_MAX                     (7) // 0x00000007
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_12_DEF                     (0x00000004)
  #define MC0_CH0_CR_SCH0_BIT_DESWIZZLE_Bit_12_HSH                     (0x0336E480)

#define MC0_CH0_CR_SCH1_BIT_DESWIZZLE_REG                              (0x0000E484)
//Duplicate of MC0_CH0_CR_SCH0_BIT_DESWIZZLE_REG

#define MC0_CH0_CR_SC_PBR_REG                                          (0x0000E488)
#define MC0_CH0_CR_SC_PBR_DEF                                          (0x0A0009CC)
#define MC0_CH0_CR_SC_PBR_VOLATILE_BITFIELDS_MSK                       (0x00000000)

  #define MC0_CH0_CR_SC_PBR_PBR_OOO_Dis_OFF                            ( 1)
  #define MC0_CH0_CR_SC_PBR_PBR_OOO_Dis_WID                            ( 1)
  #define MC0_CH0_CR_SC_PBR_PBR_OOO_Dis_MSK                            (0x00000002)
  #define MC0_CH0_CR_SC_PBR_PBR_OOO_Dis_MIN                            (0)
  #define MC0_CH0_CR_SC_PBR_PBR_OOO_Dis_MAX                            (1) // 0x00000001
  #define MC0_CH0_CR_SC_PBR_PBR_OOO_Dis_DEF                            (0x00000000)
  #define MC0_CH0_CR_SC_PBR_PBR_OOO_Dis_HSH                            (0x0102E488)

  #define MC0_CH0_CR_SC_PBR_ECS_REFab_Enable_OFF                       ( 2)
  #define MC0_CH0_CR_SC_PBR_ECS_REFab_Enable_WID                       ( 1)
  #define MC0_CH0_CR_SC_PBR_ECS_REFab_Enable_MSK                       (0x00000004)
  #define MC0_CH0_CR_SC_PBR_ECS_REFab_Enable_MIN                       (0)
  #define MC0_CH0_CR_SC_PBR_ECS_REFab_Enable_MAX                       (1) // 0x00000001
  #define MC0_CH0_CR_SC_PBR_ECS_REFab_Enable_DEF                       (0x00000001)
  #define MC0_CH0_CR_SC_PBR_ECS_REFab_Enable_HSH                       (0x0104E488)

  #define MC0_CH0_CR_SC_PBR_PBR_Disable_on_hot_OFF                     ( 3)
  #define MC0_CH0_CR_SC_PBR_PBR_Disable_on_hot_WID                     ( 1)
  #define MC0_CH0_CR_SC_PBR_PBR_Disable_on_hot_MSK                     (0x00000008)
  #define MC0_CH0_CR_SC_PBR_PBR_Disable_on_hot_MIN                     (0)
  #define MC0_CH0_CR_SC_PBR_PBR_Disable_on_hot_MAX                     (1) // 0x00000001
  #define MC0_CH0_CR_SC_PBR_PBR_Disable_on_hot_DEF                     (0x00000001)
  #define MC0_CH0_CR_SC_PBR_PBR_Disable_on_hot_HSH                     (0x0106E488)

  #define MC0_CH0_CR_SC_PBR_ECS_REFab_Period_OFF                       ( 4)
  #define MC0_CH0_CR_SC_PBR_ECS_REFab_Period_WID                       (10)
  #define MC0_CH0_CR_SC_PBR_ECS_REFab_Period_MSK                       (0x00003FF0)
  #define MC0_CH0_CR_SC_PBR_ECS_REFab_Period_MIN                       (0)
  #define MC0_CH0_CR_SC_PBR_ECS_REFab_Period_MAX                       (1023) // 0x000003FF
  #define MC0_CH0_CR_SC_PBR_ECS_REFab_Period_DEF                       (0x0000009C)
  #define MC0_CH0_CR_SC_PBR_ECS_REFab_Period_HSH                       (0x0A08E488)

  #define MC0_CH0_CR_SC_PBR_PBR_extended_disable_while_DQS_OSC_OFF     (25)
  #define MC0_CH0_CR_SC_PBR_PBR_extended_disable_while_DQS_OSC_WID     ( 1)
  #define MC0_CH0_CR_SC_PBR_PBR_extended_disable_while_DQS_OSC_MSK     (0x02000000)
  #define MC0_CH0_CR_SC_PBR_PBR_extended_disable_while_DQS_OSC_MIN     (0)
  #define MC0_CH0_CR_SC_PBR_PBR_extended_disable_while_DQS_OSC_MAX     (1) // 0x00000001
  #define MC0_CH0_CR_SC_PBR_PBR_extended_disable_while_DQS_OSC_DEF     (0x00000001)
  #define MC0_CH0_CR_SC_PBR_PBR_extended_disable_while_DQS_OSC_HSH     (0x0132E488)

  #define MC0_CH0_CR_SC_PBR_Disable_NoPanic_PBR_Cbit_OFF               (26)
  #define MC0_CH0_CR_SC_PBR_Disable_NoPanic_PBR_Cbit_WID               ( 1)
  #define MC0_CH0_CR_SC_PBR_Disable_NoPanic_PBR_Cbit_MSK               (0x04000000)
  #define MC0_CH0_CR_SC_PBR_Disable_NoPanic_PBR_Cbit_MIN               (0)
  #define MC0_CH0_CR_SC_PBR_Disable_NoPanic_PBR_Cbit_MAX               (1) // 0x00000001
  #define MC0_CH0_CR_SC_PBR_Disable_NoPanic_PBR_Cbit_DEF               (0x00000000)
  #define MC0_CH0_CR_SC_PBR_Disable_NoPanic_PBR_Cbit_HSH               (0x0134E488)

  #define MC0_CH0_CR_SC_PBR_PBR_Idle_Bank_Condition_WR_dis_OFF         (27)
  #define MC0_CH0_CR_SC_PBR_PBR_Idle_Bank_Condition_WR_dis_WID         ( 1)
  #define MC0_CH0_CR_SC_PBR_PBR_Idle_Bank_Condition_WR_dis_MSK         (0x08000000)
  #define MC0_CH0_CR_SC_PBR_PBR_Idle_Bank_Condition_WR_dis_MIN         (0)
  #define MC0_CH0_CR_SC_PBR_PBR_Idle_Bank_Condition_WR_dis_MAX         (1) // 0x00000001
  #define MC0_CH0_CR_SC_PBR_PBR_Idle_Bank_Condition_WR_dis_DEF         (0x00000001)
  #define MC0_CH0_CR_SC_PBR_PBR_Idle_Bank_Condition_WR_dis_HSH         (0x0136E488)

#define MC0_CH0_CR_DVFSQ_CTL_REG                                       (0x0000E48C)
#define MC0_CH0_CR_DVFSQ_CTL_DEF                                       (0x00000000)
#define MC0_CH0_CR_DVFSQ_CTL_VOLATILE_BITFIELDS_MSK                    (0x00000000)

  #define MC0_CH0_CR_DVFSQ_CTL_tVRCG_ENABLE_OFF                        ( 0)
  #define MC0_CH0_CR_DVFSQ_CTL_tVRCG_ENABLE_WID                        (11)
  #define MC0_CH0_CR_DVFSQ_CTL_tVRCG_ENABLE_MSK                        (0x000007FF)
  #define MC0_CH0_CR_DVFSQ_CTL_tVRCG_ENABLE_MIN                        (0)
  #define MC0_CH0_CR_DVFSQ_CTL_tVRCG_ENABLE_MAX                        (2047) // 0x000007FF
  #define MC0_CH0_CR_DVFSQ_CTL_tVRCG_ENABLE_DEF                        (0x00000000)
  #define MC0_CH0_CR_DVFSQ_CTL_tVRCG_ENABLE_HSH                        (0x0B00E48C)

  #define MC0_CH0_CR_DVFSQ_CTL_tVRCG_DISABLE_OFF                       (11)
  #define MC0_CH0_CR_DVFSQ_CTL_tVRCG_DISABLE_WID                       (11)
  #define MC0_CH0_CR_DVFSQ_CTL_tVRCG_DISABLE_MSK                       (0x003FF800)
  #define MC0_CH0_CR_DVFSQ_CTL_tVRCG_DISABLE_MIN                       (0)
  #define MC0_CH0_CR_DVFSQ_CTL_tVRCG_DISABLE_MAX                       (2047) // 0x000007FF
  #define MC0_CH0_CR_DVFSQ_CTL_tVRCG_DISABLE_DEF                       (0x00000000)
  #define MC0_CH0_CR_DVFSQ_CTL_tVRCG_DISABLE_HSH                       (0x0B16E48C)

  #define MC0_CH0_CR_DVFSQ_CTL_MRS_tFC_pointer_OFF                     (22)
  #define MC0_CH0_CR_DVFSQ_CTL_MRS_tFC_pointer_WID                     ( 4)
  #define MC0_CH0_CR_DVFSQ_CTL_MRS_tFC_pointer_MSK                     (0x03C00000)
  #define MC0_CH0_CR_DVFSQ_CTL_MRS_tFC_pointer_MIN                     (0)
  #define MC0_CH0_CR_DVFSQ_CTL_MRS_tFC_pointer_MAX                     (15) // 0x0000000F
  #define MC0_CH0_CR_DVFSQ_CTL_MRS_tFC_pointer_DEF                     (0x00000000)
  #define MC0_CH0_CR_DVFSQ_CTL_MRS_tFC_pointer_HSH                     (0x042CE48C)

  #define MC0_CH0_CR_DVFSQ_CTL_DVFSQ_OFF                               (31)
  #define MC0_CH0_CR_DVFSQ_CTL_DVFSQ_WID                               ( 1)
  #define MC0_CH0_CR_DVFSQ_CTL_DVFSQ_MSK                               (0x80000000)
  #define MC0_CH0_CR_DVFSQ_CTL_DVFSQ_MIN                               (0)
  #define MC0_CH0_CR_DVFSQ_CTL_DVFSQ_MAX                               (1) // 0x00000001
  #define MC0_CH0_CR_DVFSQ_CTL_DVFSQ_DEF                               (0x00000000)
  #define MC0_CH0_CR_DVFSQ_CTL_DVFSQ_HSH                               (0x013EE48C)

#define MC0_CH0_CR_TC_MR_REG                                           (0x0000E490)
#define MC0_CH0_CR_TC_MR_DEF                                           (0x00001F10)
#define MC0_CH0_CR_TC_MR_VOLATILE_BITFIELDS_MSK                        (0x00000000)

  #define MC0_CH0_CR_TC_MR_tMRR_OFF                                    ( 0)
  #define MC0_CH0_CR_TC_MR_tMRR_WID                                    ( 8)
  #define MC0_CH0_CR_TC_MR_tMRR_MSK                                    (0x000000FF)
  #define MC0_CH0_CR_TC_MR_tMRR_MIN                                    (0)
  #define MC0_CH0_CR_TC_MR_tMRR_MAX                                    (255) // 0x000000FF
  #define MC0_CH0_CR_TC_MR_tMRR_DEF                                    (0x00000010)
  #define MC0_CH0_CR_TC_MR_tMRR_HSH                                    (0x0800E490)

  #define MC0_CH0_CR_TC_MR_PREMRR_OFF                                  ( 8)
  #define MC0_CH0_CR_TC_MR_PREMRR_WID                                  ( 8)
  #define MC0_CH0_CR_TC_MR_PREMRR_MSK                                  (0x0000FF00)
  #define MC0_CH0_CR_TC_MR_PREMRR_MIN                                  (0)
  #define MC0_CH0_CR_TC_MR_PREMRR_MAX                                  (255) // 0x000000FF
  #define MC0_CH0_CR_TC_MR_PREMRR_DEF                                  (0x0000001F)
  #define MC0_CH0_CR_TC_MR_PREMRR_HSH                                  (0x0810E490)

  #define MC0_CH0_CR_TC_MR_tMRD_OFF                                    (16)
  #define MC0_CH0_CR_TC_MR_tMRD_WID                                    ( 7)
  #define MC0_CH0_CR_TC_MR_tMRD_MSK                                    (0x007F0000)
  #define MC0_CH0_CR_TC_MR_tMRD_MIN                                    (0)
  #define MC0_CH0_CR_TC_MR_tMRD_MAX                                    (127) // 0x0000007F
  #define MC0_CH0_CR_TC_MR_tMRD_DEF                                    (0x00000000)
  #define MC0_CH0_CR_TC_MR_tMRD_HSH                                    (0x0720E490)

#define MC0_CH0_CR_SCH1_DDR_MR_RESULT_HIGH_REG                         (0x0000E498)
//Duplicate of MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_REG

#define MC0_CH0_CR_SCH1_DDR_MR_RESULT_ECC_REG                          (0x0000E49C)
//Duplicate of MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_REG

#define MC0_CH0_CR_TC_RFTP_REG                                         (0x0000E4A0)
#define MC0_CH0_CR_TC_RFTP_DEF                                         (0x00C03C0002D01004)
#define MC0_CH0_CR_TC_RFTP_VOLATILE_BITFIELDS_MSK                      (0x0000000000000000)

  #define MC0_CH0_CR_TC_RFTP_tREFI_OFF                                 ( 0)
  #define MC0_CH0_CR_TC_RFTP_tREFI_WID                                 (18)
  #define MC0_CH0_CR_TC_RFTP_tREFI_MSK                                 (0x0003FFFF)
  #define MC0_CH0_CR_TC_RFTP_tREFI_MIN                                 (0)
  #define MC0_CH0_CR_TC_RFTP_tREFI_MAX                                 (131071) // 0x0001FFFF
  #define MC0_CH0_CR_TC_RFTP_tREFI_DEF                                 (0x00001004)
  #define MC0_CH0_CR_TC_RFTP_tREFI_HSH                                 (0x5200E4A0)

  #define MC0_CH0_CR_TC_RFTP_tRFC_OFF                                  (18)
  #define MC0_CH0_CR_TC_RFTP_tRFC_WID                                  (13)
  #define MC0_CH0_CR_TC_RFTP_tRFC_MSK                                  (0x7FFC0000)
  #define MC0_CH0_CR_TC_RFTP_tRFC_MIN                                  (0)
  #define MC0_CH0_CR_TC_RFTP_tRFC_MAX                                  (4095) // 0x00000FFF
  #define MC0_CH0_CR_TC_RFTP_tRFC_DEF                                  (0x000000B4)
  #define MC0_CH0_CR_TC_RFTP_tRFC_HSH                                  (0x4D24E4A0)

  #define MC0_CH0_CR_TC_RFTP_tRFCpb_OFF                                (40)
  #define MC0_CH0_CR_TC_RFTP_tRFCpb_WID                                (11)
  #define MC0_CH0_CR_TC_RFTP_tRFCpb_MSK                                (0x0007FF0000000000ULL)
  #define MC0_CH0_CR_TC_RFTP_tRFCpb_MIN                                (0)
  #define MC0_CH0_CR_TC_RFTP_tRFCpb_MAX                                (1023) // 0x000003FF
  #define MC0_CH0_CR_TC_RFTP_tRFCpb_DEF                                (0x0000003C)
  #define MC0_CH0_CR_TC_RFTP_tRFCpb_HSH                                (0x4B50E4A0)

  #define MC0_CH0_CR_TC_RFTP_tREFSBRD_OFF                              (51)
  #define MC0_CH0_CR_TC_RFTP_tREFSBRD_WID                              ( 8)
  #define MC0_CH0_CR_TC_RFTP_tREFSBRD_MSK                              (0x07F8000000000000ULL)
  #define MC0_CH0_CR_TC_RFTP_tREFSBRD_MIN                              (0)
  #define MC0_CH0_CR_TC_RFTP_tREFSBRD_MAX                              (255) // 0x000000FF
  #define MC0_CH0_CR_TC_RFTP_tREFSBRD_DEF                              (0x00000018)
  #define MC0_CH0_CR_TC_RFTP_tREFSBRD_HSH                              (0x4866E4A0)

#define MC0_CH0_CR_MRS_CR_MAILBOX_REG                                  (0x0000E4A8)
#define MC0_CH0_CR_MRS_CR_MAILBOX_DEF                                  (0x0000000000000000)
#define MC0_CH0_CR_MRS_CR_MAILBOX_VOLATILE_BITFIELDS_MSK               (0xFFFFFFFF00000001)

  #define MC0_CH0_CR_MRS_CR_MAILBOX_Run_OFF                            ( 0)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Run_WID                            ( 1)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Run_MSK                            (0x00000001)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Run_MIN                            (0)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Run_MAX                            (1) // 0x00000001
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Run_DEF                            (0x00000000)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Run_HSH                            (0x4180E4A8)

  #define MC0_CH0_CR_MRS_CR_MAILBOX_Opcode_OFF                         ( 1)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Opcode_WID                         ( 1)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Opcode_MSK                         (0x00000002)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Opcode_MIN                         (0)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Opcode_MAX                         (1) // 0x00000001
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Opcode_DEF                         (0x00000000)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Opcode_HSH                         (0x4102E4A8)

  #define MC0_CH0_CR_MRS_CR_MAILBOX_Sel_Type_OFF                       ( 2)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Sel_Type_WID                       ( 2)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Sel_Type_MSK                       (0x0000000C)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Sel_Type_MIN                       (0)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Sel_Type_MAX                       (3) // 0x00000003
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Sel_Type_DEF                       (0x00000000)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Sel_Type_HSH                       (0x4204E4A8)

  #define MC0_CH0_CR_MRS_CR_MAILBOX_Sel_Index_OFF                      ( 4)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Sel_Index_WID                      ( 8)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Sel_Index_MSK                      (0x00000FF0)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Sel_Index_MIN                      (0)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Sel_Index_MAX                      (255) // 0x000000FF
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Sel_Index_DEF                      (0x00000000)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_Sel_Index_HSH                      (0x4808E4A8)

  #define MC0_CH0_CR_MRS_CR_MAILBOX_CR_Data_OFF                        (32)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_CR_Data_WID                        (32)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_CR_Data_MSK                        (0xFFFFFFFF00000000ULL)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_CR_Data_MIN                        (0)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_CR_Data_MAX                        (4294967295) // 0xFFFFFFFF
  #define MC0_CH0_CR_MRS_CR_MAILBOX_CR_Data_DEF                        (0x00000000)
  #define MC0_CH0_CR_MRS_CR_MAILBOX_CR_Data_HSH                        (0x60C0E4A8)

#define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_REG                             (0x0000E4B8)
#define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_DEF                             (0x000000E4)
#define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_VOLATILE_BITFIELDS_MSK          (0x00000000)

  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_0_OFF                    ( 0)
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_0_WID                    ( 2)
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_0_MSK                    (0x00000003)
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_0_MIN                    (0)
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_0_MAX                    (3) // 0x00000003
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_0_DEF                    (0x00000000)
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_0_HSH                    (0x0200E4B8)

  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_1_OFF                    ( 2)
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_1_WID                    ( 2)
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_1_MSK                    (0x0000000C)
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_1_MIN                    (0)
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_1_MAX                    (3) // 0x00000003
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_1_DEF                    (0x00000001)
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_1_HSH                    (0x0204E4B8)

  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_2_OFF                    ( 4)
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_2_WID                    ( 2)
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_2_MSK                    (0x00000030)
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_2_MIN                    (0)
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_2_MAX                    (3) // 0x00000003
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_2_DEF                    (0x00000002)
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_2_HSH                    (0x0208E4B8)

  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_3_OFF                    ( 6)
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_3_WID                    ( 2)
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_3_MSK                    (0x000000C0)
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_3_MIN                    (0)
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_3_MAX                    (3) // 0x00000003
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_3_DEF                    (0x00000003)
  #define MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_Byte_3_HSH                    (0x020CE4B8)

#define MC0_CH0_CR_SCH1_BYTE_DESWIZZLE_REG                             (0x0000E4BC)
//Duplicate of MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_REG

#define MC0_CH0_CR_TC_SREXITTP_REG                                     (0x0000E4C0)
#define MC0_CH0_CR_TC_SREXITTP_DEF                                     (0x1000205000000000)
#define MC0_CH0_CR_TC_SREXITTP_VOLATILE_BITFIELDS_MSK                  (0x0000000000000000)

  #define MC0_CH0_CR_TC_SREXITTP_tXSR_OFF                              ( 0)
  #define MC0_CH0_CR_TC_SREXITTP_tXSR_WID                              (13)
  #define MC0_CH0_CR_TC_SREXITTP_tXSR_MSK                              (0x00001FFF)
  #define MC0_CH0_CR_TC_SREXITTP_tXSR_MIN                              (0)
  #define MC0_CH0_CR_TC_SREXITTP_tXSR_MAX                              (2576) // 0x00000A10
  #define MC0_CH0_CR_TC_SREXITTP_tXSR_DEF                              (0x00000000)
  #define MC0_CH0_CR_TC_SREXITTP_tXSR_HSH                              (0x4D00E4C0)

  #define MC0_CH0_CR_TC_SREXITTP_tXSR_DS_OFF                           (13)
  #define MC0_CH0_CR_TC_SREXITTP_tXSR_DS_WID                           (15)
  #define MC0_CH0_CR_TC_SREXITTP_tXSR_DS_MSK                           (0x0FFFE000)
  #define MC0_CH0_CR_TC_SREXITTP_tXSR_DS_MIN                           (0)
  #define MC0_CH0_CR_TC_SREXITTP_tXSR_DS_MAX                           (1018) // 0x000003FA
  #define MC0_CH0_CR_TC_SREXITTP_tXSR_DS_DEF                           (0x00000000)
  #define MC0_CH0_CR_TC_SREXITTP_tXSR_DS_HSH                           (0x4F1AE4C0)

  #define MC0_CH0_CR_TC_SREXITTP_lpmode4_srx_delay_OFF                 (32)
  #define MC0_CH0_CR_TC_SREXITTP_lpmode4_srx_delay_WID                 ( 8)
  #define MC0_CH0_CR_TC_SREXITTP_lpmode4_srx_delay_MSK                 (0x000000FF00000000ULL)
  #define MC0_CH0_CR_TC_SREXITTP_lpmode4_srx_delay_MIN                 (0)
  #define MC0_CH0_CR_TC_SREXITTP_lpmode4_srx_delay_MAX                 (255) // 0x000000FF
  #define MC0_CH0_CR_TC_SREXITTP_lpmode4_srx_delay_DEF                 (0x00000050)
  #define MC0_CH0_CR_TC_SREXITTP_lpmode4_srx_delay_HSH                 (0x4840E4C0)

  #define MC0_CH0_CR_TC_SREXITTP_tSR_OFF                               (40)
  #define MC0_CH0_CR_TC_SREXITTP_tSR_WID                               ( 7)
  #define MC0_CH0_CR_TC_SREXITTP_tSR_MSK                               (0x00007F0000000000ULL)
  #define MC0_CH0_CR_TC_SREXITTP_tSR_MIN                               (0)
  #define MC0_CH0_CR_TC_SREXITTP_tSR_MAX                               (127) // 0x0000007F
  #define MC0_CH0_CR_TC_SREXITTP_tSR_DEF                               (0x00000020)
  #define MC0_CH0_CR_TC_SREXITTP_tSR_HSH                               (0x4750E4C0)

  #define MC0_CH0_CR_TC_SREXITTP_tXSDLL_OFF                            (51)
  #define MC0_CH0_CR_TC_SREXITTP_tXSDLL_WID                            (13)
  #define MC0_CH0_CR_TC_SREXITTP_tXSDLL_MSK                            (0xFFF8000000000000ULL)
  #define MC0_CH0_CR_TC_SREXITTP_tXSDLL_MIN                            (0)
  #define MC0_CH0_CR_TC_SREXITTP_tXSDLL_MAX                            (4095) // 0x00000FFF
  #define MC0_CH0_CR_TC_SREXITTP_tXSDLL_DEF                            (0x00000200)
  #define MC0_CH0_CR_TC_SREXITTP_tXSDLL_HSH                            (0x4D66E4C0)

#define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_REG                          (0x0000E4C8)
#define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_DEF                          (0x00080100)
#define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_VOLATILE_BITFIELDS_MSK       (0x00000000)

  #define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_RETRAINING_PERIOD_OFF      ( 0)
  #define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_RETRAINING_PERIOD_WID      (16)
  #define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_RETRAINING_PERIOD_MSK      (0x0000FFFF)
  #define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_RETRAINING_PERIOD_MIN      (0)
  #define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_RETRAINING_PERIOD_MAX      (65535) // 0x0000FFFF
  #define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_RETRAINING_PERIOD_DEF      (0x00000100)
  #define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_RETRAINING_PERIOD_HSH      (0x1000E4C8)

  #define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_EN_RD_RETRAINING_OFF       (18)
  #define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_EN_RD_RETRAINING_WID       ( 1)
  #define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_EN_RD_RETRAINING_MSK       (0x00040000)
  #define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_EN_RD_RETRAINING_MIN       (0)
  #define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_EN_RD_RETRAINING_MAX       (1) // 0x00000001
  #define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_EN_RD_RETRAINING_DEF       (0x00000000)
  #define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_EN_RD_RETRAINING_HSH       (0x0124E4C8)

  #define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_EN_WR_RETRAINING_OFF       (19)
  #define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_EN_WR_RETRAINING_WID       ( 1)
  #define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_EN_WR_RETRAINING_MSK       (0x00080000)
  #define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_EN_WR_RETRAINING_MIN       (0)
  #define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_EN_WR_RETRAINING_MAX       (1) // 0x00000001
  #define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_EN_WR_RETRAINING_DEF       (0x00000001)
  #define MC0_CH0_CR_RETRAINING_OSCL_PARAMS_EN_WR_RETRAINING_HSH       (0x0126E4C8)

#define MC0_CH0_CR_MRH_GENERIC_COMMAND_REG                             (0x0000E4CC)
#define MC0_CH0_CR_MRH_GENERIC_COMMAND_DEF                             (0x00000000)
#define MC0_CH0_CR_MRH_GENERIC_COMMAND_VOLATILE_BITFIELDS_MSK          (0x00000000)

  #define MC0_CH0_CR_MRH_GENERIC_COMMAND_ca_bus_OFF                    ( 0)
  #define MC0_CH0_CR_MRH_GENERIC_COMMAND_ca_bus_WID                    (28)
  #define MC0_CH0_CR_MRH_GENERIC_COMMAND_ca_bus_MSK                    (0x0FFFFFFF)
  #define MC0_CH0_CR_MRH_GENERIC_COMMAND_ca_bus_MIN                    (0)
  #define MC0_CH0_CR_MRH_GENERIC_COMMAND_ca_bus_MAX                    (268435455) // 0x0FFFFFFF
  #define MC0_CH0_CR_MRH_GENERIC_COMMAND_ca_bus_DEF                    (0x00000000)
  #define MC0_CH0_CR_MRH_GENERIC_COMMAND_ca_bus_HSH                    (0x1C00E4CC)

  #define MC0_CH0_CR_MRH_GENERIC_COMMAND_Generic_MRH_override_OFF      (28)
  #define MC0_CH0_CR_MRH_GENERIC_COMMAND_Generic_MRH_override_WID      ( 1)
  #define MC0_CH0_CR_MRH_GENERIC_COMMAND_Generic_MRH_override_MSK      (0x10000000)
  #define MC0_CH0_CR_MRH_GENERIC_COMMAND_Generic_MRH_override_MIN      (0)
  #define MC0_CH0_CR_MRH_GENERIC_COMMAND_Generic_MRH_override_MAX      (1) // 0x00000001
  #define MC0_CH0_CR_MRH_GENERIC_COMMAND_Generic_MRH_override_DEF      (0x00000000)
  #define MC0_CH0_CR_MRH_GENERIC_COMMAND_Generic_MRH_override_HSH      (0x0138E4CC)

  #define MC0_CH0_CR_MRH_GENERIC_COMMAND_two_cyc_command_OFF           (29)
  #define MC0_CH0_CR_MRH_GENERIC_COMMAND_two_cyc_command_WID           ( 1)
  #define MC0_CH0_CR_MRH_GENERIC_COMMAND_two_cyc_command_MSK           (0x20000000)
  #define MC0_CH0_CR_MRH_GENERIC_COMMAND_two_cyc_command_MIN           (0)
  #define MC0_CH0_CR_MRH_GENERIC_COMMAND_two_cyc_command_MAX           (1) // 0x00000001
  #define MC0_CH0_CR_MRH_GENERIC_COMMAND_two_cyc_command_DEF           (0x00000000)
  #define MC0_CH0_CR_MRH_GENERIC_COMMAND_two_cyc_command_HSH           (0x013AE4CC)

#define MC0_CH0_CR_TC_RETRAINING_OSCL_REG                              (0x0000E4D0)
#define MC0_CH0_CR_TC_RETRAINING_OSCL_DEF                              (0x03F03F56)
#define MC0_CH0_CR_TC_RETRAINING_OSCL_VOLATILE_BITFIELDS_MSK           (0x00000000)

  #define MC0_CH0_CR_TC_RETRAINING_OSCL_tOSCO_OFF                      ( 0)
  #define MC0_CH0_CR_TC_RETRAINING_OSCL_tOSCO_WID                      ( 8)
  #define MC0_CH0_CR_TC_RETRAINING_OSCL_tOSCO_MSK                      (0x000000FF)
  #define MC0_CH0_CR_TC_RETRAINING_OSCL_tOSCO_MIN                      (0)
  #define MC0_CH0_CR_TC_RETRAINING_OSCL_tOSCO_MAX                      (255) // 0x000000FF
  #define MC0_CH0_CR_TC_RETRAINING_OSCL_tOSCO_DEF                      (0x00000056)
  #define MC0_CH0_CR_TC_RETRAINING_OSCL_tOSCO_HSH                      (0x0800E4D0)

  #define MC0_CH0_CR_TC_RETRAINING_OSCL_WR_OSCL_RUNTIME_OFF            ( 8)
  #define MC0_CH0_CR_TC_RETRAINING_OSCL_WR_OSCL_RUNTIME_WID            (12)
  #define MC0_CH0_CR_TC_RETRAINING_OSCL_WR_OSCL_RUNTIME_MSK            (0x000FFF00)
  #define MC0_CH0_CR_TC_RETRAINING_OSCL_WR_OSCL_RUNTIME_MIN            (0)
  #define MC0_CH0_CR_TC_RETRAINING_OSCL_WR_OSCL_RUNTIME_MAX            (4095) // 0x00000FFF
  #define MC0_CH0_CR_TC_RETRAINING_OSCL_WR_OSCL_RUNTIME_DEF            (0x0000003F)
  #define MC0_CH0_CR_TC_RETRAINING_OSCL_WR_OSCL_RUNTIME_HSH            (0x0C10E4D0)

  #define MC0_CH0_CR_TC_RETRAINING_OSCL_RD_OSCL_RUNTIME_OFF            (20)
  #define MC0_CH0_CR_TC_RETRAINING_OSCL_RD_OSCL_RUNTIME_WID            (12)
  #define MC0_CH0_CR_TC_RETRAINING_OSCL_RD_OSCL_RUNTIME_MSK            (0xFFF00000)
  #define MC0_CH0_CR_TC_RETRAINING_OSCL_RD_OSCL_RUNTIME_MIN            (0)
  #define MC0_CH0_CR_TC_RETRAINING_OSCL_RD_OSCL_RUNTIME_MAX            (4095) // 0x00000FFF
  #define MC0_CH0_CR_TC_RETRAINING_OSCL_RD_OSCL_RUNTIME_DEF            (0x0000003F)
  #define MC0_CH0_CR_TC_RETRAINING_OSCL_RD_OSCL_RUNTIME_HSH            (0x0C28E4D0)

#define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_REG                             (0x0000E4D4)
#define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_DEF                             (0x00000000)
#define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_VOLATILE_BITFIELDS_MSK          (0xF80FFFFF)

  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_RD_RETRAINING_OFF       ( 0)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_RD_RETRAINING_WID       ( 4)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_RD_RETRAINING_MSK       (0x0000000F)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_RD_RETRAINING_MIN       (0)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_RD_RETRAINING_MAX       (15) // 0x0000000F
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_RD_RETRAINING_DEF       (0x00000000)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_RD_RETRAINING_HSH       (0x0480E4D4)

  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_WR_RETRAINING_OFF       ( 4)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_WR_RETRAINING_WID       ( 4)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_WR_RETRAINING_MSK       (0x000000F0)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_WR_RETRAINING_MIN       (0)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_WR_RETRAINING_MAX       (15) // 0x0000000F
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_WR_RETRAINING_DEF       (0x00000000)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_WR_RETRAINING_HSH       (0x0488E4D4)

  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_TEMP_READ_OFF           ( 8)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_TEMP_READ_WID           ( 4)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_TEMP_READ_MSK           (0x00000F00)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_TEMP_READ_MIN           (0)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_TEMP_READ_MAX           (15) // 0x0000000F
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_TEMP_READ_DEF           (0x00000000)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_TEMP_READ_HSH           (0x0490E4D4)

  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_ZQ_CALIBRATION_OFF      (12)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_ZQ_CALIBRATION_WID      ( 4)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_ZQ_CALIBRATION_MSK      (0x0000F000)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_ZQ_CALIBRATION_MIN      (0)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_ZQ_CALIBRATION_MAX      (15) // 0x0000000F
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_ZQ_CALIBRATION_DEF      (0x00000000)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_ZQ_CALIBRATION_HSH      (0x0498E4D4)

  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_REFRESH_OFF             (16)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_REFRESH_WID             ( 4)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_REFRESH_MSK             (0x000F0000)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_REFRESH_MIN             (0)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_REFRESH_MAX             (15) // 0x0000000F
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_REFRESH_DEF             (0x00000000)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_FORCE_REFRESH_HSH             (0x04A0E4D4)

  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_RD_RETRAINING_ABORTED_OFF     (27)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_RD_RETRAINING_ABORTED_WID     ( 1)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_RD_RETRAINING_ABORTED_MSK     (0x08000000)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_RD_RETRAINING_ABORTED_MIN     (0)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_RD_RETRAINING_ABORTED_MAX     (1) // 0x00000001
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_RD_RETRAINING_ABORTED_DEF     (0x00000000)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_RD_RETRAINING_ABORTED_HSH     (0x01B6E4D4)

  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_WR_RETRAINING_ABORTED_OFF     (28)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_WR_RETRAINING_ABORTED_WID     ( 1)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_WR_RETRAINING_ABORTED_MSK     (0x10000000)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_WR_RETRAINING_ABORTED_MIN     (0)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_WR_RETRAINING_ABORTED_MAX     (1) // 0x00000001
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_WR_RETRAINING_ABORTED_DEF     (0x00000000)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_WR_RETRAINING_ABORTED_HSH     (0x01B8E4D4)

  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_TEMP_READ_ABORTED_OFF         (29)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_TEMP_READ_ABORTED_WID         ( 1)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_TEMP_READ_ABORTED_MSK         (0x20000000)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_TEMP_READ_ABORTED_MIN         (0)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_TEMP_READ_ABORTED_MAX         (1) // 0x00000001
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_TEMP_READ_ABORTED_DEF         (0x00000000)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_TEMP_READ_ABORTED_HSH         (0x01BAE4D4)

  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_ZQ_CALIBRATION_ABORTED_OFF    (30)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_ZQ_CALIBRATION_ABORTED_WID    ( 1)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_ZQ_CALIBRATION_ABORTED_MSK    (0x40000000)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_ZQ_CALIBRATION_ABORTED_MIN    (0)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_ZQ_CALIBRATION_ABORTED_MAX    (1) // 0x00000001
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_ZQ_CALIBRATION_ABORTED_DEF    (0x00000000)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_ZQ_CALIBRATION_ABORTED_HSH    (0x01BCE4D4)

  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_REFRESH_ABORTED_OFF           (31)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_REFRESH_ABORTED_WID           ( 1)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_REFRESH_ABORTED_MSK           (0x80000000)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_REFRESH_ABORTED_MIN           (0)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_REFRESH_ABORTED_MAX           (1) // 0x00000001
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_REFRESH_ABORTED_DEF           (0x00000000)
  #define MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_REFRESH_ABORTED_HSH           (0x01BEE4D4)

#define MC0_CH0_CR_WDB_RD_WR_DFX_DATA_REG                              (0x0000E4D8)
#define MC0_CH0_CR_WDB_RD_WR_DFX_DATA_DEF                              (0x0000000000000000)
#define MC0_CH0_CR_WDB_RD_WR_DFX_DATA_VOLATILE_BITFIELDS_MSK           (0xFFFFFFFFFFFFFFFF)

  #define MC0_CH0_CR_WDB_RD_WR_DFX_DATA_DATA_OFF                       ( 0)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_DATA_DATA_WID                       (64)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_DATA_DATA_MSK                       (0xFFFFFFFFFFFFFFFFULL)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_DATA_DATA_MIN                       (0)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_DATA_DATA_MAX                       (18446744073709551615ULL) // 0xFFFFFFFFFFFFFFFF
  #define MC0_CH0_CR_WDB_RD_WR_DFX_DATA_DATA_DEF                       (0x00000000)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_DATA_DATA_HSH                       (0x4080E4D8)

#define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_REG                               (0x0000E4E0)
#define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_DEF                               (0x00000000)
#define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_VOLATILE_BITFIELDS_MSK            (0x00000001)

  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_GO_OFF                          ( 0)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_GO_WID                          ( 1)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_GO_MSK                          (0x00000001)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_GO_MIN                          (0)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_GO_MAX                          (1) // 0x00000001
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_GO_DEF                          (0x00000000)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_GO_HSH                          (0x0180E4E0)

  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_Chunk_Sel_OFF                   ( 1)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_Chunk_Sel_WID                   ( 8)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_Chunk_Sel_MSK                   (0x000001FE)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_Chunk_Sel_MIN                   (0)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_Chunk_Sel_MAX                   (255) // 0x000000FF
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_Chunk_Sel_DEF                   (0x00000000)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_Chunk_Sel_HSH                   (0x0802E4E0)

  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_Op_OFF                          ( 9)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_Op_WID                          ( 1)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_Op_MSK                          (0x00000200)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_Op_MIN                          (0)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_Op_MAX                          (1) // 0x00000001
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_Op_DEF                          (0x00000000)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_Op_HSH                          (0x0112E4E0)

  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_SubCh_OFF                       (10)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_SubCh_WID                       ( 1)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_SubCh_MSK                       (0x00000400)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_SubCh_MIN                       (0)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_SubCh_MAX                       (1) // 0x00000001
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_SubCh_DEF                       (0x00000000)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_SubCh_HSH                       (0x0114E4E0)

  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_WID_OFF                         (11)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_WID_WID                         ( 7)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_WID_MSK                         (0x0003F800)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_WID_MIN                         (0)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_WID_MAX                         (127) // 0x0000007F
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_WID_DEF                         (0x00000000)
  #define MC0_CH0_CR_WDB_RD_WR_DFX_CTL_WID_HSH                         (0x0716E4E0)

#define MC0_CH0_CR_WDB_MBIST_0_REG                                     (0x0000E4E8)
#define MC0_CH0_CR_WDB_MBIST_0_DEF                                     (0x00000000)
#define MC0_CH0_CR_WDB_MBIST_0_VOLATILE_BITFIELDS_MSK                  (0x80000003)

  #define MC0_CH0_CR_WDB_MBIST_0_PASS_OFF                              ( 0)
  #define MC0_CH0_CR_WDB_MBIST_0_PASS_WID                              ( 1)
  #define MC0_CH0_CR_WDB_MBIST_0_PASS_MSK                              (0x00000001)
  #define MC0_CH0_CR_WDB_MBIST_0_PASS_MIN                              (0)
  #define MC0_CH0_CR_WDB_MBIST_0_PASS_MAX                              (1) // 0x00000001
  #define MC0_CH0_CR_WDB_MBIST_0_PASS_DEF                              (0x00000000)
  #define MC0_CH0_CR_WDB_MBIST_0_PASS_HSH                              (0x0180E4E8)

  #define MC0_CH0_CR_WDB_MBIST_0_Complete_OFF                          ( 1)
  #define MC0_CH0_CR_WDB_MBIST_0_Complete_WID                          ( 1)
  #define MC0_CH0_CR_WDB_MBIST_0_Complete_MSK                          (0x00000002)
  #define MC0_CH0_CR_WDB_MBIST_0_Complete_MIN                          (0)
  #define MC0_CH0_CR_WDB_MBIST_0_Complete_MAX                          (1) // 0x00000001
  #define MC0_CH0_CR_WDB_MBIST_0_Complete_DEF                          (0x00000000)
  #define MC0_CH0_CR_WDB_MBIST_0_Complete_HSH                          (0x0182E4E8)

  #define MC0_CH0_CR_WDB_MBIST_0_Inject_Failure_OFF                    ( 8)
  #define MC0_CH0_CR_WDB_MBIST_0_Inject_Failure_WID                    ( 1)
  #define MC0_CH0_CR_WDB_MBIST_0_Inject_Failure_MSK                    (0x00000100)
  #define MC0_CH0_CR_WDB_MBIST_0_Inject_Failure_MIN                    (0)
  #define MC0_CH0_CR_WDB_MBIST_0_Inject_Failure_MAX                    (1) // 0x00000001
  #define MC0_CH0_CR_WDB_MBIST_0_Inject_Failure_DEF                    (0x00000000)
  #define MC0_CH0_CR_WDB_MBIST_0_Inject_Failure_HSH                    (0x0110E4E8)

  #define MC0_CH0_CR_WDB_MBIST_0_RUN_BUSY_OFF                          (31)
  #define MC0_CH0_CR_WDB_MBIST_0_RUN_BUSY_WID                          ( 1)
  #define MC0_CH0_CR_WDB_MBIST_0_RUN_BUSY_MSK                          (0x80000000)
  #define MC0_CH0_CR_WDB_MBIST_0_RUN_BUSY_MIN                          (0)
  #define MC0_CH0_CR_WDB_MBIST_0_RUN_BUSY_MAX                          (1) // 0x00000001
  #define MC0_CH0_CR_WDB_MBIST_0_RUN_BUSY_DEF                          (0x00000000)
  #define MC0_CH0_CR_WDB_MBIST_0_RUN_BUSY_HSH                          (0x01BEE4E8)

#define MC0_CH0_CR_WDB_MBIST_1_REG                                     (0x0000E4EC)
//Duplicate of MC0_CH0_CR_WDB_MBIST_0_REG

#define MC0_CH0_CR_RDB_MBIST_REG                                       (0x0000E4F8)
//Duplicate of MC0_CH0_CR_WDB_MBIST_0_REG

#define MC0_CH0_CR_ECC_INJECT_COUNT_REG                                (0x0000E4FC)
#define MC0_CH0_CR_ECC_INJECT_COUNT_DEF                                (0xFFFFFFFF)
#define MC0_CH0_CR_ECC_INJECT_COUNT_VOLATILE_BITFIELDS_MSK             (0x00000000)

  #define MC0_CH0_CR_ECC_INJECT_COUNT_Count_OFF                        ( 0)
  #define MC0_CH0_CR_ECC_INJECT_COUNT_Count_WID                        (32)
  #define MC0_CH0_CR_ECC_INJECT_COUNT_Count_MSK                        (0xFFFFFFFF)
  #define MC0_CH0_CR_ECC_INJECT_COUNT_Count_MIN                        (0)
  #define MC0_CH0_CR_ECC_INJECT_COUNT_Count_MAX                        (4294967295) // 0xFFFFFFFF
  #define MC0_CH0_CR_ECC_INJECT_COUNT_Count_DEF                        (0xFFFFFFFF)
  #define MC0_CH0_CR_ECC_INJECT_COUNT_Count_HSH                        (0x2000E4FC)

#define MC0_CH0_CR_PWM_DDR_SUBCH0_RDDATA_COUNTER_REG                   (0x0000E500)
#define MC0_CH0_CR_PWM_DDR_SUBCH0_RDDATA_COUNTER_DEF                   (0x0000000000000000)
#define MC0_CH0_CR_PWM_DDR_SUBCH0_RDDATA_COUNTER_VOLATILE_BITFIELDS_MSK (0xFFFFFFFFFFFFFFFF)

  #define MC0_CH0_CR_PWM_DDR_SUBCH0_RDDATA_COUNTER_RDDATA_COUNTER_OFF  ( 0)
  #define MC0_CH0_CR_PWM_DDR_SUBCH0_RDDATA_COUNTER_RDDATA_COUNTER_WID  (64)
  #define MC0_CH0_CR_PWM_DDR_SUBCH0_RDDATA_COUNTER_RDDATA_COUNTER_MSK  (0xFFFFFFFFFFFFFFFFULL)
  #define MC0_CH0_CR_PWM_DDR_SUBCH0_RDDATA_COUNTER_RDDATA_COUNTER_MIN  (0)
  #define MC0_CH0_CR_PWM_DDR_SUBCH0_RDDATA_COUNTER_RDDATA_COUNTER_MAX  (18446744073709551615ULL) // 0xFFFFFFFFFFFFFFFF
  #define MC0_CH0_CR_PWM_DDR_SUBCH0_RDDATA_COUNTER_RDDATA_COUNTER_DEF  (0x00000000)
  #define MC0_CH0_CR_PWM_DDR_SUBCH0_RDDATA_COUNTER_RDDATA_COUNTER_HSH  (0x4080E500)

#define MC0_CH0_CR_PWM_DDR_SUBCH1_RDDATA_COUNTER_REG                   (0x0000E508)
//Duplicate of MC0_CH0_CR_PWM_DDR_SUBCH0_RDDATA_COUNTER_REG

#define MC0_CH0_CR_PWM_DDR_SUBCH0_WRDATA_COUNTER_REG                   (0x0000E510)
#define MC0_CH0_CR_PWM_DDR_SUBCH0_WRDATA_COUNTER_DEF                   (0x0000000000000000)
#define MC0_CH0_CR_PWM_DDR_SUBCH0_WRDATA_COUNTER_VOLATILE_BITFIELDS_MSK (0xFFFFFFFFFFFFFFFF)

  #define MC0_CH0_CR_PWM_DDR_SUBCH0_WRDATA_COUNTER_WRDATA_COUNTER_OFF  ( 0)
  #define MC0_CH0_CR_PWM_DDR_SUBCH0_WRDATA_COUNTER_WRDATA_COUNTER_WID  (64)
  #define MC0_CH0_CR_PWM_DDR_SUBCH0_WRDATA_COUNTER_WRDATA_COUNTER_MSK  (0xFFFFFFFFFFFFFFFFULL)
  #define MC0_CH0_CR_PWM_DDR_SUBCH0_WRDATA_COUNTER_WRDATA_COUNTER_MIN  (0)
  #define MC0_CH0_CR_PWM_DDR_SUBCH0_WRDATA_COUNTER_WRDATA_COUNTER_MAX  (18446744073709551615ULL) // 0xFFFFFFFFFFFFFFFF
  #define MC0_CH0_CR_PWM_DDR_SUBCH0_WRDATA_COUNTER_WRDATA_COUNTER_DEF  (0x00000000)
  #define MC0_CH0_CR_PWM_DDR_SUBCH0_WRDATA_COUNTER_WRDATA_COUNTER_HSH  (0x4080E510)

#define MC0_CH0_CR_PWM_DDR_SUBCH1_WRDATA_COUNTER_REG                   (0x0000E518)
//Duplicate of MC0_CH0_CR_PWM_DDR_SUBCH0_WRDATA_COUNTER_REG

#define MC0_CH0_CR_WDB_CAPTURE_STATUS_REG                              (0x0000E520)
#define MC0_CH0_CR_WDB_CAPTURE_STATUS_DEF                              (0x00000000)
#define MC0_CH0_CR_WDB_CAPTURE_STATUS_VOLATILE_BITFIELDS_MSK           (0x00007F7F)

  #define MC0_CH0_CR_WDB_CAPTURE_STATUS_WDB_SC0_Current_Capture_Pointer_OFF ( 0)
  #define MC0_CH0_CR_WDB_CAPTURE_STATUS_WDB_SC0_Current_Capture_Pointer_WID ( 7)
  #define MC0_CH0_CR_WDB_CAPTURE_STATUS_WDB_SC0_Current_Capture_Pointer_MSK (0x0000007F)
  #define MC0_CH0_CR_WDB_CAPTURE_STATUS_WDB_SC0_Current_Capture_Pointer_MIN (0)
  #define MC0_CH0_CR_WDB_CAPTURE_STATUS_WDB_SC0_Current_Capture_Pointer_MAX (127) // 0x0000007F
  #define MC0_CH0_CR_WDB_CAPTURE_STATUS_WDB_SC0_Current_Capture_Pointer_DEF (0x00000000)
  #define MC0_CH0_CR_WDB_CAPTURE_STATUS_WDB_SC0_Current_Capture_Pointer_HSH (0x0780E520)

  #define MC0_CH0_CR_WDB_CAPTURE_STATUS_WDB_SC1_Current_Capture_Pointer_OFF ( 8)
  #define MC0_CH0_CR_WDB_CAPTURE_STATUS_WDB_SC1_Current_Capture_Pointer_WID ( 7)
  #define MC0_CH0_CR_WDB_CAPTURE_STATUS_WDB_SC1_Current_Capture_Pointer_MSK (0x00007F00)
  #define MC0_CH0_CR_WDB_CAPTURE_STATUS_WDB_SC1_Current_Capture_Pointer_MIN (0)
  #define MC0_CH0_CR_WDB_CAPTURE_STATUS_WDB_SC1_Current_Capture_Pointer_MAX (127) // 0x0000007F
  #define MC0_CH0_CR_WDB_CAPTURE_STATUS_WDB_SC1_Current_Capture_Pointer_DEF (0x00000000)
  #define MC0_CH0_CR_WDB_CAPTURE_STATUS_WDB_SC1_Current_Capture_Pointer_HSH (0x0790E520)

#define MC0_CH0_CR_WDB_CAPTURE_CTL_REG                                 (0x0000E524)
#define MC0_CH0_CR_WDB_CAPTURE_CTL_DEF                                 (0x00000000)
#define MC0_CH0_CR_WDB_CAPTURE_CTL_VOLATILE_BITFIELDS_MSK              (0x00000000)

  #define MC0_CH0_CR_WDB_CAPTURE_CTL_Enable_WDB_Capture_OFF            ( 0)
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_Enable_WDB_Capture_WID            ( 1)
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_Enable_WDB_Capture_MSK            (0x00000001)
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_Enable_WDB_Capture_MIN            (0)
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_Enable_WDB_Capture_MAX            (1) // 0x00000001
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_Enable_WDB_Capture_DEF            (0x00000000)
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_Enable_WDB_Capture_HSH            (0x0100E524)

  #define MC0_CH0_CR_WDB_CAPTURE_CTL_WDB_Starting_Capture_Pointer_OFF  ( 8)
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_WDB_Starting_Capture_Pointer_WID  ( 7)
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_WDB_Starting_Capture_Pointer_MSK  (0x00007F00)
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_WDB_Starting_Capture_Pointer_MIN  (0)
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_WDB_Starting_Capture_Pointer_MAX  (127) // 0x0000007F
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_WDB_Starting_Capture_Pointer_DEF  (0x00000000)
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_WDB_Starting_Capture_Pointer_HSH  (0x0710E524)

  #define MC0_CH0_CR_WDB_CAPTURE_CTL_WDB_Ending_Capture_Pointer_OFF    (16)
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_WDB_Ending_Capture_Pointer_WID    ( 7)
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_WDB_Ending_Capture_Pointer_MSK    (0x007F0000)
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_WDB_Ending_Capture_Pointer_MIN    (0)
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_WDB_Ending_Capture_Pointer_MAX    (127) // 0x0000007F
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_WDB_Ending_Capture_Pointer_DEF    (0x00000000)
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_WDB_Ending_Capture_Pointer_HSH    (0x0720E524)

  #define MC0_CH0_CR_WDB_CAPTURE_CTL_WDB_Capture_Block_SubCh_OFF       (23)
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_WDB_Capture_Block_SubCh_WID       ( 2)
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_WDB_Capture_Block_SubCh_MSK       (0x01800000)
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_WDB_Capture_Block_SubCh_MIN       (0)
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_WDB_Capture_Block_SubCh_MAX       (3) // 0x00000003
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_WDB_Capture_Block_SubCh_DEF       (0x00000000)
  #define MC0_CH0_CR_WDB_CAPTURE_CTL_WDB_Capture_Block_SubCh_HSH       (0x022EE524)

#define MC0_CH0_CR_MRH_CONFIG_REG                                      (0x0000E528)
#define MC0_CH0_CR_MRH_CONFIG_DEF                                      (0x00000000)
#define MC0_CH0_CR_MRH_CONFIG_VOLATILE_BITFIELDS_MSK                   (0x00000000)

  #define MC0_CH0_CR_MRH_CONFIG_mrh_quiet_time_before_command_OFF      ( 0)
  #define MC0_CH0_CR_MRH_CONFIG_mrh_quiet_time_before_command_WID      (10)
  #define MC0_CH0_CR_MRH_CONFIG_mrh_quiet_time_before_command_MSK      (0x000003FF)
  #define MC0_CH0_CR_MRH_CONFIG_mrh_quiet_time_before_command_MIN      (0)
  #define MC0_CH0_CR_MRH_CONFIG_mrh_quiet_time_before_command_MAX      (1023) // 0x000003FF
  #define MC0_CH0_CR_MRH_CONFIG_mrh_quiet_time_before_command_DEF      (0x00000000)
  #define MC0_CH0_CR_MRH_CONFIG_mrh_quiet_time_before_command_HSH      (0x0A00E528)

  #define MC0_CH0_CR_MRH_CONFIG_mrh_quiet_time_after_command_OFF       (10)
  #define MC0_CH0_CR_MRH_CONFIG_mrh_quiet_time_after_command_WID       (10)
  #define MC0_CH0_CR_MRH_CONFIG_mrh_quiet_time_after_command_MSK       (0x000FFC00)
  #define MC0_CH0_CR_MRH_CONFIG_mrh_quiet_time_after_command_MIN       (0)
  #define MC0_CH0_CR_MRH_CONFIG_mrh_quiet_time_after_command_MAX       (1023) // 0x000003FF
  #define MC0_CH0_CR_MRH_CONFIG_mrh_quiet_time_after_command_DEF       (0x00000000)
  #define MC0_CH0_CR_MRH_CONFIG_mrh_quiet_time_after_command_HSH       (0x0A14E528)

#define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_REG                           (0x0000E52C)
#define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_DEF                           (0x00000000)
#define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_VOLATILE_BITFIELDS_MSK        (0x00000000)

  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_Enable_OFF                  ( 0)
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_Enable_WID                  ( 1)
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_Enable_MSK                  (0x00000001)
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_Enable_MIN                  (0)
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_Enable_MAX                  (1) // 0x00000001
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_Enable_DEF                  (0x00000000)
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_Enable_HSH                  (0x0100E52C)

  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_IncreaseRate_OFF            ( 1)
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_IncreaseRate_WID            ( 2)
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_IncreaseRate_MSK            (0x00000006)
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_IncreaseRate_MIN            (0)
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_IncreaseRate_MAX            (3) // 0x00000003
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_IncreaseRate_DEF            (0x00000000)
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_IncreaseRate_HSH            (0x0202E52C)

  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_MaxCount_OFF                ( 3)
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_MaxCount_WID                ( 1)
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_MaxCount_MSK                (0x00000008)
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_MaxCount_MIN                (0)
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_MaxCount_MAX                (1) // 0x00000001
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_MaxCount_DEF                (0x00000000)
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_MaxCount_HSH                (0x0106E52C)

  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_TemperatureAware_OFF        ( 4)
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_TemperatureAware_WID        ( 1)
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_TemperatureAware_MSK        (0x00000010)
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_TemperatureAware_MIN        (0)
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_TemperatureAware_MAX        (1) // 0x00000001
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_TemperatureAware_DEF        (0x00000000)
  #define MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_TemperatureAware_HSH        (0x0108E52C)

#define MC0_CH0_CR_RH_TRR_LFSR0_REG                                    (0x0000E530)
#define MC0_CH0_CR_RH_TRR_LFSR0_DEF                                    (0x00000000)
#define MC0_CH0_CR_RH_TRR_LFSR0_VOLATILE_BITFIELDS_MSK                 (0x00000000)

  #define MC0_CH0_CR_RH_TRR_LFSR0_LFSR_OFF                             ( 0)
  #define MC0_CH0_CR_RH_TRR_LFSR0_LFSR_WID                             (32)
  #define MC0_CH0_CR_RH_TRR_LFSR0_LFSR_MSK                             (0xFFFFFFFF)
  #define MC0_CH0_CR_RH_TRR_LFSR0_LFSR_MIN                             (0)
  #define MC0_CH0_CR_RH_TRR_LFSR0_LFSR_MAX                             (4294967295) // 0xFFFFFFFF
  #define MC0_CH0_CR_RH_TRR_LFSR0_LFSR_DEF                             (0x00000000)
  #define MC0_CH0_CR_RH_TRR_LFSR0_LFSR_HSH                             (0x2000E530)

#define MC0_CH0_CR_RH_TRR_LFSR1_REG                                    (0x0000E534)
//Duplicate of MC0_CH0_CR_RH_TRR_LFSR0_REG

#define MC0_CH0_CR_MRS_FSM_CONTROL_REG                                 (0x0000E5D0)
#define MC0_CH0_CR_MRS_FSM_CONTROL_DEF                                 (0x00000001)
#define MC0_CH0_CR_MRS_FSM_CONTROL_VOLATILE_BITFIELDS_MSK              (0x00000000)

  #define MC0_CH0_CR_MRS_FSM_CONTROL_send_preall_OFF                   ( 0)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_send_preall_WID                   ( 1)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_send_preall_MSK                   (0x00000001)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_send_preall_MIN                   (0)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_send_preall_MAX                   (1) // 0x00000001
  #define MC0_CH0_CR_MRS_FSM_CONTROL_send_preall_DEF                   (0x00000001)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_send_preall_HSH                   (0x0100E5D0)

  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_zqcl_OFF                       ( 1)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_zqcl_WID                       ( 1)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_zqcl_MSK                       (0x00000002)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_zqcl_MIN                       (0)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_zqcl_MAX                       (1) // 0x00000001
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_zqcl_DEF                       (0x00000000)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_zqcl_HSH                       (0x0102E5D0)

  #define MC0_CH0_CR_MRS_FSM_CONTROL_lpddr_switch_fsp_OFF              ( 2)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_lpddr_switch_fsp_WID              ( 1)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_lpddr_switch_fsp_MSK              (0x00000004)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_lpddr_switch_fsp_MIN              (0)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_lpddr_switch_fsp_MAX              (1) // 0x00000001
  #define MC0_CH0_CR_MRS_FSM_CONTROL_lpddr_switch_fsp_DEF              (0x00000000)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_lpddr_switch_fsp_HSH              (0x0104E5D0)

  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_dq_osc_start_OFF               ( 3)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_dq_osc_start_WID               ( 1)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_dq_osc_start_MSK               (0x00000008)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_dq_osc_start_MIN               (0)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_dq_osc_start_MAX               (1) // 0x00000001
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_dq_osc_start_DEF               (0x00000000)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_dq_osc_start_HSH               (0x0106E5D0)

  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_rd_retrain_start_OFF           ( 4)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_rd_retrain_start_WID           ( 1)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_rd_retrain_start_MSK           (0x00000010)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_rd_retrain_start_MIN           (0)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_rd_retrain_start_MAX           (1) // 0x00000001
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_rd_retrain_start_DEF           (0x00000000)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_rd_retrain_start_HSH           (0x0108E5D0)

  #define MC0_CH0_CR_MRS_FSM_CONTROL_split_transition_OFF              ( 5)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_split_transition_WID              ( 1)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_split_transition_MSK              (0x00000020)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_split_transition_MIN              (0)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_split_transition_MAX              (1) // 0x00000001
  #define MC0_CH0_CR_MRS_FSM_CONTROL_split_transition_DEF              (0x00000000)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_split_transition_HSH              (0x010AE5D0)

  #define MC0_CH0_CR_MRS_FSM_CONTROL_generic_mrs_fsm_device_loop_enable_OFF ( 6)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_generic_mrs_fsm_device_loop_enable_WID ( 1)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_generic_mrs_fsm_device_loop_enable_MSK (0x00000040)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_generic_mrs_fsm_device_loop_enable_MIN (0)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_generic_mrs_fsm_device_loop_enable_MAX (1) // 0x00000001
  #define MC0_CH0_CR_MRS_FSM_CONTROL_generic_mrs_fsm_device_loop_enable_DEF (0x00000000)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_generic_mrs_fsm_device_loop_enable_HSH (0x010CE5D0)

  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_pda_for_ecc_OFF                ( 7)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_pda_for_ecc_WID                ( 1)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_pda_for_ecc_MSK                (0x00000080)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_pda_for_ecc_MIN                (0)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_pda_for_ecc_MAX                (1) // 0x00000001
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_pda_for_ecc_DEF                (0x00000000)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_do_pda_for_ecc_HSH                (0x010EE5D0)

  #define MC0_CH0_CR_MRS_FSM_CONTROL_generic_mrs_fsm_breakpoint_address_OFF ( 8)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_generic_mrs_fsm_breakpoint_address_WID ( 7)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_generic_mrs_fsm_breakpoint_address_MSK (0x00007F00)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_generic_mrs_fsm_breakpoint_address_MIN (0)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_generic_mrs_fsm_breakpoint_address_MAX (127) // 0x0000007F
  #define MC0_CH0_CR_MRS_FSM_CONTROL_generic_mrs_fsm_breakpoint_address_DEF (0x00000000)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_generic_mrs_fsm_breakpoint_address_HSH (0x0710E5D0)

#define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_REG                           (0x0000E5D4)
#define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_DEF                           (0x00000020)
#define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_VOLATILE_BITFIELDS_MSK        (0x00000000)

  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_cbit_mrs_ignore_rh_block_OFF ( 0)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_cbit_mrs_ignore_rh_block_WID ( 1)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_cbit_mrs_ignore_rh_block_MSK (0x00000001)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_cbit_mrs_ignore_rh_block_MIN (0)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_cbit_mrs_ignore_rh_block_MAX (1) // 0x00000001
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_cbit_mrs_ignore_rh_block_DEF (0x00000000)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_cbit_mrs_ignore_rh_block_HSH (0x0100E5D4)

  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_run_delay_OFF               ( 1)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_run_delay_WID               ( 5)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_run_delay_MSK               (0x0000003E)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_run_delay_MIN               (0)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_run_delay_MAX               (31) // 0x0000001F
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_run_delay_DEF               (0x00000010)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_run_delay_HSH               (0x0502E5D4)

  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_dis_mrs_fsm_auto_start_OFF  ( 6)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_dis_mrs_fsm_auto_start_WID  ( 1)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_dis_mrs_fsm_auto_start_MSK  (0x00000040)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_dis_mrs_fsm_auto_start_MIN  (0)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_dis_mrs_fsm_auto_start_MAX  (1) // 0x00000001
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_dis_mrs_fsm_auto_start_DEF  (0x00000000)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_dis_mrs_fsm_auto_start_HSH  (0x010CE5D4)

  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_cbit_count_each_rank_OFF    ( 7)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_cbit_count_each_rank_WID    ( 1)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_cbit_count_each_rank_MSK    (0x00000080)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_cbit_count_each_rank_MIN    (0)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_cbit_count_each_rank_MAX    (1) // 0x00000001
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_cbit_count_each_rank_DEF    (0x00000000)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_cbit_count_each_rank_HSH    (0x010EE5D4)

  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_mrs_vrcg_ovrd_en_OFF        ( 8)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_mrs_vrcg_ovrd_en_WID        ( 1)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_mrs_vrcg_ovrd_en_MSK        (0x00000100)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_mrs_vrcg_ovrd_en_MIN        (0)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_mrs_vrcg_ovrd_en_MAX        (1) // 0x00000001
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_mrs_vrcg_ovrd_en_DEF        (0x00000000)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_mrs_vrcg_ovrd_en_HSH        (0x0110E5D4)

  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_mrs_vrcg_ovrd_val_OFF       ( 9)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_mrs_vrcg_ovrd_val_WID       ( 1)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_mrs_vrcg_ovrd_val_MSK       (0x00000200)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_mrs_vrcg_ovrd_val_MIN       (0)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_mrs_vrcg_ovrd_val_MAX       (1) // 0x00000001
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_mrs_vrcg_ovrd_val_DEF       (0x00000000)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_mrs_vrcg_ovrd_val_HSH       (0x0112E5D4)

  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_dvfsc_outside_sagv_dis_OFF  (10)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_dvfsc_outside_sagv_dis_WID  ( 1)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_dvfsc_outside_sagv_dis_MSK  (0x00000400)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_dvfsc_outside_sagv_dis_MIN  (0)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_dvfsc_outside_sagv_dis_MAX  (1) // 0x00000001
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_dvfsc_outside_sagv_dis_DEF  (0x00000000)
  #define MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_dvfsc_outside_sagv_dis_HSH  (0x0114E5D4)

#define MC0_CH0_CR_MRS_FSM_RUN_REG                                     (0x0000E5D8)
#define MC0_CH0_CR_MRS_FSM_RUN_DEF                                     (0x00000000)
#define MC0_CH0_CR_MRS_FSM_RUN_VOLATILE_BITFIELDS_MSK                  (0x00000001)

  #define MC0_CH0_CR_MRS_FSM_RUN_Run_OFF                               ( 0)
  #define MC0_CH0_CR_MRS_FSM_RUN_Run_WID                               ( 1)
  #define MC0_CH0_CR_MRS_FSM_RUN_Run_MSK                               (0x00000001)
  #define MC0_CH0_CR_MRS_FSM_RUN_Run_MIN                               (0)
  #define MC0_CH0_CR_MRS_FSM_RUN_Run_MAX                               (1) // 0x00000001
  #define MC0_CH0_CR_MRS_FSM_RUN_Run_DEF                               (0x00000000)
  #define MC0_CH0_CR_MRS_FSM_RUN_Run_HSH                               (0x0180E5D8)

#define MC0_CH0_CR_PL_AGENT_CFG_DTF_REG                                (0x0000E5E0)
#define MC0_CH0_CR_PL_AGENT_CFG_DTF_DEF                                (0x00002000)
#define MC0_CH0_CR_PL_AGENT_CFG_DTF_VOLATILE_BITFIELDS_MSK             (0x00000000)

  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_data_trace_mode_OFF              ( 0)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_data_trace_mode_WID              ( 2)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_data_trace_mode_MSK              (0x00000003)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_data_trace_mode_MIN              (0)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_data_trace_mode_MAX              (3) // 0x00000003
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_data_trace_mode_DEF              (0x00000000)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_data_trace_mode_HSH              (0x0200E5E0)

  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_UI_to_trace_OFF                  ( 2)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_UI_to_trace_WID                  ( 3)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_UI_to_trace_MSK                  (0x0000001C)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_UI_to_trace_MIN                  (0)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_UI_to_trace_MAX                  (7) // 0x00000007
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_UI_to_trace_DEF                  (0x00000000)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_UI_to_trace_HSH                  (0x0304E5E0)

  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_byte_to_trace_OFF                ( 5)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_byte_to_trace_WID                ( 3)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_byte_to_trace_MSK                (0x000000E0)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_byte_to_trace_MIN                (0)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_byte_to_trace_MAX                (7) // 0x00000007
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_byte_to_trace_DEF                (0x00000000)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_byte_to_trace_HSH                (0x030AE5E0)

  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_ECC_EN_OFF                       ( 8)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_ECC_EN_WID                       ( 1)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_ECC_EN_MSK                       (0x00000100)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_ECC_EN_MIN                       (0)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_ECC_EN_MAX                       (1) // 0x00000001
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_ECC_EN_DEF                       (0x00000000)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_ECC_EN_HSH                       (0x0110E5E0)

  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_ECC_BYTE_replace_OFF             ( 9)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_ECC_BYTE_replace_WID             ( 3)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_ECC_BYTE_replace_MSK             (0x00000E00)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_ECC_BYTE_replace_MIN             (0)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_ECC_BYTE_replace_MAX             (7) // 0x00000007
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_ECC_BYTE_replace_DEF             (0x00000000)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_ECC_BYTE_replace_HSH             (0x0312E5E0)

  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_trace_read_data_OFF              (12)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_trace_read_data_WID              ( 1)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_trace_read_data_MSK              (0x00001000)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_trace_read_data_MIN              (0)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_trace_read_data_MAX              (1) // 0x00000001
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_trace_read_data_DEF              (0x00000000)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_trace_read_data_HSH              (0x0118E5E0)

  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_trace_write_data_OFF             (13)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_trace_write_data_WID             ( 1)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_trace_write_data_MSK             (0x00002000)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_trace_write_data_MIN             (0)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_trace_write_data_MAX             (1) // 0x00000001
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_trace_write_data_DEF             (0x00000001)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_trace_write_data_HSH             (0x011AE5E0)

  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_DDRPL_Activate_OFF               (14)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_DDRPL_Activate_WID               ( 1)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_DDRPL_Activate_MSK               (0x00004000)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_DDRPL_Activate_MIN               (0)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_DDRPL_Activate_MAX               (1) // 0x00000001
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_DDRPL_Activate_DEF               (0x00000000)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_DDRPL_Activate_HSH               (0x011CE5E0)

  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_SCHTrace_OFF                     (15)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_SCHTrace_WID                     ( 1)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_SCHTrace_MSK                     (0x00008000)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_SCHTrace_MIN                     (0)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_SCHTrace_MAX                     (1) // 0x00000001
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_SCHTrace_DEF                     (0x00000000)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_SCHTrace_HSH                     (0x011EE5E0)

  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_ddrpl_data_trace_width_OFF       (17)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_ddrpl_data_trace_width_WID       ( 2)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_ddrpl_data_trace_width_MSK       (0x00060000)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_ddrpl_data_trace_width_MIN       (0)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_ddrpl_data_trace_width_MAX       (2) // 0x00000002
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_ddrpl_data_trace_width_DEF       (0x00000000)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_ddrpl_data_trace_width_HSH       (0x0222E5E0)

  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_data_quarter_mode_byte_select_OFF (19)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_data_quarter_mode_byte_select_WID ( 2)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_data_quarter_mode_byte_select_MSK (0x00180000)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_data_quarter_mode_byte_select_MIN (0)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_data_quarter_mode_byte_select_MAX (3) // 0x00000003
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_data_quarter_mode_byte_select_DEF (0x00000000)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_data_quarter_mode_byte_select_HSH (0x0226E5E0)

  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_data_half_mode_byte_select_OFF   (21)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_data_half_mode_byte_select_WID   ( 1)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_data_half_mode_byte_select_MSK   (0x00200000)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_data_half_mode_byte_select_MIN   (0)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_data_half_mode_byte_select_MAX   (1) // 0x00000001
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_data_half_mode_byte_select_DEF   (0x00000000)
  #define MC0_CH0_CR_PL_AGENT_CFG_DTF_data_half_mode_byte_select_HSH   (0x012AE5E0)

#define MC0_CH0_CR_MCMNTS_SPARE2_REG                                   (0x0000E5F8)
#define MC0_CH0_CR_MCMNTS_SPARE2_DEF                                   (0x00000000)
#define MC0_CH0_CR_MCMNTS_SPARE2_VOLATILE_BITFIELDS_MSK                (0xFFFF0000)

  #define MC0_CH0_CR_MCMNTS_SPARE2_Spare_RW_OFF                        ( 0)
  #define MC0_CH0_CR_MCMNTS_SPARE2_Spare_RW_WID                        (16)
  #define MC0_CH0_CR_MCMNTS_SPARE2_Spare_RW_MSK                        (0x0000FFFF)
  #define MC0_CH0_CR_MCMNTS_SPARE2_Spare_RW_MIN                        (0)
  #define MC0_CH0_CR_MCMNTS_SPARE2_Spare_RW_MAX                        (65535) // 0x0000FFFF
  #define MC0_CH0_CR_MCMNTS_SPARE2_Spare_RW_DEF                        (0x00000000)
  #define MC0_CH0_CR_MCMNTS_SPARE2_Spare_RW_HSH                        (0x1000E5F8)

  #define MC0_CH0_CR_MCMNTS_SPARE2_Spare_RW_V_OFF                      (16)
  #define MC0_CH0_CR_MCMNTS_SPARE2_Spare_RW_V_WID                      (16)
  #define MC0_CH0_CR_MCMNTS_SPARE2_Spare_RW_V_MSK                      (0xFFFF0000)
  #define MC0_CH0_CR_MCMNTS_SPARE2_Spare_RW_V_MIN                      (0)
  #define MC0_CH0_CR_MCMNTS_SPARE2_Spare_RW_V_MAX                      (65535) // 0x0000FFFF
  #define MC0_CH0_CR_MCMNTS_SPARE2_Spare_RW_V_DEF                      (0x00000000)
  #define MC0_CH0_CR_MCMNTS_SPARE2_Spare_RW_V_HSH                      (0x10A0E5F8)

#define MC0_CH0_CR_MCMNTS_SPARE_REG                                    (0x0000E5FC)
#define MC0_CH0_CR_MCMNTS_SPARE_DEF                                    (0x00000000)
#define MC0_CH0_CR_MCMNTS_SPARE_VOLATILE_BITFIELDS_MSK                 (0xFF000000)

  #define MC0_CH0_CR_MCMNTS_SPARE_Spare_RW_OFF                         ( 0)
  #define MC0_CH0_CR_MCMNTS_SPARE_Spare_RW_WID                         ( 8)
  #define MC0_CH0_CR_MCMNTS_SPARE_Spare_RW_MSK                         (0x000000FF)
  #define MC0_CH0_CR_MCMNTS_SPARE_Spare_RW_MIN                         (0)
  #define MC0_CH0_CR_MCMNTS_SPARE_Spare_RW_MAX                         (255) // 0x000000FF
  #define MC0_CH0_CR_MCMNTS_SPARE_Spare_RW_DEF                         (0x00000000)
  #define MC0_CH0_CR_MCMNTS_SPARE_Spare_RW_HSH                         (0x0800E5FC)

  #define MC0_CH0_CR_MCMNTS_SPARE_DisBlockActDuringPanicRef_OFF        ( 8)
  #define MC0_CH0_CR_MCMNTS_SPARE_DisBlockActDuringPanicRef_WID        ( 1)
  #define MC0_CH0_CR_MCMNTS_SPARE_DisBlockActDuringPanicRef_MSK        (0x00000100)
  #define MC0_CH0_CR_MCMNTS_SPARE_DisBlockActDuringPanicRef_MIN        (0)
  #define MC0_CH0_CR_MCMNTS_SPARE_DisBlockActDuringPanicRef_MAX        (1) // 0x00000001
  #define MC0_CH0_CR_MCMNTS_SPARE_DisBlockActDuringPanicRef_DEF        (0x00000000)
  #define MC0_CH0_CR_MCMNTS_SPARE_DisBlockActDuringPanicRef_HSH        (0x0110E5FC)

  #define MC0_CH0_CR_MCMNTS_SPARE_dis_trr_on_any_bank_rhblock_OFF      ( 9)
  #define MC0_CH0_CR_MCMNTS_SPARE_dis_trr_on_any_bank_rhblock_WID      ( 1)
  #define MC0_CH0_CR_MCMNTS_SPARE_dis_trr_on_any_bank_rhblock_MSK      (0x00000200)
  #define MC0_CH0_CR_MCMNTS_SPARE_dis_trr_on_any_bank_rhblock_MIN      (0)
  #define MC0_CH0_CR_MCMNTS_SPARE_dis_trr_on_any_bank_rhblock_MAX      (1) // 0x00000001
  #define MC0_CH0_CR_MCMNTS_SPARE_dis_trr_on_any_bank_rhblock_DEF      (0x00000000)
  #define MC0_CH0_CR_MCMNTS_SPARE_dis_trr_on_any_bank_rhblock_HSH      (0x0112E5FC)

  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_dont_sub_mrs_delay_from_mr_timing_OFF (10)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_dont_sub_mrs_delay_from_mr_timing_WID ( 1)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_dont_sub_mrs_delay_from_mr_timing_MSK (0x00000400)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_dont_sub_mrs_delay_from_mr_timing_MIN (0)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_dont_sub_mrs_delay_from_mr_timing_MAX (1) // 0x00000001
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_dont_sub_mrs_delay_from_mr_timing_DEF (0x00000000)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_dont_sub_mrs_delay_from_mr_timing_HSH (0x0114E5FC)

  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_block_mrs_4cycles_after_mnt_OFF (11)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_block_mrs_4cycles_after_mnt_WID ( 1)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_block_mrs_4cycles_after_mnt_MSK (0x00000800)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_block_mrs_4cycles_after_mnt_MIN (0)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_block_mrs_4cycles_after_mnt_MAX (1) // 0x00000001
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_block_mrs_4cycles_after_mnt_DEF (0x00000000)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_block_mrs_4cycles_after_mnt_HSH (0x0116E5FC)

  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_allow_timed_ABR_OFF             (12)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_allow_timed_ABR_WID             ( 1)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_allow_timed_ABR_MSK             (0x00001000)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_allow_timed_ABR_MIN             (0)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_allow_timed_ABR_MAX             (1) // 0x00000001
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_allow_timed_ABR_DEF             (0x00000000)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_allow_timed_ABR_HSH             (0x0118E5FC)

  #define MC0_CH0_CR_MCMNTS_SPARE_DisSREXcnt_OFF                       (13)
  #define MC0_CH0_CR_MCMNTS_SPARE_DisSREXcnt_WID                       ( 1)
  #define MC0_CH0_CR_MCMNTS_SPARE_DisSREXcnt_MSK                       (0x00002000)
  #define MC0_CH0_CR_MCMNTS_SPARE_DisSREXcnt_MIN                       (0)
  #define MC0_CH0_CR_MCMNTS_SPARE_DisSREXcnt_MAX                       (1) // 0x00000001
  #define MC0_CH0_CR_MCMNTS_SPARE_DisSREXcnt_DEF                       (0x00000000)
  #define MC0_CH0_CR_MCMNTS_SPARE_DisSREXcnt_HSH                       (0x011AE5FC)

  #define MC0_CH0_CR_MCMNTS_SPARE_pattern_match_enable_OFF             (14)
  #define MC0_CH0_CR_MCMNTS_SPARE_pattern_match_enable_WID             ( 1)
  #define MC0_CH0_CR_MCMNTS_SPARE_pattern_match_enable_MSK             (0x00004000)
  #define MC0_CH0_CR_MCMNTS_SPARE_pattern_match_enable_MIN             (0)
  #define MC0_CH0_CR_MCMNTS_SPARE_pattern_match_enable_MAX             (1) // 0x00000001
  #define MC0_CH0_CR_MCMNTS_SPARE_pattern_match_enable_DEF             (0x00000000)
  #define MC0_CH0_CR_MCMNTS_SPARE_pattern_match_enable_HSH             (0x011CE5FC)

  #define MC0_CH0_CR_MCMNTS_SPARE_Dis_Block_DQS_MR18_on_MRS_OFF        (15)
  #define MC0_CH0_CR_MCMNTS_SPARE_Dis_Block_DQS_MR18_on_MRS_WID        ( 1)
  #define MC0_CH0_CR_MCMNTS_SPARE_Dis_Block_DQS_MR18_on_MRS_MSK        (0x00008000)
  #define MC0_CH0_CR_MCMNTS_SPARE_Dis_Block_DQS_MR18_on_MRS_MIN        (0)
  #define MC0_CH0_CR_MCMNTS_SPARE_Dis_Block_DQS_MR18_on_MRS_MAX        (1) // 0x00000001
  #define MC0_CH0_CR_MCMNTS_SPARE_Dis_Block_DQS_MR18_on_MRS_DEF        (0x00000000)
  #define MC0_CH0_CR_MCMNTS_SPARE_Dis_Block_DQS_MR18_on_MRS_HSH        (0x011EE5FC)

  #define MC0_CH0_CR_MCMNTS_SPARE_rf_fast_sleep_disable_OFF            (16)
  #define MC0_CH0_CR_MCMNTS_SPARE_rf_fast_sleep_disable_WID            ( 1)
  #define MC0_CH0_CR_MCMNTS_SPARE_rf_fast_sleep_disable_MSK            (0x00010000)
  #define MC0_CH0_CR_MCMNTS_SPARE_rf_fast_sleep_disable_MIN            (0)
  #define MC0_CH0_CR_MCMNTS_SPARE_rf_fast_sleep_disable_MAX            (1) // 0x00000001
  #define MC0_CH0_CR_MCMNTS_SPARE_rf_fast_sleep_disable_DEF            (0x00000000)
  #define MC0_CH0_CR_MCMNTS_SPARE_rf_fast_sleep_disable_HSH            (0x0120E5FC)

  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_ranks_abr_to_clkgate_OFF        (17)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_ranks_abr_to_clkgate_WID        ( 1)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_ranks_abr_to_clkgate_MSK        (0x00020000)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_ranks_abr_to_clkgate_MIN        (0)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_ranks_abr_to_clkgate_MAX        (1) // 0x00000001
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_ranks_abr_to_clkgate_DEF        (0x00000000)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_ranks_abr_to_clkgate_HSH        (0x0122E5FC)

  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_dis_ovrd_sagv_sre_srx_with_nop_OFF (18)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_dis_ovrd_sagv_sre_srx_with_nop_WID ( 1)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_dis_ovrd_sagv_sre_srx_with_nop_MSK (0x00040000)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_dis_ovrd_sagv_sre_srx_with_nop_MIN (0)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_dis_ovrd_sagv_sre_srx_with_nop_MAX (1) // 0x00000001
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_dis_ovrd_sagv_sre_srx_with_nop_DEF (0x00000000)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_dis_ovrd_sagv_sre_srx_with_nop_HSH (0x0124E5FC)

  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_signal_all_ch_sr_when_pde_OFF   (19)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_signal_all_ch_sr_when_pde_WID   ( 1)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_signal_all_ch_sr_when_pde_MSK   (0x00080000)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_signal_all_ch_sr_when_pde_MIN   (0)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_signal_all_ch_sr_when_pde_MAX   (1) // 0x00000001
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_signal_all_ch_sr_when_pde_DEF   (0x00000000)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_signal_all_ch_sr_when_pde_HSH   (0x0126E5FC)

  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_allow_trr_on_pkgc_drain_OFF     (20)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_allow_trr_on_pkgc_drain_WID     ( 1)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_allow_trr_on_pkgc_drain_MSK     (0x00100000)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_allow_trr_on_pkgc_drain_MIN     (0)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_allow_trr_on_pkgc_drain_MAX     (1) // 0x00000001
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_allow_trr_on_pkgc_drain_DEF     (0x00000000)
  #define MC0_CH0_CR_MCMNTS_SPARE_cbit_allow_trr_on_pkgc_drain_HSH     (0x0128E5FC)

  #define MC0_CH0_CR_MCMNTS_SPARE_Spare_RW_V_OFF                       (24)
  #define MC0_CH0_CR_MCMNTS_SPARE_Spare_RW_V_WID                       ( 8)
  #define MC0_CH0_CR_MCMNTS_SPARE_Spare_RW_V_MSK                       (0xFF000000)
  #define MC0_CH0_CR_MCMNTS_SPARE_Spare_RW_V_MIN                       (0)
  #define MC0_CH0_CR_MCMNTS_SPARE_Spare_RW_V_MAX                       (255) // 0x000000FF
  #define MC0_CH0_CR_MCMNTS_SPARE_Spare_RW_V_DEF                       (0x00000000)
  #define MC0_CH0_CR_MCMNTS_SPARE_Spare_RW_V_HSH                       (0x08B0E5FC)

#define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_REG                             (0x0000E600)
#define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_DEF                             (0x00000008)
#define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_VOLATILE_BITFIELDS_MSK          (0x00000000)

  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_drainless_sagv_do_zq_OFF      ( 0)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_drainless_sagv_do_zq_WID      ( 1)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_drainless_sagv_do_zq_MSK      (0x00000001)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_drainless_sagv_do_zq_MIN      (0)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_drainless_sagv_do_zq_MAX      (1) // 0x00000001
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_drainless_sagv_do_zq_DEF      (0x00000000)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_drainless_sagv_do_zq_HSH      (0x0100E600)

  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_dis_separate_zq_block_OFF     ( 1)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_dis_separate_zq_block_WID     ( 1)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_dis_separate_zq_block_MSK     (0x00000002)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_dis_separate_zq_block_MIN     (0)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_dis_separate_zq_block_MAX     (1) // 0x00000001
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_dis_separate_zq_block_DEF     (0x00000000)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_dis_separate_zq_block_HSH     (0x0102E600)

  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_disregardzqonsrtime_OFF       ( 2)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_disregardzqonsrtime_WID       ( 1)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_disregardzqonsrtime_MSK       (0x00000004)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_disregardzqonsrtime_MIN       (0)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_disregardzqonsrtime_MAX       (1) // 0x00000001
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_disregardzqonsrtime_DEF       (0x00000000)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_disregardzqonsrtime_HSH       (0x0104E600)

  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_background_zq_mode_OFF        ( 3)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_background_zq_mode_WID        ( 1)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_background_zq_mode_MSK        (0x00000008)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_background_zq_mode_MIN        (0)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_background_zq_mode_MAX        (1) // 0x00000001
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_background_zq_mode_DEF        (0x00000001)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_background_zq_mode_HSH        (0x0106E600)

  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_discrossrankzqblock_OFF       ( 4)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_discrossrankzqblock_WID       ( 1)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_discrossrankzqblock_MSK       (0x00000010)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_discrossrankzqblock_MIN       (0)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_discrossrankzqblock_MAX       (1) // 0x00000001
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_discrossrankzqblock_DEF       (0x00000000)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_discrossrankzqblock_HSH       (0x0108E600)

  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_dis_zq_OFF                    (17)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_dis_zq_WID                    ( 1)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_dis_zq_MSK                    (0x00020000)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_dis_zq_MIN                    (0)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_dis_zq_MAX                    (1) // 0x00000001
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_dis_zq_DEF                    (0x00000000)
  #define MC0_CH0_CR_ZQCAL_CONTROL_UARCH_dis_zq_HSH                    (0x0122E600)

#define MC0_CH1_CR_TC_PRE_REG                                          (0x0000E800)
//Duplicate of MC0_CH0_CR_TC_PRE_REG

#define MC0_CH1_CR_TC_RDRD_REG                                         (0x0000E80C)
//Duplicate of MC0_CH0_CR_TC_RDRD_REG

#define MC0_CH1_CR_TC_RDWR_REG                                         (0x0000E810)
//Duplicate of MC0_CH0_CR_TC_RDWR_REG

#define MC0_CH1_CR_TC_WRRD_REG                                         (0x0000E814)
//Duplicate of MC0_CH0_CR_TC_WRRD_REG

#define MC0_CH1_CR_TC_WRWR_REG                                         (0x0000E818)
//Duplicate of MC0_CH0_CR_TC_WRWR_REG

#define MC0_CH1_CR_PM_ADAPTIVE_CKE_REG                                 (0x0000E81C)
//Duplicate of MC0_CH0_CR_PM_ADAPTIVE_CKE_REG

#define MC0_CH1_CR_SC_ROUNDTRIP_LATENCY_REG                            (0x0000E820)
//Duplicate of MC0_CH0_CR_SC_ROUNDTRIP_LATENCY_REG

#define MC0_CH1_CR_PM_PDWN_CONFIG_UARCH_REG                            (0x0000E824)
//Duplicate of MC0_CH0_CR_PM_PDWN_CONFIG_UARCH_REG

#define MC0_CH1_CR_SCHED_CBIT_REG                                      (0x0000E828)
//Duplicate of MC0_CH0_CR_SCHED_CBIT_REG

#define MC0_CH1_CR_SCHED_SECOND_CBIT_REG                               (0x0000E82C)
//Duplicate of MC0_CH0_CR_SCHED_SECOND_CBIT_REG

#define MC0_CH1_CR_DFT_MISC_REG                                        (0x0000E830)
//Duplicate of MC0_CH0_CR_DFT_MISC_REG

#define MC0_CH1_CR_SC_PCIT_REG                                         (0x0000E834)
//Duplicate of MC0_CH0_CR_SC_PCIT_REG

#define MC0_CH1_CR_ECC_DFT_REG                                         (0x0000E838)
//Duplicate of MC0_CH0_CR_ECC_DFT_REG

#define MC0_CH1_CR_ECC_DFT_UARCH_REG                                   (0x0000E83C)
//Duplicate of MC0_CH0_CR_ECC_DFT_UARCH_REG

#define MC0_CH1_CR_PM_PDWN_CONFIG_REG                                  (0x0000E840)
//Duplicate of MC0_CH0_CR_PM_PDWN_CONFIG_REG

#define MC0_CH1_CR_ECCERRLOG0_REG                                      (0x0000E848)
//Duplicate of MC0_CH0_CR_ECCERRLOG0_REG

#define MC0_CH1_CR_ECCERRLOG1_REG                                      (0x0000E84C)
//Duplicate of MC0_CH0_CR_ECCERRLOG1_REG

#define MC0_CH1_CR_TC_PWRDN_REG                                        (0x0000E850)
//Duplicate of MC0_CH0_CR_TC_PWRDN_REG

#define MC0_CH1_CR_QUEUE_ENTRY_DISABLE_WPQ_HIGH_REG                    (0x0000E858)
//Duplicate of MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_HIGH_REG

#define MC0_CH1_CR_QUEUE_ENTRY_DISABLE_IPQ_REG                         (0x0000E85C)
//Duplicate of MC0_CH0_CR_QUEUE_ENTRY_DISABLE_IPQ_REG

#define MC0_CH1_CR_QUEUE_ENTRY_DISABLE_WPQ_LOW_REG                     (0x0000E860)
//Duplicate of MC0_CH0_CR_QUEUE_ENTRY_DISABLE_WPQ_LOW_REG

#define MC0_CH1_CR_SC_WDBWM_REG                                        (0x0000E868)
//Duplicate of MC0_CH0_CR_SC_WDBWM_REG

#define MC0_CH1_CR_TC_CAS_REG                                          (0x0000E870)
//Duplicate of MC0_CH0_CR_TC_CAS_REG

#define MC0_CH1_CR_MCSCHEDS_SPARE_REG                                  (0x0000E878)
//Duplicate of MC0_CH0_CR_MCSCHEDS_SPARE_REG

#define MC0_CH1_CR_TC_MPC_REG                                          (0x0000E87C)
//Duplicate of MC0_CH0_CR_TC_MPC_REG

#define MC0_CH1_CR_SC_ODT_MATRIX_REG                                   (0x0000E880)
//Duplicate of MC0_CH0_CR_SC_ODT_MATRIX_REG

#define MC0_CH1_CR_DFT_BLOCK_REG                                       (0x0000E884)
//Duplicate of MC0_CH0_CR_DFT_BLOCK_REG

#define MC0_CH1_CR_SC_GS_CFG_REG                                       (0x0000E888)
//Duplicate of MC0_CH0_CR_SC_GS_CFG_REG

#define MC0_CH1_CR_SC_GS_CFG_UARCH_REG                                 (0x0000E88C)
//Duplicate of MC0_CH0_CR_SC_GS_CFG_UARCH_REG

#define MC0_CH1_CR_SC_PH_THROTTLING_0_REG                              (0x0000E890)
//Duplicate of MC0_CH0_CR_SC_PH_THROTTLING_0_REG

#define MC0_CH1_CR_SC_PH_THROTTLING_1_REG                              (0x0000E894)
//Duplicate of MC0_CH0_CR_SC_PH_THROTTLING_1_REG

#define MC0_CH1_CR_SC_PH_THROTTLING_2_REG                              (0x0000E898)
//Duplicate of MC0_CH0_CR_SC_PH_THROTTLING_2_REG

#define MC0_CH1_CR_SC_PH_THROTTLING_3_REG                              (0x0000E89C)
//Duplicate of MC0_CH0_CR_SC_PH_THROTTLING_3_REG

#define MC0_CH1_CR_SC_WPQ_THRESHOLD_REG                                (0x0000E8A0)
//Duplicate of MC0_CH0_CR_SC_WPQ_THRESHOLD_REG

#define MC0_CH1_CR_SC_PR_CNT_CONFIG_REG                                (0x0000E8A8)
//Duplicate of MC0_CH0_CR_SC_PR_CNT_CONFIG_REG

#define MC0_CH1_CR_REUT_CH_MISC_CKE_CS_CTRL_REG                        (0x0000E8B0)
//Duplicate of MC0_CH0_CR_REUT_CH_MISC_CKE_CS_CTRL_REG

#define MC0_CH1_CR_SC_GS_CFG_TRAINING_REG                              (0x0000E8B4)
//Duplicate of MC0_CH0_CR_SC_GS_CFG_TRAINING_REG

#define MC0_CH1_CR_SPID_LOW_POWER_CTL_REG                              (0x0000E8B8)
//Duplicate of MC0_CH0_CR_SPID_LOW_POWER_CTL_REG

#define MC0_CH1_CR_SCHED_THIRD_CBIT_REG                                (0x0000E8C0)
//Duplicate of MC0_CH0_CR_SCHED_THIRD_CBIT_REG

#define MC0_CH1_CR_DEADLOCK_BREAKER_REG                                (0x0000E8C4)
//Duplicate of MC0_CH0_CR_DEADLOCK_BREAKER_REG

#define MC0_CH1_CR_MCSCHEDS_GLOBAL_DRIVER_GATE_CFG_REG                 (0x0000E8D0)
//Duplicate of MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_REG

#define MC0_CH1_CR_SC_BLOCKING_RULES_CFG_REG                           (0x0000E8E0)
//Duplicate of MC0_CH0_CR_SC_BLOCKING_RULES_CFG_REG

#define MC0_CH1_CR_PWM_DDR_SUBCH0_REQ_OCCUPANCY_COUNTER_REG            (0x0000E8E8)
//Duplicate of MC0_CH0_CR_PWM_DDR_SUBCH0_REQ_OCCUPANCY_COUNTER_REG

#define MC0_CH1_CR_PWM_DDR_SUBCH1_REQ_OCCUPANCY_COUNTER_REG            (0x0000E8F0)
//Duplicate of MC0_CH0_CR_PWM_DDR_SUBCH0_REQ_OCCUPANCY_COUNTER_REG

#define MC0_CH1_CR_TR_RRDVALID_CTRL_REG                                (0x0000E900)
//Duplicate of MC0_CH0_CR_TR_RRDVALID_CTRL_REG

#define MC0_CH1_CR_TR_RRDVALID_DATA_REG                                (0x0000E904)
//Duplicate of MC0_CH0_CR_TR_RRDVALID_DATA_REG

#define MC0_CH1_CR_TC_WCK_REG                                          (0x0000E908)
//Duplicate of MC0_CH0_CR_TC_WCK_REG

#define MC0_CH1_CR_WMM_READ_CONFIG_REG                                 (0x0000E910)
//Duplicate of MC0_CH0_CR_WMM_READ_CONFIG_REG

#define MC0_CH1_CR_MC2PHY_BGF_CTRL_REG                                 (0x0000E914)
//Duplicate of MC0_CH0_CR_MC2PHY_BGF_CTRL_REG

#define MC0_CH1_CR_SC_ADAPTIVE_PCIT_REG                                (0x0000E918)
//Duplicate of MC0_CH0_CR_SC_ADAPTIVE_PCIT_REG

#define MC0_CH1_CR_ROWHAMMER_CTL_REG                                   (0x0000E920)
//Duplicate of MC0_CH0_CR_ROWHAMMER_CTL_REG

#define MC0_CH1_CR_MERGE_REQ_READS_PQ_REG                              (0x0000E928)
//Duplicate of MC0_CH0_CR_MERGE_REQ_READS_PQ_REG

#define MC0_CH1_CR_DFI_CTL_REG                                         (0x0000E92C)
//Duplicate of MC0_CH0_CR_DFI_CTL_REG

#define MC0_CH1_CR_AUTO_PRE_CONTROL_REG                                (0x0000E930)
//Duplicate of MC0_CH0_CR_AUTO_PRE_CONTROL_REG

#define MC0_CH1_CR_TC_ACT_REG                                          (0x0000E938)
//Duplicate of MC0_CH0_CR_TC_ACT_REG

#define MC0_CH1_CR_QUEUE_ENTRY_DISABLE_RPQ_REG                         (0x0000E940)
//Duplicate of MC0_CH0_CR_QUEUE_ENTRY_DISABLE_RPQ_REG

#define MC0_CH1_CR_WCK_CONFIG_REG                                      (0x0000E948)
//Duplicate of MC0_CH0_CR_WCK_CONFIG_REG

#define MC0_CH1_CR_SC_WDBWM_UARCH_REG                                  (0x0000E958)
//Duplicate of MC0_CH0_CR_SC_WDBWM_UARCH_REG

#define MC0_CH1_CR_ROWHAMMER_CTL_UARCH_REG                             (0x0000E95C)
//Duplicate of MC0_CH0_CR_ROWHAMMER_CTL_UARCH_REG

#define MC0_CH1_CR_RH_TRR_CONTROL_REG                                  (0x0000EC00)
//Duplicate of MC0_CH0_CR_RH_TRR_CONTROL_REG

#define MC0_CH1_CR_REUT_CH_MISC_REFRESH_CTRL_REG                       (0x0000EC04)
//Duplicate of MC0_CH0_CR_REUT_CH_MISC_REFRESH_CTRL_REG

#define MC0_CH1_CR_REUT_CH_MISC_ZQ_CTRL_REG                            (0x0000EC08)
//Duplicate of MC0_CH0_CR_REUT_CH_MISC_ZQ_CTRL_REG

#define MC0_CH1_CR_TC_REFM_REG                                         (0x0000EC0C)
//Duplicate of MC0_CH0_CR_TC_REFM_REG

#define MC0_CH1_CR_RH_TRR_CONTROL_UARCH_REG                            (0x0000EC10)
//Duplicate of MC0_CH0_CR_RH_TRR_CONTROL_UARCH_REG

#define MC0_CH1_CR_DDR_MR_COMMAND_REG                                  (0x0000EC14)
//Duplicate of MC0_CH0_CR_DDR_MR_COMMAND_REG

#define MC0_CH1_CR_SCH0_DDR_MR_RESULT_LOW_REG                          (0x0000EC18)
//Duplicate of MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_REG

#define MC0_CH1_CR_SCH0_DDR_MR_RESULT_HIGH_REG                         (0x0000EC1C)
//Duplicate of MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_REG

#define MC0_CH1_CR_SCH0_DDR_MR_RESULT_ECC_REG                          (0x0000EC20)
//Duplicate of MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_REG

#define MC0_CH1_CR_RANK_TEMPERATURE_REG                                (0x0000EC24)
//Duplicate of MC0_CH0_CR_RANK_TEMPERATURE_REG

#define MC0_CH1_CR_PM_CONFIG_THERM_STATUS_REG                          (0x0000EC28)
//Duplicate of MC0_CH0_CR_PM_CONFIG_THERM_STATUS_REG

#define MC0_CH1_CR_MC_RDB_CREDITS_REG                                  (0x0000EC2C)
//Duplicate of MC0_CH0_CR_MC_RDB_CREDITS_REG

#define MC0_CH1_CR_RH_TRR_ADDRESS_REG                                  (0x0000EC30)
//Duplicate of MC0_CH0_CR_RH_TRR_ADDRESS_REG

#define MC0_CH1_CR_MC_INIT_STATE_UARCH_REG                             (0x0000EC34)
//Duplicate of MC0_CH0_CR_MC_INIT_STATE_UARCH_REG

#define MC0_CH1_CR_RFP_REG                                             (0x0000EC38)
//Duplicate of MC0_CH0_CR_RFP_REG

#define MC0_CH1_CR_RFP_UARCH_REG                                       (0x0000EC40)
//Duplicate of MC0_CH0_CR_RFP_UARCH_REG

#define MC0_CH1_CR_MC_REFRESH_STAGGER_REG                              (0x0000EC44)
//Duplicate of MC0_CH0_CR_MC_REFRESH_STAGGER_REG

#define MC0_CH1_CR_TC_ZQCAL_REG                                        (0x0000EC48)
//Duplicate of MC0_CH0_CR_TC_ZQCAL_REG

#define MC0_CH1_CR_ZQCAL_CONTROL_REG                                   (0x0000EC4C)
//Duplicate of MC0_CH0_CR_ZQCAL_CONTROL_REG

#define MC0_CH1_CR_SCH1_DDR_MR_RESULT_LOW_REG                          (0x0000EC50)
//Duplicate of MC0_CH0_CR_SCH0_DDR_MR_RESULT_LOW_REG

#define MC0_CH1_CR_MC_INIT_STATE_REG                                   (0x0000EC54)
//Duplicate of MC0_CH0_CR_MC_INIT_STATE_REG

#define MC0_CH1_CR_WDB_VISA_SEL_REG                                    (0x0000EC58)
//Duplicate of MC0_CH0_CR_WDB_VISA_SEL_REG

#define MC0_CH1_CR_REF_FSM_STATUS_REG                                  (0x0000EC60)
//Duplicate of MC0_CH0_CR_REF_FSM_STATUS_REG

#define MC0_CH1_CR_PM_DIMM_ACT_ENERGY_REG                              (0x0000EC68)
//Duplicate of MC0_CH0_CR_PM_DIMM_ACT_ENERGY_REG

#define MC0_CH1_CR_PM_DIMM_RD_ENERGY_REG                               (0x0000EC6C)
//Duplicate of MC0_CH0_CR_PM_DIMM_RD_ENERGY_REG

#define MC0_CH1_CR_PM_DIMM_WR_ENERGY_REG                               (0x0000EC70)
//Duplicate of MC0_CH0_CR_PM_DIMM_WR_ENERGY_REG

#define MC0_CH1_CR_PM_SREF_CONTROL_REG                                 (0x0000EC74)
//Duplicate of MC0_CH0_CR_PM_SREF_CONTROL_REG

#define MC0_CH1_CR_SC_WR_DELAY_REG                                     (0x0000EC78)
//Duplicate of MC0_CH0_CR_SC_WR_DELAY_REG

#define MC0_CH1_CR_READ_RETURN_DFT_REG                                 (0x0000EC7C)
//Duplicate of MC0_CH0_CR_READ_RETURN_DFT_REG

#define MC0_CH1_CR_SCH0_BIT_DESWIZZLE_REG                              (0x0000EC80)
//Duplicate of MC0_CH0_CR_SCH0_BIT_DESWIZZLE_REG

#define MC0_CH1_CR_SCH1_BIT_DESWIZZLE_REG                              (0x0000EC84)
//Duplicate of MC0_CH0_CR_SCH0_BIT_DESWIZZLE_REG

#define MC0_CH1_CR_SC_PBR_REG                                          (0x0000EC88)
//Duplicate of MC0_CH0_CR_SC_PBR_REG

#define MC0_CH1_CR_DVFSQ_CTL_REG                                       (0x0000EC8C)
//Duplicate of MC0_CH0_CR_DVFSQ_CTL_REG

#define MC0_CH1_CR_TC_MR_REG                                           (0x0000EC90)
//Duplicate of MC0_CH0_CR_TC_MR_REG

#define MC0_CH1_CR_SCH1_DDR_MR_RESULT_HIGH_REG                         (0x0000EC98)
//Duplicate of MC0_CH0_CR_SCH0_DDR_MR_RESULT_HIGH_REG

#define MC0_CH1_CR_SCH1_DDR_MR_RESULT_ECC_REG                          (0x0000EC9C)
//Duplicate of MC0_CH0_CR_SCH0_DDR_MR_RESULT_ECC_REG

#define MC0_CH1_CR_TC_RFTP_REG                                         (0x0000ECA0)
//Duplicate of MC0_CH0_CR_TC_RFTP_REG

#define MC0_CH1_CR_MRS_CR_MAILBOX_REG                                  (0x0000ECA8)
//Duplicate of MC0_CH0_CR_MRS_CR_MAILBOX_REG

#define MC0_CH1_CR_SCH0_BYTE_DESWIZZLE_REG                             (0x0000ECB8)
//Duplicate of MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_REG

#define MC0_CH1_CR_SCH1_BYTE_DESWIZZLE_REG                             (0x0000ECBC)
//Duplicate of MC0_CH0_CR_SCH0_BYTE_DESWIZZLE_REG

#define MC0_CH1_CR_TC_SREXITTP_REG                                     (0x0000ECC0)
//Duplicate of MC0_CH0_CR_TC_SREXITTP_REG

#define MC0_CH1_CR_RETRAINING_OSCL_PARAMS_REG                          (0x0000ECC8)
//Duplicate of MC0_CH0_CR_RETRAINING_OSCL_PARAMS_REG

#define MC0_CH1_CR_MRH_GENERIC_COMMAND_REG                             (0x0000ECCC)
//Duplicate of MC0_CH0_CR_MRH_GENERIC_COMMAND_REG

#define MC0_CH1_CR_TC_RETRAINING_OSCL_REG                              (0x0000ECD0)
//Duplicate of MC0_CH0_CR_TC_RETRAINING_OSCL_REG

#define MC0_CH1_CR_FORCE_DRAM_MNT_FLOW_REG                             (0x0000ECD4)
//Duplicate of MC0_CH0_CR_FORCE_DRAM_MNT_FLOW_REG

#define MC0_CH1_CR_WDB_RD_WR_DFX_DATA_REG                              (0x0000ECD8)
//Duplicate of MC0_CH0_CR_WDB_RD_WR_DFX_DATA_REG

#define MC0_CH1_CR_WDB_RD_WR_DFX_CTL_REG                               (0x0000ECE0)
//Duplicate of MC0_CH0_CR_WDB_RD_WR_DFX_CTL_REG

#define MC0_CH1_CR_WDB_MBIST_0_REG                                     (0x0000ECE8)
//Duplicate of MC0_CH0_CR_WDB_MBIST_0_REG

#define MC0_CH1_CR_WDB_MBIST_1_REG                                     (0x0000ECEC)
//Duplicate of MC0_CH0_CR_WDB_MBIST_0_REG

#define MC0_CH1_CR_RDB_MBIST_REG                                       (0x0000ECF8)
//Duplicate of MC0_CH0_CR_WDB_MBIST_0_REG

#define MC0_CH1_CR_ECC_INJECT_COUNT_REG                                (0x0000ECFC)
//Duplicate of MC0_CH0_CR_ECC_INJECT_COUNT_REG

#define MC0_CH1_CR_PWM_DDR_SUBCH0_RDDATA_COUNTER_REG                   (0x0000ED00)
//Duplicate of MC0_CH0_CR_PWM_DDR_SUBCH0_RDDATA_COUNTER_REG

#define MC0_CH1_CR_PWM_DDR_SUBCH1_RDDATA_COUNTER_REG                   (0x0000ED08)
//Duplicate of MC0_CH0_CR_PWM_DDR_SUBCH0_RDDATA_COUNTER_REG

#define MC0_CH1_CR_PWM_DDR_SUBCH0_WRDATA_COUNTER_REG                   (0x0000ED10)
//Duplicate of MC0_CH0_CR_PWM_DDR_SUBCH0_WRDATA_COUNTER_REG

#define MC0_CH1_CR_PWM_DDR_SUBCH1_WRDATA_COUNTER_REG                   (0x0000ED18)
//Duplicate of MC0_CH0_CR_PWM_DDR_SUBCH0_WRDATA_COUNTER_REG

#define MC0_CH1_CR_WDB_CAPTURE_STATUS_REG                              (0x0000ED20)
//Duplicate of MC0_CH0_CR_WDB_CAPTURE_STATUS_REG

#define MC0_CH1_CR_WDB_CAPTURE_CTL_REG                                 (0x0000ED24)
//Duplicate of MC0_CH0_CR_WDB_CAPTURE_CTL_REG

#define MC0_CH1_CR_MRH_CONFIG_REG                                      (0x0000ED28)
//Duplicate of MC0_CH0_CR_MRH_CONFIG_REG

#define MC0_CH1_CR_PAGEOPEN_POLICY_LIMIT_REG                           (0x0000ED2C)
//Duplicate of MC0_CH0_CR_PAGEOPEN_POLICY_LIMIT_REG

#define MC0_CH1_CR_RH_TRR_LFSR0_REG                                    (0x0000ED30)
//Duplicate of MC0_CH0_CR_RH_TRR_LFSR0_REG

#define MC0_CH1_CR_RH_TRR_LFSR1_REG                                    (0x0000ED34)
//Duplicate of MC0_CH0_CR_RH_TRR_LFSR0_REG

#define MC0_CH1_CR_MRS_FSM_CONTROL_REG                                 (0x0000EDD0)
//Duplicate of MC0_CH0_CR_MRS_FSM_CONTROL_REG

#define MC0_CH1_CR_MRS_FSM_CONTROL_UARCH_REG                           (0x0000EDD4)
//Duplicate of MC0_CH0_CR_MRS_FSM_CONTROL_UARCH_REG

#define MC0_CH1_CR_MRS_FSM_RUN_REG                                     (0x0000EDD8)
//Duplicate of MC0_CH0_CR_MRS_FSM_RUN_REG

#define MC0_CH1_CR_PL_AGENT_CFG_DTF_REG                                (0x0000EDE0)
//Duplicate of MC0_CH0_CR_PL_AGENT_CFG_DTF_REG

#define MC0_CH1_CR_MCMNTS_SPARE2_REG                                   (0x0000EDF8)
//Duplicate of MC0_CH0_CR_MCMNTS_SPARE2_REG

#define MC0_CH1_CR_MCMNTS_SPARE_REG                                    (0x0000EDFC)
//Duplicate of MC0_CH0_CR_MCMNTS_SPARE_REG

#define MC0_CH1_CR_ZQCAL_CONTROL_UARCH_REG                             (0x0000EE00)
//Duplicate of MC0_CH0_CR_ZQCAL_CONTROL_UARCH_REG
#pragma pack(pop)
#endif
