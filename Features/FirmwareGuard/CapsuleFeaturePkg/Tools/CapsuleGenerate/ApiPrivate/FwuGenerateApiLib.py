## @file
#  Private API to generate the CSME FWUpdate image via FIT tool.
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

from CapsuleCommon import *

from CapsuleGenerate._Global import *

class FwuGenerateApi (object):
    #
    # Get the FIT tool section configuration.
    #
    CFG_FIT_TOOL_SECTION: dict = gPlatformCfgData[FIT_TOOL_SECTION]

    #
    # Python script used for this API.
    #
    PY_FWU_BUILDER_SCRIPT_PATH: str = \
        JoinPath (
          WORKSPACE_PATH,
          CFG_FIT_TOOL_SECTION[IFWI_FWUPDATE_BUILDER_SCRIPT_KEY],
          )

    #
    # Path of workspace and export directory.
    #
    FWU_WORKSPACE_DIR_PATH: str = \
        JoinPath (
          TEMPORARY_DIR_PATH,
          FIT_FWUPDATE_DIR_NAME,
          )
    FWU_EXPORT_DIR_PATH   : str = FWU_WORKSPACE_DIR_PATH

    #
    # Assign the platform name of layout
    #
    LAYOUT_PLATFORM: str = CFG_FIT_TOOL_SECTION[FIT_TOOL_LAYOUT_PLATFORM]

    def __init__ (
        self,
        IfwiPath       : Union[str, os.PathLike],
        FitToolPath    : Union[str, os.PathLike],
        IfwiCfgFilePath: Union[str, os.PathLike],
        CSEImagePath   : Union[str, os.PathLike] = '',
        ):
        """ Class for API to build the CSME FWUpdate image via FIT tool.

        Args:
            IfwiPath (Union[str, os.PathLike]):
                The path of IFWI image to be decomposed.
            FitToolPath (Union[str, os.PathLike]):
                The FIT tool to support to do the decomposing.
            IfwiCfgFilePath (Union[str, os.PathLike]):
                Path of IFWI XML configuration file.
            CSEImagePath (Union[str, os.PathLike], optional):
                CSE image file. (Corporate IFWI is mandatory)
                Defaults to ''.

        Raises:
            None.

        Returns:
            None.
        """
        self.__IfwiPath       : str = AbsPath (IfwiPath)
        self.__FitToolPath    : str = AbsPath (FitToolPath)
        self.__IfwiCfgFilePath: str = AbsPath (IfwiCfgFilePath)
        self.__CseImagePath   : str = AbsPath (CSEImagePath)

        self.__PreCheck ()

        self.__FWUpdateImgFileName: str = FWUPDATE_IMAGE_FILENAME
        self.__FWUpdateImgPath    : str = \
            JoinPath (
              FwuGenerateApi.FWU_EXPORT_DIR_PATH,
              self.__FWUpdateImgFileName,
              )

    @property
    def Path (self) -> str:
        """ Return the path of generated CSME FWUpdate image.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Path of generated CSME FWUpdate image.
        """
        return self.__FWUpdateImgPath

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
            FwuGenerateApi.PY_FWU_BUILDER_SCRIPT_PATH,
            self.__IfwiPath,
            self.__FitToolPath,
            self.__IfwiCfgFilePath,
            ]

        CheckFileListExist (CheckPathList)

        #
        # Allow to bypass the CSE image path.
        # Since it is optional for Consumer IFWI.
        #
        CheckPathList = [
            self.__CseImagePath,
            ]
        CheckFileListExist (CheckPathList, IsBypassEmpty = True)

    def Execute (self) -> int:
        """ Call the script to generate the CSME FWUpdate image.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                The exit code of CSME FWUpdate generator script.
                (Detail exit code should be reference its own)
        """
        WorkspacePath: str = FwuGenerateApi.FWU_WORKSPACE_DIR_PATH
        ExportPath   : str = FwuGenerateApi.FWU_EXPORT_DIR_PATH
        ExitCode     : int = int ()
        Platform     : str = FwuGenerateApi.LAYOUT_PLATFORM

        TouchDir (WorkspacePath)
        TouchDir (ExportPath)

        CmdList: List[str] = [
            f'{FwuGenerateApi.PY_FWU_BUILDER_SCRIPT_PATH}',
            '-i', f'{self.__IfwiPath}',
            '-f', f'{self.__FitToolPath}',
            '-c', f'{self.__IfwiCfgFilePath}',
            '-d', f'{WorkspacePath}',
            '-o', f'{self.__FWUpdateImgPath}',

            #
            # If input CSE image, need to assign the -cse flag.
            #
            '-cse' if self.__CseImagePath else '',
            f'{self.__CseImagePath}' if self.__CseImagePath else '',

            #
            # If need to override the platform config, need to assign
            # the -p flag.
            #
            '-p' if Platform else '',
            f'{Platform}' if Platform else '',
            ]

        ExitCode, _, _ = ExecPythonCmd (CmdList, IsException = False)
        if (ExitCode != STATUS_SUCCESS):
            raise ErrorException ('Failed to generate FWUpdate image.')

        return ExitCode
