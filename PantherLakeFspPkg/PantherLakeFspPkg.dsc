## @file
#  Platform description.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2016 Intel Corporation.
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
  #
  # Set platform specific package/folder name, same as passed from PREBUILD script.
  # PLATFORM_PACKAGE would be the same as PLATFORM_NAME as well as package build folder
  # DEFINE only takes effect at R9 DSC and FDF.
  #
  DEFINE      FSP_PACKAGE                     = PantherLakeFspPkg
  DEFINE      PLATFORM_SI_PACKAGE             = OneSiliconPkg
  DEFINE      SILICON_FSP_PATH                = $(PLATFORM_SI_PACKAGE)/Fsp
  DEFINE      SILICON_PRODUCT_PATH            = $(PLATFORM_SI_PACKAGE)/Product/PantherLake
  DEFINE      FSP_ARCH                        = X64
  #
  # Silicon On/Off feature are defined here
  # Should put it before a new section, since it also has section.
  #


  !include $(SILICON_PRODUCT_PATH)/SiPkgPcdInit.dsc

  #
  # BIOS build switches configuration
  #
  !include $(FSP_PACKAGE)/FspPkgPcdInit.dsc
  !include $(FSP_PACKAGE)/FspPkgPcdUpdate.dsc

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = $(FSP_PACKAGE)
  PLATFORM_GUID                  = 1BEDB57A-7904-406e-8486-C89FC7FB39EE
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = $(CFG_OUTDIR)/$(FSP_PACKAGE)
  SUPPORTED_ARCHITECTURES        = $(FSP_ARCH)|X64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = $(FSP_PACKAGE)/$(FSP_PACKAGE).fdf

  #
  # UPD tool definition
  #
  FSP_T_UPD_TOOL_GUID            = 34686CA3-34F9-4901-B82A-BA630F0714C6
  FSP_M_UPD_TOOL_GUID            = 39A250DB-E465-4DD1-A2AC-E2BD3C0E2385
  FSP_S_UPD_TOOL_GUID            = CAE3605B-5B34-4C85-B3D7-27D54273C40F
  FSP_T_UPD_FFS_GUID             = 70BCF6A5-FFB1-47D8-B1AE-EFE5508E23EA
  FSP_M_UPD_FFS_GUID             = D5B86AEA-6AF7-40D4-8014-982301BC3D89
  FSP_S_UPD_FFS_GUID             = E3CD9B18-998C-4F76-B65E-98B154E5446F

################################################################################
#
# SKU Identification section - list of all SKU IDs supported by this
#                              Platform.
#
################################################################################
[SkuIds]
  0|DEFAULT              # The entry: 0|DEFAULT is reserved and always required.

################################################################################
#
# Library Class section - list of all Library Classes needed by this Platform.
#
################################################################################
!include MdePkg/MdeLibs.dsc.inc

# Maintain the strict order to avoid overriding Libraries.
!include $(SILICON_PRODUCT_PATH)/SiPkgCommonLib.dsc
!include $(SILICON_PRODUCT_PATH)/SiPkgPeiLib.dsc
!include $(SILICON_PRODUCT_PATH)/SiPkgDxeLib.dsc

[LibraryClasses.common.PEIM, LibraryClasses.common.PEI_CORE, LibraryClasses.common.SEC]
#
# UEFI & PI
#
  PeiServicesTablePointerLib|MdePkg/Library/PeiServicesTablePointerLibIdt/PeiServicesTablePointerLibIdt.inf
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  PeiPolicyUpdatePreMemLib|$(FSP_PACKAGE)/Library/PeiPolicyUpdatePreMemLib/PeiPolicyUpdatePreMemLib.inf
  PeiPolicyUpdateLib|$(FSP_PACKAGE)/Library/PeiPolicyUpdateLib/PeiPolicyUpdateLib.inf

#
# Generic Modules
#
  OemHookStatusCodeLib|MdeModulePkg/Library/OemHookStatusCodeLibNull/OemHookStatusCodeLibNull.inf

#
# Misc
#
  CpuExceptionHandlerLib|UefiCpuPkg/Library/CpuExceptionHandlerLib/SecPeiCpuExceptionHandlerLib.inf
!if gPantherLakeFspPkgTokenSpaceGuid.PcdMonoStatusCode == TRUE
  MonoStatusCodeLib|$(SILICON_FSP_PATH)/Library/MonoStatusCode/MonoStatusCode.inf
!else
  MonoStatusCodeLib|$(SILICON_FSP_PATH)/Library/MonoStatusCodeNull/MonoStatusCodeNull.inf
!endif

[LibraryClasses.common.PEIM]
#
# Entry point
#
  PeimEntryPoint|MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf

!if gSiPkgTokenSpaceGuid.PcdSignedFspEnable == TRUE
#
# Crypto services
#
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/PeiCryptLib.inf
!endif

#
# Silicon Init Pei Library
#
  SiPolicyLib|$(PLATFORM_SI_PACKAGE)/Product/PantherLake/Library/PeiSiPolicyLib/PeiSiPolicyLib.inf
  SiFviInitLib|$(PLATFORM_SI_PACKAGE)/LibraryPrivate/PeiSiFviInitLib/PeiSiFviInitLib.inf
  StallPpiLib|$(PLATFORM_SI_PACKAGE)/Library/PeiInstallStallPpiLib/PeiStallPpiLib.inf
  SiPolicyOverrideLib|$(PLATFORM_SI_PACKAGE)/Product/PantherLake/LibraryPrivate/PeiPolicyOverrideLib/PeiSiPolicyOverrideLib.inf
  PeiSiSsidLib|$(PLATFORM_SI_PACKAGE)/LibraryPrivate/PeiSiSsidLib/PeiSiSsidLib.inf
  PciIoLib|$(PLATFORM_SI_PACKAGE)/LibraryPrivate/PciIoLib/PciIoLib.inf
  IpWrapper64BitLib|$(PLATFORM_SI_PACKAGE)/LibraryPrivate/BaseIpWrapper64BitLib/BaseIpWrapper64BitLib.inf

