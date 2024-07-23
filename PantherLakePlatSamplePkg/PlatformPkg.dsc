## @file
#  Platform description.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2020 Intel Corporation.
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
#  @par Specification
##

[Defines]
  #
  # Set platform specific package/folder name, same as passed from PREBUILD script.
  # PLATFORM_FULL_PACKAGE would be the same as PLATFORM_NAME as well as package build folder
  # DEFINE only takes effect at R9 DSC and FDF.
  #
  DEFINE      PLATFORM_PACKAGE                = MinPlatformPkg
  DEFINE      PLATFORM_FULL_PACKAGE           = PantherLakePlatSamplePkg
  DEFINE      PLATFORM_SI_PACKAGE             = OneSiliconPkg
  DEFINE      SILICON_PRODUCT_PATH            = OneSiliconPkg/Product/PantherLake
  DEFINE      PLATFORM_FSP_BIN_PACKAGE        = PantherLakeFspBinPkg
  DEFINE      PLATFORM_BOARD_PACKAGE          = PantherLakeBoardPkg
  DEFINE      PLATFORM_BSP_PATH               = PantherLakeBoardPkg/BoardSupport
  DEFINE      PLATFORM_FEATURES_PATH          = $(PLATFORM_FULL_PACKAGE)/Features

  #
  # Platform On/Off features are defined here
  #
  !include $(PLATFORM_BOARD_PACKAGE)\BoardPkgConfigDefault.dsc
  !include $(PLATFORM_BOARD_PACKAGE)\BoardPkgConfig.dsc

[PcdsFixedAtBuild]

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                       = $(PLATFORM_FULL_PACKAGE)
  PLATFORM_GUID                       = 465B0A0B-7AC1-443b-8F67-7B8DEC145F90
  PLATFORM_VERSION                    = 0.1
  DSC_SPECIFICATION                   = 0x00010005
  OUTPUT_DIRECTORY                    = Build/$(PLATFORM_FULL_PACKAGE)
  SUPPORTED_ARCHITECTURES             = IA32|X64
  BUILD_TARGETS                       = DEBUG|RELEASE
  SKUID_IDENTIFIER                    = DEFAULT

################################################################################
#
# Library Class section - list of all Library Classes needed by this Platform.
#
################################################################################
[LibraryClasses.common]

!include $(SILICON_PRODUCT_PATH)/SiPkgCommonLib.dsc
!include $(PLATFORM_PACKAGE)/Include/Dsc/CoreCommonLib.dsc
!include $(PLATFORM_FULL_PACKAGE)/PlatformCommonLib.dsc
!include $(PLATFORM_BIN_PACKAGE)/Include/Dsc/BinCommonLib.dsc

  UserPasswordLib|UserAuthFeaturePkg/Library/UserPasswordLib/UserPasswordLib.inf
  UserPasswordUiLib|UserAuthFeaturePkg/Library/UserPasswordUiLib/UserPasswordUiLib.inf
  PlatformPasswordLib|UserAuthFeaturePkg/Library/PlatformPasswordLibNull/PlatformPasswordLibNull.inf

  PlatformHookLib|MdeModulePkg/Library/BasePlatformHookLibNull/BasePlatformHookLibNull.inf

  ConfigBlockLib|IntelSiliconPkg/Library/BaseConfigBlockLib/BaseConfigBlockLib.inf

  SiMtrrLib|$(PLATFORM_SI_PACKAGE)/Library/SiMtrrLib/SiMtrrLib.inf

  BoardIdsLib|$(PLATFORM_BOARD_PACKAGE)/Library/PeiDxeBoardIdsLib/PeiDxeBoardIdsLib.inf
  FixedPointLib|$(PLATFORM_SI_PACKAGE)/Library/FixedPointLib/FixedPointLib.inf
  SiliconInitLib|$(PLATFORM_BSP_PATH)/Library/SiliconInitLib/SiliconInitLib.inf

  PostCodeMapLib|PostCodeDebugFeaturePkg/Library/PostCodeMapLib/PostCodeMapLib.inf

  Usb3DebugPortParameterLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseUsb3DebugPortParameterLibCmos/BaseUsb3DebugPortParameterLibCmos.inf

  FspCommonLib|IntelFsp2Pkg/Library/BaseFspCommonLib/BaseFspCommonLib.inf
  FspSwitchStackLib|IntelFsp2Pkg/Library/BaseFspSwitchStackLib/BaseFspSwitchStackLib.inf            # RPPO-CNL-0090
  FspWrapperApiLib|IntelFsp2WrapperPkg/Library/BaseFspWrapperApiLib/BaseFspWrapperApiLib.inf

  MipiSysTLib|MdePkg/Library/MipiSysTLib/MipiSysTLib.inf
  TraceHubDebugLib|$(PLATFORM_SI_PACKAGE)/Library/TraceHubDebugLib/BaseTraceHubDebugLib.inf
  FitAccessLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseFitAccessLib/BaseFitAccessLib.inf
  AmdSvsmLib|UefiCpuPkg/Library/AmdSvsmLibNull/AmdSvsmLibNull.inf

