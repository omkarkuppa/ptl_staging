## @file
#  Definition of Intel specific acronym or terminology.
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

import time

#
# Type of IFWI
#
IFWI_CONSUMER : str = 'Consumer'
IFWI_CORPORATE: str = 'Corporate'

#
# Type of Sign
#
SIGN_PRE_PRODUCTION: str = 'PreProdSign'
SIGN_PRODUCTION    : str = 'ProdSign'

#
# Type of Firmware
#
FW_IFWI      : str = 'Ifwi'
FW_MONOLITHIC: str = 'Monolithic'
FW_DESCRIPTOR: str = 'Descriptor'
FW_BIOS      : str = 'Bios'
FW_EC        : str = 'Ec'
FW_CSME      : str = 'Me'
FW_FWUPDATE  : str = 'FWUpdate'
FW_CSE_REGION: str = 'CseRegion'
FW_GBE       : str = 'GbE'
FW_ISH_PDT   : str = 'IshPdt'
FW_BTGACM    : str = 'BtGAcm'
FW_FSP       : str = 'Fsp'
FW_UCODE     : str = 'Ucode'
FW_FOXVILLE  : str = 'Foxville'

INTEL_COPYRIGHT_STR: str = f'''\
#  INTEL CONFIDENTIAL
#  Copyright (C) {time.strftime ("%Y")} Intel Corporation.
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
#  in the License.\
'''
