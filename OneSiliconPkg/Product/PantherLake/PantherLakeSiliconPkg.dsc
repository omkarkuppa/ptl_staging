## @file
#  Component description file for the PantherLake SiPkg DSC file.
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
#@par Specification Reference:
#
##

[Defines]
  PLATFORM_NAME = OneSiliconPkg
  PLATFORM_GUID = CCD38CA7-61D3-4185-9CDA-A9FDF209CB31
  PLATFORM_VERSION = 0.4
  DSC_SPECIFICATION = 0x00010005
  OUTPUT_DIRECTORY = Build/OneSiliconPkg
  SUPPORTED_ARCHITECTURES = IA32|X64
  BUILD_TARGETS = DEBUG|RELEASE
  SKUID_IDENTIFIER = DEFAULT

  DEFINE   PLATFORM_SI_PACKAGE        = OneSiliconPkg
  DEFINE   SILICON_PRODUCT_PATH       = OneSiliconPkg/Product/PantherLake

  #
  # Silicon On/Off feature are defined here
  #
  !include $(SILICON_PRODUCT_PATH)/SiPkgPcdInit.dsc

  #
  # Definition for Build Flag
  #
  !include $(SILICON_PRODUCT_PATH)/SiPkgBuildOption.dsc

  #
  # Basic/Common library instances provided by MdePkg.
  #
  !include MdePkg/MdeLibs.dsc.inc

