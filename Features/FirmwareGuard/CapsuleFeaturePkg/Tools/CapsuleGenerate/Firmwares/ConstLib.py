## @file
#  Definition of hardware configuration and capsule payload information.
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

from CapsuleGenerate._Global import *

###############################################################################
#                             Common Build Command                            #
###############################################################################
KEYWORD_CMD_IFWI_IMG: str = '-ifwi'
KEYWORD_CMD_FIT_TOOL: str = '-fit'

KEYWORD_CMD_BIOS_IMG   : str = '-bios'
KEYWORD_CMD_ME_IMG     : str = '-me'
KEYWORD_CMD_EC_IMG     : str = '-ec'
KEYWORD_CMD_GBE_IMG    : str = '-gbe'
KEYWORD_CMD_ISH_PDT_IMG: str = '-pdt'

###############################################################################
#                            Hardware Configuration                           #
###############################################################################
HW_SPI_INFO              : dict = gPlatformCfgData[DEVICE_SECTION][SPI_SECTION]
SPI_TOP_SWAP_SIZE_DATA   : dict = HW_SPI_INFO[SPI_TOP_SWAP_SIZE_SECTION]
SPI_FLASH_BLOCK_SIZE_DATA: dict = HW_SPI_INFO[SPI_FLASH_BLOCK_SIZE_SECTION]

###############################################################################
#             Capsule Payload component file name, GUID, and path             #
###############################################################################
#
# BIOS
#
BIOS_CFG_FILE_NAME : str = gFwComponentInfo[BIOS_CONFIG_KEY][FILE_NAME_KEY]
BIOS_CFG_FILE_GUID : str = gFwComponentInfo[BIOS_CONFIG_KEY][FILE_GUID_KEY]
BIOS_CFG_FILE_PATH : str = JoinPath (TEMPORARY_DIR_PATH, BIOS_CFG_FILE_NAME)
BIOS_IMG_FILE_NAME : str = gFwComponentInfo[BIOS_COMPONENT_KEY][FILE_NAME_KEY]
BIOS_IMG_FILE_GUID : str = gFwComponentInfo[BIOS_COMPONENT_KEY][FILE_GUID_KEY]
BIOS_IMG_FILE_PATH : str = JoinPath (TEMPORARY_DIR_PATH, BIOS_IMG_FILE_NAME)
BIOS_BGUP_FILE_NAME: str = gFwComponentInfo[BIOS_BGUP_KEY][FILE_NAME_KEY]
BIOS_BGUP_FILE_GUID: str = gFwComponentInfo[BIOS_BGUP_KEY][FILE_GUID_KEY]
BIOS_BGUP_FILE_PATH: str = JoinPath (TEMPORARY_DIR_PATH, BIOS_BGUP_FILE_NAME)

#
# Extended BIOS
#
EXT_BIOS_CFG_FILE_NAME: str = gFwComponentInfo[EXT_BIOS_CONFIG_KEY][FILE_NAME_KEY]
EXT_BIOS_CFG_FILE_GUID: str = gFwComponentInfo[EXT_BIOS_CONFIG_KEY][FILE_GUID_KEY]
EXT_BIOS_CFG_FILE_PATH: str = JoinPath (TEMPORARY_DIR_PATH, EXT_BIOS_CFG_FILE_NAME)

#
# EC
#
EC_IMG_FILE_NAME: str = gFwComponentInfo[EC_COMPONENT_KEY][FILE_NAME_KEY]
EC_IMG_FILE_GUID: str = gFwComponentInfo[EC_COMPONENT_KEY][FILE_GUID_KEY]
EC_IMG_FILE_PATH: str = JoinPath (TEMPORARY_DIR_PATH, EC_IMG_FILE_NAME)

#
# ME
#
ME_IMG_FILE_NAME: str = gFwComponentInfo[ME_COMPONENT_KEY][FILE_NAME_KEY]
ME_IMG_FILE_GUID: str = gFwComponentInfo[ME_COMPONENT_KEY][FILE_GUID_KEY]
ME_IMG_FILE_PATH: str = JoinPath (TEMPORARY_DIR_PATH, ME_IMG_FILE_NAME)

#
# GbE
#
GBE_IMG_FILE_NAME: str = gFwComponentInfo[GBE_COMPONENT_KEY][FILE_NAME_KEY]
GBE_IMG_FILE_GUID: str = gFwComponentInfo[GBE_COMPONENT_KEY][FILE_GUID_KEY]
GBE_IMG_FILE_PATH: str = JoinPath (TEMPORARY_DIR_PATH, GBE_IMG_FILE_NAME)

