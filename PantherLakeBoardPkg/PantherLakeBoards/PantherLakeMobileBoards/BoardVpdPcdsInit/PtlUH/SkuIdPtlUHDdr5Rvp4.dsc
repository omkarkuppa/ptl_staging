## @file
#  Board description file initializes configuration (PCD) settings for the project.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2024 Intel Corporation.
#
#  This software and the related documents are Intel copyrighted materials,
#  and your use of them is governed by the express license under which they
#  were provided to you ("License"). Unless the License provides otherwise,
#  you may not use, modify, copy, publish, distribute, disclose or transmit
#  this software or the related documents without Intel's prior written
#  permission.
#
#  This software and the related documents are provided as is, with no
#  express or implied warranties, other than those that are expressly stated
#  in the License.
#
# @par Specification
##
[Defines]
  PIN_GPIO_ACTIVE_HIGH            = 1
  PIN_GPIO_ACTIVE_LOW             = 0
  BOARD_REAL_BATTERY_SUPPORTED    = 1
  BOARD_VIRTUAL_BATTERY_SUPPORTED = 2
  BOARD_NO_BATTERY_SUPPORT        = 0

[Packages]
  PantherLakeBoardPkg/BoardPkg.dec

#####################################################################
#  PTL UH DDR5 T3 - RVP4
#     Board ID: 0x04 (BoardIdPtlUHDdr5Rvp4)
#   Display ID: 0x00
#       SKU ID: 0x00000004 (SkuIdPtlUHDdr5Rvp4)
#####################################################################
[PcdsDynamicExVpd.common.SkuIdPtlUHDdr5Rvp4]

  # PSS Read Serial Number
  gI2cFeaturePkgTokenSpaceGuid.VpdPcdPssReadSN|*|TRUE

  # PSS I2C Peripheral Address
  gI2cFeaturePkgTokenSpaceGuid.VpdPcdPssI2cDeviceAddress|*|0x6E

  # PSS I2C Bus Number
  gI2cFeaturePkgTokenSpaceGuid.VpdPcdPssI2cBusNumber|*|0x03

  # SPD Address Override
  gBoardModuleTokenSpaceGuid.VpdPcdSpdAddressOverride|*|FALSE

  # Usb2 Physical Tuning Table
  gBoardModuleTokenSpaceGuid.VpdPcdUsb2PhyTuningTable| * |{CODE(
  { 16,
    {{ 0x6, 0x0, 0x3, 0x0 },   // Port 1
     { 0x6, 0x0, 0x3, 0x0 },   // Port 2
     { 0x6, 0x0, 0x3, 0x0 },   // Port 3
     { 0x6, 0x0, 0x3, 0x0 },   // Port 4
     { 0x6, 0x0, 0x3, 0x0 },   // Port 5
     { 0x6, 0x0, 0x3, 0x0 },   // Port 6
     { 0x6, 0x0, 0x3, 0x0 },   // Port 7
     { 0x6, 0x0, 0x3, 0x0 },   // Port 8
     { 0x6, 0x0, 0x3, 0x0 },   // Port 9
     { 0x6, 0x0, 0x3, 0x0 },   // Port 10
     { 0x6, 0x0, 0x3, 0x0 },   // Port 11
     { 0x6, 0x0, 0x3, 0x0 },   // Port 12
     { 0x6, 0x0, 0x3, 0x0 },   // Port 13
     { 0x6, 0x0, 0x3, 0x0 },   // Port 14
     { 0x6, 0x0, 0x3, 0x0 },   // Port 15
     { 0x6, 0x0, 0x3, 0x0 }    // Port 16
    }
  })}

  # Default GPIO tier 1 groups are DW0 - GPP_F , DW1 - GPP_D , DW2 - GPP_A
  gBoardModuleTokenSpaceGuid.VpdPcdGpioGroupToGpeDw0|*|{CODE(
   { PTL_PCD_GROUP_GPP_F }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdGpioGroupToGpeDw1|*|{CODE(
   { PTL_PCD_GROUP_GPP_D }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdGpioGroupToGpeDw2|*|{CODE(
   { PTL_PCD_GROUP_GPPASPI0 }
  )}

  # GPIO Table WwanOn
  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableWwanOnEarlyPreMem| * |{CODE(
  {
    { GPIOV2_PTL_PCD_XXGPP_H_16, {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,     GpioV2IntDis,                GpioV2ResetResume,    GpioV2TermNone,  GpioV2Lock,   GpioV2Unlock }},  // WWAN_PWREN
    { GPIOV2_PTL_PCD_XXGPP_A_9,  {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,     GpioV2IntDis,                GpioV2ResetResume,    GpioV2TermNone,  GpioV2Lock,   GpioV2Unlock }},  // M.2_WWAN_FCP_OFF_N
    { GPIOV2_PTL_PCD_XXGPP_B_20, {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,     GpioV2IntDis,                GpioV2ResetResume,    GpioV2TermNone,  GpioV2Lock,   GpioV2Unlock }},  // M.2_WWAN_RST_N
    { GPIOV2_PTL_PCD_XXGPP_D_3,  {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,     GpioV2IntDis,                GpioV2ResetHost,      GpioV2TermNone,  GpioV2Lock,   GpioV2Unlock }},  // M.2_WWAN_PERST_GPIO_N
    { GPIOV2_PTL_PCD_XXGPP_E_2,  {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirInInv,  GpioV2StateDefault,  GpioV2IntLevel|GpioV2IntSci, GpioV2ResetHostDeep,  GpioV2TermNone,  GpioV2Unlock, GpioV2Lock   }},  // WWAN_WAKE_GPIO_N
    { GPIOV2_PTL_PCD_XXGPP_A_10, {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,     GpioV2IntDis,                GpioV2ResetResume,    GpioV2TermNone,  GpioV2Lock,   GpioV2Unlock }},  // M.2_WWAN_DISABLE_N
    { 0x0 }
  })}

  # GPIO Table WwanOff
  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableWwanOffEarlyPreMem| * |{CODE(
  {
    { GPIOV2_PTL_PCD_XXGPP_A_9,  {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateLow,      GpioV2IntDis,                GpioV2ResetHostDeep,  GpioV2TermNone,  GpioV2Lock,   GpioV2Unlock }},  // M.2_WWAN_FCP_OFF_N
    { GPIOV2_PTL_PCD_XXGPP_B_20, {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateLow,      GpioV2IntDis,                GpioV2ResetHostDeep,  GpioV2TermNone,  GpioV2Lock,   GpioV2Unlock }},  // M.2_WWAN_RST_N
    { GPIOV2_PTL_PCD_XXGPP_D_3,  {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateLow,      GpioV2IntDis,                GpioV2ResetHostDeep,  GpioV2TermNone,  GpioV2Lock,   GpioV2Unlock }},  // M.2_WWAN_PERST_GPIO_N
    { 0x0 }
  })}

  # GPIO Table M80 WwanOn
  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableM80WwanOnEarlyPreMem| * |{CODE(
  {
    { GPIOV2_PTL_PCD_XXGPP_H_16, {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                GpioV2ResetResume,    GpioV2TermNone,  GpioV2Lock,   GpioV2Unlock }}, // WWAN_PWREN
    { GPIOV2_PTL_PCD_XXGPP_A_9,  {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                GpioV2ResetResume,    GpioV2TermNone,  GpioV2Lock,   GpioV2Unlock }}, // M.2_WWAN_FCP_OFF_N
    { GPIOV2_PTL_PCD_XXGPP_B_20, {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateLow,     GpioV2IntDis,                GpioV2ResetResume,    GpioV2TermNone,  GpioV2Lock,   GpioV2Unlock }}, // M.2_WWAN_RST_N
    { GPIOV2_PTL_PCD_XXGPP_D_3,  {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateLow,     GpioV2IntDis,                GpioV2ResetHost,      GpioV2TermNone,  GpioV2Lock,   GpioV2Unlock }}, // M.2_WWAN_PERST_GPIO_N
    { GPIOV2_PTL_PCD_XXGPP_E_2,  {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirInInv,  GpioV2StateDefault, GpioV2IntLevel|GpioV2IntSci, GpioV2ResetHostDeep,  GpioV2TermNone,  GpioV2Unlock, GpioV2Lock   }}, // WWAN_WAKE_GPIO_N
    { GPIOV2_PTL_PCD_XXGPP_A_10, {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                GpioV2ResetResume,    GpioV2TermNone,  GpioV2Lock,   GpioV2Unlock }}, // M.2_WWAN_DISABLE_N
    { 0x0 }
  })}


  # MRC UserBd
  gBoardModuleTokenSpaceGuid.VpdPcdSaMiscUserBd|*|0x05

  # MRC Ddr Frequency Limit
  gBoardModuleTokenSpaceGuid.VpdPcdSaDdrFreqLimit|*|0x0

  # Mrc Spd Data Size
  gBoardModuleTokenSpaceGuid.VpdPcdMrcSpdDataSize|*|512

  # Mrc Spd Address Table
  gBoardModuleTokenSpaceGuid.VpdPcdMrcSpdAddressTable| * |{CODE(
  { 16,
    { 0xA0 ,   // MC0 C0 D0
      0x00 ,   // MC0 C0 D1
      0xA0 ,   // MC0 C1 D0
      0x00 ,   // MC0 C1 D1
      0x00 ,   // MC0 C2 D0 - unused for DDR5
      0x00 ,   // MC0 C2 D1 - unused for DDR5
      0x00 ,   // MC0 C3 D0 - unused for DDR5
      0x00 ,   // MC0 C3 D1 - unused for DDR5
      0xA4 ,   // MC1 C0 D0
      0x00 ,   // MC1 C0 D1
      0xA4 ,   // MC1 C1 D0
      0x00 ,   // MC1 C1 D1
      0x00 ,   // MC1 C2 D0 - unused for DDR5
      0x00 ,   // MC1 C2 D1 - unused for DDR5
      0x00 ,   // MC1 C3 D0 - unused for DDR5
      0x00     // MC1 C3 D1 - unused for DDR5
    }
  })}

  # Each DIMM Slot Mechanical present bit map, bit 0 -> DIMM 0, bit 1 -> DIMM1, ...
  # if the bit is 1, the related DIMM slot is present.
  # 0x1 - For 1DPC memory DIMM config, 0x3 - For 2DPC memory DIMM config.
  gBoardModuleTokenSpaceGuid.VpdPcdMrcNoOfDimmSlot|*|{CODE({
    {
      // Controller-0
      {
        0x1, // DIMM0
        0x1, // DIMM0
        0x1, // DIMM0
        0x1  // DIMM0
      },
      // Controller-1
      {
        0x1, // DIMM0
        0x1, // DIMM0
        0x1, // DIMM0
        0x1  // DIMM0
      }
    }
  })}

  # MRC BoardStackUp
  gBoardModuleTokenSpaceGuid.VpdPcdMrcBoardStackUp|*|0x0

  # MRC BoardTopology
  gBoardModuleTokenSpaceGuid.VpdPcdMrcBoardTopology|*|0x0

  # Mrc DQ Byte Map
  gBoardModuleTokenSpaceGuid.VpdPcdMrcDqByteMap| * |{CODE(
  {
    {{ // Channel 0:
     { 0x0F, 0xF0 }, // CLK0 goes to package 0 - Bytes[3:0], CLK1 goes to package 1 - Bytes[7:4]
     { 0x0F, 0xF0 }, // Cmd CAA goes to Bytes[3:0], Cmd CAB goes to Byte[7:4]
     { 0xFF, 0x00 }, // CTL (CS) goes to all bytes
     { 0x00, 0x00 }, // Unused in ICL MRC
     { 0x00, 0x00 }, // Unused in ICL MRC
     { 0x00, 0x00 }  // Unused in ICL MRC
     },
     { // Channel 1:
     { 0x0F, 0xF0 }, // CLK0 goes to package 0 - Bytes[3:0], CLK1 goes to package 1 - Bytes[7:4]
     { 0x0F, 0xF0 }, // Cmd CAA goes to Bytes[3:0], Cmd CAB goes to Byte[7:4]
     { 0xFF, 0x00 }, // CTL (CS) goes to all bytes
     { 0x00, 0x00 }, // Unused in ICL MRC
     { 0x00, 0x00 }, // Unused in ICL MRC
     { 0x00, 0x00 }  // Unused in ICL MRC
     }
    }
  })}

  # MRC Rcom Resistor. 0 for default Termination
  gBoardModuleTokenSpaceGuid.VpdPcdMrcRcompResistor|*|0x0

  # Mrc Rcom Target. 0 for default target
  gBoardModuleTokenSpaceGuid.VpdPcdMrcRcompTarget| * |{CODE(
  {  5,
     { 0x0, 0x0, 0x0, 0x0, 0x0 }
  })}

  # MRC DQ Pins InterLeaved Control. FALSE for NON-INTERLEAVED (NIL) ,TRUE for INTERLEAVED (IL)
  gBoardModuleTokenSpaceGuid.VpdPcdMrcDqPinsInterleavedControl|*|FALSE

  # MRC DQ Pins InterLeaved
  gBoardModuleTokenSpaceGuid.VpdPcdMrcDqPinsInterleaved|*|FALSE

  # Mrc LP5 CCC Configuration. 0 for Ascending and 0xFF for Descending
  gBoardModuleTokenSpaceGuid.VpdPcdMrcLp5CccConfig|*|0xFF
  #
  # CA Vref routing: board-dependent
  # 0 - VREF_CA goes to both CH_A and CH_B (LPDDR3/DDR3L)
  # 1 - VREF_CA to CH_A, VREF_DQ_A to CH_B (should not be used)
  # 2 - VREF_CA to CH_A, VREF_DQ_B to CH_B (DDR4)
  #
  # Set it to 2 for all our DDR4 boards; it is ignored for LPDDR4
  #
  # Mrc Ca Vref Configuration
  gBoardModuleTokenSpaceGuid.VpdPcdMrcCaVrefConfig|*|0x2

  # Mrc Cmd Mirror Configuration
  gBoardModuleTokenSpaceGuid.VpdPcdMrcCmdMirror|*|0x0

  # E-DVFSC Support
  gBoardModuleTokenSpaceGuid.VpdPcdVdd2lPowerRailPresent|*|0x1

  #Display DDi Config table
  gBoardModuleTokenSpaceGuid.VpdPcdDisplayDdiConfigTable| * |{CODE(
  { 16,
    {DdiPortEdp,         // DDI Port A Config : DdiPortDisabled = No LFP is Connected, DdiPortEdp = eDP, DdiPortMipiDsi = MIPI DSI
     DdiPortDisabled,    // DDI Port B Config : DdiPortDisabled = No LFP is Connected, DdiPortEdp = eDP, DdiPortMipiDsi = MIPI DSI
     DdiHpdDisable,      // DDI Port A HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdDisable,      // DDI Port B HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdDisable,      // DDI Port C HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdDisable,      // DDI Port 1 HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdDisable,      // DDI Port 2 HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdDisable,      // DDI Port 3 HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdDisable,      // DDI Port 4 HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiDisable,         // DDI Port A DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
     DdiDisable,         // DDI Port B DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
     DdiDisable,         // DDI Port C DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
     DdiDisable,         // DDI Port 1 DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
     DdiDisable,         // DDI Port 2 DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
     DdiDisable,         // DDI Port 3 DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
     DdiDisable          // DDI Port 4 DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
    }
  })}

  # Board Retimer Force Power Gpio
  gBoardModuleTokenSpaceGuid.VpdPcdBoardRetimerForcePwrGpio|*|{CODE(
    { GPIOV2_PTL_PCD_XXGPP_B_21 }   // TCP_RETIMER_FORCE_PWR
  )}

  # Recovery Mode Gpio
  # BoardGpioTypePch,BoardGpioTypeExpander,BoardGpioTypeNotSupported
  gBoardModuleTokenSpaceGuid.VpdPcdRecoveryModeGpio|*|{CODE(
   { BoardGpioTypeNotSupported }
  )}

  # WWAN Gpios
  gBoardModuleTokenSpaceGuid.VpdPcdWwanFullCardPowerOffGpio|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_A_9 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdWwanBbrstGpio|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_B_20}
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdWwanPerstGpio|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_D_3 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdTouchpanel0IrqGpio|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_E_18 } // THC0_SPI1_INT_N_TCH_PNL1
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdTouchpanel1IrqGpio|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_F_18 } // THC1_SPI2_INT_N_TCH_PNL2
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdTouchpadIrqGpio|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_F_18 } // TCH_PAD_INT_N
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdTouchpadIrqGpioPolarity|*|$(PIN_GPIO_ACTIVE_LOW)

  gBoardModuleTokenSpaceGuid.VpdPcdWwanFullCardPowerOffGpioPolarity|*|$(PIN_GPIO_ACTIVE_HIGH)
  gBoardModuleTokenSpaceGuid.VpdPcdWwanBbrstGpioPolarity|*|$(PIN_GPIO_ACTIVE_LOW)
  gBoardModuleTokenSpaceGuid.VpdPcdWwanPerstGpioPolarity|*|$(PIN_GPIO_ACTIVE_LOW)
  gBoardModuleTokenSpaceGuid.VpdPcdWwanSourceClock|*|5
  gBoardModuleTokenSpaceGuid.VpdPcdWwanRootPortNumber|*|3

  # GPIO Blink enable and PWM Control.
  gBoardModuleTokenSpaceGuid.VpdPcdPwmBlinkEnable|*|{CODE(
   { 0x0 } //ISH_GP_3_SNSR_HDR has rework SOC_PCBEEP
  )}

  # VPD PCD Ratio
  gBoardModuleTokenSpaceGuid.VpdPcdCpuRatio|*|0x0

  # BIOS Guard
  gBoardModuleTokenSpaceGuid.VpdPcdBiosGuard|*|0x0

  # Odd Power Init Enable
  gBoardModuleTokenSpaceGuid.VpdPcdOddPowerInitEnable|*|FALSE

  # Smbus Alert function Init.
  gBoardModuleTokenSpaceGuid.VpdPcdSmbusAlertEnable|*|FALSE

  # EC FailSafe Cpu Temp
  gBoardModuleTokenSpaceGuid.VpdPcdEcFailSafeActionCpuTemp|*|85

  # EC FailSafe Fan Speed Setting
  gBoardModuleTokenSpaceGuid.VpdPcdEcFailSafeActionFanPwm|*|65

  # Touch Panel1 Table
  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableTouchPanel0|*|{CODE({
  // Touch Panel 0, Same pins shared between THC and I2C based Panel,
    {GPIOV2_PTL_PCD_XXGPP_F_8,   {GpioV2PadModeGpio,  GpioV2HostOwnAcpi,  GpioV2DirOut,     GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHost,   GpioV2TermDefault}},  // TCH_PNL1_PWR_EN
    {GPIOV2_PTL_PCD_XXGPP_E_16,  {GpioV2PadModeGpio,  GpioV2HostOwnAcpi,  GpioV2DirOut,     GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHost,   GpioV2TermDefault}},  // THC0_SPI1_RST_N_TCH_PNL1
    {GPIOV2_PTL_PCD_XXGPP_E_18,  {GpioV2PadModeGpio,  GpioV2HostOwnGpio,  GpioV2DirInInv,   GpioV2StateDefault, GpioV2IntDis,                 GpioV2ResetDefault,   GpioV2TermNone,  GpioV2Unlock,  GpioV2Lock}},  // THC0_SPI1_INT_N_TCH_PNL1
    { 0x0 }
  })}

  # Touch Panel2 Table
  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableTouchPanel1|*|{CODE({
  // Touch Panel 1, Not used by default in RVP; Applicable as Rework;
    {GPIOV2_PTL_PCD_XXGPP_E_7,   {GpioV2PadModeGpio,  GpioV2HostOwnAcpi,  GpioV2DirOut,     GpioV2StateHigh,    GpioV2IntDis,                GpioV2ResetHost,     GpioV2TermDefault}},  // TCH_PNL2_PWR_EN
    {GPIOV2_PTL_PCD_XXGPP_F_16,  {GpioV2PadModeGpio,  GpioV2HostOwnAcpi,  GpioV2DirOut,     GpioV2StateHigh,    GpioV2IntDis,                GpioV2ResetHost,     GpioV2TermDefault}},  // THC1_SPI2_RST_N_TCH_PNL2
    {GPIOV2_PTL_PCD_XXGPP_F_18,  {GpioV2PadModeGpio,  GpioV2HostOwnGpio,  GpioV2DirInInv,   GpioV2StateDefault, GpioV2IntDis,                GpioV2ResetHost,     GpioV2TermNone,  GpioV2Unlock,  GpioV2Lock}},  // THC1_SPI2_INT_N_TCH_PNL2
    { 0x0 }
  })}

  # Cnvd Gpio Table
  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableLch|*|{CODE({
    { GPIOV2_PTL_PCD_XXGPP_F_20, {GpioV2PadModeGpio,  GpioV2HostOwnGpio,  GpioV2DirIn,  GpioV2StateDefault, GpioV2IntEdge|GpioV2IntApic,  GpioV2ResetHost,    GpioV2TermNone,  GpioV2Unlock, GpioV2Lock}}, // CVS_HST_WAKE
    { GPIOV2_PTL_PCD_XXGPP_E_10, {GpioV2PadModeGpio,  GpioV2HostOwnGpio,  GpioV2DirOut, GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetDefault,  GpioV2TermNone, GpioV2Unlock, GpioV2Lock}}, // CRD1_RST_N
    { 0x0 }
  })}

  # Lch gpio
  gBoardModuleTokenSpaceGuid.VpdPcdLchIrqGpioPin|*|{CODE(
    { GPIOV2_PTL_PCD_XXGPP_F_20 } // CVS_HST_WAKE
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdLchResetGpioPin|*|{CODE(
    { GPIOV2_PTL_PCD_XXGPP_E_10 } // CRD1_RST_N
  )}

  # Vr Alert Enable
  gBoardModuleTokenSpaceGuid.VpdPcdVrAlertEnable|*|FALSE

  # Pch Thermal Hot Enable
  gBoardModuleTokenSpaceGuid.VpdPcdPchThermalHotEnable|*|FALSE

  # Thermal Sensor Gpio C Pmsync Enable
  gBoardModuleTokenSpaceGuid.VpdPcdMemoryThermalSensorGpioCPmsyncEnable|*|FALSE

  # Thermal Sensor Gpio D Pmsync Enable
  gBoardModuleTokenSpaceGuid.VpdPcdMemoryThermalSensorGpioDPmsyncEnable|*|FALSE

  # M.2 Gen4 SSD (NIST)
  gBoardModuleTokenSpaceGuid.VpdPcdNvmeRecoveryPrimarySlotPortNumber|*|0x5

  # PCIE SLOT 1 - X2 CONNECTOR RTD3
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1RootPort|*|0x1
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1PwrEnableGpioNo|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_A_8 } // X2_PCIE_SLOT_PWR_EN
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1HoldRstGpioNo|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_D_19 } // X2_DT_PCIE_RST_N
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1PwrEnableGpioPolarity|*|$(PIN_GPIO_ACTIVE_HIGH)
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1HoldRstGpioPolarity|*|$(PIN_GPIO_ACTIVE_LOW)

  # PCIE SLOT 2 - X8 CONNECTOR RTD3
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot2RootPort|*|0xB
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot2PwrEnableGpioNo|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_F_10 } // PEG_SLOT_PWR_EN_N
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot2HoldRstGpioNo|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_D_9 } // PEG_SLOT_RST_N
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot2PwrEnableGpioPolarity|*|$(PIN_GPIO_ACTIVE_LOW)
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot2HoldRstGpioPolarity|*|$(PIN_GPIO_ACTIVE_LOW)

  # M.2 SSD Slot #1 RTD3
  gBoardModuleTokenSpaceGuid.VpdPcdM2Ssd1PowerEnableGpio|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_B_10 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdM2Ssd1RstGpio|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_B_9 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdM2Ssd1PowerEnableGpioPolarity|*|$(PIN_GPIO_ACTIVE_HIGH)
  gBoardModuleTokenSpaceGuid.VpdPcdM2Ssd1RstGpioPolarity|*|$(PIN_GPIO_ACTIVE_LOW)

  # Mipi Cam
  gBoardModuleTokenSpaceGuid.VpdPcdMipiCamGpioEnable|*|0x1

  # M.2 SSD Slot #2 RTD3
  gBoardModuleTokenSpaceGuid.VpdPcdM2Ssd2PowerEnableGpio|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_B_16 } // GEN5_SSD_PWREN
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdM2Ssd2RstGpio|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_E_3 } // M2_GEN5_SSD_RESET_N
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdM2Ssd2PowerEnableGpioPolarity|*|$(PIN_GPIO_ACTIVE_HIGH)
  gBoardModuleTokenSpaceGuid.VpdPcdM2Ssd2RstGpioPolarity|*|$(PIN_GPIO_ACTIVE_LOW)

  # USB Type-C PD Information Present : FALSE - Disable, TRUE - Enable
  gBoardModuleTokenSpaceGuid.VpdPcdUsbCEcSupportPdInfoPresent|*|TRUE

  # USB Type-C Retimer Information Present : FALSE - Disable, TRUE - Enable
  gBoardModuleTokenSpaceGuid.VpdPcdUsbCEcSupportRetimerInfoPresent|*|TRUE

  # UsbC Retimer Flash Configuration Structure : {CascadedRetimerFlashIndex, FirstUsbCConnectorIndex, SecondUsbCConnectorIndex}
  gBoardModuleTokenSpaceGuid.VpdPcdUsbCRetimerFlash| * |{CODE(
  {
   {0x1, 0x0, 0x1},
   {0x2, 0x0, 0x1},
   {0x1, 0x2, 0x7},
   {0x1, 0x3, 0x7},
   {0x0}  // terminator
  })}

  # UsbC Retimer Capsule Debug Progress Code Print: FALSE - Disable, TRUE - Enable
  gBoardModuleTokenSpaceGuid.VpdPcdUsbCCapsuleProgressCodeEnable|*|FALSE

  # Control the EC debug Info Print level before executing any command for PD Bridge, default is DEBUG_VERBOSE
  gBoardModuleTokenSpaceGuid.VpdPcdPdBridgeDebugInfoPrintLevel|*|0x00400000

  #
  # *** IMPORTANT ***
  #   All of the USBC connectors MUST be described at the beginning of the table.
  #
  # USB Connector Board Config : {ConnectorConnectable, ConnectorVisibility, ConnectorType, UsbOcPinType, UsbOcPin, Usb2Controller, Usb2PortNum, Usb3Controller, Usb3PortNum}
  gBoardModuleTokenSpaceGuid.VpdPcdUsbConnector| * |{CODE(
  {
  // Connectable,   Visibility, ConnectorType, UsbOcPinType, UsbOcPin, Usb2Controller, Usb2PortNum, Usb3Controller, Usb3PortNum
    {CONNECTABLE,   VISIBLE,    0x09,          VW_OC,        OC_PIN0,  PCH_USB2,       PORT1,       TCSS_USB3,      PORT1}, // TCP0
    {CONNECTABLE,   VISIBLE,    0x09,          VW_OC,        OC_PIN1,  PCH_USB2,       PORT2,       TCSS_USB3,      PORT2}, // TCP1
    {CONNECTABLE,   VISIBLE,    0x09,          VW_OC,        OC_PIN2,  PCH_USB2,       PORT3,       TCSS_USB3,      PORT3}, // TCP2
    {UNCONNECTABLE, VISIBLE,    0x09,          VW_OC,        OC_PIN3,  PCH_USB2,       PORT4,       TCSS_USB3,      PORT4}, // TCP3, default is HDMI
    {CONNECTABLE,   VISIBLE,    0x00,          PHYSICAL_OC,  OC_PIN0,  PCH_USB2,       PORT5,       PCH_USB3,       PORT1}, // TAP1
    {CONNECTABLE,   VISIBLE,    0x00,          PHYSICAL_OC,  OC_PIN0,  PCH_USB2,       PORT6,       PCH_USB3,       PORT2}, // TAP2
    {CONNECTABLE,   INVISIBLE,  0x00,          0,            0,        PCH_USB2,       PORT7,       0,              0    }, // FPS
    {CONNECTABLE,   INVISIBLE,  0x00,          0,            0,        PCH_USB2,       PORT8,       0,              0    }, // M.2 WLAN
    {0x0}  // terminator
  })}
  gBoardModuleTokenSpaceGuid.VpdPcdCvsUsbConnector| * |{CODE(
  {
  // Connectable,   Visibility, ConnectorType, UsbOcPinType, UsbOcPin, Usb2Controller, Usb2PortNum, Usb3Controller, Usb3PortNum
    {CONNECTABLE,   VISIBLE,    0x09,          VW_OC,        OC_PIN0,  PCH_USB2,       PORT1,       TCSS_USB3,      PORT1}, // TCP0
    {CONNECTABLE,   VISIBLE,    0x09,          VW_OC,        OC_PIN1,  PCH_USB2,       PORT2,       TCSS_USB3,      PORT2}, // TCP1
    {UNCONNECTABLE, VISIBLE,    0x09,          VW_OC,        OC_PIN3,  PCH_USB2,       PORT4,       TCSS_USB3,      PORT4}, // TCP3, default is HDMI
    {CONNECTABLE,   INVISIBLE,  0xFF,          0,            0,        PCH_USB2,       PORT3,       0,              0    }, // CRD
    {CONNECTABLE,   VISIBLE,    0x00,          PHYSICAL_OC,  OC_PIN0,  PCH_USB2,       PORT5,       PCH_USB3,       PORT1}, // TAP1
    {CONNECTABLE,   VISIBLE,    0x00,          PHYSICAL_OC,  OC_PIN0,  PCH_USB2,       PORT6,       PCH_USB3,       PORT2}, // TAP2
    {CONNECTABLE,   VISIBLE,    0x00,          0,            0,        PCH_USB2,       PORT7,       0,              0    }, // FPS
    {CONNECTABLE,   INVISIBLE,  0x00,          0,            0,        PCH_USB2,       PORT8,       0,              0    }, // M.2 WLAN
    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdUsbCConnector| * |{CODE(
  {
  // ModularIoSupported, EdpModularIoSupported, RetimerCount, PcieSupported, DpAltModeSupported, Usb4Supported, Tbt3Supported, PdNum,    AuxDpMode, TbtControllerType, TbtControllerNum, DmaPortNum, PcieType, PcieRootPortNum
    {FIXED_IO,           0,                     1,            1,             1,                  1,             1,             USBC_PD1, 0,         ITBT_DMA,          TBT_CONTROLLER1,  DMA_PORT1,  0,        0}, // TCP0
    {FIXED_IO,           0,                     1,            1,             1,                  1,             1,             USBC_PD1, 0,         ITBT_DMA,          TBT_CONTROLLER1,  DMA_PORT2,  0,        0}, // TCP1
    {MODULAR_IO,         0,                     1,            1,             1,                  1,             1,             USBC_PD2, 0,         ITBT_DMA,          TBT_CONTROLLER2,  DMA_PORT1,  0,        0}, // TCP2
    {MODULAR_IO,         0,                     0,            0,             0,                  0,             0,             USBC_PD3, 2,         ITBT_DMA,          TBT_CONTROLLER2,  DMA_PORT2,  0,        0}, // TCP3, default is HDMI
    {0x0}  // terminator
  })}
  gBoardModuleTokenSpaceGuid.VpdPcdCvsUsbCConnector| * |{CODE(
  {
  // ModularIoSupported, EdpModularIoSupported, RetimerCount, PcieSupported, DpAltModeSupported, Usb4Supported, Tbt3Supported, PdNum,    AuxDpMode, TbtControllerType, TbtControllerNum, DmaPortNum, PcieType, PcieRootPortNum
    {FIXED_IO,           0,                     1,            1,             1,                  1,             1,             USBC_PD1, 0,         ITBT_DMA,          TBT_CONTROLLER1,  DMA_PORT1,  0,        0}, // TCP0
    {FIXED_IO,           0,                     1,            1,             1,                  1,             1,             USBC_PD1, 0,         ITBT_DMA,          TBT_CONTROLLER1,  DMA_PORT2,  0,        0}, // TCP1
    {MODULAR_IO,         0,                     0,            0,             0,                  0,             0,             USBC_PD3, 2,         ITBT_DMA,          TBT_CONTROLLER2,  DMA_PORT2,  0,        0}, // TCP3, default is HDMI
    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdTcssCovUsbA|*|{CODE(
  {
    0, // Port1 Orientation
    0, // Port1 Physical OC pin
    0, // Port2 Orientation
    0, // Port2 Physical OC pin
    0, // Port3 Orientation
    3, // Port3 Physical OC pin
    0, // Port4 Orientation
    3  // Port4 Physical OC pin
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdTcssPmcPdEnable|*|TRUE
  gBoardModuleTokenSpaceGuid.VpdPcdUsbCUcmMode|*|0x1 # 0: Unsupported, 1: UCSI, 2: UCMCx
  gBoardModuleTokenSpaceGuid.VpdPcdHdaI2sCodecIrqGpio|*|{CODE(
   { 0x0 } // CODEC_INT // Present on rework3
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdHdaI2sCodecI2cBusNumber|*|0x3
  gBoardModuleTokenSpaceGuid.VpdPcdWwanModemBaseBandResetGpio|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_E_8 } // M.2_WAKE_ON_WWAN_N
  )}

# Root Port PEWAKE
   gBoardModuleTokenSpaceGuid.VpdPcdRootPortPewakeConfig|*|{CODE(
  {
    GPIOV2_PTL_PCD_XXGPP_B_25,   // PEWAKE Pin for RP01
    0,                           // PEWAKE Pin for RP02
    GPIOV2_PTL_PCD_XXGPP_E_2,    // PEWAKE Pin for RP03
    0,                           // PEWAKE Pin for RP04
    0,                           // PEWAKE Pin for RP05
    0,                           // PEWAKE Pin for RP06
    0,                           // PEWAKE Pin for RP07
    0,                           // PEWAKE Pin for RP08
    0,                           // PEWAKE Pin for RP09
    0,                           // PEWAKE Pin for RP10
    GPIOV2_PTL_PCD_XXGPP_D_24,   // PEWAKE Pin for RP11
    0,                           // PEWAKE Pin for RP12
    0xFFFFFFFF                   // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdBoardRtd3TableSignature|*|{'P','t','l','R','v','p','3','4'}
  gBoardModuleTokenSpaceGuid.VpdPcdBatterySupport|*|($(BOARD_REAL_BATTERY_SUPPORTED)|$(BOARD_VIRTUAL_BATTERY_SUPPORTED))
  gBoardModuleTokenSpaceGuid.VpdPcdMipiCamSensor|*|FALSE
  gBoardModuleTokenSpaceGuid.VpdPcdZPoddConfig|*|0
  gBoardModuleTokenSpaceGuid.VpdPcdSmcRuntimeSciPin|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdConvertableDockSupport|*|TRUE
  gBoardModuleTokenSpaceGuid.VpdPcdEcHotKeyF3Support|*|TRUE
  gBoardModuleTokenSpaceGuid.VpdPcdEcHotKeyF4Support|*|TRUE
  gBoardModuleTokenSpaceGuid.VpdPcdEcHotKeyF5Support|*|TRUE
  gBoardModuleTokenSpaceGuid.VpdPcdEcHotKeyF6Support|*|TRUE
  gBoardModuleTokenSpaceGuid.VpdPcdEcHotKeyF7Support|*|TRUE
  gBoardModuleTokenSpaceGuid.VpdPcdEcHotKeyF8Support|*|TRUE
  gBoardModuleTokenSpaceGuid.VpdPcdVirtualButtonVolumeUpSupport|*|TRUE
  gBoardModuleTokenSpaceGuid.VpdPcdVirtualButtonVolumeDownSupport|*|TRUE
  gBoardModuleTokenSpaceGuid.VpdPcdVirtualButtonHomeButtonSupport|*|TRUE
  gBoardModuleTokenSpaceGuid.VpdPcdVirtualButtonRotationLockSupport|*|TRUE
  gBoardModuleTokenSpaceGuid.VpdPcdSlateModeSwitchSupport|*|TRUE
  gBoardModuleTokenSpaceGuid.VpdPcdVirtualGpioButtonSupport|*|TRUE
  gBoardModuleTokenSpaceGuid.VpdPcdPmPowerButtonGpioPin|*|{CODE(
   { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdAcDcAutoSwitchSupport|*|FALSE
  gBoardModuleTokenSpaceGuid.VpdPcdAcpiEnableAllButtonSupport|*|TRUE
  gBoardModuleTokenSpaceGuid.VpdPcdAcpiHidDriverButtonSupport|*|TRUE

  gBoardModuleTokenSpaceGuid.VpdPcdTpm2IntGpio|*|{CODE(
    { GPIOV2_PTL_PCD_XXGPP_D_15 }  // SPI_TPM_INT_N
  )}

  # LID status for gop
  gBoardModuleTokenSpaceGuid.VpdPcdGpioLidStatus|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_F_23 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdSkuType|*|0x2
  gBoardModuleTokenSpaceGuid.VpdPcdDisableMrcRetraining|*|0x0

  # PTL-UH RVP4 power meter table
  gBoardModuleTokenSpaceGuid.VpdPcdPowerMeter|*|{CODE(
  { 7,    // DeviceSize
    '3',  // I2C_Scope
    {
     //BaseAddress, {RailName, ResistorValue}
     {    0x18,     {{"SoC_VCCCORE_PH1",    2}, {"SoC_VCCCORE_PH2",    2}, {"SoC_VCCCORE_PH3",    2}, {"SoC_VCCCORE_PH4",  2}}},
     {    0x1E,     {{"SoC_VCCGT_PH1",      2}, {"SoC_VCCGT_PH2",      2}, {"SoC_VCCSA",          2}, {"SoC_VDDQ",         2}}},
     {    0x11,     {{"SoC_VCCPRIM_VNNAON", 1}, {"SoC_VCCPRIM_IO",     4}, {"SoC_VCCPRIM3P3",    22}, {"SoC_VCCPRIM1P8",  10}}},
     {    0x15,     {{"SoC_VCCRTC",        22}, {"SoC_VCCST",          2}, {"SoC_VDD2_CPU",       2}, {"DRAM_VIN_BULK",    5}}},
     {    0x19,     {{"STORAGE_UFS_V2P5S", 22}, {"STORAGE_UFS_V1P2S", 10}, {"PCIE_X8_SLOT_V3P3S", 5}, {"PCIE_X8_SLOT_V12S",2}}},
     {    0x14,     {{"DISPLAY_BKLIGHT",   10}, {"DISPLAY_VDD",       10}, {"CONNECTIVITY_WLAN",  5}, {"STORAGE_V3P3A",    5}}},
     {    0x17,     {{"SYSTEM_VBATA1",      2}, {"SYSTEM_VBATA2",      2}, {"CAMERA_V1P8S",      10}, {"CAMERA_V3P3S",    10}}},
    }
  })}

  # PTL-UH RVP4 Telemetry table
  gBoardModuleTokenSpaceGuid.VpdPcdTelemetry|*|{CODE(
    { 0x0 }
  )}

  # PCIE CLOCK USGAGE for PTL H
  gBoardModuleTokenSpaceGuid.VpdPcdPcieClkUsageMap|*|{CODE(
  {{
    PCIE_PCH + 10,  // X8 CEM Slot
    PCIE_PCH + 8,   // M.2 Gen5 SSD
    PCIE_PCH,       // x2 PCIe slot (LR)
    LAN_CLOCK,      // GBE LAN
    NOT_USED,
    PCIE_PCH + 2,   // M.2 WWAN
    PCIE_PCH + 4,   // M.2 Gen4 SSD
    NOT_USED,
    NOT_USED
  }}
  )}

  # PCIE CLOCK USGAGE for PTL P
  gBoardModuleTokenSpaceGuid.VpdPcdPPcieClkUsageMap|*|{CODE(
  {{
    NOT_USED,
    PCIE_PCH + 8,   // M.2 Gen5 SSD
    PCIE_PCH,       // x2 PCIe slot (LR)
    LAN_CLOCK,      // GBE LAN
    NOT_USED,
    PCIE_PCH + 2,   // M.2 WWAN
    PCIE_PCH + 4,   // M.2 Gen4 SSD
    NOT_USED,
    NOT_USED
  }}
  )}


  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTable|*|{CODE(
  {
  //
  // x2 Slot
  //
    {GPIOV2_PTL_PCD_XXGPP_A_8,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // X2_PCIE_SLOT_PWR_EN
    {GPIOV2_PTL_PCD_XXGPP_D_19,  {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // X2_DT_PCIE_RST_N
    {GPIOV2_PTL_PCD_XXGPP_B_25,  {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirInInv,  GpioV2StateDefault, GpioV2IntLevel|GpioV2IntSci,  GpioV2ResetHostDeep,  GpioV2TermDefault,  GpioV2Unlock,  GpioV2Lock}},  // X2_SLOT_WAKE_N

  //
  // x8 Slot \ PEG
  //
    {GPIOV2_PTL_PCD_XXGPP_E_19,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // PEG_SLOT_DGPU_SEL_N
    {GPIOV2_PTL_PCD_XXGPP_F_10,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateLow,     GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // PEG_SLOT_PWR_EN_N
    {GPIOV2_PTL_PCD_XXGPP_D_9,    {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // PEG_SLOT_RST_N
    {GPIOV2_PTL_PCD_XXGPP_D_24,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirInInv,  GpioV2StateDefault, GpioV2IntLevel|GpioV2IntSci,  GpioV2ResetHostDeep,  GpioV2TermDefault, GpioV2Unlock,  GpioV2Lock}}, // PEG_SLOT_WAKE_N
    {GPIOV2_PTL_PCD_XXGPP_E_20,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirIn,    GpioV2StateDefault,  GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // PEG_SLOT_DGPU_PWR_OK

  //
  // M.2 SSD Gen4
  //
    {GPIOV2_PTL_PCD_XXGPP_B_10,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,     GpioV2IntDis,      GpioV2ResetHost,  GpioV2TermDefault}}, //GEN4_SSD_PWREN
    {GPIOV2_PTL_PCD_XXGPP_B_9,    {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,      GpioV2ResetHost,  GpioV2TermDefault}},  // M2_GEN4_SSD_RESET_N

  //
  // M.2 SSD Gen5
  //
    {GPIOV2_PTL_PCD_XXGPP_B_16,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,      GpioV2ResetHost,  GpioV2TermDefault}}, // GEN5_SSD_PWREN
    {GPIOV2_PTL_PCD_XXGPP_E_3,    {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,      GpioV2ResetHost,  GpioV2TermDefault}},  // M2_GEN5_SSD_RESET_N

  //
  // CNVi (WLAN)
  //
    {GPIOV2_PTL_PCD_XXGPP_A_16,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,      GpioV2ResetHostDeep,  GpioV2TermDefault}}, // BT_RF_KILL_N
    {GPIOV2_PTL_PCD_XXGPP_A_17,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,      GpioV2ResetHostDeep,  GpioV2TermDefault}}, // WIFI_RF_KILL_N
    {GPIOV2_PTL_PCD_XXGPP_F_6,    {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirIn,     GpioV2StateDefault, GpioV2IntDis,      GpioV2ResetHost,      GpioV2TermDefault}}, // WLAN_WWAN_COEX3

  //
  // Camera Conn 1
  //
    {GPIOV2_PTL_PCD_XXGPP_C_5,   {GpioV2PadModeGpio,  GpioV2HostOwnAcpi,  GpioV2DirOut,    GpioV2StateLow,     GpioV2IntDis,    GpioV2ResetHost,    GpioV2TermDefault}},  // CRD1_PWREN
    {GPIOV2_PTL_PCD_XXGPP_H_1,   {GpioV2PadModeGpio,  GpioV2HostOwnAcpi,  GpioV2DirOut,    GpioV2StateLow,     GpioV2IntDis,    GpioV2ResetHost,    GpioV2TermDefault}},  // CRD_CAM_STROBE
    {GPIOV2_PTL_PCD_XXGPP_C_15,  {GpioV2PadModeGpio,  GpioV2HostOwnAcpi,  GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,    GpioV2ResetHost,    GpioV2TermDefault}},  // CRD1_CLK_EN
    {GPIOV2_PTL_PCD_XXGPP_E_6,   {GpioV2PadModeGpio,  GpioV2HostOwnAcpi,  GpioV2DirIn,     GpioV2StateDefault, GpioV2IntLevel,  GpioV2ResetHost,    GpioV2TermDefault}},  // SECURE_CAM_SW
    {GPIOV2_PTL_PCD_XXGPP_F_20,  {GpioV2PadModeGpio,  GpioV2HostOwnAcpi,  GpioV2DirOut,    GpioV2StateLow,     GpioV2IntDis,    GpioV2ResetHost,    GpioV2TermDefault}},  // GPP_PRIVACY_LED_CAM1_CVS_HST_WAKE
    {GPIOV2_PTL_PCD_XXGPP_E_10,  {GpioV2PadModeGpio,  GpioV2HostOwnAcpi,  GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,    GpioV2ResetHost,    GpioV2TermDefault}},  // CRD1_RST_N

  //
  // Camera Conn 2
  //
    {GPIOV2_PTL_PCD_XXGPP_C_8,  {GpioV2PadModeGpio,   GpioV2HostOwnAcpi,  GpioV2DirOut,    GpioV2StateLow,    GpioV2IntDis,      GpioV2ResetHost,  GpioV2TermDefault}},  // CRD2_PWREN
    {GPIOV2_PTL_PCD_XXGPP_E_1,  {GpioV2PadModeGpio,   GpioV2HostOwnAcpi,  GpioV2DirOut,    GpioV2StateHigh,   GpioV2IntDis,      GpioV2ResetHost,  GpioV2TermDefault}},  // CRD2_RST_N
    {GPIOV2_PTL_PCD_XXGPP_F_19, {GpioV2PadModeGpio,   GpioV2HostOwnAcpi,  GpioV2DirOut,    GpioV2StateLow,    GpioV2IntDis,      GpioV2ResetHost,  GpioV2TermDefault}},  // GPP_PRIVACY_LED_CAM2

  //
  // TPM
  //
    { GPIOV2_PTL_PCD_XXGPP_D_15, {GpioV2PadModeGpio, GpioV2HostOwnGpio,    GpioV2DirInInv,    GpioV2StateDefault,  GpioV2IntEdge|GpioV2IntSci,  GpioV2ResetHostDeep,  GpioV2TermNone,  GpioV2TermDefault}},  // SPI_TPM_INT_N

  //
  // Touch panel-1
  //
    {GPIOV2_PTL_PCD_XXGPP_F_8,   {GpioV2PadModeGpio,  GpioV2HostOwnAcpi,  GpioV2DirOut,     GpioV2StateHigh,     GpioV2IntDis,     GpioV2ResetHost,     GpioV2TermDefault}},  // TCH_PNL1_PWR_EN

  //
  // Touch pad
  //
    {GPIOV2_PTL_PCD_XXGPP_F_18,  {GpioV2PadModeGpio,  GpioV2HostOwnGpio,  GpioV2DirInInv,   GpioV2StateDefault,  GpioV2IntDis,      GpioV2ResetHost,     GpioV2TermNone,  GpioV2Unlock,  GpioV2Lock}},  // TCH_PAD_INT_N

  //
  // TCSS
  //
    {GPIOV2_PTL_PCD_XXGPP_V_17, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetResume,  GpioV2TermNone}}, // GPP_V17_TCP_RT_S0IX_ENTRY_EXIT_N

  //
  // Not Used pins set to High Impedance
  //
    {GPIOV2_PTL_PCD_XXGPP_H_0,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdCoinlessModeDetectGpio|*|{CODE({
    {GPIOV2_PTL_PCD_XXGPP_D_14, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirIn,  GpioV2StateDefault,  GpioV2IntLevel, GpioV2ResetHost,  GpioV2TermDefault, GpioV2Unlock,  GpioV2Lock}},  // COINLESS_MODE_SELECT

    {0x0}
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdMrcSpdData| * |{CODE(
    { 0x0 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdMrcDqsMapCpu2Dram| * |{CODE(
    { 0x0 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdMrcDqMapCpu2Dram| * |{CODE(
    { 0x0 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableEarlyPreMem|*|{CODE({
    //
    // Lid Switch Wake Gpio
    //
    {GPIOV2_PTL_PCD_XXGPP_F_23, {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirIn,  GpioV2StateDefault,  GpioV2IntDis, GpioV2ResetHostDeep,  GpioV2TermDefault}}, // SMC_LID
    { 0x0 }  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdMrcChannelToCkdQckMapping| * |{CODE(
  { 8,
    { 0 ,   // Port 0 Controller 0 Channel 0 Dimm 0
      0 ,   // Port 1 Controller 0 Channel 0 Dimm 1
      1 ,   // Port 2 Controller 0 Channel 1 Dimm 0
      0 ,   // Port 3 Controller 0 Channel 1 Dimm 1
      0 ,   // Port 4 Controller 1 Channel 0 Dimm 0
      0 ,   // Port 5 Controller 1 Channel 0 Dimm 1
      1 ,   // Port 6 Controller 1 Channel 1 Dimm 0
      0     // Port 7 Controller 1 Channel 1 Dimm 1
    }
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdMrcPhyClockToCkdDimm| * |{CODE(
  { 8,
    { 0 ,   // Port 0 Controller 0 Channel 0 Dimm 0
      0 ,   // Port 1 Controller 0 Channel 0 Dimm 1
      0 ,   // Port 2 Controller 0 Channel 1 Dimm 0
      0 ,   // Port 3 Controller 0 Channel 1 Dimm 1
      8 ,   // Port 4 Controller 1 Channel 0 Dimm 0
      0 ,   // Port 5 Controller 1 Channel 0 Dimm 1
      8 ,   // Port 6 Controller 1 Channel 1 Dimm 0
      0     // Port 7 Controller 1 Channel 1 Dimm 1
    }
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTablePreMem|*|{CODE({
    //
    // TCSS
    //
    {GPIOV2_PTL_PCD_XXGPP_D_1,  {GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirOut,  GpioV2StateHigh,   GpioV2IntDis,  GpioV2ResetHostDeep,  GpioV2TermDefault}}, // MOD_TCSS1_TYP_A_VBUS_EN
    {GPIOV2_PTL_PCD_XXGPP_F_11, {GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirOut,  GpioV2StateHigh,   GpioV2IntDis,  GpioV2ResetHostDeep,  GpioV2TermDefault}}, // MOD_TCSS2_TYP_A_VBUS_EN
    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdTcssEnable|*|1

  gBoardModuleTokenSpaceGuid.VpdPcdPmicIdMap|*|0x0

  gBoardModuleTokenSpaceGuid.VpdPcdCnvBoardConfig| * |{CODE(
  {
    {
      0,                            // Revision
      0,                            // WlanRootPortNumber
      0,                            // DiscreteUsbBtPortNumber
      8,                            // IntegratedUsbBtPortNumber
      GPIOV2_PTL_PCD_XXGPP_A_17,    // WlanRfKillGpio
      0,                            // WlanWakeGpio
      0,                            // WlanRstGpio
      GPIOV2_PTL_PCD_XXGPP_A_16,    // BtRfKillGpio
      0                             // BtIrqGpio
    }
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdClwlI2cController|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdClwlI2cSlaveAddress|*|0x0

[PcdsDynamicHii.common.SkuIdPtlUHDdr5Rvp4.STANDARD]
  gStructPcdTokenSpaceGuid.PcdSetup.PchI2cSensorDevicePort[5]|0x1                                    # Connected device
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioDmicLinkEnable[0]|0x0                               # DMIC #0
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioDmicLinkEnable[1]|0x0                               # DMIC #1
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshI2cEnable[1]|0x0                                        # I2C1
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshSpiEnable[0]|0x1                                        # SPI_0
  gStructPcdTokenSpaceGuid.PcdSetup.TcssUcmDevice|0x0                                                # USBC connector manager selection

#####################################################################
#  PTL UH DDR5 T3 - RVP4 BOM
#     Board ID: 0x04 (BomIdPtlUHDdr5Rvp4)
#   Display ID: 0x00
#       SKU ID: 0x00060004 (SkuIdPtlUHDdr5Rvp4Bom)
#####################################################################
[PcdsDynamicExVpd.common.SkuIdPtlUHDdr5Rvp4Bom]

  # Root Port PEWAKE
   gBoardModuleTokenSpaceGuid.VpdPcdRootPortPewakeConfig|*|{CODE(
  {
    GPIOV2_PTL_PCD_XXGPP_B_25,   // PEWAKE Pin for RP01
    0,                           // PEWAKE Pin for RP02
    0,                           // PEWAKE Pin for RP03
    0,                           // PEWAKE Pin for RP04
    0,                           // PEWAKE Pin for RP05
    0,                           // PEWAKE Pin for RP06
    0,                           // PEWAKE Pin for RP07
    0,                           // PEWAKE Pin for RP08
    0,                           // PEWAKE Pin for RP09
    0,                           // PEWAKE Pin for RP10
    GPIOV2_PTL_PCD_XXGPP_D_24,   // PEWAKE Pin for RP11
    0,                           // PEWAKE Pin for RP12
    0xFFFFFFFF                   // terminator
  })}

  # PCIE CLOCK USGAGE for PTL H
  gBoardModuleTokenSpaceGuid.VpdPcdPcieClkUsageMap|*|{CODE(
  {{
    PCIE_PCH + 10,  // X8 CEM Slot
    PCIE_PCH + 8,   // M.2 Gen5 SSD
    PCIE_PCH,       // x4 PCIe DT CEM slot (LR)
    NOT_USED,
    NOT_USED,
    NOT_USED,
    PCIE_PCH + 4,   // M.2 Gen4 SSD
    NOT_USED,
    NOT_USED
  }}
  )}

  # PCIE CLOCK USGAGE for PTL P
  gBoardModuleTokenSpaceGuid.VpdPcdPPcieClkUsageMap|*|{CODE(
  {{
    NOT_USED,
    PCIE_PCH + 8,   // M.2 Gen5 SSD
    PCIE_PCH,       // x4 PCIe DT CEM slot (LR)
    NOT_USED,
    NOT_USED,
    NOT_USED,
    PCIE_PCH + 4,   // M.2 Gen4 SSD
    NOT_USED,
    NOT_USED
  }}
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdWwanRootPortNumber|*|0

 # WWAN Gpios
  gBoardModuleTokenSpaceGuid.VpdPcdWwanFullCardPowerOffGpio|*|{CODE(
   { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdWwanBbrstGpio|*|{CODE(
   { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdWwanPerstGpio|*|{CODE(
   { 0x0 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdWwanFullCardPowerOffGpioPolarity|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdWwanBbrstGpioPolarity|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdWwanPerstGpioPolarity|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdWwanSourceClock|*|0


#####################################################################
#  PTL UH DDR5 T3 - RVP4 Dp
#     Board ID: 0x04 (BoardIdIdPtlUHDdr5Rvp4)
#   Display ID: 0x01
#       SKU ID: 0x01000004 (SkuIdPtlUHDdr5Rvp4Dp)
#####################################################################
[PcdsDynamicExVpd.common.SkuIdPtlUHDdr5Rvp4Dp]

  #Display DDi Config table
  gBoardModuleTokenSpaceGuid.VpdPcdDisplayDdiConfigTable| * |{CODE(
  { 16,
    {DdiPortDisabled,    // DDI Port A Config : DdiPortDisabled = No LFP is Connected, DdiPortEdp = eDP, DdiPortMipiDsi = MIPI DSI
     DdiPortDisabled,    // DDI Port B Config : DdiPortDisabled = No LFP is Connected, DdiPortEdp = eDP, DdiPortMipiDsi = MIPI DSI
     DdiHpdEnable,       // DDI Port A HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdDisable,      // DDI Port B HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdDisable,      // DDI Port C HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdDisable,      // DDI Port 1 HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdDisable,      // DDI Port 2 HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdDisable,      // DDI Port 3 HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdDisable,      // DDI Port 4 HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiDdcEnable,       // DDI Port A DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
     DdiDisable,         // DDI Port B DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
     DdiDisable,         // DDI Port C DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
     DdiDisable,         // DDI Port 1 DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
     DdiDisable,         // DDI Port 2 DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
     DdiDisable,         // DDI Port 3 DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
     DdiDisable          // DDI Port 4 DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
    }
  })}
