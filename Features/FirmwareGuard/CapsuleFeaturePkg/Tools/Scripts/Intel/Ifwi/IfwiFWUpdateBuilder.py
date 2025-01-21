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
from abc import ABCMeta, abstractmethod
from typing import Union

from CapsuleCommon import *

MAJ_VER: int = 0
MIN_VER: int = 2

__prog__       : str = os.path.basename (__file__)
__version__    : str = f'{__prog__} Version {MAJ_VER}.{MIN_VER}'
__copyright__  : str = 'Copyright (C) 2023 Intel Corporation.. All rights reserved.\n'
__description__: str = 'Script to build the CSME FWUpdate image from IFWI.\n'

DEFAULT_IFWI_INFO_FILE_NAME: str = 'ImageInfo.txt'
DEFAULT_FWU_FILENAME       : str = 'FWUpdate.bin'

KEYWORD_IMAGE_LAYOUT: str = 'Image Layout'

class _LayoutTypeDecoder (metaclass = ABCMeta):
    def __init__ (self, LayoutName: str, Regex: str):
        """ Class to decode the layout information.

        Args:
            LayoutName (str):
                Layout name to be decoded.
            Regex (str):
                Regular expression for the layout name.

        Raises:
            None.

        Returns
            None.
        """
        self.__LayoutName: str = LayoutName
        self.__Regex     : str = Regex

    @property
    def SearchGroup (self) -> Union[None, re.Match]:
        """ Return the groups information based on REGEX.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, re.Match]:
                Match groups based on REGEX.
                If failed to decode, return as None.
        """
        SearchGroup: re.Match  = None

        SearchGroup = re.search (
                           self.__Regex,
                           self.__LayoutName,
                           re.I | re.DOTALL,
                           )

        return SearchGroup

    @property
    def Valid (self) -> bool:
        """ Return the validity of the input layout name.

        Args:
            None.

        Raises:
            None.

        Returns:
            bool:
                False - Input layout name is not valid.
                True  - Input layout name is valid.
        """
        if (self.SearchGroup is None):
            return False

        return True

    @property
    @abstractmethod
    def Platform (self) -> Union[None, str]:
        """ Return the content of the platform segment.

        Required:
            Yes.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, str]:
                Content of the platform segment.
                If failed to decode, return as None.
        """
        return NotImplementedError

    @Platform.setter
    @abstractmethod
    def Platform (self, Platform: str) -> None:
        """ Set the content the platform segment.

        Required:
            Yes.

        Args:
            Platform (str):
                Customized content of the platform segment to be set.

        Raises:
            None.

        Returns:
            None.
        """
        return NotImplementedError

    @property
    @abstractmethod
    def Chipset (self) -> Union[None, str]:
        """ Return the content of the chipset segment.

        Required:
            Yes.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, str]:
                Content of the chipset segment
                If failed to decode, return as None.
        """
        return NotImplementedError

    @property
    @abstractmethod
    def Sku (self) -> Union[None, str]:
        """ Return the content of the SKU segment.

        Required:
            Yes.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, str]:
                Content of the SKU segment
                If failed to decode, return as None.
        """
        return NotImplementedError

    @property
    @abstractmethod
    def FwuLayoutName (self) -> Union[None, str]:
        """ Return the layout name for the FWUpdate image.

        Required:
            Yes.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, str]:
                Layout name for the FWUpdate image.
                If failed to decode, return as None.
        """
        return NotImplementedError

