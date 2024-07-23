## @file
#  CnvFeaturePkgHostTest DSC include file for host based test DSC.
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
# @par Specification
#
##

[Defines]
  PLATFORM_NAME                  = CnvFeaturePkgHostTest.dsc
  PLATFORM_GUID                  = 1443A470-F02F-4601-A7D0-AAB73139DD6F
  PLATFORM_VERSION               = 0.5
  DSC_SPECIFICATION              = 0x0001001C
  OUTPUT_DIRECTORY               = Build/CnvFeaturePkg/HostTest
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
  DxeServicesTableLib|MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf
  VariablePolicyHelperLib|MdeModulePkg/Library/VariablePolicyHelperLib/VariablePolicyHelperLib.inf
  PcdLib|CnvFeaturePkg/Test/CommonMock//Library/BasePcdLibMock/BasePcdLibMock.inf

[Components]
  #
  # Build Unit test Components with HOST_APPLICATION.
  #
  CnvFeaturePkg/CnvUefiConfigVariables/UnitTest/CnvUefiConfigVariables2Host.inf {
    <PcdsFixedAtBuild>
      gCnvFeaturePkgTokenSpaceGuid.PcdCnvUefiVarVersion|2
    # <LibraryClasses>
    #   PcdLib|CnvFeaturePkg/Test/CommonMock//Library/BasePcdLibMock/BasePcdLibMock.inf
  }
  CnvFeaturePkg/CnvVfrSetupMenu/UnitTest/CnvVfrSetupMenuHost.inf

[PcdsFixedAtBuild]
  #
  # For CnvUefiPolicyInitDxe test case
  #
  gCnvFeaturePkgTokenSpaceGuid.PcdCnvSetupMenu|TRUE
