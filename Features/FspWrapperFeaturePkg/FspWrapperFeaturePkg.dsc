## @file
# This is a build description for FSP Wrapper Feature package
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

[Defines]
  PLATFORM_NAME                  = FspWrapperFeaturePkg
  PLATFORM_GUID                  = 48215BA6-56F8-43DA-B47F-F4CFB604FE36
  PLATFORM_VERSION               = 0.5
  DSC_SPECIFICATION              = 0x0001001C
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = X64
  BUILD_TARGETS                  = DEBUG|RELEASE|NOOPT
  SKUID_IDENTIFIER               = DEFAULT
  DXE_ARCH                       = X64

  DEFINE CHECK_FSP_ERROR_INFO_HOB_DXE_DEBUG_LIB = MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf

[PcdsFixedAtBuild]
  gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection|1

################################################################################
#
# Library Class section - list of all Library Classes needed by this feature.
#
################################################################################
[LibraryClasses]
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLibRepStr/BaseMemoryLibRepStr.inf
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  FspErrorInfoLib|OneSiliconPkg/Library/FspErrorInfoLib/PeiDxeFspErrorInfoLib.inf
  ReportStatusCodeLib|MdePkg/Library/BaseReportStatusCodeLibNull/BaseReportStatusCodeLibNull.inf
  MemoryAllocationLib|MdeModulePkg/Library/BaseMemoryAllocationLibNull/BaseMemoryAllocationLibNull.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  RegisterFilterLib|MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf
  HobLib|MdeModulePkg/Library/BaseHobLibNull/BaseHobLibNull.inf
  PlatformFspsVerificationLib|FspWrapperFeaturePkg/Library/PlatformFspsVerificationLibNull/PlatformFspsVerificationLibNull.inf

[LibraryClasses.common.DXE_DRIVER, LibraryClasses.common.UEFI_DRIVER]
  DxeServicesTableLib|MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf

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

!include FspWrapperFeaturePkg/Include/Dsc/FspWrapperFeature.dsc