[LibraryClasses.IA32]

!include $(SILICON_PRODUCT_PATH)/SiPkgPeiLib.dsc
!include $(PLATFORM_PACKAGE)/Include/Dsc/CorePeiLib.dsc
!include $(PLATFORM_FULL_PACKAGE)/PlatformPeiLib.dsc
!include $(PLATFORM_BIN_PACKAGE)/Include/Dsc/BinPeiLib.dsc

  SmmAccessLib|IntelSiliconPkg/Feature/SmmAccess/Library/PeiSmmAccessLib/PeiSmmAccessLib.inf
  BiosIdLib|BoardModulePkg/Library/BiosIdLib/PeiBiosIdLib.inf
  MpInitLib|UefiCpuPkg/Library/MpInitLib/PeiMpInitLib.inf
  TraceHubDebugSysTLib|MdeModulePkg/Library/TraceHubDebugSysTLib/PeiTraceHubDebugSysTLib.inf
  BoardConfigLib|$(PLATFORM_BSP_PATH)/Library/PeiBoardConfigLib/PeiBoardConfigLib.inf
  SetupDataCacheLib|$(PLATFORM_BSP_PATH)/Features/Setup/Library/PeiSetupDataCacheLib/PeiSetupDataCacheLib.inf

!if gPlatformModuleTokenSpaceGuid.PcdUsb3SerialStatusCodeEnable == TRUE
  Usb3DebugPortLib|Usb3DebugFeaturePkg/Library/Usb3DebugPortLib/Usb3DebugPortLibPei.inf
!else
  Usb3DebugPortLib|Usb3DebugFeaturePkg/Library/Usb3DebugPortLib/Usb3DebugPortLibNull.inf
!endif

[LibraryClasses.IA32.SEC]
!if $(TARGET) == DEBUG
  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf
!endif

[LibraryClasses.IA32.PEI_CORE]

[LibraryClasses.IA32.PEIM]
!if $(TARGET) == DEBUG
  DebugLib|MdeModulePkg/Library/PeiDebugLibDebugPpi/PeiDebugLibDebugPpi.inf
!endif

[LibraryClasses.X64]

!include $(SILICON_PRODUCT_PATH)/SiPkgDxeLib.dsc
!include $(PLATFORM_PACKAGE)/Include/Dsc/CoreDxeLib.dsc
!include $(PLATFORM_FULL_PACKAGE)/PlatformDxeLib.dsc
!include $(PLATFORM_BIN_PACKAGE)/Include/Dsc/BinDxeLib.dsc

  TcgPhysicalPresenceLib|SecurityPkg/Library/DxeTcgPhysicalPresenceLib/DxeTcgPhysicalPresenceLib.inf
  TcgPpVendorLib|SecurityPkg/Library/TcgPpVendorLibNull/TcgPpVendorLibNull.inf

  BiosIdLib|BoardModulePkg/Library/BiosIdLib/DxeBiosIdLib.inf

  TraceHubDebugSysTLib|MdeModulePkg/Library/TraceHubDebugSysTLib/DxeSmmTraceHubDebugSysTLib.inf

!if gPlatformModuleTokenSpaceGuid.PcdUsb3SerialStatusCodeEnable == TRUE
  Usb3DebugPortLib|Usb3DebugFeaturePkg/Library/Usb3DebugPortLib/Usb3DebugPortLibDxe.inf
!else
  Usb3DebugPortLib|Usb3DebugFeaturePkg/Library/Usb3DebugPortLib/Usb3DebugPortLibNull.inf
!endif

!if gSiPkgTokenSpaceGuid.PcdSmbiosEnable == TRUE
  DxeSmbiosProcessorLib|$(PLATFORM_SI_PACKAGE)/Library/DxeSmbiosProcessorLib/DxeSmbiosProcessorLib.inf
!else
  DxeSmbiosProcessorLib|$(PLATFORM_SI_PACKAGE)/Library/DxeSmbiosProcessorLibNull/DxeSmbiosProcessorLibNull.inf