#
# IshPdt
#
ISH_PDT_IMG_FILE_NAME: str = gFwComponentInfo[ISH_PDT_COMPONENT_KEY][FILE_NAME_KEY]
ISH_PDT_IMG_FILE_GUID: str = gFwComponentInfo[ISH_PDT_COMPONENT_KEY][FILE_GUID_KEY]
ISH_PDT_IMG_FILE_PATH: str = JoinPath (TEMPORARY_DIR_PATH, ISH_PDT_IMG_FILE_NAME)

#
# IFWI
#
IFWI_CFG_FILE_NAME : str = gFwComponentInfo[IFWI_CONFIG_KEY][FILE_NAME_KEY]
IFWI_CFG_FILE_GUID : str = gFwComponentInfo[IFWI_CONFIG_KEY][FILE_GUID_KEY]
IFWI_CFG_FILE_PATH : str = JoinPath (TEMPORARY_DIR_PATH, IFWI_CFG_FILE_NAME)
IFWI_IMG_FILE_NAME : str = gFwComponentInfo[IFWI_COMPONENT_KEY][FILE_NAME_KEY]
IFWI_IMG_FILE_GUID : str = gFwComponentInfo[IFWI_COMPONENT_KEY][FILE_GUID_KEY]
IFWI_IMG_FILE_PATH : str = JoinPath (TEMPORARY_DIR_PATH, IFWI_IMG_FILE_NAME)
IFWI_BGUP_FILE_NAME: str = gFwComponentInfo[IFWI_BGUP_KEY][FILE_NAME_KEY]
IFWI_BGUP_FILE_GUID: str = gFwComponentInfo[IFWI_BGUP_KEY][FILE_GUID_KEY]
IFWI_BGUP_FILE_PATH: str = JoinPath (TEMPORARY_DIR_PATH, IFWI_BGUP_FILE_NAME)

#
# BtGAcm
#
BTG_ACM_IMG_FILE_NAME : str = gFwComponentInfo[BTG_ACM_COMPONENT_KEY][FILE_NAME_KEY]
BTG_ACM_IMG_FILE_GUID : str = gFwComponentInfo[BTG_ACM_COMPONENT_KEY][FILE_GUID_KEY]
BTG_ACM_IMG_FILE_PATH : str = JoinPath (TEMPORARY_DIR_PATH, BTG_ACM_IMG_FILE_NAME)
BTG_ACM_CFG_FILE_NAME : str = gFwComponentInfo[BTG_ACM_CONFIG_KEY][FILE_NAME_KEY]
BTG_ACM_CFG_FILE_GUID : str = gFwComponentInfo[BTG_ACM_CONFIG_KEY][FILE_GUID_KEY]
BTG_ACM_CFG_FILE_PATH : str = JoinPath (TEMPORARY_DIR_PATH, BTG_ACM_CFG_FILE_NAME)
BTG_ACM_BGUP_FILE_NAME: str = gFwComponentInfo[BTG_ACM_BGUP_KEY][FILE_NAME_KEY]
BTG_ACM_BGUP_FILE_GUID: str = gFwComponentInfo[BTG_ACM_BGUP_KEY][FILE_GUID_KEY]
BTG_ACM_BGUP_FILE_PATH: str = JoinPath (TEMPORARY_DIR_PATH, BTG_ACM_BGUP_FILE_NAME)

#
# FSP
#
FSP_IMG_FILE_NAME : str = gFwComponentInfo[FSP_COMPONENT_KEY][FILE_NAME_KEY]
FSP_IMG_FILE_GUID : str = gFwComponentInfo[FSP_COMPONENT_KEY][FILE_GUID_KEY]
FSP_IMG_FILE_PATH : str = JoinPath (TEMPORARY_DIR_PATH, FSP_IMG_FILE_NAME)
FSP_CFG_FILE_NAME : str = gFwComponentInfo[FSP_CONFIG_KEY][FILE_NAME_KEY]
FSP_CFG_FILE_GUID : str = gFwComponentInfo[FSP_CONFIG_KEY][FILE_GUID_KEY]
FSP_CFG_FILE_PATH : str = JoinPath (TEMPORARY_DIR_PATH, FSP_CFG_FILE_NAME)
FSP_BGUP_FILE_NAME: str = gFwComponentInfo[FSP_BGUP_KEY][FILE_NAME_KEY]
FSP_BGUP_FILE_GUID: str = gFwComponentInfo[FSP_BGUP_KEY][FILE_GUID_KEY]
FSP_BGUP_FILE_PATH: str = JoinPath (TEMPORARY_DIR_PATH, FSP_BGUP_FILE_NAME)
FBM_IMG_FILE_NAME : str = gFwComponentInfo[FBM_COMPONENT_KEY][FILE_NAME_KEY]
FBM_IMG_FILE_PATH : str = JoinPath (TEMPORARY_DIR_PATH, FBM_IMG_FILE_NAME)

