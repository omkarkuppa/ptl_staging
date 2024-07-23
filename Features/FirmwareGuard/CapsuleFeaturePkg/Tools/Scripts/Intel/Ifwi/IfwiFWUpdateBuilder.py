## @file
#  Script to do build the CSME FWUpdate binary from IFWI and FIT tool.
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
import re
import argparse
from typing import Union

from CapsuleCommon import *

MAJ_VER: int = 0
MIN_VER: int = 1

__prog__       : str = os.path.basename (__file__)
__version__    : str = f'{__prog__} Version {MAJ_VER}.{MIN_VER}'
__copyright__  : str = 'Copyright (C) 2023 Intel Corporation.. All rights reserved.\n'
__description__: str = 'Script to build the CSME FWUpdate image from IFWI.\n'

MFIT_FWU_LAYOUT_NAME: str = 'Intel(R) {Platform} {Chipset} - FWUpdate'

DEFAULT_IFWI_INFO_FILE_NAME: str = 'ImageInfo.txt'
DEFAULT_FWU_FILENAME       : str = 'FWUpdate.bin'

KEYWORD_IMAGE_LAYOUT: str = 'Image Layout'
LAYOUT_REGEX        : str = \
    r'^Intel\(R\) (([A-Za-z]+\s)+)[-] ([A-Za-z]+) [-] ([A-Za-z0-9]+)$'

