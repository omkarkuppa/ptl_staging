## @file
#  Board description file initializes configuration (PCD) settings for the project.
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

[Packages]
  Usb3DebugFeaturePkg/Usb3DebugFeaturePkg.dec
  PostCodeDebugFeaturePkg/PostCodeDebugFeaturePkg.dec
  BeepDebugFeaturePkg/BeepDebugFeaturePkg.dec
  NhltFeaturePkg/NhltFeaturePkg.dec
  BeepPkg/BeepPkg.dec
  SndwDevTopologyFeaturePkg/SndwDevTopologyFeaturePkg.dec
  DmarAcpiFeaturePkg/DmarAcpiFeaturePkg.dec
  MebxFeaturePkg/MebxFeaturePkg.dec
  AmtPetAlertFeaturePkg/AmtPetAlertFeaturePkg.dec
  AmtMacPassThroughFeaturePkg/AmtMacPassThroughFeaturePkg.dec
  AmtPetInitFeaturePkg/AmtPetInitFeaturePkg.dec
  PsrFeaturePkg/PsrFeaturePkg.dec
  ClientSmbiosFeaturePkg/ClientSmbiosFeaturePkg.dec
  CnvFeaturePkg/CnvFeaturePkg.dec
  VtioFeaturePkg/VtioFeaturePkg.dec
  TmeFeaturePkg/TmeFeaturePkg.dec
  TseFeaturePkg/TseFeaturePkg.dec
  $(PLATFORM_BIN_PACKAGE)/$(PLATFORM_BIN_PACKAGE).dec
  $(COMMON_BIN_PACKAGE)/$(COMMON_BIN_PACKAGE).dec
  XmlCliFeaturePkg/XmlCliFeaturePkg.dec
  PmaxFeaturePkg/PmaxFeaturePkg.dec
  CapsuleFeaturePkg/CapsuleFeaturePkg.dec
  UsbCCapsuleFeaturePkg/UsbCCapsuleFeaturePkg.dec
  FspWrapperFeaturePkg/FspWrapperFeaturePkg.dec
  ProtectedVariableFeaturePkg/ProtectedVariableFeaturePkg.dec
!if gSiPkgTokenSpaceGuid.PcdOverclockEnable == TRUE
  WatchdogTimerFeaturePkg/WatchdogTimerFeaturePkg.dec
!endif
  OneClickRecoveryFeaturePkg/OneClickRecoveryFeaturePkg.dec
  PlatformSanitizeFeaturePkg/PlatformSanitizeFeaturePkg.dec
  MemoryTelemetryFeaturePkg/MemoryTelemetryFeaturePkg.dec
  TdxFeaturePkg/TdxFeaturePkg.dec

[PcdsFeatureFlag]
  ## This PCD specified whether ACPI SDT protocol is installed.
  # ROYAL_PARK_OVERRIDE: PcdInstallAcpiSdtProtocol
  gEfiMdeModulePkgTokenSpaceGuid.PcdInstallAcpiSdtProtocol|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreImageLoaderSearchTeSectionFirst|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdPciDegradeResourceForOptionRom|FALSE
  # Disabling PcdBrowserGrayOutTextStatement causes all empty/disabled rows active
  gEfiMdeModulePkgTokenSpaceGuid.PcdBrowserGrayOutTextStatement|TRUE

  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmEnableBspElection|TRUE

  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterDebugInit|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterMemInit|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable|FALSE
  # Build scripts override the value of this PCD, update value in scripts for the change to take effect.
  gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdStandaloneMmEnable|FALSE

  gBoardModuleTokenSpaceGuid.PcdSecurityEnable|FALSE
  gBoardModuleTokenSpaceGuid.PcdAdvancedFeatureEnable|FALSE
  gBoardModuleTokenSpaceGuid.PcdOptimizationEnable|FALSE
  gBoardModuleTokenSpaceGuid.PcdUefiShellEnable|FALSE

  gNhltFeaturePkgTokenSpaceGuid.PcdNhltFeatureEnable|FALSE
  gBeepModuleTokenSpaceGuid.PcdDxeBeepEnable|FALSE
  gBeepModuleTokenSpaceGuid.PcdPeiBeepEnable|FALSE
  gSndwDevTopologyFeaturePkgTokenSpaceGuid.PcdSndwDevTopologyFeatureEnable|FALSE
  gSndwDevTopologyFeaturePkgTokenSpaceGuid.PcdSndwDevTopologyBtSndwSupported|FALSE
  gSndwDevTopologyFeaturePkgTokenSpaceGuid.PcdSndwDevTopologyIDispSndwSupported|FALSE

  gDmarAcpiFeaturePkgTokenSpaceGuid.PcdDmarFeatureEnable|TRUE

  gMebxFeaturePkgTokenSpaceGuid.PcdMebxFeatureEnable|TRUE
  gUcsiFeaturePkgTokenSpaceGuid.PcdUcsiFeatureEnable|FALSE
  gUsbPortMapPkgTokenSpaceGuid.PcdUsbPortMapEnable|TRUE
  gAmtPetAlertFeaturePkgTokenSpaceGuid.PcdAmtPetAlertFeatureEnable|TRUE

  gAmtMacPassThroughFeaturePkgTokenSpaceGuid.PcdAmtMacPassThroughFeatureEnable|TRUE

  gAmtPetInitFeaturePkgTokenSpaceGuid.PcdAmtPetInitFeatureEnable|TRUE

  gPsrFeatureModuleTokenSpaceGuid.PcdPlatformServiceRecordSupport|FALSE

  gCrashLogFeaturePkgTokenSpaceGuid.PcdCrashLogFeatureEnable|TRUE

  gVtioFeaturePkgTokenSpaceGuid.PcdVtioFeatureEnable|FALSE
  gTmeFeaturePkgTokenSpaceGuid.PcdTmeFeatureEnable|TRUE

  # Total Storage Encryption
  gTseFeaturePkgTokenSpaceGuid.PcdTseFeatureEnable|TRUE

  # Tdx
  gTdxFeaturePkgTokenSpaceGuid.PcdTdxFeatureEnable|TRUE

  # XmlCli: Initialize Feature Pcd
  gXmlCliFeaturePkgTokenSpaceGuid.PcdXmlCliFeatureEnable|FALSE
  gXmlCliFeaturePkgTokenSpaceGuid.PcdStandaloneMmXmlCliEnable|FALSE

  # I2C
  gI2cFeaturePkgTokenSpaceGuid.PcdI2cFeatureEnable|TRUE
  gI2cFeaturePkgTokenSpaceGuid.PcdI2cPssEnable|TRUE
  gI2cFeaturePkgTokenSpaceGuid.PcdI2cTouchEnable|TRUE
  gI2cFeaturePkgTokenSpaceGuid.PcdI2cTouchCommonEnable|TRUE
  gI2cFeaturePkgTokenSpaceGuid.PcdI2cTouchVersion|0x1

  # UFS
  gUfsFeaturePkgTokenSpaceGuid.PcdUfsFeatureEnable|FALSE
  gUfsFeaturePkgTokenSpaceGuid.PcdUfsEraseSupport|FALSE

  # Platform Sanitize
  gPsFeatureModuleTokenSpaceGuid.PcdPlatformSanitizeFeatureEnable|TRUE

  # Nvme feature
  gNvmeFeaturePkgTokenSpaceGuid.PcdNvmeFeatureEnable|TRUE

  # THC
  gThcFeaturePkgTokenSpaceGuid.PcdThcMultipleVenFeatureSupport    |TRUE
  gThcFeaturePkgTokenSpaceGuid.PcdThcHidFeatureEnable |TRUE