#
# Pch
#
  PtlPcdPolicyLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdPolicyLib/PtlPcdPolicyLib.inf
  PeiSpiPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Spi/LibraryPrivate/PeiSpiPolicyLib/PeiSpiPolicyLib.inf

#
# Me
#
  HeciAsyncSupportLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/LibraryPrivate/PeiDxeHeciAsyncSupportLibNull/PeiDxeHeciAsyncSupportLibNull.inf
  HeciUtilsLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Heci/HeciUtils/HeciUtilsPeiLib.inf
  HeciTransportCoreLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Heci/HeciTransport/HeciTransportCoreLib/HeciTransportCorePeiLib.inf
  HeciControlFilterLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Heci/HeciControl/HeciControlFilterLib/HeciControlFilterLib.inf

#
# Cpu
#
!if gSiPkgTokenSpaceGuid.PcdCpuPowerOnConfigEnable == TRUE
  CpuPowerOnConfigLib|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuInit/LibraryPrivate/PeiCpuPowerOnConfigLib/PeiCpuPowerOnConfigLib.inf
!else
  CpuPowerOnConfigLib|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuInit/LibraryPrivate/PeiCpuPowerOnConfigLibDisable/PeiCpuPowerOnConfigLibDisable.inf
!endif

!if gSiPkgTokenSpaceGuid.PcdSmbiosEnable == TRUE
  SmbiosCpuLib|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuInit/LibraryPrivate/PeiSmbiosCpuLib/PeiSmbiosCpuLib.inf
!else
  SmbiosCpuLib|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuInit/LibraryPrivate/PeiSmbiosCpuLibNull/PeiSmbiosCpuLibNull.inf
!endif

#
# Txt
#
!if gSiPkgTokenSpaceGuid.PcdTxtEnable == TRUE
  PeiTxtLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Txt/LibraryPrivate/PeiTxtLib/PeiTxtLib.inf
!else
  PeiTxtLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Txt/LibraryPrivate/PeiTxtLibNull/PeiTxtLibNull.inf
!endif
  TxtPolicyLibPreMem|$(PLATFORM_SI_PACKAGE)/IpBlock/Txt/LibraryPrivate/PeiTxtPolicyLib/PeiTxtPolicyLibPreMem.inf

!if gSiPkgTokenSpaceGuid.PcdTxtEnable == TRUE || gSiPkgTokenSpaceGuid.PcdBiosGuardEnable == TRUE
 PeiDprInfoLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Dpr/LibraryPrivate/PeiDprInfoLib/PeiDprInfoLib.inf
!else
 PeiDprInfoLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Dpr/LibraryPrivate/PeiDprInfoLibNull/PeiDprInfoLibNull.inf
!endif

#
# VT-D
#
  PeiGetVtdPmrAlignmentLib|IntelSiliconPkg/Library/PeiGetVtdPmrAlignmentLib/PeiGetVtdPmrAlignmentLib.inf
  PeiMemorySubSystemInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/MemorySubSystem/LibraryPrivate/MemorySubSystemInitLib/MemorySubSystemInitLib.inf
  MemorySubSystemFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/MemorySubSystemFruLib/MemorySubtemSysFruLib.inf
  PeiMemorySubSystemInfoLib|$(PLATFORM_SI_PACKAGE)/IpBlock/MemorySubSystem/LibraryPrivate/MemorySubSystemInfoLib/MemorySubSystemInfoLib.inf
  MemorySubSystemPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/MemorySubSystem/LibraryPrivate/MemorySubSystemPolicyLib/MemorySubSystemPolicyLib.inf
  PeiMemorySubSystemFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/MemorySubSystemFruLib/PeiMemorySubSystemFruLib.inf

#
#  Cce IpBlock
#
  CceLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Cce/LibraryPrivate/CceLibVer1/CceLibVer1.inf

#
#  TDx IpBlock
#
PeiTdxLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Tdx/LibraryPrivate/PeiTdxLib/PeiTdxLib.inf
TdxFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/TdxFruLib/TdxFruLib.inf

#
# Cpu
#
  CpuPowerManagementPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuPowerManagement/LibraryPrivate/PeiCpuPowerManagementPolicyLib/PeiCpuPowerManagementPolicyLib.inf
  CpuInitPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuInit/LibraryPrivate/PeiCpuInitPolicyLib/PeiCpuInitPolicyLib.inf
  PeiBootGuardPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/BootGuard/LibraryPrivate/PeiBootGuardPolicyLib/PeiBootGuardPolicyLib.inf

#
# TBT
#
  PeiITbtPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Usb4/PeiITbtPolicyLib/PeiITbtPolicyLib.inf
  MpInitLib|UefiCpuPkg/Library/MpInitLib/PeiMpInitLib.inf

!if $(TARGET) == DEBUG
  DebugLib|$(SILICON_FSP_PATH)/Library/FspDebugLibService/FspDebugLibService.inf
!else
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
!endif
  CpuCacheInfoLib|UefiCpuPkg/Library/CpuCacheInfoLib/PeiCpuCacheInfoLib.inf

[LibraryClasses.common.PEIM, LibraryClasses.common.PEI_CORE]
  ExtractGuidedSectionLib|MdePkg/Library/PeiExtractGuidedSectionLib/PeiExtractGuidedSectionLib.inf
  ResetSystemLib|MdeModulePkg/Library/PeiResetSystemLib/PeiResetSystemLib.inf
  PciLib|MdePkg/Library/BasePciLibPciExpress/BasePciLibPciExpress.inf
  TimerLib|UefiCpuPkg/Library/CpuTimerLib/BaseCpuTimerLib.inf

