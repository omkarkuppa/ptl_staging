## @file
#  PCD configuration build description file for the PantherLakeURvp board.
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
[Packages]
  $(PLATFORM_BIN_PACKAGE)/$(PLATFORM_BIN_PACKAGE).dec

################################################################################
#
# Pcd Section - list of all PCD Entries used by this board.
#
################################################################################

[PcdsFixedAtBuild.common]
  ######################################
  # Key Boot Stage and FSP configuration
  ######################################
  #
  # Please select the Boot Stage here.
  # Stage 1 - enable debug (system deadloop after debug init)
  # Stage 2 - mem init (system deadloop after mem init)
  # Stage 3 - boot to shell only
  # Stage 4 - boot to OS
  # Stage 5 - boot to OS with security boot enabled
  # Stage 6 - boot with advanced features enabled
  #
  gMinPlatformPkgTokenSpaceGuid.PcdBootStage|5

  #
  # 0: FSP Wrapper is running in Dispatch mode.
  # 1: FSP Wrapper is running in API mode.
  # Note: Dispatch mode is currently NOT supported for this board.
  #
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection|0

  #
  # FALSE: The board is not a FSP wrapper (FSP binary not used)
  # TRUE:  The board is a FSP wrapper (FSP binary is used)
  #
  gMinPlatformPkgTokenSpaceGuid.PcdFspWrapperBootMode|TRUE

  #
  # FSP Base address PCD will be updated in FDF basing on flash map.
  #
  gIntelFsp2WrapperTokenSpaceGuid.PcdFsptBaseAddress|0
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspmBaseAddress|0

  gIntelFsp2PkgTokenSpaceGuid.PcdTemporaryRamBase|0xFEF00000
  gIntelFsp2PkgTokenSpaceGuid.PcdTemporaryRamSize|0x00080000
  gSiPkgTokenSpaceGuid.PcdTsegSize|0x1000000

  ######################################
  # Silicon Configuration
  ######################################
  # SA
  gSiPkgTokenSpaceGuid.PcdIgdEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdPeiDisplayEnable|TRUE


  # Others
  gSiPkgTokenSpaceGuid.PcdAcpiEnable|TRUE
  gSiPkgTokenSpaceGuid.PcdLpcIoDecodeRange|0x0010
  gSiPkgTokenSpaceGuid.PcdLpcIoEnableDecoding|0x3C03

  #
  # When sharing stack with boot loader, FSP only needs a small temp ram for heap
  #
!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  gIntelFsp2PkgTokenSpaceGuid.PcdFspTemporaryRamSize|0x00020000
!else
      #
      # FSP Dispatch mode will not establish separate Stack or Heap.
      #
  gIntelFsp2PkgTokenSpaceGuid.PcdFspTemporaryRamSize|0
!endif

  gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress|0xE0000000
  gMinPlatformPkgTokenSpaceGuid.PcdPciExpressRegionLength|0x10000000
  gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreMaxPeiStackSize|0x40000
  gPantherLakeBinPkgTokenSpaceGuid.PcdMicrocodeBinEnable|TRUE          #Microcode
  gPantherLakeBinPkgTokenSpaceGuid.PcdIntelGopBinEnable|TRUE
  gBoardModuleTokenSpaceGuid.PcdFspWrapperResetVectorInFsp|TRUE

!if $(TARGET) == DEBUG
  gSiPkgTokenSpaceGuid.PcdLpssUartNumber|0
  gSiPkgTokenSpaceGuid.PcdLpssUartDebugEnable|TRUE
!endif

  gSiPkgTokenSpaceGuid.PcdSmbusBaseAddress|0x0000EFA0

[PcdsFeatureFlag.common]
  ######################################
  # Edk2 Configuration
  ######################################
  gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreImageLoaderSearchTeSectionFirst|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdInstallAcpiSdtProtocol|TRUE

  ######################################
  # Platform Configuration
  ######################################
  gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterDebugInit|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdStopAfterMemInit|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable|FALSE
  gMinPlatformPkgTokenSpaceGuid.PcdSerialTerminalEnable|TRUE
  gMinPlatformPkgTokenSpaceGuid.PcdStandaloneMmEnable|FALSE

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
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 4
  gMinPlatformPkgTokenSpaceGuid.PcdBootToShellOnly|FALSE
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 5
  gMinPlatformPkgTokenSpaceGuid.PcdUefiSecureBootEnable|TRUE
  gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable|TRUE
!endif

  gBoardModuleTokenSpaceGuid.PcdAdvancedFeatureEnable|FALSE

[PcdsFixedAtBuild.common]
  ######################################
  # Edk2 Configuration
  ######################################
!if $(TARGET) == RELEASE
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x0
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x3
!else
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2F
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x07
!endif

!if $(TARGET) == RELEASE
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|FALSE
!else
  gEfiMdeModulePkgTokenSpaceGuid.PcdStatusCodeUseSerial|TRUE
