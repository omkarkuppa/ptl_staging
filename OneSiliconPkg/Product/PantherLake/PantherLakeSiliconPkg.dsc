## @file
#  Component description file for the PantherLake SiPkg DSC file.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2016 Intel Corporation.
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
  PLATFORM_NAME = OneSiliconPkg
  PLATFORM_GUID = CCD38CA7-61D3-4185-9CDA-A9FDF209CB31
  PLATFORM_VERSION = 0.4
  DSC_SPECIFICATION = 0x00010005
  OUTPUT_DIRECTORY = Build/OneSiliconPkg
  SUPPORTED_ARCHITECTURES = X64
  BUILD_TARGETS = DEBUG|RELEASE

  #
  # Basic/Common library instances provided by MdePkg.
  #
  !include MdePkg/MdeLibs.dsc.inc

[LibraryClasses.common]
  #
  # Entry point
  #
  PeimEntryPoint|MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf

  #
  # Basic
  #
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLibRepStr/BaseMemoryLibRepStr.inf

  #
  # UEFI & PI
  #
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf

  #
  # Misc
  #
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf

[Components.X64]