[LibraryClasses.common.PEI_CORE]
  #
  # Entry point
  #
  PeiCoreEntryPoint|MdePkg/Library/PeiCoreEntryPoint/PeiCoreEntryPoint.inf

[LibraryClasses]
  SiPostCodeMapLib|$(PLATFORM_SI_PACKAGE)/Library/Rsc2PostCodeMapLib/SiPostCodeMapLib.inf
  PostCodeMapOverrideLib|$(PLATFORM_SI_PACKAGE)/Library/Rsc2PostCodeMapLib/PostCodeMapOverrideLib.inf
  PlatformPostCodeMapLib|$(PLATFORM_SI_PACKAGE)/Library/Rsc2PostCodeMapLibNull/PlatformPostCodeMapLibNull.inf
  Rsc2PostCodeMapLib|$(PLATFORM_SI_PACKAGE)/Library/Rsc2PostCodeMapLib/Rsc2PostCodeMapLib.inf

  FspFbmSupportLib|$(FSP_PACKAGE)/Library/BaseFspFbmSupportLib/BaseFspFbmSupportLib.inf
!if gSiPkgTokenSpaceGuid.PcdSignedFspEnable == TRUE
#
# Crypto services
#
  OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLib.inf
  IntrinsicLib|CryptoPkg/Library/IntrinsicLib/IntrinsicLib.inf
  RngLib|MdePkg/Library/BaseRngLib/BaseRngLib.inf
  FspVerificationLib|$(FSP_PACKAGE)/Library/BaseFspVerificationLib/BaseFspVerificationLib.inf
  FspMeasurementLib|$(FSP_PACKAGE)/Library/MeasurementLib/FspMeasurementLib.inf
  Tpm2DeviceLib|$(FSP_PACKAGE)/Library/SecTpm2DeviceLibDTpm/Tpm2DeviceLibDTpmFsp.inf
  Tpm2CommandLib|SecurityPkg/Library/Tpm2CommandLib/Tpm2CommandLib.inf
!endif

#
# UEFI & PI
#
  FspMultiPhaseLib|IntelFsp2Pkg/Library/BaseFspMultiPhaseLib/BaseFspMultiPhaseLib.inf
  ExtractFspmLib|$(FSP_PACKAGE)/Library/ExtractFspmLib/ExtractFspmLib.inf
  ExtractGuidedSectionLib|PantherLakeFspPkg/Library/SecExtractGuidedSectionLib/SecExtractGuidedSectionLib.inf
  LzmaArchDecompressLib|MdeModulePkg/Library/LzmaCustomDecompressLib/LzmaCustomDecompressLib.inf

#
# Silicon Init Pei Library
#
  SiConfigBlockLib|$(PLATFORM_SI_PACKAGE)/Library/BaseSiConfigBlockLib/BaseSiConfigBlockLib.inf

#
# Pch
#
  TraceHubHookLib|$(SILICON_PRODUCT_PATH)/Library/TraceHubHookLib/PeiTraceHubHookLib.inf
  MipiSysTLib|MdePkg/Library/MipiSysTLib/MipiSysTLib.inf
  TraceHubDebugLib|$(PLATFORM_SI_PACKAGE)/Library/TraceHubDebugLib/BaseTraceHubDebugLib.inf
  TraceHubDebugSysTLib|MdeModulePkg/Library/TraceHubDebugSysTLib/PeiTraceHubDebugSysTLib.inf

#
# VT-D
#
  PeiGetVtdPmrAlignmentLib|IntelSiliconPkg/Library/PeiGetVtdPmrAlignmentLib/PeiGetVtdPmrAlignmentLib.inf

#
# MkTme IpBlock
#

#
# TBT
#

