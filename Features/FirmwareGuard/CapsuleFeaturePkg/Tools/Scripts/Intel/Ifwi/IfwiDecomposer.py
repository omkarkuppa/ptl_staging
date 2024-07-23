## @file
#  Script to decompose the firmware binary from IFWI by FIT tool.
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

import glob
import os
import argparse
from typing import List, Union

from CapsuleCommon import *

MAJ_VER: int = 0
MIN_VER: int = 1

__prog__       : str = os.path.basename (__file__)
__version__    : str = f'{__prog__} Version {MAJ_VER}.{MIN_VER}'
__copyright__  : str = 'Copyright (C) 2023 Intel Corporation. All rights reserved.\n'
__description__: str = 'Script to decompose FW image from IFWI via FIT tool.\n'

DEFAULT_IFWI_FILENAME                : str  = 'Ifwi.bin'
DEFAULT_IFWI_CONFIG_XML_FILENAME     : str  = 'Ifwi.xml'
DEFAULT_FIT_TOOL_WORKSPACE_NAME      : Path = Path (DEFAULT_IFWI_FILENAME).stem
DEFAULT_IFWI_DECOMPOSE_FOLDER_NAME   : str  = \
    JoinPath (
      DEFAULT_FIT_TOOL_WORKSPACE_NAME,
      'Decompose',
      )
DEFAULT_IFWI_INTERMEDIATE_FOLDER_NAME: str  = \
    JoinPath (
      DEFAULT_FIT_TOOL_WORKSPACE_NAME,
      'Intermediate',
      )
DEFAULT_FIT_TOOL_FILENAME            : str  = 'FitTool.exe'

#
# Component of IFWI naming and decompose path
#
DEFAULT_BIOS_FILENAME   : str       = 'ClientBios.rom'
BIOS_DECOMPOSE_PATH_LIST: List[str] = [
    f'{DEFAULT_IFWI_INTERMEDIATE_FOLDER_NAME}\BIOS Region.bin',
    ]

DEFAULT_EC_FILENAME   : str       = 'Ec.bin'
EC_DECOMPOSE_PATH_LIST: List[str] = [
    f'{DEFAULT_IFWI_INTERMEDIATE_FOLDER_NAME}\EC Region.bin',
    f'{DEFAULT_IFWI_DECOMPOSE_FOLDER_NAME}\*#EcRegion.bin',
    ]

DEFAULT_CSME_FILENAME   : str       = 'CSERegion.bin'
CSME_DECOMPOSE_PATH_LIST: List[str] = [
    f'{DEFAULT_IFWI_INTERMEDIATE_FOLDER_NAME}\CSE Region.bin',
    ]

DEFAULT_GBE_FILENAME   : str       = 'GbE.bin'
GBE_DECOMPOSE_PATH_LIST: List[str] = [
    f'{DEFAULT_IFWI_INTERMEDIATE_FOLDER_NAME}\GBE Region.bin',
    ]

DEFAULT_ISHPDT_FILENAME   : str       = 'IshPdt.bin'
ISHPDT_DECOMPOSE_PATH_LIST: List[str] = [
    f'{DEFAULT_IFWI_INTERMEDIATE_FOLDER_NAME}\PdtBinary.bin',
    f'{DEFAULT_IFWI_DECOMPOSE_FOLDER_NAME}\*#IshPdtBinary.bin',
    f'{DEFAULT_IFWI_DECOMPOSE_FOLDER_NAME}\*ish_pdt_binary.bin',
    ]

#
# List of exit code
#
EXIT_SUCCESS: int = 0x00
#
# IFWI XML config parser
#
EXIT_VALUE_MISMATCHED   : int = 0x01
EXIT_NODE_NOT_FOUND     : int = 0x02
EXIT_ATTRIBUTE_NOT_FOUND: int = 0x03
EXIT_UNKNOWN            : int = 0xFF

