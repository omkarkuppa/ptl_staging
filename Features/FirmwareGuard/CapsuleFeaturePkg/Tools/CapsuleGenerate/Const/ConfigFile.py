## @file
#  Definition of Section/Key name mapping to configuration file.
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

TITLE_KEY: str = 'Title'

#
# Common section
#
COMMON_SECTION         : str = 'Common'
VENDOR_KEY             : str = 'Vendor'
PLATFORM_GENERATION_KEY: str = 'PlatformGeneration'
PLATFORM_FULLNAME_KEY  : str = 'PlatformFullName'
PLATFORM_PREFIX_KEY    : str = 'PlatformPrefix'
HASH_ALGORITHM_KEY     : str = 'HashAlgorithm'

#
# Keystores section
#
KEYSTORES_SECTION           : str = 'Keystore'
CAP_TRUST_PUBLIC_CERT_KEY   : str = 'CapTrustPublicCert'
CAP_OTHER_PUBLIC_CERT_KEY   : str = 'CapOtherPublicCert'
CAP_SIGNER_PRIVATE_CERT_KEY : str = 'CapSignerPrivateCert'
WIN_DRIVER_PFX_CERT_KEY     : str = 'WinDriverPfxCert'
ME_PRE_PROD_CERT_SUBJECT_KEY: str = 'MePreProdCertSubject'
ME_PROD_CERT_SUBJECT_KEY    : str = 'MeProdCertSubject'

#
# Path section
#
PATH_SECTION          : str = 'Path'
TOOL_DIR_KEY          : str = 'ToolDir'
TEMPLATE_DIR_KEY      : str = 'TemplateDir'
EDKII_BASETOOL_KEY    : str = 'EdkIIBaseToolDir'
OPENSSL_DIR_KEY       : str = 'OpenSSLDir'
EDSS_SIGN_TOOL_DIR_KEY: str = 'EdssSignFileDir'
#
# PyScript sub-section
#
PY_SCRIPT_SECTION          : str = 'PyScript'
CLEAN_UP_SCRIPT_KEY        : str = 'CleanUpScript'
XDR_BUILDER_SCRIPT_KEY     : str = 'XdrBuilderScript'
EDKII_FD_BUILDER_SCRIPT_KEY: str = 'EdkiiFdBuilderScript'
WIN_INF_GEN_SCRIPT_KEY     : str = 'WinInfGeneratorScript'
WIN_CAT_GEN_SCRIPT_KEY     : str = 'WinCatGeneratorScript'
WIN_CAT_SIGN_SCRIPT_KEY    : str = 'WinCatSignerScript'

#
# FitTool section
#
FIT_TOOL_SECTION                 : str = 'FitTool'
FIT_TOOL_TYPE_KEY                : str = 'Method'
FIT_TOOL_LAYOUT_PLATFORM         : str = 'LayoutPlatform'
IFWI_DECOMPOSER_SCRIPT_KEY       : str = 'IfwiDecomposerScript'
IFWI_XML_CONFIG_PARSER_SCRIPT_KEY: str = 'IfwiXmlConfigParserScript'
IFWI_FWUPDATE_BUILDER_SCRIPT_KEY : str = 'IfwiFWUpdateBuilderScript'

#
# BiosGuard section
#
BIOSGUARD_SECTION        : str = 'BiosGuard'
BIOSGUARD_PLATFORM_ID_KEY: str = 'PlatformId'
BGUP_GENERATOR_SCRIPT_KEY: str = 'BgupGeneratorScript'
#
# BiosGuard Program sub-section
#
BIOSGUARD_PROGRAM_SECTION: str = 'Program'
BGPB_PROGRAM_KEY         : str = 'Bgpb'
BGSL2BIN_PROGRAM_KEY     : str = 'Bgsl2Bin'
#
# BiosGuard Certificate sub-section
#
BIOSGUARD_CERTIFICATE_SECTION: str = 'Certificate'
BIOSGUARD_SIGN_TYPE_KEY      : str = 'SignType'
BIOSGUARD_SIGN_ALGORITHM_KEY : str = 'Algorithm'
BIOSGUARD_SIGN_TOOL_KEY      : str = 'SignTool'
BIOSGUARD_SIGN_PBK_KEY       : str = 'PbkFile'
BIOSGUARD_SIGN_PVK_KEY       : str = 'PvkFile'