[PcdsFeatureFlag.X64]
# Optimze Driver init time in FastBoot Mode
# If set PcdPs2KbdExtendedVerification to False, we can save 380 ms for Ps2KeyboardDxe driver initialize time
  gEfiMdeModulePkgTokenSpaceGuid.PcdPs2KbdExtendedVerification|FALSE
  #
  # Enabling PcdHiiOsRuntimeSupport casuses S4 failure due to
  # insuficient Runtime memory allocation
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdHiiOsRuntimeSupport|FALSE

[PcdsFixedAtBuild]
  #
  # Enable FV migration as default
  # Platform should build gEdkiiMigrationInfoGuid HOB to indicate which FVs
  # should be migrated
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdMigrateTemporaryRamFirmwareVolumes|TRUE
  gSiPkgTokenSpaceGuid.PcdAcpiEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdSmbiosEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdS3Enable|FALSE
  gSiPkgTokenSpaceGuid.PcdTcssSupport|FALSE
  gSiPkgTokenSpaceGuid.PcdBiosGuardEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdBiosGuardLegacyAcpi|TRUE
  gSiPkgTokenSpaceGuid.PcdTdxEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdBootGuardEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdDtprEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdAtaEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdOcWdtEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdLpssUartEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdIpuEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdPeiDisplayEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdVtdEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdDmiEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdPpmEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdLpssUartDebugEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdPpamEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdSpaEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdSpecialPoolEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdPsmiEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdVmdEnable|FALSE
  gProtectedVariableFeaturePkgTokenSpaceGuid.PcdProtectedVariableEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdStatusCodeUseTraceHub|TRUE
  gSiPkgTokenSpaceGuid.PcdTraceHubCatalogEnable|FALSE
  gSiPkgTokenSpaceGuid.PcdTraceHubDebugLibMaster|0x48
  gSiPkgTokenSpaceGuid.PcdTraceHubDebugLibChannel|0xE
  gSiPkgTokenSpaceGuid.PcdAcpiDebugEnableFlag|TRUE   # Acpi debug enable flag
  gPlatformModuleTokenSpaceGuid.PcdTerminalOverDebugEnable|TRUE
  gDptfFeaturePkgTokenSpaceGuid.PcdDptfFeatureEnable|TRUE
  gMipiCamFeaturePkgTokenSpaceGuid.PcdMipiCamFeatureEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdLpcIoDecodeRange|0x0010
  gSiPkgTokenSpaceGuid.PcdLpcIoEnableDecoding|0x7C03
  gSiPkgTokenSpaceGuid.PcdBootGuardPerformanceEnable|FALSE

  # Platform Sanitize
  gPsFeatureModuleTokenSpaceGuid.PcdPlatformSanitizeRpeSupport|TRUE
  gPsFeatureModuleTokenSpaceGuid.PcdPsSimulationSupport|FALSE

  # Nvme feature
  !if gNvmeFeaturePkgTokenSpaceGuid.PcdNvmeFeatureEnable == TRUE
    gNvmeFeaturePkgTokenSpaceGuid.PcdNvmePciScanEnable  |TRUE
    gNvmeFeaturePkgTokenSpaceGuid.PcdNvmePlnEnable      |TRUE
    !if gPsFeatureModuleTokenSpaceGuid.PcdPlatformSanitizeFeatureEnable == TRUE
      gNvmeFeaturePkgTokenSpaceGuid.PcdNvmeEraseEnable    |TRUE
    !else
      gNvmeFeaturePkgTokenSpaceGuid.PcdNvmeEraseEnable    |FALSE
    !endif
  !else
    gNvmeFeaturePkgTokenSpaceGuid.PcdNvmePciScanEnable  |FALSE
    gNvmeFeaturePkgTokenSpaceGuid.PcdNvmePlnEnable      |FALSE
    gNvmeFeaturePkgTokenSpaceGuid.PcdNvmeEraseEnable    |FALSE
  !endif

  #
  # 32MB TSEG.
  #
  gSiPkgTokenSpaceGuid.PcdTsegSize|0x2000000
  gSiPkgTokenSpaceGuid.PcdRegBarBaseAddress|0xF0000000

  gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection|0
  gSiPkgTokenSpaceGuid.PcdEnableFspmCompression|TRUE
  gSiPkgTokenSpaceGuid.PcdSignedFspEnable|FALSE

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
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspMeasurementConfig|0x80000008

  #
  # Default BootStage is set here.
  # Stage 1 - enable debug (system deadloop after debug init)
  # Stage 2 - mem init (system deadloop after mem init)
  # Stage 3 - boot to shell only
  # Stage 4 - boot to OS
  # Stage 5 - boot to OS with security boot enabled
  # Stage 6 - boot with advanced features
  # stage 7 - tuning
  #
  gMinPlatformPkgTokenSpaceGuid.PcdBootStage|6
  gMinPlatformPkgTokenSpaceGuid.PcdFspWrapperBootMode|TRUE
  gMinPlatformPkgTokenSpaceGuid.PcdMaxCpuSocketCount|1
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiAcpiReclaimMemorySize|0xA2
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiAcpiNvsMemorySize|0xE8
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiReservedMemorySize|0x3100
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiRtDataMemorySize|0x6E
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiRtCodeMemorySize|0x99
  #
  # The PCDs are used to control the Windows SMM Security Mitigations Table - Protection Flags
  #
  # BIT0: If set, expresses that for all synchronous SMM entries,SMM will validate that input and output buffers lie entirely within the expected fixed memory regions.
  # BIT1: If set, expresses that for all synchronous SMM entries, SMM will validate that input and output pointers embedded within the fixed communication buffer only refer to address ranges \
  #       that lie entirely within the expected fixed memory regions.
  # BIT2: Firmware setting this bit is an indication that it will not allow reconfiguration of system resources via non-architectural mechanisms.
  # BIT3-31: Reserved
  #
  gMinPlatformPkgTokenSpaceGuid.PcdWsmtProtectionFlags|0x07

  gMinPlatformPkgTokenSpaceGuid.PcdEnableCompressedFspNvsBuffer|TRUE

  gBoardModuleTokenSpaceGuid.PcdAcpiDebugFeatureEnable|FALSE
  gBoardModuleTokenSpaceGuid.PcdS4Enable|FALSE
  gBoardModuleTokenSpaceGuid.PcdIntelRaidEnable|FALSE
  gBoardModuleTokenSpaceGuid.PcdTerminalEnable|FALSE
  gBoardModuleTokenSpaceGuid.PcdDefaultBoardId|0x27
  gBoardModuleTokenSpaceGuid.PcdFspPeiGopDisable|FALSE
  # Build scripts override the value of this PCD, update value in scripts for the change to take effect.
  gBoardModuleTokenSpaceGuid.PcdSetupEnable|TRUE
  ## Crypto Modulization Feature Enabling
  gBoardModuleTokenSpaceGuid.PcdModularCryptoEnable|FALSE
  gBoardModuleTokenSpaceGuid.PcdSipkgBinaryEnable|FALSE
  gEcFeaturePkgTokenSpaceGuid.PcdEcEnable|TRUE
  gBoardModuleTokenSpaceGuid.PcdGbeFwUpdateEnable|FALSE

  # Multi-IBB support
  gMultiIbbFeaturePkgTokenSpaceGuid.PcdMultiIbbFeatureEnable|FALSE
  gMultiIbbFeaturePkgTokenSpaceGuid.PcdCacheMissDetectEnable|FALSE
  gMultiIbbFeaturePkgTokenSpaceGuid.PcdMultiIbbInfoCount|4

  gCmosFeaturePkgTokenSpaceGuid.PcdPlatformCmosAccessSupport|FALSE
  gPlatformModuleTokenSpaceGuid.PcdCapsuleEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdUsb3SerialStatusCodeEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdUserAuthenticationEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdPciHotplugEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdVirtualKeyboardEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdEbcEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdHddPasswordEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdNetworkEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdMouseEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdSinitAcmBinEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdScsiEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdJpgEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdUsbEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdNetworkIp6Enable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdNetworkIscsiEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdNetworkVlanEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdCryptoEnable|FALSE    # Current Smbios implementation needs this
  gPlatformModuleTokenSpaceGuid.PcdLzmaEnable|TRUE
  gPlatformModuleTokenSpaceGuid.PcdDxeCompressEnable|TRUE
  gPlatformModuleTokenSpaceGuid.PcdUserIdentificationEnable|FALSE

  gPlatformModuleTokenSpaceGuid.PcdSkipFspTempRamInitAndExit|FALSE
  gPlatformModuleTokenSpaceGuid.PcdOpalPasswordEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdMemoryTestEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdTpmEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdBeepStatusCodeEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdPostCodeStatusCodeEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdSymbolInReleaseEnable|FALSE
  gPlatformModuleTokenSpaceGuid.PcdExtendedBiosRegionSupport|FALSE
  gPlatformModuleTokenSpaceGuid.PcdRamDebugEnable|TRUE

  #
  # BIOS build switches configuration
  #
  gPlatformModuleTokenSpaceGuid.PcdPchLowMmioSize|0x0
  gPlatformModuleTokenSpaceGuid.PcdLMemBarAddress|(gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress - 0x10000000) # 0xD0000000
  gPlatformModuleTokenSpaceGuid.PcdGttMmAddress|(gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress - 0x11000000) # 0xCF000000
  gPlatformModuleTokenSpaceGuid.PcdSerialPortEnable|TRUE
  #
  # PlatformTemporaryMmioAssignmentBegin
  #
  # When gEfiPeiBootInRecoveryModePpiGuid is installed, below MMIO resource would be
  # temporarily assigned to NVME/AHCI host controller after FspSiInitDone and be released at EndOfPei.
  # Please take care of platform resource assignment to avoid conflicts.
  #
  gPlatformModuleTokenSpaceGuid.PcdAhciPeiMmioBase |(gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress - 0x30000000)   # 0xB0000000
  gPlatformModuleTokenSpaceGuid.PcdAhciPeiMmioLimit|(gPlatformModuleTokenSpaceGuid.PcdAhciPeiMmioBase + 0x1000000)     # 0xB1000000
  gPlatformModuleTokenSpaceGuid.PcdNvmeHcPeiMmioBase|(gPlatformModuleTokenSpaceGuid.PcdAhciPeiMmioLimit)               # 0xB1000000
  gPlatformModuleTokenSpaceGuid.PcdNvmeHcPeiMmioLimit|(gPlatformModuleTokenSpaceGuid.PcdNvmeHcPeiMmioBase + 0x1000000) # 0xB2000000

  #
  # PlatformTemporaryMmioAssignmentEnd
  #

  gSiPkgTokenSpaceGuid.PcdDeprecatedCryptoRemove|TRUE

  #
  # UEFI Replay Protected Variable Feature Support
  #

  !if (gProtectedVariableFeaturePkgTokenSpaceGuid.PcdProtectedVariableEnable == TRUE)
    gEfiProtectedVariablePkgTokenSpaceGuid.PcdProtectedVariableIntegrity|TRUE
    gEfiProtectedVariablePkgTokenSpaceGuid.PcdProtectedVariableConfidentiality|TRUE
  !endif

  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVendor|L"Intel"
  # Change PcdBootManagerMenuFile to point to UiApp
  gEfiMdeModulePkgTokenSpaceGuid.PcdBootManagerMenuFile|{ 0x8b, 0x7d, 0x9a, 0xd8, 0x16, 0xd0, 0x26, 0x4d, 0x93, 0xe3, 0xea, 0xb6, 0xb4, 0xd3, 0xb0, 0xa2 }
  gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreMaxPeiStackSize|0x80000
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxVariableSize|0x10000
  gEfiMdeModulePkgTokenSpaceGuid.PcdHwErrStorageSize|0x00000800
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxHardwareErrorVariableSize|0x400
  gEfiMdeModulePkgTokenSpaceGuid.PcdAriSupport|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdLoadModuleAtFixAddressEnable|$(TOP_MEMORY_ADDRESS)
  gEfiMdeModulePkgTokenSpaceGuid.PcdBrowserSubtitleTextColor|0x0
  gEfiMdeModulePkgTokenSpaceGuid.PcdBrowserFieldTextColor|0x01
  gEfiMdeModulePkgTokenSpaceGuid.PcdReclaimVariableSpaceAtEndOfDxe|TRUE
  # The bit width of data to be written to Port80 set to 16
  gEfiMdePkgTokenSpaceGuid.PcdPort80DataWidth|16

  #
  # Crypto Modulization
  #
  ## Crypto Modulization Scope
  gEfiCryptoPkgTokenSpaceGuid.PcdCryptoServiceFamilyEnable.HmacMd5.Family    | PCD_CRYPTO_SERVICE_ENABLE_FAMILY
  gEfiCryptoPkgTokenSpaceGuid.PcdCryptoServiceFamilyEnable.HmacSha1.Family   | PCD_CRYPTO_SERVICE_ENABLE_FAMILY
  gEfiCryptoPkgTokenSpaceGuid.PcdCryptoServiceFamilyEnable.HmacSha256.Family | PCD_CRYPTO_SERVICE_ENABLE_FAMILY
  gEfiCryptoPkgTokenSpaceGuid.PcdCryptoServiceFamilyEnable.Md4.Family        | PCD_CRYPTO_SERVICE_ENABLE_FAMILY
  gEfiCryptoPkgTokenSpaceGuid.PcdCryptoServiceFamilyEnable.Md5.Family        | PCD_CRYPTO_SERVICE_ENABLE_FAMILY
  gEfiCryptoPkgTokenSpaceGuid.PcdCryptoServiceFamilyEnable.Pkcs.Family       | PCD_CRYPTO_SERVICE_ENABLE_FAMILY
  gEfiCryptoPkgTokenSpaceGuid.PcdCryptoServiceFamilyEnable.Dh.Family         | PCD_CRYPTO_SERVICE_ENABLE_FAMILY
  gEfiCryptoPkgTokenSpaceGuid.PcdCryptoServiceFamilyEnable.Random.Family     | PCD_CRYPTO_SERVICE_ENABLE_FAMILY
  gEfiCryptoPkgTokenSpaceGuid.PcdCryptoServiceFamilyEnable.Rsa.Family        | PCD_CRYPTO_SERVICE_ENABLE_FAMILY
  gEfiCryptoPkgTokenSpaceGuid.PcdCryptoServiceFamilyEnable.Sha1.Family       | PCD_CRYPTO_SERVICE_ENABLE_FAMILY
  gEfiCryptoPkgTokenSpaceGuid.PcdCryptoServiceFamilyEnable.Sha256.Family     | PCD_CRYPTO_SERVICE_ENABLE_FAMILY
  gEfiCryptoPkgTokenSpaceGuid.PcdCryptoServiceFamilyEnable.Sha384.Family     | PCD_CRYPTO_SERVICE_ENABLE_FAMILY
  gEfiCryptoPkgTokenSpaceGuid.PcdCryptoServiceFamilyEnable.Sha512.Family     | PCD_CRYPTO_SERVICE_ENABLE_FAMILY
  gEfiCryptoPkgTokenSpaceGuid.PcdCryptoServiceFamilyEnable.X509.Family       | PCD_CRYPTO_SERVICE_ENABLE_FAMILY
  gEfiCryptoPkgTokenSpaceGuid.PcdCryptoServiceFamilyEnable.Tdes.Family       | PCD_CRYPTO_SERVICE_ENABLE_FAMILY
  gEfiCryptoPkgTokenSpaceGuid.PcdCryptoServiceFamilyEnable.Aes.Family        | PCD_CRYPTO_SERVICE_ENABLE_FAMILY
  gEfiCryptoPkgTokenSpaceGuid.PcdCryptoServiceFamilyEnable.Arc4.Family       | PCD_CRYPTO_SERVICE_ENABLE_FAMILY
  gEfiCryptoPkgTokenSpaceGuid.PcdCryptoServiceFamilyEnable.Sm3.Family        | PCD_CRYPTO_SERVICE_ENABLE_FAMILY
  gEfiCryptoPkgTokenSpaceGuid.PcdCryptoServiceFamilyEnable.Hkdf.Family       | PCD_CRYPTO_SERVICE_ENABLE_FAMILY
  gEfiCryptoPkgTokenSpaceGuid.PcdCryptoServiceFamilyEnable.Tls.Family        | PCD_CRYPTO_SERVICE_ENABLE_FAMILY
  gEfiCryptoPkgTokenSpaceGuid.PcdCryptoServiceFamilyEnable.TlsSet.Family     | PCD_CRYPTO_SERVICE_ENABLE_FAMILY
  gEfiCryptoPkgTokenSpaceGuid.PcdCryptoServiceFamilyEnable.TlsGet.Family     | PCD_CRYPTO_SERVICE_ENABLE_FAMILY

  gNhltFeaturePkgTokenSpaceGuid.NhltConfigurationByPcdEnabled|FALSE