class IfwiFWUpdateBuilder (object):
    def __init__ (
        self,
        IfwiPath     : Union[str, os.PathLike],
        FitToolPath  : Union[str, os.PathLike],
        XmlConfigPath: Union[str, os.PathLike],
        DecomposePath: Union[str, os.PathLike],
        CSEImagePath : Union[str, os.PathLike] = '',
        Platform     : str                     = '',
        ) -> None:
        """ Class to build the FWUpdate image from IFWI via FIT tool.

        Args:
            IfwiPath (Union[str, os.PathLike]):
                The path of IFWI image.
            FitToolPath (Union[str, os.PathLike]):
                The path of FIT tool.
            XmlConfigPath (Union[str, os.PathLike]):
                The path of IFWI XML configuration file.
            DecomposePath (Union[str, os.PathLike]):
                The path of to do the decompose operation. (Workspace)
            CSEImagePath (Union[str, os.PathLike], optional):
                The path of CSE image.
                (If input is Corporate IFWI, this is mandatory)
                Defaults to ''.
            Platform (str, optional):
                Assigned the specific platform name for the FWUpdate
                layout.
                (If not input, would be based on the IFWI layout)
                Defaults to ''.

        Raises:
            None.

        Returns:
            None.
        """
        #
        # File
        #
        self.__IfwiPath     : str = AbsPath (IfwiPath)
        self.__FitToolPath  : str = AbsPath (FitToolPath)
        self.__XmlConfigPath: str = AbsPath (XmlConfigPath)
        self.__CSEImagePath : str = AbsPath (CSEImagePath)

        #
        # Directory
        #
        self.__DecomposePath: str = AbsPath (DecomposePath)

        self.__Platform: str = Platform

        self.__PreCheck ()
        TouchDir (self.__DecomposePath)
        self.__ImageInfoPath: str = self.__GetIfwiInfo ()

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
            self.__IfwiPath,
            self.__FitToolPath,
            self.__XmlConfigPath,
            ]
        CheckFileListExist (CheckPathList)

        #
        # Allow to bypass the CSE image path.
        # Since it is optional for Consumer IFWI.
        #
        CheckPathList = [
            self.__CSEImagePath,
            ]
        CheckFileListExist (CheckPathList, IsBypassEmpty = True)

    def __GetIfwiInfo (self) -> str:
        """ Get the IFWI information via FIT tool.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The absolute path of ImageInfo file.
        """
        Content   : str  = None
        OutputFile: File = None
        OutputPath: str  = JoinPath (
                             self.__DecomposePath,
                             DEFAULT_IFWI_INFO_FILE_NAME,
                             )

        Cmd: List[str] = [
            f'{self.__FitToolPath}', '--imageinfo',
            f'{self.__IfwiPath}',
            ]

        _, Output, _ = ExecOsSystemCmd (
                         Cmd,
                         Cwd    = f'{self.__DecomposePath}',
                         StdOut = STREAM_PIPE,
                         )

        Content    = Output.decode ('utf-8')
        OutputFile = File (FileContent = Content)
        OutputFile.Save (OutputPath)

        return OutputPath

    def __GetLayoutInfo (self) -> Tuple[str, str, str]:
        """ Get the layout information from ImageInfo.

        Note:
            Platform is allowed to input to override.

        Args:
            None.

        Raises:
            ValueError:
                (1) Can not find the keyword in ImageInfo file.
                (2) Find more than one keyword in ImageInfo file.

        Returns:
            Tuple[str, str, str]:
                str:
                    The platform name of the layout.
                str:
                    The chipset name of the layout.
                str:
                    The sku name of the layout.
                    Should be Consumer or Corporate.
        """
        ImageInfoFile: File = File (
                                FilePath    = self.__ImageInfoPath,
                                IsReadLines = True,
                                )

        LayoutInfoLoc: List[int] = ImageInfoFile.Find (KEYWORD_IMAGE_LAYOUT)

        LayoutInfo : List[str] = list ()
        LayoutName : str       = str ()
        SearchGroup: re.Match  = None
        PlatChip   : str       = str ()
        Platform   : str       = str ()
        Chipset    : str       = str ()
        Sku        : str       = str ()

        if len (LayoutInfoLoc) == 0:
            raise ValueError (
                    f'Can not find the layout information.'
                    f' With the keyword {[KEYWORD_IMAGE_LAYOUT]}'
                    )
        elif len (LayoutInfoLoc) > 1:
            raise ValueError (f'Find more than one keyword. [{LayoutInfoLoc}]')

        LayoutInfo = ImageInfoFile.GetLineData (LayoutInfoLoc[0]).split ()
        LayoutName = ' '.join (LayoutInfo[2:])

        #
        # Possible layout format:
        #   (1) Intel(R) XLake S Chipset - Consumer - SPI
        #   (2) Intel(R) XLake S and YLake S Chipsets - Corporate - SPI
        #
        #   Group(1) => XLake S Chipset
        #   Group(2) => Chipset
        #   Group(3) => Consumer
        #   Group(4) => SPI
        #
        #   Note: Need to trim the string.
        #
        SearchGroup = re.search (LAYOUT_REGEX, LayoutName, re.I | re.DOTALL)

        if SearchGroup is None:
            raise ValueError (
                    f'Layout format is not supported! - [{LayoutName}]'
                    )
        elif len (SearchGroup.groups ()) != 4:
            raise ValueError (
                    f'Search group number is not 4 - [{SearchGroup.groups ()}]'
                    )

        PlatChip = str (SearchGroup.group (1)).strip ()
        Chipset  = str (SearchGroup.group (2)).strip ()
        Sku      = str (SearchGroup.group (3)).strip ()

        #
        # Remove the Chipset/Chipsets string.
        # Remove the chipset type string.
        #
        Platform = PlatChip.replace (Chipset, '').strip ()[:-1].strip ()
        Chipset  = PlatChip.replace (Platform, '').strip ()

        if self.__Platform:
            DEBUG (DEBUG_INFO, f'Override the platform into {self.__Platform}')
            Platform = self.__Platform

        DEBUG (DEBUG_TRACE, f'Layout Platform: {Platform}')
        DEBUG (DEBUG_TRACE, f'Layout Chipset : {Chipset}')
        DEBUG (DEBUG_TRACE, f'Layout Sku     : {Sku}')

        return Platform, Chipset, Sku

    def __SaveFwupdateFile (self, Output: Union[str, os.PathLike]) -> None:
        """ Save the FWUpdate image to specific path.

        Note:
            If directory is not exist, would try to touch that place.

        Args:
            Output (Union[str, os.PathLike]):
                The path of FWUpdate image export.

        Raises:
            None.

        Returns:
            None.
        """
        Source: str = JoinPath (self.__DecomposePath, DEFAULT_FWU_FILENAME)
        Target: str = AbsPath (Output)

        TouchDir (os.path.dirname (Target))
        CopyFile (Source, Target)

    def GenCsmeFwupdate (self, Output: Union[str, os.PathLike]) -> None:
        """ Function call to generate the CSME FWUpdate image.

        Args:
            Output (Union[str, os.PathLike]):
                The path of FWUpdate image export.

        Raises:
            ErrorException:
                Corporate IFWI not assign the CSE image path.

        Returns:
            None.
        """
        Platform      : str = str ()
        Chipset       : str = str ()
        Sku           : str = str ()
        FWUpdateLayout: str = str ()

        Platform, Chipset, Sku = self.__GetLayoutInfo ()

        if (Sku == IFWI_CORPORATE) and (not self.__CSEImagePath):
            ErrorException (f'Corporate IFWI require assign CSE image.')

        FWUpdateLayout = MFIT_FWU_LAYOUT_NAME.format (
                                                Platform = Platform,
                                                Chipset  = Chipset,
                                                )

        Cmd: List[str] = [
            f'{self.__FitToolPath}',

            '--me' if self.__CSEImagePath else '',
            f'{self.__CSEImagePath}' if self.__CSEImagePath else f'',

            '--layout',     f'{FWUpdateLayout}',
            '--loadconfig', f'{self.__XmlConfigPath}',
            '--build',      f'{DEFAULT_FWU_FILENAME}',
            ]

        ExecOsSystemCmd (Cmd, Cwd = self.__DecomposePath)

        self.__SaveFwupdateFile (Output)

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
                 '--ifwi',
                 dest     = 'Ifwi',
                 help     = 'The path of IFWI image file.',
                 required = True,
                 )
        Parser.add_argument (
                 '-f',
                 '-F',
                 '--fit',
                 dest     = 'Fit',
                 help     = 'The path of Fit tool.',
                 required = True,
                 )
        Parser.add_argument (
                 '-c',
                 '-C',
                 '--config',
                 dest     = 'Config',
                 help     = 'The IFWI XML configuration file.',
                 required = True,
                 )
        Parser.add_argument (
                 '-cse',
                 '-CSE',
                 '--cse',
                 dest     = 'Cse',
                 help     = 'The path of CSE image file. (Corporate IFWI is mandatory)',
                 default  = '',
                 required = False,
                 )
        Parser.add_argument (
                 '-p',
                 '-P',
                 '--platform',
                 dest     = 'Platform',
                 help     = 'The platform to be override from layout.',
                 default  = '',
                 required = False,
                 )
        Parser.add_argument (
                 '-d',
                 '-D',
                 '--decompose',
                 dest     = 'Decompose',
                 help     = 'The path to decompose the FWUpdate image. (Workspace)',
                 default  = '.',
                 required = False,
                 )
        Parser.add_argument (
                 '-o',
                 '-O',
                 '--output',
                 dest     = 'Output',
                 help     = 'The path of FWUpdate image output.',
                 default  = DEFAULT_FWU_FILENAME,
                 required = False,
                 )

        return Parser.parse_args ()

    Args: argparse.Namespace = GetArgs ()

    IfwiPath     : str = AbsPath (Args.Ifwi)
    FitToolPath  : str = AbsPath (Args.Fit)
    XmlConfigPath: str = AbsPath (Args.Config)
    CseImagePath : str = AbsPath (Args.Cse)
    DecomposePath: str = AbsPath (Args.Decompose)
    OutputPath   : str = AbsPath (Args.Output)
    Platform     : str = Args.Platform

    DEBUG (DEBUG_INFO, f'--------------- Build FWUpdate Start ---------------')

    Builder: IfwiFWUpdateBuilder = \
        IfwiFWUpdateBuilder (
          IfwiPath      = IfwiPath,
          FitToolPath   = FitToolPath,
          XmlConfigPath = XmlConfigPath,
          DecomposePath = DecomposePath,
          CSEImagePath  = CseImagePath,
          Platform      = Platform,
          )

    Builder.GenCsmeFwupdate (OutputPath)

    DEBUG (DEBUG_INFO, f'---------------- Build FWUpdate End ----------------')