#
# Device section
#
DEVICE_SECTION: str = 'Device'
#
# SPI sub-section
#
SPI_SECTION: str = 'Spi'
#
# SPI TopSwapSize sub-section
#
SPI_TOP_SWAP_SIZE_SECTION: str = 'TopSwapSize'
#
# SPI FlashBlockSize sub-section
#
SPI_FLASH_BLOCK_SIZE_SECTION: str = 'FlashBlockSize'

#
# Firmware Component section
#
FIRMWARE_COMPONENT_SECTION: str = 'FirmwareComponent'
FILE_NAME_KEY             : str = 'FileName'
FILE_GUID_KEY             : str = 'FileGuid'
BIOS_COMPONENT_KEY        : str = 'Bios'
BIOS_BGUP_KEY             : str = 'BiosBgup'
BIOS_CONFIG_KEY           : str = 'BiosConfig'
EXT_BIOS_CONFIG_KEY       : str = 'ExtBiosConfig'
EC_COMPONENT_KEY          : str = 'Ec'
GBE_COMPONENT_KEY         : str = 'GbE'
ME_COMPONENT_KEY          : str = 'Me'
ISH_PDT_COMPONENT_KEY     : str = 'IshPdt'
IFWI_COMPONENT_KEY        : str = 'Ifwi'
IFWI_BGUP_KEY             : str = 'IfwiBgup'
IFWI_CONFIG_KEY           : str = 'IfwiConfig'
BTG_ACM_COMPONENT_KEY     : str = 'BtGAcm'
BTG_ACM_BGUP_KEY          : str = 'BtGAcmBgup'
BTG_ACM_CONFIG_KEY        : str = 'BtGAcmConfig'
FSP_COMPONENT_KEY         : str = 'Fsp'
FSP_BGUP_KEY              : str = 'FspBgup'
FSP_CONFIG_KEY            : str = 'FspConfig'
FBM_COMPONENT_KEY         : str = 'Fbm'
UCODE_COMPONENT_KEY       : str = 'Ucode'
UCODE_VERSION_KEY         : str = 'UcodeVersion'
UCODE_BGUP_KEY            : str = 'UcodeBgup'
UCODE_XDR_KEY             : str = 'UcodeXdr'
FOXVILLE_COMPONENT_KEY    : str = 'Foxville'

#
# Firmware Section
#
FIRMWARE_SECTION    : str = 'Firmware'
FW_VERSION_KEY      : str = 'Version'
IS_ENABLED_KEY      : str = 'IsEnabled'
FW_BUILD_OP_CODE_KEY: str = 'BuildCmd'
CAP_FILE_PREFIX_KEY : str = 'CapFilePrefix'
CAP_FILE_POSTFIX_KEY: str = 'CapFilePostfix'
#
# Command sub-section
#
FW_CMD_SECTION_KEY: str = 'Command'
#
# Command helper
#
CMD_HELPER_TITLE_KEY   : str       = 'Title'
CMD_HELPER_DESC_KEY    : str       = 'Description'
CMD_HELPER_SUB_DESC_KEY: str       = 'SubDescription'
CMD_HELPER_KEY_LIST    : List[str] = [
    CMD_HELPER_TITLE_KEY,
    CMD_HELPER_DESC_KEY,
    CMD_HELPER_SUB_DESC_KEY,
    ]
#
# Command configuration
#
CMD_COMPULSORY_KEY : str = 'Compulsory'
CMD_DESCRIPTION_KEY: str = 'Description'
CMD_SYNTAX_KEY     : str = 'Syntax'
CMD_DEFAULT_VAL_KEY: str = 'Default'

#
# Guid sub-section
#
FMP_GUID_SECTION_KEY: str = 'Guid'

#
# FlashMap Info sub-section (Optional for each FMP Device)
#
FLASH_MAP_OFFSET_SECTION: str = 'Offset'
FM_OFFSET_KEY           : str = 'Offset'
FM_SIZE_KEY             : str = 'Size'
