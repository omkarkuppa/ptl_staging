## @file
#  Library provided basic function to do the system operation.
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

import platform
import shlex
import subprocess
import sys
from typing import Tuple

from CapsuleCommon.Base.BaseLib import *
from CapsuleCommon.Base.DebugLib import *
from CapsuleCommon.Base.DirectoryLib import *
from CapsuleCommon.Base.PathLib import *

#
# Definition for input/output stream.
#
STREAM_PIPE  : int = 0x00
STREAM_STDIN : int = 0x01
STREAM_STDOUT: int = 0x02
STREAM_STDERR: int = 0x03

STREAM_MAPPING_TABLE: Dict[int, Any] = {
    STREAM_PIPE  : subprocess.PIPE,
    STREAM_STDIN : sys.stdin,
    STREAM_STDOUT: sys.stdout,
    STREAM_STDERR: sys.stderr,
    }

def GetSystemPlatform () -> str:
    """ Get the system platform name.

    Args:
        None.

    Raises:
        None.

    Returns:
        str:
            The current system platform name.
            It is leverage from Python native package platform.
    """
    System: str = platform.system ()
    DEBUG (DEBUG_TRACE, f'Platform System: {System}')

    return System

def IsWindowsPlatform () -> bool:
    """ Check if it is Windows-based system.

    Args:
        None.

    Raises:
        None.

    Returns:
        bool:
            True  - It is Windows-based system.
            False - It is not Windows-based system.
    """
    Result: bool = False

    Result = (platform.system () == 'Windows')
    DEBUG (DEBUG_TRACE, f'Windows-based System: {Result}')

    return Result

def IsLinuxPlatform () -> bool:
    """ Check if it is Linux-based system.

    Args:
        None.

    Raises:
        None.

    Returns:
        bool:
            True  - It is Linux-based system.
            False - It is not Linux-based system.
    """
    Result: bool = False

    Result = (platform.system () == 'Linux')
    DEBUG (DEBUG_TRACE, f'Linux-based System: {Result}')

    return Result

def IsMacPlatform () -> bool:
    """ Check if it is macOS-based system.

    Args:
        None.

    Raises:
        None.

    Returns:
        bool:
            True  - It is macOS-based system.
            False - It is not macOS-based system.
    """
    Result: bool = False

    Result = (platform.system () == 'Darwin')
    DEBUG (DEBUG_TRACE, f'MacOS-based System: {Result}')

    return Result

def ExecOsSystemCmd (
    Cmd        : Union[str, List[str]],
    Env        : dict = None,
    Cwd        : str  = None,
    StdOut     : int  = STREAM_STDOUT,
    StdErr     : int  = STREAM_STDERR,
    ErrMsg     : str  = '',
    IsException: bool = True,
    ) -> Tuple[int, bytes, bytes]:
    """ Call the system command from OS.

    Note:
        Caller could decide to raise the exception or not.

    Args:
        Cmd (Union[str, List[str]]):
            The command to be executed.
        Env (dict, optional):
            Customize environment variable. (Should be key-value)
            Defaults to None.
        Cwd (str, optional):
            Set the working directory for executing.
            Defaults to None.
        StdOut (int, optional):
            Standard output stream.
            Defaults to STREAM_STDOUT.
        StdErr (int, optional):
            Standard error output stream.
            Defaults to STREAM_STDERR.
        ErrMsg (str, optional):
            The error message to raise the exception.
            Defaults to ''.
        IsException (bool, optional):
            True  - To raise the exception when exitcode not is 0x0.
            False - Not to raise the exception when exitcode not is 0x0.
            Defaults to True.

    Raises:
        TypeError:
            (1) IsException is not bool type.
            (2) Cwd is passed but not str type.
            (3) Cmd should be str or list of str.
        ValueError:
            (1) Cwd path is not exist.
            (2) Cwd path is not directory.
            (3) StdOut is not one of the STREAM_MAPPING_TABLE key.
            (4) StdErr is not one of the STREAM_MAPPING_TABLE key.
        Exception:
            If IsException is True, would raise the exception based on
            input ErrMsg when exitcode is not 0x0.

    Returns:
        Tuple[int, bytes, bytes]:
            int:
                Program execute exitcode number. (0x0 is succeed)
            bytes:
                The output log in bytes.
            bytes:
                The error log in bytes.
    """
    FullCmdList: List[str]        = list ()
    ExitCode   : Union[None, int] = None
    Output     : bytes            = bytes ()
    Error      : bytes            = bytes ()
    Program    : subprocess.Popen = None

    if not isinstance (IsException, bool):
        raise TypeError (f'IsException should be bool type.')

    if Cwd is not None:
        if not isinstance (Cwd, str):
            raise TypeError (f'Cwd should be str type.')

        if not IsPathExist (Cwd):
            raise ValueError (f'Assign working directory [{Cwd}] not exist.')
        elif not IsDir (Cwd):
            raise ValueError (f'Assign working directory [{Cwd}] not directory.')

    if isinstance (Cmd, str):
        FullCmdList = shlex.split (Cmd)
    elif isinstance (Cmd, list):
        FullCmdList = list (filter (None, Cmd))
    else:
        raise TypeError (f'Command should be str or list of str.')

    if StdOut not in STREAM_MAPPING_TABLE.keys ():
        raise ValueError (f'StdOut is invalid value - [{StdOut}]')
    elif StdErr not in STREAM_MAPPING_TABLE.keys ():
        raise ValueError (f'StdErr is invalid value - [{StdErr}]')

    Program       = subprocess.Popen (
                                 FullCmdList,
                                 stdout = STREAM_MAPPING_TABLE[StdOut],
                                 stderr = STREAM_MAPPING_TABLE[StdErr],
                                 env    = Env,
                                 cwd    = Cwd,
                                 )
    Output, Error = Program.communicate()
    ExitCode      = Program.returncode

    if (ExitCode != STATUS_SUCCESS) and (IsException):
        raise Exception (ErrMsg)

    return ExitCode, Output, Error