[LibraryClasses.common]
  #
  # Entry point
  #
  PeiCoreEntryPoint|MdePkg/Library/PeiCoreEntryPoint/PeiCoreEntryPoint.inf
  PeimEntryPoint|MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf
  DxeCoreEntryPoint|MdePkg/Library/DxeCoreEntryPoint/DxeCoreEntryPoint.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
  PeCoffExtraActionLib|MdePkg/Library/BasePeCoffExtraActionLibNull/BasePeCoffExtraActionLibNull.inf

  #
  # Basic
  #
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLibRepStr/BaseMemoryLibRepStr.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  CpuLib|MdePkg/Library/BaseCpuLib/BaseCpuLib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  PciSegmentLib|$(PLATFORM_SI_PACKAGE)/Library/BasePciSegmentMultiSegLibPci/BasePciSegmentMultiSegLibPci.inf
  PciLib|MdePkg/Library/BasePciLibPciExpress/BasePciLibPciExpress.inf
  PciCf8Lib|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
  PciExpressLib|$(PLATFORM_SI_PACKAGE)/Library/BasePciExpressMultiSegLib/BasePciExpressMultiSegLib.inf
  CacheMaintenanceLib|MdePkg/Library/BaseCacheMaintenanceLib/BaseCacheMaintenanceLib.inf
  PeCoffLib|MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf
  PeCoffGetEntryPointLib|MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
  PeiVrFruLib|$(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/VoltageRegulator/LibraryPrivate/PeiVrFruLib/PeiVrFruLib.inf

  #
  # UEFI & PI
  #
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  UefiRuntimeLib|MdePkg/Library/UefiRuntimeLib/UefiRuntimeLib.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  PeiServicesTablePointerLib|MdePkg/Library/PeiServicesTablePointerLibIdt/PeiServicesTablePointerLibIdt.inf
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  DxeServicesTableLib|MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf

  S3BootScriptLib|MdePkg/Library/BaseS3BootScriptLibNull/BaseS3BootScriptLibNull.inf
  S3IoLib|MdePkg/Library/BaseS3IoLib/BaseS3IoLib.inf
  S3PciLib|MdePkg/Library/BaseS3PciLib/BaseS3PciLib.inf
  S3PciSegmentLib|MdePkg/Library/BaseS3PciSegmentLib/BaseS3PciSegmentLib.inf

  UefiUsbLib|MdePkg/Library/UefiUsbLib/UefiUsbLib.inf
  UefiScsiLib|MdePkg/Library/UefiScsiLib/UefiScsiLib.inf
  SynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf

  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf

  #
  # Misc
  #
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  TimerLib|MdePkg/Library/BaseTimerLibNullTemplate/BaseTimerLibNullTemplate.inf
  PostCodeLib|MdePkg/Library/BasePostCodeLibDebug/BasePostCodeLibDebug.inf
  ReportStatusCodeLib|MdePkg/Library/BaseReportStatusCodeLibNull/BaseReportStatusCodeLibNull.inf
  MtrrLib|UefiCpuPkg/Library/MtrrLib/MtrrLib.inf
  RngLib|MdePkg/Library/BaseRngLib/BaseRngLib.inf
  LocalApicLib|UefiCpuPkg/Library/BaseXApicX2ApicLib/BaseXApicX2ApicLib.inf

#####################################################################################################

#
# Silicon Init Common Library
#
!include $(SILICON_PRODUCT_PATH)/SiPkgCommonLib.dsc
ConfigBlockLib|IntelSiliconPkg/Library/BaseConfigBlockLib/BaseConfigBlockLib.inf

[LibraryClasses.IA32]
#
# PEI phase common
#
  PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  ExtractGuidedSectionLib|MdePkg/Library/PeiExtractGuidedSectionLib/PeiExtractGuidedSectionLib.inf
  CpuCacheInfoLib|UefiCpuPkg/Library/CpuCacheInfoLib/PeiCpuCacheInfoLib.inf
  MicrocodeLib|UefiCpuPkg/Library/MicrocodeLib/MicrocodeLib.inf

#####################################################################################################################################

#
# Silicon Init Pei Library
#
!include $(SILICON_PRODUCT_PATH)/SiPkgPeiLib.dsc

[LibraryClasses.IA32.SEC]
  GpioHelpersLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Gpio/LibraryPrivate/BaseGpioHelpersLibNull/BaseGpioHelpersLibNull.inf

[LibraryClasses.IA32.PEIM, LibraryClasses.X64.PEIM]
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLibBase.inf

[LibraryClasses.X64]
 #
 # DXE phase common
 #
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  ExtractGuidedSectionLib|MdePkg/Library/DxeExtractGuidedSectionLib/DxeExtractGuidedSectionLib.inf

#
# Hsti
#
  HstiLib|MdePkg/Library/DxeHstiLib/DxeHstiLib.inf

###################################################################################################
#
# Silicon Init Dxe Library
#
!include $(SILICON_PRODUCT_PATH)/SiPkgDxeLib.dsc

[LibraryClasses.X64.PEIM]

[LibraryClasses.X64.DXE_CORE]
  HobLib|MdePkg/Library/DxeCoreHobLib/DxeCoreHobLib.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf

[LibraryClasses.X64.DXE_SMM_DRIVER]
  SmmServicesTableLib|MdePkg/Library/SmmServicesTableLib/SmmServicesTableLib.inf
  MemoryAllocationLib|MdePkg/Library/SmmMemoryAllocationLib/SmmMemoryAllocationLib.inf
  SmmIoLib|MdePkg/Library/SmmIoLib/SmmIoLib.inf
  SmmMemLib|MdePkg/Library/SmmMemLib/SmmMemLib.inf
  MmServicesTableLib|MdePkg/Library/MmServicesTableLib/MmServicesTableLib.inf

[LibraryClasses.X64.SMM_CORE]

[LibraryClasses.X64.UEFI_DRIVER]
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf

[LibraryClasses.X64.UEFI_APPLICATION]
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf

#
# Overclocking
#
!if gSiPkgTokenSpaceGuid.PcdOverclockEnable == TRUE
  $(PLATFORM_SI_PACKAGE)/Fru/PtlCDie/Overclocking/LibraryPrivate/BaseOcFruLib/BaseOcFruLib.inf
  $(PLATFORM_SI_PACKAGE)/IpBlock/Overclocking/LibraryPrivate/PeiDxeSmmOcCommonLib/PeiDxeSmmOcCommonLib.inf
  $(PLATFORM_SI_PACKAGE)/IpBlock/Overclocking/Library/DxeOcPlatformLib/DxeOcPlatformLib.inf
!else
  $(PLATFORM_SI_PACKAGE)/IpBlock/Overclocking/LibraryPrivate/PeiDxeSmmOcCommonLibNull/PeiDxeSmmOcCommonLibNull.inf
  $(PLATFORM_SI_PACKAGE)/IpBlock/Overclocking/Library/DxeOcPlatformLibNull/DxeOcPlatformLibNull.inf
!endif
[Components.IA32]
!include $(SILICON_PRODUCT_PATH)/SiPkgPei.dsc
$(PLATFORM_SI_PACKAGE)/Fru/PtlPcd/LibraryPrivate/PtlPcdTcssPmcLib/PtlPcdTcssPmcLib.inf

[Components.X64]
!include $(SILICON_PRODUCT_PATH)/SiPkgDxe.dsc
#
# AcpiTables
#
$(PLATFORM_SI_PACKAGE)/Pch/AcpiTables/Dsdt/PchAcpiTablesSelfTest.inf

#
# DMI
#
!if gSiPkgTokenSpaceGuid.PcdDmiEnable == TRUE
  $(PLATFORM_SI_PACKAGE)/IpBlock/Dmi/Library/PeiDmiTaskDispatchLib/PeiDmiTaskDispatchLib.inf
!else
  $(PLATFORM_SI_PACKAGE)/IpBlock/Dmi/Library/PeiDmiTaskDispatchLibNull/PeiDmiTaskDispatchLibNull.inf
!endif
  $(PLATFORM_SI_PACKAGE)/IpBlock/Dmi/LibraryPrivate/PeiDmiPolicyLib/PeiDmiPolicyLib.inf
  $(PLATFORM_SI_PACKAGE)/IpBlock/Dmi/LibraryPrivate/PeiDmiInitLib/PeiDmiInitLib.inf
  $(PLATFORM_SI_PACKAGE)/IpBlock/MemoryInit/Library/PeiMemPolicyLib/PeiMemPolicyLibPtl.inf
