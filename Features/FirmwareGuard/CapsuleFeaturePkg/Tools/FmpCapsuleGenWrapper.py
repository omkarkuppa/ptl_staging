## @file
#  Generic script to generate FMP signed Capsule.
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

import copy
import os
from typing import Dict

from CapsuleCommon import *

TOOLKIT_PATH                : str = os.path.dirname (os.path.abspath (__file__))
PY_CAPSULE_COMMON_PKG_PATH  : str = JoinPath (TOOLKIT_PATH, 'CapsuleCommon')
PY_CAPSULE_GENERATE_PKG_PATH: str = JoinPath (TOOLKIT_PATH, 'CapsuleGenerate')

#
# Definition of environment variables.
#
ENV_DEBUG_LEVEL_STR    : str = 'DEBUG_LEVEL'
ENV_COMMON_CFG_FILE_STR: str = 'COMMON_CONFIG_FILE'
ENV_PLAT_CFG_FILE_STR  : str = 'PLATFORM_CONFIG_FILE'
ENV_PY_PATH_STR        : str = 'PYTHONPATH'

DEFAULT_DEBUG_LEVEL : str       = DEBUG_INFO
COMMON_CFG_FILE_PATH: str       = \
    JoinPath (
      PY_CAPSULE_GENERATE_PKG_PATH,
      'Config',
      'Common.toml'
      )
PYTHON_PATH_LIST    : List[str] = [
    AbsPath (TOOLKIT_PATH),
    ]

#
# Exit code of this script.
#
EXIT_SUCCESS: int = 0x00

