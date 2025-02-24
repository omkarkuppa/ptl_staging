## @file
#  DSC file for Hii StructurePcd.
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
#
##

[Defines]
  VPD_TOOL_GUID                  = 8C3D856A-9BE6-468E-850A-24F7A8D38E08
!if gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable == TRUE
  VPD_AUTHENTICATED_VARIABLE_STORE = TRUE
!endif

[PcdsDynamicExVpd.common.DEFAULT]
  gEfiMdeModulePkgTokenSpaceGuid.PcdNvStoreDefaultValueBuffer|*

[PcdsDynamicHii.common.DEFAULT.STANDARD]
gStructPcdTokenSpaceGuid.PcdAmtPprEnable|L"AmtPprEnable"|gMemTelAmtPprVariableGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdAmtMacPassThroughConfig|L"AmtMacPassThroughConfig"|gAmtMacPassThroughConfigGuid|0x00||NV, BS
gStructPcdTokenSpaceGuid.PcdAmtPetInitConfig|L"AmtPetInitConfig"|gAmtPetInitConfigGuid|0x00||NV, BS
gStructPcdTokenSpaceGuid.PcdCnvSetup|L"CnvSetup"|gCnvFeatureSetupGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdComAttributes|L"ComAttributes"|gSetupVariableGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdCpuSetup|L"CpuSetup"|gCpuSetupVariableGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdCrashLogVariable|L"CrashLogVariable"|gCrashLogVariableGuid|0x00||NV, BS, RT
!if $(TARGET) == DEBUG
  gStructPcdTokenSpaceGuid.PcdDebugConfigData|L"DebugConfigData"|gDebugConfigVariableGuid|0x00||NV, BS, RT
!endif
gStructPcdTokenSpaceGuid.PcdDptfConfig|L"DptfConfig"|gDptfConfigVariableGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdLpeSetup|L"LpeSetup"|gPsLpeSetupVariableGuid|0x00||NV, BS
gStructPcdTokenSpaceGuid.PcdRpeSetup|L"RpeSetup"|gPsRpeSetupVariableGuid|0x00||NV, BS
gStructPcdTokenSpaceGuid.PcdMeSetup|L"MeSetup"|gMeSetupVariableGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdMipiCamConfig|L"MipiCamConfig"|gMipiCamConfigVariableGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdNhltEndpointsTableConfigurationVariable|L"NhltEndpointsTableConfigurationVariable"|gNhltEndpointsTableConfigurationVariableGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdNvmeSetup|L"NvmeSetup"|gNvmeSetupVariableGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdPchSetup|L"PchSetup"|gPchSetupVariableGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdPlatformEraseVariable|L"PlatformEraseSetup"|gPsPlatformEraseVariableGuid|0x00||NV, BS
gStructPcdTokenSpaceGuid.PcdSaSetup|L"SaSetup"|gSaSetupVariableGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdSetup|L"Setup"|gSetupVariableGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdSiSetup|L"SiSetup"|gSiSetupVariableGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdSndwDevTopologyConfigurationVariable|L"SndwDevTopologyConfigurationVariable"|gSndwDevTopologyConfigurationVariableGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdTCG2_CONFIGURATION|L"TCG2_CONFIGURATION"|gTcg2ConfigFormSetGuid|0x00||NV, BS
gStructPcdTokenSpaceGuid.PcdTCG2_VERSION|L"TCG2_VERSION"|gTcg2ConfigFormSetGuid|0x00||NV, BS
gStructPcdTokenSpaceGuid.PcdVtioCfg|L"VtioCfg"|gVtioSetupGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdXmlCli|L"XmlCli"|gXmlCliSetupGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdCrashLogCtlVariable|L"CrashLogCtlVariable"|gCrashLogCtlVariableGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdBoardInfoSetup|L"BoardInfoSetup"|gBoardInfoVariableGuid|0x00||NV, BS, RT
gStructPcdTokenSpaceGuid.PcdFspVSetup|L"FspVSetup"|gSetupVariableGuid|0x00||NV, BS, RT

gStructPcdTokenSpaceGuid.PcdCrashLogCtlVariable.CpuClear|0x0
# Alphabet sort the following Pcd for easy maintainability
gStructPcdTokenSpaceGuid.PcdAmtMacPassThroughConfig.AmtMacPassThroughState|0x0                     # MAC Pass Through
gStructPcdTokenSpaceGuid.PcdAmtPetInitConfig.FwProgress|0x1                                        # PET Events Progress
!if gCnvFeaturePkgTokenSpaceGuid.PcdCnvIntegratedSupport == 0x1
  gStructPcdTokenSpaceGuid.PcdCnvSetup.CnviMode|0x1                                                  # CNVi Mode
  gStructPcdTokenSpaceGuid.PcdCnvSetup.CnviWifiCore|0x1                                              # Wi-Fi Core
  gStructPcdTokenSpaceGuid.PcdCnvSetup.CnviBtCore|0x1                                                # BT Core
  gStructPcdTokenSpaceGuid.PcdCnvSetup.CnviBtInterface|0x1                                           # BT Interface
!if gCnvFeaturePkgTokenSpaceGuid.PcdBtAudioOffloadSupport == 0x1
  gStructPcdTokenSpaceGuid.PcdCnvSetup.CnviBtAudioOffloadInterface|0x0                               # BT Audio Offload Interface
!endif
!endif
!if gCnvFeaturePkgTokenSpaceGuid.PcdCnvUefiVarVersion == 0x3
  !if gCnvFeaturePkgTokenSpaceGuid.PcdBtAudioOffloadSupport == 0x1
    gStructPcdTokenSpaceGuid.PcdCnvSetup.CnviBtAudioOffload|0x1                                        # BT Audio Offload
  !endif
!endif
!if gCnvFeaturePkgTokenSpaceGuid.PcdCnvDiscreteSupport == 0x1
  gStructPcdTokenSpaceGuid.PcdCnvSetup.SkipVidDidCheck|0x0                                           # Skip VDID Check
  gStructPcdTokenSpaceGuid.PcdCnvSetup.DiscreteBtModule|0x1                                          # Discrete Bluetooth Interface
!endif
!if gCnvFeaturePkgTokenSpaceGuid.PcdPrebootBleSupport == 0x1
  gStructPcdTokenSpaceGuid.PcdCnvSetup.PrebootBleEnable|0x0                                          # Preboot BLE
