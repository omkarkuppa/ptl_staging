## @file
#  Board description file updates configuration (PCD) settings by if-statement for this project.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2021 Intel Corporation.
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

[PcdsFeatureFlag]
!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 1
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterDebugInit|TRUE
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 2
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterDebugInit|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterMemInit|TRUE
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 3
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterMemInit|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly|TRUE
  gBoardModuleTokenSpaceGuid.PcdUefiShellEnable|TRUE
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 4
  gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly|FALSE
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 5
  gBoardModuleTokenSpaceGuid.PcdSecurityEnable|TRUE
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 6
  gBoardModuleTokenSpaceGuid.PcdAdvancedFeatureEnable|TRUE
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage == 7
  gBoardModuleTokenSpaceGuid.PcdOptimizationEnable|TRUE
!endif

!if gBoardModuleTokenSpaceGuid.PcdSecurityEnable == TRUE
  gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable|TRUE
!endif  #PcdSecurityEnable

!if gBoardModuleTokenSpaceGuid.PcdOptimizationEnable == TRUE
  gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable|TRUE
!endif  #PcdOptimizationEnable

!if gBoardModuleTokenSpaceGuid.PcdAdvancedFeatureEnable == TRUE
  gNhltFeaturePkgTokenSpaceGuid.PcdNhltFeatureEnable|TRUE
  gMebxFeaturePkgTokenSpaceGuid.PcdMebxFeatureEnable|TRUE
  gAmtMacPassThroughFeaturePkgTokenSpaceGuid.PcdAmtMacPassThroughFeatureEnable|TRUE
  gAmtPetInitFeaturePkgTokenSpaceGuid.PcdAmtPetInitFeatureEnable|TRUE
  gPsrFeatureModuleTokenSpaceGuid.PcdPlatformServiceRecordSupport|TRUE
  gVtioFeaturePkgTokenSpaceGuid.PcdVtioFeatureEnable|TRUE
  gTmeFeaturePkgTokenSpaceGuid.PcdTmeFeatureEnable|TRUE
  gTseFeaturePkgTokenSpaceGuid.PcdTseFeatureEnable|TRUE
  gTdxFeaturePkgTokenSpaceGuid.PcdTdxFeatureEnable|TRUE
  gSndwDevTopologyFeaturePkgTokenSpaceGuid.PcdSndwDevTopologyFeatureEnable|TRUE
  gSndwDevTopologyFeaturePkgTokenSpaceGuid.PcdSndwDevTopologyIDispSndwSupported|TRUE
    gSndwDevTopologyFeaturePkgTokenSpaceGuid.PcdSndwDevTopologyBtSndwSupported|TRUE

  # Tcss UsbTypeC : Update Pcd value
  gUcsiFeaturePkgTokenSpaceGuid.PcdUcsiFeatureEnable|TRUE
  gUsbPortMapPkgTokenSpaceGuid.PcdUsbPortMapEnable|TRUE
!endif

  # I2C
  gI2cFeaturePkgTokenSpaceGuid.PcdI2cFeatureEnable|TRUE
  gI2cFeaturePkgTokenSpaceGuid.PcdI2cPssEnable|TRUE
  gI2cFeaturePkgTokenSpaceGuid.PcdI2cTouchEnable|TRUE

  # UFS
  gUfsFeaturePkgTokenSpaceGuid.PcdUfsFeatureEnable|TRUE
  !if gUfsFeaturePkgTokenSpaceGuid.PcdUfsFeatureEnable
    !if gPsFeatureModuleTokenSpaceGuid.PcdPlatformSanitizeFeatureEnable
        gUfsFeaturePkgTokenSpaceGuid.PcdUfsEraseSupport|TRUE
    !endif
  !endif

!if $(FSP64_BUILD) == TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdDxeIplSwitchToLongMode|FALSE
!else
  gEfiMdeModulePkgTokenSpaceGuid.PcdDxeIplSwitchToLongMode|TRUE
!endif

[PcdsFixedAtBuild]
#
# 14 MB BIOS Flash Base Address and Size
#
  gSiPkgTokenSpaceGuid.PcdBiosAreaBaseAddress|0xFF200000
  gSiPkgTokenSpaceGuid.PcdBiosSize|0x0E00000
#
# Setting feature PCDs based on boot stages set above
#
!if gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly == FALSE
  gSiPkgTokenSpaceGuid.PcdAcpiEnable|TRUE
  gBoardModuleTokenSpaceGuid.PcdAcpiDebugFeatureEnable|TRUE
!endif  #PcdBootToShellOnly

