## @file
#  Api to expose the Firmware Management Protocol Device Model/Instance.
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

#
# FMP Device Model.
#
from CapsuleGenerate.Model.FmpDevice import FmpDevice

#
# FMP Device Command.
#
from CapsuleGenerate.Firmwares.ConstLib import \
    KEYWORD_CMD_IFWI_IMG, KEYWORD_CMD_FIT_TOOL, KEYWORD_CMD_BIOS_IMG, \
    KEYWORD_CMD_ME_IMG, KEYWORD_CMD_EC_IMG, KEYWORD_CMD_GBE_IMG, \
    KEYWORD_CMD_ISH_PDT_IMG

#
# Fmp Device Instance.
#
from CapsuleGenerate.Firmwares.BiosFirmware.BiosFmpDevice \
    import BIOS_DEVICE_NAME, Bios
from CapsuleGenerate.Firmwares.BiosFirmware.ExtBiosFmpDevice \
    import EXT_BIOS_DEVICE_NAME, ExtendedBios
from CapsuleGenerate.Firmwares.EcFirmware.EcFmpDevice \
    import EC_DEVICE_NAME, Ec
from CapsuleGenerate.Firmwares.IshPdtFirmware.IshPdtFmpDevice \
    import ISH_PDT_DEVICE_NAME, IshPdt
from CapsuleGenerate.Firmwares.MeFirmware.MeFmpDevice \
    import ME_DEVICE_NAME, Me

from CapsuleGenerate.Firmwares.MonolithicFirmware.MonolithicFmpDevice \
    import MONOLITHIC_DEVICE_NAME, Monolithic
from CapsuleGenerate.Firmwares.IfwiFirmware.IfwiFmpDevice \
    import IFWI_DEVICE_NAME, Ifwi

from CapsuleGenerate.Firmwares.BtGAcmFirmware.BtGAcmFmpDevice \
    import BTG_ACM_DEVICE_NAME, BtGAcm
from CapsuleGenerate.Firmwares.FspFirmware.FspFmpDevice \
    import FSP_DEVICE_NAME, Fsp
from CapsuleGenerate.Firmwares.UcodeFirmware.UcodeFmpDevice \
    import UCODE_DEVICE_NAME, Ucode

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    #
    # Model
    #
    'FmpDevice',
    #
    # Common Command
    #
    'KEYWORD_CMD_IFWI_IMG',
    'KEYWORD_CMD_FIT_TOOL',
    'KEYWORD_CMD_BIOS_IMG',
    'KEYWORD_CMD_ME_IMG',
    'KEYWORD_CMD_EC_IMG',
    'KEYWORD_CMD_GBE_IMG',
    'KEYWORD_CMD_ISH_PDT_IMG',
    #
    # BIOS FW
    #
    'BIOS_DEVICE_NAME',
    'Bios',
    #
    # Extended BIOS FW
    #
    'EXT_BIOS_DEVICE_NAME',
    'ExtendedBios',
    #
    # EC FW
    #
    'EC_DEVICE_NAME',
    'Ec',
    #
    # ISH PDT FW
    #
    'ISH_PDT_DEVICE_NAME',
    'IshPdt',
    #
    # ME FW
    #
    'ME_DEVICE_NAME',
    'Me',
    #
    # Monolithic FW
    #
    'MONOLITHIC_DEVICE_NAME',
    'Monolithic',
    #
    # IFWI FW
    #
    'IFWI_DEVICE_NAME',
    'Ifwi',
    #
    # BtGAcm modular FW
    #
    'BTG_ACM_DEVICE_NAME',
    'BtGAcm',
    #
    # FSP FW
    #
    'FSP_DEVICE_NAME',
    'Fsp',
    #
    # uCode modular FW
    #
    'UCODE_DEVICE_NAME',
    'Ucode',
    ]