!endif
gStructPcdTokenSpaceGuid.PcdComAttributes.PortAttributes[0].BaudRate|0x1c200                       # Set COM Baud Rate
gStructPcdTokenSpaceGuid.PcdComAttributes.PortAttributes[0].ComTag.ComNum|0x0                      # Set COM Tag
gStructPcdTokenSpaceGuid.PcdComAttributes.PortAttributes[0].DataBits|0x8                           # Set COM Data Bits
gStructPcdTokenSpaceGuid.PcdComAttributes.PortAttributes[0].FlowControl|0x0                        # Set COM Flow Control
gStructPcdTokenSpaceGuid.PcdComAttributes.PortAttributes[0].IsEnabled|0x1                          # COM0
gStructPcdTokenSpaceGuid.PcdComAttributes.PortAttributes[0].LegacyResolution|0x1                   # Set COM Legacy Resolution
gStructPcdTokenSpaceGuid.PcdComAttributes.PortAttributes[0].Length|0x0                             # Set COM Length
gStructPcdTokenSpaceGuid.PcdComAttributes.PortAttributes[0].Parity|0x1                             # Set COM Parity
gStructPcdTokenSpaceGuid.PcdComAttributes.PortAttributes[0].StopBits|0x1                           # Set COM Stop Bits
gStructPcdTokenSpaceGuid.PcdComAttributes.PortAttributes[0].TerminalType|0x0                       # Set COM Terminal Type
gStructPcdTokenSpaceGuid.PcdComAttributes.TotalSize|0x19                                           # Set COM Data Length
gStructPcdTokenSpaceGuid.PcdCpuSetup.AES|0x1                                                       # AES
gStructPcdTokenSpaceGuid.PcdCpuSetup.AcBrickCapacity|0x1                                           # AC Brick Capacity
gStructPcdTokenSpaceGuid.PcdCpuSetup.AcLoadline[0]|0x0                                             # AC Loadline
gStructPcdTokenSpaceGuid.PcdCpuSetup.AcLoadline[1]|0x0                                             # AC Loadline
gStructPcdTokenSpaceGuid.PcdCpuSetup.AcLoadline[2]|0x0                                             # AC Loadline
gStructPcdTokenSpaceGuid.PcdCpuSetup.AcLoadline[3]|0x0                                             # AC Loadline
gStructPcdTokenSpaceGuid.PcdCpuSetup.AcheckRequest|0x0                                             # Alias Check Request
gStructPcdTokenSpaceGuid.PcdCpuSetup.AcousticNoiseMitigation|0x0                                   # Acoustic Noise Mitigation
gStructPcdTokenSpaceGuid.PcdCpuSetup.PcoreHysteresisWindow|0x0                                     # Pcore Hysteresis Window
gStructPcdTokenSpaceGuid.PcdCpuSetup.EcoreHysteresisWindow|0x0                                     # Ecore Hysteresis Window
gStructPcdTokenSpaceGuid.PcdCpuSetup.ActiveCoreCount|0xff                                          # Active Performance-cores
gStructPcdTokenSpaceGuid.PcdCpuSetup.ActiveLpAtomCoreCount|0xff                                    # Active LP Efficient-cores
gStructPcdTokenSpaceGuid.PcdCpuSetup.ActiveSmallCoreCount|0xff                                     # Active Efficient-cores
gStructPcdTokenSpaceGuid.PcdCpuSetup.ApIdleManner|0x2                                              # AP threads Idle Manner
gStructPcdTokenSpaceGuid.PcdCpuSetup.ApplyConfigTdp|0x1                                            # Enable Configurable Base Power
gStructPcdTokenSpaceGuid.PcdCpuSetup.AtomRatioLimitNumCore[0]|0x0                                  # E-core Turbo Ratio Limit CoreCount0
gStructPcdTokenSpaceGuid.PcdCpuSetup.AtomRatioLimitNumCore[1]|0x0                                  # E-core Turbo Ratio Limit CoreCount1
gStructPcdTokenSpaceGuid.PcdCpuSetup.AtomRatioLimitNumCore[2]|0x0                                  # E-core Turbo Ratio Limit CoreCount2
gStructPcdTokenSpaceGuid.PcdCpuSetup.AtomRatioLimitNumCore[3]|0x0                                  # E-core Turbo Ratio Limit CoreCount3
gStructPcdTokenSpaceGuid.PcdCpuSetup.AtomRatioLimitNumCore[4]|0x0                                  # E-core Turbo Ratio Limit CoreCount4
gStructPcdTokenSpaceGuid.PcdCpuSetup.AtomRatioLimitNumCore[5]|0x0                                  # E-core Turbo Ratio Limit CoreCount5
gStructPcdTokenSpaceGuid.PcdCpuSetup.AtomRatioLimitNumCore[6]|0x0                                  # E-core Turbo Ratio Limit CoreCount6
gStructPcdTokenSpaceGuid.PcdCpuSetup.AtomRatioLimitNumCore[7]|0x0                                  # E-core Turbo Ratio Limit CoreCount7
gStructPcdTokenSpaceGuid.PcdCpuSetup.AtomRatioLimitRatio[0]|0x0                                    # E-core Turbo Ratio Limit Ratio0
gStructPcdTokenSpaceGuid.PcdCpuSetup.AtomRatioLimitRatio[1]|0x0                                    # E-core Turbo Ratio Limit Ratio1
gStructPcdTokenSpaceGuid.PcdCpuSetup.AtomRatioLimitRatio[2]|0x0                                    # E-core Turbo Ratio Limit Ratio2
gStructPcdTokenSpaceGuid.PcdCpuSetup.AtomRatioLimitRatio[3]|0x0                                    # E-core Turbo Ratio Limit Ratio3
gStructPcdTokenSpaceGuid.PcdCpuSetup.AtomRatioLimitRatio[4]|0x0                                    # E-core Turbo Ratio Limit Ratio4
gStructPcdTokenSpaceGuid.PcdCpuSetup.AtomRatioLimitRatio[5]|0x0                                    # E-core Turbo Ratio Limit Ratio5
gStructPcdTokenSpaceGuid.PcdCpuSetup.AtomRatioLimitRatio[6]|0x0                                    # E-core Turbo Ratio Limit Ratio6
gStructPcdTokenSpaceGuid.PcdCpuSetup.AtomRatioLimitRatio[7]|0x0                                    # E-core Turbo Ratio Limit Ratio7
gStructPcdTokenSpaceGuid.PcdCpuSetup.AvxDisable|0x0                                                # AVX
gStructPcdTokenSpaceGuid.PcdCpuSetup.BistOnReset|0x0                                               # BIST
gStructPcdTokenSpaceGuid.PcdCpuSetup.ReduceXecores|0x1                                              # Reduce XeCores
gStructPcdTokenSpaceGuid.PcdCpuSetup.BootFrequency|0x2                                             # Boot performance mode
gStructPcdTokenSpaceGuid.PcdCpuSetup.BootMaxFrequency|0x1                                          # Boot Max Frequency
gStructPcdTokenSpaceGuid.PcdCpuSetup.CStateAutoDemotion|0x1                                        # C-State Auto Demotion
gStructPcdTokenSpaceGuid.PcdCpuSetup.CStatePreWake|0x1                                             # CState Pre-Wake
gStructPcdTokenSpaceGuid.PcdCpuSetup.CStateUnDemotion|0x1                                          # C-State Un-demotion
gStructPcdTokenSpaceGuid.PcdCpuSetup.ConfigTdpBios|0x0                                             # CTDP BIOS control
gStructPcdTokenSpaceGuid.PcdCpuSetup.ConfigTdpLevel|0x0                                            # Configurable Base Power Boot Mode
gStructPcdTokenSpaceGuid.PcdCpuSetup.ConfigTdpLock|0x0                                             # Configurable Base Power Lock
gStructPcdTokenSpaceGuid.PcdCpuSetup.CpuRatio|0xf                                                 # CPU Flex Ratio Settings
gStructPcdTokenSpaceGuid.PcdCpuSetup.CpuRatioOverride|0x0                                          # CPU Flex Ratio Override
gStructPcdTokenSpaceGuid.PcdCpuSetup.CrashLogGprs|0x0                                              # CrashLog GPRs
gStructPcdTokenSpaceGuid.PcdCpuSetup.CustomPowerLimit1Power|0x0                                    # Power Limit 1
gStructPcdTokenSpaceGuid.PcdCpuSetup.CustomPowerLimit1Time|0x0                                     # Power Limit 1 Time Window
gStructPcdTokenSpaceGuid.PcdCpuSetup.CustomPowerLimit2Power|0x0                                    # Power Limit 2
gStructPcdTokenSpaceGuid.PcdCpuSetup.CustomTurboActivationRatio|0x0                                # Config Base Power Turbo Activation Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.DcLoadline[0]|0x0                                             # DC Loadline
gStructPcdTokenSpaceGuid.PcdCpuSetup.DcLoadline[1]|0x0                                             # DC Loadline
gStructPcdTokenSpaceGuid.PcdCpuSetup.DcLoadline[2]|0x0                                             # DC Loadline
gStructPcdTokenSpaceGuid.PcdCpuSetup.DcLoadline[3]|0x0                                             # DC Loadline
gStructPcdTokenSpaceGuid.PcdCpuSetup.DebugInterfaceEnable|0x2                                      # CPU Run Control
gStructPcdTokenSpaceGuid.PcdCpuSetup.DebugInterfaceLockEnable|0x1                                  # CPU Run Control Lock
gStructPcdTokenSpaceGuid.PcdCpuSetup.DfdEnable|0x0                                                 # DFD Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.DisableVrThermalAlert|0x0                                     # Disable VR Thermal Alert
gStructPcdTokenSpaceGuid.PcdCpuSetup.DlvrPhaseSsc|0x0                                              # DLVR PHASE_SSC
gStructPcdTokenSpaceGuid.PcdCpuSetup.DlvrRfiEnable|0x1                                             # Global DLVR RFI Mitigation Control
gStructPcdTokenSpaceGuid.PcdCpuSetup.DlvrRfiFrequency|0x0                                          # DLVR RFI Frequency
gStructPcdTokenSpaceGuid.PcdCpuSetup.DlvrSpreadSpectrumPercentage|0x2                              # DLVR SSC Value
gStructPcdTokenSpaceGuid.PcdCpuSetup.DualTauBoost|0x0                                              # Dual Tau Boost
gStructPcdTokenSpaceGuid.PcdCpuSetup.EcAlgorithmSel|0x1                                            # EC Algorithm Selection
gStructPcdTokenSpaceGuid.PcdCpuSetup.EcGuardBandValue|0x0                                          # EC Guard Band Value
gStructPcdTokenSpaceGuid.PcdCpuSetup.EcPollingPeriod|0x1                                           # EC Polling Period
gStructPcdTokenSpaceGuid.PcdCpuSetup.EcTurboControlMode|0x0                                        # EC Turbo Control Mode
gStructPcdTokenSpaceGuid.PcdCpuSetup.EnableAllThermalFunctions|0x1                                 # Enable All Thermal Functions
gStructPcdTokenSpaceGuid.PcdCpuSetup.EnableCx|0x1                                                  # C states
gStructPcdTokenSpaceGuid.PcdCpuSetup.EnableFastVmode[0]|0x1                                        # Core VR Fast Vmode
gStructPcdTokenSpaceGuid.PcdCpuSetup.EnableFastVmode[1]|0x1                                        # GT VR Fast Vmode
gStructPcdTokenSpaceGuid.PcdCpuSetup.EnableFastVmode[2]|0x1                                        # SA VR Fast Vmode
gStructPcdTokenSpaceGuid.PcdCpuSetup.CepEnable[0]|0x1                                              # Core VR CEP Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.CepEnable[2]|0x1                                              # SA VR CEP Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.CepEnable[3]|0x1                                              # LP Atom Core VR CEP Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.EnableGv|0x1                                                  # Intel(R) SpeedStep(tm)
gStructPcdTokenSpaceGuid.PcdCpuSetup.EnableHwp|0x1                                                 # Intel(R) Speed Shift Technology
gStructPcdTokenSpaceGuid.PcdCpuSetup.EnableHwpAutoEppGrouping|0x1                                  # HwP Autonomous EPP Grouping
gStructPcdTokenSpaceGuid.PcdCpuSetup.EnableDynamicEfficiencyControl|0x0                            # Dynamic Efficiency Control
gStructPcdTokenSpaceGuid.PcdCpuSetup.EnableHwpAutoPerCorePstate|0x1                                # HwP Autonomous Per Core P State
gStructPcdTokenSpaceGuid.PcdCpuSetup.EnableRp|0x1                                                  # Resource Priority Feature
gStructPcdTokenSpaceGuid.PcdCpuSetup.EnableThermalMonitor|0x1                                      # Thermal Monitor
gStructPcdTokenSpaceGuid.PcdCpuSetup.EnableVsysCritical|0x0                                        # Vsys/Psys Critical
gStructPcdTokenSpaceGuid.PcdCpuSetup.EnergyEfficientPState|0x1                                     # Energy Efficient P-state
gStructPcdTokenSpaceGuid.PcdCpuSetup.EnergyEfficientTurbo|0x1                                      # Energy Efficient Turbo
gStructPcdTokenSpaceGuid.PcdCpuSetup.EpocFclkFreq|0x3                                              # FCLK Frequency for Early Power On
gStructPcdTokenSpaceGuid.PcdCpuSetup.FastPkgCRampDisable[0]|0x0                                    # Disable Fast PKG C State Ramp for Core Domain
gStructPcdTokenSpaceGuid.PcdCpuSetup.FastPkgCRampDisable[1]|0x0                                    # Disable Fast PKG C State Ramp for GT Domain
gStructPcdTokenSpaceGuid.PcdCpuSetup.FastPkgCRampDisable[3]|0x0                                    # Disable Fast PKG C State Ramp for Atom Domain
gStructPcdTokenSpaceGuid.PcdCpuSetup.HwpInterruptControl|0x1                                       # Intel(R) Speed Shift Technology Interrupt Control
gStructPcdTokenSpaceGuid.PcdCpuSetup.HwpLock|0x1                                                   # HwP Lock
gStructPcdTokenSpaceGuid.PcdCpuSetup.IccLimit[0]|0x0                                               # VR Fast Vmode ICC Limit
gStructPcdTokenSpaceGuid.PcdCpuSetup.IccLimit[1]|0x0                                               # VR Fast Vmode ICC Limit
gStructPcdTokenSpaceGuid.PcdCpuSetup.IccLimit[2]|0x0                                               # VR Fast Vmode ICC Limit
gStructPcdTokenSpaceGuid.PcdCpuSetup.IccLimit[3]|0x0                                               # VR Fast Vmode ICC Limit
gStructPcdTokenSpaceGuid.PcdCpuSetup.IccMax[0]|0x0                                                 # VR Current Limit
gStructPcdTokenSpaceGuid.PcdCpuSetup.IccMax[1]|0x0                                                 # VR Current Limit
gStructPcdTokenSpaceGuid.PcdCpuSetup.IccMax[2]|0x0                                                 # VR Current Limit
gStructPcdTokenSpaceGuid.PcdCpuSetup.IccMax[3]|0x0                                                 # VR Current Limit
gStructPcdTokenSpaceGuid.PcdCpuSetup.ImonOffset[0]|0x0                                             # IMON Offset
gStructPcdTokenSpaceGuid.PcdCpuSetup.ImonOffset[1]|0x0                                             # IMON Offset
gStructPcdTokenSpaceGuid.PcdCpuSetup.ImonOffset[2]|0x0                                             # IMON Offset
gStructPcdTokenSpaceGuid.PcdCpuSetup.ImonOffset[3]|0x0                                             # IMON Offset
gStructPcdTokenSpaceGuid.PcdCpuSetup.ImonOffsetPrefix[0]|0x0                                       # IMON Prefix
gStructPcdTokenSpaceGuid.PcdCpuSetup.ImonOffsetPrefix[1]|0x0                                       # IMON Prefix
gStructPcdTokenSpaceGuid.PcdCpuSetup.ImonOffsetPrefix[2]|0x0                                       # IMON Prefix
gStructPcdTokenSpaceGuid.PcdCpuSetup.ImonOffsetPrefix[3]|0x0                                       # IMON Prefix
gStructPcdTokenSpaceGuid.PcdCpuSetup.ImonSlope[0]|0x0                                              # IMON Slope
gStructPcdTokenSpaceGuid.PcdCpuSetup.ImonSlope[1]|0x0                                              # IMON Slope
gStructPcdTokenSpaceGuid.PcdCpuSetup.ImonSlope[2]|0x0                                              # IMON Slope
gStructPcdTokenSpaceGuid.PcdCpuSetup.ImonSlope[3]|0x0                                              # IMON Slope
gStructPcdTokenSpaceGuid.PcdCpuSetup.InterruptRedirectMode|0x0                                     # Interrupt Redirection Mode Selection
gStructPcdTokenSpaceGuid.PcdCpuSetup.TdcMode[0]|0x0                                                # TdcMode
gStructPcdTokenSpaceGuid.PcdCpuSetup.TdcMode[1]|0x0                                                # TdcMode
gStructPcdTokenSpaceGuid.PcdCpuSetup.TdcMode[2]|0x0                                                # TdcMode
gStructPcdTokenSpaceGuid.PcdCpuSetup.TdcMode[3]|0x0                                                # TdcMode
gStructPcdTokenSpaceGuid.PcdCpuSetup.VrVoltageLimit[0]|0x0                                         # VR Voltage Limit
gStructPcdTokenSpaceGuid.PcdCpuSetup.VrVoltageLimit[1]|0x0                                         # VR Voltage Limit
gStructPcdTokenSpaceGuid.PcdCpuSetup.VrVoltageLimit[2]|0x0                                         # VR Voltage Limit
gStructPcdTokenSpaceGuid.PcdCpuSetup.VrVoltageLimit[3]|0x0                                         # VR Voltage Limit
gStructPcdTokenSpaceGuid.PcdCpuSetup.IsysCurrentL1Tau|0x0                                          # ISYS Current Limit L1 Tau
gStructPcdTokenSpaceGuid.PcdCpuSetup.IsysCurrentLimitL1|0x0                                        # ISYS Current Limit L1
gStructPcdTokenSpaceGuid.PcdCpuSetup.IsysCurrentLimitL1Enable|0x0                                  # ISYS Current Limit L1 Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.IsysCurrentLimitL2|0x0                                        # ISYS Current Limit L2
gStructPcdTokenSpaceGuid.PcdCpuSetup.IsysCurrentLimitL2Enable|0x0                                  # ISYS Current Limit L2 Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.VsysMax|0x0                                                   # Vsys Max System battery volatge
gStructPcdTokenSpaceGuid.PcdCpuSetup.SkinTempControl|0x0                                           # Skin Temperature Control Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.SkinTargetTemp[0]|0x0                                         # Skin Temperature Target
gStructPcdTokenSpaceGuid.PcdCpuSetup.SkinTargetTemp[1]|0x0                                         # Skin Temperature Target
gStructPcdTokenSpaceGuid.PcdCpuSetup.SkinTargetTemp[2]|0x0                                         # Skin Temperature Target
gStructPcdTokenSpaceGuid.PcdCpuSetup.SkinTempControlEnable[0]|0x0                                  # Skin Control Temperature Enable MMIO
gStructPcdTokenSpaceGuid.PcdCpuSetup.SkinTempControlEnable[1]|0x0                                  # Skin Control Temperature Enable MMIO
gStructPcdTokenSpaceGuid.PcdCpuSetup.SkinTempControlEnable[2]|0x0                                  # Skin Control Temperature Enable MMIO
gStructPcdTokenSpaceGuid.PcdCpuSetup.SkinControlLoopGain[0]|0x0                                    # Skin Temperature Loop Gain
gStructPcdTokenSpaceGuid.PcdCpuSetup.SkinControlLoopGain[1]|0x0                                    # Skin Temperature Loop Gain
gStructPcdTokenSpaceGuid.PcdCpuSetup.SkinControlLoopGain[2]|0x0                                    # Skin Temperature Loop Gain
gStructPcdTokenSpaceGuid.PcdCpuSetup.SkinTempOverrideEnable[0]|0x0                                 # Skin Temperature Override Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.SkinTempOverrideEnable[1]|0x0                                 # Skin Temperature Override Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.SkinTempOverrideEnable[2]|0x0                                 # Skin Temperature Override Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.SkinMinPerformanceLevel[0]|0x0                                # Skin Temperature Minimum Performance Level
gStructPcdTokenSpaceGuid.PcdCpuSetup.SkinMinPerformanceLevel[1]|0x0                                # Skin Temperature Minimum Performance Level
gStructPcdTokenSpaceGuid.PcdCpuSetup.SkinMinPerformanceLevel[2]|0x0                                # Skin Temperature Minimum Performance Level
gStructPcdTokenSpaceGuid.PcdCpuSetup.SkinTempOverride[0]|0x0                                       # Skin Temperature Override
gStructPcdTokenSpaceGuid.PcdCpuSetup.SkinTempOverride[1]|0x0                                       # Skin Temperature Override
gStructPcdTokenSpaceGuid.PcdCpuSetup.SkinTempOverride[2]|0x0                                       # Skin Temperature Override
gStructPcdTokenSpaceGuid.PcdCpuSetup.LongDurationPwrLimitOverride|0x0                              # Power Limit 1 Override
gStructPcdTokenSpaceGuid.PcdCpuSetup.MachineCheck|0x1                                              # MachineCheck
gStructPcdTokenSpaceGuid.PcdCpuSetup.MonitorMwait|0x1                                              # MonitorMWait
gStructPcdTokenSpaceGuid.PcdCpuSetup.NumOfCustomPStates|0x0                                        # Number of P states
gStructPcdTokenSpaceGuid.PcdCpuSetup.OverclockingLock|0x1                                          # Overclocking Lock
gStructPcdTokenSpaceGuid.PcdCpuSetup.PkgCStateDemotion|0x1                                         # Package C-State Demotion
gStructPcdTokenSpaceGuid.PcdCpuSetup.PkgCStateLimit|0xff                                           # Package C State Limit
gStructPcdTokenSpaceGuid.PcdCpuSetup.PkgCStateUnDemotion|0x1                                       # Package C-State Un-demotion
gStructPcdTokenSpaceGuid.PcdCpuSetup.PlatformPowerLimit1Enable|0x0                                 # Platform PL1 Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.PlatformPowerLimit1Power|0x0                                  # Platform PL1 Power
gStructPcdTokenSpaceGuid.PcdCpuSetup.PlatformPowerLimit1Time|0x0                                   # Platform PL1 Time Window
gStructPcdTokenSpaceGuid.PcdCpuSetup.PlatformPowerLimit2Enable|0x0                                 # Platform PL2 Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.PlatformPowerLimit2Power|0x0                                  # Platform PL2 Power
gStructPcdTokenSpaceGuid.PcdCpuSetup.PlatformAtxTelemetryUnit|0x0                                  # ATX Telemetry Unit
gStructPcdTokenSpaceGuid.PcdCpuSetup.PmgCstCfgCtrlLock|0x1                                         # CFG Lock
gStructPcdTokenSpaceGuid.PcdCpuSetup.PowerFloorDisplayDisconnect|0x1                               # Display Disconnect
gStructPcdTokenSpaceGuid.PcdCpuSetup.PowerFloorPcieGenDowngrade|0x1                                # Pcie Gen Speed Downgrade for Floor Power Management
gStructPcdTokenSpaceGuid.PcdCpuSetup.PowerFloorManagement|0x1                                      # Power Floor Management
gStructPcdTokenSpaceGuid.PcdCpuSetup.PowerLimit1|0x0                                               # Power Limit 1
gStructPcdTokenSpaceGuid.PcdCpuSetup.PowerLimit1Time|0x0                                           # Power Limit 1 Time Window
gStructPcdTokenSpaceGuid.PcdCpuSetup.PowerLimit2|0x1                                               # Power Limit 2 Override
gStructPcdTokenSpaceGuid.PcdCpuSetup.PowerLimit2Power|0x0                                          # Power Limit 2
gStructPcdTokenSpaceGuid.PcdCpuSetup.PowerLimit3|0x0                                               # Power Limit 3
gStructPcdTokenSpaceGuid.PcdCpuSetup.PowerLimit3DutyCycle|0x0                                      # Power Limit 3 Duty Cycle
gStructPcdTokenSpaceGuid.PcdCpuSetup.ResponseMode|0x0                                              # Response Mode
gStructPcdTokenSpaceGuid.PcdCpuSetup.PowerLimit3Lock|0x0                                           # Power Limit 3 Lock
gStructPcdTokenSpaceGuid.PcdCpuSetup.PowerLimit3Override|0x0                                       # Power Limit 3 Override
gStructPcdTokenSpaceGuid.PcdCpuSetup.PowerLimit3Time|0x0                                           # Power Limit 3 Time Window
gStructPcdTokenSpaceGuid.PcdCpuSetup.PowerLimit4|0x0                                               # Power Limit 4
gStructPcdTokenSpaceGuid.PcdCpuSetup.PowerLimit4Boost|0x0                                          # Power Limit 4 Boost
gStructPcdTokenSpaceGuid.PcdCpuSetup.PowerLimit4Lock|0x0                                           # Power Limit 4 Lock
gStructPcdTokenSpaceGuid.PcdCpuSetup.PowerLimit4Override|0x0                                       # Power Limit 4 Override
!if $(TARGET) == "DEBUG"
gStructPcdTokenSpaceGuid.PcdCpuSetup.PpinEnableMode|0x0                                            # PPIN Enable Mode
gStructPcdTokenSpaceGuid.PcdCpuSetup.PpinSupport|0x0                                               # PPIN Feature Support
!endif
gStructPcdTokenSpaceGuid.PcdCpuSetup.ForcePrDemotion|0x1                                           # FORCEPR Demotion
gStructPcdTokenSpaceGuid.PcdCpuSetup.VrAlertDemotion|0x1                                           # VrAlert Demotion
gStructPcdTokenSpaceGuid.PcdCpuSetup.ProcessorTraceBspOnly|0x0                                     # Processor Trace Core
gStructPcdTokenSpaceGuid.PcdCpuSetup.ProcessorTraceEnable|0x0                                      # Processor trace
gStructPcdTokenSpaceGuid.PcdCpuSetup.ProcessorTraceMemSize|0xff                                    # Processor trace memory allocation
gStructPcdTokenSpaceGuid.PcdCpuSetup.ProcessorTraceOutputScheme|0x0                                # Processor Trace OutPut Scheme
gStructPcdTokenSpaceGuid.PcdCpuSetup.ProcessorTraceTimingPacket|0x0                                # Processor Trace Timing Packet
gStructPcdTokenSpaceGuid.PcdCpuSetup.Ps1Threshold[0]|0x50                                          # PS1 Current Threshold
gStructPcdTokenSpaceGuid.PcdCpuSetup.Ps1Threshold[1]|0x50                                          # PS1 Current Threshold
gStructPcdTokenSpaceGuid.PcdCpuSetup.Ps1Threshold[2]|0x50                                          # PS1 Current Threshold
gStructPcdTokenSpaceGuid.PcdCpuSetup.Ps1Threshold[3]|0x50                                          # PS1 Current Threshold
gStructPcdTokenSpaceGuid.PcdCpuSetup.Ps2Threshold[0]|0x14                                          # PS2 Current Threshold
gStructPcdTokenSpaceGuid.PcdCpuSetup.Ps2Threshold[1]|0x14                                          # PS2 Current Threshold
gStructPcdTokenSpaceGuid.PcdCpuSetup.Ps2Threshold[2]|0x14                                          # PS2 Current Threshold
gStructPcdTokenSpaceGuid.PcdCpuSetup.Ps2Threshold[3]|0x14                                          # PS2 Current Threshold
gStructPcdTokenSpaceGuid.PcdCpuSetup.Ps3Enable[0]|0x1                                              # PS3 Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.Ps3Enable[1]|0x1                                              # PS3 Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.Ps3Enable[2]|0x1                                              # PS3 Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.Ps3Enable[3]|0x1                                              # PS3 Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.Ps3Threshold[0]|0x04                                          # PS3 Current Threshold
gStructPcdTokenSpaceGuid.PcdCpuSetup.Ps3Threshold[1]|0x04                                          # PS3 Current Threshold
gStructPcdTokenSpaceGuid.PcdCpuSetup.Ps3Threshold[2]|0x04                                          # PS3 Current Threshold
gStructPcdTokenSpaceGuid.PcdCpuSetup.Ps3Threshold[3]|0x04                                          # PS3 Current Threshold
gStructPcdTokenSpaceGuid.PcdCpuSetup.Ps4Enable[0]|0x1                                              # PS4 Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.Ps4Enable[1]|0x1                                              # PS4 Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.Ps4Enable[2]|0x1                                              # PS4 Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.Ps4Enable[3]|0x1                                              # PS4 Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.PsysCriticalThreshold|0x1d4c0                                 # Vsys/Psys Critical Threshold
gStructPcdTokenSpaceGuid.PcdCpuSetup.PsysFullScale|0x30d40                                         # Vsys/Psys Full Scale
gStructPcdTokenSpaceGuid.PcdCpuSetup.PsysOffset|0x0                                                # PSYS Offset
gStructPcdTokenSpaceGuid.PcdCpuSetup.PsysOffsetPrefix|0x0                                          # PSYS Prefix
gStructPcdTokenSpaceGuid.PcdCpuSetup.PsysPmax|0x0                                                  # PSYS PMax Power
gStructPcdTokenSpaceGuid.PcdCpuSetup.PsysSlope|0x0                                                 # PSYS Slope
gStructPcdTokenSpaceGuid.PcdCpuSetup.ThETAIbattEnable|0x0                                          # ThETA Ibatt Feature
gStructPcdTokenSpaceGuid.PcdCpuSetup.RaceToHalt|0x1                                                # Race To Halt (RTH)
gStructPcdTokenSpaceGuid.PcdCpuSetup.RatioLimitNumCore[0]|0x0                                      # P-core Turbo Ratio Limit Core0
gStructPcdTokenSpaceGuid.PcdCpuSetup.RatioLimitNumCore[1]|0x0                                      # P-core Turbo Ratio Limit Core1
gStructPcdTokenSpaceGuid.PcdCpuSetup.RatioLimitNumCore[2]|0x0                                      # P-core Turbo Ratio Limit Core2
gStructPcdTokenSpaceGuid.PcdCpuSetup.RatioLimitNumCore[3]|0x0                                      # P-core Turbo Ratio Limit Core3
gStructPcdTokenSpaceGuid.PcdCpuSetup.RatioLimitNumCore[4]|0x0                                      # P-core Turbo Ratio Limit Core4
gStructPcdTokenSpaceGuid.PcdCpuSetup.RatioLimitNumCore[5]|0x0                                      # P-core Turbo Ratio Limit Core5
gStructPcdTokenSpaceGuid.PcdCpuSetup.RatioLimitNumCore[6]|0x0                                      # P-core Turbo Ratio Limit Core6
gStructPcdTokenSpaceGuid.PcdCpuSetup.RatioLimitNumCore[7]|0x0                                      # P-core Turbo Ratio Limit Core7
gStructPcdTokenSpaceGuid.PcdCpuSetup.RatioLimitRatio[0]|0x0                                        # P-core Turbo Ratio Limit Ratio0 (TRLR)
gStructPcdTokenSpaceGuid.PcdCpuSetup.RatioLimitRatio[1]|0x0                                        # P-core Turbo Ratio Limit Ratio1 (TRLR)
gStructPcdTokenSpaceGuid.PcdCpuSetup.RatioLimitRatio[2]|0x0                                        # P-core Turbo Ratio Limit Ratio2 (TRLR)
gStructPcdTokenSpaceGuid.PcdCpuSetup.RatioLimitRatio[3]|0x0                                        # P-core Turbo Ratio Limit Ratio3 (TRLR)
gStructPcdTokenSpaceGuid.PcdCpuSetup.RatioLimitRatio[4]|0x0                                        # P-core Turbo Ratio Limit Ratio4 (TRLR)
gStructPcdTokenSpaceGuid.PcdCpuSetup.RatioLimitRatio[5]|0x0                                        # P-core Turbo Ratio Limit Ratio5 (TRLR)
gStructPcdTokenSpaceGuid.PcdCpuSetup.RatioLimitRatio[6]|0x0                                        # P-core Turbo Ratio Limit Ratio6 (TRLR)
gStructPcdTokenSpaceGuid.PcdCpuSetup.RatioLimitRatio[7]|0x0                                        # P-core Turbo Ratio Limit Ratio7 (TRLR)
gStructPcdTokenSpaceGuid.PcdCpuSetup.DprSize|0x4                                                   # DPR Memory Size (MB) | DPR Size
gStructPcdTokenSpaceGuid.PcdCpuSetup.ResetAux|0x0                                                  # Reset AUX Content
gStructPcdTokenSpaceGuid.PcdCpuSetup.SlowSlewRate[0]|0x0                                           # Slow Slew Rate for Core Domain
gStructPcdTokenSpaceGuid.PcdCpuSetup.SlowSlewRate[1]|0x0                                           # Slow Slew Rate for GT Domain
gStructPcdTokenSpaceGuid.PcdCpuSetup.SlowSlewRate[3]|0x0                                           # Slow Slew Rate for Atom Domain
gStructPcdTokenSpaceGuid.PcdCpuSetup.SmmProcTraceEnable|0x0                                        # SMM Processor Trace
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[0]|0x0                                             # Max P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[1]|0x0                                             # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[2]|0x0                                             # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[3]|0x0                                             # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[4]|0x0                                             # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[5]|0x0                                             # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[6]|0x0                                             # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[7]|0x0                                             # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[8]|0x0                                             # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[9]|0x0                                             # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[10]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[11]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[12]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[13]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[14]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[15]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[16]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[17]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[18]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[19]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[20]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[21]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[22]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[23]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[24]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[25]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[26]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[27]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[28]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[29]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[30]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[31]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[32]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[33]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[34]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[35]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[36]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[37]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[38]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatio[39]|0x0                                            # P-State Ratio
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatioMax16[0]|0x0                                        # Max P-State Ratio (Max 16 table)
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatioMax16[1]|0x0                                        # P-State Ratio (Max 16 table)
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatioMax16[2]|0x0                                        # P-State Ratio (Max 16 table)
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatioMax16[3]|0x0                                        # P-State Ratio (Max 16 table)
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatioMax16[4]|0x0                                        # P-State Ratio (Max 16 table)
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatioMax16[5]|0x0                                        # P-State Ratio (Max 16 table)
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatioMax16[6]|0x0                                        # P-State Ratio (Max 16 table)
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatioMax16[7]|0x0                                        # P-State Ratio (Max 16 table)
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatioMax16[8]|0x0                                        # P-State Ratio (Max 16 table)
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatioMax16[9]|0x0                                        # P-State Ratio (Max 16 table)
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatioMax16[10]|0x0                                       # P-State Ratio (Max 16 table)
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatioMax16[11]|0x0                                       # P-State Ratio (Max 16 table)
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatioMax16[12]|0x0                                       # P-State Ratio (Max 16 table)
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatioMax16[13]|0x0                                       # P-State Ratio (Max 16 table)
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatioMax16[14]|0x0                                       # P-State Ratio (Max 16 table)
gStructPcdTokenSpaceGuid.PcdCpuSetup.StateRatioMax16[15]|0x0                                       # Min P-State Ratio (Max 16 table)
gStructPcdTokenSpaceGuid.PcdCpuSetup.TCCActivationOffset|0x0                                       # Tcc Activation Offset
gStructPcdTokenSpaceGuid.PcdCpuSetup.TStatesEnable|0x0                                             # ACPI T-States
gStructPcdTokenSpaceGuid.PcdCpuSetup.TccOffsetLock|0x1                                             # Tcc Offset Lock Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.TdcCurrentLimit[0]|0x0                                        # TDC Current Limit
gStructPcdTokenSpaceGuid.PcdCpuSetup.TdcCurrentLimit[1]|0x0                                        # TDC Current Limit
gStructPcdTokenSpaceGuid.PcdCpuSetup.TdcCurrentLimit[2]|0x0                                        # TDC Current Limit
gStructPcdTokenSpaceGuid.PcdCpuSetup.TdcCurrentLimit[3]|0x0                                        # TDC Current Limit
gStructPcdTokenSpaceGuid.PcdCpuSetup.TdcEnable[0]|0x1                                              # TDC Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.TdcEnable[1]|0x1                                              # TDC Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.TdcEnable[2]|0x1                                              # TDC Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.TdcEnable[3]|0x1                                              # TDC Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.TdcLock[0]|0x0                                                # TDC Lock
gStructPcdTokenSpaceGuid.PcdCpuSetup.TdcLock[1]|0x0                                                # TDC Lock
gStructPcdTokenSpaceGuid.PcdCpuSetup.TdcLock[2]|0x0                                                # TDC Lock
gStructPcdTokenSpaceGuid.PcdCpuSetup.TdcLock[3]|0x0                                                # TDC Lock
gStructPcdTokenSpaceGuid.PcdCpuSetup.TdcTimeWindow[0]|0x0                                          # TDC Time Window
gStructPcdTokenSpaceGuid.PcdCpuSetup.TdcTimeWindow[1]|0x0                                          # TDC Time Window
gStructPcdTokenSpaceGuid.PcdCpuSetup.TdcTimeWindow[2]|0x0                                          # TDC Time Window
gStructPcdTokenSpaceGuid.PcdCpuSetup.TdcTimeWindow[3]|0x0                                          # TDC Time Window
gStructPcdTokenSpaceGuid.PcdCpuSetup.TdxEnable|0x0                                                 # Trust Domain Extension (TDX)
gStructPcdTokenSpaceGuid.PcdCpuSetup.ThreeStrikeCounter|0x1                                        # Three Strike Counter
gStructPcdTokenSpaceGuid.PcdCpuSetup.TimedMwait|0x0                                                # Timed MWAIT
gStructPcdTokenSpaceGuid.PcdCpuSetup.TmeBypassCapability|0x0                                       # Total Memory Encryption (TME) Bypass
gStructPcdTokenSpaceGuid.PcdCpuSetup.TmeEnable|0x0                                                 # Total Memory Encryption
gStructPcdTokenSpaceGuid.PcdCpuSetup.TseEnable|0x0                                                 # Intel Total Storage Encryption
gStructPcdTokenSpaceGuid.PcdCpuSetup.TurboMode|0x1                                                 # Turbo Mode
gStructPcdTokenSpaceGuid.PcdCpuSetup.TurboPowerLimitLock|0x0                                       # Package Power Limit MSR Lock
gStructPcdTokenSpaceGuid.PcdCpuSetup.Txt|0x0                                                       # Intel Trusted Execution Technology
gStructPcdTokenSpaceGuid.PcdCpuSetup.VT|0x1                                                        # Intel (VMX) Virtualization Technology
gStructPcdTokenSpaceGuid.PcdCpuSetup.VrConfigEnable[0]|0x1                                         # VR Config Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.VrConfigEnable[1]|0x1                                         # VR Config Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.VrConfigEnable[2]|0x1                                         # VR Config Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.VrConfigEnable[3]|0x1                                         # VR Config Enable
gStructPcdTokenSpaceGuid.PcdCpuSetup.VsysAssertionDeglitchExponent|0x0                             # Assertion Deglitch Exponent
gStructPcdTokenSpaceGuid.PcdCpuSetup.VsysAssertionDeglitchMantissa|0x1                             # Assertion Deglitch Mantissa
gStructPcdTokenSpaceGuid.PcdCpuSetup.VsysCriticalThreshold|0x1770                                  # Vsys/Psys Critical Threshold
gStructPcdTokenSpaceGuid.PcdCpuSetup.VsysDeassertionDeglitchExponent|0x2                           # De assertion Deglitch Exponent
gStructPcdTokenSpaceGuid.PcdCpuSetup.VsysDeassertionDeglitchMantissa|0xd                           # De assertion Deglitch Mantissa
gStructPcdTokenSpaceGuid.PcdCpuSetup.VsysFullScale|0x5dc0                                          # Vsys/Psys Full Scale
gStructPcdTokenSpaceGuid.PcdCpuSetup.X2ApicEnable|0x1                                              # X2APIC Enable
gStructPcdTokenSpaceGuid.PcdFspVSetup.FspMVTestCaseId|0xFFFF                                       # FspM Validation Test Case ID
gStructPcdTokenSpaceGuid.PcdFspVSetup.FspSVTestCaseId|0xFFFF                                       # FspS Validation Test Case ID
gStructPcdTokenSpaceGuid.PcdCrashLogVariable.CpuClear|0x0                                          # CrashLog Clear Enable
gStructPcdTokenSpaceGuid.PcdCrashLogVariable.CpuRearm|0x1                                          # CrashLog Re-Arm Enable
gStructPcdTokenSpaceGuid.PcdCrashLogVariable.EnableCrashLog|0x1                                    # CrashLog Feature
gStructPcdTokenSpaceGuid.PcdCrashLogVariable.SiliconClear|0x0                                      # CrashLog Clear Enable
gStructPcdTokenSpaceGuid.PcdCrashLogVariable.SiliconGblRst|0x0                                     # CrashLog Global Reset Trigger mask status
gStructPcdTokenSpaceGuid.PcdCrashLogVariable.SiliconGblRstMask0|0xffffffff                         # Global Reset Trigger mask 0
gStructPcdTokenSpaceGuid.PcdCrashLogVariable.SiliconGblRstMask1|0xffffffff                         # Global Reset Trigger mask 1
gStructPcdTokenSpaceGuid.PcdCrashLogVariable.SiliconOnAllReset|0x0                                 # CrashLog On All Reset
gStructPcdTokenSpaceGuid.PcdCrashLogVariable.SiliconRearm|0x1                                      # CrashLog Re-Arm Enable
!if $(TARGET) == "DEBUG"
gStructPcdTokenSpaceGuid.PcdDebugConfigData.RamDebugInterface|0x0                                  # RAM
gStructPcdTokenSpaceGuid.PcdDebugConfigData.SerialDebug|0x3                                        # Serial Debug Messages
gStructPcdTokenSpaceGuid.PcdDebugConfigData.SerialDebugBaudRate|0x7                                # Serial Debug Message Baud Rate
gStructPcdTokenSpaceGuid.PcdDebugConfigData.SerialDebugMrcLevel|0x3                                # MRC Serial Debug Messages
gStructPcdTokenSpaceGuid.PcdDebugConfigData.SerialIoDebugInterface|0x0                             # Serial IO UART
gStructPcdTokenSpaceGuid.PcdDebugConfigData.SerialIoUartDebugBaudRate|0x1c200                      # Baud Rate
gStructPcdTokenSpaceGuid.PcdDebugConfigData.SerialIoUartDebugControllerNumber|0x0                  # Controller Number
gStructPcdTokenSpaceGuid.PcdDebugConfigData.SerialIoUartDebugDataBits|0x8                          # Word Length
gStructPcdTokenSpaceGuid.PcdDebugConfigData.SerialIoUartDebugFlowControl|0x0                       # Flow Control
gStructPcdTokenSpaceGuid.PcdDebugConfigData.SerialIoUartDebugParity|0x1                            # Parity Bits
gStructPcdTokenSpaceGuid.PcdDebugConfigData.SerialIoUartDebugStopBits|0x1                          # Stop Bits
gStructPcdTokenSpaceGuid.PcdDebugConfigData.TraceHubDebugInterface|0x1                             # Trace Hub
gStructPcdTokenSpaceGuid.PcdDebugConfigData.UartDebugInterface|0x1                                 # Legacy UART
gStructPcdTokenSpaceGuid.PcdDebugConfigData.Usb3DebugInterface|0x0                                 # USB3
!endif
gStructPcdTokenSpaceGuid.PcdDptfConfig.ArtgObject|0x0                                              # ARTG Object
gStructPcdTokenSpaceGuid.PcdDptfConfig.EnableBatteryParticipant|0x0                                # Battery Participant
gStructPcdTokenSpaceGuid.PcdDptfConfig.EnableChargerParticipant|0x1                                # Charger participant
gStructPcdTokenSpaceGuid.PcdDptfConfig.EnableDCFG|0x0                                              # Intel(R) Innovation Platform Framework Configuration
gStructPcdTokenSpaceGuid.PcdDptfConfig.EnableFan1Device|0x1                                        # FAN1 Device
gStructPcdTokenSpaceGuid.PcdDptfConfig.EnableFan2Device|0x0                                        # FAN2 Device
gStructPcdTokenSpaceGuid.PcdDptfConfig.EnableFan3Device|0x0                                        # FAN3 Device
gStructPcdTokenSpaceGuid.PcdDptfConfig.EnablePowerParticipant|0x0                                  # Power participant
gStructPcdTokenSpaceGuid.PcdDptfConfig.EnableSen1Participant|0x0                                   # Sensor Device 1
gStructPcdTokenSpaceGuid.PcdDptfConfig.EnableSen2Participant|0x0                                   # Sensor Device 2
gStructPcdTokenSpaceGuid.PcdDptfConfig.EnableSen3Participant|0x0                                   # Sensor Device 3
gStructPcdTokenSpaceGuid.PcdDptfConfig.EnableSen4Participant|0x0                                   # Sensor Device 4
gStructPcdTokenSpaceGuid.PcdDptfConfig.EnableSen5Participant|0x0                                   # Sensor Device 5
gStructPcdTokenSpaceGuid.PcdDptfConfig.OemDesignVariable0|0x1                                      # Design Variable 0
gStructPcdTokenSpaceGuid.PcdDptfConfig.OemDesignVariable1|0x0                                      # Design Variable 1
gStructPcdTokenSpaceGuid.PcdDptfConfig.OemDesignVariable2|0x0                                      # Design Variable 2
gStructPcdTokenSpaceGuid.PcdDptfConfig.OemDesignVariable3|0x0                                      # Design Variable 3
gStructPcdTokenSpaceGuid.PcdDptfConfig.OemDesignVariable4|0x0                                      # Design Variable 4
gStructPcdTokenSpaceGuid.PcdDptfConfig.OemDesignVariable5|0x0                                      # Design Variable 5
gStructPcdTokenSpaceGuid.PcdDptfConfig.PmaxObject|0x0                                              # PMAX Object
gStructPcdTokenSpaceGuid.PcdDptfConfig.PowerParticipantPollingRate|0x0                             # Intel(R) Innovation Platform Framework Battery Sampling Period
gStructPcdTokenSpaceGuid.PcdDptfConfig.PpccObject|0x0                                              # PPCC Object
gStructPcdTokenSpaceGuid.PcdDptfConfig.PpccStepSize|0x1f4                                          # PPCC Step Size
gStructPcdTokenSpaceGuid.PcdLpeSetup.LpeBiosReloadGoldenConfig|0x0                                 # BIOS Reload Golden Config
gStructPcdTokenSpaceGuid.PcdLpeSetup.LpeClearNvm|0x0                                               # Clear NVM Variable
gStructPcdTokenSpaceGuid.PcdLpeSetup.LpeClearTPM|0x0                                               # TPM CLEAR
gStructPcdTokenSpaceGuid.PcdLpeSetup.LpeCsmeUnconfigure|0x0                                        # Unconfigure CSME
gStructPcdTokenSpaceGuid.PcdLpeSetup.LpeEnable|0x0                                                 # Perform Local Platform Erase Operations
gStructPcdTokenSpaceGuid.PcdRpeSetup.RpeEnable|0x1                                                 # Perform Remote Platform Erase Operations
gStructPcdTokenSpaceGuid.PcdLpeSetup.LpeEraseAll|0x1                                               # Erase All
gStructPcdTokenSpaceGuid.PcdLpeSetup.LpeEraseSSD|0x0                                               # Storage Device Erase
gStructPcdTokenSpaceGuid.PcdLpeSetup.LpeOemCustomAction|0x0                                        # OEM Custom Actions
gStructPcdTokenSpaceGuid.PcdLpeSetup.LpeStorageEraseVerify|0x0                                     # Storage Erase Verification
gStructPcdTokenSpaceGuid.PcdMeSetup.OcrBootHttps|0x1
gStructPcdTokenSpaceGuid.PcdMeSetup.OcrBootPba|0x1
gStructPcdTokenSpaceGuid.PcdMeSetup.OcrBootWinRe|0x1
gStructPcdTokenSpaceGuid.PcdMeSetup.OcrAmtDisSecBoot|0x1
gStructPcdTokenSpaceGuid.PcdMeSetup.Amt|0x1                                                        # AMT BIOS Features
gStructPcdTokenSpaceGuid.PcdMeSetup.AmtCiraRequest|0x0                                             # Activate Remote Assistance Process
gStructPcdTokenSpaceGuid.PcdMeSetup.AsfSensorsTable|0x0                                            # ASF Sensors Table
gStructPcdTokenSpaceGuid.PcdMeSetup.AutoArbSvnCommit|0x0                                           # Automatic HW-Enforced Anti-Rollback SVN
gStructPcdTokenSpaceGuid.PcdMeSetup.BiosPause|0x0                                                  # BIOS Pause Before Booting Capability State
gStructPcdTokenSpaceGuid.PcdMeSetup.BiosReflash|0x1                                                # BIOS Reflash Capability State
gStructPcdTokenSpaceGuid.PcdMeSetup.BiosSetup|0x1                                                  # BIOS Boot to Setup Capability State
gStructPcdTokenSpaceGuid.PcdMeSetup.CoreBiosDoneEnabled|0x1                                        # Core Bios Done Message
gStructPcdTokenSpaceGuid.PcdMeSetup.CseDataResilience|0x1                                          # CSE Data Resilience Support
gStructPcdTokenSpaceGuid.PcdMeSetup.PseEomFlowEnable|0x0                                           # PSE EOM Flow Support
gStructPcdTokenSpaceGuid.PcdMeSetup.DidInitStat|0x0                                                # Force ME DID Init Status
gStructPcdTokenSpaceGuid.PcdMeSetup.DisableCpuReplacedPolling|0x0                                  # CPU Replaced Polling Disable
gStructPcdTokenSpaceGuid.PcdMeSetup.DisableD0I3SettingForHeci|0x0                                  # D0I3 Setting for HECI Disable
gStructPcdTokenSpaceGuid.PcdMeSetup.DisableMessageCheck|0x0                                        # HECI Message check Disable
gStructPcdTokenSpaceGuid.PcdMeSetup.EndOfPostMessage|0x2                                           # End Of Post Message
gStructPcdTokenSpaceGuid.PcdMeSetup.SseCommunication|0x0                                           # SSE/SSE++ Communication
gStructPcdTokenSpaceGuid.PcdMeSetup.ExtendMeMeasuredBoot|0x0                                       # Extend CSME Measurement to TPM-PCR
gStructPcdTokenSpaceGuid.PcdMeSetup.HeciCommunication|0x0                                          # HECI Communication
!if gProtectedVariableFeaturePkgTokenSpaceGuid.PcdProtectedVariableEnable == 1
gStructPcdTokenSpaceGuid.PcdMeSetup.HeciCommunication2|0x1                                         # HECI2 Interface Communication
!else
gStructPcdTokenSpaceGuid.PcdMeSetup.HeciCommunication2|0x0                                         # HECI2 Interface Communication
!endif
gStructPcdTokenSpaceGuid.PcdMeSetup.HeciCommunication3|0x0                                         # HECI3 Interface Communication
gStructPcdTokenSpaceGuid.PcdMeSetup.HeciTimeouts|0x1                                               # HECI Timeouts
gStructPcdTokenSpaceGuid.PcdMeSetup.KtDeviceEnable|0x0                                             # KT Device
gStructPcdTokenSpaceGuid.PcdMeSetup.MctpBroadcastCycle|0x0                                         # MCTP Broadcast Cycle
gStructPcdTokenSpaceGuid.PcdMeSetup.MeFwDowngrade|0x0                                              # Me FW Image Re-Flash
gStructPcdTokenSpaceGuid.PcdMeSetup.MeUnconfigOnRtcClear|0x1                                       # ME Unconfig on RTC Clear
gStructPcdTokenSpaceGuid.PcdMeSetup.SecureBootExposureToFw|0x1                                     # BIOS Secure Boot Capability Exposure to FW State
gStructPcdTokenSpaceGuid.PcdMeSetup.SkipMbpHob|0x0                                                 # MBP HOB Skip
gStructPcdTokenSpaceGuid.PcdMeSetup.UnConfigureMe|0x0                                              # Unconfigure ME
gStructPcdTokenSpaceGuid.PcdMeSetup.UsbProvision|0x0                                               # USB Provisioning of AMT
gStructPcdTokenSpaceGuid.PcdMeSetup.WatchDogEnabled|0x0                                            # WatchDog
gStructPcdTokenSpaceGuid.PcdMeSetup.WatchDogTimerBios|0x0                                          # BIOS Timer
gStructPcdTokenSpaceGuid.PcdMeSetup.WatchDogTimerOs|0x0                                            # OS Timer
gStructPcdTokenSpaceGuid.PcdMeSetup.vProTbtDock|0x1                                                # vPro TBT Dock Support
!if gMipiCamFeaturePkgTokenSpaceGuid.PcdMipiCamFeatureEnable == TRUE
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0|0x1                                # Control Logic 1
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_Type|0x2                           # Control Logic Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_CrdVersion|0x50                    # CRD Version
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_InputClock|0x0                     # Input Clock
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_PchClockSource|0x1                 # PCH Clock Source
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_Pld|0x29                           # PMIC Flash Panel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_I2cChannel|0x2                     # I2C Channel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_I2cAddress|0x49                    # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_Wled1Type|0x1                      # WLED1 Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_Wled1FlashMaxCurrent|0x0           # WLED1 Flash Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_Wled1TorchMaxCurrent|0x0           # WLED1 Torch Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_Wled2Type|0x0                      # WLED2 Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_Wled2FlashMaxCurrent|0x0           # WLED2 Flash Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_Wled2TorchMaxCurrent|0x0           # WLED2 Torch Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioPinsEnabled|0x0                # Number of GPIO Pins
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioGroupPadNumber[0]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioGroupPadNumber[1]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioGroupPadNumber[2]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioGroupPadNumber[3]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioGroupPadNumber[4]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioGroupPadNumber[5]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioGroupNumber[0]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioGroupNumber[1]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioGroupNumber[2]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioGroupNumber[3]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioGroupNumber[4]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioGroupNumber[5]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioComNumber[0]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioComNumber[1]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioComNumber[2]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioComNumber[3]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioComNumber[4]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioComNumber[5]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioFunction[0]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioFunction[1]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioFunction[2]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioFunction[3]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioFunction[4]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioFunction[5]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioFunction_User[0]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioFunction_User[1]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioFunction_User[2]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioFunction_User[3]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioFunction_User[4]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioFunction_User[5]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioActiveValue[0]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioActiveValue[1]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioActiveValue[2]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioActiveValue[3]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioActiveValue[4]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioActiveValue[5]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioInitialValue[0]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioInitialValue[1]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioInitialValue[2]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioInitialValue[3]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioInitialValue[4]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic0_GpioInitialValue[5]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1|0x1                                # Control Logic 2
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_Type|0x1                           # Control Logic Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_CrdVersion|0x20                    # CRD Version
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_InputClock|0x3                     # Input Clock
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_PchClockSource|0x0                 # PCH Clock Source
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_Pld|0x29                           # PMIC Flash Panel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_I2cChannel|0x0                     # I2C Channel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_I2cAddress|0x49                    # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_Wled1Type|0x0                      # WLED1 Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_Wled1FlashMaxCurrent|0x0           # WLED1 Flash Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_Wled1TorchMaxCurrent|0x0           # WLED1 Torch Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_Wled2Type|0x0                      # WLED2 Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_Wled2FlashMaxCurrent|0x0           # WLED2 Flash Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_Wled2TorchMaxCurrent|0x0           # WLED2 Torch Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioPinsEnabled|0x3                # Number of GPIO Pins
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioGroupPadNumber[0]|0x0a         # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioGroupPadNumber[1]|0x05         # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioGroupPadNumber[2]|0x14         # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioGroupPadNumber[3]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioGroupPadNumber[4]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioGroupPadNumber[5]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioGroupNumber[0]|0x01            # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioGroupNumber[1]|0x01            # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioGroupNumber[2]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioGroupNumber[3]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioGroupNumber[4]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioGroupNumber[5]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioComNumber[0]|0x01              # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioComNumber[1]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioComNumber[2]|0x01              # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioComNumber[3]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioComNumber[4]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioComNumber[5]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioFunction[0]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioFunction[1]|0x0b               # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioFunction[2]|0x0d               # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioFunction[3]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioFunction[4]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioFunction[5]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioFunction_User[0]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioFunction_User[1]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioFunction_User[2]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioFunction_User[3]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioFunction_User[4]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioFunction_User[5]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioActiveValue[0]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioActiveValue[1]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioActiveValue[2]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioActiveValue[3]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioActiveValue[4]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioActiveValue[5]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioInitialValue[0]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioInitialValue[1]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioInitialValue[2]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioInitialValue[3]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioInitialValue[4]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic1_GpioInitialValue[5]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2|0x0                                # Control Logic 3
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_Type|0x1                           # Control Logic Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_CrdVersion|0x20                    # CRD Version
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_InputClock|0x0                     # Input Clock
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_PchClockSource|0x0                 # PCH Clock Source
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_Pld|0x21                           # PMIC Flash Panel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_I2cChannel|0x0                     # I2C Channel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_I2cAddress|0x49                    # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_Wled1Type|0x0                      # WLED1 Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_Wled1FlashMaxCurrent|0x0           # WLED1 Flash Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_Wled1TorchMaxCurrent|0x0           # WLED1 Torch Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_Wled2Type|0x0                      # WLED2 Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_Wled2FlashMaxCurrent|0x0           # WLED2 Flash Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_Wled2TorchMaxCurrent|0x0           # WLED2 Torch Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioPinsEnabled|0x0                # Number of GPIO Pins
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioGroupPadNumber[0]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioGroupPadNumber[1]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioGroupPadNumber[2]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioGroupPadNumber[3]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioGroupPadNumber[4]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioGroupPadNumber[5]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioGroupNumber[0]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioGroupNumber[1]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioGroupNumber[2]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioGroupNumber[3]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioGroupNumber[4]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioGroupNumber[5]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioComNumber[0]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioComNumber[1]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioComNumber[2]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioComNumber[3]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioComNumber[4]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioComNumber[5]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioFunction[0]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioFunction[1]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioFunction[2]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioFunction[3]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioFunction[4]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioFunction[5]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioFunction_User[0]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioFunction_User[1]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioFunction_User[2]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioFunction_User[3]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioFunction_User[4]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioFunction_User[5]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioActiveValue[0]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioActiveValue[1]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioActiveValue[2]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioActiveValue[3]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioActiveValue[4]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioActiveValue[5]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioInitialValue[0]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioInitialValue[1]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioInitialValue[2]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioInitialValue[3]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioInitialValue[4]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic2_GpioInitialValue[5]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3|0x0                                # Control Logic 4
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_Type|0x1                           # Control Logic Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_CrdVersion|0x20                    # CRD Version
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_InputClock|0x0                     # Input Clock
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_PchClockSource|0x0                 # PCH Clock Source
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_Pld|0x21                           # PMIC Flash Panel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_I2cChannel|0x0                     # I2C Channel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_I2cAddress|0x49                    # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_Wled1Type|0x0                      # WLED1 Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_Wled1FlashMaxCurrent|0x0           # WLED1 Flash Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_Wled1TorchMaxCurrent|0x0           # WLED1 Torch Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_Wled2Type|0x0                      # WLED2 Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_Wled2FlashMaxCurrent|0x0           # WLED2 Flash Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_Wled2TorchMaxCurrent|0x0           # WLED2 Torch Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioPinsEnabled|0x0                # Number of GPIO Pins
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioGroupPadNumber[0]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioGroupPadNumber[1]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioGroupPadNumber[2]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioGroupPadNumber[3]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioGroupPadNumber[4]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioGroupPadNumber[5]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioGroupNumber[0]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioGroupNumber[1]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioGroupNumber[2]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioGroupNumber[3]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioGroupNumber[4]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioGroupNumber[5]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioComNumber[0]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioComNumber[1]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioComNumber[2]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioComNumber[3]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioComNumber[4]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioComNumber[5]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioFunction[0]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioFunction[1]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioFunction[2]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioFunction[3]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioFunction[4]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioFunction[5]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioFunction_User[0]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioFunction_User[1]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioFunction_User[2]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioFunction_User[3]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioFunction_User[4]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioFunction_User[5]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioActiveValue[0]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioActiveValue[1]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioActiveValue[2]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioActiveValue[3]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioActiveValue[4]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioActiveValue[5]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioInitialValue[0]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioInitialValue[1]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioInitialValue[2]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioInitialValue[3]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioInitialValue[4]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic3_GpioInitialValue[5]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4|0x0                                # Control Logic 5
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_Type|0x1                           # Control Logic Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_CrdVersion|0x20                    # CRD Version
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_InputClock|0x0                     # Input Clock
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_PchClockSource|0x0                 # PCH Clock Source
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_Pld|0x21                           # PMIC Flash Panel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_I2cChannel|0x0                     # I2C Channel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_I2cAddress|0x49                    # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_Wled1Type|0x0                      # WLED1 Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_Wled1FlashMaxCurrent|0x0           # WLED1 Flash Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_Wled1TorchMaxCurrent|0x0           # WLED1 Torch Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_Wled2Type|0x0                      # WLED2 Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_Wled2FlashMaxCurrent|0x0           # WLED2 Flash Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_Wled2TorchMaxCurrent|0x0           # WLED2 Torch Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioPinsEnabled|0x0                # Number of GPIO Pins
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioGroupPadNumber[0]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioGroupPadNumber[1]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioGroupPadNumber[2]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioGroupPadNumber[3]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioGroupPadNumber[4]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioGroupPadNumber[5]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioGroupNumber[0]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioGroupNumber[1]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioGroupNumber[2]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioGroupNumber[3]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioGroupNumber[4]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioGroupNumber[5]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioComNumber[0]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioComNumber[1]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioComNumber[2]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioComNumber[3]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioComNumber[4]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioComNumber[5]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioFunction[0]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioFunction[1]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioFunction[2]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioFunction[3]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioFunction[4]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioFunction[5]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioFunction_User[0]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioFunction_User[1]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioFunction_User[2]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioFunction_User[3]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioFunction_User[4]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioFunction_User[5]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioActiveValue[0]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioActiveValue[1]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioActiveValue[2]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioActiveValue[3]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioActiveValue[4]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioActiveValue[5]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioInitialValue[0]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioInitialValue[1]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioInitialValue[2]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioInitialValue[3]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioInitialValue[4]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic4_GpioInitialValue[5]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5|0x0                                # Control Logic 6
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_Type|0x1                           # Control Logic Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_CrdVersion|0x20                    # CRD Version
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_InputClock|0x0                     # Input Clock
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_PchClockSource|0x0                 # PCH Clock Source
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_Pld|0x21                           # PMIC Flash Panel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_I2cChannel|0x0                     # I2C Channel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_I2cAddress|0x49                    # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_Wled1Type|0x0                      # WLED1 Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_Wled1FlashMaxCurrent|0x0           # WLED1 Flash Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_Wled1TorchMaxCurrent|0x0           # WLED1 Torch Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_Wled2Type|0x0                      # WLED2 Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_Wled2FlashMaxCurrent|0x0           # WLED2 Flash Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_Wled2TorchMaxCurrent|0x0           # WLED2 Torch Max Current
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioPinsEnabled|0x0                # Number of GPIO Pins
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioGroupPadNumber[0]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioGroupPadNumber[1]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioGroupPadNumber[2]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioGroupPadNumber[3]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioGroupPadNumber[4]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioGroupPadNumber[5]|0x0          # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioGroupNumber[0]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioGroupNumber[1]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioGroupNumber[2]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioGroupNumber[3]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioGroupNumber[4]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioGroupNumber[5]|0x0             # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioComNumber[0]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioComNumber[1]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioComNumber[2]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioComNumber[3]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioComNumber[4]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioComNumber[5]|0x0               # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioFunction[0]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioFunction[1]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioFunction[2]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioFunction[3]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioFunction[4]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioFunction[5]|0x0                # Function
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioFunction_User[0]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioFunction_User[1]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioFunction_User[2]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioFunction_User[3]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioFunction_User[4]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioFunction_User[5]|0x15          # User Custom
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioActiveValue[0]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioActiveValue[1]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioActiveValue[2]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioActiveValue[3]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioActiveValue[4]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioActiveValue[5]|0x1             # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioInitialValue[0]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioInitialValue[1]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioInitialValue[2]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioInitialValue[3]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioInitialValue[4]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_ControlLogic5_GpioInitialValue[5]|0x0            # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash0_GpioActiveValue|0x1                       # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash0_GpioComNumber|0x0                         # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash0_GpioGroupNumber|0x0                       # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash0_GpioGroupPadNumber|0x0                    # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash0_GpioInitialValue|0x0                      # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash0_GpioSupport|0x0                           # Flash Trigger Gpio Support
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash0_I2cAddress|0x67                           # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash0_I2cChannel|0x0                            # I2C Channel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash0_Mode|0x0                                  # Flash Mode
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash0_Model|0x0                                 # Flash Model
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash0_ModuleName|{0x0, 0x0}                     # Camera module name
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash0_UserHid|{0x0, 0x0}                        # Custom HID
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash1_GpioActiveValue|0x1                       # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash1_GpioComNumber|0x0                         # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash1_GpioGroupNumber|0x0                       # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash1_GpioGroupPadNumber|0x0                    # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash1_GpioInitialValue|0x0                      # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash1_GpioSupport|0x0                           # Flash Trigger Gpio Support
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash1_I2cAddress|0x63                           # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash1_I2cChannel|0x2                            # I2C Channel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash1_Mode|0x3                                  # Flash Mode
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash1_Model|0x0                                 # Flash Model
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash1_ModuleName|L"CJFKE26"                     # Camera module name
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash1_UserHid|{0x0, 0x0}                        # Custom HID
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash2_GpioActiveValue|0x1                       # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash2_GpioComNumber|0x0                         # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash2_GpioGroupNumber|0x0                       # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash2_GpioGroupPadNumber|0x0                     # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash2_GpioInitialValue|0x0                      # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash2_GpioSupport|0x0                           # Flash Trigger Gpio Support
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash2_I2cAddress|0x63                           # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash2_I2cChannel|0x2                            # I2C Channel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash2_Mode|0x3                                  # Flash Mode
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash2_Model|0x0                                 # Flash Model
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash2_ModuleName|{0x0, 0x0}                     # Camera module name
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash2_UserHid|{0x0, 0x0}                        # Custom HID
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash3_GpioActiveValue|0x1                       # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash3_GpioComNumber|0x0                         # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash3_GpioGroupNumber|0x0                       # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash3_GpioGroupPadNumber|0x0                    # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash3_GpioInitialValue|0x0                      # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash3_GpioSupport|0x0                           # Flash Trigger Gpio Support
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash3_I2cAddress|0x67                           # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash3_I2cChannel|0x0                            # I2C Channel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash3_Mode|0x0                                  # Flash Mode
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash3_Model|0x0                                 # Flash Model
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash3_ModuleName|{0x0, 0x0}                     # Camera module name
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash3_UserHid|{0x0, 0x0}                        # Custom HID
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash4_GpioActiveValue|0x1                       # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash4_GpioComNumber|0x0                         # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash4_GpioGroupNumber|0x0                       # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash4_GpioGroupPadNumber|0x0                    # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash4_GpioInitialValue|0x0                      # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash4_GpioSupport|0x0                           # Flash Trigger Gpio Support
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash4_I2cAddress|0x67                           # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash4_I2cChannel|0x0                            # I2C Channel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash4_Mode|0x0                                  # Flash Mode
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash4_Model|0x0                                 # Flash Model
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash4_ModuleName|{0x0, 0x0}                     # Camera module name
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash4_UserHid|{0x0, 0x0}                        # Custom HID
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash5_GpioActiveValue|0x1                       # Active Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash5_GpioComNumber|0x0                         # Com number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash5_GpioGroupNumber|0x0                       # Group Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash5_GpioGroupPadNumber|0x0                    # Group Pad Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash5_GpioInitialValue|0x0                      # Initial Value
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash5_GpioSupport|0x0                           # Flash Trigger Gpio Support
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash5_I2cAddress|0x67                           # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash5_I2cChannel|0x0                            # I2C Channel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash5_Mode|0x0                                  # Flash Mode
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash5_Model|0x0                                 # Flash Model
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash5_ModuleName|{0x0, 0x0}                     # Camera module name
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Flash5_UserHid|{0x0, 0x0}                        # Custom HID
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0|0x1                                        # Camera1
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_CameraPhysicalLocation|0x69                # Camera position
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_Customize_Id[0]|0x11                       # Customize Device1 ID Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_Customize_Id[1]|0x12                       # Customize Device2 ID Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_Customize_Id[2]|0x13                       # Customize Device3 ID Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_ControlLogic|0x0                # GPIO control
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_CrdVersion|0x50                 # CRD Version
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_Degree|0x0                      # Rotation
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_EepromType|0x6                  # EEPROM Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_EepromTypeCustom|0x0            # Custom  EEPROM
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_FlashID|0x0                     # Flash Driver Selection
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_FlashSupport|0x3                # Flash Support
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_LaneConfiguration|0x0           # LaneConfiguration
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_LaneUsed|0x2                    # LaneUsed
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_LinkUsed|0x2                    # MIPI port
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_Mclk|0x124f800                  # MCLK
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_PhyConfiguration|0x0            # PhyConfiguration
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_PmicPosition|0x0                # PMIC Position
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_PrivacyLed|0x0                  # Privacy LED
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_VcmType|0x2                     # VCM Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_VcmTypeCustom|0x0               # Custom VCM
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_DriverData_VoltageRail|0x0                 # Voltage Rail
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_FlashDriverSelection|0x0                   # Flash Driver Selection
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cAddress[0]|0x10                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cAddress[1]|0x0c                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cAddress[2]|0x50                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cAddress[3]|0x51                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cAddress[4]|0x52                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cAddress[5]|0x53                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cAddress[6]|0x54                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cAddress[7]|0x55                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cAddress[8]|0x56                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cAddress[9]|0x57                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cAddress[10]|0x58                        # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cAddress[11]|0x59                        # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cChannel|0x2                             # I2C Channel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cDeviceType[0]|0x0                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cDeviceType[1]|0x1                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cDeviceType[2]|0x2                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cDeviceType[3]|0x3                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cDeviceType[4]|0x4                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cDeviceType[5]|0x5                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cDeviceType[6]|0x6                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cDeviceType[7]|0x7                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cDeviceType[8]|0x8                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cDeviceType[9]|0x9                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cDeviceType[10]|0xa                      # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cDeviceType[11]|0xc                      # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_I2cDevicesEnabled|0x7                      # Number of I2C Components
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_ModuleName|L"09B13"                        # Camera module name
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_SensorModel|0x1                            # Sensor Model
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link0_UserHid|{0x0, 0x0}                         # Custom HID
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1|0x1                                        # Camera2
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_CameraPhysicalLocation|0x61                # Camera position
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_Customize_Id[0]|0x11                       # Customize Device1 ID Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_Customize_Id[1]|0x12                       # Customize Device2 ID Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_Customize_Id[2]|0x13                       # Customize Device3 ID Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_ControlLogic|0x0                # GPIO control
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_CrdVersion|0x50                 # CRD Version
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_Degree|0x0                      # Rotation
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_EepromType|0x0                  # EEPROM Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_EepromTypeCustom|0x0            # Custom  EEPROM
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_FlashID|0x1                     # Flash Driver Selection
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_FlashSupport|0x3                # Flash Support
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_LaneConfiguration|0x0           # LaneConfiguration
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_LaneUsed|0x1                    # LaneUsed
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_LinkUsed|0x0                    # MIPI port
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_Mclk|0x124f800                  # MCLK
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_PhyConfiguration|0x0            # PhyConfiguration
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_PmicPosition|0x1                # PMIC Position
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_PrivacyLed|0x0                  # Privacy LED
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_VcmType|0x0                     # VCM Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_VcmTypeCustom|0x0               # Custom VCM
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_DriverData_VoltageRail|0x0                 # Voltage Rail
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_FlashDriverSelection|0x1                   # Flash Driver Selection
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cAddress[0]|0x60                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cAddress[1]|0x0c                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cAddress[2]|0x50                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cAddress[3]|0x51                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cAddress[4]|0x52                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cAddress[5]|0x53                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cAddress[6]|0x54                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cAddress[7]|0x55                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cAddress[8]|0x56                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cAddress[9]|0x57                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cAddress[10]|0x58                        # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cAddress[11]|0x59                        # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cChannel|0x2                             # I2C Channel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cDeviceType[0]|0x0                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cDeviceType[1]|0x1                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cDeviceType[2]|0x2                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cDeviceType[3]|0x3                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cDeviceType[4]|0x4                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cDeviceType[5]|0x5                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cDeviceType[6]|0x6                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cDeviceType[7]|0x7                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cDeviceType[8]|0x8                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cDeviceType[9]|0x9                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cDeviceType[10]|0xa                      # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cDeviceType[11]|0xc                      # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_I2cDevicesEnabled|0x1                      # Number of I2C Components
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_ModuleName|L"CJFKE26"                      # Camera module name
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_SensorModel|0x2                            # Sensor Model
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link1_UserHid|{0x0, 0x0}                         # Custom HID
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2|0x1                                        # Camera3
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_CameraPhysicalLocation|0x61                # Camera position
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_Customize_Id[0]|0x11                       # Customize Device1 ID Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_Customize_Id[1]|0x12                       # Customize Device2 ID Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_Customize_Id[2]|0x13                       # Customize Device3 ID Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_DriverData_ControlLogic|0x1                # GPIO control
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_DriverData_CrdVersion|0x20                 # CRD Version
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_DriverData_Degree|0x0                      # Rotation
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_DriverData_EepromType|0x14                 # EEPROM Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_DriverData_EepromTypeCustom|0x0            # Custom  EEPROM
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_DriverData_FlashID|0x2                     # Flash Driver Selection
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_DriverData_FlashSupport|0x2                # Flash Support
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_DriverData_LaneConfiguration|0x0           # LaneConfiguration
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_DriverData_LaneUsed|0x2                    # LaneUsed
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_DriverData_LinkUsed|0x1                    # MIPI port
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_DriverData_Mclk|0x124f800                  # MCLK
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_DriverData_PhyConfiguration|0x0            # PhyConfiguration
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_DriverData_PmicPosition|0x0                # PMIC Position
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_DriverData_PrivacyLed|0x0                  # Privacy LED
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_DriverData_VcmType|0x0                     # VCM Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_DriverData_VcmTypeCustom|0x0               # Custom VCM
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_DriverData_VoltageRail|0x0                 # Voltage Rail
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_FlashDriverSelection|0x0                   # Flash Driver Selection
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cAddress[0]|0x10                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cAddress[1]|0x50                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cAddress[2]|0x0c                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cAddress[3]|0x51                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cAddress[4]|0x52                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cAddress[5]|0x53                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cAddress[6]|0x54                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cAddress[7]|0x55                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cAddress[8]|0x56                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cAddress[9]|0x57                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cAddress[10]|0x58                        # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cAddress[11]|0x59                        # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cChannel|0x1                             # I2C Channel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cDeviceType[0]|0x0                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cDeviceType[1]|0x2                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cDeviceType[2]|0x1                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cDeviceType[3]|0x3                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cDeviceType[4]|0x4                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cDeviceType[5]|0x5                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cDeviceType[6]|0x6                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cDeviceType[7]|0x7                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cDeviceType[8]|0x8                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cDeviceType[9]|0x0                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cDeviceType[10]|0x0                      # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cDeviceType[11]|0x0                      # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_I2cDevicesEnabled|0x2                      # Number of I2C Components
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_ModuleName|L"KAFE799"                       # Camera module name
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_SensorModel|0x4                            # Sensor Model
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link2_UserHid|{0x0, 0x0}                         # Custom HID
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3|0x0                                        # Camera4
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_CameraPhysicalLocation|0x69                # Camera position
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_Customize_Id[0]|0x11                       # Customize Device1 ID Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_Customize_Id[1]|0x12                       # Customize Device2 ID Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_Customize_Id[2]|0x13                       # Customize Device3 ID Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_DriverData_ControlLogic|0xFF               # GPIO control
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_DriverData_CrdVersion|0x20                 # CRD Version
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_DriverData_Degree|0x0                      # Rotation
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_DriverData_EepromType|0x0                  # EEPROM Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_DriverData_EepromTypeCustom|0x0            # Custom  EEPROM
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_DriverData_FlashID|0x3                     # Flash Driver Selection
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_DriverData_FlashSupport|0x0                # Flash Support
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_DriverData_LaneConfiguration|0x0           # LaneConfiguration
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_DriverData_LaneUsed|0x1                    # LaneUsed
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_DriverData_LinkUsed|0x0                    # MIPI port
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_DriverData_Mclk|0x124f800                  # MCLK
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_DriverData_PhyConfiguration|0x0            # PhyConfiguration
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_DriverData_PmicPosition|0x0                # PMIC Position
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_DriverData_PrivacyLed|0x0                  # Privacy LED
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_DriverData_VcmType|0x0                     # VCM Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_DriverData_VcmTypeCustom|0x0               # Custom VCM
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_DriverData_VoltageRail|0x0                 # Voltage Rail
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_FlashDriverSelection|0x0                   # Flash Driver Selection
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cAddress[0]|0x10                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cAddress[1]|0x0c                          # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cAddress[2]|0x50                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cAddress[3]|0x51                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cAddress[4]|0x52                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cAddress[5]|0x53                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cAddress[6]|0x54                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cAddress[7]|0x55                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cAddress[8]|0x56                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cAddress[9]|0x57                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cAddress[10]|0x58                        # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cAddress[11]|0x59                        # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cChannel|0x0                             # I2C Channel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cDeviceType[0]|0x0                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cDeviceType[1]|0x1                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cDeviceType[2]|0x2                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cDeviceType[3]|0x3                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cDeviceType[4]|0x4                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cDeviceType[5]|0x5                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cDeviceType[6]|0x6                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cDeviceType[7]|0x7                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cDeviceType[8]|0x8                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cDeviceType[9]|0x9                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cDeviceType[10]|0xa                      # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cDeviceType[11]|0xc                      # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_I2cDevicesEnabled|0x0                      # Number of I2C Components
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_ModuleName|{0x0, 0x0}                      # Camera module name
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_SensorModel|0x0                            # Sensor Model
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link3_UserHid|{0x0, 0x0}                         # Custom HID
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4|0x0                                        # Camera5
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_CameraPhysicalLocation|0x69                # Camera position
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_Customize_Id[0]|0x11                       # Customize Device1 ID Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_Customize_Id[1]|0x12                       # Customize Device2 ID Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_Customize_Id[2]|0x13                       # Customize Device3 ID Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_DriverData_ControlLogic|0xFF               # GPIO control
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_DriverData_CrdVersion|0x20                 # CRD Version
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_DriverData_Degree|0x0                      # Rotation
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_DriverData_EepromType|0x0                  # EEPROM Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_DriverData_EepromTypeCustom|0x0            # Custom  EEPROM
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_DriverData_FlashID|0x4                     # Flash Driver Selection
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_DriverData_FlashSupport|0x0                # Flash Support
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_DriverData_LaneConfiguration|0x0           # LaneConfiguration
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_DriverData_LaneUsed|0x1                    # LaneUsed
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_DriverData_LinkUsed|0x0                    # MIPI port
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_DriverData_Mclk|0x124f800                  # MCLK
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_DriverData_PhyConfiguration|0x0            # PhyConfiguration
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_DriverData_PmicPosition|0x0                # PMIC Position
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_DriverData_PrivacyLed|0x0                  # Privacy LED
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_DriverData_VcmType|0x0                     # VCM Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_DriverData_VcmTypeCustom|0x0               # Custom VCM
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_DriverData_VoltageRail|0x0                 # Voltage Rail
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_FlashDriverSelection|0x0                   # Flash Driver Selection
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cAddress[0]|0x10                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cAddress[1]|0xc                          # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cAddress[2]|0x50                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cAddress[3]|0x51                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cAddress[4]|0x52                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cAddress[5]|0x53                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cAddress[6]|0x54                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cAddress[7]|0x55                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cAddress[8]|0x56                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cAddress[9]|0x57                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cAddress[10]|0x58                        # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cAddress[11]|0x59                        # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cChannel|0x0                             # I2C Channel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cDeviceType[0]|0x0                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cDeviceType[1]|0x1                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cDeviceType[2]|0x2                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cDeviceType[3]|0x3                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cDeviceType[4]|0x4                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cDeviceType[5]|0x5                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cDeviceType[6]|0x6                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cDeviceType[7]|0x7                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cDeviceType[8]|0x8                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cDeviceType[9]|0x9                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cDeviceType[10]|0xa                      # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cDeviceType[11]|0xc                      # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_I2cDevicesEnabled|0x0                      # Number of I2C Components
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_ModuleName|{0x0, 0x0}                      # Camera module name
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_SensorModel|0x0                            # Sensor Model
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link4_UserHid|{0x0, 0x0}                         # Custom HID
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5|0x0                                        # Camera6
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_CameraPhysicalLocation|0x69                # Camera position
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_Customize_Id[0]|0x11                       # Customize Device1 ID Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_Customize_Id[1]|0x12                       # Customize Device2 ID Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_Customize_Id[2]|0x13                       # Customize Device3 ID Number
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_DriverData_ControlLogic|0xFF               # GPIO control
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_DriverData_CrdVersion|0x20                 # CRD Version
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_DriverData_Degree|0x0                      # Rotation
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_DriverData_EepromType|0x0                  # EEPROM Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_DriverData_EepromTypeCustom|0x0            # Custom  EEPROM
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_DriverData_FlashID|0x5                     # Flash Driver Selection
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_DriverData_FlashSupport|0x0                # Flash Support
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_DriverData_LaneConfiguration|0x0           # LaneConfiguration
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_DriverData_LaneUsed|0x1                    # LaneUsed
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_DriverData_LinkUsed|0x0                    # MIPI port
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_DriverData_Mclk|0x124f800                  # MCLK
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_DriverData_PhyConfiguration|0x0            # PhyConfiguration
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_DriverData_PmicPosition|0x0                # PMIC Position
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_DriverData_PrivacyLed|0x0                  # Privacy LED
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_DriverData_VcmType|0x0                     # VCM Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_DriverData_VcmTypeCustom|0x0               # Custom VCM
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_DriverData_VoltageRail|0x0                 # Voltage Rail
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_FlashDriverSelection|0x0                   # Flash Driver Selection
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cAddress[0]|0x10                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cAddress[1]|0x0C                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cAddress[2]|0x50                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cAddress[3]|0x51                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cAddress[4]|0x52                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cAddress[5]|0x53                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cAddress[6]|0x54                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cAddress[7]|0x55                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cAddress[8]|0x56                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cAddress[9]|0x57                         # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cAddress[10]|0x58                        # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cAddress[11]|0x59                        # I2C Address
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cChannel|0x0                             # I2C Channel
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cDeviceType[0]|0x0                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cDeviceType[1]|0x1                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cDeviceType[2]|0x2                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cDeviceType[3]|0x3                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cDeviceType[4]|0x4                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cDeviceType[5]|0x5                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cDeviceType[6]|0x6                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cDeviceType[7]|0x7                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cDeviceType[8]|0x8                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cDeviceType[9]|0x9                       # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cDeviceType[10]|0xa                      # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cDeviceType[11]|0xc                      # Device Type
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_I2cDevicesEnabled|0x0                      # Number of I2C Components
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_ModuleName|{0x0, 0x0}                      # Camera module name
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_SensorModel|0x0                            # Sensor Model
    gStructPcdTokenSpaceGuid.PcdMipiCamConfig.MipiCam_Link5_UserHid|{0x0, 0x0}                         # Custom HID
