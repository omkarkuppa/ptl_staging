## @file
#  Definition of Intel BIOS specific acronym or terminology.
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

BIOS_NORMAL_TYPE    : str = 'Normal'
BIOS_RESILIENCY_TYPE: str = 'Resiliency'

#
# Layout of BIOS Firmware Image
# Should be the same as the BIOS_UPDATE_PROGRESS defined without prefix.
#
BIOS_REGION_EXT : str = 'Ext'
BIOS_REGION_NVS : str = 'Nvs'
BIOS_REGION_IBB : str = 'Ibb'
BIOS_REGION_IBBR: str = 'Ibbr'
BIOS_REGION_OBB : str = 'Obb'
BIOS_REGION_OBB1: str = 'Obb1'
