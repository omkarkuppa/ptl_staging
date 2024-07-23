## @file
#  Generate FMP signed Capsule for PantherLake platform.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2024 Intel Corporation.
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

import argparse
import copy
import os
import sys
from typing import Dict

#
# Definition for platform full name and platform SKU.
#
PLATFORM_FULL_NAME: str = 'PantherLake'

PTL_MOBILE_PLAT_NAME_STR: str = 'PTLP'
PTL_MOBILE_PREFIX_STR   : str = 'PTL_P'

PLATFORM_PREFIX_MAPPING_TABLE: Dict [str, str] = {
    PTL_MOBILE_PLAT_NAME_STR: PTL_MOBILE_PREFIX_STR,
    }

#
# Definition for path. (Platform specific.)
#
PLAT_BOARD_PKG_PATH : str = os.path.dirname (__file__)
WORKSPACE_INTEL_PATH: str = os.path.dirname (PLAT_BOARD_PKG_PATH)
CAP_FEATURE_PKG     : str = \
    os.path.join (
              WORKSPACE_INTEL_PATH,
              'Features',
              'FirmwareGuard',
              'CapsuleFeaturePkg',
              )
CAP_BRD_FEATURE_SUPPORT_PATH: str = \
    os.path.join (
              PLAT_BOARD_PKG_PATH,
              'Features',
              'CapsuleUpdate',
              )

#
# Definition for path.
#
WORKSPACE       : str = os.path.dirname (WORKSPACE_INTEL_PATH)
TOOLKIT_PATH    : str = os.path.join (CAP_FEATURE_PKG, 'Tools')
CAPSULE_CFG_PATH: str = os.path.join (CAP_BRD_FEATURE_SUPPORT_PATH, 'Config')

#
# Import the CapsuleCommon package.
#
sys.path.append (TOOLKIT_PATH)
try:
    from CapsuleCommon import *
except:
    raise ImportError (f'Failed to import the CapsuleCommon package.')

#
# Definition for postfix.
#
EMBEDDED_POSTFIX: str = '_Embedded'

#
# Definition of reserved commands.
#
CLEAN_COMMAND: str = 'cln'

#
# Exit code of this script.
#
EXIT_SUCCESS: int = 0x00