!endif
gStructPcdTokenSpaceGuid.PcdNhltEndpointsTableConfigurationVariable.NhltDmicMonoEnabled|0x0        # Dmic Mono
gStructPcdTokenSpaceGuid.PcdNhltEndpointsTableConfigurationVariable.NhltDmicStereoEnabled|0x0      # Dmic Stereo
gStructPcdTokenSpaceGuid.PcdNhltEndpointsTableConfigurationVariable.NhltDmicStereoVpEnabled|0x0    # Dmic Stereo VP
gStructPcdTokenSpaceGuid.PcdNhltEndpointsTableConfigurationVariable.NhltDmicQuadEnabled|0x1        # Dmic Quad
gStructPcdTokenSpaceGuid.PcdNhltEndpointsTableConfigurationVariable.NhltBluetoothEnabled|0x2       # Bt
gStructPcdTokenSpaceGuid.PcdNhltEndpointsTableConfigurationVariable.NhltI2sAlc274Enabled|0x0       # I2s Alc274
gStructPcdTokenSpaceGuid.PcdNvmeSetup.PlnEnable|0x2                                                # Power Loss Notification Feature
gStructPcdTokenSpaceGuid.PcdPchSetup.AetTraceHubMode|0x0                                           # AET Trace Hub Destination
gStructPcdTokenSpaceGuid.PcdPchSetup.BiosTraceSinkMode|0x0                                         # BIOS trace destination
gStructPcdTokenSpaceGuid.PcdPchSetup.CompletionTimeout[0]|0x0                                      # Completion Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.CompletionTimeout[1]|0x0                                      # Completion Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.CompletionTimeout[2]|0x0                                      # Completion Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.CompletionTimeout[3]|0x0                                      # Completion Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.CompletionTimeout[4]|0x0                                      # Completion Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.CompletionTimeout[5]|0x0                                      # Completion Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.CompletionTimeout[6]|0x0                                      # Completion Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.CompletionTimeout[7]|0x0                                      # Completion Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.CompletionTimeout[8]|0x0                                      # Completion Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.CompletionTimeout[9]|0x0                                      # Completion Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.CompletionTimeout[10]|0x0                                     # Completion Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.CompletionTimeout[11]|0x0                                     # Completion Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.CpuBclkPllOn|0x0                                              # CPU BCLK PLL configuration
gStructPcdTokenSpaceGuid.PcdPchSetup.DciClkEnable|0x1                                              # DCI Clock Enable
gStructPcdTokenSpaceGuid.PcdPchSetup.DciDbcMode|0x4                                                # USB DbC Enable Mode
gStructPcdTokenSpaceGuid.PcdPchSetup.DciEn|0x0                                                     # DCI Enable
gStructPcdTokenSpaceGuid.PcdPchSetup.Enable8254ClockGating|0x1                                     # Enable 8254 Clock Gate
gStructPcdTokenSpaceGuid.PcdPchSetup.EnableTcoTimer|0x0                                            # Enable TCO Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.EnhancePort8xhDecoding|0x1                                    # Enhance Port 80h LPC Decoding
!if gPlatformModuleTokenSpaceGuid.PcdExtendedBiosRegionSupport == TRUE
    gStructPcdTokenSpaceGuid.PcdPchSetup.ExtendedBiosDecodeRangeEnable|0x1                         # Extended BIOS Range Decode
!endif
gStructPcdTokenSpaceGuid.PcdPchSetup.FprrEnable|0x1                                                # Flash Protection Range Registers (FPRR)
gStructPcdTokenSpaceGuid.PcdPchSetup.HdaDiscBtOffEnabled|0x0                                       # Discrete BT HCI Audio Offload Support
gStructPcdTokenSpaceGuid.PcdPchSetup.HdaDiscBtOffSspLink|0x0                                       # Discrete BT HCI Audio Offload Link
gStructPcdTokenSpaceGuid.PcdPchSetup.IchPort80Route|0x0                                            # Port 80h Redirection
gStructPcdTokenSpaceGuid.PcdPchSetup.IehMode|0x0                                                   # IEH Mode
gStructPcdTokenSpaceGuid.PcdPchSetup.KeepEarlyTrace|0x0                                            # Keep Early Trace
gStructPcdTokenSpaceGuid.PcdPchSetup.PchAdditionalSerialIoDevices|0x0                              # Additional Serial IO devices
gStructPcdTokenSpaceGuid.PcdPchSetup.PchBiosLock|0x1                                               # BIOS Lock
gStructPcdTokenSpaceGuid.PcdPchSetup.PchCrid|0x0                                                   # Compatible Revision ID
gStructPcdTokenSpaceGuid.PcdPchSetup.PchEnergyReport|0x1                                           # PCH Energy Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PchEspiHostC10ReportEnable|0x0                                # HOST_C10 reporting to Target
gStructPcdTokenSpaceGuid.PcdPchSetup.PchEspiLgmrEnable|0x0                                         # LGMR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchGpioIrqRoute|0xe                                           # GPIO IRQ Route
!if gSiPkgTokenSpaceGuid.PcdEmbeddedEnable == 0x0
    gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudio|0x1                                            # HD Audio
!else
    gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudio|0x0                                            # HD Audio
