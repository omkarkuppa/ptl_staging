## @file
# FreeSpaceSkipFvCopyHostTest DSC include file for host based test DSC
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2024 Intel Corporation.
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

  PLATFORM_NAME                   = FreeSpaceSkipFvCopyHostTest.dsc
  PLATFORM_GUID                   = 4ad240fe-bd09-41fe-bd2b-82c5ffff38f5
  PLATFORM_VERSION                = 0.1
  DSC_SPECIFICATION               = 0x00010005
  OUTPUT_DIRECTORY                = Build/FreeSpaceSkipFvCopyPei/HostTest
  SUPPORTED_ARCHITECTURES         = IA32|X64
  BUILD_TARGETS                   = NOOPT
  SKUID_IDENTIFIER                = DEFAULT

  #
  # Include UnitTestFrameworkPkgHost.dsc.inc
  #
!include UnitTestFrameworkPkg/UnitTestFrameworkPkgHost.dsc.inc

[PcdsFixedAtBuild]
  gSiPkgTokenSpaceGuid.PcdSpiDmaEnable                 |FALSE

[LibraryClasses]
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  #
  # Entry point
  #
  PeimEntryPoint|MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf
  #
  # PI
  #
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  PeiServicesTablePointerLib|MdePkg/Library/PeiServicesTablePointerLib/PeiServicesTablePointerLib.inf

[Components]
  #
  # Build Unit test Components with HOST_APPLICATION.
  #
  FvCopyFeaturePkg/FreeSpaceSkipFvCopyPei/UnitTest/FreeSpaceSkipFvCopyHostTest.inf