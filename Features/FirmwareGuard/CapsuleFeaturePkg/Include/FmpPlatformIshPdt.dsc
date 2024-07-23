## @file
# FmpDxe driver for ISH PDT system firmware update.
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
      FILE_GUID = $(FMP_CLIENT_PLATFORM_SYSTEM_ISH_PDT)
    <PcdsFixedAtBuild>
      #
      # Unicode name string that is used to populate FMP Image Descriptor for this capsule update module
      #
      gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceImageIdName|L"$(FMP_CLIENT_PLATFORM_NAME) ISH PDT System Firmware Device"

      #
      # ESRT and FMP Lowest Support Version for this capsule update module
      #
      gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceBuildTimeLowestSupportedVersion|0x00000000

      gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceProgressWatchdogTimeInSeconds|0

      #
      # Capsule Update Progress Bar Color.  Set to lavender (RGB) (128, 128, 255)
      #
      gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceProgressColor|0x008080FF

    <LibraryClasses>
      FmpDeviceLib|CapsuleFeaturePkg/Library/FmpDeviceLibIshPdt/FmpDeviceLibIshPdt.inf
  }
