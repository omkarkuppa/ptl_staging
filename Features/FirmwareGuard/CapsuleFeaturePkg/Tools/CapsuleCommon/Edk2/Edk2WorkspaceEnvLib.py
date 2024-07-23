## @file
#  Library supported to create EDK-II build environment workspace.
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
from typing import Union

from CapsuleCommon.Base.DirectoryLib import *
from CapsuleCommon.Base.FileLib import *
from CapsuleCommon.Base.PathLib import *
from CapsuleCommon.Wrapper.FileWrapperLib import *

from CapsuleCommon.Edk2.ConstantLib import *

EDKII_BUILD_TARGET_TEMPLATE: str = '''
## @file
#  EDK-II build target file.
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

TOOL_CHAIN_CONF              = Conf/tools_def.txt
BUILD_RULE_CONF              = Conf/build_rule.txt

MAX_CONCURRENT_THREAD_NUMBER = 1
ACTIVE_PLATFORM              = {PkgName}/{DscName}
TARGET                       = {Target}
TARGET_ARCH                  = {Arch}
TOOL_CHAIN_TAG               = {ToolChain}
'''

class Edk2WorkspaceEnv (object):
    def __init__ (
        self,
        Workspace    : Union[str, os.PathLike],
        Edk2ToolsPath: Union[str, os.PathLike],
        ) -> None:
        """ Class to build the EDK-II workspace environment.

        Args:
            Workspace (Union[str, os.PathLike]):
                EDK-II defined workspace path.
            Edk2ToolsPath (Union[str, os.PathLike]):
                Path to EDK-II BaseTools.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Workspace    : str = AbsPath (Workspace)
        self.__Edk2ToolsPath: str = AbsPath (Edk2ToolsPath)
        self.__ConfDirPath  : str = JoinPath (
                                      self.__Workspace,
                                      EDKII_CONF_DIR_NAME,
                                      )

    @property
    def BuildRuleFilePath (self) -> str:
        """ Get the EDK-II build rule file path.

        Args:
            None

        Raises:
            None.

        Returns:
            str:
                Path to the workspace EDK-II build rule file.
        """
        return JoinPath (self.__Workspace, EDKII_BUILD_RULE_FILE_PATH)

    @property
    def TargetFilePath (self) -> str:
        """ Get the EDK-II target file path.

        Args:
            None

        Raises:
            None.

        Returns:
            str:
                Path to the workspace EDK-II target file.
        """
        return JoinPath (self.__Workspace, EDKII_TARGET_FILE_PATH)

    @property
    def ToolsDefFilePath (self) -> str:
        """ Get the EDK-II tools definition file path.

        Args:
            None

        Raises:
            None.

        Returns:
            str:
                Path to the workspace EDK-II tools definition file.
        """
        return JoinPath (self.__Workspace, EDKII_TOOLS_DEF_FILE_PATH)

    def Initialize (self) -> None:
        """ Expose function to initial the EDK-II workspace environment.

        Note:
            Refer from "toolsetup.bat" flow within EDK-II BaseTools.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        Edk2FilePath     : str            = None
        WorkspaceFilePath: str            = None
        FilePathDict     : Dict[str, str] = {
            EDKII_BUILD_RULE_TEMPLATE_FILE_PATH: self.BuildRuleFilePath,
            EDKII_TARGET_TEMPLATE_FILE_PATH    : self.TargetFilePath,
            EDKII_TOOLS_DEF_TEMPLATE_FILE_PATH : self.ToolsDefFilePath,
            }

        TouchDir (self.__ConfDirPath)

        for SrcPath, DesPath in FilePathDict.items ():
            Edk2FilePath      = JoinPath (self.__Edk2ToolsPath, SrcPath)
            WorkspaceFilePath = DesPath
            DEBUG (
              DEBUG_TRACE,
              f'Copy from {Edk2FilePath} to {WorkspaceFilePath}'
              )
            CopyFile (Edk2FilePath, WorkspaceFilePath)

    def CreateTargetFile (
        self,
        PkgName  : str                   = 'SamplePkg',
        DscName  : str                   = 'Sample.dsc',
        Target   : str                   = BUILD_TARGET_DEBUG_TYPE,
        Arch     : Union[str, List[str]] = ARCH_IA32,
        ToolChain: str                   = TOOL_CHAIN_TAG_VS2015X86,
        ) -> None:
        """ Create the EDK-II build environment target file.

        Args:
            PkgName (str, optional):
                The target package name to be built.
                Defaults to 'SamplePkg'.
            DscName (str, optional):
                The target DSC file name.
                Defaults to 'Sample.dsc'.
            Target (str, optional):
                The build target.
                Should be one of BUILD_TARGET_TYPE_LIST.
                Defaults to BUILD_TARGET_DEBUG_TYPE.
            Arch (Union[str, List[str]], optional):
                The platform architecture.
                Should be one of ARCH_TYPE_LIST
                Defaults to ARCH_IA32.
            ToolChain (str, optional):
                The build tool chain.
                Defaults to TOOL_CHAIN_TAG_VS2015X86.

        Raises:
            TypeError:
                (1) PkgName is not str type.
                (2) DscName is not str type.
                (3) Target is not str type.
                (4) ToolChain is not str type.
                (5) Arch is not str or list type.

        Returns:
            None.
        """
        FilePath   : str  = self.TargetFilePath
        TargetFile : File = File (FileContent = EDKII_BUILD_TARGET_TEMPLATE)

        if not isinstance (PkgName, str):
            raise TypeError (f'PkgName should be str type.')
        elif not isinstance (DscName, str):
            raise TypeError (f'DscName should be str type.')
        elif not isinstance (Target, str):
            raise TypeError (f'Target should be str type.')
        elif not isinstance (ToolChain, str):
            raise TypeError (f'ToolChain should be str type.')
        elif not isinstance (Arch, (list, str)):
            raise TypeError (f'Arch should be str/list type.')

        MappingData: dict = {
            'PkgName'  : PkgName,
            'DscName'  : DscName,
            'Target'   : Target,
            'Arch'     : Arch if isinstance (Arch, str) else ' '.join (Arch),
            'ToolChain': ToolChain,
            }
        TargetFile.Fill (MappingData)
        TargetFile.Save (FilePath)
