## @file
#  Library provided the basic function to do the directory operation.
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
import shutil
from typing import Union

from CapsuleCommon.Base.DebugLib import *
from CapsuleCommon.Base.PathLib import *

def IsDir (SourcePath: Union[str, os.PathLike]) -> bool:
    """ To check the input path is directory.

    Args:
        SourcePath (Union[str, os.PathLike]):
            To check the specific path is a directory.

    Raises:
        None.

    Returns:
        bool:
            True  - Specific path is directory.
            False - Specific path is not directory.
    """
    Source: str = AbsPath (SourcePath)

    return os.path.isdir (Source)

def MakeDir (TargetPath: Union[str, os.PathLike]) -> bool:
    """ To create the correspond directory with specific path.

    Note:
        (1) If directory is existence, function would bypass.
        (2) Others error would raise the exception by os.makedirs.

    Args:
        TargetPath (Union[str, os.PathLike]):
            The specific path to create the directory.

    Raises:
        None.

    Returns:
        bool:
            True  - Success to create the specific directory.
            False - Directory is existence, no need to create.
    """
    Result: bool = False
    Path  : str  = AbsPath (TargetPath)

    try:
        os.makedirs (Path)
    except FileExistsError:
        DEBUG (
          DEBUG_TRACE,
          f'The directory "{Path}" is existence, fail '
          f'to create the directory.'
          )

    return Result

def TouchDir (TargetPath: Union[str, os.PathLike]) -> bool:
    """ Create the directory with specific path if not existence.

    Args:
        TargetPath (Union[str, os.PathLike]):
            The specific path to create the directory if not existence.

    Raises:
        None.

    Returns:
        bool:
            True  - Exist and no need to create the specific directory.
            False - Not exist and create the specific directory.
    """
    Result: bool = False
    Path  : str  = AbsPath (TargetPath)

    if IsPathExist (Path):
        if not IsDir (Path):
            raise ValueError (f'Path is exist but not directory. [{Path}]')
        else:
            DEBUG (
              DEBUG_INFO,
              f'The directory "{Path}" is exist, skip to create the directory.'
              )
            Result = True
    else:
        MakeDir (Path)

    return Result

def CopyDir (
    SourcePath : Union[str, os.PathLike],
    TargetPath : Union[str, os.PathLike],
    IsOverwrite: bool = True,
    ) -> None:
    """ Recursive to from specific directory to another one.

    Args:
        SourcePath (Union[str, os.PathLike]):
            The specific source directory path.
        TargetPath (Union[str, os.PathLike]):
            The specific destination directory path.
        IsOverwrite (bool, optional):
            True  - Without raising the exception when target
                    existence/source missing.
            False - With raising the exception when target
                    existence/source missing.
            Defaults to True.

    Raises:
        TypeError:
            IsOverwrite is not bool type.

    Returns:
        None.
    """
    Source: str = AbsPath (SourcePath)
    Target: str = AbsPath (TargetPath)

    if not isinstance (IsOverwrite, bool):
        raise TypeError (f'IsOverwrite should be bool type.')

    DEBUG (DEBUG_TRACE, f'Copy the directory: {Source} -> {Target}')

    shutil.copytree (Source, Target, dirs_exist_ok = IsOverwrite)

def RemoveDir (Path: Union[str, os.PathLike], IsSafeRemove = True) -> bool:
    """ To remove the correspond directory with specific path.

    Args:
        Path (Union[str, os.PathLike]):
            The specific directory path to be removed.
        IsSafeRemove (bool, optional):
            True  - Check the path exist and not file first.
            False - Do not check try to remove directly.
            Defaults to True.

    Raises:
        TypeError:
            IsSafeRemove is not bool type.

    Returns:
        bool:
            True  - Directory remove successfully.
            False - Directory remove failed.
    """
    TargetPath: str = AbsPath (Path)

    if not isinstance (IsSafeRemove, bool):
        raise TypeError (f'IsSafeRemove should be bool type.')

    DEBUG (DEBUG_TRACE, f'Prepare to remove the directory: {TargetPath}')

    if IsSafeRemove:
        if not IsPathExist (TargetPath):
            DEBUG (DEBUG_TRACE, f'Not exist [{TargetPath}] no need to remove.')
            return False
        elif not IsDir (TargetPath):
            DEBUG (DEBUG_TRACE, f'[{TargetPath}] is file, can not remove.')
            return False

    shutil.rmtree (TargetPath)

    return True

def CheckDirListExist (InputList: list, IsBypassEmpty: bool = False) -> None:
    """ Check list of path is exist and is directory.

    Args:
        InputList (list):
            List of path to be checked.
        IsBypassEmpty (bool, optional):
            If check the input is empty or None.
            True  - Bypass the check.
            False - Raise the ValueError exception.
            Defaults to False.

    Raises:
        TypeError:
            Input is not list type.
        ValueError:
            (1) Path is empty. (When IsBypassEmpty is False)
            (2) Path is not exist.
            (3) Path is not directory.

    Returns:
        None.
    """
    if not isinstance (InputList, list):
        raise TypeError (f'Check the input should be list type.')

    for Path in InputList:
        DEBUG (DEBUG_TRACE, f'Check the path: {Path}')
        if not Path:
            if IsBypassEmpty:
                continue
            else:
                raise ValueError (f'File path check failed. Can not empty.')

        if not IsPathExist (Path):
            raise ValueError (f'Path [{Path}] is not exist.')

        if not IsDir (Path):
            raise ValueError (f'Path [{Path}] is not directory.')
