## @file
# This is a build description file for the UsbC CapsuleUpdate advanced feature.
# This file should be included into another package DSC file to build this feature.
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
# @par Specification Reference:
##

[Packages]
  UsbCCapsuleFeaturePkg/UsbCCapsuleFeaturePkg.dec

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  DEFINE FMP_CLIENT_PLATFORM_TBT_RETIMER          = 2FE2CBFC-B9AA-4A93-AB5B-40173B581C42
  DEFINE FMP_CLIENT_PLATFORM_DISCRETE_TBT         = 86A885EE-D71E-2ED6-0FC1-9D6CCC9677EB

################################################################################
#
# Library Class section - list of all Library Classes needed by this feature.
#
################################################################################
[LibraryClasses.common.DXE_DRIVER,LibraryClasses.common.UEFI_APPLICATION]

  TbtNvmRetimerUpdateLib|UsbCCapsuleFeaturePkg/Library/TbtNvmRetimerUpdateLib/TbtNvmRetimerUpdateLib.inf
  UsbcCapsuleDebugLib|UsbCCapsuleFeaturePkg/Library/UsbcCapsuleDebugLib/UsbcCapsuleDebugLib.inf

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

#
# Feature PEI Components
#

# @todo: Change below line to [Components.$(PEI_ARCH)] after https://bugzilla.tianocore.org/show_bug.cgi?id=2308
#        is completed.
[Components.IA32]
  #####################################
  # UsbC Capsule Feature Package
  #####################################


#
# Feature DXE Components
#

# @todo: Change below line to [Components.$(DXE_ARCH)] after https://bugzilla.tianocore.org/show_bug.cgi?id=2308
#        is completed.
[Components.X64]
  #####################################
  # UsbC Capsule Feature Package
  #####################################

  UsbCCapsuleFeaturePkg/UsbCRetimerDxe/UsbCRetimerDxe.inf
#
# UsbCRetimer FMP
#

  FmpDevicePkg/FmpDxe/FmpDxe.inf {
    <Defines>
      FILE_GUID = $(FMP_CLIENT_PLATFORM_TBT_RETIMER)

    <PcdsFixedAtBuild>
      gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceImageIdName|L"UsbC Retimer Device"
      gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceBuildTimeLowestSupportedVersion|0x00000000
      gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceProgressWatchdogTimeInSeconds|0
      gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceProgressColor|0x00FF8000

      # Using Pkcs7 test keys from EDKII open source. This cannot be used in product.
      !include BaseTools/Source/Python/Pkcs7Sign/TestRoot.cer.gFmpDevicePkgTokenSpaceGuid.PcdFmpDevicePkcs7CertBufferXdr.inc

    <LibraryClasses>
      FmpDeviceLib|UsbCCapsuleFeaturePkg/Library/FmpDeviceLib/UsbCRetimer/FmpDeviceLibTbtRetimer.inf
  }

#
# UsbCDiscreteTBT FMP
#

  FmpDevicePkg/FmpDxe/FmpDxe.inf {
    <Defines>
      FILE_GUID = $(FMP_CLIENT_PLATFORM_DISCRETE_TBT)

    <PcdsFixedAtBuild>
      gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceImageIdName|L"UsbC Discrete Thunderbolt Controller Device"
      gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceBuildTimeLowestSupportedVersion|0x00000000
      gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceProgressWatchdogTimeInSeconds|0
      gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceProgressColor|0x00FF8000

      # Using Pkcs7 test keys from EDKII open source. This cannot be used in product.
      !include BaseTools/Source/Python/Pkcs7Sign/TestRoot.cer.gFmpDevicePkgTokenSpaceGuid.PcdFmpDevicePkcs7CertBufferXdr.inc

    <LibraryClasses>
      FmpDeviceLib|UsbCCapsuleFeaturePkg/Library/FmpDeviceLib/DiscreteTbt/FmpDeviceLibDiscreteTbt.inf
  }

#
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
