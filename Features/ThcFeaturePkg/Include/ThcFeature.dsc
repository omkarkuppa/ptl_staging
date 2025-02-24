## @file
#  Description (DSC) File for THC.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2025 Intel Corporation.
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

[Packages]
  ThcFeaturePkg/ThcFeaturePkg.dec

[LibraryClasses]
  #######################################
  # ThcFeaturePkg Package
  #######################################

[Components.IA32]
  #####################################
  # Thc Feature Package
  #####################################

[Components.X64]
  #####################################
  # Thc Feature Package
  #####################################
!if gThcFeaturePkgTokenSpaceGuid.PcdThcHidFeatureEnable == TRUE
  ThcFeaturePkg/HidInterfaceDriver/HidDriver.inf
!endif

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
