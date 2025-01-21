## @file
#  Script to sign Windows driver update package CAT file.
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
#   - https://learn.microsoft.com/en-us/windows/win32/seccrypto/signtool
#
##

import os
import argparse
from typing import List

from CapsuleCommon import *

MAJ_VER: int = 0
MIN_VER: int = 1

__prog__       : str = os.path.basename (__file__)
__version__    : str = f'{__prog__} Version {MAJ_VER}.{MIN_VER}'
__copyright__  : str = 'Copyright (c) 2023 Intel Corporation. All rights reserved.\n'
__description__: str = 'Script to sign the CAT file for Windows Driver Package.\n'

#
# Pre-defined variable
#
SHA256_SIGN: str = 'SHA256'

#
# Exit code of this script.
#
EXIT_SUCCESS: int = 0x00

class WindowsDriverSigner (object):
    def __init__ (
        self,
        CatFilePath: Union[str, os.PathLike],
        PfxFilePath: Union[str, os.PathLike],
        ) -> None:
        """ Class to sign the CAT file.

        Args:
            CatFilePath (Union[str, os.PathLike]):
                The path of CAT to be signed.
            PfxFilePath (Union[str, os.PathLike]):
                The path of PFX file used to sign the CAT file.

        Raises:
            None.

        Returns:
            None.
        """
        self.__CatFilePath : str = AbsPath (CatFilePath)
        self.__PfxFilePath : str = AbsPath (PfxFilePath)

        self.__PreCheck ()

        self.__SignToolPath: str = WinWdkTool ().SignTool

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
            self.__CatFilePath,
            self.__PfxFilePath,
            ]

        CheckFileListExist (CheckPathList)

    def Sign (self) -> int:
        """ Call the Windows WDK tools SignTool to sign CAT file.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                The exit code of the SignTool program.
        """
        CertHash : str  = SHA256_SIGN
        IsVerbose: bool = True

        CmdList: List[str] = [
            f'{self.__SignToolPath}',
            'sign', '/a',
            '/fd' , f'{CertHash}',           # Certificate Hash
            '/f'  , f'{self.__PfxFilePath}', # Sign certificate file
            ]

        if IsVerbose:
            CmdList.append ('/v')

        CmdList.append (f'{self.__CatFilePath}') # CAT file

        ExitCode, _, _ = ExecOsSystemCmd (CmdList)

        return ExitCode

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'WindowsDriverSigner',
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
                 '-c',
                 '-C',
                 '--cat',
                 dest     = 'CatFilePath',
                 help     = 'Path of Windows driver update package CAT file.',
                 required = True,
                 )
        Parser.add_argument (
                 '-p',
                 '-P',
                 '--pfx',
                 dest     = 'PfxFilePath',
                 help     = 'Path of PFX file used to sign the CAT file.',
                 required = True,
                 )

        return Parser.parse_args ()

    Args: argparse.Namespace = GetArgs ()

    CatFilePath: str = AbsPath (Args.CatFilePath)
    PfxFilePath: str = AbsPath (Args.PfxFilePath)

    DEBUG (DEBUG_INFO, f'-------- Windows Driver CAT Sign Start --------')

    Signer: WindowsDriverSigner = \
        WindowsDriverSigner (
          CatFilePath = CatFilePath,
          PfxFilePath = PfxFilePath,
          )
    Signer.Sign ()

    DEBUG (DEBUG_INFO, f'--------- Windows Driver CAT Sign End ---------')

    sys.exit (EXIT_SUCCESS)