!endif

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################
[PcdsFeatureFlag.common]

[PcdsFeatureFlag.X64]

[PcdsFixedAtBuild.common]

[PcdsFeatureFlag]

[PcdsFixedAtBuild.IA32]

[PcdsFixedAtBuild.X64]

[PcdsPatchableInModule.common]

[PcdsDynamicHii.common.DEFAULT]

  gPlatformModuleTokenSpaceGuid.PcdPlatformMemoryCheck|L"MemoryCheck"|gPlatformModuleTokenSpaceGuid|0x0|0
  gPlatformModuleTokenSpaceGuid.PcdFastBootEnable|L"FastBootEnable"|gPlatformModuleTokenSpaceGuid|0x0|0

[PcdsDynamicHii.X64.DEFAULT]

  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|L"Timeout"|gEfiGlobalVariableGuid|0x0|5 # Variable: L"Timeout"

  gSiPkgTokenSpaceGuid.PcdCpuSmmUseDelayIndication|L"CpuSmm"|gCpuSmmGuid|0x2|1
  gSiPkgTokenSpaceGuid.PcdCpuSmmUseBlockIndication|L"CpuSmm"|gCpuSmmGuid|0x3|1
  gSiPkgTokenSpaceGuid.PcdCpuSmmUseSmmEnableIndication|L"CpuSmm"|gCpuSmmGuid|0x4|1

[PcdsDynamicDefault]
  gEfiMdeModulePkgTokenSpaceGuid.PcdSrIovSupport|FALSE

[PcdsDynamicDefault.common.DEFAULT]

  gMinPlatformPkgTokenSpaceGuid.PcdPciReservedMemBase |0x80000000
  gMinPlatformPkgTokenSpaceGuid.PcdPciReservedMemLimit|0xC0000000
  gMinPlatformPkgTokenSpaceGuid.PcdPciReservedMemAbove4GBBase  |0xFFFFFFFFFFFFFFFF
  gMinPlatformPkgTokenSpaceGuid.PcdPciReservedMemAbove4GBLimit |0x0000000000000000
  gMinPlatformPkgTokenSpaceGuid.PcdPciSegmentCount|0x1

!if (gBoardModuleTokenSpaceGuid.PcdS4Enable == FALSE)
  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|FALSE
!else
  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|TRUE
!endif

!if gBoardModuleTokenSpaceGuid.PcdAcpiDebugFeatureEnable == TRUE
  gAcpiDebugFeaturePkgTokenSpaceGuid.PcdAcpiDebugFeatureEnable|TRUE
!else
  gAcpiDebugFeaturePkgTokenSpaceGuid.PcdAcpiDebugFeatureEnable|FALSE
!endif

  gEfiMdeModulePkgTokenSpaceGuid.PcdAtaSmartEnable|TRUE

  ## The mask is used to control VTd behavior.<BR><BR>
  #  BIT0: Enable IOMMU during boot (If DMAR table is installed in DXE. If VTD_INFO_PPI is installed in PEI.)
  #  BIT1: Enable IOMMU when transfer control to OS (ExitBootService in normal boot. EndOfPEI in S3)
  #  BIT2: Force no IOMMU access attribute request recording before DMAR table is installed.
  # @Prompt The policy for VTd driver behavior.
  gIntelSiliconPkgTokenSpaceGuid.PcdVTdPolicyPropertyMask|0x00000004
  gEfiMdeModulePkgTokenSpaceGuid.PcdRecoveryFileName|L"Capsule.cap"|VOID*|0x20
  gSiPkgTokenSpaceGuid.PcdPciExpressRegionLength|0x10000000

  gEfiSecurityPkgTokenSpaceGuid.PcdFirmwareDebuggerInitialized|FALSE

  gProtectedVariableFeaturePkgTokenSpaceGuid.PcdRpmcSupported|FALSE

[PcdsDynamicExDefault.common.DEFAULT]

!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  ## Specifies max supported number of Logical Processors.
  # @Prompt Configure max supported number of Logical Processorss
  gUefiCpuPkgTokenSpaceGuid.PcdCpuMaxLogicalProcessorNumber|16
  ## Specifies the AP wait loop state during POST phase.
!else
  ## Specifies max supported number of Logical Processors.
  # @Prompt Configure max supported number of Logical Processorss
  gUefiCpuPkgTokenSpaceGuid.PcdCpuMaxLogicalProcessorNumber|12
