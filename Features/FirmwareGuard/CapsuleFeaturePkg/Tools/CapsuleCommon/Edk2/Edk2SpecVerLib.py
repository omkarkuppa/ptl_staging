## @file
#  Definition of EDK-II version of specification.
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
#    - https://github.com/tianocore/tianocore.github.io/wiki/EDK-II-Documentation
#
##

#
# Declaration (DEC) File
#
EDKII_DEC_SPEC_V_1_27_VER    : int = 0x0001001B
EDKII_DEC_SPEC_V_1_27_VER_STR: str = '0x0001001B'

EDKII_DEC_SPEC_LATEST_VER    : int = EDKII_DEC_SPEC_V_1_27_VER
EDKII_DEC_SPEC_LATEST_VER_STR: str = EDKII_DEC_SPEC_V_1_27_VER_STR

#
# Platform Description (DSC) File
#
EDKII_DSC_SPEC_V_1_28_VER    : int = 0x0001001C
EDKII_DSC_SPEC_V_1_28_VER_STR: str = '0x0001001C'

EDKII_DSC_SPEC_LATEST_VER    : int = EDKII_DSC_SPEC_V_1_28_VER
EDKII_DSC_SPEC_LATEST_VER_STR: str = EDKII_DSC_SPEC_V_1_28_VER_STR

#
# Flash Description (FDF) File
#
EDKII_FDF_SPEC_V_1_28_VER    : int = 0x0001001C
EDKII_FDF_SPEC_V_1_28_VER_STR: str = '0x0001001C'

EDKII_FDF_SPEC_LATEST_VER    : int = EDKII_DSC_SPEC_V_1_28_VER
EDKII_FDF_SPEC_LATEST_VER_STR: str = EDKII_DSC_SPEC_V_1_28_VER_STR
