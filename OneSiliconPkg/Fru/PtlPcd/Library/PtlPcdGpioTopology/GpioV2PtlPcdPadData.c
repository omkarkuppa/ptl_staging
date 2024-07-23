/** @file
  This file provides GPIO pad data for PTL_PCD PCH.

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

#include <GpioV2Signals.h>
#include <GpioV2ControllerInterface.h>

// Native functions for Pad: xxgpp_v_0
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_v_0_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_BATLOW },        .PadMode = GpioV2PadModeNative1 }  // Function: 1 batlow_b
};

// Native functions for Pad: xxgpp_v_1
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_v_1_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_AC_PRESENT },    .PadMode = GpioV2PadModeNative1 }  // Function: 1 ac_present
};

// Native functions for Pad: xxgpp_v_2
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_v_2_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_SOC_WAKE },      .PadMode = GpioV2PadModeNative1 }  // Function: 1 soc_wake_b
};

// Native functions for Pad: xxgpp_v_3
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_v_3_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_PWRBTN },        .PadMode = GpioV2PadModeNative1 }  // Function: 1 pwrbtn_b
};

// Native functions for Pad: xxgpp_v_4
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_v_4_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_SLP(3) },        .PadMode = GpioV2PadModeNative1 }  // Function: 1 slp_s3_b
};

// Native functions for Pad: xxgpp_v_5
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_v_5_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_SLP(4) },        .PadMode = GpioV2PadModeNative1 }  // Function: 1 slp_s4_b
};

// Native functions for Pad: xxgpp_v_6
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_v_6_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_SLP_A },         .PadMode = GpioV2PadModeNative1 }  // Function: 1 slp_a_b
};

// Native functions for Pad: xxgpp_v_7
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_v_7_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_SUSCLK },        .PadMode = GpioV2PadModeNative1 }  // Function: 1 susclk
};

// Native functions for Pad: xxgpp_v_8
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_v_8_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_SLP_WLAN },      .PadMode = GpioV2PadModeNative1 }  // Function: 1 slp_wlan_b
};

// Native functions for Pad: xxgpp_v_9
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_v_9_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_SLP(5) },        .PadMode = GpioV2PadModeNative1 }  // Function: 1 slp_s5_b
};

// Native functions for Pad: xxgpp_v_10
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_v_10_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_LANPHYPC },      .PadMode = GpioV2PadModeNative1 }  // Function: 1 lanphypc
};

// Native functions for Pad: xxgpp_v_11
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_v_11_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_SLP_LAN },       .PadMode = GpioV2PadModeNative1 }  // Function: 1 slp_lan_b
};

// Native functions for Pad: xxgpp_v_12
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_v_12_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_WAKE },          .PadMode = GpioV2PadModeNative1 }  // Function: 1 wake_b
};

// Native functions for Pad: xxgpp_v_13
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_v_13_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_CATERR },        .PadMode = GpioV2PadModeNative1 }  // Function: 1 caterr_b
};

// Native functions for Pad: xxgpp_v_14
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_v_14_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_FORCEPR },       .PadMode = GpioV2PadModeNative1 }  // Function: 1 forcepr_b
};

// Native functions for Pad: xxgpp_v_15
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_v_15_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_THERMTRIP },     .PadMode = GpioV2PadModeNative1 }  // Function: 1 thermtrip_b
};

// Native functions for Pad: xxgpp_v_16
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_v_16_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_VCCST },         .PadMode = GpioV2PadModeNative1 }  // Function: 1 vccst_en
};

// Native functions for Pad: xxsys_pwrok
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxsys_pwrok_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_SYS_PWROK },     .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxsys_pwrok
};

// Native functions for Pad: xxsys_reset_b
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxsys_reset_b_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_SYS_RESET },     .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxsys_reset_b
};

// Native functions for Pad: xxl_bklten
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxl_bklten_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PANEL_BKLTEN },      .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxl_bklten
};

// Native functions for Pad: xxl_bkltctl
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxl_bkltctl_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PANEL_BKLTCTL },     .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxl_bkltctl
};

// Native functions for Pad: xxl_vdden
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxl_vdden_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PANEL_AVDD_EN },     .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxl_vdden
};

// Native functions for Pad: xxgpp_c_0
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_0_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SMB_CLK },           .PadMode = GpioV2PadModeNative1 }, // Function: 1 smbclk
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(0) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_0
};

// Native functions for Pad: xxgpp_c_1
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_1_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SMB_DATA },          .PadMode = GpioV2PadModeNative1 }, // Function: 1 smbdata
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(1) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_1
};

// Native functions for Pad: xxgpp_c_2
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_2_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SMB_ALERT_B },       .PadMode = GpioV2PadModeNative1 }, // Function: 1 smbalert_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(2) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_2
};

// Native functions for Pad: xxgpp_c_3
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_3_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SMB_SML_CLK(0) },    .PadMode = GpioV2PadModeNative1 }, // Function: 1 sml0clk
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(3) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_3
};

// Native functions for Pad: xxgpp_c_4
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_4_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SMB_SML_DATA(0) },   .PadMode = GpioV2PadModeNative1 }, // Function: 1 sml0data
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(4) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_4
};

// Native functions for Pad: xxgpp_c_5
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_5_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SMB_SML_ALERT(0) },  .PadMode = GpioV2PadModeNative1 }, // Function: 1 sml0alert_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(5) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_5
};

// Native functions for Pad: xxgpp_c_6
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_6_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SMB_SML_CLK(1) },    .PadMode = GpioV2PadModeNative1 }, // Function: 1 sml1clk
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(6) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_6
};

// Native functions for Pad: xxgpp_c_7
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_7_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SMB_SML_DATA(1) },   .PadMode = GpioV2PadModeNative1 }, // Function: 1 sml1data
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(7) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_7
};

// Native functions for Pad: xxgpp_c_8
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_8_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SMB_SML_ALERT(1) },  .PadMode = GpioV2PadModeNative1 }, // Function: 1 sml1alert_b
  { .Signal = { .Value = GPIOV2_SIGNAL_SMB_PCH_HOT_B },     .PadMode = GpioV2PadModeNative2 }, // Function: 2 pchhot_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(8) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_8
};

// Native functions for Pad: xxgpp_c_9
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_9_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PCIE_SRCCLKREQ(0) }, .PadMode = GpioV2PadModeNative1 }, // Function: 1 srcclkreq0_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(9) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_9
};

// Native functions for Pad: xxgpp_c_10
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_10_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PCIE_SRCCLKREQ(1) }, .PadMode = GpioV2PadModeNative1 }, // Function: 1 srcclkreq1_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(10) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_10
};

// Native functions for Pad: xxgpp_c_11
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_11_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PCIE_SRCCLKREQ(2) }, .PadMode = GpioV2PadModeNative1 }, // Function: 1 srcclkreq2_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(11) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_11
};

// Native functions for Pad: xxgpp_c_12
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_12_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PCIE_SRCCLKREQ(3) }, .PadMode = GpioV2PadModeNative1 }, // Function: 1 srcclkreq3_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(12) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_12
};

// Native functions for Pad: xxgpp_c_13
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_13_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PCIE_SRCCLKREQ(4) }, .PadMode = GpioV2PadModeNative1 }, // Function: 1 srcclkreq4_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(13) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_13
};

// Native functions for Pad: xxgpp_c_14
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_14_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PCIE_SRCCLKREQ(5) }, .PadMode = GpioV2PadModeNative1 }, // Function: 1 srcclkreq5_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(14) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_14
};

// Native functions for Pad: xxgpp_c_15
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_15_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(15) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_15
};

// Native functions for Pad: xxgpp_c_16
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_16_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_TBT_LSX_A(0) },      .PadMode = GpioV2PadModeNative1 }, // Function: 1 tbt_lsx0_a
  { .Signal = { .Value = GPIOV2_SIGNAL_DDP_CTRL_CLK(1) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 ddp1_ctrlclk
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(16) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_16
};

// Native functions for Pad: xxgpp_c_17
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_17_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_TBT_LSX_B(0) },      .PadMode = GpioV2PadModeNative1 }, // Function: 1 tbt_lsx0_b
  { .Signal = { .Value = GPIOV2_SIGNAL_DDP_CTRL_DATA(1) },  .PadMode = GpioV2PadModeNative2 }, // Function: 2 ddp1_ctrldata
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(17) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_17
};

// Native functions for Pad: xxgpp_c_18
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_18_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_TBT_LSX_A(1) },      .PadMode = GpioV2PadModeNative1 }, // Function: 1 tbt_lsx1_a
  { .Signal = { .Value = GPIOV2_SIGNAL_DDP_CTRL_CLK(2) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 ddp2_ctrlclk
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(18) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_18
};

// Native functions for Pad: xxgpp_c_19
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_19_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_TBT_LSX_B(1) },      .PadMode = GpioV2PadModeNative1 }, // Function: 1 tbt_lsx1_b
  { .Signal = { .Value = GPIOV2_SIGNAL_DDP_CTRL_DATA(2) },  .PadMode = GpioV2PadModeNative2 }, // Function: 2 ddp2_ctrldata
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(19) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_19
};

// Native functions for Pad: xxgpp_c_20
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_20_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_TBT_LSX_A(2) },      .PadMode = GpioV2PadModeNative1 }, // Function: 1 tbt_lsx2_a
  { .Signal = { .Value = GPIOV2_SIGNAL_DDP_CTRL_CLK(3) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 ddp3_ctrlclk
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(20) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_20
};

// Native functions for Pad: xxgpp_c_21
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_21_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_TBT_LSX_B(2) },      .PadMode = GpioV2PadModeNative1 }, // Function: 1 tbt_lsx2_b
  { .Signal = { .Value = GPIOV2_SIGNAL_DDP_CTRL_DATA(3) },  .PadMode = GpioV2PadModeNative2 }, // Function: 2 ddp3_ctrldata
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(21) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_21
};

// Native functions for Pad: xxgpp_c_22
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_22_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_TBT_LSX_A(3) },      .PadMode = GpioV2PadModeNative1 }, // Function: 1 tbt_lsx3_a
  { .Signal = { .Value = GPIOV2_SIGNAL_DDP_CTRL_CLK(4) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 ddp4_ctrlclk
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(22) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_22
};

// Native functions for Pad: xxgpp_c_23
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_c_23_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_TBT_LSX_B(3) },      .PadMode = GpioV2PadModeNative1 }, // Function: 1 tbt_lsx3_b
  { .Signal = { .Value = GPIOV2_SIGNAL_DDP_CTRL_DATA(4) },  .PadMode = GpioV2PadModeNative2 }, // Function: 2 ddp4_ctrldata
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPC(23) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPC_23
};

// Native functions for Pad: xxgpp_f_0
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_0_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_BRI_DT },       .PadMode = GpioV2PadModeNative1 }, // Function: 1 cnv_bri_dt
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_UART_RTS(2) },  .PadMode = GpioV2PadModeNative2 }, // Function: 2 uart2_rts_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(0) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPF_0
};

// Native functions for Pad: xxgpp_f_1
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_1_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_BRI_RSP },      .PadMode = GpioV2PadModeNative1 }, // Function: 1 cnv_bri_rsp
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_UART_RX(2) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 uart2_rxd
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(1) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPF_1
};

// Native functions for Pad: xxgpp_f_2
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_2_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_RGI_DT },       .PadMode = GpioV2PadModeNative1 }, // Function: 1 cnv_rgi_dt
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_UART_TX(2) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 uart2_txd
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(2) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPF_2
};

// Native functions for Pad: xxgpp_f_3
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_3_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_RGI_RSP },      .PadMode = GpioV2PadModeNative1 }, // Function: 1 cnv_rgi_rsp
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_UART_CTS(2) },  .PadMode = GpioV2PadModeNative2 }, // Function: 2 uart2_cts_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(3) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPF_3
};

// Native functions for Pad: xxgpp_f_4
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_4_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_RF_RESET },     .PadMode = GpioV2PadModeNative1 }, // Function: 1 cnv_rf_reset_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(4) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPF_4
};

// Native functions for Pad: xxgpp_f_5
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_5_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_CLKREQ },       .PadMode = GpioV2PadModeNative3 }, // Function: 3 crf_clkreq
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(5) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPF_5
};

// Native functions for Pad: xxgpp_f_6
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_6_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_PA_BLANKING },  .PadMode = GpioV2PadModeNative1 }, // Function: 1 cnv_pa_blanking
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(6) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPF_6
};

// Native functions for Pad: xxgpp_f_7
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_7_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_MISC_FUSA_DIAGTEST_EN },.PadMode = GpioV2PadModeNative1 }, // Function: 1 fusa_diagtest_en
  { .Signal = { .Value = GPIOV2_SIGNAL_IPU_IMGCLKOUT(2) },  .PadMode = GpioV2PadModeNative2 }, // Function: 2 imgclkout_2
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(7) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPF_7
};

// Native functions for Pad: xxgpp_f_8
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_8_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_MISC_FUSA_DIAGTEST_MODE },.PadMode = GpioV2PadModeNative1 }, // Function: 1 fusa_diagtest_mode
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(8) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPF_8
};

// Native functions for Pad: xxgpp_f_9
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_9_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_SX_HOLDOFF },    .PadMode = GpioV2PadModeNative2 }, // Function: 2 sx_exit_holdoff_b
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_GP(11) },        .PadMode = GpioV2PadModeNative4 }, // Function: 4 ish_gp_11
  { .Signal = { .Value = GPIOV2_SIGNAL_MISC_IEH_FATAL_ERR },.PadMode = GpioV2PadModeNative5 }, // Function: 5 ieh_fatal_err2_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(9) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPF_9
};

// Native functions for Pad: xxgpp_f_10
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_10_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(10) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPF_10
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_GP(6) },         .PadMode = GpioV2PadModeNative8 }  // Function: 8 a_ish_gp_6
};

// Native functions for Pad: xxgpp_f_11
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_11_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_CLK(1) },        .PadMode = GpioV2PadModeNative3 }, // Function: 3 thc1_spi2_clk
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_SPI_CLK },       .PadMode = GpioV2PadModeNative4 }, // Function: 4 a_ish_spi_clk
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_SPI_CLK(1) },   .PadMode = GpioV2PadModeNative5 }, // Function: 5 gspi1_clk
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(11) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPF_11
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH2_CLK },      .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch2_clk
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_CLK(1) },  .PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_clk
  { .Signal = { .Value = GPIOV2_SIGNAL_VISAFUSECH1_CLK },   .PadMode = GpioV2PadModeNative11 }  // Function: 11 visafusech1_clk
};

// Native functions for Pad: xxgpp_f_12
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_12_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_I2C_SCL(1) },    .PadMode = GpioV2PadModeNative1 }, // Function: 1 thc_i2c1_scl
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I3C_SCL(2) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 i3c2_scl
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_IO0(1) },        .PadMode = GpioV2PadModeNative3 }, // Function: 3 thc1_spi2_io_0
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_SPI_MISO },      .PadMode = GpioV2PadModeNative4 }, // Function: 4 a_ish_spi_miso
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_SPI_MOSI(1) },  .PadMode = GpioV2PadModeNative5 }, // Function: 5 gspi1_mosi
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(12) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPF_12
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH2_DATA(0) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch2_d0
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I2C_SCL(5) },   .PadMode = GpioV2PadModeNative8 }, // Function: 8 i2c5_scl
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(0) }, .PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d00
  { .Signal = { .Value = GPIOV2_SIGNAL_VISAFUSECH1_DATA(0) },.PadMode = GpioV2PadModeNative11 }  // Function: 11 visafusech1_d0
};

// Native functions for Pad: xxgpp_f_13
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_13_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_I2C_SDA(1) },    .PadMode = GpioV2PadModeNative1 }, // Function: 1 thc_i2c1_sda
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I3C_SDA(2) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 i3c2_sda
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_IO1(1) },        .PadMode = GpioV2PadModeNative3 }, // Function: 3 thc1_spi2_io_1
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_SPI_MOSI },      .PadMode = GpioV2PadModeNative4 }, // Function: 4 a_ish_spi_mosi
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_SPI_MISO(1) },  .PadMode = GpioV2PadModeNative5 }, // Function: 5 gspi1_miso
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(13) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPF_13
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH2_DATA(1) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch2_d1
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I2C_SDA(5) },   .PadMode = GpioV2PadModeNative8 }, // Function: 8 i2c5_sda
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(1) }, .PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d01
  { .Signal = { .Value = GPIOV2_SIGNAL_VISAFUSECH1_DATA(1) },.PadMode = GpioV2PadModeNative11 }  // Function: 11 visafusech1_d1
};

// Native functions for Pad: xxgpp_f_14
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_14_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_IO2(1) },        .PadMode = GpioV2PadModeNative3 }, // Function: 3 thc1_spi2_io_2
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(14) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPF_14
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH2_DATA(2) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch2_d2
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_SPI_MOSI(0) },  .PadMode = GpioV2PadModeNative8 }, // Function: 8 a_gspi0_mosi
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(2) }, .PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d02
  { .Signal = { .Value = GPIOV2_SIGNAL_VISAFUSECH1_DATA(2) },.PadMode = GpioV2PadModeNative11 }  // Function: 11 visafusech1_d2
};

// Native functions for Pad: xxgpp_f_15
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_15_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_IO3(1) },        .PadMode = GpioV2PadModeNative3 }, // Function: 3 thc1_spi2_io_3
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(15) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPF_15
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH2_DATA(3) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch2_d3
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_SPI_MISO(0) },  .PadMode = GpioV2PadModeNative8 }, // Function: 8 a_gspi0_miso
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(3) }, .PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d03
  { .Signal = { .Value = GPIOV2_SIGNAL_VISAFUSECH1_DATA(3) },.PadMode = GpioV2PadModeNative11 }  // Function: 11 visafusech1_d3
};

// Native functions for Pad: xxgpp_f_16
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_16_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_RSTB(1) },       .PadMode = GpioV2PadModeNative3 }, // Function: 3 thc1_spi2_rst_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(16) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPF_16
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH2_DATA(4) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch2_d4
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_SPI_CLK(0) },   .PadMode = GpioV2PadModeNative8 }, // Function: 8 a_gspi0_clk
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(4) }, .PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d04
  { .Signal = { .Value = GPIOV2_SIGNAL_VISAFUSECH1_DATA(4) },.PadMode = GpioV2PadModeNative11 }  // Function: 11 visafusech1_d4
};

// Native functions for Pad: xxgpp_f_17
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_17_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_CSB(1) },        .PadMode = GpioV2PadModeNative3 }, // Function: 3 thc1_spi2_cs_b
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_SPI_CS },        .PadMode = GpioV2PadModeNative4 }, // Function: 4 a_ish_spi_cs_b
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_SPI_CS(1) },    .PadMode = GpioV2PadModeNative5 }, // Function: 5 gspi1_cs0_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(17) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPF_17
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH2_DATA(5) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch2_d5
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(5) }, .PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d05
  { .Signal = { .Value = GPIOV2_SIGNAL_VISAFUSECH1_DATA(5) },.PadMode = GpioV2PadModeNative11 }  // Function: 11 visafusech1_d5
};

// Native functions for Pad: xxgpp_f_18
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_18_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_SPI_INT(1) },    .PadMode = GpioV2PadModeNative3 }, // Function: 3 thc1_spi2_int_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(18) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPF_18
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH2_DATA(6) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch2_d6
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_SPI_CS(0) },    .PadMode = GpioV2PadModeNative8 }, // Function: 8 a_gspi0_cs0_b
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(6) }, .PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d06
  { .Signal = { .Value = GPIOV2_SIGNAL_VISAFUSECH1_DATA(6) },.PadMode = GpioV2PadModeNative11 }  // Function: 11 visafusech1_d6
};

// Native functions for Pad: xxgpp_f_19
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_19_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(19) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPF_19
};

// Native functions for Pad: xxgpp_f_20
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_20_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(20) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPF_20
};

// Native functions for Pad: xxgpp_f_21
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_21_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(21) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPF_21
};

// Native functions for Pad: xxgpp_f_22
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_22_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_DSYNC(1) },      .PadMode = GpioV2PadModeNative3 }, // Function: 3 thc1_spi2_dsync
  { .Signal = { .Value = GPIOV2_SIGNAL_MISC_IEH_CORR_ERR }, .PadMode = GpioV2PadModeNative5 }, // Function: 5 ieh_corr_err0_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(22) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPF_22
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH2_DATA(7) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch2_d7
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_GP(8) },         .PadMode = GpioV2PadModeNative8 }, // Function: 8 a_ish_gp_8
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(7) }, .PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d07
  { .Signal = { .Value = GPIOV2_SIGNAL_VISAFUSECH1_DATA(7) },.PadMode = GpioV2PadModeNative11 }  // Function: 11 visafusech1_d7
};

// Native functions for Pad: xxgpp_f_23
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_f_23_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_MISC_IEH_NONFATAL_ERR },.PadMode = GpioV2PadModeNative5 }, // Function: 5 ieh_nonfatal_err1_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPF(23) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPF_23
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_GP(9) },         .PadMode = GpioV2PadModeNative8 }  // Function: 8 a_ish_gp_9
};

// Native functions for Pad: thc1_gspi1_i3c2_clk_loopbk
GPIOV2_PAD_SIGNAL PtlPcdPch0_thc1_gspi1_i3c2_clk_loopbk_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I3C_SCL_FB(2) },.PadMode = GpioV2PadModeNative2 }, // Function: 2 i3c2_clk_loopbk
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_CLK_LOOPBACK(1) },.PadMode = GpioV2PadModeNative3 }  // Function: 3 thc1_spi2_clk_loopbk
};

// Native functions for Pad: xxgpp_e_0
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_e_0_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPE(0) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPE_0
};

// Native functions for Pad: xxgpp_e_1
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_e_1_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_CPU_GP(2) },         .PadMode = GpioV2PadModeNative1 }, // Function: 1 cpu_gp_2
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_SLP_DRAM },      .PadMode = GpioV2PadModeNative2 }, // Function: 2 slp_dram_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPE(1) },       .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPE_1
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_GP(5) },         .PadMode = GpioV2PadModeNative8 }  // Function: 8 a_ish_gp_5
};

// Native functions for Pad: xxgpp_e_2
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_e_2_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_CPU_GP(3) },         .PadMode = GpioV2PadModeNative1 }, // Function: 1 cpu_gp_3
  { .Signal = { .Value = GPIOV2_SIGNAL_VR_ALERT },          .PadMode = GpioV2PadModeNative2 }, // Function: 2 vralert_b
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_GP(10) },        .PadMode = GpioV2PadModeNative4 }, // Function: 4 ish_gp_10
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPE(2) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPE_2
};

// Native functions for Pad: xxgpp_e_3
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_e_3_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_CPU_GP(0) },         .PadMode = GpioV2PadModeNative1 }, // Function: 1 cpu_gp_0
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPE(3) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPE_3
};

// Native functions for Pad: xxgpp_e_4
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_e_4_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPE(4) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPE_4
};

// Native functions for Pad: xxgpp_e_5
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_e_5_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_GP(7) },         .PadMode = GpioV2PadModeNative4 }, // Function: 4 ish_gp_7
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPE(5) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPE_5
};

// Native functions for Pad: xxgpp_e_6
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_e_6_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPE(6) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPE_6
};

// Native functions for Pad: xxgpp_e_7
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_e_7_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_DDP_CTRL_CLK_A },    .PadMode = GpioV2PadModeNative1 }, // Function: 1 ddpa_ctrlclk
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPE(7) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPE_7
};

// Native functions for Pad: xxgpp_e_8
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_e_8_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_DDP_CTRL_DATA_A },   .PadMode = GpioV2PadModeNative1 }, // Function: 1 ddpa_ctrldata
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPE(8) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPE_8
};

// Native functions for Pad: xxgpp_e_9
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_e_9_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_USB_OC(0) },         .PadMode = GpioV2PadModeNative1 }, // Function: 1 usb2_oc0_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPE(9) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPE_9
};

// Native functions for Pad: xxgpp_e_10
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_e_10_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPE(10) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPE_10
};

// Native functions for Pad: xxgpp_e_11
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_e_11_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_CLK(0) },        .PadMode = GpioV2PadModeNative3 }, // Function: 3 thc0_spi1_clk
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_SPI_CLK(0) },   .PadMode = GpioV2PadModeNative5 }, // Function: 5 gspi0_clk
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPE(11) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPE_11
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH1_CLK },      .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch1_clk
  { .Signal = { .Value = GPIOV2_SIGNAL_VISAOSSEFUSECH1_CLK },.PadMode = GpioV2PadModeNative11 }  // Function: 11 visaossefusech1_clk
};

// Native functions for Pad: xxgpp_e_12
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_e_12_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_I2C_SCL(0) },    .PadMode = GpioV2PadModeNative1 }, // Function: 1 thc_i2c0_scl
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_IO0(0) },        .PadMode = GpioV2PadModeNative3 }, // Function: 3 thc0_spi1_io_0
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_SPI_MOSI(0) },  .PadMode = GpioV2PadModeNative5 }, // Function: 5 gspi0_mosi
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPE(12) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPE_12
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH1_DATA(0) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch1_d0
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I2C_SCL(4) },   .PadMode = GpioV2PadModeNative8 }, // Function: 8 i2c4_scl
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(8) }, .PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d08
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_DEBUG_A(0) },   .PadMode = GpioV2PadModeNative10 }, // Function: 10 a_cnv_debug_00
  { .Signal = { .Value = GPIOV2_SIGNAL_VISAOSSEFUSECH1_DATA(0) },.PadMode = GpioV2PadModeNative11 }  // Function: 11 visaossefusech1_d0
};

// Native functions for Pad: xxgpp_e_13
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_e_13_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_I2C_SDA(0) },    .PadMode = GpioV2PadModeNative1 }, // Function: 1 thc_i2c0_sda
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_IO1(0) },        .PadMode = GpioV2PadModeNative3 }, // Function: 3 thc0_spi1_io_1
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_SPI_MISO(0) },  .PadMode = GpioV2PadModeNative5 }, // Function: 5 gspi0_miso
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPE(13) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPE_13
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH1_DATA(1) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch1_d1
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I2C_SDA(4) },   .PadMode = GpioV2PadModeNative8 }, // Function: 8 i2c4_sda
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(9) }, .PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d09
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_DEBUG_A(1) },   .PadMode = GpioV2PadModeNative10 }, // Function: 10 a_cnv_debug_01
  { .Signal = { .Value = GPIOV2_SIGNAL_VISAOSSEFUSECH1_DATA(1) },.PadMode = GpioV2PadModeNative11 }  // Function: 11 visaossefusech1_d1
};

// Native functions for Pad: xxgpp_e_14
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_e_14_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_IO2(0) },        .PadMode = GpioV2PadModeNative3 }, // Function: 3 thc0_spi1_io_2
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPE(14) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPE_14
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH1_DATA(2) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch1_d2
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(10) },.PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d10
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_DEBUG_A(2) },   .PadMode = GpioV2PadModeNative10 }, // Function: 10 a_cnv_debug_02
  { .Signal = { .Value = GPIOV2_SIGNAL_VISAOSSEFUSECH1_DATA(2) },.PadMode = GpioV2PadModeNative11 }  // Function: 11 visaossefusech1_d2
};

// Native functions for Pad: xxgpp_e_15
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_e_15_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_IO3(0) },        .PadMode = GpioV2PadModeNative3 }, // Function: 3 thc0_spi1_io_3
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPE(15) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPE_15
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH1_DATA(3) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch1_d3
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(11) },.PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d11
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_DEBUG_A(3) },   .PadMode = GpioV2PadModeNative10 }, // Function: 10 a_cnv_debug_03
  { .Signal = { .Value = GPIOV2_SIGNAL_VISAOSSEFUSECH1_DATA(3) },.PadMode = GpioV2PadModeNative11 }  // Function: 11 visaossefusech1_d3
};

// Native functions for Pad: xxgpp_e_16
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_e_16_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_RSTB(0) },       .PadMode = GpioV2PadModeNative3 }, // Function: 3 thc0_spi1_rst_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPE(16) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPE_16
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH1_DATA(4) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch1_d4
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(12) },.PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d12
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_DEBUG_A(4) },   .PadMode = GpioV2PadModeNative10 }, // Function: 10 a_cnv_debug_04
  { .Signal = { .Value = GPIOV2_SIGNAL_VISAOSSEFUSECH1_DATA(4) },.PadMode = GpioV2PadModeNative11 }  // Function: 11 visaossefusech1_d4
};

// Native functions for Pad: xxgpp_e_17
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_e_17_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_CSB(0) },        .PadMode = GpioV2PadModeNative3 }, // Function: 3 thc0_spi1_cs_b
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_SPI_CS(0) },    .PadMode = GpioV2PadModeNative5 }, // Function: 5 gspi0_cs0_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPE(17) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPE_17
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH1_DATA(5) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch1_d5
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(13) },.PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d13
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_DEBUG_A(5) },   .PadMode = GpioV2PadModeNative10 }, // Function: 10 a_cnv_debug_05
  { .Signal = { .Value = GPIOV2_SIGNAL_VISAOSSEFUSECH1_DATA(5) },.PadMode = GpioV2PadModeNative11 }  // Function: 11 visaossefusech1_d5
};

// Native functions for Pad: xxgpp_e_18
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_e_18_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_SPI_INT(0) },    .PadMode = GpioV2PadModeNative3 }, // Function: 3 thc0_spi1_int_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPE(18) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPE_18
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH1_DATA(6) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch1_d6
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(14) },.PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d14
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_DEBUG_A(6) },   .PadMode = GpioV2PadModeNative10 }, // Function: 10 a_cnv_debug_06
  { .Signal = { .Value = GPIOV2_SIGNAL_VISAOSSEFUSECH1_DATA(6) },.PadMode = GpioV2PadModeNative11 }  // Function: 11 visaossefusech1_d6
};

// Native functions for Pad: xxgpp_e_19
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_e_19_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PMC_I2C_SDA },       .PadMode = GpioV2PadModeNative1 }, // Function: 1 pmc_i2c_sda
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPE(19) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPE_19
};

// Native functions for Pad: xxgpp_e_20
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_e_20_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PMC_I2C_SCL },       .PadMode = GpioV2PadModeNative1 }, // Function: 1 pmc_i2c_scl
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPE(20) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPE_20
};

// Native functions for Pad: xxgpp_e_21
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_e_21_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PMC_ALERT_B },       .PadMode = GpioV2PadModeNative1 }, // Function: 1 pmcalert_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPE(21) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPE_21
};

// Native functions for Pad: xxgpp_e_22
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_e_22_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_DSYNC(0) },      .PadMode = GpioV2PadModeNative3 }, // Function: 3 thc0_spi1_dsync
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPE(22) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPE_22
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH1_DATA(7) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch1_d7
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(15) },.PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d15
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_DEBUG_A(7) },   .PadMode = GpioV2PadModeNative10 }, // Function: 10 a_cnv_debug_07
  { .Signal = { .Value = GPIOV2_SIGNAL_VISAOSSEFUSECH1_DATA(7) },.PadMode = GpioV2PadModeNative11 }  // Function: 11 visaossefusech1_d7
};

// Native functions for Pad: xxboothalt_b
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxboothalt_b_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_BOOTHALT },      .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxboothalt_b
};

// Native functions for Pad: thc0_gspi0_clk_loopbk
GPIOV2_PAD_SIGNAL PtlPcdPch0_thc0_gspi0_clk_loopbk_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_THC_CLK_LOOPBACK(0) },.PadMode = GpioV2PadModeNative3 }  // Function: 3 thc0_spi1_clk_loopbk
};

// Native functions for Pad: xxepd_on
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxepd_on_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_EPD_ON },        .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxepd_on
};

// Native functions for Pad: xxvdd2_pwrgd
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxvdd2_pwrgd_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_VDD2_PWRGD },    .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxvdd2_pwrgd
};

// Native functions for Pad: xxjtag_mbpb0
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxjtag_mbpb0_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_JTAG_MBPB(0) },      .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxjtag_mbpb0
};

// Native functions for Pad: xxjtag_mbpb1
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxjtag_mbpb1_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_JTAG_MBPB(1) },      .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxjtag_mbpb1
};

// Native functions for Pad: xxjtag_mbpb2
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxjtag_mbpb2_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_JTAG_MBPB(2) },      .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxjtag_mbpb2
};

// Native functions for Pad: xxjtag_mbpb3
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxjtag_mbpb3_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_JTAG_MBPB(3) },      .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxjtag_mbpb3
};

// Native functions for Pad: xxjtag_tdo
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxjtag_tdo_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_JTAG_TDO },          .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxjtag_tdo
};

// Native functions for Pad: xxprdy_b
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxprdy_b_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_JTAG_PRDY_B },       .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxprdy_b
};

// Native functions for Pad: xxpreq_b
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxpreq_b_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_JTAG_PREQ_B },       .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxpreq_b
};

// Native functions for Pad: xxjtag_tdi
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxjtag_tdi_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_JTAG_TDI },          .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxjtag_tdi
};

// Native functions for Pad: xxjtag_tms
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxjtag_tms_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_JTAG_TMS },          .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxjtag_tms
};

// Native functions for Pad: xxjtag_tck
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxjtag_tck_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_JTAG_TCK },          .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxjtag_tck
};

// Native functions for Pad: xxjtag_trst_b
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxjtag_trst_b_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_JTAG_TRST_B },       .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxjtag_trst_b
};

// Native functions for Pad: xxddsp_hpdalv
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxddsp_hpdalv_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_DDSP_HPDALV },       .PadMode = GpioV2PadModeNative1 }, // Function: 1 xxddsp_hpdalv
  { .Signal = { .Value = GPIOV2_SIGNAL_DISP_MISC_A },       .PadMode = GpioV2PadModeNative2 }  // Function: 2 disp_misca
};

// Native functions for Pad: xxgpp_h_0
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_0_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(0) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPH_0
};

// Native functions for Pad: xxgpp_h_1
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_1_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(1) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPH_1
};

// Native functions for Pad: xxgpp_h_2
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_2_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(2) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPH_2
};

// Native functions for Pad: xxgpp_h_3
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_3_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_MISC_MIC_MUTE },     .PadMode = GpioV2PadModeNative1 }, // Function: 1 mic_mute
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(3) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPH_3
};

// Native functions for Pad: xxgpp_h_4
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_4_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I2C_SDA(2) },   .PadMode = GpioV2PadModeNative1 }, // Function: 1 i2c2_sda
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_MFUART_RXD(2) },.PadMode = GpioV2PadModeNative2 }, // Function: 2 cnv_mfuart2_rxd
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(4) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPH_4
};

// Native functions for Pad: xxgpp_h_5
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_5_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I2C_SCL(2) },   .PadMode = GpioV2PadModeNative1 }, // Function: 1 i2c2_scl
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_MFUART_TXD(2) },.PadMode = GpioV2PadModeNative2 }, // Function: 2 cnv_mfuart2_txd
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(5) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPH_5
};

// Native functions for Pad: xxgpp_h_6
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_6_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I2C_SDA(3) },   .PadMode = GpioV2PadModeNative1 }, // Function: 1 i2c3_sda
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_UART_RX(1) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 uart1_rxd
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_UART_RX(1) },    .PadMode = GpioV2PadModeNative3 }, // Function: 3 a_ish_uart1_rxd
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(6) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPH_6
};

// Native functions for Pad: xxgpp_h_7
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_7_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I2C_SCL(3) },   .PadMode = GpioV2PadModeNative1 }, // Function: 1 i2c3_scl
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_UART_TX(1) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 uart1_txd
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_UART_TX(1) },    .PadMode = GpioV2PadModeNative3 }, // Function: 3 a_ish_uart1_txd
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(7) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPH_7
};

// Native functions for Pad: xxgpp_h_8
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_8_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_UART_RX(0) },   .PadMode = GpioV2PadModeNative1 }, // Function: 1 uart0_rxd
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(8) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPH_8
};

// Native functions for Pad: xxgpp_h_9
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_9_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_UART_TX(0) },   .PadMode = GpioV2PadModeNative1 }, // Function: 1 uart0_txd
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(9) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPH_9
};

// Native functions for Pad: xxgpp_h_10
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_10_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_UART_RTS(0) },  .PadMode = GpioV2PadModeNative1 }, // Function: 1 uart0_rts_b
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I3C_SDA(1) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 a_i3c1_sda
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(10) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPH_10
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_GP(10) },        .PadMode = GpioV2PadModeNative8 }  // Function: 8 a_ish_gp_10
};

// Native functions for Pad: xxgpp_h_11
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_11_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_UART_CTS(0) },  .PadMode = GpioV2PadModeNative1 }, // Function: 1 uart0_cts_b
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I3C_SCL(1) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 a_i3c1_scl
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(11) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPH_11
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_GP(11) },        .PadMode = GpioV2PadModeNative8 }  // Function: 8 a_ish_gp_11
};

// Native functions for Pad: xxgpp_h_12
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_12_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(12) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPH_12
};

// Native functions for Pad: xxgpp_h_13
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_13_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_CPU_C10_GATE },      .PadMode = GpioV2PadModeNative1 }, // Function: 1 cpu_c10_gate_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(13) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPH_13
};

// Native functions for Pad: xxgpp_h_14
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_14_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_UART_RX(1) },    .PadMode = GpioV2PadModeNative1 }, // Function: 1 ish_uart1_rxd
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_UART_RX(1) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 a_uart1_rxd
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_I2C_SDA(1) },    .PadMode = GpioV2PadModeNative3 }, // Function: 3 ish_i2c1_sda
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_I3C_SDA(1) },    .PadMode = GpioV2PadModeNative4 }, // Function: 4 ish_i3c1_sda
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(14) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPH_14
};

// Native functions for Pad: xxgpp_h_15
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_15_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_UART_TX(1) },    .PadMode = GpioV2PadModeNative1 }, // Function: 1 ish_uart1_txd
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_UART_TX(1) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 a_uart1_txd
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_I2C_SCL(1) },    .PadMode = GpioV2PadModeNative3 }, // Function: 3 ish_i2c1_scl
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_I3C_SCL(1) },    .PadMode = GpioV2PadModeNative4 }, // Function: 4 ish_i3c1_scl
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(15) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPH_15
};

// Native functions for Pad: xxgpp_h_16
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_16_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_TBT_LSX_OE(2) },     .PadMode = GpioV2PadModeNative1 }, // Function: 1 tbt_lsx2_oe
  { .Signal = { .Value = GPIOV2_SIGNAL_PCIE_LNK_DOWN },     .PadMode = GpioV2PadModeNative3 }, // Function: 3 pcie_lnk_down
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(16) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPH_16
};

// Native functions for Pad: xxgpp_h_17
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_17_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_MISC_MIC_MUTE_LED }, .PadMode = GpioV2PadModeNative1 }, // Function: 1 mic_mute_led
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(17) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPH_17
};

// Native functions for Pad: xxgpp_h_18
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_18_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(18) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPH_18
};

// Native functions for Pad: xxgpp_h_19
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_19_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I2C_SDA(0) },   .PadMode = GpioV2PadModeNative1 }, // Function: 1 i2c0_sda
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I3C_SDA(0) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 i3c0_sda
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(19) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPH_19
};

// Native functions for Pad: xxgpp_h_20
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_20_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I2C_SCL(0) },   .PadMode = GpioV2PadModeNative1 }, // Function: 1 i2c0_scl
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I3C_SCL(0) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 i3c0_scl
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(20) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPH_20
};

// Native functions for Pad: xxgpp_h_21
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_21_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I2C_SDA(1) },   .PadMode = GpioV2PadModeNative1 }, // Function: 1 i2c1_sda
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I3C_SDA(1) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 i3c1_sda
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(21) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPH_21
};

// Native functions for Pad: xxgpp_h_22
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_22_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I2C_SCL(1) },   .PadMode = GpioV2PadModeNative1 }, // Function: 1 i2c1_scl
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I3C_SCL(1) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 i3c1_scl
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(22) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPH_22
};

// Native functions for Pad: xxgpp_h_23
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_23_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ESPI_CS(4) },        .PadMode = GpioV2PadModeNative1 }, // Function: 1 espi_cs4_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(23) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPH_23
};

// Native functions for Pad: xxgpp_h_24
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_h_24_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ESPI_ALERT(4) },     .PadMode = GpioV2PadModeNative1 }, // Function: 1 espi_alert4_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPH(24) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPH_24
};

// Native functions for Pad: lpi3c1_clk_loopbk
GPIOV2_PAD_SIGNAL PtlPcdPch0_lpi3c1_clk_loopbk_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I3C_SCL_FB(1) },.PadMode = GpioV2PadModeNative2 }  // Function: 2 i3c1_clk_loopbk
};

// Native functions for Pad: lpi3c0_clk_loopbk
GPIOV2_PAD_SIGNAL PtlPcdPch0_lpi3c0_clk_loopbk_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I3C_SCL_FB(0) },.PadMode = GpioV2PadModeNative2 }  // Function: 2 i3c0_clk_loopbk
};

// Native functions for Pad: ishi3c1_clk_loopbk
GPIOV2_PAD_SIGNAL PtlPcdPch0_ishi3c1_clk_loopbk_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_I3C_SCL_FB(1) }, .PadMode = GpioV2PadModeNative4 }  // Function: 4 ish_i3c1_clk_loopbk
};

// Native functions for Pad: xxgpp_a_0
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_a_0_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ESPI_IO(0) },        .PadMode = GpioV2PadModeNative1 }, // Function: 1 espi_io_0
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPA(0) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPA_0
};

// Native functions for Pad: xxgpp_a_1
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_a_1_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ESPI_IO(1) },        .PadMode = GpioV2PadModeNative1 }, // Function: 1 espi_io_1
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPA(1) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPA_1
};

// Native functions for Pad: xxgpp_a_2
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_a_2_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ESPI_IO(2) },        .PadMode = GpioV2PadModeNative1 }, // Function: 1 espi_io_2
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_PRIPWRDNACK },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 pripwrdnack
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPA(2) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPA_2
};

// Native functions for Pad: xxgpp_a_3
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_a_3_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ESPI_IO(3) },        .PadMode = GpioV2PadModeNative1 }, // Function: 1 espi_io_3
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_PRIACK },        .PadMode = GpioV2PadModeNative2 }, // Function: 2 priack_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPA(3) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPA_3
};

// Native functions for Pad: xxgpp_a_4
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_a_4_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ESPI_CS(0) },        .PadMode = GpioV2PadModeNative1 }, // Function: 1 espi_cs0_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPA(4) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPA_4
};

// Native functions for Pad: xxgpp_a_5
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_a_5_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ESPI_CLK },          .PadMode = GpioV2PadModeNative1 }, // Function: 1 espi_clk
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPA(5) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPA_5
};

// Native functions for Pad: xxgpp_a_6
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_a_6_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ESPI_RESET },        .PadMode = GpioV2PadModeNative1 }, // Function: 1 espi_reset_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPA(6) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPA_6
};

// Native functions for Pad: xxgpp_a_7
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_a_7_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPA(7) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPA_7
};

// Native functions for Pad: xxgpp_a_8
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_a_8_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_OSSE_SMLCLK },       .PadMode = GpioV2PadModeNative1 }, // Function: 1 osse_smlclk
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPA(8) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPA_8
};

// Native functions for Pad: xxgpp_a_9
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_a_9_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_OSSE_SMLDATA },      .PadMode = GpioV2PadModeNative1 }, // Function: 1 osse_smldata
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPA(9) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPA_9
};

// Native functions for Pad: xxgpp_a_10
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_a_10_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_OSSE_SMLALERT_B },   .PadMode = GpioV2PadModeNative1 }, // Function: 1 osse_smlalert_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPA(10) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPA_10
};

// Native functions for Pad: xxgpp_a_11
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_a_11_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPA(11) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPA_11
};

// Native functions for Pad: xxgpp_a_12
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_a_12_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPA(12) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPA_12
};

// Native functions for Pad: xxgpp_a_13
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_a_13_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ESPI_CS(1) },        .PadMode = GpioV2PadModeNative1 }, // Function: 1 espi_cs1_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPA(13) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPA_13
};

// Native functions for Pad: xxgpp_a_14
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_a_14_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPA(14) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPA_14
};

// Native functions for Pad: xxgpp_a_15
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_a_15_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_MISC_DNX_FORCE_RELOAD },.PadMode = GpioV2PadModeNative1 }, // Function: 1 dnx_force_reload
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPA(15) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPA_15
};

// Native functions for Pad: xxgpp_a_16
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_a_16_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ESPI_CS(2) },        .PadMode = GpioV2PadModeNative1 }, // Function: 1 espi_cs2_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPA(16) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPA_16
};

// Native functions for Pad: xxgpp_a_17
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_a_17_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ESPI_CS(3) },        .PadMode = GpioV2PadModeNative1 }, // Function: 1 espi_cs3_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPA(17) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPA_17
};

// Native functions for Pad: xxspi0_io_2
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxspi0_io_2_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SPI_IO(2) },         .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxspi0_io_2
};

// Native functions for Pad: xxspi0_io_3
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxspi0_io_3_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SPI_IO(3) },         .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxspi0_io_3
};

// Native functions for Pad: xxspi0_mosi_io_0
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxspi0_mosi_io_0_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SPI_MOSI },          .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxspi0_mosi_io_0
};

// Native functions for Pad: xxspi0_miso_io_1
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxspi0_miso_io_1_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SPI_MISO },          .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxspi0_miso_io_1
};

// Native functions for Pad: xxspi0_tpm_cs_b
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxspi0_tpm_cs_b_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SPI_TPM_CS },        .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxspi0_tpm_cs_b
};

// Native functions for Pad: xxspi0_flash_0_cs_b
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxspi0_flash_0_cs_b_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SPI_FLASH_CS(0) },   .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxspi0_flash_0_cs_b
};

// Native functions for Pad: xxspi0_flash_1_cs_b
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxspi0_flash_1_cs_b_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SPI_FLASH_CS(1) },   .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxspi0_flash_1_cs_b
};

// Native functions for Pad: xxspi0_clk
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxspi0_clk_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SPI_CLK },           .PadMode = GpioV2PadModeNative1 }  // Function: 1 xxspi0_clk
};

// Native functions for Pad: vGPIO_USB_0
GPIOV2_PAD_SIGNAL PtlPcdPch0_vGPIO_USB_0_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ESPI_USB_OCB(0) },   .PadMode = GpioV2PadModeNative1 }  // Function: 1 ESPI_USB_OCB_0
};

// Native functions for Pad: vGPIO_USB_1
GPIOV2_PAD_SIGNAL PtlPcdPch0_vGPIO_USB_1_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ESPI_USB_OCB(1) },   .PadMode = GpioV2PadModeNative1 }  // Function: 1 ESPI_USB_OCB_1
};

// Native functions for Pad: vGPIO_USB_2
GPIOV2_PAD_SIGNAL PtlPcdPch0_vGPIO_USB_2_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ESPI_USB_OCB(2) },   .PadMode = GpioV2PadModeNative1 }  // Function: 1 ESPI_USB_OCB_2
};

// Native functions for Pad: vGPIO_USB_3
GPIOV2_PAD_SIGNAL PtlPcdPch0_vGPIO_USB_3_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ESPI_USB_OCB(3) },   .PadMode = GpioV2PadModeNative1 }  // Function: 1 ESPI_USB_OCB_3
};

// Native functions for Pad: vGPIO_USB_4
GPIOV2_PAD_SIGNAL PtlPcdPch0_vGPIO_USB_4_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_CPU_USB_OCB(0) },    .PadMode = GpioV2PadModeNative1 }  // Function: 1 USB_CPU_OCB_0
};

// Native functions for Pad: vGPIO_USB_5
GPIOV2_PAD_SIGNAL PtlPcdPch0_vGPIO_USB_5_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_CPU_USB_OCB(1) },    .PadMode = GpioV2PadModeNative1 }  // Function: 1 USB_CPU_OCB_1
};

// Native functions for Pad: vGPIO_USB_6
GPIOV2_PAD_SIGNAL PtlPcdPch0_vGPIO_USB_6_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_CPU_USB_OCB(2) },    .PadMode = GpioV2PadModeNative1 }  // Function: 1 USB_CPU_OCB_2
};

// Native functions for Pad: vGPIO_USB_7
GPIOV2_PAD_SIGNAL PtlPcdPch0_vGPIO_USB_7_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_CPU_USB_OCB(3) },    .PadMode = GpioV2PadModeNative1 }  // Function: 1 USB_CPU_OCB_3
};

// Native functions for Pad: xxgpp_s_0
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_s_0_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SNDW_CLK(3) },       .PadMode = GpioV2PadModeNative1 }, // Function: 1 sndw3_clk
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_TXD(1) },        .PadMode = GpioV2PadModeNative6 }  // Function: 6 i2s1_txd
};

// Native functions for Pad: xxgpp_s_1
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_s_1_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SNDW_DATA0(3) },     .PadMode = GpioV2PadModeNative1 }, // Function: 1 sndw3_data0
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_RXD(1) },        .PadMode = GpioV2PadModeNative6 }  // Function: 6 i2s1_rxd
};

// Native functions for Pad: xxgpp_s_2
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_s_2_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SNDW_DATA1(3) },     .PadMode = GpioV2PadModeNative1 }, // Function: 1 sndw3_data1
  { .Signal = { .Value = GPIOV2_SIGNAL_SNDW_CLK(0) },       .PadMode = GpioV2PadModeNative3 }, // Function: 3 sndw0_clk
  { .Signal = { .Value = GPIOV2_SIGNAL_DMIC_CLKA(0) },      .PadMode = GpioV2PadModeNative5 }, // Function: 5 dmic_clk_a_0
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_SCLK(1) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 i2s1_sclk
};

// Native functions for Pad: xxgpp_s_3
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_s_3_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SNDW_DATA2(3) },     .PadMode = GpioV2PadModeNative1 }, // Function: 1 sndw3_data2
  { .Signal = { .Value = GPIOV2_SIGNAL_SNDW_DATA1(2) },     .PadMode = GpioV2PadModeNative2 }, // Function: 2 sndw2_data1
  { .Signal = { .Value = GPIOV2_SIGNAL_SNDW_DATA0(0) },     .PadMode = GpioV2PadModeNative3 }, // Function: 3 sndw0_data0
  { .Signal = { .Value = GPIOV2_SIGNAL_DMIC_DATA(0) },      .PadMode = GpioV2PadModeNative5 }, // Function: 5 dmic_data_0
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_SFRM(1) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 i2s1_sfrm
};

// Native functions for Pad: xxgpp_s_4
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_s_4_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SNDW_CLK(2) },       .PadMode = GpioV2PadModeNative2 }, // Function: 2 sndw2_clk
  { .Signal = { .Value = GPIOV2_SIGNAL_DMIC_CLKA(0) },      .PadMode = GpioV2PadModeNative5 }, // Function: 5 dmic_clk_a_0
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_SCLK(2) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 i2s2_sclk
};

// Native functions for Pad: xxgpp_s_5
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_s_5_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SNDW_DATA0(2) },     .PadMode = GpioV2PadModeNative2 }, // Function: 2 sndw2_data0
  { .Signal = { .Value = GPIOV2_SIGNAL_DMIC_DATA(0) },      .PadMode = GpioV2PadModeNative5 }, // Function: 5 dmic_data_0
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_SFRM(2) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 i2s2_sfrm
};

// Native functions for Pad: xxgpp_s_6
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_s_6_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SNDW_DATA1(2) },     .PadMode = GpioV2PadModeNative2 }, // Function: 2 sndw2_data1
  { .Signal = { .Value = GPIOV2_SIGNAL_SNDW_CLK(1) },       .PadMode = GpioV2PadModeNative3 }, // Function: 3 sndw1_clk
  { .Signal = { .Value = GPIOV2_SIGNAL_DMIC_CLKA(1) },      .PadMode = GpioV2PadModeNative5 }, // Function: 5 dmic_clk_a_1
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_TXD(2) },        .PadMode = GpioV2PadModeNative6 }  // Function: 6 i2s2_txd
};

// Native functions for Pad: xxgpp_s_7
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_s_7_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_SNDW_DATA3(3) },     .PadMode = GpioV2PadModeNative1 }, // Function: 1 sndw3_data3
  { .Signal = { .Value = GPIOV2_SIGNAL_SNDW_DATA2(2) },     .PadMode = GpioV2PadModeNative2 }, // Function: 2 sndw2_data2
  { .Signal = { .Value = GPIOV2_SIGNAL_SNDW_DATA0(1) },     .PadMode = GpioV2PadModeNative3 }, // Function: 3 sndw1_data0
  { .Signal = { .Value = GPIOV2_SIGNAL_DMIC_DATA(1) },      .PadMode = GpioV2PadModeNative5 }, // Function: 5 dmic_data_1
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_RXD(2) },        .PadMode = GpioV2PadModeNative6 }  // Function: 6 i2s2_rxd
};

// Native functions for Pad: xxgpp_b_0
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_0_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_USBC_SMLCLK },       .PadMode = GpioV2PadModeNative1 }, // Function: 1 usbc_smlclk
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(0) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPB_0
};

// Native functions for Pad: xxgpp_b_1
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_1_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_USBC_SMLDATA },      .PadMode = GpioV2PadModeNative1 }, // Function: 1 usbc_smldata
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(1) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPB_1
};

// Native functions for Pad: xxgpp_b_2
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_2_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_I2C_SDA(0) },    .PadMode = GpioV2PadModeNative3 }, // Function: 3 ish_i2c0_sda
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_I3C_SDA(0) },    .PadMode = GpioV2PadModeNative4 }, // Function: 4 ish_i3c0_sda
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(2) },       .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPB_2
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH3_DATA(0) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch3_d0
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I2C_SDA(2) },   .PadMode = GpioV2PadModeNative8 }, // Function: 8 a_i2c2_sda
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(16) }, .PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d00
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_DEBUG(0) },     .PadMode = GpioV2PadModeNative10 }  // Function: 10 cnv_debug_00
};

// Native functions for Pad: xxgpp_b_3
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_3_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_I2C_SCL(0) },    .PadMode = GpioV2PadModeNative3 }, // Function: 3 ish_i2c0_scl
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_I3C_SCL(0) },    .PadMode = GpioV2PadModeNative4 }, // Function: 4 ish_i3c0_scl
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(3) },       .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPB_3
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH3_DATA(1) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch3_d1
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I2C_SCL(2) },   .PadMode = GpioV2PadModeNative8 }, // Function: 8 a_i2c2_scl
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(17) }, .PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d01
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_DEBUG(1) },     .PadMode = GpioV2PadModeNative10 }  // Function: 10 cnv_debug_01
};

// Native functions for Pad: xxgpp_b_4
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_4_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_GP(0) },         .PadMode = GpioV2PadModeNative4 }, // Function: 4 ish_gp_0
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(4) },       .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPB_4
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH3_DATA(2) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch3_d2
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(18) }, .PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d02
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_DEBUG(2) },     .PadMode = GpioV2PadModeNative10 }  // Function: 10 cnv_debug_02
};

// Native functions for Pad: xxgpp_b_5
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_5_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_GP(1) },         .PadMode = GpioV2PadModeNative4 }, // Function: 4 ish_gp_1
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(5) },       .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPB_5
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH3_DATA(3) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch3_d3
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(19) }, .PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d03
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_DEBUG(3) },     .PadMode = GpioV2PadModeNative10 }  // Function: 10 cnv_debug_03
};

// Native functions for Pad: xxgpp_b_6
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_6_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_GP(2) },         .PadMode = GpioV2PadModeNative4 }, // Function: 4 ish_gp_2
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(6) },       .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPB_6
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH3_DATA(4) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch3_d4
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(20) }, .PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d04
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_DEBUG(4) },     .PadMode = GpioV2PadModeNative10 }  // Function: 10 cnv_debug_04
};

// Native functions for Pad: xxgpp_b_7
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_7_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_GP(3) },         .PadMode = GpioV2PadModeNative4 }, // Function: 4 ish_gp_3
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(7) },       .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPB_7
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH3_DATA(5) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch3_d5
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(21) }, .PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d05
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_DEBUG(5) },     .PadMode = GpioV2PadModeNative10 }  // Function: 10 cnv_debug_05
};

// Native functions for Pad: xxgpp_b_8
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_8_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_GP(4) },         .PadMode = GpioV2PadModeNative4 }, // Function: 4 ish_gp_4
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(8) },       .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPB_8
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH3_DATA(6) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch3_d6
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(22) }, .PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d06
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_DEBUG(6) },     .PadMode = GpioV2PadModeNative10 }  // Function: 10 cnv_debug_06
};

// Native functions for Pad: xxgpp_b_9
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_9_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_DDSP_HPD(1) },       .PadMode = GpioV2PadModeNative2 }, // Function: 2 ddsp_hpd1
  { .Signal = { .Value = GPIOV2_SIGNAL_DISP_MISC(1) },      .PadMode = GpioV2PadModeNative3 }, // Function: 3 disp_misc1
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(9) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPB_9
};

// Native functions for Pad: xxgpp_b_10
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_10_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_DDSP_HPD(2) },       .PadMode = GpioV2PadModeNative2 }, // Function: 2 ddsp_hpd2
  { .Signal = { .Value = GPIOV2_SIGNAL_DISP_MISC(2) },      .PadMode = GpioV2PadModeNative3 }, // Function: 3 disp_misc2
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(10) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPB_10
};

// Native functions for Pad: xxgpp_b_11
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_11_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_USB_OC(1) },         .PadMode = GpioV2PadModeNative1 }, // Function: 1 usb2_oc1_b
  { .Signal = { .Value = GPIOV2_SIGNAL_DDSP_HPD(3) },       .PadMode = GpioV2PadModeNative2 }, // Function: 2 ddsp_hpd3
  { .Signal = { .Value = GPIOV2_SIGNAL_DISP_MISC(3) },      .PadMode = GpioV2PadModeNative3 }, // Function: 3 disp_misc3
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(11) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPB_11
};

// Native functions for Pad: xxgpp_b_12
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_12_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_SLP(0) },        .PadMode = GpioV2PadModeNative1 }, // Function: 1 slp_s0_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(12) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPB_12
};

// Native functions for Pad: xxgpp_b_13
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_13_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PWR_PLTRST },        .PadMode = GpioV2PadModeNative1 }, // Function: 1 pltrst_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(13) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPB_13
};

// Native functions for Pad: xxgpp_b_14
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_14_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_USB_OC(2) },         .PadMode = GpioV2PadModeNative1 }, // Function: 1 usb2_oc2_b
  { .Signal = { .Value = GPIOV2_SIGNAL_DDSP_HPD(4) },       .PadMode = GpioV2PadModeNative2 }, // Function: 2 ddsp_hpd4
  { .Signal = { .Value = GPIOV2_SIGNAL_DISP_MISC(4) },      .PadMode = GpioV2PadModeNative3 }, // Function: 3 disp_misc4
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(14) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPB_14
};

// Native functions for Pad: xxgpp_b_15
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_15_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_USB_OC(3) },         .PadMode = GpioV2PadModeNative1 }, // Function: 1 usb2_oc3_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(15) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPB_15
};

// Native functions for Pad: xxgpp_b_16
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_16_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_TBT_LSX_OE(1) },     .PadMode = GpioV2PadModeNative1 }, // Function: 1 tbt_lsx1_oe
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(16) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPB_16
};

// Native functions for Pad: xxgpp_b_17
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_17_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_TBT_LSX_OE(0) },     .PadMode = GpioV2PadModeNative1 }, // Function: 1 tbt_lsx0_oe
  { .Signal = { .Value = GPIOV2_SIGNAL_PANEL_AVDD_EN2 },    .PadMode = GpioV2PadModeNative2 }, // Function: 2 l_vdden2
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(17) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPB_17
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH4_DATA(0) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch4_d0
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(23) }, .PadMode = GpioV2PadModeNative9 }  // Function: 9 pti_trace_d08
};

// Native functions for Pad: xxgpp_b_18
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_18_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_I2C_SDA(2) },    .PadMode = GpioV2PadModeNative1 }, // Function: 1 ish_i2c2_sda
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I2C_SDA(4) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 a_i2c4_sda
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_MFUART_RXD(0) },.PadMode = GpioV2PadModeNative3 }, // Function: 3 cnv_mfuart0_rxd
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(18) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPB_18
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH3_DATA(7) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch3_d7
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(24) }, .PadMode = GpioV2PadModeNative9 }, // Function: 9 pti_trace_d07
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_DEBUG(7) },     .PadMode = GpioV2PadModeNative10 }  // Function: 10 cnv_debug_07
};

// Native functions for Pad: xxgpp_b_19
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_19_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_I2C_SCL(2) },    .PadMode = GpioV2PadModeNative1 }, // Function: 1 ish_i2c2_scl
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I2C_SCL(4) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 a_i2c4_scl
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_MFUART_TXD(0) },.PadMode = GpioV2PadModeNative3 }, // Function: 3 cnv_mfuart0_txd
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(19) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPB_19
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH3_CLK },      .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch3_clk
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_CLK(0) },  .PadMode = GpioV2PadModeNative9 }  // Function: 9 pti_trace_clk
};

// Native functions for Pad: xxgpp_b_20
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_20_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I2C_SDA(5) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 a_i2c5_sda
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_MFUART_RTS(0) },.PadMode = GpioV2PadModeNative3 }, // Function: 3 cnv_mfuart0_rts_b
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_GP(8) },         .PadMode = GpioV2PadModeNative4 }, // Function: 4 ish_gp_8
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(20) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPB_20
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH4_DATA(1) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch4_d1
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(25) }, .PadMode = GpioV2PadModeNative9 }  // Function: 9 pti_trace_d09
};

// Native functions for Pad: xxgpp_b_21
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_21_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I2C_SCL(5) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 a_i2c5_scl
  { .Signal = { .Value = GPIOV2_SIGNAL_CNVI_MFUART_CTS(0) },.PadMode = GpioV2PadModeNative3 }, // Function: 3 cnv_mfuart0_cts_b
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_GP(9) },         .PadMode = GpioV2PadModeNative4 }, // Function: 4 ish_gp_9
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(21) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPB_21
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH4_DATA(2) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch4_d2
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(26) },.PadMode = GpioV2PadModeNative9 }  // Function: 9 pti_trace_d10
};

// Native functions for Pad: xxgpp_b_22
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_22_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_TIME_SYNC(0) },      .PadMode = GpioV2PadModeNative1 }, // Function: 1 time_sync_0
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_GP(5) },         .PadMode = GpioV2PadModeNative4 }, // Function: 4 ish_gp_5
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(22) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPB_22
};

// Native functions for Pad: xxgpp_b_23
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_23_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_TIME_SYNC(1) },      .PadMode = GpioV2PadModeNative1 }, // Function: 1 time_sync_1
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_GP(6) },         .PadMode = GpioV2PadModeNative4 }, // Function: 4 ish_gp_6
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(23) },      .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPB_23
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH4_DATA(3) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch4_d3
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(27) },.PadMode = GpioV2PadModeNative9 }  // Function: 9 pti_trace_d11
};

// Native functions for Pad: xxgpp_b_24
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_24_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ESPI_ALERT(0) },     .PadMode = GpioV2PadModeNative1 }, // Function: 1 espi_alert0_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(24) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPB_24
};

// Native functions for Pad: xxgpp_b_25
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_b_25_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ESPI_ALERT(1) },     .PadMode = GpioV2PadModeNative1 }, // Function: 1 espi_alert1_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPB(25) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPB_25
};

// Native functions for Pad: ishi3c0_clk_loopbk
GPIOV2_PAD_SIGNAL PtlPcdPch0_ishi3c0_clk_loopbk_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_I3C_SCL_FB(0) }, .PadMode = GpioV2PadModeNative4 }  // Function: 4 ish_i3c0_clk_loopbk
};

// Native functions for Pad: xxgpp_d_0
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_0_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_IPU_IMGCLKOUT(1) },  .PadMode = GpioV2PadModeNative1 }, // Function: 1 imgclkout_1
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(0) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPD_0
};

// Native functions for Pad: xxgpp_d_1
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_1_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I2C_SDA(3) },   .PadMode = GpioV2PadModeNative1 }, // Function: 1 a_i2c3_sda
  { .Signal = { .Value = GPIOV2_SIGNAL_PANEL_BKLTEN2 },     .PadMode = GpioV2PadModeNative2 }, // Function: 2 l_bklten2
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_I2C_SDA(2) },    .PadMode = GpioV2PadModeNative3 }, // Function: 3 a_ish_i2c2_sda
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(1) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPD_1
};

// Native functions for Pad: xxgpp_d_2
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_2_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_LPSS_I2C_SCL(3) },   .PadMode = GpioV2PadModeNative1 }, // Function: 1 a_i2c3_scl
  { .Signal = { .Value = GPIOV2_SIGNAL_PANEL_BKLTCTL2 },    .PadMode = GpioV2PadModeNative2 }, // Function: 2 l_bkltctl2
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_I2C_SCL(2) },    .PadMode = GpioV2PadModeNative3 }, // Function: 3 a_ish_i2c2_scl
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(2) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPD_2
};

// Native functions for Pad: xxgpp_d_3
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_3_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_CPU_GP(1) },         .PadMode = GpioV2PadModeNative1 }, // Function: 1 cpu_gp_1
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(3) },       .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPD_3
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH4_DATA(4) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch4_d4
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(28) },.PadMode = GpioV2PadModeNative9 }  // Function: 9 pti_trace_d12
};

// Native functions for Pad: xxgpp_d_4
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_4_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_IPU_IMGCLKOUT(0) },  .PadMode = GpioV2PadModeNative1 }, // Function: 1 imgclkout_0
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(4) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPD_4
};

// Native functions for Pad: xxgpp_d_5
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_5_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_UART_RX(0) },    .PadMode = GpioV2PadModeNative2 }, // Function: 2 ish_uart0_rxd
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_SPI_CS },        .PadMode = GpioV2PadModeNative3 }, // Function: 3 ish_spi_cs_b
  { .Signal = { .Value = GPIOV2_SIGNAL_SMB_SML_BDATA(0) },  .PadMode = GpioV2PadModeNative5 }, // Function: 5 sml0bdata
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(5) },       .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPD_5
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH4_DATA(5) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch4_d5
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(29) },.PadMode = GpioV2PadModeNative9 }  // Function: 9 pti_trace_d13
};

// Native functions for Pad: xxgpp_d_6
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_6_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_UART_TX(0) },    .PadMode = GpioV2PadModeNative2 }, // Function: 2 ish_uart0_txd
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_SPI_CLK },       .PadMode = GpioV2PadModeNative3 }, // Function: 3 ish_spi_clk
  { .Signal = { .Value = GPIOV2_SIGNAL_SMB_SML_BCLK(0) },   .PadMode = GpioV2PadModeNative5 }, // Function: 5 sml0bclk
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(6) },       .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPD_6
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH4_DATA(6) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch4_d6
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(30) },.PadMode = GpioV2PadModeNative9 }  // Function: 9 pti_trace_d14
};

// Native functions for Pad: xxgpp_d_7
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_7_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_UART_RTS(0) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 ish_uart0_rts_b
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_SPI_MISO },      .PadMode = GpioV2PadModeNative3 }, // Function: 3 ish_spi_miso
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(7) },       .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPD_7
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH4_DATA(7) },  .PadMode = GpioV2PadModeNative7 }, // Function: 7 visa2ch4_d7
  { .Signal = { .Value = GPIOV2_SIGNAL_PTI_TRACE_DATA(31) },.PadMode = GpioV2PadModeNative9 }  // Function: 9 pti_trace_d15
};

// Native functions for Pad: xxgpp_d_8
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_8_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_UART_CTS(0) },   .PadMode = GpioV2PadModeNative2 }, // Function: 2 ish_uart0_cts_b
  { .Signal = { .Value = GPIOV2_SIGNAL_ISH_SPI_MOSI },      .PadMode = GpioV2PadModeNative3 }, // Function: 3 ish_spi_mosi
  { .Signal = { .Value = GPIOV2_SIGNAL_SMB_SML_BALERT(0) }, .PadMode = GpioV2PadModeNative5 }, // Function: 5 sml0balert_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(8) },       .PadMode = GpioV2PadModeNative6 }, // Function: 6 IOM_GPPD_8
  { .Signal = { .Value = GPIOV2_SIGNAL_VISA2CH4_CLK },      .PadMode = GpioV2PadModeNative7 }  // Function: 7 visa2ch4_clk
};

// Native functions for Pad: xxgpp_d_9
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_9_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_MCLK },          .PadMode = GpioV2PadModeNative2 }, // Function: 2 i2s_mclk1_out
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(9) },       .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPD_9
};

// Native functions for Pad: xxgpp_d_10
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_10_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_HDA_BCLK },          .PadMode = GpioV2PadModeNative1 }, // Function: 1 hda_bclk
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_SCLK(0) },       .PadMode = GpioV2PadModeNative2 }, // Function: 2 i2s0_sclk
  { .Signal = { .Value = GPIOV2_SIGNAL_HDA_CPU_BCLK },      .PadMode = GpioV2PadModeNative4 }, // Function: 4 hdacpu_bclk
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(10) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPD_10
};

// Native functions for Pad: xxgpp_d_11
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_11_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_HDA_SYNC },          .PadMode = GpioV2PadModeNative1 }, // Function: 1 hda_sync
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_SFRM(0) },       .PadMode = GpioV2PadModeNative2 }, // Function: 2 i2s0_sfrm
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(11) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPD_11
};

// Native functions for Pad: xxgpp_d_12
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_12_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_HDA_SDO },           .PadMode = GpioV2PadModeNative1 }, // Function: 1 hda_sdo
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_TXD(0) },        .PadMode = GpioV2PadModeNative2 }, // Function: 2 i2s0_txd
  { .Signal = { .Value = GPIOV2_SIGNAL_HDA_CPU_SDO },       .PadMode = GpioV2PadModeNative4 }, // Function: 4 hdacpu_sdo
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(12) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPD_12
};

// Native functions for Pad: xxgpp_d_13
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_13_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_HDA_SDI(0) },        .PadMode = GpioV2PadModeNative1 }, // Function: 1 hda_sdi_0
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_RXD(0) },        .PadMode = GpioV2PadModeNative2 }, // Function: 2 i2s0_rxd
  { .Signal = { .Value = GPIOV2_SIGNAL_HDA_CPU_SDI },       .PadMode = GpioV2PadModeNative4 }, // Function: 4 hdacpu_sdi
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(13) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPD_13
};

// Native functions for Pad: xxgpp_d_14
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_14_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_TBT_LSX_OE(3) },     .PadMode = GpioV2PadModeNative2 }, // Function: 2 tbt_lsx3_oe
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(14) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPD_14
};

// Native functions for Pad: xxgpp_d_15
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_15_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(15) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPD_15
};

// Native functions for Pad: xxgpp_d_16
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_16_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_HDA_RSTB },          .PadMode = GpioV2PadModeNative1 }, // Function: 1 hda_rst_b
  { .Signal = { .Value = GPIOV2_SIGNAL_DMIC_CLKA(1) },      .PadMode = GpioV2PadModeNative3 }, // Function: 3 dmic_clk_a_1
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(16) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPD_16
};

// Native functions for Pad: xxgpp_d_17
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_17_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_HDA_SDI(1) },        .PadMode = GpioV2PadModeNative1 }, // Function: 1 hda_sdi_1
  { .Signal = { .Value = GPIOV2_SIGNAL_DMIC_DATA(1) },      .PadMode = GpioV2PadModeNative3 }, // Function: 3 dmic_data_1
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(17) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPD_17
};

// Native functions for Pad: xxgpp_d_18
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_18_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PCIE_SRCCLKREQ(6) }, .PadMode = GpioV2PadModeNative1 }, // Function: 1 srcclkreq6_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(18) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPD_18
};

// Native functions for Pad: xxgpp_d_19
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_19_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_TBT_LSX_OE(0) },     .PadMode = GpioV2PadModeNative2 }, // Function: 2 tbt_lsx0_oe
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(19) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPD_19
};

// Native functions for Pad: xxgpp_d_20
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_20_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_PCIE_SRCCLKREQ(7) }, .PadMode = GpioV2PadModeNative1 }, // Function: 1 srcclkreq7_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(20) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPD_20
};

// Native functions for Pad: xxgpp_d_21
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_21_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_UFS_REFCLK },        .PadMode = GpioV2PadModeNative1 }, // Function: 1 ufs_refclk
  { .Signal = { .Value = GPIOV2_SIGNAL_PCIE_SRCCLKREQ(8) }, .PadMode = GpioV2PadModeNative2 }, // Function: 2 srcclkreq8_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(21) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPD_21
};

// Native functions for Pad: xxgpp_d_22
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_22_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(22) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPD_22
};

// Native functions for Pad: xxgpp_d_23
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_23_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(23) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPD_23
};

// Native functions for Pad: xxgpp_d_24
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_24_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ESPI_ALERT(2) },     .PadMode = GpioV2PadModeNative1 }, // Function: 1 espi_alert2_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(24) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPD_24
};

// Native functions for Pad: xxgpp_d_25
GPIOV2_PAD_SIGNAL PtlPcdPch0_xxgpp_d_25_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_ESPI_ALERT(3) },     .PadMode = GpioV2PadModeNative1 }, // Function: 1 espi_alert3_b
  { .Signal = { .Value = GPIOV2_SIGNAL_IOM_GPPD(25) },      .PadMode = GpioV2PadModeNative6 }  // Function: 6 IOM_GPPD_25
};

// Native functions for Pad: vGPIO_30
GPIOV2_PAD_SIGNAL PtlPcdPch0_vGPIO_30_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_SCLK(0) },       .PadMode = GpioV2PadModeNative1 }, // Function: 1 i2s0_sclk
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_SCLK(1) },       .PadMode = GpioV2PadModeNative2 }, // Function: 2 i2s1_sclk
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_SCLK(2) },       .PadMode = GpioV2PadModeNative3 }  // Function: 3 i2s2_sclk
};

// Native functions for Pad: vGPIO_31
GPIOV2_PAD_SIGNAL PtlPcdPch0_vGPIO_31_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_SFRM(0) },       .PadMode = GpioV2PadModeNative1 }, // Function: 1 i2s0_sfrm
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_SFRM(1) },       .PadMode = GpioV2PadModeNative2 }, // Function: 2 i2s1_sfrm
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_SFRM(2) },       .PadMode = GpioV2PadModeNative3 }  // Function: 3 i2s2_sfrm
};

// Native functions for Pad: vGPIO_32
GPIOV2_PAD_SIGNAL PtlPcdPch0_vGPIO_32_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_RXD(0) },        .PadMode = GpioV2PadModeNative1 }, // Function: 1 i2s0_rxd
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_RXD(1) },        .PadMode = GpioV2PadModeNative2 }, // Function: 2 i2s1_rxd
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_RXD(2) },        .PadMode = GpioV2PadModeNative3 }  // Function: 3 i2s2_rxd
};

// Native functions for Pad: vGPIO_33
GPIOV2_PAD_SIGNAL PtlPcdPch0_vGPIO_33_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_TXD(0) },        .PadMode = GpioV2PadModeNative1 }, // Function: 1 i2s0_txd
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_TXD(1) },        .PadMode = GpioV2PadModeNative2 }, // Function: 2 i2s1_txd
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_TXD(2) },        .PadMode = GpioV2PadModeNative3 }  // Function: 3 i2s2_txd
};

// Native functions for Pad: vGPIO_40
GPIOV2_PAD_SIGNAL PtlPcdPch0_vGPIO_40_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_SCLK(0) },       .PadMode = GpioV2PadModeNative1 }, // Function: 1 i2s0_sclk
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_SCLK(1) },       .PadMode = GpioV2PadModeNative2 }, // Function: 2 i2s1_sclk
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_SCLK(2) },       .PadMode = GpioV2PadModeNative3 }  // Function: 3 i2s2_sclk
};

// Native functions for Pad: vGPIO_41
GPIOV2_PAD_SIGNAL PtlPcdPch0_vGPIO_41_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_SFRM(0) },       .PadMode = GpioV2PadModeNative1 }, // Function: 1 i2s0_sfrm
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_SFRM(1) },       .PadMode = GpioV2PadModeNative2 }, // Function: 2 i2s1_sfrm
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_SFRM(2) },       .PadMode = GpioV2PadModeNative3 }  // Function: 3 i2s2_sfrm
};

// Native functions for Pad: vGPIO_42
GPIOV2_PAD_SIGNAL PtlPcdPch0_vGPIO_42_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_RXD(0) },        .PadMode = GpioV2PadModeNative1 }, // Function: 1 i2s0_rxd
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_RXD(1) },        .PadMode = GpioV2PadModeNative2 }, // Function: 2 i2s1_rxd
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_RXD(2) },        .PadMode = GpioV2PadModeNative3 }  // Function: 3 i2s2_rxd
};

// Native functions for Pad: vGPIO_43
GPIOV2_PAD_SIGNAL PtlPcdPch0_vGPIO_43_Signals[] = {
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_TXD(0) },        .PadMode = GpioV2PadModeNative1 }, // Function: 1 i2s0_txd
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_TXD(1) },        .PadMode = GpioV2PadModeNative2 }, // Function: 2 i2s1_txd
  { .Signal = { .Value = GPIOV2_SIGNAL_I2S_TXD(2) },        .PadMode = GpioV2PadModeNative3 }  // Function: 3 i2s2_txd
};


//
// Pad data for group: GPP_V
//
GPIOV2_PAD_DATA PtlPcdPch0Community0Group0PadData[] = {
  { .PadSignals = PtlPcdPch0_xxgpp_v_0_Signals,        .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_v_1_Signals,        .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_v_2_Signals,        .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_v_3_Signals,        .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_v_4_Signals,        .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_v_5_Signals,        .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_v_6_Signals,        .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_v_7_Signals,        .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_v_8_Signals,        .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_v_9_Signals,        .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_v_10_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_v_11_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_v_12_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_v_13_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_v_14_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_v_15_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_v_16_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = NULL,                                .PadSignalsNum = 0  },
  { .PadSignals = PtlPcdPch0_xxsys_pwrok_Signals,      .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxsys_reset_b_Signals,    .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxl_bklten_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxl_bkltctl_Signals,      .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxl_vdden_Signals,        .PadSignalsNum = 1  },
  { .PadSignals = NULL,                                .PadSignalsNum = 0  }
};

//
// Pad data for group: GPP_C
//
GPIOV2_PAD_DATA PtlPcdPch0Community0Group1PadData[] = {
  { .PadSignals = PtlPcdPch0_xxgpp_c_0_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_c_1_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_c_2_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_c_3_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_c_4_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_c_5_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_c_6_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_c_7_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_c_8_Signals,        .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_c_9_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_c_10_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_c_11_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_c_12_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_c_13_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_c_14_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_c_15_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_c_16_Signals,       .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_c_17_Signals,       .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_c_18_Signals,       .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_c_19_Signals,       .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_c_20_Signals,       .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_c_21_Signals,       .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_c_22_Signals,       .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_c_23_Signals,       .PadSignalsNum = 3  }
};

//
// Pad data for group: GPP_F
//
GPIOV2_PAD_DATA PtlPcdPch0Community1Group0PadData[] = {
  { .PadSignals = PtlPcdPch0_xxgpp_f_0_Signals,        .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_f_1_Signals,        .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_f_2_Signals,        .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_f_3_Signals,        .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_f_4_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_f_5_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_f_6_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_f_7_Signals,        .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_f_8_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_f_9_Signals,        .PadSignalsNum = 4  },
  { .PadSignals = PtlPcdPch0_xxgpp_f_10_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_f_11_Signals,       .PadSignalsNum = 7  },
  { .PadSignals = PtlPcdPch0_xxgpp_f_12_Signals,       .PadSignalsNum = 10 },
  { .PadSignals = PtlPcdPch0_xxgpp_f_13_Signals,       .PadSignalsNum = 10 },
  { .PadSignals = PtlPcdPch0_xxgpp_f_14_Signals,       .PadSignalsNum = 6  },
  { .PadSignals = PtlPcdPch0_xxgpp_f_15_Signals,       .PadSignalsNum = 6  },
  { .PadSignals = PtlPcdPch0_xxgpp_f_16_Signals,       .PadSignalsNum = 6  },
  { .PadSignals = PtlPcdPch0_xxgpp_f_17_Signals,       .PadSignalsNum = 7  },
  { .PadSignals = PtlPcdPch0_xxgpp_f_18_Signals,       .PadSignalsNum = 6  },
  { .PadSignals = PtlPcdPch0_xxgpp_f_19_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_f_20_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_f_21_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_f_22_Signals,       .PadSignalsNum = 7  },
  { .PadSignals = PtlPcdPch0_xxgpp_f_23_Signals,       .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_thc1_gspi1_i3c2_clk_loopbk_Signals,  .PadSignalsNum = 2  },
  { .PadSignals = NULL,                                .PadSignalsNum = 0  }
};

//
// Pad data for group: GPP_E
//
GPIOV2_PAD_DATA PtlPcdPch0Community1Group1PadData[] = {
  { .PadSignals = PtlPcdPch0_xxgpp_e_0_Signals,        .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_e_1_Signals,        .PadSignalsNum = 4  },
  { .PadSignals = PtlPcdPch0_xxgpp_e_2_Signals,        .PadSignalsNum = 4  },
  { .PadSignals = PtlPcdPch0_xxgpp_e_3_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_e_4_Signals,        .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_e_5_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_e_6_Signals,        .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_e_7_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_e_8_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_e_9_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_e_10_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_e_11_Signals,       .PadSignalsNum = 5  },
  { .PadSignals = PtlPcdPch0_xxgpp_e_12_Signals,       .PadSignalsNum = 9  },
  { .PadSignals = PtlPcdPch0_xxgpp_e_13_Signals,       .PadSignalsNum = 9  },
  { .PadSignals = PtlPcdPch0_xxgpp_e_14_Signals,       .PadSignalsNum = 6  },
  { .PadSignals = PtlPcdPch0_xxgpp_e_15_Signals,       .PadSignalsNum = 6  },
  { .PadSignals = PtlPcdPch0_xxgpp_e_16_Signals,       .PadSignalsNum = 6  },
  { .PadSignals = PtlPcdPch0_xxgpp_e_17_Signals,       .PadSignalsNum = 7  },
  { .PadSignals = PtlPcdPch0_xxgpp_e_18_Signals,       .PadSignalsNum = 6  },
  { .PadSignals = PtlPcdPch0_xxgpp_e_19_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_e_20_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_e_21_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_e_22_Signals,       .PadSignalsNum = 6  },
  { .PadSignals = PtlPcdPch0_xxboothalt_b_Signals,     .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_thc0_gspi0_clk_loopbk_Signals,  .PadSignalsNum = 1  }
};

//
// Pad data for group: CPUJTAG
//
GPIOV2_PAD_DATA PtlPcdPch0Community3Group0PadData[] = {
  { .PadSignals = PtlPcdPch0_xxepd_on_Signals,         .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxvdd2_pwrgd_Signals,     .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxjtag_mbpb0_Signals,     .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxjtag_mbpb1_Signals,     .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxjtag_mbpb2_Signals,     .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxjtag_mbpb3_Signals,     .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxjtag_tdo_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxprdy_b_Signals,         .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxpreq_b_Signals,         .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxjtag_tdi_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxjtag_tms_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxjtag_tck_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = NULL,                                .PadSignalsNum = 0  },
  { .PadSignals = PtlPcdPch0_xxjtag_trst_b_Signals,    .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxddsp_hpdalv_Signals,    .PadSignalsNum = 2  }
};

//
// Pad data for group: GPP_H
//
GPIOV2_PAD_DATA PtlPcdPch0Community3Group1PadData[] = {
  { .PadSignals = PtlPcdPch0_xxgpp_h_0_Signals,        .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_1_Signals,        .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_2_Signals,        .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_3_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_4_Signals,        .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_5_Signals,        .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_6_Signals,        .PadSignalsNum = 4  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_7_Signals,        .PadSignalsNum = 4  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_8_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_9_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_10_Signals,       .PadSignalsNum = 4  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_11_Signals,       .PadSignalsNum = 4  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_12_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_13_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_14_Signals,       .PadSignalsNum = 5  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_15_Signals,       .PadSignalsNum = 5  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_16_Signals,       .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_17_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_18_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_19_Signals,       .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_20_Signals,       .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_21_Signals,       .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_22_Signals,       .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_23_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_h_24_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_lpi3c1_clk_loopbk_Signals,  .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_lpi3c0_clk_loopbk_Signals,  .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_ishi3c1_clk_loopbk_Signals,  .PadSignalsNum = 1  }
};

//
// Pad data for group: GPPASPI0
//
GPIOV2_PAD_DATA PtlPcdPch0Community3Group2PadData[] = {
  { .PadSignals = PtlPcdPch0_xxgpp_a_0_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_a_1_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_a_2_Signals,        .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_a_3_Signals,        .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_a_4_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_a_5_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_a_6_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_a_7_Signals,        .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_a_8_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_a_9_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_a_10_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_a_11_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_a_12_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_a_13_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_a_14_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_a_15_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_a_16_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_a_17_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxspi0_io_2_Signals,      .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxspi0_io_3_Signals,      .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxspi0_mosi_io_0_Signals,  .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxspi0_miso_io_1_Signals,  .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxspi0_tpm_cs_b_Signals,  .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxspi0_flash_0_cs_b_Signals,  .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxspi0_flash_1_cs_b_Signals,  .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxspi0_clk_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = NULL,                                .PadSignalsNum = 0  },
  { .PadSignals = NULL,                                .PadSignalsNum = 0  }
};

//
// Pad data for group: vGPIO_3
//
GPIOV2_PAD_DATA PtlPcdPch0Community3Group3PadData[] = {
  { .PadSignals = PtlPcdPch0_vGPIO_USB_0_Signals,      .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_vGPIO_USB_1_Signals,      .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_vGPIO_USB_2_Signals,      .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_vGPIO_USB_3_Signals,      .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_vGPIO_USB_4_Signals,      .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_vGPIO_USB_5_Signals,      .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_vGPIO_USB_6_Signals,      .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_vGPIO_USB_7_Signals,      .PadSignalsNum = 1  },
  { .PadSignals = NULL,                                .PadSignalsNum = 0  },
  { .PadSignals = NULL,                                .PadSignalsNum = 0  },
  { .PadSignals = NULL,                                .PadSignalsNum = 0  },
  { .PadSignals = NULL,                                .PadSignalsNum = 0  },
  { .PadSignals = NULL,                                .PadSignalsNum = 0  },
  { .PadSignals = NULL,                                .PadSignalsNum = 0  }
};

//
// Pad data for group: GPP_S
//
GPIOV2_PAD_DATA PtlPcdPch0Community4Group0PadData[] = {
  { .PadSignals = PtlPcdPch0_xxgpp_s_0_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_s_1_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_s_2_Signals,        .PadSignalsNum = 4  },
  { .PadSignals = PtlPcdPch0_xxgpp_s_3_Signals,        .PadSignalsNum = 5  },
  { .PadSignals = PtlPcdPch0_xxgpp_s_4_Signals,        .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_s_5_Signals,        .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_s_6_Signals,        .PadSignalsNum = 4  },
  { .PadSignals = PtlPcdPch0_xxgpp_s_7_Signals,        .PadSignalsNum = 5  }
};

//
// Pad data for group: GPP_B
//
GPIOV2_PAD_DATA PtlPcdPch0Community5Group0PadData[] = {
  { .PadSignals = PtlPcdPch0_xxgpp_b_0_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_1_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_2_Signals,        .PadSignalsNum = 7  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_3_Signals,        .PadSignalsNum = 7  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_4_Signals,        .PadSignalsNum = 5  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_5_Signals,        .PadSignalsNum = 5  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_6_Signals,        .PadSignalsNum = 5  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_7_Signals,        .PadSignalsNum = 5  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_8_Signals,        .PadSignalsNum = 5  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_9_Signals,        .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_10_Signals,       .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_11_Signals,       .PadSignalsNum = 4  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_12_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_13_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_14_Signals,       .PadSignalsNum = 4  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_15_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_16_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_17_Signals,       .PadSignalsNum = 5  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_18_Signals,       .PadSignalsNum = 7  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_19_Signals,       .PadSignalsNum = 6  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_20_Signals,       .PadSignalsNum = 6  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_21_Signals,       .PadSignalsNum = 6  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_22_Signals,       .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_23_Signals,       .PadSignalsNum = 5  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_24_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_b_25_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_ishi3c0_clk_loopbk_Signals,  .PadSignalsNum = 1  }
};

//
// Pad data for group: GPP_D
//
GPIOV2_PAD_DATA PtlPcdPch0Community5Group1PadData[] = {
  { .PadSignals = PtlPcdPch0_xxgpp_d_0_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_1_Signals,        .PadSignalsNum = 4  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_2_Signals,        .PadSignalsNum = 4  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_3_Signals,        .PadSignalsNum = 4  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_4_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_5_Signals,        .PadSignalsNum = 6  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_6_Signals,        .PadSignalsNum = 6  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_7_Signals,        .PadSignalsNum = 5  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_8_Signals,        .PadSignalsNum = 5  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_9_Signals,        .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_10_Signals,       .PadSignalsNum = 4  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_11_Signals,       .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_12_Signals,       .PadSignalsNum = 4  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_13_Signals,       .PadSignalsNum = 4  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_14_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_15_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_16_Signals,       .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_17_Signals,       .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_18_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_19_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_20_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_21_Signals,       .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_22_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_23_Signals,       .PadSignalsNum = 1  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_24_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = PtlPcdPch0_xxgpp_d_25_Signals,       .PadSignalsNum = 2  },
  { .PadSignals = NULL,                                .PadSignalsNum = 0  }
};

//
// Pad data for group: vGPIO
//
GPIOV2_PAD_DATA PtlPcdPch0Community5Group2PadData[] = {
  { .PadSignals = NULL,                                .PadSignalsNum = 0  },
  { .PadSignals = NULL,                                .PadSignalsNum = 0  },
  { .PadSignals = PtlPcdPch0_vGPIO_30_Signals,         .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_vGPIO_31_Signals,         .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_vGPIO_32_Signals,         .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_vGPIO_33_Signals,         .PadSignalsNum = 3  },
  { .PadSignals = NULL,                                .PadSignalsNum = 0  },
  { .PadSignals = NULL,                                .PadSignalsNum = 0  },
  { .PadSignals = NULL,                                .PadSignalsNum = 0  },
  { .PadSignals = NULL,                                .PadSignalsNum = 0  },
  { .PadSignals = PtlPcdPch0_vGPIO_40_Signals,         .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_vGPIO_41_Signals,         .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_vGPIO_42_Signals,         .PadSignalsNum = 3  },
  { .PadSignals = PtlPcdPch0_vGPIO_43_Signals,         .PadSignalsNum = 3  },
  { .PadSignals = NULL,                                .PadSignalsNum = 0  },
  { .PadSignals = NULL,                                .PadSignalsNum = 0  },
  { .PadSignals = NULL,                                .PadSignalsNum = 0  },
  { .PadSignals = NULL,                                .PadSignalsNum = 0  }
};
