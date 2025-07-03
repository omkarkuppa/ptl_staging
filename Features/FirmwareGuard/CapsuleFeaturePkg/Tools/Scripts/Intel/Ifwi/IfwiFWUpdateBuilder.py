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
import argparse
from typing import Union

from CapsuleCommon import *

MAJ_VER: int = 0
MIN_VER: int = 3

__prog__       : str = os.path.basename (__file__)
__version__    : str = f'{__prog__} Version {MAJ_VER}.{MIN_VER}'
__copyright__  : str = 'Copyright (C) 2023 Intel Corporation.. All rights reserved.\n'
__description__: str = 'Script to build the CSME FWUpdate image from IFWI.\n'

DEFAULT_IFWI_INFO_FILE_NAME: str = 'ImageInfo.txt'
DEFAULT_FWU_FILENAME       : str = 'FWUpdate.bin'

KEYWORD_IMAGE_LAYOUT: str = 'Image Layout'
KEYWORD_FWU_LAYOUT  : str = 'FWUpdate'

class _LayoutInfo (object):
    def __init__ (self, LayoutName: str) -> None:
        """ Class to represent the image layout information.

        Args:
            LayoutName (str):
                The layout name of the IFWI image.

        Raises:
            None.

        Returns:
            None.
        """
        self.__LayoutName   : str = LayoutName
        self.__Sku          : str = self.__GetIfwiSku ()
        self.__FwuLayoutName: str = self.__GetFwuLayoutName ()

    @property
    def LayoutName (self) -> str:
        """ Get the layout name of the IFWI image.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The layout name of IFWI image.
        """
        return self.__LayoutName

    @property
    def Sku (self) -> str:
        """ Get the SKU type of the IFWI image.

        Args:
            None.

        Raises:
            None

        Returns:
            str:
                The SKU type of the IFWI image.
                (Should be IFWI_CONSUMER or IFWI_CORPORATE.)
        """
        return self.__Sku

    @property
    def FwuLayoutName (self) -> str:
        """ Get the layout name for FWUpdate image.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The layout name for FWUpdate image.
        """
        return self.__FwuLayoutName

    def __GetIfwiSku (self) -> str:
        """ Check the SKU type of the IFWI image.

        Args:
            None.

        Raises:
            ValueError:
                Unrecognized the SKU type of the IFWI image.

        Returns:
            str:
                The SKU type of the IFWI image.
                (Should be IFWI_CONSUMER or IFWI_CORPORATE.)
        """
        IsFound       : bool      = False
        LayoutName    : str       = self.__LayoutName.upper ()
        LayoutNameList: List[str] = LayoutName.split ()

        #
        # Check if the layout is for Consumer SKU.
        #
        IsFound, _ = SearchValueInList (
                       InputList    = LayoutNameList,
                       TargetValue  = IFWI_CONSUMER,
                       IsIgnoreCase = True,
                       )
        if IsFound:
            return IFWI_CONSUMER

        #
        # Check if the layout is for Corporate SKU.
        #
        IsFound, _ = SearchValueInList (
                       InputList    = LayoutNameList,
                       TargetValue  = IFWI_CONSUMER,
                       IsIgnoreCase = True,
                       )
        if IsFound:
            return IFWI_CORPORATE

        raise ValueError (
                f'Unknown IFWI SKU type: [{self.__LayoutName}]'
                )

    def __GetFwuLayoutName (self) -> str:
        """ Retrieve the layout name for FWUpdate image.

        Note:
            This function would support below layout name format:
                (1) Intel(R) XLake S Chipset - Consumer - SPI
                    Intel(R) XLake S Chipset - Corporate - SPI - B0
                    -> Intel(R) XLake S Chipset - FWUpdate

                (2) Intel(R) XLake P and YLake P Chipsets - Consumer - SPI
                    Intel(R) XLake P and YLake P Chipsets - Corporate - SPI
                    -> Intel(R) XLake P and YLake P Chipsets - FWUpdate

                (3) XLake Desktop Chipset - Consumer - SPI
                    XLake Desktop Chipset - Corporate - SPI - RDL 0.8
                    -> XLake Desktop Chipset - FWUpdate

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The layout name for FWUpdate image.
        """
        IfwiLayoutNameList: List[str] = self.__LayoutName.split ()
        FwuLayoutNameList : List[str] = list ()
        Value             : str       = None
        IsMatched         : bool      = False
        FwuLayoutName     : str       = None

        for Value in IfwiLayoutNameList:
            IsMatched = CheckStringMatch (Input1 = Value, Input2 = self.Sku)
            if not IsMatched:
                FwuLayoutNameList.append (Value)
                continue

            FwuLayoutNameList.append (KEYWORD_FWU_LAYOUT)
            break

        FwuLayoutName = ' '.join (FwuLayoutNameList)

        DEBUG (DEBUG_INFO, f'FWUpdate Layout Name: {FwuLayoutName}')

        return FwuLayoutName

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

    def __GetLayoutInfo (self) -> _LayoutInfo:
        """ Get the layout information from ImageInfo file.

        Args:
            None.

        Raises:
            ValueError:
                (1) Can not find the keyword in ImageInfo file.
                (2) Find more than one keyword in ImageInfo file.

        Returns:
            _LayoutInfo:
                Decoded layout information deliver by _LayoutInfo type.
        """
        ImageInfoFile      : File        = None
        LayoutNameLoc      : List[int]   = None
        LayoutNameEntryList: List[str]   = None
        LayoutInfo         : _LayoutInfo = None

        ImageInfoFile = File (
                          FilePath    = self.__ImageInfoPath,
                          IsReadLines = True,
                          )

        LayoutNameLoc = ImageInfoFile.Find (KEYWORD_IMAGE_LAYOUT)
        if len (LayoutNameLoc) == 0:
            raise ValueError (
                    f'Can not find the image layout name.'
                    f' With the keyword {[KEYWORD_IMAGE_LAYOUT]}'
                    )
        elif len (LayoutNameLoc) > 1:
            raise ValueError (f'Find more than one keyword. {LayoutNameLoc}')

        LayoutNameEntryList = ImageInfoFile.GetLineData (LayoutNameLoc[0]).split ()
        LayoutName          = ' '.join (LayoutNameEntryList[2:])

        DEBUG (DEBUG_TRACE, f'Image Layout Name: {LayoutName}')

        LayoutInfo = _LayoutInfo (LayoutName = LayoutName)

        return LayoutInfo

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
        LayoutInfo: _LayoutInfo = self.__GetLayoutInfo ()

        if (LayoutInfo.Sku == IFWI_CORPORATE) and (not self.__CSEImagePath):
            ErrorException (f'Corporate IFWI require assign CSE image.')

        FWUpdateLayout = LayoutInfo.FwuLayoutName

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
