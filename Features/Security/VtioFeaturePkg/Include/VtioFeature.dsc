## @file
# This is a build description for the VTIO advanced feature.
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
#@par Specification Reference:
##

[Packages]
  VtioFeaturePkg/VtioFeaturePkg.dec

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

[LibraryClasses.common.DXE_DRIVER, LibraryClasses.common.UEFI_DRIVER]
  PchPciBdfLib|OneSiliconPkg/Pch/Library/BasePchPciBdfLib/BasePchPciBdfLib.inf

################################################################################
#
# Component section - list of all components that need built for this feature.
#
# Note: The EDK II DSC file is not used to specify how compiled binary images get placed
#       into firmware volume images. This section is just a list of modules to compile from
#       source into UEFI-compliant binaries.
#       It is the FDF file that contains information on combining binary files into firmware
#       volume images, whose concept is beyond UEFI and is described in PI specification.
#       There may also be modules listed in this section that are not required in the FDF file,
#       When a module listed here is excluded from FDF file, then UEFI-compliant binary will be
#       generated for it, but the binary will not be put into any firmware volume.
#
################################################################################

[Components.X64]
  VtioFeaturePkg/VtioSetup/VtioSetup.inf
  VtioFeaturePkg/VtioDxe/VtioDxe.inf

###################################################################################################
#
# BuildOptions Section - Define the module specific tool chain flags that should be used as
#                        the default flags for a module. These flags are appended to any
#                        standard flags that are defined by the build process. They can be
#                        applied for any modules or only those modules with the specific
#                        module style (EDK or EDKII) specified in [Components] section.
#
#                        For advanced features, it is recommended to enable [BuildOptions] in
#                        the applicable INF file so it does not affect the whole board package
#                        build when this DSC file is active.
#
###################################################################################################
[BuildOptions]