class _PlatformHook (object):
    BOARD_BUILD_CFG_PATH: str = \
        JoinPath (PLAT_BOARD_PKG_PATH, 'BoardPkgBuildConfig.yaml')

    WORKSPACE_BINARIES : str = \
        JoinPath (WORKSPACE,            'Binaries')
    WORKSPACE_CORE     : str = \
        JoinPath (WORKSPACE,            'Edk2')
    SILICON_PKG_PATH   : str = \
        JoinPath (WORKSPACE_INTEL_PATH, 'OneSiliconPkg')
    OPEN_BOARD_PKG_PATH: str = \
        JoinPath (WORKSPACE_INTEL_PATH, f'{PLATFORM_FULL_NAME}OpenBoardPkg')
    PLAT_BIN_PKG_PATH  : str = \
        JoinPath (WORKSPACE_BINARIES,   f'{PLATFORM_FULL_NAME}BinPkg')

    TOOLKIT_TOOLS_PATH       : str = JoinPath (TOOLKIT_PATH, 'Tools')
    TOOLKIT_SIGNING_KEYS_PATH: str = JoinPath (TOOLKIT_PATH, 'SigningKeys')

    def __init__ (self) -> None:
        """ Class to perform the platform specific operation.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        #
        # EnvConfig variable shall be read-only and use by deep copy.
        #
        self.__EnvConfig: Dict[str, str] = os.environ.copy ()

    def __BuildBaseTools (self) -> int:
        """ Build for the EDK-II BaseTools and platform tools.

        Args:
            None.

        Raises:
            ErrorException:
                Failed to build the EDK-II BaseTools.

        Returns:
            int:
                Status of this function operation.
        """
        ExitCode: int       = None
        CmdList : List[str] = [
            JoinPath (PLAT_BOARD_PKG_PATH, 'prep.bat'),
            'c',
            ]

        ExitCode, _, _ = ExecOsSystemCmd (CmdList)
        if ExitCode != STATUS_SUCCESS:
            raise ErrorException ('Failed to build EDK-II BaseTools.')

        return STATUS_SUCCESS

    def __EnvInit (self) -> int:
        """ Initial the capsule build environment.

        Note:
            This function copy the necessary resources.
                (1) Sample signing key for capsule.
                (2) Sample signing key for Windows update package.
                (3) EDK-II BaseTools.
                (4) OpenSSL tool.
                (5) BiosGuard tool.
                (6) Sample signing key for BiosGuard update package.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Status of this function operation.
        """
        MAPPING_TABLE: List[Tuple[str, str]] = [
            #
            # BaseTools
            #
            (
                JoinPath (_PlatformHook.WORKSPACE_CORE,     'BaseTools'),
                JoinPath (_PlatformHook.TOOLKIT_TOOLS_PATH, 'BaseTools')
            ),
            #
            # Signing Keys
            #
            (
                JoinPath (CAP_FEATURE_PKG, 'SigningKeys'),
                _PlatformHook.TOOLKIT_SIGNING_KEYS_PATH,
            ),
            (
                JoinPath (_PlatformHook.PLAT_BIN_PKG_PATH, 'CapsuleUpdate', 'SigningKeys'),
                _PlatformHook.TOOLKIT_SIGNING_KEYS_PATH,
            ),
            #
            # OpenSSL
            #
            (
                JoinPath (_PlatformHook.WORKSPACE_BINARIES, 'Tools', 'OpenSSL'),
                JoinPath (_PlatformHook.TOOLKIT_TOOLS_PATH, 'OpenSSL')
            ),
            #
            # BiosGuard
            #
            (
                JoinPath (_PlatformHook.PLAT_BIN_PKG_PATH, 'Tools', 'ToolScripts', 'BiosGuard'),
                JoinPath (_PlatformHook.TOOLKIT_TOOLS_PATH, 'BiosGuard')
            ),
            (
                JoinPath (_PlatformHook.PLAT_BIN_PKG_PATH, 'Tools', 'InternalOnly', 'BiosGuard'),
                JoinPath (_PlatformHook.TOOLKIT_TOOLS_PATH, 'BiosGuard')
            ),
            ]

        TouchDir (_PlatformHook.TOOLKIT_TOOLS_PATH)
        TouchDir (_PlatformHook.TOOLKIT_SIGNING_KEYS_PATH)

        for Source, Target in MAPPING_TABLE:
            CopyDir (Source, Target)

        return STATUS_SUCCESS

    def PreHook (self, *Args, **Kwargs) -> int:
        """ Platform specific operation before capsule building.

        Args:
            *Args:
                Position argument passed to this function.
            **Kwargs:
                Keyword argument passed to this function.

        Raises:
            None.

        Returns:
            int:
                Status of this function operation.
        """
        KEY_IS_CLEAN_CMD           : str = 'IsClnCmd'
        KEY_IGNORE_BASE_TOOLS_BUILD: str = 'IsIgnoreBaseToolsBuild'

        Status: int = STATUS_SUCCESS

        if (KEY_IS_CLEAN_CMD in Kwargs) and (Kwargs[KEY_IS_CLEAN_CMD]):
            DEBUG (
              DEBUG_INFO,
              "Bypass the Platform PreHook with CLEAN command set."
              )
            return STATUS_SUCCESS

        #
        # Check if need to build BaseTools from source.
        #
        if (KEY_IGNORE_BASE_TOOLS_BUILD in Kwargs) and \
            (not Kwargs[KEY_IGNORE_BASE_TOOLS_BUILD]):
            Status = self.__BuildBaseTools ()
            if (Status != STATUS_SUCCESS):
                return STATUS_ERROR
        else:
            DEBUG (DEBUG_TRACE, 'Bypass to build the EDK-II BaseTools.')

        #
        # Initial the capsule build environment.
        #
        Status = self.__EnvInit ()
        if (Status != STATUS_SUCCESS):
            return STATUS_ERROR

        return Status

    def PostHook (self, *Args, **Kwargs) -> int:
        """ Platform specific operation after capsule building.

        Args:
            *Args:
                Position argument passed to this function.
            **Kwargs:
                Keyword argument passed to this function.

        Raises:
            None.

        Returns:
            int:
                Status of this function operation.
        """
        KEY_IS_CLEAN_CMD: str = 'IsClnCmd'

        Status: int = STATUS_SUCCESS

        if (KEY_IS_CLEAN_CMD in Kwargs) and (Kwargs[KEY_IS_CLEAN_CMD]):
            DEBUG (
              DEBUG_INFO,
              "Bypass the Platform PostHook with CLEAN command set."
              )
            return STATUS_SUCCESS

        return Status

class _PlatformConfig (object):
    CFG_FILE_EXT: str = 'toml'

    def __init__ (
        self,
        PlatformName   : str,
        IsEmbeddedBuild: bool = False,
        ) -> None:
        """ Class to get the platform configuration file.

        Args:
            PlatformName (str):
                Name of this platform.
                (Shall be key of PLATFORM_PREFIX_MAPPING_TABLE.)
            IsEmbedded (bool, optional):
                Build capsule for embedded platform.
                    False - Build for not embedded platform.
                    True  - Build for embedded platform.
                Defaults to False.

        Raises:
            None.

        Returns:
            None.
        """
        self.__PlatformName   : str  = PlatformName.upper ()
        self.__IsEmbeddedBuild: bool = IsEmbeddedBuild

        self.__PreCheck ()

        self.__FilePath: str = self.__GetFilePath ()

    @property
    def FilePath (self) -> str:
        """ Return the path of the platform configuration file.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Path to the platform configuration file.
        """
        return AbsPath (self.__FilePath)

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) PlatformName is not str type.
                (2) IsEmbeddedBuild is not bool type.
            ValueError:
                Invalid PlatformName, the platform name shall be
                one of PLATFORM_PREFIX_MAPPING_TABLE.

        Returns:
            None.
        """
        if not isinstance (self.__PlatformName, str):
            raise TypeError (f'PlatformName shall be str type.')
        elif not isinstance (self.__IsEmbeddedBuild, bool):
            raise TypeError (f'IsEmbeddedBuild shall be bool type.')

        if self.__PlatformName.upper () not in PLATFORM_PREFIX_MAPPING_TABLE.keys ():
            raise ValueError (
                    f'Invalid platform name, shall be one of '
                    f'{PLATFORM_PREFIX_MAPPING_TABLE.keys ()} '
                    f'but get [{self.__PlatformName}]'
                    )

    def __GetFileName (self) -> str:
        """ Get the platform configuration file name.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Name of the platform configuration file.
        """
        Prefix : str = PLATFORM_PREFIX_MAPPING_TABLE[self.__PlatformName]
        Postfix: str = str ()

        if self.__IsEmbeddedBuild:
            Postfix = EMBEDDED_POSTFIX

        return f'{Prefix}{Postfix}.{_PlatformConfig.CFG_FILE_EXT}'

    def __GetFilePath (self) -> str:
        """ Get the path of the platform configuration file.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Path to the platform configuration file.
        """
        FileName: str = self.__GetFileName ()
        FilePath: str = AbsPath (JoinPath (CAPSULE_CFG_PATH, FileName))

        return FilePath