!if gBoardModuleTokenSpaceGuid.PcdAdvancedFeatureEnable == TRUE
  gSiPkgTokenSpaceGuid.PcdPpmEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdSmbiosEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdS3Enable|TRUE
  gSiPkgTokenSpaceGuid.PcdTcssSupport|TRUE
  gSiPkgTokenSpaceGuid.PcdBootGuardEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdTxtEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdTseEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable|FALSE

  gSiPkgTokenSpaceGuid.PcdMrcTraceMessageSupported|TRUE

  gPlatformModuleTokenSpaceGuid.PcdBeepStatusCodeEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdPostCodeStatusCodeEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdSourceDebugEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdBiosGuardEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdHgEnable|TRUE
  gBoardModuleTokenSpaceGuid.PcdS4Enable|TRUE

  # Multi-IBB support
  gMultiIbbFeaturePkgTokenSpaceGuid.PcdMultiIbbFeatureEnable|FALSE
  !if gMultiIbbFeaturePkgTokenSpaceGuid.PcdMultiIbbFeatureEnable == FALSE
    gMultiIbbFeaturePkgTokenSpaceGuid.PcdMultiIbbInfoCount|1
    gMultiIbbFeaturePkgTokenSpaceGuid.PcdCacheMissDetectEnable|FALSE
  !else
    gMultiIbbFeaturePkgTokenSpaceGuid.PcdMultiIbbInfoCount|4
    gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection|0
    gMinPlatformPkgTokenSpaceGuid.PcdFspDispatchModeUseFspPeiMain|FALSE
  !endif

  gBoardModuleTokenSpaceGuid.PcdIntelRaidEnable|TRUE
  gBoardModuleTokenSpaceGuid.PcdTerminalEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdOverclockEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdSpecialPoolEnable|FALSE
  gBoardModuleTokenSpaceGuid.PcdGbeFwUpdateEnable|TRUE

  gSiPkgTokenSpaceGuid.PcdEmbeddedEnable|0x0
  gSiPkgTokenSpaceGuid.PcdAmtEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdAtaEnable|FALSE
  gMipiCamFeaturePkgTokenSpaceGuid.PcdIpuDriverPlatformId|0x12

  ## This PCD controls the OCR capabilites set by OEM.
  #  Default values is 0xF as all capabilites are enabled.
  #  BIT0 - Controls OCR HTTPS Boot capability (Default Enabled).
  #  BIT1 - Controls OCR PBA boot capability (Default Enabled).
  #  BIT2 - Controls OCR WinRe boot capability (Default Enabled).
  #  BIT3 - Controls OCR Secure boot disable capability (Default Enabled).
  gOneClickRecoveryFeaturePkgTokenSpaceGuid.PcdOneClickRecoveryCapabilities|0xF

  !if (gProtectedVariableFeaturePkgTokenSpaceGuid.PcdProtectedVariableEnable == TRUE)
    gEfiProtectedVariablePkgTokenSpaceGuid.PcdProtectedVariableIntegrity|TRUE
    gEfiProtectedVariablePkgTokenSpaceGuid.PcdProtectedVariableConfidentiality|TRUE
  !endif

    gUsb4FeaturePkgTokenSpaceGuid.PcdDTbtEnable|TRUE
    gUsbCCapsuleFeaturePkgTokenSpaceGuid.PcdMaxUsbCPdNumber|0x4

  !if gUsb4FeaturePkgTokenSpaceGuid.PcdDTbtEnable == TRUE
    gUsb4FeaturePkgTokenSpaceGuid.PcdBoardDTbtControllerNumber|0x1
    gUsb4FeaturePkgTokenSpaceGuid.PcdIoeDTbtRpSupport|0x00000020
    gUsb4FeaturePkgTokenSpaceGuid.PcdPchDTbtRpSupport|0x00000000
  !endif

  gPlatformModuleTokenSpaceGuid.PcdCapsuleEnable|TRUE
  gPlatformModuleTokenSpaceGuid.PcdUsb3SerialStatusCodeEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdUserAuthenticationEnable|TRUE

  gPlatformModuleTokenSpaceGuid.PcdPciHotplugEnable|TRUE
  gPlatformModuleTokenSpaceGuid.PcdVirtualKeyboardEnable|TRUE
  gPlatformModuleTokenSpaceGuid.PcdEbcEnable|TRUE
  gPlatformModuleTokenSpaceGuid.PcdHddPasswordEnable|TRUE
  gPlatformModuleTokenSpaceGuid.PcdNetworkEnable|TRUE
  gPantherLakeBinPkgTokenSpaceGuid.PcdGigUndiBinEnable|TRUE
  gPlatformModuleTokenSpaceGuid.PcdSinitAcmBinEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdScsiEnable|TRUE
  gPlatformModuleTokenSpaceGuid.PcdJpgEnable|TRUE
  gPlatformModuleTokenSpaceGuid.PcdUsbEnable|TRUE
  gCmosFeaturePkgTokenSpaceGuid.PcdPlatformCmosAccessSupport|TRUE

  gFspWrapperFeaturePkgTokenSpaceGuid.PcdFspWrapperFeatureSupport|TRUE

  #
  # Build Switch for bin files.
  #
  !if gSiPkgTokenSpaceGuid.PcdBiosGuardEnable == TRUE
    gPantherLakeBinPkgTokenSpaceGuid.PcdBiosGuardBinEnable|TRUE             #BiosGuardModule.bin
  !else
    gPantherLakeBinPkgTokenSpaceGuid.PcdBiosGuardBinEnable|FALSE
  !endif
  !if gNhltFeaturePkgTokenSpaceGuid.PcdNhltFeatureEnable == TRUE
    gPantherLakeBinPkgTokenSpaceGuid.PcdNhltBinEnable|TRUE
  !endif
  gPlatformModuleTokenSpaceGuid.PcdSinitAcmBinEnable|FALSE           #SinitAc.bin

  #
  # Build Switch for efi files.
  #

  !if gSiPkgTokenSpaceGuid.PcdAtaEnable == TRUE
    !if gBoardModuleTokenSpaceGuid.PcdIntelRaidEnable == TRUE
      gCommonBinPkgTokenSpaceGuid.PcdRaidDriverEfiEnable|TRUE        # RaidDriver.efi
      gCommonBinPkgTokenSpaceGuid.PcdRsteDriverEfiEnable|TRUE        # SataDriverRste.efi
    !endif
  !endif

  gPlatformModuleTokenSpaceGuid.PcdNetworkIp6Enable|TRUE
  gPlatformModuleTokenSpaceGuid.PcdNetworkIscsiEnable|TRUE
  gPlatformModuleTokenSpaceGuid.PcdNetworkVlanEnable|TRUE
  gPlatformModuleTokenSpaceGuid.PcdCryptoEnable|TRUE    # Current Smbios implementation needs this
  gPlatformModuleTokenSpaceGuid.PcdLzmaEnable|TRUE
  gPlatformModuleTokenSpaceGuid.PcdDxeCompressEnable|TRUE
  gPlatformModuleTokenSpaceGuid.PcdUserIdentificationEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdOpalPasswordEnable|TRUE
  gPlatformModuleTokenSpaceGuid.PcdMemoryTestEnable|TRUE
  gPantherLakeBinPkgTokenSpaceGuid.PcdDgrPolicyOverride|FALSE
  !if gPlatformModuleTokenSpaceGuid.PcdCryptoEnable == TRUE
    gBoardModuleTokenSpaceGuid.PcdModularCryptoEnable|TRUE
  !endif

  gSiPkgTokenSpaceGuid.PcdOcWdtEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdLpssUartEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdIpuEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdIgdEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdPeiDisplayEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdVtdEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdCpuPowerOnConfigEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdDmiEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdVmdEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdPttEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdSmmVariableEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdThcEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdOptimizeCompilerEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdPpmEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdPsmiEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdSpsStateSaveEnable|TRUE
  !if $(TARGET) == DEBUG
    gSiPkgTokenSpaceGuid.PcdSpaEnable|TRUE
  !endif
  gSiPkgTokenSpaceGuid.PcdNpuEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdDtprEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdPpamEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdStatusCodeUseTraceHub|TRUE

  gCnvFeaturePkgTokenSpaceGuid.PcdCnvFeatureEnable|TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdCnvBinLoadFromESP|TRUE
