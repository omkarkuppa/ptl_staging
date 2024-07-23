## @file
#  Script to generate the catalog file for Windows Driver Package.
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
#   - Inf2Cat: https://learn.microsoft.com/en-us/windows-hardware/drivers/devtest/inf2cat
#
##

import os
import argparse

from CapsuleCommon import *

MAJ_VER: int = 0
MIN_VER: int = 1

__prog__       : str = os.path.basename (__file__)
__version__    : str = f'{__prog__} Version {MAJ_VER}.{MIN_VER}'
__copyright__  : str = 'Copyright (c) 2023 Intel Corporation. All rights reserved.\n'
__description__: str = 'Script to create the CAT file for Windows Driver Package.\n'

OS_WIN10_X64: str = '10_X64'

#
# Exit code of this script.
#
EXIT_SUCCESS: int = 0x00

class WindowsDriverCatGenerator (object):
    def __init__ (
        self,
        DriverPkgPath: Union[str, os.PathLike],
        ) -> None:
        """ Class to create the Windows driver update package CAT file.

        Args:
            DriverPkgPath (Union[str, os.PathLike]):
                Path of Windows driver update package.
                (Should be include capsule and INF file)

        Raises:
            None.

        Returns:
            None.
        """
        self.__Inf2CatToolPath : str = WinWdkTool ().Inf2Cat
        self.__DrivePkgPath    : str = AbsPath (DriverPkgPath)

        self.__PreCheck ()

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        CheckPathList: List[str] = [
            self.__DrivePkgPath,
            ]

        CheckDirListExist (CheckPathList)

    def Build (self) -> int:
        """ Call the Windows WDK tools Inf2Cat to build CAT file.

        Note:
            (1) It is caller responsibility to install the WDK
                correctly before calling this function.
            (2) Toolkit should be installed on default path.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                The exit code of the Inf2Cat program.
        """
        IsVerbose: bool = True

        DEBUG (DEBUG_INFO, f'Inf2Cat tool path: {self.__Inf2CatToolPath}')

        CmdList: List[str] = [
            f'{self.__Inf2CatToolPath}',
            f'/driver:{self.__DrivePkgPath}',
            f'/os:{OS_WIN10_X64}',
            '/uselocaltime',
            ]

        if IsVerbose:
            CmdList.append ('/verbose')

        Exitcode, OutputLog, ErrorLog = ExecOsSystemCmd (CmdList)

        if Exitcode == 0x00:
            print (OutputLog)
        else:
            print (ErrorLog)

        return Exitcode

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'WindowsDriverCatGenerator',
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
                 '-i',
                 '-I',
                 '--input',
                 dest     = 'InputPath',
                 help     = 'Path of the Windows driver update package.',
                 required = True,
                 )

        return Parser.parse_args ()

    Args: argparse.Namespace = GetArgs ()

    UpdatePkgPath: str = AbsPath (Args.InputPath)

    DEBUG (DEBUG_INFO, f'-------- Windows Driver CAT Generate Start --------')

    Generator: WindowsDriverCatGenerator = \
        WindowsDriverCatGenerator (
          UpdatePkgPath,
          )
    Generator.Build ()

    DEBUG (DEBUG_INFO, f'--------- Windows Driver CAT Generate End ---------')

    sys.exit (EXIT_SUCCESS)
