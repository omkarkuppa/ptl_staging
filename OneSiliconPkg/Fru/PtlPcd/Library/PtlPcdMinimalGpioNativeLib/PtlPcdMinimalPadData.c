/** @file
  This file provides minimal GPIO Pad details for PTL PCD.

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
#include "PtlPcdMinimalGpio.h"


GLOBAL_REMOVE_IF_UNREFERENCED GPIOV2_MIN_NATIVE_SIGNAL_DATA mPtlPcdMinNativeSignals[] = {
  { { .Value = GPIOV2_SIGNAL_IOM_GPPA(0) }, GPIOV2_PTL_PCD_XXGPP_A_0, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x00 },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPA(1) }, GPIOV2_PTL_PCD_XXGPP_A_1, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x01 },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPA(2) }, GPIOV2_PTL_PCD_XXGPP_A_2, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x02 },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPA(3) }, GPIOV2_PTL_PCD_XXGPP_A_3, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x03 },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPA(4) }, GPIOV2_PTL_PCD_XXGPP_A_4, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x04 },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPA(5) }, GPIOV2_PTL_PCD_XXGPP_A_5, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x05 },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPA(6) }, GPIOV2_PTL_PCD_XXGPP_A_6, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x06 },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPA(7) }, GPIOV2_PTL_PCD_XXGPP_A_7, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x07 },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPA(8) }, GPIOV2_PTL_PCD_XXGPP_A_8, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x00 },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPA(9) }, GPIOV2_PTL_PCD_XXGPP_A_9, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x01 },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPA(10) }, GPIOV2_PTL_PCD_XXGPP_A_10, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x02 },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPA(11) }, GPIOV2_PTL_PCD_XXGPP_A_11, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x03 },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPA(12) }, GPIOV2_PTL_PCD_XXGPP_A_12, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x04 },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPA(13) }, GPIOV2_PTL_PCD_XXGPP_A_13, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x05 },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPA(14) }, GPIOV2_PTL_PCD_XXGPP_A_14, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x06 },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPA(15) }, GPIOV2_PTL_PCD_XXGPP_A_15, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x07 },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPA(16) }, GPIOV2_PTL_PCD_XXGPP_A_16, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x00  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPA(17) }, GPIOV2_PTL_PCD_XXGPP_A_17, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x01  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(0) },  GPIOV2_PTL_PCD_XXGPP_B_0,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x03  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(1) },  GPIOV2_PTL_PCD_XXGPP_B_1,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x04  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(2) },  GPIOV2_PTL_PCD_XXGPP_B_2,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x05  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(3) },  GPIOV2_PTL_PCD_XXGPP_B_3,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x06  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(4) },  GPIOV2_PTL_PCD_XXGPP_B_4,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x07  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(5) },  GPIOV2_PTL_PCD_XXGPP_B_5,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x00  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(6) },  GPIOV2_PTL_PCD_XXGPP_B_6,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x01  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(7) },  GPIOV2_PTL_PCD_XXGPP_B_7,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x02  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(8) },  GPIOV2_PTL_PCD_XXGPP_B_8,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x03  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(9) },  GPIOV2_PTL_PCD_XXGPP_B_9,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x04  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(10) }, GPIOV2_PTL_PCD_XXGPP_B_10, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x05  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(11) }, GPIOV2_PTL_PCD_XXGPP_B_11, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x06  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(12) }, GPIOV2_PTL_PCD_XXGPP_B_12, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x07  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(13) }, GPIOV2_PTL_PCD_XXGPP_B_13, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x00  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(14) }, GPIOV2_PTL_PCD_XXGPP_B_14, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x01  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(15) }, GPIOV2_PTL_PCD_XXGPP_B_15, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x02  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(16) }, GPIOV2_PTL_PCD_XXGPP_B_16, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x03  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(17) }, GPIOV2_PTL_PCD_XXGPP_B_17, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x04  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(18) }, GPIOV2_PTL_PCD_XXGPP_B_18, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x05  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(19) }, GPIOV2_PTL_PCD_XXGPP_B_19, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x06  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(20) }, GPIOV2_PTL_PCD_XXGPP_B_20, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x07  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(21) }, GPIOV2_PTL_PCD_XXGPP_B_21, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x00  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(22) }, GPIOV2_PTL_PCD_XXGPP_B_22, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x01  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(23) }, GPIOV2_PTL_PCD_XXGPP_B_23, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x02  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(24) }, GPIOV2_PTL_PCD_XXGPP_B_24, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x13, 0x00  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPB(25) }, GPIOV2_PTL_PCD_XXGPP_B_25, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x13, 0x01  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(0) },  GPIOV2_PTL_PCD_XXGPP_C_0,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x00  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(1) },  GPIOV2_PTL_PCD_XXGPP_C_1,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x01  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(2) },  GPIOV2_PTL_PCD_XXGPP_C_2,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x02  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(3) },  GPIOV2_PTL_PCD_XXGPP_C_3,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x03  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(4) },  GPIOV2_PTL_PCD_XXGPP_C_4,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x04  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(5) },  GPIOV2_PTL_PCD_XXGPP_C_5,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x05  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(6) },  GPIOV2_PTL_PCD_XXGPP_C_6,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x06  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(7) },  GPIOV2_PTL_PCD_XXGPP_C_7,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x07  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(8) },  GPIOV2_PTL_PCD_XXGPP_C_8,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x00  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(9) },  GPIOV2_PTL_PCD_XXGPP_C_9,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x01  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(10) }, GPIOV2_PTL_PCD_XXGPP_C_10,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x02  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(11) }, GPIOV2_PTL_PCD_XXGPP_C_11,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x03  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(12) }, GPIOV2_PTL_PCD_XXGPP_C_12,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x04  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(13) }, GPIOV2_PTL_PCD_XXGPP_C_13,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x05  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(14) }, GPIOV2_PTL_PCD_XXGPP_C_14,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x06  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(15) }, GPIOV2_PTL_PCD_XXGPP_C_15,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x07  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(16) }, GPIOV2_PTL_PCD_XXGPP_C_16,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x00  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(17) }, GPIOV2_PTL_PCD_XXGPP_C_17,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x01  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(18) }, GPIOV2_PTL_PCD_XXGPP_C_18,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x02  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(19) }, GPIOV2_PTL_PCD_XXGPP_C_19,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x03  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(20) }, GPIOV2_PTL_PCD_XXGPP_C_20,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x04  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(21) }, GPIOV2_PTL_PCD_XXGPP_C_21,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x05  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(22) }, GPIOV2_PTL_PCD_XXGPP_C_22,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x06  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPC(23) }, GPIOV2_PTL_PCD_XXGPP_C_23,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x07  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(0) },  GPIOV2_PTL_PCD_XXGPP_D_0,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x00  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(1) },  GPIOV2_PTL_PCD_XXGPP_D_1,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x01  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(2) },  GPIOV2_PTL_PCD_XXGPP_D_2,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x02  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(3) },  GPIOV2_PTL_PCD_XXGPP_D_3,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x03  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(4) },  GPIOV2_PTL_PCD_XXGPP_D_4,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x04  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(5) },  GPIOV2_PTL_PCD_XXGPP_D_5,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x05  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(6) },  GPIOV2_PTL_PCD_XXGPP_D_6,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x06  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(7) },  GPIOV2_PTL_PCD_XXGPP_D_7,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x07  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(8) },  GPIOV2_PTL_PCD_XXGPP_D_8,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x15, 0x00  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(9) },  GPIOV2_PTL_PCD_XXGPP_D_9,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x15, 0x01  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(10) }, GPIOV2_PTL_PCD_XXGPP_D_10, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x15, 0x02  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(11) }, GPIOV2_PTL_PCD_XXGPP_D_11, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x15, 0x03  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(12) }, GPIOV2_PTL_PCD_XXGPP_D_12, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x15, 0x04  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(13) }, GPIOV2_PTL_PCD_XXGPP_D_13, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x15, 0x05  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(14) }, GPIOV2_PTL_PCD_XXGPP_D_14, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x15, 0x06  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(15) }, GPIOV2_PTL_PCD_XXGPP_D_15, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x15, 0x07  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(16) }, GPIOV2_PTL_PCD_XXGPP_D_16, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x16, 0x00  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(17) }, GPIOV2_PTL_PCD_XXGPP_D_17, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x16, 0x01  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(18) }, GPIOV2_PTL_PCD_XXGPP_D_18, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x16, 0x02  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(19) }, GPIOV2_PTL_PCD_XXGPP_D_19, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x16, 0x03  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(20) }, GPIOV2_PTL_PCD_XXGPP_D_20, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x16, 0x04  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(21) }, GPIOV2_PTL_PCD_XXGPP_D_21, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x16, 0x05  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(22) }, GPIOV2_PTL_PCD_XXGPP_D_22, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x16, 0x06  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(23) }, GPIOV2_PTL_PCD_XXGPP_D_23, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x16, 0x07  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(24) }, GPIOV2_PTL_PCD_XXGPP_D_24, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x17, 0x00  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPD(25) }, GPIOV2_PTL_PCD_XXGPP_D_25, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x17, 0x01  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPE(0) },  GPIOV2_PTL_PCD_XXGPP_E_0,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x13, 0x00  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPE(1) },  GPIOV2_PTL_PCD_XXGPP_E_1,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x13, 0x01  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPE(2) },  GPIOV2_PTL_PCD_XXGPP_E_2,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x13, 0x02  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPE(3) },  GPIOV2_PTL_PCD_XXGPP_E_3,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x13, 0x03  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPE(4) },  GPIOV2_PTL_PCD_XXGPP_E_4,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x13, 0x04  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPE(5) },  GPIOV2_PTL_PCD_XXGPP_E_5,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x13, 0x05  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPE(6) },  GPIOV2_PTL_PCD_XXGPP_E_6,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x13, 0x06  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPE(7) },  GPIOV2_PTL_PCD_XXGPP_E_7,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x13, 0x07  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPE(8) },  GPIOV2_PTL_PCD_XXGPP_E_8,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x00  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPE(9) },  GPIOV2_PTL_PCD_XXGPP_E_9,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x01  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPE(10) }, GPIOV2_PTL_PCD_XXGPP_E_10, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x02  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPE(11) }, GPIOV2_PTL_PCD_XXGPP_E_11, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x03  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPE(12) }, GPIOV2_PTL_PCD_XXGPP_E_12, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x04  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPE(13) }, GPIOV2_PTL_PCD_XXGPP_E_13, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x05  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPE(14) }, GPIOV2_PTL_PCD_XXGPP_E_14, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x06  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPE(15) }, GPIOV2_PTL_PCD_XXGPP_E_15, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x07  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPE(16) }, GPIOV2_PTL_PCD_XXGPP_E_16, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x15, 0x00  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPE(17) }, GPIOV2_PTL_PCD_XXGPP_E_17, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x15, 0x01  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPE(18) }, GPIOV2_PTL_PCD_XXGPP_E_18, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x15, 0x02  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPE(19) }, GPIOV2_PTL_PCD_XXGPP_E_19, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x15, 0x03  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPE(20) }, GPIOV2_PTL_PCD_XXGPP_E_20, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x15, 0x04  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPE(21) }, GPIOV2_PTL_PCD_XXGPP_E_21, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x15, 0x05  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPE(22) }, GPIOV2_PTL_PCD_XXGPP_E_22, GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x15, 0x06  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(0) },  GPIOV2_PTL_PCD_XXGPP_F_0,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x00  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(1) },  GPIOV2_PTL_PCD_XXGPP_F_1,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x01  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(2) },  GPIOV2_PTL_PCD_XXGPP_F_2,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x02  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(3) },  GPIOV2_PTL_PCD_XXGPP_F_3,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x03  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(4) },  GPIOV2_PTL_PCD_XXGPP_F_4,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x04  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(5) },  GPIOV2_PTL_PCD_XXGPP_F_5,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x05  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(6) },  GPIOV2_PTL_PCD_XXGPP_F_6,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x06  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(7) },  GPIOV2_PTL_PCD_XXGPP_F_7,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x10, 0x07  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(8) },  GPIOV2_PTL_PCD_XXGPP_F_8,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x00  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(9) },  GPIOV2_PTL_PCD_XXGPP_F_9,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x01  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(10) }, GPIOV2_PTL_PCD_XXGPP_F_10,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x02  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(11) }, GPIOV2_PTL_PCD_XXGPP_F_11,  GpioV2PadModeNative6, GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x03  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(12) }, GPIOV2_PTL_PCD_XXGPP_F_12,  GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x04  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(13) }, GPIOV2_PTL_PCD_XXGPP_F_13,  GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x05  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(14) }, GPIOV2_PTL_PCD_XXGPP_F_14,  GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x06  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(15) }, GPIOV2_PTL_PCD_XXGPP_F_15,  GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x11, 0x07  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(16) }, GPIOV2_PTL_PCD_XXGPP_F_16,  GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x00  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(17) }, GPIOV2_PTL_PCD_XXGPP_F_17,  GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x01  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(18) }, GPIOV2_PTL_PCD_XXGPP_F_18,  GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x02  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(19) }, GPIOV2_PTL_PCD_XXGPP_F_19,  GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x03  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(20) }, GPIOV2_PTL_PCD_XXGPP_F_20,  GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x04  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(21) }, GPIOV2_PTL_PCD_XXGPP_F_21,  GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x05  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(22) }, GPIOV2_PTL_PCD_XXGPP_F_22,  GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x06  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPF(23) }, GPIOV2_PTL_PCD_XXGPP_F_23,  GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x12, 0x07  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(0) }, GPIOV2_PTL_PCD_XXGPP_H_0,   GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x13, 0x00  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(1) }, GPIOV2_PTL_PCD_XXGPP_H_1,   GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x13, 0x01  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(2) }, GPIOV2_PTL_PCD_XXGPP_H_2,   GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x13, 0x02  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(3) }, GPIOV2_PTL_PCD_XXGPP_H_3,   GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x13, 0x03  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(4) }, GPIOV2_PTL_PCD_XXGPP_H_4,   GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x13, 0x04  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(5) }, GPIOV2_PTL_PCD_XXGPP_H_5,   GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x13, 0x05  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(6) }, GPIOV2_PTL_PCD_XXGPP_H_6,   GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x13, 0x06  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(7) }, GPIOV2_PTL_PCD_XXGPP_H_7,   GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x13, 0x07  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(8) }, GPIOV2_PTL_PCD_XXGPP_H_8,   GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x00  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(9) }, GPIOV2_PTL_PCD_XXGPP_H_9,   GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x01  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(10) }, GPIOV2_PTL_PCD_XXGPP_H_10, GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x02  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(11) }, GPIOV2_PTL_PCD_XXGPP_H_11, GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x03  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(12) }, GPIOV2_PTL_PCD_XXGPP_H_12, GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x04  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(13) }, GPIOV2_PTL_PCD_XXGPP_H_13, GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x05  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(14) }, GPIOV2_PTL_PCD_XXGPP_H_14, GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x06  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(15) }, GPIOV2_PTL_PCD_XXGPP_H_15, GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x14, 0x07  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(16) }, GPIOV2_PTL_PCD_XXGPP_H_16, GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x15, 0x00  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(17) }, GPIOV2_PTL_PCD_XXGPP_H_17, GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x15, 0x01  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(18) }, GPIOV2_PTL_PCD_XXGPP_H_18, GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x15, 0x02  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(19) }, GPIOV2_PTL_PCD_XXGPP_H_19, GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x15, 0x03  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(20) }, GPIOV2_PTL_PCD_XXGPP_H_20, GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x15, 0x04  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(21) }, GPIOV2_PTL_PCD_XXGPP_H_21, GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x15, 0x05  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(22) }, GPIOV2_PTL_PCD_XXGPP_H_22, GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x15, 0x06  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(23) }, GPIOV2_PTL_PCD_XXGPP_H_23, GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x15, 0x07  },
  { { .Value = GPIOV2_SIGNAL_IOM_GPPH(24) }, GPIOV2_PTL_PCD_XXGPP_H_24, GpioV2PadModeNative6,  GpioV2IosStateDefault, GpioV2IosTermDefault, TRUE , 0x16, 0x00  },
};
/**
  Get the appropriate GPIO native signal data array based on WCL support.

  @param[out] SignalData          Pointer to the signal data array
  @param[out] SignalDataArraySize Size of the signal data array

  @retval EFI_SUCCESS           Signal data retrieved successfully
  @retval EFI_INVALID_PARAMETER Invalid parameters provided

**/
EFI_STATUS
GetPcdMinNativeSignalData (
  OUT GPIOV2_MIN_NATIVE_SIGNAL_DATA **SignalData,
  OUT UINT32                         *SignalDataArraySize
  )
{
  if ((SignalData == NULL) || (SignalDataArraySize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *SignalDataArraySize = ARRAY_SIZE (mPtlPcdMinNativeSignals);
  *SignalData = mPtlPcdMinNativeSignals;
    return EFI_SUCCESS;
}
