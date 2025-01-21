## @file
#  Definition of data type value.
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
# Definition of data type string.
#
INT8_TYPE : str = 'Int8'
INT16_TYPE: str = 'Int16'
INT32_TYPE: str = 'Int32'
INT64_TYPE: str = 'Int64'

UINT8_TYPE : str = 'Uint8'
UINT16_TYPE: str = 'Uint16'
UINT32_TYPE: str = 'Uint32'
UINT64_TYPE: str = 'Uint64'

#
# Byte size of data type.
#
INT8_BYTE_VALUE : int = 1
INT16_BYTE_VALUE: int = 2
INT32_BYTE_VALUE: int = 4
INT64_BYTE_VALUE: int = 8

UINT8_BYTE_VALUE : int = 1
UINT16_BYTE_VALUE: int = 2
UINT32_BYTE_VALUE: int = 4
UINT64_BYTE_VALUE: int = 8

#
# Minimum and maximum value of data type.
#
INT8_MIN_VALUE : int = -0x7F - 0x01
INT16_MIN_VALUE: int = -0x7FFF - 0x0001
INT32_MIN_VALUE: int = -0x7FFFFFFF - 0x00000001
INT64_MIN_VALUE: int = -0x7FFFFFFFFFFFFFFF - 0x0000000000000001

INT8_MAX_VALUE : int = +0x7F
INT16_MAX_VALUE: int = +0x7FFF
INT32_MAX_VALUE: int = +0x7FFFFFFF
INT64_MAX_VALUE: int = +0x7FFFFFFFFFFFFFFF

UINT8_MIN_VALUE : int = 0x00
UINT16_MIN_VALUE: int = 0x0000
UINT32_MIN_VALUE: int = 0x00000000
UINT64_MIN_VALUE: int = 0x0000000000000000

UINT8_MAX_VALUE : int = 0xFF
UINT16_MAX_VALUE: int = 0xFFFF
UINT32_MAX_VALUE: int = 0xFFFFFFFF
UINT64_MAX_VALUE: int = 0xFFFFFFFFFFFFFFFF

#
# Unsigned-integer mask value.
#
UINT8_MASK_VALUE : int = 0xFF
UINT16_MASK_VALUE: int = 0xFFFF
UINT32_MASK_VALUE: int = 0xFFFFFFFF
UINT64_MASK_VALUE: int = 0xFFFFFFFFFFFFFFFF
