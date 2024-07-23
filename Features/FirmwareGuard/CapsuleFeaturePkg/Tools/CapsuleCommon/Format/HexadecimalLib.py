## @file
#  Library provided basic function for hexadecimal operation.
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

HEX_STRING_REGEX: str = '^(0x|0X)?[a-fA-F\d]+$'

def IsHexStr (String: str) -> Union[None, re.Match]:
    """ To trial the input value is hexadecimal format.

    Args:
        String (str):
            The string to be tested if it is hexadecimal format.

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

    Regex: Pattern = re.compile (HEX_STRING_REGEX)

    return Regex.match (String)

def AddHexPrefix (Hex: str, IsLowercase: bool = True) -> str:
    """ Add the hexadecimal value string '0x' and '0X' prefix.

    Args:
        Hex (str):
            The hexadecimal string to add the prefix '0x' and '0X'.
        IsLowercase (bool, optional):
            True  - The hexadecimal value uses lowercase.
            False - The hexadecimal value uses uppercase.
            Defaults to True.

    Raises:
        TypeError:
            IsLowercase is not bool type.
        ValueError:
            The input value to be tested is not hexadecimal format.

    Returns:
        str:
            Hexadecimal number in string type. (With 0x/0X prefix)
    """
    Result: str = str (Hex)

    if not isinstance (IsLowercase, bool):
        raise TypeError (f'IsLowercase should be bool type.')
    elif not IsHexStr (Result):
        raise ValueError (f'Please input the valid hexadecimal string.')

    Result: str = RemoveHexPrefix (Hex)

    if IsLowercase:
        Result = '0x' + Result
    else:
        Result = '0X' + Result

    return Result

def RemoveHexPrefix (Hex: str) -> str:
    """ Remove the hexadecimal value string '0x' and '0X' prefix.

    Args:
        Hex (str):
            The hexadecimal string to remove the prefix '0x' and '0X'.

    Raises:
        ValueError:
            The input value to be tested is not hexadecimal format.

    Returns:
        str:
            Hexadecimal number in string type. (Without 0x/0X prefix)
    """
    Result: str = str (Hex)

    if not IsHexStr (Result):
        raise ValueError ('Please input the valid hexadecimal string.')

    Result = Result.replace ('0x', '')
    Result = Result.replace ('0X', '')

    return Result

def DecToHex (Dec: Union[str, int], IsLowercase: bool = False) -> str:
    """ Convert the value from decimal to hexadecimal value.

    Note:
        (1) The return value without prefix '0x' or '0X.
        (2) Input DEC cannot be negative one.

    Args:
        Dec (Union[str, int]):
            The value of decimal number to be converted.
        IsLowercase (bool, optional):
            True  - The hexadecimal value uses lowercase.
            False - The hexadecimal value uses uppercase.
            Defaults to False.

    Raises:
        ValueError:
            Input DEC value is negative.
        TypeError:
            (1) Dec is neither int nor str type.
            (2) IsLowercase is not bool type.

    Returns:
        str:
            Hexadecimal number in string type. (Without 0x/0X prefix)
    """
    if not isinstance (Dec, (int, str)):
        raise TypeError (f'Input type should be int or str.')
    if not isinstance (IsLowercase, bool):
        raise TypeError (f'IsLowercase should be bool type.')

    Format: str = 'x' if IsLowercase else 'X'
    Value : str = int (Dec)
    Result: str = str ()

    if Value < 0:
        raise ValueError ('Input value can not be negative.')

    Result = format (int (Dec), Format)

    return Result

def HexToDec (Hex: str) -> int:
    """ Convert the value from hexadecimal to decimal value.

    Args:
        Hex (str):
            The value of hexadecimal number to be converted.

    Raises:
        ValueError:
            The input value to be tested is not hexadecimal format.

    Returns:
        int:
            Decimal number after getting converted.
    """

    if not IsHexStr (Hex):
        raise ValueError ('Please input the valid hexadecimal string.')

    if Hex.startswith ('0x') or Hex.startswith ('0X'):
        return int (Hex, 0)
    else:
        return int (Hex, 16)

def FormatHex (
    Input            : Union[str, int],
    IsLowercase      : bool = False,
    IsPrefix         : bool = True,
    IsPrefixLowercase: bool = True,
    IsPadding        : bool = False,
    IsPaddingRight   : bool = False,
    PaddingSize      : int  = 8
    ) -> str:
    """ Format the hexadecimal value into specific string.

    Note:
        Configurable:
            - Prefix (Lower/Upper Case)
            - Value (Lower/Upper Case)
            - Padding (Size/Direction)

    Args:
        Input (Union[str, int]):
            The hexadecimal value to be formatted to specific string.
            (Supported str and int type)
        IsLowercase (bool, optional):
            True  - The hexadecimal value uses lowercase.
            False - The hexadecimal value uses uppercase.
            Defaults to False.
        IsPrefix (bool, optional):
            True  - Need the '0x' or '0X' prefix.
            False - Without the '0x' or '0X' prefix.
            If set to True, PrefixLowercase would be the valid input.
            Defaults to True.
        IsPrefixLowercase (bool, optional):
            True  - The hexadecimal prefix uses lowercase.
            False - The hexadecimal prefix uses uppercase.
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
            Defaults to 8.

    Raises:
        TypeError:
            (1) IsLowercase is not bool type.
            (2) IsPrefix is not bool type.
            (3) IsPrefixLowercase is not bool type.
            (4) IsPadding is not bool type.
            (5) IsPaddingRight is not bool type.
        ValueError:
            The input value to be tested is not hexadecimal format.

    Returns:
        str:
            The hexadecimal string after do the formatted.
    """
    if not isinstance (IsLowercase, bool):
        raise TypeError (f'IsLowercase should be bool type.')
    elif not isinstance (IsPrefix, bool):
        raise TypeError (f'IsPrefix should be bool type.')
    elif not isinstance (IsPrefixLowercase, bool):
        raise TypeError (f'IsPrefixLowercase should be bool type.')
    elif not isinstance (IsPadding, bool):
        raise TypeError (f'IsPadding should be bool type.')
    elif not isinstance (IsPaddingRight, bool):
        raise TypeError (f'IsPaddingRight should be bool type.')

    Result: str = None

    if isinstance (Input, int):
        Result = DecToHex (Input, IsLowercase)
    else:
        Result = Input

    if not IsHexStr (Result):
        raise ValueError ('Please input the valid hexadecimal string.')

    Result = RemoveHexPrefix (Result)
    Result = Result.lower () if IsLowercase else Result.upper ()

    if IsPadding:
        Result = ZeroPadding (Result, PaddingSize, IsPaddingRight)

    if IsPrefix:
        Result = AddHexPrefix (Result, IsPrefixLowercase)

    return Result
