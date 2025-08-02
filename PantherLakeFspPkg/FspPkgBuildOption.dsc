## @file
# Fsp build option configuration file.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2014 Intel Corporation.
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

[BuildOptions]
# Define Build Options both for EDK and EDKII drivers.

!if gPantherLakeFspPkgTokenSpaceGuid.PcdSymbolInReleaseEnable == TRUE
  DEFINE DSC_SYMBOL_IN_RELEASE_BUILD_OPTIONS = /Zi
  DEFINE DSC_SYMBOL_IN_RELEASE_LINK_BUILD_OPTIONS = /DEBUG
!else
  DEFINE DSC_SYMBOL_IN_RELEASE_BUILD_OPTIONS =
  DEFINE DSC_SYMBOL_IN_RELEASE_LINK_BUILD_OPTIONS =
!endif

##################
# Rsa2048Sha256Sign tool definitions
#
# Notes: This tool defintion uses a test signing key for development purposes only.
#        The tool Rsa2048Sha256GenerateKeys can be used to generate a new private/public key
#        and the gEfiSecurityPkgTokenSpaceGuid.PcdRsa2048Sha256PublicKeyBuffer PCD value.
#        A custom tool/script can be implemented using the new private/public key with
#        the Rsa2048Sha256Sign tool and this tool defintiion can be updated to use a
#        custom tool/script.
#
#   Generate new private/public key and gEfiSecurityPkgTokenSpaceGuid.PcdRsa2048Sha256PublicKeyBuffer PCD value
#
#       Rsa2048Sha256GenerateKeys.py -o MyKey.pem --public-key-hash-c MyKey.pcd
#
#   Custom script example (MyRsa2048Sha256Sign.cmd):
#
#       Rsa2048Sha256Sign --private-key MyKey.pem %1 %2 %3 %4 %5 %6 %7 %8 %9
#
#   WARNING: Vendors that uses private keys are responsible for proper management and protection
#            of private keys.  Vendors may choose to use infrastructure such as signing servers
#            or signing portals to support the management and protection of private keys.
#
##################
*_*_*_RSA2048SHA256SIGN_PATH = Rsa2048Sha256SignPlatform
*_*_*_RSA2048SHA256SIGN_GUID = A7717414-C616-4977-9420-844712A735BF

[BuildOptions.Common.EDKII]
!if gPantherLakeFspPkgTokenSpaceGuid.PcdSymbolInReleaseEnable == TRUE
MSFT:  RELEASE_*_*_CC_FLAGS    = $(DSC_SYMBOL_IN_RELEASE_BUILD_OPTIONS)
MSFT:  RELEASE_*_*_ASM_FLAGS   = $(DSC_SYMBOL_IN_RELEASE_BUILD_OPTIONS)
MSFT:  RELEASE_*_*_DLINK_FLAGS = $(DSC_SYMBOL_IN_RELEASE_LINK_BUILD_OPTIONS)
!endif










