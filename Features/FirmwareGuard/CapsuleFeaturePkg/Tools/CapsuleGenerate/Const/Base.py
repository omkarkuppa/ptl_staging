## @file
#  Basic definition use within this package.
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

#
# Reserved keyword
#
IGNORE        : str = 'IGNORE'
DEFAULT       : str = 'DEFAULT'
NOT_APPLICABLE: str = 'NA'

#
# Toolkit generated directory name
#
TEMPORARY_DIR_NAME     : str = 'Temporary'
OUTPUT_DIR_NAME        : str = 'Output'
WIN_DRIVER_PKG_DIR_NAME: str = 'Windows'

BGSL_DIR_NAME          : str = 'Bgsl'
BGUP_DIR_NAME          : str = 'Bgup'

FIT_DECOMPOSE_DIR_NAME       : str = 'Decompose'
FIT_FWUPDATE_DIR_NAME        : str = 'Fwu'
FIT_COMPONENT_OUTPUT_DIR_NAME: str = 'Component'

#
# IFWI Decomposed filename
#
IFWI_XML_CFG_FILENAME    : str = 'Ifwi.xml'
BIOS_DECOMPOSE_FILENAME  : str = 'ClientBios.rom'
EC_DECOMPOSE_FILENAME    : str = 'Ec.bin'
ME_DECOMPOSE_FILENAME    : str = 'CSERegion.bin'
GBE_DECOMPOSE_FILENAME   : str = 'GbE.bin'
ISHPDT_DECOMPOSE_FILENAME: str = 'Pdt.bin'
FWUPDATE_IMAGE_FILENAME  : str = 'FWUpdate.bin'
