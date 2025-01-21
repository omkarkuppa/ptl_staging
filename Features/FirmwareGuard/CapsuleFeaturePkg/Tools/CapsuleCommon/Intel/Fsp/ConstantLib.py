## @file
#  Definition of Intel FSP specific acronym or terminology.
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
#    - Intel Firmware Support Package EAS, Version 2.4 (Errata A)
#
##

MAX_FSP_COMPONENT_NUMBER: int = 5

#
# Layout of FSP firmware image.
#
FSP_T: str = 'FspT'
FSP_M: str = 'FspM'
FSP_S: str = 'FspS'
FSP_I: str = 'FspI'
FSP_O: str = 'FspO'
FBM  : str = 'FBM'

#
# Component ID.
#
FSP_T_COMPONENT_ID: int = 0b0001
FSP_M_COMPONENT_ID: int = 0b0010
FSP_S_COMPONENT_ID: int = 0b0011
FSP_I_COMPONENT_ID: int = 0b0100
FSP_O_COMPONENT_ID: int = 0b1000