!endif #PcdAdvancedFeatureEnable

  gSiPkgTokenSpaceGuid.PcdSocCoreBootEnable|FALSE
  ## This flag is used to initialize debug output interface.
  #  BIT0 - RAM debug interface.
  #  BIT1 - UART debug interface.
  #  BIT2 - USB debug interface.
  #  BIT3 - USB3 debug interface.
  #  BIT4 - Serial IO debug interface.
  #  BIT5 - TraceHub debug interface.
  #  BIT6 - Reserved.
  #  BIT7 - CMOS control.
  gPlatformModuleTokenSpaceGuid.PcdStatusCodeFlags|0x32
  gCmosFeaturePkgTokenSpaceGuid.PcdCmosStatusCodeFlags|gPlatformModuleTokenSpaceGuid.PcdStatusCodeFlags
  !if $(TARGET) == RELEASE
    gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x0
    gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x3
    gSiPkgTokenSpaceGuid.PcdTraceHubCatalogEnable|TRUE
  !else
    gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2F
    gEfiMdeModulePkgTokenSpaceGuid.PcdSerialUseHardwareFlowControl|FALSE
    gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x07
  !endif

!if (gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE)
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x02
  gEfiMdePkgTokenSpaceGuid.PcdFixedDebugPrintErrorLevel|0x80000000   # This PCD gets overriden in prep.bat based on 'CATALOG_DEBUG_PRINT_LEVEL' ENV variable.
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80000000
  gPlatformModuleTokenSpaceGuid.PcdStatusCodeUseUsb3|FALSE
  gPlatformModuleTokenSpaceGuid.PcdStatusCodeUseSerialIoUart|FALSE
  gSiPkgTokenSpaceGuid.PcdMrcTraceMessageSupported|FALSE
  gSiPkgTokenSpaceGuid.PcdTraceHubCatalogEnable|TRUE
!endif

  gBoardModulePkgTokenSpaceGuid.PcdPs2KbMsEnable|0x01
  gBoardModulePkgTokenSpaceGuid.PcdSuperIoPciIsaBridgeDevice|{0x00, 0x00, 0x1F, 0x00}
  gBoardModulePkgTokenSpaceGuid.PcdUart1Enable|0x01
  gBoardModulePkgTokenSpaceGuid.PcdUart1IrqMask|0x0010
  gBoardModulePkgTokenSpaceGuid.PcdUart1IoPort|0x03F8
  gBoardModulePkgTokenSpaceGuid.PcdUart1Length|0x08

