## @file
#  This is the overall description file for the NonFitPayloadPkg. This includes other scope-specific
#  description files such PCD scope or build options scope to help build the overall Board
#  support for the project.
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
#  @par Specification
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                       = NonFitPayloadsPkg
  PLATFORM_GUID                       = EF89E595-7D9D-4422-A277-BDFB5AFD3E16
  PLATFORM_VERSION                    = 0.1
  DSC_SPECIFICATION                   = 0x00010005
  OUTPUT_DIRECTORY                    = Build/NonFitPayloadsPkg
  SUPPORTED_ARCHITECTURES             = IA32|X64
  BUILD_TARGETS                       = DEBUG|RELEASE
  FLASH_DEFINITION                    = Intel/PantherLakePlatSamplePkg/PayloadManagement/NonFitPayloads/NonFitPayloadsPkg.fdf

[Packages]
  Binaries/PantherLakeBinPkg/PantherLakeBinPkg.dec
  Intel/Features/FirmwareGuard/CapsuleFeaturePkg/CapsuleFeaturePkg.dec

[PcdsFixedAtBuild]
  gCapsuleFeaturePkgTokenSpaceGuid.PcdBiosResiliencyEnable|FALSE