class IfwiDecomposer (object):
    def __init__ (
        self,
        IfwiPath             : Union[str, os.PathLike],
        FitToolPath          : Union[str, os.PathLike],
        IfwiConfigParserPath : Union[str, os.PathLike],
        BiosFileName         : str = DEFAULT_BIOS_FILENAME,
        EcFileName           : str = DEFAULT_EC_FILENAME,
        MeFileName           : str = DEFAULT_CSME_FILENAME,
        GbEFileName          : str = DEFAULT_GBE_FILENAME,
        IshPdtFileName       : str = DEFAULT_ISHPDT_FILENAME,
        IfwiXmlConfigFileName: str = DEFAULT_IFWI_CONFIG_XML_FILENAME,
        DecomposePath        : Union[str, os.PathLike] = '.',
        ExportPath           : Union[str, os.PathLike] = '.',
        ) -> None:
        """ Class to support to decompose the IFWI by FIT tool.

        Args:
            IfwiPath (Union[str, os.PathLike]):
                The path of IFWI image to be decomposed.
            FitToolPath (Union[str, os.PathLike]):
                The FIT tool to support to do the decomposing.
            IfwiConfigParserPath (Union[str, os.PathLike]):
                The path of parser to check the config within IFWI XML
                config file.
            BiosFileName (str, optional):
                The file name of the BIOS component to save.
                Defaults to DEFAULT_BIOS_FILENAME.
            EcFileName (str, optional):
                The file name of the EC component to save.
                Defaults to DEFAULT_EC_FILENAME.
            MeFileName (str, optional):
                The file name of the CSME region component to save.
                Defaults to DEFAULT_CSME_FILENAME.
            GbEFileName (str, optional):
                The file name of the GbE component to save.
                Defaults to DEFAULT_GBE_FILENAME.
            IshPdtFileName (str, optional):
                The file name of the IshPdt component to save.
                Defaults to DEFAULT_ISHPDT_FILENAME.
            IfwiXmlConfigFileName (str, optional):
                The file name of the IFWI XML config file to save.
                Defaults to DEFAULT_IFWI_CONFIG_XML_FILENAME.
            DecomposePath (Union[str, os.PathLike], optional):
                The path of IFWI to be decomposed.(Workspace)
                Defaults to '.'.
            ExportPath (Union[str, os.PathLike], optional):
                The path of firmware image to be saved.
                Defaults to '.'.

        Raise:
            None.

        Returns:
            None.
        """
        self.__IfwiPath            : str = AbsPath (IfwiPath)
        self.__FitToolPath         : str = AbsPath (FitToolPath)
        self.__IfwiConfigParserPath: str = AbsPath (IfwiConfigParserPath)
        self.__DecomposePath       : str = AbsPath (DecomposePath)
        self.__ExportPath          : str = AbsPath (ExportPath)

        self.__PreCheck ()

        #
        # Path to export the image file
        #
        self.__BiosPath  : str = JoinPath (self.__ExportPath, BiosFileName)
        self.__EcPath    : str = JoinPath (self.__ExportPath, EcFileName)
        self.__MePath    : str = JoinPath (self.__ExportPath, MeFileName)
        self.__GbEPath   : str = JoinPath (self.__ExportPath, GbEFileName)
        self.__IshPdtPath: str = JoinPath (self.__ExportPath, IshPdtFileName)

        self.__IfwiConfigPath: str = \
            JoinPath (self.__ExportPath, IfwiXmlConfigFileName)

        self.__IfwiPath, self.__FitToolPath = self.__PreWork ()

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
            self.__IfwiConfigParserPath,
            ]
        CheckFileListExist (CheckPathList)

    def __PreWork (self) -> None:
        """ Preliminary work before to execute.

        Note:
            (1) Touch the necessary directory.
            (2) Copy the necessary file.

        Args:
            None.

        Raises:
            ValueError:
                IFWI XML config file path is not a file.

        Returns:
            None.
        """
        IfwiImagePath: str = \
            JoinPath (self.__ExportPath, DEFAULT_IFWI_FILENAME)
        FitToolPath  : str = \
            JoinPath (self.__ExportPath, DEFAULT_FIT_TOOL_FILENAME)

        if IsPathExist (self.__IfwiConfigPath):
            if IsDir (self.__IfwiConfigPath):
                raise ValueError (f'IFWI config path should be file.')
            elif IsFile (self.__IfwiConfigPath):
                RemoveFile (self.__IfwiConfigPath)

        #
        # Create the working directory
        #
        TouchDir (self.__DecomposePath)
        TouchDir (self.__ExportPath)

        #
        # Copy the IFWI and FIT tool to export folder
        #
        CopyFile (self.__IfwiPath, IfwiImagePath)
        CopyFile (self.__FitToolPath, FitToolPath)

        return IfwiImagePath, FitToolPath

    def __ExtractByMFit (self) -> str:
        """ Function call the FIT tool command to decompose IFWI. (mFit)

        Args:
            None.

        Raises:
            ValueError:
                Failed to decompose the IFWI.

        Returns:
            str:
                Path of IFWI XML config file to be saved.
        """
        ConfigFilePath: str = \
            JoinPath (self.__DecomposePath, DEFAULT_IFWI_CONFIG_XML_FILENAME)

        Cmd: List[str] = [
            f'{self.__FitToolPath}',
            '--decompose', f'{self.__IfwiPath}',
            '--saveconfig',f'{ConfigFilePath}',
            ]

        ExitCode, _, _ = ExecOsSystemCmd (Cmd, Cwd = self.__DecomposePath)

        if ExitCode != EXIT_SUCCESS:
            raise ValueError (f'Failed to decompose the IFWI.')

        return ConfigFilePath

    def __FindAndCopyImage (
        self,
        FwPathList  : List[str],
        ExportFwPath: str,
        ) -> bool:
        """ Find the decomposed firmware image and copy.

        Args:
            FwPathList (List[str]):
                List of path to find the firmware component.
                (Search by order, would break to search once get found)
            ExportFwPath (str):
                The file of path to copy this firmware image.

        Raises:
            None.

        Returns:
            bool:
                True  - Found the decomposed firmware image.
                False - Not found the decomposed firmware image.
        """
        IsFound   : bool = False
        SourcePath: str  = str ()

        for FwPath in FwPathList:
            SourcePath = JoinPath (self.__DecomposePath, FwPath)

            DEBUG (DEBUG_TRACE, f'Try to find the path: {SourcePath}')

            for File in glob.glob (SourcePath):
                DEBUG (DEBUG_INFO, f'Found the file: {File}')
                DEBUG (DEBUG_INFO, f'Copy to output: {ExportFwPath}')
                shutil.copy2 (File, ExportFwPath)
                IsFound = True
                break

            if IsFound:
                break

        return IsFound

    def __CheckIfwiConfig (
        self,
        NodeList   : List[str],
        Attribute  : str,
        ExpectValue: str,
        ) -> bool:
        """ Check the IFWI configuration is matched the expected value.

        Args:
            NodeList (List[str]):
                The XML node to be get.
                (Order by top to bottom to query the XML tree)
            Attribute (str):
                The attribute of this node to check.
            ExpectValue (str):
                The expected value to be compared. (Case-sensitive)

        Raises:
            ValueError:
                (1) The node is not found within the XML file.
                (2) The attribute is not found within this node.
                (3) Unknown error code get return.

        Returns:
            bool:
                True  - IFWI config is matched the expected value.
                False - IFWI config is not matched the expected value.
        """
        Result: bool = False

        Cmd: List[str] = [
            f'{self.__IfwiConfigParserPath}',
            '-i',  f'{self.__IfwiConfigPath}',
            '-a',  f'{Attribute}',
            '-ev', f'{ExpectValue}',
            ]
        Cmd = Cmd + ['-n'] + NodeList

        ExitCode, _, _ = ExecPythonCmd (Cmd, IsException = False)

        if ExitCode == EXIT_SUCCESS:
            Result = True
        elif ExitCode == EXIT_VALUE_MISMATCHED:
            Result = False
            DEBUG (DEBUG_WARN, f'Expect value is not matched in IFWI config.')
        elif ExitCode == EXIT_NODE_NOT_FOUND:
            raise ValueError (f'Node is not found in XML file.')
        elif ExitCode == EXIT_ATTRIBUTE_NOT_FOUND:
            raise ValueError (f'Attribute is not found in this node.')
        else:
            raise ValueError (f'Unknown error get raise exit code: {ExitCode}')

        return Result

    def Decompose (self) -> None:
        """ Function to do the full decompose operation.

        Note:
            (1) Supported to decompose from below tools.
                    - mFit
            (2) If GbE is disabled in IFWI, would not decompose.
            (3) If IshPdt is not supported in IFWI, would not decompose.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        NodeList      : List[str] = list ()
        ConfigFilePath: str       = self.__ExtractByMFit ()
        CopyFile (ConfigFilePath, self.__IfwiConfigPath)

        self.__FindAndCopyImage (BIOS_DECOMPOSE_PATH_LIST, self.__BiosPath)
        self.__FindAndCopyImage (EC_DECOMPOSE_PATH_LIST,   self.__EcPath  )
        self.__FindAndCopyImage (CSME_DECOMPOSE_PATH_LIST, self.__MePath  )

        #
        # GbE
        #
        NodeList = [
            f'FlashLayout',
            f'GbeRegion',
            f'Enabled',
            ]
        if self.__CheckIfwiConfig (NodeList, 'value', 'Enabled'):
            self.__FindAndCopyImage (
                   GBE_DECOMPOSE_PATH_LIST,
                   self.__GbEPath
                   )
        else:
            DEBUG (DEBUG_INFO, f'GbE is disabled within this IFWI.')

        #
        # IshPdt
        #
        NodeList = [
            f'IntegratedSensorHub',
            f'IntegratedSensorHub',
            f'IshSupported',
            ]
        if self.__CheckIfwiConfig (NodeList, 'value', 'Yes'):
            self.__FindAndCopyImage (
                   ISHPDT_DECOMPOSE_PATH_LIST,
                   self.__IshPdtPath,
                   )
        else:
            DEBUG (DEBUG_INFO, f'IshPdt is disabled within this IFWI.')

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
                 dest     = 'FitTool',
                 help     = 'The path of FIT tool.',
                 required = True,
                 )
        Parser.add_argument (
                 '-c',
                 '-C',
                 '--config-parser',
                 dest     = 'ConfigParser',
                 help     = 'The path of IFWI config parser tool.',
                 required = True,
                 )
        Parser.add_argument (
                 '-d',
                 '-D',
                 '--decompose',
                 dest     = 'DecomposeDir',
                 help     = 'The path to decompose the IFWI.(Workspace)',
                 default  = '.',
                 required = False,
                 )
        Parser.add_argument (
                 '-e',
                 '-E',
                 '--export',
                 dest     = 'ExportDir',
                 help     = 'The path to export FW image.',
                 default  = '.',
                 required = False,
                 )
        Parser.add_argument (
                 '-bios',
                 '-BIOS',
                 '--bios',
                 dest     = 'BiosFileName',
                 help     = 'The filename of the BIOS component.',
                 default  = DEFAULT_BIOS_FILENAME,
                 required = False,
                 )
        Parser.add_argument (
                 '-ec',
                 '-EC',
                 '--ec',
                 dest     = 'EcFileName',
                 help     = 'The filename of the EC component.',
                 default  = DEFAULT_EC_FILENAME,
                 required = False,
                 )
        Parser.add_argument (
                 '-me',
                 '-ME',
                 '--me',
                 dest     = 'MeFileName',
                 help     = 'The filename of the ME region component.',
                 default  = DEFAULT_CSME_FILENAME,
                 required = False,
                 )
        Parser.add_argument (
                 '-gbe',
                 '-GBE',
                 '--gbe',
                 dest     = 'GbEFileName',
                 help     = 'The filename of the GbE component.',
                 default  = DEFAULT_GBE_FILENAME,
                 required = False,
                 )
        Parser.add_argument (
                 '-pdt',
                 '-PDT',
                 '--pdt',
                 dest     = 'IshPdtFileName',
                 help     = 'The filename of the PDT component.',
                 default  = DEFAULT_ISHPDT_FILENAME,
                 required = False,
                 )
        Parser.add_argument (
                 '-config',
                 '-CONFIG',
                 '--config',
                 dest     = 'IfwiXmlConfigFileName',
                 help     = 'The filename of the IFWI XML config file.',
                 default  = DEFAULT_IFWI_CONFIG_XML_FILENAME,
                 required = False,
                 )

        return Parser.parse_args ()

    Args: argparse.Namespace = GetArgs ()

    IfwiPath        : str = Args.Ifwi
    FitToolPath     : str = Args.FitTool
    ConfigParserPath: str = Args.ConfigParser
    DecomposeDirPath: str = Args.DecomposeDir
    ExportDirPath   : str = Args.ExportDir

    BiosFileName         : str = Args.BiosFileName
    EcFileName           : str = Args.EcFileName
    MeFileName           : str = Args.MeFileName
    GbEFileName          : str = Args.GbEFileName
    IshPdtFileName       : str = Args.IshPdtFileName
    IfwiXmlConfigFileName: str = Args.IfwiXmlConfigFileName

    DEBUG (DEBUG_INFO, f'--------------- Decompose IFWI Start ---------------')

    Decomposer: IfwiDecomposer = \
        IfwiDecomposer (
          IfwiPath              = IfwiPath,
          FitToolPath           = FitToolPath,
          IfwiConfigParserPath  = ConfigParserPath,
          BiosFileName          = BiosFileName,
          EcFileName            = EcFileName,
          MeFileName            = MeFileName,
          GbEFileName           = GbEFileName,
          IshPdtFileName        = IshPdtFileName,
          IfwiXmlConfigFileName = IfwiXmlConfigFileName,
          DecomposePath         = DecomposeDirPath,
          ExportPath            = ExportDirPath,
          )

    Decomposer.Decompose ()

    DEBUG (DEBUG_INFO, f'---------------- Decompose IFWI End ----------------')