!endif
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioCodecSxWakeCapability|0x0                           # Codec Sx Wake Capability
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioDmicLinkEnable[0]|0x1                               # DMIC #0
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioDmicLinkEnable[1]|0x1                               # DMIC #1
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioDsp|0x1                                             # Audio DSP
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioDspUaaCompliance|0x0                                # Audio DSP Compliance Mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioFeature[0]|0x1                                      # WoV (Wake on Voice)
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioFeature[1]|0x1                                      # Bluetooth Sideband
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioFeature[2]|0x0                                      # Codec based VAD
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioFeature[5]|0x1                                      # BT Intel HFP
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioFeature[6]|0x1                                      # BT Intel A2DP
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioFeature[9]|0x1                                      # BT Intel Low Energy
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioFeature[10]|0x0                                     # ACX/SDCA
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioFeature[11]|0x0                                     # ACX/SDCA speaker aggregation
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioHdaLinkEnable|0x0                                   # HDA Link
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioI2sCodecSelect|0x0                                  # I2S Codec Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPme|0x0                                             # PME Enable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingMod[0]|0x0                            # Waves Post-process
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingMod[1]|0x0                            # DTS
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingMod[2]|0x0                            # IntelSST Speech
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingMod[3]|0x0                            # Dolby
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingMod[4]|0x0                            # Waves Pre-process
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingMod[5]|0x0                            # Audyssey
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingMod[6]|0x0                            # Maxim Smart AMP
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingMod[7]|0x0                            # ForteMedia SAMSoft
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingMod[9]|0x0                            # Sound Research IP
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingMod[10]|0x0                           # Conexant Pre-Process
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingMod[11]|0x0                           # Conexant Smart Amp
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingMod[12]|0x0                           # Realtek Post-Process
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingMod[13]|0x0                           # Realtek Smart Amp
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingMod[14]|0x0                           # Icepower IP MFX sub module
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingMod[15]|0x0                           # Icepower IP EFX sub module
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingMod[16]|0x0                           # Icepower IP SFX sub module
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingMod[17]|0x0                           # Voice Preprocessing
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingMod[18]|0x0                           # Acoustic Context Awareness (ACA)
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingMod[27]|0x0                           # AEC for Cirrus
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingMod[28]|0x0                           # Waves Full and Reduced for Cirrus
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingMod[29]|0x0                           # Custom Module 'Alpha'
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingMod[30]|0x0                           # Custom Module 'Beta'
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingMod[31]|0x0                           # Custom Module 'Gamma'
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingModCustomGuid1|{0x0, 0x0}          # 'Alpha' GUID:
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingModCustomGuid2|{0x0, 0x0}          # 'Beta'  GUID:
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioPostProcessingModCustomGuid3|{0x0, 0x0}          # 'Gamma' GUID:
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioSdiLaneEnable[0]|0x1                                # SDI0
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioSdiLaneEnable[1]|0x0                                # SDI1
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioSndwLinkEnable[0]|0x0                               # SNDW #0
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioSndwLinkEnable[1]|0x1                               # SNDW #1
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioSndwLinkEnable[2]|0x0                               # SNDW #2
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioSndwLinkEnable[3]|0x0                               # SNDW #3
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioSndwMultilaneEnable[0]|0x0                          # SNDW #2 Multilane
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioSndwMultilaneEnable[1]|0x2                          # SNDW #3 Multilane
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioSspLinkEnable[0]|0x0                                # SSP #0
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioSspLinkEnable[1]|0x0                                # SSP #1
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdAudioSspLinkEnable[2]|0x0                                # SSP #2
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaAlc245DmicConfiguration|0x0                             # HDA Codec ALC245 Configuration
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaAutonomousClockStopSndw[0]|0x0                          # Autonomous Clock Stop
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaAutonomousClockStopSndw[1]|0x0                          # Autonomous Clock Stop
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaAutonomousClockStopSndw[2]|0x0                          # Autonomous Clock Stop
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaAutonomousClockStopSndw[3]|0x0                          # Autonomous Clock Stop
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaAutonomousClockStopSndw[4]|0x0                          # Autonomous Clock Stop
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaClockLoopbackDelaySelectSndw[0]|0x2                     # Clock Loopback Delay Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaClockLoopbackDelaySelectSndw[1]|0x2                     # Clock Loopback Delay Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaClockLoopbackDelaySelectSndw[2]|0x2                     # Clock Loopback Delay Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaClockLoopbackDelaySelectSndw[3]|0x2                     # Clock Loopback Delay Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaClockLoopbackDelaySelectSndw[4]|0x2                     # Clock Loopback Delay Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaClockLoopbackEnableSndw[0]|0x0                          # Source Clock Loopback Enable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaClockLoopbackEnableSndw[1]|0x0                          # Source Clock Loopback Enable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaClockLoopbackEnableSndw[2]|0x0                          # Source Clock Loopback Enable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaClockLoopbackEnableSndw[3]|0x0                          # Source Clock Loopback Enable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaClockLoopbackEnableSndw[4]|0x0                          # Source Clock Loopback Enable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaClockLoopbackSourceSndw[0]|0x1                          # Source Clock Loopback Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaClockLoopbackSourceSndw[1]|0x1                          # Source Clock Loopback Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaClockLoopbackSourceSndw[2]|0x1                          # Source Clock Loopback Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaClockLoopbackSourceSndw[3]|0x1                          # Source Clock Loopback Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaClockLoopbackSourceSndw[4]|0x1                          # Source Clock Loopback Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnActiveIntervalExt2SelectSndw[0]|0x0               # Data On Active Interval Extension 2 Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnActiveIntervalExt2SelectSndw[1]|0x0               # Data On Active Interval Extension 2 Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnActiveIntervalExt2SelectSndw[2]|0x0               # Data On Active Interval Extension 2 Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnActiveIntervalExt2SelectSndw[3]|0x0               # Data On Active Interval Extension 2 Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnActiveIntervalExt2SelectSndw[4]|0x0               # Data On Active Interval Extension 2 Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnActiveIntervalExtSelectSndw[0]|0x1                # Data On Active Interval Extension Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnActiveIntervalExtSelectSndw[1]|0x1                # Data On Active Interval Extension Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnActiveIntervalExtSelectSndw[2]|0x1                # Data On Active Interval Extension Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnActiveIntervalExtSelectSndw[3]|0x1                # Data On Active Interval Extension Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnActiveIntervalExtSelectSndw[4]|0x1                # Data On Active Interval Extension Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnActiveIntervalSelectSndw[0]|0x3                   # Data On Active Interval Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnActiveIntervalSelectSndw[1]|0x3                   # Data On Active Interval Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnActiveIntervalSelectSndw[2]|0x3                   # Data On Active Interval Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnActiveIntervalSelectSndw[3]|0x3                   # Data On Active Interval Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnActiveIntervalSelectSndw[4]|0x3                   # Data On Active Interval Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnDelaySelectExt2Sndw[0]|0x1                        # Data On Delay Select Extension
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnDelaySelectExt2Sndw[1]|0x1                        # Data On Delay Select Extension
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnDelaySelectExt2Sndw[2]|0x1                        # Data On Delay Select Extension
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnDelaySelectExt2Sndw[3]|0x1                        # Data On Delay Select Extension
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnDelaySelectExt2Sndw[4]|0x1                        # Data On Delay Select Extension
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnDelaySelectExtSndw[0]|0x1                         # Data On Delay Extension Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnDelaySelectExtSndw[1]|0x1                         # Data On Delay Extension Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnDelaySelectExtSndw[2]|0x1                         # Data On Delay Extension Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnDelaySelectExtSndw[3]|0x1                         # Data On Delay Extension Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnDelaySelectExtSndw[4]|0x1                         # Data On Delay Extension Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnDelaySelectSndw[0]|0x0                            # Data On Delay Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnDelaySelectSndw[1]|0x0                            # Data On Delay Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnDelaySelectSndw[2]|0x0                            # Data On Delay Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnDelaySelectSndw[3]|0x0                            # Data On Delay Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaDataOnDelaySelectSndw[4]|0x0                            # Data On Delay Select
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaHdAudioLinkFreq|0x2                                     # HD Audio Link Frequency
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaIDisplayCodecDisconnect|0x0                             # iDisplay Audio Disconnect
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaIDisplayLinkFreq|0x4                                    # iDisplay Audio Link Frequency
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaIDisplayLinkTmode|0x3                                   # iDisplay Audio Link T-Mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaMicPrivacyDeglitch|0x1                                  # Deglitch
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaMicPrivacyHwModeDmic|0x1                                # DMIC
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaMicPrivacyHwModeSoundWire0|0x1                          # SNDW #0
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaMicPrivacyHwModeSoundWire1|0x1                          # SNDW #1
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaMicPrivacyHwModeSoundWire2|0x1                          # SNDW #2
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaMicPrivacyHwModeSoundWire3|0x1                          # SNDW #3
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaMicPrivacyHwModeSoundWire4|0x1                          # SNDW #4
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaMicPrivacyMode|0x1                                      # Microphone Privacy Mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaMicPrivacyTimeout|0x28                                  # Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PchHdaSndwClockSrcSelect|0x0                                  # SoundWire clock source
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIoApic24119Entries|0x1                                     # IOAPIC 24-119 Entries
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshEnable|0x1                                              # ISH Controller
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshGpEnable[0]|0x1                                         # GP_0
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshGpEnable[1]|0x1                                         # GP_1
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshGpEnable[2]|0x1                                         # GP_2
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshGpEnable[3]|0x1                                         # GP_3
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshGpEnable[4]|0x1                                         # GP_4
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshGpEnable[5]|0x1                                         # GP_5
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshGpEnable[6]|0x1                                         # GP_6
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshGpEnable[7]|0x1                                         # GP_7
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshGpEnable[8]|0x0                                         # GP_8
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshGpEnable[9]|0x0                                         # GP_9
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshGpEnable[10]|0x0                                        # GP_10
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshGpEnable[11]|0x1                                        # GP_11
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshI2cEnable[0]|0x1                                        # I2C0
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshI2cEnable[1]|0x0                                        # I2C1
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshI2cEnable[2]|0x1                                        # I2C2
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshI3cEnable[0]|0x0                                        # I3C0
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshI3cEnable[1]|0x1                                        # I3C1
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshMsiInterrupt|0x1                                        # MSI Interrupts
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshPdtUnlock|0x0                                           # PDT Unlock Message
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshSpiCs0Enable[0]|0x0                                     # SPI_0 CS_0
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshSpiEnable[0]|0x0                                        # SPI_0
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshUartEnable[0]|0x0                                       # UART0
gStructPcdTokenSpaceGuid.PcdPchSetup.PchIshUartEnable[1]|0x0                                       # UART1
gStructPcdTokenSpaceGuid.PcdPchSetup.PchLan|0x1                                                    # PCH LAN Controller
gStructPcdTokenSpaceGuid.PcdPchSetup.PchLatchEventsC10Exit|0x0                                     # Latch Events C10 Exit
gStructPcdTokenSpaceGuid.PcdPchSetup.PchLegacyIoLowLatency|0x0                                     # Legacy IO Low Latency
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieClkReqSupport[0]|0x0                                   # ClkReq for Clock0
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieClkReqSupport[1]|0x0                                   # ClkReq for Clock1
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieClkReqSupport[2]|0x0                                   # ClkReq for Clock2
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieClkReqSupport[3]|0x0                                   # ClkReq for Clock3
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieClkReqSupport[4]|0x0                                   # ClkReq for Clock4
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieClkReqSupport[5]|0x0                                   # ClkReq for Clock5
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieClkReqSupport[6]|0x0                                   # ClkReq for Clock6
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieClkReqSupport[7]|0x0                                   # ClkReq for Clock7
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieClkReqSupport[8]|0x0                                   # ClkReq for Clock8
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieClockUsageOverride[0]|0x0                              # Clock0 assignment
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieClockUsageOverride[1]|0x0                              # Clock1 assignment
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieClockUsageOverride[2]|0x0                              # Clock2 assignment
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieClockUsageOverride[3]|0x0                              # Clock3 assignment
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieClockUsageOverride[4]|0x0                              # Clock4 assignment
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieClockUsageOverride[5]|0x0                              # Clock5 assignment
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieClockUsageOverride[6]|0x0                              # Clock6 assignment
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieClockUsageOverride[7]|0x0                              # Clock7 assignment
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieClockUsageOverride[8]|0x0                              # Clock8 assignment
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[0]|0x1                                       # LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[1]|0x1                                       # LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[2]|0x1                                       # LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[3]|0x1                                       # LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[4]|0x1                                       # LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[5]|0x1                                       # LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[6]|0x1                                       # LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[7]|0x1                                       # LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[8]|0x1                                       # LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[9]|0x1                                       # LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[10]|0x1                                      # LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrEnable[11]|0x1                                      # LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[0]|0x1                     # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[1]|0x1                     # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[2]|0x1                     # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[3]|0x1                     # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[4]|0x1                     # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[5]|0x1                     # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[6]|0x1                     # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[7]|0x1                     # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[8]|0x1                     # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[9]|0x1                     # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[10]|0x1                    # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMode[11]|0x1                    # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[0]|0x2               # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[1]|0x2               # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[2]|0x2               # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[3]|0x2               # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[4]|0x2               # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[5]|0x2               # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[6]|0x2               # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[7]|0x2               # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[8]|0x2               # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[9]|0x2               # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[10]|0x2              # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideMultiplier[11]|0x2              # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[0]|0x41                   # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[1]|0x41                   # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[2]|0x41                   # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[3]|0x41                   # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[4]|0x41                   # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[5]|0x41                   # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[6]|0x41                   # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[7]|0x41                   # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[8]|0x41                   # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[9]|0x41                   # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[10]|0x41                  # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieNonSnoopLatencyOverrideValue[11]|0x41                  # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[0]|0x1                        # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[1]|0x1                        # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[2]|0x1                        # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[3]|0x1                        # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[4]|0x1                        # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[5]|0x1                        # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[6]|0x1                        # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[7]|0x1                        # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[8]|0x1                        # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[9]|0x1                        # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[10]|0x1                       # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMode[11]|0x1                       # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[0]|0x2                  # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[1]|0x2                  # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[2]|0x2                  # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[3]|0x2                  # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[4]|0x2                  # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[5]|0x2                  # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[6]|0x2                  # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[7]|0x2                  # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[8]|0x2                  # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[9]|0x2                  # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[10]|0x2                 # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideMultiplier[11]|0x2                 # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[0]|0x41                      # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[1]|0x41                      # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[2]|0x41                      # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[3]|0x41                      # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[4]|0x41                      # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[5]|0x41                      # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[6]|0x41                      # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[7]|0x41                      # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[8]|0x41                      # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[9]|0x41                      # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[10]|0x41                     # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieSnoopLatencyOverrideValue[11]|0x41                     # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPmErDebugMode|0x1                                          # Energy Reporting Debug Mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPmcOsIdleModeEnable|0x1                                    # OS IDLE Mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PchRtcMemoryLock|0x1                                          # RTC Memory Lock
gStructPcdTokenSpaceGuid.PcdPchSetup.PchS0ixAutoDemotion|0x1                                       # S0ix Auto Demotion
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchScsUfsEnable[0]|0x0                                        # UFS 3.1 Controller 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PchScsUfsInlineEncryption[0]|0x1                              # UFS Inline Encryption
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSerialIoI2c[0]|0x1                                         # I2C0 Controller
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSerialIoI2c[1]|0x1                                         # I2C1 Controller
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSerialIoI2c[2]|0x1                                         # I2C2 Controller
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSerialIoI2c[3]|0x1                                         # I2C3 Controller
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSerialIoI2c[4]|0x1                                         # I2C4 Controller
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSerialIoI2c[5]|0x1                                         # I2C5 Controller
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSerialIoI3c[0]|0x0                                         # I3C0 Controller
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSerialIoI3c[1]|0x0                                         # I3C1 Controller-Gpio
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSerialIoI3c[2]|0x0                                         # I3C2 Controller
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSerialIoSpi[0]|0x0                                         # SPI0 Controller
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSerialIoSpi[1]|0x0                                         # SPI1 Controller
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSerialIoSpi[2]|0x0                                         # SPI2 Controller
!if $(TARGET) == "DEBUG"
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSerialIoUart[0]|0x2                                        # UART0 Controller
!else
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSerialIoUart[0]|0x0                                        # UART0 Controller
!endif
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSerialIoUart[1]|0x0                                        # UART1 Controller
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSerialIoUart[2]|0x0                                        # UART2 Controller
gStructPcdTokenSpaceGuid.PcdPchSetup.SerialIoDebugUartPowerGating|0x0                              # LPSS UART Debug Power Gating
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSidebandLock|0x1                                           # Lock PCH Sideband Access
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSpiCsPolarity[0]|0x1                                       # ChipSelect 0 polarity
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSpiCsPolarity[1]|0x1                                       # ChipSelect 1 polarity
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSpiCsPolarity[2]|0x1                                       # ChipSelect 0 polarity
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSpiCsPolarity[3]|0x1                                       # ChipSelect 1 polarity
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSpiCsPolarity[4]|0x1                                       # ChipSelect 0 polarity
gStructPcdTokenSpaceGuid.PcdPchSetup.PchSpiCsPolarity[5]|0x1                                       # ChipSelect 1 polarity
gStructPcdTokenSpaceGuid.PcdPchSetup.PchThrmT0Level|0x0                                            # T0 Level
gStructPcdTokenSpaceGuid.PcdPchSetup.PchThrmT1Level|0x0                                            # T1 Level
gStructPcdTokenSpaceGuid.PcdPchSetup.PchThrmT2Level|0x0                                            # T2 Level
gStructPcdTokenSpaceGuid.PcdPchSetup.PchThrmTtEnable|0x0                                           # Thermal Throttling
gStructPcdTokenSpaceGuid.PcdPchSetup.PchThrmTtLock|0x0                                             # Thermal Throttling Lock
gStructPcdTokenSpaceGuid.PcdPchSetup.PchThrmTtState13Enable|0x0                                    # TT State 13
gStructPcdTokenSpaceGuid.PcdPchSetup.PchTraceHubMemReg0Size|0xe                                    # PCH TH Mem Buffer Size 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PchTraceHubMemReg1Size|0xe                                    # PCH TH Mem Buffer Size 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PchTraceHubMode|0x0                                           # PCH Trace Hub Enable Mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PchTtLevelSuggestSet|0x1                                      # PCH Thermal Throttling Level
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUartDmaEnable[0]|0x1                                       # DMA Enable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUartDmaEnable[1]|0x1                                       # DMA Enable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUartDmaEnable[2]|0x1                                       # DMA Enable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUartHwFlowCtrl[0]|0x1                                      # Hardware Flow Control
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUartHwFlowCtrl[1]|0x0                                      # Hardware Flow Control
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUartHwFlowCtrl[2]|0x1                                      # Hardware Flow Control
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUartPowerGating[0]|0x2                                     # Power Gating
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUartPowerGating[1]|0x2                                     # Power Gating
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUartPowerGating[2]|0x2                                     # Power Gating
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[0]|0x1                                           # USB HS Physical Connector #0
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[1]|0x1                                           # USB HS Physical Connector #1
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[2]|0x1                                           # USB HS Physical Connector #2
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[3]|0x1                                           # USB HS Physical Connector #3
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[4]|0x1                                           # USB HS Physical Connector #4
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[5]|0x1                                           # USB HS Physical Connector #5
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[6]|0x1                                           # USB HS Physical Connector #6
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[7]|0x1                                           # USB HS Physical Connector #7
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[8]|0x1                                           # USB HS Physical Connector #8
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[9]|0x1                                           # USB HS Physical Connector #9
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[10]|0x1                                          # USB HS Physical Connector #10
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[11]|0x1                                          # USB HS Physical Connector #11
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[12]|0x1                                          # USB HS Physical Connector #12
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbHsPort[13]|0x1                                          # USB HS Physical Connector #13
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbOverCurrentEnable|0x1                                   # USB Overcurrent
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbPdoProgramming|0x1                                      # USB PDO Programming
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbPortDisable|0x0                                         # USB Port Disable Override
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbSsPort[0]|0x1                                           # USB SS Physical Connector #0
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbSsPort[1]|0x1                                           # USB SS Physical Connector #1
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbSsPort[2]|0x1                                           # USB SS Physical Connector #2
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbSsPort[3]|0x1                                           # USB SS Physical Connector #3
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbSsPort[4]|0x1                                           # USB SS Physical Connector #4
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbSsPort[5]|0x1                                           # USB SS Physical Connector #5
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbSsPort[6]|0x1                                           # USB SS Physical Connector #6
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbSsPort[7]|0x1                                           # USB SS Physical Connector #7
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbSsPort[8]|0x1                                           # USB SS Physical Connector #8
gStructPcdTokenSpaceGuid.PcdPchSetup.PchUsbSsPort[9]|0x1                                           # USB SS Physical Connector #9
gStructPcdTokenSpaceGuid.PcdPchSetup.PchWOLFastSupport|0x0                                         # PCH LAN: Fast Startup wake support
gStructPcdTokenSpaceGuid.PcdPchSetup.PchWakeOnLan|0x1                                              # Wake on LAN Enable
gStructPcdTokenSpaceGuid.PcdPchSetup.PchWakeOnWlan|0x0                                             # Wake on WLAN and BT Enable
!if gSiPkgTokenSpaceGuid.PcdEmbeddedEnable == 0x1
    gStructPcdTokenSpaceGuid.PcdPchSetup.PchXdciSupport|0x1                                        # xDCI Support
!else
    gStructPcdTokenSpaceGuid.PcdPchSetup.PchXdciSupport|0x0                                        # xDCI Support
