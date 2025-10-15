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
#  PTL UH LP5x Adk1
#     Board ID: 0x15       (BoardIdPtlUHLp5Adk1)
#   Display ID: 0x00
#       SKU ID: 0x00000015 (SkuIdPtlUHLp5Adk1)
#####################################################################
[PcdsDynamicExVpd.common.SkuIdPtlUHLp5Adk1]

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

  # Display DDi Config table
  gBoardModuleTokenSpaceGuid.VpdPcdDisplayDdiConfigTable| * |{CODE(
  { 16,
    {DdiPortDisabled,    // DDI Port A Config : DdiPortDisabled = No LFP is Connected, DdiPortEdp = eDP
     DdiPortDisabled,    // DDI Port B Config : DdiPortDisabled = No LFP is Connected, DdiPortEdp = eDP
     DdiHpdDisable,      // DDI Port A HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdDisable,      // DDI Port B HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdDisable,      // DDI Port C HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdDisable,      // DDI Port 1 HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdDisable,      // DDI Port 2 HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdEnable,       // DDI Port 3 HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiHpdDisable,      // DDI Port 4 HPD    : DdiHpdDisable = Disable, DdiHpdEnable = Enable HPD
     DdiDisable,         // DDI Port A DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
     DdiDisable,         // DDI Port B DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
     DdiDisable,         // DDI Port C DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
     DdiDisable,         // DDI Port 1 DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
     DdiDisable,         // DDI Port 2 DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
     DdiDdcEnable,       // DDI Port 3 DDC    : DdiDisable = Disable, DdiDdcEnable = Enable DDC
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

  gBoardModuleTokenSpaceGuid.VpdPcdTouchpanel0IrqGpio|*|{CODE(
    //
    // Touchpanel0 does not support on ADK platform
    //
    { 0x0 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdTouchpanel1IrqGpio|*|{CODE(
    //
    // Touchpanel1 does not support on ADK platform
    //
    { 0x0 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdTouchpadIrqGpio|*|{CODE(
    //
    // Touchpad does not support on ADK platform
    //
    { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdTouchpadIrqGpioPolarity|*|$(PIN_GPIO_ACTIVE_LOW)


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
  gBoardModuleTokenSpaceGuid.VpdPcdEcFailSafeActionCpuTemp|*|100

  # EC FailSafe Fan Speed Setting
  gBoardModuleTokenSpaceGuid.VpdPcdEcFailSafeActionFanPwm|*|100

  gBoardModuleTokenSpaceGuid.VpdPcdEcUart|*|FALSE

  # Touch Panel1 Table
  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableTouchPanel0|*|{CODE({
  // Touch Panel 0, Same pins shared between THC and I2C based Panel,
    //
    // Touchpanel0 does not support on ADK platform
    //
    { 0x0 }
  })}

  # Touch Panel2 Table
  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableTouchPanel1|*|{CODE({
  // Touch Panel 1, Not used by default in RVP; Applicable as Rework;
    //
    // Touchpanel1 does not support on ADK platform
    //
    { 0x0 }
  })}

  # Cnvd Gpio Table
  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableLch|*|{CODE({
    //
    // Cnvd does not support on ADK platform
    //
    { 0x0 }
  })}

  # Lch gpio
  gBoardModuleTokenSpaceGuid.VpdPcdLchIrqGpioPin|*|{CODE(
    //
    // Lch does not support on ADK platform
    //
    { 0x0 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdLchResetGpioPin|*|{CODE(
    //
    // Lch does not support on ADK platform
    //
    { 0x0 }
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
  gBoardModuleTokenSpaceGuid.VpdPcdNvmeRecoveryPrimarySlotPortNumber|*|0x9

  # PCIE SLOT 1 - X1 CONNECTOR RTD3
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1RootPort|*|0  # PCIE SLOT 1 does not support on ADK platform
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1PwrEnableGpioNo|*|{CODE(
    //
    // PCIE SLOT 1 does not support on ADK platform
    //
    { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1HoldRstGpioNo|*|{CODE(
    //
    // PCIE SLOT 1 does not support on ADK platform
    //
    { 0x0 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1PwrEnableGpioPolarity|*|$(PIN_GPIO_ACTIVE_HIGH)
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot1HoldRstGpioPolarity|*|$(PIN_GPIO_ACTIVE_LOW)

  # PCIE SLOT 2 - X8 CONNECTOR RTD3
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot2RootPort|*|0  # PCIE SLOT 2 does not support on ADK platform
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot2PwrEnableGpioNo|*|{CODE(
    //
    // PCIE SLOT 2 does not support on ADK platform
    //
    { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot2HoldRstGpioNo|*|{CODE(
    //
    // PCIE SLOT 2 does not support on ADK platform
    //
    { 0x0 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot2PwrEnableGpioPolarity|*|$(PIN_GPIO_ACTIVE_LOW)
  gBoardModuleTokenSpaceGuid.VpdPcdPcieSlot2HoldRstGpioPolarity|*|$(PIN_GPIO_ACTIVE_LOW)

  # M.2 SSD Slot #1 RTD3
  gBoardModuleTokenSpaceGuid.VpdPcdM2Ssd1PowerEnableGpio|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_B_16 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdM2Ssd1RstGpio|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_E_3 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdM2Ssd1PowerEnableGpioPolarity|*|$(PIN_GPIO_ACTIVE_HIGH)
  gBoardModuleTokenSpaceGuid.VpdPcdM2Ssd1RstGpioPolarity|*|$(PIN_GPIO_ACTIVE_LOW)

  # Mipi Cam
  gBoardModuleTokenSpaceGuid.VpdPcdMipiCamGpioEnable|*|0x0 # Mipi Cam does not support on ADK platform

  # M.2 SSD Slot #2 RTD3
  gBoardModuleTokenSpaceGuid.VpdPcdM2Ssd2PowerEnableGpio|*|{CODE(
    //
    // M.2 SSD Slot 2 does not support on ADK platform
    //
    { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdM2Ssd2RstGpio|*|{CODE(
    //
    // M.2 SSD Slot 2 does not support on ADK platform
    //
    { 0x0 }
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
   {0x0}  // terminator
  })}

  # UsbC Retimer Capsule Debug Progress Code Print: FALSE - Disable, TRUE - Enable
  gBoardModuleTokenSpaceGuid.VpdPcdUsbCCapsuleProgressCodeEnable|*|FALSE

  #
  # *** IMPORTANT ***
  #   For all of the USB ports or native display connector over TCSS lanes on platform.
  #   All of the USBC connectors with PD or native display connector over TCSS lanes MUST be described at
  #   the beginning of the 2 tables with same sequence. Suggest listed sequence: USBC ports (from TCSS lanes,
  #   dTBT or standalone USBC) or native display connector over TCSS lanes, Type-A connectors and others.
  #
  #   Example:
  #     gBoardModuleTokenSpaceGuid.VpdPcdUsbConnector| * |{CODE(
  #     {
  #     // Connectable,   Visibility, ConnectorType, UsbOcPinType, UsbOcPin, Usb2Controller, Usb2PortNum, Usb3Controller, Usb3PortNum
  #       {CONNECTABLE,   VISIBLE,    0x09,          VW_OC,        OC_PIN0,  PCH_USB2,       PORT1,       TCSS_USB3,      PORT1}, // TCP0
  #       {UNCONNECTABLE, VISIBLE,    0xFF,          0,            0,        0,              0,           TCSS_USB3,      PORT2}, // Native HDMI over TCP1
  #       {UNCONNECTABLE, VISIBLE,    0xFF,          VW_OC,        OC_PIN3,  PCH_USB2,       PORT4,       TCSS_USB3,      PORT4}, // TCP3, default is HDMI (Modular IO)
  #       {CONNECTABLE,   VISIBLE,    0x00,          PHYSICAL_OC,  OC_PIN0,  PCH_USB2,       PORT3,       TCSS_USB3,      PORT3}, // TCP2 converted TAP (Type-A)
  #       {CONNECTABLE,   VISIBLE,    0x00,          PHYSICAL_OC,  OC_PIN0,  PCH_USB2,       PORT5,       PCH_USB3,       PORT1}, // TAP1
  #       {CONNECTABLE,   VISIBLE,    0x00,          PHYSICAL_OC,  OC_PIN0,  PCH_USB2,       PORT6,       PCH_USB3,       PORT2}, // TAP2
  #       {CONNECTABLE,   INVISIBLE,  0x00,          0,            0,        PCH_USB2,       PORT7,       0,              0    }, // FPS
  #       {CONNECTABLE,   INVISIBLE,  0x00,          0,            0,        PCH_USB2,       PORT8,       0,              0    }, // M.2 WLAN
  #       {0x0}  // terminator
  #     })}
  #
  #   For all of the USBC connectors with PD or native display connector over TCSS lanes.
  #   The sequence MUST align with the ones described in VpdPcdUsbConnector table.
  #
  #     gBoardModuleTokenSpaceGuid.VpdPcdUsbCConnector| * |{CODE(
  #     {
  #     // ModularIoSupported, EdpModularIoSupported, RetimerCount, PcieSupported, DpAltModeSupported, Usb4Supported, Tbt3Supported, PdNum,    AuxDpMode, TbtControllerType, TbtControllerNum, DmaPortNum, PcieType, PcieRootPortNum
  #       {FIXED_IO,           0,                     1,            1,             1,                  1,             1,             USBC_PD1, 0,         ITBT_DMA,          TBT_CONTROLLER1,  DMA_PORT1,  0,        0}, // TCP0
  #       {FIXED_IO,           0,                     0,            0,             0,                  0,             0,             0,        2,         0,                 0,                0,          0,        0}, // Native HDMI over TCP1
  #       {MODULAR_IO,         0,                     0,            0,             0,                  0,             0,             USBC_PD2, 2,         ITBT_DMA,          TBT_CONTROLLER2,  DMA_PORT2,  0,        0}, // TCP3, default is HDMI (Modular IO)
  #       {0x0}  // terminator
  #     })}
  #
  gBoardModuleTokenSpaceGuid.VpdPcdUsbConnector| * |{CODE(
  {
  // Connectable,   Visibility, ConnectorType, UsbOcPinType, UsbOcPin, Usb2Controller, Usb2PortNum, Usb3Controller, Usb3PortNum
    {CONNECTABLE,   VISIBLE,    0x09,          VW_OC,        OC_PIN0,  PCH_USB2,       PORT1,       TCSS_USB3,      PORT1}, // TCP0
    {CONNECTABLE,   VISIBLE,    0x09,          VW_OC,        OC_PIN1,  PCH_USB2,       PORT2,       TCSS_USB3,      PORT2}, // TCP1
    {UNCONNECTABLE, INVISIBLE,  0xFF,          0,            0,        PCH_USB2,       PORT3,       0,              0    }, // HDMI
    {CONNECTABLE,   VISIBLE,    0x00,          0,            0,        PCH_USB2,       PORT4,       0,              0    }, // Debug Connector
    {CONNECTABLE,   VISIBLE,    0x03,          PHYSICAL_OC,  OC_PIN0,  PCH_USB2,       PORT5,       PCH_USB3,       PORT1}, // TYPE A PORT1
    {CONNECTABLE,   VISIBLE,    0x03,          PHYSICAL_OC,  OC_PIN0,  PCH_USB2,       PORT6,       PCH_USB3,       PORT2}, // TYPE A PORT2
    {UNCONNECTABLE, INVISIBLE,  0,             0,            0,        PCH_USB2,       PORT7,       0,              0    }, // NA
    {CONNECTABLE,   INVISIBLE,  0x02,          0,            0,        PCH_USB2,       PORT8,       0,              0    }, // M.2 WLAN
    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdUsbCConnector| * |{CODE(
  {
  // ModularIoSupported, EdpModularIoSupported, RetimerCount, PcieSupported, DpAltModeSupported, Usb4Supported, Tbt3Supported, PdNum,    AuxDpMode, TbtControllerType, TbtControllerNum, DmaPortNum, PcieType, PcieRootPortNum
    {FIXED_IO,           0,                     1,            1,             1,                  1,             1,             USBC_PD1, 0,         ITBT_DMA,          TBT_CONTROLLER1,  DMA_PORT1,  0,        0}, // TCP0
    {FIXED_IO,           0,                     1,            1,             1,                  1,             1,             USBC_PD1, 0,         ITBT_DMA,          TBT_CONTROLLER1,  DMA_PORT2,  0,        0}, // TCP1
    {MODULAR_IO,         0,                     0,            0,             0,                  0,             0,             0,        2,         ITBT_DMA,          TBT_CONTROLLER2,  DMA_PORT1,  0,        0}, // HDMI
    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdTcssCovUsbA|*|{CODE(
  {
    0, // Port1 Orientation
    0, // Port1 Physical OC pin
    0, // Port2 Orientation
    0, // Port2 Physical OC pin
    0, // Port3 Orientation
    0, // Port3 Physical OC pin
    0, // Port4 Orientation
    0  // Port4 Physical OC pin
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdTcssPmcPdEnable|*|TRUE
  gBoardModuleTokenSpaceGuid.VpdPcdUsbCUcmMode|*|0x1 # 0: Unsupported, 1: UCSI, 2: UCMCx
  gBoardModuleTokenSpaceGuid.VpdPcdHdaI2sCodecIrqGpio|*|{CODE(
   { 0x0 } // CODEC_INT // Present on rework3
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdHdaI2sCodecI2cBusNumber|*|0x3
  # Root Port PEWAKE
   gBoardModuleTokenSpaceGuid.VpdPcdRootPortPewakeConfig|*|{CODE(
  {
    0,                           // PEWAKE Pin for RP01
    0,                           // PEWAKE Pin for RP02
    0,                           // PEWAKE Pin for RP03
    GPIOV2_PTL_PCD_XXGPP_A_12,   // PEWAKE Pin for RP04
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
  gBoardModuleTokenSpaceGuid.VpdPcdBoardRtd3TableSignature|*|{'P', 't', 'l', 'U', 'H', 'A', 'd', 'k'}
  gBoardModuleTokenSpaceGuid.VpdPcdBatterySupport|*|($(BOARD_NO_BATTERY_SUPPORT))
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
   { 0x0 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdSkuType|*|0x2
  gBoardModuleTokenSpaceGuid.VpdPcdDisableMrcRetraining|*|0x0

  # PTL-UH Adk1 power meter table
  gBoardModuleTokenSpaceGuid.VpdPcdPowerMeter|*|{CODE(
  { 4,    // DeviceSize
    '3',  // I2C_Scope
    {
     //BaseAddress, {RailName, ResistorValue}
     {    0x11,     {{"SNS_VBATA",          2},   {"VBATA_VCCCORE_SENSE",   2}, {"VBATA_VDD2H_SENS",      5}, {"V3P3A_MEM_SNS",      5}}},
     {    0x10,     {{"SNS_V1P8A",          5},   {"SNS_V3P3A",             1}, {"VBATA_VCCGT_SENSE",     2}, {"SNS_V3P3X_SSD",      2}}},
     {    0x13,     {{"VBATA_VCCSA_SENSE",  2},   {"VBATA_VCCIO_SENSE",     5}, {"VBATA_LP_ECORE_SENSE",  2}, {"VBATA_VNNAON_SENSE", 2}}},
     {    0x12,     {{"SNS_V5P0A",          2},   {"SNS_V_CHGR_EMI_VIN",   10}, {"V3P3A_KBC_EC_CS",       5}, {"SNS_V3P3A_WLAN",     5}}},
    }
  })}

  # PTL-UH Adk1 Telemetry table
  gBoardModuleTokenSpaceGuid.VpdPcdTelemetry|*|{CODE(
  { 6,    // DeviceSize
    '3',  // I2C_Scope
    {
     //BaseAddress, {RailName, ResistorValue}
     {    0x11,     {{"V3P3A_MEM_SNS",        5}, {"VBATA_VDD2H_SENS",      5}, {"VBATA_VCCCORE_SENSE",   2}, {"SNS_VBATA",          2}}},
     {    0x10,     {{"SNS_V3P3X_SSD",        5}, {"VBATA_VCCGT_SENSE",     2}, {"SNS_V3P3A_DP",          1}, {"SNS_V1P8A",          5}}},
     {    0x13,     {{"VBATA_VNNAON_SENSE",   2}, {"VBATA_LP_ECORE_SENSE",  2}, {"VBATA_VCCIO_SENSE",     5}, {"VBATA_VCCSA_SENSE",  2}}},
     {    0x12,     {{"SNS_VCC_EDP_BKLT_DP", 10}, {"SNS_V3P3A_LIDBRD_DP",  10}, {"SNS_V3P3A_EDP",        10}, {"SNS_V5P0A_DP",       2}}},
     {    0x14,     {{"SNS_V3P3A_WLAN_DP",    1}, {"V3P3A_KBC_EC_CS",       5}, {"SNS_V_CHGR_EMI_VIN_P", 10}, {"SNS_V_BATT_OUT_FET", 5}}},
     {    0x17,     {{"SENSE_3P3A_IR_CAM_DP", 5}, {"SENSE_3P3A_RGB_CAM_DP", 5}, {"NA",                    0}, {"NA",                 0}}},
    }
  })}

  # PCIE CLOCK USAGE for PTL H
  gBoardModuleTokenSpaceGuid.VpdPcdPcieClkUsageMap|*|{CODE(
  {{
    NOT_USED,
    PCIE_PCH + 8,   // Gen5 SSD
    NOT_USED,
    LAN_CLOCK,      // GBE LAN
    PCIE_PCH + 3,   // M.2 WLAN
    NOT_USED,
    NOT_USED,
    NOT_USED,
    NOT_USED
  }}
  )}

  # PCIE CLOCK USAGE for PTL P
  gBoardModuleTokenSpaceGuid.VpdPcdPPcieClkUsageMap|*|{CODE(
  {{
    NOT_USED,
    PCIE_PCH + 8,   // Gen5 SSD
    NOT_USED,
    LAN_CLOCK,      // GBE LAN
    PCIE_PCH + 3,   // M.2 WLAN
    NOT_USED,
    NOT_USED,
    NOT_USED,
    NOT_USED
  }}
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTable|*|{CODE(
  {
  //
  //
  // M.2 SSD Gen5
  //
    {GPIOV2_PTL_PCD_XXGPP_B_16,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,      GpioV2ResetHost,  GpioV2TermDefault}}, // GEN5_SSD_PWREN
    {GPIOV2_PTL_PCD_XXGPP_E_3,    {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,      GpioV2ResetHost,  GpioV2TermDefault}}, // M2_GEN5_SSD_RESET_N

  //
  // M.2 WLAN
  //
    {GPIOV2_PTL_PCD_XXGPP_A_12,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirInInv,  GpioV2StateDefault, GpioV2IntLevel|GpioV2IntSci,  GpioV2ResetHostDeep,  GpioV2TermDefault,  GpioV2Unlock,  GpioV2Lock}}, // WIFI_WAKE_N
    {GPIOV2_PTL_PCD_XXGPP_A_16,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHostDeep,  GpioV2TermDefault}}, // BT_RF_KILL_N
    {GPIOV2_PTL_PCD_XXGPP_A_17,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHostDeep,  GpioV2TermDefault}}, // WIFI_RF_KILL_N
    {GPIOV2_PTL_PCD_XXGPP_A_11,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,                 GpioV2ResetHost,      GpioV2TermDefault}}, // WLAN_RST_N

  //
  // TPM
  //
    { GPIOV2_PTL_PCD_XXGPP_D_15, {GpioV2PadModeGpio, GpioV2HostOwnGpio,    GpioV2DirInInv,    GpioV2StateDefault,  GpioV2IntEdge|GpioV2IntSci,  GpioV2ResetHostDeep,  GpioV2TermNone,  GpioV2TermDefault}},  // SPI_TPM_INT_N

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
    {GPIOV2_PTL_PCD_XXGPP_A_13, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_B_17, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_C_2,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_2,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_14, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_15, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_17, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_H_0,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_V_7,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC

  //
  // Not Used pins on ADK set to High Impedance
  //
    {GPIOV2_PTL_PCD_XXGPP_A_8,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_A_9,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_A_10, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_B_5,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_B_6,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_B_7,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_B_9,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_B_10, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_B_14, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_B_15, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_B_20, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_B_22, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_B_23, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_B_24, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_C_5,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_C_6,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_C_7,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_C_8,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_C_9,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_C_11, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_C_14, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_C_22, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_C_23, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_0,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_1,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_3,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_4,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_7,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_8,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_9,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_10, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_11, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_13, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_14, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_18, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_19, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_20, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_21, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_24, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_25, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_E_1,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_E_5,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_E_6,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_E_8,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_E_10, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_E_11, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_E_12, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_E_13, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_E_14, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_E_15, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_E_16, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_E_17, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_E_18, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_E_19, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_E_20, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_E_22, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_6,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_7,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_8,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_9,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_10, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_11, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_12, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_13, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_16, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_18, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_19, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_20, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_22, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_23, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_H_1,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_H_10, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_H_11, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_H_16, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_H_17, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_H_19, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_H_20, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_H_21, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_H_22, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC

    { 0x0 } // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdCoinlessModeDetectGpio|*|{CODE({
    //
    // Coinless does not support on ADK platform
    //
    { 0x0 }
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdMrcDqsMapCpu2Dram| * |{CODE({
  //
  // PTL-UH LPDDR5x ADK1 DQS byte swizzling between CPU and DRAM
  //
  // Ch 0      1         2         3
  {{ 1, 0 }, { 0, 1 }, { 1, 0 }, { 0, 1 },  // Controller 0
   { 1, 0 }, { 0, 1 }, { 1, 0 }, { 0, 1 }}  // Controller 1
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdMrcSpdData|*|{CODE(
  {
    // LPDDR5x 315b 16Gb die, QDP 2Rx16, 8GB size per package
    // Micron MT62F2G32D4DS-023 WT:C
    // 8533, ??-??-??-??
    // 4 Bank, 4 bank group, 16Gb SDRAM density
    // 16 Row bits, 10 Column bits
    // Non-Monolithic DRAM Device, 4 dies, 2 Channels per package
     1,
     {0x23,                                 ///< 0   384 SPD bytes used, 512 total
      0x10,                                 ///< 1   SPD Revision 1.0
      0x15,                                 ///< 2   DRAM Type: LPDDR5x SDRAM
      0x0E,                                 ///< 3   Module Type: Not Hybrid (DRAM only) / Non-DIMM Solution (on-board DRAM)
      0x86,                                 ///< 4   4 Bank Groups, 4 Banks, 16 Gb SDRAM density
      0x22,                                 ///< 5   16 Rows, 10 Columns
      0xB5,                                 ///< 6   Non-Monolithic DRAM Device, 4 die, 2 Channels per package, Signal Loading Matrix 1
      0x08,                                 ///< 7   SDRAM Optional Features: tMAW = 8192 * tREFI, Unlimited MAC
      0x00,                                 ///< 8   SDRAM Thermal / Refresh options: Reserved
      0x40,                                 ///< 9   Other SDRAM Optional Features: Post package repair supported, one row per bank group, Soft PPR not supported
      0x00,                                 ///< 10  Reserved
      0x00,                                 ///< 11  Module Nominal Voltage: Reserved
      0x0A,                                 ///< 12  Module Organization: 2 Ranks, x16 Device Width per Channel
      0x01,                                 ///< 13  Module Memory Bus width: 1 Channels, 16 bits channel width, no ECC
      0x00,                                 ///< 14  Module Thermal Sensor: none
      0x00,                                 ///< 15  Extended Module Type: Reserved
      0x48,                                 ///< 16  Signal Loading: Data/Strobe/Mask: 2 loads, CAC: 2 loads, CS: 1 load
      0x00,                                 ///< 17  MTB = 0.125ns, FTB = 1 ps
      0x08,                                 ///< 18  tCKAVGmin =  0.937 ns (LPDDR5X-8533 4:1)
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
      0xC1,                                 ///< 125 FTB for tCKAVGmin = -63ps
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
  // PTL-UH Lp5 ADK1 DQ bit swizzling between CPU and DRAM
  //

  //Controller 0
  {{ 14,  8, 15,  9, 10, 12, 11, 13 },  // Byte 0
   {  6,  7,  5,  4,  1,  3,  0,  2 }}, // Byte 1
  {{  0,  1,  3,  4,  2,  5,  6,  7 },  // Byte 2
   { 12, 13, 14, 15, 10, 11,  8,  9 }}, // Byte 3
  {{ 10,  9,  8, 11, 12, 15, 14, 13 },  // Byte 4
   {  1,  3,  0,  2,  6,  7,  5,  4 }}, // Byte 5
  {{  5,  6,  7,  4,  2,  3,  1,  0 },  // Byte 6
   { 15, 14, 10, 11, 12, 13,  9,  8 }}, // Byte 7

  //Controller 1
  {{  8, 10,  9, 11, 13, 12, 14, 15 },  // Byte 0
   {  4,  5,  1,  3,  7,  0,  2,  6 }}, // Byte 1
  {{  1,  0,  3,  2,  6,  7,  4,  5 },  // Byte 2
   { 11, 14, 10, 13,  9, 12,  8, 15 }}, // Byte 3
  {{ 10,  8, 15,  9, 13, 12, 14, 11 },  // Byte 4
   {  3,  4,  7,  0,  2,  5,  6,  1 }}, // Byte 5
  {{  0,  1,  2,  7,  3,  6,  5,  4 },  // Byte 6
   { 11, 15, 10,  8, 13,  9, 12, 14 }}  // Byte 7
   }})}

  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableEarlyPreMem|*|{CODE({
    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTablePreMem|*|{CODE({
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
  gPmaxFeaturePkgTokenSpaceGuid.VpdPcdPmaxDevices|*|{CODE(
   { 0x0 }
  )}

[PcdsDynamicHii.common.SkuIdPtlUHLp5Adk1.STANDARD]
  gStructPcdTokenSpaceGuid.PcdSetup.PowermeterDeviceEnable|0x1              # Enable PowerMeter = Enabled
  gStructPcdTokenSpaceGuid.PcdSetup.TelemetryDeviceEnable|0x0               # Enable Telemetry = Disabled

  #
  # DDT Fan
  #
  gStructPcdTokenSpaceGuid.PcdDptfConfig.EnableSen2Participant|0x1                                   # Sensor Device 2
  gStructPcdTokenSpaceGuid.PcdDptfConfig.EnableSen3Participant|0x1                                   # Sensor Device 3
  gStructPcdTokenSpaceGuid.PcdDptfConfig.EnableSen4Participant|0x1                                   # Sensor Device 4
  gStructPcdTokenSpaceGuid.PcdDptfConfig.EnableSen5Participant|0x1                                   # Sensor Device 5

  #
  # Platform Thermal Configuration
  #
  gStructPcdTokenSpaceGuid.PcdSetup.Ac0FanSpeed|0x41                        # Active Trip Point 0 Fan Speed
  gStructPcdTokenSpaceGuid.PcdSetup.Ac0TripPoint|0x5F                       # Active Trip Point 0
  gStructPcdTokenSpaceGuid.PcdSetup.Ac1FanSpeed|0x2D                        # Active Trip Point 1 Fan Speed
  gStructPcdTokenSpaceGuid.PcdSetup.CpuFanSpeed|0x64                        # CPU Fan Speed
  gStructPcdTokenSpaceGuid.PcdSetup.CpuTemp|0x64                            # CPU Temp
  #
  # HD Audio
  #
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioHdaLinkEnable|0x0          # HDA Link = Disabled
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioDmicLinkEnable[0]|0x0      # DMIC #0 = Disabled
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioDmicLinkEnable[1]|0x0      # DMIC #1 = Disabled
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioSndwLinkEnable[0]|0x0      # SNDW #0 = Disabled
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioSndwLinkEnable[1]|0x0      # SNDW #1 = Disabled
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioSndwLinkEnable[2]|0x1      # SNDW #2 = Enabled
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioSndwLinkEnable[3]|0x0      # SNDW #3 = Disabled
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioSndwMultilaneEnable[1]|0x1 # SNDW #3 Multilane = 2 data lanes
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioFeature[10]|0x1            # ACX/SDCA = Enabled
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioFeature[11]|0x1            # ACX/SDCA speaker aggregation = Enabled
  gStructPcdTokenSpaceGuid.PcdSndwDevTopologyConfigurationVariable.SndwDevTopologyConfigurationNumber|0x6     # SoundWire codecs topology = Configuration GCS/ADK ALC712-VB, ALC1320
  #
  # THC unused
  #
  gStructPcdTokenSpaceGuid.PcdPchSetup.ThcAssignment[0]|0x0                 # THC Port Configuration = None
  gStructPcdTokenSpaceGuid.PcdPchSetup.ThcAssignment[1]|0x0                 # THC Port Configuration = None
  #
  # ISH unused
  #
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshEnable|0x0                     # ISH Controller = Disabled
  #
  # Camera unused
  #
!if gMipiCamFeaturePkgTokenSpaceGuid.PcdMipiCamFeatureEnable == TRUE
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0|0x0       # Control Logic 1 = Disabled
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1|0x0       # Control Logic 2 = Disabled
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2|0x0       # Control Logic 3 = Disabled
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3|0x0       # Control Logic 4 = Disabled
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4|0x0       # Control Logic 5 = Disabled
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5|0x0       # Control Logic 6 = Disabled
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0|0x0               # Camera1 = Disabled
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1|0x0               # Camera2 = Disabled
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2|0x0               # Camera3 = Disabled
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3|0x0               # Camera4 = Disabled
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4|0x0               # Camera5 = Disabled
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5|0x0               # Camera6 = Disabled
!endif


#####################################################################
#  PTL UH LP5x ADK MEM  9600 Speed
#     Board ID: 0x17
#     Display ID: 0x00
#     SKU ID: 0x00000017 (SkuIdPtlUHLp5Adk9600)
#####################################################################
[PcdsDynamicExVpd.common.SkuIdPtlUHLp5Adk9600]

 gBoardModuleTokenSpaceGuid.VpdPcdMrcSpdData|*|{CODE(
  {
    // LPDDR5x 315b 16Gb die, QDP 2Rx16, 8GB size per package
    // Micron MT62F2G32D4DS-023 WT:C
    // 8533, ??-??-??-??
    // 4 Bank, 4 bank group, 16Gb SDRAM density
    // 16 Row bits, 10 Column bits
    // Non-Monolithic DRAM Device, 4 dies, 2 Channels per package
     1,
     {0x23,                                 ///< 0   384 SPD bytes used, 512 total
      0x10,                                 ///< 1   SPD Revision 1.0
      0x15,                                 ///< 2   DRAM Type: LPDDR5x SDRAM
      0x0E,                                 ///< 3   Module Type: Not Hybrid (DRAM only) / Non-DIMM Solution (on-board DRAM)
      0x86,                                 ///< 4   4 Bank Groups, 4 Banks, 16 Gb SDRAM density
      0x29,                                 ///< 5   16 Rows, 10 Columns
      0xF9,                                 ///< 6   Non-Monolithic DRAM Device, 4 die, 2 Channels per package, Signal Loading Matrix 1
      0x08,                                 ///< 7   SDRAM Optional Features: tMAW = 8192 * tREFI, Unlimited MAC
      0x00,                                 ///< 8   SDRAM Thermal / Refresh options: Reserved
      0x40,                                 ///< 9   Other SDRAM Optional Features: Post package repair supported, one row per bank group, Soft PPR not supported
      0x00,                                 ///< 10  Reserved
      0x00,                                 ///< 11  Module Nominal Voltage: Reserved
      0x09,                                 ///< 12  Module Organization: 2 Ranks, x16 Device Width per Channel
      0x01,                                 ///< 13  Module Memory Bus width: 1 Channels, 16 bits channel width, no ECC
      0x00,                                 ///< 14  Module Thermal Sensor: none
      0x00,                                 ///< 15  Extended Module Type: Reserved
      0x00,                                 ///< 16  Signal Loading: Data/Strobe/Mask: 2 loads, CAC: 2 loads, CS: 1 load
      0x00,                                 ///< 17  MTB = 0.125ns, FTB = 1 ps
      0x07,                                 ///< 18  tCKAVGmin =  0.937 ns (LPDDR5X-8533 4:1)
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
      0xD6,                                 ///< 125 FTB for tCKAVGmin = -63ps
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
      0x00,                                 ///< 325 Module ID: Module Serial Number
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