!if gXmlCliFeaturePkgTokenSpaceGuid.PcdXmlCliFeatureEnable == TRUE
  # Set Pcd Value to Client Platform, Refer README for more valid options
  gXmlCliFeaturePkgTokenSpaceGuid.PcdPlatformXmlCli|0x1
  # XmlCli Feature Package Token Space defining Setup Values
  gXmlCliFeaturePkgTokenSpaceGuid.PcdXmlCliSupport|0x0
  gXmlCliFeaturePkgTokenSpaceGuid.PcdPublishSetupPgPtr|0x1
  gXmlCliFeaturePkgTokenSpaceGuid.PcdEnableXmlCliLite|0x0
!else
  gXmlCliFeaturePkgTokenSpaceGuid.PcdPlatformXmlCli|0x0
  gXmlCliFeaturePkgTokenSpaceGuid.PcdXmlCliSupport|0x0
  gXmlCliFeaturePkgTokenSpaceGuid.PcdPublishSetupPgPtr|0x0
  gXmlCliFeaturePkgTokenSpaceGuid.PcdEnableXmlCliLite|0x0
!endif

  ## Specifies the number of variable MTRRs reserved for OS use. The default number of
  #  MTRRs reserved for OS use is 0.
  # @Prompt Number of reserved variable MTRRs.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuNumberOfReservedVariableMtrrs|0x0
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmApSyncTimeout|10000
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmStackSize|0x20000

  ## Specifies if CPU features will be initialized during S3 resume.
  # @Prompt If CPU features will be initialized during S3 resume.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuFeaturesInitOnS3Resume|TRUE

  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciBusNumber|0x0
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciDeviceNumber|0x1F
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciFunctionNumber|0x2
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciEnableRegisterOffset|0x44
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoBarEnableMask|0x80
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciBarRegisterOffset|0x00
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPortBaseAddress|0x1800
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiPm1TmrOffset|0x08
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPortBaseAddressMask|0xFFFC

  gMinPlatformPkgTokenSpaceGuid.PcdFspDispatchModeUseFspPeiMain|TRUE

  gClientSmbiosFeaturePkgTokenSpaceGuid.PcdSmbiosFeatureEnable|TRUE

  gCapsuleFeaturePkgTokenSpaceGuid.PcdTopSwapSmiSupport|TRUE
  gCapsuleFeaturePkgTokenSpaceGuid.PcdCapsuleFeatureEnable|FALSE
  gCapsuleFeaturePkgTokenSpaceGuid.PcdBiosResiliencyEnable|FALSE
  gCapsuleFeaturePkgTokenSpaceGuid.PcdMeResiliencyEnable|FALSE
  gCapsuleFeaturePkgTokenSpaceGuid.PcdNvmeRecoveryEnable|FALSE
  gCapsuleFeaturePkgTokenSpaceGuid.PcdBiosExtenedRegionEnable|FALSE
  gCapsuleFeaturePkgTokenSpaceGuid.PcdIfwiCapsuleUpdateEnable|FALSE

  gTelemetryFeaturePkgTokenSpaceGuid.PcdTelemetryFeatureEnable|FALSE

  #
  # USB4 Feature Fix PCD Initiation
  #
  gUsb4FeaturePkgTokenSpaceGuid.PcdDTbtEnable|FALSE
  gUsb4FeaturePkgTokenSpaceGuid.PcdBoardDTbtControllerNumber|0x0
  gUsb4FeaturePkgTokenSpaceGuid.PcdIoeDTbtRpSupport|0x00000000
  gUsb4FeaturePkgTokenSpaceGuid.PcdPchDTbtRpSupport|0x00000000

  gUsbCCapsuleFeaturePkgTokenSpaceGuid.PcdUsbCCapsuleFeatureEnable|FALSE
  gUsbCCapsuleFeaturePkgTokenSpaceGuid.PcdMaxUsbCPdNumber|0x04

  gFspWrapperFeaturePkgTokenSpaceGuid.PcdFspWrapperFeatureSupport|TRUE
  gFspWrapperFeaturePkgTokenSpaceGuid.PcdFspWrapperResetVectorInFsp|FALSE

  gPlatformModuleTokenSpaceGuid.PcdMultiPdtUpdateEnable|TRUE
  gPlatformModuleTokenSpaceGuid.PcdPdtSensorControlEnable|TRUE

  gBoardModuleTokenSpaceGuid.PcdPolicyDefaultChkEnable|TRUE

  #
  # FADT table setup
  # Updated in BoardFadtConfig.h
  #
  gMinPlatformPkgTokenSpaceGuid.PcdFadtSmiCmd|0x0                                 # Minimize the usage of SMM to improve the platform security
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiEnableSwSmi|0x0                            # Minimize the usage of SMM to improve the platform security
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiDisableSwSmi|0x0                           # Minimize the usage of SMM to improve the platform security
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPm1AEventBlockAddress|0x1800               # EFI_ACPI_PM1A_EVT_BLK_ADDRESS
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPm1BEventBlockAddress|0x0                  # EFI_ACPI_PM1B_EVT_BLK_ADDRESS
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPm1AControlBlockAddress|0x1804             # EFI_ACPI_PM1A_CNT_BLK_ADDRESS
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPm1BControlBlockAddress|0x0                # EFI_ACPI_PM1B_CNT_BLK_ADDRESS
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPm2ControlBlockAddress|0x1850              # EFI_ACPI_PM2_CNT_BLK_ADDRESS
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiPmTimerBlockAddress|0x1808                 # EFI_ACPI_PM_TMR_BLK_ADDRESS
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiGpe0BlockAddress|0x1860                    # EFI_ACPI_GPE0_BLK_ADDRESS
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiGpe0BlockLength|0x20                       # EFI_ACPI_GPE0_BLK_LEN
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiGpe1BlockAddress|0x1810                    # EFI_ACPI_GPE1_BLK_ADDRESS
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiGpe1BlockLength|0x18                       # EFI_ACPI_GPE1_BLK_LEN
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiGpe1Base|0x80                              # EFI_ACPI_GPE1_BASE
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiXGpe1BlkRegisterBitWidth|0xC0              # EFI_ACPI_GPE1_BLK_BIT_WIDTH
  gMinPlatformPkgTokenSpaceGuid.PcdFadtDutyWidth|0x3                              # EFI_ACPI_DUTY_WIDTH

  # Set all GAR register AccessSize to valid value.
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiResetRegisterAccessSize|0x1                # EFI_ACPI_6_5_BYTE
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiXPm1aEvtBlkAccessSize|0x2                  # EFI_ACPI_6_5_WORD
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiXPm1bEvtBlkAccessSize|0x2                  # EFI_ACPI_6_5_WORD
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiXPm1aCntBlkAccessSize|0x2                  # EFI_ACPI_6_5_WORD
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiXPm1bCntBlkAccessSize|0x2                  # EFI_ACPI_6_5_WORD
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiXPm2CntBlkAccessSize|0x1                   # EFI_ACPI_6_5_BYTE
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiXPmTmrBlkAccessSize|0x3                    # EFI_ACPI_6_5_DWORD
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiXGpe0BlkAccessSize|0x1                     # EFI_ACPI_6_5_BYTE
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiXGpe1BlkAccessSize|0x1                     # EFI_ACPI_6_5_BYTE

  # Set the S4BiosReq to 0 as we don't support it in the FACS for PTL.
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiS4BiosReq|0x0

  # One Click Recovery related PCD's
  gOneClickRecoveryFeaturePkgTokenSpaceGuid.PcdOneClickRecoveryEnable|TRUE
  gOneClickRecoveryFeaturePkgTokenSpaceGuid.PcdWifiProfileSyncEnable|TRUE

  # Memory Telemetry Feature Enablement PCD
  gMemoryTelemetryFeaturePkgTokenSpaceGuid.PcdMemoryTelemetryEnabled|TRUE
  gMemoryTelemetryFeaturePkgTokenSpaceGuid.PcdPprCapability|TRUE

  # Connectivity
  gCnvFeaturePkgTokenSpaceGuid.PcdCnvFeatureEnable|FALSE
  gCnvFeaturePkgTokenSpaceGuid.PcdCnvBinLoadFromESP|FALSE

  # PCD value to determine formset display location
  #
  # Bit 31    --- Whether to show formset, 1 - show, 0 - hide
  # Bit 30:24 --- Reserved
  # Bit 23:16 --- Formset display order
  # Bit 15:0  --- The id of form to display formset under Advanced menu
  #
  #-----------------------------------------------------------------------
  #|               Form id               |         Display order         |
  #-----------------------------------------------------------------------
  #| ADVANCED_MAIN          0x1001(4097) | XmlCli                   0x01 |
  #|                                     | CNV                      0x02 |
  #|                                     |                               |
  #-----------------------------------------------------------------------
  #| MANAGEABILITY_FORM_ID  0x1018(4120) | Platform Sanitize        0x01 |
  #|                                     | Platform Service Record  0x02 |
  #|                                     |                               |
  #|----------------------------------------------------------------------
  gPsFeatureModuleTokenSpaceGuid.PcdPsSetupConfiguration|0x80011018
  gPsrFeatureModuleTokenSpaceGuid.PcdPsrSetupConfiguration|0x80021018

  #
  # Trace Hub Debug setting
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdTraceHubDebugLevel|0x3
  gEfiMdeModulePkgTokenSpaceGuid.PcdEnableTraceHubDebugMsg|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdTraceHubDebugMmioAddress|0xFE270380

  # PCDs for MAX PCH USB2 Port Number, PCH USB3 Port Number
  gBoardModuleTokenSpaceGuid.PcdMaxPchUsb2PortNumber     | 8
  gBoardModuleTokenSpaceGuid.PcdMaxPchUsb3PortNumber     | 2

  gSiPkgTokenSpaceGuid.PcdSafBarBaseAddress|0xFA000000
  gBoardModuleTokenSpaceGuid.PcdMemoryDiagnosticTestEnable|TRUE