class _PlatformEnvConfig (object):
    ENV_DEBUG_LEVEL_STR  : str = 'DEBUG_LEVEL'
    ENV_PLAT_CFG_FILE_STR: str = 'PLATFORM_CONFIG_FILE'

    def __init__ (
        self,
        DebugLevel : str,
        PlatCfgPath: str,
        ) -> None:
        """ Class to get the platform environment configuration file.

        Args:
            DebugLevel (str):
                Level of the debug message
                It should be one of the list of DEBUG_LEVEL_LIST.
            PlatCfgPath (str):
                Path of the platform configuration file.

        Raises:
            None.

        Returns:
            None.
        """
        self.__DebugLevel : str = DebugLevel.upper ()
        self.__PlatCfgPath: str = PlatCfgPath

        self.__PreCheck ()

        self.__BaseEnvCfg: Dict[str, str] = os.environ.copy ()
        self.__PlatEnvCfg: Dict[str, str] = self.__GetEnvCfg ()

    @property
    def EnvConfig (self) -> Dict[str, str]:
        """ Return the environment variable configuration.

        Args:
            None.

        Raises:
            None.

        Returns:
            Dict[str, str]:
                Environment variable configuration in key-value.
        """
        return self.__PlatEnvCfg

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) DebugLevel is not str type.
                (2) PlatCfgPath is not str type.
            ValueError:
                (1) DebugLevel is not one of DEBUG_LEVEL_LIST.
                (2) PlatCfgPath is not exist.
                (3) PlatCfgPath is not file.

        Returns:
            None.
        """
        if not isinstance (self.__DebugLevel, str):
            raise TypeError ('DebugLevel shall be str type.')
        if not isinstance (self.__PlatCfgPath, str):
            raise TypeError ('PlatCfgPath shall be str type.')

        if self.__DebugLevel not in DEBUG_LEVEL_LIST:
            raise ValueError (f'DebugLevel shall be one of DEBUG_LEVEL_LIST.')
        elif not IsPathExist (self.__PlatCfgPath):
            raise ValueError (f'PlatCfgPath is not exist.')
        elif not IsFile (self.__PlatCfgPath):
            raise ValueError (f'PlatCfgPath is not file.')

    def __GetEnvCfg (self) -> Dict[str, str]:
        """ Get the environment variable configuration.

        Note:
            If the variable had been set outside of script,
            prompt-out the warning message and not to override.

        Args:
            None.

        Raises:
            None.

        Returns:
            Dict[str, str]:
                Environment variable configuration in key-value.
        """
        ENV_VAR_TABLE: Dict[str, str] = {
            _PlatformEnvConfig.ENV_DEBUG_LEVEL_STR  : self.__DebugLevel,
            _PlatformEnvConfig.ENV_PLAT_CFG_FILE_STR: self.__PlatCfgPath,
            }

        EnvCfg: Dict[str, str] = copy.deepcopy (self.__BaseEnvCfg)
        Value : str            = None

        for VarName, VarValue in ENV_VAR_TABLE.items ():
            Value = EnvCfg.setdefault (VarName, VarValue)

            if Value != VarValue:
                DEBUG (
                  DEBUG_WARN,
                  f'Environment variable [{VarName}] set to [{Value}] '
                  f'without override the value to [{VarValue}]'
                  )
            else:
                DEBUG (
                  DEBUG_TRACE,
                  f'Set the environment variable '
                  f'[{VarName}] - [{Value}]'
                  )

        return EnvCfg

def _BuildCapsuleEntry (
    InputCmdList: List[str],
    PlatformName: str,
    DebugLevel  : str  = DEBUG_INFO,
    IsEmbedded  : bool = False,
    ) -> int:
    """ Main function to call the script to build the capsule.

    Args:
        InputCmdList (List[str]):
            List of command to be passed into the build script.
        PlatformName (str):
            Name of this platform.
            (Shall be key of PLATFORM_PREFIX_MAPPING_TABLE.)
        DebugLevel (str, optional):
            Level of the debug message
            It should be one of the list of DEBUG_LEVEL_LIST.
            Defaults to DEBUG_INFO.
        IsEmbedded (bool, optional):
            Build capsule for embedded platform.
                False - Build for not embedded platform.
                True  - Build for embedded platform.
            Defaults to False.

    Raises:
        None.

    Returns:
        int:
            Status of this function operation.
    """
    ScriptPath: str       = JoinPath (TOOLKIT_PATH, 'FmpCapsuleGenWrapper.py')
    CmdList   : List[str] = None

    PlatCfgFilePath: str            = \
        _PlatformConfig (PlatformName, IsEmbedded).FilePath
    EnvCfg         : Dict[str, str] = \
        _PlatformEnvConfig (DebugLevel, PlatCfgFilePath).EnvConfig

    #
    # Call the main script to build the capsule.
    #
    CmdList = list ()
    CmdList.append (ScriptPath)
    CmdList.extend (InputCmdList)

    ExitCode, _, _ = ExecPythonCmd (CmdList, Env = EnvCfg)

    return ExitCode

def Main (
    InputCmdList          : List[str],
    PlatformName          : str,
    DebugLevel            : str  = DEBUG_INFO,
    IsEmbedded            : bool = False,
    IsIgnoreBaseToolsBuild: bool = False,
    ) -> int:
    """ Main function for platform capsule build.

    Args:
        InputCmdList (List[str]):
            List of command to be passed into the build script.
        PlatformName (str):
            Name of this platform.
            (Shall be key of PLATFORM_PREFIX_MAPPING_TABLE.)
        DebugLevel (str, optional):
            Level of the debug message
            It should be one of the list of DEBUG_LEVEL_LIST.
            Defaults to DEBUG_INFO.
        IsEmbedded (bool, optional):
            Build capsule for embedded platform.
                False - Build for not embedded platform.
                True  - Build for embedded platform.
            Defaults to False.
        IsIgnoreBaseToolsBuild (bool, optional):
            Build the EDK-II BaseTools.
            If bypass, then it need to provided by caller itself.
                False - Not bypass the building operation.
                True  - Bypass the building operation.
            Defaults to False.

    Raises:
        ErrorException:
            (1) Failed to perform platform PreHook function.
            (2) Failed to perform main capsule build function.
            (3) Failed to perform platform PostHook function.

    Returns:
        int:
            Exit code of this function.
    """
    IsClnCmd: bool          = False
    PlatHook: _PlatformHook = _PlatformHook ()
    CmdList : List[str]     = InputCmdList

    #
    # Check if CLEAN command is set.
    #
    IsClnCmd, _ = SearchValueInList (
                    CmdList,
                    CLEAN_COMMAND,
                    )
    if IsClnCmd:
        CmdList = [CLEAN_COMMAND]

    DEBUG (DEBUG_TRACE, f"Command passed from platform: {CmdList}")

    #
    # Platform specific PreHook function.
    #
    Status = PlatHook.PreHook (
                        IsIgnoreBaseToolsBuild = IsIgnoreBaseToolsBuild,
                        IsClnCmd               = IsClnCmd,
                        )
    if (Status != STATUS_SUCCESS):
        raise ErrorException ('Failed on platform PreHook function.')

    #
    # Main function to build the capsule.
    #
    Status = _BuildCapsuleEntry (
               InputCmdList = CmdList,
               PlatformName = PlatformName,
               DebugLevel   = DebugLevel,
               IsEmbedded   = IsEmbedded,
               )
    if (Status != STATUS_SUCCESS):
        raise ErrorException (f'Failed on main capsule build function.')

    #
    # Platform specific PostHook function.
    #
    Status = PlatHook.PostHook (IsClnCmd = IsClnCmd)
    if (Status != STATUS_SUCCESS):
        raise ErrorException ('Failed on platform PostHook function.')

    return STATUS_SUCCESS

if __name__ == '__main__':
    def GetArgs () -> Tuple[argparse.Namespace, List[str]]:
        """ Get the user input arguments and parse the known one only.

        Args:
            None.

        Raises:
            None.

        Returns:
            Tuple[argparse.Namespace, List[str]]:
                argparse.Namespace:
                    The argument object parsing from user input.
                List[str]:
                    Remain command list not parsing by this parser.
        """
        Parser: argparse.ArgumentParser = argparse.\
            ArgumentParser (
                add_help     = False,
                allow_abbrev = False,
                )

        Args   : argparse.Namespace = None
        CmdList: List[str]          = None

        Parser.add_argument (
                    '-platform',
                    dest     = 'Platform',
                    help     = f'Input the platform name {PLATFORM_PREFIX_MAPPING_TABLE.keys ()}.',
                    type     = str.upper,
                    required = False,
                    default  = PTL_MOBILE_PLAT_NAME_STR,
                    )
        Parser.add_argument (
                    '-debug-level',
                    dest     = 'DebugLevel',
                    help     = f'Set the debug level {DEBUG_LEVEL_LIST}. (Default: {DEBUG_INFO})',
                    type     = str.upper,
                    required = False,
                    default  = DEBUG_INFO,
                    )
        Parser.add_argument (
                    '--embedded',
                    dest     = 'IsEmbedded',
                    help     = 'Build with the embedded flag. (Default: False)',
                    action   = 'store_true',
                    required = False,
                    default  = False,
                    )
        Parser.add_argument (
                    '--ignore-base-tools-build',
                    dest     = 'IsIgnoreBaseToolsBuild',
                    help     = 'Bypass to build the BaseTools. (Default: False)',
                    action   = 'store_true',
                    required = False,
                    default  = False,
                    )

        Args, CmdList = Parser.parse_known_args ()

        return Args, CmdList

    Args      : argparse.Namespace = None
    CmdList   : List[str]          = None
    ExitCode  : int                = EXIT_SUCCESS
    DebugLevel: str                = DEBUG_INFO

    Args, CmdList = GetArgs ()

    DebugLevel = Args.DebugLevel
    SetDebugLevel (DebugLevel)

    ExitCode = Main (
                 InputCmdList           = CmdList,
                 PlatformName           = Args.Platform,
                 DebugLevel             = DebugLevel,
                 IsEmbedded             = Args.IsEmbedded,
                 IsIgnoreBaseToolsBuild = Args.IsIgnoreBaseToolsBuild,
                 )

    sys.exit (ExitCode)