!endif
gStructPcdTokenSpaceGuid.PcdPchSetup.PchXhciOcLock|0x1                                             # USB Overcurrent Lock
gStructPcdTokenSpaceGuid.PcdPchSetup.PchXhciUaol|0x1                                               # USB Audio Offload
gStructPcdTokenSpaceGuid.PcdPchSetup.Pcie8xhDecodePortIndex|0x1                                    # Port8xh Decode Port#
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieComplianceTestMode|0x0                                    # Compliance Test Mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieDetectTimeoutMs[0]|0x0                                    # Detect Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieDetectTimeoutMs[1]|0x0                                    # Detect Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieDetectTimeoutMs[2]|0x0                                    # Detect Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieDetectTimeoutMs[3]|0x0                                    # Detect Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieDetectTimeoutMs[4]|0x0                                    # Detect Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieDetectTimeoutMs[5]|0x0                                    # Detect Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieDetectTimeoutMs[6]|0x0                                    # Detect Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieDetectTimeoutMs[7]|0x0                                    # Detect Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieDetectTimeoutMs[8]|0x0                                    # Detect Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieDetectTimeoutMs[9]|0x0                                    # Detect Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieDetectTimeoutMs[10]|0x0                                   # Detect Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieDetectTimeoutMs[11]|0x0                                   # Detect Timeout
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieExtraBusRsvd[0]|0x0                                       # Extra Bus Reserved
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieExtraBusRsvd[1]|0x0                                       # Extra Bus Reserved
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieExtraBusRsvd[2]|0x0                                       # Extra Bus Reserved
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieExtraBusRsvd[3]|0x0                                       # Extra Bus Reserved
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieExtraBusRsvd[4]|0x0                                       # Extra Bus Reserved
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieExtraBusRsvd[5]|0x0                                       # Extra Bus Reserved
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieExtraBusRsvd[6]|0x0                                       # Extra Bus Reserved
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieExtraBusRsvd[7]|0x0                                       # Extra Bus Reserved
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieExtraBusRsvd[8]|0x0                                       # Extra Bus Reserved
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieExtraBusRsvd[9]|0x0                                       # Extra Bus Reserved
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieExtraBusRsvd[10]|0x0                                      # Extra Bus Reserved
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieExtraBusRsvd[11]|0x0                                      # Extra Bus Reserved
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieFomsCp[0]|0x0                                             # FOM Scoreboard Control Policy
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieFomsCp[1]|0x0                                             # FOM Scoreboard Control Policy
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieFomsCp[2]|0x0                                             # FOM Scoreboard Control Policy
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieFomsCp[3]|0x0                                             # FOM Scoreboard Control Policy
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieFomsCp[4]|0x0                                             # FOM Scoreboard Control Policy
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieFomsCp[5]|0x0                                             # FOM Scoreboard Control Policy
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieFomsCp[6]|0x0                                             # FOM Scoreboard Control Policy
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieFomsCp[7]|0x0                                             # FOM Scoreboard Control Policy
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieFomsCp[8]|0x0                                             # FOM Scoreboard Control Policy
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieFomsCp[9]|0x0                                             # FOM Scoreboard Control Policy
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieFomsCp[10]|0x0                                            # FOM Scoreboard Control Policy
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieFomsCp[11]|0x0                                            # FOM Scoreboard Control Policy
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqMode[0]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqMode[1]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqMode[2]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqMode[3]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqMode[4]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqMode[5]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqMode[6]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqMode[7]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqMode[8]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqMode[9]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqMode[10]|0x0                                        # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqMode[11]|0x0                                        # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1DpTxPreset[0]|0x7                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1DpTxPreset[1]|0x7                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1DpTxPreset[2]|0x7                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1DpTxPreset[3]|0x7                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1DpTxPreset[4]|0x7                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1DpTxPreset[5]|0x7                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1DpTxPreset[6]|0x7                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1DpTxPreset[7]|0x7                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1DpTxPreset[8]|0x7                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1DpTxPreset[9]|0x7                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1DpTxPreset[10]|0x7                               # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1DpTxPreset[11]|0x7                               # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1UpTxPreset[0]|0x7                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1UpTxPreset[1]|0x7                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1UpTxPreset[2]|0x7                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1UpTxPreset[3]|0x7                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1UpTxPreset[4]|0x7                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1UpTxPreset[5]|0x7                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1UpTxPreset[6]|0x7                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1UpTxPreset[7]|0x7                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1UpTxPreset[8]|0x7                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1UpTxPreset[9]|0x7                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1UpTxPreset[10]|0x7                               # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh1UpTxPreset[11]|0x7                               # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3NoOfPresetOrCoeff[0]|0x5                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3NoOfPresetOrCoeff[1]|0x5                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3NoOfPresetOrCoeff[2]|0x5                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3NoOfPresetOrCoeff[3]|0x5                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3NoOfPresetOrCoeff[4]|0x5                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3NoOfPresetOrCoeff[5]|0x5                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3NoOfPresetOrCoeff[6]|0x5                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3NoOfPresetOrCoeff[7]|0x5                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3NoOfPresetOrCoeff[8]|0x5                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3NoOfPresetOrCoeff[9]|0x5                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3NoOfPresetOrCoeff[10]|0x5                        # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3NoOfPresetOrCoeff[11]|0x5                        # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor0List[0]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor0List[1]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor0List[2]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor0List[3]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor0List[4]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor0List[5]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor0List[6]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor0List[7]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor0List[8]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor0List[9]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor0List[10]|0x0                          # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor0List[11]|0x0                          # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor1List[0]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor1List[1]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor1List[2]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor1List[3]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor1List[4]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor1List[5]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor1List[6]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor1List[7]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor1List[8]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor1List[9]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor1List[10]|0x0                          # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor1List[11]|0x0                          # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor2List[0]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor2List[1]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor2List[2]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor2List[3]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor2List[4]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor2List[5]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor2List[6]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor2List[7]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor2List[8]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor2List[9]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor2List[10]|0x0                          # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor2List[11]|0x0                          # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor3List[0]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor3List[1]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor3List[2]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor3List[3]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor3List[4]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor3List[5]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor3List[6]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor3List[7]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor3List[8]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor3List[9]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor3List[10]|0x0                          # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor3List[11]|0x0                          # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor4List[0]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor4List[1]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor4List[2]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor4List[3]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor4List[4]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor4List[5]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor4List[6]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor4List[7]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor4List[8]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor4List[9]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor4List[10]|0x0                          # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor4List[11]|0x0                          # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor5List[0]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor5List[1]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor5List[2]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor5List[3]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor5List[4]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor5List[5]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor5List[6]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor5List[7]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor5List[8]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor5List[9]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor5List[10]|0x0                          # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor5List[11]|0x0                          # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor6List[0]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor6List[1]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor6List[2]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor6List[3]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor6List[4]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor6List[5]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor6List[6]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor6List[7]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor6List[8]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor6List[9]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor6List[10]|0x0                          # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor6List[11]|0x0                          # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor7List[0]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor7List[1]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor7List[2]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor7List[3]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor7List[4]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor7List[5]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor7List[6]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor7List[7]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor7List[8]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor7List[9]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor7List[10]|0x0                          # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor7List[11]|0x0                          # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor8List[0]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor8List[1]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor8List[2]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor8List[3]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor8List[4]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor8List[5]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor8List[6]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor8List[7]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor8List[8]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor8List[9]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor8List[10]|0x0                          # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor8List[11]|0x0                          # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor9List[0]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor9List[1]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor9List[2]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor9List[3]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor9List[4]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor9List[5]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor9List[6]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor9List[7]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor9List[8]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor9List[9]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor9List[10]|0x0                          # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PostCursor9List[11]|0x0                          # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor0List[0]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor0List[1]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor0List[2]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor0List[3]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor0List[4]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor0List[5]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor0List[6]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor0List[7]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor0List[8]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor0List[9]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor0List[10]|0x0                           # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor0List[11]|0x0                           # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor1List[0]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor1List[1]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor1List[2]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor1List[3]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor1List[4]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor1List[5]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor1List[6]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor1List[7]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor1List[8]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor1List[9]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor1List[10]|0x0                           # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor1List[11]|0x0                           # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor2List[0]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor2List[1]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor2List[2]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor2List[3]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor2List[4]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor2List[5]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor2List[6]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor2List[7]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor2List[8]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor2List[9]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor2List[10]|0x0                           # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor2List[11]|0x0                           # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor3List[0]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor3List[1]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor3List[2]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor3List[3]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor3List[4]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor3List[5]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor3List[6]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor3List[7]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor3List[8]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor3List[9]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor3List[10]|0x0                           # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor3List[11]|0x0                           # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor4List[0]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor4List[1]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor4List[2]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor4List[3]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor4List[4]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor4List[5]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor4List[6]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor4List[7]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor4List[8]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor4List[9]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor4List[10]|0x0                           # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor4List[11]|0x0                           # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor5List[0]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor5List[1]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor5List[2]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor5List[3]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor5List[4]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor5List[5]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor5List[6]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor5List[7]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor5List[8]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor5List[9]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor5List[10]|0x0                           # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor5List[11]|0x0                           # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor6List[0]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor6List[1]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor6List[2]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor6List[3]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor6List[4]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor6List[5]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor6List[6]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor6List[7]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor6List[8]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor6List[9]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor6List[10]|0x0                           # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor6List[11]|0x0                           # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor7List[0]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor7List[1]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor7List[2]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor7List[3]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor7List[4]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor7List[5]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor7List[6]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor7List[7]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor7List[8]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor7List[9]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor7List[10]|0x0                           # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor7List[11]|0x0                           # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor8List[0]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor8List[1]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor8List[2]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor8List[3]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor8List[4]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor8List[5]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor8List[6]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor8List[7]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor8List[8]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor8List[9]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor8List[10]|0x0                           # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor8List[11]|0x0                           # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor9List[0]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor9List[1]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor9List[2]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor9List[3]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor9List[4]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor9List[5]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor9List[6]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor9List[7]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor9List[8]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor9List[9]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor9List[10]|0x0                           # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3PreCursor9List[11]|0x0                           # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset0List[0]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset0List[1]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset0List[2]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset0List[3]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset0List[4]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset0List[5]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset0List[6]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset0List[7]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset0List[8]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset0List[9]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset0List[10]|0x4                              # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset0List[11]|0x4                              # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset10List[0]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset10List[1]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset10List[2]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset10List[3]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset10List[4]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset10List[5]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset10List[6]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset10List[7]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset10List[8]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset10List[9]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset10List[10]|0x0                             # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset10List[11]|0x0                             # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset1List[0]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset1List[1]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset1List[2]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset1List[3]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset1List[4]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset1List[5]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset1List[6]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset1List[7]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset1List[8]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset1List[9]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset1List[10]|0x5                              # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset1List[11]|0x5                              # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset2List[0]|0x6                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset2List[1]|0x6                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset2List[2]|0x6                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset2List[3]|0x6                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset2List[4]|0x6                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset2List[5]|0x6                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset2List[6]|0x6                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset2List[7]|0x6                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset2List[8]|0x6                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset2List[9]|0x6                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset2List[10]|0x6                              # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset2List[11]|0x6                              # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset3List[0]|0x8                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset3List[1]|0x8                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset3List[2]|0x8                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset3List[3]|0x8                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset3List[4]|0x8                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset3List[5]|0x8                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset3List[6]|0x8                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset3List[7]|0x8                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset3List[8]|0x9                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset3List[9]|0x9                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset3List[10]|0x9                              # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset3List[11]|0x9                              # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset4List[0]|0x8                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset4List[1]|0x8                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset4List[2]|0x8                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset4List[3]|0x8                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset4List[4]|0x8                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset4List[5]|0x8                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset4List[6]|0x8                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset4List[7]|0x8                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset4List[8]|0x9                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset4List[9]|0x9                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset4List[10]|0x9                              # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset4List[11]|0x9                              # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset5List[0]|0x0                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset5List[1]|0x0                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset5List[2]|0x0                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset5List[3]|0x0                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset5List[4]|0x0                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset5List[5]|0x0                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset5List[6]|0x0                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset5List[7]|0x0                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset5List[8]|0x0                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset5List[9]|0x0                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset5List[10]|0x0                              # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset5List[11]|0x0                              # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset6List[0]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset6List[1]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset6List[2]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset6List[3]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset6List[4]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset6List[5]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset6List[6]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset6List[7]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset6List[8]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset6List[9]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset6List[10]|0x0                              # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset6List[11]|0x0                              # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset7List[0]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset7List[1]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset7List[2]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset7List[3]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset7List[4]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset7List[5]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset7List[6]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset7List[7]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset7List[8]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset7List[9]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset7List[10]|0x0                              # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset7List[11]|0x0                              # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset8List[0]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset8List[1]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset8List[2]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset8List[3]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset8List[4]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset8List[5]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset8List[6]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset8List[7]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset8List[8]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset8List[9]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset8List[10]|0x0                              # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset8List[11]|0x0                              # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset9List[0]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset9List[1]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset9List[2]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset9List[3]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset9List[4]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset9List[5]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset9List[6]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset9List[7]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset9List[8]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset9List[9]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset9List[10]|0x0                              # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3EqPh3Preset9List[11]|0x0                              # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3PcetTimer[0]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3PcetTimer[1]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3PcetTimer[2]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3PcetTimer[3]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3PcetTimer[4]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3PcetTimer[5]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3PcetTimer[6]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3PcetTimer[7]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3PcetTimer[8]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3PcetTimer[9]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3PcetTimer[10]|0x2                                     # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen3PcetTimer[11]|0x2                                     # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqMode[0]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqMode[1]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqMode[2]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqMode[3]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqMode[4]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqMode[5]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqMode[6]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqMode[7]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqMode[8]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqMode[9]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqMode[10]|0x0                                        # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqMode[11]|0x0                                        # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1DpTxPreset[0]|0x7                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1DpTxPreset[1]|0x7                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1DpTxPreset[2]|0x7                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1DpTxPreset[3]|0x7                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1DpTxPreset[4]|0x7                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1DpTxPreset[5]|0x7                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1DpTxPreset[6]|0x7                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1DpTxPreset[7]|0x7                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1DpTxPreset[8]|0x7                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1DpTxPreset[9]|0x7                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1DpTxPreset[10]|0x7                               # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1DpTxPreset[11]|0x7                               # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1UpTxPreset[0]|0x7                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1UpTxPreset[1]|0x7                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1UpTxPreset[2]|0x7                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1UpTxPreset[3]|0x7                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1UpTxPreset[4]|0x7                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1UpTxPreset[5]|0x7                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1UpTxPreset[6]|0x7                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1UpTxPreset[7]|0x7                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1UpTxPreset[8]|0x7                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1UpTxPreset[9]|0x7                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1UpTxPreset[10]|0x7                               # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh1UpTxPreset[11]|0x7                               # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3NoOfPresetOrCoeff[0]|0x3                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3NoOfPresetOrCoeff[1]|0x3                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3NoOfPresetOrCoeff[2]|0x3                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3NoOfPresetOrCoeff[3]|0x3                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3NoOfPresetOrCoeff[4]|0x3                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3NoOfPresetOrCoeff[5]|0x3                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3NoOfPresetOrCoeff[6]|0x3                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3NoOfPresetOrCoeff[7]|0x3                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3NoOfPresetOrCoeff[8]|0x5                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3NoOfPresetOrCoeff[9]|0x5                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3NoOfPresetOrCoeff[10]|0x5                        # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3NoOfPresetOrCoeff[11]|0x5                        # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor0List[0]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor0List[1]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor0List[2]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor0List[3]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor0List[4]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor0List[5]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor0List[6]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor0List[7]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor0List[8]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor0List[9]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor0List[10]|0x0                          # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor0List[11]|0x0                          # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor1List[0]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor1List[1]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor1List[2]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor1List[3]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor1List[4]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor1List[5]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor1List[6]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor1List[7]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor1List[8]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor1List[9]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor1List[10]|0x0                          # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor1List[11]|0x0                          # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor2List[0]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor2List[1]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor2List[2]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor2List[3]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor2List[4]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor2List[5]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor2List[6]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor2List[7]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor2List[8]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor2List[9]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor2List[10]|0x0                          # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor2List[11]|0x0                          # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor3List[0]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor3List[1]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor3List[2]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor3List[3]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor3List[4]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor3List[5]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor3List[6]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor3List[7]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor3List[8]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor3List[9]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor3List[10]|0x0                          # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor3List[11]|0x0                          # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor4List[0]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor4List[1]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor4List[2]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor4List[3]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor4List[4]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor4List[5]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor4List[6]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor4List[7]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor4List[8]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor4List[9]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor4List[10]|0x0                          # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor4List[11]|0x0                          # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor5List[0]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor5List[1]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor5List[2]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor5List[3]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor5List[4]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor5List[5]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor5List[6]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor5List[7]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor5List[8]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor5List[9]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor5List[10]|0x0                          # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor5List[11]|0x0                          # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor6List[0]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor6List[1]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor6List[2]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor6List[3]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor6List[4]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor6List[5]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor6List[6]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor6List[7]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor6List[8]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor6List[9]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor6List[10]|0x0                          # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor6List[11]|0x0                          # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor7List[0]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor7List[1]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor7List[2]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor7List[3]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor7List[4]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor7List[5]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor7List[6]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor7List[7]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor7List[8]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor7List[9]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor7List[10]|0x0                          # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor7List[11]|0x0                          # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor8List[0]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor8List[1]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor8List[2]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor8List[3]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor8List[4]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor8List[5]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor8List[6]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor8List[7]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor8List[8]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor8List[9]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor8List[10]|0x0                          # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor8List[11]|0x0                          # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor9List[0]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor9List[1]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor9List[2]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor9List[3]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor9List[4]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor9List[5]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor9List[6]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor9List[7]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor9List[8]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor9List[9]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor9List[10]|0x0                          # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PostCursor9List[11]|0x0                          # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor0List[0]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor0List[1]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor0List[2]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor0List[3]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor0List[4]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor0List[5]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor0List[6]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor0List[7]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor0List[8]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor0List[9]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor0List[10]|0x0                           # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor0List[11]|0x0                           # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor1List[0]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor1List[1]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor1List[2]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor1List[3]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor1List[4]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor1List[5]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor1List[6]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor1List[7]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor1List[8]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor1List[9]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor1List[10]|0x0                           # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor1List[11]|0x0                           # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor2List[0]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor2List[1]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor2List[2]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor2List[3]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor2List[4]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor2List[5]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor2List[6]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor2List[7]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor2List[8]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor2List[9]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor2List[10]|0x0                           # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor2List[11]|0x0                           # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor3List[0]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor3List[1]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor3List[2]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor3List[3]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor3List[4]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor3List[5]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor3List[6]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor3List[7]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor3List[8]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor3List[9]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor3List[10]|0x0                           # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor3List[11]|0x0                           # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor4List[0]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor4List[1]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor4List[2]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor4List[3]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor4List[4]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor4List[5]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor4List[6]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor4List[7]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor4List[8]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor4List[9]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor4List[10]|0x0                           # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor4List[11]|0x0                           # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor5List[0]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor5List[1]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor5List[2]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor5List[3]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor5List[4]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor5List[5]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor5List[6]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor5List[7]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor5List[8]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor5List[9]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor5List[10]|0x0                           # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor5List[11]|0x0                           # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor6List[0]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor6List[1]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor6List[2]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor6List[3]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor6List[4]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor6List[5]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor6List[6]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor6List[7]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor6List[8]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor6List[9]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor6List[10]|0x0                           # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor6List[11]|0x0                           # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor7List[0]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor7List[1]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor7List[2]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor7List[3]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor7List[4]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor7List[5]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor7List[6]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor7List[7]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor7List[8]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor7List[9]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor7List[10]|0x0                           # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor7List[11]|0x0                           # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor8List[0]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor8List[1]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor8List[2]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor8List[3]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor8List[4]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor8List[5]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor8List[6]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor8List[7]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor8List[8]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor8List[9]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor8List[10]|0x0                           # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor8List[11]|0x0                           # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor9List[0]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor9List[1]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor9List[2]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor9List[3]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor9List[4]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor9List[5]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor9List[6]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor9List[7]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor9List[8]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor9List[9]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor9List[10]|0x0                           # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3PreCursor9List[11]|0x0                           # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset0List[0]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset0List[1]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset0List[2]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset0List[3]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset0List[4]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset0List[5]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset0List[6]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset0List[7]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset0List[8]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset0List[9]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset0List[10]|0x4                              # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset0List[11]|0x4                              # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset10List[0]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset10List[1]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset10List[2]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset10List[3]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset10List[4]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset10List[5]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset10List[6]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset10List[7]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset10List[8]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset10List[9]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset10List[10]|0x0                             # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset10List[11]|0x0                             # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset1List[0]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset1List[1]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset1List[2]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset1List[3]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset1List[4]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset1List[5]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset1List[6]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset1List[7]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset1List[8]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset1List[9]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset1List[10]|0x5                              # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset1List[11]|0x5                              # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset2List[0]|0x6                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset2List[1]|0x6                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset2List[2]|0x6                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset2List[3]|0x6                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset2List[4]|0x6                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset2List[5]|0x6                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset2List[6]|0x6                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset2List[7]|0x6                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset2List[8]|0x6                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset2List[9]|0x6                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset2List[10]|0x6                              # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset2List[11]|0x6                              # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset3List[0]|0x0                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset3List[1]|0x0                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset3List[2]|0x0                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset3List[3]|0x0                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset3List[4]|0x0                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset3List[5]|0x0                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset3List[6]|0x0                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset3List[7]|0x0                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset3List[8]|0x8                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset3List[9]|0x8                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset3List[10]|0x8                              # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset3List[11]|0x8                              # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset4List[0]|0x0                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset4List[1]|0x0                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset4List[2]|0x0                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset4List[3]|0x0                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset4List[4]|0x0                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset4List[5]|0x0                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset4List[6]|0x0                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset4List[7]|0x0                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset4List[8]|0x9                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset4List[9]|0x9                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset4List[10]|0x9                              # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset4List[11]|0x9                              # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset5List[0]|0x7                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset5List[1]|0x7                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset5List[2]|0x7                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset5List[3]|0x7                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset5List[4]|0x7                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset5List[5]|0x7                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset5List[6]|0x7                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset5List[7]|0x7                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset5List[8]|0x7                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset5List[9]|0x7                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset5List[10]|0x7                              # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset5List[11]|0x7                              # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset6List[0]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset6List[1]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset6List[2]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset6List[3]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset6List[4]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset6List[5]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset6List[6]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset6List[7]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset6List[8]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset6List[9]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset6List[10]|0x0                              # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset6List[11]|0x0                              # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset7List[0]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset7List[1]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset7List[2]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset7List[3]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset7List[4]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset7List[5]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset7List[6]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset7List[7]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset7List[8]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset7List[9]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset7List[10]|0x0                              # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset7List[11]|0x0                              # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset8List[0]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset8List[1]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset8List[2]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset8List[3]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset8List[4]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset8List[5]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset8List[6]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset8List[7]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset8List[8]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset8List[9]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset8List[10]|0x0                              # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset8List[11]|0x0                              # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset9List[0]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset9List[1]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset9List[2]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset9List[3]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset9List[4]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset9List[5]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset9List[6]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset9List[7]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset9List[8]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset9List[9]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset9List[10]|0x0                              # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4EqPh3Preset9List[11]|0x0                              # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4PcetTimer[0]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4PcetTimer[1]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4PcetTimer[2]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4PcetTimer[3]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4PcetTimer[4]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4PcetTimer[5]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4PcetTimer[6]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4PcetTimer[7]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4PcetTimer[8]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4PcetTimer[9]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4PcetTimer[10]|0x2                                     # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen4PcetTimer[11]|0x2                                     # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqMode[0]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqMode[1]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqMode[2]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqMode[3]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqMode[4]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqMode[5]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqMode[6]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqMode[7]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqMode[8]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqMode[9]|0x0                                         # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqMode[10]|0x0                                        # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqMode[11]|0x0                                        # PCIe EQ mode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1DpTxPreset[0]|0x5                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1DpTxPreset[1]|0x5                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1DpTxPreset[2]|0x5                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1DpTxPreset[3]|0x5                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1DpTxPreset[4]|0x5                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1DpTxPreset[5]|0x5                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1DpTxPreset[6]|0x5                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1DpTxPreset[7]|0x5                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1DpTxPreset[8]|0x5                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1DpTxPreset[9]|0x5                                # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1DpTxPreset[10]|0x5                               # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1DpTxPreset[11]|0x5                               # EQ PH1 downstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1UpTxPreset[0]|0x5                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1UpTxPreset[1]|0x5                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1UpTxPreset[2]|0x5                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1UpTxPreset[3]|0x5                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1UpTxPreset[4]|0x5                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1UpTxPreset[5]|0x5                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1UpTxPreset[6]|0x5                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1UpTxPreset[7]|0x5                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1UpTxPreset[8]|0x5                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1UpTxPreset[9]|0x5                                # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1UpTxPreset[10]|0x5                               # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh1UpTxPreset[11]|0x5                               # EQ PH1 upstream port transmitter preset
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3NoOfPresetOrCoeff[0]|0x4                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3NoOfPresetOrCoeff[1]|0x4                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3NoOfPresetOrCoeff[2]|0x4                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3NoOfPresetOrCoeff[3]|0x4                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3NoOfPresetOrCoeff[4]|0x4                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3NoOfPresetOrCoeff[5]|0x4                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3NoOfPresetOrCoeff[6]|0x4                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3NoOfPresetOrCoeff[7]|0x4                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3NoOfPresetOrCoeff[8]|0x4                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3NoOfPresetOrCoeff[9]|0x4                         # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3NoOfPresetOrCoeff[10]|0x4                        # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3NoOfPresetOrCoeff[11]|0x4                        # Number of Preset/Coeff List
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor0List[0]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor0List[1]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor0List[2]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor0List[3]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor0List[4]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor0List[5]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor0List[6]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor0List[7]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor0List[8]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor0List[9]|0x0                           # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor0List[10]|0x0                          # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor0List[11]|0x0                          # Post-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor1List[0]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor1List[1]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor1List[2]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor1List[3]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor1List[4]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor1List[5]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor1List[6]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor1List[7]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor1List[8]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor1List[9]|0x0                           # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor1List[10]|0x0                          # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor1List[11]|0x0                          # Post-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor2List[0]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor2List[1]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor2List[2]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor2List[3]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor2List[4]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor2List[5]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor2List[6]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor2List[7]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor2List[8]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor2List[9]|0x0                           # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor2List[10]|0x0                          # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor2List[11]|0x0                          # Post-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor3List[0]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor3List[1]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor3List[2]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor3List[3]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor3List[4]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor3List[5]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor3List[6]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor3List[7]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor3List[8]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor3List[9]|0x0                           # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor3List[10]|0x0                          # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor3List[11]|0x0                          # Post-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor4List[0]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor4List[1]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor4List[2]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor4List[3]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor4List[4]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor4List[5]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor4List[6]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor4List[7]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor4List[8]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor4List[9]|0x0                           # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor4List[10]|0x0                          # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor4List[11]|0x0                          # Post-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor5List[0]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor5List[1]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor5List[2]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor5List[3]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor5List[4]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor5List[5]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor5List[6]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor5List[7]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor5List[8]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor5List[9]|0x0                           # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor5List[10]|0x0                          # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor5List[11]|0x0                          # Post-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor6List[0]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor6List[1]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor6List[2]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor6List[3]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor6List[4]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor6List[5]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor6List[6]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor6List[7]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor6List[8]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor6List[9]|0x0                           # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor6List[10]|0x0                          # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor6List[11]|0x0                          # Post-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor7List[0]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor7List[1]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor7List[2]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor7List[3]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor7List[4]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor7List[5]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor7List[6]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor7List[7]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor7List[8]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor7List[9]|0x0                           # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor7List[10]|0x0                          # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor7List[11]|0x0                          # Post-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor8List[0]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor8List[1]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor8List[2]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor8List[3]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor8List[4]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor8List[5]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor8List[6]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor8List[7]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor8List[8]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor8List[9]|0x0                           # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor8List[10]|0x0                          # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor8List[11]|0x0                          # Post-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor9List[0]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor9List[1]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor9List[2]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor9List[3]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor9List[4]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor9List[5]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor9List[6]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor9List[7]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor9List[8]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor9List[9]|0x0                           # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor9List[10]|0x0                          # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PostCursor9List[11]|0x0                          # Post-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor0List[0]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor0List[1]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor0List[2]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor0List[3]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor0List[4]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor0List[5]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor0List[6]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor0List[7]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor0List[8]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor0List[9]|0x0                            # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor0List[10]|0x0                           # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor0List[11]|0x0                           # Pre-cursor coefficient 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor1List[0]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor1List[1]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor1List[2]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor1List[3]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor1List[4]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor1List[5]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor1List[6]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor1List[7]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor1List[8]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor1List[9]|0x0                            # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor1List[10]|0x0                           # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor1List[11]|0x0                           # Pre-cursor coefficient 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor2List[0]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor2List[1]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor2List[2]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor2List[3]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor2List[4]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor2List[5]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor2List[6]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor2List[7]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor2List[8]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor2List[9]|0x0                            # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor2List[10]|0x0                           # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor2List[11]|0x0                           # Pre-cursor coefficient 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor3List[0]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor3List[1]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor3List[2]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor3List[3]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor3List[4]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor3List[5]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor3List[6]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor3List[7]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor3List[8]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor3List[9]|0x0                            # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor3List[10]|0x0                           # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor3List[11]|0x0                           # Pre-cursor coefficient 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor4List[0]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor4List[1]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor4List[2]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor4List[3]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor4List[4]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor4List[5]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor4List[6]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor4List[7]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor4List[8]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor4List[9]|0x0                            # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor4List[10]|0x0                           # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor4List[11]|0x0                           # Pre-cursor coefficient 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor5List[0]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor5List[1]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor5List[2]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor5List[3]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor5List[4]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor5List[5]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor5List[6]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor5List[7]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor5List[8]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor5List[9]|0x0                            # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor5List[10]|0x0                           # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor5List[11]|0x0                           # Pre-cursor coefficient 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor6List[0]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor6List[1]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor6List[2]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor6List[3]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor6List[4]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor6List[5]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor6List[6]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor6List[7]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor6List[8]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor6List[9]|0x0                            # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor6List[10]|0x0                           # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor6List[11]|0x0                           # Pre-cursor coefficient 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor7List[0]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor7List[1]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor7List[2]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor7List[3]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor7List[4]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor7List[5]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor7List[6]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor7List[7]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor7List[8]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor7List[9]|0x0                            # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor7List[10]|0x0                           # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor7List[11]|0x0                           # Pre-cursor coefficient 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor8List[0]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor8List[1]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor8List[2]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor8List[3]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor8List[4]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor8List[5]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor8List[6]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor8List[7]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor8List[8]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor8List[9]|0x0                            # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor8List[10]|0x0                           # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor8List[11]|0x0                           # Pre-cursor coefficient 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor9List[0]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor9List[1]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor9List[2]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor9List[3]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor9List[4]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor9List[5]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor9List[6]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor9List[7]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor9List[8]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor9List[9]|0x0                            # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor9List[10]|0x0                           # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3PreCursor9List[11]|0x0                           # Pre-cursor coefficient 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset0List[0]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset0List[1]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset0List[2]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset0List[3]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset0List[4]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset0List[5]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset0List[6]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset0List[7]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset0List[8]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset0List[9]|0x4                               # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset0List[10]|0x4                              # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset0List[11]|0x4                              # Preset 0
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset10List[0]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset10List[1]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset10List[2]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset10List[3]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset10List[4]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset10List[5]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset10List[6]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset10List[7]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset10List[8]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset10List[9]|0x0                              # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset10List[10]|0x0                             # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset10List[11]|0x0                             # Preset 10
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset1List[0]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset1List[1]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset1List[2]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset1List[3]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset1List[4]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset1List[5]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset1List[6]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset1List[7]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset1List[8]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset1List[9]|0x5                               # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset1List[10]|0x5                              # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset1List[11]|0x5                              # Preset 1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset2List[0]|0x7                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset2List[1]|0x7                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset2List[2]|0x7                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset2List[3]|0x7                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset2List[4]|0x7                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset2List[5]|0x7                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset2List[6]|0x7                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset2List[7]|0x7                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset2List[8]|0x7                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset2List[9]|0x7                               # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset2List[10]|0x7                              # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset2List[11]|0x7                              # Preset 2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset3List[0]|0x8                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset3List[1]|0x8                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset3List[2]|0x8                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset3List[3]|0x8                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset3List[4]|0x8                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset3List[5]|0x8                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset3List[6]|0x8                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset3List[7]|0x8                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset3List[8]|0x8                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset3List[9]|0x8                               # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset3List[10]|0x8                              # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset3List[11]|0x8                              # Preset 3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset4List[0]|0x0                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset4List[1]|0x0                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset4List[2]|0x0                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset4List[3]|0x0                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset4List[4]|0x0                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset4List[5]|0x0                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset4List[6]|0x0                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset4List[7]|0x0                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset4List[8]|0x0                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset4List[9]|0x0                               # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset4List[10]|0x0                              # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset4List[11]|0x0                              # Preset 4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset5List[0]|0x0                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset5List[1]|0x0                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset5List[2]|0x0                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset5List[3]|0x0                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset5List[4]|0x0                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset5List[5]|0x0                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset5List[6]|0x0                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset5List[7]|0x0                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset5List[8]|0x0                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset5List[9]|0x0                               # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset5List[10]|0x0                              # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset5List[11]|0x0                              # Preset 5
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset6List[0]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset6List[1]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset6List[2]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset6List[3]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset6List[4]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset6List[5]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset6List[6]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset6List[7]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset6List[8]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset6List[9]|0x0                               # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset6List[10]|0x0                              # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset6List[11]|0x0                              # Preset 6
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset7List[0]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset7List[1]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset7List[2]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset7List[3]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset7List[4]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset7List[5]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset7List[6]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset7List[7]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset7List[8]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset7List[9]|0x0                               # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset7List[10]|0x0                              # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset7List[11]|0x0                              # Preset 7
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset8List[0]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset8List[1]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset8List[2]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset8List[3]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset8List[4]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset8List[5]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset8List[6]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset8List[7]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset8List[8]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset8List[9]|0x0                               # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset8List[10]|0x0                              # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset8List[11]|0x0                              # Preset 8
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset9List[0]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset9List[1]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset9List[2]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset9List[3]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset9List[4]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset9List[5]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset9List[6]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset9List[7]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset9List[8]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset9List[9]|0x0                               # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset9List[10]|0x0                              # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5EqPh3Preset9List[11]|0x0                              # Preset 9
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5PcetTimer[0]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5PcetTimer[1]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5PcetTimer[2]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5PcetTimer[3]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5PcetTimer[4]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5PcetTimer[5]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5PcetTimer[6]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5PcetTimer[7]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5PcetTimer[8]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5PcetTimer[9]|0x2                                      # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5PcetTimer[10]|0x2                                     # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieGen5PcetTimer[11]|0x2                                     # PCET Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieIoRsvd[0]|0x4                                             # Reserved I/O
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieIoRsvd[1]|0x4                                             # Reserved I/O
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieIoRsvd[2]|0x4                                             # Reserved I/O
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieIoRsvd[3]|0x4                                             # Reserved I/O
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieIoRsvd[4]|0x4                                             # Reserved I/O
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieIoRsvd[5]|0x4                                             # Reserved I/O
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieIoRsvd[6]|0x4                                             # Reserved I/O
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieIoRsvd[7]|0x4                                             # Reserved I/O
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieIoRsvd[8]|0x4                                             # Reserved I/O
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieIoRsvd[9]|0x4                                             # Reserved I/O
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieIoRsvd[10]|0x4                                            # Reserved I/O
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieIoRsvd[11]|0x4                                            # Reserved I/O
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieMemRsvd[0]|0xa                                            # Reserved Memory
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieMemRsvd[1]|0xa                                            # Reserved Memory
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieMemRsvd[2]|0xa                                            # Reserved Memory
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieMemRsvd[3]|0xa                                            # Reserved Memory
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieMemRsvd[4]|0xa                                            # Reserved Memory
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieMemRsvd[5]|0xa                                            # Reserved Memory
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieMemRsvd[6]|0xa                                            # Reserved Memory
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieMemRsvd[7]|0xa                                            # Reserved Memory
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieMemRsvd[8]|0xa                                            # Reserved Memory
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieMemRsvd[9]|0xa                                            # Reserved Memory
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieMemRsvd[10]|0xa                                           # Reserved Memory
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieMemRsvd[11]|0xa                                           # Reserved Memory
gStructPcdTokenSpaceGuid.PcdPchSetup.PciePhyAutoPowerGating|0x1                                    # Phy Power Gating
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieResizableBarSupport|0x2                                   # PCIE Resizable BAR Support
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPort8xhDecode|0x0                                     # Port8xh Decode
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[0]|0x1                                        # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[1]|0x1                                        # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[2]|0x1                                        # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[3]|0x1                                        # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[4]|0x1                                        # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[5]|0x1                                        # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[6]|0x1                                        # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[7]|0x1                                        # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[8]|0x1                                        # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[9]|0x1                                        # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[10]|0x1                                       # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortACS[11]|0x1                                       # ACS
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[0]|0x4                                       # ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[1]|0x4                                       # ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[2]|0x4                                       # ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[3]|0x4                                       # ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[4]|0x4                                       # ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[5]|0x4                                       # ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[6]|0x4                                       # ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[7]|0x4                                       # ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[8]|0x4                                       # ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[9]|0x4                                       # ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[10]|0x4                                      # ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAspm[11]|0x4                                      # ASPM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[0]|0x1                                        # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[1]|0x1                                        # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[2]|0x1                                        # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[3]|0x1                                        # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[4]|0x1                                        # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[5]|0x1                                        # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[6]|0x1                                        # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[7]|0x1                                        # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[8]|0x1                                        # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[9]|0x1                                        # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[10]|0x1                                       # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortCEE[11]|0x1                                       # CER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[0]|0x1                                         # PCI Express Root Port PXPA1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[1]|0x1                                         # PCI Express Root Port PXPA2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[2]|0x1                                         # PCI Express Root Port PXPA3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[3]|0x1                                         # PCI Express Root Port PXPA4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[4]|0x1                                         # PCI Express Root Port PXPB1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[5]|0x1                                         # PCI Express Root Port PXPB2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[6]|0x1                                         # PCI Express Root Port PXPB3
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[7]|0x1                                         # PCI Express Root Port PXPB4
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[8]|0x1                                         # PCI Express Root Port PXPC1
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[9]|0x1                                         # PCI Express Root Port PXPC2
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[10]|0x1                                        # PCI Express Root Port PXPD
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortEn[11]|0x1                                        # PCI Express Root Port PXPE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[0]|0x1                                        # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[1]|0x1                                        # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[2]|0x1                                        # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[3]|0x1                                        # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[4]|0x1                                        # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[5]|0x1                                        # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[6]|0x1                                        # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[7]|0x1                                        # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[8]|0x1                                        # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[9]|0x1                                        # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[10]|0x1                                       # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortFEE[11]|0x1                                       # FER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[0]|0x0                                        # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[1]|0x0                                        # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[2]|0x0                                        # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[3]|0x0                                        # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[4]|0x0                                        # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[5]|0x0                                        # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[6]|0x0                                        # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[7]|0x0                                        # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[8]|0x0                                        # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[9]|0x0                                        # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[10]|0x0                                       # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortHPE[11]|0x0                                       # Hot Plug
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[0]|0x2                                # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[1]|0x2                                # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[2]|0x2                                # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[3]|0x2                                # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[4]|0x2                                # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[5]|0x2                                # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[6]|0x2                                # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[7]|0x2                                # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[8]|0x2                                # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[9]|0x2                                # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[10]|0x2                               # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortL1SubStates[11]|0x2                               # L1 Substates
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortLinkDownGpios[0]|0x1                              # Assertion on Link Down GPIOs
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortLinkDownGpios[1]|0x1                              # Assertion on Link Down GPIOs
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortLinkDownGpios[2]|0x1                              # Assertion on Link Down GPIOs
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortLinkDownGpios[3]|0x1                              # Assertion on Link Down GPIOs
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortLinkDownGpios[4]|0x1                              # Assertion on Link Down GPIOs
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortLinkDownGpios[5]|0x1                              # Assertion on Link Down GPIOs
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortLinkDownGpios[6]|0x1                              # Assertion on Link Down GPIOs
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortLinkDownGpios[7]|0x1                              # Assertion on Link Down GPIOs
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortLinkDownGpios[8]|0x1                              # Assertion on Link Down GPIOs
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortLinkDownGpios[9]|0x1                              # Assertion on Link Down GPIOs
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortLinkDownGpios[10]|0x1                             # Assertion on Link Down GPIOs
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortLinkDownGpios[11]|0x1                             # Assertion on Link Down GPIOs
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[0]|0x1                                        # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[1]|0x1                                        # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[2]|0x1                                        # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[3]|0x1                                        # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[4]|0x1                                        # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[5]|0x1                                        # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[6]|0x1                                        # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[7]|0x1                                        # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[8]|0x1                                        # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[9]|0x1                                        # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[10]|0x1                                       # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortNFE[11]|0x1                                       # NFER
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[0]|0x0                                       # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[1]|0x0                                       # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[2]|0x0                                       # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[3]|0x0                                       # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[4]|0x0                                       # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[5]|0x0                                       # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[6]|0x0                                       # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[7]|0x0                                       # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[8]|0x0                                       # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[9]|0x0                                       # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[10]|0x0                                      # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPMCE[11]|0x0                                      # PME SCI
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPTM[0]|0x1                                        # PTM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPTM[1]|0x1                                        # PTM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPTM[2]|0x1                                        # PTM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPTM[3]|0x1                                        # PTM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPTM[4]|0x1                                        # PTM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPTM[5]|0x1                                        # PTM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPTM[6]|0x1                                        # PTM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPTM[7]|0x1                                        # PTM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPTM[8]|0x1                                        # PTM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPTM[9]|0x1                                        # PTM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPTM[10]|0x1                                       # PTM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPTM[11]|0x1                                       # PTM
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPeerMemoryWriteEnable[0]|0x0                      # P2P Support
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPeerMemoryWriteEnable[1]|0x0                      # P2P Support
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPeerMemoryWriteEnable[2]|0x0                      # P2P Support
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPeerMemoryWriteEnable[3]|0x0                      # P2P Support
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPeerMemoryWriteEnable[4]|0x0                      # P2P Support
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPeerMemoryWriteEnable[5]|0x0                      # P2P Support
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPeerMemoryWriteEnable[6]|0x0                      # P2P Support
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPeerMemoryWriteEnable[7]|0x0                      # P2P Support
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPeerMemoryWriteEnable[8]|0x0                      # P2P Support
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPeerMemoryWriteEnable[9]|0x0                      # P2P Support
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPeerMemoryWriteEnable[10]|0x0                     # P2P Support
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortPeerMemoryWriteEnable[11]|0x0                     # P2P Support
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[0]|0x0                                        # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[1]|0x0                                        # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[2]|0x0                                        # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[3]|0x0                                        # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[4]|0x0                                        # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[5]|0x0                                        # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[6]|0x0                                        # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[7]|0x0                                        # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[8]|0x0                                        # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[9]|0x0                                        # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[10]|0x0                                       # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSCE[11]|0x0                                       # SECE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[0]|0x0                                        # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[1]|0x0                                        # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[2]|0x0                                        # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[3]|0x0                                        # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[4]|0x0                                        # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[5]|0x0                                        # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[6]|0x0                                        # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[7]|0x0                                        # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[8]|0x0                                        # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[9]|0x0                                        # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[10]|0x0                                       # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSFE[11]|0x0                                       # SEFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSI[0]|0x1                                         # Connection Type
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSI[1]|0x1                                         # Connection Type
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSI[2]|0x1                                         # Connection Type
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSI[3]|0x1                                         # Connection Type
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSI[4]|0x1                                         # Connection Type
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSI[5]|0x1                                         # Connection Type
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSI[6]|0x1                                         # Connection Type
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSI[7]|0x1                                         # Connection Type
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSI[8]|0x1                                         # Connection Type
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSI[9]|0x1                                         # Connection Type
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSI[10]|0x1                                        # Connection Type
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSI[11]|0x1                                        # Connection Type
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[0]|0x0                                        # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[1]|0x0                                        # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[2]|0x0                                        # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[3]|0x0                                        # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[4]|0x0                                        # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[5]|0x0                                        # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[6]|0x0                                        # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[7]|0x0                                        # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[8]|0x0                                        # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[9]|0x0                                        # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[10]|0x0                                       # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSNE[11]|0x0                                       # SENFE
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[0]|0x1                                        # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[1]|0x1                                        # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[2]|0x1                                        # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[3]|0x1                                        # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[4]|0x1                                        # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[5]|0x1                                        # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[6]|0x1                                        # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[7]|0x1                                        # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[8]|0x1                                        # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[9]|0x1                                        # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[10]|0x1                                       # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortAER[11]|0x1                                       # Advanced Error Reporting
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[0]|0x0                                      # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[1]|0x0                                      # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[2]|0x0                                      # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[3]|0x0                                      # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[4]|0x0                                      # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[5]|0x0                                      # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[6]|0x0                                      # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[7]|0x0                                      # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[8]|0x0                                      # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[9]|0x0                                      # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[10]|0x0                                     # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortSpeed[11]|0x0                                     # PCIe Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrOverrideSpecCompliant[0]|0x0                        # PCIe LtrOverride
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrOverrideSpecCompliant[1]|0x0                        # PCIe LtrOverride
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrOverrideSpecCompliant[2]|0x0                        # PCIe LtrOverride
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrOverrideSpecCompliant[3]|0x0                        # PCIe LtrOverride
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrOverrideSpecCompliant[4]|0x0                        # PCIe LtrOverride
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrOverrideSpecCompliant[5]|0x0                        # PCIe LtrOverride
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrOverrideSpecCompliant[6]|0x0                        # PCIe LtrOverride
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrOverrideSpecCompliant[7]|0x0                        # PCIe LtrOverride
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrOverrideSpecCompliant[8]|0x0                        # PCIe LtrOverride
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrOverrideSpecCompliant[9]|0x0                        # PCIe LtrOverride
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrOverrideSpecCompliant[10]|0x0                       # PCIe LtrOverride
gStructPcdTokenSpaceGuid.PcdPchSetup.PchPcieLtrOverrideSpecCompliant[11]|0x0                       # PCIe LtrOverride
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[0]|0x1                                        # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[1]|0x1                                        # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[2]|0x1                                        # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[3]|0x1                                        # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[4]|0x1                                        # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[5]|0x1                                        # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[6]|0x1                                        # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[7]|0x1                                        # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[8]|0x1                                        # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[9]|0x1                                        # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[10]|0x1                                       # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PcieRootPortURE[11]|0x1                                       # URR
gStructPcdTokenSpaceGuid.PcdPchSetup.PmcAdrEn|0x0                                                  # ADR enable
gStructPcdTokenSpaceGuid.PcdPchSetup.PmcWdtTimerEn|0x1                                             # PMC Watchdog Timer
gStructPcdTokenSpaceGuid.PcdPchSetup.PmcAdrHostPartitionReset|0x1                                  # Host Partition Reset ADR Enable
gStructPcdTokenSpaceGuid.PcdPchSetup.PmcAdrMultiplier1Val|0x6                                      # ADR timer 1 time unit
gStructPcdTokenSpaceGuid.PcdPchSetup.PmcAdrTimer1Val|0x20                                          # ADR timer 1 expire time
gStructPcdTokenSpaceGuid.PcdPchSetup.PmcC10DynamicThresholdAdjustment|0x0                          # C10 Dynamic threshold adjustment
gStructPcdTokenSpaceGuid.PcdPchSetup.PmcLpmS0i2p0En|0x1                                            # LPM S0i2.0
gStructPcdTokenSpaceGuid.PcdPchSetup.PmcLpmS0i2p1En|0x1                                            # LPM S0i2.1
gStructPcdTokenSpaceGuid.PcdPchSetup.PmcLpmS0i2p2En|0x1                                            # LPM S0i2.2
gStructPcdTokenSpaceGuid.PcdPchSetup.PmcPrivacyConsent|0x0                                         # Unique_ID Collection Control
gStructPcdTokenSpaceGuid.PcdPchSetup.PmodeClkEn|0x0                                                # Pmode Clock Enable
gStructPcdTokenSpaceGuid.PcdPchSetup.SlpLanLowDc|0x1                                               # SLP_LAN# Low on DC Power
gStructPcdTokenSpaceGuid.PcdPchSetup.SmbusSpdWriteDisable|0x1                                      # SPD Write Disable
gStructPcdTokenSpaceGuid.PcdPchSetup.SocBclkPllOn|0x0                                              # SoC BCLK PLL configuration
gStructPcdTokenSpaceGuid.PcdPchSetup.SocTraceHubMemReg0Size|0xe                                    # SOC TH Mem Buffer Size 0
gStructPcdTokenSpaceGuid.PcdPchSetup.SocTraceHubMemReg1Size|0xe                                    # SOC TH Mem Buffer Size 1
gStructPcdTokenSpaceGuid.PcdPchSetup.SocTraceHubMode|0x0                                           # SOC Trace Hub Enable Mode
gStructPcdTokenSpaceGuid.PcdPchSetup.StateAfterG3|0x0                                              # State After G3
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcActiveLtr[0]|0xffffffff                                    # Active LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcActiveLtr[1]|0xffffffff                                    # Active LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcAssignment[0]|0x0                                          # THC Port Configuration
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcAssignment[1]|0x0                                          # THC Port Configuration
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcClock[0]|0x0                                               # Port Clock
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcClock[1]|0x0                                               # Port Clock
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcDisplayFrameSyncPeriod[0]|0xa                              # Display Frame Sync Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcDisplayFrameSyncPeriod[1]|0xa                              # Display Frame Sync Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cAddressingMode[0]|0x0                                # Addressing Mode
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cAddressingMode[1]|0x0                                # Addressing Mode
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cConnectionSpeed[0]|0x0                               # Requested Connection Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cConnectionSpeed[1]|0x0                               # Requested Connection Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cDeviceAddress[0]|0x0                                 # HIDI2C Device address
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cDeviceAddress[1]|0x0                                 # HIDI2C Device address
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cDeviceDescriptorAddress[0]|0x0                       # Device Descriptor address
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cDeviceDescriptorAddress[1]|0x0                       # Device Descriptor address
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cFastModePlusSerialClockLineHighPeriod[0]|0x0         # FMP SCL HIGH Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cFastModePlusSerialClockLineHighPeriod[1]|0x0         # FMP SCL HIGH Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cFastModePlusSerialClockLineLowPeriod[0]|0x0          # FMP SCL LOW Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cFastModePlusSerialClockLineLowPeriod[1]|0x0          # FMP SCL LOW Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cFastModePlusSerialDataLineReceiveHoldPeriod[0]|0x0   # FMP SDA HOLD RX Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cFastModePlusSerialDataLineReceiveHoldPeriod[1]|0x0   # FMP SDA HOLD RX Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cFastModePlusSerialDataLineTransmitHoldPeriod[0]|0x0  # FMP SDA HOLD TX Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cFastModePlusSerialDataLineTransmitHoldPeriod[1]|0x0  # FMP SDA HOLD TX Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cFastModeSerialClockLineHighPeriod[0]|0x0             # FM SCL HIGH Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cFastModeSerialClockLineHighPeriod[1]|0x0             # FM SCL HIGH Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cFastModeSerialClockLineLowPeriod[0]|0x0              # FM SCL LOW Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cFastModeSerialClockLineLowPeriod[1]|0x0              # FM SCL LOW Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cFastModeSerialDataLineReceiveHoldPeriod[0]|0x0       # FM SDA HOLD RX Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cFastModeSerialDataLineReceiveHoldPeriod[1]|0x0       # FM SDA HOLD RX Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cFastModeSerialDataLineTransmitHoldPeriod[0]|0x0      # FM SDA HOLD TX Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cFastModeSerialDataLineTransmitHoldPeriod[1]|0x0      # FM SDA HOLD TX Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cHighSpeedModePlusSerialClockLineHighPeriod[0]|0x0    # HM SCL HIGH Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cHighSpeedModePlusSerialClockLineHighPeriod[1]|0x0    # HM SCL HIGH Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cHighSpeedModePlusSerialClockLineLowPeriod[0]|0x0     # HM SCL LOW Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cHighSpeedModePlusSerialClockLineLowPeriod[1]|0x0     # HM SCL LOW Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cHighSpeedModePlusSerialDataLineReceiveHoldPeriod[0]|0x0     # HM SDA HOLD RX Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cHighSpeedModePlusSerialDataLineReceiveHoldPeriod[1]|0x0     # HM SDA HOLD RX Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cHighSpeedModePlusSerialDataLineTransmitHoldPeriod[0]|0x0     # HM SDA HOLD TX Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cHighSpeedModePlusSerialDataLineTransmitHoldPeriod[1]|0x0     # HM SDA HOLD TX Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cMaxSuppressedSpikesSMFMFMP[0]|0x0                    # SUPPRESSED SPIKES SM FM FMP
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cMaxSuppressedSpikesSMFMFMP[1]|0x0                    # SUPPRESSED SPIKES SM FM FMP
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cMaximumLengthOfSuppressedSpikesInHighSpeedMode[0]|0x0     # SUPPRESSED SPIKES HM
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cMaximumLengthOfSuppressedSpikesInHighSpeedMode[1]|0x0     # SUPPRESSED SPIKES HM
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cStandardModeSerialClockLineHighPeriod[0]|0x0         # SM SCL HIGH Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cStandardModeSerialClockLineHighPeriod[1]|0x0         # SM SCL HIGH Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cStandardModeSerialClockLineLowPeriod[0]|0x0          # SM SCL LOW Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cStandardModeSerialClockLineLowPeriod[1]|0x0          # SM SCL LOW Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cStandardModeSerialDataLineReceiveHoldPeriod[0]|0x0   # SM SDA HOLD RX Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cStandardModeSerialDataLineReceiveHoldPeriod[1]|0x0   # SM SDA HOLD RX Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cStandardModeSerialDataLineTransmitHoldPeriod[0]|0x0  # SM SDA HOLD TX Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidI2cStandardModeSerialDataLineTransmitHoldPeriod[1]|0x0  # SM SDA HOLD TX Period
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidSpiConnectionSpeed[0]|0x1036640                         # Requested Connection Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidSpiConnectionSpeed[1]|0x1036640                         # Requested Connection Speed
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidSpiFlags[0]|0x0                                         # Flags
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidSpiFlags[1]|0x0                                         # Flags
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidSpiInputReportBodyAddress[0]|0x0                        # Input Report Body Address
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidSpiInputReportBodyAddress[1]|0x0                        # Input Report Body Address
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidSpiInputReportHeaderAddress[0]|0x0                      # Input Report Header Address
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidSpiInputReportHeaderAddress[1]|0x0                      # Input Report Header Address
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidSpiLimitPacketSize[0]|0x0                               # HID Over SPI Limit Packet Size
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidSpiLimitPacketSize[1]|0x0                               # HID Over SPI Limit Packet Size
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidSpiOutputReportAddress[0]|0x0                           # Output Report Address
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidSpiOutputReportAddress[1]|0x0                           # Output Report Address
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidSpiReadOpcode[0]|0x0                                    # Read Opcode
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidSpiReadOpcode[1]|0x0                                    # Read Opcode
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidSpiWriteOpcode[0]|0x0                                   # Write Opcode
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcHidSpiWriteOpcode[1]|0x0                                   # Write Opcode
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcIdleLtr[0]|0xffffffff                                      # Idle LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcIdleLtr[1]|0xffffffff                                      # Idle LTR
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcMode[0]|0x1                                                # THC Mode
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcMode[1]|0x1                                                # THC Mode
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcPerformanceLimitation[0]|0x0                               # Hid Over Spi Performance Limitation
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcPerformanceLimitation[1]|0x0                               # Hid Over Spi Performance Limitation
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcResetPadTrigger[0]|0x0                                     # Reset Pad Trigger
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcResetPadTrigger[1]|0x0                                     # Reset Pad Trigger
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcDsyncPad[0]|0x1                                            # DYSNC input
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcDsyncPad[1]|0x1                                            # DYSNC input
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcTimestampTimerMode[0]|0x0                                  # Timestamp timer behavior in D0i2
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcTimestampTimerMode[1]|0x0                                  # Timestamp timer behavior in D0i2
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcWakeOnTouch[0]|0x0                                         # Wake On Touch
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcWakeOnTouch[1]|0x0                                         # Wake On Touch
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcWotEdgeLevel[0]|0x0                                        # Edge or Level
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcWotEdgeLevel[1]|0x0                                        # Edge or Level
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcWotActiveLevel[0]|0x1                                      # Active Level
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcWotActiveLevel[1]|0x1                                      # Active Level
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcWotPinConfig[0]|0x0                                        # Pin config
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcWotPinConfig[1]|0x0                                        # Pin config
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcCustomizedSsdid[0]|0x7270                                   # Customized SubSytem Device ID
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcCustomizedSsdid[1]|0x7270                                   # Customized SubSytem Device ID
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcCustomizedSsvid[0]|0x8086                                   # Customized SubSytem Vendor ID
gStructPcdTokenSpaceGuid.PcdPchSetup.ThcCustomizedSsvid[1]|0x8086                                   # Customized SubSytem Vendor ID
gStructPcdTokenSpaceGuid.PcdPchSetup.UnlockGpioPads|0x0                                            # Force unlock on all GPIO pads

  gStructPcdTokenSpaceGuid.PcdPchSetup.PchTsnEnable[0]|0x0                                         # PCH TSN LAN Controller1
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchTsnEnable[1]|0x0                                         # PCH TSN LAN Controller2
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchTsnEnable[2]|0x0                                         # PCH TSN LAN Controller3
  gStructPcdTokenSpaceGuid.PcdPchSetup.PchTsnEnable[3]|0x0                                         # PCH TSN LAN Controller4
  gStructPcdTokenSpaceGuid.PcdPchSetup.TsnPcsEnabled|0x0                                           # Enable Timed TSN PCS

