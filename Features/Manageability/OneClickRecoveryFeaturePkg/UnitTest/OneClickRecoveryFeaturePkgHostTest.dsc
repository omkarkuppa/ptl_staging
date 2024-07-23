## @file
#  OneClickRecoveryFeaturePkgHostTest DSC include file for host based test DSC
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
  PLATFORM_NAME                  = OneClickRecoveryFeaturePkgHostTest.dsc
  PLATFORM_GUID                  = 6FD1D722-2403-4c9b-807D-9531815932AA
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/OcrFeaturePkg/HostTest
  SUPPORTED_ARCHITECTURES        = IA32|X64
  BUILD_TARGETS                  = NOOPT
  SKUID_IDENTIFIER               = DEFAULT

  #
  # Include UnitTestFrameworkPkgHost.dsc.inc
  #
!include UnitTestFrameworkPkg/UnitTestFrameworkPkgHost.dsc.inc

[LibraryClasses]
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  HobLib|MdeModulePkg/Library/BaseHobLibNull/BaseHobLibNull.inf
  DxeAsfLib|OneSiliconPkg/IpBlock/Me/Library/DxeAsfLib/DxeAsfLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  TimerLib|Ec/EcFeaturePkg/Test/CommonMock/Library/BaseTimerLibMock/BaseTimerLibMock.inf
  DxeAmtSupportLib|OneSiliconPkg/IpBlock/Amt/Library/DxeAmtSupportLibNull/DxeAmtSupportLibNull.inf
  DxeAmtHeciLib|OneClickRecoveryFeaturePkg/UnitTest/Library/DxeAmtHeciLibMock/DxeAmtHeciLibMock.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  BootGuardLib|OneSiliconPkg/IpBlock/BootGuard/LibraryPrivate/PeiDxeSmmBootGuardLibNull/PeiDxeSmmBootGuardLibNull.inf

[Components]
  #
  # Build Unit test Components with HOST_APPLICATION.
  #
  OneClickRecoveryFeaturePkg/UnitTest/OneClickRecoveryUnitTest.inf
