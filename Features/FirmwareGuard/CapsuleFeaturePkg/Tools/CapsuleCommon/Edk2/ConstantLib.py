## @file
#  Definition of EDK-II Constant.
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
#  @par Specification
#
##

from typing import List

from CapsuleCommon.Base.PathLib import JoinPath

EDKII_BASE_TOOL_PYTHON_SOURCE_PATH    : str = 'Source/Python'
EDKII_BASE_TOOL_WIN_BIN_WRAPPER_PATH  : str = 'BinWrappers/WindowsLike'
EDKII_BASE_TOOL_WIN_BIN_WRAPPER_EXT   : str = '.bat'
EDKII_BASE_TOOL_WIN_BIN_PATH          : str = 'Bin/Win32'
EDKII_BASE_TOOL_POSIX_BIN_WRAPPER_PATH: str = 'BinWrappers/PosixLike'
EDKII_BASE_TOOL_POSIX_BIN_PATH        : str = 'Source/C/bin'

#
# EDK-II 2K PKCS#7 Sample Certificate
#
EDKII_BASE_TOOL_CERT_PATH               : str = 'Source/Python/Pkcs7Sign'
EDKII_BASE_TOOL_SIGNER_PRIVATE_CERT_PATH: str = \
    JoinPath (
      EDKII_BASE_TOOL_CERT_PATH,
      'TestCert.pem',
      )
EDKII_BASE_TOOL_OTHER_PUBLIC_CERT_PATH  : str = \
    JoinPath (
      EDKII_BASE_TOOL_CERT_PATH,
      'TestSub.pub.pem',
      )
EDKII_BASE_TOOL_TRUSTED_PUBLIC_CERT_PATH: str = \
    JoinPath (
      EDKII_BASE_TOOL_CERT_PATH,
      'TestRoot.pub.pem',
      )

#
# Build Target
#
BUILD_TARGET_NOOPT_TYPE  : str       = 'NOOPT'
BUILD_TARGET_DEBUG_TYPE  : str       = 'DEBUG'
BUILD_TARGET_RELEASE_TYPE: str       = 'RELEASE'
BUILD_TARGET_TYPE_LIST   : List[str] = [
    BUILD_TARGET_NOOPT_TYPE,
    BUILD_TARGET_DEBUG_TYPE,
    BUILD_TARGET_RELEASE_TYPE,
    ]

#
# Tool Chain Tag
#
TOOL_CHAIN_TAG_CLANGDWARF: str       = 'CLANGDWARF'
TOOL_CHAIN_TAG_CLANGPDB  : str       = 'CLANGPDB'
TOOL_CHAIN_TAG_GCC       : str       = 'GCC'
TOOL_CHAIN_TAG_GCC48     : str       = 'GCC48'
TOOL_CHAIN_TAG_GCC49     : str       = 'GCC49'
TOOL_CHAIN_TAG_GCC5      : str       = 'GCC5'
TOOL_CHAIN_TAG_GCCNOLTO  : str       = 'GCCNOLTO'
TOOL_CHAIN_TAG_VS2015    : str       = 'VS2015'
TOOL_CHAIN_TAG_VS2015X86 : str       = 'VS2015x86'
TOOL_CHAIN_TAG_VS2017    : str       = 'VS2017'
TOOL_CHAIN_TAG_VS2019    : str       = 'VS2019'
TOOL_CHAIN_TAG_XCODE5    : str       = 'XCODE5'
TOOL_CHAIN_TAG_LIST      : List[str] = [
    TOOL_CHAIN_TAG_CLANGDWARF,
    TOOL_CHAIN_TAG_CLANGPDB,
    TOOL_CHAIN_TAG_GCC,
    TOOL_CHAIN_TAG_GCC48,
    TOOL_CHAIN_TAG_GCC49,
    TOOL_CHAIN_TAG_GCC5,
    TOOL_CHAIN_TAG_GCCNOLTO,
    TOOL_CHAIN_TAG_VS2015,
    TOOL_CHAIN_TAG_VS2015X86,
    TOOL_CHAIN_TAG_VS2017,
    TOOL_CHAIN_TAG_VS2019,
    TOOL_CHAIN_TAG_XCODE5,
    ]

#
# Platform Architecture
#
ARCH_IA32     : str       = 'IA32'
ARCH_X64      : str       = 'X64'
ARCH_IPF      : str       = 'IPF'
ARCH_EBC      : str       = 'EBC'
ARCH_ARM      : str       = 'ARM'
ARCH_TYPE_LIST: List[str] = [
    ARCH_IA32,
    ARCH_X64,
    ARCH_IPF,
    ARCH_EBC,
    ARCH_ARM,
    ]

