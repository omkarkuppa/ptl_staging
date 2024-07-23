## @file
# CapsuleFeaturePkgHostTest DSC include file for host based test DSC
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
#@par Specification Reference:
#
##

[Defines]
  PLATFORM_NAME                  = CapsuleFeaturePkgHostTest.dsc
  PLATFORM_GUID                  = 04fbf19e-2a05-4239-bd4b-b0bfc69e7cf4
  PLATFORM_VERSION               = 0.5
  DSC_SPECIFICATION              = 0x0001001C
  OUTPUT_DIRECTORY               = Build/FirmwareGuard/CapsuleFeaturePkg/HostTest
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

[Components]
  #
  # Build Unit test Components with HOST_APPLICATION.
  #
  FirmwareGuard/CapsuleFeaturePkg/BiosResiliencyDxe/UnitTest/BiosResiliencyDxeHost.inf
  FirmwareGuard/CapsuleFeaturePkg/MeResiliencyDxe/UnitTest/MeResiliencyDxeHost.inf
  FirmwareGuard/CapsuleFeaturePkg/PlatformInitRecoveryPei/UnitTest/PlatformInitRecoveryPeiHost.inf
