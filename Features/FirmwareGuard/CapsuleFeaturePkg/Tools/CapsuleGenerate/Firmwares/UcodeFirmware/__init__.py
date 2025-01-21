## @file
#  Python module package initial files for uCode Firmware sub-package.
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

from CapsuleGenerate.Firmwares.UcodeFirmware.UcodeFmpDevice import *

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'UCODE_DEVICE_NAME',
    'UCODE_DEVICE_NAME_UPPER',
    'UcodeFmpDevice',
    ]
