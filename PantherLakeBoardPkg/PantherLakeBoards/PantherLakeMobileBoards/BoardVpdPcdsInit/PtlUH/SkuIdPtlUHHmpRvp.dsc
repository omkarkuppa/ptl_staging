## @file
#  Board description file initializes configuration (PCD) settings for the project.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2023 Intel Corporation.
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
# PTL UH HMP RVP
#           Board ID: 0x9 (BoardIdPtlUHHmpRvp)
#         Display ID: 0x0 (DDIA eDP configuration)
#             SKU ID: 0x9 (SkuIdPtlUHHmpRvp)
#####################################################################
[PcdsDynamicExVpd.common.SkuIdPtlUHHmpRvp]
  gI2cFeaturePkgTokenSpaceGuid.VpdPcdPssReadSN|*|FALSE
  gI2cFeaturePkgTokenSpaceGuid.VpdPcdPssI2cDeviceAddress|*|0x0
  gI2cFeaturePkgTokenSpaceGuid.VpdPcdPssI2cBusNumber|*|0x00
  gBoardModuleTokenSpaceGuid.VpdPcdSpdAddressOverride|*|FALSE

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

  gBoardModuleTokenSpaceGuid.VpdPcdGpioGroupToGpeDw0|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdGpioGroupToGpeDw1|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdGpioGroupToGpeDw2|*|0x0

  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableWwanOnEarlyPreMem| * |{CODE(
  {
    { 0x0 }
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableWwanOffEarlyPreMem| * |{CODE(
  {
    { 0x0 }
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableM80WwanOnEarlyPreMem| * |{CODE(
  {
    { 0x0 }
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdSaMiscUserBd|*|0x05
  gBoardModuleTokenSpaceGuid.VpdPcdSaDdrFreqLimit|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdMrcSpdDataSize|*|512
  gBoardModuleTokenSpaceGuid.VpdPcdMrcSpdAddressTable| * |{CODE(
  { 16,
    { 0xA0 ,   // Port 0
      0x00 ,   // Port 1
      0xA0 ,   // Port 2
      0x00 ,   // Port 3
      0xA0 ,   // Port 4
      0x00 ,   // Port 5
      0xA0 ,   // Port 6
      0x00 ,   // Port 7
      0xA4 ,   // Port 8
      0x00 ,   // Port 9
      0xA4 ,   // Port 10
      0x00 ,   // Port 11
      0xA4 ,   // Port 12
      0x00 ,   // Port 13
      0xA4 ,   // Port 14
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
  gBoardModuleTokenSpaceGuid.VpdPcdMrcRcompResistor|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdMrcRcompTarget| * |{CODE(
  {  5,
     { 0x0, 0x0, 0x0, 0x0, 0x0 }
  })}
  gBoardModuleTokenSpaceGuid.VpdPcdMrcDqPinsInterleavedControl|*|TRUE
  gBoardModuleTokenSpaceGuid.VpdPcdMrcDqPinsInterleaved|*|TRUE
  gBoardModuleTokenSpaceGuid.VpdPcdMrcLp5CccConfig|*|0x0
  #
  # CA Vref routing: board-dependent
  # 0 - VREF_CA goes to both CH_A and CH_B (LPDDR3/DDR3L)
  # 1 - VREF_CA to CH_A, VREF_DQ_A to CH_B (should not be used)
  # 2 - VREF_CA to CH_A, VREF_DQ_B to CH_B (DDR4)
  #
  # Set it to 2 for all our DDR4 boards; it is ignored for LPDDR4
  #
  gBoardModuleTokenSpaceGuid.VpdPcdMrcCaVrefConfig|*|0x2
  gBoardModuleTokenSpaceGuid.VpdPcdMrcCmdMirror|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdVdd2lPowerRailPresent|*|0x1
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

  # No. of TypeC iTBT ports
  # Port3, Port2, Port1
  gBoardModuleTokenSpaceGuid.VpdPcdBoardRetimerForcePwrGpio|*|{CODE(
    { GPIOV2_PTL_PCD_XXGPP_D_20 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdRecoveryModeGpio|*|{CODE(
   { BoardGpioTypeNotSupported }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdWwanFullCardPowerOffGpio|*|{CODE(
   { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdWwanBbrstGpio|*|{CODE(
   { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdWwanPerstGpio|*|{CODE(
   { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdWwanWakeGpio|*|{CODE(
   { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdWwanFullCardPowerOffGpioPolarity|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdWwanBbrstGpioPolarity|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdWwanPerstGpioPolarity|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdWwanSourceClock|*|0
  gBoardModuleTokenSpaceGuid.VpdPcdWwanRootPortNumber|*|0
  gBoardModuleTokenSpaceGuid.VpdPcdCpuRatio|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdBiosGuard|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdOddPowerInitEnable|*|FALSE
  gBoardModuleTokenSpaceGuid.VpdPcdSmbusAlertEnable|*|FALSE
  gBoardModuleTokenSpaceGuid.VpdPcdEcFailSafeActionCpuTemp|*|85
  gBoardModuleTokenSpaceGuid.VpdPcdEcFailSafeActionFanPwm|*|65


  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableTouchPanel0|*|{CODE({
  // Touch Panel 0, Same pins shared between THC and I2C based Panel,
    {GPIOV2_PTL_PCD_XXGPP_A_8, {GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirOut,  GpioV2StateHigh, GpioV2IntDis, GpioV2ResetHost, GpioV2TermDefault}},  //TCH_PNL1_PWR_EN
    {GPIOV2_PTL_PCD_XXGPP_E_16,{GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirOut,  GpioV2StateHigh,  GpioV2IntDis,  GpioV2ResetHost,  GpioV2TermDefault}},  // GPP_E_16_THC0_SPI1_RST_B
    {GPIOV2_PTL_PCD_XXGPP_E_18,{GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirInInv,GpioV2StateDefault, GpioV2IntEdge,  GpioV2ResetHost,  GpioV2TermNone,  GpioV2Unlock,  GpioV2Lock}},  // GPP_E_18_THC0_SPI1_INT_B
    { 0x0 }
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableTouchPanel1|*|{CODE({
    // Touch Panel 1, Not used by default in RVP; Applicable for Converge Mobility RVP SKU only;
    {GPIOV2_PTL_PCD_XXGPP_A_9, {GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirOut,  GpioV2StateHigh, GpioV2IntDis, GpioV2ResetHost, GpioV2TermDefault}},  //TCH_PNL2_PWR_EN
    {GPIOV2_PTL_PCD_XXGPP_F_16,{GpioV2PadModeGpio,  GpioV2HostOwnAcpi,  GpioV2DirOut,  GpioV2StateHigh,  GpioV2IntDis,  GpioV2ResetHost,  GpioV2TermDefault}},  // THC1_SPI2_RST_N_TCH_PNL2
    {GPIOV2_PTL_PCD_XXGPP_F_18,{GpioV2PadModeGpio,  GpioV2HostOwnGpio,  GpioV2DirInInv,  GpioV2StateDefault,  GpioV2IntEdge,  GpioV2ResetHost,  GpioV2TermNone,  GpioV2Unlock,  GpioV2Lock  }},  // THC1_SPI2_INT_N_TCH_PNL2
    { 0x0 }
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableLch|*|{CODE({
    { GPIOV2_PTL_PCD_XXGPP_F_20,{ GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirIn, GpioV2StateDefault, GpioV2IntEdge|GpioV2IntApic, GpioV2ResetHost, GpioV2TermNone, GpioV2Unlock, GpioV2Lock}}, //GPP_PRIVACY_LED_CAM1_CVS_HST_WAKE
    { GPIOV2_PTL_PCD_XXGPP_E_10, { GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirOut, GpioV2StateHigh, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermNone, GpioV2Unlock, GpioV2Lock}}, // CRD1_RST_N
    { 0x0 }
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdLchIrqGpioPin|*|{CODE(
    { GPIOV2_PTL_PCD_XXGPP_F_20 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdLchResetGpioPin|*|{CODE(
    { GPIOV2_PTL_PCD_XXGPP_E_10 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdVrAlertEnable|*|FALSE
  gBoardModuleTokenSpaceGuid.VpdPcdPchThermalHotEnable|*|FALSE
  gBoardModuleTokenSpaceGuid.VpdPcdMemoryThermalSensorGpioCPmsyncEnable|*|FALSE
  gBoardModuleTokenSpaceGuid.VpdPcdMemoryThermalSensorGpioDPmsyncEnable|*|FALSE


  # PCIE GEN4 - X4 CONNECTOR RTD3
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1RootPort|*|0x1
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1PwrEnableGpioNo|*|{CODE(
   { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1HoldRstGpioNo|*|{CODE(
   { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1PwrEnableGpioPolarity|*|$(PIN_GPIO_ACTIVE_HIGH)
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1HoldRstGpioPolarity|*|$(PIN_GPIO_ACTIVE_HIGH)

  #Mipi Cam
  gBoardModuleTokenSpaceGuid.VpdPcdMipiCamGpioEnable|*|0x0

  # Touch Pad
  gBoardModuleTokenSpaceGuid.VpdPcdTouchpadIrqGpio|*|{CODE(
   { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdTouchpadIrqGpioPolarity|*|0x0

  # Touch Panel 0
  gBoardModuleTokenSpaceGuid.VpdPcdTouchpanel0IrqGpio|*|{CODE(
   { 0x0 }
  )}

  #Touch Panel 1
  gBoardModuleTokenSpaceGuid.VpdPcdTouchpanel1IrqGpio|*|{CODE(
   { 0x0 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdUsbCEcSupportPdInfoPresent|*|FALSE # USB Type-C PD Information Present : TRUE - Enable

  # USB Type-C Retimer Information Present : FALSE - Disable, TRUE - Enable
  gBoardModuleTokenSpaceGuid.VpdPcdUsbCEcSupportRetimerInfoPresent|*|FALSE
  gBoardModuleTokenSpaceGuid.VpdPcdUsbCRetimerFlash|*|{CODE(
  {
   { 0x0 }  // terminator
  })}

  # UsbC Retimer Capsule Debug Progress Code Print: FALSE - Disable, TRUE - Enable
  gBoardModuleTokenSpaceGuid.VpdPcdUsbCCapsuleProgressCodeEnable|*|FALSE

  # Control the EC debug Info Print level before executing any command for PD Bridge, default is DEBUG_VERBOSE
  gBoardModuleTokenSpaceGuid.VpdPcdPdBridgeDebugInfoPrintLevel|*|0x00400000

  gBoardModuleTokenSpaceGuid.VpdPcdTcssPmcPdEnable|*|TRUE
  gBoardModuleTokenSpaceGuid.VpdPcdUsbCUcmMode|*|0x1 # 0: Unsupported, 1: UCSI, 2: UCMCx

  gBoardModuleTokenSpaceGuid.VpdPcdHdaI2sCodecIrqGpio|*|{CODE(
   { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdHdaI2sCodecI2cBusNumber|*|0x2
  gBoardModuleTokenSpaceGuid.VpdPcdWwanModemBaseBandResetGpio|*|{CODE(
   { 0x0 }
  )}
  # Root Port PEWAKE
  gBoardModuleTokenSpaceGuid.VpdPcdRootPortPewakeConfig|*|{CODE(
  {
    0,                           // PEWAKE Pin for RP01
    0,                           // PEWAKE Pin for RP02
    0,                           // PEWAKE Pin for RP03
    0,                           // PEWAKE Pin for RP04
    0,                           // PEWAKE Pin for RP05
    0,                           // PEWAKE Pin for RP06
    0,                           // PEWAKE Pin for RP07
    0,                           // PEWAKE Pin for RP08
    0,                           // PEWAKE Pin for RP09
    0,                           // PEWAKE Pin for RP10
    0,                           // PEWAKE Pin for RP11
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
   { 0x0 }
  )}

  # LID status for gop
  gBoardModuleTokenSpaceGuid.VpdPcdGpioLidStatus|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_F_23 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdSkuType|*|0x2
  gBoardModuleTokenSpaceGuid.VpdPcdDisableMrcRetraining|*|0x0

  # PCIE CLOCK USAGE for PTL P
  gBoardModuleTokenSpaceGuid.VpdPcdPPcieClkUsageMap|*|{CODE(
  {{
    PCIE_PCH + 8, // PXPC 1x4
    NOT_USED,
    PCIE_PCH,     // PXPA 1x4
    NOT_USED,
    NOT_USED,
    NOT_USED,
  }}
  )}

###
### !!! GPIOs designated to Native Functions shall not be configured by Platform Code.
### Native Pins shall be configured by Silicon Code (based on BIOS policies setting) or soft straps(set by CSME in FITc).
###
###
  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTable|*|{CODE({
    // TCSS
    {GPIOV2_PTL_PCD_XXGPP_A_12,{GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirOut,  GpioV2StateLow,  GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //TCP0_AUX_BIAS_P
    {GPIOV2_PTL_PCD_XXGPP_A_17,{GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirOut,  GpioV2StateHigh, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //TCP0_AUX_BIAS_N

    //EC
    {GPIOV2_PTL_PCD_XXGPP_E_7, {GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirInInv,  GpioV2StateDefault,GpioV2IntLevel|GpioV2IntSci, GpioV2ResetHost,    GpioV2TermDefault,  GpioV2Unlock, GpioV2Lock  }},  //EC_SMI_N

    //PCIE4
    {GPIOV2_PTL_PCD_XXGPP_D_4, {GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirOut,   GpioV2StateHigh,    GpioV2IntDis,                GpioV2ResetHost,     GpioV2TermDefault}},  //PCIE4_PWR_EN_1
    {GPIOV2_PTL_PCD_XXGPP_F_23,{GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirOut,   GpioV2StateHigh,    GpioV2IntDis,                GpioV2ResetHost,     GpioV2TermDefault}},  //PCIE4_RST_1
    {GPIOV2_PTL_PCD_XXGPP_H_16,{GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirInInv, GpioV2StateDefault, GpioV2IntLevel|GpioV2IntSci, GpioV2ResetHostDeep, GpioV2TermDefault, GpioV2Unlock, GpioV2Lock  }},  //PCIE4_WAKE_1_N

    {GPIOV2_PTL_PCD_XXGPP_C_22,{GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirOut,   GpioV2StateHigh,    GpioV2IntDis,                GpioV2ResetHost,     GpioV2TermDefault}}, //PCIE4_PWR_EN_2
    {GPIOV2_PTL_PCD_XXGPP_D_14,{GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirOut,   GpioV2StateHigh,    GpioV2IntDis,                GpioV2ResetHost,     GpioV2TermDefault}}, //PCIE4_RST_2
    {GPIOV2_PTL_PCD_XXGPP_B_9, {GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirInInv, GpioV2StateDefault, GpioV2IntLevel|GpioV2IntSci, GpioV2ResetHostDeep, GpioV2TermDefault, GpioV2Unlock, GpioV2Lock  }},  //PCIE4_WAKE_2_N

    {GPIOV2_PTL_PCD_XXGPP_C_20,{GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirOut,   GpioV2StateHigh,    GpioV2IntDis,                GpioV2ResetHost,     GpioV2TermDefault}},  //PCIE4_PWR_EN_3
    {GPIOV2_PTL_PCD_XXGPP_D_15,{GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirOut,   GpioV2StateHigh,    GpioV2IntDis,                GpioV2ResetHost,     GpioV2TermDefault}},  //PCIE4_RST_3
    {GPIOV2_PTL_PCD_XXGPP_D_0, {GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirInInv, GpioV2StateDefault, GpioV2IntLevel|GpioV2IntSci, GpioV2ResetHostDeep, GpioV2TermDefault, GpioV2Unlock, GpioV2Lock  }},  //PCIE4_WAKE_3_N

    {GPIOV2_PTL_PCD_XXGPP_C_21,{GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirOut,   GpioV2StateHigh,    GpioV2IntDis,                GpioV2ResetHost,     GpioV2TermDefault}},  //PCIE4_PWR_EN_4
    {GPIOV2_PTL_PCD_XXGPP_B_15,{GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirOut,   GpioV2StateHigh,    GpioV2IntDis,                GpioV2ResetHost,     GpioV2TermDefault}},  //PCIE4_RST_4
    {GPIOV2_PTL_PCD_XXGPP_D_1, {GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirInInv, GpioV2StateDefault, GpioV2IntLevel|GpioV2IntSci, GpioV2ResetHostDeep, GpioV2TermDefault, GpioV2Unlock, GpioV2Lock  }},  //PCIE4_WAKE_4_N

    //PCIE5 Slot
    {GPIOV2_PTL_PCD_XXGPP_F_10,{GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirOut,   GpioV2StateHigh,   GpioV2IntDis,                GpioV2ResetHost,     GpioV2TermDefault}},  //PCIE5_PWR_EN_1
    {GPIOV2_PTL_PCD_XXGPP_D_18,{GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirOut,   GpioV2StateHigh,   GpioV2IntDis,                GpioV2ResetHost,     GpioV2TermDefault}},  //PCIE5_RST_1
    {GPIOV2_PTL_PCD_XXGPP_B_10,{GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirInInv, GpioV2StateDefault,GpioV2IntLevel|GpioV2IntSci, GpioV2ResetHostDeep, GpioV2TermDefault, GpioV2Unlock, GpioV2Lock  }},  //PCIE5_WAKE_1_N

    {GPIOV2_PTL_PCD_XXGPP_D_2, {GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirOut,   GpioV2StateHigh,   GpioV2IntDis,                GpioV2ResetHost,     GpioV2TermDefault}},  //PCIE5_PWR_EN_2
    {GPIOV2_PTL_PCD_XXGPP_F_9, {GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirOut,   GpioV2StateHigh,   GpioV2IntDis,                GpioV2ResetHost,     GpioV2TermDefault}},  //PCIE5_RST_2
    {GPIOV2_PTL_PCD_XXGPP_B_16,{GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirInInv, GpioV2StateDefault,GpioV2IntLevel|GpioV2IntSci, GpioV2ResetHostDeep, GpioV2TermDefault, GpioV2Unlock , GpioV2Lock  }},  //PCIE5_WAKE_2_N

    // Not connected
    {GPIOV2_PTL_PCD_XXGPP_B_14,{GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //STRAP
    {GPIOV2_PTL_PCD_XXGPP_C_2, {GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //STRAP
    {GPIOV2_PTL_PCD_XXGPP_C_5, {GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //STRAP
    {GPIOV2_PTL_PCD_XXGPP_C_8, {GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //Not Supported
    {GPIOV2_PTL_PCD_XXGPP_C_15,{GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //STRAP
    {GPIOV2_PTL_PCD_XXGPP_E_1, {GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //SPARE
    {GPIOV2_PTL_PCD_XXGPP_E_2, {GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //SPARE
    {GPIOV2_PTL_PCD_XXGPP_E_3, {GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //SPARE
    {GPIOV2_PTL_PCD_XXGPP_E_6, {GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //STRAP
    {GPIOV2_PTL_PCD_XXGPP_E_10,{GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //STRAP
    {GPIOV2_PTL_PCD_XXGPP_F_6, {GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //SPARE
    {GPIOV2_PTL_PCD_XXGPP_E_8, {GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //SPARE
    {GPIOV2_PTL_PCD_XXGPP_H_1, {GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //STRAP
    {GPIOV2_PTL_PCD_XXGPP_F_19,{GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //STRAP
    {GPIOV2_PTL_PCD_XXGPP_H_0, {GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //STRAP
    {GPIOV2_PTL_PCD_XXGPP_H_2, {GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //STRAP
    {GPIOV2_PTL_PCD_XXGPP_A_13,{GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //Not Supported
    {GPIOV2_PTL_PCD_XXGPP_A_15,{GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //Not Supported
    {GPIOV2_PTL_PCD_XXGPP_A_16,{GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //Not Supported
    {GPIOV2_PTL_PCD_XXGPP_B_24,{GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //Not Supported
    {GPIOV2_PTL_PCD_XXGPP_B_25,{GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //Not Supported
    {GPIOV2_PTL_PCD_XXGPP_C_23,{GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //Not Supported
    {GPIOV2_PTL_PCD_XXGPP_D_19,{GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //Not Supported
    {GPIOV2_PTL_PCD_XXGPP_D_21,{GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //Not Supported
    {GPIOV2_PTL_PCD_XXGPP_D_24,{GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //Not Supported
    {GPIOV2_PTL_PCD_XXGPP_D_25,{GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //Not Supported
    {GPIOV2_PTL_PCD_XXGPP_E_19,{GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //Not Supported
    {GPIOV2_PTL_PCD_XXGPP_E_20,{GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //Not Supported
    {GPIOV2_PTL_PCD_XXGPP_F_4, {GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //Not Supported
    {GPIOV2_PTL_PCD_XXGPP_F_5, {GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //Not Supported
    {GPIOV2_PTL_PCD_XXGPP_F_7, {GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //Not Supported
    {GPIOV2_PTL_PCD_XXGPP_F_8, {GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //Not Supported
    {GPIOV2_PTL_PCD_XXGPP_F_20,{GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //Not Supported
    {GPIOV2_PTL_PCD_XXGPP_V_17,{GpioV2PadModeHardwareDefault, GpioV2HostOwnAcpi, GpioV2DirDefault, GpioV2StateDefault, GpioV2IntDis, GpioV2ResetDefault, GpioV2TermDefault}},  //Not Supported
    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTablePreMem|*|{CODE({
    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableEarlyPreMem| * |{CODE({
    {0x0}  // terminator
  })}