#
# Silicon feature settings
#
!if gBoardModuleTokenSpaceGuid.PcdSecurityEnable == TRUE
  gPlatformModuleTokenSpaceGuid.PcdTpmEnable|TRUE
!endif

#
# DPTF Advanced Feature Enable
#
!if gDptfFeaturePkgTokenSpaceGuid.PcdDptfFeatureEnable == TRUE
  gDptfFeaturePkgTokenSpaceGuid.PcdDttFan1Participant|TRUE
  gDptfFeaturePkgTokenSpaceGuid.PcdDttFan2Participant|TRUE
  gDptfFeaturePkgTokenSpaceGuid.PcdDttFan3Participant|FALSE
  gDptfFeaturePkgTokenSpaceGuid.PcdDttPchpParticipant|FALSE
  gDptfFeaturePkgTokenSpaceGuid.PcdDttChargerParticipant|TRUE
  gDptfFeaturePkgTokenSpaceGuid.PcdDttBatteryParticipant|TRUE
  gDptfFeaturePkgTokenSpaceGuid.PcdDttPowerParticipant|TRUE
  gDptfFeaturePkgTokenSpaceGuid.PcdDttSen1Participant|FALSE
  gDptfFeaturePkgTokenSpaceGuid.PcdDttSen2Participant|TRUE
  gDptfFeaturePkgTokenSpaceGuid.PcdDttSen3Participant|TRUE
  gDptfFeaturePkgTokenSpaceGuid.PcdDttSen4Participant|TRUE
  gDptfFeaturePkgTokenSpaceGuid.PcdDttSen5Participant|TRUE
!endif

#
# FV Copy Free Space Skip Feature Enable
#
  gFvCopyPkgTokenSpaceGuid.PcdFreeSpaceSkipFvCopyEnable|TRUE

#
# Connectivity Advanced Feature Settings
#
!if gCnvFeaturePkgTokenSpaceGuid.PcdCnvFeatureEnable == TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdMaxRootPortNumber     |6
  gCnvFeaturePkgTokenSpaceGuid.PcdMaxUsb2PortNumber     |gBoardModuleTokenSpaceGuid.PcdMaxPchUsb2PortNumber
  gCnvFeaturePkgTokenSpaceGuid.PcdCnvUefiVarVersion     |3
  gCnvFeaturePkgTokenSpaceGuid.PcdCnvSetupMenu          |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdCnvAcpiTables         |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdCnvUefiVariables      |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdCnvIntegratedSupport  |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdCnvDiscreteSupport    |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdBtAudioOffloadSupport |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdPrebootBleSupport     |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdDynamicSarSupport     |TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdCnvSetupMenuConfig    |0x80021001
!else
  gCnvFeaturePkgTokenSpaceGuid.PcdCnvSetupMenu          |FALSE
  gCnvFeaturePkgTokenSpaceGuid.PcdCnvAcpiTables         |FALSE
  gCnvFeaturePkgTokenSpaceGuid.PcdCnvUefiVariables      |FALSE
  gCnvFeaturePkgTokenSpaceGuid.PcdCnvIntegratedSupport  |FALSE
  gCnvFeaturePkgTokenSpaceGuid.PcdCnvDiscreteSupport    |FALSE
  gCnvFeaturePkgTokenSpaceGuid.PcdBtAudioOffloadSupport |FALSE
  gCnvFeaturePkgTokenSpaceGuid.PcdPrebootBleSupport     |FALSE
  gCnvFeaturePkgTokenSpaceGuid.PcdDynamicSarSupport     |FALSE
!endif

#
# USB TypeC Advanced Feature Settings
#
!if gUsbPortMapPkgTokenSpaceGuid.PcdUsbPortMapEnable == TRUE
  gUsbPortMapPkgTokenSpaceGuid.PcdMaxPchUsb2PortNumber    |gBoardModuleTokenSpaceGuid.PcdMaxPchUsb2PortNumber
  gUsbPortMapPkgTokenSpaceGuid.PcdMaxPchUsb3PortNumber    |gBoardModuleTokenSpaceGuid.PcdMaxPchUsb3PortNumber
!endif



!if gSiPkgTokenSpaceGuid.PcdAmtEnable == FALSE
  gOneClickRecoveryFeaturePkgTokenSpaceGuid.PcdOneClickRecoveryEnable|FALSE
  gOneClickRecoveryFeaturePkgTokenSpaceGuid.PcdWifiProfileSyncEnable|FALSE
  gPsFeatureModuleTokenSpaceGuid.PcdPlatformSanitizeRpeSupport|FALSE
  gAmtPetInitFeaturePkgTokenSpaceGuid.PcdAmtPetInitFeatureEnable|FALSE
!endif
  # Restricting the simulation support to Intel Internal only

#
# DMAR ACPI feature settings
#
!if gDmarAcpiFeaturePkgTokenSpaceGuid.PcdDmarFeatureEnable == TRUE
  gDmarAcpiFeaturePkgTokenSpaceGuid.PcdDmarRmrrSupport|TRUE
  gDmarAcpiFeaturePkgTokenSpaceGuid.PcdDmarSatcSupport|TRUE
  gDmarAcpiFeaturePkgTokenSpaceGuid.PcdDmarSidpSupport|TRUE
  gDmarAcpiFeaturePkgTokenSpaceGuid.PcdDmarRhsaSupport|FALSE
  gDmarAcpiFeaturePkgTokenSpaceGuid.PcdDmarAtsrSupport|FALSE
