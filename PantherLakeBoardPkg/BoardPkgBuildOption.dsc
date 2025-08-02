## @file
# Board Description file to describe various build options available to build this project.
# This DSC file used to update build options flag for EDK drivers based on features.
# For Example- If Acpi is supported then below build flag need to be updated.
# !if gSiPkgTokenSpaceGuid.PcdAcpiEnable == TRUE
#   DEFINE DSC_ACPI_BUILD_OPTIONS = -DACPI_SUPPORT=1
# DEFINE DSC_PLTPKG_FEATURE_BUILD_OPTIONS = $(DSC_ACPI_BUILD_OPTIONS)
# For IA32 Global Build Flag
#      *_*_IA32_CC_FLAGS      = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
#
# Update the MSFT and GCC DLINK_FLAGS flag
# For Example- ALIGN Page size to 4KB
# [BuildOptions.common.EDKII.DXE_RUNTIME_DRIVER]
#   MSFT:*_*_*_DLINK_FLAGS = /ALIGN:4096
#   GCC:*_GCC*_*_DLINK_FLAGS = -z common-page-size=0x1000
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2017 Intel Corporation.
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


!if gSiPkgTokenSpaceGuid.PcdOptimizeCompilerEnable == FALSE
  DEFINE OPTIMIZE_DISABLE_OPTIONS = -Od -GL-
!else
  DEFINE OPTIMIZE_DISABLE_OPTIONS =
!endif

!if gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable == TRUE
  DEFINE DEFAULT_BOOT_TIME_OUT_MACRO = -DDEFAULT_BOOT_TIME_OUT=0
!else
  DEFINE DEFAULT_BOOT_TIME_OUT_MACRO = -DDEFAULT_BOOT_TIME_OUT=5
!endif

!if gSiPkgTokenSpaceGuid.PcdDeprecatedCryptoRemove == TRUE
  *_*_*_CC_FLAGS = -D DISABLE_SHA1_DEPRECATED_INTERFACES
!endif

!if gPlatformModuleTokenSpaceGuid.PcdSymbolInReleaseEnable == TRUE
  DEFINE DSC_SYMBOL_IN_RELEASE_BUILD_OPTIONS = /Zi
  DEFINE DSC_SYMBOL_IN_RELEASE_LINK_BUILD_OPTIONS = /DEBUG
!else
  DEFINE DSC_SYMBOL_IN_RELEASE_BUILD_OPTIONS =
  DEFINE DSC_SYMBOL_IN_RELEASE_LINK_BUILD_OPTIONS =
!endif

#
# Embedded Enable Build
#
!if gSiPkgTokenSpaceGuid.PcdEmbeddedEnable == 1
  DEFINE EMBEDDED_ENABLE_BUILD_OPTION = -DEMBEDDED_ENABLE_FLAG=1
!else
  DEFINE EMBEDDED_ENABLE_BUILD_OPTION =
!endif

DEFINE DSC_PLTPKG_FEATURE_BUILD_OPTIONS = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS) $(DEFAULT_BOOT_TIME_OUT_MACRO) $(EMBEDDED_ENABLE_BUILD_OPTION)

DEFINE IASL_REPORT_WARNING_AS_ERRORS  =  -we
DEFINE DSC_SYMBOL_IASL_FLAG           = $(IASL_REPORT_WARNING_AS_ERRORS)
DEFINE DSC_SYMBOL_IASL_FLAG          = $(DSC_SYMBOL_IASL_FLAG) -vw 3157  # Disable 3157 warning for unload operator issue.

[BuildOptions.Common.EDKII]
#
# For Release with Symbols
#
!if gPlatformModuleTokenSpaceGuid.PcdSymbolInReleaseEnable == TRUE
  MSFT:  RELEASE_*_*_CC_FLAGS    = $(DSC_SYMBOL_IN_RELEASE_BUILD_OPTIONS)
  MSFT:  RELEASE_*_*_ASM_FLAGS   = $(DSC_SYMBOL_IN_RELEASE_BUILD_OPTIONS)
  MSFT:  RELEASE_*_*_DLINK_FLAGS = $(DSC_SYMBOL_IN_RELEASE_LINK_BUILD_OPTIONS)