!endif
  gEfiMdeModulePkgTokenSpaceGuid.PcdAriSupport|FALSE
  gEfiMdeModulePkgTokenSpaceGuid.PcdBrowserFieldTextColor|0x01
  gEfiMdeModulePkgTokenSpaceGuid.PcdBrowserSubtitleTextColor|0x0
  gEfiMdeModulePkgTokenSpaceGuid.PcdHwErrStorageSize|0x00000800
  gEfiMdeModulePkgTokenSpaceGuid.PcdLoadModuleAtFixAddressEnable|$(TOP_MEMORY_ADDRESS)
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxHardwareErrorVariableSize|0x400
  gEfiMdeModulePkgTokenSpaceGuid.PcdMaxVariableSize|0x10000
  gEfiMdeModulePkgTokenSpaceGuid.PcdReclaimVariableSpaceAtEndOfDxe|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdSrIovSupport|FALSE
!if $(TARGET) == DEBUG
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialUseHardwareFlowControl|FALSE
!endif

  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoBarEnableMask|0x80
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciBarRegisterOffset|0x00
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciDeviceNumber|0x1F
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciEnableRegisterOffset|0x44
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPciFunctionNumber|0x2
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPortBaseAddress|0x1800
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiIoPortBaseAddressMask|0xFFFC
  gPcAtChipsetPkgTokenSpaceGuid.PcdAcpiPm1TmrOffset|0x08

  gMinPlatformPkgTokenSpaceGuid.PcdAcpiResetRegisterAccessSize|0x01
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiXPm1aEvtBlkAccessSize|0x02
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiXPm1bEvtBlkAccessSize|0x02
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiXPm1aCntBlkAccessSize|0x02
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiXPm1bCntBlkAccessSize|0x02
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiXPm2CntBlkAccessSize|0x01
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiXPmTmrBlkAccessSize|0x03
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiXGpe0BlkAccessSize|0x01
  gMinPlatformPkgTokenSpaceGuid.PcdAcpiXGpe1BlkAccessSize|0x01

  # Specifies timeout value in microseconds for the BSP to detect all APs for the first time.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuNumberOfReservedVariableMtrrs|0x0
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmApSyncTimeout|10000
  gUefiCpuPkgTokenSpaceGuid.PcdCpuSmmStackSize|0x20000

  #
  # In non-FSP build (EDK2 build) or FSP API mode below PCD are FixedAtBuild
  # (They will be DynamicEx in FSP Dispatch mode)
  #


  ## Specifies the AP wait loop state during POST phase.
  #  The value is defined as below.
  #  1: Place AP in the Hlt-Loop state.
  #  2: Place AP in the Mwait-Loop state.
  #  3: Place AP in the Run-Loop state.
  # @Prompt The AP wait loop state.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuApLoopMode|2

  ######################################
  # Platform Configuration
  ######################################
  gMinPlatformPkgTokenSpaceGuid.PcdMaxCpuSocketCount|1

  gMinPlatformPkgTokenSpaceGuid.PcdPciReservedMemBase |0x80800000
  gMinPlatformPkgTokenSpaceGuid.PcdPciReservedMemLimit|0xBFFFFFFF
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

  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiAcpiReclaimMemorySize|0xCC
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiAcpiNvsMemorySize|0xA2
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiReservedMemorySize|0x3100
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiRtDataMemorySize|0x6E
  gMinPlatformPkgTokenSpaceGuid.PcdPlatformEfiRtCodeMemorySize|0xC4

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage == 1
  gMinPlatformPkgTokenSpaceGuid.PcdTestPointIbvPlatformFeature|{0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage == 2
  gMinPlatformPkgTokenSpaceGuid.PcdTestPointIbvPlatformFeature|{0x03, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage == 3
  gMinPlatformPkgTokenSpaceGuid.PcdTestPointIbvPlatformFeature|{0x03, 0x07, 0x03, 0x05, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage == 4
  gMinPlatformPkgTokenSpaceGuid.PcdTestPointIbvPlatformFeature|{0x03, 0x07, 0x03, 0x05, 0x1F, 0x00, 0x0F, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage == 5
  gMinPlatformPkgTokenSpaceGuid.PcdTestPointIbvPlatformFeature|{0x03, 0x0F, 0x07, 0x1F, 0x1F, 0x0F, 0x0F, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdBootStage >= 6
  gMinPlatformPkgTokenSpaceGuid.PcdTestPointIbvPlatformFeature|{0x03, 0x0F, 0x07, 0x1F, 0x1F, 0x0F, 0x0F, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
!endif

gPantherLakeBinPkgTokenSpaceGuid.PcdIntelGopVbtBinEnable|TRUE
  ######################################
  # Edk2 Configuration
  ######################################
  gIntelFsp2PkgTokenSpaceGuid.PcdGlobalDataPointerAddress|0xFED00148
  gIntelFsp2WrapperTokenSpaceGuid.PcdPeiMinMemSize|0x3800000

  ######################################
  # Platform Configuration
  ######################################
  gMinPlatformPkgTokenSpaceGuid.PcdPeiPhaseStackTop|0xA0000

  ######################################
  # Edk2 Configuration
  ######################################

  # Default platform supported RFC 4646 languages: (American) English
  gEfiMdePkgTokenSpaceGuid.PcdUefiVariableDefaultPlatformLangCodes|"en-US"

[PcdsPatchableInModule.common]
  ######################################
  # Edk2 Configuration
  ######################################
  gEfiMdeModulePkgTokenSpaceGuid.PcdSmbiosVersion|0x0309
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80000046

[PcdsDynamicDefault]
  ######################################
  # Silicon Configuration
  ######################################
!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  gSiPkgTokenSpaceGuid.PcdPciExpressRegionLength|gMinPlatformPkgTokenSpaceGuid.PcdPciExpressRegionLength
!endif

  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutColumn|0x0
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutRow|0x0


  gEfiSecurityPkgTokenSpaceGuid.PcdTpm2CurrentIrqNum|0x00

  #
  # FSP Base address PCD will be updated in FDF basing on flash map.
  #
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspsBaseAddress|0

  # Platform will pre-allocate UPD buffer and pass it to FspWrapper
  # Those dummy address will be patched before FspWrapper executing
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspmUpdDataAddress64|0x00000000
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspsUpdDataAddress64|0x00000000

  gUefiCpuPkgTokenSpaceGuid.PcdCpuMaxLogicalProcessorNumber|16

!if gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable == TRUE
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmInitializationPolicy|1
  gEfiSecurityPkgTokenSpaceGuid.PcdTpmInstanceGuid|{0x5a, 0xf2, 0x6b, 0x28, 0xc3, 0xc2, 0x8c, 0x40, 0xb3, 0xb4, 0x25, 0xe6, 0x75, 0x8b, 0x73, 0x17}

  gEfiSecurityPkgTokenSpaceGuid.PcdTcg2HashAlgorithmBitmap|0xFFFFFFFF
  gEfiSecurityPkgTokenSpaceGuid.PcdTpm2HashMask|0x0000001F
  gEfiSecurityPkgTokenSpaceGuid.PcdTpm2SelfTestPolicy|0
  gEfiSecurityPkgTokenSpaceGuid.PcdTpm2ScrtmPolicy|1
!endif
[PcdsDynamicHii.X64.DEFAULT]
  ######################################
  # Edk2 Configuration
  ######################################
  gEfiMdePkgTokenSpaceGuid.PcdHardwareErrorRecordLevel|L"HwErrRecSupport"|gEfiGlobalVariableGuid|0x0|1 # Variable: L"HwErrRecSupport"

[PcdsDynamicExHii]
  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|L"Timeout"|gEfiGlobalVariableGuid|0x0|5 # Variable: L"Timeout"

[PcdsDynamicExDefault]

!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 0
  #
  # Include FSP PCD settings.
  #
  !include $(PLATFORM_FSP_BIN_PACKAGE)/FspPkgPcdShare.dsc
!endif

  ## Indicates processor trace is enabled on BSP only or not when CPU processor trace is enabled.<BR><BR>
  #  TRUE  - CPU processor trace is enabled on BSP only.<BR>
  #  FASLE - CPU processor trace is enabled on all CPU.<BR>
  # @Prompt Enable CPU processor trace only on BSP or all CPU.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuProcTraceBspOnly|FALSE

  ## Indicates if enable performance collecting when CPU processor trace is enabled.<BR><BR>
  #  CYC/TSC timing packets will be generated to collect performance data if this PCD is TRUE.<BR><BR>
  # @Prompt Enable performance collecting when processor trace is enabled.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuProcTracePerformanceCollecting|FALSE

  #
  #  GPIO table
  #
  gBoardModuleTokenSpaceGuid.PcdBoardGpioTablePreMem|{0x0}
  gBoardModuleTokenSpaceGuid.PcdBoardGpioTablePreMem.GpioConfig[20].GpioPad|0x0
  gBoardModuleTokenSpaceGuid.PcdBoardGpioTable|{0x0}
  gBoardModuleTokenSpaceGuid.PcdBoardGpioTable.GpioConfig[100].GpioPad|0x0

  #
  # EDK/FSP dispatch mode switches PcdAcpiS3Enable dynamically on gSiPkgTokenSpaceGuid.PcdS3Enable setting.
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiS3Enable|0

[PcdsDynamicExVpd.common.DEFAULT]
  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTablePreMem| * |{CODE({
    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdBoardGpioTable| * |{CODE({
    {0x0}  // terminator
  })}

  gBoardModuleTokenSpaceGuid.VpdPcdPPcieClkUsageMap| * |{CODE(
    {0x0}
  )}

  gBoardModuleTokenSpaceGuid.VpdPcdPcieClkUsageMap| * |{CODE(
    {0x0}
  )}