!endif

# Disabling 64 bit Smbios Entrypoint Structure and populated tables
# Bit0 is to enable 32 bit and Bit1 is to enable 64 bit Smbios tables
!if gSiPkgTokenSpaceGuid.PcdSmbiosEnable == TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSmbiosEntryPointProvideMethod|0x01
!endif

!if gPlatformModuleTokenSpaceGuid.PcdBeepStatusCodeEnable == TRUE
  gBeepDebugFeaturePkgTokenSpaceGuid.PcdStatusCodeUseBeep|TRUE
!else
  gBeepDebugFeaturePkgTokenSpaceGuid.PcdStatusCodeUseBeep|FALSE
!endif

  gSiPkgTokenSpaceGuid.PcdDumpDefaultSiliconPolicy|FALSE

!if gPlatformModuleTokenSpaceGuid.PcdPostCodeStatusCodeEnable == TRUE
  gPostCodeDebugFeaturePkgTokenSpaceGuid.PcdStatusCodeUsePostCode|TRUE
!else
  gPostCodeDebugFeaturePkgTokenSpaceGuid.PcdStatusCodeUsePostCode|FALSE
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable == TRUE
  gEfiMdePkgTokenSpaceGuid.PcdPerformanceLibraryPropertyMask|0x1
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxPeiPerformanceLogEntries|140
  gEfiMdeModulePkgTokenSpaceGuid.PcdEdkiiFpdtStringRecordEnableOnly|TRUE
!endif

!if $(TARGET) != RELEASE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSmiHandlerProfilePropertyMask|0x1
!endif

!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  #
  # In EDK2 mode or FSP API mode, the PciExpressBaseAddress is not necessary DynamicEx
  #
  gSiPkgTokenSpaceGuid.PcdSiPciExpressBaseAddress|gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress

  #
  # Disable migration feature in FSP API mode
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdMigrateTemporaryRamFirmwareVolumes|FALSE
!endif

  # Temporary RAM Size for FSP build
  gIntelFsp2PkgTokenSpaceGuid.PcdTemporaryRamBase|0x00000000
  gIntelFsp2PkgTokenSpaceGuid.PcdTemporaryRamSize|0x00000000
  gFspWrapperFeaturePkgTokenSpaceGuid.PcdFspWrapperPcdBootGuardCnbSize|0x1000
!if gSiPkgTokenSpaceGuid.PcdEnableFspmCompression == TRUE
  gSiPkgTokenSpaceGuid.PcdSecondaryDataStackBase|0xFA000000
  gMinPlatformPkgTokenSpaceGuid.PcdFspDispatchModeUseFspPeiMain|FALSE
!endif

  !if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
    #
    # FSP API mode will only establish separate Heap.
    #
      gIntelFsp2PkgTokenSpaceGuid.PcdFspTemporaryRamSize|0x00020000
  !else
    #
    # FSP Dispatch mode will not establish separate Stack or Heap.
    #
    gIntelFsp2PkgTokenSpaceGuid.PcdFspTemporaryRamSize|0
  !endif


!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  # Temporary RAM Stack size for FSP Build
    gUefiCpuPkgTokenSpaceGuid.PcdPeiTemporaryRamStackSize|0x00040000
!endif

!if $(TARGET) == DEBUG
  !if gSiPkgTokenSpaceGuid.PcdLpssUartEnable == TRUE
    gSiPkgTokenSpaceGuid.PcdLpssUartDebugEnable|TRUE
    gSiPkgTokenSpaceGuid.PcdLpssUartNumber|0
    gPlatformModuleTokenSpaceGuid.PcdStatusCodeUseSerialIoUart|TRUE
  !endif
!endif

  #
  # FSP Binary base address will be set in FDF basing on flash map
  #
  gIntelFsp2WrapperTokenSpaceGuid.PcdFsptBaseAddress|0

!if gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable == TRUE
  ## Pcd for OptionRom.
  #  Image verification policy settings:
  #  ALWAYS_EXECUTE                         0x00000000
  #  NEVER_EXECUTE                          0x00000001
  #  ALLOW_EXECUTE_ON_SECURITY_VIOLATION    0x00000002
  #  DEFER_EXECUTE_ON_SECURITY_VIOLATION    0x00000003
  #  DENY_EXECUTE_ON_SECURITY_VIOLATION     0x00000004
  #  QUERY_USER_ON_SECURITY_VIOLATION       0x00000005
  gEfiSecurityPkgTokenSpaceGuid.PcdOptionRomImageVerificationPolicy|0x00000004
  # Change PcdRsa2048Sha256PublicKeyBuffer's value to the content of
  # BaseTools\Source\Python\Rsa2048Sha256Sign\TestSigningPublicKey.txt for TestSigningPrivateKey.pem
  gEfiSecurityPkgTokenSpaceGuid.PcdRsa2048Sha256PublicKeyBuffer|{0x8A, 0xF3, 0x87, 0x6B, 0x0F, 0xD4, 0xA3, 0x90, 0x15, 0xD7, 0x40, 0xC5, 0x3A, 0x94, 0x9B, 0xF4, 0xE0, 0x58, 0x53, 0x58, 0x87, 0x89, 0x67, 0x84, 0x60, 0xAF, 0x8E, 0xB4, 0x16, 0x1F, 0x52, 0x51}
