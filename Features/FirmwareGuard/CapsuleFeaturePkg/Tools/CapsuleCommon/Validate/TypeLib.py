## @file
#  Library provided basic function to validate type.
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

from typing import Union

from CapsuleCommon.Base.CheckLib import *
from CapsuleCommon.Base.ConvertLib import ToBool

def IsNoneType (Input: Union[None, str]) -> bool:
    """ Validate the input is None type.

    Note:
        Check to meet one of the following condition.
            - None type -> True
            - str type  -> Is None string -> True
            - Else      -> False

    Args:
        Input (Union[None, str]):
            Caller input data to be checked if it is None.

    Raises:
        None.

    Returns:
        bool:
            True  - The input is None type.
            False - The input is not None type.
    """
    if Input is None:
        return Input is None
    elif isinstance (Input, str):
        return CheckStringMatch (Input, 'None')
    else:
        return False

def IsBoolType (Input: Union[int, str, bool]) -> bool:
    """ Validate the input is boolean type.

    Note:
        Check to meet one of the following condition.
            - boolean type -> True
            - int type
                -> 0, 1 -> True
                -> Else -> False
            - str type
                -> If input is bool type -> True
                -> Else -> False

    Args:
        Input (Union[int, str, bool]):
            The input value to be tested if it is boolean.

    Raises:
        None.

    Returns:
        bool:
            True  - The input is boolean type.
            False - The input is not boolean type.
    """
    if isinstance (Input, bool):
        return True
    elif isinstance (Input, int):
        if Input == 0 or Input == 1:
            return True
        else:
            return False
    elif isinstance (Input, str):
        try:
            ToBool (Input)
            return True
        except:
            return False
    else:
        return False
