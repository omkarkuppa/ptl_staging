## @file
#  Library provided basic function to do the value checking.
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

def CheckIntInRange (
    Input   : Union[str, int],
    MinRange: Union[str, int],
    MaxRange: Union[str, int],
    ) -> bool:
    """ To check the specific integer is located in specific range.

    Note:
        (1) To input the hexadecimal value should with 0-base prefix.
        (2) If min range and max range input in incorrect order.
            Function would swap two variables.

    Args:
        Input (Union[str, int]):
            The value to be tested.
            If it is hexadecimal value should with 0-base prefix.
            (To prevent get confused)
        MinRange (Union[str, int]):
            The minimum value in range.
            If it is hexadecimal value should with 0-base prefix.
            (To prevent get confused)
        MaxRange (Union[str, int]):
            The maximum value in range.
            If it is hexadecimal value should with 0-base prefix.
            (To prevent get confused)

    Raises:
        None.

    Returns:
        bool:
            True  - The input value is in specific range.
            False - The input value is not in specific range.
    """
    InputValue: int = ToInt (Input)
    MinValue  : int = ToInt (MinRange)
    MaxValue  : int = ToInt (MaxRange)

    if MinValue > MaxValue:
        #
        # To handle the min. and max. value input in incorrect order.
        #
        MinValue, MaxValue = MaxValue, MinValue

    if MinValue <= InputValue and InputValue <= MaxValue:
        return True
    else:
        return False

def CheckStringMatch (
    Input1      : str,
    Input2      : str,
    IsTypeCheck : bool = True,
    IsIgnoreCase: bool = True,
    ) -> bool:
    """ Check the two strings are matched. (Default case-insensitive)

    Args:
        Input1 (str):
            One string to be compared.
        Input2 (str):
            The other string to be compared.
        IsTypeCheck (bool, optional):
            Check input strings instance is str.
            Defaults to True.
        IsIgnoreCase (bool, optional):
            Mode to choose is case-sensitive or case-insensitive.
            Defaults to True.

    Raises:
        TypeError:
            (1) IsTypeCheck is not bool type.
            (2) IsIgnoreCase is not bool type.

    Returns:
        bool:
            True  - Two strings are matched.
            False - Two strings are not matched.
    """
    if not isinstance (IsTypeCheck, bool):
        raise TypeError (f'IsTypeCheck should be bool type.')
    elif not isinstance (IsIgnoreCase, bool):
        raise TypeError (f'IsIgnoreCase should be bool type.')

    if IsTypeCheck:
        assert (isinstance (Input1, str))
        assert (isinstance (Input2, str))

    InputString1: str = str (Input1)
    InputString2: str = str (Input2)

    if IsIgnoreCase:
        return InputString1.casefold () == InputString2.casefold ()
    else:
        return InputString1 == InputString2