!endif

!if gSiPkgTokenSpaceGuid.PcdS3Enable == TRUE
  ## Indicates if to shadow PEIM on S3 boot path after memory is ready.<BR><BR>
  #   TRUE  - Shadow PEIM on S3 boot path after memory is ready.<BR>
  #   FALSE - Not shadow PEIM on S3 boot path after memory is ready.<BR>
  # @Prompt Shadow Peim On S3 Boot.
  gEfiMdeModulePkgTokenSpaceGuid.PcdShadowPeimOnS3Boot|TRUE

  #
  # For RS5 (1 core, 2 thread), increase page number to avoid S3 exit fail
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdS3BootScriptRuntimeTableReservePageNumber|0x4
!endif

!if gSiPkgTokenSpaceGuid.PcdPpamEnable == TRUE
  gUefiCpuPkgTokenSpaceGuid.PcdCpuMsegSize|0x11A000
!else
  gSiPkgTokenSpaceGuid.PcdSpaEnable|FALSE
!endif

!if gSiPkgTokenSpaceGuid.PcdSmbiosEnable == TRUE
  gClientSmbiosFeaturePkgTokenSpaceGuid.PcdSmbiosFeatureEnable|TRUE
!endif

!if gPlatformModuleTokenSpaceGuid.PcdCapsuleEnable == TRUE
  gCapsuleFeaturePkgTokenSpaceGuid.PcdCapsuleFeatureEnable|TRUE
  gCapsuleFeaturePkgTokenSpaceGuid.PcdCheckArbhSvnBeforeUpdate|TRUE
  gUsbCCapsuleFeaturePkgTokenSpaceGuid.PcdUsbCCapsuleFeatureEnable|TRUE
!endif

  gTelemetryFeaturePkgTokenSpaceGuid.PcdTelemetryFeatureEnable|TRUE
  gCapsuleFeaturePkgTokenSpaceGuid.PcdTopSwapSmiSupport|FALSE

!if gBoardModuleTokenSpaceGuid.PcdAdvancedFeatureEnable == FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdPciReservedMemBase |0x80000000
  gMinPlatformPkgTokenSpaceGuid.PcdPciReservedMemLimit|0xC0000000
!endif

!if gPlatformModuleTokenSpaceGuid.PcdCapsuleEnable == TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSystemFmpCapsuleImageTypeIdGuid|{GUID(gFmpDeviceMonolithicDefaultGuid), GUID(gFmpDeviceBiosDefaultGuid)}
  gCapsuleFeaturePkgTokenSpaceGuid.PcdObbRecoveryRequiredFvGuid|{GUID(gFvUefiBootFvGuid), GUID(gFvOsBootFvGuid), GUID(gFvSecurityFvNameGuid), GUID(gFvAdvancedFvGuid), GUID(gFvUplFvGuid)}
  gCapsuleFeaturePkgTokenSpaceGuid.PcdNonFitPayloadRecoveryRequiredFvGuid|{GUID(gFvNonFitPayloadsFvNameGuid)}
!endif

#
# Overriding this PCD to reduce the NVRAM Reclaim frequency
#
!if gPlatformModuleTokenSpaceGuid.PcdNetworkEnable == TRUE
    gEfiNetworkPkgTokenSpaceGuid.PcdMaxIScsiAttemptNumber|0x04
!endif

!if gCapsuleFeaturePkgTokenSpaceGuid.PcdCapsuleFeatureEnable == TRUE
  gPantherLakeBinPkgTokenSpaceGuid.PcdObbSha384HashBinEnable|TRUE
  !if gPlatformModuleTokenSpaceGuid.PcdExtendedBiosRegionSupport == TRUE
    gPantherLakeBinPkgTokenSpaceGuid.PcdExtendedSha384HashBinEnable|TRUE
  !endif
  !if gCapsuleFeaturePkgTokenSpaceGuid.PcdBiosResiliencyEnable == TRUE
    gPantherLakeBinPkgTokenSpaceGuid.PcdIbbRBgslBinEnable|TRUE
    gPantherLakeBinPkgTokenSpaceGuid.PcdObbRBgslBinEnable|TRUE
    gPantherLakeBinPkgTokenSpaceGuid.PcdFitPayloadBgslBinEnable|TRUE
    !if gPlatformModuleTokenSpaceGuid.PcdExtendedBiosRegionSupport == TRUE
      gPantherLakeBinPkgTokenSpaceGuid.PcdExtendedBgslBinEnable|TRUE
    !endif
  !else
    # disable for now, will enable once simics support ready
    gCapsuleFeaturePkgTokenSpaceGuid.PcdNvmeRecoveryEnable|TRUE
  !endif
!endif

