## @file
#  Setup feature DXE description file.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2018 Intel Corporation.
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

  $(PLATFORM_BOARD_PACKAGE)/BoardInitAdvanced/PolicyInitAdvancedDxe/PolicyInitAdvancedDxe.inf
  $(PLATFORM_FULL_PACKAGE)/Platform/PlatformSetup/Dxe/PlatformSetup.inf

  $(PLATFORM_FULL_PACKAGE)/Setup/Setup.inf {
    <LibraryClasses>
      BaseCryptLib|CryptoPkg/Library/BaseCryptLib/BaseCryptLib.inf
!if gSiPkgTokenSpaceGuid.PcdDeprecatedCryptoRemove == FALSE
      NULL|SecurityPkg/Library/HashInstanceLibSha1/HashInstanceLibSha1.inf
!endif
      NULL|SecurityPkg/Library/HashInstanceLibSha256/HashInstanceLibSha256.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha384/HashInstanceLibSha384.inf
      NULL|SecurityPkg/Library/HashInstanceLibSha512/HashInstanceLibSha512.inf
      NULL|SecurityPkg/Library/HashInstanceLibSm3/HashInstanceLibSm3.inf
!if gBoardModuleTokenSpaceGuid.PcdAcpiDebugFeatureEnable == TRUE
    <BuildOptions>
      *_*_X64_CC_FLAGS      = -DACPI_DEBUG_FLAG
      *_*_X64_VFRPP_FLAGS   = -DACPI_DEBUG_FLAG
      *_*_X64_APP_FLAGS     = -DACPI_DEBUG_FLAG
      *_*_X64_PP_FLAGS      = -DACPI_DEBUG_FLAG
!endif
    <BuildOptions>
      *_*_X64_CC_FLAGS      = -DRTCWakeup_SUPPORT_FLAG
      *_*_X64_VFRPP_FLAGS   = -DRTCWakeup_SUPPORT_FLAG
      *_*_X64_APP_FLAGS     = -DRTCWakeup_SUPPORT_FLAG
      *_*_X64_PP_FLAGS      = -DRTCWakeup_SUPPORT_FLAG
  }

