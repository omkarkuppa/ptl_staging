#ifndef __MrcRegisterPtlDxxx_h__
#define __MrcRegisterPtlDxxx_h__
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

#define MC0_IBECC_ACTIVATE_REG                                         (0x0000D400)
#define MC0_IBECC_ACTIVATE_DEF                                         (0x00000000)
#define MC0_IBECC_ACTIVATE_VOLATILE_BITFIELDS_MSK                      (0x00000000)

  #define MC0_IBECC_ACTIVATE_IBECC_EN_OFF                              ( 0)
  #define MC0_IBECC_ACTIVATE_IBECC_EN_WID                              ( 1)
  #define MC0_IBECC_ACTIVATE_IBECC_EN_MSK                              (0x00000001)
  #define MC0_IBECC_ACTIVATE_IBECC_EN_MIN                              (0)
  #define MC0_IBECC_ACTIVATE_IBECC_EN_MAX                              (1) // 0x00000001
  #define MC0_IBECC_ACTIVATE_IBECC_EN_DEF                              (0x00000000)
  #define MC0_IBECC_ACTIVATE_IBECC_EN_HSH                              (0x0100D400)

  #define MC0_IBECC_ACTIVATE_RSVD_OFF                                  ( 1)
  #define MC0_IBECC_ACTIVATE_RSVD_WID                                  (31)
  #define MC0_IBECC_ACTIVATE_RSVD_MSK                                  (0xFFFFFFFE)
  #define MC0_IBECC_ACTIVATE_RSVD_MIN                                  (0)
  #define MC0_IBECC_ACTIVATE_RSVD_MAX                                  (2147483647) // 0x7FFFFFFF
  #define MC0_IBECC_ACTIVATE_RSVD_DEF                                  (0x00000000)
  #define MC0_IBECC_ACTIVATE_RSVD_HSH                                  (0x1F02D400)

#define MC0_IBECC_STATUS_REG                                           (0x0000D404)
#define MC0_IBECC_STATUS_DEF                                           (0x00000000)
#define MC0_IBECC_STATUS_VOLATILE_BITFIELDS_MSK                        (0x0FFFFFC1)

  #define MC0_IBECC_STATUS_IBECC_IDLE_OFF                              ( 0)
  #define MC0_IBECC_STATUS_IBECC_IDLE_WID                              ( 1)
  #define MC0_IBECC_STATUS_IBECC_IDLE_MSK                              (0x00000001)
  #define MC0_IBECC_STATUS_IBECC_IDLE_MIN                              (0)
  #define MC0_IBECC_STATUS_IBECC_IDLE_MAX                              (1) // 0x00000001
  #define MC0_IBECC_STATUS_IBECC_IDLE_DEF                              (0x00000000)
  #define MC0_IBECC_STATUS_IBECC_IDLE_HSH                              (0x0180D404)

  #define MC0_IBECC_STATUS_RSVD_OFF                                    ( 1)
  #define MC0_IBECC_STATUS_RSVD_WID                                    ( 5)
  #define MC0_IBECC_STATUS_RSVD_MSK                                    (0x0000003E)
  #define MC0_IBECC_STATUS_RSVD_MIN                                    (0)
  #define MC0_IBECC_STATUS_RSVD_MAX                                    (31) // 0x0000001F
  #define MC0_IBECC_STATUS_RSVD_DEF                                    (0x00000000)
  #define MC0_IBECC_STATUS_RSVD_HSH                                    (0x0502D404)

  #define MC0_IBECC_STATUS_RPQ_FULL_OFF                                ( 6)
  #define MC0_IBECC_STATUS_RPQ_FULL_WID                                ( 1)
  #define MC0_IBECC_STATUS_RPQ_FULL_MSK                                (0x00000040)
  #define MC0_IBECC_STATUS_RPQ_FULL_MIN                                (0)
  #define MC0_IBECC_STATUS_RPQ_FULL_MAX                                (1) // 0x00000001
  #define MC0_IBECC_STATUS_RPQ_FULL_DEF                                (0x00000000)
  #define MC0_IBECC_STATUS_RPQ_FULL_HSH                                (0x018CD404)

  #define MC0_IBECC_STATUS_RPQ_EMPTY_OFF                               ( 7)
  #define MC0_IBECC_STATUS_RPQ_EMPTY_WID                               ( 1)
  #define MC0_IBECC_STATUS_RPQ_EMPTY_MSK                               (0x00000080)
  #define MC0_IBECC_STATUS_RPQ_EMPTY_MIN                               (0)
  #define MC0_IBECC_STATUS_RPQ_EMPTY_MAX                               (1) // 0x00000001
  #define MC0_IBECC_STATUS_RPQ_EMPTY_DEF                               (0x00000000)
  #define MC0_IBECC_STATUS_RPQ_EMPTY_HSH                               (0x018ED404)

  #define MC0_IBECC_STATUS_IPQ_FULL_OFF                                ( 8)
  #define MC0_IBECC_STATUS_IPQ_FULL_WID                                ( 1)
  #define MC0_IBECC_STATUS_IPQ_FULL_MSK                                (0x00000100)
  #define MC0_IBECC_STATUS_IPQ_FULL_MIN                                (0)
  #define MC0_IBECC_STATUS_IPQ_FULL_MAX                                (1) // 0x00000001
  #define MC0_IBECC_STATUS_IPQ_FULL_DEF                                (0x00000000)
  #define MC0_IBECC_STATUS_IPQ_FULL_HSH                                (0x0190D404)

  #define MC0_IBECC_STATUS_IPQ_EMPTY_OFF                               ( 9)
  #define MC0_IBECC_STATUS_IPQ_EMPTY_WID                               ( 1)
  #define MC0_IBECC_STATUS_IPQ_EMPTY_MSK                               (0x00000200)
  #define MC0_IBECC_STATUS_IPQ_EMPTY_MIN                               (0)
  #define MC0_IBECC_STATUS_IPQ_EMPTY_MAX                               (1) // 0x00000001
  #define MC0_IBECC_STATUS_IPQ_EMPTY_DEF                               (0x00000000)
  #define MC0_IBECC_STATUS_IPQ_EMPTY_HSH                               (0x0192D404)

  #define MC0_IBECC_STATUS_WPQ_FULL_OFF                                (10)
  #define MC0_IBECC_STATUS_WPQ_FULL_WID                                ( 1)
  #define MC0_IBECC_STATUS_WPQ_FULL_MSK                                (0x00000400)
  #define MC0_IBECC_STATUS_WPQ_FULL_MIN                                (0)
  #define MC0_IBECC_STATUS_WPQ_FULL_MAX                                (1) // 0x00000001
  #define MC0_IBECC_STATUS_WPQ_FULL_DEF                                (0x00000000)
  #define MC0_IBECC_STATUS_WPQ_FULL_HSH                                (0x0194D404)

  #define MC0_IBECC_STATUS_WPQ_EMPTY_OFF                               (11)
  #define MC0_IBECC_STATUS_WPQ_EMPTY_WID                               ( 1)
  #define MC0_IBECC_STATUS_WPQ_EMPTY_MSK                               (0x00000800)
  #define MC0_IBECC_STATUS_WPQ_EMPTY_MIN                               (0)
  #define MC0_IBECC_STATUS_WPQ_EMPTY_MAX                               (1) // 0x00000001
  #define MC0_IBECC_STATUS_WPQ_EMPTY_DEF                               (0x00000000)
  #define MC0_IBECC_STATUS_WPQ_EMPTY_HSH                               (0x0196D404)

  #define MC0_IBECC_STATUS_ECC_READ_TRACKER_FULL_OFF                   (12)
  #define MC0_IBECC_STATUS_ECC_READ_TRACKER_FULL_WID                   ( 1)
  #define MC0_IBECC_STATUS_ECC_READ_TRACKER_FULL_MSK                   (0x00001000)
  #define MC0_IBECC_STATUS_ECC_READ_TRACKER_FULL_MIN                   (0)
  #define MC0_IBECC_STATUS_ECC_READ_TRACKER_FULL_MAX                   (1) // 0x00000001
  #define MC0_IBECC_STATUS_ECC_READ_TRACKER_FULL_DEF                   (0x00000000)
  #define MC0_IBECC_STATUS_ECC_READ_TRACKER_FULL_HSH                   (0x0198D404)

  #define MC0_IBECC_STATUS_ECC_READ_TRACKER_EMPTY_OFF                  (13)
  #define MC0_IBECC_STATUS_ECC_READ_TRACKER_EMPTY_WID                  ( 1)
  #define MC0_IBECC_STATUS_ECC_READ_TRACKER_EMPTY_MSK                  (0x00002000)
  #define MC0_IBECC_STATUS_ECC_READ_TRACKER_EMPTY_MIN                  (0)
  #define MC0_IBECC_STATUS_ECC_READ_TRACKER_EMPTY_MAX                  (1) // 0x00000001
  #define MC0_IBECC_STATUS_ECC_READ_TRACKER_EMPTY_DEF                  (0x00000000)
  #define MC0_IBECC_STATUS_ECC_READ_TRACKER_EMPTY_HSH                  (0x019AD404)

  #define MC0_IBECC_STATUS_PROTECTED_WRITE_EXISTS_OFF                  (14)
  #define MC0_IBECC_STATUS_PROTECTED_WRITE_EXISTS_WID                  ( 1)
  #define MC0_IBECC_STATUS_PROTECTED_WRITE_EXISTS_MSK                  (0x00004000)
  #define MC0_IBECC_STATUS_PROTECTED_WRITE_EXISTS_MIN                  (0)
  #define MC0_IBECC_STATUS_PROTECTED_WRITE_EXISTS_MAX                  (1) // 0x00000001
  #define MC0_IBECC_STATUS_PROTECTED_WRITE_EXISTS_DEF                  (0x00000000)
  #define MC0_IBECC_STATUS_PROTECTED_WRITE_EXISTS_HSH                  (0x019CD404)

  #define MC0_IBECC_STATUS_VC0_PROTECTED_READ_EXISTS_OFF               (15)
  #define MC0_IBECC_STATUS_VC0_PROTECTED_READ_EXISTS_WID               ( 1)
  #define MC0_IBECC_STATUS_VC0_PROTECTED_READ_EXISTS_MSK               (0x00008000)
  #define MC0_IBECC_STATUS_VC0_PROTECTED_READ_EXISTS_MIN               (0)
  #define MC0_IBECC_STATUS_VC0_PROTECTED_READ_EXISTS_MAX               (1) // 0x00000001
  #define MC0_IBECC_STATUS_VC0_PROTECTED_READ_EXISTS_DEF               (0x00000000)
  #define MC0_IBECC_STATUS_VC0_PROTECTED_READ_EXISTS_HSH               (0x019ED404)

  #define MC0_IBECC_STATUS_VC1_PROTECTED_READ_EXISTS_OFF               (16)
  #define MC0_IBECC_STATUS_VC1_PROTECTED_READ_EXISTS_WID               ( 1)
  #define MC0_IBECC_STATUS_VC1_PROTECTED_READ_EXISTS_MSK               (0x00010000)
  #define MC0_IBECC_STATUS_VC1_PROTECTED_READ_EXISTS_MIN               (0)
  #define MC0_IBECC_STATUS_VC1_PROTECTED_READ_EXISTS_MAX               (1) // 0x00000001
  #define MC0_IBECC_STATUS_VC1_PROTECTED_READ_EXISTS_DEF               (0x00000000)
  #define MC0_IBECC_STATUS_VC1_PROTECTED_READ_EXISTS_HSH               (0x01A0D404)

  #define MC0_IBECC_STATUS_WAR_HAZARD_EXISTS_OFF                       (17)
  #define MC0_IBECC_STATUS_WAR_HAZARD_EXISTS_WID                       ( 1)
  #define MC0_IBECC_STATUS_WAR_HAZARD_EXISTS_MSK                       (0x00020000)
  #define MC0_IBECC_STATUS_WAR_HAZARD_EXISTS_MIN                       (0)
  #define MC0_IBECC_STATUS_WAR_HAZARD_EXISTS_MAX                       (1) // 0x00000001
  #define MC0_IBECC_STATUS_WAR_HAZARD_EXISTS_DEF                       (0x00000000)
  #define MC0_IBECC_STATUS_WAR_HAZARD_EXISTS_HSH                       (0x01A2D404)

  #define MC0_IBECC_STATUS_RAW_HAZARD_EXISTS_OFF                       (18)
  #define MC0_IBECC_STATUS_RAW_HAZARD_EXISTS_WID                       ( 1)
  #define MC0_IBECC_STATUS_RAW_HAZARD_EXISTS_MSK                       (0x00040000)
  #define MC0_IBECC_STATUS_RAW_HAZARD_EXISTS_MIN                       (0)
  #define MC0_IBECC_STATUS_RAW_HAZARD_EXISTS_MAX                       (1) // 0x00000001
  #define MC0_IBECC_STATUS_RAW_HAZARD_EXISTS_DEF                       (0x00000000)
  #define MC0_IBECC_STATUS_RAW_HAZARD_EXISTS_HSH                       (0x01A4D404)

  #define MC0_IBECC_STATUS_WAW_HAZARD_EXISTS_OFF                       (19)
  #define MC0_IBECC_STATUS_WAW_HAZARD_EXISTS_WID                       ( 1)
  #define MC0_IBECC_STATUS_WAW_HAZARD_EXISTS_MSK                       (0x00080000)
  #define MC0_IBECC_STATUS_WAW_HAZARD_EXISTS_MIN                       (0)
  #define MC0_IBECC_STATUS_WAW_HAZARD_EXISTS_MAX                       (1) // 0x00000001
  #define MC0_IBECC_STATUS_WAW_HAZARD_EXISTS_DEF                       (0x00000000)
  #define MC0_IBECC_STATUS_WAW_HAZARD_EXISTS_HSH                       (0x01A6D404)

  #define MC0_IBECC_STATUS_CFI_PORT0_RX_STATE_OFF                      (20)
  #define MC0_IBECC_STATUS_CFI_PORT0_RX_STATE_WID                      ( 2)
  #define MC0_IBECC_STATUS_CFI_PORT0_RX_STATE_MSK                      (0x00300000)
  #define MC0_IBECC_STATUS_CFI_PORT0_RX_STATE_MIN                      (0)
  #define MC0_IBECC_STATUS_CFI_PORT0_RX_STATE_MAX                      (3) // 0x00000003
  #define MC0_IBECC_STATUS_CFI_PORT0_RX_STATE_DEF                      (0x00000000)
  #define MC0_IBECC_STATUS_CFI_PORT0_RX_STATE_HSH                      (0x02A8D404)

  #define MC0_IBECC_STATUS_CFI_PORT0_TX_STATE_OFF                      (22)
  #define MC0_IBECC_STATUS_CFI_PORT0_TX_STATE_WID                      ( 2)
  #define MC0_IBECC_STATUS_CFI_PORT0_TX_STATE_MSK                      (0x00C00000)
  #define MC0_IBECC_STATUS_CFI_PORT0_TX_STATE_MIN                      (0)
  #define MC0_IBECC_STATUS_CFI_PORT0_TX_STATE_MAX                      (3) // 0x00000003
  #define MC0_IBECC_STATUS_CFI_PORT0_TX_STATE_DEF                      (0x00000000)
  #define MC0_IBECC_STATUS_CFI_PORT0_TX_STATE_HSH                      (0x02ACD404)

  #define MC0_IBECC_STATUS_CFI_PORT1_RX_STATE_OFF                      (24)
  #define MC0_IBECC_STATUS_CFI_PORT1_RX_STATE_WID                      ( 2)
  #define MC0_IBECC_STATUS_CFI_PORT1_RX_STATE_MSK                      (0x03000000)
  #define MC0_IBECC_STATUS_CFI_PORT1_RX_STATE_MIN                      (0)
  #define MC0_IBECC_STATUS_CFI_PORT1_RX_STATE_MAX                      (3) // 0x00000003
  #define MC0_IBECC_STATUS_CFI_PORT1_RX_STATE_DEF                      (0x00000000)
  #define MC0_IBECC_STATUS_CFI_PORT1_RX_STATE_HSH                      (0x02B0D404)

  #define MC0_IBECC_STATUS_CFI_PORT1_TX_STATE_OFF                      (26)
  #define MC0_IBECC_STATUS_CFI_PORT1_TX_STATE_WID                      ( 2)
  #define MC0_IBECC_STATUS_CFI_PORT1_TX_STATE_MSK                      (0x0C000000)
  #define MC0_IBECC_STATUS_CFI_PORT1_TX_STATE_MIN                      (0)
  #define MC0_IBECC_STATUS_CFI_PORT1_TX_STATE_MAX                      (3) // 0x00000003
  #define MC0_IBECC_STATUS_CFI_PORT1_TX_STATE_DEF                      (0x00000000)
  #define MC0_IBECC_STATUS_CFI_PORT1_TX_STATE_HSH                      (0x02B4D404)

  #define MC0_IBECC_STATUS_RSVD2_OFF                                   (28)
  #define MC0_IBECC_STATUS_RSVD2_WID                                   ( 4)
  #define MC0_IBECC_STATUS_RSVD2_MSK                                   (0xF0000000)
  #define MC0_IBECC_STATUS_RSVD2_MIN                                   (0)
  #define MC0_IBECC_STATUS_RSVD2_MAX                                   (15) // 0x0000000F
  #define MC0_IBECC_STATUS_RSVD2_DEF                                   (0x00000000)
  #define MC0_IBECC_STATUS_RSVD2_HSH                                   (0x0438D404)

#define MC0_IBECC_IOSF_SB_EP_CTRL_REG                                  (0x0000D408)
#define MC0_IBECC_IOSF_SB_EP_CTRL_DEF                                  (0x00100100)
#define MC0_IBECC_IOSF_SB_EP_CTRL_VOLATILE_BITFIELDS_MSK               (0x00000000)

  #define MC0_IBECC_IOSF_SB_EP_CTRL_CGCTRL_CLKGATEDEF_OFF              ( 0)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_CGCTRL_CLKGATEDEF_WID              ( 1)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_CGCTRL_CLKGATEDEF_MSK              (0x00000001)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_CGCTRL_CLKGATEDEF_MIN              (0)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_CGCTRL_CLKGATEDEF_MAX              (1) // 0x00000001
  #define MC0_IBECC_IOSF_SB_EP_CTRL_CGCTRL_CLKGATEDEF_DEF              (0x00000000)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_CGCTRL_CLKGATEDEF_HSH              (0x0100D408)

  #define MC0_IBECC_IOSF_SB_EP_CTRL_RSVD0_OFF                          ( 1)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_RSVD0_WID                          ( 7)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_RSVD0_MSK                          (0x000000FE)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_RSVD0_MIN                          (0)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_RSVD0_MAX                          (127) // 0x0000007F
  #define MC0_IBECC_IOSF_SB_EP_CTRL_RSVD0_DEF                          (0x00000000)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_RSVD0_HSH                          (0x0702D408)

  #define MC0_IBECC_IOSF_SB_EP_CTRL_CGCTRL_CLKGATEN_OFF                ( 8)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_CGCTRL_CLKGATEN_WID                ( 1)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_CGCTRL_CLKGATEN_MSK                (0x00000100)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_CGCTRL_CLKGATEN_MIN                (0)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_CGCTRL_CLKGATEN_MAX                (1) // 0x00000001
  #define MC0_IBECC_IOSF_SB_EP_CTRL_CGCTRL_CLKGATEN_DEF                (0x00000001)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_CGCTRL_CLKGATEN_HSH                (0x0110D408)

  #define MC0_IBECC_IOSF_SB_EP_CTRL_RSVD1_OFF                          ( 9)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_RSVD1_WID                          ( 7)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_RSVD1_MSK                          (0x0000FE00)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_RSVD1_MIN                          (0)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_RSVD1_MAX                          (127) // 0x0000007F
  #define MC0_IBECC_IOSF_SB_EP_CTRL_RSVD1_DEF                          (0x00000000)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_RSVD1_HSH                          (0x0712D408)

  #define MC0_IBECC_IOSF_SB_EP_CTRL_CGCTRL_IDLECNT_OFF                 (16)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_CGCTRL_IDLECNT_WID                 ( 8)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_CGCTRL_IDLECNT_MSK                 (0x00FF0000)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_CGCTRL_IDLECNT_MIN                 (0)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_CGCTRL_IDLECNT_MAX                 (255) // 0x000000FF
  #define MC0_IBECC_IOSF_SB_EP_CTRL_CGCTRL_IDLECNT_DEF                 (0x00000010)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_CGCTRL_IDLECNT_HSH                 (0x0820D408)

  #define MC0_IBECC_IOSF_SB_EP_CTRL_RSVD2_OFF                          (24)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_RSVD2_WID                          ( 8)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_RSVD2_MSK                          (0xFF000000)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_RSVD2_MIN                          (0)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_RSVD2_MAX                          (255) // 0x000000FF
  #define MC0_IBECC_IOSF_SB_EP_CTRL_RSVD2_DEF                          (0x00000000)
  #define MC0_IBECC_IOSF_SB_EP_CTRL_RSVD2_HSH                          (0x0830D408)

#define MC0_IBECC_PROTECT_ADDR_RANGE_0_REG                             (0x0000D40C)
#define MC0_IBECC_PROTECT_ADDR_RANGE_0_DEF                             (0x00000000)
#define MC0_IBECC_PROTECT_ADDR_RANGE_0_VOLATILE_BITFIELDS_MSK          (0x40000000)

  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_BASE_OFF                      ( 0)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_BASE_WID                      (14)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_BASE_MSK                      (0x00003FFF)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_BASE_MIN                      (0)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_BASE_MAX                      (16383) // 0x00003FFF
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_BASE_DEF                      (0x00000000)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_BASE_HSH                      (0x0E00D40C)

  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RSVD0_OFF                     (14)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RSVD0_WID                     ( 1)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RSVD0_MSK                     (0x00004000)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RSVD0_MIN                     (0)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RSVD0_MAX                     (1) // 0x00000001
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RSVD0_DEF                     (0x00000000)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RSVD0_HSH                     (0x011CD40C)

  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RSVD1_OFF                     (15)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RSVD1_WID                     ( 1)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RSVD1_MSK                     (0x00008000)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RSVD1_MIN                     (0)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RSVD1_MAX                     (1) // 0x00000001
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RSVD1_DEF                     (0x00000000)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RSVD1_HSH                     (0x011ED40C)

  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_MASK_OFF                      (16)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_MASK_WID                      (14)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_MASK_MSK                      (0x3FFF0000)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_MASK_MIN                      (0)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_MASK_MAX                      (16383) // 0x00003FFF
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_MASK_DEF                      (0x00000000)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_MASK_HSH                      (0x0E20D40C)

  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RANGE_INIT_STATUS_OFF         (30)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RANGE_INIT_STATUS_WID         ( 1)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RANGE_INIT_STATUS_MSK         (0x40000000)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RANGE_INIT_STATUS_MIN         (0)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RANGE_INIT_STATUS_MAX         (1) // 0x00000001
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RANGE_INIT_STATUS_DEF         (0x00000000)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RANGE_INIT_STATUS_HSH         (0x01BCD40C)

  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RANGE_EN_OFF                  (31)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RANGE_EN_WID                  ( 1)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RANGE_EN_MSK                  (0x80000000)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RANGE_EN_MIN                  (0)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RANGE_EN_MAX                  (1) // 0x00000001
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RANGE_EN_DEF                  (0x00000000)
  #define MC0_IBECC_PROTECT_ADDR_RANGE_0_RANGE_EN_HSH                  (0x013ED40C)

#define MC0_IBECC_PROTECT_ADDR_RANGE_1_REG                             (0x0000D410)
//Duplicate of MC0_IBECC_PROTECT_ADDR_RANGE_0_REG

#define MC0_IBECC_PROTECT_ADDR_RANGE_2_REG                             (0x0000D414)
//Duplicate of MC0_IBECC_PROTECT_ADDR_RANGE_0_REG

#define MC0_IBECC_PROTECT_ADDR_RANGE_3_REG                             (0x0000D418)
//Duplicate of MC0_IBECC_PROTECT_ADDR_RANGE_0_REG

#define MC0_IBECC_PROTECT_ADDR_RANGE_4_REG                             (0x0000D41C)
//Duplicate of MC0_IBECC_PROTECT_ADDR_RANGE_0_REG

#define MC0_IBECC_PROTECT_ADDR_RANGE_5_REG                             (0x0000D420)
//Duplicate of MC0_IBECC_PROTECT_ADDR_RANGE_0_REG

#define MC0_IBECC_PROTECT_ADDR_RANGE_6_REG                             (0x0000D424)
//Duplicate of MC0_IBECC_PROTECT_ADDR_RANGE_0_REG

#define MC0_IBECC_PROTECT_ADDR_RANGE_7_REG                             (0x0000D428)
//Duplicate of MC0_IBECC_PROTECT_ADDR_RANGE_0_REG

#define MC0_IBECC_DEFAULT_PG_CP_REG                                    (0x0000D430)
#define MC0_IBECC_DEFAULT_PG_CP_DEF                                    (0x0000040001000208)
#define MC0_IBECC_DEFAULT_PG_CP_VOLATILE_BITFIELDS_MSK                 (0x0000000000000000)

  #define MC0_IBECC_DEFAULT_PG_CP_SAI_MASK_OFF                         ( 0)
  #define MC0_IBECC_DEFAULT_PG_CP_SAI_MASK_WID                         (64)
  #define MC0_IBECC_DEFAULT_PG_CP_SAI_MASK_MSK                         (0xFFFFFFFFFFFFFFFFULL)
  #define MC0_IBECC_DEFAULT_PG_CP_SAI_MASK_MIN                         (0)
  #define MC0_IBECC_DEFAULT_PG_CP_SAI_MASK_MAX                         (18446744073709551615ULL) // 0xFFFFFFFFFFFFFFFF
  #define MC0_IBECC_DEFAULT_PG_CP_SAI_MASK_DEF                         (0x40001000208)
  #define MC0_IBECC_DEFAULT_PG_CP_SAI_MASK_HSH                         (0x4000D430)

#define MC0_IBECC_DEFAULT_PG_RAC_REG                                   (0x0000D438)
#define MC0_IBECC_DEFAULT_PG_RAC_DEF                                   (0xFFFFFFFFFFFFFFFF)
#define MC0_IBECC_DEFAULT_PG_RAC_VOLATILE_BITFIELDS_MSK                (0x0000000000000000)

  #define MC0_IBECC_DEFAULT_PG_RAC_SAI_MASK_OFF                        ( 0)
  #define MC0_IBECC_DEFAULT_PG_RAC_SAI_MASK_WID                        (64)
  #define MC0_IBECC_DEFAULT_PG_RAC_SAI_MASK_MSK                        (0xFFFFFFFFFFFFFFFFULL)
  #define MC0_IBECC_DEFAULT_PG_RAC_SAI_MASK_MIN                        (0)
  #define MC0_IBECC_DEFAULT_PG_RAC_SAI_MASK_MAX                        (18446744073709551615ULL) // 0xFFFFFFFFFFFFFFFF
  #define MC0_IBECC_DEFAULT_PG_RAC_SAI_MASK_DEF                        (0xFFFFFFFFFFFFFFFF)
  #define MC0_IBECC_DEFAULT_PG_RAC_SAI_MASK_HSH                        (0x4000D438)

#define MC0_IBECC_DEFAULT_PG_WAC_REG                                   (0x0000D440)
#define MC0_IBECC_DEFAULT_PG_WAC_DEF                                   (0x000004000100061A)
#define MC0_IBECC_DEFAULT_PG_WAC_VOLATILE_BITFIELDS_MSK                (0x0000000000000000)

  #define MC0_IBECC_DEFAULT_PG_WAC_SAI_MASK_OFF                        ( 0)
  #define MC0_IBECC_DEFAULT_PG_WAC_SAI_MASK_WID                        (64)
  #define MC0_IBECC_DEFAULT_PG_WAC_SAI_MASK_MSK                        (0xFFFFFFFFFFFFFFFFULL)
  #define MC0_IBECC_DEFAULT_PG_WAC_SAI_MASK_MIN                        (0)
  #define MC0_IBECC_DEFAULT_PG_WAC_SAI_MASK_MAX                        (18446744073709551615ULL) // 0xFFFFFFFFFFFFFFFF
  #define MC0_IBECC_DEFAULT_PG_WAC_SAI_MASK_DEF                        (0x4000100061A)
  #define MC0_IBECC_DEFAULT_PG_WAC_SAI_MASK_HSH                        (0x4000D440)

#define MC0_IBECC_DEBUG_PG_CP_REG                                      (0x0000D448)
//Duplicate of MC0_IBECC_DEFAULT_PG_CP_REG

#define MC0_IBECC_DEBUG_PG_RAC_REG                                     (0x0000D450)
//Duplicate of MC0_IBECC_DEFAULT_PG_RAC_REG

#define MC0_IBECC_DEBUG_PG_WAC_REG                                     (0x0000D458)
#define MC0_IBECC_DEBUG_PG_WAC_DEF                                     (0x000004000300061A)
#define MC0_IBECC_DEBUG_PG_WAC_VOLATILE_BITFIELDS_MSK                  (0x0000000000000000)

  #define MC0_IBECC_DEBUG_PG_WAC_SAI_MASK_OFF                          ( 0)
  #define MC0_IBECC_DEBUG_PG_WAC_SAI_MASK_WID                          (64)
  #define MC0_IBECC_DEBUG_PG_WAC_SAI_MASK_MSK                          (0xFFFFFFFFFFFFFFFFULL)
  #define MC0_IBECC_DEBUG_PG_WAC_SAI_MASK_MIN                          (0)
  #define MC0_IBECC_DEBUG_PG_WAC_SAI_MASK_MAX                          (18446744073709551615ULL) // 0xFFFFFFFFFFFFFFFF
  #define MC0_IBECC_DEBUG_PG_WAC_SAI_MASK_DEF                          (0x4000300061A)
  #define MC0_IBECC_DEBUG_PG_WAC_SAI_MASK_HSH                          (0x4000D458)

#define MC0_IBECC_OS_PG_CP_REG                                         (0x0000D460)
//Duplicate of MC0_IBECC_DEFAULT_PG_CP_REG

#define MC0_IBECC_OS_PG_RAC_REG                                        (0x0000D468)
//Duplicate of MC0_IBECC_DEFAULT_PG_RAC_REG

#define MC0_IBECC_OS_PG_WAC_REG                                        (0x0000D470)
#define MC0_IBECC_OS_PG_WAC_DEF                                        (0x000004000100061B)
#define MC0_IBECC_OS_PG_WAC_VOLATILE_BITFIELDS_MSK                     (0x0000000000000000)

  #define MC0_IBECC_OS_PG_WAC_SAI_MASK_OFF                             ( 0)
  #define MC0_IBECC_OS_PG_WAC_SAI_MASK_WID                             (64)
  #define MC0_IBECC_OS_PG_WAC_SAI_MASK_MSK                             (0xFFFFFFFFFFFFFFFFULL)
  #define MC0_IBECC_OS_PG_WAC_SAI_MASK_MIN                             (0)
  #define MC0_IBECC_OS_PG_WAC_SAI_MASK_MAX                             (18446744073709551615ULL) // 0xFFFFFFFFFFFFFFFF
  #define MC0_IBECC_OS_PG_WAC_SAI_MASK_DEF                             (0x4000100061B)
  #define MC0_IBECC_OS_PG_WAC_SAI_MASK_HSH                             (0x4000D470)

#define MC0_IBECC_CLK_GATE_EN_0_REG                                    (0x0000D47C)
#define MC0_IBECC_CLK_GATE_EN_0_DEF                                    (0xFFF7FFFF)
#define MC0_IBECC_CLK_GATE_EN_0_VOLATILE_BITFIELDS_MSK                 (0x00000000)

  #define MC0_IBECC_CLK_GATE_EN_0_CLK_GATE_EN_OFF                      ( 0)
  #define MC0_IBECC_CLK_GATE_EN_0_CLK_GATE_EN_WID                      (32)
  #define MC0_IBECC_CLK_GATE_EN_0_CLK_GATE_EN_MSK                      (0xFFFFFFFF)
  #define MC0_IBECC_CLK_GATE_EN_0_CLK_GATE_EN_MIN                      (0)
  #define MC0_IBECC_CLK_GATE_EN_0_CLK_GATE_EN_MAX                      (4294967295) // 0xFFFFFFFF
  #define MC0_IBECC_CLK_GATE_EN_0_CLK_GATE_EN_DEF                      (0xFFF7FFFF)
  #define MC0_IBECC_CLK_GATE_EN_0_CLK_GATE_EN_HSH                      (0x2000D47C)

#define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_REG                     (0x0000D480)
#define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_DEF                     (0x20200000)
#define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_VOLATILE_BITFIELDS_MSK  (0x00000000)

  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_RESERVED0_OFF         ( 0)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_RESERVED0_WID         (16)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_RESERVED0_MSK         (0x0000FFFF)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_RESERVED0_MIN         (0)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_RESERVED0_MAX         (65535) // 0x0000FFFF
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_RESERVED0_DEF         (0x00000000)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_RESERVED0_HSH         (0x1000D480)

  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_REQ_WR_MAX_CREDIT_OFF (16)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_REQ_WR_MAX_CREDIT_WID ( 8)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_REQ_WR_MAX_CREDIT_MSK (0x00FF0000)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_REQ_WR_MAX_CREDIT_MIN (0)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_REQ_WR_MAX_CREDIT_MAX (255) // 0x000000FF
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_REQ_WR_MAX_CREDIT_DEF (0x00000020)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_REQ_WR_MAX_CREDIT_HSH (0x0820D480)

  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_REQ_RD_MAX_CREDIT_OFF (24)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_REQ_RD_MAX_CREDIT_WID ( 8)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_REQ_RD_MAX_CREDIT_MSK (0xFF000000)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_REQ_RD_MAX_CREDIT_MIN (0)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_REQ_RD_MAX_CREDIT_MAX (255) // 0x000000FF
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_REQ_RD_MAX_CREDIT_DEF (0x00000020)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC0_REQ_RD_MAX_CREDIT_HSH (0x0830D480)

#define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_REG                     (0x0000D484)
#define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_DEF                     (0x10100000)
#define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_VOLATILE_BITFIELDS_MSK  (0x00000000)

  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_RESERVED0_OFF         ( 0)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_RESERVED0_WID         (16)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_RESERVED0_MSK         (0x0000FFFF)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_RESERVED0_MIN         (0)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_RESERVED0_MAX         (65535) // 0x0000FFFF
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_RESERVED0_DEF         (0x00000000)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_RESERVED0_HSH         (0x1000D484)

  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_REQ_WR_MAX_CREDIT_OFF (16)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_REQ_WR_MAX_CREDIT_WID ( 8)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_REQ_WR_MAX_CREDIT_MSK (0x00FF0000)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_REQ_WR_MAX_CREDIT_MIN (0)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_REQ_WR_MAX_CREDIT_MAX (255) // 0x000000FF
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_REQ_WR_MAX_CREDIT_DEF (0x00000010)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_REQ_WR_MAX_CREDIT_HSH (0x0820D484)

  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_REQ_RD_MAX_CREDIT_OFF (24)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_REQ_RD_MAX_CREDIT_WID ( 8)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_REQ_RD_MAX_CREDIT_MSK (0xFF000000)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_REQ_RD_MAX_CREDIT_MIN (0)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_REQ_RD_MAX_CREDIT_MAX (255) // 0x000000FF
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_REQ_RD_MAX_CREDIT_DEF (0x00000010)
  #define MC0_IBECC_P0_REQUEST_CREDIT_CONFIG_VC1_REQ_RD_MAX_CREDIT_HSH (0x0830D484)

#define MC0_IBECC_P0_REQUEST_SHARED_CREDIT_CONFIG_REG                  (0x0000D488)
#define MC0_IBECC_P0_REQUEST_SHARED_CREDIT_CONFIG_DEF                  (0x00000020)
#define MC0_IBECC_P0_REQUEST_SHARED_CREDIT_CONFIG_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MC0_IBECC_P0_REQUEST_SHARED_CREDIT_CONFIG_REQ_WR_SHARED_CREDIT_OFF ( 0)
  #define MC0_IBECC_P0_REQUEST_SHARED_CREDIT_CONFIG_REQ_WR_SHARED_CREDIT_WID ( 8)
  #define MC0_IBECC_P0_REQUEST_SHARED_CREDIT_CONFIG_REQ_WR_SHARED_CREDIT_MSK (0x000000FF)
  #define MC0_IBECC_P0_REQUEST_SHARED_CREDIT_CONFIG_REQ_WR_SHARED_CREDIT_MIN (0)
  #define MC0_IBECC_P0_REQUEST_SHARED_CREDIT_CONFIG_REQ_WR_SHARED_CREDIT_MAX (255) // 0x000000FF
  #define MC0_IBECC_P0_REQUEST_SHARED_CREDIT_CONFIG_REQ_WR_SHARED_CREDIT_DEF (0x00000020)
  #define MC0_IBECC_P0_REQUEST_SHARED_CREDIT_CONFIG_REQ_WR_SHARED_CREDIT_HSH (0x0800D488)

  #define MC0_IBECC_P0_REQUEST_SHARED_CREDIT_CONFIG_RESERVED1_OFF      ( 8)
  #define MC0_IBECC_P0_REQUEST_SHARED_CREDIT_CONFIG_RESERVED1_WID      (24)
  #define MC0_IBECC_P0_REQUEST_SHARED_CREDIT_CONFIG_RESERVED1_MSK      (0xFFFFFF00)
  #define MC0_IBECC_P0_REQUEST_SHARED_CREDIT_CONFIG_RESERVED1_MIN      (0)
  #define MC0_IBECC_P0_REQUEST_SHARED_CREDIT_CONFIG_RESERVED1_MAX      (16777215) // 0x00FFFFFF
  #define MC0_IBECC_P0_REQUEST_SHARED_CREDIT_CONFIG_RESERVED1_DEF      (0x00000000)
  #define MC0_IBECC_P0_REQUEST_SHARED_CREDIT_CONFIG_RESERVED1_HSH      (0x1810D488)

#define MC0_IBECC_CFI_BLOCK_REG                                        (0x0000D48C)
#define MC0_IBECC_CFI_BLOCK_DEF                                        (0x00000000)
#define MC0_IBECC_CFI_BLOCK_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define MC0_IBECC_CFI_BLOCK_P0_RX_REQ_BLOCK_OFF                      ( 0)
  #define MC0_IBECC_CFI_BLOCK_P0_RX_REQ_BLOCK_WID                      ( 1)
  #define MC0_IBECC_CFI_BLOCK_P0_RX_REQ_BLOCK_MSK                      (0x00000001)
  #define MC0_IBECC_CFI_BLOCK_P0_RX_REQ_BLOCK_MIN                      (0)
  #define MC0_IBECC_CFI_BLOCK_P0_RX_REQ_BLOCK_MAX                      (1) // 0x00000001
  #define MC0_IBECC_CFI_BLOCK_P0_RX_REQ_BLOCK_DEF                      (0x00000000)
  #define MC0_IBECC_CFI_BLOCK_P0_RX_REQ_BLOCK_HSH                      (0x0100D48C)

  #define MC0_IBECC_CFI_BLOCK_P0_RX_DATA_BLOCK_OFF                     ( 1)
  #define MC0_IBECC_CFI_BLOCK_P0_RX_DATA_BLOCK_WID                     ( 1)
  #define MC0_IBECC_CFI_BLOCK_P0_RX_DATA_BLOCK_MSK                     (0x00000002)
  #define MC0_IBECC_CFI_BLOCK_P0_RX_DATA_BLOCK_MIN                     (0)
  #define MC0_IBECC_CFI_BLOCK_P0_RX_DATA_BLOCK_MAX                     (1) // 0x00000001
  #define MC0_IBECC_CFI_BLOCK_P0_RX_DATA_BLOCK_DEF                     (0x00000000)
  #define MC0_IBECC_CFI_BLOCK_P0_RX_DATA_BLOCK_HSH                     (0x0102D48C)

  #define MC0_IBECC_CFI_BLOCK_P0_TX_RSP_CRD_BLOCK_OFF                  ( 2)
  #define MC0_IBECC_CFI_BLOCK_P0_TX_RSP_CRD_BLOCK_WID                  ( 1)
  #define MC0_IBECC_CFI_BLOCK_P0_TX_RSP_CRD_BLOCK_MSK                  (0x00000004)
  #define MC0_IBECC_CFI_BLOCK_P0_TX_RSP_CRD_BLOCK_MIN                  (0)
  #define MC0_IBECC_CFI_BLOCK_P0_TX_RSP_CRD_BLOCK_MAX                  (1) // 0x00000001
  #define MC0_IBECC_CFI_BLOCK_P0_TX_RSP_CRD_BLOCK_DEF                  (0x00000000)
  #define MC0_IBECC_CFI_BLOCK_P0_TX_RSP_CRD_BLOCK_HSH                  (0x0104D48C)

  #define MC0_IBECC_CFI_BLOCK_P0_TX_DATA_CRD_BLOCK_OFF                 ( 3)
  #define MC0_IBECC_CFI_BLOCK_P0_TX_DATA_CRD_BLOCK_WID                 ( 1)
  #define MC0_IBECC_CFI_BLOCK_P0_TX_DATA_CRD_BLOCK_MSK                 (0x00000008)
  #define MC0_IBECC_CFI_BLOCK_P0_TX_DATA_CRD_BLOCK_MIN                 (0)
  #define MC0_IBECC_CFI_BLOCK_P0_TX_DATA_CRD_BLOCK_MAX                 (1) // 0x00000001
  #define MC0_IBECC_CFI_BLOCK_P0_TX_DATA_CRD_BLOCK_DEF                 (0x00000000)
  #define MC0_IBECC_CFI_BLOCK_P0_TX_DATA_CRD_BLOCK_HSH                 (0x0106D48C)

  #define MC0_IBECC_CFI_BLOCK_P1_RX_RSP_BLOCK_OFF                      ( 4)
  #define MC0_IBECC_CFI_BLOCK_P1_RX_RSP_BLOCK_WID                      ( 1)
  #define MC0_IBECC_CFI_BLOCK_P1_RX_RSP_BLOCK_MSK                      (0x00000010)
  #define MC0_IBECC_CFI_BLOCK_P1_RX_RSP_BLOCK_MIN                      (0)
  #define MC0_IBECC_CFI_BLOCK_P1_RX_RSP_BLOCK_MAX                      (1) // 0x00000001
  #define MC0_IBECC_CFI_BLOCK_P1_RX_RSP_BLOCK_DEF                      (0x00000000)
  #define MC0_IBECC_CFI_BLOCK_P1_RX_RSP_BLOCK_HSH                      (0x0108D48C)

  #define MC0_IBECC_CFI_BLOCK_P1_RX_DATA_BLOCK_OFF                     ( 5)
  #define MC0_IBECC_CFI_BLOCK_P1_RX_DATA_BLOCK_WID                     ( 1)
  #define MC0_IBECC_CFI_BLOCK_P1_RX_DATA_BLOCK_MSK                     (0x00000020)
  #define MC0_IBECC_CFI_BLOCK_P1_RX_DATA_BLOCK_MIN                     (0)
  #define MC0_IBECC_CFI_BLOCK_P1_RX_DATA_BLOCK_MAX                     (1) // 0x00000001
  #define MC0_IBECC_CFI_BLOCK_P1_RX_DATA_BLOCK_DEF                     (0x00000000)
  #define MC0_IBECC_CFI_BLOCK_P1_RX_DATA_BLOCK_HSH                     (0x010AD48C)

  #define MC0_IBECC_CFI_BLOCK_P1_TX_REQ_CRD_BLOCK_OFF                  ( 6)
  #define MC0_IBECC_CFI_BLOCK_P1_TX_REQ_CRD_BLOCK_WID                  ( 1)
  #define MC0_IBECC_CFI_BLOCK_P1_TX_REQ_CRD_BLOCK_MSK                  (0x00000040)
  #define MC0_IBECC_CFI_BLOCK_P1_TX_REQ_CRD_BLOCK_MIN                  (0)
  #define MC0_IBECC_CFI_BLOCK_P1_TX_REQ_CRD_BLOCK_MAX                  (1) // 0x00000001
  #define MC0_IBECC_CFI_BLOCK_P1_TX_REQ_CRD_BLOCK_DEF                  (0x00000000)
  #define MC0_IBECC_CFI_BLOCK_P1_TX_REQ_CRD_BLOCK_HSH                  (0x010CD48C)

  #define MC0_IBECC_CFI_BLOCK_P1_TX_DATA_CRD_BLOCK_OFF                 ( 7)
  #define MC0_IBECC_CFI_BLOCK_P1_TX_DATA_CRD_BLOCK_WID                 ( 1)
  #define MC0_IBECC_CFI_BLOCK_P1_TX_DATA_CRD_BLOCK_MSK                 (0x00000080)
  #define MC0_IBECC_CFI_BLOCK_P1_TX_DATA_CRD_BLOCK_MIN                 (0)
  #define MC0_IBECC_CFI_BLOCK_P1_TX_DATA_CRD_BLOCK_MAX                 (1) // 0x00000001
  #define MC0_IBECC_CFI_BLOCK_P1_TX_DATA_CRD_BLOCK_DEF                 (0x00000000)
  #define MC0_IBECC_CFI_BLOCK_P1_TX_DATA_CRD_BLOCK_HSH                 (0x010ED48C)

  #define MC0_IBECC_CFI_BLOCK_RSVD_OFF                                 ( 8)
  #define MC0_IBECC_CFI_BLOCK_RSVD_WID                                 (24)
  #define MC0_IBECC_CFI_BLOCK_RSVD_MSK                                 (0xFFFFFF00)
  #define MC0_IBECC_CFI_BLOCK_RSVD_MIN                                 (0)
  #define MC0_IBECC_CFI_BLOCK_RSVD_MAX                                 (16777215) // 0x00FFFFFF
  #define MC0_IBECC_CFI_BLOCK_RSVD_DEF                                 (0x00000000)
  #define MC0_IBECC_CFI_BLOCK_RSVD_HSH                                 (0x1810D48C)

#define MC0_IBECC_MBIST_POST_CTRL_WDB_REG                              (0x0000D490)
#define MC0_IBECC_MBIST_POST_CTRL_WDB_DEF                              (0x00000000)
#define MC0_IBECC_MBIST_POST_CTRL_WDB_VOLATILE_BITFIELDS_MSK           (0x80000003)

  #define MC0_IBECC_MBIST_POST_CTRL_WDB_PASS_OFF                       ( 0)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_PASS_WID                       ( 1)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_PASS_MSK                       (0x00000001)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_PASS_MIN                       (0)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_PASS_MAX                       (1) // 0x00000001
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_PASS_DEF                       (0x00000000)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_PASS_HSH                       (0x0180D490)

  #define MC0_IBECC_MBIST_POST_CTRL_WDB_COMPLETE_OFF                   ( 1)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_COMPLETE_WID                   ( 1)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_COMPLETE_MSK                   (0x00000002)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_COMPLETE_MIN                   (0)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_COMPLETE_MAX                   (1) // 0x00000001
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_COMPLETE_DEF                   (0x00000000)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_COMPLETE_HSH                   (0x0182D490)

  #define MC0_IBECC_MBIST_POST_CTRL_WDB_RSVD1_OFF                      ( 2)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_RSVD1_WID                      ( 6)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_RSVD1_MSK                      (0x000000FC)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_RSVD1_MIN                      (0)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_RSVD1_MAX                      (63) // 0x0000003F
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_RSVD1_DEF                      (0x00000000)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_RSVD1_HSH                      (0x0604D490)

  #define MC0_IBECC_MBIST_POST_CTRL_WDB_MBIST_FAIL_INJECT_OFF          ( 8)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_MBIST_FAIL_INJECT_WID          ( 1)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_MBIST_FAIL_INJECT_MSK          (0x00000100)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_MBIST_FAIL_INJECT_MIN          (0)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_MBIST_FAIL_INJECT_MAX          (1) // 0x00000001
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_MBIST_FAIL_INJECT_DEF          (0x00000000)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_MBIST_FAIL_INJECT_HSH          (0x0110D490)

  #define MC0_IBECC_MBIST_POST_CTRL_WDB_RSVD2_OFF                      ( 9)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_RSVD2_WID                      (22)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_RSVD2_MSK                      (0x7FFFFE00)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_RSVD2_MIN                      (0)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_RSVD2_MAX                      (4194303) // 0x003FFFFF
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_RSVD2_DEF                      (0x00000000)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_RSVD2_HSH                      (0x1612D490)

  #define MC0_IBECC_MBIST_POST_CTRL_WDB_RUN_BUSY_OFF                   (31)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_RUN_BUSY_WID                   ( 1)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_RUN_BUSY_MSK                   (0x80000000)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_RUN_BUSY_MIN                   (0)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_RUN_BUSY_MAX                   (1) // 0x00000001
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_RUN_BUSY_DEF                   (0x00000000)
  #define MC0_IBECC_MBIST_POST_CTRL_WDB_RUN_BUSY_HSH                   (0x01BED490)

#define MC0_IBECC_MBIST_POST_CTRL_RDB_REG                              (0x0000D494)
//Duplicate of MC0_IBECC_MBIST_POST_CTRL_WDB_REG

#define MC0_IBECC_PARITY_CONTROL_REG                                   (0x0000D498)
#define MC0_IBECC_PARITY_CONTROL_DEF                                   (0x0000001F)
#define MC0_IBECC_PARITY_CONTROL_VOLATILE_BITFIELDS_MSK                (0x00000000)

  #define MC0_IBECC_PARITY_CONTROL_RADDR_PARITY_CHK_EN_OFF             ( 0)
  #define MC0_IBECC_PARITY_CONTROL_RADDR_PARITY_CHK_EN_WID             ( 1)
  #define MC0_IBECC_PARITY_CONTROL_RADDR_PARITY_CHK_EN_MSK             (0x00000001)
  #define MC0_IBECC_PARITY_CONTROL_RADDR_PARITY_CHK_EN_MIN             (0)
  #define MC0_IBECC_PARITY_CONTROL_RADDR_PARITY_CHK_EN_MAX             (1) // 0x00000001
  #define MC0_IBECC_PARITY_CONTROL_RADDR_PARITY_CHK_EN_DEF             (0x00000001)
  #define MC0_IBECC_PARITY_CONTROL_RADDR_PARITY_CHK_EN_HSH             (0x0100D498)

  #define MC0_IBECC_PARITY_CONTROL_RDATA_PARITY_CHK_EN_OFF             ( 1)
  #define MC0_IBECC_PARITY_CONTROL_RDATA_PARITY_CHK_EN_WID             ( 1)
  #define MC0_IBECC_PARITY_CONTROL_RDATA_PARITY_CHK_EN_MSK             (0x00000002)
  #define MC0_IBECC_PARITY_CONTROL_RDATA_PARITY_CHK_EN_MIN             (0)
  #define MC0_IBECC_PARITY_CONTROL_RDATA_PARITY_CHK_EN_MAX             (1) // 0x00000001
  #define MC0_IBECC_PARITY_CONTROL_RDATA_PARITY_CHK_EN_DEF             (0x00000001)
  #define MC0_IBECC_PARITY_CONTROL_RDATA_PARITY_CHK_EN_HSH             (0x0102D498)

  #define MC0_IBECC_PARITY_CONTROL_RHEADER_PARITY_CHK_EN_OFF           ( 2)
  #define MC0_IBECC_PARITY_CONTROL_RHEADER_PARITY_CHK_EN_WID           ( 1)
  #define MC0_IBECC_PARITY_CONTROL_RHEADER_PARITY_CHK_EN_MSK           (0x00000004)
  #define MC0_IBECC_PARITY_CONTROL_RHEADER_PARITY_CHK_EN_MIN           (0)
  #define MC0_IBECC_PARITY_CONTROL_RHEADER_PARITY_CHK_EN_MAX           (1) // 0x00000001
  #define MC0_IBECC_PARITY_CONTROL_RHEADER_PARITY_CHK_EN_DEF           (0x00000001)
  #define MC0_IBECC_PARITY_CONTROL_RHEADER_PARITY_CHK_EN_HSH           (0x0104D498)

  #define MC0_IBECC_PARITY_CONTROL_WHEADER_PARITY_CHK_EN_OFF           ( 3)
  #define MC0_IBECC_PARITY_CONTROL_WHEADER_PARITY_CHK_EN_WID           ( 1)
  #define MC0_IBECC_PARITY_CONTROL_WHEADER_PARITY_CHK_EN_MSK           (0x00000008)
  #define MC0_IBECC_PARITY_CONTROL_WHEADER_PARITY_CHK_EN_MIN           (0)
  #define MC0_IBECC_PARITY_CONTROL_WHEADER_PARITY_CHK_EN_MAX           (1) // 0x00000001
  #define MC0_IBECC_PARITY_CONTROL_WHEADER_PARITY_CHK_EN_DEF           (0x00000001)
  #define MC0_IBECC_PARITY_CONTROL_WHEADER_PARITY_CHK_EN_HSH           (0x0106D498)

  #define MC0_IBECC_PARITY_CONTROL_WDATA_PARITY_CHK_EN_OFF             ( 4)
  #define MC0_IBECC_PARITY_CONTROL_WDATA_PARITY_CHK_EN_WID             ( 1)
  #define MC0_IBECC_PARITY_CONTROL_WDATA_PARITY_CHK_EN_MSK             (0x00000010)
  #define MC0_IBECC_PARITY_CONTROL_WDATA_PARITY_CHK_EN_MIN             (0)
  #define MC0_IBECC_PARITY_CONTROL_WDATA_PARITY_CHK_EN_MAX             (1) // 0x00000001
  #define MC0_IBECC_PARITY_CONTROL_WDATA_PARITY_CHK_EN_DEF             (0x00000001)
  #define MC0_IBECC_PARITY_CONTROL_WDATA_PARITY_CHK_EN_HSH             (0x0108D498)

  #define MC0_IBECC_PARITY_CONTROL_RSVD0_OFF                           ( 5)
  #define MC0_IBECC_PARITY_CONTROL_RSVD0_WID                           ( 3)
  #define MC0_IBECC_PARITY_CONTROL_RSVD0_MSK                           (0x000000E0)
  #define MC0_IBECC_PARITY_CONTROL_RSVD0_MIN                           (0)
  #define MC0_IBECC_PARITY_CONTROL_RSVD0_MAX                           (7) // 0x00000007
  #define MC0_IBECC_PARITY_CONTROL_RSVD0_DEF                           (0x00000000)
  #define MC0_IBECC_PARITY_CONTROL_RSVD0_HSH                           (0x030AD498)

  #define MC0_IBECC_PARITY_CONTROL_DIS_PARITY_PCIE_ERROR_OFF           ( 8)
  #define MC0_IBECC_PARITY_CONTROL_DIS_PARITY_PCIE_ERROR_WID           ( 1)
  #define MC0_IBECC_PARITY_CONTROL_DIS_PARITY_PCIE_ERROR_MSK           (0x00000100)
  #define MC0_IBECC_PARITY_CONTROL_DIS_PARITY_PCIE_ERROR_MIN           (0)
  #define MC0_IBECC_PARITY_CONTROL_DIS_PARITY_PCIE_ERROR_MAX           (1) // 0x00000001
  #define MC0_IBECC_PARITY_CONTROL_DIS_PARITY_PCIE_ERROR_DEF           (0x00000000)
  #define MC0_IBECC_PARITY_CONTROL_DIS_PARITY_PCIE_ERROR_HSH           (0x0110D498)

  #define MC0_IBECC_PARITY_CONTROL_DIS_PARITY_LOG_OFF                  ( 9)
  #define MC0_IBECC_PARITY_CONTROL_DIS_PARITY_LOG_WID                  ( 1)
  #define MC0_IBECC_PARITY_CONTROL_DIS_PARITY_LOG_MSK                  (0x00000200)
  #define MC0_IBECC_PARITY_CONTROL_DIS_PARITY_LOG_MIN                  (0)
  #define MC0_IBECC_PARITY_CONTROL_DIS_PARITY_LOG_MAX                  (1) // 0x00000001
  #define MC0_IBECC_PARITY_CONTROL_DIS_PARITY_LOG_DEF                  (0x00000000)
  #define MC0_IBECC_PARITY_CONTROL_DIS_PARITY_LOG_HSH                  (0x0112D498)

  #define MC0_IBECC_PARITY_CONTROL_RSVD1_OFF                           (10)
  #define MC0_IBECC_PARITY_CONTROL_RSVD1_WID                           (21)
  #define MC0_IBECC_PARITY_CONTROL_RSVD1_MSK                           (0x7FFFFC00)
  #define MC0_IBECC_PARITY_CONTROL_RSVD1_MIN                           (0)
  #define MC0_IBECC_PARITY_CONTROL_RSVD1_MAX                           (2097151) // 0x001FFFFF
  #define MC0_IBECC_PARITY_CONTROL_RSVD1_DEF                           (0x00000000)
  #define MC0_IBECC_PARITY_CONTROL_RSVD1_HSH                           (0x1514D498)

  #define MC0_IBECC_PARITY_CONTROL_PARITY_CHK_EN_OFF                   (31)
  #define MC0_IBECC_PARITY_CONTROL_PARITY_CHK_EN_WID                   ( 1)
  #define MC0_IBECC_PARITY_CONTROL_PARITY_CHK_EN_MSK                   (0x80000000)
  #define MC0_IBECC_PARITY_CONTROL_PARITY_CHK_EN_MIN                   (0)
  #define MC0_IBECC_PARITY_CONTROL_PARITY_CHK_EN_MAX                   (1) // 0x00000001
  #define MC0_IBECC_PARITY_CONTROL_PARITY_CHK_EN_DEF                   (0x00000000)
  #define MC0_IBECC_PARITY_CONTROL_PARITY_CHK_EN_HSH                   (0x013ED498)

#define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_REG                    (0x0000D500)
#define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_DEF                    (0x18180000)
#define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_RESERVED0_OFF        ( 0)
  #define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_RESERVED0_WID        (16)
  #define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_RESERVED0_MSK        (0x0000FFFF)
  #define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_RESERVED0_MIN        (0)
  #define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_RESERVED0_MAX        (65535) // 0x0000FFFF
  #define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_RESERVED0_DEF        (0x00000000)
  #define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_RESERVED0_HSH        (0x1000D500)

  #define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_WR_RSP_MAX_CREDIT_OFF (16)
  #define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_WR_RSP_MAX_CREDIT_WID ( 8)
  #define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_WR_RSP_MAX_CREDIT_MSK (0x00FF0000)
  #define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_WR_RSP_MAX_CREDIT_MIN (0)
  #define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_WR_RSP_MAX_CREDIT_MAX (255) // 0x000000FF
  #define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_WR_RSP_MAX_CREDIT_DEF (0x00000018)
  #define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_WR_RSP_MAX_CREDIT_HSH (0x0820D500)

  #define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_RD_DATA_MAX_CREDIT_OFF (24)
  #define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_RD_DATA_MAX_CREDIT_WID ( 8)
  #define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_RD_DATA_MAX_CREDIT_MSK (0xFF000000)
  #define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_RD_DATA_MAX_CREDIT_MIN (0)
  #define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_RD_DATA_MAX_CREDIT_MAX (255) // 0x000000FF
  #define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_RD_DATA_MAX_CREDIT_DEF (0x00000018)
  #define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_RD_DATA_MAX_CREDIT_HSH (0x0830D500)

#define MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC1_REG                    (0x0000D504)
//Duplicate of MC0_IBECC_P1_RESPONSE_CREDIT_CONFIG_VC0_REG

#define MC0_IBECC_CFI_PORT_CONTROL_REG                                 (0x0000D508)
#define MC0_IBECC_CFI_PORT_CONTROL_DEF                                 (0x00004F86)
#define MC0_IBECC_CFI_PORT_CONTROL_VOLATILE_BITFIELDS_MSK              (0x00000000)

  #define MC0_IBECC_CFI_PORT_CONTROL_force_cfi_clkreq_OFF              ( 0)
  #define MC0_IBECC_CFI_PORT_CONTROL_force_cfi_clkreq_WID              ( 1)
  #define MC0_IBECC_CFI_PORT_CONTROL_force_cfi_clkreq_MSK              (0x00000001)
  #define MC0_IBECC_CFI_PORT_CONTROL_force_cfi_clkreq_MIN              (0)
  #define MC0_IBECC_CFI_PORT_CONTROL_force_cfi_clkreq_MAX              (1) // 0x00000001
  #define MC0_IBECC_CFI_PORT_CONTROL_force_cfi_clkreq_DEF              (0x00000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_force_cfi_clkreq_HSH              (0x0100D508)

  #define MC0_IBECC_CFI_PORT_CONTROL_peer_clkack_bypass_OFF            ( 1)
  #define MC0_IBECC_CFI_PORT_CONTROL_peer_clkack_bypass_WID            ( 1)
  #define MC0_IBECC_CFI_PORT_CONTROL_peer_clkack_bypass_MSK            (0x00000002)
  #define MC0_IBECC_CFI_PORT_CONTROL_peer_clkack_bypass_MIN            (0)
  #define MC0_IBECC_CFI_PORT_CONTROL_peer_clkack_bypass_MAX            (1) // 0x00000001
  #define MC0_IBECC_CFI_PORT_CONTROL_peer_clkack_bypass_DEF            (0x00000001)
  #define MC0_IBECC_CFI_PORT_CONTROL_peer_clkack_bypass_HSH            (0x0102D508)

  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_clkack_bypass_OFF             ( 2)
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_clkack_bypass_WID             ( 1)
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_clkack_bypass_MSK             (0x00000004)
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_clkack_bypass_MIN             (0)
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_clkack_bypass_MAX             (1) // 0x00000001
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_clkack_bypass_DEF             (0x00000001)
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_clkack_bypass_HSH             (0x0104D508)

  #define MC0_IBECC_CFI_PORT_CONTROL_force_peerclkreq_OFF              ( 3)
  #define MC0_IBECC_CFI_PORT_CONTROL_force_peerclkreq_WID              ( 1)
  #define MC0_IBECC_CFI_PORT_CONTROL_force_peerclkreq_MSK              (0x00000008)
  #define MC0_IBECC_CFI_PORT_CONTROL_force_peerclkreq_MIN              (0)
  #define MC0_IBECC_CFI_PORT_CONTROL_force_peerclkreq_MAX              (1) // 0x00000001
  #define MC0_IBECC_CFI_PORT_CONTROL_force_peerclkreq_DEF              (0x00000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_force_peerclkreq_HSH              (0x0106D508)

  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_link_mode_ovr_en_OFF          ( 4)
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_link_mode_ovr_en_WID          ( 1)
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_link_mode_ovr_en_MSK          (0x00000010)
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_link_mode_ovr_en_MIN          (0)
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_link_mode_ovr_en_MAX          (1) // 0x00000001
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_link_mode_ovr_en_DEF          (0x00000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_link_mode_ovr_en_HSH          (0x0108D508)

  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_link_mode_val_OFF             ( 5)
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_link_mode_val_WID             ( 1)
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_link_mode_val_MSK             (0x00000020)
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_link_mode_val_MIN             (0)
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_link_mode_val_MAX             (1) // 0x00000001
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_link_mode_val_DEF             (0x00000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_link_mode_val_HSH             (0x010AD508)

  #define MC0_IBECC_CFI_PORT_CONTROL_RSVD_1_OFF                        ( 6)
  #define MC0_IBECC_CFI_PORT_CONTROL_RSVD_1_WID                        ( 1)
  #define MC0_IBECC_CFI_PORT_CONTROL_RSVD_1_MSK                        (0x00000040)
  #define MC0_IBECC_CFI_PORT_CONTROL_RSVD_1_MIN                        (0)
  #define MC0_IBECC_CFI_PORT_CONTROL_RSVD_1_MAX                        (1) // 0x00000001
  #define MC0_IBECC_CFI_PORT_CONTROL_RSVD_1_DEF                        (0x00000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_RSVD_1_HSH                        (0x010CD508)

  #define MC0_IBECC_CFI_PORT_CONTROL_Cfi_clk_HYST_threshold_OFF        ( 7)
  #define MC0_IBECC_CFI_PORT_CONTROL_Cfi_clk_HYST_threshold_WID        ( 7)
  #define MC0_IBECC_CFI_PORT_CONTROL_Cfi_clk_HYST_threshold_MSK        (0x00003F80)
  #define MC0_IBECC_CFI_PORT_CONTROL_Cfi_clk_HYST_threshold_MIN        (0)
  #define MC0_IBECC_CFI_PORT_CONTROL_Cfi_clk_HYST_threshold_MAX        (127) // 0x0000007F
  #define MC0_IBECC_CFI_PORT_CONTROL_Cfi_clk_HYST_threshold_DEF        (0x0000001F)
  #define MC0_IBECC_CFI_PORT_CONTROL_Cfi_clk_HYST_threshold_HSH        (0x070ED508)

  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_link_clk_detect_hold_OFF      (14)
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_link_clk_detect_hold_WID      ( 4)
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_link_clk_detect_hold_MSK      (0x0003C000)
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_link_clk_detect_hold_MIN      (0)
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_link_clk_detect_hold_MAX      (15) // 0x0000000F
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_link_clk_detect_hold_DEF      (0x00000001)
  #define MC0_IBECC_CFI_PORT_CONTROL_cfi_link_clk_detect_hold_HSH      (0x041CD508)

  #define MC0_IBECC_CFI_PORT_CONTROL_vc1_read_prio_in_mask_OFF         (18)
  #define MC0_IBECC_CFI_PORT_CONTROL_vc1_read_prio_in_mask_WID         ( 1)
  #define MC0_IBECC_CFI_PORT_CONTROL_vc1_read_prio_in_mask_MSK         (0x00040000)
  #define MC0_IBECC_CFI_PORT_CONTROL_vc1_read_prio_in_mask_MIN         (0)
  #define MC0_IBECC_CFI_PORT_CONTROL_vc1_read_prio_in_mask_MAX         (1) // 0x00000001
  #define MC0_IBECC_CFI_PORT_CONTROL_vc1_read_prio_in_mask_DEF         (0x00000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_vc1_read_prio_in_mask_HSH         (0x0124D508)

  #define MC0_IBECC_CFI_PORT_CONTROL_vc1_write_prio_in_mask_OFF        (19)
  #define MC0_IBECC_CFI_PORT_CONTROL_vc1_write_prio_in_mask_WID        ( 1)
  #define MC0_IBECC_CFI_PORT_CONTROL_vc1_write_prio_in_mask_MSK        (0x00080000)
  #define MC0_IBECC_CFI_PORT_CONTROL_vc1_write_prio_in_mask_MIN        (0)
  #define MC0_IBECC_CFI_PORT_CONTROL_vc1_write_prio_in_mask_MAX        (1) // 0x00000001
  #define MC0_IBECC_CFI_PORT_CONTROL_vc1_write_prio_in_mask_DEF        (0x00000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_vc1_write_prio_in_mask_HSH        (0x0126D508)

  #define MC0_IBECC_CFI_PORT_CONTROL_P0_disconnect_OFF                 (20)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_disconnect_WID                 ( 1)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_disconnect_MSK                 (0x00100000)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_disconnect_MIN                 (0)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_disconnect_MAX                 (1) // 0x00000001
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_disconnect_DEF                 (0x00000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_disconnect_HSH                 (0x0128D508)

  #define MC0_IBECC_CFI_PORT_CONTROL_P0_connect_OFF                    (21)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_connect_WID                    ( 1)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_connect_MSK                    (0x00200000)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_connect_MIN                    (0)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_connect_MAX                    (1) // 0x00000001
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_connect_DEF                    (0x00000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_connect_HSH                    (0x012AD508)

  #define MC0_IBECC_CFI_PORT_CONTROL_P0_RX_block_con_OFF               (22)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_RX_block_con_WID               ( 1)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_RX_block_con_MSK               (0x00400000)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_RX_block_con_MIN               (0)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_RX_block_con_MAX               (1) // 0x00000001
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_RX_block_con_DEF               (0x00000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_RX_block_con_HSH               (0x012CD508)

  #define MC0_IBECC_CFI_PORT_CONTROL_P1_disconnect_OFF                 (23)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_disconnect_WID                 ( 1)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_disconnect_MSK                 (0x00800000)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_disconnect_MIN                 (0)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_disconnect_MAX                 (1) // 0x00000001
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_disconnect_DEF                 (0x00000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_disconnect_HSH                 (0x012ED508)

  #define MC0_IBECC_CFI_PORT_CONTROL_P1_connect_OFF                    (24)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_connect_WID                    ( 1)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_connect_MSK                    (0x01000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_connect_MIN                    (0)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_connect_MAX                    (1) // 0x00000001
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_connect_DEF                    (0x00000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_connect_HSH                    (0x0130D508)

  #define MC0_IBECC_CFI_PORT_CONTROL_P1_RX_block_con_OFF               (25)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_RX_block_con_WID               ( 1)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_RX_block_con_MSK               (0x02000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_RX_block_con_MIN               (0)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_RX_block_con_MAX               (1) // 0x00000001
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_RX_block_con_DEF               (0x00000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_RX_block_con_HSH               (0x0132D508)

  #define MC0_IBECC_CFI_PORT_CONTROL_P1_TxDATACntEn_OFF                (26)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_TxDATACntEn_WID                ( 1)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_TxDATACntEn_MSK                (0x04000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_TxDATACntEn_MIN                (0)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_TxDATACntEn_MAX                (1) // 0x00000001
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_TxDATACntEn_DEF                (0x00000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_TxDATACntEn_HSH                (0x0134D508)

  #define MC0_IBECC_CFI_PORT_CONTROL_P0_TxDATACntEn_OFF                (27)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_TxDATACntEn_WID                ( 1)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_TxDATACntEn_MSK                (0x08000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_TxDATACntEn_MIN                (0)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_TxDATACntEn_MAX                (1) // 0x00000001
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_TxDATACntEn_DEF                (0x00000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_TxDATACntEn_HSH                (0x0136D508)

  #define MC0_IBECC_CFI_PORT_CONTROL_P1_RxDATACntEn_OFF                (28)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_RxDATACntEn_WID                ( 1)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_RxDATACntEn_MSK                (0x10000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_RxDATACntEn_MIN                (0)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_RxDATACntEn_MAX                (1) // 0x00000001
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_RxDATACntEn_DEF                (0x00000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_P1_RxDATACntEn_HSH                (0x0138D508)

  #define MC0_IBECC_CFI_PORT_CONTROL_P0_RxDATACntEn_OFF                (29)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_RxDATACntEn_WID                ( 1)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_RxDATACntEn_MSK                (0x20000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_RxDATACntEn_MIN                (0)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_RxDATACntEn_MAX                (1) // 0x00000001
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_RxDATACntEn_DEF                (0x00000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_P0_RxDATACntEn_HSH                (0x013AD508)

  #define MC0_IBECC_CFI_PORT_CONTROL_RSVD_0_OFF                        (30)
  #define MC0_IBECC_CFI_PORT_CONTROL_RSVD_0_WID                        ( 2)
  #define MC0_IBECC_CFI_PORT_CONTROL_RSVD_0_MSK                        (0xC0000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_RSVD_0_MIN                        (0)
  #define MC0_IBECC_CFI_PORT_CONTROL_RSVD_0_MAX                        (3) // 0x00000003
  #define MC0_IBECC_CFI_PORT_CONTROL_RSVD_0_DEF                        (0x00000000)
  #define MC0_IBECC_CFI_PORT_CONTROL_RSVD_0_HSH                        (0x023CD508)

#define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_REG                     (0x0000D50C)
#define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_DEF                     (0x0000000C)
#define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_VOLATILE_BITFIELDS_MSK  (0x00000000)

  #define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_GRACE_CNTR_MAX_OFF    ( 0)
  #define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_GRACE_CNTR_MAX_WID    ( 8)
  #define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_GRACE_CNTR_MAX_MSK    (0x000000FF)
  #define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_GRACE_CNTR_MAX_MIN    (0)
  #define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_GRACE_CNTR_MAX_MAX    (255) // 0x000000FF
  #define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_GRACE_CNTR_MAX_DEF    (0x0000000C)
  #define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_GRACE_CNTR_MAX_HSH    (0x0800D50C)

  #define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_CLKGATE_DISABLE_OFF   ( 8)
  #define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_CLKGATE_DISABLE_WID   ( 1)
  #define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_CLKGATE_DISABLE_MSK   (0x00000100)
  #define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_CLKGATE_DISABLE_MIN   (0)
  #define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_CLKGATE_DISABLE_MAX   (1) // 0x00000001
  #define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_CLKGATE_DISABLE_DEF   (0x00000000)
  #define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_CLKGATE_DISABLE_HSH   (0x0110D50C)

  #define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_RSVD_OFF              ( 9)
  #define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_RSVD_WID              (23)
  #define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_RSVD_MSK              (0xFFFFFE00)
  #define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_RSVD_MIN              (0)
  #define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_RSVD_MAX              (8388607) // 0x007FFFFF
  #define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_RSVD_DEF              (0x00000000)
  #define MC0_IBECC_QCLK_GLB_DRV_CLKGATE_CONTROL_RSVD_HSH              (0x1712D50C)

#define MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG                              (0x0000D520)
#define MC0_IBECC_ECC_VC0_RD_REQCOUNT_DEF                              (0x0000000000000000)
#define MC0_IBECC_ECC_VC0_RD_REQCOUNT_VOLATILE_BITFIELDS_MSK           (0xFFFFFFFFFFFFFFFF)

  #define MC0_IBECC_ECC_VC0_RD_REQCOUNT_COUNT_OFF                      ( 0)
  #define MC0_IBECC_ECC_VC0_RD_REQCOUNT_COUNT_WID                      (64)
  #define MC0_IBECC_ECC_VC0_RD_REQCOUNT_COUNT_MSK                      (0xFFFFFFFFFFFFFFFFULL)
  #define MC0_IBECC_ECC_VC0_RD_REQCOUNT_COUNT_MIN                      (0)
  #define MC0_IBECC_ECC_VC0_RD_REQCOUNT_COUNT_MAX                      (18446744073709551615ULL) // 0xFFFFFFFFFFFFFFFF
  #define MC0_IBECC_ECC_VC0_RD_REQCOUNT_COUNT_DEF                      (0x00000000)
  #define MC0_IBECC_ECC_VC0_RD_REQCOUNT_COUNT_HSH                      (0x4080D520)

#define MC0_IBECC_ECC_VC1_RD_REQCOUNT_REG                              (0x0000D528)
//Duplicate of MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG

#define MC0_IBECC_ECC_VC0_WR_REQCOUNT_REG                              (0x0000D530)
//Duplicate of MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG

#define MC0_IBECC_ECC_VC1_WR_REQCOUNT_REG                              (0x0000D538)
//Duplicate of MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG

#define MC0_IBECC_NOECC_VC0_RD_REQCOUNT_REG                            (0x0000D540)
//Duplicate of MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG

#define MC0_IBECC_NOECC_VC1_RD_REQCOUNT_REG                            (0x0000D548)
//Duplicate of MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG

#define MC0_IBECC_NOECC_VC0_WR_REQCOUNT_REG                            (0x0000D550)
//Duplicate of MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG

#define MC0_IBECC_NOECC_VC1_WR_REQCOUNT_REG                            (0x0000D558)
//Duplicate of MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG

#define MC0_IBECC_REQUEST_ARB_CONFIG_REG                               (0x0000D564)
#define MC0_IBECC_REQUEST_ARB_CONFIG_DEF                               (0x00003333)
#define MC0_IBECC_REQUEST_ARB_CONFIG_VOLATILE_BITFIELDS_MSK            (0x00000000)

  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC0_RD_PARK_SIZE_OFF            ( 0)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC0_RD_PARK_SIZE_WID            ( 4)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC0_RD_PARK_SIZE_MSK            (0x0000000F)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC0_RD_PARK_SIZE_MIN            (0)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC0_RD_PARK_SIZE_MAX            (15) // 0x0000000F
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC0_RD_PARK_SIZE_DEF            (0x00000003)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC0_RD_PARK_SIZE_HSH            (0x0400D564)

  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC1_RD_PARK_SIZE_OFF            ( 4)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC1_RD_PARK_SIZE_WID            ( 4)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC1_RD_PARK_SIZE_MSK            (0x000000F0)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC1_RD_PARK_SIZE_MIN            (0)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC1_RD_PARK_SIZE_MAX            (15) // 0x0000000F
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC1_RD_PARK_SIZE_DEF            (0x00000003)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC1_RD_PARK_SIZE_HSH            (0x0408D564)

  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC0_WR_PARK_SIZE_OFF            ( 8)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC0_WR_PARK_SIZE_WID            ( 4)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC0_WR_PARK_SIZE_MSK            (0x00000F00)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC0_WR_PARK_SIZE_MIN            (0)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC0_WR_PARK_SIZE_MAX            (15) // 0x0000000F
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC0_WR_PARK_SIZE_DEF            (0x00000003)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC0_WR_PARK_SIZE_HSH            (0x0410D564)

  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC1_WR_PARK_SIZE_OFF            (12)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC1_WR_PARK_SIZE_WID            ( 4)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC1_WR_PARK_SIZE_MSK            (0x0000F000)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC1_WR_PARK_SIZE_MIN            (0)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC1_WR_PARK_SIZE_MAX            (15) // 0x0000000F
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC1_WR_PARK_SIZE_DEF            (0x00000003)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_VC1_WR_PARK_SIZE_HSH            (0x0418D564)

  #define MC0_IBECC_REQUEST_ARB_CONFIG_RSVD_OFF                        (16)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_RSVD_WID                        (16)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_RSVD_MSK                        (0xFFFF0000)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_RSVD_MIN                        (0)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_RSVD_MAX                        (65535) // 0x0000FFFF
  #define MC0_IBECC_REQUEST_ARB_CONFIG_RSVD_DEF                        (0x00000000)
  #define MC0_IBECC_REQUEST_ARB_CONFIG_RSVD_HSH                        (0x1020D564)

#define MC0_IBECC_RESPONSE_ARB_CONFIG_REG                              (0x0000D568)
//Duplicate of MC0_IBECC_REQUEST_ARB_CONFIG_REG

#define MC0_IBECC_ECC_ERROR_LOG_REG                                    (0x0000D570)
#define MC0_IBECC_ECC_ERROR_LOG_DEF                                    (0x0000000000000000)
#define MC0_IBECC_ECC_ERROR_LOG_VOLATILE_BITFIELDS_MSK                 (0xFFFFC07FFFFFFFF8)

  #define MC0_IBECC_ECC_ERROR_LOG_RSVD0_OFF                            ( 0)
  #define MC0_IBECC_ECC_ERROR_LOG_RSVD0_WID                            ( 3)
  #define MC0_IBECC_ECC_ERROR_LOG_RSVD0_MSK                            (0x00000007)
  #define MC0_IBECC_ECC_ERROR_LOG_RSVD0_MIN                            (0)
  #define MC0_IBECC_ECC_ERROR_LOG_RSVD0_MAX                            (7) // 0x00000007
  #define MC0_IBECC_ECC_ERROR_LOG_RSVD0_DEF                            (0x00000000)
  #define MC0_IBECC_ECC_ERROR_LOG_RSVD0_HSH                            (0x4300D570)

  #define MC0_IBECC_ECC_ERROR_LOG_CERR_OVERFLOW_OFF                    ( 3)
  #define MC0_IBECC_ECC_ERROR_LOG_CERR_OVERFLOW_WID                    ( 1)
  #define MC0_IBECC_ECC_ERROR_LOG_CERR_OVERFLOW_MSK                    (0x00000008)
  #define MC0_IBECC_ECC_ERROR_LOG_CERR_OVERFLOW_MIN                    (0)
  #define MC0_IBECC_ECC_ERROR_LOG_CERR_OVERFLOW_MAX                    (1) // 0x00000001
  #define MC0_IBECC_ECC_ERROR_LOG_CERR_OVERFLOW_DEF                    (0x00000000)
  #define MC0_IBECC_ECC_ERROR_LOG_CERR_OVERFLOW_HSH                    (0x4186D570)

  #define MC0_IBECC_ECC_ERROR_LOG_MERR_OVERFLOW_OFF                    ( 4)
  #define MC0_IBECC_ECC_ERROR_LOG_MERR_OVERFLOW_WID                    ( 1)
  #define MC0_IBECC_ECC_ERROR_LOG_MERR_OVERFLOW_MSK                    (0x00000010)
  #define MC0_IBECC_ECC_ERROR_LOG_MERR_OVERFLOW_MIN                    (0)
  #define MC0_IBECC_ECC_ERROR_LOG_MERR_OVERFLOW_MAX                    (1) // 0x00000001
  #define MC0_IBECC_ECC_ERROR_LOG_MERR_OVERFLOW_DEF                    (0x00000000)
  #define MC0_IBECC_ECC_ERROR_LOG_MERR_OVERFLOW_HSH                    (0x4188D570)

  #define MC0_IBECC_ECC_ERROR_LOG_ERRADD_OFF                           ( 5)
  #define MC0_IBECC_ECC_ERROR_LOG_ERRADD_WID                           (34)
  #define MC0_IBECC_ECC_ERROR_LOG_ERRADD_MSK                           (0x0000007FFFFFFFE0ULL)
  #define MC0_IBECC_ECC_ERROR_LOG_ERRADD_MIN                           (0)
  #define MC0_IBECC_ECC_ERROR_LOG_ERRADD_MAX                           (17179869183ULL) // 0x3FFFFFFFF
  #define MC0_IBECC_ECC_ERROR_LOG_ERRADD_DEF                           (0x00000000)
  #define MC0_IBECC_ECC_ERROR_LOG_ERRADD_HSH                           (0x628AD570)

  #define MC0_IBECC_ECC_ERROR_LOG_RSVD1_OFF                            (39)
  #define MC0_IBECC_ECC_ERROR_LOG_RSVD1_WID                            ( 7)
  #define MC0_IBECC_ECC_ERROR_LOG_RSVD1_MSK                            (0x00003F8000000000ULL)
  #define MC0_IBECC_ECC_ERROR_LOG_RSVD1_MIN                            (0)
  #define MC0_IBECC_ECC_ERROR_LOG_RSVD1_MAX                            (127) // 0x0000007F
  #define MC0_IBECC_ECC_ERROR_LOG_RSVD1_DEF                            (0x00000000)
  #define MC0_IBECC_ECC_ERROR_LOG_RSVD1_HSH                            (0x474ED570)

  #define MC0_IBECC_ECC_ERROR_LOG_ERRSYND_OFF                          (46)
  #define MC0_IBECC_ECC_ERROR_LOG_ERRSYND_WID                          (16)
  #define MC0_IBECC_ECC_ERROR_LOG_ERRSYND_MSK                          (0x3FFFC00000000000ULL)
  #define MC0_IBECC_ECC_ERROR_LOG_ERRSYND_MIN                          (0)
  #define MC0_IBECC_ECC_ERROR_LOG_ERRSYND_MAX                          (65535) // 0x0000FFFF
  #define MC0_IBECC_ECC_ERROR_LOG_ERRSYND_DEF                          (0x00000000)
  #define MC0_IBECC_ECC_ERROR_LOG_ERRSYND_HSH                          (0x50DCD570)

  #define MC0_IBECC_ECC_ERROR_LOG_CERRSTS_OFF                          (62)
  #define MC0_IBECC_ECC_ERROR_LOG_CERRSTS_WID                          ( 1)
  #define MC0_IBECC_ECC_ERROR_LOG_CERRSTS_MSK                          (0x4000000000000000ULL)
  #define MC0_IBECC_ECC_ERROR_LOG_CERRSTS_MIN                          (0)
  #define MC0_IBECC_ECC_ERROR_LOG_CERRSTS_MAX                          (1) // 0x00000001
  #define MC0_IBECC_ECC_ERROR_LOG_CERRSTS_DEF                          (0x00000000)
  #define MC0_IBECC_ECC_ERROR_LOG_CERRSTS_HSH                          (0x41FCD570)

  #define MC0_IBECC_ECC_ERROR_LOG_MERRSTS_OFF                          (63)
  #define MC0_IBECC_ECC_ERROR_LOG_MERRSTS_WID                          ( 1)
  #define MC0_IBECC_ECC_ERROR_LOG_MERRSTS_MSK                          (0x8000000000000000ULL)
  #define MC0_IBECC_ECC_ERROR_LOG_MERRSTS_MIN                          (0)
  #define MC0_IBECC_ECC_ERROR_LOG_MERRSTS_MAX                          (1) // 0x00000001
  #define MC0_IBECC_ECC_ERROR_LOG_MERRSTS_DEF                          (0x00000000)
  #define MC0_IBECC_ECC_ERROR_LOG_MERRSTS_HSH                          (0x41FED570)

#define MC0_IBECC_PARITY_ERR_LOG_REG                                   (0x0000D578)
#define MC0_IBECC_PARITY_ERR_LOG_DEF                                   (0x0000000000000000)
#define MC0_IBECC_PARITY_ERR_LOG_VOLATILE_BITFIELDS_MSK                (0xF000007FFFFFFFF0)

  #define MC0_IBECC_PARITY_ERR_LOG_RSVD1_OFF                           ( 0)
  #define MC0_IBECC_PARITY_ERR_LOG_RSVD1_WID                           ( 4)
  #define MC0_IBECC_PARITY_ERR_LOG_RSVD1_MSK                           (0x0000000F)
  #define MC0_IBECC_PARITY_ERR_LOG_RSVD1_MIN                           (0)
  #define MC0_IBECC_PARITY_ERR_LOG_RSVD1_MAX                           (15) // 0x0000000F
  #define MC0_IBECC_PARITY_ERR_LOG_RSVD1_DEF                           (0x00000000)
  #define MC0_IBECC_PARITY_ERR_LOG_RSVD1_HSH                           (0x4400D578)

  #define MC0_IBECC_PARITY_ERR_LOG_PERR_OVERFLOW_OFF                   ( 4)
  #define MC0_IBECC_PARITY_ERR_LOG_PERR_OVERFLOW_WID                   ( 1)
  #define MC0_IBECC_PARITY_ERR_LOG_PERR_OVERFLOW_MSK                   (0x00000010)
  #define MC0_IBECC_PARITY_ERR_LOG_PERR_OVERFLOW_MIN                   (0)
  #define MC0_IBECC_PARITY_ERR_LOG_PERR_OVERFLOW_MAX                   (1) // 0x00000001
  #define MC0_IBECC_PARITY_ERR_LOG_PERR_OVERFLOW_DEF                   (0x00000000)
  #define MC0_IBECC_PARITY_ERR_LOG_PERR_OVERFLOW_HSH                   (0x4188D578)

  #define MC0_IBECC_PARITY_ERR_LOG_ERR_ADDRESS_OFF                     ( 5)
  #define MC0_IBECC_PARITY_ERR_LOG_ERR_ADDRESS_WID                     (34)
  #define MC0_IBECC_PARITY_ERR_LOG_ERR_ADDRESS_MSK                     (0x0000007FFFFFFFE0ULL)
  #define MC0_IBECC_PARITY_ERR_LOG_ERR_ADDRESS_MIN                     (0)
  #define MC0_IBECC_PARITY_ERR_LOG_ERR_ADDRESS_MAX                     (17179869183ULL) // 0x3FFFFFFFF
  #define MC0_IBECC_PARITY_ERR_LOG_ERR_ADDRESS_DEF                     (0x00000000)
  #define MC0_IBECC_PARITY_ERR_LOG_ERR_ADDRESS_HSH                     (0x628AD578)

  #define MC0_IBECC_PARITY_ERR_LOG_RSVD2_OFF                           (39)
  #define MC0_IBECC_PARITY_ERR_LOG_RSVD2_WID                           (21)
  #define MC0_IBECC_PARITY_ERR_LOG_RSVD2_MSK                           (0x0FFFFF8000000000ULL)
  #define MC0_IBECC_PARITY_ERR_LOG_RSVD2_MIN                           (0)
  #define MC0_IBECC_PARITY_ERR_LOG_RSVD2_MAX                           (2097151) // 0x001FFFFF
  #define MC0_IBECC_PARITY_ERR_LOG_RSVD2_DEF                           (0x00000000)
  #define MC0_IBECC_PARITY_ERR_LOG_RSVD2_HSH                           (0x554ED578)

  #define MC0_IBECC_PARITY_ERR_LOG_ERR_TYPE_OFF                        (60)
  #define MC0_IBECC_PARITY_ERR_LOG_ERR_TYPE_WID                        ( 3)
  #define MC0_IBECC_PARITY_ERR_LOG_ERR_TYPE_MSK                        (0x7000000000000000ULL)
  #define MC0_IBECC_PARITY_ERR_LOG_ERR_TYPE_MIN                        (0)
  #define MC0_IBECC_PARITY_ERR_LOG_ERR_TYPE_MAX                        (7) // 0x00000007
  #define MC0_IBECC_PARITY_ERR_LOG_ERR_TYPE_DEF                        (0x00000000)
  #define MC0_IBECC_PARITY_ERR_LOG_ERR_TYPE_HSH                        (0x43F8D578)

  #define MC0_IBECC_PARITY_ERR_LOG_ERR_STS_OFF                         (63)
  #define MC0_IBECC_PARITY_ERR_LOG_ERR_STS_WID                         ( 1)
  #define MC0_IBECC_PARITY_ERR_LOG_ERR_STS_MSK                         (0x8000000000000000ULL)
  #define MC0_IBECC_PARITY_ERR_LOG_ERR_STS_MIN                         (0)
  #define MC0_IBECC_PARITY_ERR_LOG_ERR_STS_MAX                         (1) // 0x00000001
  #define MC0_IBECC_PARITY_ERR_LOG_ERR_STS_DEF                         (0x00000000)
  #define MC0_IBECC_PARITY_ERR_LOG_ERR_STS_HSH                         (0x41FED578)

#define MC0_IBECC_ECC_INJ_ADDR_MASK_REG                                (0x0000D580)
#define MC0_IBECC_ECC_INJ_ADDR_MASK_DEF                                (0x0000000000000000)
#define MC0_IBECC_ECC_INJ_ADDR_MASK_VOLATILE_BITFIELDS_MSK             (0x0000000000000000)

  #define MC0_IBECC_ECC_INJ_ADDR_MASK_RSVD1_OFF                        ( 0)
  #define MC0_IBECC_ECC_INJ_ADDR_MASK_RSVD1_WID                        ( 5)
  #define MC0_IBECC_ECC_INJ_ADDR_MASK_RSVD1_MSK                        (0x0000001F)
  #define MC0_IBECC_ECC_INJ_ADDR_MASK_RSVD1_MIN                        (0)
  #define MC0_IBECC_ECC_INJ_ADDR_MASK_RSVD1_MAX                        (31) // 0x0000001F
  #define MC0_IBECC_ECC_INJ_ADDR_MASK_RSVD1_DEF                        (0x00000000)
  #define MC0_IBECC_ECC_INJ_ADDR_MASK_RSVD1_HSH                        (0x4500D580)

  #define MC0_IBECC_ECC_INJ_ADDR_MASK_ADDRESS_OFF                      ( 5)
  #define MC0_IBECC_ECC_INJ_ADDR_MASK_ADDRESS_WID                      (34)
  #define MC0_IBECC_ECC_INJ_ADDR_MASK_ADDRESS_MSK                      (0x0000007FFFFFFFE0ULL)
  #define MC0_IBECC_ECC_INJ_ADDR_MASK_ADDRESS_MIN                      (0)
  #define MC0_IBECC_ECC_INJ_ADDR_MASK_ADDRESS_MAX                      (17179869183ULL) // 0x3FFFFFFFF
  #define MC0_IBECC_ECC_INJ_ADDR_MASK_ADDRESS_DEF                      (0x00000000)
  #define MC0_IBECC_ECC_INJ_ADDR_MASK_ADDRESS_HSH                      (0x620AD580)

  #define MC0_IBECC_ECC_INJ_ADDR_MASK_RSVD2_OFF                        (39)
  #define MC0_IBECC_ECC_INJ_ADDR_MASK_RSVD2_WID                        (25)
  #define MC0_IBECC_ECC_INJ_ADDR_MASK_RSVD2_MSK                        (0xFFFFFF8000000000ULL)
  #define MC0_IBECC_ECC_INJ_ADDR_MASK_RSVD2_MIN                        (0)
  #define MC0_IBECC_ECC_INJ_ADDR_MASK_RSVD2_MAX                        (33554431) // 0x01FFFFFF
  #define MC0_IBECC_ECC_INJ_ADDR_MASK_RSVD2_DEF                        (0x00000000)
  #define MC0_IBECC_ECC_INJ_ADDR_MASK_RSVD2_HSH                        (0x594ED580)

#define MC0_IBECC_ECC_INJ_ADDR_BASE_REG                                (0x0000D588)
//Duplicate of MC0_IBECC_ECC_INJ_ADDR_MASK_REG

#define MC0_IBECC_PARITY_ERR_INJ_REG                                   (0x0000D590)
#define MC0_IBECC_PARITY_ERR_INJ_DEF                                   (0x00000000)
#define MC0_IBECC_PARITY_ERR_INJ_VOLATILE_BITFIELDS_MSK                (0x001FFFFF)

  #define MC0_IBECC_PARITY_ERR_INJ_WR_DATA_ERR_EN_OFF                  ( 0)
  #define MC0_IBECC_PARITY_ERR_INJ_WR_DATA_ERR_EN_WID                  ( 8)
  #define MC0_IBECC_PARITY_ERR_INJ_WR_DATA_ERR_EN_MSK                  (0x000000FF)
  #define MC0_IBECC_PARITY_ERR_INJ_WR_DATA_ERR_EN_MIN                  (0)
  #define MC0_IBECC_PARITY_ERR_INJ_WR_DATA_ERR_EN_MAX                  (255) // 0x000000FF
  #define MC0_IBECC_PARITY_ERR_INJ_WR_DATA_ERR_EN_DEF                  (0x00000000)
  #define MC0_IBECC_PARITY_ERR_INJ_WR_DATA_ERR_EN_HSH                  (0x0880D590)

  #define MC0_IBECC_PARITY_ERR_INJ_RD_ADDR_ERR_EN_OFF                  ( 8)
  #define MC0_IBECC_PARITY_ERR_INJ_RD_ADDR_ERR_EN_WID                  ( 1)
  #define MC0_IBECC_PARITY_ERR_INJ_RD_ADDR_ERR_EN_MSK                  (0x00000100)
  #define MC0_IBECC_PARITY_ERR_INJ_RD_ADDR_ERR_EN_MIN                  (0)
  #define MC0_IBECC_PARITY_ERR_INJ_RD_ADDR_ERR_EN_MAX                  (1) // 0x00000001
  #define MC0_IBECC_PARITY_ERR_INJ_RD_ADDR_ERR_EN_DEF                  (0x00000000)
  #define MC0_IBECC_PARITY_ERR_INJ_RD_ADDR_ERR_EN_HSH                  (0x0190D590)

  #define MC0_IBECC_PARITY_ERR_INJ_WR_HDR_ERR_EN_OFF                   ( 9)
  #define MC0_IBECC_PARITY_ERR_INJ_WR_HDR_ERR_EN_WID                   ( 2)
  #define MC0_IBECC_PARITY_ERR_INJ_WR_HDR_ERR_EN_MSK                   (0x00000600)
  #define MC0_IBECC_PARITY_ERR_INJ_WR_HDR_ERR_EN_MIN                   (0)
  #define MC0_IBECC_PARITY_ERR_INJ_WR_HDR_ERR_EN_MAX                   (3) // 0x00000003
  #define MC0_IBECC_PARITY_ERR_INJ_WR_HDR_ERR_EN_DEF                   (0x00000000)
  #define MC0_IBECC_PARITY_ERR_INJ_WR_HDR_ERR_EN_HSH                   (0x0292D590)

  #define MC0_IBECC_PARITY_ERR_INJ_RD_DATA_ERR_EN_OFF                  (11)
  #define MC0_IBECC_PARITY_ERR_INJ_RD_DATA_ERR_EN_WID                  ( 8)
  #define MC0_IBECC_PARITY_ERR_INJ_RD_DATA_ERR_EN_MSK                  (0x0007F800)
  #define MC0_IBECC_PARITY_ERR_INJ_RD_DATA_ERR_EN_MIN                  (0)
  #define MC0_IBECC_PARITY_ERR_INJ_RD_DATA_ERR_EN_MAX                  (255) // 0x000000FF
  #define MC0_IBECC_PARITY_ERR_INJ_RD_DATA_ERR_EN_DEF                  (0x00000000)
  #define MC0_IBECC_PARITY_ERR_INJ_RD_DATA_ERR_EN_HSH                  (0x0896D590)

  #define MC0_IBECC_PARITY_ERR_INJ_RD_HDR_ERR_EN_OFF                   (19)
  #define MC0_IBECC_PARITY_ERR_INJ_RD_HDR_ERR_EN_WID                   ( 2)
  #define MC0_IBECC_PARITY_ERR_INJ_RD_HDR_ERR_EN_MSK                   (0x00180000)
  #define MC0_IBECC_PARITY_ERR_INJ_RD_HDR_ERR_EN_MIN                   (0)
  #define MC0_IBECC_PARITY_ERR_INJ_RD_HDR_ERR_EN_MAX                   (3) // 0x00000003
  #define MC0_IBECC_PARITY_ERR_INJ_RD_HDR_ERR_EN_DEF                   (0x00000000)
  #define MC0_IBECC_PARITY_ERR_INJ_RD_HDR_ERR_EN_HSH                   (0x02A6D590)

  #define MC0_IBECC_PARITY_ERR_INJ_RSVD3_OFF                           (21)
  #define MC0_IBECC_PARITY_ERR_INJ_RSVD3_WID                           (11)
  #define MC0_IBECC_PARITY_ERR_INJ_RSVD3_MSK                           (0xFFE00000)
  #define MC0_IBECC_PARITY_ERR_INJ_RSVD3_MIN                           (0)
  #define MC0_IBECC_PARITY_ERR_INJ_RSVD3_MAX                           (2047) // 0x000007FF
  #define MC0_IBECC_PARITY_ERR_INJ_RSVD3_DEF                           (0x00000000)
  #define MC0_IBECC_PARITY_ERR_INJ_RSVD3_HSH                           (0x0B2AD590)

#define MC0_IBECC_CONTROL_REG                                          (0x0000D594)
#define MC0_IBECC_CONTROL_DEF                                          (0x02100300)
#define MC0_IBECC_CONTROL_VOLATILE_BITFIELDS_MSK                       (0x00000000)

  #define MC0_IBECC_CONTROL_OPERATION_MODE_OFF                         ( 0)
  #define MC0_IBECC_CONTROL_OPERATION_MODE_WID                         ( 2)
  #define MC0_IBECC_CONTROL_OPERATION_MODE_MSK                         (0x00000003)
  #define MC0_IBECC_CONTROL_OPERATION_MODE_MIN                         (0)
  #define MC0_IBECC_CONTROL_OPERATION_MODE_MAX                         (3) // 0x00000003
  #define MC0_IBECC_CONTROL_OPERATION_MODE_DEF                         (0x00000000)
  #define MC0_IBECC_CONTROL_OPERATION_MODE_HSH                         (0x0200D594)

  #define MC0_IBECC_CONTROL_EC_DIS_OFF                                 ( 2)
  #define MC0_IBECC_CONTROL_EC_DIS_WID                                 ( 1)
  #define MC0_IBECC_CONTROL_EC_DIS_MSK                                 (0x00000004)
  #define MC0_IBECC_CONTROL_EC_DIS_MIN                                 (0)
  #define MC0_IBECC_CONTROL_EC_DIS_MAX                                 (1) // 0x00000001
  #define MC0_IBECC_CONTROL_EC_DIS_DEF                                 (0x00000000)
  #define MC0_IBECC_CONTROL_EC_DIS_HSH                                 (0x0104D594)

  #define MC0_IBECC_CONTROL_DIS_PCIE_ERROR_OFF                         ( 3)
  #define MC0_IBECC_CONTROL_DIS_PCIE_ERROR_WID                         ( 1)
  #define MC0_IBECC_CONTROL_DIS_PCIE_ERROR_MSK                         (0x00000008)
  #define MC0_IBECC_CONTROL_DIS_PCIE_ERROR_MIN                         (0)
  #define MC0_IBECC_CONTROL_DIS_PCIE_ERROR_MAX                         (1) // 0x00000001
  #define MC0_IBECC_CONTROL_DIS_PCIE_ERROR_DEF                         (0x00000000)
  #define MC0_IBECC_CONTROL_DIS_PCIE_ERROR_HSH                         (0x0106D594)

  #define MC0_IBECC_CONTROL_DIS_MCA_LOG_OFF                            ( 4)
  #define MC0_IBECC_CONTROL_DIS_MCA_LOG_WID                            ( 1)
  #define MC0_IBECC_CONTROL_DIS_MCA_LOG_MSK                            (0x00000010)
  #define MC0_IBECC_CONTROL_DIS_MCA_LOG_MIN                            (0)
  #define MC0_IBECC_CONTROL_DIS_MCA_LOG_MAX                            (1) // 0x00000001
  #define MC0_IBECC_CONTROL_DIS_MCA_LOG_DEF                            (0x00000000)
  #define MC0_IBECC_CONTROL_DIS_MCA_LOG_HSH                            (0x0108D594)

  #define MC0_IBECC_CONTROL_CLKGT_DIS_OFF                              ( 5)
  #define MC0_IBECC_CONTROL_CLKGT_DIS_WID                              ( 1)
  #define MC0_IBECC_CONTROL_CLKGT_DIS_MSK                              (0x00000020)
  #define MC0_IBECC_CONTROL_CLKGT_DIS_MIN                              (0)
  #define MC0_IBECC_CONTROL_CLKGT_DIS_MAX                              (1) // 0x00000001
  #define MC0_IBECC_CONTROL_CLKGT_DIS_DEF                              (0x00000000)
  #define MC0_IBECC_CONTROL_CLKGT_DIS_HSH                              (0x010AD594)

  #define MC0_IBECC_CONTROL_PM_RSP_WAIT_ON_DISCONNECT_OFF              ( 6)
  #define MC0_IBECC_CONTROL_PM_RSP_WAIT_ON_DISCONNECT_WID              ( 1)
  #define MC0_IBECC_CONTROL_PM_RSP_WAIT_ON_DISCONNECT_MSK              (0x00000040)
  #define MC0_IBECC_CONTROL_PM_RSP_WAIT_ON_DISCONNECT_MIN              (0)
  #define MC0_IBECC_CONTROL_PM_RSP_WAIT_ON_DISCONNECT_MAX              (1) // 0x00000001
  #define MC0_IBECC_CONTROL_PM_RSP_WAIT_ON_DISCONNECT_DEF              (0x00000000)
  #define MC0_IBECC_CONTROL_PM_RSP_WAIT_ON_DISCONNECT_HSH              (0x010CD594)

  #define MC0_IBECC_CONTROL_FIXED_WINDOW_OFF                           ( 7)
  #define MC0_IBECC_CONTROL_FIXED_WINDOW_WID                           ( 1)
  #define MC0_IBECC_CONTROL_FIXED_WINDOW_MSK                           (0x00000080)
  #define MC0_IBECC_CONTROL_FIXED_WINDOW_MIN                           (0)
  #define MC0_IBECC_CONTROL_FIXED_WINDOW_MAX                           (1) // 0x00000001
  #define MC0_IBECC_CONTROL_FIXED_WINDOW_DEF                           (0x00000000)
  #define MC0_IBECC_CONTROL_FIXED_WINDOW_HSH                           (0x010ED594)

  #define MC0_IBECC_CONTROL_SYN_REQ_PRIO_OVERRIDE_OFF                  ( 8)
  #define MC0_IBECC_CONTROL_SYN_REQ_PRIO_OVERRIDE_WID                  ( 1)
  #define MC0_IBECC_CONTROL_SYN_REQ_PRIO_OVERRIDE_MSK                  (0x00000100)
  #define MC0_IBECC_CONTROL_SYN_REQ_PRIO_OVERRIDE_MIN                  (0)
  #define MC0_IBECC_CONTROL_SYN_REQ_PRIO_OVERRIDE_MAX                  (1) // 0x00000001
  #define MC0_IBECC_CONTROL_SYN_REQ_PRIO_OVERRIDE_DEF                  (0x00000001)
  #define MC0_IBECC_CONTROL_SYN_REQ_PRIO_OVERRIDE_HSH                  (0x0110D594)

  #define MC0_IBECC_CONTROL_DIS_HAZARD_OFF                             ( 9)
  #define MC0_IBECC_CONTROL_DIS_HAZARD_WID                             ( 1)
  #define MC0_IBECC_CONTROL_DIS_HAZARD_MSK                             (0x00000200)
  #define MC0_IBECC_CONTROL_DIS_HAZARD_MIN                             (0)
  #define MC0_IBECC_CONTROL_DIS_HAZARD_MAX                             (1) // 0x00000001
  #define MC0_IBECC_CONTROL_DIS_HAZARD_DEF                             (0x00000001)
  #define MC0_IBECC_CONTROL_DIS_HAZARD_HSH                             (0x0112D594)

  #define MC0_IBECC_CONTROL_DISABLE_FASTSLEEP_OFF                      (10)
  #define MC0_IBECC_CONTROL_DISABLE_FASTSLEEP_WID                      ( 1)
  #define MC0_IBECC_CONTROL_DISABLE_FASTSLEEP_MSK                      (0x00000400)
  #define MC0_IBECC_CONTROL_DISABLE_FASTSLEEP_MIN                      (0)
  #define MC0_IBECC_CONTROL_DISABLE_FASTSLEEP_MAX                      (1) // 0x00000001
  #define MC0_IBECC_CONTROL_DISABLE_FASTSLEEP_DEF                      (0x00000000)
  #define MC0_IBECC_CONTROL_DISABLE_FASTSLEEP_HSH                      (0x0114D594)

  #define MC0_IBECC_CONTROL_RSVD_11_OFF                                (11)
  #define MC0_IBECC_CONTROL_RSVD_11_WID                                ( 1)
  #define MC0_IBECC_CONTROL_RSVD_11_MSK                                (0x00000800)
  #define MC0_IBECC_CONTROL_RSVD_11_MIN                                (0)
  #define MC0_IBECC_CONTROL_RSVD_11_MAX                                (1) // 0x00000001
  #define MC0_IBECC_CONTROL_RSVD_11_DEF                                (0x00000000)
  #define MC0_IBECC_CONTROL_RSVD_11_HSH                                (0x0116D594)

  #define MC0_IBECC_CONTROL_ECC_GRANULARITY_OFF                        (12)
  #define MC0_IBECC_CONTROL_ECC_GRANULARITY_WID                        ( 1)
  #define MC0_IBECC_CONTROL_ECC_GRANULARITY_MSK                        (0x00001000)
  #define MC0_IBECC_CONTROL_ECC_GRANULARITY_MIN                        (0)
  #define MC0_IBECC_CONTROL_ECC_GRANULARITY_MAX                        (1) // 0x00000001
  #define MC0_IBECC_CONTROL_ECC_GRANULARITY_DEF                        (0x00000000)
  #define MC0_IBECC_CONTROL_ECC_GRANULARITY_HSH                        (0x0118D594)

  #define MC0_IBECC_CONTROL_RSVD_13_OFF                                (13)
  #define MC0_IBECC_CONTROL_RSVD_13_WID                                ( 1)
  #define MC0_IBECC_CONTROL_RSVD_13_MSK                                (0x00002000)
  #define MC0_IBECC_CONTROL_RSVD_13_MIN                                (0)
  #define MC0_IBECC_CONTROL_RSVD_13_MAX                                (1) // 0x00000001
  #define MC0_IBECC_CONTROL_RSVD_13_DEF                                (0x00000000)
  #define MC0_IBECC_CONTROL_RSVD_13_HSH                                (0x011AD594)

  #define MC0_IBECC_CONTROL_DIS_PERF_COUNTERS_OFF                      (14)
  #define MC0_IBECC_CONTROL_DIS_PERF_COUNTERS_WID                      ( 1)
  #define MC0_IBECC_CONTROL_DIS_PERF_COUNTERS_MSK                      (0x00004000)
  #define MC0_IBECC_CONTROL_DIS_PERF_COUNTERS_MIN                      (0)
  #define MC0_IBECC_CONTROL_DIS_PERF_COUNTERS_MAX                      (1) // 0x00000001
  #define MC0_IBECC_CONTROL_DIS_PERF_COUNTERS_DEF                      (0x00000000)
  #define MC0_IBECC_CONTROL_DIS_PERF_COUNTERS_HSH                      (0x011CD594)

  #define MC0_IBECC_CONTROL_DISABLE_MULTIPLE_PCH_MSG_OFF               (15)
  #define MC0_IBECC_CONTROL_DISABLE_MULTIPLE_PCH_MSG_WID               ( 1)
  #define MC0_IBECC_CONTROL_DISABLE_MULTIPLE_PCH_MSG_MSK               (0x00008000)
  #define MC0_IBECC_CONTROL_DISABLE_MULTIPLE_PCH_MSG_MIN               (0)
  #define MC0_IBECC_CONTROL_DISABLE_MULTIPLE_PCH_MSG_MAX               (1) // 0x00000001
  #define MC0_IBECC_CONTROL_DISABLE_MULTIPLE_PCH_MSG_DEF               (0x00000000)
  #define MC0_IBECC_CONTROL_DISABLE_MULTIPLE_PCH_MSG_HSH               (0x011ED594)

  #define MC0_IBECC_CONTROL_CFI_IDLE_TIME_OFF                          (16)
  #define MC0_IBECC_CONTROL_CFI_IDLE_TIME_WID                          ( 8)
  #define MC0_IBECC_CONTROL_CFI_IDLE_TIME_MSK                          (0x00FF0000)
  #define MC0_IBECC_CONTROL_CFI_IDLE_TIME_MIN                          (0)
  #define MC0_IBECC_CONTROL_CFI_IDLE_TIME_MAX                          (255) // 0x000000FF
  #define MC0_IBECC_CONTROL_CFI_IDLE_TIME_DEF                          (0x00000010)
  #define MC0_IBECC_CONTROL_CFI_IDLE_TIME_HSH                          (0x0820D594)

  #define MC0_IBECC_CONTROL_RSVD_24_OFF                                (24)
  #define MC0_IBECC_CONTROL_RSVD_24_WID                                ( 1)
  #define MC0_IBECC_CONTROL_RSVD_24_MSK                                (0x01000000)
  #define MC0_IBECC_CONTROL_RSVD_24_MIN                                (0)
  #define MC0_IBECC_CONTROL_RSVD_24_MAX                                (1) // 0x00000001
  #define MC0_IBECC_CONTROL_RSVD_24_DEF                                (0x00000000)
  #define MC0_IBECC_CONTROL_RSVD_24_HSH                                (0x0130D594)

  #define MC0_IBECC_CONTROL_RSB_ENABLE_OFF                             (25)
  #define MC0_IBECC_CONTROL_RSB_ENABLE_WID                             ( 1)
  #define MC0_IBECC_CONTROL_RSB_ENABLE_MSK                             (0x02000000)
  #define MC0_IBECC_CONTROL_RSB_ENABLE_MIN                             (0)
  #define MC0_IBECC_CONTROL_RSB_ENABLE_MAX                             (1) // 0x00000001
  #define MC0_IBECC_CONTROL_RSB_ENABLE_DEF                             (0x00000001)
  #define MC0_IBECC_CONTROL_RSB_ENABLE_HSH                             (0x0132D594)

  #define MC0_IBECC_CONTROL_DIS_CFI_EARLY_VALID_GEN_OFF                (26)
  #define MC0_IBECC_CONTROL_DIS_CFI_EARLY_VALID_GEN_WID                ( 2)
  #define MC0_IBECC_CONTROL_DIS_CFI_EARLY_VALID_GEN_MSK                (0x0C000000)
  #define MC0_IBECC_CONTROL_DIS_CFI_EARLY_VALID_GEN_MIN                (0)
  #define MC0_IBECC_CONTROL_DIS_CFI_EARLY_VALID_GEN_MAX                (3) // 0x00000003
  #define MC0_IBECC_CONTROL_DIS_CFI_EARLY_VALID_GEN_DEF                (0x00000000)
  #define MC0_IBECC_CONTROL_DIS_CFI_EARLY_VALID_GEN_HSH                (0x0234D594)

  #define MC0_IBECC_CONTROL_RSVD_28_OFF                                (28)
  #define MC0_IBECC_CONTROL_RSVD_28_WID                                ( 1)
  #define MC0_IBECC_CONTROL_RSVD_28_MSK                                (0x10000000)
  #define MC0_IBECC_CONTROL_RSVD_28_MIN                                (0)
  #define MC0_IBECC_CONTROL_RSVD_28_MAX                                (1) // 0x00000001
  #define MC0_IBECC_CONTROL_RSVD_28_DEF                                (0x00000000)
  #define MC0_IBECC_CONTROL_RSVD_28_HSH                                (0x0138D594)

  #define MC0_IBECC_CONTROL_DIS_INIT_BLOCK_OFF                         (29)
  #define MC0_IBECC_CONTROL_DIS_INIT_BLOCK_WID                         ( 1)
  #define MC0_IBECC_CONTROL_DIS_INIT_BLOCK_MSK                         (0x20000000)
  #define MC0_IBECC_CONTROL_DIS_INIT_BLOCK_MIN                         (0)
  #define MC0_IBECC_CONTROL_DIS_INIT_BLOCK_MAX                         (1) // 0x00000001
  #define MC0_IBECC_CONTROL_DIS_INIT_BLOCK_DEF                         (0x00000000)
  #define MC0_IBECC_CONTROL_DIS_INIT_BLOCK_HSH                         (0x013AD594)

  #define MC0_IBECC_CONTROL_INIT_BLOCK_PROT_ONLY_OFF                   (30)
  #define MC0_IBECC_CONTROL_INIT_BLOCK_PROT_ONLY_WID                   ( 1)
  #define MC0_IBECC_CONTROL_INIT_BLOCK_PROT_ONLY_MSK                   (0x40000000)
  #define MC0_IBECC_CONTROL_INIT_BLOCK_PROT_ONLY_MIN                   (0)
  #define MC0_IBECC_CONTROL_INIT_BLOCK_PROT_ONLY_MAX                   (1) // 0x00000001
  #define MC0_IBECC_CONTROL_INIT_BLOCK_PROT_ONLY_DEF                   (0x00000000)
  #define MC0_IBECC_CONTROL_INIT_BLOCK_PROT_ONLY_HSH                   (0x013CD594)

  #define MC0_IBECC_CONTROL_RSVD_31_OFF                                (31)
  #define MC0_IBECC_CONTROL_RSVD_31_WID                                ( 1)
  #define MC0_IBECC_CONTROL_RSVD_31_MSK                                (0x80000000)
  #define MC0_IBECC_CONTROL_RSVD_31_MIN                                (0)
  #define MC0_IBECC_CONTROL_RSVD_31_MAX                                (1) // 0x00000001
  #define MC0_IBECC_CONTROL_RSVD_31_DEF                                (0x00000000)
  #define MC0_IBECC_CONTROL_RSVD_31_HSH                                (0x013ED594)

#define MC0_IBECC_ECC_INJ_CONTROL_REG                                  (0x0000D598)
#define MC0_IBECC_ECC_INJ_CONTROL_DEF                                  (0x00000000)
#define MC0_IBECC_ECC_INJ_CONTROL_VOLATILE_BITFIELDS_MSK               (0x00000000)

  #define MC0_IBECC_ECC_INJ_CONTROL_ECC_INJECT_OFF                     ( 0)
  #define MC0_IBECC_ECC_INJ_CONTROL_ECC_INJECT_WID                     ( 3)
  #define MC0_IBECC_ECC_INJ_CONTROL_ECC_INJECT_MSK                     (0x00000007)
  #define MC0_IBECC_ECC_INJ_CONTROL_ECC_INJECT_MIN                     (0)
  #define MC0_IBECC_ECC_INJ_CONTROL_ECC_INJECT_MAX                     (7) // 0x00000007
  #define MC0_IBECC_ECC_INJ_CONTROL_ECC_INJECT_DEF                     (0x00000000)
  #define MC0_IBECC_ECC_INJ_CONTROL_ECC_INJECT_HSH                     (0x0300D598)

  #define MC0_IBECC_ECC_INJ_CONTROL_RSVD1_OFF                          ( 3)
  #define MC0_IBECC_ECC_INJ_CONTROL_RSVD1_WID                          ( 5)
  #define MC0_IBECC_ECC_INJ_CONTROL_RSVD1_MSK                          (0x000000F8)
  #define MC0_IBECC_ECC_INJ_CONTROL_RSVD1_MIN                          (0)
  #define MC0_IBECC_ECC_INJ_CONTROL_RSVD1_MAX                          (31) // 0x0000001F
  #define MC0_IBECC_ECC_INJ_CONTROL_RSVD1_DEF                          (0x00000000)
  #define MC0_IBECC_ECC_INJ_CONTROL_RSVD1_HSH                          (0x0506D598)

  #define MC0_IBECC_ECC_INJ_CONTROL_COUNT_OFF                          ( 8)
  #define MC0_IBECC_ECC_INJ_CONTROL_COUNT_WID                          ( 8)
  #define MC0_IBECC_ECC_INJ_CONTROL_COUNT_MSK                          (0x0000FF00)
  #define MC0_IBECC_ECC_INJ_CONTROL_COUNT_MIN                          (0)
  #define MC0_IBECC_ECC_INJ_CONTROL_COUNT_MAX                          (255) // 0x000000FF
  #define MC0_IBECC_ECC_INJ_CONTROL_COUNT_DEF                          (0x00000000)
  #define MC0_IBECC_ECC_INJ_CONTROL_COUNT_HSH                          (0x0810D598)

  #define MC0_IBECC_ECC_INJ_CONTROL_RSVD2_OFF                          (16)
  #define MC0_IBECC_ECC_INJ_CONTROL_RSVD2_WID                          (16)
  #define MC0_IBECC_ECC_INJ_CONTROL_RSVD2_MSK                          (0xFFFF0000)
  #define MC0_IBECC_ECC_INJ_CONTROL_RSVD2_MIN                          (0)
  #define MC0_IBECC_ECC_INJ_CONTROL_RSVD2_MAX                          (65535) // 0x0000FFFF
  #define MC0_IBECC_ECC_INJ_CONTROL_RSVD2_DEF                          (0x00000000)
  #define MC0_IBECC_ECC_INJ_CONTROL_RSVD2_HSH                          (0x1020D598)

#define MC0_IBECC_MBIST_POST_CTRL_RSB_REG                              (0x0000D5B8)
//Duplicate of MC0_IBECC_MBIST_POST_CTRL_WDB_REG

#define MC0_IBECC_ECC_VC0_SYND_RD_REQCOUNT_REG                         (0x0000D5C0)
//Duplicate of MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG

#define MC0_IBECC_ECC_VC1_SYND_RD_REQCOUNT_REG                         (0x0000D5C8)
//Duplicate of MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG

#define MC0_IBECC_ECC_VC0_SYND_WR_REQCOUNT_REG                         (0x0000D5D0)
//Duplicate of MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG

#define MC0_IBECC_ECC_VC1_SYND_WR_REQCOUNT_REG                         (0x0000D5D8)
//Duplicate of MC0_IBECC_ECC_VC0_RD_REQCOUNT_REG

#define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_REG                   (0x0000D5E0)
#define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_DEF                   (0x00000C30)
#define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_MAX_VC0_ENTRIES_OFF ( 0)
  #define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_MAX_VC0_ENTRIES_WID ( 6)
  #define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_MAX_VC0_ENTRIES_MSK (0x0000003F)
  #define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_MAX_VC0_ENTRIES_MIN (0)
  #define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_MAX_VC0_ENTRIES_MAX (63) // 0x0000003F
  #define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_MAX_VC0_ENTRIES_DEF (0x00000030)
  #define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_MAX_VC0_ENTRIES_HSH (0x0600D5E0)

  #define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_MAX_VC1_ENTRIES_OFF ( 6)
  #define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_MAX_VC1_ENTRIES_WID ( 6)
  #define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_MAX_VC1_ENTRIES_MSK (0x00000FC0)
  #define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_MAX_VC1_ENTRIES_MIN (0)
  #define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_MAX_VC1_ENTRIES_MAX (63) // 0x0000003F
  #define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_MAX_VC1_ENTRIES_DEF (0x00000030)
  #define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_MAX_VC1_ENTRIES_HSH (0x060CD5E0)

  #define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_RSVD_OFF            (12)
  #define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_RSVD_WID            (20)
  #define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_RSVD_MSK            (0xFFFFF000)
  #define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_RSVD_MIN            (0)
  #define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_RSVD_MAX            (1048575) // 0x000FFFFF
  #define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_RSVD_DEF            (0x00000000)
  #define MC0_IBECC_RD_TRKR_VC_MAX_ENTRY_THRESHOLD_RSVD_HSH            (0x1418D5E0)

#define MC0_IBECC_BLOCK_BIDS_REG                                       (0x0000D5E4)
#define MC0_IBECC_BLOCK_BIDS_DEF                                       (0x00000000)
#define MC0_IBECC_BLOCK_BIDS_VOLATILE_BITFIELDS_MSK                    (0x00000000)

  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_RD_REQ_OFF          ( 0)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_RD_REQ_WID          ( 1)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_RD_REQ_MSK          (0x00000001)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_RD_REQ_MIN          (0)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_RD_REQ_MAX          (1) // 0x00000001
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_RD_REQ_DEF          (0x00000000)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_RD_REQ_HSH          (0x0100D5E4)

  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_RD_REQ_OFF          ( 1)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_RD_REQ_WID          ( 1)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_RD_REQ_MSK          (0x00000002)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_RD_REQ_MIN          (0)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_RD_REQ_MAX          (1) // 0x00000001
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_RD_REQ_DEF          (0x00000000)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_RD_REQ_HSH          (0x0102D5E4)

  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_WR_REQ_OFF          ( 2)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_WR_REQ_WID          ( 1)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_WR_REQ_MSK          (0x00000004)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_WR_REQ_MIN          (0)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_WR_REQ_MAX          (1) // 0x00000001
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_WR_REQ_DEF          (0x00000000)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_WR_REQ_HSH          (0x0104D5E4)

  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_WR_REQ_OFF          ( 3)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_WR_REQ_WID          ( 1)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_WR_REQ_MSK          (0x00000008)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_WR_REQ_MIN          (0)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_WR_REQ_MAX          (1) // 0x00000001
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_WR_REQ_DEF          (0x00000000)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_WR_REQ_HSH          (0x0106D5E4)

  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC0_RD_REQ_OFF        ( 4)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC0_RD_REQ_WID        ( 1)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC0_RD_REQ_MSK        (0x00000010)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC0_RD_REQ_MIN        (0)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC0_RD_REQ_MAX        (1) // 0x00000001
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC0_RD_REQ_DEF        (0x00000000)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC0_RD_REQ_HSH        (0x0108D5E4)

  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC1_RD_REQ_OFF        ( 5)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC1_RD_REQ_WID        ( 1)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC1_RD_REQ_MSK        (0x00000020)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC1_RD_REQ_MIN        (0)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC1_RD_REQ_MAX        (1) // 0x00000001
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC1_RD_REQ_DEF        (0x00000000)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC1_RD_REQ_HSH        (0x010AD5E4)

  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC0_WR_REQ_OFF        ( 6)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC0_WR_REQ_WID        ( 1)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC0_WR_REQ_MSK        (0x00000040)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC0_WR_REQ_MIN        (0)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC0_WR_REQ_MAX        (1) // 0x00000001
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC0_WR_REQ_DEF        (0x00000000)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC0_WR_REQ_HSH        (0x010CD5E4)

  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC1_WR_REQ_OFF        ( 7)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC1_WR_REQ_WID        ( 1)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC1_WR_REQ_MSK        (0x00000080)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC1_WR_REQ_MIN        (0)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC1_WR_REQ_MAX        (1) // 0x00000001
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC1_WR_REQ_DEF        (0x00000000)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC1_WR_REQ_HSH        (0x010ED5E4)

  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_RD_CPL_OFF          ( 8)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_RD_CPL_WID          ( 1)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_RD_CPL_MSK          (0x00000100)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_RD_CPL_MIN          (0)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_RD_CPL_MAX          (1) // 0x00000001
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_RD_CPL_DEF          (0x00000000)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_RD_CPL_HSH          (0x0110D5E4)

  #define MC0_IBECC_BLOCK_BIDS_BLOCK_NON_PROTECTED_VC0_RD_CPL_OFF      ( 9)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_NON_PROTECTED_VC0_RD_CPL_WID      ( 1)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_NON_PROTECTED_VC0_RD_CPL_MSK      (0x00000200)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_NON_PROTECTED_VC0_RD_CPL_MIN      (0)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_NON_PROTECTED_VC0_RD_CPL_MAX      (1) // 0x00000001
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_NON_PROTECTED_VC0_RD_CPL_DEF      (0x00000000)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_NON_PROTECTED_VC0_RD_CPL_HSH      (0x0112D5E4)

  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_RD_CPL_OFF          (10)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_RD_CPL_WID          ( 1)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_RD_CPL_MSK          (0x00000400)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_RD_CPL_MIN          (0)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_RD_CPL_MAX          (1) // 0x00000001
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_RD_CPL_DEF          (0x00000000)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_RD_CPL_HSH          (0x0114D5E4)

  #define MC0_IBECC_BLOCK_BIDS_BLOCK_NON_PROTECTED_VC1_RD_CPL_OFF      (11)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_NON_PROTECTED_VC1_RD_CPL_WID      ( 1)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_NON_PROTECTED_VC1_RD_CPL_MSK      (0x00000800)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_NON_PROTECTED_VC1_RD_CPL_MIN      (0)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_NON_PROTECTED_VC1_RD_CPL_MAX      (1) // 0x00000001
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_NON_PROTECTED_VC1_RD_CPL_DEF      (0x00000000)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_NON_PROTECTED_VC1_RD_CPL_HSH      (0x0116D5E4)

  #define MC0_IBECC_BLOCK_BIDS_BLOCK_INIT_WR_REQ_OFF                   (12)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_INIT_WR_REQ_WID                   ( 1)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_INIT_WR_REQ_MSK                   (0x00001000)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_INIT_WR_REQ_MIN                   (0)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_INIT_WR_REQ_MAX                   (1) // 0x00000001
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_INIT_WR_REQ_DEF                   (0x00000000)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_INIT_WR_REQ_HSH                   (0x0118D5E4)

  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_WR_RSP_OFF          (13)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_WR_RSP_WID          ( 1)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_WR_RSP_MSK          (0x00002000)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_WR_RSP_MIN          (0)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_WR_RSP_MAX          (1) // 0x00000001
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_WR_RSP_DEF          (0x00000000)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC0_WR_RSP_HSH          (0x011AD5E4)

  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC0_WR_RSP_OFF        (14)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC0_WR_RSP_WID        ( 1)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC0_WR_RSP_MSK        (0x00004000)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC0_WR_RSP_MIN        (0)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC0_WR_RSP_MAX        (1) // 0x00000001
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC0_WR_RSP_DEF        (0x00000000)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC0_WR_RSP_HSH        (0x011CD5E4)

  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_WR_RSP_OFF          (15)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_WR_RSP_WID          ( 1)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_WR_RSP_MSK          (0x00008000)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_WR_RSP_MIN          (0)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_WR_RSP_MAX          (1) // 0x00000001
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_WR_RSP_DEF          (0x00000000)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_PROTECTED_VC1_WR_RSP_HSH          (0x011ED5E4)

  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC1_WR_RSP_OFF        (16)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC1_WR_RSP_WID        ( 1)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC1_WR_RSP_MSK        (0x00010000)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC1_WR_RSP_MIN        (0)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC1_WR_RSP_MAX        (1) // 0x00000001
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC1_WR_RSP_DEF        (0x00000000)
  #define MC0_IBECC_BLOCK_BIDS_BLOCK_UNPROTECTED_VC1_WR_RSP_HSH        (0x0120D5E4)

  #define MC0_IBECC_BLOCK_BIDS_RSVD_OFF                                (17)
  #define MC0_IBECC_BLOCK_BIDS_RSVD_WID                                (15)
  #define MC0_IBECC_BLOCK_BIDS_RSVD_MSK                                (0xFFFE0000)
  #define MC0_IBECC_BLOCK_BIDS_RSVD_MIN                                (0)
  #define MC0_IBECC_BLOCK_BIDS_RSVD_MAX                                (32767) // 0x00007FFF
  #define MC0_IBECC_BLOCK_BIDS_RSVD_DEF                                (0x00000000)
  #define MC0_IBECC_BLOCK_BIDS_RSVD_HSH                                (0x0F22D5E4)

#define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_REG                         (0x0000D5E8)
#define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_DEF                         (0x00000000)
#define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_VOLATILE_BITFIELDS_MSK      (0x00000000)

  #define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_ADDRESS_OFF               ( 0)
  #define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_ADDRESS_WID               (15)
  #define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_ADDRESS_MSK               (0x00007FFF)
  #define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_ADDRESS_MIN               (0)
  #define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_ADDRESS_MAX               (32767) // 0x00007FFF
  #define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_ADDRESS_DEF               (0x00000000)
  #define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_ADDRESS_HSH               (0x0F00D5E8)

  #define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_RSVD_OFF                  (15)
  #define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_RSVD_WID                  (17)
  #define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_RSVD_MSK                  (0xFFFF8000)
  #define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_RSVD_MIN                  (0)
  #define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_RSVD_MAX                  (131071) // 0x0001FFFF
  #define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_RSVD_DEF                  (0x00000000)
  #define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_RSVD_HSH                  (0x111ED5E8)

#define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_1_REG                         (0x0000D5EC)
//Duplicate of MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_REG

#define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_2_REG                         (0x0000D5F0)
//Duplicate of MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_REG

#define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_3_REG                         (0x0000D5F4)
//Duplicate of MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_REG

#define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_4_REG                         (0x0000D5F8)
//Duplicate of MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_REG

#define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_5_REG                         (0x0000D5FC)
//Duplicate of MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_REG

#define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_6_REG                         (0x0000D600)
//Duplicate of MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_REG

#define MC0_IBECC_ECC_STORAGE_ADDR_RANGE_7_REG                         (0x0000D604)
//Duplicate of MC0_IBECC_ECC_STORAGE_ADDR_RANGE_0_REG

#define MC0_IBECC_MDST_TRIM_FUSE_REG                                   (0x0000D608)
#define MC0_IBECC_MDST_TRIM_FUSE_DEF                                   (0x00000000)
#define MC0_IBECC_MDST_TRIM_FUSE_VOLATILE_BITFIELDS_MSK                (0x00000000)

  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_SBC_OFF                        ( 0)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_SBC_WID                        ( 2)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_SBC_MSK                        (0x00000003)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_SBC_MIN                        (0)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_SBC_MAX                        (3) // 0x00000003
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_SBC_DEF                        (0x00000000)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_SBC_HSH                        (0x0200D608)

  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_STBYP_OFF                      ( 2)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_STBYP_WID                      ( 1)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_STBYP_MSK                      (0x00000004)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_STBYP_MIN                      (0)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_STBYP_MAX                      (1) // 0x00000001
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_STBYP_DEF                      (0x00000000)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_STBYP_HSH                      (0x0104D608)

  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_WMCE_OFF                       ( 3)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_WMCE_WID                       ( 2)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_WMCE_MSK                       (0x00000018)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_WMCE_MIN                       (0)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_WMCE_MAX                       (3) // 0x00000003
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_WMCE_DEF                       (0x00000000)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_WMCE_HSH                       (0x0206D608)

  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_RMCE_OFF                       ( 5)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_RMCE_WID                       ( 2)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_RMCE_MSK                       (0x00000060)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_RMCE_MIN                       (0)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_RMCE_MAX                       (3) // 0x00000003
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_RMCE_DEF                       (0x00000000)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_RMCE_HSH                       (0x020AD608)

  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_MCE_OFF                        ( 7)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_MCE_WID                        ( 1)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_MCE_MSK                        (0x00000080)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_MCE_MIN                        (0)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_MCE_MAX                        (1) // 0x00000001
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_MCE_DEF                        (0x00000000)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_MCE_HSH                        (0x010ED608)

  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_TRIM_FUSE_OFF                  ( 8)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_TRIM_FUSE_WID                  (13)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_TRIM_FUSE_MSK                  (0x001FFF00)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_TRIM_FUSE_MIN                  (0)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_TRIM_FUSE_MAX                  (8191) // 0x00001FFF
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_TRIM_FUSE_DEF                  (0x00000000)
  #define MC0_IBECC_MDST_TRIM_FUSE_MDST_TRIM_FUSE_HSH                  (0x0D10D608)

  #define MC0_IBECC_MDST_TRIM_FUSE_RSVD_OFF                            (21)
  #define MC0_IBECC_MDST_TRIM_FUSE_RSVD_WID                            (11)
  #define MC0_IBECC_MDST_TRIM_FUSE_RSVD_MSK                            (0xFFE00000)
  #define MC0_IBECC_MDST_TRIM_FUSE_RSVD_MIN                            (0)
  #define MC0_IBECC_MDST_TRIM_FUSE_RSVD_MAX                            (2047) // 0x000007FF
  #define MC0_IBECC_MDST_TRIM_FUSE_RSVD_DEF                            (0x00000000)
  #define MC0_IBECC_MDST_TRIM_FUSE_RSVD_HSH                            (0x0B2AD608)

#define MC0_IBECC_ECC_ERR_COUNT_REG                                    (0x0000D610)
#define MC0_IBECC_ECC_ERR_COUNT_DEF                                    (0x0000000000000000)
#define MC0_IBECC_ECC_ERR_COUNT_VOLATILE_BITFIELDS_MSK                 (0xFFFFFFFFFFFFFFFF)

  #define MC0_IBECC_ECC_ERR_COUNT_ECC_ERR_COUNT_OFF                    ( 0)
  #define MC0_IBECC_ECC_ERR_COUNT_ECC_ERR_COUNT_WID                    (64)
  #define MC0_IBECC_ECC_ERR_COUNT_ECC_ERR_COUNT_MSK                    (0xFFFFFFFFFFFFFFFFULL)
  #define MC0_IBECC_ECC_ERR_COUNT_ECC_ERR_COUNT_MIN                    (0)
  #define MC0_IBECC_ECC_ERR_COUNT_ECC_ERR_COUNT_MAX                    (18446744073709551615ULL) // 0xFFFFFFFFFFFFFFFF
  #define MC0_IBECC_ECC_ERR_COUNT_ECC_ERR_COUNT_DEF                    (0x00000000)
  #define MC0_IBECC_ECC_ERR_COUNT_ECC_ERR_COUNT_HSH                    (0x4080D610)

#define MC0_IBECC_RESTRICTED_PG_CP_REG                                 (0x0000D618)
//Duplicate of MC0_IBECC_DEFAULT_PG_CP_REG

#define MC0_IBECC_RESTRICTED_PG_RAC_REG                                (0x0000D620)
//Duplicate of MC0_IBECC_DEFAULT_PG_RAC_REG

#define MC0_IBECC_RESTRICTED_PG_WAC_REG                                (0x0000D628)
//Duplicate of MC0_IBECC_DEFAULT_PG_CP_REG

#define MC0_IBECC_MEMORY_INIT_CONTROL_REG                              (0x0000D630)
#define MC0_IBECC_MEMORY_INIT_CONTROL_DEF                              (0x00000000)
#define MC0_IBECC_MEMORY_INIT_CONTROL_VOLATILE_BITFIELDS_MSK           (0x800000FF)

  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_0_OFF               ( 0)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_0_WID               ( 1)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_0_MSK               (0x00000001)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_0_MIN               (0)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_0_MAX               (1) // 0x00000001
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_0_DEF               (0x00000000)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_0_HSH               (0x0180D630)

  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_1_OFF               ( 1)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_1_WID               ( 1)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_1_MSK               (0x00000002)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_1_MIN               (0)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_1_MAX               (1) // 0x00000001
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_1_DEF               (0x00000000)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_1_HSH               (0x0182D630)

  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_2_OFF               ( 2)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_2_WID               ( 1)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_2_MSK               (0x00000004)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_2_MIN               (0)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_2_MAX               (1) // 0x00000001
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_2_DEF               (0x00000000)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_2_HSH               (0x0184D630)

  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_3_OFF               ( 3)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_3_WID               ( 1)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_3_MSK               (0x00000008)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_3_MIN               (0)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_3_MAX               (1) // 0x00000001
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_3_DEF               (0x00000000)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_3_HSH               (0x0186D630)

  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_4_OFF               ( 4)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_4_WID               ( 1)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_4_MSK               (0x00000010)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_4_MIN               (0)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_4_MAX               (1) // 0x00000001
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_4_DEF               (0x00000000)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_4_HSH               (0x0188D630)

  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_5_OFF               ( 5)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_5_WID               ( 1)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_5_MSK               (0x00000020)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_5_MIN               (0)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_5_MAX               (1) // 0x00000001
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_5_DEF               (0x00000000)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_5_HSH               (0x018AD630)

  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_6_OFF               ( 6)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_6_WID               ( 1)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_6_MSK               (0x00000040)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_6_MIN               (0)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_6_MAX               (1) // 0x00000001
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_6_DEF               (0x00000000)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_6_HSH               (0x018CD630)

  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_7_OFF               ( 7)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_7_WID               ( 1)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_7_MSK               (0x00000080)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_7_MIN               (0)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_7_MAX               (1) // 0x00000001
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_7_DEF               (0x00000000)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_RANGE_7_HSH               (0x018ED630)

  #define MC0_IBECC_MEMORY_INIT_CONTROL_RSVD_OFF                       ( 8)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_RSVD_WID                       (23)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_RSVD_MSK                       (0x7FFFFF00)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_RSVD_MIN                       (0)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_RSVD_MAX                       (8388607) // 0x007FFFFF
  #define MC0_IBECC_MEMORY_INIT_CONTROL_RSVD_DEF                       (0x00000000)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_RSVD_HSH                       (0x1710D630)

  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_ALL_RANGES_OFF            (31)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_ALL_RANGES_WID            ( 1)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_ALL_RANGES_MSK            (0x80000000)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_ALL_RANGES_MIN            (0)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_ALL_RANGES_MAX            (1) // 0x00000001
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_ALL_RANGES_DEF            (0x00000000)
  #define MC0_IBECC_MEMORY_INIT_CONTROL_INIT_ALL_RANGES_HSH            (0x01BED630)

#define MC0_IBECC_P1_DATA_TX_CNT_REG                                   (0x0000D634)
#define MC0_IBECC_P1_DATA_TX_CNT_DEF                                   (0x00000000)
#define MC0_IBECC_P1_DATA_TX_CNT_VOLATILE_BITFIELDS_MSK                (0x01FFFFFF)

  #define MC0_IBECC_P1_DATA_TX_CNT_DATA_TX_CNT_OFF                     ( 0)
  #define MC0_IBECC_P1_DATA_TX_CNT_DATA_TX_CNT_WID                     (25)
  #define MC0_IBECC_P1_DATA_TX_CNT_DATA_TX_CNT_MSK                     (0x01FFFFFF)
  #define MC0_IBECC_P1_DATA_TX_CNT_DATA_TX_CNT_MIN                     (0)
  #define MC0_IBECC_P1_DATA_TX_CNT_DATA_TX_CNT_MAX                     (33554431) // 0x01FFFFFF
  #define MC0_IBECC_P1_DATA_TX_CNT_DATA_TX_CNT_DEF                     (0x00000000)
  #define MC0_IBECC_P1_DATA_TX_CNT_DATA_TX_CNT_HSH                     (0x1980D634)

  #define MC0_IBECC_P1_DATA_TX_CNT_RSVD_OFF                            (25)
  #define MC0_IBECC_P1_DATA_TX_CNT_RSVD_WID                            ( 7)
  #define MC0_IBECC_P1_DATA_TX_CNT_RSVD_MSK                            (0xFE000000)
  #define MC0_IBECC_P1_DATA_TX_CNT_RSVD_MIN                            (0)
  #define MC0_IBECC_P1_DATA_TX_CNT_RSVD_MAX                            (127) // 0x0000007F
  #define MC0_IBECC_P1_DATA_TX_CNT_RSVD_DEF                            (0x00000000)
  #define MC0_IBECC_P1_DATA_TX_CNT_RSVD_HSH                            (0x0732D634)

#define MC0_IBECC_P1_DATA_RX_CNT_REG                                   (0x0000D638)
#define MC0_IBECC_P1_DATA_RX_CNT_DEF                                   (0x00000000)
#define MC0_IBECC_P1_DATA_RX_CNT_VOLATILE_BITFIELDS_MSK                (0x01FFFFFF)

  #define MC0_IBECC_P1_DATA_RX_CNT_DATA_RX_CNT_OFF                     ( 0)
  #define MC0_IBECC_P1_DATA_RX_CNT_DATA_RX_CNT_WID                     (25)
  #define MC0_IBECC_P1_DATA_RX_CNT_DATA_RX_CNT_MSK                     (0x01FFFFFF)
  #define MC0_IBECC_P1_DATA_RX_CNT_DATA_RX_CNT_MIN                     (0)
  #define MC0_IBECC_P1_DATA_RX_CNT_DATA_RX_CNT_MAX                     (33554431) // 0x01FFFFFF
  #define MC0_IBECC_P1_DATA_RX_CNT_DATA_RX_CNT_DEF                     (0x00000000)
  #define MC0_IBECC_P1_DATA_RX_CNT_DATA_RX_CNT_HSH                     (0x1980D638)

  #define MC0_IBECC_P1_DATA_RX_CNT_RSVD_OFF                            (25)
  #define MC0_IBECC_P1_DATA_RX_CNT_RSVD_WID                            ( 7)
  #define MC0_IBECC_P1_DATA_RX_CNT_RSVD_MSK                            (0xFE000000)
  #define MC0_IBECC_P1_DATA_RX_CNT_RSVD_MIN                            (0)
  #define MC0_IBECC_P1_DATA_RX_CNT_RSVD_MAX                            (127) // 0x0000007F
  #define MC0_IBECC_P1_DATA_RX_CNT_RSVD_DEF                            (0x00000000)
  #define MC0_IBECC_P1_DATA_RX_CNT_RSVD_HSH                            (0x0732D638)

#define MC0_IBECC_P0_DATA_TX_CNT_REG                                   (0x0000D63C)
//Duplicate of MC0_IBECC_P1_DATA_TX_CNT_REG

#define MC0_IBECC_P0_DATA_RX_CNT_REG                                   (0x0000D640)
//Duplicate of MC0_IBECC_P1_DATA_RX_CNT_REG

#define MC0_IBECC_ADDR_HASH_REG                                        (0x0000D644)
#define MC0_IBECC_ADDR_HASH_DEF                                        (0x00000000)
#define MC0_IBECC_ADDR_HASH_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define MC0_IBECC_ADDR_HASH_HASH_ENABLED_OFF                         ( 0)
  #define MC0_IBECC_ADDR_HASH_HASH_ENABLED_WID                         ( 1)
  #define MC0_IBECC_ADDR_HASH_HASH_ENABLED_MSK                         (0x00000001)
  #define MC0_IBECC_ADDR_HASH_HASH_ENABLED_MIN                         (0)
  #define MC0_IBECC_ADDR_HASH_HASH_ENABLED_MAX                         (1) // 0x00000001
  #define MC0_IBECC_ADDR_HASH_HASH_ENABLED_DEF                         (0x00000000)
  #define MC0_IBECC_ADDR_HASH_HASH_ENABLED_HSH                         (0x0100D644)

  #define MC0_IBECC_ADDR_HASH_HASH_LSB_OFF                             ( 1)
  #define MC0_IBECC_ADDR_HASH_HASH_LSB_WID                             ( 3)
  #define MC0_IBECC_ADDR_HASH_HASH_LSB_MSK                             (0x0000000E)
  #define MC0_IBECC_ADDR_HASH_HASH_LSB_MIN                             (0)
  #define MC0_IBECC_ADDR_HASH_HASH_LSB_MAX                             (7) // 0x00000007
  #define MC0_IBECC_ADDR_HASH_HASH_LSB_DEF                             (0x00000000)
  #define MC0_IBECC_ADDR_HASH_HASH_LSB_HSH                             (0x0302D644)

  #define MC0_IBECC_ADDR_HASH_RESERVED0_OFF                            ( 4)
  #define MC0_IBECC_ADDR_HASH_RESERVED0_WID                            ( 2)
  #define MC0_IBECC_ADDR_HASH_RESERVED0_MSK                            (0x00000030)
  #define MC0_IBECC_ADDR_HASH_RESERVED0_MIN                            (0)
  #define MC0_IBECC_ADDR_HASH_RESERVED0_MAX                            (3) // 0x00000003
  #define MC0_IBECC_ADDR_HASH_RESERVED0_DEF                            (0x00000000)
  #define MC0_IBECC_ADDR_HASH_RESERVED0_HSH                            (0x0208D644)

  #define MC0_IBECC_ADDR_HASH_HASH_MASK_OFF                            ( 6)
  #define MC0_IBECC_ADDR_HASH_HASH_MASK_WID                            (14)
  #define MC0_IBECC_ADDR_HASH_HASH_MASK_MSK                            (0x000FFFC0)
  #define MC0_IBECC_ADDR_HASH_HASH_MASK_MIN                            (0)
  #define MC0_IBECC_ADDR_HASH_HASH_MASK_MAX                            (16383) // 0x00003FFF
  #define MC0_IBECC_ADDR_HASH_HASH_MASK_DEF                            (0x00000000)
  #define MC0_IBECC_ADDR_HASH_HASH_MASK_HSH                            (0x0E0CD644)

  #define MC0_IBECC_ADDR_HASH_RESERVED1_OFF                            (20)
  #define MC0_IBECC_ADDR_HASH_RESERVED1_WID                            (12)
  #define MC0_IBECC_ADDR_HASH_RESERVED1_MSK                            (0xFFF00000)
  #define MC0_IBECC_ADDR_HASH_RESERVED1_MIN                            (0)
  #define MC0_IBECC_ADDR_HASH_RESERVED1_MAX                            (4095) // 0x00000FFF
  #define MC0_IBECC_ADDR_HASH_RESERVED1_DEF                            (0x00000000)
  #define MC0_IBECC_ADDR_HASH_RESERVED1_HSH                            (0x0C28D644)

#define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_REG                       (0x0000D648)
#define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_DEF                       (0x00000004)
#define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_VOLATILE_BITFIELDS_MSK    (0x00FFFF00)

  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_VERSION_OFF          ( 0)
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_VERSION_WID          ( 8)
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_VERSION_MSK          (0x000000FF)
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_VERSION_MIN          (0)
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_VERSION_MAX          (255) // 0x000000FF
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_VERSION_DEF          (0x00000004)
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_VERSION_HSH          (0x0800D648)

  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_DERIVATIVE_OFF       ( 8)
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_DERIVATIVE_WID       ( 4)
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_DERIVATIVE_MSK       (0x00000F00)
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_DERIVATIVE_MIN       (0)
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_DERIVATIVE_MAX       (15) // 0x0000000F
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_DERIVATIVE_DEF       (0x00000000)
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_DERIVATIVE_HSH       (0x0490D648)

  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_SEGMENT_OFF          (12)
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_SEGMENT_WID          ( 4)
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_SEGMENT_MSK          (0x0000F000)
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_SEGMENT_MIN          (0)
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_SEGMENT_MAX          (15) // 0x0000000F
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_SEGMENT_DEF          (0x00000000)
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_SEGMENT_HSH          (0x0498D648)

  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_SUBVERSION_OFF       (16)
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_SUBVERSION_WID       ( 8)
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_SUBVERSION_MSK       (0x00FF0000)
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_SUBVERSION_MIN       (0)
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_SUBVERSION_MAX       (255) // 0x000000FF
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_SUBVERSION_DEF       (0x00000000)
  #define MC0_IBECC_IP_VERSION_0_0_0_IBECCHBAR_IP_SUBVERSION_HSH       (0x08A0D648)

#define MC0_IBECC_CFI_HOST_REG                                         (0x0000D64C)
#define MC0_IBECC_CFI_HOST_DEF                                         (0x00000000)
#define MC0_IBECC_CFI_HOST_VOLATILE_BITFIELDS_MSK                      (0x000000FF)

  #define MC0_IBECC_CFI_HOST_HOST_ID_OFF                               ( 0)
  #define MC0_IBECC_CFI_HOST_HOST_ID_WID                               ( 8)
  #define MC0_IBECC_CFI_HOST_HOST_ID_MSK                               (0x000000FF)
  #define MC0_IBECC_CFI_HOST_HOST_ID_MIN                               (0)
  #define MC0_IBECC_CFI_HOST_HOST_ID_MAX                               (255) // 0x000000FF
  #define MC0_IBECC_CFI_HOST_HOST_ID_DEF                               (0x00000000)
  #define MC0_IBECC_CFI_HOST_HOST_ID_HSH                               (0x0880D64C)

  #define MC0_IBECC_CFI_HOST_DTL_OFF                                   ( 8)
  #define MC0_IBECC_CFI_HOST_DTL_WID                                   ( 1)
  #define MC0_IBECC_CFI_HOST_DTL_MSK                                   (0x00000100)
  #define MC0_IBECC_CFI_HOST_DTL_MIN                                   (0)
  #define MC0_IBECC_CFI_HOST_DTL_MAX                                   (1) // 0x00000001
  #define MC0_IBECC_CFI_HOST_DTL_DEF                                   (0x00000000)
  #define MC0_IBECC_CFI_HOST_DTL_HSH                                   (0x0110D64C)

  #define MC0_IBECC_CFI_HOST_RSL_OFF                                   ( 9)
  #define MC0_IBECC_CFI_HOST_RSL_WID                                   ( 1)
  #define MC0_IBECC_CFI_HOST_RSL_MSK                                   (0x00000200)
  #define MC0_IBECC_CFI_HOST_RSL_MIN                                   (0)
  #define MC0_IBECC_CFI_HOST_RSL_MAX                                   (1) // 0x00000001
  #define MC0_IBECC_CFI_HOST_RSL_DEF                                   (0x00000000)
  #define MC0_IBECC_CFI_HOST_RSL_HSH                                   (0x0112D64C)

  #define MC0_IBECC_CFI_HOST_RSVD_OFF                                  (10)
  #define MC0_IBECC_CFI_HOST_RSVD_WID                                  (22)
  #define MC0_IBECC_CFI_HOST_RSVD_MSK                                  (0xFFFFFC00)
  #define MC0_IBECC_CFI_HOST_RSVD_MIN                                  (0)
  #define MC0_IBECC_CFI_HOST_RSVD_MAX                                  (4194303) // 0x003FFFFF
  #define MC0_IBECC_CFI_HOST_RSVD_DEF                                  (0x00000000)
  #define MC0_IBECC_CFI_HOST_RSVD_HSH                                  (0x1614D64C)

#define MC0_MAD_INTER_CHANNEL_REG                                      (0x0000D800)
#define MC0_MAD_INTER_CHANNEL_DEF                                      (0x00000000)
#define MC0_MAD_INTER_CHANNEL_VOLATILE_BITFIELDS_MSK                   (0x00FFFF00)

  #define MC0_MAD_INTER_CHANNEL_DDR_TYPE_OFF                           ( 0)
  #define MC0_MAD_INTER_CHANNEL_DDR_TYPE_WID                           ( 2)
  #define MC0_MAD_INTER_CHANNEL_DDR_TYPE_MSK                           (0x00000003)
  #define MC0_MAD_INTER_CHANNEL_DDR_TYPE_MIN                           (0)
  #define MC0_MAD_INTER_CHANNEL_DDR_TYPE_MAX                           (3) // 0x00000003
  #define MC0_MAD_INTER_CHANNEL_DDR_TYPE_DEF                           (0x00000000)
  #define MC0_MAD_INTER_CHANNEL_DDR_TYPE_HSH                           (0x0200D800)

  #define MC0_MAD_INTER_CHANNEL_ECC_OFF                                ( 2)
  #define MC0_MAD_INTER_CHANNEL_ECC_WID                                ( 1)
  #define MC0_MAD_INTER_CHANNEL_ECC_MSK                                (0x00000004)
  #define MC0_MAD_INTER_CHANNEL_ECC_MIN                                (0)
  #define MC0_MAD_INTER_CHANNEL_ECC_MAX                                (1) // 0x00000001
  #define MC0_MAD_INTER_CHANNEL_ECC_DEF                                (0x00000000)
  #define MC0_MAD_INTER_CHANNEL_ECC_HSH                                (0x0104D800)

  #define MC0_MAD_INTER_CHANNEL_CH_0_SIZE_OFF                          ( 8)
  #define MC0_MAD_INTER_CHANNEL_CH_0_SIZE_WID                          ( 8)
  #define MC0_MAD_INTER_CHANNEL_CH_0_SIZE_MSK                          (0x0000FF00)
  #define MC0_MAD_INTER_CHANNEL_CH_0_SIZE_MIN                          (0)
  #define MC0_MAD_INTER_CHANNEL_CH_0_SIZE_MAX                          (255) // 0x000000FF
  #define MC0_MAD_INTER_CHANNEL_CH_0_SIZE_DEF                          (0x00000000)
  #define MC0_MAD_INTER_CHANNEL_CH_0_SIZE_HSH                          (0x0890D800)

  #define MC0_MAD_INTER_CHANNEL_CH_1_SIZE_OFF                          (16)
  #define MC0_MAD_INTER_CHANNEL_CH_1_SIZE_WID                          ( 8)
  #define MC0_MAD_INTER_CHANNEL_CH_1_SIZE_MSK                          (0x00FF0000)
  #define MC0_MAD_INTER_CHANNEL_CH_1_SIZE_MIN                          (0)
  #define MC0_MAD_INTER_CHANNEL_CH_1_SIZE_MAX                          (255) // 0x000000FF
  #define MC0_MAD_INTER_CHANNEL_CH_1_SIZE_DEF                          (0x00000000)
  #define MC0_MAD_INTER_CHANNEL_CH_1_SIZE_HSH                          (0x08A0D800)

#define MC0_MAD_INTRA_CHANNEL_0_REG                                    (0x0000D804)
#define MC0_MAD_INTRA_CHANNEL_0_DEF                                    (0x00000000)
#define MC0_MAD_INTRA_CHANNEL_0_VOLATILE_BITFIELDS_MSK                 (0x00000000)

  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_DENSITY_OFF                  ( 0)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_DENSITY_WID                  ( 4)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_DENSITY_MSK                  (0x0000000F)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_DENSITY_MIN                  (0)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_DENSITY_MAX                  (15) // 0x0000000F
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_DENSITY_DEF                  (0x00000000)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_DENSITY_HSH                  (0x0400D804)

  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_WIDTH_OFF                    ( 6)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_WIDTH_WID                    ( 1)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_WIDTH_MSK                    (0x00000040)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_WIDTH_MIN                    (0)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_WIDTH_MAX                    (1) // 0x00000001
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_WIDTH_DEF                    (0x00000000)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_WIDTH_HSH                    (0x010CD804)

  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_RANKS_OFF                    ( 7)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_RANKS_WID                    ( 1)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_RANKS_MSK                    (0x00000080)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_RANKS_MIN                    (0)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_RANKS_MAX                    (1) // 0x00000001
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_RANKS_DEF                    (0x00000000)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_0_RANKS_HSH                    (0x010ED804)

  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_DENSITY_OFF                  ( 8)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_DENSITY_WID                  ( 4)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_DENSITY_MSK                  (0x00000F00)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_DENSITY_MIN                  (0)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_DENSITY_MAX                  (15) // 0x0000000F
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_DENSITY_DEF                  (0x00000000)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_DENSITY_HSH                  (0x0410D804)

  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_WIDTH_OFF                    (14)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_WIDTH_WID                    ( 1)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_WIDTH_MSK                    (0x00004000)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_WIDTH_MIN                    (0)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_WIDTH_MAX                    (1) // 0x00000001
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_WIDTH_DEF                    (0x00000000)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_WIDTH_HSH                    (0x011CD804)

  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_RANKS_OFF                    (15)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_RANKS_WID                    ( 1)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_RANKS_MSK                    (0x00008000)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_RANKS_MIN                    (0)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_RANKS_MAX                    (1) // 0x00000001
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_RANKS_DEF                    (0x00000000)
  #define MC0_MAD_INTRA_CHANNEL_0_SUBCH_1_RANKS_HSH                    (0x011ED804)

#define MC0_MAD_INTRA_CHANNEL_1_REG                                    (0x0000D808)
//Duplicate of MC0_MAD_INTRA_CHANNEL_0_REG

#define MC0_MAD_DRAM_HASH_0_REG                                        (0x0000D80C)
#define MC0_MAD_DRAM_HASH_0_DEF                                        (0x00000031)
#define MC0_MAD_DRAM_HASH_0_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define MC0_MAD_DRAM_HASH_0_eim_OFF                                  ( 0)
  #define MC0_MAD_DRAM_HASH_0_eim_WID                                  ( 1)
  #define MC0_MAD_DRAM_HASH_0_eim_MSK                                  (0x00000001)
  #define MC0_MAD_DRAM_HASH_0_eim_MIN                                  (0)
  #define MC0_MAD_DRAM_HASH_0_eim_MAX                                  (1) // 0x00000001
  #define MC0_MAD_DRAM_HASH_0_eim_DEF                                  (0x00000001)
  #define MC0_MAD_DRAM_HASH_0_eim_HSH                                  (0x0100D80C)

  #define MC0_MAD_DRAM_HASH_0_ebh_OFF                                  ( 1)
  #define MC0_MAD_DRAM_HASH_0_ebh_WID                                  ( 1)
  #define MC0_MAD_DRAM_HASH_0_ebh_MSK                                  (0x00000002)
  #define MC0_MAD_DRAM_HASH_0_ebh_MIN                                  (0)
  #define MC0_MAD_DRAM_HASH_0_ebh_MAX                                  (1) // 0x00000001
  #define MC0_MAD_DRAM_HASH_0_ebh_DEF                                  (0x00000000)
  #define MC0_MAD_DRAM_HASH_0_ebh_HSH                                  (0x0102D80C)

  #define MC0_MAD_DRAM_HASH_0_bg0hash_OFF                              ( 4)
  #define MC0_MAD_DRAM_HASH_0_bg0hash_WID                              ( 3)
  #define MC0_MAD_DRAM_HASH_0_bg0hash_MSK                              (0x00000070)
  #define MC0_MAD_DRAM_HASH_0_bg0hash_MIN                              (0)
  #define MC0_MAD_DRAM_HASH_0_bg0hash_MAX                              (7) // 0x00000007
  #define MC0_MAD_DRAM_HASH_0_bg0hash_DEF                              (0x00000003)
  #define MC0_MAD_DRAM_HASH_0_bg0hash_HSH                              (0x0308D80C)

  #define MC0_MAD_DRAM_HASH_0_bg1hash_OFF                              ( 8)
  #define MC0_MAD_DRAM_HASH_0_bg1hash_WID                              ( 3)
  #define MC0_MAD_DRAM_HASH_0_bg1hash_MSK                              (0x00000700)
  #define MC0_MAD_DRAM_HASH_0_bg1hash_MIN                              (0)
  #define MC0_MAD_DRAM_HASH_0_bg1hash_MAX                              (7) // 0x00000007
  #define MC0_MAD_DRAM_HASH_0_bg1hash_DEF                              (0x00000000)
  #define MC0_MAD_DRAM_HASH_0_bg1hash_HSH                              (0x0310D80C)

  #define MC0_MAD_DRAM_HASH_0_bg2hash_OFF                              (12)
  #define MC0_MAD_DRAM_HASH_0_bg2hash_WID                              ( 3)
  #define MC0_MAD_DRAM_HASH_0_bg2hash_MSK                              (0x00007000)
  #define MC0_MAD_DRAM_HASH_0_bg2hash_MIN                              (0)
  #define MC0_MAD_DRAM_HASH_0_bg2hash_MAX                              (7) // 0x00000007
  #define MC0_MAD_DRAM_HASH_0_bg2hash_DEF                              (0x00000000)
  #define MC0_MAD_DRAM_HASH_0_bg2hash_HSH                              (0x0318D80C)

#define MC0_MAD_DRAM_HASH_1_REG                                        (0x0000D810)
//Duplicate of MC0_MAD_DRAM_HASH_0_REG

#define MC0_MCDECS_MISC_REG                                            (0x0000D818)
#define MC0_MCDECS_MISC_DEF                                            (0x00000000)
#define MC0_MCDECS_MISC_VOLATILE_BITFIELDS_MSK                         (0xFFFC0000)

  #define MC0_MCDECS_MISC_Spare_RW_OFF                                 ( 0)
  #define MC0_MCDECS_MISC_Spare_RW_WID                                 (16)
  #define MC0_MCDECS_MISC_Spare_RW_MSK                                 (0x0000FFFF)
  #define MC0_MCDECS_MISC_Spare_RW_MIN                                 (0)
  #define MC0_MCDECS_MISC_Spare_RW_MAX                                 (65535) // 0x0000FFFF
  #define MC0_MCDECS_MISC_Spare_RW_DEF                                 (0x00000000)
  #define MC0_MCDECS_MISC_Spare_RW_HSH                                 (0x1000D818)

  #define MC0_MCDECS_MISC_cbit_RGBSync_accelerate_x16_OFF              (16)
  #define MC0_MCDECS_MISC_cbit_RGBSync_accelerate_x16_WID              ( 1)
  #define MC0_MCDECS_MISC_cbit_RGBSync_accelerate_x16_MSK              (0x00010000)
  #define MC0_MCDECS_MISC_cbit_RGBSync_accelerate_x16_MIN              (0)
  #define MC0_MCDECS_MISC_cbit_RGBSync_accelerate_x16_MAX              (1) // 0x00000001
  #define MC0_MCDECS_MISC_cbit_RGBSync_accelerate_x16_DEF              (0x00000000)
  #define MC0_MCDECS_MISC_cbit_RGBSync_accelerate_x16_HSH              (0x0120D818)

  #define MC0_MCDECS_MISC_cbit_ddrpl_enable_mains_glbdrv_OFF           (17)
  #define MC0_MCDECS_MISC_cbit_ddrpl_enable_mains_glbdrv_WID           ( 1)
  #define MC0_MCDECS_MISC_cbit_ddrpl_enable_mains_glbdrv_MSK           (0x00020000)
  #define MC0_MCDECS_MISC_cbit_ddrpl_enable_mains_glbdrv_MIN           (0)
  #define MC0_MCDECS_MISC_cbit_ddrpl_enable_mains_glbdrv_MAX           (1) // 0x00000001
  #define MC0_MCDECS_MISC_cbit_ddrpl_enable_mains_glbdrv_DEF           (0x00000000)
  #define MC0_MCDECS_MISC_cbit_ddrpl_enable_mains_glbdrv_HSH           (0x0122D818)

  #define MC0_MCDECS_MISC_spare_RW_V_OFF                               (18)
  #define MC0_MCDECS_MISC_spare_RW_V_WID                               (14)
  #define MC0_MCDECS_MISC_spare_RW_V_MSK                               (0xFFFC0000)
  #define MC0_MCDECS_MISC_spare_RW_V_MIN                               (0)
  #define MC0_MCDECS_MISC_spare_RW_V_MAX                               (16383) // 0x00003FFF
  #define MC0_MCDECS_MISC_spare_RW_V_DEF                               (0x00000000)
  #define MC0_MCDECS_MISC_spare_RW_V_HSH                               (0x0EA4D818)

#define MC0_MCDECS_CBIT_REG                                            (0x0000D81C)
#define MC0_MCDECS_CBIT_DEF                                            (0x00000000)
#define MC0_MCDECS_CBIT_VOLATILE_BITFIELDS_MSK                         (0x00000008)

  #define MC0_MCDECS_CBIT_increase_rcomp_OFF                           ( 0)
  #define MC0_MCDECS_CBIT_increase_rcomp_WID                           ( 1)
  #define MC0_MCDECS_CBIT_increase_rcomp_MSK                           (0x00000001)
  #define MC0_MCDECS_CBIT_increase_rcomp_MIN                           (0)
  #define MC0_MCDECS_CBIT_increase_rcomp_MAX                           (1) // 0x00000001
  #define MC0_MCDECS_CBIT_increase_rcomp_DEF                           (0x00000000)
  #define MC0_MCDECS_CBIT_increase_rcomp_HSH                           (0x0100D81C)

  #define MC0_MCDECS_CBIT_invert_dclk_clone_OFF                        ( 1)
  #define MC0_MCDECS_CBIT_invert_dclk_clone_WID                        ( 1)
  #define MC0_MCDECS_CBIT_invert_dclk_clone_MSK                        (0x00000002)
  #define MC0_MCDECS_CBIT_invert_dclk_clone_MIN                        (0)
  #define MC0_MCDECS_CBIT_invert_dclk_clone_MAX                        (1) // 0x00000001
  #define MC0_MCDECS_CBIT_invert_dclk_clone_DEF                        (0x00000000)
  #define MC0_MCDECS_CBIT_invert_dclk_clone_HSH                        (0x0102D81C)

  #define MC0_MCDECS_CBIT_freeze_pwm_counters_OFF                      ( 3)
  #define MC0_MCDECS_CBIT_freeze_pwm_counters_WID                      ( 1)
  #define MC0_MCDECS_CBIT_freeze_pwm_counters_MSK                      (0x00000008)
  #define MC0_MCDECS_CBIT_freeze_pwm_counters_MIN                      (0)
  #define MC0_MCDECS_CBIT_freeze_pwm_counters_MAX                      (1) // 0x00000001
  #define MC0_MCDECS_CBIT_freeze_pwm_counters_DEF                      (0x00000000)
  #define MC0_MCDECS_CBIT_freeze_pwm_counters_HSH                      (0x0186D81C)

  #define MC0_MCDECS_CBIT_dis_single_ch_sr_OFF                         ( 4)
  #define MC0_MCDECS_CBIT_dis_single_ch_sr_WID                         ( 1)
  #define MC0_MCDECS_CBIT_dis_single_ch_sr_MSK                         (0x00000010)
  #define MC0_MCDECS_CBIT_dis_single_ch_sr_MIN                         (0)
  #define MC0_MCDECS_CBIT_dis_single_ch_sr_MAX                         (1) // 0x00000001
  #define MC0_MCDECS_CBIT_dis_single_ch_sr_DEF                         (0x00000000)
  #define MC0_MCDECS_CBIT_dis_single_ch_sr_HSH                         (0x0108D81C)

  #define MC0_MCDECS_CBIT_dis_other_ch_stolen_ref_OFF                  ( 5)
  #define MC0_MCDECS_CBIT_dis_other_ch_stolen_ref_WID                  ( 1)
  #define MC0_MCDECS_CBIT_dis_other_ch_stolen_ref_MSK                  (0x00000020)
  #define MC0_MCDECS_CBIT_dis_other_ch_stolen_ref_MIN                  (0)
  #define MC0_MCDECS_CBIT_dis_other_ch_stolen_ref_MAX                  (1) // 0x00000001
  #define MC0_MCDECS_CBIT_dis_other_ch_stolen_ref_DEF                  (0x00000000)
  #define MC0_MCDECS_CBIT_dis_other_ch_stolen_ref_HSH                  (0x010AD81C)

  #define MC0_MCDECS_CBIT_oppsr_pending_wr_clkgate_dis_mcmain_OFF      ( 6)
  #define MC0_MCDECS_CBIT_oppsr_pending_wr_clkgate_dis_mcmain_WID      ( 1)
  #define MC0_MCDECS_CBIT_oppsr_pending_wr_clkgate_dis_mcmain_MSK      (0x00000040)
  #define MC0_MCDECS_CBIT_oppsr_pending_wr_clkgate_dis_mcmain_MIN      (0)
  #define MC0_MCDECS_CBIT_oppsr_pending_wr_clkgate_dis_mcmain_MAX      (1) // 0x00000001
  #define MC0_MCDECS_CBIT_oppsr_pending_wr_clkgate_dis_mcmain_DEF      (0x00000000)
  #define MC0_MCDECS_CBIT_oppsr_pending_wr_clkgate_dis_mcmain_HSH      (0x010CD81C)

  #define MC0_MCDECS_CBIT_block_rsp_credit_return_OFF                  ( 7)
  #define MC0_MCDECS_CBIT_block_rsp_credit_return_WID                  ( 1)
  #define MC0_MCDECS_CBIT_block_rsp_credit_return_MSK                  (0x00000080)
  #define MC0_MCDECS_CBIT_block_rsp_credit_return_MIN                  (0)
  #define MC0_MCDECS_CBIT_block_rsp_credit_return_MAX                  (1) // 0x00000001
  #define MC0_MCDECS_CBIT_block_rsp_credit_return_DEF                  (0x00000000)
  #define MC0_MCDECS_CBIT_block_rsp_credit_return_HSH                  (0x010ED81C)

  #define MC0_MCDECS_CBIT_block_data_credit_return_OFF                 ( 8)
  #define MC0_MCDECS_CBIT_block_data_credit_return_WID                 ( 1)
  #define MC0_MCDECS_CBIT_block_data_credit_return_MSK                 (0x00000100)
  #define MC0_MCDECS_CBIT_block_data_credit_return_MIN                 (0)
  #define MC0_MCDECS_CBIT_block_data_credit_return_MAX                 (1) // 0x00000001
  #define MC0_MCDECS_CBIT_block_data_credit_return_DEF                 (0x00000000)
  #define MC0_MCDECS_CBIT_block_data_credit_return_HSH                 (0x0110D81C)

  #define MC0_MCDECS_CBIT_dis_clk_gate_OFF                             ( 9)
  #define MC0_MCDECS_CBIT_dis_clk_gate_WID                             ( 1)
  #define MC0_MCDECS_CBIT_dis_clk_gate_MSK                             (0x00000200)
  #define MC0_MCDECS_CBIT_dis_clk_gate_MIN                             (0)
  #define MC0_MCDECS_CBIT_dis_clk_gate_MAX                             (1) // 0x00000001
  #define MC0_MCDECS_CBIT_dis_clk_gate_DEF                             (0x00000000)
  #define MC0_MCDECS_CBIT_dis_clk_gate_HSH                             (0x0112D81C)

  #define MC0_MCDECS_CBIT_freeze_AFD_on_RTB_OFF                        (10)
  #define MC0_MCDECS_CBIT_freeze_AFD_on_RTB_WID                        ( 1)
  #define MC0_MCDECS_CBIT_freeze_AFD_on_RTB_MSK                        (0x00000400)
  #define MC0_MCDECS_CBIT_freeze_AFD_on_RTB_MIN                        (0)
  #define MC0_MCDECS_CBIT_freeze_AFD_on_RTB_MAX                        (1) // 0x00000001
  #define MC0_MCDECS_CBIT_freeze_AFD_on_RTB_DEF                        (0x00000000)
  #define MC0_MCDECS_CBIT_freeze_AFD_on_RTB_HSH                        (0x0114D81C)

  #define MC0_MCDECS_CBIT_iosfsb_keep_ISM_active_OFF                   (11)
  #define MC0_MCDECS_CBIT_iosfsb_keep_ISM_active_WID                   ( 1)
  #define MC0_MCDECS_CBIT_iosfsb_keep_ISM_active_MSK                   (0x00000800)
  #define MC0_MCDECS_CBIT_iosfsb_keep_ISM_active_MIN                   (0)
  #define MC0_MCDECS_CBIT_iosfsb_keep_ISM_active_MAX                   (1) // 0x00000001
  #define MC0_MCDECS_CBIT_iosfsb_keep_ISM_active_DEF                   (0x00000000)
  #define MC0_MCDECS_CBIT_iosfsb_keep_ISM_active_HSH                   (0x0116D81C)

  #define MC0_MCDECS_CBIT_ignoreRefBetweenSRX2SRE_OFF                  (12)
  #define MC0_MCDECS_CBIT_ignoreRefBetweenSRX2SRE_WID                  ( 1)
  #define MC0_MCDECS_CBIT_ignoreRefBetweenSRX2SRE_MSK                  (0x00001000)
  #define MC0_MCDECS_CBIT_ignoreRefBetweenSRX2SRE_MIN                  (0)
  #define MC0_MCDECS_CBIT_ignoreRefBetweenSRX2SRE_MAX                  (1) // 0x00000001
  #define MC0_MCDECS_CBIT_ignoreRefBetweenSRX2SRE_DEF                  (0x00000000)
  #define MC0_MCDECS_CBIT_ignoreRefBetweenSRX2SRE_HSH                  (0x0118D81C)

  #define MC0_MCDECS_CBIT_dis_cfi_spec_req_early_valid_OFF             (13)
  #define MC0_MCDECS_CBIT_dis_cfi_spec_req_early_valid_WID             ( 1)
  #define MC0_MCDECS_CBIT_dis_cfi_spec_req_early_valid_MSK             (0x00002000)
  #define MC0_MCDECS_CBIT_dis_cfi_spec_req_early_valid_MIN             (0)
  #define MC0_MCDECS_CBIT_dis_cfi_spec_req_early_valid_MAX             (1) // 0x00000001
  #define MC0_MCDECS_CBIT_dis_cfi_spec_req_early_valid_DEF             (0x00000000)
  #define MC0_MCDECS_CBIT_dis_cfi_spec_req_early_valid_HSH             (0x011AD81C)

  #define MC0_MCDECS_CBIT_dis_cmi_ep_power_flows_OFF                   (18)
  #define MC0_MCDECS_CBIT_dis_cmi_ep_power_flows_WID                   ( 1)
  #define MC0_MCDECS_CBIT_dis_cmi_ep_power_flows_MSK                   (0x00040000)
  #define MC0_MCDECS_CBIT_dis_cmi_ep_power_flows_MIN                   (0)
  #define MC0_MCDECS_CBIT_dis_cmi_ep_power_flows_MAX                   (1) // 0x00000001
  #define MC0_MCDECS_CBIT_dis_cmi_ep_power_flows_DEF                   (0x00000000)
  #define MC0_MCDECS_CBIT_dis_cmi_ep_power_flows_HSH                   (0x0124D81C)

  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_opcodes_OFF                (19)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_opcodes_WID                ( 1)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_opcodes_MSK                (0x00080000)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_opcodes_MIN                (0)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_opcodes_MAX                (1) // 0x00000001
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_opcodes_DEF                (0x00000000)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_opcodes_HSH                (0x0126D81C)

  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_bar_OFF                    (20)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_bar_WID                    ( 1)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_bar_MSK                    (0x00100000)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_bar_MIN                    (0)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_bar_MAX                    (1) // 0x00000001
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_bar_DEF                    (0x00000000)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_bar_HSH                    (0x0128D81C)

  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_range_OFF                  (21)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_range_WID                  ( 1)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_range_MSK                  (0x00200000)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_range_MIN                  (0)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_range_MAX                  (1) // 0x00000001
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_range_DEF                  (0x00000000)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_range_HSH                  (0x012AD81C)

  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_sai_OFF                    (22)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_sai_WID                    ( 1)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_sai_MSK                    (0x00400000)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_sai_MIN                    (0)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_sai_MAX                    (1) // 0x00000001
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_sai_DEF                    (0x00000000)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_error_sai_HSH                    (0x012CD81C)

  #define MC0_MCDECS_CBIT_dis_cfi_spec_rsp_cpl_early_valid_OFF         (24)
  #define MC0_MCDECS_CBIT_dis_cfi_spec_rsp_cpl_early_valid_WID         ( 1)
  #define MC0_MCDECS_CBIT_dis_cfi_spec_rsp_cpl_early_valid_MSK         (0x01000000)
  #define MC0_MCDECS_CBIT_dis_cfi_spec_rsp_cpl_early_valid_MIN         (0)
  #define MC0_MCDECS_CBIT_dis_cfi_spec_rsp_cpl_early_valid_MAX         (1) // 0x00000001
  #define MC0_MCDECS_CBIT_dis_cfi_spec_rsp_cpl_early_valid_DEF         (0x00000000)
  #define MC0_MCDECS_CBIT_dis_cfi_spec_rsp_cpl_early_valid_HSH         (0x0130D81C)

  #define MC0_MCDECS_CBIT_force_sb_ep_clk_req_OFF                      (25)
  #define MC0_MCDECS_CBIT_force_sb_ep_clk_req_WID                      ( 1)
  #define MC0_MCDECS_CBIT_force_sb_ep_clk_req_MSK                      (0x02000000)
  #define MC0_MCDECS_CBIT_force_sb_ep_clk_req_MIN                      (0)
  #define MC0_MCDECS_CBIT_force_sb_ep_clk_req_MAX                      (1) // 0x00000001
  #define MC0_MCDECS_CBIT_force_sb_ep_clk_req_DEF                      (0x00000000)
  #define MC0_MCDECS_CBIT_force_sb_ep_clk_req_HSH                      (0x0132D81C)

  #define MC0_MCDECS_CBIT_dis_cfi_wr_rsp_OFF                           (26)
  #define MC0_MCDECS_CBIT_dis_cfi_wr_rsp_WID                           ( 1)
  #define MC0_MCDECS_CBIT_dis_cfi_wr_rsp_MSK                           (0x04000000)
  #define MC0_MCDECS_CBIT_dis_cfi_wr_rsp_MIN                           (0)
  #define MC0_MCDECS_CBIT_dis_cfi_wr_rsp_MAX                           (1) // 0x00000001
  #define MC0_MCDECS_CBIT_dis_cfi_wr_rsp_DEF                           (0x00000000)
  #define MC0_MCDECS_CBIT_dis_cfi_wr_rsp_HSH                           (0x0134D81C)

  #define MC0_MCDECS_CBIT_dis_iosf_sb_clk_gate_OFF                     (27)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_clk_gate_WID                     ( 1)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_clk_gate_MSK                     (0x08000000)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_clk_gate_MIN                     (0)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_clk_gate_MAX                     (1) // 0x00000001
  #define MC0_MCDECS_CBIT_dis_iosf_sb_clk_gate_DEF                     (0x00000000)
  #define MC0_MCDECS_CBIT_dis_iosf_sb_clk_gate_HSH                     (0x0136D81C)

  #define MC0_MCDECS_CBIT_dis_glbdrv_clk_gate_OFF                      (28)
  #define MC0_MCDECS_CBIT_dis_glbdrv_clk_gate_WID                      ( 1)
  #define MC0_MCDECS_CBIT_dis_glbdrv_clk_gate_MSK                      (0x10000000)
  #define MC0_MCDECS_CBIT_dis_glbdrv_clk_gate_MIN                      (0)
  #define MC0_MCDECS_CBIT_dis_glbdrv_clk_gate_MAX                      (1) // 0x00000001
  #define MC0_MCDECS_CBIT_dis_glbdrv_clk_gate_DEF                      (0x00000000)
  #define MC0_MCDECS_CBIT_dis_glbdrv_clk_gate_HSH                      (0x0138D81C)

  #define MC0_MCDECS_CBIT_dis_reg_clk_gate_OFF                         (29)
  #define MC0_MCDECS_CBIT_dis_reg_clk_gate_WID                         ( 1)
  #define MC0_MCDECS_CBIT_dis_reg_clk_gate_MSK                         (0x20000000)
  #define MC0_MCDECS_CBIT_dis_reg_clk_gate_MIN                         (0)
  #define MC0_MCDECS_CBIT_dis_reg_clk_gate_MAX                         (1) // 0x00000001
  #define MC0_MCDECS_CBIT_dis_reg_clk_gate_DEF                         (0x00000000)
  #define MC0_MCDECS_CBIT_dis_reg_clk_gate_HSH                         (0x013AD81C)

#define MC0_MAD_CHANNEL_HASH_REG                                       (0x0000D824)
#define MC0_MAD_CHANNEL_HASH_DEF                                       (0x02024D00)
#define MC0_MAD_CHANNEL_HASH_VOLATILE_BITFIELDS_MSK                    (0x00000000)

  #define MC0_MAD_CHANNEL_HASH_HASH_MASK_OFF                           ( 6)
  #define MC0_MAD_CHANNEL_HASH_HASH_MASK_WID                           (14)
  #define MC0_MAD_CHANNEL_HASH_HASH_MASK_MSK                           (0x000FFFC0)
  #define MC0_MAD_CHANNEL_HASH_HASH_MASK_MIN                           (0)
  #define MC0_MAD_CHANNEL_HASH_HASH_MASK_MAX                           (16383) // 0x00003FFF
  #define MC0_MAD_CHANNEL_HASH_HASH_MASK_DEF                           (0x00000934)
  #define MC0_MAD_CHANNEL_HASH_HASH_MASK_HSH                           (0x0E0CD824)

  #define MC0_MAD_CHANNEL_HASH_HASH_LSB_MASK_BIT_OFF                   (24)
  #define MC0_MAD_CHANNEL_HASH_HASH_LSB_MASK_BIT_WID                   ( 3)
  #define MC0_MAD_CHANNEL_HASH_HASH_LSB_MASK_BIT_MSK                   (0x07000000)
  #define MC0_MAD_CHANNEL_HASH_HASH_LSB_MASK_BIT_MIN                   (0)
  #define MC0_MAD_CHANNEL_HASH_HASH_LSB_MASK_BIT_MAX                   (7) // 0x00000007
  #define MC0_MAD_CHANNEL_HASH_HASH_LSB_MASK_BIT_DEF                   (0x00000002)
  #define MC0_MAD_CHANNEL_HASH_HASH_LSB_MASK_BIT_HSH                   (0x0330D824)

#define MC0_MAD_SUB_CHANNEL_HASH_REG                                   (0x0000D828)
#define MC0_MAD_SUB_CHANNEL_HASH_DEF                                   (0x03020E00)
#define MC0_MAD_SUB_CHANNEL_HASH_VOLATILE_BITFIELDS_MSK                (0x00000000)

  #define MC0_MAD_SUB_CHANNEL_HASH_SUBCH_HASH_MASK_OFF                 ( 6)
  #define MC0_MAD_SUB_CHANNEL_HASH_SUBCH_HASH_MASK_WID                 (14)
  #define MC0_MAD_SUB_CHANNEL_HASH_SUBCH_HASH_MASK_MSK                 (0x000FFFC0)
  #define MC0_MAD_SUB_CHANNEL_HASH_SUBCH_HASH_MASK_MIN                 (0)
  #define MC0_MAD_SUB_CHANNEL_HASH_SUBCH_HASH_MASK_MAX                 (16383) // 0x00003FFF
  #define MC0_MAD_SUB_CHANNEL_HASH_SUBCH_HASH_MASK_DEF                 (0x00000838)
  #define MC0_MAD_SUB_CHANNEL_HASH_SUBCH_HASH_MASK_HSH                 (0x0E0CD828)

  #define MC0_MAD_SUB_CHANNEL_HASH_SUBCH_HASH_LSB_MASK_BIT_OFF         (24)
  #define MC0_MAD_SUB_CHANNEL_HASH_SUBCH_HASH_LSB_MASK_BIT_WID         ( 3)
  #define MC0_MAD_SUB_CHANNEL_HASH_SUBCH_HASH_LSB_MASK_BIT_MSK         (0x07000000)
  #define MC0_MAD_SUB_CHANNEL_HASH_SUBCH_HASH_LSB_MASK_BIT_MIN         (0)
  #define MC0_MAD_SUB_CHANNEL_HASH_SUBCH_HASH_LSB_MASK_BIT_MAX         (7) // 0x00000007
  #define MC0_MAD_SUB_CHANNEL_HASH_SUBCH_HASH_LSB_MASK_BIT_DEF         (0x00000003)
  #define MC0_MAD_SUB_CHANNEL_HASH_SUBCH_HASH_LSB_MASK_BIT_HSH         (0x0330D828)

#define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_REG                       (0x0000D82C)
#define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_DEF                       (0x02020100)
#define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_VOLATILE_BITFIELDS_MSK    (0x00000000)

  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID0_OFF      ( 0)
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID0_WID      ( 8)
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID0_MSK      (0x000000FF)
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID0_MIN      (0)
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID0_MAX      (255) // 0x000000FF
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID0_DEF      (0x00000000)
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID0_HSH      (0x0800D82C)

  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID1_OFF      ( 8)
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID1_WID      ( 8)
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID1_MSK      (0x0000FF00)
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID1_MIN      (0)
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID1_MAX      (255) // 0x000000FF
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID1_DEF      (0x00000001)
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID1_HSH      (0x0810D82C)

  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID2_OFF      (16)
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID2_WID      ( 8)
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID2_MSK      (0x00FF0000)
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID2_MIN      (0)
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID2_MAX      (255) // 0x000000FF
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID2_DEF      (0x00000002)
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID2_HSH      (0x0820D82C)

  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID3_OFF      (24)
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID3_WID      ( 8)
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID3_MSK      (0xFF000000)
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID3_MIN      (0)
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID3_MAX      (255) // 0x000000FF
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID3_DEF      (0x00000002)
  #define MC0_PWM_PROGRAMMABLE_REQCOUNT_CONFIG_CMI_Source_ID3_HSH      (0x0830D82C)

#define MC0_MC_INIT_STATE_G_REG                                        (0x0000D830)
#define MC0_MC_INIT_STATE_G_DEF                                        (0x00000000)
#define MC0_MC_INIT_STATE_G_VOLATILE_BITFIELDS_MSK                     (0x000001F0)

  #define MC0_MC_INIT_STATE_G_refresh_enable_OFF                       ( 3)
  #define MC0_MC_INIT_STATE_G_refresh_enable_WID                       ( 1)
  #define MC0_MC_INIT_STATE_G_refresh_enable_MSK                       (0x00000008)
  #define MC0_MC_INIT_STATE_G_refresh_enable_MIN                       (0)
  #define MC0_MC_INIT_STATE_G_refresh_enable_MAX                       (1) // 0x00000001
  #define MC0_MC_INIT_STATE_G_refresh_enable_DEF                       (0x00000000)
  #define MC0_MC_INIT_STATE_G_refresh_enable_HSH                       (0x0106D830)

  #define MC0_MC_INIT_STATE_G_init_complete_OFF                        ( 4)
  #define MC0_MC_INIT_STATE_G_init_complete_WID                        ( 4)
  #define MC0_MC_INIT_STATE_G_init_complete_MSK                        (0x000000F0)
  #define MC0_MC_INIT_STATE_G_init_complete_MIN                        (0)
  #define MC0_MC_INIT_STATE_G_init_complete_MAX                        (15) // 0x0000000F
  #define MC0_MC_INIT_STATE_G_init_complete_DEF                        (0x00000000)
  #define MC0_MC_INIT_STATE_G_init_complete_HSH                        (0x0488D830)

  #define MC0_MC_INIT_STATE_G_sr_state_OFF                             ( 8)
  #define MC0_MC_INIT_STATE_G_sr_state_WID                             ( 1)
  #define MC0_MC_INIT_STATE_G_sr_state_MSK                             (0x00000100)
  #define MC0_MC_INIT_STATE_G_sr_state_MIN                             (0)
  #define MC0_MC_INIT_STATE_G_sr_state_MAX                             (1) // 0x00000001
  #define MC0_MC_INIT_STATE_G_sr_state_DEF                             (0x00000000)
  #define MC0_MC_INIT_STATE_G_sr_state_HSH                             (0x0190D830)

  #define MC0_MC_INIT_STATE_G_pure_srx_OFF                             ( 9)
  #define MC0_MC_INIT_STATE_G_pure_srx_WID                             ( 1)
  #define MC0_MC_INIT_STATE_G_pure_srx_MSK                             (0x00000200)
  #define MC0_MC_INIT_STATE_G_pure_srx_MIN                             (0)
  #define MC0_MC_INIT_STATE_G_pure_srx_MAX                             (1) // 0x00000001
  #define MC0_MC_INIT_STATE_G_pure_srx_DEF                             (0x00000000)
  #define MC0_MC_INIT_STATE_G_pure_srx_HSH                             (0x0112D830)

  #define MC0_MC_INIT_STATE_G_dclk_enable_OFF                          (22)
  #define MC0_MC_INIT_STATE_G_dclk_enable_WID                          ( 1)
  #define MC0_MC_INIT_STATE_G_dclk_enable_MSK                          (0x00400000)
  #define MC0_MC_INIT_STATE_G_dclk_enable_MIN                          (0)
  #define MC0_MC_INIT_STATE_G_dclk_enable_MAX                          (1) // 0x00000001
  #define MC0_MC_INIT_STATE_G_dclk_enable_DEF                          (0x00000000)
  #define MC0_MC_INIT_STATE_G_dclk_enable_HSH                          (0x012CD830)

#define MC0_MC_IP_VERSION_REG                                          (0x0000D834)
#define MC0_MC_IP_VERSION_DEF                                          (0x000A850A)
#define MC0_MC_IP_VERSION_VOLATILE_BITFIELDS_MSK                       (0xFF0FF000)

  #define MC0_MC_IP_VERSION_IP_VERSION_OFF                             ( 0)
  #define MC0_MC_IP_VERSION_IP_VERSION_WID                             ( 8)
  #define MC0_MC_IP_VERSION_IP_VERSION_MSK                             (0x000000FF)
  #define MC0_MC_IP_VERSION_IP_VERSION_MIN                             (0)
  #define MC0_MC_IP_VERSION_IP_VERSION_MAX                             (255) // 0x000000FF
  #define MC0_MC_IP_VERSION_IP_VERSION_DEF                             (0x0000000A)
  #define MC0_MC_IP_VERSION_IP_VERSION_HSH                             (0x0800D834)

  #define MC0_MC_IP_VERSION_IP_DERIVATIVE_OFF                          ( 8)
  #define MC0_MC_IP_VERSION_IP_DERIVATIVE_WID                          ( 4)
  #define MC0_MC_IP_VERSION_IP_DERIVATIVE_MSK                          (0x00000F00)
  #define MC0_MC_IP_VERSION_IP_DERIVATIVE_MIN                          (0)
  #define MC0_MC_IP_VERSION_IP_DERIVATIVE_MAX                          (15) // 0x0000000F
  #define MC0_MC_IP_VERSION_IP_DERIVATIVE_DEF                          (0x00000005)
  #define MC0_MC_IP_VERSION_IP_DERIVATIVE_HSH                          (0x0410D834)

  #define MC0_MC_IP_VERSION_IP_SEGMENT_OFF                             (12)
  #define MC0_MC_IP_VERSION_IP_SEGMENT_WID                             ( 8)
  #define MC0_MC_IP_VERSION_IP_SEGMENT_MSK                             (0x000FF000)
  #define MC0_MC_IP_VERSION_IP_SEGMENT_MIN                             (0)
  #define MC0_MC_IP_VERSION_IP_SEGMENT_MAX                             (255) // 0x000000FF
  #define MC0_MC_IP_VERSION_IP_SEGMENT_DEF                             (0x000000A8)
  #define MC0_MC_IP_VERSION_IP_SEGMENT_HSH                             (0x0898D834)

  #define MC0_MC_IP_VERSION_IP_SUBVERSION_OFF                          (24)
  #define MC0_MC_IP_VERSION_IP_SUBVERSION_WID                          ( 8)
  #define MC0_MC_IP_VERSION_IP_SUBVERSION_MSK                          (0xFF000000)
  #define MC0_MC_IP_VERSION_IP_SUBVERSION_MIN                          (0)
  #define MC0_MC_IP_VERSION_IP_SUBVERSION_MAX                          (255) // 0x000000FF
  #define MC0_MC_IP_VERSION_IP_SUBVERSION_DEF                          (0x00000000)
  #define MC0_MC_IP_VERSION_IP_SUBVERSION_HSH                          (0x08B0D834)

#define MC0_MC_RTL_VERSION_REG                                         (0x0000D838)
#define MC0_MC_RTL_VERSION_DEF                                         (0x00000000)
#define MC0_MC_RTL_VERSION_VOLATILE_BITFIELDS_MSK                      (0x00000000)

  #define MC0_MC_RTL_VERSION_IP_MODEL_OFF                              ( 0)
  #define MC0_MC_RTL_VERSION_IP_MODEL_WID                              (32)
  #define MC0_MC_RTL_VERSION_IP_MODEL_MSK                              (0xFFFFFFFF)
  #define MC0_MC_RTL_VERSION_IP_MODEL_MIN                              (0)
  #define MC0_MC_RTL_VERSION_IP_MODEL_MAX                              (4294967295) // 0xFFFFFFFF
  #define MC0_MC_RTL_VERSION_IP_MODEL_DEF                              (0x00000000)
  #define MC0_MC_RTL_VERSION_IP_MODEL_HSH                              (0x2000D838)

#define MC0_PM_OPP_SREF_IDLE_TIMER_REG                                 (0x0000D83C)
#define MC0_PM_OPP_SREF_IDLE_TIMER_DEF                                 (0x00000100)
#define MC0_PM_OPP_SREF_IDLE_TIMER_VOLATILE_BITFIELDS_MSK              (0x00000000)

  #define MC0_PM_OPP_SREF_IDLE_TIMER_Idle_timer_OFF                    ( 0)
  #define MC0_PM_OPP_SREF_IDLE_TIMER_Idle_timer_WID                    (16)
  #define MC0_PM_OPP_SREF_IDLE_TIMER_Idle_timer_MSK                    (0x0000FFFF)
  #define MC0_PM_OPP_SREF_IDLE_TIMER_Idle_timer_MIN                    (0)
  #define MC0_PM_OPP_SREF_IDLE_TIMER_Idle_timer_MAX                    (65535) // 0x0000FFFF
  #define MC0_PM_OPP_SREF_IDLE_TIMER_Idle_timer_DEF                    (0x00000100)
  #define MC0_PM_OPP_SREF_IDLE_TIMER_Idle_timer_HSH                    (0x1000D83C)

#define MC0_PWM_TOTAL_REQCOUNT_REG                                     (0x0000D840)
#define MC0_PWM_TOTAL_REQCOUNT_DEF                                     (0x0000000000000000)
#define MC0_PWM_TOTAL_REQCOUNT_VOLATILE_BITFIELDS_MSK                  (0xFFFFFFFFFFFFFFFF)

  #define MC0_PWM_TOTAL_REQCOUNT_count_OFF                             ( 0)
  #define MC0_PWM_TOTAL_REQCOUNT_count_WID                             (64)
  #define MC0_PWM_TOTAL_REQCOUNT_count_MSK                             (0xFFFFFFFFFFFFFFFFULL)
  #define MC0_PWM_TOTAL_REQCOUNT_count_MIN                             (0)
  #define MC0_PWM_TOTAL_REQCOUNT_count_MAX                             (18446744073709551615ULL) // 0xFFFFFFFFFFFFFFFF
  #define MC0_PWM_TOTAL_REQCOUNT_count_DEF                             (0x00000000)
  #define MC0_PWM_TOTAL_REQCOUNT_count_HSH                             (0x4080D840)

#define MC0_PWM_PROGRAMMABLE_REQCOUNT_0_REG                            (0x0000D848)
//Duplicate of MC0_PWM_TOTAL_REQCOUNT_REG

#define MC0_PWM_PROGRAMMABLE_REQCOUNT_1_REG                            (0x0000D850)
//Duplicate of MC0_PWM_TOTAL_REQCOUNT_REG

#define MC0_PWM_RDCAS_COUNT_REG                                        (0x0000D858)
//Duplicate of MC0_PWM_TOTAL_REQCOUNT_REG

#define MC0_PM_OPP_SREF_ENABLE_REG                                     (0x0000D860)
#define MC0_PM_OPP_SREF_ENABLE_DEF                                     (0x03200001)
#define MC0_PM_OPP_SREF_ENABLE_VOLATILE_BITFIELDS_MSK                  (0x00000018)

  #define MC0_PM_OPP_SREF_ENABLE_ALLOW_OPP_SR_OFF                      ( 0)
  #define MC0_PM_OPP_SREF_ENABLE_ALLOW_OPP_SR_WID                      ( 1)
  #define MC0_PM_OPP_SREF_ENABLE_ALLOW_OPP_SR_MSK                      (0x00000001)
  #define MC0_PM_OPP_SREF_ENABLE_ALLOW_OPP_SR_MIN                      (0)
  #define MC0_PM_OPP_SREF_ENABLE_ALLOW_OPP_SR_MAX                      (1) // 0x00000001
  #define MC0_PM_OPP_SREF_ENABLE_ALLOW_OPP_SR_DEF                      (0x00000001)
  #define MC0_PM_OPP_SREF_ENABLE_ALLOW_OPP_SR_HSH                      (0x0100D860)

  #define MC0_PM_OPP_SREF_ENABLE_pm_opp_sr_policy_OFF                  ( 3)
  #define MC0_PM_OPP_SREF_ENABLE_pm_opp_sr_policy_WID                  ( 2)
  #define MC0_PM_OPP_SREF_ENABLE_pm_opp_sr_policy_MSK                  (0x00000018)
  #define MC0_PM_OPP_SREF_ENABLE_pm_opp_sr_policy_MIN                  (0)
  #define MC0_PM_OPP_SREF_ENABLE_pm_opp_sr_policy_MAX                  (3) // 0x00000003
  #define MC0_PM_OPP_SREF_ENABLE_pm_opp_sr_policy_DEF                  (0x00000000)
  #define MC0_PM_OPP_SREF_ENABLE_pm_opp_sr_policy_HSH                  (0x0286D860)

  #define MC0_PM_OPP_SREF_ENABLE_min_duration_OFF                      (16)
  #define MC0_PM_OPP_SREF_ENABLE_min_duration_WID                      (16)
  #define MC0_PM_OPP_SREF_ENABLE_min_duration_MSK                      (0xFFFF0000)
  #define MC0_PM_OPP_SREF_ENABLE_min_duration_MIN                      (0)
  #define MC0_PM_OPP_SREF_ENABLE_min_duration_MAX                      (65535) // 0x0000FFFF
  #define MC0_PM_OPP_SREF_ENABLE_min_duration_DEF                      (0x00000320)
  #define MC0_PM_OPP_SREF_ENABLE_min_duration_HSH                      (0x1020D860)

#define MC0_READ_OCCUPANCY_COUNT_REG                                   (0x0000D868)
//Duplicate of MC0_PWM_TOTAL_REQCOUNT_REG

#define MC0_STALL_DRAIN_REG                                            (0x0000D874)
#define MC0_STALL_DRAIN_DEF                                            (0x00000000)
#define MC0_STALL_DRAIN_VOLATILE_BITFIELDS_MSK                         (0x00001011)

  #define MC0_STALL_DRAIN_stall_until_drain_OFF                        ( 0)
  #define MC0_STALL_DRAIN_stall_until_drain_WID                        ( 1)
  #define MC0_STALL_DRAIN_stall_until_drain_MSK                        (0x00000001)
  #define MC0_STALL_DRAIN_stall_until_drain_MIN                        (0)
  #define MC0_STALL_DRAIN_stall_until_drain_MAX                        (1) // 0x00000001
  #define MC0_STALL_DRAIN_stall_until_drain_DEF                        (0x00000000)
  #define MC0_STALL_DRAIN_stall_until_drain_HSH                        (0x0180D874)

  #define MC0_STALL_DRAIN_stall_input_OFF                              ( 1)
  #define MC0_STALL_DRAIN_stall_input_WID                              ( 1)
  #define MC0_STALL_DRAIN_stall_input_MSK                              (0x00000002)
  #define MC0_STALL_DRAIN_stall_input_MIN                              (0)
  #define MC0_STALL_DRAIN_stall_input_MAX                              (1) // 0x00000001
  #define MC0_STALL_DRAIN_stall_input_DEF                              (0x00000000)
  #define MC0_STALL_DRAIN_stall_input_HSH                              (0x0102D874)

  #define MC0_STALL_DRAIN_mc_drained_OFF                               ( 4)
  #define MC0_STALL_DRAIN_mc_drained_WID                               ( 1)
  #define MC0_STALL_DRAIN_mc_drained_MSK                               (0x00000010)
  #define MC0_STALL_DRAIN_mc_drained_MIN                               (0)
  #define MC0_STALL_DRAIN_mc_drained_MAX                               (1) // 0x00000001
  #define MC0_STALL_DRAIN_mc_drained_DEF                               (0x00000000)
  #define MC0_STALL_DRAIN_mc_drained_HSH                               (0x0188D874)

  #define MC0_STALL_DRAIN_stall_state_OFF                              (12)
  #define MC0_STALL_DRAIN_stall_state_WID                              ( 1)
  #define MC0_STALL_DRAIN_stall_state_MSK                              (0x00001000)
  #define MC0_STALL_DRAIN_stall_state_MIN                              (0)
  #define MC0_STALL_DRAIN_stall_state_MAX                              (1) // 0x00000001
  #define MC0_STALL_DRAIN_stall_state_DEF                              (0x00000000)
  #define MC0_STALL_DRAIN_stall_state_HSH                              (0x0198D874)

#define MC0_IPC_MC_ARB_REG                                             (0x0000D878)
#define MC0_IPC_MC_ARB_DEF                                             (0x00001844)
#define MC0_IPC_MC_ARB_VOLATILE_BITFIELDS_MSK                          (0x00000000)

  #define MC0_IPC_MC_ARB_NonVC1Threshold_OFF                           ( 0)
  #define MC0_IPC_MC_ARB_NonVC1Threshold_WID                           ( 4)
  #define MC0_IPC_MC_ARB_NonVC1Threshold_MSK                           (0x0000000F)
  #define MC0_IPC_MC_ARB_NonVC1Threshold_MIN                           (0)
  #define MC0_IPC_MC_ARB_NonVC1Threshold_MAX                           (15) // 0x0000000F
  #define MC0_IPC_MC_ARB_NonVC1Threshold_DEF                           (0x00000004)
  #define MC0_IPC_MC_ARB_NonVC1Threshold_HSH                           (0x0400D878)

  #define MC0_IPC_MC_ARB_VC1RdThreshold_OFF                            ( 4)
  #define MC0_IPC_MC_ARB_VC1RdThreshold_WID                            ( 4)
  #define MC0_IPC_MC_ARB_VC1RdThreshold_MSK                            (0x000000F0)
  #define MC0_IPC_MC_ARB_VC1RdThreshold_MIN                            (0)
  #define MC0_IPC_MC_ARB_VC1RdThreshold_MAX                            (15) // 0x0000000F
  #define MC0_IPC_MC_ARB_VC1RdThreshold_DEF                            (0x00000004)
  #define MC0_IPC_MC_ARB_VC1RdThreshold_HSH                            (0x0408D878)

  #define MC0_IPC_MC_ARB_FixedRateEn_OFF                               ( 8)
  #define MC0_IPC_MC_ARB_FixedRateEn_WID                               ( 1)
  #define MC0_IPC_MC_ARB_FixedRateEn_MSK                               (0x00000100)
  #define MC0_IPC_MC_ARB_FixedRateEn_MIN                               (0)
  #define MC0_IPC_MC_ARB_FixedRateEn_MAX                               (1) // 0x00000001
  #define MC0_IPC_MC_ARB_FixedRateEn_DEF                               (0x00000000)
  #define MC0_IPC_MC_ARB_FixedRateEn_HSH                               (0x0110D878)

  #define MC0_IPC_MC_ARB_HIGH_PRIO_LIM_OFF                             ( 9)
  #define MC0_IPC_MC_ARB_HIGH_PRIO_LIM_WID                             ( 3)
  #define MC0_IPC_MC_ARB_HIGH_PRIO_LIM_MSK                             (0x00000E00)
  #define MC0_IPC_MC_ARB_HIGH_PRIO_LIM_MIN                             (0)
  #define MC0_IPC_MC_ARB_HIGH_PRIO_LIM_MAX                             (7) // 0x00000007
  #define MC0_IPC_MC_ARB_HIGH_PRIO_LIM_DEF                             (0x00000004)
  #define MC0_IPC_MC_ARB_HIGH_PRIO_LIM_HSH                             (0x0312D878)

  #define MC0_IPC_MC_ARB_LOW_PRIO_LIM_OFF                              (12)
  #define MC0_IPC_MC_ARB_LOW_PRIO_LIM_WID                              ( 3)
  #define MC0_IPC_MC_ARB_LOW_PRIO_LIM_MSK                              (0x00007000)
  #define MC0_IPC_MC_ARB_LOW_PRIO_LIM_MIN                              (0)
  #define MC0_IPC_MC_ARB_LOW_PRIO_LIM_MAX                              (7) // 0x00000007
  #define MC0_IPC_MC_ARB_LOW_PRIO_LIM_DEF                              (0x00000001)
  #define MC0_IPC_MC_ARB_LOW_PRIO_LIM_HSH                              (0x0318D878)

  #define MC0_IPC_MC_ARB_spare_OFF                                     (15)
  #define MC0_IPC_MC_ARB_spare_WID                                     ( 8)
  #define MC0_IPC_MC_ARB_spare_MSK                                     (0x007F8000)
  #define MC0_IPC_MC_ARB_spare_MIN                                     (0)
  #define MC0_IPC_MC_ARB_spare_MAX                                     (255) // 0x000000FF
  #define MC0_IPC_MC_ARB_spare_DEF                                     (0x00000000)
  #define MC0_IPC_MC_ARB_spare_HSH                                     (0x081ED878)

#define MC0_IPC_MC_DEC_ARB_REG                                         (0x0000D87C)
//Duplicate of MC0_IPC_MC_ARB_REG

#define MC0_QUEUE_CREDIT_C_REG                                         (0x0000D880)
#define MC0_QUEUE_CREDIT_C_DEF                                         (0x24506030)
#define MC0_QUEUE_CREDIT_C_VOLATILE_BITFIELDS_MSK                      (0x00000000)

  #define MC0_QUEUE_CREDIT_C_RPQ_count_OFF                             ( 0)
  #define MC0_QUEUE_CREDIT_C_RPQ_count_WID                             ( 6)
  #define MC0_QUEUE_CREDIT_C_RPQ_count_MSK                             (0x0000003F)
  #define MC0_QUEUE_CREDIT_C_RPQ_count_MIN                             (0)
  #define MC0_QUEUE_CREDIT_C_RPQ_count_MAX                             (48) // 0x00000030
  #define MC0_QUEUE_CREDIT_C_RPQ_count_DEF                             (0x00000030)
  #define MC0_QUEUE_CREDIT_C_RPQ_count_HSH                             (0x0600D880)

  #define MC0_QUEUE_CREDIT_C_WPQ_count_OFF                             ( 8)
  #define MC0_QUEUE_CREDIT_C_WPQ_count_WID                             ( 7)
  #define MC0_QUEUE_CREDIT_C_WPQ_count_MSK                             (0x00007F00)
  #define MC0_QUEUE_CREDIT_C_WPQ_count_MIN                             (0)
  #define MC0_QUEUE_CREDIT_C_WPQ_count_MAX                             (96) // 0x00000060
  #define MC0_QUEUE_CREDIT_C_WPQ_count_DEF                             (0x00000060)
  #define MC0_QUEUE_CREDIT_C_WPQ_count_HSH                             (0x0710D880)

  #define MC0_QUEUE_CREDIT_C_IPQ_count_OFF                             (16)
  #define MC0_QUEUE_CREDIT_C_IPQ_count_WID                             ( 5)
  #define MC0_QUEUE_CREDIT_C_IPQ_count_MSK                             (0x001F0000)
  #define MC0_QUEUE_CREDIT_C_IPQ_count_MIN                             (0)
  #define MC0_QUEUE_CREDIT_C_IPQ_count_MAX                             (16) // 0x00000010
  #define MC0_QUEUE_CREDIT_C_IPQ_count_DEF                             (0x00000010)
  #define MC0_QUEUE_CREDIT_C_IPQ_count_HSH                             (0x0520D880)

  #define MC0_QUEUE_CREDIT_C_WPQ_MinSlotsToReq_OFF                     (21)
  #define MC0_QUEUE_CREDIT_C_WPQ_MinSlotsToReq_WID                     ( 4)
  #define MC0_QUEUE_CREDIT_C_WPQ_MinSlotsToReq_MSK                     (0x01E00000)
  #define MC0_QUEUE_CREDIT_C_WPQ_MinSlotsToReq_MIN                     (2)
  #define MC0_QUEUE_CREDIT_C_WPQ_MinSlotsToReq_MAX                     (15) // 0x0000000F
  #define MC0_QUEUE_CREDIT_C_WPQ_MinSlotsToReq_DEF                     (0x00000002)
  #define MC0_QUEUE_CREDIT_C_WPQ_MinSlotsToReq_HSH                     (0x042AD880)

  #define MC0_QUEUE_CREDIT_C_IPQ_MinSlotsToReq_OFF                     (25)
  #define MC0_QUEUE_CREDIT_C_IPQ_MinSlotsToReq_WID                     ( 3)
  #define MC0_QUEUE_CREDIT_C_IPQ_MinSlotsToReq_MSK                     (0x0E000000)
  #define MC0_QUEUE_CREDIT_C_IPQ_MinSlotsToReq_MIN                     (2)
  #define MC0_QUEUE_CREDIT_C_IPQ_MinSlotsToReq_MAX                     (7) // 0x00000007
  #define MC0_QUEUE_CREDIT_C_IPQ_MinSlotsToReq_DEF                     (0x00000002)
  #define MC0_QUEUE_CREDIT_C_IPQ_MinSlotsToReq_HSH                     (0x0332D880)

  #define MC0_QUEUE_CREDIT_C_RPQ_MinSlotsToReq_OFF                     (28)
  #define MC0_QUEUE_CREDIT_C_RPQ_MinSlotsToReq_WID                     ( 4)
  #define MC0_QUEUE_CREDIT_C_RPQ_MinSlotsToReq_MSK                     (0xF0000000)
  #define MC0_QUEUE_CREDIT_C_RPQ_MinSlotsToReq_MIN                     (2)
  #define MC0_QUEUE_CREDIT_C_RPQ_MinSlotsToReq_MAX                     (15) // 0x0000000F
  #define MC0_QUEUE_CREDIT_C_RPQ_MinSlotsToReq_DEF                     (0x00000002)
  #define MC0_QUEUE_CREDIT_C_RPQ_MinSlotsToReq_HSH                     (0x0438D880)

#define MC0_ECC_INJ_ADDR_COMPARE_REG                                   (0x0000D888)
#define MC0_ECC_INJ_ADDR_COMPARE_DEF                                   (0x0000000000000000)
#define MC0_ECC_INJ_ADDR_COMPARE_VOLATILE_BITFIELDS_MSK                (0x0000000000000000)

  #define MC0_ECC_INJ_ADDR_COMPARE_Address_OFF                         ( 0)
  #define MC0_ECC_INJ_ADDR_COMPARE_Address_WID                         (33)
  #define MC0_ECC_INJ_ADDR_COMPARE_Address_MSK                         (0x00000001FFFFFFFFULL)
  #define MC0_ECC_INJ_ADDR_COMPARE_Address_MIN                         (0)
  #define MC0_ECC_INJ_ADDR_COMPARE_Address_MAX                         (8589934591ULL) // 0x1FFFFFFFF
  #define MC0_ECC_INJ_ADDR_COMPARE_Address_DEF                         (0x00000000)
  #define MC0_ECC_INJ_ADDR_COMPARE_Address_HSH                         (0x6100D888)

#define MC0_PWM_WRCAS_COUNT_REG                                        (0x0000D8A0)
//Duplicate of MC0_PWM_TOTAL_REQCOUNT_REG

#define MC0_PWM_COMMAND_COUNT_REG                                      (0x0000D8A8)
//Duplicate of MC0_PWM_TOTAL_REQCOUNT_REG

#define MC0_TOLUD_REG                                                  (0x0000D8BC)
#define MC0_TOLUD_DEF                                                  (0x00100000)
#define MC0_TOLUD_VOLATILE_BITFIELDS_MSK                               (0x00000000)

  #define MC0_TOLUD_TOLUD_OFF                                          (20)
  #define MC0_TOLUD_TOLUD_WID                                          (12)
  #define MC0_TOLUD_TOLUD_MSK                                          (0xFFF00000)
  #define MC0_TOLUD_TOLUD_MIN                                          (0)
  #define MC0_TOLUD_TOLUD_MAX                                          (4095) // 0x00000FFF
  #define MC0_TOLUD_TOLUD_DEF                                          (0x00000001)
  #define MC0_TOLUD_TOLUD_HSH                                          (0x0C28D8BC)

#define MC0_PWM_ACT_COUNT_REG                                          (0x0000D8C0)
//Duplicate of MC0_PWM_TOTAL_REQCOUNT_REG

#define MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_REG                         (0x0000D900)
#define MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_DEF                         (0x0000000000000000)
#define MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_VOLATILE_BITFIELDS_MSK      (0x0000000000000000)

  #define MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_sys_addr_OFF              ( 0)
  #define MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_sys_addr_WID              (33)
  #define MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_sys_addr_MSK              (0x00000001FFFFFFFFULL)
  #define MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_sys_addr_MIN              (0)
  #define MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_sys_addr_MAX              (8589934591ULL) // 0x1FFFFFFFF
  #define MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_sys_addr_DEF              (0x00000000)
  #define MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_sys_addr_HSH              (0x6100D900)

#define MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_1_REG                         (0x0000D908)
//Duplicate of MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_REG

#define MC0_GDXC_DDR_SYS_ADD_FILTER_MATCH_0_REG                        (0x0000D910)
//Duplicate of MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_REG

#define MC0_GDXC_DDR_SYS_ADD_FILTER_MATCH_1_REG                        (0x0000D918)
//Duplicate of MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_REG

#define MC0_GDXC_DDR_SYS_ADD_TRIGGER_MASK_REG                          (0x0000D920)
//Duplicate of MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_REG

#define MC0_GDXC_DDR_SYS_ADD_TRIGGER_MATCH_REG                         (0x0000D928)
//Duplicate of MC0_GDXC_DDR_SYS_ADD_FILTER_MASK_0_REG

#define MC0_SC_QOS_REG                                                 (0x0000D930)
#define MC0_SC_QOS_DEF                                                 (0x0000B05A00B40384)
#define MC0_SC_QOS_VOLATILE_BITFIELDS_MSK                              (0x0000000000000000)

  #define MC0_SC_QOS_Isoch_time_window_OFF                             ( 0)
  #define MC0_SC_QOS_Isoch_time_window_WID                             (17)
  #define MC0_SC_QOS_Isoch_time_window_MSK                             (0x0001FFFF)
  #define MC0_SC_QOS_Isoch_time_window_MIN                             (0)
  #define MC0_SC_QOS_Isoch_time_window_MAX                             (131071) // 0x0001FFFF
  #define MC0_SC_QOS_Isoch_time_window_DEF                             (0x00000384)
  #define MC0_SC_QOS_Isoch_time_window_HSH                             (0x5100D930)

  #define MC0_SC_QOS_Write_starvation_window_OFF                       (17)
  #define MC0_SC_QOS_Write_starvation_window_WID                       (11)
  #define MC0_SC_QOS_Write_starvation_window_MSK                       (0x0FFE0000)
  #define MC0_SC_QOS_Write_starvation_window_MIN                       (0)
  #define MC0_SC_QOS_Write_starvation_window_MAX                       (2047) // 0x000007FF
  #define MC0_SC_QOS_Write_starvation_window_DEF                       (0x0000005A)
  #define MC0_SC_QOS_Write_starvation_window_HSH                       (0x4B22D930)

  #define MC0_SC_QOS_Read_starvation_window_OFF                        (32)
  #define MC0_SC_QOS_Read_starvation_window_WID                        (11)
  #define MC0_SC_QOS_Read_starvation_window_MSK                        (0x000007FF00000000ULL)
  #define MC0_SC_QOS_Read_starvation_window_MIN                        (0)
  #define MC0_SC_QOS_Read_starvation_window_MAX                        (2047) // 0x000007FF
  #define MC0_SC_QOS_Read_starvation_window_DEF                        (0x0000005A)
  #define MC0_SC_QOS_Read_starvation_window_HSH                        (0x4B40D930)

  #define MC0_SC_QOS_VC0_read_count_OFF                                (43)
  #define MC0_SC_QOS_VC0_read_count_WID                                ( 9)
  #define MC0_SC_QOS_VC0_read_count_MSK                                (0x000FF80000000000ULL)
  #define MC0_SC_QOS_VC0_read_count_MIN                                (0)
  #define MC0_SC_QOS_VC0_read_count_MAX                                (511) // 0x000001FF
  #define MC0_SC_QOS_VC0_read_count_DEF                                (0x00000016)
  #define MC0_SC_QOS_VC0_read_count_HSH                                (0x4956D930)

#define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_REG                         (0x0000D938)
#define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_DEF                         (0x0000000F)
#define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_VOLATILE_BITFIELDS_MSK      (0x00000000)

  #define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_GLB_GRACE_CNT_OFF         ( 0)
  #define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_GLB_GRACE_CNT_WID         ( 8)
  #define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_GLB_GRACE_CNT_MSK         (0x000000FF)
  #define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_GLB_GRACE_CNT_MIN         (15)
  #define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_GLB_GRACE_CNT_MAX         (255) // 0x000000FF
  #define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_GLB_GRACE_CNT_DEF         (0x0000000F)
  #define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_GLB_GRACE_CNT_HSH         (0x0800D938)

  #define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_GLB_DRV_GATE_DIS_OFF      ( 8)
  #define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_GLB_DRV_GATE_DIS_WID      ( 1)
  #define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_GLB_DRV_GATE_DIS_MSK      (0x00000100)
  #define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_GLB_DRV_GATE_DIS_MIN      (0)
  #define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_GLB_DRV_GATE_DIS_MAX      (1) // 0x00000001
  #define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_GLB_DRV_GATE_DIS_DEF      (0x00000000)
  #define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_GLB_DRV_GATE_DIS_HSH      (0x0110D938)

  #define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_qclk_global_driver_override_to_dclk_OFF ( 9)
  #define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_qclk_global_driver_override_to_dclk_WID ( 1)
  #define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_qclk_global_driver_override_to_dclk_MSK (0x00000200)
  #define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_qclk_global_driver_override_to_dclk_MIN (0)
  #define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_qclk_global_driver_override_to_dclk_MAX (1) // 0x00000001
  #define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_qclk_global_driver_override_to_dclk_DEF (0x00000000)
  #define MC0_MCMAINS_GLOBAL_DRIVER_GATE_CFG_qclk_global_driver_override_to_dclk_HSH (0x0112D938)

#define MC0_PM_CONTROL_REG                                             (0x0000D93C)
#define MC0_PM_CONTROL_DEF                                             (0x00801C30)
#define MC0_PM_CONTROL_VOLATILE_BITFIELDS_MSK                          (0x1FFF0000)

  #define MC0_PM_CONTROL_delay_PM_Ack_cycles_OFF                       ( 0)
  #define MC0_PM_CONTROL_delay_PM_Ack_cycles_WID                       (10)
  #define MC0_PM_CONTROL_delay_PM_Ack_cycles_MSK                       (0x000003FF)
  #define MC0_PM_CONTROL_delay_PM_Ack_cycles_MIN                       (0)
  #define MC0_PM_CONTROL_delay_PM_Ack_cycles_MAX                       (1023) // 0x000003FF
  #define MC0_PM_CONTROL_delay_PM_Ack_cycles_DEF                       (0x00000030)
  #define MC0_PM_CONTROL_delay_PM_Ack_cycles_HSH                       (0x0A00D93C)

  #define MC0_PM_CONTROL_R1_Ack_reset_bgf_run_OFF                      (10)
  #define MC0_PM_CONTROL_R1_Ack_reset_bgf_run_WID                      ( 1)
  #define MC0_PM_CONTROL_R1_Ack_reset_bgf_run_MSK                      (0x00000400)
  #define MC0_PM_CONTROL_R1_Ack_reset_bgf_run_MIN                      (0)
  #define MC0_PM_CONTROL_R1_Ack_reset_bgf_run_MAX                      (1) // 0x00000001
  #define MC0_PM_CONTROL_R1_Ack_reset_bgf_run_DEF                      (0x00000001)
  #define MC0_PM_CONTROL_R1_Ack_reset_bgf_run_HSH                      (0x0114D93C)

  #define MC0_PM_CONTROL_SAGV_Ack_reset_bgf_run_OFF                    (11)
  #define MC0_PM_CONTROL_SAGV_Ack_reset_bgf_run_WID                    ( 1)
  #define MC0_PM_CONTROL_SAGV_Ack_reset_bgf_run_MSK                    (0x00000800)
  #define MC0_PM_CONTROL_SAGV_Ack_reset_bgf_run_MIN                    (0)
  #define MC0_PM_CONTROL_SAGV_Ack_reset_bgf_run_MAX                    (1) // 0x00000001
  #define MC0_PM_CONTROL_SAGV_Ack_reset_bgf_run_DEF                    (0x00000001)
  #define MC0_PM_CONTROL_SAGV_Ack_reset_bgf_run_HSH                    (0x0116D93C)

  #define MC0_PM_CONTROL_init_complete_reset_bgf_run_OFF               (12)
  #define MC0_PM_CONTROL_init_complete_reset_bgf_run_WID               ( 1)
  #define MC0_PM_CONTROL_init_complete_reset_bgf_run_MSK               (0x00001000)
  #define MC0_PM_CONTROL_init_complete_reset_bgf_run_MIN               (0)
  #define MC0_PM_CONTROL_init_complete_reset_bgf_run_MAX               (1) // 0x00000001
  #define MC0_PM_CONTROL_init_complete_reset_bgf_run_DEF               (0x00000001)
  #define MC0_PM_CONTROL_init_complete_reset_bgf_run_HSH               (0x0118D93C)

  #define MC0_PM_CONTROL_Override_DDRPLDrainDone_Cbit_OFF              (13)
  #define MC0_PM_CONTROL_Override_DDRPLDrainDone_Cbit_WID              ( 1)
  #define MC0_PM_CONTROL_Override_DDRPLDrainDone_Cbit_MSK              (0x00002000)
  #define MC0_PM_CONTROL_Override_DDRPLDrainDone_Cbit_MIN              (0)
  #define MC0_PM_CONTROL_Override_DDRPLDrainDone_Cbit_MAX              (1) // 0x00000001
  #define MC0_PM_CONTROL_Override_DDRPLDrainDone_Cbit_DEF              (0x00000000)
  #define MC0_PM_CONTROL_Override_DDRPLDrainDone_Cbit_HSH              (0x011AD93C)

  #define MC0_PM_CONTROL_en_lpmode_exit_any_rd_early_wakeup_OFF        (14)
  #define MC0_PM_CONTROL_en_lpmode_exit_any_rd_early_wakeup_WID        ( 1)
  #define MC0_PM_CONTROL_en_lpmode_exit_any_rd_early_wakeup_MSK        (0x00004000)
  #define MC0_PM_CONTROL_en_lpmode_exit_any_rd_early_wakeup_MIN        (0)
  #define MC0_PM_CONTROL_en_lpmode_exit_any_rd_early_wakeup_MAX        (1) // 0x00000001
  #define MC0_PM_CONTROL_en_lpmode_exit_any_rd_early_wakeup_DEF        (0x00000000)
  #define MC0_PM_CONTROL_en_lpmode_exit_any_rd_early_wakeup_HSH        (0x011CD93C)

  #define MC0_PM_CONTROL_PM_REQ_Status_Clear_Select_OFF                (15)
  #define MC0_PM_CONTROL_PM_REQ_Status_Clear_Select_WID                ( 1)
  #define MC0_PM_CONTROL_PM_REQ_Status_Clear_Select_MSK                (0x00008000)
  #define MC0_PM_CONTROL_PM_REQ_Status_Clear_Select_MIN                (0)
  #define MC0_PM_CONTROL_PM_REQ_Status_Clear_Select_MAX                (1) // 0x00000001
  #define MC0_PM_CONTROL_PM_REQ_Status_Clear_Select_DEF                (0x00000000)
  #define MC0_PM_CONTROL_PM_REQ_Status_Clear_Select_HSH                (0x011ED93C)

  #define MC0_PM_CONTROL_PM_REQ_R0_received_OFF                        (16)
  #define MC0_PM_CONTROL_PM_REQ_R0_received_WID                        ( 1)
  #define MC0_PM_CONTROL_PM_REQ_R0_received_MSK                        (0x00010000)
  #define MC0_PM_CONTROL_PM_REQ_R0_received_MIN                        (0)
  #define MC0_PM_CONTROL_PM_REQ_R0_received_MAX                        (1) // 0x00000001
  #define MC0_PM_CONTROL_PM_REQ_R0_received_DEF                        (0x00000000)
  #define MC0_PM_CONTROL_PM_REQ_R0_received_HSH                        (0x01A0D93C)

  #define MC0_PM_CONTROL_PM_REQ_R0_SAGV_received_OFF                   (17)
  #define MC0_PM_CONTROL_PM_REQ_R0_SAGV_received_WID                   ( 1)
  #define MC0_PM_CONTROL_PM_REQ_R0_SAGV_received_MSK                   (0x00020000)
  #define MC0_PM_CONTROL_PM_REQ_R0_SAGV_received_MIN                   (0)
  #define MC0_PM_CONTROL_PM_REQ_R0_SAGV_received_MAX                   (1) // 0x00000001
  #define MC0_PM_CONTROL_PM_REQ_R0_SAGV_received_DEF                   (0x00000000)
  #define MC0_PM_CONTROL_PM_REQ_R0_SAGV_received_HSH                   (0x01A2D93C)

  #define MC0_PM_CONTROL_PM_REQ_R1_received_OFF                        (18)
  #define MC0_PM_CONTROL_PM_REQ_R1_received_WID                        ( 1)
  #define MC0_PM_CONTROL_PM_REQ_R1_received_MSK                        (0x00040000)
  #define MC0_PM_CONTROL_PM_REQ_R1_received_MIN                        (0)
  #define MC0_PM_CONTROL_PM_REQ_R1_received_MAX                        (1) // 0x00000001
  #define MC0_PM_CONTROL_PM_REQ_R1_received_DEF                        (0x00000000)
  #define MC0_PM_CONTROL_PM_REQ_R1_received_HSH                        (0x01A4D93C)

  #define MC0_PM_CONTROL_PM_REQ_R1_deep_sr_received_OFF                (19)
  #define MC0_PM_CONTROL_PM_REQ_R1_deep_sr_received_WID                ( 1)
  #define MC0_PM_CONTROL_PM_REQ_R1_deep_sr_received_MSK                (0x00080000)
  #define MC0_PM_CONTROL_PM_REQ_R1_deep_sr_received_MIN                (0)
  #define MC0_PM_CONTROL_PM_REQ_R1_deep_sr_received_MAX                (1) // 0x00000001
  #define MC0_PM_CONTROL_PM_REQ_R1_deep_sr_received_DEF                (0x00000000)
  #define MC0_PM_CONTROL_PM_REQ_R1_deep_sr_received_HSH                (0x01A6D93C)

  #define MC0_PM_CONTROL_PM_REQ_UnBlock_received_OFF                   (20)
  #define MC0_PM_CONTROL_PM_REQ_UnBlock_received_WID                   ( 1)
  #define MC0_PM_CONTROL_PM_REQ_UnBlock_received_MSK                   (0x00100000)
  #define MC0_PM_CONTROL_PM_REQ_UnBlock_received_MIN                   (0)
  #define MC0_PM_CONTROL_PM_REQ_UnBlock_received_MAX                   (1) // 0x00000001
  #define MC0_PM_CONTROL_PM_REQ_UnBlock_received_DEF                   (0x00000000)
  #define MC0_PM_CONTROL_PM_REQ_UnBlock_received_HSH                   (0x01A8D93C)

  #define MC0_PM_CONTROL_PM_REQ_Acked_OFF                              (21)
  #define MC0_PM_CONTROL_PM_REQ_Acked_WID                              ( 1)
  #define MC0_PM_CONTROL_PM_REQ_Acked_MSK                              (0x00200000)
  #define MC0_PM_CONTROL_PM_REQ_Acked_MIN                              (0)
  #define MC0_PM_CONTROL_PM_REQ_Acked_MAX                              (1) // 0x00000001
  #define MC0_PM_CONTROL_PM_REQ_Acked_DEF                              (0x00000000)
  #define MC0_PM_CONTROL_PM_REQ_Acked_HSH                              (0x01AAD93C)

  #define MC0_PM_CONTROL_MC_State_blocked_R0_OFF                       (22)
  #define MC0_PM_CONTROL_MC_State_blocked_R0_WID                       ( 1)
  #define MC0_PM_CONTROL_MC_State_blocked_R0_MSK                       (0x00400000)
  #define MC0_PM_CONTROL_MC_State_blocked_R0_MIN                       (0)
  #define MC0_PM_CONTROL_MC_State_blocked_R0_MAX                       (1) // 0x00000001
  #define MC0_PM_CONTROL_MC_State_blocked_R0_DEF                       (0x00000000)
  #define MC0_PM_CONTROL_MC_State_blocked_R0_HSH                       (0x01ACD93C)

  #define MC0_PM_CONTROL_MC_State_blocked_R1_OFF                       (23)
  #define MC0_PM_CONTROL_MC_State_blocked_R1_WID                       ( 1)
  #define MC0_PM_CONTROL_MC_State_blocked_R1_MSK                       (0x00800000)
  #define MC0_PM_CONTROL_MC_State_blocked_R1_MIN                       (0)
  #define MC0_PM_CONTROL_MC_State_blocked_R1_MAX                       (1) // 0x00000001
  #define MC0_PM_CONTROL_MC_State_blocked_R1_DEF                       (0x00000001)
  #define MC0_PM_CONTROL_MC_State_blocked_R1_HSH                       (0x01AED93C)

  #define MC0_PM_CONTROL_PM_REQ_VRCG_Enabled_received_OFF              (24)
  #define MC0_PM_CONTROL_PM_REQ_VRCG_Enabled_received_WID              ( 1)
  #define MC0_PM_CONTROL_PM_REQ_VRCG_Enabled_received_MSK              (0x01000000)
  #define MC0_PM_CONTROL_PM_REQ_VRCG_Enabled_received_MIN              (0)
  #define MC0_PM_CONTROL_PM_REQ_VRCG_Enabled_received_MAX              (1) // 0x00000001
  #define MC0_PM_CONTROL_PM_REQ_VRCG_Enabled_received_DEF              (0x00000000)
  #define MC0_PM_CONTROL_PM_REQ_VRCG_Enabled_received_HSH              (0x01B0D93C)

  #define MC0_PM_CONTROL_PM_REQ_VRCG_Disabled_received_OFF             (25)
  #define MC0_PM_CONTROL_PM_REQ_VRCG_Disabled_received_WID             ( 1)
  #define MC0_PM_CONTROL_PM_REQ_VRCG_Disabled_received_MSK             (0x02000000)
  #define MC0_PM_CONTROL_PM_REQ_VRCG_Disabled_received_MIN             (0)
  #define MC0_PM_CONTROL_PM_REQ_VRCG_Disabled_received_MAX             (1) // 0x00000001
  #define MC0_PM_CONTROL_PM_REQ_VRCG_Disabled_received_DEF             (0x00000000)
  #define MC0_PM_CONTROL_PM_REQ_VRCG_Disabled_received_HSH             (0x01B2D93C)

  #define MC0_PM_CONTROL_PM_REQ_OppSR_cmd_0_received_OFF               (26)
  #define MC0_PM_CONTROL_PM_REQ_OppSR_cmd_0_received_WID               ( 1)
  #define MC0_PM_CONTROL_PM_REQ_OppSR_cmd_0_received_MSK               (0x04000000)
  #define MC0_PM_CONTROL_PM_REQ_OppSR_cmd_0_received_MIN               (0)
  #define MC0_PM_CONTROL_PM_REQ_OppSR_cmd_0_received_MAX               (1) // 0x00000001
  #define MC0_PM_CONTROL_PM_REQ_OppSR_cmd_0_received_DEF               (0x00000000)
  #define MC0_PM_CONTROL_PM_REQ_OppSR_cmd_0_received_HSH               (0x01B4D93C)

  #define MC0_PM_CONTROL_PM_REQ_OppSR_cmd_1_received_OFF               (27)
  #define MC0_PM_CONTROL_PM_REQ_OppSR_cmd_1_received_WID               ( 1)
  #define MC0_PM_CONTROL_PM_REQ_OppSR_cmd_1_received_MSK               (0x08000000)
  #define MC0_PM_CONTROL_PM_REQ_OppSR_cmd_1_received_MIN               (0)
  #define MC0_PM_CONTROL_PM_REQ_OppSR_cmd_1_received_MAX               (1) // 0x00000001
  #define MC0_PM_CONTROL_PM_REQ_OppSR_cmd_1_received_DEF               (0x00000000)
  #define MC0_PM_CONTROL_PM_REQ_OppSR_cmd_1_received_HSH               (0x01B6D93C)

  #define MC0_PM_CONTROL_PM_REQ_OppSR_cmd_2_received_OFF               (28)
  #define MC0_PM_CONTROL_PM_REQ_OppSR_cmd_2_received_WID               ( 1)
  #define MC0_PM_CONTROL_PM_REQ_OppSR_cmd_2_received_MSK               (0x10000000)
  #define MC0_PM_CONTROL_PM_REQ_OppSR_cmd_2_received_MIN               (0)
  #define MC0_PM_CONTROL_PM_REQ_OppSR_cmd_2_received_MAX               (1) // 0x00000001
  #define MC0_PM_CONTROL_PM_REQ_OppSR_cmd_2_received_DEF               (0x00000000)
  #define MC0_PM_CONTROL_PM_REQ_OppSR_cmd_2_received_HSH               (0x01B8D93C)

  #define MC0_PM_CONTROL_en_pdx_early_fabric_wakeup_OFF                (29)
  #define MC0_PM_CONTROL_en_pdx_early_fabric_wakeup_WID                ( 1)
  #define MC0_PM_CONTROL_en_pdx_early_fabric_wakeup_MSK                (0x20000000)
  #define MC0_PM_CONTROL_en_pdx_early_fabric_wakeup_MIN                (0)
  #define MC0_PM_CONTROL_en_pdx_early_fabric_wakeup_MAX                (1) // 0x00000001
  #define MC0_PM_CONTROL_en_pdx_early_fabric_wakeup_DEF                (0x00000000)
  #define MC0_PM_CONTROL_en_pdx_early_fabric_wakeup_HSH                (0x013AD93C)

  #define MC0_PM_CONTROL_en_srx_early_fabric_wakeup_OFF                (30)
  #define MC0_PM_CONTROL_en_srx_early_fabric_wakeup_WID                ( 1)
  #define MC0_PM_CONTROL_en_srx_early_fabric_wakeup_MSK                (0x40000000)
  #define MC0_PM_CONTROL_en_srx_early_fabric_wakeup_MIN                (0)
  #define MC0_PM_CONTROL_en_srx_early_fabric_wakeup_MAX                (1) // 0x00000001
  #define MC0_PM_CONTROL_en_srx_early_fabric_wakeup_DEF                (0x00000000)
  #define MC0_PM_CONTROL_en_srx_early_fabric_wakeup_HSH                (0x013CD93C)

  #define MC0_PM_CONTROL_en_pdx_any_rd_early_wakeup_OFF                (31)
  #define MC0_PM_CONTROL_en_pdx_any_rd_early_wakeup_WID                ( 1)
  #define MC0_PM_CONTROL_en_pdx_any_rd_early_wakeup_MSK                (0x80000000)
  #define MC0_PM_CONTROL_en_pdx_any_rd_early_wakeup_MIN                (0)
  #define MC0_PM_CONTROL_en_pdx_any_rd_early_wakeup_MAX                (1) // 0x00000001
  #define MC0_PM_CONTROL_en_pdx_any_rd_early_wakeup_DEF                (0x00000000)
  #define MC0_PM_CONTROL_en_pdx_any_rd_early_wakeup_HSH                (0x013ED93C)

#define MC0_CLOCK_SPINE_GATE_CFG_REG                                   (0x0000D940)
#define MC0_CLOCK_SPINE_GATE_CFG_DEF                                   (0x00000010)
#define MC0_CLOCK_SPINE_GATE_CFG_VOLATILE_BITFIELDS_MSK                (0x00000000)

  #define MC0_CLOCK_SPINE_GATE_CFG_CLOCK_SPINE_GRACE_CNT_OFF           ( 0)
  #define MC0_CLOCK_SPINE_GATE_CFG_CLOCK_SPINE_GRACE_CNT_WID           ( 8)
  #define MC0_CLOCK_SPINE_GATE_CFG_CLOCK_SPINE_GRACE_CNT_MSK           (0x000000FF)
  #define MC0_CLOCK_SPINE_GATE_CFG_CLOCK_SPINE_GRACE_CNT_MIN           (15)
  #define MC0_CLOCK_SPINE_GATE_CFG_CLOCK_SPINE_GRACE_CNT_MAX           (255) // 0x000000FF
  #define MC0_CLOCK_SPINE_GATE_CFG_CLOCK_SPINE_GRACE_CNT_DEF           (0x00000010)
  #define MC0_CLOCK_SPINE_GATE_CFG_CLOCK_SPINE_GRACE_CNT_HSH           (0x0800D940)

  #define MC0_CLOCK_SPINE_GATE_CFG_CLOCK_SPINE_GATE_DIS_OFF            ( 8)
  #define MC0_CLOCK_SPINE_GATE_CFG_CLOCK_SPINE_GATE_DIS_WID            ( 1)
  #define MC0_CLOCK_SPINE_GATE_CFG_CLOCK_SPINE_GATE_DIS_MSK            (0x00000100)
  #define MC0_CLOCK_SPINE_GATE_CFG_CLOCK_SPINE_GATE_DIS_MIN            (0)
  #define MC0_CLOCK_SPINE_GATE_CFG_CLOCK_SPINE_GATE_DIS_MAX            (1) // 0x00000001
  #define MC0_CLOCK_SPINE_GATE_CFG_CLOCK_SPINE_GATE_DIS_DEF            (0x00000000)
  #define MC0_CLOCK_SPINE_GATE_CFG_CLOCK_SPINE_GATE_DIS_HSH            (0x0110D940)

#define MC0_PWM_COUNTERS_DURATION_REG                                  (0x0000D948)
#define MC0_PWM_COUNTERS_DURATION_DEF                                  (0x0000000000000000)
#define MC0_PWM_COUNTERS_DURATION_VOLATILE_BITFIELDS_MSK               (0xFFFFFFFFFFFFFFFF)

  #define MC0_PWM_COUNTERS_DURATION_Count_OFF                          ( 0)
  #define MC0_PWM_COUNTERS_DURATION_Count_WID                          (64)
  #define MC0_PWM_COUNTERS_DURATION_Count_MSK                          (0xFFFFFFFFFFFFFFFFULL)
  #define MC0_PWM_COUNTERS_DURATION_Count_MIN                          (0)
  #define MC0_PWM_COUNTERS_DURATION_Count_MAX                          (18446744073709551615ULL) // 0xFFFFFFFFFFFFFFFF
  #define MC0_PWM_COUNTERS_DURATION_Count_DEF                          (0x00000000)
  #define MC0_PWM_COUNTERS_DURATION_Count_HSH                          (0x4080D948)

#define MC0_MCDECS_SECOND_CBIT_REG                                     (0x0000D954)
#define MC0_MCDECS_SECOND_CBIT_DEF                                     (0x00300000)
#define MC0_MCDECS_SECOND_CBIT_VOLATILE_BITFIELDS_MSK                  (0x00000000)

  #define MC0_MCDECS_SECOND_CBIT_cmi_req_stall_enable_OFF              ( 0)
  #define MC0_MCDECS_SECOND_CBIT_cmi_req_stall_enable_WID              ( 1)
  #define MC0_MCDECS_SECOND_CBIT_cmi_req_stall_enable_MSK              (0x00000001)
  #define MC0_MCDECS_SECOND_CBIT_cmi_req_stall_enable_MIN              (0)
  #define MC0_MCDECS_SECOND_CBIT_cmi_req_stall_enable_MAX              (1) // 0x00000001
  #define MC0_MCDECS_SECOND_CBIT_cmi_req_stall_enable_DEF              (0x00000000)
  #define MC0_MCDECS_SECOND_CBIT_cmi_req_stall_enable_HSH              (0x0100D954)

  #define MC0_MCDECS_SECOND_CBIT_cmi_req_stall_phase_OFF               ( 1)
  #define MC0_MCDECS_SECOND_CBIT_cmi_req_stall_phase_WID               ( 1)
  #define MC0_MCDECS_SECOND_CBIT_cmi_req_stall_phase_MSK               (0x00000002)
  #define MC0_MCDECS_SECOND_CBIT_cmi_req_stall_phase_MIN               (0)
  #define MC0_MCDECS_SECOND_CBIT_cmi_req_stall_phase_MAX               (1) // 0x00000001
  #define MC0_MCDECS_SECOND_CBIT_cmi_req_stall_phase_DEF               (0x00000000)
  #define MC0_MCDECS_SECOND_CBIT_cmi_req_stall_phase_HSH               (0x0102D954)

  #define MC0_MCDECS_SECOND_CBIT_block_scheduler_OFF                   ( 2)
  #define MC0_MCDECS_SECOND_CBIT_block_scheduler_WID                   ( 4)
  #define MC0_MCDECS_SECOND_CBIT_block_scheduler_MSK                   (0x0000003C)
  #define MC0_MCDECS_SECOND_CBIT_block_scheduler_MIN                   (0)
  #define MC0_MCDECS_SECOND_CBIT_block_scheduler_MAX                   (15) // 0x0000000F
  #define MC0_MCDECS_SECOND_CBIT_block_scheduler_DEF                   (0x00000000)
  #define MC0_MCDECS_SECOND_CBIT_block_scheduler_HSH                   (0x0404D954)

  #define MC0_MCDECS_SECOND_CBIT_dis_other_mc_stolen_ref_OFF           ( 8)
  #define MC0_MCDECS_SECOND_CBIT_dis_other_mc_stolen_ref_WID           ( 1)
  #define MC0_MCDECS_SECOND_CBIT_dis_other_mc_stolen_ref_MSK           (0x00000100)
  #define MC0_MCDECS_SECOND_CBIT_dis_other_mc_stolen_ref_MIN           (0)
  #define MC0_MCDECS_SECOND_CBIT_dis_other_mc_stolen_ref_MAX           (1) // 0x00000001
  #define MC0_MCDECS_SECOND_CBIT_dis_other_mc_stolen_ref_DEF           (0x00000000)
  #define MC0_MCDECS_SECOND_CBIT_dis_other_mc_stolen_ref_HSH           (0x0110D954)

  #define MC0_MCDECS_SECOND_CBIT_init_complete_override_en_OFF         (12)
  #define MC0_MCDECS_SECOND_CBIT_init_complete_override_en_WID         ( 4)
  #define MC0_MCDECS_SECOND_CBIT_init_complete_override_en_MSK         (0x0000F000)
  #define MC0_MCDECS_SECOND_CBIT_init_complete_override_en_MIN         (0)
  #define MC0_MCDECS_SECOND_CBIT_init_complete_override_en_MAX         (15) // 0x0000000F
  #define MC0_MCDECS_SECOND_CBIT_init_complete_override_en_DEF         (0x00000000)
  #define MC0_MCDECS_SECOND_CBIT_init_complete_override_en_HSH         (0x0418D954)

  #define MC0_MCDECS_SECOND_CBIT_init_complete_override_value_OFF      (16)
  #define MC0_MCDECS_SECOND_CBIT_init_complete_override_value_WID      ( 4)
  #define MC0_MCDECS_SECOND_CBIT_init_complete_override_value_MSK      (0x000F0000)
  #define MC0_MCDECS_SECOND_CBIT_init_complete_override_value_MIN      (0)
  #define MC0_MCDECS_SECOND_CBIT_init_complete_override_value_MAX      (15) // 0x0000000F
  #define MC0_MCDECS_SECOND_CBIT_init_complete_override_value_DEF      (0x00000000)
  #define MC0_MCDECS_SECOND_CBIT_init_complete_override_value_HSH      (0x0420D954)

  #define MC0_MCDECS_SECOND_CBIT_prevent_sr_on_vc1_high_prio_OFF       (20)
  #define MC0_MCDECS_SECOND_CBIT_prevent_sr_on_vc1_high_prio_WID       ( 1)
  #define MC0_MCDECS_SECOND_CBIT_prevent_sr_on_vc1_high_prio_MSK       (0x00100000)
  #define MC0_MCDECS_SECOND_CBIT_prevent_sr_on_vc1_high_prio_MIN       (0)
  #define MC0_MCDECS_SECOND_CBIT_prevent_sr_on_vc1_high_prio_MAX       (1) // 0x00000001
  #define MC0_MCDECS_SECOND_CBIT_prevent_sr_on_vc1_high_prio_DEF       (0x00000001)
  #define MC0_MCDECS_SECOND_CBIT_prevent_sr_on_vc1_high_prio_HSH       (0x0128D954)

  #define MC0_MCDECS_SECOND_CBIT_use_initcomplete_ch0_only_OFF         (21)
  #define MC0_MCDECS_SECOND_CBIT_use_initcomplete_ch0_only_WID         ( 1)
  #define MC0_MCDECS_SECOND_CBIT_use_initcomplete_ch0_only_MSK         (0x00200000)
  #define MC0_MCDECS_SECOND_CBIT_use_initcomplete_ch0_only_MIN         (0)
  #define MC0_MCDECS_SECOND_CBIT_use_initcomplete_ch0_only_MAX         (1) // 0x00000001
  #define MC0_MCDECS_SECOND_CBIT_use_initcomplete_ch0_only_DEF         (0x00000001)
  #define MC0_MCDECS_SECOND_CBIT_use_initcomplete_ch0_only_HSH         (0x012AD954)

  #define MC0_MCDECS_SECOND_CBIT_Allow_RH_Debt_in_SR_OFF               (23)
  #define MC0_MCDECS_SECOND_CBIT_Allow_RH_Debt_in_SR_WID               ( 1)
  #define MC0_MCDECS_SECOND_CBIT_Allow_RH_Debt_in_SR_MSK               (0x00800000)
  #define MC0_MCDECS_SECOND_CBIT_Allow_RH_Debt_in_SR_MIN               (0)
  #define MC0_MCDECS_SECOND_CBIT_Allow_RH_Debt_in_SR_MAX               (1) // 0x00000001
  #define MC0_MCDECS_SECOND_CBIT_Allow_RH_Debt_in_SR_DEF               (0x00000000)
  #define MC0_MCDECS_SECOND_CBIT_Allow_RH_Debt_in_SR_HSH               (0x012ED954)

  #define MC0_MCDECS_SECOND_CBIT_delay_qsync_OFF                       (24)
  #define MC0_MCDECS_SECOND_CBIT_delay_qsync_WID                       ( 2)
  #define MC0_MCDECS_SECOND_CBIT_delay_qsync_MSK                       (0x03000000)
  #define MC0_MCDECS_SECOND_CBIT_delay_qsync_MIN                       (0)
  #define MC0_MCDECS_SECOND_CBIT_delay_qsync_MAX                       (3) // 0x00000003
  #define MC0_MCDECS_SECOND_CBIT_delay_qsync_DEF                       (0x00000000)
  #define MC0_MCDECS_SECOND_CBIT_delay_qsync_HSH                       (0x0230D954)

  #define MC0_MCDECS_SECOND_CBIT_hvm_iso_override_OFF                  (26)
  #define MC0_MCDECS_SECOND_CBIT_hvm_iso_override_WID                  ( 1)
  #define MC0_MCDECS_SECOND_CBIT_hvm_iso_override_MSK                  (0x04000000)
  #define MC0_MCDECS_SECOND_CBIT_hvm_iso_override_MIN                  (0)
  #define MC0_MCDECS_SECOND_CBIT_hvm_iso_override_MAX                  (1) // 0x00000001
  #define MC0_MCDECS_SECOND_CBIT_hvm_iso_override_DEF                  (0x00000000)
  #define MC0_MCDECS_SECOND_CBIT_hvm_iso_override_HSH                  (0x0134D954)

#define MC0_ECC_INJ_ADDR_MASK_REG                                      (0x0000D958)
#define MC0_ECC_INJ_ADDR_MASK_DEF                                      (0x00000001FFFFFFFF)
#define MC0_ECC_INJ_ADDR_MASK_VOLATILE_BITFIELDS_MSK                   (0x0000000000000000)

  #define MC0_ECC_INJ_ADDR_MASK_Address_OFF                            ( 0)
  #define MC0_ECC_INJ_ADDR_MASK_Address_WID                            (33)
  #define MC0_ECC_INJ_ADDR_MASK_Address_MSK                            (0x00000001FFFFFFFFULL)
  #define MC0_ECC_INJ_ADDR_MASK_Address_MIN                            (0)
  #define MC0_ECC_INJ_ADDR_MASK_Address_MAX                            (8589934591ULL) // 0x1FFFFFFFF
  #define MC0_ECC_INJ_ADDR_MASK_Address_DEF                            (0x1FFFFFFFF)
  #define MC0_ECC_INJ_ADDR_MASK_Address_HSH                            (0x6100D958)

#define MC0_SC_QOS2_REG                                                (0x0000D960)
#define MC0_SC_QOS2_DEF                                                (0x0000005A038400E1)
#define MC0_SC_QOS2_VOLATILE_BITFIELDS_MSK                             (0x0000000000000000)

  #define MC0_SC_QOS2_RW_Isoch_time_window_OFF                         ( 0)
  #define MC0_SC_QOS2_RW_Isoch_time_window_WID                         (17)
  #define MC0_SC_QOS2_RW_Isoch_time_window_MSK                         (0x0001FFFF)
  #define MC0_SC_QOS2_RW_Isoch_time_window_MIN                         (0)
  #define MC0_SC_QOS2_RW_Isoch_time_window_MAX                         (131071) // 0x0001FFFF
  #define MC0_SC_QOS2_RW_Isoch_time_window_DEF                         (0x000000E1)
  #define MC0_SC_QOS2_RW_Isoch_time_window_HSH                         (0x5100D960)

  #define MC0_SC_QOS2_RW_Write_starvation_window_OFF                   (17)
  #define MC0_SC_QOS2_RW_Write_starvation_window_WID                   (11)
  #define MC0_SC_QOS2_RW_Write_starvation_window_MSK                   (0x0FFE0000)
  #define MC0_SC_QOS2_RW_Write_starvation_window_MIN                   (0)
  #define MC0_SC_QOS2_RW_Write_starvation_window_MAX                   (2047) // 0x000007FF
  #define MC0_SC_QOS2_RW_Write_starvation_window_DEF                   (0x000001C2)
  #define MC0_SC_QOS2_RW_Write_starvation_window_HSH                   (0x4B22D960)

  #define MC0_SC_QOS2_RW_Read_starvation_window_OFF                    (32)
  #define MC0_SC_QOS2_RW_Read_starvation_window_WID                    (11)
  #define MC0_SC_QOS2_RW_Read_starvation_window_MSK                    (0x000007FF00000000ULL)
  #define MC0_SC_QOS2_RW_Read_starvation_window_MIN                    (0)
  #define MC0_SC_QOS2_RW_Read_starvation_window_MAX                    (2047) // 0x000007FF
  #define MC0_SC_QOS2_RW_Read_starvation_window_DEF                    (0x0000005A)
  #define MC0_SC_QOS2_RW_Read_starvation_window_HSH                    (0x4B40D960)

#define MC0_SC_QOS3_REG                                                (0x0000D968)
#define MC0_SC_QOS3_DEF                                                (0x0000782F)
#define MC0_SC_QOS3_VOLATILE_BITFIELDS_MSK                             (0x00000000)

  #define MC0_SC_QOS3_Yellow_Decay_x128_OFF                            ( 0)
  #define MC0_SC_QOS3_Yellow_Decay_x128_WID                            ( 9)
  #define MC0_SC_QOS3_Yellow_Decay_x128_MSK                            (0x000001FF)
  #define MC0_SC_QOS3_Yellow_Decay_x128_MIN                            (0)
  #define MC0_SC_QOS3_Yellow_Decay_x128_MAX                            (511) // 0x000001FF
  #define MC0_SC_QOS3_Yellow_Decay_x128_DEF                            (0x0000002F)
  #define MC0_SC_QOS3_Yellow_Decay_x128_HSH                            (0x0900D968)

  #define MC0_SC_QOS3_Yellow_Threshold_OFF                             ( 9)
  #define MC0_SC_QOS3_Yellow_Threshold_WID                             (10)
  #define MC0_SC_QOS3_Yellow_Threshold_MSK                             (0x0007FE00)
  #define MC0_SC_QOS3_Yellow_Threshold_MIN                             (0)
  #define MC0_SC_QOS3_Yellow_Threshold_MAX                             (1023) // 0x000003FF
  #define MC0_SC_QOS3_Yellow_Threshold_DEF                             (0x0000003C)
  #define MC0_SC_QOS3_Yellow_Threshold_HSH                             (0x0A12D968)

#define MC0_NORMALMODE_CFG_REG                                         (0x0000D96C)
#define MC0_NORMALMODE_CFG_DEF                                         (0x00000000)
#define MC0_NORMALMODE_CFG_VOLATILE_BITFIELDS_MSK                      (0x00000008)

  #define MC0_NORMALMODE_CFG_normalmode_OFF                            ( 0)
  #define MC0_NORMALMODE_CFG_normalmode_WID                            ( 1)
  #define MC0_NORMALMODE_CFG_normalmode_MSK                            (0x00000001)
  #define MC0_NORMALMODE_CFG_normalmode_MIN                            (0)
  #define MC0_NORMALMODE_CFG_normalmode_MAX                            (1) // 0x00000001
  #define MC0_NORMALMODE_CFG_normalmode_DEF                            (0x00000000)
  #define MC0_NORMALMODE_CFG_normalmode_HSH                            (0x0100D96C)

  #define MC0_NORMALMODE_CFG_Force_SR_State_OFF                        ( 1)
  #define MC0_NORMALMODE_CFG_Force_SR_State_WID                        ( 2)
  #define MC0_NORMALMODE_CFG_Force_SR_State_MSK                        (0x00000006)
  #define MC0_NORMALMODE_CFG_Force_SR_State_MIN                        (0)
  #define MC0_NORMALMODE_CFG_Force_SR_State_MAX                        (3) // 0x00000003
  #define MC0_NORMALMODE_CFG_Force_SR_State_DEF                        (0x00000000)
  #define MC0_NORMALMODE_CFG_Force_SR_State_HSH                        (0x0202D96C)

  #define MC0_NORMALMODE_CFG_SR_is_forced_OFF                          ( 3)
  #define MC0_NORMALMODE_CFG_SR_is_forced_WID                          ( 1)
  #define MC0_NORMALMODE_CFG_SR_is_forced_MSK                          (0x00000008)
  #define MC0_NORMALMODE_CFG_SR_is_forced_MIN                          (0)
  #define MC0_NORMALMODE_CFG_SR_is_forced_MAX                          (1) // 0x00000001
  #define MC0_NORMALMODE_CFG_SR_is_forced_DEF                          (0x00000000)
  #define MC0_NORMALMODE_CFG_SR_is_forced_HSH                          (0x0186D96C)

#define MC0_MC_CPGC_CONTROL_REG                                        (0x0000D970)
#define MC0_MC_CPGC_CONTROL_DEF                                        (0x00000000)
#define MC0_MC_CPGC_CONTROL_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define MC0_MC_CPGC_CONTROL_reset_cpgc_OFF                           ( 2)
  #define MC0_MC_CPGC_CONTROL_reset_cpgc_WID                           ( 1)
  #define MC0_MC_CPGC_CONTROL_reset_cpgc_MSK                           (0x00000004)
  #define MC0_MC_CPGC_CONTROL_reset_cpgc_MIN                           (0)
  #define MC0_MC_CPGC_CONTROL_reset_cpgc_MAX                           (1) // 0x00000001
  #define MC0_MC_CPGC_CONTROL_reset_cpgc_DEF                           (0x00000000)
  #define MC0_MC_CPGC_CONTROL_reset_cpgc_HSH                           (0x0104D970)

  #define MC0_MC_CPGC_CONTROL_cpgc_active_OFF                          (24)
  #define MC0_MC_CPGC_CONTROL_cpgc_active_WID                          ( 1)
  #define MC0_MC_CPGC_CONTROL_cpgc_active_MSK                          (0x01000000)
  #define MC0_MC_CPGC_CONTROL_cpgc_active_MIN                          (0)
  #define MC0_MC_CPGC_CONTROL_cpgc_active_MAX                          (1) // 0x00000001
  #define MC0_MC_CPGC_CONTROL_cpgc_active_DEF                          (0x00000000)
  #define MC0_MC_CPGC_CONTROL_cpgc_active_HSH                          (0x0130D970)

  #define MC0_MC_CPGC_CONTROL_cpgc_ecc_byte_OFF                        (28)
  #define MC0_MC_CPGC_CONTROL_cpgc_ecc_byte_WID                        ( 3)
  #define MC0_MC_CPGC_CONTROL_cpgc_ecc_byte_MSK                        (0x70000000)
  #define MC0_MC_CPGC_CONTROL_cpgc_ecc_byte_MIN                        (0)
  #define MC0_MC_CPGC_CONTROL_cpgc_ecc_byte_MAX                        (7) // 0x00000007
  #define MC0_MC_CPGC_CONTROL_cpgc_ecc_byte_DEF                        (0x00000000)
  #define MC0_MC_CPGC_CONTROL_cpgc_ecc_byte_HSH                        (0x0338D970)

  #define MC0_MC_CPGC_CONTROL_cpgc_ecc_enable_OFF                      (31)
  #define MC0_MC_CPGC_CONTROL_cpgc_ecc_enable_WID                      ( 1)
  #define MC0_MC_CPGC_CONTROL_cpgc_ecc_enable_MSK                      (0x80000000)
  #define MC0_MC_CPGC_CONTROL_cpgc_ecc_enable_MIN                      (0)
  #define MC0_MC_CPGC_CONTROL_cpgc_ecc_enable_MAX                      (1) // 0x00000001
  #define MC0_MC_CPGC_CONTROL_cpgc_ecc_enable_DEF                      (0x00000000)
  #define MC0_MC_CPGC_CONTROL_cpgc_ecc_enable_HSH                      (0x013ED970)

#define MC0_MC_CPGC_CONTROL_UARCH_REG                                  (0x0000D974)
#define MC0_MC_CPGC_CONTROL_UARCH_DEF                                  (0x00000000)
#define MC0_MC_CPGC_CONTROL_UARCH_VOLATILE_BITFIELDS_MSK               (0x00000000)

  #define MC0_MC_CPGC_CONTROL_UARCH_in_order_ingress_OFF               ( 0)
  #define MC0_MC_CPGC_CONTROL_UARCH_in_order_ingress_WID               ( 1)
  #define MC0_MC_CPGC_CONTROL_UARCH_in_order_ingress_MSK               (0x00000001)
  #define MC0_MC_CPGC_CONTROL_UARCH_in_order_ingress_MIN               (0)
  #define MC0_MC_CPGC_CONTROL_UARCH_in_order_ingress_MAX               (1) // 0x00000001
  #define MC0_MC_CPGC_CONTROL_UARCH_in_order_ingress_DEF               (0x00000000)
  #define MC0_MC_CPGC_CONTROL_UARCH_in_order_ingress_HSH               (0x0100D974)

  #define MC0_MC_CPGC_CONTROL_UARCH_lock_on_active_cpgc_cmi_ism_OFF    ( 1)
  #define MC0_MC_CPGC_CONTROL_UARCH_lock_on_active_cpgc_cmi_ism_WID    ( 1)
  #define MC0_MC_CPGC_CONTROL_UARCH_lock_on_active_cpgc_cmi_ism_MSK    (0x00000002)
  #define MC0_MC_CPGC_CONTROL_UARCH_lock_on_active_cpgc_cmi_ism_MIN    (0)
  #define MC0_MC_CPGC_CONTROL_UARCH_lock_on_active_cpgc_cmi_ism_MAX    (1) // 0x00000001
  #define MC0_MC_CPGC_CONTROL_UARCH_lock_on_active_cpgc_cmi_ism_DEF    (0x00000000)
  #define MC0_MC_CPGC_CONTROL_UARCH_lock_on_active_cpgc_cmi_ism_HSH    (0x0102D974)

  #define MC0_MC_CPGC_CONTROL_UARCH_delay_cpl_data_OFF                 ( 3)
  #define MC0_MC_CPGC_CONTROL_UARCH_delay_cpl_data_WID                 ( 1)
  #define MC0_MC_CPGC_CONTROL_UARCH_delay_cpl_data_MSK                 (0x00000008)
  #define MC0_MC_CPGC_CONTROL_UARCH_delay_cpl_data_MIN                 (0)
  #define MC0_MC_CPGC_CONTROL_UARCH_delay_cpl_data_MAX                 (1) // 0x00000001
  #define MC0_MC_CPGC_CONTROL_UARCH_delay_cpl_data_DEF                 (0x00000000)
  #define MC0_MC_CPGC_CONTROL_UARCH_delay_cpl_data_HSH                 (0x0106D974)

  #define MC0_MC_CPGC_CONTROL_UARCH_delay_cpl_info_OFF                 ( 4)
  #define MC0_MC_CPGC_CONTROL_UARCH_delay_cpl_info_WID                 ( 1)
  #define MC0_MC_CPGC_CONTROL_UARCH_delay_cpl_info_MSK                 (0x00000010)
  #define MC0_MC_CPGC_CONTROL_UARCH_delay_cpl_info_MIN                 (0)
  #define MC0_MC_CPGC_CONTROL_UARCH_delay_cpl_info_MAX                 (1) // 0x00000001
  #define MC0_MC_CPGC_CONTROL_UARCH_delay_cpl_info_DEF                 (0x00000000)
  #define MC0_MC_CPGC_CONTROL_UARCH_delay_cpl_info_HSH                 (0x0108D974)

  #define MC0_MC_CPGC_CONTROL_UARCH_halfcachelinemode_OFF              ( 5)
  #define MC0_MC_CPGC_CONTROL_UARCH_halfcachelinemode_WID              ( 1)
  #define MC0_MC_CPGC_CONTROL_UARCH_halfcachelinemode_MSK              (0x00000020)
  #define MC0_MC_CPGC_CONTROL_UARCH_halfcachelinemode_MIN              (0)
  #define MC0_MC_CPGC_CONTROL_UARCH_halfcachelinemode_MAX              (1) // 0x00000001
  #define MC0_MC_CPGC_CONTROL_UARCH_halfcachelinemode_DEF              (0x00000000)
  #define MC0_MC_CPGC_CONTROL_UARCH_halfcachelinemode_HSH              (0x010AD974)

  #define MC0_MC_CPGC_CONTROL_UARCH_cmd_req_noninterleave_OFF          ( 6)
  #define MC0_MC_CPGC_CONTROL_UARCH_cmd_req_noninterleave_WID          ( 1)
  #define MC0_MC_CPGC_CONTROL_UARCH_cmd_req_noninterleave_MSK          (0x00000040)
  #define MC0_MC_CPGC_CONTROL_UARCH_cmd_req_noninterleave_MIN          (0)
  #define MC0_MC_CPGC_CONTROL_UARCH_cmd_req_noninterleave_MAX          (1) // 0x00000001
  #define MC0_MC_CPGC_CONTROL_UARCH_cmd_req_noninterleave_DEF          (0x00000000)
  #define MC0_MC_CPGC_CONTROL_UARCH_cmd_req_noninterleave_HSH          (0x010CD974)

#define MC0_PARITYERRLOG_REG                                           (0x0000D9A0)
#define MC0_PARITYERRLOG_DEF                                           (0x0000000000000000)
#define MC0_PARITYERRLOG_VOLATILE_BITFIELDS_MSK                        (0xF800007FFFFFFFFF)

  #define MC0_PARITYERRLOG_ERR_ADDRESS_OFF                             ( 0)
  #define MC0_PARITYERRLOG_ERR_ADDRESS_WID                             (39)
  #define MC0_PARITYERRLOG_ERR_ADDRESS_MSK                             (0x0000007FFFFFFFFFULL)
  #define MC0_PARITYERRLOG_ERR_ADDRESS_MIN                             (0)
  #define MC0_PARITYERRLOG_ERR_ADDRESS_MAX                             (549755813887ULL) // 0x7FFFFFFFFF
  #define MC0_PARITYERRLOG_ERR_ADDRESS_DEF                             (0x00000000)
  #define MC0_PARITYERRLOG_ERR_ADDRESS_HSH                             (0x6780D9A0)

  #define MC0_PARITYERRLOG_RESERVED_OFF                                (39)
  #define MC0_PARITYERRLOG_RESERVED_WID                                (20)
  #define MC0_PARITYERRLOG_RESERVED_MSK                                (0x07FFFF8000000000ULL)
  #define MC0_PARITYERRLOG_RESERVED_MIN                                (0)
  #define MC0_PARITYERRLOG_RESERVED_MAX                                (1048575) // 0x000FFFFF
  #define MC0_PARITYERRLOG_RESERVED_DEF                                (0x00000000)
  #define MC0_PARITYERRLOG_RESERVED_HSH                                (0x544ED9A0)

  #define MC0_PARITYERRLOG_ERR_SRC_OFF                                 (59)
  #define MC0_PARITYERRLOG_ERR_SRC_WID                                 ( 1)
  #define MC0_PARITYERRLOG_ERR_SRC_MSK                                 (0x0800000000000000ULL)
  #define MC0_PARITYERRLOG_ERR_SRC_MIN                                 (0)
  #define MC0_PARITYERRLOG_ERR_SRC_MAX                                 (1) // 0x00000001
  #define MC0_PARITYERRLOG_ERR_SRC_DEF                                 (0x00000000)
  #define MC0_PARITYERRLOG_ERR_SRC_HSH                                 (0x41F6D9A0)

  #define MC0_PARITYERRLOG_ERR_TYPE_OFF                                (60)
  #define MC0_PARITYERRLOG_ERR_TYPE_WID                                ( 2)
  #define MC0_PARITYERRLOG_ERR_TYPE_MSK                                (0x3000000000000000ULL)
  #define MC0_PARITYERRLOG_ERR_TYPE_MIN                                (0)
  #define MC0_PARITYERRLOG_ERR_TYPE_MAX                                (3) // 0x00000003
  #define MC0_PARITYERRLOG_ERR_TYPE_DEF                                (0x00000000)
  #define MC0_PARITYERRLOG_ERR_TYPE_HSH                                (0x42F8D9A0)

  #define MC0_PARITYERRLOG_ERR_STS_OVERFLOW_OFF                        (62)
  #define MC0_PARITYERRLOG_ERR_STS_OVERFLOW_WID                        ( 1)
  #define MC0_PARITYERRLOG_ERR_STS_OVERFLOW_MSK                        (0x4000000000000000ULL)
  #define MC0_PARITYERRLOG_ERR_STS_OVERFLOW_MIN                        (0)
  #define MC0_PARITYERRLOG_ERR_STS_OVERFLOW_MAX                        (1) // 0x00000001
  #define MC0_PARITYERRLOG_ERR_STS_OVERFLOW_DEF                        (0x00000000)
  #define MC0_PARITYERRLOG_ERR_STS_OVERFLOW_HSH                        (0x41FCD9A0)

  #define MC0_PARITYERRLOG_ERR_STS_OFF                                 (63)
  #define MC0_PARITYERRLOG_ERR_STS_WID                                 ( 1)
  #define MC0_PARITYERRLOG_ERR_STS_MSK                                 (0x8000000000000000ULL)
  #define MC0_PARITYERRLOG_ERR_STS_MIN                                 (0)
  #define MC0_PARITYERRLOG_ERR_STS_MAX                                 (1) // 0x00000001
  #define MC0_PARITYERRLOG_ERR_STS_DEF                                 (0x00000000)
  #define MC0_PARITYERRLOG_ERR_STS_HSH                                 (0x41FED9A0)

#define MC0_PARITY_ERR_INJ_REG                                         (0x0000D9A8)
#define MC0_PARITY_ERR_INJ_DEF                                         (0x00000000)
#define MC0_PARITY_ERR_INJ_VOLATILE_BITFIELDS_MSK                      (0x000007FF)

  #define MC0_PARITY_ERR_INJ_DATA_ERR_EN_OFF                           ( 0)
  #define MC0_PARITY_ERR_INJ_DATA_ERR_EN_WID                           ( 8)
  #define MC0_PARITY_ERR_INJ_DATA_ERR_EN_MSK                           (0x000000FF)
  #define MC0_PARITY_ERR_INJ_DATA_ERR_EN_MIN                           (0)
  #define MC0_PARITY_ERR_INJ_DATA_ERR_EN_MAX                           (255) // 0x000000FF
  #define MC0_PARITY_ERR_INJ_DATA_ERR_EN_DEF                           (0x00000000)
  #define MC0_PARITY_ERR_INJ_DATA_ERR_EN_HSH                           (0x0880D9A8)

  #define MC0_PARITY_ERR_INJ_REQ_ADDR_ERR_EN_OFF                       ( 8)
  #define MC0_PARITY_ERR_INJ_REQ_ADDR_ERR_EN_WID                       ( 1)
  #define MC0_PARITY_ERR_INJ_REQ_ADDR_ERR_EN_MSK                       (0x00000100)
  #define MC0_PARITY_ERR_INJ_REQ_ADDR_ERR_EN_MIN                       (0)
  #define MC0_PARITY_ERR_INJ_REQ_ADDR_ERR_EN_MAX                       (1) // 0x00000001
  #define MC0_PARITY_ERR_INJ_REQ_ADDR_ERR_EN_DEF                       (0x00000000)
  #define MC0_PARITY_ERR_INJ_REQ_ADDR_ERR_EN_HSH                       (0x0190D9A8)

  #define MC0_PARITY_ERR_INJ_RWD_HEADER_ERR_EN_OFF                     ( 9)
  #define MC0_PARITY_ERR_INJ_RWD_HEADER_ERR_EN_WID                     ( 2)
  #define MC0_PARITY_ERR_INJ_RWD_HEADER_ERR_EN_MSK                     (0x00000600)
  #define MC0_PARITY_ERR_INJ_RWD_HEADER_ERR_EN_MIN                     (0)
  #define MC0_PARITY_ERR_INJ_RWD_HEADER_ERR_EN_MAX                     (3) // 0x00000003
  #define MC0_PARITY_ERR_INJ_RWD_HEADER_ERR_EN_DEF                     (0x00000000)
  #define MC0_PARITY_ERR_INJ_RWD_HEADER_ERR_EN_HSH                     (0x0292D9A8)

  #define MC0_PARITY_ERR_INJ_RSVD_OFF                                  (11)
  #define MC0_PARITY_ERR_INJ_RSVD_WID                                  ( 5)
  #define MC0_PARITY_ERR_INJ_RSVD_MSK                                  (0x0000F800)
  #define MC0_PARITY_ERR_INJ_RSVD_MIN                                  (0)
  #define MC0_PARITY_ERR_INJ_RSVD_MAX                                  (31) // 0x0000001F
  #define MC0_PARITY_ERR_INJ_RSVD_DEF                                  (0x00000000)
  #define MC0_PARITY_ERR_INJ_RSVD_HSH                                  (0x0516D9A8)

  #define MC0_PARITY_ERR_INJ_ERR_INJ_MASK_OFF                          (16)
  #define MC0_PARITY_ERR_INJ_ERR_INJ_MASK_WID                          ( 6)
  #define MC0_PARITY_ERR_INJ_ERR_INJ_MASK_MSK                          (0x003F0000)
  #define MC0_PARITY_ERR_INJ_ERR_INJ_MASK_MIN                          (0)
  #define MC0_PARITY_ERR_INJ_ERR_INJ_MASK_MAX                          (63) // 0x0000003F
  #define MC0_PARITY_ERR_INJ_ERR_INJ_MASK_DEF                          (0x00000000)
  #define MC0_PARITY_ERR_INJ_ERR_INJ_MASK_HSH                          (0x0620D9A8)

  #define MC0_PARITY_ERR_INJ_RSVD2_OFF                                 (22)
  #define MC0_PARITY_ERR_INJ_RSVD2_WID                                 ( 9)
  #define MC0_PARITY_ERR_INJ_RSVD2_MSK                                 (0x7FC00000)
  #define MC0_PARITY_ERR_INJ_RSVD2_MIN                                 (0)
  #define MC0_PARITY_ERR_INJ_RSVD2_MAX                                 (511) // 0x000001FF
  #define MC0_PARITY_ERR_INJ_RSVD2_DEF                                 (0x00000000)
  #define MC0_PARITY_ERR_INJ_RSVD2_HSH                                 (0x092CD9A8)

  #define MC0_PARITY_ERR_INJ_DATA_ERR_INJ_SRC_OFF                      (31)
  #define MC0_PARITY_ERR_INJ_DATA_ERR_INJ_SRC_WID                      ( 1)
  #define MC0_PARITY_ERR_INJ_DATA_ERR_INJ_SRC_MSK                      (0x80000000)
  #define MC0_PARITY_ERR_INJ_DATA_ERR_INJ_SRC_MIN                      (0)
  #define MC0_PARITY_ERR_INJ_DATA_ERR_INJ_SRC_MAX                      (1) // 0x00000001
  #define MC0_PARITY_ERR_INJ_DATA_ERR_INJ_SRC_DEF                      (0x00000000)
  #define MC0_PARITY_ERR_INJ_DATA_ERR_INJ_SRC_HSH                      (0x013ED9A8)

#define MC0_PARITY_CONTROL_REG                                         (0x0000D9B4)
#define MC0_PARITY_CONTROL_DEF                                         (0x0000003F)
#define MC0_PARITY_CONTROL_VOLATILE_BITFIELDS_MSK                      (0x00000000)

  #define MC0_PARITY_CONTROL_REQ_ADDR_PARITY_EN_OFF                    ( 0)
  #define MC0_PARITY_CONTROL_REQ_ADDR_PARITY_EN_WID                    ( 1)
  #define MC0_PARITY_CONTROL_REQ_ADDR_PARITY_EN_MSK                    (0x00000001)
  #define MC0_PARITY_CONTROL_REQ_ADDR_PARITY_EN_MIN                    (0)
  #define MC0_PARITY_CONTROL_REQ_ADDR_PARITY_EN_MAX                    (1) // 0x00000001
  #define MC0_PARITY_CONTROL_REQ_ADDR_PARITY_EN_DEF                    (0x00000001)
  #define MC0_PARITY_CONTROL_REQ_ADDR_PARITY_EN_HSH                    (0x0100D9B4)

  #define MC0_PARITY_CONTROL_RWD_HEADER_PARITY_EN_OFF                  ( 1)
  #define MC0_PARITY_CONTROL_RWD_HEADER_PARITY_EN_WID                  ( 1)
  #define MC0_PARITY_CONTROL_RWD_HEADER_PARITY_EN_MSK                  (0x00000002)
  #define MC0_PARITY_CONTROL_RWD_HEADER_PARITY_EN_MIN                  (0)
  #define MC0_PARITY_CONTROL_RWD_HEADER_PARITY_EN_MAX                  (1) // 0x00000001
  #define MC0_PARITY_CONTROL_RWD_HEADER_PARITY_EN_DEF                  (0x00000001)
  #define MC0_PARITY_CONTROL_RWD_HEADER_PARITY_EN_HSH                  (0x0102D9B4)

  #define MC0_PARITY_CONTROL_WDATA_PARITY_EN_OFF                       ( 2)
  #define MC0_PARITY_CONTROL_WDATA_PARITY_EN_WID                       ( 2)
  #define MC0_PARITY_CONTROL_WDATA_PARITY_EN_MSK                       (0x0000000C)
  #define MC0_PARITY_CONTROL_WDATA_PARITY_EN_MIN                       (0)
  #define MC0_PARITY_CONTROL_WDATA_PARITY_EN_MAX                       (3) // 0x00000003
  #define MC0_PARITY_CONTROL_WDATA_PARITY_EN_DEF                       (0x00000003)
  #define MC0_PARITY_CONTROL_WDATA_PARITY_EN_HSH                       (0x0204D9B4)

  #define MC0_PARITY_CONTROL_RDATA_PARITY_EN_OFF                       ( 4)
  #define MC0_PARITY_CONTROL_RDATA_PARITY_EN_WID                       ( 1)
  #define MC0_PARITY_CONTROL_RDATA_PARITY_EN_MSK                       (0x00000010)
  #define MC0_PARITY_CONTROL_RDATA_PARITY_EN_MIN                       (0)
  #define MC0_PARITY_CONTROL_RDATA_PARITY_EN_MAX                       (1) // 0x00000001
  #define MC0_PARITY_CONTROL_RDATA_PARITY_EN_DEF                       (0x00000001)
  #define MC0_PARITY_CONTROL_RDATA_PARITY_EN_HSH                       (0x0108D9B4)

  #define MC0_PARITY_CONTROL_RDCPL_HEADER_PARITY_EN_OFF                ( 5)
  #define MC0_PARITY_CONTROL_RDCPL_HEADER_PARITY_EN_WID                ( 1)
  #define MC0_PARITY_CONTROL_RDCPL_HEADER_PARITY_EN_MSK                (0x00000020)
  #define MC0_PARITY_CONTROL_RDCPL_HEADER_PARITY_EN_MIN                (0)
  #define MC0_PARITY_CONTROL_RDCPL_HEADER_PARITY_EN_MAX                (1) // 0x00000001
  #define MC0_PARITY_CONTROL_RDCPL_HEADER_PARITY_EN_DEF                (0x00000001)
  #define MC0_PARITY_CONTROL_RDCPL_HEADER_PARITY_EN_HSH                (0x010AD9B4)

  #define MC0_PARITY_CONTROL_RSVD_0_OFF                                ( 6)
  #define MC0_PARITY_CONTROL_RSVD_0_WID                                ( 2)
  #define MC0_PARITY_CONTROL_RSVD_0_MSK                                (0x000000C0)
  #define MC0_PARITY_CONTROL_RSVD_0_MIN                                (0)
  #define MC0_PARITY_CONTROL_RSVD_0_MAX                                (3) // 0x00000003
  #define MC0_PARITY_CONTROL_RSVD_0_DEF                                (0x00000000)
  #define MC0_PARITY_CONTROL_RSVD_0_HSH                                (0x020CD9B4)

  #define MC0_PARITY_CONTROL_DIS_PARITY_PCIE_ERR_OFF                   ( 8)
  #define MC0_PARITY_CONTROL_DIS_PARITY_PCIE_ERR_WID                   ( 1)
  #define MC0_PARITY_CONTROL_DIS_PARITY_PCIE_ERR_MSK                   (0x00000100)
  #define MC0_PARITY_CONTROL_DIS_PARITY_PCIE_ERR_MIN                   (0)
  #define MC0_PARITY_CONTROL_DIS_PARITY_PCIE_ERR_MAX                   (1) // 0x00000001
  #define MC0_PARITY_CONTROL_DIS_PARITY_PCIE_ERR_DEF                   (0x00000000)
  #define MC0_PARITY_CONTROL_DIS_PARITY_PCIE_ERR_HSH                   (0x0110D9B4)

  #define MC0_PARITY_CONTROL_DIS_PARITY_LOG_OFF                        ( 9)
  #define MC0_PARITY_CONTROL_DIS_PARITY_LOG_WID                        ( 1)
  #define MC0_PARITY_CONTROL_DIS_PARITY_LOG_MSK                        (0x00000200)
  #define MC0_PARITY_CONTROL_DIS_PARITY_LOG_MIN                        (0)
  #define MC0_PARITY_CONTROL_DIS_PARITY_LOG_MAX                        (1) // 0x00000001
  #define MC0_PARITY_CONTROL_DIS_PARITY_LOG_DEF                        (0x00000000)
  #define MC0_PARITY_CONTROL_DIS_PARITY_LOG_HSH                        (0x0112D9B4)

  #define MC0_PARITY_CONTROL_RSVD_1_OFF                                (10)
  #define MC0_PARITY_CONTROL_RSVD_1_WID                                (21)
  #define MC0_PARITY_CONTROL_RSVD_1_MSK                                (0x7FFFFC00)
  #define MC0_PARITY_CONTROL_RSVD_1_MIN                                (0)
  #define MC0_PARITY_CONTROL_RSVD_1_MAX                                (2097151) // 0x001FFFFF
  #define MC0_PARITY_CONTROL_RSVD_1_DEF                                (0x00000000)
  #define MC0_PARITY_CONTROL_RSVD_1_HSH                                (0x1514D9B4)

  #define MC0_PARITY_CONTROL_PARITY_EN_OFF                             (31)
  #define MC0_PARITY_CONTROL_PARITY_EN_WID                             ( 1)
  #define MC0_PARITY_CONTROL_PARITY_EN_MSK                             (0x80000000)
  #define MC0_PARITY_CONTROL_PARITY_EN_MIN                             (0)
  #define MC0_PARITY_CONTROL_PARITY_EN_MAX                             (1) // 0x00000001
  #define MC0_PARITY_CONTROL_PARITY_EN_DEF                             (0x00000000)
  #define MC0_PARITY_CONTROL_PARITY_EN_HSH                             (0x013ED9B4)

#define MC0_MAD_MC_HASH_REG                                            (0x0000D9B8)
#define MC0_MAD_MC_HASH_DEF                                            (0x00000005)
#define MC0_MAD_MC_HASH_VOLATILE_BITFIELDS_MSK                         (0x00000000)

  #define MC0_MAD_MC_HASH_Hash_enabled_OFF                             ( 0)
  #define MC0_MAD_MC_HASH_Hash_enabled_WID                             ( 1)
  #define MC0_MAD_MC_HASH_Hash_enabled_MSK                             (0x00000001)
  #define MC0_MAD_MC_HASH_Hash_enabled_MIN                             (0)
  #define MC0_MAD_MC_HASH_Hash_enabled_MAX                             (1) // 0x00000001
  #define MC0_MAD_MC_HASH_Hash_enabled_DEF                             (0x00000001)
  #define MC0_MAD_MC_HASH_Hash_enabled_HSH                             (0x0100D9B8)

  #define MC0_MAD_MC_HASH_Hash_LSB_OFF                                 ( 1)
  #define MC0_MAD_MC_HASH_Hash_LSB_WID                                 ( 3)
  #define MC0_MAD_MC_HASH_Hash_LSB_MSK                                 (0x0000000E)
  #define MC0_MAD_MC_HASH_Hash_LSB_MIN                                 (0)
  #define MC0_MAD_MC_HASH_Hash_LSB_MAX                                 (7) // 0x00000007
  #define MC0_MAD_MC_HASH_Hash_LSB_DEF                                 (0x00000002)
  #define MC0_MAD_MC_HASH_Hash_LSB_HSH                                 (0x0302D9B8)

  #define MC0_MAD_MC_HASH_Zone1_start_OFF                              (20)
  #define MC0_MAD_MC_HASH_Zone1_start_WID                              (10)
  #define MC0_MAD_MC_HASH_Zone1_start_MSK                              (0x3FF00000)
  #define MC0_MAD_MC_HASH_Zone1_start_MIN                              (0)
  #define MC0_MAD_MC_HASH_Zone1_start_MAX                              (1023) // 0x000003FF
  #define MC0_MAD_MC_HASH_Zone1_start_DEF                              (0x00000000)
  #define MC0_MAD_MC_HASH_Zone1_start_HSH                              (0x0A28D9B8)

#define MC0_PMON_UNIT_CONTROL_REG                                      (0x0000D9C8)
#define MC0_PMON_UNIT_CONTROL_DEF                                      (0x0000000000000000)
#define MC0_PMON_UNIT_CONTROL_VOLATILE_BITFIELDS_MSK                   (0x0000000000000300)

  #define MC0_PMON_UNIT_CONTROL_frz_OFF                                ( 0)
  #define MC0_PMON_UNIT_CONTROL_frz_WID                                ( 1)
  #define MC0_PMON_UNIT_CONTROL_frz_MSK                                (0x00000001)
  #define MC0_PMON_UNIT_CONTROL_frz_MIN                                (0)
  #define MC0_PMON_UNIT_CONTROL_frz_MAX                                (1) // 0x00000001
  #define MC0_PMON_UNIT_CONTROL_frz_DEF                                (0x00000000)
  #define MC0_PMON_UNIT_CONTROL_frz_HSH                                (0x4100D9C8)

  #define MC0_PMON_UNIT_CONTROL_reset_ctrl_OFF                         ( 8)
  #define MC0_PMON_UNIT_CONTROL_reset_ctrl_WID                         ( 1)
  #define MC0_PMON_UNIT_CONTROL_reset_ctrl_MSK                         (0x00000100)
  #define MC0_PMON_UNIT_CONTROL_reset_ctrl_MIN                         (0)
  #define MC0_PMON_UNIT_CONTROL_reset_ctrl_MAX                         (1) // 0x00000001
  #define MC0_PMON_UNIT_CONTROL_reset_ctrl_DEF                         (0x00000000)
  #define MC0_PMON_UNIT_CONTROL_reset_ctrl_HSH                         (0x4190D9C8)

  #define MC0_PMON_UNIT_CONTROL_reset_ctrs_OFF                         ( 9)
  #define MC0_PMON_UNIT_CONTROL_reset_ctrs_WID                         ( 1)
  #define MC0_PMON_UNIT_CONTROL_reset_ctrs_MSK                         (0x00000200)
  #define MC0_PMON_UNIT_CONTROL_reset_ctrs_MIN                         (0)
  #define MC0_PMON_UNIT_CONTROL_reset_ctrs_MAX                         (1) // 0x00000001
  #define MC0_PMON_UNIT_CONTROL_reset_ctrs_DEF                         (0x00000000)
  #define MC0_PMON_UNIT_CONTROL_reset_ctrs_HSH                         (0x4192D9C8)

#define MC0_PMON_UNIT_STATUS_REG                                       (0x0000D9D0)
#define MC0_PMON_UNIT_STATUS_DEF                                       (0x0000000000000000)
#define MC0_PMON_UNIT_STATUS_VOLATILE_BITFIELDS_MSK                    (0x000000000000001F)

  #define MC0_PMON_UNIT_STATUS_ctr_ov_OFF                              ( 0)
  #define MC0_PMON_UNIT_STATUS_ctr_ov_WID                              ( 5)
  #define MC0_PMON_UNIT_STATUS_ctr_ov_MSK                              (0x0000001F)
  #define MC0_PMON_UNIT_STATUS_ctr_ov_MIN                              (0)
  #define MC0_PMON_UNIT_STATUS_ctr_ov_MAX                              (31) // 0x0000001F
  #define MC0_PMON_UNIT_STATUS_ctr_ov_DEF                              (0x00000000)
  #define MC0_PMON_UNIT_STATUS_ctr_ov_HSH                              (0x4580D9D0)

#define MC0_PMON_COUNTER_CONTROL_0_REG                                 (0x0000D9D8)
#define MC0_PMON_COUNTER_CONTROL_0_DEF                                 (0x0000000000000000)
#define MC0_PMON_COUNTER_CONTROL_0_VOLATILE_BITFIELDS_MSK              (0x00000000FF960FFF)

  #define MC0_PMON_COUNTER_CONTROL_0_ev_sel_OFF                        ( 0)
  #define MC0_PMON_COUNTER_CONTROL_0_ev_sel_WID                        ( 8)
  #define MC0_PMON_COUNTER_CONTROL_0_ev_sel_MSK                        (0x000000FF)
  #define MC0_PMON_COUNTER_CONTROL_0_ev_sel_MIN                        (0)
  #define MC0_PMON_COUNTER_CONTROL_0_ev_sel_MAX                        (255) // 0x000000FF
  #define MC0_PMON_COUNTER_CONTROL_0_ev_sel_DEF                        (0x00000000)
  #define MC0_PMON_COUNTER_CONTROL_0_ev_sel_HSH                        (0x4880D9D8)

  #define MC0_PMON_COUNTER_CONTROL_0_ch_mask_OFF                       ( 8)
  #define MC0_PMON_COUNTER_CONTROL_0_ch_mask_WID                       ( 4)
  #define MC0_PMON_COUNTER_CONTROL_0_ch_mask_MSK                       (0x00000F00)
  #define MC0_PMON_COUNTER_CONTROL_0_ch_mask_MIN                       (0)
  #define MC0_PMON_COUNTER_CONTROL_0_ch_mask_MAX                       (15) // 0x0000000F
  #define MC0_PMON_COUNTER_CONTROL_0_ch_mask_DEF                       (0x00000000)
  #define MC0_PMON_COUNTER_CONTROL_0_ch_mask_HSH                       (0x4490D9D8)

  #define MC0_PMON_COUNTER_CONTROL_0_rst_OFF                           (17)
  #define MC0_PMON_COUNTER_CONTROL_0_rst_WID                           ( 1)
  #define MC0_PMON_COUNTER_CONTROL_0_rst_MSK                           (0x00020000)
  #define MC0_PMON_COUNTER_CONTROL_0_rst_MIN                           (0)
  #define MC0_PMON_COUNTER_CONTROL_0_rst_MAX                           (1) // 0x00000001
  #define MC0_PMON_COUNTER_CONTROL_0_rst_DEF                           (0x00000000)
  #define MC0_PMON_COUNTER_CONTROL_0_rst_HSH                           (0x41A2D9D8)

  #define MC0_PMON_COUNTER_CONTROL_0_edge_det_OFF                      (18)
  #define MC0_PMON_COUNTER_CONTROL_0_edge_det_WID                      ( 1)
  #define MC0_PMON_COUNTER_CONTROL_0_edge_det_MSK                      (0x00040000)
  #define MC0_PMON_COUNTER_CONTROL_0_edge_det_MIN                      (0)
  #define MC0_PMON_COUNTER_CONTROL_0_edge_det_MAX                      (1) // 0x00000001
  #define MC0_PMON_COUNTER_CONTROL_0_edge_det_DEF                      (0x00000000)
  #define MC0_PMON_COUNTER_CONTROL_0_edge_det_HSH                      (0x41A4D9D8)

  #define MC0_PMON_COUNTER_CONTROL_0_frz_on_ov_OFF                     (20)
  #define MC0_PMON_COUNTER_CONTROL_0_frz_on_ov_WID                     ( 1)
  #define MC0_PMON_COUNTER_CONTROL_0_frz_on_ov_MSK                     (0x00100000)
  #define MC0_PMON_COUNTER_CONTROL_0_frz_on_ov_MIN                     (0)
  #define MC0_PMON_COUNTER_CONTROL_0_frz_on_ov_MAX                     (1) // 0x00000001
  #define MC0_PMON_COUNTER_CONTROL_0_frz_on_ov_DEF                     (0x00000000)
  #define MC0_PMON_COUNTER_CONTROL_0_frz_on_ov_HSH                     (0x41A8D9D8)

  #define MC0_PMON_COUNTER_CONTROL_0_invert_OFF                        (23)
  #define MC0_PMON_COUNTER_CONTROL_0_invert_WID                        ( 1)
  #define MC0_PMON_COUNTER_CONTROL_0_invert_MSK                        (0x00800000)
  #define MC0_PMON_COUNTER_CONTROL_0_invert_MIN                        (0)
  #define MC0_PMON_COUNTER_CONTROL_0_invert_MAX                        (1) // 0x00000001
  #define MC0_PMON_COUNTER_CONTROL_0_invert_DEF                        (0x00000000)
  #define MC0_PMON_COUNTER_CONTROL_0_invert_HSH                        (0x41AED9D8)

  #define MC0_PMON_COUNTER_CONTROL_0_thresh_OFF                        (24)
  #define MC0_PMON_COUNTER_CONTROL_0_thresh_WID                        ( 8)
  #define MC0_PMON_COUNTER_CONTROL_0_thresh_MSK                        (0xFF000000)
  #define MC0_PMON_COUNTER_CONTROL_0_thresh_MIN                        (0)
  #define MC0_PMON_COUNTER_CONTROL_0_thresh_MAX                        (255) // 0x000000FF
  #define MC0_PMON_COUNTER_CONTROL_0_thresh_DEF                        (0x00000000)
  #define MC0_PMON_COUNTER_CONTROL_0_thresh_HSH                        (0x48B0D9D8)

#define MC0_PMON_COUNTER_CONTROL_1_REG                                 (0x0000D9E0)
//Duplicate of MC0_PMON_COUNTER_CONTROL_0_REG

#define MC0_PMON_COUNTER_CONTROL_2_REG                                 (0x0000D9E8)
//Duplicate of MC0_PMON_COUNTER_CONTROL_0_REG

#define MC0_PMON_COUNTER_CONTROL_3_REG                                 (0x0000D9F0)
//Duplicate of MC0_PMON_COUNTER_CONTROL_0_REG

#define MC0_PMON_COUNTER_CONTROL_4_REG                                 (0x0000D9F8)
//Duplicate of MC0_PMON_COUNTER_CONTROL_0_REG

#define MC0_PMON_COUNTER_DATA_0_REG                                    (0x0000DA00)
#define MC0_PMON_COUNTER_DATA_0_DEF                                    (0x0000000000000000)
#define MC0_PMON_COUNTER_DATA_0_VOLATILE_BITFIELDS_MSK                 (0xFFFFFFFFFFFFFFFF)

  #define MC0_PMON_COUNTER_DATA_0_event_count_OFF                      ( 0)
  #define MC0_PMON_COUNTER_DATA_0_event_count_WID                      (64)
  #define MC0_PMON_COUNTER_DATA_0_event_count_MSK                      (0xFFFFFFFFFFFFFFFFULL)
  #define MC0_PMON_COUNTER_DATA_0_event_count_MIN                      (0)
  #define MC0_PMON_COUNTER_DATA_0_event_count_MAX                      (18446744073709551615ULL) // 0xFFFFFFFFFFFFFFFF
  #define MC0_PMON_COUNTER_DATA_0_event_count_DEF                      (0x00000000)
  #define MC0_PMON_COUNTER_DATA_0_event_count_HSH                      (0x4080DA00)

#define MC0_PMON_COUNTER_DATA_1_REG                                    (0x0000DA08)
//Duplicate of MC0_PMON_COUNTER_DATA_0_REG

#define MC0_PMON_COUNTER_DATA_2_REG                                    (0x0000DA10)
//Duplicate of MC0_PMON_COUNTER_DATA_0_REG

#define MC0_PMON_COUNTER_DATA_3_REG                                    (0x0000DA18)
//Duplicate of MC0_PMON_COUNTER_DATA_0_REG

#define MC0_PMON_COUNTER_DATA_4_REG                                    (0x0000DA20)
//Duplicate of MC0_PMON_COUNTER_DATA_0_REG

#define MC0_OS_TELEMETRY_CONTROL_REG                                   (0x0000DA30)
#define MC0_OS_TELEMETRY_CONTROL_DEF                                   (0x00000000)
#define MC0_OS_TELEMETRY_CONTROL_VOLATILE_BITFIELDS_MSK                (0x00000000)

  #define MC0_OS_TELEMETRY_CONTROL_EnableOSTelemetry_OFF               ( 0)
  #define MC0_OS_TELEMETRY_CONTROL_EnableOSTelemetry_WID               ( 1)
  #define MC0_OS_TELEMETRY_CONTROL_EnableOSTelemetry_MSK               (0x00000001)
  #define MC0_OS_TELEMETRY_CONTROL_EnableOSTelemetry_MIN               (0)
  #define MC0_OS_TELEMETRY_CONTROL_EnableOSTelemetry_MAX               (1) // 0x00000001
  #define MC0_OS_TELEMETRY_CONTROL_EnableOSTelemetry_DEF               (0x00000000)
  #define MC0_OS_TELEMETRY_CONTROL_EnableOSTelemetry_HSH               (0x0100DA30)

  #define MC0_OS_TELEMETRY_CONTROL_enable_idle_ostelemetry_OFF         ( 1)
  #define MC0_OS_TELEMETRY_CONTROL_enable_idle_ostelemetry_WID         ( 1)
  #define MC0_OS_TELEMETRY_CONTROL_enable_idle_ostelemetry_MSK         (0x00000002)
  #define MC0_OS_TELEMETRY_CONTROL_enable_idle_ostelemetry_MIN         (0)
  #define MC0_OS_TELEMETRY_CONTROL_enable_idle_ostelemetry_MAX         (1) // 0x00000001
  #define MC0_OS_TELEMETRY_CONTROL_enable_idle_ostelemetry_DEF         (0x00000000)
  #define MC0_OS_TELEMETRY_CONTROL_enable_idle_ostelemetry_HSH         (0x0102DA30)

#define MC0_PM_OPP_SR_POLICY_0_REG                                     (0x0000DA34)
#define MC0_PM_OPP_SR_POLICY_0_DEF                                     (0x01208028)
#define MC0_PM_OPP_SR_POLICY_0_VOLATILE_BITFIELDS_MSK                  (0x00000000)

  #define MC0_PM_OPP_SR_POLICY_0_read_delay_OFF                        ( 0)
  #define MC0_PM_OPP_SR_POLICY_0_read_delay_WID                        (12)
  #define MC0_PM_OPP_SR_POLICY_0_read_delay_MSK                        (0x00000FFF)
  #define MC0_PM_OPP_SR_POLICY_0_read_delay_MIN                        (0)
  #define MC0_PM_OPP_SR_POLICY_0_read_delay_MAX                        (4095) // 0x00000FFF
  #define MC0_PM_OPP_SR_POLICY_0_read_delay_DEF                        (0x00000028)
  #define MC0_PM_OPP_SR_POLICY_0_read_delay_HSH                        (0x0C00DA34)

  #define MC0_PM_OPP_SR_POLICY_0_read_req_OFF                          (12)
  #define MC0_PM_OPP_SR_POLICY_0_read_req_WID                          ( 6)
  #define MC0_PM_OPP_SR_POLICY_0_read_req_MSK                          (0x0003F000)
  #define MC0_PM_OPP_SR_POLICY_0_read_req_MIN                          (0)
  #define MC0_PM_OPP_SR_POLICY_0_read_req_MAX                          (63) // 0x0000003F
  #define MC0_PM_OPP_SR_POLICY_0_read_req_DEF                          (0x00000008)
  #define MC0_PM_OPP_SR_POLICY_0_read_req_HSH                          (0x0618DA34)

  #define MC0_PM_OPP_SR_POLICY_0_write_req_OFF                         (18)
  #define MC0_PM_OPP_SR_POLICY_0_write_req_WID                         ( 6)
  #define MC0_PM_OPP_SR_POLICY_0_write_req_MSK                         (0x00FC0000)
  #define MC0_PM_OPP_SR_POLICY_0_write_req_MIN                         (0)
  #define MC0_PM_OPP_SR_POLICY_0_write_req_MAX                         (63) // 0x0000003F
  #define MC0_PM_OPP_SR_POLICY_0_write_req_DEF                         (0x00000008)
  #define MC0_PM_OPP_SR_POLICY_0_write_req_HSH                         (0x0624DA34)

  #define MC0_PM_OPP_SR_POLICY_0_LPMode4_EN_OFF                        (24)
  #define MC0_PM_OPP_SR_POLICY_0_LPMode4_EN_WID                        ( 2)
  #define MC0_PM_OPP_SR_POLICY_0_LPMode4_EN_MSK                        (0x03000000)
  #define MC0_PM_OPP_SR_POLICY_0_LPMode4_EN_MIN                        (0)
  #define MC0_PM_OPP_SR_POLICY_0_LPMode4_EN_MAX                        (3) // 0x00000003
  #define MC0_PM_OPP_SR_POLICY_0_LPMode4_EN_DEF                        (0x00000001)
  #define MC0_PM_OPP_SR_POLICY_0_LPMode4_EN_HSH                        (0x0230DA34)

  #define MC0_PM_OPP_SR_POLICY_0_delay_lp_vc1_OFF                      (26)
  #define MC0_PM_OPP_SR_POLICY_0_delay_lp_vc1_WID                      ( 1)
  #define MC0_PM_OPP_SR_POLICY_0_delay_lp_vc1_MSK                      (0x04000000)
  #define MC0_PM_OPP_SR_POLICY_0_delay_lp_vc1_MIN                      (0)
  #define MC0_PM_OPP_SR_POLICY_0_delay_lp_vc1_MAX                      (1) // 0x00000001
  #define MC0_PM_OPP_SR_POLICY_0_delay_lp_vc1_DEF                      (0x00000000)
  #define MC0_PM_OPP_SR_POLICY_0_delay_lp_vc1_HSH                      (0x0134DA34)

#define MC0_PM_OPP_SR_POLICY_1_REG                                     (0x0000DA38)
//Duplicate of MC0_PM_OPP_SR_POLICY_0_REG

#define MC0_MC_CFI_FABRIC_CONTROL_REG                                  (0x0000DA40)
#define MC0_MC_CFI_FABRIC_CONTROL_DEF                                  (0x78001200)
#define MC0_MC_CFI_FABRIC_CONTROL_VOLATILE_BITFIELDS_MSK               (0x00000000)

  #define MC0_MC_CFI_FABRIC_CONTROL_dis_cfi_clk_gate_OFF               ( 4)
  #define MC0_MC_CFI_FABRIC_CONTROL_dis_cfi_clk_gate_WID               ( 1)
  #define MC0_MC_CFI_FABRIC_CONTROL_dis_cfi_clk_gate_MSK               (0x00000010)
  #define MC0_MC_CFI_FABRIC_CONTROL_dis_cfi_clk_gate_MIN               (0)
  #define MC0_MC_CFI_FABRIC_CONTROL_dis_cfi_clk_gate_MAX               (1) // 0x00000001
  #define MC0_MC_CFI_FABRIC_CONTROL_dis_cfi_clk_gate_DEF               (0x00000000)
  #define MC0_MC_CFI_FABRIC_CONTROL_dis_cfi_clk_gate_HSH               (0x0108DA40)

  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_rx_lock_on_connect_OFF         ( 6)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_rx_lock_on_connect_WID         ( 1)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_rx_lock_on_connect_MSK         (0x00000040)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_rx_lock_on_connect_MIN         (0)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_rx_lock_on_connect_MAX         (1) // 0x00000001
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_rx_lock_on_connect_DEF         (0x00000000)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_rx_lock_on_connect_HSH         (0x010CDA40)

  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_tx_lock_on_connect_OFF         ( 9)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_tx_lock_on_connect_WID         ( 1)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_tx_lock_on_connect_MSK         (0x00000200)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_tx_lock_on_connect_MIN         (0)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_tx_lock_on_connect_MAX         (1) // 0x00000001
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_tx_lock_on_connect_DEF         (0x00000001)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_tx_lock_on_connect_HSH         (0x0112DA40)

  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_rx_sagv_block_en_OFF           (10)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_rx_sagv_block_en_WID           ( 1)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_rx_sagv_block_en_MSK           (0x00000400)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_rx_sagv_block_en_MIN           (0)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_rx_sagv_block_en_MAX           (1) // 0x00000001
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_rx_sagv_block_en_DEF           (0x00000000)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_rx_sagv_block_en_HSH           (0x0114DA40)

  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_tx_sagv_block_en_OFF           (11)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_tx_sagv_block_en_WID           ( 1)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_tx_sagv_block_en_MSK           (0x00000800)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_tx_sagv_block_en_MIN           (0)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_tx_sagv_block_en_MAX           (1) // 0x00000001
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_tx_sagv_block_en_DEF           (0x00000000)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_tx_sagv_block_en_HSH           (0x0116DA40)

  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_sagv_block_delay_OFF           (12)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_sagv_block_delay_WID           ( 4)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_sagv_block_delay_MSK           (0x0000F000)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_sagv_block_delay_MIN           (0)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_sagv_block_delay_MAX           (15) // 0x0000000F
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_sagv_block_delay_DEF           (0x00000001)
  #define MC0_MC_CFI_FABRIC_CONTROL_cfi_sagv_block_delay_HSH           (0x0418DA40)

  #define MC0_MC_CFI_FABRIC_CONTROL_mc_cfi_link_peer_clkack_bypass_OFF (27)
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_cfi_link_peer_clkack_bypass_WID ( 1)
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_cfi_link_peer_clkack_bypass_MSK (0x08000000)
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_cfi_link_peer_clkack_bypass_MIN (0)
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_cfi_link_peer_clkack_bypass_MAX (1) // 0x00000001
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_cfi_link_peer_clkack_bypass_DEF (0x00000001)
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_cfi_link_peer_clkack_bypass_HSH (0x0136DA40)

  #define MC0_MC_CFI_FABRIC_CONTROL_mc_rsp_peer_clkack_bypass_OFF      (28)
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_rsp_peer_clkack_bypass_WID      ( 1)
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_rsp_peer_clkack_bypass_MSK      (0x10000000)
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_rsp_peer_clkack_bypass_MIN      (0)
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_rsp_peer_clkack_bypass_MAX      (1) // 0x00000001
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_rsp_peer_clkack_bypass_DEF      (0x00000001)
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_rsp_peer_clkack_bypass_HSH      (0x0138DA40)

  #define MC0_MC_CFI_FABRIC_CONTROL_mc_req_credits_peer_clkack_bypass_OFF (29)
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_req_credits_peer_clkack_bypass_WID ( 1)
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_req_credits_peer_clkack_bypass_MSK (0x20000000)
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_req_credits_peer_clkack_bypass_MIN (0)
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_req_credits_peer_clkack_bypass_MAX (1) // 0x00000001
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_req_credits_peer_clkack_bypass_DEF (0x00000001)
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_req_credits_peer_clkack_bypass_HSH (0x013ADA40)

  #define MC0_MC_CFI_FABRIC_CONTROL_mc_rwd_credits_peer_clkack_bypass_OFF (30)
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_rwd_credits_peer_clkack_bypass_WID ( 1)
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_rwd_credits_peer_clkack_bypass_MSK (0x40000000)
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_rwd_credits_peer_clkack_bypass_MIN (0)
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_rwd_credits_peer_clkack_bypass_MAX (1) // 0x00000001
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_rwd_credits_peer_clkack_bypass_DEF (0x00000001)
  #define MC0_MC_CFI_FABRIC_CONTROL_mc_rwd_credits_peer_clkack_bypass_HSH (0x013CDA40)

#define MC0_MC_CFI_CONTROL_REG                                         (0x0000DA44)
#define MC0_MC_CFI_CONTROL_DEF                                         (0x00000000)
#define MC0_MC_CFI_CONTROL_VOLATILE_BITFIELDS_MSK                      (0x00000000)

  #define MC0_MC_CFI_CONTROL_Cclk_is_Dclk_OFF                          ( 0)
  #define MC0_MC_CFI_CONTROL_Cclk_is_Dclk_WID                          ( 1)
  #define MC0_MC_CFI_CONTROL_Cclk_is_Dclk_MSK                          (0x00000001)
  #define MC0_MC_CFI_CONTROL_Cclk_is_Dclk_MIN                          (0)
  #define MC0_MC_CFI_CONTROL_Cclk_is_Dclk_MAX                          (1) // 0x00000001
  #define MC0_MC_CFI_CONTROL_Cclk_is_Dclk_DEF                          (0x00000000)
  #define MC0_MC_CFI_CONTROL_Cclk_is_Dclk_HSH                          (0x0100DA44)

  #define MC0_MC_CFI_CONTROL_dis_VC1_cpl_interleave_OFF                ( 1)
  #define MC0_MC_CFI_CONTROL_dis_VC1_cpl_interleave_WID                ( 1)
  #define MC0_MC_CFI_CONTROL_dis_VC1_cpl_interleave_MSK                (0x00000002)
  #define MC0_MC_CFI_CONTROL_dis_VC1_cpl_interleave_MIN                (0)
  #define MC0_MC_CFI_CONTROL_dis_VC1_cpl_interleave_MAX                (1) // 0x00000001
  #define MC0_MC_CFI_CONTROL_dis_VC1_cpl_interleave_DEF                (0x00000000)
  #define MC0_MC_CFI_CONTROL_dis_VC1_cpl_interleave_HSH                (0x0102DA44)

  #define MC0_MC_CFI_CONTROL_dis_nonVC1_cpl_interleave_OFF             ( 2)
  #define MC0_MC_CFI_CONTROL_dis_nonVC1_cpl_interleave_WID             ( 1)
  #define MC0_MC_CFI_CONTROL_dis_nonVC1_cpl_interleave_MSK             (0x00000004)
  #define MC0_MC_CFI_CONTROL_dis_nonVC1_cpl_interleave_MIN             (0)
  #define MC0_MC_CFI_CONTROL_dis_nonVC1_cpl_interleave_MAX             (1) // 0x00000001
  #define MC0_MC_CFI_CONTROL_dis_nonVC1_cpl_interleave_DEF             (0x00000000)
  #define MC0_MC_CFI_CONTROL_dis_nonVC1_cpl_interleave_HSH             (0x0104DA44)

  #define MC0_MC_CFI_CONTROL_dis_all_cpl_interleave_OFF                ( 3)
  #define MC0_MC_CFI_CONTROL_dis_all_cpl_interleave_WID                ( 1)
  #define MC0_MC_CFI_CONTROL_dis_all_cpl_interleave_MSK                (0x00000008)
  #define MC0_MC_CFI_CONTROL_dis_all_cpl_interleave_MIN                (0)
  #define MC0_MC_CFI_CONTROL_dis_all_cpl_interleave_MAX                (1) // 0x00000001
  #define MC0_MC_CFI_CONTROL_dis_all_cpl_interleave_DEF                (0x00000000)
  #define MC0_MC_CFI_CONTROL_dis_all_cpl_interleave_HSH                (0x0106DA44)

#define MC0_PM_REQ_FORCE_DFX_REG                                       (0x0000DA48)
#define MC0_PM_REQ_FORCE_DFX_DEF                                       (0x00000000)
#define MC0_PM_REQ_FORCE_DFX_VOLATILE_BITFIELDS_MSK                    (0x000000FF)

  #define MC0_PM_REQ_FORCE_DFX_RSP_OFF                                 ( 0)
  #define MC0_PM_REQ_FORCE_DFX_RSP_WID                                 ( 1)
  #define MC0_PM_REQ_FORCE_DFX_RSP_MSK                                 (0x00000001)
  #define MC0_PM_REQ_FORCE_DFX_RSP_MIN                                 (0)
  #define MC0_PM_REQ_FORCE_DFX_RSP_MAX                                 (1) // 0x00000001
  #define MC0_PM_REQ_FORCE_DFX_RSP_DEF                                 (0x00000000)
  #define MC0_PM_REQ_FORCE_DFX_RSP_HSH                                 (0x0180DA48)

  #define MC0_PM_REQ_FORCE_DFX_Force_drain_R0_message_OFF              ( 1)
  #define MC0_PM_REQ_FORCE_DFX_Force_drain_R0_message_WID              ( 1)
  #define MC0_PM_REQ_FORCE_DFX_Force_drain_R0_message_MSK              (0x00000002)
  #define MC0_PM_REQ_FORCE_DFX_Force_drain_R0_message_MIN              (0)
  #define MC0_PM_REQ_FORCE_DFX_Force_drain_R0_message_MAX              (1) // 0x00000001
  #define MC0_PM_REQ_FORCE_DFX_Force_drain_R0_message_DEF              (0x00000000)
  #define MC0_PM_REQ_FORCE_DFX_Force_drain_R0_message_HSH              (0x0182DA48)

  #define MC0_PM_REQ_FORCE_DFX_Force_drain_R1_message_OFF              ( 2)
  #define MC0_PM_REQ_FORCE_DFX_Force_drain_R1_message_WID              ( 1)
  #define MC0_PM_REQ_FORCE_DFX_Force_drain_R1_message_MSK              (0x00000004)
  #define MC0_PM_REQ_FORCE_DFX_Force_drain_R1_message_MIN              (0)
  #define MC0_PM_REQ_FORCE_DFX_Force_drain_R1_message_MAX              (1) // 0x00000001
  #define MC0_PM_REQ_FORCE_DFX_Force_drain_R1_message_DEF              (0x00000000)
  #define MC0_PM_REQ_FORCE_DFX_Force_drain_R1_message_HSH              (0x0184DA48)

  #define MC0_PM_REQ_FORCE_DFX_Force_drain_R0_SAGV_message_OFF         ( 3)
  #define MC0_PM_REQ_FORCE_DFX_Force_drain_R0_SAGV_message_WID         ( 1)
  #define MC0_PM_REQ_FORCE_DFX_Force_drain_R0_SAGV_message_MSK         (0x00000008)
  #define MC0_PM_REQ_FORCE_DFX_Force_drain_R0_SAGV_message_MIN         (0)
  #define MC0_PM_REQ_FORCE_DFX_Force_drain_R0_SAGV_message_MAX         (1) // 0x00000001
  #define MC0_PM_REQ_FORCE_DFX_Force_drain_R0_SAGV_message_DEF         (0x00000000)
  #define MC0_PM_REQ_FORCE_DFX_Force_drain_R0_SAGV_message_HSH         (0x0186DA48)

  #define MC0_PM_REQ_FORCE_DFX_Force_unblock_message_OFF               ( 4)
  #define MC0_PM_REQ_FORCE_DFX_Force_unblock_message_WID               ( 1)
  #define MC0_PM_REQ_FORCE_DFX_Force_unblock_message_MSK               (0x00000010)
  #define MC0_PM_REQ_FORCE_DFX_Force_unblock_message_MIN               (0)
  #define MC0_PM_REQ_FORCE_DFX_Force_unblock_message_MAX               (1) // 0x00000001
  #define MC0_PM_REQ_FORCE_DFX_Force_unblock_message_DEF               (0x00000000)
  #define MC0_PM_REQ_FORCE_DFX_Force_unblock_message_HSH               (0x0188DA48)

  #define MC0_PM_REQ_FORCE_DFX_Force_vrcg_enable_message_OFF           ( 5)
  #define MC0_PM_REQ_FORCE_DFX_Force_vrcg_enable_message_WID           ( 1)
  #define MC0_PM_REQ_FORCE_DFX_Force_vrcg_enable_message_MSK           (0x00000020)
  #define MC0_PM_REQ_FORCE_DFX_Force_vrcg_enable_message_MIN           (0)
  #define MC0_PM_REQ_FORCE_DFX_Force_vrcg_enable_message_MAX           (1) // 0x00000001
  #define MC0_PM_REQ_FORCE_DFX_Force_vrcg_enable_message_DEF           (0x00000000)
  #define MC0_PM_REQ_FORCE_DFX_Force_vrcg_enable_message_HSH           (0x018ADA48)

  #define MC0_PM_REQ_FORCE_DFX_Force_vrcg_disable_message_OFF          ( 6)
  #define MC0_PM_REQ_FORCE_DFX_Force_vrcg_disable_message_WID          ( 1)
  #define MC0_PM_REQ_FORCE_DFX_Force_vrcg_disable_message_MSK          (0x00000040)
  #define MC0_PM_REQ_FORCE_DFX_Force_vrcg_disable_message_MIN          (0)
  #define MC0_PM_REQ_FORCE_DFX_Force_vrcg_disable_message_MAX          (1) // 0x00000001
  #define MC0_PM_REQ_FORCE_DFX_Force_vrcg_disable_message_DEF          (0x00000000)
  #define MC0_PM_REQ_FORCE_DFX_Force_vrcg_disable_message_HSH          (0x018CDA48)

  #define MC0_PM_REQ_FORCE_DFX_Force_OppSR_cmd_message_OFF             ( 7)
  #define MC0_PM_REQ_FORCE_DFX_Force_OppSR_cmd_message_WID             ( 1)
  #define MC0_PM_REQ_FORCE_DFX_Force_OppSR_cmd_message_MSK             (0x00000080)
  #define MC0_PM_REQ_FORCE_DFX_Force_OppSR_cmd_message_MIN             (0)
  #define MC0_PM_REQ_FORCE_DFX_Force_OppSR_cmd_message_MAX             (1) // 0x00000001
  #define MC0_PM_REQ_FORCE_DFX_Force_OppSR_cmd_message_DEF             (0x00000000)
  #define MC0_PM_REQ_FORCE_DFX_Force_OppSR_cmd_message_HSH             (0x018EDA48)

  #define MC0_PM_REQ_FORCE_DFX_OppSR_cmd_select_OFF                    ( 8)
  #define MC0_PM_REQ_FORCE_DFX_OppSR_cmd_select_WID                    ( 2)
  #define MC0_PM_REQ_FORCE_DFX_OppSR_cmd_select_MSK                    (0x00000300)
  #define MC0_PM_REQ_FORCE_DFX_OppSR_cmd_select_MIN                    (0)
  #define MC0_PM_REQ_FORCE_DFX_OppSR_cmd_select_MAX                    (3) // 0x00000003
  #define MC0_PM_REQ_FORCE_DFX_OppSR_cmd_select_DEF                    (0x00000000)
  #define MC0_PM_REQ_FORCE_DFX_OppSR_cmd_select_HSH                    (0x0210DA48)

#define MC0_DDRPL_CFG2_DTF_REG                                         (0x0000DA4C)
#define MC0_DDRPL_CFG2_DTF_DEF                                         (0x00000000)
#define MC0_DDRPL_CFG2_DTF_VOLATILE_BITFIELDS_MSK                      (0x00000000)

  #define MC0_DDRPL_CFG2_DTF_DVP_block_req_delay_OFF                   ( 0)
  #define MC0_DDRPL_CFG2_DTF_DVP_block_req_delay_WID                   ( 9)
  #define MC0_DDRPL_CFG2_DTF_DVP_block_req_delay_MSK                   (0x000001FF)
  #define MC0_DDRPL_CFG2_DTF_DVP_block_req_delay_MIN                   (0)
  #define MC0_DDRPL_CFG2_DTF_DVP_block_req_delay_MAX                   (511) // 0x000001FF
  #define MC0_DDRPL_CFG2_DTF_DVP_block_req_delay_DEF                   (0x00000000)
  #define MC0_DDRPL_CFG2_DTF_DVP_block_req_delay_HSH                   (0x0900DA4C)

  #define MC0_DDRPL_CFG2_DTF_Override_CFI_RX_Req_trace_packet_OFF      ( 9)
  #define MC0_DDRPL_CFG2_DTF_Override_CFI_RX_Req_trace_packet_WID      ( 1)
  #define MC0_DDRPL_CFG2_DTF_Override_CFI_RX_Req_trace_packet_MSK      (0x00000200)
  #define MC0_DDRPL_CFG2_DTF_Override_CFI_RX_Req_trace_packet_MIN      (0)
  #define MC0_DDRPL_CFG2_DTF_Override_CFI_RX_Req_trace_packet_MAX      (1) // 0x00000001
  #define MC0_DDRPL_CFG2_DTF_Override_CFI_RX_Req_trace_packet_DEF      (0x00000000)
  #define MC0_DDRPL_CFG2_DTF_Override_CFI_RX_Req_trace_packet_HSH      (0x0112DA4C)

  #define MC0_DDRPL_CFG2_DTF_Override_CFI_RX_Data_trace_packet_OFF     (10)
  #define MC0_DDRPL_CFG2_DTF_Override_CFI_RX_Data_trace_packet_WID     ( 1)
  #define MC0_DDRPL_CFG2_DTF_Override_CFI_RX_Data_trace_packet_MSK     (0x00000400)
  #define MC0_DDRPL_CFG2_DTF_Override_CFI_RX_Data_trace_packet_MIN     (0)
  #define MC0_DDRPL_CFG2_DTF_Override_CFI_RX_Data_trace_packet_MAX     (1) // 0x00000001
  #define MC0_DDRPL_CFG2_DTF_Override_CFI_RX_Data_trace_packet_DEF     (0x00000000)
  #define MC0_DDRPL_CFG2_DTF_Override_CFI_RX_Data_trace_packet_HSH     (0x0114DA4C)

#define MC0_SC_QOS_UARCH_REG                                           (0x0000DA50)
#define MC0_SC_QOS_UARCH_DEF                                           (0x00000407)
#define MC0_SC_QOS_UARCH_VOLATILE_BITFIELDS_MSK                        (0x00000000)

  #define MC0_SC_QOS_UARCH_VC1_Read_starvation_en_OFF                  ( 0)
  #define MC0_SC_QOS_UARCH_VC1_Read_starvation_en_WID                  ( 1)
  #define MC0_SC_QOS_UARCH_VC1_Read_starvation_en_MSK                  (0x00000001)
  #define MC0_SC_QOS_UARCH_VC1_Read_starvation_en_MIN                  (0)
  #define MC0_SC_QOS_UARCH_VC1_Read_starvation_en_MAX                  (1) // 0x00000001
  #define MC0_SC_QOS_UARCH_VC1_Read_starvation_en_DEF                  (0x00000001)
  #define MC0_SC_QOS_UARCH_VC1_Read_starvation_en_HSH                  (0x0100DA50)

  #define MC0_SC_QOS_UARCH_Write_starvation_in_Isoc_en_OFF             ( 1)
  #define MC0_SC_QOS_UARCH_Write_starvation_in_Isoc_en_WID             ( 1)
  #define MC0_SC_QOS_UARCH_Write_starvation_in_Isoc_en_MSK             (0x00000002)
  #define MC0_SC_QOS_UARCH_Write_starvation_in_Isoc_en_MIN             (0)
  #define MC0_SC_QOS_UARCH_Write_starvation_in_Isoc_en_MAX             (1) // 0x00000001
  #define MC0_SC_QOS_UARCH_Write_starvation_in_Isoc_en_DEF             (0x00000001)
  #define MC0_SC_QOS_UARCH_Write_starvation_in_Isoc_en_HSH             (0x0102DA50)

  #define MC0_SC_QOS_UARCH_Read_starvation_in_Isoch_en_OFF             ( 2)
  #define MC0_SC_QOS_UARCH_Read_starvation_in_Isoch_en_WID             ( 1)
  #define MC0_SC_QOS_UARCH_Read_starvation_in_Isoch_en_MSK             (0x00000004)
  #define MC0_SC_QOS_UARCH_Read_starvation_in_Isoch_en_MIN             (0)
  #define MC0_SC_QOS_UARCH_Read_starvation_in_Isoch_en_MAX             (1) // 0x00000001
  #define MC0_SC_QOS_UARCH_Read_starvation_in_Isoch_en_DEF             (0x00000001)
  #define MC0_SC_QOS_UARCH_Read_starvation_in_Isoch_en_HSH             (0x0104DA50)

  #define MC0_SC_QOS_UARCH_VC0_counter_disable_OFF                     ( 3)
  #define MC0_SC_QOS_UARCH_VC0_counter_disable_WID                     ( 1)
  #define MC0_SC_QOS_UARCH_VC0_counter_disable_MSK                     (0x00000008)
  #define MC0_SC_QOS_UARCH_VC0_counter_disable_MIN                     (0)
  #define MC0_SC_QOS_UARCH_VC0_counter_disable_MAX                     (1) // 0x00000001
  #define MC0_SC_QOS_UARCH_VC0_counter_disable_DEF                     (0x00000000)
  #define MC0_SC_QOS_UARCH_VC0_counter_disable_HSH                     (0x0106DA50)

  #define MC0_SC_QOS_UARCH_Force_MCVC1Demote_OFF                       ( 4)
  #define MC0_SC_QOS_UARCH_Force_MCVC1Demote_WID                       ( 1)
  #define MC0_SC_QOS_UARCH_Force_MCVC1Demote_MSK                       (0x00000010)
  #define MC0_SC_QOS_UARCH_Force_MCVC1Demote_MIN                       (0)
  #define MC0_SC_QOS_UARCH_Force_MCVC1Demote_MAX                       (1) // 0x00000001
  #define MC0_SC_QOS_UARCH_Force_MCVC1Demote_DEF                       (0x00000000)
  #define MC0_SC_QOS_UARCH_Force_MCVC1Demote_HSH                       (0x0108DA50)

  #define MC0_SC_QOS_UARCH_Disable_MCVC1Demote_OFF                     ( 5)
  #define MC0_SC_QOS_UARCH_Disable_MCVC1Demote_WID                     ( 1)
  #define MC0_SC_QOS_UARCH_Disable_MCVC1Demote_MSK                     (0x00000020)
  #define MC0_SC_QOS_UARCH_Disable_MCVC1Demote_MIN                     (0)
  #define MC0_SC_QOS_UARCH_Disable_MCVC1Demote_MAX                     (1) // 0x00000001
  #define MC0_SC_QOS_UARCH_Disable_MCVC1Demote_DEF                     (0x00000000)
  #define MC0_SC_QOS_UARCH_Disable_MCVC1Demote_HSH                     (0x010ADA50)

  #define MC0_SC_QOS_UARCH_MC_Ignore_VC1Demote_OFF                     ( 6)
  #define MC0_SC_QOS_UARCH_MC_Ignore_VC1Demote_WID                     ( 1)
  #define MC0_SC_QOS_UARCH_MC_Ignore_VC1Demote_MSK                     (0x00000040)
  #define MC0_SC_QOS_UARCH_MC_Ignore_VC1Demote_MIN                     (0)
  #define MC0_SC_QOS_UARCH_MC_Ignore_VC1Demote_MAX                     (1) // 0x00000001
  #define MC0_SC_QOS_UARCH_MC_Ignore_VC1Demote_DEF                     (0x00000000)
  #define MC0_SC_QOS_UARCH_MC_Ignore_VC1Demote_HSH                     (0x010CDA50)

  #define MC0_SC_QOS_UARCH_Ignore_RGBSync_OFF                          ( 7)
  #define MC0_SC_QOS_UARCH_Ignore_RGBSync_WID                          ( 1)
  #define MC0_SC_QOS_UARCH_Ignore_RGBSync_MSK                          (0x00000080)
  #define MC0_SC_QOS_UARCH_Ignore_RGBSync_MIN                          (0)
  #define MC0_SC_QOS_UARCH_Ignore_RGBSync_MAX                          (1) // 0x00000001
  #define MC0_SC_QOS_UARCH_Ignore_RGBSync_DEF                          (0x00000000)
  #define MC0_SC_QOS_UARCH_Ignore_RGBSync_HSH                          (0x010EDA50)

  #define MC0_SC_QOS_UARCH_Force_MC_WPriority_OFF                      ( 8)
  #define MC0_SC_QOS_UARCH_Force_MC_WPriority_WID                      ( 1)
  #define MC0_SC_QOS_UARCH_Force_MC_WPriority_MSK                      (0x00000100)
  #define MC0_SC_QOS_UARCH_Force_MC_WPriority_MIN                      (0)
  #define MC0_SC_QOS_UARCH_Force_MC_WPriority_MAX                      (1) // 0x00000001
  #define MC0_SC_QOS_UARCH_Force_MC_WPriority_DEF                      (0x00000000)
  #define MC0_SC_QOS_UARCH_Force_MC_WPriority_HSH                      (0x0110DA50)

  #define MC0_SC_QOS_UARCH_Disable_MC_WPriority_OFF                    ( 9)
  #define MC0_SC_QOS_UARCH_Disable_MC_WPriority_WID                    ( 1)
  #define MC0_SC_QOS_UARCH_Disable_MC_WPriority_MSK                    (0x00000200)
  #define MC0_SC_QOS_UARCH_Disable_MC_WPriority_MIN                    (0)
  #define MC0_SC_QOS_UARCH_Disable_MC_WPriority_MAX                    (1) // 0x00000001
  #define MC0_SC_QOS_UARCH_Disable_MC_WPriority_DEF                    (0x00000000)
  #define MC0_SC_QOS_UARCH_Disable_MC_WPriority_HSH                    (0x0112DA50)

  #define MC0_SC_QOS_UARCH_allow_cross_vc_blocking_OFF                 (10)
  #define MC0_SC_QOS_UARCH_allow_cross_vc_blocking_WID                 ( 1)
  #define MC0_SC_QOS_UARCH_allow_cross_vc_blocking_MSK                 (0x00000400)
  #define MC0_SC_QOS_UARCH_allow_cross_vc_blocking_MIN                 (0)
  #define MC0_SC_QOS_UARCH_allow_cross_vc_blocking_MAX                 (1) // 0x00000001
  #define MC0_SC_QOS_UARCH_allow_cross_vc_blocking_DEF                 (0x00000001)
  #define MC0_SC_QOS_UARCH_allow_cross_vc_blocking_HSH                 (0x0114DA50)

  #define MC0_SC_QOS_UARCH_VC1_block_VC0_OFF                           (11)
  #define MC0_SC_QOS_UARCH_VC1_block_VC0_WID                           ( 1)
  #define MC0_SC_QOS_UARCH_VC1_block_VC0_MSK                           (0x00000800)
  #define MC0_SC_QOS_UARCH_VC1_block_VC0_MIN                           (0)
  #define MC0_SC_QOS_UARCH_VC1_block_VC0_MAX                           (1) // 0x00000001
  #define MC0_SC_QOS_UARCH_VC1_block_VC0_DEF                           (0x00000000)
  #define MC0_SC_QOS_UARCH_VC1_block_VC0_HSH                           (0x0116DA50)

  #define MC0_SC_QOS_UARCH_VC0_block_VC1_OFF                           (12)
  #define MC0_SC_QOS_UARCH_VC0_block_VC1_WID                           ( 1)
  #define MC0_SC_QOS_UARCH_VC0_block_VC1_MSK                           (0x00001000)
  #define MC0_SC_QOS_UARCH_VC0_block_VC1_MIN                           (0)
  #define MC0_SC_QOS_UARCH_VC0_block_VC1_MAX                           (1) // 0x00000001
  #define MC0_SC_QOS_UARCH_VC0_block_VC1_DEF                           (0x00000000)
  #define MC0_SC_QOS_UARCH_VC0_block_VC1_HSH                           (0x0118DA50)

  #define MC0_SC_QOS_UARCH_Delay_VC1_on_read_starvation_OFF            (13)
  #define MC0_SC_QOS_UARCH_Delay_VC1_on_read_starvation_WID            ( 1)
  #define MC0_SC_QOS_UARCH_Delay_VC1_on_read_starvation_MSK            (0x00002000)
  #define MC0_SC_QOS_UARCH_Delay_VC1_on_read_starvation_MIN            (0)
  #define MC0_SC_QOS_UARCH_Delay_VC1_on_read_starvation_MAX            (1) // 0x00000001
  #define MC0_SC_QOS_UARCH_Delay_VC1_on_read_starvation_DEF            (0x00000000)
  #define MC0_SC_QOS_UARCH_Delay_VC1_on_read_starvation_HSH            (0x011ADA50)

#define MC0_DDRPL_CFG_DTF_REG                                          (0x0000DF00)
#define MC0_DDRPL_CFG_DTF_DEF                                          (0x000E0000)
#define MC0_DDRPL_CFG_DTF_VOLATILE_BITFIELDS_MSK                       (0x00000000)

  #define MC0_DDRPL_CFG_DTF_MCHTrace_OFF                               ( 4)
  #define MC0_DDRPL_CFG_DTF_MCHTrace_WID                               ( 1)
  #define MC0_DDRPL_CFG_DTF_MCHTrace_MSK                               (0x00000010)
  #define MC0_DDRPL_CFG_DTF_MCHTrace_MIN                               (0)
  #define MC0_DDRPL_CFG_DTF_MCHTrace_MAX                               (1) // 0x00000001
  #define MC0_DDRPL_CFG_DTF_MCHTrace_DEF                               (0x00000000)
  #define MC0_DDRPL_CFG_DTF_MCHTrace_HSH                               (0x0108DF00)

  #define MC0_DDRPL_CFG_DTF_SCHTrace_OFF                               ( 5)
  #define MC0_DDRPL_CFG_DTF_SCHTrace_WID                               ( 1)
  #define MC0_DDRPL_CFG_DTF_SCHTrace_MSK                               (0x00000020)
  #define MC0_DDRPL_CFG_DTF_SCHTrace_MIN                               (0)
  #define MC0_DDRPL_CFG_DTF_SCHTrace_MAX                               (1) // 0x00000001
  #define MC0_DDRPL_CFG_DTF_SCHTrace_DEF                               (0x00000000)
  #define MC0_DDRPL_CFG_DTF_SCHTrace_HSH                               (0x010ADF00)

  #define MC0_DDRPL_CFG_DTF_Switch_64b_Data_Trace_OFF                  (10)
  #define MC0_DDRPL_CFG_DTF_Switch_64b_Data_Trace_WID                  ( 1)
  #define MC0_DDRPL_CFG_DTF_Switch_64b_Data_Trace_MSK                  (0x00000400)
  #define MC0_DDRPL_CFG_DTF_Switch_64b_Data_Trace_MIN                  (0)
  #define MC0_DDRPL_CFG_DTF_Switch_64b_Data_Trace_MAX                  (1) // 0x00000001
  #define MC0_DDRPL_CFG_DTF_Switch_64b_Data_Trace_DEF                  (0x00000000)
  #define MC0_DDRPL_CFG_DTF_Switch_64b_Data_Trace_HSH                  (0x0114DF00)

  #define MC0_DDRPL_CFG_DTF_DVP_Never_Block_Req_OFF                    (11)
  #define MC0_DDRPL_CFG_DTF_DVP_Never_Block_Req_WID                    ( 1)
  #define MC0_DDRPL_CFG_DTF_DVP_Never_Block_Req_MSK                    (0x00000800)
  #define MC0_DDRPL_CFG_DTF_DVP_Never_Block_Req_MIN                    (0)
  #define MC0_DDRPL_CFG_DTF_DVP_Never_Block_Req_MAX                    (1) // 0x00000001
  #define MC0_DDRPL_CFG_DTF_DVP_Never_Block_Req_DEF                    (0x00000000)
  #define MC0_DDRPL_CFG_DTF_DVP_Never_Block_Req_HSH                    (0x0116DF00)

  #define MC0_DDRPL_CFG_DTF_DVP_Block_Req_on_PMA_clk_stop_OFF          (12)
  #define MC0_DDRPL_CFG_DTF_DVP_Block_Req_on_PMA_clk_stop_WID          ( 1)
  #define MC0_DDRPL_CFG_DTF_DVP_Block_Req_on_PMA_clk_stop_MSK          (0x00001000)
  #define MC0_DDRPL_CFG_DTF_DVP_Block_Req_on_PMA_clk_stop_MIN          (0)
  #define MC0_DDRPL_CFG_DTF_DVP_Block_Req_on_PMA_clk_stop_MAX          (1) // 0x00000001
  #define MC0_DDRPL_CFG_DTF_DVP_Block_Req_on_PMA_clk_stop_DEF          (0x00000000)
  #define MC0_DDRPL_CFG_DTF_DVP_Block_Req_on_PMA_clk_stop_HSH          (0x0118DF00)

  #define MC0_DDRPL_CFG_DTF_DSO_BlkReq_GVInProgress_DIS_OFF            (13)
  #define MC0_DDRPL_CFG_DTF_DSO_BlkReq_GVInProgress_DIS_WID            ( 1)
  #define MC0_DDRPL_CFG_DTF_DSO_BlkReq_GVInProgress_DIS_MSK            (0x00002000)
  #define MC0_DDRPL_CFG_DTF_DSO_BlkReq_GVInProgress_DIS_MIN            (0)
  #define MC0_DDRPL_CFG_DTF_DSO_BlkReq_GVInProgress_DIS_MAX            (1) // 0x00000001
  #define MC0_DDRPL_CFG_DTF_DSO_BlkReq_GVInProgress_DIS_DEF            (0x00000000)
  #define MC0_DDRPL_CFG_DTF_DSO_BlkReq_GVInProgress_DIS_HSH            (0x011ADF00)

  #define MC0_DDRPL_CFG_DTF_DTF_Survive_Mode_OFF                       (14)
  #define MC0_DDRPL_CFG_DTF_DTF_Survive_Mode_WID                       ( 1)
  #define MC0_DDRPL_CFG_DTF_DTF_Survive_Mode_MSK                       (0x00004000)
  #define MC0_DDRPL_CFG_DTF_DTF_Survive_Mode_MIN                       (0)
  #define MC0_DDRPL_CFG_DTF_DTF_Survive_Mode_MAX                       (1) // 0x00000001
  #define MC0_DDRPL_CFG_DTF_DTF_Survive_Mode_DEF                       (0x00000000)
  #define MC0_DDRPL_CFG_DTF_DTF_Survive_Mode_HSH                       (0x011CDF00)

  #define MC0_DDRPL_CFG_DTF_CFI_OBS_clk_resume_cbit_OFF                (15)
  #define MC0_DDRPL_CFG_DTF_CFI_OBS_clk_resume_cbit_WID                ( 1)
  #define MC0_DDRPL_CFG_DTF_CFI_OBS_clk_resume_cbit_MSK                (0x00008000)
  #define MC0_DDRPL_CFG_DTF_CFI_OBS_clk_resume_cbit_MIN                (0)
  #define MC0_DDRPL_CFG_DTF_CFI_OBS_clk_resume_cbit_MAX                (1) // 0x00000001
  #define MC0_DDRPL_CFG_DTF_CFI_OBS_clk_resume_cbit_DEF                (0x00000000)
  #define MC0_DDRPL_CFG_DTF_CFI_OBS_clk_resume_cbit_HSH                (0x011EDF00)

  #define MC0_DDRPL_CFG_DTF_Enable_ACC_Trace_OFF                       (17)
  #define MC0_DDRPL_CFG_DTF_Enable_ACC_Trace_WID                       ( 1)
  #define MC0_DDRPL_CFG_DTF_Enable_ACC_Trace_MSK                       (0x00020000)
  #define MC0_DDRPL_CFG_DTF_Enable_ACC_Trace_MIN                       (0)
  #define MC0_DDRPL_CFG_DTF_Enable_ACC_Trace_MAX                       (1) // 0x00000001
  #define MC0_DDRPL_CFG_DTF_Enable_ACC_Trace_DEF                       (0x00000001)
  #define MC0_DDRPL_CFG_DTF_Enable_ACC_Trace_HSH                       (0x0122DF00)

  #define MC0_DDRPL_CFG_DTF_Enable_ReadData_Trace_OFF                  (18)
  #define MC0_DDRPL_CFG_DTF_Enable_ReadData_Trace_WID                  ( 1)
  #define MC0_DDRPL_CFG_DTF_Enable_ReadData_Trace_MSK                  (0x00040000)
  #define MC0_DDRPL_CFG_DTF_Enable_ReadData_Trace_MIN                  (0)
  #define MC0_DDRPL_CFG_DTF_Enable_ReadData_Trace_MAX                  (1) // 0x00000001
  #define MC0_DDRPL_CFG_DTF_Enable_ReadData_Trace_DEF                  (0x00000001)
  #define MC0_DDRPL_CFG_DTF_Enable_ReadData_Trace_HSH                  (0x0124DF00)

  #define MC0_DDRPL_CFG_DTF_Enable_WriteData_Trace_OFF                 (19)
  #define MC0_DDRPL_CFG_DTF_Enable_WriteData_Trace_WID                 ( 1)
  #define MC0_DDRPL_CFG_DTF_Enable_WriteData_Trace_MSK                 (0x00080000)
  #define MC0_DDRPL_CFG_DTF_Enable_WriteData_Trace_MIN                 (0)
  #define MC0_DDRPL_CFG_DTF_Enable_WriteData_Trace_MAX                 (1) // 0x00000001
  #define MC0_DDRPL_CFG_DTF_Enable_WriteData_Trace_DEF                 (0x00000001)
  #define MC0_DDRPL_CFG_DTF_Enable_WriteData_Trace_HSH                 (0x0126DF00)

  #define MC0_DDRPL_CFG_DTF_DDRPL_Activate_OFF                         (20)
  #define MC0_DDRPL_CFG_DTF_DDRPL_Activate_WID                         ( 1)
  #define MC0_DDRPL_CFG_DTF_DDRPL_Activate_MSK                         (0x00100000)
  #define MC0_DDRPL_CFG_DTF_DDRPL_Activate_MIN                         (0)
  #define MC0_DDRPL_CFG_DTF_DDRPL_Activate_MAX                         (1) // 0x00000001
  #define MC0_DDRPL_CFG_DTF_DDRPL_Activate_DEF                         (0x00000000)
  #define MC0_DDRPL_CFG_DTF_DDRPL_Activate_HSH                         (0x0128DF00)

  #define MC0_DDRPL_CFG_DTF_DDRPL_GLB_DRV_GATE_DIS_OFF                 (21)
  #define MC0_DDRPL_CFG_DTF_DDRPL_GLB_DRV_GATE_DIS_WID                 ( 1)
  #define MC0_DDRPL_CFG_DTF_DDRPL_GLB_DRV_GATE_DIS_MSK                 (0x00200000)
  #define MC0_DDRPL_CFG_DTF_DDRPL_GLB_DRV_GATE_DIS_MIN                 (0)
  #define MC0_DDRPL_CFG_DTF_DDRPL_GLB_DRV_GATE_DIS_MAX                 (1) // 0x00000001
  #define MC0_DDRPL_CFG_DTF_DDRPL_GLB_DRV_GATE_DIS_DEF                 (0x00000000)
  #define MC0_DDRPL_CFG_DTF_DDRPL_GLB_DRV_GATE_DIS_HSH                 (0x012ADF00)

  #define MC0_DDRPL_CFG_DTF_CFI_OBS_BLOCK_ON_SAGV_OFF                  (22)
  #define MC0_DDRPL_CFG_DTF_CFI_OBS_BLOCK_ON_SAGV_WID                  ( 1)
  #define MC0_DDRPL_CFG_DTF_CFI_OBS_BLOCK_ON_SAGV_MSK                  (0x00400000)
  #define MC0_DDRPL_CFG_DTF_CFI_OBS_BLOCK_ON_SAGV_MIN                  (0)
  #define MC0_DDRPL_CFG_DTF_CFI_OBS_BLOCK_ON_SAGV_MAX                  (1) // 0x00000001
  #define MC0_DDRPL_CFG_DTF_CFI_OBS_BLOCK_ON_SAGV_DEF                  (0x00000000)
  #define MC0_DDRPL_CFG_DTF_CFI_OBS_BLOCK_ON_SAGV_HSH                  (0x012CDF00)

  #define MC0_DDRPL_CFG_DTF_Spare_RW_OFF                               (29)
  #define MC0_DDRPL_CFG_DTF_Spare_RW_WID                               ( 3)
  #define MC0_DDRPL_CFG_DTF_Spare_RW_MSK                               (0xE0000000)
  #define MC0_DDRPL_CFG_DTF_Spare_RW_MIN                               (0)
  #define MC0_DDRPL_CFG_DTF_Spare_RW_MAX                               (3) // 0x00000003
  #define MC0_DDRPL_CFG_DTF_Spare_RW_DEF                               (0x00000000)
  #define MC0_DDRPL_CFG_DTF_Spare_RW_HSH                               (0x033ADF00)

#define MC0_DDRPL_DEBUG_DTF_REG                                        (0x0000DF08)
#define MC0_DDRPL_DEBUG_DTF_DEF                                        (0x00000000)
#define MC0_DDRPL_DEBUG_DTF_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define MC0_DDRPL_DEBUG_DTF_Read_Data_UserDefined_Bits_OFF           (16)
  #define MC0_DDRPL_DEBUG_DTF_Read_Data_UserDefined_Bits_WID           ( 8)
  #define MC0_DDRPL_DEBUG_DTF_Read_Data_UserDefined_Bits_MSK           (0x00FF0000)
  #define MC0_DDRPL_DEBUG_DTF_Read_Data_UserDefined_Bits_MIN           (0)
  #define MC0_DDRPL_DEBUG_DTF_Read_Data_UserDefined_Bits_MAX           (255) // 0x000000FF
  #define MC0_DDRPL_DEBUG_DTF_Read_Data_UserDefined_Bits_DEF           (0x00000000)
  #define MC0_DDRPL_DEBUG_DTF_Read_Data_UserDefined_Bits_HSH           (0x0820DF08)

  #define MC0_DDRPL_DEBUG_DTF_Write_Data_UserDefined_Bits_OFF          (24)
  #define MC0_DDRPL_DEBUG_DTF_Write_Data_UserDefined_Bits_WID          ( 8)
  #define MC0_DDRPL_DEBUG_DTF_Write_Data_UserDefined_Bits_MSK          (0xFF000000)
  #define MC0_DDRPL_DEBUG_DTF_Write_Data_UserDefined_Bits_MIN          (0)
  #define MC0_DDRPL_DEBUG_DTF_Write_Data_UserDefined_Bits_MAX          (255) // 0x000000FF
  #define MC0_DDRPL_DEBUG_DTF_Write_Data_UserDefined_Bits_DEF          (0x00000000)
  #define MC0_DDRPL_DEBUG_DTF_Write_Data_UserDefined_Bits_HSH          (0x0830DF08)

#define MC0_DDRPL_VISA_LANES_REG                                       (0x0000DF0C)
#define MC0_DDRPL_VISA_LANES_DEF                                       (0x00000000)
#define MC0_DDRPL_VISA_LANES_VOLATILE_BITFIELDS_MSK                    (0xFFFFFFFF)

  #define MC0_DDRPL_VISA_LANES_Visa_Qclk_Lane_1_out_OFF                ( 0)
  #define MC0_DDRPL_VISA_LANES_Visa_Qclk_Lane_1_out_WID                ( 8)
  #define MC0_DDRPL_VISA_LANES_Visa_Qclk_Lane_1_out_MSK                (0x000000FF)
  #define MC0_DDRPL_VISA_LANES_Visa_Qclk_Lane_1_out_MIN                (0)
  #define MC0_DDRPL_VISA_LANES_Visa_Qclk_Lane_1_out_MAX                (255) // 0x000000FF
  #define MC0_DDRPL_VISA_LANES_Visa_Qclk_Lane_1_out_DEF                (0x00000000)
  #define MC0_DDRPL_VISA_LANES_Visa_Qclk_Lane_1_out_HSH                (0x0880DF0C)

  #define MC0_DDRPL_VISA_LANES_Visa_Qclk_Lane_0_out_OFF                ( 8)
  #define MC0_DDRPL_VISA_LANES_Visa_Qclk_Lane_0_out_WID                ( 8)
  #define MC0_DDRPL_VISA_LANES_Visa_Qclk_Lane_0_out_MSK                (0x0000FF00)
  #define MC0_DDRPL_VISA_LANES_Visa_Qclk_Lane_0_out_MIN                (0)
  #define MC0_DDRPL_VISA_LANES_Visa_Qclk_Lane_0_out_MAX                (255) // 0x000000FF
  #define MC0_DDRPL_VISA_LANES_Visa_Qclk_Lane_0_out_DEF                (0x00000000)
  #define MC0_DDRPL_VISA_LANES_Visa_Qclk_Lane_0_out_HSH                (0x0890DF0C)

  #define MC0_DDRPL_VISA_LANES_Visa_Dclk_Lane_1_out_OFF                (16)
  #define MC0_DDRPL_VISA_LANES_Visa_Dclk_Lane_1_out_WID                ( 8)
  #define MC0_DDRPL_VISA_LANES_Visa_Dclk_Lane_1_out_MSK                (0x00FF0000)
  #define MC0_DDRPL_VISA_LANES_Visa_Dclk_Lane_1_out_MIN                (0)
  #define MC0_DDRPL_VISA_LANES_Visa_Dclk_Lane_1_out_MAX                (255) // 0x000000FF
  #define MC0_DDRPL_VISA_LANES_Visa_Dclk_Lane_1_out_DEF                (0x00000000)
  #define MC0_DDRPL_VISA_LANES_Visa_Dclk_Lane_1_out_HSH                (0x08A0DF0C)

  #define MC0_DDRPL_VISA_LANES_Visa_Dclk_Lane_0_out_OFF                (24)
  #define MC0_DDRPL_VISA_LANES_Visa_Dclk_Lane_0_out_WID                ( 8)
  #define MC0_DDRPL_VISA_LANES_Visa_Dclk_Lane_0_out_MSK                (0xFF000000)
  #define MC0_DDRPL_VISA_LANES_Visa_Dclk_Lane_0_out_MIN                (0)
  #define MC0_DDRPL_VISA_LANES_Visa_Dclk_Lane_0_out_MAX                (255) // 0x000000FF
  #define MC0_DDRPL_VISA_LANES_Visa_Dclk_Lane_0_out_DEF                (0x00000000)
  #define MC0_DDRPL_VISA_LANES_Visa_Dclk_Lane_0_out_HSH                (0x08B0DF0C)

#define MC0_DDRPL_VISA_CFG_DTF_REG                                     (0x0000DF10)
#define MC0_DDRPL_VISA_CFG_DTF_DEF                                     (0x00000000)
#define MC0_DDRPL_VISA_CFG_DTF_VOLATILE_BITFIELDS_MSK                  (0x00000000)

  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane0_Data_Stage_Dclk_tree_OFF    ( 0)
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane0_Data_Stage_Dclk_tree_WID    ( 2)
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane0_Data_Stage_Dclk_tree_MSK    (0x00000003)
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane0_Data_Stage_Dclk_tree_MIN    (0)
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane0_Data_Stage_Dclk_tree_MAX    (7) // 0x00000007
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane0_Data_Stage_Dclk_tree_DEF    (0x00000000)
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane0_Data_Stage_Dclk_tree_HSH    (0x0200DF10)

  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane1_Data_Stage_Dclk_tree_OFF    ( 2)
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane1_Data_Stage_Dclk_tree_WID    ( 2)
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane1_Data_Stage_Dclk_tree_MSK    (0x0000000C)
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane1_Data_Stage_Dclk_tree_MIN    (0)
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane1_Data_Stage_Dclk_tree_MAX    (7) // 0x00000007
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane1_Data_Stage_Dclk_tree_DEF    (0x00000000)
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane1_Data_Stage_Dclk_tree_HSH    (0x0204DF10)

  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane0_Data_Stage_Qclk_tree_OFF    ( 4)
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane0_Data_Stage_Qclk_tree_WID    ( 2)
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane0_Data_Stage_Qclk_tree_MSK    (0x00000030)
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane0_Data_Stage_Qclk_tree_MIN    (0)
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane0_Data_Stage_Qclk_tree_MAX    (7) // 0x00000007
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane0_Data_Stage_Qclk_tree_DEF    (0x00000000)
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane0_Data_Stage_Qclk_tree_HSH    (0x0208DF10)

  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane1_Data_Stage_Qclk_tree_OFF    ( 6)
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane1_Data_Stage_Qclk_tree_WID    ( 2)
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane1_Data_Stage_Qclk_tree_MSK    (0x000000C0)
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane1_Data_Stage_Qclk_tree_MIN    (0)
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane1_Data_Stage_Qclk_tree_MAX    (7) // 0x00000007
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane1_Data_Stage_Qclk_tree_DEF    (0x00000000)
  #define MC0_DDRPL_VISA_CFG_DTF_VisaLane1_Data_Stage_Qclk_tree_HSH    (0x020CDF10)

#define MC0_DDRPL_DRAMADDRESS_FILTER_REG                               (0x0000DF14)
#define MC0_DDRPL_DRAMADDRESS_FILTER_DEF                               (0x00000000)
#define MC0_DDRPL_DRAMADDRESS_FILTER_VOLATILE_BITFIELDS_MSK            (0x00000000)

  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Rank_EN_OFF         ( 0)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Rank_EN_WID         ( 1)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Rank_EN_MSK         (0x00000001)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Rank_EN_MIN         (0)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Rank_EN_MAX         (1) // 0x00000001
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Rank_EN_DEF         (0x00000000)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Rank_EN_HSH         (0x0100DF14)

  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Group_EN_OFF        ( 1)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Group_EN_WID        ( 1)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Group_EN_MSK        (0x00000002)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Group_EN_MIN        (0)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Group_EN_MAX        (1) // 0x00000001
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Group_EN_DEF        (0x00000000)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Group_EN_HSH        (0x0102DF14)

  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Bank_EN_OFF         ( 2)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Bank_EN_WID         ( 1)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Bank_EN_MSK         (0x00000004)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Bank_EN_MIN         (0)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Bank_EN_MAX         (1) // 0x00000001
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Bank_EN_DEF         (0x00000000)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Bank_EN_HSH         (0x0104DF14)

  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Row_EN_OFF          ( 3)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Row_EN_WID          ( 1)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Row_EN_MSK          (0x00000008)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Row_EN_MIN          (0)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Row_EN_MAX          (1) // 0x00000001
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Row_EN_DEF          (0x00000000)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Row_EN_HSH          (0x0106DF14)

  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Column_EN_OFF       ( 4)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Column_EN_WID       ( 1)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Column_EN_MSK       (0x00000010)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Column_EN_MIN       (0)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Column_EN_MAX       (1) // 0x00000001
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Column_EN_DEF       (0x00000000)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Data_Filter_Column_EN_HSH       (0x0108DF14)

  #define MC0_DDRPL_DRAMADDRESS_FILTER_Column_Value_OFF                ( 5)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Column_Value_WID                ( 8)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Column_Value_MSK                (0x00001FE0)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Column_Value_MIN                (0)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Column_Value_MAX                (1) // 0x00000001
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Column_Value_DEF                (0x00000000)
  #define MC0_DDRPL_DRAMADDRESS_FILTER_Column_Value_HSH                (0x080ADF14)

#define MC0_DDRPL_DRAMADDRESS_FILTER2_REG                              (0x0000DF18)
#define MC0_DDRPL_DRAMADDRESS_FILTER2_DEF                              (0x00000000)
#define MC0_DDRPL_DRAMADDRESS_FILTER2_VOLATILE_BITFIELDS_MSK           (0x00000000)

  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Rank_Value_OFF                 ( 0)
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Rank_Value_WID                 ( 2)
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Rank_Value_MSK                 (0x00000003)
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Rank_Value_MIN                 (0)
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Rank_Value_MAX                 (1) // 0x00000001
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Rank_Value_DEF                 (0x00000000)
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Rank_Value_HSH                 (0x0200DF18)

  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Group_Value_OFF                ( 2)
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Group_Value_WID                ( 2)
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Group_Value_MSK                (0x0000000C)
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Group_Value_MIN                (0)
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Group_Value_MAX                (1) // 0x00000001
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Group_Value_DEF                (0x00000000)
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Group_Value_HSH                (0x0204DF18)

  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Bank_Value_OFF                 ( 4)
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Bank_Value_WID                 ( 3)
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Bank_Value_MSK                 (0x00000070)
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Bank_Value_MIN                 (0)
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Bank_Value_MAX                 (1) // 0x00000001
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Bank_Value_DEF                 (0x00000000)
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Bank_Value_HSH                 (0x0308DF18)

  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Row_Value_OFF                  ( 7)
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Row_Value_WID                  (18)
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Row_Value_MSK                  (0x01FFFF80)
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Row_Value_MIN                  (0)
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Row_Value_MAX                  (1) // 0x00000001
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Row_Value_DEF                  (0x00000000)
  #define MC0_DDRPL_DRAMADDRESS_FILTER2_Row_Value_HSH                  (0x120EDF18)
#pragma pack(pop)
#endif
