## @file
# FmpDxe driver for TSN MAC Address update.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2022 Intel Corporation.
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

  FmpDevicePkg/FmpDxe/FmpDxe.inf {
    <Defines>
      #
      # ESRT and FMP GUID for system firmware capsule update
      #
      FILE_GUID = $(FMP_CLIENT_PLATFORM_SYSTEM_TSN_MAC_ADDR)
    <PcdsFixedAtBuild>
      #
      # Unicode name string that is used to populate FMP Image Descriptor for this capsule update module
      #
      gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceImageIdName|L"MAC Address Sub Region"

      #
      # ESRT and FMP Lowest Support Version for this capsule update module
      gPlatformModuleTokenSpaceGuid.PcdSystemFirmwareFmpLowestSupportedVersion|0

      gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceProgressWatchdogTimeInSeconds|0

      #
      # Capsule Update Progress Bar Color.  Set to blue (RGB) (0, 0, 255)
      #
      gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceProgressColor|0x000000FF

      #
      # Certificates used to authenticate capsule update image
      #
      !include $(PLATFORM_FEATURES_PATH)/CapsuleUpdate/FmpCertificate.dsc

    <LibraryClasses>
      #
      # Generic libraries that are used "as is" by all FMP modules
      #
      FmpPayloadHeaderLib|FmpDevicePkg/Library/FmpPayloadHeaderLibV1/FmpPayloadHeaderLibV1.inf
      FmpAuthenticationLib|SecurityPkg/Library/FmpAuthenticationLibPkcs7/FmpAuthenticationLibPkcs7.inf
      FmpDependencyLib|FmpDevicePkg/Library/FmpDependencyLib/FmpDependencyLib.inf
      #
      # Platform specific capsule policy library
      #
      CapsuleUpdatePolicyLib|$(PLATFORM_FEATURES_PATH)/CapsuleUpdate/Library/CapsuleUpdatePolicyLib/CapsuleUpdatePolicyLib.inf
      #
      # Platform specific library that processes a capsule and updates the FW storage device
      #
      FmpDeviceLib|$(PLATFORM_FEATURES_PATH)/CapsuleUpdate/Library/FmpDeviceLibTsnMacAddr/FmpDeviceLibTsnMacAddr.inf
      SubRegionUpdateLib|$(PLATFORM_FEATURES_PATH)/CapsuleUpdate/Library/SubRegionUpdateLib/SubRegionUpdateLib.inf
      FmpDependencyCheckLib|FmpDevicePkg/Library/FmpDependencyCheckLibNull/FmpDependencyCheckLibNull.inf
      FmpDependencyDeviceLib|FmpDevicePkg/Library/FmpDependencyDeviceLibNull/FmpDependencyDeviceLibNull.inf
  }
