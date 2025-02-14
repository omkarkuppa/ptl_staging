## @file
#  Board description file initializes configuration (PCD) settings for the project.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2022 Intel Corporation.
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
#  PTL UH LP5x T3 - RVP1
#     Board ID: 0x01 (BoardIdPtlUHLp5Rvp1)
#   Display ID: 0x00
#       SKU ID: 0x00000001 (SkuIdPtlUHLp5Rvp1)
#####################################################################
[PcdsDynamicExVpd.common.SkuIdPtlUHLp5Rvp1]

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
    { 0xA8 ,   // Port 0
      0x00 ,   // Port 1
      0xA8 ,   // Port 2
      0x00 ,   // Port 3
      0xA8 ,   // Port 4
      0x00 ,   // Port 5
      0xA8 ,   // Port 6
      0x00 ,   // Port 7
      0xA8 ,   // Port 8
      0x00 ,   // Port 9
      0xA8 ,   // Port 10
      0x00 ,   // Port 11
      0xA8 ,   // Port 12
      0x00 ,   // Port 13
      0xA8 ,   // Port 14
      0x00     // Port 15
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
  gBoardModuleTokenSpaceGuid.VpdPcdMrcBoardStackUp|*|0x1

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

  # Display DDi Config table
  gBoardModuleTokenSpaceGuid.VpdPcdDisplayDdiConfigTable| * |{CODE(
  { 16,
    {DdiPortEdp,         // DDI Port A Config : DdiPortDisabled = No LFP is Connected, DdiPortEdp = eDP
     DdiPortDisabled,    // DDI Port B Config : DdiPortDisabled = No LFP is Connected, DdiPortEdp = eDP
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
   { GPIOV2_PTL_PCD_XXGPP_E_18 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdTouchpanel1IrqGpio|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_F_18 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdTouchpadIrqGpio|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_F_18 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdTouchpadIrqGpioPolarity|*|$(PIN_GPIO_ACTIVE_LOW)

  gBoardModuleTokenSpaceGuid.VpdPcdWwanFullCardPowerOffGpioPolarity|*|$(PIN_GPIO_ACTIVE_HIGH)
  gBoardModuleTokenSpaceGuid.VpdPcdWwanBbrstGpioPolarity|*|$(PIN_GPIO_ACTIVE_LOW)
  gBoardModuleTokenSpaceGuid.VpdPcdWwanPerstGpioPolarity|*|$(PIN_GPIO_ACTIVE_LOW)
  gBoardModuleTokenSpaceGuid.VpdPcdWwanSourceClock|*|5
  gBoardModuleTokenSpaceGuid.VpdPcdWwanRootPortNumber|*|2

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

# PCIE SLOT 1 - X1 CONNECTOR RTD3
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1RootPort|*|0x3
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1PwrEnableGpioNo|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_A_8 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1HoldRstGpioNo|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_D_19 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1PwrEnableGpioPolarity|*|$(PIN_GPIO_ACTIVE_HIGH)
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1HoldRstGpioPolarity|*|$(PIN_GPIO_ACTIVE_LOW)

  # PCIE SLOT 2 - X8 CONNECTOR RTD3
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot2RootPort|*|0xB
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot2PwrEnableGpioNo|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_F_10 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot2HoldRstGpioNo|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_D_9 }
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
   { GPIOV2_PTL_PCD_XXGPP_B_16 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdM2Ssd2RstGpio|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_E_3 }
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
   { GPIOV2_PTL_PCD_XXGPP_E_8 }
  )}
  # Root Port PEWAKE
   gBoardModuleTokenSpaceGuid.VpdPcdRootPortPewakeConfig|*|{CODE(
  {
    0,                           // PEWAKE Pin for RP01
    GPIOV2_PTL_PCD_XXGPP_E_2,    // PEWAKE Pin for RP02
    GPIOV2_PTL_PCD_XXGPP_B_25,   // PEWAKE Pin for RP03
    GPIOV2_PTL_PCD_XXGPP_A_12,   // PEWAKE Pin for RP04
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
  gBoardModuleTokenSpaceGuid.VpdPcdBoardRtd3TableSignature|*|{'P', 't', 'l', 'U', 'H', 'R', 'v', 'p'}
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

  # PTL-UH RVP1 power meter table
  gBoardModuleTokenSpaceGuid.VpdPcdPowerMeter|*|{CODE(
  { 7,    // DeviceSize
    '3',  // I2C_Scope
    {
     //BaseAddress, {RailName, ResistorValue}
     {    0x18,     {{"SoC_VCCCORE_PH1",    2}, {"SoC_VCCCORE_PH2", 2}, {"SoC_VCCCORE_PH3",    2}, {"SoC_VCCCORE_PH4", 2}}},
     {    0x1E,     {{"SoC_VCCGT_PH1",      2}, {"SoC_VCCGT_PH2",   2}, {"SoC_VCCSA",          2}, {"SoC_VDDQ",        2}}},
     {    0x11,     {{"SoC_VCCPRIM_VNNAON", 1}, {"SoC_VCCPRIM_IO",  4}, {"SoC_VCCPRIM3P3",    22}, {"SoC_VCCPRIM1P8", 10}}},
     {    0x15,     {{"SoC_VCCRTC",        22}, {"SoC_VCCST",       2}, {"SoC_VDD2_CPU",       2}, {"SoC_VDDQ_CPU",    5}}},
     {    0x19,     {{"DRAM_VDD2H",         2}, {"DRAM_VDD2L",      5}, {"DRAM_VDDQ",          5}, {"DRAM_VDD1",       5}}},
     {    0x14,     {{"DISPLAY_BKLIGHT",   10}, {"DISPLAY_VDD",    10}, {"CONNECTIVITY_WLAN",  5}, {"STORAGE_V3P3A",   5}}},
     {    0x17,     {{"SYSTEM_VBATA1",      2}, {"SYSTEM_VBATA2",   2}, {"CAMERA_V1P8S",      10}, {"CAMERA_V3P3S",  10}}},
    }
  })}

  # PTL-UH RVP1 Telemetry table
  gBoardModuleTokenSpaceGuid.VpdPcdTelemetry|*|{CODE(
  { 7,    // DeviceSize
    '3',  // I2C_Scope
    {
     //BaseAddress, {RailName, ResistorValue}
     {    0x18,     {{"SoC_VCCCORE_VR_IN",    2},  {"PCIE_X8_SLOT_V12", 2},  {"PCIE_X8_SLOT_V3P3",    5},  {"USBC_TCP2_SRC",        5}}},
     {    0x1E,     {{"SoC_VCCGT_VIN",        2},  {"USBC_TCP3_SRC",    5},  {"SoC_VCCSA_VIN",        2},  {"SoC_VCC_LP_ECORE_VIN", 2}}},
     {    0x11,     {{"SoC_VCCPRIM_VNNAON",   1},  {"SoC_VCCPRIM_IO",   4},  {"USBC_TCP0_SRC",        2},  {"SoC_VCCPRIM1P8",       10}}},
     {    0x15,     {{"SSD_GEN5_V3P3A",       5},  {"USBC_TCP1_SRC",    2},  {"SoC_VDD2_CPU",         2},  {"SoC_VDDQ_CPU",         5}}},
     {    0x19,     {{"LP5_VDD2H_VR_VIN",     5},  {"LP5_VR_VIN",       10}, {"UFS_V2P5S",            22}, {"UFS_V1P2S",            10}}},
     {    0x14,     {{"DISPLAY_BKLIGHT",      10}, {"DISPLAY_VDD",      10}, {"CONNECTIVITY_WLAN",    5},  {"SSD_GEN4_V3P3A",       5}}},
     {    0x17,     {{"SYSTEM_VBATA1",        2},  {"SYSTEM_VBATA2",    2},  {"CONNECTIVITY_WWAN",    5},  {"SYSTEM_ADP_INPUT",     10}}},
    }
  })}

  # PCIE CLOCK USGAGE for PTL H
  gBoardModuleTokenSpaceGuid.VpdPcdPcieClkUsageMap|*|{CODE(
  {{
    PCIE_PCH + 10,  // X8 CEM Slot
    PCIE_PCH + 8,   // Gen5 SSD / x4 PCIe slot
    PCIE_PCH + 2,   // x1 Pcie Slot
    LAN_CLOCK,      // GBE LAN
    PCIE_PCH + 3,   // M.2 WLAN
    PCIE_PCH + 1,   // M.2 WWAN
    PCIE_PCH + 4,   // Gen4 M.2 SSD
    NOT_USED,
    NOT_USED
  }}
  )}

  # PCIE CLOCK USGAGE for PTL P
  gBoardModuleTokenSpaceGuid.VpdPcdPPcieClkUsageMap|*|{CODE(
  {{
    NOT_USED,
    PCIE_PCH + 8,   // Gen5 SSD / x4 PCIe slot
    PCIE_PCH + 2,   // x1 Pcie Slot
    LAN_CLOCK,      // GBE LAN
    PCIE_PCH + 3,   // M.2 WLAN
    PCIE_PCH + 1,   // M.2 WWAN
    PCIE_PCH + 4,   // Gen4 M.2 SSD
    NOT_USED,
    NOT_USED
  }}
  )}


  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTable|*|{CODE(
  {
  //
  // x1 Slot
  //
    {GPIOV2_PTL_PCD_XXGPP_A_8,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // X1_PCIE_SLOT_PWR_EN
    {GPIOV2_PTL_PCD_XXGPP_D_19,  {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // X1_DT_PCIE_RST_N
    {GPIOV2_PTL_PCD_XXGPP_B_25,  {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirInInv,  GpioV2StateDefault, GpioV2IntLevel|GpioV2IntSci,  GpioV2ResetHostDeep,  GpioV2TermDefault,  GpioV2Unlock,  GpioV2Lock}},  // X1_SLOT_WAKE_N

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
  // M.2 WLAN
  //
    {GPIOV2_PTL_PCD_XXGPP_F_6,    {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirIn,     GpioV2StateDefault, GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // WLAN_WWAN_COEX3
    {GPIOV2_PTL_PCD_XXGPP_A_12,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirInInv,  GpioV2StateDefault, GpioV2IntLevel|GpioV2IntSci,  GpioV2ResetHostDeep,  GpioV2TermDefault,  GpioV2Unlock,  GpioV2Lock}}, // WIFI_WAKE_N
    {GPIOV2_PTL_PCD_XXGPP_A_16,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHostDeep,  GpioV2TermDefault}}, // BT_RF_KILL_N
    {GPIOV2_PTL_PCD_XXGPP_A_17,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHostDeep,  GpioV2TermDefault}}, // WIFI_RF_KILL_N
    {GPIOV2_PTL_PCD_XXGPP_A_11,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // WLAN_RST_N

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
    {GPIOV2_PTL_PCD_XXGPP_F_18,  {GpioV2PadModeGpio,  GpioV2HostOwnGpio,  GpioV2DirInInv,   GpioV2StateDefault,  GpioV2IntDis,       GpioV2ResetHost,     GpioV2TermNone,  GpioV2Unlock,  GpioV2Lock}},  // TCH_PAD_INT_N

  //
  // TCSS
  //
    {GPIOV2_PTL_PCD_XXGPP_V_17, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetResume,  GpioV2TermNone}}, // GPP_V17_TCP_RT_S0IX_ENTRY_EXIT_N
  //
  // Only for PNP
  //
    {GPIOV2_PTL_PCD_XXGPP_H_2,   {GpioV2PadModeGpio,  GpioV2HostOwnAcpi,  GpioV2DirOut,    GpioV2StateHigh,     GpioV2IntDis,    GpioV2ResetHost,    GpioV2TermDefault}},  // DEBUG_TRACE_PNP

  //
  // Not Used pins set to High Impedance
  //
    {GPIOV2_PTL_PCD_XXGPP_H_0,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    { 0x0 } // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdCoinlessSpiNorClearGpio|*|{CODE({
    {GPIOV2_PTL_PCD_XXGPP_D_14, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirIn,  GpioV2StateDefault,  GpioV2IntLevel, GpioV2ResetHost,  GpioV2TermDefault, GpioV2Unlock,  GpioV2Lock}},  // COINLESS_MODE_SELECT

    {0x0}
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdMrcDqsMapCpu2Dram| * |{CODE({
  //
  // PTL-UH LPDDR5x T3 DQS byte swizzling between CPU and DRAM
  //
  // Ch 0     1         2         3
  {{ 1, 0 }, { 0, 1 }, { 0, 1 }, { 0, 1 },  // Controller 0
   { 0, 1 }, { 0, 1 }, { 1, 0 }, { 0, 1 }}  // Controller 1
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdMrcSpdData|*|{CODE(
  {
    // LPDDR5x 315b 16Gb die, DDP 1Rx16, 4GB size per package
    // Hynix H58G56BK7BX068
    // 7500, ??-??-??-??
    // 4 Banks, 4 bank groups, 16Gb SDRAM density
    // 16 Row bits, 10 Column bits
    // Non-Monolithic DRAM Device, 2 dies, 2 Channels per package
     1,
     {0x23,                                 ///< 0   384 SPD bytes used, 512 total
      0x10,                                 ///< 1   SPD Revision 1.0
      0x15,                                 ///< 2   DRAM Type: LPDDR5x SDRAM
      0x0E,                                 ///< 3   Module Type: Not Hybrid (DRAM only) / Non-DIMM Solution (on-board DRAM)
      0x86,                                 ///< 4   4 Bank Groups, 4 Banks, 16 Gb SDRAM density
      0x21,                                 ///< 5   16 Rows, 10 Columns
      0x95,                                 ///< 6   Non-Monolithic DRAM Device, 2 die, 2 Channels per package, Signal Loading Matrix 1
      0x08,                                 ///< 7   SDRAM Optional Features: tMAW = 8192 * tREFI, Unlimited MAC
      0x00,                                 ///< 8   SDRAM Thermal / Refresh options: Reserved
      0x40,                                 ///< 9   Other SDRAM Optional Features: Post package repair supported, one row per bank group, Soft PPR not supported
      0x00,                                 ///< 10  Reserved
      0x00,                                 ///< 11  Module Nominal Voltage: Reserved
      0x02,                                 ///< 12  Module Organization: 1 Rank, x16 Device Width per Channel
      0x01,                                 ///< 13  Module Memory Bus width: 1 Channels, 16 bits channel width, no ECC
      0x00,                                 ///< 14  Module Thermal Sensor: none
      0x00,                                 ///< 15  Extended Module Type: Reserved
      0x48,                                 ///< 16  Signal Loading: Data/Strobe/Mask: 2 loads, CAC: 2 loads, CS: 1 load
      0x00,                                 ///< 17  MTB = 0.125ns, FTB = 1 ps
      0x09,                                 ///< 18  tCKAVGmin =  1.067 ns (LPDDR5X-7500 4:1)
      0xFF,                                 ///< 19  tCKAVGmax = 32.002 ns
      0x92,                                 ///< 20  CAS Latencies supported (First Byte) : 14, 10, 6
      0x55,                                 ///< 21  CAS Latencies supported (Second Byte): 28, 24, 20, 16
      0x05,                                 ///< 22  CAS Latencies supported (Third Byte) : 36, 32
      0x00,                                 ///< 23  CAS Latencies supported (Fourth Byte):
      0xAA,                                 ///< 24  Minimum CAS Latency (tAAmin) = 21.25 ns
      0x00,                                 ///< 25  Read and Write Latency Set options: Write Latency Set A and DBI-Read Disabled
      0x90,                                 ///< 26  Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
      0xA8,                                 ///< 27  Row precharge time for all banks (tRPab) = 21 ns
      0x90,                                 ///< 28  Minimum row precharge time (tRPpb) = 18 ns
      0xC0,                                 ///< 29  tRFCab = 280 ns (16 Gb)
      0x08,                                 ///< 30  tRFCab MSB
      0x60,                                 ///< 31  tRFCpb = 140 ns (16 Gb)
      0x04,                                 ///< 32  tRFCpb MSB
      0, 0, 0, 0, 0, 0, 0,                  ///< 33 - 39
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 40 - 49
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 50 - 59
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 60 - 69
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 70 - 79
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 80 - 89
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 90 - 99
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 100 - 109
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 110 - 119
      0x00,                                 ///< 120 FTB for Row precharge time per bank (tRPpb) = 18 ns
      0x00,                                 ///< 121 FTB for Row precharge time for all banks (tRPab) = 21 ns
      0x00,                                 ///< 122 FTB for Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
      0x00,                                 ///< 123 FTB for tAAmin = 21.25 ns
      0x7F,                                 ///< 124 FTB for tCKAVGmax = 32.002 ns
      0xC6,                                 ///< 125 FTB for tCKAVGmin = -58ps
      0x00,                                 ///< 126 CRC A
      0x00,                                 ///< 127 CRC B
      0, 0,                                 ///< 128 - 129
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 130 - 139
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 140 - 149
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 150 - 159
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 160 - 169
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 170 - 179
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 180 - 189
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 190 - 199
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 200 - 209
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 210 - 219
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 220 - 229
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 230 - 239
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 240 - 249
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 250 - 259
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 260 - 269
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 270 - 279
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 280 - 289
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 290 - 299
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 300 - 309
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 310 - 319
      0x00,                                 ///< 320 Module Manufacturer ID Code, Least Significant Byte
      0x00,                                 ///< 321 Module Manufacturer ID Code, Most Significant Byte
      0x00,                                 ///< 322 Module Manufacturing Location
      0x00,                                 ///< 323 Module Manufacturing Date Year
      0x00,                                 ///< 324 Module Manufacturing Date Week
      0x20,                                 ///< 325 Module ID: Module Serial Number
      0x00,                                 ///< 326 Module Serial Number B
      0x00,                                 ///< 327 Module Serial Number C
      0x00,                                 ///< 328 Module Serial Number D
      0x20, 0x20, 0x20, 0x20, 0x20,         ///< 329 - 333 Module Part Number: Unused bytes coded as ASCII Blanks (0x20)
      0x20, 0x20, 0x20, 0x20, 0x20,         ///< 334 - 338 Module Part Number
      0x20, 0x20, 0x20, 0x20, 0x20,         ///< 339 - 343 Module Part Number
      0x20, 0x20, 0x20, 0x20, 0x20,         ///< 344 - 348 Module Part Number
      0x00,                                 ///< 349 Module Revision Code
      0x00,                                 ///< 350 DRAM Manufacturer ID Code, Least Significant Byte
      0x00,                                 ///< 351 DRAM Manufacturer ID Code, Most Significant Byte
      0x00,                                 ///< 352 DRAM Stepping
      0, 0, 0, 0, 0, 0, 0,                  ///< 353 - 359
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 360 - 369
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 370 - 379
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 380 - 389
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 390 - 399
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 400 - 409
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 410 - 419
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 420 - 429
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 430 - 439
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 440 - 449
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 450 - 459
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 460 - 469
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 470 - 479
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 480 - 489
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 490 - 499
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 500 - 509
      0, 0                                  ///< 510 - 511
  }})}

  gBoardModuleTokenSpaceGuid.VpdPcdMrcDqMapCpu2Dram|*|{CODE({{
  //
  // PTL-UH Lp5 DQ bit swizzling between CPU and DRAM
  //

  //Controller 0
  {{  13,  14,  12, 15, 11, 10, 8,  9  },   // Byte 0
   {  7,   5,   4,  6,  0,  3,  1,  2  }},  // Byte 1
  {{  1,   3,   0,  2,  7,  4,  6,  5  },   // Byte 2
   {  12,  13,  14, 15, 11, 10, 9,  8  }},  // Byte 3
  {{  0,   2,   1,  3,  6,  4,  7,  5  },   // Byte 4
   {  14,  13,  15, 12, 8,  11, 10, 9  }},  // Byte 5
  {{  6,   5,   7,  4,  2,  3,  1,  0  },   // Byte 6
   {  10,  8,   11, 9,  12, 15, 13, 14 }},  // Byte 7

  //Controller 1
  {{  2,   1,   3,  0,  4,  7,  5,  6  },   // Byte 0
   {  15,  14,  12, 13, 9,  11, 10, 8  }},  // Byte 1
  {{  6,   5,   7,  4,  3,  1,  0,  2  },   // Byte 2
   {  10,  9,   11, 8,  13, 14, 12, 15 }},  // Byte 3
  {{  9,   10,  11, 8,  14, 12, 13, 15 },   // Byte 4
   {  0,   1,   2,  3,  5,  7,  4,  6  }},  // Byte 5
  {{  0,   1,   2,  3,  7,  5,  6,  4  },   // Byte 6
   {  14,  13,  15, 12, 10, 8,  11, 9  }},  // Byte 7
  }})}

  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableEarlyPreMem|*|{CODE({
    //
    // Lid Switch Wake Gpio
    //
    {GPIOV2_PTL_PCD_XXGPP_F_23, {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirIn,  GpioV2StateDefault,  GpioV2IntDis, GpioV2ResetHostDeep,  GpioV2TermDefault}}, // SMC_LID
    {0x0}  // terminator
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
      3,                            // WlanRootPortNumber
      8,                            // DiscreteUsbBtPortNumber
      8,                            // IntegratedUsbBtPortNumber
      GPIOV2_PTL_PCD_XXGPP_A_17,    // WlanRfKillGpio
      GPIOV2_PTL_PCD_XXGPP_A_12,    // WlanWakeGpio
      GPIOV2_PTL_PCD_XXGPP_A_11,    // WlanRstGpio
      GPIOV2_PTL_PCD_XXGPP_A_16,    // BtRfKillGpio
      0                             // BtIrqGpio
    }
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdClwlI2cController|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdClwlI2cSlaveAddress|*|0x0

[PcdsDynamicHii.common.SkuIdPtlUHLp5Rvp1.STANDARD]
  gStructPcdTokenSpaceGuid.PcdSetup.PchI2cSensorDevicePort[5]|0x1                                    # Connected device
  gStructPcdTokenSpaceGuid.PcdSetup.TelemetryDeviceEnable|0x1                                        # Enable Telemetry
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioDmicLinkEnable[0]|0x0                               # DMIC #0
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioDmicLinkEnable[1]|0x0                               # DMIC #1
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshI2cEnable[1]|0x0                                        # I2C1
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshSpiEnable[0]|0x1                                        # SPI_0


#####################################################################
#  PTL UH LP5x T3 - RVP1 BOM
#     Board ID: 0x01
#     Bom ID  : 0x06 (BomIdPtlUHLp5Rvp)
#   Display ID: 0x00
#       SKU ID: 0x00060001 (SkuIdPtlUHLp5Rvp1Bom)
#####################################################################
[PcdsDynamicExVpd.common.SkuIdPtlUHLp5Rvp1Bom]
  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTable|*|{CODE(
  {
  //
  // x1 Slot
  //
    {GPIOV2_PTL_PCD_XXGPP_A_8,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // X1_PCIE_SLOT_PWR_EN
    {GPIOV2_PTL_PCD_XXGPP_D_19,  {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // X1_DT_PCIE_RST_N
    {GPIOV2_PTL_PCD_XXGPP_B_25,  {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirInInv,  GpioV2StateDefault, GpioV2IntLevel|GpioV2IntSci,  GpioV2ResetHostDeep,  GpioV2TermDefault,  GpioV2Unlock,  GpioV2Lock}},  // X1_SLOT_WAKE_N

  //
  // x4 Slot
  //
    {GPIOV2_PTL_PCD_XXGPP_D_25,  {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirInInv,  GpioV2StateDefault, GpioV2IntLevel|GpioV2IntSci,  GpioV2ResetHostDeep,  GpioV2TermDefault,  GpioV2Unlock,  GpioV2Lock}},  // X4_SLOT_WAKE_N
    {GPIOV2_PTL_PCD_XXGPP_B_16,  {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateLow,     GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}},  // X4_PCIE_SLOT_PWR_EN_N
    {GPIOV2_PTL_PCD_XXGPP_E_3,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}},  // M2_GEN5_SSD_RESET_N

  //
  // x8 Slot \ PEG
  //
    {GPIOV2_PTL_PCD_XXGPP_E_19,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // PEG_SLOT_DGPU_SEL_N
    {GPIOV2_PTL_PCD_XXGPP_F_10,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateLow,     GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // PEG_SLOT_PWR_EN_N
    {GPIOV2_PTL_PCD_XXGPP_D_9,    {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // PEG_SLOT_RST_N
    {GPIOV2_PTL_PCD_XXGPP_D_24,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirInInv,  GpioV2StateDefault, GpioV2IntLevel|GpioV2IntSci,  GpioV2ResetHostDeep,  GpioV2TermDefault, GpioV2Unlock,  GpioV2Lock}}, // PEG_SLOT_WAKE_N
    {GPIOV2_PTL_PCD_XXGPP_E_20,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirIn,     GpioV2StateDefault,  GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // PEG_SLOT_DGPU_PWR_OK

  //
  // M.2 SSD Gen4
  //
    {GPIOV2_PTL_PCD_XXGPP_B_10,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,     GpioV2IntDis,      GpioV2ResetHost,  GpioV2TermDefault}}, //GEN4_SSD_PWREN
    {GPIOV2_PTL_PCD_XXGPP_B_9,    {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,      GpioV2ResetHost,  GpioV2TermDefault}},  // M2_GEN4_SSD_RESET_N

  //
  // M.2 WLAN
  //
    {GPIOV2_PTL_PCD_XXGPP_F_6,    {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirIn,     GpioV2StateDefault, GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // WLAN_WWAN_COEX3
    {GPIOV2_PTL_PCD_XXGPP_A_12,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirInInv,  GpioV2StateDefault, GpioV2IntLevel|GpioV2IntSci,  GpioV2ResetHostDeep,  GpioV2TermDefault,  GpioV2Unlock,  GpioV2Lock}}, // WIFI_WAKE_N
    {GPIOV2_PTL_PCD_XXGPP_A_16,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHostDeep,  GpioV2TermDefault}}, // BT_RF_KILL_N
    {GPIOV2_PTL_PCD_XXGPP_A_17,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHostDeep,  GpioV2TermDefault}}, // WIFI_RF_KILL_N
    {GPIOV2_PTL_PCD_XXGPP_A_11,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // WLAN_RST_N

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
    {GPIOV2_PTL_PCD_XXGPP_F_18,  {GpioV2PadModeGpio,  GpioV2HostOwnGpio,  GpioV2DirInInv,   GpioV2StateDefault,  GpioV2IntDis,     GpioV2ResetHost,     GpioV2TermNone,  GpioV2Unlock,  GpioV2Lock}},  // TCH_PAD_INT_N

  //
  // TCSS
  //
    {GPIOV2_PTL_PCD_XXGPP_V_17, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetResume,  GpioV2TermNone}}, // GPP_V17_TCP_RT_S0IX_ENTRY_EXIT_N

    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdM2Ssd2PowerEnableGpioPolarity|*|$(PIN_GPIO_ACTIVE_LOW)

  # Root Port PEWAKE
   gBoardModuleTokenSpaceGuid.VpdPcdRootPortPewakeConfig|*|{CODE(
  {
    0,                           // PEWAKE Pin for RP01
    GPIOV2_PTL_PCD_XXGPP_E_2,    // PEWAKE Pin for RP02
    GPIOV2_PTL_PCD_XXGPP_B_25,   // PEWAKE Pin for RP03
    GPIOV2_PTL_PCD_XXGPP_A_12,   // PEWAKE Pin for RP04
    0,                           // PEWAKE Pin for RP05
    0,                           // PEWAKE Pin for RP06
    0,                           // PEWAKE Pin for RP07
    0,                           // PEWAKE Pin for RP08
    GPIOV2_PTL_PCD_XXGPP_D_25,   // PEWAKE Pin for RP09
    0,                           // PEWAKE Pin for RP10
    GPIOV2_PTL_PCD_XXGPP_D_24,   // PEWAKE Pin for RP11
    0,                           // PEWAKE Pin for RP12
    0xFFFFFFFF                   // terminator
  })}


#####################################################################
#  PTL UH LP5x T3 MEMSD 32GB EPS
#     Board ID: 0x01
#     Bom ID: 0x03 (BomIdPtlUHLp5EpsRvp)
#   Display ID: 0x00
#       SKU ID: 0x00030001 (SkuIdPtlUHLp5MemSdEpsRvp1)
#####################################################################
[PcdsDynamicExVpd.common.SkuIdPtlUHLp5MemSdEpsRvp1]

  gBoardModuleTokenSpaceGuid.VpdPcdMrcSpdData|*|{CODE(
  {
    // LPDDR5x 315b 16Gb die, DDP 2Rx16 QDP, 8GB size per package
    // Hynix H58G66BK7BX067
    // 7500, ??-??-??-??
    // 4 Banks, 4 bank groups, 16Gb SDRAM density
    // 16 Row bits, 11 Column bits
    // Non-Monolithic DRAM Device, 2 dies, 2 Channels per package
     1,
     {0x23,                                 ///< 0   384 SPD bytes used, 512 total
      0x10,                                 ///< 1   SPD Revision 1.0
      0x15,                                 ///< 2   DRAM Type: LPDDR5x SDRAM
      0x0E,                                 ///< 3   Module Type: Not Hybrid (DRAM only) / Non-DIMM Solution (on-board DRAM)
      0x16,                                 ///< 4   4 Bank Groups, 4 Banks, 16 Gb SDRAM density
      0x22,                                 ///< 5   16 Rows, 11 Columns
      0xB5,                                 ///< 6   Non-Monolithic DRAM Device, 2 die, 2 Channels per package, Signal Loading Matrix 1
      0x08,                                 ///< 7   SDRAM Optional Features: tMAW = 8192 * tREFI, Unlimited MAC
      0x00,                                 ///< 8   SDRAM Thermal / Refresh options: Reserved
      0x40,                                 ///< 9   Other SDRAM Optional Features: Post package repair supported, one row per bank group, Soft PPR not supported
      0x00,                                 ///< 10  Reserved
      0x00,                                 ///< 11  Module Nominal Voltage: Reserved
      0x0A,                                 ///< 12  Module Organization: 2 Rank, x16 Device Width per Channel
      0x01,                                 ///< 13  Module Memory Bus width: 1 Channels, 16 bits channel width, no ECC
      0x00,                                 ///< 14  Module Thermal Sensor: none
      0x00,                                 ///< 15  Extended Module Type: Reserved
      0x48,                                 ///< 16  Signal Loading: Data/Strobe/Mask: 2 loads, CAC: 2 loads, CS: 1 load
      0x00,                                 ///< 17  MTB = 0.125ns, FTB = 1 ps
      0x08,                                 ///< 18  tCKAVGmin = 0.937 ns (LPDDR5X-8533 4:1)
      0xFF,                                 ///< 19  tCKAVGmax = 32.002 ns
      0x92,                                 ///< 20  CAS Latencies supported (First Byte) : 14, 10, 6
      0x55,                                 ///< 21  CAS Latencies supported (Second Byte): 28, 24, 20, 16
      0x05,                                 ///< 22  CAS Latencies supported (Third Byte) : 36, 32
      0x00,                                 ///< 23  CAS Latencies supported (Fourth Byte):
      0xAA,                                 ///< 24  Minimum CAS Latency (tAAmin) = 21.25 ns
      0x00,                                 ///< 25  Read and Write Latency Set options: Write Latency Set A and DBI-Read Disabled
      0x90,                                 ///< 26  Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
      0xA8,                                 ///< 27  Row precharge time for all banks (tRPab) = 21 ns
      0x90,                                 ///< 28  Minimum row precharge time (tRPpb) = 18 ns
      0xC0,                                 ///< 29  tRFCab = 280 ns (16 Gb)
      0x08,                                 ///< 30  tRFCab MSB
      0x60,                                 ///< 31  tRFCpb = 140 ns (16 Gb)
      0x04,                                 ///< 32  tRFCpb MSB
      0, 0, 0, 0, 0, 0, 0,                  ///< 33 - 39
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 40 - 49
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 50 - 59
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 60 - 69
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 70 - 79
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 80 - 89
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 90 - 99
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 100 - 109
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 110 - 119
      0x00,                                 ///< 120 FTB for Row precharge time per bank (tRPpb) = 18 ns
      0x00,                                 ///< 121 FTB for Row precharge time for all banks (tRPab) = 21 ns
      0x00,                                 ///< 122 FTB for Minimum RAS-to-CAS delay (tRCDmin) = 18 ns
      0x00,                                 ///< 123 FTB for tAAmin = 21.25 ns
      0x7F,                                 ///< 124 FTB for tCKAVGmax = 32.002 ns
      0x43,                                 ///< 125 FTB for tCKAVGmin = -58ps
      0x00,                                 ///< 126 CRC A
      0x00,                                 ///< 127 CRC B
      0, 0,                                 ///< 128 - 129
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 130 - 139
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 140 - 149
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 150 - 159
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 160 - 169
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 170 - 179
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 180 - 189
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 190 - 199
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 200 - 209
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 210 - 219
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 220 - 229
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 230 - 239
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 240 - 249
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 250 - 259
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 260 - 269
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 270 - 279
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 280 - 289
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 290 - 299
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 300 - 309
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 310 - 319
      0x00,                                 ///< 320 Module Manufacturer ID Code, Least Significant Byte
      0x00,                                 ///< 321 Module Manufacturer ID Code, Most Significant Byte
      0x00,                                 ///< 322 Module Manufacturing Location
      0x00,                                 ///< 323 Module Manufacturing Date Year
      0x00,                                 ///< 324 Module Manufacturing Date Week
      0x20,                                 ///< 325 Module ID: Module Serial Number
      0x00,                                 ///< 326 Module Serial Number B
      0x00,                                 ///< 327 Module Serial Number C
      0x00,                                 ///< 328 Module Serial Number D
      0x20, 0x20, 0x20, 0x20, 0x20,         ///< 329 - 333 Module Part Number: Unused bytes coded as ASCII Blanks (0x20)
      0x20, 0x20, 0x20, 0x20, 0x20,         ///< 334 - 338 Module Part Number
      0x20, 0x20, 0x20, 0x20, 0x20,         ///< 339 - 343 Module Part Number
      0x20, 0x20, 0x20, 0x20, 0x20,         ///< 344 - 348 Module Part Number
      0x00,                                 ///< 349 Module Revision Code
      0x00,                                 ///< 350 DRAM Manufacturer ID Code, Least Significant Byte
      0x00,                                 ///< 351 DRAM Manufacturer ID Code, Most Significant Byte
      0x00,                                 ///< 352 DRAM Stepping
      0, 0, 0, 0, 0, 0, 0,                  ///< 353 - 359
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 360 - 369
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 370 - 379
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 380 - 389
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 390 - 399
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 400 - 409
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 410 - 419
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 420 - 429
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 430 - 439
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 440 - 449
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 450 - 459
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 460 - 469
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 470 - 479
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 480 - 489
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 490 - 499
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 500 - 509
      0, 0                                  ///< 510 - 511
  }})}