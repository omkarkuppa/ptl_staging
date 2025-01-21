## @file
#  Library provided basic function to do the file operation.
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
from io import TextIOWrapper
from typing import Tuple, Union

from CapsuleCommon.Base.DebugLib import *
from CapsuleCommon.Base.DirectoryLib import *
from CapsuleCommon.Base.PathLib import *

def IsFile (SourcePath: Union[str, os.PathLike]) -> bool:
    """ To check the input path is file.

    Args:
        SourcePath (Union[str, os.PathLike]):
            To check the specific path is a file.

    Raises:
        None.

    Returns:
        bool:
            True  - Specific path is file.
            False - Specific path is not file.
    """
    Source: str = AbsPath (SourcePath)
    return os.path.isfile (Source)

def CreateDummyFile (TargetPath: Union[str, os.PathLike]) -> None:
    """ Create a file with '$DUM' byte file.

    Args:
        TargetPath (Union[str, os.PathLike]):
            The file path to create the dummy file.

    Raises:
        None.

    Returns:
        None.
    """
    Path: str = AbsPath (TargetPath)

    TouchDir (os.path.dirname (Path))

    DEBUG (DEBUG_TRACE, f'Create the dummy file: {Path}')

    try:
        File: TextIOWrapper = open (Path, 'wb')
        File.write (b'$DUM')
    finally:
        File.close ()

