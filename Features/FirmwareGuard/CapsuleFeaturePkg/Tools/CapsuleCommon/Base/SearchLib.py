## @file
#  Library provided basic function to do the search operation.
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

from typing import Any, Dict, List, Tuple

from CapsuleCommon.Base.CheckLib import *

def SearchValueInList (
    InputList   : List[Any],
    TargetValue : Any,
    IsIgnoreCase: bool = True,
    IsLastItem  : bool = True,
    IsTypeCheck : bool = True,
    ) -> Tuple [bool, Union[None, Any]]:
    """ Search specific target item within the list.

    Note:
        Each list element and target should be available to do the type
        casting into str.

    Args:
        InputList (List[Any]):
            The list of input to be found the specific target value.
            If input type is not the instance of list, would get assert.
            (It is need to be converted into string to do comparison
            within each element)
        TargetValue (Any):
            The value to be use to search.
            (It is need to be converted into string to do comparison)
        IsIgnoreCase (bool, optional):
            Option to search within case-insensitive.
            True  - To ignore the case-sensitive.
            False - Enable the case-sensitive.
            Defaults to True.
        IsLastItem (bool, optional):
            Option to return the last matched item.
            True  - Return the last matched item.
            False - Return the first matched item.
            Defaults to True.
        IsTypeCheck (bool, optional):
            Option to do the type checking between source element and
            target.
            True  - With the type checking before comparison.
            False - Without the type checking before comparison.
            Defaults to True.

    Raises:
        TypeError:
            (1) IsIgnoreCase is not bool type.
            (2) IsLastItem is not bool type.
            (3) IsTypeCheck is not bool type.

    Returns:
        bool:
            True  - Found the correspond target value.
            False - Not found the correspond target value.
        Union[None, Any]:
            Correspond element within list.
            If it is None, need to check if found.
    """
    assert (isinstance (InputList, list) == True)

    IsFound: bool = False
    Result : Any  = None
    Source : str  = str ()
    Target : str  = str (TargetValue)

    if not isinstance (IsIgnoreCase, bool):
        raise TypeError (f'IsIgnoreCase should be bool type.')
    elif not isinstance (IsLastItem, bool):
        raise TypeError (f'IsLastItem should be bool type.')
    elif not isinstance (IsTypeCheck, bool):
        raise TypeError (f'IsTypeCheck should be bool type.')

    for SourceValue in InputList:
        #
        # If need to check the type first.
        #
        if IsTypeCheck and (type (SourceValue) != type (TargetValue)):
            continue

        #
        # To check the string is matched.
        #
        Source = str (SourceValue)
        if CheckStringMatch (Source, Target, IsIgnoreCase):
            IsFound = True
            Result = SourceValue

        #
        # If no need to find the last item, break iteration when found
        # the first item.
        #
        if (IsFound) and (not IsLastItem):
            break

    return IsFound, Result

def SearchKeyInDict (
    InputDict   : Dict[Any, Any],
    TargetKey   : Any,
    IsIgnoreCase: bool = True,
    IsLastItem  : bool = True,
    IsTypeCheck : bool = True,
    ) -> Tuple [bool, Union[None, Any]]:
    """ Search specific target key within the dict.

    Note:
        (1) Each item's key in dict and target should be available
            to do the type casting into str.
        (2) For nested dict case, only support the top-level.

    Args:
        InputDict (Dict[Any, Any]):
            The dict of key list to be found the specific target value.
            If input type is not the instance of dict, would get assert.
            (It is need to be converted into string to do comparison
             within each element's key)
        TargetKey (Any):
            The value to be use to search within the keys.
            (It is need to be converted into string to do comparison)
        IsIgnoreCase (bool, optional):
            Option to search within case-insensitive.
            True  - To ignore the case-sensitive.
            False - Enable the case-sensitive.
            Defaults to True.
        IsLastItem (bool, optional):
            Option to return the last matched item.
            True  - Return the last matched item.
            False - Return the first matched item.
            Defaults to True.
        IsTypeCheck (bool, optional):
            Option to do the type checking between source element and
            target.
            True  - With the type checking before comparison.
            False - Without the type checking before comparison.
            Defaults to True.

    Returns:
        bool:
            True  - Found the correspond target value.
            False - Not found the correspond target value.
        Union[None, Any]:
            Correspond item's key within dict.
            If it is None, need to check if found.
    """
    assert (isinstance (InputDict, dict) == True)

    IsFound     : bool = False
    DictKeysList: list = list (InputDict.keys ())
    Result      : Any  = None

    IsFound, Result = SearchValueInList (
                        DictKeysList,
                        TargetKey,
                        IsIgnoreCase = IsIgnoreCase,
                        IsLastItem   = IsLastItem,
                        IsTypeCheck  = IsTypeCheck,
                        )

    return IsFound, Result