!endif

  #  The value is defined as below.
  #  1: Place AP in the Hlt-Loop state.
  #  2: Place AP in the Mwait-Loop state.
  #  3: Place AP in the Run-Loop state.
  # @Prompt The AP wait loop state.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuApLoopMode|2

  ## Specifies the base address of the first microcode Patch in the microcode Region.
  # @Prompt Microcode Region base address.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuMicrocodePatchAddress|0x0

  ## Specifies the size of the microcode Region.
  # @Prompt Microcode Region size.
  gUefiCpuPkgTokenSpaceGuid.PcdCpuMicrocodePatchRegionSize|0x0
  gEfiMdeModulePkgTokenSpaceGuid.PcdFastPS2Detection|FALSE

  #
  # Some of the PCD consumed by both FSP and bootloader should be defined
  # here for bootloader to consume.
  #

[PcdsDynamicDefault.X64.DEFAULT]

  gEfiMdeModulePkgTokenSpaceGuid.PcdConInConnectOnDemand|FALSE

################################################################################
#
# Components Section
#
################################################################################
[Components.IA32]

  $(PLATFORM_FEATURES_PATH)/Pct/PlatformConfigTool.inf
  $(PLATFORM_FEATURES_PATH)/VTd/PlatformVTdSamplePei/PlatformVTdSamplePei.inf

  $(PLATFORM_FEATURES_PATH)/PlatformStatusCodeHandler/Pei/PlatformStatusCodeHandlerPei.inf {
    <LibraryClasses>
      !if $(TARGET) == DEBUG
        DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
      !else
        !if gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE
          DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
        !endif
      !endif
  }

!if gBoardModuleTokenSpaceGuid.PcdAdvancedFeatureEnable == TRUE
  $(PLATFORM_FULL_PACKAGE)/PlatformInitAdvanced/PlatformInitAdvancedPei/PlatformInitAdvancedPreMem.inf {
    <LibraryClasses>
      PeiPlatformRecoveryLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiPlatformRecoveryLib/PeiPlatformRecoveryLib.inf
      GuidForwardLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiGuidForwardLib/PeiGuidForwardLib.inf
        PeiWdtAppLib|$(PLATFORM_FULL_PACKAGE)/Library/PeiWdtAppLib/PeiWdtAppLib.inf
  }
!endif  # PcdAdvancedFeatureEnable

!if gBoardModuleTokenSpaceGuid.PcdAdvancedFeatureEnable == TRUE
  #
  # Crash Log Support
  #
  $(PLATFORM_FEATURES_PATH)/CrashLog/CrashLogPei.inf
!endif  # PcdAdvancedFeatureEnable


  $(PLATFORM_FULL_PACKAGE)/FspWrapper/FspWrapperPeim/FspWrapperPeim.inf

[Components.X64]

  $(PLATFORM_FEATURES_PATH)/Cnv/CnvUefiVariables/CnvUefiVariables.inf
  $(PLATFORM_FEATURES_PATH)/CrashLog/CrashLogDxe.inf
  $(PLATFORM_FULL_PACKAGE)/Library/DxeDriverBdat/BdatList.inf
  $(PLATFORM_FEATURES_PATH)/VTd/PlatformVTdSampleDxe/PlatformVTdSampleDxe.inf
  $(PLATFORM_FULL_PACKAGE)/Platform/SpiFvbService/SpiFvbServiceDxe.inf
  $(PLATFORM_FEATURES_PATH)/Rst/RstUefiDriverSupport/RstUefiDriverSupport.inf
  $(PLATFORM_FEATURES_PATH)/Hsti/HstiIhvDxe/HstiIhvDxe.inf {
    <LibraryClasses>
      HstiLib|MdePkg/Library/DxeHstiLib/DxeHstiLib.inf
      Tpm2CommandLib|SecurityPkg/Library/Tpm2CommandLib/Tpm2CommandLib.inf
  }
  $(PLATFORM_FEATURES_PATH)/PlatformStatusCodeHandler/RuntimeDxe/PlatformStatusCodeHandlerRuntimeDxe.inf {
    <LibraryClasses>
      SerialIoUartLib|$(PLATFORM_SI_PACKAGE)/IpBlock/SerialIo/Uart/Library/PeiDxeSmmSerialIoUartLib/DxeRuntimeSerialIoUartLib.inf
      !if $(TARGET) == DEBUG
        DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
        SerialIoUartDebugPropertyLib|$(PLATFORM_FULL_PACKAGE)/Library/SerialIoUartDebugPropertyLib/DxeSmmSerialIoUartDebugPropertyLib.inf
      !else
        !if gSiPkgTokenSpaceGuid.PcdSiCatalogDebugEnable == TRUE
          DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
        !endif
      !endif
  }

  $(PLATFORM_FULL_PACKAGE)/Platform/PciPlatform/Dxe/PciPlatform.inf {
    <LibraryClasses>
      DxeOpromPatchLib|$(PLATFORM_FULL_PACKAGE)/Library/DxeOpromPatchLib/DxeOpromPatchLib.inf
  }

