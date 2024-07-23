## @file
#  Library provided the basic function to manipulate path operation.
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

import os
from pathlib import Path, PurePath
from typing import Union

from CapsuleCommon.Base.DebugLib import *

def FormatPath (SourcePath: Union[str, os.PathLike]) -> os.PathLike:
    """ To normalize the input source path.

    Note:
        (1) Remove unnecessary slash or character.
        (2) Change to the correspond slash/back-slash based on different
            platform.

    Args:
        SourcePath (Union[str, os.PathLike]):
            The input path needed to do the normalization.

    Raises:
        TypeError:
            The input is not instance of string and PathLike.

    Returns:
        os.PathLike:
            Output the path after doing the normalization.
    """
    if not isinstance (SourcePath, (str, os.PathLike)):
        raise TypeError (f'Please input the string/PathLike type')

    return Path (SourcePath)

def AbsPath (SourcePath: Union[str, os.PathLike]) -> Union[None, str]:
    """ Convert the directory path into absolute path.

    Note:
        (1) If input is None, return directly.
        (2) If input is empty str, return directly.
        (3) If PurePath input empty str, return the current directory.
        (4) Environment variable within path would be expanded.

    Args:
        SourcePath (Union[str, os.PathLike]):
            The path to be converted into absolute path.

    Raises:
        TypeError:
            The input is not None nor instance of string and PathLike.

    Returns:
        str:
            Output the path after converting to absolute path.
    """
    if not isinstance (SourcePath, (type (None), str, os.PathLike)):
        raise TypeError (f'Please input the None/string/PathLike type')

    if not SourcePath:
        return SourcePath

    FormattedPath: Union[str, os.PathLike] = str ()

    FormattedPath = os.path.expandvars (SourcePath)
    FormattedPath = FormatPath (FormattedPath)
    FormattedPath = os.path.abspath (FormattedPath)

    return FormattedPath

def IsPathExist (SourcePath: Union[str, os.PathLike]) -> bool:
    """ To check the specific path is present.

    Args:
        SourcePath (Union[str, os.PathLike]):
            The input path to be checked existence.

    Raises:
        None

    Returns:
        bool:
            True  - The path is existence.
            False - The path is not existence.
    """
    Path  : str  = AbsPath (SourcePath)
    Result: bool = os.path.exists (Path)

    DEBUG (DEBUG_INFO, f'Checking the path "{Path}" result: {Result}')

    return Result

def JoinPath (*Args) -> str:
    """ To join the two (or more) paths.

    Args:
        *Args:
            Path list to be joined.

    Raises:
        None.

    Returns:
        str:
            The path after getting joined.
    """
    Path    : str      = os.path.join (*Args)
    NormPath: PurePath = PurePath (Path)

    return str (NormPath)
