## @file
# Common libs that consumbed by platform FMP devices.
# This file should be included into another package DSC file to build this feature.
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
#@par Specification Reference:
##

[Packages]
  CapsuleFeaturePkg/CapsuleFeaturePkg.dec

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  DEFINE FMP_CLIENT_PLATFORM_SYSTEM_MONO          = 7FEB1D5D-33F4-48D3-BD11-C4B36B6D0E57 # gFmpDeviceMonolithicDefaultGuid
  DEFINE FMP_CLIENT_PLATFORM_SYSTEM_BIOS          = 6C8E136F-D3E6-4131-AC32-4687CB4ABD27 # gFmpDeviceBiosDefaultGuid
  DEFINE FMP_CLIENT_PLATFORM_SYSTEM_EXTENDED_BIOS = FD0AADC7-9696-4795-8F34-53069F759FB0 # gFmpDeviceExtendedBiosDefaultGuid
  DEFINE FMP_CLIENT_PLATFORM_SYSTEM_BTGACM        = 4E88068B-41B2-4E05-893C-DB0B43F7D348 # gFmpDeviceBtGAcmDefaultGuid
  DEFINE FMP_CLIENT_PLATFORM_SYSTEM_UCODE         = 69585D92-B50A-4AD7-B265-2EB1AE066574 # gFmpDeviceuCodeDefaultGuid
  DEFINE FMP_CLIENT_PLATFORM_SYSTEM_FSP           = 5E8F2C7B-6D4F-4A9E-BF6D-8A7E9B0F3C2A # gFmpDeviceFspDefaultGuid
  DEFINE FMP_CLIENT_PLATFORM_SYSTEM_ME            = 0EAB05C1-766A-4805-A039-3081DE0210C7 # gFmpDeviceMeDefaultGuid
  DEFINE FMP_CLIENT_PLATFORM_SYSTEM_EC            = 3DD84775-EC79-4ECB-8404-74DE030C3F77 # gFmpDeviceEcDefaultGuid
  DEFINE FMP_CLIENT_PLATFORM_SYSTEM_IFWI          = F9FA5980-95F5-491A-9583-136D7F26D63E # gFmpDeviceIfwiDefaultGuid
  DEFINE FMP_CLIENT_PLATFORM_SYSTEM_ISH_PDT       = 76CA0AD8-4A14-4389-B7E5-FD88791762AD # gFmpDeviceIshPdtDefaultGuid
  DEFINE FMP_CLIENT_PLATFORM_SYSTEM_TSN_MAC_ADDR  = 6FEE88FF-49ED-48F1-B77B-EAD15771ABE7 # gFmpDevicePlatformTsnMacAddrGuid
  DEFINE FMP_CLIENT_PLATFORM_FOXVILLE             = 6E36078B-6B6F-42BC-981E-4C639AFA4D6D # gFmpDeviceFoxvilleDefaultGuid

  !ifndef FMP_CLIENT_PLATFORM_NAME
    DEFINE FMP_CLIENT_PLATFORM_NAME = Client
  !endif

  !ifndef CSME_GENERATION
    DEFINE CSME_GENERATION = 0
  !endif

[PcdsFixedAtBuild]

  gFmpDevicePkgTokenSpaceGuid.PcdFmpDeviceProgressWatchdogTimeInSeconds|0

################################################################################
#
# Library Class section - list of all Library Classes needed by this feature.
#
################################################################################
[LibraryClasses]
  ResiliencySupportLib|CapsuleFeaturePkg/Library/PeiDxeSmmResiliencySupportLib/PeiDxeSmmResiliencySupportLib.inf
  HashSupportLib|CapsuleFeaturePkg/LibraryPrivate/PeiDxeSmmHashSupportLib/PeiDxeSmmHashSupportLib.inf
  FitHelperLib|CapsuleFeaturePkg/LibraryPrivate/PeiDxeSmmFitHelperLib/PeiDxeSmmFitHelperLib.inf
  AcmHelperLib|CapsuleFeaturePkg/LibraryPrivate/PeiDxeSmmAcmHelperLib/PeiDxeSmmAcmHelperLib.inf

[LibraryClasses.common.PEIM]
  #  #######################################
  #  # Feature Packages
  #  #######################################
  TopSwapSupportLib|CapsuleFeaturePkg/LibraryPrivate/TopSwapSupportPeiLib/TopSwapSupportPeiLib.inf