[PcdsFixedAtBuild.IA32]
  gEfiMdeModulePkgTokenSpaceGuid.PcdVpdBaseAddress|0x0
  gMinPlatformPkgTokenSpaceGuid.PcdPeiPhaseStackTop|0xA0000

[PcdsFixedAtBuild.X64]
  # Default platform supported RFC 4646 languages: (American) English
  gEfiMdePkgTokenSpaceGuid.PcdUefiVariableDefaultPlatformLangCodes|"en-US"

[PcdsPatchableInModule]
  gEfiMdeModulePkgTokenSpaceGuid.PcdSmbiosVersion|0x0308

  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80000046

[PcdsDynamicExHii]
  gPlatformModuleTokenSpaceGuid.PcdPlatformMemoryCheck|L"MemoryCheck"|gPlatformModuleTokenSpaceGuid|0x0|0
  gPlatformModuleTokenSpaceGuid.PcdComPortAttributes0IsEnabled|L"ComAttributes"|gSetupVariableGuid|0x16|0
  gEfiMdePkgTokenSpaceGuid.PcdDefaultTerminalType|L"ComAttributes"|gSetupVariableGuid|0x13|0
  gEfiSecurityPkgTokenSpaceGuid.PcdTpm2AcpiTableRev|L"TCG2_VERSION"|gTcg2ConfigFormSetGuid|0x8|4|NV,BS
  gPlatformModuleTokenSpaceGuid.PcdFastBootEnable|L"FastBootEnable"|gPlatformModuleTokenSpaceGuid|0x1|0
  gCapsuleFeaturePkgTokenSpaceGuid.PcdSystemFirmwareFmpSupportedMode|L"SysFmpMode"|gSysFwUpdateProgressGuid|0x0|0xFF|NV,BS
  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|L"Timeout"|gEfiGlobalVariableGuid|0x0|5 # Variable: L"Timeout"
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutColumn|L"ConOutConfig"|gPlatformModuleTokenSpaceGuid|0x0|100
  gEfiMdeModulePkgTokenSpaceGuid.PcdSetupConOutRow|L"ConOutConfig"|gPlatformModuleTokenSpaceGuid|0x4|31
  gEfiMdePkgTokenSpaceGuid.PcdHardwareErrorRecordLevel|L"HwErrRecSupport"|gEfiGlobalVariableGuid|0x0|1 # Variable: L"HwErrRecSupport"

