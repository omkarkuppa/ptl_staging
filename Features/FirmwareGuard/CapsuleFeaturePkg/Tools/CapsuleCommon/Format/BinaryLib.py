## @file
#  Library provided basic function for binary operation.
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

import re
from typing import Pattern, Union

from CapsuleCommon.Base.PaddingLib import *

BIN_STRING_REGEX: str = '^(0b|0B)?[0,1]+$'

def IsBinStr (String: str) -> Union[None, re.Match]:
    """ To trial the input value is binary format.

    Args:
        String (str):
            The string to be tested if it is binary format.

    Raises:
        TypeError:
            The input is not string type.

    Returns:
        Union[None, re.Match]:
            None     - The input string is not matched the pattern.
            re.Match - The input string is matched the pattern.
    """
    if not isinstance (String, str):
        raise TypeError ('!!! Should input the string type !!!')

    Regex: Pattern = re.compile (BIN_STRING_REGEX)

    return Regex.match (String)

def RemoveBinPrefix (Bin: str) -> str:
    """ Remove the binary value string '0b' and '0B' prefix.

    Args:
        Bin (str):
            The binary string to remove the prefix '0b' and '0B'.

    Raises:
        ValueError:
            The input value to be tested is not binary format.

    Returns:
        str:
            Binary number in string type. (Without 0b/0B prefix)
    """
    Result: str = str (Bin)

    if not IsBinStr (Result):
        raise ValueError ('Please input the valid binary string.')

    Result = Result.replace ('0b', '')
    Result = Result.replace ('0B', '')

    return Result

def AddBinPrefix (Bin: str, IsLowercase: bool = True) -> str:
    """ Add the binary value string '0b' and '0B' prefix.

    Args:
        Bin (str):
            The binary string to add the prefix '0b' and '0B'.
        IsLowercase (bool, optional):
            True  - The binary value uses lowercase.
            False - The binary value uses uppercase.
            Defaults to True.

    Raises:
        TypeError:
            IsLowercase is not bool type.
        ValueError:
            The input value to be tested is not binary format.

    Returns:
        str:
            Binary number in string type. (With 0b/0B prefix)
    """
    Result: str = str (Bin)

    if not isinstance (IsLowercase, bool):
        raise TypeError (f'IsLowercase should be bool type.')
    elif not IsBinStr (Result):
        raise ValueError (f'Please input the valid binary string.')

    Result: str = RemoveBinPrefix (Bin)

    if IsLowercase:
        Result = '0b' + Result
    else:
        Result = '0B' + Result

    return Result

def DecToBin (Dec: Union[str, int]) -> str:
    """ Convert the value from decimal to binary value.

    Note:
        (1) The return value without prefix '0b' or '0B'.
        (2) Input DEC cannot be negative one.

    Args:
        Dec (Union[str, int]):
            The value of decimal number to be converted.

    Raises:
        ValueError:
            Input DEC value is negative.
        TypeError:
            Dec is neither int nor str type.

    Returns:
        str:
            Binary number in string type. (Without 0b/0B prefix)
    """
    if not isinstance (Dec, (int, str)):
        raise TypeError (f'Input type should be int or str.')

    Value : str = int (Dec)
    Result: str = str ()

    if Value < 0:
        raise ValueError ('Input value can not be negative.')

    Result = format (int (Dec), 'b')

    return Result

def FormatBin (
    Input            : Union[str, int],
    IsPrefix         : bool = True,
    IsPrefixLowercase: bool = True,
    IsPadding        : bool = False,
    IsPaddingRight   : bool = False,
    PaddingSize      : int  = 64,
    ) -> str:
    """ Format the binary value into specific string.

    Note:
        Configurable:
            - Prefix (Lower/Upper Case)
            - Padding (Size/Direction)

    Args:
        Input (Union[str, int]):
            The binary value to be formatted to specific string.
            (Supported str and int type)
        IsPrefix (bool, optional):
            True  - Need the '0b' or '0B' prefix.
            False - Without the '0b' or '0B' prefix.
            If set to True, PrefixLowercase would be the valid input.
            Defaults to True.
        IsPrefixLowercase (bool, optional):
            True  - The binary prefix uses lowercase.
            False - The binary prefix uses uppercase.
            Defaults to True.
        IsPadding (bool, optional):
            Option if need the padding.
            If set to True, PaddingRight and PaddingSize would be
            the valid input.
            Defaults to False.
        IsPaddingRight (bool, optional):
            Option to do padding direction.
            True - right padding, False - left padding.
        PaddingSize (int, optional):
            Option to do padding direction.
            True - right padding, False - left padding.
            This value should not be negative.
            Defaults to 64.

    Raises:
        TypeError:
            (1) IsPrefix is not bool type.
            (2) IsPrefixLowercase is not bool type.
            (3) IsPadding is not bool type.
            (4) IsPaddingRight is not bool type.
        ValueError:
            The input value to be tested is not binary format.

    Returns:
        str:
            The binary string after do the formatted.
    """
    if not isinstance (IsPrefix, bool):
        raise TypeError (f'IsPrefix should be bool type.')
    elif not isinstance (IsPrefixLowercase, bool):
        raise TypeError (f'IsPrefixLowercase should be bool type.')
    elif not isinstance (IsPadding, bool):
        raise TypeError (f'IsPadding should be bool type.')
    elif not isinstance (IsPaddingRight, bool):
        raise TypeError (f'IsPaddingRight should be bool type.')

    Result: str = DecToBin (Input) if isinstance (Input, int) else Input

    if not IsBinStr (Result):
        raise ValueError ('Please input the valid binary string.')

    Result = RemoveBinPrefix (Result)

    if IsPadding:
        Result = ZeroPadding (Result, PaddingSize, IsPaddingRight)

    if IsPrefix:
        Result = AddBinPrefix (Result, IsPrefixLowercase)

    return Result
