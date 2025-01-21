## @file
#  Definition of Global variable use within this package.
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

from CapsuleCommon import *

from CapsuleGenerate.Const import *
from CapsuleGenerate._CapsuleConfigLib import *

########################################################################
#                              Pre-defined                             #
########################################################################

#
# Toolkit related path cannot decode from configuration file.
# The path of this dependent on toolkit actually located on user-side.
#
PACKAGE_PATH       : str = os.path.dirname (os.path.abspath (__file__))
WORKSPACE_PATH     : str = os.path.dirname (PACKAGE_PATH)
TEMPORARY_DIR_PATH : str = JoinPath (WORKSPACE_PATH, TEMPORARY_DIR_NAME)
OUTPUT_DIR_PATH    : str = JoinPath (WORKSPACE_PATH, OUTPUT_DIR_NAME)
OUTPUT_WIN_DIR_PATH: str = JoinPath (OUTPUT_DIR_PATH, WIN_DRIVER_PKG_DIR_NAME)

########################################################################
#                             Configuration                            #
########################################################################
CAPSULE_CFG    : CapsuleConfig            = CapsuleConfig ()
COMMON_CONFIG  : MutableMapping[str, Any] = CAPSULE_CFG.Common
PLATFORM_CONFIG: MutableMapping[str, Any] = CAPSULE_CFG.Platform

gCommonCfgData  : dict = COMMON_CONFIG
gPlatformCfgData: dict = CAPSULE_CFG.CommonPlatformData

gCommonFmpFwInfo : dict = gCommonCfgData[FIRMWARE_SECTION]
gPathInfo        : dict = gPlatformCfgData[PATH_SECTION]
gPYScriptPathInfo: dict = gPathInfo[PY_SCRIPT_SECTION]
gFmpFirmwareInfo : dict = gPlatformCfgData[FIRMWARE_SECTION]
gFwComponentInfo : dict = gPlatformCfgData[FIRMWARE_COMPONENT_SECTION]
gCapSignKeyInfo  : dict = gPlatformCfgData[KEYSTORES_SECTION]

TEMPLATE_DIR_PATH: str = JoinPath (WORKSPACE_PATH, gPathInfo[TEMPLATE_DIR_KEY])

########################################################################
#                     Platform Common Configuration                    #
########################################################################
PLATFORM_COMMON_CFG     : dict = gPlatformCfgData[COMMON_SECTION]
PLATFORM_FULL_NAME      : str  = PLATFORM_COMMON_CFG[PLATFORM_FULLNAME_KEY]
PLATFORM_FULL_NAME_UPPER: str  = PLATFORM_FULL_NAME.upper ()
PLATFORM_PREFIX_NAME    : str  = PLATFORM_COMMON_CFG[PLATFORM_PREFIX_KEY]
PLATFORM_GENERATION     : int  = int (PLATFORM_COMMON_CFG[PLATFORM_GENERATION_KEY])
CAPSULE_HASH_ALGORITHM  : str  = PLATFORM_COMMON_CFG[HASH_ALGORITHM_KEY]
VENDOR_NAME             : str  = PLATFORM_COMMON_CFG[VENDOR_KEY]

########################################################################
#                EDK-II Build Environment Configuration                #
########################################################################
EDKII_TOOL_PATH      : str = JoinPath (
    WORKSPACE_PATH,
    gPathInfo[EDKII_BASETOOL_KEY]
    )
EDKII_WORKSPACE_PATH : str       = TEMPORARY_DIR_PATH
EDKII_CAP_PKG_NAME   : str       = 'CapsulePkg'
EDKII_CAP_PKG_GUID   : str       = 'F6E7730E-0C7A-4741-9DFC-6BC8B86CD865'
EDKII_BLD_TARGET_NAME: str       = BUILD_TARGET_RELEASE_TYPE
EDKII_BLD_CHAIN_NAME : str       = 'CAPSULETOOL'
EDKII_BLD_ARCH_LIST  : List[str] = [ARCH_IA32, ARCH_X64]

EDKII_BLD_DIR_NAME       : str = 'Build'
EDKII_BLD_TARGET_DIR_NAME: str = '_'.join ([
    EDKII_BLD_TARGET_NAME,
    EDKII_BLD_CHAIN_NAME]
    )
EDKII_BLD_FV_DIR_NAME    : str = 'FV'
EDKII_BLD_FV_DIR_PATH    : str = JoinPath (
    EDKII_WORKSPACE_PATH,
    EDKII_BLD_DIR_NAME,
    EDKII_CAP_PKG_NAME,
    EDKII_BLD_TARGET_DIR_NAME,
    EDKII_BLD_FV_DIR_NAME,
    )

EDKII_DSC_FILE_NAME: str = EDKII_CAP_PKG_NAME + EXTENSION_DSC_FILE
EDKII_FDF_FILE_NAME: str = EDKII_CAP_PKG_NAME + EXTENSION_FDF_FILE
EDKII_DSC_FILE_PATH: str = JoinPath (EDKII_WORKSPACE_PATH, EDKII_DSC_FILE_NAME)
EDKII_FDF_FILE_PATH: str = JoinPath (EDKII_WORKSPACE_PATH, EDKII_FDF_FILE_NAME)

gEdkIIEnvCfg: dict = Edk2BuildEnv (
    Workspace     = EDKII_WORKSPACE_PATH,
    Edk2ToolsPath = EDKII_TOOL_PATH,
    ).EnvVar

########################################################################
#       BiosGuard Update Package Build Environment Configuration       #
########################################################################
gBiosGuardCfgData     : dict = gPlatformCfgData[BIOSGUARD_SECTION]
BIOSGUARD_PLATFORM_ID : str  = gBiosGuardCfgData[BIOSGUARD_PLATFORM_ID_KEY]
BIOSGUARD_BGSL_DIR    : str  = JoinPath (
    TEMPORARY_DIR_PATH,
    BGSL_DIR_NAME
    )
BIOSGUARD_BGUP_DIR    : str  = JoinPath (
    TEMPORARY_DIR_PATH,
    BGUP_DIR_NAME
    )
