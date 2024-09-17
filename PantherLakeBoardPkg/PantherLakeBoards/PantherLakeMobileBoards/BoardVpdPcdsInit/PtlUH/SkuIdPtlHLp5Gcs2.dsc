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
#  PTL H LP5 Gcs2
#     Board ID: 0x12       (BoardIdPtlHLp5Gcs2)
#   Display ID: 0x00
#       SKU ID: 0x00000012 (SkuIdPtlHLp5Gcs2)
#####################################################################
[PcdsDynamicExVpd.common.SkuIdPtlHLp5Gcs2]

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

  # Default GPIO tier 1 groups are DW0 - GPP_F, DW1 - GPP_D , DW2 - GPP_A
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
  gBoardModuleTokenSpaceGuid.VpdPcdSaMiscUserBd|*|0x01

  # MRC Ddr Frequency Limit
  gBoardModuleTokenSpaceGuid.VpdPcdSaDdrFreqLimit|*|0x0

  # Mrc Spd Data Size
  gBoardModuleTokenSpaceGuid.VpdPcdMrcSpdDataSize|*|512

  # Mrc Spd Address Table
  # GCS use internal table
  gBoardModuleTokenSpaceGuid.VpdPcdMrcSpdAddressTable| * |{CODE(
  { 16,
    { 0x00 ,   // Port 0
      0x00 ,   // Port 1
      0x00 ,   // Port 2
      0x00 ,   // Port 3
      0x00 ,   // Port 4
      0x00 ,   // Port 5
      0x00 ,   // Port 6
      0x00 ,   // Port 7
      0x00 ,   // Port 8
      0x00 ,   // Port 9
      0x00 ,   // Port 10
      0x00 ,   // Port 11
      0x00 ,   // Port 12
      0x00 ,   // Port 13
      0x00 ,   // Port 14
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

  # Touch Pad
  gBoardModuleTokenSpaceGuid.VpdPcdTouchpadIrqGpio|*|{CODE(
   { GPIOV2_PTL_PCD_XXGPP_F_18 } // GPP_F18_THC1_SPI2_INT_N
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
  gBoardModuleTokenSpaceGuid.VpdPcdEcFailSafeActionCpuTemp|*|85

  # EC FailSafe Fan Speed Setting
  gBoardModuleTokenSpaceGuid.VpdPcdEcFailSafeActionFanPwm|*|65

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
  # UINT8  CascadedRetimerFlashIndex   : 2;
  #   Cascaded Retimer Flash Index per USBC connector
  #   0x0 - Unsupported
  # UINT8  FirstUsbCConnectorIndex     : 3;
  #   The first USBC connector index supported by the retimer flash.
  #   0 - USBC Connector1
  # UINT8  SecondUsbCConnectorIndex    : 3;
  #   The second USBC connector index supported by the retimer flash.
  #   1 - USBC Connector2
  gBoardModuleTokenSpaceGuid.VpdPcdUsbCRetimerFlash| * |{CODE(
  {
    {0x1, 0x0, 0x1},
    {0x0}  // terminator
   })}

  # UsbC Retimer Capsule Debug Progress Code Print: FALSE - Disable, TRUE - Enable
  gBoardModuleTokenSpaceGuid.VpdPcdUsbCCapsuleProgressCodeEnable|*|FALSE

  # USB Connector Board Config : {ConnectorConnectable, ConnectorVisibility, ConnectorType, UsbOcPinType, UsbOcPin, Usb2Controller, Usb2PortNum, Usb3Controller, Usb3PortNum}
  gBoardModuleTokenSpaceGuid.VpdPcdUsbConnector| * |{CODE(
  {
  // Connectable,   Visibility, ConnectorType, UsbOcPinType, UsbOcPin, Usb2Controller, Usb2PortNum, Usb3Controller, Usb3PortNum
    {CONNECTABLE,   VISIBLE,    0x09,          VW_OC,        OC_PIN0,  PCH_USB2,       PORT1,       TCSS_USB3,      PORT1}, // TCP0
    {CONNECTABLE,   VISIBLE,    0x09,          VW_OC,        OC_PIN1,  PCH_USB2,       PORT2,       TCSS_USB3,      PORT2}, // TCP1
    {CONNECTABLE,   VISIBLE,    0x00,          0,            0,        PCH_USB2,       PORT3,       0,              0    }, // Cam Conn
    {UNCONNECTABLE, INVISIBLE,  0x00,          0,            0,        0,              0,           0,              0    }, // NA
    {CONNECTABLE,   VISIBLE,    0x00,          PHYSICAL_OC,  OC_PIN0,  PCH_USB2,       PORT5,       PCH_USB3,       PORT1}, // Debug Connector
    {CONNECTABLE,   VISIBLE,    0x00,          PHYSICAL_OC,  OC_PIN0,  PCH_USB2,       PORT6,       PCH_USB3,       PORT2}, // Type-A
    {UNCONNECTABLE, INVISIBLE,  0x00,          0,            0,        0,              0,           0,              0    }, // NA
    {CONNECTABLE,   VISIBLE,    0x00,          0,            0,        PCH_USB2,       PORT8,       0,              0    }, // M.2 WLAN
    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdUsbCConnector| * |{CODE(
  {
  // ModularIoSupported, EdpModularIoSupported, RetimerCount, PcieSupported, DpAltModeSupported, Usb4Supported, Tbt3Supported, PdNum,    AuxDpMode, TbtControllerType, TbtControllerNum, DmaPortNum, PcieType, PcieRootPortNum
    {FIXED_IO,           0,                     1,            1,             1,                  1,             1,             USBC_PD1, 0,         ITBT_DMA,          TBT_CONTROLLER1,  DMA_PORT1,  0,        0}, // TCP0
    {FIXED_IO,           0,                     1,            1,             1,                  1,             1,             USBC_PD2, 0,         ITBT_DMA,          TBT_CONTROLLER1,  DMA_PORT2,  0,        0}, // TCP1
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
  gBoardModuleTokenSpaceGuid.VpdPcdBoardRtd3TableSignature|*|{'P', 't', 'l', '_', 'H', 'G', 'c', 's'}
  gBoardModuleTokenSpaceGuid.VpdPcdBoardSsdRtd3TableSignature|*|{'P', 't', 'l', '_', 'H', 'S', 's', 'd'}
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
   { 0x0 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdSkuType|*|0x2
  gBoardModuleTokenSpaceGuid.VpdPcdDisableMrcRetraining|*|0x0

  # PTL-UH GCS power meter table
  gBoardModuleTokenSpaceGuid.VpdPcdPowerMeter|*|{CODE(
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

  # PCIE CLOCK USGAGE for PTL-H GCS
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

  # PCIE CLOCK USGAGE for PTL-P
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
  // M.2 SSD Gen5
  //
    {GPIOV2_PTL_PCD_XXGPP_B_16,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,      GpioV2ResetHost,  GpioV2TermDefault}}, // GEN5_SSD_PWREN
    {GPIOV2_PTL_PCD_XXGPP_E_3,    {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,      GpioV2ResetHost,  GpioV2TermDefault}},  // M2_GEN5_SSD_RESET_N

  //
  // M.2 WLAN
  //
    {GPIOV2_PTL_PCD_XXGPP_A_12,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirInInv,  GpioV2StateDefault, GpioV2IntLevel|GpioV2IntSci,  GpioV2ResetHostDeep,  GpioV2TermDefault,  GpioV2Unlock,  GpioV2Lock}},  // WIFI_WAKE_N
    {GPIOV2_PTL_PCD_XXGPP_A_16,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,      GpioV2ResetHostDeep,  GpioV2TermDefault}},  // BT_RF_KILL_N
    {GPIOV2_PTL_PCD_XXGPP_A_17,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,      GpioV2ResetHostDeep,  GpioV2TermDefault}},  // WIFI_RF_KILL_N
    {GPIOV2_PTL_PCD_XXGPP_A_11,   {GpioV2PadModeGpio, GpioV2HostOwnAcpi,    GpioV2DirOut,    GpioV2StateHigh,    GpioV2IntDis,      GpioV2ResetHost,  GpioV2TermDefault}}, // WLAN_RST_N

  //
  // Camera Conn
  //
    {GPIOV2_PTL_PCD_XXGPP_C_8,  {GpioV2PadModeGpio,   GpioV2HostOwnAcpi,  GpioV2DirOut,    GpioV2StateLow,    GpioV2IntDis,     GpioV2ResetHost,  GpioV2TermDefault}},  // IR_PWRDN
    {GPIOV2_PTL_PCD_XXGPP_E_1,  {GpioV2PadModeGpio,   GpioV2HostOwnAcpi,  GpioV2DirOut,    GpioV2StateHigh,   GpioV2IntDis,      GpioV2ResetHost,  GpioV2TermDefault}},  // IR_RST_N
    {GPIOV2_PTL_PCD_XXGPP_F_20,  {GpioV2PadModeGpio,  GpioV2HostOwnAcpi,  GpioV2DirOut,    GpioV2StateLow,     GpioV2IntDis,    GpioV2ResetHost,    GpioV2TermDefault}},  // GPP_PRIVACY_LED_CAM1_CVS_HST_WAKE

  //
  // TPM
  //
    { GPIOV2_PTL_PCD_XXGPP_D_15, {GpioV2PadModeGpio, GpioV2HostOwnGpio,    GpioV2DirInInv,    GpioV2StateDefault,  GpioV2IntEdge|GpioV2IntSci,  GpioV2ResetHostDeep,  GpioV2TermNone,  GpioV2TermDefault}},  // SPI_TPM_INT_N

  //
  // Touch pad
  //
    {GPIOV2_PTL_PCD_XXGPP_F_18,  {GpioV2PadModeGpio,  GpioV2HostOwnGpio,  GpioV2DirInInv,   GpioV2StateDefault, GpioV2IntEdge|GpioV2IntApic, GpioV2ResetHost,     GpioV2TermNone,  GpioV2Unlock,  GpioV2Lock}},  // GPP_F18_THC1_SPI2_INT_N

  //
  // TCSS
  //
    {GPIOV2_PTL_PCD_XXGPP_V_17, {GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirOut,  GpioV2StateHigh,   GpioV2IntDis,  GpioV2ResetHost,      GpioV2TermDefault}}, // GPP_V17_TCP_RT_S0IX_ENTRY_EXIT_N

  //
  // Type-C , TBT Re-Timers
  //
    {GPIOV2_PTL_PCD_XXGPP_B_21, {GpioV2PadModeGpio, GpioV2HostOwnAcpi, GpioV2DirOut,  GpioV2StateLow,    GpioV2IntDis,  GpioV2ResetHostDeep,  GpioV2TermDefault}},  // TCP_RETIMER_FORCE_PWR

  //
  // Not Used pins set to High Impedance
  //
    {GPIOV2_PTL_PCD_XXGPP_A_8,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_A_9,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_A_10, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_B_10, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_B_11, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_B_15, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_B_17, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_B_20, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_B_24, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_C_6,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_C_7,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_C_9,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_C_11, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_C_14, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_C_20, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_C_21, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_C_22, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_C_23, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_0,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_1,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_2,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_3,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_4,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_7,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_8,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_9,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_10, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_11, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_13, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_14, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_17, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_18, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_19, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_20, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_21, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_24, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_D_25, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_E_8,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
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
    {GPIOV2_PTL_PCD_XXGPP_F_10, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_11, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_14, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_15, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_16, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_17, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_22, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_F_23, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_H_10, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_H_11, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_H_16, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_H_19, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_H_20, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {GPIOV2_PTL_PCD_XXGPP_S_7,  {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone, GpioV2StateDefault, GpioV2IntDefault, GpioV2ResetDefault, GpioV2TermNone}}, // NC
    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdRetimerPowerStateGpio|*|{CODE(
    { GPIOV2_PTL_PCD_XXGPP_V_17 } // TCP_RT_S0IX_ENTRY_EXIT_N
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdMrcDqsMapCpu2Dram| * |{CODE({
  //
  // PTL-H LPDDR5 GCS DQS byte swizzling between CPU and DRAM
  //
  // Ch 0      1         2         3
  {{ 1, 0 }, { 0, 1 }, { 1, 0 }, { 0, 1 },  // Controller 0
   { 1, 0 }, { 0, 1 }, { 1, 0 }, { 0, 1 }}  // Controller 1
  })}

  # GCS2 Micron MT62F4G32D8DV-023, mLpddr5xOdp16Gb315b8533Spd
  gBoardModuleTokenSpaceGuid.VpdPcdMrcSpdData|*|{CODE(
  {
    // LPDDR5x 315b 16Gb die, ODP 2Rx8, 16GB size per package
    // Micron MT62F4G32D8DV-023
    // 8533, ??-??-??-??
    // 4 Banks, 4 bank groups, 16Gb SDRAM density
    // 17 Row bits, 10 Column bits
    // Non-Monolithic DRAM Device, 8 dies, 2 Channels per package
    1,
     {0x23,                                 ///< 0   384 SPD bytes used, 512 total
      0x10,                                 ///< 1   SPD Revision 1.0
      0x15,                                 ///< 2   DRAM Type: LPDDR5x SDRAM
      0x0E,                                 ///< 3   Module Type: Not Hybrid (DRAM only) / Non-DIMM Solution (on-board DRAM)
      0x86,                                 ///< 4   4 Bank Groups, 4 Banks, 16 Gb SDRAM density
      0x29,                                 ///< 5   17 Rows, 10 Columns
      0xF9,                                 ///< 6   Non-Monolithic DRAM Device, 8 die, 4 Channels per package, Signal Loading Matrix 1
      0x08,                                 ///< 7   SDRAM Optional Features: tMAW = 8192 * tREFI, Unlimited MAC
      0x00,                                 ///< 8   SDRAM Thermal / Refresh options: Reserved
      0x40,                                 ///< 9   Other SDRAM Optional Features: Post package repair supported, one row per bank group, Soft PPR not supported
      0x00,                                 ///< 10  Reserved
      0x00,                                 ///< 11  Module Nominal Voltage: Reserved
      0x09,                                 ///< 12  Module Organization: 2 Ranks, x8 Device Width per Channel
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
  // PTL-H GCS DQ bit swizzling between CPU and DRAM
  //

  //Controller 0
  {{ 14,  8, 15,  9, 10, 12, 11, 13  },  // Byte 0
   {  6,  7,  5,  4,  1,  3,  0,  2  }}, // Byte 1
  {{  0,  1,  3,  4,  2,  5,  6,  7  },  // Byte 2
   { 12, 13, 14, 15, 10, 11,  8,  9  }}, // Byte 3
  {{ 10,  9,  8, 11, 12, 15, 14, 13  },  // Byte 4
   {  1,  3,  0,  2,  6,  7,  5,  4  }}, // Byte 5
  {{  5,  6,  7,  4,  2,  3,  1,  0  },  // Byte 6
   { 15, 14, 10, 11, 12, 13,  9,  8  }}, // Byte 7

  //Controller 1
  {{  8, 10,  9, 11, 13, 12, 14, 15  },  // Byte 0
   {  4,  5,  1,  3,  7,  0,  2,  6  }}, // Byte 1
  {{  1,  0,  3,  2,  6,  7,  4,  5  },  // Byte 2
   { 11, 14, 10, 13,  9, 12,  8, 15  }}, // Byte 3
  {{ 10,  8, 15,  9, 13, 12, 14, 11  },  // Byte 4
   {  3,  4,  7,  0,  2,  5,  6,  1  }}, // Byte 5
  {{  0,  1,  2,  7,  3,  6,  5,  4  },  // Byte 6
   { 11, 15, 10,  8, 13,  9, 12, 14  }}  // Byte 7
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

[PcdsDynamicHii.common.SkuIdPtlHLp5Gcs2.STANDARD]
  gStructPcdTokenSpaceGuid.PcdSetup.PchI2cSensorDevicePort[0]|0x2                                    # Connected device
  gStructPcdTokenSpaceGuid.PcdSetup.PchI2cSensorDevicePort[3]|0x1                                    # Connected device
  gStructPcdTokenSpaceGuid.PcdSetup.PchI2cSensorDevicePort[4]|0x2                                    # Connected device
  gStructPcdTokenSpaceGuid.PcdSetup.PchI2cSensorDevicePort[5]|0x3                                    # Connected device
  gStructPcdTokenSpaceGuid.PcdSetup.PchI2cTouchPadType|0x7                                           # THAT Touch Pad
  gStructPcdTokenSpaceGuid.PcdSetup.TelemetryDeviceEnable|0x1                                        # Enable Telemetry
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioDmicLinkEnable[0]|0x0                               # DMIC #0
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioDmicLinkEnable[1]|0x0                               # DMIC #1
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshI2cEnable[1]|0x0                                        # I2C1
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshSpiEnable[0]|0x1                                        # SPI_0

!if gMipiCamFeaturePkgTokenSpaceGuid.PcdMipiCamFeatureEnable == TRUE
  #
  # Control Logic 1
  #
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0|0x1                         # Control Logic 1     = Enabled
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_Type|0x1                    # Control Logic Type  = Discrete
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_CrdVersion|0x20             # CRD Version         = CRD-D
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_InputClock|0x3              # Input Clock         = 19.2 MHz
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_PchClockSource|0x0          # PCH Clock Source    = IMGCLKOUT_0
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioPinsEnabled|0x3         # Number of GPIO Pins = 3
  # GPIO 0
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioGroupPadNumber[0]|0x01  # Group Pad Number    = 1
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioGroupNumber[0]|0x01     # Group Number        = C_D_E_H
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioComNumber[0]|0x01       # Com number          = COM1
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioFunction[0]|0x00        # Function            = Reset
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioActiveValue[0]|0x1      # Active Value        = 1
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioInitialValue[0]|0x0     # Initial Value       = 0
  # GPIO 1
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioGroupPadNumber[1]|0x08  # Group Pad Number    = 8
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioGroupNumber[1]|0x01     # Group Number        = C_D_E_H
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioComNumber[1]|0x00       # Com number          = COM0
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioFunction[1]|0x0B        # Function            = Power_En
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioActiveValue[1]|0x1      # Active Value        = 1
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioInitialValue[1]|0x0     # Initial Value       = 0
  # GPIO 2
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioGroupPadNumber[2]|0x13  # Group Pad Number    = 19
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioGroupNumber[2]|0x00     # Group Number        = B_F_S_V
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioComNumber[2]|0x01       # Com number          = COM1
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioFunction[2]|0x0D        # Function            = pLED_En
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioActiveValue[2]|0x1      # Active Value        = 1
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioInitialValue[2]|0x0     # Initial Value       = 0
  #
  # Camera 1
  #
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0|0x1                               # Camera1                  = Enable
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_SensorModel|0x2                   # Sensor Model             = OG0VA1B
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_CrdVersion|0x50        # CRD Version              = CRD-G2
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_ControlLogic|0x0       # GPIO control             = Control Logic 1
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_CameraPhysicalLocation|0x61       # Camera position          = Front
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_FlashSupport|0x3       # Flash Support            = Enabled
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_PrivacyLed|0x0         # Privacy LED              = Driver default
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_Degree|0x0             # Rotation                 = 0
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_VoltageRail|0x0        # Voltage Rail             = 3 voltage rail
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_PhyConfiguration|0x0   # PhyConfiguration         = DPHY
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_LaneConfiguration|0x0  # LaneConfiguration        = 0
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_ModuleName|L"CJFKE26"             # Camera module name       = CJFKE26
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_LinkUsed|0x2           # MIPI port                = 2
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_LaneUsed|0x1           # LaneUsed                 = x1
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_Mclk|0x124F800         # MCLK                     = 19200000
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_EepromType|0x00        # EEPROM Type              = ROM_NONE
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_VcmType|0x00           # VCM Type                 = VCM_NONE
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cDevicesEnabled|0x1             # Number of I2C Components = 1
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cChannel|0x2                    # I2C Channel              = I2C2
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cAddress[0]|0x60                # I2C Address              = 60
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cDeviceType[0]|0x00             # Device Type              = Sensor
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_FlashDriverSelection|0x1          # Flash Driver Selection   = External
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_FlashID|0x0            # Flash Driver Selection   = Camera Flash ID 1
  #
  # Flash 1
  #
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash0_Model|0x0              # Flash Model                = External - LM3643
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash0_Mode|0x3               # Flash Mode                 = IR Led
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash0_ModuleName|L"CJFKE26"  # Camera module name         = CJFKE26
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash0_I2cChannel|0x2         # I2C Channel                = I2C2
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash0_I2cAddress|0x67        # I2C Address                = 67
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash0_GpioSupport|0x1        # Flash Trigger Gpio Support = Enabled
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash0_GpioGroupPadNumber|0x1 # Group Pad Number           = 1
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash0_GpioGroupNumber|0x01   # Group Number               = C_D_E_H
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash0_GpioComNumber|0x02     # Com number                 = COM3
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash0_GpioActiveValue|0x1    # Active Value               = 1
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash0_GpioInitialValue|0x0   # Initial Value              = 0
  #
  # Control Logic 2
  #
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1|0x1                         # Control Logic 2     = Enabled
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_Type|0x1                    # Control Logic Type  = Discrete
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_CrdVersion|0x20             # CRD Version         = CRD-D
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_InputClock|0x3              # Input Clock         = 19.2 MHz
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_PchClockSource|0x0          # PCH Clock Source    = IMGCLKOUT_0
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioPinsEnabled|0x3         # Number of GPIO Pins = 3
  # GPIO 0
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioGroupPadNumber[0]|0x0A  # Group Pad Number    = 10
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioGroupNumber[0]|0x01     # Group Number        = C_D_E_H
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioComNumber[0]|0x01       # Com number          = COM1
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioFunction[0]|0x00        # Function            = Reset
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioActiveValue[0]|0x1      # Active Value        = 1
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioInitialValue[0]|0x0     # Initial Value       = 0
  # GPIO 1
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioGroupPadNumber[1]|0x05  # Group Pad Number    = 5
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioGroupNumber[1]|0x01     # Group Number        = C_D_E_H
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioComNumber[1]|0x00       # Com number          = COM0
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioFunction[1]|0x0B        # Function            = Power_En
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioActiveValue[1]|0x1      # Active Value        = 1
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioInitialValue[1]|0x0     # Initial Value       = 0
  # GPIO 2
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioGroupPadNumber[2]|0x14  # Group Pad Number    = 20
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioGroupNumber[2]|0x00     # Group Number        = B_F_S_V
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioComNumber[2]|0x01       # Com number          = COM1
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioFunction[2]|0x0D        # Function            = pLED_En
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioActiveValue[2]|0x1      # Active Value        = 1
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioInitialValue[2]|0x0     # Initial Value       = 0
  #
  # Camera 2
  #
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1|0x1                               # Camera2                  = Enable
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_SensorModel|0x4                   # Sensor Model             = OV08X40
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_CrdVersion|0x20        # CRD Version              = CRD-D
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_ControlLogic|0x1       # GPIO control             = Control Logic 2
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_CameraPhysicalLocation|0x61       # Camera position          = Front
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_FlashSupport|0x3       # Flash Support            = Enabled
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_PrivacyLed|0x0         # Privacy LED              = Driver default
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_Degree|0x0             # Rotation                 = 0
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_VoltageRail|0x0        # Voltage Rail             = 3 voltage rail
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_PhyConfiguration|0x0   # PhyConfiguration         = DPHY
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_LaneConfiguration|0x0  # LaneConfiguration        = 0
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_ModuleName|L"KAFE799"             # Camera module name       = KAFE799
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_LinkUsed|0x0           # MIPI port                = 0
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_LaneUsed|0x4           # LaneUsed                 = x4
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_Mclk|0x124F800         # MCLK                     = 19200000
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_EepromType|0x14        # EEPROM Type              = ROM_EEPROM_GT24P64E
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_VcmType|0x00           # VCM Type                 = VCM_NONE
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cDevicesEnabled|0x2             # Number of I2C Components = 2
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cChannel|0x1                    # I2C Channel              = I2C1
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cAddress[0]|0x10                # I2C Address              = 10
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cDeviceType[0]|0x00             # Device Type              = Sensor
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cAddress[1]|0x50                # I2C Address              = 50
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cDeviceType[1]|0x02             # Device Type              = EEPROM
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_FlashDriverSelection|0x0          # Flash Driver Selection   = Disabled
  #
  # Camera unused
  #
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2|0x0 # Control Logic 3 = Disabled
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3|0x0 # Control Logic 4 = Disabled
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4|0x0 # Control Logic 5 = Disabled
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5|0x0 # Control Logic 6 = Disabled
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2|0x0         # Camera3 = Enable
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3|0x0         # Camera4 = Enable
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4|0x0         # Camera5 = Enable
  gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5|0x0         # Camera6 = Enable
!endif
