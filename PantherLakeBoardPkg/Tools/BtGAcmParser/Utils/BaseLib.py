## @file
#  Base library to declare the base common function.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2022 Intel Corporation.
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
# @par Specification
##
import os
from typing import Union

from .ConstantLib import *

def DecToHex (Dec, Lowercase=False):
    """ Covert the decimal number to hexadecimal number. (Without 0x)

    Args:
        Dec       [str|int]: The decimal number to be converted.
        Lowercase [bool    : The case of return value.
                             TRUE            - Lowercase.
                             False (Default) - UpperCase.

    Returns:
        Result [str]: Hexadecimal number in string type.

    Raises:
        None.
    """
    Format = 'x' if Lowercase else 'X'
    Result = format (int (Dec), Format)
    return Result

def FormatHex (Hex, Lowercase=False, Prefix=True, Padding=False, PaddingRight=False, PaddingSize=8, PrefixLowercase=True):
    """ To customize hexadecimal number.

    Args:
        Hex             [int|str]: The hexadecimal number. If it is integer number would convert to hexadecimal first.
        Lowercase       [bool]   : The case of the hexadecimal value. True - Lowercase; False (Default) - UpperCase.
        Prefix          [bool]   : Is the return hexadecimal value need "0x" prefix. True - with prefix; False - without prefix.
        Padding         [bool]   : Is the return hexadecimal need zero-padding or not. True - with 0 padding;
                                   False (Default) - without 0 padding.
        PaddingRight    [bool]   : To do the 0 padding on return value. True - on right side; False (Default) - on left side.
        PaddingSize     [int]    : The padding size.
        PrefixLowercase [bool]   : The case of the prefix "x". True (Default) - Lowercase; False - UpperCase.

    Returns:
        Output [str]: The hexadecimal number string after being formatted.

    Raises:
        None.
    """
    Output = DecToHex (Hex, Lowercase) if type (Hex) is int else Hex

    if Padding:
        if PaddingRight:
            Output = Output.ljust (PaddingSize, '0')
        else:
            Output = Output.rjust (PaddingSize, '0')

    if Prefix:
        Output = RemoveHexPrefix (Output)  # To prevent duplicate prefix
        if PrefixLowercase:
            Output = '0x' + Output
        else:
            Output = '0X' + Output

    return Output

def RemoveHexPrefix (Hex):
    """ Remove the hexadecimal 0x prefix.

    Args:
        Hex [int|str]: The hexadecimal number with string.
                       It is caller responsible for providing the correct value.

    Returns:
        HexString [str]: The hexadecimal number string without "0x" prefix.

    Raises:
        None.
    """
    HexString = str (Hex)
    HexString = HexString.replace ('0x', '')
    HexString = HexString.replace ('0X', '')
    return HexString

def Padding (String, Size, PaddingRight=False, PaddingString=' '):
    """ To do the padding from giving string and giving padding string.

    Args:
        String        [str] : The string to do the padding.
        Size          [int] : The padding size.
        PaddingRight  [bool]: To do the padding on return value. True - on right side; False (Default) - on left side.
        PaddingString [str] : The string used to do the padding

    Returns:
        Result [str]: The string after do the padding.

    Raises:
        None.
    """
    Result = str (String)

    if PaddingRight:
        Result = Result.ljust (Size, PaddingString)
    else:
        Result = Result.rjust (Size, PaddingString)

    return Result

def ZeroPadding (String, Size, PaddingRight=False):
    """ To do the zero padding from giving string.

    Args:
        String       [str] : The string to do the zero padding.
        Size         [int] : The padding size.
        PaddingRight [bool]: To do the 0 padding on return value. True - on right side; False (Default) - on left side.

    Returns:
        Result [str]: The string after do the zero padding.

    Raises:
        None.
    """
    Result = Padding (String, Size, PaddingRight, '0')
    return Result

def CovertStrToInt (Input):
    """ To covert the string to integer number.

    Args:
        Input [str|int]: The input value to be converted to string.
                         Could be the hexadecimal number with 0x.

    Returns:
        Result [str]: The string after do the padding.

    Raises:
        TypeError: The input type is not string nor integer number.
    """
    if type (Input) == str:
        try:
            return int (Input, 0)
        except:
            print (f"!!! Input value [{Input}] not integer format !!!")
            raise TypeError(f"!!! Only allowed to input the integrate format value !!!")
    elif type (Input) == int:
        return Input
    else:
        print (f"Input type is {type (Input)}.")
        raise TypeError ("!!! Input only allow string or integrate !!!")

def GetFileNameAndExt (Path):
    """ Get the filename and file extension from path.
        It is caller responsible for giving the correct file path.

    Args:
        Path [str]: The string of path.

    Returns:
        FileName  [str]: The filename from given path.
        Extension [str]: The file extension from given path.

    Raises:
        None.
    """
    FileName  = str ()
    Extension = str ()

    if Path is None or not Path:
        pass
    else:
        FileName, Extension = os.path.splitext(Path)
        FileName = os.path.basename (FileName)

    return FileName, Extension

def CheckIntInRange (Input, Min, Max):
    """ Check the integer number is located in the specific range.

    Args:
        Input [str|int]: The integer number to be test.
        Min   [str|int]: The minimum number for the range.
        Max   [str|int]: The maximum number for the range.

    Returns:
        Result [bool]: True  - The number in range.
                       False - The number out of range.

    Raises:
        None.
    """
    InputValue = CovertStrToInt (Input)
    MinValue   = CovertStrToInt (Min)
    MaxValue   = CovertStrToInt (Max)

    if MinValue <= InputValue and InputValue <= MaxValue:
        return True
    else:
        return False