gStructPcdTokenSpaceGuid.PcdPlatformEraseVariable.PlatformEraseSimulation|0x0                      # Simulation Mode
gStructPcdTokenSpaceGuid.PcdPlatformEraseVariable.SanitizeOrder|{0x03,0x04,0x02,0x01}              # Erase Method Priority
gStructPcdTokenSpaceGuid.PcdSaSetup.BdatEnable|0x0                                                 # BDAT ACPI Table Support
gStructPcdTokenSpaceGuid.PcdSaSetup.BdatTestType|0x0                                               # BDAT Memory Test Type
gStructPcdTokenSpaceGuid.PcdSaSetup.ConfigureGT|0x1                                                # Configure GT for use
gStructPcdTokenSpaceGuid.PcdSaSetup.ConfigureMedia|0x1                                             # Configure Media for use
gStructPcdTokenSpaceGuid.PcdSaSetup.CridEnable|0x0                                                 # CRID Support
gStructPcdTokenSpaceGuid.PcdSaSetup.TcssD3ColdEnable|0x1                                           # TCSS D3 Cold Enable/Disable
gStructPcdTokenSpaceGuid.PcdSaSetup.DdrOddRatioMode|0x0                                            # QCLK Odd Ratio
gStructPcdTokenSpaceGuid.PcdSaSetup.DdrRefClk|0x0                                                  # Memory Reference Clock
gStructPcdTokenSpaceGuid.PcdSaSetup.DllBwEn0|0x0                                                   # DllBwEn[0]
gStructPcdTokenSpaceGuid.PcdSaSetup.DllBwEn1|0x1                                                   # DllBwEn[1]
gStructPcdTokenSpaceGuid.PcdSaSetup.DllBwEn2|0x2                                                   # DllBwEn[2]
gStructPcdTokenSpaceGuid.PcdSaSetup.DllBwEn3|0x2                                                   # DllBwEn[3]
gStructPcdTokenSpaceGuid.PcdSaSetup.DmaControlGuarantee|0x1                                        # DMA Control Guarantee
gStructPcdTokenSpaceGuid.PcdSaSetup.EnableMediaStandby|0x1                                         # MC6(Media Standby)
gStructPcdTokenSpaceGuid.PcdSaSetup.EnableRenderStandby|0x1                                        # RC6(Render Standby)
gStructPcdTokenSpaceGuid.PcdSaSetup.EnableVtd|0x1                                                  # VT-d
gStructPcdTokenSpaceGuid.PcdSaSetup.GopConfigEnable|0x1                                            # GOP Config Driver Enable
gStructPcdTokenSpaceGuid.PcdSaSetup.IgdDvmt50PreAlloc|0x2                                          # DVMT Pre-Allocated
gStructPcdTokenSpaceGuid.PcdSaSetup.IGpuGsm2Size|0xFF                                              # DVMT Pre-Allocated
gStructPcdTokenSpaceGuid.PcdSaSetup.InternalGraphics|0x2                                           # Internal Graphics
gStructPcdTokenSpaceGuid.PcdSaSetup.LpddrMemWriteLatencySet|0x1                                    # Lpddr Mem WL Set
gStructPcdTokenSpaceGuid.PcdSaSetup.MaxTolud|0x0                                                   # Max TOLUD
gStructPcdTokenSpaceGuid.PcdSaSetup.MemTestOnWarmBoot|0x1                                          # Memory Test on Warm Boot
gStructPcdTokenSpaceGuid.PcdSaSetup.MemoryBandwidthCompression|0x1                                 # Memory Bandwidth Compression
gStructPcdTokenSpaceGuid.PcdSaSetup.PanelPowerEnable|0x1                                           # VDD Enable
gStructPcdTokenSpaceGuid.PcdSaSetup.PeiGraphicsPeimInit|0x1                                        # Intel Graphics Pei Display Peim
gStructPcdTokenSpaceGuid.PcdSaSetup.PllOffsetCal|0x1                                               # PLL Offset Calibration
gStructPcdTokenSpaceGuid.PcdSaSetup.PrimaryDisplay|0x0                                             # Primary Display
gStructPcdTokenSpaceGuid.PcdSaSetup.PtmEnabled[0]|0x1                                              # PTM
gStructPcdTokenSpaceGuid.PcdSaSetup.PtmEnabled[1]|0x1                                              # PTM
gStructPcdTokenSpaceGuid.PcdSaSetup.PtmEnabled[2]|0x1                                              # PTM
gStructPcdTokenSpaceGuid.PcdSaSetup.PtmEnabled[3]|0x1                                              # PTM
gStructPcdTokenSpaceGuid.PcdSaSetup.RC1pGtFreqEnable|0x0                                           # GT RC1p Support
gStructPcdTokenSpaceGuid.PcdSaSetup.RC1pMediaFreqEnable|0x0                                        # Media RC1p Support
gStructPcdTokenSpaceGuid.PcdSaSetup.RaidR0|0x1                                                     # RAID0
gStructPcdTokenSpaceGuid.PcdSaSetup.RaidR1|0x1                                                     # RAID1
gStructPcdTokenSpaceGuid.PcdSaSetup.RaidR10|0x1                                                    # RAID10
gStructPcdTokenSpaceGuid.PcdSaSetup.RaidR5|0x1                                                     # RAID5
gStructPcdTokenSpaceGuid.PcdSaSetup.RealtimeMemoryTiming|0x0                                       # Realtime Memory Timing
gStructPcdTokenSpaceGuid.PcdSaSetup.SaIpuEnable|0x1                                                # IPU Device (B0:D5:F0)
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtForceLtrOverride[0]|0x0                              # Force LTR Override
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtForceLtrOverride[1]|0x0                              # Force LTR Override
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtForceLtrOverride[2]|0x0                              # Force LTR Override
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtForceLtrOverride[3]|0x0                              # Force LTR Override
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtLtrConfigLock[0]|0x0                                 # LTR Lock
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtLtrConfigLock[1]|0x0                                 # LTR Lock
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtLtrConfigLock[2]|0x0                                 # LTR Lock
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtLtrConfigLock[3]|0x0                                 # LTR Lock
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtLtrEnable[0]|0x1                                     # LTR
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtLtrEnable[1]|0x1                                     # LTR
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtLtrEnable[2]|0x1                                     # LTR
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtLtrEnable[3]|0x1                                     # LTR
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtNonSnoopLatencyOverrideMode[0]|0x1                   # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtNonSnoopLatencyOverrideMode[1]|0x1                   # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtNonSnoopLatencyOverrideMode[2]|0x1                   # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtNonSnoopLatencyOverrideMode[3]|0x1                   # Non Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtNonSnoopLatencyOverrideMultiplier[0]|0x2             # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtNonSnoopLatencyOverrideMultiplier[1]|0x2             # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtNonSnoopLatencyOverrideMultiplier[2]|0x2             # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtNonSnoopLatencyOverrideMultiplier[3]|0x2             # Non Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtNonSnoopLatencyOverrideValue[0]|0xc8                 # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtNonSnoopLatencyOverrideValue[1]|0xc8                 # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtNonSnoopLatencyOverrideValue[2]|0xc8                 # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtNonSnoopLatencyOverrideValue[3]|0xc8                 # Non Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtSnoopLatencyOverrideMode[0]|0x1                      # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtSnoopLatencyOverrideMode[1]|0x1                      # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtSnoopLatencyOverrideMode[2]|0x1                      # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtSnoopLatencyOverrideMode[3]|0x1                      # Snoop Latency Override
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtSnoopLatencyOverrideMultiplier[0]|0x2                # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtSnoopLatencyOverrideMultiplier[1]|0x2                # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtSnoopLatencyOverrideMultiplier[2]|0x2                # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtSnoopLatencyOverrideMultiplier[3]|0x2                # Snoop Latency Multiplier
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtSnoopLatencyOverrideValue[0]|0xc8                    # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtSnoopLatencyOverrideValue[1]|0xc8                    # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtSnoopLatencyOverrideValue[2]|0xc8                    # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdSaSetup.SaPcieItbtSnoopLatencyOverrideValue[3]|0xc8                    # Snoop Latency Value
gStructPcdTokenSpaceGuid.PcdSaSetup.SkipExtGfxScan|0x0                                             # Skip Scanning of External Gfx Card
gStructPcdTokenSpaceGuid.PcdSaSetup.SolFeatureEnabled|0x1                                          # Intel Graphics PreMem SOL
gStructPcdTokenSpaceGuid.PcdSaSetup.StrongWkLeaker|0x7                                             # Strong Weak Leaker
gStructPcdTokenSpaceGuid.PcdSaSetup.TcStateLimit|0xa                                               # Tc C-State Limit
gStructPcdTokenSpaceGuid.PcdSaSetup.TcNotifyIgd|0x0                                                # Tc Notify iGFX
gStructPcdTokenSpaceGuid.PcdSaSetup.TcssPortConf[0]|0x7                                            # ITBT PCIE0 Root Port
gStructPcdTokenSpaceGuid.PcdSaSetup.TcssPortConf[1]|0x7                                            # ITBT PCIE1 Root Port
gStructPcdTokenSpaceGuid.PcdSaSetup.TcssPortConf[2]|0x7                                            # ITBT PCIE2 Root Port
gStructPcdTokenSpaceGuid.PcdSaSetup.TcssPortConf[3]|0x7                                            # ITBT PCIE3 Root Port
gStructPcdTokenSpaceGuid.PcdSaSetup.TcssVccstStatus|0x1                                            # VCCST status of IOM
gStructPcdTokenSpaceGuid.PcdSaSetup.TcssXhciEn|0x1                                                 # TCSS xHCI Support
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdEnable|0x0                                                  # Enable VMD controller
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdGlobalMapping|0x0                                           # Enable VMD Global Mapping
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[0]|0x0                                                 # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[1]|0x0                                                 # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[2]|0x0                                                 # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[3]|0x0                                                 # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[4]|0x0                                                 # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[5]|0x0                                                 # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[6]|0x0                                                 # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[7]|0x0                                                 # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[8]|0x0                                                 # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[9]|0x0                                                 # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[10]|0x0                                                # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[11]|0x0                                                # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[12]|0x0                                                # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[13]|0x0                                                # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[14]|0x0                                                # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[15]|0x0                                                # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[16]|0x0                                                # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[17]|0x0                                                # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[18]|0x0                                                # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[19]|0x0                                                # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[20]|0x0                                                # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[21]|0x0                                                # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[22]|0x0                                                # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[23]|0x0                                                # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[24]|0x0                                                # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[25]|0x0                                                # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[26]|0x0                                                # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[27]|0x0                                                # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[28]|0x0                                                # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[29]|0x0                                                # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.VmdPort[30]|0x0                                                # N/A
gStructPcdTokenSpaceGuid.PcdSaSetup.NpuEnable|0x1                                                  # NPU Device (B0:D11:F0)
gStructPcdTokenSpaceGuid.PcdSaSetup.WrDqDqsReTraining|0x1                                          # Write DqDqs Re-Training
gStructPcdTokenSpaceGuid.PcdSaSetup.CnviDdrRfim|0x1                                                # Global DDR RFI Mitigation Control

##########################
# MRC - ExtInputs start
##########################

gStructPcdTokenSpaceGuid.PcdSaSetup.HobBufferSize|0x0                                              # Offset 2 HOB Buffer Size
gStructPcdTokenSpaceGuid.PcdSaSetup.SpdProfileSelected|0x0                                         # Offset 3 Memory profile
gStructPcdTokenSpaceGuid.PcdSaSetup.tCL|0x0                                                        # Offset 4 tCL
gStructPcdTokenSpaceGuid.PcdSaSetup.tRCDtRP|0x0                                                    # Offset 6 tRCD/tRP
gStructPcdTokenSpaceGuid.PcdSaSetup.tRAS|0x0                                                       # Offset 8 tRAS
gStructPcdTokenSpaceGuid.PcdSaSetup.tWR|0x0                                                        # Offset 10 tWR
gStructPcdTokenSpaceGuid.PcdSaSetup.tRFC|0x0                                                       # Offset 12 tRFC
gStructPcdTokenSpaceGuid.PcdSaSetup.tRRD|0x0                                                       # Offset 14 tRRD
gStructPcdTokenSpaceGuid.PcdSaSetup.tWTR|0x0                                                       # Offset 16 tWTR
gStructPcdTokenSpaceGuid.PcdSaSetup.tRTP|0x0                                                       # Offset 18 tRTP
gStructPcdTokenSpaceGuid.PcdSaSetup.tFAW|0x0                                                       # Offset 20 tFAW
gStructPcdTokenSpaceGuid.PcdSaSetup.tCWL|0x0                                                       # Offset 22 tCWL
gStructPcdTokenSpaceGuid.PcdSaSetup.tREFI|0x0                                                      # Offset 24 tREFI
gStructPcdTokenSpaceGuid.PcdSaSetup.MemoryVoltage|0x0                                              # Offset 28 Memory Voltage
gStructPcdTokenSpaceGuid.PcdSaSetup.Vdd2Mv|0x0                                                     # Offset 30 Vdd2Mv
gStructPcdTokenSpaceGuid.PcdSaSetup.EccSupport|0x0                                                 # Offset 32 Bit 0 - ECC Support
gStructPcdTokenSpaceGuid.PcdSaSetup.RemapEnable|0x1                                                #           Bit 2 - Memory Remap
gStructPcdTokenSpaceGuid.PcdSaSetup.ScramblerSupport|0x1                                           #           Bit 3 - Memory Scrambler
gStructPcdTokenSpaceGuid.PcdSaSetup.ProbelessTrace|0x0                                             #           Bit 4 - Probeless Trace
gStructPcdTokenSpaceGuid.PcdSaSetup.PerBankRefresh|0x1                                             #           Bit 5 - Per Bank Refresh
gStructPcdTokenSpaceGuid.PcdSaSetup.ExtendedBankHashing|0x1                                        #           Bit 7 - Extended Bank Hashing
gStructPcdTokenSpaceGuid.PcdSaSetup.RmtPerTask|0x0                                                 #           Bit 10 - Rank Margin Tool Per Task
gStructPcdTokenSpaceGuid.PcdSaSetup.TrainTrace|0x0                                                 #           Bit 11 - Training Tracing
gStructPcdTokenSpaceGuid.PcdSaSetup.DisPgCloseIdleTimeout|0x0                                      #           Bit 14 - Disable Page Close Idle Timeout
gStructPcdTokenSpaceGuid.PcdSaSetup.Ibecc|0x0                                                      #           Bit 15 - In-Band ECC Support
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccParity|0x0                                                #           Bit 16 - In-Band ECC Parity Control
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccOperationMode|0x2                                         #           Bit 17:18 - In-Band ECC Operation Mode
gStructPcdTokenSpaceGuid.PcdSaSetup.ChHashOverride|0x0                                             #           Bit 19 - Channel Hash override
gStructPcdTokenSpaceGuid.PcdSaSetup.RetrainOnFastFail|0x1                                          #           Bit 21 - Restart MRC in Cold mode if SW MemTest fails during Fast flow
gStructPcdTokenSpaceGuid.PcdSaSetup.DvfsqEnabled|0x0                                               #           Bit 22 - DVFSQ
gStructPcdTokenSpaceGuid.PcdSaSetup.DvfscEnabled|0x0                                               #           Bit 23 - E-DVFSC
gStructPcdTokenSpaceGuid.PcdSaSetup.VoltageReadout|0x0                                             #           Bit 24 - Display VCCClk and PBias voltage readings across partitions
gStructPcdTokenSpaceGuid.PcdSaSetup.CccHalfFrequency|0x0                                           #           Bit 25 - CCC Half Frequency
gStructPcdTokenSpaceGuid.PcdSaSetup.Ddr5AutoPrechargeEnable|0x0                                    #           Bit 26:27 - DDR5 Auto Precharge Enable
gStructPcdTokenSpaceGuid.PcdSaSetup.Lp5SplitACTEnable|0x2                                          #           Bit 28:29 - LP5 SplitACT Enable
gStructPcdTokenSpaceGuid.PcdSaSetup.RetrainToWorkingChannel|0x00                                   #           Bit 30 - Disable Failing Channel during Cold Boot and Retrain To Working Channel
gStructPcdTokenSpaceGuid.PcdSaSetup.SubChHashOverride|0x00                                         #           Bit 31 - Select if SubChannel Hash setting values will be taken from input parameters or automatically taken from POR values depending on DRAM type detected.
gStructPcdTokenSpaceGuid.PcdSaSetup.DisableMc0Ch0|0x0                                              # Offset 36 Controller 0, Channel 0 Control
gStructPcdTokenSpaceGuid.PcdSaSetup.DisableMc0Ch1|0x0                                              # Offset 37 Controller 0, Channel 1 Control
gStructPcdTokenSpaceGuid.PcdSaSetup.DisableMc0Ch2|0x0                                              # Offset 38 Controller 0, Channel 2 Control
gStructPcdTokenSpaceGuid.PcdSaSetup.DisableMc0Ch3|0x0                                              # Offset 39 Controller 0, Channel 3 Control
gStructPcdTokenSpaceGuid.PcdSaSetup.DisableMc1Ch0|0x0                                              # Offset 40 Controller 1, Channel 0 Control
gStructPcdTokenSpaceGuid.PcdSaSetup.DisableMc1Ch1|0x0                                              # Offset 41 Controller 1, Channel 1 Control
gStructPcdTokenSpaceGuid.PcdSaSetup.DisableMc1Ch2|0x0                                              # Offset 42 Controller 1, Channel 2 Control
gStructPcdTokenSpaceGuid.PcdSaSetup.DisableMc1Ch3|0x0                                              # Offset 43 Controller 1, Channel 3 Control
gStructPcdTokenSpaceGuid.PcdSaSetup.DdrRatio|0x0                                                   # Offset 44 Memory Ratio
gStructPcdTokenSpaceGuid.PcdSaSetup.ChHashInterleaveBit|0x2                                        # Offset 46 Ch Hash Interleaved Bit

gStructPcdTokenSpaceGuid.PcdSaSetup.ECT|0x1                                                        # Offset 48 BIT0 - Early Command Training
gStructPcdTokenSpaceGuid.PcdSaSetup.SOT|0x0                                                        #           BIT1 - SenseAmp Offset Training
gStructPcdTokenSpaceGuid.PcdSaSetup.ERDMPRTC2D|0x1                                                 #           BIT2 - Early ReadMPR Timing Centering 2D
gStructPcdTokenSpaceGuid.PcdSaSetup.RDMPRT|0x0                                                     #           BIT3 - Read MPR Training
gStructPcdTokenSpaceGuid.PcdSaSetup.RCVET|0x1                                                      #           BIT4 - Receive Enable Training
gStructPcdTokenSpaceGuid.PcdSaSetup.JWRL|0x1                                                       #           BIT5 - Jedec Write Leveling
gStructPcdTokenSpaceGuid.PcdSaSetup.EWRTC2D|0x1                                                    #           BIT6 - Early Write Time Centering 2D
gStructPcdTokenSpaceGuid.PcdSaSetup.ERDTC2D|0x1                                                    #           BIT7 - Early Read Time Centering 2D
gStructPcdTokenSpaceGuid.PcdSaSetup.UNMATCHEDWRTC1D|0x1                                            #           BIT8 - Unmatched Write Timing Centering 1D
gStructPcdTokenSpaceGuid.PcdSaSetup.WRTC1D|0x1                                                     #           BIT9 - Write Timing Centering 1D
gStructPcdTokenSpaceGuid.PcdSaSetup.WRVC1D|0x1                                                     #           BIT10 - Write Voltage Centering 1D
gStructPcdTokenSpaceGuid.PcdSaSetup.RDTC1D|0x1                                                     #           BIT11 - Read Timing Centering 1D
gStructPcdTokenSpaceGuid.PcdSaSetup.RDVC1D|0x1                                                     #           BIT12 - Read Voltage Centering 1D
gStructPcdTokenSpaceGuid.PcdSaSetup.WRTC2D|0x0                                                     #           BIT13 - Write Timing Centering 2D
gStructPcdTokenSpaceGuid.PcdSaSetup.RDTC2D|0x0                                                     #           BIT14 - Read Timing Centering 2D
gStructPcdTokenSpaceGuid.PcdSaSetup.WRVC2D|0x0                                                     #           BIT15 - Write Voltage Centering 2D
gStructPcdTokenSpaceGuid.PcdSaSetup.RDVC2D|0x0                                                     #           BIT16 - Read Voltage Centering 2D
gStructPcdTokenSpaceGuid.PcdSaSetup.WRDSEQT|0x1                                                    #           BIT17 - Write Drive Strength/Equalization 2D*
gStructPcdTokenSpaceGuid.PcdSaSetup.DQSRF|0x1                                                      #           BIT18 - DQS Rise/Fall
gStructPcdTokenSpaceGuid.PcdSaSetup.RDDQSODTT|0x0                                                  #           BIT19 - Read DQS ODT Training
gStructPcdTokenSpaceGuid.PcdSaSetup.RDEQT|0x1                                                      #           BIT20 - Read Equalization Training*
gStructPcdTokenSpaceGuid.PcdSaSetup.DUNITC|0x1                                                     #           BIT21 - Dunit Configuration
gStructPcdTokenSpaceGuid.PcdSaSetup.CMDVC|0x1                                                      #           BIT22 - Command Voltage Centering
gStructPcdTokenSpaceGuid.PcdSaSetup.LCT|0x0                                                        #           BIT23 - Late Command Training
gStructPcdTokenSpaceGuid.PcdSaSetup.RTL|0x0                                                        #           BIT24 - Round Trip Latency
gStructPcdTokenSpaceGuid.PcdSaSetup.TAT|0x1                                                        #           BIT25 - Turn Around Timing Training
gStructPcdTokenSpaceGuid.PcdSaSetup.RMT|0x0                                                        #           BIT26 - Rank Margin Tool
gStructPcdTokenSpaceGuid.PcdSaSetup.RMTEVENODD|0x0                                                 #           BIT27 - Rank Margin Tool Even Odd
gStructPcdTokenSpaceGuid.PcdSaSetup.ALIASCHK|0x0                                                   #           BIT28 - DIMM SPD Alias Test
gStructPcdTokenSpaceGuid.PcdSaSetup.RCVENC1D|0x0                                                   #           BIT29 - Receive Enable Centering 1D
gStructPcdTokenSpaceGuid.PcdSaSetup.RMC|0x0                                                        #           BIT30 - Retrain Margin Check
gStructPcdTokenSpaceGuid.PcdSaSetup.PRETRAIN|0x1                                                   #           BIT31 - PreTraining

gStructPcdTokenSpaceGuid.PcdSaSetup.DCCPICODELUT|0x0                                               # Offset 52 BIT0 - DCC PI Code LUT Calibration
gStructPcdTokenSpaceGuid.PcdSaSetup.DIMMODTT|0x1                                                   #           BIT1 - Dimm ODT Training*
gStructPcdTokenSpaceGuid.PcdSaSetup.DIMMRONT|0x0                                                   #           BIT2 - DIMM RON Training*
gStructPcdTokenSpaceGuid.PcdSaSetup.TXTCO|0x1                                                      #           BIT3 - TxDqTCO Comp Training*
gStructPcdTokenSpaceGuid.PcdSaSetup.CLKTCO|0x0                                                     #           BIT4 - ClkTCO Comp Training*
gStructPcdTokenSpaceGuid.PcdSaSetup.CMDSR|0x0                                                      #           BIT5 - CMD CTL CLK Slew Rate
gStructPcdTokenSpaceGuid.PcdSaSetup.CMDDSEQ|0x0                                                    #           BIT6 - CMD/CTL DS & E 2D
gStructPcdTokenSpaceGuid.PcdSaSetup.DIMMODTCA|0x0                                                  #           BIT7 - DIMM CA ODT Training
gStructPcdTokenSpaceGuid.PcdSaSetup.DDR5ODTTIMING|0x0                                              #           BIT8 - DDR5 ODT Timing Config
gStructPcdTokenSpaceGuid.PcdSaSetup.DBI|0x0                                                        #           BIT9 - DBI
gStructPcdTokenSpaceGuid.PcdSaSetup.DLLDCC|0x1                                                     #           BIT10 - DLL DCC Calibration
gStructPcdTokenSpaceGuid.PcdSaSetup.DLLBWSEL|0x1                                                   #           BIT11 - DLL BW Select Calibration
gStructPcdTokenSpaceGuid.PcdSaSetup.RDVREFDC|0x0                                                   #           BIT12 - Read Vref Decap Training*
gStructPcdTokenSpaceGuid.PcdSaSetup.RMTBIT|0x0                                                     #           BIT14 - Rank Margin Tool Per Bit
gStructPcdTokenSpaceGuid.PcdSaSetup.DQDQSSWZ|0x1                                                   #           BIT15 - Map Dq Dqs Swizzle
gStructPcdTokenSpaceGuid.PcdSaSetup.REFPI|0x1                                                      #           BIT16 - Ref PI Calibration
gStructPcdTokenSpaceGuid.PcdSaSetup.DCCLP5READDCA|0x1                                              #           BIT17 - DRAM DCC for LP5 READ DCA
gStructPcdTokenSpaceGuid.PcdSaSetup.VCCCLKFF|0x0                                                   #           BIT18 - VccCLK FF Offset Correction
gStructPcdTokenSpaceGuid.PcdSaSetup.FUNCDCCDQS|0x0                                                 #           BIT19 - Functional Duty Cycle Correction for DDR5 DQS
gStructPcdTokenSpaceGuid.PcdSaSetup.FUNCDCCCLK|0x0                                                 #           BIT20 - Functional Duty Cycle Correction for DDR5 CLK
gStructPcdTokenSpaceGuid.PcdSaSetup.FUNCDCCWCK|0x0                                                 #           BIT21 - Functional Duty Cycle Correction for LP5 WCK
gStructPcdTokenSpaceGuid.PcdSaSetup.FUNCDCCDQ|0x0                                                  #           BIT22 - Functional Duty Cycle Correction for Data DQ
gStructPcdTokenSpaceGuid.PcdSaSetup.DATAPILIN|0x1                                                  #           BIT23 - Data PI Linearity Calibration
gStructPcdTokenSpaceGuid.PcdSaSetup.DDR5XTALK|0x0                                                  #           BIT24 - Ddr5 Rx Cross-Talk Cancellation
gStructPcdTokenSpaceGuid.PcdSaSetup.DCCLP5WCKDCA|0x1                                               #           BIT25 - DRAM DCC for LP5 WCK DCA
gStructPcdTokenSpaceGuid.PcdSaSetup.RXUNMATCHEDCAL|0x1                                             #           BIT26 - Unmatched Rx Calibaration
gStructPcdTokenSpaceGuid.PcdSaSetup.WRTDIMMDFE|0x1                                                 #           BIT27 - DIMM DFE Training

gStructPcdTokenSpaceGuid.PcdSaSetup.RXDQSDCC|0x1                                                   # Offset 56 BIT0 - RXDQS Duty Cycle Correction
gStructPcdTokenSpaceGuid.PcdSaSetup.DIMMNTODT|0x0                                                  #           BIT1 - DIMM Non-Target ODT training
gStructPcdTokenSpaceGuid.PcdSaSetup.TXDQSDCC |0x1                                                  #           BIT2 - RxVref Per-Bit Training
gStructPcdTokenSpaceGuid.PcdSaSetup.RXVREFPERBIT|0x0                                               #           BIT3 - TXDQS DCC Training
gStructPcdTokenSpaceGuid.PcdSaSetup.PPR|0x0                                                        #           BIT4 - Post Package Repair
gStructPcdTokenSpaceGuid.PcdSaSetup.LVRAUTOTRIM|0x0                                                #           BIT5 - LVR Auto Trim
gStructPcdTokenSpaceGuid.PcdSaSetup.OPTIMIZECOMP|0x0                                               #           BIT7 - Comp optimization
gStructPcdTokenSpaceGuid.PcdSaSetup.WRTRETRAIN|0x1                                                 #           BIT8 - Write Dq/Dqs Retraining
gStructPcdTokenSpaceGuid.PcdSaSetup.TLINECLKCAL|0x1                                                #           BIT13 - Tline Clk Calibration
gStructPcdTokenSpaceGuid.PcdSaSetup.DCCPISERIALCAL|0x1                                             #           BIT14 - DCC PI Serializer Calibration
gStructPcdTokenSpaceGuid.PcdSaSetup.PHASECLKCAL|0x0                                                #           BIT15 - DCC Phase Clk Calibration
gStructPcdTokenSpaceGuid.PcdSaSetup.WCKPADDCCCAL|0x1                                               #           BIT16 - Wck Pad DCC Calibration
gStructPcdTokenSpaceGuid.PcdSaSetup.RDCTLET|0x1                                                    #           BIT17 - Read CTLE Training
gStructPcdTokenSpaceGuid.PcdSaSetup.RDDQODTT|0x1                                                   #           BIT18 - Read DQ ODT Training
gStructPcdTokenSpaceGuid.PcdSaSetup.EMPHASIS|0x1                                                   #           BIT19 - Pre-Emphasis LP5 Training
gStructPcdTokenSpaceGuid.PcdSaSetup.RXDQSVOCC|0x0                                                  #           BIT26 - RxDqsVocCentering1D Training
gStructPcdTokenSpaceGuid.PcdSaSetup.MrcTimeMeasure|0x0                                             # Offset 60 Bit 0 - Time Measure
gStructPcdTokenSpaceGuid.PcdSaSetup.MrcFastBoot|0x1                                                #           Bit 1 - Fast Boot
gStructPcdTokenSpaceGuid.PcdSaSetup.RankInterleave|0x1                                             #           Bit 3 - RI Support
gStructPcdTokenSpaceGuid.PcdSaSetup.EnhancedInterleave|0x1                                         #           Bit 4 - Enh Interleave Support
gStructPcdTokenSpaceGuid.PcdSaSetup.ChHashEnable|0x1                                               #           Bit 6 - Ch Hash Support
gStructPcdTokenSpaceGuid.PcdSaSetup.EnablePwrDn|0x1                                                #           Bit 7 - DDR PowerDown and idle counter
gStructPcdTokenSpaceGuid.PcdSaSetup.EnablePwrDnLpddr|0x1                                           #           Bit 8 - For LPDDR Only: DDR PowerDown and idle counter
gStructPcdTokenSpaceGuid.PcdSaSetup.SrefCfgEna|0x1                                                 #           Bit 9 - SelfRefresh Enable
gStructPcdTokenSpaceGuid.PcdSaSetup.ThrtCkeMinDefeatLpddr|0x1                                      #           Bit 10 - For LPDDR Only: Throttler CKEMin Defeature
gStructPcdTokenSpaceGuid.PcdSaSetup.ThrtCkeMinDefeat|0x1                                           #           Bit 11 - Throttler CKEMin Defeature
gStructPcdTokenSpaceGuid.PcdSaSetup.AllowOppRefBelowWriteThrehold|0x0                              #           Bit 14 - Allow Opp Ref Below Write Threhold
gStructPcdTokenSpaceGuid.PcdSaSetup.ForceSingleRank|0x0                                            #           Bit 16 - Force Single Rank
gStructPcdTokenSpaceGuid.PcdSaSetup.DynamicMemoryBoost|0x0                                         #           Bit 17 - Dynamic Memory Boost
gStructPcdTokenSpaceGuid.PcdSaSetup.RealtimeMemoryFrequency|0x0                                    #           Bit 18 - Realtime Memory Frequency
gStructPcdTokenSpaceGuid.PcdSaSetup.ExitOnFailure|0x1                                              #           Bit 19 - Exit On Failure (MRC)
gStructPcdTokenSpaceGuid.PcdSaSetup.ForceRetrainPath|0x0                                           #           Bit 20 - Force Retrain
gStructPcdTokenSpaceGuid.PcdSaSetup.SaGv|0x1                                                       #           Bit 21 - SAGV
gStructPcdTokenSpaceGuid.PcdSaSetup.IsDdr5MR7WicaSupported|0x0                                     #           Bit 22 - DDR5 DRAM MR7 WICA 0.5 tCK offset alignment support
gStructPcdTokenSpaceGuid.PcdSaSetup.CccPinsInterleaved|0x0                                         #           Bit 25 - Interleaving mode of CCC pins which depends on board routing
gStructPcdTokenSpaceGuid.PcdSaSetup.EnPeriodicComp|0x1                                             #           Bit 26 - Enable/disable Periodic Compensation
gStructPcdTokenSpaceGuid.PcdSaSetup.AsyncOdtDis|0x0                                                #           Bit 27 - Async Odt Support
gStructPcdTokenSpaceGuid.PcdSaSetup.OpportunisticRead|0x1                                          #           Bit 28 - Opportunistic Read
gStructPcdTokenSpaceGuid.PcdSaSetup.Disable2CycleBypass|0x0                                        #           Bit 29 Cycle Bypass Support
gStructPcdTokenSpaceGuid.PcdSaSetup.OCSafeMode|0x1                                                 #           Bit 30 OverClocking Safe Mode
gStructPcdTokenSpaceGuid.PcdSaSetup.SrefCfgIdleTmr|0x640                                           # Offset 64 SelfRefresh IdleTimer
gStructPcdTokenSpaceGuid.PcdSaSetup.ChHashMask|0x830                                               # Offset 66 Ch Hash Mask
gStructPcdTokenSpaceGuid.PcdSaSetup.DdrFreqLimit|0                                                 # Offset 68 Maximum Memory Frequency. 0 - Auto
gStructPcdTokenSpaceGuid.PcdSaSetup.ThrtCkeMinTmr|0x0                                              # Offset 70 Throttler CKEMin Timer
gStructPcdTokenSpaceGuid.PcdSaSetup.ThrtCkeMinTmrLpddr|0x0                                         # Offset 71 For LPDDR Only: Throttler CKEMin Timer
gStructPcdTokenSpaceGuid.PcdSaSetup.SaGvWpMask|0xf                                                 # Offset 72 SA GV Mask
gStructPcdTokenSpaceGuid.PcdSaSetup.NModeSupport|0x0                                               # Offset 73 NMode
gStructPcdTokenSpaceGuid.PcdSaSetup.McRefreshRate|0x0                                              # Offset 74 MC Refresh Rate
gStructPcdTokenSpaceGuid.PcdSaSetup.PowerDownMode|0xff                                             # Offset 75 Power Down Mode
gStructPcdTokenSpaceGuid.PcdSaSetup.PwdwnIdleCounter|0x0                                           # Offset 76 Pwr Down Idle Timer
gStructPcdTokenSpaceGuid.PcdSaSetup.RMTLoopCount|0x0                                               # Offset 84 Rank Margin Tool Loop Count
gStructPcdTokenSpaceGuid.PcdSaSetup.GearRatio|0x0                                                  # Offset 86 Gear Selection for SAGV Disabled. 0 - Auto, 2-Gear 2, 4-Gear 4
gStructPcdTokenSpaceGuid.PcdSaSetup.DdrOneDpc|0x3                                                  # Offset 87 DDR_1DPC
gStructPcdTokenSpaceGuid.PcdSaSetup.VddqVoltageOverride|0x0                                        # Offset 88 Vddq Voltage Override
gStructPcdTokenSpaceGuid.PcdSaSetup.VccIogVoltageOverride|0x0                                      # Offset 90 VccIog Voltage Override
gStructPcdTokenSpaceGuid.PcdSaSetup.VccClkVoltageOverride|0x0                                      # Offset 92 VccClk Voltage Override
gStructPcdTokenSpaceGuid.PcdSaSetup.LpddrRttWr|0x0                                                 # Offset 94 LPDDR ODT RttWr
gStructPcdTokenSpaceGuid.PcdSaSetup.LpddrRttCa|0x0                                                 # Offset 95 LPDDR ODT RttCa
gStructPcdTokenSpaceGuid.PcdSaSetup.SaGvFreq[0]|0                                                  # Offset 96 First Point Frequency. 0 - Auto
gStructPcdTokenSpaceGuid.PcdSaSetup.SaGvFreq[1]|0                                                  # Offset 98 Second Point Frequency. 0 - Auto
gStructPcdTokenSpaceGuid.PcdSaSetup.SaGvFreq[2]|0                                                  # Offset 100 Third Point Frequency. 0 - Auto
gStructPcdTokenSpaceGuid.PcdSaSetup.SaGvFreq[3]|0                                                  # Offset 102 Fourth Point Frequency. 0 - Auto
gStructPcdTokenSpaceGuid.PcdSaSetup.SaGvGear[0]|0x0                                                # Offset 104 First Point Gear, Gear Selection for SAGV points. 0 - Auto, 2-Gear 2, 4-Gear 4
gStructPcdTokenSpaceGuid.PcdSaSetup.SaGvGear[1]|0x0                                                # Offset 105 Second Point Gear, Gear Selection for SAGV points. 0 - Auto, 2-Gear 2, 4-Gear 4
gStructPcdTokenSpaceGuid.PcdSaSetup.SaGvGear[2]|0x0                                                # Offset 106 Third Point Gear, Gear Selection for SAGV points. 0 - Auto, 2-Gear 2, 4-Gear 4
gStructPcdTokenSpaceGuid.PcdSaSetup.SaGvGear[3]|0x0                                                # Offset 107 Fourth Point Gear, Gear Selection for SAGV points. 0 - Auto, 2-Gear 2, 4-Gear 4
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionEnable[0]|0x0                              # Offset 108 IBECC Protect Region 0
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionEnable[1]|0x0                              # Offset 109 IBECC Protect Region 1
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionEnable[2]|0x0                              # Offset 110 IBECC Protect Region 2
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionEnable[3]|0x0                              # Offset 111 IBECC Protect Region 3
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionEnable[4]|0x0                              # Offset 112 IBECC Protect Region 4
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionEnable[5]|0x0                              # Offset 113 IBECC Protect Region 5
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionEnable[6]|0x0                              # Offset 114 IBECC Protect Region 6
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionEnable[7]|0x0                              # Offset 115 IBECC Protect Region 7
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionOverallBase[0]|0x0                         # Protected Region 0 Base (in size of 32MB)
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionOverallBase[1]|0x0                         # Protected Region 1 Base (in size of 32MB)
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionOverallBase[2]|0x0                         # Protected Region 2 Base (in size of 32MB)
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionOverallBase[3]|0x0                         # Protected Region 3 Base (in size of 32MB)
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionOverallBase[4]|0x0                         # Protected Region 4 Base (in size of 32MB)
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionOverallBase[5]|0x0                         # Protected Region 5 Base (in size of 32MB)
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionOverallBase[6]|0x0                         # Protected Region 6 Base (in size of 32MB)
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionOverallBase[7]|0x0                         # Protected Region 7 Base (in size of 32MB)
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionMask[0]|0x3fff                             # Offset 132 Protected Region 0 Size
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionMask[1]|0x3fff                             # Offset 134 Protected Region 1 Size
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionMask[2]|0x3fff                             # Offset 136 Protected Region 2 Size
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionMask[3]|0x3fff                             # Offset 138 Protected Region 3 Size
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionMask[4]|0x3fff                             # Offset 140 Protected Region 4 Size
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionMask[5]|0x3fff                             # Offset 142 Protected Region 5 Size
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionMask[6]|0x3fff                             # Offset 144 Protected Region 6 Size
gStructPcdTokenSpaceGuid.PcdSaSetup.IbeccProtectedRegionMask[7]|0x3fff                             # Offset 146 Protected Region 7 Size