class _LayoutType1 (_LayoutTypeDecoder):
    IFWI_LAYOUT_NAME_REGEX: str = \
        r'^Intel\(R\) (([A-Za-z]+\s)+)[-] ([A-Za-z]+) [-] ([A-Za-z0-9]+)$'
    MFIT_FWU_LAYOUT_NAME  : str = \
        'Intel(R) {Platform} {Chipset} - FWUpdate'

    def __init__ (self, LayoutName: str) -> None:
        """ Class to decode the layout with Type 1 format.

        Notes:
            This layout type supported below format.
                (1) Intel(R) XLake S Chipset - Consumer - SPI
                (2) Intel(R) XLake S and YLake S Chipsets - Corporate - SPI

        Args:
            LayoutName (str):
                Layout name to be decoded.

        Raises:
            None.

        Returns:
            None.
        """
        super ().__init__ (LayoutName, _LayoutType1.IFWI_LAYOUT_NAME_REGEX)

        self.__SearchGroup: re.Match = super ().SearchGroup

        self.__Platform: Union[None, str] = None
        self.__Chipset : Union[None, str] = None
        self.__Sku     : Union[None, str] = None

        self.__InitialVariable ()

    @property
    def Platform (self) -> Union[None, str]:
        """ Return the content of the platform segment.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, str]:
                Content of the platform segment.
                If failed to decode, return as None.
        """
        if not self.Valid:
            return None

        return self.__Platform

    @Platform.setter
    def Platform (self, Platform: str) -> None:
        """ Set the content the platform segment.

        Args:
            Platform (str):
                Customized content of the platform segment to be set.

        Raises:
            TypeError:
                Platform is not str type.

        Returns:
            None.
        """
        if not isinstance (Platform, str):
            raise TypeError (f'Platform shall be str type.')

        if Platform:
            DEBUG (DEBUG_INFO, f'Override the platform into {self.__Platform}')
            self.__Platform = Platform

    @property
    def Chipset (self) -> Union[None, str]:
        """ Return the content of the chipset segment.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, str]:
                Content of the chipset segment
                If failed to decode, return as None.
        """
        if not self.Valid:
            return None

        return self.__Chipset

    @property
    def Sku (self) -> Union[None, str]:
        """ Return the content of the SKU segment.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, str]:
                Content of the SKU segment
                If failed to decode, return as None.
        """
        if not self.Valid:
            return None

        return self.__Sku

    @property
    def FwuLayoutName (self) -> Union[None, str]:
        """ Return the layout name for the FWUpdate image.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, str]:
                Layout name for the FWUpdate image.
                If failed to decode, return as None.
        """
        LayoutFormat: str = _LayoutType1.MFIT_FWU_LAYOUT_NAME

        if (not self.Valid):
            return None

        return LayoutFormat.format (
                              Platform = self.Platform,
                              Chipset  = self.Chipset,
                              )

    def __Decode (self) ->Tuple[str, str, str]:
        """ Decode the layout name with REGEX.

        Notes:
            (1) This layout type decode below groups
                Group(1) => XLake S Chipset
                Group(2) => Chipset
                Group(3) => Consumer
                Group(4) => SPI
            (2) Need to trim the string.

        Args:
            None.

        Raises:
            ValueError:
                Unexpected number of groups decoded.

        Returns:
            Tuple[str, str, str]:
                str:
                    Content of the platform segment.
                str:
                    Content of the chipset segment.
                str:
                    Content of the SKU segment.
        """
        PlatChip   : Union[None, str] = None
        Platform   : Union[None, str] = None
        Chipset    : Union[None, str] = None
        Sku        : Union[None, str] = None

        if len (self.__SearchGroup.groups ()) != 4:
            raise ValueError (
                    f'Search group number is not 4 - [{self.__SearchGroup.groups ()}]'
                    )

        PlatChip = str (self.__SearchGroup.group (1)).strip ()
        Chipset  = str (self.__SearchGroup.group (2)).strip ()
        Sku      = str (self.__SearchGroup.group (3)).strip ()

        #
        # Remove the Chipset/Chipsets string.
        # Remove the chipset type string.
        #
        Platform = PlatChip.replace (Chipset, '').strip ()[:-1].strip ()
        Chipset  = PlatChip.replace (Platform, '').strip ()

        return Platform, Chipset, Sku

    def __InitialVariable (self) -> None:
        """ Initial the variables for this object.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        if (not self.Valid):
            return

        self.__Platform, self.__Chipset, self.__Sku = self.__Decode ()

class _LayoutType2 (_LayoutTypeDecoder):
    IFWI_LAYOUT_NAME_REGEX: str = \
        r'^(([A-Za-z]+\s)+)[-] ([A-Za-z]+) [-] ([A-Za-z0-9]+)$'
    MFIT_FWU_LAYOUT_NAME  : str = \
        '{Platform} {Chipset} - FWUpdate'

    def __init__ (self, LayoutName: str) -> None:
        """ Class to decode the layout with Type 2 format.

        Notes:
            This layout type supported below format.
                (1) XLake S Chipset - Consumer - SPI
                (2) XLake S and YLake S Chipsets - Corporate - SPI

        Args:
            LayoutName (str):
                Layout name to be decoded.

        Raises:
            None.

        Returns:
            None.
        """
        super ().__init__ (LayoutName, _LayoutType2.IFWI_LAYOUT_NAME_REGEX)

        self.__SearchGroup: re.Match = super ().SearchGroup

        self.__Platform: Union[None, str] = None
        self.__Chipset : Union[None, str] = None
        self.__Sku     : Union[None, str] = None

        self.__InitialVariable ()

    @property
    def Platform (self) -> Union[None, str]:
        """ Return the content of the platform segment.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, str]:
                Content of the platform segment.
                If failed to decode, return as None.
        """
        if not self.Valid:
            return None

        return self.__Platform

    @Platform.setter
    def Platform (self, Platform: str) -> None:
        if Platform:
            DEBUG (DEBUG_INFO, f'Override the platform into {self.__Platform}')
            self.__Platform = Platform

    @property
    def Chipset (self) -> Union[None, str]:
        """ Return the content of the chipset segment.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, str]:
                Content of the chipset segment
                If failed to decode, return as None.
        """
        if not self.Valid:
            return None

        return self.__Chipset

    @property
    def Sku (self) -> Union[None, str]:
        """ Return the content of the SKU segment.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, str]:
                Content of the SKU segment
                If failed to decode, return as None.
        """
        if not self.Valid:
            return None

        return self.__Sku

    @property
    def FwuLayoutName (self) -> Union[None, str]:
        """ Return the layout name for the FWUpdate image.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, str]:
                Layout name for the FWUpdate image.
                If failed to decode, return as None.
        """
        LayoutFormat: str = _LayoutType2.MFIT_FWU_LAYOUT_NAME

        if (not self.Valid):
            return None

        return LayoutFormat.format (
                              Platform = self.Platform,
                              Chipset  = self.Chipset,
                              )

    def __Decode (self) ->Tuple[str, str, str]:
        """ Decode the layout name with REGEX.

        Notes:
            (1) This layout type decode below groups
                Group(1) => XLake S Chipset
                Group(2) => Chipset
                Group(3) => Consumer
                Group(4) => SPI
            (2) Need to trim the string.

        Args:
            None.

        Raises:
            ValueError:
                Unexpected number of groups decoded.

        Returns:
            Tuple[str, str, str]:
                str:
                    Content of the platform segment.
                str:
                    Content of the chipset segment.
                str:
                    Content of the SKU segment.
        """
        PlatChip   : Union[None, str] = None
        Platform   : Union[None, str] = None
        Chipset    : Union[None, str] = None
        Sku        : Union[None, str] = None

        if len (self.__SearchGroup.groups ()) != 4:
            raise ValueError (
                    f'Search group number is not 4 - [{self.__SearchGroup.groups ()}]'
                    )

        PlatChip = str (self.__SearchGroup.group (1)).strip ()
        Chipset  = str (self.__SearchGroup.group (2)).strip ()
        Sku      = str (self.__SearchGroup.group (3)).strip ()

        #
        # Remove the Chipset/Chipsets string.
        # Remove the chipset type string.
        #
        Platform = PlatChip.replace (Chipset, '').strip ()[:-1].strip ()
        Chipset  = PlatChip.replace (Platform, '').strip ()

        return Platform, Chipset, Sku

    def __InitialVariable (self) -> None:
        """ Initial the variables for this object.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        if (not self.Valid):
            return

        self.__Platform, self.__Chipset, self.__Sku = self.__Decode ()

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

    def __GetLayoutType (self, LayoutName: str) -> _LayoutTypeDecoder:
        """ Get the type of layout name.

        Args:
            LayoutName (str):
                Layout name to be decoded.

        Raises:
            ErrorException:
                Failed to decode the type of layout.

        Returns:
            _LayoutTypeDecoder:
                Decoded layout information deliver by _LayoutTypeDecoder type.
        """
        LayoutType1: _LayoutType1 = _LayoutType1 (LayoutName = LayoutName)
        LayoutType2: _LayoutType2 = _LayoutType2 (LayoutName = LayoutName)

        if (LayoutType1.Valid):
            return LayoutType1
        elif (LayoutType2.Valid):
            return LayoutType2
        else:
            raise ErrorException (f'Failed to decode the IFWI layout - {LayoutName}')

    def __GetLayoutInfo (self) -> _LayoutTypeDecoder:
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
            _LayoutTypeDecoder:
                Decoded layout information deliver by _LayoutTypeDecoder type.
        """
        ImageInfoFile: File = File (
                                FilePath    = self.__ImageInfoPath,
                                IsReadLines = True,
                                )

        LayoutInfoLoc: List[int] = ImageInfoFile.Find (KEYWORD_IMAGE_LAYOUT)

        LayoutInfo: List[str]          = list ()
        LayoutName: str                = str ()
        LayoutType: _LayoutTypeDecoder = None

        if len (LayoutInfoLoc) == 0:
            raise ValueError (
                    f'Can not find the layout information.'
                    f' With the keyword {[KEYWORD_IMAGE_LAYOUT]}'
                    )
        elif len (LayoutInfoLoc) > 1:
            raise ValueError (f'Find more than one keyword. [{LayoutInfoLoc}]')

        LayoutInfo = ImageInfoFile.GetLineData (LayoutInfoLoc[0]).split ()
        LayoutName = ' '.join (LayoutInfo[2:])

        LayoutType = self.__GetLayoutType (LayoutName)
        LayoutType.Platform = self.__Platform

        DEBUG (DEBUG_TRACE, f'Layout Platform: {LayoutType.Platform}')
        DEBUG (DEBUG_TRACE, f'Layout Chipset : {LayoutType.Chipset}')
        DEBUG (DEBUG_TRACE, f'Layout Sku     : {LayoutType.Sku}')

        return LayoutType

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
        LayoutType    : _LayoutTypeDecoder = None
        FWUpdateLayout: str                = str ()

        LayoutType = self.__GetLayoutInfo ()

        if (LayoutType.Sku == IFWI_CORPORATE) and (not self.__CSEImagePath):
            ErrorException (f'Corporate IFWI require assign CSE image.')

        FWUpdateLayout = LayoutType.FwuLayoutName

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