!endif
#
# For IA32 Global Build Flag
#
       *_*_IA32_CC_FLAGS      = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
       *_*_IA32_VFRPP_FLAGS   = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
       *_*_IA32_APP_FLAGS     = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
       *_*_IA32_ASLPP_FLAGS   = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
       *_*_IA32_ASLCC_FLAGS   = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
       *_*_IA32_NASM_FLAGS    = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)

#
# For IA32 Specific Build Flag
#
GCC:   *_*_IA32_PP_FLAGS      = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
MSFT:  *_*_IA32_ASM_FLAGS     = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
MSFT:  *_*_IA32_CC_FLAGS      = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS)
MSFT:  *_*_IA32_VFRPP_FLAGS   = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS)
MSFT:  *_*_IA32_APP_FLAGS     = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS)
MSFT:  *_*_IA32_ASLPP_FLAGS   = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS)
MSFT:  *_*_IA32_ASLCC_FLAGS   = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS)

#
# For X64 Global Build Flag
#
       *_*_X64_CC_FLAGS       = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
       *_*_X64_VFRPP_FLAGS    = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
       *_*_X64_APP_FLAGS      = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
       *_*_X64_ASLPP_FLAGS    = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
       *_*_X64_ASLCC_FLAGS    = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
       *_*_X64_NASM_FLAGS     = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
       *_*_X64_ASL_FLAGS      = $(DSC_SYMBOL_IASL_FLAG)

#
# For X64 Specific Build Flag
#
GCC:   *_*_X64_PP_FLAGS       = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
MSFT:  *_*_X64_ASM_FLAGS      = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
MSFT:  *_*_X64_CC_FLAGS       = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS)
MSFT:  *_*_X64_VFRPP_FLAGS    = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS)
MSFT:  *_*_X64_APP_FLAGS      = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS)
MSFT:  *_*_X64_ASLPP_FLAGS    = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
MSFT:  *_*_X64_ASLCC_FLAGS    = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)

##################
# Rsa2048Sha256Sign tool definitions
#
# Notes: This tool definition uses a test signing key for development purposes only.
#        The tool Rsa2048Sha256GenerateKeys can be used to generate a new private/public key
#        and the gEfiSecurityPkgTokenSpaceGuid.PcdRsa2048Sha256PublicKeyBuffer PCD value.
#        A custom tool/script can be implemented using the new private/public key with
#        the Rsa2048Sha256Sign tool and this tool definition can be updated to use a
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
*_*_*_RSA2048SHA256SIGN_PATH  = Rsa2048Sha256SignPlatform
*_*_*_RSA2048SHA256SIGN_GUID  = A7717414-C616-4977-9420-844712A735BF

# Force PE/COFF sections to be aligned at 4KB boundaries to support page level protection
[BuildOptions.common.EDKII.DXE_SMM_DRIVER, BuildOptions.common.EDKII.SMM_CORE]
  MSFT:*_*_*_DLINK_FLAGS = /ALIGN:4096
  GCC:*_GCC*_*_DLINK_FLAGS = -z common-page-size=0x1000

# Force PE/COFF sections to be aligned at 4KB boundaries to support MemoryAttribute table
[BuildOptions.common.EDKII.DXE_RUNTIME_DRIVER]
  MSFT:*_*_*_DLINK_FLAGS = /ALIGN:4096
  GCC:*_GCC*_*_DLINK_FLAGS = -z common-page-size=0x1000

# Force PE/COFF sections to be aligned at 4KB boundaries to support NX protection
[BuildOptions.common.EDKII.DXE_DRIVER, BuildOptions.common.EDKII.DXE_CORE, BuildOptions.common.EDKII.UEFI_DRIVER, BuildOptions.common.EDKII.UEFI_APPLICATION]
  #MSFT:*_*_*_DLINK_FLAGS = /ALIGN:4096
  #GCC:*_GCC*_*_DLINK_FLAGS = -z common-page-size=0x1000