[LibraryClasses.common.DXE_DRIVER]
  #
  #  #######################################
  #  # Edk2 Packages
  #  #######################################
  #
  # Generic libraries that are used "as is" by all FMP modules
  #
  FmpPayloadHeaderLib|FmpDevicePkg/Library/FmpPayloadHeaderLibV1/FmpPayloadHeaderLibV1.inf
  FmpAuthenticationLib|SecurityPkg/Library/FmpAuthenticationLibPkcs7/FmpAuthenticationLibPkcs7.inf
  FmpDependencyLib|FmpDevicePkg/Library/FmpDependencyLib/FmpDependencyLib.inf
  FmpDependencyCheckLib|FmpDevicePkg/Library/FmpDependencyCheckLibNull/FmpDependencyCheckLibNull.inf
  FmpDependencyDeviceLib|FmpDevicePkg/Library/FmpDependencyDeviceLibNull/FmpDependencyDeviceLibNull.inf
  CapsuleUpdatePolicyLib|FmpDevicePkg/Library/CapsuleUpdatePolicyLibOnProtocol/CapsuleUpdatePolicyLibOnProtocol.inf

  #  #######################################
  #  # Feature Packages
  #  #######################################
  DisplayUpdateProgressLib|CapsuleFeaturePkg/Library/PlatformDisplayUpdateProgressLib/PlatformDisplayUpdateProgressLib.inf

  BiosUpdateHelpersLib|CapsuleFeaturePkg/Library/BiosUpdateHelpersLib/BiosUpdateHelpersLib.inf
  PlatformFlashAccessLib|CapsuleFeaturePkg/Library/PlatformFlashAccessLib/PlatformFlashAccessLib.inf
  ComponentUpdateLib|CapsuleFeaturePkg/Library/ComponentUpdateLib/ComponentUpdateLib.inf
  SeamlessRecoverySupportLib|CapsuleFeaturePkg/Library/SeamlessRecoverySupportLib/SeamlessRecoverySupportLib.inf
  CapsuleUpdateResetLib|CapsuleFeaturePkg/Library/CapsuleUpdateResetLib/CapsuleUpdateResetLib.inf

  IniParsingLib|CapsuleFeaturePkg/LibraryPrivate/IniParsingLib/IniParsingLib.inf
  FvImageExtractionLib|CapsuleFeaturePkg/LibraryPrivate/FvImageExtractionLib/FvImageExtractionLib.inf
  ConfigProfileAccessLib|CapsuleFeaturePkg/LibraryPrivate/ConfigProfileAccessLib/ConfigProfileAccessLib.inf
  TopSwapSupportLib|CapsuleFeaturePkg/LibraryPrivate/TopSwapSupportDxeLib/TopSwapSupportDxeLib.inf
  CbntSvnCheckLib|CapsuleFeaturePkg/Library/CbntSvnCheckLib/CbntSvnCheckLib.inf

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
[Components.$(FSP_ARCH)]
  #####################################
  # Capsule Feature Package
  #####################################

  CapsuleFeaturePkg/PlatformInitRecoveryPei/PlatformInitRecoveryPei.inf {
  <LibraryClasses>
!if $(TARGET) == DEBUG
    # To support debugging capsule update within debug resiliency BIOS in PEI phase
    DebugLib|MdeModulePkg/Library/PeiDebugLibDebugPpi/PeiDebugLibDebugPpi.inf
!endif
    PeiPlatformRecoveryLib|CapsuleFeaturePkg/Library/PeiPlatformRecoveryLib/PeiPlatformRecoveryLib.inf
  }

  CapsuleFeaturePkg/NvmeBasedRecovery/NvmeRecoveryPei/NvmeRecoveryPei.inf {
  <LibraryClasses>
!if $(TARGET) == DEBUG
    # To support debugging capsule update within debug resiliency BIOS in PEI phase
    DebugLib|MdeModulePkg/Library/PeiDebugLibDebugPpi/PeiDebugLibDebugPpi.inf
!endif
  }

#
# Feature DXE Components
#

# @todo: Change below line to [Components.$(DXE_ARCH)] after https://bugzilla.tianocore.org/show_bug.cgi?id=2308
#        is completed.
[Components.X64]
  #####################################
  # Capsule Feature Package
  #####################################

  !include CapsuleFeaturePkg/Include/FmpPlatformMonolithic.dsc
  !include CapsuleFeaturePkg/Include/FmpPlatformBios.dsc
  !include CapsuleFeaturePkg/Include/FmpPlatformExtendedBios.dsc
  !include CapsuleFeaturePkg/Include/FmpPlatformBtGAcm.dsc
  !include CapsuleFeaturePkg/Include/FmpPlatformuCode.dsc
  !include CapsuleFeaturePkg/Include/FmpPlatformFsp.dsc
  !include CapsuleFeaturePkg/Include/FmpPlatformMe.dsc
  !include CapsuleFeaturePkg/Include/FmpPlatformEc.dsc
  !include CapsuleFeaturePkg/Include/FmpPlatformIshPdt.dsc
  !include CapsuleFeaturePkg/Include/FmpPlatformIfwi.dsc
  !include CapsuleFeaturePkg/Include/FmpPlatformFoxville.dsc
!if gSiPkgTokenSpaceGuid.PcdTsnSupport == 0x1
  !include CapsuleFeaturePkg/Include/FmpPlatformTsnMacAddr.dsc
!endif

  CapsuleFeaturePkg/BiosResiliencyDxe/BiosResiliencyDxe.inf
  CapsuleFeaturePkg/MeResiliencyDxe/MeResiliencyDxe.inf
  CapsuleFeaturePkg/NvmeBasedRecovery/NvmeRecoveryDxe/NvmeRecoveryDxe.inf

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
