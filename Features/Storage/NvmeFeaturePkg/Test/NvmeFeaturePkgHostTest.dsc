## @file
# NvmeFeaturePkgHostTest DSC include file for host based test DSC.
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
  PLATFORM_NAME                  = NvmeFeaturePkgHostTest
  PLATFORM_GUID                  = A72A2C34-0E4F-4352-8EC5-FCD3F186DC2F
  PLATFORM_VERSION               = 0.5
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/NvmeFeaturePkg/HostTest
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
  #
  # NvmePciHcPeiTests
  #
  NvmeFeaturePkg/NvmePciHcPei/UnitTest/NvmePciHcPeiHost.inf
  #
  # NvmeEraseDxe
  #
  NvmeFeaturePkg/NvmeEraseDxe/UnitTest/NvmeEraseDxeHost.inf
  #
  # PowerLossNotifyDxe
  #
  NvmeFeaturePkg/PowerLossNotifyDxe/UnitTest/PowerLossNotifyDxeHost.inf
