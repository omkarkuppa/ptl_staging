## @file
# This is a build description for the SataFeaturePkg.
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
  PLATFORM_NAME                  = SataFeaturePkg
  PLATFORM_GUID                  = 8A2C1FE8-E7C5-4FC1-B1C6-D7E8A71EAA3A
  PLATFORM_VERSION               = 0.5
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = IA32|X64
  BUILD_TARGETS                  = DEBUG|RELEASE|NOOPT
  SKUID_IDENTIFIER               = DEFAULT
  PEI_ARCH                       = IA32
  DXE_ARCH                       = X64

#
# Set Feature PCDs
# These PCD must be set before the core include files, libraries.
#
[PcdsFeatureFlag]
  gSataFeaturePkgTokenSpaceGuid.PcdSataFeatureEnable    |TRUE

[PcdsFixedAtBuild]
  !if gSataFeaturePkgTokenSpaceGuid.PcdSataFeatureEnable == TRUE
    gSataFeaturePkgTokenSpaceGuid.PcdSataPciScanEnable  |TRUE
    gSataFeaturePkgTokenSpaceGuid.PcdSataEraseEnable    |TRUE
  !else
    gSataFeaturePkgTokenSpaceGuid.PcdSataPciScanEnable  |FALSE
    gSataFeaturePkgTokenSpaceGuid.PcdSataEraseEnable    |FALSE
  !endif

################################################################################
#
# Library Class section - list of all Library Classes needed by this feature.
#
################################################################################

!include MdePkg/MdeLibs.dsc.inc

[LibraryClasses]
  #######################################
  # Edk2 Packages
  #######################################
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLibRepStr/BaseMemoryLibRepStr.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  PciCf8Lib|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
  PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
  PciSegmentLib|MdePkg/Library/BasePciSegmentLibPci/BasePciSegmentLibPci.inf

[LibraryClasses.common.PEI_CORE,LibraryClasses.common.PEIM]
  #######################################
  # Edk2 Packages
  #######################################
  HobLib|MdePkg/Library/PeiHobLib/PeiHobLib.inf
  MemoryAllocationLib|MdePkg/Library/PeiMemoryAllocationLib/PeiMemoryAllocationLib.inf
  PcdLib|MdePkg/Library/PeiPcdLib/PeiPcdLib.inf
  PeimEntryPoint|MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf
  PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
  PeiServicesTablePointerLib|MdePkg/Library/PeiServicesTablePointerLibIdt/PeiServicesTablePointerLibIdt.inf

[LibraryClasses.common.DXE_DRIVER, LibraryClasses.common.UEFI_DRIVER]
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf

#
# This package always builds the feature.
#
!include SataFeaturePkg/Include/SataFeature.dsc
