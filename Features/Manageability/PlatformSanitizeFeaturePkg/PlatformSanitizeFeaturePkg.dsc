## @file
# This is a build description for the Platform Sanitize feature.
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
#  @par Specification Reference:
##

[Defines]
  PLATFORM_NAME                  = PsFeaturePkg
  PLATFORM_GUID                  = 78A79496-7191-44D3-84B6-78BC4C4B0D3F
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x0001001C
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = X64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT

#
# Set Feature PCDs
# These PCD must be set before the core include files, libraries.
#
[PcdsFeatureFlag]
gPsFeatureModuleTokenSpaceGuid.PcdPlatformSanitizeFeatureEnable    |TRUE

[PcdsFixedAtBuild]
gPsFeatureModuleTokenSpaceGuid.PcdPlatformSanitizeRpeSupport       |TRUE

[PcdsDynamicExDefault]
  gEfiSecurityPkgTokenSpaceGuid.PcdSkipHddPasswordPrompt|FALSE
  gEfiSecurityPkgTokenSpaceGuid.PcdSkipOpalPasswordPrompt|FALSE

################################################################################
#
# Library Class section - list of all Library Classes needed by this feature.
#
################################################################################
[LibraryClasses]
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLibRepStr/BaseMemoryLibRepStr.inf
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  UefiHiiServicesLib|MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf
  MeUtilsLib|OneSiliconPkg/IpBlock/Me/Library/PeiDxeSmmMeUtilsLib/DxeSmmMeUtilsLib.inf
  MeInfoLib|OneSiliconPkg/IpBlock/Me/Library/MeInfoLibNull/MeInfoLibNull.inf
  PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
  PciSegmentLib|MdePkg/Library/BasePciSegmentLibPci/BasePciSegmentLibPci.inf
  TimerLib|MdePkg/Library/BaseTimerLibNullTemplate/BaseTimerLibNullTemplate.inf
  ConfigBlockLib|IntelSiliconPkg/Library/BaseConfigBlockLib/BaseConfigBlockLib.inf
  PostCodeLib|MdePkg/Library/BasePostCodeLibPort80/BasePostCodeLibPort80.inf
  RngLib|MdePkg/Library/BaseRngLib/BaseRngLib.inf
  PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
  PciCf8Lib|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  MeFwStsLib|OneSiliconPkg/IpBlock/Me/Library/BaseMeFwStsLib/BaseMeFwStsLib.inf
  DxeAmtSupportLib|OneSiliconPkg/IpBlock/Amt/Library/DxeAmtSupportLibNull/DxeAmtSupportLibNull.inf
  Tpm2CommandLib|SecurityPkg/Library/Tpm2CommandLib/Tpm2CommandLib.inf
  Tpm2DeviceLib|SecurityPkg/Library/Tpm2DeviceLibTcg2/Tpm2DeviceLibTcg2.inf

[LibraryClasses.common.DXE_DRIVER, LibraryClasses.common.UEFI_DRIVER]
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  RegisterFilterLib|MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf
  MeLib|OneSiliconPkg/IpBlock/Me/Library/DxeMeLib/DxeMeLib.inf
  DxeAsfLib|OneSiliconPkg/IpBlock/Me/Library/DxeAsfLib/DxeAsfLib.inf
  DxeAmtHeciLib|OneSiliconPkg/IpBlock/Amt/Library/DxeAmtHeciLib/DxeAmtHeciLib.inf
  DxeAsfHeciLib|OneSiliconPkg/IpBlock/Me/Library/DxeAsfHeciLib/DxeAsfHeciLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
  PchPciBdfLib|OneSiliconPkg/Pch/Library/BasePchPciBdfLib/BasePchPciBdfLib.inf
  TcgStorageCoreLib|SecurityPkg/Library/TcgStorageCoreLib/TcgStorageCoreLib.inf
  TcgStorageOpalLib|SecurityPkg/Library/TcgStorageOpalLib/TcgStorageOpalLib.inf

################################################################################
#
# Component section - list of all components that need built for this feature.
#
# Note: The EDK II DSC file is not used to specify how compiled binary images get placed
#       into firmware volume images. This section is just a list of modules to compile from
#       source into UEFI-compliant binaries.
#       It is the FDF file that contains information on combining binary files into firmware
#       volume images, whose concept is beyond UEFI and is described in PI specification.
#       There may also be modules listed in this section that are not required in the FDF file,
#       When a module listed here is excluded from FDF file, then UEFI-compliant binary will be
#       generated for it, but the binary will not be put into any firmware volume.
#
################################################################################


!include PlatformSanitizeFeaturePkg/Include/PlatformSanitizeFeature.dsc