#
# EDK-II specific extensions
#
EXTENSION_DSC_FILE: str = '.dsc'
EXTENSION_FDF_FILE: str = '.fdf'
EXTENSION_INF_FILE: str = '.inf'
EXTENSION_FV_FILE : str = '.Fv'
EXTENSION_FD_FILE : str = '.fd'

#
# EDK-II build environment configuration file
#
EDKII_CONF_DIR_NAME                : str = 'Conf'
EDKII_BLD_DIR_NAME                 : str = 'Build'
#
# Conf template files
#
EDKII_BUILD_RULE_TEMPLATE_FILE_NAME: str = 'build_rule.template'
EDKII_TARGET_TEMPLATE_FILE_NAME    : str = 'target.template'
EDKII_TOOLS_DEF_TEMPLATE_FILE_NAME : str = 'tools_def.template'
#
# Workspace template files
#
EDKII_BUILD_RULE_FILE_NAME         : str = 'build_rule.txt'
EDKII_TARGET_FILE_NAME             : str = 'target.txt'
EDKII_TOOLS_DEF_FILE_NAME          : str = 'tools_def.txt'
EDKII_BUILD_RULE_TEMPLATE_FILE_PATH: str = \
    JoinPath (
      EDKII_CONF_DIR_NAME,
      EDKII_BUILD_RULE_TEMPLATE_FILE_NAME,
      )
EDKII_TARGET_TEMPLATE_FILE_PATH    : str = \
    JoinPath (
      EDKII_CONF_DIR_NAME,
      EDKII_TARGET_TEMPLATE_FILE_NAME,
      )
EDKII_TOOLS_DEF_TEMPLATE_FILE_PATH : str = \
    JoinPath (
      EDKII_CONF_DIR_NAME,
      EDKII_TOOLS_DEF_TEMPLATE_FILE_NAME,
      )
EDKII_BUILD_RULE_FILE_PATH         : str = \
    JoinPath (
      EDKII_CONF_DIR_NAME,
      EDKII_BUILD_RULE_FILE_NAME,
      )
EDKII_TARGET_FILE_PATH             : str = \
    JoinPath (
      EDKII_CONF_DIR_NAME,
      EDKII_TARGET_FILE_NAME,
      )
EDKII_TOOLS_DEF_FILE_PATH          : str = \
    JoinPath (
      EDKII_CONF_DIR_NAME,
      EDKII_TOOLS_DEF_FILE_NAME,
      )

#
# Build Tool Commands
#
EDKII_BUILD_FD_CMD     : str = 'GenFds'
EDKII_BUILD_CAPSULE_CMD: str = 'GenerateCapsule'

#
# Firmware File System (FFS) alignment.
#
FFS_ALIGNMENT_AUTO: str       = 'Auto'
FFS_ALIGNMENT_8   : str       = '8'
FFS_ALIGNMENT_16  : str       = '16'
FFS_ALIGNMENT_32  : str       = '32'
FFS_ALIGNMENT_64  : str       = '64'
FFS_ALIGNMENT_128 : str       = '128'
FFS_ALIGNMENT_512 : str       = '512'
FFS_ALIGNMENT_1K  : str       = '1K'
FFS_ALIGNMENT_4K  : str       = '4K'
FFS_ALIGNMENT_32K : str       = '32K'
FFS_ALIGNMENT_64K : str       = '64K'
FFS_ALIGNMENT_128K: str       = '128K'
FFS_ALIGNMENT_256K: str       = '256K'
FFS_ALIGNMENT_512K: str       = '512K'
FFS_ALIGNMENT_1M  : str       = '1M'
FFS_ALIGNMENT_2M  : str       = '2M'
FFS_ALIGNMENT_4M  : str       = '4M'
FFS_ALIGNMENT_8M  : str       = '8M'
FFS_ALIGNMENT_16M : str       = '16M'
FFS_ALIGNMENT_LIST: List[str] = [
    FFS_ALIGNMENT_AUTO,
    FFS_ALIGNMENT_8,
    FFS_ALIGNMENT_16,
    FFS_ALIGNMENT_32,
    FFS_ALIGNMENT_64,
    FFS_ALIGNMENT_128,
    FFS_ALIGNMENT_512,
    FFS_ALIGNMENT_1K,
    FFS_ALIGNMENT_4K,
    FFS_ALIGNMENT_32K,
    FFS_ALIGNMENT_64K,
    FFS_ALIGNMENT_128K,
    FFS_ALIGNMENT_256K,
    FFS_ALIGNMENT_512K,
    FFS_ALIGNMENT_1M,
    FFS_ALIGNMENT_2M,
    FFS_ALIGNMENT_4M,
    FFS_ALIGNMENT_8M,
    FFS_ALIGNMENT_16M,
    ]
