## @file
#  Script to build the EDKII FD file.
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
import argparse
from typing import List

from CapsuleCommon import *

MAJ_VER: int = 0
MIN_VER: int = 1

__prog__       : str = os.path.basename (__file__)
__version__    : str = f'{__prog__} Version {MAJ_VER}.{MIN_VER}'
__copyright__  : str = 'Copyright (C) 2023 Intel Corporation. All rights reserved.\n'
__description__: str = 'Script to build the EDKII FD file.\n'

class Edk2FdBuilder (object):
    def __init__ (
        self,
        BaseToolsPath: Union[str, os.PathLike],
        WorkspacePath: Union[str, os.PathLike],
        FdfFile      : str,
        DscFile      : str,
        FvName       : str,
        ) -> None:
        """ Class to build the EDKII FD file from BaseTools.

        Args:
            BaseToolsPath (Union[str, os.PathLike]):
                Path to EDK-II BaseTools.
            WorkspacePath (Union[str, os.PathLike]):
                EDK-II defined workspace path.
            FdfFile (str):
                The path of target platform FDF file.
            DscFile (str):
                The path of target platform DSC file.
            FvName (str):
                The FV name within this target platform FDF file.

        Raises:
            None.

        Returns:
            None.
        """
        self.__BaseToolsPath: str = AbsPath (BaseToolsPath)
        self.__WorkspacePath: str = AbsPath (WorkspacePath)

        self.__FdfFile: str = FdfFile
        self.__DscFile: str = DscFile
        self.__FvName : str = FvName

        self.__EnvConfig: dict = \
            Edk2BuildEnv (
              Workspace     = self.__WorkspacePath,
              Edk2ToolsPath = BaseToolsPath,
              ).EnvVar

    def __Verbose (self) -> None:
        """ Dump the debug information.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        DEBUG (DEBUG_INFO, f'Edk2BaseTools: {self.__BaseToolsPath}')
        DEBUG (DEBUG_INFO, f'Workspace    : {self.__WorkspacePath}')
        DEBUG (DEBUG_INFO, f'FDF File     : {self.__FdfFile}')
        DEBUG (DEBUG_INFO, f'DSC File     : {self.__DscFile}')
        DEBUG (DEBUG_INFO, f'FV  Name     : {self.__FvName}')

    def Build (self) -> int:
        """ Function to call the build FD command.

        Note:
            The usage need to align within the EDKII BaseTools.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                The exitcode after calling the specific program.
        """
        self.__Verbose ()

        Cmd: List[str] = [
                JoinPath (self.__BaseToolsPath, GetGenFdsToolPath ()),
                '-f', f'{self.__FdfFile}',
                '-p', f'{self.__DscFile}',
                '-i', f'{self.__FvName}',
                '-v',
                ]

        ExitCode, _, _ = ExecOsSystemCmd (
                           Cmd,
                           Env = self.__EnvConfig,
                           Cwd = f'{self.__WorkspacePath}',
                           )

        return ExitCode

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'Edk2FdBuilder',
    ]

if __name__ == '__main__':
    def GetArgs () -> argparse.Namespace:
        """ Get the user input arguments.

        Args:
            None.

        Raises:
            None.

        Returns:
            Args [argparse.Namespace]:
                The argument object parsing from user input.
        """
        Parser: argparse.ArgumentParser = argparse.\
            ArgumentParser (
              prog            = __prog__,
              description     = __copyright__ + __description__ + __version__,
              formatter_class = argparse.RawTextHelpFormatter,
              )
        Parser.add_argument (
                 '-V',
                 '-v',
                 '--Version',
                 action  = 'version',
                 version = __version__,
                 )
        Parser.add_argument (
                 '-t',
                 '-T',
                 '--basetools',
                 dest     = 'BaseTools',
                 help     = 'The path of EDKII base tools.',
                 required = True,
                 )
        Parser.add_argument (
                 '-w',
                 '-W',
                 '--workspace',
                 dest     = 'Workspace',
                 help     = 'The path of EDKII workspace.',
                 required = True,
                 )
        Parser.add_argument (
                 '-d',
                 '-D',
                 '--dsc',
                 dest     = 'Dsc',
                 help     = 'The path of target platform DSC file.',
                 required = True,
                 )
        Parser.add_argument (
                 '-f',
                 '-F',
                 '--fdf',
                 dest     = 'Fdf',
                 help     = 'The path of target platform FDF file.',
                 required = True,
                 )
        Parser.add_argument (
                 '-fv',
                 '-FV',
                 '--fv',
                 dest     = 'Fv',
                 help     = 'The FV name within target platform.',
                 required = True,
                 )

        return Parser.parse_args ()

    Args: argparse.Namespace = GetArgs ()

    BaseToolsPath: str = Args.BaseTools
    WorkspacePath: str = Args.Workspace
    FdfFilePath  : str = Args.Fdf
    DscFilePath  : str = Args.Dsc
    FvName       : str = Args.Fv

    Builder: Edk2FdBuilder = \
        Edk2FdBuilder (
          BaseToolsPath = BaseToolsPath,
          WorkspacePath = WorkspacePath,
          FdfFile       = FdfFilePath,
          DscFile       = DscFilePath,
          FvName        = FvName,
          )

    Result: int = Builder.Build ()

    sys.exit (Result)
