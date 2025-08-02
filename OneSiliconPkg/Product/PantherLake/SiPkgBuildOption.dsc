## @file
#  Silicon build option configuration file.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2016 Intel Corporation.
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

DEFINE PCH_BUILD_OPTIONS =
#
# SA
#
!if gSiPkgTokenSpaceGuid.PcdBdatEnable == TRUE
  DEFINE BDAT_BUILD_OPTION = -DBDAT_SUPPORT=1
!else
  DEFINE BDAT_BUILD_OPTION =
!endif

# Build option for LNL SOC Core Boot
!if gSiPkgTokenSpaceGuid.PcdSocCoreBootEnable == TRUE
  DEFINE SOC_CORE_BUILD_OPTIONS = -DSOC_CORE_BOOT
!else
  DEFINE SOC_CORE_BUILD_OPTIONS =
!endif

  DEFINE SLE_BUILD_OPTIONS =
!if $(TARGET) == RELEASE
  # MDEPKG_NDEBUG is introduced for the intention
  # of size reduction when compiler optimization is disabled. If MDEPKG_NDEBUG is
  # defined, then debug and assert related macros wrapped by it are the NULL implementations.
  DEFINE DEBUG_BUILD_OPTIONS = -DMDEPKG_NDEBUG
!else
  DEFINE DEBUG_BUILD_OPTIONS =
!endif

!if gSiPkgTokenSpaceGuid.PcdOptimizeCompilerEnable == FALSE
  DEFINE OPTIMIZE_DISABLE_OPTIONS = -Od -GL-
!else
  DEFINE OPTIMIZE_DISABLE_OPTIONS =
!endif


  DEFINE CPU_FLAGS = -DPRODUCT_PTL

DEFINE MAX_SOCKET_OPTION = -DMAX_SOCKET=1


  DEFINE RESTRICTED_OPTION =

DEFINE DSC_SIPKG_FEATURE_BUILD_OPTIONS = $(BDAT_BUILD_OPTION) $(DEBUG_BUILD_OPTIONS) $(PTLS_BUILD_OPTION) $(MAX_SOCKET_OPTION)
DEFINE DSC_SIPKG_FEATURE_BUILD_OPTIONS = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS) $(PCH_BUILD_OPTIONS) $(CPU_FLAGS) $(SOC_CORE_BUILD_OPTIONS) $(BFX_BUILD_OPTIONS)

  *_*_*_ASL_PATH      = $(WORKSPACE_BINARIES)/Tools/Iasl/20210930/iasl

!if gSiPkgTokenSpaceGuid.PcdSourceDebugEnable == TRUE
  *_*_X64_GENFW_FLAGS = --keepexceptiontable
!endif

[BuildOptions.Common.EDKII]

#
# For IA32 Global Build Flag
#
       *_*_IA32_CC_FLAGS      = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS)
       *_*_IA32_VFRPP_FLAGS   = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS)
       *_*_IA32_APP_FLAGS     = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS)
       *_*_IA32_ASLPP_FLAGS   = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS)
       *_*_IA32_ASLCC_FLAGS   = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS)

#
# For IA32 Specific Build Flag
#
CLANGPDB:*_*_IA32_CC_FLAGS    = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS) -Wno-enum-conversion -Wno-incompatible-pointer-types-discards-qualifiers -Wno-sometimes-uninitialized -Wno-braced-scalar-init -Wno-nonportable-include-path -Wno-misleading-indentation
GCC:   *_*_IA32_PP_FLAGS      = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS)
MSFT:  *_*_IA32_ASM_FLAGS     = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS)
MSFT:  *_*_IA32_CC_FLAGS      = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS) /w34668
MSFT:  *_*_IA32_VFRPP_FLAGS   = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS)
MSFT:  *_*_IA32_APP_FLAGS     = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS)
MSFT:  *_*_IA32_ASLPP_FLAGS   = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS)
MSFT:  *_*_IA32_ASLCC_FLAGS   = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS)

#
# For X64 Global Build Flag
#
       *_*_X64_CC_FLAGS       = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS)
       *_*_X64_VFRPP_FLAGS    = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS)
       *_*_X64_APP_FLAGS      = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS)
       *_*_X64_ASLPP_FLAGS    = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS)
       *_*_X64_ASLCC_FLAGS    = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS)

#
# For X64 Specific Build Flag
#
CLANGPDB:*_*_X64_CC_FLAGS     = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS) -Wno-enum-conversion -Wno-main-return-type -Wno-incompatible-pointer-types -Wno-implicit-function-declaration -Wno-unused-variable -Wno-incompatible-pointer-types-discards-qualifiers -Wno-ignored-pragma-optimize -Wno-bitwise-op-parentheses  -Wno-sometimes-uninitialized -Wno-unused-function -Wno-misleading-indentation
GCC:   *_*_X64_PP_FLAGS       = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS)
MSFT:  *_*_X64_ASM_FLAGS      = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS)
MSFT:  *_*_X64_CC_FLAGS       = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS) /w34668
MSFT:  *_*_X64_VFRPP_FLAGS    = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS)
MSFT:  *_*_X64_APP_FLAGS      = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS) $(OPTIMIZE_DISABLE_OPTIONS)
MSFT:  *_*_X64_ASLPP_FLAGS    = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS)
MSFT:  *_*_X64_ASLCC_FLAGS    = $(DSC_SIPKG_FEATURE_BUILD_OPTIONS)

#
# For Xcode Specific Build Flag
#
# Override assembly code build order
*_XCODE5_*_*_BUILDRULEORDER = nasm S s
# Align 47bfbd7f8069e523798ef973c8eb0abd5c6b0746 to fix the usage of VA_START in undefined way
*_XCODE5_*_CC_FLAGS = -Wno-varargs

# Force PE/COFF sections to be aligned at 4KB boundaries to support page level protection of runtime modules
[BuildOptions.common.EDKII.DXE_RUNTIME_DRIVER]
  MSFT:  *_*_*_DLINK_FLAGS      = /ALIGN:4096
  GCC:   *_GCC*_*_DLINK_FLAGS   = -z common-page-size=0x1000


