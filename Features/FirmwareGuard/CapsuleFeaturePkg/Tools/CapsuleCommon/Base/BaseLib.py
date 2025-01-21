## @file
#  Library file for basic functions/definitions.
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

import sys
from typing import Any, List

#
# Command to execute Windows program on non-Windows system
#
LINUX_WIN_EXE_WINE_CMD: str = 'wine'

COMPULSORY   : bool = True
OPTIONAL     : bool = False
NOT_AVAILABLE: None = None

#
# BOOLEAN string for environment variable
#
ENV_VAR_FALSE_STR: str = 'FALSE'
ENV_VAR_TRUE_STR : str = 'TRUE'

#
# Status code
#
STATUS_SUCCESS    : int = 0x0000_0000
STATUS_ERROR      : int = 0x0000_0001
STATUS_UNSUPPORTED: int = 0x0000_0002
STATUS_UNKNOWN    : int = 0xFFFF_FFFF

def GetPythonCmdList () -> List[str]:
    """ Get the list of command for executing Python program.

    Args:
        None.

    Raises:
        NotImplementedError:
            Script is wrapped as the executable program.
        ValueError:
            Failed to get the executed path of Python program.

    Returns:
        List[str]:
            List of command to execute the Python program.
    """
    CmdList: List[str] = list ()

    if getattr (sys, 'frozen', False):
        raise NotImplementedError (f'Not supported to run as frozen program.')

    if not sys.executable:
        raise ValueError (f'Failed to get the executable Python.')

    CmdList.append (sys.executable)

    return CmdList

def IsAscIIStr (Input: str):
    """ To check if input to be trialed is ASC-II string.

    Args:
        Input (str):
            The string to be trialed.

    Raises:
        TypeError:
            The type of input is not str.

    Returns:
        True  - The input string is ASC-II code string.
        False - The input string is not ASC-II code string.
    """
    if not isinstance (Input, str):
        raise TypeError (f'The input is not str type.')

    return Input.isascii ()

def RemoveEmptyFromList (DataList: list) -> List[Any]:
    """ Remove the empty item within the list.

    Note:
        If the member in list is str type and empty, remove it.

    Args:
        DataList (list):
            The list to be removed the empty one.

    Raises:
        TypeError:
            Input is not list type.

    Returns:
        List[Any]:
            The list after removed the empty member.
    """
    Result: List[Any] = list ()

    if not isinstance (DataList, list):
        raise TypeError (f'Only supported the input type is list.')

    for Data in DataList:
        if isinstance (Data, str):
            if not Data:
                continue

        Result.append (Data)

    return Result
