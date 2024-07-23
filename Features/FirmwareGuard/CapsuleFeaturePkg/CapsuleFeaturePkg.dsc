## @file
# This package provides functionality for Capsule Update/Recovery feature support on Intel RVP
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
# @par Specification Reference:
##

[Defines]
  PLATFORM_NAME                  = CapsuleFeaturePkg
  PLATFORM_GUID                  = F29D138A-1E65-4993-AAA4-BE684384DAD4
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = IA32|X64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  PEI_ARCH                       = IA32
  DXE_ARCH                       = X64

[PcdsFixedAtBuild]
  gSiPkgTokenSpaceGuid.PcdTsnSupport|0x0
  #
  # Certificates used to authenticate capsule update image
  #
  # Using Pkcs7 test keys from EDKII open source and 3K test keys from platform sample code. This cannot be used in product.
  !include CapsuleFeaturePkg/SigningKeys/TestRoot_TestRoo3K.inc
[PcdsDynamicExDefault]
  gEfiMdeModulePkgTokenSpaceGuid.PcdRecoveryFileName|L"FVMAIN.FV"
[LibraryClasses]
#
# Silicon Init Package
#
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  PeimEntryPoint|MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  PciSegmentLib|MdePkg/Library/BasePciSegmentLibPci/BasePciSegmentLibPci.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  TimerLib|MdePkg/Library/BaseTimerLibNullTemplate/BaseTimerLibNullTemplate.inf
  ConfigBlockLib|IntelSiliconPkg/Library/BaseConfigBlockLib/BaseConfigBlockLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  PostCodeLib|MdePkg/Library/BasePostCodeLibPort80/BasePostCodeLibPort80.inf
  OemHookStatusCodeLib|MdeModulePkg/Library/OemHookStatusCodeLibNull/OemHookStatusCodeLibNull.inf
  CpuLib|MdePkg/Library/BaseCpuLib/BaseCpuLib.inf
  ResetSystemLib|MdeModulePkg/Library/BaseResetSystemLibNull/BaseResetSystemLibNull.inf
  PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
  PciCf8Lib|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
  BaseCryptLib|CryptoPkg/Library/BaseCryptLibNull/BaseCryptLibNull.inf
  OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLib.inf
  IntrinsicLib|CryptoPkg/Library/IntrinsicLib/IntrinsicLib.inf
  RngLib|MdePkg/Library/BaseRngLib/BaseRngLib.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  VariablePolicyHelperLib|MdeModulePkg/Library/VariablePolicyHelperLib/VariablePolicyHelperLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  CpuPlatformLib|CapsuleFeaturePkg/Library/PeiDxeSmmCpuPlatformLibNull/PeiDxeSmmCpuPlatformLibNull.inf
  SpiAccessLib|CapsuleFeaturePkg/Library/PeiDxeSmmSpiAccessLibNull/PeiDxeSmmSpiAccessLibNull.inf
  VmdInfoLib|CapsuleFeaturePkg/Library/VmdInfoLibNull/VmdInfoLibNull.inf
  MeLib|OneSiliconPkg/IpBlock/Me/Library/DxeMeLib/DxeMeLib.inf
  MeInfoLib|OneSiliconPkg/IpBlock/Me/Library/MeInfoLibNull/MeInfoLibNull.inf
  MeUtilsLib|OneSiliconPkg/IpBlock/Me/Library/PeiDxeSmmMeUtilsLib/DxeSmmMeUtilsLib.inf
  MeFwStsLib|OneSiliconPkg/IpBlock/Me/Library/BaseMeFwStsLib/BaseMeFwStsLib.inf

[LibraryClasses.common]
  RegisterFilterLib|MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf

[LibraryClasses.common.PEIM]
  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  ReportStatusCodeLib|MdeModulePkg/Library/PeiReportStatusCodeLib/PeiReportStatusCodeLib.inf
  PeiServicesTablePointerLib|MdePkg/Library/PeiServicesTablePointerLib/PeiServicesTablePointerLib.inf
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/PeiCryptLib.inf
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  PlatformWdtLib|CapsuleFeaturePkg/Library/PlatformWdtLibNull/PlatformWdtLibNull.inf
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf

[LibraryClasses.common.DXE_DRIVER]
  RegisterFilterLib|MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/BaseCryptLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf

  #
  # Null libraries needed to be implemented by platform
  #
  PlatformBiosUpdateHookLib|CapsuleFeaturePkg/Library/PlatformBiosUpdateHookLibNull/PlatformBiosUpdateHookLibNull.inf
  PlatformEcUpdateHookLib|CapsuleFeaturePkg/Library/PlatformEcUpdateHookLibNull/PlatformEcUpdateHookLibNull.inf
  PlatformWdtLib|CapsuleFeaturePkg/Library/PlatformWdtLibNull/PlatformWdtLibNull.inf
  PlatformEcResetLib|CapsuleFeaturePkg/Library/PlatformEcResetLibNull/PlatformEcResetLibNull.inf
  PlatformMeUpdateHookLib|CapsuleFeaturePkg/Library/PlatformMeUpdateHookLibNull/PlatformMeUpdateHookLibNull.inf
  PlatformDxeBootStateHookLib|CapsuleFeaturePkg/Library/PlatformDxeBootStateHookLibNull/PlatformDxeBootStateHookLibNull.inf
  FoxvilleDeviceLib|CapsuleFeaturePkg/Library/FoxvilleDeviceLib/FoxvilleDeviceLibNull.inf

[LibraryClasses.common.DXE_SMM_DRIVER]
  ReportStatusCodeLib|MdeModulePkg/Library/SmmReportStatusCodeLib/SmmReportStatusCodeLib.inf
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/SmmCryptLib.inf
  MemoryAllocationLib|MdePkg/Library/SmmMemoryAllocationLib/SmmMemoryAllocationLib.inf

[Components]
  #
  # Modules
  #
  FmpDevicePkg/CapsuleUpdatePolicyDxe/CapsuleUpdatePolicyDxe.inf {
    <LibraryClasses>
      CapsuleUpdatePolicyLib|FmpDevicePkg/Library/CapsuleUpdatePolicyLibNull/CapsuleUpdatePolicyLibNull.inf
  }

#
# This package always builds the feature.
#
!include Include/CapsuleFeature.dsc
