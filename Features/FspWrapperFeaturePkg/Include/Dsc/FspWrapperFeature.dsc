## @file FspWrapperFeature.dsc
#  This is a build description for the FSP Wrapper Package Driver and Library Modules.
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
# @par Specification
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
  FspWrapperFeaturePkg/FspWrapperFeaturePkg.dec

[LibraryClasses.IA32.SEC, LibraryClasses.IA32.PEIM, LibraryClasses.X64.SEC, LibraryClasses.X64.PEIM]
  FspWrapperPlatformLib|FspWrapperFeaturePkg/Library/PeiFspWrapperPlatformLib/PeiFspWrapperPlatformLib.inf
!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  # FSP Signing support is not enabled yet for API Mode
  PlatformFspsVerificationLib|FspWrapperFeaturePkg/Library/PlatformFspsVerificationLibNull/PlatformFspsVerificationLibNull.inf
!else
  PlatformFspsVerificationLib|FspWrapperFeaturePkg/Library/PeiPlatformFspVerificationLib/PeiPlatformFspsVerificationLib.inf
!endif

[LibraryClasses.IA32.PEIM, LibraryClasses.X64.PEIM]
  FspWrapperHobProcessLib|FspWrapperFeaturePkg/Library/PeiFspWrapperHobProcessLib/PeiFspWrapperHobProcessLib.inf

[LibraryClasses.IA32.SEC, LibraryClasses.X64.SEC]
  PlatformSecLib|FspWrapperFeaturePkg/Library/SecFspWrapperPlatformSecLib/SecFspWrapperPlatformSecLib.inf

[LibraryClasses.X64.DXE_DRIVER]
  FspWrapperPlatformLib|FspWrapperFeaturePkg/Library/DxeFspWrapperPlatformLib/DxeFspWrapperPlatformLib.inf

[LibraryClasses.common]
  FspInfoLib|FspWrapperFeaturePkg/Library/FspInfoLib/FspInfoLib.inf

[Components.$(FSP_ARCH)]
!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  FspWrapperFeaturePkg/DebugEventHandler/DebugEventHandler.inf
!endif #PcdFspModeSelection
  FspWrapperFeaturePkg/FspWrapperVerifyFsps/FspWrapperVerifyFsps.inf
  FspWrapperFeaturePkg/FspWrapperPeim/FspWrapperPeim.inf

[Components.X64]
!if gIntelFsp2WrapperTokenSpaceGuid.PcdFspModeSelection == 1
  FspWrapperFeaturePkg/CheckFspErrorInfoHob/CheckFspErrorInfoHobDxe.inf {
    <LibraryClasses>
      !if $(TARGET) == DEBUG
        DebugLib|$(CHECK_FSP_ERROR_INFO_HOB_DXE_DEBUG_LIB)
      !endif
  }
!endif #PcdFspModeSelection
!if gSiPkgTokenSpaceGuid.PcdFspVEnable == TRUE
  FspWrapperFeaturePkg/FspValidation/FspVPhatUpdate/FspVPhatUpdate.inf
!endif
