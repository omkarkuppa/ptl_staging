## @file
#  FspPkgHostTest DSC include file for host based test DSC.
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
#
##

[Defines]
  PLATFORM_NAME                  = PantherLakeFspPkgHostTest.dsc
  PLATFORM_GUID                  = 5995e629-a9c4-404b-90c3-00ca45df6e23
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x0001001C
  OUTPUT_DIRECTORY               = Build/PantherLakeFspPkg/HostTest
  SUPPORTED_ARCHITECTURES        = X64
  BUILD_TARGETS                  = NOOPT
  SKUID_IDENTIFIER               = DEFAULT
  DEFINE  SILICON_PRODUCT_PATH   = OneSiliconPkg/Product/PantherLake
  DEFINE  PLATFORM_SI_PACKAGE    = OneSiliconPkg

[PcdsFixedAtBuild]
  gEfiMdePkgTokenSpaceGuid.PcdFixedDebugPrintErrorLevel   | 0x80000047

  #
  # Include UnitTestFrameworkPkgHost.dsc.inc and MdeLibs.dsc.inc
  #
!include UnitTestFrameworkPkg/UnitTestFrameworkPkgHost.dsc.inc
!include MdePkg/MdeLibs.dsc.inc

  #
  # Include PantherLake PCD init dsc
  #
!include OneSiliconPkg/Product/PantherLake/SiPkgPcdInit.dsc

  #
  # Include PantherLake Library
  #
[LibraryClasses.common]
!include OneSiliconPkg/Product/PantherLake/SiPkgCommonLib.dsc
!include OneSiliconPkg/Product/PantherLake/SiPkgPeiLib.dsc
!include OneSiliconPkg/Product/PantherLake/SiPkgBuildOption.dsc


  #
  # Include Unit Test components
  #
!include OneSiliconPkg/IpBlock/CpuUnitTest/CpuUnitTest.dsc.inc