# Maintain the strict order to avoid overriding Libraries.
  !include $(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/CommonLib.dsc
  !include $(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/CommonLib.dsc

  #
  # Basic
  #
  DebugPrintErrorLevelLib|$(SILICON_FSP_PATH)/Library/DebugPrintErrorLevelLib/FspDebugPrintErrorLevelLib.inf
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  PciCf8Lib|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLibRepStr/BaseMemoryLibRepStr.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  PeCoffGetEntryPointLib|MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
  CacheMaintenanceLib|MdePkg/Library/BaseCacheMaintenanceLib/BaseCacheMaintenanceLib.inf
  PeCoffLib|MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf
  PeCoffExtraActionLib|MdePkg/Library/BasePeCoffExtraActionLibNull/BasePeCoffExtraActionLibNull.inf
  UefiDecompressLib|MdePkg/Library/BaseUefiDecompressLib/BaseUefiDecompressLib.inf
  SynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf
  S3PciSegmentLib|MdePkg/Library/BaseS3PciSegmentLib/BaseS3PciSegmentLib.inf
  SortLib|MdeModulePkg/Library/BaseSortLib/BaseSortLib.inf
  AmdSvsmLib|UefiCpuPkg/Library/AmdSvsmLibNull/AmdSvsmLibNull.inf
  PtlPcdGpioTopologyLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdGpioTopology/PtlPcdGpioTopology.inf
  #
  # Misc
  #
  MtrrLib|UefiCpuPkg/Library/MtrrLib/MtrrLib.inf
  LocalApicLib|UefiCpuPkg/Library/BaseXApicX2ApicLib/BaseXApicX2ApicLib.inf
  ### Base on debug/release mode, choose one of the DebugLib
  ####  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf

  FspSerialIoUartDebugHelperLib|$(SILICON_FSP_PATH)/Library/FspSerialIoUartDebugHelperLib/FspSerialIoUartDebugHelperLib.inf
  MicrocodeLib|UefiCpuPkg/Library/MicrocodeLib/MicrocodeLib.inf
  RngLib|MdePkg/Library/BaseRngLib/BaseRngLib.inf
!if gSiPkgTokenSpaceGuid.PcdFspVEnable == TRUE
  FspVInfoLib|$(SILICON_FSP_PATH)/Library/FspValidation/FspVInfoLib/FspVInfoLib.inf
  FspVTestCaseLib|$(SILICON_FSP_PATH)/Library/FspValidation/TestCaseLib/FspVTestCaseLib.inf
  FspVPolicyLib|$(SILICON_FSP_PATH)/Library/FspValidation/FspVPolicyLib/FspVPolicyLib.inf
!else
  FspVPolicyLib|$(SILICON_FSP_PATH)/Library/FspValidation/FspVPolicyLib/FspVPolicyLibNull.inf
!endif

  #
  #  VMD IpBlock
  #
 VmdInfoLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Vmd/Library/VmdInfoLib/VmdInfoLib.inf
!if gSiPkgTokenSpaceGuid.PcdVmdEnable == TRUE
  PeiVmdInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Vmd/LibraryPrivate/PeiVmdInitLib/PeiVmdInitLib.inf
  PeiVmdPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Vmd/LibraryPrivate/PeiVmdPolicyLib/PeiVmdPolicyLib.inf
!else
  PeiVmdInitLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Vmd/LibraryPrivate/PeiVmdInitLibNull/PeiVmdInitLibNull.inf
  PeiVmdPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Vmd/LibraryPrivate/PeiVmdPolicyLibNull/PeiVmdPolicyLibNull.inf
!endif

  #
  # Silicon library
  #
  ConfigBlockLib|IntelSiliconPkg/Library/BaseConfigBlockLib/BaseConfigBlockLib.inf
  SerialPortLib|MdeModulePkg/Library/BaseSerialPortLib16550/BaseSerialPortLib16550.inf
  PlatformHookLib|MdeModulePkg/Library/BasePlatformHookLibNull/BasePlatformHookLibNull.inf

  #
  # Pch
  #
  GpioHelpersLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Gpio/LibraryPrivate/BaseGpioHelpersLibNull/BaseGpioHelpersLibNull.inf

  #
  # CPU
  #
  CcExitLib|UefiCpuPkg/Library/CcExitLibNull/CcExitLibNull.inf
  CpuPageTableLib|UefiCpuPkg/Library/CpuPageTableLib/CpuPageTableLib.inf


[LibraryClasses.IA32.SEC, LibraryClasses.X64.SEC]
  NemConfigLib|$(PLATFORM_SI_PACKAGE)/Library/BaseNemConfigLibNull/BaseNemConfigLibNull.inf
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/SecCryptLib.inf
  MmPciLib|$(SILICON_FSP_PATH)/Library/MmPciCf8Lib/MmPciCf8Lib.inf
  PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
  TimerLib|UefiCpuPkg/Library/CpuTimerLib/BaseCpuTimerLib.inf
  SecFspSiliconLib|$(SILICON_FSP_PATH)/Library/SecFspSiliconLib/SecFspSiliconLib.inf
  SecGetFsptApiParameterLib|$(PLATFORM_SI_PACKAGE)/Library/SecGetFsptApiParameterLib/SecGetFsptApiParameterLib.inf
!if $(TARGET) == DEBUG
  SecSerialPortLib|$(SILICON_FSP_PATH)/Library/SecSerialPortInitLib/SecSerialPortInitLib.inf
!else
  SecSerialPortLib|$(SILICON_FSP_PATH)/Library/SecSerialPortInitLib/SecSerialPortInitLibNull.inf
!endif
  GpioHelpersLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Gpio/LibraryPrivate/BaseGpioHelpersLibNull/BaseGpioHelpersLibNull.inf

  SecHostBridgeLib|$(PLATFORM_SI_PACKAGE)/IpBlock/HostBridge/Library/SecHostBridgeLib/SecHostBridgeLib.inf

[LibraryClasses.IA32.SEC, LibraryClasses.IA32.PEI_CORE, LibraryClasses.X64.SEC, LibraryClasses.X64.PEI_CORE]
  TraceHubHookLib|$(SILICON_PRODUCT_PATH)/Library/TraceHubHookLib/BaseTraceHubHookLib.inf
  TraceHubDebugSysTLib|MdeModulePkg/Library/TraceHubDebugSysTLib/BaseTraceHubDebugSysTLib.inf
!if $(TARGET) == DEBUG
  DebugLib|$(SILICON_FSP_PATH)/Library/DebugLibDebugPort/Pei/DebugLibDebugPort.inf
!else
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
!endif

[LibraryClasses.common.PEIM, LibraryClasses.common.PEI_CORE, LibraryClasses.common.SEC]
  #
  # SEC and PEI phase common
  #
  S3BootScriptLib|MdePkg/Library/BaseS3BootScriptLibNull/BaseS3BootScriptLibNull.inf
  PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  FspSwitchStackLib|IntelFsp2Pkg/Library/BaseFspSwitchStackLib/BaseFspSwitchStackLib.inf
  CacheAsRamLib|$(PLATFORM_SI_PACKAGE)/Library/BaseCacheAsRamLib/BaseCacheAsRamLib.inf
  FspCommonLib|IntelFsp2Pkg/Library/BaseFspCommonLib/BaseFspCommonLib.inf
  FspPlatformLib|IntelFsp2Pkg/Library/BaseFspPlatformLib/BaseFspPlatformLib.inf
  PtlPcdSecGpioInitLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdLpssLib/PtlPcdSecGpioInitLib.inf
  SecPreMemLpssInitLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/SecPreMemLpssInitLib/SecPreMemLpssInitLib.inf
  PtlPcdMinGpioTopology|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdGpioTopology/PtlPcdMinGpioTopology.inf

!if $(TARGET) == DEBUG
  SerialPortLib|$(SILICON_FSP_PATH)/Library/SerialPortLib/SerialPortLib.inf
!else
  SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
!endif

  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  PostCodeLib|$(SILICON_FSP_PATH)/Library/BasePostCodeLibPort80TraceHub/BasePostCodeLibPort80TraceHub.inf
  PostCodeToScratchPadLib|$(PLATFORM_SI_PACKAGE)/Library/BasePostCodeToScratchPadLibNull/BasePostCodeToScratchPadLibNull.inf
  ReportStatusCodeLib|$(PLATFORM_SI_PACKAGE)/Fsp/Library/Rsc2PostCodeReportStatusCodeLib/PeiReportStatusCodeLib/Rsc2PostCodePeiReportStatusCodeLib.inf
!if gPantherLakeFspPkgTokenSpaceGuid.PcdFspPerformanceEnable == TRUE
  PerformanceLib|MdeModulePkg/Library/PeiPerformanceLib/PeiPerformanceLib.inf
!else
  PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
!endif

  #
  # Silicon initialization library
  #
  SerialIoSpiSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdSerialIoLib/PtlPcdPeiSerialIoSpiLib.inf

#  TxtLib|$(PLATFORM_SI_PACKAGE)/Cpu/Library/PeiDxeTxtLib/PeiDxeTxtLib.inf

[LibraryClasses.common.DXE_DRIVER]
  #
  # DXE phase common
  #
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  PciLib|MdePkg/Library/BasePciLibPciExpress/BasePciLibPciExpress.inf
  TimerLib|UefiCpuPkg/Library/CpuTimerLib/BaseCpuTimerLib.inf
  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf
  DxeServicesTableLib|MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  LockBoxLib|MdeModulePkg/Library/SmmLockBoxLib/SmmLockBoxDxeLib.inf
!if gPantherLakeFspPkgTokenSpaceGuid.PcdFspPerformanceEnable == TRUE
  PerformanceLib|MdeModulePkg/Library/DxePerformanceLib/DxePerformanceLib.inf
!else
  PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
!endif
  PostCodeLib|MdePkg/Library/BasePostCodeLibDebug/BasePostCodeLibDebug.inf
  ReportStatusCodeLib|$(PLATFORM_SI_PACKAGE)/Fsp/Library/Rsc2PostCodeReportStatusCodeLib/DxeReportStatusCodeLib/Rsc2PostCodeDxeReportStatusCodeLib.inf
  #
  # UEFI & PI
  #
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
!if $(TARGET) == RELEASE
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
!else
  DebugLib|$(SILICON_FSP_PATH)/Library/DebugLibDebugPort/Dxe/DebugLibDebugPort.inf
!endif
  #
  # Framework
  #
!if gSiPkgTokenSpaceGuid.PcdS3Enable == TRUE
  S3BootScriptLib|MdeModulePkg/Library/PiDxeS3BootScriptLib/DxeS3BootScriptLib.inf
!else
  S3BootScriptLib|MdePkg/Library/BaseS3BootScriptLibNull/BaseS3BootScriptLibNull.inf
!endif
  #
  # Silicon initialization library
  #
  TraceHubHookLib|$(PLATFORM_SI_PACKAGE)/IpBlock/TraceHub/Library/TraceHubHookLib/DxeSmmTraceHubHookLib.inf
  PciExpressLib|$(PLATFORM_SI_PACKAGE)/Library/FspPciExpressMultiSegLib/FspPciExpressMultiSegLib.inf
  ResetSystemLib|MdeModulePkg/Library/DxeResetSystemLib/DxeResetSystemLib.inf
  PtlPcdInfoLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/Library/PtlPcdInfoLib/PtlPcdDxeInfoLib.inf

  #
  #  Tse IpBlock
  #
  !if gSiPkgTokenSpaceGuid.PcdTseEnable == TRUE
    DxeTseLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Tse/LibraryPrivate/DxeTseLib/DxeTseLib.inf
  !else
    DxeTseLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Tse/LibraryPrivate/DxeTseLibNull/DxeTseLibNull.inf
  !endif

  #
  # Soc
  #
  SerialIoUartSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdSerialIoLib/PtlPcdDxeSmmSerialIoUartLib.inf
  SerialIoI2cSocLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdSerialIoLib/PtlPcdDxeSmmSerialIoI2cLib.inf

  DxeSaInitFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/LibraryPrivate/DxeSaInitFruLib/DxeSaInitFruLib.inf

[PcdsDynamicVpd.Upd]
  #
  # This section is not used by the normal build process
  # Howerver, FSP will use dedicated tool to handle it and generate a
  # VPD simliar binary block (User Configuration Data). This block will
  # be accessed through a generated data structure directly rather than
  # PCD services. This is for size consideration.
  # Format:
  #   gPlatformFspPkgTokenSpaceGuid.Updxxxxxxxxxxxxn        | OFFSET | LENGTH | VALUE
  # Only simple data type is supported
  #

  # offset 0000 ~ 00B0
  !include $(FSP_PACKAGE)/Upd/FsptUpd.dsc

  # offset 0000 ~ 06AE
  !include $(FSP_PACKAGE)/Upd/FspmUpd.dsc

  # offset 0000 ~ 0D10
  !include $(FSP_PACKAGE)/Upd/FspsUpd.dsc

###################################################################################################
#
# Components Section - list of the modules and components that will be processed by compilation
#                      tools and the EDK II tools to generate PE32/PE32+/Coff image files.
#
# Note: The EDK II DSC file is not used to specify how compiled binary images get placed
#       into firmware volume images. This section is just a list of modules to compile from
#       source into UEFI-compliant binaries.
#       It is the FDF file that contains information on combining binary files into firmware
#       volume images, whose concept is beyond UEFI and is described in PI specification.
#       Binary modules do not need to be listed in this section, as they should be
#       specified in the FDF file. For example: Shell binary (Shell_Full.efi), FAT binary (Fat.efi),
#       Logo (Logo.bmp), and etc.
#       There may also be modules listed in this section that are not required in the FDF file,
#       When a module listed here is excluded from FDF file, then UEFI-compliant binary will be
#       generated for it, but the binary will not be put into any firmware volume.
#
###################################################################################################

[Components.$(FSP_ARCH)]
  #
  # SEC
  #
  IntelFsp2Pkg/FspSecCore/FspSecCoreT.inf {
    <LibraryClasses>
      MmPciLib|$(SILICON_FSP_PATH)/Library/MmPciCf8Lib/MmPciCf8Lib.inf
      PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
      FspSecPlatformLib|$(PLATFORM_SI_PACKAGE)/Library/PlatformSecLib/FspTPlatformSecLib.inf
      NULL|MdePkg/Library/StackCheckLibNull/StackCheckLibNull.inf
    <PcdsFixedAtBuild>
      gSiPkgTokenSpaceGuid.PcdFspValidatePeiServiceTablePointer|FALSE
  }
  IntelFsp2Pkg/FspSecCore/Fsp24SecCoreM.inf {
    <LibraryClasses>
      MmPciLib|$(SILICON_FSP_PATH)/Library/MmPciCf8Lib/MmPciCf8Lib.inf
      PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
      FspSecPlatformLib|$(PLATFORM_SI_PACKAGE)/Library/PlatformSecLib/FspMPlatformSecLib.inf
      NULL|$(PLATFORM_SI_PACKAGE)/Library/BasePlatformFspMultiPhaseLib/BasePlatformFspMultiPhaseLib.inf
      NULL|MdePkg/Library/StackCheckLibNull/StackCheckLibNull.inf
    <PcdsFixedAtBuild>
      gSiPkgTokenSpaceGuid.PcdFspValidatePeiServiceTablePointer|FALSE
      gIntelFsp2PkgTokenSpaceGuid.PcdMultiPhaseNumberOfPhases|0
  }
  IntelFsp2Pkg/FspSecCore/Fsp24SecCoreS.inf {
    <LibraryClasses>
      MmPciLib|$(SILICON_FSP_PATH)/Library/MmPciCf8Lib/MmPciCf8Lib.inf
      PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
      FspSecPlatformLib|$(PLATFORM_SI_PACKAGE)/Library/PlatformSecLib/FspSPlatformSecLib.inf
      NULL|$(PLATFORM_SI_PACKAGE)/Library/BasePlatformFspMultiPhaseLib/BasePlatformFspMultiPhaseLib.inf
      NULL|MdePkg/Library/StackCheckLibNull/StackCheckLibNull.inf
    <PcdsFixedAtBuild>
      gSiPkgTokenSpaceGuid.PcdFspValidatePeiServiceTablePointer|FALSE
      gIntelFsp2PkgTokenSpaceGuid.PcdMultiPhaseNumberOfPhases|1
  }

!if gSiPkgTokenSpaceGuid.PcdSignedFspEnable == TRUE
  #
  # FSP Loader
  # FspLoaderPeim shares APIs to FspSecCoreO use to save size.
  #
  $(FSP_PACKAGE)/FspLoaderPeim/FspLoaderPeim.inf {
    <LibraryClasses>
!if $(TARGET) == DEBUG
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!endif
    <PcdsFixedAtBuild>
      gSiPkgTokenSpaceGuid.PcdFspValidatePeiServiceTablePointer|FALSE
  }
!endif

  #
  # SEC Core
  #
  $(FSP_PACKAGE)/FspSecCore/FspSecCoreO.inf {
    <LibraryClasses>
      NULL|MdePkg/Library/StackCheckLibNull/StackCheckLibNull.inf
!if gSiPkgTokenSpaceGuid.PcdSignedFspEnable == TRUE
      FspLoadComponentsLib|$(FSP_PACKAGE)/Library/FspLoadComponentsLib/FspLoadComponents.inf
!else
      FspLoadComponentsLib|$(FSP_PACKAGE)/Library/FspLoadComponentsLibNull/FspLoadComponentsNull.inf
!endif
    <PcdsFixedAtBuild>
      gSiPkgTokenSpaceGuid.PcdFspValidatePeiServiceTablePointer|FALSE
  }

  #
  # Reset Vector
  #
  UefiCpuPkg/ResetVector/Vtf0/Vtf0.inf {
    <BuildOptions>
      *_*_X64_NASMB_FLAGS = -DPAGE_TABLE_1G -DARCH_X64
  }

  #
  # PEI Core
  #
  MdeModulePkg/Core/Pei/PeiMain.inf {
    <LibraryClasses>
      MmPciLib|$(SILICON_FSP_PATH)/Library/MmPciCf8Lib/MmPciCf8Lib.inf
      PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x27
!if (gSiPkgTokenSpaceGuid.PcdSignedFspEnable == TRUE)
# todo : Review how GS flag impacting PeiMain execution in Post memory phase.
!if $(TARGET) == DEBUG
    <BuildOptions>
      MSFT: DEBUG_*_*_CC_FLAGS = /GS- /volatileMetadata
!endif
!endif
  }

  #
  # PCD
  #
  MdeModulePkg/Universal/PCD/Pei/Pcd.inf {
    <LibraryClasses>
      PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
      DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
      NULL|$(SILICON_FSP_PATH)/Library/FspEmptyLib/FspEmptyLib.inf
  }
  #
  # FSP Binary Components
  #
  $(SILICON_FSP_PATH)/FspGlobalDataInit/Pei/FspGlobalDataInitPei.inf {
    <LibraryClasses>
      MmPciLib|$(SILICON_FSP_PATH)/Library/MmPciCf8Lib/MmPciCf8Lib.inf
      PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
      DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  }
!if gSiPkgTokenSpaceGuid.PcdFspVEnable == TRUE
  MdeModulePkg/Universal/ReportStatusCodeRouter/Pei/ReportStatusCodeRouterPei.inf {
    <LibraryClasses>
      NULL|$(SILICON_FSP_PATH)/Library/FspEmptyLib/FspEmptyLib.inf
  }
  $(SILICON_FSP_PATH)/FspInit/Pei/FspVInit/FspMVInit.inf
  $(SILICON_FSP_PATH)/FspInit/Pei/FspVInit/FspSVInit.inf
!endif

$(SILICON_FSP_PATH)/FspRsc2PostCodeMapInit/Pei/FspRsc2PostCodeMapServicePei.inf {
    <LibraryClasses>
      PostCodeLib|$(SILICON_FSP_PATH)/Library/BasePostCodeLibPort80TraceHub/BasePostCodeLibPort80TraceHub.inf
  }
!if $(TARGET) == DEBUG
  $(SILICON_FSP_PATH)/FspDebugInit/Pei/FspDebugServicePei.inf {
    # library for display FSP information
    <LibraryClasses>
      MmPciLib|$(SILICON_FSP_PATH)/Library/MmPciCf8Lib/MmPciCf8Lib.inf
      PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
      SerialPortLib|$(SILICON_FSP_PATH)/Library/SerialPortLib/SerialPortLib.inf
    <PcdsFixedAtBuild>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x2F
    <PcdsPatchableInModule>
      gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80080046
  }
!endif

  $(FSP_PACKAGE)/FspHeader/FspHeader.inf
  $(SILICON_PRODUCT_PATH)/SiInit/Pei/SiInitPreMem.inf {
    <LibraryClasses>
      NULL|$(FSP_PACKAGE)/Library/PeiFspReportMigrationInfoLib/PeiFspReportMigrationInfoLib.inf
  }
  MdeModulePkg/Universal/PcatSingleSegmentPciCfg2Pei/PcatSingleSegmentPciCfg2Pei.inf {
    <LibraryClasses>
      NULL|$(SILICON_FSP_PATH)/Library/FspEmptyLib/FspEmptyLib.inf
  }

  #
  # CpuMp PEIM for MpService PPI
  # In Dispatch mode: if MpServicePpi already installed by boot loader
  #                   this will be skipped.
  # In API mode     : If external MpService stack passed by UPD, this will
  #                   be skipped
  #
  # If not skipped, this module must be dispatched earlier than PostMem
  # silicon policy PPI installed, for PostMem silicon initialization to
  # consume MpService PPI.
  #
  UefiCpuPkg/CpuMpPei/CpuMpPei.inf {
    <LibraryClasses>
      NULL|$(PLATFORM_SI_PACKAGE)/Library/PeiReadyToInstallMpLib/PeiReadyToInstallMpLib.inf
!if $(TARGET) == DEBUG
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!endif
  }

  #
  # CpuFeatures PEIM
  #
  UefiCpuPkg/CpuFeatures/CpuFeaturesPei.inf {
    <LibraryClasses>
      RegisterCpuFeaturesLib|UefiCpuPkg/Library/RegisterCpuFeaturesLib/PeiRegisterCpuFeaturesLib.inf
!if $(TARGET) == DEBUG
      DebugLib|MdePkg/Library/BaseDebugLibSerialPort/BaseDebugLibSerialPort.inf
!endif
      NULL|UefiCpuPkg/Library/CpuCommonFeaturesLib/CpuCommonFeaturesLib.inf
      NULL|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuInit/LibraryPrivate/PeiCpuSpecificFeaturesLib/PeiCpuSpecificFeaturesLib.inf
  }

!if (gPantherLakeFspPkgTokenSpaceGuid.PcdAcpiInFspEnabled == TRUE)
  $(SILICON_PRODUCT_PATH)/AcpiInit/Pei/AcpiTablePei.inf
!endif

  $(SILICON_PRODUCT_PATH)/EarlyDevices/EarlyDevicesFsp.inf {
    <PcdsFixedAtBuild>
      gSiPkgTokenSpaceGuid.PcdLpssUartDebugEnable|0
    <LibraryClasses>
      NULL|$(SILICON_FSP_PATH)/Library/FspEmptyLib/FspEmptyLib.inf
  }
  $(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/SocInit/GpioV2ServicesInit/Pei/GpioV2PtlPcdPpiInitPei.inf {
    <LibraryClasses>
      NULL|$(SILICON_FSP_PATH)/Library/FspEmptyLib/FspEmptyLib.inf
  }

  !include $(PLATFORM_SI_PACKAGE)/IpBlock/Me/Heci/HeciDriverPei.dsc

  $(SILICON_PRODUCT_PATH)/SiInit/Pei/SiInit.inf

  MdeModulePkg/Core/DxeIplPeim/DxeIpl.inf {
    <LibraryClasses>
      DebugAgentLib|MdeModulePkg/Library/DebugAgentLibNull/DebugAgentLibNull.inf
      ResetSystemLib|MdeModulePkg/Library/BaseResetSystemLibNull/BaseResetSystemLibNull.inf
      NULL|$(SILICON_FSP_PATH)/Library/FspEmptyLib/FspEmptyLib.inf
  }

  MdeModulePkg/Universal/ResetSystemPei/ResetSystemPei.inf {
    <LibraryClasses>
      ResetSystemLib|$(PLATFORM_SI_PACKAGE)/Pch/Library/BaseResetSystemLib/BaseResetSystemLib.inf
      NULL|$(SILICON_FSP_PATH)/Library/FspEmptyLib/FspEmptyLib.inf
  }
  $(SILICON_FSP_PATH)/FspInit/Pei/FspInitPreMem.inf {
    <LibraryClasses>
      NULL|$(PLATFORM_SI_PACKAGE)/Library/BasePlatformFspMultiPhaseLib/BasePlatformFspMultiPhaseLib.inf
  }
  $(SILICON_FSP_PATH)/FspPcdInit/Pei/FspPcdInit.inf
  $(SILICON_FSP_PATH)/FspInit/Pei/FspInit.inf {
    <LibraryClasses>
      CpuInitPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuInit/LibraryPrivate/PeiCpuInitPolicyLib/PeiCpuInitPolicyLib.inf
      PeiBootGuardPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/BootGuard/LibraryPrivate/PeiBootGuardPolicyLib/PeiBootGuardPolicyLib.inf
      CpuPowerManagementPolicyLib|$(PLATFORM_SI_PACKAGE)/IpBlock/CpuPowerManagement/LibraryPrivate/PeiCpuPowerManagementPolicyLib/PeiCpuPowerManagementPolicyLib.inf
      NULL|$(PLATFORM_SI_PACKAGE)/Library/BasePlatformFspMultiPhaseLib/BasePlatformFspMultiPhaseLib.inf
  }

  #
  # Silicon policy should be dependency on all of Notify phase code for dispatching later than
  # SiInit done.
  #
  $(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/SocInit/Dxe/PtlPcdPchInitFsp.inf

  $(PLATFORM_SI_PACKAGE)/IpBlock/Me/HeciInit/Pei/HeciInitApi.inf

  IntelFsp2Pkg/FspNotifyPhase/FspNotifyPhasePeim.inf {
    <LibraryClasses>
      NULL|$(PLATFORM_SI_PACKAGE)/Library/PeiSiliconPolicyInitLibDependency/PeiPostMemSiliconPolicyInitLibDependency.inf
      NULL|$(SILICON_FSP_PATH)/Library/FspEmptyLib/FspEmptyLib.inf
  }
  $(SILICON_FSP_PATH)/FspInit/Pei/FspS3Notify.inf
  $(SILICON_FSP_PATH)/FspEndOfPei2/FspEndOfPei2Peim.inf

[Components.X64]
  $(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/SocInit/Dxe/PtlPcdPchInitDxeFsp.inf
  $(PLATFORM_SI_PACKAGE)/IpBlock/Me/HeciInit/Dxe/HeciInitDxeFsp.inf
  $(SILICON_PRODUCT_PATH)/SiInit/Dxe/SiInitDxeFsp.inf

!include $(SILICON_PRODUCT_PATH)/SiPkgBuildOption.dsc
!include $(FSP_PACKAGE)/FspPkgBuildOption.dsc

###################################################################################################
#
# BuildOptions Section - Define the module specific tool chain flags that should be used as
#                        the default flags for a module. These flags are appended to any
#                        standard flags that are defined by the build process. They can be
#                        applied for any modules or only those modules with the specific
#                        module style (EDK or EDKII) specified in [Components] section.
#
###################################################################################################
[BuildOptions]
# Append build options for EDK and EDKII drivers (= is Append, == is Replace)

[BuildOptions.Common.EDKII]

!if gPantherLakeFspPkgTokenSpaceGuid.PcdMiniBiosEnable == TRUE
  DEFINE EDKII_DSC_MINIBIOS_BUILD_OPTIONS = -DMRC_MINIBIOS_BUILD
!else
  DEFINE EDKII_DSC_MINIBIOS_BUILD_OPTIONS =
!endif

  DEFINE EDKII_DSC_FEATURE_BUILD_OPTIONS = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS) $(EDKII_DSC_MINIBIOS_BUILD_OPTIONS)

  DEFINE EDKII_DSC_ALL_BUILD_OPTIONS = $(EDKII_DSC_FEATURE_BUILD_OPTIONS)

!if "GCC" in $(TOOL_CHAIN_TAG)
GCC:   *_*_IA32_ASM_FLAGS   = $(EDKII_DSC_FEATURE_BUILD_OPTIONS) -Wa,--defsym
  *_*_IA32_PP_FLAGS = $(EDKII_DSC_ALL_BUILD_OPTIONS)
!else
GCC:   *_*_IA32_ASM_FLAGS   = $(EDKII_DSC_FEATURE_BUILD_OPTIONS)
GCC:   *_*_IA32_PP_FLAGS = $(EDKII_DSC_ALL_BUILD_OPTIONS)
!endif
MSFT:  *_*_IA32_ASM_FLAGS   = $(EDKII_DSC_FEATURE_BUILD_OPTIONS)
  *_*_IA32_NASM_FLAGS  = $(EDKII_DSC_FEATURE_BUILD_OPTIONS) -DUSE_SSE41_FLAG=1
  *_*_IA32_CC_FLAGS    = $(EDKII_DSC_ALL_BUILD_OPTIONS)
  *_*_IA32_VFRPP_FLAGS = $(EDKII_DSC_ALL_BUILD_OPTIONS)
  *_*_IA32_APP_FLAGS   = $(EDKII_DSC_ALL_BUILD_OPTIONS)

!if "GCC" in $(TOOL_CHAIN_TAG)
GCC:   *_*_X64_ASM_FLAGS   = $(EDKII_DSC_FEATURE_BUILD_OPTIONS) -Wa,--defsym
!else
GCC:   *_*_X64_ASM_FLAGS   = $(EDKII_DSC_FEATURE_BUILD_OPTIONS)
!endif
MSFT:  *_*_X64_ASM_FLAGS   = $(EDKII_DSC_FEATURE_BUILD_OPTIONS)
  *_*_X64_NASM_FLAGS  = $(EDKII_DSC_FEATURE_BUILD_OPTIONS) -DUSE_SSE41_FLAG=1
  *_*_X64_CC_FLAGS    = $(EDKII_DSC_ALL_BUILD_OPTIONS)
  *_*_X64_VFRPP_FLAGS = $(EDKII_DSC_ALL_BUILD_OPTIONS)
  *_*_X64_APP_FLAGS   = $(EDKII_DSC_ALL_BUILD_OPTIONS)