[PcdsDynamicExDefault]
  gEfiSecurityPkgTokenSpaceGuid.PcdCRBIdleByPass|0xFF
  ## This PCD defines initial setting of TCG2 Persistent Firmware Management Flags
  # Currently enabled flags:
  # TCG2_BIOS_TPM_MANAGEMENT_FLAG_PP_REQUIRED_FOR_TURN_OFF                (BIT5)
  # TCG2_BIOS_TPM_MANAGEMENT_FLAG_PP_REQUIRED_FOR_CHANGE_EPS              (BIT6)
  # TCG2_BIOS_TPM_MANAGEMENT_FLAG_PP_REQUIRED_FOR_CHANGE_PCRS             (BIT7)
  # TCG2_BIOS_STORAGE_MANAGEMENT_FLAG_PP_REQUIRED_FOR_DISABLE_BLOCK_SID   (BIT17)
  # @Prompt Initial setting of TCG2 Persistent Firmware Management Flags
  gEfiSecurityPkgTokenSpaceGuid.PcdTcg2PhysicalPresenceFlags|0x200E0
  gEfiSecurityPkgTokenSpaceGuid.PcdTpm2CurrentIrqNum|0x00
  gEfiSecurityPkgTokenSpaceGuid.PcdFirmwareDebuggerInitialized|FALSE
  gEfiSecurityPkgTokenSpaceGuid.PcdSkipHddPasswordPrompt|FALSE
  gEfiSecurityPkgTokenSpaceGuid.PcdSkipOpalPasswordPrompt|FALSE

  gEfiMdeModulePkgTokenSpaceGuid.PcdSrIovSupport|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|0x0
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|0x0
  gEfiMdeModulePkgTokenSpaceGuid.PcdAtaSmartEnable|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdConInConnectOnDemand|FALSE
  #
  #  Set video to native resolution as Windows 8 WHCK requirement.
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|0x0
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|0x0
  ## This is recover file name in PEI phase.
  #  The file must be in the root directory.
  #  The file name must be the 8.3 format.
  #  The PCD data must be in UNICODE format.
  # @Prompt Recover file name in PEI phase
  gEfiMdeModulePkgTokenSpaceGuid.PcdRecoveryFileName|L"Capsule.cap"|VOID*|0x20
  ## Default OEM Table ID for ACPI table creation, it is "EDK2    ".
  #  According to ACPI specification, this field is particularly useful when
  #  defining a definition block to distinguish definition block functions.
  #  The OEM assigns each dissimilar table a new OEM Table ID.
  #  This PCD is ignored for definition block.
  # @Prompt Default OEM Table ID for ACPI table creation.
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemTableId|0x4449204C45544E49 # "INTEL ID"

  gMinPlatformPkgTokenSpaceGuid.PcdPciReservedMemAbove4GBBase  |0xFFFFFFFFFFFFFFFF
  gMinPlatformPkgTokenSpaceGuid.PcdPciReservedMemAbove4GBLimit |0x0000000000000000
  gMinPlatformPkgTokenSpaceGuid.PcdPciSegmentCount|0x1
  gMinPlatformPkgTokenSpaceGuid.PcdFadtPreferredPmProfile|0x02

  #
  # FADT table setup
  # Updated in BoardFadtConfig.h
  #
  gMinPlatformPkgTokenSpaceGuid.PcdFadtIaPcBootArch|0x0001        # EFI_ACPI_IAPC_BOOT_ARCH
  gMinPlatformPkgTokenSpaceGuid.PcdFadtFlags|0x000086A5

  gCapsuleFeaturePkgTokenSpaceGuid.PcdTopSwapEnableSwSmi|0xFF
  gCapsuleFeaturePkgTokenSpaceGuid.PcdTopSwapDisableSwSmi|0xFF
  gCapsuleFeaturePkgTokenSpaceGuid.PcdNvmeRecoveryPrimarySlotPortNumber|0
  gCapsuleFeaturePkgTokenSpaceGuid.PcdNvmeRecoverySupported|0

  ## The mask is used to control VTd behavior.<BR><BR>
  #  BIT0: Enable IOMMU during boot (If DMAR table is installed in DXE. If VTD_INFO_PPI is installed in PEI.)
  #  BIT1: Enable IOMMU when transfer control to OS (ExitBootService in normal boot. EndOfPEI in S3)
  #  BIT2: Force no IOMMU access attribute request recording before DMAR table is installed.
  # @Prompt The policy for VTd driver behavior.
  gIntelSiliconPkgTokenSpaceGuid.PcdVTdPolicyPropertyMask|0x00000004

  gProtectedVariableFeaturePkgTokenSpaceGuid.PcdRpmcSupported|FALSE

  gIntelSiliconPkgTokenSpaceGuid.PcdVTdSupportAbortDmaMode|TRUE

  gEfiMdeModulePkgTokenSpaceGuid.PcdS3BootScriptTablePrivateDataPtr|0x0
  gEfiMdeModulePkgTokenSpaceGuid.PcdFastPS2Detection|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdPciExpressRegionLength|0x10000000

  #
  # Some of the PCD consumed by both FSP and bootloader should be defined
  # here for bootloader to consume.
  #

  gBoardModuleTokenSpaceGuid.PcdBoardGpioTablePreMem|{0x0}
  gBoardModuleTokenSpaceGuid.PcdBoardGpioTablePreMem.GpioConfig[20].GpioPad|0x0
  gBoardModuleTokenSpaceGuid.PcdBoardGpioTable|{0x0}

  gBoardModuleTokenSpaceGuid.PcdBoardGpioTable.GpioConfig[100].GpioPad|0x0
  gBoardModuleTokenSpaceGuid.PcdBoardGpioTableEarlyPreMem|{0x0}
  gBoardModuleTokenSpaceGuid.PcdBoardGpioTableEarlyPreMem.GpioConfig[20].GpioPad|0x0
  gBoardModuleTokenSpaceGuid.PcdBoardAcpiData|{0x0}