!if gSiPkgTokenSpaceGuid.PcdSmmVariableEnable == TRUE
  $(PLATFORM_FULL_PACKAGE)/Platform/SpiFvbService/SpiFvbServiceSmm.inf
!endif # gSiPkgTokenSpaceGuid.PcdSmmVariableEnable

  $(PLATFORM_FEATURES_PATH)/PlatformStatusCodeHandler/Smm/PlatformStatusCodeHandlerSmm.inf
  $(PLATFORM_FEATURES_PATH)/GuidForwardHandler/Smm/GuidForwardHandlerSmm.inf
  $(PLATFORM_FEATURES_PATH)/GuidForwardHandler/RuntimeDxe/GuidForwardHandlerRuntimeDxe.inf


!if gSiPkgTokenSpaceGuid.PcdSmbiosEnable == TRUE
  $(PLATFORM_FULL_PACKAGE)/Platform/SmbiosPlatformInfo/SmbiosPlatformInfo.inf
  $(PLATFORM_FEATURES_PATH)/Smbios/SmbiosMiscDxe/SmbiosMiscDxe.inf
  !if gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable == TRUE
    $(PLATFORM_FEATURES_PATH)/Smbios/SmbiosTpmDeviceInfo/SmbiosTpmDeviceInfo.inf
  !endif
!endif

!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  $(PLATFORM_FULL_PACKAGE)/FspWrapper/CheckFspErrorInfoHob/CheckFspErrorInfoHobDxe.inf {
    <LibraryClasses>
      !if $(TARGET) == DEBUG
        DebugLib|$(PLATFORM_FULL_PACKAGE)/Library/BaseDebugLibAllDebugPort/BaseDebugLibAllDebugPort.inf
      !endif
  }
!endif

#!if gPlatformModuleTokenSpaceGuid.PcdSaveMemoryConfigEnable == TRUE
  $(PLATFORM_FULL_PACKAGE)/Platform/SaveMemoryConfig/Dxe/SaveMemoryConfig.inf
#!endif

!if gSiPkgTokenSpaceGuid.PcdAcpiEnable == TRUE
  $(PLATFORM_FEATURES_PATH)/Acpi/AcpiTables/AcpiFeatures.inf
!endif

!if gBoardModuleTokenSpaceGuid.PcdSetupEnable == TRUE
  $(PLATFORM_FEATURES_PATH)/UiApp/UiApp.inf {
    <Defines>
      FILE_GUID = D89A7D8B-D016-4D26-93E3-EAB6B4D3B0A2
  }
!endif #PcdSetupEnable == TRUE

!if gBoardModuleTokenSpaceGuid.PcdSetupEnable == TRUE
  !include $(PLATFORM_BOARD_PACKAGE)/Features/Setup/Include/Dsc/SetupDxe.dsc
!endif

!if gPlatformModuleTokenSpaceGuid.PcdPciHotplugEnable == TRUE
  $(PLATFORM_FULL_PACKAGE)/Platform/PciHotPlug/PciHotPlug.inf
!endif

!if gSiPkgTokenSpaceGuid.PcdAtaEnable == TRUE
  $(PLATFORM_FEATURES_PATH)/Sata/SataController/SataController.inf
!endif


  $(PLATFORM_FEATURES_PATH)/Wdt/WdtAppDxe/WdtAppDxe.inf

!if gSiPkgTokenSpaceGuid.PcdEmbeddedEnable == TRUE
  $(PLATFORM_FEATURES_PATH)/LifeCycleState/LifeCycleState.inf
!endif

!if gBoardModuleTokenSpaceGuid.PcdAdvancedFeatureEnable == TRUE
  $(PLATFORM_FEATURES_PATH)/DashG/DxeDgOpregionInit.inf
!endif

#
# Platform Pdt feature support Driver
#
$(PLATFORM_FEATURES_PATH)/Pdt/PlatformPdtFeatureDxe.inf {
  <LibraryClasses>
    PdtUpdateLib|$(PLATFORM_FULL_PACKAGE)/Library/PdtUpdateLib/PdtUpdateLib.inf
}

!include $(PLATFORM_FULL_PACKAGE)/PlatformPkgBuildOption.dsc
