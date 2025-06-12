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

!include $(BOARD_PTLUH_BOARDS)/BoardVpdPcdsInit/PtlSimics/SkuIdPtlSimics.dsc
!include $(BOARD_PTLUH_BOARDS)/BoardVpdPcdsInit/PtlUH/SkuIdPtlHfpga.dsc
!include $(BOARD_PTLUH_BOARDS)/BoardVpdPcdsInit/PtlUH/SkuIdPtlUHLp5Rvp1.dsc
!include $(BOARD_PTLUH_BOARDS)/BoardVpdPcdsInit/Xep/PtlUH/SkuIdPtlUHLp5Aep.dsc
!include $(BOARD_PTLUH_BOARDS)/BoardVpdPcdsInit/PtlUH/SkuIdPtlUHHmpRvp.dsc
!include $(BOARD_PTLUH_BOARDS)/BoardVpdPcdsInit/PtlUH/SkuIdPtlUHLp5Rvp3.dsc
!include $(BOARD_PTLUH_BOARDS)/BoardVpdPcdsInit/PtlUH/SkuIdPtlUHDdr5Rvp4.dsc
!include $(BOARD_PTLUH_BOARDS)/BoardVpdPcdsInit/PtlUH/SkuIdPtlUHDdr5EmbCrb.dsc
!include $(BOARD_PTLUH_BOARDS)/BoardVpdPcdsInit/PtlUH/SkuIdPtlUHCammDTbTRvp2.dsc
!include $(BOARD_PTLUH_BOARDS)/BoardVpdPcdsInit/Xep/PtlUH/SkuIdPtlUHCammBep.dsc
!include $(BOARD_PTLUH_BOARDS)/BoardVpdPcdsInit/PtlUH/SkuIdPtlHLp5Gcs1.dsc
!include $(BOARD_PTLUH_BOARDS)/BoardVpdPcdsInit/PtlUH/SkuIdPtlHLp5Gcs2.dsc
!include $(BOARD_PTLUH_BOARDS)/BoardVpdPcdsInit/PtlUH/SkuIdPtlUHLp5MemSktmRvp.dsc
!include $(BOARD_PTLUH_BOARDS)/BoardVpdPcdsInit/PtlUH/SkuIdPtlUHLp5Adk1.dsc
!include $(BOARD_PTLUH_BOARDS)/BoardVpdPcdsInit/PtlUH/SkuIdPtlUHLp5Adk2.dsc

[Packages]
  PantherLakeBoardPkg/BoardPkg.dec

