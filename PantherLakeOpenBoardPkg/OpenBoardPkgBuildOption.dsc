## @file
#  platform build option configuration file.
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
# @par Specification
##
[BuildOptions]
# Define Build Options both for EDK and EDKII drivers.
  DEFINE OPTIMIZE_DISABLE_OPTIONS =
!if gMinPlatformPkgTokenSpaceGuid.PcdPerformanceEnable == TRUE
  DEFINE DEFAULT_BOOT_TIME_OUT_MACRO = -DDEFAULT_BOOT_TIME_OUT=0
!else
  DEFINE DEFAULT_BOOT_TIME_OUT_MACRO = -DDEFAULT_BOOT_TIME_OUT=5
!endif
  DEFINE DSC_SYMBOL_IN_RELEASE_BUILD_OPTIONS =
  DEFINE DSC_SYMBOL_IN_RELEASE_LINK_BUILD_OPTIONS =
DEFINE DSC_PLTPKG_FEATURE_BUILD_OPTIONS = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS) $(DEFAULT_BOOT_TIME_OUT_MACRO)
DEFINE IASL_REPORT_WARNING_AS_ERRORS  =  -we
DEFINE DSC_SYMBOL_IASL_FLAG           = $(IASL_REPORT_WARNING_AS_ERRORS)

[BuildOptions.Common.EDKII]

#
# For IA32 Global Build Flag
#
       *_*_IA32_CC_FLAGS      = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS) -D PI_SPECIFICATION_VERSION=0x00010015 -DASF_PEI
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
MSFT:  *_*_IA32_CC_FLAGS      = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS) -D PI_SPECIFICATION_VERSION=0x00010015 -DASF_PEI
MSFT:  *_*_IA32_VFRPP_FLAGS   = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS)
MSFT:  *_*_IA32_APP_FLAGS     = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS)
MSFT:  *_*_IA32_ASLPP_FLAGS   = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS)
MSFT:  *_*_IA32_ASLCC_FLAGS   = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS)

#
# For X64 Global Build Flag
#
       *_*_X64_CC_FLAGS       = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS) -D PI_SPECIFICATION_VERSION=0x00010015
       *_*_X64_VFRPP_FLAGS    = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
       *_*_X64_APP_FLAGS      = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
       *_*_X64_ASLPP_FLAGS    = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
       *_*_X64_ASLCC_FLAGS    = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
       *_*_X64_NASM_FLAGS     = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)

#
# For X64 Specific Build Flag
#
GCC:   *_*_X64_PP_FLAGS       = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
MSFT:  *_*_X64_ASM_FLAGS      = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
MSFT:  *_*_X64_CC_FLAGS       = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS) -D PI_SPECIFICATION_VERSION=0x00010015
MSFT:  *_*_X64_VFRPP_FLAGS    = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS)
MSFT:  *_*_X64_APP_FLAGS      = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS)
MSFT:  *_*_X64_ASLPP_FLAGS    = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)
MSFT:  *_*_X64_ASLCC_FLAGS    = $(DSC_PLTPKG_FEATURE_BUILD_OPTIONS)


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

