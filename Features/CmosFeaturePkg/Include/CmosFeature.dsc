## @file
#  CMOS Feature Component Description File.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2021 Intel Corporation.
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

################################################################################
# Packages section
# Note:
#   It is required for the PCDs that only used in dsc files.
################################################################################
[Packages]
  CmosFeaturePkg/CmosFeaturePkg.dec

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]

################################################################################
#
# Library Class section - list of all Library Classes needed by this feature.
#
################################################################################
[LibraryClasses]
CmosAccessLib|BoardModulePkg/Library/CmosAccessLib/CmosAccessLib.inf
PlatformNvRamHookLib|CmosFeaturePkg/Library/PlatformNvRamHookLibCmos/PlatformNvRamHookLibCmos.inf
!if gCmosFeaturePkgTokenSpaceGuid.PcdPlatformCmosAccessSupport == TRUE
  PlatformCmosAccessLib|CmosFeaturePkg/Library/PlatformCmosAccessLib/PlatformCmosAccessLib.inf
!else
  PlatformCmosAccessLibNull|CmosFeaturePkg/Library/PlatformCmosAccessLibNull/PlatformCmosAccessLibNull.inf
!endif
