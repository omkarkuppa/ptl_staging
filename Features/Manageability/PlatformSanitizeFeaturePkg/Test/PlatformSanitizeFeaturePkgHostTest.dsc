## @file
# PlatformSanitizeFeaturePkg DSC include file for host based test DSC.
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
  PLATFORM_NAME                  = PsHostTest
  PLATFORM_GUID                  = A4C7E811-F5E4-48DF-9107-49E3165247E9
  PLATFORM_VERSION               = 0.5
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/PsFeaturePkg/HostTest
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
  TimerLib|MdePkg/Library/BaseTimerLibNullTemplate/BaseTimerLibNullTemplate.inf
  TcgStorageCoreLib|SecurityPkg/Library/TcgStorageCoreLib/TcgStorageCoreLib.inf
  TcgStorageOpalLib|SecurityPkg/Library/TcgStorageOpalLib/TcgStorageOpalLib.inf

[Components]
  #
  # Build Unit test Components with HOST_APPLICATION.
  #
  #
  # PlatformOpalDxe Unit test
  #
  PlatformSanitizeFeaturePkg/StorageSanitize/PlatformOpalDxe/UnitTest/PlatformOpalDxeHost.inf {
    <LibraryClasses>
    PcdLib|PlatformSanitizeFeaturePkg/Test/CommonMock/MockPcdLib/PcdLibMock.inf
    UefiBootServicesTableLib|PlatformSanitizeFeaturePkg/Test/CommonMock/MockBootServices/MockBootServices.inf
  }
  #
  # PlatformSanitizeDxe Unit test
  #
  PlatformSanitizeFeaturePkg/PlatformSanitizeDxe/UnitTest/PlatformSanitizeUnitTest.inf