def ExecPythonCmd (
    Cmd        : Union[str, List[str]],
    Env        : dict = None,
    Cwd        : str  = None,
    StdOut     : int  = STREAM_STDOUT,
    StdErr     : int  = STREAM_STDERR,
    ErrMsg     : str  = '',
    IsException: bool = True,
    ) -> Tuple[int, bytes, bytes]:
    """ Wrapper based on ExecOsSystemCmd to add the Python cmd.

    Note:
        Caller could decide to raise the exception or not.

    Args:
        Cmd (Union[str, List[str]]):
            The command to be executed.
        Env (dict, optional):
            Customize environment variable. (Should be key-value)
            Defaults to None.
        Cwd (str, optional):
            Set the working directory for executing.
            Defaults to None.
        StdOut (int, optional):
            Standard output stream.
            Defaults to STREAM_STDOUT.
        StdErr (int, optional):
            Standard error output stream.
            Defaults to STREAM_STDERR.
        ErrMsg (str, optional):
            The error message to raise the exception.
            Defaults to ''.
        IsException (bool, optional):
            True  - To raise the exception when exitcode not is 0x0.
            False - Not to raise the exception when exitcode not is 0x0.
            Defaults to True.

    Raises:
        TypeError:
            Cmd should be str or list of str.

    Returns:
        Tuple[int, bytes, bytes]:
            int:
                Program execute exitcode number. (0x0 is succeed)
            bytes:
                The output log in bytes.
            bytes:
                The error log in bytes.
    """
    FullCmdList: List[str] = list ()
    PyCmdList  : List[str] = GetPythonCmdList ()

    if isinstance (Cmd, str):
        FullCmdList = shlex.split (Cmd)
    elif isinstance (Cmd, list):
        FullCmdList = list (filter (None, Cmd))
    else:
        raise TypeError (f'Command should be str or list of str.')

    FullCmdList = PyCmdList + FullCmdList

    return ExecOsSystemCmd (
             FullCmdList,
             Env         = Env,
             Cwd         = Cwd,
             StdOut      = StdOut,
             StdErr      = StdErr,
             ErrMsg      = ErrMsg,
             IsException = IsException
             )

def ExecWinExeCmd (
    Cmd        : Union[str, List[str]],
    Env        : dict = None,
    Cwd        : str  = None,
    StdOut     : int  = STREAM_STDOUT,
    StdErr     : int  = STREAM_STDERR,
    ErrMsg     : str  = '',
    IsException: bool = True,
    ) -> Tuple[int, bytes, bytes]:
    """ Wrapper based on ExecOsSystemCmd to execute Windows EXE program.

    Note: Caller could decide to raise the exception or not.

    Args:
        Cmd (Union[str, List[str]]):
            The command to be executed.
        Env (dict, optional):
            Customize environment variable. (Should be key-value)
            Defaults to None.
        Cwd (str, optional):
            Set the working directory for executing.
            Defaults to None.
        StdOut (int, optional):
            Standard output stream.
            Defaults to STREAM_STDOUT.
        StdErr (int, optional):
            Standard error output stream.
            Defaults to STREAM_STDERR.
        ErrMsg (str, optional):
            The error message to raise the exception.
            Defaults to ''.
        IsException (bool, optional):
            True  - To raise the exception when exitcode not is 0x0.
            False - Not to raise the exception when exitcode not is 0x0.
            Defaults to True.

    Raises:
        TypeError:
            Cmd should be str or list of str.

    Returns:
        Tuple[int, bytes, bytes]:
            int:
                Program execute exitcode number. (0x0 is succeed)
            bytes:
                The output log in bytes.
            bytes:
                The error log in bytes.
    """
    FullCmdList: List[str] = list ()
    ExeCmdList : List[str] = list ()

    if isinstance (Cmd, str):
        FullCmdList = shlex.split (Cmd)
    elif isinstance (Cmd, list):
        FullCmdList = list (filter (None, Cmd))
    else:
        raise TypeError (f'Command should be str or list of str.')

    if not IsWindowsPlatform ():
        ExeCmdList = [LINUX_WIN_EXE_WINE_CMD]

    FullCmdList = ExeCmdList + FullCmdList

    return ExecOsSystemCmd (
             FullCmdList,
             Env         = Env,
             Cwd         = Cwd,
             StdOut      = StdOut,
             StdErr      = StdErr,
             ErrMsg      = ErrMsg,
             IsException = IsException
             )
