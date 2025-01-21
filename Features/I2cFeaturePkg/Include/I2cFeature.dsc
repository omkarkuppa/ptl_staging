## @file
#  This is a build description for the I2c feature.
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
##

###################################################################################################
#
# Components Section - list of the modules and components that will be processed by compilation
#                      tools and the EDK II tools to generate PE32/PE32+/Coff image files.
#
# Note: The EDK II DSC file is not used to specify how compiled binary images get placed
#       into firmware volume images. This section is just a list of modules to compile from
#       source into UEFI-compliant binaries.
#       It is the FDF file that contains information on combining binary files into firmware
#       volume images, whose concept is beyond UEFI and is described in PI specification.
#       Binary modules do not need to be listed in this section, as they should be
#       specified in the FDF file. For example: Shell binary (Shell_Full.efi), FAT binary (Fat.efi),
#       Logo (Logo.bmp), and etc.
#       There may also be modules listed in this section that are not required in the FDF file,
#       When a module listed here is excluded from FDF file, then UEFI-compliant binary will be
#       generated for it, but the binary will not be put into any firmware volume.
#
###################################################################################################
[Packages]
  I2cFeaturePkg/I2cFeaturePkg.dec

[LibraryClasses]
  #######################################
  # I2cFeaturePkg Package
  #######################################
  HidI2cPlatformSupportLib|I2cFeaturePkg/Library/DxeHidI2cPlatformSupportLibNull/DxeHidI2cPlatformSupportLibNull.inf

[Components.IA32]
  #####################################
  # I2C Feature Package
  #####################################

[Components.X64]
  #####################################
  # I2C Feature Package
  #####################################
!if gI2cFeaturePkgTokenSpaceGuid.PcdI2cFeatureEnable == TRUE
  I2cFeaturePkg/I2cPlatformDxe/I2cPlatformDxe.inf
!if gI2cFeaturePkgTokenSpaceGuid.PcdI2cTouchEnable == TRUE
  I2cFeaturePkg/I2cTouchPanelDxe/I2cTouchPanelDxe.inf
!endif
!if gI2cFeaturePkgTokenSpaceGuid.PcdI2cPssEnable == TRUE
  I2cFeaturePkg/I2cPssMonzaDxe/I2cPssMonzaDxe.inf
!endif
!if gI2cFeaturePkgTokenSpaceGuid.PcdI2cTouchCommonEnable == TRUE

!if gI2cFeaturePkgTokenSpaceGuid.PcdI2cTouchVersion == 0x2
  I2cFeaturePkg/I2cTouchAcpiDxe/I2cTouchCommonV2/I2cCommonDxe.inf
!else
  I2cFeaturePkg/I2cTouchAcpiDxe/I2cTouchCommon/I2cCommon.inf
!endif
  I2cFeaturePkg/I2cTouchAcpiDxe/I2cTouchDevAcpi/I2cTouchDev.inf
!endif
!endif

I2cFeaturePkg/I2cPowerMeter/I2cPowerMeterAcpiTable.inf

I2cFeaturePkg/I2cSensorPei/I2cSensorPei.inf

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
