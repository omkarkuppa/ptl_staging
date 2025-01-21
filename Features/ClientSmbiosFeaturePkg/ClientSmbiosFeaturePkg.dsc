## @file
#  Description (DSC) File for Client Smbios Feature.
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
#
##

[Defines]
  PLATFORM_NAME                  = ClientSmbiosFeaturePkg
  PLATFORM_GUID                  = B5083616-DFD4-4383-A81B-454FE7238EFF
  PLATFORM_VERSION               = 0.5
  DSC_SPECIFICATION              = 0x0001001C
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = IA32|X64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT

[Packages]
  MinPlatformPkg/MinPlatformPkg.dec

!include MinPlatformPkg/Include/Dsc/MinPlatformFeaturesPcd.dsc.inc
!include MinPlatformPkg/Include/Dsc/CoreCommonLib.dsc
!include MinPlatformPkg/Include/Dsc/CoreDxeLib.dsc

[PcdsFeatureFlag.X64]
  gMinPlatformPkgTokenSpaceGuid.PcdTpm2Enable|TRUE

!include ClientSmbiosFeaturePkg/Include/ClientSmbiosFeature.dsc