!if gCapsuleFeaturePkgTokenSpaceGuid.PcdBiosResiliencyEnable == TRUE
  #
  # Debug Resiliency BIOS had size limitation, disable the policy default check to increase the size capacity.
  #
  gBoardModuleTokenSpaceGuid.PcdPolicyDefaultChkEnable|FALSE
!endif

!if gTelemetryFeaturePkgTokenSpaceGuid.PcdTelemetryFeatureEnable == TRUE
  # gBdsAllDriversConnectedProtocolGuid
  gTelemetryFeaturePkgTokenSpaceGuid.PcdPlatformFviReadyProtocolGuid|{GUID({0xdbc9fd21, 0xfad8, 0x45b0, {0x9e, 0x78, 0x27, 0x15, 0x88, 0x67, 0xcc, 0x93}})}
!endif

  gPantherLakeBinPkgTokenSpaceGuid.PcdIntelGopVbtBinEnable|TRUE

  gIntelFsp2PkgTokenSpaceGuid.PcdGlobalDataPointerAddress|0xFED00148
  gIntelFsp2WrapperTokenSpaceGuid.PcdPeiMinMemSize|0x4800000

!if gSiPkgTokenSpaceGuid.PcdS3Enable == TRUE
  # @todo the size needs to be optimized.
  # Increase the memory size to resolve S3 exit failure due to memory resource shortage
  !if $(TARGET) == DEBUG
    gPlatformModuleTokenSpaceGuid.PcdS3AcpiReservedMemorySize|0x2600000
  !else
    gPlatformModuleTokenSpaceGuid.PcdS3AcpiReservedMemorySize|0x2600000
  !endif
!endif

!if gPlatformModuleTokenSpaceGuid.PcdNetworkEnable == TRUE
  gEfiNetworkPkgTokenSpaceGuid.PcdAllowHttpConnections|TRUE
!endif

#
# UnloadDriver - AFP CnvCompatability
#
!if gCnvFeaturePkgTokenSpaceGuid.PcdCnvFeatureEnable == TRUE
!if gCnvFeaturePkgTokenSpaceGuid.PcdCnvBinLoadFromESP == TRUE
  gCnvFeaturePkgTokenSpaceGuid.PcdUnloadDriverGuids|{GUID(gUndiDriverBinGuid), GUID(gWlanDriverBinGuid), GUID(gBluetoothHciImageGuid)}
  gCnvFeaturePkgTokenSpaceGuid.PcdCnvDeviceId|{UINT16(0xE340), UINT16(0xE341), UINT16(0xE342), UINT16(0xE343), UINT16(0xE440), UINT16(0xE441), UINT16(0xE442), UINT16(0xE443)}
!endif
!endif

#
# Set PCD for SpiFvbService driver to consume
#
!if gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable == TRUE
  # Signature: gEfiAuthenticatedVariableGuid
  gIntelSiliconPkgTokenSpaceGuid.PcdFlashVariableStoreType|1
!else
  # Signature: gEfiVariableGuid
  gIntelSiliconPkgTokenSpaceGuid.PcdFlashVariableStoreType|0
!endif
  ## This PCD decides how FSP is measured
  # 1) The BootGuard ACM may already measured the FSP component, such as FSPT/FSPM.
  # We need a flag (PCD) to indicate if there is need to do such FSP measurement or NOT.
  # 2) The FSP binary includes FSP code and FSP UPD region. The UPD region is considered
  # as configuration block, and it may be updated by OEM by design.
  # This flag (PCD) is to indicate if we need isolate the the UPD region from the FSP code region.
  # BIT0: Need measure FSP.  (for FSP 1.x) - reserved in FSP2.
  # BIT1: Need measure FSPT. (for FSP 2.x)
  # BIT2: Need measure FSPM. (for FSP 2.x)
  # BIT3: Need measure FSPS. (for FSP 2.x)
  # BIT4~30: reserved.
  # BIT31: Need isolate UPD region measurement.
  #   0: measure FSP[T|M|S] as one binary in one record (PCR0).
  #   1: measure FSP UPD region in one record (PCR1),
  #      measure the FSP code without UPD in another record (PCR0).
  #
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspMeasurementConfig|0x8000000F  #Enable FSP measurement by platform

  gUefiCpuPkgTokenSpaceGuid.PcdMaxMappingAddressBeforeTempRamExit|0x100000000


# Update PcdsFeatureFlag acording to PcdsFixedAtBuild changes
[PcdsFeatureFlag]
!if gPlatformModuleTokenSpaceGuid.PcdCapsuleEnable == TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSupportUpdateCapsuleReset|TRUE
!else
  gEfiMdeModulePkgTokenSpaceGuid.PcdSupportUpdateCapsuleReset|FALSE
!endif

!if gBoardModuleTokenSpaceGuid.PcdAcpiDebugFeatureEnable == TRUE
  gAcpiDebugFeaturePkgTokenSpaceGuid.PcdAcpiDebugFeatureEnable|TRUE
!else
  gAcpiDebugFeaturePkgTokenSpaceGuid.PcdAcpiDebugFeatureEnable|FALSE
!endif
  #
  # Watchdog Timer
  #
