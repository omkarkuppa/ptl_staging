## @file
# FmpDxe driver for Foxville update.
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
#  @par Specification Reference:
#  Foxville Ethernet Controller External Architecture Specification, version 0.99
##

  FmpDevicePkg/FmpDxe/FmpDxe.inf {
    <Defines>
      #
      # ESRT and FMP GUID for system firmware capsule update
      #
      FILE_GUID = $(FMP_CLIENT_PLATFORM_FOXVILLE)
    <PcdsFixedAtBuild>
      #
      # Unicode name string that is used to populate FMP Image Descriptor for this capsule update module
      #
      gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceImageIdName|L"Intel Ethernet Controller I225/I226"

      gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceProgressWatchdogTimeInSeconds|0

      #
      # Capsule Update Progress Bar Color.  Set to blue (RGB) (0, 0, 255)
      #
      gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceProgressColor|0x000000FF

      #
      # File GUID for the Foxville FMP device driver.
      #
      gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceImageTypeIdGuid|{GUID(gFmpDeviceFoxvilleDefaultGuid)}

    <LibraryClasses>
      #
      # Platform specific library that processes a capsule and updates the FW storage device
      #
      FmpDeviceLib|CapsuleFeaturePkg/Library/FmpDeviceLibFoxville/FmpDeviceLibFoxville.inf
  }
