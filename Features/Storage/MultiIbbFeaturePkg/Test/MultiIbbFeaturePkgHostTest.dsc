## @file
# MultiIbbFeaturePkgHostTest DSC include file for host based test DSC
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
#@par Specification Reference:
#
##

[Defines]
  PLATFORM_NAME                  = MultiIbbFeaturePkgHostTest.dsc
  PLATFORM_GUID                  = CC33ACC1-B0C2-48D1-A9C2-A954A360C9A2
  PLATFORM_VERSION               = 0.5
  DSC_SPECIFICATION              = 0x0001001C
  OUTPUT_DIRECTORY               = Build/MultiIbbFeaturePkg/HostTest
  SUPPORTED_ARCHITECTURES        = IA32|X64
  BUILD_TARGETS                  = NOOPT
  SKUID_IDENTIFIER               = DEFAULT

#
# Include UnitTestFrameworkPkgHost.dsc.inc
#
!include UnitTestFrameworkPkg/UnitTestFrameworkPkgHost.dsc.inc

[LibraryClasses]
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  RngLib|MdePkg/Library/BaseRngLib/BaseRngLib.inf
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/UnitTestHostBaseCryptLib.inf
  OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  RegisterFilterLib|MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf
  AccessNemConfigLib|MultiIbbFeaturePkg/Library/AccessNemConfigLib/AccessNemConfigLib.inf

[PcdsFixedAtBuild]
  gMultiIbbFeaturePkgTokenSpaceGuid.PcdMultiIbbInfoCount|1
  gMultiIbbFeaturePkgTokenSpaceGuid.PcdMultiIbbFeatureEnable|FALSE

[Components]
  #
  # Build Unit test Components with HOST_APPLICATION.
  #
  MultiIbbFeaturePkg/MultiIbbService/UnitTest/MultiIbbServiceHost_Ibbnp1.inf {
    <LibraryClasses>
      PeiServicesLib|MultiIbbFeaturePkg/MultiIbbService/UnitTest/PrivateMock/PeiServicesLibMock/PeiServicesLibMock.inf
      MtrrLib|MultiIbbFeaturePkg/MultiIbbService/UnitTest/PrivateMock/MtrrLibMock/MtrrLibMock.inf
    <PcdsFixedAtBuild>
      gMultiIbbFeaturePkgTokenSpaceGuid.PcdMultiIbbInfoCount|1
      gMultiIbbFeaturePkgTokenSpaceGuid.PcdMultiIbbFeatureEnable|FALSE
    <BuildOptions>
      *_*_*_CC_FLAGS = -D_PCD_VALUE_PcdMultiIbbFeatureEnable=0x0
  }
  MultiIbbFeaturePkg/MultiIbbService/UnitTest/MultiIbbServiceHost.inf {
    <LibraryClasses>
      PeiServicesLib|MultiIbbFeaturePkg/MultiIbbService/UnitTest/PrivateMock/PeiServicesLibMock/PeiServicesLibMock.inf
      MtrrLib|MultiIbbFeaturePkg/MultiIbbService/UnitTest/PrivateMock/MtrrLibMock/MtrrLibMock.inf
      PcdLib|MultiIbbFeaturePkg/Test/CommonMock/PcdLibMock.inf
    <PcdsFixedAtBuild>
      gMultiIbbFeaturePkgTokenSpaceGuid.PcdMultiIbbInfoCount|4
      gMultiIbbFeaturePkgTokenSpaceGuid.PcdMultiIbbFeatureEnable|TRUE
    <BuildOptions>
      *_*_*_CC_FLAGS = -D_PCD_VALUE_PcdMultiIbbFeatureEnable=0x1
      *_*_*_CC_FLAGS = -D_PCD_VALUE_PcdCacheMissDetectEnable=0x1
  }