!if gSiPkgTokenSpaceGuid.PcdOverclockEnable == TRUE
  gWatchdogTimerFeaturePkgTokenSpaceGuid.PcdWatchdogTimerFeatureEnable|TRUE
!else
  gWatchdogTimerFeaturePkgTokenSpaceGuid.PcdWatchdogTimerFeatureEnable|FALSE
!endif

[PcdsDynamicExHii]
!if gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable == TRUE
  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|L"Timeout"|gEfiGlobalVariableGuid|0x0|1 # Variable: L"Timeout"
!endif

[PcdsDynamicExDefault]
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspsBaseAddress|0xFFD20000
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspmUpdDataAddress|0x00000000
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspsUpdDataAddress|0x00000000
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspmUpdDataAddress64|0x00000000
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspsUpdDataAddress64|0x00000000

!if $(TARGET) == DEBUG
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|TRUE
!else
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|FALSE
!endif

!if gBoardModuleTokenSpaceGuid.PcdAdvancedFeatureEnable == TRUE
  gMinPlatformPkgTokenSpaceGuid.PcdPciReservedMemBase |0x80000000
  gMinPlatformPkgTokenSpaceGuid.PcdPciReservedMemLimit|0xC0000000
!endif

!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  ## Specifies max supported number of Logical Processors.
  # @Prompt Configure max supported number of Logical Processorss
  gUefiCpuPkgTokenSpaceGuid.PcdCpuMaxLogicalProcessorNumber|16

  ## Specifies the AP wait loop state during POST phase.
  #  The value is defined as below.
  #  1: Place AP in the Hlt-Loop state.
  #  2: Place AP in the Mwait-Loop state.
  #  3: Place AP in the Run-Loop state.
  # @Prompt The AP wait loop state.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuApLoopMode|2

  ## Specifies the AP target C-state for Mwait during POST phase.
  #  The default value 0 means C1 state.
  #  The value is defined as below.<BR><BR>
  # @Prompt The specified AP target C-state for Mwait.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuApTargetCstate|0

  gSiPkgTokenSpaceGuid.PcdPciExpressRegionLength|0x10000000
!endif

!if gBoardModuleTokenSpaceGuid.PcdS4Enable == FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|FALSE
!else
  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|TRUE
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable == TRUE
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmInstanceGuid|{0x5a, 0xf2, 0x6b, 0x28, 0xc3, 0xc2, 0x8c, 0x40, 0xb3, 0xb4, 0x25, 0xe6, 0x75, 0x8b, 0x73, 0x17}
  gEfiSecurityPkgTokenSpaceGuid.PcdTcg2HashAlgorithmBitmap|0xFFFFFFFF
  gEfiSecurityPkgTokenSpaceGuid.PcdTpm2HashMask|0x00000000

  gEfiSecurityPkgTokenSpaceGuid.PcdTpm2InitializationPolicy|1
  gEfiSecurityPkgTokenSpaceGuid.PcdTpm2SelfTestPolicy|0
  gEfiSecurityPkgTokenSpaceGuid.PcdTpm2ScrtmPolicy|1

  gBoardModuleTokenSpaceGuid.PcdTpm2HierarchyChangeAuthPlatform|1
  gBoardModuleTokenSpaceGuid.PcdTpm2HierarchyControlPlatform|1
  gBoardModuleTokenSpaceGuid.PcdTpm2HierarchyControlEndorsement|1
  gBoardModuleTokenSpaceGuid.PcdTpm2HierarchyControlOwner|1
  gBoardModuleTokenSpaceGuid.PcdTpm2ChangeEps|0
  gBoardModuleTokenSpaceGuid.PcdTpm2ChangePps|0
  gBoardModuleTokenSpaceGuid.PcdTpm2Clear|0

  !if gSiPkgTokenSpaceGuid.PcdTxtEnable == TRUE
    ## The PCD is used to specify if Tcg2Platform module supports TXT provision.
    gBoardModuleTokenSpaceGuid.PcdTpm2TxtProvisionSupport|TRUE
    gBoardModuleTokenSpaceGuid.PcdTpm2TxtProvisionSHA384|FALSE
  !endif

!endif

!if gPlatformModuleTokenSpaceGuid.PcdTpmEnable == TRUE
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmInitializationPolicy|1
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmScrtmPolicy|1
!endif

!if gOneClickRecoveryFeaturePkgTokenSpaceGuid.PcdOneClickRecoveryEnable == TRUE
  ## This value is used to configure the request and response timeout when getting the recovery image from
  #  remote source. The value of 5000 will be the default 5 second timeout value
  # @Prompt HTTP Boot OS Image Request and Response Timeout
  gEfiNetworkPkgTokenSpaceGuid.PcdHttpIoTimeout|5000

  gEfiNetworkPkgTokenSpaceGuid.PcdHttpDnsRetryInterval|5
  gEfiNetworkPkgTokenSpaceGuid.PcdHttpDnsRetryCount|5
!endif

!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 0
  #
  # Include FSP PCD settings.
  #
  !include $(PLATFORM_FSP_BIN_PACKAGE)/FspPkgPcdShare.dsc
!endif

  #
  # FSP Binary base address will be set in FDF basing on flash map
  #
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspmBaseAddress|0
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspsBaseAddress|0
