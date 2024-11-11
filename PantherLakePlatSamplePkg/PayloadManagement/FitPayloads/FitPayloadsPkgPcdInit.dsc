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

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################

[PcdsFixedAtBuild]
  gMinPlatformPkgTokenSpaceGuid.PcdFlashFvMicrocodeSize|0x00080000
  gMinPlatformPkgTokenSpaceGuid.PcdFlashFvMicrocodeOffset|0x00000000
  gCapsuleFeaturePkgTokenSpaceGuid.PcdBiosResiliencyEnable|FALSE