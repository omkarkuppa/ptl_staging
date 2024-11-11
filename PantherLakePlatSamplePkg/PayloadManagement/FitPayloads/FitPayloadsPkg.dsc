## @file
#  This package provides Payload support for PantherLake
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
# @par Specification
##
[Defines]
  #
  # Set platform specific package/folder name, same as passed from PREBUILD script.
  # PLATFORM_PACKAGE would be the same as PLATFORM_NAME as well as package build folder
  # DEFINE only takes effect at R9 DSC and FDF.
  #
  DEFINE      PLATFORM_PACKAGE                = MinPlatformPkg
  DEFINE      PLATFORM_FULL_PACKAGE           = PantherLakePlatSamplePkg
  DEFINE      PLATFORM_SI_PACKAGE             = OneSiliconPkg
  DEFINE      SILICON_PRODUCT_PATH            = OneSiliconPkg/Product/PantherLake
  DEFINE      PLATFORM_FSP_BIN_PACKAGE        = PantherLakeFspBinPkg
  DEFINE      PLATFORM_RESTRICTED_PACKAGE     = PantherLakeRestrictedPkg
  DEFINE      PLATFORM_BOARD_PACKAGE          = PantherLakeBoardPkg
  DEFINE      PLATFORM_OPEN_BOARD_PACKAGE     = PantherLakeOpenBoardPkg
  DEFINE      PLATFORM_FEATURES_PATH          = $(PLATFORM_FULL_PACKAGE)/Features
  DEFINE      PLATFORM_BIN_PATH               = $(PLATFORM_FULL_PACKAGE)/InternalOnly
  DEFINE      PLATFORM_SV_PACKAGE             = SvRestricted/Platform/PantherLakeSvRestrictedPkg
  DEFINE      PLATFORM_BIN_PACKAGE            = Binaries/PantherLakeBinPkg
  DEFINE      COMMON_BIN_PACKAGE              = CommonBinPkg
  DEFINE      PLATFORM_BSP_PATH               = PantherLakeBoardPkg/BoardSupport

  !include FitPayloadsPkgPcdInit.dsc
  !include $(PLATFORM_BIN_PACKAGE)/Include/Dsc/PantherLakeBinPkgPcdInit.dsc

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                       = FitPayloadsPkg
  PLATFORM_GUID                       = 91AF567B-2B8D-41F6-A337-A67D50F2187F
  PLATFORM_VERSION                    = 0.1
  DSC_SPECIFICATION                   = 0x00010005
  OUTPUT_DIRECTORY                    = Build/FitPayloadsPkg
  SUPPORTED_ARCHITECTURES             = IA32|X64
  BUILD_TARGETS                       = DEBUG|RELEASE

!if (gCapsuleFeaturePkgTokenSpaceGuid.PcdBiosResiliencyEnable == FALSE) || ($(TARGET) == RELEASE)
  FLASH_DEFINITION                    = Intel/PantherLakePlatSamplePkg/PayloadManagement/FitPayloads/FitPayloadsPkg.fdf
!else
  FLASH_DEFINITION                    = Intel/PantherLakePlatSamplePkg/PayloadManagement/FitPayloads/FitPayloadsResPkg.fdf
!endif

[Packages]
  Edk2Platforms/Platform/Intel/MinPlatformPkg/MinPlatformPkg.dec
  Binaries/PantherLakeBinPkg/PantherLakeBinPkg.dec
  Intel/Features/FirmwareGuard/CapsuleFeaturePkg/CapsuleFeaturePkg.dec
