## @file
#  Library provided basic function to do the padding operation.
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

from CapsuleCommon.Base.ConvertLib import ToInt

def Padding (
    String        : str,
    Size          : Union[str, int],
    IsPaddingRight: bool = False,
    PaddingChar   : str  = ' ',
    ) -> str:
    """ Do the padding on specific string.

    Note:
        Default is to do left padding with space.

    Args:
        String (str):
            The string to be padding.
            (Do type casting to str type to check the type)
        Size (Union[str, int]):
            The size of padding should be an integer number.
            This value should not be negative.
        IsPaddingRight (bool, optional):
            Option to do padding direction.
            True - right padding, False - left padding.
            Defaults to False.
        PaddingChar (str, optional):
            The unicode character to do the padding.
            This should be one character. Defaults to ' '.

    Raises:
        TypeError:
            IsPaddingRight should be bool type.
        ValueError:
            The length of padding character is not 1.

    Returns:
        str:
            The string after the do the padding action.
    """
    Result     : str = str (String)
    PaddingSize: int = ToInt (Size)
    Char       : str = str (PaddingChar)

    if not isinstance (IsPaddingRight, bool):
        raise TypeError (f'IsPaddingRight should be bool type.')

    if PaddingSize < 0:
        raise ValueError (f'The input size is negative value {Size}')

    if len (Char) != 1:
        raise ValueError (f'Character used to do padding should one.')

    if IsPaddingRight:
        Result = Result.ljust (PaddingSize, Char)
    else:
        Result = Result.rjust (PaddingSize, Char)

    return Result

def ZeroPadding (
    String        : str,
    Size          : Union[str, int],
    IsPaddingRight: bool = False,
    ) -> str:
    """ Do the zero padding on specific string.

    Note:
        (1) This is the wrapper function on Padding.
        (2) Default is to do left padding with space.

    Args:
        String (str):
            The string to be padding.
            (Do type casting to str type to check the type)
        Size (Union[str, int]):
            The size of padding should be an integer number.
        IsPaddingRight (bool, optional):
            Option to do padding direction
            True - right padding, False - left padding.
            Defaults to False.

    Raises:
        None.

    Returns:
        str:
            The string after the do the zero padding action.
    """
    Result: str = Padding (String, Size, IsPaddingRight, '0')

    return Result