##
#####################################################################
## Initialize value for the VPDPcds with Zero's
#####################################################################
##
[PcdsDynamicExVpd.common.DEFAULT]
  gI2cFeaturePkgTokenSpaceGuid.VpdPcdPssReadSN|*|FALSE
  gI2cFeaturePkgTokenSpaceGuid.VpdPcdPssI2cDeviceAddress|*|0x0
  gI2cFeaturePkgTokenSpaceGuid.VpdPcdPssI2cBusNumber|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdSpdAddressOverride|*|FALSE
  gBoardModuleTokenSpaceGuid.VpdPcdDmicReworkConfig|*|FALSE
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


  gBoardModuleTokenSpaceGuid.VpdPcdSaMiscUserBd|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdSaDdrFreqLimit|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdMrcSpdDataSize|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdMrcSpdAddressTable| * |{CODE(
  { 16,
      { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }
  })}
  gBoardModuleTokenSpaceGuid.VpdPcdMrcDqByteMap| * |{CODE(
  {
    {{ // Channel 0:
     { 0x00, 0x00 }, // CLK0 goes to package 0 - Bytes[3:0], CLK1 goes to package 1 - Bytes[7:4]
     { 0x00, 0x00 }, // Cmd CAA goes to Bytes[3:0], Cmd CAB goes to Byte[7:4]
     { 0x00, 0x00 }, // CTL (CS) goes to all bytes
     { 0x00, 0x00 }, // Unused in ICL MRC
     { 0x00, 0x00 }, // Unused in ICL MRC
     { 0x00, 0x00 }  // Unused in ICL MRC
     },
     { // Channel 1:
     { 0x00, 0x00 }, // CLK0 goes to package 0 - Bytes[3:0], CLK1 goes to package 1 - Bytes[7:4]
     { 0x00, 0x00 }, // Cmd CAA goes to Bytes[3:0], Cmd CAB goes to Byte[7:4]
     { 0x00, 0x00 }, // CTL (CS) goes to all bytes
     { 0x00, 0x00 }, // Unused in ICL MRC
     { 0x00, 0x00 }, // Unused in ICL MRC
     { 0x00, 0x00 }  // Unused in ICL MRC
     }
    }
  })}
  gBoardModuleTokenSpaceGuid.VpdPcdMrcRcompResistor|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdMrcRcompTarget| * |{CODE(
  {  5,
     { 0x0, 0x0, 0x0, 0x0, 0x0}
  })}

  !if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  gBoardModuleTokenSpaceGuid.VpdPcdMrcChannelToCkdQckMapping| * |{CODE(
  { 8,
     { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }
  })}
  gBoardModuleTokenSpaceGuid.VpdPcdMrcPhyClockToCkdDimm| * |{CODE(
  { 8,
     { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }
  })}
  !endif

  gBoardModuleTokenSpaceGuid.VpdPcdMrcDqPinsInterleavedControl|*|FALSE
  gBoardModuleTokenSpaceGuid.VpdPcdMrcDqPinsInterleaved|*|FALSE
  gBoardModuleTokenSpaceGuid.VpdPcdMrcLp5CccConfig|*|0x0
  #
  # CA Vref routing: board-dependent
  # 0 - VREF_CA goes to both CH_A and CH_B (LPDDR3/DDR3L)
  # 1 - VREF_CA to CH_A, VREF_DQ_A to CH_B (should not be used)
  # 2 - VREF_CA to CH_A, VREF_DQ_B to CH_B (DDR4)
  #
  # Set it to 2 for all our DDR4 boards; it is ignored for LPDDR4
  #
  gBoardModuleTokenSpaceGuid.VpdPcdMrcCaVrefConfig|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdMrcCmdMirror|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdVdd2lPowerRailPresent|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdGopConfigSoftwareId|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdDisplayDdiConfigTable| * |{CODE(
  {  16,
     { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
       0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }
  })}
  gBoardModuleTokenSpaceGuid.VpdPcdBoardRetimerForcePwrGpio|*|{CODE(
    { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdRecoveryModeGpio|*|{CODE(
   { 0x0 }
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
  gBoardModuleTokenSpaceGuid.VpdPcdEcUart|*|FALSE
  gBoardModuleTokenSpaceGuid.VpdPcdSmbusAlertEnable|*|FALSE
  gBoardModuleTokenSpaceGuid.VpdPcdEcFailSafeActionCpuTemp|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdEcFailSafeActionFanPwm|*|0x0


  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableTouchPanel0|*|{CODE({
  // Touch Panel 0, Same pins shared between THC and I2C based Panel,
    { 0x0 }
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableTouchPanel1|*|{CODE({
    // Touch Panel 1, Not used by default in RVP; Applicable for Converge Mobility RVP SKU only;
    { 0x0 }
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableLch|*|{CODE({
    { 0x0 }
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdLchIrqGpioPin|*|{CODE(
    { 0x0 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdLchResetGpioPin|*|{CODE(
    { 0x0 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdVrAlertEnable|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdPchThermalHotEnable|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdMemoryThermalSensorGpioCPmsyncEnable|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdMemoryThermalSensorGpioDPmsyncEnable|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdDashGEnable|*|0x0

  # PCIE SLOT #1 RTD3
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
  gBoardModuleTokenSpaceGuid.VpdPcdM2Ssd1PowerEnableGpioPolarity|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdM2Ssd1RstGpioPolarity|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdIpfEnable|*|0x1

  #Mipi Cam
  gBoardModuleTokenSpaceGuid.VpdPcdMipiCamGpioEnable|*|0x0

  gBoardModuleTokenSpaceGuid.VpdPcdUsbCEcSupportPdInfoPresent|*|FALSE

  gBoardModuleTokenSpaceGuid.VpdPcdUsbCEcSupportRetimerInfoPresent|*|FALSE
  gBoardModuleTokenSpaceGuid.VpdPcdUsbCCapsuleProgressCodeEnable|*|FALSE

  # Control the EC debug Info Print level before executing any command for PD Bridge, default is DEBUG_VERBOSE
  gBoardModuleTokenSpaceGuid.VpdPcdPdBridgeDebugInfoPrintLevel|*|0x00400000

  gBoardModuleTokenSpaceGuid.VpdPcdTcssPmcPdEnable|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdUsbCUcmMode|*|0x0 # 0: Unsupported, 1: UCSI, 2: UCMCx
  gBoardModuleTokenSpaceGuid.VpdPcdHdaI2sCodecIrqGpio|*|{CODE(
   { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdHdaI2sCodecI2cBusNumber|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdWwanModemBaseBandResetGpio|*|{CODE(
   { 0x0 }
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
  gBoardModuleTokenSpaceGuid.VpdPcdBoardRtd3TableSignature|*|{0x00}
  gBoardModuleTokenSpaceGuid.VpdPcdBatterySupport|*|0x00
  gBoardModuleTokenSpaceGuid.VpdPcdMipiCamSensor|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdZPoddConfig|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdSmcRuntimeSciPin|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdConvertableDockSupport|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdEcHotKeyF3Support|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdEcHotKeyF4Support|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdEcHotKeyF5Support|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdEcHotKeyF6Support|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdEcHotKeyF7Support|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdEcHotKeyF8Support|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdVirtualButtonVolumeUpSupport|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdVirtualButtonVolumeDownSupport|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdVirtualButtonHomeButtonSupport|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdVirtualButtonRotationLockSupport|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdSlateModeSwitchSupport|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdVirtualGpioButtonSupport|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdPmPowerButtonGpioPin|*|{CODE(
   { 0x0 }
  )}
  gBoardModuleTokenSpaceGuid.VpdPcdAcDcAutoSwitchSupport|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdAcpiEnableAllButtonSupport|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdAcpiHidDriverButtonSupport|*|0x0

  gBoardModuleTokenSpaceGuid.VpdPcdTpm2IntGpio|*|{CODE(
   { 0x0 }
  )}

  # LID status in Bios stage
  gBoardModuleTokenSpaceGuid.VpdPcdGpioLidStatus|*|{CODE(
   { 0x0 }
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdSkuType|*|0x0
  gBoardModuleTokenSpaceGuid.VpdPcdDisableMrcRetraining|*|0x0

  # MRC BoardStackUp
  gBoardModuleTokenSpaceGuid.VpdPcdMrcBoardStackUp|*|0x0

  # MRC BoardTopology
  gBoardModuleTokenSpaceGuid.VpdPcdMrcBoardTopology|*|0x0

  #
  # Table for Structural PCD Of DTbt Board Configuration
  #
  # DTbtBoardSupport, DTbtPcieRpType, DTbtPcieRpSocketNumber, DTbtPcieRpSegNumber, DTbtPcieRpBusNumber, DTbtPcieRpNumber, DTbtPcieRpMps, Reserved, DTbtWakeGpioPin
  gBoardModuleTokenSpaceGuid.VpdPcdDTbtBoardConfig| * |{CODE(
  {{
      {FALSE, 0, 0, 0, 0, 0, 0, 0, 0},
      {FALSE, 0, 0, 0, 0, 0, 0, 0, 0}
    }
  })}

  gPmaxFeaturePkgTokenSpaceGuid.VpdPcdPmaxDevices| * |{CODE(
  {{
      {
         "\\_SB.PC00.HDAS"  // Realtek codec device string
      },
      0xBB8,                     // D0 peak power in mW
      0x00                       // Dx peak power in mW
   },
   {
      {
         "\\_SB.LNK0"       // WF Camera string
      },
      0x32A,                     // D0 peak power in mW without accounting for flash
      0x00                       // Dx peak power in mW
   },
   {
      {
         "\\_SB.LNK1"       // UF Camera string
      },
      0x33E,                     // D0 peak power in mW without accounting for flash
      0x00                       // Dx peak power in mW
   },
   {
      {
         "\\_SB.FLM0"       // Flash device string
      },
      0x2328,                    // D0 peak power in mW
      0x00                       // Dx peak power in mW
   }
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdPowerMeter| * |{CODE(
    {0x0}  // terminator
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdCoinlessModeDetectGpio|*|{CODE(
   { 0x0 }
  )}

  # Wait for others slot number, temporarily set slot number to 0
  gBoardModuleTokenSpaceGuid.VpdPcdNvmeRecoveryPrimarySlotPortNumber|*|0x0

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

  #
  # PSR Feature Capabilities Support
  # OEM must set/clear these bits as per system support.
  #
  gPsrFeatureModuleTokenSpaceGuid.VpdPcdPsrSupportedCapabilities|*|0x1F

  gBoardModuleTokenSpaceGuid.VpdPcdCnvBoardConfig| * |{CODE(
  {
    {
      0,                     // Revision
      0,                     // WlanRootPortNumber
      0,                     // DiscreteUsbBtPortNumber
      0,                     // IntegratedUsbBtPortNumber
      0,                     // WlanRfKillGpio
      0,                     // WlanWakeGpio
      0,                     // WlanRstGpio
      0,                     // BtRfKillGpio
      0                      // BtIrqGpio
    }
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableEnableDiscreteAudioOffloadPreMem| * |{CODE(
  {
    //
    //CNVi GPP_F4 GPP_F5 to be at High-Z
    //
    {GPIOV2_PTL_PCD_XXGPP_F_4, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone,  GpioV2StateLow, GpioV2IntDis, GpioV2ResetHost,  GpioV2TermDefault,  GpioV2Unlock,  GpioV2Lock  }},
    {GPIOV2_PTL_PCD_XXGPP_F_5, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone,  GpioV2StateLow, GpioV2IntDis, GpioV2ResetHost,  GpioV2TermDefault,  GpioV2Unlock,  GpioV2Lock  }},
    {0x0}
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableDisableDiscreteAudioOffloadPreMem| * |{CODE(
  {
    //
    //I2S GPP_S5 GPP_S6 to be at High-Z
    //
    {GPIOV2_PTL_PCD_XXGPP_S_5, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone,  GpioV2StateLow, GpioV2IntDis, GpioV2ResetHost,  GpioV2TermDefault,  GpioV2Unlock,  GpioV2Lock  }},
    {GPIOV2_PTL_PCD_XXGPP_S_6, {GpioV2PadModeGpio, GpioV2HostOwnGpio, GpioV2DirNone,  GpioV2StateLow, GpioV2IntDis, GpioV2ResetHost,  GpioV2TermDefault,  GpioV2Unlock,  GpioV2Lock  }},
    {0x0}
  })}
