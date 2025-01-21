## @file
#  Library provided basic function to do the conversion.
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

from typing import List, Union

def ToInt (Input: Union[str, int]) -> int:
    """ To convert the integer number into int type.

    Note:
        (1) If input type is str
            (a) If input value is hexadecimal number
                -> Convert to int 0-based.
            (b) Else
                -> Convert to int directly.
        (2) If input type is int
            No need to do the conversion, return directly.
        (3) Others type
            Not supported.

    Args:
        Input (Union[str, int]):
            The input value to be converted into int type.
            (Supported type: str, int)

    Raises:
        ValueError:
            Input type is string, but not integer format.
        TypeError:
            Input is not str or int.

    Returns:
        int:
            Return the int type input value.
    """
    if isinstance (Input, str):
        try:
            if Input.startswith ('0x') or Input.startswith ('0X'):
                return int (Input, 0)
            else:
                return int (Input)
        except:
            print (f'!!! Input value [{Input}] not integer format !!!')
            raise ValueError(
                    f'!!! Only allowed to input the integer format value !!!'
                    )
    elif isinstance (Input, int):
        return Input
    else:
        print (f'Input type is {type (Input)} value is {Input}.')
        raise TypeError (f'!!! Input only allow string or integer !!!')

def ToBool (Input: Union[str, int, bool]) -> bool:
    """ Convert the input into bool type.

    Note:
        Echo element within the string list should be lowercase.

    Args:
        Input (Union[str, int, bool]):
            Caller input value to be converted into bool type.

    Raises:
        ValueError:
            Input value is not recognized.
        TypeError:
            Input type is not supported.

    Returns:
        bool:
            If it is bool, return directly.
            If it is int, type casting to bool.
            If it is str,
                - If it is one of BOOL_TRUE_STR_LIST return True.
                - If it is one of BOOL_FALSE_STR_LIST return False.
                - Else raise the exception.
    """
    BOOL_TRUE_STR_LIST : List[str] = [
        'true',
        't',
        'yes',
        'y',
        ]
    BOOL_FALSE_STR_LIST: List[str] = [
        'false',
        'f',
        'no',
        'n',
        ]

    if isinstance (Input, bool):
        return Input
    elif isinstance (Input, int):
        return bool (Input)
    elif isinstance (Input, str):
        if Input.lower () in BOOL_TRUE_STR_LIST:
            return True
        elif Input.lower () in BOOL_FALSE_STR_LIST:
            return False
        else:
            raise ValueError (f'Unsupported input string [{Input}]')
    else:
        raise TypeError (f'Not supported type of input.')