class _EnvConfig (object):
    KEY_DEFAULT  : str = 'Default'
    KEY_MANDATORY: str = 'IsMandatory'

    def __init__ (self) -> None:
        """ Class for creating the environment config for this script.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__BaseEnvCfg: Dict[str, str] = os.environ.copy ()
        self.__EnvCfg    : Dict[str, str] = self.__GetEnvCfg ()

    @property
    def EnvConfig (self) -> Dict[str, str]:
        """ Return the environment variable config.

        Args:
            None.

        Raises:
            None.

        Returns:
            Dict[str, str]:
                Environment config dictionary in Key-Value format.
        """
        return self.__EnvCfg

    def __AddPythonPath (
        self,
        EnvCfg: Dict[str, str],
        ) -> Dict[str, str]:
        """ Concat the list of path in PYTHONPATH variable.

        Args:
            EnvCfg (Dict[str, str]):
                Environment config dictionary in Key-Value format.

        Raises:
            None.

        Returns:
            Dict[str, str]:
                Environment config dictionary with necessary PYTHONPATH
                in Key-Value format.
        """
        ResultConfig: Dict[str, str] = copy.deepcopy (EnvCfg)
        PathList    : List[str]      = list ()

        if ENV_PY_PATH_STR in ResultConfig:
            PathList.extend (ResultConfig[ENV_PY_PATH_STR].split (os.pathsep))

        PathList.extend (PYTHON_PATH_LIST)

        ResultConfig[ENV_PY_PATH_STR] = (os.pathsep).join (PathList)

        return ResultConfig

    def __GetEnvCfg (self) -> Dict[str, str]:
        """ Get the script environment config in Key-Value format.

        Args:
            None.

        Raises:
            ValueError:
                Mandatory environment variable is not set.

        Returns:
            Dict[str, str]:
                Environment config dictionary in Key-Value format.
        """
        MAPPING_TABLE_DICT: Dict[str, Dict] = {
            ENV_DEBUG_LEVEL_STR    : {
                _EnvConfig.KEY_MANDATORY: False,
                _EnvConfig.KEY_DEFAULT  : DEFAULT_DEBUG_LEVEL,
                },
            ENV_COMMON_CFG_FILE_STR: {
                _EnvConfig.KEY_MANDATORY: False,
                _EnvConfig.KEY_DEFAULT  : COMMON_CFG_FILE_PATH,
            },
            ENV_PLAT_CFG_FILE_STR  : {
                _EnvConfig.KEY_MANDATORY: True,
                _EnvConfig.KEY_DEFAULT  : None,
                },
            }

        EnvCfg: Dict[str, str] = copy.deepcopy (self.__BaseEnvCfg)

        for VarName, VarCfg in MAPPING_TABLE_DICT.items ():
            if VarName in EnvCfg:
                continue

            if VarCfg[_EnvConfig.KEY_MANDATORY]:
                raise ValueError (
                        f'{VarName} is not set in environment variable '
                        f'which is mandatory.'
                        )

            DEBUG (
              DEBUG_TRACE,
              f'Set [{VarName}] to [{VarCfg[_EnvConfig.KEY_DEFAULT]}]'
              )

            EnvCfg.update ({VarName: VarCfg[_EnvConfig.KEY_DEFAULT]})

        EnvCfg = self.__AddPythonPath (EnvCfg)

        return EnvCfg

class _CapsuleBuild (object):
    #
    # Python script used for this API.
    #
    PY_SCRIPT_ENV_CHECK_PATH    : str = \
        JoinPath (TOOLKIT_PATH, 'EnvCheck.py')
    PY_SCRIPT_FMP_CAP_BUILD_PATH: str = \
        JoinPath (TOOLKIT_PATH, 'FmpCapsuleGen.py')

    def __init__ (self, EnvConfig: _EnvConfig) -> None:
        """ Class for wrapping the FMP capsule build as APIs.

        Note:
            For the caller, execute the APIs as below order:
                - PreCheck
                - Build
                - PostCheck
                - Cleanup

        Args:
            EnvConfig (_EnvConfig):
                _EnvConfig object to provide the config for this script.

        Raises:
            None.

        Returns:
            None.
        """
        self.__EnvConfig   : _EnvConfig = EnvConfig
        self.__BuildCmdList: List[str]  = sys.argv[1:]

    def PreCheck (self) -> int:
        """ Exposed function to execute the PreCheck flow.

        Args:
            None.

        Raises:
            ErrorException:
                Failed on executing the PreCheck script.

        Returns:
            int:
                Status of this function operation.
        """
        ExitCode: int       = None
        CmdList : List[str] = [
            _CapsuleBuild.PY_SCRIPT_ENV_CHECK_PATH,
            '--pre-check',
            ]

        ExitCode, _, _ = ExecPythonCmd (
                           CmdList,
                           Env         = self.__EnvConfig,
                           IsException = False,
                           )
        if (ExitCode != STATUS_SUCCESS) :
            raise ErrorException (f'Failed on Capsule build PreCheck.')

        return STATUS_SUCCESS

    def PostCheck (self) -> int:
        """ Exposed function to execute the PostCheck flow.

        Args:
            None.

        Raises:
            ErrorException:
                Failed on executing the PostCheck script.

        Returns:
            int:
                Status of this function operation.
        """
        ExitCode: int       = None
        CmdList : List[str] = [
            _CapsuleBuild.PY_SCRIPT_ENV_CHECK_PATH,
            '--post-check',
            ]

        ExitCode, _, _ = ExecPythonCmd (
                           CmdList,
                           Env         = self.__EnvConfig,
                           IsException = False,
                           )
        if (ExitCode != STATUS_SUCCESS) :
            raise ErrorException (f'Failed on Capsule build PostCheck.')

        return STATUS_SUCCESS

    def Build (self) -> int:
        """ Exposed function to execute the FMP capsule build flow.

        Args:
            None.

        Raises:
            ErrorException:
                Failed on executing the FMP Capsule build script.

        Returns:
            int:
                Status of this function operation.
        """
        ExitCode: int       = None
        CmdList : List[str] = list ()

        CmdList.append (_CapsuleBuild.PY_SCRIPT_FMP_CAP_BUILD_PATH)
        CmdList.extend (self.__BuildCmdList)

        ExitCode, _, _ = ExecPythonCmd (
                           CmdList,
                           Env         = self.__EnvConfig,
                           IsException = False,
                           )
        if (ExitCode != STATUS_SUCCESS) :
            raise ErrorException (f'Failed to build the capsule.')

        return STATUS_SUCCESS

    def Cleanup (self) -> int:
        """ Exposed function to execute the Cleanup flow.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Status of this function operation.
        """
        FilePathList: List[str] = [
            #
            # Intermediated files for EDSS signing.
            #
            JoinPath (os.getcwd (), 'edss.db'),
            JoinPath (os.getcwd (), 'intel-ca.crt'),
            JoinPath (os.getcwd (), 'Log.txt'),
            ]

        DirPathList: List[str] = [
            #
            # Intermediated directories for EDSS signing.
            #
            JoinPath (os.getcwd (), 'Certificates'),
            ]

        for FilePath in FilePathList:
            if IsFile (FilePath):
                RemoveFile (FilePath)

        for DirPath in DirPathList:
            if IsDir (DirPath):
                RemoveDir (DirPath)

        return STATUS_SUCCESS

def _InitDebugLevel () -> None:
    """ Initial the debug service for this script.

    Note:
        (1) This depend on the ENV_DEBUG_LEVEL_STR environment variable.
        (2) Default value would be DEFAULT_DEBUG_LEVEL.

    Args:
        None.

    Raises:
        None.

    Returns:
        None.
    """
    DebugLevel: str = DEFAULT_DEBUG_LEVEL

    if ENV_DEBUG_LEVEL_STR in os.environ:
        DebugLevel = os.environ[ENV_DEBUG_LEVEL_STR]

    SetDebugLevel (DebugLevel)

    return DebugLevel

def _Debug (EnvConfig: Dict[str, str]) -> None:
    """ Dump the debug information of this script environment config.

    Args:
        EnvConfig (Dict[str, str]):
            Environment config dictionary in Key-Value format.

    Raises:
        None.

    Returns:
        None.
    """
    DEBUG (DEBUG_TRACE, '========== DEBUG TRACE START ==========')

    DEBUG (DEBUG_TRACE, f'Toolkit Path         = {TOOLKIT_PATH}')
    DEBUG (DEBUG_TRACE, f'Debug Level          = {EnvConfig[ENV_DEBUG_LEVEL_STR]}')
    DEBUG (DEBUG_TRACE, f'Common Config File   = {EnvConfig[ENV_COMMON_CFG_FILE_STR]}')
    DEBUG (DEBUG_TRACE, f'Platform Config File = {EnvConfig[ENV_PLAT_CFG_FILE_STR]}')
    DEBUG (DEBUG_TRACE, f'PYTHONPATH           = {EnvConfig[ENV_PY_PATH_STR]}')
    DEBUG (DEBUG_TRACE, f'Python Command       = {GetPythonCmdList ()}')

    DEBUG (DEBUG_TRACE, '========== DEBUG TRACE FINAL ==========')

def Main () -> int:
    """ Main function of flow to generate the FMP capsule.

    Args:
        None.

    Raises:
        ErrorException:
            (1) Failed to execute the PreCheck API.
            (2) Failed to execute the Build API.
            (3) Failed to execute the PostCheck API.
            (4) Failed to execute the Cleanup API.

    Returns:
        int:
            Exit code of this script.
    """
    EnvConfig   : _EnvConfig    = None
    CapsuleBuild: _CapsuleBuild = None

    #
    # Initial the debug service.
    #
    _InitDebugLevel ()

    #
    # Initial the variables.
    #
    EnvConfig    = _EnvConfig ()
    CapsuleBuild = _CapsuleBuild (EnvConfig = EnvConfig.EnvConfig)

    _Debug (EnvConfig.EnvConfig)

    #
    # PreCheck for Capsule building.
    #
    Status = CapsuleBuild.PreCheck ()
    if (Status != STATUS_SUCCESS):
        raise ErrorException (f'Failed to execute the PreCheck API.')

    #
    # Main script for Capsule building.
    #
    Status = CapsuleBuild.Build ()
    if (Status != STATUS_SUCCESS):
        raise ErrorException (f'Failed to execute the Build API.')

    #
    # PostCheck for Capsule building.
    #
    Status = CapsuleBuild.PostCheck ()
    if (Status != STATUS_SUCCESS):
        raise ErrorException (f'Failed to execute the PostCheck API.')

    #
    # Cleanup the intermediated files and directories.
    #
    Status = CapsuleBuild.Cleanup ()
    if (Status != STATUS_SUCCESS):
        raise ErrorException (f'Failed to execute the Cleanup API.')

    return EXIT_SUCCESS

if __name__ == '__main__':
    sys.exit (Main ())