def CopyFile (
    SourcePath : Union[str, os.PathLike],
    TargetPath : Union[str, os.PathLike],
    IsOverwrite: bool = True,
    ) -> None:
    """ Copy one file with specific path to another specific path.

    Note:
        If path had been existed, default would overwrite it.

    Args:
        SourcePath (Union[str, os.PathLike]):
            The specific path to copy from.
        TargetPath (Union[str, os.PathLike]):
            The specific path to copy destination.
        IsOverwrite (bool, optional):
            True  - If destination exist, do the overwrite on it.
            False - If destination exist, do nothing.
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

    DEBUG (DEBUG_TRACE, f'Copy file: {Source} -> {Target}')

    if IsPathExist (Target):
        if not IsFile (Target):
            DEBUG (DEBUG_ERROR, f'Can not copy to {Target}. It is not a file.')
            return
        elif not IsOverwrite:
            DEBUG (
              DEBUG_WARN,
              f'Destination path [{Target}] without doing copy operation.'
              )
            return

    try:
        shutil.copyfile (Source, Target)
    except shutil.SameFileError:
        DEBUG (
          DEBUG_WARN,
          f'No need to copy file since source and target path is the same.'
          )

def CombineFileName (
    *Args    : List[Union[str, List]],
    Extension: str = '',
    Separator: str = '_',
    ) -> str:
    """ To merge the list of string or string into a filename.

    Args:
        *Args (List[Union[str, List]):
            The input str or list type to be merged.
        Extension (str, optional):
            The file extension part.
            Defaults to ''.
        Separator (str, optional):
            The separator to separate different string part.
            Defaults to '_'.

    Raises:
        TypeError:
            (1) Input Extension not str type.
            (2) Input Separator not str type.
            (3) Input Args type should be str or list.
        ValueError:
            The input Args is empty.

    Returns:
        str:
            The filename and extension after getting merged.
    """
    FileNameList: list             = list ()
    MaxInputLen : int              = len (Args)
    Value       : Union[str, list] = None
    FileName    : str              = str ()
    PostExt     : str              = str ()

    if not isinstance (Extension, str):
        raise TypeError (f'Ext only support str type: {type (Extension)}')

    if not isinstance (Separator, str):
        raise TypeError (f'Sep only support str type: {type (Separator)}')

    if MaxInputLen <= 0:
        raise ValueError ('!!! Should at least input one parameter !!!')

    for Idx in range (MaxInputLen):
        Value = Args[Idx]
        if isinstance (Value, str):
            FileNameList.append (Value)
        elif isinstance (Value, list):
            FileNameList = FileNameList + Value
        else:
            raise TypeError (f'Type of {type (Value)} is not supported.')

    FileName = Separator.join (FileNameList)
    PostExt  = '.'.join (filter (None, Extension.split ('.')))

    if PostExt:
        FileName = FileName + '.' + PostExt

    return FileName

def GetFileNameAndExt (SourcePath: Union[str, os.PathLike]) -> Tuple[str, str]:
    """ To get the filename and extension from specific path.

    Args:
        SourcePath (Union[str, os.PathLike]):
            The file path to get the filename and extension.

    Raises:
        ValueError:
            (1) The input path is not empty.
            (2) The input path is not a file.

    Returns:
        tuple:
            str:
                The filename part.
            str:
                The extension part.
    """
    Source   : str = AbsPath (SourcePath)
    FileName : str = str ()
    Extension: str = str ()

    if not Source:
        raise ValueError (f'!!! The input file path can not empty !!!')

    if not IsPathExist (Source):
        DEBUG (DEBUG_WARN, f'The path is not exist trend it is a file.')
    elif not IsFile (Source):
        raise ValueError (f'!!! The input path should be a file !!!')

    FileName, Extension = os.path.splitext (Source)
    FileName            = os.path.basename (FileName)

    return FileName, Extension

def ReadTextFile (
    SourcePath : Union[str, os.PathLike],
    Encode     : str  = 'utf-8',
    IsReadLines: bool = False,
    ) -> Union[str, List[str]]:
    """ Read the text-based file from storage to memory.

    Args:
        SourcePath (Union[str, os.PathLike]):
            The path of text-based file to be read.
        Encode (str, optional):
            The encoding of this text-based file.
            Defaults to 'utf-8'.
        IsReadLines (bool, optional):
            True  - To read the file by one-by-one line in list.
            False - To read the file within one string.
            Defaults to False.

    Raises:
        TypeError:
            IsReadLines is not bool type.
        ValueError:
            (1) The assigned path is not exist.
            (2) The assigned path is not file.

    Returns:
        Union[str, List[str]]:
            If the IsReadLines is True, return type would be list.
            If the IsReadLines is False, return type would be str.
    """
    Path  : str                         = AbsPath (SourcePath)
    Fp    : Union[None, TextIOWrapper]  = None
    Result: Union[None, str, List[str]] = None

    if not isinstance (IsReadLines, bool):
        raise TypeError (f'IsReadLines should be bool type.')
    elif not IsPathExist (Path):
        raise ValueError (f'Assigned path is not exist: {Path}')
    elif not (IsFile (Path)):
        raise ValueError (f'Assigned path is not file {Path}')

    try:
        Fp = open (Path, 'r', encoding = Encode)

        if IsReadLines:
            Result = Fp.readlines ()
        else:
            Result = Fp.read ()
    finally:
        Fp.close ()

    return Result

def WriteTextFile (
    Data      : Union[str, list],
    TargetPath: Union[str, os.PathLike],
    ) -> None:
    """ Save the text-based file from memory to storage.

    Note:
        If the input is not str/list type, would attempt to do the
        type casting into str.

    Args:
        Data (Union[str, list]):
            The data to be stored into storage.
        TargetPath (Union[str, os.PathLike]):
            The path of text-based file to be output.

    Raises:
        None.

    Returns:
        None.
    """
    Path   : str                        = AbsPath (TargetPath)
    Content: str                        = str ()
    Fp     : Union[None, TextIOWrapper] = None

    if isinstance (Data, str):
        Content = Data
    elif isinstance (Data, list):
        Content = ''.join ([str (Val) for Val in Data])
    else:
        Content = str (Data)

    TouchDir (os.path.dirname (Path))

    DEBUG (DEBUG_TRACE, f'Save text-based file: {Path}')

    try:
        Fp = open (Path, 'w')
        Fp.write (Content)
    except Exception as e:
        DEBUG (
          DEBUG_ERROR,
          f'Error happen on saving the text data to {Path}'
          )
        DEBUG (
          DEBUG_ERROR,
          f'{e.message}'
          )
    finally:
        if Fp:
            Fp.close ()

def ReadBufferFile (
    SourcePath: Union[str, os.PathLike],
    ) -> bytes:
    """ Read the file from storage to memory buffer.

    Args:
        SourcePath (Union[str, os.PathLike]):
            The path of byte-based file to be read.

    Raises:
        FileNotFoundError:
            (1) The assigned path is not existence.
            (2) The assigned path is not file.

    Returns:
        bytes:
            The bytes read from storage.
    """
    Buffer: Union[None, bytes]         = None
    Fp    : Union[None, TextIOWrapper] = None

    if not IsPathExist (SourcePath):
        raise FileNotFoundError (f'!!! Specific path is not exist. !!!')
    elif not IsFile (SourcePath):
        raise FileNotFoundError (f'!!! Specific path is not file. !!!')

    try:
        Fp     = open (SourcePath, 'rb')
        Buffer = Fp.read ()
    finally:
        Fp.close ()

    return Buffer

def WriteBufferFile (
    Buffer    : Union[bytes, bytearray],
    TargetPath: Union[str, os.PathLike],
    ) -> None:
    """ To save the memory buffer into file(storage).

    Note:
        If folder not exist, would try to touch the folder.

    Args:
        Buffer (Union[bytes, bytearray]):
            The byte or bytearray memory buffer to save into storage.
        TargetPath (Union[str, os.PathLike]):
            The path of byte-based file to be output.

    Raises:
        TypeError:
            The input is not bytes/bytearray type.

    Returns:
        None.
    """
    Path: str                        = AbsPath (TargetPath)
    Fp  : Union[None, TextIOWrapper] = None

    if not isinstance (Buffer, (bytes, bytearray)):
        raise TypeError (f'Need input valid bytes/bytearray type for buffer.')

    TouchDir (os.path.dirname (Path))

    try:
        Fp = open (Path, 'wb')
        Fp.write (Buffer)
    except Exception as e:
        DEBUG (
          DEBUG_ERROR,
          f'Error happen on saving the ByteBuffer data to {Path}'
          )
        DEBUG (
          DEBUG_ERROR,
          f'{e.message}'
          )
    finally:
        if Fp:
            Fp.close ()

def RemoveFile (Path: Union[str, os.PathLike], IsSafeRemove = True):
    """ To remove the correspond file with specific path.

    Args:
        Path (Union[str, os.PathLike]):
            The specific file path to be removed.
        IsSafeRemove (bool, optional):
            True  - Check the path exist and not file first.
            False - Do not check try to remove directly.
            Defaults to True.

    Raises:
        TypeError:
            IsSafeRemove is not bool type.

    Returns:
        bool:
            True  - File remove successfully.
            False - File remove failed.
    """
    TargetPath: str = AbsPath (Path)

    if not isinstance (IsSafeRemove, bool):
        raise TypeError (f'IsSafeRemove should be bool type.')

    DEBUG (DEBUG_TRACE, f'Prepare to remove the file: {TargetPath}')

    if IsSafeRemove:
        if not IsPathExist (TargetPath):
            DEBUG (DEBUG_TRACE, f'Not exist [{TargetPath}] no need to remove.')
            return False
        elif not IsFile (TargetPath):
            DEBUG (DEBUG_TRACE, f'[{TargetPath}] is not file, can not remove.')
            return False

    os.remove (Path)

    return True

def CheckFileListExist (InputList: list, IsBypassEmpty: bool = False) -> None:
    """ Check list of path is exist and is file.

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
            (3) Path is not file.

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

        if not IsFile (Path):
            raise ValueError (f'Path [{Path}] is not file.')
