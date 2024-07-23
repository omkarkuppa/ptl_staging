## @file
#  Library supported to initial the Capsule Generate environment.
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

from CapsuleCommon import *

from CapsuleGenerate._Global import *
from CapsuleGenerate.ApiPrivate.Edk2DscGeneratorApiLib import *

EDKII_TOOLS_DEF_APPEND_CONTENT: str = f'''\

####################################################
# Below used to define the customized TOOL_CHAIN_TAG
####################################################
*_{EDKII_BLD_CHAIN_NAME}_*_*_FAMILY          = CAPSULETOOL
*_{EDKII_BLD_CHAIN_NAME}_*_*_BUILDRULEFAMILY = CAPSULETOOL

'''

class InitializeEnv (object):
    def __init__ (self) -> None:
        """ Class support to initial the capsule generate environment.

        Note:
            (1) This should be used the Temporary folder as main
                operation workspace.
            (2) All method define in here should be staticmethod.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        pass

    @staticmethod
    def TouchNeededDir () -> None:
        """ Initial the necessary directory if not exists.

        Args:
            None.

        Raises:
            ErrorException:
                Path is not exist, touch directory failed.
            ValueError:
                Path is exist but not directory.

        Returns:
            None.
        """
        DirNameList: List[str] = [
            TEMPORARY_DIR_PATH,
            OUTPUT_DIR_PATH,
            EDKII_BLD_FV_DIR_PATH,
            ]

        for Path in DirNameList:
            DEBUG (DEBUG_TRACE, f'Touch the directory: {Path}')
            TouchDir (Path)

            if not IsPathExist (Path):
                raise ErrorException (
                        f'Unexpected error occur, directory is not exist: '
                        f'{Path}'
                        )
            elif not IsDir (Path):
                raise ValueError (f'Path is exist but not directory: {Path}')

    @staticmethod
    def Edk2WorkspaceInit () -> None:
        """ Initial the EDK-II workspace environment.

        Note:
            (1) Support to create the Conf file.
            (2) Caller responsibility to create the necessary DSC and
                FDF file to build the FD/FV.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        Initiator = Edk2WorkspaceEnv (
                      Workspace     = EDKII_WORKSPACE_PATH,
                      Edk2ToolsPath = EDKII_TOOL_PATH,
                      )
        Initiator.Initialize ()
        Initiator.CreateTargetFile (
                    PkgName   = EDKII_CAP_PKG_NAME,
                    DscName   = EDKII_DSC_FILE_NAME,
                    Target    = EDKII_BLD_TARGET_NAME,
                    Arch      = EDKII_BLD_ARCH_LIST,
                    ToolChain = EDKII_BLD_CHAIN_NAME,
                    )

        #
        # If assign tool chain is not within the tools_def.txt file.
        # Need to append it to use the external tools.
        #
        if EDKII_BLD_CHAIN_NAME not in TOOL_CHAIN_TAG_LIST:
            pass
            with open (Initiator.ToolsDefFilePath, 'a') as ToolsDefFile:
                ToolsDefFile.write (EDKII_TOOLS_DEF_APPEND_CONTENT)

    @staticmethod
    def CreateEdk2DscFile () -> None:
        """ Initial the EDK-II DSC file to meet the build criteria.

        Note:
            (1) It is FMP instance responsibility to provide the FDF
                file to build the firmware volume (FV).
            (2) The version of DSC should be align with latest EDK-II
                specification.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        #
        # Create the DSC file.
        #
        Edk2DscGenerator: Edk2DscGeneratorApi = Edk2DscGeneratorApi ()
        Edk2DscGenerator.Save (EDKII_DSC_FILE_PATH)

    @staticmethod
    def CreateDummyFwFiles () -> None:
        """ Initial the necessary dummy files.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        FilePath: str = str ()

        for FwName, FwInfo in gFwComponentInfo.items ():
            FilePath = JoinPath (TEMPORARY_DIR_PATH, FwInfo[FILE_NAME_KEY])
            DEBUG (DEBUG_TRACE, f'Create the dummy {FwName} file: {FilePath}')
            CreateDummyFile (FilePath)

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'InitializeEnv',
    ]
