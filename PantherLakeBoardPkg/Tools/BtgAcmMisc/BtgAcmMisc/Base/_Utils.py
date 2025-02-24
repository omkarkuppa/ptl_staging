## @file
#  Utility defines and functions for the Base sub-package.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2025 Intel Corporation.
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
#  @par Specification Reference:
#
##

import copy
import os
import pathlib
from typing import List, Union

KILOBYTE_TO_BYTE: int = 0x400
MEGABYTE_TO_BYTE: int = KILOBYTE_TO_BYTE * KILOBYTE_TO_BYTE

def Padding (
    String       : str,
    Size         : int,
    PaddingRight : bool = False,
    PaddingString: str  = ' ',
    ) -> str:
    """ Padding the given string.

    Args:
        String (str):
            String to be padded.
        Size (int):
            Target padding size.
        PaddingRight (bool, optional):
            True  - Padding right side.
            False - Padding left side.
            Defaults to False.
        PaddingString (str, optional):
            String used to do the padding.
            Defaults to ' '.

    Raises:
        TypeError:
            (1) String is not str type.
            (2) Size is not int type.
            (3) PaddingRight is not bool type.
            (4) PaddingString is not str type.

    Returns:
        str:
            String after doing the padding.
    """
    Result: str = None

    if not isinstance (String, str):
        raise TypeError ('String shall be str type.')
    elif not isinstance (Size, int):
        raise TypeError ('Size shall be int type.')
    elif not isinstance (PaddingRight, bool):
        raise TypeError ('PaddingRight shall be bool type.')
    elif not isinstance (PaddingString, str):
        raise TypeError ('PaddingString shall be str type.')

    Result = copy.deepcopy (String)

    if PaddingRight:
        Result = Result.ljust (Size, PaddingString)
    else:
        Result = Result.rjust (Size, PaddingString)

    return Result

def ZeroPadding (
    String      : str,
    Size        : int,
    PaddingRight: bool = False,
    ) -> str:
    """ Zero padding the given string.

    Args:
        String (str):
            String to be zero padded.
        Size (int):
            Target padding size.
        PaddingRight (bool, optional):
            Direction to do the padding.
            True  - Padding right side.
            False - Padding left side.
            Defaults to False.

    Raises:
        None.

    Returns:
        str:
            String after doing the zero padding.
    """
    Result = Padding (String, Size, PaddingRight, '0')

    return Result

def RemoveHexPrefix (HexStr: str) -> str:
    """ Remove the hexadecimal string 0x prefix.

    Note:
        It is caller responsibility to provide the correct HEX string.

    Args:
        HexStr (str):
            HEX string to be removed the prefix.

    Raises:
        TypeError:
            HexStr is not a str type.

    Returns:
        str:
            Hexadecimal string without prefix.
    """
    HexString: str = None

    if not isinstance (HexStr, str):
        raise TypeError ('HexStr shall be str type.')

    HexString = copy.deepcopy (HexStr)
    HexString = HexString.replace ('0x', '')
    HexString = HexString.replace ('0X', '')

    return HexString

def ByteStrReverse (Value: str) -> str:
    """ Reverse the hexadecimal value string with byte size.

    Note:
        Return value without hexadecimal prefix.

    Args:
        Value (str):
            Input hexadecimal byte string to be reversed.

    Raises:
        TypeError:
            Value is not str type.

    Returns:
        str:
            Reversed hexadecimal byte string.
    """
    InputValue   : str       = None
    InputStrLen  : int       = None
    InputByteList: List[str] = list ()
    ReverseByte  : str       = ''

    if not isinstance (Value, str):
        raise TypeError ('Value shall be str type.')

    InputValue  = RemoveHexPrefix (Value)
    InputStrLen = len (InputValue)

    #
    # Do the padding, in case of the input value is not multiple of 2.
    #
    if InputStrLen % 2 != 0:
        InputValue   = ZeroPadding (InputValue, InputStrLen + 1)
        InputStrLen += 1

    #
    # Split the input string with byte size.
    #
    for Idx in range (int (InputStrLen / 2)):
        StartIdx = (Idx + 0) * 2
        EndIdx   = (Idx + 1) * 2
        InputByteList.append (InputValue[StartIdx:EndIdx])

    #
    # Do the list reverse and join as a string.
    #
    InputByteList.reverse ()
    ReverseByte = ReverseByte.join (InputByteList)

    return ReverseByte

def TouchDir (DirPath: Union[str, os.PathLike]) -> None:
    """ Create the directory with specific path if not existence.

    Args:
        DirPath (Union[str, os.PathLike]):
            The specific path to create the directory if not existence.

    Raises:
        TypeError:
            DirPath is not str or os.PathLike type.
        ValueError:
            DirPath exists but not directory.

    Returns:
        None.
    """
    if not isinstance (DirPath, (str, os.PathLike)):
        raise TypeError (f'DirPath shall be str or os.PathLike.')

    if not os.path.exists (DirPath):
        pathlib.Path (DirPath).mkdir (
                                 mode     = 0o777,
                                 parents  = True,
                                 exist_ok = True,
                                 )

    if not os.path.isdir (DirPath):
        raise ValueError (f'Path [{DirPath}] exist but not a directory.')
