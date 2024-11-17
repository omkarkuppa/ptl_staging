## @file
#  Library provided the EDK-II build environment variables.
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

import copy
import os
from typing import Union

from CapsuleCommon.Base.BaseLib import *
from CapsuleCommon.Base.PathLib import AbsPath, JoinPath
from CapsuleCommon.Base.SystemLib import *

from CapsuleCommon.Edk2.ConstantLib import *
from CapsuleCommon.Edk2.Edk2ToolPathLib import *

class Edk2BuildEnv (object):
    def __init__ (
        self,
        Workspace    : Union[str, os.PathLike],
        Edk2ToolsPath: Union[str, os.PathLike],
        PyCmd        : Union[None, str] = None,
        ) -> None:
        """ Class to retrieve the EDK-II environment variable.

        Args:
            Workspace (Union[str, os.PathLike]):
                EDK-II defined workspace path.
            Edk2ToolsPath (Union[str, os.PathLike]):
                Path to EDK-II BaseTools.
            PyCmd (str, optional):
                Command to call the Python.
                Defaults to None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Workspace    : str = AbsPath (Workspace)
        self.__Edk2ToolsPath: str = AbsPath (Edk2ToolsPath)
        self.__PyCmd        : str = PyCmd

        if self.__PyCmd is None:
            self.__PyCmd = ' '.join (GetPythonCmdList ())

        self.__Edk2ToolsCmdPath: str = \
            JoinPath (
              self.__Edk2ToolsPath,
              GetEdk2ToolBinWrapperPath (),
              )
        self.__Edk2PySrcPath   : str = \
            JoinPath (
              self.__Edk2ToolsPath,
              EDKII_BASE_TOOL_PYTHON_SOURCE_PATH,
              )
        self.__Edk2ToolsBinPath: str = \
            JoinPath (
              self.__Edk2ToolsPath,
              GetEdk2ToolBinPath (),
              )

        self.__EnvConfig: dict = self.__SetEnvConfigVariable ()

    @property
    def EnvVar (self) -> dict:
        """ Return EDK-II needed environment variables dictionary.

        Args:
            None.

        Raises:
            None.

        Returns:
            dict:
                Current environment variable with EDK-II.
        """
        return self.__EnvConfig

    def __SetEnvConfigVariable (self) -> dict:
        """ Set the EDK-II environment variable.

        Note:
            This might be varied between different EDK-II BaseTools
            codebase.

        Args:
            None.

        Raises:
            None.

        Returns:
            dict:
                Current environment variable with EDK-II.
        """
        #
        # Deep copy is needed.
        # To prevent any modification on current environment variable.
        #
        EnvConfig : dict = dict (copy.deepcopy (os.environ))
        EnvPath   : str  = EnvConfig.get ('PATH', '')
        PythonPath: str  = EnvConfig.get ('PYTHONPATH', '')

        EnvPath    = (os.pathsep).join ([
                                    self.__Edk2ToolsCmdPath,
                                    self.__Edk2ToolsBinPath,
                                    EnvPath,
                                    ])
        PythonPath = (os.pathsep).join ([
                                    self.__Edk2PySrcPath,
                                    PythonPath,
                                    ])

        EnvConfig['WORKSPACE']       = self.__Workspace
        EnvConfig['PYTHONPATH']      = PythonPath
        EnvConfig['PATH']            = EnvPath
        EnvConfig['PYTHON_COMMAND']  = self.__PyCmd
        EnvConfig['BASE_TOOLS_PATH'] = self.__Edk2ToolsPath
        EnvConfig['EDK_TOOLS_PATH']  = self.__Edk2ToolsCmdPath
        EnvConfig['EDK_TOOLS_BIN']   = self.__Edk2ToolsBinPath

        return EnvConfig