[PcdsDynamicExVpd.common.DEFAULT]
  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTablePreMem| * |{CODE({
    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTable| * |{CODE({
    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTableEarlyPreMem| * |{CODE({
    {0x0}  // terminator
  })}

  # PCIE CLOCK USAGE FOR PTL H
  gBoardModuleTokenSpaceGuid.VpdPcdPcieClkUsageMap| * |{CODE(
    {0x0}
  )}
  # PCIE CLOCK USAGE FOR PTL P
  gBoardModuleTokenSpaceGuid.VpdPcdPPcieClkUsageMap| * |{CODE(
    {0x0}
  )}


  gBoardModuleTokenSpaceGuid.VpdPcdMrcSpdData| * |{CODE(
    {0x0}
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdMrcDqsMapCpu2Dram| * |{CODE(
    {0x0}
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdMrcDqMapCpu2Dram| * |{CODE(
    {0x0}
  )}

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
  // Connectable, Visibility, ConnectorType, UsbOcPinType, UsbOcPin, Usb2Controller, Usb2PortNum, Usb3Controller, Usb3PortNum
    {0x0}  // terminator

  })}
  gBoardModuleTokenSpaceGuid.VpdPcdCvsUsbConnector| * |{CODE(
  {
  // Connectable, Visibility, ConnectorType, UsbOcPinType, UsbOcPin, Usb2Controller, Usb2PortNum, Usb3Controller, Usb3PortNum
    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdUsbCConnector| * |{CODE(
  {
    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdCvsUsbCConnector| * |{CODE(
  {
    {0x0}  // terminator
  })}

  # CascadedRetimerFlashIndex, FirstUsbCConnectorIndex, SecondUsbCConnectorIndex
  gBoardModuleTokenSpaceGuid.VpdPcdUsbCRetimerFlash| * |{CODE(
  {
    {0x0}  // terminator
  })}

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
