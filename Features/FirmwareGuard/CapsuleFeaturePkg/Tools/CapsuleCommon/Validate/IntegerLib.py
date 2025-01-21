## @file
#  Library provided basic function to validate integer.
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

from CapsuleCommon.Base.CheckLib import *
from CapsuleCommon.Constant.DataTypeLib import *

def IsInt8 (Input: int) -> bool:
    """ To check the input is INT-8.

    Args:
        Input (int):
            The integer value to be tested.

    Raises:
        None.

    Returns:
        bool:
            True  - The input is INT-8.
            False - The input is not INT-8.
    """
    return CheckIntInRange (Input, INT8_MIN_VALUE, INT8_MAX_VALUE)

def IsInt16 (Input: int) -> bool:
    """ To check the input is INT-16.

    Args:
        Input (int):
            The integer value to be tested.

    Raises:
        None.

    Returns:
        bool:
            True  - The input is INT-16.
            False - The input is not INT-16.
    """
    return CheckIntInRange (Input, INT16_MIN_VALUE, INT16_MAX_VALUE)

def IsInt32 (Input: int) -> bool:
    """ To check the input is INT-32.

    Args:
        Input (int):
            The integer value to be tested.

    Raises:
        None.

    Returns:
        bool:
            True  - The input is INT-32.
            False - The input is not INT-32.
    """
    return CheckIntInRange (Input, INT32_MIN_VALUE, INT32_MAX_VALUE)

def IsInt64 (Input: int) -> bool:
    """ To check the input is INT-64.

    Args:
        Input (int):
            The integer value to be tested.

    Raises:
        None.

    Returns:
        bool:
            True  - The input is INT-64.
            False - The input is not INT-64.
    """
    return CheckIntInRange (Input, INT64_MIN_VALUE, INT64_MAX_VALUE)

def IsUint8 (Input: int) -> bool:
    """ To check the input is UINT-8.

    Args:
        Input (int):
            The integer value to be tested.

    Raises:
        None.

    Returns:
        bool:
            True  - The input is UINT-8.
            False - The input is not UINT-8.
    """
    return CheckIntInRange (Input, UINT8_MIN_VALUE, UINT8_MAX_VALUE)

def IsUint16 (Input: int) -> bool:
    """ To check the input is UINT-16.

    Args:
        Input (int):
            The integer value to be tested.

    Raises:
        None.

    Returns:
        bool:
            True  - The input is UINT-16.
            False - The input is not UINT-16.
    """
    return CheckIntInRange (Input, UINT16_MIN_VALUE, UINT16_MAX_VALUE)

def IsUint32 (Input: int) -> bool:
    """ To check the input is UINT-32.

    Args:
        Input (int):
            The integer value to be tested.

    Raises:
        None.

    Returns:
        bool:
            True  - The input is UINT-32.
            False - The input is not UINT-32.
    """
    return CheckIntInRange (Input, UINT32_MIN_VALUE, UINT32_MAX_VALUE)

def IsUint64 (Input: int) -> bool:
    """ To check the input is UINT-64.

    Args:
        Input (int):
            The integer value to be tested.

    Raises:
        None.

    Returns:
        bool:
            True  - The input is UINT-64.
            False - The input is not UINT-64.
    """
    return CheckIntInRange (Input, UINT64_MIN_VALUE, UINT64_MAX_VALUE)
