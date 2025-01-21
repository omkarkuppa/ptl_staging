#ifndef __MrcRegisterStructPtl5xxx_h__
#define __MrcRegisterStructPtl5xxx_h__
/** @file
  This file was automatically generated. Modify at your own risk.
  Note that no error checking is done in these functions so ensure that the correct values are passed.

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

#pragma pack(push, 1)
typedef union {
  struct {
    UINT32 DATA                                    :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CAMARILLO_MAILBOX_DATA0_PCU_STRUCT;

typedef CAMARILLO_MAILBOX_DATA0_PCU_STRUCT CAMARILLO_MAILBOX_DATA1_PCU_STRUCT;
typedef union {
  struct {
    UINT32 COMMAND                                 :  8;  // Bits 7:0
    UINT32 PARAM1                                  :  8;  // Bits 15:8
    UINT32 PARAM2                                  :  14;  // Bits 29:16
    UINT32                                         :  1;  // Bits 30:30
    UINT32 RUN_BUSY                                :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CAMARILLO_MAILBOX_INTERFACE_PCU_STRUCT;
typedef union {
  struct {
    UINT32 HIGH_TEMP_INT_ENABLE                    :  1;  // Bits 0:0
    UINT32 LOW_TEMP_INT_ENABLE                     :  1;  // Bits 1:1
    UINT32 PROCHOT_INT_ENABLE                      :  1;  // Bits 2:2
    UINT32                                         :  1;  // Bits 3:3
    UINT32 OUT_OF_SPEC_INT_ENABLE                  :  1;  // Bits 4:4
    UINT32                                         :  3;  // Bits 7:5
    UINT32 THRESHOLD_1_REL_TEMP                    :  7;  // Bits 14:8
    UINT32 THRESHOLD_1_INT_ENABLE                  :  1;  // Bits 15:15
    UINT32 THRESHOLD_2_REL_TEMP                    :  7;  // Bits 22:16
    UINT32 THRESHOLD_2_INT_ENABLE                  :  1;  // Bits 23:23
    UINT32 POWER_INT_ENABLE                        :  1;  // Bits 24:24
    UINT32 TEMPERATURE_AVERAGING_TIME_WINDOW       :  7;  // Bits 31:25
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PACKAGE_THERM_CAMARILLO_INTERRUPT_PCU_STRUCT;
typedef union {
  struct {
    UINT32 PostCode                                :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} BIOS_POST_CODE_PCU_STRUCT;
typedef union {
  struct {
    UINT32 DATALow                                 :  32;  // Bits 31:0
    UINT32 DATAHigh                                :  32;  // Bits 63:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} PKG_IA_C0_ANY_SUM_PCU_STRUCT;

typedef PKG_IA_C0_ANY_SUM_PCU_STRUCT PKG_IA_C0_ANY_PCU_STRUCT;
typedef union {
  struct {
    UINT32 PROCHOT                                 :  1;  // Bits 0:0
    UINT32 THERMAL                                 :  1;  // Bits 1:1
    UINT32 PBM_DDR                                 :  1;  // Bits 2:2
    UINT32 REASON_3                                :  1;  // Bits 3:3
    UINT32 REASON_4                                :  1;  // Bits 4:4
    UINT32 RATL                                    :  1;  // Bits 5:5
    UINT32 VR_THERMALERT                           :  1;  // Bits 6:6
    UINT32 VR_TDC                                  :  1;  // Bits 7:7
    UINT32 OTHER                                   :  1;  // Bits 8:8
    UINT32 REASON_9                                :  1;  // Bits 9:9
    UINT32 PBM_PL1                                 :  1;  // Bits 10:10
    UINT32 PBM_PL2                                 :  1;  // Bits 11:11
    UINT32 REASON_12                               :  1;  // Bits 12:12
    UINT32 REASON_13                               :  1;  // Bits 13:13
    UINT32 MSPE                                    :  1;  // Bits 14:14
    UINT32 REASON_15                               :  1;  // Bits 15:15
    UINT32 PROCHOT_LOG                             :  1;  // Bits 16:16
    UINT32 THERMAL_LOG                             :  1;  // Bits 17:17
    UINT32 PBM_DDR_LOG                             :  1;  // Bits 18:18
    UINT32 REASON_3_LOG                            :  1;  // Bits 19:19
    UINT32 REASON_4_LOG                            :  1;  // Bits 20:20
    UINT32 RATL_LOG                                :  1;  // Bits 21:21
    UINT32 VR_THERMALERT_LOG                       :  1;  // Bits 22:22
    UINT32 VR_TDC_LOG                              :  1;  // Bits 23:23
    UINT32 OTHER_LOG                               :  1;  // Bits 24:24
    UINT32 REASON_9_LOG                            :  1;  // Bits 25:25
    UINT32 PBM_PL1_LOG                             :  1;  // Bits 26:26
    UINT32 PBM_PL2_LOG                             :  1;  // Bits 27:27
    UINT32 REASON_12_LOG                           :  1;  // Bits 28:28
    UINT32 REASON_13_LOG                           :  1;  // Bits 29:29
    UINT32 MSPE_LOG                                :  1;  // Bits 30:30
    UINT32 REASON_15_LOG                           :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} QCLK_PERF_LIMIT_REASONS_PCU_STRUCT;

typedef PKG_IA_C0_ANY_SUM_PCU_STRUCT PKG_IA_C0_ANY_RATIO_PCU_STRUCT;
typedef union {
  struct {
    UINT32 RESERVED_0                              :  1;  // Bits 0:0
    UINT32 RESERVED_1                              :  1;  // Bits 1:1
    UINT32                                         :  4;  // Bits 5:2
    UINT32 PDWN_CONFIG_CTL                         :  1;  // Bits 6:6
    UINT32                                         :  25;  // Bits 31:7
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDR_PTM_CTL_PCU_STRUCT;
typedef union {
  struct {
    UINT32 BANDWIDTH_THROTTLE                      :  4;  // Bits 3:0
    UINT32                                         :  28;  // Bits 31:4
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDR_BANDWIDTH_CONTROL_PCU_STRUCT;
typedef union {
  struct {
    UINT32 THRESHOLD1_INT_ENABLE                   :  1;  // Bits 0:0
    UINT32 THRESHOLD2_INT_ENABLE                   :  1;  // Bits 1:1
    UINT32                                         :  6;  // Bits 7:2
    UINT32 THRESHOLD1                              :  8;  // Bits 15:8
    UINT32 THRESHOLD2                              :  8;  // Bits 23:16
    UINT32 TIME_WINDOW                             :  7;  // Bits 30:24
    UINT32                                         :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDR_THERM_CAMARILLO_INTERRUPT_PCU_STRUCT;
typedef union {
  struct {
    UINT32 DDR_VOLTAGE                             :  3;  // Bits 2:0
    UINT32                                         :  29;  // Bits 31:3
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDR_VOLTAGE_PCU_STRUCT;
typedef union {
  struct {
    INT32  THERM_MARGIN                            :  16;  // Bits 15:0
    UINT32                                         :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PACKAGE_THERM_MARGIN_PCU_STRUCT;
typedef union {
  struct {
    UINT32 THERMAL_MONITOR_STATUS                  :  1;  // Bits 0:0
    UINT32 THERMAL_MONITOR_LOG                     :  1;  // Bits 1:1
    UINT32 PROCHOT_STATUS                          :  1;  // Bits 2:2
    UINT32 PROCHOT_LOG                             :  1;  // Bits 3:3
    UINT32 OUT_OF_SPEC_STATUS                      :  1;  // Bits 4:4
    UINT32 OUT_OF_SPEC_LOG                         :  1;  // Bits 5:5
    UINT32 THRESHOLD1_STATUS                       :  1;  // Bits 6:6
    UINT32 THRESHOLD1_LOG                          :  1;  // Bits 7:7
    UINT32 THRESHOLD2_STATUS                       :  1;  // Bits 8:8
    UINT32 THRESHOLD2_LOG                          :  1;  // Bits 9:9
    UINT32 POWER_LIMITATION_STATUS                 :  1;  // Bits 10:10
    UINT32 POWER_LIMITATION_LOG                    :  1;  // Bits 11:11
    UINT32                                         :  4;  // Bits 15:12
    UINT32 TEMPERATURE                             :  7;  // Bits 22:16
    UINT32                                         :  4;  // Bits 26:23
    UINT32 RESOLUTION                              :  4;  // Bits 30:27
    UINT32 VALID                                   :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PACKAGE_THERM_CAMARILLO_STATUS_PCU_STRUCT;
typedef union {
  struct {
    UINT32                                         :  8;  // Bits 7:0
    UINT32 THRESHOLD1_STATUS                       :  1;  // Bits 8:8
    UINT32 THRESHOLD1_LOG                          :  1;  // Bits 9:9
    UINT32 THRESHOLD2_STATUS                       :  1;  // Bits 10:10
    UINT32 THRESHOLD2_LOG                          :  1;  // Bits 11:11
    UINT32                                         :  20;  // Bits 31:12
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDR_THERM_CAMARILLO_STATUS_PCU_STRUCT;
typedef union {
  struct {
    UINT32 LIMIT1_POWER                            :  15;  // Bits 14:0
    UINT32 LIMIT1_ENABLE                           :  1;  // Bits 15:15
    UINT32                                         :  1;  // Bits 16:16
    UINT32 LIMIT1_TIME_WINDOW_Y                    :  5;  // Bits 21:17
    UINT32 LIMIT1_TIME_WINDOW_X                    :  2;  // Bits 23:22
    UINT32                                         :  8;  // Bits 31:24
    UINT32 LIMIT2_POWER                            :  15;  // Bits 46:32
    UINT32 LIMIT2_ENABLE                           :  1;  // Bits 47:47
    UINT32                                         :  1;  // Bits 48:48
    UINT32 LIMIT2_TIME_WINDOW_Y                    :  5;  // Bits 53:49
    UINT32 LIMIT2_TIME_WINDOW_X                    :  2;  // Bits 55:54
    UINT32                                         :  7;  // Bits 62:56
    UINT32 LOCKED                                  :  1;  // Bits 63:63
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} DDR_RAPL_LIMIT_PCU_STRUCT;
typedef union {
  struct {
    UINT32 JOULES_CONSUMED                         :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDR_ENERGY_STATUS_PCU_STRUCT;
typedef union {
  struct {
    UINT32 DURATION                                :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDR_RAPL_PERF_STATUS_PCU_STRUCT;
typedef union {
  struct {
    UINT32 COUNTS                                  :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PACKAGE_RAPL_PERF_STATUS_PCU_STRUCT;
typedef union {
  struct {
    UINT32 PROCHOT                                 :  1;  // Bits 0:0
    UINT32 THERMAL                                 :  1;  // Bits 1:1
    UINT32 SPARE_IA_2                              :  1;  // Bits 2:2
    UINT32 SPARE_IA_3                              :  1;  // Bits 3:3
    UINT32 RSR_LIMIT                               :  1;  // Bits 4:4
    UINT32 RATL                                    :  1;  // Bits 5:5
    UINT32 VR_THERMALERT                           :  1;  // Bits 6:6
    UINT32 VR_TDC                                  :  1;  // Bits 7:7
    UINT32 OTHER                                   :  1;  // Bits 8:8
    UINT32 SPARE_IA_9                              :  1;  // Bits 9:9
    UINT32 PBM_PL1                                 :  1;  // Bits 10:10
    UINT32 PBM_PL2                                 :  1;  // Bits 11:11
    UINT32 MAX_TURBO_LIMIT                         :  1;  // Bits 12:12
    UINT32 TURBO_ATTEN                             :  1;  // Bits 13:13
    UINT32 SPARE_IA_14                             :  1;  // Bits 14:14
    UINT32 SPARE_IA_15                             :  1;  // Bits 15:15
    UINT32 PROCHOT_LOG                             :  1;  // Bits 16:16
    UINT32 THERMAL_LOG                             :  1;  // Bits 17:17
    UINT32 SPARE_IA_2_LOG                          :  1;  // Bits 18:18
    UINT32 SPARE_IA_3_LOG                          :  1;  // Bits 19:19
    UINT32 RSR_LIMIT_LOG                           :  1;  // Bits 20:20
    UINT32 RATL_LOG                                :  1;  // Bits 21:21
    UINT32 VR_THERMALERT_LOG                       :  1;  // Bits 22:22
    UINT32 VR_TDC_LOG                              :  1;  // Bits 23:23
    UINT32 OTHER_LOG                               :  1;  // Bits 24:24
    UINT32 SPARE_IA_9_LOG                          :  1;  // Bits 25:25
    UINT32 PBM_PL1_LOG                             :  1;  // Bits 26:26
    UINT32 PBM_PL2_LOG                             :  1;  // Bits 27:27
    UINT32 MAX_TURBO_LIMIT_LOG                     :  1;  // Bits 28:28
    UINT32 TURBO_ATTEN_LOG                         :  1;  // Bits 29:29
    UINT32 SPARE_IA_14_LOG                         :  1;  // Bits 30:30
    UINT32 SPARE_IA_15_LOG                         :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} IA_PERF_LIMIT_REASONS_PCU_STRUCT;
typedef union {
  struct {
    UINT32 PROCHOT                                 :  1;  // Bits 0:0
    UINT32 THERMAL                                 :  1;  // Bits 1:1
    UINT32 PBM_DDR_LIMIT                           :  1;  // Bits 2:2
    UINT32 PCS_LIMIT                               :  1;  // Bits 3:3
    UINT32 RSR_LIMIT                               :  1;  // Bits 4:4
    UINT32 RATL                                    :  1;  // Bits 5:5
    UINT32 VR_THERMALERT                           :  1;  // Bits 6:6
    UINT32 VR_TDC                                  :  1;  // Bits 7:7
    UINT32 OTHER                                   :  1;  // Bits 8:8
    UINT32 SPARE_GT_9                              :  1;  // Bits 9:9
    UINT32 PBM_PL1                                 :  1;  // Bits 10:10
    UINT32 PBM_PL2                                 :  1;  // Bits 11:11
    UINT32 INEFFICIENT_OPERATION                   :  1;  // Bits 12:12
    UINT32 MAX_TURBO_LIMIT                         :  1;  // Bits 13:13
    UINT32 MSPE                                    :  1;  // Bits 14:14
    UINT32 SPARE_GT_15                             :  1;  // Bits 15:15
    UINT32 PROCHOT_LOG                             :  1;  // Bits 16:16
    UINT32 THERMAL_LOG                             :  1;  // Bits 17:17
    UINT32 PBM_DDR_LIMIT_LOG                       :  1;  // Bits 18:18
    UINT32 PCS_LIMIT_LOG                           :  1;  // Bits 19:19
    UINT32 RSR_LIMIT_LOG                           :  1;  // Bits 20:20
    UINT32 RATL_LOG                                :  1;  // Bits 21:21
    UINT32 VR_THERMALERT_LOG                       :  1;  // Bits 22:22
    UINT32 VR_TDC_LOG                              :  1;  // Bits 23:23
    UINT32 OTHER_LOG                               :  1;  // Bits 24:24
    UINT32 SPARE_GT_9_LOG                          :  1;  // Bits 25:25
    UINT32 PBM_PL1_LOG                             :  1;  // Bits 26:26
    UINT32 PBM_PL2_LOG                             :  1;  // Bits 27:27
    UINT32 INEFFICIENT_OPERATION_LOG               :  1;  // Bits 28:28
    UINT32 MAX_TURBO_LIMIT_LOG                     :  1;  // Bits 29:29
    UINT32 MSPE_LOG                                :  1;  // Bits 30:30
    UINT32 SPARE_GT_15_LOG                         :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GT_PERF_LIMIT_REASONS_PCU_STRUCT;
typedef union {
  struct {
    UINT32 PROCHOT                                 :  1;  // Bits 0:0
    UINT32 THERMAL                                 :  1;  // Bits 1:1
    UINT32 SPARE_CLR_2                             :  1;  // Bits 2:2
    UINT32 SPARE_CLR_3                             :  1;  // Bits 3:3
    UINT32 RSR                                     :  1;  // Bits 4:4
    UINT32 RATL                                    :  1;  // Bits 5:5
    UINT32 VR_THERMALERT                           :  1;  // Bits 6:6
    UINT32 VR_TDC                                  :  1;  // Bits 7:7
    UINT32 OTHER                                   :  1;  // Bits 8:8
    UINT32 SPARE_CLR_9                             :  1;  // Bits 9:9
    UINT32 PBM_PL1                                 :  1;  // Bits 10:10
    UINT32 PBM_PL2                                 :  1;  // Bits 11:11
    UINT32 SPARE_CLR_12                            :  1;  // Bits 12:12
    UINT32 SPARE_CLR_13                            :  1;  // Bits 13:13
    UINT32 SPARE_CLR_14                            :  1;  // Bits 14:14
    UINT32 SPARE_CLR_15                            :  1;  // Bits 15:15
    UINT32 PROCHOT_LOG                             :  1;  // Bits 16:16
    UINT32 THERMAL_LOG                             :  1;  // Bits 17:17
    UINT32 SPARE_CLR_2_LOG                         :  1;  // Bits 18:18
    UINT32 SPARE_CLR_3_LOG                         :  1;  // Bits 19:19
    UINT32 RSR_LOG                                 :  1;  // Bits 20:20
    UINT32 RATL_LOG                                :  1;  // Bits 21:21
    UINT32 VR_THERMALERT_LOG                       :  1;  // Bits 22:22
    UINT32 VR_TDC_LOG                              :  1;  // Bits 23:23
    UINT32 OTHER_LOG                               :  1;  // Bits 24:24
    UINT32 SPARE_CLR_9_LOG                         :  1;  // Bits 25:25
    UINT32 PBM_PL1_LOG                             :  1;  // Bits 26:26
    UINT32 PBM_PL2_LOG                             :  1;  // Bits 27:27
    UINT32 SPARE_CLR_12_LOG                        :  1;  // Bits 28:28
    UINT32 SPARE_CLR_13_LOG                        :  1;  // Bits 29:29
    UINT32 SPARE_CLR_14_LOG                        :  1;  // Bits 30:30
    UINT32 SPARE_CLR_15_LOG                        :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CLR_PERF_LIMIT_REASONS_PCU_STRUCT;
typedef union {
  struct {
    UINT32 LAST_DE_WP_REQ_SERVED                   :  2;  // Bits 1:0
    UINT32 QCLK_RATIO                              :  8;  // Bits 9:2
    UINT32 GEAR                                    :  2;  // Bits 11:10
    UINT32 IPU_IS_RATIO                            :  8;  // Bits 19:12
    UINT32 IPU_PS_RATIO                            :  8;  // Bits 27:20
    UINT32                                         :  13;  // Bits 40:28
    UINT32 SYSTEM_CACHE_DIV_DENOMINATOR            :  5;  // Bits 45:41
    UINT32 SA_VOLTAGE                              :  16;  // Bits 61:46
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} SA_PERF_STATUS0_PCU_STRUCT;
typedef union {
  struct {
    UINT32 IA_PERF_MULTIPLIER                      :  16;  // Bits 15:0
    UINT32 GT_PERF_MULTIPLIER                      :  16;  // Bits 31:16
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PRIP_TURBO_PLCY_PCU_STRUCT;
typedef union {
  struct {
    UINT32 SECPTP                                  :  5;  // Bits 4:0
    UINT32                                         :  27;  // Bits 31:5
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GFX_SECONDARY_PWR_PLANE_POLICY_PCU_STRUCT;

typedef CAMARILLO_MAILBOX_DATA0_PCU_STRUCT PRIP_NRG_STTS_PCU_STRUCT;

typedef CAMARILLO_MAILBOX_DATA0_PCU_STRUCT SECP_NRG_STTS_PCU_STRUCT;
typedef union {
  struct {
    UINT32 PKG_TDP                                 :  15;  // Bits 14:0
    UINT32                                         :  1;  // Bits 15:15
    UINT32 PKG_MIN_PWR                             :  15;  // Bits 30:16
    UINT32                                         :  1;  // Bits 31:31
    UINT32 PKG_MAX_PWR                             :  15;  // Bits 46:32
    UINT32                                         :  1;  // Bits 47:47
    UINT32 PKG_MAX_WIN                             :  7;  // Bits 54:48
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} PACKAGE_POWER_SKU_PCU_STRUCT;
typedef union {
  struct {
    UINT32 PWR_UNIT                                :  4;  // Bits 3:0
    UINT32                                         :  4;  // Bits 7:4
    UINT32 ENERGY_UNIT                             :  5;  // Bits 12:8
    UINT32                                         :  3;  // Bits 15:13
    UINT32 TIME_UNIT                               :  4;  // Bits 19:16
    UINT32                                         :  12;  // Bits 31:20
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PACKAGE_POWER_SKU_UNIT_PCU_STRUCT;

typedef CAMARILLO_MAILBOX_DATA0_PCU_STRUCT PACKAGE_ENERGY_STATUS_PCU_STRUCT;
typedef union {
  struct {
    UINT32 VPU_RATIO                               :  8;  // Bits 7:0
    UINT32 NOC_RATIO                               :  8;  // Bits 15:8
    UINT32 MEDIA_RATIO                             :  8;  // Bits 23:16
    UINT32 CD2D_RATIO                              :  8;  // Bits 31:24
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} SA_PERF_STATUS1_PCU_STRUCT;
typedef union {
  struct {
    UINT32 SLICES_VOLTAGE                          :  8;  // Bits 7:0
    UINT32                                         :  3;  // Bits 10:8
    UINT32 UNSLICE_RATIO                           :  9;  // Bits 19:11
    UINT32 SLICES_RATIO                            :  9;  // Bits 28:20
    UINT32                                         :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GT_PERF_STATUS_PCU_STRUCT;
typedef union {
  struct {
    UINT32                                      : 32;  // Bits 49:0
    UINT32                                         :  18;  // Bits 49:0
    UINT32 PLATFORMID                              :  3;  // Bits 52:50
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} PLATFORM_ID_PCU_STRUCT;
typedef union {
  struct {
    UINT32                                         :  8;  // Bits 7:0
    UINT32 MAX_NON_TURBO_LIM_RATIO                 :  8;  // Bits 15:8
    UINT32 SMM_SAVE_CAP                            :  1;  // Bits 16:16
    UINT32                                         :  6;  // Bits 22:17
    UINT32 PPIN_CAP                                :  1;  // Bits 23:23
    UINT32 OCVOLT_OVRD_AVAIL                       :  1;  // Bits 24:24
    UINT32                                         :  1;  // Bits 25:25
    UINT32 DCU_16K_MODE_AVAIL                      :  1;  // Bits 26:26
    UINT32 SAMPLE_PART                             :  1;  // Bits 27:27
    UINT32 PRG_TURBO_RATIO_EN                      :  1;  // Bits 28:28
    UINT32 PRG_TDP_LIM_EN                          :  1;  // Bits 29:29
    UINT32 PRG_TJ_OFFSET_EN                        :  1;  // Bits 30:30
    UINT32 CPUID_FAULTING_EN                       :  1;  // Bits 31:31
    UINT32 LPM_SUPPORT                             :  1;  // Bits 32:32
    UINT32 CONFIG_TDP_LEVELS                       :  2;  // Bits 34:33
    UINT32 PFAT_ENABLE                             :  1;  // Bits 35:35
    UINT32 PEG2DMIDIS_EN                           :  1;  // Bits 36:36
    UINT32 TIMED_MWAIT_ENABLE                      :  1;  // Bits 37:37
    UINT32                                         :  2;  // Bits 39:38
    UINT32 MAX_EFFICIENCY_RATIO                    :  8;  // Bits 47:40
    UINT32 MIN_OPERATING_RATIO                     :  8;  // Bits 55:48
    UINT32 PushPatch_EN                            :  1;  // Bits 56:56
    UINT32 EDRAM_ENABLE                            :  1;  // Bits 57:57
    UINT32 SXP_2LM_EN                              :  1;  // Bits 58:58
    UINT32 SMM_SUPOVR_STATE_LOCK_ENABLE            :  1;  // Bits 59:59
    UINT32 TIO_ENABLE                              :  1;  // Bits 60:60
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} PLATFORM_INFO_PCU_STRUCT;

typedef CAMARILLO_MAILBOX_DATA0_PCU_STRUCT PP1_C0_CORE_CLOCK_PCU_STRUCT;

typedef CAMARILLO_MAILBOX_DATA0_PCU_STRUCT PP0_EFFICIENT_CYCLES_PCU_STRUCT;
typedef union {
  struct {
    UINT32 DATA                                    :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PP0_THREAD_ACTIVITY_PCU_STRUCT;

typedef CAMARILLO_MAILBOX_DATA0_PCU_STRUCT PP1_EFFICIENT_CYCLES_PCU_STRUCT;
typedef union {
  struct {
    UINT32 DATA                                    :  8;  // Bits 7:0
    UINT32                                         :  24;  // Bits 31:8
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PACKAGE_TEMPERATURE_PCU_STRUCT;

typedef PACKAGE_TEMPERATURE_PCU_STRUCT PP0_TEMPERATURE_PCU_STRUCT;

typedef PACKAGE_TEMPERATURE_PCU_STRUCT PP1_TEMPERATURE_PCU_STRUCT;
typedef union {
  struct {
    UINT32 TIME_VAL                                :  32;  // Bits 31:0
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PCU_REFERENCE_CLOCK_PCU_STRUCT;
typedef union {
  struct {
    UINT32 RESERVED                                :  1;  // Bits 0:0
    UINT32 VALID                                   :  1;  // Bits 1:1
    UINT32 RESERVED_BITS                           :  4;  // Bits 5:2
    UINT32 OD                                      :  1;  // Bits 6:6
    UINT32 IM                                      :  1;  // Bits 7:7
    UINT32 NEXT_DEVICE_ACTIVITY                    :  21;  // Bits 28:8
    UINT32                                         :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DEVICE_SHARED_IDLE_DURATION_PCU_STRUCT;
typedef union {
  struct {
    UINT32 USED                                    :  1;  // Bits 0:0
    UINT32 VALID                                   :  1;  // Bits 1:1
    UINT32 RESERVED_BITS                           :  4;  // Bits 5:2
    UINT32 OD                                      :  1;  // Bits 6:6
    UINT32 IM                                      :  1;  // Bits 7:7
    UINT32 NEXT_DEVICE_ACTIVITY                    :  21;  // Bits 28:8
    UINT32                                         :  3;  // Bits 31:29
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DEVICE_DEDICATED_IDLE_DURATION_PCU_STRUCT;
typedef union {
  struct {
    UINT32 RPSTT_LIM                               :  8;  // Bits 7:0
    UINT32                                         :  24;  // Bits 31:8
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RP_STATE_LIMITS_PCU_STRUCT;
typedef union {
  struct {
    UINT32 RP0_CAP                                 :  9;  // Bits 8:0
    UINT32 RESERVED                                :  7;  // Bits 15:9
    UINT32 RPN_CAP                                 :  9;  // Bits 24:16
    UINT32                                         :  7;  // Bits 31:25
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} GT_PSTATE_CAP_PCU_STRUCT;
typedef union {
  struct {
    UINT32 TCC_OFFSET_TIME_WINDOW                  :  7;  // Bits 6:0
    UINT32 TCC_OFFSET_CLAMPING_BIT                 :  1;  // Bits 7:7
    UINT32 FAN_TEMP_TARGET_OFST                    :  8;  // Bits 15:8
    UINT32 REF_TEMP                                :  8;  // Bits 23:16
    UINT32 TJ_MAX_TCC_OFFSET                       :  7;  // Bits 30:24
    UINT32 LOCKED                                  :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} TEMPERATURE_TARGET_PCU_STRUCT;
typedef union {
  struct {
    UINT32 PKG_PWR_LIM_1                           :  15;  // Bits 14:0
    UINT32 PKG_PWR_LIM_1_EN                        :  1;  // Bits 15:15
    UINT32 PKG_CLMP_LIM_1                          :  1;  // Bits 16:16
    UINT32 PKG_PWR_LIM_1_TIME                      :  7;  // Bits 23:17
    UINT32                                         :  8;  // Bits 31:24
    UINT32 PKG_PWR_LIM_2                           :  15;  // Bits 46:32
    UINT32 PKG_PWR_LIM_2_EN                        :  1;  // Bits 47:47
    UINT32 PKG_CLMP_LIM_2                          :  1;  // Bits 48:48
    UINT32 PKG_PWR_LIM_2_TIME                      :  7;  // Bits 55:49
    UINT32                                         :  7;  // Bits 62:56
    UINT32 PKG_PWR_LIM_LOCK                        :  1;  // Bits 63:63
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} PACKAGE_RAPL_LIMIT_PCU_STRUCT;
typedef union {
  struct {
    UINT32 IA_PP_PWR_LIM                           :  15;  // Bits 14:0
    UINT32 PWR_LIM_CTRL_EN                         :  1;  // Bits 15:15
    UINT32 PP_CLAMP_LIM                            :  1;  // Bits 16:16
    UINT32 CTRL_TIME_WIN                           :  7;  // Bits 23:17
    UINT32                                         :  7;  // Bits 30:24
    UINT32 PP_PWR_LIM_LOCK                         :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PRIP_TURBO_PWR_LIM_PCU_STRUCT;
typedef union {
  struct {
    UINT32 NON_IA_PP_PWR_LIM                       :  15;  // Bits 14:0
    UINT32 PWR_LIM_CTRL_EN                         :  1;  // Bits 15:15
    UINT32 PP_CLAMP_LIM                            :  1;  // Bits 16:16
    UINT32 CTRL_TIME_WIN                           :  7;  // Bits 23:17
    UINT32                                         :  7;  // Bits 30:24
    UINT32 SP_PWR_LIM_LOCK                         :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} SECP_TURBO_PWR_LIM_PCU_STRUCT;
typedef union {
  struct {
    UINT32 CURRENT_LIMIT                           :  16;  // Bits 15:0
    UINT32 PL4_LEVEL                               :  13;  // Bits 28:16
    UINT32                                         :  2;  // Bits 30:29
    UINT32 LOCK                                    :  1;  // Bits 31:31
    UINT32 PSI1_THRESHOLD                          :  10;  // Bits 41:32
    UINT32 PSI2_THRESHOLD                          :  10;  // Bits 51:42
    UINT32 PSI3_THRESHOLD                          :  10;  // Bits 61:52
    UINT32 PS4_ENABLE                              :  1;  // Bits 62:62
    UINT32 RESERVED                                :  1;  // Bits 63:63
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} VR_CURRENT_CONFIG_PCU_STRUCT;
typedef union {
  struct {
    UINT32 THERMAL_MONITOR_STATUS                  :  1;  // Bits 0:0
    UINT32 THERMAL_MONITOR_LOG                     :  1;  // Bits 1:1
    UINT32 PROCHOT_STATUS                          :  1;  // Bits 2:2
    UINT32 PROCHOT_LOG                             :  1;  // Bits 3:3
    UINT32 OUT_OF_SPEC_STATUS                      :  1;  // Bits 4:4
    UINT32 OUT_OF_SPEC_LOG                         :  1;  // Bits 5:5
    UINT32 THRESHOLD1_STATUS                       :  1;  // Bits 6:6
    UINT32 THRESHOLD1_LOG                          :  1;  // Bits 7:7
    UINT32 THRESHOLD2_STATUS                       :  1;  // Bits 8:8
    UINT32 THRESHOLD2_LOG                          :  1;  // Bits 9:9
    UINT32 POWER_LIMITATION_STATUS                 :  1;  // Bits 10:10
    UINT32 POWER_LIMITATION_LOG                    :  1;  // Bits 11:11
    UINT32 CURRENT_LIMIT_STATUS                    :  1;  // Bits 12:12
    UINT32 CURRENT_LIMIT_LOG                       :  1;  // Bits 13:13
    UINT32 CROSS_DOMAIN_LIMIT_STATUS               :  1;  // Bits 14:14
    UINT32 CROSS_DOMAIN_LIMIT_LOG                  :  1;  // Bits 15:15
    UINT32 TEMPERATURE                             :  8;  // Bits 23:16
    UINT32                                         :  3;  // Bits 26:24
    UINT32 RESOLUTION                              :  4;  // Bits 30:27
    UINT32 VALID                                   :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} THERM_STATUS_GT_PCU_STRUCT;
typedef union {
  struct {
    UINT32 HIGH_TEMP_INT_ENABLE                    :  1;  // Bits 0:0
    UINT32 LOW_TEMP_INT_ENABLE                     :  1;  // Bits 1:1
    UINT32 PROCHOT_INT_ENABLE                      :  1;  // Bits 2:2
    UINT32                                         :  1;  // Bits 3:3
    UINT32 OUT_OF_SPEC_INT_ENABLE                  :  1;  // Bits 4:4
    UINT32                                         :  3;  // Bits 7:5
    UINT32 THRESHOLD_1_REL_TEMP                    :  7;  // Bits 14:8
    UINT32 THRESHOLD_1_INT_ENABLE                  :  1;  // Bits 15:15
    UINT32 THRESHOLD_2_REL_TEMP                    :  7;  // Bits 22:16
    UINT32 THRESHOLD_2_INT_ENABLE                  :  1;  // Bits 23:23
    UINT32 POWER_INT_ENABLE                        :  1;  // Bits 24:24
    UINT32                                         :  7;  // Bits 31:25
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} THERM_INTERRUPT_GT_PCU_STRUCT;
typedef union {
  struct {
    UINT32 RESERVED                                :  1;  // Bits 0:0
    UINT32 VALID                                   :  1;  // Bits 1:1
    UINT32 RESERVED_BITS                           :  4;  // Bits 5:2
    UINT32 OD                                      :  1;  // Bits 6:6
    UINT32 IM                                      :  1;  // Bits 7:7
    UINT32 NEXT_DEVICE_ACTIVITY                    :  21;  // Bits 28:8
    UINT32 DISABLE_MDID_EVALUATION                 :  1;  // Bits 29:29
    UINT32 FORCE_MDID_OVERRIDE                     :  1;  // Bits 30:30
    UINT32                                         :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DEVICE_IDLE_DURATION_OVERRIDE_PCU_STRUCT;
typedef union {
  struct {
    UINT32                                         :  4;  // Bits 3:0
    UINT32 PECI_CMD                                :  8;  // Bits 11:4
    UINT32                                         :  20;  // Bits 31:12
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CHAP_CONFIG_PCU_STRUCT;
typedef union {
  struct {
    UINT32 FREQ_SSC_SPRD                           :  5;  // Bits 4:0
    UINT32 FREQ_SSC_MODE                           :  1;  // Bits 5:5
    UINT32 FREQ_SSC_SPREAD_SPEC_CONTROL_LOCK       :  1;  // Bits 6:6
    UINT32 FREQ_HOPPING_ENABLE                     :  1;  // Bits 7:7
    UINT32 RFI_FREQ_SELECT                         :  2;  // Bits 9:8
    UINT32 RESERVED                                :  22;  // Bits 31:10
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RFI_EMI_CONTROL_PCU_STRUCT;
typedef union {
  struct {
    UINT32 RFI_FREQ                                :  2;  // Bits 1:0
    UINT32 RESERVED_22_2                           :  21;  // Bits 22:2
    UINT32 RFI_PLL_FREQ_CHANGE_BUSY_BIT            :  1;  // Bits 23:23
    UINT32 PHASE_SSC_ACTIVE                        :  1;  // Bits 24:24
    UINT32 FREQ_SSC_SPRD                           :  5;  // Bits 29:25
    UINT32 REVISION                                :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} RFI_STATUS_PCU_STRUCT;

typedef PKG_IA_C0_ANY_SUM_PCU_STRUCT XTAL_CLOCK_COUNT_PCU_STRUCT;
typedef union {
  struct {
    UINT32 POINT_0                                 :  10;  // Bits 9:0
    UINT32 POINT_1                                 :  10;  // Bits 19:10
    UINT32 POINT_2                                 :  10;  // Bits 29:20
    UINT32 POINT_3Low                              :  2;  // Bits 31:30
    UINT32 POINT_3High                             :  8;  // Bits 39:32
    UINT32 RESERVED                                :  24;  // Bits 63:40
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} DDR_DVFS_DATA_RATE_PCU_STRUCT;
typedef union {
  struct {
    UINT32 DATA_RATE_BASE                          :  16;  // Bits 15:0
    UINT32 DATA_RATE_DELTA_ABOVE_BASE              :  8;  // Bits 23:16
    UINT32 ERR_CODE                                :  7;  // Bits 30:24
    UINT32 RUN_BUSY                                :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDR_DVFS_RFI_RESTRICTION_PCU_STRUCT;
typedef union {
  struct {
    UINT32 RFI_DISABLE                             :  1;  // Bits 0:0
    UINT32 RESERVED                                :  31;  // Bits 31:1
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDR_DVFS_RFI_CONFIG_PCU_STRUCT;
typedef union {
  struct {
    UINT32 DEBUG_ENERGY_PP0_VALUE                  :  10;  // Bits 9:0
    UINT32 DEBUG_ENERGY_PP1_VALUE                  :  10;  // Bits 19:10
    UINT32 DEBUG_ENERGY_SA_VALUE                   :  10;  // Bits 29:20
    UINT32                                         :  2;  // Bits 31:30
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} ENERGY_DEBUG_PCU_STRUCT;
typedef union {
  struct {
    UINT32 SKPDLow                                 :  32;  // Bits 31:0
    UINT32 SKPDHigh                                :  32;  // Bits 63:32
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} SSKPD_PCU_STRUCT;

typedef QCLK_PERF_LIMIT_REASONS_PCU_STRUCT VPU_PERF_LIMIT_REASONS_PCU_STRUCT;

typedef CAMARILLO_MAILBOX_DATA0_PCU_STRUCT BIOS_MAILBOX_DATA_PCU_STRUCT;
typedef union {
  struct {
    UINT32 COMMAND                                 :  8;  // Bits 7:0
    UINT32 PARAM1                                  :  8;  // Bits 15:8
    UINT32 PARAM2                                  :  13;  // Bits 28:16
    UINT32                                         :  2;  // Bits 30:29
    UINT32 RUN_BUSY                                :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} BIOS_MAILBOX_INTERFACE_PCU_STRUCT;
typedef union {
  struct {
    UINT32 RST_CPL                                 :  1;  // Bits 0:0
    UINT32 PCIE_ENUMERATION_DONE                   :  1;  // Bits 1:1
    UINT32 C7_ALLOWED                              :  1;  // Bits 2:2
    UINT32                                         :  29;  // Bits 31:3
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} BIOS_RESET_CPL_PCU_STRUCT;
typedef union {
  struct {
    UINT32 TARGET_TEMPERATURE                      :  8;  // Bits 7:0
    UINT32 ENABLE                                  :  1;  // Bits 8:8
    UINT32                                         :  23;  // Bits 31:9
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DDR_THERM_CONTROL_PCU_STRUCT;
typedef union {
  struct {
    UINT32 UMCNF_CATERR_DIS                        :  1;  // Bits 0:0
    UINT32                                         :  31;  // Bits 31:1
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FUSA_CONFIG_PCU_STRUCT;
typedef union {
  struct {
    UINT32 COMPLETION_COUNTER                      :  24;  // Bits 23:0
    UINT32 CRC8                                    :  8;  // Bits 31:24
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FUSA_TASK_COMPLETION_COUNTER_PCU_STRUCT;
typedef union {
  struct {
    UINT32 TDP_RATIO                               :  8;  // Bits 7:0
    UINT32                                         :  24;  // Bits 31:8
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CONFIG_TDP_NOMINAL_PCU_STRUCT;
typedef union {
  struct {
    UINT32 PKG_TDP                                 :  15;  // Bits 14:0
    UINT32                                         :  1;  // Bits 15:15
    UINT32 TDP_RATIO                               :  8;  // Bits 23:16
    UINT32                                         :  8;  // Bits 31:24
    UINT32 PKG_MAX_PWR                             :  15;  // Bits 46:32
    UINT32                                         :  1;  // Bits 47:47
    UINT32 PKG_MIN_PWR                             :  15;  // Bits 62:48
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} CONFIG_TDP_LEVEL1_PCU_STRUCT;

typedef CONFIG_TDP_LEVEL1_PCU_STRUCT CONFIG_TDP_LEVEL2_PCU_STRUCT;
typedef union {
  struct {
    UINT32 TDP_LEVEL                               :  2;  // Bits 1:0
    UINT32                                         :  29;  // Bits 30:2
    UINT32 CONFIG_TDP_LOCK                         :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} CONFIG_TDP_CONTROL_PCU_STRUCT;
typedef union {
  struct {
    UINT32 MAX_NON_TURBO_RATIO                     :  8;  // Bits 7:0
    UINT32                                         :  23;  // Bits 30:8
    UINT32 TURBO_ACTIVATION_RATIO_LOCK             :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} TURBO_ACTIVATION_RATIO_PCU_STRUCT;
typedef union {
  struct {
    UINT32 MC_TIMING_RUNTIME_OC_ENABLED            :  1;  // Bits 0:0
    UINT32 RSVD_0                                  :  1;  // Bits 1:1
    UINT32 RSVD_1                                  :  1;  // Bits 2:2
    UINT32 RSVD_2                                  :  1;  // Bits 3:3
    UINT32                                         :  28;  // Bits 31:4
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} OC_STATUS_PCU_STRUCT;
typedef union {
  struct {
    UINT32 VERBOSITY_CONTROL                       :  2;  // Bits 1:0
    UINT32 RESERVED                                :  30;  // Bits 31:2
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} TELEMETRY_MISC_CONTROL_PCU_STRUCT;
typedef union {
  struct {
    UINT32 FLOW_ENABLE_0                           :  1;  // Bits 0:0
    UINT32 FLOW_ENABLE_1                           :  1;  // Bits 1:1
    UINT32 FLOW_ENABLE_2                           :  1;  // Bits 2:2
    UINT32 FLOW_ENABLE_3                           :  1;  // Bits 3:3
    UINT32 FLOW_ENABLE_4                           :  1;  // Bits 4:4
    UINT32 FLOW_ENABLE_5                           :  1;  // Bits 5:5
    UINT32 FLOW_ENABLE_6                           :  1;  // Bits 6:6
    UINT32 FLOW_ENABLE_7                           :  1;  // Bits 7:7
    UINT32 FLOW_ENABLE_8                           :  1;  // Bits 8:8
    UINT32 FLOW_ENABLE_9                           :  1;  // Bits 9:9
    UINT32 FLOW_ENABLE_10                          :  1;  // Bits 10:10
    UINT32 FLOW_ENABLE_11                          :  1;  // Bits 11:11
    UINT32 FLOW_ENABLE_12                          :  1;  // Bits 12:12
    UINT32 FLOW_ENABLE_13                          :  1;  // Bits 13:13
    UINT32 FLOW_ENABLE_14                          :  1;  // Bits 14:14
    UINT32 FLOW_ENABLE_15                          :  1;  // Bits 15:15
    UINT32 FLOW_ENABLE_16                          :  1;  // Bits 16:16
    UINT32 FLOW_ENABLE_17                          :  1;  // Bits 17:17
    UINT32 FLOW_ENABLE_18                          :  1;  // Bits 18:18
    UINT32 FLOW_ENABLE_19                          :  1;  // Bits 19:19
    UINT32 FLOW_ENABLE_20                          :  1;  // Bits 20:20
    UINT32 FLOW_ENABLE_21                          :  1;  // Bits 21:21
    UINT32 FLOW_ENABLE_22                          :  1;  // Bits 22:22
    UINT32 FLOW_ENABLE_23                          :  1;  // Bits 23:23
    UINT32 FLOW_ENABLE_24                          :  1;  // Bits 24:24
    UINT32 FLOW_ENABLE_25                          :  1;  // Bits 25:25
    UINT32 FLOW_ENABLE_26                          :  1;  // Bits 26:26
    UINT32 FLOW_ENABLE_27                          :  1;  // Bits 27:27
    UINT32 FLOW_ENABLE_28                          :  1;  // Bits 28:28
    UINT32 FLOW_ENABLE_29                          :  1;  // Bits 29:29
    UINT32 FLOW_ENABLE_30                          :  1;  // Bits 30:30
    UINT32 FLOW_ENABLE_31                          :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} TELEMETRY_FLOW_ENABLES_PCU_STRUCT;

#pragma pack(pop)
#endif
