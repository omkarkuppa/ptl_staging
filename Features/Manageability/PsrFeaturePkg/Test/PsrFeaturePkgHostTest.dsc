## @file
# PsrFeaturePkgHostTest DSC include file for host based test DSC
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
  PLATFORM_NAME                  = PsrFeaturePkgHostTest.dsc
  PLATFORM_GUID                  = 6C38EEFD-E85B-4717-A470-788B48067CCD
  PLATFORM_VERSION               = 0.5
  DSC_SPECIFICATION              = 0x0001001C
  OUTPUT_DIRECTORY               = Build/PsrFeaturePkg/HostTest
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
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  RngLib|MdePkg/Library/BaseRngLib/BaseRngLib.inf
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/UnitTestHostBaseCryptLib.inf
  OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLib.inf

[Components]
  #
  # Build Unit test Components with HOST_APPLICATION.
  #
  PsrFeaturePkg/PsrDxe/UnitTest/PsrDxeHost.inf {
    <LibraryClasses>
      MeLib|PsrFeaturePkg/PsrDxe/UnitTest/PrivateMock/Library/MeLibMock/MeLibMock.inf
      MeUtilsLib|PsrFeaturePkg/PsrDxe/UnitTest/PrivateMock/Library/MeUtilsLibMock/MeUtilsLibMock.inf
      PcdLib|PsrFeaturePkg/Test/PcdLibMock.inf
  }
  PsrFeaturePkg/PsrSetup/UnitTest/PsrSetupHost.inf {
    <LibraryClasses>
      HiiLib|PsrFeaturePkg/PsrSetup/UnitTest/PrivateMock/Library/HiiLibMock/HiiLibMock.inf
  }