gStructPcdTokenSpaceGuid.PcdSaSetup.MarginLimitL2|0x64                                             # Offset 148 Margin limit check L2 threshold
gStructPcdTokenSpaceGuid.PcdSaSetup.WriteThreshold|0x0                                             # Offset 150 Write Threshold
gStructPcdTokenSpaceGuid.PcdSaSetup.MarginLimitCheck|0x0                                           # Offset 151 Margin limit check enable
gStructPcdTokenSpaceGuid.PcdSaSetup.Lfsr0Mask|0xb                                                  # Offset 152 RH LFSR0 Mask
gStructPcdTokenSpaceGuid.PcdSaSetup.Lfsr1Mask|0xb                                                  # Offset 153 RH LFSR1 Mask
gStructPcdTokenSpaceGuid.PcdSaSetup.RefreshWm|0x1                                                  # Offset 154 Refresh Watermarks
gStructPcdTokenSpaceGuid.PcdSaSetup.DramRfmMode|0x0                                                # Offset 155 Row Hammer Mode: DRAM Refresh Management Mode
gStructPcdTokenSpaceGuid.PcdSaSetup.TargetedRowRefreshMode|0x1                                     # Offset 156 Row Hammer Mode: Targeted Row Refresh Mode
gStructPcdTokenSpaceGuid.PcdSaSetup.DrfmBrc|0x2                                                    # Offset 157 Row Hammer: Directed Refresh Management Blast Radius Configuration
gStructPcdTokenSpaceGuid.PcdSaSetup.SafeModeOverride|0xFF                                          # Offset 159 MRC Safe Mode Override
gStructPcdTokenSpaceGuid.PcdSaSetup.DdrSafeMode|0x4                                                # Offset 160 DDR PHY Safe Mode Support
gStructPcdTokenSpaceGuid.PcdSaSetup.McSafeMode|0x02                                                # Offset 164 MC Safe Mode Support
gStructPcdTokenSpaceGuid.PcdSaSetup.LpMode|0x7                                                     # Offset 165 LpMode bit mask
gStructPcdTokenSpaceGuid.PcdSaSetup.LpMode4|0x1                                                    # Offset 166 LpMode4

gStructPcdTokenSpaceGuid.PcdSaSetup.tRFCpb|0x0                                                     # Offset 168 User defined Memory Timing tRFCpb value,  valid when SpdProfileSelected is CUSTOM_PROFILE
gStructPcdTokenSpaceGuid.PcdSaSetup.tRFC2|0x0                                                      # Offset 170 User defined Memory Timing tRFC2 value,  valid when SpdProfileSelected is CUSTOM_PROFILE
gStructPcdTokenSpaceGuid.PcdSaSetup.tRFC4|0x0                                                      # Offset 172 User defined Memory Timing tRFC4 value,  valid when SpdProfileSelected is CUSTOM_PROFILE
gStructPcdTokenSpaceGuid.PcdSaSetup.tRRD_L|0x0                                                     # Offset 174 User defined Memory Timing tRRD_L value,  valid when SpdProfileSelected is CUSTOM_PROFILE
gStructPcdTokenSpaceGuid.PcdSaSetup.tRRD_S|0x0                                                     # Offset 176 User defined Memory Timing tRRD_S value,  valid when SpdProfileSelected is CUSTOM_PROFILE
gStructPcdTokenSpaceGuid.PcdSaSetup.tWTR_L|0x0                                                     # Offset 178 User defined Memory Timing tWTR_L value,  valid when SpdProfileSelected is CUSTOM_PROFILE
gStructPcdTokenSpaceGuid.PcdSaSetup.tCCD_L|0x0                                                     # Offset 180 User defined Memory Timing tCCD_L value,  valid when SpdProfileSelected is CUSTOM_PROFILE
gStructPcdTokenSpaceGuid.PcdSaSetup.tWTR_S|0x0                                                     # Offset 182 User defined Memory Timing tWTR_S value,  valid when SpdProfileSelected is CUSTOM_PROFILE

gStructPcdTokenSpaceGuid.PcdSaSetup.DimmOdt[0]|0x0                                                 # Offset 184 Dimm Odt values
gStructPcdTokenSpaceGuid.PcdSaSetup.DimmOdt[1]|0x0                                                 # Offset 186 Dimm Odt values
gStructPcdTokenSpaceGuid.PcdSaSetup.DimmOdt[2]|0x0                                                 # Offset 188 Dimm Odt values
gStructPcdTokenSpaceGuid.PcdSaSetup.DimmOdt[3]|0x0                                                 # Offset 190 Dimm Odt values
gStructPcdTokenSpaceGuid.PcdSaSetup.DimmOdt[4]|0x0                                                 # Offset 192 Dimm Odt values
gStructPcdTokenSpaceGuid.PcdSaSetup.DimmOdt[5]|0x0                                                 # Offset 194 Dimm Odt values
gStructPcdTokenSpaceGuid.PcdSaSetup.DimmOdt[6]|0x0                                                 # Offset 196 Dimm Odt values
gStructPcdTokenSpaceGuid.PcdSaSetup.DimmOdt[7]|0x0                                                 # Offset 198 Dimm Odt values
gStructPcdTokenSpaceGuid.PcdSaSetup.DimmOdt[8]|0x0                                                 # Offset 200 Dimm Odt values
gStructPcdTokenSpaceGuid.PcdSaSetup.DimmOdt[9]|0x0                                                 # Offset 202 Dimm Odt values
gStructPcdTokenSpaceGuid.PcdSaSetup.DimmOdt[10]|0x0                                                # Offset 204 Dimm Odt values
gStructPcdTokenSpaceGuid.PcdSaSetup.DimmOdt[11]|0x0                                                # Offset 206 Dimm Odt values
gStructPcdTokenSpaceGuid.PcdSaSetup.DimmOdt[12]|0x0                                                # Offset 208 Dimm Odt values
gStructPcdTokenSpaceGuid.PcdSaSetup.DimmOdt[13]|0x0                                                # Offset 210 Dimm Odt values
gStructPcdTokenSpaceGuid.PcdSaSetup.DimmOdt[14]|0x0                                                # Offset 212 Dimm Odt values
gStructPcdTokenSpaceGuid.PcdSaSetup.DimmOdt[15]|0x0                                                # Offset 214 Dimm Odt values
gStructPcdTokenSpaceGuid.PcdSaSetup.DimmOdt[16]|0x0                                                # Offset 216 Dimm Odt values
gStructPcdTokenSpaceGuid.PcdSaSetup.DimmOdt[17]|0x0                                                # Offset 218 Dimm Odt values
gStructPcdTokenSpaceGuid.PcdSaSetup.DimmOdt[18]|0x0                                                # Offset 220 Dimm Odt values
gStructPcdTokenSpaceGuid.PcdSaSetup.DimmOdt[19]|0x0                                                # Offset 222 Dimm Odt values
gStructPcdTokenSpaceGuid.PcdSaSetup.DimmOdt[20]|0x0                                                # Offset 224 Dimm Odt values
gStructPcdTokenSpaceGuid.PcdSaSetup.DimmOdt[21]|0x0                                                # Offset 226 Dimm Odt values
gStructPcdTokenSpaceGuid.PcdSaSetup.DFETap1StepSize|0x0                                            # Offset 228 Dimm DFE Tap1 Step Size
gStructPcdTokenSpaceGuid.PcdSaSetup.DFETap2StepSize|0x0                                            # Offset 229 Dimm DFE Tap1 Step Size
gStructPcdTokenSpaceGuid.PcdSaSetup.CsVrefLow|0x45                                                 # Offset 230 DDR5 Cs Sweep Low Vref Value
gStructPcdTokenSpaceGuid.PcdSaSetup.CsVrefHigh|0x1D                                                # Offset 231 DDR5 Cs Sweep High Vref Value
gStructPcdTokenSpaceGuid.PcdSaSetup.CaVrefLow|0x45                                                 # Offset 232 DDR5 Ca Sweep Low Vref Value
gStructPcdTokenSpaceGuid.PcdSaSetup.CaVrefHigh|0x1D                                                # Offset 233 DDR5 Ca Sweep High Vref Value
gStructPcdTokenSpaceGuid.PcdSaSetup.EccCorrectionMode|0x0                                          # Offset 234 ECC correction option
gStructPcdTokenSpaceGuid.PcdSaSetup.EccGranularity32BEn|0x0                                        # Offset 235 ECC Granularity
gStructPcdTokenSpaceGuid.PcdSaSetup.CAVrefCtlOffset|0x0                                            # Offset 236 CA Vref Ctl Offset
gStructPcdTokenSpaceGuid.PcdSaSetup.VrefCtlOffset|0x0                                              # Offset 237 DQ Vref Ctrl Offset
gStructPcdTokenSpaceGuid.PcdSaSetup.ClkPiCodeOffset|0x0                                            # Offset 238 Clk PI Code Offset
gStructPcdTokenSpaceGuid.PcdSaSetup.RcvEnOffset|0x0                                                # Offset 239 RcvEn Offset
gStructPcdTokenSpaceGuid.PcdSaSetup.RxDqsOffset|0x0                                                # Offset 240 Rx Dqs Offset
gStructPcdTokenSpaceGuid.PcdSaSetup.TxDqOffset|0x0                                                 # Offset 241 Tx Dq Offset
gStructPcdTokenSpaceGuid.PcdSaSetup.TxDqsOffset|0x0                                                # Offset 242 Tx Dqs Offset
gStructPcdTokenSpaceGuid.PcdSaSetup.VrefOffset|0x0                                                 # Offset 243 Vref Offset
gStructPcdTokenSpaceGuid.PcdSaSetup.CntrlrMask|0x0                                                 # Offset 244 Controller Mask
gStructPcdTokenSpaceGuid.PcdSaSetup.ChMask|0x0                                                     # Offset 245 Channel  Mask
gStructPcdTokenSpaceGuid.PcdSaSetup.LowerBasicMemTestSize|0x0                                      # Offset 248 Reduce BasicMemoryTest size
gStructPcdTokenSpaceGuid.PcdSaSetup.MCREGOFFSET|0x0                                                # Offset 249 MC Register Offset
gStructPcdTokenSpaceGuid.PcdSaSetup.PcuDdrVoltage|0xff                                             # Offset 250 PCU DDR Voltage
gStructPcdTokenSpaceGuid.PcdSaSetup.tRRSG|0                                                        # Offset 251 tRRSG Delta
gStructPcdTokenSpaceGuid.PcdSaSetup.tRRDG|0                                                        # Offset 252 tRRDG Delta
gStructPcdTokenSpaceGuid.PcdSaSetup.tRRDR|0                                                        # Offset 253 tRRDR Delta
gStructPcdTokenSpaceGuid.PcdSaSetup.tRRDD|0                                                        # Offset 254 tRRDD Delta
gStructPcdTokenSpaceGuid.PcdSaSetup.tWRSG|0                                                        # Offset 255 tWRSG Delta
gStructPcdTokenSpaceGuid.PcdSaSetup.tWRDG|0                                                        # Offset 256 tWRDG Delta
gStructPcdTokenSpaceGuid.PcdSaSetup.tWRDR|0                                                        # Offset 257 tWRDR Delta
gStructPcdTokenSpaceGuid.PcdSaSetup.tWRDD|0                                                        # Offset 258 tWRDD Delta
gStructPcdTokenSpaceGuid.PcdSaSetup.tWWSG|0                                                        # Offset 259 tWWSG Delta
gStructPcdTokenSpaceGuid.PcdSaSetup.tWWDG|0                                                        # Offset 260 tWWDG Delta
gStructPcdTokenSpaceGuid.PcdSaSetup.tWWDR|0                                                        # Offset 261 tWWDR Delta
gStructPcdTokenSpaceGuid.PcdSaSetup.tWWDD|0                                                        # Offset 262 tWWDD Delta
gStructPcdTokenSpaceGuid.PcdSaSetup.tRWSG|0                                                        # Offset 263 tRWSG Delta
gStructPcdTokenSpaceGuid.PcdSaSetup.tRWDG|0                                                        # Offset 264 tRWDG Delta
gStructPcdTokenSpaceGuid.PcdSaSetup.tRWDR|0                                                        # Offset 265 tRWDR Delta
gStructPcdTokenSpaceGuid.PcdSaSetup.tRWDD|0                                                        # Offset 266 tRWDD Delta
gStructPcdTokenSpaceGuid.PcdSaSetup.Interpreter|0x0                                                # Offset 267 MRC Interpreter

gStructPcdTokenSpaceGuid.PcdSaSetup.RcompResistor|0                                                # Offset 274 Reference RCOMP resistor on motherboard
gStructPcdTokenSpaceGuid.PcdSaSetup.RcompTarget[0]|0                                               # Offset 276 RCOMP target values for DqOdt, DqDrv, CmdDrv, CtlDrv, ClkDrv
gStructPcdTokenSpaceGuid.PcdSaSetup.RcompTarget[1]|0                                               # Offset 278 RCOMP target values for DqOdt, DqDrv, CmdDrv, CtlDrv, ClkDrv
gStructPcdTokenSpaceGuid.PcdSaSetup.RcompTarget[2]|0                                               # Offset 280 RCOMP target values for DqOdt, DqDrv, CmdDrv, CtlDrv, ClkDrv
gStructPcdTokenSpaceGuid.PcdSaSetup.RcompTarget[3]|0                                               # Offset 282 RCOMP target values for DqOdt, DqDrv, CmdDrv, CtlDrv, ClkDrv
gStructPcdTokenSpaceGuid.PcdSaSetup.RcompTarget[4]|0                                               # Offset 284 RCOMP target values for DqOdt, DqDrv, CmdDrv, CtlDrv, ClkDrv
gStructPcdTokenSpaceGuid.PcdSaSetup.tCCD_L_WR|0x0                                                  # Offset 286 User defined Memory Timing tCCD_L_WR value, valid when SpdProfileSelected is CUSTOM_PROFILE

gStructPcdTokenSpaceGuid.PcdSaSetup.PprRunWCHMATS8|0x1                                             # Run WCHMATS8 in Post Package Repair flow
gStructPcdTokenSpaceGuid.PcdSaSetup.PprRunRetention|0x1                                            # Run Data Retention in Post Package Repair flow
gStructPcdTokenSpaceGuid.PcdSaSetup.PprRunXMarch|0x0                                               # Run XMarch in Post Package Repair flow
gStructPcdTokenSpaceGuid.PcdSaSetup.PprRunXMarchG|0x1                                              # Run XMarchG in Post Package Repair flow
gStructPcdTokenSpaceGuid.PcdSaSetup.PprRunYMarchShort|0x0                                          # Run YMarchShort in Post Package Repair flow
gStructPcdTokenSpaceGuid.PcdSaSetup.PprRunYMarchLong|0x0                                           # Run YMarchLong in Post Package Repair flow
gStructPcdTokenSpaceGuid.PcdSaSetup.PprRepairType|0x0                                              # Offset 288 PPR Repair Type: 0:Do not Repair (Default), 1:Soft Repair, 2:Hard Repair
gStructPcdTokenSpaceGuid.PcdSaSetup.PprRunOnce|0x1                                                 # Offset 289 When enabled, PPR will run only once and then is disabled at next training cycle
gStructPcdTokenSpaceGuid.PcdSaSetup.PprRunAtFastboot|0x0                                           # Offset 290 When enabled, PPR will run during fastboot
gStructPcdTokenSpaceGuid.PcdSaSetup.PprErrorInjection|0x0                                          # Offset 291 When enabled, PPR will inject bad rows during testing
gStructPcdTokenSpaceGuid.PcdSaSetup.PprRepairPhysicalAddress|0x0                                   # Offset 292 PPR repair physical address
gStructPcdTokenSpaceGuid.PcdSaSetup.PprRepairRow|0x0                                               # Offset 300 PPR repair row
gStructPcdTokenSpaceGuid.PcdSaSetup.PprRepairController|0x0                                        # Offset 304 PPR repair controller
gStructPcdTokenSpaceGuid.PcdSaSetup.PprRepairChannel|0x0                                           # Offset 305 PPR repair channel
gStructPcdTokenSpaceGuid.PcdSaSetup.PprRepairDimm|0x0                                              # Offset 306 PPR repair Dimm
gStructPcdTokenSpaceGuid.PcdSaSetup.PprRepairRank|0x0                                              # Offset 307 PPR repair rank
gStructPcdTokenSpaceGuid.PcdSaSetup.PprRepairBankGroup|0x0                                         # Offset 308 PPR repair bank group
gStructPcdTokenSpaceGuid.PcdSaSetup.PprRepairBank|0x0                                              # Offset 309 PPR repair bank
gStructPcdTokenSpaceGuid.PcdSaSetup.PprForceRepair|0x0                                             # Offset 310 When enabled, PPR will issue force repair to the same row many times (90)

gStructPcdTokenSpaceGuid.PcdSaSetup.SubChHashMask|0x838                                            # Offset 316 Channel Hash Mask: 0x0001=BIT6 set(Minimal), 0x3FFF=BIT[19:6] set(Maximum), <b>0x30CE= BIT[19:18, 13:12 ,9:7] set</b>
gStructPcdTokenSpaceGuid.PcdSaSetup.SubChHashInterleaveBit|0x3                                     # Offset 318 SubCh Option to select interleave Address bit. Valid values are 0 - 3 for BITS 6 - 9 (Valid values for BDW are 0-7 for BITS 6 - 13)
gStructPcdTokenSpaceGuid.PcdSaSetup.ForceCkdBypass|0x0                                             # Offset 319 Force CKD in Bypass Mode if CKD DIMM is detected: 0 = Single PLL mode (Default), 1 = Bypass mode.

gStructPcdTokenSpaceGuid.PcdSaSetup.MimicWcDisaplayInIpq|0x0                                       # Offset 320 Bit 3-6    Mimic WC display pattern in IPQ
gStructPcdTokenSpaceGuid.PcdSaSetup.FakeSagv|0                                                     #            Bit 7      Fake SAGV
gStructPcdTokenSpaceGuid.PcdSaSetup.DisableZq|0                                                    #            Bit 8      Disable Zq Calibration
gStructPcdTokenSpaceGuid.PcdSaSetup.ReplicateSagv|0                                                #            Bit 9      Replicate SAGV point0 for 4 points
gStructPcdTokenSpaceGuid.PcdSaSetup.AdjustWckMode|0x3                                              #            Bit 10-11  Adjust Wck Mode
gStructPcdTokenSpaceGuid.PcdSaSetup.TelemetryControl|0x0                                           #            Bit 12-13  MC/PMA telemetry Control
gStructPcdTokenSpaceGuid.PcdSaSetup.SpineAndPhclkGateControl|0x0                                   #            Bit 14     PHclk\Qclk SPINE gating Control
gStructPcdTokenSpaceGuid.PcdSaSetup.SpineGatePerLpmode|0x0                                         #            Bit 15-18  SPINE Gate Control Per Lpmode
gStructPcdTokenSpaceGuid.PcdSaSetup.PhclkGatePerLpmode|0x0                                         #            Bit 19-23  PHCLK Gate Control Per Lpmode
gStructPcdTokenSpaceGuid.PcdSaSetup.DisableSwitchDfiToMc|0x0                                       #            Bit 24     Disable Switch DFI to MC after cold boot
gStructPcdTokenSpaceGuid.PcdSaSetup.RowPressEn|0x0                                                 # Offset 324 Enable/disable ROW PRESS feature
gStructPcdTokenSpaceGuid.PcdSaSetup.WeaklockEn|0x0                                                 # Offset 325 - DLL Weak Lock Support
gStructPcdTokenSpaceGuid.PcdSaSetup.RxDqsDelayCompEn|0x0                                           # Offset 326 - Rx DQS Delay Comp Support

##############################
# MRC - ExtInputs end
##############################

gStructPcdTokenSpaceGuid.PcdSetup.Ac0FanSpeed|0x64                                                 # Active Trip Point 0 Fan Speed
gStructPcdTokenSpaceGuid.PcdSetup.Ac0TripPoint|0x47                                                # Active Trip Point 0
gStructPcdTokenSpaceGuid.PcdSetup.Ac1FanSpeed|0x4b                                                 # Active Trip Point 1 Fan Speed
gStructPcdTokenSpaceGuid.PcdSetup.Ac1TripPoint|0x37                                                # Active Trip Point 1
gStructPcdTokenSpaceGuid.PcdSetup.AcpiAuto|0x0                                                     # Enable ACPI Auto Configuration
gStructPcdTokenSpaceGuid.PcdSetup.AcpiDebug|0x0                                                    # ACPI Debug
gStructPcdTokenSpaceGuid.PcdSetup.AcpiHibernate|0x1                                                # Enable Hibernation
gStructPcdTokenSpaceGuid.PcdSetup.AcpiSleepState|0x1                                               # ACPI S3 Support
gStructPcdTokenSpaceGuid.PcdSetup.AlertEnableLock|0x0                                              # Alert Enable Lock
gStructPcdTokenSpaceGuid.PcdSetup.AuxOriOverrideSupport|0x0                                        # Aux Ori Override Support
gStructPcdTokenSpaceGuid.PcdSetup.AuxOriOverride|0x0                                               # Aux Ori Override
gStructPcdTokenSpaceGuid.PcdSetup.BootFirstToShell|0x0                                             # Enforce Shell as the first boot
gStructPcdTokenSpaceGuid.PcdSetup.CPUEnergyReadEnable|0x1                                          # CPU Energy Read
gStructPcdTokenSpaceGuid.PcdSetup.CPUTempReadEnable|0x1                                            # CPU Temp Read
gStructPcdTokenSpaceGuid.PcdSetup.CSDebugLightEC|0x0                                               # EC CS Debug Light
gStructPcdTokenSpaceGuid.PcdSetup.CSNotifyEC|0x1                                                   # EC Notification
gStructPcdTokenSpaceGuid.PcdSetup.CoExistenceManager|0x0                                           # CoExistence Manager
gStructPcdTokenSpaceGuid.PcdSetup.IuerButtonEnable|0x0                                             # IUER Button Enable
gStructPcdTokenSpaceGuid.PcdSetup.IuerConvertibleEnable|0x0                                        # IUER Slate Enable
gStructPcdTokenSpaceGuid.PcdSetup.SlateIndicatorSx|0x0                                             # Slate Mode on Sx resume (Excluding S5)
gStructPcdTokenSpaceGuid.PcdSetup.IuerDockEnable|0x0                                               # IUER Dock Enable
gStructPcdTokenSpaceGuid.PcdSetup.DockIndicatorSx|0x0                                              # Dock Mode upon Sx resume (Excluding S5)
gStructPcdTokenSpaceGuid.PcdSetup.SlateIndicatorRT|0x1                                             # Slate Mode boot value
gStructPcdTokenSpaceGuid.PcdSetup.DockIndicatorRT|0x0                                              # Dock Mode boot value
!if gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable == TRUE
    gStructPcdTokenSpaceGuid.PcdSetup.ConInBehavior|0x2                                            # Console Input Behavior
!else
    gStructPcdTokenSpaceGuid.PcdSetup.ConInBehavior|0x0                                            # Console Input Behavior
!endif
gStructPcdTokenSpaceGuid.PcdSetup.ControlIommu|0x1                                                 # Pre-boot DMA Protection
gStructPcdTokenSpaceGuid.PcdSetup.CpuFanSpeed|0x41                                                 # CPU Fan Speed
gStructPcdTokenSpaceGuid.PcdSetup.CpuTemp|0x48                                                     # CPU Temp
gStructPcdTokenSpaceGuid.PcdSetup.CriticalThermalTripPoint|0x77                                    # Critical Trip Point
gStructPcdTokenSpaceGuid.PcdSetup.DeepestUSBSleepWakeCapability|0x4                                # USBC and USBA Wake Capability
gStructPcdTokenSpaceGuid.PcdSetup.DevicePasswordSupport|0x1                                        # Device password support
gStructPcdTokenSpaceGuid.PcdSetup.DgPlatformSupport|0x0                                            # DG Platform Support
gStructPcdTokenSpaceGuid.PcdSetup.DimmAlert|0x0                                                    # DIMM Alert
gStructPcdTokenSpaceGuid.PcdSetup.DisableActiveTripPoints|0x1                                      # Active Trip Points
gStructPcdTokenSpaceGuid.PcdSetup.DisableCriticalTripPoints|0x1                                    # Critical Trip Points
gStructPcdTokenSpaceGuid.PcdSetup.DisablePassiveTripPoints|0x0                                     # Passive Trip Points
gStructPcdTokenSpaceGuid.PcdSetup.DisplayBootMode|0x0                                              # Display Boot Mode
gStructPcdTokenSpaceGuid.PcdSetup.EcChargingMethod|0x0                                             # Charging Method
gStructPcdTokenSpaceGuid.PcdSetup.EcLowPowerMode|0x1                                               # EC Low Power Mode
gStructPcdTokenSpaceGuid.PcdSetup.EcPeciMode|0x0                                                   # EC PECI Mode
gStructPcdTokenSpaceGuid.PcdSetup.EfiNetworkSupport|0x0                                            # EFI Network
gStructPcdTokenSpaceGuid.PcdSetup.EnableMsiInFadt|0x1                                              # MSI enabled
gStructPcdTokenSpaceGuid.PcdSetup.EnablePcieTunnelingOverUsb4|0x1                                  # PCIE Tunneling over USB4
!if gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable == TRUE
    gStructPcdTokenSpaceGuid.PcdSetup.FastBoot|0x1                                                 # Fast Boot
!else
    gStructPcdTokenSpaceGuid.PcdSetup.FastBoot|0x0                                                 # Fast Boot
!endif
gStructPcdTokenSpaceGuid.PcdSetup.FastBootWdt|0x0                                                  # Fast Boot WDT
gStructPcdTokenSpaceGuid.PcdSetup.FirmwareConfiguration|0x2                                        # Firmware Configuration
gStructPcdTokenSpaceGuid.PcdSetup.Gpe1BlockEnable|0x1                                              # GPE1 support
gStructPcdTokenSpaceGuid.PcdSetup.GuidForward|0x0                                                  # Guid Forward
gStructPcdTokenSpaceGuid.PcdSetup.HebcValue|0x233f3                                                # HEBC value
gStructPcdTokenSpaceGuid.PcdSetup.HidEventFilterDriverEnable|0x1                                   # HID Event Filter Driver
gStructPcdTokenSpaceGuid.PcdSetup.I2cSarResetDelay|0x8                                             # Delay to wait for WWAN device to be ready before SAR reset.
gStructPcdTokenSpaceGuid.PcdSetup.ITbtConnectTopologyTimeoutInMs|0x1388                            # Connect Topology Timeout value for ITBT
gStructPcdTokenSpaceGuid.PcdSetup.ITbtForcePowerOnTimeoutInMs|0x1f4                                # Force Poweron Timeout value for ITBT
gStructPcdTokenSpaceGuid.PcdSetup.ITbtPcieExtraBusRsvd[0]|0x2a                                     # Extra Bus Reserved
gStructPcdTokenSpaceGuid.PcdSetup.ITbtPcieExtraBusRsvd[1]|0x2a                                     # Extra Bus Reserved
gStructPcdTokenSpaceGuid.PcdSetup.ITbtPcieExtraBusRsvd[2]|0x2a                                     # Extra Bus Reserved
gStructPcdTokenSpaceGuid.PcdSetup.ITbtPcieExtraBusRsvd[3]|0x2a                                     # Extra Bus Reserved
gStructPcdTokenSpaceGuid.PcdSetup.ITbtPcieMemAddrRngMax[0]|0x19                                    # Memory Alignment
gStructPcdTokenSpaceGuid.PcdSetup.ITbtPcieMemAddrRngMax[1]|0x19                                    # Memory Alignment
gStructPcdTokenSpaceGuid.PcdSetup.ITbtPcieMemAddrRngMax[2]|0x19                                    # Memory Alignment
gStructPcdTokenSpaceGuid.PcdSetup.ITbtPcieMemAddrRngMax[3]|0x19                                    # Memory Alignment
gStructPcdTokenSpaceGuid.PcdSetup.ITbtPcieMemRsvd[0]|0x60                                          # Reserved Memory
gStructPcdTokenSpaceGuid.PcdSetup.ITbtPcieMemRsvd[1]|0x60                                          # Reserved Memory
gStructPcdTokenSpaceGuid.PcdSetup.ITbtPcieMemRsvd[2]|0x60                                          # Reserved Memory
gStructPcdTokenSpaceGuid.PcdSetup.ITbtPcieMemRsvd[3]|0x60                                          # Reserved Memory
gStructPcdTokenSpaceGuid.PcdSetup.ITbtPciePMemAddrRngMax[0]|0x1c                                   # PMemory Alignment
gStructPcdTokenSpaceGuid.PcdSetup.ITbtPciePMemAddrRngMax[1]|0x1c                                   # PMemory Alignment
gStructPcdTokenSpaceGuid.PcdSetup.ITbtPciePMemAddrRngMax[2]|0x1c                                   # PMemory Alignment
gStructPcdTokenSpaceGuid.PcdSetup.ITbtPciePMemAddrRngMax[3]|0x1c                                   # PMemory Alignment
gStructPcdTokenSpaceGuid.PcdSetup.ITbtPciePMemRsvd[0]|0x8000                                       # Reserved PMemory
gStructPcdTokenSpaceGuid.PcdSetup.ITbtPciePMemRsvd[1]|0x8000                                       # Reserved PMemory
gStructPcdTokenSpaceGuid.PcdSetup.ITbtPciePMemRsvd[2]|0x8000                                       # Reserved PMemory
gStructPcdTokenSpaceGuid.PcdSetup.ITbtPciePMemRsvd[3]|0x8000                                       # Reserved PMemory
gStructPcdTokenSpaceGuid.PcdSetup.ITbtRtd3|0x1                                                     # ITBT RTD3
gStructPcdTokenSpaceGuid.PcdSetup.ITbtRtd3ExitDelay|0x0                                            # ITBT RTD3 EXIT DELAY
gStructPcdTokenSpaceGuid.PcdSetup.PcieRtd3LinkActiveTimeout|0x0                                    # PCIE RTD3 Polling Link Active Timeout
gStructPcdTokenSpaceGuid.PcdSetup.IntegratedTbtSupport|0x1                                         # Integrated Thunderbolt(TM) Support
gStructPcdTokenSpaceGuid.PcdSetup.InteractiveText|0x0                                              # Thermal Sensor 1 Temp | Thermal Sensor 2 Temp | Thermal Sensor 3 Temp | Thermal Sensor 4 Temp | Thermal Sensor 5 Temp | CPU Fan Speed | PCH DTS Temp from PCH
gStructPcdTokenSpaceGuid.PcdSetup.ItpxdpMux|0x1                                                    # Enable xdpclock
gStructPcdTokenSpaceGuid.PcdSetup.KernelDebugPatch|0x0                                             # Kernel Debug Patch
gStructPcdTokenSpaceGuid.PcdSetup.LazyConIn|0x0                                                    # Enable Lazy ConIn
!if gMipiCamFeaturePkgTokenSpaceGuid.PcdMipiCamFeatureEnable == TRUE
    gStructPcdTokenSpaceGuid.PcdSetup.LchSupport|0x0                                               # LCH Support
    gStructPcdTokenSpaceGuid.PcdSetup.LchTestDevice|0x0                                            # LCH Test Device
!endif
gStructPcdTokenSpaceGuid.PcdSetup.LoadSSDTFromFile|0x0                                             # SSDT table from file
!if gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable == TRUE
    gStructPcdTokenSpaceGuid.PcdSetup.LowPowerS0Idle|0x0                                           # Low Power S0 Idle Capability
!else
    gStructPcdTokenSpaceGuid.PcdSetup.LowPowerS0Idle|0x1                                           # Low Power S0 Idle Capability