#
# uCode
#
UCODE_VER_FILE_NAME : str = gFwComponentInfo[UCODE_VERSION_KEY][FILE_NAME_KEY]
UCODE_VER_FILE_GUID : str = gFwComponentInfo[UCODE_VERSION_KEY][FILE_GUID_KEY]
UCODE_VER_FILE_PATH : str = JoinPath (TEMPORARY_DIR_PATH, UCODE_VER_FILE_NAME)
UCODE_IMG_FILE_NAME : str = gFwComponentInfo[UCODE_COMPONENT_KEY][FILE_NAME_KEY]
UCODE_IMG_FILE_GUID : str = gFwComponentInfo[UCODE_COMPONENT_KEY][FILE_GUID_KEY]
UCODE_IMG_FILE_PATH : str = JoinPath (TEMPORARY_DIR_PATH, UCODE_IMG_FILE_NAME)
UCODE_BGUP_FILE_NAME: str = gFwComponentInfo[UCODE_BGUP_KEY][FILE_NAME_KEY]
UCODE_BGUP_FILE_GUID: str = gFwComponentInfo[UCODE_BGUP_KEY][FILE_GUID_KEY]
UCODE_BGUP_FILE_PATH: str = JoinPath (TEMPORARY_DIR_PATH, UCODE_BGUP_FILE_NAME)
UCODE_XDR_FILE_NAME : str = gFwComponentInfo[UCODE_XDR_KEY][FILE_NAME_KEY]
UCODE_XDR_FILE_GUID : str = gFwComponentInfo[UCODE_XDR_KEY][FILE_GUID_KEY]
UCODE_XDR_FILE_PATH : str = JoinPath (TEMPORARY_DIR_PATH, UCODE_XDR_FILE_NAME)

FW_FILE_NAME_KEY: str = 'FileName'
FW_FILE_PATH_KEY: str = 'FilePath'
FW_FILE_GUID_KEY: str = 'FileGuid'

FW_COMPONENT_DICT: dict = {
    FW_BIOS   : {
        FW_FILE_NAME_KEY: BIOS_IMG_FILE_NAME,
        FW_FILE_PATH_KEY: BIOS_IMG_FILE_PATH,
        FW_FILE_GUID_KEY: BIOS_IMG_FILE_GUID,
        },
    FW_EC     : {
        FW_FILE_NAME_KEY: EC_IMG_FILE_NAME,
        FW_FILE_PATH_KEY: EC_IMG_FILE_PATH,
        FW_FILE_GUID_KEY: EC_IMG_FILE_GUID,
        },
    FW_CSME   : {
        FW_FILE_NAME_KEY: ME_IMG_FILE_NAME,
        FW_FILE_PATH_KEY: ME_IMG_FILE_PATH,
        FW_FILE_GUID_KEY: ME_IMG_FILE_GUID,
        },
    FW_GBE    : {
        FW_FILE_NAME_KEY: GBE_IMG_FILE_NAME,
        FW_FILE_PATH_KEY: GBE_IMG_FILE_PATH,
        FW_FILE_GUID_KEY: GBE_IMG_FILE_GUID,
        },
    FW_ISH_PDT: {
        FW_FILE_NAME_KEY: ISH_PDT_IMG_FILE_NAME,
        FW_FILE_PATH_KEY: ISH_PDT_IMG_FILE_PATH,
        FW_FILE_GUID_KEY: ISH_PDT_IMG_FILE_GUID,
        },
    FW_IFWI: {
        FW_FILE_NAME_KEY: IFWI_IMG_FILE_NAME,
        FW_FILE_PATH_KEY: IFWI_IMG_FILE_PATH,
        FW_FILE_GUID_KEY: IFWI_IMG_FILE_GUID,
        },
    FW_BTGACM : {
        FW_FILE_NAME_KEY: BTG_ACM_IMG_FILE_NAME,
        FW_FILE_PATH_KEY: BTG_ACM_IMG_FILE_PATH,
        FW_FILE_GUID_KEY: BTG_ACM_IMG_FILE_GUID,
        },
    FW_FSP    : {
        FW_FILE_NAME_KEY: FSP_IMG_FILE_NAME,
        FW_FILE_PATH_KEY: FSP_IMG_FILE_PATH,
        FW_FILE_GUID_KEY: FSP_IMG_FILE_GUID,
        },
    FW_UCODE  : {
        FW_FILE_NAME_KEY: UCODE_IMG_FILE_NAME,
        FW_FILE_PATH_KEY: UCODE_IMG_FILE_PATH,
        FW_FILE_GUID_KEY: UCODE_IMG_FILE_GUID,
        },
    }
