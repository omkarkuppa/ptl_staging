## @file
#  Platform Sanitize Feature Component Description File.
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
##

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
[Packages]
  PlatformSanitizeFeaturePkg/PlatformSanitizeFeaturePkg.dec

[LibraryClasses]
!if gPsFeatureModuleTokenSpaceGuid.PcdPlatformSanitizeFeatureEnable == TRUE
  AhciCommandLib|PlatformSanitizeFeaturePkg/StorageSanitize/Library/AhciCommandLibNull/AhciCommandLibNull.inf
  EraseServiceLib|PlatformSanitizeFeaturePkg/StorageSanitize/Library/EraseServiceLibNull/EraseServiceLibNull.inf
  StoragePlatformEventLib|PlatformSanitizeFeaturePkg/StorageSanitize/Library/StoragePlatformEventLibNull/StoragePlatformEventLibNull.inf
  PsLpeLib|PlatformSanitizeFeaturePkg/Library/PsLpeLib/PsLpeLib.inf
  PsCsmeAccessLib|PlatformSanitizeFeaturePkg/Library/PsCsmeAccessLib/PsCsmeAccessLib.inf
!if gPsFeatureModuleTokenSpaceGuid.PcdPlatformSanitizeRpeSupport == TRUE
  PsCsmeAccessRpeLib|PlatformSanitizeFeaturePkg/Library/PsCsmeAccessRpeLib/PsCsmeAccessRpeLib.inf
!else
  PsCsmeAccessRpeLib|PlatformSanitizeFeaturePkg/Library/PsCsmeAccessRpeLibNull/PsCsmeAccessRpeLibNull.inf
!endif
  StorageServiceLib|PlatformSanitizeFeaturePkg/StorageSanitize/Library/StorageServiceLib/StorageServiceLib.inf
  PsStorageLib|PlatformSanitizeFeaturePkg/Library/PsStorageLib/PsStorageLib.inf
  SanitizeUiLib|PlatformSanitizeFeaturePkg/Library/SanitizeUiLib/SanitizeUiLib.inf
!endif

[Components.X64]
!if gPsFeatureModuleTokenSpaceGuid.PcdPlatformSanitizeFeatureEnable == TRUE
  PlatformSanitizeFeaturePkg/PlatformSanitizeSetupDxe/PlatformSantizeSetup.inf
  PlatformSanitizeFeaturePkg/PlatformSanitizeDxe/PlatformSanitizeDxe.inf
  PlatformSanitizeFeaturePkg/StorageSanitize/PlatformOpalDxe/PlatformOpalDxe.inf
  PlatformSanitizeFeaturePkg/StorageSanitize/StorageSanitizeDxe/StorageSanitizeDxe.inf
!endif