!endif
gStructPcdTokenSpaceGuid.PcdSetup.LpitResidencyCounter|0x3                                         # LPIT Residency Counter
gStructPcdTokenSpaceGuid.PcdSetup.MPdtSupport|0x0                                                  # MPDT Support
gStructPcdTokenSpaceGuid.PcdSetup.NativeAspmEnable|0x2                                             # Native ASPM
gStructPcdTokenSpaceGuid.PcdSetup.OrientationSensor|0x0                                            # Orientation Sensor
gStructPcdTokenSpaceGuid.PcdSetup.OsDebugPort|0x0                                                  # Kernel Debug Serial Port
gStructPcdTokenSpaceGuid.PcdSetup.OsNativeResourceBalance|0x0                                      # Os Native Resource Balance
gStructPcdTokenSpaceGuid.PcdSetup.PCHTempReadEnable|0x1                                            # PCH Temp Read
gStructPcdTokenSpaceGuid.PcdSetup.PL1LimitCS|0x0                                                   # CS PL1 Limit
gStructPcdTokenSpaceGuid.PcdSetup.PL1LimitCSValue|0x1194                                           # CS PL1 Value
gStructPcdTokenSpaceGuid.PcdSetup.PassiveTc1Value|0x1                                              # Passive TC1 Value
gStructPcdTokenSpaceGuid.PcdSetup.PassiveTc2Value|0x5                                              # Passive TC2 Value
gStructPcdTokenSpaceGuid.PcdSetup.PassiveThermalTripPoint|0x5f                                     # Passive Trip Point
gStructPcdTokenSpaceGuid.PcdSetup.PassiveTspValue|0xa                                              # Passive TSP Value
gStructPcdTokenSpaceGuid.PcdSetup.PchAlert|0x0                                                     # PCH Alert
gStructPcdTokenSpaceGuid.PcdSetup.PchI2cSensorDevicePort[0]|0x0                                    # Connected device
gStructPcdTokenSpaceGuid.PcdSetup.PchI2cSensorDevicePort[1]|0x0                                    # Connected device
gStructPcdTokenSpaceGuid.PcdSetup.PchI2cSensorDevicePort[2]|0x0                                    # Connected device
gStructPcdTokenSpaceGuid.PcdSetup.PchI2cSensorDevicePort[3]|0x0                                    # Connected device
gStructPcdTokenSpaceGuid.PcdSetup.PchI2cSensorDevicePort[4]|0x0                                    # Connected device
gStructPcdTokenSpaceGuid.PcdSetup.PchI2cSensorDevicePort[5]|0x0                                    # Connected device
gStructPcdTokenSpaceGuid.PcdSetup.PchI2cTouchPadBusAddress|0x0                                     # Device's bus address
gStructPcdTokenSpaceGuid.PcdSetup.PchI2cTouchPadHidAddress|0x0                                     # Device's HID address
gStructPcdTokenSpaceGuid.PcdSetup.PchI2cTouchPadIrqMode|0x1                                        # Touch Pad Interrupt Mode
gStructPcdTokenSpaceGuid.PcdSetup.PchI2cTouchPadSpeed|0x0                                          # Device's bus speed
gStructPcdTokenSpaceGuid.PcdSetup.PchI2cTouchPadType|0x0                                           # Touch Pad
gStructPcdTokenSpaceGuid.PcdSetup.PchI2cTouchPanelBusAddress|0x0                                   # Device's bus address
gStructPcdTokenSpaceGuid.PcdSetup.PchI2cTouchPanelHidAddress|0x0                                   # Device's HID address
gStructPcdTokenSpaceGuid.PcdSetup.PchI2cTouchPanelIrqMode|0x1                                      # Touch Panel Interrupt Mode
gStructPcdTokenSpaceGuid.PcdSetup.PchI2cTouchPanelSpeed|0x1                                        # Device's bus speed
gStructPcdTokenSpaceGuid.PcdSetup.PchI2cTouchPanelType|0x0                                         # Touch Panel
gStructPcdTokenSpaceGuid.PcdSetup.PchI2cWittDevice|0x0                                             # WITT/MITT Test Device
gStructPcdTokenSpaceGuid.PcdSetup.PchI2cWittVersion|0x1                                            # WITT/MITT Device selection
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFMD0|0x1e                                       # FastSpeed SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFMD1|0x1e                                       # FastSpeed SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFMD2|0x1e                                       # FastSpeed SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFMD3|0x1e                                       # FastSpeed SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFMD4|0x1e                                       # FastSpeed SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFMD5|0x1e                                       # FastSpeed SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFMH0|0x6e                                       # FastSpeed SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFMH1|0x6e                                       # FastSpeed SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFMH2|0x6e                                       # FastSpeed SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFMH3|0x6e                                       # FastSpeed SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFMH4|0x6e                                       # FastSpeed SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFMH5|0x6e                                       # FastSpeed SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFML0|0xcf                                       # FastSpeed SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFML1|0xcf                                       # FastSpeed SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFML2|0xcf                                       # FastSpeed SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFML3|0xcf                                       # FastSpeed SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFML4|0xcf                                       # FastSpeed SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFML5|0xcf                                       # FastSpeed SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFPD0|0x1e                                       # FastSpeedPlus SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFPD1|0x1e                                       # FastSpeedPlus SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFPD2|0x1e                                       # FastSpeedPlus SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFPD3|0x1e                                       # FastSpeedPlus SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFPD4|0x1e                                       # FastSpeedPlus SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFPD5|0x1e                                       # FastSpeedPlus SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFPH0|0x2b                                       # FastSpeedPlus SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFPH1|0x2b                                       # FastSpeedPlus SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFPH2|0x2b                                       # FastSpeedPlus SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFPH3|0x2b                                       # FastSpeedPlus SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFPH4|0x2b                                       # FastSpeedPlus SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFPH5|0x2b                                       # FastSpeedPlus SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFPL0|0x4c                                       # FastSpeedPlus SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFPL1|0x4c                                       # FastSpeedPlus SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFPL2|0x4c                                       # FastSpeedPlus SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFPL3|0x4c                                       # FastSpeedPlus SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFPL4|0x4c                                       # FastSpeedPlus SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingFPL5|0x4c                                       # FastSpeedPlus SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingHSD0|0x8                                        # HighSpeed SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingHSD1|0x8                                        # HighSpeed SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingHSD2|0x8                                        # HighSpeed SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingHSD3|0x8                                        # HighSpeed SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingHSD4|0x8                                        # HighSpeed SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingHSD5|0x8                                        # HighSpeed SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingHSH0|0xb                                        # HighSpeed SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingHSH1|0xb                                        # HighSpeed SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingHSH2|0xb                                        # HighSpeed SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingHSH3|0xb                                        # HighSpeed SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingHSH4|0xb                                        # HighSpeed SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingHSH5|0xb                                        # HighSpeed SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingHSL0|0x16                                       # HighSpeed SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingHSL1|0x16                                       # HighSpeed SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingHSL2|0x16                                       # HighSpeed SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingHSL3|0x16                                       # HighSpeed SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingHSL4|0x16                                       # HighSpeed SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingHSL5|0x16                                       # HighSpeed SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingM0C0|0xc8                                       # D0->D3 idle timeout (screen off)
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingM0C1|0xc8                                       # D0->D3 idle timeout (screen off)
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingM0C2|0xc8                                       # D0->D3 idle timeout (screen off)
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingM0C3|0xc8                                       # D0->D3 idle timeout (screen off)
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingM0C4|0xc8                                       # D0->D3 idle timeout (screen off)
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingM0C5|0xc8                                       # D0->D3 idle timeout (screen off)
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingM0C6|0xc8                                       # D0->D3 idle timeout (screen off)
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingM0C9|0xc8                                       # D0->D3 idle timeout (screen off)
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingM0CA|0xc8                                       # D0->D3 idle timeout (screen off)
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingM0CB|0xc8                                       # D0->D3 idle timeout (screen off)
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingM1C0|0x7d0                                      # D0->D3 idle timeout (screen on)
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingM1C1|0x7d0                                      # D0->D3 idle timeout (screen on)
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingM1C2|0x7d0                                      # D0->D3 idle timeout (screen on)
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingM1C3|0x7d0                                      # D0->D3 idle timeout (screen on)
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingM1C4|0x7d0                                      # D0->D3 idle timeout (screen on)
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingM1C5|0x7d0                                      # D0->D3 idle timeout (screen on)
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingM1C6|0x7d0                                      # D0->D3 idle timeout (screen on)
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingM1C9|0xc8                                       # D0->D3 idle timeout (screen on)
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingM1CA|0xc8                                       # D0->D3 idle timeout (screen on)
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingM1CB|0xc8                                       # D0->D3 idle timeout (screen on)
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingSSD0|0x1e                                       # StandardSpeed SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingSSD1|0x1e                                       # StandardSpeed SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingSSD2|0x1e                                       # StandardSpeed SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingSSD3|0x1e                                       # StandardSpeed SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingSSD4|0x1e                                       # StandardSpeed SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingSSD5|0x1e                                       # StandardSpeed SDA Hold
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingSSH0|0x264                                      # StandardSpeed SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingSSH1|0x264                                      # StandardSpeed SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingSSH2|0x264                                      # StandardSpeed SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingSSH3|0x264                                      # StandardSpeed SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingSSH4|0x264                                      # StandardSpeed SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingSSH5|0x2c2                                      # StandardSpeed SCL High
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingSSL0|0x2c2                                      # StandardSpeed SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingSSL1|0x2c2                                      # StandardSpeed SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingSSL2|0x2c2                                      # StandardSpeed SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingSSL3|0x2c2                                      # StandardSpeed SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingSSL4|0x2c2                                      # StandardSpeed SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoTimingSSL5|0x264                                      # StandardSpeed SCL Low
gStructPcdTokenSpaceGuid.PcdSetup.I3c0QueueThld|0x200002                                           #I3C0 Queue Threshold Control
gStructPcdTokenSpaceGuid.PcdSetup.I3c0DataBufferThld|0x1010404                                     #I3C0 Data Buffer Threshold Control
gStructPcdTokenSpaceGuid.PcdSetup.I3c0OdTiming|0xFA00FA                                            #I3C0 SCL I3C Open Drain Timing
gStructPcdTokenSpaceGuid.PcdSetup.I3c0PpTiming|0x80008                                             #I3C0 SCL I3C Push Pull Timing
gStructPcdTokenSpaceGuid.PcdSetup.I3c0SclI2cFmTiming|0xFA00FA                                      #I3C0 SCL I2C Fast Mode Timing
gStructPcdTokenSpaceGuid.PcdSetup.I3c0SclI2cFmpTiming|0x640064                                     #I3C0 SCL I2C Fast Mode Plus Timing
gStructPcdTokenSpaceGuid.PcdSetup.I3c0SclI2cSsTiming|0x32003AC                                     #I3C0 SCL I2C Standard Speed Timing
gStructPcdTokenSpaceGuid.PcdSetup.I3c0ExtLcntTiming|0x20202020                                     #I3C0 SCL Extended Low Count Timing
gStructPcdTokenSpaceGuid.PcdSetup.I3c0ExtTermLcntTiming|0x30000                                    #I3C0 SCL Termination Bit Low Count
gStructPcdTokenSpaceGuid.PcdSetup.I3c0SdaSwitchDelayTiming|0x10000                                 #I3C0 SDA Hold and Mode Switch Delay Timing
gStructPcdTokenSpaceGuid.PcdSetup.I3c0BusDelayTiming|0x8                                           #I3C0 Speed Mode Selection
gStructPcdTokenSpaceGuid.PcdSetup.I3c2QueueThld|0x200002                                           #I3C2 Queue Threshold Control
gStructPcdTokenSpaceGuid.PcdSetup.I3c2DataBufferThld|0x1010404                                     #I3C2 Data Buffer Threshold Control
gStructPcdTokenSpaceGuid.PcdSetup.I3c2OdTiming|0xFA00FA                                            #I3C2 SCL I3C Open Drain Timing
gStructPcdTokenSpaceGuid.PcdSetup.I3c2PpTiming|0x80008                                             #I3C2 SCL I3C Push Pull Timing
gStructPcdTokenSpaceGuid.PcdSetup.I3c2SclI2cFmTiming|0xFA00FA                                      #I3C2 SCL I2C Fast Mode Timing
gStructPcdTokenSpaceGuid.PcdSetup.I3c2SclI2cFmpTiming|0x640064                                     #I3C2 SCL I2C Fast Mode Plus Timing
gStructPcdTokenSpaceGuid.PcdSetup.I3c2SclI2cSsTiming|0x32003AC                                     #I3C2 SCL I2C Standard Speed Timing
gStructPcdTokenSpaceGuid.PcdSetup.I3c2ExtLcntTiming|0x20202020                                     #I3C2 SCL Extended Low Count Timing
gStructPcdTokenSpaceGuid.PcdSetup.I3c2ExtTermLcntTiming|0x30000                                    #I3C2 SCL Termination Bit Low Count
gStructPcdTokenSpaceGuid.PcdSetup.I3c2SdaSwitchDelayTiming|0x10000                                 #I3C2 SDA Hold and Mode Switch Delay Timing
gStructPcdTokenSpaceGuid.PcdSetup.I3c2BusDelayTiming|0x8                                           #I3C2 Speed Mode Selection
gStructPcdTokenSpaceGuid.PcdSetup.IpfEnable|0x1                                                    # IPF Manager Device
gStructPcdTokenSpaceGuid.PcdSetup.PchSerialIoUseTimingParameters|0x0                               # SerialIO timing parameters
gStructPcdTokenSpaceGuid.PcdSetup.PchUartUtkDevice|0x0                                             # UART Test Device
gStructPcdTokenSpaceGuid.PcdSetup.PciDelayOptimizationEcr|0x0                                      # PCI Delay Optimization
gStructPcdTokenSpaceGuid.PcdSetup.PciExpNative|0x1                                                 # Native PCIE Enable
gStructPcdTokenSpaceGuid.PcdSetup.PcieDevOnOffDelay|0xc8                                           # PCIe delay between _OFF _ON
gStructPcdTokenSpaceGuid.PcdSetup.PeciAccessMethod|0x0                                             # PECI Access Method
gStructPcdTokenSpaceGuid.PcdSetup.PepAudio|0x1                                                     # PEP Audio
gStructPcdTokenSpaceGuid.PcdSetup.PepCpu|0x1                                                       # PEP CPU
gStructPcdTokenSpaceGuid.PcdSetup.PepCsme|0x1                                                      # PEP CSME
gStructPcdTokenSpaceGuid.PcdSetup.PepGbe|0x1                                                       # PEP LAN(GBE)
gStructPcdTokenSpaceGuid.PcdSetup.PepGfx|0x1                                                       # PEP Graphics
gStructPcdTokenSpaceGuid.PcdSetup.PepHeci3|0x1                                                     # PEP HECI3
gStructPcdTokenSpaceGuid.PcdSetup.PepI2c0|0x1                                                      # PEP I2C0
gStructPcdTokenSpaceGuid.PcdSetup.PepI2c1|0x1                                                      # PEP I2C1
gStructPcdTokenSpaceGuid.PcdSetup.PepI2c2|0x1                                                      # PEP I2C2
gStructPcdTokenSpaceGuid.PcdSetup.PepI2c3|0x1                                                      # PEP I2C3
gStructPcdTokenSpaceGuid.PcdSetup.PepI2c4|0x1                                                      # PEP I2C4
gStructPcdTokenSpaceGuid.PcdSetup.PepI2c5|0x1                                                      # PEP I2C5
gStructPcdTokenSpaceGuid.PcdSetup.PepIpu|0x1                                                       # PEP IPU
gStructPcdTokenSpaceGuid.PcdSetup.PepIsh|0x1                                                       # PEP ISH
gStructPcdTokenSpaceGuid.PcdSetup.PepPcieGfx|0x3                                                   # PEP PCIe GFX
gStructPcdTokenSpaceGuid.PcdSetup.PepPcieLan|0x1                                                   # PEP PCIe LAN
gStructPcdTokenSpaceGuid.PcdSetup.PepPcieOther|0x0                                                 # PEP PCIe Other
gStructPcdTokenSpaceGuid.PcdSetup.PepPcieStorage|0x1                                               # PEP PCIe Storage
gStructPcdTokenSpaceGuid.PcdSetup.PepPcieWlan|0x1                                                  # PEP PCIe WLAN
gStructPcdTokenSpaceGuid.PcdSetup.PepSpi|0x1                                                       # PEP SPI
gStructPcdTokenSpaceGuid.PcdSetup.PepTcss|0x1                                                      # PEP TCSS
gStructPcdTokenSpaceGuid.PcdSetup.PepThc0|0x1                                                      # PEP THC0
gStructPcdTokenSpaceGuid.PcdSetup.PepThc1|0x1                                                      # PEP THC1
gStructPcdTokenSpaceGuid.PcdSetup.PepUart|0x1                                                      # PEP UART
gStructPcdTokenSpaceGuid.PcdSetup.PepUfs0|0x1                                                      # PEP UFS0
gStructPcdTokenSpaceGuid.PcdSetup.PepUfs1|0x1                                                      # PEP UFS1
gStructPcdTokenSpaceGuid.PcdSetup.PepNpu|0x1                                                       # PEP NPU
gStructPcdTokenSpaceGuid.PcdSetup.PepXhci|0x1                                                      # PEP XHCI
gStructPcdTokenSpaceGuid.PcdSetup.PepVmd|0x1                                                       # PEP VMD
gStructPcdTokenSpaceGuid.PcdSetup.PepOsse|0x0                                                      # PEP OSSE
gStructPcdTokenSpaceGuid.PcdSetup.PmaxAudioCodec|0x0                                               # PMAX Audio codec
gStructPcdTokenSpaceGuid.PcdSetup.PmaxDevice|0x0                                                   # PMAX Device
gStructPcdTokenSpaceGuid.PcdSetup.PmaxFlashDevice|0x0                                              # PMAX Flash device
gStructPcdTokenSpaceGuid.PcdSetup.PmaxUfCamera|0x0                                                 # PMAX UF Camera
gStructPcdTokenSpaceGuid.PcdSetup.PmaxWfCamera|0x0                                                 # PMAX WF Camera
gStructPcdTokenSpaceGuid.PcdSetup.PowermeterDeviceEnable|0x0                                       # Enable PowerMeter
gStructPcdTokenSpaceGuid.PcdSetup.TelemetryDeviceEnable|0x0                                        # Enable Telemetry
gStructPcdTokenSpaceGuid.PcdSetup.PseudoG3State|0x0                                                # Pseudo G3
gStructPcdTokenSpaceGuid.PcdSetup.PstateCapping|0x0                                                # P-state Capping
gStructPcdTokenSpaceGuid.PcdSetup.PtidSupport|0x1                                                  # PTID Support
gStructPcdTokenSpaceGuid.PcdSetup.PuisEnable|0x0                                                   # PUIS Enable
gStructPcdTokenSpaceGuid.PcdSetup.Rtd3AudioDelay|0xc8                                              # Audio Delay
gStructPcdTokenSpaceGuid.PcdSetup.Rtd3P0dl|0x64                                                    # PCIE Slot 5 Device Power-on delay in ms
gStructPcdTokenSpaceGuid.PcdSetup.Rtd3Pcie|0x1                                                     # RTD3 Support for PCIE Rootports
gStructPcdTokenSpaceGuid.PcdSetup.DTbtRtd3|0x0                                                     # DTBT RTD3 Enable
gStructPcdTokenSpaceGuid.PcdSetup.DTbtRtd3OffDelay|0x1F4                                           # TBT Power-off delay in ms
gStructPcdTokenSpaceGuid.PcdSetup.DTbtRtd3ClkReq|0x0                                               # DTBT RTD3 CLKREQ Enable
gStructPcdTokenSpaceGuid.PcdSetup.DTbtRtd3ClkReqDelay|0x0                                          # DTBT RTD3 CLKREQ Delay value
gStructPcdTokenSpaceGuid.PcdSetup.Rtd3SensorHub|0x44                                               # SensorHub
gStructPcdTokenSpaceGuid.PcdSetup.Rtd3Support|0x1                                                  # ACPI D3Cold Support
gStructPcdTokenSpaceGuid.PcdSetup.Rtd3TouchPadDelay|0x44                                           # TouchPad
gStructPcdTokenSpaceGuid.PcdSetup.Rtd3TouchPanelDelay|0x44                                         # TouchPanel
gStructPcdTokenSpaceGuid.PcdSetup.Rtd3WWAN|0x3                                                     # WWAN
gStructPcdTokenSpaceGuid.PcdSetup.ScanMatrixEnable|0x1                                             # Scan Matrix Keyboard Support
gStructPcdTokenSpaceGuid.PcdSetup.SensorHubType|0x1                                                # Sensor Hub Type
gStructPcdTokenSpaceGuid.PcdSetup.SensorStandby|0x0                                                # Sensor Standby
gStructPcdTokenSpaceGuid.PcdSetup.SerialPortAcpiDebug|0x0                                          # Print to Serial Port
gStructPcdTokenSpaceGuid.PcdSetup.SkipHIIUpdate|0x1                                                # Skip Setup HII Update
gStructPcdTokenSpaceGuid.PcdSetup.SkipObbHashVerification|0x2                                      # Skip OBB HASH Verify
gStructPcdTokenSpaceGuid.PcdSetup.GbeFwUpdateEnable|0x1                                            # GBE Firmware Update Support
gStructPcdTokenSpaceGuid.PcdSetup.StorageRtd3Support|0x1                                           # D3 Setting for Storage
gStructPcdTokenSpaceGuid.PcdSetup.StorageDynamicLinkManagement|0x0                                 # StorageDynamicLinkManagement
gStructPcdTokenSpaceGuid.PcdSetup.SysFwUpdateLoadDefault|0x0                                       # Restore Setup Default
gStructPcdTokenSpaceGuid.PcdSetup.SysFwUpdateSkipPowerCheck|0x0                                    # Skip Power Check
gStructPcdTokenSpaceGuid.PcdSetup.SystemTimeAndAlarmSource|0x0                                     # System Time and Alarm Source
gStructPcdTokenSpaceGuid.PcdSetup.DisableTbtPcieTreeBme|0x1                                        # Disable TBT PCIE Tree BME
gStructPcdTokenSpaceGuid.PcdSetup.TcssUcmDevice|0xFF                                               # USBC connector manager selection
gStructPcdTokenSpaceGuid.PcdSetup.TerminalOverDebug|0x1                                            # Virtual UART - Terminal Over Debug
gStructPcdTokenSpaceGuid.PcdSetup.TypecRetimerPD0|0x1                                              # Type C Port 0
gStructPcdTokenSpaceGuid.PcdSetup.TypecRetimerPD1|0x1                                              # Type C Port 1
gStructPcdTokenSpaceGuid.PcdSetup.TypecRetimerPD2|0x1                                              # Type C Port 2
gStructPcdTokenSpaceGuid.PcdSetup.TypecRetimerPD3|0x1                                              # Type C Port 3
gStructPcdTokenSpaceGuid.PcdSetup.TypecRetimerTxComplianceModeEn|0x0                               # Type C retimer TX Compliance Mode
gStructPcdTokenSpaceGuid.PcdSetup.Usb4CmMode|0x1                                                   # USB4 CM Mode
gStructPcdTokenSpaceGuid.PcdSetup.Usb4ClassOption|0x1                                              # USB4 Host Router Class Code
gStructPcdTokenSpaceGuid.PcdSetup.DiscreteTbtSupport|0x0                                           # Discrete Thunderbolt(TM) Support
gStructPcdTokenSpaceGuid.PcdSetup.DTbtController[0]|0x0                                            # DTBT Controller 0
gStructPcdTokenSpaceGuid.PcdSetup.DTbthostRouterPortNumber[0]|0x1                                  # TBT Host Router
gStructPcdTokenSpaceGuid.PcdSetup.DTbtPcieExtraBusRsvd[0]|0x2A                                     # Extra Bus Reserved, default value as DTBT_PCIE_EXTRA_BUS_RSVD_ONE_PORT_DEFAULT
gStructPcdTokenSpaceGuid.PcdSetup.DTbtPcieMemRsvd[0]|0x60                                          # Reserved Memory, default value as DTBT_PCIE_MEM_RSVD_ONE_PORT_DEFAULT
gStructPcdTokenSpaceGuid.PcdSetup.DTbtPcieMemAddrRngMax[0]|0x1A                                    # Memory Alignment, default value as DTBT_PCIE_MEM_ADDRRNGMAX_ONE_PORT_DEFAULT
gStructPcdTokenSpaceGuid.PcdSetup.DTbtPciePMemRsvd[0]|0x8000                                       # Reserved PMemory, default value as DTBT_PCIE_PMEM_RSVD_ONE_PORT_DEFAULT
gStructPcdTokenSpaceGuid.PcdSetup.DTbtPciePMemAddrRngMax[0]|0x1C                                   # PMemory Alignment, default value as DTBT_PCIE_PMEM_ADDRRNGMAX_ONE_PORT_DEFAULT
gStructPcdTokenSpaceGuid.PcdSetup.DTbtController[1]|0x0                                            # DTBT Controller 1
gStructPcdTokenSpaceGuid.PcdSetup.DTbthostRouterPortNumber[1]|0x1                                  # TBT Host Router
gStructPcdTokenSpaceGuid.PcdSetup.DTbtPcieExtraBusRsvd[1]|0x2A                                     # Extra Bus Reserved, default value as DTBT_PCIE_EXTRA_BUS_RSVD_ONE_PORT_DEFAULT
gStructPcdTokenSpaceGuid.PcdSetup.DTbtPcieMemRsvd[1]|0x60                                          # Reserved Memory, default value as DTBT_PCIE_MEM_RSVD_ONE_PORT_DEFAULT
gStructPcdTokenSpaceGuid.PcdSetup.DTbtPcieMemAddrRngMax[1]|0x1A                                    # Memory Alignment, default value as DTBT_PCIE_MEM_ADDRRNGMAX_ONE_PORT_DEFAULT
gStructPcdTokenSpaceGuid.PcdSetup.DTbtPciePMemRsvd[1]|0x8000                                       # Reserved PMemory, default value as DTBT_PCIE_PMEM_RSVD_ONE_PORT_DEFAULT
gStructPcdTokenSpaceGuid.PcdSetup.DTbtPciePMemAddrRngMax[1]|0X1C                                   # PMemory Alignment, default value as DTBT_PCIE_PMEM_ADDRRNGMAX_ONE_PORT_DEFAULT
gStructPcdTokenSpaceGuid.PcdSetup.Usb4DebugLevel|0x0                                               # USB4 Debug Level
gStructPcdTokenSpaceGuid.PcdSetup.UsbConnStatusTimeout|0x1f4                                       # Timeout for EC USB enumeration message
gStructPcdTokenSpaceGuid.PcdSetup.UsbSensorHub|0x0                                                 # USB Sensor Hub
gStructPcdTokenSpaceGuid.PcdSetup.UsbcBiosTcssHandshake|0x1                                        # BIOS-TCSS handshake
gStructPcdTokenSpaceGuid.PcdSetup.UsbcCapsuleDebugLevel|0x0                                        # USBC Capsule debug level
gStructPcdTokenSpaceGuid.PcdSetup.UsbcDataRoleSwapPlatformDisable|0x1                              # USBC DataRole Swap Platform Disable Option
gStructPcdTokenSpaceGuid.PcdSetup.VRRampUpDelay|0x10                                               # VR Ramp up delay
gStructPcdTokenSpaceGuid.PcdSetup.VirtualKeyboard|0x0                                              # Enable Virtual Keyboard
gStructPcdTokenSpaceGuid.PcdSetup.WakeOnRTCS5|0x0                                                  # Wake System from S5 via RTC
gStructPcdTokenSpaceGuid.PcdSetup.SafeLoadingBiosEnable|0x0                                        # Safe Loading Bios Enable
gStructPcdTokenSpaceGuid.PcdSetup.TsegReallocateEnableCount|0x0                                    # Tseg Reallocate Enable Count
gStructPcdTokenSpaceGuid.PcdSetup.WirelessCnvConfigDevice|0x1                                      # Wireless CNV Config Device
gStructPcdTokenSpaceGuid.PcdSetup.WwanEnable|0x0                                                   # WWAN Device
gStructPcdTokenSpaceGuid.PcdSetup.WwanFwFlashDevice|0x0                                            # Firmware Flash Device
gStructPcdTokenSpaceGuid.PcdSetup.WwanOemSvid|0x1cf8                                               # WA - WWAN OEM SVID
gStructPcdTokenSpaceGuid.PcdSetup.WwanResetWorkaround|0x1                                          # WWAN Reset Workaround
gStructPcdTokenSpaceGuid.PcdSetup.WwanSvidTimeout|0x0                                              # WA - WWAN SVID Detect Timeout
gStructPcdTokenSpaceGuid.PcdSetup.WwanTOffDisDelayMs|0x1f4                                         # TOffDis
gStructPcdTokenSpaceGuid.PcdSetup.WwanTOn2ResDelayMs|0x14                                          # TOn2Res
gStructPcdTokenSpaceGuid.PcdSetup.WwanTOnPer2PdsDelayMs|0x0                                        # TOnPer2Pds
gStructPcdTokenSpaceGuid.PcdSetup.WwanTOnRes2PerDelayMs|0x50                                       # TOnRes2Per
gStructPcdTokenSpaceGuid.PcdSetup.WwanTPer2ResDelayMs|0x14                                         # TPer2Res
gStructPcdTokenSpaceGuid.PcdSetup.WwanTRes2OffDelayMs|0xa                                          # TRes2Off
gStructPcdTokenSpaceGuid.PcdSetup.WwanTRes2PdsDelayMs|0xa                                          # TRes2Pds
gStructPcdTokenSpaceGuid.PcdSetup.WwanTResTogDelayMs|0xa                                           # TResTog
!if gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE
    gStructPcdTokenSpaceGuid.PcdSiSetup.PlatformDebugOption|0x2                                    # Platform Debug Option
!else
    gStructPcdTokenSpaceGuid.PcdSiSetup.PlatformDebugOption|0x0                                    # Platform Debug Option
!endif
gStructPcdTokenSpaceGuid.PcdSndwDevTopologyConfigurationVariable.SndwDevTopologyConfigurationNumber|0x0     # SoundWire codecs topology
gStructPcdTokenSpaceGuid.PcdTCG2_CONFIGURATION.TpmDevice|0x1                                       # Attempt TPM Device
gStructPcdTokenSpaceGuid.PcdTCG2_VERSION.PpiVersion|0x332e31                                       # Attempt PPI Version
gStructPcdTokenSpaceGuid.PcdTCG2_VERSION.Tpm2AcpiTableRev|0x4                                      # Attempt Rev of TPM2 ACPI Table
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevFlags[1]|0x0                                               # Flags
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevFlags[2]|0x0                                               # Flags
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevFlags[3]|0x0                                               # Flags
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevFlags[4]|0x0                                               # Flags
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevHeciEntry|0x0                                              # Expose HECI SDEV Entry
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevIspEntry|0x1                                               # Expose ISP SDEV Entry
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevNumberOfSensors[1]|0x2                                     # Number of Sensor Entries
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevNumberOfSensors[2]|0x0                                     # Number of Sensor Entries
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevNumberOfSensors[3]|0x1                                     # Number of Sensor Entries
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevNumberOfSensors[4]|0x1                                     # Number of Sensor Entries
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevSensorEntry1[1]|0x1                                        # Sensor Entry 1
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevSensorEntry1[2]|0x0                                        # Sensor Entry 1
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevSensorEntry1[3]|0x1                                        # Sensor Entry 1
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevSensorEntry1[4]|0x1                                        # Sensor Entry 1
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevSensorEntry2[1]|0x85                                       # Sensor Entry 2
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevSensorEntry2[2]|0x0                                        # Sensor Entry 2
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevSensorEntry2[3]|0x0                                        # Sensor Entry 2
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevSensorEntry2[4]|0x0                                        # Sensor Entry 2
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevSpi1Entry|0x0                                              # Expose SPI1 Dev 1E Fun 2 SDEV Entry
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevSpi2Entry|0x0                                              # Expose SPI2 Dev 1E Fun 3 SDEV Entry
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciAcpiPathNameDevice1|{0x0, 0x0}                      # ACPI Path Name
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciAcpiPathNameDevice2|{0x0, 0x0}                      # ACPI Path Name
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciAcpiPathStringLength[0]|0x1d                           # ACPI Path String Length
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciAcpiPathStringLength[1]|0x1c                           # ACPI Path String Length
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciAcpiPathStringOffset[0]|0x34                           # ACPI Path String Offset
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciAcpiPathStringOffset[1]|0x34                           # ACPI Path String Offset
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciClass[0]|0xe                                           # Class
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciClass[1]|0xe                                           # Class
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciDeviceAttributes[0]|0x0                                # Attributes
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciDeviceAttributes[1]|0x0                                # Attributes
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciEntry|0x0                                              # Expose XHCI SDEV Entry
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciFirmwareHashDevice1[0]|0x0                             # Firmware Hash [63:0]
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciFirmwareHashDevice1[1]|0x0                             # Firmware Hash [127:64]
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciFirmwareHashDevice1[2]|0x0                             # Firmware Hash [191:128]
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciFirmwareHashDevice1[3]|0x0                             # Firmware Hash [255:192]
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciFirmwareHashDevice2[0]|0x0                             # Firmware Hash [63:0]
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciFirmwareHashDevice2[1]|0x0                             # Firmware Hash [127:64]
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciFirmwareHashDevice2[2]|0x0                             # Firmware Hash [191:128]
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciFirmwareHashDevice2[3]|0x0                             # Firmware Hash [255:192]
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciFlags|0x0                                              # Flags
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciInterfaceNumber[0]|0x0                                 # Interface Number
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciInterfaceNumber[1]|0x0                                 # Interface Number
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciNumberOfDevices|0x2                                    # Number of USB Devices
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciProductId[0]|0x0                                       # PID
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciProductId[1]|0x0                                       # PID
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciProtocol[0]|0x1                                        # Protocol
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciProtocol[1]|0x1                                        # Protocol
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciRevision[0]|0x0                                        # Revision
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciRevision[1]|0x0                                        # Revision
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciRootPortNumber[0]|0x0                                  # Root Port Number
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciRootPortNumber[1]|0x1                                  # Root Port Number
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciSubClass[0]|0x1                                        # Subclass
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciSubClass[1]|0x1                                        # Subclass
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciVendorId[0]|0x0                                        # VID
gStructPcdTokenSpaceGuid.PcdVtioCfg.SdevXhciVendorId[1]|0x0                                        # VID
gStructPcdTokenSpaceGuid.PcdVtioCfg.VtioSupport|0x1                                                # Enable VTIO Support
!if gXmlCliFeaturePkgTokenSpaceGuid.PcdXmlCliFeatureEnable == TRUE
    !if gXmlCliFeaturePkgTokenSpaceGuid.PcdEnableXmlCliLite == 0x1
        gStructPcdTokenSpaceGuid.PcdXmlCli.EnableXmlCliLite|0x1                                        # XmlCli Lite
    !else
        gStructPcdTokenSpaceGuid.PcdXmlCli.EnableXmlCliLite|0x0                                        # XmlCli Lite
    !endif
!endif
!if gXmlCliFeaturePkgTokenSpaceGuid.PcdPublishSetupPgPtr == 0x1
    gStructPcdTokenSpaceGuid.PcdXmlCli.PublishSetupPgPtr|0x1                                       # Publish Setup page Pointer
!else
    gStructPcdTokenSpaceGuid.PcdXmlCli.PublishSetupPgPtr|0x0                                       # Publish Setup page Pointer
!endif
!if gXmlCliFeaturePkgTokenSpaceGuid.PcdXmlCliSupport == 0x1
    gStructPcdTokenSpaceGuid.PcdXmlCli.XmlCliSupport|0x1                                           # XmlCli Support
!else
    gStructPcdTokenSpaceGuid.PcdXmlCli.XmlCliSupport|0x0                                           # XmlCli Support
!endif

