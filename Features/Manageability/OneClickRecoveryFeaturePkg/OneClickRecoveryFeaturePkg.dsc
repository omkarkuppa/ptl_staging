## @file
# This is a build description for the One Click Recovery feature package.
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

[Defines]
  PLATFORM_NAME                  = OneClickRecoveryFeaturePkg
  PLATFORM_GUID                  = DD1C33C5-E40B-4F0C-9C76-79A1C3F734CB
  PLATFORM_VERSION               = 0.5
  DSC_SPECIFICATION              = 0x0001001C
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = X64
  BUILD_TARGETS                  = DEBUG|RELEASE|NOOPT
  SKUID_IDENTIFIER               = DEFAULT
  DXE_ARCH                       = X64

[PcdsFeatureFlag]
  gOneClickRecoveryFeaturePkgTokenSpaceGuid.PcdOneClickRecoveryEnable|TRUE
  gOneClickRecoveryFeaturePkgTokenSpaceGuid.PcdOneClickRecoveryCapabilities|0xF
  gOneClickRecoveryFeaturePkgTokenSpaceGuid.PcdWifiProfileSyncEnable|TRUE

[LibraryClasses]
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  NetLib|NetworkPkg/Library/DxeNetLib/DxeNetLib.inf
  HttpLib|NetworkPkg/Library/DxeHttpLib/DxeHttpLib.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  UefiBootManagerLib|MdeModulePkg/Library/UefiBootManagerLib/UefiBootManagerLib.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  DxeAsfLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Library/DxeAsfLib/DxeAsfLib.inf
  DxeAsfHeciLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Me/Library/DxeAsfHeciLib/DxeAsfHeciLib.inf
  DxeAmtHeciLib|$(PLATFORM_SI_PACKAGE)/IpBlock/Amt/Library/DxeAmtHeciLib/DxeAmtHeciLib.inf
  BootGuardLib|OneSiliconPkg/IpBlock/BootGuard/LibraryPrivate/PeiDxeSmmBootGuardLib/PeiDxeSmmBootGuardLib.inf
  OemOcrLib|$(PLATFORM_SI_PACKAGE)/Library/OemOneClickRecoveryLib/OemOneClickRecoveryLib.inf

!include OneClickRecoveryFeaturePkg/Include/Dsc/OneClickRecoveryFeature.dsc