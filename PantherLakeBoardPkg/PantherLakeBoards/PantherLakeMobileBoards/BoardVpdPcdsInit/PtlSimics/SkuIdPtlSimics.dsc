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
# PTL simics Board ID: 0x31 (BoardIdPtlSimics)
#                  SKU ID: 0x31 (SkuIdPtlSimics)
#####################################################################
[PcdsDynamicExVpd.common.SkuIdPtlSimics]
  gI2cFeaturePkgTokenSpaceGuid.VpdPcdPssReadSN|*|TRUE
  gI2cFeaturePkgTokenSpaceGuid.VpdPcdPssI2cDeviceAddress|*|0x6E
  gI2cFeaturePkgTokenSpaceGuid.VpdPcdPssI2cBusNumber|*|0x00
  gBoardModuleTokenSpaceGuid.VpdPcdSpdAddressOverride|*|FALSE
  gBoardModuleTokenSpaceGuid.VpdPcdPwmBlinkEnable|*|{CODE(
   { 0x0 }
  )}

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

  gBoardModuleTokenSpaceGuid.VpdPcdGpioGroupToGpeDw0|*|{CODE(
   { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdGpioGroupToGpeDw1|*|{CODE(
   { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdGpioGroupToGpeDw2|*|{CODE(
   { 0x0 }
  )}

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

  # MRC BoardStackUp
  gBoardModuleTokenSpaceGuid.VpdPcdMrcBoardStackUp|*|0x0

  # MRC BoardTopology
  gBoardModuleTokenSpaceGuid.VpdPcdMrcBoardTopology|*|0x0

  gBoardModuleTokenSpaceGuid.VpdPcdSaMiscUserBd|*|0x01
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

  gBoardModuleTokenSpaceGuid.VpdPcdBoardRetimerForcePwrGpio|*|{CODE(
    { 0x0 }
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
    { 0x0 }
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableTouchPanel1|*|{CODE({
    // Touch Panel 1, Not used by default in RVP; Applicable for Converge Mobility RVP SKU only;
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

  # PCIE SLOT 1 - X4 CONNECTOR RTD3
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1RootPort|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1PwrEnableGpioNo|*|{CODE(
   { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1HoldRstGpioNo|*|{CODE(
   { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1PwrEnableGpioPolarity|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1HoldRstGpioPolarity|*|0x0

  # M.2 SSD Slot #1 RTD3
  gBoardModuleTokenSpaceGuid.VpdPcdM2Ssd1PowerEnableGpio|*|{CODE(
   { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdM2Ssd1RstGpio|*|{CODE(
   { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdM2Ssd1PowerEnableGpioPolarity|*|0x1
  gBoardModuleTokenSpaceGuid.VpdPcdM2Ssd1RstGpioPolarity|*|0x0

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

  # Touch Panel 1
  gBoardModuleTokenSpaceGuid.VpdPcdTouchpanel1IrqGpio|*|{CODE(
   { 0x0 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdUsbCEcSupportPdInfoPresent|*|FALSE # USB Type-C PD Information Present : TRUE - Enable

  # USB Type-C Retimer Information Present : FALSE - Disable, TRUE - Enable
  gBoardModuleTokenSpaceGuid.VpdPcdUsbCEcSupportRetimerInfoPresent|*|TRUE
  # UsbC Retimer Flash Configuration Structure : {CascadedRetimerFlashIndex, FirstUsbCConnectorIndex, SecondUsbCConnectorIndex}
  gBoardModuleTokenSpaceGuid.VpdPcdUsbCRetimerFlash| * |{CODE(
  {
    {1, 0, 7},
    {1, 1, 7},
    {1, 2, 7},
    {1, 3, 7},
    {0x0}  // terminator
  })}

  # UsbC Retimer Capsule Debug Progress Code Print: FALSE - Disable, TRUE - Enable
  gBoardModuleTokenSpaceGuid.VpdPcdUsbCCapsuleProgressCodeEnable|*|FALSE

  #
  # *** IMPORTANT ***
  #   All of the USBC connectors MUST be described at the beginning of the table.
  #
  gBoardModuleTokenSpaceGuid.VpdPcdUsbConnector| * |{CODE(
  {
  // Connectable, Visibility, ConnectorType, UsbOcPinType, UsbOcPin, Usb2Controller, Usb2PortNum, Usb3Controller, Usb3PortNum
    {CONNECTABLE, VISIBLE,    0x09,          VW_OC,        OC_PIN0,  PCH_USB2,       PORT1,       TCSS_USB3,      PORT1}, // TCP0
    {CONNECTABLE, VISIBLE,    0x09,          VW_OC,        OC_PIN1,  PCH_USB2,       PORT2,       TCSS_USB3,      PORT2}, // TCP1
    {CONNECTABLE, VISIBLE,    0x09,          VW_OC,        OC_PIN2,  PCH_USB2,       PORT3,       TCSS_USB3,      PORT3}, // TCP2
    {CONNECTABLE, VISIBLE,    0x09,          VW_OC,        OC_PIN3,  PCH_USB2,       PORT4,       TCSS_USB3,      PORT4}, // TCP3
    {CONNECTABLE, VISIBLE,    0x00,          PHYSICAL_OC,  OC_PIN0,  PCH_USB2,       PORT5,       PCH_USB3,       PORT1}, // TAP1
    {CONNECTABLE, VISIBLE,    0x00,          PHYSICAL_OC,  OC_PIN0,  PCH_USB2,       PORT6,       PCH_USB3,       PORT2}, // TAP2
    {CONNECTABLE, VISIBLE,    0x00,          0,            0,        PCH_USB2,       PORT7,       0,              0    }, // FPS
    {CONNECTABLE, INVISIBLE,  0x00,          0,            0,        PCH_USB2,       PORT8,       0,              0    }, // M.2 WLAN
    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdUsbCConnector| * |{CODE(
  {
  // ModularIoSupported, EdpModularIoSupported, RetimerCount, PcieSupported, DpAltModeSupported, Usb4Supported, Tbt3Supported, PdNum,    AuxDpMode, TbtControllerType, TbtControllerNum, DmaPortNum, PcieType, PcieRootPortNum
    {MODULAR_IO,         1,                     1,            1,             1,                  1,             1,             USBC_PD1, 0,         ITBT_DMA,          TBT_CONTROLLER1,  DMA_PORT1,  0,        0}, // TCP0
    {MODULAR_IO,         0,                     1,            1,             1,                  1,             1,             USBC_PD2, 0,         ITBT_DMA,          TBT_CONTROLLER1,  DMA_PORT2,  0,        0}, // TCP1
    {MODULAR_IO,         0,                     1,            1,             1,                  1,             1,             USBC_PD3, 0,         ITBT_DMA,          TBT_CONTROLLER2,  DMA_PORT1,  0,        0}, // TCP2
    {MODULAR_IO,         0,                     1,            1,             1,                  1,             1,             USBC_PD4, 0,         ITBT_DMA,          TBT_CONTROLLER2,  DMA_PORT2,  0,        0}, // TCP3
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

  gBoardModuleTokenSpaceGuid.VpdPcdTcssPmcPdEnable|*|0x1
  gBoardModuleTokenSpaceGuid.VpdPcdUsbCUcmMode|*|0x1 # 0: Unsupported, 1: UCSI, 2: UCMCx
  gBoardModuleTokenSpaceGuid.VpdPcdHdaI2sCodecIrqGpio|*|{CODE(
   { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdHdaI2sCodecI2cBusNumber|*|0x4
  gBoardModuleTokenSpaceGuid.VpdPcdWwanModemBaseBandResetGpio|*|{CODE(
   { 0x0 }
  )}

  # PCIE CLOCK USHAGE for PTL H
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

  # PCIE CLOCK USAGE for PTL P
  gBoardModuleTokenSpaceGuid.VpdPcdPPcieClkUsageMap|*|{CODE(
  {{
    NOT_USED,  // X8 CEM Slot
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

  # Root Port PEWAKE
  gBoardModuleTokenSpaceGuid.VpdPcdRootPortPewakeConfig|*|{CODE(
  {
    0,                       // PEWAKE Pin for RP01
    0,                       // PEWAKE Pin for RP02
    0,                       // PEWAKE Pin for RP03
    0,                       // PEWAKE Pin for RP04
    0,                       // PEWAKE Pin for RP05
    0,                       // PEWAKE Pin for RP06
    0,                       // PEWAKE Pin for RP07
    0,                       // PEWAKE Pin for RP08
    0,                       // PEWAKE Pin for RP09
    0,                       // PEWAKE Pin for RP10
    0,                       // PEWAKE Pin for RP11
    0,                       // PEWAKE Pin for RP12
    0xFFFFFFFF               // terminator
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

  gBoardModuleTokenSpaceGuid.VpdPcdSkuType|*|0x1
  gBoardModuleTokenSpaceGuid.VpdPcdDisableMrcRetraining|*|0x0

  gBoardModuleTokenSpaceGuid.VpdPcdCoinlessSpiNorClearGpio|*|{CODE({
    {GPIOV2_PTL_PCD_XXGPP_F_9, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirIn,  GpioV2StateDefault,  GpioV2IntLevel, GpioV2ResetHost,  GpioV2TermDefault, GpioV2Unlock,  GpioV2Lock}},  // COINLESS_SPI_NOR_CLEAR
    { 0x0 }
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdNvmeRecoveryPrimarySlotPortNumber|*|0x6

[PcdsDynamicHii.common.SkuIdPtlSimics.STANDARD]
  gStructPcdTokenSpaceGuid.PcdSetup.PchI2cSensorDevicePort[0]|0x2                                    # Connected device
  gStructPcdTokenSpaceGuid.PcdSetup.AcpiDebug|0x1                                                    # ACPI Debug
  gStructPcdTokenSpaceGuid.PcdSetup.SerialPortAcpiDebug|0x1                                          # Print to Serial Port
  gStructPcdTokenSpaceGuid.PcdSetup.TcssUcmDevice|0x1                                                # USBC connector manager selection
!if gMipiCamFeaturePkgTokenSpaceGuid.PcdMipiCamFeatureEnable == TRUE
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0|0
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1|0
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2|0
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3|0
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4|0
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5|0
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0|0
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1|0
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2|0
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3|0
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4|0
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5|0
!endif
